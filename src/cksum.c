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

#اگر_تعریف_نشده HASH_ALGO_CKSUM
# define HASH_ALGO_CKSUM 0
#پایان_اگر

#إذا HASH_ALGO_جمع || HASH_ALGO_CKSUM
# include "sum.h"
#پایان_اگر
#إذا HASH_ALGO_CKSUM
# include "cksum.h"
# include "cksum_crc.h"
# include "base64.h"
#پایان_اگر
#إذا HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# include "blake2/b2sum.h"
#پایان_اگر
#إذا HASH_ALGO_MD5 || HASH_ALGO_CKSUM
# include "md5.h"
#پایان_اگر
#إذا HASH_ALGO_SHA1 || HASH_ALGO_CKSUM
# include "sha1.h"
#پایان_اگر
#إذا HASH_ALGO_SHA256 || HASH_ALGO_SHA224 || HASH_ALGO_CKSUM
# include "sha256.h"
#پایان_اگر
#إذا HASH_ALGO_SHA512 || HASH_ALGO_SHA384 || HASH_ALGO_CKSUM
# include "sha512.h"
#پایان_اگر
#إذا HASH_ALGO_CKSUM
# include "sha3.h"
#پایان_اگر
#إذا HASH_ALGO_CKSUM
# include "sm3.h"
#پایان_اگر
#ضمن "fadvise.h"
#ضمن "stdio--.h"
#ضمن "xbinary-io.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#إذا HASH_ALGO_جمع
# define برنامه_NAME "sum"
# define DIGEST_TYPE_STRING "BSD"
# define DIGEST_جریان sumfns[جمع_algorithm]
# define DIGEST_بیرون جمع_output_fns[جمع_algorithm]
# define DIGEST_بیتها 16
# define DIGEST_ALIGN 4
#وإلاإذا HASH_ALGO_CKSUM
# define MAX_DIGEST_بیتها 512
# define MAX_DIGEST_ALIGN 8
# define DIGEST_TYPE_STRING algorithm_برچسبها[cksum_algorithm]
# define DIGEST_جریان cksumfns[cksum_algorithm]
# define DIGEST_بیرون cksum_output_fns[cksum_algorithm]
# define DIGEST_بیتها MAX_DIGEST_بیتها
# define DIGEST_ALIGN MAX_DIGEST_ALIGN
#وإلاإذا HASH_ALGO_MD5
# define برنامه_NAME "md5sum"
# define DIGEST_TYPE_STRING "MD5"
# define DIGEST_جریان md5_جریان
# define DIGEST_بیتها 128
# define DIGEST_REFERENCE "RFC 1321"
# define DIGEST_ALIGN 4
#وإلاإذا HASH_ALGO_BLAKE2
# define برنامه_NAME "b2sum"
# define DIGEST_TYPE_STRING "BLAKE2b"
# define DIGEST_جریان blake2b_جریان
# define DIGEST_بیتها 512
# define DIGEST_REFERENCE "RFC 7693"
# define DIGEST_ALIGN 8
#وإلاإذا HASH_ALGO_SHA1
# define برنامه_NAME "sha1sum"
# define DIGEST_TYPE_STRING "SHA1"
# define DIGEST_جریان sha1_جریان
# define DIGEST_بیتها 160
# define DIGEST_REFERENCE "FIPS-180-1"
# define DIGEST_ALIGN 4
#وإلاإذا HASH_ALGO_SHA256
# define برنامه_NAME "sha256sum"
# define DIGEST_TYPE_STRING "SHA256"
# define DIGEST_جریان sha256_جریان
# define DIGEST_بیتها 256
# define DIGEST_REFERENCE "FIPS-180-2"
# define DIGEST_ALIGN 4
#وإلاإذا HASH_ALGO_SHA224
# define برنامه_NAME "sha224sum"
# define DIGEST_TYPE_STRING "SHA224"
# define DIGEST_جریان sha224_جریان
# define DIGEST_بیتها 224
# define DIGEST_REFERENCE "RFC 3874"
# define DIGEST_ALIGN 4
#وإلاإذا HASH_ALGO_SHA512
# define برنامه_NAME "sha512sum"
# define DIGEST_TYPE_STRING "SHA512"
# define DIGEST_جریان sha512_جریان
# define DIGEST_بیتها 512
# define DIGEST_REFERENCE "FIPS-180-2"
# define DIGEST_ALIGN 8
#وإلاإذا HASH_ALGO_SHA384
# define برنامه_NAME "sha384sum"
# define DIGEST_TYPE_STRING "SHA384"
# define DIGEST_جریان sha384_جریان
# define DIGEST_بیتها 384
# define DIGEST_REFERENCE "FIPS-180-2"
# define DIGEST_ALIGN 8
#وإلا
# error "Can't decide which hash algorithm to compile."
#پایان_اگر
#إذا !HASH_ALGO_جمع && !HASH_ALGO_CKSUM
# define DIGEST_بیرون output_file
#پایان_اگر

#إذا HASH_ALGO_CKSUM
# define برنامه_NAME (!legacy_نما ? "cksum"			 \
                       : cksum_algorithm == md5 ? "md5sum"	 \
                       : cksum_algorithm == sha1 ? "sha1sum"	 \
                       : cksum_algorithm == sha224 ? "sha224sum" \
                       : cksum_algorithm == sha256 ? "sha256sum" \
                       : cksum_algorithm == sha384 ? "sha384sum" \
                       : cksum_algorithm == sha512 ? "sha512sum" \
                       : "cksum")
#پایان_اگر

#إذا HASH_ALGO_جمع
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
#پایان_اگر
#إذا !HASH_ALGO_BLAKE2 && !HASH_ALGO_CKSUM
# define DIGEST_HEX_بایتها (DIGEST_بیتها / 4)
#پایان_اگر
#تعریف_کن DIGEST_BIN_بایتها (DIGEST_بیتها / 8)

/* The minimum length of a valid digest line.  This length does
   not include any newline character at the end of a line.  */
#إذا HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# define MIN_DIGEST_سطر_درازی 3 /* With -l 8.  */
#وإلا
# define MIN_DIGEST_سطر_درازی \
   (DIGEST_HEX_بایتها /* length of hexadecimal message digest */ \
    + 1 /* blank */ \
    + 1 /* minimum filename length */ )
#پایان_اگر

#إذا !HASH_ALGO_جمع
ساكن فراغ
output_file (حرف ثابت *file, صحيح دودویی_file, فراغ ثابت *digest,
             bool raw, bool tagged, بیعلامت حرف delim, bool آرگومانها,
             intmax_t درازی);
#پایان_اگر

