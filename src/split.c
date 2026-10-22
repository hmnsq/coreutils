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
#ଅନ୍ତର୍ଭାଵ <config.h>

#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <getopt.h>
#ଅନ୍ତର୍ଭାଵ <signal.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <sys/wait.h>
#ଅନ୍ତର୍ଭାଵ <spawn.h>

#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "alignalloc.h"
#ଅନ୍ତର୍ଭାଵ "assure.h"
#ଅନ୍ତର୍ଭାଵ "fadvise.h"
#ଅନ୍ତର୍ଭାଵ "fd-reopen.h"
#ଅନ୍ତର୍ଭାଵ "fcntl--.h"
#ଅନ୍ତର୍ଭାଵ "full-write.h"
#ଅନ୍ତର୍ଭାଵ "ioblksize.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "sig2str.h"
#ଅନ୍ତର୍ଭାଵ "sys-limits.h"
#ଅନ୍ତର୍ଭାଵ "temp-stream.h"
#ଅନ୍ତର୍ଭାଵ "unistd--.h"
#ଅନ୍ତର୍ଭାଵ "xbinary-io.h"
#ଅନ୍ତର୍ଭାଵ "xdectoint.h"
#ଅନ୍ତର୍ଭାଵ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "split"

#define AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("Richard M. Stallman")

/* Shell command to filter through, instead of creating files.  */
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *filter_command;

/* Process ID of the filter.  */
ସ୍ଥିର pid_t filter_pid;

