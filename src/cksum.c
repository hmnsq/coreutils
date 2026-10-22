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

#ضمن <config.h>

#ضمن <getopt.h>
#ضمن <sys/types.h>

#ضمن "assure.h"
#ضمن "system.h"
#ضمن "argmatch.h"
#ضمن "c-ctype.h"
#ضمن "quote.h"
#ضمن "xdectoint.h"
#ضمن "xstrtol.h"

#ifndef HASH_ALGO_CKSUM
# define HASH_ALGO_CKSUM 0
#endif

#إذا HASH_ALGO_SUM || HASH_ALGO_CKSUM
# include "sum.h"
#endif
#إذا HASH_ALGO_CKSUM
# include "cksum.h"
# include "cksum_crc.h"
# include "base64.h"
#endif
#إذا HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# include "blake2/b2sum.h"
#endif
#إذا HASH_ALGO_MD5 || HASH_ALGO_CKSUM
# include "md5.h"
#endif
#إذا HASH_ALGO_SHA1 || HASH_ALGO_CKSUM
# include "sha1.h"
#endif
#إذا HASH_ALGO_SHA256 || HASH_ALGO_SHA224 || HASH_ALGO_CKSUM
# include "sha256.h"
#endif
#إذا HASH_ALGO_SHA512 || HASH_ALGO_SHA384 || HASH_ALGO_CKSUM
# include "sha512.h"
#endif
#إذا HASH_ALGO_CKSUM
# include "sha3.h"
#endif
#إذا HASH_ALGO_CKSUM
# include "sm3.h"
#endif
#ضمن "fadvise.h"
#ضمن "stdio--.h"
#ضمن "xbinary-io.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#إذا HASH_ALGO_SUM
# define PROGRAM_NAME "sum"
# define DIGEST_TYPE_STRING "BSD"
# define DIGEST_STREAM sumfns[sum_algorithm]
# define DIGEST_OUT sum_output_fns[sum_algorithm]
# define DIGEST_BITS 16
# define DIGEST_ALIGN 4
#وإلاإذا HASH_ALGO_CKSUM
# define MAX_DIGEST_BITS 512
# define MAX_DIGEST_ALIGN 8
# define DIGEST_TYPE_STRING algorithm_tags[cksum_algorithm]
# define DIGEST_STREAM cksumfns[cksum_algorithm]
# define DIGEST_OUT cksum_output_fns[cksum_algorithm]
# define DIGEST_BITS MAX_DIGEST_BITS
# define DIGEST_ALIGN MAX_DIGEST_ALIGN
#وإلاإذا HASH_ALGO_MD5
# define PROGRAM_NAME "md5sum"
# define DIGEST_TYPE_STRING "MD5"
# define DIGEST_STREAM md5_stream
# define DIGEST_BITS 128
# define DIGEST_REFERENCE "RFC 1321"
# define DIGEST_ALIGN 4
#وإلاإذا HASH_ALGO_BLAKE2
# define PROGRAM_NAME "b2sum"
# define DIGEST_TYPE_STRING "BLAKE2b"
# define DIGEST_STREAM blake2b_stream
# define DIGEST_BITS 512
# define DIGEST_REFERENCE "RFC 7693"
# define DIGEST_ALIGN 8
#وإلاإذا HASH_ALGO_SHA1
# define PROGRAM_NAME "sha1sum"
# define DIGEST_TYPE_STRING "SHA1"
# define DIGEST_STREAM sha1_stream
# define DIGEST_BITS 160
# define DIGEST_REFERENCE "FIPS-180-1"
# define DIGEST_ALIGN 4
#وإلاإذا HASH_ALGO_SHA256
# define PROGRAM_NAME "sha256sum"
# define DIGEST_TYPE_STRING "SHA256"
# define DIGEST_STREAM sha256_stream
# define DIGEST_BITS 256
# define DIGEST_REFERENCE "FIPS-180-2"
# define DIGEST_ALIGN 4
#وإلاإذا HASH_ALGO_SHA224
# define PROGRAM_NAME "sha224sum"
# define DIGEST_TYPE_STRING "SHA224"
# define DIGEST_STREAM sha224_stream
# define DIGEST_BITS 224
# define DIGEST_REFERENCE "RFC 3874"
# define DIGEST_ALIGN 4
#وإلاإذا HASH_ALGO_SHA512
# define PROGRAM_NAME "sha512sum"
# define DIGEST_TYPE_STRING "SHA512"
# define DIGEST_STREAM sha512_stream
# define DIGEST_BITS 512
# define DIGEST_REFERENCE "FIPS-180-2"
# define DIGEST_ALIGN 8
#وإلاإذا HASH_ALGO_SHA384
# define PROGRAM_NAME "sha384sum"
# define DIGEST_TYPE_STRING "SHA384"
# define DIGEST_STREAM sha384_stream
# define DIGEST_BITS 384
# define DIGEST_REFERENCE "FIPS-180-2"
# define DIGEST_ALIGN 8
#وإلا
# error "Can't decide which hash algorithm to compile."
#endif
#إذا !HASH_ALGO_SUM && !HASH_ALGO_CKSUM
# define DIGEST_OUT output_file
#endif

#إذا HASH_ALGO_CKSUM
# define PROGRAM_NAME (!legacy_mode ? "cksum"			 \
                       : cksum_algorithm == md5 ? "md5sum"	 \
                       : cksum_algorithm == sha1 ? "sha1sum"	 \
                       : cksum_algorithm == sha224 ? "sha224sum" \
                       : cksum_algorithm == sha256 ? "sha256sum" \
                       : cksum_algorithm == sha384 ? "sha384sum" \
                       : cksum_algorithm == sha512 ? "sha512sum" \
                       : "cksum")
#endif

#إذا HASH_ALGO_SUM
# define AUTHORS \
  proper_name ("Kayvan Aghaiepour"), \
  proper_name ("David MacKenzie")
#وإلاإذا HASH_ALGO_CKSUM
# define AUTHORS \
  proper_name_lite ("Padraig Brady", "P\303\241draig Brady"), \
  proper_name ("Q. Frank Xia")
#وإلاإذا HASH_ALGO_BLAKE2
# define AUTHORS \
  proper_name_lite ("Padraig Brady", "P\303\241draig Brady"), \
  proper_name ("Samuel Neves")
#وإلا
# define AUTHORS \
  proper_name ("Ulrich Drepper"), \
  proper_name ("Scott Miller"), \
  proper_name ("David Madore")
#endif
#إذا !HASH_ALGO_BLAKE2 && !HASH_ALGO_CKSUM
# define DIGEST_HEX_BYTES (DIGEST_BITS / 4)
#endif
#define DIGEST_BIN_BYTES (DIGEST_BITS / 8)

/* The minimum length of a valid digest line.  This length does
   not include any newline character at the end of a line.  */
#إذا HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# define MIN_DIGEST_لي_LENGTH 3 /* With -l 8.  */
#وإلا
# define MIN_DIGEST_لي_LENGTH \
   (DIGEST_HEX_BYTES /* length of hexadecimal message digest */ \
    + 1 /* blank */ \
    + 1 /* minimum filename length */ )
