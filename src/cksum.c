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

#அந்தர்பாவ <config.h>

#அந்தர்பாவ <getopt.h>
#அந்தர்பாவ <sys/types.h>

#அந்தர்பாவ "assure.h"
#அந்தர்பாவ "system.h"
#அந்தர்பாவ "argmatch.h"
#அந்தர்பாவ "c-ctype.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "xdectoint.h"
#அந்தர்பாவ "xstrtol.h"

#ifndef HASH_ALGO_CKSUM
# define HASH_ALGO_CKSUM 0
#endif

#எனில் HASH_ALGO_SUM || HASH_ALGO_CKSUM
# include "sum.h"
#endif
#எனில் HASH_ALGO_CKSUM
# include "cksum.h"
# include "cksum_crc.h"
# include "base64.h"
#endif
#எனில் HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# include "blake2/b2sum.h"
#endif
#எனில் HASH_ALGO_MD5 || HASH_ALGO_CKSUM
# include "md5.h"
#endif
#எனில் HASH_ALGO_SHA1 || HASH_ALGO_CKSUM
# include "sha1.h"
#endif
#எனில் HASH_ALGO_SHA256 || HASH_ALGO_SHA224 || HASH_ALGO_CKSUM
# include "sha256.h"
#endif
#எனில் HASH_ALGO_SHA512 || HASH_ALGO_SHA384 || HASH_ALGO_CKSUM
# include "sha512.h"
#endif
#எனில் HASH_ALGO_CKSUM
# include "sha3.h"
#endif
#எனில் HASH_ALGO_CKSUM
# include "sm3.h"
#endif
#அந்தர்பாவ "fadvise.h"
#அந்தர்பாவ "stdio--.h"
#அந்தர்பாவ "xbinary-io.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#எனில் HASH_ALGO_SUM
# define PROGRAM_NAME "sum"
# define DIGEST_TYPE_STRING "BSD"
# define DIGEST_STREAM sumfns[sum_algorithm]
# define DIGEST_OUT sum_output_fns[sum_algorithm]
# define DIGEST_BITS 16
# define DIGEST_ALIGN 4
#அல்லதெனில் HASH_ALGO_CKSUM
# define MAX_DIGEST_BITS 512
# define MAX_DIGEST_ALIGN 8
# define DIGEST_TYPE_STRING algorithm_tags[cksum_algorithm]
# define DIGEST_STREAM cksumfns[cksum_algorithm]
# define DIGEST_OUT cksum_output_fns[cksum_algorithm]
# define DIGEST_BITS MAX_DIGEST_BITS
# define DIGEST_ALIGN MAX_DIGEST_ALIGN
#அல்லதெனில் HASH_ALGO_MD5
# define PROGRAM_NAME "md5sum"
# define DIGEST_TYPE_STRING "MD5"
# define DIGEST_STREAM md5_stream
# define DIGEST_BITS 128
# define DIGEST_REFERENCE "RFC 1321"
# define DIGEST_ALIGN 4
#அல்லதெனில் HASH_ALGO_BLAKE2
# define PROGRAM_NAME "b2sum"
# define DIGEST_TYPE_STRING "BLAKE2b"
# define DIGEST_STREAM blake2b_stream
# define DIGEST_BITS 512
# define DIGEST_REFERENCE "RFC 7693"
# define DIGEST_ALIGN 8
#அல்லதெனில் HASH_ALGO_SHA1
# define PROGRAM_NAME "sha1sum"
# define DIGEST_TYPE_STRING "SHA1"
# define DIGEST_STREAM sha1_stream
# define DIGEST_BITS 160
# define DIGEST_REFERENCE "FIPS-180-1"
# define DIGEST_ALIGN 4
#அல்லதெனில் HASH_ALGO_SHA256
# define PROGRAM_NAME "sha256sum"
# define DIGEST_TYPE_STRING "SHA256"
# define DIGEST_STREAM sha256_stream
# define DIGEST_BITS 256
# define DIGEST_REFERENCE "FIPS-180-2"
# define DIGEST_ALIGN 4
#அல்லதெனில் HASH_ALGO_SHA224
# define PROGRAM_NAME "sha224sum"
# define DIGEST_TYPE_STRING "SHA224"
# define DIGEST_STREAM sha224_stream
# define DIGEST_BITS 224
# define DIGEST_REFERENCE "RFC 3874"
# define DIGEST_ALIGN 4
#அல்லதெனில் HASH_ALGO_SHA512
# define PROGRAM_NAME "sha512sum"
# define DIGEST_TYPE_STRING "SHA512"
# define DIGEST_STREAM sha512_stream
# define DIGEST_BITS 512
# define DIGEST_REFERENCE "FIPS-180-2"
# define DIGEST_ALIGN 8
#அல்லதெனில் HASH_ALGO_SHA384
# define PROGRAM_NAME "sha384sum"
# define DIGEST_TYPE_STRING "SHA384"
# define DIGEST_STREAM sha384_stream
# define DIGEST_BITS 384
# define DIGEST_REFERENCE "FIPS-180-2"
# define DIGEST_ALIGN 8
#இல்லையெனில்
# error "Can't decide which hash algorithm to compile."
#endif
#எனில் !HASH_ALGO_SUM && !HASH_ALGO_CKSUM
# define DIGEST_OUT output_file
#endif

#எனில் HASH_ALGO_CKSUM
# define PROGRAM_NAME (!legacy_mode ? "cksum"			 \
                       : cksum_algorithm == md5 ? "md5sum"	 \
                       : cksum_algorithm == sha1 ? "sha1sum"	 \
                       : cksum_algorithm == sha224 ? "sha224sum" \
                       : cksum_algorithm == sha256 ? "sha256sum" \
                       : cksum_algorithm == sha384 ? "sha384sum" \
                       : cksum_algorithm == sha512 ? "sha512sum" \
                       : "cksum")
#endif

#எனில் HASH_ALGO_SUM
# define AUTHORS \
  proper_name ("Kayvan Aghaiepour"), \
  proper_name ("David MacKenzie")
#அல்லதெனில் HASH_ALGO_CKSUM
# define AUTHORS \
  proper_name_lite ("Padraig Brady", "P\303\241draig Brady"), \
  proper_name ("Q. Frank Xia")
#அல்லதெனில் HASH_ALGO_BLAKE2
# define AUTHORS \
  proper_name_lite ("Padraig Brady", "P\303\241draig Brady"), \
  proper_name ("Samuel Neves")
#இல்லையெனில்
# define AUTHORS \
  proper_name ("Ulrich Drepper"), \
  proper_name ("Scott Miller"), \
  proper_name ("David Madore")
#endif
#எனில் !HASH_ALGO_BLAKE2 && !HASH_ALGO_CKSUM
# define DIGEST_HEX_BYTES (DIGEST_BITS / 4)
#endif
#define DIGEST_BIN_BYTES (DIGEST_BITS / 8)

/* The minimum length of a valid digest line.  This length does
   not include any newline character at the end of a line.  */
#எனில் HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# define MIN_DIGEST_வரி_LENGTH 3 /* With -l 8.  */
#இல்லையெனில்
# define MIN_DIGEST_வரி_LENGTH \
   (DIGEST_HEX_BYTES /* length of hexadecimal message digest */ \
    + 1 /* blank */ \
    + 1 /* minimum filename length */ )
#endif

