/* Compute checksums of files or strings.
   Copyright (C) 1995-2026 Free Software Foundation, Inc.

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

/* Written by Ulrich Drepper <drepper@gnu.ai.mit.edu>.  */

#અન્તર્ભાવ <config.h>

#અન્તર્ભાવ <getopt.h>
#અન્તર્ભાવ <sys/types.h>

#અન્તર્ભાવ "assure.h"
#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "argmatch.h"
#અન્તર્ભાવ "c-ctype.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "xdectoint.h"
#અન્તર્ભાવ "xstrtol.h"

#ifndef HASH_ALGO_CKSUM
# define HASH_ALGO_CKSUM 0
#endif

#જો HASH_ALGO_SUM || HASH_ALGO_CKSUM
# include "sum.h"
#endif
#જો HASH_ALGO_CKSUM
# include "cksum.h"
# include "cksum_crc.h"
# include "base64.h"
#endif
#જો HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# include "blake2/b2sum.h"
#endif
#જો HASH_ALGO_MD5 || HASH_ALGO_CKSUM
# include "md5.h"
#endif
#જો HASH_ALGO_SHA1 || HASH_ALGO_CKSUM
# include "sha1.h"
#endif
#જો HASH_ALGO_SHA256 || HASH_ALGO_SHA224 || HASH_ALGO_CKSUM
# include "sha256.h"
#endif
#જો HASH_ALGO_SHA512 || HASH_ALGO_SHA384 || HASH_ALGO_CKSUM
# include "sha512.h"
#endif
#જો HASH_ALGO_CKSUM
# include "sha3.h"
#endif
#જો HASH_ALGO_CKSUM
# include "sm3.h"
#endif
#અન્તર્ભાવ "fadvise.h"
#અન્તર્ભાવ "stdio--.h"
#અન્તર્ભાવ "xbinary-io.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#જો HASH_ALGO_SUM
# define PROGRAM_NAME "sum"
# define DIGEST_TYPE_STRING "BSD"
# define DIGEST_STREAM sumfns[sum_algorithm]
# define DIGEST_OUT sum_output_fns[sum_algorithm]
# define DIGEST_BITS 16
# define DIGEST_ALIGN 4
#અથવાજો HASH_ALGO_CKSUM
# define MAX_DIGEST_BITS 512
# define MAX_DIGEST_ALIGN 8
# define DIGEST_TYPE_STRING algorithm_tags[cksum_algorithm]
# define DIGEST_STREAM cksumfns[cksum_algorithm]
# define DIGEST_OUT cksum_output_fns[cksum_algorithm]
# define DIGEST_BITS MAX_DIGEST_BITS
# define DIGEST_ALIGN MAX_DIGEST_ALIGN
#અથવાજો HASH_ALGO_MD5
# define PROGRAM_NAME "md5sum"
# define DIGEST_TYPE_STRING "MD5"
# define DIGEST_STREAM md5_stream
# define DIGEST_BITS 128
# define DIGEST_REFERENCE "RFC 1321"
# define DIGEST_ALIGN 4
#અથવાજો HASH_ALGO_BLAKE2
# define PROGRAM_NAME "b2sum"
# define DIGEST_TYPE_STRING "BLAKE2b"
# define DIGEST_STREAM blake2b_stream
# define DIGEST_BITS 512
# define DIGEST_REFERENCE "RFC 7693"
# define DIGEST_ALIGN 8
#અથવાજો HASH_ALGO_SHA1
# define PROGRAM_NAME "sha1sum"
# define DIGEST_TYPE_STRING "SHA1"
# define DIGEST_STREAM sha1_stream
# define DIGEST_BITS 160
# define DIGEST_REFERENCE "FIPS-180-1"
# define DIGEST_ALIGN 4
#અથવાજો HASH_ALGO_SHA256
# define PROGRAM_NAME "sha256sum"
# define DIGEST_TYPE_STRING "SHA256"
# define DIGEST_STREAM sha256_stream
# define DIGEST_BITS 256
# define DIGEST_REFERENCE "FIPS-180-2"
# define DIGEST_ALIGN 4
#અથવાજો HASH_ALGO_SHA224
# define PROGRAM_NAME "sha224sum"
# define DIGEST_TYPE_STRING "SHA224"
# define DIGEST_STREAM sha224_stream
# define DIGEST_BITS 224
# define DIGEST_REFERENCE "RFC 3874"
# define DIGEST_ALIGN 4
#અથવાજો HASH_ALGO_SHA512
# define PROGRAM_NAME "sha512sum"
# define DIGEST_TYPE_STRING "SHA512"
# define DIGEST_STREAM sha512_stream
# define DIGEST_BITS 512
# define DIGEST_REFERENCE "FIPS-180-2"
# define DIGEST_ALIGN 8
#અથવાજો HASH_ALGO_SHA384
# define PROGRAM_NAME "sha384sum"
# define DIGEST_TYPE_STRING "SHA384"
# define DIGEST_STREAM sha384_stream
# define DIGEST_BITS 384
# define DIGEST_REFERENCE "FIPS-180-2"
# define DIGEST_ALIGN 8
#નહીંતર
# error "Can't decide which hash algorithm to compile."
#endif
#જો !HASH_ALGO_SUM && !HASH_ALGO_CKSUM
# define DIGEST_OUT output_file
#endif

#જો HASH_ALGO_CKSUM
# define PROGRAM_NAME (!legacy_mode ? "cksum"			 \
                       : cksum_algorithm == md5 ? "md5sum"	 \
                       : cksum_algorithm == sha1 ? "sha1sum"	 \
                       : cksum_algorithm == sha224 ? "sha224sum" \
                       : cksum_algorithm == sha256 ? "sha256sum" \
                       : cksum_algorithm == sha384 ? "sha384sum" \
                       : cksum_algorithm == sha512 ? "sha512sum" \
                       : "cksum")
#endif

#જો HASH_ALGO_SUM
# define AUTHORS \
  proper_name ("Kayvan Aghaiepour"), \
  proper_name ("David MacKenzie")
#અથવાજો HASH_ALGO_CKSUM
# define AUTHORS \
  proper_name_lite ("Padraig Brady", "P\303\241draig Brady"), \
  proper_name ("Q. Frank Xia")
#અથવાજો HASH_ALGO_BLAKE2
# define AUTHORS \
  proper_name_lite ("Padraig Brady", "P\303\241draig Brady"), \
  proper_name ("Samuel Neves")
#નહીંતર
# define AUTHORS \
  proper_name ("Ulrich Drepper"), \
  proper_name ("Scott Miller"), \
  proper_name ("David Madore")
#endif
#જો !HASH_ALGO_BLAKE2 && !HASH_ALGO_CKSUM
# define DIGEST_HEX_BYTES (DIGEST_BITS / 4)
#endif
#define DIGEST_BIN_BYTES (DIGEST_BITS / 8)

/* The minimum length of a valid digest line.  This length does
   not include any newline character at the end of a line.  */
#જો HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# define MIN_DIGEST_લીટી_LENGTH 3 /* With -l 8.  */
#નહીંતર
# define MIN_DIGEST_લીટી_LENGTH \
   (DIGEST_HEX_BYTES /* length of hexadecimal message digest */ \
    + 1 /* blank */ \
    + 1 /* minimum filename length */ )
#endif

#જો !HASH_ALGO_SUM
સ્થિર શૂન્ય
output_file (અક્ષર અચલ *file, પૂર્ણાંક દ્વિઅંકી_file, શૂન્ય અચલ *digest,
             bool raw, bool tagged, unsigned અક્ષર delim, bool args,
             intmax_t length);
#endif

/* True if any of the files read were the standard input. */
સ્થિર bool have_read_stdin;

/* The minimum length of a valid checksum line for the selected algorithm.  */
સ્થિર idx_t min_digest_લીટી_length;

/* Set to the length of a digest hex string for the selected algorithm.  */
સ્થિર idx_t digest_hex_bytes;

/* With --check, don't generate any output.
   The exit code indicates success or failure.  */
