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

#اگر_غیر_تعریف_شدہ HASH_ALGO_CKSUM
# define HASH_ALGO_CKSUM 0
#اگر_ختم

#اگر HASH_ALGO_مجموع || HASH_ALGO_CKSUM
# include "sum.h"
#اگر_ختم
#اگر HASH_ALGO_CKSUM
# include "cksum.h"
# include "cksum_crc.h"
# include "base64.h"
#اگر_ختم
#اگر HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# include "blake2/b2sum.h"
#اگر_ختم
#اگر HASH_ALGO_MD5 || HASH_ALGO_CKSUM
# include "md5.h"
#اگر_ختم
#اگر HASH_ALGO_SHA1 || HASH_ALGO_CKSUM
# include "sha1.h"
#اگر_ختم
#اگر HASH_ALGO_SHA256 || HASH_ALGO_SHA224 || HASH_ALGO_CKSUM
# include "sha256.h"
#اگر_ختم
#اگر HASH_ALGO_SHA512 || HASH_ALGO_SHA384 || HASH_ALGO_CKSUM
# include "sha512.h"
#اگر_ختم
#اگر HASH_ALGO_CKSUM
# include "sha3.h"
#اگر_ختم
#اگر HASH_ALGO_CKSUM
# include "sm3.h"
#اگر_ختم
#شامل "fadvise.h"
#شامل "stdio--.h"
#شامل "xbinary-io.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#اگر HASH_ALGO_مجموع
# define پروگرام_NAME "sum"
# define DIGEST_TYPE_STRING "BSD"
# define DIGEST_دھارا sumfns[مجموع_algorithm]
# define DIGEST_برآمد مجموع_output_fns[مجموع_algorithm]
# define DIGEST_بٹ 16
# define DIGEST_ALIGN 4
#وگرنہ_اگر HASH_ALGO_CKSUM
# define MAX_DIGEST_بٹ 512
# define MAX_DIGEST_ALIGN 8
# define DIGEST_TYPE_STRING algorithm_لیبل_فہرست[cksum_algorithm]
# define DIGEST_دھارا cksumfns[cksum_algorithm]
# define DIGEST_برآمد cksum_output_fns[cksum_algorithm]
# define DIGEST_بٹ MAX_DIGEST_بٹ
# define DIGEST_ALIGN MAX_DIGEST_ALIGN
#وگرنہ_اگر HASH_ALGO_MD5
# define پروگرام_NAME "md5sum"
# define DIGEST_TYPE_STRING "MD5"
# define DIGEST_دھارا md5_دھارا
# define DIGEST_بٹ 128
# define DIGEST_REFERENCE "RFC 1321"
# define DIGEST_ALIGN 4
#وگرنہ_اگر HASH_ALGO_BLAKE2
# define پروگرام_NAME "b2sum"
# define DIGEST_TYPE_STRING "BLAKE2b"
# define DIGEST_دھارا blake2b_دھارا
# define DIGEST_بٹ 512
# define DIGEST_REFERENCE "RFC 7693"
# define DIGEST_ALIGN 8
#وگرنہ_اگر HASH_ALGO_SHA1
# define پروگرام_NAME "sha1sum"
# define DIGEST_TYPE_STRING "SHA1"
# define DIGEST_دھارا sha1_دھارا
# define DIGEST_بٹ 160
# define DIGEST_REFERENCE "FIPS-180-1"
# define DIGEST_ALIGN 4
#وگرنہ_اگر HASH_ALGO_SHA256
# define پروگرام_NAME "sha256sum"
# define DIGEST_TYPE_STRING "SHA256"
# define DIGEST_دھارا sha256_دھارا
# define DIGEST_بٹ 256
# define DIGEST_REFERENCE "FIPS-180-2"
# define DIGEST_ALIGN 4
#وگرنہ_اگر HASH_ALGO_SHA224
# define پروگرام_NAME "sha224sum"
# define DIGEST_TYPE_STRING "SHA224"
# define DIGEST_دھارا sha224_دھارا
# define DIGEST_بٹ 224
# define DIGEST_REFERENCE "RFC 3874"
# define DIGEST_ALIGN 4
#وگرنہ_اگر HASH_ALGO_SHA512
# define پروگرام_NAME "sha512sum"
# define DIGEST_TYPE_STRING "SHA512"
# define DIGEST_دھارا sha512_دھارا
# define DIGEST_بٹ 512
# define DIGEST_REFERENCE "FIPS-180-2"
# define DIGEST_ALIGN 8
#وگرنہ_اگر HASH_ALGO_SHA384
# define پروگرام_NAME "sha384sum"
# define DIGEST_TYPE_STRING "SHA384"
# define DIGEST_دھارا sha384_دھارا
# define DIGEST_بٹ 384
# define DIGEST_REFERENCE "FIPS-180-2"
# define DIGEST_ALIGN 8
#ورنہ
# error "Can't decide which hash algorithm to compile."
#اگر_ختم
#اگر !HASH_ALGO_مجموع && !HASH_ALGO_CKSUM
# define DIGEST_برآمد output_file
#اگر_ختم

#اگر HASH_ALGO_CKSUM
# define پروگرام_NAME (!legacy_منوال ? "cksum"			 \
                       : cksum_algorithm == md5 ? "md5sum"	 \
                       : cksum_algorithm == sha1 ? "sha1sum"	 \
                       : cksum_algorithm == sha224 ? "sha224sum" \
                       : cksum_algorithm == sha256 ? "sha256sum" \
                       : cksum_algorithm == sha384 ? "sha384sum" \
                       : cksum_algorithm == sha512 ? "sha512sum" \
                       : "cksum")
#اگر_ختم

#اگر HASH_ALGO_مجموع
# define AUTHORS \
  proper_name ("Kayvan Aghaiepour"), \
  proper_name ("David MacKenzie")
#وگرنہ_اگر HASH_ALGO_CKSUM
# define AUTHORS \
  proper_name_lite ("Padraig Brady", "P\303\241draig Brady"), \
  proper_name ("Q. Frank Xia")
#وگرنہ_اگر HASH_ALGO_BLAKE2
# define AUTHORS \
  proper_name_lite ("Padraig Brady", "P\303\241draig Brady"), \
  proper_name ("Samuel Neves")
#ورنہ
# define AUTHORS \
  proper_name ("Ulrich Drepper"), \
  proper_name ("Scott Miller"), \
  proper_name ("David Madore")
#اگر_ختم
#اگر !HASH_ALGO_BLAKE2 && !HASH_ALGO_CKSUM
# define DIGEST_HEX_بائٹیں (DIGEST_بٹ / 4)
#اگر_ختم
#تعریف_کرو DIGEST_BIN_بائٹیں (DIGEST_بٹ / 8)

/* The minimum length of a valid digest line.  This length does
   not include any newline character at the end of a line.  */
#اگر HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# define MIN_DIGEST_سطر_لمبائی 3 /* With -l 8.  */
#ورنہ
# define MIN_DIGEST_سطر_لمبائی \
   (DIGEST_HEX_بائٹیں /* length of hexadecimal message digest */ \
    + 1 /* blank */ \
    + 1 /* minimum filename length */ )
#اگر_ختم

#اگر !HASH_ALGO_مجموع
ساکن خالی
output_file (حرف ثابت *file, صحیح ثنائی_file, خالی ثابت *digest,
             bool raw, bool tagged, غیرعلامتی حرف delim, bool دلائل,
             intmax_t لمبائی);
#اگر_ختم

/* True if any of the files read were the standard input. */
ساکن bool have_read_stdin;

/* The minimum length of a valid checksum line for the selected algorithm.  */
ساکن idx_t min_digest_سطر_لمبائی;

/* Set to the length of a digest hex string for the selected algorithm.  */
ساکن idx_t digest_hex_بائٹیں;

