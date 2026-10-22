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
#అన్తర్భావ <config.h>

#అన్తర్భావ <stdio.h>
#అన్తర్భావ <getopt.h>
#అన్తర్భావ <signal.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <sys/wait.h>
#అన్తర్భావ <spawn.h>

#అన్తర్భావ "system.h"
#అన్తర్భావ "alignalloc.h"
#అన్తర్భావ "assure.h"
#అన్తర్భావ "fadvise.h"
#అన్తర్భావ "fd-reopen.h"
#అన్తర్భావ "fcntl--.h"
#అన్తర్భావ "full-write.h"
#అన్తర్భావ "ioblksize.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "sig2str.h"
#అన్తర్భావ "sys-limits.h"
#అన్తర్భావ "temp-stream.h"
#అన్తర్భావ "unistd--.h"
#అన్తర్భావ "xbinary-io.h"
#అన్తర్భావ "xdectoint.h"
#అన్తర్భావ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "split"

#నిర్వచించు AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("Richard M. Stallman")

/* Shell command to filter through, instead of creating files.  */
స్థిర అక్షరం స్థిరరాశి *filter_command;

/* Process ID of the filter.  */
స్థిర pid_t filter_pid;

స్థిర నిర్మాణం
{
  /* The number of open pipes.  */
  పూర్ణాంకం n_pipes;

  /* The number of allocated pipes.  */
  idx_t n_pipes_alloc;

  /* Array of open pipes.  */
  పూర్ణాంకం *pipes COUNTED_BY (n_pipes);
} open_pipes;

/* Whether SIGPIPE has the default action, when --filter is used.  */
స్థిర bool default_SIGPIPE;

/* Base name of output files.  */
స్థిర అక్షరం స్థిరరాశి *outbase = "x";

/* Name of output files.  */
స్థిర అక్షరం *outfile;

/* Pointer to the end of the prefix in OUTFILE.
   Suffixes are inserted here.  */
స్థిర అక్షరం *outfile_mid;

/* Generate new suffix when suffixes are exhausted.  */
స్థిర bool suffix_auto = true;

/* Length of OUTFILE's suffix.  */
స్థిర idx_t suffix_నిడివి;

/* Alphabet of characters to use in suffix.  */
స్థిర అక్షరం స్థిరరాశి *suffix_alphabet = "abcdefghijklmnopqrstuvwxyz";

/* Numerical suffix start value.  */
స్థిర అక్షరం స్థిరరాశి *numeric_suffix_ప్రారంభం;

/* Additional suffix to append to output file names.  */
స్థిర అక్షరం స్థిరరాశి *additional_suffix;

/* Name of input file.  May be "-".  */
స్థిర అక్షరం స్థిరరాశి *infile = "-";

/* stat buf for input file.  */
స్థిర నిర్మాణం stat లోపల_stat_buf;

/* Descriptor on which output file is open.  */
స్థిర పూర్ణాంకం output_desc = -1;

/* If true, print a diagnostic on standard error just before each
   output file is opened. */
స్థిర bool verbose;

/* If true, don't generate zero length output files. */
స్థిర bool elide_ఖాళీలు_files;

/* If true, in round robin mode, immediately copy
   input to output, which is much slower, so disabled by default.  */
స్థిర bool unbuffered;

/* The character marking end of line.  Defaults to \n below.  */
స్థిర పూర్ణాంకం eolchar = -1;

/* The split mode to use.  */
గణన విభజించు_type
{
  type_undef, type_బైట్లు, type_byteslines, type_పంక్తులు, type_digits,
  type_chunk_బైట్లు, type_chunk_పంక్తులు, type_rr
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
గణన
{
  VERBOSE_OPTION = CHAR_MAX + 1,
  FILTER_OPTION,
  IO_BLKSIZE_OPTION,
  ADDITIONAL_SUFFIX_OPTION
};

స్థిర నిర్మాణం option స్థిరరాశి longopts[] =
{
  {"bytes", required_వాదం, ఖాళీ, 'b'},
  {"lines", required_వాదం, ఖాళీ, 'l'},
  {"line-bytes", required_వాదం, ఖాళీ, 'C'},
  {"number", required_వాదం, ఖాళీ, 'n'},
  {"elide-empty-files", no_వాదం, ఖాళీ, 'e'},
  {"unbuffered", no_వాదం, ఖాళీ, 'u'},
  {"suffix-length", required_వాదం, ఖాళీ, 'a'},
  {"additional-suffix", required_వాదం, ఖాళీ,
   ADDITIONAL_SUFFIX_OPTION},
  {"numeric-suffixes", optional_వాదం, ఖాళీ, 'd'},
  {"hex-suffixes", optional_వాదం, ఖాళీ, 'x'},
  {"filter", required_వాదం, ఖాళీ, FILTER_OPTION},
  {"verbose", no_వాదం, ఖాళీ, VERBOSE_OPTION},
  {"separator", required_వాదం, ఖాళీ, 't'},
  {"-io-blksize", required_వాదం, ఖాళీ,
   IO_BLKSIZE_OPTION}, /* do not document */
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0}
};

/* Return true if the errno value, ERR, is ignorable.  */
స్థిర అంతఃవరుస bool
ignorable (పూర్ణాంకం err)
{
  తిరిగి filter_command && err == EPIPE;
}

