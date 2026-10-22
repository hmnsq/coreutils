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
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "split"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("Richard M. Stallman")

/* Shell command to filter through, instead of creating files.  */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filter_command;

/* Process ID of the filter.  */
ಸ್ಥಿರ pid_t filter_pid;

ಸ್ಥಿರ ರಚನೆ
{
  /* The number of open pipes.  */
  ಪೂರ್ಣಾಂಕ n_pipes;

  /* The number of allocated pipes.  */
  idx_t n_pipes_alloc;

  /* Array of open pipes.  */
  ಪೂರ್ಣಾಂಕ *pipes COUNTED_BY (n_pipes);
} open_pipes;

/* Whether SIGPIPE has the default action, when --filter is used.  */
ಸ್ಥಿರ bool default_SIGPIPE;

/* Base name of output files.  */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *outbase = "x";

/* Name of output files.  */
ಸ್ಥಿರ ಅಕ್ಷರ *outfile;

/* Pointer to the end of the prefix in OUTFILE.
   Suffixes are inserted here.  */
ಸ್ಥಿರ ಅಕ್ಷರ *outfile_mid;

/* Generate new suffix when suffixes are exhausted.  */
ಸ್ಥಿರ bool suffix_auto = true;

/* Length of OUTFILE's suffix.  */
ಸ್ಥಿರ idx_t suffix_ಉದ್ದಳತೆ;

/* Alphabet of characters to use in suffix.  */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *suffix_alphabet = "abcdefghijklmnopqrstuvwxyz";

/* Numerical suffix start value.  */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *numeric_suffix_ಆರಂಭ;

/* Additional suffix to append to output file names.  */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *additional_suffix;

/* Name of input file.  May be "-".  */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *infile = "-";

/* stat buf for input file.  */
ಸ್ಥಿರ ರಚನೆ stat ಒಳ_stat_buf;

/* Descriptor on which output file is open.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ output_desc = -1;

/* If true, print a diagnostic on standard error just before each
   output file is opened. */
ಸ್ಥಿರ bool verbose;

/* If true, don't generate zero length output files. */
ಸ್ಥಿರ bool elide_ಖಾಲಿಗಳು_files;

/* If true, in round robin mode, immediately copy
   input to output, which is much slower, so disabled by default.  */
ಸ್ಥಿರ bool unbuffered;

