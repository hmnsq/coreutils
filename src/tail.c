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

#ಅನ್ತರ್ಭಾವ <config.h>

#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <signal.h>

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "argmatch.h"
#ಅನ್ತರ್ಭಾವ "assure.h"
#ಅನ್ತರ್ಭಾವ "c-ctype.h"
#ಅನ್ತರ್ಭಾವ "cl-strtod.h"
#ಅನ್ತರ್ಭಾವ "dtimespec-bound.h"
#ಅನ್ತರ್ಭಾವ "fcntl--.h"
#ಅನ್ತರ್ಭಾವ "iopoll.h"
#ಅನ್ತರ್ಭಾವ "isapipe.h"
#ಅನ್ತರ್ಭಾವ "issymlink.h"
#ಅನ್ತರ್ಭಾವ "posixver.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "stat-size.h"
#ಅನ್ತರ್ಭಾವ "stat-time.h"
#ಅನ್ತರ್ಭಾವ "xbinary-io.h"
#ಅನ್ತರ್ಭಾವ "xdectoint.h"
#ಅನ್ತರ್ಭಾವ "xnanosleep.h"

#ಆದರೆ HAVE_INOTIFY
# include "hash.h"
# include <poll.h>
# include <sys/inotify.h>
#ಆದರೆ_ಅಂತ್ಯ

/* Linux can optimize the handling of local files.  */
#ಆದರೆ defined __linux__ || defined __ANDROID__
# include "fs.h"
# include "fs-is-local.h"
# ಆದರೆ HAVE_SYS_STATFS_H
#  include <sys/statfs.h>
# elif HAVE_SYS_VFS_H
#  include <sys/vfs.h>
# endif
#ಆದರೆ_ಅಂತ್ಯ

