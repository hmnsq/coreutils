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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "assure.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "argmatch.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "c-ctype.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xdectoint.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xstrtol.h"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HASH_ALGO_CKSUM
# define HASH_ALGO_CKSUM 0
#ਜੇ_ਅੰਤ

#ਜੇ HASH_ALGO_ਜੋੜ || HASH_ALGO_CKSUM
# include "sum.h"
#ਜੇ_ਅੰਤ
#ਜੇ HASH_ALGO_CKSUM
# include "cksum.h"
# include "cksum_crc.h"
# include "base64.h"
#ਜੇ_ਅੰਤ
#ਜੇ HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# include "blake2/b2sum.h"
#ਜੇ_ਅੰਤ
#ਜੇ HASH_ALGO_MD5 || HASH_ALGO_CKSUM
# include "md5.h"
#ਜੇ_ਅੰਤ
#ਜੇ HASH_ALGO_SHA1 || HASH_ALGO_CKSUM
# include "sha1.h"
#ਜੇ_ਅੰਤ
#ਜੇ HASH_ALGO_SHA256 || HASH_ALGO_SHA224 || HASH_ALGO_CKSUM
# include "sha256.h"
#ਜੇ_ਅੰਤ
#ਜੇ HASH_ALGO_SHA512 || HASH_ALGO_SHA384 || HASH_ALGO_CKSUM
# include "sha512.h"
#ਜੇ_ਅੰਤ
#ਜੇ HASH_ALGO_CKSUM
# include "sha3.h"
#ਜੇ_ਅੰਤ
#ਜੇ HASH_ALGO_CKSUM
# include "sm3.h"
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ "fadvise.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "stdio--.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xbinary-io.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਜੇ HASH_ALGO_ਜੋੜ
# define ਪ੍ਰੋਗਰਾਮ_NAME "sum"
# define DIGEST_TYPE_STRING "BSD"
# define DIGEST_ਧਾਰਾ sumfns[ਜੋੜ_algorithm]
# define DIGEST_ਬਾਹਰਲਾ ਜੋੜ_output_fns[ਜੋੜ_algorithm]
# define DIGEST_ਬਿੱਟ 16
# define DIGEST_ALIGN 4
#ਜਾਂਜੇ HASH_ALGO_CKSUM
# define MAX_DIGEST_ਬਿੱਟ 512
# define MAX_DIGEST_ALIGN 8
# define DIGEST_TYPE_STRING algorithm_ਟੈਗ_ਸੂਚੀ[cksum_algorithm]
# define DIGEST_ਧਾਰਾ cksumfns[cksum_algorithm]
# define DIGEST_ਬਾਹਰਲਾ cksum_output_fns[cksum_algorithm]
# define DIGEST_ਬਿੱਟ MAX_DIGEST_ਬਿੱਟ
# define DIGEST_ALIGN MAX_DIGEST_ALIGN
#ਜਾਂਜੇ HASH_ALGO_MD5
# define ਪ੍ਰੋਗਰਾਮ_NAME "md5sum"
# define DIGEST_TYPE_STRING "MD5"
# define DIGEST_ਧਾਰਾ md5_ਧਾਰਾ
# define DIGEST_ਬਿੱਟ 128
# define DIGEST_REFERENCE "RFC 1321"
# define DIGEST_ALIGN 4
#ਜਾਂਜੇ HASH_ALGO_BLAKE2
# define ਪ੍ਰੋਗਰਾਮ_NAME "b2sum"
# define DIGEST_TYPE_STRING "BLAKE2b"
# define DIGEST_ਧਾਰਾ blake2b_ਧਾਰਾ
# define DIGEST_ਬਿੱਟ 512
# define DIGEST_REFERENCE "RFC 7693"
# define DIGEST_ALIGN 8
#ਜਾਂਜੇ HASH_ALGO_SHA1
# define ਪ੍ਰੋਗਰਾਮ_NAME "sha1sum"
# define DIGEST_TYPE_STRING "SHA1"
# define DIGEST_ਧਾਰਾ sha1_ਧਾਰਾ
# define DIGEST_ਬਿੱਟ 160
# define DIGEST_REFERENCE "FIPS-180-1"
# define DIGEST_ALIGN 4
#ਜਾਂਜੇ HASH_ALGO_SHA256
# define ਪ੍ਰੋਗਰਾਮ_NAME "sha256sum"
# define DIGEST_TYPE_STRING "SHA256"
# define DIGEST_ਧਾਰਾ sha256_ਧਾਰਾ
# define DIGEST_ਬਿੱਟ 256
# define DIGEST_REFERENCE "FIPS-180-2"
# define DIGEST_ALIGN 4
#ਜਾਂਜੇ HASH_ALGO_SHA224
# define ਪ੍ਰੋਗਰਾਮ_NAME "sha224sum"
# define DIGEST_TYPE_STRING "SHA224"
# define DIGEST_ਧਾਰਾ sha224_ਧਾਰਾ
# define DIGEST_ਬਿੱਟ 224
# define DIGEST_REFERENCE "RFC 3874"
# define DIGEST_ALIGN 4
#ਜਾਂਜੇ HASH_ALGO_SHA512
# define ਪ੍ਰੋਗਰਾਮ_NAME "sha512sum"
# define DIGEST_TYPE_STRING "SHA512"
# define DIGEST_ਧਾਰਾ sha512_ਧਾਰਾ
# define DIGEST_ਬਿੱਟ 512
# define DIGEST_REFERENCE "FIPS-180-2"
# define DIGEST_ALIGN 8
#ਜਾਂਜੇ HASH_ALGO_SHA384
# define ਪ੍ਰੋਗਰਾਮ_NAME "sha384sum"
# define DIGEST_TYPE_STRING "SHA384"
# define DIGEST_ਧਾਰਾ sha384_ਧਾਰਾ
# define DIGEST_ਬਿੱਟ 384
# define DIGEST_REFERENCE "FIPS-180-2"
# define DIGEST_ALIGN 8
#ਨਹੀਂਤਾਂ
# error "Can't decide which hash algorithm to compile."
#ਜੇ_ਅੰਤ
#ਜੇ !HASH_ALGO_ਜੋੜ && !HASH_ALGO_CKSUM
# define DIGEST_ਬਾਹਰਲਾ output_file
#ਜੇ_ਅੰਤ

#ਜੇ HASH_ALGO_CKSUM
# define ਪ੍ਰੋਗਰਾਮ_NAME (!legacy_ਬਹੁਲਕ ? "cksum"			 \
                       : cksum_algorithm == md5 ? "md5sum"	 \
                       : cksum_algorithm == sha1 ? "sha1sum"	 \
                       : cksum_algorithm == sha224 ? "sha224sum" \
                       : cksum_algorithm == sha256 ? "sha256sum" \
                       : cksum_algorithm == sha384 ? "sha384sum" \
                       : cksum_algorithm == sha512 ? "sha512sum" \
                       : "cksum")
#ਜੇ_ਅੰਤ

#ਜੇ HASH_ALGO_ਜੋੜ
# define AUTHORS \
  proper_name ("Kayvan Aghaiepour"), \
  proper_name ("David MacKenzie")
#ਜਾਂਜੇ HASH_ALGO_CKSUM
# define AUTHORS \
  proper_name_lite ("Padraig Brady", "P\303\241draig Brady"), \
  proper_name ("Q. Frank Xia")
#ਜਾਂਜੇ HASH_ALGO_BLAKE2
# define AUTHORS \
  proper_name_lite ("Padraig Brady", "P\303\241draig Brady"), \
  proper_name ("Samuel Neves")
#ਨਹੀਂਤਾਂ
# define AUTHORS \
  proper_name ("Ulrich Drepper"), \
  proper_name ("Scott Miller"), \
  proper_name ("David Madore")
#ਜੇ_ਅੰਤ
#ਜੇ !HASH_ALGO_BLAKE2 && !HASH_ALGO_CKSUM
# define DIGEST_HEX_ਬਾਈਟਾਂ (DIGEST_ਬਿੱਟ / 4)
#ਜੇ_ਅੰਤ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DIGEST_BIN_ਬਾਈਟਾਂ (DIGEST_ਬਿੱਟ / 8)

/* The minimum length of a valid digest line.  This length does
   not include any newline character at the end of a line.  */
#ਜੇ HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# define MIN_DIGEST_ਸਤਰ_ਲੰਮਾਈ 3 /* With -l 8.  */
#ਨਹੀਂਤਾਂ
# define MIN_DIGEST_ਸਤਰ_ਲੰਮਾਈ \
   (DIGEST_HEX_ਬਾਈਟਾਂ /* length of hexadecimal message digest */ \
    + 1 /* blank */ \
    + 1 /* minimum filename length */ )
#ਜੇ_ਅੰਤ

#ਜੇ !HASH_ALGO_ਜੋੜ
ਸਥਿਰ ਖਾਲੀ
output_file (ਅੱਖਰ ਅਚਲ *file, ਪੂਰਨ ਦੁਆਧਾਰੀ_file, ਖਾਲੀ ਅਚਲ *digest,
             bool raw, bool tagged, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ delim, bool ਦਲੀਲਾਂ,
             intmax_t ਲੰਮਾਈ);
#ਜੇ_ਅੰਤ

/* True if any of the files read were the standard input. */
ਸਥਿਰ bool have_read_stdin;

/* The minimum length of a valid checksum line for the selected algorithm.  */
ਸਥਿਰ idx_t min_digest_ਸਤਰ_ਲੰਮਾਈ;

/* Set to the length of a digest hex string for the selected algorithm.  */
ਸਥਿਰ idx_t digest_hex_ਬਾਈਟਾਂ;

/* With --check, don't generate any output.
   The exit code indicates success or failure.  */
ਸਥਿਰ bool ਸਥਿਤੀ_only = false;

/* With --check, print a message to standard error warning about each
   improperly formatted checksum line.  */
ਸਥਿਰ bool warn = false;

/* With --check, ignore missing files.  */
ਸਥਿਰ bool ignore_missing = false;

/* With --check, suppress the "OK" printed for each verified file.  */
ਸਥਿਰ bool quiet = false;

/* With --check, exit with a non-zero return code if any line is
   improperly formatted. */
ਸਥਿਰ bool strict = false;

/* Whether a BSD reversed format checksum is detected.  */
ਸਥਿਰ ਪੂਰਨ bsd_reversed = -1;

/* line delimiter.  */
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ digest_delim = '\n';

#ਜੇ HASH_ALGO_CKSUM
/* If true, print base64-encoded digests, not hex.  */
ਸਥਿਰ bool base64_digest = false;
#ਜੇ_ਅੰਤ

/* If true, print binary digests, not hex.  */
ਸਥਿਰ bool raw_digest = false;

