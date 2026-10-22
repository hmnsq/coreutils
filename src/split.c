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
#ضمن <config.h>

#ضمن <stdio.h>
#ضمن <getopt.h>
#ضمن <signal.h>
#ضمن <sys/types.h>
#ضمن <sys/wait.h>
#ضمن <spawn.h>

#ضمن "system.h"
#ضمن "alignalloc.h"
#ضمن "assure.h"
#ضمن "fadvise.h"
#ضمن "fd-reopen.h"
#ضمن "fcntl--.h"
#ضمن "full-write.h"
#ضمن "ioblksize.h"
#ضمن "quote.h"
#ضمن "sig2str.h"
#ضمن "sys-limits.h"
#ضمن "temp-stream.h"
#ضمن "unistd--.h"
#ضمن "xbinary-io.h"
#ضمن "xdectoint.h"
#ضمن "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "split"

#تعریف_کرو AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("Richard M. Stallman")

/* Shell command to filter through, instead of creating files.  */
ساكن حرف ثابت *filter_command;

/* Process ID of the filter.  */
ساكن pid_t filter_pid;

ساكن بنية
{
  /* The number of open pipes.  */
  صحيح n_pipes;

  /* The number of allocated pipes.  */
  idx_t n_pipes_alloc;

  /* Array of open pipes.  */
  صحيح *pipes COUNTED_BY (n_pipes);
} open_pipes;

/* Whether SIGPIPE has the default action, when --filter is used.  */
ساكن bool default_SIGPIPE;

/* Base name of output files.  */
ساكن حرف ثابت *outbase = "x";

/* Name of output files.  */
ساكن حرف *outfile;

/* Pointer to the end of the prefix in OUTFILE.
   Suffixes are inserted here.  */
ساكن حرف *outfile_mid;

/* Generate new suffix when suffixes are exhausted.  */
ساكن bool suffix_auto = true;

/* Length of OUTFILE's suffix.  */
ساكن idx_t suffix_لمائی;

/* Alphabet of characters to use in suffix.  */
ساكن حرف ثابت *suffix_alphabet = "abcdefghijklmnopqrstuvwxyz";

/* Numerical suffix start value.  */
ساكن حرف ثابت *numeric_suffix_شروع;

/* Additional suffix to append to output file names.  */
ساكن حرف ثابت *additional_suffix;

/* Name of input file.  May be "-".  */
ساكن حرف ثابت *infile = "-";

/* stat buf for input file.  */
ساكن بنية stat وچ_stat_buf;

/* Descriptor on which output file is open.  */
ساكن صحيح output_desc = -1;

/* If true, print a diagnostic on standard error just before each
   output file is opened. */
ساكن bool verbose;

/* If true, don't generate zero length output files. */
ساكن bool elide_خالی_تھاواں_files;

/* If true, in round robin mode, immediately copy
   input to output, which is much slower, so disabled by default.  */
ساكن bool unbuffered;

/* The character marking end of line.  Defaults to \n below.  */
ساكن صحيح eolchar = -1;

