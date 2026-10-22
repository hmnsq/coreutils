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
#അന്തര്ഭാവ <config.h>

#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <getopt.h>
#അന്തര്ഭാവ <signal.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <sys/wait.h>
#അന്തര്ഭാവ <spawn.h>

#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "alignalloc.h"
#അന്തര്ഭാവ "assure.h"
#അന്തര്ഭാവ "fadvise.h"
#അന്തര്ഭാവ "fd-reopen.h"
#അന്തര്ഭാവ "fcntl--.h"
#അന്തര്ഭാവ "full-write.h"
#അന്തര്ഭാവ "ioblksize.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "sig2str.h"
#അന്തര്ഭാവ "sys-limits.h"
#അന്തര്ഭാവ "temp-stream.h"
#അന്തര്ഭാവ "unistd--.h"
#അന്തര്ഭാവ "xbinary-io.h"
#അന്തര്ഭാവ "xdectoint.h"
#അന്തര്ഭാവ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "split"

#define AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("Richard M. Stallman")

/* Shell command to filter through, instead of creating files.  */
സ്ഥിതം അക്ഷരം സ്ഥിരം *filter_command;

/* Process ID of the filter.  */
സ്ഥിതം pid_t filter_pid;

സ്ഥിതം ഘടന
{
  /* The number of open pipes.  */
  പൂർണ്ണസംഖ്യ n_pipes;

  /* The number of allocated pipes.  */
  idx_t n_pipes_alloc;

  /* Array of open pipes.  */
  പൂർണ്ണസംഖ്യ *pipes COUNTED_BY (n_pipes);
} open_pipes;

/* Whether SIGPIPE has the default action, when --filter is used.  */
സ്ഥിതം bool default_SIGPIPE;

/* Base name of output files.  */
സ്ഥിതം അക്ഷരം സ്ഥിരം *outbase = "x";

/* Name of output files.  */
സ്ഥിതം അക്ഷരം *outfile;

/* Pointer to the end of the prefix in OUTFILE.
   Suffixes are inserted here.  */
സ്ഥിതം അക്ഷരം *outfile_mid;

/* Generate new suffix when suffixes are exhausted.  */
സ്ഥിതം bool suffix_auto = true;

/* Length of OUTFILE's suffix.  */
സ്ഥിതം idx_t suffix_length;

/* Alphabet of characters to use in suffix.  */
സ്ഥിതം അക്ഷരം സ്ഥിരം *suffix_alphabet = "abcdefghijklmnopqrstuvwxyz";

/* Numerical suffix start value.  */
സ്ഥിതം അക്ഷരം സ്ഥിരം *numeric_suffix_start;

/* Additional suffix to append to output file names.  */
സ്ഥിതം അക്ഷരം സ്ഥിരം *additional_suffix;

/* Name of input file.  May be "-".  */
സ്ഥിതം അക്ഷരം സ്ഥിരം *infile = "-";

/* stat buf for input file.  */
സ്ഥിതം ഘടന stat in_stat_buf;

/* Descriptor on which output file is open.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ output_desc = -1;

/* If true, print a diagnostic on standard error just before each
   output file is opened. */
സ്ഥിതം bool verbose;

/* If true, don't generate zero length output files. */
സ്ഥിതം bool elide_empty_files;

/* If true, in round robin mode, immediately copy
   input to output, which is much slower, so disabled by default.  */
സ്ഥിതം bool unbuffered;

/* The character marking end of line.  Defaults to \n below.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ eolchar = -1;

/* The split mode to use.  */
എണ്ണൽ Split_type
{
  type_undef, type_bytes, type_byteslines, type_വരികൾ, type_digits,
  type_chunk_bytes, type_chunk_വരികൾ, type_rr
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
എണ്ണൽ
{
  VERBOSE_OPTION = CHAR_MAX + 1,
  FILTER_OPTION,
  IO_BLKSIZE_OPTION,
  ADDITIONAL_SUFFIX_OPTION
};

സ്ഥിതം ഘടന option സ്ഥിരം longopts[] =
{
  {"bytes", required_argument, ശൂന്യ, 'b'},
  {"lines", required_argument, ശൂന്യ, 'l'},
  {"line-bytes", required_argument, ശൂന്യ, 'C'},
  {"number", required_argument, ശൂന്യ, 'n'},
  {"elide-empty-files", no_argument, ശൂന്യ, 'e'},
  {"unbuffered", no_argument, ശൂന്യ, 'u'},
  {"suffix-length", required_argument, ശൂന്യ, 'a'},
  {"additional-suffix", required_argument, ശൂന്യ,
   ADDITIONAL_SUFFIX_OPTION},
  {"numeric-suffixes", optional_argument, ശൂന്യ, 'd'},
  {"hex-suffixes", optional_argument, ശൂന്യ, 'x'},
  {"filter", required_argument, ശൂന്യ, FILTER_OPTION},
  {"verbose", no_argument, ശൂന്യ, VERBOSE_OPTION},
  {"separator", required_argument, ശൂന്യ, 't'},
  {"-io-blksize", required_argument, ശൂന്യ,
   IO_BLKSIZE_OPTION}, /* do not document */
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ശൂന്യ, 0, ശൂന്യ, 0}
};

/* Return true if the errno value, ERR, is ignorable.  */
സ്ഥിതം വരിയിൽ bool
ignorable (പൂർണ്ണസംഖ്യ err)
{
  തിരികെ filter_command && err == EPIPE;
}

