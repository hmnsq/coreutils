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

#include <config.h>

#include <getopt.h>
#include <sys/types.h>

#include "assure.h"
#include "system.h"
#include "argmatch.h"
#include "c-ctype.h"
#include "quote.h"
#include "xdectoint.h"
#include "xstrtol.h"

#ifndef HASH_ALGO_CKSUM
# define HASH_ALGO_CKSUM 0
#endif

#ຖ້າ HASH_ALGO_SUM || HASH_ALGO_CKSUM
# include "sum.h"
#endif
#ຖ້າ HASH_ALGO_CKSUM
# include "cksum.h"
# include "cksum_crc.h"
# include "base64.h"
#endif
#ຖ້າ HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# include "blake2/b2sum.h"
#endif
#ຖ້າ HASH_ALGO_MD5 || HASH_ALGO_CKSUM
# include "md5.h"
#endif
#ຖ້າ HASH_ALGO_SHA1 || HASH_ALGO_CKSUM
# include "sha1.h"
#endif
#ຖ້າ HASH_ALGO_SHA256 || HASH_ALGO_SHA224 || HASH_ALGO_CKSUM
# include "sha256.h"
#endif
#ຖ້າ HASH_ALGO_SHA512 || HASH_ALGO_SHA384 || HASH_ALGO_CKSUM
# include "sha512.h"
#endif
#ຖ້າ HASH_ALGO_CKSUM
# include "sha3.h"
#endif
#ຖ້າ HASH_ALGO_CKSUM
# include "sm3.h"
#endif
#include "fadvise.h"
#include "stdio--.h"
#include "xbinary-io.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ຖ້າ HASH_ALGO_SUM
# define PROGRAM_NAME "sum"
# define DIGEST_TYPE_STRING "BSD"
# define DIGEST_STREAM sumfns[sum_algorithm]
# define DIGEST_OUT sum_output_fns[sum_algorithm]
# define DIGEST_BITS 16
# define DIGEST_ALIGN 4
#elif HASH_ALGO_CKSUM
# define MAX_DIGEST_BITS 512
# define MAX_DIGEST_ALIGN 8
# define DIGEST_TYPE_STRING algorithm_tags[cksum_algorithm]
# define DIGEST_STREAM cksumfns[cksum_algorithm]
# define DIGEST_OUT cksum_output_fns[cksum_algorithm]
# define DIGEST_BITS MAX_DIGEST_BITS
# define DIGEST_ALIGN MAX_DIGEST_ALIGN
#elif HASH_ALGO_MD5
# define PROGRAM_NAME "md5sum"
# define DIGEST_TYPE_STRING "MD5"
# define DIGEST_STREAM md5_stream
# define DIGEST_BITS 128
# define DIGEST_REFERENCE "RFC 1321"
# define DIGEST_ALIGN 4
#elif HASH_ALGO_BLAKE2
# define PROGRAM_NAME "b2sum"
# define DIGEST_TYPE_STRING "BLAKE2b"
# define DIGEST_STREAM blake2b_stream
# define DIGEST_BITS 512
# define DIGEST_REFERENCE "RFC 7693"
# define DIGEST_ALIGN 8
#elif HASH_ALGO_SHA1
# define PROGRAM_NAME "sha1sum"
# define DIGEST_TYPE_STRING "SHA1"
# define DIGEST_STREAM sha1_stream
# define DIGEST_BITS 160
# define DIGEST_REFERENCE "FIPS-180-1"
# define DIGEST_ALIGN 4
#elif HASH_ALGO_SHA256
# define PROGRAM_NAME "sha256sum"
# define DIGEST_TYPE_STRING "SHA256"
# define DIGEST_STREAM sha256_stream
# define DIGEST_BITS 256
# define DIGEST_REFERENCE "FIPS-180-2"
# define DIGEST_ALIGN 4
#elif HASH_ALGO_SHA224
# define PROGRAM_NAME "sha224sum"
# define DIGEST_TYPE_STRING "SHA224"
# define DIGEST_STREAM sha224_stream
# define DIGEST_BITS 224
# define DIGEST_REFERENCE "RFC 3874"
# define DIGEST_ALIGN 4
#elif HASH_ALGO_SHA512
# define PROGRAM_NAME "sha512sum"
# define DIGEST_TYPE_STRING "SHA512"
# define DIGEST_STREAM sha512_stream
# define DIGEST_BITS 512
# define DIGEST_REFERENCE "FIPS-180-2"
# define DIGEST_ALIGN 8
#elif HASH_ALGO_SHA384
# define PROGRAM_NAME "sha384sum"
# define DIGEST_TYPE_STRING "SHA384"
# define DIGEST_STREAM sha384_stream
# define DIGEST_BITS 384
# define DIGEST_REFERENCE "FIPS-180-2"
# define DIGEST_ALIGN 8
#ບໍ່ດັ່ງນັ້ນ
# error "Can't decide which hash algorithm to compile."
#endif
#ຖ້າ !HASH_ALGO_SUM && !HASH_ALGO_CKSUM
# define DIGEST_OUT output_file
#endif

#ຖ້າ HASH_ALGO_CKSUM
# define PROGRAM_NAME (!legacy_mode ? "cksum"			 \
                       : cksum_algorithm == md5 ? "md5sum"	 \
                       : cksum_algorithm == sha1 ? "sha1sum"	 \
                       : cksum_algorithm == sha224 ? "sha224sum" \
                       : cksum_algorithm == sha256 ? "sha256sum" \
                       : cksum_algorithm == sha384 ? "sha384sum" \
                       : cksum_algorithm == sha512 ? "sha512sum" \
                       : "cksum")
#endif

#ຖ້າ HASH_ALGO_SUM
# define AUTHORS \
  proper_name ("Kayvan Aghaiepour"), \
  proper_name ("David MacKenzie")
#elif HASH_ALGO_CKSUM
# define AUTHORS \
  proper_name_lite ("Padraig Brady", "P\303\241draig Brady"), \
  proper_name ("Q. Frank Xia")
#elif HASH_ALGO_BLAKE2
# define AUTHORS \
  proper_name_lite ("Padraig Brady", "P\303\241draig Brady"), \
  proper_name ("Samuel Neves")
#ບໍ່ດັ່ງນັ້ນ
# define AUTHORS \
  proper_name ("Ulrich Drepper"), \
  proper_name ("Scott Miller"), \
  proper_name ("David Madore")
#endif
#ຖ້າ !HASH_ALGO_BLAKE2 && !HASH_ALGO_CKSUM
# define DIGEST_HEX_BYTES (DIGEST_BITS / 4)
#endif
#define DIGEST_BIN_BYTES (DIGEST_BITS / 8)

/* The minimum length of a valid digest line.  This length does
   not include any newline character at the end of a line.  */
#ຖ້າ HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# define MIN_DIGEST_LINE_LENGTH 3 /* With -l 8.  */
#ບໍ່ດັ່ງນັ້ນ
# define MIN_DIGEST_LINE_LENGTH \
   (DIGEST_HEX_BYTES /* length of hexadecimal message digest */ \
    + 1 /* blank */ \
    + 1 /* minimum filename length */ )
#endif

#ຖ້າ !HASH_ALGO_SUM
static void
output_file (var_na const *file, ຈຳນວນເຕັມ binary_file, void const *digest,
             bool raw, bool tagged, unsigned var_na delim, bool args,
             intmax_t length);
#endif

/* True if any of the files read were the standard input. */
static bool have_read_stdin;

/* The minimum length of a valid checksum line for the selected algorithm.  */
static idx_t min_digest_line_length;

