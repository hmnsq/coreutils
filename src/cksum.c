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

#جيڪڏهن_وصف_نه_ٿيل HASH_ALGO_CKSUM
# define HASH_ALGO_CKSUM 0
#جيڪڏهن_پورو

#إذا HASH_ALGO_جوڙ || HASH_ALGO_CKSUM
# include "sum.h"
#جيڪڏهن_پورو
#إذا HASH_ALGO_CKSUM
# include "cksum.h"
# include "cksum_crc.h"
# include "base64.h"
#جيڪڏهن_پورو
#إذا HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# include "blake2/b2sum.h"
#جيڪڏهن_پورو
#إذا HASH_ALGO_MD5 || HASH_ALGO_CKSUM
# include "md5.h"
#جيڪڏهن_پورو
#إذا HASH_ALGO_SHA1 || HASH_ALGO_CKSUM
# include "sha1.h"
#جيڪڏهن_پورو
#إذا HASH_ALGO_SHA256 || HASH_ALGO_SHA224 || HASH_ALGO_CKSUM
# include "sha256.h"
#جيڪڏهن_پورو
#إذا HASH_ALGO_SHA512 || HASH_ALGO_SHA384 || HASH_ALGO_CKSUM
# include "sha512.h"
#جيڪڏهن_پورو
#إذا HASH_ALGO_CKSUM
# include "sha3.h"
#جيڪڏهن_پورو
#إذا HASH_ALGO_CKSUM
# include "sm3.h"
#جيڪڏهن_پورو
#ضمن "fadvise.h"
#ضمن "stdio--.h"
#ضمن "xbinary-io.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#إذا HASH_ALGO_جوڙ
# define پروگرام_NAME "sum"
# define DIGEST_TYPE_STRING "BSD"
# define DIGEST_وهڪرو sumfns[جوڙ_algorithm]
# define DIGEST_ٻاهريون جوڙ_output_fns[جوڙ_algorithm]
# define DIGEST_بٽ 16
# define DIGEST_ALIGN 4
#وإلاإذا HASH_ALGO_CKSUM
# define MAX_DIGEST_بٽ 512
# define MAX_DIGEST_ALIGN 8
# define DIGEST_TYPE_STRING algorithm_ٽيگ_فهرست[cksum_algorithm]
# define DIGEST_وهڪرو cksumfns[cksum_algorithm]
# define DIGEST_ٻاهريون cksum_output_fns[cksum_algorithm]
# define DIGEST_بٽ MAX_DIGEST_بٽ
# define DIGEST_ALIGN MAX_DIGEST_ALIGN
#وإلاإذا HASH_ALGO_MD5
# define پروگرام_NAME "md5sum"
# define DIGEST_TYPE_STRING "MD5"
# define DIGEST_وهڪرو md5_وهڪرو
# define DIGEST_بٽ 128
# define DIGEST_REFERENCE "RFC 1321"
# define DIGEST_ALIGN 4
#وإلاإذا HASH_ALGO_BLAKE2
# define پروگرام_NAME "b2sum"
# define DIGEST_TYPE_STRING "BLAKE2b"
# define DIGEST_وهڪرو blake2b_وهڪرو
# define DIGEST_بٽ 512
# define DIGEST_REFERENCE "RFC 7693"
# define DIGEST_ALIGN 8
#وإلاإذا HASH_ALGO_SHA1
# define پروگرام_NAME "sha1sum"
# define DIGEST_TYPE_STRING "SHA1"
# define DIGEST_وهڪرو sha1_وهڪرو
# define DIGEST_بٽ 160
# define DIGEST_REFERENCE "FIPS-180-1"
# define DIGEST_ALIGN 4
#وإلاإذا HASH_ALGO_SHA256
# define پروگرام_NAME "sha256sum"
# define DIGEST_TYPE_STRING "SHA256"
# define DIGEST_وهڪرو sha256_وهڪرو
# define DIGEST_بٽ 256
# define DIGEST_REFERENCE "FIPS-180-2"
# define DIGEST_ALIGN 4
#وإلاإذا HASH_ALGO_SHA224
# define پروگرام_NAME "sha224sum"
# define DIGEST_TYPE_STRING "SHA224"
# define DIGEST_وهڪرو sha224_وهڪرو
# define DIGEST_بٽ 224
# define DIGEST_REFERENCE "RFC 3874"
# define DIGEST_ALIGN 4
#وإلاإذا HASH_ALGO_SHA512
# define پروگرام_NAME "sha512sum"
# define DIGEST_TYPE_STRING "SHA512"
# define DIGEST_وهڪرو sha512_وهڪرو
# define DIGEST_بٽ 512
# define DIGEST_REFERENCE "FIPS-180-2"
# define DIGEST_ALIGN 8
#وإلاإذا HASH_ALGO_SHA384
# define پروگرام_NAME "sha384sum"
# define DIGEST_TYPE_STRING "SHA384"
# define DIGEST_وهڪرو sha384_وهڪرو
# define DIGEST_بٽ 384
# define DIGEST_REFERENCE "FIPS-180-2"
# define DIGEST_ALIGN 8
#وإلا
# error "Can't decide which hash algorithm to compile."
#جيڪڏهن_پورو
#إذا !HASH_ALGO_جوڙ && !HASH_ALGO_CKSUM
# define DIGEST_ٻاهريون output_file
#جيڪڏهن_پورو

#إذا HASH_ALGO_CKSUM
# define پروگرام_NAME (!legacy_گهڻائي ? "cksum"			 \
                       : cksum_algorithm == md5 ? "md5sum"	 \
                       : cksum_algorithm == sha1 ? "sha1sum"	 \
                       : cksum_algorithm == sha224 ? "sha224sum" \
                       : cksum_algorithm == sha256 ? "sha256sum" \
                       : cksum_algorithm == sha384 ? "sha384sum" \
                       : cksum_algorithm == sha512 ? "sha512sum" \
                       : "cksum")
#جيڪڏهن_پورو

#إذا HASH_ALGO_جوڙ
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
#جيڪڏهن_پورو
#إذا !HASH_ALGO_BLAKE2 && !HASH_ALGO_CKSUM
# define DIGEST_HEX_بائيٽون (DIGEST_بٽ / 4)
#جيڪڏهن_پورو
#وصف_ڪريو DIGEST_BIN_بائيٽون (DIGEST_بٽ / 8)

/* The minimum length of a valid digest line.  This length does
   not include any newline character at the end of a line.  */
#إذا HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# define MIN_DIGEST_سٽ_ڊيگهائي 3 /* With -l 8.  */
#وإلا
# define MIN_DIGEST_سٽ_ڊيگهائي \
   (DIGEST_HEX_بائيٽون /* length of hexadecimal message digest */ \
    + 1 /* blank */ \
    + 1 /* minimum filename length */ )
#جيڪڏهن_پورو

#إذا !HASH_ALGO_جوڙ
ساكن فراغ
output_file (حرف ثابت *file, صحيح ٻٽو_file, فراغ ثابت *digest,
             bool raw, bool tagged, بنانشان حرف delim, bool دليلون,
             intmax_t ڊيگهائي);