#endif

#إذا !HASH_ALGO_SUM
ساكن فراغ
output_file (حرف ثابت *file, صحيح نبيو_file, فراغ ثابت *digest,
             bool raw, bool tagged, unsigned حرف delim, bool حجوجي,
             intmax_t length);
#endif

/* True if any of the files read were the standard input. */
ساكن bool have_read_stdin;

/* The minimum length of a valid checksum line for the selected algorithm.  */
ساكن idx_t min_digest_لي_length;

/* Set to the length of a digest hex string for the selected algorithm.  */
ساكن idx_t digest_hex_bytes;

/* With --check, don't generate any output.
   The exit code indicates success or failure.  */
ساكن bool status_only = false;

/* With --check, print a message to standard error warning about each
   improperly formatted checksum line.  */
ساكن bool warn = false;

/* With --check, ignore missing files.  */
ساكن bool ignore_missing = false;

/* With --check, suppress the "OK" printed for each verified file.  */
ساكن bool quiet = false;

/* With --check, exit with a non-zero return code if any line is
   improperly formatted. */
ساكن bool strict = false;

/* Whether a BSD reversed format checksum is detected.  */
ساكن صحيح bsd_reversed = -1;

/* line delimiter.  */
ساكن unsigned حرف digest_delim = '\n';

#إذا HASH_ALGO_CKSUM
/* If true, print base64-encoded digests, not hex.  */
ساكن bool base64_digest = false;
#endif

/* If true, print binary digests, not hex.  */
ساكن bool raw_digest = false;

/* blake2b and sha3 allow the -l option.  Luckily they both have the same
   maximum digest size.  */
#إذا HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# إذا HASH_ALGO_CKSUM
static_assert (BLAKE2B_OUTBYTES == SHA3_512_DIGEST_SIZE);
# endif
# define DIGEST_MAX_LEN BLAKE2B_OUTBYTES
ساكن idx_t digest_length;
#endif /* HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM */

تعريفنوع فراغ (*digest_output_fn)(حرف ثابت *, صحيح, فراغ ثابت *, bool,
                                 bool, unsigned حرف, bool, intmax_t);
#إذا HASH_ALGO_SUM
تعداد Algorithm
{
  bsd,
  sysv,
};

ساكن تعداد Algorithm sum_algorithm;
ساكن sumfn sumfns[]=
{
  bsd_sum_stream,
  sysv_sum_stream,
};
ساكن digest_output_fn sum_output_fns[]=
{
  output_bsd,
  output_sysv,
};
#endif

#إذا HASH_ALGO_CKSUM
ساكن صحيح
md5_sum_stream (FILE *stream, فراغ *resstream, MAYBE_UNUSED intmax_t *length)
{
  أرجع md5_stream (stream, resstream);
}
ساكن صحيح
sha1_sum_stream (FILE *stream, فراغ *resstream, MAYBE_UNUSED intmax_t *length)
{
  أرجع sha1_stream (stream, resstream);
}
ساكن صحيح
sha224_sum_stream (FILE *stream, فراغ *resstream,
                   MAYBE_UNUSED intmax_t *length)
{
  أرجع sha224_stream (stream, resstream);
}
ساكن صحيح
sha256_sum_stream (FILE *stream, فراغ *resstream,
                   MAYBE_UNUSED intmax_t *length)
{
  أرجع sha256_stream (stream, resstream);
}
ساكن صحيح
sha384_sum_stream (FILE *stream, فراغ *resstream,
                   MAYBE_UNUSED intmax_t *length)
{
  أرجع sha384_stream (stream, resstream);
}
ساكن صحيح
sha512_sum_stream (FILE *stream, فراغ *resstream,
                   MAYBE_UNUSED intmax_t *length)
{
  أرجع sha512_stream (stream, resstream);
}
ساكن صحيح
sha2_sum_stream (FILE *stream, فراغ *resstream, intmax_t *length)
{
  بدل (*length)
    {
    حالة SHA224_DIGEST_SIZE:
      أرجع sha224_stream (stream, resstream);
    حالة SHA256_DIGEST_SIZE:
      أرجع sha256_stream (stream, resstream);
    حالة SHA384_DIGEST_SIZE:
      أرجع sha384_stream (stream, resstream);
    حالة SHA512_DIGEST_SIZE:
      أرجع sha512_stream (stream, resstream);
    افتراضي:
      affirm (false);
    }
}
ساكن صحيح
sha3_sum_stream (FILE *stream, فراغ *resstream, intmax_t *length)
{
  بدل (*length)
    {
    حالة SHA3_224_DIGEST_SIZE:
      أرجع sha3_224_stream (stream, resstream);
    حالة SHA3_256_DIGEST_SIZE:
      أرجع sha3_256_stream (stream, resstream);
    حالة SHA3_384_DIGEST_SIZE:
      أرجع sha3_384_stream (stream, resstream);
    حالة SHA3_512_DIGEST_SIZE:
      أرجع sha3_512_stream (stream, resstream);
    افتراضي:
      affirm (false);
    }
}
ساكن صحيح
blake2b_sum_stream (FILE *stream, فراغ *resstream, intmax_t *length)
{
  أرجع blake2b_stream (stream, resstream, *length);
}
ساكن صحيح
sm3_sum_stream (FILE *stream, فراغ *resstream, MAYBE_UNUSED intmax_t *length)
{
  أرجع sm3_stream (stream, resstream);
}

ساكن حرف ثابت *ثابت algorithm_حجوجي[] =
{
  "bsd", "sysv", "crc", "crc32b", "md5", "sha1",
  "sha224", "sha256", "sha384", "sha512", /* Legacy naming */
  "sha2", "sha3", "blake2b", "sm3", عدم
};
ساكن تعداد Algorithm ثابت algorithm_types[] =
{
  bsd, sysv, crc, crc32b, md5, sha1,
  sha224, sha256, sha384, sha512,
  sha2, sha3, blake2b, sm3,
};
ARGMATCH_VERIFY (algorithm_حجوجي, algorithm_types);

ساكن حرف ثابت *ثابت algorithm_tags[] =
{
  "BSD", "SYSV", "CRC", "CRC32B", "MD5", "SHA1",
  "SHA224", "SHA256", "SHA384", "SHA512",
  "SHA2", "SHA3", "BLAKE2b", "SM3", عدم
};
ساكن صحيح ثابت algorithm_bits[] =
{
  16, 16, 32, 32, 128, 160,
  224, 256, 384, 512,
  512, 512, 512, 256, 0
};

static_assert (countof (algorithm_bits) == countof (algorithm_حجوجي));

ساكن bool algorithm_specified = false;
ساكن bool legacy_mode = false;
تعداد Algorithm cksum_algorithm = crc;

ساكن sumfn cksumfns[]=
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
ساكن digest_output_fn cksum_output_fns[]=
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

تعداد
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

