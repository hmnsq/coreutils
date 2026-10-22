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

#অন্তর্ভাৱ <config.h>

#অন্তর্ভাৱ <getopt.h>
#অন্তর্ভাৱ <sys/types.h>

#অন্তর্ভাৱ "assure.h"
#অন্তর্ভাৱ "system.h"
#অন্তর্ভাৱ "argmatch.h"
#অন্তর্ভাৱ "c-ctype.h"
#অন্তর্ভাৱ "quote.h"
#অন্তর্ভাৱ "xdectoint.h"
#অন্তর্ভাৱ "xstrtol.h"

#ifndef HASH_ALGO_CKSUM
# define HASH_ALGO_CKSUM 0
#endif

#যদি HASH_ALGO_SUM || HASH_ALGO_CKSUM
# include "sum.h"
#endif
#যদি HASH_ALGO_CKSUM
# include "cksum.h"
# include "cksum_crc.h"
# include "base64.h"
#endif
#যদি HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# include "blake2/b2sum.h"
#endif
#যদি HASH_ALGO_MD5 || HASH_ALGO_CKSUM
# include "md5.h"
#endif
#যদি HASH_ALGO_SHA1 || HASH_ALGO_CKSUM
# include "sha1.h"
#endif
#যদি HASH_ALGO_SHA256 || HASH_ALGO_SHA224 || HASH_ALGO_CKSUM
# include "sha256.h"
#endif
#যদি HASH_ALGO_SHA512 || HASH_ALGO_SHA384 || HASH_ALGO_CKSUM
# include "sha512.h"
#endif
#যদি HASH_ALGO_CKSUM
# include "sha3.h"
#endif
#যদি HASH_ALGO_CKSUM
# include "sm3.h"
#endif
#অন্তর্ভাৱ "fadvise.h"
#অন্তর্ভাৱ "stdio--.h"
#অন্তর্ভাৱ "xbinary-io.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#যদি HASH_ALGO_SUM
# define PROGRAM_NAME "sum"
# define DIGEST_TYPE_STRING "BSD"
# define DIGEST_STREAM sumfns[sum_algorithm]
# define DIGEST_OUT sum_output_fns[sum_algorithm]
# define DIGEST_BITS 16
# define DIGEST_ALIGN 4
#নোচেদ্যদি HASH_ALGO_CKSUM
# define MAX_DIGEST_BITS 512
# define MAX_DIGEST_ALIGN 8
# define DIGEST_TYPE_STRING algorithm_tags[cksum_algorithm]
# define DIGEST_STREAM cksumfns[cksum_algorithm]
# define DIGEST_OUT cksum_output_fns[cksum_algorithm]
# define DIGEST_BITS MAX_DIGEST_BITS
# define DIGEST_ALIGN MAX_DIGEST_ALIGN
#নোচেদ্যদি HASH_ALGO_MD5
# define PROGRAM_NAME "md5sum"
# define DIGEST_TYPE_STRING "MD5"
# define DIGEST_STREAM md5_stream
# define DIGEST_BITS 128
# define DIGEST_REFERENCE "RFC 1321"
# define DIGEST_ALIGN 4
#নোচেদ্যদি HASH_ALGO_BLAKE2
# define PROGRAM_NAME "b2sum"
# define DIGEST_TYPE_STRING "BLAKE2b"
# define DIGEST_STREAM blake2b_stream
# define DIGEST_BITS 512
# define DIGEST_REFERENCE "RFC 7693"
# define DIGEST_ALIGN 8
#নোচেদ্যদি HASH_ALGO_SHA1
# define PROGRAM_NAME "sha1sum"
# define DIGEST_TYPE_STRING "SHA1"
# define DIGEST_STREAM sha1_stream
# define DIGEST_BITS 160
# define DIGEST_REFERENCE "FIPS-180-1"
# define DIGEST_ALIGN 4
#নোচেদ্যদি HASH_ALGO_SHA256
# define PROGRAM_NAME "sha256sum"
# define DIGEST_TYPE_STRING "SHA256"
# define DIGEST_STREAM sha256_stream
# define DIGEST_BITS 256
# define DIGEST_REFERENCE "FIPS-180-2"
# define DIGEST_ALIGN 4
#নোচেদ্যদি HASH_ALGO_SHA224
# define PROGRAM_NAME "sha224sum"
# define DIGEST_TYPE_STRING "SHA224"
# define DIGEST_STREAM sha224_stream
# define DIGEST_BITS 224
# define DIGEST_REFERENCE "RFC 3874"
# define DIGEST_ALIGN 4
#নোচেদ্যদি HASH_ALGO_SHA512
# define PROGRAM_NAME "sha512sum"
# define DIGEST_TYPE_STRING "SHA512"
# define DIGEST_STREAM sha512_stream
# define DIGEST_BITS 512
# define DIGEST_REFERENCE "FIPS-180-2"
# define DIGEST_ALIGN 8
#নোচেদ্যদি HASH_ALGO_SHA384
# define PROGRAM_NAME "sha384sum"
# define DIGEST_TYPE_STRING "SHA384"
# define DIGEST_STREAM sha384_stream
# define DIGEST_BITS 384
# define DIGEST_REFERENCE "FIPS-180-2"
# define DIGEST_ALIGN 8
#অন্যথা
# error "Can't decide which hash algorithm to compile."
#endif
#যদি !HASH_ALGO_SUM && !HASH_ALGO_CKSUM
# define DIGEST_OUT output_file
#endif

#যদি HASH_ALGO_CKSUM
# define PROGRAM_NAME (!legacy_mode ? "cksum"			 \
                       : cksum_algorithm == md5 ? "md5sum"	 \
                       : cksum_algorithm == sha1 ? "sha1sum"	 \
                       : cksum_algorithm == sha224 ? "sha224sum" \
                       : cksum_algorithm == sha256 ? "sha256sum" \
                       : cksum_algorithm == sha384 ? "sha384sum" \
                       : cksum_algorithm == sha512 ? "sha512sum" \
                       : "cksum")
#endif

#যদি HASH_ALGO_SUM
# define AUTHORS \
  proper_name ("Kayvan Aghaiepour"), \
  proper_name ("David MacKenzie")
#নোচেদ্যদি HASH_ALGO_CKSUM
# define AUTHORS \
  proper_name_lite ("Padraig Brady", "P\303\241draig Brady"), \
  proper_name ("Q. Frank Xia")
#নোচেদ্যদি HASH_ALGO_BLAKE2
# define AUTHORS \
  proper_name_lite ("Padraig Brady", "P\303\241draig Brady"), \
  proper_name ("Samuel Neves")
#অন্যথা
# define AUTHORS \
  proper_name ("Ulrich Drepper"), \
  proper_name ("Scott Miller"), \
  proper_name ("David Madore")
#endif
#যদি !HASH_ALGO_BLAKE2 && !HASH_ALGO_CKSUM
# define DIGEST_HEX_BYTES (DIGEST_BITS / 4)
#endif
#define DIGEST_BIN_BYTES (DIGEST_BITS / 8)

/* The minimum length of a valid digest line.  This length does
   not include any newline character at the end of a line.  */
#যদি HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# define MIN_DIGEST_সাৰি_LENGTH 3 /* With -l 8.  */
#অন্যথা
# define MIN_DIGEST_সাৰি_LENGTH \
   (DIGEST_HEX_BYTES /* length of hexadecimal message digest */ \
    + 1 /* blank */ \
    + 1 /* minimum filename length */ )
#endif

#যদি !HASH_ALGO_SUM
স্থির শূন্য
output_file (ৱর্ণ ধ্রুৱ *file, পূর্ণাঙ্ক দ্বিমিক_file, শূন্য ধ্রুৱ *digest,
             bool raw, bool tagged, unsigned ৱর্ণ delim, bool args,
             intmax_t length);