/* The character marking end of line.  Defaults to \n below.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ eolchar = -1;

/* The split mode to use.  */
ಎಣಿಕೆ ವಿಭಜಿಸು_type
{
  type_undef, type_ಬೈಟುಗಳು, type_byteslines, type_ಸಾಲುಗಳು, type_digits,
  type_chunk_ಬೈಟುಗಳು, type_chunk_ಸಾಲುಗಳು, type_rr
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ಎಣಿಕೆ
{
  VERBOSE_OPTION = CHAR_MAX + 1,
  FILTER_OPTION,
  IO_BLKSIZE_OPTION,
  ADDITIONAL_SUFFIX_OPTION
};

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ longopts[] =
{
  {"bytes", required_ವಾದ, ಖಾಲಿ, 'b'},
  {"lines", required_ವಾದ, ಖಾಲಿ, 'l'},
  {"line-bytes", required_ವಾದ, ಖಾಲಿ, 'C'},
  {"number", required_ವಾದ, ಖಾಲಿ, 'n'},
  {"elide-empty-files", no_ವಾದ, ಖಾಲಿ, 'e'},
  {"unbuffered", no_ವಾದ, ಖಾಲಿ, 'u'},
  {"suffix-length", required_ವಾದ, ಖಾಲಿ, 'a'},
  {"additional-suffix", required_ವಾದ, ಖಾಲಿ,
   ADDITIONAL_SUFFIX_OPTION},
  {"numeric-suffixes", optional_ವಾದ, ಖಾಲಿ, 'd'},
  {"hex-suffixes", optional_ವಾದ, ಖಾಲಿ, 'x'},
  {"filter", required_ವಾದ, ಖಾಲಿ, FILTER_OPTION},
  {"verbose", no_ವಾದ, ಖಾಲಿ, VERBOSE_OPTION},
  {"separator", required_ವಾದ, ಖಾಲಿ, 't'},
  {"-io-blksize", required_ವಾದ, ಖಾಲಿ,
   IO_BLKSIZE_OPTION}, /* do not document */
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};

/* Return true if the errno value, ERR, is ignorable.  */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
ignorable (ಪೂರ್ಣಾಂಕ err)
{
  ಹಿಂತಿರುಗಿ filter_command && err == EPIPE;
}

ಸ್ಥಿರ ಶೂನ್ಯ
ಗಣ_suffix_ಉದ್ದಳತೆ (intmax_t n_units, ಎಣಿಕೆ ವಿಭಜಿಸು_type ವಿಭಜಿಸು_type_2)
{
#ವ್ಯಾಖ್ಯಾನಿಸು DEFAULT_SUFFIX_ಉದ್ದಳತೆ 2

  ಪೂರ್ಣಾಂಕ suffix_ಉದ್ದಳತೆ_needed = 0;

  /* The suffix auto length feature is incompatible with
     a user specified start value as the generated suffixes
     are not all consecutive.  */
  ಆದರೆ (numeric_suffix_ಆರಂಭ)
    suffix_auto = false;

  /* Auto-calculate the suffix length if the number of files is given.  */
  ಆದರೆ (ವಿಭಜಿಸು_type_2 == type_chunk_ಬೈಟುಗಳು || ವಿಭಜಿಸು_type_2 == type_chunk_ಸಾಲುಗಳು
      || ವಿಭಜಿಸು_type_2 == type_rr)
    {
      intmax_t n_units_ಕೊನೆ = n_units - 1;
      ಆದರೆ (numeric_suffix_ಆರಂಭ)
        {
          intmax_t n_ಆರಂಭ;
          strtol_error e = xstrtoimax (numeric_suffix_ಆರಂಭ, ಖಾಲಿ, 10,
                                       &n_ಆರಂಭ, "");
          ಆದರೆ (e == LONGINT_OK && n_ಆರಂಭ < n_units)
            {
              /* Restrict auto adjustment so we don't keep
                 incrementing a suffix size arbitrarily,
                 as that would break sort order for files
                 generated from multiple split runs.  */
              ಆದರೆ (ckd_ಕೂಡಿಸು (&n_units_ಕೊನೆ, n_units_ಕೊನೆ, n_ಆರಂಭ))
                n_units_ಕೊನೆ = INTMAX_MAX;
            }

        }
      idx_t alphabet_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (suffix_alphabet);
      ಮಾಡು
        suffix_ಉದ್ದಳತೆ_needed++;
      ಆಗ (n_units_ಕೊನೆ /= alphabet_len);

      suffix_auto = false;
    }

  ಆದರೆ (suffix_ಉದ್ದಳತೆ)            /* set by user */
    {
      ಆದರೆ (suffix_ಉದ್ದಳತೆ < suffix_ಉದ್ದಳತೆ_needed)
        error (EXIT_FAILURE, 0,
               _("the suffix length needs to be at least %d"),
               suffix_ಉದ್ದಳತೆ_needed);
      suffix_auto = false;
      ಹಿಂತಿರುಗಿ;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    suffix_ಉದ್ದಳತೆ = MAX (DEFAULT_SUFFIX_ಉದ್ದಳತೆ, suffix_ಉದ್ದಳತೆ_needed);
}

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("\
Usage: %s [OPTION]... [FILE [ಪೂರ್ವಪ್ರತ್ಯಯ_2]]\n\
"),
              ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
Output pieces ದ FILE ಗಮ್ಯ PREFIXaa, PREFIXab, ...;\n\
ಪೂರ್ವನಿಯೋಜಿತ ಗಾತ್ರ_2 is 1000 ಸಾಲುಗಳು, and ಪೂರ್ವನಿಯೋಜಿತ ಪೂರ್ವಪ್ರತ್ಯಯ_2 is 'x'.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oprintf (_("\
  -a, --suffix-ಉದ್ದಳತೆ=N\n\
         generate suffixes ದ ಉದ್ದಳತೆ N (ಪೂರ್ವನಿಯೋಜಿತ %d)\n\
"), DEFAULT_SUFFIX_ಉದ್ದಳತೆ);
      oputs (_("\
      --additional-suffix=SUFFIX\n\
         append an additional SUFFIX ಗಮ್ಯ file ಹೆಸರುಗಳು\n\
"));
      oputs (_("\
  -b, --ಬೈಟುಗಳು=ಗಾತ್ರ_3\n\
         put ಗಾತ್ರ_3 ಬೈಟುಗಳು ಪ್ರತಿ output file\n\
"));
      oputs (_("\
  -C, --ಸಾಲು-ಬೈಟುಗಳು=ಗಾತ್ರ_3\n\
         put at most ಗಾತ್ರ_3 ಬೈಟುಗಳು ದ ದಾಖಲೆಗಳು ಪ್ರತಿ output file\n\
"));
      oputs (_("\
  -d\n\
         use numeric suffixes starting at 0, not alphabetic\n\
"));
      oputs (_("\
      --numeric-suffixes[=FROM]\n\
         same as -d, but allow setting the ಆರಂಭ ಮೌಲ್ಯ\n\
"));
      oputs (_("\
  -x\n\
         use hex suffixes starting at 0, not alphabetic\n\
"));
      oputs (_("\
      --hex-suffixes[=FROM]\n\
         same as -x, but allow setting the ಆರಂಭ ಮೌಲ್ಯ\n\
"));
      oputs (_("\
  -e, --elide-ಖಾಲಿಗಳು-files\n\
         ಮಾಡು not generate ಖಾಲಿಗಳು output files with '-n'\n\
"));
      oputs (_("\
      --filter=COMMAND\n\
         ಲಿಖ ಗಮ್ಯ shell COMMAND; file name is $FILE\n\
"));
      oputs (_("\
  -l, --ಸಾಲುಗಳು=ಸಂಖ್ಯೆ_2\n\
         put ಸಂಖ್ಯೆ_2 ಸಾಲುಗಳು/ದಾಖಲೆಗಳು ಪ್ರತಿ output file\n\
"));
      oputs (_("\
  -n, --ಸಂಖ್ಯೆ=CHUNKS\n\
         generate CHUNKS output files; see explanation ಕೆಳಗೆ\n\
"));
      oputs (_("\
  -t, --separator=SEP\n\
         use SEP instead ದ newline as the ದಾಖಲೆ separator;\n\
         '\\0' (zero) specifies the NUL character\n\
"));
      oputs (_("\
  -u, --unbuffered\n\
         immediately copy ಒಳಹರಿವು ಗಮ್ಯ output with '-n r/...'\n\
"));
      oputs (_("\
      --verbose\n\
         print a diagnostic just before each output file is opened\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      emit_ಗಾತ್ರ_note ();
      fputs (_("\n\
CHUNKS may be:\n\
  N       ವಿಭಜಿಸು into N files based on ಗಾತ್ರ_2 ದ ಒಳಹರಿವು\n\
  K/N     output Kth ದ N ಗಮ್ಯ standard output\n\
  l/N     ವಿಭಜಿಸು into N files without splitting ಸಾಲುಗಳು/ದಾಖಲೆಗಳು\n\
  l/K/N   output Kth ದ N ಗಮ್ಯ standard output without splitting ಸಾಲುಗಳು/ದಾಖಲೆಗಳು\n\
  r/N     like 'l' but use ಸುತ್ತು_2 robin distribution\n\
  r/K/N   likewise but only output Kth ದ N ಗಮ್ಯ standard output\n\
"), stdout);
      fputs (_("\n\
-n (except -nr) will ಬಫರ್ ಗಮ್ಯ $TMPDIR, defaulting ಗಮ್ಯ /tmp,\n\
ಆದರೆ the ಒಳಹರಿವು ಗಾತ್ರ_2 cannot easily be determined.\n\
"), stdout);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

/* Copy the data in FD to a temporary file, then make that file FD.
   Use BUF, of size BUFSIZE, to copy.  Return the number of
   bytes copied, or -1 (setting errno) on error.  */
ಸ್ಥಿರ off_t
copy_ಗಮ್ಯ_tmpfile (ಪೂರ್ಣಾಂಕ fd, ಅಕ್ಷರ *buf, idx_t bufsize)
{
  FILE *tmp;
  ಆದರೆ (!ತಾತ್ಕಾಲಿಕ_ಹರಿವು (&tmp, ಖಾಲಿ))
    ಹಿಂತಿರುಗಿ -1;
  off_t copied = 0;
  off_t r;

  ಆಗ (0 < (r = ಪಠ (fd, buf, bufsize)))
    {
      ಆದರೆ (fwrite (buf, 1, r, tmp) != r)
        ಹಿಂತಿರುಗಿ -1;
      ಆದರೆ (ckd_ಕೂಡಿಸು (&copied, copied, r))
        {
          errno = EOVERFLOW;
          ಹಿಂತಿರುಗಿ -1;
        }
    }

  ಆದರೆ (r < 0)
    ಹಿಂತಿರುಗಿ r;
  r = dup2 (fileno (tmp), fd);
  ಆದರೆ (r < 0)
    ಹಿಂತಿರುಗಿ r;
  ಆದರೆ (fclose (tmp) < 0)
    ಹಿಂತಿರುಗಿ -1;
  ಹಿಂತಿರುಗಿ copied;
}

/* Return the number of bytes that can be read from FD with status ST.
   Store up to the first BUFSIZE bytes of the file's data into BUF,
   and advance the file position by the number of bytes read.  On
   input error, set errno and return -1.  */

ಸ್ಥಿರ off_t
ಒಳಹರಿವು_file_ಗಾತ್ರ (ಪೂರ್ಣಾಂಕ fd, ರಚನೆ stat ಸ್ಥಿರಾಂಕ *st, ಅಕ್ಷರ *buf, idx_t bufsize)
{
  off_t ಗಾತ್ರ_2 = 0;
  ಮಾಡು
    {
      ssize_t n_read = ಪಠ (fd, buf + ಗಾತ್ರ_2, bufsize - ಗಾತ್ರ_2);
      ಆದರೆ (n_read <= 0)
        ಹಿಂತಿರುಗಿ n_read < 0 ? n_read : ಗಾತ್ರ_2;
      ಗಾತ್ರ_2 += n_read;
    }
  ಆಗ (ಗಾತ್ರ_2 < bufsize);

  off_t cur, ಕೊನೆ;
  ಆದರೆ ((usable_st_ಗಾತ್ರ (st) && st->st_ಗಾತ್ರ < ಗಾತ್ರ_2)
      || (cur = lseek (fd, 0, SEEK_CUR)) < 0
      || cur < ಗಾತ್ರ_2 /* E.g., /dev/zero on GNU/Linux.  */
      || (ಕೊನೆ = lseek (fd, 0, SEEK_END)) < 0)
    {
      ಅಕ್ಷರ *tmpbuf = xmalloc (bufsize);
      ಕೊನೆ = copy_ಗಮ್ಯ_tmpfile (fd, tmpbuf, bufsize);
      ಬಿಡುಗಡೆ (tmpbuf);
      ಆದರೆ (ಕೊನೆ < 0)
        ಹಿಂತಿರುಗಿ ಕೊನೆ;
      cur = 0;
    }

  ಆದರೆ (ಕೊನೆ == OFF_T_MAX /* E.g., /dev/zero on GNU/Hurd.  */
      || (cur < ಕೊನೆ && ckd_ಕೂಡಿಸು (&ಗಾತ್ರ_2, ಗಾತ್ರ_2, ಕೊನೆ - cur)))
    {
      errno = EOVERFLOW;
      ಹಿಂತಿರುಗಿ -1;
    }

  ಆದರೆ (cur < ಕೊನೆ)
    {
      off_t r = lseek (fd, cur, SEEK_SET);
      ಆದರೆ (r < 0)
        ಹಿಂತಿರುಗಿ r;
    }

  ಹಿಂತಿರುಗಿ ಗಾತ್ರ_2;
}

/* Compute the next sequential output file name and store it into the
   string 'outfile'.  */

ಸ್ಥಿರ ಶೂನ್ಯ
ಮುಂದಿನ_file_name (ಶೂನ್ಯ)
{
  /* Index in suffix_alphabet of each character in the suffix.  */
  ಸ್ಥಿರ idx_t *sufindex;
  ಸ್ಥಿರ idx_t outbase_ಉದ್ದಳತೆ;
  ಸ್ಥಿರ idx_t outfile_ಉದ್ದಳತೆ;
  ಸ್ಥಿರ idx_t addsuf_ಉದ್ದಳತೆ;

  ಆದರೆ (! outfile)
    {
      bool ಅತಿಪ್ರವಾಹ, widen;

new_name:
      widen = !! outfile_ಉದ್ದಳತೆ;

      ಆದರೆ (! widen)
        {
          /* Allocate and initialize the first file name.  */

          outbase_ಉದ್ದಳತೆ = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (outbase);
          addsuf_ಉದ್ದಳತೆ = additional_suffix ? ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (additional_suffix) : 0;
          ಅತಿಪ್ರವಾಹ = ckd_ಕೂಡಿಸು (&outfile_ಉದ್ದಳತೆ, outbase_ಉದ್ದಳತೆ + addsuf_ಉದ್ದಳತೆ,
                              suffix_ಉದ್ದಳತೆ);
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          /* Reallocate and initialize a new wider file name.
             We do this by subsuming the unchanging part of
             the generated suffix into the prefix (base), and
             reinitializing the now one longer suffix.  */

          ಅತಿಪ್ರವಾಹ = ckd_ಕೂಡಿಸು (&outfile_ಉದ್ದಳತೆ, outfile_ಉದ್ದಳತೆ, 2);
          suffix_ಉದ್ದಳತೆ++;
        }

      idx_t outfile_ಗಾತ್ರ;
      ಅತಿಪ್ರವಾಹ |= ckd_ಕೂಡಿಸು (&outfile_ಗಾತ್ರ, outfile_ಉದ್ದಳತೆ, 1);
      ಆದರೆ (ಅತಿಪ್ರವಾಹ)
        xalloc_die ();
      outfile = xirealloc (outfile, outfile_ಗಾತ್ರ);

      ಆದರೆ (! widen)
        ಸ್ಮರಣೆ_ನಕಲು (outfile, outbase, outbase_ಉದ್ದಳತೆ);
      ಇಲ್ಲದಿದ್ದರೆ
        {
          /* Append the last alphabet character to the file name prefix.  */
          outfile[outbase_ಉದ್ದಳತೆ] = suffix_alphabet[sufindex[0]];
          outbase_ಉದ್ದಳತೆ++;
        }

      outfile_mid = outfile + outbase_ಉದ್ದಳತೆ;
      memset (outfile_mid, suffix_alphabet[0], suffix_ಉದ್ದಳತೆ);
      ಆದರೆ (additional_suffix)
        ಸ್ಮರಣೆ_ನಕಲು (outfile_mid + suffix_ಉದ್ದಳತೆ, additional_suffix, addsuf_ಉದ್ದಳತೆ);
      outfile[outfile_ಉದ್ದಳತೆ] = 0;

      ಬಿಡುಗಡೆ (sufindex);
      sufindex = xicalloc (suffix_ಉದ್ದಳತೆ, ಗಾತ್ರ *sufindex);

      ಆದರೆ (numeric_suffix_ಆರಂಭ)
        {
          affirm (! widen);

          /* Update the output file name.  */
          idx_t i = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (numeric_suffix_ಆರಂಭ);
          ಸ್ಮರಣೆ_ನಕಲು (outfile_mid + suffix_ಉದ್ದಳತೆ - i, numeric_suffix_ಆರಂಭ, i);

          /* Update the suffix index.  */
          idx_t *sufindex_ಕೊನೆ = sufindex + suffix_ಉದ್ದಳತೆ;
          ಆಗ (i-- != 0)
            *--sufindex_ಕೊನೆ = numeric_suffix_ಆರಂಭ[i] - '0';
        }

#ಆದರೆ ! _POSIX_NO_TRUNC && HAVE_PATHCONF && defined _PC_NAME_MAX
      /* POSIX requires that if the output file name is too long for
         its directory, 'split' must fail without creating any files.
         This must be checked for explicitly on operating systems that
         silently truncate file names.  */
      {
        ಅಕ್ಷರ *dir = dir_name (outfile);
        ದೀರ್ಘ name_max = pathconf (dir, _PC_NAME_MAX);
        ಆದರೆ (0 <= name_max && name_max < ತಳ_len (ಕೊನೆಯ_component (outfile)))
          error (EXIT_FAILURE, ENAMETOOLONG, "%s", quotef (outfile));
        ಬಿಡುಗಡೆ (dir);
      }
#ಆದರೆ_ಅಂತ್ಯ
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      /* Increment the suffix in place, if possible.  */

      idx_t i = suffix_ಉದ್ದಳತೆ;
      ಆಗ (i-- != 0)
        {
          sufindex[i]++;
          ಆದರೆ (suffix_auto && i == 0 && ! suffix_alphabet[sufindex[0] + 1])
            ಹೋಗು new_name;
          outfile_mid[i] = suffix_alphabet[sufindex[i]];
          ಆದರೆ (outfile_mid[i])
            ಹಿಂತಿರುಗಿ;
          sufindex[i] = 0;
          outfile_mid[i] = suffix_alphabet[sufindex[i]];
        }
      error (EXIT_FAILURE, 0, _("output file suffixes exhausted"));
    }
}

/* Create or truncate a file.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
create (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name)
{
  ಆದರೆ (!filter_command)
    {
      ಆದರೆ (verbose)
        fprintf (stdout, _("creating file %s\n"), quoteaf (name));

      ಪೂರ್ಣಾಂಕ oflags = O_WRONLY | O_CREAT | O_ದ್ವಿಮಾನ;
      ಪೂರ್ಣಾಂಕ fd = open (name, oflags | O_EXCL, ಬಹುಲಕ_RW_UGO);
      ಆದರೆ (0 <= fd || errno != EEXIST)
        ಹಿಂತಿರುಗಿ fd;
      fd = open (name, oflags, ಬಹುಲಕ_RW_UGO);
      ಆದರೆ (fd < 0)
        ಹಿಂತಿರುಗಿ fd;
      ರಚನೆ stat ಹೊರಹೋಗು_stat_buf;
      ಆದರೆ (fstat (fd, &ಹೊರಹೋಗು_stat_buf) != 0)
        error (EXIT_FAILURE, errno, _("failed to stat %s"), quoteaf (name));
      ಆದರೆ (psame_inode (&ಒಳ_stat_buf, &ಹೊರಹೋಗು_stat_buf))
        error (EXIT_FAILURE, 0, _("%s would overwrite input; aborting"),
               quoteaf (name));
      ಆದರೆ (ftruncate (fd, 0) < 0
          && (S_ISREG (ಹೊರಹೋಗು_stat_buf.st_ಬಹುಲಕ) || S_TYPEISSHM (&ಹೊರಹೋಗು_stat_buf)))
        error (EXIT_FAILURE, errno, _("%s: error truncating"), quotef (name));

      ಹಿಂತಿರುಗಿ fd;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಆದರೆ (setenv ("FILE", name, 1) != 0)
        error (EXIT_FAILURE, errno,
               _("failed to set FILE environment variable"));
      ಆದರೆ (verbose)
        fprintf (stdout, _("executing with FILE=%s\n"), quotef (name));

      ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ;
      ಪೂರ್ಣಾಂಕ fd_pair[2];
      pid_t ಮಗು_pid;

      posix_spawnattr_t attr;
      posix_spawn_file_actions_t actions;

      sigset_t ಗಣ_2;
      sigemptyset (&ಗಣ_2);
      ಆದರೆ (default_SIGPIPE)
        sigaddset (&ಗಣ_2, SIGPIPE);

      ಆದರೆ (   (ಫಲಿತಾಂಶ = posix_spawnattr_init (&attr))
          || (ಫಲಿತಾಂಶ = posix_spawnattr_setflags (&attr,
                                                 (POSIX_SPAWN_USEVFORK
                                                  | POSIX_SPAWN_SETSIGDEF)))
          || (ಫಲಿತಾಂಶ = posix_spawnattr_setsigdefault (&attr, &ಗಣ_2))
          || (ಫಲಿತಾಂಶ = posix_spawn_file_actions_init (&actions))
         )
        error (EXIT_FAILURE, ಫಲಿತಾಂಶ, _("posix_spawn initialization failed"));

      ಆದರೆ (pipe (fd_pair) != 0)
        error (EXIT_FAILURE, errno, _("failed to create pipe"));

      /* We have to close any pipes that were opened during an
         earlier call, otherwise this process will be holding a
         write-pipe that will prevent the earlier process from
         reading an EOF on the corresponding read-pipe.  */
      ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < open_pipes.n_pipes; ++i)
        ಆದರೆ ((ಫಲಿತಾಂಶ = posix_spawn_file_actions_addclose (&actions,
                                                         open_pipes.pipes[i])))
          ನಿಲ್ಲಿಸು;

      ಆದರೆ (   ಫಲಿತಾಂಶ
          || (ಫಲಿತಾಂಶ = posix_spawn_file_actions_addclose (&actions, fd_pair[1]))
          || (fd_pair[0] != STDIN_FILENO
              && (   (ಫಲಿತಾಂಶ = posix_spawn_file_actions_adddup2 (&actions,
                                                                 fd_pair[0],
                                                                 STDIN_FILENO))
                  || (ಫಲಿತಾಂಶ = posix_spawn_file_actions_addclose (&actions,
                                                                  fd_pair[0]))))
         )
        error (EXIT_FAILURE, ಫಲಿತಾಂಶ, _("posix_spawn setup failed"));


      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *shell_prog = getenv ("SHELL");
      ಆದರೆ (shell_prog == ಖಾಲಿ)
        shell_prog = "/bin/sh";
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸ್ಥಿರಾಂಕ argv[] = { ಕೊನೆಯ_component (shell_prog), "-c",
                                   filter_command, ಖಾಲಿ };

      ಫಲಿತಾಂಶ = posix_spawn (&ಮಗು_pid, shell_prog, &actions, &attr,
                            (ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ *) argv, environ);
      ಆದರೆ (ಫಲಿತಾಂಶ != 0)
        error (EXIT_FAILURE, errno, _("failed to run command: \"%s -c %s\""),
               shell_prog, filter_command);

      posix_spawnattr_destroy (&attr);
      posix_spawn_file_actions_destroy (&actions);

      ಆದರೆ (close (fd_pair[0]) != 0)
        error (EXIT_FAILURE, errno, _("failed to close input pipe"));
      filter_pid = ಮಗು_pid;
      ಆದರೆ (open_pipes.n_pipes == open_pipes.n_pipes_alloc)
        open_pipes.pipes = xpalloc (open_pipes.pipes, &open_pipes.n_pipes_alloc,
                                    1, MIN (ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ, IDX_MAX),
                                    ಗಾತ್ರ *open_pipes.pipes);
      idx_t ಸ್ಥಿರಾಂಕ pipe_idx = open_pipes.n_pipes++;
      open_pipes.pipes[pipe_idx] = fd_pair[1];
      ಹಿಂತಿರುಗಿ fd_pair[1];
    }
}

/* Close the output file, and do any associated cleanup.
   If FP and FD are both specified, they refer to the same open file;
   in this case FP is closed, but FD is still used in cleanup.  */
ಸ್ಥಿರ ಶೂನ್ಯ
closeout (FILE *fp, ಪೂರ್ಣಾಂಕ fd, pid_t pid, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name)
{
  ಆದರೆ (fp != ಖಾಲಿ && fclose (fp) != 0 && ! ignorable (errno))
    error (EXIT_FAILURE, errno, "%s", quotef (name));
  ಆದರೆ (fd >= 0)
    {
      ಆದರೆ (fp == ಖಾಲಿ && close (fd) < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (name));
      ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < open_pipes.n_pipes; ++j)
        {
          ಆದರೆ (open_pipes.pipes[j] == fd)
            {
              idx_t ಸ್ಥಿರಾಂಕ ಕೊನೆಯ_idx = open_pipes.n_pipes - 1;
              open_pipes.pipes[j] = open_pipes.pipes[ಕೊನೆಯ_idx];
              open_pipes.n_pipes--;
              ನಿಲ್ಲಿಸು;
            }
        }
    }
  ಆದರೆ (pid > 0)
    {
      ಪೂರ್ಣಾಂಕ wstatus;
      ಆದರೆ (waitpid (pid, &wstatus, 0) < 0)
        error (EXIT_FAILURE, errno, _("waiting for child process"));
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (WIFSIGNALED (wstatus))
        {
          ಪೂರ್ಣಾಂಕ sig = WTERMSIG (wstatus);
          ಆದರೆ (sig != SIGPIPE)
            {
              ಅಕ್ಷರ signame[MAX (SIG2STR_MAX, INT_BUFSIZE_BOUND (ಪೂರ್ಣಾಂಕ))];
              ಆದರೆ (sig2str (sig, signame) != 0)
                sprintf (signame, "%d", sig);
              error (sig + 128, 0,
                     _("with FILE=%s, signal %s from command: %s"),
                     quotef (name), signame, filter_command);
            }
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (WIFEXITED (wstatus))
        {
          ಪೂರ್ಣಾಂಕ ex = WEXITSTATUS (wstatus);
          ಆದರೆ (ex != 0)
            error (ex, 0, _("with FILE=%s, exit %d from command: %s"),
                   quotef (name), ex, filter_command);
        }
      ಇಲ್ಲದಿದ್ದರೆ
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
cwrite (bool new_file_flag, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *bp, idx_t ಬೈಟುಗಳು)
{
  ಆದರೆ (new_file_flag)
    {
      ಆದರೆ (!bp && ಬೈಟುಗಳು == 0 && elide_ಖಾಲಿಗಳು_files)
        ಹಿಂತಿರುಗಿ true;
      closeout (ಖಾಲಿ, output_desc, filter_pid, outfile);
      ಮುಂದಿನ_file_name ();
      output_desc = create (outfile);
      ಆದರೆ (output_desc < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (outfile));
    }

  ಆದರೆ (ತುಂಬಿದ_write (output_desc, bp, ಬೈಟುಗಳು) == ಬೈಟುಗಳು)
    ಹಿಂತಿರುಗಿ true;
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಆದರೆ (! ignorable (errno))
        error (EXIT_FAILURE, errno, "%s", quotef (outfile));
      ಹಿಂತಿರುಗಿ false;
    }
}

/* Split into pieces of exactly N_BYTES bytes.
   However, the first REM_BYTES pieces should be 1 byte longer.
   Use buffer BUF, whose size is BUFSIZE.
   If INITIAL_READ is nonnegative,
   BUF contains the first INITIAL_READ input bytes.  */

ಸ್ಥಿರ ಶೂನ್ಯ
ಬೈಟುಗಳು_ವಿಭಜಿಸು (intmax_t n_ಬೈಟುಗಳು, intmax_t rem_ಬೈಟುಗಳು,
             ಅಕ್ಷರ *buf, idx_t bufsize, ssize_t initial_read,
             intmax_t max_files)
{
  bool new_file_flag = true;
  bool filter_ok = true;
  intmax_t opened = 0;
  intmax_t ಗಮ್ಯ_write = n_ಬೈಟುಗಳು + (0 < rem_ಬೈಟುಗಳು);
  bool eof = ! ಗಮ್ಯ_write;

  ಆಗ (! eof)
    {
      ssize_t n_read;
      ಆದರೆ (0 <= initial_read)
        {
          n_read = initial_read;
          initial_read = -1;
          eof = n_read < bufsize;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಆದರೆ (! filter_ok
              && 0 <= lseek (STDIN_FILENO, ಗಮ್ಯ_write, SEEK_CUR))
            {
              ಗಮ್ಯ_write = n_ಬೈಟುಗಳು + (opened + 1 < rem_ಬೈಟುಗಳು);
              new_file_flag = true;
            }

          n_read = ಪಠ (STDIN_FILENO, buf, bufsize);
          ಆದರೆ (n_read < 0)
            error (EXIT_FAILURE, errno, "%s", quotef (infile));
          eof = n_read == 0;
        }
      ಅಕ್ಷರ *bp_ಹೊರಹೋಗು = buf;
      ಆಗ (0 < ಗಮ್ಯ_write && ಗಮ್ಯ_write <= n_read)
        {
          ಆದರೆ (filter_ok || new_file_flag)
            filter_ok = cwrite (new_file_flag, bp_ಹೊರಹೋಗು, ಗಮ್ಯ_write);
          opened += new_file_flag;
          new_file_flag = !max_files || (opened < max_files);
          ಆದರೆ (! filter_ok && ! new_file_flag)
            {
              /* If filters no longer accepting input, stop reading.  */
              n_read = 0;
              eof = true;
              ನಿಲ್ಲಿಸು;
            }
          bp_ಹೊರಹೋಗು += ಗಮ್ಯ_write;
          n_read -= ಗಮ್ಯ_write;
          ಗಮ್ಯ_write = n_ಬೈಟುಗಳು + (opened < rem_ಬೈಟುಗಳು);
        }
      ಆದರೆ (0 < n_read)
        {
          ಆದರೆ (filter_ok || new_file_flag)
            filter_ok = cwrite (new_file_flag, bp_ಹೊರಹೋಗು, n_read);
          opened += new_file_flag;
          new_file_flag = false;
          ಆದರೆ (! filter_ok && opened == max_files)
            {
              /* If filters no longer accepting input, stop reading.  */
              ನಿಲ್ಲಿಸು;
            }
          ಗಮ್ಯ_write -= n_read;
        }
    }

  /* Ensure NUMBER files are created, which truncates
     any existing files or notifies any consumers on fifos.
     FIXME: Should we do this before EXIT_FAILURE?  */
  ಆಗ (opened++ < max_files)
    cwrite (true, ಖಾಲಿ, 0);
}

/* Split into pieces of exactly N_LINES lines.
   Use buffer BUF, whose size is BUFSIZE.  */

ಸ್ಥಿರ ಶೂನ್ಯ
ಸಾಲುಗಳು_ವಿಭಜಿಸು (intmax_t n_ಸಾಲುಗಳು, ಅಕ್ಷರ *buf, idx_t bufsize)
{
  ssize_t n_read;
  ಅಕ್ಷರ *bp, *bp_ಹೊರಹೋಗು, *eob;
  bool new_file_flag = true;
  intmax_t n = 0;

  ಮಾಡು
    {
      n_read = ಪಠ (STDIN_FILENO, buf, bufsize);
      ಆದರೆ (n_read < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
      bp = bp_ಹೊರಹೋಗು = buf;
      eob = bp + n_read;
      *eob = eolchar;
      ಆಗ (true)
        {
          bp = rawmemchr (bp, eolchar);
          ಆದರೆ (bp == eob)
            {
              ಆದರೆ (eob != bp_ಹೊರಹೋಗು) /* do not write 0 bytes! */
                {
                  idx_t len = eob - bp_ಹೊರಹೋಗು;
                  cwrite (new_file_flag, bp_ಹೊರಹೋಗು, len);
                  new_file_flag = false;
                }
              ನಿಲ್ಲಿಸು;
            }

          ++bp;
          ಆದರೆ (++n >= n_ಸಾಲುಗಳು)
            {
              cwrite (new_file_flag, bp_ಹೊರಹೋಗು, bp - bp_ಹೊರಹೋಗು);
              bp_ಹೊರಹೋಗು = bp;
              new_file_flag = true;
              n = 0;
            }
        }
    }
  ಆಗ (n_read);
}

/* Split into pieces that are as large as possible while still not more
   than N_BYTES bytes, and are split on line boundaries except
   where lines longer than N_BYTES bytes occur. */

ಸ್ಥಿರ ಶೂನ್ಯ
ಸಾಲು_ಬೈಟುಗಳು_ವಿಭಜಿಸು (intmax_t n_ಬೈಟುಗಳು, ಅಕ್ಷರ *buf, idx_t bufsize)
{
  ssize_t n_read;
  intmax_t n_ಹೊರಹೋಗು = 0;      /* for each split.  */
  idx_t n_hold = 0;
  ಅಕ್ಷರ *hold = ಖಾಲಿ;        /* for lines > bufsize.  */
  idx_t hold_ಗಾತ್ರ = 0;
  bool ವಿಭಜಿಸು_ಸಾಲು = false;  /* Whether a \n was output in a split.  */

  ಮಾಡು
    {
      n_read = ಪಠ (STDIN_FILENO, buf, bufsize);
      ಆದರೆ (n_read < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
      idx_t n_ಎಡ = n_read;
      ಅಕ್ಷರ *sob = buf;
      ಆಗ (n_ಎಡ)
        {
          idx_t ವಿಭಜಿಸು_rest = 0;
          ಅಕ್ಷರ *eoc = ಖಾಲಿ;
          ಅಕ್ಷರ *eol;

          /* Determine End Of Chunk and/or End of Line,
             which are used below to select what to write or buffer.  */
          ಆದರೆ (n_ಬೈಟುಗಳು - n_ಹೊರಹೋಗು - n_hold <= n_ಎಡ)
            {
              /* Have enough for split.  */
              ವಿಭಜಿಸು_rest = n_ಬೈಟುಗಳು - n_ಹೊರಹೋಗು - n_hold;
              eoc = sob + ವಿಭಜಿಸು_rest - 1;
              eol = memrchr (sob, eolchar, ವಿಭಜಿಸು_rest);
            }
          ಇಲ್ಲದಿದ್ದರೆ
            eol = memrchr (sob, eolchar, n_ಎಡ);

          /* Output hold space if possible.  */
          ಆದರೆ (n_hold && !(!eol && n_ಹೊರಹೋಗು))
            {
              cwrite (n_ಹೊರಹೋಗು == 0, hold, n_hold);
              n_ಹೊರಹೋಗು += n_hold;
              n_hold = 0;
            }

          /* Output to eol if present.  */
          ಆದರೆ (eol)
            {
              ವಿಭಜಿಸು_ಸಾಲು = true;
              idx_t n_write = eol - sob + 1;
              cwrite (n_ಹೊರಹೋಗು == 0, sob, n_write);
              n_ಹೊರಹೋಗು += n_write;
              n_ಎಡ -= n_write;
              sob += n_write;
              ಆದರೆ (eoc)
                ವಿಭಜಿಸು_rest -= n_write;
            }

          /* Output to eoc or eob if possible.  */
          ಆದರೆ (n_ಎಡ && !ವಿಭಜಿಸು_ಸಾಲು)
            {
              idx_t n_write = eoc ? ವಿಭಜಿಸು_rest : n_ಎಡ;
              cwrite (n_ಹೊರಹೋಗು == 0, sob, n_write);
              n_ಹೊರಹೋಗು += n_write;
              n_ಎಡ -= n_write;
              sob += n_write;
              ಆದರೆ (eoc)
                ವಿಭಜಿಸು_rest -= n_write;
            }

          /* Update hold if needed.  */
          ಆದರೆ ((eoc && ವಿಭಜಿಸು_rest) || (!eoc && n_ಎಡ))
            {
              idx_t n_buf = eoc ? ವಿಭಜಿಸು_rest : n_ಎಡ;
              ಆದರೆ (hold_ಗಾತ್ರ - n_hold < n_buf)
                hold = xpalloc (hold, &hold_ಗಾತ್ರ, n_buf - (hold_ಗಾತ್ರ - n_hold),
                                -1, ಗಾತ್ರ *hold);
              ಸ್ಮರಣೆ_ನಕಲು (hold + n_hold, sob, n_buf);
              n_hold += n_buf;
              n_ಎಡ -= n_buf;
              sob += n_buf;
            }

          /* Reset for new split.  */
          ಆದರೆ (eoc)
            {
              n_ಹೊರಹೋಗು = 0;
              ವಿಭಜಿಸು_ಸಾಲು = false;
            }
        }
    }
  ಆಗ (n_read);

  /* Handle no eol at end of file.  */
  ಆದರೆ (n_hold)
    cwrite (n_ಹೊರಹೋಗು == 0, hold, n_hold);

  ಬಿಡುಗಡೆ (hold);
}

/* -n l/[K/]N: Write lines to files of approximately file size / N.
   The file is partitioned into file size / N sized portions, with the
   last assigned any excess.  If a line _starts_ within a partition
   it is written completely to the corresponding file.  Since lines
   are not split even if they overlap a partition, the files written
   can be larger or smaller than the partition size, and even empty
   if a line is so long as to completely overlap the partition.  */

ಸ್ಥಿರ ಶೂನ್ಯ
ಸಾಲುಗಳು_chunk_ವಿಭಜಿಸು (intmax_t k, intmax_t n, ಅಕ್ಷರ *buf, idx_t bufsize,
                   ssize_t initial_read, off_t file_ಗಾತ್ರ)
{
  affirm (n && k <= n);

  intmax_t rem_ಬೈಟುಗಳು = file_ಗಾತ್ರ % n;
  off_t chunk_ಗಾತ್ರ = file_ಗಾತ್ರ / n;
  intmax_t chunk_no = 1;
  off_t chunk_ಕೊನೆ = chunk_ಗಾತ್ರ + (0 < rem_ಬೈಟುಗಳು);
  off_t n_written = 0;
  bool new_file_flag = true;
  bool chunk_truncated = false;

  ಆದರೆ (k > 1 && 0 < file_ಗಾತ್ರ)
    {
      /* Start reading 1 byte before kth chunk of file.  */
      off_t ಆರಂಭ = (k - 1) * chunk_ಗಾತ್ರ + MIN (k - 1, rem_ಬೈಟುಗಳು) - 1;
      ಆದರೆ (ಆರಂಭ < initial_read)
        {
          memmove (buf, buf + ಆರಂಭ, initial_read - ಆರಂಭ);
          initial_read -= ಆರಂಭ;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಆದರೆ (initial_read < ಆರಂಭ
              && lseek (STDIN_FILENO, ಆರಂಭ - initial_read, SEEK_CUR) < 0)
            error (EXIT_FAILURE, errno, "%s", quotef (infile));
          initial_read = -1;
        }
      n_written = ಆರಂಭ;
      chunk_no = k - 1;
      chunk_ಕೊನೆ = ಆರಂಭ + 1;
    }

  ಆಗ (n_written < file_ಗಾತ್ರ)
    {
      ಅಕ್ಷರ *bp = buf, *eob;
      ssize_t n_read;
      ಆದರೆ (0 <= initial_read)
        {
          n_read = initial_read;
          initial_read = -1;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          n_read = ಪಠ (STDIN_FILENO, buf,
                         MIN (bufsize, file_ಗಾತ್ರ - n_written));
          ಆದರೆ (n_read < 0)
            error (EXIT_FAILURE, errno, "%s", quotef (infile));
        }
      ಆದರೆ (n_read == 0)
        ನಿಲ್ಲಿಸು; /* eof.  */
      chunk_truncated = false;
      eob = buf + n_read;

      ಆಗ (bp != eob)
        {
          idx_t ಗಮ್ಯ_write;
          bool ಮುಂದಿನ = false;

          /* Begin looking for '\n' at last byte of chunk.  */
          off_t skip = MIN (n_read, MAX (0, chunk_ಕೊನೆ - 1 - n_written));
          ಅಕ್ಷರ *bp_ಹೊರಹೋಗು = memchr (bp + skip, eolchar, n_read - skip);
          ಆದರೆ (bp_ಹೊರಹೋಗು)
            {
              bp_ಹೊರಹೋಗು++;
              ಮುಂದಿನ = true;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            bp_ಹೊರಹೋಗು = eob;
          ಗಮ್ಯ_write = bp_ಹೊರಹೋಗು - bp;

          ಆದರೆ (k == chunk_no)
            {
              /* We don't use the stdout buffer here since we're writing
                 large chunks from an existing file, so it's more efficient
                 to write out directly.  */
              ಆದರೆ (ತುಂಬಿದ_write (STDOUT_FILENO, bp, ಗಮ್ಯ_write) != ಗಮ್ಯ_write)
                write_error ();
            }
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (! k)
            cwrite (new_file_flag, bp, ಗಮ್ಯ_write);
          n_written += ಗಮ್ಯ_write;
          bp += ಗಮ್ಯ_write;
          n_read -= ಗಮ್ಯ_write;
          new_file_flag = ಮುಂದಿನ;

          /* A line could have been so long that it skipped
             entire chunks. So create empty files in that case.  */
          ಆಗ (ಮುಂದಿನ || chunk_ಕೊನೆ <= n_written)
            {
              ಆದರೆ (!ಮುಂದಿನ && bp == eob)
                {
                  /* replenish buf, before going to next chunk.  */
                  chunk_truncated = true;
                  ನಿಲ್ಲಿಸು;
                }
              ಆದರೆ (k == chunk_no)
                ಹಿಂತಿರುಗಿ;
              chunk_ಕೊನೆ += chunk_ಗಾತ್ರ + (chunk_no < rem_ಬೈಟುಗಳು);
              chunk_no++;
              ಆದರೆ (chunk_ಕೊನೆ <= n_written)
                {
                  ಆದರೆ (! k)
                    cwrite (true, ಖಾಲಿ, 0);
                }
              ಇಲ್ಲದಿದ್ದರೆ
                ಮುಂದಿನ = false;
            }
        }
    }

  ಆದರೆ (chunk_truncated)
    chunk_no++;

  /* Ensure NUMBER files are created, which truncates
     any existing files or notifies any consumers on fifos.
     FIXME: Should we do this before EXIT_FAILURE?  */
  ಆದರೆ (!k)
    ಆಗ (chunk_no++ <= n)
      cwrite (true, ಖಾಲಿ, 0);
}

/* -n K/N: Extract Kth of N chunks.  */

ಸ್ಥಿರ ಶೂನ್ಯ
ಬೈಟುಗಳು_chunk_ಹೊರತೆಗೆ (intmax_t k, intmax_t n, ಅಕ್ಷರ *buf, idx_t bufsize,
                     ssize_t initial_read, off_t file_ಗಾತ್ರ)
{
  off_t ಆರಂಭ;
  off_t ಕೊನೆ;

  affirm (0 < k && k <= n);

  ಆರಂಭ = (k - 1) * (file_ಗಾತ್ರ / n) + MIN (k - 1, file_ಗಾತ್ರ % n);
  ಕೊನೆ = k == n ? file_ಗಾತ್ರ : k * (file_ಗಾತ್ರ / n) + MIN (k, file_ಗಾತ್ರ % n);

  ಆದರೆ (ಆರಂಭ < initial_read)
    {
      memmove (buf, buf + ಆರಂಭ, initial_read - ಆರಂಭ);
      initial_read -= ಆರಂಭ;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಆದರೆ (initial_read < ಆರಂಭ
          && lseek (STDIN_FILENO, ಆರಂಭ - initial_read, SEEK_CUR) < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
      initial_read = -1;
    }

  ಆಗ (ಆರಂಭ < ಕೊನೆ)
    {
      ssize_t n_read;
      ಆದರೆ (0 <= initial_read)
        {
          n_read = initial_read;
          initial_read = -1;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          n_read = ಪಠ (STDIN_FILENO, buf, bufsize);
          ಆದರೆ (n_read < 0)
            error (EXIT_FAILURE, errno, "%s", quotef (infile));
        }
      ಆದರೆ (n_read == 0)
        ನಿಲ್ಲಿಸು; /* eof.  */
      n_read = MIN (n_read, ಕೊನೆ - ಆರಂಭ);
      ಆದರೆ (ತುಂಬಿದ_write (STDOUT_FILENO, buf, n_read) != n_read
          && ! ignorable (errno))
        error (EXIT_FAILURE, errno, "%s", quotef ("-"));
      ಆರಂಭ += n_read;
    }
}

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ ದ_info
{
  ಅಕ್ಷರ *ದ_name;
  ಪೂರ್ಣಾಂಕ ofd;
  FILE *ofile;
  pid_t opid;
} ದ_t;

ಎಣಿಕೆ
{
  OFD_NEW = -1,
  OFD_APPEND = -2
};

/* Rotate file descriptors when we're writing to more output files than we
   have available file descriptors.
   Return whether we came under file resource pressure.
   If so, it's probably best to close each file when finished with it.  */

ಸ್ಥಿರ bool
ofile_open (ದ_t *files, idx_t i_check, idx_t nfiles)
{
  bool file_ಎಲ್ಲೆ = false;

  ಆದರೆ (files[i_check].ofd <= OFD_NEW)
    {
      ಪೂರ್ಣಾಂಕ fd;
      idx_t i_reopen = i_check ? i_check - 1 : nfiles - 1;

      /* Another process could have opened a file in between the calls to
         close and open, so we should keep trying until open succeeds or
         we've closed all of our files.  */
      ಆಗ (true)
        {
          ಆದರೆ (files[i_check].ofd == OFD_NEW)
            fd = create (files[i_check].ದ_name);
          ಇಲ್ಲದಿದ್ದರೆ /* OFD_APPEND  */
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
              fd = open (files[i_check].ದ_name,
                         O_WRONLY | O_ದ್ವಿಮಾನ | O_APPEND | O_NONBLOCK);
            }

          ಆದರೆ (0 <= fd)
            ನಿಲ್ಲಿಸು;

          ಆದರೆ (!(errno == EMFILE || errno == ENFILE))
            error (EXIT_FAILURE, errno, "%s", quotef (files[i_check].ದ_name));

          file_ಎಲ್ಲೆ = true;

          /* Search backwards for an open file to close.  */
          ಆಗ (files[i_reopen].ofd < 0)
            {
              i_reopen = i_reopen ? i_reopen - 1 : nfiles - 1;
              /* No more open files to close, exit with E[NM]FILE.  */
              ಆದರೆ (i_reopen == i_check)
                error (EXIT_FAILURE, errno, "%s",
                       quotef (files[i_check].ದ_name));
            }

          ಆದರೆ (fclose (files[i_reopen].ofile) != 0)
            error (EXIT_FAILURE, errno, "%s", quotef (files[i_reopen].ದ_name));
          files[i_reopen].ofile = ಖಾಲಿ;
          files[i_reopen].ofd = OFD_APPEND;
        }

      files[i_check].ofd = fd;
      FILE *ofile = fdopen (fd, "a");
      ಆದರೆ (!ofile)
        error (EXIT_FAILURE, errno, "%s", quotef (files[i_check].ದ_name));
      files[i_check].ofile = ofile;
      files[i_check].opid = filter_pid;
      filter_pid = 0;
    }

  ಹಿಂತಿರುಗಿ file_ಎಲ್ಲೆ;
}

/* -n r/[K/]N: Divide file into N chunks in round robin fashion.
   Use BUF of size BUFSIZE for the buffer, and if allocating storage
   put its address into *FILESP to pacify -fsanitize=leak.
   When K == 0, we try to keep the files open in parallel.
   If we run out of file resources, then we revert
   to opening and closing each file for each line.  */

ಸ್ಥಿರ ಶೂನ್ಯ
ಸಾಲುಗಳು_rr (intmax_t k, intmax_t n, ಅಕ್ಷರ *buf, idx_t bufsize, ದ_t **filesp)
{
  bool wrapped = false;
  bool wrote = false;
  bool file_ಎಲ್ಲೆ;
  idx_t i_file;
  ದ_t *files IF_LINT (= ಖಾಲಿ);
  intmax_t ಸಾಲು_no;

  ಆದರೆ (k)
    ಸಾಲು_no = 1;
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಆದರೆ (IDX_MAX < n)
        xalloc_die ();
      files = *filesp = xinmalloc (n, ಗಾತ್ರ *files);

      /* Generate output file names. */
      ಗಾಗಿ (i_file = 0; i_file < n; i_file++)
        {
          ಮುಂದಿನ_file_name ();
          files[i_file].ದ_name = xstrdup (outfile);
          files[i_file].ofd = OFD_NEW;
          files[i_file].ofile = ಖಾಲಿ;
          files[i_file].opid = 0;
        }
      i_file = 0;
      file_ಎಲ್ಲೆ = false;
    }

  ಆಗ (true)
    {
      ಅಕ್ಷರ *bp = buf, *eob;
      ssize_t n_read = ಪಠ (STDIN_FILENO, buf, bufsize);
      ಆದರೆ (n_read < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (n_read == 0)
        ನಿಲ್ಲಿಸು; /* eof.  */
      eob = buf + n_read;

      ಆಗ (bp != eob)
        {
          idx_t ಗಮ್ಯ_write;
          bool ಮುಂದಿನ = false;

          /* Find end of line. */
          ಅಕ್ಷರ *bp_ಹೊರಹೋಗು = memchr (bp, eolchar, eob - bp);
          ಆದರೆ (bp_ಹೊರಹೋಗು)
            {
              bp_ಹೊರಹೋಗು++;
              ಮುಂದಿನ = true;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            bp_ಹೊರಹೋಗು = eob;
          ಗಮ್ಯ_write = bp_ಹೊರಹೋಗು - bp;

          ಆದರೆ (k)
            {
              ಆದರೆ (ಸಾಲು_no == k && unbuffered)
                {
                  ಆದರೆ (ತುಂಬಿದ_write (STDOUT_FILENO, bp, ಗಮ್ಯ_write) != ಗಮ್ಯ_write)
                    write_error ();
                }
              ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಸಾಲು_no == k && fwrite (bp, ಗಮ್ಯ_write, 1, stdout) != 1)
                {
                  write_error ();
                }
              ಆದರೆ (ಮುಂದಿನ)
                ಸಾಲು_no = (ಸಾಲು_no == n) ? 1 : ಸಾಲು_no + 1;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              /* Secure file descriptor. */
              file_ಎಲ್ಲೆ |= ofile_open (files, i_file, n);
              ಆದರೆ (unbuffered)
                {
                  /* Note writing to fd, rather than flushing the FILE gives
                     an 8% performance benefit, due to reduced data copying.  */
                  ಆದರೆ (ತುಂಬಿದ_write (files[i_file].ofd, bp, ಗಮ್ಯ_write) != ಗಮ್ಯ_write
                      && ! ignorable (errno))
                    error (EXIT_FAILURE, errno, "%s",
                           quotef (files[i_file].ದ_name));
                }
              ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (fwrite (bp, ಗಮ್ಯ_write, 1, files[i_file].ofile) != 1
                       && ! ignorable (errno))
                error (EXIT_FAILURE, errno, "%s",
                       quotef (files[i_file].ದ_name));

              ಆದರೆ (! ignorable (errno))
                wrote = true;

              ಆದರೆ (file_ಎಲ್ಲೆ)
                {
                  ಆದರೆ (fclose (files[i_file].ofile) != 0)
                    error (EXIT_FAILURE, errno, "%s",
                           quotef (files[i_file].ದ_name));
                  files[i_file].ofile = ಖಾಲಿ;
                  files[i_file].ofd = OFD_APPEND;
                }
              ಆದರೆ (ಮುಂದಿನ && ++i_file == n)
                {
                  wrapped = true;
                  /* If no filters are accepting input, stop reading.  */
                  ಆದರೆ (! wrote)
                    ಹೋಗು no_filters;
                  wrote = false;
                  i_file = 0;
                }
            }

          bp = bp_ಹೊರಹೋಗು;
        }
    }

no_filters:
  /* Ensure all files created, so that any existing files are truncated,
     and to signal any waiting fifo consumers.
     Also, close any open file descriptors.
     FIXME: Should we do this before EXIT_FAILURE?  */
  ಆದರೆ (!k)
    {
      idx_t ceiling = wrapped ? n : i_file;
      ಗಾಗಿ (i_file = 0; i_file < n; i_file++)
        {
          ಆದರೆ (i_file >= ceiling && !elide_ಖಾಲಿಗಳು_files)
            file_ಎಲ್ಲೆ |= ofile_open (files, i_file, n);
          ಆದರೆ (files[i_file].ofd >= 0)
            closeout (files[i_file].ofile, files[i_file].ofd,
                      files[i_file].opid, files[i_file].ದ_name);
          files[i_file].ofd = OFD_APPEND;
        }
    }
}

#ವ್ಯಾಖ್ಯಾನಿಸು FAIL_ONLY_ONE_WAY()					\
  ಮಾಡು								\
    {								\
      error (0, 0, _("cannot split in more than one way"));	\
      usage (EXIT_FAILURE);					\
    }								\
  ಆಗ (0)

/* Report a string-to-integer conversion failure MSGID with ARG.  */

ಸ್ಥಿರ _Noreturn ಶೂನ್ಯ
strtoint_die (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *msgid, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *arg)
{
  error (EXIT_FAILURE, errno == EINVAL ? 0 : errno, "%s: %s",
         gettext (msgid), quote (arg));
}

/* Use OVERFLOW_OK when it is OK to ignore LONGINT_OVERFLOW errors, since the
   extreme value will do the right thing anyway on any practical platform.  */
#ವ್ಯಾಖ್ಯಾನಿಸು ಅತಿಪ್ರವಾಹ_OK LONGINT_ಅತಿಪ್ರವಾಹ

/* Parse ARG for number of bytes or lines.  The number can be followed
   by MULTIPLIERS, and the resulting value must be positive.
   If the number cannot be parsed, diagnose with MSG.
   Return the number parsed, or an INTMAX_MAX on overflow.  */

ಸ್ಥಿರ intmax_t
parse_n_units (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *arg, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *multipliers, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *msgid)
{
  intmax_t n;
  ಆದರೆ (ಅತಿಪ್ರವಾಹ_OK < xstrtoimax (arg, ಖಾಲಿ, 10, &n, multipliers) || n < 1)
    strtoint_die (msgid, arg);
  ಹಿಂತಿರುಗಿ n;
}

/* Parse K/N syntax of chunk options.  */

ಸ್ಥಿರ ಶೂನ್ಯ
parse_chunk (intmax_t *k_units, intmax_t *n_units, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *arg)
{
  ಅಕ್ಷರ *argend;
  strtol_error e = xstrtoimax (arg, &argend, 10, n_units, "");
  ಆದರೆ (e == LONGINT_INVALID_SUFFIX_CHAR && *argend == '/')
    {
      *k_units = *n_units;
      *n_units = parse_n_units (argend + 1, "",
                                N_("invalid number of chunks"));
      ಆದರೆ (! (0 < *k_units && *k_units <= *n_units))
        error (EXIT_FAILURE, 0, "%s: %s", _("invalid chunk number"),
               quote_mem (arg, argend - arg));
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (! (e <= ಅತಿಪ್ರವಾಹ_OK && 0 < *n_units))
    strtoint_die (N_("invalid number of chunks"), arg);
}


ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  ಎಣಿಕೆ ವಿಭಜಿಸು_type ವಿಭಜಿಸು_type_2 = type_undef;
  idx_t ಒಳ_blk_ಗಾತ್ರ = 0;	/* optimal block size of input file device */
  idx_t ಪುಟ_ಗಾತ್ರ = getpagesize ();
  intmax_t k_units = 0;
  intmax_t n_units = 0;

  ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ multipliers[] = "bEGKkMmPQRTYZ0";
  ಪೂರ್ಣಾಂಕ c;
  ಪೂರ್ಣಾಂಕ digits_optind = 0;
  off_t file_ಗಾತ್ರ = OFF_T_MAX;

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* Parse command line options.  */

  ಆಗ (true)
    {
      /* This is the argv-index of the option we will read next.  */
      ಪೂರ್ಣಾಂಕ this_optind = optind ? optind : 1;

      c = getopt_long (argc, argv, "0123456789C:a:b:del:n:t:ux",
                       longopts, ಖಾಲಿ);
      ಆದರೆ (c == -1)
        ನಿಲ್ಲಿಸು;

      ಆಯ್ಕೆ (c)
        {
        ಸ್ಥಿತಿ 'a':
          suffix_ಉದ್ದಳತೆ = xdectoimax (optarg, 0, IDX_MAX,
                                      "", _("invalid suffix length"), 0);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ ADDITIONAL_SUFFIX_OPTION:
          {
            ಪೂರ್ಣಾಂಕ suffix_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (optarg);
            ಆದರೆ (ಕೊನೆಯ_component (optarg) != optarg
                || (suffix_len && ISSLASH (optarg[suffix_len - 1])))
              {
                error (0, 0,
                       _("invalid suffix %s, contains directory separator"),
                       quote (optarg));
                usage (EXIT_FAILURE);
              }
          }
          additional_suffix = optarg;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'b':
          ಆದರೆ (ವಿಭಜಿಸು_type_2 != type_undef)
            FAIL_ONLY_ONE_WAY ();
          ವಿಭಜಿಸು_type_2 = type_ಬೈಟುಗಳು;
          n_units = parse_n_units (optarg, multipliers,
                                   N_("invalid number of bytes"));
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'l':
          ಆದರೆ (ವಿಭಜಿಸು_type_2 != type_undef)
            FAIL_ONLY_ONE_WAY ();
          ವಿಭಜಿಸು_type_2 = type_ಸಾಲುಗಳು;
          n_units = parse_n_units (optarg, "", N_("invalid number of lines"));
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'C':
          ಆದರೆ (ವಿಭಜಿಸು_type_2 != type_undef)
            FAIL_ONLY_ONE_WAY ();
          ವಿಭಜಿಸು_type_2 = type_byteslines;
          n_units = parse_n_units (optarg, multipliers,
                                   N_("invalid number of lines"));
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'n':
          ಆದರೆ (ವಿಭಜಿಸು_type_2 != type_undef)
            FAIL_ONLY_ONE_WAY ();
          /* skip any whitespace */
          ಆಗ (ಖಾಲಿಯೇ (ಗಮ್ಯ_uchar (*optarg)))
            optarg++;
          ಆದರೆ (STRNCMP_LIT (optarg, "r/") == 0)
            {
              ವಿಭಜಿಸು_type_2 = type_rr;
              optarg += 2;
            }
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (STRNCMP_LIT (optarg, "l/") == 0)
            {
              ವಿಭಜಿಸು_type_2 = type_chunk_ಸಾಲುಗಳು;
              optarg += 2;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            ವಿಭಜಿಸು_type_2 = type_chunk_ಬೈಟುಗಳು;
          parse_chunk (&k_units, &n_units, optarg);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'u':
          unbuffered = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 't':
          {
            ಅಕ್ಷರ neweol = optarg[0];
            ಆದರೆ (! neweol)
              error (EXIT_FAILURE, 0, _("empty record separator"));
            ಆದರೆ (optarg[1])
              {
                ಆದರೆ (streq (optarg, "\\0"))
                  neweol = '\0';
                ಇಲ್ಲದಿದ್ದರೆ
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
            ಆದರೆ (0 <= eolchar && neweol != eolchar)
              {
                error (EXIT_FAILURE, 0,
                       _("multiple separator characters specified"));
              }

            eolchar = neweol;
          }
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ '0':
        ಸ್ಥಿತಿ '1':
        ಸ್ಥಿತಿ '2':
        ಸ್ಥಿತಿ '3':
        ಸ್ಥಿತಿ '4':
        ಸ್ಥಿತಿ '5':
        ಸ್ಥಿತಿ '6':
        ಸ್ಥಿತಿ '7':
        ಸ್ಥಿತಿ '8':
        ಸ್ಥಿತಿ '9':
          ಆದರೆ (ವಿಭಜಿಸು_type_2 == type_undef)
            {
              ವಿಭಜಿಸು_type_2 = type_digits;
              n_units = 0;
            }
          ಆದರೆ (ವಿಭಜಿಸು_type_2 != type_undef && ವಿಭಜಿಸು_type_2 != type_digits)
            FAIL_ONLY_ONE_WAY ();
          ಆದರೆ (digits_optind != 0 && digits_optind != this_optind)
            n_units = 0;	/* More than one number given; ignore other. */
          digits_optind = this_optind;
          ಆದರೆ (ckd_mul (&n_units, n_units, 10)
              || ckd_ಕೂಡಿಸು (&n_units, n_units, c - '0'))
            n_units = INTMAX_MAX;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'd':
        ಸ್ಥಿತಿ 'x':
          ಆದರೆ (c == 'd')
            suffix_alphabet = "0123456789";
          ಇಲ್ಲದಿದ್ದರೆ
            suffix_alphabet = "0123456789abcdef";
          ಆದರೆ (optarg)
            {
              ಆದರೆ (ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (optarg) != strspn (optarg, suffix_alphabet))
                {
                  error (0, 0,
                         (c == 'd') ?
                           _("%s: invalid start value for numerical suffix") :
                           _("%s: invalid start value for hexadecimal suffix"),
                         quote (optarg));
                  usage (EXIT_FAILURE);
                }
              ಇಲ್ಲದಿದ್ದರೆ
                {
                  /* Skip any leading zero.  */
                  ಆಗ (*optarg == '0' && *(optarg + 1) != '\0')
                    optarg++;
                  numeric_suffix_ಆರಂಭ = optarg;
                }
            }
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'e':
          elide_ಖಾಲಿಗಳು_files = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ FILTER_OPTION:
          filter_command = optarg;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ IO_BLKSIZE_OPTION:
          ಒಳ_blk_ಗಾತ್ರ = xnumtoumax (optarg, 10, 1,
                                    MIN (SYS_BUFSIZE_MAX,
                                         MIN (IDX_MAX, SIZE_MAX) - 1),
                                    multipliers, _("invalid IO block size"),
                                    0, XTOINT_MIN_RANGE);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ VERBOSE_OPTION:
          verbose = true;
          ನಿಲ್ಲಿಸು;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);

        ಪೂರ್ವನಿಯೋಜಿತ:
          usage (EXIT_FAILURE);
        }
    }

  ಆದರೆ (k_units != 0 && filter_command)
    {
      error (0, 0, _("--filter does not process a chunk extracted to "
                     "standard output"));
      usage (EXIT_FAILURE);
    }

  /* Handle default case.  */
  ಆದರೆ (ವಿಭಜಿಸು_type_2 == type_undef)
    {
      ವಿಭಜಿಸು_type_2 = type_ಸಾಲುಗಳು;
      n_units = 1000;
    }

  ಆದರೆ (n_units == 0)
    {
      error (0, 0, _("invalid number of lines: %s"), quote ("0"));
      usage (EXIT_FAILURE);
    }

  ಆದರೆ (eolchar < 0)
    eolchar = '\n';

  ಗಣ_suffix_ಉದ್ದಳತೆ (n_units, ವಿಭಜಿಸು_type_2);

  /* Get out the filename arguments.  */

  ಆದರೆ (optind < argc)
    infile = argv[optind++];

  ಆದರೆ (optind < argc)
    outbase = argv[optind++];

  ಆದರೆ (optind < argc)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind]));
      usage (EXIT_FAILURE);
    }

  /* Check that the suffix length is large enough for the numerical
     suffix start value.  */
  ಆದರೆ (numeric_suffix_ಆರಂಭ && ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (numeric_suffix_ಆರಂಭ) > suffix_ಉದ್ದಳತೆ)
    {
      error (0, 0, _("numerical suffix start value is too large "
                     "for the suffix length"));
      usage (EXIT_FAILURE);
    }

  /* Open the input file.  */
  ಆದರೆ (! streq (infile, "-")
      && fd_reopen (STDIN_FILENO, infile, O_RDONLY, 0) < 0)
    error (EXIT_FAILURE, errno, _("cannot open %s for reading"),
           quoteaf (infile));

  /* Binary I/O is safer when byte counts are used.  */
  xset_ದ್ವಿಮಾನ_ಬಹುಲಕ (STDIN_FILENO, O_ದ್ವಿಮಾನ);

  /* Advise the kernel of our access pattern.  */
  fdadvise (STDIN_FILENO, 0, 0, FADVISE_SEQUENTIAL);

  /* Get the optimal block size of input device and make a buffer.  */

  ಆದರೆ (fstat (STDIN_FILENO, &ಒಳ_stat_buf) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (infile));

  ಆದರೆ (ಒಳ_blk_ಗಾತ್ರ == 0)
    {
      ಒಳ_blk_ಗಾತ್ರ = io_blksize (&ಒಳ_stat_buf);
      ಆದರೆ (SYS_BUFSIZE_MAX < ಒಳ_blk_ಗಾತ್ರ)
        ಒಳ_blk_ಗಾತ್ರ = SYS_BUFSIZE_MAX;
    }

  ಅಕ್ಷರ *buf = xalignalloc (ಪುಟ_ಗಾತ್ರ, ಒಳ_blk_ಗಾತ್ರ + 1);
  ssize_t initial_read = -1;

  ಆದರೆ (ವಿಭಜಿಸು_type_2 == type_chunk_ಬೈಟುಗಳು || ವಿಭಜಿಸು_type_2 == type_chunk_ಸಾಲುಗಳು)
    {
      file_ಗಾತ್ರ = ಒಳಹರಿವು_file_ಗಾತ್ರ (STDIN_FILENO, &ಒಳ_stat_buf,
                                   buf, ಒಳ_blk_ಗಾತ್ರ);
      ಆದರೆ (file_ಗಾತ್ರ < 0)
        error (EXIT_FAILURE, errno, _("%s: cannot determine file size"),
               quotef (infile));
      initial_read = MIN (file_ಗಾತ್ರ, ಒಳ_blk_ಗಾತ್ರ);
    }

  /* When filtering, closure of one pipe must not terminate the process,
     as there may still be other streams expecting input from us.  */
  ಆದರೆ (filter_command)
    default_SIGPIPE = ಸಂಕೇತ (SIGPIPE, SIG_IGN) == SIG_DFL;

  ಆಯ್ಕೆ (ವಿಭಜಿಸು_type_2)
    {
    ಸ್ಥಿತಿ type_digits:
    ಸ್ಥಿತಿ type_ಸಾಲುಗಳು:
      ಸಾಲುಗಳು_ವಿಭಜಿಸು (n_units, buf, ಒಳ_blk_ಗಾತ್ರ);
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ type_ಬೈಟುಗಳು:
      ಬೈಟುಗಳು_ವಿಭಜಿಸು (n_units, 0, buf, ಒಳ_blk_ಗಾತ್ರ, -1, 0);
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ type_byteslines:
      ಸಾಲು_ಬೈಟುಗಳು_ವಿಭಜಿಸು (n_units, buf, ಒಳ_blk_ಗಾತ್ರ);
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ type_chunk_ಬೈಟುಗಳು:
      ಆದರೆ (k_units == 0)
        ಬೈಟುಗಳು_ವಿಭಜಿಸು (file_ಗಾತ್ರ / n_units, file_ಗಾತ್ರ % n_units,
                     buf, ಒಳ_blk_ಗಾತ್ರ, initial_read, n_units);
      ಇಲ್ಲದಿದ್ದರೆ
        ಬೈಟುಗಳು_chunk_ಹೊರತೆಗೆ (k_units, n_units, buf, ಒಳ_blk_ಗಾತ್ರ, initial_read,
                             file_ಗಾತ್ರ);
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ type_chunk_ಸಾಲುಗಳು:
      ಸಾಲುಗಳು_chunk_ವಿಭಜಿಸು (k_units, n_units, buf, ಒಳ_blk_ಗಾತ್ರ, initial_read,
                         file_ಗಾತ್ರ);
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ type_rr:
      /* Note, this is like 'sed -n ${k}~${n}p' when k > 0,
         but the functionality is provided for symmetry.  */
      {
        ದ_t *files;
        ಸಾಲುಗಳು_rr (k_units, n_units, buf, ಒಳ_blk_ಗಾತ್ರ, &files);
      }
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ type_undef: ಪೂರ್ವನಿಯೋಜಿತ:
      affirm (false);
    }

  ಆದರೆ (close (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (infile));
  closeout (ಖಾಲಿ, output_desc, filter_pid, outfile);

  main_exit (EXIT_SUCCESS);
}