స్థిర శూన్య
సమితి_suffix_నిడివి (intmax_t n_units, గణన విభజించు_type విభజించు_type_2)
{
#నిర్వచించు DEFAULT_SUFFIX_నిడివి 2

  పూర్ణాంకం suffix_నిడివి_needed = 0;

  /* The suffix auto length feature is incompatible with
     a user specified start value as the generated suffixes
     are not all consecutive.  */
  ఒకవేళ (numeric_suffix_ప్రారంభం)
    suffix_auto = false;

  /* Auto-calculate the suffix length if the number of files is given.  */
  ఒకవేళ (విభజించు_type_2 == type_chunk_బైట్లు || విభజించు_type_2 == type_chunk_పంక్తులు
      || విభజించు_type_2 == type_rr)
    {
      intmax_t n_units_చివర = n_units - 1;
      ఒకవేళ (numeric_suffix_ప్రారంభం)
        {
          intmax_t n_ప్రారంభం;
          strtol_error e = xstrtoimax (numeric_suffix_ప్రారంభం, ఖాళీ, 10,
                                       &n_ప్రారంభం, "");
          ఒకవేళ (e == LONGINT_OK && n_ప్రారంభం < n_units)
            {
              /* Restrict auto adjustment so we don't keep
                 incrementing a suffix size arbitrarily,
                 as that would break sort order for files
                 generated from multiple split runs.  */
              ఒకవేళ (ckd_కూడు (&n_units_చివర, n_units_చివర, n_ప్రారంభం))
                n_units_చివర = INTMAX_MAX;
            }

        }
      idx_t alphabet_len = అక్షరశ్రేణి_పొడవు (suffix_alphabet);
      చేయి
        suffix_నిడివి_needed++;
      అయితే (n_units_చివర /= alphabet_len);

      suffix_auto = false;
    }

  ఒకవేళ (suffix_నిడివి)            /* set by user */
    {
      ఒకవేళ (suffix_నిడివి < suffix_నిడివి_needed)
        error (EXIT_FAILURE, 0,
               _("the suffix length needs to be at least %d"),
               suffix_నిడివి_needed);
      suffix_auto = false;
      తిరిగి;
    }
  లేకపోతే
    suffix_నిడివి = MAX (DEFAULT_SUFFIX_నిడివి, suffix_నిడివి_needed);
}

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("\
Usage: %s [OPTION]... [FILE [ఉపసర్గ_2]]\n\
"),
              కార్యక్రమం_name);
      fputs (_("\
Output pieces యొక్క FILE గమ్యం PREFIXaa, PREFIXab, ...;\n\
అప్రమేయ కొలత is 1000 పంక్తులు, and అప్రమేయ ఉపసర్గ_2 is 'x'.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oprintf (_("\
  -a, --suffix-నిడివి=N\n\
         generate suffixes యొక్క నిడివి N (అప్రమేయ %d)\n\
"), DEFAULT_SUFFIX_నిడివి);
      oputs (_("\
      --additional-suffix=SUFFIX\n\
         append an additional SUFFIX గమ్యం file పేర్లు\n\
"));
      oputs (_("\
  -b, --బైట్లు=కొలత_2\n\
         put కొలత_2 బైట్లు చొప్పున output file\n\
"));
      oputs (_("\
  -C, --పంక్తి-బైట్లు=కొలత_2\n\
         put at most కొలత_2 బైట్లు యొక్క రికార్డులు చొప్పున output file\n\
"));
      oputs (_("\
  -d\n\
         use numeric suffixes starting at 0, not alphabetic\n\
"));
      oputs (_("\
      --numeric-suffixes[=FROM]\n\
         same as -d, but allow setting the ప్రారంభం విలువ\n\
"));
      oputs (_("\
  -x\n\
         use hex suffixes starting at 0, not alphabetic\n\
"));
      oputs (_("\
      --hex-suffixes[=FROM]\n\
         same as -x, but allow setting the ప్రారంభం విలువ\n\
"));
      oputs (_("\
  -e, --elide-ఖాళీలు-files\n\
         చేయి not generate ఖాళీలు output files with '-n'\n\
"));
      oputs (_("\
      --filter=COMMAND\n\
         లిఖ గమ్యం shell COMMAND; file name is $FILE\n\
"));
      oputs (_("\
  -l, --పంక్తులు=సంఖ్య_2\n\
         put సంఖ్య_2 పంక్తులు/రికార్డులు చొప్పున output file\n\
"));
      oputs (_("\
  -n, --సంఖ్య=CHUNKS\n\
         generate CHUNKS output files; see explanation క్రింద\n\
"));
      oputs (_("\
  -t, --separator=SEP\n\
         use SEP instead యొక్క newline as the రికార్డు separator;\n\
         '\\0' (zero) specifies the NUL character\n\
"));
      oputs (_("\
  -u, --unbuffered\n\
         immediately copy ఆదానం గమ్యం output with '-n r/...'\n\
"));
      oputs (_("\
      --verbose\n\
         print a diagnostic just before each output file is opened\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      emit_కొలత_note ();
      fputs (_("\n\
CHUNKS may be:\n\
  N       విభజించు into N files based on కొలత యొక్క ఆదానం\n\
  K/N     output Kth యొక్క N గమ్యం standard output\n\
  l/N     విభజించు into N files without splitting పంక్తులు/రికార్డులు\n\
  l/K/N   output Kth యొక్క N గమ్యం standard output without splitting పంక్తులు/రికార్డులు\n\
  r/N     like 'l' but use విడత_2 robin distribution\n\
  r/K/N   likewise but only output Kth యొక్క N గమ్యం standard output\n\
"), stdout);
      fputs (_("\n\
-n (except -nr) will బఫరు గమ్యం $TMPDIR, defaulting గమ్యం /tmp,\n\
ఒకవేళ the ఆదానం కొలత cannot easily be determined.\n\
"), stdout);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

/* Copy the data in FD to a temporary file, then make that file FD.
   Use BUF, of size BUFSIZE, to copy.  Return the number of
   bytes copied, or -1 (setting errno) on error.  */
స్థిర off_t
copy_గమ్యం_tmpfile (పూర్ణాంకం fd, అక్షరం *buf, idx_t bufsize)
{
  FILE *tmp;
  ఒకవేళ (!తాత్కాలిక_ప్రవాహం (&tmp, ఖాళీ))
    తిరిగి -1;
  off_t copied = 0;
  off_t r;

  అయితే (0 < (r = పఠ (fd, buf, bufsize)))
    {
      ఒకవేళ (fwrite (buf, 1, r, tmp) != r)
        తిరిగి -1;
      ఒకవేళ (ckd_కూడు (&copied, copied, r))
        {
          errno = EOVERFLOW;
          తిరిగి -1;
        }
    }

  ఒకవేళ (r < 0)
    తిరిగి r;
  r = dup2 (fileno (tmp), fd);
  ఒకవేళ (r < 0)
    తిరిగి r;
  ఒకవేళ (fclose (tmp) < 0)
    తిరిగి -1;
  తిరిగి copied;
}

/* Return the number of bytes that can be read from FD with status ST.
   Store up to the first BUFSIZE bytes of the file's data into BUF,
   and advance the file position by the number of bytes read.  On
   input error, set errno and return -1.  */

స్థిర off_t
ఆదానం_file_కొలత (పూర్ణాంకం fd, నిర్మాణం stat స్థిరరాశి *st, అక్షరం *buf, idx_t bufsize)
{
  off_t కొలత = 0;
  చేయి
    {
      ssize_t n_read = పఠ (fd, buf + కొలత, bufsize - కొలత);
      ఒకవేళ (n_read <= 0)
        తిరిగి n_read < 0 ? n_read : కొలత;
      కొలత += n_read;
    }
  అయితే (కొలత < bufsize);

  off_t cur, చివర;
  ఒకవేళ ((usable_st_కొలత (st) && st->st_కొలత < కొలత)
      || (cur = lseek (fd, 0, SEEK_CUR)) < 0
      || cur < కొలత /* E.g., /dev/zero on GNU/Linux.  */
      || (చివర = lseek (fd, 0, SEEK_END)) < 0)
    {
      అక్షరం *tmpbuf = xmalloc (bufsize);
      చివర = copy_గమ్యం_tmpfile (fd, tmpbuf, bufsize);
      విడుదల (tmpbuf);
      ఒకవేళ (చివర < 0)
        తిరిగి చివర;
      cur = 0;
    }

  ఒకవేళ (చివర == OFF_T_MAX /* E.g., /dev/zero on GNU/Hurd.  */
      || (cur < చివర && ckd_కూడు (&కొలత, కొలత, చివర - cur)))
    {
      errno = EOVERFLOW;
      తిరిగి -1;
    }

  ఒకవేళ (cur < చివర)
    {
      off_t r = lseek (fd, cur, SEEK_SET);
      ఒకవేళ (r < 0)
        తిరిగి r;
    }

  తిరిగి కొలత;
}

/* Compute the next sequential output file name and store it into the
   string 'outfile'.  */

స్థిర శూన్య
తదుపరి_file_name (శూన్య)
{
  /* Index in suffix_alphabet of each character in the suffix.  */
  స్థిర idx_t *sufindex;
  స్థిర idx_t outbase_నిడివి;
  స్థిర idx_t outfile_నిడివి;
  స్థిర idx_t addsuf_నిడివి;

  ఒకవేళ (! outfile)
    {
      bool అధికప్రవాహం, widen;

new_name:
      widen = !! outfile_నిడివి;

      ఒకవేళ (! widen)
        {
          /* Allocate and initialize the first file name.  */

          outbase_నిడివి = అక్షరశ్రేణి_పొడవు (outbase);
          addsuf_నిడివి = additional_suffix ? అక్షరశ్రేణి_పొడవు (additional_suffix) : 0;
          అధికప్రవాహం = ckd_కూడు (&outfile_నిడివి, outbase_నిడివి + addsuf_నిడివి,
                              suffix_నిడివి);
        }
      లేకపోతే
        {
          /* Reallocate and initialize a new wider file name.
             We do this by subsuming the unchanging part of
             the generated suffix into the prefix (base), and
             reinitializing the now one longer suffix.  */

          అధికప్రవాహం = ckd_కూడు (&outfile_నిడివి, outfile_నిడివి, 2);
          suffix_నిడివి++;
        }

      idx_t outfile_కొలత;
      అధికప్రవాహం |= ckd_కూడు (&outfile_కొలత, outfile_నిడివి, 1);
      ఒకవేళ (అధికప్రవాహం)
        xalloc_die ();
      outfile = xirealloc (outfile, outfile_కొలత);

      ఒకవేళ (! widen)
        జ్ఞాపకం_నకలు (outfile, outbase, outbase_నిడివి);
      లేకపోతే
        {
          /* Append the last alphabet character to the file name prefix.  */
          outfile[outbase_నిడివి] = suffix_alphabet[sufindex[0]];
          outbase_నిడివి++;
        }

      outfile_mid = outfile + outbase_నిడివి;
      memset (outfile_mid, suffix_alphabet[0], suffix_నిడివి);
      ఒకవేళ (additional_suffix)
        జ్ఞాపకం_నకలు (outfile_mid + suffix_నిడివి, additional_suffix, addsuf_నిడివి);
      outfile[outfile_నిడివి] = 0;

      విడుదల (sufindex);
      sufindex = xicalloc (suffix_నిడివి, పరిమాణం *sufindex);

      ఒకవేళ (numeric_suffix_ప్రారంభం)
        {
          affirm (! widen);

          /* Update the output file name.  */
          idx_t i = అక్షరశ్రేణి_పొడవు (numeric_suffix_ప్రారంభం);
          జ్ఞాపకం_నకలు (outfile_mid + suffix_నిడివి - i, numeric_suffix_ప్రారంభం, i);

          /* Update the suffix index.  */
          idx_t *sufindex_చివర = sufindex + suffix_నిడివి;
          అయితే (i-- != 0)
            *--sufindex_చివర = numeric_suffix_ప్రారంభం[i] - '0';
        }

#ఒకవేళ ! _POSIX_NO_TRUNC && HAVE_PATHCONF && defined _PC_NAME_MAX
      /* POSIX requires that if the output file name is too long for
         its directory, 'split' must fail without creating any files.
         This must be checked for explicitly on operating systems that
         silently truncate file names.  */
      {
        అక్షరం *dir = dir_name (outfile);
        దీర్ఘ name_max = pathconf (dir, _PC_NAME_MAX);
        ఒకవేళ (0 <= name_max && name_max < పునాది_len (చివరి_component (outfile)))
          error (EXIT_FAILURE, ENAMETOOLONG, "%s", quotef (outfile));
        విడుదల (dir);
      }
#అయితే_ముగింపు
    }
  లేకపోతే
    {
      /* Increment the suffix in place, if possible.  */

      idx_t i = suffix_నిడివి;
      అయితే (i-- != 0)
        {
          sufindex[i]++;
          ఒకవేళ (suffix_auto && i == 0 && ! suffix_alphabet[sufindex[0] + 1])
            వెళ్ళు new_name;
          outfile_mid[i] = suffix_alphabet[sufindex[i]];
          ఒకవేళ (outfile_mid[i])
            తిరిగి;
          sufindex[i] = 0;
          outfile_mid[i] = suffix_alphabet[sufindex[i]];
        }
      error (EXIT_FAILURE, 0, _("output file suffixes exhausted"));
    }
}

/* Create or truncate a file.  */

స్థిర పూర్ణాంకం
create (అక్షరం స్థిరరాశి *name)
{
  ఒకవేళ (!filter_command)
    {
      ఒకవేళ (verbose)
        fprintf (stdout, _("creating file %s\n"), quoteaf (name));

      పూర్ణాంకం oflags = O_WRONLY | O_CREAT | O_ద్విమాన;
      పూర్ణాంకం fd = open (name, oflags | O_EXCL, బహుళకం_RW_UGO);
      ఒకవేళ (0 <= fd || errno != EEXIST)
        తిరిగి fd;
      fd = open (name, oflags, బహుళకం_RW_UGO);
      ఒకవేళ (fd < 0)
        తిరిగి fd;
      నిర్మాణం stat బహిర్గతం_stat_buf;
      ఒకవేళ (fstat (fd, &బహిర్గతం_stat_buf) != 0)
        error (EXIT_FAILURE, errno, _("failed to stat %s"), quoteaf (name));
      ఒకవేళ (psame_inode (&లోపల_stat_buf, &బహిర్గతం_stat_buf))
        error (EXIT_FAILURE, 0, _("%s would overwrite input; aborting"),
               quoteaf (name));
      ఒకవేళ (ftruncate (fd, 0) < 0
          && (S_ISREG (బహిర్గతం_stat_buf.st_బహుళకం) || S_TYPEISSHM (&బహిర్గతం_stat_buf)))
        error (EXIT_FAILURE, errno, _("%s: error truncating"), quotef (name));

      తిరిగి fd;
    }
  లేకపోతే
    {
      ఒకవేళ (setenv ("FILE", name, 1) != 0)
        error (EXIT_FAILURE, errno,
               _("failed to set FILE environment variable"));
      ఒకవేళ (verbose)
        fprintf (stdout, _("executing with FILE=%s\n"), quotef (name));

      పూర్ణాంకం ఫలితం;
      పూర్ణాంకం fd_pair[2];
      pid_t బిడ్డ_pid;

      posix_spawnattr_t attr;
      posix_spawn_file_actions_t actions;

      sigset_t సమితి_2;
      sigemptyset (&సమితి_2);
      ఒకవేళ (default_SIGPIPE)
        sigaddset (&సమితి_2, SIGPIPE);

      ఒకవేళ (   (ఫలితం = posix_spawnattr_init (&attr))
          || (ఫలితం = posix_spawnattr_setflags (&attr,
                                                 (POSIX_SPAWN_USEVFORK
                                                  | POSIX_SPAWN_SETSIGDEF)))
          || (ఫలితం = posix_spawnattr_setsigdefault (&attr, &సమితి_2))
          || (ఫలితం = posix_spawn_file_actions_init (&actions))
         )
        error (EXIT_FAILURE, ఫలితం, _("posix_spawn initialization failed"));

      ఒకవేళ (pipe (fd_pair) != 0)
        error (EXIT_FAILURE, errno, _("failed to create pipe"));

      /* We have to close any pipes that were opened during an
         earlier call, otherwise this process will be holding a
         write-pipe that will prevent the earlier process from
         reading an EOF on the corresponding read-pipe.  */
      కొరకు (పూర్ణాంకం i = 0; i < open_pipes.n_pipes; ++i)
        ఒకవేళ ((ఫలితం = posix_spawn_file_actions_addclose (&actions,
                                                         open_pipes.pipes[i])))
          ఆపు;

      ఒకవేళ (   ఫలితం
          || (ఫలితం = posix_spawn_file_actions_addclose (&actions, fd_pair[1]))
          || (fd_pair[0] != STDIN_FILENO
              && (   (ఫలితం = posix_spawn_file_actions_adddup2 (&actions,
                                                                 fd_pair[0],
                                                                 STDIN_FILENO))
                  || (ఫలితం = posix_spawn_file_actions_addclose (&actions,
                                                                  fd_pair[0]))))
         )
        error (EXIT_FAILURE, ఫలితం, _("posix_spawn setup failed"));


      అక్షరం స్థిరరాశి *shell_prog = getenv ("SHELL");
      ఒకవేళ (shell_prog == ఖాళీ)
        shell_prog = "/bin/sh";
      అక్షరం స్థిరరాశి *స్థిరరాశి argv[] = { చివరి_component (shell_prog), "-c",
                                   filter_command, ఖాళీ };

      ఫలితం = posix_spawn (&బిడ్డ_pid, shell_prog, &actions, &attr,
                            (అక్షరం * స్థిరరాశి *) argv, environ);
      ఒకవేళ (ఫలితం != 0)
        error (EXIT_FAILURE, errno, _("failed to run command: \"%s -c %s\""),
               shell_prog, filter_command);

      posix_spawnattr_destroy (&attr);
      posix_spawn_file_actions_destroy (&actions);

      ఒకవేళ (close (fd_pair[0]) != 0)
        error (EXIT_FAILURE, errno, _("failed to close input pipe"));
      filter_pid = బిడ్డ_pid;
      ఒకవేళ (open_pipes.n_pipes == open_pipes.n_pipes_alloc)
        open_pipes.pipes = xpalloc (open_pipes.pipes, &open_pipes.n_pipes_alloc,
                                    1, MIN (గరిష్ఠ_పూర్ణాంకం, IDX_MAX),
                                    పరిమాణం *open_pipes.pipes);
      idx_t స్థిరరాశి pipe_idx = open_pipes.n_pipes++;
      open_pipes.pipes[pipe_idx] = fd_pair[1];
      తిరిగి fd_pair[1];
    }
}

/* Close the output file, and do any associated cleanup.
   If FP and FD are both specified, they refer to the same open file;
   in this case FP is closed, but FD is still used in cleanup.  */
స్థిర శూన్య
closeout (FILE *fp, పూర్ణాంకం fd, pid_t pid, అక్షరం స్థిరరాశి *name)
{
  ఒకవేళ (fp != ఖాళీ && fclose (fp) != 0 && ! ignorable (errno))
    error (EXIT_FAILURE, errno, "%s", quotef (name));
  ఒకవేళ (fd >= 0)
    {
      ఒకవేళ (fp == ఖాళీ && close (fd) < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (name));
      కొరకు (పూర్ణాంకం j = 0; j < open_pipes.n_pipes; ++j)
        {
          ఒకవేళ (open_pipes.pipes[j] == fd)
            {
              idx_t స్థిరరాశి చివరి_idx = open_pipes.n_pipes - 1;
              open_pipes.pipes[j] = open_pipes.pipes[చివరి_idx];
              open_pipes.n_pipes--;
              ఆపు;
            }
        }
    }
  ఒకవేళ (pid > 0)
    {
      పూర్ణాంకం wstatus;
      ఒకవేళ (waitpid (pid, &wstatus, 0) < 0)
        error (EXIT_FAILURE, errno, _("waiting for child process"));
      లేకపోతే ఒకవేళ (WIFSIGNALED (wstatus))
        {
          పూర్ణాంకం sig = WTERMSIG (wstatus);
          ఒకవేళ (sig != SIGPIPE)
            {
              అక్షరం signame[MAX (SIG2STR_MAX, INT_BUFSIZE_BOUND (పూర్ణాంకం))];
              ఒకవేళ (sig2str (sig, signame) != 0)
                sprintf (signame, "%d", sig);
              error (sig + 128, 0,
                     _("with FILE=%s, signal %s from command: %s"),
                     quotef (name), signame, filter_command);
            }
        }
      లేకపోతే ఒకవేళ (WIFEXITED (wstatus))
        {
          పూర్ణాంకం ex = WEXITSTATUS (wstatus);
          ఒకవేళ (ex != 0)
            error (ex, 0, _("with FILE=%s, exit %d from command: %s"),
                   quotef (name), ex, filter_command);
        }
      లేకపోతే
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

స్థిర bool
cwrite (bool new_file_flag, అక్షరం స్థిరరాశి *bp, idx_t బైట్లు)
{
  ఒకవేళ (new_file_flag)
    {
      ఒకవేళ (!bp && బైట్లు == 0 && elide_ఖాళీలు_files)
        తిరిగి true;
      closeout (ఖాళీ, output_desc, filter_pid, outfile);
      తదుపరి_file_name ();
      output_desc = create (outfile);
      ఒకవేళ (output_desc < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (outfile));
    }

  ఒకవేళ (నిండిన_write (output_desc, bp, బైట్లు) == బైట్లు)
    తిరిగి true;
  లేకపోతే
    {
      ఒకవేళ (! ignorable (errno))
        error (EXIT_FAILURE, errno, "%s", quotef (outfile));
      తిరిగి false;
    }
}

/* Split into pieces of exactly N_BYTES bytes.
   However, the first REM_BYTES pieces should be 1 byte longer.
   Use buffer BUF, whose size is BUFSIZE.
   If INITIAL_READ is nonnegative,
   BUF contains the first INITIAL_READ input bytes.  */

స్థిర శూన్య
బైట్లు_విభజించు (intmax_t n_బైట్లు, intmax_t rem_బైట్లు,
             అక్షరం *buf, idx_t bufsize, ssize_t initial_read,
             intmax_t max_files)
{
  bool new_file_flag = true;
  bool filter_ok = true;
  intmax_t opened = 0;
  intmax_t గమ్యం_write = n_బైట్లు + (0 < rem_బైట్లు);
  bool eof = ! గమ్యం_write;

  అయితే (! eof)
    {
      ssize_t n_read;
      ఒకవేళ (0 <= initial_read)
        {
          n_read = initial_read;
          initial_read = -1;
          eof = n_read < bufsize;
        }
      లేకపోతే
        {
          ఒకవేళ (! filter_ok
              && 0 <= lseek (STDIN_FILENO, గమ్యం_write, SEEK_CUR))
            {
              గమ్యం_write = n_బైట్లు + (opened + 1 < rem_బైట్లు);
              new_file_flag = true;
            }

          n_read = పఠ (STDIN_FILENO, buf, bufsize);
          ఒకవేళ (n_read < 0)
            error (EXIT_FAILURE, errno, "%s", quotef (infile));
          eof = n_read == 0;
        }
      అక్షరం *bp_బహిర్గతం = buf;
      అయితే (0 < గమ్యం_write && గమ్యం_write <= n_read)
        {
          ఒకవేళ (filter_ok || new_file_flag)
            filter_ok = cwrite (new_file_flag, bp_బహిర్గతం, గమ్యం_write);
          opened += new_file_flag;
          new_file_flag = !max_files || (opened < max_files);
          ఒకవేళ (! filter_ok && ! new_file_flag)
            {
              /* If filters no longer accepting input, stop reading.  */
              n_read = 0;
              eof = true;
              ఆపు;
            }
          bp_బహిర్గతం += గమ్యం_write;
          n_read -= గమ్యం_write;
          గమ్యం_write = n_బైట్లు + (opened < rem_బైట్లు);
        }
      ఒకవేళ (0 < n_read)
        {
          ఒకవేళ (filter_ok || new_file_flag)
            filter_ok = cwrite (new_file_flag, bp_బహిర్గతం, n_read);
          opened += new_file_flag;
          new_file_flag = false;
          ఒకవేళ (! filter_ok && opened == max_files)
            {
              /* If filters no longer accepting input, stop reading.  */
              ఆపు;
            }
          గమ్యం_write -= n_read;
        }
    }

  /* Ensure NUMBER files are created, which truncates
     any existing files or notifies any consumers on fifos.
     FIXME: Should we do this before EXIT_FAILURE?  */
  అయితే (opened++ < max_files)
    cwrite (true, ఖాళీ, 0);
}

/* Split into pieces of exactly N_LINES lines.
   Use buffer BUF, whose size is BUFSIZE.  */

స్థిర శూన్య
పంక్తులు_విభజించు (intmax_t n_పంక్తులు, అక్షరం *buf, idx_t bufsize)
{
  ssize_t n_read;
  అక్షరం *bp, *bp_బహిర్గతం, *eob;
  bool new_file_flag = true;
  intmax_t n = 0;

  చేయి
    {
      n_read = పఠ (STDIN_FILENO, buf, bufsize);
      ఒకవేళ (n_read < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
      bp = bp_బహిర్గతం = buf;
      eob = bp + n_read;
      *eob = eolchar;
      అయితే (true)
        {
          bp = rawmemchr (bp, eolchar);
          ఒకవేళ (bp == eob)
            {
              ఒకవేళ (eob != bp_బహిర్గతం) /* do not write 0 bytes! */
                {
                  idx_t len = eob - bp_బహిర్గతం;
                  cwrite (new_file_flag, bp_బహిర్గతం, len);
                  new_file_flag = false;
                }
              ఆపు;
            }

          ++bp;
          ఒకవేళ (++n >= n_పంక్తులు)
            {
              cwrite (new_file_flag, bp_బహిర్గతం, bp - bp_బహిర్గతం);
              bp_బహిర్గతం = bp;
              new_file_flag = true;
              n = 0;
            }
        }
    }
  అయితే (n_read);
}

/* Split into pieces that are as large as possible while still not more
   than N_BYTES bytes, and are split on line boundaries except
   where lines longer than N_BYTES bytes occur. */

స్థిర శూన్య
పంక్తి_బైట్లు_విభజించు (intmax_t n_బైట్లు, అక్షరం *buf, idx_t bufsize)
{
  ssize_t n_read;
  intmax_t n_బహిర్గతం = 0;      /* for each split.  */
  idx_t n_hold = 0;
  అక్షరం *hold = ఖాళీ;        /* for lines > bufsize.  */
  idx_t hold_కొలత = 0;
  bool విభజించు_పంక్తి = false;  /* Whether a \n was output in a split.  */

  చేయి
    {
      n_read = పఠ (STDIN_FILENO, buf, bufsize);
      ఒకవేళ (n_read < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
      idx_t n_ఎడమ = n_read;
      అక్షరం *sob = buf;
      అయితే (n_ఎడమ)
        {
          idx_t విభజించు_rest = 0;
          అక్షరం *eoc = ఖాళీ;
          అక్షరం *eol;

          /* Determine End Of Chunk and/or End of Line,
             which are used below to select what to write or buffer.  */
          ఒకవేళ (n_బైట్లు - n_బహిర్గతం - n_hold <= n_ఎడమ)
            {
              /* Have enough for split.  */
              విభజించు_rest = n_బైట్లు - n_బహిర్గతం - n_hold;
              eoc = sob + విభజించు_rest - 1;
              eol = memrchr (sob, eolchar, విభజించు_rest);
            }
          లేకపోతే
            eol = memrchr (sob, eolchar, n_ఎడమ);

          /* Output hold space if possible.  */
          ఒకవేళ (n_hold && !(!eol && n_బహిర్గతం))
            {
              cwrite (n_బహిర్గతం == 0, hold, n_hold);
              n_బహిర్గతం += n_hold;
              n_hold = 0;
            }

          /* Output to eol if present.  */
          ఒకవేళ (eol)
            {
              విభజించు_పంక్తి = true;
              idx_t n_write = eol - sob + 1;
              cwrite (n_బహిర్గతం == 0, sob, n_write);
              n_బహిర్గతం += n_write;
              n_ఎడమ -= n_write;
              sob += n_write;
              ఒకవేళ (eoc)
                విభజించు_rest -= n_write;
            }

          /* Output to eoc or eob if possible.  */
          ఒకవేళ (n_ఎడమ && !విభజించు_పంక్తి)
            {
              idx_t n_write = eoc ? విభజించు_rest : n_ఎడమ;
              cwrite (n_బహిర్గతం == 0, sob, n_write);
              n_బహిర్గతం += n_write;
              n_ఎడమ -= n_write;
              sob += n_write;
              ఒకవేళ (eoc)
                విభజించు_rest -= n_write;
            }

          /* Update hold if needed.  */
          ఒకవేళ ((eoc && విభజించు_rest) || (!eoc && n_ఎడమ))
            {
              idx_t n_buf = eoc ? విభజించు_rest : n_ఎడమ;
              ఒకవేళ (hold_కొలత - n_hold < n_buf)
                hold = xpalloc (hold, &hold_కొలత, n_buf - (hold_కొలత - n_hold),
                                -1, పరిమాణం *hold);
              జ్ఞాపకం_నకలు (hold + n_hold, sob, n_buf);
              n_hold += n_buf;
              n_ఎడమ -= n_buf;
              sob += n_buf;
            }

          /* Reset for new split.  */
          ఒకవేళ (eoc)
            {
              n_బహిర్గతం = 0;
              విభజించు_పంక్తి = false;
            }
        }
    }
  అయితే (n_read);

  /* Handle no eol at end of file.  */
  ఒకవేళ (n_hold)
    cwrite (n_బహిర్గతం == 0, hold, n_hold);

  విడుదల (hold);
}

/* -n l/[K/]N: Write lines to files of approximately file size / N.
   The file is partitioned into file size / N sized portions, with the
   last assigned any excess.  If a line _starts_ within a partition
   it is written completely to the corresponding file.  Since lines
   are not split even if they overlap a partition, the files written
   can be larger or smaller than the partition size, and even empty
   if a line is so long as to completely overlap the partition.  */

స్థిర శూన్య
పంక్తులు_chunk_విభజించు (intmax_t k, intmax_t n, అక్షరం *buf, idx_t bufsize,
                   ssize_t initial_read, off_t file_కొలత)
{
  affirm (n && k <= n);

  intmax_t rem_బైట్లు = file_కొలత % n;
  off_t chunk_కొలత = file_కొలత / n;
  intmax_t chunk_no = 1;
  off_t chunk_చివర = chunk_కొలత + (0 < rem_బైట్లు);
  off_t n_written = 0;
  bool new_file_flag = true;
  bool chunk_truncated = false;

  ఒకవేళ (k > 1 && 0 < file_కొలత)
    {
      /* Start reading 1 byte before kth chunk of file.  */
      off_t ప్రారంభం = (k - 1) * chunk_కొలత + MIN (k - 1, rem_బైట్లు) - 1;
      ఒకవేళ (ప్రారంభం < initial_read)
        {
          memmove (buf, buf + ప్రారంభం, initial_read - ప్రారంభం);
          initial_read -= ప్రారంభం;
        }
      లేకపోతే
        {
          ఒకవేళ (initial_read < ప్రారంభం
              && lseek (STDIN_FILENO, ప్రారంభం - initial_read, SEEK_CUR) < 0)
            error (EXIT_FAILURE, errno, "%s", quotef (infile));
          initial_read = -1;
        }
      n_written = ప్రారంభం;
      chunk_no = k - 1;
      chunk_చివర = ప్రారంభం + 1;
    }

  అయితే (n_written < file_కొలత)
    {
      అక్షరం *bp = buf, *eob;
      ssize_t n_read;
      ఒకవేళ (0 <= initial_read)
        {
          n_read = initial_read;
          initial_read = -1;
        }
      లేకపోతే
        {
          n_read = పఠ (STDIN_FILENO, buf,
                         MIN (bufsize, file_కొలత - n_written));
          ఒకవేళ (n_read < 0)
            error (EXIT_FAILURE, errno, "%s", quotef (infile));
        }
      ఒకవేళ (n_read == 0)
        ఆపు; /* eof.  */
      chunk_truncated = false;
      eob = buf + n_read;

      అయితే (bp != eob)
        {
          idx_t గమ్యం_write;
          bool తదుపరి = false;

          /* Begin looking for '\n' at last byte of chunk.  */
          off_t skip = MIN (n_read, MAX (0, chunk_చివర - 1 - n_written));
          అక్షరం *bp_బహిర్గతం = memchr (bp + skip, eolchar, n_read - skip);
          ఒకవేళ (bp_బహిర్గతం)
            {
              bp_బహిర్గతం++;
              తదుపరి = true;
            }
          లేకపోతే
            bp_బహిర్గతం = eob;
          గమ్యం_write = bp_బహిర్గతం - bp;

          ఒకవేళ (k == chunk_no)
            {
              /* We don't use the stdout buffer here since we're writing
                 large chunks from an existing file, so it's more efficient
                 to write out directly.  */
              ఒకవేళ (నిండిన_write (STDOUT_FILENO, bp, గమ్యం_write) != గమ్యం_write)
                write_error ();
            }
          లేకపోతే ఒకవేళ (! k)
            cwrite (new_file_flag, bp, గమ్యం_write);
          n_written += గమ్యం_write;
          bp += గమ్యం_write;
          n_read -= గమ్యం_write;
          new_file_flag = తదుపరి;

          /* A line could have been so long that it skipped
             entire chunks. So create empty files in that case.  */
          అయితే (తదుపరి || chunk_చివర <= n_written)
            {
              ఒకవేళ (!తదుపరి && bp == eob)
                {
                  /* replenish buf, before going to next chunk.  */
                  chunk_truncated = true;
                  ఆపు;
                }
              ఒకవేళ (k == chunk_no)
                తిరిగి;
              chunk_చివర += chunk_కొలత + (chunk_no < rem_బైట్లు);
              chunk_no++;
              ఒకవేళ (chunk_చివర <= n_written)
                {
                  ఒకవేళ (! k)
                    cwrite (true, ఖాళీ, 0);
                }
              లేకపోతే
                తదుపరి = false;
            }
        }
    }

  ఒకవేళ (chunk_truncated)
    chunk_no++;

  /* Ensure NUMBER files are created, which truncates
     any existing files or notifies any consumers on fifos.
     FIXME: Should we do this before EXIT_FAILURE?  */
  ఒకవేళ (!k)
    అయితే (chunk_no++ <= n)
      cwrite (true, ఖాళీ, 0);
}

/* -n K/N: Extract Kth of N chunks.  */

స్థిర శూన్య
బైట్లు_chunk_వెలికితీయి (intmax_t k, intmax_t n, అక్షరం *buf, idx_t bufsize,
                     ssize_t initial_read, off_t file_కొలత)
{
  off_t ప్రారంభం;
  off_t చివర;

  affirm (0 < k && k <= n);

  ప్రారంభం = (k - 1) * (file_కొలత / n) + MIN (k - 1, file_కొలత % n);
  చివర = k == n ? file_కొలత : k * (file_కొలత / n) + MIN (k, file_కొలత % n);

  ఒకవేళ (ప్రారంభం < initial_read)
    {
      memmove (buf, buf + ప్రారంభం, initial_read - ప్రారంభం);
      initial_read -= ప్రారంభం;
    }
  లేకపోతే
    {
      ఒకవేళ (initial_read < ప్రారంభం
          && lseek (STDIN_FILENO, ప్రారంభం - initial_read, SEEK_CUR) < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
      initial_read = -1;
    }

  అయితే (ప్రారంభం < చివర)
    {
      ssize_t n_read;
      ఒకవేళ (0 <= initial_read)
        {
          n_read = initial_read;
          initial_read = -1;
        }
      లేకపోతే
        {
          n_read = పఠ (STDIN_FILENO, buf, bufsize);
          ఒకవేళ (n_read < 0)
            error (EXIT_FAILURE, errno, "%s", quotef (infile));
        }
      ఒకవేళ (n_read == 0)
        ఆపు; /* eof.  */
      n_read = MIN (n_read, చివర - ప్రారంభం);
      ఒకవేళ (నిండిన_write (STDOUT_FILENO, buf, n_read) != n_read
          && ! ignorable (errno))
        error (EXIT_FAILURE, errno, "%s", quotef ("-"));
      ప్రారంభం += n_read;
    }
}

రకనిర్వచనం నిర్మాణం యొక్క_info
{
  అక్షరం *యొక్క_name;
  పూర్ణాంకం ofd;
  FILE *ofile;
  pid_t opid;
} యొక్క_t;

గణన
{
  OFD_NEW = -1,
  OFD_APPEND = -2
};

/* Rotate file descriptors when we're writing to more output files than we
   have available file descriptors.
   Return whether we came under file resource pressure.
   If so, it's probably best to close each file when finished with it.  */

స్థిర bool
ofile_open (యొక్క_t *files, idx_t i_check, idx_t nfiles)
{
  bool file_హద్దు = false;

  ఒకవేళ (files[i_check].ofd <= OFD_NEW)
    {
      పూర్ణాంకం fd;
      idx_t i_reopen = i_check ? i_check - 1 : nfiles - 1;

      /* Another process could have opened a file in between the calls to
         close and open, so we should keep trying until open succeeds or
         we've closed all of our files.  */
      అయితే (true)
        {
          ఒకవేళ (files[i_check].ofd == OFD_NEW)
            fd = create (files[i_check].యొక్క_name);
          లేకపోతే /* OFD_APPEND  */
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
              fd = open (files[i_check].యొక్క_name,
                         O_WRONLY | O_ద్విమాన | O_APPEND | O_NONBLOCK);
            }

          ఒకవేళ (0 <= fd)
            ఆపు;

          ఒకవేళ (!(errno == EMFILE || errno == ENFILE))
            error (EXIT_FAILURE, errno, "%s", quotef (files[i_check].యొక్క_name));

          file_హద్దు = true;

          /* Search backwards for an open file to close.  */
          అయితే (files[i_reopen].ofd < 0)
            {
              i_reopen = i_reopen ? i_reopen - 1 : nfiles - 1;
              /* No more open files to close, exit with E[NM]FILE.  */
              ఒకవేళ (i_reopen == i_check)
                error (EXIT_FAILURE, errno, "%s",
                       quotef (files[i_check].యొక్క_name));
            }

          ఒకవేళ (fclose (files[i_reopen].ofile) != 0)
            error (EXIT_FAILURE, errno, "%s", quotef (files[i_reopen].యొక్క_name));
          files[i_reopen].ofile = ఖాళీ;
          files[i_reopen].ofd = OFD_APPEND;
        }

      files[i_check].ofd = fd;
      FILE *ofile = fdopen (fd, "a");
      ఒకవేళ (!ofile)
        error (EXIT_FAILURE, errno, "%s", quotef (files[i_check].యొక్క_name));
      files[i_check].ofile = ofile;
      files[i_check].opid = filter_pid;
      filter_pid = 0;
    }

  తిరిగి file_హద్దు;
}

/* -n r/[K/]N: Divide file into N chunks in round robin fashion.
   Use BUF of size BUFSIZE for the buffer, and if allocating storage
   put its address into *FILESP to pacify -fsanitize=leak.
   When K == 0, we try to keep the files open in parallel.
   If we run out of file resources, then we revert
   to opening and closing each file for each line.  */

స్థిర శూన్య
పంక్తులు_rr (intmax_t k, intmax_t n, అక్షరం *buf, idx_t bufsize, యొక్క_t **filesp)
{
  bool wrapped = false;
  bool wrote = false;
  bool file_హద్దు;
  idx_t i_file;
  యొక్క_t *files IF_LINT (= ఖాళీ);
  intmax_t పంక్తి_no;

  ఒకవేళ (k)
    పంక్తి_no = 1;
  లేకపోతే
    {
      ఒకవేళ (IDX_MAX < n)
        xalloc_die ();
      files = *filesp = xinmalloc (n, పరిమాణం *files);

      /* Generate output file names. */
      కొరకు (i_file = 0; i_file < n; i_file++)
        {
          తదుపరి_file_name ();
          files[i_file].యొక్క_name = xstrdup (outfile);
          files[i_file].ofd = OFD_NEW;
          files[i_file].ofile = ఖాళీ;
          files[i_file].opid = 0;
        }
      i_file = 0;
      file_హద్దు = false;
    }

  అయితే (true)
    {
      అక్షరం *bp = buf, *eob;
      ssize_t n_read = పఠ (STDIN_FILENO, buf, bufsize);
      ఒకవేళ (n_read < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
      లేకపోతే ఒకవేళ (n_read == 0)
        ఆపు; /* eof.  */
      eob = buf + n_read;

      అయితే (bp != eob)
        {
          idx_t గమ్యం_write;
          bool తదుపరి = false;

          /* Find end of line. */
          అక్షరం *bp_బహిర్గతం = memchr (bp, eolchar, eob - bp);
          ఒకవేళ (bp_బహిర్గతం)
            {
              bp_బహిర్గతం++;
              తదుపరి = true;
            }
          లేకపోతే
            bp_బహిర్గతం = eob;
          గమ్యం_write = bp_బహిర్గతం - bp;

          ఒకవేళ (k)
            {
              ఒకవేళ (పంక్తి_no == k && unbuffered)
                {
                  ఒకవేళ (నిండిన_write (STDOUT_FILENO, bp, గమ్యం_write) != గమ్యం_write)
                    write_error ();
                }
              లేకపోతే ఒకవేళ (పంక్తి_no == k && fwrite (bp, గమ్యం_write, 1, stdout) != 1)
                {
                  write_error ();
                }
              ఒకవేళ (తదుపరి)
                పంక్తి_no = (పంక్తి_no == n) ? 1 : పంక్తి_no + 1;
            }
          లేకపోతే
            {
              /* Secure file descriptor. */
              file_హద్దు |= ofile_open (files, i_file, n);
              ఒకవేళ (unbuffered)
                {
                  /* Note writing to fd, rather than flushing the FILE gives
                     an 8% performance benefit, due to reduced data copying.  */
                  ఒకవేళ (నిండిన_write (files[i_file].ofd, bp, గమ్యం_write) != గమ్యం_write
                      && ! ignorable (errno))
                    error (EXIT_FAILURE, errno, "%s",
                           quotef (files[i_file].యొక్క_name));
                }
              లేకపోతే ఒకవేళ (fwrite (bp, గమ్యం_write, 1, files[i_file].ofile) != 1
                       && ! ignorable (errno))
                error (EXIT_FAILURE, errno, "%s",
                       quotef (files[i_file].యొక్క_name));

              ఒకవేళ (! ignorable (errno))
                wrote = true;

              ఒకవేళ (file_హద్దు)
                {
                  ఒకవేళ (fclose (files[i_file].ofile) != 0)
                    error (EXIT_FAILURE, errno, "%s",
                           quotef (files[i_file].యొక్క_name));
                  files[i_file].ofile = ఖాళీ;
                  files[i_file].ofd = OFD_APPEND;
                }
              ఒకవేళ (తదుపరి && ++i_file == n)
                {
                  wrapped = true;
                  /* If no filters are accepting input, stop reading.  */
                  ఒకవేళ (! wrote)
                    వెళ్ళు no_filters;
                  wrote = false;
                  i_file = 0;
                }
            }

          bp = bp_బహిర్గతం;
        }
    }

no_filters:
  /* Ensure all files created, so that any existing files are truncated,
     and to signal any waiting fifo consumers.
     Also, close any open file descriptors.
     FIXME: Should we do this before EXIT_FAILURE?  */
  ఒకవేళ (!k)
    {
      idx_t ceiling = wrapped ? n : i_file;
      కొరకు (i_file = 0; i_file < n; i_file++)
        {
          ఒకవేళ (i_file >= ceiling && !elide_ఖాళీలు_files)
            file_హద్దు |= ofile_open (files, i_file, n);
          ఒకవేళ (files[i_file].ofd >= 0)
            closeout (files[i_file].ofile, files[i_file].ofd,
                      files[i_file].opid, files[i_file].యొక్క_name);
          files[i_file].ofd = OFD_APPEND;
        }
    }
}

#నిర్వచించు FAIL_ONLY_ONE_WAY()					\
  చేయి								\
    {								\
      error (0, 0, _("cannot split in more than one way"));	\
      usage (EXIT_FAILURE);					\
    }								\
  అయితే (0)

/* Report a string-to-integer conversion failure MSGID with ARG.  */

స్థిర _Noreturn శూన్య
strtoint_die (అక్షరం స్థిరరాశి *msgid, అక్షరం స్థిరరాశి *arg)
{
  error (EXIT_FAILURE, errno == EINVAL ? 0 : errno, "%s: %s",
         gettext (msgid), quote (arg));
}

/* Use OVERFLOW_OK when it is OK to ignore LONGINT_OVERFLOW errors, since the
   extreme value will do the right thing anyway on any practical platform.  */
#నిర్వచించు అధికప్రవాహం_OK LONGINT_అధికప్రవాహం

/* Parse ARG for number of bytes or lines.  The number can be followed
   by MULTIPLIERS, and the resulting value must be positive.
   If the number cannot be parsed, diagnose with MSG.
   Return the number parsed, or an INTMAX_MAX on overflow.  */

స్థిర intmax_t
parse_n_units (అక్షరం స్థిరరాశి *arg, అక్షరం స్థిరరాశి *multipliers, అక్షరం స్థిరరాశి *msgid)
{
  intmax_t n;
  ఒకవేళ (అధికప్రవాహం_OK < xstrtoimax (arg, ఖాళీ, 10, &n, multipliers) || n < 1)
    strtoint_die (msgid, arg);
  తిరిగి n;
}

/* Parse K/N syntax of chunk options.  */

స్థిర శూన్య
parse_chunk (intmax_t *k_units, intmax_t *n_units, అక్షరం స్థిరరాశి *arg)
{
  అక్షరం *argend;
  strtol_error e = xstrtoimax (arg, &argend, 10, n_units, "");
  ఒకవేళ (e == LONGINT_INVALID_SUFFIX_CHAR && *argend == '/')
    {
      *k_units = *n_units;
      *n_units = parse_n_units (argend + 1, "",
                                N_("invalid number of chunks"));
      ఒకవేళ (! (0 < *k_units && *k_units <= *n_units))
        error (EXIT_FAILURE, 0, "%s: %s", _("invalid chunk number"),
               quote_mem (arg, argend - arg));
    }
  లేకపోతే ఒకవేళ (! (e <= అధికప్రవాహం_OK && 0 < *n_units))
    strtoint_die (N_("invalid number of chunks"), arg);
}


పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  గణన విభజించు_type విభజించు_type_2 = type_undef;
  idx_t లోపల_blk_కొలత = 0;	/* optimal block size of input file device */
  idx_t పుట_కొలత = getpagesize ();
  intmax_t k_units = 0;
  intmax_t n_units = 0;

  స్థిర అక్షరం స్థిరరాశి multipliers[] = "bEGKkMmPQRTYZ0";
  పూర్ణాంకం c;
  పూర్ణాంకం digits_optind = 0;
  off_t file_కొలత = OFF_T_MAX;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* Parse command line options.  */

  అయితే (true)
    {
      /* This is the argv-index of the option we will read next.  */
      పూర్ణాంకం this_optind = optind ? optind : 1;

      c = getopt_long (argc, argv, "0123456789C:a:b:del:n:t:ux",
                       longopts, ఖాళీ);
      ఒకవేళ (c == -1)
        ఆపు;

      ఎంపిక (c)
        {
        స్థితి 'a':
          suffix_నిడివి = xdectoimax (optarg, 0, IDX_MAX,
                                      "", _("invalid suffix length"), 0);
          ఆపు;

        స్థితి ADDITIONAL_SUFFIX_OPTION:
          {
            పూర్ణాంకం suffix_len = అక్షరశ్రేణి_పొడవు (optarg);
            ఒకవేళ (చివరి_component (optarg) != optarg
                || (suffix_len && ISSLASH (optarg[suffix_len - 1])))
              {
                error (0, 0,
                       _("invalid suffix %s, contains directory separator"),
                       quote (optarg));
                usage (EXIT_FAILURE);
              }
          }
          additional_suffix = optarg;
          ఆపు;

        స్థితి 'b':
          ఒకవేళ (విభజించు_type_2 != type_undef)
            FAIL_ONLY_ONE_WAY ();
          విభజించు_type_2 = type_బైట్లు;
          n_units = parse_n_units (optarg, multipliers,
                                   N_("invalid number of bytes"));
          ఆపు;

        స్థితి 'l':
          ఒకవేళ (విభజించు_type_2 != type_undef)
            FAIL_ONLY_ONE_WAY ();
          విభజించు_type_2 = type_పంక్తులు;
          n_units = parse_n_units (optarg, "", N_("invalid number of lines"));
          ఆపు;

        స్థితి 'C':
          ఒకవేళ (విభజించు_type_2 != type_undef)
            FAIL_ONLY_ONE_WAY ();
          విభజించు_type_2 = type_byteslines;
          n_units = parse_n_units (optarg, multipliers,
                                   N_("invalid number of lines"));
          ఆపు;

        స్థితి 'n':
          ఒకవేళ (విభజించు_type_2 != type_undef)
            FAIL_ONLY_ONE_WAY ();
          /* skip any whitespace */
          అయితే (ఖాళీనా (గమ్యం_uchar (*optarg)))
            optarg++;
          ఒకవేళ (STRNCMP_LIT (optarg, "r/") == 0)
            {
              విభజించు_type_2 = type_rr;
              optarg += 2;
            }
          లేకపోతే ఒకవేళ (STRNCMP_LIT (optarg, "l/") == 0)
            {
              విభజించు_type_2 = type_chunk_పంక్తులు;
              optarg += 2;
            }
          లేకపోతే
            విభజించు_type_2 = type_chunk_బైట్లు;
          parse_chunk (&k_units, &n_units, optarg);
          ఆపు;

        స్థితి 'u':
          unbuffered = true;
          ఆపు;

        స్థితి 't':
          {
            అక్షరం neweol = optarg[0];
            ఒకవేళ (! neweol)
              error (EXIT_FAILURE, 0, _("empty record separator"));
            ఒకవేళ (optarg[1])
              {
                ఒకవేళ (streq (optarg, "\\0"))
                  neweol = '\0';
                లేకపోతే
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
            ఒకవేళ (0 <= eolchar && neweol != eolchar)
              {
                error (EXIT_FAILURE, 0,
                       _("multiple separator characters specified"));
              }

            eolchar = neweol;
          }
          ఆపు;

        స్థితి '0':
        స్థితి '1':
        స్థితి '2':
        స్థితి '3':
        స్థితి '4':
        స్థితి '5':
        స్థితి '6':
        స్థితి '7':
        స్థితి '8':
        స్థితి '9':
          ఒకవేళ (విభజించు_type_2 == type_undef)
            {
              విభజించు_type_2 = type_digits;
              n_units = 0;
            }
          ఒకవేళ (విభజించు_type_2 != type_undef && విభజించు_type_2 != type_digits)
            FAIL_ONLY_ONE_WAY ();
          ఒకవేళ (digits_optind != 0 && digits_optind != this_optind)
            n_units = 0;	/* More than one number given; ignore other. */
          digits_optind = this_optind;
          ఒకవేళ (ckd_mul (&n_units, n_units, 10)
              || ckd_కూడు (&n_units, n_units, c - '0'))
            n_units = INTMAX_MAX;
          ఆపు;

        స్థితి 'd':
        స్థితి 'x':
          ఒకవేళ (c == 'd')
            suffix_alphabet = "0123456789";
          లేకపోతే
            suffix_alphabet = "0123456789abcdef";
          ఒకవేళ (optarg)
            {
              ఒకవేళ (అక్షరశ్రేణి_పొడవు (optarg) != strspn (optarg, suffix_alphabet))
                {
                  error (0, 0,
                         (c == 'd') ?
                           _("%s: invalid start value for numerical suffix") :
                           _("%s: invalid start value for hexadecimal suffix"),
                         quote (optarg));
                  usage (EXIT_FAILURE);
                }
              లేకపోతే
                {
                  /* Skip any leading zero.  */
                  అయితే (*optarg == '0' && *(optarg + 1) != '\0')
                    optarg++;
                  numeric_suffix_ప్రారంభం = optarg;
                }
            }
          ఆపు;

        స్థితి 'e':
          elide_ఖాళీలు_files = true;
          ఆపు;

        స్థితి FILTER_OPTION:
          filter_command = optarg;
          ఆపు;

        స్థితి IO_BLKSIZE_OPTION:
          లోపల_blk_కొలత = xnumtoumax (optarg, 10, 1,
                                    MIN (SYS_BUFSIZE_MAX,
                                         MIN (IDX_MAX, SIZE_MAX) - 1),
                                    multipliers, _("invalid IO block size"),
                                    0, XTOINT_MIN_RANGE);
          ఆపు;

        స్థితి VERBOSE_OPTION:
          verbose = true;
          ఆపు;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);

        అప్రమేయ:
          usage (EXIT_FAILURE);
        }
    }

  ఒకవేళ (k_units != 0 && filter_command)
    {
      error (0, 0, _("--filter does not process a chunk extracted to "
                     "standard output"));
      usage (EXIT_FAILURE);
    }

  /* Handle default case.  */
  ఒకవేళ (విభజించు_type_2 == type_undef)
    {
      విభజించు_type_2 = type_పంక్తులు;
      n_units = 1000;
    }

  ఒకవేళ (n_units == 0)
    {
      error (0, 0, _("invalid number of lines: %s"), quote ("0"));
      usage (EXIT_FAILURE);
    }

  ఒకవేళ (eolchar < 0)
    eolchar = '\n';

  సమితి_suffix_నిడివి (n_units, విభజించు_type_2);

  /* Get out the filename arguments.  */

  ఒకవేళ (optind < argc)
    infile = argv[optind++];

  ఒకవేళ (optind < argc)
    outbase = argv[optind++];

  ఒకవేళ (optind < argc)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind]));
      usage (EXIT_FAILURE);
    }

  /* Check that the suffix length is large enough for the numerical
     suffix start value.  */
  ఒకవేళ (numeric_suffix_ప్రారంభం && అక్షరశ్రేణి_పొడవు (numeric_suffix_ప్రారంభం) > suffix_నిడివి)
    {
      error (0, 0, _("numerical suffix start value is too large "
                     "for the suffix length"));
      usage (EXIT_FAILURE);
    }

  /* Open the input file.  */
  ఒకవేళ (! streq (infile, "-")
      && fd_reopen (STDIN_FILENO, infile, O_RDONLY, 0) < 0)
    error (EXIT_FAILURE, errno, _("cannot open %s for reading"),
           quoteaf (infile));

  /* Binary I/O is safer when byte counts are used.  */
  xset_ద్విమాన_బహుళకం (STDIN_FILENO, O_ద్విమాన);

  /* Advise the kernel of our access pattern.  */
  fdadvise (STDIN_FILENO, 0, 0, FADVISE_SEQUENTIAL);

  /* Get the optimal block size of input device and make a buffer.  */

  ఒకవేళ (fstat (STDIN_FILENO, &లోపల_stat_buf) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (infile));

  ఒకవేళ (లోపల_blk_కొలత == 0)
    {
      లోపల_blk_కొలత = io_blksize (&లోపల_stat_buf);
      ఒకవేళ (SYS_BUFSIZE_MAX < లోపల_blk_కొలత)
        లోపల_blk_కొలత = SYS_BUFSIZE_MAX;
    }

  అక్షరం *buf = xalignalloc (పుట_కొలత, లోపల_blk_కొలత + 1);
  ssize_t initial_read = -1;

  ఒకవేళ (విభజించు_type_2 == type_chunk_బైట్లు || విభజించు_type_2 == type_chunk_పంక్తులు)
    {
      file_కొలత = ఆదానం_file_కొలత (STDIN_FILENO, &లోపల_stat_buf,
                                   buf, లోపల_blk_కొలత);
      ఒకవేళ (file_కొలత < 0)
        error (EXIT_FAILURE, errno, _("%s: cannot determine file size"),
               quotef (infile));
      initial_read = MIN (file_కొలత, లోపల_blk_కొలత);
    }

  /* When filtering, closure of one pipe must not terminate the process,
     as there may still be other streams expecting input from us.  */
  ఒకవేళ (filter_command)
    default_SIGPIPE = సంకేతం (SIGPIPE, SIG_IGN) == SIG_DFL;

  ఎంపిక (విభజించు_type_2)
    {
    స్థితి type_digits:
    స్థితి type_పంక్తులు:
      పంక్తులు_విభజించు (n_units, buf, లోపల_blk_కొలత);
      ఆపు;

    స్థితి type_బైట్లు:
      బైట్లు_విభజించు (n_units, 0, buf, లోపల_blk_కొలత, -1, 0);
      ఆపు;

    స్థితి type_byteslines:
      పంక్తి_బైట్లు_విభజించు (n_units, buf, లోపల_blk_కొలత);
      ఆపు;

    స్థితి type_chunk_బైట్లు:
      ఒకవేళ (k_units == 0)
        బైట్లు_విభజించు (file_కొలత / n_units, file_కొలత % n_units,
                     buf, లోపల_blk_కొలత, initial_read, n_units);
      లేకపోతే
        బైట్లు_chunk_వెలికితీయి (k_units, n_units, buf, లోపల_blk_కొలత, initial_read,
                             file_కొలత);
      ఆపు;

    స్థితి type_chunk_పంక్తులు:
      పంక్తులు_chunk_విభజించు (k_units, n_units, buf, లోపల_blk_కొలత, initial_read,
                         file_కొలత);
      ఆపు;

    స్థితి type_rr:
      /* Note, this is like 'sed -n ${k}~${n}p' when k > 0,
         but the functionality is provided for symmetry.  */
      {
        యొక్క_t *files;
        పంక్తులు_rr (k_units, n_units, buf, లోపల_blk_కొలత, &files);
      }
      ఆపు;

    స్థితి type_undef: అప్రమేయ:
      affirm (false);
    }

  ఒకవేళ (close (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (infile));
  closeout (ఖాళీ, output_desc, filter_pid, outfile);

  main_exit (EXIT_SUCCESS);
}
