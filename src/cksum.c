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

#ଅନ୍ତର୍ଭାଵ <config.h>

#ଅନ୍ତର୍ଭାଵ <getopt.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>

#ଅନ୍ତର୍ଭାଵ "assure.h"
#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "argmatch.h"
#ଅନ୍ତର୍ଭାଵ "c-ctype.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "xdectoint.h"
#ଅନ୍ତର୍ଭାଵ "xstrtol.h"

#ifndef HASH_ALGO_CKSUM
# define HASH_ALGO_CKSUM 0
#endif

#ଯଦି HASH_ALGO_SUM || HASH_ALGO_CKSUM
# include "sum.h"
#endif
#ଯଦି HASH_ALGO_CKSUM
# include "cksum.h"
# include "cksum_crc.h"
# include "base64.h"
#endif
#ଯଦି HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# include "blake2/b2sum.h"
#endif
#ଯଦି HASH_ALGO_MD5 || HASH_ALGO_CKSUM
# include "md5.h"
#endif
#ଯଦି HASH_ALGO_SHA1 || HASH_ALGO_CKSUM
# include "sha1.h"
#endif
#ଯଦି HASH_ALGO_SHA256 || HASH_ALGO_SHA224 || HASH_ALGO_CKSUM
# include "sha256.h"
#endif
#ଯଦି HASH_ALGO_SHA512 || HASH_ALGO_SHA384 || HASH_ALGO_CKSUM
# include "sha512.h"
#endif
#ଯଦି HASH_ALGO_CKSUM
# include "sha3.h"
#endif
#ଯଦି HASH_ALGO_CKSUM
# include "sm3.h"
#endif
#ଅନ୍ତର୍ଭାଵ "fadvise.h"
#ଅନ୍ତର୍ଭାଵ "stdio--.h"
#ଅନ୍ତର୍ଭାଵ "xbinary-io.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ଯଦି HASH_ALGO_SUM
# define PROGRAM_NAME "sum"
# define DIGEST_TYPE_STRING "BSD"
# define DIGEST_STREAM sumfns[sum_algorithm]
# define DIGEST_OUT sum_output_fns[sum_algorithm]
# define DIGEST_BITS 16
# define DIGEST_ALIGN 4
#ନଚେତଯଦି HASH_ALGO_CKSUM
# define MAX_DIGEST_BITS 512
# define MAX_DIGEST_ALIGN 8
# define DIGEST_TYPE_STRING algorithm_tags[cksum_algorithm]
# define DIGEST_STREAM cksumfns[cksum_algorithm]
# define DIGEST_OUT cksum_output_fns[cksum_algorithm]
# define DIGEST_BITS MAX_DIGEST_BITS
# define DIGEST_ALIGN MAX_DIGEST_ALIGN
#ନଚେତଯଦି HASH_ALGO_MD5
# define PROGRAM_NAME "md5sum"
# define DIGEST_TYPE_STRING "MD5"
# define DIGEST_STREAM md5_stream
# define DIGEST_BITS 128
# define DIGEST_REFERENCE "RFC 1321"
# define DIGEST_ALIGN 4
#ନଚେତଯଦି HASH_ALGO_BLAKE2
# define PROGRAM_NAME "b2sum"
# define DIGEST_TYPE_STRING "BLAKE2b"
# define DIGEST_STREAM blake2b_stream
# define DIGEST_BITS 512
# define DIGEST_REFERENCE "RFC 7693"
# define DIGEST_ALIGN 8
#ନଚେତଯଦି HASH_ALGO_SHA1
# define PROGRAM_NAME "sha1sum"
# define DIGEST_TYPE_STRING "SHA1"
# define DIGEST_STREAM sha1_stream
# define DIGEST_BITS 160
# define DIGEST_REFERENCE "FIPS-180-1"
# define DIGEST_ALIGN 4
#ନଚେତଯଦି HASH_ALGO_SHA256
# define PROGRAM_NAME "sha256sum"
# define DIGEST_TYPE_STRING "SHA256"
# define DIGEST_STREAM sha256_stream
# define DIGEST_BITS 256
# define DIGEST_REFERENCE "FIPS-180-2"
# define DIGEST_ALIGN 4
#ନଚେତଯଦି HASH_ALGO_SHA224
# define PROGRAM_NAME "sha224sum"
# define DIGEST_TYPE_STRING "SHA224"
# define DIGEST_STREAM sha224_stream
# define DIGEST_BITS 224
# define DIGEST_REFERENCE "RFC 3874"
# define DIGEST_ALIGN 4
#ନଚେତଯଦି HASH_ALGO_SHA512
# define PROGRAM_NAME "sha512sum"
# define DIGEST_TYPE_STRING "SHA512"
# define DIGEST_STREAM sha512_stream
# define DIGEST_BITS 512
# define DIGEST_REFERENCE "FIPS-180-2"
# define DIGEST_ALIGN 8
#ନଚେତଯଦି HASH_ALGO_SHA384
# define PROGRAM_NAME "sha384sum"
# define DIGEST_TYPE_STRING "SHA384"
# define DIGEST_STREAM sha384_stream
# define DIGEST_BITS 384
# define DIGEST_REFERENCE "FIPS-180-2"
# define DIGEST_ALIGN 8
#ଅନ୍ୟଥା
# error "Can't decide which hash algorithm to compile."
#endif
#ଯଦି !HASH_ALGO_SUM && !HASH_ALGO_CKSUM
# define DIGEST_OUT output_file
#endif

#ଯଦି HASH_ALGO_CKSUM
# define PROGRAM_NAME (!legacy_mode ? "cksum"			 \
                       : cksum_algorithm == md5 ? "md5sum"	 \
                       : cksum_algorithm == sha1 ? "sha1sum"	 \
                       : cksum_algorithm == sha224 ? "sha224sum" \
                       : cksum_algorithm == sha256 ? "sha256sum" \
                       : cksum_algorithm == sha384 ? "sha384sum" \
                       : cksum_algorithm == sha512 ? "sha512sum" \
                       : "cksum")
#endif

#ଯଦି HASH_ALGO_SUM
# define AUTHORS \
  proper_name ("Kayvan Aghaiepour"), \
  proper_name ("David MacKenzie")
#ନଚେତଯଦି HASH_ALGO_CKSUM
# define AUTHORS \
  proper_name_lite ("Padraig Brady", "P\303\241draig Brady"), \
  proper_name ("Q. Frank Xia")
#ନଚେତଯଦି HASH_ALGO_BLAKE2
# define AUTHORS \
  proper_name_lite ("Padraig Brady", "P\303\241draig Brady"), \
  proper_name ("Samuel Neves")
#ଅନ୍ୟଥା
# define AUTHORS \
  proper_name ("Ulrich Drepper"), \
  proper_name ("Scott Miller"), \
  proper_name ("David Madore")
#endif
#ଯଦି !HASH_ALGO_BLAKE2 && !HASH_ALGO_CKSUM
# define DIGEST_HEX_BYTES (DIGEST_BITS / 4)
#endif
#define DIGEST_BIN_BYTES (DIGEST_BITS / 8)

/* The minimum length of a valid digest line.  This length does
   not include any newline character at the end of a line.  */
#ଯଦି HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# define MIN_DIGEST_ଧାଡ଼ି_LENGTH 3 /* With -l 8.  */
#ଅନ୍ୟଥା
# define MIN_DIGEST_ଧାଡ଼ି_LENGTH \
   (DIGEST_HEX_BYTES /* length of hexadecimal message digest */ \
    + 1 /* blank */ \
    + 1 /* minimum filename length */ )
#endif

#ଯଦି !HASH_ALGO_SUM
ସ୍ଥିର ଶୂନ୍ୟ
output_file (ଅକ୍ଷର ଅଚଳ *file, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଦ୍ୱିମିକ_file, ଶୂନ୍ୟ ଅଚଳ *digest,
             bool raw, bool tagged, unsigned ଅକ୍ଷର delim, bool args,
             intmax_t length);
#endif

/* True if any of the files read were the standard input. */
ସ୍ଥିର bool have_read_stdin;

/* The minimum length of a valid checksum line for the selected algorithm.  */
ସ୍ଥିର idx_t min_digest_ଧାଡ଼ି_length;

/* Set to the length of a digest hex string for the selected algorithm.  */
ସ୍ଥିର idx_t digest_hex_bytes;

/* With --check, don't generate any output.
   The exit code indicates success or failure.  */
