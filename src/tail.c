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

#شامل <config.h>

#شامل <stdio.h>
#شامل <getopt.h>
#شامل <sys/types.h>
#شامل <signal.h>

#شامل "system.h"
#شامل "argmatch.h"
#شامل "assure.h"
#شامل "c-ctype.h"
#شامل "cl-strtod.h"
#شامل "dtimespec-bound.h"
#شامل "fcntl--.h"
#شامل "iopoll.h"
#شامل "isapipe.h"
#شامل "issymlink.h"
#شامل "posixver.h"
#شامل "quote.h"
#شامل "stat-size.h"
#شامل "stat-time.h"
#شامل "xbinary-io.h"
#شامل "xdectoint.h"
#شامل "xnanosleep.h"

#اگر HAVE_INOTIFY
# include "hash.h"
# include <poll.h>
# include <sys/inotify.h>
#اگر_ختم

/* Linux can optimize the handling of local files.  */
#اگر defined __linux__ || defined __ANDROID__
# include "fs.h"
# include "fs-is-local.h"
# اگر HAVE_SYS_STATFS_H
#  include <sys/statfs.h>
# elif HAVE_SYS_VFS_H
#  include <sys/vfs.h>
# endif
#اگر_ختم

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "tail"

#تعریف_کرو AUTHORS \
  proper_name ("Paul Rubin"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Ian Lance Taylor"), \
  proper_name ("Jim Meyering")

/* Number of items to tail.  */
#تعریف_کرو DEFAULT_N_سطریں 10

/* Special values for dump_remainder's N_BYTES parameter.  */
شمار { COPY_مقصد_EOF = -1, COPY_A_بفر = -2 };

/* The type of a large counter.  Although it is always nonnegative,
   it is signed to help signed overflow checking catch any bugs.  */
قسمتعریف intmax_t گنتی_t;
#تعریف_کرو گنتی_MAX INTMAX_MAX

/* FIXME: make Follow_name the default?  */
#تعریف_کرو DEFAULT_FOLLOW_منوال Follow_descriptor

شمار Follow_منوال
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
#تعریف_کرو IS_TAILABLE_FILE_TYPE(منوال_3) \
  (S_ISREG (منوال_3) || S_ISFIFO (منوال_3) || S_ISSOCK (منوال_3) || S_ISCHR (منوال_3))

ساکن حرف ثابت *ثابت follow_منوال_string[] =
{
  "descriptor", "name", تہی
};

ساکن شمار Follow_منوال ثابت follow_منوال_map[] =
{
  Follow_descriptor, Follow_name,
};

ساخت File_spec
{
  /* The actual file name, or "-" for stdin.  */
  حرف *name;

  /* The prettified name; the same as NAME unless NAME is "-".  */
  حرف ثابت *prettyname;

  /* Attributes of the file the last time we checked.  */
  ساخت timespec mtime;
  dev_t st_dev;
  ino_t st_ino;
  mode_t منوال_2;

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
  صحیح fd;

  /* 0 if the file's last op succeeded, otherwise the op's errno if
     applicable, otherwise -1.  */
  صحیح errnum;

  /* 1 if O_NONBLOCK is clear, 0 if set, -1 if not known.  */
  صحیح blocking;

#اگر HAVE_INOTIFY
  /* The watch descriptor used by inotify.  */
  صحیح wd;

  /* The parent directory watch descriptor.  It is used only
   * when Follow_name is used.  */
  صحیح والد_wd;

  /* Offset in NAME of the basename part.  */
  idx_t basename_آغاز;
#اگر_ختم

  /* See description of DEFAULT_MAX_N_... below.  */
  گنتی_t n_unchanged_stats;
};

/* Keep trying to open a file even if it is inaccessible when tail starts
   or if it becomes inaccessible later -- useful only with -f.  */
ساکن bool reopen_inaccessible_files;

/* If true, interpret the numeric argument as the number of lines.
   Otherwise, interpret it as the number of bytes.  */
ساکن bool گنتی_سطریں = true;

/* Whether we follow the name of each file or the file descriptor
   that is initially associated with each name.  */
ساکن شمار Follow_منوال follow_منوال = Follow_descriptor;

/* If true, read from the ends of all specified files until killed.  */
ساکن bool forever;

/* If true, monitor output so we exit if pipe reader terminates.  */
ساکن bool monitor_output;

/* If true, count from start of file instead of end.  */
ساکن bool from_آغاز;

/* If true, print filename headers.  */
ساکن bool print_سرخیاں;

/* Character to split lines by. */
ساکن حرف سطر_انتہا = '\n';

/* When to print the filename banners.  */
شمار سرخی_منوال
{
  مضرب_files, always, never
};

/* When tailing a file by name, if there have been this many consecutive
   iterations for which the file has not changed, then open/fstat
   the file to determine if that file name is still associated with the
   same device/inode-number pair as before.  This option is meaningful only
   when following by name.  --max-unchanged-stats=N  */
شمار { DEFAULT_MAX_N_UNCHANGED_STATS_BETWEEN_OPENS = 5 };
ساکن گنتی_t max_n_unchanged_stats_between_opens =
  DEFAULT_MAX_N_UNCHANGED_STATS_BETWEEN_OPENS;

/* The process IDs of the processes to watch (those writing the followed
   files, or perhaps other processes the user cares about).  */
ساکن صحیح nbpids = 0;
ساکن pid_t * pids = تہی;
ساکن idx_t pids_alloc;

/* Used to determine the buffer size when scanning backwards in a file.  */
ساکن idx_t صفحہ_سائز;

/* True if we have ever read standard input.  */
ساکن bool have_read_stdin;

/* If nonzero, skip the is-regular-file test used to determine whether
   to use the lseek optimization.  Instead, use the more general (and
   more expensive) code unconditionally. Intended solely for testing.  */
ساکن bool presume_داخلہ_pipe;

/* If nonzero then don't use inotify even if available.  */
ساکن bool disable_inotify;

/* Annotate the output with extra info to aid the user.  */
ساکن bool debug;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
شمار
{
  RETRY_OPTION = CHAR_MAX + 1,
  MAX_UNCHANGED_STATS_OPTION,
  PID_OPTION,
  PRESUME_داخلہ_PIPE_OPTION,
  LONG_FOLLOW_OPTION,
  DISABLE_INOTIFY_OPTION,
  DEBUG_پروگرام_OPTION,
};

ساکن ساخت option ثابت long_options[] =
{
  {"bytes", required_دلیل, تہی, 'c'},
  {"debug", no_دلیل, تہی, DEBUG_پروگرام_OPTION},
  {"follow", optional_دلیل, تہی, LONG_FOLLOW_OPTION},
  {"lines", required_دلیل, تہی, 'n'},
  {"max-unchanged-stats", required_دلیل, تہی,
   MAX_UNCHANGED_STATS_OPTION},
  {"-disable-inotify", no_دلیل, تہی,
   DISABLE_INOTIFY_OPTION}, /* do not document */
  {"pid", required_دلیل, تہی, PID_OPTION},
  {"-presume-input-pipe", no_دلیل, تہی,
   PRESUME_داخلہ_PIPE_OPTION}, /* do not document */
  {"quiet", no_دلیل, تہی, 'q'},
  {"retry", no_دلیل, تہی, RETRY_OPTION},
  {"silent", no_دلیل, تہی, 'q'},
  {"sleep-interval", required_دلیل, تہی, 's'},
  {"verbose", no_دلیل, تہی, 'v'},
  {"zero-terminated", no_دلیل, تہی, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0}
};

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              پروگرام_name);
      لکھو (_("\
Print the آخری %d سطریں کا each FILE مقصد standard output.\n\
With مزید than one FILE, precede each with a سرخی giving the file name.\n\
"), DEFAULT_N_سطریں);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

     oputs (_("\
  -c, --بائٹیں=[+]عدد_2\n\
         output the آخری عدد_2 بائٹیں;\n\
         or use -c +عدد_2 مقصد output starting with byte عدد_2 کا each file\n\
"));
     oputs (_("\
      --debug\n\
         indicate which --follow implementation is مستعمل\n\
"));
     oputs (_("\
  -f, --follow[={name|descriptor}]\n\
         output appended ڈیٹا as the file grows;\n\
         an absent option دلیل means 'descriptor'\n\
"));
     oputs (_("\
  -F\n\
         same as --follow=name --retry\n\
"));
     oprintf (_("\
  -n, --سطریں=[+]عدد_2\n\
         output the آخری عدد_2 سطریں, instead کا the آخری %d;\n\
         or use -n +عدد_2 مقصد skip عدد_2-1 سطریں at the آغاز\n\
"), DEFAULT_N_سطریں);
     oprintf (_("\
      --max-unchanged-stats=N\n\
         with --follow=name, reopen a FILE which has not\n\
         بدلا سائز after N (طےشدہ %d) iterations\n\
         مقصد see اگر it has been unlinked or renamed\n\
         (this is the usual حالت کا rotated لوگارتھم files);\n\
         with inotify, this option is rarely useful\n\
"), DEFAULT_MAX_N_UNCHANGED_STATS_BETWEEN_OPENS);
     oputs (_("\
      --pid=PID\n\
         with -f, باہر_نکلو after PID no longer exists;\n\
         can be repeated مقصد watch مضرب processes\n\
"));
     oputs (_("\
  -q, --quiet, --silent\n\
         never output سرخیاں giving file نام\n\
"));
     oputs (_("\
      --retry\n\
         keep trying مقصد open a file اگر it is inaccessible\n\
"));
     oputs (_("\
  -s, --sleep-interval=N\n\
         with -f, sleep برائے approximately N seconds\n\
         (طےشدہ 1.0) between iterations;\n\
         with inotify and --pid=P,\n\
         check process P at کم_سے_کم once every N seconds\n\
"));
     oputs (_("\
  -v, --verbose\n\
         always output سرخیاں giving file نام\n\
"));
     oputs (_("\
  -z, --zero-terminated\n\
         سطر delimiter is NUL, not newline\n\
"));
     oputs (HELP_OPTION_DESCRIPTION);
     oputs (نسخہ_OPTION_DESCRIPTION);
     fputs (_("\
\n\
عدد_2 may have a multiplier suffix:\n\
b 512, kB 1000, K 1024, MB 1000*1000, M 1024*1024,\n\
GB 1000*1000*1000, G 1024*1024*1024, and so on برائے T, P, E, Z, Y, R, Q.\n\
ثنائی_2 prefixes can be مستعمل, too: KiB=K, MiB=M, and so on.\n\
\n\
"), stdout);
     fputs (_("\
With --follow (-f), دم defaults مقصد following the file descriptor, which\n\
means that even اگر a دم'ed file is renamed, دم will جاری مقصد track\n\
its انتہا.  This طےشدہ behavior is not desirable when you really want مقصد\n\
track the actual name کا the file, not the file descriptor (e.g., لوگارتھم\n\
rotation).  Use --follow=name اندر that حالت.  That causes دم مقصد track the\n\
named file اندر a way that accommodates renaming, removal and creation.\n\
"), stdout);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

