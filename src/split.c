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
#अन्तर्भाव <config.h>

#अन्तर्भाव <stdio.h>
#अन्तर्भाव <getopt.h>
#अन्तर्भाव <signal.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <sys/wait.h>
#अन्तर्भाव <spawn.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "alignalloc.h"
#अन्तर्भाव "assure.h"
#अन्तर्भाव "fadvise.h"
#अन्तर्भाव "fd-reopen.h"
#अन्तर्भाव "fcntl--.h"
#अन्तर्भाव "full-write.h"
#अन्तर्भाव "ioblksize.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "sig2str.h"
#अन्तर्भाव "sys-limits.h"
#अन्तर्भाव "temp-stream.h"
#अन्तर्भाव "unistd--.h"
#अन्तर्भाव "xbinary-io.h"
#अन्तर्भाव "xdectoint.h"
#अन्तर्भाव "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "split"

#define AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("Richard M. Stallman")

/* Shell command to filter through, instead of creating files.  */
स्थिर अक्षर स्थिरांक *filter_command;

/* Process ID of the filter.  */
स्थिर pid_t filter_pid;

स्थिर संरचना
{
  /* The number of open pipes.  */
  पूर्णांक n_pipes;

  /* The number of allocated pipes.  */
  idx_t n_pipes_alloc;

  /* Array of open pipes.  */
  पूर्णांक *pipes COUNTED_BY (n_pipes);
} open_pipes;

/* Whether SIGPIPE has the default action, when --filter is used.  */
स्थिर bool default_SIGPIPE;

/* Base name of output files.  */
स्थिर अक्षर स्थिरांक *outbase = "x";

/* Name of output files.  */
स्थिर अक्षर *outfile;

/* Pointer to the end of the prefix in OUTFILE.
   Suffixes are inserted here.  */
स्थिर अक्षर *outfile_mid;

/* Generate new suffix when suffixes are exhausted.  */
स्थिर bool suffix_auto = true;

/* Length of OUTFILE's suffix.  */
स्थिर idx_t suffix_length;

/* Alphabet of characters to use in suffix.  */
स्थिर अक्षर स्थिरांक *suffix_alphabet = "abcdefghijklmnopqrstuvwxyz";

/* Numerical suffix start value.  */
स्थिर अक्षर स्थिरांक *numeric_suffix_start;

/* Additional suffix to append to output file names.  */
स्थिर अक्षर स्थिरांक *additional_suffix;

/* Name of input file.  May be "-".  */
स्थिर अक्षर स्थिरांक *infile = "-";

/* stat buf for input file.  */
स्थिर संरचना stat in_stat_buf;

/* Descriptor on which output file is open.  */
स्थिर पूर्णांक output_desc = -1;

/* If true, print a diagnostic on standard error just before each
   output file is opened. */
स्थिर bool verbose;

/* If true, don't generate zero length output files. */
स्थिर bool elide_empty_files;

/* If true, in round robin mode, immediately copy
   input to output, which is much slower, so disabled by default.  */
स्थिर bool unbuffered;

/* The character marking end of line.  Defaults to \n below.  */
स्थिर पूर्णांक eolchar = -1;