/* blake2b and sha3 allow the -l option.  Luckily they both have the same
   maximum digest size.  */
#ਜੇ HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# ਜੇ HASH_ALGO_CKSUM
static_assert (BLAKE2B_OUTBYTES == SHA3_512_DIGEST_ਮਾਪ);
# endif
# define DIGEST_MAX_LEN BLAKE2B_OUTBYTES
ਸਥਿਰ idx_t digest_ਲੰਮਾਈ;
#ਜੇ_ਅੰਤ /* HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM */

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*digest_output_fn)(ਅੱਖਰ ਅਚਲ *, ਪੂਰਨ, ਖਾਲੀ ਅਚਲ *, bool,
                                 bool, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ, bool, intmax_t);
#ਜੇ HASH_ALGO_ਜੋੜ
ਗਿਣਤੀ Algorithm
{
  bsd,
  sysv,
};

ਸਥਿਰ ਗਿਣਤੀ Algorithm ਜੋੜ_algorithm;
ਸਥਿਰ sumfn sumfns[]=
{
  bsd_ਜੋੜ_ਧਾਰਾ,
  sysv_ਜੋੜ_ਧਾਰਾ,
};
ਸਥਿਰ digest_output_fn ਜੋੜ_output_fns[]=
{
  output_bsd,
  output_sysv,
};
#ਜੇ_ਅੰਤ

#ਜੇ HASH_ALGO_CKSUM
ਸਥਿਰ ਪੂਰਨ
md5_ਜੋੜ_ਧਾਰਾ (FILE *ਧਾਰਾ, ਖਾਲੀ *resstream, MAYBE_ਅਣਵਰਤਿਆ intmax_t *ਲੰਮਾਈ)
{
  ਵਾਪਸ md5_ਧਾਰਾ (ਧਾਰਾ, resstream);
}
ਸਥਿਰ ਪੂਰਨ
sha1_ਜੋੜ_ਧਾਰਾ (FILE *ਧਾਰਾ, ਖਾਲੀ *resstream, MAYBE_ਅਣਵਰਤਿਆ intmax_t *ਲੰਮਾਈ)
{
  ਵਾਪਸ sha1_ਧਾਰਾ (ਧਾਰਾ, resstream);
}
ਸਥਿਰ ਪੂਰਨ
sha224_ਜੋੜ_ਧਾਰਾ (FILE *ਧਾਰਾ, ਖਾਲੀ *resstream,
                   MAYBE_ਅਣਵਰਤਿਆ intmax_t *ਲੰਮਾਈ)
{
  ਵਾਪਸ sha224_ਧਾਰਾ (ਧਾਰਾ, resstream);
}
ਸਥਿਰ ਪੂਰਨ
sha256_ਜੋੜ_ਧਾਰਾ (FILE *ਧਾਰਾ, ਖਾਲੀ *resstream,
                   MAYBE_ਅਣਵਰਤਿਆ intmax_t *ਲੰਮਾਈ)
{
  ਵਾਪਸ sha256_ਧਾਰਾ (ਧਾਰਾ, resstream);
}
ਸਥਿਰ ਪੂਰਨ
sha384_ਜੋੜ_ਧਾਰਾ (FILE *ਧਾਰਾ, ਖਾਲੀ *resstream,
                   MAYBE_ਅਣਵਰਤਿਆ intmax_t *ਲੰਮਾਈ)
{
  ਵਾਪਸ sha384_ਧਾਰਾ (ਧਾਰਾ, resstream);
}
ਸਥਿਰ ਪੂਰਨ
sha512_ਜੋੜ_ਧਾਰਾ (FILE *ਧਾਰਾ, ਖਾਲੀ *resstream,
                   MAYBE_ਅਣਵਰਤਿਆ intmax_t *ਲੰਮਾਈ)
{
  ਵਾਪਸ sha512_ਧਾਰਾ (ਧਾਰਾ, resstream);
}
ਸਥਿਰ ਪੂਰਨ
sha2_ਜੋੜ_ਧਾਰਾ (FILE *ਧਾਰਾ, ਖਾਲੀ *resstream, intmax_t *ਲੰਮਾਈ)
{
  ਚੋਣ (*ਲੰਮਾਈ)
    {
    ਹਾਲਤ SHA224_DIGEST_ਮਾਪ:
      ਵਾਪਸ sha224_ਧਾਰਾ (ਧਾਰਾ, resstream);
    ਹਾਲਤ SHA256_DIGEST_ਮਾਪ:
      ਵਾਪਸ sha256_ਧਾਰਾ (ਧਾਰਾ, resstream);
    ਹਾਲਤ SHA384_DIGEST_ਮਾਪ:
      ਵਾਪਸ sha384_ਧਾਰਾ (ਧਾਰਾ, resstream);
    ਹਾਲਤ SHA512_DIGEST_ਮਾਪ:
      ਵਾਪਸ sha512_ਧਾਰਾ (ਧਾਰਾ, resstream);
    ਮੂਲ:
      affirm (false);
    }
}
ਸਥਿਰ ਪੂਰਨ
sha3_ਜੋੜ_ਧਾਰਾ (FILE *ਧਾਰਾ, ਖਾਲੀ *resstream, intmax_t *ਲੰਮਾਈ)
{
  ਚੋਣ (*ਲੰਮਾਈ)
    {
    ਹਾਲਤ SHA3_224_DIGEST_ਮਾਪ:
      ਵਾਪਸ sha3_224_ਧਾਰਾ (ਧਾਰਾ, resstream);
    ਹਾਲਤ SHA3_256_DIGEST_ਮਾਪ:
      ਵਾਪਸ sha3_256_ਧਾਰਾ (ਧਾਰਾ, resstream);
    ਹਾਲਤ SHA3_384_DIGEST_ਮਾਪ:
      ਵਾਪਸ sha3_384_ਧਾਰਾ (ਧਾਰਾ, resstream);
    ਹਾਲਤ SHA3_512_DIGEST_ਮਾਪ:
      ਵਾਪਸ sha3_512_ਧਾਰਾ (ਧਾਰਾ, resstream);
    ਮੂਲ:
      affirm (false);
    }
}
ਸਥਿਰ ਪੂਰਨ
blake2b_ਜੋੜ_ਧਾਰਾ (FILE *ਧਾਰਾ, ਖਾਲੀ *resstream, intmax_t *ਲੰਮਾਈ)
{
  ਵਾਪਸ blake2b_ਧਾਰਾ (ਧਾਰਾ, resstream, *ਲੰਮਾਈ);
}
ਸਥਿਰ ਪੂਰਨ
sm3_ਜੋੜ_ਧਾਰਾ (FILE *ਧਾਰਾ, ਖਾਲੀ *resstream, MAYBE_ਅਣਵਰਤਿਆ intmax_t *ਲੰਮਾਈ)
{
  ਵਾਪਸ sm3_ਧਾਰਾ (ਧਾਰਾ, resstream);
}

ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ਅਚਲ algorithm_ਦਲੀਲਾਂ[] =
{
  "bsd", "sysv", "crc", "crc32b", "md5", "sha1",
  "sha224", "sha256", "sha384", "sha512", /* Legacy naming */
  "sha2", "sha3", "blake2b", "sm3", ਨਿਰਾ
};
ਸਥਿਰ ਗਿਣਤੀ Algorithm ਅਚਲ algorithm_types[] =
{
  bsd, sysv, ਚੱਕਰੀ_ਜਾਂਚ, crc32b, md5, sha1,
  sha224, sha256, sha384, sha512,
  sha2, sha3, blake2b, sm3,
};
ARGMATCH_VERIFY (algorithm_ਦਲੀਲਾਂ, algorithm_types);

ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ਅਚਲ algorithm_ਟੈਗ_ਸੂਚੀ[] =
{
  "BSD", "SYSV", "CRC", "CRC32B", "MD5", "SHA1",
  "SHA224", "SHA256", "SHA384", "SHA512",
  "SHA2", "SHA3", "BLAKE2b", "SM3", ਨਿਰਾ
};
ਸਥਿਰ ਪੂਰਨ ਅਚਲ algorithm_ਬਿੱਟ[] =
{
  16, 16, 32, 32, 128, 160,
  224, 256, 384, 512,
  512, 512, 512, 256, 0
};

static_assert (countof (algorithm_ਬਿੱਟ) == countof (algorithm_ਦਲੀਲਾਂ));

ਸਥਿਰ bool algorithm_specified = false;
ਸਥਿਰ bool legacy_ਬਹੁਲਕ = false;
ਗਿਣਤੀ Algorithm cksum_algorithm = ਚੱਕਰੀ_ਜਾਂਚ;

