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
#پایان_اگر

/* Linux can optimize the handling of local files.  */
#إذا defined __linux__ || defined __ANDROID__
# include "fs.h"
# include "fs-is-local.h"
# إذا HAVE_SYS_STATFS_H
#  include <sys/statfs.h>
# elif HAVE_SYS_VFS_H
#  include <sys/vfs.h>
# endif
#پایان_اگر

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کن برنامه_NAME "tail"

#تعریف_کن AUTHORS \
  proper_name ("Paul Rubin"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Ian Lance Taylor"), \
  proper_name ("Jim Meyering")

/* Number of items to tail.  */
#تعریف_کن DEFAULT_N_سطرها 10

/* Special values for dump_remainder's N_BYTES parameter.  */
تعداد { COPY_مقصد_EOF = -1, COPY_A_میانگیر = -2 };

/* The type of a large counter.  Although it is always nonnegative,
   it is signed to help signed overflow checking catch any bugs.  */
تعريفنوع intmax_t شمار_t;
#تعریف_کن شمار_MAX INTMAX_MAX

/* FIXME: make Follow_name the default?  */
#تعریف_کن DEFAULT_FOLLOW_نما Follow_descriptor

تعداد Follow_نما
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
#تعریف_کن IS_TAILABLE_FILE_TYPE(نما_3) \
  (S_ISREG (نما_3) || S_ISFIFO (نما_3) || S_ISSOCK (نما_3) || S_ISCHR (نما_3))

ساكن حرف ثابت *ثابت follow_نما_string[] =
{
  "descriptor", "name", عدم
};

ساكن تعداد Follow_نما ثابت follow_نما_map[] =
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
  mode_t نما_2;

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
  صحيح والد_wd;

  /* Offset in NAME of the basename part.  */
  idx_t basename_شروع;
#پایان_اگر

  /* See description of DEFAULT_MAX_N_... below.  */
  شمار_t n_unchanged_stats;
};

/* Keep trying to open a file even if it is inaccessible when tail starts
   or if it becomes inaccessible later -- useful only with -f.  */
ساكن bool reopen_inaccessible_files;

/* If true, interpret the numeric argument as the number of lines.
   Otherwise, interpret it as the number of bytes.  */
ساكن bool شمار_سطرها = true;

/* Whether we follow the name of each file or the file descriptor
   that is initially associated with each name.  */
ساكن تعداد Follow_نما follow_نما = Follow_descriptor;

/* If true, read from the ends of all specified files until killed.  */
ساكن bool forever;

/* If true, monitor output so we exit if pipe reader terminates.  */
ساكن bool monitor_output;

/* If true, count from start of file instead of end.  */
ساكن bool from_شروع;

/* If true, print filename headers.  */
ساكن bool print_سرآیندها;

/* Character to split lines by. */
ساكن حرف سطر_انتها = '\n';

/* When to print the filename banners.  */
تعداد سرآیند_نما
{
  مضرب_files, always, never
};

/* When tailing a file by name, if there have been this many consecutive
   iterations for which the file has not changed, then open/fstat
   the file to determine if that file name is still associated with the
   same device/inode-number pair as before.  This option is meaningful only
   when following by name.  --max-unchanged-stats=N  */
تعداد { DEFAULT_MAX_N_UNCHANGED_STATS_BETWEEN_OPENS = 5 };
ساكن شمار_t max_n_unchanged_stats_between_opens =
  DEFAULT_MAX_N_UNCHANGED_STATS_BETWEEN_OPENS;

/* The process IDs of the processes to watch (those writing the followed
   files, or perhaps other processes the user cares about).  */
ساكن صحيح nbpids = 0;
ساكن pid_t * pids = عدم;
ساكن idx_t pids_alloc;

/* Used to determine the buffer size when scanning backwards in a file.  */
ساكن idx_t صفحه_اندازه;

/* True if we have ever read standard input.  */
ساكن bool have_read_stdin;

/* If nonzero, skip the is-regular-file test used to determine whether
   to use the lseek optimization.  Instead, use the more general (and
   more expensive) code unconditionally. Intended solely for testing.  */
ساكن bool presume_درون_داد_pipe;

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
  PRESUME_درون_داد_PIPE_OPTION,
  LONG_FOLLOW_OPTION,
  DISABLE_INOTIFY_OPTION,
  DEBUG_برنامه_OPTION,
};

