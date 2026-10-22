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
#অন্তর্ভাব <config.h>

#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <getopt.h>
#অন্তর্ভাব <signal.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <sys/wait.h>
#অন্তর্ভাব <spawn.h>

#অন্তর্ভাব "system.h"
#অন্তর্ভাব "alignalloc.h"
#অন্তর্ভাব "assure.h"
#অন্তর্ভাব "fadvise.h"
#অন্তর্ভাব "fd-reopen.h"
#অন্তর্ভাব "fcntl--.h"
#অন্তর্ভাব "full-write.h"
#অন্তর্ভাব "ioblksize.h"
#অন্তর্ভাব "quote.h"
#অন্তর্ভাব "sig2str.h"
#অন্তর্ভাব "sys-limits.h"
#অন্তর্ভাব "temp-stream.h"
#অন্তর্ভাব "unistd--.h"
#অন্তর্ভাব "xbinary-io.h"
#অন্তর্ভাব "xdectoint.h"
#অন্তর্ভাব "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "split"

#define AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("Richard M. Stallman")

/* Shell command to filter through, instead of creating files.  */
স্থির অক্ষর ধ্রুবক *filter_command;

/* Process ID of the filter.  */
স্থির pid_t filter_pid;

স্থির কাঠামো
{
  /* The number of open pipes.  */
  পূর্ণসংখ্যা n_pipes;

  /* The number of allocated pipes.  */
  idx_t n_pipes_alloc;

  /* Array of open pipes.  */
  পূর্ণসংখ্যা *pipes COUNTED_BY (n_pipes);
} open_pipes;

/* Whether SIGPIPE has the default action, when --filter is used.  */
স্থির bool default_SIGPIPE;

/* Base name of output files.  */
স্থির অক্ষর ধ্রুবক *outbase = "x";

/* Name of output files.  */
স্থির অক্ষর *outfile;

/* Pointer to the end of the prefix in OUTFILE.
   Suffixes are inserted here.  */
স্থির অক্ষর *outfile_mid;

/* Generate new suffix when suffixes are exhausted.  */
স্থির bool suffix_auto = true;

/* Length of OUTFILE's suffix.  */
স্থির idx_t suffix_দৈর্ঘ্য;

/* Alphabet of characters to use in suffix.  */
স্থির অক্ষর ধ্রুবক *suffix_alphabet = "abcdefghijklmnopqrstuvwxyz";

/* Numerical suffix start value.  */
স্থির অক্ষর ধ্রুবক *numeric_suffix_start;

/* Additional suffix to append to output file names.  */
স্থির অক্ষর ধ্রুবক *additional_suffix;

/* Name of input file.  May be "-".  */
স্থির অক্ষর ধ্রুবক *infile = "-";

/* stat buf for input file.  */
স্থির কাঠামো stat in_stat_buf;

/* Descriptor on which output file is open.  */
স্থির পূর্ণসংখ্যা output_desc = -1;

/* If true, print a diagnostic on standard error just before each
   output file is opened. */
স্থির bool verbose;

/* If true, don't generate zero length output files. */
স্থির bool elide_empty_files;

/* If true, in round robin mode, immediately copy
   input to output, which is much slower, so disabled by default.  */
স্থির bool unbuffered;

/* The character marking end of line.  Defaults to \n below.  */
স্থির পূর্ণসংখ্যা eolchar = -1;

/* The split mode to use.  */
গণনা Split_type
{
  type_undef, type_bytes, type_byteslines, type_সারিগুলি, type_digits,
  type_chunk_bytes, type_chunk_সারিগুলি, type_rr
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
গণনা
{
  VERBOSE_OPTION = CHAR_MAX + 1,
  FILTER_OPTION,
  IO_BLKSIZE_OPTION,
  ADDITIONAL_SUFFIX_OPTION
};

স্থির কাঠামো option ধ্রুবক longopts[] =
{
  {"bytes", required_argument, ফাঁকা, 'b'},
  {"lines", required_argument, ফাঁকা, 'l'},
  {"line-bytes", required_argument, ফাঁকা, 'C'},
  {"number", required_argument, ফাঁকা, 'n'},
  {"elide-empty-files", no_argument, ফাঁকা, 'e'},
  {"unbuffered", no_argument, ফাঁকা, 'u'},
  {"suffix-length", required_argument, ফাঁকা, 'a'},
  {"additional-suffix", required_argument, ফাঁকা,
   ADDITIONAL_SUFFIX_OPTION},
  {"numeric-suffixes", optional_argument, ফাঁকা, 'd'},
  {"hex-suffixes", optional_argument, ফাঁকা, 'x'},
  {"filter", required_argument, ফাঁকা, FILTER_OPTION},
  {"verbose", no_argument, ফাঁকা, VERBOSE_OPTION},
  {"separator", required_argument, ফাঁকা, 't'},
  {"-io-blksize", required_argument, ফাঁকা,
   IO_BLKSIZE_OPTION}, /* do not document */
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ফাঁকা, 0, ফাঁকা, 0}
};

/* Return true if the errno value, ERR, is ignorable.  */
স্থির অন্তঃরেখ bool
ignorable (পূর্ণসংখ্যা err)
{
  ফেরত filter_command && err == EPIPE;
}

স্থির শূন্য
set_suffix_দৈর্ঘ্য (intmax_t n_units, গণনা Split_type split_type)
{
#define DEFAULT_SUFFIX_দৈর্ঘ্য 2

  পূর্ণসংখ্যা suffix_দৈর্ঘ্য_needed = 0;

  /* The suffix auto length feature is incompatible with
     a user specified start value as the generated suffixes
     are not all consecutive.  */
  যদি (numeric_suffix_start)
    suffix_auto = false;

  /* Auto-calculate the suffix length if the number of files is given.  */
  যদি (split_type == type_chunk_bytes || split_type == type_chunk_সারিগুলি
      || split_type == type_rr)
    {
      intmax_t n_units_end = n_units - 1;
      যদি (numeric_suffix_start)
        {
          intmax_t n_start;
          strtol_error e = xstrtoimax (numeric_suffix_start, ফাঁকা, 10,
                                       &n_start, "");
          যদি (e == LONGINT_OK && n_start < n_units)
            {
              /* Restrict auto adjustment so we don't keep
                 incrementing a suffix size arbitrarily,
                 as that would break sort order for files
                 generated from multiple split runs.  */
              যদি (ckd_add (&n_units_end, n_units_end, n_start))
                n_units_end = INTMAX_MAX;
            }

        }
      idx_t alphabet_len = strlen (suffix_alphabet);
      করো
        suffix_দৈর্ঘ্য_needed++;
      যখন (n_units_end /= alphabet_len);

      suffix_auto = false;
    }

  যদি (suffix_দৈর্ঘ্য)            /* set by user */
    {
      যদি (suffix_দৈর্ঘ্য < suffix_দৈর্ঘ্য_needed)
        error (EXIT_FAILURE, 0,
               _("the suffix length needs to be at least %d"),
               suffix_দৈর্ঘ্য_needed);
      suffix_auto = false;
      ফেরত;
    }
  অন্যথা
    suffix_দৈর্ঘ্য = MAX (DEFAULT_SUFFIX_দৈর্ঘ্য, suffix_দৈর্ঘ্য_needed);
}