/* The split mode to use.  */
गणना Split_type
{
  type_undef, type_bytes, type_byteslines, type_ओळी, type_digits,
  type_chunk_bytes, type_chunk_ओळी, type_rr
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
गणना
{
  VERBOSE_OPTION = अक्षर_उच्च + 1,
  FILTER_OPTION,
  IO_BLKSIZE_OPTION,
  ADDITIONAL_SUFFIX_OPTION
};

स्थिर संरचना option स्थिरांक longopts[] =
{
  {"bytes", required_argument, रिक्त, 'b'},
  {"lines", required_argument, रिक्त, 'l'},
  {"line-bytes", required_argument, रिक्त, 'C'},
  {"number", required_argument, रिक्त, 'n'},
  {"elide-empty-files", no_argument, रिक्त, 'e'},
  {"unbuffered", no_argument, रिक्त, 'u'},
  {"suffix-length", required_argument, रिक्त, 'a'},
  {"additional-suffix", required_argument, रिक्त,
   ADDITIONAL_SUFFIX_OPTION},
  {"numeric-suffixes", optional_argument, रिक्त, 'd'},
  {"hex-suffixes", optional_argument, रिक्त, 'x'},
  {"filter", required_argument, रिक्त, FILTER_OPTION},
  {"verbose", no_argument, रिक्त, VERBOSE_OPTION},
  {"separator", required_argument, रिक्त, 't'},
  {"-io-blksize", required_argument, रिक्त,
   IO_BLKSIZE_OPTION}, /* do not document */
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

/* Return true if the errno value, ERR, is ignorable.  */
स्थिर ओळीत bool
ignorable (पूर्णांक err)
{
  परत filter_command && err == EPIPE;
}

स्थिर शून्य
set_suffix_length (intmax_t n_units, गणना Split_type split_type)
{
#define DEFAULT_SUFFIX_LENGTH 2

  पूर्णांक suffix_length_needed = 0;

  /* The suffix auto length feature is incompatible with
     a user specified start value as the generated suffixes
     are not all consecutive.  */
  जर (numeric_suffix_start)
    suffix_auto = false;

  /* Auto-calculate the suffix length if the number of files is given.  */
  जर (split_type == type_chunk_bytes || split_type == type_chunk_ओळी
      || split_type == type_rr)
    {
      intmax_t n_units_end = n_units - 1;
      जर (numeric_suffix_start)
        {
          intmax_t n_start;
          strtol_error e = xstrtoimax (numeric_suffix_start, रिक्त, 10,
                                       &n_start, "");
          जर (e == LONGINT_OK && n_start < n_units)
            {
              /* Restrict auto adjustment so we don't keep
                 incrementing a suffix size arbitrarily,
                 as that would break sort order for files
                 generated from multiple split runs.  */
              जर (ckd_add (&n_units_end, n_units_end, n_start))
                n_units_end = INTMAX_MAX;
            }

        }
      idx_t alphabet_len = म_माप (suffix_alphabet);
      करा
        suffix_length_needed++;
      पर्यंत (n_units_end /= alphabet_len);

      suffix_auto = false;
    }

  जर (suffix_length)            /* set by user */
    {
      जर (suffix_length < suffix_length_needed)
        error (निकास_त्रुटि, 0,
               _("the suffix length needs to be at least %d"),
               suffix_length_needed);
      suffix_auto = false;
      परत;
    }
  अन्यथा
    suffix_length = MAX (DEFAULT_SUFFIX_LENGTH, suffix_length_needed);
}

शून्य
usage (पूर्णांक status)
{
  जर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s [OPTION]... [खाता [PREFIX]]\n\
"),
              program_name);
      ख_माला_दो (_("\
Output pieces of खाता to PREFIXaa, PREFIXab, ...;\n\
पूर्वनिर्धारित size is 1000 ओळी, and पूर्वनिर्धारित PREFIX is 'x'.\n\
"), मानक_निकास);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oprintf (_("\
  -a, --suffix-length=N\n\
         generate suffixes of length N (पूर्वनिर्धारित %d)\n\
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
  -C, --ओळ-bytes=SIZE\n\
         put at most SIZE bytes of records per output file\n\
"));
      oputs (_("\
  -d\n\
         use numeric suffixes starting at 0, not alphabetic\n\
"));
      oputs (_("\
      --numeric-suffixes[=FROM]\n\
         same as -d, but allow setting the start मूल्य\n\
"));
      oputs (_("\
  -x\n\
         use hex suffixes starting at 0, not alphabetic\n\
"));
      oputs (_("\
      --hex-suffixes[=FROM]\n\
         same as -x, but allow setting the start मूल्य\n\
"));
      oputs (_("\
  -e, --elide-empty-files\n\
         करा not generate empty output files with '-n'\n\
"));
      oputs (_("\
      --filter=COMMAND\n\
         लिख to shell COMMAND; file name is $खाता\n\
"));
      oputs (_("\
  -l, --ओळी=संख्या_2\n\
         put संख्या_2 ओळी/records per output file\n\
"));
      oputs (_("\
  -n, --संख्या=CHUNKS\n\
         generate CHUNKS output files; see explanation below\n\
"));
      oputs (_("\
  -t, --separator=SEP\n\
         use SEP instead of newline as the record separator;\n\
         '\\0' (zero) specifies the NUL character\n\
"));
      oputs (_("\
  -u, --unbuffered\n\
         immediately copy आदान to output with '-n r/...'\n\
"));
      oputs (_("\
      --verbose\n\
         print a diagnostic just before each output file is opened\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_size_note ();
      ख_माला_दो (_("\n\
CHUNKS may be:\n\
  N       split into N files based on size of आदान\n\
  K/N     output Kth of N to standard output\n\
  l/N     split into N files without splitting ओळी/records\n\
  l/K/N   output Kth of N to standard output without splitting ओळी/records\n\
  r/N     like 'l' but use round robin distribution\n\
  r/K/N   likewise but only output Kth of N to standard output\n\
"), मानक_निकास);
      ख_माला_दो (_("\n\
-n (except -nr) will buffer to $TMPDIR, defaulting to /tmp,\n\
जर the आदान size cannot easily be determined.\n\
"), मानक_निकास);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

/* Copy the data in FD to a temporary file, then make that file FD.
   Use BUF, of size BUFSIZE, to copy.  Return the number of
   bytes copied, or -1 (setting errno) on error.  */
स्थिर off_t
copy_to_tmpfile (पूर्णांक fd, अक्षर *buf, idx_t bufsize)
{
  खाता *tmp;
  जर (!temp_stream (&tmp, रिक्त))
    परत -1;
  off_t copied = 0;
  off_t r;

  पर्यंत (0 < (r = पठ (fd, buf, bufsize)))
    {
      जर (ख_डालो (buf, 1, r, tmp) != r)
        परत -1;
      जर (ckd_add (&copied, copied, r))
        {
          त्रुटि_सं = EOVERFLOW;
          परत -1;
        }
    }

  जर (r < 0)
    परत r;
  r = dup2 (fileno (tmp), fd);
  जर (r < 0)
    परत r;
  जर (ख_बंद (tmp) < 0)
    परत -1;
  परत copied;
}

/* Return the number of bytes that can be read from FD with status ST.
   Store up to the first BUFSIZE bytes of the file's data into BUF,
   and advance the file position by the number of bytes read.  On
   input error, set errno and return -1.  */

स्थिर off_t
आदान_file_size (पूर्णांक fd, संरचना stat स्थिरांक *st, अक्षर *buf, idx_t bufsize)
{
  off_t size = 0;
  करा
    {
      ssize_t n_read = पठ (fd, buf + size, bufsize - size);
      जर (n_read <= 0)
        परत n_read < 0 ? n_read : size;
      size += n_read;
    }
  पर्यंत (size < bufsize);

  off_t cur, end;
  जर ((usable_st_size (st) && st->st_size < size)
      || (cur = lseek (fd, 0, प्रस्तुत_से)) < 0
      || cur < size /* E.g., /dev/zero on GNU/Linux.  */
      || (end = lseek (fd, 0, अंत_से)) < 0)
    {
      अक्षर *tmpbuf = xmalloc (bufsize);
      end = copy_to_tmpfile (fd, tmpbuf, bufsize);
      मुक्त (tmpbuf);
      जर (end < 0)
        परत end;
      cur = 0;
    }

  जर (end == OFF_T_MAX /* E.g., /dev/zero on GNU/Hurd.  */
      || (cur < end && ckd_add (&size, size, end - cur)))
    {
      त्रुटि_सं = EOVERFLOW;
      परत -1;
    }

  जर (cur < end)
    {
      off_t r = lseek (fd, cur, शुरू_से);
      जर (r < 0)
        परत r;
    }

  परत size;
}

/* Compute the next sequential output file name and store it into the
   string 'outfile'.  */

स्थिर शून्य
next_file_name (शून्य)
{
  /* Index in suffix_alphabet of each character in the suffix.  */
  स्थिर idx_t *sufindex;
  स्थिर idx_t outbase_length;
  स्थिर idx_t outfile_length;
  स्थिर idx_t addsuf_length;

  जर (! outfile)
    {
      bool overflow, widen;

new_name:
      widen = !! outfile_length;

      जर (! widen)
        {
          /* Allocate and initialize the first file name.  */

          outbase_length = म_माप (outbase);
          addsuf_length = additional_suffix ? म_माप (additional_suffix) : 0;
          overflow = ckd_add (&outfile_length, outbase_length + addsuf_length,
                              suffix_length);
        }
      अन्यथा
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
      जर (overflow)
        xalloc_die ();
      outfile = xirealloc (outfile, outfile_size);

      जर (! widen)
        स_नकल (outfile, outbase, outbase_length);
      अन्यथा
        {
          /* Append the last alphabet character to the file name prefix.  */
          outfile[outbase_length] = suffix_alphabet[sufindex[0]];
          outbase_length++;
        }

      outfile_mid = outfile + outbase_length;
      स_रखो (outfile_mid, suffix_alphabet[0], suffix_length);
      जर (additional_suffix)
        स_नकल (outfile_mid + suffix_length, additional_suffix, addsuf_length);
      outfile[outfile_length] = 0;

      मुक्त (sufindex);
      sufindex = xicalloc (suffix_length, आकार *sufindex);

      जर (numeric_suffix_start)
        {
          affirm (! widen);

          /* Update the output file name.  */
          idx_t i = म_माप (numeric_suffix_start);
          स_नकल (outfile_mid + suffix_length - i, numeric_suffix_start, i);

          /* Update the suffix index.  */
          idx_t *sufindex_end = sufindex + suffix_length;
          पर्यंत (i-- != 0)
            *--sufindex_end = numeric_suffix_start[i] - '0';
        }

#जर ! _POSIX_NO_TRUNC && HAVE_PATHCONF && defined _PC_NAME_MAX
      /* POSIX requires that if the output file name is too long for
         its directory, 'split' must fail without creating any files.
         This must be checked for explicitly on operating systems that
         silently truncate file names.  */
      {
        अक्षर *dir = dir_name (outfile);
        दीर्घ name_max = pathconf (dir, _PC_NAME_MAX);
        जर (0 <= name_max && name_max < base_len (last_component (outfile)))
          error (निकास_त्रुटि, ENAMETOOLONG, "%s", quotef (outfile));
        मुक्त (dir);
      }
#endif
    }
  अन्यथा
    {
      /* Increment the suffix in place, if possible.  */

      idx_t i = suffix_length;
      पर्यंत (i-- != 0)
        {
          sufindex[i]++;
          जर (suffix_auto && i == 0 && ! suffix_alphabet[sufindex[0] + 1])
            जा new_name;
          outfile_mid[i] = suffix_alphabet[sufindex[i]];
          जर (outfile_mid[i])
            परत;
          sufindex[i] = 0;
          outfile_mid[i] = suffix_alphabet[sufindex[i]];
        }
      error (निकास_त्रुटि, 0, _("output file suffixes exhausted"));
    }
}

/* Create or truncate a file.  */

स्थिर पूर्णांक
create (अक्षर स्थिरांक *name)
{
  जर (!filter_command)
    {
      जर (verbose)
        ख_लिखो (मानक_निकास, _("creating file %s\n"), quoteaf (name));

      पूर्णांक oflags = O_WRONLY | O_CREAT | O_द्विमान;
      पूर्णांक fd = खोलो (name, oflags | O_EXCL, MODE_RW_UGO);
      जर (0 <= fd || त्रुटि_सं != EEXIST)
        परत fd;
      fd = खोलो (name, oflags, MODE_RW_UGO);
      जर (fd < 0)
        परत fd;
      संरचना stat out_stat_buf;
      जर (ख_स्थिति (fd, &out_stat_buf) != 0)
        error (निकास_त्रुटि, त्रुटि_सं, _("failed to stat %s"), quoteaf (name));
      जर (psame_inode (&in_stat_buf, &out_stat_buf))
        error (निकास_त्रुटि, 0, _("%s would overwrite input; aborting"),
               quoteaf (name));
      जर (ftruncate (fd, 0) < 0
          && (S_ISREG (out_stat_buf.st_mode) || S_TYPEISSHM (&out_stat_buf)))
        error (निकास_त्रुटि, त्रुटि_सं, _("%s: error truncating"), quotef (name));

      परत fd;
    }
  अन्यथा
    {
      जर (setenv ("FILE", name, 1) != 0)
        error (निकास_त्रुटि, त्रुटि_सं,
               _("failed to set FILE environment variable"));
      जर (verbose)
        ख_लिखो (मानक_निकास, _("executing with FILE=%s\n"), quotef (name));

      पूर्णांक result;
      पूर्णांक fd_pair[2];
      pid_t child_pid;

      posix_spawnattr_t attr;
      posix_spawn_file_actions_t actions;

      sigset_t set;
      sigemptyset (&set);
      जर (default_SIGPIPE)
        sigaddset (&set, SIGPIPE);

      जर (   (result = posix_spawnattr_init (&attr))
          || (result = posix_spawnattr_setflags (&attr,
                                                 (POSIX_SPAWN_USEVFORK
                                                  | POSIX_SPAWN_SETSIGDEF)))
          || (result = posix_spawnattr_setsigdefault (&attr, &set))
          || (result = posix_spawn_file_actions_init (&actions))
         )
        error (निकास_त्रुटि, result, _("posix_spawn initialization failed"));

      जर (pipe (fd_pair) != 0)
        error (निकास_त्रुटि, त्रुटि_सं, _("failed to create pipe"));

      /* We have to close any pipes that were opened during an
         earlier call, otherwise this process will be holding a
         write-pipe that will prevent the earlier process from
         reading an EOF on the corresponding read-pipe.  */
      साठी (पूर्णांक i = 0; i < open_pipes.n_pipes; ++i)
        जर ((result = posix_spawn_file_actions_addclose (&actions,
                                                         open_pipes.pipes[i])))
          थांबा;

      जर (   result
          || (result = posix_spawn_file_actions_addclose (&actions, fd_pair[1]))
          || (fd_pair[0] != STDIN_FILENO
              && (   (result = posix_spawn_file_actions_adddup2 (&actions,
                                                                 fd_pair[0],
                                                                 STDIN_FILENO))
                  || (result = posix_spawn_file_actions_addclose (&actions,
                                                                  fd_pair[0]))))
         )
        error (निकास_त्रुटि, result, _("posix_spawn setup failed"));


      अक्षर स्थिरांक *shell_prog = दो_पर्या ("SHELL");
      जर (shell_prog == रिक्त)
        shell_prog = "/bin/sh";
      अक्षर स्थिरांक *स्थिरांक argv[] = { last_component (shell_prog), "-c",
                                   filter_command, रिक्त };

      result = posix_spawn (&child_pid, shell_prog, &actions, &attr,
                            (अक्षर * स्थिरांक *) argv, environ);
      जर (result != 0)
        error (निकास_त्रुटि, त्रुटि_सं, _("failed to run command: \"%s -c %s\""),
               shell_prog, filter_command);

      posix_spawnattr_destroy (&attr);
      posix_spawn_file_actions_destroy (&actions);

      जर (बंद (fd_pair[0]) != 0)
        error (निकास_त्रुटि, त्रुटि_सं, _("failed to close input pipe"));
      filter_pid = child_pid;
      जर (open_pipes.n_pipes == open_pipes.n_pipes_alloc)
        open_pipes.pipes = xpalloc (open_pipes.pipes, &open_pipes.n_pipes_alloc,
                                    1, MIN (पूर्णांक_उच्च, IDX_MAX),
                                    आकार *open_pipes.pipes);
      idx_t स्थिरांक pipe_idx = open_pipes.n_pipes++;
      open_pipes.pipes[pipe_idx] = fd_pair[1];
      परत fd_pair[1];
    }
}

/* Close the output file, and do any associated cleanup.
   If FP and FD are both specified, they refer to the same open file;
   in this case FP is closed, but FD is still used in cleanup.  */
स्थिर शून्य
closeout (खाता *fp, पूर्णांक fd, pid_t pid, अक्षर स्थिरांक *name)
{
  जर (fp != रिक्त && ख_बंद (fp) != 0 && ! ignorable (त्रुटि_सं))
    error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (name));
  जर (fd >= 0)
    {
      जर (fp == रिक्त && बंद (fd) < 0)
        error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (name));
      साठी (पूर्णांक j = 0; j < open_pipes.n_pipes; ++j)
        {
          जर (open_pipes.pipes[j] == fd)
            {
              idx_t स्थिरांक last_idx = open_pipes.n_pipes - 1;
              open_pipes.pipes[j] = open_pipes.pipes[last_idx];
              open_pipes.n_pipes--;
              थांबा;
            }
        }
    }
  जर (pid > 0)
    {
      पूर्णांक wstatus;
      जर (waitpid (pid, &wstatus, 0) < 0)
        error (निकास_त्रुटि, त्रुटि_सं, _("waiting for child process"));
      अन्यथा जर (WIFSIGNALED (wstatus))
        {
          पूर्णांक sig = WTERMSIG (wstatus);
          जर (sig != SIGPIPE)
            {
              अक्षर signame[MAX (SIG2STR_MAX, INT_BUFSIZE_BOUND (पूर्णांक))];
              जर (sig2str (sig, signame) != 0)
                प्र_लिखो (signame, "%d", sig);
              error (sig + 128, 0,
                     _("with FILE=%s, signal %s from command: %s"),
                     quotef (name), signame, filter_command);
            }
        }
      अन्यथा जर (WIFEXITED (wstatus))
        {
          पूर्णांक ex = WEXITSTATUS (wstatus);
          जर (ex != 0)
            error (ex, 0, _("with FILE=%s, exit %d from command: %s"),
                   quotef (name), ex, filter_command);
        }
      अन्यथा
        {
          /* shouldn't happen.  */
          error (निकास_त्रुटि, 0,
                 _("unknown status from command (0x%X)"), wstatus + 0u);
        }
    }
}

/* Write BYTES bytes at BP to an output file.
   If NEW_FILE_FLAG is true, open the next output file.
   Otherwise add to the same output file already in use.
   Return true if successful.  */

स्थिर bool
cwrite (bool new_file_flag, अक्षर स्थिरांक *bp, idx_t bytes)
{
  जर (new_file_flag)
    {
      जर (!bp && bytes == 0 && elide_empty_files)
        परत true;
      closeout (रिक्त, output_desc, filter_pid, outfile);
      next_file_name ();
      output_desc = create (outfile);
      जर (output_desc < 0)
        error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (outfile));
    }

  जर (full_write (output_desc, bp, bytes) == bytes)
    परत true;
  अन्यथा
    {
      जर (! ignorable (त्रुटि_सं))
        error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (outfile));
      परत false;
    }
}

/* Split into pieces of exactly N_BYTES bytes.
   However, the first REM_BYTES pieces should be 1 byte longer.
   Use buffer BUF, whose size is BUFSIZE.
   If INITIAL_READ is nonnegative,
   BUF contains the first INITIAL_READ input bytes.  */

स्थिर शून्य
bytes_split (intmax_t n_bytes, intmax_t rem_bytes,
             अक्षर *buf, idx_t bufsize, ssize_t initial_read,
             intmax_t max_files)
{
  bool new_file_flag = true;
  bool filter_ok = true;
  intmax_t opened = 0;
  intmax_t to_write = n_bytes + (0 < rem_bytes);
  bool eof = ! to_write;

  पर्यंत (! eof)
    {
      ssize_t n_read;
      जर (0 <= initial_read)
        {
          n_read = initial_read;
          initial_read = -1;
          eof = n_read < bufsize;
        }
      अन्यथा
        {
          जर (! filter_ok
              && 0 <= lseek (STDIN_FILENO, to_write, प्रस्तुत_से))
            {
              to_write = n_bytes + (opened + 1 < rem_bytes);
              new_file_flag = true;
            }

          n_read = पठ (STDIN_FILENO, buf, bufsize);
          जर (n_read < 0)
            error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (infile));
          eof = n_read == 0;
        }
      अक्षर *bp_out = buf;
      पर्यंत (0 < to_write && to_write <= n_read)
        {
          जर (filter_ok || new_file_flag)
            filter_ok = cwrite (new_file_flag, bp_out, to_write);
          opened += new_file_flag;
          new_file_flag = !max_files || (opened < max_files);
          जर (! filter_ok && ! new_file_flag)
            {
              /* If filters no longer accepting input, stop reading.  */
              n_read = 0;
              eof = true;
              थांबा;
            }
          bp_out += to_write;
          n_read -= to_write;
          to_write = n_bytes + (opened < rem_bytes);
        }
      जर (0 < n_read)
        {
          जर (filter_ok || new_file_flag)
            filter_ok = cwrite (new_file_flag, bp_out, n_read);
          opened += new_file_flag;
          new_file_flag = false;
          जर (! filter_ok && opened == max_files)
            {
              /* If filters no longer accepting input, stop reading.  */
              थांबा;
            }
          to_write -= n_read;
        }
    }

  /* Ensure NUMBER files are created, which truncates
     any existing files or notifies any consumers on fifos.
     FIXME: Should we do this before EXIT_FAILURE?  */
  पर्यंत (opened++ < max_files)
    cwrite (true, रिक्त, 0);
}

/* Split into pieces of exactly N_LINES lines.
   Use buffer BUF, whose size is BUFSIZE.  */

स्थिर शून्य
ओळी_split (intmax_t n_ओळी, अक्षर *buf, idx_t bufsize)
{
  ssize_t n_read;
  अक्षर *bp, *bp_out, *eob;
  bool new_file_flag = true;
  intmax_t n = 0;

  करा
    {
      n_read = पठ (STDIN_FILENO, buf, bufsize);
      जर (n_read < 0)
        error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (infile));
      bp = bp_out = buf;
      eob = bp + n_read;
      *eob = eolchar;
      पर्यंत (true)
        {
          bp = rawmemchr (bp, eolchar);
          जर (bp == eob)
            {
              जर (eob != bp_out) /* do not write 0 bytes! */
                {
                  idx_t len = eob - bp_out;
                  cwrite (new_file_flag, bp_out, len);
                  new_file_flag = false;
                }
              थांबा;
            }

          ++bp;
          जर (++n >= n_ओळी)
            {
              cwrite (new_file_flag, bp_out, bp - bp_out);
              bp_out = bp;
              new_file_flag = true;
              n = 0;
            }
        }
    }
  पर्यंत (n_read);
}