ਸਥਿਰ sumfn cksumfns[]=
{
  bsd_ਜੋੜ_ਧਾਰਾ,
  sysv_ਜੋੜ_ਧਾਰਾ,
  ਚੱਕਰੀ_ਜਾਂਚ_ਜੋੜ_ਧਾਰਾ,
  crc32b_ਜੋੜ_ਧਾਰਾ,
  md5_ਜੋੜ_ਧਾਰਾ,
  sha1_ਜੋੜ_ਧਾਰਾ,
  sha224_ਜੋੜ_ਧਾਰਾ,
  sha256_ਜੋੜ_ਧਾਰਾ,
  sha384_ਜੋੜ_ਧਾਰਾ,
  sha512_ਜੋੜ_ਧਾਰਾ,
  sha2_ਜੋੜ_ਧਾਰਾ,
  sha3_ਜੋੜ_ਧਾਰਾ,
  blake2b_ਜੋੜ_ਧਾਰਾ,
  sm3_ਜੋੜ_ਧਾਰਾ,
};
ਸਥਿਰ digest_output_fn cksum_output_fns[]=
{
  output_bsd,
  output_sysv,
  output_ਚੱਕਰੀ_ਜਾਂਚ,
  output_ਚੱਕਰੀ_ਜਾਂਚ,
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
#ਜੇ_ਅੰਤ

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */

ਗਿਣਤੀ
{
  IGNORE_MISSING_OPTION = CHAR_MAX + 1,
  ਸਥਿਤੀ_OPTION,
  QUIET_OPTION,
  STRICT_OPTION,
  ਟੈਗ_OPTION,
  UNTAG_OPTION,
  DEBUG_ਪ੍ਰੋਗਰਾਮ_OPTION,
  RAW_OPTION,
  BASE64_OPTION,
};

#ਜੇ HASH_ALGO_CKSUM
ਸਥਿਰ ਬਣਤਰ option ਅਚਲ legacy_long_options[] =
{
  { "check", no_ਦਲੀਲ, ਨਿਰਾ, 'c' },
  { "ignore-missing", no_ਦਲੀਲ, ਨਿਰਾ, IGNORE_MISSING_OPTION },
  { "quiet", no_ਦਲੀਲ, ਨਿਰਾ, QUIET_OPTION },
  { "status", no_ਦਲੀਲ, ਨਿਰਾ, ਸਥਿਤੀ_OPTION },
  { "warn", no_ਦਲੀਲ, ਨਿਰਾ, 'w' },
  { "strict", no_ਦਲੀਲ, ਨਿਰਾ, STRICT_OPTION },
  { "tag", no_ਦਲੀਲ, ਨਿਰਾ, ਟੈਗ_OPTION },
  { "zero", no_ਦਲੀਲ, ਨਿਰਾ, 'z' },

  { "binary", no_ਦਲੀਲ, ਨਿਰਾ, 'b' }, /* Deprecated.  */
  { "text", no_ਦਲੀਲ, ਨਿਰਾ, 't' }, /* Deprecated.  */

  { GETOPT_HELP_OPTION_DECL },
  { GETOPT_ਸੰਸਕਰਣ_OPTION_DECL },
  { ਨਿਰਾ, 0, ਨਿਰਾ, 0 }
};
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ long_options[] =
{
#ਜੇ HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
  { "length", required_ਦਲੀਲ, ਨਿਰਾ, 'l'},
#ਜੇ_ਅੰਤ

#ਜੇ !HASH_ALGO_ਜੋੜ
  { "check", no_ਦਲੀਲ, ਨਿਰਾ, 'c' },
  { "ignore-missing", no_ਦਲੀਲ, ਨਿਰਾ, IGNORE_MISSING_OPTION},
  { "quiet", no_ਦਲੀਲ, ਨਿਰਾ, QUIET_OPTION },
  { "status", no_ਦਲੀਲ, ਨਿਰਾ, ਸਥਿਤੀ_OPTION },
  { "warn", no_ਦਲੀਲ, ਨਿਰਾ, 'w' },
  { "strict", no_ਦਲੀਲ, ਨਿਰਾ, STRICT_OPTION },
  { "tag", no_ਦਲੀਲ, ਨਿਰਾ, ਟੈਗ_OPTION },
  { "zero", no_ਦਲੀਲ, ਨਿਰਾ, 'z' },

# ਜੇ HASH_ALGO_CKSUM
  { "algorithm", required_ਦਲੀਲ, ਨਿਰਾ, 'a'},
  { "base64", no_ਦਲੀਲ, ਨਿਰਾ, BASE64_OPTION },
  { "debug", no_ਦਲੀਲ, ਨਿਰਾ, DEBUG_ਪ੍ਰੋਗਰਾਮ_OPTION},
  { "raw", no_ਦਲੀਲ, ਨਿਰਾ, RAW_OPTION},
  { "untagged", no_ਦਲੀਲ, ਨਿਰਾ, UNTAG_OPTION },
# endif
  { "binary", no_ਦਲੀਲ, ਨਿਰਾ, 'b' }, /* Deprecated.  */
  { "text", no_ਦਲੀਲ, ਨਿਰਾ, 't' }, /* Deprecated.  */

#ਨਹੀਂਤਾਂ
  {"sysv", no_ਦਲੀਲ, ਨਿਰਾ, 's'},  /* Not in cksum.  */
#ਜੇ_ਅੰਤ

  { GETOPT_HELP_OPTION_DECL },
  { GETOPT_ਸੰਸਕਰਣ_OPTION_DECL },
  { ਨਿਰਾ, 0, ਨਿਰਾ, 0 }
};

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("\
Usage: %s [OPTION]... [FILE]...\n\
"), ਪ੍ਰੋਗਰਾਮ_name);
#ਜੇ HASH_ALGO_CKSUM
    ਜੇ (legacy_ਬਹੁਲਕ)
      {
        ਲਿਖੋ (_("\
Print or check %s checksums.\n\
"), DIGEST_TYPE_STRING);
        fputs (_("\
Legacy interface ਮੰਜ਼ਿਲ the cksum(1) utility.\n\
"), stdout);
      }
    ਨਹੀਂਤਾਂ
      fputs (_("\
Print or verify checksums.\n\
By ਮੂਲ use the 32 ਦੋਆਧਾਰੀ_ਅੰਕ ਚੱਕਰੀ_ਜਾਂਚ_2 algorithm.\n\
"), stdout);
#ਨਹੀਂਤਾਂ
      ਲਿਖੋ (_("\
Print or check %s (%d-ਦੋਆਧਾਰੀ_ਅੰਕ) checksums.\n\
"),
              DIGEST_TYPE_STRING,
              DIGEST_ਬਿੱਟ);
      fputs (_("\
Legacy interface ਮੰਜ਼ਿਲ the cksum(1) utility.\n\
"), stdout);
#ਜੇ_ਅੰਤ

      emit_stdin_note ();
#ਜੇ HASH_ALGO_ਜੋੜ
      oputs (_("\
  -r\n\
         use BSD ਜੋੜ algorithm (the ਮੂਲ), use 1K ਖੰਡਾਂ\n\
"));
      oputs (_("\
  -s, --sysv\n\
         use System V ਜੋੜ algorithm, use 512 ਬਾਈਟਾਂ ਖੰਡਾਂ\n\
"));
#ਜੇ_ਅੰਤ
#ਜੇ HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
        emit_mandatory_arg_note ();
#ਜੇ_ਅੰਤ
#ਜੇ HASH_ALGO_CKSUM
     ਜੇ (! legacy_ਬਹੁਲਕ)
        oputs (_("\
  -a, --algorithm=TYPE\n\
         select the digest type ਮੰਜ਼ਿਲ use.  See DIGEST ਹੇਠਾਂ\n\
"));
     ਜੇ (! legacy_ਬਹੁਲਕ)
        oputs (_("\
      --base64\n\
         emit base64-encoded digests, not hexadecimal\n\
"));
#ਜੇ_ਅੰਤ
#ਜੇ !HASH_ALGO_ਜੋੜ
# ਜੇ HASH_ALGO_CKSUM
     ਜੇ (legacy_ਬਹੁਲਕ)
       {
# endif
      ਜੇ (O_ਦੁਆਧਾਰੀ)
        oputs (_("\
  -b, --ਦੁਆਧਾਰੀ\n\
         ਪਠ ਵਿੱਚ ਦੁਆਧਾਰੀ ਬਹੁਲਕ_2 (ਮੂਲ unless reading tty stdin)\n\
"));
      ਨਹੀਂਤਾਂ
        oputs (_("\
  -b, --ਦੁਆਧਾਰੀ\n\
         ਪਠ ਵਿੱਚ ਦੁਆਧਾਰੀ ਬਹੁਲਕ_2\n\
"));
# ਜੇ HASH_ALGO_CKSUM
       }
# endif
        oputs (_("\
  -c, --check\n\
         ਪਠ checksums from the FILEs and check them\n\
"));
# ਜੇ HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
#  ਜੇ HASH_ALGO_CKSUM
     ਜੇ (! legacy_ਬਹੁਲਕ)
#  endif
        oputs (_("\
  -l, --ਲੰਮਾਈ=ਬਿੱਟ\n\
         digest ਲੰਮਾਈ ਵਿੱਚ ਬਿੱਟ_2; must not exceed the max ਮਾਪ\n\
         and must be a ਗੁਣਜ ਦਾ 8 ਲਈ blake2b;\n\
         must be 224, 256, 384, or 512 ਲਈ sha2 or sha3\n\
"));
# endif
# ਜੇ HASH_ALGO_CKSUM
    ਜੇ (! legacy_ਬਹੁਲਕ)
        oputs (_("\
      --raw\n\
         emit a raw ਦੁਆਧਾਰੀ digest, not hexadecimal\n\
"));
    ਜੇ (legacy_ਬਹੁਲਕ)
      oputs (_("\
      --ਟੈਗ\n\
         create a BSD-style checksum\n\
"));
    ਨਹੀਂਤਾਂ
      oputs (_("\
      --ਟੈਗ\n\
         create a BSD-style checksum (the ਮੂਲ)\n\
"));
    ਜੇ (! legacy_ਬਹੁਲਕ)
      oputs (_("\
      --untagged\n\
         create a reversed style checksum, without digest type\n\
"));
# ਨਹੀਂਤਾਂ
      oputs (_("\
      --ਟੈਗ\n\
         create a BSD-style checksum\n\
"));
# endif
# ਜੇ HASH_ALGO_CKSUM
    ਜੇ (legacy_ਬਹੁਲਕ)
      {
# endif
      ਜੇ (O_ਦੁਆਧਾਰੀ)
        oputs (_("\
  -t, --ਪਾਠ\n\
         ਪਠ ਵਿੱਚ ਪਾਠ ਬਹੁਲਕ_2 (ਮੂਲ ਜੇ reading tty stdin)\n\
"));
      ਨਹੀਂਤਾਂ
        oputs (_("\
  -t, --ਪਾਠ\n\
         ਪਠ ਵਿੱਚ ਪਾਠ ਬਹੁਲਕ_2 (ਮੂਲ)\n\
"));
# ਜੇ HASH_ALGO_CKSUM
      }