#جيڪڏهن_پورو

/* True if any of the files read were the standard input. */
ساكن bool have_read_stdin;

/* The minimum length of a valid checksum line for the selected algorithm.  */
ساكن idx_t min_digest_سٽ_ڊيگهائي;

/* Set to the length of a digest hex string for the selected algorithm.  */
ساكن idx_t digest_hex_بائيٽون;

/* With --check, don't generate any output.
   The exit code indicates success or failure.  */
ساكن bool حالت_only = false;

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
ساكن بنانشان حرف digest_delim = '\n';

#إذا HASH_ALGO_CKSUM
/* If true, print base64-encoded digests, not hex.  */
ساكن bool base64_digest = false;
#جيڪڏهن_پورو

/* If true, print binary digests, not hex.  */
ساكن bool raw_digest = false;

/* blake2b and sha3 allow the -l option.  Luckily they both have the same
   maximum digest size.  */
#إذا HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# إذا HASH_ALGO_CKSUM
static_assert (BLAKE2B_OUTBYTES == SHA3_512_DIGEST_سائيز);
# endif
# define DIGEST_MAX_LEN BLAKE2B_OUTBYTES
ساكن idx_t digest_ڊيگهائي;
#جيڪڏهن_پورو /* HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM */

تعريفنوع فراغ (*digest_output_fn)(حرف ثابت *, صحيح, فراغ ثابت *, bool,
                                 bool, بنانشان حرف, bool, intmax_t);
#إذا HASH_ALGO_جوڙ
تعداد Algorithm
{
  bsd,
  sysv,
};

ساكن تعداد Algorithm جوڙ_algorithm;
ساكن sumfn sumfns[]=
{
  bsd_جوڙ_وهڪرو,
  sysv_جوڙ_وهڪرو,
};
ساكن digest_output_fn جوڙ_output_fns[]=
{
  output_bsd,
  output_sysv,
};
#جيڪڏهن_پورو

#إذا HASH_ALGO_CKSUM
ساكن صحيح
md5_جوڙ_وهڪرو (FILE *وهڪرو, فراغ *resstream, MAYBE_اڻ_استعمال_ٿيل intmax_t *ڊيگهائي)
{
  أرجع md5_وهڪرو (وهڪرو, resstream);
}
ساكن صحيح
sha1_جوڙ_وهڪرو (FILE *وهڪرو, فراغ *resstream, MAYBE_اڻ_استعمال_ٿيل intmax_t *ڊيگهائي)
{
  أرجع sha1_وهڪرو (وهڪرو, resstream);
}
ساكن صحيح
sha224_جوڙ_وهڪرو (FILE *وهڪرو, فراغ *resstream,
                   MAYBE_اڻ_استعمال_ٿيل intmax_t *ڊيگهائي)
{
  أرجع sha224_وهڪرو (وهڪرو, resstream);
}
ساكن صحيح
sha256_جوڙ_وهڪرو (FILE *وهڪرو, فراغ *resstream,
                   MAYBE_اڻ_استعمال_ٿيل intmax_t *ڊيگهائي)
{
  أرجع sha256_وهڪرو (وهڪرو, resstream);
}
ساكن صحيح
sha384_جوڙ_وهڪرو (FILE *وهڪرو, فراغ *resstream,
                   MAYBE_اڻ_استعمال_ٿيل intmax_t *ڊيگهائي)
{
  أرجع sha384_وهڪرو (وهڪرو, resstream);
}
ساكن صحيح
sha512_جوڙ_وهڪرو (FILE *وهڪرو, فراغ *resstream,
                   MAYBE_اڻ_استعمال_ٿيل intmax_t *ڊيگهائي)
{
  أرجع sha512_وهڪرو (وهڪرو, resstream);
}
ساكن صحيح
sha2_جوڙ_وهڪرو (FILE *وهڪرو, فراغ *resstream, intmax_t *ڊيگهائي)
{
  بدل (*ڊيگهائي)
    {
    حالة SHA224_DIGEST_سائيز:
      أرجع sha224_وهڪرو (وهڪرو, resstream);
    حالة SHA256_DIGEST_سائيز:
      أرجع sha256_وهڪرو (وهڪرو, resstream);
    حالة SHA384_DIGEST_سائيز:
      أرجع sha384_وهڪرو (وهڪرو, resstream);
    حالة SHA512_DIGEST_سائيز:
      أرجع sha512_وهڪرو (وهڪرو, resstream);
    افتراضي:
      affirm (false);
    }
}
ساكن صحيح
sha3_جوڙ_وهڪرو (FILE *وهڪرو, فراغ *resstream, intmax_t *ڊيگهائي)
{
  بدل (*ڊيگهائي)
    {
    حالة SHA3_224_DIGEST_سائيز:
      أرجع sha3_224_وهڪرو (وهڪرو, resstream);
    حالة SHA3_256_DIGEST_سائيز:
      أرجع sha3_256_وهڪرو (وهڪرو, resstream);
    حالة SHA3_384_DIGEST_سائيز:
      أرجع sha3_384_وهڪرو (وهڪرو, resstream);
    حالة SHA3_512_DIGEST_سائيز:
      أرجع sha3_512_وهڪرو (وهڪرو, resstream);
    افتراضي:
      affirm (false);
    }
}
ساكن صحيح
blake2b_جوڙ_وهڪرو (FILE *وهڪرو, فراغ *resstream, intmax_t *ڊيگهائي)
{
  أرجع blake2b_وهڪرو (وهڪرو, resstream, *ڊيگهائي);
}
ساكن صحيح
sm3_جوڙ_وهڪرو (FILE *وهڪرو, فراغ *resstream, MAYBE_اڻ_استعمال_ٿيل intmax_t *ڊيگهائي)
{
  أرجع sm3_وهڪرو (وهڪرو, resstream);
}

ساكن حرف ثابت *ثابت algorithm_دليلون[] =
{
  "bsd", "sysv", "crc", "crc32b", "md5", "sha1",
  "sha224", "sha256", "sha384", "sha512", /* Legacy naming */
  "sha2", "sha3", "blake2b", "sm3", عدم
};
ساكن تعداد Algorithm ثابت algorithm_types[] =
{
  bsd, sysv, چڪري_جاچ, crc32b, md5, sha1,
  sha224, sha256, sha384, sha512,
  sha2, sha3, blake2b, sm3,
};
ARGMATCH_VERIFY (algorithm_دليلون, algorithm_types);

ساكن حرف ثابت *ثابت algorithm_ٽيگ_فهرست[] =
{
  "BSD", "SYSV", "CRC", "CRC32B", "MD5", "SHA1",
  "SHA224", "SHA256", "SHA384", "SHA512",
  "SHA2", "SHA3", "BLAKE2b", "SM3", عدم
};
ساكن صحيح ثابت algorithm_بٽ[] =
{
  16, 16, 32, 32, 128, 160,
  224, 256, 384, 512,
  512, 512, 512, 256, 0
};

static_assert (countof (algorithm_بٽ) == countof (algorithm_دليلون));