ସ୍ଥିର ସଂରଚନା
{
  /* The number of open pipes.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_pipes;

  /* The number of allocated pipes.  */
  idx_t n_pipes_alloc;

  /* Array of open pipes.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା *pipes COUNTED_BY (n_pipes);
} open_pipes;

/* Whether SIGPIPE has the default action, when --filter is used.  */
ସ୍ଥିର bool default_SIGPIPE;

/* Base name of output files.  */
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *outbase = "x";

/* Name of output files.  */
ସ୍ଥିର ଅକ୍ଷର *outfile;

/* Pointer to the end of the prefix in OUTFILE.
   Suffixes are inserted here.  */
ସ୍ଥିର ଅକ୍ଷର *outfile_mid;

/* Generate new suffix when suffixes are exhausted.  */
ସ୍ଥିର bool suffix_auto = true;

/* Length of OUTFILE's suffix.  */
ସ୍ଥିର idx_t suffix_length;

/* Alphabet of characters to use in suffix.  */
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *suffix_alphabet = "abcdefghijklmnopqrstuvwxyz";

/* Numerical suffix start value.  */
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *numeric_suffix_start;

/* Additional suffix to append to output file names.  */
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *additional_suffix;

/* Name of input file.  May be "-".  */
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *infile = "-";

/* stat buf for input file.  */
ସ୍ଥିର ସଂରଚନା stat in_stat_buf;

/* Descriptor on which output file is open.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା output_desc = -1;

/* If true, print a diagnostic on standard error just before each
   output file is opened. */
ସ୍ଥିର bool verbose;

/* If true, don't generate zero length output files. */
ସ୍ଥିର bool elide_empty_files;

/* If true, in round robin mode, immediately copy
   input to output, which is much slower, so disabled by default.  */
ସ୍ଥିର bool unbuffered;

/* The character marking end of line.  Defaults to \n below.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା eolchar = -1;

/* The split mode to use.  */
ଗଣନା Split_type
{
  type_undef, type_bytes, type_byteslines, type_ଧାଡ଼ିଗୁଡ଼ିକ, type_digits,
  type_chunk_bytes, type_chunk_ଧାଡ଼ିଗୁଡ଼ିକ, type_rr
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ଗଣନା
{
  VERBOSE_OPTION = CHAR_MAX + 1,
  FILTER_OPTION,
  IO_BLKSIZE_OPTION,
  ADDITIONAL_SUFFIX_OPTION
};

ସ୍ଥିର ସଂରଚନା option ଅଚଳ longopts[] =
{
  {"bytes", required_argument, ଖାଲି, 'b'},
  {"lines", required_argument, ଖାଲି, 'l'},
  {"line-bytes", required_argument, ଖାଲି, 'C'},
  {"number", required_argument, ଖାଲି, 'n'},
  {"elide-empty-files", no_argument, ଖାଲି, 'e'},
  {"unbuffered", no_argument, ଖାଲି, 'u'},
  {"suffix-length", required_argument, ଖାଲି, 'a'},
  {"additional-suffix", required_argument, ଖାଲି,
   ADDITIONAL_SUFFIX_OPTION},
  {"numeric-suffixes", optional_argument, ଖାଲି, 'd'},
  {"hex-suffixes", optional_argument, ଖାଲି, 'x'},
  {"filter", required_argument, ଖାଲି, FILTER_OPTION},
  {"verbose", no_argument, ଖାଲି, VERBOSE_OPTION},
  {"separator", required_argument, ଖାଲି, 't'},
  {"-io-blksize", required_argument, ଖାଲି,
   IO_BLKSIZE_OPTION}, /* do not document */
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ଖାଲି, 0, ଖାଲି, 0}
};

/* Return true if the errno value, ERR, is ignorable.  */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
ignorable (ପୂର୍ଣ୍ଣସଂଖ୍ୟା err)
{
  ଫେରନ୍ତୁ filter_command && err == EPIPE;
}

ସ୍ଥିର ଶୂନ୍ୟ
set_suffix_length (intmax_t n_units, ଗଣନା Split_type split_type)
{
#define DEFAULT_SUFFIX_LENGTH 2

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା suffix_length_needed = 0;

  /* The suffix auto length feature is incompatible with
     a user specified start value as the generated suffixes
     are not all consecutive.  */
  ଯଦି (numeric_suffix_start)
    suffix_auto = false;

  /* Auto-calculate the suffix length if the number of files is given.  */
  ଯଦି (split_type == type_chunk_bytes || split_type == type_chunk_ଧାଡ଼ିଗୁଡ଼ିକ
      || split_type == type_rr)
    {
      intmax_t n_units_end = n_units - 1;
      ଯଦି (numeric_suffix_start)
        {
          intmax_t n_start;
          strtol_error e = xstrtoimax (numeric_suffix_start, ଖାଲି, 10,
                                       &n_start, "");
          ଯଦି (e == LONGINT_OK && n_start < n_units)
            {
              /* Restrict auto adjustment so we don't keep
                 incrementing a suffix size arbitrarily,
                 as that would break sort order for files
                 generated from multiple split runs.  */
              ଯଦି (ckd_add (&n_units_end, n_units_end, n_start))
                n_units_end = INTMAX_MAX;
            }

        }
      idx_t alphabet_len = strlen (suffix_alphabet);
      କର
        suffix_length_needed++;
      ଯେତେବେଳେ (n_units_end /= alphabet_len);

      suffix_auto = false;
    }

  ଯଦି (suffix_length)            /* set by user */
    {
      ଯଦି (suffix_length < suffix_length_needed)
        error (EXIT_FAILURE, 0,
               _("the suffix length needs to be at least %d"),
               suffix_length_needed);
      suffix_auto = false;
      ଫେରନ୍ତୁ;
    }
  ଅନ୍ୟଥା
    suffix_length = MAX (DEFAULT_SUFFIX_LENGTH, suffix_length_needed);
}

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
    {
      printf (_("\
Usage: %s [OPTION]... [FILE [PREFIX]]\n\
"),
              program_name);
      fputs (_("\
Output pieces of FILE to PREFIXaa, PREFIXab, ...;\n\
ପୂର୍ବନିର୍ଦ୍ଧାରିତ size is 1000 ଧାଡ଼ିଗୁଡ଼ିକ, and ପୂର୍ବନିର୍ଦ୍ଧାରିତ PREFIX is 'x'.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oprintf (_("\
  -a, --suffix-length=N\n\
         generate suffixes of length N (ପୂର୍ବନିର୍ଦ୍ଧାରିତ %d)\n\
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
  -C, --ଧାଡ଼ି-bytes=SIZE\n\
         put at most SIZE bytes of records per output file\n\
"));
      oputs (_("\
  -d\n\
         use numeric suffixes starting at 0, not alphabetic\n\
"));
      oputs (_("\
      --numeric-suffixes[=FROM]\n\
         same as -d, but allow setting the start ମୂଲ୍ୟ\n\
"));
      oputs (_("\
  -x\n\
         use hex suffixes starting at 0, not alphabetic\n\
"));
      oputs (_("\
      --hex-suffixes[=FROM]\n\
         same as -x, but allow setting the start ମୂଲ୍ୟ\n\
"));
      oputs (_("\
  -e, --elide-empty-files\n\
         କର not generate empty output files with '-n'\n\
"));
      oputs (_("\
      --filter=COMMAND\n\
         ଲିଖ to shell COMMAND; file name is $FILE\n\
"));
      oputs (_("\
  -l, --ଧାଡ଼ିଗୁଡ଼ିକ=ସଂଖ୍ୟା_2\n\
         put ସଂଖ୍ୟା_2 ଧାଡ଼ିଗୁଡ଼ିକ/records per output file\n\
"));
      oputs (_("\
  -n, --ସଂଖ୍ୟା=CHUNKS\n\
         generate CHUNKS output files; see explanation below\n\
"));
      oputs (_("\
  -t, --separator=SEP\n\
         use SEP instead of newline as the record separator;\n\
         '\\0' (zero) specifies the NUL character\n\
"));
      oputs (_("\
  -u, --unbuffered\n\
         immediately copy ନିବେଶ to output with '-n r/...'\n\
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
  N       split into N files based on size of ନିବେଶ\n\
  K/N     output Kth of N to standard output\n\
  l/N     split into N files without splitting ଧାଡ଼ିଗୁଡ଼ିକ/records\n\
  l/K/N   output Kth of N to standard output without splitting ଧାଡ଼ିଗୁଡ଼ିକ/records\n\
  r/N     like 'l' but use round robin distribution\n\
  r/K/N   likewise but only output Kth of N to standard output\n\
"), stdout);
      fputs (_("\n\
-n (except -nr) will buffer to $TMPDIR, defaulting to /tmp,\n\
ଯଦି the ନିବେଶ size cannot easily be determined.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Copy the data in FD to a temporary file, then make that file FD.
   Use BUF, of size BUFSIZE, to copy.  Return the number of
   bytes copied, or -1 (setting errno) on error.  */
ସ୍ଥିର off_t
copy_to_tmpfile (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅକ୍ଷର *buf, idx_t bufsize)
{
  FILE *tmp;
  ଯଦି (!temp_stream (&tmp, ଖାଲି))
    ଫେରନ୍ତୁ -1;
  off_t copied = 0;
  off_t r;

  ଯେତେବେଳେ (0 < (r = ପଠ (fd, buf, bufsize)))
    {
      ଯଦି (fwrite (buf, 1, r, tmp) != r)
        ଫେରନ୍ତୁ -1;
      ଯଦି (ckd_add (&copied, copied, r))
        {
          errno = EOVERFLOW;
          ଫେରନ୍ତୁ -1;
        }
    }

  ଯଦି (r < 0)
    ଫେରନ୍ତୁ r;
  r = dup2 (fileno (tmp), fd);
  ଯଦି (r < 0)
    ଫେରନ୍ତୁ r;
  ଯଦି (fclose (tmp) < 0)
    ଫେରନ୍ତୁ -1;
  ଫେରନ୍ତୁ copied;
}

/* Return the number of bytes that can be read from FD with status ST.
   Store up to the first BUFSIZE bytes of the file's data into BUF,
   and advance the file position by the number of bytes read.  On
   input error, set errno and return -1.  */

ସ୍ଥିର off_t
ନିବେଶ_file_size (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ସଂରଚନା stat ଅଚଳ *st, ଅକ୍ଷର *buf, idx_t bufsize)
{
  off_t size = 0;
  କର
    {
      ssize_t n_read = ପଠ (fd, buf + size, bufsize - size);
      ଯଦି (n_read <= 0)
        ଫେରନ୍ତୁ n_read < 0 ? n_read : size;
      size += n_read;
    }
  ଯେତେବେଳେ (size < bufsize);

  off_t cur, end;
  ଯଦି ((usable_st_size (st) && st->st_size < size)
      || (cur = lseek (fd, 0, SEEK_CUR)) < 0
      || cur < size /* E.g., /dev/zero on GNU/Linux.  */
      || (end = lseek (fd, 0, SEEK_END)) < 0)
    {
      ଅକ୍ଷର *tmpbuf = xmalloc (bufsize);
      end = copy_to_tmpfile (fd, tmpbuf, bufsize);
      ମୁକ୍ତ (tmpbuf);
      ଯଦି (end < 0)
        ଫେରନ୍ତୁ end;
      cur = 0;
    }

  ଯଦି (end == OFF_T_MAX /* E.g., /dev/zero on GNU/Hurd.  */
      || (cur < end && ckd_add (&size, size, end - cur)))
    {
      errno = EOVERFLOW;
      ଫେରନ୍ତୁ -1;
    }

  ଯଦି (cur < end)
    {
      off_t r = lseek (fd, cur, SEEK_SET);
      ଯଦି (r < 0)
        ଫେରନ୍ତୁ r;
    }

  ଫେରନ୍ତୁ size;
}

/* Compute the next sequential output file name and store it into the
   string 'outfile'.  */

ସ୍ଥିର ଶୂନ୍ୟ
next_file_name (ଶୂନ୍ୟ)
{
  /* Index in suffix_alphabet of each character in the suffix.  */
  ସ୍ଥିର idx_t *sufindex;
  ସ୍ଥିର idx_t outbase_length;
  ସ୍ଥିର idx_t outfile_length;
  ସ୍ଥିର idx_t addsuf_length;

  ଯଦି (! outfile)
    {
      bool overflow, widen;

new_name:
      widen = !! outfile_length;

      ଯଦି (! widen)
        {
          /* Allocate and initialize the first file name.  */

          outbase_length = strlen (outbase);
          addsuf_length = additional_suffix ? strlen (additional_suffix) : 0;
          overflow = ckd_add (&outfile_length, outbase_length + addsuf_length,
                              suffix_length);
        }
      ଅନ୍ୟଥା
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
      ଯଦି (overflow)
        xalloc_die ();
      outfile = xirealloc (outfile, outfile_size);

      ଯଦି (! widen)
        memcpy (outfile, outbase, outbase_length);
      ଅନ୍ୟଥା
        {
          /* Append the last alphabet character to the file name prefix.  */
          outfile[outbase_length] = suffix_alphabet[sufindex[0]];
          outbase_length++;
        }

      outfile_mid = outfile + outbase_length;
      memset (outfile_mid, suffix_alphabet[0], suffix_length);
      ଯଦି (additional_suffix)
        memcpy (outfile_mid + suffix_length, additional_suffix, addsuf_length);
      outfile[outfile_length] = 0;

      ମୁକ୍ତ (sufindex);
      sufindex = xicalloc (suffix_length, ଆକାର *sufindex);

      ଯଦି (numeric_suffix_start)
        {
          affirm (! widen);

          /* Update the output file name.  */
          idx_t i = strlen (numeric_suffix_start);
          memcpy (outfile_mid + suffix_length - i, numeric_suffix_start, i);

          /* Update the suffix index.  */
          idx_t *sufindex_end = sufindex + suffix_length;
          ଯେତେବେଳେ (i-- != 0)
            *--sufindex_end = numeric_suffix_start[i] - '0';
        }

#ଯଦି ! _POSIX_NO_TRUNC && HAVE_PATHCONF && defined _PC_NAME_MAX
      /* POSIX requires that if the output file name is too long for
         its directory, 'split' must fail without creating any files.
         This must be checked for explicitly on operating systems that
         silently truncate file names.  */
      {
        ଅକ୍ଷର *dir = dir_name (outfile);
        long name_max = pathconf (dir, _PC_NAME_MAX);
        ଯଦି (0 <= name_max && name_max < base_len (last_component (outfile)))
          error (EXIT_FAILURE, ENAMETOOLONG, "%s", quotef (outfile));
        ମୁକ୍ତ (dir);
      }
#endif
    }
  ଅନ୍ୟଥା
    {
      /* Increment the suffix in place, if possible.  */

      idx_t i = suffix_length;
      ଯେତେବେଳେ (i-- != 0)
        {
          sufindex[i]++;
          ଯଦି (suffix_auto && i == 0 && ! suffix_alphabet[sufindex[0] + 1])
            ଯାଅ new_name;
          outfile_mid[i] = suffix_alphabet[sufindex[i]];
          ଯଦି (outfile_mid[i])
            ଫେରନ୍ତୁ;
          sufindex[i] = 0;
          outfile_mid[i] = suffix_alphabet[sufindex[i]];
        }
      error (EXIT_FAILURE, 0, _("output file suffixes exhausted"));
    }
}

/* Create or truncate a file.  */

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
create (ଅକ୍ଷର ଅଚଳ *name)
{
  ଯଦି (!filter_command)
    {
      ଯଦି (verbose)
        fprintf (stdout, _("creating file %s\n"), quoteaf (name));

      ପୂର୍ଣ୍ଣସଂଖ୍ୟା oflags = O_WRONLY | O_CREAT | O_ଦ୍ୱିମିକ;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = open (name, oflags | O_EXCL, MODE_RW_UGO);
      ଯଦି (0 <= fd || errno != EEXIST)
        ଫେରନ୍ତୁ fd;
      fd = open (name, oflags, MODE_RW_UGO);
      ଯଦି (fd < 0)
        ଫେରନ୍ତୁ fd;
      ସଂରଚନା stat out_stat_buf;
      ଯଦି (fstat (fd, &out_stat_buf) != 0)
        error (EXIT_FAILURE, errno, _("failed to stat %s"), quoteaf (name));
      ଯଦି (psame_inode (&in_stat_buf, &out_stat_buf))
        error (EXIT_FAILURE, 0, _("%s would overwrite input; aborting"),
               quoteaf (name));
      ଯଦି (ftruncate (fd, 0) < 0
          && (S_ISREG (out_stat_buf.st_mode) || S_TYPEISSHM (&out_stat_buf)))
        error (EXIT_FAILURE, errno, _("%s: error truncating"), quotef (name));

      ଫେରନ୍ତୁ fd;
    }
  ଅନ୍ୟଥା
    {
      ଯଦି (setenv ("FILE", name, 1) != 0)
        error (EXIT_FAILURE, errno,
               _("failed to set FILE environment variable"));
      ଯଦି (verbose)
        fprintf (stdout, _("executing with FILE=%s\n"), quotef (name));

      ପୂର୍ଣ୍ଣସଂଖ୍ୟା result;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd_pair[2];
      pid_t child_pid;

      posix_spawnattr_t attr;
      posix_spawn_file_actions_t actions;

      sigset_t set;
      sigemptyset (&set);
      ଯଦି (default_SIGPIPE)
        sigaddset (&set, SIGPIPE);

      ଯଦି (   (result = posix_spawnattr_init (&attr))
          || (result = posix_spawnattr_setflags (&attr,
                                                 (POSIX_SPAWN_USEVFORK
                                                  | POSIX_SPAWN_SETSIGDEF)))
          || (result = posix_spawnattr_setsigdefault (&attr, &set))
          || (result = posix_spawn_file_actions_init (&actions))
         )
        error (EXIT_FAILURE, result, _("posix_spawn initialization failed"));

      ଯଦି (pipe (fd_pair) != 0)
        error (EXIT_FAILURE, errno, _("failed to create pipe"));

      /* We have to close any pipes that were opened during an
         earlier call, otherwise this process will be holding a
         write-pipe that will prevent the earlier process from
         reading an EOF on the corresponding read-pipe.  */
      ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < open_pipes.n_pipes; ++i)
        ଯଦି ((result = posix_spawn_file_actions_addclose (&actions,
                                                         open_pipes.pipes[i])))
          ଭାଙ୍ଗ;

      ଯଦି (   result
          || (result = posix_spawn_file_actions_addclose (&actions, fd_pair[1]))
          || (fd_pair[0] != STDIN_FILENO
              && (   (result = posix_spawn_file_actions_adddup2 (&actions,
                                                                 fd_pair[0],
                                                                 STDIN_FILENO))
                  || (result = posix_spawn_file_actions_addclose (&actions,
                                                                  fd_pair[0]))))
         )
        error (EXIT_FAILURE, result, _("posix_spawn setup failed"));


      ଅକ୍ଷର ଅଚଳ *shell_prog = getenv ("SHELL");
      ଯଦି (shell_prog == ଖାଲି)
        shell_prog = "/bin/sh";
      ଅକ୍ଷର ଅଚଳ *ଅଚଳ argv[] = { last_component (shell_prog), "-c",
                                   filter_command, ଖାଲି };

      result = posix_spawn (&child_pid, shell_prog, &actions, &attr,
                            (ଅକ୍ଷର * ଅଚଳ *) argv, environ);
      ଯଦି (result != 0)
        error (EXIT_FAILURE, errno, _("failed to run command: \"%s -c %s\""),
               shell_prog, filter_command);

      posix_spawnattr_destroy (&attr);
      posix_spawn_file_actions_destroy (&actions);

      ଯଦି (close (fd_pair[0]) != 0)
        error (EXIT_FAILURE, errno, _("failed to close input pipe"));
      filter_pid = child_pid;
      ଯଦି (open_pipes.n_pipes == open_pipes.n_pipes_alloc)
        open_pipes.pipes = xpalloc (open_pipes.pipes, &open_pipes.n_pipes_alloc,
                                    1, MIN (INT_MAX, IDX_MAX),
                                    ଆକାର *open_pipes.pipes);
      idx_t ଅଚଳ pipe_idx = open_pipes.n_pipes++;
      open_pipes.pipes[pipe_idx] = fd_pair[1];
      ଫେରନ୍ତୁ fd_pair[1];
    }
}

/* Close the output file, and do any associated cleanup.
   If FP and FD are both specified, they refer to the same open file;
   in this case FP is closed, but FD is still used in cleanup.  */
ସ୍ଥିର ଶୂନ୍ୟ
closeout (FILE *fp, ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, pid_t pid, ଅକ୍ଷର ଅଚଳ *name)
{
  ଯଦି (fp != ଖାଲି && fclose (fp) != 0 && ! ignorable (errno))
    error (EXIT_FAILURE, errno, "%s", quotef (name));
  ଯଦି (fd >= 0)
    {
      ଯଦି (fp == ଖାଲି && close (fd) < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (name));
      ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < open_pipes.n_pipes; ++j)
        {
          ଯଦି (open_pipes.pipes[j] == fd)
            {
              idx_t ଅଚଳ last_idx = open_pipes.n_pipes - 1;
              open_pipes.pipes[j] = open_pipes.pipes[last_idx];
              open_pipes.n_pipes--;
              ଭାଙ୍ଗ;
            }
        }
    }
  ଯଦି (pid > 0)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା wstatus;
      ଯଦି (waitpid (pid, &wstatus, 0) < 0)
        error (EXIT_FAILURE, errno, _("waiting for child process"));
      ଅନ୍ୟଥା ଯଦି (WIFSIGNALED (wstatus))
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା sig = WTERMSIG (wstatus);
          ଯଦି (sig != SIGPIPE)
            {
              ଅକ୍ଷର signame[MAX (SIG2STR_MAX, INT_BUFSIZE_BOUND (ପୂର୍ଣ୍ଣସଂଖ୍ୟା))];
              ଯଦି (sig2str (sig, signame) != 0)
                sprintf (signame, "%d", sig);
              error (sig + 128, 0,
                     _("with FILE=%s, signal %s from command: %s"),
                     quotef (name), signame, filter_command);
            }
        }
      ଅନ୍ୟଥା ଯଦି (WIFEXITED (wstatus))
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା ex = WEXITSTATUS (wstatus);
          ଯଦି (ex != 0)
            error (ex, 0, _("with FILE=%s, exit %d from command: %s"),
                   quotef (name), ex, filter_command);
        }
      ଅନ୍ୟଥା
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

ସ୍ଥିର bool
cwrite (bool new_file_flag, ଅକ୍ଷର ଅଚଳ *bp, idx_t bytes)
{
  ଯଦି (new_file_flag)
    {
      ଯଦି (!bp && bytes == 0 && elide_empty_files)
        ଫେରନ୍ତୁ true;
      closeout (ଖାଲି, output_desc, filter_pid, outfile);
      next_file_name ();
      output_desc = create (outfile);
      ଯଦି (output_desc < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (outfile));
    }

  ଯଦି (full_write (output_desc, bp, bytes) == bytes)
    ଫେରନ୍ତୁ true;
  ଅନ୍ୟଥା
    {
      ଯଦି (! ignorable (errno))
        error (EXIT_FAILURE, errno, "%s", quotef (outfile));
      ଫେରନ୍ତୁ false;
    }
}

/* Split into pieces of exactly N_BYTES bytes.
   However, the first REM_BYTES pieces should be 1 byte longer.
   Use buffer BUF, whose size is BUFSIZE.
   If INITIAL_READ is nonnegative,
   BUF contains the first INITIAL_READ input bytes.  */

ସ୍ଥିର ଶୂନ୍ୟ
bytes_split (intmax_t n_bytes, intmax_t rem_bytes,
             ଅକ୍ଷର *buf, idx_t bufsize, ssize_t initial_read,
             intmax_t max_files)
{
  bool new_file_flag = true;
  bool filter_ok = true;
  intmax_t opened = 0;
  intmax_t to_write = n_bytes + (0 < rem_bytes);
  bool eof = ! to_write;

  ଯେତେବେଳେ (! eof)
    {
      ssize_t n_read;
      ଯଦି (0 <= initial_read)
        {
          n_read = initial_read;
          initial_read = -1;
          eof = n_read < bufsize;
        }
      ଅନ୍ୟଥା
        {
          ଯଦି (! filter_ok
              && 0 <= lseek (STDIN_FILENO, to_write, SEEK_CUR))
            {
              to_write = n_bytes + (opened + 1 < rem_bytes);
              new_file_flag = true;
            }

          n_read = ପଠ (STDIN_FILENO, buf, bufsize);
          ଯଦି (n_read < 0)
            error (EXIT_FAILURE, errno, "%s", quotef (infile));
          eof = n_read == 0;
        }
      ଅକ୍ଷର *bp_out = buf;
      ଯେତେବେଳେ (0 < to_write && to_write <= n_read)
        {
          ଯଦି (filter_ok || new_file_flag)
            filter_ok = cwrite (new_file_flag, bp_out, to_write);
          opened += new_file_flag;
          new_file_flag = !max_files || (opened < max_files);
          ଯଦି (! filter_ok && ! new_file_flag)
            {
              /* If filters no longer accepting input, stop reading.  */
              n_read = 0;
              eof = true;
              ଭାଙ୍ଗ;
            }
          bp_out += to_write;
          n_read -= to_write;
          to_write = n_bytes + (opened < rem_bytes);
        }
      ଯଦି (0 < n_read)
        {
          ଯଦି (filter_ok || new_file_flag)
            filter_ok = cwrite (new_file_flag, bp_out, n_read);
          opened += new_file_flag;
          new_file_flag = false;
          ଯଦି (! filter_ok && opened == max_files)
            {
              /* If filters no longer accepting input, stop reading.  */
              ଭାଙ୍ଗ;
            }
          to_write -= n_read;
        }
    }

  /* Ensure NUMBER files are created, which truncates
     any existing files or notifies any consumers on fifos.
     FIXME: Should we do this before EXIT_FAILURE?  */
  ଯେତେବେଳେ (opened++ < max_files)
    cwrite (true, ଖାଲି, 0);
}

/* Split into pieces of exactly N_LINES lines.
   Use buffer BUF, whose size is BUFSIZE.  */

ସ୍ଥିର ଶୂନ୍ୟ
ଧାଡ଼ିଗୁଡ଼ିକ_split (intmax_t n_ଧାଡ଼ିଗୁଡ଼ିକ, ଅକ୍ଷର *buf, idx_t bufsize)
{
  ssize_t n_read;
  ଅକ୍ଷର *bp, *bp_out, *eob;
  bool new_file_flag = true;
  intmax_t n = 0;

  କର
    {
      n_read = ପଠ (STDIN_FILENO, buf, bufsize);
      ଯଦି (n_read < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
      bp = bp_out = buf;
      eob = bp + n_read;
      *eob = eolchar;
      ଯେତେବେଳେ (true)
        {
          bp = rawmemchr (bp, eolchar);
          ଯଦି (bp == eob)
            {
              ଯଦି (eob != bp_out) /* do not write 0 bytes! */
                {
                  idx_t len = eob - bp_out;
                  cwrite (new_file_flag, bp_out, len);
                  new_file_flag = false;
                }
              ଭାଙ୍ଗ;
            }

          ++bp;
          ଯଦି (++n >= n_ଧାଡ଼ିଗୁଡ଼ିକ)
            {
              cwrite (new_file_flag, bp_out, bp - bp_out);
              bp_out = bp;
              new_file_flag = true;
              n = 0;
            }
        }
    }
  ଯେତେବେଳେ (n_read);
}

/* Split into pieces that are as large as possible while still not more
   than N_BYTES bytes, and are split on line boundaries except
   where lines longer than N_BYTES bytes occur. */

ସ୍ଥିର ଶୂନ୍ୟ
ଧାଡ଼ି_bytes_split (intmax_t n_bytes, ଅକ୍ଷର *buf, idx_t bufsize)
{
  ssize_t n_read;
  intmax_t n_out = 0;      /* for each split.  */
  idx_t n_hold = 0;
  ଅକ୍ଷର *hold = ଖାଲି;        /* for lines > bufsize.  */
  idx_t hold_size = 0;
  bool split_ଧାଡ଼ି = false;  /* Whether a \n was output in a split.  */

  କର
    {
      n_read = ପଠ (STDIN_FILENO, buf, bufsize);
      ଯଦି (n_read < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
      idx_t n_ବାମ = n_read;
      ଅକ୍ଷର *sob = buf;
      ଯେତେବେଳେ (n_ବାମ)
        {
          idx_t split_rest = 0;
          ଅକ୍ଷର *eoc = ଖାଲି;
          ଅକ୍ଷର *eol;

          /* Determine End Of Chunk and/or End of Line,
             which are used below to select what to write or buffer.  */
          ଯଦି (n_bytes - n_out - n_hold <= n_ବାମ)
            {
              /* Have enough for split.  */
              split_rest = n_bytes - n_out - n_hold;
              eoc = sob + split_rest - 1;
              eol = memrchr (sob, eolchar, split_rest);
            }
          ଅନ୍ୟଥା
            eol = memrchr (sob, eolchar, n_ବାମ);

          /* Output hold space if possible.  */
          ଯଦି (n_hold && !(!eol && n_out))
            {
              cwrite (n_out == 0, hold, n_hold);
              n_out += n_hold;
              n_hold = 0;
            }

          /* Output to eol if present.  */
          ଯଦି (eol)
            {
              split_ଧାଡ଼ି = true;
              idx_t n_write = eol - sob + 1;
              cwrite (n_out == 0, sob, n_write);
              n_out += n_write;
              n_ବାମ -= n_write;
              sob += n_write;
              ଯଦି (eoc)
                split_rest -= n_write;
            }

          /* Output to eoc or eob if possible.  */
          ଯଦି (n_ବାମ && !split_ଧାଡ଼ି)
            {
              idx_t n_write = eoc ? split_rest : n_ବାମ;
              cwrite (n_out == 0, sob, n_write);
              n_out += n_write;
              n_ବାମ -= n_write;
              sob += n_write;
              ଯଦି (eoc)
                split_rest -= n_write;
            }

          /* Update hold if needed.  */
          ଯଦି ((eoc && split_rest) || (!eoc && n_ବାମ))
            {
              idx_t n_buf = eoc ? split_rest : n_ବାମ;
              ଯଦି (hold_size - n_hold < n_buf)
                hold = xpalloc (hold, &hold_size, n_buf - (hold_size - n_hold),
                                -1, ଆକାର *hold);
              memcpy (hold + n_hold, sob, n_buf);
              n_hold += n_buf;
              n_ବାମ -= n_buf;
              sob += n_buf;
            }

          /* Reset for new split.  */
          ଯଦି (eoc)
            {
              n_out = 0;
              split_ଧାଡ଼ି = false;
            }
        }
    }
  ଯେତେବେଳେ (n_read);

  /* Handle no eol at end of file.  */
  ଯଦି (n_hold)
    cwrite (n_out == 0, hold, n_hold);

  ମୁକ୍ତ (hold);
}

/* -n l/[K/]N: Write lines to files of approximately file size / N.
   The file is partitioned into file size / N sized portions, with the
   last assigned any excess.  If a line _starts_ within a partition
   it is written completely to the corresponding file.  Since lines
   are not split even if they overlap a partition, the files written
   can be larger or smaller than the partition size, and even empty
   if a line is so long as to completely overlap the partition.  */

ସ୍ଥିର ଶୂନ୍ୟ
ଧାଡ଼ିଗୁଡ଼ିକ_chunk_split (intmax_t k, intmax_t n, ଅକ୍ଷର *buf, idx_t bufsize,
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

  ଯଦି (k > 1 && 0 < file_size)
    {
      /* Start reading 1 byte before kth chunk of file.  */
      off_t start = (k - 1) * chunk_size + MIN (k - 1, rem_bytes) - 1;
      ଯଦି (start < initial_read)
        {
          memmove (buf, buf + start, initial_read - start);
          initial_read -= start;
        }
      ଅନ୍ୟଥା
        {
          ଯଦି (initial_read < start
              && lseek (STDIN_FILENO, start - initial_read, SEEK_CUR) < 0)
            error (EXIT_FAILURE, errno, "%s", quotef (infile));
          initial_read = -1;
        }
      n_written = start;
      chunk_no = k - 1;
      chunk_end = start + 1;
    }

  ଯେତେବେଳେ (n_written < file_size)
    {
      ଅକ୍ଷର *bp = buf, *eob;
      ssize_t n_read;
      ଯଦି (0 <= initial_read)
        {
          n_read = initial_read;
          initial_read = -1;
        }
      ଅନ୍ୟଥା
        {
          n_read = ପଠ (STDIN_FILENO, buf,
                         MIN (bufsize, file_size - n_written));
          ଯଦି (n_read < 0)
            error (EXIT_FAILURE, errno, "%s", quotef (infile));
        }
      ଯଦି (n_read == 0)
        ଭାଙ୍ଗ; /* eof.  */
      chunk_truncated = false;
      eob = buf + n_read;

      ଯେତେବେଳେ (bp != eob)
        {
          idx_t to_write;
          bool next = false;

          /* Begin looking for '\n' at last byte of chunk.  */
          off_t skip = MIN (n_read, MAX (0, chunk_end - 1 - n_written));
          ଅକ୍ଷର *bp_out = memchr (bp + skip, eolchar, n_read - skip);
          ଯଦି (bp_out)
            {
              bp_out++;
              next = true;
            }
          ଅନ୍ୟଥା
            bp_out = eob;
          to_write = bp_out - bp;

          ଯଦି (k == chunk_no)
            {
              /* We don't use the stdout buffer here since we're writing
                 large chunks from an existing file, so it's more efficient
                 to write out directly.  */
              ଯଦି (full_write (STDOUT_FILENO, bp, to_write) != to_write)
                write_error ();
            }
          ଅନ୍ୟଥା ଯଦି (! k)
            cwrite (new_file_flag, bp, to_write);
          n_written += to_write;
          bp += to_write;
          n_read -= to_write;
          new_file_flag = next;

          /* A line could have been so long that it skipped
             entire chunks. So create empty files in that case.  */
          ଯେତେବେଳେ (next || chunk_end <= n_written)
            {
              ଯଦି (!next && bp == eob)
                {
                  /* replenish buf, before going to next chunk.  */
                  chunk_truncated = true;
                  ଭାଙ୍ଗ;
                }
              ଯଦି (k == chunk_no)
                ଫେରନ୍ତୁ;
              chunk_end += chunk_size + (chunk_no < rem_bytes);
              chunk_no++;
              ଯଦି (chunk_end <= n_written)
                {
                  ଯଦି (! k)
                    cwrite (true, ଖାଲି, 0);
                }
              ଅନ୍ୟଥା
                next = false;
            }
        }
    }

  ଯଦି (chunk_truncated)
    chunk_no++;

  /* Ensure NUMBER files are created, which truncates
     any existing files or notifies any consumers on fifos.
     FIXME: Should we do this before EXIT_FAILURE?  */
  ଯଦି (!k)
    ଯେତେବେଳେ (chunk_no++ <= n)
      cwrite (true, ଖାଲି, 0);
}

/* -n K/N: Extract Kth of N chunks.  */

ସ୍ଥିର ଶୂନ୍ୟ
bytes_chunk_extract (intmax_t k, intmax_t n, ଅକ୍ଷର *buf, idx_t bufsize,
                     ssize_t initial_read, off_t file_size)
{
  off_t start;
  off_t end;

  affirm (0 < k && k <= n);

  start = (k - 1) * (file_size / n) + MIN (k - 1, file_size % n);
  end = k == n ? file_size : k * (file_size / n) + MIN (k, file_size % n);

  ଯଦି (start < initial_read)
    {
      memmove (buf, buf + start, initial_read - start);
      initial_read -= start;
    }
  ଅନ୍ୟଥା
    {
      ଯଦି (initial_read < start
          && lseek (STDIN_FILENO, start - initial_read, SEEK_CUR) < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
      initial_read = -1;
    }

  ଯେତେବେଳେ (start < end)
    {
      ssize_t n_read;
      ଯଦି (0 <= initial_read)
        {
          n_read = initial_read;
          initial_read = -1;
        }
      ଅନ୍ୟଥା
        {
          n_read = ପଠ (STDIN_FILENO, buf, bufsize);
          ଯଦି (n_read < 0)
            error (EXIT_FAILURE, errno, "%s", quotef (infile));
        }
      ଯଦି (n_read == 0)
        ଭାଙ୍ଗ; /* eof.  */
      n_read = MIN (n_read, end - start);
      ଯଦି (full_write (STDOUT_FILENO, buf, n_read) != n_read
          && ! ignorable (errno))
        error (EXIT_FAILURE, errno, "%s", quotef ("-"));
      start += n_read;
    }
}

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା of_info
{
  ଅକ୍ଷର *of_name;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ofd;
  FILE *ofile;
  pid_t opid;
} of_t;

ଗଣନା
{
  OFD_NEW = -1,
  OFD_APPEND = -2
};

/* Rotate file descriptors when we're writing to more output files than we
   have available file descriptors.
   Return whether we came under file resource pressure.
   If so, it's probably best to close each file when finished with it.  */

ସ୍ଥିର bool
ofile_open (of_t *files, idx_t i_check, idx_t nfiles)
{
  bool file_limit = false;

  ଯଦି (files[i_check].ofd <= OFD_NEW)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd;
      idx_t i_reopen = i_check ? i_check - 1 : nfiles - 1;

      /* Another process could have opened a file in between the calls to
         close and open, so we should keep trying until open succeeds or
         we've closed all of our files.  */
      ଯେତେବେଳେ (true)
        {
          ଯଦି (files[i_check].ofd == OFD_NEW)
            fd = create (files[i_check].of_name);
          ଅନ୍ୟଥା /* OFD_APPEND  */
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
                         O_WRONLY | O_ଦ୍ୱିମିକ | O_APPEND | O_NONBLOCK);
            }

          ଯଦି (0 <= fd)
            ଭାଙ୍ଗ;

          ଯଦି (!(errno == EMFILE || errno == ENFILE))
            error (EXIT_FAILURE, errno, "%s", quotef (files[i_check].of_name));

          file_limit = true;

          /* Search backwards for an open file to close.  */
          ଯେତେବେଳେ (files[i_reopen].ofd < 0)
            {
              i_reopen = i_reopen ? i_reopen - 1 : nfiles - 1;
              /* No more open files to close, exit with E[NM]FILE.  */
              ଯଦି (i_reopen == i_check)
                error (EXIT_FAILURE, errno, "%s",
                       quotef (files[i_check].of_name));
            }

          ଯଦି (fclose (files[i_reopen].ofile) != 0)
            error (EXIT_FAILURE, errno, "%s", quotef (files[i_reopen].of_name));
          files[i_reopen].ofile = ଖାଲି;
          files[i_reopen].ofd = OFD_APPEND;
        }

      files[i_check].ofd = fd;
      FILE *ofile = fdopen (fd, "a");
      ଯଦି (!ofile)
        error (EXIT_FAILURE, errno, "%s", quotef (files[i_check].of_name));
      files[i_check].ofile = ofile;
      files[i_check].opid = filter_pid;
      filter_pid = 0;
    }

  ଫେରନ୍ତୁ file_limit;
}

/* -n r/[K/]N: Divide file into N chunks in round robin fashion.
   Use BUF of size BUFSIZE for the buffer, and if allocating storage
   put its address into *FILESP to pacify -fsanitize=leak.
   When K == 0, we try to keep the files open in parallel.
   If we run out of file resources, then we revert
   to opening and closing each file for each line.  */

ସ୍ଥିର ଶୂନ୍ୟ
ଧାଡ଼ିଗୁଡ଼ିକ_rr (intmax_t k, intmax_t n, ଅକ୍ଷର *buf, idx_t bufsize, of_t **filesp)
{
  bool wrapped = false;
  bool wrote = false;
  bool file_limit;
  idx_t i_file;
  of_t *files IF_LINT (= ଖାଲି);
  intmax_t ଧାଡ଼ି_no;

  ଯଦି (k)
    ଧାଡ଼ି_no = 1;
  ଅନ୍ୟଥା
    {
      ଯଦି (IDX_MAX < n)
        xalloc_die ();
      files = *filesp = xinmalloc (n, ଆକାର *files);

      /* Generate output file names. */
      ପାଇଁ (i_file = 0; i_file < n; i_file++)
        {
          next_file_name ();
          files[i_file].of_name = xstrdup (outfile);
          files[i_file].ofd = OFD_NEW;
          files[i_file].ofile = ଖାଲି;
          files[i_file].opid = 0;
        }
      i_file = 0;
      file_limit = false;
    }

  ଯେତେବେଳେ (true)
    {
      ଅକ୍ଷର *bp = buf, *eob;
      ssize_t n_read = ପଠ (STDIN_FILENO, buf, bufsize);
      ଯଦି (n_read < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
      ଅନ୍ୟଥା ଯଦି (n_read == 0)
        ଭାଙ୍ଗ; /* eof.  */
      eob = buf + n_read;

      ଯେତେବେଳେ (bp != eob)
        {
          idx_t to_write;
          bool next = false;

          /* Find end of line. */
          ଅକ୍ଷର *bp_out = memchr (bp, eolchar, eob - bp);
          ଯଦି (bp_out)
            {
              bp_out++;
              next = true;
            }
          ଅନ୍ୟଥା
            bp_out = eob;
          to_write = bp_out - bp;

          ଯଦି (k)
            {
              ଯଦି (ଧାଡ଼ି_no == k && unbuffered)
                {
                  ଯଦି (full_write (STDOUT_FILENO, bp, to_write) != to_write)
                    write_error ();
                }
              ଅନ୍ୟଥା ଯଦି (ଧାଡ଼ି_no == k && fwrite (bp, to_write, 1, stdout) != 1)
                {
                  write_error ();
                }
              ଯଦି (next)
                ଧାଡ଼ି_no = (ଧାଡ଼ି_no == n) ? 1 : ଧାଡ଼ି_no + 1;
            }
          ଅନ୍ୟଥା
            {
              /* Secure file descriptor. */
              file_limit |= ofile_open (files, i_file, n);
              ଯଦି (unbuffered)
                {
                  /* Note writing to fd, rather than flushing the FILE gives
                     an 8% performance benefit, due to reduced data copying.  */
                  ଯଦି (full_write (files[i_file].ofd, bp, to_write) != to_write
                      && ! ignorable (errno))
                    error (EXIT_FAILURE, errno, "%s",
                           quotef (files[i_file].of_name));
                }
              ଅନ୍ୟଥା ଯଦି (fwrite (bp, to_write, 1, files[i_file].ofile) != 1
                       && ! ignorable (errno))
                error (EXIT_FAILURE, errno, "%s",
                       quotef (files[i_file].of_name));

              ଯଦି (! ignorable (errno))
                wrote = true;

              ଯଦି (file_limit)
                {
                  ଯଦି (fclose (files[i_file].ofile) != 0)
                    error (EXIT_FAILURE, errno, "%s",
                           quotef (files[i_file].of_name));
                  files[i_file].ofile = ଖାଲି;
                  files[i_file].ofd = OFD_APPEND;
                }
              ଯଦି (next && ++i_file == n)
                {
                  wrapped = true;
                  /* If no filters are accepting input, stop reading.  */
                  ଯଦି (! wrote)
                    ଯାଅ no_filters;
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
  ଯଦି (!k)
    {
      idx_t ceiling = wrapped ? n : i_file;
      ପାଇଁ (i_file = 0; i_file < n; i_file++)
        {
          ଯଦି (i_file >= ceiling && !elide_empty_files)
            file_limit |= ofile_open (files, i_file, n);
          ଯଦି (files[i_file].ofd >= 0)
            closeout (files[i_file].ofile, files[i_file].ofd,
                      files[i_file].opid, files[i_file].of_name);
          files[i_file].ofd = OFD_APPEND;
        }
    }
}

#define FAIL_ONLY_ONE_WAY()					\
  କର								\
    {								\
      error (0, 0, _("cannot split in more than one way"));	\
      usage (EXIT_FAILURE);					\
    }								\
  ଯେତେବେଳେ (0)

/* Report a string-to-integer conversion failure MSGID with ARG.  */

ସ୍ଥିର _Noreturn ଶୂନ୍ୟ
strtoint_die (ଅକ୍ଷର ଅଚଳ *msgid, ଅକ୍ଷର ଅଚଳ *arg)
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

ସ୍ଥିର intmax_t
parse_n_units (ଅକ୍ଷର ଅଚଳ *arg, ଅକ୍ଷର ଅଚଳ *multipliers, ଅକ୍ଷର ଅଚଳ *msgid)
{
  intmax_t n;
  ଯଦି (OVERFLOW_OK < xstrtoimax (arg, ଖାଲି, 10, &n, multipliers) || n < 1)
    strtoint_die (msgid, arg);
  ଫେରନ୍ତୁ n;
}

/* Parse K/N syntax of chunk options.  */

ସ୍ଥିର ଶୂନ୍ୟ
parse_chunk (intmax_t *k_units, intmax_t *n_units, ଅକ୍ଷର ଅଚଳ *arg)
{
  ଅକ୍ଷର *argend;
  strtol_error e = xstrtoimax (arg, &argend, 10, n_units, "");
  ଯଦି (e == LONGINT_INVALID_SUFFIX_CHAR && *argend == '/')
    {
      *k_units = *n_units;
      *n_units = parse_n_units (argend + 1, "",
                                N_("invalid number of chunks"));
      ଯଦି (! (0 < *k_units && *k_units <= *n_units))
        error (EXIT_FAILURE, 0, "%s: %s", _("invalid chunk number"),
               quote_mem (arg, argend - arg));
    }
  ଅନ୍ୟଥା ଯଦି (! (e <= OVERFLOW_OK && 0 < *n_units))
    strtoint_die (N_("invalid number of chunks"), arg);
}


ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  ଗଣନା Split_type split_type = type_undef;
  idx_t in_blk_size = 0;	/* optimal block size of input file device */
  idx_t page_size = getpagesize ();
  intmax_t k_units = 0;
  intmax_t n_units = 0;

  ସ୍ଥିର ଅକ୍ଷର ଅଚଳ multipliers[] = "bEGKkMmPQRTYZ0";
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା c;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା digits_optind = 0;
  off_t file_size = OFF_T_MAX;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* Parse command line options.  */

  ଯେତେବେଳେ (true)
    {
      /* This is the argv-index of the option we will read next.  */
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା this_optind = optind ? optind : 1;

      c = getopt_long (argc, argv, "0123456789C:a:b:del:n:t:ux",
                       longopts, ଖାଲି);
      ଯଦି (c == -1)
        ଭାଙ୍ଗ;

      ବାଛ (c)
        {
        ସ୍ଥିତି 'a':
          suffix_length = xdectoimax (optarg, 0, IDX_MAX,
                                      "", _("invalid suffix length"), 0);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି ADDITIONAL_SUFFIX_OPTION:
          {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା suffix_len = strlen (optarg);
            ଯଦି (last_component (optarg) != optarg
                || (suffix_len && ISSLASH (optarg[suffix_len - 1])))
              {
                error (0, 0,
                       _("invalid suffix %s, contains directory separator"),
                       quote (optarg));
                usage (EXIT_FAILURE);
              }
          }
          additional_suffix = optarg;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'b':
          ଯଦି (split_type != type_undef)
            FAIL_ONLY_ONE_WAY ();
          split_type = type_bytes;
          n_units = parse_n_units (optarg, multipliers,
                                   N_("invalid number of bytes"));
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'l':
          ଯଦି (split_type != type_undef)
            FAIL_ONLY_ONE_WAY ();
          split_type = type_ଧାଡ଼ିଗୁଡ଼ିକ;
          n_units = parse_n_units (optarg, "", N_("invalid number of lines"));
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'C':
          ଯଦି (split_type != type_undef)
            FAIL_ONLY_ONE_WAY ();
          split_type = type_byteslines;
          n_units = parse_n_units (optarg, multipliers,
                                   N_("invalid number of lines"));
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'n':
          ଯଦି (split_type != type_undef)
            FAIL_ONLY_ONE_WAY ();
          /* skip any whitespace */
          ଯେତେବେଳେ (isspace (to_uchar (*optarg)))
            optarg++;
          ଯଦି (STRNCMP_LIT (optarg, "r/") == 0)
            {
              split_type = type_rr;
              optarg += 2;
            }
          ଅନ୍ୟଥା ଯଦି (STRNCMP_LIT (optarg, "l/") == 0)
            {
              split_type = type_chunk_ଧାଡ଼ିଗୁଡ଼ିକ;
              optarg += 2;
            }
          ଅନ୍ୟଥା
            split_type = type_chunk_bytes;
          parse_chunk (&k_units, &n_units, optarg);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'u':
          unbuffered = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 't':
          {
            ଅକ୍ଷର neweol = optarg[0];
            ଯଦି (! neweol)
              error (EXIT_FAILURE, 0, _("empty record separator"));
            ଯଦି (optarg[1])
              {
                ଯଦି (streq (optarg, "\\0"))
                  neweol = '\0';
                ଅନ୍ୟଥା
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
            ଯଦି (0 <= eolchar && neweol != eolchar)
              {
                error (EXIT_FAILURE, 0,
                       _("multiple separator characters specified"));
              }

            eolchar = neweol;
          }
          ଭାଙ୍ଗ;

        ସ୍ଥିତି '0':
        ସ୍ଥିତି '1':
        ସ୍ଥିତି '2':
        ସ୍ଥିତି '3':
        ସ୍ଥିତି '4':
        ସ୍ଥିତି '5':
        ସ୍ଥିତି '6':
        ସ୍ଥିତି '7':
        ସ୍ଥିତି '8':
        ସ୍ଥିତି '9':
          ଯଦି (split_type == type_undef)
            {
              split_type = type_digits;
              n_units = 0;
            }
          ଯଦି (split_type != type_undef && split_type != type_digits)
            FAIL_ONLY_ONE_WAY ();
          ଯଦି (digits_optind != 0 && digits_optind != this_optind)
            n_units = 0;	/* More than one number given; ignore other. */
          digits_optind = this_optind;
          ଯଦି (ckd_mul (&n_units, n_units, 10)
              || ckd_add (&n_units, n_units, c - '0'))
            n_units = INTMAX_MAX;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'd':
        ସ୍ଥିତି 'x':
          ଯଦି (c == 'd')
            suffix_alphabet = "0123456789";
          ଅନ୍ୟଥା
            suffix_alphabet = "0123456789abcdef";
          ଯଦି (optarg)
            {
              ଯଦି (strlen (optarg) != strspn (optarg, suffix_alphabet))
                {
                  error (0, 0,
                         (c == 'd') ?
                           _("%s: invalid start value for numerical suffix") :
                           _("%s: invalid start value for hexadecimal suffix"),
                         quote (optarg));
                  usage (EXIT_FAILURE);
                }
              ଅନ୍ୟଥା
                {
                  /* Skip any leading zero.  */
                  ଯେତେବେଳେ (*optarg == '0' && *(optarg + 1) != '\0')
                    optarg++;
                  numeric_suffix_start = optarg;
                }
            }
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'e':
          elide_empty_files = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି FILTER_OPTION:
          filter_command = optarg;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି IO_BLKSIZE_OPTION:
          in_blk_size = xnumtoumax (optarg, 10, 1,
                                    MIN (SYS_BUFSIZE_MAX,
                                         MIN (IDX_MAX, SIZE_MAX) - 1),
                                    multipliers, _("invalid IO block size"),
                                    0, XTOINT_MIN_RANGE);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି VERBOSE_OPTION:
          verbose = true;
          ଭାଙ୍ଗ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          usage (EXIT_FAILURE);
        }
    }

  ଯଦି (k_units != 0 && filter_command)
    {
      error (0, 0, _("--filter does not process a chunk extracted to "
                     "standard output"));
      usage (EXIT_FAILURE);
    }

  /* Handle default case.  */
  ଯଦି (split_type == type_undef)
    {
      split_type = type_ଧାଡ଼ିଗୁଡ଼ିକ;
      n_units = 1000;
    }

  ଯଦି (n_units == 0)
    {
      error (0, 0, _("invalid number of lines: %s"), quote ("0"));
      usage (EXIT_FAILURE);
    }

  ଯଦି (eolchar < 0)
    eolchar = '\n';

  set_suffix_length (n_units, split_type);

  /* Get out the filename arguments.  */

  ଯଦି (optind < argc)
    infile = argv[optind++];

  ଯଦି (optind < argc)
    outbase = argv[optind++];

  ଯଦି (optind < argc)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind]));
      usage (EXIT_FAILURE);
    }

  /* Check that the suffix length is large enough for the numerical
     suffix start value.  */
  ଯଦି (numeric_suffix_start && strlen (numeric_suffix_start) > suffix_length)
    {
      error (0, 0, _("numerical suffix start value is too large "
                     "for the suffix length"));
      usage (EXIT_FAILURE);
    }

  /* Open the input file.  */
  ଯଦି (! streq (infile, "-")
      && fd_reopen (STDIN_FILENO, infile, O_RDONLY, 0) < 0)
    error (EXIT_FAILURE, errno, _("cannot open %s for reading"),
           quoteaf (infile));

  /* Binary I/O is safer when byte counts are used.  */
  xset_ଦ୍ୱିମିକ_mode (STDIN_FILENO, O_ଦ୍ୱିମିକ);

  /* Advise the kernel of our access pattern.  */
  fdadvise (STDIN_FILENO, 0, 0, FADVISE_SEQUENTIAL);

  /* Get the optimal block size of input device and make a buffer.  */

  ଯଦି (fstat (STDIN_FILENO, &in_stat_buf) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (infile));

  ଯଦି (in_blk_size == 0)
    {
      in_blk_size = io_blksize (&in_stat_buf);
      ଯଦି (SYS_BUFSIZE_MAX < in_blk_size)
        in_blk_size = SYS_BUFSIZE_MAX;
    }

  ଅକ୍ଷର *buf = xalignalloc (page_size, in_blk_size + 1);
  ssize_t initial_read = -1;

  ଯଦି (split_type == type_chunk_bytes || split_type == type_chunk_ଧାଡ଼ିଗୁଡ଼ିକ)
    {
      file_size = ନିବେଶ_file_size (STDIN_FILENO, &in_stat_buf,
                                   buf, in_blk_size);
      ଯଦି (file_size < 0)
        error (EXIT_FAILURE, errno, _("%s: cannot determine file size"),
               quotef (infile));
      initial_read = MIN (file_size, in_blk_size);
    }

  /* When filtering, closure of one pipe must not terminate the process,
     as there may still be other streams expecting input from us.  */
  ଯଦି (filter_command)
    default_SIGPIPE = ସଙ୍କେତ (SIGPIPE, SIG_IGN) == SIG_DFL;

  ବାଛ (split_type)
    {
    ସ୍ଥିତି type_digits:
    ସ୍ଥିତି type_ଧାଡ଼ିଗୁଡ଼ିକ:
      ଧାଡ଼ିଗୁଡ଼ିକ_split (n_units, buf, in_blk_size);
      ଭାଙ୍ଗ;

    ସ୍ଥିତି type_bytes:
      bytes_split (n_units, 0, buf, in_blk_size, -1, 0);
      ଭାଙ୍ଗ;

    ସ୍ଥିତି type_byteslines:
      ଧାଡ଼ି_bytes_split (n_units, buf, in_blk_size);
      ଭାଙ୍ଗ;

    ସ୍ଥିତି type_chunk_bytes:
      ଯଦି (k_units == 0)
        bytes_split (file_size / n_units, file_size % n_units,
                     buf, in_blk_size, initial_read, n_units);
      ଅନ୍ୟଥା
        bytes_chunk_extract (k_units, n_units, buf, in_blk_size, initial_read,
                             file_size);
      ଭାଙ୍ଗ;

    ସ୍ଥିତି type_chunk_ଧାଡ଼ିଗୁଡ଼ିକ:
      ଧାଡ଼ିଗୁଡ଼ିକ_chunk_split (k_units, n_units, buf, in_blk_size, initial_read,
                         file_size);
      ଭାଙ୍ଗ;

    ସ୍ଥିତି type_rr:
      /* Note, this is like 'sed -n ${k}~${n}p' when k > 0,
         but the functionality is provided for symmetry.  */
      {
        of_t *files;
        ଧାଡ଼ିଗୁଡ଼ିକ_rr (k_units, n_units, buf, in_blk_size, &files);
      }
      ଭାଙ୍ଗ;

    ସ୍ଥିତି type_undef: ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      affirm (false);
    }

  ଯଦି (close (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (infile));
  closeout (ଖାଲି, output_desc, filter_pid, outfile);

  main_exit (EXIT_SUCCESS);
}