ସ୍ଥିର bool status_only = false;

/* With --check, print a message to standard error warning about each
   improperly formatted checksum line.  */
ସ୍ଥିର bool warn = false;

/* With --check, ignore missing files.  */
ସ୍ଥିର bool ignore_missing = false;

/* With --check, suppress the "OK" printed for each verified file.  */
ସ୍ଥିର bool quiet = false;

/* With --check, exit with a non-zero return code if any line is
   improperly formatted. */
ସ୍ଥିର bool strict = false;

/* Whether a BSD reversed format checksum is detected.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା bsd_reversed = -1;

/* line delimiter.  */
ସ୍ଥିର unsigned ଅକ୍ଷର digest_delim = '\n';

#ଯଦି HASH_ALGO_CKSUM
/* If true, print base64-encoded digests, not hex.  */
ସ୍ଥିର bool base64_digest = false;
#endif

/* If true, print binary digests, not hex.  */
ସ୍ଥିର bool raw_digest = false;

/* blake2b and sha3 allow the -l option.  Luckily they both have the same
   maximum digest size.  */
#ଯଦି HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# ଯଦି HASH_ALGO_CKSUM
static_assert (BLAKE2B_OUTBYTES == SHA3_512_DIGEST_SIZE);
# endif
# define DIGEST_MAX_LEN BLAKE2B_OUTBYTES
ସ୍ଥିର idx_t digest_length;
#endif /* HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM */

ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*digest_output_fn)(ଅକ୍ଷର ଅଚଳ *, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ଶୂନ୍ୟ ଅଚଳ *, bool,
                                 bool, unsigned ଅକ୍ଷର, bool, intmax_t);
#ଯଦି HASH_ALGO_SUM
ଗଣନା Algorithm
{
  bsd,
  sysv,
};

ସ୍ଥିର ଗଣନା Algorithm sum_algorithm;
ସ୍ଥିର sumfn sumfns[]=
{
  bsd_sum_stream,
  sysv_sum_stream,
};
ସ୍ଥିର digest_output_fn sum_output_fns[]=
{
  output_bsd,
  output_sysv,
};
#endif

#ଯଦି HASH_ALGO_CKSUM
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
md5_sum_stream (FILE *stream, ଶୂନ୍ୟ *resstream, MAYBE_UNUSED intmax_t *length)
{
  ଫେରନ୍ତୁ md5_stream (stream, resstream);
}
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
sha1_sum_stream (FILE *stream, ଶୂନ୍ୟ *resstream, MAYBE_UNUSED intmax_t *length)
{
  ଫେରନ୍ତୁ sha1_stream (stream, resstream);
}
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
sha224_sum_stream (FILE *stream, ଶୂନ୍ୟ *resstream,
                   MAYBE_UNUSED intmax_t *length)
{
  ଫେରନ୍ତୁ sha224_stream (stream, resstream);
}
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
sha256_sum_stream (FILE *stream, ଶୂନ୍ୟ *resstream,
                   MAYBE_UNUSED intmax_t *length)
{
  ଫେରନ୍ତୁ sha256_stream (stream, resstream);
}
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
sha384_sum_stream (FILE *stream, ଶୂନ୍ୟ *resstream,
                   MAYBE_UNUSED intmax_t *length)
{
  ଫେରନ୍ତୁ sha384_stream (stream, resstream);
}
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
sha512_sum_stream (FILE *stream, ଶୂନ୍ୟ *resstream,
                   MAYBE_UNUSED intmax_t *length)
{
  ଫେରନ୍ତୁ sha512_stream (stream, resstream);
}
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
sha2_sum_stream (FILE *stream, ଶୂନ୍ୟ *resstream, intmax_t *length)
{
  ବାଛ (*length)
    {
    ସ୍ଥିତି SHA224_DIGEST_SIZE:
      ଫେରନ୍ତୁ sha224_stream (stream, resstream);
    ସ୍ଥିତି SHA256_DIGEST_SIZE:
      ଫେରନ୍ତୁ sha256_stream (stream, resstream);
    ସ୍ଥିତି SHA384_DIGEST_SIZE:
      ଫେରନ୍ତୁ sha384_stream (stream, resstream);
    ସ୍ଥିତି SHA512_DIGEST_SIZE:
      ଫେରନ୍ତୁ sha512_stream (stream, resstream);
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      affirm (false);
    }
}
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
sha3_sum_stream (FILE *stream, ଶୂନ୍ୟ *resstream, intmax_t *length)
{
  ବାଛ (*length)
    {
    ସ୍ଥିତି SHA3_224_DIGEST_SIZE:
      ଫେରନ୍ତୁ sha3_224_stream (stream, resstream);
    ସ୍ଥିତି SHA3_256_DIGEST_SIZE:
      ଫେରନ୍ତୁ sha3_256_stream (stream, resstream);
    ସ୍ଥିତି SHA3_384_DIGEST_SIZE:
      ଫେରନ୍ତୁ sha3_384_stream (stream, resstream);
    ସ୍ଥିତି SHA3_512_DIGEST_SIZE:
      ଫେରନ୍ତୁ sha3_512_stream (stream, resstream);
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      affirm (false);
    }
}
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
blake2b_sum_stream (FILE *stream, ଶୂନ୍ୟ *resstream, intmax_t *length)
{
  ଫେରନ୍ତୁ blake2b_stream (stream, resstream, *length);
}
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
sm3_sum_stream (FILE *stream, ଶୂନ୍ୟ *resstream, MAYBE_UNUSED intmax_t *length)
{
  ଫେରନ୍ତୁ sm3_stream (stream, resstream);
}

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *ଅଚଳ algorithm_args[] =
{
  "bsd", "sysv", "crc", "crc32b", "md5", "sha1",
  "sha224", "sha256", "sha384", "sha512", /* Legacy naming */
  "sha2", "sha3", "blake2b", "sm3", ଖାଲି
};
ସ୍ଥିର ଗଣନା Algorithm ଅଚଳ algorithm_types[] =
{
  bsd, sysv, crc, crc32b, md5, sha1,
  sha224, sha256, sha384, sha512,
  sha2, sha3, blake2b, sm3,
};
ARGMATCH_VERIFY (algorithm_args, algorithm_types);

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *ଅଚଳ algorithm_tags[] =
{
  "BSD", "SYSV", "CRC", "CRC32B", "MD5", "SHA1",
  "SHA224", "SHA256", "SHA384", "SHA512",
  "SHA2", "SHA3", "BLAKE2b", "SM3", ଖାଲି
};
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଚଳ algorithm_bits[] =
{
  16, 16, 32, 32, 128, 160,
  224, 256, 384, 512,
  512, 512, 512, 256, 0
};

static_assert (countof (algorithm_bits) == countof (algorithm_args));

ସ୍ଥିର bool algorithm_specified = false;
ସ୍ଥିର bool legacy_mode = false;
ଗଣନା Algorithm cksum_algorithm = crc;

ସ୍ଥିର sumfn cksumfns[]=
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
ସ୍ଥିର digest_output_fn cksum_output_fns[]=
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

ଗଣନା
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

#ଯଦି HASH_ALGO_CKSUM
ସ୍ଥିର ସଂରଚନା option ଅଚଳ legacy_long_options[] =
{
  { "check", no_argument, ଖାଲି, 'c' },
  { "ignore-missing", no_argument, ଖାଲି, IGNORE_MISSING_OPTION },
  { "quiet", no_argument, ଖାଲି, QUIET_OPTION },
  { "status", no_argument, ଖାଲି, STATUS_OPTION },
  { "warn", no_argument, ଖାଲି, 'w' },
  { "strict", no_argument, ଖାଲି, STRICT_OPTION },
  { "tag", no_argument, ଖାଲି, TAG_OPTION },
  { "zero", no_argument, ଖାଲି, 'z' },

  { "binary", no_argument, ଖାଲି, 'b' }, /* Deprecated.  */
  { "text", no_argument, ଖାଲି, 't' }, /* Deprecated.  */

  { GETOPT_HELP_OPTION_DECL },
  { GETOPT_VERSION_OPTION_DECL },
  { ଖାଲି, 0, ଖାଲି, 0 }
};
#endif