#endif

/* True if any of the files read were the standard input. */
স্থির bool have_read_stdin;

/* The minimum length of a valid checksum line for the selected algorithm.  */
স্থির idx_t min_digest_সাৰি_length;

/* Set to the length of a digest hex string for the selected algorithm.  */
স্থির idx_t digest_hex_bytes;

/* With --check, don't generate any output.
   The exit code indicates success or failure.  */
স্থির bool status_only = false;

/* With --check, print a message to standard error warning about each
   improperly formatted checksum line.  */
স্থির bool warn = false;

/* With --check, ignore missing files.  */
স্থির bool ignore_missing = false;

/* With --check, suppress the "OK" printed for each verified file.  */
স্থির bool quiet = false;

/* With --check, exit with a non-zero return code if any line is
   improperly formatted. */
স্থির bool strict = false;

/* Whether a BSD reversed format checksum is detected.  */
স্থির পূর্ণাঙ্ক bsd_reversed = -1;

/* line delimiter.  */
স্থির unsigned ৱর্ণ digest_delim = '\n';

#যদি HASH_ALGO_CKSUM
/* If true, print base64-encoded digests, not hex.  */
স্থির bool base64_digest = false;
#endif

/* If true, print binary digests, not hex.  */
স্থির bool raw_digest = false;

/* blake2b and sha3 allow the -l option.  Luckily they both have the same
   maximum digest size.  */
#যদি HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# যদি HASH_ALGO_CKSUM
static_assert (BLAKE2B_OUTBYTES == SHA3_512_DIGEST_SIZE);
# endif
# define DIGEST_MAX_LEN BLAKE2B_OUTBYTES
স্থির idx_t digest_length;
#endif /* HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM */

প্রকারনির্দেশ শূন্য (*digest_output_fn)(ৱর্ণ ধ্রুৱ *, পূর্ণাঙ্ক, শূন্য ধ্রুৱ *, bool,
                                 bool, unsigned ৱর্ণ, bool, intmax_t);
#যদি HASH_ALGO_SUM
গণনা Algorithm
{
  bsd,
  sysv,
};

স্থির গণনা Algorithm sum_algorithm;
স্থির sumfn sumfns[]=
{
  bsd_sum_stream,
  sysv_sum_stream,
};
স্থির digest_output_fn sum_output_fns[]=
{
  output_bsd,
  output_sysv,
};
#endif

#যদি HASH_ALGO_CKSUM
স্থির পূর্ণাঙ্ক
md5_sum_stream (FILE *stream, শূন্য *resstream, MAYBE_UNUSED intmax_t *length)
{
  ঘূৰাই md5_stream (stream, resstream);
}
স্থির পূর্ণাঙ্ক
sha1_sum_stream (FILE *stream, শূন্য *resstream, MAYBE_UNUSED intmax_t *length)
{
  ঘূৰাই sha1_stream (stream, resstream);
}
স্থির পূর্ণাঙ্ক
sha224_sum_stream (FILE *stream, শূন্য *resstream,
                   MAYBE_UNUSED intmax_t *length)
{
  ঘূৰাই sha224_stream (stream, resstream);
}
স্থির পূর্ণাঙ্ক
sha256_sum_stream (FILE *stream, শূন্য *resstream,
                   MAYBE_UNUSED intmax_t *length)
{
  ঘূৰাই sha256_stream (stream, resstream);
}
স্থির পূর্ণাঙ্ক
sha384_sum_stream (FILE *stream, শূন্য *resstream,
                   MAYBE_UNUSED intmax_t *length)
{
  ঘূৰাই sha384_stream (stream, resstream);
}
স্থির পূর্ণাঙ্ক
sha512_sum_stream (FILE *stream, শূন্য *resstream,
                   MAYBE_UNUSED intmax_t *length)
{
  ঘূৰাই sha512_stream (stream, resstream);
}
স্থির পূর্ণাঙ্ক
sha2_sum_stream (FILE *stream, শূন্য *resstream, intmax_t *length)
{
  ৱিকল্প (*length)
    {
    পক্ষ SHA224_DIGEST_SIZE:
      ঘূৰাই sha224_stream (stream, resstream);
    পক্ষ SHA256_DIGEST_SIZE:
      ঘূৰাই sha256_stream (stream, resstream);
    পক্ষ SHA384_DIGEST_SIZE:
      ঘূৰাই sha384_stream (stream, resstream);
    পক্ষ SHA512_DIGEST_SIZE:
      ঘূৰাই sha512_stream (stream, resstream);
    স্ৱভাৱ:
      affirm (false);
    }
}
স্থির পূর্ণাঙ্ক
sha3_sum_stream (FILE *stream, শূন্য *resstream, intmax_t *length)
{
  ৱিকল্প (*length)
    {
    পক্ষ SHA3_224_DIGEST_SIZE:
      ঘূৰাই sha3_224_stream (stream, resstream);
    পক্ষ SHA3_256_DIGEST_SIZE:
      ঘূৰাই sha3_256_stream (stream, resstream);
    পক্ষ SHA3_384_DIGEST_SIZE:
      ঘূৰাই sha3_384_stream (stream, resstream);
    পক্ষ SHA3_512_DIGEST_SIZE:
      ঘূৰাই sha3_512_stream (stream, resstream);
    স্ৱভাৱ:
      affirm (false);
    }
}
স্থির পূর্ণাঙ্ক
blake2b_sum_stream (FILE *stream, শূন্য *resstream, intmax_t *length)
{
  ঘূৰাই blake2b_stream (stream, resstream, *length);
}
স্থির পূর্ণাঙ্ক
sm3_sum_stream (FILE *stream, শূন্য *resstream, MAYBE_UNUSED intmax_t *length)
{
  ঘূৰাই sm3_stream (stream, resstream);
}

স্থির ৱর্ণ ধ্রুৱ *ধ্রুৱ algorithm_args[] =
{
  "bsd", "sysv", "crc", "crc32b", "md5", "sha1",
  "sha224", "sha256", "sha384", "sha512", /* Legacy naming */
  "sha2", "sha3", "blake2b", "sm3", রিক্ত
};
স্থির গণনা Algorithm ধ্রুৱ algorithm_types[] =
{
  bsd, sysv, crc, crc32b, md5, sha1,
  sha224, sha256, sha384, sha512,
  sha2, sha3, blake2b, sm3,
};
ARGMATCH_VERIFY (algorithm_args, algorithm_types);

স্থির ৱর্ণ ধ্রুৱ *ধ্রুৱ algorithm_tags[] =
{
  "BSD", "SYSV", "CRC", "CRC32B", "MD5", "SHA1",
  "SHA224", "SHA256", "SHA384", "SHA512",
  "SHA2", "SHA3", "BLAKE2b", "SM3", রিক্ত
};
স্থির পূর্ণাঙ্ক ধ্রুৱ algorithm_bits[] =
{
  16, 16, 32, 32, 128, 160,
  224, 256, 384, 512,
  512, 512, 512, 256, 0
};

static_assert (countof (algorithm_bits) == countof (algorithm_args));

স্থির bool algorithm_specified = false;
স্থির bool legacy_mode = false;
গণনা Algorithm cksum_algorithm = crc;

স্থির sumfn cksumfns[]=
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
স্থির digest_output_fn cksum_output_fns[]=
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

গণনা
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