સ્થિર bool status_only = false;

/* With --check, print a message to standard error warning about each
   improperly formatted checksum line.  */
સ્થિર bool warn = false;

/* With --check, ignore missing files.  */
સ્થિર bool ignore_missing = false;

/* With --check, suppress the "OK" printed for each verified file.  */
સ્થિર bool quiet = false;

/* With --check, exit with a non-zero return code if any line is
   improperly formatted. */
સ્થિર bool strict = false;

/* Whether a BSD reversed format checksum is detected.  */
સ્થિર પૂર્ણાંક bsd_reversed = -1;

/* line delimiter.  */
સ્થિર unsigned અક્ષર digest_delim = '\n';

#જો HASH_ALGO_CKSUM
/* If true, print base64-encoded digests, not hex.  */
સ્થિર bool base64_digest = false;
#endif

/* If true, print binary digests, not hex.  */
સ્થિર bool raw_digest = false;

/* blake2b and sha3 allow the -l option.  Luckily they both have the same
   maximum digest size.  */
#જો HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# જો HASH_ALGO_CKSUM
static_assert (BLAKE2B_OUTBYTES == SHA3_512_DIGEST_SIZE);
# endif
# define DIGEST_MAX_LEN BLAKE2B_OUTBYTES
સ્થિર idx_t digest_length;
#endif /* HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM */

પ્રકારવ્યાખ્યા શૂન્ય (*digest_output_fn)(અક્ષર અચલ *, પૂર્ણાંક, શૂન્ય અચલ *, bool,
                                 bool, unsigned અક્ષર, bool, intmax_t);
#જો HASH_ALGO_SUM
ગણના Algorithm
{
  bsd,
  sysv,
};

સ્થિર ગણના Algorithm sum_algorithm;
સ્થિર sumfn sumfns[]=
{
  bsd_sum_stream,
  sysv_sum_stream,
};
સ્થિર digest_output_fn sum_output_fns[]=
{
  output_bsd,
  output_sysv,
};
#endif

#જો HASH_ALGO_CKSUM
સ્થિર પૂર્ણાંક
md5_sum_stream (FILE *stream, શૂન્ય *resstream, MAYBE_UNUSED intmax_t *length)
{
  પાછા md5_stream (stream, resstream);
}
સ્થિર પૂર્ણાંક
sha1_sum_stream (FILE *stream, શૂન્ય *resstream, MAYBE_UNUSED intmax_t *length)
{
  પાછા sha1_stream (stream, resstream);
}
સ્થિર પૂર્ણાંક
sha224_sum_stream (FILE *stream, શૂન્ય *resstream,
                   MAYBE_UNUSED intmax_t *length)
{
  પાછા sha224_stream (stream, resstream);
}
સ્થિર પૂર્ણાંક
sha256_sum_stream (FILE *stream, શૂન્ય *resstream,
                   MAYBE_UNUSED intmax_t *length)
{
  પાછા sha256_stream (stream, resstream);
}
સ્થિર પૂર્ણાંક
sha384_sum_stream (FILE *stream, શૂન્ય *resstream,
                   MAYBE_UNUSED intmax_t *length)
{
  પાછા sha384_stream (stream, resstream);
}
સ્થિર પૂર્ણાંક
sha512_sum_stream (FILE *stream, શૂન્ય *resstream,
                   MAYBE_UNUSED intmax_t *length)
{
  પાછા sha512_stream (stream, resstream);
}
સ્થિર પૂર્ણાંક
sha2_sum_stream (FILE *stream, શૂન્ય *resstream, intmax_t *length)
{
  પસંદગી (*length)
    {
    સ્થિતિ SHA224_DIGEST_SIZE:
      પાછા sha224_stream (stream, resstream);
    સ્થિતિ SHA256_DIGEST_SIZE:
      પાછા sha256_stream (stream, resstream);
    સ્થિતિ SHA384_DIGEST_SIZE:
      પાછા sha384_stream (stream, resstream);
    સ્થિતિ SHA512_DIGEST_SIZE:
      પાછા sha512_stream (stream, resstream);
    મૂળભૂત:
      affirm (false);
    }
}
સ્થિર પૂર્ણાંક
sha3_sum_stream (FILE *stream, શૂન્ય *resstream, intmax_t *length)
{
  પસંદગી (*length)
    {
    સ્થિતિ SHA3_224_DIGEST_SIZE:
      પાછા sha3_224_stream (stream, resstream);
    સ્થિતિ SHA3_256_DIGEST_SIZE:
      પાછા sha3_256_stream (stream, resstream);
    સ્થિતિ SHA3_384_DIGEST_SIZE:
      પાછા sha3_384_stream (stream, resstream);
    સ્થિતિ SHA3_512_DIGEST_SIZE:
      પાછા sha3_512_stream (stream, resstream);
    મૂળભૂત:
      affirm (false);
    }
}
સ્થિર પૂર્ણાંક
blake2b_sum_stream (FILE *stream, શૂન્ય *resstream, intmax_t *length)
{
  પાછા blake2b_stream (stream, resstream, *length);
}
સ્થિર પૂર્ણાંક
sm3_sum_stream (FILE *stream, શૂન્ય *resstream, MAYBE_UNUSED intmax_t *length)
{
  પાછા sm3_stream (stream, resstream);
}

સ્થિર અક્ષર અચલ *અચલ algorithm_args[] =
{
  "bsd", "sysv", "crc", "crc32b", "md5", "sha1",
  "sha224", "sha256", "sha384", "sha512", /* Legacy naming */
  "sha2", "sha3", "blake2b", "sm3", ખાલી
};
સ્થિર ગણના Algorithm અચલ algorithm_types[] =
{
  bsd, sysv, crc, crc32b, md5, sha1,
  sha224, sha256, sha384, sha512,
  sha2, sha3, blake2b, sm3,
};
ARGMATCH_VERIFY (algorithm_args, algorithm_types);

સ્થિર અક્ષર અચલ *અચલ algorithm_tags[] =
{
  "BSD", "SYSV", "CRC", "CRC32B", "MD5", "SHA1",
  "SHA224", "SHA256", "SHA384", "SHA512",
  "SHA2", "SHA3", "BLAKE2b", "SM3", ખાલી
};
સ્થિર પૂર્ણાંક અચલ algorithm_bits[] =
{
  16, 16, 32, 32, 128, 160,
  224, 256, 384, 512,
  512, 512, 512, 256, 0
};

static_assert (countof (algorithm_bits) == countof (algorithm_args));

સ્થિર bool algorithm_specified = false;
સ્થિર bool legacy_mode = false;
ગણના Algorithm cksum_algorithm = crc;

સ્થિર sumfn cksumfns[]=
{
  bsd_sum_stream,
  sysv_sum_stream,
  crc_sum_stream,
  crc32b_sum_stream,
  md5_sum_stream,
  sha1_sum_stream,
  sha224_sum_stream,
  sha256_sum_stream,
  sha384_sum_stream,
  sha512_sum_stream,
  sha2_sum_stream,
  sha3_sum_stream,
  blake2b_sum_stream,
  sm3_sum_stream,
};
સ્થિર digest_output_fn cksum_output_fns[]=
{
  output_bsd,
  output_sysv,
  output_crc,
  output_crc,
  output_file,
  output_file,
  output_file,
  output_file,
  output_file,
  output_file,
  output_file,
  output_file,
  output_file,
  output_file,
};
bool cksum_debug;
#endif

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */

ગણના
{
  IGNORE_MISSING_OPTION = CHAR_MAX + 1,
  STATUS_OPTION,
  QUIET_OPTION,
  STRICT_OPTION,
  TAG_OPTION,
  UNTAG_OPTION,
  DEBUG_PROGRAM_OPTION,
  RAW_OPTION,
  BASE64_OPTION,
};