#إذا HASH_ALGO_CKSUM
ساكن بنية option ثابت legacy_long_options[] =
{
  { "check", no_argument, عدم, 'c' },
  { "ignore-missing", no_argument, عدم, IGNORE_MISSING_OPTION },
  { "quiet", no_argument, عدم, QUIET_OPTION },
  { "status", no_argument, عدم, STATUS_OPTION },
  { "warn", no_argument, عدم, 'w' },
  { "strict", no_argument, عدم, STRICT_OPTION },
  { "tag", no_argument, عدم, TAG_OPTION },
  { "zero", no_argument, عدم, 'z' },

  { "binary", no_argument, عدم, 'b' }, /* Deprecated.  */
  { "text", no_argument, عدم, 't' }, /* Deprecated.  */

  { GETOPT_HELP_OPTION_DECL },
  { GETOPT_VERSION_OPTION_DECL },
  { عدم, 0, عدم, 0 }
};
#endif

ساكن بنية option ثابت long_options[] =
{
#إذا HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
  { "length", required_argument, عدم, 'l'},
#endif

#إذا !HASH_ALGO_SUM
  { "check", no_argument, عدم, 'c' },
  { "ignore-missing", no_argument, عدم, IGNORE_MISSING_OPTION},
  { "quiet", no_argument, عدم, QUIET_OPTION },
  { "status", no_argument, عدم, STATUS_OPTION },
  { "warn", no_argument, عدم, 'w' },
  { "strict", no_argument, عدم, STRICT_OPTION },
  { "tag", no_argument, عدم, TAG_OPTION },
  { "zero", no_argument, عدم, 'z' },

# إذا HASH_ALGO_CKSUM
  { "algorithm", required_argument, عدم, 'a'},
  { "base64", no_argument, عدم, BASE64_OPTION },
  { "debug", no_argument, عدم, DEBUG_PROGRAM_OPTION},
  { "raw", no_argument, عدم, RAW_OPTION},
  { "untagged", no_argument, عدم, UNTAG_OPTION },
# endif
  { "binary", no_argument, عدم, 'b' }, /* Deprecated.  */
  { "text", no_argument, عدم, 't' }, /* Deprecated.  */

#وإلا
  {"sysv", no_argument, عدم, 's'},  /* Not in cksum.  */
#endif

  { GETOPT_HELP_OPTION_DECL },
  { GETOPT_VERSION_OPTION_DECL },
  { عدم, 0, عدم, 0 }
};

