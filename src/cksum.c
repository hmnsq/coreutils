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

#شامل <config.h>

#شامل <getopt.h>
#شامل <sys/types.h>

#شامل "assure.h"
#شامل "system.h"
#شامل "argmatch.h"
#شامل "c-ctype.h"
#شامل "quote.h"
#شامل "xdectoint.h"
#شامل "xstrtol.h"

#ifndef HASH_ALGO_CKSUM
# define HASH_ALGO_CKSUM 0
#endif

#اگر HASH_ALGO_SUM || HASH_ALGO_CKSUM
# include "sum.h"
#endif
#اگر HASH_ALGO_CKSUM
# include "cksum.h"
# include "cksum_crc.h"
# include "base64.h"
#endif
#اگر HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# include "blake2/b2sum.h"
#endif
#اگر HASH_ALGO_MD5 || HASH_ALGO_CKSUM
# include "md5.h"
#endif
#اگر HASH_ALGO_SHA1 || HASH_ALGO_CKSUM
# include "sha1.h"
#endif
#اگر HASH_ALGO_SHA256 || HASH_ALGO_SHA224 || HASH_ALGO_CKSUM
# include "sha256.h"
#endif
#اگر HASH_ALGO_SHA512 || HASH_ALGO_SHA384 || HASH_ALGO_CKSUM
# include "sha512.h"
#endif
#اگر HASH_ALGO_CKSUM
# include "sha3.h"
#endif
#اگر HASH_ALGO_CKSUM
# include "sm3.h"
#endif
#شامل "fadvise.h"
#شامل "stdio--.h"
#شامل "xbinary-io.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#اگر HASH_ALGO_SUM
# define PROGRAM_NAME "sum"
# define DIGEST_TYPE_STRING "BSD"
# define DIGEST_STREAM sumfns[sum_algorithm]
# define DIGEST_OUT sum_output_fns[sum_algorithm]
# define DIGEST_BITS 16
# define DIGEST_ALIGN 4
#وگرنهاگر HASH_ALGO_CKSUM
# define MAX_DIGEST_BITS 512
# define MAX_DIGEST_ALIGN 8
# define DIGEST_TYPE_STRING algorithm_tags[cksum_algorithm]
# define DIGEST_STREAM cksumfns[cksum_algorithm]
# define DIGEST_OUT cksum_output_fns[cksum_algorithm]
# define DIGEST_BITS MAX_DIGEST_BITS
# define DIGEST_ALIGN MAX_DIGEST_ALIGN
#وگرنهاگر HASH_ALGO_MD5
# define PROGRAM_NAME "md5sum"
# define DIGEST_TYPE_STRING "MD5"
# define DIGEST_STREAM md5_stream
# define DIGEST_BITS 128
# define DIGEST_REFERENCE "RFC 1321"
# define DIGEST_ALIGN 4
#وگرنهاگر HASH_ALGO_BLAKE2
# define PROGRAM_NAME "b2sum"
# define DIGEST_TYPE_STRING "BLAKE2b"
# define DIGEST_STREAM blake2b_stream
# define DIGEST_BITS 512
# define DIGEST_REFERENCE "RFC 7693"
# define DIGEST_ALIGN 8
#وگرنهاگر HASH_ALGO_SHA1
# define PROGRAM_NAME "sha1sum"
# define DIGEST_TYPE_STRING "SHA1"
# define DIGEST_STREAM sha1_stream
# define DIGEST_BITS 160
# define DIGEST_REFERENCE "FIPS-180-1"
# define DIGEST_ALIGN 4
#وگرنهاگر HASH_ALGO_SHA256
# define PROGRAM_NAME "sha256sum"
# define DIGEST_TYPE_STRING "SHA256"
# define DIGEST_STREAM sha256_stream
# define DIGEST_BITS 256
# define DIGEST_REFERENCE "FIPS-180-2"
# define DIGEST_ALIGN 4
#وگرنهاگر HASH_ALGO_SHA224
# define PROGRAM_NAME "sha224sum"
# define DIGEST_TYPE_STRING "SHA224"
# define DIGEST_STREAM sha224_stream
# define DIGEST_BITS 224
# define DIGEST_REFERENCE "RFC 3874"
# define DIGEST_ALIGN 4
#وگرنهاگر HASH_ALGO_SHA512
# define PROGRAM_NAME "sha512sum"
# define DIGEST_TYPE_STRING "SHA512"
# define DIGEST_STREAM sha512_stream
# define DIGEST_BITS 512
# define DIGEST_REFERENCE "FIPS-180-2"
# define DIGEST_ALIGN 8
#وگرنهاگر HASH_ALGO_SHA384
# define PROGRAM_NAME "sha384sum"
# define DIGEST_TYPE_STRING "SHA384"
# define DIGEST_STREAM sha384_stream
# define DIGEST_BITS 384
# define DIGEST_REFERENCE "FIPS-180-2"
# define DIGEST_ALIGN 8
#وگرنه
# error "Can't decide which hash algorithm to compile."
#endif
#اگر !HASH_ALGO_SUM && !HASH_ALGO_CKSUM
# define DIGEST_OUT output_file
#endif

#اگر HASH_ALGO_CKSUM
# define PROGRAM_NAME (!legacy_mode ? "cksum"			 \
                       : cksum_algorithm == md5 ? "md5sum"	 \
                       : cksum_algorithm == sha1 ? "sha1sum"	 \
                       : cksum_algorithm == sha224 ? "sha224sum" \
                       : cksum_algorithm == sha256 ? "sha256sum" \
                       : cksum_algorithm == sha384 ? "sha384sum" \
                       : cksum_algorithm == sha512 ? "sha512sum" \
                       : "cksum")
#endif

#اگر HASH_ALGO_SUM
# define AUTHORS \
  proper_name ("Kayvan Aghaiepour"), \
  proper_name ("David MacKenzie")
#وگرنهاگر HASH_ALGO_CKSUM
# define AUTHORS \
  proper_name_lite ("Padraig Brady", "P\303\241draig Brady"), \
  proper_name ("Q. Frank Xia")
#وگرنهاگر HASH_ALGO_BLAKE2
# define AUTHORS \
  proper_name_lite ("Padraig Brady", "P\303\241draig Brady"), \
  proper_name ("Samuel Neves")
#وگرنه
# define AUTHORS \
  proper_name ("Ulrich Drepper"), \
  proper_name ("Scott Miller"), \
  proper_name ("David Madore")
#endif
#اگر !HASH_ALGO_BLAKE2 && !HASH_ALGO_CKSUM
# define DIGEST_HEX_BYTES (DIGEST_BITS / 4)
#endif
#define DIGEST_BIN_BYTES (DIGEST_BITS / 8)

/* The minimum length of a valid digest line.  This length does
   not include any newline character at the end of a line.  */
#اگر HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# define MIN_DIGEST_سطر_LENGTH 3 /* With -l 8.  */
#وگرنه
# define MIN_DIGEST_سطر_LENGTH \
   (DIGEST_HEX_BYTES /* length of hexadecimal message digest */ \
    + 1 /* blank */ \
    + 1 /* minimum filename length */ )