/* The official name of this program (e.g., no 'g' prefix).  */
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "tail"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS \
  proper_name ("Paul Rubin"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Ian Lance Taylor"), \
  proper_name ("Jim Meyering")

/* Number of items to tail.  */
#ವ್ಯಾಖ್ಯಾನಿಸು DEFAULT_N_ಸಾಲುಗಳು 10

/* Special values for dump_remainder's N_BYTES parameter.  */
ಎಣಿಕೆ { COPY_ಗಮ್ಯ_EOF = -1, COPY_A_ಬಫರ್ = -2 };

/* The type of a large counter.  Although it is always nonnegative,
   it is signed to help signed overflow checking catch any bugs.  */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ intmax_t ಎಣಿಕೆ_t;
#ವ್ಯಾಖ್ಯಾನಿಸು ಎಣಿಕೆ_MAX INTMAX_MAX

/* FIXME: make Follow_name the default?  */
#ವ್ಯಾಖ್ಯಾನಿಸು DEFAULT_FOLLOW_ಬಹುಲಕ Follow_descriptor

ಎಣಿಕೆ Follow_ಬಹುಲಕ
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
#ವ್ಯಾಖ್ಯಾನಿಸು IS_TAILABLE_FILE_TYPE(ಬಹುಲಕ_3) \
  (S_ISREG (ಬಹುಲಕ_3) || S_ISFIFO (ಬಹುಲಕ_3) || S_ISSOCK (ಬಹುಲಕ_3) || S_ISCHR (ಬಹುಲಕ_3))

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸ್ಥಿರಾಂಕ follow_ಬಹುಲಕ_string[] =
{
  "descriptor", "name", ಖಾಲಿ
};

ಸ್ಥಿರ ಎಣಿಕೆ Follow_ಬಹುಲಕ ಸ್ಥಿರಾಂಕ follow_ಬಹುಲಕ_map[] =
{
  Follow_descriptor, Follow_name,
};

ರಚನೆ File_spec
{
  /* The actual file name, or "-" for stdin.  */
  ಅಕ್ಷರ *name;

  /* The prettified name; the same as NAME unless NAME is "-".  */
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *prettyname;

  /* Attributes of the file the last time we checked.  */
  ರಚನೆ timespec mtime;
  dev_t st_dev;
  ino_t st_ino;
  mode_t ಬಹುಲಕ_2;

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
  ಪೂರ್ಣಾಂಕ fd;

  /* 0 if the file's last op succeeded, otherwise the op's errno if
     applicable, otherwise -1.  */
  ಪೂರ್ಣಾಂಕ errnum;

  /* 1 if O_NONBLOCK is clear, 0 if set, -1 if not known.  */
  ಪೂರ್ಣಾಂಕ blocking;

#ಆದರೆ HAVE_INOTIFY
  /* The watch descriptor used by inotify.  */
  ಪೂರ್ಣಾಂಕ wd;

  /* The parent directory watch descriptor.  It is used only
   * when Follow_name is used.  */
  ಪೂರ್ಣಾಂಕ ಪೋಷಕ_wd;

  /* Offset in NAME of the basename part.  */
  idx_t basename_ಆರಂಭ;
#ಆದರೆ_ಅಂತ್ಯ

  /* See description of DEFAULT_MAX_N_... below.  */
  ಎಣಿಕೆ_t n_unchanged_stats;
};

/* Keep trying to open a file even if it is inaccessible when tail starts
   or if it becomes inaccessible later -- useful only with -f.  */
ಸ್ಥಿರ bool reopen_inaccessible_files;

/* If true, interpret the numeric argument as the number of lines.
   Otherwise, interpret it as the number of bytes.  */
ಸ್ಥಿರ bool ಎಣಿಕೆ_ಸಾಲುಗಳು = true;

/* Whether we follow the name of each file or the file descriptor
   that is initially associated with each name.  */
ಸ್ಥಿರ ಎಣಿಕೆ Follow_ಬಹುಲಕ follow_ಬಹುಲಕ = Follow_descriptor;

/* If true, read from the ends of all specified files until killed.  */
ಸ್ಥಿರ bool forever;

/* If true, monitor output so we exit if pipe reader terminates.  */
ಸ್ಥಿರ bool monitor_output;

/* If true, count from start of file instead of end.  */
ಸ್ಥಿರ bool from_ಆರಂಭ;

/* If true, print filename headers.  */
ಸ್ಥಿರ bool print_ಶೀರ್ಷಿಕೆಗಳು;

/* Character to split lines by. */
ಸ್ಥಿರ ಅಕ್ಷರ ಸಾಲು_ಕೊನೆ = '\n';

/* When to print the filename banners.  */
ಎಣಿಕೆ ಶೀರ್ಷಿಕೆ_ಬಹುಲಕ
{
  ಅಪವರ್ತ್ಯ_files, always, never
};

/* When tailing a file by name, if there have been this many consecutive
   iterations for which the file has not changed, then open/fstat
   the file to determine if that file name is still associated with the
   same device/inode-number pair as before.  This option is meaningful only
   when following by name.  --max-unchanged-stats=N  */
ಎಣಿಕೆ { DEFAULT_MAX_N_UNCHANGED_STATS_BETWEEN_OPENS = 5 };
ಸ್ಥಿರ ಎಣಿಕೆ_t max_n_unchanged_stats_between_opens =
  DEFAULT_MAX_N_UNCHANGED_STATS_BETWEEN_OPENS;

/* The process IDs of the processes to watch (those writing the followed
   files, or perhaps other processes the user cares about).  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ nbpids = 0;
ಸ್ಥಿರ pid_t * pids = ಖಾಲಿ;
ಸ್ಥಿರ idx_t pids_alloc;

/* Used to determine the buffer size when scanning backwards in a file.  */
ಸ್ಥಿರ idx_t ಪುಟ_ಗಾತ್ರ;

/* True if we have ever read standard input.  */
ಸ್ಥಿರ bool have_read_stdin;

/* If nonzero, skip the is-regular-file test used to determine whether
   to use the lseek optimization.  Instead, use the more general (and
   more expensive) code unconditionally. Intended solely for testing.  */
ಸ್ಥಿರ bool presume_ಒಳಹರಿವು_pipe;

/* If nonzero then don't use inotify even if available.  */
ಸ್ಥಿರ bool disable_inotify;

/* Annotate the output with extra info to aid the user.  */
ಸ್ಥಿರ bool debug;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ಎಣಿಕೆ
{
  RETRY_OPTION = CHAR_MAX + 1,
  MAX_UNCHANGED_STATS_OPTION,
  PID_OPTION,
  PRESUME_ಒಳಹರಿವು_PIPE_OPTION,
  LONG_FOLLOW_OPTION,
  DISABLE_INOTIFY_OPTION,
  DEBUG_ಕಾರ್ಯಕ್ರಮ_OPTION,
};

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ long_options[] =
{
  {"bytes", required_ವಾದ, ಖಾಲಿ, 'c'},
  {"debug", no_ವಾದ, ಖಾಲಿ, DEBUG_ಕಾರ್ಯಕ್ರಮ_OPTION},
  {"follow", optional_ವಾದ, ಖಾಲಿ, LONG_FOLLOW_OPTION},
  {"lines", required_ವಾದ, ಖಾಲಿ, 'n'},
  {"max-unchanged-stats", required_ವಾದ, ಖಾಲಿ,
   MAX_UNCHANGED_STATS_OPTION},
  {"-disable-inotify", no_ವಾದ, ಖಾಲಿ,
   DISABLE_INOTIFY_OPTION}, /* do not document */
  {"pid", required_ವಾದ, ಖಾಲಿ, PID_OPTION},
  {"-presume-input-pipe", no_ವಾದ, ಖಾಲಿ,
   PRESUME_ಒಳಹರಿವು_PIPE_OPTION}, /* do not document */
  {"quiet", no_ವಾದ, ಖಾಲಿ, 'q'},
  {"retry", no_ವಾದ, ಖಾಲಿ, RETRY_OPTION},
  {"silent", no_ವಾದ, ಖಾಲಿ, 'q'},
  {"sleep-interval", required_ವಾದ, ಖಾಲಿ, 's'},
  {"verbose", no_ವಾದ, ಖಾಲಿ, 'v'},
  {"zero-terminated", no_ವಾದ, ಖಾಲಿ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              ಕಾರ್ಯಕ್ರಮ_name);
      ಬರೆ (_("\
Print the ಕೊನೆಯ %d ಸಾಲುಗಳು ದ each FILE ಗಮ್ಯ standard output.\n\
With ಇನ್ನಷ್ಟು than one FILE, precede each with a ಶೀರ್ಷಿಕೆ giving the file name.\n\
"), DEFAULT_N_ಸಾಲುಗಳು);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

     oputs (_("\
  -c, --ಬೈಟುಗಳು=[+]ಅಂಕೆ_2\n\
         output the ಕೊನೆಯ ಅಂಕೆ_2 ಬೈಟುಗಳು;\n\
         or use -c +ಅಂಕೆ_2 ಗಮ್ಯ output starting with byte ಅಂಕೆ_2 ದ each file\n\
"));
     oputs (_("\
      --debug\n\
         indicate which --follow implementation is ಬಳಸಿದ\n\
"));
     oputs (_("\
  -f, --follow[={name|descriptor}]\n\
         output appended ದತ್ತಾಂಶ as the file grows;\n\
         an absent option ವಾದ means 'descriptor'\n\
"));
     oputs (_("\
  -F\n\
         same as --follow=name --retry\n\
"));
     oprintf (_("\
  -n, --ಸಾಲುಗಳು=[+]ಅಂಕೆ_2\n\
         output the ಕೊನೆಯ ಅಂಕೆ_2 ಸಾಲುಗಳು, instead ದ the ಕೊನೆಯ %d;\n\
         or use -n +ಅಂಕೆ_2 ಗಮ್ಯ skip ಅಂಕೆ_2-1 ಸಾಲುಗಳು at the ಆರಂಭ\n\
"), DEFAULT_N_ಸಾಲುಗಳು);
     oprintf (_("\
      --max-unchanged-stats=N\n\
         with --follow=name, reopen a FILE which has not\n\
         ಬದಲಾಯಿತು ಗಾತ್ರ_2 after N (ಪೂರ್ವನಿಯೋಜಿತ %d) iterations\n\
         ಗಮ್ಯ see ಆದರೆ it has been unlinked or renamed\n\
         (this is the usual ಸ್ಥಿತಿ ದ rotated ಲಘುಗಣಕ files);\n\
         with inotify, this option is rarely useful\n\
"), DEFAULT_MAX_N_UNCHANGED_STATS_BETWEEN_OPENS);
     oputs (_("\
      --pid=PID\n\
         with -f, ನಿರ್ಗಮಿಸು after PID no longer exists;\n\
         can be repeated ಗಮ್ಯ watch ಅಪವರ್ತ್ಯ processes\n\
"));
     oputs (_("\
  -q, --quiet, --silent\n\
         never output ಶೀರ್ಷಿಕೆಗಳು giving file ಹೆಸರುಗಳು\n\
"));
     oputs (_("\
      --retry\n\
         keep trying ಗಮ್ಯ open a file ಆದರೆ it is inaccessible\n\
"));
     oputs (_("\
  -s, --sleep-interval=N\n\
         with -f, sleep ಗಾಗಿ approximately N seconds\n\
         (ಪೂರ್ವನಿಯೋಜಿತ 1.0) between iterations;\n\
         with inotify and --pid=P,\n\
         check process P at ಕನಿಷ್ಠ once every N seconds\n\
"));
     oputs (_("\
  -v, --verbose\n\
         always output ಶೀರ್ಷಿಕೆಗಳು giving file ಹೆಸರುಗಳು\n\
"));
     oputs (_("\
  -z, --zero-terminated\n\
         ಸಾಲು delimiter is NUL, not newline\n\
"));
     oputs (HELP_OPTION_DESCRIPTION);
     oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
     fputs (_("\
\n\
ಅಂಕೆ_2 may have a multiplier suffix:\n\
b 512, kB 1000, K 1024, MB 1000*1000, M 1024*1024,\n\
GB 1000*1000*1000, G 1024*1024*1024, and so on ಗಾಗಿ T, P, E, Z, Y, R, Q.\n\
ದ್ವಿಮಾನ_2 prefixes can be ಬಳಸಿದ, too: KiB=K, MiB=M, and so on.\n\
\n\
"), stdout);
     fputs (_("\
With --follow (-f), ಬಾಲ defaults ಗಮ್ಯ following the file descriptor, which\n\
means that even ಆದರೆ a ಬಾಲ'ed file is renamed, ಬಾಲ will ಮುಂದುವರಿ ಗಮ್ಯ track\n\
its ಕೊನೆ.  This ಪೂರ್ವನಿಯೋಜಿತ behavior is not desirable when you really want ಗಮ್ಯ\n\
track the actual name ದ the file, not the file descriptor (e.g., ಲಘುಗಣಕ\n\
rotation).  Use --follow=name ಒಳ that ಸ್ಥಿತಿ.  That causes ಬಾಲ ಗಮ್ಯ track the\n\
named file ಒಳ a way that accommodates renaming, removal and creation.\n\
"), stdout);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

/* Ensure exit, either with SIGPIPE or EXIT_FAILURE status.  */
ಸ್ಥಿರ ಶೂನ್ಯ
die_pipe (ಶೂನ್ಯ)
{
  ಎತ್ತು (SIGPIPE);
  ನಿರ್ಗಮಿಸು (EXIT_FAILURE);
}

/* If the output has gone away, then terminate
   as we would if we had written to this output.  */
ಸ್ಥಿರ ಶೂನ್ಯ
check_output_alive (ಶೂನ್ಯ)
{
  ಆದರೆ (! monitor_output)
    ಹಿಂತಿರುಗಿ;

  ಆದರೆ (iopoll (-1, STDOUT_FILENO, false) == IOPOLL_BROKEN_OUTPUT)
    die_pipe ();
}

MAYBE_ಬಳಸದ ಸ್ಥಿರ bool
valid_file_spec (ರಚನೆ File_spec ಸ್ಥಿರಾಂಕ *f)
{
  /* Exactly one of the following subexpressions must be true. */
  ಹಿಂತಿರುಗಿ (f->fd < 0) ^ (f->errnum == 0);
}

/* Call lseek with the specified arguments FD, OFFSET, WHENCE.
   FD corresponds to PRETTYNAME.
   Give a diagnostic and exit nonzero if lseek fails.
   Otherwise, return the resulting offset.  */

ಸ್ಥಿರ off_t
xlseek (ಪೂರ್ಣಾಂಕ fd, off_t ಸ್ಥಾನಾಂತರ, ಪೂರ್ಣಾಂಕ whence, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *prettyname)
{
  off_t new_ಸ್ಥಾನಾಂತರ = lseek (fd, ಸ್ಥಾನಾಂತರ, whence);

  ಆದರೆ (0 <= new_ಸ್ಥಾನಾಂತರ)
    ಹಿಂತಿರುಗಿ new_ಸ್ಥಾನಾಂತರ;

  ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *whence_msgid[] = {
    [SEEK_SET] = N_("%s: cannot seek to offset %jd"),
    [SEEK_CUR] = N_("%s: cannot seek to relative offset %jd"),
    [SEEK_END] = N_("%s: cannot seek to end-relative offset %jd")
  };
  intmax_t joffset = ಸ್ಥಾನಾಂತರ;
  error (EXIT_FAILURE, errno, gettext (whence_msgid[whence]),
         quotef (prettyname), joffset);
}

/* Record a file F with descriptor FD, read position READ_POS, status ST,
   and blocking status BLOCKING.  READ_POS matters only for regular files,
   and READ_POS < 0 means the position is unknown.  */

ಸ್ಥಿರ ಶೂನ್ಯ
ದಾಖಲೆ_open_fd (ರಚನೆ File_spec *f, ಪೂರ್ಣಾಂಕ fd,
                off_t read_pos, ರಚನೆ stat ಸ್ಥಿರಾಂಕ *st,
                ಪೂರ್ಣಾಂಕ blocking)
{
  f->fd = fd;
  f->mtime = get_stat_mtime (st);
  f->st_dev = st->st_dev;
  f->st_ino = st->st_ino;
  f->ಬಹುಲಕ_2 = st->st_ಬಹುಲಕ;
  ಆದರೆ (S_ISREG (st->st_ಬಹುಲಕ))
    f->read_pos = (read_pos < 0
                   ? xlseek (fd, 0, SEEK_CUR, f->prettyname)
                   : read_pos);
  f->blocking = blocking;
  f->n_unchanged_stats = 0;
  f->ignore = false;
}

/* Close the file with descriptor FD and spec F.  */

ಸ್ಥಿರ ಶೂನ್ಯ
close_fd (ಪೂರ್ಣಾಂಕ fd, ರಚನೆ File_spec ಸ್ಥಿರಾಂಕ *f)
{
  ಆದರೆ (STDIN_FILENO < fd && close (fd) < 0)
    error (0, errno, _("closing %s (fd=%d)"), quoteaf (f->prettyname), fd);
}

ಸ್ಥಿರ ಶೂನ್ಯ
write_ಶೀರ್ಷಿಕೆ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *prettyname)
{
  ಸ್ಥಿರ bool ಮೊದಲ_file = true;

  ಬರೆ ("%s==> %s <==\n", ಮೊದಲ_file ? "" : "\n", quotef (prettyname));
  ಮೊದಲ_file = false;
}

/* Write N_BYTES from BUFFER to stdout.
   Exit immediately on error with a single diagnostic.  */

ಸ್ಥಿರ ಶೂನ್ಯ
xwrite_stdout (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಬಫರ್, idx_t n_ಬೈಟುಗಳು)
{
  ಆದರೆ (n_ಬೈಟುಗಳು > 0 && fwrite (ಬಫರ್, 1, n_ಬೈಟುಗಳು, stdout) < n_ಬೈಟುಗಳು)
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

ಸ್ಥಿರ ಎಣಿಕೆ_t
dump_remainder (bool want_ಶೀರ್ಷಿಕೆ, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *prettyname, ಪೂರ್ಣಾಂಕ fd,
                ಎಣಿಕೆ_t n_ಬೈಟುಗಳು)
{
  ಎಣಿಕೆ_t n_read = 0;
  ಎಣಿಕೆ_t n_ಉಳಿದ = n_ಬೈಟುಗಳು;

  ಮಾಡು
    {
      ಅಕ್ಷರ ಬಫರ್[BUFSIZ];
      idx_t n = n_ಬೈಟುಗಳು < 0 || BUFSIZ < n_ಉಳಿದ ? BUFSIZ : n_ಉಳಿದ;
      ssize_t ಬೈಟುಗಳು_read = ಪಠ (fd, ಬಫರ್, n);
      ಆದರೆ (ಬೈಟುಗಳು_read < 0)
        {
          ಆದರೆ (errno != EAGAIN)
            error (EXIT_FAILURE, errno, _("error reading %s"),
                   quoteaf (prettyname));
          ನಿಲ್ಲಿಸು;
        }
      ಆದರೆ (ಬೈಟುಗಳು_read == 0)
        ನಿಲ್ಲಿಸು;
      n_read += ಬೈಟುಗಳು_read;

      ಆದರೆ (want_ಶೀರ್ಷಿಕೆ)
        {
          write_ಶೀರ್ಷಿಕೆ (prettyname);
          want_ಶೀರ್ಷಿಕೆ = false;
        }
      xwrite_stdout (ಬಫರ್, ಬೈಟುಗಳು_read);
      ಆದರೆ (n_ಬೈಟುಗಳು == COPY_A_ಬಫರ್)
        ನಿಲ್ಲಿಸು;

      n_ಉಳಿದ -= ಬೈಟುಗಳು_read;
      /* n_remaining < 0 if n_bytes == COPY_TO_EOF.  */
    }
  ಆಗ (n_ಉಳಿದ != 0);

  ಹಿಂತಿರುಗಿ n_read;
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

ಸ್ಥಿರ off_t
file_ಸಾಲುಗಳು (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *prettyname, ಪೂರ್ಣಾಂಕ fd, ರಚನೆ stat ಸ್ಥಿರಾಂಕ *sb,
            ಎಣಿಕೆ_t n_ಸಾಲುಗಳು, off_t ಆರಂಭ_pos, off_t ಕೊನೆ_pos)
{
  ಅಕ್ಷರ *ಬಫರ್;
  idx_t bufsize = BUFSIZ;
  off_t pos = ಕೊನೆ_pos;

  ಆದರೆ (n_ಸಾಲುಗಳು == 0)
    ಹಿಂತಿರುಗಿ pos;

  /* Be careful with files with sizes that are a multiple of the page size,
     as on /proc or /sys file systems these files accept seeking to within
     the file, but then return no data when read.  So use a buffer that's
     at least PAGE_SIZE to avoid seeking within such files.

     We could also indirectly use a large enough buffer through io_blksize()
     however this would be less efficient in the common case, as it would
     generally pick a larger buffer size, resulting in reading more data
     from the end of the file.  */
  affirm (S_ISREG (sb->st_ಬಹುಲಕ));
  ಆದರೆ (sb->st_ಗಾತ್ರ % ಪುಟ_ಗಾತ್ರ == 0)
    bufsize = MAX (BUFSIZ, ಪುಟ_ಗಾತ್ರ);

  ಬಫರ್ = ximalloc (bufsize);

  /* Set 'bytes_read' to the size of the last, probably partial, buffer;
     0 < 'bytes_read' <= 'bufsize'.  */
  idx_t ಬೈಟುಗಳು_ಗಮ್ಯ_read = (pos - ಆರಂಭ_pos) % bufsize;
  ಆದರೆ (ಬೈಟುಗಳು_ಗಮ್ಯ_read == 0)
    ಬೈಟುಗಳು_ಗಮ್ಯ_read = bufsize;
  /* Make 'pos' a multiple of 'bufsize' (0 if the file is short), so that all
     reads will be on block boundaries, which might increase efficiency.  */
  pos = xlseek (fd, -ಬೈಟುಗಳು_ಗಮ್ಯ_read, SEEK_CUR, prettyname);
  ssize_t ಬೈಟುಗಳು_read = ಪಠ (fd, ಬಫರ್, ಬೈಟುಗಳು_ಗಮ್ಯ_read);
  ಆದರೆ (ಬೈಟುಗಳು_read < 0)
    {
      pos = -1 - errno;
      error (0, errno, _("error reading %s"), quoteaf (prettyname));
      ಹೋಗು free_ಬಫರ್;
    }

  /* Count the incomplete line on files that don't end with a newline.  */
  ಆದರೆ (ಬೈಟುಗಳು_read && ಬಫರ್[ಬೈಟುಗಳು_read - 1] != ಸಾಲು_ಕೊನೆ)
    --n_ಸಾಲುಗಳು;

  ಮಾಡು
    {
      pos += ಬೈಟುಗಳು_read;

      /* Scan backward, counting the newlines in this bufferfull.  */

      idx_t n = ಬೈಟುಗಳು_read;
      ಆಗ (n)
        {
          ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *nl;
          nl = memrchr (ಬಫರ್, ಸಾಲು_ಕೊನೆ, n);
          ಆದರೆ (nl == ಖಾಲಿ)
            ನಿಲ್ಲಿಸು;
          n = nl - ಬಫರ್;
          ಆದರೆ (n_ಸಾಲುಗಳು-- == 0)
            {
              /* If this newline isn't the last character in the buffer,
                 output the part that is after it.  */
              xwrite_stdout (nl + 1, ಬೈಟುಗಳು_read - (n + 1));
              pos += dump_remainder (false, prettyname, fd, ಕೊನೆ_pos - pos);
              ಹೋಗು free_ಬಫರ್;
            }
        }

      /* Not enough newlines in that bufferfull.  */
      ಆದರೆ (pos - ಬೈಟುಗಳು_read == ಆರಂಭ_pos)
        {
          /* Not enough lines in the file; print everything from
             start_pos to the end.  */
          xwrite_stdout (ಬಫರ್, ಬೈಟುಗಳು_read);
          dump_remainder (false, prettyname, fd, ಕೊನೆ_pos - pos);
          ಹೋಗು free_ಬಫರ್;
        }

      pos = xlseek (fd, -(bufsize + ಬೈಟುಗಳು_read), SEEK_CUR, prettyname);
      ಬೈಟುಗಳು_read = ಪಠ (fd, ಬಫರ್, bufsize);
      ಆದರೆ (ಬೈಟುಗಳು_read < 0)
        {
          pos = -1 - errno;
          error (0, errno, _("error reading %s"), quoteaf (prettyname));
          ಹೋಗು free_ಬಫರ್;
        }
    }
  ಆಗ (ಬೈಟುಗಳು_read > 0);

free_ಬಫರ್:
  ಬಿಡುಗಡೆ (ಬಫರ್);
  ಹಿಂತಿರುಗಿ pos;
}

/* Print the last N_LINES lines from the end of the standard input,
   open for reading as pipe FD.
   Buffer the text as a linked list of LBUFFERs, adding them as needed.
   Return -1 if successful, (-1 - errno) otherwise.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
pipe_ಸಾಲುಗಳು (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *prettyname, ಪೂರ್ಣಾಂಕ fd, ಎಣಿಕೆ_t n_ಸಾಲುಗಳು)
{
  ರಚನೆ linebuffer
  {
    ಅಕ್ಷರ ಬಫರ್[BUFSIZ];
    idx_t nbytes;
    idx_t nlines;
    ರಚನೆ linebuffer *ಮುಂದಿನ;
  };
  ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ linebuffer LBUFFER;
  LBUFFER *ಮೊದಲ, *ಕೊನೆಯ, *tmp;
  idx_t ಒಟ್ಟು_ಸಾಲುಗಳು = 0;	/* Total number of newlines in all buffers.  */
  ಪೂರ್ಣಾಂಕ ret = -1;
  ssize_t n_read;		/* Size in bytes of most recent read */

  ಮೊದಲ = ಕೊನೆಯ = xmalloc (ಗಾತ್ರ (LBUFFER));
  ಮೊದಲ->nbytes = ಮೊದಲ->nlines = 0;
  ಮೊದಲ->ಮುಂದಿನ = ಖಾಲಿ;
  tmp = xmalloc (ಗಾತ್ರ (LBUFFER));

  /* Input is always read into a fresh buffer.  */
  ಆಗ (true)
    {
      n_read = ಪಠ (fd, tmp->ಬಫರ್, BUFSIZ);
      ಆದರೆ (n_read <= 0)
        ನಿಲ್ಲಿಸು;
      tmp->nbytes = n_read;
      tmp->nlines = 0;
      tmp->ಮುಂದಿನ = ಖಾಲಿ;

      /* Count the number of newlines just read.  */
      {
        ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಬಫರ್_ಕೊನೆ = tmp->ಬಫರ್ + n_read;
        ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *p = tmp->ಬಫರ್;
        ಆಗ ((p = memchr (p, ಸಾಲು_ಕೊನೆ, ಬಫರ್_ಕೊನೆ - p)))
          {
            ++p;
            ++tmp->nlines;
          }
      }
      ಒಟ್ಟು_ಸಾಲುಗಳು += tmp->nlines;

      /* If there is enough room in the last buffer read, just append the new
         one to it.  This is because when reading from a pipe, 'n_read' can
         often be very small.  */
      ಆದರೆ (tmp->nbytes + ಕೊನೆಯ->nbytes < BUFSIZ)
        {
          ಸ್ಮರಣೆ_ನಕಲು (&ಕೊನೆಯ->ಬಫರ್[ಕೊನೆಯ->nbytes], tmp->ಬಫರ್, tmp->nbytes);
          ಕೊನೆಯ->nbytes += tmp->nbytes;
          ಕೊನೆಯ->nlines += tmp->nlines;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          /* If there's not enough room, link the new buffer onto the end of
             the list, then either free up the oldest buffer for the next
             read if that would leave enough lines, or else malloc a new one.
             Some compaction mechanism is possible but probably not
             worthwhile.  */
          ಕೊನೆಯ = ಕೊನೆಯ->ಮುಂದಿನ = tmp;
          ಆದರೆ (ಒಟ್ಟು_ಸಾಲುಗಳು - ಮೊದಲ->nlines > n_ಸಾಲುಗಳು)
            {
              tmp = ಮೊದಲ;
              ಒಟ್ಟು_ಸಾಲುಗಳು -= ಮೊದಲ->nlines;
              ಮೊದಲ = ಮೊದಲ->ಮುಂದಿನ;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            tmp = xmalloc (ಗಾತ್ರ (LBUFFER));
        }
    }

  ಬಿಡುಗಡೆ (tmp);

  ಆದರೆ (n_read < 0 && errno != EAGAIN)
    {
      ret = -1 - errno;
      error (0, errno, _("error reading %s"), quoteaf (prettyname));
      ಹೋಗು free_lbuffers;
    }

  /* If the file is empty, then bail out.  */
  ಆದರೆ (ಕೊನೆಯ->nbytes == 0)
    ಹೋಗು free_lbuffers;

  /* This prevents a core dump when the pipe contains no newlines.  */
  ಆದರೆ (n_ಸಾಲುಗಳು == 0)
    ಹೋಗು free_lbuffers;

  /* Count the incomplete line on files that don't end with a newline.  */
  ಆದರೆ (ಕೊನೆಯ->ಬಫರ್[ಕೊನೆಯ->nbytes - 1] != ಸಾಲು_ಕೊನೆ)
    {
      ++ಕೊನೆಯ->nlines;
      ++ಒಟ್ಟು_ಸಾಲುಗಳು;
    }

  /* Run through the list, printing lines.  First, skip over unneeded
     buffers.  */
  ಗಾಗಿ (tmp = ಮೊದಲ; ಒಟ್ಟು_ಸಾಲುಗಳು - tmp->nlines > n_ಸಾಲುಗಳು; tmp = tmp->ಮುಂದಿನ)
    ಒಟ್ಟು_ಸಾಲುಗಳು -= tmp->nlines;

  /* Find the correct beginning, then print the rest of the file.  */
  {
    ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *beg = tmp->ಬಫರ್;
    ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಬಫರ್_ಕೊನೆ = tmp->ಬಫರ್ + tmp->nbytes;
    ಆದರೆ (ಒಟ್ಟು_ಸಾಲುಗಳು > n_ಸಾಲುಗಳು)
      {
        /* Skip 'total_lines' - 'n_lines' newlines.  We made sure that
           'total_lines' - 'n_lines' <= 'tmp->nlines'.  */
        ಗಾಗಿ (idx_t j = ಒಟ್ಟು_ಸಾಲುಗಳು - n_ಸಾಲುಗಳು; j; --j)
          {
            beg = rawmemchr (beg, ಸಾಲು_ಕೊನೆ);
            ++beg;
          }
      }

    xwrite_stdout (beg, ಬಫರ್_ಕೊನೆ - beg);
  }

  ಗಾಗಿ (tmp = tmp->ಮುಂದಿನ; tmp; tmp = tmp->ಮುಂದಿನ)
    xwrite_stdout (tmp->ಬಫರ್, tmp->nbytes);

free_lbuffers:
  ಆಗ (ಮೊದಲ)
    {
      tmp = ಮೊದಲ->ಮುಂದಿನ;
      ಬಿಡುಗಡೆ (ಮೊದಲ);
      ಮೊದಲ = tmp;
    }
  ಹಿಂತಿರುಗಿ ret;
}

/* Print the last N_BYTES characters from the end of FD.
   Work even if the input is a pipe.
   This is a stripped down version of pipe_lines.
   The initial file offset is READ_POS if nonnegative, otherwise unknown.
   Return (-1 - errno) on failure, otherwise the resulting file offset
   if known, otherwise -1.  */

ಸ್ಥಿರ off_t
pipe_ಬೈಟುಗಳು (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *prettyname, ಪೂರ್ಣಾಂಕ fd, ಎಣಿಕೆ_t n_ಬೈಟುಗಳು,
            off_t read_pos)
{
  ರಚನೆ charbuffer
  {
    ಅಕ್ಷರ ಬಫರ್[BUFSIZ];
    idx_t nbytes;
    ರಚನೆ charbuffer *ಮುಂದಿನ;
  };
  ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ charbuffer CBUFFER;
  CBUFFER *ಮೊದಲ, *ಕೊನೆಯ, *tmp;
  idx_t i;			/* Index into buffers.  */
  intmax_t ಒಟ್ಟು_ಬೈಟುಗಳು = 0;	/* Total characters in all buffers.  */
  ssize_t n_read;

  ಆದರೆ (read_pos < 0)
    read_pos = TYPE_MINIMUM (off_t);

  ಮೊದಲ = ಕೊನೆಯ = xmalloc (ಗಾತ್ರ (CBUFFER));
  ಮೊದಲ->nbytes = 0;
  ಮೊದಲ->ಮುಂದಿನ = ಖಾಲಿ;
  tmp = xmalloc (ಗಾತ್ರ (CBUFFER));

  /* Input is always read into a fresh buffer.  */
  ಆಗ (true)
    {
      n_read = ಪಠ (fd, tmp->ಬಫರ್, BUFSIZ);
      ಆದರೆ (n_read <= 0)
        ನಿಲ್ಲಿಸು;
      read_pos += n_read;
      tmp->nbytes = n_read;
      tmp->ಮುಂದಿನ = ಖಾಲಿ;

      ಒಟ್ಟು_ಬೈಟುಗಳು += tmp->nbytes;
      /* If there is enough room in the last buffer read, just append the new
         one to it.  This is because when reading from a pipe, 'nbytes' can
         often be very small.  */
      ಆದರೆ (tmp->nbytes + ಕೊನೆಯ->nbytes < BUFSIZ)
        {
          ಸ್ಮರಣೆ_ನಕಲು (&ಕೊನೆಯ->ಬಫರ್[ಕೊನೆಯ->nbytes], tmp->ಬಫರ್, tmp->nbytes);
          ಕೊನೆಯ->nbytes += tmp->nbytes;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          /* If there's not enough room, link the new buffer onto the end of
             the list, then either free up the oldest buffer for the next
             read if that would leave enough characters, or else malloc a new
             one.  Some compaction mechanism is possible but probably not
             worthwhile.  */
          ಕೊನೆಯ = ಕೊನೆಯ->ಮುಂದಿನ = tmp;
          ಆದರೆ (ಒಟ್ಟು_ಬೈಟುಗಳು - ಮೊದಲ->nbytes > n_ಬೈಟುಗಳು)
            {
              tmp = ಮೊದಲ;
              ಒಟ್ಟು_ಬೈಟುಗಳು -= ಮೊದಲ->nbytes;
              ಮೊದಲ = ಮೊದಲ->ಮುಂದಿನ;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              tmp = xmalloc (ಗಾತ್ರ (CBUFFER));
            }
        }
    }

  ಬಿಡುಗಡೆ (tmp);

  ಆದರೆ (n_read < 0 && errno != EAGAIN)
    {
      read_pos = -1 - errno;
      error (0, errno, _("error reading %s"), quoteaf (prettyname));
      ಹೋಗು free_cbuffers;
    }

  /* Run through the list, printing characters.  First, skip over unneeded
     buffers.  */
  ಗಾಗಿ (tmp = ಮೊದಲ; ಒಟ್ಟು_ಬೈಟುಗಳು - tmp->nbytes > n_ಬೈಟುಗಳು; tmp = tmp->ಮುಂದಿನ)
    ಒಟ್ಟು_ಬೈಟುಗಳು -= tmp->nbytes;

  /* Find the correct beginning, then print the rest of the file.
     We made sure that 'total_bytes' - 'n_bytes' <= 'tmp->nbytes'.  */
  ಆದರೆ (ಒಟ್ಟು_ಬೈಟುಗಳು > n_ಬೈಟುಗಳು)
    i = ಒಟ್ಟು_ಬೈಟುಗಳು - n_ಬೈಟುಗಳು;
  ಇಲ್ಲದಿದ್ದರೆ
    i = 0;
  xwrite_stdout (&tmp->ಬಫರ್[i], tmp->nbytes - i);

  ಗಾಗಿ (tmp = tmp->ಮುಂದಿನ; tmp; tmp = tmp->ಮುಂದಿನ)
    xwrite_stdout (tmp->ಬಫರ್, tmp->nbytes);

  ಆದರೆ (read_pos < 0)
    read_pos = -1;

free_cbuffers:
  ಆಗ (ಮೊದಲ)
    {
      tmp = ಮೊದಲ->ಮುಂದಿನ;
      ಬಿಡುಗಡೆ (ಮೊದಲ);
      ಮೊದಲ = tmp;
    }
  ಹಿಂತಿರುಗಿ read_pos;
}

/* Skip N_BYTES characters from the start of pipe FD, and print
   any extra characters that were read beyond that.
   Return (-1 - errno) on failure, -1 if EOF, 0 otherwise.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ಆರಂಭ_ಬೈಟುಗಳು (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *prettyname, ಪೂರ್ಣಾಂಕ fd, ಎಣಿಕೆ_t n_ಬೈಟುಗಳು)
{
  ಅಕ್ಷರ ಬಫರ್[BUFSIZ];

  ಆಗ (0 < n_ಬೈಟುಗಳು)
    {
      ssize_t ಬೈಟುಗಳು_read = ಪಠ (fd, ಬಫರ್, BUFSIZ);
      ಆದರೆ (ಬೈಟುಗಳು_read == 0)
        ಹಿಂತಿರುಗಿ -1;
      ಆದರೆ (ಬೈಟುಗಳು_read < 0)
        {
          ಪೂರ್ಣಾಂಕ ret = -1 - errno;
          error (0, errno, _("error reading %s"), quoteaf (prettyname));
          ಹಿಂತಿರುಗಿ ret;
        }
      ಆದರೆ (ಬೈಟುಗಳು_read <= n_ಬೈಟುಗಳು)
        n_ಬೈಟುಗಳು -= ಬೈಟುಗಳು_read;
      ಇಲ್ಲದಿದ್ದರೆ
        {
          xwrite_stdout (&ಬಫರ್[n_ಬೈಟುಗಳು], ಬೈಟುಗಳು_read - n_ಬೈಟುಗಳು);
          ನಿಲ್ಲಿಸು;
        }
    }

  ಹಿಂತಿರುಗಿ 0;
}

/* Skip N_LINES lines at the start of file or pipe FD, and print
   any extra bytes that were read beyond that.
   Return (-1 - errno) on failure, -1 if EOF, 0 otherwise.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ಆರಂಭ_ಸಾಲುಗಳು (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *prettyname, ಪೂರ್ಣಾಂಕ fd, ಎಣಿಕೆ_t n_ಸಾಲುಗಳು)
{
  ಆದರೆ (n_ಸಾಲುಗಳು == 0)
    ಹಿಂತಿರುಗಿ 0;

  ಆಗ (true)
    {
      ಅಕ್ಷರ ಬಫರ್[BUFSIZ];
      ssize_t ಬೈಟುಗಳು_read = ಪಠ (fd, ಬಫರ್, BUFSIZ);
      ಆದರೆ (ಬೈಟುಗಳು_read == 0) /* EOF */
        ಹಿಂತಿರುಗಿ -1;
      ಆದರೆ (ಬೈಟುಗಳು_read < 0) /* error */
        {
          ಪೂರ್ಣಾಂಕ ret = -1 - errno;
          error (0, errno, _("error reading %s"), quoteaf (prettyname));
          ಹಿಂತಿರುಗಿ ret;
        }

      ಅಕ್ಷರ *ಬಫರ್_ಕೊನೆ = ಬಫರ್ + ಬೈಟುಗಳು_read;
      ಅಕ್ಷರ *p = ಬಫರ್;
      ಆಗ ((p = memchr (p, ಸಾಲು_ಕೊನೆ, ಬಫರ್_ಕೊನೆ - p)))
        {
          ++p;
          ಆದರೆ (--n_ಸಾಲುಗಳು == 0)
            {
              ಆದರೆ (p < ಬಫರ್_ಕೊನೆ)
                xwrite_stdout (p, ಬಫರ್_ಕೊನೆ - p);
              ಹಿಂತಿರುಗಿ 0;
            }
        }
    }
}

/* Return true if FD might be open on a file residing on a remote file system,
   false if FD is known to be local.  F is the file's spec.
   If fstatfs fails, give a diagnostic and return true.
   If fstatfs cannot be called, return true.  */
ಸ್ಥಿರ bool
fremote (ಪೂರ್ಣಾಂಕ fd, ರಚನೆ File_spec ಸ್ಥಿರಾಂಕ *f)
{
  bool remote = true;           /* be conservative (poll by default).  */

#ಆದರೆ HAVE_FSTATFS && HAVE_STRUCT_STATFS_F_TYPE \
 && (defined __linux__ || defined __ANDROID__)
  ರಚನೆ statfs buf;
  ಪೂರ್ಣಾಂಕ err = fstatfs (fd, &buf);
  ಆದರೆ (err != 0)
    {
      /* On at least linux-2.6.38, fstatfs fails with ENOSYS when FD
         is open on a pipe.  Treat that like a remote file.  */
      ಆದರೆ (errno != ENOSYS)
        error (0, errno, _("cannot determine location of %s. "
                           "reverting to polling"), quoteaf (f->prettyname));
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      /* Treat unrecognized file systems as "remote", so caller polls.
         Note README-release has instructions for syncing the internal
         list with the latest Linux kernel file system constants.  */
      remote = is_local_fs_type (buf.f_type) <= 0;
    }
#ಆದರೆ_ಅಂತ್ಯ

  ಹಿಂತಿರುಗಿ remote;
}

/* open/fstat F->name and handle changes.  */
ಸ್ಥಿರ ಶೂನ್ಯ
recheck (ರಚನೆ File_spec *f, bool blocking)
{
  ರಚನೆ stat new_stats;
  bool ok = false;
  bool is_stdin = (streq (f->name, "-"));
  ಪೂರ್ಣಾಂಕ prev_errnum = f->errnum;
  bool new_file;
  ಪೂರ್ಣಾಂಕ fd = (is_stdin
            ? STDIN_FILENO
            : open (f->name, O_RDONLY | (blocking ? 0 : O_NONBLOCK)));
  ಪೂರ್ಣಾಂಕ open_errno = fd < 0 ? errno : 0;

  affirm (valid_file_spec (f));

  ಆದರೆ (! disable_inotify && issymlink (f->name) == 1)
    {
      /* Diagnose the edge case where a regular file is changed
         to a symlink.  We avoid inotify with symlinks since
         it's awkward to match between symlink name and target.  */
      f->errnum = -1;
      f->ignore = true;

      error (0, 0, _("%s has been replaced with an untailable symbolic link"),
             quoteaf (f->prettyname));
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (fd < 0 || fstat (fd, &new_stats) < 0)
    {
      f->errnum = fd < 0 ? open_errno : errno;
      ಆದರೆ (fd < 0)
        {
          ಆದರೆ (f->tailable)
            {
              /* FIXME-maybe: detect the case in which the file first becomes
                 unreadable (perms), and later becomes readable again and can
                 be seen to be the same file (dev/ino).  Otherwise, tail prints
                 the entire contents of the file when it becomes readable.  */
              error (0, f->errnum, _("%s has become inaccessible"),
                     quoteaf (f->prettyname));
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              /* say nothing... it's still not tailable */
            }
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (prev_errnum != f->errnum)
        error (0, f->errnum, "%s", quotef (f->prettyname));
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!IS_TAILABLE_FILE_TYPE (new_stats.st_ಬಹುಲಕ))
    {
      f->errnum = -1;
      f->ignore = ! (reopen_inaccessible_files && follow_ಬಹುಲಕ == Follow_name);
      ಆದರೆ (f->tailable || ! (prev_errnum < 0 || prev_errnum == EISDIR))
        error (0, 0, _("%s has been replaced with an untailable file%s"),
               quoteaf (f->prettyname),
               f->ignore ? _("; giving up on this name") : "");
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ((f->remote = fremote (fd, f)) && ! disable_inotify)
    {
      f->errnum = -1;
      error (0, 0, _("%s has been replaced with an untailable remote file"),
             quoteaf (f->prettyname));
      f->ignore = true;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ok = true;
      f->errnum = 0;
    }

  f->tailable = ok;

  new_file = false;
  ಆದರೆ (!ok)
    {
      close_fd (fd, f);
      close_fd (f->fd, f);
      f->fd = -1;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (prev_errnum && prev_errnum != ENOENT)
    {
      new_file = true;
      affirm (f->fd < 0);
      error (0, 0, _("%s has become accessible"), quoteaf (f->prettyname));
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (f->fd < 0)
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
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!SAME_INODE (*f, new_stats))
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
  ಇಲ್ಲದಿದ್ದರೆ
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
  ಆದರೆ (new_file)
    {
      /* Start at the file's current position, normally the beginning.  */
      ದಾಖಲೆ_open_fd (f, fd, -1, &new_stats, is_stdin ? -1 : blocking);
    }
}

/* Return true if any of the N_FILES files in F are live, i.e., have
   open file descriptors, or should be checked again (see --retry).
   When following descriptors, checking should only continue when any
   of the files is not yet ignored.  */

ಸ್ಥಿರ bool
any_ಜೀವಂತ_files (ಸ್ಥಿರಾಂಕ ರಚನೆ File_spec *f, ಪೂರ್ಣಾಂಕ n_files)
{
  /* In inotify mode, ignore may be set for files
     which may later be replaced with new files.
     So always consider files live in -F mode.  */
  ಆದರೆ (reopen_inaccessible_files && follow_ಬಹುಲಕ == Follow_name)
    ಹಿಂತಿರುಗಿ true;

  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < n_files; i++)
    {
      ಆದರೆ (0 <= f[i].fd)
        ಹಿಂತಿರುಗಿ true;
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಆದರೆ (! f[i].ignore && reopen_inaccessible_files)
            ಹಿಂತಿರುಗಿ true;
        }
    }

  ಹಿಂತಿರುಗಿ false;
}

/* Return true if any writers exist, even as zombies.  */

ಸ್ಥಿರ bool
some_writers_exist (ಶೂನ್ಯ)
{
  /* To avoid some (though not all) races if the kernel reuses PIDs,
     check all PIDs instead of returning merely because one PID exists.  */
  ಗಾಗಿ (idx_t i = 0; i < nbpids; )
    {
      ಆದರೆ (kill (pids[i], 0) < 0 && errno == ESRCH)
        {
          nbpids--;
          memmove (&pids[i], &pids[i + 1], (nbpids - i) * ಗಾತ್ರ pids[i]);
        }
      ಇಲ್ಲದಿದ್ದರೆ
        i++;
    }


  ಹಿಂತಿರುಗಿ 0 < nbpids;
}

/* Tail N_FILES files forever, or until killed.
   The pertinent information for each file is stored in an entry of F.
   Loop over each of them, doing an fstat to see if they have changed size,
   and an occasional open/fstat to see if any dev/ino pair has changed.
   If none of them have changed size in one iteration, sleep for a
   while and try again.  Continue until the user interrupts us.  */

ಸ್ಥಿರ ಶೂನ್ಯ
ಬಾಲ_forever (ರಚನೆ File_spec *f, ಪೂರ್ಣಾಂಕ n_files, ದ್ವಿಗುಣ sleep_interval)
{
  ಪೂರ್ಣಾಂಕ ಕೊನೆಯ = n_files - 1;

  ಸ್ಥಿರ bool debugged;

  /* Flush initial output, in case reading blocks below.  */
  ಆದರೆ (fflush (stdout) < 0)
    write_error ();

  ಆಗ (true)
    {
      /* Use blocking I/O as an optimization, when it's easy.  */
      bool blocking = (!nbpids && follow_ಬಹುಲಕ == Follow_descriptor
                       && n_files == 1 && 0 <= f[0].fd && !S_ISREG (f[0].ಬಹುಲಕ_2));

      ಆದರೆ (debug && !debugged)
        {
          debugged = true;
          error (0, 0, "%s", blocking
                             ? _("using blocking mode")
                             : _("using polling mode"));
        }

      bool any_ಒಳಹರಿವು = false;

      ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < n_files; i++)
        {
          ರಚನೆ stat stats;

          ಆದರೆ (f[i].ignore)
            ಮುಂದುವರಿ;

          ಪೂರ್ಣಾಂಕ fd = f[i].fd;
          ಆದರೆ (fd < 0)
            {
              recheck (&f[i], blocking);
              ಮುಂದುವರಿ;
            }

          ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *prettyname = f[i].prettyname;
          mode_t ಬಹುಲಕ_2 = f[i].ಬಹುಲಕ_2;

          ಆದರೆ (f[i].blocking != blocking)
            {
              ಪೂರ್ಣಾಂಕ old_flags = fcntl (fd, F_GETFL);
              ಪೂರ್ಣಾಂಕ new_flags = old_flags | (blocking ? 0 : O_NONBLOCK);
              ಆದರೆ (old_flags < 0
                  || (new_flags != old_flags
                      && fcntl (fd, F_SETFL, new_flags) < 0))
                {
                  /* Don't update f[i].blocking if fcntl fails.  */
                  ಆದರೆ (S_ISREG (f[i].ಬಹುಲಕ_2) && errno == EPERM)
                    {
                      /* This happens when using tail -f on a file with
                         the append-only attribute.  */
                    }
                  ಇಲ್ಲದಿದ್ದರೆ
                    error (EXIT_FAILURE, errno,
                           _("%s: cannot change nonblocking mode"),
                           quotef (prettyname));
                }
              ಇಲ್ಲದಿದ್ದರೆ
                f[i].blocking = blocking;
            }

          bool read_unchanged = false;
          ಆದರೆ (!f[i].blocking)
            {
              ಆದರೆ (fstat (fd, &stats) < 0)
                {
                  f[i].fd = -1;
                  f[i].errnum = errno;
                  error (0, errno, "%s", quotef (prettyname));
                  close (fd); /* ignore failure */
                  ಮುಂದುವರಿ;
                }

              ಆದರೆ (f[i].ಬಹುಲಕ_2 == stats.st_ಬಹುಲಕ
                  && (! S_ISREG (stats.st_ಬಹುಲಕ)
                      || f[i].read_pos == stats.st_ಗಾತ್ರ)
                  && timespec_cmp (f[i].mtime, get_stat_mtime (&stats)) == 0)
                {
                  ಆದರೆ ((max_n_unchanged_stats_between_opens
                       <= f[i].n_unchanged_stats++)
                      && follow_ಬಹುಲಕ == Follow_name)
                    {
                      recheck (&f[i], f[i].blocking);
                      f[i].n_unchanged_stats = 0;
                    }
                  ಆದರೆ (fd != f[i].fd || S_ISREG (stats.st_ಬಹುಲಕ) || 1 < n_files)
                    ಮುಂದುವರಿ;
                  ಇಲ್ಲದಿದ್ದರೆ
                    read_unchanged = true;
                }

              affirm (fd == f[i].fd);

              /* This file has changed.  Print out what we can, and
                 then keep looping.  */

              f[i].mtime = get_stat_mtime (&stats);
              f[i].ಬಹುಲಕ_2 = stats.st_ಬಹುಲಕ;

              /* reset counter */
              ಆದರೆ (! read_unchanged)
                f[i].n_unchanged_stats = 0;

              /* XXX: This is only a heuristic, as the file may have also
                 been truncated and written to if st_size >= size
                 (in which case we ignore new data <= size).  */
              ಆದರೆ (S_ISREG (ಬಹುಲಕ_2) && stats.st_ಗಾತ್ರ < f[i].read_pos)
                {
                  error (0, 0, _("%s: file truncated"),
                         quotef (prettyname));
                  /* Assume the file was truncated to 0,
                     and therefore output all "new" data.  */
                  f[i].read_pos = xlseek (fd, 0, SEEK_SET, prettyname);
                }

              ಆದರೆ (i != ಕೊನೆಯ)
                {
                  ಆದರೆ (print_ಶೀರ್ಷಿಕೆಗಳು)
                    write_ಶೀರ್ಷಿಕೆ (prettyname);
                  ಕೊನೆಯ = i;
                }
            }

          /* Don't read more than st_size on networked file systems
             because it was seen on glusterfs at least, that st_size
             may be smaller than the data read on a _subsequent_ stat call.  */
          ಎಣಿಕೆ_t ಬೈಟುಗಳು_ಗಮ್ಯ_read;
          ಆದರೆ (f[i].blocking)
            ಬೈಟುಗಳು_ಗಮ್ಯ_read = COPY_A_ಬಫರ್;
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (S_ISREG (ಬಹುಲಕ_2) && f[i].remote)
            ಬೈಟುಗಳು_ಗಮ್ಯ_read = stats.st_ಗಾತ್ರ - f[i].read_pos;
          ಇಲ್ಲದಿದ್ದರೆ
            ಬೈಟುಗಳು_ಗಮ್ಯ_read = COPY_ಗಮ್ಯ_EOF;

          ಎಣಿಕೆ_t nr = dump_remainder (false, prettyname, fd, ಬೈಟುಗಳು_ಗಮ್ಯ_read);
          ಆದರೆ (0 < nr)
            {
              ಆದರೆ (S_ISREG (ಬಹುಲಕ_2))
                f[i].read_pos += nr;
              ಆದರೆ (read_unchanged)
                f[i].n_unchanged_stats = 0;
              any_ಒಳಹರಿವು = true;
            }
        }

      ಆದರೆ (! any_ಜೀವಂತ_files (f, n_files))
        {
          error (EXIT_FAILURE, 0, _("no files remaining"));
          ನಿಲ್ಲಿಸು;
        }

      ಆದರೆ ((!any_ಒಳಹರಿವು || blocking) && fflush (stdout) < 0)
        write_error ();

      check_output_alive ();

      ಆದರೆ (!any_ಒಳಹರಿವು)
        {
          ಆದರೆ (pids)
            {
              /* Exit loop if it was already known that no writers exist.
                 Otherwise, to avoid a race, loop one more time
                 without sleeping if no writers exist now.  */
              ಆದರೆ (!nbpids)
                ನಿಲ್ಲಿಸು;
              ಆದರೆ (!some_writers_exist ())
                ಮುಂದುವರಿ;
            }

          ಆದರೆ (xnanosleep (sleep_interval))
            error (EXIT_FAILURE, errno, _("cannot read realtime clock"));
        }
    }
}

#ಆದರೆ HAVE_INOTIFY

/* Return true if any of the N_FILES files in F is remote, i.e., has
   an open file descriptor and is on a network file system.  */

ಸ್ಥಿರ bool
any_remote_file (ಸ್ಥಿರಾಂಕ ರಚನೆ File_spec *f, ಪೂರ್ಣಾಂಕ n_files)
{
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < n_files; i++)
    ಆದರೆ (0 <= f[i].fd && f[i].remote)
      ಹಿಂತಿರುಗಿ true;
  ಹಿಂತಿರುಗಿ false;
}

/* Return true if any of the N_FILES files in F is non remote, i.e., has
   an open file descriptor and is not on a network file system.  */

ಸ್ಥಿರ bool
any_non_remote_file (ಸ್ಥಿರಾಂಕ ರಚನೆ File_spec *f, ಪೂರ್ಣಾಂಕ n_files)
{
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < n_files; i++)
    ಆದರೆ (0 <= f[i].fd && ! f[i].remote)
      ಹಿಂತಿರುಗಿ true;
  ಹಿಂತಿರುಗಿ false;
}