فراغ
usage (صحيح status)
{
  إذا (status != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"), program_name);
#إذا HASH_ALGO_CKSUM
    إذا (legacy_mode)
      {
        printf (_("\
بغ or check %s checksums.\n\
"), DIGEST_TYPE_STRING);
        fputs (_("\
Legacy interface to the cksum(1) utility.\n\
"), stdout);
      }
    وإلا
      fputs (_("\
بغ or verify checksums.\n\
By افتراضي use the 32 bit CRC algorithm.\n\
"), stdout);
#وإلا
      printf (_("\
بغ or check %s (%d-bit) checksums.\n\
"),
              DIGEST_TYPE_STRING,
              DIGEST_BITS);
      fputs (_("\
Legacy interface to the cksum(1) utility.\n\
"), stdout);
#endif

      emit_stdin_note ();
#إذا HASH_ALGO_SUM
      oputs (_("\
  -r\n\
         use BSD sum algorithm (the افتراضي), use 1K blocks\n\
"));
      oputs (_("\
  -s, --sysv\n\
         use System V sum algorithm, use 512 bytes blocks\n\
"));
#endif
#إذا HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
        emit_mandatory_arg_note ();
#endif
#إذا HASH_ALGO_CKSUM
     إذا (! legacy_mode)
        oputs (_("\
  -a, --algorithm=TYPE\n\
         select the digest type to use.  See DIGEST below\n\
"));
     إذا (! legacy_mode)
        oputs (_("\
      --base64\n\
         emit base64-encoded digests, not hexadecimal\n\
"));
#endif
#إذا !HASH_ALGO_SUM
# إذا HASH_ALGO_CKSUM
     إذا (legacy_mode)
       {
# endif
      إذا (O_نبيو)
        oputs (_("\
  -b, --نبيو\n\
         اقرأ in نبيو mode (افتراضي unless reading tty stdin)\n\
"));
      وإلا
        oputs (_("\
  -b, --نبيو\n\
         اقرأ in نبيو mode\n\
"));
# إذا HASH_ALGO_CKSUM
       }
# endif
        oputs (_("\
  -c, --check\n\
         اقرأ checksums from the FILEs and check them\n\
"));
# إذا HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
#  إذا HASH_ALGO_CKSUM
     إذا (! legacy_mode)
#  endif
        oputs (_("\
  -l, --length=BITS\n\
         digest length in bits; must not exceed the max size\n\
         and must be a multiple of 8 لكل blake2b;\n\
         must be 224, 256, 384, or 512 لكل sha2 or sha3\n\
"));
# endif
# إذا HASH_ALGO_CKSUM
    إذا (! legacy_mode)
        oputs (_("\
      --raw\n\
         emit a raw نبيو digest, not hexadecimal\n\
"));
    إذا (legacy_mode)
      oputs (_("\
      --tag\n\
         create a BSD-style checksum\n\
"));
    وإلا
      oputs (_("\
      --tag\n\
         create a BSD-style checksum (the افتراضي)\n\
"));
    إذا (! legacy_mode)
      oputs (_("\
      --untagged\n\
         create a reversed style checksum, without digest type\n\
"));
# وإلا
      oputs (_("\
      --tag\n\
         create a BSD-style checksum\n\
"));
# endif
# إذا HASH_ALGO_CKSUM
    إذا (legacy_mode)
      {
# endif
      إذا (O_نبيو)
        oputs (_("\
  -t, --text\n\
         اقرأ in text mode (افتراضي إذا reading tty stdin)\n\
"));
      وإلا
        oputs (_("\
  -t, --text\n\
         اقرأ in text mode (افتراضي)\n\
"));
# إذا HASH_ALGO_CKSUM
      }
# endif
      oputs (_("\
  -z, --zero\n\
         end each output لي with NUL, not newline,\n\
         and disable file name escaping\n\
"));
      fputs (_("\
\n\
The following five options are useful only when verifying checksums:\n\
"), stdout);
      oputs (_("\
      --ignore-missing\n\
         don't fail or report status لكل missing files\n\
"));
      oputs (_("\
      --quiet\n\
         don't بغ_2 OK لكل each successfully verified file\n\
"));
      oputs (_("\
      --status\n\
         don't output anything, status code shows success\n\
"));
      oputs (_("\
      --strict\n\
         exit non-zero لكل improperly formatted checksum ليوك\n\
"));
      oputs (_("\
  -w, --warn\n\
         warn about improperly formatted checksum ليوك\n\
"));
#endif
#إذا HASH_ALGO_CKSUM
    إذا (! legacy_mode)
      oputs (_("\
      --debug\n\
         indicate which implementation used\n\
"));
#endif
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
#إذا HASH_ALGO_CKSUM
    إذا (! legacy_mode)
      fputs (_("\
\n\
DIGEST determines the digest algorithm and افتراضي output format:\n\
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
#إذا !HASH_ALGO_SUM && !HASH_ALGO_CKSUM
      printf (_("\
\n\
The sums are computed as described in %s.\n"), DIGEST_REFERENCE);
      fputs (_("\
When checking, the شغرو should be a former output of this program.\n\
The افتراضي mode is to بغ_2 a لي with: checksum, a space,\n\
a character indicating شغرو mode ('*' لكل نبيو, ' ' لكل text\n\
or where نبيو is insignificant), and name لكل each FILE.\n\
\n\
There is no difference between نبيو mode and text mode on GNU systems.\
\n"), stdout);
#endif
#إذا HASH_ALGO_CKSUM
      fputs (_("\
When checking, the شغرو should be a former output of this program,\n\
or equivalent standalone program.\
\n"), stdout);
#endif
      emit_ancillary_info (PROGRAM_NAME);
    }

  exit (status);
}

#إذا !HASH_ALGO_SUM
/* Given a string S, return TRUE if it contains problematic characters
   that need escaping.  Note we escape '\' itself to provide some forward
   compat to introduce escaping of other characters.  */

ATTRIBUTE_PURE
ساكن bool
problematic_chars (حرف ثابت *s)
{
  idx_t length = strcspn (s, "\\\n\r");
  أرجع s[length] != '\0';
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

ساكن حرف *
filename_unescape (حرف *s, idx_t s_len)
{
  حرف *dst = s;

  لكل (idx_t i = 0; i < s_len; i++)
    {
      بدل (s[i])
        {
        حالة '\\':
          إذا (i == s_len - 1)
            {
              /* File name ends with an unescaped backslash: invalid.  */
              أرجع عدم;
            }
          ++i;
          بدل (s[i])
            {
            حالة 'n':
              *dst++ = '\n';
              اكسر;
            حالة 'r':
              *dst++ = '\r';
              اكسر;
            حالة '\\':
              *dst++ = '\\';
              اكسر;
            افتراضي:
              /* Only '\', 'n' or 'r' may follow a backslash.  */
              أرجع عدم;
            }
          اكسر;

        حالة '\0':
          /* The file name may not contain a NUL.  */
          أرجع عدم;

        افتراضي:
          *dst++ = s[i];
          اكسر;
        }
    }
  إذا (dst < s + s_len)
    *dst = '\0';

  أرجع s;
}

/* Return true if S is a LEN-byte NUL-terminated string of hex or base64
   digits and has the expected length.  Otherwise, return false.  */
ATTRIBUTE_PURE
ساكن bool
valid_digits (unsigned حرف ثابت *s, idx_t len)
{
#إذا HASH_ALGO_CKSUM
  إذا (len == BASE64_LENGTH (digest_length >> 3))
    {
      idx_t i;
      لكل (i = 0; i < len - digest_length % 3; i++)
        {
          إذا (!isbase64 (*s))
            أرجع false;
          ++s;
        }
      لكل ( ; i < len; i++)
        {
          إذا (*s != '=')
            أرجع false;
          ++s;
        }
    }
  وإلا
#endif
  إذا (len == digest_hex_bytes)
    {
      لكل (idx_t i = 0; i < digest_hex_bytes; i++)
        {
          إذا (!c_isxdigit (*s))
            أرجع false;
          ++s;
        }
    }
  وإلا
    أرجع false;

  أرجع *s == '\0';
}

/* Split the checksum string S (of length S_LEN) from a BSD 'md5' or
   'sha1' command into two parts: a hexadecimal digest, and the file
   name.  S is modified.  Set *D_LEN to the length of the digest string.
   Return true if successful.  */

ساكن bool
bsd_split_3 (حرف *s, idx_t s_len,
             unsigned حرف **digest, idx_t *d_len,
             حرف **file_name, bool escaped_filename)
{
  إذا (s_len == 0)
    أرجع false;

  /* Find end of filename.  */
  idx_t i = s_len - 1;
  طالما (i && s[i] != ')')
    i--;

  إذا (s[i] != ')')
    أرجع false;

  *file_name = s;

  إذا (escaped_filename && filename_unescape (s, i) == عدم)
    أرجع false;

  s[i++] = '\0';

  طالما (c_isblank (s[i]))
    i++;

  إذا (s[i] != '=')
    أرجع false;

  i++;

  طالما (c_isblank (s[i]))
    i++;

  *digest = (unsigned حرف *) &s[i];

  *d_len = s_len - i;
  أرجع valid_digits (*digest, *d_len);
}

#إذا HASH_ALGO_CKSUM
/* Return the corresponding Algorithm for the string S,
   or -1 for no match.  */

ساكن ptrdiff_t
algorithm_from_tag (حرف *s)
{
  /* Limit check size to this length for perf reasons.  */
  ساكن idx_t max_tag_len;
  إذا (! max_tag_len)
    {
      حرف ثابت * ثابت * tag = algorithm_tags;
      طالما (*tag)
        {
          idx_t tag_len = strlen (*tag++);
          max_tag_len = MAX (tag_len, max_tag_len);
        }
    }

  idx_t i = 0;

  /* Find end of tag */
  طالما (i <= max_tag_len && s[i] && ! c_isblank (s[i])
         && s[i] != '-' && s[i] != '(')
    ++i;

  إذا (i > max_tag_len)
    أرجع -1;

  /* Terminate tag, and lookup.  */
  حرف sep = s[i];
  s[i] = '\0';
  ptrdiff_t algo = argmatch_exact (s, algorithm_tags);
  s[i] = sep;

  أرجع algo;
}
#endif

/* Split the string S (of length S_LEN) into three parts:
   a hexadecimal digest, binary flag, and the file name.
   S is modified.  Set *D_LEN to the length of the digest string.
   Return true if successful.  */

ساكن bool
split_3 (حرف *s, idx_t s_len,
         unsigned حرف **digest, idx_t *d_len, صحيح *نبيو, حرف **file_name)
{
  bool escaped_filename = false;
  idx_t algo_name_len;

  idx_t i = 0;
  طالما (c_isblank (s[i]))
    ++i;

  إذا (s[i] == '\\')
    {
      ++i;
      escaped_filename = true;
    }

  /* Check for BSD-style checksum line. */

#إذا HASH_ALGO_CKSUM
  إذا (! algorithm_specified || cksum_algorithm == sha2)
    {
      ptrdiff_t algo_tag = algorithm_from_tag (s + i);
      إذا (! algorithm_specified)
        {
          إذا (algo_tag >= 0)
            {
              إذا (algo_tag <= crc32b)
                أرجع false;  /* We don't support checking these formats.  */
              cksum_algorithm = algo_tag;
            }
          وإلا
            أرجع false;      /* We only support tagged format without -a.  */
        }
      وإلا
        {
          إذا (cksum_algorithm == sha2 && (algo_tag == sha2
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
  إذا (STREQ_LEN (s + i, DIGEST_TYPE_STRING, algo_name_len))
    {
      i += algo_name_len;
#إذا HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM

# إذا HASH_ALGO_BLAKE2
      digest_length = DIGEST_MAX_LEN * 8;
# وإلا
      digest_length = algorithm_bits[cksum_algorithm];
# endif
      إذا (s[i] == '-')  /* length specified. Not base64 */
        {
          ++i;
          intmax_t length;
          حرف *siend;
          إذا (!c_isdigit (s[i])
              || xstrtoimax (s + i, &siend, 0, &length, عدم) != LONGINT_OK)
            أرجع false;
# إذا HASH_ALGO_CKSUM
          وإلا إذا (cksum_algorithm == sha2 || cksum_algorithm == sha3)
            {
              إذا (length != SHA224_DIGEST_SIZE * 8
                  && length != SHA256_DIGEST_SIZE * 8
                  && length != SHA384_DIGEST_SIZE * 8
                  && length != SHA512_DIGEST_SIZE * 8)
                أرجع false;
            }
# endif
          وإلا إذا (!(0 < length && length <= digest_length && length % 8 == 0))
            أرجع false;

          i = siend - s;
          digest_length = length;
        }
      digest_hex_bytes = digest_length >> 2;
#endif
      إذا (s[i] == ' ')
        ++i;
      إذا (s[i] == '(')  /* not base64 */
        {
          ++i;
          *نبيو = 0;
          أرجع bsd_split_3 (s + i, s_len - i,
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
  إذا (s_len - i < min_digest_لي_length + (s[i] == '\\'))
    أرجع false;

  *digest = (unsigned حرف *) &s[i];

#إذا HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
  /* Auto determine length.  */
# إذا HASH_ALGO_CKSUM
  إذا (cksum_algorithm == blake2b
      || cksum_algorithm == sha2 || cksum_algorithm == sha3) {
# endif
  unsigned حرف ثابت *hp = *digest;
  digest_hex_bytes = 0;
  لكل (; c_isxdigit (*hp); ++hp, ++digest_hex_bytes)
    ;
# إذا HASH_ALGO_CKSUM
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
  لكل (; isubase64 (*hp); ++hp, ++digest_base64_bytes)
    ;
  لكل (; *hp == '='; ++hp, ++trailing_equals)
    ;
  إذا ((cksum_algorithm == sha2 || cksum_algorithm == sha3)
      && digest_hex_bytes >> 1 != SHA224_DIGEST_SIZE
      && digest_hex_bytes >> 1 != SHA256_DIGEST_SIZE
      && digest_hex_bytes >> 1 != SHA384_DIGEST_SIZE
      && digest_hex_bytes >> 1 != SHA512_DIGEST_SIZE)
    {
      إذا (digest_base64_bytes + trailing_equals
          == BASE64_LENGTH (SHA224_DIGEST_SIZE))
        digest_hex_bytes = SHA224_DIGEST_SIZE * 2;
      وإلا إذا (digest_base64_bytes + trailing_equals
               == BASE64_LENGTH (SHA256_DIGEST_SIZE))
        digest_hex_bytes = SHA256_DIGEST_SIZE * 2;
      وإلا إذا (digest_base64_bytes + trailing_equals
               == BASE64_LENGTH (SHA384_DIGEST_SIZE))
        digest_hex_bytes = SHA384_DIGEST_SIZE * 2;
      وإلا إذا (digest_base64_bytes + trailing_equals
               == BASE64_LENGTH (SHA512_DIGEST_SIZE))
        digest_hex_bytes = SHA512_DIGEST_SIZE * 2;
      وإلا
        أرجع false;
    }
  وإلا إذا (cksum_algorithm == blake2b
           && digest_hex_bytes < digest_base64_bytes)
    {
      لكل (صحيح j = 8; j <= DIGEST_MAX_LEN * 8; j += 8)
        {
          إذا (BASE64_LENGTH (j / 8) == digest_base64_bytes + trailing_equals
              && j % 3 == trailing_equals)
            {
              digest_hex_bytes = j / 4;
              اكسر;
            }
        }
    }
# endif
  إذا (digest_hex_bytes < 2 || digest_hex_bytes % 2
      || DIGEST_MAX_LEN * 2 < digest_hex_bytes)
    أرجع false;
  digest_length = digest_hex_bytes * 4;
# إذا HASH_ALGO_CKSUM
  }
# endif
#endif

  /* This field must be the hexadecimal or base64 representation
     of the message digest.  */
  طالما (s[i] && !c_isblank (s[i]))
    i++;

  /* The digest must be followed by at least one whitespace character.  */
  إذا (i == s_len)
    أرجع false;

  *d_len = &s[i] - (حرف *) *digest;
  s[i++] = '\0';

  إذا (! valid_digits (*digest, *d_len))
    أرجع false;

  /* If "bsd reversed" format detected.  */
  إذا ((s_len - i == 1) || (s[i] != ' ' && s[i] != '*'))
    {
      /* Don't allow mixing bsd and standard formats,
         to minimize security issues with attackers
         renaming files with leading spaces.
         This assumes that with bsd format checksums
         that the first file name does not have
         a leading ' ' or '*'.  */
      إذا (bsd_reversed == 0)
        أرجع false;
      bsd_reversed = 1;
    }
  وإلا إذا (bsd_reversed != 1)
    {
      bsd_reversed = 0;
      *نبيو = (s[i++] == '*');
    }

  /* All characters between the type indicator and end of line are
     significant -- that includes leading and trailing white space.  */
  *file_name = &s[i];

  إذا (escaped_filename)
    أرجع filename_unescape (&s[i], s_len - i) != عدم;

  أرجع true;
}

#إذا !HASH_ALGO_SUM
/* If ESCAPE is true, then translate each:
   NEWLINE byte to the string, "\\n",
   CARRIAGE RETURN byte to the string, "\\r",
   and each backslash to "\\\\".  */
ساكن فراغ
بغ_filename (حرف ثابت *file, bool escape)
{
  إذا (! escape)
    {
      fputs (file, stdout);
      أرجع;
    }

  طالما (*file)
    {
      بدل (*file)
        {
        حالة '\n':
          fputs ("\\n", stdout);
          اكسر;

        حالة '\r':
          fputs ("\\r", stdout);
          اكسر;

        حالة '\\':
          fputs ("\\\\", stdout);
          اكسر;

        افتراضي:
          putchar (*file);
          اكسر;
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

ساكن bool
digest_file (حرف ثابت *filename, صحيح *نبيو, unsigned حرف *bin_result,
             bool *missing, MAYBE_UNUSED intmax_t *length)
{
  FILE *fp;
  صحيح err;
  bool is_stdin = streq (filename, "-");

  *missing = false;

  إذا (is_stdin)
    {
      have_read_stdin = true;
      fp = stdin;
      إذا (O_نبيو && *نبيو)
        {
          إذا (*نبيو < 0)
            *نبيو = ! isatty (STDIN_FILENO);
          إذا (*نبيو)
            xset_نبيو_mode (STDIN_FILENO, O_نبيو);
        }
    }
  وإلا
    {
      fp = fopen (filename, O_نبيو ? (*نبيو ? "rb" : "rt") : "r");
      إذا (fp == عدم)
        {
          إذا (ignore_missing && errno == ENOENT)
            {
              *missing = true;
              أرجع true;
            }
          error (0, errno, "%s", quotef (filename));
          أرجع false;
        }
    }

  fadvise (fp, FADVISE_SEQUENTIAL);

#إذا HASH_ALGO_CKSUM
  إذا (cksum_algorithm == blake2b
      || cksum_algorithm == sha2 || cksum_algorithm == sha3)
    *length = digest_length >> 3;
  err = DIGEST_STREAM (fp, bin_result, length);
#وإلاإذا HASH_ALGO_SUM
  err = DIGEST_STREAM (fp, bin_result, length);
#وإلاإذا HASH_ALGO_BLAKE2
  err = DIGEST_STREAM (fp, bin_result, digest_length >> 3);
#وإلا
  err = DIGEST_STREAM (fp, bin_result);
#endif
  err = err ? errno : 0;
  إذا (is_stdin)
    clearerr (fp);
  وإلا إذا (fclose (fp) != 0 && !err)
    err = errno;

  إذا (err)
    {
      error (0, err, "%s", quotef (filename));
      أرجع false;
    }

  أرجع true;
}

#إذا !HASH_ALGO_SUM
ساكن فراغ
output_file (حرف ثابت *file, صحيح نبيو_file, فراغ ثابت *digest,
             MAYBE_UNUSED bool raw, bool tagged, unsigned حرف delim,
             MAYBE_UNUSED bool حجوجي, MAYBE_UNUSED intmax_t length)
{
# إذا HASH_ALGO_CKSUM
  إذا (raw)
    {
      fwrite (digest, 1, digest_length >> 3, stdout);
      أرجع;
    }
# endif

  unsigned حرف ثابت *bin_buffer = digest;

  /* Output a leading backslash if the file name contains problematic chars.  */
  bool needs_escape = delim == '\n' && problematic_chars (file);

  إذا (needs_escape)
    putchar ('\\');

  إذا (tagged)
    {
# إذا HASH_ALGO_CKSUM
      إذا (cksum_algorithm == sha2)
        printf ("SHA%u", (unsigned صحيح) digest_length);
      وإلا
# endif
      fputs (DIGEST_TYPE_STRING, stdout);
# إذا HASH_ALGO_BLAKE2
      إذا (digest_length < DIGEST_MAX_LEN * 8)
        printf ("-%u", (unsigned صحيح) digest_length);
# elif HASH_ALGO_CKSUM
      إذا (cksum_algorithm == sha3)
        printf ("-%u", (unsigned صحيح) digest_length);
      إذا (cksum_algorithm == blake2b)
        {
          إذا (digest_length < DIGEST_MAX_LEN * 8)
            printf ("-%u", (unsigned صحيح) digest_length);
        }
# endif
      fputs (" (", stdout);
      بغ_filename (file, needs_escape);
      fputs (") = ", stdout);
    }

# إذا HASH_ALGO_CKSUM
  إذا (base64_digest)
    {
      حرف b64[BASE64_LENGTH (DIGEST_BIN_BYTES) + 1];
      base64_encode ((حرف ثابت *) bin_buffer, digest_length >> 3,
                     b64, حجم b64);
      fputs (b64, stdout);
    }
  وإلا
# endif
    {
      لكل (idx_t i = 0; i < (digest_hex_bytes >> 1); ++i)
        printf ("%02x", bin_buffer[i]);
    }

  إذا (!tagged)
    {
      putchar (' ');
      putchar (نبيو_file ? '*' : ' ');
      بغ_filename (file, needs_escape);
    }

  putchar (delim);
}
#endif

#إذا HASH_ALGO_CKSUM
/* Return true if B64_DIGEST is the same as the base64 digest of the
   DIGEST_LENGTH/8 bytes at BIN_BUFFER.  */
ساكن bool
b64_equal (unsigned حرف ثابت *b64_digest, unsigned حرف ثابت *bin_buffer)
{
  idx_t b64_n_bytes = BASE64_LENGTH (digest_length >> 3);
  حرف b64[BASE64_LENGTH (DIGEST_BIN_BYTES) + 1];
  base64_encode ((حرف ثابت *) bin_buffer, digest_length >> 3,
                 b64, حجم b64);
  أرجع memeq (b64_digest, b64, b64_n_bytes + 1);
}
#endif

/* Return true if HEX_DIGEST is the same as the hex-encoded digest of the
   DIGEST_LENGTH/8 bytes at BIN_BUFFER.  */
ساكن bool
hex_equal (unsigned حرف ثابت *hex_digest, unsigned حرف ثابت *bin_buffer)
{
  ساكن ثابت حرف bin2hex[] = { '0', '1', '2', '3',
                                  '4', '5', '6', '7',
                                  '8', '9', 'a', 'b',
                                  'c', 'd', 'e', 'f' };
  idx_t digest_bin_bytes = digest_hex_bytes >> 1;

  /* Compare generated binary number with text representation
     in check file.  Ignore case of hex digits.  */
  idx_t cnt;
  لكل (cnt = 0; cnt < digest_bin_bytes; ++cnt)
    {
      إذا (c_tolower (hex_digest[2 * cnt])
          != bin2hex[bin_buffer[cnt] >> 4]
          || (c_tolower (hex_digest[2 * cnt + 1])
              != (bin2hex[bin_buffer[cnt] & 0xf])))
        اكسر;
    }
  أرجع cnt == digest_bin_bytes;
}

ساكن bool
digest_check (حرف ثابت *checkfile_name)
{
  FILE *checkfile_stream;
  intmax_t n_misformatted_ليوك = 0;
  intmax_t n_mismatched_checksums = 0;
  intmax_t n_open_or_read_failures = 0;
  bool properly_formatted_ليوك = false;
  bool matched_checksums = false;
  unsigned حرف bin_buffer_unaligned[DIGEST_BIN_BYTES + DIGEST_ALIGN];
  /* Make sure bin_buffer is properly aligned. */
  unsigned حرف *bin_buffer = ptr_align (bin_buffer_unaligned, DIGEST_ALIGN);
  intmax_t لي_لمب;
  حرف *لي;
  size_t لي_chars_allocated;
  bool is_stdin = streq (checkfile_name, "-");

  إذا (is_stdin)
    {
      have_read_stdin = true;
      checkfile_name = _("standard input");
      checkfile_stream = stdin;
    }
  وإلا
    {
      checkfile_stream = fopen (checkfile_name, "r");
      إذا (checkfile_stream == عدم)
        {
          error (0, errno, "%s", quotef (checkfile_name));
          أرجع false;
        }
    }

  لي_لمب = 0;
  لي = عدم;
  لي_chars_allocated = 0;
  افعل
    {
      حرف *filename;
      صحيح نبيو;
      unsigned حرف *digest;
      ssize_t لي_length;

      ++لي_لمب;
      إذا (لي_لمب == 0)
        error (EXIT_FAILURE, 0, _("%s: too many checksum lines"),
               quotef (checkfile_name));

      لي_length = getline (&لي, &لي_chars_allocated, checkfile_stream);
      إذا (لي_length <= 0)
        اكسر;

      /* Ignore comment lines, which begin with a '#' character.  */
      إذا (لي[0] == '#')
        استمر;

      /* Remove any trailing newline.  */
      لي_length -= لي[لي_length - 1] == '\n';
      /* Remove any trailing carriage return.  */
      لي_length -= لي[لي_length - (0 < لي_length)] == '\r';

      /* Ignore empty lines.  */
      إذا (لي_length == 0)
        استمر;

      لي[لي_length] = '\0';

      idx_t d_len;
      إذا (! (split_3 (لي, لي_length, &digest, &d_len, &نبيو, &filename)
             && ! (is_stdin && streq (filename, "-"))))
        {
          ++n_misformatted_ليوك;

          إذا (warn)
            {
              error (0, 0,
                     _("%s: %jd"
                       ": improperly formatted %s checksum line"),
                     quotef (checkfile_name), لي_لمب,
                     DIGEST_TYPE_STRING);
            }
        }
      وإلا
        {
          bool ok;
          bool missing;

          properly_formatted_ليوك = true;

          intmax_t length;
          ok = digest_file (filename, &نبيو, bin_buffer, &missing, &length);

          إذا (!ok)
            {
              ++n_open_or_read_failures;
              إذا (!status_only)
                printf ("%s: %s\n", quotef (filename),_("FAILED open or read"));
            }
          وإلا إذا (ignore_missing && missing)
            {
              /* Ignore missing files with --ignore-missing.  */
              ;
            }
          وإلا
            {
              bool match = false;
#إذا HASH_ALGO_CKSUM
              إذا (d_len == BASE64_LENGTH (digest_length >> 3))
                match = b64_equal (digest, bin_buffer);
              وإلا
#endif
                إذا (d_len == digest_hex_bytes)
                  match = hex_equal (digest, bin_buffer);

              إذا (match)
                matched_checksums = true;
              وإلا
                ++n_mismatched_checksums;

              إذا (!status_only)
                {
                  إذا (! match || ! quiet)
                    fputs (quotef (filename), stdout);

                  إذا (! match)
                    printf (": %s\n", _("FAILED"));
                  وإلا إذا (!quiet)
                    printf (": %s\n", _("OK"));
                }
            }

          إذا (ferror (stdout))
            write_error ();
        }
    }
  طالما (!feof (checkfile_stream) && !ferror (checkfile_stream));

  حرر (لي);

  صحيح err = ferror (checkfile_stream) ? 0 : -1;
  إذا (is_stdin)
    clearerr (checkfile_stream);
  وإلا إذا (fclose (checkfile_stream) != 0 && err < 0)
    err = errno;

  إذا (0 <= err)
    {
      error (0, err, err ? "%s" : _("%s: read error"),
             quotef (checkfile_name));
      أرجع false;
    }

  إذا (! properly_formatted_ليوك)
    {
      /* Warn if no tests are found.  */
      error (0, 0, _("%s: no properly formatted checksum lines found"),
             quotef (checkfile_name));
    }
  وإلا
    {
      إذا (!status_only)
        {
          إذا (n_misformatted_ليوك != 0)
            error (0, 0,
                   (ngettext
                    ("WARNING: %jd line is improperly formatted",
                     "WARNING: %jd lines are improperly formatted",
                     select_plural (n_misformatted_ليوك))),
                   n_misformatted_ليوك);

          إذا (n_open_or_read_failures != 0)
            error (0, 0,
                   (ngettext
                    ("WARNING: %jd listed file could not be read",
                     "WARNING: %jd listed files could not be read",
                     select_plural (n_open_or_read_failures))),
                   n_open_or_read_failures);

          إذا (n_mismatched_checksums != 0)
            error (0, 0,
                   (ngettext
                    ("WARNING: %jd computed checksum did NOT match",
                     "WARNING: %jd computed checksums did NOT match",
                     select_plural (n_mismatched_checksums))),
                   n_mismatched_checksums);

          إذا (ignore_missing && ! matched_checksums)
            error (0, 0, _("%s: no file was verified"),
                   quotef (checkfile_name));
        }
    }

  أرجع (properly_formatted_ليوك
          && matched_checksums
          && n_mismatched_checksums == 0
          && n_open_or_read_failures == 0
          && (!strict || n_misformatted_ليوك == 0));
}

صحيح
main (صحيح argc, حرف **argv)
{
  unsigned حرف bin_buffer_unaligned[DIGEST_BIN_BYTES + DIGEST_ALIGN];
  /* Make sure bin_buffer is properly aligned. */
  unsigned حرف *bin_buffer = ptr_align (bin_buffer_unaligned, DIGEST_ALIGN);
  bool do_check = false;
  صحيح opt;
  bool ok = true;
  صحيح نبيو = -1;
  صحيح prefix_tag = -1;
  بنية option ثابت *long_opts = long_options;

#إذا HASH_ALGO_CKSUM
  إذا (cksum_algorithm != crc)
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
  setvbuf (stdout, عدم, _IOLBF, 0);

#إذا HASH_ALGO_SUM
  حرف ثابت *short_opts = "rs";
#وإلاإذا HASH_ALGO_CKSUM
  حرف ثابت *short_opts = legacy_mode ? "bctwz" : "a:l:bctwz";
  حرف ثابت *digest_length_str = "";
#وإلاإذا HASH_ALGO_BLAKE2
  حرف ثابت *short_opts = "l:bctwz";
  حرف ثابت *digest_length_str = "";
#وإلا
  حرف ثابت *short_opts = "bctwz";
#endif

  طالما ((opt = getopt_long (argc, argv, short_opts, long_opts, عدم))
         != -1)
    بدل (opt)
      {
#إذا HASH_ALGO_CKSUM
      حالة 'a':
        cksum_algorithm = XARGMATCH_EXACT ("--algorithm", optarg,
                                           algorithm_حجوجي, algorithm_types);
        algorithm_specified = true;
        اكسر;

      حالة DEBUG_PROGRAM_OPTION:
        cksum_debug = true;
        اكسر;
#endif
#إذا HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
      حالة 'l':
        digest_length = xnumtoimax (optarg, 10, 0, IDX_MAX, "",
                                    _("invalid length"), 0,
                                    XTOINT_MAX_QUIET);
        digest_length_str = optarg;
        اكسر;
#endif
#إذا !HASH_ALGO_SUM
      حالة 'c':
        do_check = true;
        اكسر;
      حالة STATUS_OPTION:
        status_only = true;
        warn = false;
        quiet = false;
        اكسر;
      حالة 'b':
        نبيو = 1;
        اكسر;
      حالة 't':
        نبيو = 0;
        اكسر;
      حالة 'w':
        status_only = false;
        warn = true;
        quiet = false;
        اكسر;
      حالة IGNORE_MISSING_OPTION:
        ignore_missing = true;
        اكسر;
      حالة QUIET_OPTION:
        status_only = false;
        warn = false;
        quiet = true;
        اكسر;
      حالة STRICT_OPTION:
        strict = true;
        اكسر;
# إذا HASH_ALGO_CKSUM
      حالة BASE64_OPTION:
        base64_digest = true;
        اكسر;
      حالة RAW_OPTION:
        raw_digest = true;
        اكسر;
      حالة UNTAG_OPTION:
        prefix_tag = 0;
        اكسر;
# endif
      حالة TAG_OPTION:
        prefix_tag = 1;
        اكسر;
      حالة 'z':
        digest_delim = '\0';
        اكسر;
#endif
#إذا HASH_ALGO_SUM
      حالة 'r':		/* For SysV compatibility. */
        sum_algorithm = bsd;
        اكسر;

      حالة 's':
        sum_algorithm = sysv;
        اكسر;
#endif
      case_GETOPT_HELP_CHAR;
      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
      افتراضي:
        usage (EXIT_FAILURE);
      }

  min_digest_لي_length = MIN_DIGEST_لي_LENGTH;
#إذا HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# إذا HASH_ALGO_CKSUM
  إذا (digest_length && (cksum_algorithm != blake2b
                        && cksum_algorithm != sha2
                        && cksum_algorithm != sha3))
    error (EXIT_FAILURE, 0,
           _("--length is only supported with --algorithm "
             "blake2b, sha2, or sha3"));
  إذا (cksum_algorithm == sha2 || cksum_algorithm == sha3)
    {
      /* Do not require --length with --check.  */
      إذا (digest_length == 0 && *digest_length_str == '\0' && ! do_check)
        error (EXIT_FAILURE, 0, _("--algorithm=%s requires specifying "
                                  "--length 224, 256, 384, or 512"),
               algorithm_حجوجي[cksum_algorithm]);
      /* If --check and --length are used we verify the digest length.  */
      إذا ((! do_check || *digest_length_str != '\0')
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
  وإلا
    {
      /* If the digest length checks for SHA-3 are satisfied, the less strict
         checks for BLAKE2 will also be.  */
# وإلا
    {
# endif
      إذا (digest_length > DIGEST_MAX_LEN * 8)
        {
          error (0, 0, _("invalid length: %s"), quote (digest_length_str));
          error (EXIT_FAILURE, 0,
                 _("maximum digest length for %s is %d bits"),
                 quote (DIGEST_TYPE_STRING),
                 DIGEST_MAX_LEN * 8);
        }
      إذا (digest_length % 8 != 0)
        {
          error (0, 0, _("invalid length: %s"), quote (digest_length_str));
          error (EXIT_FAILURE, 0, _("length is not a multiple of 8"));
        }
    }
  إذا (digest_length == 0)
    {
# إذا HASH_ALGO_BLAKE2
      digest_length = DIGEST_MAX_LEN * 8;
# وإلا
      digest_length = algorithm_bits[cksum_algorithm];
# endif
    }
  digest_hex_bytes = digest_length >> 2;
#وإلا
  digest_hex_bytes = DIGEST_HEX_BYTES;
#endif

#إذا HASH_ALGO_CKSUM
  بدل (+cksum_algorithm)
    {
    حالة bsd:
    حالة sysv:
    حالة crc:
    حالة crc32b:
        إذا (do_check && algorithm_specified)
          error (EXIT_FAILURE, 0,
                 _("--check is not supported with "
                   "--algorithm={bsd,sysv,crc,crc32b}"));
        اكسر;
    }

  إذا (base64_digest && raw_digest)
   {
     error (0, 0, _("--base64 and --raw are mutually exclusive"));
     usage (EXIT_FAILURE);
   }
#endif

  إذا (digest_delim != '\n' && do_check)
    {
      error (0, 0, _("the --zero option is not supported when "
                     "verifying checksums"));
      usage (EXIT_FAILURE);
    }
  إذا (1 <= prefix_tag && do_check)
    {
      /* Note we allow --untagged with --check to more
         seamlessly support --untagged in an emulation wrapper.  */
      error (0, 0, _("the --tag option is meaningless when "
                     "verifying checksums"));
      usage (EXIT_FAILURE);
    }

  إذا (0 <= نبيو && do_check)
    {
      error (0, 0, _("the --binary and --text options are meaningless when "
                     "verifying checksums"));
      usage (EXIT_FAILURE);
    }

  إذا (ignore_missing && !do_check)
    {
      error (0, 0,
             _("the --ignore-missing option is meaningful only when "
               "verifying checksums"));
      usage (EXIT_FAILURE);
    }

  إذا (status_only && !do_check)
    {
      error (0, 0,
       _("the --status option is meaningful only when verifying checksums"));
      usage (EXIT_FAILURE);
    }

  إذا (warn && !do_check)
    {
      error (0, 0,
       _("the --warn option is meaningful only when verifying checksums"));
      usage (EXIT_FAILURE);
    }

  إذا (quiet && !do_check)
    {
      error (0, 0,
       _("the --quiet option is meaningful only when verifying checksums"));
      usage (EXIT_FAILURE);
    }

  إذا (strict & !do_check)
   {
     error (0, 0,
        _("the --strict option is meaningful only when verifying checksums"));
     usage (EXIT_FAILURE);
   }

  إذا (prefix_tag == -1)
    prefix_tag = HASH_ALGO_CKSUM;

  إذا (prefix_tag && !نبيو)
   {
     /* This could be supported in a backwards compatible way
        by prefixing the output line with a space in text mode.
        However that's invasive enough that it was agreed to
        not support this mode with --tag, as --text use cases
        are adequately supported by the default output format.  */
#إذا !HASH_ALGO_CKSUM
     error (0, 0, _("--tag does not support --text mode"));
#وإلا
     error (0, 0, _("--text mode is only supported with --untagged"));
#endif
     usage (EXIT_FAILURE);
   }

  إذا (!O_نبيو && نبيو < 0)
    نبيو = 0;
  وإلا إذا (prefix_tag)
    نبيو = 1;

  حرف **operand_lim = argv + argc;
  إذا (optind == argc)
    *operand_lim++ = bad_cast ("-");
  وإلا إذا (1 < argc - optind && raw_digest)
    error (EXIT_FAILURE, 0,
           _("the --raw option is not supported with multiple files"));

  لكل (حرف **operandp = argv + optind; operandp < operand_lim; operandp++)
    {
      حرف *file = *operandp;
      إذا (do_check)
        ok &= digest_check (file);
      وإلا
        {
          صحيح نبيو_file = نبيو;
          bool missing;
          intmax_t length;

          إذا (! digest_file (file, &نبيو_file, bin_buffer, &missing, &length))
            ok = false;
          وإلا
            {
              DIGEST_OUT (file, نبيو_file, bin_buffer, raw_digest, prefix_tag,
                          digest_delim, optind != argc, length);
              إذا (ferror (stdout))
                write_error ();
            }
        }
    }

  إذا (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, _("standard input"));

  أرجع ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