#endif

#اگر !HASH_ALGO_SUM
ایستا تهی
output_file (نویسه ثابت *file, صحیح دودویی_file, تهی ثابت *digest,
             bool raw, bool tagged, unsigned نویسه delim, bool args,
             intmax_t length);
#endif

/* True if any of the files read were the standard input. */
ایستا bool have_read_stdin;

/* The minimum length of a valid checksum line for the selected algorithm.  */
ایستا idx_t min_digest_سطر_length;

/* Set to the length of a digest hex string for the selected algorithm.  */
ایستا idx_t digest_hex_bytes;

/* With --check, don't generate any output.
   The exit code indicates success or failure.  */
ایستا bool status_only = false;

/* With --check, print a message to standard error warning about each
   improperly formatted checksum line.  */
ایستا bool warn = false;

/* With --check, ignore missing files.  */
ایستا bool ignore_missing = false;

/* With --check, suppress the "OK" printed for each verified file.  */
ایستا bool quiet = false;

/* With --check, exit with a non-zero return code if any line is
   improperly formatted. */
ایستا bool strict = false;

/* Whether a BSD reversed format checksum is detected.  */
ایستا صحیح bsd_reversed = -1;

/* line delimiter.  */
ایستا unsigned نویسه digest_delim = '\n';

#اگر HASH_ALGO_CKSUM
/* If true, print base64-encoded digests, not hex.  */
ایستا bool base64_digest = false;
#endif

/* If true, print binary digests, not hex.  */
ایستا bool raw_digest = false;

/* blake2b and sha3 allow the -l option.  Luckily they both have the same
   maximum digest size.  */
#اگر HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# اگر HASH_ALGO_CKSUM
static_assert (BLAKE2B_OUTBYTES == SHA3_512_DIGEST_SIZE);
# endif
# define DIGEST_MAX_LEN BLAKE2B_OUTBYTES
ایستا idx_t digest_length;
#endif /* HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM */

تعریفنوع تهی (*digest_output_fn)(نویسه ثابت *, صحیح, تهی ثابت *, bool,
                                 bool, unsigned نویسه, bool, intmax_t);
#اگر HASH_ALGO_SUM
شمارش Algorithm
{
  bsd,
  sysv,
};

ایستا شمارش Algorithm sum_algorithm;
ایستا sumfn sumfns[]=
{
  bsd_sum_stream,
  sysv_sum_stream,
};
ایستا digest_output_fn sum_output_fns[]=
{
  output_bsd,
  output_sysv,
};
#endif

#اگر HASH_ALGO_CKSUM
ایستا صحیح
md5_sum_stream (FILE *stream, تهی *resstream, MAYBE_UNUSED intmax_t *length)
{
  بازگشت md5_stream (stream, resstream);
}
ایستا صحیح
sha1_sum_stream (FILE *stream, تهی *resstream, MAYBE_UNUSED intmax_t *length)
{
  بازگشت sha1_stream (stream, resstream);
}
ایستا صحیح
sha224_sum_stream (FILE *stream, تهی *resstream,
                   MAYBE_UNUSED intmax_t *length)
{
  بازگشت sha224_stream (stream, resstream);
}
ایستا صحیح
sha256_sum_stream (FILE *stream, تهی *resstream,
                   MAYBE_UNUSED intmax_t *length)
{
  بازگشت sha256_stream (stream, resstream);
}
ایستا صحیح
sha384_sum_stream (FILE *stream, تهی *resstream,
                   MAYBE_UNUSED intmax_t *length)
{
  بازگشت sha384_stream (stream, resstream);
}
ایستا صحیح
sha512_sum_stream (FILE *stream, تهی *resstream,
                   MAYBE_UNUSED intmax_t *length)
{
  بازگشت sha512_stream (stream, resstream);
}
ایستا صحیح
sha2_sum_stream (FILE *stream, تهی *resstream, intmax_t *length)
{
  انتخاب (*length)
    {
    حالت SHA224_DIGEST_SIZE:
      بازگشت sha224_stream (stream, resstream);
    حالت SHA256_DIGEST_SIZE:
      بازگشت sha256_stream (stream, resstream);
    حالت SHA384_DIGEST_SIZE:
      بازگشت sha384_stream (stream, resstream);
    حالت SHA512_DIGEST_SIZE:
      بازگشت sha512_stream (stream, resstream);
    پیشفرض:
      affirm (false);
    }
}
ایستا صحیح
sha3_sum_stream (FILE *stream, تهی *resstream, intmax_t *length)
{
  انتخاب (*length)
    {
    حالت SHA3_224_DIGEST_SIZE:
      بازگشت sha3_224_stream (stream, resstream);
    حالت SHA3_256_DIGEST_SIZE:
      بازگشت sha3_256_stream (stream, resstream);
    حالت SHA3_384_DIGEST_SIZE:
      بازگشت sha3_384_stream (stream, resstream);
    حالت SHA3_512_DIGEST_SIZE:
      بازگشت sha3_512_stream (stream, resstream);
    پیشفرض:
      affirm (false);
    }
}
ایستا صحیح
blake2b_sum_stream (FILE *stream, تهی *resstream, intmax_t *length)
{
  بازگشت blake2b_stream (stream, resstream, *length);
}
ایستا صحیح
sm3_sum_stream (FILE *stream, تهی *resstream, MAYBE_UNUSED intmax_t *length)
{
  بازگشت sm3_stream (stream, resstream);
}

ایستا نویسه ثابت *ثابت algorithm_args[] =
{
  "bsd", "sysv", "crc", "crc32b", "md5", "sha1",
  "sha224", "sha256", "sha384", "sha512", /* Legacy naming */
  "sha2", "sha3", "blake2b", "sm3", پوچ
};
ایستا شمارش Algorithm ثابت algorithm_types[] =
{
  bsd, sysv, crc, crc32b, md5, sha1,
  sha224, sha256, sha384, sha512,
  sha2, sha3, blake2b, sm3,
};
ARGMATCH_VERIFY (algorithm_args, algorithm_types);

ایستا نویسه ثابت *ثابت algorithm_tags[] =
{
  "BSD", "SYSV", "CRC", "CRC32B", "MD5", "SHA1",
  "SHA224", "SHA256", "SHA384", "SHA512",
  "SHA2", "SHA3", "BLAKE2b", "SM3", پوچ
};
ایستا صحیح ثابت algorithm_bits[] =
{
  16, 16, 32, 32, 128, 160,
  224, 256, 384, 512,
  512, 512, 512, 256, 0
};

static_assert (countof (algorithm_bits) == countof (algorithm_args));

ایستا bool algorithm_specified = false;
ایستا bool legacy_mode = false;
شمارش Algorithm cksum_algorithm = crc;

ایستا sumfn cksumfns[]=
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
ایستا digest_output_fn cksum_output_fns[]=
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

شمارش
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