# endif
      oputs (_("\
  -z, --zero\n\
         ਸਿਰਾ each output ਸਤਰ with NUL, not newline,\n\
         and disable file name escaping\n\
"));
      fputs (_("\
\n\
The following five options are useful only when verifying checksums:\n\
"), stdout);
      oputs (_("\
      --ignore-missing\n\
         don't fail or report ਸਥਿਤੀ ਲਈ missing files\n\
"));
      oputs (_("\
      --quiet\n\
         don't print OK ਲਈ each successfully verified file\n\
"));
      oputs (_("\
      --ਸਥਿਤੀ\n\
         don't output anything, ਸਥਿਤੀ ਕੋਡ shows success\n\
"));
      oputs (_("\
      --strict\n\
         ਬਾਹਰ_ਨਿਕਲੋ non-zero ਲਈ improperly formatted checksum ਸਤਰਾਂ\n\
"));
      oputs (_("\
  -w, --warn\n\
         warn about improperly formatted checksum ਸਤਰਾਂ\n\
"));
#ਜੇ_ਅੰਤ
#ਜੇ HASH_ALGO_CKSUM
    ਜੇ (! legacy_ਬਹੁਲਕ)
      oputs (_("\
      --debug\n\
         indicate which implementation ਵਰਤਿਆ\n\
"));
#ਜੇ_ਅੰਤ
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
#ਜੇ HASH_ALGO_CKSUM
    ਜੇ (! legacy_ਬਹੁਲਕ)
      fputs (_("\
\n\
DIGEST determines the digest algorithm and ਮੂਲ output format:\n\
  sysv      (equivalent ਮੰਜ਼ਿਲ ਜੋੜ -s)\n\
  bsd       (equivalent ਮੰਜ਼ਿਲ ਜੋੜ -r)\n\
  ਚੱਕਰੀ_ਜਾਂਚ       (equivalent ਮੰਜ਼ਿਲ cksum)\n\
  crc32b    (only ਉਪਲਬਧ through cksum)\n\
  md5       (equivalent ਮੰਜ਼ਿਲ md5sum)\n\
  sha1      (equivalent ਮੰਜ਼ਿਲ sha1sum)\n\
  sha2      (equivalent ਮੰਜ਼ਿਲ sha{224,256,384,512}ਜੋੜ)\n\
  sha3      (only ਉਪਲਬਧ through cksum)\n\
  blake2b   (equivalent ਮੰਜ਼ਿਲ b2sum)\n\
  sm3       (only ਉਪਲਬਧ through cksum)\n\
\n"), stdout);
#ਜੇ_ਅੰਤ
#ਜੇ !HASH_ALGO_ਜੋੜ && !HASH_ALGO_CKSUM
      ਲਿਖੋ (_("\
\n\
The sums are computed as described ਵਿੱਚ %s.\n"), DIGEST_REFERENCE);
      fputs (_("\
When checking, the ਨਿਵੇਸ਼ should be a former output ਦਾ this ਪ੍ਰੋਗਰਾਮ.\n\
The ਮੂਲ ਬਹੁਲਕ_2 is ਮੰਜ਼ਿਲ print a ਸਤਰ with: checksum, a space,\n\
a character indicating ਨਿਵੇਸ਼ ਬਹੁਲਕ_2 ('*' ਲਈ ਦੁਆਧਾਰੀ, ' ' ਲਈ ਪਾਠ\n\
or where ਦੁਆਧਾਰੀ is insignificant), and name ਲਈ each FILE.\n\
\n\
There is no difference between ਦੁਆਧਾਰੀ ਬਹੁਲਕ_2 and ਪਾਠ ਬਹੁਲਕ_2 on GNU systems.\
\n"), stdout);
#ਜੇ_ਅੰਤ
#ਜੇ HASH_ALGO_CKSUM
      fputs (_("\
When checking, the ਨਿਵੇਸ਼ should be a former output ਦਾ this ਪ੍ਰੋਗਰਾਮ,\n\
or equivalent standalone ਪ੍ਰੋਗਰਾਮ.\
\n"), stdout);
#ਜੇ_ਅੰਤ
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }

  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

#ਜੇ !HASH_ALGO_ਜੋੜ
/* Given a string S, return TRUE if it contains problematic characters
   that need escaping.  Note we escape '\' itself to provide some forward
   compat to introduce escaping of other characters.  */

ATTRIBUTE_ਸ਼ੁੱਧ
ਸਥਿਰ bool
problematic_chars (ਅੱਖਰ ਅਚਲ *s)
{
  idx_t ਲੰਮਾਈ = strcspn (s, "\\\n\r");
  ਵਾਪਸ s[ਲੰਮਾਈ] != '\0';
}
#ਜੇ_ਅੰਤ

/* Given a file name, S of length S_LEN, that is not NUL-terminated,
   modify it in place, performing the equivalent of this sed substitution:
   's/\\n/\n/g;s/\\r/\r/g;s/\\\\/\\/g' i.e., replacing each "\\n" string
   with a newline, each "\\r" string with a carriage return,
   and each "\\\\" with a single backslash, NUL-terminate it and return S.
   If S is not a valid escaped file name, i.e., if it ends with an odd number
   of backslashes or if it contains a backslash followed by anything other
   than "n" or another backslash, return NULL.  */

ਸਥਿਰ ਅੱਖਰ *
filename_unescape (ਅੱਖਰ *s, idx_t s_len)
{
  ਅੱਖਰ *dst = s;

  ਲਈ (idx_t i = 0; i < s_len; i++)
    {
      ਚੋਣ (s[i])
        {
        ਹਾਲਤ '\\':
          ਜੇ (i == s_len - 1)
            {
              /* File name ends with an unescaped backslash: invalid.  */
              ਵਾਪਸ ਨਿਰਾ;
            }
          ++i;
          ਚੋਣ (s[i])
            {
            ਹਾਲਤ 'n':
              *dst++ = '\n';
              ਰੋਕੋ;
            ਹਾਲਤ 'r':
              *dst++ = '\r';
              ਰੋਕੋ;
            ਹਾਲਤ '\\':
              *dst++ = '\\';
              ਰੋਕੋ;
            ਮੂਲ:
              /* Only '\', 'n' or 'r' may follow a backslash.  */
              ਵਾਪਸ ਨਿਰਾ;
            }
          ਰੋਕੋ;

        ਹਾਲਤ '\0':
          /* The file name may not contain a NUL.  */
          ਵਾਪਸ ਨਿਰਾ;

        ਮੂਲ:
          *dst++ = s[i];
          ਰੋਕੋ;
        }
    }
  ਜੇ (dst < s + s_len)
    *dst = '\0';

  ਵਾਪਸ s;
}

/* Return true if S is a LEN-byte NUL-terminated string of hex or base64
   digits and has the expected length.  Otherwise, return false.  */
ATTRIBUTE_ਸ਼ੁੱਧ
ਸਥਿਰ bool
valid_digits (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ਅਚਲ *s, idx_t len)
{
#ਜੇ HASH_ALGO_CKSUM
  ਜੇ (len == BASE64_ਲੰਮਾਈ (digest_ਲੰਮਾਈ >> 3))
    {
      idx_t i;
      ਲਈ (i = 0; i < len - digest_ਲੰਮਾਈ % 3; i++)
        {
          ਜੇ (!isbase64 (*s))
            ਵਾਪਸ false;
          ++s;
        }
      ਲਈ ( ; i < len; i++)
        {
          ਜੇ (*s != '=')
            ਵਾਪਸ false;
          ++s;
        }
    }
  ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
  ਜੇ (len == digest_hex_ਬਾਈਟਾਂ)
    {
      ਲਈ (idx_t i = 0; i < digest_hex_ਬਾਈਟਾਂ; i++)
        {
          ਜੇ (!c_isxdigit (*s))
            ਵਾਪਸ false;
          ++s;
        }
    }
  ਨਹੀਂਤਾਂ
    ਵਾਪਸ false;

  ਵਾਪਸ *s == '\0';
}

/* Split the checksum string S (of length S_LEN) from a BSD 'md5' or
   'sha1' command into two parts: a hexadecimal digest, and the file
   name.  S is modified.  Set *D_LEN to the length of the digest string.
   Return true if successful.  */

ਸਥਿਰ bool
bsd_ਵੰਡੋ_3 (ਅੱਖਰ *s, idx_t s_len,
             ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ **digest, idx_t *d_len,
             ਅੱਖਰ **file_name, bool escaped_filename)
{
  ਜੇ (s_len == 0)
    ਵਾਪਸ false;

  /* Find end of filename.  */
  idx_t i = s_len - 1;
  ਜਦੋਂ (i && s[i] != ')')
    i--;

  ਜੇ (s[i] != ')')
    ਵਾਪਸ false;

  *file_name = s;

  ਜੇ (escaped_filename && filename_unescape (s, i) == ਨਿਰਾ)
    ਵਾਪਸ false;

  s[i++] = '\0';

  ਜਦੋਂ (c_isblank (s[i]))
    i++;

  ਜੇ (s[i] != '=')
    ਵਾਪਸ false;

  i++;

  ਜਦੋਂ (c_isblank (s[i]))
    i++;

  *digest = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *) &s[i];

  *d_len = s_len - i;
  ਵਾਪਸ valid_digits (*digest, *d_len);
}

#ਜੇ HASH_ALGO_CKSUM
/* Return the corresponding Algorithm for the string S,
   or -1 for no match.  */

ਸਥਿਰ ptrdiff_t
algorithm_from_ਟੈਗ (ਅੱਖਰ *s)
{
  /* Limit check size to this length for perf reasons.  */
  ਸਥਿਰ idx_t max_ਟੈਗ_len;
  ਜੇ (! max_ਟੈਗ_len)
    {
      ਅੱਖਰ ਅਚਲ * ਅਚਲ * ਟੈਗ = algorithm_ਟੈਗ_ਸੂਚੀ;
      ਜਦੋਂ (*ਟੈਗ)
        {
          idx_t ਟੈਗ_len = ਲੜੀ_ਲੰਬਾਈ (*ਟੈਗ++);
          max_ਟੈਗ_len = MAX (ਟੈਗ_len, max_ਟੈਗ_len);
        }
    }

  idx_t i = 0;

  /* Find end of tag */
  ਜਦੋਂ (i <= max_ਟੈਗ_len && s[i] && ! c_isblank (s[i])
         && s[i] != '-' && s[i] != '(')
    ++i;

  ਜੇ (i > max_ਟੈਗ_len)
    ਵਾਪਸ -1;

  /* Terminate tag, and lookup.  */
  ਅੱਖਰ sep = s[i];
  s[i] = '\0';
  ptrdiff_t algo = argmatch_exact (s, algorithm_ਟੈਗ_ਸੂਚੀ);
  s[i] = sep;

  ਵਾਪਸ algo;
}
#ਜੇ_ਅੰਤ

/* Split the string S (of length S_LEN) into three parts:
   a hexadecimal digest, binary flag, and the file name.
   S is modified.  Set *D_LEN to the length of the digest string.
   Return true if successful.  */