/* Return true if any of the N_FILES files in F is a symlink.
   Note we don't worry about the edge case where "-" exists,
   since that will have the same consequences for inotify,
   which is the only context this function is currently used.  */

ಸ್ಥಿರ bool
any_symlinks (ಸ್ಥಿರಾಂಕ ರಚನೆ File_spec *f, ಪೂರ್ಣಾಂಕ n_files)
{
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < n_files; i++)
    ಆದರೆ (issymlink (f[i].name) == 1)
      ಹಿಂತಿರುಗಿ true;
  ಹಿಂತಿರುಗಿ false;
}

/* Return true if any of the N_FILES files in F is not
   a regular file or fifo.  This is used to avoid adding inotify
   watches on a device file for example, which inotify
   will accept, but not give any events for.  */

ಸ್ಥಿರ bool
any_non_regular_fifo (ಸ್ಥಿರಾಂಕ ರಚನೆ File_spec *f, ಪೂರ್ಣಾಂಕ n_files)
{
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < n_files; i++)
    ಆದರೆ (0 <= f[i].fd && ! S_ISREG (f[i].ಬಹುಲಕ_2) && ! S_ISFIFO (f[i].ಬಹುಲಕ_2))
      ಹಿಂತಿರುಗಿ true;
  ಹಿಂತಿರುಗಿ false;
}

