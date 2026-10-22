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
#समावेश <config.h>

#समावेश <stdio.h>
#समावेश <getopt.h>
#समावेश <signal.h>
#समावेश <sys/types.h>
#समावेश <sys/wait.h>
#समावेश <spawn.h>

#समावेश "system.h"
#समावेश "alignalloc.h"
#समावेश "assure.h"
#समावेश "fadvise.h"
#समावेश "fd-reopen.h"
#समावेश "fcntl--.h"
#समावेश "full-write.h"
#समावेश "ioblksize.h"
#समावेश "quote.h"
#समावेश "sig2str.h"
#समावेश "sys-limits.h"
#समावेश "temp-stream.h"
#समावेश "unistd--.h"
#समावेश "xbinary-io.h"
#समावेश "xdectoint.h"
#समावेश "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "split"

#घोषणा AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("Richard M. Stallman")

/* Shell command to filter through, instead of creating files.  */
अटल अक्षर स्थिर *filter_command;

/* Process ID of the filter.  */
अटल pid_t filter_pid;

अटल काष्ठा
{
  /* The number of open pipes.  */
  पूर्णांक n_pipes;

  /* The number of allocated pipes.  */
  idx_t n_pipes_alloc;

  /* Array of open pipes.  */
  पूर्णांक *pipes COUNTED_BY (n_pipes);
} open_pipes;

/* Whether SIGPIPE has the default action, when --filter is used.  */
अटल bool default_SIGPIPE;

/* Base name of output files.  */
अटल अक्षर स्थिर *outbase = "x";

/* Name of output files.  */
अटल अक्षर *outfile;

/* Pointer to the end of the prefix in OUTFILE.
   Suffixes are inserted here.  */
अटल अक्षर *outfile_mid;

/* Generate new suffix when suffixes are exhausted.  */
अटल bool suffix_auto = true;

/* Length of OUTFILE's suffix.  */
अटल idx_t suffix_लंबाई;

/* Alphabet of characters to use in suffix.  */
अटल अक्षर स्थिर *suffix_alphabet = "abcdefghijklmnopqrstuvwxyz";

/* Numerical suffix start value.  */
अटल अक्षर स्थिर *numeric_suffix_start;

/* Additional suffix to append to output file names.  */
अटल अक्षर स्थिर *additional_suffix;

/* Name of input file.  May be "-".  */
अटल अक्षर स्थिर *infile = "-";

/* stat buf for input file.  */
अटल काष्ठा stat in_stat_buf;

/* Descriptor on which output file is open.  */
अटल पूर्णांक output_desc = -1;

/* If true, print a diagnostic on standard error just before each
   output file is opened. */
अटल bool verbose;

/* If true, don't generate zero length output files. */
अटल bool elide_empty_files;

/* If true, in round robin mode, immediately copy
   input to output, which is much slower, so disabled by default.  */
अटल bool unbuffered;

/* The character marking end of line.  Defaults to \n below.  */
अटल पूर्णांक eolchar = -1;

