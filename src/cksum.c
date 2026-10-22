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

#অন্তর্ভাব <config.h>

#অন্তর্ভাব <getopt.h>
#অন্তর্ভাব <sys/types.h>

#অন্তর্ভাব "assure.h"
#অন্তর্ভাব "system.h"
#অন্তর্ভাব "argmatch.h"
#অন্তর্ভাব "c-ctype.h"
#অন্তর্ভাব "quote.h"
#অন্তর্ভাব "xdectoint.h"
#অন্তর্ভাব "xstrtol.h"

#ifndef HASH_ALGO_CKSUM
# define HASH_ALGO_CKSUM 0
#endif

#যদি HASH_ALGO_যোগ || HASH_ALGO_CKSUM
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
#অন্তর্ভাব "fadvise.h"
#অন্তর্ভাব "stdio--.h"
#অন্তর্ভাব "xbinary-io.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#যদি HASH_ALGO_যোগ
# define PROGRAM_NAME "sum"
# define DIGEST_TYPE_STRING "BSD"
# define DIGEST_STREAM sumfns[যোগ_algorithm]
# define DIGEST_OUT যোগ_output_fns[যোগ_algorithm]
# define DIGEST_BITS 16
# define DIGEST_ALIGN 4
#নতুবা_যদি HASH_ALGO_CKSUM
# define MAX_DIGEST_BITS 512
# define MAX_DIGEST_ALIGN 8
# define DIGEST_TYPE_STRING algorithm_tags[cksum_algorithm]
# define DIGEST_STREAM cksumfns[cksum_algorithm]
# define DIGEST_OUT cksum_output_fns[cksum_algorithm]
# define DIGEST_BITS MAX_DIGEST_BITS
# define DIGEST_ALIGN MAX_DIGEST_ALIGN
#নতুবা_যদি HASH_ALGO_MD5
# define PROGRAM_NAME "md5sum"
# define DIGEST_TYPE_STRING "MD5"
# define DIGEST_STREAM md5_stream
# define DIGEST_BITS 128
# define DIGEST_REFERENCE "RFC 1321"
# define DIGEST_ALIGN 4
#নতুবা_যদি HASH_ALGO_BLAKE2
# define PROGRAM_NAME "b2sum"
# define DIGEST_TYPE_STRING "BLAKE2b"
# define DIGEST_STREAM blake2b_stream
# define DIGEST_BITS 512
# define DIGEST_REFERENCE "RFC 7693"
# define DIGEST_ALIGN 8
#নতুবা_যদি HASH_ALGO_SHA1
# define PROGRAM_NAME "sha1sum"
# define DIGEST_TYPE_STRING "SHA1"
# define DIGEST_STREAM sha1_stream
# define DIGEST_BITS 160
# define DIGEST_REFERENCE "FIPS-180-1"
# define DIGEST_ALIGN 4
#নতুবা_যদি HASH_ALGO_SHA256
# define PROGRAM_NAME "sha256sum"
# define DIGEST_TYPE_STRING "SHA256"
# define DIGEST_STREAM sha256_stream
# define DIGEST_BITS 256
# define DIGEST_REFERENCE "FIPS-180-2"
# define DIGEST_ALIGN 4
#নতুবা_যদি HASH_ALGO_SHA224
# define PROGRAM_NAME "sha224sum"
# define DIGEST_TYPE_STRING "SHA224"
# define DIGEST_STREAM sha224_stream
# define DIGEST_BITS 224
# define DIGEST_REFERENCE "RFC 3874"
# define DIGEST_ALIGN 4
#নতুবা_যদি HASH_ALGO_SHA512
# define PROGRAM_NAME "sha512sum"
# define DIGEST_TYPE_STRING "SHA512"
# define DIGEST_STREAM sha512_stream
# define DIGEST_BITS 512
# define DIGEST_REFERENCE "FIPS-180-2"
# define DIGEST_ALIGN 8
#নতুবা_যদি HASH_ALGO_SHA384
# define PROGRAM_NAME "sha384sum"
# define DIGEST_TYPE_STRING "SHA384"
# define DIGEST_STREAM sha384_stream
# define DIGEST_BITS 384
# define DIGEST_REFERENCE "FIPS-180-2"
# define DIGEST_ALIGN 8
#অন্যথা
# error "Can't decide which hash algorithm to compile."
#endif
#যদি !HASH_ALGO_যোগ && !HASH_ALGO_CKSUM
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

#যদি HASH_ALGO_যোগ
# define AUTHORS \
  proper_name ("Kayvan Aghaiepour"), \
  proper_name ("David MacKenzie")
#নতুবা_যদি HASH_ALGO_CKSUM
# define AUTHORS \
  proper_name_lite ("Padraig Brady", "P\303\241draig Brady"), \
  proper_name ("Q. Frank Xia")
#নতুবা_যদি HASH_ALGO_BLAKE2
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
# define MIN_DIGEST_সারি_দৈর্ঘ্য 3 /* With -l 8.  */
#অন্যথা
# define MIN_DIGEST_সারি_দৈর্ঘ্য \
   (DIGEST_HEX_BYTES /* length of hexadecimal message digest */ \
    + 1 /* blank */ \
    + 1 /* minimum filename length */ )
#endif

#যদি !HASH_ALGO_যোগ
স্থির শূন্য
output_file (অক্ষর ধ্রুবক *file, পূর্ণসংখ্যা দ্বিমিক_file, শূন্য ধ্রুবক *digest,
             bool raw, bool tagged, unsigned অক্ষর delim, bool args,
             intmax_t দৈর্ঘ্য);
#endif

/* True if any of the files read were the standard input. */
স্থির bool have_read_stdin;

/* The minimum length of a valid checksum line for the selected algorithm.  */
স্থির idx_t min_digest_সারি_দৈর্ঘ্য;

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
স্থির পূর্ণসংখ্যা bsd_reversed = -1;

/* line delimiter.  */
স্থির unsigned অক্ষর digest_delim = '\n';

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
static_assert (BLAKE2B_OUTBYTES == SHA3_512_DIGEST_আকার);
# endif
# define DIGEST_MAX_LEN BLAKE2B_OUTBYTES
স্থির idx_t digest_দৈর্ঘ্য;
#endif /* HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM */

প্রকারলেখ শূন্য (*digest_output_fn)(অক্ষর ধ্রুবক *, পূর্ণসংখ্যা, শূন্য ধ্রুবক *, bool,
                                 bool, unsigned অক্ষর, bool, intmax_t);
#যদি HASH_ALGO_যোগ
গণনা Algorithm
{
  bsd,
  sysv,
};

স্থির গণনা Algorithm যোগ_algorithm;
স্থির sumfn sumfns[]=
{
  bsd_যোগ_stream,
  sysv_যোগ_stream,
};
স্থির digest_output_fn যোগ_output_fns[]=
{
  output_bsd,
  output_sysv,
};
#endif

#যদি HASH_ALGO_CKSUM
স্থির পূর্ণসংখ্যা
md5_যোগ_stream (FILE *stream, শূন্য *resstream, MAYBE_UNUSED intmax_t *দৈর্ঘ্য)
{
  ফেরত md5_stream (stream, resstream);
}
স্থির পূর্ণসংখ্যা
sha1_যোগ_stream (FILE *stream, শূন্য *resstream, MAYBE_UNUSED intmax_t *দৈর্ঘ্য)
{
  ফেরত sha1_stream (stream, resstream);
}
স্থির পূর্ণসংখ্যা
sha224_যোগ_stream (FILE *stream, শূন্য *resstream,
                   MAYBE_UNUSED intmax_t *দৈর্ঘ্য)
{
  ফেরত sha224_stream (stream, resstream);
}
স্থির পূর্ণসংখ্যা
sha256_যোগ_stream (FILE *stream, শূন্য *resstream,
                   MAYBE_UNUSED intmax_t *দৈর্ঘ্য)
{
  ফেরত sha256_stream (stream, resstream);
}
স্থির পূর্ণসংখ্যা
sha384_যোগ_stream (FILE *stream, শূন্য *resstream,
                   MAYBE_UNUSED intmax_t *দৈর্ঘ্য)
{
  ফেরত sha384_stream (stream, resstream);
}
স্থির পূর্ণসংখ্যা
sha512_যোগ_stream (FILE *stream, শূন্য *resstream,
                   MAYBE_UNUSED intmax_t *দৈর্ঘ্য)
{
  ফেরত sha512_stream (stream, resstream);
}
স্থির পূর্ণসংখ্যা
sha2_যোগ_stream (FILE *stream, শূন্য *resstream, intmax_t *দৈর্ঘ্য)
{
  নির্বাচন (*দৈর্ঘ্য)
    {
    ক্ষেত্র SHA224_DIGEST_আকার:
      ফেরত sha224_stream (stream, resstream);
    ক্ষেত্র SHA256_DIGEST_আকার:
      ফেরত sha256_stream (stream, resstream);
    ক্ষেত্র SHA384_DIGEST_আকার:
      ফেরত sha384_stream (stream, resstream);
    ক্ষেত্র SHA512_DIGEST_আকার:
      ফেরত sha512_stream (stream, resstream);
    পূর্বনির্ধারিত:
      affirm (false);
    }
}
স্থির পূর্ণসংখ্যা
sha3_যোগ_stream (FILE *stream, শূন্য *resstream, intmax_t *দৈর্ঘ্য)
{
  নির্বাচন (*দৈর্ঘ্য)
    {
    ক্ষেত্র SHA3_224_DIGEST_আকার:
      ফেরত sha3_224_stream (stream, resstream);
    ক্ষেত্র SHA3_256_DIGEST_আকার:
      ফেরত sha3_256_stream (stream, resstream);
    ক্ষেত্র SHA3_384_DIGEST_আকার:
      ফেরত sha3_384_stream (stream, resstream);
    ক্ষেত্র SHA3_512_DIGEST_আকার:
      ফেরত sha3_512_stream (stream, resstream);
    পূর্বনির্ধারিত:
      affirm (false);
    }
}
স্থির পূর্ণসংখ্যা
blake2b_যোগ_stream (FILE *stream, শূন্য *resstream, intmax_t *দৈর্ঘ্য)
{
  ফেরত blake2b_stream (stream, resstream, *দৈর্ঘ্য);
}
স্থির পূর্ণসংখ্যা
sm3_যোগ_stream (FILE *stream, শূন্য *resstream, MAYBE_UNUSED intmax_t *দৈর্ঘ্য)
{
  ফেরত sm3_stream (stream, resstream);
}

স্থির অক্ষর ধ্রুবক *ধ্রুবক algorithm_args[] =
{
  "bsd", "sysv", "crc", "crc32b", "md5", "sha1",
  "sha224", "sha256", "sha384", "sha512", /* Legacy naming */
  "sha2", "sha3", "blake2b", "sm3", ফাঁকা
};
স্থির গণনা Algorithm ধ্রুবক algorithm_types[] =
{
  bsd, sysv, crc, crc32b, md5, sha1,
  sha224, sha256, sha384, sha512,
  sha2, sha3, blake2b, sm3,
};
ARGMATCH_VERIFY (algorithm_args, algorithm_types);

স্থির অক্ষর ধ্রুবক *ধ্রুবক algorithm_tags[] =
{
  "BSD", "SYSV", "CRC", "CRC32B", "MD5", "SHA1",
  "SHA224", "SHA256", "SHA384", "SHA512",
  "SHA2", "SHA3", "BLAKE2b", "SM3", ফাঁকা
};
স্থির পূর্ণসংখ্যা ধ্রুবক algorithm_bits[] =
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
  bsd_যোগ_stream,
  sysv_যোগ_stream,
  crc_যোগ_stream,
  crc32b_যোগ_stream,
  md5_যোগ_stream,
  sha1_যোগ_stream,
  sha224_যোগ_stream,
  sha256_যোগ_stream,
  sha384_যোগ_stream,
  sha512_যোগ_stream,
  sha2_যোগ_stream,
  sha3_যোগ_stream,
  blake2b_যোগ_stream,
  sm3_যোগ_stream,
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
স্থির কাঠামো option ধ্রুবক legacy_long_options[] =
{
  { "check", no_argument, ফাঁকা, 'c' },
  { "ignore-missing", no_argument, ফাঁকা, IGNORE_MISSING_OPTION },
  { "quiet", no_argument, ফাঁকা, QUIET_OPTION },
  { "status", no_argument, ফাঁকা, STATUS_OPTION },
  { "warn", no_argument, ফাঁকা, 'w' },
  { "strict", no_argument, ফাঁকা, STRICT_OPTION },
  { "tag", no_argument, ফাঁকা, TAG_OPTION },
  { "zero", no_argument, ফাঁকা, 'z' },

  { "binary", no_argument, ফাঁকা, 'b' }, /* Deprecated.  */
  { "text", no_argument, ফাঁকা, 't' }, /* Deprecated.  */

  { GETOPT_HELP_OPTION_DECL },
  { GETOPT_VERSION_OPTION_DECL },
  { ফাঁকা, 0, ফাঁকা, 0 }
};
#endif

