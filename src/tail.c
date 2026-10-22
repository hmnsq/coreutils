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

#ضمن <config.h>

#ضمن <stdio.h>
#ضمن <getopt.h>
#ضمن <sys/types.h>
#ضمن <signal.h>

#ضمن "system.h"
#ضمن "argmatch.h"
#ضمن "assure.h"
#ضمن "c-ctype.h"
#ضمن "cl-strtod.h"
#ضمن "dtimespec-bound.h"
#ضمن "fcntl--.h"
#ضمن "iopoll.h"
#ضمن "isapipe.h"
#ضمن "issymlink.h"
#ضمن "posixver.h"
#ضمن "quote.h"
#ضمن "stat-size.h"
#ضمن "stat-time.h"
#ضمن "xbinary-io.h"
#ضمن "xdectoint.h"
#ضمن "xnanosleep.h"

#إذا HAVE_INOTIFY
# include "hash.h"
# include <poll.h>
# include <sys/inotify.h>
#د_که_پای

/* Linux can optimize the handling of local files.  */
#إذا defined __linux__ || defined __ANDROID__
# include "fs.h"
# include "fs-is-local.h"
# إذا HAVE_SYS_STATFS_H
#  include <sys/statfs.h>
# elif HAVE_SYS_VFS_H
#  include <sys/vfs.h>
# endif
#د_که_پای

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کړه پروګرام_NAME "tail"