#اگر HASH_ALGO_CKSUM
ایستا ساختار option ثابت legacy_long_options[] =
{
  { "check", no_argument, پوچ, 'c' },
  { "ignore-missing", no_argument, پوچ, IGNORE_MISSING_OPTION },
  { "quiet", no_argument, پوچ, QUIET_OPTION },
  { "status", no_argument, پوچ, STATUS_OPTION },
  { "warn", no_argument, پوچ, 'w' },
  { "strict", no_argument, پوچ, STRICT_OPTION },
  { "tag", no_argument, پوچ, TAG_OPTION },
  { "zero", no_argument, پوچ, 'z' },

  { "binary", no_argument, پوچ, 'b' }, /* Deprecated.  */
  { "text", no_argument, پوچ, 't' }, /* Deprecated.  */

  { GETOPT_HELP_OPTION_DECL },
  { GETOPT_VERSION_OPTION_DECL },
  { پوچ, 0, پوچ, 0 }
};
#endif

ایستا ساختار option ثابت long_options[] =
{
#اگر HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
  { "length", required_argument, پوچ, 'l'},
#endif

#اگر !HASH_ALGO_SUM
  { "check", no_argument, پوچ, 'c' },
  { "ignore-missing", no_argument, پوچ, IGNORE_MISSING_OPTION},
  { "quiet", no_argument, پوچ, QUIET_OPTION },
  { "status", no_argument, پوچ, STATUS_OPTION },
  { "warn", no_argument, پوچ, 'w' },
  { "strict", no_argument, پوچ, STRICT_OPTION },
  { "tag", no_argument, پوچ, TAG_OPTION },
  { "zero", no_argument, پوچ, 'z' },

# اگر HASH_ALGO_CKSUM
  { "algorithm", required_argument, پوچ, 'a'},
  { "base64", no_argument, پوچ, BASE64_OPTION },
  { "debug", no_argument, پوچ, DEBUG_PROGRAM_OPTION},
  { "raw", no_argument, پوچ, RAW_OPTION},
  { "untagged", no_argument, پوچ, UNTAG_OPTION },
# endif
  { "binary", no_argument, پوچ, 'b' }, /* Deprecated.  */
  { "text", no_argument, پوچ, 't' }, /* Deprecated.  */

#وگرنه
  {"sysv", no_argument, پوچ, 's'},  /* Not in cksum.  */
#endif

  { GETOPT_HELP_OPTION_DECL },
  { GETOPT_VERSION_OPTION_DECL },
  { پوچ, 0, پوچ, 0 }
};