#જો HASH_ALGO_CKSUM
સ્થિર સંરચના option અચલ legacy_long_options[] =
{
  { "check", no_argument, ખાલી, 'c' },
  { "ignore-missing", no_argument, ખાલી, IGNORE_MISSING_OPTION },
  { "quiet", no_argument, ખાલી, QUIET_OPTION },
  { "status", no_argument, ખાલી, STATUS_OPTION },
  { "warn", no_argument, ખાલી, 'w' },
  { "strict", no_argument, ખાલી, STRICT_OPTION },
  { "tag", no_argument, ખાલી, TAG_OPTION },
  { "zero", no_argument, ખાલી, 'z' },

  { "binary", no_argument, ખાલી, 'b' }, /* Deprecated.  */
  { "text", no_argument, ખાલી, 't' }, /* Deprecated.  */

  { GETOPT_HELP_OPTION_DECL },
  { GETOPT_VERSION_OPTION_DECL },
  { ખાલી, 0, ખાલી, 0 }
};
#endif

સ્થિર સંરચના option અચલ long_options[] =
{
#જો HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
  { "length", required_argument, ખાલી, 'l'},
#endif

#જો !HASH_ALGO_SUM
  { "check", no_argument, ખાલી, 'c' },
  { "ignore-missing", no_argument, ખાલી, IGNORE_MISSING_OPTION},
  { "quiet", no_argument, ખાલી, QUIET_OPTION },
  { "status", no_argument, ખાલી, STATUS_OPTION },
  { "warn", no_argument, ખાલી, 'w' },
  { "strict", no_argument, ખાલી, STRICT_OPTION },
  { "tag", no_argument, ખાલી, TAG_OPTION },
  { "zero", no_argument, ખાલી, 'z' },

# જો HASH_ALGO_CKSUM
  { "algorithm", required_argument, ખાલી, 'a'},
  { "base64", no_argument, ખાલી, BASE64_OPTION },
  { "debug", no_argument, ખાલી, DEBUG_PROGRAM_OPTION},
  { "raw", no_argument, ખાલી, RAW_OPTION},
  { "untagged", no_argument, ખાલી, UNTAG_OPTION },
# endif
  { "binary", no_argument, ખાલી, 'b' }, /* Deprecated.  */
  { "text", no_argument, ખાલી, 't' }, /* Deprecated.  */

#નહીંતર
  {"sysv", no_argument, ખાલી, 's'},  /* Not in cksum.  */
#endif

  { GETOPT_HELP_OPTION_DECL },
  { GETOPT_VERSION_OPTION_DECL },
  { ખાલી, 0, ખાલી, 0 }
};

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"), program_name);
#જો HASH_ALGO_CKSUM
    જો (legacy_mode)
      {
        printf (_("\
Print or check %s checksums.\n\
"), DIGEST_TYPE_STRING);
        fputs (_("\
Legacy interface to the cksum(1) utility.\n\
"), stdout);
      }
    નહીંતર
      fputs (_("\
Print or verify checksums.\n\
By મૂળભૂત use the 32 bit CRC algorithm.\n\
"), stdout);
#નહીંતર
      printf (_("\
Print or check %s (%d-bit) checksums.\n\
"),
              DIGEST_TYPE_STRING,
              DIGEST_BITS);
      fputs (_("\
Legacy interface to the cksum(1) utility.\n\
"), stdout);
#endif

      emit_stdin_note ();
#જો HASH_ALGO_SUM
      oputs (_("\
  -r\n\
         use BSD sum algorithm (the મૂળભૂત), use 1K blocks\n\
"));
      oputs (_("\
  -s, --sysv\n\
         use System V sum algorithm, use 512 bytes blocks\n\
"));
#endif
#જો HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
        emit_mandatory_arg_note ();
#endif
#જો HASH_ALGO_CKSUM
     જો (! legacy_mode)
        oputs (_("\
  -a, --algorithm=TYPE\n\
         select the digest type to use.  See DIGEST below\n\
"));
     જો (! legacy_mode)
        oputs (_("\
      --base64\n\
         emit base64-encoded digests, not hexadecimal\n\
"));
#endif
#જો !HASH_ALGO_SUM
# જો HASH_ALGO_CKSUM
     જો (legacy_mode)
       {
# endif
      જો (O_દ્વિઅંકી)
        oputs (_("\
  -b, --દ્વિઅંકી\n\
         પઠ in દ્વિઅંકી mode (મૂળભૂત unless reading tty stdin)\n\
"));
      નહીંતર
        oputs (_("\
  -b, --દ્વિઅંકી\n\
         પઠ in દ્વિઅંકી mode\n\
"));
# જો HASH_ALGO_CKSUM
       }
# endif
        oputs (_("\
  -c, --check\n\
         પઠ checksums from the FILEs and check them\n\
"));
# જો HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
#  જો HASH_ALGO_CKSUM
     જો (! legacy_mode)
#  endif
        oputs (_("\
  -l, --length=BITS\n\
         digest length in bits; must not exceed the max size\n\
         and must be a multiple of 8 માટે blake2b;\n\
         must be 224, 256, 384, or 512 માટે sha2 or sha3\n\
"));
# endif
# જો HASH_ALGO_CKSUM
    જો (! legacy_mode)
        oputs (_("\
      --raw\n\
         emit a raw દ્વિઅંકી digest, not hexadecimal\n\
"));
    જો (legacy_mode)
      oputs (_("\
      --tag\n\
         create a BSD-style checksum\n\
"));
    નહીંતર
      oputs (_("\
      --tag\n\
         create a BSD-style checksum (the મૂળભૂત)\n\
"));
    જો (! legacy_mode)
      oputs (_("\
      --untagged\n\
         create a reversed style checksum, without digest type\n\
"));
# નહીંતર
      oputs (_("\
      --tag\n\
         create a BSD-style checksum\n\
"));
# endif
# જો HASH_ALGO_CKSUM
    જો (legacy_mode)
      {
# endif
      જો (O_દ્વિઅંકી)
        oputs (_("\
  -t, --text\n\
         પઠ in text mode (મૂળભૂત જો reading tty stdin)\n\
"));
      નહીંતર
        oputs (_("\
  -t, --text\n\
         પઠ in text mode (મૂળભૂત)\n\
"));
# જો HASH_ALGO_CKSUM
      }
# endif
      oputs (_("\
  -z, --zero\n\
         end each output લીટી with NUL, not newline,\n\
         and disable file name escaping\n\
"));
      fputs (_("\
\n\
The following five options are useful only when verifying checksums:\n\
"), stdout);
      oputs (_("\
      --ignore-missing\n\
         don't fail or report status માટે missing files\n\
"));
      oputs (_("\
      --quiet\n\
         don't print OK માટે each successfully verified file\n\
"));
      oputs (_("\
      --status\n\
         don't output anything, status code shows success\n\
"));
      oputs (_("\
      --strict\n\
         exit non-zero માટે improperly formatted checksum લીટીઓ\n\
"));
      oputs (_("\
  -w, --warn\n\
         warn about improperly formatted checksum લીટીઓ\n\
"));
#endif
#જો HASH_ALGO_CKSUM
    જો (! legacy_mode)
      oputs (_("\
      --debug\n\
         indicate which implementation used\n\
"));
#endif
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
#જો HASH_ALGO_CKSUM
    જો (! legacy_mode)
      fputs (_("\
\n\
DIGEST determines the digest algorithm and મૂળભૂત output format:\n\
  sysv      (equivalent to sum -s)\n\
  bsd       (equivalent to sum -r)\n\
  crc       (equivalent to cksum)\n\
  crc32b    (only available through cksum)\n\
  md5       (equivalent to md5sum)\n\
  sha1      (equivalent to sha1sum)\n\
  sha2      (equivalent to sha{224,256,384,512}sum)\n\
  sha3      (only available through cksum)\n\
  blake2b   (equivalent to b2sum)\n\
  sm3       (only available through cksum)\n\
\n"), stdout);
#endif
#જો !HASH_ALGO_SUM && !HASH_ALGO_CKSUM
      printf (_("\
\n\
The sums are computed as described in %s.\n"), DIGEST_REFERENCE);
      fputs (_("\
When checking, the આગત should be a former output of this program.\n\
The મૂળભૂત mode is to print a લીટી with: checksum, a space,\n\
a character indicating આગત mode ('*' માટે દ્વિઅંકી, ' ' માટે text\n\
or where દ્વિઅંકી is insignificant), and name માટે each FILE.\n\
\n\
There is no difference between દ્વિઅંકી mode and text mode on GNU systems.\
\n"), stdout);
#endif
#જો HASH_ALGO_CKSUM
      fputs (_("\
When checking, the આગત should be a former output of this program,\n\
or equivalent standalone program.\
\n"), stdout);
#endif
      emit_ancillary_info (PROGRAM_NAME);
    }

  exit (status);
}

