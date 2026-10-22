/* split.c -- split a file into pieces.
   Copyright (C) 1988-2026 Free Software Foundation, Inc.

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

/* By tege@sics.se, with rms.

   TODO:
   * support -p REGEX as in BSD's split.
   * support --suppress-matched as in csplit.  */
#شامل <config.h>

#شامل <stdio.h>
#شامل <getopt.h>
#شامل <signal.h>
#شامل <sys/types.h>
#شامل <sys/wait.h>
#شامل <spawn.h>

#شامل "system.h"
#شامل "alignalloc.h"
#شامل "assure.h"
#شامل "fadvise.h"
#شامل "fd-reopen.h"
#شامل "fcntl--.h"
#شامل "full-write.h"
#شامل "ioblksize.h"
#شامل "quote.h"
#شامل "sig2str.h"
#شامل "sys-limits.h"
#شامل "temp-stream.h"
#شامل "unistd--.h"
#شامل "xbinary-io.h"
#شامل "xdectoint.h"
#شامل "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "split"

#define AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("Richard M. Stallman")

/* Shell command to filter through, instead of creating files.  */
ایستا نویسه ثابت *filter_command;

/* Process ID of the filter.  */
ایستا pid_t filter_pid;

ایستا ساختار
{
  /* The number of open pipes.  */
  صحیح n_pipes;

  /* The number of allocated pipes.  */
  idx_t n_pipes_alloc;

  /* Array of open pipes.  */
  صحیح *pipes COUNTED_BY (n_pipes);
} open_pipes;

/* Whether SIGPIPE has the default action, when --filter is used.  */
ایستا bool default_SIGPIPE;

/* Base name of output files.  */
ایستا نویسه ثابت *outbase = "x";

/* Name of output files.  */
ایستا نویسه *outfile;

/* Pointer to the end of the prefix in OUTFILE.
   Suffixes are inserted here.  */
ایستا نویسه *outfile_mid;

/* Generate new suffix when suffixes are exhausted.  */
ایستا bool suffix_auto = true;

/* Length of OUTFILE's suffix.  */
ایستا idx_t suffix_length;

/* Alphabet of characters to use in suffix.  */
ایستا نویسه ثابت *suffix_alphabet = "abcdefghijklmnopqrstuvwxyz";

/* Numerical suffix start value.  */
ایستا نویسه ثابت *numeric_suffix_start;

/* Additional suffix to append to output file names.  */
ایستا نویسه ثابت *additional_suffix;

/* Name of input file.  May be "-".  */
ایستا نویسه ثابت *infile = "-";

/* stat buf for input file.  */
ایستا ساختار stat in_stat_buf;

/* Descriptor on which output file is open.  */
ایستا صحیح output_desc = -1;

/* If true, print a diagnostic on standard error just before each
   output file is opened. */
ایستا bool verbose;

/* If true, don't generate zero length output files. */
ایستا bool elide_empty_files;

/* If true, in round robin mode, immediately copy
   input to output, which is much slower, so disabled by default.  */
ایستا bool unbuffered;

/* The character marking end of line.  Defaults to \n below.  */
ایستا صحیح eolchar = -1;