#எனில் !HASH_ALGO_SUM
நிலையான வெறுமை
output_file (எழுத்து மாறிலி *file, முழு இருமம்_file, வெறுமை மாறிலி *digest,
             bool raw, bool tagged, unsigned எழுத்து delim, bool args,
             intmax_t length);
#endif

/* True if any of the files read were the standard input. */
நிலையான bool have_read_stdin;

/* The minimum length of a valid checksum line for the selected algorithm.  */
நிலையான idx_t min_digest_வரி_length;

/* Set to the length of a digest hex string for the selected algorithm.  */
நிலையான idx_t digest_hex_bytes;

/* With --check, don't generate any output.
   The exit code indicates success or failure.  */
நிலையான bool status_only = false;

/* With --check, print a message to standard error warning about each
   improperly formatted checksum line.  */
நிலையான bool warn = false;

/* With --check, ignore missing files.  */
நிலையான bool ignore_missing = false;

/* With --check, suppress the "OK" printed for each verified file.  */
நிலையான bool quiet = false;

/* With --check, exit with a non-zero return code if any line is
   improperly formatted. */
நிலையான bool strict = false;

/* Whether a BSD reversed format checksum is detected.  */
நிலையான முழு bsd_reversed = -1;

/* line delimiter.  */
நிலையான unsigned எழுத்து digest_delim = '\n';

#எனில் HASH_ALGO_CKSUM
/* If true, print base64-encoded digests, not hex.  */
நிலையான bool base64_digest = false;
#endif

/* If true, print binary digests, not hex.  */
நிலையான bool raw_digest = false;

/* blake2b and sha3 allow the -l option.  Luckily they both have the same
   maximum digest size.  */
#எனில் HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# எனில் HASH_ALGO_CKSUM
static_assert (BLAKE2B_OUTBYTES == SHA3_512_DIGEST_SIZE);
# endif
# define DIGEST_MAX_LEN BLAKE2B_OUTBYTES
நிலையான idx_t digest_length;
#endif /* HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM */

வகைவரை வெறுமை (*digest_output_fn)(எழுத்து மாறிலி *, முழு, வெறுமை மாறிலி *, bool,
                                 bool, unsigned எழுத்து, bool, intmax_t);
#எனில் HASH_ALGO_SUM
எண்ணிக்கை Algorithm
{
  bsd,
  sysv,
};

நிலையான எண்ணிக்கை Algorithm sum_algorithm;
நிலையான sumfn sumfns[]=
{
  bsd_sum_stream,
  sysv_sum_stream,
};
நிலையான digest_output_fn sum_output_fns[]=
{
  output_bsd,
  output_sysv,
};
#endif

#எனில் HASH_ALGO_CKSUM
நிலையான முழு
md5_sum_stream (FILE *stream, வெறுமை *resstream, MAYBE_UNUSED intmax_t *length)
{
  திரும்பு md5_stream (stream, resstream);
}
நிலையான முழு
sha1_sum_stream (FILE *stream, வெறுமை *resstream, MAYBE_UNUSED intmax_t *length)
{
  திரும்பு sha1_stream (stream, resstream);
}
நிலையான முழு
sha224_sum_stream (FILE *stream, வெறுமை *resstream,
                   MAYBE_UNUSED intmax_t *length)
{
  திரும்பு sha224_stream (stream, resstream);
}
நிலையான முழு
sha256_sum_stream (FILE *stream, வெறுமை *resstream,
                   MAYBE_UNUSED intmax_t *length)
{
  திரும்பு sha256_stream (stream, resstream);
}
நிலையான முழு
sha384_sum_stream (FILE *stream, வெறுமை *resstream,
                   MAYBE_UNUSED intmax_t *length)
{
  திரும்பு sha384_stream (stream, resstream);
}
நிலையான முழு
sha512_sum_stream (FILE *stream, வெறுமை *resstream,
                   MAYBE_UNUSED intmax_t *length)
{
  திரும்பு sha512_stream (stream, resstream);
}
நிலையான முழு
sha2_sum_stream (FILE *stream, வெறுமை *resstream, intmax_t *length)
{
  தேர்வு (*length)
    {
    நிலை SHA224_DIGEST_SIZE:
      திரும்பு sha224_stream (stream, resstream);
    நிலை SHA256_DIGEST_SIZE:
      திரும்பு sha256_stream (stream, resstream);
    நிலை SHA384_DIGEST_SIZE:
      திரும்பு sha384_stream (stream, resstream);
    நிலை SHA512_DIGEST_SIZE:
      திரும்பு sha512_stream (stream, resstream);
    இயல்பு:
      affirm (false);
    }
}
நிலையான முழு
sha3_sum_stream (FILE *stream, வெறுமை *resstream, intmax_t *length)
{
  தேர்வு (*length)
    {
    நிலை SHA3_224_DIGEST_SIZE:
      திரும்பு sha3_224_stream (stream, resstream);
    நிலை SHA3_256_DIGEST_SIZE:
      திரும்பு sha3_256_stream (stream, resstream);
    நிலை SHA3_384_DIGEST_SIZE:
      திரும்பு sha3_384_stream (stream, resstream);
    நிலை SHA3_512_DIGEST_SIZE:
      திரும்பு sha3_512_stream (stream, resstream);
    இயல்பு:
      affirm (false);
    }
}
நிலையான முழு
blake2b_sum_stream (FILE *stream, வெறுமை *resstream, intmax_t *length)
{
  திரும்பு blake2b_stream (stream, resstream, *length);
}
நிலையான முழு
sm3_sum_stream (FILE *stream, வெறுமை *resstream, MAYBE_UNUSED intmax_t *length)
{
  திரும்பு sm3_stream (stream, resstream);
}

நிலையான எழுத்து மாறிலி *மாறிலி algorithm_args[] =
{
  "bsd", "sysv", "crc", "crc32b", "md5", "sha1",
  "sha224", "sha256", "sha384", "sha512", /* Legacy naming */
  "sha2", "sha3", "blake2b", "sm3", வெற்று
};
நிலையான எண்ணிக்கை Algorithm மாறிலி algorithm_types[] =
{
  bsd, sysv, crc, crc32b, md5, sha1,
  sha224, sha256, sha384, sha512,
  sha2, sha3, blake2b, sm3,
};
ARGMATCH_VERIFY (algorithm_args, algorithm_types);

நிலையான எழுத்து மாறிலி *மாறிலி algorithm_tags[] =
{
  "BSD", "SYSV", "CRC", "CRC32B", "MD5", "SHA1",
  "SHA224", "SHA256", "SHA384", "SHA512",
  "SHA2", "SHA3", "BLAKE2b", "SM3", வெற்று
};
நிலையான முழு மாறிலி algorithm_bits[] =
{
  16, 16, 32, 32, 128, 160,
  224, 256, 384, 512,
  512, 512, 512, 256, 0
};

static_assert (countof (algorithm_bits) == countof (algorithm_args));

நிலையான bool algorithm_specified = false;
நிலையான bool legacy_mode = false;
எண்ணிக்கை Algorithm cksum_algorithm = crc;

நிலையான sumfn cksumfns[]=
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
நிலையான digest_output_fn cksum_output_fns[]=
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

எண்ணிக்கை
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