/* The split mode to use.  */
क्रमागत Split_type
{
  type_undef, type_bytes, type_byteslines, type_पंक्तियाँ, type_digits,
  type_chunk_bytes, type_chunk_पंक्तियाँ, type_rr
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
क्रमागत
{
  VERBOSE_OPTION = अक्षर_उच्च + 1,
  FILTER_OPTION,
  IO_BLKSIZE_OPTION,
  ADDITIONAL_SUFFIX_OPTION
};

अटल काष्ठा option स्थिर longopts[] =
{
  {"bytes", required_argument, शून्य, 'b'},
  {"lines", required_argument, शून्य, 'l'},
  {"line-bytes", required_argument, शून्य, 'C'},
  {"number", required_argument, शून्य, 'n'},
  {"elide-empty-files", no_argument, शून्य, 'e'},
  {"unbuffered", no_argument, शून्य, 'u'},
  {"suffix-length", required_argument, शून्य, 'a'},
  {"additional-suffix", required_argument, शून्य,
   ADDITIONAL_SUFFIX_OPTION},
  {"numeric-suffixes", optional_argument, शून्य, 'd'},
  {"hex-suffixes", optional_argument, शून्य, 'x'},
  {"filter", required_argument, शून्य, FILTER_OPTION},
  {"verbose", no_argument, शून्य, VERBOSE_OPTION},
  {"separator", required_argument, शून्य, 't'},
  {"-io-blksize", required_argument, शून्य,
   IO_BLKSIZE_OPTION}, /* do not document */
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {शून्य, 0, शून्य, 0}
};

/* Return true if the errno value, ERR, is ignorable.  */
अटल अंतरभूत bool
ignorable (पूर्णांक err)
{
  वापस filter_command && err == EPIPE;
}

अटल व्योम
set_suffix_लंबाई (intmax_t n_units, क्रमागत Split_type split_type)
{
#घोषणा DEFAULT_SUFFIX_लंबाई 2

  पूर्णांक suffix_लंबाई_needed = 0;

  /* The suffix auto length feature is incompatible with
     a user specified start value as the generated suffixes
     are not all consecutive.  */
  अगर (numeric_suffix_start)
    suffix_auto = false;

  /* Auto-calculate the suffix length if the number of files is given.  */
  अगर (split_type == type_chunk_bytes || split_type == type_chunk_पंक्तियाँ
      || split_type == type_rr)
    {
      intmax_t n_units_end = n_units - 1;
      अगर (numeric_suffix_start)
        {
          intmax_t n_start;
          strtol_error e = xstrtoimax (numeric_suffix_start, शून्य, 10,
                                       &n_start, "");
          अगर (e == LONGINT_OK && n_start < n_units)
            {
              /* Restrict auto adjustment so we don't keep
                 incrementing a suffix size arbitrarily,
                 as that would break sort order for files
                 generated from multiple split runs.  */
              अगर (ckd_add (&n_units_end, n_units_end, n_start))
                n_units_end = INTMAX_MAX;
            }

        }
      idx_t alphabet_len = म_माप (suffix_alphabet);
      करो
        suffix_लंबाई_needed++;
      जबतक (n_units_end /= alphabet_len);

      suffix_auto = false;
    }

  अगर (suffix_लंबाई)            /* set by user */
    {
      अगर (suffix_लंबाई < suffix_लंबाई_needed)
        error (निकास_त्रुटि, 0,
               _("the suffix length needs to be at least %d"),
               suffix_लंबाई_needed);
      suffix_auto = false;
      वापस;
    }
  अन्यथा
    suffix_लंबाई = MAX (DEFAULT_SUFFIX_लंबाई, suffix_लंबाई_needed);
}

व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s [OPTION]... [खाता [PREFIX]]\n\
"),
              program_name);
      ख_माला_दो (_("\
Output pieces of खाता to PREFIXaa, PREFIXab, ...;\n\
शेष आकार is 1000 पंक्तियाँ, and शेष PREFIX is 'x'.\n\
"), मानक_निकास);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oprintf (_("\
  -a, --suffix-लंबाई=N\n\
         generate suffixes of लंबाई N (शेष %d)\n\
"), DEFAULT_SUFFIX_लंबाई);
      oputs (_("\
      --additional-suffix=SUFFIX\n\
         append an additional SUFFIX to file names\n\
"));
      oputs (_("\
  -b, --bytes=आकार_2\n\
         put आकार_2 bytes per output file\n\
"));
      oputs (_("\
  -C, --पंक्ति-bytes=आकार_2\n\
         put at most आकार_2 bytes of records per output file\n\
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
         करो not generate empty output files with '-n'\n\
"));
      oputs (_("\
      --filter=COMMAND\n\
         ग_लिखो to shell COMMAND; file name is $खाता\n\
"));
      oputs (_("\
  -l, --पंक्तियाँ=संख्या_2\n\
         put संख्या_2 पंक्तियाँ/records per output file\n\
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
         immediately copy निवेश to output with '-n r/...'\n\
"));
      oputs (_("\
      --verbose\n\
         print a diagnostic just before each output file is opened\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_आकार_note ();
      ख_माला_दो (_("\n\
CHUNKS may be:\n\
  N       split into N files based on आकार of निवेश\n\
  K/N     output Kth of N to standard output\n\
  l/N     split into N files without splitting पंक्तियाँ/records\n\
  l/K/N   output Kth of N to standard output without splitting पंक्तियाँ/records\n\
  r/N     like 'l' but use round robin distribution\n\
  r/K/N   likewise but only output Kth of N to standard output\n\
"), मानक_निकास);
      ख_माला_दो (_("\n\
-n (except -nr) will बफर to $TMPDIR, defaulting to /tmp,\n\
अगर the निवेश आकार cannot easily be determined.\n\
"), मानक_निकास);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

/* Copy the data in FD to a temporary file, then make that file FD.
   Use BUF, of size BUFSIZE, to copy.  Return the number of
   bytes copied, or -1 (setting errno) on error.  */
अटल off_t
copy_to_tmpfile (पूर्णांक fd, अक्षर *buf, idx_t bufsize)
{
  खाता *tmp;
  अगर (!अस्थायी_stream (&tmp, शून्य))
    वापस -1;
  off_t copied = 0;
  off_t r;

  जबतक (0 < (r = पढ़ो (fd, buf, bufsize)))
    {
      अगर (ख_डालो (buf, 1, r, tmp) != r)
        वापस -1;
      अगर (ckd_add (&copied, copied, r))
        {
          त्रुटि_सं = EOVERFLOW;
          वापस -1;
        }
    }

  अगर (r < 0)
    वापस r;
  r = dup2 (fileno (tmp), fd);
  अगर (r < 0)
    वापस r;
  अगर (ख_बंद (tmp) < 0)
    वापस -1;
  वापस copied;
}

/* Return the number of bytes that can be read from FD with status ST.
   Store up to the first BUFSIZE bytes of the file's data into BUF,
   and advance the file position by the number of bytes read.  On
   input error, set errno and return -1.  */

अटल off_t
निवेश_file_आकार (पूर्णांक fd, काष्ठा stat स्थिर *st, अक्षर *buf, idx_t bufsize)
{
  off_t आकार = 0;
  करो
    {
      ssize_t n_read = पढ़ो (fd, buf + आकार, bufsize - आकार);
      अगर (n_read <= 0)
        वापस n_read < 0 ? n_read : आकार;
      आकार += n_read;
    }
  जबतक (आकार < bufsize);

  off_t cur, end;
  अगर ((usable_st_आकार (st) && st->st_आकार < आकार)
      || (cur = lseek (fd, 0, प्रस्तुत_से)) < 0
      || cur < आकार /* E.g., /dev/zero on GNU/Linux.  */
      || (end = lseek (fd, 0, अंत_से)) < 0)
    {
      अक्षर *tmpbuf = xmalloc (bufsize);
      end = copy_to_tmpfile (fd, tmpbuf, bufsize);
      मुक्त (tmpbuf);
      अगर (end < 0)
        वापस end;
      cur = 0;
    }

  अगर (end == OFF_T_MAX /* E.g., /dev/zero on GNU/Hurd.  */
      || (cur < end && ckd_add (&आकार, आकार, end - cur)))
    {
      त्रुटि_सं = EOVERFLOW;
      वापस -1;
    }

  अगर (cur < end)
    {
      off_t r = lseek (fd, cur, शुरू_से);
      अगर (r < 0)
        वापस r;
    }

  वापस आकार;
}

/* Compute the next sequential output file name and store it into the
   string 'outfile'.  */

अटल व्योम
अग्र_file_name (व्योम)
{
  /* Index in suffix_alphabet of each character in the suffix.  */
  अटल idx_t *sufindex;
  अटल idx_t outbase_लंबाई;
  अटल idx_t outfile_लंबाई;
  अटल idx_t addsuf_लंबाई;

  अगर (! outfile)
    {
      bool overflow, widen;

new_name:
      widen = !! outfile_लंबाई;

      अगर (! widen)
        {
          /* Allocate and initialize the first file name.  */

          outbase_लंबाई = म_माप (outbase);
          addsuf_लंबाई = additional_suffix ? म_माप (additional_suffix) : 0;
          overflow = ckd_add (&outfile_लंबाई, outbase_लंबाई + addsuf_लंबाई,
                              suffix_लंबाई);
        }
      अन्यथा
        {
          /* Reallocate and initialize a new wider file name.
             We do this by subsuming the unchanging part of
             the generated suffix into the prefix (base), and
             reinitializing the now one longer suffix.  */

          overflow = ckd_add (&outfile_लंबाई, outfile_लंबाई, 2);
          suffix_लंबाई++;
        }

      idx_t outfile_आकार;
      overflow |= ckd_add (&outfile_आकार, outfile_लंबाई, 1);
      अगर (overflow)
        xalloc_die ();
      outfile = xirealloc (outfile, outfile_आकार);

      अगर (! widen)
        स_नकल (outfile, outbase, outbase_लंबाई);
      अन्यथा
        {
          /* Append the last alphabet character to the file name prefix.  */
          outfile[outbase_लंबाई] = suffix_alphabet[sufindex[0]];
          outbase_लंबाई++;
        }

      outfile_mid = outfile + outbase_लंबाई;
      स_रखो (outfile_mid, suffix_alphabet[0], suffix_लंबाई);
      अगर (additional_suffix)
        स_नकल (outfile_mid + suffix_लंबाई, additional_suffix, addsuf_लंबाई);
      outfile[outfile_लंबाई] = 0;

      मुक्त (sufindex);
      sufindex = xicalloc (suffix_लंबाई, माप *sufindex);

      अगर (numeric_suffix_start)
        {
          affirm (! widen);

          /* Update the output file name.  */
          idx_t i = म_माप (numeric_suffix_start);
          स_नकल (outfile_mid + suffix_लंबाई - i, numeric_suffix_start, i);

          /* Update the suffix index.  */
          idx_t *sufindex_end = sufindex + suffix_लंबाई;
          जबतक (i-- != 0)
            *--sufindex_end = numeric_suffix_start[i] - '0';
        }

#अगर ! _POSIX_NO_TRUNC && HAVE_PATHCONF && defined _PC_NAME_MAX
      /* POSIX requires that if the output file name is too long for
         its directory, 'split' must fail without creating any files.
         This must be checked for explicitly on operating systems that
         silently truncate file names.  */
      {
        अक्षर *dir = dir_name (outfile);
        दीर्घ name_max = pathconf (dir, _PC_NAME_MAX);
        अगर (0 <= name_max && name_max < base_len (last_component (outfile)))
          error (निकास_त्रुटि, ENAMETOOLONG, "%s", quotef (outfile));
        मुक्त (dir);
      }
#पूर्ण_अगर
    }
  अन्यथा
    {
      /* Increment the suffix in place, if possible.  */

      idx_t i = suffix_लंबाई;
      जबतक (i-- != 0)
        {
          sufindex[i]++;
          अगर (suffix_auto && i == 0 && ! suffix_alphabet[sufindex[0] + 1])
            जाओ new_name;
          outfile_mid[i] = suffix_alphabet[sufindex[i]];
          अगर (outfile_mid[i])
            वापस;
          sufindex[i] = 0;
          outfile_mid[i] = suffix_alphabet[sufindex[i]];
        }
      error (निकास_त्रुटि, 0, _("output file suffixes exhausted"));
    }
}

/* Create or truncate a file.  */

अटल पूर्णांक
create (अक्षर स्थिर *name)
{
  अगर (!filter_command)
    {
      अगर (verbose)
        ख_लिखो (मानक_निकास, _("creating file %s\n"), quoteaf (name));

      पूर्णांक oflags = O_WRONLY | O_CREAT | O_द्विआधारी;
      पूर्णांक fd = खोलो (name, oflags | O_EXCL, MODE_RW_UGO);
      अगर (0 <= fd || त्रुटि_सं != EEXIST)
        वापस fd;
      fd = खोलो (name, oflags, MODE_RW_UGO);
      अगर (fd < 0)
        वापस fd;
      काष्ठा stat out_stat_buf;
      अगर (ख_स्थिति (fd, &out_stat_buf) != 0)
        error (निकास_त्रुटि, त्रुटि_सं, _("failed to stat %s"), quoteaf (name));
      अगर (psame_inode (&in_stat_buf, &out_stat_buf))
        error (निकास_त्रुटि, 0, _("%s would overwrite input; aborting"),
               quoteaf (name));
      अगर (ftruncate (fd, 0) < 0
          && (S_ISREG (out_stat_buf.st_mode) || S_TYPEISSHM (&out_stat_buf)))
        error (निकास_त्रुटि, त्रुटि_सं, _("%s: error truncating"), quotef (name));

      वापस fd;
    }
  अन्यथा
    {
      अगर (setenv ("FILE", name, 1) != 0)
        error (निकास_त्रुटि, त्रुटि_सं,
               _("failed to set FILE environment variable"));
      अगर (verbose)
        ख_लिखो (मानक_निकास, _("executing with FILE=%s\n"), quotef (name));

      पूर्णांक परिणाम;
      पूर्णांक fd_pair[2];
      pid_t child_pid;

      posix_spawnattr_t attr;
      posix_spawn_file_actions_t actions;

      sigset_t set;
      sigemptyset (&set);
      अगर (default_SIGPIPE)
        sigaddset (&set, SIGPIPE);

      अगर (   (परिणाम = posix_spawnattr_init (&attr))
          || (परिणाम = posix_spawnattr_setflags (&attr,
                                                 (POSIX_SPAWN_USEVFORK
                                                  | POSIX_SPAWN_SETSIGDEF)))
          || (परिणाम = posix_spawnattr_setsigdefault (&attr, &set))
          || (परिणाम = posix_spawn_file_actions_init (&actions))
         )
        error (निकास_त्रुटि, परिणाम, _("posix_spawn initialization failed"));

      अगर (pipe (fd_pair) != 0)
        error (निकास_त्रुटि, त्रुटि_सं, _("failed to create pipe"));

      /* We have to close any pipes that were opened during an
         earlier call, otherwise this process will be holding a
         write-pipe that will prevent the earlier process from
         reading an EOF on the corresponding read-pipe.  */
      क्रम (पूर्णांक i = 0; i < open_pipes.n_pipes; ++i)
        अगर ((परिणाम = posix_spawn_file_actions_addclose (&actions,
                                                         open_pipes.pipes[i])))
          अवरोध;

      अगर (   परिणाम
          || (परिणाम = posix_spawn_file_actions_addclose (&actions, fd_pair[1]))
          || (fd_pair[0] != STDIN_FILENO
              && (   (परिणाम = posix_spawn_file_actions_adddup2 (&actions,
                                                                 fd_pair[0],
                                                                 STDIN_FILENO))
                  || (परिणाम = posix_spawn_file_actions_addclose (&actions,
                                                                  fd_pair[0]))))
         )
        error (निकास_त्रुटि, परिणाम, _("posix_spawn setup failed"));


      अक्षर स्थिर *shell_prog = दो_पर्या ("SHELL");
      अगर (shell_prog == शून्य)
        shell_prog = "/bin/sh";
      अक्षर स्थिर *स्थिर argv[] = { last_component (shell_prog), "-c",
                                   filter_command, शून्य };

      परिणाम = posix_spawn (&child_pid, shell_prog, &actions, &attr,
                            (अक्षर * स्थिर *) argv, environ);
      अगर (परिणाम != 0)
        error (निकास_त्रुटि, त्रुटि_सं, _("failed to run command: \"%s -c %s\""),
               shell_prog, filter_command);

      posix_spawnattr_destroy (&attr);
      posix_spawn_file_actions_destroy (&actions);

      अगर (बंद (fd_pair[0]) != 0)
        error (निकास_त्रुटि, त्रुटि_सं, _("failed to close input pipe"));
      filter_pid = child_pid;
      अगर (open_pipes.n_pipes == open_pipes.n_pipes_alloc)
        open_pipes.pipes = xpalloc (open_pipes.pipes, &open_pipes.n_pipes_alloc,
                                    1, MIN (पूर्णांक_उच्च, IDX_MAX),
                                    माप *open_pipes.pipes);
      idx_t स्थिर pipe_idx = open_pipes.n_pipes++;
      open_pipes.pipes[pipe_idx] = fd_pair[1];
      वापस fd_pair[1];
    }
}

/* Close the output file, and do any associated cleanup.
   If FP and FD are both specified, they refer to the same open file;
   in this case FP is closed, but FD is still used in cleanup.  */
अटल व्योम
closeout (खाता *fp, पूर्णांक fd, pid_t pid, अक्षर स्थिर *name)
{
  अगर (fp != शून्य && ख_बंद (fp) != 0 && ! ignorable (त्रुटि_सं))
    error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (name));
  अगर (fd >= 0)
    {
      अगर (fp == शून्य && बंद (fd) < 0)
        error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (name));
      क्रम (पूर्णांक j = 0; j < open_pipes.n_pipes; ++j)
        {
          अगर (open_pipes.pipes[j] == fd)
            {
              idx_t स्थिर last_idx = open_pipes.n_pipes - 1;
              open_pipes.pipes[j] = open_pipes.pipes[last_idx];
              open_pipes.n_pipes--;
              अवरोध;
            }
        }
    }
  अगर (pid > 0)
    {
      पूर्णांक wstatus;
      अगर (waitpid (pid, &wstatus, 0) < 0)
        error (निकास_त्रुटि, त्रुटि_सं, _("waiting for child process"));
      अन्यथा अगर (WIFSIGNALED (wstatus))
        {
          पूर्णांक sig = WTERMSIG (wstatus);
          अगर (sig != SIGPIPE)
            {
              अक्षर signame[MAX (SIG2STR_MAX, INT_BUFSIZE_BOUND (पूर्णांक))];
              अगर (sig2str (sig, signame) != 0)
                प्र_लिखो (signame, "%d", sig);
              error (sig + 128, 0,
                     _("with FILE=%s, signal %s from command: %s"),
                     quotef (name), signame, filter_command);
            }
        }
      अन्यथा अगर (WIFEXITED (wstatus))
        {
          पूर्णांक ex = WEXITSTATUS (wstatus);
          अगर (ex != 0)
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

अटल bool
cwrite (bool new_file_flag, अक्षर स्थिर *bp, idx_t bytes)
{
  अगर (new_file_flag)
    {
      अगर (!bp && bytes == 0 && elide_empty_files)
        वापस true;
      closeout (शून्य, output_desc, filter_pid, outfile);
      अग्र_file_name ();
      output_desc = create (outfile);
      अगर (output_desc < 0)
        error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (outfile));
    }

  अगर (full_write (output_desc, bp, bytes) == bytes)
    वापस true;
  अन्यथा
    {
      अगर (! ignorable (त्रुटि_सं))
        error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (outfile));
      वापस false;
    }
}

/* Split into pieces of exactly N_BYTES bytes.
   However, the first REM_BYTES pieces should be 1 byte longer.
   Use buffer BUF, whose size is BUFSIZE.
   If INITIAL_READ is nonnegative,
   BUF contains the first INITIAL_READ input bytes.  */

अटल व्योम
bytes_split (intmax_t n_bytes, intmax_t rem_bytes,
             अक्षर *buf, idx_t bufsize, ssize_t initial_read,
             intmax_t max_files)
{
  bool new_file_flag = true;
  bool filter_ok = true;
  intmax_t opened = 0;
  intmax_t to_write = n_bytes + (0 < rem_bytes);
  bool eof = ! to_write;

  जबतक (! eof)
    {
      ssize_t n_read;
      अगर (0 <= initial_read)
        {
          n_read = initial_read;
          initial_read = -1;
          eof = n_read < bufsize;
        }
      अन्यथा
        {
          अगर (! filter_ok
              && 0 <= lseek (STDIN_FILENO, to_write, प्रस्तुत_से))
            {
              to_write = n_bytes + (opened + 1 < rem_bytes);
              new_file_flag = true;
            }

          n_read = पढ़ो (STDIN_FILENO, buf, bufsize);
          अगर (n_read < 0)
            error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (infile));
          eof = n_read == 0;
        }
      अक्षर *bp_out = buf;
      जबतक (0 < to_write && to_write <= n_read)
        {
          अगर (filter_ok || new_file_flag)
            filter_ok = cwrite (new_file_flag, bp_out, to_write);
          opened += new_file_flag;
          new_file_flag = !max_files || (opened < max_files);
          अगर (! filter_ok && ! new_file_flag)
            {
              /* If filters no longer accepting input, stop reading.  */
              n_read = 0;
              eof = true;
              अवरोध;
            }
          bp_out += to_write;
          n_read -= to_write;
          to_write = n_bytes + (opened < rem_bytes);
        }
      अगर (0 < n_read)
        {
          अगर (filter_ok || new_file_flag)
            filter_ok = cwrite (new_file_flag, bp_out, n_read);
          opened += new_file_flag;
          new_file_flag = false;
          अगर (! filter_ok && opened == max_files)
            {
              /* If filters no longer accepting input, stop reading.  */
              अवरोध;
            }
          to_write -= n_read;
        }
    }

  /* Ensure NUMBER files are created, which truncates
     any existing files or notifies any consumers on fifos.
     FIXME: Should we do this before EXIT_FAILURE?  */
  जबतक (opened++ < max_files)
    cwrite (true, शून्य, 0);
}

/* Split into pieces of exactly N_LINES lines.
   Use buffer BUF, whose size is BUFSIZE.  */

अटल व्योम
पंक्तियाँ_split (intmax_t n_पंक्तियाँ, अक्षर *buf, idx_t bufsize)
{
  ssize_t n_read;
  अक्षर *bp, *bp_out, *eob;
  bool new_file_flag = true;
  intmax_t n = 0;

  करो
    {
      n_read = पढ़ो (STDIN_FILENO, buf, bufsize);
      अगर (n_read < 0)
        error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (infile));
      bp = bp_out = buf;
      eob = bp + n_read;
      *eob = eolchar;
      जबतक (true)
        {
          bp = rawmemchr (bp, eolchar);
          अगर (bp == eob)
            {
              अगर (eob != bp_out) /* do not write 0 bytes! */
                {
                  idx_t len = eob - bp_out;
                  cwrite (new_file_flag, bp_out, len);
                  new_file_flag = false;
                }
              अवरोध;
            }

          ++bp;
          अगर (++n >= n_पंक्तियाँ)
            {
              cwrite (new_file_flag, bp_out, bp - bp_out);
              bp_out = bp;
              new_file_flag = true;
              n = 0;
            }
        }
    }
  जबतक (n_read);
}

/* Split into pieces that are as large as possible while still not more
   than N_BYTES bytes, and are split on line boundaries except
   where lines longer than N_BYTES bytes occur. */

अटल व्योम
पंक्ति_bytes_split (intmax_t n_bytes, अक्षर *buf, idx_t bufsize)
{
  ssize_t n_read;
  intmax_t n_out = 0;      /* for each split.  */
  idx_t n_hold = 0;
  अक्षर *hold = शून्य;        /* for lines > bufsize.  */
  idx_t hold_आकार = 0;
  bool split_पंक्ति = false;  /* Whether a \n was output in a split.  */

  करो
    {
      n_read = पढ़ो (STDIN_FILENO, buf, bufsize);
      अगर (n_read < 0)
        error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (infile));
      idx_t n_बायाँ = n_read;
      अक्षर *sob = buf;
      जबतक (n_बायाँ)
        {
          idx_t split_rest = 0;
          अक्षर *eoc = शून्य;
          अक्षर *eol;

          /* Determine End Of Chunk and/or End of Line,
             which are used below to select what to write or buffer.  */
          अगर (n_bytes - n_out - n_hold <= n_बायाँ)
            {
              /* Have enough for split.  */
              split_rest = n_bytes - n_out - n_hold;
              eoc = sob + split_rest - 1;
              eol = memrchr (sob, eolchar, split_rest);
            }
          अन्यथा
            eol = memrchr (sob, eolchar, n_बायाँ);

          /* Output hold space if possible.  */
          अगर (n_hold && !(!eol && n_out))
            {
              cwrite (n_out == 0, hold, n_hold);
              n_out += n_hold;
              n_hold = 0;
            }

          /* Output to eol if present.  */
          अगर (eol)
            {
              split_पंक्ति = true;
              idx_t n_write = eol - sob + 1;
              cwrite (n_out == 0, sob, n_write);
              n_out += n_write;
              n_बायाँ -= n_write;
              sob += n_write;
              अगर (eoc)
                split_rest -= n_write;
            }

          /* Output to eoc or eob if possible.  */
          अगर (n_बायाँ && !split_पंक्ति)
            {
              idx_t n_write = eoc ? split_rest : n_बायाँ;
              cwrite (n_out == 0, sob, n_write);
              n_out += n_write;
              n_बायाँ -= n_write;
              sob += n_write;
              अगर (eoc)
                split_rest -= n_write;
            }

          /* Update hold if needed.  */
          अगर ((eoc && split_rest) || (!eoc && n_बायाँ))
            {
              idx_t n_buf = eoc ? split_rest : n_बायाँ;
              अगर (hold_आकार - n_hold < n_buf)
                hold = xpalloc (hold, &hold_आकार, n_buf - (hold_आकार - n_hold),
                                -1, माप *hold);
              स_नकल (hold + n_hold, sob, n_buf);
              n_hold += n_buf;
              n_बायाँ -= n_buf;
              sob += n_buf;
            }

          /* Reset for new split.  */
          अगर (eoc)
            {
              n_out = 0;
              split_पंक्ति = false;
            }
        }
    }
  जबतक (n_read);

  /* Handle no eol at end of file.  */
  अगर (n_hold)
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

अटल व्योम
पंक्तियाँ_chunk_split (intmax_t k, intmax_t n, अक्षर *buf, idx_t bufsize,
                   ssize_t initial_read, off_t file_आकार)
{
  affirm (n && k <= n);

  intmax_t rem_bytes = file_आकार % n;
  off_t chunk_आकार = file_आकार / n;
  intmax_t chunk_no = 1;
  off_t chunk_end = chunk_आकार + (0 < rem_bytes);
  off_t n_written = 0;
  bool new_file_flag = true;
  bool chunk_truncated = false;

  अगर (k > 1 && 0 < file_आकार)
    {
      /* Start reading 1 byte before kth chunk of file.  */
      off_t start = (k - 1) * chunk_आकार + MIN (k - 1, rem_bytes) - 1;
      अगर (start < initial_read)
        {
          स_हटाओ (buf, buf + start, initial_read - start);
          initial_read -= start;
        }
      अन्यथा
        {
          अगर (initial_read < start
              && lseek (STDIN_FILENO, start - initial_read, प्रस्तुत_से) < 0)
            error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (infile));
          initial_read = -1;
        }
      n_written = start;
      chunk_no = k - 1;
      chunk_end = start + 1;
    }

  जबतक (n_written < file_आकार)
    {
      अक्षर *bp = buf, *eob;
      ssize_t n_read;
      अगर (0 <= initial_read)
        {
          n_read = initial_read;
          initial_read = -1;
        }
      अन्यथा
        {
          n_read = पढ़ो (STDIN_FILENO, buf,
                         MIN (bufsize, file_आकार - n_written));
          अगर (n_read < 0)
            error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (infile));
        }
      अगर (n_read == 0)
        अवरोध; /* eof.  */
      chunk_truncated = false;
      eob = buf + n_read;

      जबतक (bp != eob)
        {
          idx_t to_write;
          bool अग्र = false;

          /* Begin looking for '\n' at last byte of chunk.  */
          off_t skip = MIN (n_read, MAX (0, chunk_end - 1 - n_written));
          अक्षर *bp_out = स_प्रथम (bp + skip, eolchar, n_read - skip);
          अगर (bp_out)
            {
              bp_out++;
              अग्र = true;
            }
          अन्यथा
            bp_out = eob;
          to_write = bp_out - bp;

          अगर (k == chunk_no)
            {
              /* We don't use the stdout buffer here since we're writing
                 large chunks from an existing file, so it's more efficient
                 to write out directly.  */
              अगर (full_write (STDOUT_FILENO, bp, to_write) != to_write)
                write_error ();
            }
          अन्यथा अगर (! k)
            cwrite (new_file_flag, bp, to_write);
          n_written += to_write;
          bp += to_write;
          n_read -= to_write;
          new_file_flag = अग्र;

          /* A line could have been so long that it skipped
             entire chunks. So create empty files in that case.  */
          जबतक (अग्र || chunk_end <= n_written)
            {
              अगर (!अग्र && bp == eob)
                {
                  /* replenish buf, before going to next chunk.  */
                  chunk_truncated = true;
                  अवरोध;
                }
              अगर (k == chunk_no)
                वापस;
              chunk_end += chunk_आकार + (chunk_no < rem_bytes);
              chunk_no++;
              अगर (chunk_end <= n_written)
                {
                  अगर (! k)
                    cwrite (true, शून्य, 0);
                }
              अन्यथा
                अग्र = false;
            }
        }
    }

  अगर (chunk_truncated)
    chunk_no++;

  /* Ensure NUMBER files are created, which truncates
     any existing files or notifies any consumers on fifos.
     FIXME: Should we do this before EXIT_FAILURE?  */
  अगर (!k)
    जबतक (chunk_no++ <= n)
      cwrite (true, शून्य, 0);
}

/* -n K/N: Extract Kth of N chunks.  */

अटल व्योम
bytes_chunk_extract (intmax_t k, intmax_t n, अक्षर *buf, idx_t bufsize,
                     ssize_t initial_read, off_t file_आकार)
{
  off_t start;
  off_t end;

  affirm (0 < k && k <= n);

  start = (k - 1) * (file_आकार / n) + MIN (k - 1, file_आकार % n);
  end = k == n ? file_आकार : k * (file_आकार / n) + MIN (k, file_आकार % n);

  अगर (start < initial_read)
    {
      स_हटाओ (buf, buf + start, initial_read - start);
      initial_read -= start;
    }
  अन्यथा
    {
      अगर (initial_read < start
          && lseek (STDIN_FILENO, start - initial_read, प्रस्तुत_से) < 0)
        error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (infile));
      initial_read = -1;
    }

  जबतक (start < end)
    {
      ssize_t n_read;
      अगर (0 <= initial_read)
        {
          n_read = initial_read;
          initial_read = -1;
        }
      अन्यथा
        {
          n_read = पढ़ो (STDIN_FILENO, buf, bufsize);
          अगर (n_read < 0)
            error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (infile));
        }
      अगर (n_read == 0)
        अवरोध; /* eof.  */
      n_read = MIN (n_read, end - start);
      अगर (full_write (STDOUT_FILENO, buf, n_read) != n_read
          && ! ignorable (त्रुटि_सं))
        error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef ("-"));
      start += n_read;
    }
}