স্থির কাঠামো option ধ্রুবক long_options[] =
{
#যদি HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
  { "length", required_argument, ফাঁকা, 'l'},
#endif

#যদি !HASH_ALGO_যোগ
  { "check", no_argument, ফাঁকা, 'c' },
  { "ignore-missing", no_argument, ফাঁকা, IGNORE_MISSING_OPTION},
  { "quiet", no_argument, ফাঁকা, QUIET_OPTION },
  { "status", no_argument, ফাঁকা, STATUS_OPTION },
  { "warn", no_argument, ফাঁকা, 'w' },
  { "strict", no_argument, ফাঁকা, STRICT_OPTION },
  { "tag", no_argument, ফাঁকা, TAG_OPTION },
  { "zero", no_argument, ফাঁকা, 'z' },

# যদি HASH_ALGO_CKSUM
  { "algorithm", required_argument, ফাঁকা, 'a'},
  { "base64", no_argument, ফাঁকা, BASE64_OPTION },
  { "debug", no_argument, ফাঁকা, DEBUG_PROGRAM_OPTION},
  { "raw", no_argument, ফাঁকা, RAW_OPTION},
  { "untagged", no_argument, ফাঁকা, UNTAG_OPTION },
# endif
  { "binary", no_argument, ফাঁকা, 'b' }, /* Deprecated.  */
  { "text", no_argument, ফাঁকা, 't' }, /* Deprecated.  */

#অন্যথা
  {"sysv", no_argument, ফাঁকা, 's'},  /* Not in cksum.  */
#endif

  { GETOPT_HELP_OPTION_DECL },
  { GETOPT_VERSION_OPTION_DECL },
  { ফাঁকা, 0, ফাঁকা, 0 }
};