ساكن بنية option ثابت long_options[] =
{
  {"bytes", required_آرگومان, عدم, 'c'},
  {"debug", no_آرگومان, عدم, DEBUG_برنامه_OPTION},
  {"follow", optional_آرگومان, عدم, LONG_FOLLOW_OPTION},
  {"lines", required_آرگومان, عدم, 'n'},
  {"max-unchanged-stats", required_آرگومان, عدم,
   MAX_UNCHANGED_STATS_OPTION},
  {"-disable-inotify", no_آرگومان, عدم,
   DISABLE_INOTIFY_OPTION}, /* do not document */
  {"pid", required_آرگومان, عدم, PID_OPTION},
  {"-presume-input-pipe", no_آرگومان, عدم,
   PRESUME_درون_داد_PIPE_OPTION}, /* do not document */
  {"quiet", no_آرگومان, عدم, 'q'},
  {"retry", no_آرگومان, عدم, RETRY_OPTION},
  {"silent", no_آرگومان, عدم, 'q'},
  {"sleep-interval", required_آرگومان, عدم, 's'},
  {"verbose", no_آرگومان, عدم, 'v'},
  {"zero-terminated", no_آرگومان, عدم, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخه_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح وضعیت)
{
  إذا (وضعیت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کن (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              برنامه_name);
      چاپ_کن (_("\
Print the آخرین %d سطرها از each FILE مقصد standard output.\n\
With بیشتر than one FILE, precede each with a سرآیند giving the file name.\n\
"), DEFAULT_N_سطرها);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

     oputs (_("\
  -c, --بایتها=[+]رقم_2\n\
         output the آخرین رقم_2 بایتها;\n\
         or use -c +رقم_2 مقصد output starting with byte رقم_2 از each file\n\
"));
     oputs (_("\
      --debug\n\
         indicate which --follow implementation is استفاده_شده\n\
"));
     oputs (_("\
  -f, --follow[={name|descriptor}]\n\
         output appended داده as the file grows;\n\
         an absent option آرگومان means 'descriptor'\n\
"));
     oputs (_("\
  -F\n\
         same as --follow=name --retry\n\
"));
     oprintf (_("\
  -n, --سطرها=[+]رقم_2\n\
         output the آخرین رقم_2 سطرها, instead از the آخرین %d;\n\
         or use -n +رقم_2 مقصد skip رقم_2-1 سطرها at the شروع\n\
"), DEFAULT_N_سطرها);
     oprintf (_("\
      --max-unchanged-stats=N\n\
         with --follow=name, reopen a FILE which has not\n\
         تغییریافته اندازه after N (افتراضي %d) iterations\n\
         مقصد see إذا it has been unlinked or renamed\n\
         (this is the usual حالة از rotated لگاریتم files);\n\
         with inotify, this option is rarely useful\n\
"), DEFAULT_MAX_N_UNCHANGED_STATS_BETWEEN_OPENS);
     oputs (_("\
      --pid=PID\n\
         with -f, خروج after PID no longer exists;\n\
         can be repeated مقصد watch مضرب processes\n\
"));
     oputs (_("\
  -q, --quiet, --silent\n\
         never output سرآیندها giving file نامها\n\
"));
     oputs (_("\
      --retry\n\
         keep trying مقصد open a file إذا it is inaccessible\n\
"));
     oputs (_("\
  -s, --sleep-interval=N\n\
         with -f, sleep لكل approximately N seconds\n\
         (افتراضي 1.0) between iterations;\n\
         with inotify and --pid=P,\n\
         check process P at کمترین once every N seconds\n\
"));
     oputs (_("\
  -v, --verbose\n\
         always output سرآیندها giving file نامها\n\
"));
     oputs (_("\
  -z, --zero-terminated\n\
         سطر delimiter is NUL, not newline\n\
"));
     oputs (HELP_OPTION_DESCRIPTION);
     oputs (نسخه_OPTION_DESCRIPTION);
     fputs (_("\
\n\
رقم_2 may have a multiplier suffix:\n\
b 512, kB 1000, K 1024, MB 1000*1000, M 1024*1024,\n\
GB 1000*1000*1000, G 1024*1024*1024, and so on لكل T, P, E, Z, Y, R, Q.\n\
دودویی_2 prefixes can be استفاده_شده, too: KiB=K, MiB=M, and so on.\n\
\n\
"), stdout);
     fputs (_("\
With --follow (-f), دم defaults مقصد following the file descriptor, which\n\
means that even إذا a دم'ed file is renamed, دم will استمر مقصد track\n\
its انتها.  This افتراضي behavior is not desirable when you really want مقصد\n\
track the actual name از the file, not the file descriptor (e.g., لگاریتم\n\
rotation).  Use --follow=name درون that حالة.  That causes دم مقصد track the\n\
named file درون a way that accommodates renaming, removal and creation.\n\
"), stdout);
      emit_ancillary_info (برنامه_NAME);
    }
  خروج (وضعیت);
}

/* Ensure exit, either with SIGPIPE or EXIT_FAILURE status.  */
ساكن فراغ
die_pipe (فراغ)
{
  ارفع (SIGPIPE);
  خروج (EXIT_FAILURE);
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

MAYBE_استفادهنشده ساكن bool
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
xlseek (صحيح fd, off_t جابه_جایی, صحيح whence, حرف ثابت *prettyname)
{
  off_t new_جابه_جایی = lseek (fd, جابه_جایی, whence);

  إذا (0 <= new_جابه_جایی)
    أرجع new_جابه_جایی;

  ساكن حرف ثابت *whence_msgid[] = {
    [SEEK_SET] = N_("%s: cannot seek to offset %jd"),
    [SEEK_CUR] = N_("%s: cannot seek to relative offset %jd"),
    [SEEK_END] = N_("%s: cannot seek to end-relative offset %jd")
  };
  intmax_t joffset = جابه_جایی;
  error (EXIT_FAILURE, errno, gettext (whence_msgid[whence]),
         quotef (prettyname), joffset);
}

/* Record a file F with descriptor FD, read position READ_POS, status ST,
   and blocking status BLOCKING.  READ_POS matters only for regular files,
   and READ_POS < 0 means the position is unknown.  */

ساكن فراغ
رکورد_open_fd (بنية File_spec *f, صحيح fd,
                off_t read_pos, بنية stat ثابت *st,
                صحيح blocking)
{
  f->fd = fd;
  f->mtime = get_stat_mtime (st);
  f->st_dev = st->st_dev;
  f->st_ino = st->st_ino;
  f->نما_2 = st->st_نما;
  إذا (S_ISREG (st->st_نما))
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
write_سرآیند (حرف ثابت *prettyname)
{
  ساكن bool اولی_file = true;

  چاپ_کن ("%s==> %s <==\n", اولی_file ? "" : "\n", quotef (prettyname));
  اولی_file = false;
}

/* Write N_BYTES from BUFFER to stdout.
   Exit immediately on error with a single diagnostic.  */

ساكن فراغ
xwrite_stdout (حرف ثابت *میانگیر, idx_t n_بایتها)
{
  إذا (n_بایتها > 0 && fwrite (میانگیر, 1, n_بایتها, stdout) < n_بایتها)
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

ساكن شمار_t
dump_remainder (bool want_سرآیند, حرف ثابت *prettyname, صحيح fd,
                شمار_t n_بایتها)
{
  شمار_t n_read = 0;
  شمار_t n_مانده = n_بایتها;

  افعل
    {
      حرف میانگیر[BUFSIZ];
      idx_t n = n_بایتها < 0 || BUFSIZ < n_مانده ? BUFSIZ : n_مانده;
      ssize_t بایتها_read = اقرأ (fd, میانگیر, n);
      إذا (بایتها_read < 0)
        {
          إذا (errno != EAGAIN)
            error (EXIT_FAILURE, errno, _("error reading %s"),
                   quoteaf (prettyname));
          اكسر;
        }
      إذا (بایتها_read == 0)
        اكسر;
      n_read += بایتها_read;

      إذا (want_سرآیند)
        {
          write_سرآیند (prettyname);
          want_سرآیند = false;
        }
      xwrite_stdout (میانگیر, بایتها_read);
      إذا (n_بایتها == COPY_A_میانگیر)
        اكسر;

      n_مانده -= بایتها_read;
      /* n_remaining < 0 if n_bytes == COPY_TO_EOF.  */
    }
  طالما (n_مانده != 0);

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
file_سطرها (حرف ثابت *prettyname, صحيح fd, بنية stat ثابت *sb,
            شمار_t n_سطرها, off_t شروع_pos, off_t انتها_pos)
{
  حرف *میانگیر;
  idx_t bufsize = BUFSIZ;
  off_t pos = انتها_pos;

  إذا (n_سطرها == 0)
    أرجع pos;

  /* Be careful with files with sizes that are a multiple of the page size,
     as on /proc or /sys file systems these files accept seeking to within
     the file, but then return no data when read.  So use a buffer that's
     at least PAGE_SIZE to avoid seeking within such files.

     We could also indirectly use a large enough buffer through io_blksize()
     however this would be less efficient in the common case, as it would
     generally pick a larger buffer size, resulting in reading more data
     from the end of the file.  */
  affirm (S_ISREG (sb->st_نما));
  إذا (sb->st_اندازه % صفحه_اندازه == 0)
    bufsize = MAX (BUFSIZ, صفحه_اندازه);

  میانگیر = ximalloc (bufsize);

  /* Set 'bytes_read' to the size of the last, probably partial, buffer;
     0 < 'bytes_read' <= 'bufsize'.  */
  idx_t بایتها_مقصد_read = (pos - شروع_pos) % bufsize;
  إذا (بایتها_مقصد_read == 0)
    بایتها_مقصد_read = bufsize;
  /* Make 'pos' a multiple of 'bufsize' (0 if the file is short), so that all
     reads will be on block boundaries, which might increase efficiency.  */
  pos = xlseek (fd, -بایتها_مقصد_read, SEEK_CUR, prettyname);
  ssize_t بایتها_read = اقرأ (fd, میانگیر, بایتها_مقصد_read);
  إذا (بایتها_read < 0)
    {
      pos = -1 - errno;
      error (0, errno, _("error reading %s"), quoteaf (prettyname));
      اذهب free_میانگیر;
    }

  /* Count the incomplete line on files that don't end with a newline.  */
  إذا (بایتها_read && میانگیر[بایتها_read - 1] != سطر_انتها)
    --n_سطرها;

  افعل
    {
      pos += بایتها_read;

      /* Scan backward, counting the newlines in this bufferfull.  */

      idx_t n = بایتها_read;
      طالما (n)
        {
          حرف ثابت *nl;
          nl = memrchr (میانگیر, سطر_انتها, n);
          إذا (nl == عدم)
            اكسر;
          n = nl - میانگیر;
          إذا (n_سطرها-- == 0)
            {
              /* If this newline isn't the last character in the buffer,
                 output the part that is after it.  */
              xwrite_stdout (nl + 1, بایتها_read - (n + 1));
              pos += dump_remainder (false, prettyname, fd, انتها_pos - pos);
              اذهب free_میانگیر;
            }
        }

      /* Not enough newlines in that bufferfull.  */
      إذا (pos - بایتها_read == شروع_pos)
        {
          /* Not enough lines in the file; print everything from
             start_pos to the end.  */
          xwrite_stdout (میانگیر, بایتها_read);
          dump_remainder (false, prettyname, fd, انتها_pos - pos);
          اذهب free_میانگیر;
        }

      pos = xlseek (fd, -(bufsize + بایتها_read), SEEK_CUR, prettyname);
      بایتها_read = اقرأ (fd, میانگیر, bufsize);
      إذا (بایتها_read < 0)
        {
          pos = -1 - errno;
          error (0, errno, _("error reading %s"), quoteaf (prettyname));
          اذهب free_میانگیر;
        }
    }
  طالما (بایتها_read > 0);

free_میانگیر:
  حرر (میانگیر);
  أرجع pos;
}

/* Print the last N_LINES lines from the end of the standard input,
   open for reading as pipe FD.
   Buffer the text as a linked list of LBUFFERs, adding them as needed.
   Return -1 if successful, (-1 - errno) otherwise.  */

ساكن صحيح
pipe_سطرها (حرف ثابت *prettyname, صحيح fd, شمار_t n_سطرها)
{
  بنية linebuffer
  {
    حرف میانگیر[BUFSIZ];
    idx_t nbytes;
    idx_t nlines;
    بنية linebuffer *بعدی;
  };
  تعريفنوع بنية linebuffer LBUFFER;
  LBUFFER *اولی, *آخرین, *tmp;
  idx_t مجموع_سطرها = 0;	/* Total number of newlines in all buffers.  */
  صحيح ret = -1;
  ssize_t n_read;		/* Size in bytes of most recent read */

  اولی = آخرین = xmalloc (حجم (LBUFFER));
  اولی->nbytes = اولی->nlines = 0;
  اولی->بعدی = عدم;
  tmp = xmalloc (حجم (LBUFFER));

  /* Input is always read into a fresh buffer.  */
  طالما (true)
    {
      n_read = اقرأ (fd, tmp->میانگیر, BUFSIZ);
      إذا (n_read <= 0)
        اكسر;
      tmp->nbytes = n_read;
      tmp->nlines = 0;
      tmp->بعدی = عدم;

      /* Count the number of newlines just read.  */
      {
        حرف ثابت *میانگیر_انتها = tmp->میانگیر + n_read;
        حرف ثابت *p = tmp->میانگیر;
        طالما ((p = memchr (p, سطر_انتها, میانگیر_انتها - p)))
          {
            ++p;
            ++tmp->nlines;
          }
      }
      مجموع_سطرها += tmp->nlines;

      /* If there is enough room in the last buffer read, just append the new
         one to it.  This is because when reading from a pipe, 'n_read' can
         often be very small.  */
      إذا (tmp->nbytes + آخرین->nbytes < BUFSIZ)
        {
          کپی_حافظه (&آخرین->میانگیر[آخرین->nbytes], tmp->میانگیر, tmp->nbytes);
          آخرین->nbytes += tmp->nbytes;
          آخرین->nlines += tmp->nlines;
        }
      وإلا
        {
          /* If there's not enough room, link the new buffer onto the end of
             the list, then either free up the oldest buffer for the next
             read if that would leave enough lines, or else malloc a new one.
             Some compaction mechanism is possible but probably not
             worthwhile.  */
          آخرین = آخرین->بعدی = tmp;
          إذا (مجموع_سطرها - اولی->nlines > n_سطرها)
            {
              tmp = اولی;
              مجموع_سطرها -= اولی->nlines;
              اولی = اولی->بعدی;
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
  إذا (آخرین->nbytes == 0)
    اذهب free_lbuffers;

  /* This prevents a core dump when the pipe contains no newlines.  */
  إذا (n_سطرها == 0)
    اذهب free_lbuffers;

  /* Count the incomplete line on files that don't end with a newline.  */
  إذا (آخرین->میانگیر[آخرین->nbytes - 1] != سطر_انتها)
    {
      ++آخرین->nlines;
      ++مجموع_سطرها;
    }

  /* Run through the list, printing lines.  First, skip over unneeded
     buffers.  */
  لكل (tmp = اولی; مجموع_سطرها - tmp->nlines > n_سطرها; tmp = tmp->بعدی)
    مجموع_سطرها -= tmp->nlines;

  /* Find the correct beginning, then print the rest of the file.  */
  {
    حرف ثابت *beg = tmp->میانگیر;
    حرف ثابت *میانگیر_انتها = tmp->میانگیر + tmp->nbytes;
    إذا (مجموع_سطرها > n_سطرها)
      {
        /* Skip 'total_lines' - 'n_lines' newlines.  We made sure that
           'total_lines' - 'n_lines' <= 'tmp->nlines'.  */
        لكل (idx_t j = مجموع_سطرها - n_سطرها; j; --j)
          {
            beg = rawmemchr (beg, سطر_انتها);
            ++beg;
          }
      }

    xwrite_stdout (beg, میانگیر_انتها - beg);
  }

  لكل (tmp = tmp->بعدی; tmp; tmp = tmp->بعدی)
    xwrite_stdout (tmp->میانگیر, tmp->nbytes);

free_lbuffers:
  طالما (اولی)
    {
      tmp = اولی->بعدی;
      حرر (اولی);
      اولی = tmp;
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
pipe_بایتها (حرف ثابت *prettyname, صحيح fd, شمار_t n_بایتها,
            off_t read_pos)
{
  بنية charbuffer
  {
    حرف میانگیر[BUFSIZ];
    idx_t nbytes;
    بنية charbuffer *بعدی;
  };
  تعريفنوع بنية charbuffer CBUFFER;
  CBUFFER *اولی, *آخرین, *tmp;
  idx_t i;			/* Index into buffers.  */
  intmax_t مجموع_بایتها = 0;	/* Total characters in all buffers.  */
  ssize_t n_read;

  إذا (read_pos < 0)
    read_pos = TYPE_MINIMUM (off_t);

  اولی = آخرین = xmalloc (حجم (CBUFFER));
  اولی->nbytes = 0;
  اولی->بعدی = عدم;
  tmp = xmalloc (حجم (CBUFFER));

  /* Input is always read into a fresh buffer.  */
  طالما (true)
    {
      n_read = اقرأ (fd, tmp->میانگیر, BUFSIZ);
      إذا (n_read <= 0)
        اكسر;
      read_pos += n_read;
      tmp->nbytes = n_read;
      tmp->بعدی = عدم;

      مجموع_بایتها += tmp->nbytes;
      /* If there is enough room in the last buffer read, just append the new
         one to it.  This is because when reading from a pipe, 'nbytes' can
         often be very small.  */
      إذا (tmp->nbytes + آخرین->nbytes < BUFSIZ)
        {
          کپی_حافظه (&آخرین->میانگیر[آخرین->nbytes], tmp->میانگیر, tmp->nbytes);
          آخرین->nbytes += tmp->nbytes;
        }
      وإلا
        {
          /* If there's not enough room, link the new buffer onto the end of
             the list, then either free up the oldest buffer for the next
             read if that would leave enough characters, or else malloc a new
             one.  Some compaction mechanism is possible but probably not
             worthwhile.  */
          آخرین = آخرین->بعدی = tmp;
          إذا (مجموع_بایتها - اولی->nbytes > n_بایتها)
            {
              tmp = اولی;
              مجموع_بایتها -= اولی->nbytes;
              اولی = اولی->بعدی;
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
  لكل (tmp = اولی; مجموع_بایتها - tmp->nbytes > n_بایتها; tmp = tmp->بعدی)
    مجموع_بایتها -= tmp->nbytes;

  /* Find the correct beginning, then print the rest of the file.
     We made sure that 'total_bytes' - 'n_bytes' <= 'tmp->nbytes'.  */
  إذا (مجموع_بایتها > n_بایتها)
    i = مجموع_بایتها - n_بایتها;
  وإلا
    i = 0;
  xwrite_stdout (&tmp->میانگیر[i], tmp->nbytes - i);

  لكل (tmp = tmp->بعدی; tmp; tmp = tmp->بعدی)
    xwrite_stdout (tmp->میانگیر, tmp->nbytes);

  إذا (read_pos < 0)
    read_pos = -1;

free_cbuffers:
  طالما (اولی)
    {
      tmp = اولی->بعدی;
      حرر (اولی);
      اولی = tmp;
    }
  أرجع read_pos;
}

/* Skip N_BYTES characters from the start of pipe FD, and print
   any extra characters that were read beyond that.
   Return (-1 - errno) on failure, -1 if EOF, 0 otherwise.  */

ساكن صحيح
شروع_بایتها (حرف ثابت *prettyname, صحيح fd, شمار_t n_بایتها)
{
  حرف میانگیر[BUFSIZ];

  طالما (0 < n_بایتها)
    {
      ssize_t بایتها_read = اقرأ (fd, میانگیر, BUFSIZ);
      إذا (بایتها_read == 0)
        أرجع -1;
      إذا (بایتها_read < 0)
        {
          صحيح ret = -1 - errno;
          error (0, errno, _("error reading %s"), quoteaf (prettyname));
          أرجع ret;
        }
      إذا (بایتها_read <= n_بایتها)
        n_بایتها -= بایتها_read;
      وإلا
        {
          xwrite_stdout (&میانگیر[n_بایتها], بایتها_read - n_بایتها);
          اكسر;
        }
    }

  أرجع 0;
}

/* Skip N_LINES lines at the start of file or pipe FD, and print
   any extra bytes that were read beyond that.
   Return (-1 - errno) on failure, -1 if EOF, 0 otherwise.  */

ساكن صحيح
شروع_سطرها (حرف ثابت *prettyname, صحيح fd, شمار_t n_سطرها)
{
  إذا (n_سطرها == 0)
    أرجع 0;

  طالما (true)
    {
      حرف میانگیر[BUFSIZ];
      ssize_t بایتها_read = اقرأ (fd, میانگیر, BUFSIZ);
      إذا (بایتها_read == 0) /* EOF */
        أرجع -1;
      إذا (بایتها_read < 0) /* error */
        {
          صحيح ret = -1 - errno;
          error (0, errno, _("error reading %s"), quoteaf (prettyname));
          أرجع ret;
        }

      حرف *میانگیر_انتها = میانگیر + بایتها_read;
      حرف *p = میانگیر;
      طالما ((p = memchr (p, سطر_انتها, میانگیر_انتها - p)))
        {
          ++p;
          إذا (--n_سطرها == 0)
            {
              إذا (p < میانگیر_انتها)
                xwrite_stdout (p, میانگیر_انتها - p);
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
#پایان_اگر

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
  وإلا إذا (!IS_TAILABLE_FILE_TYPE (new_stats.st_نما))
    {
      f->errnum = -1;
      f->ignore = ! (reopen_inaccessible_files && follow_نما == Follow_name);
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
      رکورد_open_fd (f, fd, -1, &new_stats, is_stdin ? -1 : blocking);
    }
}

/* Return true if any of the N_FILES files in F are live, i.e., have
   open file descriptors, or should be checked again (see --retry).
   When following descriptors, checking should only continue when any
   of the files is not yet ignored.  */

ساكن bool
any_زنده_files (ثابت بنية File_spec *f, صحيح n_files)
{
  /* In inotify mode, ignore may be set for files
     which may later be replaced with new files.
     So always consider files live in -F mode.  */
  إذا (reopen_inaccessible_files && follow_نما == Follow_name)
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
دم_forever (بنية File_spec *f, صحيح n_files, مزدوج sleep_interval)
{
  صحيح آخرین = n_files - 1;

  ساكن bool debugged;

  /* Flush initial output, in case reading blocks below.  */
  إذا (fflush (stdout) < 0)
    write_error ();

  طالما (true)
    {
      /* Use blocking I/O as an optimization, when it's easy.  */
      bool blocking = (!nbpids && follow_نما == Follow_descriptor
                       && n_files == 1 && 0 <= f[0].fd && !S_ISREG (f[0].نما_2));

      إذا (debug && !debugged)
        {
          debugged = true;
          error (0, 0, "%s", blocking
                             ? _("using blocking mode")
                             : _("using polling mode"));
        }

      bool any_درون_داد = false;

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
          mode_t نما_2 = f[i].نما_2;

          إذا (f[i].blocking != blocking)
            {
              صحيح old_flags = fcntl (fd, F_GETFL);
              صحيح new_flags = old_flags | (blocking ? 0 : O_NONBLOCK);
              إذا (old_flags < 0
                  || (new_flags != old_flags
                      && fcntl (fd, F_SETFL, new_flags) < 0))
                {
                  /* Don't update f[i].blocking if fcntl fails.  */
                  إذا (S_ISREG (f[i].نما_2) && errno == EPERM)
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

              إذا (f[i].نما_2 == stats.st_نما
                  && (! S_ISREG (stats.st_نما)
                      || f[i].read_pos == stats.st_اندازه)
                  && timespec_cmp (f[i].mtime, get_stat_mtime (&stats)) == 0)
                {
                  إذا ((max_n_unchanged_stats_between_opens
                       <= f[i].n_unchanged_stats++)
                      && follow_نما == Follow_name)
                    {
                      recheck (&f[i], f[i].blocking);
                      f[i].n_unchanged_stats = 0;
                    }
                  إذا (fd != f[i].fd || S_ISREG (stats.st_نما) || 1 < n_files)
                    استمر;
                  وإلا
                    read_unchanged = true;
                }

              affirm (fd == f[i].fd);

              /* This file has changed.  Print out what we can, and
                 then keep looping.  */

              f[i].mtime = get_stat_mtime (&stats);
              f[i].نما_2 = stats.st_نما;

              /* reset counter */
              إذا (! read_unchanged)
                f[i].n_unchanged_stats = 0;

              /* XXX: This is only a heuristic, as the file may have also
                 been truncated and written to if st_size >= size
                 (in which case we ignore new data <= size).  */
              إذا (S_ISREG (نما_2) && stats.st_اندازه < f[i].read_pos)
                {
                  error (0, 0, _("%s: file truncated"),
                         quotef (prettyname));
                  /* Assume the file was truncated to 0,
                     and therefore output all "new" data.  */
                  f[i].read_pos = xlseek (fd, 0, SEEK_SET, prettyname);
                }

              إذا (i != آخرین)
                {
                  إذا (print_سرآیندها)
                    write_سرآیند (prettyname);
                  آخرین = i;
                }
            }

          /* Don't read more than st_size on networked file systems
             because it was seen on glusterfs at least, that st_size
             may be smaller than the data read on a _subsequent_ stat call.  */
          شمار_t بایتها_مقصد_read;
          إذا (f[i].blocking)
            بایتها_مقصد_read = COPY_A_میانگیر;
          وإلا إذا (S_ISREG (نما_2) && f[i].remote)
            بایتها_مقصد_read = stats.st_اندازه - f[i].read_pos;
          وإلا
            بایتها_مقصد_read = COPY_مقصد_EOF;

          شمار_t nr = dump_remainder (false, prettyname, fd, بایتها_مقصد_read);
          إذا (0 < nr)
            {
              إذا (S_ISREG (نما_2))
                f[i].read_pos += nr;
              إذا (read_unchanged)
                f[i].n_unchanged_stats = 0;
              any_درون_داد = true;
            }
        }

      إذا (! any_زنده_files (f, n_files))
        {
          error (EXIT_FAILURE, 0, _("no files remaining"));
          اكسر;
        }

      إذا ((!any_درون_داد || blocking) && fflush (stdout) < 0)
        write_error ();

      check_output_alive ();

      إذا (!any_درون_داد)
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
    إذا (0 <= f[i].fd && ! S_ISREG (f[i].نما_2) && ! S_ISFIFO (f[i].نما_2))
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
wd_hasher (ثابت فراغ *مدخل, size_t tabsize)
{
  ثابت بنية File_spec *spec = مدخل;
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
والد_name_hasher (ثابت فراغ *مدخل, size_t tabsize)
{
  ثابت بنية File_spec *spec = مدخل;
  حرف ثابت *name = spec->name + spec->basename_شروع;
  /* hash_string already reduces modulo TABSIZE; mix in parent_wd so the
     same basename under different directories does not collide by design.  */
  أرجع (hash_string (name, tabsize) + (size_t) spec->والد_wd) % tabsize;
}

ساكن bool
والد_name_comparator (ثابت فراغ *e1, ثابت فراغ *e2)
{
  ثابت بنية File_spec *a = e1;
  ثابت بنية File_spec *b = e2;
  أرجع (a->والد_wd == b->والد_wd
          && streq (a->name + a->basename_شروع,
                    b->name + b->basename_شروع));
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
  إذا (S_ISREG (fspec->نما_2) && stats.st_اندازه < fspec->read_pos)
    {
      error (0, 0, _("%s: file truncated"), quotef (prettyname));
      fspec->read_pos = xlseek (fspec->fd, 0, SEEK_SET, prettyname);
    }
  وإلا إذا (S_ISREG (fspec->نما_2) && stats.st_اندازه == fspec->read_pos
           && timespec_cmp (fspec->mtime, get_stat_mtime (&stats)) == 0)
    أرجع;

  bool want_سرآیند = print_سرآیندها && (fspec != *prev_fspec);

  شمار_t nr = dump_remainder (want_سرآیند, prettyname, fspec->fd, COPY_مقصد_EOF);
  إذا (0 < nr)
    {
      إذا (S_ISREG (fspec->نما_2))
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
دم_forever_inotify (صحيح wd, بنية File_spec *f, صحيح n_files,
                      مزدوج sleep_interval, Hash_جدول **wd_مقصد_namep,
                      Hash_جدول **والد_by_namep)
{
# إذا دم_TEST_SLEEP
  /* Delay between open() and inotify_add_watch()
     to help trigger different cases.  */
  xnanosleep (1000000);
# endif
  صحيح max_realloc = 3;

  /* Map an inotify watch descriptor to the name of the file it's watching.  */
  Hash_جدول *wd_مقصد_name;

  /* Map (parent_wd, basename) to File_spec for Follow_name
     directory events.  */
  Hash_جدول *والد_by_name = عدم;

  bool یافت_watchable_file = false;
  bool tailed_but_unwatchable = false;
  bool یافت_unwatchable_dir = false;
  bool no_inotify_resources = false;
  بنية File_spec *prev_fspec;
  idx_t evlen = 0;
  حرف *evbuf;
  idx_t evbuf_off = 0;

  *والد_by_namep = عدم;

  wd_مقصد_name = hash_initialize (n_files, عدم, wd_hasher, wd_comparator,
                                عدم);
  إذا (! wd_مقصد_name)
    xalloc_die ();
  *wd_مقصد_namep = wd_مقصد_name;

  إذا (follow_نما == Follow_name)
    {
      والد_by_name = hash_initialize (n_files, عدم, والد_name_hasher,
                                        والد_name_comparator, عدم);
      إذا (! والد_by_name)
        xalloc_die ();
      *والد_by_namep = والد_by_name;
    }

  /* The events mask used with inotify on files (not directories).  */
  uint32_t inotify_wd_ماسک = درون_MODIFY;
  /* TODO: Perhaps monitor these events in Follow_descriptor mode also,
     to tag reported file names with "deleted", "moved" etc.  */
  إذا (follow_نما == Follow_name)
    inotify_wd_ماسک |= (درون_ATTRIB | درون_DELETE_SELF | درون_MOVE_SELF);

  /* Add an inotify watch for each watched file.  If -F is specified then watch
     its parent directory too, in this way when they re-appear we can add them
     again to the watch list.  */
  لكل (صحيح i = 0; i < n_files; i++)
    {
      إذا (!f[i].ignore)
        {
          idx_t fnlen = طول_رشته (f[i].name);
          إذا (evlen < fnlen)
            evlen = fnlen;

          f[i].wd = -1;

          إذا (follow_نما == Follow_name)
            {
              idx_t dirlen = dir_len (f[i].name);
              حرف prev = f[i].name[dirlen];
              f[i].basename_شروع = آخرین_component (f[i].name) - f[i].name;

              f[i].name[dirlen] = '\0';

               /* It's fine to add the same directory more than once.
                  In that case the same watch descriptor is returned.  */
              f[i].والد_wd = inotify_جمع_کن_watch (wd, dirlen ? f[i].name : ".",
                                                  (درون_CREATE | درون_DELETE
                                                   | درون_MOVED_مقصد | درون_ATTRIB
                                                   | درون_DELETE_SELF));

              f[i].name[dirlen] = prev;

              إذا (f[i].والد_wd < 0)
                {
                  إذا (errno != ENOSPC) /* suppress confusing error.  */
                    error (0, errno, _("cannot watch parent directory of %s"),
                           quoteaf (f[i].name));
                  وإلا
                    error (0, 0, _("inotify resources exhausted"));
                  یافت_unwatchable_dir = true;
                  /* We revert to polling below.  Note invalid uses
                     of the inotify API will still be diagnosed.  */
                  اكسر;
                }

              إذا (hash_درج (والد_by_name, &(f[i])) == عدم)
                xalloc_die ();
            }

          f[i].wd = inotify_جمع_کن_watch (wd, f[i].name, inotify_wd_ماسک);

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

          إذا (hash_درج (wd_مقصد_name, &(f[i])) == عدم)
            xalloc_die ();

          یافت_watchable_file = true;
        }
    }

  /* Linux kernel 2.6.24 at least has a bug where eventually, ENOSPC is always
     returned by inotify_add_watch.  In any case we should revert to polling
     when there are no inotify resources.  Also a specified directory may not
     be currently present or accessible, so revert to polling.  Also an already
     tailed but unwatchable due rename/unlink race, should also revert.  */
  إذا (no_inotify_resources || یافت_unwatchable_dir
      || (follow_نما == Follow_descriptor && tailed_but_unwatchable))
    أرجع;
  إذا (follow_نما == Follow_descriptor && !یافت_watchable_file)
    خروج (EXIT_FAILURE);

  prev_fspec = &(f[n_files - 1]);

  /* Check files again.  New files or data can be available since last time we
     checked and before they are watched by inotify.  */
  لكل (صحيح i = 0; i < n_files; i++)
    {
      إذا (! f[i].ignore)
        {
          /* check for new files.  */
          إذا (follow_نما == Follow_name)
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
      إذا (follow_نما == Follow_name
          && ! reopen_inaccessible_files
          && hash_get_n_entries (wd_مقصد_name) == 0)
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
                    خروج (EXIT_SUCCESS);

                  إذا (!some_writers_exist () || sleep_interval <= 0)
                    delay = 0;
                  وإلا إذا (sleep_interval < بیشترین_صحیح / 1000 - 1)
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
      إذا ((ev->ماسک & درون_DELETE_SELF) && ! ev->len)
        {
          لكل (صحيح i = 0; i < n_files; i++)
            {
              إذا (ev->wd == f[i].والد_wd)
                {
                  error (0, 0,
                      _("directory containing watched file was removed"));
                  أرجع;
                }
            }
        }

      إذا (ev->len) /* event on ev->name in watched directory.  */
        {
          بنية File_spec کلید;
          إذا (! والد_by_name)
            استمر;

          /* Probe key: basename is the whole string at KEY.NAME.  */
          کلید.والد_wd = ev->wd;
          کلید.name = ev->name;
          کلید.basename_شروع = 0;
          fspec = hash_lookup (والد_by_name, &کلید);

          /* It is not a watched file.  */
          إذا (! fspec)
            استمر;

          صحيح new_wd = -1;
          bool deleting = !! (ev->ماسک & درون_DELETE);

          إذا (! deleting)
            {
              /* Adding the same inode again will look up any existing wd.  */
              new_wd = inotify_جمع_کن_watch (wd, fspec->name, inotify_wd_ماسک);
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
                  hash_remove (wd_مقصد_name, fspec);
                }

              fspec->wd = new_wd;

              إذا (new_wd < 0)
                استمر;

              /* If the file was moved then inotify will use the source file wd
                for the destination file.  Make sure the key is not present in
                the table.  */
              بنية File_spec *prev = hash_remove (wd_مقصد_name, fspec);
              إذا (prev && prev != fspec)
                {
                  إذا (follow_نما == Follow_name)
                    recheck (prev, false);
                  prev->wd = -1;
                  close_fd (prev->fd, prev);
                }

              إذا (hash_درج (wd_مقصد_name, fspec) == عدم)
                xalloc_die ();
            }

          إذا (follow_نما == Follow_name)
            recheck (fspec, false);
        }
      وإلا
        {
          بنية File_spec کلید;
          کلید.wd = ev->wd;
          fspec = hash_lookup (wd_مقصد_name, &کلید);
        }

      إذا (! fspec)
        استمر;

      إذا (ev->ماسک & (درون_ATTRIB | درون_DELETE | درون_DELETE_SELF | درون_MOVE_SELF))
        {
          /* Note for IN_MOVE_SELF (the file we're watching has
             been clobbered via a rename) without --retry we leave the watch
             in place since it may still be part of the set
             of watched names.  */
          إذا (ev->ماسک & درون_DELETE_SELF
              || (!reopen_inaccessible_files && (ev->ماسک & درون_MOVE_SELF)))
            {
              inotify_rm_watch (wd, fspec->wd);
              hash_remove (wd_مقصد_name, fspec);
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
#پایان_اگر

/* Output the last bytes of the file PRETTYNAME open for reading
   in FD and with status ST.  Output the last N_BYTES bytes.
   Return (-1 - errno) on failure, otherwise the resulting file offset
   if known, otherwise -1.  */

ساكن off_t
دم_بایتها (حرف ثابت *prettyname, صحيح fd, بنية stat ثابت *st,
            شمار_t n_بایتها)
{
  off_t current_pos
    = (presume_درون_داد_pipe
       ? -1
       : lseek (fd, from_شروع ? MIN (n_بایتها, OFF_T_MAX) : 0, SEEK_CUR));

  إذا (from_شروع)
    {
      إذا (current_pos < 0)
        {
          صحيح t = شروع_بایتها (prettyname, fd, n_بایتها);
          إذا (t < 0)
            أرجع t;
        }
      n_بایتها = COPY_مقصد_EOF;
    }
  وإلا
    {
      off_t initial_pos = current_pos;
      off_t انتها_pos = -1;

      إذا (0 <= current_pos)
        {
          إذا (usable_st_اندازه (st))
            {
              /* Use st_size only if it's so large that this is
                 probably not a /proc or similar file, where st_size
                 is notional.  */
              off_t smallish_اندازه = STP_BLKSIZE (st);
              إذا (smallish_اندازه < st->st_اندازه)
                انتها_pos = st->st_اندازه;
            }
          وإلا
            {
              off_t minus_n;
              إذا (ckd_sub (&minus_n, 0, n_بایتها))
                انتها_pos = 0;
              وإلا
                {
                  off_t e_n_pos = lseek (fd, minus_n, SEEK_END);
                  إذا (0 <= e_n_pos)
                    {
                      current_pos = e_n_pos;
                      انتها_pos = e_n_pos + n_بایتها;
                    }
                  وإلا
                    {
                      /* POSIX says errno is unreliable here.
                         Perhaps the file is smaller than N_BYTES.
                         Try again at file end.  */
                      off_t e_pos = lseek (fd, 0, SEEK_END);
                      إذا (0 <= e_pos)
                        current_pos = انتها_pos = e_pos;
                    }
               }
            }
        }

      /* If the end is known and more than N_BYTES after the initial
         position, go to N_BYTES before the end; otherwise go back to
         the initial position.  But do not lseek if lseek already failed.  */
      off_t pos = (initial_pos < انتها_pos && n_بایتها < انتها_pos - initial_pos
                   ? انتها_pos - n_بایتها
                   : initial_pos);
      إذا (pos != current_pos)
        current_pos = xlseek (fd, pos, SEEK_SET, prettyname);

      إذا (انتها_pos < 0)
        أرجع pipe_بایتها (prettyname, fd, n_بایتها, current_pos);
    }

  شمار_t nr = dump_remainder (false, prettyname, fd, n_بایتها);
  أرجع current_pos < 0 ? -1 : current_pos + nr;
}

/* Output the last lines of the file PRETTYNAME open for reading
   in FD and with status ST.  Output the last N_LINES lines.
   Return (-1 - errno) on failure, otherwise the resulting file offset
   if known, otherwise -1.  */

ساكن off_t
دم_سطرها (حرف ثابت *prettyname, صحيح fd, بنية stat ثابت *st,
            شمار_t n_سطرها)
{
  إذا (from_شروع)
    {
      /* If skipping all input use lseek if possible, for speed.  */
      إذا (OFF_T_MAX <= n_سطرها)
        {
          off_t e = lseek (fd, 0, SEEK_END);
          إذا (0 <= e)
            أرجع e;
        }

      صحيح t = شروع_سطرها (prettyname, fd, n_سطرها);
      إذا (t < 0)
        أرجع t;
      dump_remainder (false, prettyname, fd, COPY_مقصد_EOF);
      أرجع -1;
    }
  وإلا
    {
      /* Use file_lines only if FD is a regular file and SEEK_END
         reports more data.  */
      off_t شروع_pos = (!presume_درون_داد_pipe && S_ISREG (st->st_نما)
                         ? lseek (fd, 0, SEEK_CUR)
                         : -1);
      off_t انتها_pos = شروع_pos < 0 ? -1 : lseek (fd, 0, SEEK_END);
      أرجع (انتها_pos < 0
              ? pipe_سطرها (prettyname, fd, n_سطرها)
              : شروع_pos < انتها_pos
              ? file_سطرها (prettyname, fd, st, n_سطرها, شروع_pos, انتها_pos)
              : شروع_pos != انتها_pos
              ? (/* Do not read from before the start offset,
                    even if the input file shrank.  */
                 xlseek (fd, شروع_pos, SEEK_SET, prettyname))
              : شروع_pos);
    }
}

/* Display the last part of file FILENAME,
   open for reading via FD and with status *ST.
   Return (-1 - errno) on failure, otherwise the resulting file offset
   if known, otherwise -1.  */

ساكن off_t
دم (حرف ثابت *filename, صحيح fd, بنية stat ثابت *st, شمار_t n_units)
{
  أرجع (شمار_سطرها ? دم_سطرها : دم_بایتها) (filename, fd, st, n_units);
}

/* Display the last N_UNITS units of the file described by F.
   Return true if successful.  */

ساكن bool
دم_file (بنية File_spec *f, شمار_t n_files, شمار_t n_units)
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
      xset_دودویی_نما (STDIN_FILENO, O_دودویی);
    }
  وإلا
    fd = open (f->name, O_RDONLY | O_دودویی | (nonblocking ? O_NONBLOCK : 0));

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
      إذا (print_سرآیندها)
        write_سرآیند (f->prettyname);

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
          read_pos = دم (f->prettyname, fd, &stats, n_units);
          ok = -1 <= read_pos;
          f->errnum = ok ? 0 : -1 - read_pos;

          إذا (IS_TAILABLE_FILE_TYPE (stats.st_نما))
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
              رکورد_open_fd (f, fd, read_pos, &stats, (is_stdin ? -1 : 1));
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
parse_obsolete_option (صحيح argc, حرف * ثابت *argv, شمار_t *n_units)
{
  bool t_from_شروع;
  bool t_شمار_سطرها = true;
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

      t_from_شروع = true;
      اكسر;

    حالة '-':
      /* In the non-obsolete form, "-" is standard input and "-c"
         requires an option-argument.  The obsolete multidigit options
         are supported as a GNU extension even when conforming to
         POSIX 1003.1-2001 or later, so don't complain about them.  */
      إذا (!obsolete_usage && !p[p[0] == 'c'])
        أرجع false;

      t_from_شروع = false;
      اكسر;
    }

  شمار_t n;
  إذا (!c_isdigit (*p))
    n = DEFAULT_N_سطرها;
  وإلا
    لكل (n = 0; c_isdigit (*p); p++)
      n = ckd_mul (&n, n, 10) || ckd_جمع_کن (&n, n, *p - '0') ? شمار_MAX : n;

  بدل (*p)
    {
    حالة 'b': n = ckd_mul (&n, n, 512) ? شمار_MAX : n; FALLTHROUGH;
    حالة 'c': t_شمار_سطرها = false; FALLTHROUGH;
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
  from_شروع = t_from_شروع;
  شمار_سطرها = t_شمار_سطرها;
  forever = t_forever;

  أرجع true;
}

ساكن فراغ
parse_options (صحيح argc, حرف **argv,
               شمار_t *n_units, تعداد سرآیند_نما *سرآیند_نما,
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
          follow_نما = Follow_name;
          reopen_inaccessible_files = true;
          اكسر;

        حالة 'c':
        حالة 'n':
          شمار_سطرها = (c == 'n');
          إذا (*optarg == '+')
            from_شروع = true;
          وإلا إذا (*optarg == '-')
            ++optarg;

          *n_units = xnumtoimax (optarg, 10, 0, شمار_MAX, "bkKmMGTPEZYRQ0",
                                 (شمار_سطرها
                                  ? _("invalid number of lines")
                                  : _("invalid number of bytes"))
                                 , 0, XTOINT_MAX_QUIET);
          اكسر;

        حالة 'f':
        حالة LONG_FOLLOW_OPTION:
          forever = true;
          إذا (optarg == عدم)
            follow_نما = DEFAULT_FOLLOW_نما;
          وإلا
            follow_نما = XARGMATCH ("--follow", optarg,
                                     follow_نما_string, follow_نما_map);
          اكسر;

        حالة RETRY_OPTION:
          reopen_inaccessible_files = true;
          اكسر;

        حالة MAX_UNCHANGED_STATS_OPTION:
          /* --max-unchanged-stats=N */
          max_n_unchanged_stats_between_opens =
            xnumtoimax (optarg, 10, 0, شمار_MAX, "",
                        _("invalid maximum number of unchanged stats"
                          " between opens"),
                        0, XTOINT_MAX_QUIET);
          اكسر;

        حالة DEBUG_برنامه_OPTION:
          debug = true;
          اكسر;

        حالة DISABLE_INOTIFY_OPTION:
          disable_inotify = true;
          اكسر;

        حالة PID_OPTION:
          إذا (nbpids == pids_alloc)
            pids = xpalloc (pids, &pids_alloc, 1,
                            MIN (بیشترین_صحیح, IDX_MAX), حجم *pids);
          pids[nbpids++] = xdectoumax (optarg, 0, PID_T_MAX, "",
                                       _("invalid PID"), 0);
          اكسر;

        حالة PRESUME_درون_داد_PIPE_OPTION:
          presume_درون_داد_pipe = true;
          اكسر;

        حالة 'q':
          *سرآیند_نما = never;
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
          *سرآیند_نما = always;
          اكسر;

        حالة 'z':
          سطر_انتها = '\0';
          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخه_CHAR (برنامه_NAME, AUTHORS);

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
      وإلا إذا (follow_نما == Follow_descriptor)
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
         && (S_ISFIFO (f[i].نما_2)
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
اصلی (صحيح argc, حرف **argv)
{
  تعداد سرآیند_نما سرآیند_نما = مضرب_files;
  bool ok = true;
  /* If from_start, the number of items to skip before printing; otherwise,
     the number of items at the end of the file to print.  */
  شمار_t n_units = DEFAULT_N_سطرها;
  صحيح n_files;
  حرف **file;
  بنية File_spec *F;
  bool obsolete_option;

  /* The number of seconds to sleep between iterations.
     During one iteration, every file name or descriptor is checked to
     see if it has changed.  */
  مزدوج sleep_interval = 1.0;

  initialize_main (&argc, &argv);
  مجموعه_برنامه_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  {
    صحيح p = getpagesize ();
    إذا (IDX_MAX < p)
      xalloc_die ();
    صفحه_اندازه = p;
  }

  obsolete_option = parse_obsolete_option (argc, argv, &n_units);
  argc -= obsolete_option;
  argv += obsolete_option;
  parse_options (argc, argv, &n_units, &سرآیند_نما, &sleep_interval);

  /* To start printing with item N_UNITS from the start of the file, skip
     N_UNITS - 1 items.  'tail -n +0' is actually meaningless, but for Unix
     compatibility it's treated the same as 'tail -n +1'.  */
  n_units -= from_شروع && 0 < n_units && n_units < شمار_MAX;

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
    bool یافت_hyphen = false;

    لكل (صحيح i = 0; i < n_files; i++)
      إذا (streq (file[i], "-"))
        یافت_hyphen = true;

    /* When following by name, there must be a name.  */
    إذا (یافت_hyphen && follow_نما == Follow_name)
      error (EXIT_FAILURE, 0, _("cannot follow %s by name"), quoteaf ("-"));

    /* When following forever, and not using simple blocking, warn if
       any file is '-' as the stats() used to check for input are ineffective.
       This is only a warning, since tail's output (before a failing seek,
       and that from any non-stdin files) might still be useful.  */
    إذا (forever && یافت_hyphen)
      {
        بنية stat درون_stat;
        bool blocking_stdin;
        blocking_stdin = (!nbpids && follow_نما == Follow_descriptor
                          && n_files == 1 && ! fstat (STDIN_FILENO, &درون_stat)
                          && ! S_ISREG (درون_stat.st_نما));

        إذا (! blocking_stdin && isatty (STDIN_FILENO))
          error (0, 0, _("warning: following standard input"
                         " indefinitely is ineffective"));
      }
  }

  /* Don't read anything if we'll never output anything.  */
  إذا (! forever && n_units == (from_شروع ? شمار_MAX : 0))
    أرجع EXIT_SUCCESS;

  إذا (IDX_MAX < n_files)
    xalloc_die ();
  F = xinmalloc (n_files, حجم *F);
  لكل (صحيح i = 0; i < n_files; i++)
    {
      F[i].name = file[i];
      F[i].prettyname = streq (file[i], "-") ? _("standard input") : file[i];
    }

  إذا (سرآیند_نما == always
      || (سرآیند_نما == مضرب_files && n_files > 1))
    print_سرآیندها = true;

  xset_دودویی_نما (STDOUT_FILENO, O_دودویی);

  لكل (صحيح i = 0; i < n_files; i++)
    ok &= دم_file (&F[i], n_files, n_units);

  إذا (forever && ignore_fifo_and_pipe (F, n_files))
    {
      /* If stdout is a fifo or pipe, then monitor it
         so that we exit if the reader goes away.  */
      بنية stat بیرون_stat;
      إذا (fstat (STDOUT_FILENO, &بیرون_stat) < 0)
        error (EXIT_FAILURE, errno, _("standard output"));
      monitor_output = (S_ISFIFO (بیرون_stat.st_نما)
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
                               || (!ok && follow_نما == Follow_descriptor)))
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
              Hash_جدول *والد_ht = عدم;
              دم_forever_inotify (wd, F, n_files, sleep_interval,
                                    &ht, &والد_ht);
              hash_free (ht);
              إذا (والد_ht)
                hash_free (والد_ht);
              close (wd);
              errno = 0;
            }
          error (0, errno, _("inotify cannot be used, reverting to polling"));
        }
#پایان_اگر
      disable_inotify = true;
      دم_forever (F, n_files, sleep_interval);
    }

  إذا (have_read_stdin && close (STDIN_FILENO) < 0)
    error (EXIT_FAILURE, errno, "-");
  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