/* With --check, don't generate any output.
   The exit code indicates success or failure.  */
ساکن bool حالت_only = false;

/* With --check, print a message to standard error warning about each
   improperly formatted checksum line.  */
ساکن bool warn = false;

/* With --check, ignore missing files.  */
ساکن bool ignore_missing = false;

/* With --check, suppress the "OK" printed for each verified file.  */
ساکن bool quiet = false;

/* With --check, exit with a non-zero return code if any line is
   improperly formatted. */
ساکن bool strict = false;

/* Whether a BSD reversed format checksum is detected.  */
ساکن صحیح bsd_reversed = -1;

/* line delimiter.  */
ساکن غیرعلامتی حرف digest_delim = '\n';

#اگر HASH_ALGO_CKSUM
/* If true, print base64-encoded digests, not hex.  */
ساکن bool base64_digest = false;
#اگر_ختم

/* If true, print binary digests, not hex.  */
ساکن bool raw_digest = false;

/* blake2b and sha3 allow the -l option.  Luckily they both have the same
   maximum digest size.  */
#اگر HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# اگر HASH_ALGO_CKSUM
static_assert (BLAKE2B_OUTBYTES == SHA3_512_DIGEST_سائز);
# endif
# define DIGEST_MAX_LEN BLAKE2B_OUTBYTES
ساکن idx_t digest_لمبائی;
#اگر_ختم /* HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM */

قسمتعریف خالی (*digest_output_fn)(حرف ثابت *, صحیح, خالی ثابت *, bool,
                                 bool, غیرعلامتی حرف, bool, intmax_t);
#اگر HASH_ALGO_مجموع
شمار Algorithm
{
  bsd,
  sysv,
};

ساکن شمار Algorithm مجموع_algorithm;
ساکن sumfn sumfns[]=
{
  bsd_مجموع_دھارا,
  sysv_مجموع_دھارا,
};
ساکن digest_output_fn مجموع_output_fns[]=
{
  output_bsd,
  output_sysv,
};
#اگر_ختم

#اگر HASH_ALGO_CKSUM
ساکن صحیح
md5_مجموع_دھارا (FILE *دھارا, خالی *resstream, MAYBE_غیر_مستعمل intmax_t *لمبائی)
{
  واپس md5_دھارا (دھارا, resstream);
}
ساکن صحیح
sha1_مجموع_دھارا (FILE *دھارا, خالی *resstream, MAYBE_غیر_مستعمل intmax_t *لمبائی)
{
  واپس sha1_دھارا (دھارا, resstream);
}
ساکن صحیح
sha224_مجموع_دھارا (FILE *دھارا, خالی *resstream,
                   MAYBE_غیر_مستعمل intmax_t *لمبائی)
{
  واپس sha224_دھارا (دھارا, resstream);
}
ساکن صحیح
sha256_مجموع_دھارا (FILE *دھارا, خالی *resstream,
                   MAYBE_غیر_مستعمل intmax_t *لمبائی)
{
  واپس sha256_دھارا (دھارا, resstream);
}
ساکن صحیح
sha384_مجموع_دھارا (FILE *دھارا, خالی *resstream,
                   MAYBE_غیر_مستعمل intmax_t *لمبائی)
{
  واپس sha384_دھارا (دھارا, resstream);
}
ساکن صحیح
sha512_مجموع_دھارا (FILE *دھارا, خالی *resstream,
                   MAYBE_غیر_مستعمل intmax_t *لمبائی)
{
  واپس sha512_دھارا (دھارا, resstream);
}
ساکن صحیح
sha2_مجموع_دھارا (FILE *دھارا, خالی *resstream, intmax_t *لمبائی)
{
  انتخاب (*لمبائی)
    {
    حالت SHA224_DIGEST_سائز:
      واپس sha224_دھارا (دھارا, resstream);
    حالت SHA256_DIGEST_سائز:
      واپس sha256_دھارا (دھارا, resstream);
    حالت SHA384_DIGEST_سائز:
      واپس sha384_دھارا (دھارا, resstream);
    حالت SHA512_DIGEST_سائز:
      واپس sha512_دھارا (دھارا, resstream);
    طےشدہ:
      affirm (false);
    }
}
ساکن صحیح
sha3_مجموع_دھارا (FILE *دھارا, خالی *resstream, intmax_t *لمبائی)
{
  انتخاب (*لمبائی)
    {
    حالت SHA3_224_DIGEST_سائز:
      واپس sha3_224_دھارا (دھارا, resstream);
    حالت SHA3_256_DIGEST_سائز:
      واپس sha3_256_دھارا (دھارا, resstream);
    حالت SHA3_384_DIGEST_سائز:
      واپس sha3_384_دھارا (دھارا, resstream);
    حالت SHA3_512_DIGEST_سائز:
      واپس sha3_512_دھارا (دھارا, resstream);
    طےشدہ:
      affirm (false);
    }
}
ساکن صحیح
blake2b_مجموع_دھارا (FILE *دھارا, خالی *resstream, intmax_t *لمبائی)
{
  واپس blake2b_دھارا (دھارا, resstream, *لمبائی);
}
ساکن صحیح
sm3_مجموع_دھارا (FILE *دھارا, خالی *resstream, MAYBE_غیر_مستعمل intmax_t *لمبائی)
{
  واپس sm3_دھارا (دھارا, resstream);
}

ساکن حرف ثابت *ثابت algorithm_دلائل[] =
{
  "bsd", "sysv", "crc", "crc32b", "md5", "sha1",
  "sha224", "sha256", "sha384", "sha512", /* Legacy naming */
  "sha2", "sha3", "blake2b", "sm3", تہی
};
ساکن شمار Algorithm ثابت algorithm_types[] =
{
  bsd, sysv, چکری_جانچ, crc32b, md5, sha1,
  sha224, sha256, sha384, sha512,
  sha2, sha3, blake2b, sm3,
};
ARGMATCH_VERIFY (algorithm_دلائل, algorithm_types);

ساکن حرف ثابت *ثابت algorithm_لیبل_فہرست[] =
{
  "BSD", "SYSV", "CRC", "CRC32B", "MD5", "SHA1",
  "SHA224", "SHA256", "SHA384", "SHA512",
  "SHA2", "SHA3", "BLAKE2b", "SM3", تہی
};
ساکن صحیح ثابت algorithm_بٹ[] =
{
  16, 16, 32, 32, 128, 160,
  224, 256, 384, 512,
  512, 512, 512, 256, 0
};

static_assert (countof (algorithm_بٹ) == countof (algorithm_دلائل));

ساکن bool algorithm_specified = false;
ساکن bool legacy_منوال = false;
شمار Algorithm cksum_algorithm = چکری_جانچ;