#જો !HASH_ALGO_SUM
/* Given a string S, return TRUE if it contains problematic characters
   that need escaping.  Note we escape '\' itself to provide some forward
   compat to introduce escaping of other characters.  */

ATTRIBUTE_PURE
સ્થિર bool
problematic_chars (અક્ષર અચલ *s)
{
  idx_t length = strcspn (s, "\\\n\r");
  પાછા s[length] != '\0';
}
#endif

/* Given a file name, S of length S_LEN, that is not NUL-terminated,
   modify it in place, performing the equivalent of this sed substitution:
   's/\\n/\n/g;s/\\r/\r/g;s/\\\\/\\/g' i.e., replacing each "\\n" string
   with a newline, each "\\r" string with a carriage return,
   and each "\\\\" with a single backslash, NUL-terminate it and return S.
   If S is not a valid escaped file name, i.e., if it ends with an odd number
   of backslashes or if it contains a backslash followed by anything other
   than "n" or another backslash, return NULL.  */

સ્થિર અક્ષર *
filename_unescape (અક્ષર *s, idx_t s_len)
{
  અક્ષર *dst = s;

  માટે (idx_t i = 0; i < s_len; i++)
    {
      પસંદગી (s[i])
        {
        સ્થિતિ '\\':
          જો (i == s_len - 1)
            {
              /* File name ends with an unescaped backslash: invalid.  */
              પાછા ખાલી;
            }
          ++i;
          પસંદગી (s[i])
            {
            સ્થિતિ 'n':
              *dst++ = '\n';
              તોડો;
            સ્થિતિ 'r':
              *dst++ = '\r';
              તોડો;
            સ્થિતિ '\\':
              *dst++ = '\\';
              તોડો;
            મૂળભૂત:
              /* Only '\', 'n' or 'r' may follow a backslash.  */
              પાછા ખાલી;
            }
          તોડો;

        સ્થિતિ '\0':
          /* The file name may not contain a NUL.  */
          પાછા ખાલી;

        મૂળભૂત:
          *dst++ = s[i];
          તોડો;
        }
    }
  જો (dst < s + s_len)
    *dst = '\0';

  પાછા s;
}

/* Return true if S is a LEN-byte NUL-terminated string of hex or base64
   digits and has the expected length.  Otherwise, return false.  */
ATTRIBUTE_PURE
સ્થિર bool
valid_digits (unsigned અક્ષર અચલ *s, idx_t len)
{
#જો HASH_ALGO_CKSUM
  જો (len == BASE64_LENGTH (digest_length >> 3))
    {
      idx_t i;
      માટે (i = 0; i < len - digest_length % 3; i++)
        {
          જો (!isbase64 (*s))
            પાછા false;
          ++s;
        }
      માટે ( ; i < len; i++)
        {
          જો (*s != '=')
            પાછા false;
          ++s;
        }
    }
  નહીંતર
#endif
  જો (len == digest_hex_bytes)
    {
      માટે (idx_t i = 0; i < digest_hex_bytes; i++)
        {
          જો (!c_isxdigit (*s))
            પાછા false;
          ++s;
        }
    }
  નહીંતર
    પાછા false;

  પાછા *s == '\0';
}

/* Split the checksum string S (of length S_LEN) from a BSD 'md5' or
   'sha1' command into two parts: a hexadecimal digest, and the file
   name.  S is modified.  Set *D_LEN to the length of the digest string.
   Return true if successful.  */

સ્થિર bool
bsd_split_3 (અક્ષર *s, idx_t s_len,
             unsigned અક્ષર **digest, idx_t *d_len,
             અક્ષર **file_name, bool escaped_filename)
{
  જો (s_len == 0)
    પાછા false;

  /* Find end of filename.  */
  idx_t i = s_len - 1;
  જ્યારે (i && s[i] != ')')
    i--;

  જો (s[i] != ')')
    પાછા false;

  *file_name = s;

  જો (escaped_filename && filename_unescape (s, i) == ખાલી)
    પાછા false;

  s[i++] = '\0';

  જ્યારે (c_isblank (s[i]))
    i++;

  જો (s[i] != '=')
    પાછા false;

  i++;

  જ્યારે (c_isblank (s[i]))
    i++;

  *digest = (unsigned અક્ષર *) &s[i];

  *d_len = s_len - i;
  પાછા valid_digits (*digest, *d_len);
}

#જો HASH_ALGO_CKSUM
/* Return the corresponding Algorithm for the string S,
   or -1 for no match.  */

સ્થિર ptrdiff_t
algorithm_from_tag (અક્ષર *s)
{
  /* Limit check size to this length for perf reasons.  */
  સ્થિર idx_t max_tag_len;
  જો (! max_tag_len)
    {
      અક્ષર અચલ * અચલ * tag = algorithm_tags;
      જ્યારે (*tag)
        {
          idx_t tag_len = strlen (*tag++);
          max_tag_len = MAX (tag_len, max_tag_len);
        }
    }

  idx_t i = 0;

  /* Find end of tag */
  જ્યારે (i <= max_tag_len && s[i] && ! c_isblank (s[i])
         && s[i] != '-' && s[i] != '(')
    ++i;

  જો (i > max_tag_len)
    પાછા -1;

  /* Terminate tag, and lookup.  */
  અક્ષર sep = s[i];
  s[i] = '\0';
  ptrdiff_t algo = argmatch_exact (s, algorithm_tags);
  s[i] = sep;

  પાછા algo;
}
#endif

/* Split the string S (of length S_LEN) into three parts:
   a hexadecimal digest, binary flag, and the file name.
   S is modified.  Set *D_LEN to the length of the digest string.
   Return true if successful.  */