#যদি HASH_ALGO_CKSUM
স্থির সংরচনা option ধ্রুৱ legacy_long_options[] =
{
  { "check", no_argument, রিক্ত, 'c' },
  { "ignore-missing", no_argument, রিক্ত, IGNORE_MISSING_OPTION },
  { "quiet", no_argument, রিক্ত, QUIET_OPTION },
  { "status", no_argument, রিক্ত, STATUS_OPTION },
  { "warn", no_argument, রিক্ত, 'w' },
  { "strict", no_argument, রিক্ত, STRICT_OPTION },
  { "tag", no_argument, রিক্ত, TAG_OPTION },
  { "zero", no_argument, রিক্ত, 'z' },

  { "binary", no_argument, রিক্ত, 'b' }, /* Deprecated.  */
  { "text", no_argument, রিক্ত, 't' }, /* Deprecated.  */

  { GETOPT_HELP_OPTION_DECL },
  { GETOPT_VERSION_OPTION_DECL },
  { রিক্ত, 0, রিক্ত, 0 }
};
#endif

স্থির সংরচনা option ধ্রুৱ long_options[] =
{
#যদি HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
  { "length", required_argument, রিক্ত, 'l'},
#endif

#যদি !HASH_ALGO_SUM
  { "check", no_argument, রিক্ত, 'c' },
  { "ignore-missing", no_argument, রিক্ত, IGNORE_MISSING_OPTION},
  { "quiet", no_argument, রিক্ত, QUIET_OPTION },
  { "status", no_argument, রিক্ত, STATUS_OPTION },
  { "warn", no_argument, রিক্ত, 'w' },
  { "strict", no_argument, রিক্ত, STRICT_OPTION },
  { "tag", no_argument, রিক্ত, TAG_OPTION },
  { "zero", no_argument, রিক্ত, 'z' },

# যদি HASH_ALGO_CKSUM
  { "algorithm", required_argument, রিক্ত, 'a'},
  { "base64", no_argument, রিক্ত, BASE64_OPTION },
  { "debug", no_argument, রিক্ত, DEBUG_PROGRAM_OPTION},
  { "raw", no_argument, রিক্ত, RAW_OPTION},
  { "untagged", no_argument, রিক্ত, UNTAG_OPTION },
# endif
  { "binary", no_argument, রিক্ত, 'b' }, /* Deprecated.  */
  { "text", no_argument, রিক্ত, 't' }, /* Deprecated.  */

#অন্যথা
  {"sysv", no_argument, রিক্ত, 's'},  /* Not in cksum.  */
#endif

  { GETOPT_HELP_OPTION_DECL },
  { GETOPT_VERSION_OPTION_DECL },
  { রিক্ত, 0, রিক্ত, 0 }
};