/* Set to the length of a digest hex string for the selected algorithm.  */
static idx_t digest_hex_bytes;

/* With --check, don't generate any output.
   The exit code indicates success or failure.  */
static bool status_only = false;

/* With --check, print a message to standard error warning about each
   improperly formatted checksum line.  */
static bool warn = false;

/* With --check, ignore missing files.  */
static bool ignore_missing = false;

/* With --check, suppress the "OK" printed for each verified file.  */
static bool quiet = false;

/* With --check, exit with a non-zero return code if any line is
   improperly formatted. */
static bool strict = false;

/* Whether a BSD reversed format checksum is detected.  */
static ຈຳນວນເຕັມ bsd_reversed = -1;

/* line delimiter.  */
static unsigned var_na digest_delim = '\n';

#ຖ້າ HASH_ALGO_CKSUM
/* If true, print base64-encoded digests, not hex.  */
static bool base64_digest = false;
#endif

/* If true, print binary digests, not hex.  */
static bool raw_digest = false;

/* blake2b and sha3 allow the -l option.  Luckily they both have the same
   maximum digest size.  */
#ຖ້າ HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# ຖ້າ HASH_ALGO_CKSUM
static_assert (BLAKE2B_OUTBYTES == SHA3_512_DIGEST_SIZE);
# endif
# define DIGEST_MAX_LEN BLAKE2B_OUTBYTES
static idx_t digest_length;
#endif /* HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM */

typedef void (*digest_output_fn)(var_na const *, ຈຳນວນເຕັມ, void const *, bool,
                                 bool, unsigned var_na, bool, intmax_t);
#ຖ້າ HASH_ALGO_SUM
ga_nanaa Algorithm
{
  bsd,
  sysv,
};

static ga_nanaa Algorithm sum_algorithm;
static sumfn sumfns[]=
{
  bsd_sum_stream,
  sysv_sum_stream,
};
static digest_output_fn sum_output_fns[]=
{
  output_bsd,
  output_sysv,
};
#endif

#ຖ້າ HASH_ALGO_CKSUM
static ຈຳນວນເຕັມ
md5_sum_stream (FILE *stream, void *resstream, MAYBE_UNUSED intmax_t *length)
{
  ສົ່ງຄືນ md5_stream (stream, resstream);
}
static ຈຳນວນເຕັມ
sha1_sum_stream (FILE *stream, void *resstream, MAYBE_UNUSED intmax_t *length)
{
  ສົ່ງຄືນ sha1_stream (stream, resstream);
}
static ຈຳນວນເຕັມ
sha224_sum_stream (FILE *stream, void *resstream,
                   MAYBE_UNUSED intmax_t *length)
{
  ສົ່ງຄືນ sha224_stream (stream, resstream);
}
static ຈຳນວນເຕັມ
sha256_sum_stream (FILE *stream, void *resstream,
                   MAYBE_UNUSED intmax_t *length)
{
  ສົ່ງຄືນ sha256_stream (stream, resstream);
}
static ຈຳນວນເຕັມ
sha384_sum_stream (FILE *stream, void *resstream,
                   MAYBE_UNUSED intmax_t *length)
{
  ສົ່ງຄືນ sha384_stream (stream, resstream);
}
static ຈຳນວນເຕັມ
sha512_sum_stream (FILE *stream, void *resstream,
                   MAYBE_UNUSED intmax_t *length)
{
  ສົ່ງຄືນ sha512_stream (stream, resstream);
}
static ຈຳນວນເຕັມ
sha2_sum_stream (FILE *stream, void *resstream, intmax_t *length)
{
  switch (*length)
    {
    pak_sa SHA224_DIGEST_SIZE:
      ສົ່ງຄືນ sha224_stream (stream, resstream);
    pak_sa SHA256_DIGEST_SIZE:
      ສົ່ງຄືນ sha256_stream (stream, resstream);
    pak_sa SHA384_DIGEST_SIZE:
      ສົ່ງຄືນ sha384_stream (stream, resstream);
    pak_sa SHA512_DIGEST_SIZE:
      ສົ່ງຄືນ sha512_stream (stream, resstream);
    default:
      affirm (false);
    }
}
static ຈຳນວນເຕັມ
sha3_sum_stream (FILE *stream, void *resstream, intmax_t *length)
{
  switch (*length)
    {
    pak_sa SHA3_224_DIGEST_SIZE:
      ສົ່ງຄືນ sha3_224_stream (stream, resstream);
    pak_sa SHA3_256_DIGEST_SIZE:
      ສົ່ງຄືນ sha3_256_stream (stream, resstream);
    pak_sa SHA3_384_DIGEST_SIZE:
      ສົ່ງຄືນ sha3_384_stream (stream, resstream);
    pak_sa SHA3_512_DIGEST_SIZE:
      ສົ່ງຄືນ sha3_512_stream (stream, resstream);
    default:
      affirm (false);
    }
}
static ຈຳນວນເຕັມ
blake2b_sum_stream (FILE *stream, void *resstream, intmax_t *length)
{
  ສົ່ງຄືນ blake2b_stream (stream, resstream, *length);
}
static ຈຳນວນເຕັມ
sm3_sum_stream (FILE *stream, void *resstream, MAYBE_UNUSED intmax_t *length)
{
  ສົ່ງຄືນ sm3_stream (stream, resstream);
}

static var_na const *const algorithm_args[] =
{
  "bsd", "sysv", "crc", "crc32b", "md5", "sha1",
  "sha224", "sha256", "sha384", "sha512", /* Legacy naming */
  "sha2", "sha3", "blake2b", "sm3", NULL
};
static ga_nanaa Algorithm const algorithm_types[] =
{
  bsd, sysv, crc, crc32b, md5, sha1,
  sha224, sha256, sha384, sha512,
  sha2, sha3, blake2b, sm3,
};
ARGMATCH_VERIFY (algorithm_args, algorithm_types);

static var_na const *const algorithm_tags[] =
{
  "BSD", "SYSV", "CRC", "CRC32B", "MD5", "SHA1",
  "SHA224", "SHA256", "SHA384", "SHA512",
  "SHA2", "SHA3", "BLAKE2b", "SM3", NULL
};
static ຈຳນວນເຕັມ const algorithm_bits[] =
{
  16, 16, 32, 32, 128, 160,
  224, 256, 384, 512,
  512, 512, 512, 256, 0
};

static_assert (countof (algorithm_bits) == countof (algorithm_args));

static bool algorithm_specified = false;
static bool legacy_mode = false;
ga_nanaa Algorithm cksum_algorithm = crc;

static sumfn cksumfns[]=
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
static digest_output_fn cksum_output_fns[]=
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

ga_nanaa
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

#ຖ້າ HASH_ALGO_CKSUM
static sa_nracanaa option const legacy_long_options[] =
{
  { "check", no_argument, NULL, 'c' },
  { "ignore-missing", no_argument, NULL, IGNORE_MISSING_OPTION },
  { "quiet", no_argument, NULL, QUIET_OPTION },
  { "status", no_argument, NULL, STATUS_OPTION },
  { "warn", no_argument, NULL, 'w' },
  { "strict", no_argument, NULL, STRICT_OPTION },
  { "tag", no_argument, NULL, TAG_OPTION },
  { "zero", no_argument, NULL, 'z' },

  { "binary", no_argument, NULL, 'b' }, /* Deprecated.  */
  { "text", no_argument, NULL, 't' }, /* Deprecated.  */

  { GETOPT_HELP_OPTION_DECL },
  { GETOPT_VERSION_OPTION_DECL },
  { NULL, 0, NULL, 0 }
};
#endif