/* Ensure exit, either with SIGPIPE or EXIT_FAILURE status.  */
ساکن خالی
die_pipe (خالی)
{
  اٹھاؤ (SIGPIPE);
  باہر_نکلو (EXIT_FAILURE);
}

/* If the output has gone away, then terminate
   as we would if we had written to this output.  */
ساکن خالی
check_output_alive (خالی)
{
  اگر (! monitor_output)
    واپس;

  اگر (iopoll (-1, STDOUT_FILENO, false) == IOPOLL_BROKEN_OUTPUT)
    die_pipe ();
}

MAYBE_غیر_مستعمل ساکن bool
valid_file_spec (ساخت File_spec ثابت *f)
{
  /* Exactly one of the following subexpressions must be true. */
  واپس (f->fd < 0) ^ (f->errnum == 0);
}

/* Call lseek with the specified arguments FD, OFFSET, WHENCE.
   FD corresponds to PRETTYNAME.
   Give a diagnostic and exit nonzero if lseek fails.
   Otherwise, return the resulting offset.  */

ساکن off_t
xlseek (صحیح fd, off_t انحراف, صحیح whence, حرف ثابت *prettyname)
{
  off_t new_انحراف = lseek (fd, انحراف, whence);

  اگر (0 <= new_انحراف)
    واپس new_انحراف;

  ساکن حرف ثابت *whence_msgid[] = {
    [SEEK_SET] = N_("%s: cannot seek to offset %jd"),
    [SEEK_CUR] = N_("%s: cannot seek to relative offset %jd"),
    [SEEK_END] = N_("%s: cannot seek to end-relative offset %jd")
  };
  intmax_t joffset = انحراف;
  error (EXIT_FAILURE, errno, gettext (whence_msgid[whence]),
         quotef (prettyname), joffset);
}

/* Record a file F with descriptor FD, read position READ_POS, status ST,
   and blocking status BLOCKING.  READ_POS matters only for regular files,
   and READ_POS < 0 means the position is unknown.  */

ساکن خالی
ریکارڈ_open_fd (ساخت File_spec *f, صحیح fd,
                off_t read_pos, ساخت stat ثابت *st,
                صحیح blocking)
{
  f->fd = fd;
  f->mtime = get_stat_mtime (st);
  f->st_dev = st->st_dev;
  f->st_ino = st->st_ino;
  f->منوال_2 = st->st_منوال;
  اگر (S_ISREG (st->st_منوال))
    f->read_pos = (read_pos < 0
                   ? xlseek (fd, 0, SEEK_CUR, f->prettyname)
                   : read_pos);
  f->blocking = blocking;
  f->n_unchanged_stats = 0;
  f->ignore = false;
}

/* Close the file with descriptor FD and spec F.  */

ساکن خالی
close_fd (صحیح fd, ساخت File_spec ثابت *f)
{
  اگر (STDIN_FILENO < fd && close (fd) < 0)
    error (0, errno, _("closing %s (fd=%d)"), quoteaf (f->prettyname), fd);
}

ساکن خالی
write_سرخی (حرف ثابت *prettyname)
{
  ساکن bool پہلا_file = true;

  لکھو ("%s==> %s <==\n", پہلا_file ? "" : "\n", quotef (prettyname));
  پہلا_file = false;
}

/* Write N_BYTES from BUFFER to stdout.
   Exit immediately on error with a single diagnostic.  */