ساکن sumfn cksumfns[]=
{
  bsd_مجموع_دھارا,
  sysv_مجموع_دھارا,
  چکری_جانچ_مجموع_دھارا,
  crc32b_مجموع_دھارا,
  md5_مجموع_دھارا,
  sha1_مجموع_دھارا,
  sha224_مجموع_دھارا,
  sha256_مجموع_دھارا,
  sha384_مجموع_دھارا,
  sha512_مجموع_دھارا,
  sha2_مجموع_دھارا,
  sha3_مجموع_دھارا,
  blake2b_مجموع_دھارا,
  sm3_مجموع_دھارا,
};
ساکن digest_output_fn cksum_output_fns[]=
{
  output_bsd,
  output_sysv,
  output_چکری_جانچ,
  output_چکری_جانچ,
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
#اگر_ختم

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */

شمار
{
  IGNORE_MISSING_OPTION = CHAR_MAX + 1,
  حالت_OPTION,
  QUIET_OPTION,
  STRICT_OPTION,
  لیبل_OPTION,
  UNTAG_OPTION,
  DEBUG_پروگرام_OPTION,
  RAW_OPTION,
  BASE64_OPTION,
};

#اگر HASH_ALGO_CKSUM
ساکن ساخت option ثابت legacy_long_options[] =
{
  { "check", no_دلیل, تہی, 'c' },
  { "ignore-missing", no_دلیل, تہی, IGNORE_MISSING_OPTION },
  { "quiet", no_دلیل, تہی, QUIET_OPTION },
  { "status", no_دلیل, تہی, حالت_OPTION },
  { "warn", no_دلیل, تہی, 'w' },
  { "strict", no_دلیل, تہی, STRICT_OPTION },
  { "tag", no_دلیل, تہی, لیبل_OPTION },
  { "zero", no_دلیل, تہی, 'z' },

  { "binary", no_دلیل, تہی, 'b' }, /* Deprecated.  */
  { "text", no_دلیل, تہی, 't' }, /* Deprecated.  */

  { GETOPT_HELP_OPTION_DECL },
  { GETOPT_نسخہ_OPTION_DECL },
  { تہی, 0, تہی, 0 }
};
#اگر_ختم

ساکن ساخت option ثابت long_options[] =
{
#اگر HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
  { "length", required_دلیل, تہی, 'l'},
#اگر_ختم

#اگر !HASH_ALGO_مجموع
  { "check", no_دلیل, تہی, 'c' },
  { "ignore-missing", no_دلیل, تہی, IGNORE_MISSING_OPTION},
  { "quiet", no_دلیل, تہی, QUIET_OPTION },
  { "status", no_دلیل, تہی, حالت_OPTION },
  { "warn", no_دلیل, تہی, 'w' },
  { "strict", no_دلیل, تہی, STRICT_OPTION },
  { "tag", no_دلیل, تہی, لیبل_OPTION },
  { "zero", no_دلیل, تہی, 'z' },

# اگر HASH_ALGO_CKSUM
  { "algorithm", required_دلیل, تہی, 'a'},
  { "base64", no_دلیل, تہی, BASE64_OPTION },
  { "debug", no_دلیل, تہی, DEBUG_پروگرام_OPTION},
  { "raw", no_دلیل, تہی, RAW_OPTION},
  { "untagged", no_دلیل, تہی, UNTAG_OPTION },
# endif
  { "binary", no_دلیل, تہی, 'b' }, /* Deprecated.  */
  { "text", no_دلیل, تہی, 't' }, /* Deprecated.  */

#ورنہ
  {"sysv", no_دلیل, تہی, 's'},  /* Not in cksum.  */
#اگر_ختم

  { GETOPT_HELP_OPTION_DECL },
  { GETOPT_نسخہ_OPTION_DECL },
  { تہی, 0, تہی, 0 }
};

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("\
Usage: %s [OPTION]... [FILE]...\n\
"), پروگرام_name);
#اگر HASH_ALGO_CKSUM
    اگر (legacy_منوال)
      {
        لکھو (_("\
Print or check %s checksums.\n\
"), DIGEST_TYPE_STRING);
        fputs (_("\
Legacy interface مقصد the cksum(1) utility.\n\
"), stdout);
      }
    ورنہ
      fputs (_("\
Print or verify checksums.\n\
By طےشدہ use the 32 ثنائی_ہندسہ چکری_جانچ_2 algorithm.\n\
"), stdout);
#ورنہ
      لکھو (_("\
Print or check %s (%d-ثنائی_ہندسہ) checksums.\n\
"),
              DIGEST_TYPE_STRING,
              DIGEST_بٹ);
      fputs (_("\
Legacy interface مقصد the cksum(1) utility.\n\
"), stdout);
#اگر_ختم

      emit_stdin_note ();
#اگر HASH_ALGO_مجموع
      oputs (_("\
  -r\n\
         use BSD مجموع algorithm (the طےشدہ), use 1K بلاکیں\n\
"));
      oputs (_("\
  -s, --sysv\n\
         use System V مجموع algorithm, use 512 بائٹیں بلاکیں\n\
"));
#اگر_ختم
#اگر HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
        emit_mandatory_arg_note ();
