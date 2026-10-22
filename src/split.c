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
#تعریف_کرو پروگرام_NAME "split"

#تعریف_کرو AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("Richard M. Stallman")

/* Shell command to filter through, instead of creating files.  */
ساکن حرف ثابت *filter_command;

/* Process ID of the filter.  */
ساکن pid_t filter_pid;

ساکن ساخت
{
  /* The number of open pipes.  */
  صحیح n_pipes;

  /* The number of allocated pipes.  */
  idx_t n_pipes_alloc;

  /* Array of open pipes.  */
  صحیح *pipes COUNTED_BY (n_pipes);
} open_pipes;

/* Whether SIGPIPE has the default action, when --filter is used.  */
ساکن bool default_SIGPIPE;

/* Base name of output files.  */
ساکن حرف ثابت *outbase = "x";

/* Name of output files.  */
ساکن حرف *outfile;

/* Pointer to the end of the prefix in OUTFILE.
   Suffixes are inserted here.  */
ساکن حرف *outfile_mid;

/* Generate new suffix when suffixes are exhausted.  */
ساکن bool suffix_auto = true;

/* Length of OUTFILE's suffix.  */
ساکن idx_t suffix_لمبائی;

/* Alphabet of characters to use in suffix.  */
ساکن حرف ثابت *suffix_alphabet = "abcdefghijklmnopqrstuvwxyz";

/* Numerical suffix start value.  */
ساکن حرف ثابت *numeric_suffix_آغاز;

/* Additional suffix to append to output file names.  */
ساکن حرف ثابت *additional_suffix;

/* Name of input file.  May be "-".  */
ساکن حرف ثابت *infile = "-";

/* stat buf for input file.  */
ساکن ساخت stat اندر_stat_buf;

/* Descriptor on which output file is open.  */
ساکن صحیح output_desc = -1;

/* If true, print a diagnostic on standard error just before each
   output file is opened. */
ساکن bool verbose;

/* If true, don't generate zero length output files. */
ساکن bool elide_خالی_خانے_files;

/* If true, in round robin mode, immediately copy
   input to output, which is much slower, so disabled by default.  */
ساکن bool unbuffered;

/* The character marking end of line.  Defaults to \n below.  */
ساکن صحیح eolchar = -1;

/* The split mode to use.  */
شمار بانٹو_type
{
  type_undef, type_بائٹیں, type_byteslines, type_سطریں, type_digits,
  type_chunk_بائٹیں, type_chunk_سطریں, type_rr
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
شمار
{
  VERBOSE_OPTION = CHAR_MAX + 1,
  FILTER_OPTION,
  IO_BLKSIZE_OPTION,
  ADDITIONAL_SUFFIX_OPTION
};

ساکن ساخت option ثابت longopts[] =
{
  {"bytes", required_دلیل, تہی, 'b'},
  {"lines", required_دلیل, تہی, 'l'},
  {"line-bytes", required_دلیل, تہی, 'C'},
  {"number", required_دلیل, تہی, 'n'},
  {"elide-empty-files", no_دلیل, تہی, 'e'},
  {"unbuffered", no_دلیل, تہی, 'u'},
  {"suffix-length", required_دلیل, تہی, 'a'},
  {"additional-suffix", required_دلیل, تہی,
   ADDITIONAL_SUFFIX_OPTION},
  {"numeric-suffixes", optional_دلیل, تہی, 'd'},
  {"hex-suffixes", optional_دلیل, تہی, 'x'},
  {"filter", required_دلیل, تہی, FILTER_OPTION},
  {"verbose", no_دلیل, تہی, VERBOSE_OPTION},
  {"separator", required_دلیل, تہی, 't'},
  {"-io-blksize", required_دلیل, تہی,
   IO_BLKSIZE_OPTION}, /* do not document */
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0}
};

/* Return true if the errno value, ERR, is ignorable.  */
ساکن اندرونی bool
ignorable (صحیح err)
{
  واپس filter_command && err == EPIPE;
}