സ്ഥിതം ശൂന്യം
set_suffix_length (intmax_t n_units, എണ്ണൽ Split_type split_type)
{
#define DEFAULT_SUFFIX_LENGTH 2

  പൂർണ്ണസംഖ്യ suffix_length_needed = 0;

  /* The suffix auto length feature is incompatible with
     a user specified start value as the generated suffixes
     are not all consecutive.  */
  എങ്കിൽ (numeric_suffix_start)
    suffix_auto = false;

  /* Auto-calculate the suffix length if the number of files is given.  */
  എങ്കിൽ (split_type == type_chunk_bytes || split_type == type_chunk_വരികൾ
      || split_type == type_rr)
    {
      intmax_t n_units_end = n_units - 1;
      എങ്കിൽ (numeric_suffix_start)
        {
          intmax_t n_start;
          strtol_error e = xstrtoimax (numeric_suffix_start, ശൂന്യ, 10,
                                       &n_start, "");
          എങ്കിൽ (e == LONGINT_OK && n_start < n_units)
            {
              /* Restrict auto adjustment so we don't keep
                 incrementing a suffix size arbitrarily,
                 as that would break sort order for files
                 generated from multiple split runs.  */
              എങ്കിൽ (ckd_add (&n_units_end, n_units_end, n_start))
                n_units_end = INTMAX_MAX;
            }

        }
      idx_t alphabet_len = strlen (suffix_alphabet);
      ചെയ്യുക
        suffix_length_needed++;
      ആകുമ്പോൾ (n_units_end /= alphabet_len);

      suffix_auto = false;
    }

  എങ്കിൽ (suffix_length)            /* set by user */
    {
      എങ്കിൽ (suffix_length < suffix_length_needed)
        error (EXIT_FAILURE, 0,
               _("the suffix length needs to be at least %d"),
               suffix_length_needed);
      suffix_auto = false;
      തിരികെ;
    }
  അല്ലെങ്കിൽ
    suffix_length = MAX (DEFAULT_SUFFIX_LENGTH, suffix_length_needed);
}

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
    {
      printf (_("\
Usage: %s [OPTION]... [FILE [PREFIX]]\n\
"),
              program_name);
      fputs (_("\
Output pieces of FILE to PREFIXaa, PREFIXab, ...;\n\
സ്ഥിരസ്ഥിതി size is 1000 വരികൾ, and സ്ഥിരസ്ഥിതി PREFIX is 'x'.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oprintf (_("\
  -a, --suffix-length=N\n\
         generate suffixes of length N (സ്ഥിരസ്ഥിതി %d)\n\
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
  -C, --വരി-bytes=SIZE\n\
         put at most SIZE bytes of records per output file\n\
"));
      oputs (_("\
  -d\n\
         use numeric suffixes starting at 0, not alphabetic\n\
"));
      oputs (_("\
      --numeric-suffixes[=FROM]\n\
         same as -d, but allow setting the start മൂല്യം\n\
"));
      oputs (_("\
  -x\n\
         use hex suffixes starting at 0, not alphabetic\n\
"));
      oputs (_("\
      --hex-suffixes[=FROM]\n\
         same as -x, but allow setting the start മൂല്യം\n\
"));
      oputs (_("\
  -e, --elide-empty-files\n\
         ചെയ്യുക not generate empty output files with '-n'\n\
"));
      oputs (_("\
      --filter=COMMAND\n\
         ലിഖ to shell COMMAND; file name is $FILE\n\
"));
      oputs (_("\
  -l, --വരികൾ=സംഖ്യ_2\n\
         put സംഖ്യ_2 വരികൾ/records per output file\n\
"));
      oputs (_("\
  -n, --സംഖ്യ=CHUNKS\n\
         generate CHUNKS output files; see explanation below\n\
"));
      oputs (_("\
  -t, --separator=SEP\n\
         use SEP instead of newline as the record separator;\n\
         '\\0' (zero) specifies the NUL character\n\
"));
      oputs (_("\
  -u, --unbuffered\n\
         immediately copy നിവേശം to output with '-n r/...'\n\
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
  N       split into N files based on size of നിവേശം\n\
  K/N     output Kth of N to standard output\n\
  l/N     split into N files without splitting വരികൾ/records\n\
  l/K/N   output Kth of N to standard output without splitting വരികൾ/records\n\
  r/N     like 'l' but use round robin distribution\n\
  r/K/N   likewise but only output Kth of N to standard output\n\
"), stdout);
      fputs (_("\n\
-n (except -nr) will buffer to $TMPDIR, defaulting to /tmp,\n\
എങ്കിൽ the നിവേശം size cannot easily be determined.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Copy the data in FD to a temporary file, then make that file FD.
   Use BUF, of size BUFSIZE, to copy.  Return the number of
   bytes copied, or -1 (setting errno) on error.  */
സ്ഥിതം off_t
copy_to_tmpfile (പൂർണ്ണസംഖ്യ fd, അക്ഷരം *buf, idx_t bufsize)
{
  FILE *tmp;
  എങ്കിൽ (!temp_stream (&tmp, ശൂന്യ))
    തിരികെ -1;
  off_t copied = 0;
  off_t r;

  ആകുമ്പോൾ (0 < (r = പഠ (fd, buf, bufsize)))
    {
      എങ്കിൽ (fwrite (buf, 1, r, tmp) != r)
        തിരികെ -1;
      എങ്കിൽ (ckd_add (&copied, copied, r))
        {
          errno = EOVERFLOW;
          തിരികെ -1;
        }
    }

  എങ്കിൽ (r < 0)
    തിരികെ r;
  r = dup2 (fileno (tmp), fd);
  എങ്കിൽ (r < 0)
    തിരികെ r;
  എങ്കിൽ (fclose (tmp) < 0)
    തിരികെ -1;
  തിരികെ copied;
}

/* Return the number of bytes that can be read from FD with status ST.
   Store up to the first BUFSIZE bytes of the file's data into BUF,
   and advance the file position by the number of bytes read.  On
   input error, set errno and return -1.  */

സ്ഥിതം off_t
നിവേശം_file_size (പൂർണ്ണസംഖ്യ fd, ഘടന stat സ്ഥിരം *st, അക്ഷരം *buf, idx_t bufsize)
{
  off_t size = 0;
  ചെയ്യുക
    {
      ssize_t n_read = പഠ (fd, buf + size, bufsize - size);
      എങ്കിൽ (n_read <= 0)
        തിരികെ n_read < 0 ? n_read : size;
      size += n_read;
    }
  ആകുമ്പോൾ (size < bufsize);

  off_t cur, end;
  എങ്കിൽ ((usable_st_size (st) && st->st_size < size)
      || (cur = lseek (fd, 0, SEEK_CUR)) < 0
      || cur < size /* E.g., /dev/zero on GNU/Linux.  */
      || (end = lseek (fd, 0, SEEK_END)) < 0)
    {
      അക്ഷരം *tmpbuf = xmalloc (bufsize);
      end = copy_to_tmpfile (fd, tmpbuf, bufsize);
      സ്വതന്ത്രം (tmpbuf);
      എങ്കിൽ (end < 0)
        തിരികെ end;
      cur = 0;
    }

  എങ്കിൽ (end == OFF_T_MAX /* E.g., /dev/zero on GNU/Hurd.  */
      || (cur < end && ckd_add (&size, size, end - cur)))
    {
      errno = EOVERFLOW;
      തിരികെ -1;
    }

  എങ്കിൽ (cur < end)
    {
      off_t r = lseek (fd, cur, SEEK_SET);
      എങ്കിൽ (r < 0)
        തിരികെ r;
    }

  തിരികെ size;
}

/* Compute the next sequential output file name and store it into the
   string 'outfile'.  */

സ്ഥിതം ശൂന്യം
next_file_name (ശൂന്യം)
{
  /* Index in suffix_alphabet of each character in the suffix.  */
  സ്ഥിതം idx_t *sufindex;
  സ്ഥിതം idx_t outbase_length;
  സ്ഥിതം idx_t outfile_length;
  സ്ഥിതം idx_t addsuf_length;

  എങ്കിൽ (! outfile)
    {
      bool overflow, widen;

new_name:
      widen = !! outfile_length;

      എങ്കിൽ (! widen)
        {
          /* Allocate and initialize the first file name.  */

          outbase_length = strlen (outbase);
          addsuf_length = additional_suffix ? strlen (additional_suffix) : 0;
          overflow = ckd_add (&outfile_length, outbase_length + addsuf_length,
                              suffix_length);
        }
      അല്ലെങ്കിൽ
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
      എങ്കിൽ (overflow)
        xalloc_die ();
      outfile = xirealloc (outfile, outfile_size);

      എങ്കിൽ (! widen)
        memcpy (outfile, outbase, outbase_length);
      അല്ലെങ്കിൽ
        {
          /* Append the last alphabet character to the file name prefix.  */
          outfile[outbase_length] = suffix_alphabet[sufindex[0]];
          outbase_length++;
        }

      outfile_mid = outfile + outbase_length;
      memset (outfile_mid, suffix_alphabet[0], suffix_length);
      എങ്കിൽ (additional_suffix)
        memcpy (outfile_mid + suffix_length, additional_suffix, addsuf_length);
      outfile[outfile_length] = 0;

      സ്വതന്ത്രം (sufindex);
      sufindex = xicalloc (suffix_length, വലുപ്പം *sufindex);

      എങ്കിൽ (numeric_suffix_start)
        {
          affirm (! widen);

          /* Update the output file name.  */
          idx_t i = strlen (numeric_suffix_start);
          memcpy (outfile_mid + suffix_length - i, numeric_suffix_start, i);

          /* Update the suffix index.  */
          idx_t *sufindex_end = sufindex + suffix_length;
          ആകുമ്പോൾ (i-- != 0)
            *--sufindex_end = numeric_suffix_start[i] - '0';
        }

#എങ്കിൽ ! _POSIX_NO_TRUNC && HAVE_PATHCONF && defined _PC_NAME_MAX
      /* POSIX requires that if the output file name is too long for
         its directory, 'split' must fail without creating any files.
         This must be checked for explicitly on operating systems that
         silently truncate file names.  */
      {
        അക്ഷരം *dir = dir_name (outfile);
        long name_max = pathconf (dir, _PC_NAME_MAX);
        എങ്കിൽ (0 <= name_max && name_max < base_len (last_component (outfile)))
          error (EXIT_FAILURE, ENAMETOOLONG, "%s", quotef (outfile));
        സ്വതന്ത്രം (dir);
      }
#endif
    }
  അല്ലെങ്കിൽ
    {
      /* Increment the suffix in place, if possible.  */

      idx_t i = suffix_length;
      ആകുമ്പോൾ (i-- != 0)
        {
          sufindex[i]++;
          എങ്കിൽ (suffix_auto && i == 0 && ! suffix_alphabet[sufindex[0] + 1])
            പോകുക new_name;
          outfile_mid[i] = suffix_alphabet[sufindex[i]];
          എങ്കിൽ (outfile_mid[i])
            തിരികെ;
          sufindex[i] = 0;
          outfile_mid[i] = suffix_alphabet[sufindex[i]];
        }
      error (EXIT_FAILURE, 0, _("output file suffixes exhausted"));
    }
}

/* Create or truncate a file.  */

സ്ഥിതം പൂർണ്ണസംഖ്യ
create (അക്ഷരം സ്ഥിരം *name)
{
  എങ്കിൽ (!filter_command)
    {
      എങ്കിൽ (verbose)
        fprintf (stdout, _("creating file %s\n"), quoteaf (name));

      പൂർണ്ണസംഖ്യ oflags = O_WRONLY | O_CREAT | O_ദ്വിമാന;
      പൂർണ്ണസംഖ്യ fd = open (name, oflags | O_EXCL, MODE_RW_UGO);
      എങ്കിൽ (0 <= fd || errno != EEXIST)
        തിരികെ fd;
      fd = open (name, oflags, MODE_RW_UGO);
      എങ്കിൽ (fd < 0)
        തിരികെ fd;
      ഘടന stat out_stat_buf;
      എങ്കിൽ (fstat (fd, &out_stat_buf) != 0)
        error (EXIT_FAILURE, errno, _("failed to stat %s"), quoteaf (name));
      എങ്കിൽ (psame_inode (&in_stat_buf, &out_stat_buf))
        error (EXIT_FAILURE, 0, _("%s would overwrite input; aborting"),
               quoteaf (name));
      എങ്കിൽ (ftruncate (fd, 0) < 0
          && (S_ISREG (out_stat_buf.st_mode) || S_TYPEISSHM (&out_stat_buf)))
        error (EXIT_FAILURE, errno, _("%s: error truncating"), quotef (name));

      തിരികെ fd;
    }
  അല്ലെങ്കിൽ
    {
      എങ്കിൽ (setenv ("FILE", name, 1) != 0)
        error (EXIT_FAILURE, errno,
               _("failed to set FILE environment variable"));
      എങ്കിൽ (verbose)
        fprintf (stdout, _("executing with FILE=%s\n"), quotef (name));

      പൂർണ്ണസംഖ്യ result;
      പൂർണ്ണസംഖ്യ fd_pair[2];
      pid_t child_pid;

      posix_spawnattr_t attr;
      posix_spawn_file_actions_t actions;

      sigset_t set;
      sigemptyset (&set);
      എങ്കിൽ (default_SIGPIPE)
        sigaddset (&set, SIGPIPE);

      എങ്കിൽ (   (result = posix_spawnattr_init (&attr))
          || (result = posix_spawnattr_setflags (&attr,
                                                 (POSIX_SPAWN_USEVFORK
                                                  | POSIX_SPAWN_SETSIGDEF)))
          || (result = posix_spawnattr_setsigdefault (&attr, &set))
          || (result = posix_spawn_file_actions_init (&actions))
         )
        error (EXIT_FAILURE, result, _("posix_spawn initialization failed"));

      എങ്കിൽ (pipe (fd_pair) != 0)
        error (EXIT_FAILURE, errno, _("failed to create pipe"));

      /* We have to close any pipes that were opened during an
         earlier call, otherwise this process will be holding a
         write-pipe that will prevent the earlier process from
         reading an EOF on the corresponding read-pipe.  */
      വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < open_pipes.n_pipes; ++i)
        എങ്കിൽ ((result = posix_spawn_file_actions_addclose (&actions,
                                                         open_pipes.pipes[i])))
          നിർത്തുക;

      എങ്കിൽ (   result
          || (result = posix_spawn_file_actions_addclose (&actions, fd_pair[1]))
          || (fd_pair[0] != STDIN_FILENO
              && (   (result = posix_spawn_file_actions_adddup2 (&actions,
                                                                 fd_pair[0],
                                                                 STDIN_FILENO))
                  || (result = posix_spawn_file_actions_addclose (&actions,
                                                                  fd_pair[0]))))
         )
        error (EXIT_FAILURE, result, _("posix_spawn setup failed"));


      അക്ഷരം സ്ഥിരം *shell_prog = getenv ("SHELL");
      എങ്കിൽ (shell_prog == ശൂന്യ)
        shell_prog = "/bin/sh";
      അക്ഷരം സ്ഥിരം *സ്ഥിരം argv[] = { last_component (shell_prog), "-c",
                                   filter_command, ശൂന്യ };

      result = posix_spawn (&child_pid, shell_prog, &actions, &attr,
                            (അക്ഷരം * സ്ഥിരം *) argv, environ);
      എങ്കിൽ (result != 0)
        error (EXIT_FAILURE, errno, _("failed to run command: \"%s -c %s\""),
               shell_prog, filter_command);

      posix_spawnattr_destroy (&attr);
      posix_spawn_file_actions_destroy (&actions);

      എങ്കിൽ (close (fd_pair[0]) != 0)
        error (EXIT_FAILURE, errno, _("failed to close input pipe"));
      filter_pid = child_pid;
      എങ്കിൽ (open_pipes.n_pipes == open_pipes.n_pipes_alloc)
        open_pipes.pipes = xpalloc (open_pipes.pipes, &open_pipes.n_pipes_alloc,
                                    1, MIN (INT_MAX, IDX_MAX),
                                    വലുപ്പം *open_pipes.pipes);
      idx_t സ്ഥിരം pipe_idx = open_pipes.n_pipes++;
      open_pipes.pipes[pipe_idx] = fd_pair[1];
      തിരികെ fd_pair[1];
    }
}

/* Close the output file, and do any associated cleanup.
   If FP and FD are both specified, they refer to the same open file;
   in this case FP is closed, but FD is still used in cleanup.  */
സ്ഥിതം ശൂന്യം
closeout (FILE *fp, പൂർണ്ണസംഖ്യ fd, pid_t pid, അക്ഷരം സ്ഥിരം *name)
{
  എങ്കിൽ (fp != ശൂന്യ && fclose (fp) != 0 && ! ignorable (errno))
    error (EXIT_FAILURE, errno, "%s", quotef (name));
  എങ്കിൽ (fd >= 0)
    {
      എങ്കിൽ (fp == ശൂന്യ && close (fd) < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (name));
      വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < open_pipes.n_pipes; ++j)
        {
          എങ്കിൽ (open_pipes.pipes[j] == fd)
            {
              idx_t സ്ഥിരം last_idx = open_pipes.n_pipes - 1;
              open_pipes.pipes[j] = open_pipes.pipes[last_idx];
              open_pipes.n_pipes--;
              നിർത്തുക;
            }
        }
    }
  എങ്കിൽ (pid > 0)
    {
      പൂർണ്ണസംഖ്യ wstatus;
      എങ്കിൽ (waitpid (pid, &wstatus, 0) < 0)
        error (EXIT_FAILURE, errno, _("waiting for child process"));
      അല്ലെങ്കിൽ എങ്കിൽ (WIFSIGNALED (wstatus))
        {
          പൂർണ്ണസംഖ്യ sig = WTERMSIG (wstatus);
          എങ്കിൽ (sig != SIGPIPE)
            {
              അക്ഷരം signame[MAX (SIG2STR_MAX, INT_BUFSIZE_BOUND (പൂർണ്ണസംഖ്യ))];
              എങ്കിൽ (sig2str (sig, signame) != 0)
                sprintf (signame, "%d", sig);
              error (sig + 128, 0,
                     _("with FILE=%s, signal %s from command: %s"),
                     quotef (name), signame, filter_command);
            }
        }
      അല്ലെങ്കിൽ എങ്കിൽ (WIFEXITED (wstatus))
        {
          പൂർണ്ണസംഖ്യ ex = WEXITSTATUS (wstatus);
          എങ്കിൽ (ex != 0)
            error (ex, 0, _("with FILE=%s, exit %d from command: %s"),
                   quotef (name), ex, filter_command);
        }
      അല്ലെങ്കിൽ
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

സ്ഥിതം bool
cwrite (bool new_file_flag, അക്ഷരം സ്ഥിരം *bp, idx_t bytes)
{
  എങ്കിൽ (new_file_flag)
    {
      എങ്കിൽ (!bp && bytes == 0 && elide_empty_files)
        തിരികെ true;
      closeout (ശൂന്യ, output_desc, filter_pid, outfile);
      next_file_name ();
      output_desc = create (outfile);
      എങ്കിൽ (output_desc < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (outfile));
    }

  എങ്കിൽ (full_write (output_desc, bp, bytes) == bytes)
    തിരികെ true;
  അല്ലെങ്കിൽ
    {
      എങ്കിൽ (! ignorable (errno))
        error (EXIT_FAILURE, errno, "%s", quotef (outfile));
      തിരികെ false;
    }
}

/* Split into pieces of exactly N_BYTES bytes.
   However, the first REM_BYTES pieces should be 1 byte longer.
   Use buffer BUF, whose size is BUFSIZE.
   If INITIAL_READ is nonnegative,
   BUF contains the first INITIAL_READ input bytes.  */

സ്ഥിതം ശൂന്യം
bytes_split (intmax_t n_bytes, intmax_t rem_bytes,
             അക്ഷരം *buf, idx_t bufsize, ssize_t initial_read,
             intmax_t max_files)
{
  bool new_file_flag = true;
  bool filter_ok = true;
  intmax_t opened = 0;
  intmax_t to_write = n_bytes + (0 < rem_bytes);
  bool eof = ! to_write;

  ആകുമ്പോൾ (! eof)
    {
      ssize_t n_read;
      എങ്കിൽ (0 <= initial_read)
        {
          n_read = initial_read;
          initial_read = -1;
          eof = n_read < bufsize;
        }
      അല്ലെങ്കിൽ
        {
          എങ്കിൽ (! filter_ok
              && 0 <= lseek (STDIN_FILENO, to_write, SEEK_CUR))
            {
              to_write = n_bytes + (opened + 1 < rem_bytes);
              new_file_flag = true;
            }

          n_read = പഠ (STDIN_FILENO, buf, bufsize);
          എങ്കിൽ (n_read < 0)
            error (EXIT_FAILURE, errno, "%s", quotef (infile));
          eof = n_read == 0;
        }
      അക്ഷരം *bp_out = buf;
      ആകുമ്പോൾ (0 < to_write && to_write <= n_read)
        {
          എങ്കിൽ (filter_ok || new_file_flag)
            filter_ok = cwrite (new_file_flag, bp_out, to_write);
          opened += new_file_flag;
          new_file_flag = !max_files || (opened < max_files);
          എങ്കിൽ (! filter_ok && ! new_file_flag)
            {
              /* If filters no longer accepting input, stop reading.  */
              n_read = 0;
              eof = true;
              നിർത്തുക;
            }
          bp_out += to_write;
          n_read -= to_write;
          to_write = n_bytes + (opened < rem_bytes);
        }
      എങ്കിൽ (0 < n_read)
        {
          എങ്കിൽ (filter_ok || new_file_flag)
            filter_ok = cwrite (new_file_flag, bp_out, n_read);
          opened += new_file_flag;
          new_file_flag = false;
          എങ്കിൽ (! filter_ok && opened == max_files)
            {
              /* If filters no longer accepting input, stop reading.  */
              നിർത്തുക;
            }
          to_write -= n_read;
        }
    }

  /* Ensure NUMBER files are created, which truncates
     any existing files or notifies any consumers on fifos.
     FIXME: Should we do this before EXIT_FAILURE?  */
  ആകുമ്പോൾ (opened++ < max_files)
    cwrite (true, ശൂന്യ, 0);
}

/* Split into pieces of exactly N_LINES lines.
   Use buffer BUF, whose size is BUFSIZE.  */

സ്ഥിതം ശൂന്യം
വരികൾ_split (intmax_t n_വരികൾ, അക്ഷരം *buf, idx_t bufsize)
{
  ssize_t n_read;
  അക്ഷരം *bp, *bp_out, *eob;
  bool new_file_flag = true;
  intmax_t n = 0;

  ചെയ്യുക
    {
      n_read = പഠ (STDIN_FILENO, buf, bufsize);
      എങ്കിൽ (n_read < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
      bp = bp_out = buf;
      eob = bp + n_read;
      *eob = eolchar;
      ആകുമ്പോൾ (true)
        {
          bp = rawmemchr (bp, eolchar);
          എങ്കിൽ (bp == eob)
            {
              എങ്കിൽ (eob != bp_out) /* do not write 0 bytes! */
                {
                  idx_t len = eob - bp_out;
                  cwrite (new_file_flag, bp_out, len);
                  new_file_flag = false;
                }
              നിർത്തുക;
            }

          ++bp;
          എങ്കിൽ (++n >= n_വരികൾ)
            {
              cwrite (new_file_flag, bp_out, bp - bp_out);
              bp_out = bp;
              new_file_flag = true;
              n = 0;
            }
        }
    }
  ആകുമ്പോൾ (n_read);
}

/* Split into pieces that are as large as possible while still not more
   than N_BYTES bytes, and are split on line boundaries except
   where lines longer than N_BYTES bytes occur. */

സ്ഥിതം ശൂന്യം
വരി_bytes_split (intmax_t n_bytes, അക്ഷരം *buf, idx_t bufsize)
{
  ssize_t n_read;
  intmax_t n_out = 0;      /* for each split.  */
  idx_t n_hold = 0;
  അക്ഷരം *hold = ശൂന്യ;        /* for lines > bufsize.  */
  idx_t hold_size = 0;
  bool split_വരി = false;  /* Whether a \n was output in a split.  */

  ചെയ്യുക
    {
      n_read = പഠ (STDIN_FILENO, buf, bufsize);
      എങ്കിൽ (n_read < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
      idx_t n_ഇടത് = n_read;
      അക്ഷരം *sob = buf;
      ആകുമ്പോൾ (n_ഇടത്)
        {
          idx_t split_rest = 0;
          അക്ഷരം *eoc = ശൂന്യ;
          അക്ഷരം *eol;

          /* Determine End Of Chunk and/or End of Line,
             which are used below to select what to write or buffer.  */
          എങ്കിൽ (n_bytes - n_out - n_hold <= n_ഇടത്)
            {
              /* Have enough for split.  */
              split_rest = n_bytes - n_out - n_hold;
              eoc = sob + split_rest - 1;
              eol = memrchr (sob, eolchar, split_rest);
            }
          അല്ലെങ്കിൽ
            eol = memrchr (sob, eolchar, n_ഇടത്);

          /* Output hold space if possible.  */
          എങ്കിൽ (n_hold && !(!eol && n_out))
            {
              cwrite (n_out == 0, hold, n_hold);
              n_out += n_hold;
              n_hold = 0;
            }

          /* Output to eol if present.  */
          എങ്കിൽ (eol)
            {
              split_വരി = true;
              idx_t n_write = eol - sob + 1;
              cwrite (n_out == 0, sob, n_write);
              n_out += n_write;
              n_ഇടത് -= n_write;
              sob += n_write;
              എങ്കിൽ (eoc)
                split_rest -= n_write;
            }

          /* Output to eoc or eob if possible.  */
          എങ്കിൽ (n_ഇടത് && !split_വരി)
            {
              idx_t n_write = eoc ? split_rest : n_ഇടത്;
              cwrite (n_out == 0, sob, n_write);
              n_out += n_write;
              n_ഇടത് -= n_write;
              sob += n_write;
              എങ്കിൽ (eoc)
                split_rest -= n_write;
            }

          /* Update hold if needed.  */
          എങ്കിൽ ((eoc && split_rest) || (!eoc && n_ഇടത്))
            {
              idx_t n_buf = eoc ? split_rest : n_ഇടത്;
              എങ്കിൽ (hold_size - n_hold < n_buf)
                hold = xpalloc (hold, &hold_size, n_buf - (hold_size - n_hold),
                                -1, വലുപ്പം *hold);
              memcpy (hold + n_hold, sob, n_buf);
              n_hold += n_buf;
              n_ഇടത് -= n_buf;
              sob += n_buf;
            }

          /* Reset for new split.  */
          എങ്കിൽ (eoc)
            {
              n_out = 0;
              split_വരി = false;
            }
        }
    }
  ആകുമ്പോൾ (n_read);

  /* Handle no eol at end of file.  */
  എങ്കിൽ (n_hold)
    cwrite (n_out == 0, hold, n_hold);

  സ്വതന്ത്രം (hold);
}

/* -n l/[K/]N: Write lines to files of approximately file size / N.
   The file is partitioned into file size / N sized portions, with the
   last assigned any excess.  If a line _starts_ within a partition
   it is written completely to the corresponding file.  Since lines
   are not split even if they overlap a partition, the files written
   can be larger or smaller than the partition size, and even empty
   if a line is so long as to completely overlap the partition.  */

സ്ഥിതം ശൂന്യം
വരികൾ_chunk_split (intmax_t k, intmax_t n, അക്ഷരം *buf, idx_t bufsize,
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

  എങ്കിൽ (k > 1 && 0 < file_size)
    {
      /* Start reading 1 byte before kth chunk of file.  */
      off_t start = (k - 1) * chunk_size + MIN (k - 1, rem_bytes) - 1;
      എങ്കിൽ (start < initial_read)
        {
          memmove (buf, buf + start, initial_read - start);
          initial_read -= start;
        }
      അല്ലെങ്കിൽ
        {
          എങ്കിൽ (initial_read < start
              && lseek (STDIN_FILENO, start - initial_read, SEEK_CUR) < 0)
            error (EXIT_FAILURE, errno, "%s", quotef (infile));
          initial_read = -1;
        }
      n_written = start;
      chunk_no = k - 1;
      chunk_end = start + 1;
    }

  ആകുമ്പോൾ (n_written < file_size)
    {
      അക്ഷരം *bp = buf, *eob;
      ssize_t n_read;
      എങ്കിൽ (0 <= initial_read)
        {
          n_read = initial_read;
          initial_read = -1;
        }
      അല്ലെങ്കിൽ
        {
          n_read = പഠ (STDIN_FILENO, buf,
                         MIN (bufsize, file_size - n_written));
          എങ്കിൽ (n_read < 0)
            error (EXIT_FAILURE, errno, "%s", quotef (infile));
        }
      എങ്കിൽ (n_read == 0)
        നിർത്തുക; /* eof.  */
      chunk_truncated = false;
      eob = buf + n_read;

      ആകുമ്പോൾ (bp != eob)
        {
          idx_t to_write;
          bool next = false;

          /* Begin looking for '\n' at last byte of chunk.  */
          off_t skip = MIN (n_read, MAX (0, chunk_end - 1 - n_written));
          അക്ഷരം *bp_out = memchr (bp + skip, eolchar, n_read - skip);
          എങ്കിൽ (bp_out)
            {
              bp_out++;
              next = true;
            }
          അല്ലെങ്കിൽ
            bp_out = eob;
          to_write = bp_out - bp;

          എങ്കിൽ (k == chunk_no)
            {
              /* We don't use the stdout buffer here since we're writing
                 large chunks from an existing file, so it's more efficient
                 to write out directly.  */
              എങ്കിൽ (full_write (STDOUT_FILENO, bp, to_write) != to_write)
                write_error ();
            }
          അല്ലെങ്കിൽ എങ്കിൽ (! k)
            cwrite (new_file_flag, bp, to_write);
          n_written += to_write;
          bp += to_write;
          n_read -= to_write;
          new_file_flag = next;

          /* A line could have been so long that it skipped
             entire chunks. So create empty files in that case.  */
          ആകുമ്പോൾ (next || chunk_end <= n_written)
            {
              എങ്കിൽ (!next && bp == eob)
                {
                  /* replenish buf, before going to next chunk.  */
                  chunk_truncated = true;
                  നിർത്തുക;
                }
              എങ്കിൽ (k == chunk_no)
                തിരികെ;
              chunk_end += chunk_size + (chunk_no < rem_bytes);
              chunk_no++;
              എങ്കിൽ (chunk_end <= n_written)
                {
                  എങ്കിൽ (! k)
                    cwrite (true, ശൂന്യ, 0);
                }
              അല്ലെങ്കിൽ
                next = false;
            }
        }
    }

  എങ്കിൽ (chunk_truncated)
    chunk_no++;

  /* Ensure NUMBER files are created, which truncates
     any existing files or notifies any consumers on fifos.
     FIXME: Should we do this before EXIT_FAILURE?  */
  എങ്കിൽ (!k)
    ആകുമ്പോൾ (chunk_no++ <= n)
      cwrite (true, ശൂന്യ, 0);
}

/* -n K/N: Extract Kth of N chunks.  */

സ്ഥിതം ശൂന്യം
bytes_chunk_extract (intmax_t k, intmax_t n, അക്ഷരം *buf, idx_t bufsize,
                     ssize_t initial_read, off_t file_size)
{
  off_t start;
  off_t end;

  affirm (0 < k && k <= n);

  start = (k - 1) * (file_size / n) + MIN (k - 1, file_size % n);
  end = k == n ? file_size : k * (file_size / n) + MIN (k, file_size % n);

  എങ്കിൽ (start < initial_read)
    {
      memmove (buf, buf + start, initial_read - start);
      initial_read -= start;
    }
  അല്ലെങ്കിൽ
    {
      എങ്കിൽ (initial_read < start
          && lseek (STDIN_FILENO, start - initial_read, SEEK_CUR) < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
      initial_read = -1;
    }

  ആകുമ്പോൾ (start < end)
    {
      ssize_t n_read;
      എങ്കിൽ (0 <= initial_read)
        {
          n_read = initial_read;
          initial_read = -1;
        }
      അല്ലെങ്കിൽ
        {
          n_read = പഠ (STDIN_FILENO, buf, bufsize);
          എങ്കിൽ (n_read < 0)
            error (EXIT_FAILURE, errno, "%s", quotef (infile));
        }
      എങ്കിൽ (n_read == 0)
        നിർത്തുക; /* eof.  */
      n_read = MIN (n_read, end - start);
      എങ്കിൽ (full_write (STDOUT_FILENO, buf, n_read) != n_read
          && ! ignorable (errno))
        error (EXIT_FAILURE, errno, "%s", quotef ("-"));
      start += n_read;
    }
}

തരനിർവചനം ഘടന of_info
{
  അക്ഷരം *of_name;
  പൂർണ്ണസംഖ്യ ofd;
  FILE *ofile;
  pid_t opid;
} of_t;

എണ്ണൽ
{
  OFD_NEW = -1,
  OFD_APPEND = -2
};

/* Rotate file descriptors when we're writing to more output files than we
   have available file descriptors.
   Return whether we came under file resource pressure.
   If so, it's probably best to close each file when finished with it.  */

സ്ഥിതം bool
ofile_open (of_t *files, idx_t i_check, idx_t nfiles)
{
  bool file_limit = false;

  എങ്കിൽ (files[i_check].ofd <= OFD_NEW)
    {
      പൂർണ്ണസംഖ്യ fd;
      idx_t i_reopen = i_check ? i_check - 1 : nfiles - 1;

      /* Another process could have opened a file in between the calls to
         close and open, so we should keep trying until open succeeds or
         we've closed all of our files.  */
      ആകുമ്പോൾ (true)
        {
          എങ്കിൽ (files[i_check].ofd == OFD_NEW)
            fd = create (files[i_check].of_name);
          അല്ലെങ്കിൽ /* OFD_APPEND  */
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
                         O_WRONLY | O_ദ്വിമാന | O_APPEND | O_NONBLOCK);
            }

          എങ്കിൽ (0 <= fd)
            നിർത്തുക;

          എങ്കിൽ (!(errno == EMFILE || errno == ENFILE))
            error (EXIT_FAILURE, errno, "%s", quotef (files[i_check].of_name));

          file_limit = true;

          /* Search backwards for an open file to close.  */
          ആകുമ്പോൾ (files[i_reopen].ofd < 0)
            {
              i_reopen = i_reopen ? i_reopen - 1 : nfiles - 1;
              /* No more open files to close, exit with E[NM]FILE.  */
              എങ്കിൽ (i_reopen == i_check)
                error (EXIT_FAILURE, errno, "%s",
                       quotef (files[i_check].of_name));
            }

          എങ്കിൽ (fclose (files[i_reopen].ofile) != 0)
            error (EXIT_FAILURE, errno, "%s", quotef (files[i_reopen].of_name));
          files[i_reopen].ofile = ശൂന്യ;
          files[i_reopen].ofd = OFD_APPEND;
        }

      files[i_check].ofd = fd;
      FILE *ofile = fdopen (fd, "a");
      എങ്കിൽ (!ofile)
        error (EXIT_FAILURE, errno, "%s", quotef (files[i_check].of_name));
      files[i_check].ofile = ofile;
      files[i_check].opid = filter_pid;
      filter_pid = 0;
    }

  തിരികെ file_limit;
}

/* -n r/[K/]N: Divide file into N chunks in round robin fashion.
   Use BUF of size BUFSIZE for the buffer, and if allocating storage
   put its address into *FILESP to pacify -fsanitize=leak.
   When K == 0, we try to keep the files open in parallel.
   If we run out of file resources, then we revert
   to opening and closing each file for each line.  */

സ്ഥിതം ശൂന്യം
വരികൾ_rr (intmax_t k, intmax_t n, അക്ഷരം *buf, idx_t bufsize, of_t **filesp)
{
  bool wrapped = false;
  bool wrote = false;
  bool file_limit;
  idx_t i_file;
  of_t *files IF_LINT (= ശൂന്യ);
  intmax_t വരി_no;

  എങ്കിൽ (k)
    വരി_no = 1;
  അല്ലെങ്കിൽ
    {
      എങ്കിൽ (IDX_MAX < n)
        xalloc_die ();
      files = *filesp = xinmalloc (n, വലുപ്പം *files);

      /* Generate output file names. */
      വേണ്ടി (i_file = 0; i_file < n; i_file++)
        {
          next_file_name ();
          files[i_file].of_name = xstrdup (outfile);
          files[i_file].ofd = OFD_NEW;
          files[i_file].ofile = ശൂന്യ;
          files[i_file].opid = 0;
        }
      i_file = 0;
      file_limit = false;
    }

  ആകുമ്പോൾ (true)
    {
      അക്ഷരം *bp = buf, *eob;
      ssize_t n_read = പഠ (STDIN_FILENO, buf, bufsize);
      എങ്കിൽ (n_read < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
      അല്ലെങ്കിൽ എങ്കിൽ (n_read == 0)
        നിർത്തുക; /* eof.  */
      eob = buf + n_read;

      ആകുമ്പോൾ (bp != eob)
        {
          idx_t to_write;
          bool next = false;

          /* Find end of line. */
          അക്ഷരം *bp_out = memchr (bp, eolchar, eob - bp);
          എങ്കിൽ (bp_out)
            {
              bp_out++;
              next = true;
            }
          അല്ലെങ്കിൽ
            bp_out = eob;
          to_write = bp_out - bp;

          എങ്കിൽ (k)
            {
              എങ്കിൽ (വരി_no == k && unbuffered)
                {
                  എങ്കിൽ (full_write (STDOUT_FILENO, bp, to_write) != to_write)
                    write_error ();
                }
              അല്ലെങ്കിൽ എങ്കിൽ (വരി_no == k && fwrite (bp, to_write, 1, stdout) != 1)
                {
                  write_error ();
                }
              എങ്കിൽ (next)
                വരി_no = (വരി_no == n) ? 1 : വരി_no + 1;
            }
          അല്ലെങ്കിൽ
            {
              /* Secure file descriptor. */
              file_limit |= ofile_open (files, i_file, n);
              എങ്കിൽ (unbuffered)
                {
                  /* Note writing to fd, rather than flushing the FILE gives
                     an 8% performance benefit, due to reduced data copying.  */
                  എങ്കിൽ (full_write (files[i_file].ofd, bp, to_write) != to_write
                      && ! ignorable (errno))
                    error (EXIT_FAILURE, errno, "%s",
                           quotef (files[i_file].of_name));
                }
              അല്ലെങ്കിൽ എങ്കിൽ (fwrite (bp, to_write, 1, files[i_file].ofile) != 1
                       && ! ignorable (errno))
                error (EXIT_FAILURE, errno, "%s",
                       quotef (files[i_file].of_name));

              എങ്കിൽ (! ignorable (errno))
                wrote = true;

              എങ്കിൽ (file_limit)
                {
                  എങ്കിൽ (fclose (files[i_file].ofile) != 0)
                    error (EXIT_FAILURE, errno, "%s",
                           quotef (files[i_file].of_name));
                  files[i_file].ofile = ശൂന്യ;
                  files[i_file].ofd = OFD_APPEND;
                }
              എങ്കിൽ (next && ++i_file == n)
                {
                  wrapped = true;
                  /* If no filters are accepting input, stop reading.  */
                  എങ്കിൽ (! wrote)
                    പോകുക no_filters;
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
  എങ്കിൽ (!k)
    {
      idx_t ceiling = wrapped ? n : i_file;
      വേണ്ടി (i_file = 0; i_file < n; i_file++)
        {
          എങ്കിൽ (i_file >= ceiling && !elide_empty_files)
            file_limit |= ofile_open (files, i_file, n);
          എങ്കിൽ (files[i_file].ofd >= 0)
            closeout (files[i_file].ofile, files[i_file].ofd,
                      files[i_file].opid, files[i_file].of_name);
          files[i_file].ofd = OFD_APPEND;
        }
    }
}

#define FAIL_ONLY_ONE_WAY()					\
  ചെയ്യുക								\
    {								\
      error (0, 0, _("cannot split in more than one way"));	\
      usage (EXIT_FAILURE);					\
    }								\
  ആകുമ്പോൾ (0)

/* Report a string-to-integer conversion failure MSGID with ARG.  */

സ്ഥിതം _Noreturn ശൂന്യം
strtoint_die (അക്ഷരം സ്ഥിരം *msgid, അക്ഷരം സ്ഥിരം *arg)
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

സ്ഥിതം intmax_t
parse_n_units (അക്ഷരം സ്ഥിരം *arg, അക്ഷരം സ്ഥിരം *multipliers, അക്ഷരം സ്ഥിരം *msgid)
{
  intmax_t n;
  എങ്കിൽ (OVERFLOW_OK < xstrtoimax (arg, ശൂന്യ, 10, &n, multipliers) || n < 1)
    strtoint_die (msgid, arg);
  തിരികെ n;
}

/* Parse K/N syntax of chunk options.  */

സ്ഥിതം ശൂന്യം
parse_chunk (intmax_t *k_units, intmax_t *n_units, അക്ഷരം സ്ഥിരം *arg)
{
  അക്ഷരം *argend;
  strtol_error e = xstrtoimax (arg, &argend, 10, n_units, "");
  എങ്കിൽ (e == LONGINT_INVALID_SUFFIX_CHAR && *argend == '/')
    {
      *k_units = *n_units;
      *n_units = parse_n_units (argend + 1, "",
                                N_("invalid number of chunks"));
      എങ്കിൽ (! (0 < *k_units && *k_units <= *n_units))
        error (EXIT_FAILURE, 0, "%s: %s", _("invalid chunk number"),
               quote_mem (arg, argend - arg));
    }
  അല്ലെങ്കിൽ എങ്കിൽ (! (e <= OVERFLOW_OK && 0 < *n_units))
    strtoint_die (N_("invalid number of chunks"), arg);
}


പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  എണ്ണൽ Split_type split_type = type_undef;
  idx_t in_blk_size = 0;	/* optimal block size of input file device */
  idx_t page_size = getpagesize ();
  intmax_t k_units = 0;
  intmax_t n_units = 0;

  സ്ഥിതം അക്ഷരം സ്ഥിരം multipliers[] = "bEGKkMmPQRTYZ0";
  പൂർണ്ണസംഖ്യ c;
  പൂർണ്ണസംഖ്യ digits_optind = 0;
  off_t file_size = OFF_T_MAX;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* Parse command line options.  */

  ആകുമ്പോൾ (true)
    {
      /* This is the argv-index of the option we will read next.  */
      പൂർണ്ണസംഖ്യ this_optind = optind ? optind : 1;

      c = getopt_long (argc, argv, "0123456789C:a:b:del:n:t:ux",
                       longopts, ശൂന്യ);
      എങ്കിൽ (c == -1)
        നിർത്തുക;

      തിരഞ്ഞെടുപ്പ് (c)
        {
        അവസ്ഥ 'a':
          suffix_length = xdectoimax (optarg, 0, IDX_MAX,
                                      "", _("invalid suffix length"), 0);
          നിർത്തുക;

        അവസ്ഥ ADDITIONAL_SUFFIX_OPTION:
          {
            പൂർണ്ണസംഖ്യ suffix_len = strlen (optarg);
            എങ്കിൽ (last_component (optarg) != optarg
                || (suffix_len && ISSLASH (optarg[suffix_len - 1])))
              {
                error (0, 0,
                       _("invalid suffix %s, contains directory separator"),
                       quote (optarg));
                usage (EXIT_FAILURE);
              }
          }
          additional_suffix = optarg;
          നിർത്തുക;

        അവസ്ഥ 'b':
          എങ്കിൽ (split_type != type_undef)
            FAIL_ONLY_ONE_WAY ();
          split_type = type_bytes;
          n_units = parse_n_units (optarg, multipliers,
                                   N_("invalid number of bytes"));
          നിർത്തുക;

        അവസ്ഥ 'l':
          എങ്കിൽ (split_type != type_undef)
            FAIL_ONLY_ONE_WAY ();
          split_type = type_വരികൾ;
          n_units = parse_n_units (optarg, "", N_("invalid number of lines"));
          നിർത്തുക;

        അവസ്ഥ 'C':
          എങ്കിൽ (split_type != type_undef)
            FAIL_ONLY_ONE_WAY ();
          split_type = type_byteslines;
          n_units = parse_n_units (optarg, multipliers,
                                   N_("invalid number of lines"));
          നിർത്തുക;

        അവസ്ഥ 'n':
          എങ്കിൽ (split_type != type_undef)
            FAIL_ONLY_ONE_WAY ();
          /* skip any whitespace */
          ആകുമ്പോൾ (isspace (to_uchar (*optarg)))
            optarg++;
          എങ്കിൽ (STRNCMP_LIT (optarg, "r/") == 0)
            {
              split_type = type_rr;
              optarg += 2;
            }
          അല്ലെങ്കിൽ എങ്കിൽ (STRNCMP_LIT (optarg, "l/") == 0)
            {
              split_type = type_chunk_വരികൾ;
              optarg += 2;
            }
          അല്ലെങ്കിൽ
            split_type = type_chunk_bytes;
          parse_chunk (&k_units, &n_units, optarg);
          നിർത്തുക;

        അവസ്ഥ 'u':
          unbuffered = true;
          നിർത്തുക;

        അവസ്ഥ 't':
          {
            അക്ഷരം neweol = optarg[0];
            എങ്കിൽ (! neweol)
              error (EXIT_FAILURE, 0, _("empty record separator"));
            എങ്കിൽ (optarg[1])
              {
                എങ്കിൽ (streq (optarg, "\\0"))
                  neweol = '\0';
                അല്ലെങ്കിൽ
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
            എങ്കിൽ (0 <= eolchar && neweol != eolchar)
              {
                error (EXIT_FAILURE, 0,
                       _("multiple separator characters specified"));
              }

            eolchar = neweol;
          }
          നിർത്തുക;

        അവസ്ഥ '0':
        അവസ്ഥ '1':
        അവസ്ഥ '2':
        അവസ്ഥ '3':
        അവസ്ഥ '4':
        അവസ്ഥ '5':
        അവസ്ഥ '6':
        അവസ്ഥ '7':
        അവസ്ഥ '8':
        അവസ്ഥ '9':
          എങ്കിൽ (split_type == type_undef)
            {
              split_type = type_digits;
              n_units = 0;
            }
          എങ്കിൽ (split_type != type_undef && split_type != type_digits)
            FAIL_ONLY_ONE_WAY ();
          എങ്കിൽ (digits_optind != 0 && digits_optind != this_optind)
            n_units = 0;	/* More than one number given; ignore other. */
          digits_optind = this_optind;
          എങ്കിൽ (ckd_mul (&n_units, n_units, 10)
              || ckd_add (&n_units, n_units, c - '0'))
            n_units = INTMAX_MAX;
          നിർത്തുക;

        അവസ്ഥ 'd':
        അവസ്ഥ 'x':
          എങ്കിൽ (c == 'd')
            suffix_alphabet = "0123456789";
          അല്ലെങ്കിൽ
            suffix_alphabet = "0123456789abcdef";
          എങ്കിൽ (optarg)
            {
              എങ്കിൽ (strlen (optarg) != strspn (optarg, suffix_alphabet))
                {
                  error (0, 0,
                         (c == 'd') ?
                           _("%s: invalid start value for numerical suffix") :
                           _("%s: invalid start value for hexadecimal suffix"),
                         quote (optarg));
                  usage (EXIT_FAILURE);
                }
              അല്ലെങ്കിൽ
                {
                  /* Skip any leading zero.  */
                  ആകുമ്പോൾ (*optarg == '0' && *(optarg + 1) != '\0')
                    optarg++;
                  numeric_suffix_start = optarg;
                }
            }
          നിർത്തുക;

        അവസ്ഥ 'e':
          elide_empty_files = true;
          നിർത്തുക;

        അവസ്ഥ FILTER_OPTION:
          filter_command = optarg;
          നിർത്തുക;

        അവസ്ഥ IO_BLKSIZE_OPTION:
          in_blk_size = xnumtoumax (optarg, 10, 1,
                                    MIN (SYS_BUFSIZE_MAX,
                                         MIN (IDX_MAX, SIZE_MAX) - 1),
                                    multipliers, _("invalid IO block size"),
                                    0, XTOINT_MIN_RANGE);
          നിർത്തുക;

        അവസ്ഥ VERBOSE_OPTION:
          verbose = true;
          നിർത്തുക;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        സ്ഥിരസ്ഥിതി:
          usage (EXIT_FAILURE);
        }
    }

  എങ്കിൽ (k_units != 0 && filter_command)
    {
      error (0, 0, _("--filter does not process a chunk extracted to "
                     "standard output"));
      usage (EXIT_FAILURE);
    }

  /* Handle default case.  */
  എങ്കിൽ (split_type == type_undef)
    {
      split_type = type_വരികൾ;
      n_units = 1000;
    }

  എങ്കിൽ (n_units == 0)
    {
      error (0, 0, _("invalid number of lines: %s"), quote ("0"));
      usage (EXIT_FAILURE);
    }

  എങ്കിൽ (eolchar < 0)
    eolchar = '\n';

  set_suffix_length (n_units, split_type);

  /* Get out the filename arguments.  */

  എങ്കിൽ (optind < argc)
    infile = argv[optind++];

  എങ്കിൽ (optind < argc)
    outbase = argv[optind++];

  എങ്കിൽ (optind < argc)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind]));
      usage (EXIT_FAILURE);
    }

  /* Check that the suffix length is large enough for the numerical
     suffix start value.  */
  എങ്കിൽ (numeric_suffix_start && strlen (numeric_suffix_start) > suffix_length)
    {
      error (0, 0, _("numerical suffix start value is too large "
                     "for the suffix length"));
      usage (EXIT_FAILURE);
    }

  /* Open the input file.  */
  എങ്കിൽ (! streq (infile, "-")
      && fd_reopen (STDIN_FILENO, infile, O_RDONLY, 0) < 0)
    error (EXIT_FAILURE, errno, _("cannot open %s for reading"),
           quoteaf (infile));

  /* Binary I/O is safer when byte counts are used.  */
  xset_ദ്വിമാന_mode (STDIN_FILENO, O_ദ്വിമാന);

  /* Advise the kernel of our access pattern.  */
  fdadvise (STDIN_FILENO, 0, 0, FADVISE_SEQUENTIAL);

  /* Get the optimal block size of input device and make a buffer.  */

  എങ്കിൽ (fstat (STDIN_FILENO, &in_stat_buf) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (infile));

  എങ്കിൽ (in_blk_size == 0)
    {
      in_blk_size = io_blksize (&in_stat_buf);
      എങ്കിൽ (SYS_BUFSIZE_MAX < in_blk_size)
        in_blk_size = SYS_BUFSIZE_MAX;
    }

  അക്ഷരം *buf = xalignalloc (page_size, in_blk_size + 1);
  ssize_t initial_read = -1;

  എങ്കിൽ (split_type == type_chunk_bytes || split_type == type_chunk_വരികൾ)
    {
      file_size = നിവേശം_file_size (STDIN_FILENO, &in_stat_buf,
                                   buf, in_blk_size);
      എങ്കിൽ (file_size < 0)
        error (EXIT_FAILURE, errno, _("%s: cannot determine file size"),
               quotef (infile));
      initial_read = MIN (file_size, in_blk_size);
    }

  /* When filtering, closure of one pipe must not terminate the process,
     as there may still be other streams expecting input from us.  */
  എങ്കിൽ (filter_command)
    default_SIGPIPE = സൂചന (SIGPIPE, SIG_IGN) == SIG_DFL;

  തിരഞ്ഞെടുപ്പ് (split_type)
    {
    അവസ്ഥ type_digits:
    അവസ്ഥ type_വരികൾ:
      വരികൾ_split (n_units, buf, in_blk_size);
      നിർത്തുക;

    അവസ്ഥ type_bytes:
      bytes_split (n_units, 0, buf, in_blk_size, -1, 0);
      നിർത്തുക;

    അവസ്ഥ type_byteslines:
      വരി_bytes_split (n_units, buf, in_blk_size);
      നിർത്തുക;

    അവസ്ഥ type_chunk_bytes:
      എങ്കിൽ (k_units == 0)
        bytes_split (file_size / n_units, file_size % n_units,
                     buf, in_blk_size, initial_read, n_units);
      അല്ലെങ്കിൽ
        bytes_chunk_extract (k_units, n_units, buf, in_blk_size, initial_read,
                             file_size);
      നിർത്തുക;

    അവസ്ഥ type_chunk_വരികൾ:
      വരികൾ_chunk_split (k_units, n_units, buf, in_blk_size, initial_read,
                         file_size);
      നിർത്തുക;

    അവസ്ഥ type_rr:
      /* Note, this is like 'sed -n ${k}~${n}p' when k > 0,
         but the functionality is provided for symmetry.  */
      {
        of_t *files;
        വരികൾ_rr (k_units, n_units, buf, in_blk_size, &files);
      }
      നിർത്തുക;

    അവസ്ഥ type_undef: സ്ഥിരസ്ഥിതി:
      affirm (false);
    }

  എങ്കിൽ (close (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (infile));
  closeout (ശൂന്യ, output_desc, filter_pid, outfile);

  main_exit (EXIT_SUCCESS);
}