#اگر_ختم
#اگر HASH_ALGO_CKSUM
     اگر (! legacy_منوال)
        oputs (_("\
  -a, --algorithm=TYPE\n\
         select the digest type مقصد use.  See DIGEST نیچے\n\
"));
     اگر (! legacy_منوال)
        oputs (_("\
      --base64\n\
         emit base64-encoded digests, not hexadecimal\n\
"));
#اگر_ختم
#اگر !HASH_ALGO_مجموع
# اگر HASH_ALGO_CKSUM
     اگر (legacy_منوال)
       {
# endif
      اگر (O_ثنائی)
        oputs (_("\
  -b, --ثنائی\n\
         اقرأ اندر ثنائی منوال_2 (طےشدہ unless reading tty stdin)\n\
"));
      ورنہ
        oputs (_("\
  -b, --ثنائی\n\
         اقرأ اندر ثنائی منوال_2\n\
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
     اگر (! legacy_منوال)
#  endif
        oputs (_("\
  -l, --لمبائی=بٹ\n\
         digest لمبائی اندر بٹ_2; must not exceed the max سائز\n\
         and must be a مضرب کا 8 برائے blake2b;\n\
         must be 224, 256, 384, or 512 برائے sha2 or sha3\n\
"));
# endif
# اگر HASH_ALGO_CKSUM
    اگر (! legacy_منوال)
        oputs (_("\
      --raw\n\
         emit a raw ثنائی digest, not hexadecimal\n\
"));
    اگر (legacy_منوال)
      oputs (_("\
      --لیبل\n\
         create a BSD-style checksum\n\
"));
    ورنہ
      oputs (_("\
      --لیبل\n\
         create a BSD-style checksum (the طےشدہ)\n\
"));
    اگر (! legacy_منوال)
      oputs (_("\
      --untagged\n\
         create a reversed style checksum, without digest type\n\
"));
# ورنہ
      oputs (_("\
      --لیبل\n\
         create a BSD-style checksum\n\
"));
# endif
# اگر HASH_ALGO_CKSUM
    اگر (legacy_منوال)
      {
# endif
      اگر (O_ثنائی)
        oputs (_("\
  -t, --متن\n\
         اقرأ اندر متن منوال_2 (طےشدہ اگر reading tty stdin)\n\
"));
      ورنہ
        oputs (_("\
  -t, --متن\n\
         اقرأ اندر متن منوال_2 (طےشدہ)\n\
"));
# اگر HASH_ALGO_CKSUM
      }
# endif
      oputs (_("\
  -z, --zero\n\
         انتہا each output سطر with NUL, not newline,\n\
         and disable file name escaping\n\
"));
      fputs (_("\
\n\
The following five options are useful only when verifying checksums:\n\
"), stdout);
      oputs (_("\
      --ignore-missing\n\
         don't fail or report حالت_2 برائے missing files\n\
"));
      oputs (_("\
      --quiet\n\
         don't print OK برائے each successfully verified file\n\
"));
      oputs (_("\
      --حالت_2\n\
         don't output anything, حالت_2 رمز shows success\n\
"));
      oputs (_("\
      --strict\n\
         باہر_نکلو non-zero برائے improperly formatted checksum سطریں\n\
"));
      oputs (_("\
  -w, --warn\n\
         warn about improperly formatted checksum سطریں\n\
"));
#اگر_ختم
#اگر HASH_ALGO_CKSUM
    اگر (! legacy_منوال)
      oputs (_("\
      --debug\n\
         indicate which implementation مستعمل\n\
"));
#اگر_ختم
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
#اگر HASH_ALGO_CKSUM
    اگر (! legacy_منوال)
      fputs (_("\
\n\
DIGEST determines the digest algorithm and طےشدہ output format:\n\
  sysv      (equivalent مقصد مجموع -s)\n\
  bsd       (equivalent مقصد مجموع -r)\n\
  چکری_جانچ       (equivalent مقصد cksum)\n\
  crc32b    (only دستیاب through cksum)\n\
  md5       (equivalent مقصد md5sum)\n\
  sha1      (equivalent مقصد sha1sum)\n\
  sha2      (equivalent مقصد sha{224,256,384,512}مجموع)\n\
  sha3      (only دستیاب through cksum)\n\
  blake2b   (equivalent مقصد b2sum)\n\
  sm3       (only دستیاب through cksum)\n\
\n"), stdout);
#اگر_ختم
#اگر !HASH_ALGO_مجموع && !HASH_ALGO_CKSUM
      لکھو (_("\
\n\
The sums are computed as described اندر %s.\n"), DIGEST_REFERENCE);
      fputs (_("\
When checking, the داخلہ should be a former output کا this پروگرام.\n\
The طےشدہ منوال_2 is مقصد print a سطر with: checksum, a space,\n\
a character indicating داخلہ منوال_2 ('*' برائے ثنائی, ' ' برائے متن\n\
or where ثنائی is insignificant), and name برائے each FILE.\n\
\n\
There is no difference between ثنائی منوال_2 and متن منوال_2 on GNU systems.\
\n"), stdout);
#اگر_ختم
#اگر HASH_ALGO_CKSUM
      fputs (_("\
When checking, the داخلہ should be a former output کا this پروگرام,\n\
or equivalent standalone پروگرام.\
\n"), stdout);
#اگر_ختم
      emit_ancillary_info (پروگرام_NAME);
    }

  باہر_نکلو (حالت_2);
}

#اگر !HASH_ALGO_مجموع
/* Given a string S, return TRUE if it contains problematic characters
   that need escaping.  Note we escape '\' itself to provide some forward
   compat to introduce escaping of other characters.  */

ATTRIBUTE_خالص
ساکن bool
problematic_chars (حرف ثابت *s)
{
  idx_t لمبائی = strcspn (s, "\\\n\r");
  واپس s[لمبائی] != '\0';
}
#اگر_ختم

/* Given a file name, S of length S_LEN, that is not NUL-terminated,
   modify it in place, performing the equivalent of this sed substitution:
   's/\\n/\n/g;s/\\r/\r/g;s/\\\\/\\/g' i.e., replacing each "\\n" string
   with a newline, each "\\r" string with a carriage return,
   and each "\\\\" with a single backslash, NUL-terminate it and return S.
   If S is not a valid escaped file name, i.e., if it ends with an odd number
   of backslashes or if it contains a backslash followed by anything other
   than "n" or another backslash, return NULL.  */

ساکن حرف *
filename_unescape (حرف *s, idx_t s_len)
{
  حرف *dst = s;

  برائے (idx_t i = 0; i < s_len; i++)
    {
      انتخاب (s[i])
        {
        حالت '\\':
          اگر (i == s_len - 1)
            {
              /* File name ends with an unescaped backslash: invalid.  */
              واپس تہی;
            }
          ++i;
          انتخاب (s[i])
            {
            حالت 'n':
              *dst++ = '\n';
              توڑو;
            حالت 'r':
              *dst++ = '\r';
              توڑو;
            حالت '\\':
              *dst++ = '\\';
              توڑو;
            طےشدہ:
              /* Only '\', 'n' or 'r' may follow a backslash.  */
              واپس تہی;
            }
          توڑو;

        حالت '\0':
          /* The file name may not contain a NUL.  */
          واپس تہی;

        طےشدہ:
          *dst++ = s[i];
          توڑو;
        }
    }
  اگر (dst < s + s_len)
    *dst = '\0';

  واپس s;
}

/* Return true if S is a LEN-byte NUL-terminated string of hex or base64
   digits and has the expected length.  Otherwise, return false.  */
ATTRIBUTE_خالص
ساکن bool
valid_digits (غیرعلامتی حرف ثابت *s, idx_t len)
{
#اگر HASH_ALGO_CKSUM
  اگر (len == BASE64_لمبائی (digest_لمبائی >> 3))
    {
      idx_t i;
      برائے (i = 0; i < len - digest_لمبائی % 3; i++)
        {
          اگر (!isbase64 (*s))
            واپس false;
          ++s;
        }
      برائے ( ; i < len; i++)
        {
          اگر (*s != '=')
            واپس false;
          ++s;
        }
    }
  ورنہ
#اگر_ختم
  اگر (len == digest_hex_بائٹیں)
    {
      برائے (idx_t i = 0; i < digest_hex_بائٹیں; i++)
        {
          اگر (!c_isxdigit (*s))
            واپس false;
          ++s;
        }
    }
  ورنہ
    واپس false;

  واپس *s == '\0';
}

/* Split the checksum string S (of length S_LEN) from a BSD 'md5' or
   'sha1' command into two parts: a hexadecimal digest, and the file
   name.  S is modified.  Set *D_LEN to the length of the digest string.
   Return true if successful.  */

ساکن bool
bsd_بانٹو_3 (حرف *s, idx_t s_len,
             غیرعلامتی حرف **digest, idx_t *d_len,
             حرف **file_name, bool escaped_filename)
{
  اگر (s_len == 0)
    واپس false;

  /* Find end of filename.  */
  idx_t i = s_len - 1;
  جب (i && s[i] != ')')
    i--;

  اگر (s[i] != ')')
    واپس false;

  *file_name = s;

  اگر (escaped_filename && filename_unescape (s, i) == تہی)
    واپس false;

  s[i++] = '\0';

  جب (c_isblank (s[i]))
    i++;

  اگر (s[i] != '=')
    واپس false;

  i++;

  جب (c_isblank (s[i]))
    i++;

  *digest = (غیرعلامتی حرف *) &s[i];

  *d_len = s_len - i;
  واپس valid_digits (*digest, *d_len);
}

#اگر HASH_ALGO_CKSUM
/* Return the corresponding Algorithm for the string S,
   or -1 for no match.  */

ساکن ptrdiff_t
algorithm_from_لیبل (حرف *s)
{
  /* Limit check size to this length for perf reasons.  */
  ساکن idx_t max_لیبل_len;
  اگر (! max_لیبل_len)
    {
      حرف ثابت * ثابت * لیبل = algorithm_لیبل_فہرست;
      جب (*لیبل)
        {
          idx_t لیبل_len = لڑی_لمبائی (*لیبل++);
          max_لیبل_len = MAX (لیبل_len, max_لیبل_len);
        }
    }

  idx_t i = 0;

  /* Find end of tag */
  جب (i <= max_لیبل_len && s[i] && ! c_isblank (s[i])
         && s[i] != '-' && s[i] != '(')
    ++i;

  اگر (i > max_لیبل_len)
    واپس -1;

  /* Terminate tag, and lookup.  */
  حرف sep = s[i];
  s[i] = '\0';
  ptrdiff_t algo = argmatch_exact (s, algorithm_لیبل_فہرست);
  s[i] = sep;

  واپس algo;
}
#اگر_ختم

/* Split the string S (of length S_LEN) into three parts:
   a hexadecimal digest, binary flag, and the file name.
   S is modified.  Set *D_LEN to the length of the digest string.
   Return true if successful.  */

ساکن bool
بانٹو_3 (حرف *s, idx_t s_len,
         غیرعلامتی حرف **digest, idx_t *d_len, صحیح *ثنائی, حرف **file_name)
{
  bool escaped_filename = false;
  idx_t algo_name_len;

  idx_t i = 0;
  جب (c_isblank (s[i]))
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
      ptrdiff_t algo_لیبل = algorithm_from_لیبل (s + i);
      اگر (! algorithm_specified)
        {
          اگر (algo_لیبل >= 0)
            {
              اگر (algo_لیبل <= crc32b)
                واپس false;  /* We don't support checking these formats.  */
              cksum_algorithm = algo_لیبل;
            }
          ورنہ
            واپس false;      /* We only support tagged format without -a.  */
        }
      ورنہ
        {
          اگر (cksum_algorithm == sha2 && (algo_لیبل == sha2
              || algo_لیبل == sha224 || algo_لیبل == sha256
              || algo_لیبل == sha384 || algo_لیبل == sha512))
            cksum_algorithm = algo_لیبل;
        }
    }
#اگر_ختم

  /* Try to parse BSD or OpenSSL tagged format.  I.e.:
     openssl: MD5(f)= d41d8cd98f00b204e9800998ecf8427e
     bsd:     MD5 (f) = d41d8cd98f00b204e9800998ecf8427e  */

  idx_t parse_انحراف = i;
  algo_name_len = لڑی_لمبائی (DIGEST_TYPE_STRING);
  اگر (STREQ_LEN (s + i, DIGEST_TYPE_STRING, algo_name_len))
    {
      i += algo_name_len;
#اگر HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM

# اگر HASH_ALGO_BLAKE2
      digest_لمبائی = DIGEST_MAX_LEN * 8;
# ورنہ
      digest_لمبائی = algorithm_بٹ[cksum_algorithm];
# endif
      اگر (s[i] == '-')  /* length specified. Not base64 */
        {
          ++i;
          intmax_t لمبائی;
          حرف *siend;
          اگر (!c_isdigit (s[i])
              || xstrtoimax (s + i, &siend, 0, &لمبائی, تہی) != LONGINT_OK)
            واپس false;
# اگر HASH_ALGO_CKSUM
          ورنہ اگر (cksum_algorithm == sha2 || cksum_algorithm == sha3)
            {
              اگر (لمبائی != SHA224_DIGEST_سائز * 8
                  && لمبائی != SHA256_DIGEST_سائز * 8
                  && لمبائی != SHA384_DIGEST_سائز * 8
                  && لمبائی != SHA512_DIGEST_سائز * 8)
                واپس false;
            }
# endif
          ورنہ اگر (!(0 < لمبائی && لمبائی <= digest_لمبائی && لمبائی % 8 == 0))
            واپس false;

          i = siend - s;
          digest_لمبائی = لمبائی;
        }
      digest_hex_بائٹیں = digest_لمبائی >> 2;
#اگر_ختم
      اگر (s[i] == ' ')
        ++i;
      اگر (s[i] == '(')  /* not base64 */
        {
          ++i;
          *ثنائی = 0;
          واپس bsd_بانٹو_3 (s + i, s_len - i,
                              digest, d_len, file_name, escaped_filename);
        }

      /* Note with --base64 --untagged format, we may have matched a "tag".
         Even very short digests with: cksum -a blake2b -l24 --untagged --base64
         So fallback to checking untagged format if issues detecting tags.  */
      i = parse_انحراف;
    }

  /* Ignore this line if it is too short.
     Each line must have at least 'min_digest_line_length - 1' (or one more, if
     the first is a backslash) more characters to contain correct message digest
     information.  */
  اگر (s_len - i < min_digest_سطر_لمبائی + (s[i] == '\\'))
    واپس false;

  *digest = (غیرعلامتی حرف *) &s[i];

#اگر HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
  /* Auto determine length.  */
# اگر HASH_ALGO_CKSUM
  اگر (cksum_algorithm == blake2b
      || cksum_algorithm == sha2 || cksum_algorithm == sha3) {
# endif
  غیرعلامتی حرف ثابت *hp = *digest;
  digest_hex_بائٹیں = 0;
  برائے (; c_isxdigit (*hp); ++hp, ++digest_hex_بائٹیں)
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
  idx_t digest_base64_بائٹیں = digest_hex_بائٹیں;
  idx_t trailing_equals = 0;
  برائے (; isubase64 (*hp); ++hp, ++digest_base64_بائٹیں)
    ;
  برائے (; *hp == '='; ++hp, ++trailing_equals)
    ;
  اگر ((cksum_algorithm == sha2 || cksum_algorithm == sha3)
      && digest_hex_بائٹیں >> 1 != SHA224_DIGEST_سائز
      && digest_hex_بائٹیں >> 1 != SHA256_DIGEST_سائز
      && digest_hex_بائٹیں >> 1 != SHA384_DIGEST_سائز
      && digest_hex_بائٹیں >> 1 != SHA512_DIGEST_سائز)
    {
      اگر (digest_base64_بائٹیں + trailing_equals
          == BASE64_لمبائی (SHA224_DIGEST_سائز))
        digest_hex_بائٹیں = SHA224_DIGEST_سائز * 2;
      ورنہ اگر (digest_base64_بائٹیں + trailing_equals
               == BASE64_لمبائی (SHA256_DIGEST_سائز))
        digest_hex_بائٹیں = SHA256_DIGEST_سائز * 2;
      ورنہ اگر (digest_base64_بائٹیں + trailing_equals
               == BASE64_لمبائی (SHA384_DIGEST_سائز))
        digest_hex_بائٹیں = SHA384_DIGEST_سائز * 2;
      ورنہ اگر (digest_base64_بائٹیں + trailing_equals
               == BASE64_لمبائی (SHA512_DIGEST_سائز))
        digest_hex_بائٹیں = SHA512_DIGEST_سائز * 2;
      ورنہ
        واپس false;
    }
  ورنہ اگر (cksum_algorithm == blake2b
           && digest_hex_بائٹیں < digest_base64_بائٹیں)
    {
      برائے (صحیح j = 8; j <= DIGEST_MAX_LEN * 8; j += 8)
        {
          اگر (BASE64_لمبائی (j / 8) == digest_base64_بائٹیں + trailing_equals
              && j % 3 == trailing_equals)
            {
              digest_hex_بائٹیں = j / 4;
              توڑو;
            }
        }
    }
# endif
  اگر (digest_hex_بائٹیں < 2 || digest_hex_بائٹیں % 2
      || DIGEST_MAX_LEN * 2 < digest_hex_بائٹیں)
    واپس false;
  digest_لمبائی = digest_hex_بائٹیں * 4;
# اگر HASH_ALGO_CKSUM
  }
# endif
#اگر_ختم

  /* This field must be the hexadecimal or base64 representation
     of the message digest.  */
  جب (s[i] && !c_isblank (s[i]))
    i++;

  /* The digest must be followed by at least one whitespace character.  */
  اگر (i == s_len)
    واپس false;

  *d_len = &s[i] - (حرف *) *digest;
  s[i++] = '\0';

  اگر (! valid_digits (*digest, *d_len))
    واپس false;

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
        واپس false;
      bsd_reversed = 1;
    }
  ورنہ اگر (bsd_reversed != 1)
    {
      bsd_reversed = 0;
      *ثنائی = (s[i++] == '*');
    }

  /* All characters between the type indicator and end of line are
     significant -- that includes leading and trailing white space.  */
  *file_name = &s[i];

  اگر (escaped_filename)
    واپس filename_unescape (&s[i], s_len - i) != تہی;

  واپس true;
}

