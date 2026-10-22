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
#અન્તર્ભાવ <config.h>

#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <getopt.h>
#અન્તર્ભાવ <signal.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <sys/wait.h>
#અન્તર્ભાવ <spawn.h>

#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "alignalloc.h"
#અન્તર્ભાવ "assure.h"
#અન્તર્ભાવ "fadvise.h"
#અન્તર્ભાવ "fd-reopen.h"
#અન્તર્ભાવ "fcntl--.h"
#અન્તર્ભાવ "full-write.h"
#અન્તર્ભાવ "ioblksize.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "sig2str.h"
#અન્તર્ભાવ "sys-limits.h"
#અન્તર્ભાવ "temp-stream.h"
#અન્તર્ભાવ "unistd--.h"
#અન્તર્ભાવ "xbinary-io.h"
#અન્તર્ભાવ "xdectoint.h"
#અન્તર્ભાવ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "split"

#define AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("Richard M. Stallman")

/* Shell command to filter through, instead of creating files.  */
સ્થિર અક્ષર અચલ *filter_command;

/* Process ID of the filter.  */
સ્થિર pid_t filter_pid;

સ્થિર સંરચના
{
  /* The number of open pipes.  */
  પૂર્ણાંક n_pipes;

  /* The number of allocated pipes.  */
  idx_t n_pipes_alloc;

  /* Array of open pipes.  */
  પૂર્ણાંક *pipes COUNTED_BY (n_pipes);
} open_pipes;

/* Whether SIGPIPE has the default action, when --filter is used.  */
સ્થિર bool default_SIGPIPE;

/* Base name of output files.  */
સ્થિર અક્ષર અચલ *outbase = "x";

/* Name of output files.  */
સ્થિર અક્ષર *outfile;

/* Pointer to the end of the prefix in OUTFILE.
   Suffixes are inserted here.  */
સ્થિર અક્ષર *outfile_mid;

/* Generate new suffix when suffixes are exhausted.  */
સ્થિર bool suffix_auto = true;

/* Length of OUTFILE's suffix.  */
સ્થિર idx_t suffix_length;

/* Alphabet of characters to use in suffix.  */
સ્થિર અક્ષર અચલ *suffix_alphabet = "abcdefghijklmnopqrstuvwxyz";

/* Numerical suffix start value.  */
સ્થિર અક્ષર અચલ *numeric_suffix_start;

/* Additional suffix to append to output file names.  */
સ્થિર અક્ષર અચલ *additional_suffix;

/* Name of input file.  May be "-".  */
સ્થિર અક્ષર અચલ *infile = "-";

/* stat buf for input file.  */
સ્થિર સંરચના stat in_stat_buf;

/* Descriptor on which output file is open.  */
સ્થિર પૂર્ણાંક output_desc = -1;

/* If true, print a diagnostic on standard error just before each
   output file is opened. */
સ્થિર bool verbose;

/* If true, don't generate zero length output files. */
સ્થિર bool elide_empty_files;

/* If true, in round robin mode, immediately copy
   input to output, which is much slower, so disabled by default.  */
સ્થિર bool unbuffered;

/* The character marking end of line.  Defaults to \n below.  */
સ્થિર પૂર્ણાંક eolchar = -1;

/* The split mode to use.  */
ગણના Split_type
{
  type_undef, type_bytes, type_byteslines, type_લીટીઓ, type_digits,
  type_chunk_bytes, type_chunk_લીટીઓ, type_rr
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ગણના
{
  VERBOSE_OPTION = CHAR_MAX + 1,
  FILTER_OPTION,
  IO_BLKSIZE_OPTION,
  ADDITIONAL_SUFFIX_OPTION
};

સ્થિર સંરચના option અચલ longopts[] =
{
  {"bytes", required_argument, ખાલી, 'b'},
  {"lines", required_argument, ખાલી, 'l'},
  {"line-bytes", required_argument, ખાલી, 'C'},
  {"number", required_argument, ખાલી, 'n'},
  {"elide-empty-files", no_argument, ખાલી, 'e'},
  {"unbuffered", no_argument, ખાલી, 'u'},
  {"suffix-length", required_argument, ખાલી, 'a'},
  {"additional-suffix", required_argument, ખાલી,
   ADDITIONAL_SUFFIX_OPTION},
  {"numeric-suffixes", optional_argument, ખાલી, 'd'},
  {"hex-suffixes", optional_argument, ખાલી, 'x'},
  {"filter", required_argument, ખાલી, FILTER_OPTION},
  {"verbose", no_argument, ખાલી, VERBOSE_OPTION},
  {"separator", required_argument, ખાલી, 't'},
  {"-io-blksize", required_argument, ખાલી,
   IO_BLKSIZE_OPTION}, /* do not document */
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ખાલી, 0, ખાલી, 0}
};

/* Return true if the errno value, ERR, is ignorable.  */
સ્થિર અંતઃરેખ bool
ignorable (પૂર્ણાંક err)
{
  પાછા filter_command && err == EPIPE;
}