/* Return true if any of the N_FILES files in F represents
   stdin and is not ignored.  */

ಸ್ಥಿರ bool
tailable_stdin (ಸ್ಥಿರಾಂಕ ರಚನೆ File_spec *f, ಪೂರ್ಣಾಂಕ n_files)
{
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < n_files; i++)
    ಆದರೆ (!f[i].ignore && streq (f[i].name, "-"))
      ಹಿಂತಿರುಗಿ true;
  ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ size_t
wd_hasher (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ನಮೂದು, size_t tabsize)
{
  ಸ್ಥಿರಾಂಕ ರಚನೆ File_spec *spec = ನಮೂದು;
  ಹಿಂತಿರುಗಿ spec->wd % tabsize;
}

ಸ್ಥಿರ bool
wd_comparator (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *e1, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *e2)
{
  ಸ್ಥಿರಾಂಕ ರಚನೆ File_spec *spec1 = e1;
  ಸ್ಥಿರಾಂಕ ರಚನೆ File_spec *spec2 = e2;
  ಹಿಂತಿರುಗಿ spec1->wd == spec2->wd;
}

/* Hash Follow_name watches by parent directory wd + basename so directory
   inotify events can be matched in expected O(1) instead of scanning all
   files.  */
ಸ್ಥಿರ size_t
ಪೋಷಕ_name_hasher (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ನಮೂದು, size_t tabsize)
{
  ಸ್ಥಿರಾಂಕ ರಚನೆ File_spec *spec = ನಮೂದು;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name = spec->name + spec->basename_ಆರಂಭ;
  /* hash_string already reduces modulo TABSIZE; mix in parent_wd so the
     same basename under different directories does not collide by design.  */
  ಹಿಂತಿರುಗಿ (hash_string (name, tabsize) + (size_t) spec->ಪೋಷಕ_wd) % tabsize;
}