ਸਥਿਰ bool
ਵੰਡੋ_3 (ਅੱਖਰ *s, idx_t s_len,
         ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ **digest, idx_t *d_len, ਪੂਰਨ *ਦੁਆਧਾਰੀ, ਅੱਖਰ **file_name)
{
  bool escaped_filename = false;
  idx_t algo_name_len;

  idx_t i = 0;
  ਜਦੋਂ (c_isblank (s[i]))
    ++i;

  ਜੇ (s[i] == '\\')
    {
      ++i;
      escaped_filename = true;
    }

  /* Check for BSD-style checksum line. */

#ਜੇ HASH_ALGO_CKSUM
  ਜੇ (! algorithm_specified || cksum_algorithm == sha2)
    {
      ptrdiff_t algo_ਟੈਗ = algorithm_from_ਟੈਗ (s + i);
      ਜੇ (! algorithm_specified)
        {
          ਜੇ (algo_ਟੈਗ >= 0)
            {
              ਜੇ (algo_ਟੈਗ <= crc32b)
                ਵਾਪਸ false;  /* We don't support checking these formats.  */
              cksum_algorithm = algo_ਟੈਗ;
            }
          ਨਹੀਂਤਾਂ
            ਵਾਪਸ false;      /* We only support tagged format without -a.  */
        }
      ਨਹੀਂਤਾਂ
        {
          ਜੇ (cksum_algorithm == sha2 && (algo_ਟੈਗ == sha2
              || algo_ਟੈਗ == sha224 || algo_ਟੈਗ == sha256
              || algo_ਟੈਗ == sha384 || algo_ਟੈਗ == sha512))
            cksum_algorithm = algo_ਟੈਗ;
        }
    }
#ਜੇ_ਅੰਤ

  /* Try to parse BSD or OpenSSL tagged format.  I.e.:
     openssl: MD5(f)= d41d8cd98f00b204e9800998ecf8427e
     bsd:     MD5 (f) = d41d8cd98f00b204e9800998ecf8427e  */

  idx_t parse_ਵਿੱਥ = i;
  algo_name_len = ਲੜੀ_ਲੰਬਾਈ (DIGEST_TYPE_STRING);
  ਜੇ (STREQ_LEN (s + i, DIGEST_TYPE_STRING, algo_name_len))
    {
      i += algo_name_len;
#ਜੇ HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM

# ਜੇ HASH_ALGO_BLAKE2
      digest_ਲੰਮਾਈ = DIGEST_MAX_LEN * 8;
# ਨਹੀਂਤਾਂ
      digest_ਲੰਮਾਈ = algorithm_ਬਿੱਟ[cksum_algorithm];
# endif
      ਜੇ (s[i] == '-')  /* length specified. Not base64 */
        {
          ++i;
          intmax_t ਲੰਮਾਈ;
          ਅੱਖਰ *siend;
          ਜੇ (!c_isdigit (s[i])
              || xstrtoimax (s + i, &siend, 0, &ਲੰਮਾਈ, ਨਿਰਾ) != LONGINT_OK)
            ਵਾਪਸ false;
# ਜੇ HASH_ALGO_CKSUM
          ਨਹੀਂਤਾਂ ਜੇ (cksum_algorithm == sha2 || cksum_algorithm == sha3)
            {
              ਜੇ (ਲੰਮਾਈ != SHA224_DIGEST_ਮਾਪ * 8
                  && ਲੰਮਾਈ != SHA256_DIGEST_ਮਾਪ * 8
                  && ਲੰਮਾਈ != SHA384_DIGEST_ਮਾਪ * 8
                  && ਲੰਮਾਈ != SHA512_DIGEST_ਮਾਪ * 8)
                ਵਾਪਸ false;
            }
# endif
          ਨਹੀਂਤਾਂ ਜੇ (!(0 < ਲੰਮਾਈ && ਲੰਮਾਈ <= digest_ਲੰਮਾਈ && ਲੰਮਾਈ % 8 == 0))
            ਵਾਪਸ false;

          i = siend - s;
          digest_ਲੰਮਾਈ = ਲੰਮਾਈ;
        }
      digest_hex_ਬਾਈਟਾਂ = digest_ਲੰਮਾਈ >> 2;
#ਜੇ_ਅੰਤ
      ਜੇ (s[i] == ' ')
        ++i;
      ਜੇ (s[i] == '(')  /* not base64 */
        {
          ++i;
          *ਦੁਆਧਾਰੀ = 0;
          ਵਾਪਸ bsd_ਵੰਡੋ_3 (s + i, s_len - i,
                              digest, d_len, file_name, escaped_filename);
        }

      /* Note with --base64 --untagged format, we may have matched a "tag".
         Even very short digests with: cksum -a blake2b -l24 --untagged --base64
         So fallback to checking untagged format if issues detecting tags.  */
      i = parse_ਵਿੱਥ;
    }

  /* Ignore this line if it is too short.
     Each line must have at least 'min_digest_line_length - 1' (or one more, if
     the first is a backslash) more characters to contain correct message digest
     information.  */
  ਜੇ (s_len - i < min_digest_ਸਤਰ_ਲੰਮਾਈ + (s[i] == '\\'))
    ਵਾਪਸ false;

  *digest = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *) &s[i];

#ਜੇ HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
  /* Auto determine length.  */
# ਜੇ HASH_ALGO_CKSUM
  ਜੇ (cksum_algorithm == blake2b
      || cksum_algorithm == sha2 || cksum_algorithm == sha3) {
# endif
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ਅਚਲ *hp = *digest;
  digest_hex_ਬਾਈਟਾਂ = 0;
  ਲਈ (; c_isxdigit (*hp); ++hp, ++digest_hex_ਬਾਈਟਾਂ)
    ;
# ਜੇ HASH_ALGO_CKSUM
  /* Check the number of base64 characters.  This works because the hexadecimal
     character set is a subset of the base64 character set.
     Note there is the ambiguity that all characters are hex when they
     are actually base64 encoded, which could be ambiguous with:
        cksum -a sha2 -l 384 --base64 --untagged
        cksum -a sha2 -l 256 --untagged
     Similarly for sha3 and blake2b.
     However at this length the chances are exceedingly rare (1 in 480R),
     and smaller blake2b lengths aren't practical for verification anyway.  */
  idx_t digest_base64_ਬਾਈਟਾਂ = digest_hex_ਬਾਈਟਾਂ;
  idx_t trailing_equals = 0;
  ਲਈ (; isubase64 (*hp); ++hp, ++digest_base64_ਬਾਈਟਾਂ)
    ;
  ਲਈ (; *hp == '='; ++hp, ++trailing_equals)
    ;
  ਜੇ ((cksum_algorithm == sha2 || cksum_algorithm == sha3)
      && digest_hex_ਬਾਈਟਾਂ >> 1 != SHA224_DIGEST_ਮਾਪ
      && digest_hex_ਬਾਈਟਾਂ >> 1 != SHA256_DIGEST_ਮਾਪ
      && digest_hex_ਬਾਈਟਾਂ >> 1 != SHA384_DIGEST_ਮਾਪ
      && digest_hex_ਬਾਈਟਾਂ >> 1 != SHA512_DIGEST_ਮਾਪ)
    {
      ਜੇ (digest_base64_ਬਾਈਟਾਂ + trailing_equals
          == BASE64_ਲੰਮਾਈ (SHA224_DIGEST_ਮਾਪ))
        digest_hex_ਬਾਈਟਾਂ = SHA224_DIGEST_ਮਾਪ * 2;
      ਨਹੀਂਤਾਂ ਜੇ (digest_base64_ਬਾਈਟਾਂ + trailing_equals
               == BASE64_ਲੰਮਾਈ (SHA256_DIGEST_ਮਾਪ))
        digest_hex_ਬਾਈਟਾਂ = SHA256_DIGEST_ਮਾਪ * 2;
      ਨਹੀਂਤਾਂ ਜੇ (digest_base64_ਬਾਈਟਾਂ + trailing_equals
               == BASE64_ਲੰਮਾਈ (SHA384_DIGEST_ਮਾਪ))
        digest_hex_ਬਾਈਟਾਂ = SHA384_DIGEST_ਮਾਪ * 2;
      ਨਹੀਂਤਾਂ ਜੇ (digest_base64_ਬਾਈਟਾਂ + trailing_equals
               == BASE64_ਲੰਮਾਈ (SHA512_DIGEST_ਮਾਪ))
        digest_hex_ਬਾਈਟਾਂ = SHA512_DIGEST_ਮਾਪ * 2;
      ਨਹੀਂਤਾਂ
        ਵਾਪਸ false;
    }
  ਨਹੀਂਤਾਂ ਜੇ (cksum_algorithm == blake2b
           && digest_hex_ਬਾਈਟਾਂ < digest_base64_ਬਾਈਟਾਂ)
    {
      ਲਈ (ਪੂਰਨ j = 8; j <= DIGEST_MAX_LEN * 8; j += 8)
        {
          ਜੇ (BASE64_ਲੰਮਾਈ (j / 8) == digest_base64_ਬਾਈਟਾਂ + trailing_equals
              && j % 3 == trailing_equals)
            {
              digest_hex_ਬਾਈਟਾਂ = j / 4;
              ਰੋਕੋ;
            }
        }
    }
# endif
  ਜੇ (digest_hex_ਬਾਈਟਾਂ < 2 || digest_hex_ਬਾਈਟਾਂ % 2
      || DIGEST_MAX_LEN * 2 < digest_hex_ਬਾਈਟਾਂ)
    ਵਾਪਸ false;
  digest_ਲੰਮਾਈ = digest_hex_ਬਾਈਟਾਂ * 4;
# ਜੇ HASH_ALGO_CKSUM
  }
# endif
#ਜੇ_ਅੰਤ

  /* This field must be the hexadecimal or base64 representation
     of the message digest.  */
  ਜਦੋਂ (s[i] && !c_isblank (s[i]))
    i++;

  /* The digest must be followed by at least one whitespace character.  */
  ਜੇ (i == s_len)
    ਵਾਪਸ false;

  *d_len = &s[i] - (ਅੱਖਰ *) *digest;
  s[i++] = '\0';

  ਜੇ (! valid_digits (*digest, *d_len))
    ਵਾਪਸ false;

  /* If "bsd reversed" format detected.  */
  ਜੇ ((s_len - i == 1) || (s[i] != ' ' && s[i] != '*'))
    {
      /* Don't allow mixing bsd and standard formats,
         to minimize security issues with attackers
         renaming files with leading spaces.
         This assumes that with bsd format checksums
         that the first file name does not have
         a leading ' ' or '*'.  */
      ਜੇ (bsd_reversed == 0)
        ਵਾਪਸ false;
      bsd_reversed = 1;
    }
  ਨਹੀਂਤਾਂ ਜੇ (bsd_reversed != 1)
    {
      bsd_reversed = 0;
      *ਦੁਆਧਾਰੀ = (s[i++] == '*');
    }

  /* All characters between the type indicator and end of line are
     significant -- that includes leading and trailing white space.  */
  *file_name = &s[i];

  ਜੇ (escaped_filename)
    ਵਾਪਸ filename_unescape (&s[i], s_len - i) != ਨਿਰਾ;

  ਵਾਪਸ true;
}

#ਜੇ !HASH_ALGO_ਜੋੜ
/* If ESCAPE is true, then translate each:
   NEWLINE byte to the string, "\\n",
   CARRIAGE RETURN byte to the string, "\\r",
   and each backslash to "\\\\".  */
ਸਥਿਰ ਖਾਲੀ
print_filename (ਅੱਖਰ ਅਚਲ *file, bool escape)
{
  ਜੇ (! escape)
    {
      fputs (file, stdout);
      ਵਾਪਸ;
    }

  ਜਦੋਂ (*file)
    {
      ਚੋਣ (*file)
        {
        ਹਾਲਤ '\n':
          fputs ("\\n", stdout);
          ਰੋਕੋ;

        ਹਾਲਤ '\r':
          fputs ("\\r", stdout);
          ਰੋਕੋ;

        ਹਾਲਤ '\\':
          fputs ("\\\\", stdout);
          ਰੋਕੋ;

        ਮੂਲ:
          ਅੱਖਰ_ਲਿਖੋ (*file);
          ਰੋਕੋ;
        }
      file++;
    }
}
#ਜੇ_ਅੰਤ

