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
#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <signal.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/wait.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <spawn.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "alignalloc.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "assure.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "fadvise.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "fd-reopen.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "fcntl--.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "full-write.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ioblksize.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "sig2str.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "sys-limits.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "temp-stream.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unistd--.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xbinary-io.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xdectoint.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "split"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("Richard M. Stallman")

/* Shell command to filter through, instead of creating files.  */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *filter_command;

/* Process ID of the filter.  */
ਸਥਿਰ pid_t filter_pid;

ਸਥਿਰ ਬਣਤਰ
{
  /* The number of open pipes.  */
  ਪੂਰਨ n_pipes;

  /* The number of allocated pipes.  */
  idx_t n_pipes_alloc;

  /* Array of open pipes.  */
  ਪੂਰਨ *pipes COUNTED_BY (n_pipes);
} open_pipes;

/* Whether SIGPIPE has the default action, when --filter is used.  */
ਸਥਿਰ bool default_SIGPIPE;

/* Base name of output files.  */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *outbase = "x";

/* Name of output files.  */
ਸਥਿਰ ਅੱਖਰ *outfile;

/* Pointer to the end of the prefix in OUTFILE.
   Suffixes are inserted here.  */
ਸਥਿਰ ਅੱਖਰ *outfile_mid;

/* Generate new suffix when suffixes are exhausted.  */
ਸਥਿਰ bool suffix_auto = true;

/* Length of OUTFILE's suffix.  */
ਸਥਿਰ idx_t suffix_ਲੰਮਾਈ;

/* Alphabet of characters to use in suffix.  */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *suffix_alphabet = "abcdefghijklmnopqrstuvwxyz";

/* Numerical suffix start value.  */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *numeric_suffix_ਸ਼ੁਰੂ;

/* Additional suffix to append to output file names.  */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *additional_suffix;

/* Name of input file.  May be "-".  */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *infile = "-";

/* stat buf for input file.  */
ਸਥਿਰ ਬਣਤਰ stat ਵਿੱਚ_stat_buf;

/* Descriptor on which output file is open.  */
ਸਥਿਰ ਪੂਰਨ output_desc = -1;

/* If true, print a diagnostic on standard error just before each
   output file is opened. */
ਸਥਿਰ bool verbose;

/* If true, don't generate zero length output files. */
ਸਥਿਰ bool elide_ਖਾਲੀ_ਥਾਵਾਂ_files;

/* If true, in round robin mode, immediately copy
   input to output, which is much slower, so disabled by default.  */
ਸਥਿਰ bool unbuffered;

/* The character marking end of line.  Defaults to \n below.  */
ਸਥਿਰ ਪੂਰਨ eolchar = -1;