#اگر !HASH_ALGO_مجموع
/* If ESCAPE is true, then translate each:
   NEWLINE byte to the string, "\\n",
   CARRIAGE RETURN byte to the string, "\\r",
   and each backslash to "\\\\".  */
ساکن خالی
print_filename (حرف ثابت *file, bool escape)
{
  اگر (! escape)
    {
      fputs (file, stdout);
      واپس;
    }

  جب (*file)
    {
      انتخاب (*file)
        {
        حالت '\n':
          fputs ("\\n", stdout);
          توڑو;

        حالت '\r':
          fputs ("\\r", stdout);
          توڑو;

        حالت '\\':
          fputs ("\\\\", stdout);
          توڑو;

        طےشدہ:
          حرف_لکھو (*file);
          توڑو;
        }
      file++;
    }
}
#اگر_ختم

/* An interface to the function, DIGEST_STREAM.
   Operate on FILENAME (it may be "-").

   *BINARY indicates whether the file is binary.  BINARY < 0 means it
   depends on whether binary mode makes any difference and the file is
   a terminal; in that case, clear *BINARY if the file was treated as
   text because it was a terminal.

   Put the checksum in *BIN_RESULT, which must be properly aligned.
   Put true in *MISSING if the file can't be opened due to ENOENT.
   Return true if successful.  */