/* Split into pieces that are as large as possible while still not more
   than N_BYTES bytes, and are split on line boundaries except
   where lines longer than N_BYTES bytes occur. */

स्थिर शून्य
ओळ_bytes_split (intmax_t n_bytes, अक्षर *buf, idx_t bufsize)
{
  ssize_t n_read;
  intmax_t n_out = 0;      /* for each split.  */
  idx_t n_hold = 0;
  अक्षर *hold = रिक्त;        /* for lines > bufsize.  */
  idx_t hold_size = 0;
  bool split_ओळ = false;  /* Whether a \n was output in a split.  */

  करा
    {
      n_read = पठ (STDIN_FILENO, buf, bufsize);
      जर (n_read < 0)
        error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (infile));
      idx_t n_डावा = n_read;
      अक्षर *sob = buf;
      पर्यंत (n_डावा)
        {
          idx_t split_rest = 0;
          अक्षर *eoc = रिक्त;
          अक्षर *eol;

          /* Determine End Of Chunk and/or End of Line,
             which are used below to select what to write or buffer.  */
          जर (n_bytes - n_out - n_hold <= n_डावा)
            {
              /* Have enough for split.  */
              split_rest = n_bytes - n_out - n_hold;
              eoc = sob + split_rest - 1;
              eol = memrchr (sob, eolchar, split_rest);
            }
          अन्यथा
            eol = memrchr (sob, eolchar, n_डावा);

          /* Output hold space if possible.  */
          जर (n_hold && !(!eol && n_out))
            {
              cwrite (n_out == 0, hold, n_hold);
              n_out += n_hold;
              n_hold = 0;
            }

          /* Output to eol if present.  */
          जर (eol)
            {
              split_ओळ = true;
              idx_t n_write = eol - sob + 1;
              cwrite (n_out == 0, sob, n_write);
              n_out += n_write;
              n_डावा -= n_write;
              sob += n_write;
              जर (eoc)
                split_rest -= n_write;
            }

          /* Output to eoc or eob if possible.  */
          जर (n_डावा && !split_ओळ)
            {
              idx_t n_write = eoc ? split_rest : n_डावा;
              cwrite (n_out == 0, sob, n_write);
              n_out += n_write;
              n_डावा -= n_write;
              sob += n_write;
              जर (eoc)
                split_rest -= n_write;
            }

          /* Update hold if needed.  */
          जर ((eoc && split_rest) || (!eoc && n_डावा))
            {
              idx_t n_buf = eoc ? split_rest : n_डावा;
              जर (hold_size - n_hold < n_buf)
                hold = xpalloc (hold, &hold_size, n_buf - (hold_size - n_hold),
                                -1, आकार *hold);
              स_नकल (hold + n_hold, sob, n_buf);
              n_hold += n_buf;
              n_डावा -= n_buf;
              sob += n_buf;
            }

          /* Reset for new split.  */
          जर (eoc)
            {
              n_out = 0;
              split_ओळ = false;
            }
        }
    }
  पर्यंत (n_read);

  /* Handle no eol at end of file.  */
  जर (n_hold)
    cwrite (n_out == 0, hold, n_hold);

  मुक्त (hold);
}

