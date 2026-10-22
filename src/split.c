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
#அந்தர்பாவ <config.h>

#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <getopt.h>
#அந்தர்பாவ <signal.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <sys/wait.h>
#அந்தர்பாவ <spawn.h>

#அந்தர்பாவ "system.h"
#அந்தர்பாவ "alignalloc.h"
#அந்தர்பாவ "assure.h"
#அந்தர்பாவ "fadvise.h"
#அந்தர்பாவ "fd-reopen.h"
#அந்தர்பாவ "fcntl--.h"
#அந்தர்பாவ "full-write.h"
#அந்தர்பாவ "ioblksize.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "sig2str.h"
#அந்தர்பாவ "sys-limits.h"
#அந்தர்பாவ "temp-stream.h"
#அந்தர்பாவ "unistd--.h"
#அந்தர்பாவ "xbinary-io.h"
#அந்தர்பாவ "xdectoint.h"
#அந்தர்பாவ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "split"

#define AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("Richard M. Stallman")

/* Shell command to filter through, instead of creating files.  */
நிலையான எழுத்து மாறிலி *filter_command;

/* Process ID of the filter.  */
நிலையான pid_t filter_pid;

நிலையான கட்டமைப்பு
{
  /* The number of open pipes.  */
  முழு n_pipes;

  /* The number of allocated pipes.  */
  idx_t n_pipes_alloc;

  /* Array of open pipes.  */
  முழு *pipes COUNTED_BY (n_pipes);
} open_pipes;

/* Whether SIGPIPE has the default action, when --filter is used.  */
நிலையான bool default_SIGPIPE;

/* Base name of output files.  */
நிலையான எழுத்து மாறிலி *outbase = "x";

/* Name of output files.  */
நிலையான எழுத்து *outfile;

/* Pointer to the end of the prefix in OUTFILE.
   Suffixes are inserted here.  */
நிலையான எழுத்து *outfile_mid;

/* Generate new suffix when suffixes are exhausted.  */
நிலையான bool suffix_auto = true;

/* Length of OUTFILE's suffix.  */
நிலையான idx_t suffix_length;

/* Alphabet of characters to use in suffix.  */
நிலையான எழுத்து மாறிலி *suffix_alphabet = "abcdefghijklmnopqrstuvwxyz";

/* Numerical suffix start value.  */
நிலையான எழுத்து மாறிலி *numeric_suffix_start;

/* Additional suffix to append to output file names.  */
நிலையான எழுத்து மாறிலி *additional_suffix;

/* Name of input file.  May be "-".  */
நிலையான எழுத்து மாறிலி *infile = "-";

/* stat buf for input file.  */
நிலையான கட்டமைப்பு stat in_stat_buf;

/* Descriptor on which output file is open.  */
நிலையான முழு output_desc = -1;

/* If true, print a diagnostic on standard error just before each
   output file is opened. */
நிலையான bool verbose;

/* If true, don't generate zero length output files. */
நிலையான bool elide_empty_files;

/* If true, in round robin mode, immediately copy
   input to output, which is much slower, so disabled by default.  */
நிலையான bool unbuffered;

/* The character marking end of line.  Defaults to \n below.  */
நிலையான முழு eolchar = -1;