ساکن خالی
xwrite_stdout (حرف ثابت *بفر, idx_t n_بائٹیں)
{
  اگر (n_بائٹیں > 0 && fwrite (بفر, 1, n_بائٹیں, stdout) < n_بائٹیں)
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

ساکن گنتی_t
dump_remainder (bool want_سرخی, حرف ثابت *prettyname, صحیح fd,
                گنتی_t n_بائٹیں)
{
  گنتی_t n_read = 0;
  گنتی_t n_بقایا = n_بائٹیں;

  کرو
    {
      حرف بفر[BUFSIZ];
      idx_t n = n_بائٹیں < 0 || BUFSIZ < n_بقایا ? BUFSIZ : n_بقایا;
      ssize_t بائٹیں_read = اقرأ (fd, بفر, n);
      اگر (بائٹیں_read < 0)
        {
          اگر (errno != EAGAIN)
            error (EXIT_FAILURE, errno, _("error reading %s"),
                   quoteaf (prettyname));
          توڑو;
        }
      اگر (بائٹیں_read == 0)
        توڑو;
      n_read += بائٹیں_read;

      اگر (want_سرخی)
        {
          write_سرخی (prettyname);
          want_سرخی = false;
        }
      xwrite_stdout (بفر, بائٹیں_read);
      اگر (n_بائٹیں == COPY_A_بفر)
        توڑو;

      n_بقایا -= بائٹیں_read;
      /* n_remaining < 0 if n_bytes == COPY_TO_EOF.  */
    }
  جب (n_بقایا != 0);

  واپس n_read;
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

ساکن off_t
file_سطریں (حرف ثابت *prettyname, صحیح fd, ساخت stat ثابت *sb,
            گنتی_t n_سطریں, off_t آغاز_pos, off_t انتہا_pos)
{
  حرف *بفر;
  idx_t bufsize = BUFSIZ;
  off_t pos = انتہا_pos;

  اگر (n_سطریں == 0)
    واپس pos;

  /* Be careful with files with sizes that are a multiple of the page size,
     as on /proc or /sys file systems these files accept seeking to within
     the file, but then return no data when read.  So use a buffer that's
     at least PAGE_SIZE to avoid seeking within such files.

     We could also indirectly use a large enough buffer through io_blksize()
     however this would be less efficient in the common case, as it would
     generally pick a larger buffer size, resulting in reading more data
     from the end of the file.  */
  affirm (S_ISREG (sb->st_منوال));
  اگر (sb->st_سائز % صفحہ_سائز == 0)
    bufsize = MAX (BUFSIZ, صفحہ_سائز);

  بفر = ximalloc (bufsize);

  /* Set 'bytes_read' to the size of the last, probably partial, buffer;
     0 < 'bytes_read' <= 'bufsize'.  */
  idx_t بائٹیں_مقصد_read = (pos - آغاز_pos) % bufsize;
  اگر (بائٹیں_مقصد_read == 0)
    بائٹیں_مقصد_read = bufsize;
  /* Make 'pos' a multiple of 'bufsize' (0 if the file is short), so that all
     reads will be on block boundaries, which might increase efficiency.  */
  pos = xlseek (fd, -بائٹیں_مقصد_read, SEEK_CUR, prettyname);
  ssize_t بائٹیں_read = اقرأ (fd, بفر, بائٹیں_مقصد_read);
  اگر (بائٹیں_read < 0)
    {
      pos = -1 - errno;
      error (0, errno, _("error reading %s"), quoteaf (prettyname));
      جاؤ free_بفر;
    }

  /* Count the incomplete line on files that don't end with a newline.  */
  اگر (بائٹیں_read && بفر[بائٹیں_read - 1] != سطر_انتہا)
    --n_سطریں;

  کرو
    {
      pos += بائٹیں_read;

      /* Scan backward, counting the newlines in this bufferfull.  */

      idx_t n = بائٹیں_read;
      جب (n)
        {
          حرف ثابت *nl;
          nl = memrchr (بفر, سطر_انتہا, n);
          اگر (nl == تہی)
            توڑو;
          n = nl - بفر;
          اگر (n_سطریں-- == 0)
            {
              /* If this newline isn't the last character in the buffer,
                 output the part that is after it.  */
              xwrite_stdout (nl + 1, بائٹیں_read - (n + 1));
              pos += dump_remainder (false, prettyname, fd, انتہا_pos - pos);
              جاؤ free_بفر;
            }
        }

      /* Not enough newlines in that bufferfull.  */
      اگر (pos - بائٹیں_read == آغاز_pos)
        {
          /* Not enough lines in the file; print everything from
             start_pos to the end.  */
          xwrite_stdout (بفر, بائٹیں_read);
          dump_remainder (false, prettyname, fd, انتہا_pos - pos);
          جاؤ free_بفر;
        }

      pos = xlseek (fd, -(bufsize + بائٹیں_read), SEEK_CUR, prettyname);
      بائٹیں_read = اقرأ (fd, بفر, bufsize);
      اگر (بائٹیں_read < 0)
        {
          pos = -1 - errno;
          error (0, errno, _("error reading %s"), quoteaf (prettyname));
          جاؤ free_بفر;
        }
    }
  جب (بائٹیں_read > 0);

free_بفر:
  آزاد (بفر);
  واپس pos;
}

/* Print the last N_LINES lines from the end of the standard input,
   open for reading as pipe FD.
   Buffer the text as a linked list of LBUFFERs, adding them as needed.
   Return -1 if successful, (-1 - errno) otherwise.  */

ساکن صحیح
pipe_سطریں (حرف ثابت *prettyname, صحیح fd, گنتی_t n_سطریں)
{
  ساخت linebuffer
  {
    حرف بفر[BUFSIZ];
    idx_t nbytes;
    idx_t nlines;
    ساخت linebuffer *اگلا;
  };
  قسمتعریف ساخت linebuffer LBUFFER;
  LBUFFER *پہلا, *آخری, *tmp;
  idx_t کل_سطریں = 0;	/* Total number of newlines in all buffers.  */
  صحیح ret = -1;
  ssize_t n_read;		/* Size in bytes of most recent read */

  پہلا = آخری = xmalloc (حجم (LBUFFER));
  پہلا->nbytes = پہلا->nlines = 0;
  پہلا->اگلا = تہی;
  tmp = xmalloc (حجم (LBUFFER));

  /* Input is always read into a fresh buffer.  */
  جب (true)
    {
      n_read = اقرأ (fd, tmp->بفر, BUFSIZ);
      اگر (n_read <= 0)
        توڑو;
      tmp->nbytes = n_read;
      tmp->nlines = 0;
      tmp->اگلا = تہی;

      /* Count the number of newlines just read.  */
      {
        حرف ثابت *بفر_انتہا = tmp->بفر + n_read;
        حرف ثابت *p = tmp->بفر;
        جب ((p = memchr (p, سطر_انتہا, بفر_انتہا - p)))
          {
            ++p;
            ++tmp->nlines;
          }
      }
      کل_سطریں += tmp->nlines;

      /* If there is enough room in the last buffer read, just append the new
         one to it.  This is because when reading from a pipe, 'n_read' can
         often be very small.  */
      اگر (tmp->nbytes + آخری->nbytes < BUFSIZ)
        {
          حافظہ_نقل (&آخری->بفر[آخری->nbytes], tmp->بفر, tmp->nbytes);
          آخری->nbytes += tmp->nbytes;
          آخری->nlines += tmp->nlines;
        }
      ورنہ
        {
          /* If there's not enough room, link the new buffer onto the end of
             the list, then either free up the oldest buffer for the next
             read if that would leave enough lines, or else malloc a new one.
             Some compaction mechanism is possible but probably not
             worthwhile.  */
          آخری = آخری->اگلا = tmp;
          اگر (کل_سطریں - پہلا->nlines > n_سطریں)
            {
              tmp = پہلا;
              کل_سطریں -= پہلا->nlines;
              پہلا = پہلا->اگلا;
            }
          ورنہ
            tmp = xmalloc (حجم (LBUFFER));
        }
    }

  آزاد (tmp);

  اگر (n_read < 0 && errno != EAGAIN)
    {
      ret = -1 - errno;
      error (0, errno, _("error reading %s"), quoteaf (prettyname));
      جاؤ free_lbuffers;
    }

  /* If the file is empty, then bail out.  */
  اگر (آخری->nbytes == 0)
    جاؤ free_lbuffers;

  /* This prevents a core dump when the pipe contains no newlines.  */
  اگر (n_سطریں == 0)
    جاؤ free_lbuffers;

  /* Count the incomplete line on files that don't end with a newline.  */
  اگر (آخری->بفر[آخری->nbytes - 1] != سطر_انتہا)
    {
      ++آخری->nlines;
      ++کل_سطریں;
    }

  /* Run through the list, printing lines.  First, skip over unneeded
     buffers.  */
  برائے (tmp = پہلا; کل_سطریں - tmp->nlines > n_سطریں; tmp = tmp->اگلا)
    کل_سطریں -= tmp->nlines;

  /* Find the correct beginning, then print the rest of the file.  */
  {
    حرف ثابت *beg = tmp->بفر;
    حرف ثابت *بفر_انتہا = tmp->بفر + tmp->nbytes;
    اگر (کل_سطریں > n_سطریں)
      {
        /* Skip 'total_lines' - 'n_lines' newlines.  We made sure that
           'total_lines' - 'n_lines' <= 'tmp->nlines'.  */
        برائے (idx_t j = کل_سطریں - n_سطریں; j; --j)
          {
            beg = rawmemchr (beg, سطر_انتہا);
            ++beg;
          }
      }

    xwrite_stdout (beg, بفر_انتہا - beg);
  }

  برائے (tmp = tmp->اگلا; tmp; tmp = tmp->اگلا)
    xwrite_stdout (tmp->بفر, tmp->nbytes);

free_lbuffers:
  جب (پہلا)
    {
      tmp = پہلا->اگلا;
      آزاد (پہلا);
      پہلا = tmp;
    }
  واپس ret;
}

/* Print the last N_BYTES characters from the end of FD.
   Work even if the input is a pipe.
   This is a stripped down version of pipe_lines.
   The initial file offset is READ_POS if nonnegative, otherwise unknown.
   Return (-1 - errno) on failure, otherwise the resulting file offset
   if known, otherwise -1.  */

ساکن off_t
pipe_بائٹیں (حرف ثابت *prettyname, صحیح fd, گنتی_t n_بائٹیں,
            off_t read_pos)
{
  ساخت charbuffer
  {
    حرف بفر[BUFSIZ];
    idx_t nbytes;
    ساخت charbuffer *اگلا;
  };
  قسمتعریف ساخت charbuffer CBUFFER;
  CBUFFER *پہلا, *آخری, *tmp;
  idx_t i;			/* Index into buffers.  */
  intmax_t کل_بائٹیں = 0;	/* Total characters in all buffers.  */
  ssize_t n_read;

  اگر (read_pos < 0)
    read_pos = TYPE_MINIMUM (off_t);

  پہلا = آخری = xmalloc (حجم (CBUFFER));
  پہلا->nbytes = 0;
  پہلا->اگلا = تہی;
  tmp = xmalloc (حجم (CBUFFER));

  /* Input is always read into a fresh buffer.  */
  جب (true)
    {
      n_read = اقرأ (fd, tmp->بفر, BUFSIZ);
      اگر (n_read <= 0)
        توڑو;
      read_pos += n_read;
      tmp->nbytes = n_read;
      tmp->اگلا = تہی;

      کل_بائٹیں += tmp->nbytes;
      /* If there is enough room in the last buffer read, just append the new
         one to it.  This is because when reading from a pipe, 'nbytes' can
         often be very small.  */
      اگر (tmp->nbytes + آخری->nbytes < BUFSIZ)
        {
          حافظہ_نقل (&آخری->بفر[آخری->nbytes], tmp->بفر, tmp->nbytes);
          آخری->nbytes += tmp->nbytes;
        }
      ورنہ
        {
          /* If there's not enough room, link the new buffer onto the end of
             the list, then either free up the oldest buffer for the next
             read if that would leave enough characters, or else malloc a new
             one.  Some compaction mechanism is possible but probably not
             worthwhile.  */
          آخری = آخری->اگلا = tmp;
          اگر (کل_بائٹیں - پہلا->nbytes > n_بائٹیں)
            {
              tmp = پہلا;
              کل_بائٹیں -= پہلا->nbytes;
              پہلا = پہلا->اگلا;
            }
          ورنہ
            {
              tmp = xmalloc (حجم (CBUFFER));
            }
        }
    }

  آزاد (tmp);

  اگر (n_read < 0 && errno != EAGAIN)
    {
      read_pos = -1 - errno;
      error (0, errno, _("error reading %s"), quoteaf (prettyname));
      جاؤ free_cbuffers;
    }

  /* Run through the list, printing characters.  First, skip over unneeded
     buffers.  */
  برائے (tmp = پہلا; کل_بائٹیں - tmp->nbytes > n_بائٹیں; tmp = tmp->اگلا)
    کل_بائٹیں -= tmp->nbytes;

  /* Find the correct beginning, then print the rest of the file.
     We made sure that 'total_bytes' - 'n_bytes' <= 'tmp->nbytes'.  */
  اگر (کل_بائٹیں > n_بائٹیں)
    i = کل_بائٹیں - n_بائٹیں;
  ورنہ
    i = 0;
  xwrite_stdout (&tmp->بفر[i], tmp->nbytes - i);

  برائے (tmp = tmp->اگلا; tmp; tmp = tmp->اگلا)
    xwrite_stdout (tmp->بفر, tmp->nbytes);

  اگر (read_pos < 0)
    read_pos = -1;

free_cbuffers:
  جب (پہلا)
    {
      tmp = پہلا->اگلا;
      آزاد (پہلا);
      پہلا = tmp;
    }
  واپس read_pos;
}

/* Skip N_BYTES characters from the start of pipe FD, and print
   any extra characters that were read beyond that.
   Return (-1 - errno) on failure, -1 if EOF, 0 otherwise.  */

ساکن صحیح
آغاز_بائٹیں (حرف ثابت *prettyname, صحیح fd, گنتی_t n_بائٹیں)
{
  حرف بفر[BUFSIZ];

  جب (0 < n_بائٹیں)
    {
      ssize_t بائٹیں_read = اقرأ (fd, بفر, BUFSIZ);
      اگر (بائٹیں_read == 0)
        واپس -1;
      اگر (بائٹیں_read < 0)
        {
          صحیح ret = -1 - errno;
          error (0, errno, _("error reading %s"), quoteaf (prettyname));
          واپس ret;
        }
      اگر (بائٹیں_read <= n_بائٹیں)
        n_بائٹیں -= بائٹیں_read;
      ورنہ
        {
          xwrite_stdout (&بفر[n_بائٹیں], بائٹیں_read - n_بائٹیں);
          توڑو;
        }
    }

  واپس 0;
}

/* Skip N_LINES lines at the start of file or pipe FD, and print
   any extra bytes that were read beyond that.
   Return (-1 - errno) on failure, -1 if EOF, 0 otherwise.  */

ساکن صحیح
آغاز_سطریں (حرف ثابت *prettyname, صحیح fd, گنتی_t n_سطریں)
{
  اگر (n_سطریں == 0)
    واپس 0;

  جب (true)
    {
      حرف بفر[BUFSIZ];
      ssize_t بائٹیں_read = اقرأ (fd, بفر, BUFSIZ);
      اگر (بائٹیں_read == 0) /* EOF */
        واپس -1;
      اگر (بائٹیں_read < 0) /* error */
        {
          صحیح ret = -1 - errno;
          error (0, errno, _("error reading %s"), quoteaf (prettyname));
          واپس ret;
        }

      حرف *بفر_انتہا = بفر + بائٹیں_read;
      حرف *p = بفر;
      جب ((p = memchr (p, سطر_انتہا, بفر_انتہا - p)))
        {
          ++p;
          اگر (--n_سطریں == 0)
            {
              اگر (p < بفر_انتہا)
                xwrite_stdout (p, بفر_انتہا - p);
              واپس 0;
            }
        }
    }
}

/* Return true if FD might be open on a file residing on a remote file system,
   false if FD is known to be local.  F is the file's spec.
   If fstatfs fails, give a diagnostic and return true.
   If fstatfs cannot be called, return true.  */
ساکن bool
fremote (صحیح fd, ساخت File_spec ثابت *f)
{
  bool remote = true;           /* be conservative (poll by default).  */

#اگر HAVE_FSTATFS && HAVE_STRUCT_STATFS_F_TYPE \
 && (defined __linux__ || defined __ANDROID__)
  ساخت statfs buf;
  صحیح err = fstatfs (fd, &buf);
  اگر (err != 0)
    {
      /* On at least linux-2.6.38, fstatfs fails with ENOSYS when FD
         is open on a pipe.  Treat that like a remote file.  */
      اگر (errno != ENOSYS)
        error (0, errno, _("cannot determine location of %s. "
                           "reverting to polling"), quoteaf (f->prettyname));
    }
  ورنہ
    {
      /* Treat unrecognized file systems as "remote", so caller polls.
         Note README-release has instructions for syncing the internal
         list with the latest Linux kernel file system constants.  */
      remote = is_local_fs_type (buf.f_type) <= 0;
    }
#اگر_ختم

  واپس remote;
}

/* open/fstat F->name and handle changes.  */
ساکن خالی
recheck (ساخت File_spec *f, bool blocking)
{
  ساخت stat new_stats;
  bool ok = false;
  bool is_stdin = (streq (f->name, "-"));
  صحیح پچھلا_errnum = f->errnum;
  bool new_file;
  صحیح fd = (is_stdin
            ? STDIN_FILENO
            : open (f->name, O_RDONLY | (blocking ? 0 : O_NONBLOCK)));
  صحیح open_errno = fd < 0 ? errno : 0;

  affirm (valid_file_spec (f));

  اگر (! disable_inotify && issymlink (f->name) == 1)
    {
      /* Diagnose the edge case where a regular file is changed
         to a symlink.  We avoid inotify with symlinks since
         it's awkward to match between symlink name and target.  */
      f->errnum = -1;
      f->ignore = true;

      error (0, 0, _("%s has been replaced with an untailable symbolic link"),
             quoteaf (f->prettyname));
    }
  ورنہ اگر (fd < 0 || fstat (fd, &new_stats) < 0)
    {
      f->errnum = fd < 0 ? open_errno : errno;
      اگر (fd < 0)
        {
          اگر (f->tailable)
            {
              /* FIXME-maybe: detect the case in which the file first becomes
                 unreadable (perms), and later becomes readable again and can
                 be seen to be the same file (dev/ino).  Otherwise, tail prints
                 the entire contents of the file when it becomes readable.  */
              error (0, f->errnum, _("%s has become inaccessible"),
                     quoteaf (f->prettyname));
            }
          ورنہ
            {
              /* say nothing... it's still not tailable */
            }
        }
      ورنہ اگر (پچھلا_errnum != f->errnum)
        error (0, f->errnum, "%s", quotef (f->prettyname));
    }
  ورنہ اگر (!IS_TAILABLE_FILE_TYPE (new_stats.st_منوال))
    {
      f->errnum = -1;
      f->ignore = ! (reopen_inaccessible_files && follow_منوال == Follow_name);
      اگر (f->tailable || ! (پچھلا_errnum < 0 || پچھلا_errnum == EISDIR))
        error (0, 0, _("%s has been replaced with an untailable file%s"),
               quoteaf (f->prettyname),
               f->ignore ? _("; giving up on this name") : "");
    }
  ورنہ اگر ((f->remote = fremote (fd, f)) && ! disable_inotify)
    {
      f->errnum = -1;
      error (0, 0, _("%s has been replaced with an untailable remote file"),
             quoteaf (f->prettyname));
      f->ignore = true;
    }
  ورنہ
    {
      ok = true;
      f->errnum = 0;
    }

  f->tailable = ok;

  new_file = false;
  اگر (!ok)
    {
      close_fd (fd, f);
      close_fd (f->fd, f);
      f->fd = -1;
    }
  ورنہ اگر (پچھلا_errnum && پچھلا_errnum != ENOENT)
    {
      new_file = true;
      affirm (f->fd < 0);
      error (0, 0, _("%s has become accessible"), quoteaf (f->prettyname));
    }
  ورنہ اگر (f->fd < 0)
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
  ورنہ اگر (!SAME_INODE (*f, new_stats))
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
  ورنہ
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
  اگر (new_file)
    {
      /* Start at the file's current position, normally the beginning.  */
      ریکارڈ_open_fd (f, fd, -1, &new_stats, is_stdin ? -1 : blocking);
    }
}

/* Return true if any of the N_FILES files in F are live, i.e., have
   open file descriptors, or should be checked again (see --retry).
   When following descriptors, checking should only continue when any
   of the files is not yet ignored.  */

ساکن bool
any_زندہ_files (ثابت ساخت File_spec *f, صحیح n_files)
{
  /* In inotify mode, ignore may be set for files
     which may later be replaced with new files.
     So always consider files live in -F mode.  */
  اگر (reopen_inaccessible_files && follow_منوال == Follow_name)
    واپس true;

  برائے (صحیح i = 0; i < n_files; i++)
    {
      اگر (0 <= f[i].fd)
        واپس true;
      ورنہ
        {
          اگر (! f[i].ignore && reopen_inaccessible_files)
            واپس true;
        }
    }

  واپس false;
}

/* Return true if any writers exist, even as zombies.  */

ساکن bool
some_writers_exist (خالی)
{
  /* To avoid some (though not all) races if the kernel reuses PIDs,
     check all PIDs instead of returning merely because one PID exists.  */
  برائے (idx_t i = 0; i < nbpids; )
    {
      اگر (kill (pids[i], 0) < 0 && errno == ESRCH)
        {
          nbpids--;
          memmove (&pids[i], &pids[i + 1], (nbpids - i) * حجم pids[i]);
        }
      ورنہ
        i++;
    }


  واپس 0 < nbpids;
}

/* Tail N_FILES files forever, or until killed.
   The pertinent information for each file is stored in an entry of F.
   Loop over each of them, doing an fstat to see if they have changed size,
   and an occasional open/fstat to see if any dev/ino pair has changed.
   If none of them have changed size in one iteration, sleep for a
   while and try again.  Continue until the user interrupts us.  */

ساکن خالی
دم_forever (ساخت File_spec *f, صحیح n_files, دوہرا sleep_interval)
{
  صحیح آخری = n_files - 1;

  ساکن bool debugged;

  /* Flush initial output, in case reading blocks below.  */
  اگر (fflush (stdout) < 0)
    write_error ();

  جب (true)
    {
      /* Use blocking I/O as an optimization, when it's easy.  */
      bool blocking = (!nbpids && follow_منوال == Follow_descriptor
                       && n_files == 1 && 0 <= f[0].fd && !S_ISREG (f[0].منوال_2));

      اگر (debug && !debugged)
        {
          debugged = true;
          error (0, 0, "%s", blocking
                             ? _("using blocking mode")
                             : _("using polling mode"));
        }

      bool any_داخلہ = false;

      برائے (صحیح i = 0; i < n_files; i++)
        {
          ساخت stat stats;

          اگر (f[i].ignore)
            جاری;

          صحیح fd = f[i].fd;
          اگر (fd < 0)
            {
              recheck (&f[i], blocking);
              جاری;
            }

          حرف ثابت *prettyname = f[i].prettyname;
          mode_t منوال_2 = f[i].منوال_2;

          اگر (f[i].blocking != blocking)
            {
              صحیح old_flags = fcntl (fd, F_GETFL);
              صحیح new_flags = old_flags | (blocking ? 0 : O_NONBLOCK);
              اگر (old_flags < 0
                  || (new_flags != old_flags
                      && fcntl (fd, F_SETFL, new_flags) < 0))
                {
                  /* Don't update f[i].blocking if fcntl fails.  */
                  اگر (S_ISREG (f[i].منوال_2) && errno == EPERM)
                    {
                      /* This happens when using tail -f on a file with
                         the append-only attribute.  */
                    }
                  ورنہ
                    error (EXIT_FAILURE, errno,
                           _("%s: cannot change nonblocking mode"),
                           quotef (prettyname));
                }
              ورنہ
                f[i].blocking = blocking;
            }

          bool read_unchanged = false;
          اگر (!f[i].blocking)
            {
              اگر (fstat (fd, &stats) < 0)
                {
                  f[i].fd = -1;
                  f[i].errnum = errno;
                  error (0, errno, "%s", quotef (prettyname));
                  close (fd); /* ignore failure */
                  جاری;
                }

              اگر (f[i].منوال_2 == stats.st_منوال
                  && (! S_ISREG (stats.st_منوال)
                      || f[i].read_pos == stats.st_سائز)
                  && timespec_cmp (f[i].mtime, get_stat_mtime (&stats)) == 0)
                {
                  اگر ((max_n_unchanged_stats_between_opens
                       <= f[i].n_unchanged_stats++)
                      && follow_منوال == Follow_name)
                    {
                      recheck (&f[i], f[i].blocking);
                      f[i].n_unchanged_stats = 0;
                    }
                  اگر (fd != f[i].fd || S_ISREG (stats.st_منوال) || 1 < n_files)
                    جاری;
                  ورنہ
                    read_unchanged = true;
                }

              affirm (fd == f[i].fd);

              /* This file has changed.  Print out what we can, and
                 then keep looping.  */

              f[i].mtime = get_stat_mtime (&stats);
              f[i].منوال_2 = stats.st_منوال;

              /* reset counter */
              اگر (! read_unchanged)
                f[i].n_unchanged_stats = 0;

              /* XXX: This is only a heuristic, as the file may have also
                 been truncated and written to if st_size >= size
                 (in which case we ignore new data <= size).  */
              اگر (S_ISREG (منوال_2) && stats.st_سائز < f[i].read_pos)
                {
                  error (0, 0, _("%s: file truncated"),
                         quotef (prettyname));
                  /* Assume the file was truncated to 0,
                     and therefore output all "new" data.  */
                  f[i].read_pos = xlseek (fd, 0, SEEK_SET, prettyname);
                }

              اگر (i != آخری)
                {
                  اگر (print_سرخیاں)
                    write_سرخی (prettyname);
                  آخری = i;
                }
            }

          /* Don't read more than st_size on networked file systems
             because it was seen on glusterfs at least, that st_size
             may be smaller than the data read on a _subsequent_ stat call.  */
          گنتی_t بائٹیں_مقصد_read;
          اگر (f[i].blocking)
            بائٹیں_مقصد_read = COPY_A_بفر;
          ورنہ اگر (S_ISREG (منوال_2) && f[i].remote)
            بائٹیں_مقصد_read = stats.st_سائز - f[i].read_pos;
          ورنہ
            بائٹیں_مقصد_read = COPY_مقصد_EOF;

          گنتی_t nr = dump_remainder (false, prettyname, fd, بائٹیں_مقصد_read);
          اگر (0 < nr)
            {
              اگر (S_ISREG (منوال_2))
                f[i].read_pos += nr;
              اگر (read_unchanged)
                f[i].n_unchanged_stats = 0;
              any_داخلہ = true;
            }
        }

      اگر (! any_زندہ_files (f, n_files))
        {
          error (EXIT_FAILURE, 0, _("no files remaining"));
          توڑو;
        }

      اگر ((!any_داخلہ || blocking) && fflush (stdout) < 0)
        write_error ();

      check_output_alive ();

      اگر (!any_داخلہ)
        {
          اگر (pids)
            {
              /* Exit loop if it was already known that no writers exist.
                 Otherwise, to avoid a race, loop one more time
                 without sleeping if no writers exist now.  */
              اگر (!nbpids)
                توڑو;
              اگر (!some_writers_exist ())
                جاری;
            }

          اگر (xnanosleep (sleep_interval))
            error (EXIT_FAILURE, errno, _("cannot read realtime clock"));
        }
    }
}

#اگر HAVE_INOTIFY

/* Return true if any of the N_FILES files in F is remote, i.e., has
   an open file descriptor and is on a network file system.  */

ساکن bool
any_remote_file (ثابت ساخت File_spec *f, صحیح n_files)
{
  برائے (صحیح i = 0; i < n_files; i++)
    اگر (0 <= f[i].fd && f[i].remote)
      واپس true;
  واپس false;
}

/* Return true if any of the N_FILES files in F is non remote, i.e., has
   an open file descriptor and is not on a network file system.  */

ساکن bool
any_non_remote_file (ثابت ساخت File_spec *f, صحیح n_files)
{
  برائے (صحیح i = 0; i < n_files; i++)
    اگر (0 <= f[i].fd && ! f[i].remote)
      واپس true;
  واپس false;
}

/* Return true if any of the N_FILES files in F is a symlink.
   Note we don't worry about the edge case where "-" exists,
   since that will have the same consequences for inotify,
   which is the only context this function is currently used.  */

ساکن bool
any_symlinks (ثابت ساخت File_spec *f, صحیح n_files)
{
  برائے (صحیح i = 0; i < n_files; i++)
    اگر (issymlink (f[i].name) == 1)
      واپس true;
  واپس false;
}

/* Return true if any of the N_FILES files in F is not
   a regular file or fifo.  This is used to avoid adding inotify
   watches on a device file for example, which inotify
   will accept, but not give any events for.  */

ساکن bool
any_non_regular_fifo (ثابت ساخت File_spec *f, صحیح n_files)
{
  برائے (صحیح i = 0; i < n_files; i++)
    اگر (0 <= f[i].fd && ! S_ISREG (f[i].منوال_2) && ! S_ISFIFO (f[i].منوال_2))
      واپس true;
  واپس false;
}

/* Return true if any of the N_FILES files in F represents
   stdin and is not ignored.  */

ساکن bool
tailable_stdin (ثابت ساخت File_spec *f, صحیح n_files)
{
  برائے (صحیح i = 0; i < n_files; i++)
    اگر (!f[i].ignore && streq (f[i].name, "-"))
      واپس true;
  واپس false;
}

ساکن size_t
wd_hasher (ثابت خالی *اندراج_2, size_t tabsize)
{
  ثابت ساخت File_spec *spec = اندراج_2;
  واپس spec->wd % tabsize;
}

ساکن bool
wd_comparator (ثابت خالی *e1, ثابت خالی *e2)
{
  ثابت ساخت File_spec *spec1 = e1;
  ثابت ساخت File_spec *spec2 = e2;
  واپس spec1->wd == spec2->wd;
}

/* Hash Follow_name watches by parent directory wd + basename so directory
   inotify events can be matched in expected O(1) instead of scanning all
   files.  */
ساکن size_t
والد_name_hasher (ثابت خالی *اندراج_2, size_t tabsize)
{
  ثابت ساخت File_spec *spec = اندراج_2;
  حرف ثابت *name = spec->name + spec->basename_آغاز;
  /* hash_string already reduces modulo TABSIZE; mix in parent_wd so the
     same basename under different directories does not collide by design.  */
  واپس (hash_string (name, tabsize) + (size_t) spec->والد_wd) % tabsize;
}

ساکن bool
والد_name_comparator (ثابت خالی *e1, ثابت خالی *e2)
{
  ثابت ساخت File_spec *a = e1;
  ثابت ساخت File_spec *b = e2;
  واپس (a->والد_wd == b->والد_wd
          && streq (a->name + a->basename_آغاز,
                    b->name + b->basename_آغاز));
}

/* Output (new) data for FSPEC->fd.
   PREV_FSPEC records the last File_spec for which we output.  */
ساکن خالی
check_fspec (ساخت File_spec *fspec, ساخت File_spec **پچھلا_fspec)
{
  اگر (fspec->fd < 0)
    واپس;

  ساخت stat stats;
  اگر (fstat (fspec->fd, &stats) < 0)
    {
      fspec->errnum = errno;
      close_fd (fspec->fd, fspec);
      fspec->fd = -1;
      واپس;
    }

  حرف ثابت *prettyname = fspec->prettyname;

  /* XXX: This is only a heuristic, as the file may have also
     been truncated and written to if st_size >= size
     (in which case we ignore new data <= size).
     Though in the inotify case it's more likely we'll get
     separate events for truncate() and write().  */
  اگر (S_ISREG (fspec->منوال_2) && stats.st_سائز < fspec->read_pos)
    {
      error (0, 0, _("%s: file truncated"), quotef (prettyname));
      fspec->read_pos = xlseek (fspec->fd, 0, SEEK_SET, prettyname);
    }
  ورنہ اگر (S_ISREG (fspec->منوال_2) && stats.st_سائز == fspec->read_pos
           && timespec_cmp (fspec->mtime, get_stat_mtime (&stats)) == 0)
    واپس;

  bool want_سرخی = print_سرخیاں && (fspec != *پچھلا_fspec);

  گنتی_t nr = dump_remainder (want_سرخی, prettyname, fspec->fd, COPY_مقصد_EOF);
  اگر (0 < nr)
    {
      اگر (S_ISREG (fspec->منوال_2))
        fspec->read_pos += nr;
      *پچھلا_fspec = fspec;
      اگر (fflush (stdout) < 0)
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
ساکن خالی
دم_forever_inotify (صحیح wd, ساخت File_spec *f, صحیح n_files,
                      دوہرا sleep_interval, Hash_جدول **wd_مقصد_namep,
                      Hash_جدول **والد_by_namep)
{
# اگر دم_TEST_SLEEP
  /* Delay between open() and inotify_add_watch()
     to help trigger different cases.  */
  xnanosleep (1000000);
# endif
  صحیح max_realloc = 3;

  /* Map an inotify watch descriptor to the name of the file it's watching.  */
  Hash_جدول *wd_مقصد_name;

  /* Map (parent_wd, basename) to File_spec for Follow_name
     directory events.  */
  Hash_جدول *والد_by_name = تہی;

  bool ملا_watchable_file = false;
  bool tailed_but_unwatchable = false;
  bool ملا_unwatchable_dir = false;
  bool no_inotify_resources = false;
  ساخت File_spec *پچھلا_fspec;
  idx_t evlen = 0;
  حرف *evbuf;
  idx_t evbuf_off = 0;

  *والد_by_namep = تہی;

  wd_مقصد_name = hash_initialize (n_files, تہی, wd_hasher, wd_comparator,
                                تہی);
  اگر (! wd_مقصد_name)
    xalloc_die ();
  *wd_مقصد_namep = wd_مقصد_name;

  اگر (follow_منوال == Follow_name)
    {
      والد_by_name = hash_initialize (n_files, تہی, والد_name_hasher,
                                        والد_name_comparator, تہی);
      اگر (! والد_by_name)
        xalloc_die ();
      *والد_by_namep = والد_by_name;
    }

  /* The events mask used with inotify on files (not directories).  */
  uint32_t inotify_wd_نقاب = اندر_MODIFY;
  /* TODO: Perhaps monitor these events in Follow_descriptor mode also,
     to tag reported file names with "deleted", "moved" etc.  */
  اگر (follow_منوال == Follow_name)
    inotify_wd_نقاب |= (اندر_ATTRIB | اندر_DELETE_SELF | اندر_MOVE_SELF);

  /* Add an inotify watch for each watched file.  If -F is specified then watch
     its parent directory too, in this way when they re-appear we can add them
     again to the watch list.  */
  برائے (صحیح i = 0; i < n_files; i++)
    {
      اگر (!f[i].ignore)
        {
          idx_t fnlen = لڑی_لمبائی (f[i].name);
          اگر (evlen < fnlen)
            evlen = fnlen;

          f[i].wd = -1;

          اگر (follow_منوال == Follow_name)
            {
              idx_t dirlen = dir_len (f[i].name);
              حرف پچھلا = f[i].name[dirlen];
              f[i].basename_آغاز = آخری_component (f[i].name) - f[i].name;

              f[i].name[dirlen] = '\0';

               /* It's fine to add the same directory more than once.
                  In that case the same watch descriptor is returned.  */
              f[i].والد_wd = inotify_جوڑو_watch (wd, dirlen ? f[i].name : ".",
                                                  (اندر_CREATE | اندر_DELETE
                                                   | اندر_MOVED_مقصد | اندر_ATTRIB
                                                   | اندر_DELETE_SELF));

              f[i].name[dirlen] = پچھلا;

              اگر (f[i].والد_wd < 0)
                {
                  اگر (errno != ENOSPC) /* suppress confusing error.  */
                    error (0, errno, _("cannot watch parent directory of %s"),
                           quoteaf (f[i].name));
                  ورنہ
                    error (0, 0, _("inotify resources exhausted"));
                  ملا_unwatchable_dir = true;
                  /* We revert to polling below.  Note invalid uses
                     of the inotify API will still be diagnosed.  */
                  توڑو;
                }

              اگر (hash_داخل_کرو (والد_by_name, &(f[i])) == تہی)
                xalloc_die ();
            }

          f[i].wd = inotify_جوڑو_watch (wd, f[i].name, inotify_wd_نقاب);

          اگر (f[i].wd < 0)
            {
              اگر (0 <= f[i].fd)  /* already tailed.  */
                tailed_but_unwatchable = true;
              اگر (errno == ENOSPC || errno == ENOMEM)
                {
                  no_inotify_resources = true;
                  error (0, 0, _("inotify resources exhausted"));
                  توڑو;
                }
              ورنہ اگر (errno != f[i].errnum)
                error (0, errno, _("cannot watch %s"), quoteaf (f[i].name));
              جاری;
            }

          اگر (hash_داخل_کرو (wd_مقصد_name, &(f[i])) == تہی)
            xalloc_die ();

          ملا_watchable_file = true;
        }
    }

  /* Linux kernel 2.6.24 at least has a bug where eventually, ENOSPC is always
     returned by inotify_add_watch.  In any case we should revert to polling
     when there are no inotify resources.  Also a specified directory may not
     be currently present or accessible, so revert to polling.  Also an already
     tailed but unwatchable due rename/unlink race, should also revert.  */
  اگر (no_inotify_resources || ملا_unwatchable_dir
      || (follow_منوال == Follow_descriptor && tailed_but_unwatchable))
    واپس;
  اگر (follow_منوال == Follow_descriptor && !ملا_watchable_file)
    باہر_نکلو (EXIT_FAILURE);

  پچھلا_fspec = &(f[n_files - 1]);

  /* Check files again.  New files or data can be available since last time we
     checked and before they are watched by inotify.  */
  برائے (صحیح i = 0; i < n_files; i++)
    {
      اگر (! f[i].ignore)
        {
          /* check for new files.  */
          اگر (follow_منوال == Follow_name)
            recheck (&(f[i]), false);
          ورنہ اگر (0 <= f[i].fd)
            {
              /* If the file was replaced in the small window since we tailed,
                 then assume the watch is on the wrong item (different to
                 that we've already produced output for), and so revert to
                 polling the original descriptor.  */
              ساخت stat stats;

              اگر (! (stat (f[i].name, &stats) < 0
                     || SAME_INODE (f[i], stats)))
                {
                  error (0, errno, _("%s was replaced"),
                         quoteaf (f[i].prettyname));
                  واپس;
                }
            }

          /* check for new data.  */
          check_fspec (&f[i], &پچھلا_fspec);
        }
    }

  اگر (debug)
    error (0, 0, "%s", _("using notification mode"));

  evlen += حجم (ساخت inotify_event) + 1;
  evbuf = ximalloc (evlen);

  /* Wait for inotify events and handle them.  Events on directories
     ensure that watched files can be re-added when following by name.
     This loop blocks on the 'read' call until a new event is notified.
     But when --pid=P is specified, tail usually waits via poll.  */
  ptrdiff_t len = 0;
  جب (true)
    {
      ساخت File_spec *fspec;
      ساخت inotify_event *ev;
      خالی *void_ev;

      /* When following by name without --retry, and the last file has
         been unlinked or renamed-away, diagnose it and return.  */
      اگر (follow_منوال == Follow_name
          && ! reopen_inaccessible_files
          && hash_get_n_entries (wd_مقصد_name) == 0)
        error (EXIT_FAILURE, 0, _("no files remaining"));

      اگر (len <= evbuf_off)
        {
          /* Poll for inotify events.  When watching a PID, ensure
             that a read from WD will not block indefinitely.
             If MONITOR_OUTPUT, also poll for a broken output pipe.  */

          صحیح file_change;
          ساخت pollfd pfd[2];
          کرو
            {
              /* How many ms to wait for changes.  -1 means wait forever.  */
              صحیح delay = -1;

              اگر (pids)
                {
                  اگر (!nbpids)
                    باہر_نکلو (EXIT_SUCCESS);

                  اگر (!some_writers_exist () || sleep_interval <= 0)
                    delay = 0;
                  ورنہ اگر (sleep_interval < زیادہ_سے_زیادہ_عدد / 1000 - 1)
                    {
                      /* delay = ceil (sleep_interval * 1000), sans libm.  */
                      دوہرا ddelay = sleep_interval * 1000;
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
          جب (file_change == 0 || (file_change < 0 && errno == EINTR));

          اگر (file_change < 0)
            error (EXIT_FAILURE, errno,
                   _("error waiting for inotify and output events"));
          اگر (pfd[1].revents)
            die_pipe ();

          len = اقرأ (wd, evbuf, evlen);
          evbuf_off = 0;

          /* For kernels prior to 2.6.21, read returns 0 when the buffer
             is too small.  */
          اگر ((len == 0 || (len < 0 && errno == EINVAL))
              && max_realloc--)
            {
              len = 0;
              evlen *= 2;
              evbuf = xirealloc (evbuf, evlen);
              جاری;
            }

          اگر (len <= 0)
            error (EXIT_FAILURE, errno, _("error reading inotify event"));
        }

      void_ev = evbuf + evbuf_off;
      ev = void_ev;
      evbuf_off += حجم (*ev) + ev->len;

      /* If a directory is deleted, IN_DELETE_SELF is emitted
         with ev->name of length 0.
         We need to catch it, otherwise it would wait forever,
         as wd for directory becomes inactive. Revert to polling now.   */
      اگر ((ev->نقاب & اندر_DELETE_SELF) && ! ev->len)
        {
          برائے (صحیح i = 0; i < n_files; i++)
            {
              اگر (ev->wd == f[i].والد_wd)
                {
                  error (0, 0,
                      _("directory containing watched file was removed"));
                  واپس;
                }
            }
        }

      اگر (ev->len) /* event on ev->name in watched directory.  */
        {
          ساخت File_spec کلید;
          اگر (! والد_by_name)
            جاری;

          /* Probe key: basename is the whole string at KEY.NAME.  */
          کلید.والد_wd = ev->wd;
          کلید.name = ev->name;
          کلید.basename_آغاز = 0;
          fspec = hash_lookup (والد_by_name, &کلید);

          /* It is not a watched file.  */
          اگر (! fspec)
            جاری;

          صحیح new_wd = -1;
          bool deleting = !! (ev->نقاب & اندر_DELETE);

          اگر (! deleting)
            {
              /* Adding the same inode again will look up any existing wd.  */
              new_wd = inotify_جوڑو_watch (wd, fspec->name, inotify_wd_نقاب);
            }

          اگر (! deleting && new_wd < 0)
            {
              اگر (errno == ENOSPC || errno == ENOMEM)
                {
                  error (0, 0, _("inotify resources exhausted"));
                  واپس; /* revert to polling.  */
                }
              ورنہ
                {
                  /* Can get ENOENT for a dangling symlink for example.  */
                  error (0, errno, _("cannot watch %s"), quoteaf (fspec->name));
                }
              /* We'll continue below after removing the existing watch.  */
            }

          /* This will be false if only attributes of file change.  */
          bool new_watch;
          new_watch = (! deleting) && (fspec->wd < 0 || new_wd != fspec->wd);

          اگر (new_watch)
            {
              اگر (0 <= fspec->wd)
                {
                  inotify_rm_watch (wd, fspec->wd);
                  hash_remove (wd_مقصد_name, fspec);
                }

              fspec->wd = new_wd;

              اگر (new_wd < 0)
                جاری;

              /* If the file was moved then inotify will use the source file wd
                for the destination file.  Make sure the key is not present in
                the table.  */
              ساخت File_spec *پچھلا = hash_remove (wd_مقصد_name, fspec);
              اگر (پچھلا && پچھلا != fspec)
                {
                  اگر (follow_منوال == Follow_name)
                    recheck (پچھلا, false);
                  پچھلا->wd = -1;
                  close_fd (پچھلا->fd, پچھلا);
                }

              اگر (hash_داخل_کرو (wd_مقصد_name, fspec) == تہی)
                xalloc_die ();
            }

          اگر (follow_منوال == Follow_name)
            recheck (fspec, false);
        }
      ورنہ
        {
          ساخت File_spec کلید;
          کلید.wd = ev->wd;
          fspec = hash_lookup (wd_مقصد_name, &کلید);
        }

      اگر (! fspec)
        جاری;

      اگر (ev->نقاب & (اندر_ATTRIB | اندر_DELETE | اندر_DELETE_SELF | اندر_MOVE_SELF))
        {
          /* Note for IN_MOVE_SELF (the file we're watching has
             been clobbered via a rename) without --retry we leave the watch
             in place since it may still be part of the set
             of watched names.  */
          اگر (ev->نقاب & اندر_DELETE_SELF
              || (!reopen_inaccessible_files && (ev->نقاب & اندر_MOVE_SELF)))
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

          جاری;
        }
      check_fspec (fspec, &پچھلا_fspec);
    }
}
#اگر_ختم

/* Output the last bytes of the file PRETTYNAME open for reading
   in FD and with status ST.  Output the last N_BYTES bytes.
   Return (-1 - errno) on failure, otherwise the resulting file offset
   if known, otherwise -1.  */

ساکن off_t
دم_بائٹیں (حرف ثابت *prettyname, صحیح fd, ساخت stat ثابت *st,
            گنتی_t n_بائٹیں)
{
  off_t current_pos
    = (presume_داخلہ_pipe
       ? -1
       : lseek (fd, from_آغاز ? MIN (n_بائٹیں, OFF_T_MAX) : 0, SEEK_CUR));

  اگر (from_آغاز)
    {
      اگر (current_pos < 0)
        {
          صحیح t = آغاز_بائٹیں (prettyname, fd, n_بائٹیں);
          اگر (t < 0)
            واپس t;
        }
      n_بائٹیں = COPY_مقصد_EOF;
    }
  ورنہ
    {
      off_t initial_pos = current_pos;
      off_t انتہا_pos = -1;

      اگر (0 <= current_pos)
        {
          اگر (usable_st_سائز (st))
            {
              /* Use st_size only if it's so large that this is
                 probably not a /proc or similar file, where st_size
                 is notional.  */
              off_t smallish_سائز = STP_BLKSIZE (st);
              اگر (smallish_سائز < st->st_سائز)
                انتہا_pos = st->st_سائز;
            }
          ورنہ
            {
              off_t minus_n;
              اگر (ckd_sub (&minus_n, 0, n_بائٹیں))
                انتہا_pos = 0;
              ورنہ
                {
                  off_t e_n_pos = lseek (fd, minus_n, SEEK_END);
                  اگر (0 <= e_n_pos)
                    {
                      current_pos = e_n_pos;
                      انتہا_pos = e_n_pos + n_بائٹیں;
                    }
                  ورنہ
                    {
                      /* POSIX says errno is unreliable here.
                         Perhaps the file is smaller than N_BYTES.
                         Try again at file end.  */
                      off_t e_pos = lseek (fd, 0, SEEK_END);
                      اگر (0 <= e_pos)
                        current_pos = انتہا_pos = e_pos;
                    }
               }
            }
        }

      /* If the end is known and more than N_BYTES after the initial
         position, go to N_BYTES before the end; otherwise go back to
         the initial position.  But do not lseek if lseek already failed.  */
      off_t pos = (initial_pos < انتہا_pos && n_بائٹیں < انتہا_pos - initial_pos
                   ? انتہا_pos - n_بائٹیں
                   : initial_pos);
      اگر (pos != current_pos)
        current_pos = xlseek (fd, pos, SEEK_SET, prettyname);

      اگر (انتہا_pos < 0)
        واپس pipe_بائٹیں (prettyname, fd, n_بائٹیں, current_pos);
    }

  گنتی_t nr = dump_remainder (false, prettyname, fd, n_بائٹیں);
  واپس current_pos < 0 ? -1 : current_pos + nr;
}

/* Output the last lines of the file PRETTYNAME open for reading
   in FD and with status ST.  Output the last N_LINES lines.
   Return (-1 - errno) on failure, otherwise the resulting file offset
   if known, otherwise -1.  */

ساکن off_t
دم_سطریں (حرف ثابت *prettyname, صحیح fd, ساخت stat ثابت *st,
            گنتی_t n_سطریں)
{
  اگر (from_آغاز)
    {
      /* If skipping all input use lseek if possible, for speed.  */
      اگر (OFF_T_MAX <= n_سطریں)
        {
          off_t e = lseek (fd, 0, SEEK_END);
          اگر (0 <= e)
            واپس e;
        }

      صحیح t = آغاز_سطریں (prettyname, fd, n_سطریں);
      اگر (t < 0)
        واپس t;
      dump_remainder (false, prettyname, fd, COPY_مقصد_EOF);
      واپس -1;
    }
  ورنہ
    {
      /* Use file_lines only if FD is a regular file and SEEK_END
         reports more data.  */
      off_t آغاز_pos = (!presume_داخلہ_pipe && S_ISREG (st->st_منوال)
                         ? lseek (fd, 0, SEEK_CUR)
                         : -1);
      off_t انتہا_pos = آغاز_pos < 0 ? -1 : lseek (fd, 0, SEEK_END);
      واپس (انتہا_pos < 0
              ? pipe_سطریں (prettyname, fd, n_سطریں)
              : آغاز_pos < انتہا_pos
              ? file_سطریں (prettyname, fd, st, n_سطریں, آغاز_pos, انتہا_pos)
              : آغاز_pos != انتہا_pos
              ? (/* Do not read from before the start offset,
                    even if the input file shrank.  */
                 xlseek (fd, آغاز_pos, SEEK_SET, prettyname))
              : آغاز_pos);
    }
}

/* Display the last part of file FILENAME,
   open for reading via FD and with status *ST.
   Return (-1 - errno) on failure, otherwise the resulting file offset
   if known, otherwise -1.  */

ساکن off_t
دم (حرف ثابت *filename, صحیح fd, ساخت stat ثابت *st, گنتی_t n_units)
{
  واپس (گنتی_سطریں ? دم_سطریں : دم_بائٹیں) (filename, fd, st, n_units);
}

/* Display the last N_UNITS units of the file described by F.
   Return true if successful.  */

ساکن bool
دم_file (ساخت File_spec *f, گنتی_t n_files, گنتی_t n_units)
{
  صحیح fd;
  bool ok;

  /* Avoid blocking if we may need to process asynchronously.  */
  bool nonblocking = forever && (nbpids || n_files > 1);

  bool is_stdin = (streq (f->name, "-"));

  اگر (is_stdin)
    {
      have_read_stdin = true;
      fd = STDIN_FILENO;
      xset_ثنائی_منوال (STDIN_FILENO, O_ثنائی);
    }
  ورنہ
    fd = open (f->name, O_RDONLY | O_ثنائی | (nonblocking ? O_NONBLOCK : 0));

  f->tailable = false;

  اگر (fd < 0)
    {
      اگر (forever)
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
  ورنہ
    {
      اگر (print_سرخیاں)
        write_سرخی (f->prettyname);

      off_t read_pos;
      ساخت stat stats;
      ok = 0 <= fstat (fd, &stats);
      اگر (!ok)
        {
          f->errnum = errno;
          error (0, f->errnum, _("cannot fstat %s"), quoteaf (f->prettyname));
        }
      ورنہ
        {
          read_pos = دم (f->prettyname, fd, &stats, n_units);
          ok = -1 <= read_pos;
          f->errnum = ok ? 0 : -1 - read_pos;

          اگر (IS_TAILABLE_FILE_TYPE (stats.st_منوال))
            f->tailable = true;
          ورنہ اگر (forever)
            {
              ok = false;
              f->errnum = -1;
              error (0, 0, _("%s: cannot follow end of this type of file%s"),
                     quotef (f->prettyname),
                     (reopen_inaccessible_files ? ""
                      : _("; giving up on this name")));
            }
        }

      اگر (forever)
        {
          اگر (!ok)
            {
              f->ignore = ! reopen_inaccessible_files;
              close_fd (fd, f);
              f->fd = -1;
            }
          ورنہ
            {
              ریکارڈ_open_fd (f, fd, read_pos, &stats, (is_stdin ? -1 : 1));
              f->remote = fremote (fd, f);
            }
        }
      ورنہ
        {
          اگر (!is_stdin && close (fd) < 0)
            {
              error (0, errno, _("error reading %s"),
                     quoteaf (f->prettyname));
              ok = false;
            }
        }
    }

  واپس ok;
}

/* If obsolete usage is allowed, and the command line arguments are of
   the obsolete form and the option string is well-formed, set
   *N_UNITS, the globals COUNT_LINES, FOREVER, and FROM_START, and
   return true.  If the command line arguments are obviously incorrect
   (e.g., because obsolete usage is not allowed and the arguments are
   incorrect for non-obsolete usage), report an error and exit.
   Otherwise, return false and don't modify any parameter or global
   variable.  */

ساکن bool
parse_obsolete_option (صحیح argc, حرف * ثابت *argv, گنتی_t *n_units)
{
  bool t_from_آغاز;
  bool t_گنتی_سطریں = true;
  bool t_forever = false;

  /* With the obsolete form, there is one option string and at most
     one file argument.  Watch out for "-" and "--", though.  */
  اگر (! (argc == 2
         || (argc == 3 && ! (argv[2][0] == '-' && argv[2][1]))
         || (3 <= argc && argc <= 4 && streq (argv[2], "--"))))
    واپس false;

  صحیح posix_ver = posix2_نسخہ ();
  bool obsolete_usage = posix_ver < 200112;
  bool traditional_usage = obsolete_usage || 200809 <= posix_ver;
  حرف ثابت *p = argv[1];

  انتخاب (*p++)
    {
    طےشدہ:
      واپس false;

    حالت '+':
      /* Leading "+" is a file name in the standard form.  */
      اگر (!traditional_usage)
        واپس false;

      t_from_آغاز = true;
      توڑو;

    حالت '-':
      /* In the non-obsolete form, "-" is standard input and "-c"
         requires an option-argument.  The obsolete multidigit options
         are supported as a GNU extension even when conforming to
         POSIX 1003.1-2001 or later, so don't complain about them.  */
      اگر (!obsolete_usage && !p[p[0] == 'c'])
        واپس false;

      t_from_آغاز = false;
      توڑو;
    }

  گنتی_t n;
  اگر (!c_isdigit (*p))
    n = DEFAULT_N_سطریں;
  ورنہ
    برائے (n = 0; c_isdigit (*p); p++)
      n = ckd_mul (&n, n, 10) || ckd_جوڑو (&n, n, *p - '0') ? گنتی_MAX : n;

  انتخاب (*p)
    {
    حالت 'b': n = ckd_mul (&n, n, 512) ? گنتی_MAX : n; FALLTHROUGH;
    حالت 'c': t_گنتی_سطریں = false; FALLTHROUGH;
    حالت 'l': p++; توڑو;
    }

  اگر (*p == 'f')
    {
      t_forever = true;
      ++p;
    }

  اگر (*p)
    واپس false;

  *n_units = n;

  /* Set globals.  */
  from_آغاز = t_from_آغاز;
  گنتی_سطریں = t_گنتی_سطریں;
  forever = t_forever;

  واپس true;
}

ساکن خالی
parse_options (صحیح argc, حرف **argv,
               گنتی_t *n_units, شمار سرخی_منوال *سرخی_منوال,
               دوہرا *sleep_interval)
{
  صحیح c;

  جب ((c = getopt_long (argc, argv, "c:n:fFqs:vz0123456789",
                           long_options, تہی))
         != -1)
    {
      انتخاب (c)
        {
        حالت 'F':
          forever = true;
          follow_منوال = Follow_name;
          reopen_inaccessible_files = true;
          توڑو;

        حالت 'c':
        حالت 'n':
          گنتی_سطریں = (c == 'n');
          اگر (*optarg == '+')
            from_آغاز = true;
          ورنہ اگر (*optarg == '-')
            ++optarg;

          *n_units = xnumtoimax (optarg, 10, 0, گنتی_MAX, "bkKmMGTPEZYRQ0",
                                 (گنتی_سطریں
                                  ? _("invalid number of lines")
                                  : _("invalid number of bytes"))
                                 , 0, XTOINT_MAX_QUIET);
          توڑو;

        حالت 'f':
        حالت LONG_FOLLOW_OPTION:
          forever = true;
          اگر (optarg == تہی)
            follow_منوال = DEFAULT_FOLLOW_منوال;
          ورنہ
            follow_منوال = XARGMATCH ("--follow", optarg,
                                     follow_منوال_string, follow_منوال_map);
          توڑو;

        حالت RETRY_OPTION:
          reopen_inaccessible_files = true;
          توڑو;

        حالت MAX_UNCHANGED_STATS_OPTION:
          /* --max-unchanged-stats=N */
          max_n_unchanged_stats_between_opens =
            xnumtoimax (optarg, 10, 0, گنتی_MAX, "",
                        _("invalid maximum number of unchanged stats"
                          " between opens"),
                        0, XTOINT_MAX_QUIET);
          توڑو;

        حالت DEBUG_پروگرام_OPTION:
          debug = true;
          توڑو;

        حالت DISABLE_INOTIFY_OPTION:
          disable_inotify = true;
          توڑو;

        حالت PID_OPTION:
          اگر (nbpids == pids_alloc)
            pids = xpalloc (pids, &pids_alloc, 1,
                            MIN (زیادہ_سے_زیادہ_عدد, IDX_MAX), حجم *pids);
          pids[nbpids++] = xdectoumax (optarg, 0, PID_T_MAX, "",
                                       _("invalid PID"), 0);
          توڑو;

        حالت PRESUME_داخلہ_PIPE_OPTION:
          presume_داخلہ_pipe = true;
          توڑو;

        حالت 'q':
          *سرخی_منوال = never;
          توڑو;

        حالت 's':
          {
            حرف *ep;
            errno = 0;
            دوہرا s = cl_strtod (optarg, &ep);
            اگر (optarg == ep || *ep || ! (0 <= s))
              error (EXIT_FAILURE, 0,
                     _("invalid number of seconds: %s"), quote (optarg));
            *sleep_interval = dtimespec_bound (s, errno);
          }
          توڑو;

        حالت 'v':
          *سرخی_منوال = always;
          توڑو;

        حالت 'z':
          سطر_انتہا = '\0';
          توڑو;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

        حالت '0': حالت '1': حالت '2': حالت '3': حالت '4':
        حالت '5': حالت '6': حالت '7': حالت '8': حالت '9':
          error (EXIT_FAILURE, 0, _("option used in invalid context -- %c"), c);

        طےشدہ:
          usage (EXIT_FAILURE);
        }
    }

  اگر (reopen_inaccessible_files)
    {
      اگر (!forever)
        {
          reopen_inaccessible_files = false;
          error (0, 0, _("warning: --retry ignored; --retry is useful"
                         " only when following"));
        }
      ورنہ اگر (follow_منوال == Follow_descriptor)
        error (0, 0, _("warning: --retry only effective for the initial open"));
    }

  اگر (pids && (!forever || (kill (pids[0], 0) < 0 && errno == ENOSYS)))
    {
      error (0, 0,
             _(forever
               ? "warning: --pid=PID is not supported on this system"
               : ("warning: PID ignored;"
                  " --pid=PID is useful only when following")));
      آزاد (pids);
      pids = تہی;
    }
}

/* Mark as '.ignore'd each member of F that corresponds to a
   pipe or fifo, and return true if any are not marked.  */
ساکن bool
ignore_fifo_and_pipe (ساخت File_spec *f, صحیح n_files)
{
  /* When there is no FILE operand and stdin is a pipe or FIFO
     POSIX requires that tail ignore the -f option.
     Since we allow multiple FILE operands, we extend that to say: with -f,
     ignore any "-" operand that corresponds to a pipe or FIFO.  */
  bool some_viable = false;

  برائے (صحیح i = 0; i < n_files; i++)
    {
      bool is_a_fifo_or_pipe =
        (streq (f[i].name, "-")
         && !f[i].ignore
         && 0 <= f[i].fd
         && (S_ISFIFO (f[i].منوال_2)
             || (HAVE_FIFO_PIPES != 1 && isapipe (f[i].fd))));
      اگر (is_a_fifo_or_pipe)
        {
          f[i].fd = -1;
          f[i].errnum = -1;
          f[i].ignore = true;
        }
      ورنہ
        some_viable = true;
    }

  واپس some_viable;
}

صحیح
مین (صحیح argc, حرف **argv)
{
  شمار سرخی_منوال سرخی_منوال = مضرب_files;
  bool ok = true;
  /* If from_start, the number of items to skip before printing; otherwise,
     the number of items at the end of the file to print.  */
  گنتی_t n_units = DEFAULT_N_سطریں;
  صحیح n_files;
  حرف **file;
  ساخت File_spec *F;
  bool obsolete_option;

  /* The number of seconds to sleep between iterations.
     During one iteration, every file name or descriptor is checked to
     see if it has changed.  */
  دوہرا sleep_interval = 1.0;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  {
    صحیح p = getpagesize ();
    اگر (IDX_MAX < p)
      xalloc_die ();
    صفحہ_سائز = p;
  }

  obsolete_option = parse_obsolete_option (argc, argv, &n_units);
  argc -= obsolete_option;
  argv += obsolete_option;
  parse_options (argc, argv, &n_units, &سرخی_منوال, &sleep_interval);

  /* To start printing with item N_UNITS from the start of the file, skip
     N_UNITS - 1 items.  'tail -n +0' is actually meaningless, but for Unix
     compatibility it's treated the same as 'tail -n +1'.  */
  n_units -= from_آغاز && 0 < n_units && n_units < گنتی_MAX;

  اگر (optind < argc)
    {
      n_files = argc - optind;
      file = argv + optind;
    }
  ورنہ
    {
      ساکن حرف *dummy_stdin = (حرف *) "-";
      n_files = 1;
      file = &dummy_stdin;
    }

  {
    bool ملا_hyphen = false;

    برائے (صحیح i = 0; i < n_files; i++)
      اگر (streq (file[i], "-"))
        ملا_hyphen = true;

    /* When following by name, there must be a name.  */
    اگر (ملا_hyphen && follow_منوال == Follow_name)
      error (EXIT_FAILURE, 0, _("cannot follow %s by name"), quoteaf ("-"));

    /* When following forever, and not using simple blocking, warn if
       any file is '-' as the stats() used to check for input are ineffective.
       This is only a warning, since tail's output (before a failing seek,
       and that from any non-stdin files) might still be useful.  */
    اگر (forever && ملا_hyphen)
      {
        ساخت stat اندر_stat;
        bool blocking_stdin;
        blocking_stdin = (!nbpids && follow_منوال == Follow_descriptor
                          && n_files == 1 && ! fstat (STDIN_FILENO, &اندر_stat)
                          && ! S_ISREG (اندر_stat.st_منوال));

        اگر (! blocking_stdin && isatty (STDIN_FILENO))
          error (0, 0, _("warning: following standard input"
                         " indefinitely is ineffective"));
      }
  }

  /* Don't read anything if we'll never output anything.  */
  اگر (! forever && n_units == (from_آغاز ? گنتی_MAX : 0))
    واپس EXIT_SUCCESS;

  اگر (IDX_MAX < n_files)
    xalloc_die ();
  F = xinmalloc (n_files, حجم *F);
  برائے (صحیح i = 0; i < n_files; i++)
    {
      F[i].name = file[i];
      F[i].prettyname = streq (file[i], "-") ? _("standard input") : file[i];
    }

  اگر (سرخی_منوال == always
      || (سرخی_منوال == مضرب_files && n_files > 1))
    print_سرخیاں = true;

  xset_ثنائی_منوال (STDOUT_FILENO, O_ثنائی);

  برائے (صحیح i = 0; i < n_files; i++)
    ok &= دم_file (&F[i], n_files, n_units);

  اگر (forever && ignore_fifo_and_pipe (F, n_files))
    {
      /* If stdout is a fifo or pipe, then monitor it
         so that we exit if the reader goes away.  */
      ساخت stat برآمد_stat;
      اگر (fstat (STDOUT_FILENO, &برآمد_stat) < 0)
        error (EXIT_FAILURE, errno, _("standard output"));
      monitor_output = (S_ISFIFO (برآمد_stat.st_منوال)
                        || (HAVE_FIFO_PIPES != 1 && isapipe (STDOUT_FILENO)));

#اگر HAVE_INOTIFY
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
      اگر (!disable_inotify && (tailable_stdin (F, n_files)
                               || any_remote_file (F, n_files)
                               || ! any_non_remote_file (F, n_files)
                               || any_symlinks (F, n_files)
                               || any_non_regular_fifo (F, n_files)
                               || (!ok && follow_منوال == Follow_descriptor)))
        disable_inotify = true;

      اگر (!disable_inotify)
        {
          صحیح wd = inotify_init ();
          اگر (0 <= wd)
            {
              /* Flush any output from tail_file, now, since
                 tail_forever_inotify flushes only after writing,
                 not before reading.  */
              اگر (fflush (stdout) < 0)
                write_error ();

              Hash_جدول *ht;
              Hash_جدول *والد_ht = تہی;
              دم_forever_inotify (wd, F, n_files, sleep_interval,
                                    &ht, &والد_ht);
              hash_free (ht);
              اگر (والد_ht)
                hash_free (والد_ht);
              close (wd);
              errno = 0;
            }
          error (0, errno, _("inotify cannot be used, reverting to polling"));
        }
#اگر_ختم
      disable_inotify = true;
      دم_forever (F, n_files, sleep_interval);
    }

  اگر (have_read_stdin && close (STDIN_FILENO) < 0)
    error (EXIT_FAILURE, errno, "-");
  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