/* True if any of the files read were the standard input. */
ساكن bool have_read_stdin;

/* The minimum length of a valid checksum line for the selected algorithm.  */
ساكن idx_t min_digest_سطر_درازی;

/* Set to the length of a digest hex string for the selected algorithm.  */
ساكن idx_t digest_hex_بایتها;

/* With --check, don't generate any output.
   The exit code indicates success or failure.  */
ساكن bool وضعیت_only = false;

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
ساكن بیعلامت حرف digest_delim = '\n';

#إذا HASH_ALGO_CKSUM
/* If true, print base64-encoded digests, not hex.  */
ساكن bool base64_digest = false;
#پایان_اگر

/* If true, print binary digests, not hex.  */
ساكن bool raw_digest = false;

/* blake2b and sha3 allow the -l option.  Luckily they both have the same
   maximum digest size.  */
#إذا HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# إذا HASH_ALGO_CKSUM
static_assert (BLAKE2B_OUTBYTES == SHA3_512_DIGEST_اندازه);
# endif
# define DIGEST_MAX_LEN BLAKE2B_OUTBYTES
ساكن idx_t digest_درازی;
#پایان_اگر /* HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM */

تعريفنوع فراغ (*digest_output_fn)(حرف ثابت *, صحيح, فراغ ثابت *, bool,
                                 bool, بیعلامت حرف, bool, intmax_t);
#إذا HASH_ALGO_جمع
تعداد Algorithm
{
  bsd,
  sysv,
};

ساكن تعداد Algorithm جمع_algorithm;
ساكن sumfn sumfns[]=
{
  bsd_جمع_جریان,
  sysv_جمع_جریان,
};
ساكن digest_output_fn جمع_output_fns[]=
{
  output_bsd,
  output_sysv,
};
#پایان_اگر

#إذا HASH_ALGO_CKSUM
ساكن صحيح
md5_جمع_جریان (FILE *جریان, فراغ *resstream, MAYBE_استفادهنشده intmax_t *درازی)
{
  أرجع md5_جریان (جریان, resstream);
}
ساكن صحيح
sha1_جمع_جریان (FILE *جریان, فراغ *resstream, MAYBE_استفادهنشده intmax_t *درازی)
{
  أرجع sha1_جریان (جریان, resstream);
}
ساكن صحيح
sha224_جمع_جریان (FILE *جریان, فراغ *resstream,
                   MAYBE_استفادهنشده intmax_t *درازی)
{
  أرجع sha224_جریان (جریان, resstream);
}
ساكن صحيح
sha256_جمع_جریان (FILE *جریان, فراغ *resstream,
                   MAYBE_استفادهنشده intmax_t *درازی)
{
  أرجع sha256_جریان (جریان, resstream);
}
ساكن صحيح
sha384_جمع_جریان (FILE *جریان, فراغ *resstream,
                   MAYBE_استفادهنشده intmax_t *درازی)
{
  أرجع sha384_جریان (جریان, resstream);
}
ساكن صحيح
sha512_جمع_جریان (FILE *جریان, فراغ *resstream,
                   MAYBE_استفادهنشده intmax_t *درازی)
{
  أرجع sha512_جریان (جریان, resstream);
}
ساكن صحيح
sha2_جمع_جریان (FILE *جریان, فراغ *resstream, intmax_t *درازی)
{
  بدل (*درازی)
    {
    حالة SHA224_DIGEST_اندازه:
      أرجع sha224_جریان (جریان, resstream);
    حالة SHA256_DIGEST_اندازه:
      أرجع sha256_جریان (جریان, resstream);
    حالة SHA384_DIGEST_اندازه:
      أرجع sha384_جریان (جریان, resstream);
    حالة SHA512_DIGEST_اندازه:
      أرجع sha512_جریان (جریان, resstream);
    افتراضي:
      affirm (false);
    }
}
ساكن صحيح
sha3_جمع_جریان (FILE *جریان, فراغ *resstream, intmax_t *درازی)
{
  بدل (*درازی)
    {
    حالة SHA3_224_DIGEST_اندازه:
      أرجع sha3_224_جریان (جریان, resstream);
    حالة SHA3_256_DIGEST_اندازه:
      أرجع sha3_256_جریان (جریان, resstream);
    حالة SHA3_384_DIGEST_اندازه:
      أرجع sha3_384_جریان (جریان, resstream);
    حالة SHA3_512_DIGEST_اندازه:
      أرجع sha3_512_جریان (جریان, resstream);
    افتراضي:
      affirm (false);
    }
}
ساكن صحيح
blake2b_جمع_جریان (FILE *جریان, فراغ *resstream, intmax_t *درازی)
{
  أرجع blake2b_جریان (جریان, resstream, *درازی);
}
ساكن صحيح
sm3_جمع_جریان (FILE *جریان, فراغ *resstream, MAYBE_استفادهنشده intmax_t *درازی)
{
  أرجع sm3_جریان (جریان, resstream);
}

ساكن حرف ثابت *ثابت algorithm_آرگومانها[] =
{
  "bsd", "sysv", "crc", "crc32b", "md5", "sha1",
  "sha224", "sha256", "sha384", "sha512", /* Legacy naming */
  "sha2", "sha3", "blake2b", "sm3", عدم
};
ساكن تعداد Algorithm ثابت algorithm_types[] =
{
  bsd, sysv, بررسی_چرخهای, crc32b, md5, sha1,
  sha224, sha256, sha384, sha512,
  sha2, sha3, blake2b, sm3,
};
ARGMATCH_VERIFY (algorithm_آرگومانها, algorithm_types);

ساكن حرف ثابت *ثابت algorithm_برچسبها[] =
{
  "BSD", "SYSV", "CRC", "CRC32B", "MD5", "SHA1",
  "SHA224", "SHA256", "SHA384", "SHA512",
  "SHA2", "SHA3", "BLAKE2b", "SM3", عدم
};
ساكن صحيح ثابت algorithm_بیتها[] =
{
  16, 16, 32, 32, 128, 160,
  224, 256, 384, 512,
  512, 512, 512, 256, 0
};

static_assert (countof (algorithm_بیتها) == countof (algorithm_آرگومانها));

ساكن bool algorithm_specified = false;
ساكن bool legacy_نما = false;
تعداد Algorithm cksum_algorithm = بررسی_چرخهای;