ساکن bool
digest_file (حرف ثابت *filename, صحیح *ثنائی, غیرعلامتی حرف *bin_نتیجہ,
             bool *missing, MAYBE_غیر_مستعمل intmax_t *لمبائی)
{
  FILE *fp;
  صحیح err;
  bool is_stdin = streq (filename, "-");

  *missing = false;

  اگر (is_stdin)
    {
      have_read_stdin = true;
      fp = stdin;
      اگر (O_ثنائی && *ثنائی)
        {
          اگر (*ثنائی < 0)
            *ثنائی = ! isatty (STDIN_FILENO);
          اگر (*ثنائی)
            xset_ثنائی_منوال (STDIN_FILENO, O_ثنائی);
        }
    }
  ورنہ
    {
      fp = fopen (filename, O_ثنائی ? (*ثنائی ? "rb" : "rt") : "r");
      اگر (fp == تہی)
        {
          اگر (ignore_missing && errno == ENOENT)
            {
              *missing = true;
              واپس true;
            }
          error (0, errno, "%s", quotef (filename));
          واپس false;
        }
    }

  fadvise (fp, FADVISE_SEQUENTIAL);

#اگر HASH_ALGO_CKSUM
  اگر (cksum_algorithm == blake2b
      || cksum_algorithm == sha2 || cksum_algorithm == sha3)
    *لمبائی = digest_لمبائی >> 3;
  err = DIGEST_دھارا (fp, bin_نتیجہ, لمبائی);
#وگرنہ_اگر HASH_ALGO_مجموع
  err = DIGEST_دھارا (fp, bin_نتیجہ, لمبائی);
#وگرنہ_اگر HASH_ALGO_BLAKE2
  err = DIGEST_دھارا (fp, bin_نتیجہ, digest_لمبائی >> 3);
#ورنہ
  err = DIGEST_دھارا (fp, bin_نتیجہ);
#اگر_ختم
  err = err ? errno : 0;
  اگر (is_stdin)
    clearerr (fp);
  ورنہ اگر (fclose (fp) != 0 && !err)
    err = errno;

  اگر (err)
    {
      error (0, err, "%s", quotef (filename));
      واپس false;
    }

  واپس true;
}

#اگر !HASH_ALGO_مجموع
ساکن خالی
output_file (حرف ثابت *file, صحیح ثنائی_file, خالی ثابت *digest,
             MAYBE_غیر_مستعمل bool raw, bool tagged, غیرعلامتی حرف delim,
             MAYBE_غیر_مستعمل bool دلائل, MAYBE_غیر_مستعمل intmax_t لمبائی)
{
# اگر HASH_ALGO_CKSUM
  اگر (raw)
    {
      fwrite (digest, 1, digest_لمبائی >> 3, stdout);
      واپس;
    }
# endif

  غیرعلامتی حرف ثابت *bin_بفر = digest;

  /* Output a leading backslash if the file name contains problematic chars.  */
  bool needs_escape = delim == '\n' && problematic_chars (file);

  اگر (needs_escape)
    حرف_لکھو ('\\');

  اگر (tagged)
    {
# اگر HASH_ALGO_CKSUM
      اگر (cksum_algorithm == sha2)
        لکھو ("SHA%u", (غیرعلامتی صحیح) digest_لمبائی);
      ورنہ
# endif
      fputs (DIGEST_TYPE_STRING, stdout);
# اگر HASH_ALGO_BLAKE2
      اگر (digest_لمبائی < DIGEST_MAX_LEN * 8)
        لکھو ("-%u", (غیرعلامتی صحیح) digest_لمبائی);
# elif HASH_ALGO_CKSUM
      اگر (cksum_algorithm == sha3)
        لکھو ("-%u", (غیرعلامتی صحیح) digest_لمبائی);
      اگر (cksum_algorithm == blake2b)
        {
          اگر (digest_لمبائی < DIGEST_MAX_LEN * 8)
            لکھو ("-%u", (غیرعلامتی صحیح) digest_لمبائی);
        }
# endif
      fputs (" (", stdout);
      print_filename (file, needs_escape);
      fputs (") = ", stdout);
    }

# اگر HASH_ALGO_CKSUM
  اگر (base64_digest)
    {
      حرف b64[BASE64_لمبائی (DIGEST_BIN_بائٹیں) + 1];
      base64_encode ((حرف ثابت *) bin_بفر, digest_لمبائی >> 3,
                     b64, حجم b64);
      fputs (b64, stdout);
    }
  ورنہ
# endif
    {
      برائے (idx_t i = 0; i < (digest_hex_بائٹیں >> 1); ++i)
        لکھو ("%02x", bin_بفر[i]);
    }

  اگر (!tagged)
    {
      حرف_لکھو (' ');
      حرف_لکھو (ثنائی_file ? '*' : ' ');
      print_filename (file, needs_escape);
    }

  حرف_لکھو (delim);
}
#اگر_ختم

#اگر HASH_ALGO_CKSUM
/* Return true if B64_DIGEST is the same as the base64 digest of the
   DIGEST_LENGTH/8 bytes at BIN_BUFFER.  */
ساکن bool
b64_equal (غیرعلامتی حرف ثابت *b64_digest, غیرعلامتی حرف ثابت *bin_بفر)
{
  idx_t b64_n_بائٹیں = BASE64_لمبائی (digest_لمبائی >> 3);
  حرف b64[BASE64_لمبائی (DIGEST_BIN_بائٹیں) + 1];
  base64_encode ((حرف ثابت *) bin_بفر, digest_لمبائی >> 3,
                 b64, حجم b64);
  واپس memeq (b64_digest, b64, b64_n_بائٹیں + 1);
}
#اگر_ختم

/* Return true if HEX_DIGEST is the same as the hex-encoded digest of the
   DIGEST_LENGTH/8 bytes at BIN_BUFFER.  */
ساکن bool
hex_equal (غیرعلامتی حرف ثابت *hex_digest, غیرعلامتی حرف ثابت *bin_بفر)
{
  ساکن ثابت حرف bin2hex[] = { '0', '1', '2', '3',
                                  '4', '5', '6', '7',
                                  '8', '9', 'a', 'b',
                                  'c', 'd', 'e', 'f' };
  idx_t digest_bin_بائٹیں = digest_hex_بائٹیں >> 1;

  /* Compare generated binary number with text representation
     in check file.  Ignore case of hex digits.  */
  idx_t cnt;
  برائے (cnt = 0; cnt < digest_bin_بائٹیں; ++cnt)
    {
      اگر (c_tolower (hex_digest[2 * cnt])
          != bin2hex[bin_بفر[cnt] >> 4]
          || (c_tolower (hex_digest[2 * cnt + 1])
              != (bin2hex[bin_بفر[cnt] & 0xf])))
        توڑو;
    }
  واپس cnt == digest_bin_بائٹیں;
}