/* An interface to the function, DIGEST_STREAM.
   Operate on FILENAME (it may be "-").

   *BINARY indicates whether the file is binary.  BINARY < 0 means it
   depends on whether binary mode makes any difference and the file is
   a terminal; in that case, clear *BINARY if the file was treated as
   text because it was a terminal.

   Put the checksum in *BIN_RESULT, which must be properly aligned.
   Put true in *MISSING if the file can't be opened due to ENOENT.
   Return true if successful.  */

ਸਥਿਰ bool
digest_file (ਅੱਖਰ ਅਚਲ *filename, ਪੂਰਨ *ਦੁਆਧਾਰੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *bin_ਨਤੀਜਾ,
             bool *missing, MAYBE_ਅਣਵਰਤਿਆ intmax_t *ਲੰਮਾਈ)
{
  FILE *fp;
  ਪੂਰਨ err;
  bool is_stdin = streq (filename, "-");

  *missing = false;

  ਜੇ (is_stdin)
    {
      have_read_stdin = true;
      fp = stdin;
      ਜੇ (O_ਦੁਆਧਾਰੀ && *ਦੁਆਧਾਰੀ)
        {
          ਜੇ (*ਦੁਆਧਾਰੀ < 0)
            *ਦੁਆਧਾਰੀ = ! isatty (STDIN_FILENO);
          ਜੇ (*ਦੁਆਧਾਰੀ)
            xset_ਦੁਆਧਾਰੀ_ਬਹੁਲਕ (STDIN_FILENO, O_ਦੁਆਧਾਰੀ);
        }
    }
  ਨਹੀਂਤਾਂ
    {
      fp = fopen (filename, O_ਦੁਆਧਾਰੀ ? (*ਦੁਆਧਾਰੀ ? "rb" : "rt") : "r");
      ਜੇ (fp == ਨਿਰਾ)
        {
          ਜੇ (ignore_missing && errno == ENOENT)
            {
              *missing = true;
              ਵਾਪਸ true;
            }
          error (0, errno, "%s", quotef (filename));
          ਵਾਪਸ false;
        }
    }

  fadvise (fp, FADVISE_SEQUENTIAL);

#ਜੇ HASH_ALGO_CKSUM
  ਜੇ (cksum_algorithm == blake2b
      || cksum_algorithm == sha2 || cksum_algorithm == sha3)
    *ਲੰਮਾਈ = digest_ਲੰਮਾਈ >> 3;
  err = DIGEST_ਧਾਰਾ (fp, bin_ਨਤੀਜਾ, ਲੰਮਾਈ);
#ਜਾਂਜੇ HASH_ALGO_ਜੋੜ
  err = DIGEST_ਧਾਰਾ (fp, bin_ਨਤੀਜਾ, ਲੰਮਾਈ);
#ਜਾਂਜੇ HASH_ALGO_BLAKE2
  err = DIGEST_ਧਾਰਾ (fp, bin_ਨਤੀਜਾ, digest_ਲੰਮਾਈ >> 3);
#ਨਹੀਂਤਾਂ
  err = DIGEST_ਧਾਰਾ (fp, bin_ਨਤੀਜਾ);
#ਜੇ_ਅੰਤ
  err = err ? errno : 0;
  ਜੇ (is_stdin)
    clearerr (fp);
  ਨਹੀਂਤਾਂ ਜੇ (fclose (fp) != 0 && !err)
    err = errno;

  ਜੇ (err)
    {
      error (0, err, "%s", quotef (filename));
      ਵਾਪਸ false;
    }

  ਵਾਪਸ true;
}

#ਜੇ !HASH_ALGO_ਜੋੜ
ਸਥਿਰ ਖਾਲੀ
output_file (ਅੱਖਰ ਅਚਲ *file, ਪੂਰਨ ਦੁਆਧਾਰੀ_file, ਖਾਲੀ ਅਚਲ *digest,
             MAYBE_ਅਣਵਰਤਿਆ bool raw, bool tagged, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ delim,
             MAYBE_ਅਣਵਰਤਿਆ bool ਦਲੀਲਾਂ, MAYBE_ਅਣਵਰਤਿਆ intmax_t ਲੰਮਾਈ)
{
# ਜੇ HASH_ALGO_CKSUM
  ਜੇ (raw)
    {
      fwrite (digest, 1, digest_ਲੰਮਾਈ >> 3, stdout);
      ਵਾਪਸ;
    }
# endif

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ਅਚਲ *bin_ਬਫਰ = digest;

  /* Output a leading backslash if the file name contains problematic chars.  */
  bool needs_escape = delim == '\n' && problematic_chars (file);

  ਜੇ (needs_escape)
    ਅੱਖਰ_ਲਿਖੋ ('\\');

  ਜੇ (tagged)
    {
# ਜੇ HASH_ALGO_CKSUM
      ਜੇ (cksum_algorithm == sha2)
        ਲਿਖੋ ("SHA%u", (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) digest_ਲੰਮਾਈ);
      ਨਹੀਂਤਾਂ
# endif
      fputs (DIGEST_TYPE_STRING, stdout);
# ਜੇ HASH_ALGO_BLAKE2
      ਜੇ (digest_ਲੰਮਾਈ < DIGEST_MAX_LEN * 8)
        ਲਿਖੋ ("-%u", (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) digest_ਲੰਮਾਈ);
# elif HASH_ALGO_CKSUM
      ਜੇ (cksum_algorithm == sha3)
        ਲਿਖੋ ("-%u", (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) digest_ਲੰਮਾਈ);
      ਜੇ (cksum_algorithm == blake2b)
        {
          ਜੇ (digest_ਲੰਮਾਈ < DIGEST_MAX_LEN * 8)
            ਲਿਖੋ ("-%u", (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) digest_ਲੰਮਾਈ);
        }
# endif
      fputs (" (", stdout);
      print_filename (file, needs_escape);
      fputs (") = ", stdout);
    }

# ਜੇ HASH_ALGO_CKSUM
  ਜੇ (base64_digest)
    {
      ਅੱਖਰ b64[BASE64_ਲੰਮਾਈ (DIGEST_BIN_ਬਾਈਟਾਂ) + 1];
      base64_encode ((ਅੱਖਰ ਅਚਲ *) bin_ਬਫਰ, digest_ਲੰਮਾਈ >> 3,
                     b64, ਆਕਾਰ b64);
      fputs (b64, stdout);
    }
  ਨਹੀਂਤਾਂ
# endif
    {
      ਲਈ (idx_t i = 0; i < (digest_hex_ਬਾਈਟਾਂ >> 1); ++i)
        ਲਿਖੋ ("%02x", bin_ਬਫਰ[i]);
    }

  ਜੇ (!tagged)
    {
      ਅੱਖਰ_ਲਿਖੋ (' ');
      ਅੱਖਰ_ਲਿਖੋ (ਦੁਆਧਾਰੀ_file ? '*' : ' ');
      print_filename (file, needs_escape);
    }

  ਅੱਖਰ_ਲਿਖੋ (delim);
}
#ਜੇ_ਅੰਤ

#ਜੇ HASH_ALGO_CKSUM
/* Return true if B64_DIGEST is the same as the base64 digest of the
   DIGEST_LENGTH/8 bytes at BIN_BUFFER.  */
ਸਥਿਰ bool
b64_equal (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ਅਚਲ *b64_digest, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ਅਚਲ *bin_ਬਫਰ)
{
  idx_t b64_n_ਬਾਈਟਾਂ = BASE64_ਲੰਮਾਈ (digest_ਲੰਮਾਈ >> 3);
  ਅੱਖਰ b64[BASE64_ਲੰਮਾਈ (DIGEST_BIN_ਬਾਈਟਾਂ) + 1];
  base64_encode ((ਅੱਖਰ ਅਚਲ *) bin_ਬਫਰ, digest_ਲੰਮਾਈ >> 3,
                 b64, ਆਕਾਰ b64);
  ਵਾਪਸ memeq (b64_digest, b64, b64_n_ਬਾਈਟਾਂ + 1);
}
#ਜੇ_ਅੰਤ

/* Return true if HEX_DIGEST is the same as the hex-encoded digest of the
   DIGEST_LENGTH/8 bytes at BIN_BUFFER.  */
ਸਥਿਰ bool
hex_equal (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ਅਚਲ *hex_digest, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ਅਚਲ *bin_ਬਫਰ)
{
  ਸਥਿਰ ਅਚਲ ਅੱਖਰ bin2hex[] = { '0', '1', '2', '3',
                                  '4', '5', '6', '7',
                                  '8', '9', 'a', 'b',
                                  'c', 'd', 'e', 'f' };
  idx_t digest_bin_ਬਾਈਟਾਂ = digest_hex_ਬਾਈਟਾਂ >> 1;

  /* Compare generated binary number with text representation
     in check file.  Ignore case of hex digits.  */
  idx_t cnt;
  ਲਈ (cnt = 0; cnt < digest_bin_ਬਾਈਟਾਂ; ++cnt)
    {
      ਜੇ (c_tolower (hex_digest[2 * cnt])
          != bin2hex[bin_ਬਫਰ[cnt] >> 4]
          || (c_tolower (hex_digest[2 * cnt + 1])
              != (bin2hex[bin_ਬਫਰ[cnt] & 0xf])))
        ਰੋਕੋ;
    }
  ਵਾਪਸ cnt == digest_bin_ਬਾਈਟਾਂ;
}