સ્થિર શૂન્ય
set_suffix_length (intmax_t n_units, ગણના Split_type split_type)
{
#define DEFAULT_SUFFIX_LENGTH 2

  પૂર્ણાંક suffix_length_needed = 0;

  /* The suffix auto length feature is incompatible with
     a user specified start value as the generated suffixes
     are not all consecutive.  */
  જો (numeric_suffix_start)
    suffix_auto = false;

  /* Auto-calculate the suffix length if the number of files is given.  */
  જો (split_type == type_chunk_bytes || split_type == type_chunk_લીટીઓ
      || split_type == type_rr)
    {
      intmax_t n_units_end = n_units - 1;
      જો (numeric_suffix_start)
        {
          intmax_t n_start;
          strtol_error e = xstrtoimax (numeric_suffix_start, ખાલી, 10,
                                       &n_start, "");
          જો (e == LONGINT_OK && n_start < n_units)
            {
              /* Restrict auto adjustment so we don't keep
                 incrementing a suffix size arbitrarily,
                 as that would break sort order for files
                 generated from multiple split runs.  */
              જો (ckd_add (&n_units_end, n_units_end, n_start))
                n_units_end = INTMAX_MAX;
            }

        }
      idx_t alphabet_len = strlen (suffix_alphabet);
      કરો
        suffix_length_needed++;
      જ્યારે (n_units_end /= alphabet_len);

      suffix_auto = false;
    }

  જો (suffix_length)            /* set by user */
    {
      જો (suffix_length < suffix_length_needed)
        error (EXIT_FAILURE, 0,
               _("the suffix length needs to be at least %d"),
               suffix_length_needed);
      suffix_auto = false;
      પાછા;
    }
  નહીંતર
    suffix_length = MAX (DEFAULT_SUFFIX_LENGTH, suffix_length_needed);
}

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
    {
      printf (_("\
Usage: %s [OPTION]... [FILE [PREFIX]]\n\
"),
              program_name);
      fputs (_("\
Output pieces of FILE to PREFIXaa, PREFIXab, ...;\n\
મૂળભૂત size is 1000 લીટીઓ, and મૂળભૂત PREFIX is 'x'.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oprintf (_("\
  -a, --suffix-length=N\n\
         generate suffixes of length N (મૂળભૂત %d)\n\
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
  -C, --લીટી-bytes=SIZE\n\
         put at most SIZE bytes of records per output file\n\
"));
      oputs (_("\
  -d\n\
         use numeric suffixes starting at 0, not alphabetic\n\
"));
      oputs (_("\
      --numeric-suffixes[=FROM]\n\
         same as -d, but allow setting the start કિંમત\n\
"));
      oputs (_("\
  -x\n\
         use hex suffixes starting at 0, not alphabetic\n\
"));
      oputs (_("\
      --hex-suffixes[=FROM]\n\
         same as -x, but allow setting the start કિંમત\n\
"));
      oputs (_("\
  -e, --elide-empty-files\n\
         કરો not generate empty output files with '-n'\n\
"));
      oputs (_("\
      --filter=COMMAND\n\
         લિખ to shell COMMAND; file name is $FILE\n\
"));
      oputs (_("\
  -l, --લીટીઓ=સંખ્યા_2\n\
         put સંખ્યા_2 લીટીઓ/records per output file\n\
"));
      oputs (_("\
  -n, --સંખ્યા=CHUNKS\n\
         generate CHUNKS output files; see explanation below\n\
"));
      oputs (_("\
  -t, --separator=SEP\n\
         use SEP instead of newline as the record separator;\n\
         '\\0' (zero) specifies the NUL character\n\
"));
      oputs (_("\
  -u, --unbuffered\n\
         immediately copy આગત to output with '-n r/...'\n\
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
  N       split into N files based on size of આગત\n\
  K/N     output Kth of N to standard output\n\
  l/N     split into N files without splitting લીટીઓ/records\n\
  l/K/N   output Kth of N to standard output without splitting લીટીઓ/records\n\
  r/N     like 'l' but use round robin distribution\n\
  r/K/N   likewise but only output Kth of N to standard output\n\
"), stdout);
      fputs (_("\n\
-n (except -nr) will buffer to $TMPDIR, defaulting to /tmp,\n\
જો the આગત size cannot easily be determined.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Copy the data in FD to a temporary file, then make that file FD.
   Use BUF, of size BUFSIZE, to copy.  Return the number of
   bytes copied, or -1 (setting errno) on error.  */
સ્થિર off_t
copy_to_tmpfile (પૂર્ણાંક fd, અક્ષર *buf, idx_t bufsize)
{
  FILE *tmp;
  જો (!temp_stream (&tmp, ખાલી))
    પાછા -1;
  off_t copied = 0;
  off_t r;

  જ્યારે (0 < (r = પઠ (fd, buf, bufsize)))
    {
      જો (fwrite (buf, 1, r, tmp) != r)
        પાછા -1;
      જો (ckd_add (&copied, copied, r))
        {
          errno = EOVERFLOW;
          પાછા -1;
        }
    }

  જો (r < 0)
    પાછા r;
  r = dup2 (fileno (tmp), fd);
  જો (r < 0)
    પાછા r;
  જો (fclose (tmp) < 0)
    પાછા -1;
  પાછા copied;
}

/* Return the number of bytes that can be read from FD with status ST.
   Store up to the first BUFSIZE bytes of the file's data into BUF,
   and advance the file position by the number of bytes read.  On
   input error, set errno and return -1.  */

સ્થિર off_t
આગત_file_size (પૂર્ણાંક fd, સંરચના stat અચલ *st, અક્ષર *buf, idx_t bufsize)
{
  off_t size = 0;
  કરો
    {
      ssize_t n_read = પઠ (fd, buf + size, bufsize - size);
      જો (n_read <= 0)
        પાછા n_read < 0 ? n_read : size;
      size += n_read;
    }
  જ્યારે (size < bufsize);

  off_t cur, end;
  જો ((usable_st_size (st) && st->st_size < size)
      || (cur = lseek (fd, 0, SEEK_CUR)) < 0
      || cur < size /* E.g., /dev/zero on GNU/Linux.  */
      || (end = lseek (fd, 0, SEEK_END)) < 0)
    {
      અક્ષર *tmpbuf = xmalloc (bufsize);
      end = copy_to_tmpfile (fd, tmpbuf, bufsize);
      મુક્ત (tmpbuf);
      જો (end < 0)
        પાછા end;
      cur = 0;
    }

  જો (end == OFF_T_MAX /* E.g., /dev/zero on GNU/Hurd.  */
      || (cur < end && ckd_add (&size, size, end - cur)))
    {
      errno = EOVERFLOW;
      પાછા -1;
    }

  જો (cur < end)
    {
      off_t r = lseek (fd, cur, SEEK_SET);
      જો (r < 0)
        પાછા r;
    }

  પાછા size;
}

/* Compute the next sequential output file name and store it into the
   string 'outfile'.  */

સ્થિર શૂન્ય
next_file_name (શૂન્ય)
{
  /* Index in suffix_alphabet of each character in the suffix.  */
  સ્થિર idx_t *sufindex;
  સ્થિર idx_t outbase_length;
  સ્થિર idx_t outfile_length;
  સ્થિર idx_t addsuf_length;

  જો (! outfile)
    {
      bool overflow, widen;

new_name:
      widen = !! outfile_length;

      જો (! widen)
        {
          /* Allocate and initialize the first file name.  */

          outbase_length = strlen (outbase);
          addsuf_length = additional_suffix ? strlen (additional_suffix) : 0;
          overflow = ckd_add (&outfile_length, outbase_length + addsuf_length,
                              suffix_length);
        }
      નહીંતર
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
      જો (overflow)
        xalloc_die ();
      outfile = xirealloc (outfile, outfile_size);

      જો (! widen)
        memcpy (outfile, outbase, outbase_length);
      નહીંતર
        {
          /* Append the last alphabet character to the file name prefix.  */
          outfile[outbase_length] = suffix_alphabet[sufindex[0]];
          outbase_length++;
        }

      outfile_mid = outfile + outbase_length;
      memset (outfile_mid, suffix_alphabet[0], suffix_length);
      જો (additional_suffix)
        memcpy (outfile_mid + suffix_length, additional_suffix, addsuf_length);
      outfile[outfile_length] = 0;

      મુક્ત (sufindex);
      sufindex = xicalloc (suffix_length, કદ *sufindex);

      જો (numeric_suffix_start)
        {
          affirm (! widen);

          /* Update the output file name.  */
          idx_t i = strlen (numeric_suffix_start);
          memcpy (outfile_mid + suffix_length - i, numeric_suffix_start, i);

          /* Update the suffix index.  */
          idx_t *sufindex_end = sufindex + suffix_length;
          જ્યારે (i-- != 0)
            *--sufindex_end = numeric_suffix_start[i] - '0';
        }

#જો ! _POSIX_NO_TRUNC && HAVE_PATHCONF && defined _PC_NAME_MAX
      /* POSIX requires that if the output file name is too long for
         its directory, 'split' must fail without creating any files.
         This must be checked for explicitly on operating systems that
         silently truncate file names.  */
      {
        અક્ષર *dir = dir_name (outfile);
        long name_max = pathconf (dir, _PC_NAME_MAX);
        જો (0 <= name_max && name_max < base_len (last_component (outfile)))
          error (EXIT_FAILURE, ENAMETOOLONG, "%s", quotef (outfile));
        મુક્ત (dir);
      }
#endif
    }
  નહીંતર
    {
      /* Increment the suffix in place, if possible.  */

      idx_t i = suffix_length;
      જ્યારે (i-- != 0)
        {
          sufindex[i]++;
          જો (suffix_auto && i == 0 && ! suffix_alphabet[sufindex[0] + 1])
            જાઓ new_name;
          outfile_mid[i] = suffix_alphabet[sufindex[i]];
          જો (outfile_mid[i])
            પાછા;
          sufindex[i] = 0;
          outfile_mid[i] = suffix_alphabet[sufindex[i]];
        }
      error (EXIT_FAILURE, 0, _("output file suffixes exhausted"));
    }
}

/* Create or truncate a file.  */

સ્થિર પૂર્ણાંક
create (અક્ષર અચલ *name)
{
  જો (!filter_command)
    {
      જો (verbose)
        fprintf (stdout, _("creating file %s\n"), quoteaf (name));

      પૂર્ણાંક oflags = O_WRONLY | O_CREAT | O_દ્વિઅંકી;
      પૂર્ણાંક fd = open (name, oflags | O_EXCL, MODE_RW_UGO);
      જો (0 <= fd || errno != EEXIST)
        પાછા fd;
      fd = open (name, oflags, MODE_RW_UGO);
      જો (fd < 0)
        પાછા fd;
      સંરચના stat out_stat_buf;
      જો (fstat (fd, &out_stat_buf) != 0)
        error (EXIT_FAILURE, errno, _("failed to stat %s"), quoteaf (name));
      જો (psame_inode (&in_stat_buf, &out_stat_buf))
        error (EXIT_FAILURE, 0, _("%s would overwrite input; aborting"),
               quoteaf (name));
      જો (ftruncate (fd, 0) < 0
          && (S_ISREG (out_stat_buf.st_mode) || S_TYPEISSHM (&out_stat_buf)))
        error (EXIT_FAILURE, errno, _("%s: error truncating"), quotef (name));

      પાછા fd;
    }
  નહીંતર
    {
      જો (setenv ("FILE", name, 1) != 0)
        error (EXIT_FAILURE, errno,
               _("failed to set FILE environment variable"));
      જો (verbose)
        fprintf (stdout, _("executing with FILE=%s\n"), quotef (name));

      પૂર્ણાંક result;
      પૂર્ણાંક fd_pair[2];
      pid_t child_pid;

      posix_spawnattr_t attr;
      posix_spawn_file_actions_t actions;

      sigset_t set;
      sigemptyset (&set);
      જો (default_SIGPIPE)
        sigaddset (&set, SIGPIPE);

      જો (   (result = posix_spawnattr_init (&attr))
          || (result = posix_spawnattr_setflags (&attr,
                                                 (POSIX_SPAWN_USEVFORK
                                                  | POSIX_SPAWN_SETSIGDEF)))
          || (result = posix_spawnattr_setsigdefault (&attr, &set))
          || (result = posix_spawn_file_actions_init (&actions))
         )
        error (EXIT_FAILURE, result, _("posix_spawn initialization failed"));

      જો (pipe (fd_pair) != 0)
        error (EXIT_FAILURE, errno, _("failed to create pipe"));

      /* We have to close any pipes that were opened during an
         earlier call, otherwise this process will be holding a
         write-pipe that will prevent the earlier process from
         reading an EOF on the corresponding read-pipe.  */
      માટે (પૂર્ણાંક i = 0; i < open_pipes.n_pipes; ++i)
        જો ((result = posix_spawn_file_actions_addclose (&actions,
                                                         open_pipes.pipes[i])))
          તોડો;

      જો (   result
          || (result = posix_spawn_file_actions_addclose (&actions, fd_pair[1]))
          || (fd_pair[0] != STDIN_FILENO
              && (   (result = posix_spawn_file_actions_adddup2 (&actions,
                                                                 fd_pair[0],
                                                                 STDIN_FILENO))
                  || (result = posix_spawn_file_actions_addclose (&actions,
                                                                  fd_pair[0]))))
         )
        error (EXIT_FAILURE, result, _("posix_spawn setup failed"));


      અક્ષર અચલ *shell_prog = getenv ("SHELL");
      જો (shell_prog == ખાલી)
        shell_prog = "/bin/sh";
      અક્ષર અચલ *અચલ argv[] = { last_component (shell_prog), "-c",
                                   filter_command, ખાલી };

      result = posix_spawn (&child_pid, shell_prog, &actions, &attr,
                            (અક્ષર * અચલ *) argv, environ);
      જો (result != 0)
        error (EXIT_FAILURE, errno, _("failed to run command: \"%s -c %s\""),
               shell_prog, filter_command);

      posix_spawnattr_destroy (&attr);
      posix_spawn_file_actions_destroy (&actions);

      જો (close (fd_pair[0]) != 0)
        error (EXIT_FAILURE, errno, _("failed to close input pipe"));
      filter_pid = child_pid;
      જો (open_pipes.n_pipes == open_pipes.n_pipes_alloc)
        open_pipes.pipes = xpalloc (open_pipes.pipes, &open_pipes.n_pipes_alloc,
                                    1, MIN (INT_MAX, IDX_MAX),
                                    કદ *open_pipes.pipes);
      idx_t અચલ pipe_idx = open_pipes.n_pipes++;
      open_pipes.pipes[pipe_idx] = fd_pair[1];
      પાછા fd_pair[1];
    }
}

/* Close the output file, and do any associated cleanup.
   If FP and FD are both specified, they refer to the same open file;
   in this case FP is closed, but FD is still used in cleanup.  */
સ્થિર શૂન્ય
closeout (FILE *fp, પૂર્ણાંક fd, pid_t pid, અક્ષર અચલ *name)
{
  જો (fp != ખાલી && fclose (fp) != 0 && ! ignorable (errno))
    error (EXIT_FAILURE, errno, "%s", quotef (name));
  જો (fd >= 0)
    {
      જો (fp == ખાલી && close (fd) < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (name));
      માટે (પૂર્ણાંક j = 0; j < open_pipes.n_pipes; ++j)
        {
          જો (open_pipes.pipes[j] == fd)
            {
              idx_t અચલ last_idx = open_pipes.n_pipes - 1;
              open_pipes.pipes[j] = open_pipes.pipes[last_idx];
              open_pipes.n_pipes--;
              તોડો;
            }
        }
    }
  જો (pid > 0)
    {
      પૂર્ણાંક wstatus;
      જો (waitpid (pid, &wstatus, 0) < 0)
        error (EXIT_FAILURE, errno, _("waiting for child process"));
      નહીંતર જો (WIFSIGNALED (wstatus))
        {
          પૂર્ણાંક sig = WTERMSIG (wstatus);
          જો (sig != SIGPIPE)
            {
              અક્ષર signame[MAX (SIG2STR_MAX, INT_BUFSIZE_BOUND (પૂર્ણાંક))];
              જો (sig2str (sig, signame) != 0)
                sprintf (signame, "%d", sig);
              error (sig + 128, 0,
                     _("with FILE=%s, signal %s from command: %s"),
                     quotef (name), signame, filter_command);
            }
        }
      નહીંતર જો (WIFEXITED (wstatus))
        {
          પૂર્ણાંક ex = WEXITSTATUS (wstatus);
          જો (ex != 0)
            error (ex, 0, _("with FILE=%s, exit %d from command: %s"),
                   quotef (name), ex, filter_command);
        }
      નહીંતર
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

સ્થિર bool
cwrite (bool new_file_flag, અક્ષર અચલ *bp, idx_t bytes)
{
  જો (new_file_flag)
    {
      જો (!bp && bytes == 0 && elide_empty_files)
        પાછા true;
      closeout (ખાલી, output_desc, filter_pid, outfile);
      next_file_name ();
      output_desc = create (outfile);
      જો (output_desc < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (outfile));
    }

  જો (full_write (output_desc, bp, bytes) == bytes)
    પાછા true;
  નહીંતર
    {
      જો (! ignorable (errno))
        error (EXIT_FAILURE, errno, "%s", quotef (outfile));
      પાછા false;
    }
}

/* Split into pieces of exactly N_BYTES bytes.
   However, the first REM_BYTES pieces should be 1 byte longer.
   Use buffer BUF, whose size is BUFSIZE.
   If INITIAL_READ is nonnegative,
   BUF contains the first INITIAL_READ input bytes.  */

સ્થિર શૂન્ય
bytes_split (intmax_t n_bytes, intmax_t rem_bytes,
             અક્ષર *buf, idx_t bufsize, ssize_t initial_read,
             intmax_t max_files)
{
  bool new_file_flag = true;
  bool filter_ok = true;
  intmax_t opened = 0;
  intmax_t to_write = n_bytes + (0 < rem_bytes);
  bool eof = ! to_write;

  જ્યારે (! eof)
    {
      ssize_t n_read;
      જો (0 <= initial_read)
        {
          n_read = initial_read;
          initial_read = -1;
          eof = n_read < bufsize;
        }
      નહીંતર
        {
          જો (! filter_ok
              && 0 <= lseek (STDIN_FILENO, to_write, SEEK_CUR))
            {
              to_write = n_bytes + (opened + 1 < rem_bytes);
              new_file_flag = true;
            }

          n_read = પઠ (STDIN_FILENO, buf, bufsize);
          જો (n_read < 0)
            error (EXIT_FAILURE, errno, "%s", quotef (infile));
          eof = n_read == 0;
        }
      અક્ષર *bp_out = buf;
      જ્યારે (0 < to_write && to_write <= n_read)
        {
          જો (filter_ok || new_file_flag)
            filter_ok = cwrite (new_file_flag, bp_out, to_write);
          opened += new_file_flag;
          new_file_flag = !max_files || (opened < max_files);
          જો (! filter_ok && ! new_file_flag)
            {
              /* If filters no longer accepting input, stop reading.  */
              n_read = 0;
              eof = true;
              તોડો;
            }
          bp_out += to_write;
          n_read -= to_write;
          to_write = n_bytes + (opened < rem_bytes);
        }
      જો (0 < n_read)
        {
          જો (filter_ok || new_file_flag)
            filter_ok = cwrite (new_file_flag, bp_out, n_read);
          opened += new_file_flag;
          new_file_flag = false;
          જો (! filter_ok && opened == max_files)
            {
              /* If filters no longer accepting input, stop reading.  */
              તોડો;
            }
          to_write -= n_read;
        }
    }

  /* Ensure NUMBER files are created, which truncates
     any existing files or notifies any consumers on fifos.
     FIXME: Should we do this before EXIT_FAILURE?  */
  જ્યારે (opened++ < max_files)
    cwrite (true, ખાલી, 0);
}

/* Split into pieces of exactly N_LINES lines.
   Use buffer BUF, whose size is BUFSIZE.  */

સ્થિર શૂન્ય
લીટીઓ_split (intmax_t n_લીટીઓ, અક્ષર *buf, idx_t bufsize)
{
  ssize_t n_read;
  અક્ષર *bp, *bp_out, *eob;
  bool new_file_flag = true;
  intmax_t n = 0;

  કરો
    {
      n_read = પઠ (STDIN_FILENO, buf, bufsize);
      જો (n_read < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
      bp = bp_out = buf;
      eob = bp + n_read;
      *eob = eolchar;
      જ્યારે (true)
        {
          bp = rawmemchr (bp, eolchar);
          જો (bp == eob)
            {
              જો (eob != bp_out) /* do not write 0 bytes! */
                {
                  idx_t len = eob - bp_out;
                  cwrite (new_file_flag, bp_out, len);
                  new_file_flag = false;
                }
              તોડો;
            }

          ++bp;
          જો (++n >= n_લીટીઓ)
            {
              cwrite (new_file_flag, bp_out, bp - bp_out);
              bp_out = bp;
              new_file_flag = true;
              n = 0;
            }
        }
    }
  જ્યારે (n_read);
}

/* Split into pieces that are as large as possible while still not more
   than N_BYTES bytes, and are split on line boundaries except
   where lines longer than N_BYTES bytes occur. */

સ્થિર શૂન્ય
લીટી_bytes_split (intmax_t n_bytes, અક્ષર *buf, idx_t bufsize)
{
  ssize_t n_read;
  intmax_t n_out = 0;      /* for each split.  */
  idx_t n_hold = 0;
  અક્ષર *hold = ખાલી;        /* for lines > bufsize.  */
  idx_t hold_size = 0;
  bool split_લીટી = false;  /* Whether a \n was output in a split.  */

  કરો
    {
      n_read = પઠ (STDIN_FILENO, buf, bufsize);
      જો (n_read < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
      idx_t n_ડાબું = n_read;
      અક્ષર *sob = buf;
      જ્યારે (n_ડાબું)
        {
          idx_t split_rest = 0;
          અક્ષર *eoc = ખાલી;
          અક્ષર *eol;

          /* Determine End Of Chunk and/or End of Line,
             which are used below to select what to write or buffer.  */
          જો (n_bytes - n_out - n_hold <= n_ડાબું)
            {
              /* Have enough for split.  */
              split_rest = n_bytes - n_out - n_hold;
              eoc = sob + split_rest - 1;
              eol = memrchr (sob, eolchar, split_rest);
            }
          નહીંતર
            eol = memrchr (sob, eolchar, n_ડાબું);

          /* Output hold space if possible.  */
          જો (n_hold && !(!eol && n_out))
            {
              cwrite (n_out == 0, hold, n_hold);
              n_out += n_hold;
              n_hold = 0;
            }

          /* Output to eol if present.  */
          જો (eol)
            {
              split_લીટી = true;
              idx_t n_write = eol - sob + 1;
              cwrite (n_out == 0, sob, n_write);
              n_out += n_write;
              n_ડાબું -= n_write;
              sob += n_write;
              જો (eoc)
                split_rest -= n_write;
            }

          /* Output to eoc or eob if possible.  */
          જો (n_ડાબું && !split_લીટી)
            {
              idx_t n_write = eoc ? split_rest : n_ડાબું;
              cwrite (n_out == 0, sob, n_write);
              n_out += n_write;
              n_ડાબું -= n_write;
              sob += n_write;
              જો (eoc)
                split_rest -= n_write;
            }

          /* Update hold if needed.  */
          જો ((eoc && split_rest) || (!eoc && n_ડાબું))
            {
              idx_t n_buf = eoc ? split_rest : n_ડાબું;
              જો (hold_size - n_hold < n_buf)
                hold = xpalloc (hold, &hold_size, n_buf - (hold_size - n_hold),
                                -1, કદ *hold);
              memcpy (hold + n_hold, sob, n_buf);
              n_hold += n_buf;
              n_ડાબું -= n_buf;
              sob += n_buf;
            }

          /* Reset for new split.  */
          જો (eoc)
            {
              n_out = 0;
              split_લીટી = false;
            }
        }
    }
  જ્યારે (n_read);

  /* Handle no eol at end of file.  */
  જો (n_hold)
    cwrite (n_out == 0, hold, n_hold);

  મુક્ત (hold);
}

/* -n l/[K/]N: Write lines to files of approximately file size / N.
   The file is partitioned into file size / N sized portions, with the
   last assigned any excess.  If a line _starts_ within a partition
   it is written completely to the corresponding file.  Since lines
   are not split even if they overlap a partition, the files written
   can be larger or smaller than the partition size, and even empty
   if a line is so long as to completely overlap the partition.  */

સ્થિર શૂન્ય
લીટીઓ_chunk_split (intmax_t k, intmax_t n, અક્ષર *buf, idx_t bufsize,
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

  જો (k > 1 && 0 < file_size)
    {
      /* Start reading 1 byte before kth chunk of file.  */
      off_t start = (k - 1) * chunk_size + MIN (k - 1, rem_bytes) - 1;
      જો (start < initial_read)
        {
          memmove (buf, buf + start, initial_read - start);
          initial_read -= start;
        }
      નહીંતર
        {
          જો (initial_read < start
              && lseek (STDIN_FILENO, start - initial_read, SEEK_CUR) < 0)
            error (EXIT_FAILURE, errno, "%s", quotef (infile));
          initial_read = -1;
        }
      n_written = start;
      chunk_no = k - 1;
      chunk_end = start + 1;
    }

  જ્યારે (n_written < file_size)
    {
      અક્ષર *bp = buf, *eob;
      ssize_t n_read;
      જો (0 <= initial_read)
        {
          n_read = initial_read;
          initial_read = -1;
        }
      નહીંતર
        {
          n_read = પઠ (STDIN_FILENO, buf,
                         MIN (bufsize, file_size - n_written));
          જો (n_read < 0)
            error (EXIT_FAILURE, errno, "%s", quotef (infile));
        }
      જો (n_read == 0)
        તોડો; /* eof.  */
      chunk_truncated = false;
      eob = buf + n_read;

      જ્યારે (bp != eob)
        {
          idx_t to_write;
          bool next = false;

          /* Begin looking for '\n' at last byte of chunk.  */
          off_t skip = MIN (n_read, MAX (0, chunk_end - 1 - n_written));
          અક્ષર *bp_out = memchr (bp + skip, eolchar, n_read - skip);
          જો (bp_out)
            {
              bp_out++;
              next = true;
            }
          નહીંતર
            bp_out = eob;
          to_write = bp_out - bp;

          જો (k == chunk_no)
            {
              /* We don't use the stdout buffer here since we're writing
                 large chunks from an existing file, so it's more efficient
                 to write out directly.  */
              જો (full_write (STDOUT_FILENO, bp, to_write) != to_write)
                write_error ();
            }
          નહીંતર જો (! k)
            cwrite (new_file_flag, bp, to_write);
          n_written += to_write;
          bp += to_write;
          n_read -= to_write;
          new_file_flag = next;

          /* A line could have been so long that it skipped
             entire chunks. So create empty files in that case.  */
          જ્યારે (next || chunk_end <= n_written)
            {
              જો (!next && bp == eob)
                {
                  /* replenish buf, before going to next chunk.  */
                  chunk_truncated = true;
                  તોડો;
                }
              જો (k == chunk_no)
                પાછા;
              chunk_end += chunk_size + (chunk_no < rem_bytes);
              chunk_no++;
              જો (chunk_end <= n_written)
                {
                  જો (! k)
                    cwrite (true, ખાલી, 0);
                }
              નહીંતર
                next = false;
            }
        }
    }

  જો (chunk_truncated)
    chunk_no++;

  /* Ensure NUMBER files are created, which truncates
     any existing files or notifies any consumers on fifos.
     FIXME: Should we do this before EXIT_FAILURE?  */
  જો (!k)
    જ્યારે (chunk_no++ <= n)
      cwrite (true, ખાલી, 0);
}

/* -n K/N: Extract Kth of N chunks.  */

સ્થિર શૂન્ય
bytes_chunk_extract (intmax_t k, intmax_t n, અક્ષર *buf, idx_t bufsize,
                     ssize_t initial_read, off_t file_size)
{
  off_t start;
  off_t end;

  affirm (0 < k && k <= n);

  start = (k - 1) * (file_size / n) + MIN (k - 1, file_size % n);
  end = k == n ? file_size : k * (file_size / n) + MIN (k, file_size % n);

  જો (start < initial_read)
    {
      memmove (buf, buf + start, initial_read - start);
      initial_read -= start;
    }
  નહીંતર
    {
      જો (initial_read < start
          && lseek (STDIN_FILENO, start - initial_read, SEEK_CUR) < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
      initial_read = -1;
    }

  જ્યારે (start < end)
    {
      ssize_t n_read;
      જો (0 <= initial_read)
        {
          n_read = initial_read;
          initial_read = -1;
        }
      નહીંતર
        {
          n_read = પઠ (STDIN_FILENO, buf, bufsize);
          જો (n_read < 0)
            error (EXIT_FAILURE, errno, "%s", quotef (infile));
        }
      જો (n_read == 0)
        તોડો; /* eof.  */
      n_read = MIN (n_read, end - start);
      જો (full_write (STDOUT_FILENO, buf, n_read) != n_read
          && ! ignorable (errno))
        error (EXIT_FAILURE, errno, "%s", quotef ("-"));
      start += n_read;
    }
}

પ્રકારવ્યાખ્યા સંરચના of_info
{
  અક્ષર *of_name;
  પૂર્ણાંક ofd;
  FILE *ofile;
  pid_t opid;
} of_t;

ગણના
{
  OFD_NEW = -1,
  OFD_APPEND = -2
};

/* Rotate file descriptors when we're writing to more output files than we
   have available file descriptors.
   Return whether we came under file resource pressure.
   If so, it's probably best to close each file when finished with it.  */

સ્થિર bool
ofile_open (of_t *files, idx_t i_check, idx_t nfiles)
{
  bool file_limit = false;

  જો (files[i_check].ofd <= OFD_NEW)
    {
      પૂર્ણાંક fd;
      idx_t i_reopen = i_check ? i_check - 1 : nfiles - 1;

      /* Another process could have opened a file in between the calls to
         close and open, so we should keep trying until open succeeds or
         we've closed all of our files.  */
      જ્યારે (true)
        {
          જો (files[i_check].ofd == OFD_NEW)
            fd = create (files[i_check].of_name);
          નહીંતર /* OFD_APPEND  */
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
                         O_WRONLY | O_દ્વિઅંકી | O_APPEND | O_NONBLOCK);
            }

          જો (0 <= fd)
            તોડો;

          જો (!(errno == EMFILE || errno == ENFILE))
            error (EXIT_FAILURE, errno, "%s", quotef (files[i_check].of_name));

          file_limit = true;

          /* Search backwards for an open file to close.  */
          જ્યારે (files[i_reopen].ofd < 0)
            {
              i_reopen = i_reopen ? i_reopen - 1 : nfiles - 1;
              /* No more open files to close, exit with E[NM]FILE.  */
              જો (i_reopen == i_check)
                error (EXIT_FAILURE, errno, "%s",
                       quotef (files[i_check].of_name));
            }

          જો (fclose (files[i_reopen].ofile) != 0)
            error (EXIT_FAILURE, errno, "%s", quotef (files[i_reopen].of_name));
          files[i_reopen].ofile = ખાલી;
          files[i_reopen].ofd = OFD_APPEND;
        }

      files[i_check].ofd = fd;
      FILE *ofile = fdopen (fd, "a");
      જો (!ofile)
        error (EXIT_FAILURE, errno, "%s", quotef (files[i_check].of_name));
      files[i_check].ofile = ofile;
      files[i_check].opid = filter_pid;
      filter_pid = 0;
    }

  પાછા file_limit;
}

/* -n r/[K/]N: Divide file into N chunks in round robin fashion.
   Use BUF of size BUFSIZE for the buffer, and if allocating storage
   put its address into *FILESP to pacify -fsanitize=leak.
   When K == 0, we try to keep the files open in parallel.
   If we run out of file resources, then we revert
   to opening and closing each file for each line.  */

સ્થિર શૂન્ય
લીટીઓ_rr (intmax_t k, intmax_t n, અક્ષર *buf, idx_t bufsize, of_t **filesp)
{
  bool wrapped = false;
  bool wrote = false;
  bool file_limit;
  idx_t i_file;
  of_t *files IF_LINT (= ખાલી);
  intmax_t લીટી_no;

  જો (k)
    લીટી_no = 1;
  નહીંતર
    {
      જો (IDX_MAX < n)
        xalloc_die ();
      files = *filesp = xinmalloc (n, કદ *files);

      /* Generate output file names. */
      માટે (i_file = 0; i_file < n; i_file++)
        {
          next_file_name ();
          files[i_file].of_name = xstrdup (outfile);
          files[i_file].ofd = OFD_NEW;
          files[i_file].ofile = ખાલી;
          files[i_file].opid = 0;
        }
      i_file = 0;
      file_limit = false;
    }

  જ્યારે (true)
    {
      અક્ષર *bp = buf, *eob;
      ssize_t n_read = પઠ (STDIN_FILENO, buf, bufsize);
      જો (n_read < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
      નહીંતર જો (n_read == 0)
        તોડો; /* eof.  */
      eob = buf + n_read;

      જ્યારે (bp != eob)
        {
          idx_t to_write;
          bool next = false;

          /* Find end of line. */
          અક્ષર *bp_out = memchr (bp, eolchar, eob - bp);
          જો (bp_out)
            {
              bp_out++;
              next = true;
            }
          નહીંતર
            bp_out = eob;
          to_write = bp_out - bp;

          જો (k)
            {
              જો (લીટી_no == k && unbuffered)
                {
                  જો (full_write (STDOUT_FILENO, bp, to_write) != to_write)
                    write_error ();
                }
              નહીંતર જો (લીટી_no == k && fwrite (bp, to_write, 1, stdout) != 1)
                {
                  write_error ();
                }
              જો (next)
                લીટી_no = (લીટી_no == n) ? 1 : લીટી_no + 1;
            }
          નહીંતર
            {
              /* Secure file descriptor. */
              file_limit |= ofile_open (files, i_file, n);
              જો (unbuffered)
                {
                  /* Note writing to fd, rather than flushing the FILE gives
                     an 8% performance benefit, due to reduced data copying.  */
                  જો (full_write (files[i_file].ofd, bp, to_write) != to_write
                      && ! ignorable (errno))
                    error (EXIT_FAILURE, errno, "%s",
                           quotef (files[i_file].of_name));
                }
              નહીંતર જો (fwrite (bp, to_write, 1, files[i_file].ofile) != 1
                       && ! ignorable (errno))
                error (EXIT_FAILURE, errno, "%s",
                       quotef (files[i_file].of_name));

              જો (! ignorable (errno))
                wrote = true;

              જો (file_limit)
                {
                  જો (fclose (files[i_file].ofile) != 0)
                    error (EXIT_FAILURE, errno, "%s",
                           quotef (files[i_file].of_name));
                  files[i_file].ofile = ખાલી;
                  files[i_file].ofd = OFD_APPEND;
                }
              જો (next && ++i_file == n)
                {
                  wrapped = true;
                  /* If no filters are accepting input, stop reading.  */
                  જો (! wrote)
                    જાઓ no_filters;
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
  જો (!k)
    {
      idx_t ceiling = wrapped ? n : i_file;
      માટે (i_file = 0; i_file < n; i_file++)
        {
          જો (i_file >= ceiling && !elide_empty_files)
            file_limit |= ofile_open (files, i_file, n);
          જો (files[i_file].ofd >= 0)
            closeout (files[i_file].ofile, files[i_file].ofd,
                      files[i_file].opid, files[i_file].of_name);
          files[i_file].ofd = OFD_APPEND;
        }
    }
}

#define FAIL_ONLY_ONE_WAY()					\
  કરો								\
    {								\
      error (0, 0, _("cannot split in more than one way"));	\
      usage (EXIT_FAILURE);					\
    }								\
  જ્યારે (0)

/* Report a string-to-integer conversion failure MSGID with ARG.  */

સ્થિર _Noreturn શૂન્ય
strtoint_die (અક્ષર અચલ *msgid, અક્ષર અચલ *arg)
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

સ્થિર intmax_t
parse_n_units (અક્ષર અચલ *arg, અક્ષર અચલ *multipliers, અક્ષર અચલ *msgid)
{
  intmax_t n;
  જો (OVERFLOW_OK < xstrtoimax (arg, ખાલી, 10, &n, multipliers) || n < 1)
    strtoint_die (msgid, arg);
  પાછા n;
}

/* Parse K/N syntax of chunk options.  */

સ્થિર શૂન્ય
parse_chunk (intmax_t *k_units, intmax_t *n_units, અક્ષર અચલ *arg)
{
  અક્ષર *argend;
  strtol_error e = xstrtoimax (arg, &argend, 10, n_units, "");
  જો (e == LONGINT_INVALID_SUFFIX_CHAR && *argend == '/')
    {
      *k_units = *n_units;
      *n_units = parse_n_units (argend + 1, "",
                                N_("invalid number of chunks"));
      જો (! (0 < *k_units && *k_units <= *n_units))
        error (EXIT_FAILURE, 0, "%s: %s", _("invalid chunk number"),
               quote_mem (arg, argend - arg));
    }
  નહીંતર જો (! (e <= OVERFLOW_OK && 0 < *n_units))
    strtoint_die (N_("invalid number of chunks"), arg);
}


પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  ગણના Split_type split_type = type_undef;
  idx_t in_blk_size = 0;	/* optimal block size of input file device */
  idx_t page_size = getpagesize ();
  intmax_t k_units = 0;
  intmax_t n_units = 0;

  સ્થિર અક્ષર અચલ multipliers[] = "bEGKkMmPQRTYZ0";
  પૂર્ણાંક c;
  પૂર્ણાંક digits_optind = 0;
  off_t file_size = OFF_T_MAX;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* Parse command line options.  */

  જ્યારે (true)
    {
      /* This is the argv-index of the option we will read next.  */
      પૂર્ણાંક this_optind = optind ? optind : 1;

      c = getopt_long (argc, argv, "0123456789C:a:b:del:n:t:ux",
                       longopts, ખાલી);
      જો (c == -1)
        તોડો;

      પસંદગી (c)
        {
        સ્થિતિ 'a':
          suffix_length = xdectoimax (optarg, 0, IDX_MAX,
                                      "", _("invalid suffix length"), 0);
          તોડો;

        સ્થિતિ ADDITIONAL_SUFFIX_OPTION:
          {
            પૂર્ણાંક suffix_len = strlen (optarg);
            જો (last_component (optarg) != optarg
                || (suffix_len && ISSLASH (optarg[suffix_len - 1])))
              {
                error (0, 0,
                       _("invalid suffix %s, contains directory separator"),
                       quote (optarg));
                usage (EXIT_FAILURE);
              }
          }
          additional_suffix = optarg;
          તોડો;

        સ્થિતિ 'b':
          જો (split_type != type_undef)
            FAIL_ONLY_ONE_WAY ();
          split_type = type_bytes;
          n_units = parse_n_units (optarg, multipliers,
                                   N_("invalid number of bytes"));
          તોડો;

        સ્થિતિ 'l':
          જો (split_type != type_undef)
            FAIL_ONLY_ONE_WAY ();
          split_type = type_લીટીઓ;
          n_units = parse_n_units (optarg, "", N_("invalid number of lines"));
          તોડો;

        સ્થિતિ 'C':
          જો (split_type != type_undef)
            FAIL_ONLY_ONE_WAY ();
          split_type = type_byteslines;
          n_units = parse_n_units (optarg, multipliers,
                                   N_("invalid number of lines"));
          તોડો;

        સ્થિતિ 'n':
          જો (split_type != type_undef)
            FAIL_ONLY_ONE_WAY ();
          /* skip any whitespace */
          જ્યારે (isspace (to_uchar (*optarg)))
            optarg++;
          જો (STRNCMP_LIT (optarg, "r/") == 0)
            {
              split_type = type_rr;
              optarg += 2;
            }
          નહીંતર જો (STRNCMP_LIT (optarg, "l/") == 0)
            {
              split_type = type_chunk_લીટીઓ;
              optarg += 2;
            }
          નહીંતર
            split_type = type_chunk_bytes;
          parse_chunk (&k_units, &n_units, optarg);
          તોડો;

        સ્થિતિ 'u':
          unbuffered = true;
          તોડો;

        સ્થિતિ 't':
          {
            અક્ષર neweol = optarg[0];
            જો (! neweol)
              error (EXIT_FAILURE, 0, _("empty record separator"));
            જો (optarg[1])
              {
                જો (streq (optarg, "\\0"))
                  neweol = '\0';
                નહીંતર
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
            જો (0 <= eolchar && neweol != eolchar)
              {
                error (EXIT_FAILURE, 0,
                       _("multiple separator characters specified"));
              }

            eolchar = neweol;
          }
          તોડો;

        સ્થિતિ '0':
        સ્થિતિ '1':
        સ્થિતિ '2':
        સ્થિતિ '3':
        સ્થિતિ '4':
        સ્થિતિ '5':
        સ્થિતિ '6':
        સ્થિતિ '7':
        સ્થિતિ '8':
        સ્થિતિ '9':
          જો (split_type == type_undef)
            {
              split_type = type_digits;
              n_units = 0;
            }
          જો (split_type != type_undef && split_type != type_digits)
            FAIL_ONLY_ONE_WAY ();
          જો (digits_optind != 0 && digits_optind != this_optind)
            n_units = 0;	/* More than one number given; ignore other. */
          digits_optind = this_optind;
          જો (ckd_mul (&n_units, n_units, 10)
              || ckd_add (&n_units, n_units, c - '0'))
            n_units = INTMAX_MAX;
          તોડો;

        સ્થિતિ 'd':
        સ્થિતિ 'x':
          જો (c == 'd')
            suffix_alphabet = "0123456789";
          નહીંતર
            suffix_alphabet = "0123456789abcdef";
          જો (optarg)
            {
              જો (strlen (optarg) != strspn (optarg, suffix_alphabet))
                {
                  error (0, 0,
                         (c == 'd') ?
                           _("%s: invalid start value for numerical suffix") :
                           _("%s: invalid start value for hexadecimal suffix"),
                         quote (optarg));
                  usage (EXIT_FAILURE);
                }
              નહીંતર
                {
                  /* Skip any leading zero.  */
                  જ્યારે (*optarg == '0' && *(optarg + 1) != '\0')
                    optarg++;
                  numeric_suffix_start = optarg;
                }
            }
          તોડો;

        સ્થિતિ 'e':
          elide_empty_files = true;
          તોડો;

        સ્થિતિ FILTER_OPTION:
          filter_command = optarg;
          તોડો;

        સ્થિતિ IO_BLKSIZE_OPTION:
          in_blk_size = xnumtoumax (optarg, 10, 1,
                                    MIN (SYS_BUFSIZE_MAX,
                                         MIN (IDX_MAX, SIZE_MAX) - 1),
                                    multipliers, _("invalid IO block size"),
                                    0, XTOINT_MIN_RANGE);
          તોડો;

        સ્થિતિ VERBOSE_OPTION:
          verbose = true;
          તોડો;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        મૂળભૂત:
          usage (EXIT_FAILURE);
        }
    }

  જો (k_units != 0 && filter_command)
    {
      error (0, 0, _("--filter does not process a chunk extracted to "
                     "standard output"));
      usage (EXIT_FAILURE);
    }

  /* Handle default case.  */
  જો (split_type == type_undef)
    {
      split_type = type_લીટીઓ;
      n_units = 1000;
    }

  જો (n_units == 0)
    {
      error (0, 0, _("invalid number of lines: %s"), quote ("0"));
      usage (EXIT_FAILURE);
    }

  જો (eolchar < 0)
    eolchar = '\n';

  set_suffix_length (n_units, split_type);

  /* Get out the filename arguments.  */

  જો (optind < argc)
    infile = argv[optind++];

  જો (optind < argc)
    outbase = argv[optind++];

  જો (optind < argc)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind]));
      usage (EXIT_FAILURE);
    }

  /* Check that the suffix length is large enough for the numerical
     suffix start value.  */
  જો (numeric_suffix_start && strlen (numeric_suffix_start) > suffix_length)
    {
      error (0, 0, _("numerical suffix start value is too large "
                     "for the suffix length"));
      usage (EXIT_FAILURE);
    }

  /* Open the input file.  */
  જો (! streq (infile, "-")
      && fd_reopen (STDIN_FILENO, infile, O_RDONLY, 0) < 0)
    error (EXIT_FAILURE, errno, _("cannot open %s for reading"),
           quoteaf (infile));

  /* Binary I/O is safer when byte counts are used.  */
  xset_દ્વિઅંકી_mode (STDIN_FILENO, O_દ્વિઅંકી);

  /* Advise the kernel of our access pattern.  */
  fdadvise (STDIN_FILENO, 0, 0, FADVISE_SEQUENTIAL);

  /* Get the optimal block size of input device and make a buffer.  */

  જો (fstat (STDIN_FILENO, &in_stat_buf) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (infile));

  જો (in_blk_size == 0)
    {
      in_blk_size = io_blksize (&in_stat_buf);
      જો (SYS_BUFSIZE_MAX < in_blk_size)
        in_blk_size = SYS_BUFSIZE_MAX;
    }

  અક્ષર *buf = xalignalloc (page_size, in_blk_size + 1);
  ssize_t initial_read = -1;

  જો (split_type == type_chunk_bytes || split_type == type_chunk_લીટીઓ)
    {
      file_size = આગત_file_size (STDIN_FILENO, &in_stat_buf,
                                   buf, in_blk_size);
      જો (file_size < 0)
        error (EXIT_FAILURE, errno, _("%s: cannot determine file size"),
               quotef (infile));
      initial_read = MIN (file_size, in_blk_size);
    }

  /* When filtering, closure of one pipe must not terminate the process,
     as there may still be other streams expecting input from us.  */
  જો (filter_command)
    default_SIGPIPE = સંકેત (SIGPIPE, SIG_IGN) == SIG_DFL;

  પસંદગી (split_type)
    {
    સ્થિતિ type_digits:
    સ્થિતિ type_લીટીઓ:
      લીટીઓ_split (n_units, buf, in_blk_size);
      તોડો;

    સ્થિતિ type_bytes:
      bytes_split (n_units, 0, buf, in_blk_size, -1, 0);
      તોડો;

    સ્થિતિ type_byteslines:
      લીટી_bytes_split (n_units, buf, in_blk_size);
      તોડો;

    સ્થિતિ type_chunk_bytes:
      જો (k_units == 0)
        bytes_split (file_size / n_units, file_size % n_units,
                     buf, in_blk_size, initial_read, n_units);
      નહીંતર
        bytes_chunk_extract (k_units, n_units, buf, in_blk_size, initial_read,
                             file_size);
      તોડો;

    સ્થિતિ type_chunk_લીટીઓ:
      લીટીઓ_chunk_split (k_units, n_units, buf, in_blk_size, initial_read,
                         file_size);
      તોડો;

    સ્થિતિ type_rr:
      /* Note, this is like 'sed -n ${k}~${n}p' when k > 0,
         but the functionality is provided for symmetry.  */
      {
        of_t *files;
        લીટીઓ_rr (k_units, n_units, buf, in_blk_size, &files);
      }
      તોડો;

    સ્થિતિ type_undef: મૂળભૂત:
      affirm (false);
    }

  જો (close (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (infile));
  closeout (ખાલી, output_desc, filter_pid, outfile);

  main_exit (EXIT_SUCCESS);
}