સ્થિર bool
split_3 (અક્ષર *s, idx_t s_len,
         unsigned અક્ષર **digest, idx_t *d_len, પૂર્ણાંક *દ્વિઅંકી, અક્ષર **file_name)
{
  bool escaped_filename = false;
  idx_t algo_name_len;

  idx_t i = 0;
  જ્યારે (c_isblank (s[i]))
    ++i;

  જો (s[i] == '\\')
    {
      ++i;
      escaped_filename = true;
    }

  /* Check for BSD-style checksum line. */

#જો HASH_ALGO_CKSUM
  જો (! algorithm_specified || cksum_algorithm == sha2)
    {
      ptrdiff_t algo_tag = algorithm_from_tag (s + i);
      જો (! algorithm_specified)
        {
          જો (algo_tag >= 0)
            {
              જો (algo_tag <= crc32b)
                પાછા false;  /* We don't support checking these formats.  */
              cksum_algorithm = algo_tag;
            }
          નહીંતર
            પાછા false;      /* We only support tagged format without -a.  */
        }
      નહીંતર
        {
          જો (cksum_algorithm == sha2 && (algo_tag == sha2
              || algo_tag == sha224 || algo_tag == sha256
              || algo_tag == sha384 || algo_tag == sha512))
            cksum_algorithm = algo_tag;
        }
    }
#endif

  /* Try to parse BSD or OpenSSL tagged format.  I.e.:
     openssl: MD5(f)= d41d8cd98f00b204e9800998ecf8427e
     bsd:     MD5 (f) = d41d8cd98f00b204e9800998ecf8427e  */

  idx_t parse_offset = i;
  algo_name_len = strlen (DIGEST_TYPE_STRING);
  જો (STREQ_LEN (s + i, DIGEST_TYPE_STRING, algo_name_len))
    {
      i += algo_name_len;
#જો HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM

# જો HASH_ALGO_BLAKE2
      digest_length = DIGEST_MAX_LEN * 8;
# નહીંતર
      digest_length = algorithm_bits[cksum_algorithm];
# endif
      જો (s[i] == '-')  /* length specified. Not base64 */
        {
          ++i;
          intmax_t length;
          અક્ષર *siend;
          જો (!c_isdigit (s[i])
              || xstrtoimax (s + i, &siend, 0, &length, ખાલી) != LONGINT_OK)
            પાછા false;
# જો HASH_ALGO_CKSUM
          નહીંતર જો (cksum_algorithm == sha2 || cksum_algorithm == sha3)
            {
              જો (length != SHA224_DIGEST_SIZE * 8
                  && length != SHA256_DIGEST_SIZE * 8
                  && length != SHA384_DIGEST_SIZE * 8
                  && length != SHA512_DIGEST_SIZE * 8)
                પાછા false;
            }
# endif
          નહીંતર જો (!(0 < length && length <= digest_length && length % 8 == 0))
            પાછા false;

          i = siend - s;
          digest_length = length;
        }
      digest_hex_bytes = digest_length >> 2;
#endif
      જો (s[i] == ' ')
        ++i;
      જો (s[i] == '(')  /* not base64 */
        {
          ++i;
          *દ્વિઅંકી = 0;
          પાછા bsd_split_3 (s + i, s_len - i,
                              digest, d_len, file_name, escaped_filename);
        }

      /* Note with --base64 --untagged format, we may have matched a "tag".
         Even very short digests with: cksum -a blake2b -l24 --untagged --base64
         So fallback to checking untagged format if issues detecting tags.  */
      i = parse_offset;
    }

  /* Ignore this line if it is too short.
     Each line must have at least 'min_digest_line_length - 1' (or one more, if
     the first is a backslash) more characters to contain correct message digest
     information.  */
  જો (s_len - i < min_digest_લીટી_length + (s[i] == '\\'))
    પાછા false;

  *digest = (unsigned અક્ષર *) &s[i];

#જો HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
  /* Auto determine length.  */
# જો HASH_ALGO_CKSUM
  જો (cksum_algorithm == blake2b
      || cksum_algorithm == sha2 || cksum_algorithm == sha3) {
# endif
  unsigned અક્ષર અચલ *hp = *digest;
  digest_hex_bytes = 0;
  માટે (; c_isxdigit (*hp); ++hp, ++digest_hex_bytes)
    ;
# જો HASH_ALGO_CKSUM
  /* Check the number of base64 characters.  This works because the hexadecimal
     character set is a subset of the base64 character set.
     Note there is the ambiguity that all characters are hex when they
     are actually base64 encoded, which could be ambiguous with:
        cksum -a sha2 -l 384 --base64 --untagged
        cksum -a sha2 -l 256 --untagged
     Similarly for sha3 and blake2b.
     However at this length the chances are exceedingly rare (1 in 480R),
     and smaller blake2b lengths aren't practical for verification anyway.  */
  idx_t digest_base64_bytes = digest_hex_bytes;
  idx_t trailing_equals = 0;
  માટે (; isubase64 (*hp); ++hp, ++digest_base64_bytes)
    ;
  માટે (; *hp == '='; ++hp, ++trailing_equals)
    ;
  જો ((cksum_algorithm == sha2 || cksum_algorithm == sha3)
      && digest_hex_bytes >> 1 != SHA224_DIGEST_SIZE
      && digest_hex_bytes >> 1 != SHA256_DIGEST_SIZE
      && digest_hex_bytes >> 1 != SHA384_DIGEST_SIZE
      && digest_hex_bytes >> 1 != SHA512_DIGEST_SIZE)
    {
      જો (digest_base64_bytes + trailing_equals
          == BASE64_LENGTH (SHA224_DIGEST_SIZE))
        digest_hex_bytes = SHA224_DIGEST_SIZE * 2;
      નહીંતર જો (digest_base64_bytes + trailing_equals
               == BASE64_LENGTH (SHA256_DIGEST_SIZE))
        digest_hex_bytes = SHA256_DIGEST_SIZE * 2;
      નહીંતર જો (digest_base64_bytes + trailing_equals
               == BASE64_LENGTH (SHA384_DIGEST_SIZE))
        digest_hex_bytes = SHA384_DIGEST_SIZE * 2;
      નહીંતર જો (digest_base64_bytes + trailing_equals
               == BASE64_LENGTH (SHA512_DIGEST_SIZE))
        digest_hex_bytes = SHA512_DIGEST_SIZE * 2;
      નહીંતર
        પાછા false;
    }
  નહીંતર જો (cksum_algorithm == blake2b
           && digest_hex_bytes < digest_base64_bytes)
    {
      માટે (પૂર્ણાંક j = 8; j <= DIGEST_MAX_LEN * 8; j += 8)
        {
          જો (BASE64_LENGTH (j / 8) == digest_base64_bytes + trailing_equals
              && j % 3 == trailing_equals)
            {
              digest_hex_bytes = j / 4;
              તોડો;
            }
        }
    }
# endif
  જો (digest_hex_bytes < 2 || digest_hex_bytes % 2
      || DIGEST_MAX_LEN * 2 < digest_hex_bytes)
    પાછા false;
  digest_length = digest_hex_bytes * 4;
# જો HASH_ALGO_CKSUM
  }
# endif
#endif

  /* This field must be the hexadecimal or base64 representation
     of the message digest.  */
  જ્યારે (s[i] && !c_isblank (s[i]))
    i++;

  /* The digest must be followed by at least one whitespace character.  */
  જો (i == s_len)
    પાછા false;

  *d_len = &s[i] - (અક્ષર *) *digest;
  s[i++] = '\0';

  જો (! valid_digits (*digest, *d_len))
    પાછા false;

  /* If "bsd reversed" format detected.  */
  જો ((s_len - i == 1) || (s[i] != ' ' && s[i] != '*'))
    {
      /* Don't allow mixing bsd and standard formats,
         to minimize security issues with attackers
         renaming files with leading spaces.
         This assumes that with bsd format checksums
         that the first file name does not have
         a leading ' ' or '*'.  */
      જો (bsd_reversed == 0)
        પાછા false;
      bsd_reversed = 1;
    }
  નહીંતર જો (bsd_reversed != 1)
    {
      bsd_reversed = 0;
      *દ્વિઅંકી = (s[i++] == '*');
    }

  /* All characters between the type indicator and end of line are
     significant -- that includes leading and trailing white space.  */
  *file_name = &s[i];

  જો (escaped_filename)
    પાછા filename_unescape (&s[i], s_len - i) != ખાલી;

  પાછા true;
}

#જો !HASH_ALGO_SUM
/* If ESCAPE is true, then translate each:
   NEWLINE byte to the string, "\\n",
   CARRIAGE RETURN byte to the string, "\\r",
   and each backslash to "\\\\".  */
સ્થિર શૂન્ય
print_filename (અક્ષર અચલ *file, bool escape)
{
  જો (! escape)
    {
      fputs (file, stdout);
      પાછા;
    }

  જ્યારે (*file)
    {
      પસંદગી (*file)
        {
        સ્થિતિ '\n':
          fputs ("\\n", stdout);
          તોડો;

        સ્થિતિ '\r':
          fputs ("\\r", stdout);
          તોડો;

        સ્થિતિ '\\':
          fputs ("\\\\", stdout);
          તોડો;

        મૂળભૂત:
          putchar (*file);
          તોડો;
        }
      file++;
    }
}
#endif

/* An interface to the function, DIGEST_STREAM.
   Operate on FILENAME (it may be "-").

   *BINARY indicates whether the file is binary.  BINARY < 0 means it
   depends on whether binary mode makes any difference and the file is
   a terminal; in that case, clear *BINARY if the file was treated as
   text because it was a terminal.

   Put the checksum in *BIN_RESULT, which must be properly aligned.
   Put true in *MISSING if the file can't be opened due to ENOENT.
   Return true if successful.  */