ਸਥਿਰ bool
digest_check (ਅੱਖਰ ਅਚਲ *checkfile_name)
{
  FILE *checkfile_ਧਾਰਾ;
  intmax_t n_misformatted_ਸਤਰਾਂ = 0;
  intmax_t n_mismatched_checksums = 0;
  intmax_t n_open_or_read_failures = 0;
  bool properly_formatted_ਸਤਰਾਂ = false;
  bool matched_checksums = false;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ bin_ਬਫਰ_unaligned[DIGEST_BIN_ਬਾਈਟਾਂ + DIGEST_ALIGN];
  /* Make sure bin_buffer is properly aligned. */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *bin_ਬਫਰ = ptr_align (bin_ਬਫਰ_unaligned, DIGEST_ALIGN);
  intmax_t ਸਤਰ_ਸੰਖਿਆ;
  ਅੱਖਰ *ਸਤਰ;
  size_t ਸਤਰ_chars_allocated;
  bool is_stdin = streq (checkfile_name, "-");

  ਜੇ (is_stdin)
    {
      have_read_stdin = true;
      checkfile_name = _("standard input");
      checkfile_ਧਾਰਾ = stdin;
    }
  ਨਹੀਂਤਾਂ
    {
      checkfile_ਧਾਰਾ = fopen (checkfile_name, "r");
      ਜੇ (checkfile_ਧਾਰਾ == ਨਿਰਾ)
        {
          error (0, errno, "%s", quotef (checkfile_name));
          ਵਾਪਸ false;
        }
    }

  ਸਤਰ_ਸੰਖਿਆ = 0;
  ਸਤਰ = ਨਿਰਾ;
  ਸਤਰ_chars_allocated = 0;
  ਕਰੋ
    {
      ਅੱਖਰ *filename;
      ਪੂਰਨ ਦੁਆਧਾਰੀ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *digest;
      ssize_t ਸਤਰ_ਲੰਮਾਈ;

      ++ਸਤਰ_ਸੰਖਿਆ;
      ਜੇ (ਸਤਰ_ਸੰਖਿਆ == 0)
        error (EXIT_FAILURE, 0, _("%s: too many checksum lines"),
               quotef (checkfile_name));

      ਸਤਰ_ਲੰਮਾਈ = getline (&ਸਤਰ, &ਸਤਰ_chars_allocated, checkfile_ਧਾਰਾ);
      ਜੇ (ਸਤਰ_ਲੰਮਾਈ <= 0)
        ਰੋਕੋ;

      /* Ignore comment lines, which begin with a '#' character.  */
      ਜੇ (ਸਤਰ[0] == '#')
        ਜਾਰੀ;

      /* Remove any trailing newline.  */
      ਸਤਰ_ਲੰਮਾਈ -= ਸਤਰ[ਸਤਰ_ਲੰਮਾਈ - 1] == '\n';
      /* Remove any trailing carriage return.  */
      ਸਤਰ_ਲੰਮਾਈ -= ਸਤਰ[ਸਤਰ_ਲੰਮਾਈ - (0 < ਸਤਰ_ਲੰਮਾਈ)] == '\r';

      /* Ignore empty lines.  */
      ਜੇ (ਸਤਰ_ਲੰਮਾਈ == 0)
        ਜਾਰੀ;

      ਸਤਰ[ਸਤਰ_ਲੰਮਾਈ] = '\0';

      idx_t d_len;
      ਜੇ (! (ਵੰਡੋ_3 (ਸਤਰ, ਸਤਰ_ਲੰਮਾਈ, &digest, &d_len, &ਦੁਆਧਾਰੀ, &filename)
             && ! (is_stdin && streq (filename, "-"))))
        {
          ++n_misformatted_ਸਤਰਾਂ;

          ਜੇ (warn)
            {
              error (0, 0,
                     _("%s: %jd"
                       ": improperly formatted %s checksum line"),
                     quotef (checkfile_name), ਸਤਰ_ਸੰਖਿਆ,
                     DIGEST_TYPE_STRING);
            }
        }
      ਨਹੀਂਤਾਂ
        {
          bool ok;
          bool missing;

          properly_formatted_ਸਤਰਾਂ = true;

          intmax_t ਲੰਮਾਈ;
          ok = digest_file (filename, &ਦੁਆਧਾਰੀ, bin_ਬਫਰ, &missing, &ਲੰਮਾਈ);

          ਜੇ (!ok)
            {
              ++n_open_or_read_failures;
              ਜੇ (!ਸਥਿਤੀ_only)
                ਲਿਖੋ ("%s: %s\n", quotef (filename),_("FAILED open or read"));
            }
          ਨਹੀਂਤਾਂ ਜੇ (ignore_missing && missing)
            {
              /* Ignore missing files with --ignore-missing.  */
              ;
            }
          ਨਹੀਂਤਾਂ
            {
              bool ਮੇਲ = false;
#ਜੇ HASH_ALGO_CKSUM
              ਜੇ (d_len == BASE64_ਲੰਮਾਈ (digest_ਲੰਮਾਈ >> 3))
                ਮੇਲ = b64_equal (digest, bin_ਬਫਰ);
              ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
                ਜੇ (d_len == digest_hex_ਬਾਈਟਾਂ)
                  ਮੇਲ = hex_equal (digest, bin_ਬਫਰ);

              ਜੇ (ਮੇਲ)
                matched_checksums = true;
              ਨਹੀਂਤਾਂ
                ++n_mismatched_checksums;

              ਜੇ (!ਸਥਿਤੀ_only)
                {
                  ਜੇ (! ਮੇਲ || ! quiet)
                    fputs (quotef (filename), stdout);

                  ਜੇ (! ਮੇਲ)
                    ਲਿਖੋ (": %s\n", _("FAILED"));
                  ਨਹੀਂਤਾਂ ਜੇ (!quiet)
                    ਲਿਖੋ (": %s\n", _("OK"));
                }
            }

          ਜੇ (ferror (stdout))
            write_error ();
        }
    }
  ਜਦੋਂ (!feof (checkfile_ਧਾਰਾ) && !ferror (checkfile_ਧਾਰਾ));

  ਮੁਕਤ (ਸਤਰ);

  ਪੂਰਨ err = ferror (checkfile_ਧਾਰਾ) ? 0 : -1;
  ਜੇ (is_stdin)
    clearerr (checkfile_ਧਾਰਾ);
  ਨਹੀਂਤਾਂ ਜੇ (fclose (checkfile_ਧਾਰਾ) != 0 && err < 0)
    err = errno;

  ਜੇ (0 <= err)
    {
      error (0, err, err ? "%s" : _("%s: read error"),
             quotef (checkfile_name));
      ਵਾਪਸ false;
    }

  ਜੇ (! properly_formatted_ਸਤਰਾਂ)
    {
      /* Warn if no tests are found.  */
      error (0, 0, _("%s: no properly formatted checksum lines found"),
             quotef (checkfile_name));
    }
  ਨਹੀਂਤਾਂ
    {
      ਜੇ (!ਸਥਿਤੀ_only)
        {
          ਜੇ (n_misformatted_ਸਤਰਾਂ != 0)
            error (0, 0,
                   (ngettext
                    ("WARNING: %jd line is improperly formatted",
                     "WARNING: %jd lines are improperly formatted",
                     select_plural (n_misformatted_ਸਤਰਾਂ))),
                   n_misformatted_ਸਤਰਾਂ);

          ਜੇ (n_open_or_read_failures != 0)
            error (0, 0,
                   (ngettext
                    ("WARNING: %jd listed file could not be read",
                     "WARNING: %jd listed files could not be read",
                     select_plural (n_open_or_read_failures))),
                   n_open_or_read_failures);

          ਜੇ (n_mismatched_checksums != 0)
            error (0, 0,
                   (ngettext
                    ("WARNING: %jd computed checksum did NOT match",
                     "WARNING: %jd computed checksums did NOT match",
                     select_plural (n_mismatched_checksums))),
                   n_mismatched_checksums);

          ਜੇ (ignore_missing && ! matched_checksums)
            error (0, 0, _("%s: no file was verified"),
                   quotef (checkfile_name));
        }
    }

  ਵਾਪਸ (properly_formatted_ਸਤਰਾਂ
          && matched_checksums
          && n_mismatched_checksums == 0
          && n_open_or_read_failures == 0
          && (!strict || n_misformatted_ਸਤਰਾਂ == 0));
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ bin_ਬਫਰ_unaligned[DIGEST_BIN_ਬਾਈਟਾਂ + DIGEST_ALIGN];
  /* Make sure bin_buffer is properly aligned. */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *bin_ਬਫਰ = ptr_align (bin_ਬਫਰ_unaligned, DIGEST_ALIGN);
  bool do_check = false;
  ਪੂਰਨ opt;
  bool ok = true;
  ਪੂਰਨ ਦੁਆਧਾਰੀ = -1;
  ਪੂਰਨ ਅਗੇਤਰ_ਟੈਗ = -1;
  ਬਣਤਰ option ਅਚਲ *long_opts = long_options;

#ਜੇ HASH_ALGO_CKSUM
  ਜੇ (cksum_algorithm != ਚੱਕਰੀ_ਜਾਂਚ)
    {
      legacy_ਬਹੁਲਕ = true;
      ਅਗੇਤਰ_ਟੈਗ = 0;
      algorithm_specified = true;
      long_opts = legacy_long_options;
    }
#ਜੇ_ਅੰਤ

  /* Setting values of global variables.  */
  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* Line buffer stdout to ensure lines are written atomically and immediately
     so that processes running in parallel do not intersperse their output.  */
  setvbuf (stdout, ਨਿਰਾ, _IOLBF, 0);

#ਜੇ HASH_ALGO_ਜੋੜ
  ਅੱਖਰ ਅਚਲ *short_opts = "rs";
#ਜਾਂਜੇ HASH_ALGO_CKSUM
  ਅੱਖਰ ਅਚਲ *short_opts = legacy_ਬਹੁਲਕ ? "bctwz" : "a:l:bctwz";
  ਅੱਖਰ ਅਚਲ *digest_ਲੰਮਾਈ_str = "";
#ਜਾਂਜੇ HASH_ALGO_BLAKE2
  ਅੱਖਰ ਅਚਲ *short_opts = "l:bctwz";
  ਅੱਖਰ ਅਚਲ *digest_ਲੰਮਾਈ_str = "";
#ਨਹੀਂਤਾਂ
  ਅੱਖਰ ਅਚਲ *short_opts = "bctwz";
#ਜੇ_ਅੰਤ

  ਜਦੋਂ ((opt = getopt_long (argc, argv, short_opts, long_opts, ਨਿਰਾ))
         != -1)
    ਚੋਣ (opt)
      {
#ਜੇ HASH_ALGO_CKSUM
      ਹਾਲਤ 'a':
        cksum_algorithm = XARGMATCH_EXACT ("--algorithm", optarg,
                                           algorithm_ਦਲੀਲਾਂ, algorithm_types);
        algorithm_specified = true;
        ਰੋਕੋ;

      ਹਾਲਤ DEBUG_ਪ੍ਰੋਗਰਾਮ_OPTION:
        cksum_debug = true;
        ਰੋਕੋ;
#ਜੇ_ਅੰਤ
#ਜੇ HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
      ਹਾਲਤ 'l':
        digest_ਲੰਮਾਈ = xnumtoimax (optarg, 10, 0, IDX_MAX, "",
                                    _("invalid length"), 0,
                                    XTOINT_MAX_QUIET);
        digest_ਲੰਮਾਈ_str = optarg;
        ਰੋਕੋ;
#ਜੇ_ਅੰਤ
#ਜੇ !HASH_ALGO_ਜੋੜ
      ਹਾਲਤ 'c':
        do_check = true;
        ਰੋਕੋ;
      ਹਾਲਤ ਸਥਿਤੀ_OPTION:
        ਸਥਿਤੀ_only = true;
        warn = false;
        quiet = false;
        ਰੋਕੋ;
      ਹਾਲਤ 'b':
        ਦੁਆਧਾਰੀ = 1;
        ਰੋਕੋ;
      ਹਾਲਤ 't':
        ਦੁਆਧਾਰੀ = 0;
        ਰੋਕੋ;
      ਹਾਲਤ 'w':
        ਸਥਿਤੀ_only = false;
        warn = true;
        quiet = false;
        ਰੋਕੋ;
      ਹਾਲਤ IGNORE_MISSING_OPTION:
        ignore_missing = true;
        ਰੋਕੋ;
      ਹਾਲਤ QUIET_OPTION:
        ਸਥਿਤੀ_only = false;
        warn = false;
        quiet = true;
        ਰੋਕੋ;
      ਹਾਲਤ STRICT_OPTION:
        strict = true;
        ਰੋਕੋ;
# ਜੇ HASH_ALGO_CKSUM
      ਹਾਲਤ BASE64_OPTION:
        base64_digest = true;
        ਰੋਕੋ;
      ਹਾਲਤ RAW_OPTION:
        raw_digest = true;
        ਰੋਕੋ;
      ਹਾਲਤ UNTAG_OPTION:
        ਅਗੇਤਰ_ਟੈਗ = 0;
        ਰੋਕੋ;
# endif
      ਹਾਲਤ ਟੈਗ_OPTION:
        ਅਗੇਤਰ_ਟੈਗ = 1;
        ਰੋਕੋ;
      ਹਾਲਤ 'z':
        digest_delim = '\0';
        ਰੋਕੋ;
#ਜੇ_ਅੰਤ
#ਜੇ HASH_ALGO_ਜੋੜ
      ਹਾਲਤ 'r':		/* For SysV compatibility. */
        ਜੋੜ_algorithm = bsd;
        ਰੋਕੋ;

      ਹਾਲਤ 's':
        ਜੋੜ_algorithm = sysv;
        ਰੋਕੋ;
#ਜੇ_ਅੰਤ
      case_GETOPT_HELP_CHAR;
      case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);
      ਮੂਲ:
        usage (EXIT_FAILURE);
      }

  min_digest_ਸਤਰ_ਲੰਮਾਈ = MIN_DIGEST_ਸਤਰ_ਲੰਮਾਈ;