ساكن sumfn cksumfns[]=
{
  bsd_جمع_جریان,
  sysv_جمع_جریان,
  بررسی_چرخهای_جمع_جریان,
  crc32b_جمع_جریان,
  md5_جمع_جریان,
  sha1_جمع_جریان,
  sha224_جمع_جریان,
  sha256_جمع_جریان,
  sha384_جمع_جریان,
  sha512_جمع_جریان,
  sha2_جمع_جریان,
  sha3_جمع_جریان,
  blake2b_جمع_جریان,
  sm3_جمع_جریان,
};
ساكن digest_output_fn cksum_output_fns[]=
{
  output_bsd,
  output_sysv,
  output_بررسی_چرخهای,
  output_بررسی_چرخهای,
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
#پایان_اگر

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */

تعداد
{
  IGNORE_MISSING_OPTION = CHAR_MAX + 1,
  وضعیت_OPTION,
  QUIET_OPTION,
  STRICT_OPTION,
  برچسب_OPTION,
  UNTAG_OPTION,
  DEBUG_برنامه_OPTION,
  RAW_OPTION,
  BASE64_OPTION,
};

#إذا HASH_ALGO_CKSUM
ساكن بنية option ثابت legacy_long_options[] =
{
  { "check", no_آرگومان, عدم, 'c' },
  { "ignore-missing", no_آرگومان, عدم, IGNORE_MISSING_OPTION },
  { "quiet", no_آرگومان, عدم, QUIET_OPTION },
  { "status", no_آرگومان, عدم, وضعیت_OPTION },
  { "warn", no_آرگومان, عدم, 'w' },
  { "strict", no_آرگومان, عدم, STRICT_OPTION },
  { "tag", no_آرگومان, عدم, برچسب_OPTION },
  { "zero", no_آرگومان, عدم, 'z' },

  { "binary", no_آرگومان, عدم, 'b' }, /* Deprecated.  */
  { "text", no_آرگومان, عدم, 't' }, /* Deprecated.  */

  { GETOPT_HELP_OPTION_DECL },
  { GETOPT_نسخه_OPTION_DECL },
  { عدم, 0, عدم, 0 }
};
#پایان_اگر

ساكن بنية option ثابت long_options[] =
{
#إذا HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
  { "length", required_آرگومان, عدم, 'l'},
#پایان_اگر

#إذا !HASH_ALGO_جمع
  { "check", no_آرگومان, عدم, 'c' },
  { "ignore-missing", no_آرگومان, عدم, IGNORE_MISSING_OPTION},
  { "quiet", no_آرگومان, عدم, QUIET_OPTION },
  { "status", no_آرگومان, عدم, وضعیت_OPTION },
  { "warn", no_آرگومان, عدم, 'w' },
  { "strict", no_آرگومان, عدم, STRICT_OPTION },
  { "tag", no_آرگومان, عدم, برچسب_OPTION },
  { "zero", no_آرگومان, عدم, 'z' },

# إذا HASH_ALGO_CKSUM
  { "algorithm", required_آرگومان, عدم, 'a'},
  { "base64", no_آرگومان, عدم, BASE64_OPTION },
  { "debug", no_آرگومان, عدم, DEBUG_برنامه_OPTION},
  { "raw", no_آرگومان, عدم, RAW_OPTION},
  { "untagged", no_آرگومان, عدم, UNTAG_OPTION },
# endif
  { "binary", no_آرگومان, عدم, 'b' }, /* Deprecated.  */
  { "text", no_آرگومان, عدم, 't' }, /* Deprecated.  */

#وإلا
  {"sysv", no_آرگومان, عدم, 's'},  /* Not in cksum.  */
#پایان_اگر

  { GETOPT_HELP_OPTION_DECL },
  { GETOPT_نسخه_OPTION_DECL },
  { عدم, 0, عدم, 0 }
};

فراغ
usage (صحيح وضعیت)
{
  إذا (وضعیت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کن (_("\
Usage: %s [OPTION]... [FILE]...\n\
"), برنامه_name);
#إذا HASH_ALGO_CKSUM
    إذا (legacy_نما)
      {
        چاپ_کن (_("\
Print or check %s checksums.\n\
"), DIGEST_TYPE_STRING);
        fputs (_("\
Legacy interface مقصد the cksum(1) utility.\n\
"), stdout);
      }
    وإلا
      fputs (_("\
Print or verify checksums.\n\
By افتراضي use the 32 بیت بررسی_چرخهای_2 algorithm.\n\
"), stdout);
#وإلا
      چاپ_کن (_("\
Print or check %s (%d-بیت) checksums.\n\
"),
              DIGEST_TYPE_STRING,
              DIGEST_بیتها);
      fputs (_("\
Legacy interface مقصد the cksum(1) utility.\n\
"), stdout);
#پایان_اگر

      emit_stdin_note ();
#إذا HASH_ALGO_جمع
      oputs (_("\
  -r\n\
         use BSD جمع algorithm (the افتراضي), use 1K بلوکها\n\
"));
      oputs (_("\
  -s, --sysv\n\
         use System V جمع algorithm, use 512 بایتها بلوکها\n\
"));
#پایان_اگر
#إذا HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
        emit_mandatory_arg_note ();
#پایان_اگر
#إذا HASH_ALGO_CKSUM
     إذا (! legacy_نما)
        oputs (_("\
  -a, --algorithm=TYPE\n\
         select the digest type مقصد use.  See DIGEST زیر\n\
"));
     إذا (! legacy_نما)
        oputs (_("\
      --base64\n\
         emit base64-encoded digests, not hexadecimal\n\
"));
#پایان_اگر
#إذا !HASH_ALGO_جمع
# إذا HASH_ALGO_CKSUM
     إذا (legacy_نما)
       {
# endif
      إذا (O_دودویی)
        oputs (_("\
  -b, --دودویی\n\
         اقرأ درون دودویی نما_2 (افتراضي unless reading tty stdin)\n\
"));
      وإلا
        oputs (_("\
  -b, --دودویی\n\
         اقرأ درون دودویی نما_2\n\
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
     إذا (! legacy_نما)
#  endif
        oputs (_("\
  -l, --درازی=بیتها\n\
         digest درازی درون بیتها_2; must not exceed the max اندازه\n\
         and must be a مضرب از 8 لكل blake2b;\n\
         must be 224, 256, 384, or 512 لكل sha2 or sha3\n\
"));
# endif
# إذا HASH_ALGO_CKSUM
    إذا (! legacy_نما)
        oputs (_("\
      --raw\n\
         emit a raw دودویی digest, not hexadecimal\n\
"));
    إذا (legacy_نما)
      oputs (_("\
      --برچسب\n\
         create a BSD-style checksum\n\
"));
    وإلا
      oputs (_("\
      --برچسب\n\
         create a BSD-style checksum (the افتراضي)\n\
"));
    إذا (! legacy_نما)
      oputs (_("\
      --untagged\n\
         create a reversed style checksum, without digest type\n\
"));
# وإلا
      oputs (_("\
      --برچسب\n\
         create a BSD-style checksum\n\
"));
# endif
# إذا HASH_ALGO_CKSUM
    إذا (legacy_نما)
      {
# endif
      إذا (O_دودویی)
        oputs (_("\
  -t, --متن\n\
         اقرأ درون متن نما_2 (افتراضي إذا reading tty stdin)\n\
"));
      وإلا
        oputs (_("\
  -t, --متن\n\
         اقرأ درون متن نما_2 (افتراضي)\n\
"));
# إذا HASH_ALGO_CKSUM
      }
# endif
      oputs (_("\
  -z, --zero\n\
         انتها each output سطر with NUL, not newline,\n\
         and disable file name escaping\n\
"));
      fputs (_("\
\n\
The following five options are useful only when verifying checksums:\n\
"), stdout);
      oputs (_("\
      --ignore-missing\n\
         don't fail or report وضعیت لكل missing files\n\
"));
      oputs (_("\
      --quiet\n\
         don't print OK لكل each successfully verified file\n\
"));
      oputs (_("\
      --وضعیت\n\
         don't output anything, وضعیت کد shows success\n\
"));
      oputs (_("\
      --strict\n\
         خروج non-zero لكل improperly formatted checksum سطرها\n\
"));
      oputs (_("\
  -w, --warn\n\
         warn about improperly formatted checksum سطرها\n\
"));
#پایان_اگر
#إذا HASH_ALGO_CKSUM
    إذا (! legacy_نما)
      oputs (_("\
      --debug\n\
         indicate which implementation استفاده_شده\n\
"));
#پایان_اگر
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);
#إذا HASH_ALGO_CKSUM
    إذا (! legacy_نما)
      fputs (_("\
\n\
DIGEST determines the digest algorithm and افتراضي output format:\n\
  sysv      (equivalent مقصد جمع -s)\n\
  bsd       (equivalent مقصد جمع -r)\n\
  بررسی_چرخهای       (equivalent مقصد cksum)\n\
  crc32b    (only موجود through cksum)\n\
  md5       (equivalent مقصد md5sum)\n\
  sha1      (equivalent مقصد sha1sum)\n\
  sha2      (equivalent مقصد sha{224,256,384,512}جمع)\n\
  sha3      (only موجود through cksum)\n\
  blake2b   (equivalent مقصد b2sum)\n\
  sm3       (only موجود through cksum)\n\
\n"), stdout);
#پایان_اگر
#إذا !HASH_ALGO_جمع && !HASH_ALGO_CKSUM
      چاپ_کن (_("\
\n\
The sums are computed as described درون %s.\n"), DIGEST_REFERENCE);
      fputs (_("\
When checking, the درون_داد should be a former output از this برنامه.\n\
The افتراضي نما_2 is مقصد print a سطر with: checksum, a space,\n\
a character indicating درون_داد نما_2 ('*' لكل دودویی, ' ' لكل متن\n\
or where دودویی is insignificant), and name لكل each FILE.\n\
\n\
There is no difference between دودویی نما_2 and متن نما_2 on GNU systems.\
\n"), stdout);
#پایان_اگر
#إذا HASH_ALGO_CKSUM
      fputs (_("\
When checking, the درون_داد should be a former output از this برنامه,\n\
or equivalent standalone برنامه.\
\n"), stdout);
#پایان_اگر
      emit_ancillary_info (برنامه_NAME);
    }

  خروج (وضعیت);
}

#إذا !HASH_ALGO_جمع
/* Given a string S, return TRUE if it contains problematic characters
   that need escaping.  Note we escape '\' itself to provide some forward
   compat to introduce escaping of other characters.  */

ATTRIBUTE_خالص
ساكن bool
problematic_chars (حرف ثابت *s)
{
  idx_t درازی = strcspn (s, "\\\n\r");
  أرجع s[درازی] != '\0';
}
#پایان_اگر

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
ATTRIBUTE_خالص
ساكن bool
valid_digits (بیعلامت حرف ثابت *s, idx_t len)
{
#إذا HASH_ALGO_CKSUM
  إذا (len == BASE64_درازی (digest_درازی >> 3))
    {
      idx_t i;
      لكل (i = 0; i < len - digest_درازی % 3; i++)
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
#پایان_اگر
  إذا (len == digest_hex_بایتها)
    {
      لكل (idx_t i = 0; i < digest_hex_بایتها; i++)
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
bsd_تقسیم_3 (حرف *s, idx_t s_len,
             بیعلامت حرف **digest, idx_t *d_len,
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

  *digest = (بیعلامت حرف *) &s[i];

  *d_len = s_len - i;
  أرجع valid_digits (*digest, *d_len);
}

#إذا HASH_ALGO_CKSUM
/* Return the corresponding Algorithm for the string S,
   or -1 for no match.  */

ساكن ptrdiff_t
algorithm_from_برچسب (حرف *s)
{
  /* Limit check size to this length for perf reasons.  */
  ساكن idx_t max_برچسب_len;
  إذا (! max_برچسب_len)
    {
      حرف ثابت * ثابت * برچسب = algorithm_برچسبها;
      طالما (*برچسب)
        {
          idx_t برچسب_len = طول_رشته (*برچسب++);
          max_برچسب_len = MAX (برچسب_len, max_برچسب_len);
        }
    }

  idx_t i = 0;

  /* Find end of tag */
  طالما (i <= max_برچسب_len && s[i] && ! c_isblank (s[i])
         && s[i] != '-' && s[i] != '(')
    ++i;

  إذا (i > max_برچسب_len)
    أرجع -1;

  /* Terminate tag, and lookup.  */
  حرف sep = s[i];
  s[i] = '\0';
  ptrdiff_t algo = argmatch_exact (s, algorithm_برچسبها);
  s[i] = sep;

  أرجع algo;
}
#پایان_اگر

/* Split the string S (of length S_LEN) into three parts:
   a hexadecimal digest, binary flag, and the file name.
   S is modified.  Set *D_LEN to the length of the digest string.
   Return true if successful.  */

ساكن bool
تقسیم_3 (حرف *s, idx_t s_len,
         بیعلامت حرف **digest, idx_t *d_len, صحيح *دودویی, حرف **file_name)
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
      ptrdiff_t algo_برچسب = algorithm_from_برچسب (s + i);
      إذا (! algorithm_specified)
        {
          إذا (algo_برچسب >= 0)
            {
              إذا (algo_برچسب <= crc32b)
                أرجع false;  /* We don't support checking these formats.  */
              cksum_algorithm = algo_برچسب;
            }
          وإلا
            أرجع false;      /* We only support tagged format without -a.  */
        }
      وإلا
        {
          إذا (cksum_algorithm == sha2 && (algo_برچسب == sha2
              || algo_برچسب == sha224 || algo_برچسب == sha256
              || algo_برچسب == sha384 || algo_برچسب == sha512))
            cksum_algorithm = algo_برچسب;
        }
    }
#پایان_اگر

  /* Try to parse BSD or OpenSSL tagged format.  I.e.:
     openssl: MD5(f)= d41d8cd98f00b204e9800998ecf8427e
     bsd:     MD5 (f) = d41d8cd98f00b204e9800998ecf8427e  */

  idx_t parse_جابه_جایی = i;
  algo_name_len = طول_رشته (DIGEST_TYPE_STRING);
  إذا (STREQ_LEN (s + i, DIGEST_TYPE_STRING, algo_name_len))
    {
      i += algo_name_len;
#إذا HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM

# إذا HASH_ALGO_BLAKE2
      digest_درازی = DIGEST_MAX_LEN * 8;
# وإلا
      digest_درازی = algorithm_بیتها[cksum_algorithm];
# endif
      إذا (s[i] == '-')  /* length specified. Not base64 */
        {
          ++i;
          intmax_t درازی;
          حرف *siend;
          إذا (!c_isdigit (s[i])
              || xstrtoimax (s + i, &siend, 0, &درازی, عدم) != LONGINT_OK)
            أرجع false;
# إذا HASH_ALGO_CKSUM
          وإلا إذا (cksum_algorithm == sha2 || cksum_algorithm == sha3)
            {
              إذا (درازی != SHA224_DIGEST_اندازه * 8
                  && درازی != SHA256_DIGEST_اندازه * 8
                  && درازی != SHA384_DIGEST_اندازه * 8
                  && درازی != SHA512_DIGEST_اندازه * 8)
                أرجع false;
            }
# endif
          وإلا إذا (!(0 < درازی && درازی <= digest_درازی && درازی % 8 == 0))
            أرجع false;

          i = siend - s;
          digest_درازی = درازی;
        }
      digest_hex_بایتها = digest_درازی >> 2;
#پایان_اگر
      إذا (s[i] == ' ')
        ++i;
      إذا (s[i] == '(')  /* not base64 */
        {
          ++i;
          *دودویی = 0;
          أرجع bsd_تقسیم_3 (s + i, s_len - i,
                              digest, d_len, file_name, escaped_filename);
        }

      /* Note with --base64 --untagged format, we may have matched a "tag".
         Even very short digests with: cksum -a blake2b -l24 --untagged --base64
         So fallback to checking untagged format if issues detecting tags.  */
      i = parse_جابه_جایی;
    }

  /* Ignore this line if it is too short.
     Each line must have at least 'min_digest_line_length - 1' (or one more, if
     the first is a backslash) more characters to contain correct message digest
     information.  */
  إذا (s_len - i < min_digest_سطر_درازی + (s[i] == '\\'))
    أرجع false;

  *digest = (بیعلامت حرف *) &s[i];

#إذا HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
  /* Auto determine length.  */
# إذا HASH_ALGO_CKSUM
  إذا (cksum_algorithm == blake2b
      || cksum_algorithm == sha2 || cksum_algorithm == sha3) {
# endif
  بیعلامت حرف ثابت *hp = *digest;
  digest_hex_بایتها = 0;
  لكل (; c_isxdigit (*hp); ++hp, ++digest_hex_بایتها)
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
  idx_t digest_base64_بایتها = digest_hex_بایتها;
  idx_t trailing_equals = 0;
  لكل (; isubase64 (*hp); ++hp, ++digest_base64_بایتها)
    ;
  لكل (; *hp == '='; ++hp, ++trailing_equals)
    ;
  إذا ((cksum_algorithm == sha2 || cksum_algorithm == sha3)
      && digest_hex_بایتها >> 1 != SHA224_DIGEST_اندازه
      && digest_hex_بایتها >> 1 != SHA256_DIGEST_اندازه
      && digest_hex_بایتها >> 1 != SHA384_DIGEST_اندازه
      && digest_hex_بایتها >> 1 != SHA512_DIGEST_اندازه)
    {
      إذا (digest_base64_بایتها + trailing_equals
          == BASE64_درازی (SHA224_DIGEST_اندازه))
        digest_hex_بایتها = SHA224_DIGEST_اندازه * 2;
      وإلا إذا (digest_base64_بایتها + trailing_equals
               == BASE64_درازی (SHA256_DIGEST_اندازه))
        digest_hex_بایتها = SHA256_DIGEST_اندازه * 2;
      وإلا إذا (digest_base64_بایتها + trailing_equals
               == BASE64_درازی (SHA384_DIGEST_اندازه))
        digest_hex_بایتها = SHA384_DIGEST_اندازه * 2;
      وإلا إذا (digest_base64_بایتها + trailing_equals
               == BASE64_درازی (SHA512_DIGEST_اندازه))
        digest_hex_بایتها = SHA512_DIGEST_اندازه * 2;
      وإلا
        أرجع false;
    }
  وإلا إذا (cksum_algorithm == blake2b
           && digest_hex_بایتها < digest_base64_بایتها)
    {
      لكل (صحيح j = 8; j <= DIGEST_MAX_LEN * 8; j += 8)
        {
          إذا (BASE64_درازی (j / 8) == digest_base64_بایتها + trailing_equals
              && j % 3 == trailing_equals)
            {
              digest_hex_بایتها = j / 4;
              اكسر;
            }
        }
    }
# endif
  إذا (digest_hex_بایتها < 2 || digest_hex_بایتها % 2
      || DIGEST_MAX_LEN * 2 < digest_hex_بایتها)
    أرجع false;
  digest_درازی = digest_hex_بایتها * 4;
# إذا HASH_ALGO_CKSUM
  }
# endif
#پایان_اگر

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
      *دودویی = (s[i++] == '*');
    }

  /* All characters between the type indicator and end of line are
     significant -- that includes leading and trailing white space.  */
  *file_name = &s[i];

  إذا (escaped_filename)
    أرجع filename_unescape (&s[i], s_len - i) != عدم;

  أرجع true;
}

#إذا !HASH_ALGO_جمع
/* If ESCAPE is true, then translate each:
   NEWLINE byte to the string, "\\n",
   CARRIAGE RETURN byte to the string, "\\r",
   and each backslash to "\\\\".  */
ساكن فراغ
print_filename (حرف ثابت *file, bool escape)
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
          حرف_چاپ_کن (*file);
          اكسر;
        }
      file++;
    }
}
#پایان_اگر

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
digest_file (حرف ثابت *filename, صحيح *دودویی, بیعلامت حرف *bin_نتیجه,
             bool *missing, MAYBE_استفادهنشده intmax_t *درازی)
{
  FILE *fp;
  صحيح err;
  bool is_stdin = streq (filename, "-");

  *missing = false;

  إذا (is_stdin)
    {
      have_read_stdin = true;
      fp = stdin;
      إذا (O_دودویی && *دودویی)
        {
          إذا (*دودویی < 0)
            *دودویی = ! isatty (STDIN_FILENO);
          إذا (*دودویی)
            xset_دودویی_نما (STDIN_FILENO, O_دودویی);
        }
    }
  وإلا
    {
      fp = fopen (filename, O_دودویی ? (*دودویی ? "rb" : "rt") : "r");
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
    *درازی = digest_درازی >> 3;
  err = DIGEST_جریان (fp, bin_نتیجه, درازی);
#وإلاإذا HASH_ALGO_جمع
  err = DIGEST_جریان (fp, bin_نتیجه, درازی);
#وإلاإذا HASH_ALGO_BLAKE2
  err = DIGEST_جریان (fp, bin_نتیجه, digest_درازی >> 3);
#وإلا
  err = DIGEST_جریان (fp, bin_نتیجه);
#پایان_اگر
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

#إذا !HASH_ALGO_جمع
ساكن فراغ
output_file (حرف ثابت *file, صحيح دودویی_file, فراغ ثابت *digest,
             MAYBE_استفادهنشده bool raw, bool tagged, بیعلامت حرف delim,
             MAYBE_استفادهنشده bool آرگومانها, MAYBE_استفادهنشده intmax_t درازی)
{
# إذا HASH_ALGO_CKSUM
  إذا (raw)
    {
      fwrite (digest, 1, digest_درازی >> 3, stdout);
      أرجع;
    }
# endif

  بیعلامت حرف ثابت *bin_میانگیر = digest;

  /* Output a leading backslash if the file name contains problematic chars.  */
  bool needs_escape = delim == '\n' && problematic_chars (file);

  إذا (needs_escape)
    حرف_چاپ_کن ('\\');

  إذا (tagged)
    {
# إذا HASH_ALGO_CKSUM
      إذا (cksum_algorithm == sha2)
        چاپ_کن ("SHA%u", (بیعلامت صحيح) digest_درازی);
      وإلا
# endif
      fputs (DIGEST_TYPE_STRING, stdout);
# إذا HASH_ALGO_BLAKE2
      إذا (digest_درازی < DIGEST_MAX_LEN * 8)
        چاپ_کن ("-%u", (بیعلامت صحيح) digest_درازی);
# elif HASH_ALGO_CKSUM
      إذا (cksum_algorithm == sha3)
        چاپ_کن ("-%u", (بیعلامت صحيح) digest_درازی);
      إذا (cksum_algorithm == blake2b)
        {
          إذا (digest_درازی < DIGEST_MAX_LEN * 8)
            چاپ_کن ("-%u", (بیعلامت صحيح) digest_درازی);
        }
# endif
      fputs (" (", stdout);
      print_filename (file, needs_escape);
      fputs (") = ", stdout);
    }

# إذا HASH_ALGO_CKSUM
  إذا (base64_digest)
    {
      حرف b64[BASE64_درازی (DIGEST_BIN_بایتها) + 1];
      base64_encode ((حرف ثابت *) bin_میانگیر, digest_درازی >> 3,
                     b64, حجم b64);
      fputs (b64, stdout);
    }
  وإلا
# endif
    {
      لكل (idx_t i = 0; i < (digest_hex_بایتها >> 1); ++i)
        چاپ_کن ("%02x", bin_میانگیر[i]);
    }

  إذا (!tagged)
    {
      حرف_چاپ_کن (' ');
      حرف_چاپ_کن (دودویی_file ? '*' : ' ');
      print_filename (file, needs_escape);
    }

  حرف_چاپ_کن (delim);
}
#پایان_اگر

#إذا HASH_ALGO_CKSUM
/* Return true if B64_DIGEST is the same as the base64 digest of the
   DIGEST_LENGTH/8 bytes at BIN_BUFFER.  */
ساكن bool
b64_equal (بیعلامت حرف ثابت *b64_digest, بیعلامت حرف ثابت *bin_میانگیر)
{
  idx_t b64_n_بایتها = BASE64_درازی (digest_درازی >> 3);
  حرف b64[BASE64_درازی (DIGEST_BIN_بایتها) + 1];
  base64_encode ((حرف ثابت *) bin_میانگیر, digest_درازی >> 3,
                 b64, حجم b64);
  أرجع memeq (b64_digest, b64, b64_n_بایتها + 1);
}
#پایان_اگر

/* Return true if HEX_DIGEST is the same as the hex-encoded digest of the
   DIGEST_LENGTH/8 bytes at BIN_BUFFER.  */
ساكن bool
hex_equal (بیعلامت حرف ثابت *hex_digest, بیعلامت حرف ثابت *bin_میانگیر)
{
  ساكن ثابت حرف bin2hex[] = { '0', '1', '2', '3',
                                  '4', '5', '6', '7',
                                  '8', '9', 'a', 'b',
                                  'c', 'd', 'e', 'f' };
  idx_t digest_bin_بایتها = digest_hex_بایتها >> 1;

  /* Compare generated binary number with text representation
     in check file.  Ignore case of hex digits.  */
  idx_t cnt;
  لكل (cnt = 0; cnt < digest_bin_بایتها; ++cnt)
    {
      إذا (c_tolower (hex_digest[2 * cnt])
          != bin2hex[bin_میانگیر[cnt] >> 4]
          || (c_tolower (hex_digest[2 * cnt + 1])
              != (bin2hex[bin_میانگیر[cnt] & 0xf])))
        اكسر;
    }
  أرجع cnt == digest_bin_بایتها;
}

ساكن bool
digest_check (حرف ثابت *checkfile_name)
{
  FILE *checkfile_جریان;
  intmax_t n_misformatted_سطرها = 0;
  intmax_t n_mismatched_checksums = 0;
  intmax_t n_open_or_read_failures = 0;
  bool properly_formatted_سطرها = false;
  bool matched_checksums = false;
  بیعلامت حرف bin_میانگیر_unaligned[DIGEST_BIN_بایتها + DIGEST_ALIGN];
  /* Make sure bin_buffer is properly aligned. */
  بیعلامت حرف *bin_میانگیر = ptr_align (bin_میانگیر_unaligned, DIGEST_ALIGN);
  intmax_t سطر_عدد;
  حرف *سطر;
  size_t سطر_chars_allocated;
  bool is_stdin = streq (checkfile_name, "-");

  إذا (is_stdin)
    {
      have_read_stdin = true;
      checkfile_name = _("standard input");
      checkfile_جریان = stdin;
    }
  وإلا
    {
      checkfile_جریان = fopen (checkfile_name, "r");
      إذا (checkfile_جریان == عدم)
        {
          error (0, errno, "%s", quotef (checkfile_name));
          أرجع false;
        }
    }

  سطر_عدد = 0;
  سطر = عدم;
  سطر_chars_allocated = 0;
  افعل
    {
      حرف *filename;
      صحيح دودویی;
      بیعلامت حرف *digest;
      ssize_t سطر_درازی;

      ++سطر_عدد;
      إذا (سطر_عدد == 0)
        error (EXIT_FAILURE, 0, _("%s: too many checksum lines"),
               quotef (checkfile_name));

      سطر_درازی = getline (&سطر, &سطر_chars_allocated, checkfile_جریان);
      إذا (سطر_درازی <= 0)
        اكسر;

      /* Ignore comment lines, which begin with a '#' character.  */
      إذا (سطر[0] == '#')
        استمر;

      /* Remove any trailing newline.  */
      سطر_درازی -= سطر[سطر_درازی - 1] == '\n';
      /* Remove any trailing carriage return.  */
      سطر_درازی -= سطر[سطر_درازی - (0 < سطر_درازی)] == '\r';

      /* Ignore empty lines.  */
      إذا (سطر_درازی == 0)
        استمر;

      سطر[سطر_درازی] = '\0';

      idx_t d_len;
      إذا (! (تقسیم_3 (سطر, سطر_درازی, &digest, &d_len, &دودویی, &filename)
             && ! (is_stdin && streq (filename, "-"))))
        {
          ++n_misformatted_سطرها;

          إذا (warn)
            {
              error (0, 0,
                     _("%s: %jd"
                       ": improperly formatted %s checksum line"),
                     quotef (checkfile_name), سطر_عدد,
                     DIGEST_TYPE_STRING);
            }
        }
      وإلا
        {
          bool ok;
          bool missing;

          properly_formatted_سطرها = true;

          intmax_t درازی;
          ok = digest_file (filename, &دودویی, bin_میانگیر, &missing, &درازی);

          إذا (!ok)
            {
              ++n_open_or_read_failures;
              إذا (!وضعیت_only)
                چاپ_کن ("%s: %s\n", quotef (filename),_("FAILED open or read"));
            }
          وإلا إذا (ignore_missing && missing)
            {
              /* Ignore missing files with --ignore-missing.  */
              ;
            }
          وإلا
            {
              bool تطابق = false;
#إذا HASH_ALGO_CKSUM
              إذا (d_len == BASE64_درازی (digest_درازی >> 3))
                تطابق = b64_equal (digest, bin_میانگیر);
              وإلا
#پایان_اگر
                إذا (d_len == digest_hex_بایتها)
                  تطابق = hex_equal (digest, bin_میانگیر);

              إذا (تطابق)
                matched_checksums = true;
              وإلا
                ++n_mismatched_checksums;

              إذا (!وضعیت_only)
                {
                  إذا (! تطابق || ! quiet)
                    fputs (quotef (filename), stdout);

                  إذا (! تطابق)
                    چاپ_کن (": %s\n", _("FAILED"));
                  وإلا إذا (!quiet)
                    چاپ_کن (": %s\n", _("OK"));
                }
            }

          إذا (ferror (stdout))
            write_error ();
        }
    }
  طالما (!feof (checkfile_جریان) && !ferror (checkfile_جریان));

  حرر (سطر);

  صحيح err = ferror (checkfile_جریان) ? 0 : -1;
  إذا (is_stdin)
    clearerr (checkfile_جریان);
  وإلا إذا (fclose (checkfile_جریان) != 0 && err < 0)
    err = errno;

  إذا (0 <= err)
    {
      error (0, err, err ? "%s" : _("%s: read error"),
             quotef (checkfile_name));
      أرجع false;
    }

  إذا (! properly_formatted_سطرها)
    {
      /* Warn if no tests are found.  */
      error (0, 0, _("%s: no properly formatted checksum lines found"),
             quotef (checkfile_name));
    }
  وإلا
    {
      إذا (!وضعیت_only)
        {
          إذا (n_misformatted_سطرها != 0)
            error (0, 0,
                   (ngettext
                    ("WARNING: %jd line is improperly formatted",
                     "WARNING: %jd lines are improperly formatted",
                     select_plural (n_misformatted_سطرها))),
                   n_misformatted_سطرها);

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

  أرجع (properly_formatted_سطرها
          && matched_checksums
          && n_mismatched_checksums == 0
          && n_open_or_read_failures == 0
          && (!strict || n_misformatted_سطرها == 0));
}

صحيح
اصلی (صحيح argc, حرف **argv)
{
  بیعلامت حرف bin_میانگیر_unaligned[DIGEST_BIN_بایتها + DIGEST_ALIGN];
  /* Make sure bin_buffer is properly aligned. */
  بیعلامت حرف *bin_میانگیر = ptr_align (bin_میانگیر_unaligned, DIGEST_ALIGN);
  bool do_check = false;
  صحيح opt;
  bool ok = true;
  صحيح دودویی = -1;
  صحيح پیشوند_برچسب = -1;
  بنية option ثابت *long_opts = long_options;

#إذا HASH_ALGO_CKSUM
  إذا (cksum_algorithm != بررسی_چرخهای)
    {
      legacy_نما = true;
      پیشوند_برچسب = 0;
      algorithm_specified = true;
      long_opts = legacy_long_options;
    }
#پایان_اگر

  /* Setting values of global variables.  */
  initialize_main (&argc, &argv);
  مجموعه_برنامه_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* Line buffer stdout to ensure lines are written atomically and immediately
     so that processes running in parallel do not intersperse their output.  */
  setvbuf (stdout, عدم, _IOLBF, 0);

#إذا HASH_ALGO_جمع
  حرف ثابت *short_opts = "rs";
#وإلاإذا HASH_ALGO_CKSUM
  حرف ثابت *short_opts = legacy_نما ? "bctwz" : "a:l:bctwz";
  حرف ثابت *digest_درازی_str = "";
#وإلاإذا HASH_ALGO_BLAKE2
  حرف ثابت *short_opts = "l:bctwz";
  حرف ثابت *digest_درازی_str = "";
#وإلا
  حرف ثابت *short_opts = "bctwz";
#پایان_اگر

  طالما ((opt = getopt_long (argc, argv, short_opts, long_opts, عدم))
         != -1)
    بدل (opt)
      {
#إذا HASH_ALGO_CKSUM
      حالة 'a':
        cksum_algorithm = XARGMATCH_EXACT ("--algorithm", optarg,
                                           algorithm_آرگومانها, algorithm_types);
        algorithm_specified = true;
        اكسر;

      حالة DEBUG_برنامه_OPTION:
        cksum_debug = true;
        اكسر;
#پایان_اگر
#إذا HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
      حالة 'l':
        digest_درازی = xnumtoimax (optarg, 10, 0, IDX_MAX, "",
                                    _("invalid length"), 0,
                                    XTOINT_MAX_QUIET);
        digest_درازی_str = optarg;
        اكسر;
#پایان_اگر
#إذا !HASH_ALGO_جمع
      حالة 'c':
        do_check = true;
        اكسر;
      حالة وضعیت_OPTION:
        وضعیت_only = true;
        warn = false;
        quiet = false;
        اكسر;
      حالة 'b':
        دودویی = 1;
        اكسر;
      حالة 't':
        دودویی = 0;
        اكسر;
      حالة 'w':
        وضعیت_only = false;
        warn = true;
        quiet = false;
        اكسر;
      حالة IGNORE_MISSING_OPTION:
        ignore_missing = true;
        اكسر;
      حالة QUIET_OPTION:
        وضعیت_only = false;
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
        پیشوند_برچسب = 0;
        اكسر;
# endif
      حالة برچسب_OPTION:
        پیشوند_برچسب = 1;
        اكسر;
      حالة 'z':
        digest_delim = '\0';
        اكسر;
#پایان_اگر
#إذا HASH_ALGO_جمع
      حالة 'r':		/* For SysV compatibility. */
        جمع_algorithm = bsd;
        اكسر;

      حالة 's':
        جمع_algorithm = sysv;
        اكسر;
#پایان_اگر
      case_GETOPT_HELP_CHAR;
      case_GETOPT_نسخه_CHAR (برنامه_NAME, AUTHORS);
      افتراضي:
        usage (EXIT_FAILURE);
      }

  min_digest_سطر_درازی = MIN_DIGEST_سطر_درازی;
#إذا HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# إذا HASH_ALGO_CKSUM
  إذا (digest_درازی && (cksum_algorithm != blake2b
                        && cksum_algorithm != sha2
                        && cksum_algorithm != sha3))
    error (EXIT_FAILURE, 0,
           _("--length is only supported with --algorithm "
             "blake2b, sha2, or sha3"));
  إذا (cksum_algorithm == sha2 || cksum_algorithm == sha3)
    {
      /* Do not require --length with --check.  */
      إذا (digest_درازی == 0 && *digest_درازی_str == '\0' && ! do_check)
        error (EXIT_FAILURE, 0, _("--algorithm=%s requires specifying "
                                  "--length 224, 256, 384, or 512"),
               algorithm_آرگومانها[cksum_algorithm]);
      /* If --check and --length are used we verify the digest length.  */
      إذا ((! do_check || *digest_درازی_str != '\0')
          && digest_درازی != SHA224_DIGEST_اندازه * 8
          && digest_درازی != SHA256_DIGEST_اندازه * 8
          && digest_درازی != SHA384_DIGEST_اندازه * 8
          && digest_درازی != SHA512_DIGEST_اندازه * 8)
        {
          error (0, 0, _("invalid length: %s"), quote (digest_درازی_str));
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
      إذا (digest_درازی > DIGEST_MAX_LEN * 8)
        {
          error (0, 0, _("invalid length: %s"), quote (digest_درازی_str));
          error (EXIT_FAILURE, 0,
                 _("maximum digest length for %s is %d bits"),
                 quote (DIGEST_TYPE_STRING),
                 DIGEST_MAX_LEN * 8);
        }
      إذا (digest_درازی % 8 != 0)
        {
          error (0, 0, _("invalid length: %s"), quote (digest_درازی_str));
          error (EXIT_FAILURE, 0, _("length is not a multiple of 8"));
        }
    }
  إذا (digest_درازی == 0)
    {
# إذا HASH_ALGO_BLAKE2
      digest_درازی = DIGEST_MAX_LEN * 8;
# وإلا
      digest_درازی = algorithm_بیتها[cksum_algorithm];
# endif
    }
  digest_hex_بایتها = digest_درازی >> 2;
#وإلا
  digest_hex_بایتها = DIGEST_HEX_بایتها;
#پایان_اگر

#إذا HASH_ALGO_CKSUM
  بدل (+cksum_algorithm)
    {
    حالة bsd:
    حالة sysv:
    حالة بررسی_چرخهای:
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
#پایان_اگر

  إذا (digest_delim != '\n' && do_check)
    {
      error (0, 0, _("the --zero option is not supported when "
                     "verifying checksums"));
      usage (EXIT_FAILURE);
    }
  إذا (1 <= پیشوند_برچسب && do_check)
    {
      /* Note we allow --untagged with --check to more
         seamlessly support --untagged in an emulation wrapper.  */
      error (0, 0, _("the --tag option is meaningless when "
                     "verifying checksums"));
      usage (EXIT_FAILURE);
    }

  إذا (0 <= دودویی && do_check)
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

  إذا (وضعیت_only && !do_check)
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

  إذا (پیشوند_برچسب == -1)
    پیشوند_برچسب = HASH_ALGO_CKSUM;

  إذا (پیشوند_برچسب && !دودویی)
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
#پایان_اگر
     usage (EXIT_FAILURE);
   }

  إذا (!O_دودویی && دودویی < 0)
    دودویی = 0;
  وإلا إذا (پیشوند_برچسب)
    دودویی = 1;

  حرف **عملوند_lim = argv + argc;
  إذا (optind == argc)
    *عملوند_lim++ = bad_cast ("-");
  وإلا إذا (1 < argc - optind && raw_digest)
    error (EXIT_FAILURE, 0,
           _("the --raw option is not supported with multiple files"));

  لكل (حرف **operandp = argv + optind; operandp < عملوند_lim; operandp++)
    {
      حرف *file = *operandp;
      إذا (do_check)
        ok &= digest_check (file);
      وإلا
        {
          صحيح دودویی_file = دودویی;
          bool missing;
          intmax_t درازی;

          إذا (! digest_file (file, &دودویی_file, bin_میانگیر, &missing, &درازی))
            ok = false;
          وإلا
            {
              DIGEST_بیرون (file, دودویی_file, bin_میانگیر, raw_digest, پیشوند_برچسب,
                          digest_delim, optind != argc, درازی);
              إذا (ferror (stdout))
                write_error ();
            }
        }
    }

  إذا (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, _("standard input"));

  أرجع ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
