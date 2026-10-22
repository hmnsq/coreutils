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
#ಅನ್ತರ್ಭಾವ <config.h>

#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <signal.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <sys/wait.h>
#ಅನ್ತರ್ಭಾವ <spawn.h>

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "alignalloc.h"
#ಅನ್ತರ್ಭಾವ "assure.h"
#ಅನ್ತರ್ಭಾವ "fadvise.h"
#ಅನ್ತರ್ಭಾವ "fd-reopen.h"
#ಅನ್ತರ್ಭಾವ "fcntl--.h"
#ಅನ್ತರ್ಭಾವ "full-write.h"
#ಅನ್ತರ್ಭಾವ "ioblksize.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "sig2str.h"
#ಅನ್ತರ್ಭಾವ "sys-limits.h"
#ಅನ್ತರ್ಭಾವ "temp-stream.h"
#ಅನ್ತರ್ಭಾವ "unistd--.h"
#ಅನ್ತರ್ಭಾವ "xbinary-io.h"
#ಅನ್ತರ್ಭಾವ "xdectoint.h"
#ಅನ್ತರ್ಭಾವ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "split"

#define AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("Richard M. Stallman")

/* Shell command to filter through, instead of creating files.  */
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *filter_command;

/* Process ID of the filter.  */
ಸ್ಥಿರ pid_t filter_pid;

ಸ್ಥಿರ ಸಂರಚನಾ
{
  /* The number of open pipes.  */
  ಪೂರ್ಣಾಙ್ಕ n_pipes;

  /* The number of allocated pipes.  */
  idx_t n_pipes_alloc;

  /* Array of open pipes.  */
  ಪೂರ್ಣಾಙ್ಕ *pipes COUNTED_BY (n_pipes);
} open_pipes;

/* Whether SIGPIPE has the default action, when --filter is used.  */
ಸ್ಥಿರ bool default_SIGPIPE;

/* Base name of output files.  */
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *outbase = "x";

/* Name of output files.  */
ಸ್ಥಿರ ವರ್ಣ *outfile;

/* Pointer to the end of the prefix in OUTFILE.
   Suffixes are inserted here.  */
ಸ್ಥಿರ ವರ್ಣ *outfile_mid;

/* Generate new suffix when suffixes are exhausted.  */
ಸ್ಥಿರ bool suffix_auto = true;

/* Length of OUTFILE's suffix.  */
ಸ್ಥಿರ idx_t suffix_length;

/* Alphabet of characters to use in suffix.  */
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *suffix_alphabet = "abcdefghijklmnopqrstuvwxyz";

/* Numerical suffix start value.  */
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *numeric_suffix_start;

/* Additional suffix to append to output file names.  */
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *additional_suffix;

/* Name of input file.  May be "-".  */
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *infile = "-";

/* stat buf for input file.  */
ಸ್ಥಿರ ಸಂರಚನಾ stat in_stat_buf;

/* Descriptor on which output file is open.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ output_desc = -1;

/* If true, print a diagnostic on standard error just before each
   output file is opened. */
ಸ್ಥಿರ bool verbose;

/* If true, don't generate zero length output files. */
ಸ್ಥಿರ bool elide_empty_files;

/* If true, in round robin mode, immediately copy
   input to output, which is much slower, so disabled by default.  */
ಸ್ಥಿರ bool unbuffered;

/* The character marking end of line.  Defaults to \n below.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ eolchar = -1;

/* The split mode to use.  */
ಗಣನಾ Split_type
{
  type_undef, type_bytes, type_byteslines, type_ಸಾಲುಗಳು, type_digits,
  type_chunk_bytes, type_chunk_ಸಾಲುಗಳು, type_rr
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ಗಣನಾ
{
  VERBOSE_OPTION = CHAR_MAX + 1,
  FILTER_OPTION,
  IO_BLKSIZE_OPTION,
  ADDITIONAL_SUFFIX_OPTION
};

ಸ್ಥಿರ ಸಂರಚನಾ option ಧ್ರುವ longopts[] =
{
  {"bytes", required_argument, ರಿಕ್ತ, 'b'},
  {"lines", required_argument, ರಿಕ್ತ, 'l'},
  {"line-bytes", required_argument, ರಿಕ್ತ, 'C'},
  {"number", required_argument, ರಿಕ್ತ, 'n'},
  {"elide-empty-files", no_argument, ರಿಕ್ತ, 'e'},
  {"unbuffered", no_argument, ರಿಕ್ತ, 'u'},
  {"suffix-length", required_argument, ರಿಕ್ತ, 'a'},
  {"additional-suffix", required_argument, ರಿಕ್ತ,
   ADDITIONAL_SUFFIX_OPTION},
  {"numeric-suffixes", optional_argument, ರಿಕ್ತ, 'd'},
  {"hex-suffixes", optional_argument, ರಿಕ್ತ, 'x'},
  {"filter", required_argument, ರಿಕ್ತ, FILTER_OPTION},
  {"verbose", no_argument, ರಿಕ್ತ, VERBOSE_OPTION},
  {"separator", required_argument, ರಿಕ್ತ, 't'},
  {"-io-blksize", required_argument, ರಿಕ್ತ,
   IO_BLKSIZE_OPTION}, /* do not document */
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ರಿಕ್ತ, 0, ರಿಕ್ತ, 0}
};

/* Return true if the errno value, ERR, is ignorable.  */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
ignorable (ಪೂರ್ಣಾಙ್ಕ err)
{
  ಪ್ರತ್ಯಾಗಮ filter_command && err == EPIPE;
}