ସ୍ଥିର ସଂରଚନା option ଅଚଳ long_options[] =
{
#ଯଦି HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
  { "length", required_argument, ଖାଲି, 'l'},
#endif

#ଯଦି !HASH_ALGO_SUM
  { "check", no_argument, ଖାଲି, 'c' },
  { "ignore-missing", no_argument, ଖାଲି, IGNORE_MISSING_OPTION},
  { "quiet", no_argument, ଖାଲି, QUIET_OPTION },
  { "status", no_argument, ଖାଲି, STATUS_OPTION },
  { "warn", no_argument, ଖାଲି, 'w' },
  { "strict", no_argument, ଖାଲି, STRICT_OPTION },
  { "tag", no_argument, ଖାଲି, TAG_OPTION },
  { "zero", no_argument, ଖାଲି, 'z' },

# ଯଦି HASH_ALGO_CKSUM
  { "algorithm", required_argument, ଖାଲି, 'a'},
  { "base64", no_argument, ଖାଲି, BASE64_OPTION },
  { "debug", no_argument, ଖାଲି, DEBUG_PROGRAM_OPTION},
  { "raw", no_argument, ଖାଲି, RAW_OPTION},
  { "untagged", no_argument, ଖାଲି, UNTAG_OPTION },
# endif
  { "binary", no_argument, ଖାଲି, 'b' }, /* Deprecated.  */
  { "text", no_argument, ଖାଲି, 't' }, /* Deprecated.  */

#ଅନ୍ୟଥା
  {"sysv", no_argument, ଖାଲି, 's'},  /* Not in cksum.  */
#endif

  { GETOPT_HELP_OPTION_DECL },
  { GETOPT_VERSION_OPTION_DECL },
  { ଖାଲି, 0, ଖାଲି, 0 }
};

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"), program_name);
#ଯଦି HASH_ALGO_CKSUM
    ଯଦି (legacy_mode)
      {
        printf (_("\
Print or check %s checksums.\n\
"), DIGEST_TYPE_STRING);
        fputs (_("\
Legacy interface to the cksum(1) utility.\n\
"), stdout);
      }
    ଅନ୍ୟଥା
      fputs (_("\
Print or verify checksums.\n\
By ପୂର୍ବନିର୍ଦ୍ଧାରିତ use the 32 bit CRC algorithm.\n\
"), stdout);
#ଅନ୍ୟଥା
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
#ଯଦି HASH_ALGO_SUM
      oputs (_("\
  -r\n\
         use BSD sum algorithm (the ପୂର୍ବନିର୍ଦ୍ଧାରିତ), use 1K blocks\n\
"));
      oputs (_("\
  -s, --sysv\n\
         use System V sum algorithm, use 512 bytes blocks\n\
"));
#endif
#ଯଦି HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
        emit_mandatory_arg_note ();
#endif
#ଯଦି HASH_ALGO_CKSUM
     ଯଦି (! legacy_mode)
        oputs (_("\
  -a, --algorithm=TYPE\n\
         select the digest type to use.  See DIGEST below\n\
"));
     ଯଦି (! legacy_mode)
        oputs (_("\
      --base64\n\
         emit base64-encoded digests, not hexadecimal\n\
"));
#endif
#ଯଦି !HASH_ALGO_SUM
# ଯଦି HASH_ALGO_CKSUM
     ଯଦି (legacy_mode)
       {
# endif
      ଯଦି (O_ଦ୍ୱିମିକ)
        oputs (_("\
  -b, --ଦ୍ୱିମିକ\n\
         ପଠ in ଦ୍ୱିମିକ mode (ପୂର୍ବନିର୍ଦ୍ଧାରିତ unless reading tty stdin)\n\
"));
      ଅନ୍ୟଥା
        oputs (_("\
  -b, --ଦ୍ୱିମିକ\n\
         ପଠ in ଦ୍ୱିମିକ mode\n\
"));
# ଯଦି HASH_ALGO_CKSUM
       }
# endif
        oputs (_("\
  -c, --check\n\
         ପଠ checksums from the FILEs and check them\n\
"));
# ଯଦି HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
#  ଯଦି HASH_ALGO_CKSUM
     ଯଦି (! legacy_mode)
#  endif
        oputs (_("\
  -l, --length=BITS\n\
         digest length in bits; must not exceed the max size\n\
         and must be a multiple of 8 ପାଇଁ blake2b;\n\
         must be 224, 256, 384, or 512 ପାଇଁ sha2 or sha3\n\
"));
# endif
# ଯଦି HASH_ALGO_CKSUM
    ଯଦି (! legacy_mode)
        oputs (_("\
      --raw\n\
         emit a raw ଦ୍ୱିମିକ digest, not hexadecimal\n\
"));
    ଯଦି (legacy_mode)
      oputs (_("\
      --tag\n\
         create a BSD-style checksum\n\
"));
    ଅନ୍ୟଥା
      oputs (_("\
      --tag\n\
         create a BSD-style checksum (the ପୂର୍ବନିର୍ଦ୍ଧାରିତ)\n\
"));
    ଯଦି (! legacy_mode)
      oputs (_("\
      --untagged\n\
         create a reversed style checksum, without digest type\n\
"));
# ଅନ୍ୟଥା
      oputs (_("\
      --tag\n\
         create a BSD-style checksum\n\
"));
# endif
# ଯଦି HASH_ALGO_CKSUM
    ଯଦି (legacy_mode)
      {
# endif
      ଯଦି (O_ଦ୍ୱିମିକ)
        oputs (_("\
  -t, --text\n\
         ପଠ in text mode (ପୂର୍ବନିର୍ଦ୍ଧାରିତ ଯଦି reading tty stdin)\n\
"));
      ଅନ୍ୟଥା
        oputs (_("\
  -t, --text\n\
         ପଠ in text mode (ପୂର୍ବନିର୍ଦ୍ଧାରିତ)\n\
"));
# ଯଦି HASH_ALGO_CKSUM
      }
# endif
      oputs (_("\
  -z, --zero\n\
         end each output ଧାଡ଼ି with NUL, not newline,\n\
         and disable file name escaping\n\
"));
      fputs (_("\
\n\
The following five options are useful only when verifying checksums:\n\
"), stdout);
      oputs (_("\
      --ignore-missing\n\
         don't fail or report status ପାଇଁ missing files\n\
"));
      oputs (_("\
      --quiet\n\
         don't print OK ପାଇଁ each successfully verified file\n\
"));
      oputs (_("\
      --status\n\
         don't output anything, status code shows success\n\
"));
      oputs (_("\
      --strict\n\
         exit non-zero ପାଇଁ improperly formatted checksum ଧାଡ଼ିଗୁଡ଼ିକ\n\
"));
      oputs (_("\
  -w, --warn\n\
         warn about improperly formatted checksum ଧାଡ଼ିଗୁଡ଼ିକ\n\
"));
#endif
#ଯଦି HASH_ALGO_CKSUM
    ଯଦି (! legacy_mode)
      oputs (_("\
      --debug\n\
         indicate which implementation used\n\
"));
#endif
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
#ଯଦି HASH_ALGO_CKSUM
    ଯଦି (! legacy_mode)
      fputs (_("\
\n\
DIGEST determines the digest algorithm and ପୂର୍ବନିର୍ଦ୍ଧାରିତ output format:\n\
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
#ଯଦି !HASH_ALGO_SUM && !HASH_ALGO_CKSUM
      printf (_("\
\n\
The sums are computed as described in %s.\n"), DIGEST_REFERENCE);
      fputs (_("\
When checking, the ନିବେଶ should be a former output of this program.\n\
The ପୂର୍ବନିର୍ଦ୍ଧାରିତ mode is to print a ଧାଡ଼ି with: checksum, a space,\n\
a character indicating ନିବେଶ mode ('*' ପାଇଁ ଦ୍ୱିମିକ, ' ' ପାଇଁ text\n\
or where ଦ୍ୱିମିକ is insignificant), and name ପାଇଁ each FILE.\n\
\n\
There is no difference between ଦ୍ୱିମିକ mode and text mode on GNU systems.\
\n"), stdout);
#endif
#ଯଦି HASH_ALGO_CKSUM
      fputs (_("\
When checking, the ନିବେଶ should be a former output of this program,\n\
or equivalent standalone program.\
\n"), stdout);
#endif
      emit_ancillary_info (PROGRAM_NAME);
    }

  exit (status);
}

#ଯଦି !HASH_ALGO_SUM
/* Given a string S, return TRUE if it contains problematic characters
   that need escaping.  Note we escape '\' itself to provide some forward
   compat to introduce escaping of other characters.  */