#எனில் HASH_ALGO_CKSUM
நிலையான கட்டமைப்பு option மாறிலி legacy_long_options[] =
{
  { "check", no_argument, வெற்று, 'c' },
  { "ignore-missing", no_argument, வெற்று, IGNORE_MISSING_OPTION },
  { "quiet", no_argument, வெற்று, QUIET_OPTION },
  { "status", no_argument, வெற்று, STATUS_OPTION },
  { "warn", no_argument, வெற்று, 'w' },
  { "strict", no_argument, வெற்று, STRICT_OPTION },
  { "tag", no_argument, வெற்று, TAG_OPTION },
  { "zero", no_argument, வெற்று, 'z' },

  { "binary", no_argument, வெற்று, 'b' }, /* Deprecated.  */
  { "text", no_argument, வெற்று, 't' }, /* Deprecated.  */

  { GETOPT_HELP_OPTION_DECL },
  { GETOPT_VERSION_OPTION_DECL },
  { வெற்று, 0, வெற்று, 0 }
};
#endif

நிலையான கட்டமைப்பு option மாறிலி long_options[] =
{
#எனில் HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
  { "length", required_argument, வெற்று, 'l'},
#endif

#எனில் !HASH_ALGO_SUM
  { "check", no_argument, வெற்று, 'c' },
  { "ignore-missing", no_argument, வெற்று, IGNORE_MISSING_OPTION},
  { "quiet", no_argument, வெற்று, QUIET_OPTION },
  { "status", no_argument, வெற்று, STATUS_OPTION },
  { "warn", no_argument, வெற்று, 'w' },
  { "strict", no_argument, வெற்று, STRICT_OPTION },
  { "tag", no_argument, வெற்று, TAG_OPTION },
  { "zero", no_argument, வெற்று, 'z' },

# எனில் HASH_ALGO_CKSUM
  { "algorithm", required_argument, வெற்று, 'a'},
  { "base64", no_argument, வெற்று, BASE64_OPTION },
  { "debug", no_argument, வெற்று, DEBUG_PROGRAM_OPTION},
  { "raw", no_argument, வெற்று, RAW_OPTION},
  { "untagged", no_argument, வெற்று, UNTAG_OPTION },
# endif
  { "binary", no_argument, வெற்று, 'b' }, /* Deprecated.  */
  { "text", no_argument, வெற்று, 't' }, /* Deprecated.  */

#இல்லையெனில்
  {"sysv", no_argument, வெற்று, 's'},  /* Not in cksum.  */
#endif

  { GETOPT_HELP_OPTION_DECL },
  { GETOPT_VERSION_OPTION_DECL },
  { வெற்று, 0, வெற்று, 0 }
};

வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"), program_name);
#எனில் HASH_ALGO_CKSUM
    எனில் (legacy_mode)
      {
        printf (_("\
Print or check %s checksums.\n\
"), DIGEST_TYPE_STRING);
        fputs (_("\
Legacy interface to the cksum(1) utility.\n\
"), stdout);
      }
    இல்லையெனில்
      fputs (_("\
Print or verify checksums.\n\
By இயல்பு use the 32 bit CRC algorithm.\n\
"), stdout);
#இல்லையெனில்
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
#எனில் HASH_ALGO_SUM
      oputs (_("\
  -r\n\
         use BSD sum algorithm (the இயல்பு), use 1K blocks\n\
"));
      oputs (_("\
  -s, --sysv\n\
         use System V sum algorithm, use 512 bytes blocks\n\
"));
#endif
#எனில் HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
        emit_mandatory_arg_note ();
#endif
#எனில் HASH_ALGO_CKSUM
     எனில் (! legacy_mode)
        oputs (_("\
  -a, --algorithm=TYPE\n\
         select the digest type to use.  See DIGEST below\n\
"));
     எனில் (! legacy_mode)
        oputs (_("\
      --base64\n\
         emit base64-encoded digests, not hexadecimal\n\
"));
#endif
#எனில் !HASH_ALGO_SUM
# எனில் HASH_ALGO_CKSUM
     எனில் (legacy_mode)
       {
# endif
      எனில் (O_இருமம்)
        oputs (_("\
  -b, --இருமம்\n\
         பட in இருமம் mode (இயல்பு unless reading tty stdin)\n\
"));
      இல்லையெனில்
        oputs (_("\
  -b, --இருமம்\n\
         பட in இருமம் mode\n\
"));
# எனில் HASH_ALGO_CKSUM
       }
# endif
        oputs (_("\
  -c, --check\n\
         பட checksums from the FILEs and check them\n\
"));
# எனில் HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
#  எனில் HASH_ALGO_CKSUM
     எனில் (! legacy_mode)
#  endif
        oputs (_("\
  -l, --length=BITS\n\
         digest length in bits; must not exceed the max size\n\
         and must be a multiple of 8 க்கு blake2b;\n\
         must be 224, 256, 384, or 512 க்கு sha2 or sha3\n\
"));
# endif
# எனில் HASH_ALGO_CKSUM
    எனில் (! legacy_mode)
        oputs (_("\
      --raw\n\
         emit a raw இருமம் digest, not hexadecimal\n\
"));
    எனில் (legacy_mode)
      oputs (_("\
      --tag\n\
         create a BSD-style checksum\n\
"));
    இல்லையெனில்
      oputs (_("\
      --tag\n\
         create a BSD-style checksum (the இயல்பு)\n\
"));
    எனில் (! legacy_mode)
      oputs (_("\
      --untagged\n\
         create a reversed style checksum, without digest type\n\
"));
# இல்லையெனில்
      oputs (_("\
      --tag\n\
         create a BSD-style checksum\n\
"));
# endif
# எனில் HASH_ALGO_CKSUM
    எனில் (legacy_mode)
      {
# endif
      எனில் (O_இருமம்)
        oputs (_("\
  -t, --text\n\
         பட in text mode (இயல்பு எனில் reading tty stdin)\n\
"));
      இல்லையெனில்
        oputs (_("\
  -t, --text\n\
         பட in text mode (இயல்பு)\n\
"));
# எனில் HASH_ALGO_CKSUM
      }
# endif
      oputs (_("\
  -z, --zero\n\
         end each output வரி with NUL, not newline,\n\
         and disable file name escaping\n\
"));
      fputs (_("\
\n\
The following five options are useful only when verifying checksums:\n\
"), stdout);
      oputs (_("\
      --ignore-missing\n\
         don't fail or report status க்கு missing files\n\
"));
      oputs (_("\
      --quiet\n\
         don't print OK க்கு each successfully verified file\n\
"));
      oputs (_("\
      --status\n\
         don't output anything, status code shows success\n\
"));
      oputs (_("\
      --strict\n\
         exit non-zero க்கு improperly formatted checksum வரிகள்\n\
"));
      oputs (_("\
  -w, --warn\n\
         warn about improperly formatted checksum வரிகள்\n\
"));
#endif
#எனில் HASH_ALGO_CKSUM
    எனில் (! legacy_mode)
      oputs (_("\
      --debug\n\
         indicate which implementation used\n\
"));
#endif
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
#எனில் HASH_ALGO_CKSUM
    எனில் (! legacy_mode)
      fputs (_("\
\n\
DIGEST determines the digest algorithm and இயல்பு output format:\n\
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
#எனில் !HASH_ALGO_SUM && !HASH_ALGO_CKSUM
      printf (_("\
\n\
The sums are computed as described in %s.\n"), DIGEST_REFERENCE);
      fputs (_("\
When checking, the உள்ளீடு should be a former output of this program.\n\
The இயல்பு mode is to print a வரி with: checksum, a space,\n\
a character indicating உள்ளீடு mode ('*' க்கு இருமம், ' ' க்கு text\n\
or where இருமம் is insignificant), and name க்கு each FILE.\n\
\n\
There is no difference between இருமம் mode and text mode on GNU systems.\
\n"), stdout);
#endif
#எனில் HASH_ALGO_CKSUM
      fputs (_("\
When checking, the உள்ளீடு should be a former output of this program,\n\
or equivalent standalone program.\
\n"), stdout);
#endif
      emit_ancillary_info (PROGRAM_NAME);
    }

  exit (status);
}