સ્થિર bool
digest_file (અક્ષર અચલ *filename, પૂર્ણાંક *દ્વિઅંકી, unsigned અક્ષર *bin_result,
             bool *missing, MAYBE_UNUSED intmax_t *length)
{
  FILE *fp;
  પૂર્ણાંક err;
  bool is_stdin = streq (filename, "-");

  *missing = false;

  જો (is_stdin)
    {
      have_read_stdin = true;
      fp = stdin;
      જો (O_દ્વિઅંકી && *દ્વિઅંકી)
        {
          જો (*દ્વિઅંકી < 0)
            *દ્વિઅંકી = ! isatty (STDIN_FILENO);
          જો (*દ્વિઅંકી)
            xset_દ્વિઅંકી_mode (STDIN_FILENO, O_દ્વિઅંકી);
        }
    }
  નહીંતર
    {
      fp = fopen (filename, O_દ્વિઅંકી ? (*દ્વિઅંકી ? "rb" : "rt") : "r");
      જો (fp == ખાલી)
        {
          જો (ignore_missing && errno == ENOENT)
            {
              *missing = true;
              પાછા true;
            }
          error (0, errno, "%s", quotef (filename));
          પાછા false;
        }
    }

  fadvise (fp, FADVISE_SEQUENTIAL);

#જો HASH_ALGO_CKSUM
  જો (cksum_algorithm == blake2b
      || cksum_algorithm == sha2 || cksum_algorithm == sha3)
    *length = digest_length >> 3;
  err = DIGEST_STREAM (fp, bin_result, length);
#અથવાજો HASH_ALGO_SUM
  err = DIGEST_STREAM (fp, bin_result, length);
#અથવાજો HASH_ALGO_BLAKE2
  err = DIGEST_STREAM (fp, bin_result, digest_length >> 3);
#નહીંતર
  err = DIGEST_STREAM (fp, bin_result);
#endif
  err = err ? errno : 0;
  જો (is_stdin)
    clearerr (fp);
  નહીંતર જો (fclose (fp) != 0 && !err)
    err = errno;

  જો (err)
    {
      error (0, err, "%s", quotef (filename));
      પાછા false;
    }

  પાછા true;
}

#જો !HASH_ALGO_SUM
સ્થિર શૂન્ય
output_file (અક્ષર અચલ *file, પૂર્ણાંક દ્વિઅંકી_file, શૂન્ય અચલ *digest,
             MAYBE_UNUSED bool raw, bool tagged, unsigned અક્ષર delim,
             MAYBE_UNUSED bool args, MAYBE_UNUSED intmax_t length)
{
# જો HASH_ALGO_CKSUM
  જો (raw)
    {
      fwrite (digest, 1, digest_length >> 3, stdout);
      પાછા;
    }
# endif

  unsigned અક્ષર અચલ *bin_buffer = digest;

  /* Output a leading backslash if the file name contains problematic chars.  */
  bool needs_escape = delim == '\n' && problematic_chars (file);

  જો (needs_escape)
    putchar ('\\');

  જો (tagged)
    {
# જો HASH_ALGO_CKSUM
      જો (cksum_algorithm == sha2)
        printf ("SHA%u", (unsigned પૂર્ણાંક) digest_length);
      નહીંતર
# endif
      fputs (DIGEST_TYPE_STRING, stdout);
# જો HASH_ALGO_BLAKE2
      જો (digest_length < DIGEST_MAX_LEN * 8)
        printf ("-%u", (unsigned પૂર્ણાંક) digest_length);
# elif HASH_ALGO_CKSUM
      જો (cksum_algorithm == sha3)
        printf ("-%u", (unsigned પૂર્ણાંક) digest_length);
      જો (cksum_algorithm == blake2b)
        {
          જો (digest_length < DIGEST_MAX_LEN * 8)
            printf ("-%u", (unsigned પૂર્ણાંક) digest_length);
        }
# endif
      fputs (" (", stdout);
      print_filename (file, needs_escape);
      fputs (") = ", stdout);
    }

# જો HASH_ALGO_CKSUM
  જો (base64_digest)
    {
      અક્ષર b64[BASE64_LENGTH (DIGEST_BIN_BYTES) + 1];
      base64_encode ((અક્ષર અચલ *) bin_buffer, digest_length >> 3,
                     b64, કદ b64);
      fputs (b64, stdout);
    }
  નહીંતર
# endif
    {
      માટે (idx_t i = 0; i < (digest_hex_bytes >> 1); ++i)
        printf ("%02x", bin_buffer[i]);
    }

  જો (!tagged)
    {
      putchar (' ');
      putchar (દ્વિઅંકી_file ? '*' : ' ');
      print_filename (file, needs_escape);
    }

  putchar (delim);
}
#endif

#જો HASH_ALGO_CKSUM
/* Return true if B64_DIGEST is the same as the base64 digest of the
   DIGEST_LENGTH/8 bytes at BIN_BUFFER.  */
સ્થિર bool
b64_equal (unsigned અક્ષર અચલ *b64_digest, unsigned અક્ષર અચલ *bin_buffer)
{
  idx_t b64_n_bytes = BASE64_LENGTH (digest_length >> 3);
  અક્ષર b64[BASE64_LENGTH (DIGEST_BIN_BYTES) + 1];
  base64_encode ((અક્ષર અચલ *) bin_buffer, digest_length >> 3,
                 b64, કદ b64);
  પાછા memeq (b64_digest, b64, b64_n_bytes + 1);
}
#endif

/* Return true if HEX_DIGEST is the same as the hex-encoded digest of the
   DIGEST_LENGTH/8 bytes at BIN_BUFFER.  */
સ્થિર bool
hex_equal (unsigned અક્ષર અચલ *hex_digest, unsigned અક્ષર અચલ *bin_buffer)
{
  સ્થિર અચલ અક્ષર bin2hex[] = { '0', '1', '2', '3',
                                  '4', '5', '6', '7',
                                  '8', '9', 'a', 'b',
                                  'c', 'd', 'e', 'f' };
  idx_t digest_bin_bytes = digest_hex_bytes >> 1;

  /* Compare generated binary number with text representation
     in check file.  Ignore case of hex digits.  */
  idx_t cnt;
  માટે (cnt = 0; cnt < digest_bin_bytes; ++cnt)
    {
      જો (c_tolower (hex_digest[2 * cnt])
          != bin2hex[bin_buffer[cnt] >> 4]
          || (c_tolower (hex_digest[2 * cnt + 1])
              != (bin2hex[bin_buffer[cnt] & 0xf])))
        તોડો;
    }
  પાછા cnt == digest_bin_bytes;
}