प्रकार काष्ठा of_info
{
  अक्षर *of_name;
  पूर्णांक ofd;
  खाता *ofile;
  pid_t opid;
} of_t;

क्रमागत
{
  OFD_NEW = -1,
  OFD_APPEND = -2
};

/* Rotate file descriptors when we're writing to more output files than we
   have available file descriptors.
   Return whether we came under file resource pressure.
   If so, it's probably best to close each file when finished with it.  */

अटल bool
ofile_open (of_t *files, idx_t i_check, idx_t nfiles)
{
  bool file_limit = false;

  अगर (files[i_check].ofd <= OFD_NEW)
    {
      पूर्णांक fd;
      idx_t i_reopen = i_check ? i_check - 1 : nfiles - 1;

      /* Another process could have opened a file in between the calls to
         close and open, so we should keep trying until open succeeds or
         we've closed all of our files.  */
      जबतक (true)
        {
          अगर (files[i_check].ofd == OFD_NEW)
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
                         O_WRONLY | O_द्विआधारी | O_APPEND | O_NONBLOCK);
            }

          अगर (0 <= fd)
            अवरोध;

          अगर (!(त्रुटि_सं == EMFILE || त्रुटि_सं == ENFILE))
            error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (files[i_check].of_name));

          file_limit = true;

          /* Search backwards for an open file to close.  */
          जबतक (files[i_reopen].ofd < 0)
            {
              i_reopen = i_reopen ? i_reopen - 1 : nfiles - 1;
              /* No more open files to close, exit with E[NM]FILE.  */
              अगर (i_reopen == i_check)
                error (निकास_त्रुटि, त्रुटि_सं, "%s",
                       quotef (files[i_check].of_name));
            }

          अगर (ख_बंद (files[i_reopen].ofile) != 0)
            error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (files[i_reopen].of_name));
          files[i_reopen].ofile = शून्य;
          files[i_reopen].ofd = OFD_APPEND;
        }

      files[i_check].ofd = fd;
      खाता *ofile = fdopen (fd, "a");
      अगर (!ofile)
        error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (files[i_check].of_name));
      files[i_check].ofile = ofile;
      files[i_check].opid = filter_pid;
      filter_pid = 0;
    }

  वापस file_limit;
}