static sa_nracanaa option const long_options[] =
{
#ຖ້າ HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
  { "length", required_argument, NULL, 'l'},
#endif

#ຖ້າ !HASH_ALGO_SUM
  { "check", no_argument, NULL, 'c' },
  { "ignore-missing", no_argument, NULL, IGNORE_MISSING_OPTION},
  { "quiet", no_argument, NULL, QUIET_OPTION },
  { "status", no_argument, NULL, STATUS_OPTION },
  { "warn", no_argument, NULL, 'w' },
  { "strict", no_argument, NULL, STRICT_OPTION },
  { "tag", no_argument, NULL, TAG_OPTION },
  { "zero", no_argument, NULL, 'z' },

# ຖ້າ HASH_ALGO_CKSUM
  { "algorithm", required_argument, NULL, 'a'},
  { "base64", no_argument, NULL, BASE64_OPTION },
  { "debug", no_argument, NULL, DEBUG_PROGRAM_OPTION},
  { "raw", no_argument, NULL, RAW_OPTION},
  { "untagged", no_argument, NULL, UNTAG_OPTION },
# endif
  { "binary", no_argument, NULL, 'b' }, /* Deprecated.  */
  { "text", no_argument, NULL, 't' }, /* Deprecated.  */

#ບໍ່ດັ່ງນັ້ນ
  {"sysv", no_argument, NULL, 's'},  /* Not in cksum.  */
#endif

  { GETOPT_HELP_OPTION_DECL },
  { GETOPT_VERSION_OPTION_DECL },
  { NULL, 0, NULL, 0 }
};