/* The split mode to use.  */
تعداد ونڈو_type
{
  type_undef, type_بائٹاں, type_byteslines, type_سطریں, type_digits,
  type_chunk_بائٹاں, type_chunk_سطریں, type_rr
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
تعداد
{
  VERBOSE_OPTION = CHAR_MAX + 1,
  FILTER_OPTION,
  IO_BLKSIZE_OPTION,
  ADDITIONAL_SUFFIX_OPTION
};

ساكن بنية option ثابت longopts[] =
{
  {"bytes", required_دلیل, عدم, 'b'},
  {"lines", required_دلیل, عدم, 'l'},
  {"line-bytes", required_دلیل, عدم, 'C'},
  {"number", required_دلیل, عدم, 'n'},
  {"elide-empty-files", no_دلیل, عدم, 'e'},
  {"unbuffered", no_دلیل, عدم, 'u'},
  {"suffix-length", required_دلیل, عدم, 'a'},
  {"additional-suffix", required_دلیل, عدم,
   ADDITIONAL_SUFFIX_OPTION},
  {"numeric-suffixes", optional_دلیل, عدم, 'd'},
  {"hex-suffixes", optional_دلیل, عدم, 'x'},
  {"filter", required_دلیل, عدم, FILTER_OPTION},
  {"verbose", no_دلیل, عدم, VERBOSE_OPTION},
  {"separator", required_دلیل, عدم, 't'},
  {"-io-blksize", required_دلیل, عدم,
   IO_BLKSIZE_OPTION}, /* do not document */
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

/* Return true if the errno value, ERR, is ignorable.  */
ساكن مضمن bool
ignorable (صحيح err)
{
  أرجع filter_command && err == EPIPE;
}

ساكن فراغ
سیٹ_suffix_لمائی (intmax_t n_units, تعداد ونڈو_type ونڈو_type_2)
{
#تعریف_کرو DEFAULT_SUFFIX_لمائی 2

  صحيح suffix_لمائی_needed = 0;

  /* The suffix auto length feature is incompatible with
     a user specified start value as the generated suffixes
     are not all consecutive.  */
  إذا (numeric_suffix_شروع)
    suffix_auto = false;

  /* Auto-calculate the suffix length if the number of files is given.  */
  إذا (ونڈو_type_2 == type_chunk_بائٹاں || ونڈو_type_2 == type_chunk_سطریں
      || ونڈو_type_2 == type_rr)
    {
      intmax_t n_units_سرا = n_units - 1;
      إذا (numeric_suffix_شروع)
        {
          intmax_t n_شروع;
          strtol_error e = xstrtoimax (numeric_suffix_شروع, عدم, 10,
                                       &n_شروع, "");
          إذا (e == LONGINT_OK && n_شروع < n_units)
            {
              /* Restrict auto adjustment so we don't keep
                 incrementing a suffix size arbitrarily,
                 as that would break sort order for files
                 generated from multiple split runs.  */
              إذا (ckd_جوڑو (&n_units_سرا, n_units_سرا, n_شروع))
                n_units_سرا = INTMAX_MAX;
            }

        }
      idx_t alphabet_len = لڑی_لمبائی (suffix_alphabet);
      افعل
        suffix_لمائی_needed++;
      طالما (n_units_سرا /= alphabet_len);

      suffix_auto = false;
    }

  إذا (suffix_لمائی)            /* set by user */
    {
      إذا (suffix_لمائی < suffix_لمائی_needed)
        error (EXIT_FAILURE, 0,
               _("the suffix length needs to be at least %d"),
               suffix_لمائی_needed);
      suffix_auto = false;
      أرجع;
    }
  وإلا
    suffix_لمائی = MAX (DEFAULT_SUFFIX_لمائی, suffix_لمائی_needed);
}

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکھو (_("\
Usage: %s [OPTION]... [FILE [اگیتر_2]]\n\
"),
              پروگرام_name);
      fputs (_("\
Output pieces دا FILE منزل PREFIXaa, PREFIXab, ...;\n\
افتراضي ناپ is 1000 سطریں, and افتراضي اگیتر_2 is 'x'.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oprintf (_("\
  -a, --suffix-لمائی=N\n\
         generate suffixes دا لمائی N (افتراضي %d)\n\
"), DEFAULT_SUFFIX_لمائی);
      oputs (_("\
      --additional-suffix=SUFFIX\n\
         append an additional SUFFIX منزل file ناں\n\
"));
      oputs (_("\
  -b, --بائٹاں=ناپ_2\n\
         put ناپ_2 بائٹاں فی output file\n\
"));
      oputs (_("\
  -C, --سطر-بائٹاں=ناپ_2\n\
         put at most ناپ_2 بائٹاں دا ریکارڈاں فی output file\n\
"));
      oputs (_("\
  -d\n\
         use numeric suffixes starting at 0, not alphabetic\n\
"));
      oputs (_("\
      --numeric-suffixes[=FROM]\n\
         same as -d, but allow setting the شروع قدر\n\
"));
      oputs (_("\
  -x\n\
         use hex suffixes starting at 0, not alphabetic\n\
"));
      oputs (_("\
      --hex-suffixes[=FROM]\n\
         same as -x, but allow setting the شروع قدر\n\
"));
      oputs (_("\
  -e, --elide-خالی_تھاواں-files\n\
         افعل not generate خالی_تھاواں output files with '-n'\n\
"));
      oputs (_("\
      --filter=COMMAND\n\
         اكتب منزل shell COMMAND; file name is $FILE\n\
"));
      oputs (_("\
  -l, --سطریں=نمبر_2\n\
         put نمبر_2 سطریں/ریکارڈاں فی output file\n\
"));
      oputs (_("\
  -n, --نمبر=CHUNKS\n\
         generate CHUNKS output files; see explanation ہیٹھاں\n\
"));
      oputs (_("\
  -t, --separator=SEP\n\
         use SEP instead دا newline as the ریکارڈ separator;\n\
         '\\0' (zero) specifies the NUL character\n\
"));
      oputs (_("\
  -u, --unbuffered\n\
         immediately copy داخلہ منزل output with '-n r/...'\n\
"));
      oputs (_("\
      --verbose\n\
         print a diagnostic just before each output file is opened\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      emit_ناپ_note ();
      fputs (_("\n\
CHUNKS may be:\n\
  N       ونڈو into N files based on ناپ دا داخلہ\n\
  K/N     output Kth دا N منزل standard output\n\
  l/N     ونڈو into N files without splitting سطریں/ریکارڈاں\n\
  l/K/N   output Kth دا N منزل standard output without splitting سطریں/ریکارڈاں\n\
  r/N     like 'l' but use گیڑا_2 robin distribution\n\
  r/K/N   likewise but only output Kth دا N منزل standard output\n\
"), stdout);
      fputs (_("\n\
-n (except -nr) will بفر منزل $TMPDIR, defaulting منزل /tmp,\n\
إذا the داخلہ ناپ cannot easily be determined.\n\
"), stdout);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت);
}

/* Copy the data in FD to a temporary file, then make that file FD.
   Use BUF, of size BUFSIZE, to copy.  Return the number of
   bytes copied, or -1 (setting errno) on error.  */
ساكن off_t
copy_منزل_tmpfile (صحيح fd, حرف *buf, idx_t bufsize)
{
  FILE *tmp;
  إذا (!عارضی_دھارا (&tmp, عدم))
    أرجع -1;
  off_t copied = 0;
  off_t r;

  طالما (0 < (r = اقرأ (fd, buf, bufsize)))
    {
      إذا (fwrite (buf, 1, r, tmp) != r)
        أرجع -1;
      إذا (ckd_جوڑو (&copied, copied, r))
        {
          errno = EOVERFLOW;
          أرجع -1;
        }
    }

  إذا (r < 0)
    أرجع r;
  r = dup2 (fileno (tmp), fd);
  إذا (r < 0)
    أرجع r;
  إذا (fclose (tmp) < 0)
    أرجع -1;
  أرجع copied;
}

/* Return the number of bytes that can be read from FD with status ST.
   Store up to the first BUFSIZE bytes of the file's data into BUF,
   and advance the file position by the number of bytes read.  On
   input error, set errno and return -1.  */

ساكن off_t
داخلہ_file_ناپ (صحيح fd, بنية stat ثابت *st, حرف *buf, idx_t bufsize)
{
  off_t ناپ = 0;
  افعل
    {
      ssize_t n_read = اقرأ (fd, buf + ناپ, bufsize - ناپ);
      إذا (n_read <= 0)
        أرجع n_read < 0 ? n_read : ناپ;
      ناپ += n_read;
    }
  طالما (ناپ < bufsize);

  off_t cur, سرا;
  إذا ((usable_st_ناپ (st) && st->st_ناپ < ناپ)
      || (cur = lseek (fd, 0, SEEK_CUR)) < 0
      || cur < ناپ /* E.g., /dev/zero on GNU/Linux.  */
      || (سرا = lseek (fd, 0, SEEK_END)) < 0)
    {
      حرف *tmpbuf = xmalloc (bufsize);
      سرا = copy_منزل_tmpfile (fd, tmpbuf, bufsize);
      حرر (tmpbuf);
      إذا (سرا < 0)
        أرجع سرا;
      cur = 0;
    }

  إذا (سرا == OFF_T_MAX /* E.g., /dev/zero on GNU/Hurd.  */
      || (cur < سرا && ckd_جوڑو (&ناپ, ناپ, سرا - cur)))
    {
      errno = EOVERFLOW;
      أرجع -1;
    }

  إذا (cur < سرا)
    {
      off_t r = lseek (fd, cur, SEEK_SET);
      إذا (r < 0)
        أرجع r;
    }

  أرجع ناپ;
}

/* Compute the next sequential output file name and store it into the
   string 'outfile'.  */

ساكن فراغ
اگلا_file_name (فراغ)
{
  /* Index in suffix_alphabet of each character in the suffix.  */
  ساكن idx_t *sufindex;
  ساكن idx_t outbase_لمائی;
  ساكن idx_t outfile_لمائی;
  ساكن idx_t addsuf_لمائی;

  إذا (! outfile)
    {
      bool حد_توں_ودھ, widen;

new_name:
      widen = !! outfile_لمائی;

      إذا (! widen)
        {
          /* Allocate and initialize the first file name.  */

          outbase_لمائی = لڑی_لمبائی (outbase);
          addsuf_لمائی = additional_suffix ? لڑی_لمبائی (additional_suffix) : 0;
          حد_توں_ودھ = ckd_جوڑو (&outfile_لمائی, outbase_لمائی + addsuf_لمائی,
                              suffix_لمائی);
        }
      وإلا
        {
          /* Reallocate and initialize a new wider file name.
             We do this by subsuming the unchanging part of
             the generated suffix into the prefix (base), and
             reinitializing the now one longer suffix.  */

          حد_توں_ودھ = ckd_جوڑو (&outfile_لمائی, outfile_لمائی, 2);
          suffix_لمائی++;
        }

      idx_t outfile_ناپ;
      حد_توں_ودھ |= ckd_جوڑو (&outfile_ناپ, outfile_لمائی, 1);
      إذا (حد_توں_ودھ)
        xalloc_die ();
      outfile = xirealloc (outfile, outfile_ناپ);

      إذا (! widen)
        حافظہ_نقل (outfile, outbase, outbase_لمائی);
      وإلا
        {
          /* Append the last alphabet character to the file name prefix.  */
          outfile[outbase_لمائی] = suffix_alphabet[sufindex[0]];
          outbase_لمائی++;
        }

      outfile_mid = outfile + outbase_لمائی;
      memset (outfile_mid, suffix_alphabet[0], suffix_لمائی);
      إذا (additional_suffix)
        حافظہ_نقل (outfile_mid + suffix_لمائی, additional_suffix, addsuf_لمائی);
      outfile[outfile_لمائی] = 0;

      حرر (sufindex);
      sufindex = xicalloc (suffix_لمائی, حجم *sufindex);

      إذا (numeric_suffix_شروع)
        {
          affirm (! widen);

          /* Update the output file name.  */
          idx_t i = لڑی_لمبائی (numeric_suffix_شروع);
          حافظہ_نقل (outfile_mid + suffix_لمائی - i, numeric_suffix_شروع, i);

          /* Update the suffix index.  */
          idx_t *sufindex_سرا = sufindex + suffix_لمائی;
          طالما (i-- != 0)
            *--sufindex_سرا = numeric_suffix_شروع[i] - '0';
        }

#إذا ! _POSIX_NO_TRUNC && HAVE_PATHCONF && defined _PC_NAME_MAX
      /* POSIX requires that if the output file name is too long for
         its directory, 'split' must fail without creating any files.
         This must be checked for explicitly on operating systems that
         silently truncate file names.  */
      {
        حرف *dir = dir_name (outfile);
        لما name_max = pathconf (dir, _PC_NAME_MAX);
        إذا (0 <= name_max && name_max < ادھار_len (آخری_component (outfile)))
          error (EXIT_FAILURE, ENAMETOOLONG, "%s", quotef (outfile));
        حرر (dir);
      }
#جے_انت
    }
  وإلا
    {
      /* Increment the suffix in place, if possible.  */

      idx_t i = suffix_لمائی;
      طالما (i-- != 0)
        {
          sufindex[i]++;
          إذا (suffix_auto && i == 0 && ! suffix_alphabet[sufindex[0] + 1])
            اذهب new_name;
          outfile_mid[i] = suffix_alphabet[sufindex[i]];
          إذا (outfile_mid[i])
            أرجع;
          sufindex[i] = 0;
          outfile_mid[i] = suffix_alphabet[sufindex[i]];
        }
      error (EXIT_FAILURE, 0, _("output file suffixes exhausted"));
    }
}

/* Create or truncate a file.  */

ساكن صحيح
create (حرف ثابت *name)
{
  إذا (!filter_command)
    {
      إذا (verbose)
        fprintf (stdout, _("creating file %s\n"), quoteaf (name));

      صحيح oflags = O_WRONLY | O_CREAT | O_ثنائی;
      صحيح fd = open (name, oflags | O_EXCL, منوال_RW_UGO);
      إذا (0 <= fd || errno != EEXIST)
        أرجع fd;
      fd = open (name, oflags, منوال_RW_UGO);
      إذا (fd < 0)
        أرجع fd;
      بنية stat باہرلا_stat_buf;
      إذا (fstat (fd, &باہرلا_stat_buf) != 0)
        error (EXIT_FAILURE, errno, _("failed to stat %s"), quoteaf (name));
      إذا (psame_inode (&وچ_stat_buf, &باہرلا_stat_buf))
        error (EXIT_FAILURE, 0, _("%s would overwrite input; aborting"),
               quoteaf (name));
      إذا (ftruncate (fd, 0) < 0
          && (S_ISREG (باہرلا_stat_buf.st_منوال) || S_TYPEISSHM (&باہرلا_stat_buf)))
        error (EXIT_FAILURE, errno, _("%s: error truncating"), quotef (name));

      أرجع fd;
    }
  وإلا
    {
      إذا (setenv ("FILE", name, 1) != 0)
        error (EXIT_FAILURE, errno,
               _("failed to set FILE environment variable"));
      إذا (verbose)
        fprintf (stdout, _("executing with FILE=%s\n"), quotef (name));

      صحيح نتیجہ;
      صحيح fd_pair[2];
      pid_t بچہ_pid;

      posix_spawnattr_t attr;
      posix_spawn_file_actions_t actions;

      sigset_t سیٹ_2;
      sigemptyset (&سیٹ_2);
      إذا (default_SIGPIPE)
        sigaddset (&سیٹ_2, SIGPIPE);

      إذا (   (نتیجہ = posix_spawnattr_init (&attr))
          || (نتیجہ = posix_spawnattr_setflags (&attr,
                                                 (POSIX_SPAWN_USEVFORK
                                                  | POSIX_SPAWN_SETSIGDEF)))
          || (نتیجہ = posix_spawnattr_setsigdefault (&attr, &سیٹ_2))
          || (نتیجہ = posix_spawn_file_actions_init (&actions))
         )
        error (EXIT_FAILURE, نتیجہ, _("posix_spawn initialization failed"));

      إذا (pipe (fd_pair) != 0)
        error (EXIT_FAILURE, errno, _("failed to create pipe"));

      /* We have to close any pipes that were opened during an
         earlier call, otherwise this process will be holding a
         write-pipe that will prevent the earlier process from
         reading an EOF on the corresponding read-pipe.  */
      لكل (صحيح i = 0; i < open_pipes.n_pipes; ++i)
        إذا ((نتیجہ = posix_spawn_file_actions_addclose (&actions,
                                                         open_pipes.pipes[i])))
          اكسر;

      إذا (   نتیجہ
          || (نتیجہ = posix_spawn_file_actions_addclose (&actions, fd_pair[1]))
          || (fd_pair[0] != STDIN_FILENO
              && (   (نتیجہ = posix_spawn_file_actions_adddup2 (&actions,
                                                                 fd_pair[0],
                                                                 STDIN_FILENO))
                  || (نتیجہ = posix_spawn_file_actions_addclose (&actions,
                                                                  fd_pair[0]))))
         )
        error (EXIT_FAILURE, نتیجہ, _("posix_spawn setup failed"));


      حرف ثابت *shell_prog = getenv ("SHELL");
      إذا (shell_prog == عدم)
        shell_prog = "/bin/sh";
      حرف ثابت *ثابت argv[] = { آخری_component (shell_prog), "-c",
                                   filter_command, عدم };

      نتیجہ = posix_spawn (&بچہ_pid, shell_prog, &actions, &attr,
                            (حرف * ثابت *) argv, environ);
      إذا (نتیجہ != 0)
        error (EXIT_FAILURE, errno, _("failed to run command: \"%s -c %s\""),
               shell_prog, filter_command);

      posix_spawnattr_destroy (&attr);
      posix_spawn_file_actions_destroy (&actions);

      إذا (close (fd_pair[0]) != 0)
        error (EXIT_FAILURE, errno, _("failed to close input pipe"));
      filter_pid = بچہ_pid;
      إذا (open_pipes.n_pipes == open_pipes.n_pipes_alloc)
        open_pipes.pipes = xpalloc (open_pipes.pipes, &open_pipes.n_pipes_alloc,
                                    1, MIN (ودھ_توں_ودھ_عدد, IDX_MAX),
                                    حجم *open_pipes.pipes);
      idx_t ثابت pipe_idx = open_pipes.n_pipes++;
      open_pipes.pipes[pipe_idx] = fd_pair[1];
      أرجع fd_pair[1];
    }
}

/* Close the output file, and do any associated cleanup.
   If FP and FD are both specified, they refer to the same open file;
   in this case FP is closed, but FD is still used in cleanup.  */
ساكن فراغ
closeout (FILE *fp, صحيح fd, pid_t pid, حرف ثابت *name)
{
  إذا (fp != عدم && fclose (fp) != 0 && ! ignorable (errno))
    error (EXIT_FAILURE, errno, "%s", quotef (name));
  إذا (fd >= 0)
    {
      إذا (fp == عدم && close (fd) < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (name));
      لكل (صحيح j = 0; j < open_pipes.n_pipes; ++j)
        {
          إذا (open_pipes.pipes[j] == fd)
            {
              idx_t ثابت آخری_idx = open_pipes.n_pipes - 1;
              open_pipes.pipes[j] = open_pipes.pipes[آخری_idx];
              open_pipes.n_pipes--;
              اكسر;
            }
        }
    }
  إذا (pid > 0)
    {
      صحيح wstatus;
      إذا (waitpid (pid, &wstatus, 0) < 0)
        error (EXIT_FAILURE, errno, _("waiting for child process"));
      وإلا إذا (WIFSIGNALED (wstatus))
        {
          صحيح sig = WTERMSIG (wstatus);
          إذا (sig != SIGPIPE)
            {
              حرف signame[MAX (SIG2STR_MAX, INT_BUFSIZE_BOUND (صحيح))];
              إذا (sig2str (sig, signame) != 0)
                sprintf (signame, "%d", sig);
              error (sig + 128, 0,
                     _("with FILE=%s, signal %s from command: %s"),
                     quotef (name), signame, filter_command);
            }
        }
      وإلا إذا (WIFEXITED (wstatus))
        {
          صحيح ex = WEXITSTATUS (wstatus);
          إذا (ex != 0)
            error (ex, 0, _("with FILE=%s, exit %d from command: %s"),
                   quotef (name), ex, filter_command);
        }
      وإلا
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

ساكن bool
cwrite (bool new_file_flag, حرف ثابت *bp, idx_t بائٹاں)
{
  إذا (new_file_flag)
    {
      إذا (!bp && بائٹاں == 0 && elide_خالی_تھاواں_files)
        أرجع true;
      closeout (عدم, output_desc, filter_pid, outfile);
      اگلا_file_name ();
      output_desc = create (outfile);
      إذا (output_desc < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (outfile));
    }

  إذا (بھریا_write (output_desc, bp, بائٹاں) == بائٹاں)
    أرجع true;
  وإلا
    {
      إذا (! ignorable (errno))
        error (EXIT_FAILURE, errno, "%s", quotef (outfile));
      أرجع false;
    }
}

/* Split into pieces of exactly N_BYTES bytes.
   However, the first REM_BYTES pieces should be 1 byte longer.
   Use buffer BUF, whose size is BUFSIZE.
   If INITIAL_READ is nonnegative,
   BUF contains the first INITIAL_READ input bytes.  */

ساكن فراغ
بائٹاں_ونڈو (intmax_t n_بائٹاں, intmax_t rem_بائٹاں,
             حرف *buf, idx_t bufsize, ssize_t initial_read,
             intmax_t max_files)
{
  bool new_file_flag = true;
  bool filter_ok = true;
  intmax_t opened = 0;
  intmax_t منزل_write = n_بائٹاں + (0 < rem_بائٹاں);
  bool eof = ! منزل_write;

  طالما (! eof)
    {
      ssize_t n_read;
      إذا (0 <= initial_read)
        {
          n_read = initial_read;
          initial_read = -1;
          eof = n_read < bufsize;
        }
      وإلا
        {
          إذا (! filter_ok
              && 0 <= lseek (STDIN_FILENO, منزل_write, SEEK_CUR))
            {
              منزل_write = n_بائٹاں + (opened + 1 < rem_بائٹاں);
              new_file_flag = true;
            }

          n_read = اقرأ (STDIN_FILENO, buf, bufsize);
          إذا (n_read < 0)
            error (EXIT_FAILURE, errno, "%s", quotef (infile));
          eof = n_read == 0;
        }
      حرف *bp_باہرلا = buf;
      طالما (0 < منزل_write && منزل_write <= n_read)
        {
          إذا (filter_ok || new_file_flag)
            filter_ok = cwrite (new_file_flag, bp_باہرلا, منزل_write);
          opened += new_file_flag;
          new_file_flag = !max_files || (opened < max_files);
          إذا (! filter_ok && ! new_file_flag)
            {
              /* If filters no longer accepting input, stop reading.  */
              n_read = 0;
              eof = true;
              اكسر;
            }
          bp_باہرلا += منزل_write;
          n_read -= منزل_write;
          منزل_write = n_بائٹاں + (opened < rem_بائٹاں);
        }
      إذا (0 < n_read)
        {
          إذا (filter_ok || new_file_flag)
            filter_ok = cwrite (new_file_flag, bp_باہرلا, n_read);
          opened += new_file_flag;
          new_file_flag = false;
          إذا (! filter_ok && opened == max_files)
            {
              /* If filters no longer accepting input, stop reading.  */
              اكسر;
            }
          منزل_write -= n_read;
        }
    }

  /* Ensure NUMBER files are created, which truncates
     any existing files or notifies any consumers on fifos.
     FIXME: Should we do this before EXIT_FAILURE?  */
  طالما (opened++ < max_files)
    cwrite (true, عدم, 0);
}

/* Split into pieces of exactly N_LINES lines.
   Use buffer BUF, whose size is BUFSIZE.  */

ساكن فراغ
سطریں_ونڈو (intmax_t n_سطریں, حرف *buf, idx_t bufsize)
{
  ssize_t n_read;
  حرف *bp, *bp_باہرلا, *eob;
  bool new_file_flag = true;
  intmax_t n = 0;

  افعل
    {
      n_read = اقرأ (STDIN_FILENO, buf, bufsize);
      إذا (n_read < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
      bp = bp_باہرلا = buf;
      eob = bp + n_read;
      *eob = eolchar;
      طالما (true)
        {
          bp = rawmemchr (bp, eolchar);
          إذا (bp == eob)
            {
              إذا (eob != bp_باہرلا) /* do not write 0 bytes! */
                {
                  idx_t len = eob - bp_باہرلا;
                  cwrite (new_file_flag, bp_باہرلا, len);
                  new_file_flag = false;
                }
              اكسر;
            }

          ++bp;
          إذا (++n >= n_سطریں)
            {
              cwrite (new_file_flag, bp_باہرلا, bp - bp_باہرلا);
              bp_باہرلا = bp;
              new_file_flag = true;
              n = 0;
            }
        }
    }
  طالما (n_read);
}

/* Split into pieces that are as large as possible while still not more
   than N_BYTES bytes, and are split on line boundaries except
   where lines longer than N_BYTES bytes occur. */

ساكن فراغ
سطر_بائٹاں_ونڈو (intmax_t n_بائٹاں, حرف *buf, idx_t bufsize)
{
  ssize_t n_read;
  intmax_t n_باہرلا = 0;      /* for each split.  */
  idx_t n_hold = 0;
  حرف *hold = عدم;        /* for lines > bufsize.  */
  idx_t hold_ناپ = 0;
  bool ونڈو_سطر = false;  /* Whether a \n was output in a split.  */

  افعل
    {
      n_read = اقرأ (STDIN_FILENO, buf, bufsize);
      إذا (n_read < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
      idx_t n_بایاں = n_read;
      حرف *sob = buf;
      طالما (n_بایاں)
        {
          idx_t ونڈو_rest = 0;
          حرف *eoc = عدم;
          حرف *eol;

          /* Determine End Of Chunk and/or End of Line,
             which are used below to select what to write or buffer.  */
          إذا (n_بائٹاں - n_باہرلا - n_hold <= n_بایاں)
            {
              /* Have enough for split.  */
              ونڈو_rest = n_بائٹاں - n_باہرلا - n_hold;
              eoc = sob + ونڈو_rest - 1;
              eol = memrchr (sob, eolchar, ونڈو_rest);
            }
          وإلا
            eol = memrchr (sob, eolchar, n_بایاں);

          /* Output hold space if possible.  */
          إذا (n_hold && !(!eol && n_باہرلا))
            {
              cwrite (n_باہرلا == 0, hold, n_hold);
              n_باہرلا += n_hold;
              n_hold = 0;
            }

          /* Output to eol if present.  */
          إذا (eol)
            {
              ونڈو_سطر = true;
              idx_t n_write = eol - sob + 1;
              cwrite (n_باہرلا == 0, sob, n_write);
              n_باہرلا += n_write;
              n_بایاں -= n_write;
              sob += n_write;
              إذا (eoc)
                ونڈو_rest -= n_write;
            }

          /* Output to eoc or eob if possible.  */
          إذا (n_بایاں && !ونڈو_سطر)
            {
              idx_t n_write = eoc ? ونڈو_rest : n_بایاں;
              cwrite (n_باہرلا == 0, sob, n_write);
              n_باہرلا += n_write;
              n_بایاں -= n_write;
              sob += n_write;
              إذا (eoc)
                ونڈو_rest -= n_write;
            }

          /* Update hold if needed.  */
          إذا ((eoc && ونڈو_rest) || (!eoc && n_بایاں))
            {
              idx_t n_buf = eoc ? ونڈو_rest : n_بایاں;
              إذا (hold_ناپ - n_hold < n_buf)
                hold = xpalloc (hold, &hold_ناپ, n_buf - (hold_ناپ - n_hold),
                                -1, حجم *hold);
              حافظہ_نقل (hold + n_hold, sob, n_buf);
              n_hold += n_buf;
              n_بایاں -= n_buf;
              sob += n_buf;
            }

          /* Reset for new split.  */
          إذا (eoc)
            {
              n_باہرلا = 0;
              ونڈو_سطر = false;
            }
        }
    }
  طالما (n_read);

  /* Handle no eol at end of file.  */
  إذا (n_hold)
    cwrite (n_باہرلا == 0, hold, n_hold);

  حرر (hold);
}

/* -n l/[K/]N: Write lines to files of approximately file size / N.
   The file is partitioned into file size / N sized portions, with the
   last assigned any excess.  If a line _starts_ within a partition
   it is written completely to the corresponding file.  Since lines
   are not split even if they overlap a partition, the files written
   can be larger or smaller than the partition size, and even empty
   if a line is so long as to completely overlap the partition.  */

ساكن فراغ
سطریں_chunk_ونڈو (intmax_t k, intmax_t n, حرف *buf, idx_t bufsize,
                   ssize_t initial_read, off_t file_ناپ)
{
  affirm (n && k <= n);

  intmax_t rem_بائٹاں = file_ناپ % n;
  off_t chunk_ناپ = file_ناپ / n;
  intmax_t chunk_no = 1;
  off_t chunk_سرا = chunk_ناپ + (0 < rem_بائٹاں);
  off_t n_written = 0;
  bool new_file_flag = true;
  bool chunk_truncated = false;

  إذا (k > 1 && 0 < file_ناپ)
    {
      /* Start reading 1 byte before kth chunk of file.  */
      off_t شروع = (k - 1) * chunk_ناپ + MIN (k - 1, rem_بائٹاں) - 1;
      إذا (شروع < initial_read)
        {
          memmove (buf, buf + شروع, initial_read - شروع);
          initial_read -= شروع;
        }
      وإلا
        {
          إذا (initial_read < شروع
              && lseek (STDIN_FILENO, شروع - initial_read, SEEK_CUR) < 0)
            error (EXIT_FAILURE, errno, "%s", quotef (infile));
          initial_read = -1;
        }
      n_written = شروع;
      chunk_no = k - 1;
      chunk_سرا = شروع + 1;
    }

  طالما (n_written < file_ناپ)
    {
      حرف *bp = buf, *eob;
      ssize_t n_read;
      إذا (0 <= initial_read)
        {
          n_read = initial_read;
          initial_read = -1;
        }
      وإلا
        {
          n_read = اقرأ (STDIN_FILENO, buf,
                         MIN (bufsize, file_ناپ - n_written));
          إذا (n_read < 0)
            error (EXIT_FAILURE, errno, "%s", quotef (infile));
        }
      إذا (n_read == 0)
        اكسر; /* eof.  */
      chunk_truncated = false;
      eob = buf + n_read;

      طالما (bp != eob)
        {
          idx_t منزل_write;
          bool اگلا = false;

          /* Begin looking for '\n' at last byte of chunk.  */
          off_t skip = MIN (n_read, MAX (0, chunk_سرا - 1 - n_written));
          حرف *bp_باہرلا = memchr (bp + skip, eolchar, n_read - skip);
          إذا (bp_باہرلا)
            {
              bp_باہرلا++;
              اگلا = true;
            }
          وإلا
            bp_باہرلا = eob;
          منزل_write = bp_باہرلا - bp;

          إذا (k == chunk_no)
            {
              /* We don't use the stdout buffer here since we're writing
                 large chunks from an existing file, so it's more efficient
                 to write out directly.  */
              إذا (بھریا_write (STDOUT_FILENO, bp, منزل_write) != منزل_write)
                write_error ();
            }
          وإلا إذا (! k)
            cwrite (new_file_flag, bp, منزل_write);
          n_written += منزل_write;
          bp += منزل_write;
          n_read -= منزل_write;
          new_file_flag = اگلا;

          /* A line could have been so long that it skipped
             entire chunks. So create empty files in that case.  */
          طالما (اگلا || chunk_سرا <= n_written)
            {
              إذا (!اگلا && bp == eob)
                {
                  /* replenish buf, before going to next chunk.  */
                  chunk_truncated = true;
                  اكسر;
                }
              إذا (k == chunk_no)
                أرجع;
              chunk_سرا += chunk_ناپ + (chunk_no < rem_بائٹاں);
              chunk_no++;
              إذا (chunk_سرا <= n_written)
                {
                  إذا (! k)
                    cwrite (true, عدم, 0);
                }
              وإلا
                اگلا = false;
            }
        }
    }

  إذا (chunk_truncated)
    chunk_no++;

  /* Ensure NUMBER files are created, which truncates
     any existing files or notifies any consumers on fifos.
     FIXME: Should we do this before EXIT_FAILURE?  */
  إذا (!k)
    طالما (chunk_no++ <= n)
      cwrite (true, عدم, 0);
}

/* -n K/N: Extract Kth of N chunks.  */

ساكن فراغ
بائٹاں_chunk_کڈھو (intmax_t k, intmax_t n, حرف *buf, idx_t bufsize,
                     ssize_t initial_read, off_t file_ناپ)
{
  off_t شروع;
  off_t سرا;

  affirm (0 < k && k <= n);

  شروع = (k - 1) * (file_ناپ / n) + MIN (k - 1, file_ناپ % n);
  سرا = k == n ? file_ناپ : k * (file_ناپ / n) + MIN (k, file_ناپ % n);

  إذا (شروع < initial_read)
    {
      memmove (buf, buf + شروع, initial_read - شروع);
      initial_read -= شروع;
    }
  وإلا
    {
      إذا (initial_read < شروع
          && lseek (STDIN_FILENO, شروع - initial_read, SEEK_CUR) < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
      initial_read = -1;
    }

  طالما (شروع < سرا)
    {
      ssize_t n_read;
      إذا (0 <= initial_read)
        {
          n_read = initial_read;
          initial_read = -1;
        }
      وإلا
        {
          n_read = اقرأ (STDIN_FILENO, buf, bufsize);
          إذا (n_read < 0)
            error (EXIT_FAILURE, errno, "%s", quotef (infile));
        }
      إذا (n_read == 0)
        اكسر; /* eof.  */
      n_read = MIN (n_read, سرا - شروع);
      إذا (بھریا_write (STDOUT_FILENO, buf, n_read) != n_read
          && ! ignorable (errno))
        error (EXIT_FAILURE, errno, "%s", quotef ("-"));
      شروع += n_read;
    }
}

تعريفنوع بنية دا_info
{
  حرف *دا_name;
  صحيح ofd;
  FILE *ofile;
  pid_t opid;
} دا_t;

تعداد
{
  OFD_NEW = -1,
  OFD_APPEND = -2
};

/* Rotate file descriptors when we're writing to more output files than we
   have available file descriptors.
   Return whether we came under file resource pressure.
   If so, it's probably best to close each file when finished with it.  */

ساكن bool
ofile_open (دا_t *files, idx_t i_check, idx_t nfiles)
{
  bool file_حد_بندی = false;

  إذا (files[i_check].ofd <= OFD_NEW)
    {
      صحيح fd;
      idx_t i_reopen = i_check ? i_check - 1 : nfiles - 1;

      /* Another process could have opened a file in between the calls to
         close and open, so we should keep trying until open succeeds or
         we've closed all of our files.  */
      طالما (true)
        {
          إذا (files[i_check].ofd == OFD_NEW)
            fd = create (files[i_check].دا_name);
          وإلا /* OFD_APPEND  */
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
              fd = open (files[i_check].دا_name,
                         O_WRONLY | O_ثنائی | O_APPEND | O_NONBLOCK);
            }

          إذا (0 <= fd)
            اكسر;

          إذا (!(errno == EMFILE || errno == ENFILE))
            error (EXIT_FAILURE, errno, "%s", quotef (files[i_check].دا_name));

          file_حد_بندی = true;

          /* Search backwards for an open file to close.  */
          طالما (files[i_reopen].ofd < 0)
            {
              i_reopen = i_reopen ? i_reopen - 1 : nfiles - 1;
              /* No more open files to close, exit with E[NM]FILE.  */
              إذا (i_reopen == i_check)
                error (EXIT_FAILURE, errno, "%s",
                       quotef (files[i_check].دا_name));
            }

          إذا (fclose (files[i_reopen].ofile) != 0)
            error (EXIT_FAILURE, errno, "%s", quotef (files[i_reopen].دا_name));
          files[i_reopen].ofile = عدم;
          files[i_reopen].ofd = OFD_APPEND;
        }

      files[i_check].ofd = fd;
      FILE *ofile = fdopen (fd, "a");
      إذا (!ofile)
        error (EXIT_FAILURE, errno, "%s", quotef (files[i_check].دا_name));
      files[i_check].ofile = ofile;
      files[i_check].opid = filter_pid;
      filter_pid = 0;
    }

  أرجع file_حد_بندی;
}

/* -n r/[K/]N: Divide file into N chunks in round robin fashion.
   Use BUF of size BUFSIZE for the buffer, and if allocating storage
   put its address into *FILESP to pacify -fsanitize=leak.
   When K == 0, we try to keep the files open in parallel.
   If we run out of file resources, then we revert
   to opening and closing each file for each line.  */

ساكن فراغ
سطریں_rr (intmax_t k, intmax_t n, حرف *buf, idx_t bufsize, دا_t **filesp)
{
  bool wrapped = false;
  bool wrote = false;
  bool file_حد_بندی;
  idx_t i_file;
  دا_t *files IF_LINT (= عدم);
  intmax_t سطر_no;

  إذا (k)
    سطر_no = 1;
  وإلا
    {
      إذا (IDX_MAX < n)
        xalloc_die ();
      files = *filesp = xinmalloc (n, حجم *files);

      /* Generate output file names. */
      لكل (i_file = 0; i_file < n; i_file++)
        {
          اگلا_file_name ();
          files[i_file].دا_name = xstrdup (outfile);
          files[i_file].ofd = OFD_NEW;
          files[i_file].ofile = عدم;
          files[i_file].opid = 0;
        }
      i_file = 0;
      file_حد_بندی = false;
    }

  طالما (true)
    {
      حرف *bp = buf, *eob;
      ssize_t n_read = اقرأ (STDIN_FILENO, buf, bufsize);
      إذا (n_read < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
      وإلا إذا (n_read == 0)
        اكسر; /* eof.  */
      eob = buf + n_read;

      طالما (bp != eob)
        {
          idx_t منزل_write;
          bool اگلا = false;

          /* Find end of line. */
          حرف *bp_باہرلا = memchr (bp, eolchar, eob - bp);
          إذا (bp_باہرلا)
            {
              bp_باہرلا++;
              اگلا = true;
            }
          وإلا
            bp_باہرلا = eob;
          منزل_write = bp_باہرلا - bp;

          إذا (k)
            {
              إذا (سطر_no == k && unbuffered)
                {
                  إذا (بھریا_write (STDOUT_FILENO, bp, منزل_write) != منزل_write)
                    write_error ();
                }
              وإلا إذا (سطر_no == k && fwrite (bp, منزل_write, 1, stdout) != 1)
                {
                  write_error ();
                }
              إذا (اگلا)
                سطر_no = (سطر_no == n) ? 1 : سطر_no + 1;
            }
          وإلا
            {
              /* Secure file descriptor. */
              file_حد_بندی |= ofile_open (files, i_file, n);
              إذا (unbuffered)
                {
                  /* Note writing to fd, rather than flushing the FILE gives
                     an 8% performance benefit, due to reduced data copying.  */
                  إذا (بھریا_write (files[i_file].ofd, bp, منزل_write) != منزل_write
                      && ! ignorable (errno))
                    error (EXIT_FAILURE, errno, "%s",
                           quotef (files[i_file].دا_name));
                }
              وإلا إذا (fwrite (bp, منزل_write, 1, files[i_file].ofile) != 1
                       && ! ignorable (errno))
                error (EXIT_FAILURE, errno, "%s",
                       quotef (files[i_file].دا_name));

              إذا (! ignorable (errno))
                wrote = true;

              إذا (file_حد_بندی)
                {
                  إذا (fclose (files[i_file].ofile) != 0)
                    error (EXIT_FAILURE, errno, "%s",
                           quotef (files[i_file].دا_name));
                  files[i_file].ofile = عدم;
                  files[i_file].ofd = OFD_APPEND;
                }
              إذا (اگلا && ++i_file == n)
                {
                  wrapped = true;
                  /* If no filters are accepting input, stop reading.  */
                  إذا (! wrote)
                    اذهب no_filters;
                  wrote = false;
                  i_file = 0;
                }
            }

          bp = bp_باہرلا;
        }
    }

no_filters:
  /* Ensure all files created, so that any existing files are truncated,
     and to signal any waiting fifo consumers.
     Also, close any open file descriptors.
     FIXME: Should we do this before EXIT_FAILURE?  */
  إذا (!k)
    {
      idx_t ceiling = wrapped ? n : i_file;
      لكل (i_file = 0; i_file < n; i_file++)
        {
          إذا (i_file >= ceiling && !elide_خالی_تھاواں_files)
            file_حد_بندی |= ofile_open (files, i_file, n);
          إذا (files[i_file].ofd >= 0)
            closeout (files[i_file].ofile, files[i_file].ofd,
                      files[i_file].opid, files[i_file].دا_name);
          files[i_file].ofd = OFD_APPEND;
        }
    }
}

#تعریف_کرو FAIL_ONLY_ONE_WAY()					\
  افعل								\
    {								\
      error (0, 0, _("cannot split in more than one way"));	\
      usage (EXIT_FAILURE);					\
    }								\
  طالما (0)

/* Report a string-to-integer conversion failure MSGID with ARG.  */

ساكن _Noreturn فراغ
strtoint_die (حرف ثابت *msgid, حرف ثابت *arg)
{
  error (EXIT_FAILURE, errno == EINVAL ? 0 : errno, "%s: %s",
         gettext (msgid), quote (arg));
}

/* Use OVERFLOW_OK when it is OK to ignore LONGINT_OVERFLOW errors, since the
   extreme value will do the right thing anyway on any practical platform.  */
#تعریف_کرو حد_توں_ودھ_OK LONGINT_حد_توں_ودھ

/* Parse ARG for number of bytes or lines.  The number can be followed
   by MULTIPLIERS, and the resulting value must be positive.
   If the number cannot be parsed, diagnose with MSG.
   Return the number parsed, or an INTMAX_MAX on overflow.  */

ساكن intmax_t
parse_n_units (حرف ثابت *arg, حرف ثابت *multipliers, حرف ثابت *msgid)
{
  intmax_t n;
  إذا (حد_توں_ودھ_OK < xstrtoimax (arg, عدم, 10, &n, multipliers) || n < 1)
    strtoint_die (msgid, arg);
  أرجع n;
}

/* Parse K/N syntax of chunk options.  */

ساكن فراغ
parse_chunk (intmax_t *k_units, intmax_t *n_units, حرف ثابت *arg)
{
  حرف *argend;
  strtol_error e = xstrtoimax (arg, &argend, 10, n_units, "");
  إذا (e == LONGINT_INVALID_SUFFIX_CHAR && *argend == '/')
    {
      *k_units = *n_units;
      *n_units = parse_n_units (argend + 1, "",
                                N_("invalid number of chunks"));
      إذا (! (0 < *k_units && *k_units <= *n_units))
        error (EXIT_FAILURE, 0, "%s: %s", _("invalid chunk number"),
               quote_mem (arg, argend - arg));
    }
  وإلا إذا (! (e <= حد_توں_ودھ_OK && 0 < *n_units))
    strtoint_die (N_("invalid number of chunks"), arg);
}


صحيح
مکھ (صحيح argc, حرف **argv)
{
  تعداد ونڈو_type ونڈو_type_2 = type_undef;
  idx_t وچ_blk_ناپ = 0;	/* optimal block size of input file device */
  idx_t ورقہ_ناپ = getpagesize ();
  intmax_t k_units = 0;
  intmax_t n_units = 0;

  ساكن حرف ثابت multipliers[] = "bEGKkMmPQRTYZ0";
  صحيح c;
  صحيح digits_optind = 0;
  off_t file_ناپ = OFF_T_MAX;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* Parse command line options.  */

  طالما (true)
    {
      /* This is the argv-index of the option we will read next.  */
      صحيح this_optind = optind ? optind : 1;

      c = getopt_long (argc, argv, "0123456789C:a:b:del:n:t:ux",
                       longopts, عدم);
      إذا (c == -1)
        اكسر;

      بدل (c)
        {
        حالة 'a':
          suffix_لمائی = xdectoimax (optarg, 0, IDX_MAX,
                                      "", _("invalid suffix length"), 0);
          اكسر;

        حالة ADDITIONAL_SUFFIX_OPTION:
          {
            صحيح suffix_len = لڑی_لمبائی (optarg);
            إذا (آخری_component (optarg) != optarg
                || (suffix_len && ISSLASH (optarg[suffix_len - 1])))
              {
                error (0, 0,
                       _("invalid suffix %s, contains directory separator"),
                       quote (optarg));
                usage (EXIT_FAILURE);
              }
          }
          additional_suffix = optarg;
          اكسر;

        حالة 'b':
          إذا (ونڈو_type_2 != type_undef)
            FAIL_ONLY_ONE_WAY ();
          ونڈو_type_2 = type_بائٹاں;
          n_units = parse_n_units (optarg, multipliers,
                                   N_("invalid number of bytes"));
          اكسر;

        حالة 'l':
          إذا (ونڈو_type_2 != type_undef)
            FAIL_ONLY_ONE_WAY ();
          ونڈو_type_2 = type_سطریں;
          n_units = parse_n_units (optarg, "", N_("invalid number of lines"));
          اكسر;

        حالة 'C':
          إذا (ونڈو_type_2 != type_undef)
            FAIL_ONLY_ONE_WAY ();
          ونڈو_type_2 = type_byteslines;
          n_units = parse_n_units (optarg, multipliers,
                                   N_("invalid number of lines"));
          اكسر;

        حالة 'n':
          إذا (ونڈو_type_2 != type_undef)
            FAIL_ONLY_ONE_WAY ();
          /* skip any whitespace */
          طالما (خلا_اے (منزل_uchar (*optarg)))
            optarg++;
          إذا (STRNCMP_LIT (optarg, "r/") == 0)
            {
              ونڈو_type_2 = type_rr;
              optarg += 2;
            }
          وإلا إذا (STRNCMP_LIT (optarg, "l/") == 0)
            {
              ونڈو_type_2 = type_chunk_سطریں;
              optarg += 2;
            }
          وإلا
            ونڈو_type_2 = type_chunk_بائٹاں;
          parse_chunk (&k_units, &n_units, optarg);
          اكسر;

        حالة 'u':
          unbuffered = true;
          اكسر;

        حالة 't':
          {
            حرف neweol = optarg[0];
            إذا (! neweol)
              error (EXIT_FAILURE, 0, _("empty record separator"));
            إذا (optarg[1])
              {
                إذا (streq (optarg, "\\0"))
                  neweol = '\0';
                وإلا
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
            إذا (0 <= eolchar && neweol != eolchar)
              {
                error (EXIT_FAILURE, 0,
                       _("multiple separator characters specified"));
              }

            eolchar = neweol;
          }
          اكسر;

        حالة '0':
        حالة '1':
        حالة '2':
        حالة '3':
        حالة '4':
        حالة '5':
        حالة '6':
        حالة '7':
        حالة '8':
        حالة '9':
          إذا (ونڈو_type_2 == type_undef)
            {
              ونڈو_type_2 = type_digits;
              n_units = 0;
            }
          إذا (ونڈو_type_2 != type_undef && ونڈو_type_2 != type_digits)
            FAIL_ONLY_ONE_WAY ();
          إذا (digits_optind != 0 && digits_optind != this_optind)
            n_units = 0;	/* More than one number given; ignore other. */
          digits_optind = this_optind;
          إذا (ckd_mul (&n_units, n_units, 10)
              || ckd_جوڑو (&n_units, n_units, c - '0'))
            n_units = INTMAX_MAX;
          اكسر;

        حالة 'd':
        حالة 'x':
          إذا (c == 'd')
            suffix_alphabet = "0123456789";
          وإلا
            suffix_alphabet = "0123456789abcdef";
          إذا (optarg)
            {
              إذا (لڑی_لمبائی (optarg) != strspn (optarg, suffix_alphabet))
                {
                  error (0, 0,
                         (c == 'd') ?
                           _("%s: invalid start value for numerical suffix") :
                           _("%s: invalid start value for hexadecimal suffix"),
                         quote (optarg));
                  usage (EXIT_FAILURE);
                }
              وإلا
                {
                  /* Skip any leading zero.  */
                  طالما (*optarg == '0' && *(optarg + 1) != '\0')
                    optarg++;
                  numeric_suffix_شروع = optarg;
                }
            }
          اكسر;

        حالة 'e':
          elide_خالی_تھاواں_files = true;
          اكسر;

        حالة FILTER_OPTION:
          filter_command = optarg;
          اكسر;

        حالة IO_BLKSIZE_OPTION:
          وچ_blk_ناپ = xnumtoumax (optarg, 10, 1,
                                    MIN (SYS_BUFSIZE_MAX,
                                         MIN (IDX_MAX, SIZE_MAX) - 1),
                                    multipliers, _("invalid IO block size"),
                                    0, XTOINT_MIN_RANGE);
          اكسر;

        حالة VERBOSE_OPTION:
          verbose = true;
          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  إذا (k_units != 0 && filter_command)
    {
      error (0, 0, _("--filter does not process a chunk extracted to "
                     "standard output"));
      usage (EXIT_FAILURE);
    }

  /* Handle default case.  */
  إذا (ونڈو_type_2 == type_undef)
    {
      ونڈو_type_2 = type_سطریں;
      n_units = 1000;
    }

  إذا (n_units == 0)
    {
      error (0, 0, _("invalid number of lines: %s"), quote ("0"));
      usage (EXIT_FAILURE);
    }

  إذا (eolchar < 0)
    eolchar = '\n';

  سیٹ_suffix_لمائی (n_units, ونڈو_type_2);

  /* Get out the filename arguments.  */

  إذا (optind < argc)
    infile = argv[optind++];

  إذا (optind < argc)
    outbase = argv[optind++];

  إذا (optind < argc)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind]));
      usage (EXIT_FAILURE);
    }

  /* Check that the suffix length is large enough for the numerical
     suffix start value.  */
  إذا (numeric_suffix_شروع && لڑی_لمبائی (numeric_suffix_شروع) > suffix_لمائی)
    {
      error (0, 0, _("numerical suffix start value is too large "
                     "for the suffix length"));
      usage (EXIT_FAILURE);
    }

  /* Open the input file.  */
  إذا (! streq (infile, "-")
      && fd_reopen (STDIN_FILENO, infile, O_RDONLY, 0) < 0)
    error (EXIT_FAILURE, errno, _("cannot open %s for reading"),
           quoteaf (infile));

  /* Binary I/O is safer when byte counts are used.  */
  xset_ثنائی_منوال (STDIN_FILENO, O_ثنائی);

  /* Advise the kernel of our access pattern.  */
  fdadvise (STDIN_FILENO, 0, 0, FADVISE_SEQUENTIAL);

  /* Get the optimal block size of input device and make a buffer.  */

  إذا (fstat (STDIN_FILENO, &وچ_stat_buf) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (infile));

  إذا (وچ_blk_ناپ == 0)
    {
      وچ_blk_ناپ = io_blksize (&وچ_stat_buf);
      إذا (SYS_BUFSIZE_MAX < وچ_blk_ناپ)
        وچ_blk_ناپ = SYS_BUFSIZE_MAX;
    }

  حرف *buf = xalignalloc (ورقہ_ناپ, وچ_blk_ناپ + 1);
  ssize_t initial_read = -1;

  إذا (ونڈو_type_2 == type_chunk_بائٹاں || ونڈو_type_2 == type_chunk_سطریں)
    {
      file_ناپ = داخلہ_file_ناپ (STDIN_FILENO, &وچ_stat_buf,
                                   buf, وچ_blk_ناپ);
      إذا (file_ناپ < 0)
        error (EXIT_FAILURE, errno, _("%s: cannot determine file size"),
               quotef (infile));
      initial_read = MIN (file_ناپ, وچ_blk_ناپ);
    }

  /* When filtering, closure of one pipe must not terminate the process,
     as there may still be other streams expecting input from us.  */
  إذا (filter_command)
    default_SIGPIPE = إشارة (SIGPIPE, SIG_IGN) == SIG_DFL;

  بدل (ونڈو_type_2)
    {
    حالة type_digits:
    حالة type_سطریں:
      سطریں_ونڈو (n_units, buf, وچ_blk_ناپ);
      اكسر;

    حالة type_بائٹاں:
      بائٹاں_ونڈو (n_units, 0, buf, وچ_blk_ناپ, -1, 0);
      اكسر;

    حالة type_byteslines:
      سطر_بائٹاں_ونڈو (n_units, buf, وچ_blk_ناپ);
      اكسر;

    حالة type_chunk_بائٹاں:
      إذا (k_units == 0)
        بائٹاں_ونڈو (file_ناپ / n_units, file_ناپ % n_units,
                     buf, وچ_blk_ناپ, initial_read, n_units);
      وإلا
        بائٹاں_chunk_کڈھو (k_units, n_units, buf, وچ_blk_ناپ, initial_read,
                             file_ناپ);
      اكسر;

    حالة type_chunk_سطریں:
      سطریں_chunk_ونڈو (k_units, n_units, buf, وچ_blk_ناپ, initial_read,
                         file_ناپ);
      اكسر;

    حالة type_rr:
      /* Note, this is like 'sed -n ${k}~${n}p' when k > 0,
         but the functionality is provided for symmetry.  */
      {
        دا_t *files;
        سطریں_rr (k_units, n_units, buf, وچ_blk_ناپ, &files);
      }
      اكسر;

    حالة type_undef: افتراضي:
      affirm (false);
    }

  إذا (close (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (infile));
  closeout (عدم, output_desc, filter_pid, outfile);

  main_exit (EXIT_SUCCESS);
}