/* -n l/[K/]N: Write lines to files of approximately file size / N.
   The file is partitioned into file size / N sized portions, with the
   last assigned any excess.  If a line _starts_ within a partition
   it is written completely to the corresponding file.  Since lines
   are not split even if they overlap a partition, the files written
   can be larger or smaller than the partition size, and even empty
   if a line is so long as to completely overlap the partition.  */

स्थिर शून्य
ओळी_chunk_split (intmax_t k, intmax_t n, अक्षर *buf, idx_t bufsize,
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

  जर (k > 1 && 0 < file_size)
    {
      /* Start reading 1 byte before kth chunk of file.  */
      off_t start = (k - 1) * chunk_size + MIN (k - 1, rem_bytes) - 1;
      जर (start < initial_read)
        {
          स_हटाओ (buf, buf + start, initial_read - start);
          initial_read -= start;
        }
      अन्यथा
        {
          जर (initial_read < start
              && lseek (STDIN_FILENO, start - initial_read, प्रस्तुत_से) < 0)
            error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (infile));
          initial_read = -1;
        }
      n_written = start;
      chunk_no = k - 1;
      chunk_end = start + 1;
    }

  पर्यंत (n_written < file_size)
    {
      अक्षर *bp = buf, *eob;
      ssize_t n_read;
      जर (0 <= initial_read)
        {
          n_read = initial_read;
          initial_read = -1;
        }
      अन्यथा
        {
          n_read = पठ (STDIN_FILENO, buf,
                         MIN (bufsize, file_size - n_written));
          जर (n_read < 0)
            error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (infile));
        }
      जर (n_read == 0)
        थांबा; /* eof.  */
      chunk_truncated = false;
      eob = buf + n_read;

      पर्यंत (bp != eob)
        {
          idx_t to_write;
          bool next = false;

          /* Begin looking for '\n' at last byte of chunk.  */
          off_t skip = MIN (n_read, MAX (0, chunk_end - 1 - n_written));
          अक्षर *bp_out = स_प्रथम (bp + skip, eolchar, n_read - skip);
          जर (bp_out)
            {
              bp_out++;
              next = true;
            }
          अन्यथा
            bp_out = eob;
          to_write = bp_out - bp;

          जर (k == chunk_no)
            {
              /* We don't use the stdout buffer here since we're writing
                 large chunks from an existing file, so it's more efficient
                 to write out directly.  */
              जर (full_write (STDOUT_FILENO, bp, to_write) != to_write)
                write_error ();
            }
          अन्यथा जर (! k)
            cwrite (new_file_flag, bp, to_write);
          n_written += to_write;
          bp += to_write;
          n_read -= to_write;
          new_file_flag = next;

          /* A line could have been so long that it skipped
             entire chunks. So create empty files in that case.  */
          पर्यंत (next || chunk_end <= n_written)
            {
              जर (!next && bp == eob)
                {
                  /* replenish buf, before going to next chunk.  */
                  chunk_truncated = true;
                  थांबा;
                }
              जर (k == chunk_no)
                परत;
              chunk_end += chunk_size + (chunk_no < rem_bytes);
              chunk_no++;
              जर (chunk_end <= n_written)
                {
                  जर (! k)
                    cwrite (true, रिक्त, 0);
                }
              अन्यथा
                next = false;
            }
        }
    }

  जर (chunk_truncated)
    chunk_no++;

  /* Ensure NUMBER files are created, which truncates
     any existing files or notifies any consumers on fifos.
     FIXME: Should we do this before EXIT_FAILURE?  */
  जर (!k)
    पर्यंत (chunk_no++ <= n)
      cwrite (true, रिक्त, 0);
}