#ਜੇ HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# ਜੇ HASH_ALGO_CKSUM
  ਜੇ (digest_ਲੰਮਾਈ && (cksum_algorithm != blake2b
                        && cksum_algorithm != sha2
                        && cksum_algorithm != sha3))
    error (EXIT_FAILURE, 0,
           _("--length is only supported with --algorithm "
             "blake2b, sha2, or sha3"));
  ਜੇ (cksum_algorithm == sha2 || cksum_algorithm == sha3)
    {
      /* Do not require --length with --check.  */
      ਜੇ (digest_ਲੰਮਾਈ == 0 && *digest_ਲੰਮਾਈ_str == '\0' && ! do_check)
        error (EXIT_FAILURE, 0, _("--algorithm=%s requires specifying "
                                  "--length 224, 256, 384, or 512"),
               algorithm_ਦਲੀਲਾਂ[cksum_algorithm]);
      /* If --check and --length are used we verify the digest length.  */
      ਜੇ ((! do_check || *digest_ਲੰਮਾਈ_str != '\0')
          && digest_ਲੰਮਾਈ != SHA224_DIGEST_ਮਾਪ * 8
          && digest_ਲੰਮਾਈ != SHA256_DIGEST_ਮਾਪ * 8
          && digest_ਲੰਮਾਈ != SHA384_DIGEST_ਮਾਪ * 8
          && digest_ਲੰਮਾਈ != SHA512_DIGEST_ਮਾਪ * 8)
        {
          error (0, 0, _("invalid length: %s"), quote (digest_ਲੰਮਾਈ_str));
          error (EXIT_FAILURE, 0, _("digest length for %s must be "
                                    "224, 256, 384, or 512"),
                 quote (DIGEST_TYPE_STRING));
        }
    }
  ਨਹੀਂਤਾਂ
    {
      /* If the digest length checks for SHA-3 are satisfied, the less strict
         checks for BLAKE2 will also be.  */
# ਨਹੀਂਤਾਂ
    {
# endif
      ਜੇ (digest_ਲੰਮਾਈ > DIGEST_MAX_LEN * 8)
        {
          error (0, 0, _("invalid length: %s"), quote (digest_ਲੰਮਾਈ_str));
          error (EXIT_FAILURE, 0,
                 _("maximum digest length for %s is %d bits"),
                 quote (DIGEST_TYPE_STRING),
                 DIGEST_MAX_LEN * 8);
        }
      ਜੇ (digest_ਲੰਮਾਈ % 8 != 0)
        {
          error (0, 0, _("invalid length: %s"), quote (digest_ਲੰਮਾਈ_str));
          error (EXIT_FAILURE, 0, _("length is not a multiple of 8"));
        }
    }
  ਜੇ (digest_ਲੰਮਾਈ == 0)
    {
# ਜੇ HASH_ALGO_BLAKE2
      digest_ਲੰਮਾਈ = DIGEST_MAX_LEN * 8;
# ਨਹੀਂਤਾਂ
      digest_ਲੰਮਾਈ = algorithm_ਬਿੱਟ[cksum_algorithm];
# endif
    }
  digest_hex_ਬਾਈਟਾਂ = digest_ਲੰਮਾਈ >> 2;
#ਨਹੀਂਤਾਂ
  digest_hex_ਬਾਈਟਾਂ = DIGEST_HEX_ਬਾਈਟਾਂ;
#ਜੇ_ਅੰਤ

#ਜੇ HASH_ALGO_CKSUM
  ਚੋਣ (+cksum_algorithm)
    {
    ਹਾਲਤ bsd:
    ਹਾਲਤ sysv:
    ਹਾਲਤ ਚੱਕਰੀ_ਜਾਂਚ:
    ਹਾਲਤ crc32b:
        ਜੇ (do_check && algorithm_specified)
          error (EXIT_FAILURE, 0,
                 _("--check is not supported with "
                   "--algorithm={bsd,sysv,crc,crc32b}"));
        ਰੋਕੋ;
    }

  ਜੇ (base64_digest && raw_digest)
   {
     error (0, 0, _("--base64 and --raw are mutually exclusive"));
     usage (EXIT_FAILURE);
   }
#ਜੇ_ਅੰਤ

  ਜੇ (digest_delim != '\n' && do_check)
    {
      error (0, 0, _("the --zero option is not supported when "
                     "verifying checksums"));
      usage (EXIT_FAILURE);
    }
  ਜੇ (1 <= ਅਗੇਤਰ_ਟੈਗ && do_check)
    {
      /* Note we allow --untagged with --check to more
         seamlessly support --untagged in an emulation wrapper.  */
      error (0, 0, _("the --tag option is meaningless when "
                     "verifying checksums"));
      usage (EXIT_FAILURE);
    }

  ਜੇ (0 <= ਦੁਆਧਾਰੀ && do_check)
    {
      error (0, 0, _("the --binary and --text options are meaningless when "
                     "verifying checksums"));
      usage (EXIT_FAILURE);
    }

  ਜੇ (ignore_missing && !do_check)
    {
      error (0, 0,
             _("the --ignore-missing option is meaningful only when "
               "verifying checksums"));
      usage (EXIT_FAILURE);
    }

  ਜੇ (ਸਥਿਤੀ_only && !do_check)
    {
      error (0, 0,
       _("the --status option is meaningful only when verifying checksums"));
      usage (EXIT_FAILURE);
    }

  ਜੇ (warn && !do_check)
    {
      error (0, 0,
       _("the --warn option is meaningful only when verifying checksums"));
      usage (EXIT_FAILURE);
    }

  ਜੇ (quiet && !do_check)
    {
      error (0, 0,
       _("the --quiet option is meaningful only when verifying checksums"));
      usage (EXIT_FAILURE);
    }

  ਜੇ (strict & !do_check)
   {
     error (0, 0,
        _("the --strict option is meaningful only when verifying checksums"));
     usage (EXIT_FAILURE);
   }

  ਜੇ (ਅਗੇਤਰ_ਟੈਗ == -1)
    ਅਗੇਤਰ_ਟੈਗ = HASH_ALGO_CKSUM;

  ਜੇ (ਅਗੇਤਰ_ਟੈਗ && !ਦੁਆਧਾਰੀ)
   {
     /* This could be supported in a backwards compatible way
        by prefixing the output line with a space in text mode.
        However that's invasive enough that it was agreed to
        not support this mode with --tag, as --text use cases
        are adequately supported by the default output format.  */
#ਜੇ !HASH_ALGO_CKSUM
     error (0, 0, _("--tag does not support --text mode"));
#ਨਹੀਂਤਾਂ
     error (0, 0, _("--text mode is only supported with --untagged"));
#ਜੇ_ਅੰਤ
     usage (EXIT_FAILURE);
   }

  ਜੇ (!O_ਦੁਆਧਾਰੀ && ਦੁਆਧਾਰੀ < 0)
    ਦੁਆਧਾਰੀ = 0;
  ਨਹੀਂਤਾਂ ਜੇ (ਅਗੇਤਰ_ਟੈਗ)
    ਦੁਆਧਾਰੀ = 1;

  ਅੱਖਰ **ਸੰਕਾਰਜ_lim = argv + argc;
  ਜੇ (optind == argc)
    *ਸੰਕਾਰਜ_lim++ = bad_cast ("-");
  ਨਹੀਂਤਾਂ ਜੇ (1 < argc - optind && raw_digest)
    error (EXIT_FAILURE, 0,
           _("the --raw option is not supported with multiple files"));

  ਲਈ (ਅੱਖਰ **operandp = argv + optind; operandp < ਸੰਕਾਰਜ_lim; operandp++)
    {
      ਅੱਖਰ *file = *operandp;
      ਜੇ (do_check)
        ok &= digest_check (file);
      ਨਹੀਂਤਾਂ
        {
          ਪੂਰਨ ਦੁਆਧਾਰੀ_file = ਦੁਆਧਾਰੀ;
          bool missing;
          intmax_t ਲੰਮਾਈ;

          ਜੇ (! digest_file (file, &ਦੁਆਧਾਰੀ_file, bin_ਬਫਰ, &missing, &ਲੰਮਾਈ))
            ok = false;
          ਨਹੀਂਤਾਂ
            {
              DIGEST_ਬਾਹਰਲਾ (file, ਦੁਆਧਾਰੀ_file, bin_ਬਫਰ, raw_digest, ਅਗੇਤਰ_ਟੈਗ,
                          digest_delim, optind != argc, ਲੰਮਾਈ);
              ਜੇ (ferror (stdout))
                write_error ();
            }
        }
    }

  ਜੇ (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, _("standard input"));

  ਵਾਪਸ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