/* -n r/[K/]N: Divide file into N chunks in round robin fashion.
   Use BUF of size BUFSIZE for the buffer, and if allocating storage
   put its address into *FILESP to pacify -fsanitize=leak.
   When K == 0, we try to keep the files open in parallel.
   If we run out of file resources, then we revert
   to opening and closing each file for each line.  */

अटल व्योम
पंक्तियाँ_rr (intmax_t k, intmax_t n, अक्षर *buf, idx_t bufsize, of_t **filesp)
{
  bool wrapped = false;
  bool wrote = false;
  bool file_limit;
  idx_t i_file;
  of_t *files IF_LINT (= शून्य);
  intmax_t पंक्ति_no;

  अगर (k)
    पंक्ति_no = 1;
  अन्यथा
    {
      अगर (IDX_MAX < n)
        xalloc_die ();
      files = *filesp = xinmalloc (n, माप *files);

      /* Generate output file names. */
      क्रम (i_file = 0; i_file < n; i_file++)
        {
          अग्र_file_name ();
          files[i_file].of_name = xstrdup (outfile);
          files[i_file].ofd = OFD_NEW;
          files[i_file].ofile = शून्य;
          files[i_file].opid = 0;
        }
      i_file = 0;
      file_limit = false;
    }

  जबतक (true)
    {
      अक्षर *bp = buf, *eob;
      ssize_t n_read = पढ़ो (STDIN_FILENO, buf, bufsize);
      अगर (n_read < 0)
        error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (infile));
      अन्यथा अगर (n_read == 0)
        अवरोध; /* eof.  */
      eob = buf + n_read;

      जबतक (bp != eob)
        {
          idx_t to_write;
          bool अग्र = false;

          /* Find end of line. */
          अक्षर *bp_out = स_प्रथम (bp, eolchar, eob - bp);
          अगर (bp_out)
            {
              bp_out++;
              अग्र = true;
            }
          अन्यथा
            bp_out = eob;
          to_write = bp_out - bp;

          अगर (k)
            {
              अगर (पंक्ति_no == k && unbuffered)
                {
                  अगर (full_write (STDOUT_FILENO, bp, to_write) != to_write)
                    write_error ();
                }
              अन्यथा अगर (पंक्ति_no == k && ख_डालो (bp, to_write, 1, मानक_निकास) != 1)
                {
                  write_error ();
                }
              अगर (अग्र)
                पंक्ति_no = (पंक्ति_no == n) ? 1 : पंक्ति_no + 1;
            }
          अन्यथा
            {
              /* Secure file descriptor. */
              file_limit |= ofile_open (files, i_file, n);
              अगर (unbuffered)
                {
                  /* Note writing to fd, rather than flushing the FILE gives
                     an 8% performance benefit, due to reduced data copying.  */
                  अगर (full_write (files[i_file].ofd, bp, to_write) != to_write
                      && ! ignorable (त्रुटि_सं))
                    error (निकास_त्रुटि, त्रुटि_सं, "%s",
                           quotef (files[i_file].of_name));
                }
              अन्यथा अगर (ख_डालो (bp, to_write, 1, files[i_file].ofile) != 1
                       && ! ignorable (त्रुटि_सं))
                error (निकास_त्रुटि, त्रुटि_सं, "%s",
                       quotef (files[i_file].of_name));

              अगर (! ignorable (त्रुटि_सं))
                wrote = true;

              अगर (file_limit)
                {
                  अगर (ख_बंद (files[i_file].ofile) != 0)
                    error (निकास_त्रुटि, त्रुटि_सं, "%s",
                           quotef (files[i_file].of_name));
                  files[i_file].ofile = शून्य;
                  files[i_file].ofd = OFD_APPEND;
                }
              अगर (अग्र && ++i_file == n)
                {
                  wrapped = true;
                  /* If no filters are accepting input, stop reading.  */
                  अगर (! wrote)
                    जाओ no_filters;
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
  अगर (!k)
    {
      idx_t ceiling = wrapped ? n : i_file;
      क्रम (i_file = 0; i_file < n; i_file++)
        {
          अगर (i_file >= ceiling && !elide_empty_files)
            file_limit |= ofile_open (files, i_file, n);
          अगर (files[i_file].ofd >= 0)
            closeout (files[i_file].ofile, files[i_file].ofd,
                      files[i_file].opid, files[i_file].of_name);
          files[i_file].ofd = OFD_APPEND;
        }
    }
}

#घोषणा FAIL_ONLY_ONE_WAY()					\
  करो								\
    {								\
      error (0, 0, _("cannot split in more than one way"));	\
      usage (निकास_त्रुटि);					\
    }								\
  जबतक (0)

/* Report a string-to-integer conversion failure MSGID with ARG.  */

अटल _Noreturn व्योम
strtoint_die (अक्षर स्थिर *msgid, अक्षर स्थिर *arg)
{
  error (निकास_त्रुटि, त्रुटि_सं == EINVAL ? 0 : त्रुटि_सं, "%s: %s",
         gettext (msgid), quote (arg));
}

/* Use OVERFLOW_OK when it is OK to ignore LONGINT_OVERFLOW errors, since the
   extreme value will do the right thing anyway on any practical platform.  */
#घोषणा OVERFLOW_OK LONGINT_OVERFLOW

/* Parse ARG for number of bytes or lines.  The number can be followed
   by MULTIPLIERS, and the resulting value must be positive.
   If the number cannot be parsed, diagnose with MSG.
   Return the number parsed, or an INTMAX_MAX on overflow.  */

अटल intmax_t
parse_n_units (अक्षर स्थिर *arg, अक्षर स्थिर *multipliers, अक्षर स्थिर *msgid)
{
  intmax_t n;
  अगर (OVERFLOW_OK < xstrtoimax (arg, शून्य, 10, &n, multipliers) || n < 1)
    strtoint_die (msgid, arg);
  वापस n;
}

/* Parse K/N syntax of chunk options.  */

अटल व्योम
parse_chunk (intmax_t *k_units, intmax_t *n_units, अक्षर स्थिर *arg)
{
  अक्षर *argend;
  strtol_error e = xstrtoimax (arg, &argend, 10, n_units, "");
  अगर (e == LONGINT_INVALID_SUFFIX_CHAR && *argend == '/')
    {
      *k_units = *n_units;
      *n_units = parse_n_units (argend + 1, "",
                                N_("invalid number of chunks"));
      अगर (! (0 < *k_units && *k_units <= *n_units))
        error (निकास_त्रुटि, 0, "%s: %s", _("invalid chunk number"),
               quote_mem (arg, argend - arg));
    }
  अन्यथा अगर (! (e <= OVERFLOW_OK && 0 < *n_units))
    strtoint_die (N_("invalid number of chunks"), arg);
}


पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  क्रमागत Split_type split_type = type_undef;
  idx_t in_blk_आकार = 0;	/* optimal block size of input file device */
  idx_t page_आकार = getpagesize ();
  intmax_t k_units = 0;
  intmax_t n_units = 0;

  अटल अक्षर स्थिर multipliers[] = "bEGKkMmPQRTYZ0";
  पूर्णांक c;
  पूर्णांक digits_optind = 0;
  off_t file_आकार = OFF_T_MAX;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  /* Parse command line options.  */

  जबतक (true)
    {
      /* This is the argv-index of the option we will read next.  */
      पूर्णांक this_optind = optind ? optind : 1;

      c = getopt_long (argc, argv, "0123456789C:a:b:del:n:t:ux",
                       longopts, शून्य);
      अगर (c == -1)
        अवरोध;

      चयन (c)
        {
        हाल 'a':
          suffix_लंबाई = xdectoimax (optarg, 0, IDX_MAX,
                                      "", _("invalid suffix length"), 0);
          अवरोध;

        हाल ADDITIONAL_SUFFIX_OPTION:
          {
            पूर्णांक suffix_len = म_माप (optarg);
            अगर (last_component (optarg) != optarg
                || (suffix_len && ISSLASH (optarg[suffix_len - 1])))
              {
                error (0, 0,
                       _("invalid suffix %s, contains directory separator"),
                       quote (optarg));
                usage (निकास_त्रुटि);
              }
          }
          additional_suffix = optarg;
          अवरोध;

        हाल 'b':
          अगर (split_type != type_undef)
            FAIL_ONLY_ONE_WAY ();
          split_type = type_bytes;
          n_units = parse_n_units (optarg, multipliers,
                                   N_("invalid number of bytes"));
          अवरोध;

        हाल 'l':
          अगर (split_type != type_undef)
            FAIL_ONLY_ONE_WAY ();
          split_type = type_पंक्तियाँ;
          n_units = parse_n_units (optarg, "", N_("invalid number of lines"));
          अवरोध;

        हाल 'C':
          अगर (split_type != type_undef)
            FAIL_ONLY_ONE_WAY ();
          split_type = type_byteslines;
          n_units = parse_n_units (optarg, multipliers,
                                   N_("invalid number of lines"));
          अवरोध;

        हाल 'n':
          अगर (split_type != type_undef)
            FAIL_ONLY_ONE_WAY ();
          /* skip any whitespace */
          जबतक (है_खाली (to_uchar (*optarg)))
            optarg++;
          अगर (STRNCMP_LIT (optarg, "r/") == 0)
            {
              split_type = type_rr;
              optarg += 2;
            }
          अन्यथा अगर (STRNCMP_LIT (optarg, "l/") == 0)
            {
              split_type = type_chunk_पंक्तियाँ;
              optarg += 2;
            }
          अन्यथा
            split_type = type_chunk_bytes;
          parse_chunk (&k_units, &n_units, optarg);
          अवरोध;

        हाल 'u':
          unbuffered = true;
          अवरोध;

        हाल 't':
          {
            अक्षर neweol = optarg[0];
            अगर (! neweol)
              error (निकास_त्रुटि, 0, _("empty record separator"));
            अगर (optarg[1])
              {
                अगर (streq (optarg, "\\0"))
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
            अगर (0 <= eolchar && neweol != eolchar)
              {
                error (निकास_त्रुटि, 0,
                       _("multiple separator characters specified"));
              }

            eolchar = neweol;
          }
          अवरोध;

        हाल '0':
        हाल '1':
        हाल '2':
        हाल '3':
        हाल '4':
        हाल '5':
        हाल '6':
        हाल '7':
        हाल '8':
        हाल '9':
          अगर (split_type == type_undef)
            {
              split_type = type_digits;
              n_units = 0;
            }
          अगर (split_type != type_undef && split_type != type_digits)
            FAIL_ONLY_ONE_WAY ();
          अगर (digits_optind != 0 && digits_optind != this_optind)
            n_units = 0;	/* More than one number given; ignore other. */
          digits_optind = this_optind;
          अगर (ckd_mul (&n_units, n_units, 10)
              || ckd_add (&n_units, n_units, c - '0'))
            n_units = INTMAX_MAX;
          अवरोध;

        हाल 'd':
        हाल 'x':
          अगर (c == 'd')
            suffix_alphabet = "0123456789";
          अन्यथा
            suffix_alphabet = "0123456789abcdef";
          अगर (optarg)
            {
              अगर (म_माप (optarg) != म_अखोज (optarg, suffix_alphabet))
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
                  जबतक (*optarg == '0' && *(optarg + 1) != '\0')
                    optarg++;
                  numeric_suffix_start = optarg;
                }
            }
          अवरोध;

        हाल 'e':
          elide_empty_files = true;
          अवरोध;

        हाल FILTER_OPTION:
          filter_command = optarg;
          अवरोध;

        हाल IO_BLKSIZE_OPTION:
          in_blk_आकार = xnumtoumax (optarg, 10, 1,
                                    MIN (SYS_BUFSIZE_MAX,
                                         MIN (IDX_MAX, SIZE_MAX) - 1),
                                    multipliers, _("invalid IO block size"),
                                    0, XTOINT_MIN_RANGE);
          अवरोध;

        हाल VERBOSE_OPTION:
          verbose = true;
          अवरोध;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        शेष:
          usage (निकास_त्रुटि);
        }
    }

  अगर (k_units != 0 && filter_command)
    {
      error (0, 0, _("--filter does not process a chunk extracted to "
                     "standard output"));
      usage (निकास_त्रुटि);
    }

  /* Handle default case.  */
  अगर (split_type == type_undef)
    {
      split_type = type_पंक्तियाँ;
      n_units = 1000;
    }

  अगर (n_units == 0)
    {
      error (0, 0, _("invalid number of lines: %s"), quote ("0"));
      usage (निकास_त्रुटि);
    }

  अगर (eolchar < 0)
    eolchar = '\n';

  set_suffix_लंबाई (n_units, split_type);

  /* Get out the filename arguments.  */

  अगर (optind < argc)
    infile = argv[optind++];

  अगर (optind < argc)
    outbase = argv[optind++];

  अगर (optind < argc)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind]));
      usage (निकास_त्रुटि);
    }

  /* Check that the suffix length is large enough for the numerical
     suffix start value.  */
  अगर (numeric_suffix_start && म_माप (numeric_suffix_start) > suffix_लंबाई)
    {
      error (0, 0, _("numerical suffix start value is too large "
                     "for the suffix length"));
      usage (निकास_त्रुटि);
    }

  /* Open the input file.  */
  अगर (! streq (infile, "-")
      && fd_reopen (STDIN_FILENO, infile, O_RDONLY, 0) < 0)
    error (निकास_त्रुटि, त्रुटि_सं, _("cannot open %s for reading"),
           quoteaf (infile));

  /* Binary I/O is safer when byte counts are used.  */
  xset_द्विआधारी_mode (STDIN_FILENO, O_द्विआधारी);

  /* Advise the kernel of our access pattern.  */
  fdadvise (STDIN_FILENO, 0, 0, FADVISE_SEQUENTIAL);

  /* Get the optimal block size of input device and make a buffer.  */

  अगर (ख_स्थिति (STDIN_FILENO, &in_stat_buf) != 0)
    error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (infile));

  अगर (in_blk_आकार == 0)
    {
      in_blk_आकार = io_blksize (&in_stat_buf);
      अगर (SYS_BUFSIZE_MAX < in_blk_आकार)
        in_blk_आकार = SYS_BUFSIZE_MAX;
    }

  अक्षर *buf = xalignalloc (page_आकार, in_blk_आकार + 1);
  ssize_t initial_read = -1;

  अगर (split_type == type_chunk_bytes || split_type == type_chunk_पंक्तियाँ)
    {
      file_आकार = निवेश_file_आकार (STDIN_FILENO, &in_stat_buf,
                                   buf, in_blk_आकार);
      अगर (file_आकार < 0)
        error (निकास_त्रुटि, त्रुटि_सं, _("%s: cannot determine file size"),
               quotef (infile));
      initial_read = MIN (file_आकार, in_blk_आकार);
    }

  /* When filtering, closure of one pipe must not terminate the process,
     as there may still be other streams expecting input from us.  */
  अगर (filter_command)
    default_SIGPIPE = संकेत (SIGPIPE, संक_छोड़ो) == संक_पूर्व;

  चयन (split_type)
    {
    हाल type_digits:
    हाल type_पंक्तियाँ:
      पंक्तियाँ_split (n_units, buf, in_blk_आकार);
      अवरोध;

    हाल type_bytes:
      bytes_split (n_units, 0, buf, in_blk_आकार, -1, 0);
      अवरोध;

    हाल type_byteslines:
      पंक्ति_bytes_split (n_units, buf, in_blk_आकार);
      अवरोध;

    हाल type_chunk_bytes:
      अगर (k_units == 0)
        bytes_split (file_आकार / n_units, file_आकार % n_units,
                     buf, in_blk_आकार, initial_read, n_units);
      अन्यथा
        bytes_chunk_extract (k_units, n_units, buf, in_blk_आकार, initial_read,
                             file_आकार);
      अवरोध;

    हाल type_chunk_पंक्तियाँ:
      पंक्तियाँ_chunk_split (k_units, n_units, buf, in_blk_आकार, initial_read,
                         file_आकार);
      अवरोध;

    हाल type_rr:
      /* Note, this is like 'sed -n ${k}~${n}p' when k > 0,
         but the functionality is provided for symmetry.  */
      {
        of_t *files;
        पंक्तियाँ_rr (k_units, n_units, buf, in_blk_आकार, &files);
      }
      अवरोध;

    हाल type_undef: शेष:
      affirm (false);
    }

  अगर (बंद (STDIN_FILENO) != 0)
    error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (infile));
  closeout (शून्य, output_desc, filter_pid, outfile);

  main_exit (निकास_सफल);
}