#تعریف_کړه AUTHORS \
  proper_name ("Paul Rubin"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Ian Lance Taylor"), \
  proper_name ("Jim Meyering")

/* Number of items to tail.  */
#تعریف_کړه DEFAULT_N_کرښې 10

/* Special values for dump_remainder's N_BYTES parameter.  */
تعداد { COPY_منزل_EOF = -1, COPY_A_بفر = -2 };

/* The type of a large counter.  Although it is always nonnegative,
   it is signed to help signed overflow checking catch any bugs.  */
تعريفنوع intmax_t شمېر_t;
#تعریف_کړه شمېر_MAX INTMAX_MAX

/* FIXME: make Follow_name the default?  */
#تعریف_کړه DEFAULT_FOLLOW_ډېرپېښ Follow_descriptor

تعداد Follow_ډېرپېښ
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
#تعریف_کړه IS_TAILABLE_FILE_TYPE(ډېرپېښ_3) \
  (S_ISREG (ډېرپېښ_3) || S_ISFIFO (ډېرپېښ_3) || S_ISSOCK (ډېرپېښ_3) || S_ISCHR (ډېرپېښ_3))

ساكن حرف ثابت *ثابت follow_ډېرپېښ_string[] =
{
  "descriptor", "name", عدم
};

ساكن تعداد Follow_ډېرپېښ ثابت follow_ډېرپېښ_map[] =
{
  Follow_descriptor, Follow_name,
};

بنية File_spec
{
  /* The actual file name, or "-" for stdin.  */
  حرف *name;

  /* The prettified name; the same as NAME unless NAME is "-".  */
  حرف ثابت *prettyname;

  /* Attributes of the file the last time we checked.  */
  بنية timespec mtime;
  dev_t st_dev;
  ino_t st_ino;
  mode_t ډېرپېښ_2;

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
  صحيح fd;

  /* 0 if the file's last op succeeded, otherwise the op's errno if
     applicable, otherwise -1.  */
  صحيح errnum;

  /* 1 if O_NONBLOCK is clear, 0 if set, -1 if not known.  */
  صحيح blocking;

#إذا HAVE_INOTIFY
  /* The watch descriptor used by inotify.  */
  صحيح wd;

  /* The parent directory watch descriptor.  It is used only
   * when Follow_name is used.  */
  صحيح پلار_wd;

  /* Offset in NAME of the basename part.  */
  idx_t basename_پیل;
#د_که_پای

  /* See description of DEFAULT_MAX_N_... below.  */
  شمېر_t n_unchanged_stats;
};

/* Keep trying to open a file even if it is inaccessible when tail starts
   or if it becomes inaccessible later -- useful only with -f.  */
ساكن bool reopen_inaccessible_files;

/* If true, interpret the numeric argument as the number of lines.
   Otherwise, interpret it as the number of bytes.  */
ساكن bool شمېر_کرښې = true;

/* Whether we follow the name of each file or the file descriptor
   that is initially associated with each name.  */
ساكن تعداد Follow_ډېرپېښ follow_ډېرپېښ = Follow_descriptor;

/* If true, read from the ends of all specified files until killed.  */
ساكن bool forever;

/* If true, monitor output so we exit if pipe reader terminates.  */
ساكن bool monitor_output;

/* If true, count from start of file instead of end.  */
ساكن bool from_پیل;

/* If true, print filename headers.  */
ساكن bool print_سرلیکونه;

/* Character to split lines by. */
ساكن حرف کرښه_پای_ټکی = '\n';

/* When to print the filename banners.  */
تعداد سرلیک_ډېرپېښ
{
  مضرب_files, always, never
};

/* When tailing a file by name, if there have been this many consecutive
   iterations for which the file has not changed, then open/fstat
   the file to determine if that file name is still associated with the
   same device/inode-number pair as before.  This option is meaningful only
   when following by name.  --max-unchanged-stats=N  */
تعداد { DEFAULT_MAX_N_UNCHANGED_STATS_BETWEEN_OPENS = 5 };
ساكن شمېر_t max_n_unchanged_stats_between_opens =
  DEFAULT_MAX_N_UNCHANGED_STATS_BETWEEN_OPENS;

/* The process IDs of the processes to watch (those writing the followed
   files, or perhaps other processes the user cares about).  */
ساكن صحيح nbpids = 0;
ساكن pid_t * pids = عدم;
ساكن idx_t pids_alloc;

/* Used to determine the buffer size when scanning backwards in a file.  */
ساكن idx_t پاڼه_اندازه;

/* True if we have ever read standard input.  */
ساكن bool have_read_stdin;

/* If nonzero, skip the is-regular-file test used to determine whether
   to use the lseek optimization.  Instead, use the more general (and
   more expensive) code unconditionally. Intended solely for testing.  */
ساكن bool presume_ننوت_pipe;

/* If nonzero then don't use inotify even if available.  */
ساكن bool disable_inotify;

/* Annotate the output with extra info to aid the user.  */
ساكن bool debug;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
تعداد
{
  RETRY_OPTION = CHAR_MAX + 1,
  MAX_UNCHANGED_STATS_OPTION,
  PID_OPTION,
  PRESUME_ننوت_PIPE_OPTION,
  LONG_FOLLOW_OPTION,
  DISABLE_INOTIFY_OPTION,
  DEBUG_پروګرام_OPTION,
};

ساكن بنية option ثابت long_options[] =
{
  {"bytes", required_دلیل, عدم, 'c'},
  {"debug", no_دلیل, عدم, DEBUG_پروګرام_OPTION},
  {"follow", optional_دلیل, عدم, LONG_FOLLOW_OPTION},
  {"lines", required_دلیل, عدم, 'n'},
  {"max-unchanged-stats", required_دلیل, عدم,
   MAX_UNCHANGED_STATS_OPTION},
  {"-disable-inotify", no_دلیل, عدم,
   DISABLE_INOTIFY_OPTION}, /* do not document */
  {"pid", required_دلیل, عدم, PID_OPTION},
  {"-presume-input-pipe", no_دلیل, عدم,
   PRESUME_ننوت_PIPE_OPTION}, /* do not document */
  {"quiet", no_دلیل, عدم, 'q'},
  {"retry", no_دلیل, عدم, RETRY_OPTION},
  {"silent", no_دلیل, عدم, 'q'},
  {"sleep-interval", required_دلیل, عدم, 's'},
  {"verbose", no_دلیل, عدم, 'v'},
  {"zero-terminated", no_دلیل, عدم, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخه_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کړه (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              پروګرام_name);
      چاپ_کړه (_("\
Print the وروستی %d کرښې د each FILE منزل standard output.\n\
With نور than one FILE, precede each with a سرلیک giving the file name.\n\
"), DEFAULT_N_کرښې);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

     oputs (_("\
  -c, --بایټونه=[+]عدد_2\n\
         output the وروستی عدد_2 بایټونه;\n\
         or use -c +عدد_2 منزل output starting with byte عدد_2 د each file\n\
"));
     oputs (_("\
      --debug\n\
         indicate which --follow implementation is کارول_شوی\n\
"));
     oputs (_("\
  -f, --follow[={name|descriptor}]\n\
         output appended معلومات as the file grows;\n\
         an absent option دلیل means 'descriptor'\n\
"));
     oputs (_("\
  -F\n\
         same as --follow=name --retry\n\
"));
     oprintf (_("\
  -n, --کرښې=[+]عدد_2\n\
         output the وروستی عدد_2 کرښې, instead د the وروستی %d;\n\
         or use -n +عدد_2 منزل skip عدد_2-1 کرښې at the پیل\n\
"), DEFAULT_N_کرښې);
     oprintf (_("\
      --max-unchanged-stats=N\n\
         with --follow=name, reopen a FILE which has not\n\
         بدل_شوی اندازه after N (افتراضي %d) iterations\n\
         منزل see إذا it has been unlinked or renamed\n\
         (this is the usual حالة د rotated لوګاریتم files);\n\
         with inotify, this option is rarely useful\n\
"), DEFAULT_MAX_N_UNCHANGED_STATS_BETWEEN_OPENS);
     oputs (_("\
      --pid=PID\n\
         with -f, وتل after PID no longer exists;\n\
         can be repeated منزل watch مضرب processes\n\
"));
     oputs (_("\
  -q, --quiet, --silent\n\
         never output سرلیکونه giving file نومونه\n\
"));
     oputs (_("\
      --retry\n\
         keep trying منزل open a file إذا it is inaccessible\n\
"));
     oputs (_("\
  -s, --sleep-interval=N\n\
         with -f, sleep لكل approximately N seconds\n\
         (افتراضي 1.0) between iterations;\n\
         with inotify and --pid=P,\n\
         check process P at لږترلږه once every N seconds\n\
"));
     oputs (_("\
  -v, --verbose\n\
         always output سرلیکونه giving file نومونه\n\
"));
     oputs (_("\
  -z, --zero-terminated\n\
         کرښه delimiter is NUL, not newline\n\
"));
     oputs (HELP_OPTION_DESCRIPTION);
     oputs (نسخه_OPTION_DESCRIPTION);
     fputs (_("\
\n\
عدد_2 may have a multiplier suffix:\n\
b 512, kB 1000, K 1024, MB 1000*1000, M 1024*1024,\n\
GB 1000*1000*1000, G 1024*1024*1024, and so on لكل T, P, E, Z, Y, R, Q.\n\
دوګونی_2 prefixes can be کارول_شوی, too: KiB=K, MiB=M, and so on.\n\
\n\
"), stdout);
     fputs (_("\
With --follow (-f), لکۍ defaults منزل following the file descriptor, which\n\
means that even إذا a لکۍ'ed file is renamed, لکۍ will استمر منزل track\n\
its پای_ټکی.  This افتراضي behavior is not desirable when you really want منزل\n\
track the actual name د the file, not the file descriptor (e.g., لوګاریتم\n\
rotation).  Use --follow=name دننه that حالة.  That causes لکۍ منزل track the\n\
named file دننه a way that accommodates renaming, removal and creation.\n\
"), stdout);
      emit_ancillary_info (پروګرام_NAME);
    }
  وتل (حالت);
}

/* Ensure exit, either with SIGPIPE or EXIT_FAILURE status.  */
ساكن فراغ
die_pipe (فراغ)
{
  ارفع (SIGPIPE);
  وتل (EXIT_FAILURE);
}

/* If the output has gone away, then terminate
   as we would if we had written to this output.  */
ساكن فراغ
check_output_alive (فراغ)
{
  إذا (! monitor_output)
    أرجع;

  إذا (iopoll (-1, STDOUT_FILENO, false) == IOPOLL_BROKEN_OUTPUT)
    die_pipe ();
}

MAYBE_ناکارول_شوی ساكن bool
valid_file_spec (بنية File_spec ثابت *f)
{
  /* Exactly one of the following subexpressions must be true. */
  أرجع (f->fd < 0) ^ (f->errnum == 0);
}

/* Call lseek with the specified arguments FD, OFFSET, WHENCE.
   FD corresponds to PRETTYNAME.
   Give a diagnostic and exit nonzero if lseek fails.
   Otherwise, return the resulting offset.  */

ساكن off_t
xlseek (صحيح fd, off_t دننه_واټن, صحيح whence, حرف ثابت *prettyname)
{
  off_t new_دننه_واټن = lseek (fd, دننه_واټن, whence);

  إذا (0 <= new_دننه_واټن)
    أرجع new_دننه_واټن;

  ساكن حرف ثابت *whence_msgid[] = {
    [SEEK_SET] = N_("%s: cannot seek to offset %jd"),
    [SEEK_CUR] = N_("%s: cannot seek to relative offset %jd"),
    [SEEK_END] = N_("%s: cannot seek to end-relative offset %jd")
  };
  intmax_t joffset = دننه_واټن;
  error (EXIT_FAILURE, errno, gettext (whence_msgid[whence]),
         quotef (prettyname), joffset);
}

/* Record a file F with descriptor FD, read position READ_POS, status ST,
   and blocking status BLOCKING.  READ_POS matters only for regular files,
   and READ_POS < 0 means the position is unknown.  */

ساكن فراغ
ریکارډ_open_fd (بنية File_spec *f, صحيح fd,
                off_t read_pos, بنية stat ثابت *st,
                صحيح blocking)
{
  f->fd = fd;
  f->mtime = get_stat_mtime (st);
  f->st_dev = st->st_dev;
  f->st_ino = st->st_ino;
  f->ډېرپېښ_2 = st->st_ډېرپېښ;
  إذا (S_ISREG (st->st_ډېرپېښ))
    f->read_pos = (read_pos < 0
                   ? xlseek (fd, 0, SEEK_CUR, f->prettyname)
                   : read_pos);
  f->blocking = blocking;
  f->n_unchanged_stats = 0;
  f->ignore = false;
}

/* Close the file with descriptor FD and spec F.  */

ساكن فراغ
close_fd (صحيح fd, بنية File_spec ثابت *f)
{
  إذا (STDIN_FILENO < fd && close (fd) < 0)
    error (0, errno, _("closing %s (fd=%d)"), quoteaf (f->prettyname), fd);
}

ساكن فراغ
write_سرلیک (حرف ثابت *prettyname)
{
  ساكن bool لومړی_file = true;

  چاپ_کړه ("%s==> %s <==\n", لومړی_file ? "" : "\n", quotef (prettyname));
  لومړی_file = false;
}

/* Write N_BYTES from BUFFER to stdout.
   Exit immediately on error with a single diagnostic.  */

ساكن فراغ
xwrite_stdout (حرف ثابت *بفر, idx_t n_بایټونه)
{
  إذا (n_بایټونه > 0 && fwrite (بفر, 1, n_بایټونه, stdout) < n_بایټونه)
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

ساكن شمېر_t
dump_remainder (bool want_سرلیک, حرف ثابت *prettyname, صحيح fd,
                شمېر_t n_بایټونه)
{
  شمېر_t n_read = 0;
  شمېر_t n_پاتې_شوی = n_بایټونه;

  افعل
    {
      حرف بفر[BUFSIZ];
      idx_t n = n_بایټونه < 0 || BUFSIZ < n_پاتې_شوی ? BUFSIZ : n_پاتې_شوی;
      ssize_t بایټونه_read = اقرأ (fd, بفر, n);
      إذا (بایټونه_read < 0)
        {
          إذا (errno != EAGAIN)
            error (EXIT_FAILURE, errno, _("error reading %s"),
                   quoteaf (prettyname));
          اكسر;
        }
      إذا (بایټونه_read == 0)
        اكسر;
      n_read += بایټونه_read;

      إذا (want_سرلیک)
        {
          write_سرلیک (prettyname);
          want_سرلیک = false;
        }
      xwrite_stdout (بفر, بایټونه_read);
      إذا (n_بایټونه == COPY_A_بفر)
        اكسر;

      n_پاتې_شوی -= بایټونه_read;
      /* n_remaining < 0 if n_bytes == COPY_TO_EOF.  */
    }
  طالما (n_پاتې_شوی != 0);

  أرجع n_read;
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

ساكن off_t
file_کرښې (حرف ثابت *prettyname, صحيح fd, بنية stat ثابت *sb,
            شمېر_t n_کرښې, off_t پیل_pos, off_t پای_ټکی_pos)
{
  حرف *بفر;
  idx_t bufsize = BUFSIZ;
  off_t pos = پای_ټکی_pos;

  إذا (n_کرښې == 0)
    أرجع pos;

  /* Be careful with files with sizes that are a multiple of the page size,
     as on /proc or /sys file systems these files accept seeking to within
     the file, but then return no data when read.  So use a buffer that's
     at least PAGE_SIZE to avoid seeking within such files.

     We could also indirectly use a large enough buffer through io_blksize()
     however this would be less efficient in the common case, as it would
     generally pick a larger buffer size, resulting in reading more data
     from the end of the file.  */
  affirm (S_ISREG (sb->st_ډېرپېښ));
  إذا (sb->st_اندازه % پاڼه_اندازه == 0)
    bufsize = MAX (BUFSIZ, پاڼه_اندازه);

  بفر = ximalloc (bufsize);

  /* Set 'bytes_read' to the size of the last, probably partial, buffer;
     0 < 'bytes_read' <= 'bufsize'.  */
  idx_t بایټونه_منزل_read = (pos - پیل_pos) % bufsize;
  إذا (بایټونه_منزل_read == 0)
    بایټونه_منزل_read = bufsize;
  /* Make 'pos' a multiple of 'bufsize' (0 if the file is short), so that all
     reads will be on block boundaries, which might increase efficiency.  */
  pos = xlseek (fd, -بایټونه_منزل_read, SEEK_CUR, prettyname);
  ssize_t بایټونه_read = اقرأ (fd, بفر, بایټونه_منزل_read);
  إذا (بایټونه_read < 0)
    {
      pos = -1 - errno;
      error (0, errno, _("error reading %s"), quoteaf (prettyname));
      اذهب free_بفر;
    }

  /* Count the incomplete line on files that don't end with a newline.  */
  إذا (بایټونه_read && بفر[بایټونه_read - 1] != کرښه_پای_ټکی)
    --n_کرښې;

  افعل
    {
      pos += بایټونه_read;

      /* Scan backward, counting the newlines in this bufferfull.  */

      idx_t n = بایټونه_read;
      طالما (n)
        {
          حرف ثابت *nl;
          nl = memrchr (بفر, کرښه_پای_ټکی, n);
          إذا (nl == عدم)
            اكسر;
          n = nl - بفر;
          إذا (n_کرښې-- == 0)
            {
              /* If this newline isn't the last character in the buffer,
                 output the part that is after it.  */
              xwrite_stdout (nl + 1, بایټونه_read - (n + 1));
              pos += dump_remainder (false, prettyname, fd, پای_ټکی_pos - pos);
              اذهب free_بفر;
            }
        }

      /* Not enough newlines in that bufferfull.  */
      إذا (pos - بایټونه_read == پیل_pos)
        {
          /* Not enough lines in the file; print everything from
             start_pos to the end.  */
          xwrite_stdout (بفر, بایټونه_read);
          dump_remainder (false, prettyname, fd, پای_ټکی_pos - pos);
          اذهب free_بفر;
        }

      pos = xlseek (fd, -(bufsize + بایټونه_read), SEEK_CUR, prettyname);
      بایټونه_read = اقرأ (fd, بفر, bufsize);
      إذا (بایټونه_read < 0)
        {
          pos = -1 - errno;
          error (0, errno, _("error reading %s"), quoteaf (prettyname));
          اذهب free_بفر;
        }
    }
  طالما (بایټونه_read > 0);

free_بفر:
  حرر (بفر);
  أرجع pos;
}

/* Print the last N_LINES lines from the end of the standard input,
   open for reading as pipe FD.
   Buffer the text as a linked list of LBUFFERs, adding them as needed.
   Return -1 if successful, (-1 - errno) otherwise.  */

ساكن صحيح
pipe_کرښې (حرف ثابت *prettyname, صحيح fd, شمېر_t n_کرښې)
{
  بنية linebuffer
  {
    حرف بفر[BUFSIZ];
    idx_t nbytes;
    idx_t nlines;
    بنية linebuffer *بل;
  };
  تعريفنوع بنية linebuffer LBUFFER;
  LBUFFER *لومړی, *وروستی, *tmp;
  idx_t ټول_کرښې = 0;	/* Total number of newlines in all buffers.  */
  صحيح ret = -1;
  ssize_t n_read;		/* Size in bytes of most recent read */

  لومړی = وروستی = xmalloc (حجم (LBUFFER));
  لومړی->nbytes = لومړی->nlines = 0;
  لومړی->بل = عدم;
  tmp = xmalloc (حجم (LBUFFER));

  /* Input is always read into a fresh buffer.  */
  طالما (true)
    {
      n_read = اقرأ (fd, tmp->بفر, BUFSIZ);
      إذا (n_read <= 0)
        اكسر;
      tmp->nbytes = n_read;
      tmp->nlines = 0;
      tmp->بل = عدم;

      /* Count the number of newlines just read.  */
      {
        حرف ثابت *بفر_پای_ټکی = tmp->بفر + n_read;
        حرف ثابت *p = tmp->بفر;
        طالما ((p = memchr (p, کرښه_پای_ټکی, بفر_پای_ټکی - p)))
          {
            ++p;
            ++tmp->nlines;
          }
      }
      ټول_کرښې += tmp->nlines;

      /* If there is enough room in the last buffer read, just append the new
         one to it.  This is because when reading from a pipe, 'n_read' can
         often be very small.  */
      إذا (tmp->nbytes + وروستی->nbytes < BUFSIZ)
        {
          حافظه_نقل (&وروستی->بفر[وروستی->nbytes], tmp->بفر, tmp->nbytes);
          وروستی->nbytes += tmp->nbytes;
          وروستی->nlines += tmp->nlines;
        }
      وإلا
        {
          /* If there's not enough room, link the new buffer onto the end of
             the list, then either free up the oldest buffer for the next
             read if that would leave enough lines, or else malloc a new one.
             Some compaction mechanism is possible but probably not
             worthwhile.  */
          وروستی = وروستی->بل = tmp;
          إذا (ټول_کرښې - لومړی->nlines > n_کرښې)
            {
              tmp = لومړی;
              ټول_کرښې -= لومړی->nlines;
              لومړی = لومړی->بل;
            }
          وإلا
            tmp = xmalloc (حجم (LBUFFER));
        }
    }

  حرر (tmp);

  إذا (n_read < 0 && errno != EAGAIN)
    {
      ret = -1 - errno;
      error (0, errno, _("error reading %s"), quoteaf (prettyname));
      اذهب free_lbuffers;
    }

  /* If the file is empty, then bail out.  */
  إذا (وروستی->nbytes == 0)
    اذهب free_lbuffers;

  /* This prevents a core dump when the pipe contains no newlines.  */
  إذا (n_کرښې == 0)
    اذهب free_lbuffers;

  /* Count the incomplete line on files that don't end with a newline.  */
  إذا (وروستی->بفر[وروستی->nbytes - 1] != کرښه_پای_ټکی)
    {
      ++وروستی->nlines;
      ++ټول_کرښې;
    }

  /* Run through the list, printing lines.  First, skip over unneeded
     buffers.  */
  لكل (tmp = لومړی; ټول_کرښې - tmp->nlines > n_کرښې; tmp = tmp->بل)
    ټول_کرښې -= tmp->nlines;

  /* Find the correct beginning, then print the rest of the file.  */
  {
    حرف ثابت *beg = tmp->بفر;
    حرف ثابت *بفر_پای_ټکی = tmp->بفر + tmp->nbytes;
    إذا (ټول_کرښې > n_کرښې)
      {
        /* Skip 'total_lines' - 'n_lines' newlines.  We made sure that
           'total_lines' - 'n_lines' <= 'tmp->nlines'.  */
        لكل (idx_t j = ټول_کرښې - n_کرښې; j; --j)
          {
            beg = rawmemchr (beg, کرښه_پای_ټکی);
            ++beg;
          }
      }

    xwrite_stdout (beg, بفر_پای_ټکی - beg);
  }

  لكل (tmp = tmp->بل; tmp; tmp = tmp->بل)
    xwrite_stdout (tmp->بفر, tmp->nbytes);

free_lbuffers:
  طالما (لومړی)
    {
      tmp = لومړی->بل;
      حرر (لومړی);
      لومړی = tmp;
    }
  أرجع ret;
}

/* Print the last N_BYTES characters from the end of FD.
   Work even if the input is a pipe.
   This is a stripped down version of pipe_lines.
   The initial file offset is READ_POS if nonnegative, otherwise unknown.
   Return (-1 - errno) on failure, otherwise the resulting file offset
   if known, otherwise -1.  */

ساكن off_t
pipe_بایټونه (حرف ثابت *prettyname, صحيح fd, شمېر_t n_بایټونه,
            off_t read_pos)
{
  بنية charbuffer
  {
    حرف بفر[BUFSIZ];
    idx_t nbytes;
    بنية charbuffer *بل;
  };
  تعريفنوع بنية charbuffer CBUFFER;
  CBUFFER *لومړی, *وروستی, *tmp;
  idx_t i;			/* Index into buffers.  */
  intmax_t ټول_بایټونه = 0;	/* Total characters in all buffers.  */
  ssize_t n_read;

  إذا (read_pos < 0)
    read_pos = TYPE_MINIMUM (off_t);

  لومړی = وروستی = xmalloc (حجم (CBUFFER));
  لومړی->nbytes = 0;
  لومړی->بل = عدم;
  tmp = xmalloc (حجم (CBUFFER));

  /* Input is always read into a fresh buffer.  */
  طالما (true)
    {
      n_read = اقرأ (fd, tmp->بفر, BUFSIZ);
      إذا (n_read <= 0)
        اكسر;
      read_pos += n_read;
      tmp->nbytes = n_read;
      tmp->بل = عدم;

      ټول_بایټونه += tmp->nbytes;
      /* If there is enough room in the last buffer read, just append the new
         one to it.  This is because when reading from a pipe, 'nbytes' can
         often be very small.  */
      إذا (tmp->nbytes + وروستی->nbytes < BUFSIZ)
        {
          حافظه_نقل (&وروستی->بفر[وروستی->nbytes], tmp->بفر, tmp->nbytes);
          وروستی->nbytes += tmp->nbytes;
        }
      وإلا
        {
          /* If there's not enough room, link the new buffer onto the end of
             the list, then either free up the oldest buffer for the next
             read if that would leave enough characters, or else malloc a new
             one.  Some compaction mechanism is possible but probably not
             worthwhile.  */
          وروستی = وروستی->بل = tmp;
          إذا (ټول_بایټونه - لومړی->nbytes > n_بایټونه)
            {
              tmp = لومړی;
              ټول_بایټونه -= لومړی->nbytes;
              لومړی = لومړی->بل;
            }
          وإلا
            {
              tmp = xmalloc (حجم (CBUFFER));
            }
        }
    }

  حرر (tmp);

  إذا (n_read < 0 && errno != EAGAIN)
    {
      read_pos = -1 - errno;
      error (0, errno, _("error reading %s"), quoteaf (prettyname));
      اذهب free_cbuffers;
    }

  /* Run through the list, printing characters.  First, skip over unneeded
     buffers.  */
  لكل (tmp = لومړی; ټول_بایټونه - tmp->nbytes > n_بایټونه; tmp = tmp->بل)
    ټول_بایټونه -= tmp->nbytes;

  /* Find the correct beginning, then print the rest of the file.
     We made sure that 'total_bytes' - 'n_bytes' <= 'tmp->nbytes'.  */
  إذا (ټول_بایټونه > n_بایټونه)
    i = ټول_بایټونه - n_بایټونه;
  وإلا
    i = 0;
  xwrite_stdout (&tmp->بفر[i], tmp->nbytes - i);

  لكل (tmp = tmp->بل; tmp; tmp = tmp->بل)
    xwrite_stdout (tmp->بفر, tmp->nbytes);

  إذا (read_pos < 0)
    read_pos = -1;

free_cbuffers:
  طالما (لومړی)
    {
      tmp = لومړی->بل;
      حرر (لومړی);
      لومړی = tmp;
    }
  أرجع read_pos;
}

/* Skip N_BYTES characters from the start of pipe FD, and print
   any extra characters that were read beyond that.
   Return (-1 - errno) on failure, -1 if EOF, 0 otherwise.  */

ساكن صحيح
پیل_بایټونه (حرف ثابت *prettyname, صحيح fd, شمېر_t n_بایټونه)
{
  حرف بفر[BUFSIZ];

  طالما (0 < n_بایټونه)
    {
      ssize_t بایټونه_read = اقرأ (fd, بفر, BUFSIZ);
      إذا (بایټونه_read == 0)
        أرجع -1;
      إذا (بایټونه_read < 0)
        {
          صحيح ret = -1 - errno;
          error (0, errno, _("error reading %s"), quoteaf (prettyname));
          أرجع ret;
        }
      إذا (بایټونه_read <= n_بایټونه)
        n_بایټونه -= بایټونه_read;
      وإلا
        {
          xwrite_stdout (&بفر[n_بایټونه], بایټونه_read - n_بایټونه);
          اكسر;
        }
    }

  أرجع 0;
}

/* Skip N_LINES lines at the start of file or pipe FD, and print
   any extra bytes that were read beyond that.
   Return (-1 - errno) on failure, -1 if EOF, 0 otherwise.  */

ساكن صحيح
پیل_کرښې (حرف ثابت *prettyname, صحيح fd, شمېر_t n_کرښې)
{
  إذا (n_کرښې == 0)
    أرجع 0;

  طالما (true)
    {
      حرف بفر[BUFSIZ];
      ssize_t بایټونه_read = اقرأ (fd, بفر, BUFSIZ);
      إذا (بایټونه_read == 0) /* EOF */
        أرجع -1;
      إذا (بایټونه_read < 0) /* error */
        {
          صحيح ret = -1 - errno;
          error (0, errno, _("error reading %s"), quoteaf (prettyname));
          أرجع ret;
        }

      حرف *بفر_پای_ټکی = بفر + بایټونه_read;
      حرف *p = بفر;
      طالما ((p = memchr (p, کرښه_پای_ټکی, بفر_پای_ټکی - p)))
        {
          ++p;
          إذا (--n_کرښې == 0)
            {
              إذا (p < بفر_پای_ټکی)
                xwrite_stdout (p, بفر_پای_ټکی - p);
              أرجع 0;
            }
        }
    }
}

/* Return true if FD might be open on a file residing on a remote file system,
   false if FD is known to be local.  F is the file's spec.
   If fstatfs fails, give a diagnostic and return true.
   If fstatfs cannot be called, return true.  */
ساكن bool
fremote (صحيح fd, بنية File_spec ثابت *f)
{
  bool remote = true;           /* be conservative (poll by default).  */

#إذا HAVE_FSTATFS && HAVE_STRUCT_STATFS_F_TYPE \
 && (defined __linux__ || defined __ANDROID__)
  بنية statfs buf;
  صحيح err = fstatfs (fd, &buf);
  إذا (err != 0)
    {
      /* On at least linux-2.6.38, fstatfs fails with ENOSYS when FD
         is open on a pipe.  Treat that like a remote file.  */
      إذا (errno != ENOSYS)
        error (0, errno, _("cannot determine location of %s. "
                           "reverting to polling"), quoteaf (f->prettyname));
    }
  وإلا
    {
      /* Treat unrecognized file systems as "remote", so caller polls.
         Note README-release has instructions for syncing the internal
         list with the latest Linux kernel file system constants.  */
      remote = is_local_fs_type (buf.f_type) <= 0;
    }
#د_که_پای

  أرجع remote;
}

/* open/fstat F->name and handle changes.  */
ساكن فراغ
recheck (بنية File_spec *f, bool blocking)
{
  بنية stat new_stats;
  bool ok = false;
  bool is_stdin = (streq (f->name, "-"));
  صحيح prev_errnum = f->errnum;
  bool new_file;
  صحيح fd = (is_stdin
            ? STDIN_FILENO
            : open (f->name, O_RDONLY | (blocking ? 0 : O_NONBLOCK)));
  صحيح open_errno = fd < 0 ? errno : 0;

  affirm (valid_file_spec (f));

  إذا (! disable_inotify && issymlink (f->name) == 1)
    {
      /* Diagnose the edge case where a regular file is changed
         to a symlink.  We avoid inotify with symlinks since
         it's awkward to match between symlink name and target.  */
      f->errnum = -1;
      f->ignore = true;

      error (0, 0, _("%s has been replaced with an untailable symbolic link"),
             quoteaf (f->prettyname));
    }
  وإلا إذا (fd < 0 || fstat (fd, &new_stats) < 0)
    {
      f->errnum = fd < 0 ? open_errno : errno;
      إذا (fd < 0)
        {
          إذا (f->tailable)
            {
              /* FIXME-maybe: detect the case in which the file first becomes
                 unreadable (perms), and later becomes readable again and can
                 be seen to be the same file (dev/ino).  Otherwise, tail prints
                 the entire contents of the file when it becomes readable.  */
              error (0, f->errnum, _("%s has become inaccessible"),
                     quoteaf (f->prettyname));
            }
          وإلا
            {
              /* say nothing... it's still not tailable */
            }
        }
      وإلا إذا (prev_errnum != f->errnum)
        error (0, f->errnum, "%s", quotef (f->prettyname));
    }
  وإلا إذا (!IS_TAILABLE_FILE_TYPE (new_stats.st_ډېرپېښ))
    {
      f->errnum = -1;
      f->ignore = ! (reopen_inaccessible_files && follow_ډېرپېښ == Follow_name);
      إذا (f->tailable || ! (prev_errnum < 0 || prev_errnum == EISDIR))
        error (0, 0, _("%s has been replaced with an untailable file%s"),
               quoteaf (f->prettyname),
               f->ignore ? _("; giving up on this name") : "");
    }
  وإلا إذا ((f->remote = fremote (fd, f)) && ! disable_inotify)
    {
      f->errnum = -1;
      error (0, 0, _("%s has been replaced with an untailable remote file"),
             quoteaf (f->prettyname));
      f->ignore = true;
    }
  وإلا
    {
      ok = true;
      f->errnum = 0;
    }

  f->tailable = ok;

  new_file = false;
  إذا (!ok)
    {
      close_fd (fd, f);
      close_fd (f->fd, f);
      f->fd = -1;
    }
  وإلا إذا (prev_errnum && prev_errnum != ENOENT)
    {
      new_file = true;
      affirm (f->fd < 0);
      error (0, 0, _("%s has become accessible"), quoteaf (f->prettyname));
    }
  وإلا إذا (f->fd < 0)
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
  وإلا إذا (!SAME_INODE (*f, new_stats))
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
  وإلا
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
  إذا (new_file)
    {
      /* Start at the file's current position, normally the beginning.  */
      ریکارډ_open_fd (f, fd, -1, &new_stats, is_stdin ? -1 : blocking);
    }
}

/* Return true if any of the N_FILES files in F are live, i.e., have
   open file descriptors, or should be checked again (see --retry).
   When following descriptors, checking should only continue when any
   of the files is not yet ignored.  */

ساكن bool
any_ژوندی_files (ثابت بنية File_spec *f, صحيح n_files)
{
  /* In inotify mode, ignore may be set for files
     which may later be replaced with new files.
     So always consider files live in -F mode.  */
  إذا (reopen_inaccessible_files && follow_ډېرپېښ == Follow_name)
    أرجع true;

  لكل (صحيح i = 0; i < n_files; i++)
    {
      إذا (0 <= f[i].fd)
        أرجع true;
      وإلا
        {
          إذا (! f[i].ignore && reopen_inaccessible_files)
            أرجع true;
        }
    }

  أرجع false;
}

/* Return true if any writers exist, even as zombies.  */

ساكن bool
some_writers_exist (فراغ)
{
  /* To avoid some (though not all) races if the kernel reuses PIDs,
     check all PIDs instead of returning merely because one PID exists.  */
  لكل (idx_t i = 0; i < nbpids; )
    {
      إذا (kill (pids[i], 0) < 0 && errno == ESRCH)
        {
          nbpids--;
          memmove (&pids[i], &pids[i + 1], (nbpids - i) * حجم pids[i]);
        }
      وإلا
        i++;
    }


  أرجع 0 < nbpids;
}

/* Tail N_FILES files forever, or until killed.
   The pertinent information for each file is stored in an entry of F.
   Loop over each of them, doing an fstat to see if they have changed size,
   and an occasional open/fstat to see if any dev/ino pair has changed.
   If none of them have changed size in one iteration, sleep for a
   while and try again.  Continue until the user interrupts us.  */

ساكن فراغ
لکۍ_forever (بنية File_spec *f, صحيح n_files, مزدوج sleep_interval)
{
  صحيح وروستی = n_files - 1;

  ساكن bool debugged;

  /* Flush initial output, in case reading blocks below.  */
  إذا (fflush (stdout) < 0)
    write_error ();

  طالما (true)
    {
      /* Use blocking I/O as an optimization, when it's easy.  */
      bool blocking = (!nbpids && follow_ډېرپېښ == Follow_descriptor
                       && n_files == 1 && 0 <= f[0].fd && !S_ISREG (f[0].ډېرپېښ_2));

      إذا (debug && !debugged)
        {
          debugged = true;
          error (0, 0, "%s", blocking
                             ? _("using blocking mode")
                             : _("using polling mode"));
        }

      bool any_ننوت = false;

      لكل (صحيح i = 0; i < n_files; i++)
        {
          بنية stat stats;

          إذا (f[i].ignore)
            استمر;

          صحيح fd = f[i].fd;
          إذا (fd < 0)
            {
              recheck (&f[i], blocking);
              استمر;
            }

          حرف ثابت *prettyname = f[i].prettyname;
          mode_t ډېرپېښ_2 = f[i].ډېرپېښ_2;

          إذا (f[i].blocking != blocking)
            {
              صحيح old_flags = fcntl (fd, F_GETFL);
              صحيح new_flags = old_flags | (blocking ? 0 : O_NONBLOCK);
              إذا (old_flags < 0
                  || (new_flags != old_flags
                      && fcntl (fd, F_SETFL, new_flags) < 0))
                {
                  /* Don't update f[i].blocking if fcntl fails.  */
                  إذا (S_ISREG (f[i].ډېرپېښ_2) && errno == EPERM)
                    {
                      /* This happens when using tail -f on a file with
                         the append-only attribute.  */
                    }
                  وإلا
                    error (EXIT_FAILURE, errno,
                           _("%s: cannot change nonblocking mode"),
                           quotef (prettyname));
                }
              وإلا
                f[i].blocking = blocking;
            }

          bool read_unchanged = false;
          إذا (!f[i].blocking)
            {
              إذا (fstat (fd, &stats) < 0)
                {
                  f[i].fd = -1;
                  f[i].errnum = errno;
                  error (0, errno, "%s", quotef (prettyname));
                  close (fd); /* ignore failure */
                  استمر;
                }

              إذا (f[i].ډېرپېښ_2 == stats.st_ډېرپېښ
                  && (! S_ISREG (stats.st_ډېرپېښ)
                      || f[i].read_pos == stats.st_اندازه)
                  && timespec_cmp (f[i].mtime, get_stat_mtime (&stats)) == 0)
                {
                  إذا ((max_n_unchanged_stats_between_opens
                       <= f[i].n_unchanged_stats++)
                      && follow_ډېرپېښ == Follow_name)
                    {
                      recheck (&f[i], f[i].blocking);
                      f[i].n_unchanged_stats = 0;
                    }
                  إذا (fd != f[i].fd || S_ISREG (stats.st_ډېرپېښ) || 1 < n_files)
                    استمر;
                  وإلا
                    read_unchanged = true;
                }

              affirm (fd == f[i].fd);

              /* This file has changed.  Print out what we can, and
                 then keep looping.  */

              f[i].mtime = get_stat_mtime (&stats);
              f[i].ډېرپېښ_2 = stats.st_ډېرپېښ;

              /* reset counter */
              إذا (! read_unchanged)
                f[i].n_unchanged_stats = 0;

              /* XXX: This is only a heuristic, as the file may have also
                 been truncated and written to if st_size >= size
                 (in which case we ignore new data <= size).  */
              إذا (S_ISREG (ډېرپېښ_2) && stats.st_اندازه < f[i].read_pos)
                {
                  error (0, 0, _("%s: file truncated"),
                         quotef (prettyname));
                  /* Assume the file was truncated to 0,
                     and therefore output all "new" data.  */
                  f[i].read_pos = xlseek (fd, 0, SEEK_SET, prettyname);
                }

              إذا (i != وروستی)
                {
                  إذا (print_سرلیکونه)
                    write_سرلیک (prettyname);
                  وروستی = i;
                }
            }

          /* Don't read more than st_size on networked file systems
             because it was seen on glusterfs at least, that st_size
             may be smaller than the data read on a _subsequent_ stat call.  */
          شمېر_t بایټونه_منزل_read;
          إذا (f[i].blocking)
            بایټونه_منزل_read = COPY_A_بفر;
          وإلا إذا (S_ISREG (ډېرپېښ_2) && f[i].remote)
            بایټونه_منزل_read = stats.st_اندازه - f[i].read_pos;
          وإلا
            بایټونه_منزل_read = COPY_منزل_EOF;

          شمېر_t nr = dump_remainder (false, prettyname, fd, بایټونه_منزل_read);
          إذا (0 < nr)
            {
              إذا (S_ISREG (ډېرپېښ_2))
                f[i].read_pos += nr;
              إذا (read_unchanged)
                f[i].n_unchanged_stats = 0;
              any_ننوت = true;
            }
        }

      إذا (! any_ژوندی_files (f, n_files))
        {
          error (EXIT_FAILURE, 0, _("no files remaining"));
          اكسر;
        }

      إذا ((!any_ننوت || blocking) && fflush (stdout) < 0)
        write_error ();

      check_output_alive ();

      إذا (!any_ننوت)
        {
          إذا (pids)
            {
              /* Exit loop if it was already known that no writers exist.
                 Otherwise, to avoid a race, loop one more time
                 without sleeping if no writers exist now.  */
              إذا (!nbpids)
                اكسر;
              إذا (!some_writers_exist ())
                استمر;
            }

          إذا (xnanosleep (sleep_interval))
            error (EXIT_FAILURE, errno, _("cannot read realtime clock"));
        }
    }
}

#إذا HAVE_INOTIFY

/* Return true if any of the N_FILES files in F is remote, i.e., has
   an open file descriptor and is on a network file system.  */

ساكن bool
any_remote_file (ثابت بنية File_spec *f, صحيح n_files)
{
  لكل (صحيح i = 0; i < n_files; i++)
    إذا (0 <= f[i].fd && f[i].remote)
      أرجع true;
  أرجع false;
}

/* Return true if any of the N_FILES files in F is non remote, i.e., has
   an open file descriptor and is not on a network file system.  */

ساكن bool
any_non_remote_file (ثابت بنية File_spec *f, صحيح n_files)
{
  لكل (صحيح i = 0; i < n_files; i++)
    إذا (0 <= f[i].fd && ! f[i].remote)
      أرجع true;
  أرجع false;
}

/* Return true if any of the N_FILES files in F is a symlink.
   Note we don't worry about the edge case where "-" exists,
   since that will have the same consequences for inotify,
   which is the only context this function is currently used.  */

ساكن bool
any_symlinks (ثابت بنية File_spec *f, صحيح n_files)
{
  لكل (صحيح i = 0; i < n_files; i++)
    إذا (issymlink (f[i].name) == 1)
      أرجع true;
  أرجع false;
}

/* Return true if any of the N_FILES files in F is not
   a regular file or fifo.  This is used to avoid adding inotify
   watches on a device file for example, which inotify
   will accept, but not give any events for.  */

ساكن bool
any_non_regular_fifo (ثابت بنية File_spec *f, صحيح n_files)
{
  لكل (صحيح i = 0; i < n_files; i++)
    إذا (0 <= f[i].fd && ! S_ISREG (f[i].ډېرپېښ_2) && ! S_ISFIFO (f[i].ډېرپېښ_2))
      أرجع true;
  أرجع false;
}

/* Return true if any of the N_FILES files in F represents
   stdin and is not ignored.  */

ساكن bool
tailable_stdin (ثابت بنية File_spec *f, صحيح n_files)
{
  لكل (صحيح i = 0; i < n_files; i++)
    إذا (!f[i].ignore && streq (f[i].name, "-"))
      أرجع true;
  أرجع false;
}

ساكن size_t
wd_hasher (ثابت فراغ *ثبت, size_t tabsize)
{
  ثابت بنية File_spec *spec = ثبت;
  أرجع spec->wd % tabsize;
}

ساكن bool
wd_comparator (ثابت فراغ *e1, ثابت فراغ *e2)
{
  ثابت بنية File_spec *spec1 = e1;
  ثابت بنية File_spec *spec2 = e2;
  أرجع spec1->wd == spec2->wd;
}

/* Hash Follow_name watches by parent directory wd + basename so directory
   inotify events can be matched in expected O(1) instead of scanning all
   files.  */
ساكن size_t
پلار_name_hasher (ثابت فراغ *ثبت, size_t tabsize)
{
  ثابت بنية File_spec *spec = ثبت;
  حرف ثابت *name = spec->name + spec->basename_پیل;
  /* hash_string already reduces modulo TABSIZE; mix in parent_wd so the
     same basename under different directories does not collide by design.  */
  أرجع (hash_string (name, tabsize) + (size_t) spec->پلار_wd) % tabsize;
}

ساكن bool
پلار_name_comparator (ثابت فراغ *e1, ثابت فراغ *e2)
{
  ثابت بنية File_spec *a = e1;
  ثابت بنية File_spec *b = e2;
  أرجع (a->پلار_wd == b->پلار_wd
          && streq (a->name + a->basename_پیل,
                    b->name + b->basename_پیل));
}

/* Output (new) data for FSPEC->fd.
   PREV_FSPEC records the last File_spec for which we output.  */
ساكن فراغ
check_fspec (بنية File_spec *fspec, بنية File_spec **prev_fspec)
{
  إذا (fspec->fd < 0)
    أرجع;

  بنية stat stats;
  إذا (fstat (fspec->fd, &stats) < 0)
    {
      fspec->errnum = errno;
      close_fd (fspec->fd, fspec);
      fspec->fd = -1;
      أرجع;
    }

  حرف ثابت *prettyname = fspec->prettyname;

  /* XXX: This is only a heuristic, as the file may have also
     been truncated and written to if st_size >= size
     (in which case we ignore new data <= size).
     Though in the inotify case it's more likely we'll get
     separate events for truncate() and write().  */
  إذا (S_ISREG (fspec->ډېرپېښ_2) && stats.st_اندازه < fspec->read_pos)
    {
      error (0, 0, _("%s: file truncated"), quotef (prettyname));
      fspec->read_pos = xlseek (fspec->fd, 0, SEEK_SET, prettyname);
    }
  وإلا إذا (S_ISREG (fspec->ډېرپېښ_2) && stats.st_اندازه == fspec->read_pos
           && timespec_cmp (fspec->mtime, get_stat_mtime (&stats)) == 0)
    أرجع;

  bool want_سرلیک = print_سرلیکونه && (fspec != *prev_fspec);

  شمېر_t nr = dump_remainder (want_سرلیک, prettyname, fspec->fd, COPY_منزل_EOF);
  إذا (0 < nr)
    {
      إذا (S_ISREG (fspec->ډېرپېښ_2))
        fspec->read_pos += nr;
      *prev_fspec = fspec;
      إذا (fflush (stdout) < 0)
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
ساكن فراغ
لکۍ_forever_inotify (صحيح wd, بنية File_spec *f, صحيح n_files,
                      مزدوج sleep_interval, Hash_جدول **wd_منزل_namep,
                      Hash_جدول **پلار_by_namep)
{
# إذا لکۍ_TEST_SLEEP
  /* Delay between open() and inotify_add_watch()
     to help trigger different cases.  */
  xnanosleep (1000000);
# endif
  صحيح max_realloc = 3;

  /* Map an inotify watch descriptor to the name of the file it's watching.  */
  Hash_جدول *wd_منزل_name;

  /* Map (parent_wd, basename) to File_spec for Follow_name
     directory events.  */
  Hash_جدول *پلار_by_name = عدم;

  bool وموندل_watchable_file = false;
  bool tailed_but_unwatchable = false;
  bool وموندل_unwatchable_dir = false;
  bool no_inotify_resources = false;
  بنية File_spec *prev_fspec;
  idx_t evlen = 0;
  حرف *evbuf;
  idx_t evbuf_off = 0;

  *پلار_by_namep = عدم;

  wd_منزل_name = hash_initialize (n_files, عدم, wd_hasher, wd_comparator,
                                عدم);
  إذا (! wd_منزل_name)
    xalloc_die ();
  *wd_منزل_namep = wd_منزل_name;

  إذا (follow_ډېرپېښ == Follow_name)
    {
      پلار_by_name = hash_initialize (n_files, عدم, پلار_name_hasher,
                                        پلار_name_comparator, عدم);
      إذا (! پلار_by_name)
        xalloc_die ();
      *پلار_by_namep = پلار_by_name;
    }

  /* The events mask used with inotify on files (not directories).  */
  uint32_t inotify_wd_ماسک = دننه_MODIFY;
  /* TODO: Perhaps monitor these events in Follow_descriptor mode also,
     to tag reported file names with "deleted", "moved" etc.  */
  إذا (follow_ډېرپېښ == Follow_name)
    inotify_wd_ماسک |= (دننه_ATTRIB | دننه_DELETE_SELF | دننه_MOVE_SELF);

  /* Add an inotify watch for each watched file.  If -F is specified then watch
     its parent directory too, in this way when they re-appear we can add them
     again to the watch list.  */
  لكل (صحيح i = 0; i < n_files; i++)
    {
      إذا (!f[i].ignore)
        {
          idx_t fnlen = مزي_اوږدوالی (f[i].name);
          إذا (evlen < fnlen)
            evlen = fnlen;

          f[i].wd = -1;

          إذا (follow_ډېرپېښ == Follow_name)
            {
              idx_t dirlen = dir_len (f[i].name);
              حرف prev = f[i].name[dirlen];
              f[i].basename_پیل = وروستی_component (f[i].name) - f[i].name;

              f[i].name[dirlen] = '\0';

               /* It's fine to add the same directory more than once.
                  In that case the same watch descriptor is returned.  */
              f[i].پلار_wd = inotify_جمع_کړه_watch (wd, dirlen ? f[i].name : ".",
                                                  (دننه_CREATE | دننه_DELETE
                                                   | دننه_MOVED_منزل | دننه_ATTRIB
                                                   | دننه_DELETE_SELF));

              f[i].name[dirlen] = prev;

              إذا (f[i].پلار_wd < 0)
                {
                  إذا (errno != ENOSPC) /* suppress confusing error.  */
                    error (0, errno, _("cannot watch parent directory of %s"),
                           quoteaf (f[i].name));
                  وإلا
                    error (0, 0, _("inotify resources exhausted"));
                  وموندل_unwatchable_dir = true;
                  /* We revert to polling below.  Note invalid uses
                     of the inotify API will still be diagnosed.  */
                  اكسر;
                }

              إذا (hash_ورننباسه (پلار_by_name, &(f[i])) == عدم)
                xalloc_die ();
            }

          f[i].wd = inotify_جمع_کړه_watch (wd, f[i].name, inotify_wd_ماسک);

          إذا (f[i].wd < 0)
            {
              إذا (0 <= f[i].fd)  /* already tailed.  */
                tailed_but_unwatchable = true;
              إذا (errno == ENOSPC || errno == ENOMEM)
                {
                  no_inotify_resources = true;
                  error (0, 0, _("inotify resources exhausted"));
                  اكسر;
                }
              وإلا إذا (errno != f[i].errnum)
                error (0, errno, _("cannot watch %s"), quoteaf (f[i].name));
              استمر;
            }

          إذا (hash_ورننباسه (wd_منزل_name, &(f[i])) == عدم)
            xalloc_die ();

          وموندل_watchable_file = true;
        }
    }

  /* Linux kernel 2.6.24 at least has a bug where eventually, ENOSPC is always
     returned by inotify_add_watch.  In any case we should revert to polling
     when there are no inotify resources.  Also a specified directory may not
     be currently present or accessible, so revert to polling.  Also an already
     tailed but unwatchable due rename/unlink race, should also revert.  */
  إذا (no_inotify_resources || وموندل_unwatchable_dir
      || (follow_ډېرپېښ == Follow_descriptor && tailed_but_unwatchable))
    أرجع;
  إذا (follow_ډېرپېښ == Follow_descriptor && !وموندل_watchable_file)
    وتل (EXIT_FAILURE);

  prev_fspec = &(f[n_files - 1]);

  /* Check files again.  New files or data can be available since last time we
     checked and before they are watched by inotify.  */
  لكل (صحيح i = 0; i < n_files; i++)
    {
      إذا (! f[i].ignore)
        {
          /* check for new files.  */
          إذا (follow_ډېرپېښ == Follow_name)
            recheck (&(f[i]), false);
          وإلا إذا (0 <= f[i].fd)
            {
              /* If the file was replaced in the small window since we tailed,
                 then assume the watch is on the wrong item (different to
                 that we've already produced output for), and so revert to
                 polling the original descriptor.  */
              بنية stat stats;

              إذا (! (stat (f[i].name, &stats) < 0
                     || SAME_INODE (f[i], stats)))
                {
                  error (0, errno, _("%s was replaced"),
                         quoteaf (f[i].prettyname));
                  أرجع;
                }
            }

          /* check for new data.  */
          check_fspec (&f[i], &prev_fspec);
        }
    }

  إذا (debug)
    error (0, 0, "%s", _("using notification mode"));

  evlen += حجم (بنية inotify_event) + 1;
  evbuf = ximalloc (evlen);

  /* Wait for inotify events and handle them.  Events on directories
     ensure that watched files can be re-added when following by name.
     This loop blocks on the 'read' call until a new event is notified.
     But when --pid=P is specified, tail usually waits via poll.  */
  ptrdiff_t len = 0;
  طالما (true)
    {
      بنية File_spec *fspec;
      بنية inotify_event *ev;
      فراغ *void_ev;

      /* When following by name without --retry, and the last file has
         been unlinked or renamed-away, diagnose it and return.  */
      إذا (follow_ډېرپېښ == Follow_name
          && ! reopen_inaccessible_files
          && hash_get_n_entries (wd_منزل_name) == 0)
        error (EXIT_FAILURE, 0, _("no files remaining"));

      إذا (len <= evbuf_off)
        {
          /* Poll for inotify events.  When watching a PID, ensure
             that a read from WD will not block indefinitely.
             If MONITOR_OUTPUT, also poll for a broken output pipe.  */

          صحيح file_change;
          بنية pollfd pfd[2];
          افعل
            {
              /* How many ms to wait for changes.  -1 means wait forever.  */
              صحيح delay = -1;

              إذا (pids)
                {
                  إذا (!nbpids)
                    وتل (EXIT_SUCCESS);

                  إذا (!some_writers_exist () || sleep_interval <= 0)
                    delay = 0;
                  وإلا إذا (sleep_interval < لوی_عدد / 1000 - 1)
                    {
                      /* delay = ceil (sleep_interval * 1000), sans libm.  */
                      مزدوج ddelay = sleep_interval * 1000;
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
          طالما (file_change == 0 || (file_change < 0 && errno == EINTR));

          إذا (file_change < 0)
            error (EXIT_FAILURE, errno,
                   _("error waiting for inotify and output events"));
          إذا (pfd[1].revents)
            die_pipe ();

          len = اقرأ (wd, evbuf, evlen);
          evbuf_off = 0;

          /* For kernels prior to 2.6.21, read returns 0 when the buffer
             is too small.  */
          إذا ((len == 0 || (len < 0 && errno == EINVAL))
              && max_realloc--)
            {
              len = 0;
              evlen *= 2;
              evbuf = xirealloc (evbuf, evlen);
              استمر;
            }

          إذا (len <= 0)
            error (EXIT_FAILURE, errno, _("error reading inotify event"));
        }

      void_ev = evbuf + evbuf_off;
      ev = void_ev;
      evbuf_off += حجم (*ev) + ev->len;

      /* If a directory is deleted, IN_DELETE_SELF is emitted
         with ev->name of length 0.
         We need to catch it, otherwise it would wait forever,
         as wd for directory becomes inactive. Revert to polling now.   */
      إذا ((ev->ماسک & دننه_DELETE_SELF) && ! ev->len)
        {
          لكل (صحيح i = 0; i < n_files; i++)
            {
              إذا (ev->wd == f[i].پلار_wd)
                {
                  error (0, 0,
                      _("directory containing watched file was removed"));
                  أرجع;
                }
            }
        }

      إذا (ev->len) /* event on ev->name in watched directory.  */
        {
          بنية File_spec کلی;
          إذا (! پلار_by_name)
            استمر;

          /* Probe key: basename is the whole string at KEY.NAME.  */
          کلی.پلار_wd = ev->wd;
          کلی.name = ev->name;
          کلی.basename_پیل = 0;
          fspec = hash_lookup (پلار_by_name, &کلی);

          /* It is not a watched file.  */
          إذا (! fspec)
            استمر;

          صحيح new_wd = -1;
          bool deleting = !! (ev->ماسک & دننه_DELETE);

          إذا (! deleting)
            {
              /* Adding the same inode again will look up any existing wd.  */
              new_wd = inotify_جمع_کړه_watch (wd, fspec->name, inotify_wd_ماسک);
            }

          إذا (! deleting && new_wd < 0)
            {
              إذا (errno == ENOSPC || errno == ENOMEM)
                {
                  error (0, 0, _("inotify resources exhausted"));
                  أرجع; /* revert to polling.  */
                }
              وإلا
                {
                  /* Can get ENOENT for a dangling symlink for example.  */
                  error (0, errno, _("cannot watch %s"), quoteaf (fspec->name));
                }
              /* We'll continue below after removing the existing watch.  */
            }

          /* This will be false if only attributes of file change.  */
          bool new_watch;
          new_watch = (! deleting) && (fspec->wd < 0 || new_wd != fspec->wd);

          إذا (new_watch)
            {
              إذا (0 <= fspec->wd)
                {
                  inotify_rm_watch (wd, fspec->wd);
                  hash_remove (wd_منزل_name, fspec);
                }

              fspec->wd = new_wd;

              إذا (new_wd < 0)
                استمر;

              /* If the file was moved then inotify will use the source file wd
                for the destination file.  Make sure the key is not present in
                the table.  */
              بنية File_spec *prev = hash_remove (wd_منزل_name, fspec);
              إذا (prev && prev != fspec)
                {
                  إذا (follow_ډېرپېښ == Follow_name)
                    recheck (prev, false);
                  prev->wd = -1;
                  close_fd (prev->fd, prev);
                }

              إذا (hash_ورننباسه (wd_منزل_name, fspec) == عدم)
                xalloc_die ();
            }

          إذا (follow_ډېرپېښ == Follow_name)
            recheck (fspec, false);
        }
      وإلا
        {
          بنية File_spec کلی;
          کلی.wd = ev->wd;
          fspec = hash_lookup (wd_منزل_name, &کلی);
        }

      إذا (! fspec)
        استمر;

      إذا (ev->ماسک & (دننه_ATTRIB | دننه_DELETE | دننه_DELETE_SELF | دننه_MOVE_SELF))
        {
          /* Note for IN_MOVE_SELF (the file we're watching has
             been clobbered via a rename) without --retry we leave the watch
             in place since it may still be part of the set
             of watched names.  */
          إذا (ev->ماسک & دننه_DELETE_SELF
              || (!reopen_inaccessible_files && (ev->ماسک & دننه_MOVE_SELF)))
            {
              inotify_rm_watch (wd, fspec->wd);
              hash_remove (wd_منزل_name, fspec);
            }

          /* Note we get IN_ATTRIB for unlink() as st_nlink decrements.
             The usual path is a close() done in recheck() triggers
             an IN_DELETE_SELF event as the inode is removed.
             However sometimes open() will succeed as even though
             st_nlink is decremented, the dentry (cache) is not updated.
             Thus we depend on the IN_DELETE event on the directory
             to trigger processing for the removed file.  */

          recheck (fspec, false);

          استمر;
        }
      check_fspec (fspec, &prev_fspec);
    }
}
#د_که_پای

/* Output the last bytes of the file PRETTYNAME open for reading
   in FD and with status ST.  Output the last N_BYTES bytes.
   Return (-1 - errno) on failure, otherwise the resulting file offset
   if known, otherwise -1.  */

ساكن off_t
لکۍ_بایټونه (حرف ثابت *prettyname, صحيح fd, بنية stat ثابت *st,
            شمېر_t n_بایټونه)
{
  off_t current_pos
    = (presume_ننوت_pipe
       ? -1
       : lseek (fd, from_پیل ? MIN (n_بایټونه, OFF_T_MAX) : 0, SEEK_CUR));

  إذا (from_پیل)
    {
      إذا (current_pos < 0)
        {
          صحيح t = پیل_بایټونه (prettyname, fd, n_بایټونه);
          إذا (t < 0)
            أرجع t;
        }
      n_بایټونه = COPY_منزل_EOF;
    }
  وإلا
    {
      off_t initial_pos = current_pos;
      off_t پای_ټکی_pos = -1;

      إذا (0 <= current_pos)
        {
          إذا (usable_st_اندازه (st))
            {
              /* Use st_size only if it's so large that this is
                 probably not a /proc or similar file, where st_size
                 is notional.  */
              off_t smallish_اندازه = STP_BLKSIZE (st);
              إذا (smallish_اندازه < st->st_اندازه)
                پای_ټکی_pos = st->st_اندازه;
            }
          وإلا
            {
              off_t minus_n;
              إذا (ckd_sub (&minus_n, 0, n_بایټونه))
                پای_ټکی_pos = 0;
              وإلا
                {
                  off_t e_n_pos = lseek (fd, minus_n, SEEK_END);
                  إذا (0 <= e_n_pos)
                    {
                      current_pos = e_n_pos;
                      پای_ټکی_pos = e_n_pos + n_بایټونه;
                    }
                  وإلا
                    {
                      /* POSIX says errno is unreliable here.
                         Perhaps the file is smaller than N_BYTES.
                         Try again at file end.  */
                      off_t e_pos = lseek (fd, 0, SEEK_END);
                      إذا (0 <= e_pos)
                        current_pos = پای_ټکی_pos = e_pos;
                    }
               }
            }
        }

      /* If the end is known and more than N_BYTES after the initial
         position, go to N_BYTES before the end; otherwise go back to
         the initial position.  But do not lseek if lseek already failed.  */
      off_t pos = (initial_pos < پای_ټکی_pos && n_بایټونه < پای_ټکی_pos - initial_pos
                   ? پای_ټکی_pos - n_بایټونه
                   : initial_pos);
      إذا (pos != current_pos)
        current_pos = xlseek (fd, pos, SEEK_SET, prettyname);

      إذا (پای_ټکی_pos < 0)
        أرجع pipe_بایټونه (prettyname, fd, n_بایټونه, current_pos);
    }

  شمېر_t nr = dump_remainder (false, prettyname, fd, n_بایټونه);
  أرجع current_pos < 0 ? -1 : current_pos + nr;
}

/* Output the last lines of the file PRETTYNAME open for reading
   in FD and with status ST.  Output the last N_LINES lines.
   Return (-1 - errno) on failure, otherwise the resulting file offset
   if known, otherwise -1.  */

ساكن off_t
لکۍ_کرښې (حرف ثابت *prettyname, صحيح fd, بنية stat ثابت *st,
            شمېر_t n_کرښې)
{
  إذا (from_پیل)
    {
      /* If skipping all input use lseek if possible, for speed.  */
      إذا (OFF_T_MAX <= n_کرښې)
        {
          off_t e = lseek (fd, 0, SEEK_END);
          إذا (0 <= e)
            أرجع e;
        }

      صحيح t = پیل_کرښې (prettyname, fd, n_کرښې);
      إذا (t < 0)
        أرجع t;
      dump_remainder (false, prettyname, fd, COPY_منزل_EOF);
      أرجع -1;
    }
  وإلا
    {
      /* Use file_lines only if FD is a regular file and SEEK_END
         reports more data.  */
      off_t پیل_pos = (!presume_ننوت_pipe && S_ISREG (st->st_ډېرپېښ)
                         ? lseek (fd, 0, SEEK_CUR)
                         : -1);
      off_t پای_ټکی_pos = پیل_pos < 0 ? -1 : lseek (fd, 0, SEEK_END);
      أرجع (پای_ټکی_pos < 0
              ? pipe_کرښې (prettyname, fd, n_کرښې)
              : پیل_pos < پای_ټکی_pos
              ? file_کرښې (prettyname, fd, st, n_کرښې, پیل_pos, پای_ټکی_pos)
              : پیل_pos != پای_ټکی_pos
              ? (/* Do not read from before the start offset,
                    even if the input file shrank.  */
                 xlseek (fd, پیل_pos, SEEK_SET, prettyname))
              : پیل_pos);
    }
}

/* Display the last part of file FILENAME,
   open for reading via FD and with status *ST.
   Return (-1 - errno) on failure, otherwise the resulting file offset
   if known, otherwise -1.  */

ساكن off_t
لکۍ (حرف ثابت *filename, صحيح fd, بنية stat ثابت *st, شمېر_t n_units)
{
  أرجع (شمېر_کرښې ? لکۍ_کرښې : لکۍ_بایټونه) (filename, fd, st, n_units);
}

/* Display the last N_UNITS units of the file described by F.
   Return true if successful.  */

ساكن bool
لکۍ_file (بنية File_spec *f, شمېر_t n_files, شمېر_t n_units)
{
  صحيح fd;
  bool ok;

  /* Avoid blocking if we may need to process asynchronously.  */
  bool nonblocking = forever && (nbpids || n_files > 1);

  bool is_stdin = (streq (f->name, "-"));

  إذا (is_stdin)
    {
      have_read_stdin = true;
      fd = STDIN_FILENO;
      xset_دوګونی_ډېرپېښ (STDIN_FILENO, O_دوګونی);
    }
  وإلا
    fd = open (f->name, O_RDONLY | O_دوګونی | (nonblocking ? O_NONBLOCK : 0));

  f->tailable = false;

  إذا (fd < 0)
    {
      إذا (forever)
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
  وإلا
    {
      إذا (print_سرلیکونه)
        write_سرلیک (f->prettyname);

      off_t read_pos;
      بنية stat stats;
      ok = 0 <= fstat (fd, &stats);
      إذا (!ok)
        {
          f->errnum = errno;
          error (0, f->errnum, _("cannot fstat %s"), quoteaf (f->prettyname));
        }
      وإلا
        {
          read_pos = لکۍ (f->prettyname, fd, &stats, n_units);
          ok = -1 <= read_pos;
          f->errnum = ok ? 0 : -1 - read_pos;

          إذا (IS_TAILABLE_FILE_TYPE (stats.st_ډېرپېښ))
            f->tailable = true;
          وإلا إذا (forever)
            {
              ok = false;
              f->errnum = -1;
              error (0, 0, _("%s: cannot follow end of this type of file%s"),
                     quotef (f->prettyname),
                     (reopen_inaccessible_files ? ""
                      : _("; giving up on this name")));
            }
        }

      إذا (forever)
        {
          إذا (!ok)
            {
              f->ignore = ! reopen_inaccessible_files;
              close_fd (fd, f);
              f->fd = -1;
            }
          وإلا
            {
              ریکارډ_open_fd (f, fd, read_pos, &stats, (is_stdin ? -1 : 1));
              f->remote = fremote (fd, f);
            }
        }
      وإلا
        {
          إذا (!is_stdin && close (fd) < 0)
            {
              error (0, errno, _("error reading %s"),
                     quoteaf (f->prettyname));
              ok = false;
            }
        }
    }

  أرجع ok;
}

/* If obsolete usage is allowed, and the command line arguments are of
   the obsolete form and the option string is well-formed, set
   *N_UNITS, the globals COUNT_LINES, FOREVER, and FROM_START, and
   return true.  If the command line arguments are obviously incorrect
   (e.g., because obsolete usage is not allowed and the arguments are
   incorrect for non-obsolete usage), report an error and exit.
   Otherwise, return false and don't modify any parameter or global
   variable.  */

ساكن bool
parse_obsolete_option (صحيح argc, حرف * ثابت *argv, شمېر_t *n_units)
{
  bool t_from_پیل;
  bool t_شمېر_کرښې = true;
  bool t_forever = false;

  /* With the obsolete form, there is one option string and at most
     one file argument.  Watch out for "-" and "--", though.  */
  إذا (! (argc == 2
         || (argc == 3 && ! (argv[2][0] == '-' && argv[2][1]))
         || (3 <= argc && argc <= 4 && streq (argv[2], "--"))))
    أرجع false;

  صحيح posix_ver = posix2_نسخه ();
  bool obsolete_usage = posix_ver < 200112;
  bool traditional_usage = obsolete_usage || 200809 <= posix_ver;
  حرف ثابت *p = argv[1];

  بدل (*p++)
    {
    افتراضي:
      أرجع false;

    حالة '+':
      /* Leading "+" is a file name in the standard form.  */
      إذا (!traditional_usage)
        أرجع false;

      t_from_پیل = true;
      اكسر;

    حالة '-':
      /* In the non-obsolete form, "-" is standard input and "-c"
         requires an option-argument.  The obsolete multidigit options
         are supported as a GNU extension even when conforming to
         POSIX 1003.1-2001 or later, so don't complain about them.  */
      إذا (!obsolete_usage && !p[p[0] == 'c'])
        أرجع false;

      t_from_پیل = false;
      اكسر;
    }

  شمېر_t n;
  إذا (!c_isdigit (*p))
    n = DEFAULT_N_کرښې;
  وإلا
    لكل (n = 0; c_isdigit (*p); p++)
      n = ckd_mul (&n, n, 10) || ckd_جمع_کړه (&n, n, *p - '0') ? شمېر_MAX : n;

  بدل (*p)
    {
    حالة 'b': n = ckd_mul (&n, n, 512) ? شمېر_MAX : n; FALLTHROUGH;
    حالة 'c': t_شمېر_کرښې = false; FALLTHROUGH;
    حالة 'l': p++; اكسر;
    }

  إذا (*p == 'f')
    {
      t_forever = true;
      ++p;
    }

  إذا (*p)
    أرجع false;

  *n_units = n;

  /* Set globals.  */
  from_پیل = t_from_پیل;
  شمېر_کرښې = t_شمېر_کرښې;
  forever = t_forever;

  أرجع true;
}

ساكن فراغ
parse_options (صحيح argc, حرف **argv,
               شمېر_t *n_units, تعداد سرلیک_ډېرپېښ *سرلیک_ډېرپېښ,
               مزدوج *sleep_interval)
{
  صحيح c;

  طالما ((c = getopt_long (argc, argv, "c:n:fFqs:vz0123456789",
                           long_options, عدم))
         != -1)
    {
      بدل (c)
        {
        حالة 'F':
          forever = true;
          follow_ډېرپېښ = Follow_name;
          reopen_inaccessible_files = true;
          اكسر;

        حالة 'c':
        حالة 'n':
          شمېر_کرښې = (c == 'n');
          إذا (*optarg == '+')
            from_پیل = true;
          وإلا إذا (*optarg == '-')
            ++optarg;

          *n_units = xnumtoimax (optarg, 10, 0, شمېر_MAX, "bkKmMGTPEZYRQ0",
                                 (شمېر_کرښې
                                  ? _("invalid number of lines")
                                  : _("invalid number of bytes"))
                                 , 0, XTOINT_MAX_QUIET);
          اكسر;

        حالة 'f':
        حالة LONG_FOLLOW_OPTION:
          forever = true;
          إذا (optarg == عدم)
            follow_ډېرپېښ = DEFAULT_FOLLOW_ډېرپېښ;
          وإلا
            follow_ډېرپېښ = XARGMATCH ("--follow", optarg,
                                     follow_ډېرپېښ_string, follow_ډېرپېښ_map);
          اكسر;

        حالة RETRY_OPTION:
          reopen_inaccessible_files = true;
          اكسر;

        حالة MAX_UNCHANGED_STATS_OPTION:
          /* --max-unchanged-stats=N */
          max_n_unchanged_stats_between_opens =
            xnumtoimax (optarg, 10, 0, شمېر_MAX, "",
                        _("invalid maximum number of unchanged stats"
                          " between opens"),
                        0, XTOINT_MAX_QUIET);
          اكسر;

        حالة DEBUG_پروګرام_OPTION:
          debug = true;
          اكسر;

        حالة DISABLE_INOTIFY_OPTION:
          disable_inotify = true;
          اكسر;

        حالة PID_OPTION:
          إذا (nbpids == pids_alloc)
            pids = xpalloc (pids, &pids_alloc, 1,
                            MIN (لوی_عدد, IDX_MAX), حجم *pids);
          pids[nbpids++] = xdectoumax (optarg, 0, PID_T_MAX, "",
                                       _("invalid PID"), 0);
          اكسر;

        حالة PRESUME_ننوت_PIPE_OPTION:
          presume_ننوت_pipe = true;
          اكسر;

        حالة 'q':
          *سرلیک_ډېرپېښ = never;
          اكسر;

        حالة 's':
          {
            حرف *ep;
            errno = 0;
            مزدوج s = cl_strtod (optarg, &ep);
            إذا (optarg == ep || *ep || ! (0 <= s))
              error (EXIT_FAILURE, 0,
                     _("invalid number of seconds: %s"), quote (optarg));
            *sleep_interval = dtimespec_bound (s, errno);
          }
          اكسر;

        حالة 'v':
          *سرلیک_ډېرپېښ = always;
          اكسر;

        حالة 'z':
          کرښه_پای_ټکی = '\0';
          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخه_CHAR (پروګرام_NAME, AUTHORS);

        حالة '0': حالة '1': حالة '2': حالة '3': حالة '4':
        حالة '5': حالة '6': حالة '7': حالة '8': حالة '9':
          error (EXIT_FAILURE, 0, _("option used in invalid context -- %c"), c);

        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  إذا (reopen_inaccessible_files)
    {
      إذا (!forever)
        {
          reopen_inaccessible_files = false;
          error (0, 0, _("warning: --retry ignored; --retry is useful"
                         " only when following"));
        }
      وإلا إذا (follow_ډېرپېښ == Follow_descriptor)
        error (0, 0, _("warning: --retry only effective for the initial open"));
    }

  إذا (pids && (!forever || (kill (pids[0], 0) < 0 && errno == ENOSYS)))
    {
      error (0, 0,
             _(forever
               ? "warning: --pid=PID is not supported on this system"
               : ("warning: PID ignored;"
                  " --pid=PID is useful only when following")));
      حرر (pids);
      pids = عدم;
    }
}

/* Mark as '.ignore'd each member of F that corresponds to a
   pipe or fifo, and return true if any are not marked.  */
ساكن bool
ignore_fifo_and_pipe (بنية File_spec *f, صحيح n_files)
{
  /* When there is no FILE operand and stdin is a pipe or FIFO
     POSIX requires that tail ignore the -f option.
     Since we allow multiple FILE operands, we extend that to say: with -f,
     ignore any "-" operand that corresponds to a pipe or FIFO.  */
  bool some_viable = false;

  لكل (صحيح i = 0; i < n_files; i++)
    {
      bool is_a_fifo_or_pipe =
        (streq (f[i].name, "-")
         && !f[i].ignore
         && 0 <= f[i].fd
         && (S_ISFIFO (f[i].ډېرپېښ_2)
             || (HAVE_FIFO_PIPES != 1 && isapipe (f[i].fd))));
      إذا (is_a_fifo_or_pipe)
        {
          f[i].fd = -1;
          f[i].errnum = -1;
          f[i].ignore = true;
        }
      وإلا
        some_viable = true;
    }

  أرجع some_viable;
}

صحيح
اصلي (صحيح argc, حرف **argv)
{
  تعداد سرلیک_ډېرپېښ سرلیک_ډېرپېښ = مضرب_files;
  bool ok = true;
  /* If from_start, the number of items to skip before printing; otherwise,
     the number of items at the end of the file to print.  */
  شمېر_t n_units = DEFAULT_N_کرښې;
  صحيح n_files;
  حرف **file;
  بنية File_spec *F;
  bool obsolete_option;

  /* The number of seconds to sleep between iterations.
     During one iteration, every file name or descriptor is checked to
     see if it has changed.  */
  مزدوج sleep_interval = 1.0;

  initialize_main (&argc, &argv);
  سېټ_پروګرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  {
    صحيح p = getpagesize ();
    إذا (IDX_MAX < p)
      xalloc_die ();
    پاڼه_اندازه = p;
  }

  obsolete_option = parse_obsolete_option (argc, argv, &n_units);
  argc -= obsolete_option;
  argv += obsolete_option;
  parse_options (argc, argv, &n_units, &سرلیک_ډېرپېښ, &sleep_interval);

  /* To start printing with item N_UNITS from the start of the file, skip
     N_UNITS - 1 items.  'tail -n +0' is actually meaningless, but for Unix
     compatibility it's treated the same as 'tail -n +1'.  */
  n_units -= from_پیل && 0 < n_units && n_units < شمېر_MAX;

  إذا (optind < argc)
    {
      n_files = argc - optind;
      file = argv + optind;
    }
  وإلا
    {
      ساكن حرف *dummy_stdin = (حرف *) "-";
      n_files = 1;
      file = &dummy_stdin;
    }

  {
    bool وموندل_hyphen = false;

    لكل (صحيح i = 0; i < n_files; i++)
      إذا (streq (file[i], "-"))
        وموندل_hyphen = true;

    /* When following by name, there must be a name.  */
    إذا (وموندل_hyphen && follow_ډېرپېښ == Follow_name)
      error (EXIT_FAILURE, 0, _("cannot follow %s by name"), quoteaf ("-"));

    /* When following forever, and not using simple blocking, warn if
       any file is '-' as the stats() used to check for input are ineffective.
       This is only a warning, since tail's output (before a failing seek,
       and that from any non-stdin files) might still be useful.  */
    إذا (forever && وموندل_hyphen)
      {
        بنية stat دننه_stat;
        bool blocking_stdin;
        blocking_stdin = (!nbpids && follow_ډېرپېښ == Follow_descriptor
                          && n_files == 1 && ! fstat (STDIN_FILENO, &دننه_stat)
                          && ! S_ISREG (دننه_stat.st_ډېرپېښ));

        إذا (! blocking_stdin && isatty (STDIN_FILENO))
          error (0, 0, _("warning: following standard input"
                         " indefinitely is ineffective"));
      }
  }

  /* Don't read anything if we'll never output anything.  */
  إذا (! forever && n_units == (from_پیل ? شمېر_MAX : 0))
    أرجع EXIT_SUCCESS;

  إذا (IDX_MAX < n_files)
    xalloc_die ();
  F = xinmalloc (n_files, حجم *F);
  لكل (صحيح i = 0; i < n_files; i++)
    {
      F[i].name = file[i];
      F[i].prettyname = streq (file[i], "-") ? _("standard input") : file[i];
    }

  إذا (سرلیک_ډېرپېښ == always
      || (سرلیک_ډېرپېښ == مضرب_files && n_files > 1))
    print_سرلیکونه = true;

  xset_دوګونی_ډېرپېښ (STDOUT_FILENO, O_دوګونی);

  لكل (صحيح i = 0; i < n_files; i++)
    ok &= لکۍ_file (&F[i], n_files, n_units);

  إذا (forever && ignore_fifo_and_pipe (F, n_files))
    {
      /* If stdout is a fifo or pipe, then monitor it
         so that we exit if the reader goes away.  */
      بنية stat بهر_stat;
      إذا (fstat (STDOUT_FILENO, &بهر_stat) < 0)
        error (EXIT_FAILURE, errno, _("standard output"));
      monitor_output = (S_ISFIFO (بهر_stat.st_ډېرپېښ)
                        || (HAVE_FIFO_PIPES != 1 && isapipe (STDOUT_FILENO)));

#إذا HAVE_INOTIFY
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
      إذا (!disable_inotify && (tailable_stdin (F, n_files)
                               || any_remote_file (F, n_files)
                               || ! any_non_remote_file (F, n_files)
                               || any_symlinks (F, n_files)
                               || any_non_regular_fifo (F, n_files)
                               || (!ok && follow_ډېرپېښ == Follow_descriptor)))
        disable_inotify = true;

      إذا (!disable_inotify)
        {
          صحيح wd = inotify_init ();
          إذا (0 <= wd)
            {
              /* Flush any output from tail_file, now, since
                 tail_forever_inotify flushes only after writing,
                 not before reading.  */
              إذا (fflush (stdout) < 0)
                write_error ();

              Hash_جدول *ht;
              Hash_جدول *پلار_ht = عدم;
              لکۍ_forever_inotify (wd, F, n_files, sleep_interval,
                                    &ht, &پلار_ht);
              hash_free (ht);
              إذا (پلار_ht)
                hash_free (پلار_ht);
              close (wd);
              errno = 0;
            }
          error (0, errno, _("inotify cannot be used, reverting to polling"));
        }
#د_که_پای
      disable_inotify = true;
      لکۍ_forever (F, n_files, sleep_interval);
    }

  إذا (have_read_stdin && close (STDIN_FILENO) < 0)
    error (EXIT_FAILURE, errno, "-");
  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