ساكن bool algorithm_specified = false;
ساكن bool legacy_گهڻائي = false;
تعداد Algorithm cksum_algorithm = چڪري_جاچ;

ساكن sumfn cksumfns[]=
{
  bsd_جوڙ_وهڪرو,
  sysv_جوڙ_وهڪرو,
  چڪري_جاچ_جوڙ_وهڪرو,
  crc32b_جوڙ_وهڪرو,
  md5_جوڙ_وهڪرو,
  sha1_جوڙ_وهڪرو,
  sha224_جوڙ_وهڪرو,
  sha256_جوڙ_وهڪرو,
  sha384_جوڙ_وهڪرو,
  sha512_جوڙ_وهڪرو,
  sha2_جوڙ_وهڪرو,
  sha3_جوڙ_وهڪرو,
  blake2b_جوڙ_وهڪرو,
  sm3_جوڙ_وهڪرو,
};
ساكن digest_output_fn cksum_output_fns[]=
{
  output_bsd,
  output_sysv,
  output_چڪري_جاچ,
  output_چڪري_جاچ,
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
#جيڪڏهن_پورو

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */

تعداد
{
  IGNORE_MISSING_OPTION = CHAR_MAX + 1,
  حالت_OPTION,
  QUIET_OPTION,
  STRICT_OPTION,
  ٽيگ_OPTION,
  UNTAG_OPTION,
  DEBUG_پروگرام_OPTION,
  RAW_OPTION,
  BASE64_OPTION,
};

#إذا HASH_ALGO_CKSUM
ساكن بنية option ثابت legacy_long_options[] =
{
  { "check", no_دليل, عدم, 'c' },
  { "ignore-missing", no_دليل, عدم, IGNORE_MISSING_OPTION },
  { "quiet", no_دليل, عدم, QUIET_OPTION },
  { "status", no_دليل, عدم, حالت_OPTION },
  { "warn", no_دليل, عدم, 'w' },
  { "strict", no_دليل, عدم, STRICT_OPTION },
  { "tag", no_دليل, عدم, ٽيگ_OPTION },
  { "zero", no_دليل, عدم, 'z' },

  { "binary", no_دليل, عدم, 'b' }, /* Deprecated.  */
  { "text", no_دليل, عدم, 't' }, /* Deprecated.  */

  { GETOPT_HELP_OPTION_DECL },
  { GETOPT_نسخو_OPTION_DECL },
  { عدم, 0, عدم, 0 }
};
#جيڪڏهن_پورو

ساكن بنية option ثابت long_options[] =
{
#إذا HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
  { "length", required_دليل, عدم, 'l'},
#جيڪڏهن_پورو

#إذا !HASH_ALGO_جوڙ
  { "check", no_دليل, عدم, 'c' },
  { "ignore-missing", no_دليل, عدم, IGNORE_MISSING_OPTION},
  { "quiet", no_دليل, عدم, QUIET_OPTION },
  { "status", no_دليل, عدم, حالت_OPTION },
  { "warn", no_دليل, عدم, 'w' },
  { "strict", no_دليل, عدم, STRICT_OPTION },
  { "tag", no_دليل, عدم, ٽيگ_OPTION },
  { "zero", no_دليل, عدم, 'z' },

# إذا HASH_ALGO_CKSUM
  { "algorithm", required_دليل, عدم, 'a'},
  { "base64", no_دليل, عدم, BASE64_OPTION },
  { "debug", no_دليل, عدم, DEBUG_پروگرام_OPTION},
  { "raw", no_دليل, عدم, RAW_OPTION},
  { "untagged", no_دليل, عدم, UNTAG_OPTION },
# endif
  { "binary", no_دليل, عدم, 'b' }, /* Deprecated.  */
  { "text", no_دليل, عدم, 't' }, /* Deprecated.  */

#وإلا
  {"sysv", no_دليل, عدم, 's'},  /* Not in cksum.  */
#جيڪڏهن_پورو

  { GETOPT_HELP_OPTION_DECL },
  { GETOPT_نسخو_OPTION_DECL },
  { عدم, 0, عدم, 0 }
};

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکو (_("\
Usage: %s [OPTION]... [FILE]...\n\
"), پروگرام_name);
#إذا HASH_ALGO_CKSUM
    إذا (legacy_گهڻائي)
      {
        لکو (_("\
Print or check %s checksums.\n\
"), DIGEST_TYPE_STRING);
        fputs (_("\
Legacy interface منزل the cksum(1) utility.\n\
"), stdout);
      }
    وإلا
      fputs (_("\
Print or verify checksums.\n\
By افتراضي use the 32 ٻٽو_انگ چڪري_جاچ_2 algorithm.\n\
"), stdout);
#وإلا
      لکو (_("\
Print or check %s (%d-ٻٽو_انگ) checksums.\n\
"),
              DIGEST_TYPE_STRING,
              DIGEST_بٽ);
      fputs (_("\
Legacy interface منزل the cksum(1) utility.\n\
"), stdout);
#جيڪڏهن_پورو

      emit_stdin_note ();
#إذا HASH_ALGO_جوڙ
      oputs (_("\
  -r\n\
         use BSD جوڙ algorithm (the افتراضي), use 1K بلاڪون\n\
"));
      oputs (_("\
  -s, --sysv\n\
         use System V جوڙ algorithm, use 512 بائيٽون بلاڪون\n\
"));
#جيڪڏهن_پورو
#إذا HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
        emit_mandatory_arg_note ();
#جيڪڏهن_پورو
#إذا HASH_ALGO_CKSUM
     إذا (! legacy_گهڻائي)
        oputs (_("\
  -a, --algorithm=TYPE\n\
         select the digest type منزل use.  See DIGEST هيٺ\n\
"));
     إذا (! legacy_گهڻائي)
        oputs (_("\
      --base64\n\
         emit base64-encoded digests, not hexadecimal\n\
"));
#جيڪڏهن_پورو
#إذا !HASH_ALGO_جوڙ
# إذا HASH_ALGO_CKSUM
     إذا (legacy_گهڻائي)
       {
# endif
      إذا (O_ٻٽو)
        oputs (_("\
  -b, --ٻٽو\n\
         اقرأ اندريون ٻٽو گهڻائي_2 (افتراضي unless reading tty stdin)\n\
"));
      وإلا
        oputs (_("\
  -b, --ٻٽو\n\
         اقرأ اندريون ٻٽو گهڻائي_2\n\
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
     إذا (! legacy_گهڻائي)
#  endif
        oputs (_("\
  -l, --ڊيگهائي=بٽ\n\
         digest ڊيگهائي اندريون بٽ_2; must not exceed the max سائيز\n\
         and must be a گڻو جو 8 لكل blake2b;\n\
         must be 224, 256, 384, or 512 لكل sha2 or sha3\n\
"));
# endif
# إذا HASH_ALGO_CKSUM
    إذا (! legacy_گهڻائي)
        oputs (_("\
      --raw\n\
         emit a raw ٻٽو digest, not hexadecimal\n\
"));
    إذا (legacy_گهڻائي)
      oputs (_("\
      --ٽيگ\n\
         create a BSD-style checksum\n\
"));
    وإلا
      oputs (_("\
      --ٽيگ\n\
         create a BSD-style checksum (the افتراضي)\n\
"));
    إذا (! legacy_گهڻائي)
      oputs (_("\
      --untagged\n\
         create a reversed style checksum, without digest type\n\
"));
# وإلا
      oputs (_("\
      --ٽيگ\n\
         create a BSD-style checksum\n\
"));
# endif
# إذا HASH_ALGO_CKSUM
    إذا (legacy_گهڻائي)
      {
# endif
      إذا (O_ٻٽو)
        oputs (_("\
  -t, --متن\n\
         اقرأ اندريون متن گهڻائي_2 (افتراضي إذا reading tty stdin)\n\
"));
      وإلا
        oputs (_("\
  -t, --متن\n\
         اقرأ اندريون متن گهڻائي_2 (افتراضي)\n\
"));
# إذا HASH_ALGO_CKSUM
      }
# endif
      oputs (_("\
  -z, --zero\n\
         پڇاڙي each output سٽ with NUL, not newline,\n\
         and disable file name escaping\n\
"));
      fputs (_("\
\n\
The following five options are useful only when verifying checksums:\n\
"), stdout);
      oputs (_("\
      --ignore-missing\n\
         don't fail or report حالت لكل missing files\n\
"));
      oputs (_("\
      --quiet\n\
         don't print OK لكل each successfully verified file\n\
"));
      oputs (_("\
      --حالت\n\
         don't output anything, حالت ڪوڊ shows success\n\
"));
      oputs (_("\
      --strict\n\
         ٻاهر_نڪرو non-zero لكل improperly formatted checksum سٽون\n\
"));
      oputs (_("\
  -w, --warn\n\
         warn about improperly formatted checksum سٽون\n\
"));
#جيڪڏهن_پورو
#إذا HASH_ALGO_CKSUM
    إذا (! legacy_گهڻائي)
      oputs (_("\
      --debug\n\
         indicate which implementation استعمال_ٿيل\n\
"));
#جيڪڏهن_پورو
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخو_OPTION_DESCRIPTION);
#إذا HASH_ALGO_CKSUM
    إذا (! legacy_گهڻائي)
      fputs (_("\
\n\
DIGEST determines the digest algorithm and افتراضي output format:\n\
  sysv      (equivalent منزل جوڙ -s)\n\
  bsd       (equivalent منزل جوڙ -r)\n\
  چڪري_جاچ       (equivalent منزل cksum)\n\
  crc32b    (only موجود through cksum)\n\
  md5       (equivalent منزل md5sum)\n\
  sha1      (equivalent منزل sha1sum)\n\
  sha2      (equivalent منزل sha{224,256,384,512}جوڙ)\n\
  sha3      (only موجود through cksum)\n\
  blake2b   (equivalent منزل b2sum)\n\
  sm3       (only موجود through cksum)\n\
\n"), stdout);
#جيڪڏهن_پورو
#إذا !HASH_ALGO_جوڙ && !HASH_ALGO_CKSUM
      لکو (_("\
\n\
The sums are computed as described اندريون %s.\n"), DIGEST_REFERENCE);
      fputs (_("\
When checking, the داخلا should be a former output جو this پروگرام.\n\
The افتراضي گهڻائي_2 is منزل print a سٽ with: checksum, a space,\n\
a character indicating داخلا گهڻائي_2 ('*' لكل ٻٽو, ' ' لكل متن\n\
or where ٻٽو is insignificant), and name لكل each FILE.\n\
\n\
There is no difference between ٻٽو گهڻائي_2 and متن گهڻائي_2 on GNU systems.\
\n"), stdout);
#جيڪڏهن_پورو
#إذا HASH_ALGO_CKSUM
      fputs (_("\
When checking, the داخلا should be a former output جو this پروگرام,\n\
or equivalent standalone پروگرام.\
\n"), stdout);
#جيڪڏهن_پورو
      emit_ancillary_info (پروگرام_NAME);
    }

  ٻاهر_نڪرو (حالت);
}

#إذا !HASH_ALGO_جوڙ
/* Given a string S, return TRUE if it contains problematic characters
   that need escaping.  Note we escape '\' itself to provide some forward
   compat to introduce escaping of other characters.  */

ATTRIBUTE_خالص
ساكن bool
problematic_chars (حرف ثابت *s)
{
  idx_t ڊيگهائي = strcspn (s, "\\\n\r");
  أرجع s[ڊيگهائي] != '\0';
}
#جيڪڏهن_پورو

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
valid_digits (بنانشان حرف ثابت *s, idx_t len)
{
#إذا HASH_ALGO_CKSUM
  إذا (len == BASE64_ڊيگهائي (digest_ڊيگهائي >> 3))
    {
      idx_t i;
      لكل (i = 0; i < len - digest_ڊيگهائي % 3; i++)
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
#جيڪڏهن_پورو
  إذا (len == digest_hex_بائيٽون)
    {
      لكل (idx_t i = 0; i < digest_hex_بائيٽون; i++)
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
bsd_ورهايو_3 (حرف *s, idx_t s_len,
             بنانشان حرف **digest, idx_t *d_len,
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

  *digest = (بنانشان حرف *) &s[i];

  *d_len = s_len - i;
  أرجع valid_digits (*digest, *d_len);
}

#إذا HASH_ALGO_CKSUM
/* Return the corresponding Algorithm for the string S,
   or -1 for no match.  */

ساكن ptrdiff_t
algorithm_from_ٽيگ (حرف *s)
{
  /* Limit check size to this length for perf reasons.  */
  ساكن idx_t max_ٽيگ_len;
  إذا (! max_ٽيگ_len)
    {
      حرف ثابت * ثابت * ٽيگ = algorithm_ٽيگ_فهرست;
      طالما (*ٽيگ)
        {
          idx_t ٽيگ_len = لڙي_ڊيگهه (*ٽيگ++);
          max_ٽيگ_len = MAX (ٽيگ_len, max_ٽيگ_len);
        }
    }

  idx_t i = 0;

  /* Find end of tag */
  طالما (i <= max_ٽيگ_len && s[i] && ! c_isblank (s[i])
         && s[i] != '-' && s[i] != '(')
    ++i;

  إذا (i > max_ٽيگ_len)
    أرجع -1;

  /* Terminate tag, and lookup.  */
  حرف sep = s[i];
  s[i] = '\0';
  ptrdiff_t algo = argmatch_exact (s, algorithm_ٽيگ_فهرست);
  s[i] = sep;

  أرجع algo;
}
#جيڪڏهن_پورو

/* Split the string S (of length S_LEN) into three parts:
   a hexadecimal digest, binary flag, and the file name.
   S is modified.  Set *D_LEN to the length of the digest string.
   Return true if successful.  */

ساكن bool
ورهايو_3 (حرف *s, idx_t s_len,
         بنانشان حرف **digest, idx_t *d_len, صحيح *ٻٽو, حرف **file_name)
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
      ptrdiff_t algo_ٽيگ = algorithm_from_ٽيگ (s + i);
      إذا (! algorithm_specified)
        {
          إذا (algo_ٽيگ >= 0)
            {
              إذا (algo_ٽيگ <= crc32b)
                أرجع false;  /* We don't support checking these formats.  */
              cksum_algorithm = algo_ٽيگ;
            }
          وإلا
            أرجع false;      /* We only support tagged format without -a.  */
        }
      وإلا
        {
          إذا (cksum_algorithm == sha2 && (algo_ٽيگ == sha2
              || algo_ٽيگ == sha224 || algo_ٽيگ == sha256
              || algo_ٽيگ == sha384 || algo_ٽيگ == sha512))
            cksum_algorithm = algo_ٽيگ;
        }
    }
#جيڪڏهن_پورو

  /* Try to parse BSD or OpenSSL tagged format.  I.e.:
     openssl: MD5(f)= d41d8cd98f00b204e9800998ecf8427e
     bsd:     MD5 (f) = d41d8cd98f00b204e9800998ecf8427e  */

  idx_t parse_وٿي = i;
  algo_name_len = لڙي_ڊيگهه (DIGEST_TYPE_STRING);
  إذا (STREQ_LEN (s + i, DIGEST_TYPE_STRING, algo_name_len))
    {
      i += algo_name_len;
#إذا HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM

# إذا HASH_ALGO_BLAKE2
      digest_ڊيگهائي = DIGEST_MAX_LEN * 8;
# وإلا
      digest_ڊيگهائي = algorithm_بٽ[cksum_algorithm];
# endif
      إذا (s[i] == '-')  /* length specified. Not base64 */
        {
          ++i;
          intmax_t ڊيگهائي;
          حرف *siend;
          إذا (!c_isdigit (s[i])
              || xstrtoimax (s + i, &siend, 0, &ڊيگهائي, عدم) != LONGINT_OK)
            أرجع false;
# إذا HASH_ALGO_CKSUM
          وإلا إذا (cksum_algorithm == sha2 || cksum_algorithm == sha3)
            {
              إذا (ڊيگهائي != SHA224_DIGEST_سائيز * 8
                  && ڊيگهائي != SHA256_DIGEST_سائيز * 8
                  && ڊيگهائي != SHA384_DIGEST_سائيز * 8
                  && ڊيگهائي != SHA512_DIGEST_سائيز * 8)
                أرجع false;
            }
# endif
          وإلا إذا (!(0 < ڊيگهائي && ڊيگهائي <= digest_ڊيگهائي && ڊيگهائي % 8 == 0))
            أرجع false;

          i = siend - s;
          digest_ڊيگهائي = ڊيگهائي;
        }
      digest_hex_بائيٽون = digest_ڊيگهائي >> 2;
#جيڪڏهن_پورو
      إذا (s[i] == ' ')
        ++i;
      إذا (s[i] == '(')  /* not base64 */
        {
          ++i;
          *ٻٽو = 0;
          أرجع bsd_ورهايو_3 (s + i, s_len - i,
                              digest, d_len, file_name, escaped_filename);
        }

      /* Note with --base64 --untagged format, we may have matched a "tag".
         Even very short digests with: cksum -a blake2b -l24 --untagged --base64
         So fallback to checking untagged format if issues detecting tags.  */
      i = parse_وٿي;
    }

  /* Ignore this line if it is too short.
     Each line must have at least 'min_digest_line_length - 1' (or one more, if
     the first is a backslash) more characters to contain correct message digest
     information.  */
  إذا (s_len - i < min_digest_سٽ_ڊيگهائي + (s[i] == '\\'))
    أرجع false;

  *digest = (بنانشان حرف *) &s[i];

#إذا HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
  /* Auto determine length.  */
# إذا HASH_ALGO_CKSUM
  إذا (cksum_algorithm == blake2b
      || cksum_algorithm == sha2 || cksum_algorithm == sha3) {
# endif
  بنانشان حرف ثابت *hp = *digest;
  digest_hex_بائيٽون = 0;
  لكل (; c_isxdigit (*hp); ++hp, ++digest_hex_بائيٽون)
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
  idx_t digest_base64_بائيٽون = digest_hex_بائيٽون;
  idx_t trailing_equals = 0;
  لكل (; isubase64 (*hp); ++hp, ++digest_base64_بائيٽون)
    ;
  لكل (; *hp == '='; ++hp, ++trailing_equals)
    ;
  إذا ((cksum_algorithm == sha2 || cksum_algorithm == sha3)
      && digest_hex_بائيٽون >> 1 != SHA224_DIGEST_سائيز
      && digest_hex_بائيٽون >> 1 != SHA256_DIGEST_سائيز
      && digest_hex_بائيٽون >> 1 != SHA384_DIGEST_سائيز
      && digest_hex_بائيٽون >> 1 != SHA512_DIGEST_سائيز)
    {
      إذا (digest_base64_بائيٽون + trailing_equals
          == BASE64_ڊيگهائي (SHA224_DIGEST_سائيز))
        digest_hex_بائيٽون = SHA224_DIGEST_سائيز * 2;
      وإلا إذا (digest_base64_بائيٽون + trailing_equals
               == BASE64_ڊيگهائي (SHA256_DIGEST_سائيز))
        digest_hex_بائيٽون = SHA256_DIGEST_سائيز * 2;
      وإلا إذا (digest_base64_بائيٽون + trailing_equals
               == BASE64_ڊيگهائي (SHA384_DIGEST_سائيز))
        digest_hex_بائيٽون = SHA384_DIGEST_سائيز * 2;
      وإلا إذا (digest_base64_بائيٽون + trailing_equals
               == BASE64_ڊيگهائي (SHA512_DIGEST_سائيز))
        digest_hex_بائيٽون = SHA512_DIGEST_سائيز * 2;
      وإلا
        أرجع false;
    }
  وإلا إذا (cksum_algorithm == blake2b
           && digest_hex_بائيٽون < digest_base64_بائيٽون)
    {
      لكل (صحيح j = 8; j <= DIGEST_MAX_LEN * 8; j += 8)
        {
          إذا (BASE64_ڊيگهائي (j / 8) == digest_base64_بائيٽون + trailing_equals
              && j % 3 == trailing_equals)
            {
              digest_hex_بائيٽون = j / 4;
              اكسر;
            }
        }
    }
# endif
  إذا (digest_hex_بائيٽون < 2 || digest_hex_بائيٽون % 2
      || DIGEST_MAX_LEN * 2 < digest_hex_بائيٽون)
    أرجع false;
  digest_ڊيگهائي = digest_hex_بائيٽون * 4;
# إذا HASH_ALGO_CKSUM
  }
# endif
#جيڪڏهن_پورو

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
      *ٻٽو = (s[i++] == '*');
    }

  /* All characters between the type indicator and end of line are
     significant -- that includes leading and trailing white space.  */
  *file_name = &s[i];

  إذا (escaped_filename)
    أرجع filename_unescape (&s[i], s_len - i) != عدم;

  أرجع true;
}

#إذا !HASH_ALGO_جوڙ
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
          اکر_لکو (*file);
          اكسر;
        }
      file++;
    }
}
#جيڪڏهن_پورو

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
digest_file (حرف ثابت *filename, صحيح *ٻٽو, بنانشان حرف *bin_نتيجو,
             bool *missing, MAYBE_اڻ_استعمال_ٿيل intmax_t *ڊيگهائي)
{
  FILE *fp;
  صحيح err;
  bool is_stdin = streq (filename, "-");

  *missing = false;

  إذا (is_stdin)
    {
      have_read_stdin = true;
      fp = stdin;
      إذا (O_ٻٽو && *ٻٽو)
        {
          إذا (*ٻٽو < 0)
            *ٻٽو = ! isatty (STDIN_FILENO);
          إذا (*ٻٽو)
            xset_ٻٽو_گهڻائي (STDIN_FILENO, O_ٻٽو);
        }
    }
  وإلا
    {
      fp = fopen (filename, O_ٻٽو ? (*ٻٽو ? "rb" : "rt") : "r");
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
    *ڊيگهائي = digest_ڊيگهائي >> 3;
  err = DIGEST_وهڪرو (fp, bin_نتيجو, ڊيگهائي);
#وإلاإذا HASH_ALGO_جوڙ
  err = DIGEST_وهڪرو (fp, bin_نتيجو, ڊيگهائي);
#وإلاإذا HASH_ALGO_BLAKE2
  err = DIGEST_وهڪرو (fp, bin_نتيجو, digest_ڊيگهائي >> 3);
#وإلا
  err = DIGEST_وهڪرو (fp, bin_نتيجو);
#جيڪڏهن_پورو
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

#إذا !HASH_ALGO_جوڙ
ساكن فراغ
output_file (حرف ثابت *file, صحيح ٻٽو_file, فراغ ثابت *digest,
             MAYBE_اڻ_استعمال_ٿيل bool raw, bool tagged, بنانشان حرف delim,
             MAYBE_اڻ_استعمال_ٿيل bool دليلون, MAYBE_اڻ_استعمال_ٿيل intmax_t ڊيگهائي)
{
# إذا HASH_ALGO_CKSUM
  إذا (raw)
    {
      fwrite (digest, 1, digest_ڊيگهائي >> 3, stdout);
      أرجع;
    }
# endif

  بنانشان حرف ثابت *bin_بفر = digest;

  /* Output a leading backslash if the file name contains problematic chars.  */
  bool needs_escape = delim == '\n' && problematic_chars (file);

  إذا (needs_escape)
    اکر_لکو ('\\');

  إذا (tagged)
    {
# إذا HASH_ALGO_CKSUM
      إذا (cksum_algorithm == sha2)
        لکو ("SHA%u", (بنانشان صحيح) digest_ڊيگهائي);
      وإلا
# endif
      fputs (DIGEST_TYPE_STRING, stdout);
# إذا HASH_ALGO_BLAKE2
      إذا (digest_ڊيگهائي < DIGEST_MAX_LEN * 8)
        لکو ("-%u", (بنانشان صحيح) digest_ڊيگهائي);
# elif HASH_ALGO_CKSUM
      إذا (cksum_algorithm == sha3)
        لکو ("-%u", (بنانشان صحيح) digest_ڊيگهائي);
      إذا (cksum_algorithm == blake2b)
        {
          إذا (digest_ڊيگهائي < DIGEST_MAX_LEN * 8)
            لکو ("-%u", (بنانشان صحيح) digest_ڊيگهائي);
        }
# endif
      fputs (" (", stdout);
      print_filename (file, needs_escape);
      fputs (") = ", stdout);
    }

# إذا HASH_ALGO_CKSUM
  إذا (base64_digest)
    {
      حرف b64[BASE64_ڊيگهائي (DIGEST_BIN_بائيٽون) + 1];
      base64_encode ((حرف ثابت *) bin_بفر, digest_ڊيگهائي >> 3,
                     b64, حجم b64);
      fputs (b64, stdout);
    }
  وإلا
# endif
    {
      لكل (idx_t i = 0; i < (digest_hex_بائيٽون >> 1); ++i)
        لکو ("%02x", bin_بفر[i]);
    }

  إذا (!tagged)
    {
      اکر_لکو (' ');
      اکر_لکو (ٻٽو_file ? '*' : ' ');
      print_filename (file, needs_escape);
    }

  اکر_لکو (delim);
}
#جيڪڏهن_پورو

#إذا HASH_ALGO_CKSUM
/* Return true if B64_DIGEST is the same as the base64 digest of the
   DIGEST_LENGTH/8 bytes at BIN_BUFFER.  */
ساكن bool
b64_equal (بنانشان حرف ثابت *b64_digest, بنانشان حرف ثابت *bin_بفر)
{
  idx_t b64_n_بائيٽون = BASE64_ڊيگهائي (digest_ڊيگهائي >> 3);
  حرف b64[BASE64_ڊيگهائي (DIGEST_BIN_بائيٽون) + 1];
  base64_encode ((حرف ثابت *) bin_بفر, digest_ڊيگهائي >> 3,
                 b64, حجم b64);
  أرجع memeq (b64_digest, b64, b64_n_بائيٽون + 1);
}
#جيڪڏهن_پورو

/* Return true if HEX_DIGEST is the same as the hex-encoded digest of the
   DIGEST_LENGTH/8 bytes at BIN_BUFFER.  */
ساكن bool
hex_equal (بنانشان حرف ثابت *hex_digest, بنانشان حرف ثابت *bin_بفر)
{
  ساكن ثابت حرف bin2hex[] = { '0', '1', '2', '3',
                                  '4', '5', '6', '7',
                                  '8', '9', 'a', 'b',
                                  'c', 'd', 'e', 'f' };
  idx_t digest_bin_بائيٽون = digest_hex_بائيٽون >> 1;

  /* Compare generated binary number with text representation
     in check file.  Ignore case of hex digits.  */
  idx_t cnt;
  لكل (cnt = 0; cnt < digest_bin_بائيٽون; ++cnt)
    {
      إذا (c_tolower (hex_digest[2 * cnt])
          != bin2hex[bin_بفر[cnt] >> 4]
          || (c_tolower (hex_digest[2 * cnt + 1])
              != (bin2hex[bin_بفر[cnt] & 0xf])))
        اكسر;
    }
  أرجع cnt == digest_bin_بائيٽون;
}

ساكن bool
digest_check (حرف ثابت *checkfile_name)
{
  FILE *checkfile_وهڪرو;
  intmax_t n_misformatted_سٽون = 0;
  intmax_t n_mismatched_checksums = 0;
  intmax_t n_open_or_read_failures = 0;
  bool properly_formatted_سٽون = false;
  bool matched_checksums = false;
  بنانشان حرف bin_بفر_unaligned[DIGEST_BIN_بائيٽون + DIGEST_ALIGN];
  /* Make sure bin_buffer is properly aligned. */
  بنانشان حرف *bin_بفر = ptr_align (bin_بفر_unaligned, DIGEST_ALIGN);
  intmax_t سٽ_عدد;
  حرف *سٽ;
  size_t سٽ_chars_allocated;
  bool is_stdin = streq (checkfile_name, "-");

  إذا (is_stdin)
    {
      have_read_stdin = true;
      checkfile_name = _("standard input");
      checkfile_وهڪرو = stdin;
    }
  وإلا
    {
      checkfile_وهڪرو = fopen (checkfile_name, "r");
      إذا (checkfile_وهڪرو == عدم)
        {
          error (0, errno, "%s", quotef (checkfile_name));
          أرجع false;
        }
    }

  سٽ_عدد = 0;
  سٽ = عدم;
  سٽ_chars_allocated = 0;
  افعل
    {
      حرف *filename;
      صحيح ٻٽو;
      بنانشان حرف *digest;
      ssize_t سٽ_ڊيگهائي;

      ++سٽ_عدد;
      إذا (سٽ_عدد == 0)
        error (EXIT_FAILURE, 0, _("%s: too many checksum lines"),
               quotef (checkfile_name));

      سٽ_ڊيگهائي = getline (&سٽ, &سٽ_chars_allocated, checkfile_وهڪرو);
      إذا (سٽ_ڊيگهائي <= 0)
        اكسر;

      /* Ignore comment lines, which begin with a '#' character.  */
      إذا (سٽ[0] == '#')
        استمر;

      /* Remove any trailing newline.  */
      سٽ_ڊيگهائي -= سٽ[سٽ_ڊيگهائي - 1] == '\n';
      /* Remove any trailing carriage return.  */
      سٽ_ڊيگهائي -= سٽ[سٽ_ڊيگهائي - (0 < سٽ_ڊيگهائي)] == '\r';

      /* Ignore empty lines.  */
      إذا (سٽ_ڊيگهائي == 0)
        استمر;

      سٽ[سٽ_ڊيگهائي] = '\0';

      idx_t d_len;
      إذا (! (ورهايو_3 (سٽ, سٽ_ڊيگهائي, &digest, &d_len, &ٻٽو, &filename)
             && ! (is_stdin && streq (filename, "-"))))
        {
          ++n_misformatted_سٽون;

          إذا (warn)
            {
              error (0, 0,
                     _("%s: %jd"
                       ": improperly formatted %s checksum line"),
                     quotef (checkfile_name), سٽ_عدد,
                     DIGEST_TYPE_STRING);
            }
        }
      وإلا
        {
          bool ok;
          bool missing;

          properly_formatted_سٽون = true;

          intmax_t ڊيگهائي;
          ok = digest_file (filename, &ٻٽو, bin_بفر, &missing, &ڊيگهائي);

          إذا (!ok)
            {
              ++n_open_or_read_failures;
              إذا (!حالت_only)
                لکو ("%s: %s\n", quotef (filename),_("FAILED open or read"));
            }
          وإلا إذا (ignore_missing && missing)
            {
              /* Ignore missing files with --ignore-missing.  */
              ;
            }
          وإلا
            {
              bool ميلاپ = false;
#إذا HASH_ALGO_CKSUM
              إذا (d_len == BASE64_ڊيگهائي (digest_ڊيگهائي >> 3))
                ميلاپ = b64_equal (digest, bin_بفر);
              وإلا
#جيڪڏهن_پورو
                إذا (d_len == digest_hex_بائيٽون)
                  ميلاپ = hex_equal (digest, bin_بفر);

              إذا (ميلاپ)
                matched_checksums = true;
              وإلا
                ++n_mismatched_checksums;

              إذا (!حالت_only)
                {
                  إذا (! ميلاپ || ! quiet)
                    fputs (quotef (filename), stdout);

                  إذا (! ميلاپ)
                    لکو (": %s\n", _("FAILED"));
                  وإلا إذا (!quiet)
                    لکو (": %s\n", _("OK"));
                }
            }

          إذا (ferror (stdout))
            write_error ();
        }
    }
  طالما (!feof (checkfile_وهڪرو) && !ferror (checkfile_وهڪرو));

  حرر (سٽ);

  صحيح err = ferror (checkfile_وهڪرو) ? 0 : -1;
  إذا (is_stdin)
    clearerr (checkfile_وهڪرو);
  وإلا إذا (fclose (checkfile_وهڪرو) != 0 && err < 0)
    err = errno;

  إذا (0 <= err)
    {
      error (0, err, err ? "%s" : _("%s: read error"),
             quotef (checkfile_name));
      أرجع false;
    }

  إذا (! properly_formatted_سٽون)
    {
      /* Warn if no tests are found.  */
      error (0, 0, _("%s: no properly formatted checksum lines found"),
             quotef (checkfile_name));
    }
  وإلا
    {
      إذا (!حالت_only)
        {
          إذا (n_misformatted_سٽون != 0)
            error (0, 0,
                   (ngettext
                    ("WARNING: %jd line is improperly formatted",
                     "WARNING: %jd lines are improperly formatted",
                     select_plural (n_misformatted_سٽون))),
                   n_misformatted_سٽون);

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

  أرجع (properly_formatted_سٽون
          && matched_checksums
          && n_mismatched_checksums == 0
          && n_open_or_read_failures == 0
          && (!strict || n_misformatted_سٽون == 0));
}

صحيح
مکيه (صحيح argc, حرف **argv)
{
  بنانشان حرف bin_بفر_unaligned[DIGEST_BIN_بائيٽون + DIGEST_ALIGN];
  /* Make sure bin_buffer is properly aligned. */
  بنانشان حرف *bin_بفر = ptr_align (bin_بفر_unaligned, DIGEST_ALIGN);
  bool do_check = false;
  صحيح opt;
  bool ok = true;
  صحيح ٻٽو = -1;
  صحيح اڳياڙي_ٽيگ = -1;
  بنية option ثابت *long_opts = long_options;

#إذا HASH_ALGO_CKSUM
  إذا (cksum_algorithm != چڪري_جاچ)
    {
      legacy_گهڻائي = true;
      اڳياڙي_ٽيگ = 0;
      algorithm_specified = true;
      long_opts = legacy_long_options;
    }
#جيڪڏهن_پورو

  /* Setting values of global variables.  */
  initialize_main (&argc, &argv);
  سيٽ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* Line buffer stdout to ensure lines are written atomically and immediately
     so that processes running in parallel do not intersperse their output.  */
  setvbuf (stdout, عدم, _IOLBF, 0);

#إذا HASH_ALGO_جوڙ
  حرف ثابت *short_opts = "rs";
#وإلاإذا HASH_ALGO_CKSUM
  حرف ثابت *short_opts = legacy_گهڻائي ? "bctwz" : "a:l:bctwz";
  حرف ثابت *digest_ڊيگهائي_str = "";
#وإلاإذا HASH_ALGO_BLAKE2
  حرف ثابت *short_opts = "l:bctwz";
  حرف ثابت *digest_ڊيگهائي_str = "";
#وإلا
  حرف ثابت *short_opts = "bctwz";
#جيڪڏهن_پورو

  طالما ((opt = getopt_long (argc, argv, short_opts, long_opts, عدم))
         != -1)
    بدل (opt)
      {
#إذا HASH_ALGO_CKSUM
      حالة 'a':
        cksum_algorithm = XARGMATCH_EXACT ("--algorithm", optarg,
                                           algorithm_دليلون, algorithm_types);
        algorithm_specified = true;
        اكسر;

      حالة DEBUG_پروگرام_OPTION:
        cksum_debug = true;
        اكسر;
#جيڪڏهن_پورو
#إذا HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
      حالة 'l':
        digest_ڊيگهائي = xnumtoimax (optarg, 10, 0, IDX_MAX, "",
                                    _("invalid length"), 0,
                                    XTOINT_MAX_QUIET);
        digest_ڊيگهائي_str = optarg;
        اكسر;
#جيڪڏهن_پورو
#إذا !HASH_ALGO_جوڙ
      حالة 'c':
        do_check = true;
        اكسر;
      حالة حالت_OPTION:
        حالت_only = true;
        warn = false;
        quiet = false;
        اكسر;
      حالة 'b':
        ٻٽو = 1;
        اكسر;
      حالة 't':
        ٻٽو = 0;
        اكسر;
      حالة 'w':
        حالت_only = false;
        warn = true;
        quiet = false;
        اكسر;
      حالة IGNORE_MISSING_OPTION:
        ignore_missing = true;
        اكسر;
      حالة QUIET_OPTION:
        حالت_only = false;
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
        اڳياڙي_ٽيگ = 0;
        اكسر;
# endif
      حالة ٽيگ_OPTION:
        اڳياڙي_ٽيگ = 1;
        اكسر;
      حالة 'z':
        digest_delim = '\0';
        اكسر;
#جيڪڏهن_پورو
#إذا HASH_ALGO_جوڙ
      حالة 'r':		/* For SysV compatibility. */
        جوڙ_algorithm = bsd;
        اكسر;

      حالة 's':
        جوڙ_algorithm = sysv;
        اكسر;
#جيڪڏهن_پورو
      case_GETOPT_HELP_CHAR;
      case_GETOPT_نسخو_CHAR (پروگرام_NAME, AUTHORS);
      افتراضي:
        usage (EXIT_FAILURE);
      }

  min_digest_سٽ_ڊيگهائي = MIN_DIGEST_سٽ_ڊيگهائي;
#إذا HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# إذا HASH_ALGO_CKSUM
  إذا (digest_ڊيگهائي && (cksum_algorithm != blake2b
                        && cksum_algorithm != sha2
                        && cksum_algorithm != sha3))
    error (EXIT_FAILURE, 0,
           _("--length is only supported with --algorithm "
             "blake2b, sha2, or sha3"));
  إذا (cksum_algorithm == sha2 || cksum_algorithm == sha3)
    {
      /* Do not require --length with --check.  */
      إذا (digest_ڊيگهائي == 0 && *digest_ڊيگهائي_str == '\0' && ! do_check)
        error (EXIT_FAILURE, 0, _("--algorithm=%s requires specifying "
                                  "--length 224, 256, 384, or 512"),
               algorithm_دليلون[cksum_algorithm]);
      /* If --check and --length are used we verify the digest length.  */
      إذا ((! do_check || *digest_ڊيگهائي_str != '\0')
          && digest_ڊيگهائي != SHA224_DIGEST_سائيز * 8
          && digest_ڊيگهائي != SHA256_DIGEST_سائيز * 8
          && digest_ڊيگهائي != SHA384_DIGEST_سائيز * 8
          && digest_ڊيگهائي != SHA512_DIGEST_سائيز * 8)
        {
          error (0, 0, _("invalid length: %s"), quote (digest_ڊيگهائي_str));
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
      إذا (digest_ڊيگهائي > DIGEST_MAX_LEN * 8)
        {
          error (0, 0, _("invalid length: %s"), quote (digest_ڊيگهائي_str));
          error (EXIT_FAILURE, 0,
                 _("maximum digest length for %s is %d bits"),
                 quote (DIGEST_TYPE_STRING),
                 DIGEST_MAX_LEN * 8);
        }
      إذا (digest_ڊيگهائي % 8 != 0)
        {
          error (0, 0, _("invalid length: %s"), quote (digest_ڊيگهائي_str));
          error (EXIT_FAILURE, 0, _("length is not a multiple of 8"));
        }
    }
  إذا (digest_ڊيگهائي == 0)
    {
# إذا HASH_ALGO_BLAKE2
      digest_ڊيگهائي = DIGEST_MAX_LEN * 8;
# وإلا
      digest_ڊيگهائي = algorithm_بٽ[cksum_algorithm];
# endif
    }
  digest_hex_بائيٽون = digest_ڊيگهائي >> 2;
#وإلا
  digest_hex_بائيٽون = DIGEST_HEX_بائيٽون;
#جيڪڏهن_پورو

#إذا HASH_ALGO_CKSUM
  بدل (+cksum_algorithm)
    {
    حالة bsd:
    حالة sysv:
    حالة چڪري_جاچ:
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
#جيڪڏهن_پورو

  إذا (digest_delim != '\n' && do_check)
    {
      error (0, 0, _("the --zero option is not supported when "
                     "verifying checksums"));
      usage (EXIT_FAILURE);
    }
  إذا (1 <= اڳياڙي_ٽيگ && do_check)
    {
      /* Note we allow --untagged with --check to more
         seamlessly support --untagged in an emulation wrapper.  */
      error (0, 0, _("the --tag option is meaningless when "
                     "verifying checksums"));
      usage (EXIT_FAILURE);
    }

  إذا (0 <= ٻٽو && do_check)
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

  إذا (حالت_only && !do_check)
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

  إذا (اڳياڙي_ٽيگ == -1)
    اڳياڙي_ٽيگ = HASH_ALGO_CKSUM;

  إذا (اڳياڙي_ٽيگ && !ٻٽو)
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
#جيڪڏهن_پورو
     usage (EXIT_FAILURE);
   }

  إذا (!O_ٻٽو && ٻٽو < 0)
    ٻٽو = 0;
  وإلا إذا (اڳياڙي_ٽيگ)
    ٻٽو = 1;

  حرف **عمليو_lim = argv + argc;
  إذا (optind == argc)
    *عمليو_lim++ = bad_cast ("-");
  وإلا إذا (1 < argc - optind && raw_digest)
    error (EXIT_FAILURE, 0,
           _("the --raw option is not supported with multiple files"));

  لكل (حرف **operandp = argv + optind; operandp < عمليو_lim; operandp++)
    {
      حرف *file = *operandp;
      إذا (do_check)
        ok &= digest_check (file);
      وإلا
        {
          صحيح ٻٽو_file = ٻٽو;
          bool missing;
          intmax_t ڊيگهائي;

          إذا (! digest_file (file, &ٻٽو_file, bin_بفر, &missing, &ڊيگهائي))
            ok = false;
          وإلا
            {
              DIGEST_ٻاهريون (file, ٻٽو_file, bin_بفر, raw_digest, اڳياڙي_ٽيگ,
                          digest_delim, optind != argc, ڊيگهائي);
              إذا (ferror (stdout))
                write_error ();
            }
        }
    }

  إذا (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, _("standard input"));

  أرجع ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