শূন্য
usage (পূর্ণাঙ্ক status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"), program_name);
#যদি HASH_ALGO_CKSUM
    যদি (legacy_mode)
      {
        printf (_("\
Print or check %s checksums.\n\
"), DIGEST_TYPE_STRING);
        fputs (_("\
Legacy interface to the cksum(1) utility.\n\
"), stdout);
      }
    অন্যথা
      fputs (_("\
Print or verify checksums.\n\
By স্ৱভাৱ use the 32 bit CRC algorithm.\n\
"), stdout);
#অন্যথা
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
#যদি HASH_ALGO_SUM
      oputs (_("\
  -r\n\
         use BSD sum algorithm (the স্ৱভাৱ), use 1K blocks\n\
"));
      oputs (_("\
  -s, --sysv\n\
         use System V sum algorithm, use 512 bytes blocks\n\
"));
#endif
#যদি HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
        emit_mandatory_arg_note ();
#endif
#যদি HASH_ALGO_CKSUM
     যদি (! legacy_mode)
        oputs (_("\
  -a, --algorithm=TYPE\n\
         select the digest type to use.  See DIGEST below\n\
"));
     যদি (! legacy_mode)
        oputs (_("\
      --base64\n\
         emit base64-encoded digests, not hexadecimal\n\
"));
#endif
#যদি !HASH_ALGO_SUM
# যদি HASH_ALGO_CKSUM
     যদি (legacy_mode)
       {
# endif
      যদি (O_দ্বিমিক)
        oputs (_("\
  -b, --দ্বিমিক\n\
         পঠ in দ্বিমিক mode (স্ৱভাৱ unless reading tty stdin)\n\
"));
      অন্যথা
        oputs (_("\
  -b, --দ্বিমিক\n\
         পঠ in দ্বিমিক mode\n\
"));
# যদি HASH_ALGO_CKSUM
       }
# endif
        oputs (_("\
  -c, --check\n\
         পঠ checksums from the FILEs and check them\n\
"));
# যদি HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
#  যদি HASH_ALGO_CKSUM
     যদি (! legacy_mode)
#  endif
        oputs (_("\
  -l, --length=BITS\n\
         digest length in bits; must not exceed the max size\n\
         and must be a multiple of 8 বাবে blake2b;\n\
         must be 224, 256, 384, or 512 বাবে sha2 or sha3\n\
"));
# endif
# যদি HASH_ALGO_CKSUM
    যদি (! legacy_mode)
        oputs (_("\
      --raw\n\
         emit a raw দ্বিমিক digest, not hexadecimal\n\
"));
    যদি (legacy_mode)
      oputs (_("\
      --tag\n\
         create a BSD-style checksum\n\
"));
    অন্যথা
      oputs (_("\
      --tag\n\
         create a BSD-style checksum (the স্ৱভাৱ)\n\
"));
    যদি (! legacy_mode)
      oputs (_("\
      --untagged\n\
         create a reversed style checksum, without digest type\n\
"));
# অন্যথা
      oputs (_("\
      --tag\n\
         create a BSD-style checksum\n\
"));
# endif
# যদি HASH_ALGO_CKSUM
    যদি (legacy_mode)
      {
# endif
      যদি (O_দ্বিমিক)
        oputs (_("\
  -t, --text\n\
         পঠ in text mode (স্ৱভাৱ যদি reading tty stdin)\n\
"));
      অন্যথা
        oputs (_("\
  -t, --text\n\
         পঠ in text mode (স্ৱভাৱ)\n\
"));
# যদি HASH_ALGO_CKSUM
      }
# endif
      oputs (_("\
  -z, --zero\n\
         end each output সাৰি with NUL, not newline,\n\
         and disable file name escaping\n\
"));
      fputs (_("\
\n\
The following five options are useful only when verifying checksums:\n\
"), stdout);
      oputs (_("\
      --ignore-missing\n\
         don't fail or report status বাবে missing files\n\
"));
      oputs (_("\
      --quiet\n\
         don't print OK বাবে each successfully verified file\n\
"));
      oputs (_("\
      --status\n\
         don't output anything, status code shows success\n\
"));
      oputs (_("\
      --strict\n\
         exit non-zero বাবে improperly formatted checksum সাৰিগুলি\n\
"));
      oputs (_("\
  -w, --warn\n\
         warn about improperly formatted checksum সাৰিগুলি\n\
"));
#endif
#যদি HASH_ALGO_CKSUM
    যদি (! legacy_mode)
      oputs (_("\
      --debug\n\
         indicate which implementation used\n\
"));
#endif
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
#যদি HASH_ALGO_CKSUM
    যদি (! legacy_mode)
      fputs (_("\
\n\
DIGEST determines the digest algorithm and স্ৱভাৱ output format:\n\
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
#যদি !HASH_ALGO_SUM && !HASH_ALGO_CKSUM
      printf (_("\
\n\
The sums are computed as described in %s.\n"), DIGEST_REFERENCE);
      fputs (_("\
When checking, the প্ৰবেশ should be a former output of this program.\n\
The স্ৱভাৱ mode is to print a সাৰি with: checksum, a space,\n\
a character indicating প্ৰবেশ mode ('*' বাবে দ্বিমিক, ' ' বাবে text\n\
or where দ্বিমিক is insignificant), and name বাবে each FILE.\n\
\n\
There is no difference between দ্বিমিক mode and text mode on GNU systems.\
\n"), stdout);
#endif
#যদি HASH_ALGO_CKSUM
      fputs (_("\
When checking, the প্ৰবেশ should be a former output of this program,\n\
or equivalent standalone program.\
\n"), stdout);
#endif
      emit_ancillary_info (PROGRAM_NAME);
    }

  exit (status);
}

#যদি !HASH_ALGO_SUM
/* Given a string S, return TRUE if it contains problematic characters
   that need escaping.  Note we escape '\' itself to provide some forward
   compat to introduce escaping of other characters.  */

ATTRIBUTE_PURE
স্থির bool
problematic_chars (ৱর্ণ ধ্রুৱ *s)
{
  idx_t length = strcspn (s, "\\\n\r");
  ঘূৰাই s[length] != '\0';
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

স্থির ৱর্ণ *
filename_unescape (ৱর্ণ *s, idx_t s_len)
{
  ৱর্ণ *dst = s;

  বাবে (idx_t i = 0; i < s_len; i++)
    {
      ৱিকল্প (s[i])
        {
        পক্ষ '\\':
          যদি (i == s_len - 1)
            {
              /* File name ends with an unescaped backslash: invalid.  */
              ঘূৰাই রিক্ত;
            }
          ++i;
          ৱিকল্প (s[i])
            {
            পক্ষ 'n':
              *dst++ = '\n';
              ভঙ্গ;
            পক্ষ 'r':
              *dst++ = '\r';
              ভঙ্গ;
            পক্ষ '\\':
              *dst++ = '\\';
              ভঙ্গ;
            স্ৱভাৱ:
              /* Only '\', 'n' or 'r' may follow a backslash.  */
              ঘূৰাই রিক্ত;
            }
          ভঙ্গ;

        পক্ষ '\0':
          /* The file name may not contain a NUL.  */
          ঘূৰাই রিক্ত;

        স্ৱভাৱ:
          *dst++ = s[i];
          ভঙ্গ;
        }
    }
  যদি (dst < s + s_len)
    *dst = '\0';

  ঘূৰাই s;
}

/* Return true if S is a LEN-byte NUL-terminated string of hex or base64
   digits and has the expected length.  Otherwise, return false.  */
ATTRIBUTE_PURE
স্থির bool
valid_digits (unsigned ৱর্ণ ধ্রুৱ *s, idx_t len)
{
#যদি HASH_ALGO_CKSUM
  যদি (len == BASE64_LENGTH (digest_length >> 3))
    {
      idx_t i;
      বাবে (i = 0; i < len - digest_length % 3; i++)
        {
          যদি (!isbase64 (*s))
            ঘূৰাই false;
          ++s;
        }
      বাবে ( ; i < len; i++)
        {
          যদি (*s != '=')
            ঘূৰাই false;
          ++s;
        }
    }
  অন্যথা
#endif
  যদি (len == digest_hex_bytes)
    {
      বাবে (idx_t i = 0; i < digest_hex_bytes; i++)
        {
          যদি (!c_isxdigit (*s))
            ঘূৰাই false;
          ++s;
        }
    }
  অন্যথা
    ঘূৰাই false;

  ঘূৰাই *s == '\0';
}

/* Split the checksum string S (of length S_LEN) from a BSD 'md5' or
   'sha1' command into two parts: a hexadecimal digest, and the file
   name.  S is modified.  Set *D_LEN to the length of the digest string.
   Return true if successful.  */

স্থির bool
bsd_split_3 (ৱর্ণ *s, idx_t s_len,
             unsigned ৱর্ণ **digest, idx_t *d_len,
             ৱর্ণ **file_name, bool escaped_filename)
{
  যদি (s_len == 0)
    ঘূৰাই false;

  /* Find end of filename.  */
  idx_t i = s_len - 1;
  যেতিয়ালৈ (i && s[i] != ')')
    i--;

  যদি (s[i] != ')')
    ঘূৰাই false;

  *file_name = s;

  যদি (escaped_filename && filename_unescape (s, i) == রিক্ত)
    ঘূৰাই false;

  s[i++] = '\0';

  যেতিয়ালৈ (c_isblank (s[i]))
    i++;

  যদি (s[i] != '=')
    ঘূৰাই false;

  i++;

  যেতিয়ালৈ (c_isblank (s[i]))
    i++;

  *digest = (unsigned ৱর্ণ *) &s[i];

  *d_len = s_len - i;
  ঘূৰাই valid_digits (*digest, *d_len);
}

#যদি HASH_ALGO_CKSUM
/* Return the corresponding Algorithm for the string S,
   or -1 for no match.  */

স্থির ptrdiff_t
algorithm_from_tag (ৱর্ণ *s)
{
  /* Limit check size to this length for perf reasons.  */
  স্থির idx_t max_tag_len;
  যদি (! max_tag_len)
    {
      ৱর্ণ ধ্রুৱ * ধ্রুৱ * tag = algorithm_tags;
      যেতিয়ালৈ (*tag)
        {
          idx_t tag_len = strlen (*tag++);
          max_tag_len = MAX (tag_len, max_tag_len);
        }
    }

  idx_t i = 0;

  /* Find end of tag */
  যেতিয়ালৈ (i <= max_tag_len && s[i] && ! c_isblank (s[i])
         && s[i] != '-' && s[i] != '(')
    ++i;

  যদি (i > max_tag_len)
    ঘূৰাই -1;

  /* Terminate tag, and lookup.  */
  ৱর্ণ sep = s[i];
  s[i] = '\0';
  ptrdiff_t algo = argmatch_exact (s, algorithm_tags);
  s[i] = sep;

  ঘূৰাই algo;
}
#endif

/* Split the string S (of length S_LEN) into three parts:
   a hexadecimal digest, binary flag, and the file name.
   S is modified.  Set *D_LEN to the length of the digest string.
   Return true if successful.  */

স্থির bool
split_3 (ৱর্ণ *s, idx_t s_len,
         unsigned ৱর্ণ **digest, idx_t *d_len, পূর্ণাঙ্ক *দ্বিমিক, ৱর্ণ **file_name)
{
  bool escaped_filename = false;
  idx_t algo_name_len;

  idx_t i = 0;
  যেতিয়ালৈ (c_isblank (s[i]))
    ++i;

  যদি (s[i] == '\\')
    {
      ++i;
      escaped_filename = true;
    }

  /* Check for BSD-style checksum line. */

#যদি HASH_ALGO_CKSUM
  যদি (! algorithm_specified || cksum_algorithm == sha2)
    {
      ptrdiff_t algo_tag = algorithm_from_tag (s + i);
      যদি (! algorithm_specified)
        {
          যদি (algo_tag >= 0)
            {
              যদি (algo_tag <= crc32b)
                ঘূৰাই false;  /* We don't support checking these formats.  */
              cksum_algorithm = algo_tag;
            }
          অন্যথা
            ঘূৰাই false;      /* We only support tagged format without -a.  */
        }
      অন্যথা
        {
          যদি (cksum_algorithm == sha2 && (algo_tag == sha2
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
  যদি (STREQ_LEN (s + i, DIGEST_TYPE_STRING, algo_name_len))
    {
      i += algo_name_len;
#যদি HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM

# যদি HASH_ALGO_BLAKE2
      digest_length = DIGEST_MAX_LEN * 8;
# অন্যথা
      digest_length = algorithm_bits[cksum_algorithm];
# endif
      যদি (s[i] == '-')  /* length specified. Not base64 */
        {
          ++i;
          intmax_t length;
          ৱর্ণ *siend;
          যদি (!c_isdigit (s[i])
              || xstrtoimax (s + i, &siend, 0, &length, রিক্ত) != LONGINT_OK)
            ঘূৰাই false;
# যদি HASH_ALGO_CKSUM
          অন্যথা যদি (cksum_algorithm == sha2 || cksum_algorithm == sha3)
            {
              যদি (length != SHA224_DIGEST_SIZE * 8
                  && length != SHA256_DIGEST_SIZE * 8
                  && length != SHA384_DIGEST_SIZE * 8
                  && length != SHA512_DIGEST_SIZE * 8)
                ঘূৰাই false;
            }
# endif
          অন্যথা যদি (!(0 < length && length <= digest_length && length % 8 == 0))
            ঘূৰাই false;

          i = siend - s;
          digest_length = length;
        }
      digest_hex_bytes = digest_length >> 2;
#endif
      যদি (s[i] == ' ')
        ++i;
      যদি (s[i] == '(')  /* not base64 */
        {
          ++i;
          *দ্বিমিক = 0;
          ঘূৰাই bsd_split_3 (s + i, s_len - i,
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
  যদি (s_len - i < min_digest_সাৰি_length + (s[i] == '\\'))
    ঘূৰাই false;

  *digest = (unsigned ৱর্ণ *) &s[i];

#যদি HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
  /* Auto determine length.  */
# যদি HASH_ALGO_CKSUM
  যদি (cksum_algorithm == blake2b
      || cksum_algorithm == sha2 || cksum_algorithm == sha3) {
# endif
  unsigned ৱর্ণ ধ্রুৱ *hp = *digest;
  digest_hex_bytes = 0;
  বাবে (; c_isxdigit (*hp); ++hp, ++digest_hex_bytes)
    ;
# যদি HASH_ALGO_CKSUM
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
  বাবে (; isubase64 (*hp); ++hp, ++digest_base64_bytes)
    ;
  বাবে (; *hp == '='; ++hp, ++trailing_equals)
    ;
  যদি ((cksum_algorithm == sha2 || cksum_algorithm == sha3)
      && digest_hex_bytes >> 1 != SHA224_DIGEST_SIZE
      && digest_hex_bytes >> 1 != SHA256_DIGEST_SIZE
      && digest_hex_bytes >> 1 != SHA384_DIGEST_SIZE
      && digest_hex_bytes >> 1 != SHA512_DIGEST_SIZE)
    {
      যদি (digest_base64_bytes + trailing_equals
          == BASE64_LENGTH (SHA224_DIGEST_SIZE))
        digest_hex_bytes = SHA224_DIGEST_SIZE * 2;
      অন্যথা যদি (digest_base64_bytes + trailing_equals
               == BASE64_LENGTH (SHA256_DIGEST_SIZE))
        digest_hex_bytes = SHA256_DIGEST_SIZE * 2;
      অন্যথা যদি (digest_base64_bytes + trailing_equals
               == BASE64_LENGTH (SHA384_DIGEST_SIZE))
        digest_hex_bytes = SHA384_DIGEST_SIZE * 2;
      অন্যথা যদি (digest_base64_bytes + trailing_equals
               == BASE64_LENGTH (SHA512_DIGEST_SIZE))
        digest_hex_bytes = SHA512_DIGEST_SIZE * 2;
      অন্যথা
        ঘূৰাই false;
    }
  অন্যথা যদি (cksum_algorithm == blake2b
           && digest_hex_bytes < digest_base64_bytes)
    {
      বাবে (পূর্ণাঙ্ক j = 8; j <= DIGEST_MAX_LEN * 8; j += 8)
        {
          যদি (BASE64_LENGTH (j / 8) == digest_base64_bytes + trailing_equals
              && j % 3 == trailing_equals)
            {
              digest_hex_bytes = j / 4;
              ভঙ্গ;
            }
        }
    }
# endif
  যদি (digest_hex_bytes < 2 || digest_hex_bytes % 2
      || DIGEST_MAX_LEN * 2 < digest_hex_bytes)
    ঘূৰাই false;
  digest_length = digest_hex_bytes * 4;
# যদি HASH_ALGO_CKSUM
  }
# endif
#endif

  /* This field must be the hexadecimal or base64 representation
     of the message digest.  */
  যেতিয়ালৈ (s[i] && !c_isblank (s[i]))
    i++;

  /* The digest must be followed by at least one whitespace character.  */
  যদি (i == s_len)
    ঘূৰাই false;

  *d_len = &s[i] - (ৱর্ণ *) *digest;
  s[i++] = '\0';

  যদি (! valid_digits (*digest, *d_len))
    ঘূৰাই false;

  /* If "bsd reversed" format detected.  */
  যদি ((s_len - i == 1) || (s[i] != ' ' && s[i] != '*'))
    {
      /* Don't allow mixing bsd and standard formats,
         to minimize security issues with attackers
         renaming files with leading spaces.
         This assumes that with bsd format checksums
         that the first file name does not have
         a leading ' ' or '*'.  */
      যদি (bsd_reversed == 0)
        ঘূৰাই false;
      bsd_reversed = 1;
    }
  অন্যথা যদি (bsd_reversed != 1)
    {
      bsd_reversed = 0;
      *দ্বিমিক = (s[i++] == '*');
    }

  /* All characters between the type indicator and end of line are
     significant -- that includes leading and trailing white space.  */
  *file_name = &s[i];

  যদি (escaped_filename)
    ঘূৰাই filename_unescape (&s[i], s_len - i) != রিক্ত;

  ঘূৰাই true;
}

#যদি !HASH_ALGO_SUM
/* If ESCAPE is true, then translate each:
   NEWLINE byte to the string, "\\n",
   CARRIAGE RETURN byte to the string, "\\r",
   and each backslash to "\\\\".  */
স্থির শূন্য
print_filename (ৱর্ণ ধ্রুৱ *file, bool escape)
{
  যদি (! escape)
    {
      fputs (file, stdout);
      ঘূৰাই;
    }

  যেতিয়ালৈ (*file)
    {
      ৱিকল্প (*file)
        {
        পক্ষ '\n':
          fputs ("\\n", stdout);
          ভঙ্গ;

        পক্ষ '\r':
          fputs ("\\r", stdout);
          ভঙ্গ;

        পক্ষ '\\':
          fputs ("\\\\", stdout);
          ভঙ্গ;

        স্ৱভাৱ:
          putchar (*file);
          ভঙ্গ;
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

স্থির bool
digest_file (ৱর্ণ ধ্রুৱ *filename, পূর্ণাঙ্ক *দ্বিমিক, unsigned ৱর্ণ *bin_result,
             bool *missing, MAYBE_UNUSED intmax_t *length)
{
  FILE *fp;
  পূর্ণাঙ্ক err;
  bool is_stdin = streq (filename, "-");

  *missing = false;

  যদি (is_stdin)
    {
      have_read_stdin = true;
      fp = stdin;
      যদি (O_দ্বিমিক && *দ্বিমিক)
        {
          যদি (*দ্বিমিক < 0)
            *দ্বিমিক = ! isatty (STDIN_FILENO);
          যদি (*দ্বিমিক)
            xset_দ্বিমিক_mode (STDIN_FILENO, O_দ্বিমিক);
        }
    }
  অন্যথা
    {
      fp = fopen (filename, O_দ্বিমিক ? (*দ্বিমিক ? "rb" : "rt") : "r");
      যদি (fp == রিক্ত)
        {
          যদি (ignore_missing && errno == ENOENT)
            {
              *missing = true;
              ঘূৰাই true;
            }
          error (0, errno, "%s", quotef (filename));
          ঘূৰাই false;
        }
    }

  fadvise (fp, FADVISE_SEQUENTIAL);

#যদি HASH_ALGO_CKSUM
  যদি (cksum_algorithm == blake2b
      || cksum_algorithm == sha2 || cksum_algorithm == sha3)
    *length = digest_length >> 3;
  err = DIGEST_STREAM (fp, bin_result, length);
#নোচেদ্যদি HASH_ALGO_SUM
  err = DIGEST_STREAM (fp, bin_result, length);
#নোচেদ্যদি HASH_ALGO_BLAKE2
  err = DIGEST_STREAM (fp, bin_result, digest_length >> 3);
#অন্যথা
  err = DIGEST_STREAM (fp, bin_result);
#endif
  err = err ? errno : 0;
  যদি (is_stdin)
    clearerr (fp);
  অন্যথা যদি (fclose (fp) != 0 && !err)
    err = errno;

  যদি (err)
    {
      error (0, err, "%s", quotef (filename));
      ঘূৰাই false;
    }

  ঘূৰাই true;
}

#যদি !HASH_ALGO_SUM
স্থির শূন্য
output_file (ৱর্ণ ধ্রুৱ *file, পূর্ণাঙ্ক দ্বিমিক_file, শূন্য ধ্রুৱ *digest,
             MAYBE_UNUSED bool raw, bool tagged, unsigned ৱর্ণ delim,
             MAYBE_UNUSED bool args, MAYBE_UNUSED intmax_t length)
{
# যদি HASH_ALGO_CKSUM
  যদি (raw)
    {
      fwrite (digest, 1, digest_length >> 3, stdout);
      ঘূৰাই;
    }
# endif

  unsigned ৱর্ণ ধ্রুৱ *bin_buffer = digest;

  /* Output a leading backslash if the file name contains problematic chars.  */
  bool needs_escape = delim == '\n' && problematic_chars (file);

  যদি (needs_escape)
    putchar ('\\');

  যদি (tagged)
    {
# যদি HASH_ALGO_CKSUM
      যদি (cksum_algorithm == sha2)
        printf ("SHA%u", (unsigned পূর্ণাঙ্ক) digest_length);
      অন্যথা
# endif
      fputs (DIGEST_TYPE_STRING, stdout);
# যদি HASH_ALGO_BLAKE2
      যদি (digest_length < DIGEST_MAX_LEN * 8)
        printf ("-%u", (unsigned পূর্ণাঙ্ক) digest_length);
# elif HASH_ALGO_CKSUM
      যদি (cksum_algorithm == sha3)
        printf ("-%u", (unsigned পূর্ণাঙ্ক) digest_length);
      যদি (cksum_algorithm == blake2b)
        {
          যদি (digest_length < DIGEST_MAX_LEN * 8)
            printf ("-%u", (unsigned পূর্ণাঙ্ক) digest_length);
        }
# endif
      fputs (" (", stdout);
      print_filename (file, needs_escape);
      fputs (") = ", stdout);
    }

# যদি HASH_ALGO_CKSUM
  যদি (base64_digest)
    {
      ৱর্ণ b64[BASE64_LENGTH (DIGEST_BIN_BYTES) + 1];
      base64_encode ((ৱর্ণ ধ্রুৱ *) bin_buffer, digest_length >> 3,
                     b64, পরিমাণ b64);
      fputs (b64, stdout);
    }
  অন্যথা
# endif
    {
      বাবে (idx_t i = 0; i < (digest_hex_bytes >> 1); ++i)
        printf ("%02x", bin_buffer[i]);
    }

  যদি (!tagged)
    {
      putchar (' ');
      putchar (দ্বিমিক_file ? '*' : ' ');
      print_filename (file, needs_escape);
    }

  putchar (delim);
}
#endif

#যদি HASH_ALGO_CKSUM
/* Return true if B64_DIGEST is the same as the base64 digest of the
   DIGEST_LENGTH/8 bytes at BIN_BUFFER.  */
স্থির bool
b64_equal (unsigned ৱর্ণ ধ্রুৱ *b64_digest, unsigned ৱর্ণ ধ্রুৱ *bin_buffer)
{
  idx_t b64_n_bytes = BASE64_LENGTH (digest_length >> 3);
  ৱর্ণ b64[BASE64_LENGTH (DIGEST_BIN_BYTES) + 1];
  base64_encode ((ৱর্ণ ধ্রুৱ *) bin_buffer, digest_length >> 3,
                 b64, পরিমাণ b64);
  ঘূৰাই memeq (b64_digest, b64, b64_n_bytes + 1);
}
#endif

/* Return true if HEX_DIGEST is the same as the hex-encoded digest of the
   DIGEST_LENGTH/8 bytes at BIN_BUFFER.  */
স্থির bool
hex_equal (unsigned ৱর্ণ ধ্রুৱ *hex_digest, unsigned ৱর্ণ ধ্রুৱ *bin_buffer)
{
  স্থির ধ্রুৱ ৱর্ণ bin2hex[] = { '0', '1', '2', '3',
                                  '4', '5', '6', '7',
                                  '8', '9', 'a', 'b',
                                  'c', 'd', 'e', 'f' };
  idx_t digest_bin_bytes = digest_hex_bytes >> 1;

  /* Compare generated binary number with text representation
     in check file.  Ignore case of hex digits.  */
  idx_t cnt;
  বাবে (cnt = 0; cnt < digest_bin_bytes; ++cnt)
    {
      যদি (c_tolower (hex_digest[2 * cnt])
          != bin2hex[bin_buffer[cnt] >> 4]
          || (c_tolower (hex_digest[2 * cnt + 1])
              != (bin2hex[bin_buffer[cnt] & 0xf])))
        ভঙ্গ;
    }
  ঘূৰাই cnt == digest_bin_bytes;
}

স্থির bool
digest_check (ৱর্ণ ধ্রুৱ *checkfile_name)
{
  FILE *checkfile_stream;
  intmax_t n_misformatted_সাৰিগুলি = 0;
  intmax_t n_mismatched_checksums = 0;
  intmax_t n_open_or_read_failures = 0;
  bool properly_formatted_সাৰিগুলি = false;
  bool matched_checksums = false;
  unsigned ৱর্ণ bin_buffer_unaligned[DIGEST_BIN_BYTES + DIGEST_ALIGN];
  /* Make sure bin_buffer is properly aligned. */
  unsigned ৱর্ণ *bin_buffer = ptr_align (bin_buffer_unaligned, DIGEST_ALIGN);
  intmax_t সাৰি_সংখ্যা;
  ৱর্ণ *সাৰি;
  size_t সাৰি_chars_allocated;
  bool is_stdin = streq (checkfile_name, "-");

  যদি (is_stdin)
    {
      have_read_stdin = true;
      checkfile_name = _("standard input");
      checkfile_stream = stdin;
    }
  অন্যথা
    {
      checkfile_stream = fopen (checkfile_name, "r");
      যদি (checkfile_stream == রিক্ত)
        {
          error (0, errno, "%s", quotef (checkfile_name));
          ঘূৰাই false;
        }
    }

  সাৰি_সংখ্যা = 0;
  সাৰি = রিক্ত;
  সাৰি_chars_allocated = 0;
  কৰা
    {
      ৱর্ণ *filename;
      পূর্ণাঙ্ক দ্বিমিক;
      unsigned ৱর্ণ *digest;
      ssize_t সাৰি_length;

      ++সাৰি_সংখ্যা;
      যদি (সাৰি_সংখ্যা == 0)
        error (EXIT_FAILURE, 0, _("%s: too many checksum lines"),
               quotef (checkfile_name));

      সাৰি_length = getline (&সাৰি, &সাৰি_chars_allocated, checkfile_stream);
      যদি (সাৰি_length <= 0)
        ভঙ্গ;

      /* Ignore comment lines, which begin with a '#' character.  */
      যদি (সাৰি[0] == '#')
        অনুৱর্ত;

      /* Remove any trailing newline.  */
      সাৰি_length -= সাৰি[সাৰি_length - 1] == '\n';
      /* Remove any trailing carriage return.  */
      সাৰি_length -= সাৰি[সাৰি_length - (0 < সাৰি_length)] == '\r';

      /* Ignore empty lines.  */
      যদি (সাৰি_length == 0)
        অনুৱর্ত;

      সাৰি[সাৰি_length] = '\0';

      idx_t d_len;
      যদি (! (split_3 (সাৰি, সাৰি_length, &digest, &d_len, &দ্বিমিক, &filename)
             && ! (is_stdin && streq (filename, "-"))))
        {
          ++n_misformatted_সাৰিগুলি;

          যদি (warn)
            {
              error (0, 0,
                     _("%s: %jd"
                       ": improperly formatted %s checksum line"),
                     quotef (checkfile_name), সাৰি_সংখ্যা,
                     DIGEST_TYPE_STRING);
            }
        }
      অন্যথা
        {
          bool ok;
          bool missing;

          properly_formatted_সাৰিগুলি = true;

          intmax_t length;
          ok = digest_file (filename, &দ্বিমিক, bin_buffer, &missing, &length);

          যদি (!ok)
            {
              ++n_open_or_read_failures;
              যদি (!status_only)
                printf ("%s: %s\n", quotef (filename),_("FAILED open or read"));
            }
          অন্যথা যদি (ignore_missing && missing)
            {
              /* Ignore missing files with --ignore-missing.  */
              ;
            }
          অন্যথা
            {
              bool match = false;
#যদি HASH_ALGO_CKSUM
              যদি (d_len == BASE64_LENGTH (digest_length >> 3))
                match = b64_equal (digest, bin_buffer);
              অন্যথা
#endif
                যদি (d_len == digest_hex_bytes)
                  match = hex_equal (digest, bin_buffer);

              যদি (match)
                matched_checksums = true;
              অন্যথা
                ++n_mismatched_checksums;

              যদি (!status_only)
                {
                  যদি (! match || ! quiet)
                    fputs (quotef (filename), stdout);

                  যদি (! match)
                    printf (": %s\n", _("FAILED"));
                  অন্যথা যদি (!quiet)
                    printf (": %s\n", _("OK"));
                }
            }

          যদি (ferror (stdout))
            write_error ();
        }
    }
  যেতিয়ালৈ (!feof (checkfile_stream) && !ferror (checkfile_stream));

  মুক্ত (সাৰি);

  পূর্ণাঙ্ক err = ferror (checkfile_stream) ? 0 : -1;
  যদি (is_stdin)
    clearerr (checkfile_stream);
  অন্যথা যদি (fclose (checkfile_stream) != 0 && err < 0)
    err = errno;

  যদি (0 <= err)
    {
      error (0, err, err ? "%s" : _("%s: read error"),
             quotef (checkfile_name));
      ঘূৰাই false;
    }

  যদি (! properly_formatted_সাৰিগুলি)
    {
      /* Warn if no tests are found.  */
      error (0, 0, _("%s: no properly formatted checksum lines found"),
             quotef (checkfile_name));
    }
  অন্যথা
    {
      যদি (!status_only)
        {
          যদি (n_misformatted_সাৰিগুলি != 0)
            error (0, 0,
                   (ngettext
                    ("WARNING: %jd line is improperly formatted",
                     "WARNING: %jd lines are improperly formatted",
                     select_plural (n_misformatted_সাৰিগুলি))),
                   n_misformatted_সাৰিগুলি);

          যদি (n_open_or_read_failures != 0)
            error (0, 0,
                   (ngettext
                    ("WARNING: %jd listed file could not be read",
                     "WARNING: %jd listed files could not be read",
                     select_plural (n_open_or_read_failures))),
                   n_open_or_read_failures);

          যদি (n_mismatched_checksums != 0)
            error (0, 0,
                   (ngettext
                    ("WARNING: %jd computed checksum did NOT match",
                     "WARNING: %jd computed checksums did NOT match",
                     select_plural (n_mismatched_checksums))),
                   n_mismatched_checksums);

          যদি (ignore_missing && ! matched_checksums)
            error (0, 0, _("%s: no file was verified"),
                   quotef (checkfile_name));
        }
    }

  ঘূৰাই (properly_formatted_সাৰিগুলি
          && matched_checksums
          && n_mismatched_checksums == 0
          && n_open_or_read_failures == 0
          && (!strict || n_misformatted_সাৰিগুলি == 0));
}

পূর্ণাঙ্ক
main (পূর্ণাঙ্ক argc, ৱর্ণ **argv)
{
  unsigned ৱর্ণ bin_buffer_unaligned[DIGEST_BIN_BYTES + DIGEST_ALIGN];
  /* Make sure bin_buffer is properly aligned. */
  unsigned ৱর্ণ *bin_buffer = ptr_align (bin_buffer_unaligned, DIGEST_ALIGN);
  bool do_check = false;
  পূর্ণাঙ্ক opt;
  bool ok = true;
  পূর্ণাঙ্ক দ্বিমিক = -1;
  পূর্ণাঙ্ক prefix_tag = -1;
  সংরচনা option ধ্রুৱ *long_opts = long_options;

#যদি HASH_ALGO_CKSUM
  যদি (cksum_algorithm != crc)
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
  setvbuf (stdout, রিক্ত, _IOLBF, 0);

#যদি HASH_ALGO_SUM
  ৱর্ণ ধ্রুৱ *short_opts = "rs";
#নোচেদ্যদি HASH_ALGO_CKSUM
  ৱর্ণ ধ্রুৱ *short_opts = legacy_mode ? "bctwz" : "a:l:bctwz";
  ৱর্ণ ধ্রুৱ *digest_length_str = "";
#নোচেদ্যদি HASH_ALGO_BLAKE2
  ৱর্ণ ধ্রুৱ *short_opts = "l:bctwz";
  ৱর্ণ ধ্রুৱ *digest_length_str = "";
#অন্যথা
  ৱর্ণ ধ্রুৱ *short_opts = "bctwz";
#endif

  যেতিয়ালৈ ((opt = getopt_long (argc, argv, short_opts, long_opts, রিক্ত))
         != -1)
    ৱিকল্প (opt)
      {
#যদি HASH_ALGO_CKSUM
      পক্ষ 'a':
        cksum_algorithm = XARGMATCH_EXACT ("--algorithm", optarg,
                                           algorithm_args, algorithm_types);
        algorithm_specified = true;
        ভঙ্গ;

      পক্ষ DEBUG_PROGRAM_OPTION:
        cksum_debug = true;
        ভঙ্গ;
#endif
#যদি HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
      পক্ষ 'l':
        digest_length = xnumtoimax (optarg, 10, 0, IDX_MAX, "",
                                    _("invalid length"), 0,
                                    XTOINT_MAX_QUIET);
        digest_length_str = optarg;
        ভঙ্গ;
#endif
#যদি !HASH_ALGO_SUM
      পক্ষ 'c':
        do_check = true;
        ভঙ্গ;
      পক্ষ STATUS_OPTION:
        status_only = true;
        warn = false;
        quiet = false;
        ভঙ্গ;
      পক্ষ 'b':
        দ্বিমিক = 1;
        ভঙ্গ;
      পক্ষ 't':
        দ্বিমিক = 0;
        ভঙ্গ;
      পক্ষ 'w':
        status_only = false;
        warn = true;
        quiet = false;
        ভঙ্গ;
      পক্ষ IGNORE_MISSING_OPTION:
        ignore_missing = true;
        ভঙ্গ;
      পক্ষ QUIET_OPTION:
        status_only = false;
        warn = false;
        quiet = true;
        ভঙ্গ;
      পক্ষ STRICT_OPTION:
        strict = true;
        ভঙ্গ;
# যদি HASH_ALGO_CKSUM
      পক্ষ BASE64_OPTION:
        base64_digest = true;
        ভঙ্গ;
      পক্ষ RAW_OPTION:
        raw_digest = true;
        ভঙ্গ;
      পক্ষ UNTAG_OPTION:
        prefix_tag = 0;
        ভঙ্গ;
# endif
      পক্ষ TAG_OPTION:
        prefix_tag = 1;
        ভঙ্গ;
      পক্ষ 'z':
        digest_delim = '\0';
        ভঙ্গ;
#endif
#যদি HASH_ALGO_SUM
      পক্ষ 'r':		/* For SysV compatibility. */
        sum_algorithm = bsd;
        ভঙ্গ;

      পক্ষ 's':
        sum_algorithm = sysv;
        ভঙ্গ;
#endif
      case_GETOPT_HELP_CHAR;
      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
      স্ৱভাৱ:
        usage (EXIT_FAILURE);
      }

  min_digest_সাৰি_length = MIN_DIGEST_সাৰি_LENGTH;
#যদি HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# যদি HASH_ALGO_CKSUM
  যদি (digest_length && (cksum_algorithm != blake2b
                        && cksum_algorithm != sha2
                        && cksum_algorithm != sha3))
    error (EXIT_FAILURE, 0,
           _("--length is only supported with --algorithm "
             "blake2b, sha2, or sha3"));
  যদি (cksum_algorithm == sha2 || cksum_algorithm == sha3)
    {
      /* Do not require --length with --check.  */
      যদি (digest_length == 0 && *digest_length_str == '\0' && ! do_check)
        error (EXIT_FAILURE, 0, _("--algorithm=%s requires specifying "
                                  "--length 224, 256, 384, or 512"),
               algorithm_args[cksum_algorithm]);
      /* If --check and --length are used we verify the digest length.  */
      যদি ((! do_check || *digest_length_str != '\0')
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
  অন্যথা
    {
      /* If the digest length checks for SHA-3 are satisfied, the less strict
         checks for BLAKE2 will also be.  */
# অন্যথা
    {
# endif
      যদি (digest_length > DIGEST_MAX_LEN * 8)
        {
          error (0, 0, _("invalid length: %s"), quote (digest_length_str));
          error (EXIT_FAILURE, 0,
                 _("maximum digest length for %s is %d bits"),
                 quote (DIGEST_TYPE_STRING),
                 DIGEST_MAX_LEN * 8);
        }
      যদি (digest_length % 8 != 0)
        {
          error (0, 0, _("invalid length: %s"), quote (digest_length_str));
          error (EXIT_FAILURE, 0, _("length is not a multiple of 8"));
        }
    }
  যদি (digest_length == 0)
    {
# যদি HASH_ALGO_BLAKE2
      digest_length = DIGEST_MAX_LEN * 8;
# অন্যথা
      digest_length = algorithm_bits[cksum_algorithm];
# endif
    }
  digest_hex_bytes = digest_length >> 2;
#অন্যথা
  digest_hex_bytes = DIGEST_HEX_BYTES;
#endif

#যদি HASH_ALGO_CKSUM
  ৱিকল্প (+cksum_algorithm)
    {
    পক্ষ bsd:
    পক্ষ sysv:
    পক্ষ crc:
    পক্ষ crc32b:
        যদি (do_check && algorithm_specified)
          error (EXIT_FAILURE, 0,
                 _("--check is not supported with "
                   "--algorithm={bsd,sysv,crc,crc32b}"));
        ভঙ্গ;
    }

  যদি (base64_digest && raw_digest)
   {
     error (0, 0, _("--base64 and --raw are mutually exclusive"));
     usage (EXIT_FAILURE);
   }
#endif

  যদি (digest_delim != '\n' && do_check)
    {
      error (0, 0, _("the --zero option is not supported when "
                     "verifying checksums"));
      usage (EXIT_FAILURE);
    }
  যদি (1 <= prefix_tag && do_check)
    {
      /* Note we allow --untagged with --check to more
         seamlessly support --untagged in an emulation wrapper.  */
      error (0, 0, _("the --tag option is meaningless when "
                     "verifying checksums"));
      usage (EXIT_FAILURE);
    }

  যদি (0 <= দ্বিমিক && do_check)
    {
      error (0, 0, _("the --binary and --text options are meaningless when "
                     "verifying checksums"));
      usage (EXIT_FAILURE);
    }

  যদি (ignore_missing && !do_check)
    {
      error (0, 0,
             _("the --ignore-missing option is meaningful only when "
               "verifying checksums"));
      usage (EXIT_FAILURE);
    }

  যদি (status_only && !do_check)
    {
      error (0, 0,
       _("the --status option is meaningful only when verifying checksums"));
      usage (EXIT_FAILURE);
    }

  যদি (warn && !do_check)
    {
      error (0, 0,
       _("the --warn option is meaningful only when verifying checksums"));
      usage (EXIT_FAILURE);
    }

  যদি (quiet && !do_check)
    {
      error (0, 0,
       _("the --quiet option is meaningful only when verifying checksums"));
      usage (EXIT_FAILURE);
    }

  যদি (strict & !do_check)
   {
     error (0, 0,
        _("the --strict option is meaningful only when verifying checksums"));
     usage (EXIT_FAILURE);
   }

  যদি (prefix_tag == -1)
    prefix_tag = HASH_ALGO_CKSUM;

  যদি (prefix_tag && !দ্বিমিক)
   {
     /* This could be supported in a backwards compatible way
        by prefixing the output line with a space in text mode.
        However that's invasive enough that it was agreed to
        not support this mode with --tag, as --text use cases
        are adequately supported by the default output format.  */
#যদি !HASH_ALGO_CKSUM
     error (0, 0, _("--tag does not support --text mode"));
#অন্যথা
     error (0, 0, _("--text mode is only supported with --untagged"));
#endif
     usage (EXIT_FAILURE);
   }

  যদি (!O_দ্বিমিক && দ্বিমিক < 0)
    দ্বিমিক = 0;
  অন্যথা যদি (prefix_tag)
    দ্বিমিক = 1;

  ৱর্ণ **operand_lim = argv + argc;
  যদি (optind == argc)
    *operand_lim++ = bad_cast ("-");
  অন্যথা যদি (1 < argc - optind && raw_digest)
    error (EXIT_FAILURE, 0,
           _("the --raw option is not supported with multiple files"));

  বাবে (ৱর্ণ **operandp = argv + optind; operandp < operand_lim; operandp++)
    {
      ৱর্ণ *file = *operandp;
      যদি (do_check)
        ok &= digest_check (file);
      অন্যথা
        {
          পূর্ণাঙ্ক দ্বিমিক_file = দ্বিমিক;
          bool missing;
          intmax_t length;

          যদি (! digest_file (file, &দ্বিমিক_file, bin_buffer, &missing, &length))
            ok = false;
          অন্যথা
            {
              DIGEST_OUT (file, দ্বিমিক_file, bin_buffer, raw_digest, prefix_tag,
                          digest_delim, optind != argc, length);
              যদি (ferror (stdout))
                write_error ();
            }
        }
    }

  যদি (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, _("standard input"));

  ঘূৰাই ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