ساکن bool
digest_check (حرف ثابت *checkfile_name)
{
  FILE *checkfile_دھارا;
  intmax_t n_misformatted_سطریں = 0;
  intmax_t n_mismatched_checksums = 0;
  intmax_t n_open_or_read_failures = 0;
  bool properly_formatted_سطریں = false;
  bool matched_checksums = false;
  غیرعلامتی حرف bin_بفر_unaligned[DIGEST_BIN_بائٹیں + DIGEST_ALIGN];
  /* Make sure bin_buffer is properly aligned. */
  غیرعلامتی حرف *bin_بفر = ptr_align (bin_بفر_unaligned, DIGEST_ALIGN);
  intmax_t سطر_نمبر;
  حرف *سطر;
  size_t سطر_chars_allocated;
  bool is_stdin = streq (checkfile_name, "-");

  اگر (is_stdin)
    {
      have_read_stdin = true;
      checkfile_name = _("standard input");
      checkfile_دھارا = stdin;
    }
  ورنہ
    {
      checkfile_دھارا = fopen (checkfile_name, "r");
      اگر (checkfile_دھارا == تہی)
        {
          error (0, errno, "%s", quotef (checkfile_name));
          واپس false;
        }
    }

  سطر_نمبر = 0;
  سطر = تہی;
  سطر_chars_allocated = 0;
  کرو
    {
      حرف *filename;
      صحیح ثنائی;
      غیرعلامتی حرف *digest;
      ssize_t سطر_لمبائی;

      ++سطر_نمبر;
      اگر (سطر_نمبر == 0)
        error (EXIT_FAILURE, 0, _("%s: too many checksum lines"),
               quotef (checkfile_name));

      سطر_لمبائی = getline (&سطر, &سطر_chars_allocated, checkfile_دھارا);
      اگر (سطر_لمبائی <= 0)
        توڑو;

      /* Ignore comment lines, which begin with a '#' character.  */
      اگر (سطر[0] == '#')
        جاری;

      /* Remove any trailing newline.  */
      سطر_لمبائی -= سطر[سطر_لمبائی - 1] == '\n';
      /* Remove any trailing carriage return.  */
      سطر_لمبائی -= سطر[سطر_لمبائی - (0 < سطر_لمبائی)] == '\r';

      /* Ignore empty lines.  */
      اگر (سطر_لمبائی == 0)
        جاری;

      سطر[سطر_لمبائی] = '\0';

      idx_t d_len;
      اگر (! (بانٹو_3 (سطر, سطر_لمبائی, &digest, &d_len, &ثنائی, &filename)
             && ! (is_stdin && streq (filename, "-"))))
        {
          ++n_misformatted_سطریں;

          اگر (warn)
            {
              error (0, 0,
                     _("%s: %jd"
                       ": improperly formatted %s checksum line"),
                     quotef (checkfile_name), سطر_نمبر,
                     DIGEST_TYPE_STRING);
            }
        }
      ورنہ
        {
          bool ok;
          bool missing;

          properly_formatted_سطریں = true;

          intmax_t لمبائی;
          ok = digest_file (filename, &ثنائی, bin_بفر, &missing, &لمبائی);

          اگر (!ok)
            {
              ++n_open_or_read_failures;
              اگر (!حالت_only)
                لکھو ("%s: %s\n", quotef (filename),_("FAILED open or read"));
            }
          ورنہ اگر (ignore_missing && missing)
            {
              /* Ignore missing files with --ignore-missing.  */
              ;
            }
          ورنہ
            {
              bool مطابقت = false;
#اگر HASH_ALGO_CKSUM
              اگر (d_len == BASE64_لمبائی (digest_لمبائی >> 3))
                مطابقت = b64_equal (digest, bin_بفر);
              ورنہ
#اگر_ختم
                اگر (d_len == digest_hex_بائٹیں)
                  مطابقت = hex_equal (digest, bin_بفر);

              اگر (مطابقت)
                matched_checksums = true;
              ورنہ
                ++n_mismatched_checksums;

              اگر (!حالت_only)
                {
                  اگر (! مطابقت || ! quiet)
                    fputs (quotef (filename), stdout);

                  اگر (! مطابقت)
                    لکھو (": %s\n", _("FAILED"));
                  ورنہ اگر (!quiet)
                    لکھو (": %s\n", _("OK"));
                }
            }

          اگر (ferror (stdout))
            write_error ();
        }
    }
  جب (!feof (checkfile_دھارا) && !ferror (checkfile_دھارا));

  آزاد (سطر);

  صحیح err = ferror (checkfile_دھارا) ? 0 : -1;
  اگر (is_stdin)
    clearerr (checkfile_دھارا);
  ورنہ اگر (fclose (checkfile_دھارا) != 0 && err < 0)
    err = errno;

  اگر (0 <= err)
    {
      error (0, err, err ? "%s" : _("%s: read error"),
             quotef (checkfile_name));
      واپس false;
    }

  اگر (! properly_formatted_سطریں)
    {
      /* Warn if no tests are found.  */
      error (0, 0, _("%s: no properly formatted checksum lines found"),
             quotef (checkfile_name));
    }
  ورنہ
    {
      اگر (!حالت_only)
        {
          اگر (n_misformatted_سطریں != 0)
            error (0, 0,
                   (ngettext
                    ("WARNING: %jd line is improperly formatted",
                     "WARNING: %jd lines are improperly formatted",
                     select_plural (n_misformatted_سطریں))),
                   n_misformatted_سطریں);

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

  واپس (properly_formatted_سطریں
          && matched_checksums
          && n_mismatched_checksums == 0
          && n_open_or_read_failures == 0
          && (!strict || n_misformatted_سطریں == 0));
}

صحیح
مین (صحیح argc, حرف **argv)
{
  غیرعلامتی حرف bin_بفر_unaligned[DIGEST_BIN_بائٹیں + DIGEST_ALIGN];
  /* Make sure bin_buffer is properly aligned. */
  غیرعلامتی حرف *bin_بفر = ptr_align (bin_بفر_unaligned, DIGEST_ALIGN);
  bool do_check = false;
  صحیح opt;
  bool ok = true;
  صحیح ثنائی = -1;
  صحیح سابقہ_لیبل = -1;
  ساخت option ثابت *long_opts = long_options;

#اگر HASH_ALGO_CKSUM
  اگر (cksum_algorithm != چکری_جانچ)
    {
      legacy_منوال = true;
      سابقہ_لیبل = 0;
      algorithm_specified = true;
      long_opts = legacy_long_options;
    }
#اگر_ختم

  /* Setting values of global variables.  */
  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* Line buffer stdout to ensure lines are written atomically and immediately
     so that processes running in parallel do not intersperse their output.  */
  setvbuf (stdout, تہی, _IOLBF, 0);

#اگر HASH_ALGO_مجموع
  حرف ثابت *short_opts = "rs";
#وگرنہ_اگر HASH_ALGO_CKSUM
  حرف ثابت *short_opts = legacy_منوال ? "bctwz" : "a:l:bctwz";
  حرف ثابت *digest_لمبائی_str = "";
#وگرنہ_اگر HASH_ALGO_BLAKE2
  حرف ثابت *short_opts = "l:bctwz";
  حرف ثابت *digest_لمبائی_str = "";
#ورنہ
  حرف ثابت *short_opts = "bctwz";
#اگر_ختم

  جب ((opt = getopt_long (argc, argv, short_opts, long_opts, تہی))
         != -1)
    انتخاب (opt)
      {
#اگر HASH_ALGO_CKSUM
      حالت 'a':
        cksum_algorithm = XARGMATCH_EXACT ("--algorithm", optarg,
                                           algorithm_دلائل, algorithm_types);
        algorithm_specified = true;
        توڑو;

      حالت DEBUG_پروگرام_OPTION:
        cksum_debug = true;
        توڑو;
#اگر_ختم
#اگر HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
      حالت 'l':
        digest_لمبائی = xnumtoimax (optarg, 10, 0, IDX_MAX, "",
                                    _("invalid length"), 0,
                                    XTOINT_MAX_QUIET);
        digest_لمبائی_str = optarg;
        توڑو;
#اگر_ختم
#اگر !HASH_ALGO_مجموع
      حالت 'c':
        do_check = true;
        توڑو;
      حالت حالت_OPTION:
        حالت_only = true;
        warn = false;
        quiet = false;
        توڑو;
      حالت 'b':
        ثنائی = 1;
        توڑو;
      حالت 't':
        ثنائی = 0;
        توڑو;
      حالت 'w':
        حالت_only = false;
        warn = true;
        quiet = false;
        توڑو;
      حالت IGNORE_MISSING_OPTION:
        ignore_missing = true;
        توڑو;
      حالت QUIET_OPTION:
        حالت_only = false;
        warn = false;
        quiet = true;
        توڑو;
      حالت STRICT_OPTION:
        strict = true;
        توڑو;
# اگر HASH_ALGO_CKSUM
      حالت BASE64_OPTION:
        base64_digest = true;
        توڑو;
      حالت RAW_OPTION:
        raw_digest = true;
        توڑو;
      حالت UNTAG_OPTION:
        سابقہ_لیبل = 0;
        توڑو;
# endif
      حالت لیبل_OPTION:
        سابقہ_لیبل = 1;
        توڑو;
      حالت 'z':
        digest_delim = '\0';
        توڑو;
#اگر_ختم
#اگر HASH_ALGO_مجموع
      حالت 'r':		/* For SysV compatibility. */
        مجموع_algorithm = bsd;
        توڑو;

      حالت 's':
        مجموع_algorithm = sysv;
        توڑو;
#اگر_ختم
      case_GETOPT_HELP_CHAR;
      case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);
      طےشدہ:
        usage (EXIT_FAILURE);
      }

  min_digest_سطر_لمبائی = MIN_DIGEST_سطر_لمبائی;
#اگر HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# اگر HASH_ALGO_CKSUM
  اگر (digest_لمبائی && (cksum_algorithm != blake2b
                        && cksum_algorithm != sha2
                        && cksum_algorithm != sha3))
    error (EXIT_FAILURE, 0,
           _("--length is only supported with --algorithm "
             "blake2b, sha2, or sha3"));
  اگر (cksum_algorithm == sha2 || cksum_algorithm == sha3)
    {
      /* Do not require --length with --check.  */
      اگر (digest_لمبائی == 0 && *digest_لمبائی_str == '\0' && ! do_check)
        error (EXIT_FAILURE, 0, _("--algorithm=%s requires specifying "
                                  "--length 224, 256, 384, or 512"),
               algorithm_دلائل[cksum_algorithm]);
      /* If --check and --length are used we verify the digest length.  */
      اگر ((! do_check || *digest_لمبائی_str != '\0')
          && digest_لمبائی != SHA224_DIGEST_سائز * 8
          && digest_لمبائی != SHA256_DIGEST_سائز * 8
          && digest_لمبائی != SHA384_DIGEST_سائز * 8
          && digest_لمبائی != SHA512_DIGEST_سائز * 8)
        {
          error (0, 0, _("invalid length: %s"), quote (digest_لمبائی_str));
          error (EXIT_FAILURE, 0, _("digest length for %s must be "
                                    "224, 256, 384, or 512"),
                 quote (DIGEST_TYPE_STRING));
        }
    }
  ورنہ
    {
      /* If the digest length checks for SHA-3 are satisfied, the less strict
         checks for BLAKE2 will also be.  */
# ورنہ
    {
# endif
      اگر (digest_لمبائی > DIGEST_MAX_LEN * 8)
        {
          error (0, 0, _("invalid length: %s"), quote (digest_لمبائی_str));
          error (EXIT_FAILURE, 0,
                 _("maximum digest length for %s is %d bits"),
                 quote (DIGEST_TYPE_STRING),
                 DIGEST_MAX_LEN * 8);
        }
      اگر (digest_لمبائی % 8 != 0)
        {
          error (0, 0, _("invalid length: %s"), quote (digest_لمبائی_str));
          error (EXIT_FAILURE, 0, _("length is not a multiple of 8"));
        }
    }
  اگر (digest_لمبائی == 0)
    {
# اگر HASH_ALGO_BLAKE2
      digest_لمبائی = DIGEST_MAX_LEN * 8;
# ورنہ
      digest_لمبائی = algorithm_بٹ[cksum_algorithm];
# endif
    }
  digest_hex_بائٹیں = digest_لمبائی >> 2;
#ورنہ
  digest_hex_بائٹیں = DIGEST_HEX_بائٹیں;
#اگر_ختم

#اگر HASH_ALGO_CKSUM
  انتخاب (+cksum_algorithm)
    {
    حالت bsd:
    حالت sysv:
    حالت چکری_جانچ:
    حالت crc32b:
        اگر (do_check && algorithm_specified)
          error (EXIT_FAILURE, 0,
                 _("--check is not supported with "
                   "--algorithm={bsd,sysv,crc,crc32b}"));
        توڑو;
    }

  اگر (base64_digest && raw_digest)
   {
     error (0, 0, _("--base64 and --raw are mutually exclusive"));
     usage (EXIT_FAILURE);
   }
#اگر_ختم

  اگر (digest_delim != '\n' && do_check)
    {
      error (0, 0, _("the --zero option is not supported when "
                     "verifying checksums"));
      usage (EXIT_FAILURE);
    }
  اگر (1 <= سابقہ_لیبل && do_check)
    {
      /* Note we allow --untagged with --check to more
         seamlessly support --untagged in an emulation wrapper.  */
      error (0, 0, _("the --tag option is meaningless when "
                     "verifying checksums"));
      usage (EXIT_FAILURE);
    }

  اگر (0 <= ثنائی && do_check)
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

  اگر (حالت_only && !do_check)
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

  اگر (سابقہ_لیبل == -1)
    سابقہ_لیبل = HASH_ALGO_CKSUM;

  اگر (سابقہ_لیبل && !ثنائی)
   {
     /* This could be supported in a backwards compatible way
        by prefixing the output line with a space in text mode.
        However that's invasive enough that it was agreed to
        not support this mode with --tag, as --text use cases
        are adequately supported by the default output format.  */
#اگر !HASH_ALGO_CKSUM
     error (0, 0, _("--tag does not support --text mode"));
#ورنہ
     error (0, 0, _("--text mode is only supported with --untagged"));
#اگر_ختم
     usage (EXIT_FAILURE);
   }

  اگر (!O_ثنائی && ثنائی < 0)
    ثنائی = 0;
  ورنہ اگر (سابقہ_لیبل)
    ثنائی = 1;

  حرف **عملیہ_lim = argv + argc;
  اگر (optind == argc)
    *عملیہ_lim++ = bad_cast ("-");
  ورنہ اگر (1 < argc - optind && raw_digest)
    error (EXIT_FAILURE, 0,
           _("the --raw option is not supported with multiple files"));

  برائے (حرف **operandp = argv + optind; operandp < عملیہ_lim; operandp++)
    {
      حرف *file = *operandp;
      اگر (do_check)
        ok &= digest_check (file);
      ورنہ
        {
          صحیح ثنائی_file = ثنائی;
          bool missing;
          intmax_t لمبائی;

          اگر (! digest_file (file, &ثنائی_file, bin_بفر, &missing, &لمبائی))
            ok = false;
          ورنہ
            {
              DIGEST_برآمد (file, ثنائی_file, bin_بفر, raw_digest, سابقہ_لیبل,
                          digest_delim, optind != argc, لمبائی);
              اگر (ferror (stdout))
                write_error ();
            }
        }
    }

  اگر (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, _("standard input"));

  واپس ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