સ્થિર bool
digest_check (અક્ષર અચલ *checkfile_name)
{
  FILE *checkfile_stream;
  intmax_t n_misformatted_લીટીઓ = 0;
  intmax_t n_mismatched_checksums = 0;
  intmax_t n_open_or_read_failures = 0;
  bool properly_formatted_લીટીઓ = false;
  bool matched_checksums = false;
  unsigned અક્ષર bin_buffer_unaligned[DIGEST_BIN_BYTES + DIGEST_ALIGN];
  /* Make sure bin_buffer is properly aligned. */
  unsigned અક્ષર *bin_buffer = ptr_align (bin_buffer_unaligned, DIGEST_ALIGN);
  intmax_t લીટી_સંખ્યા;
  અક્ષર *લીટી;
  size_t લીટી_chars_allocated;
  bool is_stdin = streq (checkfile_name, "-");

  જો (is_stdin)
    {
      have_read_stdin = true;
      checkfile_name = _("standard input");
      checkfile_stream = stdin;
    }
  નહીંતર
    {
      checkfile_stream = fopen (checkfile_name, "r");
      જો (checkfile_stream == ખાલી)
        {
          error (0, errno, "%s", quotef (checkfile_name));
          પાછા false;
        }
    }

  લીટી_સંખ્યા = 0;
  લીટી = ખાલી;
  લીટી_chars_allocated = 0;
  કરો
    {
      અક્ષર *filename;
      પૂર્ણાંક દ્વિઅંકી;
      unsigned અક્ષર *digest;
      ssize_t લીટી_length;

      ++લીટી_સંખ્યા;
      જો (લીટી_સંખ્યા == 0)
        error (EXIT_FAILURE, 0, _("%s: too many checksum lines"),
               quotef (checkfile_name));

      લીટી_length = getline (&લીટી, &લીટી_chars_allocated, checkfile_stream);
      જો (લીટી_length <= 0)
        તોડો;

      /* Ignore comment lines, which begin with a '#' character.  */
      જો (લીટી[0] == '#')
        ચાલુ;

      /* Remove any trailing newline.  */
      લીટી_length -= લીટી[લીટી_length - 1] == '\n';
      /* Remove any trailing carriage return.  */
      લીટી_length -= લીટી[લીટી_length - (0 < લીટી_length)] == '\r';

      /* Ignore empty lines.  */
      જો (લીટી_length == 0)
        ચાલુ;

      લીટી[લીટી_length] = '\0';

      idx_t d_len;
      જો (! (split_3 (લીટી, લીટી_length, &digest, &d_len, &દ્વિઅંકી, &filename)
             && ! (is_stdin && streq (filename, "-"))))
        {
          ++n_misformatted_લીટીઓ;

          જો (warn)
            {
              error (0, 0,
                     _("%s: %jd"
                       ": improperly formatted %s checksum line"),
                     quotef (checkfile_name), લીટી_સંખ્યા,
                     DIGEST_TYPE_STRING);
            }
        }
      નહીંતર
        {
          bool ok;
          bool missing;

          properly_formatted_લીટીઓ = true;

          intmax_t length;
          ok = digest_file (filename, &દ્વિઅંકી, bin_buffer, &missing, &length);

          જો (!ok)
            {
              ++n_open_or_read_failures;
              જો (!status_only)
                printf ("%s: %s\n", quotef (filename),_("FAILED open or read"));
            }
          નહીંતર જો (ignore_missing && missing)
            {
              /* Ignore missing files with --ignore-missing.  */
              ;
            }
          નહીંતર
            {
              bool match = false;
#જો HASH_ALGO_CKSUM
              જો (d_len == BASE64_LENGTH (digest_length >> 3))
                match = b64_equal (digest, bin_buffer);
              નહીંતર
#endif
                જો (d_len == digest_hex_bytes)
                  match = hex_equal (digest, bin_buffer);

              જો (match)
                matched_checksums = true;
              નહીંતર
                ++n_mismatched_checksums;

              જો (!status_only)
                {
                  જો (! match || ! quiet)
                    fputs (quotef (filename), stdout);

                  જો (! match)
                    printf (": %s\n", _("FAILED"));
                  નહીંતર જો (!quiet)
                    printf (": %s\n", _("OK"));
                }
            }

          જો (ferror (stdout))
            write_error ();
        }
    }
  જ્યારે (!feof (checkfile_stream) && !ferror (checkfile_stream));

  મુક્ત (લીટી);

  પૂર્ણાંક err = ferror (checkfile_stream) ? 0 : -1;
  જો (is_stdin)
    clearerr (checkfile_stream);
  નહીંતર જો (fclose (checkfile_stream) != 0 && err < 0)
    err = errno;

  જો (0 <= err)
    {
      error (0, err, err ? "%s" : _("%s: read error"),
             quotef (checkfile_name));
      પાછા false;
    }

  જો (! properly_formatted_લીટીઓ)
    {
      /* Warn if no tests are found.  */
      error (0, 0, _("%s: no properly formatted checksum lines found"),
             quotef (checkfile_name));
    }
  નહીંતર
    {
      જો (!status_only)
        {
          જો (n_misformatted_લીટીઓ != 0)
            error (0, 0,
                   (ngettext
                    ("WARNING: %jd line is improperly formatted",
                     "WARNING: %jd lines are improperly formatted",
                     select_plural (n_misformatted_લીટીઓ))),
                   n_misformatted_લીટીઓ);

          જો (n_open_or_read_failures != 0)
            error (0, 0,
                   (ngettext
                    ("WARNING: %jd listed file could not be read",
                     "WARNING: %jd listed files could not be read",
                     select_plural (n_open_or_read_failures))),
                   n_open_or_read_failures);

          જો (n_mismatched_checksums != 0)
            error (0, 0,
                   (ngettext
                    ("WARNING: %jd computed checksum did NOT match",
                     "WARNING: %jd computed checksums did NOT match",
                     select_plural (n_mismatched_checksums))),
                   n_mismatched_checksums);

          જો (ignore_missing && ! matched_checksums)
            error (0, 0, _("%s: no file was verified"),
                   quotef (checkfile_name));
        }
    }

  પાછા (properly_formatted_લીટીઓ
          && matched_checksums
          && n_mismatched_checksums == 0
          && n_open_or_read_failures == 0
          && (!strict || n_misformatted_લીટીઓ == 0));
}

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  unsigned અક્ષર bin_buffer_unaligned[DIGEST_BIN_BYTES + DIGEST_ALIGN];
  /* Make sure bin_buffer is properly aligned. */
  unsigned અક્ષર *bin_buffer = ptr_align (bin_buffer_unaligned, DIGEST_ALIGN);
  bool do_check = false;
  પૂર્ણાંક opt;
  bool ok = true;
  પૂર્ણાંક દ્વિઅંકી = -1;
  પૂર્ણાંક prefix_tag = -1;
  સંરચના option અચલ *long_opts = long_options;

#જો HASH_ALGO_CKSUM
  જો (cksum_algorithm != crc)
    {
      legacy_mode = true;
      prefix_tag = 0;
      algorithm_specified = true;
      long_opts = legacy_long_options;
    }
#endif

  /* Setting values of global variables.  */
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* Line buffer stdout to ensure lines are written atomically and immediately
     so that processes running in parallel do not intersperse their output.  */
  setvbuf (stdout, ખાલી, _IOLBF, 0);

#જો HASH_ALGO_SUM
  અક્ષર અચલ *short_opts = "rs";
#અથવાજો HASH_ALGO_CKSUM
  અક્ષર અચલ *short_opts = legacy_mode ? "bctwz" : "a:l:bctwz";
  અક્ષર અચલ *digest_length_str = "";
#અથવાજો HASH_ALGO_BLAKE2
  અક્ષર અચલ *short_opts = "l:bctwz";
  અક્ષર અચલ *digest_length_str = "";
#નહીંતર
  અક્ષર અચલ *short_opts = "bctwz";