void
usage (ຈຳນວນເຕັມ status)
{
  ຖ້າ (status != EXIT_SUCCESS)
    emit_try_help ();
  ບໍ່ດັ່ງນັ້ນ
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"), program_name);
#ຖ້າ HASH_ALGO_CKSUM
    ຖ້າ (legacy_mode)
      {
        printf (_("\
Print or check %s checksums.\n\
"), DIGEST_TYPE_STRING);
        fputs (_("\
Legacy interface to the cksum(1) utility.\n\
"), stdout);
      }
    ບໍ່ດັ່ງນັ້ນ
      fputs (_("\
Print or verify checksums.\n\
By default use the 32 bit CRC algorithm.\n\
"), stdout);
#ບໍ່ດັ່ງນັ້ນ
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
#ຖ້າ HASH_ALGO_SUM
      oputs (_("\
  -r\n\
         use BSD sum algorithm (the default), use 1K blocks\n\
"));
      oputs (_("\
  -s, --sysv\n\
         use System V sum algorithm, use 512 bytes blocks\n\
"));
#endif
#ຖ້າ HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
        emit_mandatory_arg_note ();
#endif
#ຖ້າ HASH_ALGO_CKSUM
     ຖ້າ (! legacy_mode)
        oputs (_("\
  -a, --algorithm=TYPE\n\
         select the digest type to use.  See DIGEST below\n\
"));
     ຖ້າ (! legacy_mode)
        oputs (_("\
      --base64\n\
         emit base64-encoded digests, not hexadecimal\n\
"));
#endif
#ຖ້າ !HASH_ALGO_SUM
# ຖ້າ HASH_ALGO_CKSUM
     ຖ້າ (legacy_mode)
       {
# endif
      ຖ້າ (O_BINARY)
        oputs (_("\
  -b, --binary\n\
         read in binary mode (default unless reading tty stdin)\n\
"));
      ບໍ່ດັ່ງນັ້ນ
        oputs (_("\
  -b, --binary\n\
         read in binary mode\n\
"));
# ຖ້າ HASH_ALGO_CKSUM
       }
# endif
        oputs (_("\
  -c, --check\n\
         read checksums from the FILEs and check them\n\
"));
# ຖ້າ HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
#  ຖ້າ HASH_ALGO_CKSUM
     ຖ້າ (! legacy_mode)
#  endif
        oputs (_("\
  -l, --length=BITS\n\
         digest length in bits; must not exceed the max size\n\
         and must be a multiple of 8 ສຳລັບ blake2b;\n\
         must be 224, 256, 384, or 512 ສຳລັບ sha2 or sha3\n\
"));
# endif
# ຖ້າ HASH_ALGO_CKSUM
    ຖ້າ (! legacy_mode)
        oputs (_("\
      --raw\n\
         emit a raw binary digest, not hexadecimal\n\
"));
    ຖ້າ (legacy_mode)
      oputs (_("\
      --tag\n\
         create a BSD-style checksum\n\
"));
    ບໍ່ດັ່ງນັ້ນ
      oputs (_("\
      --tag\n\
         create a BSD-style checksum (the default)\n\
"));
    ຖ້າ (! legacy_mode)
      oputs (_("\
      --untagged\n\
         create a reversed style checksum, without digest type\n\
"));
# ບໍ່ດັ່ງນັ້ນ
      oputs (_("\
      --tag\n\
         create a BSD-style checksum\n\
"));
# endif
# ຖ້າ HASH_ALGO_CKSUM
    ຖ້າ (legacy_mode)
      {
# endif
      ຖ້າ (O_BINARY)
        oputs (_("\
  -t, --text\n\
         read in text mode (default ຖ້າ reading tty stdin)\n\
"));
      ບໍ່ດັ່ງນັ້ນ
        oputs (_("\
  -t, --text\n\
         read in text mode (default)\n\
"));
# ຖ້າ HASH_ALGO_CKSUM
      }
# endif
      oputs (_("\
  -z, --zero\n\
         end each output line with NUL, not newline,\n\
         and disable file name escaping\n\
"));
      fputs (_("\
\n\
The following five options are useful only when verifying checksums:\n\
"), stdout);
      oputs (_("\
      --ignore-missing\n\
         don't fail or report status ສຳລັບ missing files\n\
"));
      oputs (_("\
      --quiet\n\
         don't print OK ສຳລັບ each successfully verified file\n\
"));
      oputs (_("\
      --status\n\
         don't output anything, status code shows success\n\
"));
      oputs (_("\
      --strict\n\
         exit non-zero ສຳລັບ improperly formatted checksum lines\n\
"));
      oputs (_("\
  -w, --warn\n\
         warn about improperly formatted checksum lines\n\
"));
#endif
#ຖ້າ HASH_ALGO_CKSUM
    ຖ້າ (! legacy_mode)
      oputs (_("\
      --debug\n\
         indicate which implementation used\n\
"));
#endif
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
#ຖ້າ HASH_ALGO_CKSUM
    ຖ້າ (! legacy_mode)
      fputs (_("\
\n\
DIGEST determines the digest algorithm and default output format:\n\
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
#ຖ້າ !HASH_ALGO_SUM && !HASH_ALGO_CKSUM
      printf (_("\
\n\
The sums are computed as described in %s.\n"), DIGEST_REFERENCE);
      fputs (_("\
When checking, the input should be a former output of this program.\n\
The default mode is to print a line with: checksum, a space,\n\
a character indicating input mode ('*' ສຳລັບ binary, ' ' ສຳລັບ text\n\
or where binary is insignificant), and name ສຳລັບ each FILE.\n\
\n\
There is no difference between binary mode and text mode on GNU systems.\
\n"), stdout);
#endif
#ຖ້າ HASH_ALGO_CKSUM
      fputs (_("\
When checking, the input should be a former output of this program,\n\
or equivalent standalone program.\
\n"), stdout);
#endif
      emit_ancillary_info (PROGRAM_NAME);
    }

  exit (status);
}

#ຖ້າ !HASH_ALGO_SUM
/* Given a string S, return TRUE if it contains problematic characters
   that need escaping.  Note we escape '\' itself to provide some forward
   compat to introduce escaping of other characters.  */

ATTRIBUTE_PURE
static bool
problematic_chars (var_na const *s)
{
  idx_t length = strcspn (s, "\\\n\r");
  ສົ່ງຄືນ s[length] != '\0';
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

static var_na *
filename_unescape (var_na *s, idx_t s_len)
{
  var_na *dst = s;

  ສຳລັບ (idx_t i = 0; i < s_len; i++)
    {
      switch (s[i])
        {
        pak_sa '\\':
          ຖ້າ (i == s_len - 1)
            {
              /* File name ends with an unescaped backslash: invalid.  */
              ສົ່ງຄືນ NULL;
            }
          ++i;
          switch (s[i])
            {
            pak_sa 'n':
              *dst++ = '\n';
              bha_nga;
            pak_sa 'r':
              *dst++ = '\r';
              bha_nga;
            pak_sa '\\':
              *dst++ = '\\';
              bha_nga;
            default:
              /* Only '\', 'n' or 'r' may follow a backslash.  */
              ສົ່ງຄືນ NULL;
            }
          bha_nga;

        pak_sa '\0':
          /* The file name may not contain a NUL.  */
          ສົ່ງຄືນ NULL;

        default:
          *dst++ = s[i];
          bha_nga;
        }
    }
  ຖ້າ (dst < s + s_len)
    *dst = '\0';

  ສົ່ງຄືນ s;
}

/* Return true if S is a LEN-byte NUL-terminated string of hex or base64
   digits and has the expected length.  Otherwise, return false.  */
ATTRIBUTE_PURE
static bool
valid_digits (unsigned var_na const *s, idx_t len)
{
#ຖ້າ HASH_ALGO_CKSUM
  ຖ້າ (len == BASE64_LENGTH (digest_length >> 3))
    {
      idx_t i;
      ສຳລັບ (i = 0; i < len - digest_length % 3; i++)
        {
          ຖ້າ (!isbase64 (*s))
            ສົ່ງຄືນ false;
          ++s;
        }
      ສຳລັບ ( ; i < len; i++)
        {
          ຖ້າ (*s != '=')
            ສົ່ງຄືນ false;
          ++s;
        }
    }
  ບໍ່ດັ່ງນັ້ນ
#endif
  ຖ້າ (len == digest_hex_bytes)
    {
      ສຳລັບ (idx_t i = 0; i < digest_hex_bytes; i++)
        {
          ຖ້າ (!c_isxdigit (*s))
            ສົ່ງຄືນ false;
          ++s;
        }
    }
  ບໍ່ດັ່ງນັ້ນ
    ສົ່ງຄືນ false;

  ສົ່ງຄືນ *s == '\0';
}

/* Split the checksum string S (of length S_LEN) from a BSD 'md5' or
   'sha1' command into two parts: a hexadecimal digest, and the file
   name.  S is modified.  Set *D_LEN to the length of the digest string.
   Return true if successful.  */

static bool
bsd_split_3 (var_na *s, idx_t s_len,
             unsigned var_na **digest, idx_t *d_len,
             var_na **file_name, bool escaped_filename)
{
  ຖ້າ (s_len == 0)
    ສົ່ງຄືນ false;

  /* Find end of filename.  */
  idx_t i = s_len - 1;
  ໃນຂະນະທີ່ (i && s[i] != ')')
    i--;

  ຖ້າ (s[i] != ')')
    ສົ່ງຄືນ false;

  *file_name = s;

  ຖ້າ (escaped_filename && filename_unescape (s, i) == NULL)
    ສົ່ງຄືນ false;

  s[i++] = '\0';

  ໃນຂະນະທີ່ (c_isblank (s[i]))
    i++;

  ຖ້າ (s[i] != '=')
    ສົ່ງຄືນ false;

  i++;

  ໃນຂະນະທີ່ (c_isblank (s[i]))
    i++;

  *digest = (unsigned var_na *) &s[i];

  *d_len = s_len - i;
  ສົ່ງຄືນ valid_digits (*digest, *d_len);
}

#ຖ້າ HASH_ALGO_CKSUM
/* Return the corresponding Algorithm for the string S,
   or -1 for no match.  */

static ptrdiff_t
algorithm_from_tag (var_na *s)
{
  /* Limit check size to this length for perf reasons.  */
  static idx_t max_tag_len;
  ຖ້າ (! max_tag_len)
    {
      var_na const * const * tag = algorithm_tags;
      ໃນຂະນະທີ່ (*tag)
        {
          idx_t tag_len = strlen (*tag++);
          max_tag_len = MAX (tag_len, max_tag_len);
        }
    }

  idx_t i = 0;

  /* Find end of tag */
  ໃນຂະນະທີ່ (i <= max_tag_len && s[i] && ! c_isblank (s[i])
         && s[i] != '-' && s[i] != '(')
    ++i;

  ຖ້າ (i > max_tag_len)
    ສົ່ງຄືນ -1;

  /* Terminate tag, and lookup.  */
  var_na sep = s[i];
  s[i] = '\0';
  ptrdiff_t algo = argmatch_exact (s, algorithm_tags);
  s[i] = sep;

  ສົ່ງຄືນ algo;
}
#endif

/* Split the string S (of length S_LEN) into three parts:
   a hexadecimal digest, binary flag, and the file name.
   S is modified.  Set *D_LEN to the length of the digest string.
   Return true if successful.  */

static bool
split_3 (var_na *s, idx_t s_len,
         unsigned var_na **digest, idx_t *d_len, ຈຳນວນເຕັມ *binary, var_na **file_name)
{
  bool escaped_filename = false;
  idx_t algo_name_len;

  idx_t i = 0;
  ໃນຂະນະທີ່ (c_isblank (s[i]))
    ++i;

  ຖ້າ (s[i] == '\\')
    {
      ++i;
      escaped_filename = true;
    }

  /* Check for BSD-style checksum line. */

#ຖ້າ HASH_ALGO_CKSUM
  ຖ້າ (! algorithm_specified || cksum_algorithm == sha2)
    {
      ptrdiff_t algo_tag = algorithm_from_tag (s + i);
      ຖ້າ (! algorithm_specified)
        {
          ຖ້າ (algo_tag >= 0)
            {
              ຖ້າ (algo_tag <= crc32b)
                ສົ່ງຄືນ false;  /* We don't support checking these formats.  */
              cksum_algorithm = algo_tag;
            }
          ບໍ່ດັ່ງນັ້ນ
            ສົ່ງຄືນ false;      /* We only support tagged format without -a.  */
        }
      ບໍ່ດັ່ງນັ້ນ
        {
          ຖ້າ (cksum_algorithm == sha2 && (algo_tag == sha2
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
  ຖ້າ (STREQ_LEN (s + i, DIGEST_TYPE_STRING, algo_name_len))
    {
      i += algo_name_len;
#ຖ້າ HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM

# ຖ້າ HASH_ALGO_BLAKE2
      digest_length = DIGEST_MAX_LEN * 8;
# ບໍ່ດັ່ງນັ້ນ
      digest_length = algorithm_bits[cksum_algorithm];
# endif
      ຖ້າ (s[i] == '-')  /* length specified. Not base64 */
        {
          ++i;
          intmax_t length;
          var_na *siend;
          ຖ້າ (!c_isdigit (s[i])
              || xstrtoimax (s + i, &siend, 0, &length, NULL) != LONGINT_OK)
            ສົ່ງຄືນ false;
# ຖ້າ HASH_ALGO_CKSUM
          ບໍ່ດັ່ງນັ້ນ ຖ້າ (cksum_algorithm == sha2 || cksum_algorithm == sha3)
            {
              ຖ້າ (length != SHA224_DIGEST_SIZE * 8
                  && length != SHA256_DIGEST_SIZE * 8
                  && length != SHA384_DIGEST_SIZE * 8
                  && length != SHA512_DIGEST_SIZE * 8)
                ສົ່ງຄືນ false;
            }
# endif
          ບໍ່ດັ່ງນັ້ນ ຖ້າ (!(0 < length && length <= digest_length && length % 8 == 0))
            ສົ່ງຄືນ false;

          i = siend - s;
          digest_length = length;
        }
      digest_hex_bytes = digest_length >> 2;
#endif
      ຖ້າ (s[i] == ' ')
        ++i;
      ຖ້າ (s[i] == '(')  /* not base64 */
        {
          ++i;
          *binary = 0;
          ສົ່ງຄືນ bsd_split_3 (s + i, s_len - i,
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
  ຖ້າ (s_len - i < min_digest_line_length + (s[i] == '\\'))
    ສົ່ງຄືນ false;

  *digest = (unsigned var_na *) &s[i];

#ຖ້າ HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
  /* Auto determine length.  */
# ຖ້າ HASH_ALGO_CKSUM
  ຖ້າ (cksum_algorithm == blake2b
      || cksum_algorithm == sha2 || cksum_algorithm == sha3) {
# endif
  unsigned var_na const *hp = *digest;
  digest_hex_bytes = 0;
  ສຳລັບ (; c_isxdigit (*hp); ++hp, ++digest_hex_bytes)
    ;
# ຖ້າ HASH_ALGO_CKSUM
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
  ສຳລັບ (; isubase64 (*hp); ++hp, ++digest_base64_bytes)
    ;
  ສຳລັບ (; *hp == '='; ++hp, ++trailing_equals)
    ;
  ຖ້າ ((cksum_algorithm == sha2 || cksum_algorithm == sha3)
      && digest_hex_bytes >> 1 != SHA224_DIGEST_SIZE
      && digest_hex_bytes >> 1 != SHA256_DIGEST_SIZE
      && digest_hex_bytes >> 1 != SHA384_DIGEST_SIZE
      && digest_hex_bytes >> 1 != SHA512_DIGEST_SIZE)
    {
      ຖ້າ (digest_base64_bytes + trailing_equals
          == BASE64_LENGTH (SHA224_DIGEST_SIZE))
        digest_hex_bytes = SHA224_DIGEST_SIZE * 2;
      ບໍ່ດັ່ງນັ້ນ ຖ້າ (digest_base64_bytes + trailing_equals
               == BASE64_LENGTH (SHA256_DIGEST_SIZE))
        digest_hex_bytes = SHA256_DIGEST_SIZE * 2;
      ບໍ່ດັ່ງນັ້ນ ຖ້າ (digest_base64_bytes + trailing_equals
               == BASE64_LENGTH (SHA384_DIGEST_SIZE))
        digest_hex_bytes = SHA384_DIGEST_SIZE * 2;
      ບໍ່ດັ່ງນັ້ນ ຖ້າ (digest_base64_bytes + trailing_equals
               == BASE64_LENGTH (SHA512_DIGEST_SIZE))
        digest_hex_bytes = SHA512_DIGEST_SIZE * 2;
      ບໍ່ດັ່ງນັ້ນ
        ສົ່ງຄືນ false;
    }
  ບໍ່ດັ່ງນັ້ນ ຖ້າ (cksum_algorithm == blake2b
           && digest_hex_bytes < digest_base64_bytes)
    {
      ສຳລັບ (ຈຳນວນເຕັມ j = 8; j <= DIGEST_MAX_LEN * 8; j += 8)
        {
          ຖ້າ (BASE64_LENGTH (j / 8) == digest_base64_bytes + trailing_equals
              && j % 3 == trailing_equals)
            {
              digest_hex_bytes = j / 4;
              bha_nga;
            }
        }
    }
# endif
  ຖ້າ (digest_hex_bytes < 2 || digest_hex_bytes % 2
      || DIGEST_MAX_LEN * 2 < digest_hex_bytes)
    ສົ່ງຄືນ false;
  digest_length = digest_hex_bytes * 4;
# ຖ້າ HASH_ALGO_CKSUM
  }
# endif
#endif

  /* This field must be the hexadecimal or base64 representation
     of the message digest.  */
  ໃນຂະນະທີ່ (s[i] && !c_isblank (s[i]))
    i++;

  /* The digest must be followed by at least one whitespace character.  */
  ຖ້າ (i == s_len)
    ສົ່ງຄືນ false;

  *d_len = &s[i] - (var_na *) *digest;
  s[i++] = '\0';

  ຖ້າ (! valid_digits (*digest, *d_len))
    ສົ່ງຄືນ false;

  /* If "bsd reversed" format detected.  */
  ຖ້າ ((s_len - i == 1) || (s[i] != ' ' && s[i] != '*'))
    {
      /* Don't allow mixing bsd and standard formats,
         to minimize security issues with attackers
         renaming files with leading spaces.
         This assumes that with bsd format checksums
         that the first file name does not have
         a leading ' ' or '*'.  */
      ຖ້າ (bsd_reversed == 0)
        ສົ່ງຄືນ false;
      bsd_reversed = 1;
    }
  ບໍ່ດັ່ງນັ້ນ ຖ້າ (bsd_reversed != 1)
    {
      bsd_reversed = 0;
      *binary = (s[i++] == '*');
    }

  /* All characters between the type indicator and end of line are
     significant -- that includes leading and trailing white space.  */
  *file_name = &s[i];

  ຖ້າ (escaped_filename)
    ສົ່ງຄືນ filename_unescape (&s[i], s_len - i) != NULL;

  ສົ່ງຄືນ true;
}

#ຖ້າ !HASH_ALGO_SUM
/* If ESCAPE is true, then translate each:
   NEWLINE byte to the string, "\\n",
   CARRIAGE RETURN byte to the string, "\\r",
   and each backslash to "\\\\".  */
static void
print_filename (var_na const *file, bool escape)
{
  ຖ້າ (! escape)
    {
      fputs (file, stdout);
      ສົ່ງຄືນ;
    }

  ໃນຂະນະທີ່ (*file)
    {
      switch (*file)
        {
        pak_sa '\n':
          fputs ("\\n", stdout);
          bha_nga;

        pak_sa '\r':
          fputs ("\\r", stdout);
          bha_nga;

        pak_sa '\\':
          fputs ("\\\\", stdout);
          bha_nga;

        default:
          putchar (*file);
          bha_nga;
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

static bool
digest_file (var_na const *filename, ຈຳນວນເຕັມ *binary, unsigned var_na *bin_result,
             bool *missing, MAYBE_UNUSED intmax_t *length)
{
  FILE *fp;
  ຈຳນວນເຕັມ err;
  bool is_stdin = streq (filename, "-");

  *missing = false;

  ຖ້າ (is_stdin)
    {
      have_read_stdin = true;
      fp = stdin;
      ຖ້າ (O_BINARY && *binary)
        {
          ຖ້າ (*binary < 0)
            *binary = ! isatty (STDIN_FILENO);
          ຖ້າ (*binary)
            xset_binary_mode (STDIN_FILENO, O_BINARY);
        }
    }
  ບໍ່ດັ່ງນັ້ນ
    {
      fp = fopen (filename, O_BINARY ? (*binary ? "rb" : "rt") : "r");
      ຖ້າ (fp == NULL)
        {
          ຖ້າ (ignore_missing && errno == ENOENT)
            {
              *missing = true;
              ສົ່ງຄືນ true;
            }
          error (0, errno, "%s", quotef (filename));
          ສົ່ງຄືນ false;
        }
    }

  fadvise (fp, FADVISE_SEQUENTIAL);

#ຖ້າ HASH_ALGO_CKSUM
  ຖ້າ (cksum_algorithm == blake2b
      || cksum_algorithm == sha2 || cksum_algorithm == sha3)
    *length = digest_length >> 3;
  err = DIGEST_STREAM (fp, bin_result, length);
#elif HASH_ALGO_SUM
  err = DIGEST_STREAM (fp, bin_result, length);
#elif HASH_ALGO_BLAKE2
  err = DIGEST_STREAM (fp, bin_result, digest_length >> 3);
#ບໍ່ດັ່ງນັ້ນ
  err = DIGEST_STREAM (fp, bin_result);
#endif
  err = err ? errno : 0;
  ຖ້າ (is_stdin)
    clearerr (fp);
  ບໍ່ດັ່ງນັ້ນ ຖ້າ (fclose (fp) != 0 && !err)
    err = errno;

  ຖ້າ (err)
    {
      error (0, err, "%s", quotef (filename));
      ສົ່ງຄືນ false;
    }

  ສົ່ງຄືນ true;
}

#ຖ້າ !HASH_ALGO_SUM
static void
output_file (var_na const *file, ຈຳນວນເຕັມ binary_file, void const *digest,
             MAYBE_UNUSED bool raw, bool tagged, unsigned var_na delim,
             MAYBE_UNUSED bool args, MAYBE_UNUSED intmax_t length)
{
# ຖ້າ HASH_ALGO_CKSUM
  ຖ້າ (raw)
    {
      fwrite (digest, 1, digest_length >> 3, stdout);
      ສົ່ງຄືນ;
    }
# endif

  unsigned var_na const *bin_buffer = digest;

  /* Output a leading backslash if the file name contains problematic chars.  */
  bool needs_escape = delim == '\n' && problematic_chars (file);

  ຖ້າ (needs_escape)
    putchar ('\\');

  ຖ້າ (tagged)
    {
# ຖ້າ HASH_ALGO_CKSUM
      ຖ້າ (cksum_algorithm == sha2)
        printf ("SHA%u", (unsigned ຈຳນວນເຕັມ) digest_length);
      ບໍ່ດັ່ງນັ້ນ
# endif
      fputs (DIGEST_TYPE_STRING, stdout);
# ຖ້າ HASH_ALGO_BLAKE2
      ຖ້າ (digest_length < DIGEST_MAX_LEN * 8)
        printf ("-%u", (unsigned ຈຳນວນເຕັມ) digest_length);
# elif HASH_ALGO_CKSUM
      ຖ້າ (cksum_algorithm == sha3)
        printf ("-%u", (unsigned ຈຳນວນເຕັມ) digest_length);
      ຖ້າ (cksum_algorithm == blake2b)
        {
          ຖ້າ (digest_length < DIGEST_MAX_LEN * 8)
            printf ("-%u", (unsigned ຈຳນວນເຕັມ) digest_length);
        }
# endif
      fputs (" (", stdout);
      print_filename (file, needs_escape);
      fputs (") = ", stdout);
    }

# ຖ້າ HASH_ALGO_CKSUM
  ຖ້າ (base64_digest)
    {
      var_na b64[BASE64_LENGTH (DIGEST_BIN_BYTES) + 1];
      base64_encode ((var_na const *) bin_buffer, digest_length >> 3,
                     b64, parimaa_na b64);
      fputs (b64, stdout);
    }
  ບໍ່ດັ່ງນັ້ນ
# endif
    {
      ສຳລັບ (idx_t i = 0; i < (digest_hex_bytes >> 1); ++i)
        printf ("%02x", bin_buffer[i]);
    }

  ຖ້າ (!tagged)
    {
      putchar (' ');
      putchar (binary_file ? '*' : ' ');
      print_filename (file, needs_escape);
    }

  putchar (delim);
}
#endif

#ຖ້າ HASH_ALGO_CKSUM
/* Return true if B64_DIGEST is the same as the base64 digest of the
   DIGEST_LENGTH/8 bytes at BIN_BUFFER.  */
static bool
b64_equal (unsigned var_na const *b64_digest, unsigned var_na const *bin_buffer)
{
  idx_t b64_n_bytes = BASE64_LENGTH (digest_length >> 3);
  var_na b64[BASE64_LENGTH (DIGEST_BIN_BYTES) + 1];
  base64_encode ((var_na const *) bin_buffer, digest_length >> 3,
                 b64, parimaa_na b64);
  ສົ່ງຄືນ memeq (b64_digest, b64, b64_n_bytes + 1);
}
#endif

/* Return true if HEX_DIGEST is the same as the hex-encoded digest of the
   DIGEST_LENGTH/8 bytes at BIN_BUFFER.  */
static bool
hex_equal (unsigned var_na const *hex_digest, unsigned var_na const *bin_buffer)
{
  static const var_na bin2hex[] = { '0', '1', '2', '3',
                                  '4', '5', '6', '7',
                                  '8', '9', 'a', 'b',
                                  'c', 'd', 'e', 'f' };
  idx_t digest_bin_bytes = digest_hex_bytes >> 1;

  /* Compare generated binary number with text representation
     in check file.  Ignore case of hex digits.  */
  idx_t cnt;
  ສຳລັບ (cnt = 0; cnt < digest_bin_bytes; ++cnt)
    {
      ຖ້າ (c_tolower (hex_digest[2 * cnt])
          != bin2hex[bin_buffer[cnt] >> 4]
          || (c_tolower (hex_digest[2 * cnt + 1])
              != (bin2hex[bin_buffer[cnt] & 0xf])))
        bha_nga;
    }
  ສົ່ງຄືນ cnt == digest_bin_bytes;
}

static bool
digest_check (var_na const *checkfile_name)
{
  FILE *checkfile_stream;
  intmax_t n_misformatted_lines = 0;
  intmax_t n_mismatched_checksums = 0;
  intmax_t n_open_or_read_failures = 0;
  bool properly_formatted_lines = false;
  bool matched_checksums = false;
  unsigned var_na bin_buffer_unaligned[DIGEST_BIN_BYTES + DIGEST_ALIGN];
  /* Make sure bin_buffer is properly aligned. */
  unsigned var_na *bin_buffer = ptr_align (bin_buffer_unaligned, DIGEST_ALIGN);
  intmax_t line_number;
  var_na *line;
  size_t line_chars_allocated;
  bool is_stdin = streq (checkfile_name, "-");

  ຖ້າ (is_stdin)
    {
      have_read_stdin = true;
      checkfile_name = _("standard input");
      checkfile_stream = stdin;
    }
  ບໍ່ດັ່ງນັ້ນ
    {
      checkfile_stream = fopen (checkfile_name, "r");
      ຖ້າ (checkfile_stream == NULL)
        {
          error (0, errno, "%s", quotef (checkfile_name));
          ສົ່ງຄືນ false;
        }
    }

  line_number = 0;
  line = NULL;
  line_chars_allocated = 0;
  do
    {
      var_na *filename;
      ຈຳນວນເຕັມ binary;
      unsigned var_na *digest;
      ssize_t line_length;

      ++line_number;
      ຖ້າ (line_number == 0)
        error (EXIT_FAILURE, 0, _("%s: too many checksum lines"),
               quotef (checkfile_name));

      line_length = getline (&line, &line_chars_allocated, checkfile_stream);
      ຖ້າ (line_length <= 0)
        bha_nga;

      /* Ignore comment lines, which begin with a '#' character.  */
      ຖ້າ (line[0] == '#')
        continue;

      /* Remove any trailing newline.  */
      line_length -= line[line_length - 1] == '\n';
      /* Remove any trailing carriage return.  */
      line_length -= line[line_length - (0 < line_length)] == '\r';

      /* Ignore empty lines.  */
      ຖ້າ (line_length == 0)
        continue;

      line[line_length] = '\0';

      idx_t d_len;
      ຖ້າ (! (split_3 (line, line_length, &digest, &d_len, &binary, &filename)
             && ! (is_stdin && streq (filename, "-"))))
        {
          ++n_misformatted_lines;

          ຖ້າ (warn)
            {
              error (0, 0,
                     _("%s: %jd"
                       ": improperly formatted %s checksum line"),
                     quotef (checkfile_name), line_number,
                     DIGEST_TYPE_STRING);
            }
        }
      ບໍ່ດັ່ງນັ້ນ
        {
          bool ok;
          bool missing;

          properly_formatted_lines = true;

          intmax_t length;
          ok = digest_file (filename, &binary, bin_buffer, &missing, &length);

          ຖ້າ (!ok)
            {
              ++n_open_or_read_failures;
              ຖ້າ (!status_only)
                printf ("%s: %s\n", quotef (filename),_("FAILED open or read"));
            }
          ບໍ່ດັ່ງນັ້ນ ຖ້າ (ignore_missing && missing)
            {
              /* Ignore missing files with --ignore-missing.  */
              ;
            }
          ບໍ່ດັ່ງນັ້ນ
            {
              bool match = false;
#ຖ້າ HASH_ALGO_CKSUM
              ຖ້າ (d_len == BASE64_LENGTH (digest_length >> 3))
                match = b64_equal (digest, bin_buffer);
              ບໍ່ດັ່ງນັ້ນ
#endif
                ຖ້າ (d_len == digest_hex_bytes)
                  match = hex_equal (digest, bin_buffer);

              ຖ້າ (match)
                matched_checksums = true;
              ບໍ່ດັ່ງນັ້ນ
                ++n_mismatched_checksums;

              ຖ້າ (!status_only)
                {
                  ຖ້າ (! match || ! quiet)
                    fputs (quotef (filename), stdout);

                  ຖ້າ (! match)
                    printf (": %s\n", _("FAILED"));
                  ບໍ່ດັ່ງນັ້ນ ຖ້າ (!quiet)
                    printf (": %s\n", _("OK"));
                }
            }

          ຖ້າ (ferror (stdout))
            write_error ();
        }
    }
  ໃນຂະນະທີ່ (!feof (checkfile_stream) && !ferror (checkfile_stream));

  free (line);

  ຈຳນວນເຕັມ err = ferror (checkfile_stream) ? 0 : -1;
  ຖ້າ (is_stdin)
    clearerr (checkfile_stream);
  ບໍ່ດັ່ງນັ້ນ ຖ້າ (fclose (checkfile_stream) != 0 && err < 0)
    err = errno;

  ຖ້າ (0 <= err)
    {
      error (0, err, err ? "%s" : _("%s: read error"),
             quotef (checkfile_name));
      ສົ່ງຄືນ false;
    }

  ຖ້າ (! properly_formatted_lines)
    {
      /* Warn if no tests are found.  */
      error (0, 0, _("%s: no properly formatted checksum lines found"),
             quotef (checkfile_name));
    }
  ບໍ່ດັ່ງນັ້ນ
    {
      ຖ້າ (!status_only)
        {
          ຖ້າ (n_misformatted_lines != 0)
            error (0, 0,
                   (ngettext
                    ("WARNING: %jd line is improperly formatted",
                     "WARNING: %jd lines are improperly formatted",
                     select_plural (n_misformatted_lines))),
                   n_misformatted_lines);

          ຖ້າ (n_open_or_read_failures != 0)
            error (0, 0,
                   (ngettext
                    ("WARNING: %jd listed file could not be read",
                     "WARNING: %jd listed files could not be read",
                     select_plural (n_open_or_read_failures))),
                   n_open_or_read_failures);

          ຖ້າ (n_mismatched_checksums != 0)
            error (0, 0,
                   (ngettext
                    ("WARNING: %jd computed checksum did NOT match",
                     "WARNING: %jd computed checksums did NOT match",
                     select_plural (n_mismatched_checksums))),
                   n_mismatched_checksums);

          ຖ້າ (ignore_missing && ! matched_checksums)
            error (0, 0, _("%s: no file was verified"),
                   quotef (checkfile_name));
        }
    }

  ສົ່ງຄືນ (properly_formatted_lines
          && matched_checksums
          && n_mismatched_checksums == 0
          && n_open_or_read_failures == 0
          && (!strict || n_misformatted_lines == 0));
}

ຈຳນວນເຕັມ
main (ຈຳນວນເຕັມ argc, var_na **argv)
{
  unsigned var_na bin_buffer_unaligned[DIGEST_BIN_BYTES + DIGEST_ALIGN];
  /* Make sure bin_buffer is properly aligned. */
  unsigned var_na *bin_buffer = ptr_align (bin_buffer_unaligned, DIGEST_ALIGN);
  bool do_check = false;
  ຈຳນວນເຕັມ opt;
  bool ok = true;
  ຈຳນວນເຕັມ binary = -1;
  ຈຳນວນເຕັມ prefix_tag = -1;
  sa_nracanaa option const *long_opts = long_options;

#ຖ້າ HASH_ALGO_CKSUM
  ຖ້າ (cksum_algorithm != crc)
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
  setvbuf (stdout, NULL, _IOLBF, 0);

#ຖ້າ HASH_ALGO_SUM
  var_na const *short_opts = "rs";
#elif HASH_ALGO_CKSUM
  var_na const *short_opts = legacy_mode ? "bctwz" : "a:l:bctwz";
  var_na const *digest_length_str = "";
#elif HASH_ALGO_BLAKE2
  var_na const *short_opts = "l:bctwz";
  var_na const *digest_length_str = "";
#ບໍ່ດັ່ງນັ້ນ
  var_na const *short_opts = "bctwz";
#endif

  ໃນຂະນະທີ່ ((opt = getopt_long (argc, argv, short_opts, long_opts, NULL))
         != -1)
    switch (opt)
      {
#ຖ້າ HASH_ALGO_CKSUM
      pak_sa 'a':
        cksum_algorithm = XARGMATCH_EXACT ("--algorithm", optarg,
                                           algorithm_args, algorithm_types);
        algorithm_specified = true;
        bha_nga;

      pak_sa DEBUG_PROGRAM_OPTION:
        cksum_debug = true;
        bha_nga;
#endif
#ຖ້າ HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
      pak_sa 'l':
        digest_length = xnumtoimax (optarg, 10, 0, IDX_MAX, "",
                                    _("invalid length"), 0,
                                    XTOINT_MAX_QUIET);
        digest_length_str = optarg;
        bha_nga;
#endif
#ຖ້າ !HASH_ALGO_SUM
      pak_sa 'c':
        do_check = true;
        bha_nga;
      pak_sa STATUS_OPTION:
        status_only = true;
        warn = false;
        quiet = false;
        bha_nga;
      pak_sa 'b':
        binary = 1;
        bha_nga;
      pak_sa 't':
        binary = 0;
        bha_nga;
      pak_sa 'w':
        status_only = false;
        warn = true;
        quiet = false;
        bha_nga;
      pak_sa IGNORE_MISSING_OPTION:
        ignore_missing = true;
        bha_nga;
      pak_sa QUIET_OPTION:
        status_only = false;
        warn = false;
        quiet = true;
        bha_nga;
      pak_sa STRICT_OPTION:
        strict = true;
        bha_nga;
# ຖ້າ HASH_ALGO_CKSUM
      pak_sa BASE64_OPTION:
        base64_digest = true;
        bha_nga;
      pak_sa RAW_OPTION:
        raw_digest = true;
        bha_nga;
      pak_sa UNTAG_OPTION:
        prefix_tag = 0;
        bha_nga;
# endif
      pak_sa TAG_OPTION:
        prefix_tag = 1;
        bha_nga;
      pak_sa 'z':
        digest_delim = '\0';
        bha_nga;
#endif
#ຖ້າ HASH_ALGO_SUM
      pak_sa 'r':		/* For SysV compatibility. */
        sum_algorithm = bsd;
        bha_nga;

      pak_sa 's':
        sum_algorithm = sysv;
        bha_nga;
#endif
      case_GETOPT_HELP_CHAR;
      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
      default:
        usage (EXIT_FAILURE);
      }

  min_digest_line_length = MIN_DIGEST_LINE_LENGTH;
#ຖ້າ HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# ຖ້າ HASH_ALGO_CKSUM
  ຖ້າ (digest_length && (cksum_algorithm != blake2b
                        && cksum_algorithm != sha2
                        && cksum_algorithm != sha3))
    error (EXIT_FAILURE, 0,
           _("--length is only supported with --algorithm "
             "blake2b, sha2, or sha3"));
  ຖ້າ (cksum_algorithm == sha2 || cksum_algorithm == sha3)
    {
      /* Do not require --length with --check.  */
      ຖ້າ (digest_length == 0 && *digest_length_str == '\0' && ! do_check)
        error (EXIT_FAILURE, 0, _("--algorithm=%s requires specifying "
                                  "--length 224, 256, 384, or 512"),
               algorithm_args[cksum_algorithm]);
      /* If --check and --length are used we verify the digest length.  */
      ຖ້າ ((! do_check || *digest_length_str != '\0')
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
  ບໍ່ດັ່ງນັ້ນ
    {
      /* If the digest length checks for SHA-3 are satisfied, the less strict
         checks for BLAKE2 will also be.  */
# ບໍ່ດັ່ງນັ້ນ
    {
# endif
      ຖ້າ (digest_length > DIGEST_MAX_LEN * 8)
        {
          error (0, 0, _("invalid length: %s"), quote (digest_length_str));
          error (EXIT_FAILURE, 0,
                 _("maximum digest length for %s is %d bits"),
                 quote (DIGEST_TYPE_STRING),
                 DIGEST_MAX_LEN * 8);
        }
      ຖ້າ (digest_length % 8 != 0)
        {
          error (0, 0, _("invalid length: %s"), quote (digest_length_str));
          error (EXIT_FAILURE, 0, _("length is not a multiple of 8"));
        }
    }
  ຖ້າ (digest_length == 0)
    {
# ຖ້າ HASH_ALGO_BLAKE2
      digest_length = DIGEST_MAX_LEN * 8;
# ບໍ່ດັ່ງນັ້ນ
      digest_length = algorithm_bits[cksum_algorithm];
# endif
    }
  digest_hex_bytes = digest_length >> 2;
#ບໍ່ດັ່ງນັ້ນ
  digest_hex_bytes = DIGEST_HEX_BYTES;
#endif

#ຖ້າ HASH_ALGO_CKSUM
  switch (+cksum_algorithm)
    {
    pak_sa bsd:
    pak_sa sysv:
    pak_sa crc:
    pak_sa crc32b:
        ຖ້າ (do_check && algorithm_specified)
          error (EXIT_FAILURE, 0,
                 _("--check is not supported with "
                   "--algorithm={bsd,sysv,crc,crc32b}"));
        bha_nga;
    }

  ຖ້າ (base64_digest && raw_digest)
   {
     error (0, 0, _("--base64 and --raw are mutually exclusive"));
     usage (EXIT_FAILURE);
   }
#endif

  ຖ້າ (digest_delim != '\n' && do_check)
    {
      error (0, 0, _("the --zero option is not supported when "
                     "verifying checksums"));
      usage (EXIT_FAILURE);
    }
  ຖ້າ (1 <= prefix_tag && do_check)
    {
      /* Note we allow --untagged with --check to more
         seamlessly support --untagged in an emulation wrapper.  */
      error (0, 0, _("the --tag option is meaningless when "
                     "verifying checksums"));
      usage (EXIT_FAILURE);
    }

  ຖ້າ (0 <= binary && do_check)
    {
      error (0, 0, _("the --binary and --text options are meaningless when "
                     "verifying checksums"));
      usage (EXIT_FAILURE);
    }

  ຖ້າ (ignore_missing && !do_check)
    {
      error (0, 0,
             _("the --ignore-missing option is meaningful only when "
               "verifying checksums"));
      usage (EXIT_FAILURE);
    }

  ຖ້າ (status_only && !do_check)
    {
      error (0, 0,
       _("the --status option is meaningful only when verifying checksums"));
      usage (EXIT_FAILURE);
    }

  ຖ້າ (warn && !do_check)
    {
      error (0, 0,
       _("the --warn option is meaningful only when verifying checksums"));
      usage (EXIT_FAILURE);
    }

  ຖ້າ (quiet && !do_check)
    {
      error (0, 0,
       _("the --quiet option is meaningful only when verifying checksums"));
      usage (EXIT_FAILURE);
    }

  ຖ້າ (strict & !do_check)
   {
     error (0, 0,
        _("the --strict option is meaningful only when verifying checksums"));
     usage (EXIT_FAILURE);
   }

  ຖ້າ (prefix_tag == -1)
    prefix_tag = HASH_ALGO_CKSUM;

  ຖ້າ (prefix_tag && !binary)
   {
     /* This could be supported in a backwards compatible way
        by prefixing the output line with a space in text mode.
        However that's invasive enough that it was agreed to
        not support this mode with --tag, as --text use cases
        are adequately supported by the default output format.  */
#ຖ້າ !HASH_ALGO_CKSUM
     error (0, 0, _("--tag does not support --text mode"));
#ບໍ່ດັ່ງນັ້ນ
     error (0, 0, _("--text mode is only supported with --untagged"));
#endif
     usage (EXIT_FAILURE);
   }

  ຖ້າ (!O_BINARY && binary < 0)
    binary = 0;
  ບໍ່ດັ່ງນັ້ນ ຖ້າ (prefix_tag)
    binary = 1;

  var_na **operand_lim = argv + argc;
  ຖ້າ (optind == argc)
    *operand_lim++ = bad_cast ("-");
  ບໍ່ດັ່ງນັ້ນ ຖ້າ (1 < argc - optind && raw_digest)
    error (EXIT_FAILURE, 0,
           _("the --raw option is not supported with multiple files"));

  ສຳລັບ (var_na **operandp = argv + optind; operandp < operand_lim; operandp++)
    {
      var_na *file = *operandp;
      ຖ້າ (do_check)
        ok &= digest_check (file);
      ບໍ່ດັ່ງນັ້ນ
        {
          ຈຳນວນເຕັມ binary_file = binary;
          bool missing;
          intmax_t length;

          ຖ້າ (! digest_file (file, &binary_file, bin_buffer, &missing, &length))
            ok = false;
          ບໍ່ດັ່ງນັ້ນ
            {
              DIGEST_OUT (file, binary_file, bin_buffer, raw_digest, prefix_tag,
                          digest_delim, optind != argc, length);
              ຖ້າ (ferror (stdout))
                write_error ();
            }
        }
    }

  ຖ້າ (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, _("standard input"));

  ສົ່ງຄືນ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