ಸ್ಥಿರ bool
ಪೋಷಕ_name_comparator (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *e1, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *e2)
{
  ಸ್ಥಿರಾಂಕ ರಚನೆ File_spec *a = e1;
  ಸ್ಥಿರಾಂಕ ರಚನೆ File_spec *b = e2;
  ಹಿಂತಿರುಗಿ (a->ಪೋಷಕ_wd == b->ಪೋಷಕ_wd
          && streq (a->name + a->basename_ಆರಂಭ,
                    b->name + b->basename_ಆರಂಭ));
}

/* Output (new) data for FSPEC->fd.
   PREV_FSPEC records the last File_spec for which we output.  */
ಸ್ಥಿರ ಶೂನ್ಯ
check_fspec (ರಚನೆ File_spec *fspec, ರಚನೆ File_spec **prev_fspec)
{
  ಆದರೆ (fspec->fd < 0)
    ಹಿಂತಿರುಗಿ;

  ರಚನೆ stat stats;
  ಆದರೆ (fstat (fspec->fd, &stats) < 0)
    {
      fspec->errnum = errno;
      close_fd (fspec->fd, fspec);
      fspec->fd = -1;
      ಹಿಂತಿರುಗಿ;
    }

  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *prettyname = fspec->prettyname;

  /* XXX: This is only a heuristic, as the file may have also
     been truncated and written to if st_size >= size
     (in which case we ignore new data <= size).
     Though in the inotify case it's more likely we'll get
     separate events for truncate() and write().  */
  ಆದರೆ (S_ISREG (fspec->ಬಹುಲಕ_2) && stats.st_ಗಾತ್ರ < fspec->read_pos)
    {
      error (0, 0, _("%s: file truncated"), quotef (prettyname));
      fspec->read_pos = xlseek (fspec->fd, 0, SEEK_SET, prettyname);
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (S_ISREG (fspec->ಬಹುಲಕ_2) && stats.st_ಗಾತ್ರ == fspec->read_pos
           && timespec_cmp (fspec->mtime, get_stat_mtime (&stats)) == 0)
    ಹಿಂತಿರುಗಿ;

  bool want_ಶೀರ್ಷಿಕೆ = print_ಶೀರ್ಷಿಕೆಗಳು && (fspec != *prev_fspec);

  ಎಣಿಕೆ_t nr = dump_remainder (want_ಶೀರ್ಷಿಕೆ, prettyname, fspec->fd, COPY_ಗಮ್ಯ_EOF);
  ಆದರೆ (0 < nr)
    {
      ಆದರೆ (S_ISREG (fspec->ಬಹುಲಕ_2))
        fspec->read_pos += nr;
      *prev_fspec = fspec;
      ಆದರೆ (fflush (stdout) < 0)
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
ಸ್ಥಿರ ಶೂನ್ಯ
ಬಾಲ_forever_inotify (ಪೂರ್ಣಾಂಕ wd, ರಚನೆ File_spec *f, ಪೂರ್ಣಾಂಕ n_files,
                      ದ್ವಿಗುಣ sleep_interval, Hash_ಕೋಷ್ಟಕ **wd_ಗಮ್ಯ_namep,
                      Hash_ಕೋಷ್ಟಕ **ಪೋಷಕ_by_namep)
{
# ಆದರೆ ಬಾಲ_TEST_SLEEP
  /* Delay between open() and inotify_add_watch()
     to help trigger different cases.  */
  xnanosleep (1000000);
# endif
  ಪೂರ್ಣಾಂಕ max_realloc = 3;

  /* Map an inotify watch descriptor to the name of the file it's watching.  */
  Hash_ಕೋಷ್ಟಕ *wd_ಗಮ್ಯ_name;

  /* Map (parent_wd, basename) to File_spec for Follow_name
     directory events.  */
  Hash_ಕೋಷ್ಟಕ *ಪೋಷಕ_by_name = ಖಾಲಿ;

  bool ಸಿಕ್ಕಿತು_watchable_file = false;
  bool tailed_but_unwatchable = false;
  bool ಸಿಕ್ಕಿತು_unwatchable_dir = false;
  bool no_inotify_resources = false;
  ರಚನೆ File_spec *prev_fspec;
  idx_t evlen = 0;
  ಅಕ್ಷರ *evbuf;
  idx_t evbuf_off = 0;

  *ಪೋಷಕ_by_namep = ಖಾಲಿ;

  wd_ಗಮ್ಯ_name = hash_initialize (n_files, ಖಾಲಿ, wd_hasher, wd_comparator,
                                ಖಾಲಿ);
  ಆದರೆ (! wd_ಗಮ್ಯ_name)
    xalloc_die ();
  *wd_ಗಮ್ಯ_namep = wd_ಗಮ್ಯ_name;

  ಆದರೆ (follow_ಬಹುಲಕ == Follow_name)
    {
      ಪೋಷಕ_by_name = hash_initialize (n_files, ಖಾಲಿ, ಪೋಷಕ_name_hasher,
                                        ಪೋಷಕ_name_comparator, ಖಾಲಿ);
      ಆದರೆ (! ಪೋಷಕ_by_name)
        xalloc_die ();
      *ಪೋಷಕ_by_namep = ಪೋಷಕ_by_name;
    }

  /* The events mask used with inotify on files (not directories).  */
  uint32_t inotify_wd_ಮುಖವಾಡ = ಒಳ_MODIFY;
  /* TODO: Perhaps monitor these events in Follow_descriptor mode also,
     to tag reported file names with "deleted", "moved" etc.  */
  ಆದರೆ (follow_ಬಹುಲಕ == Follow_name)
    inotify_wd_ಮುಖವಾಡ |= (ಒಳ_ATTRIB | ಒಳ_DELETE_SELF | ಒಳ_MOVE_SELF);

  /* Add an inotify watch for each watched file.  If -F is specified then watch
     its parent directory too, in this way when they re-appear we can add them
     again to the watch list.  */
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < n_files; i++)
    {
      ಆದರೆ (!f[i].ignore)
        {
          idx_t fnlen = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (f[i].name);
          ಆದರೆ (evlen < fnlen)
            evlen = fnlen;

          f[i].wd = -1;

          ಆದರೆ (follow_ಬಹುಲಕ == Follow_name)
            {
              idx_t dirlen = dir_len (f[i].name);
              ಅಕ್ಷರ prev = f[i].name[dirlen];
              f[i].basename_ಆರಂಭ = ಕೊನೆಯ_component (f[i].name) - f[i].name;

              f[i].name[dirlen] = '\0';

               /* It's fine to add the same directory more than once.
                  In that case the same watch descriptor is returned.  */
              f[i].ಪೋಷಕ_wd = inotify_ಕೂಡಿಸು_watch (wd, dirlen ? f[i].name : ".",
                                                  (ಒಳ_CREATE | ಒಳ_DELETE
                                                   | ಒಳ_MOVED_ಗಮ್ಯ | ಒಳ_ATTRIB
                                                   | ಒಳ_DELETE_SELF));

              f[i].name[dirlen] = prev;

              ಆದರೆ (f[i].ಪೋಷಕ_wd < 0)
                {
                  ಆದರೆ (errno != ENOSPC) /* suppress confusing error.  */
                    error (0, errno, _("cannot watch parent directory of %s"),
                           quoteaf (f[i].name));
                  ಇಲ್ಲದಿದ್ದರೆ
                    error (0, 0, _("inotify resources exhausted"));
                  ಸಿಕ್ಕಿತು_unwatchable_dir = true;
                  /* We revert to polling below.  Note invalid uses
                     of the inotify API will still be diagnosed.  */
                  ನಿಲ್ಲಿಸು;
                }

              ಆದರೆ (hash_ಸೇರಿಸು (ಪೋಷಕ_by_name, &(f[i])) == ಖಾಲಿ)
                xalloc_die ();
            }

          f[i].wd = inotify_ಕೂಡಿಸು_watch (wd, f[i].name, inotify_wd_ಮುಖವಾಡ);

          ಆದರೆ (f[i].wd < 0)
            {
              ಆದರೆ (0 <= f[i].fd)  /* already tailed.  */
                tailed_but_unwatchable = true;
              ಆದರೆ (errno == ENOSPC || errno == ENOMEM)
                {
                  no_inotify_resources = true;
                  error (0, 0, _("inotify resources exhausted"));
                  ನಿಲ್ಲಿಸು;
                }
              ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (errno != f[i].errnum)
                error (0, errno, _("cannot watch %s"), quoteaf (f[i].name));
              ಮುಂದುವರಿ;
            }

          ಆದರೆ (hash_ಸೇರಿಸು (wd_ಗಮ್ಯ_name, &(f[i])) == ಖಾಲಿ)
            xalloc_die ();

          ಸಿಕ್ಕಿತು_watchable_file = true;
        }
    }

  /* Linux kernel 2.6.24 at least has a bug where eventually, ENOSPC is always
     returned by inotify_add_watch.  In any case we should revert to polling
     when there are no inotify resources.  Also a specified directory may not
     be currently present or accessible, so revert to polling.  Also an already
     tailed but unwatchable due rename/unlink race, should also revert.  */
  ಆದರೆ (no_inotify_resources || ಸಿಕ್ಕಿತು_unwatchable_dir
      || (follow_ಬಹುಲಕ == Follow_descriptor && tailed_but_unwatchable))
    ಹಿಂತಿರುಗಿ;
  ಆದರೆ (follow_ಬಹುಲಕ == Follow_descriptor && !ಸಿಕ್ಕಿತು_watchable_file)
    ನಿರ್ಗಮಿಸು (EXIT_FAILURE);

  prev_fspec = &(f[n_files - 1]);

  /* Check files again.  New files or data can be available since last time we
     checked and before they are watched by inotify.  */
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < n_files; i++)
    {
      ಆದರೆ (! f[i].ignore)
        {
          /* check for new files.  */
          ಆದರೆ (follow_ಬಹುಲಕ == Follow_name)
            recheck (&(f[i]), false);
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (0 <= f[i].fd)
            {
              /* If the file was replaced in the small window since we tailed,
                 then assume the watch is on the wrong item (different to
                 that we've already produced output for), and so revert to
                 polling the original descriptor.  */
              ರಚನೆ stat stats;

              ಆದರೆ (! (stat (f[i].name, &stats) < 0
                     || SAME_INODE (f[i], stats)))
                {
                  error (0, errno, _("%s was replaced"),
                         quoteaf (f[i].prettyname));
                  ಹಿಂತಿರುಗಿ;
                }
            }

          /* check for new data.  */
          check_fspec (&f[i], &prev_fspec);
        }
    }

  ಆದರೆ (debug)
    error (0, 0, "%s", _("using notification mode"));

  evlen += ಗಾತ್ರ (ರಚನೆ inotify_event) + 1;
  evbuf = ximalloc (evlen);

  /* Wait for inotify events and handle them.  Events on directories
     ensure that watched files can be re-added when following by name.
     This loop blocks on the 'read' call until a new event is notified.
     But when --pid=P is specified, tail usually waits via poll.  */
  ptrdiff_t len = 0;
  ಆಗ (true)
    {
      ರಚನೆ File_spec *fspec;
      ರಚನೆ inotify_event *ev;
      ಶೂನ್ಯ *void_ev;

      /* When following by name without --retry, and the last file has
         been unlinked or renamed-away, diagnose it and return.  */
      ಆದರೆ (follow_ಬಹುಲಕ == Follow_name
          && ! reopen_inaccessible_files
          && hash_get_n_entries (wd_ಗಮ್ಯ_name) == 0)
        error (EXIT_FAILURE, 0, _("no files remaining"));

      ಆದರೆ (len <= evbuf_off)
        {
          /* Poll for inotify events.  When watching a PID, ensure
             that a read from WD will not block indefinitely.
             If MONITOR_OUTPUT, also poll for a broken output pipe.  */

          ಪೂರ್ಣಾಂಕ file_change;
          ರಚನೆ pollfd pfd[2];
          ಮಾಡು
            {
              /* How many ms to wait for changes.  -1 means wait forever.  */
              ಪೂರ್ಣಾಂಕ delay = -1;

              ಆದರೆ (pids)
                {
                  ಆದರೆ (!nbpids)
                    ನಿರ್ಗಮಿಸು (EXIT_SUCCESS);

                  ಆದರೆ (!some_writers_exist () || sleep_interval <= 0)
                    delay = 0;
                  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (sleep_interval < ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ / 1000 - 1)
                    {
                      /* delay = ceil (sleep_interval * 1000), sans libm.  */
                      ದ್ವಿಗುಣ ddelay = sleep_interval * 1000;
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
          ಆಗ (file_change == 0 || (file_change < 0 && errno == EINTR));

          ಆದರೆ (file_change < 0)
            error (EXIT_FAILURE, errno,
                   _("error waiting for inotify and output events"));
          ಆದರೆ (pfd[1].revents)
            die_pipe ();

          len = ಪಠ (wd, evbuf, evlen);
          evbuf_off = 0;

          /* For kernels prior to 2.6.21, read returns 0 when the buffer
             is too small.  */
          ಆದರೆ ((len == 0 || (len < 0 && errno == EINVAL))
              && max_realloc--)
            {
              len = 0;
              evlen *= 2;
              evbuf = xirealloc (evbuf, evlen);
              ಮುಂದುವರಿ;
            }

          ಆದರೆ (len <= 0)
            error (EXIT_FAILURE, errno, _("error reading inotify event"));
        }

      void_ev = evbuf + evbuf_off;
      ev = void_ev;
      evbuf_off += ಗಾತ್ರ (*ev) + ev->len;

      /* If a directory is deleted, IN_DELETE_SELF is emitted
         with ev->name of length 0.
         We need to catch it, otherwise it would wait forever,
         as wd for directory becomes inactive. Revert to polling now.   */
      ಆದರೆ ((ev->ಮುಖವಾಡ & ಒಳ_DELETE_SELF) && ! ev->len)
        {
          ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < n_files; i++)
            {
              ಆದರೆ (ev->wd == f[i].ಪೋಷಕ_wd)
                {
                  error (0, 0,
                      _("directory containing watched file was removed"));
                  ಹಿಂತಿರುಗಿ;
                }
            }
        }

      ಆದರೆ (ev->len) /* event on ev->name in watched directory.  */
        {
          ರಚನೆ File_spec ಕೀಲಿ;
          ಆದರೆ (! ಪೋಷಕ_by_name)
            ಮುಂದುವರಿ;

          /* Probe key: basename is the whole string at KEY.NAME.  */
          ಕೀಲಿ.ಪೋಷಕ_wd = ev->wd;
          ಕೀಲಿ.name = ev->name;
          ಕೀಲಿ.basename_ಆರಂಭ = 0;
          fspec = hash_lookup (ಪೋಷಕ_by_name, &ಕೀಲಿ);

          /* It is not a watched file.  */
          ಆದರೆ (! fspec)
            ಮುಂದುವರಿ;

          ಪೂರ್ಣಾಂಕ new_wd = -1;
          bool deleting = !! (ev->ಮುಖವಾಡ & ಒಳ_DELETE);

          ಆದರೆ (! deleting)
            {
              /* Adding the same inode again will look up any existing wd.  */
              new_wd = inotify_ಕೂಡಿಸು_watch (wd, fspec->name, inotify_wd_ಮುಖವಾಡ);
            }

          ಆದರೆ (! deleting && new_wd < 0)
            {
              ಆದರೆ (errno == ENOSPC || errno == ENOMEM)
                {
                  error (0, 0, _("inotify resources exhausted"));
                  ಹಿಂತಿರುಗಿ; /* revert to polling.  */
                }
              ಇಲ್ಲದಿದ್ದರೆ
                {
                  /* Can get ENOENT for a dangling symlink for example.  */
                  error (0, errno, _("cannot watch %s"), quoteaf (fspec->name));
                }
              /* We'll continue below after removing the existing watch.  */
            }

          /* This will be false if only attributes of file change.  */
          bool new_watch;
          new_watch = (! deleting) && (fspec->wd < 0 || new_wd != fspec->wd);

          ಆದರೆ (new_watch)
            {
              ಆದರೆ (0 <= fspec->wd)
                {
                  inotify_rm_watch (wd, fspec->wd);
                  hash_remove (wd_ಗಮ್ಯ_name, fspec);
                }

              fspec->wd = new_wd;

              ಆದರೆ (new_wd < 0)
                ಮುಂದುವರಿ;

              /* If the file was moved then inotify will use the source file wd
                for the destination file.  Make sure the key is not present in
                the table.  */
              ರಚನೆ File_spec *prev = hash_remove (wd_ಗಮ್ಯ_name, fspec);
              ಆದರೆ (prev && prev != fspec)
                {
                  ಆದರೆ (follow_ಬಹುಲಕ == Follow_name)
                    recheck (prev, false);
                  prev->wd = -1;
                  close_fd (prev->fd, prev);
                }

              ಆದರೆ (hash_ಸೇರಿಸು (wd_ಗಮ್ಯ_name, fspec) == ಖಾಲಿ)
                xalloc_die ();
            }

          ಆದರೆ (follow_ಬಹುಲಕ == Follow_name)
            recheck (fspec, false);
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ರಚನೆ File_spec ಕೀಲಿ;
          ಕೀಲಿ.wd = ev->wd;
          fspec = hash_lookup (wd_ಗಮ್ಯ_name, &ಕೀಲಿ);
        }

      ಆದರೆ (! fspec)
        ಮುಂದುವರಿ;

      ಆದರೆ (ev->ಮುಖವಾಡ & (ಒಳ_ATTRIB | ಒಳ_DELETE | ಒಳ_DELETE_SELF | ಒಳ_MOVE_SELF))
        {
          /* Note for IN_MOVE_SELF (the file we're watching has
             been clobbered via a rename) without --retry we leave the watch
             in place since it may still be part of the set
             of watched names.  */
          ಆದರೆ (ev->ಮುಖವಾಡ & ಒಳ_DELETE_SELF
              || (!reopen_inaccessible_files && (ev->ಮುಖವಾಡ & ಒಳ_MOVE_SELF)))
            {
              inotify_rm_watch (wd, fspec->wd);
              hash_remove (wd_ಗಮ್ಯ_name, fspec);
            }

          /* Note we get IN_ATTRIB for unlink() as st_nlink decrements.
             The usual path is a close() done in recheck() triggers
             an IN_DELETE_SELF event as the inode is removed.
             However sometimes open() will succeed as even though
             st_nlink is decremented, the dentry (cache) is not updated.
             Thus we depend on the IN_DELETE event on the directory
             to trigger processing for the removed file.  */

          recheck (fspec, false);

          ಮುಂದುವರಿ;
        }
      check_fspec (fspec, &prev_fspec);
    }
}
#ಆದರೆ_ಅಂತ್ಯ

/* Output the last bytes of the file PRETTYNAME open for reading
   in FD and with status ST.  Output the last N_BYTES bytes.
   Return (-1 - errno) on failure, otherwise the resulting file offset
   if known, otherwise -1.  */

ಸ್ಥಿರ off_t
ಬಾಲ_ಬೈಟುಗಳು (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *prettyname, ಪೂರ್ಣಾಂಕ fd, ರಚನೆ stat ಸ್ಥಿರಾಂಕ *st,
            ಎಣಿಕೆ_t n_ಬೈಟುಗಳು)
{
  off_t current_pos
    = (presume_ಒಳಹರಿವು_pipe
       ? -1
       : lseek (fd, from_ಆರಂಭ ? MIN (n_ಬೈಟುಗಳು, OFF_T_MAX) : 0, SEEK_CUR));

  ಆದರೆ (from_ಆರಂಭ)
    {
      ಆದರೆ (current_pos < 0)
        {
          ಪೂರ್ಣಾಂಕ t = ಆರಂಭ_ಬೈಟುಗಳು (prettyname, fd, n_ಬೈಟುಗಳು);
          ಆದರೆ (t < 0)
            ಹಿಂತಿರುಗಿ t;
        }
      n_ಬೈಟುಗಳು = COPY_ಗಮ್ಯ_EOF;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      off_t initial_pos = current_pos;
      off_t ಕೊನೆ_pos = -1;

      ಆದರೆ (0 <= current_pos)
        {
          ಆದರೆ (usable_st_ಗಾತ್ರ (st))
            {
              /* Use st_size only if it's so large that this is
                 probably not a /proc or similar file, where st_size
                 is notional.  */
              off_t smallish_ಗಾತ್ರ = STP_BLKSIZE (st);
              ಆದರೆ (smallish_ಗಾತ್ರ < st->st_ಗಾತ್ರ)
                ಕೊನೆ_pos = st->st_ಗಾತ್ರ;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              off_t minus_n;
              ಆದರೆ (ckd_sub (&minus_n, 0, n_ಬೈಟುಗಳು))
                ಕೊನೆ_pos = 0;
              ಇಲ್ಲದಿದ್ದರೆ
                {
                  off_t e_n_pos = lseek (fd, minus_n, SEEK_END);
                  ಆದರೆ (0 <= e_n_pos)
                    {
                      current_pos = e_n_pos;
                      ಕೊನೆ_pos = e_n_pos + n_ಬೈಟುಗಳು;
                    }
                  ಇಲ್ಲದಿದ್ದರೆ
                    {
                      /* POSIX says errno is unreliable here.
                         Perhaps the file is smaller than N_BYTES.
                         Try again at file end.  */
                      off_t e_pos = lseek (fd, 0, SEEK_END);
                      ಆದರೆ (0 <= e_pos)
                        current_pos = ಕೊನೆ_pos = e_pos;
                    }
               }
            }
        }

      /* If the end is known and more than N_BYTES after the initial
         position, go to N_BYTES before the end; otherwise go back to
         the initial position.  But do not lseek if lseek already failed.  */
      off_t pos = (initial_pos < ಕೊನೆ_pos && n_ಬೈಟುಗಳು < ಕೊನೆ_pos - initial_pos
                   ? ಕೊನೆ_pos - n_ಬೈಟುಗಳು
                   : initial_pos);
      ಆದರೆ (pos != current_pos)
        current_pos = xlseek (fd, pos, SEEK_SET, prettyname);

      ಆದರೆ (ಕೊನೆ_pos < 0)
        ಹಿಂತಿರುಗಿ pipe_ಬೈಟುಗಳು (prettyname, fd, n_ಬೈಟುಗಳು, current_pos);
    }

  ಎಣಿಕೆ_t nr = dump_remainder (false, prettyname, fd, n_ಬೈಟುಗಳು);
  ಹಿಂತಿರುಗಿ current_pos < 0 ? -1 : current_pos + nr;
}

/* Output the last lines of the file PRETTYNAME open for reading
   in FD and with status ST.  Output the last N_LINES lines.
   Return (-1 - errno) on failure, otherwise the resulting file offset
   if known, otherwise -1.  */

ಸ್ಥಿರ off_t
ಬಾಲ_ಸಾಲುಗಳು (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *prettyname, ಪೂರ್ಣಾಂಕ fd, ರಚನೆ stat ಸ್ಥಿರಾಂಕ *st,
            ಎಣಿಕೆ_t n_ಸಾಲುಗಳು)
{
  ಆದರೆ (from_ಆರಂಭ)
    {
      /* If skipping all input use lseek if possible, for speed.  */
      ಆದರೆ (OFF_T_MAX <= n_ಸಾಲುಗಳು)
        {
          off_t e = lseek (fd, 0, SEEK_END);
          ಆದರೆ (0 <= e)
            ಹಿಂತಿರುಗಿ e;
        }

      ಪೂರ್ಣಾಂಕ t = ಆರಂಭ_ಸಾಲುಗಳು (prettyname, fd, n_ಸಾಲುಗಳು);
      ಆದರೆ (t < 0)
        ಹಿಂತಿರುಗಿ t;
      dump_remainder (false, prettyname, fd, COPY_ಗಮ್ಯ_EOF);
      ಹಿಂತಿರುಗಿ -1;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      /* Use file_lines only if FD is a regular file and SEEK_END
         reports more data.  */
      off_t ಆರಂಭ_pos = (!presume_ಒಳಹರಿವು_pipe && S_ISREG (st->st_ಬಹುಲಕ)
                         ? lseek (fd, 0, SEEK_CUR)
                         : -1);
      off_t ಕೊನೆ_pos = ಆರಂಭ_pos < 0 ? -1 : lseek (fd, 0, SEEK_END);
      ಹಿಂತಿರುಗಿ (ಕೊನೆ_pos < 0
              ? pipe_ಸಾಲುಗಳು (prettyname, fd, n_ಸಾಲುಗಳು)
              : ಆರಂಭ_pos < ಕೊನೆ_pos
              ? file_ಸಾಲುಗಳು (prettyname, fd, st, n_ಸಾಲುಗಳು, ಆರಂಭ_pos, ಕೊನೆ_pos)
              : ಆರಂಭ_pos != ಕೊನೆ_pos
              ? (/* Do not read from before the start offset,
                    even if the input file shrank.  */
                 xlseek (fd, ಆರಂಭ_pos, SEEK_SET, prettyname))
              : ಆರಂಭ_pos);
    }
}

/* Display the last part of file FILENAME,
   open for reading via FD and with status *ST.
   Return (-1 - errno) on failure, otherwise the resulting file offset
   if known, otherwise -1.  */

ಸ್ಥಿರ off_t
ಬಾಲ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename, ಪೂರ್ಣಾಂಕ fd, ರಚನೆ stat ಸ್ಥಿರಾಂಕ *st, ಎಣಿಕೆ_t n_units)
{
  ಹಿಂತಿರುಗಿ (ಎಣಿಕೆ_ಸಾಲುಗಳು ? ಬಾಲ_ಸಾಲುಗಳು : ಬಾಲ_ಬೈಟುಗಳು) (filename, fd, st, n_units);
}

/* Display the last N_UNITS units of the file described by F.
   Return true if successful.  */

ಸ್ಥಿರ bool
ಬಾಲ_file (ರಚನೆ File_spec *f, ಎಣಿಕೆ_t n_files, ಎಣಿಕೆ_t n_units)
{
  ಪೂರ್ಣಾಂಕ fd;
  bool ok;

  /* Avoid blocking if we may need to process asynchronously.  */
  bool nonblocking = forever && (nbpids || n_files > 1);

  bool is_stdin = (streq (f->name, "-"));

  ಆದರೆ (is_stdin)
    {
      have_read_stdin = true;
      fd = STDIN_FILENO;
      xset_ದ್ವಿಮಾನ_ಬಹುಲಕ (STDIN_FILENO, O_ದ್ವಿಮಾನ);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    fd = open (f->name, O_RDONLY | O_ದ್ವಿಮಾನ | (nonblocking ? O_NONBLOCK : 0));

  f->tailable = false;

  ಆದರೆ (fd < 0)
    {
      ಆದರೆ (forever)
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
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಆದರೆ (print_ಶೀರ್ಷಿಕೆಗಳು)
        write_ಶೀರ್ಷಿಕೆ (f->prettyname);

      off_t read_pos;
      ರಚನೆ stat stats;
      ok = 0 <= fstat (fd, &stats);
      ಆದರೆ (!ok)
        {
          f->errnum = errno;
          error (0, f->errnum, _("cannot fstat %s"), quoteaf (f->prettyname));
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          read_pos = ಬಾಲ (f->prettyname, fd, &stats, n_units);
          ok = -1 <= read_pos;
          f->errnum = ok ? 0 : -1 - read_pos;

          ಆದರೆ (IS_TAILABLE_FILE_TYPE (stats.st_ಬಹುಲಕ))
            f->tailable = true;
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (forever)
            {
              ok = false;
              f->errnum = -1;
              error (0, 0, _("%s: cannot follow end of this type of file%s"),
                     quotef (f->prettyname),
                     (reopen_inaccessible_files ? ""
                      : _("; giving up on this name")));
            }
        }

      ಆದರೆ (forever)
        {
          ಆದರೆ (!ok)
            {
              f->ignore = ! reopen_inaccessible_files;
              close_fd (fd, f);
              f->fd = -1;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              ದಾಖಲೆ_open_fd (f, fd, read_pos, &stats, (is_stdin ? -1 : 1));
              f->remote = fremote (fd, f);
            }
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಆದರೆ (!is_stdin && close (fd) < 0)
            {
              error (0, errno, _("error reading %s"),
                     quoteaf (f->prettyname));
              ok = false;
            }
        }
    }

  ಹಿಂತಿರುಗಿ ok;
}

/* If obsolete usage is allowed, and the command line arguments are of
   the obsolete form and the option string is well-formed, set
   *N_UNITS, the globals COUNT_LINES, FOREVER, and FROM_START, and
   return true.  If the command line arguments are obviously incorrect
   (e.g., because obsolete usage is not allowed and the arguments are
   incorrect for non-obsolete usage), report an error and exit.
   Otherwise, return false and don't modify any parameter or global
   variable.  */

ಸ್ಥಿರ bool
parse_obsolete_option (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ *argv, ಎಣಿಕೆ_t *n_units)
{
  bool t_from_ಆರಂಭ;
  bool t_ಎಣಿಕೆ_ಸಾಲುಗಳು = true;
  bool t_forever = false;

  /* With the obsolete form, there is one option string and at most
     one file argument.  Watch out for "-" and "--", though.  */
  ಆದರೆ (! (argc == 2
         || (argc == 3 && ! (argv[2][0] == '-' && argv[2][1]))
         || (3 <= argc && argc <= 4 && streq (argv[2], "--"))))
    ಹಿಂತಿರುಗಿ false;

  ಪೂರ್ಣಾಂಕ posix_ver = posix2_ಆವೃತ್ತಿ ();
  bool obsolete_usage = posix_ver < 200112;
  bool traditional_usage = obsolete_usage || 200809 <= posix_ver;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *p = argv[1];

  ಆಯ್ಕೆ (*p++)
    {
    ಪೂರ್ವನಿಯೋಜಿತ:
      ಹಿಂತಿರುಗಿ false;

    ಸ್ಥಿತಿ '+':
      /* Leading "+" is a file name in the standard form.  */
      ಆದರೆ (!traditional_usage)
        ಹಿಂತಿರುಗಿ false;

      t_from_ಆರಂಭ = true;
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ '-':
      /* In the non-obsolete form, "-" is standard input and "-c"
         requires an option-argument.  The obsolete multidigit options
         are supported as a GNU extension even when conforming to
         POSIX 1003.1-2001 or later, so don't complain about them.  */
      ಆದರೆ (!obsolete_usage && !p[p[0] == 'c'])
        ಹಿಂತಿರುಗಿ false;

      t_from_ಆರಂಭ = false;
      ನಿಲ್ಲಿಸು;
    }

  ಎಣಿಕೆ_t n;
  ಆದರೆ (!c_isdigit (*p))
    n = DEFAULT_N_ಸಾಲುಗಳು;
  ಇಲ್ಲದಿದ್ದರೆ
    ಗಾಗಿ (n = 0; c_isdigit (*p); p++)
      n = ckd_mul (&n, n, 10) || ckd_ಕೂಡಿಸು (&n, n, *p - '0') ? ಎಣಿಕೆ_MAX : n;

  ಆಯ್ಕೆ (*p)
    {
    ಸ್ಥಿತಿ 'b': n = ckd_mul (&n, n, 512) ? ಎಣಿಕೆ_MAX : n; FALLTHROUGH;
    ಸ್ಥಿತಿ 'c': t_ಎಣಿಕೆ_ಸಾಲುಗಳು = false; FALLTHROUGH;
    ಸ್ಥಿತಿ 'l': p++; ನಿಲ್ಲಿಸು;
    }

  ಆದರೆ (*p == 'f')
    {
      t_forever = true;
      ++p;
    }

  ಆದರೆ (*p)
    ಹಿಂತಿರುಗಿ false;

  *n_units = n;

  /* Set globals.  */
  from_ಆರಂಭ = t_from_ಆರಂಭ;
  ಎಣಿಕೆ_ಸಾಲುಗಳು = t_ಎಣಿಕೆ_ಸಾಲುಗಳು;
  forever = t_forever;

  ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ ಶೂನ್ಯ
parse_options (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv,
               ಎಣಿಕೆ_t *n_units, ಎಣಿಕೆ ಶೀರ್ಷಿಕೆ_ಬಹುಲಕ *ಶೀರ್ಷಿಕೆ_ಬಹುಲಕ,
               ದ್ವಿಗುಣ *sleep_interval)
{
  ಪೂರ್ಣಾಂಕ c;

  ಆಗ ((c = getopt_long (argc, argv, "c:n:fFqs:vz0123456789",
                           long_options, ಖಾಲಿ))
         != -1)
    {
      ಆಯ್ಕೆ (c)
        {
        ಸ್ಥಿತಿ 'F':
          forever = true;
          follow_ಬಹುಲಕ = Follow_name;
          reopen_inaccessible_files = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'c':
        ಸ್ಥಿತಿ 'n':
          ಎಣಿಕೆ_ಸಾಲುಗಳು = (c == 'n');
          ಆದರೆ (*optarg == '+')
            from_ಆರಂಭ = true;
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (*optarg == '-')
            ++optarg;

          *n_units = xnumtoimax (optarg, 10, 0, ಎಣಿಕೆ_MAX, "bkKmMGTPEZYRQ0",
                                 (ಎಣಿಕೆ_ಸಾಲುಗಳು
                                  ? _("invalid number of lines")
                                  : _("invalid number of bytes"))
                                 , 0, XTOINT_MAX_QUIET);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'f':
        ಸ್ಥಿತಿ LONG_FOLLOW_OPTION:
          forever = true;
          ಆದರೆ (optarg == ಖಾಲಿ)
            follow_ಬಹುಲಕ = DEFAULT_FOLLOW_ಬಹುಲಕ;
          ಇಲ್ಲದಿದ್ದರೆ
            follow_ಬಹುಲಕ = XARGMATCH ("--follow", optarg,
                                     follow_ಬಹುಲಕ_string, follow_ಬಹುಲಕ_map);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ RETRY_OPTION:
          reopen_inaccessible_files = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ MAX_UNCHANGED_STATS_OPTION:
          /* --max-unchanged-stats=N */
          max_n_unchanged_stats_between_opens =
            xnumtoimax (optarg, 10, 0, ಎಣಿಕೆ_MAX, "",
                        _("invalid maximum number of unchanged stats"
                          " between opens"),
                        0, XTOINT_MAX_QUIET);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ DEBUG_ಕಾರ್ಯಕ್ರಮ_OPTION:
          debug = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ DISABLE_INOTIFY_OPTION:
          disable_inotify = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ PID_OPTION:
          ಆದರೆ (nbpids == pids_alloc)
            pids = xpalloc (pids, &pids_alloc, 1,
                            MIN (ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ, IDX_MAX), ಗಾತ್ರ *pids);
          pids[nbpids++] = xdectoumax (optarg, 0, PID_T_MAX, "",
                                       _("invalid PID"), 0);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ PRESUME_ಒಳಹರಿವು_PIPE_OPTION:
          presume_ಒಳಹರಿವು_pipe = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'q':
          *ಶೀರ್ಷಿಕೆ_ಬಹುಲಕ = never;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 's':
          {
            ಅಕ್ಷರ *ep;
            errno = 0;
            ದ್ವಿಗುಣ s = cl_strtod (optarg, &ep);
            ಆದರೆ (optarg == ep || *ep || ! (0 <= s))
              error (EXIT_FAILURE, 0,
                     _("invalid number of seconds: %s"), quote (optarg));
            *sleep_interval = dtimespec_bound (s, errno);
          }
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'v':
          *ಶೀರ್ಷಿಕೆ_ಬಹುಲಕ = always;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'z':
          ಸಾಲು_ಕೊನೆ = '\0';
          ನಿಲ್ಲಿಸು;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);

        ಸ್ಥಿತಿ '0': ಸ್ಥಿತಿ '1': ಸ್ಥಿತಿ '2': ಸ್ಥಿತಿ '3': ಸ್ಥಿತಿ '4':
        ಸ್ಥಿತಿ '5': ಸ್ಥಿತಿ '6': ಸ್ಥಿತಿ '7': ಸ್ಥಿತಿ '8': ಸ್ಥಿತಿ '9':
          error (EXIT_FAILURE, 0, _("option used in invalid context -- %c"), c);

        ಪೂರ್ವನಿಯೋಜಿತ:
          usage (EXIT_FAILURE);
        }
    }

  ಆದರೆ (reopen_inaccessible_files)
    {
      ಆದರೆ (!forever)
        {
          reopen_inaccessible_files = false;
          error (0, 0, _("warning: --retry ignored; --retry is useful"
                         " only when following"));
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (follow_ಬಹುಲಕ == Follow_descriptor)
        error (0, 0, _("warning: --retry only effective for the initial open"));
    }

  ಆದರೆ (pids && (!forever || (kill (pids[0], 0) < 0 && errno == ENOSYS)))
    {
      error (0, 0,
             _(forever
               ? "warning: --pid=PID is not supported on this system"
               : ("warning: PID ignored;"
                  " --pid=PID is useful only when following")));
      ಬಿಡುಗಡೆ (pids);
      pids = ಖಾಲಿ;
    }
}

/* Mark as '.ignore'd each member of F that corresponds to a
   pipe or fifo, and return true if any are not marked.  */
ಸ್ಥಿರ bool
ignore_fifo_and_pipe (ರಚನೆ File_spec *f, ಪೂರ್ಣಾಂಕ n_files)
{
  /* When there is no FILE operand and stdin is a pipe or FIFO
     POSIX requires that tail ignore the -f option.
     Since we allow multiple FILE operands, we extend that to say: with -f,
     ignore any "-" operand that corresponds to a pipe or FIFO.  */
  bool some_viable = false;

  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < n_files; i++)
    {
      bool is_a_fifo_or_pipe =
        (streq (f[i].name, "-")
         && !f[i].ignore
         && 0 <= f[i].fd
         && (S_ISFIFO (f[i].ಬಹುಲಕ_2)
             || (HAVE_FIFO_PIPES != 1 && isapipe (f[i].fd))));
      ಆದರೆ (is_a_fifo_or_pipe)
        {
          f[i].fd = -1;
          f[i].errnum = -1;
          f[i].ignore = true;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        some_viable = true;
    }

  ಹಿಂತಿರುಗಿ some_viable;
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  ಎಣಿಕೆ ಶೀರ್ಷಿಕೆ_ಬಹುಲಕ ಶೀರ್ಷಿಕೆ_ಬಹುಲಕ = ಅಪವರ್ತ್ಯ_files;
  bool ok = true;
  /* If from_start, the number of items to skip before printing; otherwise,
     the number of items at the end of the file to print.  */
  ಎಣಿಕೆ_t n_units = DEFAULT_N_ಸಾಲುಗಳು;
  ಪೂರ್ಣಾಂಕ n_files;
  ಅಕ್ಷರ **file;
  ರಚನೆ File_spec *F;
  bool obsolete_option;

  /* The number of seconds to sleep between iterations.
     During one iteration, every file name or descriptor is checked to
     see if it has changed.  */
  ದ್ವಿಗುಣ sleep_interval = 1.0;

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  {
    ಪೂರ್ಣಾಂಕ p = getpagesize ();
    ಆದರೆ (IDX_MAX < p)
      xalloc_die ();
    ಪುಟ_ಗಾತ್ರ = p;
  }

  obsolete_option = parse_obsolete_option (argc, argv, &n_units);
  argc -= obsolete_option;
  argv += obsolete_option;
  parse_options (argc, argv, &n_units, &ಶೀರ್ಷಿಕೆ_ಬಹುಲಕ, &sleep_interval);

  /* To start printing with item N_UNITS from the start of the file, skip
     N_UNITS - 1 items.  'tail -n +0' is actually meaningless, but for Unix
     compatibility it's treated the same as 'tail -n +1'.  */
  n_units -= from_ಆರಂಭ && 0 < n_units && n_units < ಎಣಿಕೆ_MAX;

  ಆದರೆ (optind < argc)
    {
      n_files = argc - optind;
      file = argv + optind;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಸ್ಥಿರ ಅಕ್ಷರ *dummy_stdin = (ಅಕ್ಷರ *) "-";
      n_files = 1;
      file = &dummy_stdin;
    }

  {
    bool ಸಿಕ್ಕಿತು_hyphen = false;

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < n_files; i++)
      ಆದರೆ (streq (file[i], "-"))
        ಸಿಕ್ಕಿತು_hyphen = true;

    /* When following by name, there must be a name.  */
    ಆದರೆ (ಸಿಕ್ಕಿತು_hyphen && follow_ಬಹುಲಕ == Follow_name)
      error (EXIT_FAILURE, 0, _("cannot follow %s by name"), quoteaf ("-"));

    /* When following forever, and not using simple blocking, warn if
       any file is '-' as the stats() used to check for input are ineffective.
       This is only a warning, since tail's output (before a failing seek,
       and that from any non-stdin files) might still be useful.  */
    ಆದರೆ (forever && ಸಿಕ್ಕಿತು_hyphen)
      {
        ರಚನೆ stat ಒಳ_stat;
        bool blocking_stdin;
        blocking_stdin = (!nbpids && follow_ಬಹುಲಕ == Follow_descriptor
                          && n_files == 1 && ! fstat (STDIN_FILENO, &ಒಳ_stat)
                          && ! S_ISREG (ಒಳ_stat.st_ಬಹುಲಕ));

        ಆದರೆ (! blocking_stdin && isatty (STDIN_FILENO))
          error (0, 0, _("warning: following standard input"
                         " indefinitely is ineffective"));
      }
  }

  /* Don't read anything if we'll never output anything.  */
  ಆದರೆ (! forever && n_units == (from_ಆರಂಭ ? ಎಣಿಕೆ_MAX : 0))
    ಹಿಂತಿರುಗಿ EXIT_SUCCESS;

  ಆದರೆ (IDX_MAX < n_files)
    xalloc_die ();
  F = xinmalloc (n_files, ಗಾತ್ರ *F);
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < n_files; i++)
    {
      F[i].name = file[i];
      F[i].prettyname = streq (file[i], "-") ? _("standard input") : file[i];
    }

  ಆದರೆ (ಶೀರ್ಷಿಕೆ_ಬಹುಲಕ == always
      || (ಶೀರ್ಷಿಕೆ_ಬಹುಲಕ == ಅಪವರ್ತ್ಯ_files && n_files > 1))
    print_ಶೀರ್ಷಿಕೆಗಳು = true;

  xset_ದ್ವಿಮಾನ_ಬಹುಲಕ (STDOUT_FILENO, O_ದ್ವಿಮಾನ);

  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < n_files; i++)
    ok &= ಬಾಲ_file (&F[i], n_files, n_units);

  ಆದರೆ (forever && ignore_fifo_and_pipe (F, n_files))
    {
      /* If stdout is a fifo or pipe, then monitor it
         so that we exit if the reader goes away.  */
      ರಚನೆ stat ಹೊರಹೋಗು_stat;
      ಆದರೆ (fstat (STDOUT_FILENO, &ಹೊರಹೋಗು_stat) < 0)
        error (EXIT_FAILURE, errno, _("standard output"));
      monitor_output = (S_ISFIFO (ಹೊರಹೋಗು_stat.st_ಬಹುಲಕ)
                        || (HAVE_FIFO_PIPES != 1 && isapipe (STDOUT_FILENO)));

#ಆದರೆ HAVE_INOTIFY
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
      ಆದರೆ (!disable_inotify && (tailable_stdin (F, n_files)
                               || any_remote_file (F, n_files)
                               || ! any_non_remote_file (F, n_files)
                               || any_symlinks (F, n_files)
                               || any_non_regular_fifo (F, n_files)
                               || (!ok && follow_ಬಹುಲಕ == Follow_descriptor)))
        disable_inotify = true;

      ಆದರೆ (!disable_inotify)
        {
          ಪೂರ್ಣಾಂಕ wd = inotify_init ();
          ಆದರೆ (0 <= wd)
            {
              /* Flush any output from tail_file, now, since
                 tail_forever_inotify flushes only after writing,
                 not before reading.  */
              ಆದರೆ (fflush (stdout) < 0)
                write_error ();

              Hash_ಕೋಷ್ಟಕ *ht;
              Hash_ಕೋಷ್ಟಕ *ಪೋಷಕ_ht = ಖಾಲಿ;
              ಬಾಲ_forever_inotify (wd, F, n_files, sleep_interval,
                                    &ht, &ಪೋಷಕ_ht);
              hash_free (ht);
              ಆದರೆ (ಪೋಷಕ_ht)
                hash_free (ಪೋಷಕ_ht);
              close (wd);
              errno = 0;
            }
          error (0, errno, _("inotify cannot be used, reverting to polling"));
        }
#ಆದರೆ_ಅಂತ್ಯ
      disable_inotify = true;
      ಬಾಲ_forever (F, n_files, sleep_interval);
    }

  ಆದರೆ (have_read_stdin && close (STDIN_FILENO) < 0)
    error (EXIT_FAILURE, errno, "-");
  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