/* -n K/N: Extract Kth of N chunks.  */

स्थिर शून्य
bytes_chunk_extract (intmax_t k, intmax_t n, अक्षर *buf, idx_t bufsize,
                     ssize_t initial_read, off_t file_size)
{
  off_t start;
  off_t end;

  affirm (0 < k && k <= n);

  start = (k - 1) * (file_size / n) + MIN (k - 1, file_size % n);
  end = k == n ? file_size : k * (file_size / n) + MIN (k, file_size % n);

  जर (start < initial_read)
    {
      स_हटाओ (buf, buf + start, initial_read - start);
      initial_read -= start;
    }
  अन्यथा
    {
      जर (initial_read < start
          && lseek (STDIN_FILENO, start - initial_read, प्रस्तुत_से) < 0)
        error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (infile));
      initial_read = -1;
    }

  पर्यंत (start < end)
    {
      ssize_t n_read;
      जर (0 <= initial_read)
        {
          n_read = initial_read;
          initial_read = -1;
        }
      अन्यथा
        {
          n_read = पठ (STDIN_FILENO, buf, bufsize);
          जर (n_read < 0)
            error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (infile));
        }
      जर (n_read == 0)
        थांबा; /* eof.  */
      n_read = MIN (n_read, end - start);
      जर (full_write (STDOUT_FILENO, buf, n_read) != n_read
          && ! ignorable (त्रुटि_सं))
        error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef ("-"));
      start += n_read;
    }
}