/* The split mode to use.  */
ਗਿਣਤੀ ਵੰਡੋ_type
{
  type_undef, type_ਬਾਈਟਾਂ, type_byteslines, type_ਸਤਰਾਂ, type_digits,
  type_chunk_ਬਾਈਟਾਂ, type_chunk_ਸਤਰਾਂ, type_rr
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ਗਿਣਤੀ
{
  VERBOSE_OPTION = CHAR_MAX + 1,
  FILTER_OPTION,
  IO_BLKSIZE_OPTION,
  ADDITIONAL_SUFFIX_OPTION
};

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ longopts[] =
{
  {"bytes", required_ਦਲੀਲ, ਨਿਰਾ, 'b'},
  {"lines", required_ਦਲੀਲ, ਨਿਰਾ, 'l'},
  {"line-bytes", required_ਦਲੀਲ, ਨਿਰਾ, 'C'},
  {"number", required_ਦਲੀਲ, ਨਿਰਾ, 'n'},
  {"elide-empty-files", no_ਦਲੀਲ, ਨਿਰਾ, 'e'},
  {"unbuffered", no_ਦਲੀਲ, ਨਿਰਾ, 'u'},
  {"suffix-length", required_ਦਲੀਲ, ਨਿਰਾ, 'a'},
  {"additional-suffix", required_ਦਲੀਲ, ਨਿਰਾ,
   ADDITIONAL_SUFFIX_OPTION},
  {"numeric-suffixes", optional_ਦਲੀਲ, ਨਿਰਾ, 'd'},
  {"hex-suffixes", optional_ਦਲੀਲ, ਨਿਰਾ, 'x'},
  {"filter", required_ਦਲੀਲ, ਨਿਰਾ, FILTER_OPTION},
  {"verbose", no_ਦਲੀਲ, ਨਿਰਾ, VERBOSE_OPTION},
  {"separator", required_ਦਲੀਲ, ਨਿਰਾ, 't'},
  {"-io-blksize", required_ਦਲੀਲ, ਨਿਰਾ,
   IO_BLKSIZE_OPTION}, /* do not document */
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};

/* Return true if the errno value, ERR, is ignorable.  */
ਸਥਿਰ ਸਤਰਅੰਦਰ bool
ignorable (ਪੂਰਨ err)
{
  ਵਾਪਸ filter_command && err == EPIPE;
}

ਸਥਿਰ ਖਾਲੀ
ਸੈੱਟ_suffix_ਲੰਮਾਈ (intmax_t n_units, ਗਿਣਤੀ ਵੰਡੋ_type ਵੰਡੋ_type_2)
{
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEFAULT_SUFFIX_ਲੰਮਾਈ 2

  ਪੂਰਨ suffix_ਲੰਮਾਈ_needed = 0;

  /* The suffix auto length feature is incompatible with
     a user specified start value as the generated suffixes
     are not all consecutive.  */
  ਜੇ (numeric_suffix_ਸ਼ੁਰੂ)
    suffix_auto = false;

  /* Auto-calculate the suffix length if the number of files is given.  */
  ਜੇ (ਵੰਡੋ_type_2 == type_chunk_ਬਾਈਟਾਂ || ਵੰਡੋ_type_2 == type_chunk_ਸਤਰਾਂ
      || ਵੰਡੋ_type_2 == type_rr)
    {
      intmax_t n_units_ਸਿਰਾ = n_units - 1;
      ਜੇ (numeric_suffix_ਸ਼ੁਰੂ)
        {
          intmax_t n_ਸ਼ੁਰੂ;
          strtol_error e = xstrtoimax (numeric_suffix_ਸ਼ੁਰੂ, ਨਿਰਾ, 10,
                                       &n_ਸ਼ੁਰੂ, "");
          ਜੇ (e == LONGINT_OK && n_ਸ਼ੁਰੂ < n_units)
            {
              /* Restrict auto adjustment so we don't keep
                 incrementing a suffix size arbitrarily,
                 as that would break sort order for files
                 generated from multiple split runs.  */
              ਜੇ (ckd_ਜੋੜੋ (&n_units_ਸਿਰਾ, n_units_ਸਿਰਾ, n_ਸ਼ੁਰੂ))
                n_units_ਸਿਰਾ = INTMAX_MAX;
            }

        }
      idx_t alphabet_len = ਲੜੀ_ਲੰਬਾਈ (suffix_alphabet);
      ਕਰੋ
        suffix_ਲੰਮਾਈ_needed++;
      ਜਦੋਂ (n_units_ਸਿਰਾ /= alphabet_len);

      suffix_auto = false;
    }

  ਜੇ (suffix_ਲੰਮਾਈ)            /* set by user */
    {
      ਜੇ (suffix_ਲੰਮਾਈ < suffix_ਲੰਮਾਈ_needed)
        error (EXIT_FAILURE, 0,
               _("the suffix length needs to be at least %d"),
               suffix_ਲੰਮਾਈ_needed);
      suffix_auto = false;
      ਵਾਪਸ;
    }
  ਨਹੀਂਤਾਂ
    suffix_ਲੰਮਾਈ = MAX (DEFAULT_SUFFIX_ਲੰਮਾਈ, suffix_ਲੰਮਾਈ_needed);
}

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("\
Usage: %s [OPTION]... [FILE [ਅਗੇਤਰ_2]]\n\
"),
              ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
Output pieces ਦਾ FILE ਮੰਜ਼ਿਲ PREFIXaa, PREFIXab, ...;\n\
ਮੂਲ ਮਾਪ is 1000 ਸਤਰਾਂ, and ਮੂਲ ਅਗੇਤਰ_2 is 'x'.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oprintf (_("\
  -a, --suffix-ਲੰਮਾਈ=N\n\
         generate suffixes ਦਾ ਲੰਮਾਈ N (ਮੂਲ %d)\n\
"), DEFAULT_SUFFIX_ਲੰਮਾਈ);
      oputs (_("\
      --additional-suffix=SUFFIX\n\
         append an additional SUFFIX ਮੰਜ਼ਿਲ file ਨਾਂ\n\
"));
      oputs (_("\
  -b, --ਬਾਈਟਾਂ=ਮਾਪ_2\n\
         put ਮਾਪ_2 ਬਾਈਟਾਂ ਪ੍ਰਤੀ output file\n\
"));
      oputs (_("\
  -C, --ਸਤਰ-ਬਾਈਟਾਂ=ਮਾਪ_2\n\
         put at most ਮਾਪ_2 ਬਾਈਟਾਂ ਦਾ ਰਿਕਾਰਡਾਂ ਪ੍ਰਤੀ output file\n\
"));
      oputs (_("\
  -d\n\
         use numeric suffixes starting at 0, not alphabetic\n\
"));
      oputs (_("\
      --numeric-suffixes[=FROM]\n\
         same as -d, but allow setting the ਸ਼ੁਰੂ ਮੁੱਲ\n\
"));
      oputs (_("\
  -x\n\
         use hex suffixes starting at 0, not alphabetic\n\
"));
      oputs (_("\
      --hex-suffixes[=FROM]\n\
         same as -x, but allow setting the ਸ਼ੁਰੂ ਮੁੱਲ\n\
"));
      oputs (_("\
  -e, --elide-ਖਾਲੀ_ਥਾਵਾਂ-files\n\
         ਕਰੋ not generate ਖਾਲੀ_ਥਾਵਾਂ output files with '-n'\n\
"));
      oputs (_("\
      --filter=COMMAND\n\
         ਲਿਖ ਮੰਜ਼ਿਲ shell COMMAND; file name is $FILE\n\
"));
      oputs (_("\
  -l, --ਸਤਰਾਂ=ਸੰਖਿਆ_2\n\
         put ਸੰਖਿਆ_2 ਸਤਰਾਂ/ਰਿਕਾਰਡਾਂ ਪ੍ਰਤੀ output file\n\
"));
      oputs (_("\
  -n, --ਸੰਖਿਆ=CHUNKS\n\
         generate CHUNKS output files; see explanation ਹੇਠਾਂ\n\
"));
      oputs (_("\
  -t, --separator=SEP\n\
         use SEP instead ਦਾ newline as the ਰਿਕਾਰਡ separator;\n\
         '\\0' (zero) specifies the NUL character\n\
"));
      oputs (_("\
  -u, --unbuffered\n\
         immediately copy ਨਿਵੇਸ਼ ਮੰਜ਼ਿਲ output with '-n r/...'\n\
"));
      oputs (_("\
      --verbose\n\
         print a diagnostic just before each output file is opened\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      emit_ਮਾਪ_note ();
      fputs (_("\n\
CHUNKS may be:\n\
  N       ਵੰਡੋ into N files based on ਮਾਪ ਦਾ ਨਿਵੇਸ਼\n\
  K/N     output Kth ਦਾ N ਮੰਜ਼ਿਲ standard output\n\
  l/N     ਵੰਡੋ into N files without splitting ਸਤਰਾਂ/ਰਿਕਾਰਡਾਂ\n\
  l/K/N   output Kth ਦਾ N ਮੰਜ਼ਿਲ standard output without splitting ਸਤਰਾਂ/ਰਿਕਾਰਡਾਂ\n\
  r/N     like 'l' but use ਗੇੜਾ_2 robin distribution\n\
  r/K/N   likewise but only output Kth ਦਾ N ਮੰਜ਼ਿਲ standard output\n\
"), stdout);
      fputs (_("\n\
-n (except -nr) will ਬਫਰ ਮੰਜ਼ਿਲ $TMPDIR, defaulting ਮੰਜ਼ਿਲ /tmp,\n\
ਜੇ the ਨਿਵੇਸ਼ ਮਾਪ cannot easily be determined.\n\
"), stdout);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

/* Copy the data in FD to a temporary file, then make that file FD.
   Use BUF, of size BUFSIZE, to copy.  Return the number of
   bytes copied, or -1 (setting errno) on error.  */
ਸਥਿਰ off_t
copy_ਮੰਜ਼ਿਲ_tmpfile (ਪੂਰਨ fd, ਅੱਖਰ *buf, idx_t bufsize)
{
  FILE *tmp;
  ਜੇ (!ਆਰਜ਼ੀ_ਧਾਰਾ (&tmp, ਨਿਰਾ))
    ਵਾਪਸ -1;
  off_t copied = 0;
  off_t r;

  ਜਦੋਂ (0 < (r = ਪਠ (fd, buf, bufsize)))
    {
      ਜੇ (fwrite (buf, 1, r, tmp) != r)
        ਵਾਪਸ -1;
      ਜੇ (ckd_ਜੋੜੋ (&copied, copied, r))
        {
          errno = EOVERFLOW;
          ਵਾਪਸ -1;
        }
    }

  ਜੇ (r < 0)
    ਵਾਪਸ r;
  r = dup2 (fileno (tmp), fd);
  ਜੇ (r < 0)
    ਵਾਪਸ r;
  ਜੇ (fclose (tmp) < 0)
    ਵਾਪਸ -1;
  ਵਾਪਸ copied;
}

/* Return the number of bytes that can be read from FD with status ST.
   Store up to the first BUFSIZE bytes of the file's data into BUF,
   and advance the file position by the number of bytes read.  On
   input error, set errno and return -1.  */

ਸਥਿਰ off_t
ਨਿਵੇਸ਼_file_ਮਾਪ (ਪੂਰਨ fd, ਬਣਤਰ stat ਅਚਲ *st, ਅੱਖਰ *buf, idx_t bufsize)
{
  off_t ਮਾਪ = 0;
  ਕਰੋ
    {
      ssize_t n_read = ਪਠ (fd, buf + ਮਾਪ, bufsize - ਮਾਪ);
      ਜੇ (n_read <= 0)
        ਵਾਪਸ n_read < 0 ? n_read : ਮਾਪ;
      ਮਾਪ += n_read;
    }
  ਜਦੋਂ (ਮਾਪ < bufsize);

  off_t cur, ਸਿਰਾ;
  ਜੇ ((usable_st_ਮਾਪ (st) && st->st_ਮਾਪ < ਮਾਪ)
      || (cur = lseek (fd, 0, SEEK_CUR)) < 0
      || cur < ਮਾਪ /* E.g., /dev/zero on GNU/Linux.  */
      || (ਸਿਰਾ = lseek (fd, 0, SEEK_END)) < 0)
    {
      ਅੱਖਰ *tmpbuf = xmalloc (bufsize);
      ਸਿਰਾ = copy_ਮੰਜ਼ਿਲ_tmpfile (fd, tmpbuf, bufsize);
      ਮੁਕਤ (tmpbuf);
      ਜੇ (ਸਿਰਾ < 0)
        ਵਾਪਸ ਸਿਰਾ;
      cur = 0;
    }

  ਜੇ (ਸਿਰਾ == OFF_T_MAX /* E.g., /dev/zero on GNU/Hurd.  */
      || (cur < ਸਿਰਾ && ckd_ਜੋੜੋ (&ਮਾਪ, ਮਾਪ, ਸਿਰਾ - cur)))
    {
      errno = EOVERFLOW;
      ਵਾਪਸ -1;
    }

  ਜੇ (cur < ਸਿਰਾ)
    {
      off_t r = lseek (fd, cur, SEEK_SET);
      ਜੇ (r < 0)
        ਵਾਪਸ r;
    }

  ਵਾਪਸ ਮਾਪ;
}

/* Compute the next sequential output file name and store it into the
   string 'outfile'.  */

ਸਥਿਰ ਖਾਲੀ
ਅਗਲਾ_file_name (ਖਾਲੀ)
{
  /* Index in suffix_alphabet of each character in the suffix.  */
  ਸਥਿਰ idx_t *sufindex;
  ਸਥਿਰ idx_t outbase_ਲੰਮਾਈ;
  ਸਥਿਰ idx_t outfile_ਲੰਮਾਈ;
  ਸਥਿਰ idx_t addsuf_ਲੰਮਾਈ;

  ਜੇ (! outfile)
    {
      bool ਉਛਾਲ, widen;

new_name:
      widen = !! outfile_ਲੰਮਾਈ;

      ਜੇ (! widen)
        {
          /* Allocate and initialize the first file name.  */

          outbase_ਲੰਮਾਈ = ਲੜੀ_ਲੰਬਾਈ (outbase);
          addsuf_ਲੰਮਾਈ = additional_suffix ? ਲੜੀ_ਲੰਬਾਈ (additional_suffix) : 0;
          ਉਛਾਲ = ckd_ਜੋੜੋ (&outfile_ਲੰਮਾਈ, outbase_ਲੰਮਾਈ + addsuf_ਲੰਮਾਈ,
                              suffix_ਲੰਮਾਈ);
        }
      ਨਹੀਂਤਾਂ
        {
          /* Reallocate and initialize a new wider file name.
             We do this by subsuming the unchanging part of
             the generated suffix into the prefix (base), and
             reinitializing the now one longer suffix.  */

          ਉਛਾਲ = ckd_ਜੋੜੋ (&outfile_ਲੰਮਾਈ, outfile_ਲੰਮਾਈ, 2);
          suffix_ਲੰਮਾਈ++;
        }

      idx_t outfile_ਮਾਪ;
      ਉਛਾਲ |= ckd_ਜੋੜੋ (&outfile_ਮਾਪ, outfile_ਲੰਮਾਈ, 1);
      ਜੇ (ਉਛਾਲ)
        xalloc_die ();
      outfile = xirealloc (outfile, outfile_ਮਾਪ);

      ਜੇ (! widen)
        ਸਮ੍ਰਿਤੀ_ਨਕਲ (outfile, outbase, outbase_ਲੰਮਾਈ);
      ਨਹੀਂਤਾਂ
        {
          /* Append the last alphabet character to the file name prefix.  */
          outfile[outbase_ਲੰਮਾਈ] = suffix_alphabet[sufindex[0]];
          outbase_ਲੰਮਾਈ++;
        }

      outfile_mid = outfile + outbase_ਲੰਮਾਈ;
      memset (outfile_mid, suffix_alphabet[0], suffix_ਲੰਮਾਈ);
      ਜੇ (additional_suffix)
        ਸਮ੍ਰਿਤੀ_ਨਕਲ (outfile_mid + suffix_ਲੰਮਾਈ, additional_suffix, addsuf_ਲੰਮਾਈ);
      outfile[outfile_ਲੰਮਾਈ] = 0;

      ਮੁਕਤ (sufindex);
      sufindex = xicalloc (suffix_ਲੰਮਾਈ, ਆਕਾਰ *sufindex);

      ਜੇ (numeric_suffix_ਸ਼ੁਰੂ)
        {
          affirm (! widen);

          /* Update the output file name.  */
          idx_t i = ਲੜੀ_ਲੰਬਾਈ (numeric_suffix_ਸ਼ੁਰੂ);
          ਸਮ੍ਰਿਤੀ_ਨਕਲ (outfile_mid + suffix_ਲੰਮਾਈ - i, numeric_suffix_ਸ਼ੁਰੂ, i);

          /* Update the suffix index.  */
          idx_t *sufindex_ਸਿਰਾ = sufindex + suffix_ਲੰਮਾਈ;
          ਜਦੋਂ (i-- != 0)
            *--sufindex_ਸਿਰਾ = numeric_suffix_ਸ਼ੁਰੂ[i] - '0';
        }

#ਜੇ ! _POSIX_NO_TRUNC && HAVE_PATHCONF && defined _PC_NAME_MAX
      /* POSIX requires that if the output file name is too long for
         its directory, 'split' must fail without creating any files.
         This must be checked for explicitly on operating systems that
         silently truncate file names.  */
      {
        ਅੱਖਰ *dir = dir_name (outfile);
        ਲੰਮਾ name_max = pathconf (dir, _PC_NAME_MAX);
        ਜੇ (0 <= name_max && name_max < ਅਧਾਰ_len (ਆਖਰੀ_component (outfile)))
          error (EXIT_FAILURE, ENAMETOOLONG, "%s", quotef (outfile));
        ਮੁਕਤ (dir);
      }
#ਜੇ_ਅੰਤ
    }
  ਨਹੀਂਤਾਂ
    {
      /* Increment the suffix in place, if possible.  */

      idx_t i = suffix_ਲੰਮਾਈ;
      ਜਦੋਂ (i-- != 0)
        {
          sufindex[i]++;
          ਜੇ (suffix_auto && i == 0 && ! suffix_alphabet[sufindex[0] + 1])
            ਜਾਓ new_name;
          outfile_mid[i] = suffix_alphabet[sufindex[i]];
          ਜੇ (outfile_mid[i])
            ਵਾਪਸ;
          sufindex[i] = 0;
          outfile_mid[i] = suffix_alphabet[sufindex[i]];
        }
      error (EXIT_FAILURE, 0, _("output file suffixes exhausted"));
    }
}

/* Create or truncate a file.  */

ਸਥਿਰ ਪੂਰਨ
create (ਅੱਖਰ ਅਚਲ *name)
{
  ਜੇ (!filter_command)
    {
      ਜੇ (verbose)
        fprintf (stdout, _("creating file %s\n"), quoteaf (name));

      ਪੂਰਨ oflags = O_WRONLY | O_CREAT | O_ਦੁਆਧਾਰੀ;
      ਪੂਰਨ fd = open (name, oflags | O_EXCL, ਬਹੁਲਕ_RW_UGO);
      ਜੇ (0 <= fd || errno != EEXIST)
        ਵਾਪਸ fd;
      fd = open (name, oflags, ਬਹੁਲਕ_RW_UGO);
      ਜੇ (fd < 0)
        ਵਾਪਸ fd;
      ਬਣਤਰ stat ਬਾਹਰਲਾ_stat_buf;
      ਜੇ (fstat (fd, &ਬਾਹਰਲਾ_stat_buf) != 0)
        error (EXIT_FAILURE, errno, _("failed to stat %s"), quoteaf (name));
      ਜੇ (psame_inode (&ਵਿੱਚ_stat_buf, &ਬਾਹਰਲਾ_stat_buf))
        error (EXIT_FAILURE, 0, _("%s would overwrite input; aborting"),
               quoteaf (name));
      ਜੇ (ftruncate (fd, 0) < 0
          && (S_ISREG (ਬਾਹਰਲਾ_stat_buf.st_ਬਹੁਲਕ) || S_TYPEISSHM (&ਬਾਹਰਲਾ_stat_buf)))
        error (EXIT_FAILURE, errno, _("%s: error truncating"), quotef (name));

      ਵਾਪਸ fd;
    }
  ਨਹੀਂਤਾਂ
    {
      ਜੇ (setenv ("FILE", name, 1) != 0)
        error (EXIT_FAILURE, errno,
               _("failed to set FILE environment variable"));
      ਜੇ (verbose)
        fprintf (stdout, _("executing with FILE=%s\n"), quotef (name));

      ਪੂਰਨ ਨਤੀਜਾ;
      ਪੂਰਨ fd_pair[2];
      pid_t ਬੱਚਾ_pid;

      posix_spawnattr_t attr;
      posix_spawn_file_actions_t actions;

      sigset_t ਸੈੱਟ_2;
      sigemptyset (&ਸੈੱਟ_2);
      ਜੇ (default_SIGPIPE)
        sigaddset (&ਸੈੱਟ_2, SIGPIPE);

      ਜੇ (   (ਨਤੀਜਾ = posix_spawnattr_init (&attr))
          || (ਨਤੀਜਾ = posix_spawnattr_setflags (&attr,
                                                 (POSIX_SPAWN_USEVFORK
                                                  | POSIX_SPAWN_SETSIGDEF)))
          || (ਨਤੀਜਾ = posix_spawnattr_setsigdefault (&attr, &ਸੈੱਟ_2))
          || (ਨਤੀਜਾ = posix_spawn_file_actions_init (&actions))
         )
        error (EXIT_FAILURE, ਨਤੀਜਾ, _("posix_spawn initialization failed"));

      ਜੇ (pipe (fd_pair) != 0)
        error (EXIT_FAILURE, errno, _("failed to create pipe"));

      /* We have to close any pipes that were opened during an
         earlier call, otherwise this process will be holding a
         write-pipe that will prevent the earlier process from
         reading an EOF on the corresponding read-pipe.  */
      ਲਈ (ਪੂਰਨ i = 0; i < open_pipes.n_pipes; ++i)
        ਜੇ ((ਨਤੀਜਾ = posix_spawn_file_actions_addclose (&actions,
                                                         open_pipes.pipes[i])))
          ਰੋਕੋ;

      ਜੇ (   ਨਤੀਜਾ
          || (ਨਤੀਜਾ = posix_spawn_file_actions_addclose (&actions, fd_pair[1]))
          || (fd_pair[0] != STDIN_FILENO
              && (   (ਨਤੀਜਾ = posix_spawn_file_actions_adddup2 (&actions,
                                                                 fd_pair[0],
                                                                 STDIN_FILENO))
                  || (ਨਤੀਜਾ = posix_spawn_file_actions_addclose (&actions,
                                                                  fd_pair[0]))))
         )
        error (EXIT_FAILURE, ਨਤੀਜਾ, _("posix_spawn setup failed"));


      ਅੱਖਰ ਅਚਲ *shell_prog = getenv ("SHELL");
      ਜੇ (shell_prog == ਨਿਰਾ)
        shell_prog = "/bin/sh";
      ਅੱਖਰ ਅਚਲ *ਅਚਲ argv[] = { ਆਖਰੀ_component (shell_prog), "-c",
                                   filter_command, ਨਿਰਾ };

      ਨਤੀਜਾ = posix_spawn (&ਬੱਚਾ_pid, shell_prog, &actions, &attr,
                            (ਅੱਖਰ * ਅਚਲ *) argv, environ);
      ਜੇ (ਨਤੀਜਾ != 0)
        error (EXIT_FAILURE, errno, _("failed to run command: \"%s -c %s\""),
               shell_prog, filter_command);

      posix_spawnattr_destroy (&attr);
      posix_spawn_file_actions_destroy (&actions);

      ਜੇ (close (fd_pair[0]) != 0)
        error (EXIT_FAILURE, errno, _("failed to close input pipe"));
      filter_pid = ਬੱਚਾ_pid;
      ਜੇ (open_pipes.n_pipes == open_pipes.n_pipes_alloc)
        open_pipes.pipes = xpalloc (open_pipes.pipes, &open_pipes.n_pipes_alloc,
                                    1, MIN (ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ, IDX_MAX),
                                    ਆਕਾਰ *open_pipes.pipes);
      idx_t ਅਚਲ pipe_idx = open_pipes.n_pipes++;
      open_pipes.pipes[pipe_idx] = fd_pair[1];
      ਵਾਪਸ fd_pair[1];
    }
}

/* Close the output file, and do any associated cleanup.
   If FP and FD are both specified, they refer to the same open file;
   in this case FP is closed, but FD is still used in cleanup.  */
ਸਥਿਰ ਖਾਲੀ
closeout (FILE *fp, ਪੂਰਨ fd, pid_t pid, ਅੱਖਰ ਅਚਲ *name)
{
  ਜੇ (fp != ਨਿਰਾ && fclose (fp) != 0 && ! ignorable (errno))
    error (EXIT_FAILURE, errno, "%s", quotef (name));
  ਜੇ (fd >= 0)
    {
      ਜੇ (fp == ਨਿਰਾ && close (fd) < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (name));
      ਲਈ (ਪੂਰਨ j = 0; j < open_pipes.n_pipes; ++j)
        {
          ਜੇ (open_pipes.pipes[j] == fd)
            {
              idx_t ਅਚਲ ਆਖਰੀ_idx = open_pipes.n_pipes - 1;
              open_pipes.pipes[j] = open_pipes.pipes[ਆਖਰੀ_idx];
              open_pipes.n_pipes--;
              ਰੋਕੋ;
            }
        }
    }
  ਜੇ (pid > 0)
    {
      ਪੂਰਨ wstatus;
      ਜੇ (waitpid (pid, &wstatus, 0) < 0)
        error (EXIT_FAILURE, errno, _("waiting for child process"));
      ਨਹੀਂਤਾਂ ਜੇ (WIFSIGNALED (wstatus))
        {
          ਪੂਰਨ sig = WTERMSIG (wstatus);
          ਜੇ (sig != SIGPIPE)
            {
              ਅੱਖਰ signame[MAX (SIG2STR_MAX, INT_BUFSIZE_BOUND (ਪੂਰਨ))];
              ਜੇ (sig2str (sig, signame) != 0)
                sprintf (signame, "%d", sig);
              error (sig + 128, 0,
                     _("with FILE=%s, signal %s from command: %s"),
                     quotef (name), signame, filter_command);
            }
        }
      ਨਹੀਂਤਾਂ ਜੇ (WIFEXITED (wstatus))
        {
          ਪੂਰਨ ex = WEXITSTATUS (wstatus);
          ਜੇ (ex != 0)
            error (ex, 0, _("with FILE=%s, exit %d from command: %s"),
                   quotef (name), ex, filter_command);
        }
      ਨਹੀਂਤਾਂ
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

ਸਥਿਰ bool
cwrite (bool new_file_flag, ਅੱਖਰ ਅਚਲ *bp, idx_t ਬਾਈਟਾਂ)
{
  ਜੇ (new_file_flag)
    {
      ਜੇ (!bp && ਬਾਈਟਾਂ == 0 && elide_ਖਾਲੀ_ਥਾਵਾਂ_files)
        ਵਾਪਸ true;
      closeout (ਨਿਰਾ, output_desc, filter_pid, outfile);
      ਅਗਲਾ_file_name ();
      output_desc = create (outfile);
      ਜੇ (output_desc < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (outfile));
    }

  ਜੇ (ਭਰਿਆ_write (output_desc, bp, ਬਾਈਟਾਂ) == ਬਾਈਟਾਂ)
    ਵਾਪਸ true;
  ਨਹੀਂਤਾਂ
    {
      ਜੇ (! ignorable (errno))
        error (EXIT_FAILURE, errno, "%s", quotef (outfile));
      ਵਾਪਸ false;
    }
}

/* Split into pieces of exactly N_BYTES bytes.
   However, the first REM_BYTES pieces should be 1 byte longer.
   Use buffer BUF, whose size is BUFSIZE.
   If INITIAL_READ is nonnegative,
   BUF contains the first INITIAL_READ input bytes.  */

ਸਥਿਰ ਖਾਲੀ
ਬਾਈਟਾਂ_ਵੰਡੋ (intmax_t n_ਬਾਈਟਾਂ, intmax_t rem_ਬਾਈਟਾਂ,
             ਅੱਖਰ *buf, idx_t bufsize, ssize_t initial_read,
             intmax_t max_files)
{
  bool new_file_flag = true;
  bool filter_ok = true;
  intmax_t opened = 0;
  intmax_t ਮੰਜ਼ਿਲ_write = n_ਬਾਈਟਾਂ + (0 < rem_ਬਾਈਟਾਂ);
  bool eof = ! ਮੰਜ਼ਿਲ_write;

  ਜਦੋਂ (! eof)
    {
      ssize_t n_read;
      ਜੇ (0 <= initial_read)
        {
          n_read = initial_read;
          initial_read = -1;
          eof = n_read < bufsize;
        }
      ਨਹੀਂਤਾਂ
        {
          ਜੇ (! filter_ok
              && 0 <= lseek (STDIN_FILENO, ਮੰਜ਼ਿਲ_write, SEEK_CUR))
            {
              ਮੰਜ਼ਿਲ_write = n_ਬਾਈਟਾਂ + (opened + 1 < rem_ਬਾਈਟਾਂ);
              new_file_flag = true;
            }

          n_read = ਪਠ (STDIN_FILENO, buf, bufsize);
          ਜੇ (n_read < 0)
            error (EXIT_FAILURE, errno, "%s", quotef (infile));
          eof = n_read == 0;
        }
      ਅੱਖਰ *bp_ਬਾਹਰਲਾ = buf;
      ਜਦੋਂ (0 < ਮੰਜ਼ਿਲ_write && ਮੰਜ਼ਿਲ_write <= n_read)
        {
          ਜੇ (filter_ok || new_file_flag)
            filter_ok = cwrite (new_file_flag, bp_ਬਾਹਰਲਾ, ਮੰਜ਼ਿਲ_write);
          opened += new_file_flag;
          new_file_flag = !max_files || (opened < max_files);
          ਜੇ (! filter_ok && ! new_file_flag)
            {
              /* If filters no longer accepting input, stop reading.  */
              n_read = 0;
              eof = true;
              ਰੋਕੋ;
            }
          bp_ਬਾਹਰਲਾ += ਮੰਜ਼ਿਲ_write;
          n_read -= ਮੰਜ਼ਿਲ_write;
          ਮੰਜ਼ਿਲ_write = n_ਬਾਈਟਾਂ + (opened < rem_ਬਾਈਟਾਂ);
        }
      ਜੇ (0 < n_read)
        {
          ਜੇ (filter_ok || new_file_flag)
            filter_ok = cwrite (new_file_flag, bp_ਬਾਹਰਲਾ, n_read);
          opened += new_file_flag;
          new_file_flag = false;
          ਜੇ (! filter_ok && opened == max_files)
            {
              /* If filters no longer accepting input, stop reading.  */
              ਰੋਕੋ;
            }
          ਮੰਜ਼ਿਲ_write -= n_read;
        }
    }

  /* Ensure NUMBER files are created, which truncates
     any existing files or notifies any consumers on fifos.
     FIXME: Should we do this before EXIT_FAILURE?  */
  ਜਦੋਂ (opened++ < max_files)
    cwrite (true, ਨਿਰਾ, 0);
}

/* Split into pieces of exactly N_LINES lines.
   Use buffer BUF, whose size is BUFSIZE.  */

ਸਥਿਰ ਖਾਲੀ
ਸਤਰਾਂ_ਵੰਡੋ (intmax_t n_ਸਤਰਾਂ, ਅੱਖਰ *buf, idx_t bufsize)
{
  ssize_t n_read;
  ਅੱਖਰ *bp, *bp_ਬਾਹਰਲਾ, *eob;
  bool new_file_flag = true;
  intmax_t n = 0;

  ਕਰੋ
    {
      n_read = ਪਠ (STDIN_FILENO, buf, bufsize);
      ਜੇ (n_read < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
      bp = bp_ਬਾਹਰਲਾ = buf;
      eob = bp + n_read;
      *eob = eolchar;
      ਜਦੋਂ (true)
        {
          bp = rawmemchr (bp, eolchar);
          ਜੇ (bp == eob)
            {
              ਜੇ (eob != bp_ਬਾਹਰਲਾ) /* do not write 0 bytes! */
                {
                  idx_t len = eob - bp_ਬਾਹਰਲਾ;
                  cwrite (new_file_flag, bp_ਬਾਹਰਲਾ, len);
                  new_file_flag = false;
                }
              ਰੋਕੋ;
            }

          ++bp;
          ਜੇ (++n >= n_ਸਤਰਾਂ)
            {
              cwrite (new_file_flag, bp_ਬਾਹਰਲਾ, bp - bp_ਬਾਹਰਲਾ);
              bp_ਬਾਹਰਲਾ = bp;
              new_file_flag = true;
              n = 0;
            }
        }
    }
  ਜਦੋਂ (n_read);
}

/* Split into pieces that are as large as possible while still not more
   than N_BYTES bytes, and are split on line boundaries except
   where lines longer than N_BYTES bytes occur. */

ਸਥਿਰ ਖਾਲੀ
ਸਤਰ_ਬਾਈਟਾਂ_ਵੰਡੋ (intmax_t n_ਬਾਈਟਾਂ, ਅੱਖਰ *buf, idx_t bufsize)
{
  ssize_t n_read;
  intmax_t n_ਬਾਹਰਲਾ = 0;      /* for each split.  */
  idx_t n_hold = 0;
  ਅੱਖਰ *hold = ਨਿਰਾ;        /* for lines > bufsize.  */
  idx_t hold_ਮਾਪ = 0;
  bool ਵੰਡੋ_ਸਤਰ = false;  /* Whether a \n was output in a split.  */

  ਕਰੋ
    {
      n_read = ਪਠ (STDIN_FILENO, buf, bufsize);
      ਜੇ (n_read < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
      idx_t n_ਖੱਬਾ = n_read;
      ਅੱਖਰ *sob = buf;
      ਜਦੋਂ (n_ਖੱਬਾ)
        {
          idx_t ਵੰਡੋ_rest = 0;
          ਅੱਖਰ *eoc = ਨਿਰਾ;
          ਅੱਖਰ *eol;

          /* Determine End Of Chunk and/or End of Line,
             which are used below to select what to write or buffer.  */
          ਜੇ (n_ਬਾਈਟਾਂ - n_ਬਾਹਰਲਾ - n_hold <= n_ਖੱਬਾ)
            {
              /* Have enough for split.  */
              ਵੰਡੋ_rest = n_ਬਾਈਟਾਂ - n_ਬਾਹਰਲਾ - n_hold;
              eoc = sob + ਵੰਡੋ_rest - 1;
              eol = memrchr (sob, eolchar, ਵੰਡੋ_rest);
            }
          ਨਹੀਂਤਾਂ
            eol = memrchr (sob, eolchar, n_ਖੱਬਾ);

          /* Output hold space if possible.  */
          ਜੇ (n_hold && !(!eol && n_ਬਾਹਰਲਾ))
            {
              cwrite (n_ਬਾਹਰਲਾ == 0, hold, n_hold);
              n_ਬਾਹਰਲਾ += n_hold;
              n_hold = 0;
            }

          /* Output to eol if present.  */
          ਜੇ (eol)
            {
              ਵੰਡੋ_ਸਤਰ = true;
              idx_t n_write = eol - sob + 1;
              cwrite (n_ਬਾਹਰਲਾ == 0, sob, n_write);
              n_ਬਾਹਰਲਾ += n_write;
              n_ਖੱਬਾ -= n_write;
              sob += n_write;
              ਜੇ (eoc)
                ਵੰਡੋ_rest -= n_write;
            }

          /* Output to eoc or eob if possible.  */
          ਜੇ (n_ਖੱਬਾ && !ਵੰਡੋ_ਸਤਰ)
            {
              idx_t n_write = eoc ? ਵੰਡੋ_rest : n_ਖੱਬਾ;
              cwrite (n_ਬਾਹਰਲਾ == 0, sob, n_write);
              n_ਬਾਹਰਲਾ += n_write;
              n_ਖੱਬਾ -= n_write;
              sob += n_write;
              ਜੇ (eoc)
                ਵੰਡੋ_rest -= n_write;
            }

          /* Update hold if needed.  */
          ਜੇ ((eoc && ਵੰਡੋ_rest) || (!eoc && n_ਖੱਬਾ))
            {
              idx_t n_buf = eoc ? ਵੰਡੋ_rest : n_ਖੱਬਾ;
              ਜੇ (hold_ਮਾਪ - n_hold < n_buf)
                hold = xpalloc (hold, &hold_ਮਾਪ, n_buf - (hold_ਮਾਪ - n_hold),
                                -1, ਆਕਾਰ *hold);
              ਸਮ੍ਰਿਤੀ_ਨਕਲ (hold + n_hold, sob, n_buf);
              n_hold += n_buf;
              n_ਖੱਬਾ -= n_buf;
              sob += n_buf;
            }

          /* Reset for new split.  */
          ਜੇ (eoc)
            {
              n_ਬਾਹਰਲਾ = 0;
              ਵੰਡੋ_ਸਤਰ = false;
            }
        }
    }
  ਜਦੋਂ (n_read);

  /* Handle no eol at end of file.  */
  ਜੇ (n_hold)
    cwrite (n_ਬਾਹਰਲਾ == 0, hold, n_hold);

  ਮੁਕਤ (hold);
}

/* -n l/[K/]N: Write lines to files of approximately file size / N.
   The file is partitioned into file size / N sized portions, with the
   last assigned any excess.  If a line _starts_ within a partition
   it is written completely to the corresponding file.  Since lines
   are not split even if they overlap a partition, the files written
   can be larger or smaller than the partition size, and even empty
   if a line is so long as to completely overlap the partition.  */

ਸਥਿਰ ਖਾਲੀ
ਸਤਰਾਂ_chunk_ਵੰਡੋ (intmax_t k, intmax_t n, ਅੱਖਰ *buf, idx_t bufsize,
                   ssize_t initial_read, off_t file_ਮਾਪ)
{
  affirm (n && k <= n);

  intmax_t rem_ਬਾਈਟਾਂ = file_ਮਾਪ % n;
  off_t chunk_ਮਾਪ = file_ਮਾਪ / n;
  intmax_t chunk_no = 1;
  off_t chunk_ਸਿਰਾ = chunk_ਮਾਪ + (0 < rem_ਬਾਈਟਾਂ);
  off_t n_written = 0;
  bool new_file_flag = true;
  bool chunk_truncated = false;

  ਜੇ (k > 1 && 0 < file_ਮਾਪ)
    {
      /* Start reading 1 byte before kth chunk of file.  */
      off_t ਸ਼ੁਰੂ = (k - 1) * chunk_ਮਾਪ + MIN (k - 1, rem_ਬਾਈਟਾਂ) - 1;
      ਜੇ (ਸ਼ੁਰੂ < initial_read)
        {
          memmove (buf, buf + ਸ਼ੁਰੂ, initial_read - ਸ਼ੁਰੂ);
          initial_read -= ਸ਼ੁਰੂ;
        }
      ਨਹੀਂਤਾਂ
        {
          ਜੇ (initial_read < ਸ਼ੁਰੂ
              && lseek (STDIN_FILENO, ਸ਼ੁਰੂ - initial_read, SEEK_CUR) < 0)
            error (EXIT_FAILURE, errno, "%s", quotef (infile));
          initial_read = -1;
        }
      n_written = ਸ਼ੁਰੂ;
      chunk_no = k - 1;
      chunk_ਸਿਰਾ = ਸ਼ੁਰੂ + 1;
    }

  ਜਦੋਂ (n_written < file_ਮਾਪ)
    {
      ਅੱਖਰ *bp = buf, *eob;
      ssize_t n_read;
      ਜੇ (0 <= initial_read)
        {
          n_read = initial_read;
          initial_read = -1;
        }
      ਨਹੀਂਤਾਂ
        {
          n_read = ਪਠ (STDIN_FILENO, buf,
                         MIN (bufsize, file_ਮਾਪ - n_written));
          ਜੇ (n_read < 0)
            error (EXIT_FAILURE, errno, "%s", quotef (infile));
        }
      ਜੇ (n_read == 0)
        ਰੋਕੋ; /* eof.  */
      chunk_truncated = false;
      eob = buf + n_read;

      ਜਦੋਂ (bp != eob)
        {
          idx_t ਮੰਜ਼ਿਲ_write;
          bool ਅਗਲਾ = false;

          /* Begin looking for '\n' at last byte of chunk.  */
          off_t skip = MIN (n_read, MAX (0, chunk_ਸਿਰਾ - 1 - n_written));
          ਅੱਖਰ *bp_ਬਾਹਰਲਾ = memchr (bp + skip, eolchar, n_read - skip);
          ਜੇ (bp_ਬਾਹਰਲਾ)
            {
              bp_ਬਾਹਰਲਾ++;
              ਅਗਲਾ = true;
            }
          ਨਹੀਂਤਾਂ
            bp_ਬਾਹਰਲਾ = eob;
          ਮੰਜ਼ਿਲ_write = bp_ਬਾਹਰਲਾ - bp;

          ਜੇ (k == chunk_no)
            {
              /* We don't use the stdout buffer here since we're writing
                 large chunks from an existing file, so it's more efficient
                 to write out directly.  */
              ਜੇ (ਭਰਿਆ_write (STDOUT_FILENO, bp, ਮੰਜ਼ਿਲ_write) != ਮੰਜ਼ਿਲ_write)
                write_error ();
            }
          ਨਹੀਂਤਾਂ ਜੇ (! k)
            cwrite (new_file_flag, bp, ਮੰਜ਼ਿਲ_write);
          n_written += ਮੰਜ਼ਿਲ_write;
          bp += ਮੰਜ਼ਿਲ_write;
          n_read -= ਮੰਜ਼ਿਲ_write;
          new_file_flag = ਅਗਲਾ;

          /* A line could have been so long that it skipped
             entire chunks. So create empty files in that case.  */
          ਜਦੋਂ (ਅਗਲਾ || chunk_ਸਿਰਾ <= n_written)
            {
              ਜੇ (!ਅਗਲਾ && bp == eob)
                {
                  /* replenish buf, before going to next chunk.  */
                  chunk_truncated = true;
                  ਰੋਕੋ;
                }
              ਜੇ (k == chunk_no)
                ਵਾਪਸ;
              chunk_ਸਿਰਾ += chunk_ਮਾਪ + (chunk_no < rem_ਬਾਈਟਾਂ);
              chunk_no++;
              ਜੇ (chunk_ਸਿਰਾ <= n_written)
                {
                  ਜੇ (! k)
                    cwrite (true, ਨਿਰਾ, 0);
                }
              ਨਹੀਂਤਾਂ
                ਅਗਲਾ = false;
            }
        }
    }

  ਜੇ (chunk_truncated)
    chunk_no++;

  /* Ensure NUMBER files are created, which truncates
     any existing files or notifies any consumers on fifos.
     FIXME: Should we do this before EXIT_FAILURE?  */
  ਜੇ (!k)
    ਜਦੋਂ (chunk_no++ <= n)
      cwrite (true, ਨਿਰਾ, 0);
}

/* -n K/N: Extract Kth of N chunks.  */

ਸਥਿਰ ਖਾਲੀ
ਬਾਈਟਾਂ_chunk_ਕੱਢੋ (intmax_t k, intmax_t n, ਅੱਖਰ *buf, idx_t bufsize,
                     ssize_t initial_read, off_t file_ਮਾਪ)
{
  off_t ਸ਼ੁਰੂ;
  off_t ਸਿਰਾ;

  affirm (0 < k && k <= n);

  ਸ਼ੁਰੂ = (k - 1) * (file_ਮਾਪ / n) + MIN (k - 1, file_ਮਾਪ % n);
  ਸਿਰਾ = k == n ? file_ਮਾਪ : k * (file_ਮਾਪ / n) + MIN (k, file_ਮਾਪ % n);

  ਜੇ (ਸ਼ੁਰੂ < initial_read)
    {
      memmove (buf, buf + ਸ਼ੁਰੂ, initial_read - ਸ਼ੁਰੂ);
      initial_read -= ਸ਼ੁਰੂ;
    }
  ਨਹੀਂਤਾਂ
    {
      ਜੇ (initial_read < ਸ਼ੁਰੂ
          && lseek (STDIN_FILENO, ਸ਼ੁਰੂ - initial_read, SEEK_CUR) < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
      initial_read = -1;
    }

  ਜਦੋਂ (ਸ਼ੁਰੂ < ਸਿਰਾ)
    {
      ssize_t n_read;
      ਜੇ (0 <= initial_read)
        {
          n_read = initial_read;
          initial_read = -1;
        }
      ਨਹੀਂਤਾਂ
        {
          n_read = ਪਠ (STDIN_FILENO, buf, bufsize);
          ਜੇ (n_read < 0)
            error (EXIT_FAILURE, errno, "%s", quotef (infile));
        }
      ਜੇ (n_read == 0)
        ਰੋਕੋ; /* eof.  */
      n_read = MIN (n_read, ਸਿਰਾ - ਸ਼ੁਰੂ);
      ਜੇ (ਭਰਿਆ_write (STDOUT_FILENO, buf, n_read) != n_read
          && ! ignorable (errno))
        error (EXIT_FAILURE, errno, "%s", quotef ("-"));
      ਸ਼ੁਰੂ += n_read;
    }
}

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ਦਾ_info
{
  ਅੱਖਰ *ਦਾ_name;
  ਪੂਰਨ ofd;
  FILE *ofile;
  pid_t opid;
} ਦਾ_t;

ਗਿਣਤੀ
{
  OFD_NEW = -1,
  OFD_APPEND = -2
};

/* Rotate file descriptors when we're writing to more output files than we
   have available file descriptors.
   Return whether we came under file resource pressure.
   If so, it's probably best to close each file when finished with it.  */

ਸਥਿਰ bool
ofile_open (ਦਾ_t *files, idx_t i_check, idx_t nfiles)
{
  bool file_ਹੱਦਬੰਦੀ = false;

  ਜੇ (files[i_check].ofd <= OFD_NEW)
    {
      ਪੂਰਨ fd;
      idx_t i_reopen = i_check ? i_check - 1 : nfiles - 1;

      /* Another process could have opened a file in between the calls to
         close and open, so we should keep trying until open succeeds or
         we've closed all of our files.  */
      ਜਦੋਂ (true)
        {
          ਜੇ (files[i_check].ofd == OFD_NEW)
            fd = create (files[i_check].ਦਾ_name);
          ਨਹੀਂਤਾਂ /* OFD_APPEND  */
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
              fd = open (files[i_check].ਦਾ_name,
                         O_WRONLY | O_ਦੁਆਧਾਰੀ | O_APPEND | O_NONBLOCK);
            }

          ਜੇ (0 <= fd)
            ਰੋਕੋ;

          ਜੇ (!(errno == EMFILE || errno == ENFILE))
            error (EXIT_FAILURE, errno, "%s", quotef (files[i_check].ਦਾ_name));

          file_ਹੱਦਬੰਦੀ = true;

          /* Search backwards for an open file to close.  */
          ਜਦੋਂ (files[i_reopen].ofd < 0)
            {
              i_reopen = i_reopen ? i_reopen - 1 : nfiles - 1;
              /* No more open files to close, exit with E[NM]FILE.  */
              ਜੇ (i_reopen == i_check)
                error (EXIT_FAILURE, errno, "%s",
                       quotef (files[i_check].ਦਾ_name));
            }

          ਜੇ (fclose (files[i_reopen].ofile) != 0)
            error (EXIT_FAILURE, errno, "%s", quotef (files[i_reopen].ਦਾ_name));
          files[i_reopen].ofile = ਨਿਰਾ;
          files[i_reopen].ofd = OFD_APPEND;
        }

      files[i_check].ofd = fd;
      FILE *ofile = fdopen (fd, "a");
      ਜੇ (!ofile)
        error (EXIT_FAILURE, errno, "%s", quotef (files[i_check].ਦਾ_name));
      files[i_check].ofile = ofile;
      files[i_check].opid = filter_pid;
      filter_pid = 0;
    }

  ਵਾਪਸ file_ਹੱਦਬੰਦੀ;
}

/* -n r/[K/]N: Divide file into N chunks in round robin fashion.
   Use BUF of size BUFSIZE for the buffer, and if allocating storage
   put its address into *FILESP to pacify -fsanitize=leak.
   When K == 0, we try to keep the files open in parallel.
   If we run out of file resources, then we revert
   to opening and closing each file for each line.  */

ਸਥਿਰ ਖਾਲੀ
ਸਤਰਾਂ_rr (intmax_t k, intmax_t n, ਅੱਖਰ *buf, idx_t bufsize, ਦਾ_t **filesp)
{
  bool wrapped = false;
  bool wrote = false;
  bool file_ਹੱਦਬੰਦੀ;
  idx_t i_file;
  ਦਾ_t *files IF_LINT (= ਨਿਰਾ);
  intmax_t ਸਤਰ_no;

  ਜੇ (k)
    ਸਤਰ_no = 1;
  ਨਹੀਂਤਾਂ
    {
      ਜੇ (IDX_MAX < n)
        xalloc_die ();
      files = *filesp = xinmalloc (n, ਆਕਾਰ *files);

      /* Generate output file names. */
      ਲਈ (i_file = 0; i_file < n; i_file++)
        {
          ਅਗਲਾ_file_name ();
          files[i_file].ਦਾ_name = xstrdup (outfile);
          files[i_file].ofd = OFD_NEW;
          files[i_file].ofile = ਨਿਰਾ;
          files[i_file].opid = 0;
        }
      i_file = 0;
      file_ਹੱਦਬੰਦੀ = false;
    }

  ਜਦੋਂ (true)
    {
      ਅੱਖਰ *bp = buf, *eob;
      ssize_t n_read = ਪਠ (STDIN_FILENO, buf, bufsize);
      ਜੇ (n_read < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
      ਨਹੀਂਤਾਂ ਜੇ (n_read == 0)
        ਰੋਕੋ; /* eof.  */
      eob = buf + n_read;

      ਜਦੋਂ (bp != eob)
        {
          idx_t ਮੰਜ਼ਿਲ_write;
          bool ਅਗਲਾ = false;

          /* Find end of line. */
          ਅੱਖਰ *bp_ਬਾਹਰਲਾ = memchr (bp, eolchar, eob - bp);
          ਜੇ (bp_ਬਾਹਰਲਾ)
            {
              bp_ਬਾਹਰਲਾ++;
              ਅਗਲਾ = true;
            }
          ਨਹੀਂਤਾਂ
            bp_ਬਾਹਰਲਾ = eob;
          ਮੰਜ਼ਿਲ_write = bp_ਬਾਹਰਲਾ - bp;

          ਜੇ (k)
            {
              ਜੇ (ਸਤਰ_no == k && unbuffered)
                {
                  ਜੇ (ਭਰਿਆ_write (STDOUT_FILENO, bp, ਮੰਜ਼ਿਲ_write) != ਮੰਜ਼ਿਲ_write)
                    write_error ();
                }
              ਨਹੀਂਤਾਂ ਜੇ (ਸਤਰ_no == k && fwrite (bp, ਮੰਜ਼ਿਲ_write, 1, stdout) != 1)
                {
                  write_error ();
                }
              ਜੇ (ਅਗਲਾ)
                ਸਤਰ_no = (ਸਤਰ_no == n) ? 1 : ਸਤਰ_no + 1;
            }
          ਨਹੀਂਤਾਂ
            {
              /* Secure file descriptor. */
              file_ਹੱਦਬੰਦੀ |= ofile_open (files, i_file, n);
              ਜੇ (unbuffered)
                {
                  /* Note writing to fd, rather than flushing the FILE gives
                     an 8% performance benefit, due to reduced data copying.  */
                  ਜੇ (ਭਰਿਆ_write (files[i_file].ofd, bp, ਮੰਜ਼ਿਲ_write) != ਮੰਜ਼ਿਲ_write
                      && ! ignorable (errno))
                    error (EXIT_FAILURE, errno, "%s",
                           quotef (files[i_file].ਦਾ_name));
                }
              ਨਹੀਂਤਾਂ ਜੇ (fwrite (bp, ਮੰਜ਼ਿਲ_write, 1, files[i_file].ofile) != 1
                       && ! ignorable (errno))
                error (EXIT_FAILURE, errno, "%s",
                       quotef (files[i_file].ਦਾ_name));

              ਜੇ (! ignorable (errno))
                wrote = true;

              ਜੇ (file_ਹੱਦਬੰਦੀ)
                {
                  ਜੇ (fclose (files[i_file].ofile) != 0)
                    error (EXIT_FAILURE, errno, "%s",
                           quotef (files[i_file].ਦਾ_name));
                  files[i_file].ofile = ਨਿਰਾ;
                  files[i_file].ofd = OFD_APPEND;
                }
              ਜੇ (ਅਗਲਾ && ++i_file == n)
                {
                  wrapped = true;
                  /* If no filters are accepting input, stop reading.  */
                  ਜੇ (! wrote)
                    ਜਾਓ no_filters;
                  wrote = false;
                  i_file = 0;
                }
            }

          bp = bp_ਬਾਹਰਲਾ;
        }
    }

no_filters:
  /* Ensure all files created, so that any existing files are truncated,
     and to signal any waiting fifo consumers.
     Also, close any open file descriptors.
     FIXME: Should we do this before EXIT_FAILURE?  */
  ਜੇ (!k)
    {
      idx_t ceiling = wrapped ? n : i_file;
      ਲਈ (i_file = 0; i_file < n; i_file++)
        {
          ਜੇ (i_file >= ceiling && !elide_ਖਾਲੀ_ਥਾਵਾਂ_files)
            file_ਹੱਦਬੰਦੀ |= ofile_open (files, i_file, n);
          ਜੇ (files[i_file].ofd >= 0)
            closeout (files[i_file].ofile, files[i_file].ofd,
                      files[i_file].opid, files[i_file].ਦਾ_name);
          files[i_file].ofd = OFD_APPEND;
        }
    }
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FAIL_ONLY_ONE_WAY()					\
  ਕਰੋ								\
    {								\
      error (0, 0, _("cannot split in more than one way"));	\
      usage (EXIT_FAILURE);					\
    }								\
  ਜਦੋਂ (0)

/* Report a string-to-integer conversion failure MSGID with ARG.  */

ਸਥਿਰ _Noreturn ਖਾਲੀ
strtoint_die (ਅੱਖਰ ਅਚਲ *msgid, ਅੱਖਰ ਅਚਲ *arg)
{
  error (EXIT_FAILURE, errno == EINVAL ? 0 : errno, "%s: %s",
         gettext (msgid), quote (arg));
}

/* Use OVERFLOW_OK when it is OK to ignore LONGINT_OVERFLOW errors, since the
   extreme value will do the right thing anyway on any practical platform.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਉਛਾਲ_OK LONGINT_ਉਛਾਲ

/* Parse ARG for number of bytes or lines.  The number can be followed
   by MULTIPLIERS, and the resulting value must be positive.
   If the number cannot be parsed, diagnose with MSG.
   Return the number parsed, or an INTMAX_MAX on overflow.  */

ਸਥਿਰ intmax_t
parse_n_units (ਅੱਖਰ ਅਚਲ *arg, ਅੱਖਰ ਅਚਲ *multipliers, ਅੱਖਰ ਅਚਲ *msgid)
{
  intmax_t n;
  ਜੇ (ਉਛਾਲ_OK < xstrtoimax (arg, ਨਿਰਾ, 10, &n, multipliers) || n < 1)
    strtoint_die (msgid, arg);
  ਵਾਪਸ n;
}

/* Parse K/N syntax of chunk options.  */

ਸਥਿਰ ਖਾਲੀ
parse_chunk (intmax_t *k_units, intmax_t *n_units, ਅੱਖਰ ਅਚਲ *arg)
{
  ਅੱਖਰ *argend;
  strtol_error e = xstrtoimax (arg, &argend, 10, n_units, "");
  ਜੇ (e == LONGINT_INVALID_SUFFIX_CHAR && *argend == '/')
    {
      *k_units = *n_units;
      *n_units = parse_n_units (argend + 1, "",
                                N_("invalid number of chunks"));
      ਜੇ (! (0 < *k_units && *k_units <= *n_units))
        error (EXIT_FAILURE, 0, "%s: %s", _("invalid chunk number"),
               quote_mem (arg, argend - arg));
    }
  ਨਹੀਂਤਾਂ ਜੇ (! (e <= ਉਛਾਲ_OK && 0 < *n_units))
    strtoint_die (N_("invalid number of chunks"), arg);
}


ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਗਿਣਤੀ ਵੰਡੋ_type ਵੰਡੋ_type_2 = type_undef;
  idx_t ਵਿੱਚ_blk_ਮਾਪ = 0;	/* optimal block size of input file device */
  idx_t ਪੰਨਾ_ਮਾਪ = getpagesize ();
  intmax_t k_units = 0;
  intmax_t n_units = 0;

  ਸਥਿਰ ਅੱਖਰ ਅਚਲ multipliers[] = "bEGKkMmPQRTYZ0";
  ਪੂਰਨ c;
  ਪੂਰਨ digits_optind = 0;
  off_t file_ਮਾਪ = OFF_T_MAX;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* Parse command line options.  */

  ਜਦੋਂ (true)
    {
      /* This is the argv-index of the option we will read next.  */
      ਪੂਰਨ this_optind = optind ? optind : 1;

      c = getopt_long (argc, argv, "0123456789C:a:b:del:n:t:ux",
                       longopts, ਨਿਰਾ);
      ਜੇ (c == -1)
        ਰੋਕੋ;

      ਚੋਣ (c)
        {
        ਹਾਲਤ 'a':
          suffix_ਲੰਮਾਈ = xdectoimax (optarg, 0, IDX_MAX,
                                      "", _("invalid suffix length"), 0);
          ਰੋਕੋ;

        ਹਾਲਤ ADDITIONAL_SUFFIX_OPTION:
          {
            ਪੂਰਨ suffix_len = ਲੜੀ_ਲੰਬਾਈ (optarg);
            ਜੇ (ਆਖਰੀ_component (optarg) != optarg
                || (suffix_len && ISSLASH (optarg[suffix_len - 1])))
              {
                error (0, 0,
                       _("invalid suffix %s, contains directory separator"),
                       quote (optarg));
                usage (EXIT_FAILURE);
              }
          }
          additional_suffix = optarg;
          ਰੋਕੋ;

        ਹਾਲਤ 'b':
          ਜੇ (ਵੰਡੋ_type_2 != type_undef)
            FAIL_ONLY_ONE_WAY ();
          ਵੰਡੋ_type_2 = type_ਬਾਈਟਾਂ;
          n_units = parse_n_units (optarg, multipliers,
                                   N_("invalid number of bytes"));
          ਰੋਕੋ;

        ਹਾਲਤ 'l':
          ਜੇ (ਵੰਡੋ_type_2 != type_undef)
            FAIL_ONLY_ONE_WAY ();
          ਵੰਡੋ_type_2 = type_ਸਤਰਾਂ;
          n_units = parse_n_units (optarg, "", N_("invalid number of lines"));
          ਰੋਕੋ;

        ਹਾਲਤ 'C':
          ਜੇ (ਵੰਡੋ_type_2 != type_undef)
            FAIL_ONLY_ONE_WAY ();
          ਵੰਡੋ_type_2 = type_byteslines;
          n_units = parse_n_units (optarg, multipliers,
                                   N_("invalid number of lines"));
          ਰੋਕੋ;

        ਹਾਲਤ 'n':
          ਜੇ (ਵੰਡੋ_type_2 != type_undef)
            FAIL_ONLY_ONE_WAY ();
          /* skip any whitespace */
          ਜਦੋਂ (ਖਾਲੀ_ਥਾਂ_ਹੈ (ਮੰਜ਼ਿਲ_uchar (*optarg)))
            optarg++;
          ਜੇ (STRNCMP_LIT (optarg, "r/") == 0)
            {
              ਵੰਡੋ_type_2 = type_rr;
              optarg += 2;
            }
          ਨਹੀਂਤਾਂ ਜੇ (STRNCMP_LIT (optarg, "l/") == 0)
            {
              ਵੰਡੋ_type_2 = type_chunk_ਸਤਰਾਂ;
              optarg += 2;
            }
          ਨਹੀਂਤਾਂ
            ਵੰਡੋ_type_2 = type_chunk_ਬਾਈਟਾਂ;
          parse_chunk (&k_units, &n_units, optarg);
          ਰੋਕੋ;

        ਹਾਲਤ 'u':
          unbuffered = true;
          ਰੋਕੋ;

        ਹਾਲਤ 't':
          {
            ਅੱਖਰ neweol = optarg[0];
            ਜੇ (! neweol)
              error (EXIT_FAILURE, 0, _("empty record separator"));
            ਜੇ (optarg[1])
              {
                ਜੇ (streq (optarg, "\\0"))
                  neweol = '\0';
                ਨਹੀਂਤਾਂ
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
            ਜੇ (0 <= eolchar && neweol != eolchar)
              {
                error (EXIT_FAILURE, 0,
                       _("multiple separator characters specified"));
              }

            eolchar = neweol;
          }
          ਰੋਕੋ;

        ਹਾਲਤ '0':
        ਹਾਲਤ '1':
        ਹਾਲਤ '2':
        ਹਾਲਤ '3':
        ਹਾਲਤ '4':
        ਹਾਲਤ '5':
        ਹਾਲਤ '6':
        ਹਾਲਤ '7':
        ਹਾਲਤ '8':
        ਹਾਲਤ '9':
          ਜੇ (ਵੰਡੋ_type_2 == type_undef)
            {
              ਵੰਡੋ_type_2 = type_digits;
              n_units = 0;
            }
          ਜੇ (ਵੰਡੋ_type_2 != type_undef && ਵੰਡੋ_type_2 != type_digits)
            FAIL_ONLY_ONE_WAY ();
          ਜੇ (digits_optind != 0 && digits_optind != this_optind)
            n_units = 0;	/* More than one number given; ignore other. */
          digits_optind = this_optind;
          ਜੇ (ckd_mul (&n_units, n_units, 10)
              || ckd_ਜੋੜੋ (&n_units, n_units, c - '0'))
            n_units = INTMAX_MAX;
          ਰੋਕੋ;

        ਹਾਲਤ 'd':
        ਹਾਲਤ 'x':
          ਜੇ (c == 'd')
            suffix_alphabet = "0123456789";
          ਨਹੀਂਤਾਂ
            suffix_alphabet = "0123456789abcdef";
          ਜੇ (optarg)
            {
              ਜੇ (ਲੜੀ_ਲੰਬਾਈ (optarg) != strspn (optarg, suffix_alphabet))
                {
                  error (0, 0,
                         (c == 'd') ?
                           _("%s: invalid start value for numerical suffix") :
                           _("%s: invalid start value for hexadecimal suffix"),
                         quote (optarg));
                  usage (EXIT_FAILURE);
                }
              ਨਹੀਂਤਾਂ
                {
                  /* Skip any leading zero.  */
                  ਜਦੋਂ (*optarg == '0' && *(optarg + 1) != '\0')
                    optarg++;
                  numeric_suffix_ਸ਼ੁਰੂ = optarg;
                }
            }
          ਰੋਕੋ;

        ਹਾਲਤ 'e':
          elide_ਖਾਲੀ_ਥਾਵਾਂ_files = true;
          ਰੋਕੋ;

        ਹਾਲਤ FILTER_OPTION:
          filter_command = optarg;
          ਰੋਕੋ;

        ਹਾਲਤ IO_BLKSIZE_OPTION:
          ਵਿੱਚ_blk_ਮਾਪ = xnumtoumax (optarg, 10, 1,
                                    MIN (SYS_BUFSIZE_MAX,
                                         MIN (IDX_MAX, SIZE_MAX) - 1),
                                    multipliers, _("invalid IO block size"),
                                    0, XTOINT_MIN_RANGE);
          ਰੋਕੋ;

        ਹਾਲਤ VERBOSE_OPTION:
          verbose = true;
          ਰੋਕੋ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);

        ਮੂਲ:
          usage (EXIT_FAILURE);
        }
    }

  ਜੇ (k_units != 0 && filter_command)
    {
      error (0, 0, _("--filter does not process a chunk extracted to "
                     "standard output"));
      usage (EXIT_FAILURE);
    }

  /* Handle default case.  */
  ਜੇ (ਵੰਡੋ_type_2 == type_undef)
    {
      ਵੰਡੋ_type_2 = type_ਸਤਰਾਂ;
      n_units = 1000;
    }

  ਜੇ (n_units == 0)
    {
      error (0, 0, _("invalid number of lines: %s"), quote ("0"));
      usage (EXIT_FAILURE);
    }

  ਜੇ (eolchar < 0)
    eolchar = '\n';

  ਸੈੱਟ_suffix_ਲੰਮਾਈ (n_units, ਵੰਡੋ_type_2);

  /* Get out the filename arguments.  */

  ਜੇ (optind < argc)
    infile = argv[optind++];

  ਜੇ (optind < argc)
    outbase = argv[optind++];

  ਜੇ (optind < argc)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind]));
      usage (EXIT_FAILURE);
    }

  /* Check that the suffix length is large enough for the numerical
     suffix start value.  */
  ਜੇ (numeric_suffix_ਸ਼ੁਰੂ && ਲੜੀ_ਲੰਬਾਈ (numeric_suffix_ਸ਼ੁਰੂ) > suffix_ਲੰਮਾਈ)
    {
      error (0, 0, _("numerical suffix start value is too large "
                     "for the suffix length"));
      usage (EXIT_FAILURE);
    }

  /* Open the input file.  */
  ਜੇ (! streq (infile, "-")
      && fd_reopen (STDIN_FILENO, infile, O_RDONLY, 0) < 0)
    error (EXIT_FAILURE, errno, _("cannot open %s for reading"),
           quoteaf (infile));

  /* Binary I/O is safer when byte counts are used.  */
  xset_ਦੁਆਧਾਰੀ_ਬਹੁਲਕ (STDIN_FILENO, O_ਦੁਆਧਾਰੀ);

  /* Advise the kernel of our access pattern.  */
  fdadvise (STDIN_FILENO, 0, 0, FADVISE_SEQUENTIAL);

  /* Get the optimal block size of input device and make a buffer.  */

  ਜੇ (fstat (STDIN_FILENO, &ਵਿੱਚ_stat_buf) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (infile));

  ਜੇ (ਵਿੱਚ_blk_ਮਾਪ == 0)
    {
      ਵਿੱਚ_blk_ਮਾਪ = io_blksize (&ਵਿੱਚ_stat_buf);
      ਜੇ (SYS_BUFSIZE_MAX < ਵਿੱਚ_blk_ਮਾਪ)
        ਵਿੱਚ_blk_ਮਾਪ = SYS_BUFSIZE_MAX;
    }

  ਅੱਖਰ *buf = xalignalloc (ਪੰਨਾ_ਮਾਪ, ਵਿੱਚ_blk_ਮਾਪ + 1);
  ssize_t initial_read = -1;

  ਜੇ (ਵੰਡੋ_type_2 == type_chunk_ਬਾਈਟਾਂ || ਵੰਡੋ_type_2 == type_chunk_ਸਤਰਾਂ)
    {
      file_ਮਾਪ = ਨਿਵੇਸ਼_file_ਮਾਪ (STDIN_FILENO, &ਵਿੱਚ_stat_buf,
                                   buf, ਵਿੱਚ_blk_ਮਾਪ);
      ਜੇ (file_ਮਾਪ < 0)
        error (EXIT_FAILURE, errno, _("%s: cannot determine file size"),
               quotef (infile));
      initial_read = MIN (file_ਮਾਪ, ਵਿੱਚ_blk_ਮਾਪ);
    }

  /* When filtering, closure of one pipe must not terminate the process,
     as there may still be other streams expecting input from us.  */
  ਜੇ (filter_command)
    default_SIGPIPE = ਸੰਕੇਤ (SIGPIPE, SIG_IGN) == SIG_DFL;

  ਚੋਣ (ਵੰਡੋ_type_2)
    {
    ਹਾਲਤ type_digits:
    ਹਾਲਤ type_ਸਤਰਾਂ:
      ਸਤਰਾਂ_ਵੰਡੋ (n_units, buf, ਵਿੱਚ_blk_ਮਾਪ);
      ਰੋਕੋ;

    ਹਾਲਤ type_ਬਾਈਟਾਂ:
      ਬਾਈਟਾਂ_ਵੰਡੋ (n_units, 0, buf, ਵਿੱਚ_blk_ਮਾਪ, -1, 0);
      ਰੋਕੋ;

    ਹਾਲਤ type_byteslines:
      ਸਤਰ_ਬਾਈਟਾਂ_ਵੰਡੋ (n_units, buf, ਵਿੱਚ_blk_ਮਾਪ);
      ਰੋਕੋ;

    ਹਾਲਤ type_chunk_ਬਾਈਟਾਂ:
      ਜੇ (k_units == 0)
        ਬਾਈਟਾਂ_ਵੰਡੋ (file_ਮਾਪ / n_units, file_ਮਾਪ % n_units,
                     buf, ਵਿੱਚ_blk_ਮਾਪ, initial_read, n_units);
      ਨਹੀਂਤਾਂ
        ਬਾਈਟਾਂ_chunk_ਕੱਢੋ (k_units, n_units, buf, ਵਿੱਚ_blk_ਮਾਪ, initial_read,
                             file_ਮਾਪ);
      ਰੋਕੋ;

    ਹਾਲਤ type_chunk_ਸਤਰਾਂ:
      ਸਤਰਾਂ_chunk_ਵੰਡੋ (k_units, n_units, buf, ਵਿੱਚ_blk_ਮਾਪ, initial_read,
                         file_ਮਾਪ);
      ਰੋਕੋ;

    ਹਾਲਤ type_rr:
      /* Note, this is like 'sed -n ${k}~${n}p' when k > 0,
         but the functionality is provided for symmetry.  */
      {
        ਦਾ_t *files;
        ਸਤਰਾਂ_rr (k_units, n_units, buf, ਵਿੱਚ_blk_ਮਾਪ, &files);
      }
      ਰੋਕੋ;

    ਹਾਲਤ type_undef: ਮੂਲ:
      affirm (false);
    }

  ਜੇ (close (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (infile));
  closeout (ਨਿਰਾ, output_desc, filter_pid, outfile);

  main_exit (EXIT_SUCCESS);
}