#endif

  જ્યારે ((opt = getopt_long (argc, argv, short_opts, long_opts, ખાલી))
         != -1)
    પસંદગી (opt)
      {
#જો HASH_ALGO_CKSUM
      સ્થિતિ 'a':
        cksum_algorithm = XARGMATCH_EXACT ("--algorithm", optarg,
                                           algorithm_args, algorithm_types);
        algorithm_specified = true;
        તોડો;

      સ્થિતિ DEBUG_PROGRAM_OPTION:
        cksum_debug = true;
        તોડો;
#endif
#જો HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
      સ્થિતિ 'l':
        digest_length = xnumtoimax (optarg, 10, 0, IDX_MAX, "",
                                    _("invalid length"), 0,
                                    XTOINT_MAX_QUIET);
        digest_length_str = optarg;
        તોડો;
#endif
#જો !HASH_ALGO_SUM
      સ્થિતિ 'c':
        do_check = true;
        તોડો;
      સ્થિતિ STATUS_OPTION:
        status_only = true;
        warn = false;
        quiet = false;
        તોડો;
      સ્થિતિ 'b':
        દ્વિઅંકી = 1;
        તોડો;
      સ્થિતિ 't':
        દ્વિઅંકી = 0;
        તોડો;
      સ્થિતિ 'w':
        status_only = false;
        warn = true;
        quiet = false;
        તોડો;
      સ્થિતિ IGNORE_MISSING_OPTION:
        ignore_missing = true;
        તોડો;
      સ્થિતિ QUIET_OPTION:
        status_only = false;
        warn = false;
        quiet = true;
        તોડો;
      સ્થિતિ STRICT_OPTION:
        strict = true;
        તોડો;
# જો HASH_ALGO_CKSUM
      સ્થિતિ BASE64_OPTION:
        base64_digest = true;
        તોડો;
      સ્થિતિ RAW_OPTION:
        raw_digest = true;
        તોડો;
      સ્થિતિ UNTAG_OPTION:
        prefix_tag = 0;
        તોડો;
# endif
      સ્થિતિ TAG_OPTION:
        prefix_tag = 1;
        તોડો;
      સ્થિતિ 'z':
        digest_delim = '\0';
        તોડો;
#endif
#જો HASH_ALGO_SUM
      સ્થિતિ 'r':		/* For SysV compatibility. */
        sum_algorithm = bsd;
        તોડો;

      સ્થિતિ 's':
        sum_algorithm = sysv;
        તોડો;
#endif
      case_GETOPT_HELP_CHAR;
      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
      મૂળભૂત:
        usage (EXIT_FAILURE);
      }

  min_digest_લીટી_length = MIN_DIGEST_લીટી_LENGTH;
#જો HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# જો HASH_ALGO_CKSUM
  જો (digest_length && (cksum_algorithm != blake2b
                        && cksum_algorithm != sha2
                        && cksum_algorithm != sha3))
    error (EXIT_FAILURE, 0,
           _("--length is only supported with --algorithm "
             "blake2b, sha2, or sha3"));
  જો (cksum_algorithm == sha2 || cksum_algorithm == sha3)
    {
      /* Do not require --length with --check.  */
      જો (digest_length == 0 && *digest_length_str == '\0' && ! do_check)
        error (EXIT_FAILURE, 0, _("--algorithm=%s requires specifying "
                                  "--length 224, 256, 384, or 512"),
               algorithm_args[cksum_algorithm]);
      /* If --check and --length are used we verify the digest length.  */
      જો ((! do_check || *digest_length_str != '\0')
          && digest_length != SHA224_DIGEST_SIZE * 8
          && digest_length != SHA256_DIGEST_SIZE * 8
          && digest_length != SHA384_DIGEST_SIZE * 8
          && digest_length != SHA512_DIGEST_SIZE * 8)
        {
          error (0, 0, _("invalid length: %s"), quote (digest_length_str));
          error (EXIT_FAILURE, 0, _("digest length for %s must be "
                                    "224, 256, 384, or 512"),
                 quote (DIGEST_TYPE_STRING));
        }
    }
  નહીંતર
    {
      /* If the digest length checks for SHA-3 are satisfied, the less strict
         checks for BLAKE2 will also be.  */
# નહીંતર
    {
# endif
      જો (digest_length > DIGEST_MAX_LEN * 8)
        {
          error (0, 0, _("invalid length: %s"), quote (digest_length_str));
          error (EXIT_FAILURE, 0,
                 _("maximum digest length for %s is %d bits"),
                 quote (DIGEST_TYPE_STRING),
                 DIGEST_MAX_LEN * 8);
        }
      જો (digest_length % 8 != 0)
        {
          error (0, 0, _("invalid length: %s"), quote (digest_length_str));
          error (EXIT_FAILURE, 0, _("length is not a multiple of 8"));
        }
    }
  જો (digest_length == 0)
    {
# જો HASH_ALGO_BLAKE2
      digest_length = DIGEST_MAX_LEN * 8;
# નહીંતર
      digest_length = algorithm_bits[cksum_algorithm];
# endif
    }
  digest_hex_bytes = digest_length >> 2;
#નહીંતર
  digest_hex_bytes = DIGEST_HEX_BYTES;
#endif

#જો HASH_ALGO_CKSUM
  પસંદગી (+cksum_algorithm)
    {
    સ્થિતિ bsd:
    સ્થિતિ sysv:
    સ્થિતિ crc:
    સ્થિતિ crc32b:
        જો (do_check && algorithm_specified)
          error (EXIT_FAILURE, 0,
                 _("--check is not supported with "
                   "--algorithm={bsd,sysv,crc,crc32b}"));
        તોડો;
    }

  જો (base64_digest && raw_digest)
   {
     error (0, 0, _("--base64 and --raw are mutually exclusive"));
     usage (EXIT_FAILURE);
   }
#endif

  જો (digest_delim != '\n' && do_check)
    {
      error (0, 0, _("the --zero option is not supported when "
                     "verifying checksums"));
      usage (EXIT_FAILURE);
    }
  જો (1 <= prefix_tag && do_check)
    {
      /* Note we allow --untagged with --check to more
         seamlessly support --untagged in an emulation wrapper.  */
      error (0, 0, _("the --tag option is meaningless when "
                     "verifying checksums"));
      usage (EXIT_FAILURE);
    }

  જો (0 <= દ્વિઅંકી && do_check)
    {
      error (0, 0, _("the --binary and --text options are meaningless when "
                     "verifying checksums"));
      usage (EXIT_FAILURE);
    }

  જો (ignore_missing && !do_check)
    {
      error (0, 0,
             _("the --ignore-missing option is meaningful only when "
               "verifying checksums"));
      usage (EXIT_FAILURE);
    }

  જો (status_only && !do_check)
    {
      error (0, 0,
       _("the --status option is meaningful only when verifying checksums"));
      usage (EXIT_FAILURE);
    }

  જો (warn && !do_check)
    {
      error (0, 0,
       _("the --warn option is meaningful only when verifying checksums"));
      usage (EXIT_FAILURE);
    }

  જો (quiet && !do_check)
    {
      error (0, 0,
       _("the --quiet option is meaningful only when verifying checksums"));
      usage (EXIT_FAILURE);
    }

  જો (strict & !do_check)
   {
     error (0, 0,
        _("the --strict option is meaningful only when verifying checksums"));
     usage (EXIT_FAILURE);
   }

  જો (prefix_tag == -1)
    prefix_tag = HASH_ALGO_CKSUM;

  જો (prefix_tag && !દ્વિઅંકી)
   {
     /* This could be supported in a backwards compatible way
        by prefixing the output line with a space in text mode.
        However that's invasive enough that it was agreed to
        not support this mode with --tag, as --text use cases
        are adequately supported by the default output format.  */
#જો !HASH_ALGO_CKSUM
     error (0, 0, _("--tag does not support --text mode"));
#નહીંતર
     error (0, 0, _("--text mode is only supported with --untagged"));
#endif
     usage (EXIT_FAILURE);
   }

  જો (!O_દ્વિઅંકી && દ્વિઅંકી < 0)
    દ્વિઅંકી = 0;
  નહીંતર જો (prefix_tag)
    દ્વિઅંકી = 1;

  અક્ષર **operand_lim = argv + argc;
  જો (optind == argc)
    *operand_lim++ = bad_cast ("-");
  નહીંતર જો (1 < argc - optind && raw_digest)
    error (EXIT_FAILURE, 0,
           _("the --raw option is not supported with multiple files"));

  માટે (અક્ષર **operandp = argv + optind; operandp < operand_lim; operandp++)
    {
      અક્ષર *file = *operandp;
      જો (do_check)
        ok &= digest_check (file);
      નહીંતર
        {
          પૂર્ણાંક દ્વિઅંકી_file = દ્વિઅંકી;
          bool missing;
          intmax_t length;

          જો (! digest_file (file, &દ્વિઅંકી_file, bin_buffer, &missing, &length))
            ok = false;
          નહીંતર
            {
              DIGEST_OUT (file, દ્વિઅંકી_file, bin_buffer, raw_digest, prefix_tag,
                          digest_delim, optind != argc, length);
              જો (ferror (stdout))
                write_error ();
            }
        }
    }

  જો (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, _("standard input"));

  પાછા ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