تهی
usage (صحیح status)
{
  اگر (status != EXIT_SUCCESS)
    emit_try_help ();
  وگرنه
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"), program_name);
#اگر HASH_ALGO_CKSUM
    اگر (legacy_mode)
      {
        printf (_("\
Print or check %s checksums.\n\
"), DIGEST_TYPE_STRING);
        fputs (_("\
Legacy interface to the cksum(1) utility.\n\
"), stdout);
      }
    وگرنه
      fputs (_("\
Print or verify checksums.\n\
By پیشفرض use the 32 bit CRC algorithm.\n\
"), stdout);
#وگرنه
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
#اگر HASH_ALGO_SUM
      oputs (_("\
  -r\n\
         use BSD sum algorithm (the پیشفرض), use 1K blocks\n\
"));
      oputs (_("\
  -s, --sysv\n\
         use System V sum algorithm, use 512 bytes blocks\n\
"));
#endif
#اگر HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
        emit_mandatory_arg_note ();
#endif
#اگر HASH_ALGO_CKSUM
     اگر (! legacy_mode)
        oputs (_("\
  -a, --algorithm=TYPE\n\
         select the digest type to use.  See DIGEST below\n\
"));
     اگر (! legacy_mode)
        oputs (_("\
      --base64\n\
         emit base64-encoded digests, not hexadecimal\n\
"));
#endif
#اگر !HASH_ALGO_SUM
# اگر HASH_ALGO_CKSUM
     اگر (legacy_mode)
       {
# endif
      اگر (O_دودویی)
        oputs (_("\
  -b, --دودویی\n\
         اقرأ in دودویی mode (پیشفرض unless reading tty stdin)\n\
"));
      وگرنه
        oputs (_("\
  -b, --دودویی\n\
         اقرأ in دودویی mode\n\
"));
# اگر HASH_ALGO_CKSUM
       }
# endif
        oputs (_("\
  -c, --check\n\
         اقرأ checksums from the FILEs and check them\n\
"));
# اگر HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
#  اگر HASH_ALGO_CKSUM
     اگر (! legacy_mode)
#  endif
        oputs (_("\
  -l, --length=BITS\n\
         digest length in bits; must not exceed the max size\n\
         and must be a multiple of 8 برای blake2b;\n\
         must be 224, 256, 384, or 512 برای sha2 or sha3\n\
"));
# endif
# اگر HASH_ALGO_CKSUM
    اگر (! legacy_mode)
        oputs (_("\
      --raw\n\
         emit a raw دودویی digest, not hexadecimal\n\
"));
    اگر (legacy_mode)
      oputs (_("\
      --tag\n\
         create a BSD-style checksum\n\
"));
    وگرنه
      oputs (_("\
      --tag\n\
         create a BSD-style checksum (the پیشفرض)\n\
"));
    اگر (! legacy_mode)
      oputs (_("\
      --untagged\n\
         create a reversed style checksum, without digest type\n\
"));
# وگرنه
      oputs (_("\
      --tag\n\
         create a BSD-style checksum\n\
"));
# endif
# اگر HASH_ALGO_CKSUM
    اگر (legacy_mode)
      {
# endif
      اگر (O_دودویی)
        oputs (_("\
  -t, --text\n\
         اقرأ in text mode (پیشفرض اگر reading tty stdin)\n\
"));
      وگرنه
        oputs (_("\
  -t, --text\n\
         اقرأ in text mode (پیشفرض)\n\
"));
# اگر HASH_ALGO_CKSUM
      }
# endif
      oputs (_("\
  -z, --zero\n\
         end each output سطر with NUL, not newline,\n\
         and disable file name escaping\n\
"));
      fputs (_("\
\n\
The following five options are useful only when verifying checksums:\n\
"), stdout);
      oputs (_("\
      --ignore-missing\n\
         don't fail or report status برای missing files\n\
"));
      oputs (_("\
      --quiet\n\
         don't print OK برای each successfully verified file\n\
"));
      oputs (_("\
      --status\n\
         don't output anything, status code shows success\n\
"));
      oputs (_("\
      --strict\n\
         exit non-zero برای improperly formatted checksum سطرها\n\
"));
      oputs (_("\
  -w, --warn\n\
         warn about improperly formatted checksum سطرها\n\
"));
#endif
#اگر HASH_ALGO_CKSUM
    اگر (! legacy_mode)
      oputs (_("\
      --debug\n\
         indicate which implementation used\n\
"));
#endif
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
#اگر HASH_ALGO_CKSUM
    اگر (! legacy_mode)
      fputs (_("\
\n\
DIGEST determines the digest algorithm and پیشفرض output format:\n\
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
#اگر !HASH_ALGO_SUM && !HASH_ALGO_CKSUM
      printf (_("\
\n\
The sums are computed as described in %s.\n"), DIGEST_REFERENCE);
      fputs (_("\
When checking, the درون_داد should be a former output of this program.\n\
The پیشفرض mode is to print a سطر with: checksum, a space,\n\
a character indicating درون_داد mode ('*' برای دودویی, ' ' برای text\n\
or where دودویی is insignificant), and name برای each FILE.\n\
\n\
There is no difference between دودویی mode and text mode on GNU systems.\
\n"), stdout);
#endif
#اگر HASH_ALGO_CKSUM
      fputs (_("\
When checking, the درون_داد should be a former output of this program,\n\
or equivalent standalone program.\
\n"), stdout);
#endif
      emit_ancillary_info (PROGRAM_NAME);
    }

  exit (status);
}

#اگر !HASH_ALGO_SUM
/* Given a string S, return TRUE if it contains problematic characters
   that need escaping.  Note we escape '\' itself to provide some forward
   compat to introduce escaping of other characters.  */

ATTRIBUTE_PURE
ایستا bool
problematic_chars (نویسه ثابت *s)
{
  idx_t length = strcspn (s, "\\\n\r");
  بازگشت s[length] != '\0';
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

ایستا نویسه *
filename_unescape (نویسه *s, idx_t s_len)
{
  نویسه *dst = s;

  برای (idx_t i = 0; i < s_len; i++)
    {
      انتخاب (s[i])
        {
        حالت '\\':
          اگر (i == s_len - 1)
            {
              /* File name ends with an unescaped backslash: invalid.  */
              بازگشت پوچ;
            }
          ++i;
          انتخاب (s[i])
            {
            حالت 'n':
              *dst++ = '\n';
              بشکن;
            حالت 'r':
              *dst++ = '\r';
              بشکن;
            حالت '\\':
              *dst++ = '\\';
              بشکن;
            پیشفرض:
              /* Only '\', 'n' or 'r' may follow a backslash.  */
              بازگشت پوچ;
            }
          بشکن;

        حالت '\0':
          /* The file name may not contain a NUL.  */
          بازگشت پوچ;

        پیشفرض:
          *dst++ = s[i];
          بشکن;
        }
    }
  اگر (dst < s + s_len)
    *dst = '\0';

  بازگشت s;
}

/* Return true if S is a LEN-byte NUL-terminated string of hex or base64
   digits and has the expected length.  Otherwise, return false.  */
ATTRIBUTE_PURE
ایستا bool
valid_digits (unsigned نویسه ثابت *s, idx_t len)
{
#اگر HASH_ALGO_CKSUM
  اگر (len == BASE64_LENGTH (digest_length >> 3))
    {
      idx_t i;
      برای (i = 0; i < len - digest_length % 3; i++)
        {
          اگر (!isbase64 (*s))
            بازگشت false;
          ++s;
        }
      برای ( ; i < len; i++)
        {
          اگر (*s != '=')
            بازگشت false;
          ++s;
        }
    }
  وگرنه
#endif
  اگر (len == digest_hex_bytes)
    {
      برای (idx_t i = 0; i < digest_hex_bytes; i++)
        {
          اگر (!c_isxdigit (*s))
            بازگشت false;
          ++s;
        }
    }
  وگرنه
    بازگشت false;

  بازگشت *s == '\0';
}

/* Split the checksum string S (of length S_LEN) from a BSD 'md5' or
   'sha1' command into two parts: a hexadecimal digest, and the file
   name.  S is modified.  Set *D_LEN to the length of the digest string.
   Return true if successful.  */

ایستا bool
bsd_split_3 (نویسه *s, idx_t s_len,
             unsigned نویسه **digest, idx_t *d_len,
             نویسه **file_name, bool escaped_filename)
{
  اگر (s_len == 0)
    بازگشت false;

  /* Find end of filename.  */
  idx_t i = s_len - 1;
  وقتی (i && s[i] != ')')
    i--;

  اگر (s[i] != ')')
    بازگشت false;

  *file_name = s;

  اگر (escaped_filename && filename_unescape (s, i) == پوچ)
    بازگشت false;

  s[i++] = '\0';

  وقتی (c_isblank (s[i]))
    i++;

  اگر (s[i] != '=')
    بازگشت false;

  i++;

  وقتی (c_isblank (s[i]))
    i++;

  *digest = (unsigned نویسه *) &s[i];

  *d_len = s_len - i;
  بازگشت valid_digits (*digest, *d_len);
}

#اگر HASH_ALGO_CKSUM
/* Return the corresponding Algorithm for the string S,
   or -1 for no match.  */

ایستا ptrdiff_t
algorithm_from_tag (نویسه *s)
{
  /* Limit check size to this length for perf reasons.  */
  ایستا idx_t max_tag_len;
  اگر (! max_tag_len)
    {
      نویسه ثابت * ثابت * tag = algorithm_tags;
      وقتی (*tag)
        {
          idx_t tag_len = strlen (*tag++);
          max_tag_len = MAX (tag_len, max_tag_len);
        }
    }

  idx_t i = 0;

  /* Find end of tag */
  وقتی (i <= max_tag_len && s[i] && ! c_isblank (s[i])
         && s[i] != '-' && s[i] != '(')
    ++i;

  اگر (i > max_tag_len)
    بازگشت -1;

  /* Terminate tag, and lookup.  */
  نویسه sep = s[i];
  s[i] = '\0';
  ptrdiff_t algo = argmatch_exact (s, algorithm_tags);
  s[i] = sep;

  بازگشت algo;
}
#endif

/* Split the string S (of length S_LEN) into three parts:
   a hexadecimal digest, binary flag, and the file name.
   S is modified.  Set *D_LEN to the length of the digest string.
   Return true if successful.  */

ایستا bool
split_3 (نویسه *s, idx_t s_len,
         unsigned نویسه **digest, idx_t *d_len, صحیح *دودویی, نویسه **file_name)
{
  bool escaped_filename = false;
  idx_t algo_name_len;

  idx_t i = 0;
  وقتی (c_isblank (s[i]))
    ++i;

  اگر (s[i] == '\\')
    {
      ++i;
      escaped_filename = true;
    }

  /* Check for BSD-style checksum line. */

#اگر HASH_ALGO_CKSUM
  اگر (! algorithm_specified || cksum_algorithm == sha2)
    {
      ptrdiff_t algo_tag = algorithm_from_tag (s + i);
      اگر (! algorithm_specified)
        {
          اگر (algo_tag >= 0)
            {
              اگر (algo_tag <= crc32b)
                بازگشت false;  /* We don't support checking these formats.  */
              cksum_algorithm = algo_tag;
            }
          وگرنه
            بازگشت false;      /* We only support tagged format without -a.  */
        }
      وگرنه
        {
          اگر (cksum_algorithm == sha2 && (algo_tag == sha2
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
  اگر (STREQ_LEN (s + i, DIGEST_TYPE_STRING, algo_name_len))
    {
      i += algo_name_len;
#اگر HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM

# اگر HASH_ALGO_BLAKE2
      digest_length = DIGEST_MAX_LEN * 8;
# وگرنه
      digest_length = algorithm_bits[cksum_algorithm];
# endif
      اگر (s[i] == '-')  /* length specified. Not base64 */
        {
          ++i;
          intmax_t length;
          نویسه *siend;
          اگر (!c_isdigit (s[i])
              || xstrtoimax (s + i, &siend, 0, &length, پوچ) != LONGINT_OK)
            بازگشت false;
# اگر HASH_ALGO_CKSUM
          وگرنه اگر (cksum_algorithm == sha2 || cksum_algorithm == sha3)
            {
              اگر (length != SHA224_DIGEST_SIZE * 8
                  && length != SHA256_DIGEST_SIZE * 8
                  && length != SHA384_DIGEST_SIZE * 8
                  && length != SHA512_DIGEST_SIZE * 8)
                بازگشت false;
            }
# endif
          وگرنه اگر (!(0 < length && length <= digest_length && length % 8 == 0))
            بازگشت false;

          i = siend - s;
          digest_length = length;
        }
      digest_hex_bytes = digest_length >> 2;
#endif
      اگر (s[i] == ' ')
        ++i;
      اگر (s[i] == '(')  /* not base64 */
        {
          ++i;
          *دودویی = 0;
          بازگشت bsd_split_3 (s + i, s_len - i,
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
  اگر (s_len - i < min_digest_سطر_length + (s[i] == '\\'))
    بازگشت false;

  *digest = (unsigned نویسه *) &s[i];

#اگر HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
  /* Auto determine length.  */
# اگر HASH_ALGO_CKSUM
  اگر (cksum_algorithm == blake2b
      || cksum_algorithm == sha2 || cksum_algorithm == sha3) {
# endif
  unsigned نویسه ثابت *hp = *digest;
  digest_hex_bytes = 0;
  برای (; c_isxdigit (*hp); ++hp, ++digest_hex_bytes)
    ;
# اگر HASH_ALGO_CKSUM
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
  برای (; isubase64 (*hp); ++hp, ++digest_base64_bytes)
    ;
  برای (; *hp == '='; ++hp, ++trailing_equals)
    ;
  اگر ((cksum_algorithm == sha2 || cksum_algorithm == sha3)
      && digest_hex_bytes >> 1 != SHA224_DIGEST_SIZE
      && digest_hex_bytes >> 1 != SHA256_DIGEST_SIZE
      && digest_hex_bytes >> 1 != SHA384_DIGEST_SIZE
      && digest_hex_bytes >> 1 != SHA512_DIGEST_SIZE)
    {
      اگر (digest_base64_bytes + trailing_equals
          == BASE64_LENGTH (SHA224_DIGEST_SIZE))
        digest_hex_bytes = SHA224_DIGEST_SIZE * 2;
      وگرنه اگر (digest_base64_bytes + trailing_equals
               == BASE64_LENGTH (SHA256_DIGEST_SIZE))
        digest_hex_bytes = SHA256_DIGEST_SIZE * 2;
      وگرنه اگر (digest_base64_bytes + trailing_equals
               == BASE64_LENGTH (SHA384_DIGEST_SIZE))
        digest_hex_bytes = SHA384_DIGEST_SIZE * 2;
      وگرنه اگر (digest_base64_bytes + trailing_equals
               == BASE64_LENGTH (SHA512_DIGEST_SIZE))
        digest_hex_bytes = SHA512_DIGEST_SIZE * 2;
      وگرنه
        بازگشت false;
    }
  وگرنه اگر (cksum_algorithm == blake2b
           && digest_hex_bytes < digest_base64_bytes)
    {
      برای (صحیح j = 8; j <= DIGEST_MAX_LEN * 8; j += 8)
        {
          اگر (BASE64_LENGTH (j / 8) == digest_base64_bytes + trailing_equals
              && j % 3 == trailing_equals)
            {
              digest_hex_bytes = j / 4;
              بشکن;
            }
        }
    }
# endif
  اگر (digest_hex_bytes < 2 || digest_hex_bytes % 2
      || DIGEST_MAX_LEN * 2 < digest_hex_bytes)
    بازگشت false;
  digest_length = digest_hex_bytes * 4;
# اگر HASH_ALGO_CKSUM
  }
# endif
#endif

  /* This field must be the hexadecimal or base64 representation
     of the message digest.  */
  وقتی (s[i] && !c_isblank (s[i]))
    i++;

  /* The digest must be followed by at least one whitespace character.  */
  اگر (i == s_len)
    بازگشت false;

  *d_len = &s[i] - (نویسه *) *digest;
  s[i++] = '\0';

  اگر (! valid_digits (*digest, *d_len))
    بازگشت false;

  /* If "bsd reversed" format detected.  */
  اگر ((s_len - i == 1) || (s[i] != ' ' && s[i] != '*'))
    {
      /* Don't allow mixing bsd and standard formats,
         to minimize security issues with attackers
         renaming files with leading spaces.
         This assumes that with bsd format checksums
         that the first file name does not have
         a leading ' ' or '*'.  */
      اگر (bsd_reversed == 0)
        بازگشت false;
      bsd_reversed = 1;
    }
  وگرنه اگر (bsd_reversed != 1)
    {
      bsd_reversed = 0;
      *دودویی = (s[i++] == '*');
    }

  /* All characters between the type indicator and end of line are
     significant -- that includes leading and trailing white space.  */
  *file_name = &s[i];

  اگر (escaped_filename)
    بازگشت filename_unescape (&s[i], s_len - i) != پوچ;

  بازگشت true;
}

#اگر !HASH_ALGO_SUM
/* If ESCAPE is true, then translate each:
   NEWLINE byte to the string, "\\n",
   CARRIAGE RETURN byte to the string, "\\r",
   and each backslash to "\\\\".  */
ایستا تهی
print_filename (نویسه ثابت *file, bool escape)
{
  اگر (! escape)
    {
      fputs (file, stdout);
      بازگشت;
    }

  وقتی (*file)
    {
      انتخاب (*file)
        {
        حالت '\n':
          fputs ("\\n", stdout);
          بشکن;

        حالت '\r':
          fputs ("\\r", stdout);
          بشکن;

        حالت '\\':
          fputs ("\\\\", stdout);
          بشکن;

        پیشفرض:
          putchar (*file);
          بشکن;
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

ایستا bool
digest_file (نویسه ثابت *filename, صحیح *دودویی, unsigned نویسه *bin_result,
             bool *missing, MAYBE_UNUSED intmax_t *length)
{
  FILE *fp;
  صحیح err;
  bool is_stdin = streq (filename, "-");

  *missing = false;

  اگر (is_stdin)
    {
      have_read_stdin = true;
      fp = stdin;
      اگر (O_دودویی && *دودویی)
        {
          اگر (*دودویی < 0)
            *دودویی = ! isatty (STDIN_FILENO);
          اگر (*دودویی)
            xset_دودویی_mode (STDIN_FILENO, O_دودویی);
        }
    }
  وگرنه
    {
      fp = fopen (filename, O_دودویی ? (*دودویی ? "rb" : "rt") : "r");
      اگر (fp == پوچ)
        {
          اگر (ignore_missing && errno == ENOENT)
            {
              *missing = true;
              بازگشت true;
            }
          error (0, errno, "%s", quotef (filename));
          بازگشت false;
        }
    }

  fadvise (fp, FADVISE_SEQUENTIAL);

#اگر HASH_ALGO_CKSUM
  اگر (cksum_algorithm == blake2b
      || cksum_algorithm == sha2 || cksum_algorithm == sha3)
    *length = digest_length >> 3;
  err = DIGEST_STREAM (fp, bin_result, length);
#وگرنهاگر HASH_ALGO_SUM
  err = DIGEST_STREAM (fp, bin_result, length);
#وگرنهاگر HASH_ALGO_BLAKE2
  err = DIGEST_STREAM (fp, bin_result, digest_length >> 3);
#وگرنه
  err = DIGEST_STREAM (fp, bin_result);
#endif
  err = err ? errno : 0;
  اگر (is_stdin)
    clearerr (fp);
  وگرنه اگر (fclose (fp) != 0 && !err)
    err = errno;

  اگر (err)
    {
      error (0, err, "%s", quotef (filename));
      بازگشت false;
    }

  بازگشت true;
}

#اگر !HASH_ALGO_SUM
ایستا تهی
output_file (نویسه ثابت *file, صحیح دودویی_file, تهی ثابت *digest,
             MAYBE_UNUSED bool raw, bool tagged, unsigned نویسه delim,
             MAYBE_UNUSED bool args, MAYBE_UNUSED intmax_t length)
{
# اگر HASH_ALGO_CKSUM
  اگر (raw)
    {
      fwrite (digest, 1, digest_length >> 3, stdout);
      بازگشت;
    }
# endif

  unsigned نویسه ثابت *bin_buffer = digest;

  /* Output a leading backslash if the file name contains problematic chars.  */
  bool needs_escape = delim == '\n' && problematic_chars (file);

  اگر (needs_escape)
    putchar ('\\');

  اگر (tagged)
    {
# اگر HASH_ALGO_CKSUM
      اگر (cksum_algorithm == sha2)
        printf ("SHA%u", (unsigned صحیح) digest_length);
      وگرنه
# endif
      fputs (DIGEST_TYPE_STRING, stdout);
# اگر HASH_ALGO_BLAKE2
      اگر (digest_length < DIGEST_MAX_LEN * 8)
        printf ("-%u", (unsigned صحیح) digest_length);
# elif HASH_ALGO_CKSUM
      اگر (cksum_algorithm == sha3)
        printf ("-%u", (unsigned صحیح) digest_length);
      اگر (cksum_algorithm == blake2b)
        {
          اگر (digest_length < DIGEST_MAX_LEN * 8)
            printf ("-%u", (unsigned صحیح) digest_length);
        }
# endif
      fputs (" (", stdout);
      print_filename (file, needs_escape);
      fputs (") = ", stdout);
    }

# اگر HASH_ALGO_CKSUM
  اگر (base64_digest)
    {
      نویسه b64[BASE64_LENGTH (DIGEST_BIN_BYTES) + 1];
      base64_encode ((نویسه ثابت *) bin_buffer, digest_length >> 3,
                     b64, اندازه b64);
      fputs (b64, stdout);
    }
  وگرنه
# endif
    {
      برای (idx_t i = 0; i < (digest_hex_bytes >> 1); ++i)
        printf ("%02x", bin_buffer[i]);
    }

  اگر (!tagged)
    {
      putchar (' ');
      putchar (دودویی_file ? '*' : ' ');
      print_filename (file, needs_escape);
    }

  putchar (delim);
}
#endif

#اگر HASH_ALGO_CKSUM
/* Return true if B64_DIGEST is the same as the base64 digest of the
   DIGEST_LENGTH/8 bytes at BIN_BUFFER.  */
ایستا bool
b64_equal (unsigned نویسه ثابت *b64_digest, unsigned نویسه ثابت *bin_buffer)
{
  idx_t b64_n_bytes = BASE64_LENGTH (digest_length >> 3);
  نویسه b64[BASE64_LENGTH (DIGEST_BIN_BYTES) + 1];
  base64_encode ((نویسه ثابت *) bin_buffer, digest_length >> 3,
                 b64, اندازه b64);
  بازگشت memeq (b64_digest, b64, b64_n_bytes + 1);
}
#endif

/* Return true if HEX_DIGEST is the same as the hex-encoded digest of the
   DIGEST_LENGTH/8 bytes at BIN_BUFFER.  */
ایستا bool
hex_equal (unsigned نویسه ثابت *hex_digest, unsigned نویسه ثابت *bin_buffer)
{
  ایستا ثابت نویسه bin2hex[] = { '0', '1', '2', '3',
                                  '4', '5', '6', '7',
                                  '8', '9', 'a', 'b',
                                  'c', 'd', 'e', 'f' };
  idx_t digest_bin_bytes = digest_hex_bytes >> 1;

  /* Compare generated binary number with text representation
     in check file.  Ignore case of hex digits.  */
  idx_t cnt;
  برای (cnt = 0; cnt < digest_bin_bytes; ++cnt)
    {
      اگر (c_tolower (hex_digest[2 * cnt])
          != bin2hex[bin_buffer[cnt] >> 4]
          || (c_tolower (hex_digest[2 * cnt + 1])
              != (bin2hex[bin_buffer[cnt] & 0xf])))
        بشکن;
    }
  بازگشت cnt == digest_bin_bytes;
}

ایستا bool
digest_check (نویسه ثابت *checkfile_name)
{
  FILE *checkfile_stream;
  intmax_t n_misformatted_سطرها = 0;
  intmax_t n_mismatched_checksums = 0;
  intmax_t n_open_or_read_failures = 0;
  bool properly_formatted_سطرها = false;
  bool matched_checksums = false;
  unsigned نویسه bin_buffer_unaligned[DIGEST_BIN_BYTES + DIGEST_ALIGN];
  /* Make sure bin_buffer is properly aligned. */
  unsigned نویسه *bin_buffer = ptr_align (bin_buffer_unaligned, DIGEST_ALIGN);
  intmax_t سطر_عدد;
  نویسه *سطر;
  size_t سطر_chars_allocated;
  bool is_stdin = streq (checkfile_name, "-");

  اگر (is_stdin)
    {
      have_read_stdin = true;
      checkfile_name = _("standard input");
      checkfile_stream = stdin;
    }
  وگرنه
    {
      checkfile_stream = fopen (checkfile_name, "r");
      اگر (checkfile_stream == پوچ)
        {
          error (0, errno, "%s", quotef (checkfile_name));
          بازگشت false;
        }
    }

  سطر_عدد = 0;
  سطر = پوچ;
  سطر_chars_allocated = 0;
  انجامبده
    {
      نویسه *filename;
      صحیح دودویی;
      unsigned نویسه *digest;
      ssize_t سطر_length;

      ++سطر_عدد;
      اگر (سطر_عدد == 0)
        error (EXIT_FAILURE, 0, _("%s: too many checksum lines"),
               quotef (checkfile_name));

      سطر_length = getline (&سطر, &سطر_chars_allocated, checkfile_stream);
      اگر (سطر_length <= 0)
        بشکن;

      /* Ignore comment lines, which begin with a '#' character.  */
      اگر (سطر[0] == '#')
        ادامه;

      /* Remove any trailing newline.  */
      سطر_length -= سطر[سطر_length - 1] == '\n';
      /* Remove any trailing carriage return.  */
      سطر_length -= سطر[سطر_length - (0 < سطر_length)] == '\r';

      /* Ignore empty lines.  */
      اگر (سطر_length == 0)
        ادامه;

      سطر[سطر_length] = '\0';

      idx_t d_len;
      اگر (! (split_3 (سطر, سطر_length, &digest, &d_len, &دودویی, &filename)
             && ! (is_stdin && streq (filename, "-"))))
        {
          ++n_misformatted_سطرها;

          اگر (warn)
            {
              error (0, 0,
                     _("%s: %jd"
                       ": improperly formatted %s checksum line"),
                     quotef (checkfile_name), سطر_عدد,
                     DIGEST_TYPE_STRING);
            }
        }
      وگرنه
        {
          bool ok;
          bool missing;

          properly_formatted_سطرها = true;

          intmax_t length;
          ok = digest_file (filename, &دودویی, bin_buffer, &missing, &length);

          اگر (!ok)
            {
              ++n_open_or_read_failures;
              اگر (!status_only)
                printf ("%s: %s\n", quotef (filename),_("FAILED open or read"));
            }
          وگرنه اگر (ignore_missing && missing)
            {
              /* Ignore missing files with --ignore-missing.  */
              ;
            }
          وگرنه
            {
              bool match = false;
#اگر HASH_ALGO_CKSUM
              اگر (d_len == BASE64_LENGTH (digest_length >> 3))
                match = b64_equal (digest, bin_buffer);
              وگرنه
#endif
                اگر (d_len == digest_hex_bytes)
                  match = hex_equal (digest, bin_buffer);

              اگر (match)
                matched_checksums = true;
              وگرنه
                ++n_mismatched_checksums;

              اگر (!status_only)
                {
                  اگر (! match || ! quiet)
                    fputs (quotef (filename), stdout);

                  اگر (! match)
                    printf (": %s\n", _("FAILED"));
                  وگرنه اگر (!quiet)
                    printf (": %s\n", _("OK"));
                }
            }

          اگر (ferror (stdout))
            write_error ();
        }
    }
  وقتی (!feof (checkfile_stream) && !ferror (checkfile_stream));

  آزاد (سطر);

  صحیح err = ferror (checkfile_stream) ? 0 : -1;
  اگر (is_stdin)
    clearerr (checkfile_stream);
  وگرنه اگر (fclose (checkfile_stream) != 0 && err < 0)
    err = errno;

  اگر (0 <= err)
    {
      error (0, err, err ? "%s" : _("%s: read error"),
             quotef (checkfile_name));
      بازگشت false;
    }

  اگر (! properly_formatted_سطرها)
    {
      /* Warn if no tests are found.  */
      error (0, 0, _("%s: no properly formatted checksum lines found"),
             quotef (checkfile_name));
    }
  وگرنه
    {
      اگر (!status_only)
        {
          اگر (n_misformatted_سطرها != 0)
            error (0, 0,
                   (ngettext
                    ("WARNING: %jd line is improperly formatted",
                     "WARNING: %jd lines are improperly formatted",
                     select_plural (n_misformatted_سطرها))),
                   n_misformatted_سطرها);

          اگر (n_open_or_read_failures != 0)
            error (0, 0,
                   (ngettext
                    ("WARNING: %jd listed file could not be read",
                     "WARNING: %jd listed files could not be read",
                     select_plural (n_open_or_read_failures))),
                   n_open_or_read_failures);

          اگر (n_mismatched_checksums != 0)
            error (0, 0,
                   (ngettext
                    ("WARNING: %jd computed checksum did NOT match",
                     "WARNING: %jd computed checksums did NOT match",
                     select_plural (n_mismatched_checksums))),
                   n_mismatched_checksums);

          اگر (ignore_missing && ! matched_checksums)
            error (0, 0, _("%s: no file was verified"),
                   quotef (checkfile_name));
        }
    }

  بازگشت (properly_formatted_سطرها
          && matched_checksums
          && n_mismatched_checksums == 0
          && n_open_or_read_failures == 0
          && (!strict || n_misformatted_سطرها == 0));
}

صحیح
اصلی (صحیح argc, نویسه **argv)
{
  unsigned نویسه bin_buffer_unaligned[DIGEST_BIN_BYTES + DIGEST_ALIGN];
  /* Make sure bin_buffer is properly aligned. */
  unsigned نویسه *bin_buffer = ptr_align (bin_buffer_unaligned, DIGEST_ALIGN);
  bool do_check = false;
  صحیح opt;
  bool ok = true;
  صحیح دودویی = -1;
  صحیح prefix_tag = -1;
  ساختار option ثابت *long_opts = long_options;

#اگر HASH_ALGO_CKSUM
  اگر (cksum_algorithm != crc)
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
  setvbuf (stdout, پوچ, _IOLBF, 0);

#اگر HASH_ALGO_SUM
  نویسه ثابت *short_opts = "rs";
#وگرنهاگر HASH_ALGO_CKSUM
  نویسه ثابت *short_opts = legacy_mode ? "bctwz" : "a:l:bctwz";
  نویسه ثابت *digest_length_str = "";
#وگرنهاگر HASH_ALGO_BLAKE2
  نویسه ثابت *short_opts = "l:bctwz";
  نویسه ثابت *digest_length_str = "";
#وگرنه
  نویسه ثابت *short_opts = "bctwz";
#endif

  وقتی ((opt = getopt_long (argc, argv, short_opts, long_opts, پوچ))
         != -1)
    انتخاب (opt)
      {
#اگر HASH_ALGO_CKSUM
      حالت 'a':
        cksum_algorithm = XARGMATCH_EXACT ("--algorithm", optarg,
                                           algorithm_args, algorithm_types);
        algorithm_specified = true;
        بشکن;

      حالت DEBUG_PROGRAM_OPTION:
        cksum_debug = true;
        بشکن;
#endif
#اگر HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
      حالت 'l':
        digest_length = xnumtoimax (optarg, 10, 0, IDX_MAX, "",
                                    _("invalid length"), 0,
                                    XTOINT_MAX_QUIET);
        digest_length_str = optarg;
        بشکن;
#endif
#اگر !HASH_ALGO_SUM
      حالت 'c':
        do_check = true;
        بشکن;
      حالت STATUS_OPTION:
        status_only = true;
        warn = false;
        quiet = false;
        بشکن;
      حالت 'b':
        دودویی = 1;
        بشکن;
      حالت 't':
        دودویی = 0;
        بشکن;
      حالت 'w':
        status_only = false;
        warn = true;
        quiet = false;
        بشکن;
      حالت IGNORE_MISSING_OPTION:
        ignore_missing = true;
        بشکن;
      حالت QUIET_OPTION:
        status_only = false;
        warn = false;
        quiet = true;
        بشکن;
      حالت STRICT_OPTION:
        strict = true;
        بشکن;
# اگر HASH_ALGO_CKSUM
      حالت BASE64_OPTION:
        base64_digest = true;
        بشکن;
      حالت RAW_OPTION:
        raw_digest = true;
        بشکن;
      حالت UNTAG_OPTION:
        prefix_tag = 0;
        بشکن;
# endif
      حالت TAG_OPTION:
        prefix_tag = 1;
        بشکن;
      حالت 'z':
        digest_delim = '\0';
        بشکن;
#endif
#اگر HASH_ALGO_SUM
      حالت 'r':		/* For SysV compatibility. */
        sum_algorithm = bsd;
        بشکن;

      حالت 's':
        sum_algorithm = sysv;
        بشکن;
#endif
      case_GETOPT_HELP_CHAR;
      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
      پیشفرض:
        usage (EXIT_FAILURE);
      }

  min_digest_سطر_length = MIN_DIGEST_سطر_LENGTH;
#اگر HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# اگر HASH_ALGO_CKSUM
  اگر (digest_length && (cksum_algorithm != blake2b
                        && cksum_algorithm != sha2
                        && cksum_algorithm != sha3))
    error (EXIT_FAILURE, 0,
           _("--length is only supported with --algorithm "
             "blake2b, sha2, or sha3"));
  اگر (cksum_algorithm == sha2 || cksum_algorithm == sha3)
    {
      /* Do not require --length with --check.  */
      اگر (digest_length == 0 && *digest_length_str == '\0' && ! do_check)
        error (EXIT_FAILURE, 0, _("--algorithm=%s requires specifying "
                                  "--length 224, 256, 384, or 512"),
               algorithm_args[cksum_algorithm]);
      /* If --check and --length are used we verify the digest length.  */
      اگر ((! do_check || *digest_length_str != '\0')
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
  وگرنه
    {
      /* If the digest length checks for SHA-3 are satisfied, the less strict
         checks for BLAKE2 will also be.  */
# وگرنه
    {
# endif
      اگر (digest_length > DIGEST_MAX_LEN * 8)
        {
          error (0, 0, _("invalid length: %s"), quote (digest_length_str));
          error (EXIT_FAILURE, 0,
                 _("maximum digest length for %s is %d bits"),
                 quote (DIGEST_TYPE_STRING),
                 DIGEST_MAX_LEN * 8);
        }
      اگر (digest_length % 8 != 0)
        {
          error (0, 0, _("invalid length: %s"), quote (digest_length_str));
          error (EXIT_FAILURE, 0, _("length is not a multiple of 8"));
        }
    }
  اگر (digest_length == 0)
    {
# اگر HASH_ALGO_BLAKE2
      digest_length = DIGEST_MAX_LEN * 8;
# وگرنه
      digest_length = algorithm_bits[cksum_algorithm];
# endif
    }
  digest_hex_bytes = digest_length >> 2;
#وگرنه
  digest_hex_bytes = DIGEST_HEX_BYTES;
#endif

#اگر HASH_ALGO_CKSUM
  انتخاب (+cksum_algorithm)
    {
    حالت bsd:
    حالت sysv:
    حالت crc:
    حالت crc32b:
        اگر (do_check && algorithm_specified)
          error (EXIT_FAILURE, 0,
                 _("--check is not supported with "
                   "--algorithm={bsd,sysv,crc,crc32b}"));
        بشکن;
    }

  اگر (base64_digest && raw_digest)
   {
     error (0, 0, _("--base64 and --raw are mutually exclusive"));
     usage (EXIT_FAILURE);
   }
#endif

  اگر (digest_delim != '\n' && do_check)
    {
      error (0, 0, _("the --zero option is not supported when "
                     "verifying checksums"));
      usage (EXIT_FAILURE);
    }
  اگر (1 <= prefix_tag && do_check)
    {
      /* Note we allow --untagged with --check to more
         seamlessly support --untagged in an emulation wrapper.  */
      error (0, 0, _("the --tag option is meaningless when "
                     "verifying checksums"));
      usage (EXIT_FAILURE);
    }

  اگر (0 <= دودویی && do_check)
    {
      error (0, 0, _("the --binary and --text options are meaningless when "
                     "verifying checksums"));
      usage (EXIT_FAILURE);
    }

  اگر (ignore_missing && !do_check)
    {
      error (0, 0,
             _("the --ignore-missing option is meaningful only when "
               "verifying checksums"));
      usage (EXIT_FAILURE);
    }

  اگر (status_only && !do_check)
    {
      error (0, 0,
       _("the --status option is meaningful only when verifying checksums"));
      usage (EXIT_FAILURE);
    }

  اگر (warn && !do_check)
    {
      error (0, 0,
       _("the --warn option is meaningful only when verifying checksums"));
      usage (EXIT_FAILURE);
    }

  اگر (quiet && !do_check)
    {
      error (0, 0,
       _("the --quiet option is meaningful only when verifying checksums"));
      usage (EXIT_FAILURE);
    }

  اگر (strict & !do_check)
   {
     error (0, 0,
        _("the --strict option is meaningful only when verifying checksums"));
     usage (EXIT_FAILURE);
   }

  اگر (prefix_tag == -1)
    prefix_tag = HASH_ALGO_CKSUM;

  اگر (prefix_tag && !دودویی)
   {
     /* This could be supported in a backwards compatible way
        by prefixing the output line with a space in text mode.
        However that's invasive enough that it was agreed to
        not support this mode with --tag, as --text use cases
        are adequately supported by the default output format.  */
#اگر !HASH_ALGO_CKSUM
     error (0, 0, _("--tag does not support --text mode"));
#وگرنه
     error (0, 0, _("--text mode is only supported with --untagged"));
#endif
     usage (EXIT_FAILURE);
   }

  اگر (!O_دودویی && دودویی < 0)
    دودویی = 0;
  وگرنه اگر (prefix_tag)
    دودویی = 1;

  نویسه **operand_lim = argv + argc;
  اگر (optind == argc)
    *operand_lim++ = bad_cast ("-");
  وگرنه اگر (1 < argc - optind && raw_digest)
    error (EXIT_FAILURE, 0,
           _("the --raw option is not supported with multiple files"));

  برای (نویسه **operandp = argv + optind; operandp < operand_lim; operandp++)
    {
      نویسه *file = *operandp;
      اگر (do_check)
        ok &= digest_check (file);
      وگرنه
        {
          صحیح دودویی_file = دودویی;
          bool missing;
          intmax_t length;

          اگر (! digest_file (file, &دودویی_file, bin_buffer, &missing, &length))
            ok = false;
          وگرنه
            {
              DIGEST_OUT (file, دودویی_file, bin_buffer, raw_digest, prefix_tag,
                          digest_delim, optind != argc, length);
              اگر (ferror (stdout))
                write_error ();
            }
        }
    }

  اگر (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, _("standard input"));

  بازگشت ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