ساکن خالی
سیٹ_suffix_لمبائی (intmax_t n_units, شمار بانٹو_type بانٹو_type_2)
{
#تعریف_کرو DEFAULT_SUFFIX_لمبائی 2

  صحیح suffix_لمبائی_needed = 0;

  /* The suffix auto length feature is incompatible with
     a user specified start value as the generated suffixes
     are not all consecutive.  */
  اگر (numeric_suffix_آغاز)
    suffix_auto = false;

  /* Auto-calculate the suffix length if the number of files is given.  */
  اگر (بانٹو_type_2 == type_chunk_بائٹیں || بانٹو_type_2 == type_chunk_سطریں
      || بانٹو_type_2 == type_rr)
    {
      intmax_t n_units_انتہا = n_units - 1;
      اگر (numeric_suffix_آغاز)
        {
          intmax_t n_آغاز;
          strtol_error e = xstrtoimax (numeric_suffix_آغاز, تہی, 10,
                                       &n_آغاز, "");
          اگر (e == LONGINT_OK && n_آغاز < n_units)
            {
              /* Restrict auto adjustment so we don't keep
                 incrementing a suffix size arbitrarily,
                 as that would break sort order for files
                 generated from multiple split runs.  */
              اگر (ckd_جوڑو (&n_units_انتہا, n_units_انتہا, n_آغاز))
                n_units_انتہا = INTMAX_MAX;
            }

        }
      idx_t alphabet_len = لڑی_لمبائی (suffix_alphabet);
      کرو
        suffix_لمبائی_needed++;
      جب (n_units_انتہا /= alphabet_len);

      suffix_auto = false;
    }

  اگر (suffix_لمبائی)            /* set by user */
    {
      اگر (suffix_لمبائی < suffix_لمبائی_needed)
        error (EXIT_FAILURE, 0,
               _("the suffix length needs to be at least %d"),
               suffix_لمبائی_needed);
      suffix_auto = false;
      واپس;
    }
  ورنہ
    suffix_لمبائی = MAX (DEFAULT_SUFFIX_لمبائی, suffix_لمبائی_needed);
}

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("\
Usage: %s [OPTION]... [FILE [سابقہ_2]]\n\
"),
              پروگرام_name);
      fputs (_("\
Output pieces کا FILE مقصد PREFIXaa, PREFIXab, ...;\n\
طےشدہ سائز is 1000 سطریں, and طےشدہ سابقہ_2 is 'x'.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oprintf (_("\
  -a, --suffix-لمبائی=N\n\
         generate suffixes کا لمبائی N (طےشدہ %d)\n\
"), DEFAULT_SUFFIX_لمبائی);
      oputs (_("\
      --additional-suffix=SUFFIX\n\
         append an additional SUFFIX مقصد file نام\n\
"));
      oputs (_("\
  -b, --بائٹیں=سائز_2\n\
         put سائز_2 بائٹیں فی output file\n\
"));
      oputs (_("\
  -C, --سطر-بائٹیں=سائز_2\n\
         put at most سائز_2 بائٹیں کا اندراجات فی output file\n\
"));
      oputs (_("\
  -d\n\
         use numeric suffixes starting at 0, not alphabetic\n\
"));
      oputs (_("\
      --numeric-suffixes[=FROM]\n\
         same as -d, but allow setting the آغاز قدر\n\
"));
      oputs (_("\
  -x\n\
         use hex suffixes starting at 0, not alphabetic\n\
"));
      oputs (_("\
      --hex-suffixes[=FROM]\n\
         same as -x, but allow setting the آغاز قدر\n\
"));
      oputs (_("\
  -e, --elide-خالی_خانے-files\n\
         کرو not generate خالی_خانے output files with '-n'\n\
"));
      oputs (_("\
      --filter=COMMAND\n\
         اكتب مقصد shell COMMAND; file name is $FILE\n\
"));
      oputs (_("\
  -l, --سطریں=نمبر_2\n\
         put نمبر_2 سطریں/اندراجات فی output file\n\
"));
      oputs (_("\
  -n, --نمبر=CHUNKS\n\
         generate CHUNKS output files; see explanation نیچے\n\
"));
      oputs (_("\
  -t, --separator=SEP\n\
         use SEP instead کا newline as the ریکارڈ separator;\n\
         '\\0' (zero) specifies the NUL character\n\
"));
      oputs (_("\
  -u, --unbuffered\n\
         immediately copy داخلہ مقصد output with '-n r/...'\n\
"));
      oputs (_("\
      --verbose\n\
         print a diagnostic just before each output file is opened\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      emit_سائز_note ();
      fputs (_("\n\
CHUNKS may be:\n\
  N       بانٹو into N files based on سائز کا داخلہ\n\
  K/N     output Kth کا N مقصد standard output\n\
  l/N     بانٹو into N files without splitting سطریں/اندراجات\n\
  l/K/N   output Kth کا N مقصد standard output without splitting سطریں/اندراجات\n\
  r/N     like 'l' but use باری_2 robin distribution\n\
  r/K/N   likewise but only output Kth کا N مقصد standard output\n\
"), stdout);
      fputs (_("\n\
-n (except -nr) will بفر مقصد $TMPDIR, defaulting مقصد /tmp,\n\
اگر the داخلہ سائز cannot easily be determined.\n\
"), stdout);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

/* Copy the data in FD to a temporary file, then make that file FD.
   Use BUF, of size BUFSIZE, to copy.  Return the number of
   bytes copied, or -1 (setting errno) on error.  */
ساکن off_t
copy_مقصد_tmpfile (صحیح fd, حرف *buf, idx_t bufsize)
{
  FILE *tmp;
  اگر (!عارضی_دھارا (&tmp, تہی))
    واپس -1;
  off_t copied = 0;
  off_t r;

  جب (0 < (r = اقرأ (fd, buf, bufsize)))
    {
      اگر (fwrite (buf, 1, r, tmp) != r)
        واپس -1;
      اگر (ckd_جوڑو (&copied, copied, r))
        {
          errno = EOVERFLOW;
          واپس -1;
        }
    }

  اگر (r < 0)
    واپس r;
  r = dup2 (fileno (tmp), fd);
  اگر (r < 0)
    واپس r;
  اگر (fclose (tmp) < 0)
    واپس -1;
  واپس copied;
}

/* Return the number of bytes that can be read from FD with status ST.
   Store up to the first BUFSIZE bytes of the file's data into BUF,
   and advance the file position by the number of bytes read.  On
   input error, set errno and return -1.  */

ساکن off_t
داخلہ_file_سائز (صحیح fd, ساخت stat ثابت *st, حرف *buf, idx_t bufsize)
{
  off_t سائز = 0;
  کرو
    {
      ssize_t n_read = اقرأ (fd, buf + سائز, bufsize - سائز);
      اگر (n_read <= 0)
        واپس n_read < 0 ? n_read : سائز;
      سائز += n_read;
    }
  جب (سائز < bufsize);

  off_t cur, انتہا;
  اگر ((usable_st_سائز (st) && st->st_سائز < سائز)
      || (cur = lseek (fd, 0, SEEK_CUR)) < 0
      || cur < سائز /* E.g., /dev/zero on GNU/Linux.  */
      || (انتہا = lseek (fd, 0, SEEK_END)) < 0)
    {
      حرف *tmpbuf = xmalloc (bufsize);
      انتہا = copy_مقصد_tmpfile (fd, tmpbuf, bufsize);
      آزاد (tmpbuf);
      اگر (انتہا < 0)
        واپس انتہا;
      cur = 0;
    }

  اگر (انتہا == OFF_T_MAX /* E.g., /dev/zero on GNU/Hurd.  */
      || (cur < انتہا && ckd_جوڑو (&سائز, سائز, انتہا - cur)))
    {
      errno = EOVERFLOW;
      واپس -1;
    }

  اگر (cur < انتہا)
    {
      off_t r = lseek (fd, cur, SEEK_SET);
      اگر (r < 0)
        واپس r;
    }

  واپس سائز;
}

/* Compute the next sequential output file name and store it into the
   string 'outfile'.  */

ساکن خالی
اگلا_file_name (خالی)
{
  /* Index in suffix_alphabet of each character in the suffix.  */
  ساکن idx_t *sufindex;
  ساکن idx_t outbase_لمبائی;
  ساکن idx_t outfile_لمبائی;
  ساکن idx_t addsuf_لمبائی;

  اگر (! outfile)
    {
      bool حد_سے_تجاوز, widen;

new_name:
      widen = !! outfile_لمبائی;

      اگر (! widen)
        {
          /* Allocate and initialize the first file name.  */

          outbase_لمبائی = لڑی_لمبائی (outbase);
          addsuf_لمبائی = additional_suffix ? لڑی_لمبائی (additional_suffix) : 0;
          حد_سے_تجاوز = ckd_جوڑو (&outfile_لمبائی, outbase_لمبائی + addsuf_لمبائی,
                              suffix_لمبائی);
        }
      ورنہ
        {
          /* Reallocate and initialize a new wider file name.
             We do this by subsuming the unchanging part of
             the generated suffix into the prefix (base), and
             reinitializing the now one longer suffix.  */

          حد_سے_تجاوز = ckd_جوڑو (&outfile_لمبائی, outfile_لمبائی, 2);
          suffix_لمبائی++;
        }

      idx_t outfile_سائز;
      حد_سے_تجاوز |= ckd_جوڑو (&outfile_سائز, outfile_لمبائی, 1);
      اگر (حد_سے_تجاوز)
        xalloc_die ();
      outfile = xirealloc (outfile, outfile_سائز);

      اگر (! widen)
        حافظہ_نقل (outfile, outbase, outbase_لمبائی);
      ورنہ
        {
          /* Append the last alphabet character to the file name prefix.  */
          outfile[outbase_لمبائی] = suffix_alphabet[sufindex[0]];
          outbase_لمبائی++;
        }

      outfile_mid = outfile + outbase_لمبائی;
      memset (outfile_mid, suffix_alphabet[0], suffix_لمبائی);
      اگر (additional_suffix)
        حافظہ_نقل (outfile_mid + suffix_لمبائی, additional_suffix, addsuf_لمبائی);
      outfile[outfile_لمبائی] = 0;

      آزاد (sufindex);
      sufindex = xicalloc (suffix_لمبائی, حجم *sufindex);

      اگر (numeric_suffix_آغاز)
        {
          affirm (! widen);

          /* Update the output file name.  */
          idx_t i = لڑی_لمبائی (numeric_suffix_آغاز);
          حافظہ_نقل (outfile_mid + suffix_لمبائی - i, numeric_suffix_آغاز, i);

          /* Update the suffix index.  */
          idx_t *sufindex_انتہا = sufindex + suffix_لمبائی;
          جب (i-- != 0)
            *--sufindex_انتہا = numeric_suffix_آغاز[i] - '0';
        }

#اگر ! _POSIX_NO_TRUNC && HAVE_PATHCONF && defined _PC_NAME_MAX
      /* POSIX requires that if the output file name is too long for
         its directory, 'split' must fail without creating any files.
         This must be checked for explicitly on operating systems that
         silently truncate file names.  */
      {
        حرف *dir = dir_name (outfile);
        طویل name_max = pathconf (dir, _PC_NAME_MAX);
        اگر (0 <= name_max && name_max < بنیاد_len (آخری_component (outfile)))
          error (EXIT_FAILURE, ENAMETOOLONG, "%s", quotef (outfile));
        آزاد (dir);
      }
#اگر_ختم
    }
  ورنہ
    {
      /* Increment the suffix in place, if possible.  */

      idx_t i = suffix_لمبائی;
      جب (i-- != 0)
        {
          sufindex[i]++;
          اگر (suffix_auto && i == 0 && ! suffix_alphabet[sufindex[0] + 1])
            جاؤ new_name;
          outfile_mid[i] = suffix_alphabet[sufindex[i]];
          اگر (outfile_mid[i])
            واپس;
          sufindex[i] = 0;
          outfile_mid[i] = suffix_alphabet[sufindex[i]];
        }
      error (EXIT_FAILURE, 0, _("output file suffixes exhausted"));
    }
}

/* Create or truncate a file.  */

ساکن صحیح
create (حرف ثابت *name)
{
  اگر (!filter_command)
    {
      اگر (verbose)
        fprintf (stdout, _("creating file %s\n"), quoteaf (name));

      صحیح oflags = O_WRONLY | O_CREAT | O_ثنائی;
      صحیح fd = open (name, oflags | O_EXCL, منوال_RW_UGO);
      اگر (0 <= fd || errno != EEXIST)
        واپس fd;
      fd = open (name, oflags, منوال_RW_UGO);
      اگر (fd < 0)
        واپس fd;
      ساخت stat برآمد_stat_buf;
      اگر (fstat (fd, &برآمد_stat_buf) != 0)
        error (EXIT_FAILURE, errno, _("failed to stat %s"), quoteaf (name));
      اگر (psame_inode (&اندر_stat_buf, &برآمد_stat_buf))
        error (EXIT_FAILURE, 0, _("%s would overwrite input; aborting"),
               quoteaf (name));
      اگر (ftruncate (fd, 0) < 0
          && (S_ISREG (برآمد_stat_buf.st_منوال) || S_TYPEISSHM (&برآمد_stat_buf)))
        error (EXIT_FAILURE, errno, _("%s: error truncating"), quotef (name));

      واپس fd;
    }
  ورنہ
    {
      اگر (setenv ("FILE", name, 1) != 0)
        error (EXIT_FAILURE, errno,
               _("failed to set FILE environment variable"));
      اگر (verbose)
        fprintf (stdout, _("executing with FILE=%s\n"), quotef (name));

      صحیح نتیجہ;
      صحیح fd_pair[2];
      pid_t بچہ_pid;

      posix_spawnattr_t attr;
      posix_spawn_file_actions_t actions;

      sigset_t سیٹ_2;
      sigemptyset (&سیٹ_2);
      اگر (default_SIGPIPE)
        sigaddset (&سیٹ_2, SIGPIPE);

      اگر (   (نتیجہ = posix_spawnattr_init (&attr))
          || (نتیجہ = posix_spawnattr_setflags (&attr,
                                                 (POSIX_SPAWN_USEVFORK
                                                  | POSIX_SPAWN_SETSIGDEF)))
          || (نتیجہ = posix_spawnattr_setsigdefault (&attr, &سیٹ_2))
          || (نتیجہ = posix_spawn_file_actions_init (&actions))
         )
        error (EXIT_FAILURE, نتیجہ, _("posix_spawn initialization failed"));

      اگر (pipe (fd_pair) != 0)
        error (EXIT_FAILURE, errno, _("failed to create pipe"));

      /* We have to close any pipes that were opened during an
         earlier call, otherwise this process will be holding a
         write-pipe that will prevent the earlier process from
         reading an EOF on the corresponding read-pipe.  */
      برائے (صحیح i = 0; i < open_pipes.n_pipes; ++i)
        اگر ((نتیجہ = posix_spawn_file_actions_addclose (&actions,
                                                         open_pipes.pipes[i])))
          توڑو;

      اگر (   نتیجہ
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
      اگر (shell_prog == تہی)
        shell_prog = "/bin/sh";
      حرف ثابت *ثابت argv[] = { آخری_component (shell_prog), "-c",
                                   filter_command, تہی };

      نتیجہ = posix_spawn (&بچہ_pid, shell_prog, &actions, &attr,
                            (حرف * ثابت *) argv, environ);
      اگر (نتیجہ != 0)
        error (EXIT_FAILURE, errno, _("failed to run command: \"%s -c %s\""),
               shell_prog, filter_command);

      posix_spawnattr_destroy (&attr);
      posix_spawn_file_actions_destroy (&actions);

      اگر (close (fd_pair[0]) != 0)
        error (EXIT_FAILURE, errno, _("failed to close input pipe"));
      filter_pid = بچہ_pid;
      اگر (open_pipes.n_pipes == open_pipes.n_pipes_alloc)
        open_pipes.pipes = xpalloc (open_pipes.pipes, &open_pipes.n_pipes_alloc,
                                    1, MIN (زیادہ_سے_زیادہ_عدد, IDX_MAX),
                                    حجم *open_pipes.pipes);
      idx_t ثابت pipe_idx = open_pipes.n_pipes++;
      open_pipes.pipes[pipe_idx] = fd_pair[1];
      واپس fd_pair[1];
    }
}

/* Close the output file, and do any associated cleanup.
   If FP and FD are both specified, they refer to the same open file;
   in this case FP is closed, but FD is still used in cleanup.  */
ساکن خالی
closeout (FILE *fp, صحیح fd, pid_t pid, حرف ثابت *name)
{
  اگر (fp != تہی && fclose (fp) != 0 && ! ignorable (errno))
    error (EXIT_FAILURE, errno, "%s", quotef (name));
  اگر (fd >= 0)
    {
      اگر (fp == تہی && close (fd) < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (name));
      برائے (صحیح j = 0; j < open_pipes.n_pipes; ++j)
        {
          اگر (open_pipes.pipes[j] == fd)
            {
              idx_t ثابت آخری_idx = open_pipes.n_pipes - 1;
              open_pipes.pipes[j] = open_pipes.pipes[آخری_idx];
              open_pipes.n_pipes--;
              توڑو;
            }
        }
    }
  اگر (pid > 0)
    {
      صحیح wstatus;
      اگر (waitpid (pid, &wstatus, 0) < 0)
        error (EXIT_FAILURE, errno, _("waiting for child process"));
      ورنہ اگر (WIFSIGNALED (wstatus))
        {
          صحیح sig = WTERMSIG (wstatus);
          اگر (sig != SIGPIPE)
            {
              حرف signame[MAX (SIG2STR_MAX, INT_BUFSIZE_BOUND (صحیح))];
              اگر (sig2str (sig, signame) != 0)
                sprintf (signame, "%d", sig);
              error (sig + 128, 0,
                     _("with FILE=%s, signal %s from command: %s"),
                     quotef (name), signame, filter_command);
            }
        }
      ورنہ اگر (WIFEXITED (wstatus))
        {
          صحیح ex = WEXITSTATUS (wstatus);
          اگر (ex != 0)
            error (ex, 0, _("with FILE=%s, exit %d from command: %s"),
                   quotef (name), ex, filter_command);
        }
      ورنہ
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

ساکن bool
cwrite (bool new_file_flag, حرف ثابت *bp, idx_t بائٹیں)
{
  اگر (new_file_flag)
    {
      اگر (!bp && بائٹیں == 0 && elide_خالی_خانے_files)
        واپس true;
      closeout (تہی, output_desc, filter_pid, outfile);
      اگلا_file_name ();
      output_desc = create (outfile);
      اگر (output_desc < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (outfile));
    }

  اگر (بھرا_write (output_desc, bp, بائٹیں) == بائٹیں)
    واپس true;
  ورنہ
    {
      اگر (! ignorable (errno))
        error (EXIT_FAILURE, errno, "%s", quotef (outfile));
      واپس false;
    }
}

/* Split into pieces of exactly N_BYTES bytes.
   However, the first REM_BYTES pieces should be 1 byte longer.
   Use buffer BUF, whose size is BUFSIZE.
   If INITIAL_READ is nonnegative,
   BUF contains the first INITIAL_READ input bytes.  */

ساکن خالی
بائٹیں_بانٹو (intmax_t n_بائٹیں, intmax_t rem_بائٹیں,
             حرف *buf, idx_t bufsize, ssize_t initial_read,
             intmax_t max_files)
{
  bool new_file_flag = true;
  bool filter_ok = true;
  intmax_t opened = 0;
  intmax_t مقصد_write = n_بائٹیں + (0 < rem_بائٹیں);
  bool eof = ! مقصد_write;

  جب (! eof)
    {
      ssize_t n_read;
      اگر (0 <= initial_read)
        {
          n_read = initial_read;
          initial_read = -1;
          eof = n_read < bufsize;
        }
      ورنہ
        {
          اگر (! filter_ok
              && 0 <= lseek (STDIN_FILENO, مقصد_write, SEEK_CUR))
            {
              مقصد_write = n_بائٹیں + (opened + 1 < rem_بائٹیں);
              new_file_flag = true;
            }

          n_read = اقرأ (STDIN_FILENO, buf, bufsize);
          اگر (n_read < 0)
            error (EXIT_FAILURE, errno, "%s", quotef (infile));
          eof = n_read == 0;
        }
      حرف *bp_برآمد = buf;
      جب (0 < مقصد_write && مقصد_write <= n_read)
        {
          اگر (filter_ok || new_file_flag)
            filter_ok = cwrite (new_file_flag, bp_برآمد, مقصد_write);
          opened += new_file_flag;
          new_file_flag = !max_files || (opened < max_files);
          اگر (! filter_ok && ! new_file_flag)
            {
              /* If filters no longer accepting input, stop reading.  */
              n_read = 0;
              eof = true;
              توڑو;
            }
          bp_برآمد += مقصد_write;
          n_read -= مقصد_write;
          مقصد_write = n_بائٹیں + (opened < rem_بائٹیں);
        }
      اگر (0 < n_read)
        {
          اگر (filter_ok || new_file_flag)
            filter_ok = cwrite (new_file_flag, bp_برآمد, n_read);
          opened += new_file_flag;
          new_file_flag = false;
          اگر (! filter_ok && opened == max_files)
            {
              /* If filters no longer accepting input, stop reading.  */
              توڑو;
            }
          مقصد_write -= n_read;
        }
    }

  /* Ensure NUMBER files are created, which truncates
     any existing files or notifies any consumers on fifos.
     FIXME: Should we do this before EXIT_FAILURE?  */
  جب (opened++ < max_files)
    cwrite (true, تہی, 0);
}

/* Split into pieces of exactly N_LINES lines.
   Use buffer BUF, whose size is BUFSIZE.  */

ساکن خالی
سطریں_بانٹو (intmax_t n_سطریں, حرف *buf, idx_t bufsize)
{
  ssize_t n_read;
  حرف *bp, *bp_برآمد, *eob;
  bool new_file_flag = true;
  intmax_t n = 0;

  کرو
    {
      n_read = اقرأ (STDIN_FILENO, buf, bufsize);
      اگر (n_read < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
      bp = bp_برآمد = buf;
      eob = bp + n_read;
      *eob = eolchar;
      جب (true)
        {
          bp = rawmemchr (bp, eolchar);
          اگر (bp == eob)
            {
              اگر (eob != bp_برآمد) /* do not write 0 bytes! */
                {
                  idx_t len = eob - bp_برآمد;
                  cwrite (new_file_flag, bp_برآمد, len);
                  new_file_flag = false;
                }
              توڑو;
            }

          ++bp;
          اگر (++n >= n_سطریں)
            {
              cwrite (new_file_flag, bp_برآمد, bp - bp_برآمد);
              bp_برآمد = bp;
              new_file_flag = true;
              n = 0;
            }
        }
    }
  جب (n_read);
}

/* Split into pieces that are as large as possible while still not more
   than N_BYTES bytes, and are split on line boundaries except
   where lines longer than N_BYTES bytes occur. */

ساکن خالی
سطر_بائٹیں_بانٹو (intmax_t n_بائٹیں, حرف *buf, idx_t bufsize)
{
  ssize_t n_read;
  intmax_t n_برآمد = 0;      /* for each split.  */
  idx_t n_hold = 0;
  حرف *hold = تہی;        /* for lines > bufsize.  */
  idx_t hold_سائز = 0;
  bool بانٹو_سطر = false;  /* Whether a \n was output in a split.  */

  کرو
    {
      n_read = اقرأ (STDIN_FILENO, buf, bufsize);
      اگر (n_read < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
      idx_t n_بایاں = n_read;
      حرف *sob = buf;
      جب (n_بایاں)
        {
          idx_t بانٹو_rest = 0;
          حرف *eoc = تہی;
          حرف *eol;

          /* Determine End Of Chunk and/or End of Line,
             which are used below to select what to write or buffer.  */
          اگر (n_بائٹیں - n_برآمد - n_hold <= n_بایاں)
            {
              /* Have enough for split.  */
              بانٹو_rest = n_بائٹیں - n_برآمد - n_hold;
              eoc = sob + بانٹو_rest - 1;
              eol = memrchr (sob, eolchar, بانٹو_rest);
            }
          ورنہ
            eol = memrchr (sob, eolchar, n_بایاں);

          /* Output hold space if possible.  */
          اگر (n_hold && !(!eol && n_برآمد))
            {
              cwrite (n_برآمد == 0, hold, n_hold);
              n_برآمد += n_hold;
              n_hold = 0;
            }

          /* Output to eol if present.  */
          اگر (eol)
            {
              بانٹو_سطر = true;
              idx_t n_write = eol - sob + 1;
              cwrite (n_برآمد == 0, sob, n_write);
              n_برآمد += n_write;
              n_بایاں -= n_write;
              sob += n_write;
              اگر (eoc)
                بانٹو_rest -= n_write;
            }

          /* Output to eoc or eob if possible.  */
          اگر (n_بایاں && !بانٹو_سطر)
            {
              idx_t n_write = eoc ? بانٹو_rest : n_بایاں;
              cwrite (n_برآمد == 0, sob, n_write);
              n_برآمد += n_write;
              n_بایاں -= n_write;
              sob += n_write;
              اگر (eoc)
                بانٹو_rest -= n_write;
            }

          /* Update hold if needed.  */
          اگر ((eoc && بانٹو_rest) || (!eoc && n_بایاں))
            {
              idx_t n_buf = eoc ? بانٹو_rest : n_بایاں;
              اگر (hold_سائز - n_hold < n_buf)
                hold = xpalloc (hold, &hold_سائز, n_buf - (hold_سائز - n_hold),
                                -1, حجم *hold);
              حافظہ_نقل (hold + n_hold, sob, n_buf);
              n_hold += n_buf;
              n_بایاں -= n_buf;
              sob += n_buf;
            }

          /* Reset for new split.  */
          اگر (eoc)
            {
              n_برآمد = 0;
              بانٹو_سطر = false;
            }
        }
    }
  جب (n_read);

  /* Handle no eol at end of file.  */
  اگر (n_hold)
    cwrite (n_برآمد == 0, hold, n_hold);

  آزاد (hold);
}

/* -n l/[K/]N: Write lines to files of approximately file size / N.
   The file is partitioned into file size / N sized portions, with the
   last assigned any excess.  If a line _starts_ within a partition
   it is written completely to the corresponding file.  Since lines
   are not split even if they overlap a partition, the files written
   can be larger or smaller than the partition size, and even empty
   if a line is so long as to completely overlap the partition.  */

ساکن خالی
سطریں_chunk_بانٹو (intmax_t k, intmax_t n, حرف *buf, idx_t bufsize,
                   ssize_t initial_read, off_t file_سائز)
{
  affirm (n && k <= n);

  intmax_t rem_بائٹیں = file_سائز % n;
  off_t chunk_سائز = file_سائز / n;
  intmax_t chunk_no = 1;
  off_t chunk_انتہا = chunk_سائز + (0 < rem_بائٹیں);
  off_t n_written = 0;
  bool new_file_flag = true;
  bool chunk_truncated = false;

  اگر (k > 1 && 0 < file_سائز)
    {
      /* Start reading 1 byte before kth chunk of file.  */
      off_t آغاز = (k - 1) * chunk_سائز + MIN (k - 1, rem_بائٹیں) - 1;
      اگر (آغاز < initial_read)
        {
          memmove (buf, buf + آغاز, initial_read - آغاز);
          initial_read -= آغاز;
        }
      ورنہ
        {
          اگر (initial_read < آغاز
              && lseek (STDIN_FILENO, آغاز - initial_read, SEEK_CUR) < 0)
            error (EXIT_FAILURE, errno, "%s", quotef (infile));
          initial_read = -1;
        }
      n_written = آغاز;
      chunk_no = k - 1;
      chunk_انتہا = آغاز + 1;
    }

  جب (n_written < file_سائز)
    {
      حرف *bp = buf, *eob;
      ssize_t n_read;
      اگر (0 <= initial_read)
        {
          n_read = initial_read;
          initial_read = -1;
        }
      ورنہ
        {
          n_read = اقرأ (STDIN_FILENO, buf,
                         MIN (bufsize, file_سائز - n_written));
          اگر (n_read < 0)
            error (EXIT_FAILURE, errno, "%s", quotef (infile));
        }
      اگر (n_read == 0)
        توڑو; /* eof.  */
      chunk_truncated = false;
      eob = buf + n_read;

      جب (bp != eob)
        {
          idx_t مقصد_write;
          bool اگلا = false;

          /* Begin looking for '\n' at last byte of chunk.  */
          off_t skip = MIN (n_read, MAX (0, chunk_انتہا - 1 - n_written));
          حرف *bp_برآمد = memchr (bp + skip, eolchar, n_read - skip);
          اگر (bp_برآمد)
            {
              bp_برآمد++;
              اگلا = true;
            }
          ورنہ
            bp_برآمد = eob;
          مقصد_write = bp_برآمد - bp;

          اگر (k == chunk_no)
            {
              /* We don't use the stdout buffer here since we're writing
                 large chunks from an existing file, so it's more efficient
                 to write out directly.  */
              اگر (بھرا_write (STDOUT_FILENO, bp, مقصد_write) != مقصد_write)
                write_error ();
            }
          ورنہ اگر (! k)
            cwrite (new_file_flag, bp, مقصد_write);
          n_written += مقصد_write;
          bp += مقصد_write;
          n_read -= مقصد_write;
          new_file_flag = اگلا;

          /* A line could have been so long that it skipped
             entire chunks. So create empty files in that case.  */
          جب (اگلا || chunk_انتہا <= n_written)
            {
              اگر (!اگلا && bp == eob)
                {
                  /* replenish buf, before going to next chunk.  */
                  chunk_truncated = true;
                  توڑو;
                }
              اگر (k == chunk_no)
                واپس;
              chunk_انتہا += chunk_سائز + (chunk_no < rem_بائٹیں);
              chunk_no++;
              اگر (chunk_انتہا <= n_written)
                {
                  اگر (! k)
                    cwrite (true, تہی, 0);
                }
              ورنہ
                اگلا = false;
            }
        }
    }

  اگر (chunk_truncated)
    chunk_no++;

  /* Ensure NUMBER files are created, which truncates
     any existing files or notifies any consumers on fifos.
     FIXME: Should we do this before EXIT_FAILURE?  */
  اگر (!k)
    جب (chunk_no++ <= n)
      cwrite (true, تہی, 0);
}

/* -n K/N: Extract Kth of N chunks.  */

ساکن خالی
بائٹیں_chunk_نکالو (intmax_t k, intmax_t n, حرف *buf, idx_t bufsize,
                     ssize_t initial_read, off_t file_سائز)
{
  off_t آغاز;
  off_t انتہا;

  affirm (0 < k && k <= n);

  آغاز = (k - 1) * (file_سائز / n) + MIN (k - 1, file_سائز % n);
  انتہا = k == n ? file_سائز : k * (file_سائز / n) + MIN (k, file_سائز % n);

  اگر (آغاز < initial_read)
    {
      memmove (buf, buf + آغاز, initial_read - آغاز);
      initial_read -= آغاز;
    }
  ورنہ
    {
      اگر (initial_read < آغاز
          && lseek (STDIN_FILENO, آغاز - initial_read, SEEK_CUR) < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
      initial_read = -1;
    }

  جب (آغاز < انتہا)
    {
      ssize_t n_read;
      اگر (0 <= initial_read)
        {
          n_read = initial_read;
          initial_read = -1;
        }
      ورنہ
        {
          n_read = اقرأ (STDIN_FILENO, buf, bufsize);
          اگر (n_read < 0)
            error (EXIT_FAILURE, errno, "%s", quotef (infile));
        }
      اگر (n_read == 0)
        توڑو; /* eof.  */
      n_read = MIN (n_read, انتہا - آغاز);
      اگر (بھرا_write (STDOUT_FILENO, buf, n_read) != n_read
          && ! ignorable (errno))
        error (EXIT_FAILURE, errno, "%s", quotef ("-"));
      آغاز += n_read;
    }
}

قسمتعریف ساخت کا_info
{
  حرف *کا_name;
  صحیح ofd;
  FILE *ofile;
  pid_t opid;
} کا_t;

شمار
{
  OFD_NEW = -1,
  OFD_APPEND = -2
};

/* Rotate file descriptors when we're writing to more output files than we
   have available file descriptors.
   Return whether we came under file resource pressure.
   If so, it's probably best to close each file when finished with it.  */

ساکن bool
ofile_open (کا_t *files, idx_t i_check, idx_t nfiles)
{
  bool file_حد_بندی = false;

  اگر (files[i_check].ofd <= OFD_NEW)
    {
      صحیح fd;
      idx_t i_reopen = i_check ? i_check - 1 : nfiles - 1;

      /* Another process could have opened a file in between the calls to
         close and open, so we should keep trying until open succeeds or
         we've closed all of our files.  */
      جب (true)
        {
          اگر (files[i_check].ofd == OFD_NEW)
            fd = create (files[i_check].کا_name);
          ورنہ /* OFD_APPEND  */
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
              fd = open (files[i_check].کا_name,
                         O_WRONLY | O_ثنائی | O_APPEND | O_NONBLOCK);
            }

          اگر (0 <= fd)
            توڑو;

          اگر (!(errno == EMFILE || errno == ENFILE))
            error (EXIT_FAILURE, errno, "%s", quotef (files[i_check].کا_name));

          file_حد_بندی = true;

          /* Search backwards for an open file to close.  */
          جب (files[i_reopen].ofd < 0)
            {
              i_reopen = i_reopen ? i_reopen - 1 : nfiles - 1;
              /* No more open files to close, exit with E[NM]FILE.  */
              اگر (i_reopen == i_check)
                error (EXIT_FAILURE, errno, "%s",
                       quotef (files[i_check].کا_name));
            }

          اگر (fclose (files[i_reopen].ofile) != 0)
            error (EXIT_FAILURE, errno, "%s", quotef (files[i_reopen].کا_name));
          files[i_reopen].ofile = تہی;
          files[i_reopen].ofd = OFD_APPEND;
        }

      files[i_check].ofd = fd;
      FILE *ofile = fdopen (fd, "a");
      اگر (!ofile)
        error (EXIT_FAILURE, errno, "%s", quotef (files[i_check].کا_name));
      files[i_check].ofile = ofile;
      files[i_check].opid = filter_pid;
      filter_pid = 0;
    }

  واپس file_حد_بندی;
}

/* -n r/[K/]N: Divide file into N chunks in round robin fashion.
   Use BUF of size BUFSIZE for the buffer, and if allocating storage
   put its address into *FILESP to pacify -fsanitize=leak.
   When K == 0, we try to keep the files open in parallel.
   If we run out of file resources, then we revert
   to opening and closing each file for each line.  */

ساکن خالی
سطریں_rr (intmax_t k, intmax_t n, حرف *buf, idx_t bufsize, کا_t **filesp)
{
  bool wrapped = false;
  bool wrote = false;
  bool file_حد_بندی;
  idx_t i_file;
  کا_t *files IF_LINT (= تہی);
  intmax_t سطر_no;

  اگر (k)
    سطر_no = 1;
  ورنہ
    {
      اگر (IDX_MAX < n)
        xalloc_die ();
      files = *filesp = xinmalloc (n, حجم *files);

      /* Generate output file names. */
      برائے (i_file = 0; i_file < n; i_file++)
        {
          اگلا_file_name ();
          files[i_file].کا_name = xstrdup (outfile);
          files[i_file].ofd = OFD_NEW;
          files[i_file].ofile = تہی;
          files[i_file].opid = 0;
        }
      i_file = 0;
      file_حد_بندی = false;
    }

  جب (true)
    {
      حرف *bp = buf, *eob;
      ssize_t n_read = اقرأ (STDIN_FILENO, buf, bufsize);
      اگر (n_read < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
      ورنہ اگر (n_read == 0)
        توڑو; /* eof.  */
      eob = buf + n_read;

      جب (bp != eob)
        {
          idx_t مقصد_write;
          bool اگلا = false;

          /* Find end of line. */
          حرف *bp_برآمد = memchr (bp, eolchar, eob - bp);
          اگر (bp_برآمد)
            {
              bp_برآمد++;
              اگلا = true;
            }
          ورنہ
            bp_برآمد = eob;
          مقصد_write = bp_برآمد - bp;

          اگر (k)
            {
              اگر (سطر_no == k && unbuffered)
                {
                  اگر (بھرا_write (STDOUT_FILENO, bp, مقصد_write) != مقصد_write)
                    write_error ();
                }
              ورنہ اگر (سطر_no == k && fwrite (bp, مقصد_write, 1, stdout) != 1)
                {
                  write_error ();
                }
              اگر (اگلا)
                سطر_no = (سطر_no == n) ? 1 : سطر_no + 1;
            }
          ورنہ
            {
              /* Secure file descriptor. */
              file_حد_بندی |= ofile_open (files, i_file, n);
              اگر (unbuffered)
                {
                  /* Note writing to fd, rather than flushing the FILE gives
                     an 8% performance benefit, due to reduced data copying.  */
                  اگر (بھرا_write (files[i_file].ofd, bp, مقصد_write) != مقصد_write
                      && ! ignorable (errno))
                    error (EXIT_FAILURE, errno, "%s",
                           quotef (files[i_file].کا_name));
                }
              ورنہ اگر (fwrite (bp, مقصد_write, 1, files[i_file].ofile) != 1
                       && ! ignorable (errno))
                error (EXIT_FAILURE, errno, "%s",
                       quotef (files[i_file].کا_name));

              اگر (! ignorable (errno))
                wrote = true;

              اگر (file_حد_بندی)
                {
                  اگر (fclose (files[i_file].ofile) != 0)
                    error (EXIT_FAILURE, errno, "%s",
                           quotef (files[i_file].کا_name));
                  files[i_file].ofile = تہی;
                  files[i_file].ofd = OFD_APPEND;
                }
              اگر (اگلا && ++i_file == n)
                {
                  wrapped = true;
                  /* If no filters are accepting input, stop reading.  */
                  اگر (! wrote)
                    جاؤ no_filters;
                  wrote = false;
                  i_file = 0;
                }
            }

          bp = bp_برآمد;
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
      برائے (i_file = 0; i_file < n; i_file++)
        {
          اگر (i_file >= ceiling && !elide_خالی_خانے_files)
            file_حد_بندی |= ofile_open (files, i_file, n);
          اگر (files[i_file].ofd >= 0)
            closeout (files[i_file].ofile, files[i_file].ofd,
                      files[i_file].opid, files[i_file].کا_name);
          files[i_file].ofd = OFD_APPEND;
        }
    }
}

#تعریف_کرو FAIL_ONLY_ONE_WAY()					\
  کرو								\
    {								\
      error (0, 0, _("cannot split in more than one way"));	\
      usage (EXIT_FAILURE);					\
    }								\
  جب (0)

/* Report a string-to-integer conversion failure MSGID with ARG.  */

ساکن _Noreturn خالی
strtoint_die (حرف ثابت *msgid, حرف ثابت *arg)
{
  error (EXIT_FAILURE, errno == EINVAL ? 0 : errno, "%s: %s",
         gettext (msgid), quote (arg));
}

/* Use OVERFLOW_OK when it is OK to ignore LONGINT_OVERFLOW errors, since the
   extreme value will do the right thing anyway on any practical platform.  */
#تعریف_کرو حد_سے_تجاوز_OK LONGINT_حد_سے_تجاوز

/* Parse ARG for number of bytes or lines.  The number can be followed
   by MULTIPLIERS, and the resulting value must be positive.
   If the number cannot be parsed, diagnose with MSG.
   Return the number parsed, or an INTMAX_MAX on overflow.  */

ساکن intmax_t
parse_n_units (حرف ثابت *arg, حرف ثابت *multipliers, حرف ثابت *msgid)
{
  intmax_t n;
  اگر (حد_سے_تجاوز_OK < xstrtoimax (arg, تہی, 10, &n, multipliers) || n < 1)
    strtoint_die (msgid, arg);
  واپس n;
}

/* Parse K/N syntax of chunk options.  */

ساکن خالی
parse_chunk (intmax_t *k_units, intmax_t *n_units, حرف ثابت *arg)
{
  حرف *argend;
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
  ورنہ اگر (! (e <= حد_سے_تجاوز_OK && 0 < *n_units))
    strtoint_die (N_("invalid number of chunks"), arg);
}


صحیح
مین (صحیح argc, حرف **argv)
{
  شمار بانٹو_type بانٹو_type_2 = type_undef;
  idx_t اندر_blk_سائز = 0;	/* optimal block size of input file device */
  idx_t صفحہ_سائز = getpagesize ();
  intmax_t k_units = 0;
  intmax_t n_units = 0;

  ساکن حرف ثابت multipliers[] = "bEGKkMmPQRTYZ0";
  صحیح c;
  صحیح digits_optind = 0;
  off_t file_سائز = OFF_T_MAX;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* Parse command line options.  */

  جب (true)
    {
      /* This is the argv-index of the option we will read next.  */
      صحیح this_optind = optind ? optind : 1;

      c = getopt_long (argc, argv, "0123456789C:a:b:del:n:t:ux",
                       longopts, تہی);
      اگر (c == -1)
        توڑو;

      انتخاب (c)
        {
        حالت 'a':
          suffix_لمبائی = xdectoimax (optarg, 0, IDX_MAX,
                                      "", _("invalid suffix length"), 0);
          توڑو;

        حالت ADDITIONAL_SUFFIX_OPTION:
          {
            صحیح suffix_len = لڑی_لمبائی (optarg);
            اگر (آخری_component (optarg) != optarg
                || (suffix_len && ISSLASH (optarg[suffix_len - 1])))
              {
                error (0, 0,
                       _("invalid suffix %s, contains directory separator"),
                       quote (optarg));
                usage (EXIT_FAILURE);
              }
          }
          additional_suffix = optarg;
          توڑو;

        حالت 'b':
          اگر (بانٹو_type_2 != type_undef)
            FAIL_ONLY_ONE_WAY ();
          بانٹو_type_2 = type_بائٹیں;
          n_units = parse_n_units (optarg, multipliers,
                                   N_("invalid number of bytes"));
          توڑو;

        حالت 'l':
          اگر (بانٹو_type_2 != type_undef)
            FAIL_ONLY_ONE_WAY ();
          بانٹو_type_2 = type_سطریں;
          n_units = parse_n_units (optarg, "", N_("invalid number of lines"));
          توڑو;

        حالت 'C':
          اگر (بانٹو_type_2 != type_undef)
            FAIL_ONLY_ONE_WAY ();
          بانٹو_type_2 = type_byteslines;
          n_units = parse_n_units (optarg, multipliers,
                                   N_("invalid number of lines"));
          توڑو;

        حالت 'n':
          اگر (بانٹو_type_2 != type_undef)
            FAIL_ONLY_ONE_WAY ();
          /* skip any whitespace */
          جب (خلا_ہے (مقصد_uchar (*optarg)))
            optarg++;
          اگر (STRNCMP_LIT (optarg, "r/") == 0)
            {
              بانٹو_type_2 = type_rr;
              optarg += 2;
            }
          ورنہ اگر (STRNCMP_LIT (optarg, "l/") == 0)
            {
              بانٹو_type_2 = type_chunk_سطریں;
              optarg += 2;
            }
          ورنہ
            بانٹو_type_2 = type_chunk_بائٹیں;
          parse_chunk (&k_units, &n_units, optarg);
          توڑو;

        حالت 'u':
          unbuffered = true;
          توڑو;

        حالت 't':
          {
            حرف neweol = optarg[0];
            اگر (! neweol)
              error (EXIT_FAILURE, 0, _("empty record separator"));
            اگر (optarg[1])
              {
                اگر (streq (optarg, "\\0"))
                  neweol = '\0';
                ورنہ
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
          توڑو;

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
          اگر (بانٹو_type_2 == type_undef)
            {
              بانٹو_type_2 = type_digits;
              n_units = 0;
            }
          اگر (بانٹو_type_2 != type_undef && بانٹو_type_2 != type_digits)
            FAIL_ONLY_ONE_WAY ();
          اگر (digits_optind != 0 && digits_optind != this_optind)
            n_units = 0;	/* More than one number given; ignore other. */
          digits_optind = this_optind;
          اگر (ckd_mul (&n_units, n_units, 10)
              || ckd_جوڑو (&n_units, n_units, c - '0'))
            n_units = INTMAX_MAX;
          توڑو;

        حالت 'd':
        حالت 'x':
          اگر (c == 'd')
            suffix_alphabet = "0123456789";
          ورنہ
            suffix_alphabet = "0123456789abcdef";
          اگر (optarg)
            {
              اگر (لڑی_لمبائی (optarg) != strspn (optarg, suffix_alphabet))
                {
                  error (0, 0,
                         (c == 'd') ?
                           _("%s: invalid start value for numerical suffix") :
                           _("%s: invalid start value for hexadecimal suffix"),
                         quote (optarg));
                  usage (EXIT_FAILURE);
                }
              ورنہ
                {
                  /* Skip any leading zero.  */
                  جب (*optarg == '0' && *(optarg + 1) != '\0')
                    optarg++;
                  numeric_suffix_آغاز = optarg;
                }
            }
          توڑو;

        حالت 'e':
          elide_خالی_خانے_files = true;
          توڑو;

        حالت FILTER_OPTION:
          filter_command = optarg;
          توڑو;

        حالت IO_BLKSIZE_OPTION:
          اندر_blk_سائز = xnumtoumax (optarg, 10, 1,
                                    MIN (SYS_BUFSIZE_MAX,
                                         MIN (IDX_MAX, SIZE_MAX) - 1),
                                    multipliers, _("invalid IO block size"),
                                    0, XTOINT_MIN_RANGE);
          توڑو;

        حالت VERBOSE_OPTION:
          verbose = true;
          توڑو;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

        طےشدہ:
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
  اگر (بانٹو_type_2 == type_undef)
    {
      بانٹو_type_2 = type_سطریں;
      n_units = 1000;
    }

  اگر (n_units == 0)
    {
      error (0, 0, _("invalid number of lines: %s"), quote ("0"));
      usage (EXIT_FAILURE);
    }

  اگر (eolchar < 0)
    eolchar = '\n';

  سیٹ_suffix_لمبائی (n_units, بانٹو_type_2);

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
  اگر (numeric_suffix_آغاز && لڑی_لمبائی (numeric_suffix_آغاز) > suffix_لمبائی)
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
  xset_ثنائی_منوال (STDIN_FILENO, O_ثنائی);

  /* Advise the kernel of our access pattern.  */
  fdadvise (STDIN_FILENO, 0, 0, FADVISE_SEQUENTIAL);

  /* Get the optimal block size of input device and make a buffer.  */

  اگر (fstat (STDIN_FILENO, &اندر_stat_buf) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (infile));

  اگر (اندر_blk_سائز == 0)
    {
      اندر_blk_سائز = io_blksize (&اندر_stat_buf);
      اگر (SYS_BUFSIZE_MAX < اندر_blk_سائز)
        اندر_blk_سائز = SYS_BUFSIZE_MAX;
    }

  حرف *buf = xalignalloc (صفحہ_سائز, اندر_blk_سائز + 1);
  ssize_t initial_read = -1;

  اگر (بانٹو_type_2 == type_chunk_بائٹیں || بانٹو_type_2 == type_chunk_سطریں)
    {
      file_سائز = داخلہ_file_سائز (STDIN_FILENO, &اندر_stat_buf,
                                   buf, اندر_blk_سائز);
      اگر (file_سائز < 0)
        error (EXIT_FAILURE, errno, _("%s: cannot determine file size"),
               quotef (infile));
      initial_read = MIN (file_سائز, اندر_blk_سائز);
    }

  /* When filtering, closure of one pipe must not terminate the process,
     as there may still be other streams expecting input from us.  */
  اگر (filter_command)
    default_SIGPIPE = اشارہ (SIGPIPE, SIG_IGN) == SIG_DFL;

  انتخاب (بانٹو_type_2)
    {
    حالت type_digits:
    حالت type_سطریں:
      سطریں_بانٹو (n_units, buf, اندر_blk_سائز);
      توڑو;

    حالت type_بائٹیں:
      بائٹیں_بانٹو (n_units, 0, buf, اندر_blk_سائز, -1, 0);
      توڑو;

    حالت type_byteslines:
      سطر_بائٹیں_بانٹو (n_units, buf, اندر_blk_سائز);
      توڑو;

    حالت type_chunk_بائٹیں:
      اگر (k_units == 0)
        بائٹیں_بانٹو (file_سائز / n_units, file_سائز % n_units,
                     buf, اندر_blk_سائز, initial_read, n_units);
      ورنہ
        بائٹیں_chunk_نکالو (k_units, n_units, buf, اندر_blk_سائز, initial_read,
                             file_سائز);
      توڑو;

    حالت type_chunk_سطریں:
      سطریں_chunk_بانٹو (k_units, n_units, buf, اندر_blk_سائز, initial_read,
                         file_سائز);
      توڑو;

    حالت type_rr:
      /* Note, this is like 'sed -n ${k}~${n}p' when k > 0,
         but the functionality is provided for symmetry.  */
      {
        کا_t *files;
        سطریں_rr (k_units, n_units, buf, اندر_blk_سائز, &files);
      }
      توڑو;

    حالت type_undef: طےشدہ:
      affirm (false);
    }

  اگر (close (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (infile));
  closeout (تہی, output_desc, filter_pid, outfile);

  main_exit (EXIT_SUCCESS);
}