ATTRIBUTE_PURE
ସ୍ଥିର bool
problematic_chars (ଅକ୍ଷର ଅଚଳ *s)
{
  idx_t length = strcspn (s, "\\\n\r");
  ଫେରନ୍ତୁ s[length] != '\0';
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

ସ୍ଥିର ଅକ୍ଷର *
filename_unescape (ଅକ୍ଷର *s, idx_t s_len)
{
  ଅକ୍ଷର *dst = s;

  ପାଇଁ (idx_t i = 0; i < s_len; i++)
    {
      ବାଛ (s[i])
        {
        ସ୍ଥିତି '\\':
          ଯଦି (i == s_len - 1)
            {
              /* File name ends with an unescaped backslash: invalid.  */
              ଫେରନ୍ତୁ ଖାଲି;
            }
          ++i;
          ବାଛ (s[i])
            {
            ସ୍ଥିତି 'n':
              *dst++ = '\n';
              ଭାଙ୍ଗ;
            ସ୍ଥିତି 'r':
              *dst++ = '\r';
              ଭାଙ୍ଗ;
            ସ୍ଥିତି '\\':
              *dst++ = '\\';
              ଭାଙ୍ଗ;
            ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
              /* Only '\', 'n' or 'r' may follow a backslash.  */
              ଫେରନ୍ତୁ ଖାଲି;
            }
          ଭାଙ୍ଗ;

        ସ୍ଥିତି '\0':
          /* The file name may not contain a NUL.  */
          ଫେରନ୍ତୁ ଖାଲି;

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          *dst++ = s[i];
          ଭାଙ୍ଗ;
        }
    }
  ଯଦି (dst < s + s_len)
    *dst = '\0';

  ଫେରନ୍ତୁ s;
}

/* Return true if S is a LEN-byte NUL-terminated string of hex or base64
   digits and has the expected length.  Otherwise, return false.  */
ATTRIBUTE_PURE
ସ୍ଥିର bool
valid_digits (unsigned ଅକ୍ଷର ଅଚଳ *s, idx_t len)
{
#ଯଦି HASH_ALGO_CKSUM
  ଯଦି (len == BASE64_LENGTH (digest_length >> 3))
    {
      idx_t i;
      ପାଇଁ (i = 0; i < len - digest_length % 3; i++)
        {
          ଯଦି (!isbase64 (*s))
            ଫେରନ୍ତୁ false;
          ++s;
        }
      ପାଇଁ ( ; i < len; i++)
        {
          ଯଦି (*s != '=')
            ଫେରନ୍ତୁ false;
          ++s;
        }
    }
  ଅନ୍ୟଥା
#endif
  ଯଦି (len == digest_hex_bytes)
    {
      ପାଇଁ (idx_t i = 0; i < digest_hex_bytes; i++)
        {
          ଯଦି (!c_isxdigit (*s))
            ଫେରନ୍ତୁ false;
          ++s;
        }
    }
  ଅନ୍ୟଥା
    ଫେରନ୍ତୁ false;

  ଫେରନ୍ତୁ *s == '\0';
}

/* Split the checksum string S (of length S_LEN) from a BSD 'md5' or
   'sha1' command into two parts: a hexadecimal digest, and the file
   name.  S is modified.  Set *D_LEN to the length of the digest string.
   Return true if successful.  */

ସ୍ଥିର bool
bsd_split_3 (ଅକ୍ଷର *s, idx_t s_len,
             unsigned ଅକ୍ଷର **digest, idx_t *d_len,
             ଅକ୍ଷର **file_name, bool escaped_filename)
{
  ଯଦି (s_len == 0)
    ଫେରନ୍ତୁ false;

  /* Find end of filename.  */
  idx_t i = s_len - 1;
  ଯେତେବେଳେ (i && s[i] != ')')
    i--;

  ଯଦି (s[i] != ')')
    ଫେରନ୍ତୁ false;

  *file_name = s;

  ଯଦି (escaped_filename && filename_unescape (s, i) == ଖାଲି)
    ଫେରନ୍ତୁ false;

  s[i++] = '\0';

  ଯେତେବେଳେ (c_isblank (s[i]))
    i++;

  ଯଦି (s[i] != '=')
    ଫେରନ୍ତୁ false;

  i++;

  ଯେତେବେଳେ (c_isblank (s[i]))
    i++;

  *digest = (unsigned ଅକ୍ଷର *) &s[i];

  *d_len = s_len - i;
  ଫେରନ୍ତୁ valid_digits (*digest, *d_len);
}

#ଯଦି HASH_ALGO_CKSUM
/* Return the corresponding Algorithm for the string S,
   or -1 for no match.  */

ସ୍ଥିର ptrdiff_t
algorithm_from_tag (ଅକ୍ଷର *s)
{
  /* Limit check size to this length for perf reasons.  */
  ସ୍ଥିର idx_t max_tag_len;
  ଯଦି (! max_tag_len)
    {
      ଅକ୍ଷର ଅଚଳ * ଅଚଳ * tag = algorithm_tags;
      ଯେତେବେଳେ (*tag)
        {
          idx_t tag_len = strlen (*tag++);
          max_tag_len = MAX (tag_len, max_tag_len);
        }
    }

  idx_t i = 0;

  /* Find end of tag */
  ଯେତେବେଳେ (i <= max_tag_len && s[i] && ! c_isblank (s[i])
         && s[i] != '-' && s[i] != '(')
    ++i;

  ଯଦି (i > max_tag_len)
    ଫେରନ୍ତୁ -1;

  /* Terminate tag, and lookup.  */
  ଅକ୍ଷର sep = s[i];
  s[i] = '\0';
  ptrdiff_t algo = argmatch_exact (s, algorithm_tags);
  s[i] = sep;

  ଫେରନ୍ତୁ algo;
}
#endif

/* Split the string S (of length S_LEN) into three parts:
   a hexadecimal digest, binary flag, and the file name.
   S is modified.  Set *D_LEN to the length of the digest string.
   Return true if successful.  */

ସ୍ଥିର bool
split_3 (ଅକ୍ଷର *s, idx_t s_len,
         unsigned ଅକ୍ଷର **digest, idx_t *d_len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *ଦ୍ୱିମିକ, ଅକ୍ଷର **file_name)
{
  bool escaped_filename = false;
  idx_t algo_name_len;

  idx_t i = 0;
  ଯେତେବେଳେ (c_isblank (s[i]))
    ++i;

  ଯଦି (s[i] == '\\')
    {
      ++i;
      escaped_filename = true;
    }

  /* Check for BSD-style checksum line. */

#ଯଦି HASH_ALGO_CKSUM
  ଯଦି (! algorithm_specified || cksum_algorithm == sha2)
    {
      ptrdiff_t algo_tag = algorithm_from_tag (s + i);
      ଯଦି (! algorithm_specified)
        {
          ଯଦି (algo_tag >= 0)
            {
              ଯଦି (algo_tag <= crc32b)
                ଫେରନ୍ତୁ false;  /* We don't support checking these formats.  */
              cksum_algorithm = algo_tag;
            }
          ଅନ୍ୟଥା
            ଫେରନ୍ତୁ false;      /* We only support tagged format without -a.  */
        }
      ଅନ୍ୟଥା
        {
          ଯଦି (cksum_algorithm == sha2 && (algo_tag == sha2
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
  ଯଦି (STREQ_LEN (s + i, DIGEST_TYPE_STRING, algo_name_len))
    {
      i += algo_name_len;
#ଯଦି HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM

# ଯଦି HASH_ALGO_BLAKE2
      digest_length = DIGEST_MAX_LEN * 8;
# ଅନ୍ୟଥା
      digest_length = algorithm_bits[cksum_algorithm];
# endif
      ଯଦି (s[i] == '-')  /* length specified. Not base64 */
        {
          ++i;
          intmax_t length;
          ଅକ୍ଷର *siend;
          ଯଦି (!c_isdigit (s[i])
              || xstrtoimax (s + i, &siend, 0, &length, ଖାଲି) != LONGINT_OK)
            ଫେରନ୍ତୁ false;
# ଯଦି HASH_ALGO_CKSUM
          ଅନ୍ୟଥା ଯଦି (cksum_algorithm == sha2 || cksum_algorithm == sha3)
            {
              ଯଦି (length != SHA224_DIGEST_SIZE * 8
                  && length != SHA256_DIGEST_SIZE * 8
                  && length != SHA384_DIGEST_SIZE * 8
                  && length != SHA512_DIGEST_SIZE * 8)
                ଫେରନ୍ତୁ false;
            }
# endif
          ଅନ୍ୟଥା ଯଦି (!(0 < length && length <= digest_length && length % 8 == 0))
            ଫେରନ୍ତୁ false;

          i = siend - s;
          digest_length = length;
        }
      digest_hex_bytes = digest_length >> 2;
#endif
      ଯଦି (s[i] == ' ')
        ++i;
      ଯଦି (s[i] == '(')  /* not base64 */
        {
          ++i;
          *ଦ୍ୱିମିକ = 0;
          ଫେରନ୍ତୁ bsd_split_3 (s + i, s_len - i,
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
  ଯଦି (s_len - i < min_digest_ଧାଡ଼ି_length + (s[i] == '\\'))
    ଫେରନ୍ତୁ false;

  *digest = (unsigned ଅକ୍ଷର *) &s[i];

#ଯଦି HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
  /* Auto determine length.  */
# ଯଦି HASH_ALGO_CKSUM
  ଯଦି (cksum_algorithm == blake2b
      || cksum_algorithm == sha2 || cksum_algorithm == sha3) {
# endif
  unsigned ଅକ୍ଷର ଅଚଳ *hp = *digest;
  digest_hex_bytes = 0;
  ପାଇଁ (; c_isxdigit (*hp); ++hp, ++digest_hex_bytes)
    ;
# ଯଦି HASH_ALGO_CKSUM
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
  ପାଇଁ (; isubase64 (*hp); ++hp, ++digest_base64_bytes)
    ;
  ପାଇଁ (; *hp == '='; ++hp, ++trailing_equals)
    ;
  ଯଦି ((cksum_algorithm == sha2 || cksum_algorithm == sha3)
      && digest_hex_bytes >> 1 != SHA224_DIGEST_SIZE
      && digest_hex_bytes >> 1 != SHA256_DIGEST_SIZE
      && digest_hex_bytes >> 1 != SHA384_DIGEST_SIZE
      && digest_hex_bytes >> 1 != SHA512_DIGEST_SIZE)
    {
      ଯଦି (digest_base64_bytes + trailing_equals
          == BASE64_LENGTH (SHA224_DIGEST_SIZE))
        digest_hex_bytes = SHA224_DIGEST_SIZE * 2;
      ଅନ୍ୟଥା ଯଦି (digest_base64_bytes + trailing_equals
               == BASE64_LENGTH (SHA256_DIGEST_SIZE))
        digest_hex_bytes = SHA256_DIGEST_SIZE * 2;
      ଅନ୍ୟଥା ଯଦି (digest_base64_bytes + trailing_equals
               == BASE64_LENGTH (SHA384_DIGEST_SIZE))
        digest_hex_bytes = SHA384_DIGEST_SIZE * 2;
      ଅନ୍ୟଥା ଯଦି (digest_base64_bytes + trailing_equals
               == BASE64_LENGTH (SHA512_DIGEST_SIZE))
        digest_hex_bytes = SHA512_DIGEST_SIZE * 2;
      ଅନ୍ୟଥା
        ଫେରନ୍ତୁ false;
    }
  ଅନ୍ୟଥା ଯଦି (cksum_algorithm == blake2b
           && digest_hex_bytes < digest_base64_bytes)
    {
      ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 8; j <= DIGEST_MAX_LEN * 8; j += 8)
        {
          ଯଦି (BASE64_LENGTH (j / 8) == digest_base64_bytes + trailing_equals
              && j % 3 == trailing_equals)
            {
              digest_hex_bytes = j / 4;
              ଭାଙ୍ଗ;
            }
        }
    }
# endif
  ଯଦି (digest_hex_bytes < 2 || digest_hex_bytes % 2
      || DIGEST_MAX_LEN * 2 < digest_hex_bytes)
    ଫେରନ୍ତୁ false;
  digest_length = digest_hex_bytes * 4;
# ଯଦି HASH_ALGO_CKSUM
  }
# endif
#endif

  /* This field must be the hexadecimal or base64 representation
     of the message digest.  */
  ଯେତେବେଳେ (s[i] && !c_isblank (s[i]))
    i++;

  /* The digest must be followed by at least one whitespace character.  */
  ଯଦି (i == s_len)
    ଫେରନ୍ତୁ false;

  *d_len = &s[i] - (ଅକ୍ଷର *) *digest;
  s[i++] = '\0';

  ଯଦି (! valid_digits (*digest, *d_len))
    ଫେରନ୍ତୁ false;

  /* If "bsd reversed" format detected.  */
  ଯଦି ((s_len - i == 1) || (s[i] != ' ' && s[i] != '*'))
    {
      /* Don't allow mixing bsd and standard formats,
         to minimize security issues with attackers
         renaming files with leading spaces.
         This assumes that with bsd format checksums
         that the first file name does not have
         a leading ' ' or '*'.  */
      ଯଦି (bsd_reversed == 0)
        ଫେରନ୍ତୁ false;
      bsd_reversed = 1;
    }
  ଅନ୍ୟଥା ଯଦି (bsd_reversed != 1)
    {
      bsd_reversed = 0;
      *ଦ୍ୱିମିକ = (s[i++] == '*');
    }

  /* All characters between the type indicator and end of line are
     significant -- that includes leading and trailing white space.  */
  *file_name = &s[i];

  ଯଦି (escaped_filename)
    ଫେରନ୍ତୁ filename_unescape (&s[i], s_len - i) != ଖାଲି;

  ଫେରନ୍ତୁ true;
}

#ଯଦି !HASH_ALGO_SUM
/* If ESCAPE is true, then translate each:
   NEWLINE byte to the string, "\\n",
   CARRIAGE RETURN byte to the string, "\\r",
   and each backslash to "\\\\".  */
ସ୍ଥିର ଶୂନ୍ୟ
print_filename (ଅକ୍ଷର ଅଚଳ *file, bool escape)
{
  ଯଦି (! escape)
    {
      fputs (file, stdout);
      ଫେରନ୍ତୁ;
    }

  ଯେତେବେଳେ (*file)
    {
      ବାଛ (*file)
        {
        ସ୍ଥିତି '\n':
          fputs ("\\n", stdout);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି '\r':
          fputs ("\\r", stdout);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି '\\':
          fputs ("\\\\", stdout);
          ଭାଙ୍ଗ;

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          putchar (*file);
          ଭାଙ୍ଗ;
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

ସ୍ଥିର bool
digest_file (ଅକ୍ଷର ଅଚଳ *filename, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *ଦ୍ୱିମିକ, unsigned ଅକ୍ଷର *bin_result,
             bool *missing, MAYBE_UNUSED intmax_t *length)
{
  FILE *fp;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା err;
  bool is_stdin = streq (filename, "-");

  *missing = false;

  ଯଦି (is_stdin)
    {
      have_read_stdin = true;
      fp = stdin;
      ଯଦି (O_ଦ୍ୱିମିକ && *ଦ୍ୱିମିକ)
        {
          ଯଦି (*ଦ୍ୱିମିକ < 0)
            *ଦ୍ୱିମିକ = ! isatty (STDIN_FILENO);
          ଯଦି (*ଦ୍ୱିମିକ)
            xset_ଦ୍ୱିମିକ_mode (STDIN_FILENO, O_ଦ୍ୱିମିକ);
        }
    }
  ଅନ୍ୟଥା
    {
      fp = fopen (filename, O_ଦ୍ୱିମିକ ? (*ଦ୍ୱିମିକ ? "rb" : "rt") : "r");
      ଯଦି (fp == ଖାଲି)
        {
          ଯଦି (ignore_missing && errno == ENOENT)
            {
              *missing = true;
              ଫେରନ୍ତୁ true;
            }
          error (0, errno, "%s", quotef (filename));
          ଫେରନ୍ତୁ false;
        }
    }

  fadvise (fp, FADVISE_SEQUENTIAL);

#ଯଦି HASH_ALGO_CKSUM
  ଯଦି (cksum_algorithm == blake2b
      || cksum_algorithm == sha2 || cksum_algorithm == sha3)
    *length = digest_length >> 3;
  err = DIGEST_STREAM (fp, bin_result, length);
#ନଚେତଯଦି HASH_ALGO_SUM
  err = DIGEST_STREAM (fp, bin_result, length);
#ନଚେତଯଦି HASH_ALGO_BLAKE2
  err = DIGEST_STREAM (fp, bin_result, digest_length >> 3);
#ଅନ୍ୟଥା
  err = DIGEST_STREAM (fp, bin_result);
#endif
  err = err ? errno : 0;
  ଯଦି (is_stdin)
    clearerr (fp);
  ଅନ୍ୟଥା ଯଦି (fclose (fp) != 0 && !err)
    err = errno;

  ଯଦି (err)
    {
      error (0, err, "%s", quotef (filename));
      ଫେରନ୍ତୁ false;
    }

  ଫେରନ୍ତୁ true;
}

#ଯଦି !HASH_ALGO_SUM
ସ୍ଥିର ଶୂନ୍ୟ
output_file (ଅକ୍ଷର ଅଚଳ *file, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଦ୍ୱିମିକ_file, ଶୂନ୍ୟ ଅଚଳ *digest,
             MAYBE_UNUSED bool raw, bool tagged, unsigned ଅକ୍ଷର delim,
             MAYBE_UNUSED bool args, MAYBE_UNUSED intmax_t length)
{
# ଯଦି HASH_ALGO_CKSUM
  ଯଦି (raw)
    {
      fwrite (digest, 1, digest_length >> 3, stdout);
      ଫେରନ୍ତୁ;
    }
# endif

  unsigned ଅକ୍ଷର ଅଚଳ *bin_buffer = digest;

  /* Output a leading backslash if the file name contains problematic chars.  */
  bool needs_escape = delim == '\n' && problematic_chars (file);

  ଯଦି (needs_escape)
    putchar ('\\');

  ଯଦି (tagged)
    {
# ଯଦି HASH_ALGO_CKSUM
      ଯଦି (cksum_algorithm == sha2)
        printf ("SHA%u", (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) digest_length);
      ଅନ୍ୟଥା
# endif
      fputs (DIGEST_TYPE_STRING, stdout);
# ଯଦି HASH_ALGO_BLAKE2
      ଯଦି (digest_length < DIGEST_MAX_LEN * 8)
        printf ("-%u", (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) digest_length);
# elif HASH_ALGO_CKSUM
      ଯଦି (cksum_algorithm == sha3)
        printf ("-%u", (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) digest_length);
      ଯଦି (cksum_algorithm == blake2b)
        {
          ଯଦି (digest_length < DIGEST_MAX_LEN * 8)
            printf ("-%u", (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) digest_length);
        }
# endif
      fputs (" (", stdout);
      print_filename (file, needs_escape);
      fputs (") = ", stdout);
    }

# ଯଦି HASH_ALGO_CKSUM
  ଯଦି (base64_digest)
    {
      ଅକ୍ଷର b64[BASE64_LENGTH (DIGEST_BIN_BYTES) + 1];
      base64_encode ((ଅକ୍ଷର ଅଚଳ *) bin_buffer, digest_length >> 3,
                     b64, ଆକାର b64);
      fputs (b64, stdout);
    }
  ଅନ୍ୟଥା
# endif
    {
      ପାଇଁ (idx_t i = 0; i < (digest_hex_bytes >> 1); ++i)
        printf ("%02x", bin_buffer[i]);
    }

  ଯଦି (!tagged)
    {
      putchar (' ');
      putchar (ଦ୍ୱିମିକ_file ? '*' : ' ');
      print_filename (file, needs_escape);
    }

  putchar (delim);
}
#endif

#ଯଦି HASH_ALGO_CKSUM
/* Return true if B64_DIGEST is the same as the base64 digest of the
   DIGEST_LENGTH/8 bytes at BIN_BUFFER.  */
ସ୍ଥିର bool
b64_equal (unsigned ଅକ୍ଷର ଅଚଳ *b64_digest, unsigned ଅକ୍ଷର ଅଚଳ *bin_buffer)
{
  idx_t b64_n_bytes = BASE64_LENGTH (digest_length >> 3);
  ଅକ୍ଷର b64[BASE64_LENGTH (DIGEST_BIN_BYTES) + 1];
  base64_encode ((ଅକ୍ଷର ଅଚଳ *) bin_buffer, digest_length >> 3,
                 b64, ଆକାର b64);
  ଫେରନ୍ତୁ memeq (b64_digest, b64, b64_n_bytes + 1);
}
#endif

/* Return true if HEX_DIGEST is the same as the hex-encoded digest of the
   DIGEST_LENGTH/8 bytes at BIN_BUFFER.  */
ସ୍ଥିର bool
hex_equal (unsigned ଅକ୍ଷର ଅଚଳ *hex_digest, unsigned ଅକ୍ଷର ଅଚଳ *bin_buffer)
{
  ସ୍ଥିର ଅଚଳ ଅକ୍ଷର bin2hex[] = { '0', '1', '2', '3',
                                  '4', '5', '6', '7',
                                  '8', '9', 'a', 'b',
                                  'c', 'd', 'e', 'f' };
  idx_t digest_bin_bytes = digest_hex_bytes >> 1;

  /* Compare generated binary number with text representation
     in check file.  Ignore case of hex digits.  */
  idx_t cnt;
  ପାଇଁ (cnt = 0; cnt < digest_bin_bytes; ++cnt)
    {
      ଯଦି (c_tolower (hex_digest[2 * cnt])
          != bin2hex[bin_buffer[cnt] >> 4]
          || (c_tolower (hex_digest[2 * cnt + 1])
              != (bin2hex[bin_buffer[cnt] & 0xf])))
        ଭାଙ୍ଗ;
    }
  ଫେରନ୍ତୁ cnt == digest_bin_bytes;
}

ସ୍ଥିର bool
digest_check (ଅକ୍ଷର ଅଚଳ *checkfile_name)
{
  FILE *checkfile_stream;
  intmax_t n_misformatted_ଧାଡ଼ିଗୁଡ଼ିକ = 0;
  intmax_t n_mismatched_checksums = 0;
  intmax_t n_open_or_read_failures = 0;
  bool properly_formatted_ଧାଡ଼ିଗୁଡ଼ିକ = false;
  bool matched_checksums = false;
  unsigned ଅକ୍ଷର bin_buffer_unaligned[DIGEST_BIN_BYTES + DIGEST_ALIGN];
  /* Make sure bin_buffer is properly aligned. */
  unsigned ଅକ୍ଷର *bin_buffer = ptr_align (bin_buffer_unaligned, DIGEST_ALIGN);
  intmax_t ଧାଡ଼ି_ସଂଖ୍ୟା;
  ଅକ୍ଷର *ଧାଡ଼ି;
  size_t ଧାଡ଼ି_chars_allocated;
  bool is_stdin = streq (checkfile_name, "-");

  ଯଦି (is_stdin)
    {
      have_read_stdin = true;
      checkfile_name = _("standard input");
      checkfile_stream = stdin;
    }
  ଅନ୍ୟଥା
    {
      checkfile_stream = fopen (checkfile_name, "r");
      ଯଦି (checkfile_stream == ଖାଲି)
        {
          error (0, errno, "%s", quotef (checkfile_name));
          ଫେରନ୍ତୁ false;
        }
    }

  ଧାଡ଼ି_ସଂଖ୍ୟା = 0;
  ଧାଡ଼ି = ଖାଲି;
  ଧାଡ଼ି_chars_allocated = 0;
  କର
    {
      ଅକ୍ଷର *filename;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଦ୍ୱିମିକ;
      unsigned ଅକ୍ଷର *digest;
      ssize_t ଧାଡ଼ି_length;

      ++ଧାଡ଼ି_ସଂଖ୍ୟା;
      ଯଦି (ଧାଡ଼ି_ସଂଖ୍ୟା == 0)
        error (EXIT_FAILURE, 0, _("%s: too many checksum lines"),
               quotef (checkfile_name));

      ଧାଡ଼ି_length = getline (&ଧାଡ଼ି, &ଧାଡ଼ି_chars_allocated, checkfile_stream);
      ଯଦି (ଧାଡ଼ି_length <= 0)
        ଭାଙ୍ଗ;

      /* Ignore comment lines, which begin with a '#' character.  */
      ଯଦି (ଧାଡ଼ି[0] == '#')
        ଜାରି;

      /* Remove any trailing newline.  */
      ଧାଡ଼ି_length -= ଧାଡ଼ି[ଧାଡ଼ି_length - 1] == '\n';
      /* Remove any trailing carriage return.  */
      ଧାଡ଼ି_length -= ଧାଡ଼ି[ଧାଡ଼ି_length - (0 < ଧାଡ଼ି_length)] == '\r';

      /* Ignore empty lines.  */
      ଯଦି (ଧାଡ଼ି_length == 0)
        ଜାରି;

      ଧାଡ଼ି[ଧାଡ଼ି_length] = '\0';

      idx_t d_len;
      ଯଦି (! (split_3 (ଧାଡ଼ି, ଧାଡ଼ି_length, &digest, &d_len, &ଦ୍ୱିମିକ, &filename)
             && ! (is_stdin && streq (filename, "-"))))
        {
          ++n_misformatted_ଧାଡ଼ିଗୁଡ଼ିକ;

          ଯଦି (warn)
            {
              error (0, 0,
                     _("%s: %jd"
                       ": improperly formatted %s checksum line"),
                     quotef (checkfile_name), ଧାଡ଼ି_ସଂଖ୍ୟା,
                     DIGEST_TYPE_STRING);
            }
        }
      ଅନ୍ୟଥା
        {
          bool ok;
          bool missing;

          properly_formatted_ଧାଡ଼ିଗୁଡ଼ିକ = true;

          intmax_t length;
          ok = digest_file (filename, &ଦ୍ୱିମିକ, bin_buffer, &missing, &length);

          ଯଦି (!ok)
            {
              ++n_open_or_read_failures;
              ଯଦି (!status_only)
                printf ("%s: %s\n", quotef (filename),_("FAILED open or read"));
            }
          ଅନ୍ୟଥା ଯଦି (ignore_missing && missing)
            {
              /* Ignore missing files with --ignore-missing.  */
              ;
            }
          ଅନ୍ୟଥା
            {
              bool match = false;
#ଯଦି HASH_ALGO_CKSUM
              ଯଦି (d_len == BASE64_LENGTH (digest_length >> 3))
                match = b64_equal (digest, bin_buffer);
              ଅନ୍ୟଥା
#endif
                ଯଦି (d_len == digest_hex_bytes)
                  match = hex_equal (digest, bin_buffer);

              ଯଦି (match)
                matched_checksums = true;
              ଅନ୍ୟଥା
                ++n_mismatched_checksums;

              ଯଦି (!status_only)
                {
                  ଯଦି (! match || ! quiet)
                    fputs (quotef (filename), stdout);

                  ଯଦି (! match)
                    printf (": %s\n", _("FAILED"));
                  ଅନ୍ୟଥା ଯଦି (!quiet)
                    printf (": %s\n", _("OK"));
                }
            }

          ଯଦି (ferror (stdout))
            write_error ();
        }
    }
  ଯେତେବେଳେ (!feof (checkfile_stream) && !ferror (checkfile_stream));

  ମୁକ୍ତ (ଧାଡ଼ି);

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା err = ferror (checkfile_stream) ? 0 : -1;
  ଯଦି (is_stdin)
    clearerr (checkfile_stream);
  ଅନ୍ୟଥା ଯଦି (fclose (checkfile_stream) != 0 && err < 0)
    err = errno;

  ଯଦି (0 <= err)
    {
      error (0, err, err ? "%s" : _("%s: read error"),
             quotef (checkfile_name));
      ଫେରନ୍ତୁ false;
    }

  ଯଦି (! properly_formatted_ଧାଡ଼ିଗୁଡ଼ିକ)
    {
      /* Warn if no tests are found.  */
      error (0, 0, _("%s: no properly formatted checksum lines found"),
             quotef (checkfile_name));
    }
  ଅନ୍ୟଥା
    {
      ଯଦି (!status_only)
        {
          ଯଦି (n_misformatted_ଧାଡ଼ିଗୁଡ଼ିକ != 0)
            error (0, 0,
                   (ngettext
                    ("WARNING: %jd line is improperly formatted",
                     "WARNING: %jd lines are improperly formatted",
                     select_plural (n_misformatted_ଧାଡ଼ିଗୁଡ଼ିକ))),
                   n_misformatted_ଧାଡ଼ିଗୁଡ଼ିକ);

          ଯଦି (n_open_or_read_failures != 0)
            error (0, 0,
                   (ngettext
                    ("WARNING: %jd listed file could not be read",
                     "WARNING: %jd listed files could not be read",
                     select_plural (n_open_or_read_failures))),
                   n_open_or_read_failures);

          ଯଦି (n_mismatched_checksums != 0)
            error (0, 0,
                   (ngettext
                    ("WARNING: %jd computed checksum did NOT match",
                     "WARNING: %jd computed checksums did NOT match",
                     select_plural (n_mismatched_checksums))),
                   n_mismatched_checksums);

          ଯଦି (ignore_missing && ! matched_checksums)
            error (0, 0, _("%s: no file was verified"),
                   quotef (checkfile_name));
        }
    }

  ଫେରନ୍ତୁ (properly_formatted_ଧାଡ଼ିଗୁଡ଼ିକ
          && matched_checksums
          && n_mismatched_checksums == 0
          && n_open_or_read_failures == 0
          && (!strict || n_misformatted_ଧାଡ଼ିଗୁଡ଼ିକ == 0));
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  unsigned ଅକ୍ଷର bin_buffer_unaligned[DIGEST_BIN_BYTES + DIGEST_ALIGN];
  /* Make sure bin_buffer is properly aligned. */
  unsigned ଅକ୍ଷର *bin_buffer = ptr_align (bin_buffer_unaligned, DIGEST_ALIGN);
  bool do_check = false;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା opt;
  bool ok = true;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଦ୍ୱିମିକ = -1;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା prefix_tag = -1;
  ସଂରଚନା option ଅଚଳ *long_opts = long_options;

#ଯଦି HASH_ALGO_CKSUM
  ଯଦି (cksum_algorithm != crc)
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
  setvbuf (stdout, ଖାଲି, _IOLBF, 0);

#ଯଦି HASH_ALGO_SUM
  ଅକ୍ଷର ଅଚଳ *short_opts = "rs";
#ନଚେତଯଦି HASH_ALGO_CKSUM
  ଅକ୍ଷର ଅଚଳ *short_opts = legacy_mode ? "bctwz" : "a:l:bctwz";
  ଅକ୍ଷର ଅଚଳ *digest_length_str = "";
#ନଚେତଯଦି HASH_ALGO_BLAKE2
  ଅକ୍ଷର ଅଚଳ *short_opts = "l:bctwz";
  ଅକ୍ଷର ଅଚଳ *digest_length_str = "";
#ଅନ୍ୟଥା
  ଅକ୍ଷର ଅଚଳ *short_opts = "bctwz";
#endif

  ଯେତେବେଳେ ((opt = getopt_long (argc, argv, short_opts, long_opts, ଖାଲି))
         != -1)
    ବାଛ (opt)
      {
#ଯଦି HASH_ALGO_CKSUM
      ସ୍ଥିତି 'a':
        cksum_algorithm = XARGMATCH_EXACT ("--algorithm", optarg,
                                           algorithm_args, algorithm_types);
        algorithm_specified = true;
        ଭାଙ୍ଗ;

      ସ୍ଥିତି DEBUG_PROGRAM_OPTION:
        cksum_debug = true;
        ଭାଙ୍ଗ;
#endif
#ଯଦି HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
      ସ୍ଥିତି 'l':
        digest_length = xnumtoimax (optarg, 10, 0, IDX_MAX, "",
                                    _("invalid length"), 0,
                                    XTOINT_MAX_QUIET);
        digest_length_str = optarg;
        ଭାଙ୍ଗ;
#endif
#ଯଦି !HASH_ALGO_SUM
      ସ୍ଥିତି 'c':
        do_check = true;
        ଭାଙ୍ଗ;
      ସ୍ଥିତି STATUS_OPTION:
        status_only = true;
        warn = false;
        quiet = false;
        ଭାଙ୍ଗ;
      ସ୍ଥିତି 'b':
        ଦ୍ୱିମିକ = 1;
        ଭାଙ୍ଗ;
      ସ୍ଥିତି 't':
        ଦ୍ୱିମିକ = 0;
        ଭାଙ୍ଗ;
      ସ୍ଥିତି 'w':
        status_only = false;
        warn = true;
        quiet = false;
        ଭାଙ୍ଗ;
      ସ୍ଥିତି IGNORE_MISSING_OPTION:
        ignore_missing = true;
        ଭାଙ୍ଗ;
      ସ୍ଥିତି QUIET_OPTION:
        status_only = false;
        warn = false;
        quiet = true;
        ଭାଙ୍ଗ;
      ସ୍ଥିତି STRICT_OPTION:
        strict = true;
        ଭାଙ୍ଗ;
# ଯଦି HASH_ALGO_CKSUM
      ସ୍ଥିତି BASE64_OPTION:
        base64_digest = true;
        ଭାଙ୍ଗ;
      ସ୍ଥିତି RAW_OPTION:
        raw_digest = true;
        ଭାଙ୍ଗ;
      ସ୍ଥିତି UNTAG_OPTION:
        prefix_tag = 0;
        ଭାଙ୍ଗ;
# endif
      ସ୍ଥିତି TAG_OPTION:
        prefix_tag = 1;
        ଭାଙ୍ଗ;
      ସ୍ଥିତି 'z':
        digest_delim = '\0';
        ଭାଙ୍ଗ;
#endif
#ଯଦି HASH_ALGO_SUM
      ସ୍ଥିତି 'r':		/* For SysV compatibility. */
        sum_algorithm = bsd;
        ଭାଙ୍ଗ;

      ସ୍ଥିତି 's':
        sum_algorithm = sysv;
        ଭାଙ୍ଗ;
#endif
      case_GETOPT_HELP_CHAR;
      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
      ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        usage (EXIT_FAILURE);
      }

  min_digest_ଧାଡ଼ି_length = MIN_DIGEST_ଧାଡ଼ି_LENGTH;
#ଯଦି HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# ଯଦି HASH_ALGO_CKSUM
  ଯଦି (digest_length && (cksum_algorithm != blake2b
                        && cksum_algorithm != sha2
                        && cksum_algorithm != sha3))
    error (EXIT_FAILURE, 0,
           _("--length is only supported with --algorithm "
             "blake2b, sha2, or sha3"));
  ଯଦି (cksum_algorithm == sha2 || cksum_algorithm == sha3)
    {
      /* Do not require --length with --check.  */
      ଯଦି (digest_length == 0 && *digest_length_str == '\0' && ! do_check)
        error (EXIT_FAILURE, 0, _("--algorithm=%s requires specifying "
                                  "--length 224, 256, 384, or 512"),
               algorithm_args[cksum_algorithm]);
      /* If --check and --length are used we verify the digest length.  */
      ଯଦି ((! do_check || *digest_length_str != '\0')
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
  ଅନ୍ୟଥା
    {
      /* If the digest length checks for SHA-3 are satisfied, the less strict
         checks for BLAKE2 will also be.  */
# ଅନ୍ୟଥା
    {
# endif
      ଯଦି (digest_length > DIGEST_MAX_LEN * 8)
        {
          error (0, 0, _("invalid length: %s"), quote (digest_length_str));
          error (EXIT_FAILURE, 0,
                 _("maximum digest length for %s is %d bits"),
                 quote (DIGEST_TYPE_STRING),
                 DIGEST_MAX_LEN * 8);
        }
      ଯଦି (digest_length % 8 != 0)
        {
          error (0, 0, _("invalid length: %s"), quote (digest_length_str));
          error (EXIT_FAILURE, 0, _("length is not a multiple of 8"));
        }
    }
  ଯଦି (digest_length == 0)
    {
# ଯଦି HASH_ALGO_BLAKE2
      digest_length = DIGEST_MAX_LEN * 8;
# ଅନ୍ୟଥା
      digest_length = algorithm_bits[cksum_algorithm];
# endif
    }
  digest_hex_bytes = digest_length >> 2;
#ଅନ୍ୟଥା
  digest_hex_bytes = DIGEST_HEX_BYTES;
#endif

#ଯଦି HASH_ALGO_CKSUM
  ବାଛ (+cksum_algorithm)
    {
    ସ୍ଥିତି bsd:
    ସ୍ଥିତି sysv:
    ସ୍ଥିତି crc:
    ସ୍ଥିତି crc32b:
        ଯଦି (do_check && algorithm_specified)
          error (EXIT_FAILURE, 0,
                 _("--check is not supported with "
                   "--algorithm={bsd,sysv,crc,crc32b}"));
        ଭାଙ୍ଗ;
    }

  ଯଦି (base64_digest && raw_digest)
   {
     error (0, 0, _("--base64 and --raw are mutually exclusive"));
     usage (EXIT_FAILURE);
   }
#endif

  ଯଦି (digest_delim != '\n' && do_check)
    {
      error (0, 0, _("the --zero option is not supported when "
                     "verifying checksums"));
      usage (EXIT_FAILURE);
    }
  ଯଦି (1 <= prefix_tag && do_check)
    {
      /* Note we allow --untagged with --check to more
         seamlessly support --untagged in an emulation wrapper.  */
      error (0, 0, _("the --tag option is meaningless when "
                     "verifying checksums"));
      usage (EXIT_FAILURE);
    }

  ଯଦି (0 <= ଦ୍ୱିମିକ && do_check)
    {
      error (0, 0, _("the --binary and --text options are meaningless when "
                     "verifying checksums"));
      usage (EXIT_FAILURE);
    }

  ଯଦି (ignore_missing && !do_check)
    {
      error (0, 0,
             _("the --ignore-missing option is meaningful only when "
               "verifying checksums"));
      usage (EXIT_FAILURE);
    }

  ଯଦି (status_only && !do_check)
    {
      error (0, 0,
       _("the --status option is meaningful only when verifying checksums"));
      usage (EXIT_FAILURE);
    }

  ଯଦି (warn && !do_check)
    {
      error (0, 0,
       _("the --warn option is meaningful only when verifying checksums"));
      usage (EXIT_FAILURE);
    }

  ଯଦି (quiet && !do_check)
    {
      error (0, 0,
       _("the --quiet option is meaningful only when verifying checksums"));
      usage (EXIT_FAILURE);
    }

  ଯଦି (strict & !do_check)
   {
     error (0, 0,
        _("the --strict option is meaningful only when verifying checksums"));
     usage (EXIT_FAILURE);
   }

  ଯଦି (prefix_tag == -1)
    prefix_tag = HASH_ALGO_CKSUM;

  ଯଦି (prefix_tag && !ଦ୍ୱିମିକ)
   {
     /* This could be supported in a backwards compatible way
        by prefixing the output line with a space in text mode.
        However that's invasive enough that it was agreed to
        not support this mode with --tag, as --text use cases
        are adequately supported by the default output format.  */
#ଯଦି !HASH_ALGO_CKSUM
     error (0, 0, _("--tag does not support --text mode"));
#ଅନ୍ୟଥା
     error (0, 0, _("--text mode is only supported with --untagged"));
#endif
     usage (EXIT_FAILURE);
   }

  ଯଦି (!O_ଦ୍ୱିମିକ && ଦ୍ୱିମିକ < 0)
    ଦ୍ୱିମିକ = 0;
  ଅନ୍ୟଥା ଯଦି (prefix_tag)
    ଦ୍ୱିମିକ = 1;

  ଅକ୍ଷର **operand_lim = argv + argc;
  ଯଦି (optind == argc)
    *operand_lim++ = bad_cast ("-");
  ଅନ୍ୟଥା ଯଦି (1 < argc - optind && raw_digest)
    error (EXIT_FAILURE, 0,
           _("the --raw option is not supported with multiple files"));

  ପାଇଁ (ଅକ୍ଷର **operandp = argv + optind; operandp < operand_lim; operandp++)
    {
      ଅକ୍ଷର *file = *operandp;
      ଯଦି (do_check)
        ok &= digest_check (file);
      ଅନ୍ୟଥା
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଦ୍ୱିମିକ_file = ଦ୍ୱିମିକ;
          bool missing;
          intmax_t length;

          ଯଦି (! digest_file (file, &ଦ୍ୱିମିକ_file, bin_buffer, &missing, &length))
            ok = false;
          ଅନ୍ୟଥା
            {
              DIGEST_OUT (file, ଦ୍ୱିମିକ_file, bin_buffer, raw_digest, prefix_tag,
                          digest_delim, optind != argc, length);
              ଯଦି (ferror (stdout))
                write_error ();
            }
        }
    }

  ଯଦି (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, _("standard input"));

  ଫେରନ୍ତୁ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