ಸ್ಥಿರ ಶೂನ್ಯ
set_suffix_length (intmax_t n_units, ಗಣನಾ Split_type split_type)
{
#define DEFAULT_SUFFIX_LENGTH 2

  ಪೂರ್ಣಾಙ್ಕ suffix_length_needed = 0;

  /* The suffix auto length feature is incompatible with
     a user specified start value as the generated suffixes
     are not all consecutive.  */
  ಯದಿ (numeric_suffix_start)
    suffix_auto = false;

  /* Auto-calculate the suffix length if the number of files is given.  */
  ಯದಿ (split_type == type_chunk_bytes || split_type == type_chunk_ಸಾಲುಗಳು
      || split_type == type_rr)
    {
      intmax_t n_units_end = n_units - 1;
      ಯದಿ (numeric_suffix_start)
        {
          intmax_t n_start;
          strtol_error e = xstrtoimax (numeric_suffix_start, ರಿಕ್ತ, 10,
                                       &n_start, "");
          ಯದಿ (e == LONGINT_OK && n_start < n_units)
            {
              /* Restrict auto adjustment so we don't keep
                 incrementing a suffix size arbitrarily,
                 as that would break sort order for files
                 generated from multiple split runs.  */
              ಯದಿ (ckd_add (&n_units_end, n_units_end, n_start))
                n_units_end = INTMAX_MAX;
            }

        }
      idx_t alphabet_len = strlen (suffix_alphabet);
      ಕುರು
        suffix_length_needed++;
      ಯಾವತ್ (n_units_end /= alphabet_len);

      suffix_auto = false;
    }

  ಯದಿ (suffix_length)            /* set by user */
    {
      ಯದಿ (suffix_length < suffix_length_needed)
        error (EXIT_FAILURE, 0,
               _("the suffix length needs to be at least %d"),
               suffix_length_needed);
      suffix_auto = false;
      ಪ್ರತ್ಯಾಗಮ;
    }
  ಅನ್ಯಥಾ
    suffix_length = MAX (DEFAULT_SUFFIX_LENGTH, suffix_length_needed);
}

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
    {
      printf (_("\
Usage: %s [OPTION]... [FILE [PREFIX]]\n\
"),
              program_name);
      fputs (_("\
Output pieces of FILE to PREFIXaa, PREFIXab, ...;\n\
ಸ್ವಭಾವ size is 1000 ಸಾಲುಗಳು, and ಸ್ವಭಾವ PREFIX is 'x'.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oprintf (_("\
  -a, --suffix-length=N\n\
         generate suffixes of length N (ಸ್ವಭಾವ %d)\n\
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
  -C, --ಸಾಲು-bytes=SIZE\n\
         put at most SIZE bytes of records per output file\n\
"));
      oputs (_("\
  -d\n\
         use numeric suffixes starting at 0, not alphabetic\n\
"));
      oputs (_("\
      --numeric-suffixes[=FROM]\n\
         same as -d, but allow setting the start ಮೌಲ್ಯ\n\
"));
      oputs (_("\
  -x\n\
         use hex suffixes starting at 0, not alphabetic\n\
"));
      oputs (_("\
      --hex-suffixes[=FROM]\n\
         same as -x, but allow setting the start ಮೌಲ್ಯ\n\
"));
      oputs (_("\
  -e, --elide-empty-files\n\
         ಕುರು not generate empty output files with '-n'\n\
"));
      oputs (_("\
      --filter=COMMAND\n\
         ಲಿಖ to shell COMMAND; file name is $FILE\n\
"));
      oputs (_("\
  -l, --ಸಾಲುಗಳು=ಸಂಖ್ಯೆ_2\n\
         put ಸಂಖ್ಯೆ_2 ಸಾಲುಗಳು/records per output file\n\
"));
      oputs (_("\
  -n, --ಸಂಖ್ಯೆ=CHUNKS\n\
         generate CHUNKS output files; see explanation below\n\
"));
      oputs (_("\
  -t, --separator=SEP\n\
         use SEP instead of newline as the record separator;\n\
         '\\0' (zero) specifies the NUL character\n\
"));
      oputs (_("\
  -u, --unbuffered\n\
         immediately copy ಒಳಹರಿವು to output with '-n r/...'\n\
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
  N       split into N files based on size of ಒಳಹರಿವು\n\
  K/N     output Kth of N to standard output\n\
  l/N     split into N files without splitting ಸಾಲುಗಳು/records\n\
  l/K/N   output Kth of N to standard output without splitting ಸಾಲುಗಳು/records\n\
  r/N     like 'l' but use round robin distribution\n\
  r/K/N   likewise but only output Kth of N to standard output\n\
"), stdout);
      fputs (_("\n\
-n (except -nr) will buffer to $TMPDIR, defaulting to /tmp,\n\
ಯದಿ the ಒಳಹರಿವು size cannot easily be determined.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Copy the data in FD to a temporary file, then make that file FD.
   Use BUF, of size BUFSIZE, to copy.  Return the number of
   bytes copied, or -1 (setting errno) on error.  */
ಸ್ಥಿರ off_t
copy_to_tmpfile (ಪೂರ್ಣಾಙ್ಕ fd, ವರ್ಣ *buf, idx_t bufsize)
{
  FILE *tmp;
  ಯದಿ (!temp_stream (&tmp, ರಿಕ್ತ))
    ಪ್ರತ್ಯಾಗಮ -1;
  off_t copied = 0;
  off_t r;

  ಯಾವತ್ (0 < (r = ಪಠ (fd, buf, bufsize)))
    {
      ಯದಿ (fwrite (buf, 1, r, tmp) != r)
        ಪ್ರತ್ಯಾಗಮ -1;
      ಯದಿ (ckd_add (&copied, copied, r))
        {
          errno = EOVERFLOW;
          ಪ್ರತ್ಯಾಗಮ -1;
        }
    }

  ಯದಿ (r < 0)
    ಪ್ರತ್ಯಾಗಮ r;
  r = dup2 (fileno (tmp), fd);
  ಯದಿ (r < 0)
    ಪ್ರತ್ಯಾಗಮ r;
  ಯದಿ (fclose (tmp) < 0)
    ಪ್ರತ್ಯಾಗಮ -1;
  ಪ್ರತ್ಯಾಗಮ copied;
}

/* Return the number of bytes that can be read from FD with status ST.
   Store up to the first BUFSIZE bytes of the file's data into BUF,
   and advance the file position by the number of bytes read.  On
   input error, set errno and return -1.  */

ಸ್ಥಿರ off_t
ಒಳಹರಿವು_file_size (ಪೂರ್ಣಾಙ್ಕ fd, ಸಂರಚನಾ stat ಧ್ರುವ *st, ವರ್ಣ *buf, idx_t bufsize)
{
  off_t size = 0;
  ಕುರು
    {
      ssize_t n_read = ಪಠ (fd, buf + size, bufsize - size);
      ಯದಿ (n_read <= 0)
        ಪ್ರತ್ಯಾಗಮ n_read < 0 ? n_read : size;
      size += n_read;
    }
  ಯಾವತ್ (size < bufsize);

  off_t cur, end;
  ಯದಿ ((usable_st_size (st) && st->st_size < size)
      || (cur = lseek (fd, 0, SEEK_CUR)) < 0
      || cur < size /* E.g., /dev/zero on GNU/Linux.  */
      || (end = lseek (fd, 0, SEEK_END)) < 0)
    {
      ವರ್ಣ *tmpbuf = xmalloc (bufsize);
      end = copy_to_tmpfile (fd, tmpbuf, bufsize);
      ಮುಕ್ತ (tmpbuf);
      ಯದಿ (end < 0)
        ಪ್ರತ್ಯಾಗಮ end;
      cur = 0;
    }

  ಯದಿ (end == OFF_T_MAX /* E.g., /dev/zero on GNU/Hurd.  */
      || (cur < end && ckd_add (&size, size, end - cur)))
    {
      errno = EOVERFLOW;
      ಪ್ರತ್ಯಾಗಮ -1;
    }

  ಯದಿ (cur < end)
    {
      off_t r = lseek (fd, cur, SEEK_SET);
      ಯದಿ (r < 0)
        ಪ್ರತ್ಯಾಗಮ r;
    }

  ಪ್ರತ್ಯಾಗಮ size;
}

/* Compute the next sequential output file name and store it into the
   string 'outfile'.  */

ಸ್ಥಿರ ಶೂನ್ಯ
next_file_name (ಶೂನ್ಯ)
{
  /* Index in suffix_alphabet of each character in the suffix.  */
  ಸ್ಥಿರ idx_t *sufindex;
  ಸ್ಥಿರ idx_t outbase_length;
  ಸ್ಥಿರ idx_t outfile_length;
  ಸ್ಥಿರ idx_t addsuf_length;

  ಯದಿ (! outfile)
    {
      bool overflow, widen;

new_name:
      widen = !! outfile_length;

      ಯದಿ (! widen)
        {
          /* Allocate and initialize the first file name.  */

          outbase_length = strlen (outbase);
          addsuf_length = additional_suffix ? strlen (additional_suffix) : 0;
          overflow = ckd_add (&outfile_length, outbase_length + addsuf_length,
                              suffix_length);
        }
      ಅನ್ಯಥಾ
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
      ಯದಿ (overflow)
        xalloc_die ();
      outfile = xirealloc (outfile, outfile_size);

      ಯದಿ (! widen)
        memcpy (outfile, outbase, outbase_length);
      ಅನ್ಯಥಾ
        {
          /* Append the last alphabet character to the file name prefix.  */
          outfile[outbase_length] = suffix_alphabet[sufindex[0]];
          outbase_length++;
        }

      outfile_mid = outfile + outbase_length;
      memset (outfile_mid, suffix_alphabet[0], suffix_length);
      ಯದಿ (additional_suffix)
        memcpy (outfile_mid + suffix_length, additional_suffix, addsuf_length);
      outfile[outfile_length] = 0;

      ಮುಕ್ತ (sufindex);
      sufindex = xicalloc (suffix_length, ಪರಿಮಾಣ *sufindex);

      ಯದಿ (numeric_suffix_start)
        {
          affirm (! widen);

          /* Update the output file name.  */
          idx_t i = strlen (numeric_suffix_start);
          memcpy (outfile_mid + suffix_length - i, numeric_suffix_start, i);

          /* Update the suffix index.  */
          idx_t *sufindex_end = sufindex + suffix_length;
          ಯಾವತ್ (i-- != 0)
            *--sufindex_end = numeric_suffix_start[i] - '0';
        }

#ಯದಿ ! _POSIX_NO_TRUNC && HAVE_PATHCONF && defined _PC_NAME_MAX
      /* POSIX requires that if the output file name is too long for
         its directory, 'split' must fail without creating any files.
         This must be checked for explicitly on operating systems that
         silently truncate file names.  */
      {
        ವರ್ಣ *dir = dir_name (outfile);
        long name_max = pathconf (dir, _PC_NAME_MAX);
        ಯದಿ (0 <= name_max && name_max < base_len (last_component (outfile)))
          error (EXIT_FAILURE, ENAMETOOLONG, "%s", quotef (outfile));
        ಮುಕ್ತ (dir);
      }
#endif
    }
  ಅನ್ಯಥಾ
    {
      /* Increment the suffix in place, if possible.  */

      idx_t i = suffix_length;
      ಯಾವತ್ (i-- != 0)
        {
          sufindex[i]++;
          ಯದಿ (suffix_auto && i == 0 && ! suffix_alphabet[sufindex[0] + 1])
            ಗಚ್ಛ new_name;
          outfile_mid[i] = suffix_alphabet[sufindex[i]];
          ಯದಿ (outfile_mid[i])
            ಪ್ರತ್ಯಾಗಮ;
          sufindex[i] = 0;
          outfile_mid[i] = suffix_alphabet[sufindex[i]];
        }
      error (EXIT_FAILURE, 0, _("output file suffixes exhausted"));
    }
}

/* Create or truncate a file.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
create (ವರ್ಣ ಧ್ರುವ *name)
{
  ಯದಿ (!filter_command)
    {
      ಯದಿ (verbose)
        fprintf (stdout, _("creating file %s\n"), quoteaf (name));

      ಪೂರ್ಣಾಙ್ಕ oflags = O_WRONLY | O_CREAT | O_ದ್ವಿಮಾನ;
      ಪೂರ್ಣಾಙ್ಕ fd = open (name, oflags | O_EXCL, MODE_RW_UGO);
      ಯದಿ (0 <= fd || errno != EEXIST)
        ಪ್ರತ್ಯಾಗಮ fd;
      fd = open (name, oflags, MODE_RW_UGO);
      ಯದಿ (fd < 0)
        ಪ್ರತ್ಯಾಗಮ fd;
      ಸಂರಚನಾ stat out_stat_buf;
      ಯದಿ (fstat (fd, &out_stat_buf) != 0)
        error (EXIT_FAILURE, errno, _("failed to stat %s"), quoteaf (name));
      ಯದಿ (psame_inode (&in_stat_buf, &out_stat_buf))
        error (EXIT_FAILURE, 0, _("%s would overwrite input; aborting"),
               quoteaf (name));
      ಯದಿ (ftruncate (fd, 0) < 0
          && (S_ISREG (out_stat_buf.st_mode) || S_TYPEISSHM (&out_stat_buf)))
        error (EXIT_FAILURE, errno, _("%s: error truncating"), quotef (name));

      ಪ್ರತ್ಯಾಗಮ fd;
    }
  ಅನ್ಯಥಾ
    {
      ಯದಿ (setenv ("FILE", name, 1) != 0)
        error (EXIT_FAILURE, errno,
               _("failed to set FILE environment variable"));
      ಯದಿ (verbose)
        fprintf (stdout, _("executing with FILE=%s\n"), quotef (name));

      ಪೂರ್ಣಾಙ್ಕ result;
      ಪೂರ್ಣಾಙ್ಕ fd_pair[2];
      pid_t child_pid;

      posix_spawnattr_t attr;
      posix_spawn_file_actions_t actions;

      sigset_t set;
      sigemptyset (&set);
      ಯದಿ (default_SIGPIPE)
        sigaddset (&set, SIGPIPE);

      ಯದಿ (   (result = posix_spawnattr_init (&attr))
          || (result = posix_spawnattr_setflags (&attr,
                                                 (POSIX_SPAWN_USEVFORK
                                                  | POSIX_SPAWN_SETSIGDEF)))
          || (result = posix_spawnattr_setsigdefault (&attr, &set))
          || (result = posix_spawn_file_actions_init (&actions))
         )
        error (EXIT_FAILURE, result, _("posix_spawn initialization failed"));

      ಯದಿ (pipe (fd_pair) != 0)
        error (EXIT_FAILURE, errno, _("failed to create pipe"));

      /* We have to close any pipes that were opened during an
         earlier call, otherwise this process will be holding a
         write-pipe that will prevent the earlier process from
         reading an EOF on the corresponding read-pipe.  */
      ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < open_pipes.n_pipes; ++i)
        ಯದಿ ((result = posix_spawn_file_actions_addclose (&actions,
                                                         open_pipes.pipes[i])))
          ಭಙ್ಗ;

      ಯದಿ (   result
          || (result = posix_spawn_file_actions_addclose (&actions, fd_pair[1]))
          || (fd_pair[0] != STDIN_FILENO
              && (   (result = posix_spawn_file_actions_adddup2 (&actions,
                                                                 fd_pair[0],
                                                                 STDIN_FILENO))
                  || (result = posix_spawn_file_actions_addclose (&actions,
                                                                  fd_pair[0]))))
         )
        error (EXIT_FAILURE, result, _("posix_spawn setup failed"));


      ವರ್ಣ ಧ್ರುವ *shell_prog = getenv ("SHELL");
      ಯದಿ (shell_prog == ರಿಕ್ತ)
        shell_prog = "/bin/sh";
      ವರ್ಣ ಧ್ರುವ *ಧ್ರುವ argv[] = { last_component (shell_prog), "-c",
                                   filter_command, ರಿಕ್ತ };

      result = posix_spawn (&child_pid, shell_prog, &actions, &attr,
                            (ವರ್ಣ * ಧ್ರುವ *) argv, environ);
      ಯದಿ (result != 0)
        error (EXIT_FAILURE, errno, _("failed to run command: \"%s -c %s\""),
               shell_prog, filter_command);

      posix_spawnattr_destroy (&attr);
      posix_spawn_file_actions_destroy (&actions);

      ಯದಿ (close (fd_pair[0]) != 0)
        error (EXIT_FAILURE, errno, _("failed to close input pipe"));
      filter_pid = child_pid;
      ಯದಿ (open_pipes.n_pipes == open_pipes.n_pipes_alloc)
        open_pipes.pipes = xpalloc (open_pipes.pipes, &open_pipes.n_pipes_alloc,
                                    1, MIN (INT_MAX, IDX_MAX),
                                    ಪರಿಮಾಣ *open_pipes.pipes);
      idx_t ಧ್ರುವ pipe_idx = open_pipes.n_pipes++;
      open_pipes.pipes[pipe_idx] = fd_pair[1];
      ಪ್ರತ್ಯಾಗಮ fd_pair[1];
    }
}

/* Close the output file, and do any associated cleanup.
   If FP and FD are both specified, they refer to the same open file;
   in this case FP is closed, but FD is still used in cleanup.  */
ಸ್ಥಿರ ಶೂನ್ಯ
closeout (FILE *fp, ಪೂರ್ಣಾಙ್ಕ fd, pid_t pid, ವರ್ಣ ಧ್ರುವ *name)
{
  ಯದಿ (fp != ರಿಕ್ತ && fclose (fp) != 0 && ! ignorable (errno))
    error (EXIT_FAILURE, errno, "%s", quotef (name));
  ಯದಿ (fd >= 0)
    {
      ಯದಿ (fp == ರಿಕ್ತ && close (fd) < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (name));
      ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; j < open_pipes.n_pipes; ++j)
        {
          ಯದಿ (open_pipes.pipes[j] == fd)
            {
              idx_t ಧ್ರುವ last_idx = open_pipes.n_pipes - 1;
              open_pipes.pipes[j] = open_pipes.pipes[last_idx];
              open_pipes.n_pipes--;
              ಭಙ್ಗ;
            }
        }
    }
  ಯದಿ (pid > 0)
    {
      ಪೂರ್ಣಾಙ್ಕ wstatus;
      ಯದಿ (waitpid (pid, &wstatus, 0) < 0)
        error (EXIT_FAILURE, errno, _("waiting for child process"));
      ಅನ್ಯಥಾ ಯದಿ (WIFSIGNALED (wstatus))
        {
          ಪೂರ್ಣಾಙ್ಕ sig = WTERMSIG (wstatus);
          ಯದಿ (sig != SIGPIPE)
            {
              ವರ್ಣ signame[MAX (SIG2STR_MAX, INT_BUFSIZE_BOUND (ಪೂರ್ಣಾಙ್ಕ))];
              ಯದಿ (sig2str (sig, signame) != 0)
                sprintf (signame, "%d", sig);
              error (sig + 128, 0,
                     _("with FILE=%s, signal %s from command: %s"),
                     quotef (name), signame, filter_command);
            }
        }
      ಅನ್ಯಥಾ ಯದಿ (WIFEXITED (wstatus))
        {
          ಪೂರ್ಣಾಙ್ಕ ex = WEXITSTATUS (wstatus);
          ಯದಿ (ex != 0)
            error (ex, 0, _("with FILE=%s, exit %d from command: %s"),
                   quotef (name), ex, filter_command);
        }
      ಅನ್ಯಥಾ
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

ಸ್ಥಿರ bool
cwrite (bool new_file_flag, ವರ್ಣ ಧ್ರುವ *bp, idx_t bytes)
{
  ಯದಿ (new_file_flag)
    {
      ಯದಿ (!bp && bytes == 0 && elide_empty_files)
        ಪ್ರತ್ಯಾಗಮ true;
      closeout (ರಿಕ್ತ, output_desc, filter_pid, outfile);
      next_file_name ();
      output_desc = create (outfile);
      ಯದಿ (output_desc < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (outfile));
    }

  ಯದಿ (full_write (output_desc, bp, bytes) == bytes)
    ಪ್ರತ್ಯಾಗಮ true;
  ಅನ್ಯಥಾ
    {
      ಯದಿ (! ignorable (errno))
        error (EXIT_FAILURE, errno, "%s", quotef (outfile));
      ಪ್ರತ್ಯಾಗಮ false;
    }
}

/* Split into pieces of exactly N_BYTES bytes.
   However, the first REM_BYTES pieces should be 1 byte longer.
   Use buffer BUF, whose size is BUFSIZE.
   If INITIAL_READ is nonnegative,
   BUF contains the first INITIAL_READ input bytes.  */

ಸ್ಥಿರ ಶೂನ್ಯ
bytes_split (intmax_t n_bytes, intmax_t rem_bytes,
             ವರ್ಣ *buf, idx_t bufsize, ssize_t initial_read,
             intmax_t max_files)
{
  bool new_file_flag = true;
  bool filter_ok = true;
  intmax_t opened = 0;
  intmax_t to_write = n_bytes + (0 < rem_bytes);
  bool eof = ! to_write;

  ಯಾವತ್ (! eof)
    {
      ssize_t n_read;
      ಯದಿ (0 <= initial_read)
        {
          n_read = initial_read;
          initial_read = -1;
          eof = n_read < bufsize;
        }
      ಅನ್ಯಥಾ
        {
          ಯದಿ (! filter_ok
              && 0 <= lseek (STDIN_FILENO, to_write, SEEK_CUR))
            {
              to_write = n_bytes + (opened + 1 < rem_bytes);
              new_file_flag = true;
            }

          n_read = ಪಠ (STDIN_FILENO, buf, bufsize);
          ಯದಿ (n_read < 0)
            error (EXIT_FAILURE, errno, "%s", quotef (infile));
          eof = n_read == 0;
        }
      ವರ್ಣ *bp_out = buf;
      ಯಾವತ್ (0 < to_write && to_write <= n_read)
        {
          ಯದಿ (filter_ok || new_file_flag)
            filter_ok = cwrite (new_file_flag, bp_out, to_write);
          opened += new_file_flag;
          new_file_flag = !max_files || (opened < max_files);
          ಯದಿ (! filter_ok && ! new_file_flag)
            {
              /* If filters no longer accepting input, stop reading.  */
              n_read = 0;
              eof = true;
              ಭಙ್ಗ;
            }
          bp_out += to_write;
          n_read -= to_write;
          to_write = n_bytes + (opened < rem_bytes);
        }
      ಯದಿ (0 < n_read)
        {
          ಯದಿ (filter_ok || new_file_flag)
            filter_ok = cwrite (new_file_flag, bp_out, n_read);
          opened += new_file_flag;
          new_file_flag = false;
          ಯದಿ (! filter_ok && opened == max_files)
            {
              /* If filters no longer accepting input, stop reading.  */
              ಭಙ್ಗ;
            }
          to_write -= n_read;
        }
    }

  /* Ensure NUMBER files are created, which truncates
     any existing files or notifies any consumers on fifos.
     FIXME: Should we do this before EXIT_FAILURE?  */
  ಯಾವತ್ (opened++ < max_files)
    cwrite (true, ರಿಕ್ತ, 0);
}

/* Split into pieces of exactly N_LINES lines.
   Use buffer BUF, whose size is BUFSIZE.  */

ಸ್ಥಿರ ಶೂನ್ಯ
ಸಾಲುಗಳು_split (intmax_t n_ಸಾಲುಗಳು, ವರ್ಣ *buf, idx_t bufsize)
{
  ssize_t n_read;
  ವರ್ಣ *bp, *bp_out, *eob;
  bool new_file_flag = true;
  intmax_t n = 0;

  ಕುರು
    {
      n_read = ಪಠ (STDIN_FILENO, buf, bufsize);
      ಯದಿ (n_read < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
      bp = bp_out = buf;
      eob = bp + n_read;
      *eob = eolchar;
      ಯಾವತ್ (true)
        {
          bp = rawmemchr (bp, eolchar);
          ಯದಿ (bp == eob)
            {
              ಯದಿ (eob != bp_out) /* do not write 0 bytes! */
                {
                  idx_t len = eob - bp_out;
                  cwrite (new_file_flag, bp_out, len);
                  new_file_flag = false;
                }
              ಭಙ್ಗ;
            }

          ++bp;
          ಯದಿ (++n >= n_ಸಾಲುಗಳು)
            {
              cwrite (new_file_flag, bp_out, bp - bp_out);
              bp_out = bp;
              new_file_flag = true;
              n = 0;
            }
        }
    }
  ಯಾವತ್ (n_read);
}

/* Split into pieces that are as large as possible while still not more
   than N_BYTES bytes, and are split on line boundaries except
   where lines longer than N_BYTES bytes occur. */

ಸ್ಥಿರ ಶೂನ್ಯ
ಸಾಲು_bytes_split (intmax_t n_bytes, ವರ್ಣ *buf, idx_t bufsize)
{
  ssize_t n_read;
  intmax_t n_out = 0;      /* for each split.  */
  idx_t n_hold = 0;
  ವರ್ಣ *hold = ರಿಕ್ತ;        /* for lines > bufsize.  */
  idx_t hold_size = 0;
  bool split_ಸಾಲು = false;  /* Whether a \n was output in a split.  */

  ಕುರು
    {
      n_read = ಪಠ (STDIN_FILENO, buf, bufsize);
      ಯದಿ (n_read < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
      idx_t n_ಎಡ = n_read;
      ವರ್ಣ *sob = buf;
      ಯಾವತ್ (n_ಎಡ)
        {
          idx_t split_rest = 0;
          ವರ್ಣ *eoc = ರಿಕ್ತ;
          ವರ್ಣ *eol;

          /* Determine End Of Chunk and/or End of Line,
             which are used below to select what to write or buffer.  */
          ಯದಿ (n_bytes - n_out - n_hold <= n_ಎಡ)
            {
              /* Have enough for split.  */
              split_rest = n_bytes - n_out - n_hold;
              eoc = sob + split_rest - 1;
              eol = memrchr (sob, eolchar, split_rest);
            }
          ಅನ್ಯಥಾ
            eol = memrchr (sob, eolchar, n_ಎಡ);

          /* Output hold space if possible.  */
          ಯದಿ (n_hold && !(!eol && n_out))
            {
              cwrite (n_out == 0, hold, n_hold);
              n_out += n_hold;
              n_hold = 0;
            }

          /* Output to eol if present.  */
          ಯದಿ (eol)
            {
              split_ಸಾಲು = true;
              idx_t n_write = eol - sob + 1;
              cwrite (n_out == 0, sob, n_write);
              n_out += n_write;
              n_ಎಡ -= n_write;
              sob += n_write;
              ಯದಿ (eoc)
                split_rest -= n_write;
            }

          /* Output to eoc or eob if possible.  */
          ಯದಿ (n_ಎಡ && !split_ಸಾಲು)
            {
              idx_t n_write = eoc ? split_rest : n_ಎಡ;
              cwrite (n_out == 0, sob, n_write);
              n_out += n_write;
              n_ಎಡ -= n_write;
              sob += n_write;
              ಯದಿ (eoc)
                split_rest -= n_write;
            }

          /* Update hold if needed.  */
          ಯದಿ ((eoc && split_rest) || (!eoc && n_ಎಡ))
            {
              idx_t n_buf = eoc ? split_rest : n_ಎಡ;
              ಯದಿ (hold_size - n_hold < n_buf)
                hold = xpalloc (hold, &hold_size, n_buf - (hold_size - n_hold),
                                -1, ಪರಿಮಾಣ *hold);
              memcpy (hold + n_hold, sob, n_buf);
              n_hold += n_buf;
              n_ಎಡ -= n_buf;
              sob += n_buf;
            }

          /* Reset for new split.  */
          ಯದಿ (eoc)
            {
              n_out = 0;
              split_ಸಾಲು = false;
            }
        }
    }
  ಯಾವತ್ (n_read);

  /* Handle no eol at end of file.  */
  ಯದಿ (n_hold)
    cwrite (n_out == 0, hold, n_hold);

  ಮುಕ್ತ (hold);
}

/* -n l/[K/]N: Write lines to files of approximately file size / N.
   The file is partitioned into file size / N sized portions, with the
   last assigned any excess.  If a line _starts_ within a partition
   it is written completely to the corresponding file.  Since lines
   are not split even if they overlap a partition, the files written
   can be larger or smaller than the partition size, and even empty
   if a line is so long as to completely overlap the partition.  */

ಸ್ಥಿರ ಶೂನ್ಯ
ಸಾಲುಗಳು_chunk_split (intmax_t k, intmax_t n, ವರ್ಣ *buf, idx_t bufsize,
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

  ಯದಿ (k > 1 && 0 < file_size)
    {
      /* Start reading 1 byte before kth chunk of file.  */
      off_t start = (k - 1) * chunk_size + MIN (k - 1, rem_bytes) - 1;
      ಯದಿ (start < initial_read)
        {
          memmove (buf, buf + start, initial_read - start);
          initial_read -= start;
        }
      ಅನ್ಯಥಾ
        {
          ಯದಿ (initial_read < start
              && lseek (STDIN_FILENO, start - initial_read, SEEK_CUR) < 0)
            error (EXIT_FAILURE, errno, "%s", quotef (infile));
          initial_read = -1;
        }
      n_written = start;
      chunk_no = k - 1;
      chunk_end = start + 1;
    }

  ಯಾವತ್ (n_written < file_size)
    {
      ವರ್ಣ *bp = buf, *eob;
      ssize_t n_read;
      ಯದಿ (0 <= initial_read)
        {
          n_read = initial_read;
          initial_read = -1;
        }
      ಅನ್ಯಥಾ
        {
          n_read = ಪಠ (STDIN_FILENO, buf,
                         MIN (bufsize, file_size - n_written));
          ಯದಿ (n_read < 0)
            error (EXIT_FAILURE, errno, "%s", quotef (infile));
        }
      ಯದಿ (n_read == 0)
        ಭಙ್ಗ; /* eof.  */
      chunk_truncated = false;
      eob = buf + n_read;

      ಯಾವತ್ (bp != eob)
        {
          idx_t to_write;
          bool next = false;

          /* Begin looking for '\n' at last byte of chunk.  */
          off_t skip = MIN (n_read, MAX (0, chunk_end - 1 - n_written));
          ವರ್ಣ *bp_out = memchr (bp + skip, eolchar, n_read - skip);
          ಯದಿ (bp_out)
            {
              bp_out++;
              next = true;
            }
          ಅನ್ಯಥಾ
            bp_out = eob;
          to_write = bp_out - bp;

          ಯದಿ (k == chunk_no)
            {
              /* We don't use the stdout buffer here since we're writing
                 large chunks from an existing file, so it's more efficient
                 to write out directly.  */
              ಯದಿ (full_write (STDOUT_FILENO, bp, to_write) != to_write)
                write_error ();
            }
          ಅನ್ಯಥಾ ಯದಿ (! k)
            cwrite (new_file_flag, bp, to_write);
          n_written += to_write;
          bp += to_write;
          n_read -= to_write;
          new_file_flag = next;

          /* A line could have been so long that it skipped
             entire chunks. So create empty files in that case.  */
          ಯಾವತ್ (next || chunk_end <= n_written)
            {
              ಯದಿ (!next && bp == eob)
                {
                  /* replenish buf, before going to next chunk.  */
                  chunk_truncated = true;
                  ಭಙ್ಗ;
                }
              ಯದಿ (k == chunk_no)
                ಪ್ರತ್ಯಾಗಮ;
              chunk_end += chunk_size + (chunk_no < rem_bytes);
              chunk_no++;
              ಯದಿ (chunk_end <= n_written)
                {
                  ಯದಿ (! k)
                    cwrite (true, ರಿಕ್ತ, 0);
                }
              ಅನ್ಯಥಾ
                next = false;
            }
        }
    }

  ಯದಿ (chunk_truncated)
    chunk_no++;

  /* Ensure NUMBER files are created, which truncates
     any existing files or notifies any consumers on fifos.
     FIXME: Should we do this before EXIT_FAILURE?  */
  ಯದಿ (!k)
    ಯಾವತ್ (chunk_no++ <= n)
      cwrite (true, ರಿಕ್ತ, 0);
}

/* -n K/N: Extract Kth of N chunks.  */

ಸ್ಥಿರ ಶೂನ್ಯ
bytes_chunk_extract (intmax_t k, intmax_t n, ವರ್ಣ *buf, idx_t bufsize,
                     ssize_t initial_read, off_t file_size)
{
  off_t start;
  off_t end;

  affirm (0 < k && k <= n);

  start = (k - 1) * (file_size / n) + MIN (k - 1, file_size % n);
  end = k == n ? file_size : k * (file_size / n) + MIN (k, file_size % n);

  ಯದಿ (start < initial_read)
    {
      memmove (buf, buf + start, initial_read - start);
      initial_read -= start;
    }
  ಅನ್ಯಥಾ
    {
      ಯದಿ (initial_read < start
          && lseek (STDIN_FILENO, start - initial_read, SEEK_CUR) < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
      initial_read = -1;
    }

  ಯಾವತ್ (start < end)
    {
      ssize_t n_read;
      ಯದಿ (0 <= initial_read)
        {
          n_read = initial_read;
          initial_read = -1;
        }
      ಅನ್ಯಥಾ
        {
          n_read = ಪಠ (STDIN_FILENO, buf, bufsize);
          ಯದಿ (n_read < 0)
            error (EXIT_FAILURE, errno, "%s", quotef (infile));
        }
      ಯದಿ (n_read == 0)
        ಭಙ್ಗ; /* eof.  */
      n_read = MIN (n_read, end - start);
      ಯದಿ (full_write (STDOUT_FILENO, buf, n_read) != n_read
          && ! ignorable (errno))
        error (EXIT_FAILURE, errno, "%s", quotef ("-"));
      start += n_read;
    }
}

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ of_info
{
  ವರ್ಣ *of_name;
  ಪೂರ್ಣಾಙ್ಕ ofd;
  FILE *ofile;
  pid_t opid;
} of_t;

ಗಣನಾ
{
  OFD_NEW = -1,
  OFD_APPEND = -2
};

/* Rotate file descriptors when we're writing to more output files than we
   have available file descriptors.
   Return whether we came under file resource pressure.
   If so, it's probably best to close each file when finished with it.  */

ಸ್ಥಿರ bool
ofile_open (of_t *files, idx_t i_check, idx_t nfiles)
{
  bool file_limit = false;

  ಯದಿ (files[i_check].ofd <= OFD_NEW)
    {
      ಪೂರ್ಣಾಙ್ಕ fd;
      idx_t i_reopen = i_check ? i_check - 1 : nfiles - 1;

      /* Another process could have opened a file in between the calls to
         close and open, so we should keep trying until open succeeds or
         we've closed all of our files.  */
      ಯಾವತ್ (true)
        {
          ಯದಿ (files[i_check].ofd == OFD_NEW)
            fd = create (files[i_check].of_name);
          ಅನ್ಯಥಾ /* OFD_APPEND  */
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
                         O_WRONLY | O_ದ್ವಿಮಾನ | O_APPEND | O_NONBLOCK);
            }

          ಯದಿ (0 <= fd)
            ಭಙ್ಗ;

          ಯದಿ (!(errno == EMFILE || errno == ENFILE))
            error (EXIT_FAILURE, errno, "%s", quotef (files[i_check].of_name));

          file_limit = true;

          /* Search backwards for an open file to close.  */
          ಯಾವತ್ (files[i_reopen].ofd < 0)
            {
              i_reopen = i_reopen ? i_reopen - 1 : nfiles - 1;
              /* No more open files to close, exit with E[NM]FILE.  */
              ಯದಿ (i_reopen == i_check)
                error (EXIT_FAILURE, errno, "%s",
                       quotef (files[i_check].of_name));
            }

          ಯದಿ (fclose (files[i_reopen].ofile) != 0)
            error (EXIT_FAILURE, errno, "%s", quotef (files[i_reopen].of_name));
          files[i_reopen].ofile = ರಿಕ್ತ;
          files[i_reopen].ofd = OFD_APPEND;
        }

      files[i_check].ofd = fd;
      FILE *ofile = fdopen (fd, "a");
      ಯದಿ (!ofile)
        error (EXIT_FAILURE, errno, "%s", quotef (files[i_check].of_name));
      files[i_check].ofile = ofile;
      files[i_check].opid = filter_pid;
      filter_pid = 0;
    }

  ಪ್ರತ್ಯಾಗಮ file_limit;
}

/* -n r/[K/]N: Divide file into N chunks in round robin fashion.
   Use BUF of size BUFSIZE for the buffer, and if allocating storage
   put its address into *FILESP to pacify -fsanitize=leak.
   When K == 0, we try to keep the files open in parallel.
   If we run out of file resources, then we revert
   to opening and closing each file for each line.  */

ಸ್ಥಿರ ಶೂನ್ಯ
ಸಾಲುಗಳು_rr (intmax_t k, intmax_t n, ವರ್ಣ *buf, idx_t bufsize, of_t **filesp)
{
  bool wrapped = false;
  bool wrote = false;
  bool file_limit;
  idx_t i_file;
  of_t *files IF_LINT (= ರಿಕ್ತ);
  intmax_t ಸಾಲು_no;

  ಯದಿ (k)
    ಸಾಲು_no = 1;
  ಅನ್ಯಥಾ
    {
      ಯದಿ (IDX_MAX < n)
        xalloc_die ();
      files = *filesp = xinmalloc (n, ಪರಿಮಾಣ *files);

      /* Generate output file names. */
      ಪ್ರತಿ (i_file = 0; i_file < n; i_file++)
        {
          next_file_name ();
          files[i_file].of_name = xstrdup (outfile);
          files[i_file].ofd = OFD_NEW;
          files[i_file].ofile = ರಿಕ್ತ;
          files[i_file].opid = 0;
        }
      i_file = 0;
      file_limit = false;
    }

  ಯಾವತ್ (true)
    {
      ವರ್ಣ *bp = buf, *eob;
      ssize_t n_read = ಪಠ (STDIN_FILENO, buf, bufsize);
      ಯದಿ (n_read < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
      ಅನ್ಯಥಾ ಯದಿ (n_read == 0)
        ಭಙ್ಗ; /* eof.  */
      eob = buf + n_read;

      ಯಾವತ್ (bp != eob)
        {
          idx_t to_write;
          bool next = false;

          /* Find end of line. */
          ವರ್ಣ *bp_out = memchr (bp, eolchar, eob - bp);
          ಯದಿ (bp_out)
            {
              bp_out++;
              next = true;
            }
          ಅನ್ಯಥಾ
            bp_out = eob;
          to_write = bp_out - bp;

          ಯದಿ (k)
            {
              ಯದಿ (ಸಾಲು_no == k && unbuffered)
                {
                  ಯದಿ (full_write (STDOUT_FILENO, bp, to_write) != to_write)
                    write_error ();
                }
              ಅನ್ಯಥಾ ಯದಿ (ಸಾಲು_no == k && fwrite (bp, to_write, 1, stdout) != 1)
                {
                  write_error ();
                }
              ಯದಿ (next)
                ಸಾಲು_no = (ಸಾಲು_no == n) ? 1 : ಸಾಲು_no + 1;
            }
          ಅನ್ಯಥಾ
            {
              /* Secure file descriptor. */
              file_limit |= ofile_open (files, i_file, n);
              ಯದಿ (unbuffered)
                {
                  /* Note writing to fd, rather than flushing the FILE gives
                     an 8% performance benefit, due to reduced data copying.  */
                  ಯದಿ (full_write (files[i_file].ofd, bp, to_write) != to_write
                      && ! ignorable (errno))
                    error (EXIT_FAILURE, errno, "%s",
                           quotef (files[i_file].of_name));
                }
              ಅನ್ಯಥಾ ಯದಿ (fwrite (bp, to_write, 1, files[i_file].ofile) != 1
                       && ! ignorable (errno))
                error (EXIT_FAILURE, errno, "%s",
                       quotef (files[i_file].of_name));

              ಯದಿ (! ignorable (errno))
                wrote = true;

              ಯದಿ (file_limit)
                {
                  ಯದಿ (fclose (files[i_file].ofile) != 0)
                    error (EXIT_FAILURE, errno, "%s",
                           quotef (files[i_file].of_name));
                  files[i_file].ofile = ರಿಕ್ತ;
                  files[i_file].ofd = OFD_APPEND;
                }
              ಯದಿ (next && ++i_file == n)
                {
                  wrapped = true;
                  /* If no filters are accepting input, stop reading.  */
                  ಯದಿ (! wrote)
                    ಗಚ್ಛ no_filters;
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
  ಯದಿ (!k)
    {
      idx_t ceiling = wrapped ? n : i_file;
      ಪ್ರತಿ (i_file = 0; i_file < n; i_file++)
        {
          ಯದಿ (i_file >= ceiling && !elide_empty_files)
            file_limit |= ofile_open (files, i_file, n);
          ಯದಿ (files[i_file].ofd >= 0)
            closeout (files[i_file].ofile, files[i_file].ofd,
                      files[i_file].opid, files[i_file].of_name);
          files[i_file].ofd = OFD_APPEND;
        }
    }
}

#define FAIL_ONLY_ONE_WAY()					\
  ಕುರು								\
    {								\
      error (0, 0, _("cannot split in more than one way"));	\
      usage (EXIT_FAILURE);					\
    }								\
  ಯಾವತ್ (0)

/* Report a string-to-integer conversion failure MSGID with ARG.  */

ಸ್ಥಿರ _Noreturn ಶೂನ್ಯ
strtoint_die (ವರ್ಣ ಧ್ರುವ *msgid, ವರ್ಣ ಧ್ರುವ *arg)
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

ಸ್ಥಿರ intmax_t
parse_n_units (ವರ್ಣ ಧ್ರುವ *arg, ವರ್ಣ ಧ್ರುವ *multipliers, ವರ್ಣ ಧ್ರುವ *msgid)
{
  intmax_t n;
  ಯದಿ (OVERFLOW_OK < xstrtoimax (arg, ರಿಕ್ತ, 10, &n, multipliers) || n < 1)
    strtoint_die (msgid, arg);
  ಪ್ರತ್ಯಾಗಮ n;
}

/* Parse K/N syntax of chunk options.  */

ಸ್ಥಿರ ಶೂನ್ಯ
parse_chunk (intmax_t *k_units, intmax_t *n_units, ವರ್ಣ ಧ್ರುವ *arg)
{
  ವರ್ಣ *argend;
  strtol_error e = xstrtoimax (arg, &argend, 10, n_units, "");
  ಯದಿ (e == LONGINT_INVALID_SUFFIX_CHAR && *argend == '/')
    {
      *k_units = *n_units;
      *n_units = parse_n_units (argend + 1, "",
                                N_("invalid number of chunks"));
      ಯದಿ (! (0 < *k_units && *k_units <= *n_units))
        error (EXIT_FAILURE, 0, "%s: %s", _("invalid chunk number"),
               quote_mem (arg, argend - arg));
    }
  ಅನ್ಯಥಾ ಯದಿ (! (e <= OVERFLOW_OK && 0 < *n_units))
    strtoint_die (N_("invalid number of chunks"), arg);
}


ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  ಗಣನಾ Split_type split_type = type_undef;
  idx_t in_blk_size = 0;	/* optimal block size of input file device */
  idx_t page_size = getpagesize ();
  intmax_t k_units = 0;
  intmax_t n_units = 0;

  ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ multipliers[] = "bEGKkMmPQRTYZ0";
  ಪೂರ್ಣಾಙ್ಕ c;
  ಪೂರ್ಣಾಙ್ಕ digits_optind = 0;
  off_t file_size = OFF_T_MAX;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* Parse command line options.  */

  ಯಾವತ್ (true)
    {
      /* This is the argv-index of the option we will read next.  */
      ಪೂರ್ಣಾಙ್ಕ this_optind = optind ? optind : 1;

      c = getopt_long (argc, argv, "0123456789C:a:b:del:n:t:ux",
                       longopts, ರಿಕ್ತ);
      ಯದಿ (c == -1)
        ಭಙ್ಗ;

      ವಿಕಲ್ಪ (c)
        {
        ಪಕ್ಷ 'a':
          suffix_length = xdectoimax (optarg, 0, IDX_MAX,
                                      "", _("invalid suffix length"), 0);
          ಭಙ್ಗ;

        ಪಕ್ಷ ADDITIONAL_SUFFIX_OPTION:
          {
            ಪೂರ್ಣಾಙ್ಕ suffix_len = strlen (optarg);
            ಯದಿ (last_component (optarg) != optarg
                || (suffix_len && ISSLASH (optarg[suffix_len - 1])))
              {
                error (0, 0,
                       _("invalid suffix %s, contains directory separator"),
                       quote (optarg));
                usage (EXIT_FAILURE);
              }
          }
          additional_suffix = optarg;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'b':
          ಯದಿ (split_type != type_undef)
            FAIL_ONLY_ONE_WAY ();
          split_type = type_bytes;
          n_units = parse_n_units (optarg, multipliers,
                                   N_("invalid number of bytes"));
          ಭಙ್ಗ;

        ಪಕ್ಷ 'l':
          ಯದಿ (split_type != type_undef)
            FAIL_ONLY_ONE_WAY ();
          split_type = type_ಸಾಲುಗಳು;
          n_units = parse_n_units (optarg, "", N_("invalid number of lines"));
          ಭಙ್ಗ;

        ಪಕ್ಷ 'C':
          ಯದಿ (split_type != type_undef)
            FAIL_ONLY_ONE_WAY ();
          split_type = type_byteslines;
          n_units = parse_n_units (optarg, multipliers,
                                   N_("invalid number of lines"));
          ಭಙ್ಗ;

        ಪಕ್ಷ 'n':
          ಯದಿ (split_type != type_undef)
            FAIL_ONLY_ONE_WAY ();
          /* skip any whitespace */
          ಯಾವತ್ (isspace (to_uchar (*optarg)))
            optarg++;
          ಯದಿ (STRNCMP_LIT (optarg, "r/") == 0)
            {
              split_type = type_rr;
              optarg += 2;
            }
          ಅನ್ಯಥಾ ಯದಿ (STRNCMP_LIT (optarg, "l/") == 0)
            {
              split_type = type_chunk_ಸಾಲುಗಳು;
              optarg += 2;
            }
          ಅನ್ಯಥಾ
            split_type = type_chunk_bytes;
          parse_chunk (&k_units, &n_units, optarg);
          ಭಙ್ಗ;

        ಪಕ್ಷ 'u':
          unbuffered = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 't':
          {
            ವರ್ಣ neweol = optarg[0];
            ಯದಿ (! neweol)
              error (EXIT_FAILURE, 0, _("empty record separator"));
            ಯದಿ (optarg[1])
              {
                ಯದಿ (streq (optarg, "\\0"))
                  neweol = '\0';
                ಅನ್ಯಥಾ
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
            ಯದಿ (0 <= eolchar && neweol != eolchar)
              {
                error (EXIT_FAILURE, 0,
                       _("multiple separator characters specified"));
              }

            eolchar = neweol;
          }
          ಭಙ್ಗ;

        ಪಕ್ಷ '0':
        ಪಕ್ಷ '1':
        ಪಕ್ಷ '2':
        ಪಕ್ಷ '3':
        ಪಕ್ಷ '4':
        ಪಕ್ಷ '5':
        ಪಕ್ಷ '6':
        ಪಕ್ಷ '7':
        ಪಕ್ಷ '8':
        ಪಕ್ಷ '9':
          ಯದಿ (split_type == type_undef)
            {
              split_type = type_digits;
              n_units = 0;
            }
          ಯದಿ (split_type != type_undef && split_type != type_digits)
            FAIL_ONLY_ONE_WAY ();
          ಯದಿ (digits_optind != 0 && digits_optind != this_optind)
            n_units = 0;	/* More than one number given; ignore other. */
          digits_optind = this_optind;
          ಯದಿ (ckd_mul (&n_units, n_units, 10)
              || ckd_add (&n_units, n_units, c - '0'))
            n_units = INTMAX_MAX;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'd':
        ಪಕ್ಷ 'x':
          ಯದಿ (c == 'd')
            suffix_alphabet = "0123456789";
          ಅನ್ಯಥಾ
            suffix_alphabet = "0123456789abcdef";
          ಯದಿ (optarg)
            {
              ಯದಿ (strlen (optarg) != strspn (optarg, suffix_alphabet))
                {
                  error (0, 0,
                         (c == 'd') ?
                           _("%s: invalid start value for numerical suffix") :
                           _("%s: invalid start value for hexadecimal suffix"),
                         quote (optarg));
                  usage (EXIT_FAILURE);
                }
              ಅನ್ಯಥಾ
                {
                  /* Skip any leading zero.  */
                  ಯಾವತ್ (*optarg == '0' && *(optarg + 1) != '\0')
                    optarg++;
                  numeric_suffix_start = optarg;
                }
            }
          ಭಙ್ಗ;

        ಪಕ್ಷ 'e':
          elide_empty_files = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ FILTER_OPTION:
          filter_command = optarg;
          ಭಙ್ಗ;

        ಪಕ್ಷ IO_BLKSIZE_OPTION:
          in_blk_size = xnumtoumax (optarg, 10, 1,
                                    MIN (SYS_BUFSIZE_MAX,
                                         MIN (IDX_MAX, SIZE_MAX) - 1),
                                    multipliers, _("invalid IO block size"),
                                    0, XTOINT_MIN_RANGE);
          ಭಙ್ಗ;

        ಪಕ್ಷ VERBOSE_OPTION:
          verbose = true;
          ಭಙ್ಗ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        ಸ್ವಭಾವ:
          usage (EXIT_FAILURE);
        }
    }

  ಯದಿ (k_units != 0 && filter_command)
    {
      error (0, 0, _("--filter does not process a chunk extracted to "
                     "standard output"));
      usage (EXIT_FAILURE);
    }

  /* Handle default case.  */
  ಯದಿ (split_type == type_undef)
    {
      split_type = type_ಸಾಲುಗಳು;
      n_units = 1000;
    }

  ಯದಿ (n_units == 0)
    {
      error (0, 0, _("invalid number of lines: %s"), quote ("0"));
      usage (EXIT_FAILURE);
    }

  ಯದಿ (eolchar < 0)
    eolchar = '\n';

  set_suffix_length (n_units, split_type);

  /* Get out the filename arguments.  */

  ಯದಿ (optind < argc)
    infile = argv[optind++];

  ಯದಿ (optind < argc)
    outbase = argv[optind++];

  ಯದಿ (optind < argc)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind]));
      usage (EXIT_FAILURE);
    }

  /* Check that the suffix length is large enough for the numerical
     suffix start value.  */
  ಯದಿ (numeric_suffix_start && strlen (numeric_suffix_start) > suffix_length)
    {
      error (0, 0, _("numerical suffix start value is too large "
                     "for the suffix length"));
      usage (EXIT_FAILURE);
    }

  /* Open the input file.  */
  ಯದಿ (! streq (infile, "-")
      && fd_reopen (STDIN_FILENO, infile, O_RDONLY, 0) < 0)
    error (EXIT_FAILURE, errno, _("cannot open %s for reading"),
           quoteaf (infile));

  /* Binary I/O is safer when byte counts are used.  */
  xset_ದ್ವಿಮಾನ_mode (STDIN_FILENO, O_ದ್ವಿಮಾನ);

  /* Advise the kernel of our access pattern.  */
  fdadvise (STDIN_FILENO, 0, 0, FADVISE_SEQUENTIAL);

  /* Get the optimal block size of input device and make a buffer.  */

  ಯದಿ (fstat (STDIN_FILENO, &in_stat_buf) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (infile));

  ಯದಿ (in_blk_size == 0)
    {
      in_blk_size = io_blksize (&in_stat_buf);
      ಯದಿ (SYS_BUFSIZE_MAX < in_blk_size)
        in_blk_size = SYS_BUFSIZE_MAX;
    }

  ವರ್ಣ *buf = xalignalloc (page_size, in_blk_size + 1);
  ssize_t initial_read = -1;

  ಯದಿ (split_type == type_chunk_bytes || split_type == type_chunk_ಸಾಲುಗಳು)
    {
      file_size = ಒಳಹರಿವು_file_size (STDIN_FILENO, &in_stat_buf,
                                   buf, in_blk_size);
      ಯದಿ (file_size < 0)
        error (EXIT_FAILURE, errno, _("%s: cannot determine file size"),
               quotef (infile));
      initial_read = MIN (file_size, in_blk_size);
    }

  /* When filtering, closure of one pipe must not terminate the process,
     as there may still be other streams expecting input from us.  */
  ಯದಿ (filter_command)
    default_SIGPIPE = ಸಙ್ಕೇತ (SIGPIPE, SIG_IGN) == SIG_DFL;

  ವಿಕಲ್ಪ (split_type)
    {
    ಪಕ್ಷ type_digits:
    ಪಕ್ಷ type_ಸಾಲುಗಳು:
      ಸಾಲುಗಳು_split (n_units, buf, in_blk_size);
      ಭಙ್ಗ;

    ಪಕ್ಷ type_bytes:
      bytes_split (n_units, 0, buf, in_blk_size, -1, 0);
      ಭಙ್ಗ;

    ಪಕ್ಷ type_byteslines:
      ಸಾಲು_bytes_split (n_units, buf, in_blk_size);
      ಭಙ್ಗ;

    ಪಕ್ಷ type_chunk_bytes:
      ಯದಿ (k_units == 0)
        bytes_split (file_size / n_units, file_size % n_units,
                     buf, in_blk_size, initial_read, n_units);
      ಅನ್ಯಥಾ
        bytes_chunk_extract (k_units, n_units, buf, in_blk_size, initial_read,
                             file_size);
      ಭಙ್ಗ;

    ಪಕ್ಷ type_chunk_ಸಾಲುಗಳು:
      ಸಾಲುಗಳು_chunk_split (k_units, n_units, buf, in_blk_size, initial_read,
                         file_size);
      ಭಙ್ಗ;

    ಪಕ್ಷ type_rr:
      /* Note, this is like 'sed -n ${k}~${n}p' when k > 0,
         but the functionality is provided for symmetry.  */
      {
        of_t *files;
        ಸಾಲುಗಳು_rr (k_units, n_units, buf, in_blk_size, &files);
      }
      ಭಙ್ಗ;

    ಪಕ್ಷ type_undef: ಸ್ವಭಾವ:
      affirm (false);
    }

  ಯದಿ (close (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (infile));
  closeout (ರಿಕ್ತ, output_desc, filter_pid, outfile);

  main_exit (EXIT_SUCCESS);
}