/* The split mode to use.  */
எண்ணிக்கை Split_type
{
  type_undef, type_bytes, type_byteslines, type_வரிகள், type_digits,
  type_chunk_bytes, type_chunk_வரிகள், type_rr
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
எண்ணிக்கை
{
  VERBOSE_OPTION = CHAR_MAX + 1,
  FILTER_OPTION,
  IO_BLKSIZE_OPTION,
  ADDITIONAL_SUFFIX_OPTION
};

நிலையான கட்டமைப்பு option மாறிலி longopts[] =
{
  {"bytes", required_argument, வெற்று, 'b'},
  {"lines", required_argument, வெற்று, 'l'},
  {"line-bytes", required_argument, வெற்று, 'C'},
  {"number", required_argument, வெற்று, 'n'},
  {"elide-empty-files", no_argument, வெற்று, 'e'},
  {"unbuffered", no_argument, வெற்று, 'u'},
  {"suffix-length", required_argument, வெற்று, 'a'},
  {"additional-suffix", required_argument, வெற்று,
   ADDITIONAL_SUFFIX_OPTION},
  {"numeric-suffixes", optional_argument, வெற்று, 'd'},
  {"hex-suffixes", optional_argument, வெற்று, 'x'},
  {"filter", required_argument, வெற்று, FILTER_OPTION},
  {"verbose", no_argument, வெற்று, VERBOSE_OPTION},
  {"separator", required_argument, வெற்று, 't'},
  {"-io-blksize", required_argument, வெற்று,
   IO_BLKSIZE_OPTION}, /* do not document */
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {வெற்று, 0, வெற்று, 0}
};

/* Return true if the errno value, ERR, is ignorable.  */
நிலையான உள்வரி bool
ignorable (முழு err)
{
  திரும்பு filter_command && err == EPIPE;
}

நிலையான வெறுமை
set_suffix_length (intmax_t n_units, எண்ணிக்கை Split_type split_type)
{
#define DEFAULT_SUFFIX_LENGTH 2

  முழு suffix_length_needed = 0;

  /* The suffix auto length feature is incompatible with
     a user specified start value as the generated suffixes
     are not all consecutive.  */
  எனில் (numeric_suffix_start)
    suffix_auto = false;

  /* Auto-calculate the suffix length if the number of files is given.  */
  எனில் (split_type == type_chunk_bytes || split_type == type_chunk_வரிகள்
      || split_type == type_rr)
    {
      intmax_t n_units_end = n_units - 1;
      எனில் (numeric_suffix_start)
        {
          intmax_t n_start;
          strtol_error e = xstrtoimax (numeric_suffix_start, வெற்று, 10,
                                       &n_start, "");
          எனில் (e == LONGINT_OK && n_start < n_units)
            {
              /* Restrict auto adjustment so we don't keep
                 incrementing a suffix size arbitrarily,
                 as that would break sort order for files
                 generated from multiple split runs.  */
              எனில் (ckd_add (&n_units_end, n_units_end, n_start))
                n_units_end = INTMAX_MAX;
            }

        }
      idx_t alphabet_len = strlen (suffix_alphabet);
      செய்
        suffix_length_needed++;
      போது (n_units_end /= alphabet_len);

      suffix_auto = false;
    }

  எனில் (suffix_length)            /* set by user */
    {
      எனில் (suffix_length < suffix_length_needed)
        error (EXIT_FAILURE, 0,
               _("the suffix length needs to be at least %d"),
               suffix_length_needed);
      suffix_auto = false;
      திரும்பு;
    }
  இல்லையெனில்
    suffix_length = MAX (DEFAULT_SUFFIX_LENGTH, suffix_length_needed);
}

வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
    {
      printf (_("\
Usage: %s [OPTION]... [FILE [PREFIX]]\n\
"),
              program_name);
      fputs (_("\
Output pieces of FILE to PREFIXaa, PREFIXab, ...;\n\
இயல்பு size is 1000 வரிகள், and இயல்பு PREFIX is 'x'.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oprintf (_("\
  -a, --suffix-length=N\n\
         generate suffixes of length N (இயல்பு %d)\n\
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
  -C, --வரி-bytes=SIZE\n\
         put at most SIZE bytes of records per output file\n\
"));
      oputs (_("\
  -d\n\
         use numeric suffixes starting at 0, not alphabetic\n\
"));
      oputs (_("\
      --numeric-suffixes[=FROM]\n\
         same as -d, but allow setting the start மதிப்பு\n\
"));
      oputs (_("\
  -x\n\
         use hex suffixes starting at 0, not alphabetic\n\
"));
      oputs (_("\
      --hex-suffixes[=FROM]\n\
         same as -x, but allow setting the start மதிப்பு\n\
"));
      oputs (_("\
  -e, --elide-empty-files\n\
         செய் not generate empty output files with '-n'\n\
"));
      oputs (_("\
      --filter=COMMAND\n\
         லிக to shell COMMAND; file name is $FILE\n\
"));
      oputs (_("\
  -l, --வரிகள்=எண்_2\n\
         put எண்_2 வரிகள்/records per output file\n\
"));
      oputs (_("\
  -n, --எண்=CHUNKS\n\
         generate CHUNKS output files; see explanation below\n\
"));
      oputs (_("\
  -t, --separator=SEP\n\
         use SEP instead of newline as the record separator;\n\
         '\\0' (zero) specifies the NUL character\n\
"));
      oputs (_("\
  -u, --unbuffered\n\
         immediately copy உள்ளீடு to output with '-n r/...'\n\
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
  N       split into N files based on size of உள்ளீடு\n\
  K/N     output Kth of N to standard output\n\
  l/N     split into N files without splitting வரிகள்/records\n\
  l/K/N   output Kth of N to standard output without splitting வரிகள்/records\n\
  r/N     like 'l' but use round robin distribution\n\
  r/K/N   likewise but only output Kth of N to standard output\n\
"), stdout);
      fputs (_("\n\
-n (except -nr) will buffer to $TMPDIR, defaulting to /tmp,\n\
எனில் the உள்ளீடு size cannot easily be determined.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Copy the data in FD to a temporary file, then make that file FD.
   Use BUF, of size BUFSIZE, to copy.  Return the number of
   bytes copied, or -1 (setting errno) on error.  */
நிலையான off_t
copy_to_tmpfile (முழு fd, எழுத்து *buf, idx_t bufsize)
{
  FILE *tmp;
  எனில் (!temp_stream (&tmp, வெற்று))
    திரும்பு -1;
  off_t copied = 0;
  off_t r;

  போது (0 < (r = பட (fd, buf, bufsize)))
    {
      எனில் (fwrite (buf, 1, r, tmp) != r)
        திரும்பு -1;
      எனில் (ckd_add (&copied, copied, r))
        {
          errno = EOVERFLOW;
          திரும்பு -1;
        }
    }

  எனில் (r < 0)
    திரும்பு r;
  r = dup2 (fileno (tmp), fd);
  எனில் (r < 0)
    திரும்பு r;
  எனில் (fclose (tmp) < 0)
    திரும்பு -1;
  திரும்பு copied;
}

/* Return the number of bytes that can be read from FD with status ST.
   Store up to the first BUFSIZE bytes of the file's data into BUF,
   and advance the file position by the number of bytes read.  On
   input error, set errno and return -1.  */

நிலையான off_t
உள்ளீடு_file_size (முழு fd, கட்டமைப்பு stat மாறிலி *st, எழுத்து *buf, idx_t bufsize)
{
  off_t size = 0;
  செய்
    {
      ssize_t n_read = பட (fd, buf + size, bufsize - size);
      எனில் (n_read <= 0)
        திரும்பு n_read < 0 ? n_read : size;
      size += n_read;
    }
  போது (size < bufsize);

  off_t cur, end;
  எனில் ((usable_st_size (st) && st->st_size < size)
      || (cur = lseek (fd, 0, SEEK_CUR)) < 0
      || cur < size /* E.g., /dev/zero on GNU/Linux.  */
      || (end = lseek (fd, 0, SEEK_END)) < 0)
    {
      எழுத்து *tmpbuf = xmalloc (bufsize);
      end = copy_to_tmpfile (fd, tmpbuf, bufsize);
      விடுவி (tmpbuf);
      எனில் (end < 0)
        திரும்பு end;
      cur = 0;
    }

  எனில் (end == OFF_T_MAX /* E.g., /dev/zero on GNU/Hurd.  */
      || (cur < end && ckd_add (&size, size, end - cur)))
    {
      errno = EOVERFLOW;
      திரும்பு -1;
    }

  எனில் (cur < end)
    {
      off_t r = lseek (fd, cur, SEEK_SET);
      எனில் (r < 0)
        திரும்பு r;
    }

  திரும்பு size;
}

/* Compute the next sequential output file name and store it into the
   string 'outfile'.  */

நிலையான வெறுமை
next_file_name (வெறுமை)
{
  /* Index in suffix_alphabet of each character in the suffix.  */
  நிலையான idx_t *sufindex;
  நிலையான idx_t outbase_length;
  நிலையான idx_t outfile_length;
  நிலையான idx_t addsuf_length;

  எனில் (! outfile)
    {
      bool overflow, widen;

new_name:
      widen = !! outfile_length;

      எனில் (! widen)
        {
          /* Allocate and initialize the first file name.  */

          outbase_length = strlen (outbase);
          addsuf_length = additional_suffix ? strlen (additional_suffix) : 0;
          overflow = ckd_add (&outfile_length, outbase_length + addsuf_length,
                              suffix_length);
        }
      இல்லையெனில்
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
      எனில் (overflow)
        xalloc_die ();
      outfile = xirealloc (outfile, outfile_size);

      எனில் (! widen)
        memcpy (outfile, outbase, outbase_length);
      இல்லையெனில்
        {
          /* Append the last alphabet character to the file name prefix.  */
          outfile[outbase_length] = suffix_alphabet[sufindex[0]];
          outbase_length++;
        }

      outfile_mid = outfile + outbase_length;
      memset (outfile_mid, suffix_alphabet[0], suffix_length);
      எனில் (additional_suffix)
        memcpy (outfile_mid + suffix_length, additional_suffix, addsuf_length);
      outfile[outfile_length] = 0;

      விடுவி (sufindex);
      sufindex = xicalloc (suffix_length, அளவு *sufindex);

      எனில் (numeric_suffix_start)
        {
          affirm (! widen);

          /* Update the output file name.  */
          idx_t i = strlen (numeric_suffix_start);
          memcpy (outfile_mid + suffix_length - i, numeric_suffix_start, i);

          /* Update the suffix index.  */
          idx_t *sufindex_end = sufindex + suffix_length;
          போது (i-- != 0)
            *--sufindex_end = numeric_suffix_start[i] - '0';
        }

#எனில் ! _POSIX_NO_TRUNC && HAVE_PATHCONF && defined _PC_NAME_MAX
      /* POSIX requires that if the output file name is too long for
         its directory, 'split' must fail without creating any files.
         This must be checked for explicitly on operating systems that
         silently truncate file names.  */
      {
        எழுத்து *dir = dir_name (outfile);
        long name_max = pathconf (dir, _PC_NAME_MAX);
        எனில் (0 <= name_max && name_max < base_len (last_component (outfile)))
          error (EXIT_FAILURE, ENAMETOOLONG, "%s", quotef (outfile));
        விடுவி (dir);
      }
#endif
    }
  இல்லையெனில்
    {
      /* Increment the suffix in place, if possible.  */

      idx_t i = suffix_length;
      போது (i-- != 0)
        {
          sufindex[i]++;
          எனில் (suffix_auto && i == 0 && ! suffix_alphabet[sufindex[0] + 1])
            செல் new_name;
          outfile_mid[i] = suffix_alphabet[sufindex[i]];
          எனில் (outfile_mid[i])
            திரும்பு;
          sufindex[i] = 0;
          outfile_mid[i] = suffix_alphabet[sufindex[i]];
        }
      error (EXIT_FAILURE, 0, _("output file suffixes exhausted"));
    }
}

/* Create or truncate a file.  */

நிலையான முழு
create (எழுத்து மாறிலி *name)
{
  எனில் (!filter_command)
    {
      எனில் (verbose)
        fprintf (stdout, _("creating file %s\n"), quoteaf (name));

      முழு oflags = O_WRONLY | O_CREAT | O_இருமம்;
      முழு fd = open (name, oflags | O_EXCL, MODE_RW_UGO);
      எனில் (0 <= fd || errno != EEXIST)
        திரும்பு fd;
      fd = open (name, oflags, MODE_RW_UGO);
      எனில் (fd < 0)
        திரும்பு fd;
      கட்டமைப்பு stat out_stat_buf;
      எனில் (fstat (fd, &out_stat_buf) != 0)
        error (EXIT_FAILURE, errno, _("failed to stat %s"), quoteaf (name));
      எனில் (psame_inode (&in_stat_buf, &out_stat_buf))
        error (EXIT_FAILURE, 0, _("%s would overwrite input; aborting"),
               quoteaf (name));
      எனில் (ftruncate (fd, 0) < 0
          && (S_ISREG (out_stat_buf.st_mode) || S_TYPEISSHM (&out_stat_buf)))
        error (EXIT_FAILURE, errno, _("%s: error truncating"), quotef (name));

      திரும்பு fd;
    }
  இல்லையெனில்
    {
      எனில் (setenv ("FILE", name, 1) != 0)
        error (EXIT_FAILURE, errno,
               _("failed to set FILE environment variable"));
      எனில் (verbose)
        fprintf (stdout, _("executing with FILE=%s\n"), quotef (name));

      முழு result;
      முழு fd_pair[2];
      pid_t child_pid;

      posix_spawnattr_t attr;
      posix_spawn_file_actions_t actions;

      sigset_t set;
      sigemptyset (&set);
      எனில் (default_SIGPIPE)
        sigaddset (&set, SIGPIPE);

      எனில் (   (result = posix_spawnattr_init (&attr))
          || (result = posix_spawnattr_setflags (&attr,
                                                 (POSIX_SPAWN_USEVFORK
                                                  | POSIX_SPAWN_SETSIGDEF)))
          || (result = posix_spawnattr_setsigdefault (&attr, &set))
          || (result = posix_spawn_file_actions_init (&actions))
         )
        error (EXIT_FAILURE, result, _("posix_spawn initialization failed"));

      எனில் (pipe (fd_pair) != 0)
        error (EXIT_FAILURE, errno, _("failed to create pipe"));

      /* We have to close any pipes that were opened during an
         earlier call, otherwise this process will be holding a
         write-pipe that will prevent the earlier process from
         reading an EOF on the corresponding read-pipe.  */
      க்கு (முழு i = 0; i < open_pipes.n_pipes; ++i)
        எனில் ((result = posix_spawn_file_actions_addclose (&actions,
                                                         open_pipes.pipes[i])))
          நிறுத்து;

      எனில் (   result
          || (result = posix_spawn_file_actions_addclose (&actions, fd_pair[1]))
          || (fd_pair[0] != STDIN_FILENO
              && (   (result = posix_spawn_file_actions_adddup2 (&actions,
                                                                 fd_pair[0],
                                                                 STDIN_FILENO))
                  || (result = posix_spawn_file_actions_addclose (&actions,
                                                                  fd_pair[0]))))
         )
        error (EXIT_FAILURE, result, _("posix_spawn setup failed"));


      எழுத்து மாறிலி *shell_prog = getenv ("SHELL");
      எனில் (shell_prog == வெற்று)
        shell_prog = "/bin/sh";
      எழுத்து மாறிலி *மாறிலி argv[] = { last_component (shell_prog), "-c",
                                   filter_command, வெற்று };

      result = posix_spawn (&child_pid, shell_prog, &actions, &attr,
                            (எழுத்து * மாறிலி *) argv, environ);
      எனில் (result != 0)
        error (EXIT_FAILURE, errno, _("failed to run command: \"%s -c %s\""),
               shell_prog, filter_command);

      posix_spawnattr_destroy (&attr);
      posix_spawn_file_actions_destroy (&actions);

      எனில் (close (fd_pair[0]) != 0)
        error (EXIT_FAILURE, errno, _("failed to close input pipe"));
      filter_pid = child_pid;
      எனில் (open_pipes.n_pipes == open_pipes.n_pipes_alloc)
        open_pipes.pipes = xpalloc (open_pipes.pipes, &open_pipes.n_pipes_alloc,
                                    1, MIN (INT_MAX, IDX_MAX),
                                    அளவு *open_pipes.pipes);
      idx_t மாறிலி pipe_idx = open_pipes.n_pipes++;
      open_pipes.pipes[pipe_idx] = fd_pair[1];
      திரும்பு fd_pair[1];
    }
}

/* Close the output file, and do any associated cleanup.
   If FP and FD are both specified, they refer to the same open file;
   in this case FP is closed, but FD is still used in cleanup.  */
நிலையான வெறுமை
closeout (FILE *fp, முழு fd, pid_t pid, எழுத்து மாறிலி *name)
{
  எனில் (fp != வெற்று && fclose (fp) != 0 && ! ignorable (errno))
    error (EXIT_FAILURE, errno, "%s", quotef (name));
  எனில் (fd >= 0)
    {
      எனில் (fp == வெற்று && close (fd) < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (name));
      க்கு (முழு j = 0; j < open_pipes.n_pipes; ++j)
        {
          எனில் (open_pipes.pipes[j] == fd)
            {
              idx_t மாறிலி last_idx = open_pipes.n_pipes - 1;
              open_pipes.pipes[j] = open_pipes.pipes[last_idx];
              open_pipes.n_pipes--;
              நிறுத்து;
            }
        }
    }
  எனில் (pid > 0)
    {
      முழு wstatus;
      எனில் (waitpid (pid, &wstatus, 0) < 0)
        error (EXIT_FAILURE, errno, _("waiting for child process"));
      இல்லையெனில் எனில் (WIFSIGNALED (wstatus))
        {
          முழு sig = WTERMSIG (wstatus);
          எனில் (sig != SIGPIPE)
            {
              எழுத்து signame[MAX (SIG2STR_MAX, INT_BUFSIZE_BOUND (முழு))];
              எனில் (sig2str (sig, signame) != 0)
                sprintf (signame, "%d", sig);
              error (sig + 128, 0,
                     _("with FILE=%s, signal %s from command: %s"),
                     quotef (name), signame, filter_command);
            }
        }
      இல்லையெனில் எனில் (WIFEXITED (wstatus))
        {
          முழு ex = WEXITSTATUS (wstatus);
          எனில் (ex != 0)
            error (ex, 0, _("with FILE=%s, exit %d from command: %s"),
                   quotef (name), ex, filter_command);
        }
      இல்லையெனில்
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

நிலையான bool
cwrite (bool new_file_flag, எழுத்து மாறிலி *bp, idx_t bytes)
{
  எனில் (new_file_flag)
    {
      எனில் (!bp && bytes == 0 && elide_empty_files)
        திரும்பு true;
      closeout (வெற்று, output_desc, filter_pid, outfile);
      next_file_name ();
      output_desc = create (outfile);
      எனில் (output_desc < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (outfile));
    }

  எனில் (full_write (output_desc, bp, bytes) == bytes)
    திரும்பு true;
  இல்லையெனில்
    {
      எனில் (! ignorable (errno))
        error (EXIT_FAILURE, errno, "%s", quotef (outfile));
      திரும்பு false;
    }
}

/* Split into pieces of exactly N_BYTES bytes.
   However, the first REM_BYTES pieces should be 1 byte longer.
   Use buffer BUF, whose size is BUFSIZE.
   If INITIAL_READ is nonnegative,
   BUF contains the first INITIAL_READ input bytes.  */

நிலையான வெறுமை
bytes_split (intmax_t n_bytes, intmax_t rem_bytes,
             எழுத்து *buf, idx_t bufsize, ssize_t initial_read,
             intmax_t max_files)
{
  bool new_file_flag = true;
  bool filter_ok = true;
  intmax_t opened = 0;
  intmax_t to_write = n_bytes + (0 < rem_bytes);
  bool eof = ! to_write;

  போது (! eof)
    {
      ssize_t n_read;
      எனில் (0 <= initial_read)
        {
          n_read = initial_read;
          initial_read = -1;
          eof = n_read < bufsize;
        }
      இல்லையெனில்
        {
          எனில் (! filter_ok
              && 0 <= lseek (STDIN_FILENO, to_write, SEEK_CUR))
            {
              to_write = n_bytes + (opened + 1 < rem_bytes);
              new_file_flag = true;
            }

          n_read = பட (STDIN_FILENO, buf, bufsize);
          எனில் (n_read < 0)
            error (EXIT_FAILURE, errno, "%s", quotef (infile));
          eof = n_read == 0;
        }
      எழுத்து *bp_out = buf;
      போது (0 < to_write && to_write <= n_read)
        {
          எனில் (filter_ok || new_file_flag)
            filter_ok = cwrite (new_file_flag, bp_out, to_write);
          opened += new_file_flag;
          new_file_flag = !max_files || (opened < max_files);
          எனில் (! filter_ok && ! new_file_flag)
            {
              /* If filters no longer accepting input, stop reading.  */
              n_read = 0;
              eof = true;
              நிறுத்து;
            }
          bp_out += to_write;
          n_read -= to_write;
          to_write = n_bytes + (opened < rem_bytes);
        }
      எனில் (0 < n_read)
        {
          எனில் (filter_ok || new_file_flag)
            filter_ok = cwrite (new_file_flag, bp_out, n_read);
          opened += new_file_flag;
          new_file_flag = false;
          எனில் (! filter_ok && opened == max_files)
            {
              /* If filters no longer accepting input, stop reading.  */
              நிறுத்து;
            }
          to_write -= n_read;
        }
    }

  /* Ensure NUMBER files are created, which truncates
     any existing files or notifies any consumers on fifos.
     FIXME: Should we do this before EXIT_FAILURE?  */
  போது (opened++ < max_files)
    cwrite (true, வெற்று, 0);
}

/* Split into pieces of exactly N_LINES lines.
   Use buffer BUF, whose size is BUFSIZE.  */

நிலையான வெறுமை
வரிகள்_split (intmax_t n_வரிகள், எழுத்து *buf, idx_t bufsize)
{
  ssize_t n_read;
  எழுத்து *bp, *bp_out, *eob;
  bool new_file_flag = true;
  intmax_t n = 0;

  செய்
    {
      n_read = பட (STDIN_FILENO, buf, bufsize);
      எனில் (n_read < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
      bp = bp_out = buf;
      eob = bp + n_read;
      *eob = eolchar;
      போது (true)
        {
          bp = rawmemchr (bp, eolchar);
          எனில் (bp == eob)
            {
              எனில் (eob != bp_out) /* do not write 0 bytes! */
                {
                  idx_t len = eob - bp_out;
                  cwrite (new_file_flag, bp_out, len);
                  new_file_flag = false;
                }
              நிறுத்து;
            }

          ++bp;
          எனில் (++n >= n_வரிகள்)
            {
              cwrite (new_file_flag, bp_out, bp - bp_out);
              bp_out = bp;
              new_file_flag = true;
              n = 0;
            }
        }
    }
  போது (n_read);
}

/* Split into pieces that are as large as possible while still not more
   than N_BYTES bytes, and are split on line boundaries except
   where lines longer than N_BYTES bytes occur. */

நிலையான வெறுமை
வரி_bytes_split (intmax_t n_bytes, எழுத்து *buf, idx_t bufsize)
{
  ssize_t n_read;
  intmax_t n_out = 0;      /* for each split.  */
  idx_t n_hold = 0;
  எழுத்து *hold = வெற்று;        /* for lines > bufsize.  */
  idx_t hold_size = 0;
  bool split_வரி = false;  /* Whether a \n was output in a split.  */

  செய்
    {
      n_read = பட (STDIN_FILENO, buf, bufsize);
      எனில் (n_read < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
      idx_t n_இடது = n_read;
      எழுத்து *sob = buf;
      போது (n_இடது)
        {
          idx_t split_rest = 0;
          எழுத்து *eoc = வெற்று;
          எழுத்து *eol;

          /* Determine End Of Chunk and/or End of Line,
             which are used below to select what to write or buffer.  */
          எனில் (n_bytes - n_out - n_hold <= n_இடது)
            {
              /* Have enough for split.  */
              split_rest = n_bytes - n_out - n_hold;
              eoc = sob + split_rest - 1;
              eol = memrchr (sob, eolchar, split_rest);
            }
          இல்லையெனில்
            eol = memrchr (sob, eolchar, n_இடது);

          /* Output hold space if possible.  */
          எனில் (n_hold && !(!eol && n_out))
            {
              cwrite (n_out == 0, hold, n_hold);
              n_out += n_hold;
              n_hold = 0;
            }

          /* Output to eol if present.  */
          எனில் (eol)
            {
              split_வரி = true;
              idx_t n_write = eol - sob + 1;
              cwrite (n_out == 0, sob, n_write);
              n_out += n_write;
              n_இடது -= n_write;
              sob += n_write;
              எனில் (eoc)
                split_rest -= n_write;
            }

          /* Output to eoc or eob if possible.  */
          எனில் (n_இடது && !split_வரி)
            {
              idx_t n_write = eoc ? split_rest : n_இடது;
              cwrite (n_out == 0, sob, n_write);
              n_out += n_write;
              n_இடது -= n_write;
              sob += n_write;
              எனில் (eoc)
                split_rest -= n_write;
            }

          /* Update hold if needed.  */
          எனில் ((eoc && split_rest) || (!eoc && n_இடது))
            {
              idx_t n_buf = eoc ? split_rest : n_இடது;
              எனில் (hold_size - n_hold < n_buf)
                hold = xpalloc (hold, &hold_size, n_buf - (hold_size - n_hold),
                                -1, அளவு *hold);
              memcpy (hold + n_hold, sob, n_buf);
              n_hold += n_buf;
              n_இடது -= n_buf;
              sob += n_buf;
            }

          /* Reset for new split.  */
          எனில் (eoc)
            {
              n_out = 0;
              split_வரி = false;
            }
        }
    }
  போது (n_read);

  /* Handle no eol at end of file.  */
  எனில் (n_hold)
    cwrite (n_out == 0, hold, n_hold);

  விடுவி (hold);
}

/* -n l/[K/]N: Write lines to files of approximately file size / N.
   The file is partitioned into file size / N sized portions, with the
   last assigned any excess.  If a line _starts_ within a partition
   it is written completely to the corresponding file.  Since lines
   are not split even if they overlap a partition, the files written
   can be larger or smaller than the partition size, and even empty
   if a line is so long as to completely overlap the partition.  */

நிலையான வெறுமை
வரிகள்_chunk_split (intmax_t k, intmax_t n, எழுத்து *buf, idx_t bufsize,
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

  எனில் (k > 1 && 0 < file_size)
    {
      /* Start reading 1 byte before kth chunk of file.  */
      off_t start = (k - 1) * chunk_size + MIN (k - 1, rem_bytes) - 1;
      எனில் (start < initial_read)
        {
          memmove (buf, buf + start, initial_read - start);
          initial_read -= start;
        }
      இல்லையெனில்
        {
          எனில் (initial_read < start
              && lseek (STDIN_FILENO, start - initial_read, SEEK_CUR) < 0)
            error (EXIT_FAILURE, errno, "%s", quotef (infile));
          initial_read = -1;
        }
      n_written = start;
      chunk_no = k - 1;
      chunk_end = start + 1;
    }

  போது (n_written < file_size)
    {
      எழுத்து *bp = buf, *eob;
      ssize_t n_read;
      எனில் (0 <= initial_read)
        {
          n_read = initial_read;
          initial_read = -1;
        }
      இல்லையெனில்
        {
          n_read = பட (STDIN_FILENO, buf,
                         MIN (bufsize, file_size - n_written));
          எனில் (n_read < 0)
            error (EXIT_FAILURE, errno, "%s", quotef (infile));
        }
      எனில் (n_read == 0)
        நிறுத்து; /* eof.  */
      chunk_truncated = false;
      eob = buf + n_read;

      போது (bp != eob)
        {
          idx_t to_write;
          bool next = false;

          /* Begin looking for '\n' at last byte of chunk.  */
          off_t skip = MIN (n_read, MAX (0, chunk_end - 1 - n_written));
          எழுத்து *bp_out = memchr (bp + skip, eolchar, n_read - skip);
          எனில் (bp_out)
            {
              bp_out++;
              next = true;
            }
          இல்லையெனில்
            bp_out = eob;
          to_write = bp_out - bp;

          எனில் (k == chunk_no)
            {
              /* We don't use the stdout buffer here since we're writing
                 large chunks from an existing file, so it's more efficient
                 to write out directly.  */
              எனில் (full_write (STDOUT_FILENO, bp, to_write) != to_write)
                write_error ();
            }
          இல்லையெனில் எனில் (! k)
            cwrite (new_file_flag, bp, to_write);
          n_written += to_write;
          bp += to_write;
          n_read -= to_write;
          new_file_flag = next;

          /* A line could have been so long that it skipped
             entire chunks. So create empty files in that case.  */
          போது (next || chunk_end <= n_written)
            {
              எனில் (!next && bp == eob)
                {
                  /* replenish buf, before going to next chunk.  */
                  chunk_truncated = true;
                  நிறுத்து;
                }
              எனில் (k == chunk_no)
                திரும்பு;
              chunk_end += chunk_size + (chunk_no < rem_bytes);
              chunk_no++;
              எனில் (chunk_end <= n_written)
                {
                  எனில் (! k)
                    cwrite (true, வெற்று, 0);
                }
              இல்லையெனில்
                next = false;
            }
        }
    }

  எனில் (chunk_truncated)
    chunk_no++;

  /* Ensure NUMBER files are created, which truncates
     any existing files or notifies any consumers on fifos.
     FIXME: Should we do this before EXIT_FAILURE?  */
  எனில் (!k)
    போது (chunk_no++ <= n)
      cwrite (true, வெற்று, 0);
}

/* -n K/N: Extract Kth of N chunks.  */

நிலையான வெறுமை
bytes_chunk_extract (intmax_t k, intmax_t n, எழுத்து *buf, idx_t bufsize,
                     ssize_t initial_read, off_t file_size)
{
  off_t start;
  off_t end;

  affirm (0 < k && k <= n);

  start = (k - 1) * (file_size / n) + MIN (k - 1, file_size % n);
  end = k == n ? file_size : k * (file_size / n) + MIN (k, file_size % n);

  எனில் (start < initial_read)
    {
      memmove (buf, buf + start, initial_read - start);
      initial_read -= start;
    }
  இல்லையெனில்
    {
      எனில் (initial_read < start
          && lseek (STDIN_FILENO, start - initial_read, SEEK_CUR) < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
      initial_read = -1;
    }

  போது (start < end)
    {
      ssize_t n_read;
      எனில் (0 <= initial_read)
        {
          n_read = initial_read;
          initial_read = -1;
        }
      இல்லையெனில்
        {
          n_read = பட (STDIN_FILENO, buf, bufsize);
          எனில் (n_read < 0)
            error (EXIT_FAILURE, errno, "%s", quotef (infile));
        }
      எனில் (n_read == 0)
        நிறுத்து; /* eof.  */
      n_read = MIN (n_read, end - start);
      எனில் (full_write (STDOUT_FILENO, buf, n_read) != n_read
          && ! ignorable (errno))
        error (EXIT_FAILURE, errno, "%s", quotef ("-"));
      start += n_read;
    }
}

வகைவரை கட்டமைப்பு of_info
{
  எழுத்து *of_name;
  முழு ofd;
  FILE *ofile;
  pid_t opid;
} of_t;

எண்ணிக்கை
{
  OFD_NEW = -1,
  OFD_APPEND = -2
};

/* Rotate file descriptors when we're writing to more output files than we
   have available file descriptors.
   Return whether we came under file resource pressure.
   If so, it's probably best to close each file when finished with it.  */

நிலையான bool
ofile_open (of_t *files, idx_t i_check, idx_t nfiles)
{
  bool file_limit = false;

  எனில் (files[i_check].ofd <= OFD_NEW)
    {
      முழு fd;
      idx_t i_reopen = i_check ? i_check - 1 : nfiles - 1;

      /* Another process could have opened a file in between the calls to
         close and open, so we should keep trying until open succeeds or
         we've closed all of our files.  */
      போது (true)
        {
          எனில் (files[i_check].ofd == OFD_NEW)
            fd = create (files[i_check].of_name);
          இல்லையெனில் /* OFD_APPEND  */
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
                         O_WRONLY | O_இருமம் | O_APPEND | O_NONBLOCK);
            }

          எனில் (0 <= fd)
            நிறுத்து;

          எனில் (!(errno == EMFILE || errno == ENFILE))
            error (EXIT_FAILURE, errno, "%s", quotef (files[i_check].of_name));

          file_limit = true;

          /* Search backwards for an open file to close.  */
          போது (files[i_reopen].ofd < 0)
            {
              i_reopen = i_reopen ? i_reopen - 1 : nfiles - 1;
              /* No more open files to close, exit with E[NM]FILE.  */
              எனில் (i_reopen == i_check)
                error (EXIT_FAILURE, errno, "%s",
                       quotef (files[i_check].of_name));
            }

          எனில் (fclose (files[i_reopen].ofile) != 0)
            error (EXIT_FAILURE, errno, "%s", quotef (files[i_reopen].of_name));
          files[i_reopen].ofile = வெற்று;
          files[i_reopen].ofd = OFD_APPEND;
        }

      files[i_check].ofd = fd;
      FILE *ofile = fdopen (fd, "a");
      எனில் (!ofile)
        error (EXIT_FAILURE, errno, "%s", quotef (files[i_check].of_name));
      files[i_check].ofile = ofile;
      files[i_check].opid = filter_pid;
      filter_pid = 0;
    }

  திரும்பு file_limit;
}

/* -n r/[K/]N: Divide file into N chunks in round robin fashion.
   Use BUF of size BUFSIZE for the buffer, and if allocating storage
   put its address into *FILESP to pacify -fsanitize=leak.
   When K == 0, we try to keep the files open in parallel.
   If we run out of file resources, then we revert
   to opening and closing each file for each line.  */

நிலையான வெறுமை
வரிகள்_rr (intmax_t k, intmax_t n, எழுத்து *buf, idx_t bufsize, of_t **filesp)
{
  bool wrapped = false;
  bool wrote = false;
  bool file_limit;
  idx_t i_file;
  of_t *files IF_LINT (= வெற்று);
  intmax_t வரி_no;

  எனில் (k)
    வரி_no = 1;
  இல்லையெனில்
    {
      எனில் (IDX_MAX < n)
        xalloc_die ();
      files = *filesp = xinmalloc (n, அளவு *files);

      /* Generate output file names. */
      க்கு (i_file = 0; i_file < n; i_file++)
        {
          next_file_name ();
          files[i_file].of_name = xstrdup (outfile);
          files[i_file].ofd = OFD_NEW;
          files[i_file].ofile = வெற்று;
          files[i_file].opid = 0;
        }
      i_file = 0;
      file_limit = false;
    }

  போது (true)
    {
      எழுத்து *bp = buf, *eob;
      ssize_t n_read = பட (STDIN_FILENO, buf, bufsize);
      எனில் (n_read < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
      இல்லையெனில் எனில் (n_read == 0)
        நிறுத்து; /* eof.  */
      eob = buf + n_read;

      போது (bp != eob)
        {
          idx_t to_write;
          bool next = false;

          /* Find end of line. */
          எழுத்து *bp_out = memchr (bp, eolchar, eob - bp);
          எனில் (bp_out)
            {
              bp_out++;
              next = true;
            }
          இல்லையெனில்
            bp_out = eob;
          to_write = bp_out - bp;

          எனில் (k)
            {
              எனில் (வரி_no == k && unbuffered)
                {
                  எனில் (full_write (STDOUT_FILENO, bp, to_write) != to_write)
                    write_error ();
                }
              இல்லையெனில் எனில் (வரி_no == k && fwrite (bp, to_write, 1, stdout) != 1)
                {
                  write_error ();
                }
              எனில் (next)
                வரி_no = (வரி_no == n) ? 1 : வரி_no + 1;
            }
          இல்லையெனில்
            {
              /* Secure file descriptor. */
              file_limit |= ofile_open (files, i_file, n);
              எனில் (unbuffered)
                {
                  /* Note writing to fd, rather than flushing the FILE gives
                     an 8% performance benefit, due to reduced data copying.  */
                  எனில் (full_write (files[i_file].ofd, bp, to_write) != to_write
                      && ! ignorable (errno))
                    error (EXIT_FAILURE, errno, "%s",
                           quotef (files[i_file].of_name));
                }
              இல்லையெனில் எனில் (fwrite (bp, to_write, 1, files[i_file].ofile) != 1
                       && ! ignorable (errno))
                error (EXIT_FAILURE, errno, "%s",
                       quotef (files[i_file].of_name));

              எனில் (! ignorable (errno))
                wrote = true;

              எனில் (file_limit)
                {
                  எனில் (fclose (files[i_file].ofile) != 0)
                    error (EXIT_FAILURE, errno, "%s",
                           quotef (files[i_file].of_name));
                  files[i_file].ofile = வெற்று;
                  files[i_file].ofd = OFD_APPEND;
                }
              எனில் (next && ++i_file == n)
                {
                  wrapped = true;
                  /* If no filters are accepting input, stop reading.  */
                  எனில் (! wrote)
                    செல் no_filters;
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
  எனில் (!k)
    {
      idx_t ceiling = wrapped ? n : i_file;
      க்கு (i_file = 0; i_file < n; i_file++)
        {
          எனில் (i_file >= ceiling && !elide_empty_files)
            file_limit |= ofile_open (files, i_file, n);
          எனில் (files[i_file].ofd >= 0)
            closeout (files[i_file].ofile, files[i_file].ofd,
                      files[i_file].opid, files[i_file].of_name);
          files[i_file].ofd = OFD_APPEND;
        }
    }
}

#define FAIL_ONLY_ONE_WAY()					\
  செய்								\
    {								\
      error (0, 0, _("cannot split in more than one way"));	\
      usage (EXIT_FAILURE);					\
    }								\
  போது (0)

/* Report a string-to-integer conversion failure MSGID with ARG.  */

நிலையான _Noreturn வெறுமை
strtoint_die (எழுத்து மாறிலி *msgid, எழுத்து மாறிலி *arg)
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

நிலையான intmax_t
parse_n_units (எழுத்து மாறிலி *arg, எழுத்து மாறிலி *multipliers, எழுத்து மாறிலி *msgid)
{
  intmax_t n;
  எனில் (OVERFLOW_OK < xstrtoimax (arg, வெற்று, 10, &n, multipliers) || n < 1)
    strtoint_die (msgid, arg);
  திரும்பு n;
}

/* Parse K/N syntax of chunk options.  */

நிலையான வெறுமை
parse_chunk (intmax_t *k_units, intmax_t *n_units, எழுத்து மாறிலி *arg)
{
  எழுத்து *argend;
  strtol_error e = xstrtoimax (arg, &argend, 10, n_units, "");
  எனில் (e == LONGINT_INVALID_SUFFIX_CHAR && *argend == '/')
    {
      *k_units = *n_units;
      *n_units = parse_n_units (argend + 1, "",
                                N_("invalid number of chunks"));
      எனில் (! (0 < *k_units && *k_units <= *n_units))
        error (EXIT_FAILURE, 0, "%s: %s", _("invalid chunk number"),
               quote_mem (arg, argend - arg));
    }
  இல்லையெனில் எனில் (! (e <= OVERFLOW_OK && 0 < *n_units))
    strtoint_die (N_("invalid number of chunks"), arg);
}


முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  எண்ணிக்கை Split_type split_type = type_undef;
  idx_t in_blk_size = 0;	/* optimal block size of input file device */
  idx_t page_size = getpagesize ();
  intmax_t k_units = 0;
  intmax_t n_units = 0;

  நிலையான எழுத்து மாறிலி multipliers[] = "bEGKkMmPQRTYZ0";
  முழு c;
  முழு digits_optind = 0;
  off_t file_size = OFF_T_MAX;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* Parse command line options.  */

  போது (true)
    {
      /* This is the argv-index of the option we will read next.  */
      முழு this_optind = optind ? optind : 1;

      c = getopt_long (argc, argv, "0123456789C:a:b:del:n:t:ux",
                       longopts, வெற்று);
      எனில் (c == -1)
        நிறுத்து;

      தேர்வு (c)
        {
        நிலை 'a':
          suffix_length = xdectoimax (optarg, 0, IDX_MAX,
                                      "", _("invalid suffix length"), 0);
          நிறுத்து;

        நிலை ADDITIONAL_SUFFIX_OPTION:
          {
            முழு suffix_len = strlen (optarg);
            எனில் (last_component (optarg) != optarg
                || (suffix_len && ISSLASH (optarg[suffix_len - 1])))
              {
                error (0, 0,
                       _("invalid suffix %s, contains directory separator"),
                       quote (optarg));
                usage (EXIT_FAILURE);
              }
          }
          additional_suffix = optarg;
          நிறுத்து;

        நிலை 'b':
          எனில் (split_type != type_undef)
            FAIL_ONLY_ONE_WAY ();
          split_type = type_bytes;
          n_units = parse_n_units (optarg, multipliers,
                                   N_("invalid number of bytes"));
          நிறுத்து;

        நிலை 'l':
          எனில் (split_type != type_undef)
            FAIL_ONLY_ONE_WAY ();
          split_type = type_வரிகள்;
          n_units = parse_n_units (optarg, "", N_("invalid number of lines"));
          நிறுத்து;

        நிலை 'C':
          எனில் (split_type != type_undef)
            FAIL_ONLY_ONE_WAY ();
          split_type = type_byteslines;
          n_units = parse_n_units (optarg, multipliers,
                                   N_("invalid number of lines"));
          நிறுத்து;

        நிலை 'n':
          எனில் (split_type != type_undef)
            FAIL_ONLY_ONE_WAY ();
          /* skip any whitespace */
          போது (isspace (to_uchar (*optarg)))
            optarg++;
          எனில் (STRNCMP_LIT (optarg, "r/") == 0)
            {
              split_type = type_rr;
              optarg += 2;
            }
          இல்லையெனில் எனில் (STRNCMP_LIT (optarg, "l/") == 0)
            {
              split_type = type_chunk_வரிகள்;
              optarg += 2;
            }
          இல்லையெனில்
            split_type = type_chunk_bytes;
          parse_chunk (&k_units, &n_units, optarg);
          நிறுத்து;

        நிலை 'u':
          unbuffered = true;
          நிறுத்து;

        நிலை 't':
          {
            எழுத்து neweol = optarg[0];
            எனில் (! neweol)
              error (EXIT_FAILURE, 0, _("empty record separator"));
            எனில் (optarg[1])
              {
                எனில் (streq (optarg, "\\0"))
                  neweol = '\0';
                இல்லையெனில்
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
            எனில் (0 <= eolchar && neweol != eolchar)
              {
                error (EXIT_FAILURE, 0,
                       _("multiple separator characters specified"));
              }

            eolchar = neweol;
          }
          நிறுத்து;

        நிலை '0':
        நிலை '1':
        நிலை '2':
        நிலை '3':
        நிலை '4':
        நிலை '5':
        நிலை '6':
        நிலை '7':
        நிலை '8':
        நிலை '9':
          எனில் (split_type == type_undef)
            {
              split_type = type_digits;
              n_units = 0;
            }
          எனில் (split_type != type_undef && split_type != type_digits)
            FAIL_ONLY_ONE_WAY ();
          எனில் (digits_optind != 0 && digits_optind != this_optind)
            n_units = 0;	/* More than one number given; ignore other. */
          digits_optind = this_optind;
          எனில் (ckd_mul (&n_units, n_units, 10)
              || ckd_add (&n_units, n_units, c - '0'))
            n_units = INTMAX_MAX;
          நிறுத்து;

        நிலை 'd':
        நிலை 'x':
          எனில் (c == 'd')
            suffix_alphabet = "0123456789";
          இல்லையெனில்
            suffix_alphabet = "0123456789abcdef";
          எனில் (optarg)
            {
              எனில் (strlen (optarg) != strspn (optarg, suffix_alphabet))
                {
                  error (0, 0,
                         (c == 'd') ?
                           _("%s: invalid start value for numerical suffix") :
                           _("%s: invalid start value for hexadecimal suffix"),
                         quote (optarg));
                  usage (EXIT_FAILURE);
                }
              இல்லையெனில்
                {
                  /* Skip any leading zero.  */
                  போது (*optarg == '0' && *(optarg + 1) != '\0')
                    optarg++;
                  numeric_suffix_start = optarg;
                }
            }
          நிறுத்து;

        நிலை 'e':
          elide_empty_files = true;
          நிறுத்து;

        நிலை FILTER_OPTION:
          filter_command = optarg;
          நிறுத்து;

        நிலை IO_BLKSIZE_OPTION:
          in_blk_size = xnumtoumax (optarg, 10, 1,
                                    MIN (SYS_BUFSIZE_MAX,
                                         MIN (IDX_MAX, SIZE_MAX) - 1),
                                    multipliers, _("invalid IO block size"),
                                    0, XTOINT_MIN_RANGE);
          நிறுத்து;

        நிலை VERBOSE_OPTION:
          verbose = true;
          நிறுத்து;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        இயல்பு:
          usage (EXIT_FAILURE);
        }
    }

  எனில் (k_units != 0 && filter_command)
    {
      error (0, 0, _("--filter does not process a chunk extracted to "
                     "standard output"));
      usage (EXIT_FAILURE);
    }

  /* Handle default case.  */
  எனில் (split_type == type_undef)
    {
      split_type = type_வரிகள்;
      n_units = 1000;
    }

  எனில் (n_units == 0)
    {
      error (0, 0, _("invalid number of lines: %s"), quote ("0"));
      usage (EXIT_FAILURE);
    }

  எனில் (eolchar < 0)
    eolchar = '\n';

  set_suffix_length (n_units, split_type);

  /* Get out the filename arguments.  */

  எனில் (optind < argc)
    infile = argv[optind++];

  எனில் (optind < argc)
    outbase = argv[optind++];

  எனில் (optind < argc)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind]));
      usage (EXIT_FAILURE);
    }

  /* Check that the suffix length is large enough for the numerical
     suffix start value.  */
  எனில் (numeric_suffix_start && strlen (numeric_suffix_start) > suffix_length)
    {
      error (0, 0, _("numerical suffix start value is too large "
                     "for the suffix length"));
      usage (EXIT_FAILURE);
    }

  /* Open the input file.  */
  எனில் (! streq (infile, "-")
      && fd_reopen (STDIN_FILENO, infile, O_RDONLY, 0) < 0)
    error (EXIT_FAILURE, errno, _("cannot open %s for reading"),
           quoteaf (infile));

  /* Binary I/O is safer when byte counts are used.  */
  xset_இருமம்_mode (STDIN_FILENO, O_இருமம்);

  /* Advise the kernel of our access pattern.  */
  fdadvise (STDIN_FILENO, 0, 0, FADVISE_SEQUENTIAL);

  /* Get the optimal block size of input device and make a buffer.  */

  எனில் (fstat (STDIN_FILENO, &in_stat_buf) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (infile));

  எனில் (in_blk_size == 0)
    {
      in_blk_size = io_blksize (&in_stat_buf);
      எனில் (SYS_BUFSIZE_MAX < in_blk_size)
        in_blk_size = SYS_BUFSIZE_MAX;
    }

  எழுத்து *buf = xalignalloc (page_size, in_blk_size + 1);
  ssize_t initial_read = -1;

  எனில் (split_type == type_chunk_bytes || split_type == type_chunk_வரிகள்)
    {
      file_size = உள்ளீடு_file_size (STDIN_FILENO, &in_stat_buf,
                                   buf, in_blk_size);
      எனில் (file_size < 0)
        error (EXIT_FAILURE, errno, _("%s: cannot determine file size"),
               quotef (infile));
      initial_read = MIN (file_size, in_blk_size);
    }

  /* When filtering, closure of one pipe must not terminate the process,
     as there may still be other streams expecting input from us.  */
  எனில் (filter_command)
    default_SIGPIPE = சமிக்ஞை (SIGPIPE, SIG_IGN) == SIG_DFL;

  தேர்வு (split_type)
    {
    நிலை type_digits:
    நிலை type_வரிகள்:
      வரிகள்_split (n_units, buf, in_blk_size);
      நிறுத்து;

    நிலை type_bytes:
      bytes_split (n_units, 0, buf, in_blk_size, -1, 0);
      நிறுத்து;

    நிலை type_byteslines:
      வரி_bytes_split (n_units, buf, in_blk_size);
      நிறுத்து;

    நிலை type_chunk_bytes:
      எனில் (k_units == 0)
        bytes_split (file_size / n_units, file_size % n_units,
                     buf, in_blk_size, initial_read, n_units);
      இல்லையெனில்
        bytes_chunk_extract (k_units, n_units, buf, in_blk_size, initial_read,
                             file_size);
      நிறுத்து;

    நிலை type_chunk_வரிகள்:
      வரிகள்_chunk_split (k_units, n_units, buf, in_blk_size, initial_read,
                         file_size);
      நிறுத்து;

    நிலை type_rr:
      /* Note, this is like 'sed -n ${k}~${n}p' when k > 0,
         but the functionality is provided for symmetry.  */
      {
        of_t *files;
        வரிகள்_rr (k_units, n_units, buf, in_blk_size, &files);
      }
      நிறுத்து;

    நிலை type_undef: இயல்பு:
      affirm (false);
    }

  எனில் (close (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (infile));
  closeout (வெற்று, output_desc, filter_pid, outfile);

  main_exit (EXIT_SUCCESS);
}