#எனில் !HASH_ALGO_SUM
/* Given a string S, return TRUE if it contains problematic characters
   that need escaping.  Note we escape '\' itself to provide some forward
   compat to introduce escaping of other characters.  */

ATTRIBUTE_PURE
நிலையான bool
problematic_chars (எழுத்து மாறிலி *s)
{
  idx_t length = strcspn (s, "\\\n\r");
  திரும்பு s[length] != '\0';
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

நிலையான எழுத்து *
filename_unescape (எழுத்து *s, idx_t s_len)
{
  எழுத்து *dst = s;

  க்கு (idx_t i = 0; i < s_len; i++)
    {
      தேர்வு (s[i])
        {
        நிலை '\\':
          எனில் (i == s_len - 1)
            {
              /* File name ends with an unescaped backslash: invalid.  */
              திரும்பு வெற்று;
            }
          ++i;
          தேர்வு (s[i])
            {
            நிலை 'n':
              *dst++ = '\n';
              நிறுத்து;
            நிலை 'r':
              *dst++ = '\r';
              நிறுத்து;
            நிலை '\\':
              *dst++ = '\\';
              நிறுத்து;
            இயல்பு:
              /* Only '\', 'n' or 'r' may follow a backslash.  */
              திரும்பு வெற்று;
            }
          நிறுத்து;

        நிலை '\0':
          /* The file name may not contain a NUL.  */
          திரும்பு வெற்று;

        இயல்பு:
          *dst++ = s[i];
          நிறுத்து;
        }
    }
  எனில் (dst < s + s_len)
    *dst = '\0';

  திரும்பு s;
}

/* Return true if S is a LEN-byte NUL-terminated string of hex or base64
   digits and has the expected length.  Otherwise, return false.  */
ATTRIBUTE_PURE
நிலையான bool
valid_digits (unsigned எழுத்து மாறிலி *s, idx_t len)
{
#எனில் HASH_ALGO_CKSUM
  எனில் (len == BASE64_LENGTH (digest_length >> 3))
    {
      idx_t i;
      க்கு (i = 0; i < len - digest_length % 3; i++)
        {
          எனில் (!isbase64 (*s))
            திரும்பு false;
          ++s;
        }
      க்கு ( ; i < len; i++)
        {
          எனில் (*s != '=')
            திரும்பு false;
          ++s;
        }
    }
  இல்லையெனில்
#endif
  எனில் (len == digest_hex_bytes)
    {
      க்கு (idx_t i = 0; i < digest_hex_bytes; i++)
        {
          எனில் (!c_isxdigit (*s))
            திரும்பு false;
          ++s;
        }
    }
  இல்லையெனில்
    திரும்பு false;

  திரும்பு *s == '\0';
}

/* Split the checksum string S (of length S_LEN) from a BSD 'md5' or
   'sha1' command into two parts: a hexadecimal digest, and the file
   name.  S is modified.  Set *D_LEN to the length of the digest string.
   Return true if successful.  */

நிலையான bool
bsd_split_3 (எழுத்து *s, idx_t s_len,
             unsigned எழுத்து **digest, idx_t *d_len,
             எழுத்து **file_name, bool escaped_filename)
{
  எனில் (s_len == 0)
    திரும்பு false;

  /* Find end of filename.  */
  idx_t i = s_len - 1;
  போது (i && s[i] != ')')
    i--;

  எனில் (s[i] != ')')
    திரும்பு false;

  *file_name = s;

  எனில் (escaped_filename && filename_unescape (s, i) == வெற்று)
    திரும்பு false;

  s[i++] = '\0';

  போது (c_isblank (s[i]))
    i++;

  எனில் (s[i] != '=')
    திரும்பு false;

  i++;

  போது (c_isblank (s[i]))
    i++;

  *digest = (unsigned எழுத்து *) &s[i];

  *d_len = s_len - i;
  திரும்பு valid_digits (*digest, *d_len);
}

#எனில் HASH_ALGO_CKSUM
/* Return the corresponding Algorithm for the string S,
   or -1 for no match.  */

நிலையான ptrdiff_t
algorithm_from_tag (எழுத்து *s)
{
  /* Limit check size to this length for perf reasons.  */
  நிலையான idx_t max_tag_len;
  எனில் (! max_tag_len)
    {
      எழுத்து மாறிலி * மாறிலி * tag = algorithm_tags;
      போது (*tag)
        {
          idx_t tag_len = strlen (*tag++);
          max_tag_len = MAX (tag_len, max_tag_len);
        }
    }

  idx_t i = 0;

  /* Find end of tag */
  போது (i <= max_tag_len && s[i] && ! c_isblank (s[i])
         && s[i] != '-' && s[i] != '(')
    ++i;

  எனில் (i > max_tag_len)
    திரும்பு -1;

  /* Terminate tag, and lookup.  */
  எழுத்து sep = s[i];
  s[i] = '\0';
  ptrdiff_t algo = argmatch_exact (s, algorithm_tags);
  s[i] = sep;

  திரும்பு algo;
}
#endif

/* Split the string S (of length S_LEN) into three parts:
   a hexadecimal digest, binary flag, and the file name.
   S is modified.  Set *D_LEN to the length of the digest string.
   Return true if successful.  */

நிலையான bool
split_3 (எழுத்து *s, idx_t s_len,
         unsigned எழுத்து **digest, idx_t *d_len, முழு *இருமம், எழுத்து **file_name)
{
  bool escaped_filename = false;
  idx_t algo_name_len;

  idx_t i = 0;
  போது (c_isblank (s[i]))
    ++i;

  எனில் (s[i] == '\\')
    {
      ++i;
      escaped_filename = true;
    }

  /* Check for BSD-style checksum line. */

#எனில் HASH_ALGO_CKSUM
  எனில் (! algorithm_specified || cksum_algorithm == sha2)
    {
      ptrdiff_t algo_tag = algorithm_from_tag (s + i);
      எனில் (! algorithm_specified)
        {
          எனில் (algo_tag >= 0)
            {
              எனில் (algo_tag <= crc32b)
                திரும்பு false;  /* We don't support checking these formats.  */
              cksum_algorithm = algo_tag;
            }
          இல்லையெனில்
            திரும்பு false;      /* We only support tagged format without -a.  */
        }
      இல்லையெனில்
        {
          எனில் (cksum_algorithm == sha2 && (algo_tag == sha2
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
  எனில் (STREQ_LEN (s + i, DIGEST_TYPE_STRING, algo_name_len))
    {
      i += algo_name_len;
#எனில் HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM

# எனில் HASH_ALGO_BLAKE2
      digest_length = DIGEST_MAX_LEN * 8;
# இல்லையெனில்
      digest_length = algorithm_bits[cksum_algorithm];
# endif
      எனில் (s[i] == '-')  /* length specified. Not base64 */
        {
          ++i;
          intmax_t length;
          எழுத்து *siend;
          எனில் (!c_isdigit (s[i])
              || xstrtoimax (s + i, &siend, 0, &length, வெற்று) != LONGINT_OK)
            திரும்பு false;
# எனில் HASH_ALGO_CKSUM
          இல்லையெனில் எனில் (cksum_algorithm == sha2 || cksum_algorithm == sha3)
            {
              எனில் (length != SHA224_DIGEST_SIZE * 8
                  && length != SHA256_DIGEST_SIZE * 8
                  && length != SHA384_DIGEST_SIZE * 8
                  && length != SHA512_DIGEST_SIZE * 8)
                திரும்பு false;
            }
# endif
          இல்லையெனில் எனில் (!(0 < length && length <= digest_length && length % 8 == 0))
            திரும்பு false;

          i = siend - s;
          digest_length = length;
        }
      digest_hex_bytes = digest_length >> 2;
#endif
      எனில் (s[i] == ' ')
        ++i;
      எனில் (s[i] == '(')  /* not base64 */
        {
          ++i;
          *இருமம் = 0;
          திரும்பு bsd_split_3 (s + i, s_len - i,
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
  எனில் (s_len - i < min_digest_வரி_length + (s[i] == '\\'))
    திரும்பு false;

  *digest = (unsigned எழுத்து *) &s[i];

#எனில் HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
  /* Auto determine length.  */
# எனில் HASH_ALGO_CKSUM
  எனில் (cksum_algorithm == blake2b
      || cksum_algorithm == sha2 || cksum_algorithm == sha3) {
# endif
  unsigned எழுத்து மாறிலி *hp = *digest;
  digest_hex_bytes = 0;
  க்கு (; c_isxdigit (*hp); ++hp, ++digest_hex_bytes)
    ;
# எனில் HASH_ALGO_CKSUM
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
  க்கு (; isubase64 (*hp); ++hp, ++digest_base64_bytes)
    ;
  க்கு (; *hp == '='; ++hp, ++trailing_equals)
    ;
  எனில் ((cksum_algorithm == sha2 || cksum_algorithm == sha3)
      && digest_hex_bytes >> 1 != SHA224_DIGEST_SIZE
      && digest_hex_bytes >> 1 != SHA256_DIGEST_SIZE
      && digest_hex_bytes >> 1 != SHA384_DIGEST_SIZE
      && digest_hex_bytes >> 1 != SHA512_DIGEST_SIZE)
    {
      எனில் (digest_base64_bytes + trailing_equals
          == BASE64_LENGTH (SHA224_DIGEST_SIZE))
        digest_hex_bytes = SHA224_DIGEST_SIZE * 2;
      இல்லையெனில் எனில் (digest_base64_bytes + trailing_equals
               == BASE64_LENGTH (SHA256_DIGEST_SIZE))
        digest_hex_bytes = SHA256_DIGEST_SIZE * 2;
      இல்லையெனில் எனில் (digest_base64_bytes + trailing_equals
               == BASE64_LENGTH (SHA384_DIGEST_SIZE))
        digest_hex_bytes = SHA384_DIGEST_SIZE * 2;
      இல்லையெனில் எனில் (digest_base64_bytes + trailing_equals
               == BASE64_LENGTH (SHA512_DIGEST_SIZE))
        digest_hex_bytes = SHA512_DIGEST_SIZE * 2;
      இல்லையெனில்
        திரும்பு false;
    }
  இல்லையெனில் எனில் (cksum_algorithm == blake2b
           && digest_hex_bytes < digest_base64_bytes)
    {
      க்கு (முழு j = 8; j <= DIGEST_MAX_LEN * 8; j += 8)
        {
          எனில் (BASE64_LENGTH (j / 8) == digest_base64_bytes + trailing_equals
              && j % 3 == trailing_equals)
            {
              digest_hex_bytes = j / 4;
              நிறுத்து;
            }
        }
    }
# endif
  எனில் (digest_hex_bytes < 2 || digest_hex_bytes % 2
      || DIGEST_MAX_LEN * 2 < digest_hex_bytes)
    திரும்பு false;
  digest_length = digest_hex_bytes * 4;
# எனில் HASH_ALGO_CKSUM
  }
# endif
#endif

  /* This field must be the hexadecimal or base64 representation
     of the message digest.  */
  போது (s[i] && !c_isblank (s[i]))
    i++;

  /* The digest must be followed by at least one whitespace character.  */
  எனில் (i == s_len)
    திரும்பு false;

  *d_len = &s[i] - (எழுத்து *) *digest;
  s[i++] = '\0';

  எனில் (! valid_digits (*digest, *d_len))
    திரும்பு false;

  /* If "bsd reversed" format detected.  */
  எனில் ((s_len - i == 1) || (s[i] != ' ' && s[i] != '*'))
    {
      /* Don't allow mixing bsd and standard formats,
         to minimize security issues with attackers
         renaming files with leading spaces.
         This assumes that with bsd format checksums
         that the first file name does not have
         a leading ' ' or '*'.  */
      எனில் (bsd_reversed == 0)
        திரும்பு false;
      bsd_reversed = 1;
    }
  இல்லையெனில் எனில் (bsd_reversed != 1)
    {
      bsd_reversed = 0;
      *இருமம் = (s[i++] == '*');
    }

  /* All characters between the type indicator and end of line are
     significant -- that includes leading and trailing white space.  */
  *file_name = &s[i];

  எனில் (escaped_filename)
    திரும்பு filename_unescape (&s[i], s_len - i) != வெற்று;

  திரும்பு true;
}

#எனில் !HASH_ALGO_SUM
/* If ESCAPE is true, then translate each:
   NEWLINE byte to the string, "\\n",
   CARRIAGE RETURN byte to the string, "\\r",
   and each backslash to "\\\\".  */
நிலையான வெறுமை
print_filename (எழுத்து மாறிலி *file, bool escape)
{
  எனில் (! escape)
    {
      fputs (file, stdout);
      திரும்பு;
    }

  போது (*file)
    {
      தேர்வு (*file)
        {
        நிலை '\n':
          fputs ("\\n", stdout);
          நிறுத்து;

        நிலை '\r':
          fputs ("\\r", stdout);
          நிறுத்து;

        நிலை '\\':
          fputs ("\\\\", stdout);
          நிறுத்து;

        இயல்பு:
          putchar (*file);
          நிறுத்து;
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

நிலையான bool
digest_file (எழுத்து மாறிலி *filename, முழு *இருமம், unsigned எழுத்து *bin_result,
             bool *missing, MAYBE_UNUSED intmax_t *length)
{
  FILE *fp;
  முழு err;
  bool is_stdin = streq (filename, "-");

  *missing = false;

  எனில் (is_stdin)
    {
      have_read_stdin = true;
      fp = stdin;
      எனில் (O_இருமம் && *இருமம்)
        {
          எனில் (*இருமம் < 0)
            *இருமம் = ! isatty (STDIN_FILENO);
          எனில் (*இருமம்)
            xset_இருமம்_mode (STDIN_FILENO, O_இருமம்);
        }
    }
  இல்லையெனில்
    {
      fp = fopen (filename, O_இருமம் ? (*இருமம் ? "rb" : "rt") : "r");
      எனில் (fp == வெற்று)
        {
          எனில் (ignore_missing && errno == ENOENT)
            {
              *missing = true;
              திரும்பு true;
            }
          error (0, errno, "%s", quotef (filename));
          திரும்பு false;
        }
    }

  fadvise (fp, FADVISE_SEQUENTIAL);

#எனில் HASH_ALGO_CKSUM
  எனில் (cksum_algorithm == blake2b
      || cksum_algorithm == sha2 || cksum_algorithm == sha3)
    *length = digest_length >> 3;
  err = DIGEST_STREAM (fp, bin_result, length);
#அல்லதெனில் HASH_ALGO_SUM
  err = DIGEST_STREAM (fp, bin_result, length);
#அல்லதெனில் HASH_ALGO_BLAKE2
  err = DIGEST_STREAM (fp, bin_result, digest_length >> 3);
#இல்லையெனில்
  err = DIGEST_STREAM (fp, bin_result);
#endif
  err = err ? errno : 0;
  எனில் (is_stdin)
    clearerr (fp);
  இல்லையெனில் எனில் (fclose (fp) != 0 && !err)
    err = errno;

  எனில் (err)
    {
      error (0, err, "%s", quotef (filename));
      திரும்பு false;
    }

  திரும்பு true;
}

#எனில் !HASH_ALGO_SUM
நிலையான வெறுமை
output_file (எழுத்து மாறிலி *file, முழு இருமம்_file, வெறுமை மாறிலி *digest,
             MAYBE_UNUSED bool raw, bool tagged, unsigned எழுத்து delim,
             MAYBE_UNUSED bool args, MAYBE_UNUSED intmax_t length)
{
# எனில் HASH_ALGO_CKSUM
  எனில் (raw)
    {
      fwrite (digest, 1, digest_length >> 3, stdout);
      திரும்பு;
    }
# endif

  unsigned எழுத்து மாறிலி *bin_buffer = digest;

  /* Output a leading backslash if the file name contains problematic chars.  */
  bool needs_escape = delim == '\n' && problematic_chars (file);

  எனில் (needs_escape)
    putchar ('\\');

  எனில் (tagged)
    {
# எனில் HASH_ALGO_CKSUM
      எனில் (cksum_algorithm == sha2)
        printf ("SHA%u", (unsigned முழு) digest_length);
      இல்லையெனில்
# endif
      fputs (DIGEST_TYPE_STRING, stdout);
# எனில் HASH_ALGO_BLAKE2
      எனில் (digest_length < DIGEST_MAX_LEN * 8)
        printf ("-%u", (unsigned முழு) digest_length);
# elif HASH_ALGO_CKSUM
      எனில் (cksum_algorithm == sha3)
        printf ("-%u", (unsigned முழு) digest_length);
      எனில் (cksum_algorithm == blake2b)
        {
          எனில் (digest_length < DIGEST_MAX_LEN * 8)
            printf ("-%u", (unsigned முழு) digest_length);
        }
# endif
      fputs (" (", stdout);
      print_filename (file, needs_escape);
      fputs (") = ", stdout);
    }

# எனில் HASH_ALGO_CKSUM
  எனில் (base64_digest)
    {
      எழுத்து b64[BASE64_LENGTH (DIGEST_BIN_BYTES) + 1];
      base64_encode ((எழுத்து மாறிலி *) bin_buffer, digest_length >> 3,
                     b64, அளவு b64);
      fputs (b64, stdout);
    }
  இல்லையெனில்
# endif
    {
      க்கு (idx_t i = 0; i < (digest_hex_bytes >> 1); ++i)
        printf ("%02x", bin_buffer[i]);
    }

  எனில் (!tagged)
    {
      putchar (' ');
      putchar (இருமம்_file ? '*' : ' ');
      print_filename (file, needs_escape);
    }

  putchar (delim);
}
#endif

#எனில் HASH_ALGO_CKSUM
/* Return true if B64_DIGEST is the same as the base64 digest of the
   DIGEST_LENGTH/8 bytes at BIN_BUFFER.  */
நிலையான bool
b64_equal (unsigned எழுத்து மாறிலி *b64_digest, unsigned எழுத்து மாறிலி *bin_buffer)
{
  idx_t b64_n_bytes = BASE64_LENGTH (digest_length >> 3);
  எழுத்து b64[BASE64_LENGTH (DIGEST_BIN_BYTES) + 1];
  base64_encode ((எழுத்து மாறிலி *) bin_buffer, digest_length >> 3,
                 b64, அளவு b64);
  திரும்பு memeq (b64_digest, b64, b64_n_bytes + 1);
}
#endif

/* Return true if HEX_DIGEST is the same as the hex-encoded digest of the
   DIGEST_LENGTH/8 bytes at BIN_BUFFER.  */
நிலையான bool
hex_equal (unsigned எழுத்து மாறிலி *hex_digest, unsigned எழுத்து மாறிலி *bin_buffer)
{
  நிலையான மாறிலி எழுத்து bin2hex[] = { '0', '1', '2', '3',
                                  '4', '5', '6', '7',
                                  '8', '9', 'a', 'b',
                                  'c', 'd', 'e', 'f' };
  idx_t digest_bin_bytes = digest_hex_bytes >> 1;

  /* Compare generated binary number with text representation
     in check file.  Ignore case of hex digits.  */
  idx_t cnt;
  க்கு (cnt = 0; cnt < digest_bin_bytes; ++cnt)
    {
      எனில் (c_tolower (hex_digest[2 * cnt])
          != bin2hex[bin_buffer[cnt] >> 4]
          || (c_tolower (hex_digest[2 * cnt + 1])
              != (bin2hex[bin_buffer[cnt] & 0xf])))
        நிறுத்து;
    }
  திரும்பு cnt == digest_bin_bytes;
}

நிலையான bool
digest_check (எழுத்து மாறிலி *checkfile_name)
{
  FILE *checkfile_stream;
  intmax_t n_misformatted_வரிகள் = 0;
  intmax_t n_mismatched_checksums = 0;
  intmax_t n_open_or_read_failures = 0;
  bool properly_formatted_வரிகள் = false;
  bool matched_checksums = false;
  unsigned எழுத்து bin_buffer_unaligned[DIGEST_BIN_BYTES + DIGEST_ALIGN];
  /* Make sure bin_buffer is properly aligned. */
  unsigned எழுத்து *bin_buffer = ptr_align (bin_buffer_unaligned, DIGEST_ALIGN);
  intmax_t வரி_எண்;
  எழுத்து *வரி;
  size_t வரி_chars_allocated;
  bool is_stdin = streq (checkfile_name, "-");

  எனில் (is_stdin)
    {
      have_read_stdin = true;
      checkfile_name = _("standard input");
      checkfile_stream = stdin;
    }
  இல்லையெனில்
    {
      checkfile_stream = fopen (checkfile_name, "r");
      எனில் (checkfile_stream == வெற்று)
        {
          error (0, errno, "%s", quotef (checkfile_name));
          திரும்பு false;
        }
    }

  வரி_எண் = 0;
  வரி = வெற்று;
  வரி_chars_allocated = 0;
  செய்
    {
      எழுத்து *filename;
      முழு இருமம்;
      unsigned எழுத்து *digest;
      ssize_t வரி_length;

      ++வரி_எண்;
      எனில் (வரி_எண் == 0)
        error (EXIT_FAILURE, 0, _("%s: too many checksum lines"),
               quotef (checkfile_name));

      வரி_length = getline (&வரி, &வரி_chars_allocated, checkfile_stream);
      எனில் (வரி_length <= 0)
        நிறுத்து;

      /* Ignore comment lines, which begin with a '#' character.  */
      எனில் (வரி[0] == '#')
        தொடர்;

      /* Remove any trailing newline.  */
      வரி_length -= வரி[வரி_length - 1] == '\n';
      /* Remove any trailing carriage return.  */
      வரி_length -= வரி[வரி_length - (0 < வரி_length)] == '\r';

      /* Ignore empty lines.  */
      எனில் (வரி_length == 0)
        தொடர்;

      வரி[வரி_length] = '\0';

      idx_t d_len;
      எனில் (! (split_3 (வரி, வரி_length, &digest, &d_len, &இருமம், &filename)
             && ! (is_stdin && streq (filename, "-"))))
        {
          ++n_misformatted_வரிகள்;

          எனில் (warn)
            {
              error (0, 0,
                     _("%s: %jd"
                       ": improperly formatted %s checksum line"),
                     quotef (checkfile_name), வரி_எண்,
                     DIGEST_TYPE_STRING);
            }
        }
      இல்லையெனில்
        {
          bool ok;
          bool missing;

          properly_formatted_வரிகள் = true;

          intmax_t length;
          ok = digest_file (filename, &இருமம், bin_buffer, &missing, &length);

          எனில் (!ok)
            {
              ++n_open_or_read_failures;
              எனில் (!status_only)
                printf ("%s: %s\n", quotef (filename),_("FAILED open or read"));
            }
          இல்லையெனில் எனில் (ignore_missing && missing)
            {
              /* Ignore missing files with --ignore-missing.  */
              ;
            }
          இல்லையெனில்
            {
              bool match = false;
#எனில் HASH_ALGO_CKSUM
              எனில் (d_len == BASE64_LENGTH (digest_length >> 3))
                match = b64_equal (digest, bin_buffer);
              இல்லையெனில்
#endif
                எனில் (d_len == digest_hex_bytes)
                  match = hex_equal (digest, bin_buffer);

              எனில் (match)
                matched_checksums = true;
              இல்லையெனில்
                ++n_mismatched_checksums;

              எனில் (!status_only)
                {
                  எனில் (! match || ! quiet)
                    fputs (quotef (filename), stdout);

                  எனில் (! match)
                    printf (": %s\n", _("FAILED"));
                  இல்லையெனில் எனில் (!quiet)
                    printf (": %s\n", _("OK"));
                }
            }

          எனில் (ferror (stdout))
            write_error ();
        }
    }
  போது (!feof (checkfile_stream) && !ferror (checkfile_stream));

  விடுவி (வரி);

  முழு err = ferror (checkfile_stream) ? 0 : -1;
  எனில் (is_stdin)
    clearerr (checkfile_stream);
  இல்லையெனில் எனில் (fclose (checkfile_stream) != 0 && err < 0)
    err = errno;

  எனில் (0 <= err)
    {
      error (0, err, err ? "%s" : _("%s: read error"),
             quotef (checkfile_name));
      திரும்பு false;
    }

  எனில் (! properly_formatted_வரிகள்)
    {
      /* Warn if no tests are found.  */
      error (0, 0, _("%s: no properly formatted checksum lines found"),
             quotef (checkfile_name));
    }
  இல்லையெனில்
    {
      எனில் (!status_only)
        {
          எனில் (n_misformatted_வரிகள் != 0)
            error (0, 0,
                   (ngettext
                    ("WARNING: %jd line is improperly formatted",
                     "WARNING: %jd lines are improperly formatted",
                     select_plural (n_misformatted_வரிகள்))),
                   n_misformatted_வரிகள்);

          எனில் (n_open_or_read_failures != 0)
            error (0, 0,
                   (ngettext
                    ("WARNING: %jd listed file could not be read",
                     "WARNING: %jd listed files could not be read",
                     select_plural (n_open_or_read_failures))),
                   n_open_or_read_failures);

          எனில் (n_mismatched_checksums != 0)
            error (0, 0,
                   (ngettext
                    ("WARNING: %jd computed checksum did NOT match",
                     "WARNING: %jd computed checksums did NOT match",
                     select_plural (n_mismatched_checksums))),
                   n_mismatched_checksums);

          எனில் (ignore_missing && ! matched_checksums)
            error (0, 0, _("%s: no file was verified"),
                   quotef (checkfile_name));
        }
    }

  திரும்பு (properly_formatted_வரிகள்
          && matched_checksums
          && n_mismatched_checksums == 0
          && n_open_or_read_failures == 0
          && (!strict || n_misformatted_வரிகள் == 0));
}

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  unsigned எழுத்து bin_buffer_unaligned[DIGEST_BIN_BYTES + DIGEST_ALIGN];
  /* Make sure bin_buffer is properly aligned. */
  unsigned எழுத்து *bin_buffer = ptr_align (bin_buffer_unaligned, DIGEST_ALIGN);
  bool do_check = false;
  முழு opt;
  bool ok = true;
  முழு இருமம் = -1;
  முழு prefix_tag = -1;
  கட்டமைப்பு option மாறிலி *long_opts = long_options;

#எனில் HASH_ALGO_CKSUM
  எனில் (cksum_algorithm != crc)
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
  setvbuf (stdout, வெற்று, _IOLBF, 0);

#எனில் HASH_ALGO_SUM
  எழுத்து மாறிலி *short_opts = "rs";
#அல்லதெனில் HASH_ALGO_CKSUM
  எழுத்து மாறிலி *short_opts = legacy_mode ? "bctwz" : "a:l:bctwz";
  எழுத்து மாறிலி *digest_length_str = "";
#அல்லதெனில் HASH_ALGO_BLAKE2
  எழுத்து மாறிலி *short_opts = "l:bctwz";
  எழுத்து மாறிலி *digest_length_str = "";
#இல்லையெனில்
  எழுத்து மாறிலி *short_opts = "bctwz";
#endif

  போது ((opt = getopt_long (argc, argv, short_opts, long_opts, வெற்று))
         != -1)
    தேர்வு (opt)
      {
#எனில் HASH_ALGO_CKSUM
      நிலை 'a':
        cksum_algorithm = XARGMATCH_EXACT ("--algorithm", optarg,
                                           algorithm_args, algorithm_types);
        algorithm_specified = true;
        நிறுத்து;

      நிலை DEBUG_PROGRAM_OPTION:
        cksum_debug = true;
        நிறுத்து;
#endif
#எனில் HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
      நிலை 'l':
        digest_length = xnumtoimax (optarg, 10, 0, IDX_MAX, "",
                                    _("invalid length"), 0,
                                    XTOINT_MAX_QUIET);
        digest_length_str = optarg;
        நிறுத்து;
#endif
#எனில் !HASH_ALGO_SUM
      நிலை 'c':
        do_check = true;
        நிறுத்து;
      நிலை STATUS_OPTION:
        status_only = true;
        warn = false;
        quiet = false;
        நிறுத்து;
      நிலை 'b':
        இருமம் = 1;
        நிறுத்து;
      நிலை 't':
        இருமம் = 0;
        நிறுத்து;
      நிலை 'w':
        status_only = false;
        warn = true;
        quiet = false;
        நிறுத்து;
      நிலை IGNORE_MISSING_OPTION:
        ignore_missing = true;
        நிறுத்து;
      நிலை QUIET_OPTION:
        status_only = false;
        warn = false;
        quiet = true;
        நிறுத்து;
      நிலை STRICT_OPTION:
        strict = true;
        நிறுத்து;
# எனில் HASH_ALGO_CKSUM
      நிலை BASE64_OPTION:
        base64_digest = true;
        நிறுத்து;
      நிலை RAW_OPTION:
        raw_digest = true;
        நிறுத்து;
      நிலை UNTAG_OPTION:
        prefix_tag = 0;
        நிறுத்து;
# endif
      நிலை TAG_OPTION:
        prefix_tag = 1;
        நிறுத்து;
      நிலை 'z':
        digest_delim = '\0';
        நிறுத்து;
#endif
#எனில் HASH_ALGO_SUM
      நிலை 'r':		/* For SysV compatibility. */
        sum_algorithm = bsd;
        நிறுத்து;

      நிலை 's':
        sum_algorithm = sysv;
        நிறுத்து;
#endif
      case_GETOPT_HELP_CHAR;
      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
      இயல்பு:
        usage (EXIT_FAILURE);
      }

  min_digest_வரி_length = MIN_DIGEST_வரி_LENGTH;
#எனில் HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# எனில் HASH_ALGO_CKSUM
  எனில் (digest_length && (cksum_algorithm != blake2b
                        && cksum_algorithm != sha2
                        && cksum_algorithm != sha3))
    error (EXIT_FAILURE, 0,
           _("--length is only supported with --algorithm "
             "blake2b, sha2, or sha3"));
  எனில் (cksum_algorithm == sha2 || cksum_algorithm == sha3)
    {
      /* Do not require --length with --check.  */
      எனில் (digest_length == 0 && *digest_length_str == '\0' && ! do_check)
        error (EXIT_FAILURE, 0, _("--algorithm=%s requires specifying "
                                  "--length 224, 256, 384, or 512"),
               algorithm_args[cksum_algorithm]);
      /* If --check and --length are used we verify the digest length.  */
      எனில் ((! do_check || *digest_length_str != '\0')
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
  இல்லையெனில்
    {
      /* If the digest length checks for SHA-3 are satisfied, the less strict
         checks for BLAKE2 will also be.  */
# இல்லையெனில்
    {
# endif
      எனில் (digest_length > DIGEST_MAX_LEN * 8)
        {
          error (0, 0, _("invalid length: %s"), quote (digest_length_str));
          error (EXIT_FAILURE, 0,
                 _("maximum digest length for %s is %d bits"),
                 quote (DIGEST_TYPE_STRING),
                 DIGEST_MAX_LEN * 8);
        }
      எனில் (digest_length % 8 != 0)
        {
          error (0, 0, _("invalid length: %s"), quote (digest_length_str));
          error (EXIT_FAILURE, 0, _("length is not a multiple of 8"));
        }
    }
  எனில் (digest_length == 0)
    {
# எனில் HASH_ALGO_BLAKE2
      digest_length = DIGEST_MAX_LEN * 8;
# இல்லையெனில்
      digest_length = algorithm_bits[cksum_algorithm];
# endif
    }
  digest_hex_bytes = digest_length >> 2;
#இல்லையெனில்
  digest_hex_bytes = DIGEST_HEX_BYTES;
#endif

#எனில் HASH_ALGO_CKSUM
  தேர்வு (+cksum_algorithm)
    {
    நிலை bsd:
    நிலை sysv:
    நிலை crc:
    நிலை crc32b:
        எனில் (do_check && algorithm_specified)
          error (EXIT_FAILURE, 0,
                 _("--check is not supported with "
                   "--algorithm={bsd,sysv,crc,crc32b}"));
        நிறுத்து;
    }

  எனில் (base64_digest && raw_digest)
   {
     error (0, 0, _("--base64 and --raw are mutually exclusive"));
     usage (EXIT_FAILURE);
   }
#endif

  எனில் (digest_delim != '\n' && do_check)
    {
      error (0, 0, _("the --zero option is not supported when "
                     "verifying checksums"));
      usage (EXIT_FAILURE);
    }
  எனில் (1 <= prefix_tag && do_check)
    {
      /* Note we allow --untagged with --check to more
         seamlessly support --untagged in an emulation wrapper.  */
      error (0, 0, _("the --tag option is meaningless when "
                     "verifying checksums"));
      usage (EXIT_FAILURE);
    }

  எனில் (0 <= இருமம் && do_check)
    {
      error (0, 0, _("the --binary and --text options are meaningless when "
                     "verifying checksums"));
      usage (EXIT_FAILURE);
    }

  எனில் (ignore_missing && !do_check)
    {
      error (0, 0,
             _("the --ignore-missing option is meaningful only when "
               "verifying checksums"));
      usage (EXIT_FAILURE);
    }

  எனில் (status_only && !do_check)
    {
      error (0, 0,
       _("the --status option is meaningful only when verifying checksums"));
      usage (EXIT_FAILURE);
    }

  எனில் (warn && !do_check)
    {
      error (0, 0,
       _("the --warn option is meaningful only when verifying checksums"));
      usage (EXIT_FAILURE);
    }

  எனில் (quiet && !do_check)
    {
      error (0, 0,
       _("the --quiet option is meaningful only when verifying checksums"));
      usage (EXIT_FAILURE);
    }

  எனில் (strict & !do_check)
   {
     error (0, 0,
        _("the --strict option is meaningful only when verifying checksums"));
     usage (EXIT_FAILURE);
   }

  எனில் (prefix_tag == -1)
    prefix_tag = HASH_ALGO_CKSUM;

  எனில் (prefix_tag && !இருமம்)
   {
     /* This could be supported in a backwards compatible way
        by prefixing the output line with a space in text mode.
        However that's invasive enough that it was agreed to
        not support this mode with --tag, as --text use cases
        are adequately supported by the default output format.  */
#எனில் !HASH_ALGO_CKSUM
     error (0, 0, _("--tag does not support --text mode"));
#இல்லையெனில்
     error (0, 0, _("--text mode is only supported with --untagged"));
#endif
     usage (EXIT_FAILURE);
   }

  எனில் (!O_இருமம் && இருமம் < 0)
    இருமம் = 0;
  இல்லையெனில் எனில் (prefix_tag)
    இருமம் = 1;

  எழுத்து **operand_lim = argv + argc;
  எனில் (optind == argc)
    *operand_lim++ = bad_cast ("-");
  இல்லையெனில் எனில் (1 < argc - optind && raw_digest)
    error (EXIT_FAILURE, 0,
           _("the --raw option is not supported with multiple files"));

  க்கு (எழுத்து **operandp = argv + optind; operandp < operand_lim; operandp++)
    {
      எழுத்து *file = *operandp;
      எனில் (do_check)
        ok &= digest_check (file);
      இல்லையெனில்
        {
          முழு இருமம்_file = இருமம்;
          bool missing;
          intmax_t length;

          எனில் (! digest_file (file, &இருமம்_file, bin_buffer, &missing, &length))
            ok = false;
          இல்லையெனில்
            {
              DIGEST_OUT (file, இருமம்_file, bin_buffer, raw_digest, prefix_tag,
                          digest_delim, optind != argc, length);
              எனில் (ferror (stdout))
                write_error ();
            }
        }
    }

  எனில் (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, _("standard input"));

  திரும்பு ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