प्रकारव्याख्या संरचना of_info
{
  अक्षर *of_name;
  पूर्णांक ofd;
  खाता *ofile;
  pid_t opid;
} of_t;

गणना
{
  OFD_NEW = -1,
  OFD_APPEND = -2
};

/* Rotate file descriptors when we're writing to more output files than we
   have available file descriptors.
   Return whether we came under file resource pressure.
   If so, it's probably best to close each file when finished with it.  */

स्थिर bool
ofile_open (of_t *files, idx_t i_check, idx_t nfiles)
{
  bool file_limit = false;

  जर (files[i_check].ofd <= OFD_NEW)
    {
      पूर्णांक fd;
      idx_t i_reopen = i_check ? i_check - 1 : nfiles - 1;

      /* Another process could have opened a file in between the calls to
         close and open, so we should keep trying until open succeeds or
         we've closed all of our files.  */
      पर्यंत (true)
        {
          जर (files[i_check].ofd == OFD_NEW)
            fd = create (files[i_check].of_name);
          अन्यथा /* OFD_APPEND  */
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
              fd = खोलो (files[i_check].of_name,
                         O_WRONLY | O_द्विमान | O_APPEND | O_NONBLOCK);
            }

          जर (0 <= fd)
            थांबा;

          जर (!(त्रुटि_सं == EMFILE || त्रुटि_सं == ENFILE))
            error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (files[i_check].of_name));

          file_limit = true;

          /* Search backwards for an open file to close.  */
          पर्यंत (files[i_reopen].ofd < 0)
            {
              i_reopen = i_reopen ? i_reopen - 1 : nfiles - 1;
              /* No more open files to close, exit with E[NM]FILE.  */
              जर (i_reopen == i_check)
                error (निकास_त्रुटि, त्रुटि_सं, "%s",
                       quotef (files[i_check].of_name));
            }

          जर (ख_बंद (files[i_reopen].ofile) != 0)
            error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (files[i_reopen].of_name));
          files[i_reopen].ofile = रिक्त;
          files[i_reopen].ofd = OFD_APPEND;
        }

      files[i_check].ofd = fd;
      खाता *ofile = fdopen (fd, "a");
      जर (!ofile)
        error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (files[i_check].of_name));
      files[i_check].ofile = ofile;
      files[i_check].opid = filter_pid;
      filter_pid = 0;
    }

  परत file_limit;
}

/* -n r/[K/]N: Divide file into N chunks in round robin fashion.
   Use BUF of size BUFSIZE for the buffer, and if allocating storage
   put its address into *FILESP to pacify -fsanitize=leak.
   When K == 0, we try to keep the files open in parallel.
   If we run out of file resources, then we revert
   to opening and closing each file for each line.  */