শূন্য
usage (পূর্ণসংখ্যা status)
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
By পূর্বনির্ধারিত use the 32 bit CRC algorithm.\n\
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
#যদি HASH_ALGO_যোগ
      oputs (_("\
  -r\n\
         use BSD যোগ algorithm (the পূর্বনির্ধারিত), use 1K blocks\n\
"));
      oputs (_("\
  -s, --sysv\n\
         use System V যোগ algorithm, use 512 bytes blocks\n\
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
#যদি !HASH_ALGO_যোগ
# যদি HASH_ALGO_CKSUM
     যদি (legacy_mode)
       {
# endif
      যদি (O_দ্বিমিক)
        oputs (_("\
  -b, --দ্বিমিক\n\
         পঠ in দ্বিমিক mode (পূর্বনির্ধারিত unless reading tty stdin)\n\
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
  -l, --দৈর্ঘ্য=BITS\n\
         digest দৈর্ঘ্য in bits; must not exceed the max আকার_2\n\
         and must be a multiple of 8 জন্য blake2b;\n\
         must be 224, 256, 384, or 512 জন্য sha2 or sha3\n\
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
         create a BSD-style checksum (the পূর্বনির্ধারিত)\n\
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
         পঠ in text mode (পূর্বনির্ধারিত যদি reading tty stdin)\n\
"));
      অন্যথা
        oputs (_("\
  -t, --text\n\
         পঠ in text mode (পূর্বনির্ধারিত)\n\
"));
# যদি HASH_ALGO_CKSUM
      }
# endif
      oputs (_("\
  -z, --zero\n\
         end each output সারি with NUL, not newline,\n\
         and disable file name escaping\n\
"));
      fputs (_("\
\n\
The following five options are useful only when verifying checksums:\n\
"), stdout);
      oputs (_("\
      --ignore-missing\n\
         don't fail or report status জন্য missing files\n\
"));
      oputs (_("\
      --quiet\n\
         don't print OK জন্য each successfully verified file\n\
"));
      oputs (_("\
      --status\n\
         don't output anything, status code shows success\n\
"));
      oputs (_("\
      --strict\n\
         exit non-zero জন্য improperly formatted checksum সারিগুলি\n\
"));
      oputs (_("\
  -w, --warn\n\
         warn about improperly formatted checksum সারিগুলি\n\
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
DIGEST determines the digest algorithm and পূর্বনির্ধারিত output format:\n\
  sysv      (equivalent to যোগ -s)\n\
  bsd       (equivalent to যোগ -r)\n\
  crc       (equivalent to cksum)\n\
  crc32b    (only available through cksum)\n\
  md5       (equivalent to md5sum)\n\
  sha1      (equivalent to sha1sum)\n\
  sha2      (equivalent to sha{224,256,384,512}যোগ)\n\
  sha3      (only available through cksum)\n\
  blake2b   (equivalent to b2sum)\n\
  sm3       (only available through cksum)\n\
\n"), stdout);
#endif
#যদি !HASH_ALGO_যোগ && !HASH_ALGO_CKSUM
      printf (_("\
\n\
The sums are computed as described in %s.\n"), DIGEST_REFERENCE);
      fputs (_("\
When checking, the প্রবেশ should be a former output of this program.\n\
The পূর্বনির্ধারিত mode is to print a সারি with: checksum, a space,\n\
a character indicating প্রবেশ mode ('*' জন্য দ্বিমিক, ' ' জন্য text\n\
or where দ্বিমিক is insignificant), and name জন্য each FILE.\n\
\n\
There is no difference between দ্বিমিক mode and text mode on GNU systems.\
\n"), stdout);
#endif
#যদি HASH_ALGO_CKSUM
      fputs (_("\
When checking, the প্রবেশ should be a former output of this program,\n\
or equivalent standalone program.\
\n"), stdout);
#endif
      emit_ancillary_info (PROGRAM_NAME);
    }

  exit (status);
}

#যদি !HASH_ALGO_যোগ
/* Given a string S, return TRUE if it contains problematic characters
   that need escaping.  Note we escape '\' itself to provide some forward
   compat to introduce escaping of other characters.  */

ATTRIBUTE_PURE
স্থির bool
problematic_chars (অক্ষর ধ্রুবক *s)
{
  idx_t দৈর্ঘ্য = strcspn (s, "\\\n\r");
  ফেরত s[দৈর্ঘ্য] != '\0';
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

স্থির অক্ষর *
filename_unescape (অক্ষর *s, idx_t s_len)
{
  অক্ষর *dst = s;

  জন্য (idx_t i = 0; i < s_len; i++)
    {
      নির্বাচন (s[i])
        {
        ক্ষেত্র '\\':
          যদি (i == s_len - 1)
            {
              /* File name ends with an unescaped backslash: invalid.  */
              ফেরত ফাঁকা;
            }
          ++i;
          নির্বাচন (s[i])
            {
            ক্ষেত্র 'n':
              *dst++ = '\n';
              বিরতি;
            ক্ষেত্র 'r':
              *dst++ = '\r';
              বিরতি;
            ক্ষেত্র '\\':
              *dst++ = '\\';
              বিরতি;
            পূর্বনির্ধারিত:
              /* Only '\', 'n' or 'r' may follow a backslash.  */
              ফেরত ফাঁকা;
            }
          বিরতি;

        ক্ষেত্র '\0':
          /* The file name may not contain a NUL.  */
          ফেরত ফাঁকা;

        পূর্বনির্ধারিত:
          *dst++ = s[i];
          বিরতি;
        }
    }
  যদি (dst < s + s_len)
    *dst = '\0';

  ফেরত s;
}

/* Return true if S is a LEN-byte NUL-terminated string of hex or base64
   digits and has the expected length.  Otherwise, return false.  */
ATTRIBUTE_PURE
স্থির bool
valid_digits (unsigned অক্ষর ধ্রুবক *s, idx_t len)
{
#যদি HASH_ALGO_CKSUM
  যদি (len == BASE64_দৈর্ঘ্য (digest_দৈর্ঘ্য >> 3))
    {
      idx_t i;
      জন্য (i = 0; i < len - digest_দৈর্ঘ্য % 3; i++)
        {
          যদি (!isbase64 (*s))
            ফেরত false;
          ++s;
        }
      জন্য ( ; i < len; i++)
        {
          যদি (*s != '=')
            ফেরত false;
          ++s;
        }
    }
  অন্যথা
#endif
  যদি (len == digest_hex_bytes)
    {
      জন্য (idx_t i = 0; i < digest_hex_bytes; i++)
        {
          যদি (!c_isxdigit (*s))
            ফেরত false;
          ++s;
        }
    }
  অন্যথা
    ফেরত false;

  ফেরত *s == '\0';
}

/* Split the checksum string S (of length S_LEN) from a BSD 'md5' or
   'sha1' command into two parts: a hexadecimal digest, and the file
   name.  S is modified.  Set *D_LEN to the length of the digest string.
   Return true if successful.  */

স্থির bool
bsd_split_3 (অক্ষর *s, idx_t s_len,
             unsigned অক্ষর **digest, idx_t *d_len,
             অক্ষর **file_name, bool escaped_filename)
{
  যদি (s_len == 0)
    ফেরত false;

  /* Find end of filename.  */
  idx_t i = s_len - 1;
  যখন (i && s[i] != ')')
    i--;

  যদি (s[i] != ')')
    ফেরত false;

  *file_name = s;

  যদি (escaped_filename && filename_unescape (s, i) == ফাঁকা)
    ফেরত false;

  s[i++] = '\0';

  যখন (c_isblank (s[i]))
    i++;

  যদি (s[i] != '=')
    ফেরত false;

  i++;

  যখন (c_isblank (s[i]))
    i++;

  *digest = (unsigned অক্ষর *) &s[i];

  *d_len = s_len - i;
  ফেরত valid_digits (*digest, *d_len);
}

#যদি HASH_ALGO_CKSUM
/* Return the corresponding Algorithm for the string S,
   or -1 for no match.  */

স্থির ptrdiff_t
algorithm_from_tag (অক্ষর *s)
{
  /* Limit check size to this length for perf reasons.  */
  স্থির idx_t max_tag_len;
  যদি (! max_tag_len)
    {
      অক্ষর ধ্রুবক * ধ্রুবক * tag = algorithm_tags;
      যখন (*tag)
        {
          idx_t tag_len = strlen (*tag++);
          max_tag_len = MAX (tag_len, max_tag_len);
        }
    }

  idx_t i = 0;

  /* Find end of tag */
  যখন (i <= max_tag_len && s[i] && ! c_isblank (s[i])
         && s[i] != '-' && s[i] != '(')
    ++i;

  যদি (i > max_tag_len)
    ফেরত -1;

  /* Terminate tag, and lookup.  */
  অক্ষর sep = s[i];
  s[i] = '\0';
  ptrdiff_t algo = argmatch_exact (s, algorithm_tags);
  s[i] = sep;

  ফেরত algo;
}
#endif

/* Split the string S (of length S_LEN) into three parts:
   a hexadecimal digest, binary flag, and the file name.
   S is modified.  Set *D_LEN to the length of the digest string.
   Return true if successful.  */

স্থির bool
split_3 (অক্ষর *s, idx_t s_len,
         unsigned অক্ষর **digest, idx_t *d_len, পূর্ণসংখ্যা *দ্বিমিক, অক্ষর **file_name)
{
  bool escaped_filename = false;
  idx_t algo_name_len;

  idx_t i = 0;
  যখন (c_isblank (s[i]))
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
                ফেরত false;  /* We don't support checking these formats.  */
              cksum_algorithm = algo_tag;
            }
          অন্যথা
            ফেরত false;      /* We only support tagged format without -a.  */
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
      digest_দৈর্ঘ্য = DIGEST_MAX_LEN * 8;
# অন্যথা
      digest_দৈর্ঘ্য = algorithm_bits[cksum_algorithm];
# endif
      যদি (s[i] == '-')  /* length specified. Not base64 */
        {
          ++i;
          intmax_t দৈর্ঘ্য;
          অক্ষর *siend;
          যদি (!c_isdigit (s[i])
              || xstrtoimax (s + i, &siend, 0, &দৈর্ঘ্য, ফাঁকা) != LONGINT_OK)
            ফেরত false;
# যদি HASH_ALGO_CKSUM
          অন্যথা যদি (cksum_algorithm == sha2 || cksum_algorithm == sha3)
            {
              যদি (দৈর্ঘ্য != SHA224_DIGEST_আকার * 8
                  && দৈর্ঘ্য != SHA256_DIGEST_আকার * 8
                  && দৈর্ঘ্য != SHA384_DIGEST_আকার * 8
                  && দৈর্ঘ্য != SHA512_DIGEST_আকার * 8)
                ফেরত false;
            }
# endif
          অন্যথা যদি (!(0 < দৈর্ঘ্য && দৈর্ঘ্য <= digest_দৈর্ঘ্য && দৈর্ঘ্য % 8 == 0))
            ফেরত false;

          i = siend - s;
          digest_দৈর্ঘ্য = দৈর্ঘ্য;
        }
      digest_hex_bytes = digest_দৈর্ঘ্য >> 2;
#endif
      যদি (s[i] == ' ')
        ++i;
      যদি (s[i] == '(')  /* not base64 */
        {
          ++i;
          *দ্বিমিক = 0;
          ফেরত bsd_split_3 (s + i, s_len - i,
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
  যদি (s_len - i < min_digest_সারি_দৈর্ঘ্য + (s[i] == '\\'))
    ফেরত false;

  *digest = (unsigned অক্ষর *) &s[i];

#যদি HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
  /* Auto determine length.  */
# যদি HASH_ALGO_CKSUM
  যদি (cksum_algorithm == blake2b
      || cksum_algorithm == sha2 || cksum_algorithm == sha3) {
# endif
  unsigned অক্ষর ধ্রুবক *hp = *digest;
  digest_hex_bytes = 0;
  জন্য (; c_isxdigit (*hp); ++hp, ++digest_hex_bytes)
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
  জন্য (; isubase64 (*hp); ++hp, ++digest_base64_bytes)
    ;
  জন্য (; *hp == '='; ++hp, ++trailing_equals)
    ;
  যদি ((cksum_algorithm == sha2 || cksum_algorithm == sha3)
      && digest_hex_bytes >> 1 != SHA224_DIGEST_আকার
      && digest_hex_bytes >> 1 != SHA256_DIGEST_আকার
      && digest_hex_bytes >> 1 != SHA384_DIGEST_আকার
      && digest_hex_bytes >> 1 != SHA512_DIGEST_আকার)
    {
      যদি (digest_base64_bytes + trailing_equals
          == BASE64_দৈর্ঘ্য (SHA224_DIGEST_আকার))
        digest_hex_bytes = SHA224_DIGEST_আকার * 2;
      অন্যথা যদি (digest_base64_bytes + trailing_equals
               == BASE64_দৈর্ঘ্য (SHA256_DIGEST_আকার))
        digest_hex_bytes = SHA256_DIGEST_আকার * 2;
      অন্যথা যদি (digest_base64_bytes + trailing_equals
               == BASE64_দৈর্ঘ্য (SHA384_DIGEST_আকার))
        digest_hex_bytes = SHA384_DIGEST_আকার * 2;
      অন্যথা যদি (digest_base64_bytes + trailing_equals
               == BASE64_দৈর্ঘ্য (SHA512_DIGEST_আকার))
        digest_hex_bytes = SHA512_DIGEST_আকার * 2;
      অন্যথা
        ফেরত false;
    }
  অন্যথা যদি (cksum_algorithm == blake2b
           && digest_hex_bytes < digest_base64_bytes)
    {
      জন্য (পূর্ণসংখ্যা j = 8; j <= DIGEST_MAX_LEN * 8; j += 8)
        {
          যদি (BASE64_দৈর্ঘ্য (j / 8) == digest_base64_bytes + trailing_equals
              && j % 3 == trailing_equals)
            {
              digest_hex_bytes = j / 4;
              বিরতি;
            }
        }
    }
# endif
  যদি (digest_hex_bytes < 2 || digest_hex_bytes % 2
      || DIGEST_MAX_LEN * 2 < digest_hex_bytes)
    ফেরত false;
  digest_দৈর্ঘ্য = digest_hex_bytes * 4;
# যদি HASH_ALGO_CKSUM
  }
# endif
#endif

  /* This field must be the hexadecimal or base64 representation
     of the message digest.  */
  যখন (s[i] && !c_isblank (s[i]))
    i++;

  /* The digest must be followed by at least one whitespace character.  */
  যদি (i == s_len)
    ফেরত false;

  *d_len = &s[i] - (অক্ষর *) *digest;
  s[i++] = '\0';

  যদি (! valid_digits (*digest, *d_len))
    ফেরত false;

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
        ফেরত false;
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
    ফেরত filename_unescape (&s[i], s_len - i) != ফাঁকা;

  ফেরত true;
}

#যদি !HASH_ALGO_যোগ
/* If ESCAPE is true, then translate each:
   NEWLINE byte to the string, "\\n",
   CARRIAGE RETURN byte to the string, "\\r",
   and each backslash to "\\\\".  */
স্থির শূন্য
print_filename (অক্ষর ধ্রুবক *file, bool escape)
{
  যদি (! escape)
    {
      fputs (file, stdout);
      ফেরত;
    }

  যখন (*file)
    {
      নির্বাচন (*file)
        {
        ক্ষেত্র '\n':
          fputs ("\\n", stdout);
          বিরতি;

        ক্ষেত্র '\r':
          fputs ("\\r", stdout);
          বিরতি;

        ক্ষেত্র '\\':
          fputs ("\\\\", stdout);
          বিরতি;

        পূর্বনির্ধারিত:
          putchar (*file);
          বিরতি;
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
digest_file (অক্ষর ধ্রুবক *filename, পূর্ণসংখ্যা *দ্বিমিক, unsigned অক্ষর *bin_ফলাফল,
             bool *missing, MAYBE_UNUSED intmax_t *দৈর্ঘ্য)
{
  FILE *fp;
  পূর্ণসংখ্যা err;
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
      যদি (fp == ফাঁকা)
        {
          যদি (ignore_missing && errno == ENOENT)
            {
              *missing = true;
              ফেরত true;
            }
          error (0, errno, "%s", quotef (filename));
          ফেরত false;
        }
    }

  fadvise (fp, FADVISE_SEQUENTIAL);

#যদি HASH_ALGO_CKSUM
  যদি (cksum_algorithm == blake2b
      || cksum_algorithm == sha2 || cksum_algorithm == sha3)
    *দৈর্ঘ্য = digest_দৈর্ঘ্য >> 3;
  err = DIGEST_STREAM (fp, bin_ফলাফল, দৈর্ঘ্য);
#নতুবা_যদি HASH_ALGO_যোগ
  err = DIGEST_STREAM (fp, bin_ফলাফল, দৈর্ঘ্য);
#নতুবা_যদি HASH_ALGO_BLAKE2
  err = DIGEST_STREAM (fp, bin_ফলাফল, digest_দৈর্ঘ্য >> 3);
#অন্যথা
  err = DIGEST_STREAM (fp, bin_ফলাফল);
#endif
  err = err ? errno : 0;
  যদি (is_stdin)
    clearerr (fp);
  অন্যথা যদি (fclose (fp) != 0 && !err)
    err = errno;

  যদি (err)
    {
      error (0, err, "%s", quotef (filename));
      ফেরত false;
    }

  ফেরত true;
}

#যদি !HASH_ALGO_যোগ
স্থির শূন্য
output_file (অক্ষর ধ্রুবক *file, পূর্ণসংখ্যা দ্বিমিক_file, শূন্য ধ্রুবক *digest,
             MAYBE_UNUSED bool raw, bool tagged, unsigned অক্ষর delim,
             MAYBE_UNUSED bool args, MAYBE_UNUSED intmax_t দৈর্ঘ্য)
{
# যদি HASH_ALGO_CKSUM
  যদি (raw)
    {
      fwrite (digest, 1, digest_দৈর্ঘ্য >> 3, stdout);
      ফেরত;
    }
# endif

  unsigned অক্ষর ধ্রুবক *bin_বাফার = digest;

  /* Output a leading backslash if the file name contains problematic chars.  */
  bool needs_escape = delim == '\n' && problematic_chars (file);

  যদি (needs_escape)
    putchar ('\\');

  যদি (tagged)
    {
# যদি HASH_ALGO_CKSUM
      যদি (cksum_algorithm == sha2)
        printf ("SHA%u", (unsigned পূর্ণসংখ্যা) digest_দৈর্ঘ্য);
      অন্যথা
# endif
      fputs (DIGEST_TYPE_STRING, stdout);
# যদি HASH_ALGO_BLAKE2
      যদি (digest_দৈর্ঘ্য < DIGEST_MAX_LEN * 8)
        printf ("-%u", (unsigned পূর্ণসংখ্যা) digest_দৈর্ঘ্য);
# elif HASH_ALGO_CKSUM
      যদি (cksum_algorithm == sha3)
        printf ("-%u", (unsigned পূর্ণসংখ্যা) digest_দৈর্ঘ্য);
      যদি (cksum_algorithm == blake2b)
        {
          যদি (digest_দৈর্ঘ্য < DIGEST_MAX_LEN * 8)
            printf ("-%u", (unsigned পূর্ণসংখ্যা) digest_দৈর্ঘ্য);
        }
# endif
      fputs (" (", stdout);
      print_filename (file, needs_escape);
      fputs (") = ", stdout);
    }

# যদি HASH_ALGO_CKSUM
  যদি (base64_digest)
    {
      অক্ষর b64[BASE64_দৈর্ঘ্য (DIGEST_BIN_BYTES) + 1];
      base64_encode ((অক্ষর ধ্রুবক *) bin_বাফার, digest_দৈর্ঘ্য >> 3,
                     b64, আকার b64);
      fputs (b64, stdout);
    }
  অন্যথা
# endif
    {
      জন্য (idx_t i = 0; i < (digest_hex_bytes >> 1); ++i)
        printf ("%02x", bin_বাফার[i]);
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
b64_equal (unsigned অক্ষর ধ্রুবক *b64_digest, unsigned অক্ষর ধ্রুবক *bin_বাফার)
{
  idx_t b64_n_bytes = BASE64_দৈর্ঘ্য (digest_দৈর্ঘ্য >> 3);
  অক্ষর b64[BASE64_দৈর্ঘ্য (DIGEST_BIN_BYTES) + 1];
  base64_encode ((অক্ষর ধ্রুবক *) bin_বাফার, digest_দৈর্ঘ্য >> 3,
                 b64, আকার b64);
  ফেরত memeq (b64_digest, b64, b64_n_bytes + 1);
}
#endif

/* Return true if HEX_DIGEST is the same as the hex-encoded digest of the
   DIGEST_LENGTH/8 bytes at BIN_BUFFER.  */
স্থির bool
hex_equal (unsigned অক্ষর ধ্রুবক *hex_digest, unsigned অক্ষর ধ্রুবক *bin_বাফার)
{
  স্থির ধ্রুবক অক্ষর bin2hex[] = { '0', '1', '2', '3',
                                  '4', '5', '6', '7',
                                  '8', '9', 'a', 'b',
                                  'c', 'd', 'e', 'f' };
  idx_t digest_bin_bytes = digest_hex_bytes >> 1;

  /* Compare generated binary number with text representation
     in check file.  Ignore case of hex digits.  */
  idx_t cnt;
  জন্য (cnt = 0; cnt < digest_bin_bytes; ++cnt)
    {
      যদি (c_tolower (hex_digest[2 * cnt])
          != bin2hex[bin_বাফার[cnt] >> 4]
          || (c_tolower (hex_digest[2 * cnt + 1])
              != (bin2hex[bin_বাফার[cnt] & 0xf])))
        বিরতি;
    }
  ফেরত cnt == digest_bin_bytes;
}

স্থির bool
digest_check (অক্ষর ধ্রুবক *checkfile_name)
{
  FILE *checkfile_stream;
  intmax_t n_misformatted_সারিগুলি = 0;
  intmax_t n_mismatched_checksums = 0;
  intmax_t n_open_or_read_failures = 0;
  bool properly_formatted_সারিগুলি = false;
  bool matched_checksums = false;
  unsigned অক্ষর bin_বাফার_unaligned[DIGEST_BIN_BYTES + DIGEST_ALIGN];
  /* Make sure bin_buffer is properly aligned. */
  unsigned অক্ষর *bin_বাফার = ptr_align (bin_বাফার_unaligned, DIGEST_ALIGN);
  intmax_t সারি_সংখ্যা;
  অক্ষর *সারি;
  size_t সারি_chars_allocated;
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
      যদি (checkfile_stream == ফাঁকা)
        {
          error (0, errno, "%s", quotef (checkfile_name));
          ফেরত false;
        }
    }

  সারি_সংখ্যা = 0;
  সারি = ফাঁকা;
  সারি_chars_allocated = 0;
  করো
    {
      অক্ষর *filename;
      পূর্ণসংখ্যা দ্বিমিক;
      unsigned অক্ষর *digest;
      ssize_t সারি_দৈর্ঘ্য;

      ++সারি_সংখ্যা;
      যদি (সারি_সংখ্যা == 0)
        error (EXIT_FAILURE, 0, _("%s: too many checksum lines"),
               quotef (checkfile_name));

      সারি_দৈর্ঘ্য = getline (&সারি, &সারি_chars_allocated, checkfile_stream);
      যদি (সারি_দৈর্ঘ্য <= 0)
        বিরতি;

      /* Ignore comment lines, which begin with a '#' character.  */
      যদি (সারি[0] == '#')
        চালু;

      /* Remove any trailing newline.  */
      সারি_দৈর্ঘ্য -= সারি[সারি_দৈর্ঘ্য - 1] == '\n';
      /* Remove any trailing carriage return.  */
      সারি_দৈর্ঘ্য -= সারি[সারি_দৈর্ঘ্য - (0 < সারি_দৈর্ঘ্য)] == '\r';

      /* Ignore empty lines.  */
      যদি (সারি_দৈর্ঘ্য == 0)
        চালু;

      সারি[সারি_দৈর্ঘ্য] = '\0';

      idx_t d_len;
      যদি (! (split_3 (সারি, সারি_দৈর্ঘ্য, &digest, &d_len, &দ্বিমিক, &filename)
             && ! (is_stdin && streq (filename, "-"))))
        {
          ++n_misformatted_সারিগুলি;

          যদি (warn)
            {
              error (0, 0,
                     _("%s: %jd"
                       ": improperly formatted %s checksum line"),
                     quotef (checkfile_name), সারি_সংখ্যা,
                     DIGEST_TYPE_STRING);
            }
        }
      অন্যথা
        {
          bool ok;
          bool missing;

          properly_formatted_সারিগুলি = true;

          intmax_t দৈর্ঘ্য;
          ok = digest_file (filename, &দ্বিমিক, bin_বাফার, &missing, &দৈর্ঘ্য);

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
              যদি (d_len == BASE64_দৈর্ঘ্য (digest_দৈর্ঘ্য >> 3))
                match = b64_equal (digest, bin_বাফার);
              অন্যথা
#endif
                যদি (d_len == digest_hex_bytes)
                  match = hex_equal (digest, bin_বাফার);

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
  যখন (!feof (checkfile_stream) && !ferror (checkfile_stream));

  মুক্ত (সারি);

  পূর্ণসংখ্যা err = ferror (checkfile_stream) ? 0 : -1;
  যদি (is_stdin)
    clearerr (checkfile_stream);
  অন্যথা যদি (fclose (checkfile_stream) != 0 && err < 0)
    err = errno;

  যদি (0 <= err)
    {
      error (0, err, err ? "%s" : _("%s: read error"),
             quotef (checkfile_name));
      ফেরত false;
    }

  যদি (! properly_formatted_সারিগুলি)
    {
      /* Warn if no tests are found.  */
      error (0, 0, _("%s: no properly formatted checksum lines found"),
             quotef (checkfile_name));
    }
  অন্যথা
    {
      যদি (!status_only)
        {
          যদি (n_misformatted_সারিগুলি != 0)
            error (0, 0,
                   (ngettext
                    ("WARNING: %jd line is improperly formatted",
                     "WARNING: %jd lines are improperly formatted",
                     select_plural (n_misformatted_সারিগুলি))),
                   n_misformatted_সারিগুলি);

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

  ফেরত (properly_formatted_সারিগুলি
          && matched_checksums
          && n_mismatched_checksums == 0
          && n_open_or_read_failures == 0
          && (!strict || n_misformatted_সারিগুলি == 0));
}

পূর্ণসংখ্যা
প্রধান (পূর্ণসংখ্যা argc, অক্ষর **argv)
{
  unsigned অক্ষর bin_বাফার_unaligned[DIGEST_BIN_BYTES + DIGEST_ALIGN];
  /* Make sure bin_buffer is properly aligned. */
  unsigned অক্ষর *bin_বাফার = ptr_align (bin_বাফার_unaligned, DIGEST_ALIGN);
  bool do_check = false;
  পূর্ণসংখ্যা opt;
  bool ok = true;
  পূর্ণসংখ্যা দ্বিমিক = -1;
  পূর্ণসংখ্যা prefix_tag = -1;
  কাঠামো option ধ্রুবক *long_opts = long_options;

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
  setvbuf (stdout, ফাঁকা, _IOLBF, 0);

#যদি HASH_ALGO_যোগ
  অক্ষর ধ্রুবক *short_opts = "rs";
#নতুবা_যদি HASH_ALGO_CKSUM
  অক্ষর ধ্রুবক *short_opts = legacy_mode ? "bctwz" : "a:l:bctwz";
  অক্ষর ধ্রুবক *digest_দৈর্ঘ্য_str = "";
#নতুবা_যদি HASH_ALGO_BLAKE2
  অক্ষর ধ্রুবক *short_opts = "l:bctwz";
  অক্ষর ধ্রুবক *digest_দৈর্ঘ্য_str = "";
#অন্যথা
  অক্ষর ধ্রুবক *short_opts = "bctwz";
#endif

  যখন ((opt = getopt_long (argc, argv, short_opts, long_opts, ফাঁকা))
         != -1)
    নির্বাচন (opt)
      {
#যদি HASH_ALGO_CKSUM
      ক্ষেত্র 'a':
        cksum_algorithm = XARGMATCH_EXACT ("--algorithm", optarg,
                                           algorithm_args, algorithm_types);
        algorithm_specified = true;
        বিরতি;

      ক্ষেত্র DEBUG_PROGRAM_OPTION:
        cksum_debug = true;
        বিরতি;
#endif
#যদি HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
      ক্ষেত্র 'l':
        digest_দৈর্ঘ্য = xnumtoimax (optarg, 10, 0, IDX_MAX, "",
                                    _("invalid length"), 0,
                                    XTOINT_MAX_QUIET);
        digest_দৈর্ঘ্য_str = optarg;
        বিরতি;
#endif
#যদি !HASH_ALGO_যোগ
      ক্ষেত্র 'c':
        do_check = true;
        বিরতি;
      ক্ষেত্র STATUS_OPTION:
        status_only = true;
        warn = false;
        quiet = false;
        বিরতি;
      ক্ষেত্র 'b':
        দ্বিমিক = 1;
        বিরতি;
      ক্ষেত্র 't':
        দ্বিমিক = 0;
        বিরতি;
      ক্ষেত্র 'w':
        status_only = false;
        warn = true;
        quiet = false;
        বিরতি;
      ক্ষেত্র IGNORE_MISSING_OPTION:
        ignore_missing = true;
        বিরতি;
      ক্ষেত্র QUIET_OPTION:
        status_only = false;
        warn = false;
        quiet = true;
        বিরতি;
      ক্ষেত্র STRICT_OPTION:
        strict = true;
        বিরতি;
# যদি HASH_ALGO_CKSUM
      ক্ষেত্র BASE64_OPTION:
        base64_digest = true;
        বিরতি;
      ক্ষেত্র RAW_OPTION:
        raw_digest = true;
        বিরতি;
      ক্ষেত্র UNTAG_OPTION:
        prefix_tag = 0;
        বিরতি;
# endif
      ক্ষেত্র TAG_OPTION:
        prefix_tag = 1;
        বিরতি;
      ক্ষেত্র 'z':
        digest_delim = '\0';
        বিরতি;
#endif
#যদি HASH_ALGO_যোগ
      ক্ষেত্র 'r':		/* For SysV compatibility. */
        যোগ_algorithm = bsd;
        বিরতি;

      ক্ষেত্র 's':
        যোগ_algorithm = sysv;
        বিরতি;
#endif
      case_GETOPT_HELP_CHAR;
      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
      পূর্বনির্ধারিত:
        usage (EXIT_FAILURE);
      }

  min_digest_সারি_দৈর্ঘ্য = MIN_DIGEST_সারি_দৈর্ঘ্য;
#যদি HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# যদি HASH_ALGO_CKSUM
  যদি (digest_দৈর্ঘ্য && (cksum_algorithm != blake2b
                        && cksum_algorithm != sha2
                        && cksum_algorithm != sha3))
    error (EXIT_FAILURE, 0,
           _("--length is only supported with --algorithm "
             "blake2b, sha2, or sha3"));
  যদি (cksum_algorithm == sha2 || cksum_algorithm == sha3)
    {
      /* Do not require --length with --check.  */
      যদি (digest_দৈর্ঘ্য == 0 && *digest_দৈর্ঘ্য_str == '\0' && ! do_check)
        error (EXIT_FAILURE, 0, _("--algorithm=%s requires specifying "
                                  "--length 224, 256, 384, or 512"),
               algorithm_args[cksum_algorithm]);
      /* If --check and --length are used we verify the digest length.  */
      যদি ((! do_check || *digest_দৈর্ঘ্য_str != '\0')
          && digest_দৈর্ঘ্য != SHA224_DIGEST_আকার * 8
          && digest_দৈর্ঘ্য != SHA256_DIGEST_আকার * 8
          && digest_দৈর্ঘ্য != SHA384_DIGEST_আকার * 8
          && digest_দৈর্ঘ্য != SHA512_DIGEST_আকার * 8)
        {
          error (0, 0, _("invalid length: %s"), quote (digest_দৈর্ঘ্য_str));
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
      যদি (digest_দৈর্ঘ্য > DIGEST_MAX_LEN * 8)
        {
          error (0, 0, _("invalid length: %s"), quote (digest_দৈর্ঘ্য_str));
          error (EXIT_FAILURE, 0,
                 _("maximum digest length for %s is %d bits"),
                 quote (DIGEST_TYPE_STRING),
                 DIGEST_MAX_LEN * 8);
        }
      যদি (digest_দৈর্ঘ্য % 8 != 0)
        {
          error (0, 0, _("invalid length: %s"), quote (digest_দৈর্ঘ্য_str));
          error (EXIT_FAILURE, 0, _("length is not a multiple of 8"));
        }
    }
  যদি (digest_দৈর্ঘ্য == 0)
    {
# যদি HASH_ALGO_BLAKE2
      digest_দৈর্ঘ্য = DIGEST_MAX_LEN * 8;
# অন্যথা
      digest_দৈর্ঘ্য = algorithm_bits[cksum_algorithm];
# endif
    }
  digest_hex_bytes = digest_দৈর্ঘ্য >> 2;
#অন্যথা
  digest_hex_bytes = DIGEST_HEX_BYTES;
#endif

#যদি HASH_ALGO_CKSUM
  নির্বাচন (+cksum_algorithm)
    {
    ক্ষেত্র bsd:
    ক্ষেত্র sysv:
    ক্ষেত্র crc:
    ক্ষেত্র crc32b:
        যদি (do_check && algorithm_specified)
          error (EXIT_FAILURE, 0,
                 _("--check is not supported with "
                   "--algorithm={bsd,sysv,crc,crc32b}"));
        বিরতি;
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

  অক্ষর **operand_lim = argv + argc;
  যদি (optind == argc)
    *operand_lim++ = bad_cast ("-");
  অন্যথা যদি (1 < argc - optind && raw_digest)
    error (EXIT_FAILURE, 0,
           _("the --raw option is not supported with multiple files"));

  জন্য (অক্ষর **operandp = argv + optind; operandp < operand_lim; operandp++)
    {
      অক্ষর *file = *operandp;
      যদি (do_check)
        ok &= digest_check (file);
      অন্যথা
        {
          পূর্ণসংখ্যা দ্বিমিক_file = দ্বিমিক;
          bool missing;
          intmax_t দৈর্ঘ্য;

          যদি (! digest_file (file, &দ্বিমিক_file, bin_বাফার, &missing, &দৈর্ঘ্য))
            ok = false;
          অন্যথা
            {
              DIGEST_OUT (file, দ্বিমিক_file, bin_বাফার, raw_digest, prefix_tag,
                          digest_delim, optind != argc, দৈর্ঘ্য);
              যদি (ferror (stdout))
                write_error ();
            }
        }
    }

  যদি (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, _("standard input"));

  ফেরত ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