শূন্য
usage (পূর্ণসংখ্যা status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
    {
      printf (_("\
Usage: %s [OPTION]... [FILE [PREFIX]]\n\
"),
              program_name);
      fputs (_("\
Output pieces of FILE to PREFIXaa, PREFIXab, ...;\n\
পূর্বনির্ধারিত আকার_2 is 1000 সারিগুলি, and পূর্বনির্ধারিত PREFIX is 'x'.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oprintf (_("\
  -a, --suffix-দৈর্ঘ্য=N\n\
         generate suffixes of দৈর্ঘ্য N (পূর্বনির্ধারিত %d)\n\
"), DEFAULT_SUFFIX_দৈর্ঘ্য);
      oputs (_("\
      --additional-suffix=SUFFIX\n\
         append an additional SUFFIX to file names\n\
"));
      oputs (_("\
  -b, --bytes=আকার_3\n\
         put আকার_3 bytes per output file\n\
"));
      oputs (_("\
  -C, --সারি-bytes=আকার_3\n\
         put at most আকার_3 bytes of records per output file\n\
"));
      oputs (_("\
  -d\n\
         use numeric suffixes starting at 0, not alphabetic\n\
"));
      oputs (_("\
      --numeric-suffixes[=FROM]\n\
         same as -d, but allow setting the start মান\n\
"));
      oputs (_("\
  -x\n\
         use hex suffixes starting at 0, not alphabetic\n\
"));
      oputs (_("\
      --hex-suffixes[=FROM]\n\
         same as -x, but allow setting the start মান\n\
"));
      oputs (_("\
  -e, --elide-empty-files\n\
         করো not generate empty output files with '-n'\n\
"));
      oputs (_("\
      --filter=COMMAND\n\
         লিখ to shell COMMAND; file name is $FILE\n\
"));
      oputs (_("\
  -l, --সারিগুলি=সংখ্যা_2\n\
         put সংখ্যা_2 সারিগুলি/records per output file\n\
"));
      oputs (_("\
  -n, --সংখ্যা=CHUNKS\n\
         generate CHUNKS output files; see explanation below\n\
"));
      oputs (_("\
  -t, --separator=SEP\n\
         use SEP instead of newline as the record separator;\n\
         '\\0' (zero) specifies the NUL character\n\
"));
      oputs (_("\
  -u, --unbuffered\n\
         immediately copy প্রবেশ to output with '-n r/...'\n\
"));
      oputs (_("\
      --verbose\n\
         print a diagnostic just before each output file is opened\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_আকার_note ();
      fputs (_("\n\
CHUNKS may be:\n\
  N       split into N files based on আকার_2 of প্রবেশ\n\
  K/N     output Kth of N to standard output\n\
  l/N     split into N files without splitting সারিগুলি/records\n\
  l/K/N   output Kth of N to standard output without splitting সারিগুলি/records\n\
  r/N     like 'l' but use round robin distribution\n\
  r/K/N   likewise but only output Kth of N to standard output\n\
"), stdout);
      fputs (_("\n\
-n (except -nr) will বাফার to $TMPDIR, defaulting to /tmp,\n\
যদি the প্রবেশ আকার_2 cannot easily be determined.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Copy the data in FD to a temporary file, then make that file FD.
   Use BUF, of size BUFSIZE, to copy.  Return the number of
   bytes copied, or -1 (setting errno) on error.  */
স্থির off_t
copy_to_tmpfile (পূর্ণসংখ্যা fd, অক্ষর *buf, idx_t bufsize)
{
  FILE *tmp;
  যদি (!অস্থায়ী_stream (&tmp, ফাঁকা))
    ফেরত -1;
  off_t copied = 0;
  off_t r;

  যখন (0 < (r = পঠ (fd, buf, bufsize)))
    {
      যদি (fwrite (buf, 1, r, tmp) != r)
        ফেরত -1;
      যদি (ckd_add (&copied, copied, r))
        {
          errno = EOVERFLOW;
          ফেরত -1;
        }
    }

  যদি (r < 0)
    ফেরত r;
  r = dup2 (fileno (tmp), fd);
  যদি (r < 0)
    ফেরত r;
  যদি (fclose (tmp) < 0)
    ফেরত -1;
  ফেরত copied;
}

/* Return the number of bytes that can be read from FD with status ST.
   Store up to the first BUFSIZE bytes of the file's data into BUF,
   and advance the file position by the number of bytes read.  On
   input error, set errno and return -1.  */

স্থির off_t
প্রবেশ_file_আকার (পূর্ণসংখ্যা fd, কাঠামো stat ধ্রুবক *st, অক্ষর *buf, idx_t bufsize)
{
  off_t আকার_2 = 0;
  করো
    {
      ssize_t n_read = পঠ (fd, buf + আকার_2, bufsize - আকার_2);
      যদি (n_read <= 0)
        ফেরত n_read < 0 ? n_read : আকার_2;
      আকার_2 += n_read;
    }
  যখন (আকার_2 < bufsize);

  off_t cur, end;
  যদি ((usable_st_আকার (st) && st->st_আকার < আকার_2)
      || (cur = lseek (fd, 0, SEEK_CUR)) < 0
      || cur < আকার_2 /* E.g., /dev/zero on GNU/Linux.  */
      || (end = lseek (fd, 0, SEEK_END)) < 0)
    {
      অক্ষর *tmpbuf = xmalloc (bufsize);
      end = copy_to_tmpfile (fd, tmpbuf, bufsize);
      মুক্ত (tmpbuf);
      যদি (end < 0)
        ফেরত end;
      cur = 0;
    }

  যদি (end == OFF_T_MAX /* E.g., /dev/zero on GNU/Hurd.  */
      || (cur < end && ckd_add (&আকার_2, আকার_2, end - cur)))
    {
      errno = EOVERFLOW;
      ফেরত -1;
    }

  যদি (cur < end)
    {
      off_t r = lseek (fd, cur, SEEK_SET);
      যদি (r < 0)
        ফেরত r;
    }

  ফেরত আকার_2;
}

/* Compute the next sequential output file name and store it into the
   string 'outfile'.  */

স্থির শূন্য
পরবর্তী_file_name (শূন্য)
{
  /* Index in suffix_alphabet of each character in the suffix.  */
  স্থির idx_t *sufindex;
  স্থির idx_t outbase_দৈর্ঘ্য;
  স্থির idx_t outfile_দৈর্ঘ্য;
  স্থির idx_t addsuf_দৈর্ঘ্য;

  যদি (! outfile)
    {
      bool overflow, widen;

new_name:
      widen = !! outfile_দৈর্ঘ্য;

      যদি (! widen)
        {
          /* Allocate and initialize the first file name.  */

          outbase_দৈর্ঘ্য = strlen (outbase);
          addsuf_দৈর্ঘ্য = additional_suffix ? strlen (additional_suffix) : 0;
          overflow = ckd_add (&outfile_দৈর্ঘ্য, outbase_দৈর্ঘ্য + addsuf_দৈর্ঘ্য,
                              suffix_দৈর্ঘ্য);
        }
      অন্যথা
        {
          /* Reallocate and initialize a new wider file name.
             We do this by subsuming the unchanging part of
             the generated suffix into the prefix (base), and
             reinitializing the now one longer suffix.  */

          overflow = ckd_add (&outfile_দৈর্ঘ্য, outfile_দৈর্ঘ্য, 2);
          suffix_দৈর্ঘ্য++;
        }

      idx_t outfile_আকার;
      overflow |= ckd_add (&outfile_আকার, outfile_দৈর্ঘ্য, 1);
      যদি (overflow)
        xalloc_die ();
      outfile = xirealloc (outfile, outfile_আকার);

      যদি (! widen)
        memcpy (outfile, outbase, outbase_দৈর্ঘ্য);
      অন্যথা
        {
          /* Append the last alphabet character to the file name prefix.  */
          outfile[outbase_দৈর্ঘ্য] = suffix_alphabet[sufindex[0]];
          outbase_দৈর্ঘ্য++;
        }

      outfile_mid = outfile + outbase_দৈর্ঘ্য;
      memset (outfile_mid, suffix_alphabet[0], suffix_দৈর্ঘ্য);
      যদি (additional_suffix)
        memcpy (outfile_mid + suffix_দৈর্ঘ্য, additional_suffix, addsuf_দৈর্ঘ্য);
      outfile[outfile_দৈর্ঘ্য] = 0;

      মুক্ত (sufindex);
      sufindex = xicalloc (suffix_দৈর্ঘ্য, আকার *sufindex);

      যদি (numeric_suffix_start)
        {
          affirm (! widen);

          /* Update the output file name.  */
          idx_t i = strlen (numeric_suffix_start);
          memcpy (outfile_mid + suffix_দৈর্ঘ্য - i, numeric_suffix_start, i);

          /* Update the suffix index.  */
          idx_t *sufindex_end = sufindex + suffix_দৈর্ঘ্য;
          যখন (i-- != 0)
            *--sufindex_end = numeric_suffix_start[i] - '0';
        }

#যদি ! _POSIX_NO_TRUNC && HAVE_PATHCONF && defined _PC_NAME_MAX
      /* POSIX requires that if the output file name is too long for
         its directory, 'split' must fail without creating any files.
         This must be checked for explicitly on operating systems that
         silently truncate file names.  */
      {
        অক্ষর *dir = dir_name (outfile);
        long name_max = pathconf (dir, _PC_NAME_MAX);
        যদি (0 <= name_max && name_max < base_len (last_component (outfile)))
          error (EXIT_FAILURE, ENAMETOOLONG, "%s", quotef (outfile));
        মুক্ত (dir);
      }
#endif
    }
  অন্যথা
    {
      /* Increment the suffix in place, if possible.  */

      idx_t i = suffix_দৈর্ঘ্য;
      যখন (i-- != 0)
        {
          sufindex[i]++;
          যদি (suffix_auto && i == 0 && ! suffix_alphabet[sufindex[0] + 1])
            যাও new_name;
          outfile_mid[i] = suffix_alphabet[sufindex[i]];
          যদি (outfile_mid[i])
            ফেরত;
          sufindex[i] = 0;
          outfile_mid[i] = suffix_alphabet[sufindex[i]];
        }
      error (EXIT_FAILURE, 0, _("output file suffixes exhausted"));
    }
}

/* Create or truncate a file.  */

স্থির পূর্ণসংখ্যা
create (অক্ষর ধ্রুবক *name)
{
  যদি (!filter_command)
    {
      যদি (verbose)
        fprintf (stdout, _("creating file %s\n"), quoteaf (name));

      পূর্ণসংখ্যা oflags = O_WRONLY | O_CREAT | O_দ্বিমিক;
      পূর্ণসংখ্যা fd = open (name, oflags | O_EXCL, MODE_RW_UGO);
      যদি (0 <= fd || errno != EEXIST)
        ফেরত fd;
      fd = open (name, oflags, MODE_RW_UGO);
      যদি (fd < 0)
        ফেরত fd;
      কাঠামো stat out_stat_buf;
      যদি (fstat (fd, &out_stat_buf) != 0)
        error (EXIT_FAILURE, errno, _("failed to stat %s"), quoteaf (name));
      যদি (psame_inode (&in_stat_buf, &out_stat_buf))
        error (EXIT_FAILURE, 0, _("%s would overwrite input; aborting"),
               quoteaf (name));
      যদি (ftruncate (fd, 0) < 0
          && (S_ISREG (out_stat_buf.st_mode) || S_TYPEISSHM (&out_stat_buf)))
        error (EXIT_FAILURE, errno, _("%s: error truncating"), quotef (name));

      ফেরত fd;
    }
  অন্যথা
    {
      যদি (setenv ("FILE", name, 1) != 0)
        error (EXIT_FAILURE, errno,
               _("failed to set FILE environment variable"));
      যদি (verbose)
        fprintf (stdout, _("executing with FILE=%s\n"), quotef (name));

      পূর্ণসংখ্যা ফলাফল;
      পূর্ণসংখ্যা fd_pair[2];
      pid_t child_pid;

      posix_spawnattr_t attr;
      posix_spawn_file_actions_t actions;

      sigset_t set;
      sigemptyset (&set);
      যদি (default_SIGPIPE)
        sigaddset (&set, SIGPIPE);

      যদি (   (ফলাফল = posix_spawnattr_init (&attr))
          || (ফলাফল = posix_spawnattr_setflags (&attr,
                                                 (POSIX_SPAWN_USEVFORK
                                                  | POSIX_SPAWN_SETSIGDEF)))
          || (ফলাফল = posix_spawnattr_setsigdefault (&attr, &set))
          || (ফলাফল = posix_spawn_file_actions_init (&actions))
         )
        error (EXIT_FAILURE, ফলাফল, _("posix_spawn initialization failed"));

      যদি (pipe (fd_pair) != 0)
        error (EXIT_FAILURE, errno, _("failed to create pipe"));

      /* We have to close any pipes that were opened during an
         earlier call, otherwise this process will be holding a
         write-pipe that will prevent the earlier process from
         reading an EOF on the corresponding read-pipe.  */
      জন্য (পূর্ণসংখ্যা i = 0; i < open_pipes.n_pipes; ++i)
        যদি ((ফলাফল = posix_spawn_file_actions_addclose (&actions,
                                                         open_pipes.pipes[i])))
          বিরতি;

      যদি (   ফলাফল
          || (ফলাফল = posix_spawn_file_actions_addclose (&actions, fd_pair[1]))
          || (fd_pair[0] != STDIN_FILENO
              && (   (ফলাফল = posix_spawn_file_actions_adddup2 (&actions,
                                                                 fd_pair[0],
                                                                 STDIN_FILENO))
                  || (ফলাফল = posix_spawn_file_actions_addclose (&actions,
                                                                  fd_pair[0]))))
         )
        error (EXIT_FAILURE, ফলাফল, _("posix_spawn setup failed"));


      অক্ষর ধ্রুবক *shell_prog = getenv ("SHELL");
      যদি (shell_prog == ফাঁকা)
        shell_prog = "/bin/sh";
      অক্ষর ধ্রুবক *ধ্রুবক argv[] = { last_component (shell_prog), "-c",
                                   filter_command, ফাঁকা };

      ফলাফল = posix_spawn (&child_pid, shell_prog, &actions, &attr,
                            (অক্ষর * ধ্রুবক *) argv, environ);
      যদি (ফলাফল != 0)
        error (EXIT_FAILURE, errno, _("failed to run command: \"%s -c %s\""),
               shell_prog, filter_command);

      posix_spawnattr_destroy (&attr);
      posix_spawn_file_actions_destroy (&actions);

      যদি (close (fd_pair[0]) != 0)
        error (EXIT_FAILURE, errno, _("failed to close input pipe"));
      filter_pid = child_pid;
      যদি (open_pipes.n_pipes == open_pipes.n_pipes_alloc)
        open_pipes.pipes = xpalloc (open_pipes.pipes, &open_pipes.n_pipes_alloc,
                                    1, MIN (INT_MAX, IDX_MAX),
                                    আকার *open_pipes.pipes);
      idx_t ধ্রুবক pipe_idx = open_pipes.n_pipes++;
      open_pipes.pipes[pipe_idx] = fd_pair[1];
      ফেরত fd_pair[1];
    }
}

/* Close the output file, and do any associated cleanup.
   If FP and FD are both specified, they refer to the same open file;
   in this case FP is closed, but FD is still used in cleanup.  */
স্থির শূন্য
closeout (FILE *fp, পূর্ণসংখ্যা fd, pid_t pid, অক্ষর ধ্রুবক *name)
{
  যদি (fp != ফাঁকা && fclose (fp) != 0 && ! ignorable (errno))
    error (EXIT_FAILURE, errno, "%s", quotef (name));
  যদি (fd >= 0)
    {
      যদি (fp == ফাঁকা && close (fd) < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (name));
      জন্য (পূর্ণসংখ্যা j = 0; j < open_pipes.n_pipes; ++j)
        {
          যদি (open_pipes.pipes[j] == fd)
            {
              idx_t ধ্রুবক last_idx = open_pipes.n_pipes - 1;
              open_pipes.pipes[j] = open_pipes.pipes[last_idx];
              open_pipes.n_pipes--;
              বিরতি;
            }
        }
    }
  যদি (pid > 0)
    {
      পূর্ণসংখ্যা wstatus;
      যদি (waitpid (pid, &wstatus, 0) < 0)
        error (EXIT_FAILURE, errno, _("waiting for child process"));
      অন্যথা যদি (WIFSIGNALED (wstatus))
        {
          পূর্ণসংখ্যা sig = WTERMSIG (wstatus);
          যদি (sig != SIGPIPE)
            {
              অক্ষর signame[MAX (SIG2STR_MAX, INT_BUFSIZE_BOUND (পূর্ণসংখ্যা))];
              যদি (sig2str (sig, signame) != 0)
                sprintf (signame, "%d", sig);
              error (sig + 128, 0,
                     _("with FILE=%s, signal %s from command: %s"),
                     quotef (name), signame, filter_command);
            }
        }
      অন্যথা যদি (WIFEXITED (wstatus))
        {
          পূর্ণসংখ্যা ex = WEXITSTATUS (wstatus);
          যদি (ex != 0)
            error (ex, 0, _("with FILE=%s, exit %d from command: %s"),
                   quotef (name), ex, filter_command);
        }
      অন্যথা
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

স্থির bool
cwrite (bool new_file_flag, অক্ষর ধ্রুবক *bp, idx_t bytes)
{
  যদি (new_file_flag)
    {
      যদি (!bp && bytes == 0 && elide_empty_files)
        ফেরত true;
      closeout (ফাঁকা, output_desc, filter_pid, outfile);
      পরবর্তী_file_name ();
      output_desc = create (outfile);
      যদি (output_desc < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (outfile));
    }

  যদি (full_write (output_desc, bp, bytes) == bytes)
    ফেরত true;
  অন্যথা
    {
      যদি (! ignorable (errno))
        error (EXIT_FAILURE, errno, "%s", quotef (outfile));
      ফেরত false;
    }
}

/* Split into pieces of exactly N_BYTES bytes.
   However, the first REM_BYTES pieces should be 1 byte longer.
   Use buffer BUF, whose size is BUFSIZE.
   If INITIAL_READ is nonnegative,
   BUF contains the first INITIAL_READ input bytes.  */

স্থির শূন্য
bytes_split (intmax_t n_bytes, intmax_t rem_bytes,
             অক্ষর *buf, idx_t bufsize, ssize_t initial_read,
             intmax_t max_files)
{
  bool new_file_flag = true;
  bool filter_ok = true;
  intmax_t opened = 0;
  intmax_t to_write = n_bytes + (0 < rem_bytes);
  bool eof = ! to_write;

  যখন (! eof)
    {
      ssize_t n_read;
      যদি (0 <= initial_read)
        {
          n_read = initial_read;
          initial_read = -1;
          eof = n_read < bufsize;
        }
      অন্যথা
        {
          যদি (! filter_ok
              && 0 <= lseek (STDIN_FILENO, to_write, SEEK_CUR))
            {
              to_write = n_bytes + (opened + 1 < rem_bytes);
              new_file_flag = true;
            }

          n_read = পঠ (STDIN_FILENO, buf, bufsize);
          যদি (n_read < 0)
            error (EXIT_FAILURE, errno, "%s", quotef (infile));
          eof = n_read == 0;
        }
      অক্ষর *bp_out = buf;
      যখন (0 < to_write && to_write <= n_read)
        {
          যদি (filter_ok || new_file_flag)
            filter_ok = cwrite (new_file_flag, bp_out, to_write);
          opened += new_file_flag;
          new_file_flag = !max_files || (opened < max_files);
          যদি (! filter_ok && ! new_file_flag)
            {
              /* If filters no longer accepting input, stop reading.  */
              n_read = 0;
              eof = true;
              বিরতি;
            }
          bp_out += to_write;
          n_read -= to_write;
          to_write = n_bytes + (opened < rem_bytes);
        }
      যদি (0 < n_read)
        {
          যদি (filter_ok || new_file_flag)
            filter_ok = cwrite (new_file_flag, bp_out, n_read);
          opened += new_file_flag;
          new_file_flag = false;
          যদি (! filter_ok && opened == max_files)
            {
              /* If filters no longer accepting input, stop reading.  */
              বিরতি;
            }
          to_write -= n_read;
        }
    }

  /* Ensure NUMBER files are created, which truncates
     any existing files or notifies any consumers on fifos.
     FIXME: Should we do this before EXIT_FAILURE?  */
  যখন (opened++ < max_files)
    cwrite (true, ফাঁকা, 0);
}

/* Split into pieces of exactly N_LINES lines.
   Use buffer BUF, whose size is BUFSIZE.  */

স্থির শূন্য
সারিগুলি_split (intmax_t n_সারিগুলি, অক্ষর *buf, idx_t bufsize)
{
  ssize_t n_read;
  অক্ষর *bp, *bp_out, *eob;
  bool new_file_flag = true;
  intmax_t n = 0;

  করো
    {
      n_read = পঠ (STDIN_FILENO, buf, bufsize);
      যদি (n_read < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
      bp = bp_out = buf;
      eob = bp + n_read;
      *eob = eolchar;
      যখন (true)
        {
          bp = rawmemchr (bp, eolchar);
          যদি (bp == eob)
            {
              যদি (eob != bp_out) /* do not write 0 bytes! */
                {
                  idx_t len = eob - bp_out;
                  cwrite (new_file_flag, bp_out, len);
                  new_file_flag = false;
                }
              বিরতি;
            }

          ++bp;
          যদি (++n >= n_সারিগুলি)
            {
              cwrite (new_file_flag, bp_out, bp - bp_out);
              bp_out = bp;
              new_file_flag = true;
              n = 0;
            }
        }
    }
  যখন (n_read);
}

/* Split into pieces that are as large as possible while still not more
   than N_BYTES bytes, and are split on line boundaries except
   where lines longer than N_BYTES bytes occur. */

স্থির শূন্য
সারি_bytes_split (intmax_t n_bytes, অক্ষর *buf, idx_t bufsize)
{
  ssize_t n_read;
  intmax_t n_out = 0;      /* for each split.  */
  idx_t n_hold = 0;
  অক্ষর *hold = ফাঁকা;        /* for lines > bufsize.  */
  idx_t hold_আকার = 0;
  bool split_সারি = false;  /* Whether a \n was output in a split.  */

  করো
    {
      n_read = পঠ (STDIN_FILENO, buf, bufsize);
      যদি (n_read < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
      idx_t n_বাম = n_read;
      অক্ষর *sob = buf;
      যখন (n_বাম)
        {
          idx_t split_rest = 0;
          অক্ষর *eoc = ফাঁকা;
          অক্ষর *eol;

          /* Determine End Of Chunk and/or End of Line,
             which are used below to select what to write or buffer.  */
          যদি (n_bytes - n_out - n_hold <= n_বাম)
            {
              /* Have enough for split.  */
              split_rest = n_bytes - n_out - n_hold;
              eoc = sob + split_rest - 1;
              eol = memrchr (sob, eolchar, split_rest);
            }
          অন্যথা
            eol = memrchr (sob, eolchar, n_বাম);

          /* Output hold space if possible.  */
          যদি (n_hold && !(!eol && n_out))
            {
              cwrite (n_out == 0, hold, n_hold);
              n_out += n_hold;
              n_hold = 0;
            }

          /* Output to eol if present.  */
          যদি (eol)
            {
              split_সারি = true;
              idx_t n_write = eol - sob + 1;
              cwrite (n_out == 0, sob, n_write);
              n_out += n_write;
              n_বাম -= n_write;
              sob += n_write;
              যদি (eoc)
                split_rest -= n_write;
            }

          /* Output to eoc or eob if possible.  */
          যদি (n_বাম && !split_সারি)
            {
              idx_t n_write = eoc ? split_rest : n_বাম;
              cwrite (n_out == 0, sob, n_write);
              n_out += n_write;
              n_বাম -= n_write;
              sob += n_write;
              যদি (eoc)
                split_rest -= n_write;
            }

          /* Update hold if needed.  */
          যদি ((eoc && split_rest) || (!eoc && n_বাম))
            {
              idx_t n_buf = eoc ? split_rest : n_বাম;
              যদি (hold_আকার - n_hold < n_buf)
                hold = xpalloc (hold, &hold_আকার, n_buf - (hold_আকার - n_hold),
                                -1, আকার *hold);
              memcpy (hold + n_hold, sob, n_buf);
              n_hold += n_buf;
              n_বাম -= n_buf;
              sob += n_buf;
            }

          /* Reset for new split.  */
          যদি (eoc)
            {
              n_out = 0;
              split_সারি = false;
            }
        }
    }
  যখন (n_read);

  /* Handle no eol at end of file.  */
  যদি (n_hold)
    cwrite (n_out == 0, hold, n_hold);

  মুক্ত (hold);
}

/* -n l/[K/]N: Write lines to files of approximately file size / N.
   The file is partitioned into file size / N sized portions, with the
   last assigned any excess.  If a line _starts_ within a partition
   it is written completely to the corresponding file.  Since lines
   are not split even if they overlap a partition, the files written
   can be larger or smaller than the partition size, and even empty
   if a line is so long as to completely overlap the partition.  */

স্থির শূন্য
সারিগুলি_chunk_split (intmax_t k, intmax_t n, অক্ষর *buf, idx_t bufsize,
                   ssize_t initial_read, off_t file_আকার)
{
  affirm (n && k <= n);

  intmax_t rem_bytes = file_আকার % n;
  off_t chunk_আকার = file_আকার / n;
  intmax_t chunk_no = 1;
  off_t chunk_end = chunk_আকার + (0 < rem_bytes);
  off_t n_written = 0;
  bool new_file_flag = true;
  bool chunk_truncated = false;

  যদি (k > 1 && 0 < file_আকার)
    {
      /* Start reading 1 byte before kth chunk of file.  */
      off_t start = (k - 1) * chunk_আকার + MIN (k - 1, rem_bytes) - 1;
      যদি (start < initial_read)
        {
          memmove (buf, buf + start, initial_read - start);
          initial_read -= start;
        }
      অন্যথা
        {
          যদি (initial_read < start
              && lseek (STDIN_FILENO, start - initial_read, SEEK_CUR) < 0)
            error (EXIT_FAILURE, errno, "%s", quotef (infile));
          initial_read = -1;
        }
      n_written = start;
      chunk_no = k - 1;
      chunk_end = start + 1;
    }

  যখন (n_written < file_আকার)
    {
      অক্ষর *bp = buf, *eob;
      ssize_t n_read;
      যদি (0 <= initial_read)
        {
          n_read = initial_read;
          initial_read = -1;
        }
      অন্যথা
        {
          n_read = পঠ (STDIN_FILENO, buf,
                         MIN (bufsize, file_আকার - n_written));
          যদি (n_read < 0)
            error (EXIT_FAILURE, errno, "%s", quotef (infile));
        }
      যদি (n_read == 0)
        বিরতি; /* eof.  */
      chunk_truncated = false;
      eob = buf + n_read;

      যখন (bp != eob)
        {
          idx_t to_write;
          bool পরবর্তী = false;

          /* Begin looking for '\n' at last byte of chunk.  */
          off_t skip = MIN (n_read, MAX (0, chunk_end - 1 - n_written));
          অক্ষর *bp_out = memchr (bp + skip, eolchar, n_read - skip);
          যদি (bp_out)
            {
              bp_out++;
              পরবর্তী = true;
            }
          অন্যথা
            bp_out = eob;
          to_write = bp_out - bp;

          যদি (k == chunk_no)
            {
              /* We don't use the stdout buffer here since we're writing
                 large chunks from an existing file, so it's more efficient
                 to write out directly.  */
              যদি (full_write (STDOUT_FILENO, bp, to_write) != to_write)
                write_error ();
            }
          অন্যথা যদি (! k)
            cwrite (new_file_flag, bp, to_write);
          n_written += to_write;
          bp += to_write;
          n_read -= to_write;
          new_file_flag = পরবর্তী;

          /* A line could have been so long that it skipped
             entire chunks. So create empty files in that case.  */
          যখন (পরবর্তী || chunk_end <= n_written)
            {
              যদি (!পরবর্তী && bp == eob)
                {
                  /* replenish buf, before going to next chunk.  */
                  chunk_truncated = true;
                  বিরতি;
                }
              যদি (k == chunk_no)
                ফেরত;
              chunk_end += chunk_আকার + (chunk_no < rem_bytes);
              chunk_no++;
              যদি (chunk_end <= n_written)
                {
                  যদি (! k)
                    cwrite (true, ফাঁকা, 0);
                }
              অন্যথা
                পরবর্তী = false;
            }
        }
    }

  যদি (chunk_truncated)
    chunk_no++;

  /* Ensure NUMBER files are created, which truncates
     any existing files or notifies any consumers on fifos.
     FIXME: Should we do this before EXIT_FAILURE?  */
  যদি (!k)
    যখন (chunk_no++ <= n)
      cwrite (true, ফাঁকা, 0);
}

/* -n K/N: Extract Kth of N chunks.  */

স্থির শূন্য
bytes_chunk_extract (intmax_t k, intmax_t n, অক্ষর *buf, idx_t bufsize,
                     ssize_t initial_read, off_t file_আকার)
{
  off_t start;
  off_t end;

  affirm (0 < k && k <= n);

  start = (k - 1) * (file_আকার / n) + MIN (k - 1, file_আকার % n);
  end = k == n ? file_আকার : k * (file_আকার / n) + MIN (k, file_আকার % n);

  যদি (start < initial_read)
    {
      memmove (buf, buf + start, initial_read - start);
      initial_read -= start;
    }
  অন্যথা
    {
      যদি (initial_read < start
          && lseek (STDIN_FILENO, start - initial_read, SEEK_CUR) < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
      initial_read = -1;
    }

  যখন (start < end)
    {
      ssize_t n_read;
      যদি (0 <= initial_read)
        {
          n_read = initial_read;
          initial_read = -1;
        }
      অন্যথা
        {
          n_read = পঠ (STDIN_FILENO, buf, bufsize);
          যদি (n_read < 0)
            error (EXIT_FAILURE, errno, "%s", quotef (infile));
        }
      যদি (n_read == 0)
        বিরতি; /* eof.  */
      n_read = MIN (n_read, end - start);
      যদি (full_write (STDOUT_FILENO, buf, n_read) != n_read
          && ! ignorable (errno))
        error (EXIT_FAILURE, errno, "%s", quotef ("-"));
      start += n_read;
    }
}

প্রকারলেখ কাঠামো of_info
{
  অক্ষর *of_name;
  পূর্ণসংখ্যা ofd;
  FILE *ofile;
  pid_t opid;
} of_t;

গণনা
{
  OFD_NEW = -1,
  OFD_APPEND = -2
};

/* Rotate file descriptors when we're writing to more output files than we
   have available file descriptors.
   Return whether we came under file resource pressure.
   If so, it's probably best to close each file when finished with it.  */

স্থির bool
ofile_open (of_t *files, idx_t i_check, idx_t nfiles)
{
  bool file_limit = false;

  যদি (files[i_check].ofd <= OFD_NEW)
    {
      পূর্ণসংখ্যা fd;
      idx_t i_reopen = i_check ? i_check - 1 : nfiles - 1;

      /* Another process could have opened a file in between the calls to
         close and open, so we should keep trying until open succeeds or
         we've closed all of our files.  */
      যখন (true)
        {
          যদি (files[i_check].ofd == OFD_NEW)
            fd = create (files[i_check].of_name);
          অন্যথা /* OFD_APPEND  */
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
                         O_WRONLY | O_দ্বিমিক | O_APPEND | O_NONBLOCK);
            }

          যদি (0 <= fd)
            বিরতি;

          যদি (!(errno == EMFILE || errno == ENFILE))
            error (EXIT_FAILURE, errno, "%s", quotef (files[i_check].of_name));

          file_limit = true;

          /* Search backwards for an open file to close.  */
          যখন (files[i_reopen].ofd < 0)
            {
              i_reopen = i_reopen ? i_reopen - 1 : nfiles - 1;
              /* No more open files to close, exit with E[NM]FILE.  */
              যদি (i_reopen == i_check)
                error (EXIT_FAILURE, errno, "%s",
                       quotef (files[i_check].of_name));
            }

          যদি (fclose (files[i_reopen].ofile) != 0)
            error (EXIT_FAILURE, errno, "%s", quotef (files[i_reopen].of_name));
          files[i_reopen].ofile = ফাঁকা;
          files[i_reopen].ofd = OFD_APPEND;
        }

      files[i_check].ofd = fd;
      FILE *ofile = fdopen (fd, "a");
      যদি (!ofile)
        error (EXIT_FAILURE, errno, "%s", quotef (files[i_check].of_name));
      files[i_check].ofile = ofile;
      files[i_check].opid = filter_pid;
      filter_pid = 0;
    }

  ফেরত file_limit;
}

/* -n r/[K/]N: Divide file into N chunks in round robin fashion.
   Use BUF of size BUFSIZE for the buffer, and if allocating storage
   put its address into *FILESP to pacify -fsanitize=leak.
   When K == 0, we try to keep the files open in parallel.
   If we run out of file resources, then we revert
   to opening and closing each file for each line.  */

স্থির শূন্য
সারিগুলি_rr (intmax_t k, intmax_t n, অক্ষর *buf, idx_t bufsize, of_t **filesp)
{
  bool wrapped = false;
  bool wrote = false;
  bool file_limit;
  idx_t i_file;
  of_t *files IF_LINT (= ফাঁকা);
  intmax_t সারি_no;

  যদি (k)
    সারি_no = 1;
  অন্যথা
    {
      যদি (IDX_MAX < n)
        xalloc_die ();
      files = *filesp = xinmalloc (n, আকার *files);

      /* Generate output file names. */
      জন্য (i_file = 0; i_file < n; i_file++)
        {
          পরবর্তী_file_name ();
          files[i_file].of_name = xstrdup (outfile);
          files[i_file].ofd = OFD_NEW;
          files[i_file].ofile = ফাঁকা;
          files[i_file].opid = 0;
        }
      i_file = 0;
      file_limit = false;
    }

  যখন (true)
    {
      অক্ষর *bp = buf, *eob;
      ssize_t n_read = পঠ (STDIN_FILENO, buf, bufsize);
      যদি (n_read < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
      অন্যথা যদি (n_read == 0)
        বিরতি; /* eof.  */
      eob = buf + n_read;

      যখন (bp != eob)
        {
          idx_t to_write;
          bool পরবর্তী = false;

          /* Find end of line. */
          অক্ষর *bp_out = memchr (bp, eolchar, eob - bp);
          যদি (bp_out)
            {
              bp_out++;
              পরবর্তী = true;
            }
          অন্যথা
            bp_out = eob;
          to_write = bp_out - bp;

          যদি (k)
            {
              যদি (সারি_no == k && unbuffered)
                {
                  যদি (full_write (STDOUT_FILENO, bp, to_write) != to_write)
                    write_error ();
                }
              অন্যথা যদি (সারি_no == k && fwrite (bp, to_write, 1, stdout) != 1)
                {
                  write_error ();
                }
              যদি (পরবর্তী)
                সারি_no = (সারি_no == n) ? 1 : সারি_no + 1;
            }
          অন্যথা
            {
              /* Secure file descriptor. */
              file_limit |= ofile_open (files, i_file, n);
              যদি (unbuffered)
                {
                  /* Note writing to fd, rather than flushing the FILE gives
                     an 8% performance benefit, due to reduced data copying.  */
                  যদি (full_write (files[i_file].ofd, bp, to_write) != to_write
                      && ! ignorable (errno))
                    error (EXIT_FAILURE, errno, "%s",
                           quotef (files[i_file].of_name));
                }
              অন্যথা যদি (fwrite (bp, to_write, 1, files[i_file].ofile) != 1
                       && ! ignorable (errno))
                error (EXIT_FAILURE, errno, "%s",
                       quotef (files[i_file].of_name));

              যদি (! ignorable (errno))
                wrote = true;

              যদি (file_limit)
                {
                  যদি (fclose (files[i_file].ofile) != 0)
                    error (EXIT_FAILURE, errno, "%s",
                           quotef (files[i_file].of_name));
                  files[i_file].ofile = ফাঁকা;
                  files[i_file].ofd = OFD_APPEND;
                }
              যদি (পরবর্তী && ++i_file == n)
                {
                  wrapped = true;
                  /* If no filters are accepting input, stop reading.  */
                  যদি (! wrote)
                    যাও no_filters;
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
  যদি (!k)
    {
      idx_t ceiling = wrapped ? n : i_file;
      জন্য (i_file = 0; i_file < n; i_file++)
        {
          যদি (i_file >= ceiling && !elide_empty_files)
            file_limit |= ofile_open (files, i_file, n);
          যদি (files[i_file].ofd >= 0)
            closeout (files[i_file].ofile, files[i_file].ofd,
                      files[i_file].opid, files[i_file].of_name);
          files[i_file].ofd = OFD_APPEND;
        }
    }
}

#define FAIL_ONLY_ONE_WAY()					\
  করো								\
    {								\
      error (0, 0, _("cannot split in more than one way"));	\
      usage (EXIT_FAILURE);					\
    }								\
  যখন (0)

/* Report a string-to-integer conversion failure MSGID with ARG.  */

স্থির _Noreturn শূন্য
strtoint_die (অক্ষর ধ্রুবক *msgid, অক্ষর ধ্রুবক *arg)
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

স্থির intmax_t
parse_n_units (অক্ষর ধ্রুবক *arg, অক্ষর ধ্রুবক *multipliers, অক্ষর ধ্রুবক *msgid)
{
  intmax_t n;
  যদি (OVERFLOW_OK < xstrtoimax (arg, ফাঁকা, 10, &n, multipliers) || n < 1)
    strtoint_die (msgid, arg);
  ফেরত n;
}

/* Parse K/N syntax of chunk options.  */

স্থির শূন্য
parse_chunk (intmax_t *k_units, intmax_t *n_units, অক্ষর ধ্রুবক *arg)
{
  অক্ষর *argend;
  strtol_error e = xstrtoimax (arg, &argend, 10, n_units, "");
  যদি (e == LONGINT_INVALID_SUFFIX_CHAR && *argend == '/')
    {
      *k_units = *n_units;
      *n_units = parse_n_units (argend + 1, "",
                                N_("invalid number of chunks"));
      যদি (! (0 < *k_units && *k_units <= *n_units))
        error (EXIT_FAILURE, 0, "%s: %s", _("invalid chunk number"),
               quote_mem (arg, argend - arg));
    }
  অন্যথা যদি (! (e <= OVERFLOW_OK && 0 < *n_units))
    strtoint_die (N_("invalid number of chunks"), arg);
}


পূর্ণসংখ্যা
প্রধান (পূর্ণসংখ্যা argc, অক্ষর **argv)
{
  গণনা Split_type split_type = type_undef;
  idx_t in_blk_আকার = 0;	/* optimal block size of input file device */
  idx_t page_আকার = getpagesize ();
  intmax_t k_units = 0;
  intmax_t n_units = 0;

  স্থির অক্ষর ধ্রুবক multipliers[] = "bEGKkMmPQRTYZ0";
  পূর্ণসংখ্যা c;
  পূর্ণসংখ্যা digits_optind = 0;
  off_t file_আকার = OFF_T_MAX;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* Parse command line options.  */

  যখন (true)
    {
      /* This is the argv-index of the option we will read next.  */
      পূর্ণসংখ্যা this_optind = optind ? optind : 1;

      c = getopt_long (argc, argv, "0123456789C:a:b:del:n:t:ux",
                       longopts, ফাঁকা);
      যদি (c == -1)
        বিরতি;

      নির্বাচন (c)
        {
        ক্ষেত্র 'a':
          suffix_দৈর্ঘ্য = xdectoimax (optarg, 0, IDX_MAX,
                                      "", _("invalid suffix length"), 0);
          বিরতি;

        ক্ষেত্র ADDITIONAL_SUFFIX_OPTION:
          {
            পূর্ণসংখ্যা suffix_len = strlen (optarg);
            যদি (last_component (optarg) != optarg
                || (suffix_len && ISSLASH (optarg[suffix_len - 1])))
              {
                error (0, 0,
                       _("invalid suffix %s, contains directory separator"),
                       quote (optarg));
                usage (EXIT_FAILURE);
              }
          }
          additional_suffix = optarg;
          বিরতি;

        ক্ষেত্র 'b':
          যদি (split_type != type_undef)
            FAIL_ONLY_ONE_WAY ();
          split_type = type_bytes;
          n_units = parse_n_units (optarg, multipliers,
                                   N_("invalid number of bytes"));
          বিরতি;

        ক্ষেত্র 'l':
          যদি (split_type != type_undef)
            FAIL_ONLY_ONE_WAY ();
          split_type = type_সারিগুলি;
          n_units = parse_n_units (optarg, "", N_("invalid number of lines"));
          বিরতি;

        ক্ষেত্র 'C':
          যদি (split_type != type_undef)
            FAIL_ONLY_ONE_WAY ();
          split_type = type_byteslines;
          n_units = parse_n_units (optarg, multipliers,
                                   N_("invalid number of lines"));
          বিরতি;

        ক্ষেত্র 'n':
          যদি (split_type != type_undef)
            FAIL_ONLY_ONE_WAY ();
          /* skip any whitespace */
          যখন (isspace (to_uchar (*optarg)))
            optarg++;
          যদি (STRNCMP_LIT (optarg, "r/") == 0)
            {
              split_type = type_rr;
              optarg += 2;
            }
          অন্যথা যদি (STRNCMP_LIT (optarg, "l/") == 0)
            {
              split_type = type_chunk_সারিগুলি;
              optarg += 2;
            }
          অন্যথা
            split_type = type_chunk_bytes;
          parse_chunk (&k_units, &n_units, optarg);
          বিরতি;

        ক্ষেত্র 'u':
          unbuffered = true;
          বিরতি;

        ক্ষেত্র 't':
          {
            অক্ষর neweol = optarg[0];
            যদি (! neweol)
              error (EXIT_FAILURE, 0, _("empty record separator"));
            যদি (optarg[1])
              {
                যদি (streq (optarg, "\\0"))
                  neweol = '\0';
                অন্যথা
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
            যদি (0 <= eolchar && neweol != eolchar)
              {
                error (EXIT_FAILURE, 0,
                       _("multiple separator characters specified"));
              }

            eolchar = neweol;
          }
          বিরতি;

        ক্ষেত্র '0':
        ক্ষেত্র '1':
        ক্ষেত্র '2':
        ক্ষেত্র '3':
        ক্ষেত্র '4':
        ক্ষেত্র '5':
        ক্ষেত্র '6':
        ক্ষেত্র '7':
        ক্ষেত্র '8':
        ক্ষেত্র '9':
          যদি (split_type == type_undef)
            {
              split_type = type_digits;
              n_units = 0;
            }
          যদি (split_type != type_undef && split_type != type_digits)
            FAIL_ONLY_ONE_WAY ();
          যদি (digits_optind != 0 && digits_optind != this_optind)
            n_units = 0;	/* More than one number given; ignore other. */
          digits_optind = this_optind;
          যদি (ckd_mul (&n_units, n_units, 10)
              || ckd_add (&n_units, n_units, c - '0'))
            n_units = INTMAX_MAX;
          বিরতি;

        ক্ষেত্র 'd':
        ক্ষেত্র 'x':
          যদি (c == 'd')
            suffix_alphabet = "0123456789";
          অন্যথা
            suffix_alphabet = "0123456789abcdef";
          যদি (optarg)
            {
              যদি (strlen (optarg) != strspn (optarg, suffix_alphabet))
                {
                  error (0, 0,
                         (c == 'd') ?
                           _("%s: invalid start value for numerical suffix") :
                           _("%s: invalid start value for hexadecimal suffix"),
                         quote (optarg));
                  usage (EXIT_FAILURE);
                }
              অন্যথা
                {
                  /* Skip any leading zero.  */
                  যখন (*optarg == '0' && *(optarg + 1) != '\0')
                    optarg++;
                  numeric_suffix_start = optarg;
                }
            }
          বিরতি;

        ক্ষেত্র 'e':
          elide_empty_files = true;
          বিরতি;

        ক্ষেত্র FILTER_OPTION:
          filter_command = optarg;
          বিরতি;

        ক্ষেত্র IO_BLKSIZE_OPTION:
          in_blk_আকার = xnumtoumax (optarg, 10, 1,
                                    MIN (SYS_BUFSIZE_MAX,
                                         MIN (IDX_MAX, SIZE_MAX) - 1),
                                    multipliers, _("invalid IO block size"),
                                    0, XTOINT_MIN_RANGE);
          বিরতি;

        ক্ষেত্র VERBOSE_OPTION:
          verbose = true;
          বিরতি;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        পূর্বনির্ধারিত:
          usage (EXIT_FAILURE);
        }
    }

  যদি (k_units != 0 && filter_command)
    {
      error (0, 0, _("--filter does not process a chunk extracted to "
                     "standard output"));
      usage (EXIT_FAILURE);
    }

  /* Handle default case.  */
  যদি (split_type == type_undef)
    {
      split_type = type_সারিগুলি;
      n_units = 1000;
    }

  যদি (n_units == 0)
    {
      error (0, 0, _("invalid number of lines: %s"), quote ("0"));
      usage (EXIT_FAILURE);
    }

  যদি (eolchar < 0)
    eolchar = '\n';

  set_suffix_দৈর্ঘ্য (n_units, split_type);

  /* Get out the filename arguments.  */

  যদি (optind < argc)
    infile = argv[optind++];

  যদি (optind < argc)
    outbase = argv[optind++];

  যদি (optind < argc)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind]));
      usage (EXIT_FAILURE);
    }

  /* Check that the suffix length is large enough for the numerical
     suffix start value.  */
  যদি (numeric_suffix_start && strlen (numeric_suffix_start) > suffix_দৈর্ঘ্য)
    {
      error (0, 0, _("numerical suffix start value is too large "
                     "for the suffix length"));
      usage (EXIT_FAILURE);
    }

  /* Open the input file.  */
  যদি (! streq (infile, "-")
      && fd_reopen (STDIN_FILENO, infile, O_RDONLY, 0) < 0)
    error (EXIT_FAILURE, errno, _("cannot open %s for reading"),
           quoteaf (infile));

  /* Binary I/O is safer when byte counts are used.  */
  xset_দ্বিমিক_mode (STDIN_FILENO, O_দ্বিমিক);

  /* Advise the kernel of our access pattern.  */
  fdadvise (STDIN_FILENO, 0, 0, FADVISE_SEQUENTIAL);

  /* Get the optimal block size of input device and make a buffer.  */

  যদি (fstat (STDIN_FILENO, &in_stat_buf) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (infile));

  যদি (in_blk_আকার == 0)
    {
      in_blk_আকার = io_blksize (&in_stat_buf);
      যদি (SYS_BUFSIZE_MAX < in_blk_আকার)
        in_blk_আকার = SYS_BUFSIZE_MAX;
    }

  অক্ষর *buf = xalignalloc (page_আকার, in_blk_আকার + 1);
  ssize_t initial_read = -1;

  যদি (split_type == type_chunk_bytes || split_type == type_chunk_সারিগুলি)
    {
      file_আকার = প্রবেশ_file_আকার (STDIN_FILENO, &in_stat_buf,
                                   buf, in_blk_আকার);
      যদি (file_আকার < 0)
        error (EXIT_FAILURE, errno, _("%s: cannot determine file size"),
               quotef (infile));
      initial_read = MIN (file_আকার, in_blk_আকার);
    }

  /* When filtering, closure of one pipe must not terminate the process,
     as there may still be other streams expecting input from us.  */
  যদি (filter_command)
    default_SIGPIPE = সংকেত (SIGPIPE, SIG_IGN) == SIG_DFL;

  নির্বাচন (split_type)
    {
    ক্ষেত্র type_digits:
    ক্ষেত্র type_সারিগুলি:
      সারিগুলি_split (n_units, buf, in_blk_আকার);
      বিরতি;

    ক্ষেত্র type_bytes:
      bytes_split (n_units, 0, buf, in_blk_আকার, -1, 0);
      বিরতি;

    ক্ষেত্র type_byteslines:
      সারি_bytes_split (n_units, buf, in_blk_আকার);
      বিরতি;

    ক্ষেত্র type_chunk_bytes:
      যদি (k_units == 0)
        bytes_split (file_আকার / n_units, file_আকার % n_units,
                     buf, in_blk_আকার, initial_read, n_units);
      অন্যথা
        bytes_chunk_extract (k_units, n_units, buf, in_blk_আকার, initial_read,
                             file_আকার);
      বিরতি;

    ক্ষেত্র type_chunk_সারিগুলি:
      সারিগুলি_chunk_split (k_units, n_units, buf, in_blk_আকার, initial_read,
                         file_আকার);
      বিরতি;

    ক্ষেত্র type_rr:
      /* Note, this is like 'sed -n ${k}~${n}p' when k > 0,
         but the functionality is provided for symmetry.  */
      {
        of_t *files;
        সারিগুলি_rr (k_units, n_units, buf, in_blk_আকার, &files);
      }
      বিরতি;

    ক্ষেত্র type_undef: পূর্বনির্ধারিত:
      affirm (false);
    }

  যদি (close (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (infile));
  closeout (ফাঁকা, output_desc, filter_pid, outfile);

  main_exit (EXIT_SUCCESS);
}