स्थिर शून्य
ओळी_rr (intmax_t k, intmax_t n, अक्षर *buf, idx_t bufsize, of_t **filesp)
{
  bool wrapped = false;
  bool wrote = false;
  bool file_limit;
  idx_t i_file;
  of_t *files IF_LINT (= रिक्त);
  intmax_t ओळ_no;

  जर (k)
    ओळ_no = 1;
  अन्यथा
    {
      जर (IDX_MAX < n)
        xalloc_die ();
      files = *filesp = xinmalloc (n, आकार *files);

      /* Generate output file names. */
      साठी (i_file = 0; i_file < n; i_file++)
        {
          next_file_name ();
          files[i_file].of_name = xstrdup (outfile);
          files[i_file].ofd = OFD_NEW;
          files[i_file].ofile = रिक्त;
          files[i_file].opid = 0;
        }
      i_file = 0;
      file_limit = false;
    }

  पर्यंत (true)
    {
      अक्षर *bp = buf, *eob;
      ssize_t n_read = पठ (STDIN_FILENO, buf, bufsize);
      जर (n_read < 0)
        error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (infile));
      अन्यथा जर (n_read == 0)
        थांबा; /* eof.  */
      eob = buf + n_read;

      पर्यंत (bp != eob)
        {
          idx_t to_write;
          bool next = false;

          /* Find end of line. */
          अक्षर *bp_out = स_प्रथम (bp, eolchar, eob - bp);
          जर (bp_out)
            {
              bp_out++;
              next = true;
            }
          अन्यथा
            bp_out = eob;
          to_write = bp_out - bp;

          जर (k)
            {
              जर (ओळ_no == k && unbuffered)
                {
                  जर (full_write (STDOUT_FILENO, bp, to_write) != to_write)
                    write_error ();
                }
              अन्यथा जर (ओळ_no == k && ख_डालो (bp, to_write, 1, मानक_निकास) != 1)
                {
                  write_error ();
                }
              जर (next)
                ओळ_no = (ओळ_no == n) ? 1 : ओळ_no + 1;
            }
          अन्यथा
            {
              /* Secure file descriptor. */
              file_limit |= ofile_open (files, i_file, n);
              जर (unbuffered)
                {
                  /* Note writing to fd, rather than flushing the FILE gives
                     an 8% performance benefit, due to reduced data copying.  */
                  जर (full_write (files[i_file].ofd, bp, to_write) != to_write
                      && ! ignorable (त्रुटि_सं))
                    error (निकास_त्रुटि, त्रुटि_सं, "%s",
                           quotef (files[i_file].of_name));
                }
              अन्यथा जर (ख_डालो (bp, to_write, 1, files[i_file].ofile) != 1
                       && ! ignorable (त्रुटि_सं))
                error (निकास_त्रुटि, त्रुटि_सं, "%s",
                       quotef (files[i_file].of_name));

              जर (! ignorable (त्रुटि_सं))
                wrote = true;

              जर (file_limit)
                {
                  जर (ख_बंद (files[i_file].ofile) != 0)
                    error (निकास_त्रुटि, त्रुटि_सं, "%s",
                           quotef (files[i_file].of_name));
                  files[i_file].ofile = रिक्त;
                  files[i_file].ofd = OFD_APPEND;
                }
              जर (next && ++i_file == n)
                {
                  wrapped = true;
                  /* If no filters are accepting input, stop reading.  */
                  जर (! wrote)
                    जा no_filters;
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
  जर (!k)
    {
      idx_t ceiling = wrapped ? n : i_file;
      साठी (i_file = 0; i_file < n; i_file++)
        {
          जर (i_file >= ceiling && !elide_empty_files)
            file_limit |= ofile_open (files, i_file, n);
          जर (files[i_file].ofd >= 0)
            closeout (files[i_file].ofile, files[i_file].ofd,
                      files[i_file].opid, files[i_file].of_name);
          files[i_file].ofd = OFD_APPEND;
        }
    }
}

#define FAIL_ONLY_ONE_WAY()					\
  करा								\
    {								\
      error (0, 0, _("cannot split in more than one way"));	\
      usage (निकास_त्रुटि);					\
    }								\
  पर्यंत (0)

/* Report a string-to-integer conversion failure MSGID with ARG.  */

स्थिर _Noreturn शून्य
strtoint_die (अक्षर स्थिरांक *msgid, अक्षर स्थिरांक *arg)
{
  error (निकास_त्रुटि, त्रुटि_सं == EINVAL ? 0 : त्रुटि_सं, "%s: %s",
         gettext (msgid), quote (arg));
}

/* Use OVERFLOW_OK when it is OK to ignore LONGINT_OVERFLOW errors, since the
   extreme value will do the right thing anyway on any practical platform.  */
#define OVERFLOW_OK LONGINT_OVERFLOW

/* Parse ARG for number of bytes or lines.  The number can be followed
   by MULTIPLIERS, and the resulting value must be positive.
   If the number cannot be parsed, diagnose with MSG.
   Return the number parsed, or an INTMAX_MAX on overflow.  */

स्थिर intmax_t
parse_n_units (अक्षर स्थिरांक *arg, अक्षर स्थिरांक *multipliers, अक्षर स्थिरांक *msgid)
{
  intmax_t n;
  जर (OVERFLOW_OK < xstrtoimax (arg, रिक्त, 10, &n, multipliers) || n < 1)
    strtoint_die (msgid, arg);
  परत n;
}

/* Parse K/N syntax of chunk options.  */

स्थिर शून्य
parse_chunk (intmax_t *k_units, intmax_t *n_units, अक्षर स्थिरांक *arg)
{
  अक्षर *argend;
  strtol_error e = xstrtoimax (arg, &argend, 10, n_units, "");
  जर (e == LONGINT_INVALID_SUFFIX_CHAR && *argend == '/')
    {
      *k_units = *n_units;
      *n_units = parse_n_units (argend + 1, "",
                                N_("invalid number of chunks"));
      जर (! (0 < *k_units && *k_units <= *n_units))
        error (निकास_त्रुटि, 0, "%s: %s", _("invalid chunk number"),
               quote_mem (arg, argend - arg));
    }
  अन्यथा जर (! (e <= OVERFLOW_OK && 0 < *n_units))
    strtoint_die (N_("invalid number of chunks"), arg);
}


पूर्णांक
main (पूर्णांक argc, अक्षर **argv)
{
  गणना Split_type split_type = type_undef;
  idx_t in_blk_size = 0;	/* optimal block size of input file device */
  idx_t page_size = getpagesize ();
  intmax_t k_units = 0;
  intmax_t n_units = 0;

  स्थिर अक्षर स्थिरांक multipliers[] = "bEGKkMmPQRTYZ0";
  पूर्णांक c;
  पूर्णांक digits_optind = 0;
  off_t file_size = OFF_T_MAX;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  /* Parse command line options.  */

  पर्यंत (true)
    {
      /* This is the argv-index of the option we will read next.  */
      पूर्णांक this_optind = optind ? optind : 1;

      c = getopt_long (argc, argv, "0123456789C:a:b:del:n:t:ux",
                       longopts, रिक्त);
      जर (c == -1)
        थांबा;

      निवड (c)
        {
        स्थिती 'a':
          suffix_length = xdectoimax (optarg, 0, IDX_MAX,
                                      "", _("invalid suffix length"), 0);
          थांबा;

        स्थिती ADDITIONAL_SUFFIX_OPTION:
          {
            पूर्णांक suffix_len = म_माप (optarg);
            जर (last_component (optarg) != optarg
                || (suffix_len && ISSLASH (optarg[suffix_len - 1])))
              {
                error (0, 0,
                       _("invalid suffix %s, contains directory separator"),
                       quote (optarg));
                usage (निकास_त्रुटि);
              }
          }
          additional_suffix = optarg;
          थांबा;

        स्थिती 'b':
          जर (split_type != type_undef)
            FAIL_ONLY_ONE_WAY ();
          split_type = type_bytes;
          n_units = parse_n_units (optarg, multipliers,
                                   N_("invalid number of bytes"));
          थांबा;

        स्थिती 'l':
          जर (split_type != type_undef)
            FAIL_ONLY_ONE_WAY ();
          split_type = type_ओळी;
          n_units = parse_n_units (optarg, "", N_("invalid number of lines"));
          थांबा;

        स्थिती 'C':
          जर (split_type != type_undef)
            FAIL_ONLY_ONE_WAY ();
          split_type = type_byteslines;
          n_units = parse_n_units (optarg, multipliers,
                                   N_("invalid number of lines"));
          थांबा;

        स्थिती 'n':
          जर (split_type != type_undef)
            FAIL_ONLY_ONE_WAY ();
          /* skip any whitespace */
          पर्यंत (है_खाली (to_uchar (*optarg)))
            optarg++;
          जर (STRNCMP_LIT (optarg, "r/") == 0)
            {
              split_type = type_rr;
              optarg += 2;
            }
          अन्यथा जर (STRNCMP_LIT (optarg, "l/") == 0)
            {
              split_type = type_chunk_ओळी;
              optarg += 2;
            }
          अन्यथा
            split_type = type_chunk_bytes;
          parse_chunk (&k_units, &n_units, optarg);
          थांबा;

        स्थिती 'u':
          unbuffered = true;
          थांबा;

        स्थिती 't':
          {
            अक्षर neweol = optarg[0];
            जर (! neweol)
              error (निकास_त्रुटि, 0, _("empty record separator"));
            जर (optarg[1])
              {
                जर (streq (optarg, "\\0"))
                  neweol = '\0';
                अन्यथा
                  {
                    /* Provoke with 'split -txx'.  Complain about
                       "multi-character tab" instead of "multibyte tab", so
                       that the diagnostic's wording does not need to be
                       changed once multibyte characters are supported.  */
                    error (निकास_त्रुटि, 0, _("multi-character separator %s"),
                           quote (optarg));
                  }
              }
            /* Make it explicit we don't support multiple separators.  */
            जर (0 <= eolchar && neweol != eolchar)
              {
                error (निकास_त्रुटि, 0,
                       _("multiple separator characters specified"));
              }

            eolchar = neweol;
          }
          थांबा;

        स्थिती '0':
        स्थिती '1':
        स्थिती '2':
        स्थिती '3':
        स्थिती '4':
        स्थिती '5':
        स्थिती '6':
        स्थिती '7':
        स्थिती '8':
        स्थिती '9':
          जर (split_type == type_undef)
            {
              split_type = type_digits;
              n_units = 0;
            }
          जर (split_type != type_undef && split_type != type_digits)
            FAIL_ONLY_ONE_WAY ();
          जर (digits_optind != 0 && digits_optind != this_optind)
            n_units = 0;	/* More than one number given; ignore other. */
          digits_optind = this_optind;
          जर (ckd_mul (&n_units, n_units, 10)
              || ckd_add (&n_units, n_units, c - '0'))
            n_units = INTMAX_MAX;
          थांबा;

        स्थिती 'd':
        स्थिती 'x':
          जर (c == 'd')
            suffix_alphabet = "0123456789";
          अन्यथा
            suffix_alphabet = "0123456789abcdef";
          जर (optarg)
            {
              जर (म_माप (optarg) != म_अखोज (optarg, suffix_alphabet))
                {
                  error (0, 0,
                         (c == 'd') ?
                           _("%s: invalid start value for numerical suffix") :
                           _("%s: invalid start value for hexadecimal suffix"),
                         quote (optarg));
                  usage (निकास_त्रुटि);
                }
              अन्यथा
                {
                  /* Skip any leading zero.  */
                  पर्यंत (*optarg == '0' && *(optarg + 1) != '\0')
                    optarg++;
                  numeric_suffix_start = optarg;
                }
            }
          थांबा;

        स्थिती 'e':
          elide_empty_files = true;
          थांबा;

        स्थिती FILTER_OPTION:
          filter_command = optarg;
          थांबा;

        स्थिती IO_BLKSIZE_OPTION:
          in_blk_size = xnumtoumax (optarg, 10, 1,
                                    MIN (SYS_BUFSIZE_MAX,
                                         MIN (IDX_MAX, SIZE_MAX) - 1),
                                    multipliers, _("invalid IO block size"),
                                    0, XTOINT_MIN_RANGE);
          थांबा;

        स्थिती VERBOSE_OPTION:
          verbose = true;
          थांबा;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        पूर्वनिर्धारित:
          usage (निकास_त्रुटि);
        }
    }

  जर (k_units != 0 && filter_command)
    {
      error (0, 0, _("--filter does not process a chunk extracted to "
                     "standard output"));
      usage (निकास_त्रुटि);
    }

  /* Handle default case.  */
  जर (split_type == type_undef)
    {
      split_type = type_ओळी;
      n_units = 1000;
    }

  जर (n_units == 0)
    {
      error (0, 0, _("invalid number of lines: %s"), quote ("0"));
      usage (निकास_त्रुटि);
    }

  जर (eolchar < 0)
    eolchar = '\n';

  set_suffix_length (n_units, split_type);

  /* Get out the filename arguments.  */

  जर (optind < argc)
    infile = argv[optind++];

  जर (optind < argc)
    outbase = argv[optind++];

  जर (optind < argc)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind]));
      usage (निकास_त्रुटि);
    }

  /* Check that the suffix length is large enough for the numerical
     suffix start value.  */
  जर (numeric_suffix_start && म_माप (numeric_suffix_start) > suffix_length)
    {
      error (0, 0, _("numerical suffix start value is too large "
                     "for the suffix length"));
      usage (निकास_त्रुटि);
    }

  /* Open the input file.  */
  जर (! streq (infile, "-")
      && fd_reopen (STDIN_FILENO, infile, O_RDONLY, 0) < 0)
    error (निकास_त्रुटि, त्रुटि_सं, _("cannot open %s for reading"),
           quoteaf (infile));

  /* Binary I/O is safer when byte counts are used.  */
  xset_द्विमान_mode (STDIN_FILENO, O_द्विमान);

  /* Advise the kernel of our access pattern.  */
  fdadvise (STDIN_FILENO, 0, 0, FADVISE_SEQUENTIAL);

  /* Get the optimal block size of input device and make a buffer.  */

  जर (ख_स्थिति (STDIN_FILENO, &in_stat_buf) != 0)
    error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (infile));

  जर (in_blk_size == 0)
    {
      in_blk_size = io_blksize (&in_stat_buf);
      जर (SYS_BUFSIZE_MAX < in_blk_size)
        in_blk_size = SYS_BUFSIZE_MAX;
    }

  अक्षर *buf = xalignalloc (page_size, in_blk_size + 1);
  ssize_t initial_read = -1;

  जर (split_type == type_chunk_bytes || split_type == type_chunk_ओळी)
    {
      file_size = आदान_file_size (STDIN_FILENO, &in_stat_buf,
                                   buf, in_blk_size);
      जर (file_size < 0)
        error (निकास_त्रुटि, त्रुटि_सं, _("%s: cannot determine file size"),
               quotef (infile));
      initial_read = MIN (file_size, in_blk_size);
    }

  /* When filtering, closure of one pipe must not terminate the process,
     as there may still be other streams expecting input from us.  */
  जर (filter_command)
    default_SIGPIPE = संकेत (SIGPIPE, संक_छोड़ो) == संक_पूर्व;

  निवड (split_type)
    {
    स्थिती type_digits:
    स्थिती type_ओळी:
      ओळी_split (n_units, buf, in_blk_size);
      थांबा;

    स्थिती type_bytes:
      bytes_split (n_units, 0, buf, in_blk_size, -1, 0);
      थांबा;

    स्थिती type_byteslines:
      ओळ_bytes_split (n_units, buf, in_blk_size);
      थांबा;

    स्थिती type_chunk_bytes:
      जर (k_units == 0)
        bytes_split (file_size / n_units, file_size % n_units,
                     buf, in_blk_size, initial_read, n_units);
      अन्यथा
        bytes_chunk_extract (k_units, n_units, buf, in_blk_size, initial_read,
                             file_size);
      थांबा;

    स्थिती type_chunk_ओळी:
      ओळी_chunk_split (k_units, n_units, buf, in_blk_size, initial_read,
                         file_size);
      थांबा;

    स्थिती type_rr:
      /* Note, this is like 'sed -n ${k}~${n}p' when k > 0,
         but the functionality is provided for symmetry.  */
      {
        of_t *files;
        ओळी_rr (k_units, n_units, buf, in_blk_size, &files);
      }
      थांबा;

    स्थिती type_undef: पूर्वनिर्धारित:
      affirm (false);
    }

  जर (बंद (STDIN_FILENO) != 0)
    error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (infile));
  closeout (रिक्त, output_desc, filter_pid, outfile);

  main_exit (निकास_सफल);
}