/* The split mode to use.  */
شمارش Split_type
{
  type_undef, type_bytes, type_byteslines, type_سطرها, type_digits,
  type_chunk_bytes, type_chunk_سطرها, type_rr
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
شمارش
{
  VERBOSE_OPTION = CHAR_MAX + 1,
  FILTER_OPTION,
  IO_BLKSIZE_OPTION,
  ADDITIONAL_SUFFIX_OPTION
};

ایستا ساختار option ثابت longopts[] =
{
  {"bytes", required_argument, پوچ, 'b'},
  {"lines", required_argument, پوچ, 'l'},
  {"line-bytes", required_argument, پوچ, 'C'},
  {"number", required_argument, پوچ, 'n'},
  {"elide-empty-files", no_argument, پوچ, 'e'},
  {"unbuffered", no_argument, پوچ, 'u'},
  {"suffix-length", required_argument, پوچ, 'a'},
  {"additional-suffix", required_argument, پوچ,
   ADDITIONAL_SUFFIX_OPTION},
  {"numeric-suffixes", optional_argument, پوچ, 'd'},
  {"hex-suffixes", optional_argument, پوچ, 'x'},
  {"filter", required_argument, پوچ, FILTER_OPTION},
  {"verbose", no_argument, پوچ, VERBOSE_OPTION},
  {"separator", required_argument, پوچ, 't'},
  {"-io-blksize", required_argument, پوچ,
   IO_BLKSIZE_OPTION}, /* do not document */
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {پوچ, 0, پوچ, 0}
};

/* Return true if the errno value, ERR, is ignorable.  */
ایستا درونخطی bool
ignorable (صحیح err)
{
  بازگشت filter_command && err == EPIPE;
}

ایستا تهی
set_suffix_length (intmax_t n_units, شمارش Split_type split_type)
{
#define DEFAULT_SUFFIX_LENGTH 2

  صحیح suffix_length_needed = 0;

  /* The suffix auto length feature is incompatible with
     a user specified start value as the generated suffixes
     are not all consecutive.  */
  اگر (numeric_suffix_start)
    suffix_auto = false;

  /* Auto-calculate the suffix length if the number of files is given.  */
  اگر (split_type == type_chunk_bytes || split_type == type_chunk_سطرها
      || split_type == type_rr)
    {
      intmax_t n_units_end = n_units - 1;
      اگر (numeric_suffix_start)
        {
          intmax_t n_start;
          strtol_error e = xstrtoimax (numeric_suffix_start, پوچ, 10,
                                       &n_start, "");
          اگر (e == LONGINT_OK && n_start < n_units)
            {
              /* Restrict auto adjustment so we don't keep
                 incrementing a suffix size arbitrarily,
                 as that would break sort order for files
                 generated from multiple split runs.  */
              اگر (ckd_add (&n_units_end, n_units_end, n_start))
                n_units_end = INTMAX_MAX;
            }

        }
      idx_t alphabet_len = strlen (suffix_alphabet);
      انجامبده
        suffix_length_needed++;
      وقتی (n_units_end /= alphabet_len);

      suffix_auto = false;
    }

  اگر (suffix_length)            /* set by user */
    {
      اگر (suffix_length < suffix_length_needed)
        error (EXIT_FAILURE, 0,
               _("the suffix length needs to be at least %d"),
               suffix_length_needed);
      suffix_auto = false;
      بازگشت;
    }
  وگرنه
    suffix_length = MAX (DEFAULT_SUFFIX_LENGTH, suffix_length_needed);
}

تهی
usage (صحیح status)
{
  اگر (status != EXIT_SUCCESS)
    emit_try_help ();
  وگرنه
    {
      printf (_("\
Usage: %s [OPTION]... [FILE [PREFIX]]\n\
"),
              program_name);
      fputs (_("\
Output pieces of FILE to PREFIXaa, PREFIXab, ...;\n\
پیشفرض size is 1000 سطرها, and پیشفرض PREFIX is 'x'.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oprintf (_("\
  -a, --suffix-length=N\n\
         generate suffixes of length N (پیشفرض %d)\n\
"), DEFAULT_SUFFIX_LENGTH);
      oputs (_("\
      --additional-suffix=SUFFIX\n\
         append an additional SUFFIX to file names\n\
"));
      oputs (_("\
  -b, --bytes=SIZE\n\
         put SIZE bytes per output file\n\
"));
      oputs (_("\
  -C, --سطر-bytes=SIZE\n\
         put at most SIZE bytes of records per output file\n\
"));
      oputs (_("\
  -d\n\
         use numeric suffixes starting at 0, not alphabetic\n\
"));
      oputs (_("\
      --numeric-suffixes[=FROM]\n\
         same as -d, but allow setting the start مقدار\n\
"));
      oputs (_("\
  -x\n\
         use hex suffixes starting at 0, not alphabetic\n\
"));
      oputs (_("\
      --hex-suffixes[=FROM]\n\
         same as -x, but allow setting the start مقدار\n\
"));
      oputs (_("\
  -e, --elide-empty-files\n\
         انجامبده not generate empty output files with '-n'\n\
"));
      oputs (_("\
      --filter=COMMAND\n\
         اكتب to shell COMMAND; file name is $FILE\n\
"));
      oputs (_("\
  -l, --سطرها=عدد_2\n\
         put عدد_2 سطرها/records per output file\n\
"));
      oputs (_("\
  -n, --عدد=CHUNKS\n\
         generate CHUNKS output files; see explanation below\n\
"));
      oputs (_("\
  -t, --separator=SEP\n\
         use SEP instead of newline as the record separator;\n\
         '\\0' (zero) specifies the NUL character\n\
"));
      oputs (_("\
  -u, --unbuffered\n\
         immediately copy درون_داد to output with '-n r/...'\n\
"));
      oputs (_("\
      --verbose\n\
         print a diagnostic just before each output file is opened\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_size_note ();
      fputs (_("\n\
CHUNKS may be:\n\
  N       split into N files based on size of درون_داد\n\
  K/N     output Kth of N to standard output\n\
  l/N     split into N files without splitting سطرها/records\n\
  l/K/N   output Kth of N to standard output without splitting سطرها/records\n\
  r/N     like 'l' but use round robin distribution\n\
  r/K/N   likewise but only output Kth of N to standard output\n\
"), stdout);
      fputs (_("\n\
-n (except -nr) will buffer to $TMPDIR, defaulting to /tmp,\n\
اگر the درون_داد size cannot easily be determined.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Copy the data in FD to a temporary file, then make that file FD.
   Use BUF, of size BUFSIZE, to copy.  Return the number of
   bytes copied, or -1 (setting errno) on error.  */
ایستا off_t
copy_to_tmpfile (صحیح fd, نویسه *buf, idx_t bufsize)
{
  FILE *tmp;
  اگر (!temp_stream (&tmp, پوچ))
    بازگشت -1;
  off_t copied = 0;
  off_t r;

  وقتی (0 < (r = اقرأ (fd, buf, bufsize)))
    {
      اگر (fwrite (buf, 1, r, tmp) != r)
        بازگشت -1;
      اگر (ckd_add (&copied, copied, r))
        {
          errno = EOVERFLOW;
          بازگشت -1;
        }
    }

  اگر (r < 0)
    بازگشت r;
  r = dup2 (fileno (tmp), fd);
  اگر (r < 0)
    بازگشت r;
  اگر (fclose (tmp) < 0)
    بازگشت -1;
  بازگشت copied;
}

/* Return the number of bytes that can be read from FD with status ST.
   Store up to the first BUFSIZE bytes of the file's data into BUF,
   and advance the file position by the number of bytes read.  On
   input error, set errno and return -1.  */

ایستا off_t
درون_داد_file_size (صحیح fd, ساختار stat ثابت *st, نویسه *buf, idx_t bufsize)
{
  off_t size = 0;
  انجامبده
    {
      ssize_t n_read = اقرأ (fd, buf + size, bufsize - size);
      اگر (n_read <= 0)
        بازگشت n_read < 0 ? n_read : size;
      size += n_read;
    }
  وقتی (size < bufsize);

  off_t cur, end;
  اگر ((usable_st_size (st) && st->st_size < size)
      || (cur = lseek (fd, 0, SEEK_CUR)) < 0
      || cur < size /* E.g., /dev/zero on GNU/Linux.  */
      || (end = lseek (fd, 0, SEEK_END)) < 0)
    {
      نویسه *tmpbuf = xmalloc (bufsize);
      end = copy_to_tmpfile (fd, tmpbuf, bufsize);
      آزاد (tmpbuf);
      اگر (end < 0)
        بازگشت end;
      cur = 0;
    }

  اگر (end == OFF_T_MAX /* E.g., /dev/zero on GNU/Hurd.  */
      || (cur < end && ckd_add (&size, size, end - cur)))
    {
      errno = EOVERFLOW;
      بازگشت -1;
    }

  اگر (cur < end)
    {
      off_t r = lseek (fd, cur, SEEK_SET);
      اگر (r < 0)
        بازگشت r;
    }

  بازگشت size;
}

/* Compute the next sequential output file name and store it into the
   string 'outfile'.  */

ایستا تهی
next_file_name (تهی)
{
  /* Index in suffix_alphabet of each character in the suffix.  */
  ایستا idx_t *sufindex;
  ایستا idx_t outbase_length;
  ایستا idx_t outfile_length;
  ایستا idx_t addsuf_length;

  اگر (! outfile)
    {
      bool overflow, widen;

new_name:
      widen = !! outfile_length;

      اگر (! widen)
        {
          /* Allocate and initialize the first file name.  */

          outbase_length = strlen (outbase);
          addsuf_length = additional_suffix ? strlen (additional_suffix) : 0;
          overflow = ckd_add (&outfile_length, outbase_length + addsuf_length,
                              suffix_length);
        }
      وگرنه
        {
          /* Reallocate and initialize a new wider file name.
             We do this by subsuming the unchanging part of
             the generated suffix into the prefix (base), and
             reinitializing the now one longer suffix.  */

          overflow = ckd_add (&outfile_length, outfile_length, 2);
          suffix_length++;
        }

      idx_t outfile_size;
      overflow |= ckd_add (&outfile_size, outfile_length, 1);
      اگر (overflow)
        xalloc_die ();
      outfile = xirealloc (outfile, outfile_size);

      اگر (! widen)
        memcpy (outfile, outbase, outbase_length);
      وگرنه
        {
          /* Append the last alphabet character to the file name prefix.  */
          outfile[outbase_length] = suffix_alphabet[sufindex[0]];
          outbase_length++;
        }

      outfile_mid = outfile + outbase_length;
      memset (outfile_mid, suffix_alphabet[0], suffix_length);
      اگر (additional_suffix)
        memcpy (outfile_mid + suffix_length, additional_suffix, addsuf_length);
      outfile[outfile_length] = 0;

      آزاد (sufindex);
      sufindex = xicalloc (suffix_length, اندازه *sufindex);

      اگر (numeric_suffix_start)
        {
          affirm (! widen);

          /* Update the output file name.  */
          idx_t i = strlen (numeric_suffix_start);
          memcpy (outfile_mid + suffix_length - i, numeric_suffix_start, i);

          /* Update the suffix index.  */
          idx_t *sufindex_end = sufindex + suffix_length;
          وقتی (i-- != 0)
            *--sufindex_end = numeric_suffix_start[i] - '0';
        }

#اگر ! _POSIX_NO_TRUNC && HAVE_PATHCONF && defined _PC_NAME_MAX
      /* POSIX requires that if the output file name is too long for
         its directory, 'split' must fail without creating any files.
         This must be checked for explicitly on operating systems that
         silently truncate file names.  */
      {
        نویسه *dir = dir_name (outfile);
        long name_max = pathconf (dir, _PC_NAME_MAX);
        اگر (0 <= name_max && name_max < base_len (last_component (outfile)))
          error (EXIT_FAILURE, ENAMETOOLONG, "%s", quotef (outfile));
        آزاد (dir);
      }
#endif
    }
  وگرنه
    {
      /* Increment the suffix in place, if possible.  */

      idx_t i = suffix_length;
      وقتی (i-- != 0)
        {
          sufindex[i]++;
          اگر (suffix_auto && i == 0 && ! suffix_alphabet[sufindex[0] + 1])
            برو new_name;
          outfile_mid[i] = suffix_alphabet[sufindex[i]];
          اگر (outfile_mid[i])
            بازگشت;
          sufindex[i] = 0;
          outfile_mid[i] = suffix_alphabet[sufindex[i]];
        }
      error (EXIT_FAILURE, 0, _("output file suffixes exhausted"));
    }
}

/* Create or truncate a file.  */

ایستا صحیح
create (نویسه ثابت *name)
{
  اگر (!filter_command)
    {
      اگر (verbose)
        fprintf (stdout, _("creating file %s\n"), quoteaf (name));

      صحیح oflags = O_WRONLY | O_CREAT | O_دودویی;
      صحیح fd = open (name, oflags | O_EXCL, MODE_RW_UGO);
      اگر (0 <= fd || errno != EEXIST)
        بازگشت fd;
      fd = open (name, oflags, MODE_RW_UGO);
      اگر (fd < 0)
        بازگشت fd;
      ساختار stat out_stat_buf;
      اگر (fstat (fd, &out_stat_buf) != 0)
        error (EXIT_FAILURE, errno, _("failed to stat %s"), quoteaf (name));
      اگر (psame_inode (&in_stat_buf, &out_stat_buf))
        error (EXIT_FAILURE, 0, _("%s would overwrite input; aborting"),
               quoteaf (name));
      اگر (ftruncate (fd, 0) < 0
          && (S_ISREG (out_stat_buf.st_mode) || S_TYPEISSHM (&out_stat_buf)))
        error (EXIT_FAILURE, errno, _("%s: error truncating"), quotef (name));

      بازگشت fd;
    }
  وگرنه
    {
      اگر (setenv ("FILE", name, 1) != 0)
        error (EXIT_FAILURE, errno,
               _("failed to set FILE environment variable"));
      اگر (verbose)
        fprintf (stdout, _("executing with FILE=%s\n"), quotef (name));

      صحیح result;
      صحیح fd_pair[2];
      pid_t child_pid;

      posix_spawnattr_t attr;
      posix_spawn_file_actions_t actions;

      sigset_t set;
      sigemptyset (&set);
      اگر (default_SIGPIPE)
        sigaddset (&set, SIGPIPE);

      اگر (   (result = posix_spawnattr_init (&attr))
          || (result = posix_spawnattr_setflags (&attr,
                                                 (POSIX_SPAWN_USEVFORK
                                                  | POSIX_SPAWN_SETSIGDEF)))
          || (result = posix_spawnattr_setsigdefault (&attr, &set))
          || (result = posix_spawn_file_actions_init (&actions))
         )
        error (EXIT_FAILURE, result, _("posix_spawn initialization failed"));

      اگر (pipe (fd_pair) != 0)
        error (EXIT_FAILURE, errno, _("failed to create pipe"));

      /* We have to close any pipes that were opened during an
         earlier call, otherwise this process will be holding a
         write-pipe that will prevent the earlier process from
         reading an EOF on the corresponding read-pipe.  */
      برای (صحیح i = 0; i < open_pipes.n_pipes; ++i)
        اگر ((result = posix_spawn_file_actions_addclose (&actions,
                                                         open_pipes.pipes[i])))
          بشکن;

      اگر (   result
          || (result = posix_spawn_file_actions_addclose (&actions, fd_pair[1]))
          || (fd_pair[0] != STDIN_FILENO
              && (   (result = posix_spawn_file_actions_adddup2 (&actions,
                                                                 fd_pair[0],
                                                                 STDIN_FILENO))
                  || (result = posix_spawn_file_actions_addclose (&actions,
                                                                  fd_pair[0]))))
         )
        error (EXIT_FAILURE, result, _("posix_spawn setup failed"));


      نویسه ثابت *shell_prog = getenv ("SHELL");
      اگر (shell_prog == پوچ)
        shell_prog = "/bin/sh";
      نویسه ثابت *ثابت argv[] = { last_component (shell_prog), "-c",
                                   filter_command, پوچ };

      result = posix_spawn (&child_pid, shell_prog, &actions, &attr,
                            (نویسه * ثابت *) argv, environ);
      اگر (result != 0)
        error (EXIT_FAILURE, errno, _("failed to run command: \"%s -c %s\""),
               shell_prog, filter_command);

      posix_spawnattr_destroy (&attr);
      posix_spawn_file_actions_destroy (&actions);

      اگر (close (fd_pair[0]) != 0)
        error (EXIT_FAILURE, errno, _("failed to close input pipe"));
      filter_pid = child_pid;
      اگر (open_pipes.n_pipes == open_pipes.n_pipes_alloc)
        open_pipes.pipes = xpalloc (open_pipes.pipes, &open_pipes.n_pipes_alloc,
                                    1, MIN (INT_MAX, IDX_MAX),
                                    اندازه *open_pipes.pipes);
      idx_t ثابت pipe_idx = open_pipes.n_pipes++;
      open_pipes.pipes[pipe_idx] = fd_pair[1];
      بازگشت fd_pair[1];
    }
}

/* Close the output file, and do any associated cleanup.
   If FP and FD are both specified, they refer to the same open file;
   in this case FP is closed, but FD is still used in cleanup.  */
ایستا تهی
closeout (FILE *fp, صحیح fd, pid_t pid, نویسه ثابت *name)
{
  اگر (fp != پوچ && fclose (fp) != 0 && ! ignorable (errno))
    error (EXIT_FAILURE, errno, "%s", quotef (name));
  اگر (fd >= 0)
    {
      اگر (fp == پوچ && close (fd) < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (name));
      برای (صحیح j = 0; j < open_pipes.n_pipes; ++j)
        {
          اگر (open_pipes.pipes[j] == fd)
            {
              idx_t ثابت last_idx = open_pipes.n_pipes - 1;
              open_pipes.pipes[j] = open_pipes.pipes[last_idx];
              open_pipes.n_pipes--;
              بشکن;
            }
        }
    }
  اگر (pid > 0)
    {
      صحیح wstatus;
      اگر (waitpid (pid, &wstatus, 0) < 0)
        error (EXIT_FAILURE, errno, _("waiting for child process"));
      وگرنه اگر (WIFSIGNALED (wstatus))
        {
          صحیح sig = WTERMSIG (wstatus);
          اگر (sig != SIGPIPE)
            {
              نویسه signame[MAX (SIG2STR_MAX, INT_BUFSIZE_BOUND (صحیح))];
              اگر (sig2str (sig, signame) != 0)
                sprintf (signame, "%d", sig);
              error (sig + 128, 0,
                     _("with FILE=%s, signal %s from command: %s"),
                     quotef (name), signame, filter_command);
            }
        }
      وگرنه اگر (WIFEXITED (wstatus))
        {
          صحیح ex = WEXITSTATUS (wstatus);
          اگر (ex != 0)
            error (ex, 0, _("with FILE=%s, exit %d from command: %s"),
                   quotef (name), ex, filter_command);
        }
      وگرنه
        {
          /* shouldn't happen.  */
          error (EXIT_FAILURE, 0,
                 _("unknown status from command (0x%X)"), wstatus + 0u);
        }
    }
}

/* Write BYTES bytes at BP to an output file.
   If NEW_FILE_FLAG is true, open the next output file.
   Otherwise add to the same output file already in use.
   Return true if successful.  */

ایستا bool
cwrite (bool new_file_flag, نویسه ثابت *bp, idx_t bytes)
{
  اگر (new_file_flag)
    {
      اگر (!bp && bytes == 0 && elide_empty_files)
        بازگشت true;
      closeout (پوچ, output_desc, filter_pid, outfile);
      next_file_name ();
      output_desc = create (outfile);
      اگر (output_desc < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (outfile));
    }

  اگر (full_write (output_desc, bp, bytes) == bytes)
    بازگشت true;
  وگرنه
    {
      اگر (! ignorable (errno))
        error (EXIT_FAILURE, errno, "%s", quotef (outfile));
      بازگشت false;
    }
}

/* Split into pieces of exactly N_BYTES bytes.
   However, the first REM_BYTES pieces should be 1 byte longer.
   Use buffer BUF, whose size is BUFSIZE.
   If INITIAL_READ is nonnegative,
   BUF contains the first INITIAL_READ input bytes.  */

ایستا تهی
bytes_split (intmax_t n_bytes, intmax_t rem_bytes,
             نویسه *buf, idx_t bufsize, ssize_t initial_read,
             intmax_t max_files)
{
  bool new_file_flag = true;
  bool filter_ok = true;
  intmax_t opened = 0;
  intmax_t to_write = n_bytes + (0 < rem_bytes);
  bool eof = ! to_write;

  وقتی (! eof)
    {
      ssize_t n_read;
      اگر (0 <= initial_read)
        {
          n_read = initial_read;
          initial_read = -1;
          eof = n_read < bufsize;
        }
      وگرنه
        {
          اگر (! filter_ok
              && 0 <= lseek (STDIN_FILENO, to_write, SEEK_CUR))
            {
              to_write = n_bytes + (opened + 1 < rem_bytes);
              new_file_flag = true;
            }

          n_read = اقرأ (STDIN_FILENO, buf, bufsize);
          اگر (n_read < 0)
            error (EXIT_FAILURE, errno, "%s", quotef (infile));
          eof = n_read == 0;
        }
      نویسه *bp_out = buf;
      وقتی (0 < to_write && to_write <= n_read)
        {
          اگر (filter_ok || new_file_flag)
            filter_ok = cwrite (new_file_flag, bp_out, to_write);
          opened += new_file_flag;
          new_file_flag = !max_files || (opened < max_files);
          اگر (! filter_ok && ! new_file_flag)
            {
              /* If filters no longer accepting input, stop reading.  */
              n_read = 0;
              eof = true;
              بشکن;
            }
          bp_out += to_write;
          n_read -= to_write;
          to_write = n_bytes + (opened < rem_bytes);
        }
      اگر (0 < n_read)
        {
          اگر (filter_ok || new_file_flag)
            filter_ok = cwrite (new_file_flag, bp_out, n_read);
          opened += new_file_flag;
          new_file_flag = false;
          اگر (! filter_ok && opened == max_files)
            {
              /* If filters no longer accepting input, stop reading.  */
              بشکن;
            }
          to_write -= n_read;
        }
    }

  /* Ensure NUMBER files are created, which truncates
     any existing files or notifies any consumers on fifos.
     FIXME: Should we do this before EXIT_FAILURE?  */
  وقتی (opened++ < max_files)
    cwrite (true, پوچ, 0);
}

/* Split into pieces of exactly N_LINES lines.
   Use buffer BUF, whose size is BUFSIZE.  */

ایستا تهی
سطرها_split (intmax_t n_سطرها, نویسه *buf, idx_t bufsize)
{
  ssize_t n_read;
  نویسه *bp, *bp_out, *eob;
  bool new_file_flag = true;
  intmax_t n = 0;

  انجامبده
    {
      n_read = اقرأ (STDIN_FILENO, buf, bufsize);
      اگر (n_read < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
      bp = bp_out = buf;
      eob = bp + n_read;
      *eob = eolchar;
      وقتی (true)
        {
          bp = rawmemchr (bp, eolchar);
          اگر (bp == eob)
            {
              اگر (eob != bp_out) /* do not write 0 bytes! */
                {
                  idx_t len = eob - bp_out;
                  cwrite (new_file_flag, bp_out, len);
                  new_file_flag = false;
                }
              بشکن;
            }

          ++bp;
          اگر (++n >= n_سطرها)
            {
              cwrite (new_file_flag, bp_out, bp - bp_out);
              bp_out = bp;
              new_file_flag = true;
              n = 0;
            }
        }
    }
  وقتی (n_read);
}

/* Split into pieces that are as large as possible while still not more
   than N_BYTES bytes, and are split on line boundaries except
   where lines longer than N_BYTES bytes occur. */

ایستا تهی
سطر_bytes_split (intmax_t n_bytes, نویسه *buf, idx_t bufsize)
{
  ssize_t n_read;
  intmax_t n_out = 0;      /* for each split.  */
  idx_t n_hold = 0;
  نویسه *hold = پوچ;        /* for lines > bufsize.  */
  idx_t hold_size = 0;
  bool split_سطر = false;  /* Whether a \n was output in a split.  */

  انجامبده
    {
      n_read = اقرأ (STDIN_FILENO, buf, bufsize);
      اگر (n_read < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
      idx_t n_چپ = n_read;
      نویسه *sob = buf;
      وقتی (n_چپ)
        {
          idx_t split_rest = 0;
          نویسه *eoc = پوچ;
          نویسه *eol;

          /* Determine End Of Chunk and/or End of Line,
             which are used below to select what to write or buffer.  */
          اگر (n_bytes - n_out - n_hold <= n_چپ)
            {
              /* Have enough for split.  */
              split_rest = n_bytes - n_out - n_hold;
              eoc = sob + split_rest - 1;
              eol = memrchr (sob, eolchar, split_rest);
            }
          وگرنه
            eol = memrchr (sob, eolchar, n_چپ);

          /* Output hold space if possible.  */
          اگر (n_hold && !(!eol && n_out))
            {
              cwrite (n_out == 0, hold, n_hold);
              n_out += n_hold;
              n_hold = 0;
            }

          /* Output to eol if present.  */
          اگر (eol)
            {
              split_سطر = true;
              idx_t n_write = eol - sob + 1;
              cwrite (n_out == 0, sob, n_write);
              n_out += n_write;
              n_چپ -= n_write;
              sob += n_write;
              اگر (eoc)
                split_rest -= n_write;
            }

          /* Output to eoc or eob if possible.  */
          اگر (n_چپ && !split_سطر)
            {
              idx_t n_write = eoc ? split_rest : n_چپ;
              cwrite (n_out == 0, sob, n_write);
              n_out += n_write;
              n_چپ -= n_write;
              sob += n_write;
              اگر (eoc)
                split_rest -= n_write;
            }

          /* Update hold if needed.  */
          اگر ((eoc && split_rest) || (!eoc && n_چپ))
            {
              idx_t n_buf = eoc ? split_rest : n_چپ;
              اگر (hold_size - n_hold < n_buf)
                hold = xpalloc (hold, &hold_size, n_buf - (hold_size - n_hold),
                                -1, اندازه *hold);
              memcpy (hold + n_hold, sob, n_buf);
              n_hold += n_buf;
              n_چپ -= n_buf;
              sob += n_buf;
            }

          /* Reset for new split.  */
          اگر (eoc)
            {
              n_out = 0;
              split_سطر = false;
            }
        }
    }
  وقتی (n_read);

  /* Handle no eol at end of file.  */
  اگر (n_hold)
    cwrite (n_out == 0, hold, n_hold);

  آزاد (hold);
}

/* -n l/[K/]N: Write lines to files of approximately file size / N.
   The file is partitioned into file size / N sized portions, with the
   last assigned any excess.  If a line _starts_ within a partition
   it is written completely to the corresponding file.  Since lines
   are not split even if they overlap a partition, the files written
   can be larger or smaller than the partition size, and even empty
   if a line is so long as to completely overlap the partition.  */

ایستا تهی
سطرها_chunk_split (intmax_t k, intmax_t n, نویسه *buf, idx_t bufsize,
                   ssize_t initial_read, off_t file_size)
{
  affirm (n && k <= n);

  intmax_t rem_bytes = file_size % n;
  off_t chunk_size = file_size / n;
  intmax_t chunk_no = 1;
  off_t chunk_end = chunk_size + (0 < rem_bytes);
  off_t n_written = 0;
  bool new_file_flag = true;
  bool chunk_truncated = false;

  اگر (k > 1 && 0 < file_size)
    {
      /* Start reading 1 byte before kth chunk of file.  */
      off_t start = (k - 1) * chunk_size + MIN (k - 1, rem_bytes) - 1;
      اگر (start < initial_read)
        {
          memmove (buf, buf + start, initial_read - start);
          initial_read -= start;
        }
      وگرنه
        {
          اگر (initial_read < start
              && lseek (STDIN_FILENO, start - initial_read, SEEK_CUR) < 0)
            error (EXIT_FAILURE, errno, "%s", quotef (infile));
          initial_read = -1;
        }
      n_written = start;
      chunk_no = k - 1;
      chunk_end = start + 1;
    }

  وقتی (n_written < file_size)
    {
      نویسه *bp = buf, *eob;
      ssize_t n_read;
      اگر (0 <= initial_read)
        {
          n_read = initial_read;
          initial_read = -1;
        }
      وگرنه
        {
          n_read = اقرأ (STDIN_FILENO, buf,
                         MIN (bufsize, file_size - n_written));
          اگر (n_read < 0)
            error (EXIT_FAILURE, errno, "%s", quotef (infile));
        }
      اگر (n_read == 0)
        بشکن; /* eof.  */
      chunk_truncated = false;
      eob = buf + n_read;

      وقتی (bp != eob)
        {
          idx_t to_write;
          bool next = false;

          /* Begin looking for '\n' at last byte of chunk.  */
          off_t skip = MIN (n_read, MAX (0, chunk_end - 1 - n_written));
          نویسه *bp_out = memchr (bp + skip, eolchar, n_read - skip);
          اگر (bp_out)
            {
              bp_out++;
              next = true;
            }
          وگرنه
            bp_out = eob;
          to_write = bp_out - bp;

          اگر (k == chunk_no)
            {
              /* We don't use the stdout buffer here since we're writing
                 large chunks from an existing file, so it's more efficient
                 to write out directly.  */
              اگر (full_write (STDOUT_FILENO, bp, to_write) != to_write)
                write_error ();
            }
          وگرنه اگر (! k)
            cwrite (new_file_flag, bp, to_write);
          n_written += to_write;
          bp += to_write;
          n_read -= to_write;
          new_file_flag = next;

          /* A line could have been so long that it skipped
             entire chunks. So create empty files in that case.  */
          وقتی (next || chunk_end <= n_written)
            {
              اگر (!next && bp == eob)
                {
                  /* replenish buf, before going to next chunk.  */
                  chunk_truncated = true;
                  بشکن;
                }
              اگر (k == chunk_no)
                بازگشت;
              chunk_end += chunk_size + (chunk_no < rem_bytes);
              chunk_no++;
              اگر (chunk_end <= n_written)
                {
                  اگر (! k)
                    cwrite (true, پوچ, 0);
                }
              وگرنه
                next = false;
            }
        }
    }

  اگر (chunk_truncated)
    chunk_no++;

  /* Ensure NUMBER files are created, which truncates
     any existing files or notifies any consumers on fifos.
     FIXME: Should we do this before EXIT_FAILURE?  */
  اگر (!k)
    وقتی (chunk_no++ <= n)
      cwrite (true, پوچ, 0);
}

/* -n K/N: Extract Kth of N chunks.  */

ایستا تهی
bytes_chunk_extract (intmax_t k, intmax_t n, نویسه *buf, idx_t bufsize,
                     ssize_t initial_read, off_t file_size)
{
  off_t start;
  off_t end;

  affirm (0 < k && k <= n);

  start = (k - 1) * (file_size / n) + MIN (k - 1, file_size % n);
  end = k == n ? file_size : k * (file_size / n) + MIN (k, file_size % n);

  اگر (start < initial_read)
    {
      memmove (buf, buf + start, initial_read - start);
      initial_read -= start;
    }
  وگرنه
    {
      اگر (initial_read < start
          && lseek (STDIN_FILENO, start - initial_read, SEEK_CUR) < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
      initial_read = -1;
    }

  وقتی (start < end)
    {
      ssize_t n_read;
      اگر (0 <= initial_read)
        {
          n_read = initial_read;
          initial_read = -1;
        }
      وگرنه
        {
          n_read = اقرأ (STDIN_FILENO, buf, bufsize);
          اگر (n_read < 0)
            error (EXIT_FAILURE, errno, "%s", quotef (infile));
        }
      اگر (n_read == 0)
        بشکن; /* eof.  */
      n_read = MIN (n_read, end - start);
      اگر (full_write (STDOUT_FILENO, buf, n_read) != n_read
          && ! ignorable (errno))
        error (EXIT_FAILURE, errno, "%s", quotef ("-"));
      start += n_read;
    }
}

تعریفنوع ساختار of_info
{
  نویسه *of_name;
  صحیح ofd;
  FILE *ofile;
  pid_t opid;
} of_t;

شمارش
{
  OFD_NEW = -1,
  OFD_APPEND = -2
};

/* Rotate file descriptors when we're writing to more output files than we
   have available file descriptors.
   Return whether we came under file resource pressure.
   If so, it's probably best to close each file when finished with it.  */

ایستا bool
ofile_open (of_t *files, idx_t i_check, idx_t nfiles)
{
  bool file_limit = false;

  اگر (files[i_check].ofd <= OFD_NEW)
    {
      صحیح fd;
      idx_t i_reopen = i_check ? i_check - 1 : nfiles - 1;

      /* Another process could have opened a file in between the calls to
         close and open, so we should keep trying until open succeeds or
         we've closed all of our files.  */
      وقتی (true)
        {
          اگر (files[i_check].ofd == OFD_NEW)
            fd = create (files[i_check].of_name);
          وگرنه /* OFD_APPEND  */
            {
              /* Attempt to append to previously opened file.
                 We use O_NONBLOCK to support writing to fifos,
                 where the other end has closed because of our
                 previous close.  In that case we'll immediately
                 get an error, rather than waiting indefinitely.
                 In specialized cases the consumer can keep reading
                 from the fifo, terminating on conditions in the data
                 itself, or perhaps never in the case of 'tail -f'.
                 I.e., for fifos it is valid to attempt this reopen.

                 We don't handle the filter_command case here, as create()
                 will exit if there are not enough files in that case.
                 I.e., we don't support restarting filters, as that would
                 put too much burden on users specifying --filter commands.  */
              fd = open (files[i_check].of_name,
                         O_WRONLY | O_دودویی | O_APPEND | O_NONBLOCK);
            }

          اگر (0 <= fd)
            بشکن;

          اگر (!(errno == EMFILE || errno == ENFILE))
            error (EXIT_FAILURE, errno, "%s", quotef (files[i_check].of_name));

          file_limit = true;

          /* Search backwards for an open file to close.  */
          وقتی (files[i_reopen].ofd < 0)
            {
              i_reopen = i_reopen ? i_reopen - 1 : nfiles - 1;
              /* No more open files to close, exit with E[NM]FILE.  */
              اگر (i_reopen == i_check)
                error (EXIT_FAILURE, errno, "%s",
                       quotef (files[i_check].of_name));
            }

          اگر (fclose (files[i_reopen].ofile) != 0)
            error (EXIT_FAILURE, errno, "%s", quotef (files[i_reopen].of_name));
          files[i_reopen].ofile = پوچ;
          files[i_reopen].ofd = OFD_APPEND;
        }

      files[i_check].ofd = fd;
      FILE *ofile = fdopen (fd, "a");
      اگر (!ofile)
        error (EXIT_FAILURE, errno, "%s", quotef (files[i_check].of_name));
      files[i_check].ofile = ofile;
      files[i_check].opid = filter_pid;
      filter_pid = 0;
    }

  بازگشت file_limit;
}

/* -n r/[K/]N: Divide file into N chunks in round robin fashion.
   Use BUF of size BUFSIZE for the buffer, and if allocating storage
   put its address into *FILESP to pacify -fsanitize=leak.
   When K == 0, we try to keep the files open in parallel.
   If we run out of file resources, then we revert
   to opening and closing each file for each line.  */

ایستا تهی
سطرها_rr (intmax_t k, intmax_t n, نویسه *buf, idx_t bufsize, of_t **filesp)
{
  bool wrapped = false;
  bool wrote = false;
  bool file_limit;
  idx_t i_file;
  of_t *files IF_LINT (= پوچ);
  intmax_t سطر_no;

  اگر (k)
    سطر_no = 1;
  وگرنه
    {
      اگر (IDX_MAX < n)
        xalloc_die ();
      files = *filesp = xinmalloc (n, اندازه *files);

      /* Generate output file names. */
      برای (i_file = 0; i_file < n; i_file++)
        {
          next_file_name ();
          files[i_file].of_name = xstrdup (outfile);
          files[i_file].ofd = OFD_NEW;
          files[i_file].ofile = پوچ;
          files[i_file].opid = 0;
        }
      i_file = 0;
      file_limit = false;
    }

  وقتی (true)
    {
      نویسه *bp = buf, *eob;
      ssize_t n_read = اقرأ (STDIN_FILENO, buf, bufsize);
      اگر (n_read < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
      وگرنه اگر (n_read == 0)
        بشکن; /* eof.  */
      eob = buf + n_read;

      وقتی (bp != eob)
        {
          idx_t to_write;
          bool next = false;

          /* Find end of line. */
          نویسه *bp_out = memchr (bp, eolchar, eob - bp);
          اگر (bp_out)
            {
              bp_out++;
              next = true;
            }
          وگرنه
            bp_out = eob;
          to_write = bp_out - bp;

          اگر (k)
            {
              اگر (سطر_no == k && unbuffered)
                {
                  اگر (full_write (STDOUT_FILENO, bp, to_write) != to_write)
                    write_error ();
                }
              وگرنه اگر (سطر_no == k && fwrite (bp, to_write, 1, stdout) != 1)
                {
                  write_error ();
                }
              اگر (next)
                سطر_no = (سطر_no == n) ? 1 : سطر_no + 1;
            }
          وگرنه
            {
              /* Secure file descriptor. */
              file_limit |= ofile_open (files, i_file, n);
              اگر (unbuffered)
                {
                  /* Note writing to fd, rather than flushing the FILE gives
                     an 8% performance benefit, due to reduced data copying.  */
                  اگر (full_write (files[i_file].ofd, bp, to_write) != to_write
                      && ! ignorable (errno))
                    error (EXIT_FAILURE, errno, "%s",
                           quotef (files[i_file].of_name));
                }
              وگرنه اگر (fwrite (bp, to_write, 1, files[i_file].ofile) != 1
                       && ! ignorable (errno))
                error (EXIT_FAILURE, errno, "%s",
                       quotef (files[i_file].of_name));

              اگر (! ignorable (errno))
                wrote = true;

              اگر (file_limit)
                {
                  اگر (fclose (files[i_file].ofile) != 0)
                    error (EXIT_FAILURE, errno, "%s",
                           quotef (files[i_file].of_name));
                  files[i_file].ofile = پوچ;
                  files[i_file].ofd = OFD_APPEND;
                }
              اگر (next && ++i_file == n)
                {
                  wrapped = true;
                  /* If no filters are accepting input, stop reading.  */
                  اگر (! wrote)
                    برو no_filters;
                  wrote = false;
                  i_file = 0;
                }
            }

          bp = bp_out;
        }
    }

no_filters:
  /* Ensure all files created, so that any existing files are truncated,
     and to signal any waiting fifo consumers.
     Also, close any open file descriptors.
     FIXME: Should we do this before EXIT_FAILURE?  */
  اگر (!k)
    {
      idx_t ceiling = wrapped ? n : i_file;
      برای (i_file = 0; i_file < n; i_file++)
        {
          اگر (i_file >= ceiling && !elide_empty_files)
            file_limit |= ofile_open (files, i_file, n);
          اگر (files[i_file].ofd >= 0)
            closeout (files[i_file].ofile, files[i_file].ofd,
                      files[i_file].opid, files[i_file].of_name);
          files[i_file].ofd = OFD_APPEND;
        }
    }
}

#define FAIL_ONLY_ONE_WAY()					\
  انجامبده								\
    {								\
      error (0, 0, _("cannot split in more than one way"));	\
      usage (EXIT_FAILURE);					\
    }								\
  وقتی (0)

/* Report a string-to-integer conversion failure MSGID with ARG.  */

ایستا _Noreturn تهی
strtoint_die (نویسه ثابت *msgid, نویسه ثابت *arg)
{
  error (EXIT_FAILURE, errno == EINVAL ? 0 : errno, "%s: %s",
         gettext (msgid), quote (arg));
}

/* Use OVERFLOW_OK when it is OK to ignore LONGINT_OVERFLOW errors, since the
   extreme value will do the right thing anyway on any practical platform.  */
#define OVERFLOW_OK LONGINT_OVERFLOW

/* Parse ARG for number of bytes or lines.  The number can be followed
   by MULTIPLIERS, and the resulting value must be positive.
   If the number cannot be parsed, diagnose with MSG.
   Return the number parsed, or an INTMAX_MAX on overflow.  */

ایستا intmax_t
parse_n_units (نویسه ثابت *arg, نویسه ثابت *multipliers, نویسه ثابت *msgid)
{
  intmax_t n;
  اگر (OVERFLOW_OK < xstrtoimax (arg, پوچ, 10, &n, multipliers) || n < 1)
    strtoint_die (msgid, arg);
  بازگشت n;
}

/* Parse K/N syntax of chunk options.  */

ایستا تهی
parse_chunk (intmax_t *k_units, intmax_t *n_units, نویسه ثابت *arg)
{
  نویسه *argend;
  strtol_error e = xstrtoimax (arg, &argend, 10, n_units, "");
  اگر (e == LONGINT_INVALID_SUFFIX_CHAR && *argend == '/')
    {
      *k_units = *n_units;
      *n_units = parse_n_units (argend + 1, "",
                                N_("invalid number of chunks"));
      اگر (! (0 < *k_units && *k_units <= *n_units))
        error (EXIT_FAILURE, 0, "%s: %s", _("invalid chunk number"),
               quote_mem (arg, argend - arg));
    }
  وگرنه اگر (! (e <= OVERFLOW_OK && 0 < *n_units))
    strtoint_die (N_("invalid number of chunks"), arg);
}


صحیح
اصلی (صحیح argc, نویسه **argv)
{
  شمارش Split_type split_type = type_undef;
  idx_t in_blk_size = 0;	/* optimal block size of input file device */
  idx_t page_size = getpagesize ();
  intmax_t k_units = 0;
  intmax_t n_units = 0;

  ایستا نویسه ثابت multipliers[] = "bEGKkMmPQRTYZ0";
  صحیح c;
  صحیح digits_optind = 0;
  off_t file_size = OFF_T_MAX;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* Parse command line options.  */

  وقتی (true)
    {
      /* This is the argv-index of the option we will read next.  */
      صحیح this_optind = optind ? optind : 1;

      c = getopt_long (argc, argv, "0123456789C:a:b:del:n:t:ux",
                       longopts, پوچ);
      اگر (c == -1)
        بشکن;

      انتخاب (c)
        {
        حالت 'a':
          suffix_length = xdectoimax (optarg, 0, IDX_MAX,
                                      "", _("invalid suffix length"), 0);
          بشکن;

        حالت ADDITIONAL_SUFFIX_OPTION:
          {
            صحیح suffix_len = strlen (optarg);
            اگر (last_component (optarg) != optarg
                || (suffix_len && ISSLASH (optarg[suffix_len - 1])))
              {
                error (0, 0,
                       _("invalid suffix %s, contains directory separator"),
                       quote (optarg));
                usage (EXIT_FAILURE);
              }
          }
          additional_suffix = optarg;
          بشکن;

        حالت 'b':
          اگر (split_type != type_undef)
            FAIL_ONLY_ONE_WAY ();
          split_type = type_bytes;
          n_units = parse_n_units (optarg, multipliers,
                                   N_("invalid number of bytes"));
          بشکن;

        حالت 'l':
          اگر (split_type != type_undef)
            FAIL_ONLY_ONE_WAY ();
          split_type = type_سطرها;
          n_units = parse_n_units (optarg, "", N_("invalid number of lines"));
          بشکن;

        حالت 'C':
          اگر (split_type != type_undef)
            FAIL_ONLY_ONE_WAY ();
          split_type = type_byteslines;
          n_units = parse_n_units (optarg, multipliers,
                                   N_("invalid number of lines"));
          بشکن;

        حالت 'n':
          اگر (split_type != type_undef)
            FAIL_ONLY_ONE_WAY ();
          /* skip any whitespace */
          وقتی (isspace (to_uchar (*optarg)))
            optarg++;
          اگر (STRNCMP_LIT (optarg, "r/") == 0)
            {
              split_type = type_rr;
              optarg += 2;
            }
          وگرنه اگر (STRNCMP_LIT (optarg, "l/") == 0)
            {
              split_type = type_chunk_سطرها;
              optarg += 2;
            }
          وگرنه
            split_type = type_chunk_bytes;
          parse_chunk (&k_units, &n_units, optarg);
          بشکن;

        حالت 'u':
          unbuffered = true;
          بشکن;

        حالت 't':
          {
            نویسه neweol = optarg[0];
            اگر (! neweol)
              error (EXIT_FAILURE, 0, _("empty record separator"));
            اگر (optarg[1])
              {
                اگر (streq (optarg, "\\0"))
                  neweol = '\0';
                وگرنه
                  {
                    /* Provoke with 'split -txx'.  Complain about
                       "multi-character tab" instead of "multibyte tab", so
                       that the diagnostic's wording does not need to be
                       changed once multibyte characters are supported.  */
                    error (EXIT_FAILURE, 0, _("multi-character separator %s"),
                           quote (optarg));
                  }
              }
            /* Make it explicit we don't support multiple separators.  */
            اگر (0 <= eolchar && neweol != eolchar)
              {
                error (EXIT_FAILURE, 0,
                       _("multiple separator characters specified"));
              }

            eolchar = neweol;
          }
          بشکن;

        حالت '0':
        حالت '1':
        حالت '2':
        حالت '3':
        حالت '4':
        حالت '5':
        حالت '6':
        حالت '7':
        حالت '8':
        حالت '9':
          اگر (split_type == type_undef)
            {
              split_type = type_digits;
              n_units = 0;
            }
          اگر (split_type != type_undef && split_type != type_digits)
            FAIL_ONLY_ONE_WAY ();
          اگر (digits_optind != 0 && digits_optind != this_optind)
            n_units = 0;	/* More than one number given; ignore other. */
          digits_optind = this_optind;
          اگر (ckd_mul (&n_units, n_units, 10)
              || ckd_add (&n_units, n_units, c - '0'))
            n_units = INTMAX_MAX;
          بشکن;

        حالت 'd':
        حالت 'x':
          اگر (c == 'd')
            suffix_alphabet = "0123456789";
          وگرنه
            suffix_alphabet = "0123456789abcdef";
          اگر (optarg)
            {
              اگر (strlen (optarg) != strspn (optarg, suffix_alphabet))
                {
                  error (0, 0,
                         (c == 'd') ?
                           _("%s: invalid start value for numerical suffix") :
                           _("%s: invalid start value for hexadecimal suffix"),
                         quote (optarg));
                  usage (EXIT_FAILURE);
                }
              وگرنه
                {
                  /* Skip any leading zero.  */
                  وقتی (*optarg == '0' && *(optarg + 1) != '\0')
                    optarg++;
                  numeric_suffix_start = optarg;
                }
            }
          بشکن;

        حالت 'e':
          elide_empty_files = true;
          بشکن;

        حالت FILTER_OPTION:
          filter_command = optarg;
          بشکن;

        حالت IO_BLKSIZE_OPTION:
          in_blk_size = xnumtoumax (optarg, 10, 1,
                                    MIN (SYS_BUFSIZE_MAX,
                                         MIN (IDX_MAX, SIZE_MAX) - 1),
                                    multipliers, _("invalid IO block size"),
                                    0, XTOINT_MIN_RANGE);
          بشکن;

        حالت VERBOSE_OPTION:
          verbose = true;
          بشکن;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        پیشفرض:
          usage (EXIT_FAILURE);
        }
    }

  اگر (k_units != 0 && filter_command)
    {
      error (0, 0, _("--filter does not process a chunk extracted to "
                     "standard output"));
      usage (EXIT_FAILURE);
    }

  /* Handle default case.  */
  اگر (split_type == type_undef)
    {
      split_type = type_سطرها;
      n_units = 1000;
    }

  اگر (n_units == 0)
    {
      error (0, 0, _("invalid number of lines: %s"), quote ("0"));
      usage (EXIT_FAILURE);
    }

  اگر (eolchar < 0)
    eolchar = '\n';

  set_suffix_length (n_units, split_type);

  /* Get out the filename arguments.  */

  اگر (optind < argc)
    infile = argv[optind++];

  اگر (optind < argc)
    outbase = argv[optind++];

  اگر (optind < argc)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind]));
      usage (EXIT_FAILURE);
    }

  /* Check that the suffix length is large enough for the numerical
     suffix start value.  */
  اگر (numeric_suffix_start && strlen (numeric_suffix_start) > suffix_length)
    {
      error (0, 0, _("numerical suffix start value is too large "
                     "for the suffix length"));
      usage (EXIT_FAILURE);
    }

  /* Open the input file.  */
  اگر (! streq (infile, "-")
      && fd_reopen (STDIN_FILENO, infile, O_RDONLY, 0) < 0)
    error (EXIT_FAILURE, errno, _("cannot open %s for reading"),
           quoteaf (infile));

  /* Binary I/O is safer when byte counts are used.  */
  xset_دودویی_mode (STDIN_FILENO, O_دودویی);

  /* Advise the kernel of our access pattern.  */
  fdadvise (STDIN_FILENO, 0, 0, FADVISE_SEQUENTIAL);

  /* Get the optimal block size of input device and make a buffer.  */

  اگر (fstat (STDIN_FILENO, &in_stat_buf) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (infile));

  اگر (in_blk_size == 0)
    {
      in_blk_size = io_blksize (&in_stat_buf);
      اگر (SYS_BUFSIZE_MAX < in_blk_size)
        in_blk_size = SYS_BUFSIZE_MAX;
    }

  نویسه *buf = xalignalloc (page_size, in_blk_size + 1);
  ssize_t initial_read = -1;

  اگر (split_type == type_chunk_bytes || split_type == type_chunk_سطرها)
    {
      file_size = درون_داد_file_size (STDIN_FILENO, &in_stat_buf,
                                   buf, in_blk_size);
      اگر (file_size < 0)
        error (EXIT_FAILURE, errno, _("%s: cannot determine file size"),
               quotef (infile));
      initial_read = MIN (file_size, in_blk_size);
    }

  /* When filtering, closure of one pipe must not terminate the process,
     as there may still be other streams expecting input from us.  */
  اگر (filter_command)
    default_SIGPIPE = سیگنال (SIGPIPE, SIG_IGN) == SIG_DFL;

  انتخاب (split_type)
    {
    حالت type_digits:
    حالت type_سطرها:
      سطرها_split (n_units, buf, in_blk_size);
      بشکن;

    حالت type_bytes:
      bytes_split (n_units, 0, buf, in_blk_size, -1, 0);
      بشکن;

    حالت type_byteslines:
      سطر_bytes_split (n_units, buf, in_blk_size);
      بشکن;

    حالت type_chunk_bytes:
      اگر (k_units == 0)
        bytes_split (file_size / n_units, file_size % n_units,
                     buf, in_blk_size, initial_read, n_units);
      وگرنه
        bytes_chunk_extract (k_units, n_units, buf, in_blk_size, initial_read,
                             file_size);
      بشکن;

    حالت type_chunk_سطرها:
      سطرها_chunk_split (k_units, n_units, buf, in_blk_size, initial_read,
                         file_size);
      بشکن;

    حالت type_rr:
      /* Note, this is like 'sed -n ${k}~${n}p' when k > 0,
         but the functionality is provided for symmetry.  */
      {
        of_t *files;
        سطرها_rr (k_units, n_units, buf, in_blk_size, &files);
      }
      بشکن;

    حالت type_undef: پیشفرض:
      affirm (false);
    }

  اگر (close (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (infile));
  closeout (پوچ, output_desc, filter_pid, outfile);

  main_exit (EXIT_SUCCESS);
}
