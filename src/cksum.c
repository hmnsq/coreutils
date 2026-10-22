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

#ಅನ್ತರ್ಭಾವ <config.h>

#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>

#ಅನ್ತರ್ಭಾವ "assure.h"
#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "argmatch.h"
#ಅನ್ತರ್ಭಾವ "c-ctype.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "xdectoint.h"
#ಅನ್ತರ್ಭಾವ "xstrtol.h"

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HASH_ALGO_CKSUM
# define HASH_ALGO_CKSUM 0
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ HASH_ALGO_ಮೊತ್ತ || HASH_ALGO_CKSUM
# include "sum.h"
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HASH_ALGO_CKSUM
# include "cksum.h"
# include "cksum_crc.h"
# include "base64.h"
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# include "blake2/b2sum.h"
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HASH_ALGO_MD5 || HASH_ALGO_CKSUM
# include "md5.h"
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HASH_ALGO_SHA1 || HASH_ALGO_CKSUM
# include "sha1.h"
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HASH_ALGO_SHA256 || HASH_ALGO_SHA224 || HASH_ALGO_CKSUM
# include "sha256.h"
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HASH_ALGO_SHA512 || HASH_ALGO_SHA384 || HASH_ALGO_CKSUM
# include "sha512.h"
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HASH_ALGO_CKSUM
# include "sha3.h"
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HASH_ALGO_CKSUM
# include "sm3.h"
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ "fadvise.h"
#ಅನ್ತರ್ಭಾವ "stdio--.h"
#ಅನ್ತರ್ಭಾವ "xbinary-io.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ಆದರೆ HASH_ALGO_ಮೊತ್ತ
# define ಕಾರ್ಯಕ್ರಮ_NAME "sum"
# define DIGEST_TYPE_STRING "BSD"
# define DIGEST_ಹರಿವು sumfns[ಮೊತ್ತ_algorithm]
# define DIGEST_ಹೊರಹೋಗು ಮೊತ್ತ_output_fns[ಮೊತ್ತ_algorithm]
# define DIGEST_ಬಿಟ್ಟುಗಳು 16
# define DIGEST_ALIGN 4
#ಅಥವಾಆದರೆ HASH_ALGO_CKSUM
# define MAX_DIGEST_ಬಿಟ್ಟುಗಳು 512
# define MAX_DIGEST_ALIGN 8
# define DIGEST_TYPE_STRING algorithm_ಟ್ಯಾಗುಗಳು[cksum_algorithm]
# define DIGEST_ಹರಿವು cksumfns[cksum_algorithm]
# define DIGEST_ಹೊರಹೋಗು cksum_output_fns[cksum_algorithm]
# define DIGEST_ಬಿಟ್ಟುಗಳು MAX_DIGEST_ಬಿಟ್ಟುಗಳು
# define DIGEST_ALIGN MAX_DIGEST_ALIGN
#ಅಥವಾಆದರೆ HASH_ALGO_MD5
# define ಕಾರ್ಯಕ್ರಮ_NAME "md5sum"
# define DIGEST_TYPE_STRING "MD5"
# define DIGEST_ಹರಿವು md5_ಹರಿವು
# define DIGEST_ಬಿಟ್ಟುಗಳು 128
# define DIGEST_REFERENCE "RFC 1321"
# define DIGEST_ALIGN 4
#ಅಥವಾಆದರೆ HASH_ALGO_BLAKE2
# define ಕಾರ್ಯಕ್ರಮ_NAME "b2sum"
# define DIGEST_TYPE_STRING "BLAKE2b"
# define DIGEST_ಹರಿವು blake2b_ಹರಿವು
# define DIGEST_ಬಿಟ್ಟುಗಳು 512
# define DIGEST_REFERENCE "RFC 7693"
# define DIGEST_ALIGN 8
#ಅಥವಾಆದರೆ HASH_ALGO_SHA1
# define ಕಾರ್ಯಕ್ರಮ_NAME "sha1sum"
# define DIGEST_TYPE_STRING "SHA1"
# define DIGEST_ಹರಿವು sha1_ಹರಿವು
# define DIGEST_ಬಿಟ್ಟುಗಳು 160
# define DIGEST_REFERENCE "FIPS-180-1"
# define DIGEST_ALIGN 4
#ಅಥವಾಆದರೆ HASH_ALGO_SHA256
# define ಕಾರ್ಯಕ್ರಮ_NAME "sha256sum"
# define DIGEST_TYPE_STRING "SHA256"
# define DIGEST_ಹರಿವು sha256_ಹರಿವು
# define DIGEST_ಬಿಟ್ಟುಗಳು 256
# define DIGEST_REFERENCE "FIPS-180-2"
# define DIGEST_ALIGN 4
#ಅಥವಾಆದರೆ HASH_ALGO_SHA224
# define ಕಾರ್ಯಕ್ರಮ_NAME "sha224sum"
# define DIGEST_TYPE_STRING "SHA224"
# define DIGEST_ಹರಿವು sha224_ಹರಿವು
# define DIGEST_ಬಿಟ್ಟುಗಳು 224
# define DIGEST_REFERENCE "RFC 3874"
# define DIGEST_ALIGN 4
#ಅಥವಾಆದರೆ HASH_ALGO_SHA512
# define ಕಾರ್ಯಕ್ರಮ_NAME "sha512sum"
# define DIGEST_TYPE_STRING "SHA512"
# define DIGEST_ಹರಿವು sha512_ಹರಿವು
# define DIGEST_ಬಿಟ್ಟುಗಳು 512
# define DIGEST_REFERENCE "FIPS-180-2"
# define DIGEST_ALIGN 8
#ಅಥವಾಆದರೆ HASH_ALGO_SHA384
# define ಕಾರ್ಯಕ್ರಮ_NAME "sha384sum"
# define DIGEST_TYPE_STRING "SHA384"
# define DIGEST_ಹರಿವು sha384_ಹರಿವು
# define DIGEST_ಬಿಟ್ಟುಗಳು 384
# define DIGEST_REFERENCE "FIPS-180-2"
# define DIGEST_ALIGN 8
#ಇಲ್ಲದಿದ್ದರೆ
# error "Can't decide which hash algorithm to compile."
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ !HASH_ALGO_ಮೊತ್ತ && !HASH_ALGO_CKSUM
# define DIGEST_ಹೊರಹೋಗು output_file
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ HASH_ALGO_CKSUM
# define ಕಾರ್ಯಕ್ರಮ_NAME (!legacy_ಬಹುಲಕ ? "cksum"			 \
                       : cksum_algorithm == md5 ? "md5sum"	 \
                       : cksum_algorithm == sha1 ? "sha1sum"	 \
                       : cksum_algorithm == sha224 ? "sha224sum" \
                       : cksum_algorithm == sha256 ? "sha256sum" \
                       : cksum_algorithm == sha384 ? "sha384sum" \
                       : cksum_algorithm == sha512 ? "sha512sum" \
                       : "cksum")
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ HASH_ALGO_ಮೊತ್ತ
# define AUTHORS \
  proper_name ("Kayvan Aghaiepour"), \
  proper_name ("David MacKenzie")
#ಅಥವಾಆದರೆ HASH_ALGO_CKSUM
# define AUTHORS \
  proper_name_lite ("Padraig Brady", "P\303\241draig Brady"), \
  proper_name ("Q. Frank Xia")
#ಅಥವಾಆದರೆ HASH_ALGO_BLAKE2
# define AUTHORS \
  proper_name_lite ("Padraig Brady", "P\303\241draig Brady"), \
  proper_name ("Samuel Neves")
#ಇಲ್ಲದಿದ್ದರೆ
# define AUTHORS \
  proper_name ("Ulrich Drepper"), \
  proper_name ("Scott Miller"), \
  proper_name ("David Madore")
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ !HASH_ALGO_BLAKE2 && !HASH_ALGO_CKSUM
# define DIGEST_HEX_ಬೈಟುಗಳು (DIGEST_ಬಿಟ್ಟುಗಳು / 4)
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿಸು DIGEST_BIN_ಬೈಟುಗಳು (DIGEST_ಬಿಟ್ಟುಗಳು / 8)

/* The minimum length of a valid digest line.  This length does
   not include any newline character at the end of a line.  */
#ಆದರೆ HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# define MIN_DIGEST_ಸಾಲು_ಉದ್ದಳತೆ 3 /* With -l 8.  */
#ಇಲ್ಲದಿದ್ದರೆ
# define MIN_DIGEST_ಸಾಲು_ಉದ್ದಳತೆ \
   (DIGEST_HEX_ಬೈಟುಗಳು /* length of hexadecimal message digest */ \
    + 1 /* blank */ \
    + 1 /* minimum filename length */ )
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ !HASH_ALGO_ಮೊತ್ತ
ಸ್ಥಿರ ಶೂನ್ಯ
output_file (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file, ಪೂರ್ಣಾಂಕ ದ್ವಿಮಾನ_file, ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *digest,
             bool raw, bool tagged, ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ delim, bool ವಾದಗಳು,
             intmax_t ಉದ್ದಳತೆ);
#ಆದರೆ_ಅಂತ್ಯ

/* True if any of the files read were the standard input. */
ಸ್ಥಿರ bool have_read_stdin;

/* The minimum length of a valid checksum line for the selected algorithm.  */
ಸ್ಥಿರ idx_t min_digest_ಸಾಲು_ಉದ್ದಳತೆ;

/* Set to the length of a digest hex string for the selected algorithm.  */
ಸ್ಥಿರ idx_t digest_hex_ಬೈಟುಗಳು;

/* With --check, don't generate any output.
   The exit code indicates success or failure.  */
ಸ್ಥಿರ bool ಸ್ಥಿತಿ_only = false;

/* With --check, print a message to standard error warning about each
   improperly formatted checksum line.  */
ಸ್ಥಿರ bool warn = false;

/* With --check, ignore missing files.  */
ಸ್ಥಿರ bool ignore_missing = false;

/* With --check, suppress the "OK" printed for each verified file.  */
ಸ್ಥಿರ bool quiet = false;

/* With --check, exit with a non-zero return code if any line is
   improperly formatted. */
ಸ್ಥಿರ bool strict = false;

/* Whether a BSD reversed format checksum is detected.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ bsd_reversed = -1;

/* line delimiter.  */
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ digest_delim = '\n';

#ಆದರೆ HASH_ALGO_CKSUM
/* If true, print base64-encoded digests, not hex.  */
ಸ್ಥಿರ bool base64_digest = false;
#ಆದರೆ_ಅಂತ್ಯ

/* If true, print binary digests, not hex.  */
ಸ್ಥಿರ bool raw_digest = false;

/* blake2b and sha3 allow the -l option.  Luckily they both have the same
   maximum digest size.  */
#ಆದರೆ HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# ಆದರೆ HASH_ALGO_CKSUM
static_assert (BLAKE2B_OUTBYTES == SHA3_512_DIGEST_ಗಾತ್ರ);
# endif
# define DIGEST_MAX_LEN BLAKE2B_OUTBYTES
ಸ್ಥಿರ idx_t digest_ಉದ್ದಳತೆ;
#ಆದರೆ_ಅಂತ್ಯ /* HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM */

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*digest_output_fn)(ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *, ಪೂರ್ಣಾಂಕ, ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *, bool,
                                 bool, ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ, bool, intmax_t);
#ಆದರೆ HASH_ALGO_ಮೊತ್ತ
ಎಣಿಕೆ Algorithm
{
  bsd,
  sysv,
};

ಸ್ಥಿರ ಎಣಿಕೆ Algorithm ಮೊತ್ತ_algorithm;
ಸ್ಥಿರ sumfn sumfns[]=
{
  bsd_ಮೊತ್ತ_ಹರಿವು,
  sysv_ಮೊತ್ತ_ಹರಿವು,
};
ಸ್ಥಿರ digest_output_fn ಮೊತ್ತ_output_fns[]=
{
  output_bsd,
  output_sysv,
};
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ HASH_ALGO_CKSUM
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
md5_ಮೊತ್ತ_ಹರಿವು (FILE *ಹರಿವು, ಶೂನ್ಯ *resstream, MAYBE_ಬಳಸದ intmax_t *ಉದ್ದಳತೆ)
{
  ಹಿಂತಿರುಗಿ md5_ಹರಿವು (ಹರಿವು, resstream);
}
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
sha1_ಮೊತ್ತ_ಹರಿವು (FILE *ಹರಿವು, ಶೂನ್ಯ *resstream, MAYBE_ಬಳಸದ intmax_t *ಉದ್ದಳತೆ)
{
  ಹಿಂತಿರುಗಿ sha1_ಹರಿವು (ಹರಿವು, resstream);
}
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
sha224_ಮೊತ್ತ_ಹರಿವು (FILE *ಹರಿವು, ಶೂನ್ಯ *resstream,
                   MAYBE_ಬಳಸದ intmax_t *ಉದ್ದಳತೆ)
{
  ಹಿಂತಿರುಗಿ sha224_ಹರಿವು (ಹರಿವು, resstream);
}
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
sha256_ಮೊತ್ತ_ಹರಿವು (FILE *ಹರಿವು, ಶೂನ್ಯ *resstream,
                   MAYBE_ಬಳಸದ intmax_t *ಉದ್ದಳತೆ)
{
  ಹಿಂತಿರುಗಿ sha256_ಹರಿವು (ಹರಿವು, resstream);
}
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
sha384_ಮೊತ್ತ_ಹರಿವು (FILE *ಹರಿವು, ಶೂನ್ಯ *resstream,
                   MAYBE_ಬಳಸದ intmax_t *ಉದ್ದಳತೆ)
{
  ಹಿಂತಿರುಗಿ sha384_ಹರಿವು (ಹರಿವು, resstream);
}
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
sha512_ಮೊತ್ತ_ಹರಿವು (FILE *ಹರಿವು, ಶೂನ್ಯ *resstream,
                   MAYBE_ಬಳಸದ intmax_t *ಉದ್ದಳತೆ)
{
  ಹಿಂತಿರುಗಿ sha512_ಹರಿವು (ಹರಿವು, resstream);
}
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
sha2_ಮೊತ್ತ_ಹರಿವು (FILE *ಹರಿವು, ಶೂನ್ಯ *resstream, intmax_t *ಉದ್ದಳತೆ)
{
  ಆಯ್ಕೆ (*ಉದ್ದಳತೆ)
    {
    ಸ್ಥಿತಿ SHA224_DIGEST_ಗಾತ್ರ:
      ಹಿಂತಿರುಗಿ sha224_ಹರಿವು (ಹರಿವು, resstream);
    ಸ್ಥಿತಿ SHA256_DIGEST_ಗಾತ್ರ:
      ಹಿಂತಿರುಗಿ sha256_ಹರಿವು (ಹರಿವು, resstream);
    ಸ್ಥಿತಿ SHA384_DIGEST_ಗಾತ್ರ:
      ಹಿಂತಿರುಗಿ sha384_ಹರಿವು (ಹರಿವು, resstream);
    ಸ್ಥಿತಿ SHA512_DIGEST_ಗಾತ್ರ:
      ಹಿಂತಿರುಗಿ sha512_ಹರಿವು (ಹರಿವು, resstream);
    ಪೂರ್ವನಿಯೋಜಿತ:
      affirm (false);
    }
}
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
sha3_ಮೊತ್ತ_ಹರಿವು (FILE *ಹರಿವು, ಶೂನ್ಯ *resstream, intmax_t *ಉದ್ದಳತೆ)
{
  ಆಯ್ಕೆ (*ಉದ್ದಳತೆ)
    {
    ಸ್ಥಿತಿ SHA3_224_DIGEST_ಗಾತ್ರ:
      ಹಿಂತಿರುಗಿ sha3_224_ಹರಿವು (ಹರಿವು, resstream);
    ಸ್ಥಿತಿ SHA3_256_DIGEST_ಗಾತ್ರ:
      ಹಿಂತಿರುಗಿ sha3_256_ಹರಿವು (ಹರಿವು, resstream);
    ಸ್ಥಿತಿ SHA3_384_DIGEST_ಗಾತ್ರ:
      ಹಿಂತಿರುಗಿ sha3_384_ಹರಿವು (ಹರಿವು, resstream);
    ಸ್ಥಿತಿ SHA3_512_DIGEST_ಗಾತ್ರ:
      ಹಿಂತಿರುಗಿ sha3_512_ಹರಿವು (ಹರಿವು, resstream);
    ಪೂರ್ವನಿಯೋಜಿತ:
      affirm (false);
    }
}
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
blake2b_ಮೊತ್ತ_ಹರಿವು (FILE *ಹರಿವು, ಶೂನ್ಯ *resstream, intmax_t *ಉದ್ದಳತೆ)
{
  ಹಿಂತಿರುಗಿ blake2b_ಹರಿವು (ಹರಿವು, resstream, *ಉದ್ದಳತೆ);
}
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
sm3_ಮೊತ್ತ_ಹರಿವು (FILE *ಹರಿವು, ಶೂನ್ಯ *resstream, MAYBE_ಬಳಸದ intmax_t *ಉದ್ದಳತೆ)
{
  ಹಿಂತಿರುಗಿ sm3_ಹರಿವು (ಹರಿವು, resstream);
}

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸ್ಥಿರಾಂಕ algorithm_ವಾದಗಳು[] =
{
  "bsd", "sysv", "crc", "crc32b", "md5", "sha1",
  "sha224", "sha256", "sha384", "sha512", /* Legacy naming */
  "sha2", "sha3", "blake2b", "sm3", ಖಾಲಿ
};
ಸ್ಥಿರ ಎಣಿಕೆ Algorithm ಸ್ಥಿರಾಂಕ algorithm_types[] =
{
  bsd, sysv, ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ, crc32b, md5, sha1,
  sha224, sha256, sha384, sha512,
  sha2, sha3, blake2b, sm3,
};
ARGMATCH_VERIFY (algorithm_ವಾದಗಳು, algorithm_types);

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸ್ಥಿರಾಂಕ algorithm_ಟ್ಯಾಗುಗಳು[] =
{
  "BSD", "SYSV", "CRC", "CRC32B", "MD5", "SHA1",
  "SHA224", "SHA256", "SHA384", "SHA512",
  "SHA2", "SHA3", "BLAKE2b", "SM3", ಖಾಲಿ
};
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಸ್ಥಿರಾಂಕ algorithm_ಬಿಟ್ಟುಗಳು[] =
{
  16, 16, 32, 32, 128, 160,
  224, 256, 384, 512,
  512, 512, 512, 256, 0
};

static_assert (countof (algorithm_ಬಿಟ್ಟುಗಳು) == countof (algorithm_ವಾದಗಳು));

ಸ್ಥಿರ bool algorithm_specified = false;
ಸ್ಥಿರ bool legacy_ಬಹುಲಕ = false;
ಎಣಿಕೆ Algorithm cksum_algorithm = ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ;

ಸ್ಥಿರ sumfn cksumfns[]=
{
  bsd_ಮೊತ್ತ_ಹರಿವು,
  sysv_ಮೊತ್ತ_ಹರಿವು,
  ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_ಮೊತ್ತ_ಹರಿವು,
  crc32b_ಮೊತ್ತ_ಹರಿವು,
  md5_ಮೊತ್ತ_ಹರಿವು,
  sha1_ಮೊತ್ತ_ಹರಿವು,
  sha224_ಮೊತ್ತ_ಹರಿವು,
  sha256_ಮೊತ್ತ_ಹರಿವು,
  sha384_ಮೊತ್ತ_ಹರಿವು,
  sha512_ಮೊತ್ತ_ಹರಿವು,
  sha2_ಮೊತ್ತ_ಹರಿವು,
  sha3_ಮೊತ್ತ_ಹರಿವು,
  blake2b_ಮೊತ್ತ_ಹರಿವು,
  sm3_ಮೊತ್ತ_ಹರಿವು,
};
ಸ್ಥಿರ digest_output_fn cksum_output_fns[]=
{
  output_bsd,
  output_sysv,
  output_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ,
  output_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ,
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
#ಆದರೆ_ಅಂತ್ಯ

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */

ಎಣಿಕೆ
{
  IGNORE_MISSING_OPTION = CHAR_MAX + 1,
  ಸ್ಥಿತಿ_OPTION,
  QUIET_OPTION,
  STRICT_OPTION,
  ಟ್ಯಾಗ್_OPTION,
  UNTAG_OPTION,
  DEBUG_ಕಾರ್ಯಕ್ರಮ_OPTION,
  RAW_OPTION,
  BASE64_OPTION,
};

#ಆದರೆ HASH_ALGO_CKSUM
ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ legacy_long_options[] =
{
  { "check", no_ವಾದ, ಖಾಲಿ, 'c' },
  { "ignore-missing", no_ವಾದ, ಖಾಲಿ, IGNORE_MISSING_OPTION },
  { "quiet", no_ವಾದ, ಖಾಲಿ, QUIET_OPTION },
  { "status", no_ವಾದ, ಖಾಲಿ, ಸ್ಥಿತಿ_OPTION },
  { "warn", no_ವಾದ, ಖಾಲಿ, 'w' },
  { "strict", no_ವಾದ, ಖಾಲಿ, STRICT_OPTION },
  { "tag", no_ವಾದ, ಖಾಲಿ, ಟ್ಯಾಗ್_OPTION },
  { "zero", no_ವಾದ, ಖಾಲಿ, 'z' },

  { "binary", no_ವಾದ, ಖಾಲಿ, 'b' }, /* Deprecated.  */
  { "text", no_ವಾದ, ಖಾಲಿ, 't' }, /* Deprecated.  */

  { GETOPT_HELP_OPTION_DECL },
  { GETOPT_ಆವೃತ್ತಿ_OPTION_DECL },
  { ಖಾಲಿ, 0, ಖಾಲಿ, 0 }
};
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ long_options[] =
{
#ಆದರೆ HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
  { "length", required_ವಾದ, ಖಾಲಿ, 'l'},
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ !HASH_ALGO_ಮೊತ್ತ
  { "check", no_ವಾದ, ಖಾಲಿ, 'c' },
  { "ignore-missing", no_ವಾದ, ಖಾಲಿ, IGNORE_MISSING_OPTION},
  { "quiet", no_ವಾದ, ಖಾಲಿ, QUIET_OPTION },
  { "status", no_ವಾದ, ಖಾಲಿ, ಸ್ಥಿತಿ_OPTION },
  { "warn", no_ವಾದ, ಖಾಲಿ, 'w' },
  { "strict", no_ವಾದ, ಖಾಲಿ, STRICT_OPTION },
  { "tag", no_ವಾದ, ಖಾಲಿ, ಟ್ಯಾಗ್_OPTION },
  { "zero", no_ವಾದ, ಖಾಲಿ, 'z' },

# ಆದರೆ HASH_ALGO_CKSUM
  { "algorithm", required_ವಾದ, ಖಾಲಿ, 'a'},
  { "base64", no_ವಾದ, ಖಾಲಿ, BASE64_OPTION },
  { "debug", no_ವಾದ, ಖಾಲಿ, DEBUG_ಕಾರ್ಯಕ್ರಮ_OPTION},
  { "raw", no_ವಾದ, ಖಾಲಿ, RAW_OPTION},
  { "untagged", no_ವಾದ, ಖಾಲಿ, UNTAG_OPTION },
# endif
  { "binary", no_ವಾದ, ಖಾಲಿ, 'b' }, /* Deprecated.  */
  { "text", no_ವಾದ, ಖಾಲಿ, 't' }, /* Deprecated.  */

#ಇಲ್ಲದಿದ್ದರೆ
  {"sysv", no_ವಾದ, ಖಾಲಿ, 's'},  /* Not in cksum.  */
#ಆದರೆ_ಅಂತ್ಯ

  { GETOPT_HELP_OPTION_DECL },
  { GETOPT_ಆವೃತ್ತಿ_OPTION_DECL },
  { ಖಾಲಿ, 0, ಖಾಲಿ, 0 }
};

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("\
Usage: %s [OPTION]... [FILE]...\n\
"), ಕಾರ್ಯಕ್ರಮ_name);
#ಆದರೆ HASH_ALGO_CKSUM
    ಆದರೆ (legacy_ಬಹುಲಕ)
      {
        ಬರೆ (_("\
Print or check %s checksums.\n\
"), DIGEST_TYPE_STRING);
        fputs (_("\
Legacy interface ಗಮ್ಯ the cksum(1) utility.\n\
"), stdout);
      }
    ಇಲ್ಲದಿದ್ದರೆ
      fputs (_("\
Print or verify checksums.\n\
By ಪೂರ್ವನಿಯೋಜಿತ use the 32 ಬಿಟ್ ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_2 algorithm.\n\
"), stdout);
#ಇಲ್ಲದಿದ್ದರೆ
      ಬರೆ (_("\
Print or check %s (%d-ಬಿಟ್) checksums.\n\
"),
              DIGEST_TYPE_STRING,
              DIGEST_ಬಿಟ್ಟುಗಳು);
      fputs (_("\
Legacy interface ಗಮ್ಯ the cksum(1) utility.\n\
"), stdout);
#ಆದರೆ_ಅಂತ್ಯ

      emit_stdin_note ();
#ಆದರೆ HASH_ALGO_ಮೊತ್ತ
      oputs (_("\
  -r\n\
         use BSD ಮೊತ್ತ algorithm (the ಪೂರ್ವನಿಯೋಜಿತ), use 1K ಖಂಡಗಳು\n\
"));
      oputs (_("\
  -s, --sysv\n\
         use System V ಮೊತ್ತ algorithm, use 512 ಬೈಟುಗಳು ಖಂಡಗಳು\n\
"));
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
        emit_mandatory_arg_note ();
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HASH_ALGO_CKSUM
     ಆದರೆ (! legacy_ಬಹುಲಕ)
        oputs (_("\
  -a, --algorithm=TYPE\n\
         select the digest type ಗಮ್ಯ use.  See DIGEST ಕೆಳಗೆ\n\
"));
     ಆದರೆ (! legacy_ಬಹುಲಕ)
        oputs (_("\
      --base64\n\
         emit base64-encoded digests, not hexadecimal\n\
"));
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ !HASH_ALGO_ಮೊತ್ತ
# ಆದರೆ HASH_ALGO_CKSUM
     ಆದರೆ (legacy_ಬಹುಲಕ)
       {
# endif
      ಆದರೆ (O_ದ್ವಿಮಾನ)
        oputs (_("\
  -b, --ದ್ವಿಮಾನ\n\
         ಪಠ ಒಳ ದ್ವಿಮಾನ ಬಹುಲಕ_2 (ಪೂರ್ವನಿಯೋಜಿತ unless reading tty stdin)\n\
"));
      ಇಲ್ಲದಿದ್ದರೆ
        oputs (_("\
  -b, --ದ್ವಿಮಾನ\n\
         ಪಠ ಒಳ ದ್ವಿಮಾನ ಬಹುಲಕ_2\n\
"));
# ಆದರೆ HASH_ALGO_CKSUM
       }
# endif
        oputs (_("\
  -c, --check\n\
         ಪಠ checksums from the FILEs and check them\n\
"));
# ಆದರೆ HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
#  ಆದರೆ HASH_ALGO_CKSUM
     ಆದರೆ (! legacy_ಬಹುಲಕ)
#  endif
        oputs (_("\
  -l, --ಉದ್ದಳತೆ=ಬಿಟ್ಟುಗಳು\n\
         digest ಉದ್ದಳತೆ ಒಳ ಬಿಟ್ಟುಗಳು_2; must not exceed the max ಗಾತ್ರ_2\n\
         and must be a ಅಪವರ್ತ್ಯ ದ 8 ಗಾಗಿ blake2b;\n\
         must be 224, 256, 384, or 512 ಗಾಗಿ sha2 or sha3\n\
"));
# endif
# ಆದರೆ HASH_ALGO_CKSUM
    ಆದರೆ (! legacy_ಬಹುಲಕ)
        oputs (_("\
      --raw\n\
         emit a raw ದ್ವಿಮಾನ digest, not hexadecimal\n\
"));
    ಆದರೆ (legacy_ಬಹುಲಕ)
      oputs (_("\
      --ಟ್ಯಾಗ್\n\
         create a BSD-style checksum\n\
"));
    ಇಲ್ಲದಿದ್ದರೆ
      oputs (_("\
      --ಟ್ಯಾಗ್\n\
         create a BSD-style checksum (the ಪೂರ್ವನಿಯೋಜಿತ)\n\
"));
    ಆದರೆ (! legacy_ಬಹುಲಕ)
      oputs (_("\
      --untagged\n\
         create a reversed style checksum, without digest type\n\
"));
# ಇಲ್ಲದಿದ್ದರೆ
      oputs (_("\
      --ಟ್ಯಾಗ್\n\
         create a BSD-style checksum\n\
"));
# endif
# ಆದರೆ HASH_ALGO_CKSUM
    ಆದರೆ (legacy_ಬಹುಲಕ)
      {
# endif
      ಆದರೆ (O_ದ್ವಿಮಾನ)
        oputs (_("\
  -t, --ಪಠ್ಯ\n\
         ಪಠ ಒಳ ಪಠ್ಯ ಬಹುಲಕ_2 (ಪೂರ್ವನಿಯೋಜಿತ ಆದರೆ reading tty stdin)\n\
"));
      ಇಲ್ಲದಿದ್ದರೆ
        oputs (_("\
  -t, --ಪಠ್ಯ\n\
         ಪಠ ಒಳ ಪಠ್ಯ ಬಹುಲಕ_2 (ಪೂರ್ವನಿಯೋಜಿತ)\n\
"));
# ಆದರೆ HASH_ALGO_CKSUM
      }
# endif
      oputs (_("\
  -z, --zero\n\
         ಕೊನೆ each output ಸಾಲು with NUL, not newline,\n\
         and disable file name escaping\n\
"));
      fputs (_("\
\n\
The following five options are useful only when verifying checksums:\n\
"), stdout);
      oputs (_("\
      --ignore-missing\n\
         don't fail or report ಸ್ಥಿತಿ_2 ಗಾಗಿ missing files\n\
"));
      oputs (_("\
      --quiet\n\
         don't print OK ಗಾಗಿ each successfully verified file\n\
"));
      oputs (_("\
      --ಸ್ಥಿತಿ_2\n\
         don't output anything, ಸ್ಥಿತಿ_2 ಸಂಕೇತ_2 shows success\n\
"));
      oputs (_("\
      --strict\n\
         ನಿರ್ಗಮಿಸು non-zero ಗಾಗಿ improperly formatted checksum ಸಾಲುಗಳು\n\
"));
      oputs (_("\
  -w, --warn\n\
         warn about improperly formatted checksum ಸಾಲುಗಳು\n\
"));
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HASH_ALGO_CKSUM
    ಆದರೆ (! legacy_ಬಹುಲಕ)
      oputs (_("\
      --debug\n\
         indicate which implementation ಬಳಸಿದ\n\
"));
#ಆದರೆ_ಅಂತ್ಯ
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
#ಆದರೆ HASH_ALGO_CKSUM
    ಆದರೆ (! legacy_ಬಹುಲಕ)
      fputs (_("\
\n\
DIGEST determines the digest algorithm and ಪೂರ್ವನಿಯೋಜಿತ output format:\n\
  sysv      (equivalent ಗಮ್ಯ ಮೊತ್ತ -s)\n\
  bsd       (equivalent ಗಮ್ಯ ಮೊತ್ತ -r)\n\
  ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ       (equivalent ಗಮ್ಯ cksum)\n\
  crc32b    (only ಲಭ್ಯ through cksum)\n\
  md5       (equivalent ಗಮ್ಯ md5sum)\n\
  sha1      (equivalent ಗಮ್ಯ sha1sum)\n\
  sha2      (equivalent ಗಮ್ಯ sha{224,256,384,512}ಮೊತ್ತ)\n\
  sha3      (only ಲಭ್ಯ through cksum)\n\
  blake2b   (equivalent ಗಮ್ಯ b2sum)\n\
  sm3       (only ಲಭ್ಯ through cksum)\n\
\n"), stdout);
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ !HASH_ALGO_ಮೊತ್ತ && !HASH_ALGO_CKSUM
      ಬರೆ (_("\
\n\
The sums are computed as described ಒಳ %s.\n"), DIGEST_REFERENCE);
      fputs (_("\
When checking, the ಒಳಹರಿವು should be a former output ದ this ಕಾರ್ಯಕ್ರಮ.\n\
The ಪೂರ್ವನಿಯೋಜಿತ ಬಹುಲಕ_2 is ಗಮ್ಯ print a ಸಾಲು with: checksum, a space,\n\
a character indicating ಒಳಹರಿವು ಬಹುಲಕ_2 ('*' ಗಾಗಿ ದ್ವಿಮಾನ, ' ' ಗಾಗಿ ಪಠ್ಯ\n\
or where ದ್ವಿಮಾನ is insignificant), and name ಗಾಗಿ each FILE.\n\
\n\
There is no difference between ದ್ವಿಮಾನ ಬಹುಲಕ_2 and ಪಠ್ಯ ಬಹುಲಕ_2 on GNU systems.\
\n"), stdout);
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HASH_ALGO_CKSUM
      fputs (_("\
When checking, the ಒಳಹರಿವು should be a former output ದ this ಕಾರ್ಯಕ್ರಮ,\n\
or equivalent standalone ಕಾರ್ಯಕ್ರಮ.\
\n"), stdout);
#ಆದರೆ_ಅಂತ್ಯ
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }

  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

#ಆದರೆ !HASH_ALGO_ಮೊತ್ತ
/* Given a string S, return TRUE if it contains problematic characters
   that need escaping.  Note we escape '\' itself to provide some forward
   compat to introduce escaping of other characters.  */

ATTRIBUTE_ಶುದ್ಧ
ಸ್ಥಿರ bool
problematic_chars (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *s)
{
  idx_t ಉದ್ದಳತೆ = strcspn (s, "\\\n\r");
  ಹಿಂತಿರುಗಿ s[ಉದ್ದಳತೆ] != '\0';
}
#ಆದರೆ_ಅಂತ್ಯ

/* Given a file name, S of length S_LEN, that is not NUL-terminated,
   modify it in place, performing the equivalent of this sed substitution:
   's/\\n/\n/g;s/\\r/\r/g;s/\\\\/\\/g' i.e., replacing each "\\n" string
   with a newline, each "\\r" string with a carriage return,
   and each "\\\\" with a single backslash, NUL-terminate it and return S.
   If S is not a valid escaped file name, i.e., if it ends with an odd number
   of backslashes or if it contains a backslash followed by anything other
   than "n" or another backslash, return NULL.  */

ಸ್ಥಿರ ಅಕ್ಷರ *
filename_unescape (ಅಕ್ಷರ *s, idx_t s_len)
{
  ಅಕ್ಷರ *dst = s;

  ಗಾಗಿ (idx_t i = 0; i < s_len; i++)
    {
      ಆಯ್ಕೆ (s[i])
        {
        ಸ್ಥಿತಿ '\\':
          ಆದರೆ (i == s_len - 1)
            {
              /* File name ends with an unescaped backslash: invalid.  */
              ಹಿಂತಿರುಗಿ ಖಾಲಿ;
            }
          ++i;
          ಆಯ್ಕೆ (s[i])
            {
            ಸ್ಥಿತಿ 'n':
              *dst++ = '\n';
              ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ 'r':
              *dst++ = '\r';
              ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ '\\':
              *dst++ = '\\';
              ನಿಲ್ಲಿಸು;
            ಪೂರ್ವನಿಯೋಜಿತ:
              /* Only '\', 'n' or 'r' may follow a backslash.  */
              ಹಿಂತಿರುಗಿ ಖಾಲಿ;
            }
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ '\0':
          /* The file name may not contain a NUL.  */
          ಹಿಂತಿರುಗಿ ಖಾಲಿ;

        ಪೂರ್ವನಿಯೋಜಿತ:
          *dst++ = s[i];
          ನಿಲ್ಲಿಸು;
        }
    }
  ಆದರೆ (dst < s + s_len)
    *dst = '\0';

  ಹಿಂತಿರುಗಿ s;
}

/* Return true if S is a LEN-byte NUL-terminated string of hex or base64
   digits and has the expected length.  Otherwise, return false.  */
ATTRIBUTE_ಶುದ್ಧ
ಸ್ಥಿರ bool
valid_digits (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *s, idx_t len)
{
#ಆದರೆ HASH_ALGO_CKSUM
  ಆದರೆ (len == BASE64_ಉದ್ದಳತೆ (digest_ಉದ್ದಳತೆ >> 3))
    {
      idx_t i;
      ಗಾಗಿ (i = 0; i < len - digest_ಉದ್ದಳತೆ % 3; i++)
        {
          ಆದರೆ (!isbase64 (*s))
            ಹಿಂತಿರುಗಿ false;
          ++s;
        }
      ಗಾಗಿ ( ; i < len; i++)
        {
          ಆದರೆ (*s != '=')
            ಹಿಂತಿರುಗಿ false;
          ++s;
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ_ಅಂತ್ಯ
  ಆದರೆ (len == digest_hex_ಬೈಟುಗಳು)
    {
      ಗಾಗಿ (idx_t i = 0; i < digest_hex_ಬೈಟುಗಳು; i++)
        {
          ಆದರೆ (!c_isxdigit (*s))
            ಹಿಂತಿರುಗಿ false;
          ++s;
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ false;

  ಹಿಂತಿರುಗಿ *s == '\0';
}

/* Split the checksum string S (of length S_LEN) from a BSD 'md5' or
   'sha1' command into two parts: a hexadecimal digest, and the file
   name.  S is modified.  Set *D_LEN to the length of the digest string.
   Return true if successful.  */

ಸ್ಥಿರ bool
bsd_ವಿಭಜಿಸು_3 (ಅಕ್ಷರ *s, idx_t s_len,
             ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ **digest, idx_t *d_len,
             ಅಕ್ಷರ **file_name, bool escaped_filename)
{
  ಆದರೆ (s_len == 0)
    ಹಿಂತಿರುಗಿ false;

  /* Find end of filename.  */
  idx_t i = s_len - 1;
  ಆಗ (i && s[i] != ')')
    i--;

  ಆದರೆ (s[i] != ')')
    ಹಿಂತಿರುಗಿ false;

  *file_name = s;

  ಆದರೆ (escaped_filename && filename_unescape (s, i) == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ false;

  s[i++] = '\0';

  ಆಗ (c_isblank (s[i]))
    i++;

  ಆದರೆ (s[i] != '=')
    ಹಿಂತಿರುಗಿ false;

  i++;

  ಆಗ (c_isblank (s[i]))
    i++;

  *digest = (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *) &s[i];

  *d_len = s_len - i;
  ಹಿಂತಿರುಗಿ valid_digits (*digest, *d_len);
}

#ಆದರೆ HASH_ALGO_CKSUM
/* Return the corresponding Algorithm for the string S,
   or -1 for no match.  */

ಸ್ಥಿರ ptrdiff_t
algorithm_from_ಟ್ಯಾಗ್ (ಅಕ್ಷರ *s)
{
  /* Limit check size to this length for perf reasons.  */
  ಸ್ಥಿರ idx_t max_ಟ್ಯಾಗ್_len;
  ಆದರೆ (! max_ಟ್ಯಾಗ್_len)
    {
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ * ಸ್ಥಿರಾಂಕ * ಟ್ಯಾಗ್ = algorithm_ಟ್ಯಾಗುಗಳು;
      ಆಗ (*ಟ್ಯಾಗ್)
        {
          idx_t ಟ್ಯಾಗ್_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (*ಟ್ಯಾಗ್++);
          max_ಟ್ಯಾಗ್_len = MAX (ಟ್ಯಾಗ್_len, max_ಟ್ಯಾಗ್_len);
        }
    }

  idx_t i = 0;

  /* Find end of tag */
  ಆಗ (i <= max_ಟ್ಯಾಗ್_len && s[i] && ! c_isblank (s[i])
         && s[i] != '-' && s[i] != '(')
    ++i;

  ಆದರೆ (i > max_ಟ್ಯಾಗ್_len)
    ಹಿಂತಿರುಗಿ -1;

  /* Terminate tag, and lookup.  */
  ಅಕ್ಷರ sep = s[i];
  s[i] = '\0';
  ptrdiff_t algo = argmatch_exact (s, algorithm_ಟ್ಯಾಗುಗಳು);
  s[i] = sep;

  ಹಿಂತಿರುಗಿ algo;
}
#ಆದರೆ_ಅಂತ್ಯ

/* Split the string S (of length S_LEN) into three parts:
   a hexadecimal digest, binary flag, and the file name.
   S is modified.  Set *D_LEN to the length of the digest string.
   Return true if successful.  */

ಸ್ಥಿರ bool
ವಿಭಜಿಸು_3 (ಅಕ್ಷರ *s, idx_t s_len,
         ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ **digest, idx_t *d_len, ಪೂರ್ಣಾಂಕ *ದ್ವಿಮಾನ, ಅಕ್ಷರ **file_name)
{
  bool escaped_filename = false;
  idx_t algo_name_len;

  idx_t i = 0;
  ಆಗ (c_isblank (s[i]))
    ++i;

  ಆದರೆ (s[i] == '\\')
    {
      ++i;
      escaped_filename = true;
    }

  /* Check for BSD-style checksum line. */

#ಆದರೆ HASH_ALGO_CKSUM
  ಆದರೆ (! algorithm_specified || cksum_algorithm == sha2)
    {
      ptrdiff_t algo_ಟ್ಯಾಗ್ = algorithm_from_ಟ್ಯಾಗ್ (s + i);
      ಆದರೆ (! algorithm_specified)
        {
          ಆದರೆ (algo_ಟ್ಯಾಗ್ >= 0)
            {
              ಆದರೆ (algo_ಟ್ಯಾಗ್ <= crc32b)
                ಹಿಂತಿರುಗಿ false;  /* We don't support checking these formats.  */
              cksum_algorithm = algo_ಟ್ಯಾಗ್;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            ಹಿಂತಿರುಗಿ false;      /* We only support tagged format without -a.  */
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಆದರೆ (cksum_algorithm == sha2 && (algo_ಟ್ಯಾಗ್ == sha2
              || algo_ಟ್ಯಾಗ್ == sha224 || algo_ಟ್ಯಾಗ್ == sha256
              || algo_ಟ್ಯಾಗ್ == sha384 || algo_ಟ್ಯಾಗ್ == sha512))
            cksum_algorithm = algo_ಟ್ಯಾಗ್;
        }
    }
#ಆದರೆ_ಅಂತ್ಯ

  /* Try to parse BSD or OpenSSL tagged format.  I.e.:
     openssl: MD5(f)= d41d8cd98f00b204e9800998ecf8427e
     bsd:     MD5 (f) = d41d8cd98f00b204e9800998ecf8427e  */

  idx_t parse_ಸ್ಥಾನಾಂತರ = i;
  algo_name_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (DIGEST_TYPE_STRING);
  ಆದರೆ (STREQ_LEN (s + i, DIGEST_TYPE_STRING, algo_name_len))
    {
      i += algo_name_len;
#ಆದರೆ HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM

# ಆದರೆ HASH_ALGO_BLAKE2
      digest_ಉದ್ದಳತೆ = DIGEST_MAX_LEN * 8;
# ಇಲ್ಲದಿದ್ದರೆ
      digest_ಉದ್ದಳತೆ = algorithm_ಬಿಟ್ಟುಗಳು[cksum_algorithm];
# endif
      ಆದರೆ (s[i] == '-')  /* length specified. Not base64 */
        {
          ++i;
          intmax_t ಉದ್ದಳತೆ;
          ಅಕ್ಷರ *siend;
          ಆದರೆ (!c_isdigit (s[i])
              || xstrtoimax (s + i, &siend, 0, &ಉದ್ದಳತೆ, ಖಾಲಿ) != LONGINT_OK)
            ಹಿಂತಿರುಗಿ false;
# ಆದರೆ HASH_ALGO_CKSUM
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (cksum_algorithm == sha2 || cksum_algorithm == sha3)
            {
              ಆದರೆ (ಉದ್ದಳತೆ != SHA224_DIGEST_ಗಾತ್ರ * 8
                  && ಉದ್ದಳತೆ != SHA256_DIGEST_ಗಾತ್ರ * 8
                  && ಉದ್ದಳತೆ != SHA384_DIGEST_ಗಾತ್ರ * 8
                  && ಉದ್ದಳತೆ != SHA512_DIGEST_ಗಾತ್ರ * 8)
                ಹಿಂತಿರುಗಿ false;
            }
# endif
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!(0 < ಉದ್ದಳತೆ && ಉದ್ದಳತೆ <= digest_ಉದ್ದಳತೆ && ಉದ್ದಳತೆ % 8 == 0))
            ಹಿಂತಿರುಗಿ false;

          i = siend - s;
          digest_ಉದ್ದಳತೆ = ಉದ್ದಳತೆ;
        }
      digest_hex_ಬೈಟುಗಳು = digest_ಉದ್ದಳತೆ >> 2;
#ಆದರೆ_ಅಂತ್ಯ
      ಆದರೆ (s[i] == ' ')
        ++i;
      ಆದರೆ (s[i] == '(')  /* not base64 */
        {
          ++i;
          *ದ್ವಿಮಾನ = 0;
          ಹಿಂತಿರುಗಿ bsd_ವಿಭಜಿಸು_3 (s + i, s_len - i,
                              digest, d_len, file_name, escaped_filename);
        }

      /* Note with --base64 --untagged format, we may have matched a "tag".
         Even very short digests with: cksum -a blake2b -l24 --untagged --base64
         So fallback to checking untagged format if issues detecting tags.  */
      i = parse_ಸ್ಥಾನಾಂತರ;
    }

  /* Ignore this line if it is too short.
     Each line must have at least 'min_digest_line_length - 1' (or one more, if
     the first is a backslash) more characters to contain correct message digest
     information.  */
  ಆದರೆ (s_len - i < min_digest_ಸಾಲು_ಉದ್ದಳತೆ + (s[i] == '\\'))
    ಹಿಂತಿರುಗಿ false;

  *digest = (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *) &s[i];

#ಆದರೆ HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
  /* Auto determine length.  */
# ಆದರೆ HASH_ALGO_CKSUM
  ಆದರೆ (cksum_algorithm == blake2b
      || cksum_algorithm == sha2 || cksum_algorithm == sha3) {
# endif
  ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *hp = *digest;
  digest_hex_ಬೈಟುಗಳು = 0;
  ಗಾಗಿ (; c_isxdigit (*hp); ++hp, ++digest_hex_ಬೈಟುಗಳು)
    ;
# ಆದರೆ HASH_ALGO_CKSUM
  /* Check the number of base64 characters.  This works because the hexadecimal
     character set is a subset of the base64 character set.
     Note there is the ambiguity that all characters are hex when they
     are actually base64 encoded, which could be ambiguous with:
        cksum -a sha2 -l 384 --base64 --untagged
        cksum -a sha2 -l 256 --untagged
     Similarly for sha3 and blake2b.
     However at this length the chances are exceedingly rare (1 in 480R),
     and smaller blake2b lengths aren't practical for verification anyway.  */
  idx_t digest_base64_ಬೈಟುಗಳು = digest_hex_ಬೈಟುಗಳು;
  idx_t trailing_equals = 0;
  ಗಾಗಿ (; isubase64 (*hp); ++hp, ++digest_base64_ಬೈಟುಗಳು)
    ;
  ಗಾಗಿ (; *hp == '='; ++hp, ++trailing_equals)
    ;
  ಆದರೆ ((cksum_algorithm == sha2 || cksum_algorithm == sha3)
      && digest_hex_ಬೈಟುಗಳು >> 1 != SHA224_DIGEST_ಗಾತ್ರ
      && digest_hex_ಬೈಟುಗಳು >> 1 != SHA256_DIGEST_ಗಾತ್ರ
      && digest_hex_ಬೈಟುಗಳು >> 1 != SHA384_DIGEST_ಗಾತ್ರ
      && digest_hex_ಬೈಟುಗಳು >> 1 != SHA512_DIGEST_ಗಾತ್ರ)
    {
      ಆದರೆ (digest_base64_ಬೈಟುಗಳು + trailing_equals
          == BASE64_ಉದ್ದಳತೆ (SHA224_DIGEST_ಗಾತ್ರ))
        digest_hex_ಬೈಟುಗಳು = SHA224_DIGEST_ಗಾತ್ರ * 2;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (digest_base64_ಬೈಟುಗಳು + trailing_equals
               == BASE64_ಉದ್ದಳತೆ (SHA256_DIGEST_ಗಾತ್ರ))
        digest_hex_ಬೈಟುಗಳು = SHA256_DIGEST_ಗಾತ್ರ * 2;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (digest_base64_ಬೈಟುಗಳು + trailing_equals
               == BASE64_ಉದ್ದಳತೆ (SHA384_DIGEST_ಗಾತ್ರ))
        digest_hex_ಬೈಟುಗಳು = SHA384_DIGEST_ಗಾತ್ರ * 2;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (digest_base64_ಬೈಟುಗಳು + trailing_equals
               == BASE64_ಉದ್ದಳತೆ (SHA512_DIGEST_ಗಾತ್ರ))
        digest_hex_ಬೈಟುಗಳು = SHA512_DIGEST_ಗಾತ್ರ * 2;
      ಇಲ್ಲದಿದ್ದರೆ
        ಹಿಂತಿರುಗಿ false;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (cksum_algorithm == blake2b
           && digest_hex_ಬೈಟುಗಳು < digest_base64_ಬೈಟುಗಳು)
    {
      ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 8; j <= DIGEST_MAX_LEN * 8; j += 8)
        {
          ಆದರೆ (BASE64_ಉದ್ದಳತೆ (j / 8) == digest_base64_ಬೈಟುಗಳು + trailing_equals
              && j % 3 == trailing_equals)
            {
              digest_hex_ಬೈಟುಗಳು = j / 4;
              ನಿಲ್ಲಿಸು;
            }
        }
    }
# endif
  ಆದರೆ (digest_hex_ಬೈಟುಗಳು < 2 || digest_hex_ಬೈಟುಗಳು % 2
      || DIGEST_MAX_LEN * 2 < digest_hex_ಬೈಟುಗಳು)
    ಹಿಂತಿರುಗಿ false;
  digest_ಉದ್ದಳತೆ = digest_hex_ಬೈಟುಗಳು * 4;
# ಆದರೆ HASH_ALGO_CKSUM
  }
# endif
#ಆದರೆ_ಅಂತ್ಯ

  /* This field must be the hexadecimal or base64 representation
     of the message digest.  */
  ಆಗ (s[i] && !c_isblank (s[i]))
    i++;

  /* The digest must be followed by at least one whitespace character.  */
  ಆದರೆ (i == s_len)
    ಹಿಂತಿರುಗಿ false;

  *d_len = &s[i] - (ಅಕ್ಷರ *) *digest;
  s[i++] = '\0';

  ಆದರೆ (! valid_digits (*digest, *d_len))
    ಹಿಂತಿರುಗಿ false;

  /* If "bsd reversed" format detected.  */
  ಆದರೆ ((s_len - i == 1) || (s[i] != ' ' && s[i] != '*'))
    {
      /* Don't allow mixing bsd and standard formats,
         to minimize security issues with attackers
         renaming files with leading spaces.
         This assumes that with bsd format checksums
         that the first file name does not have
         a leading ' ' or '*'.  */
      ಆದರೆ (bsd_reversed == 0)
        ಹಿಂತಿರುಗಿ false;
      bsd_reversed = 1;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (bsd_reversed != 1)
    {
      bsd_reversed = 0;
      *ದ್ವಿಮಾನ = (s[i++] == '*');
    }

  /* All characters between the type indicator and end of line are
     significant -- that includes leading and trailing white space.  */
  *file_name = &s[i];

  ಆದರೆ (escaped_filename)
    ಹಿಂತಿರುಗಿ filename_unescape (&s[i], s_len - i) != ಖಾಲಿ;

  ಹಿಂತಿರುಗಿ true;
}

#ಆದರೆ !HASH_ALGO_ಮೊತ್ತ
/* If ESCAPE is true, then translate each:
   NEWLINE byte to the string, "\\n",
   CARRIAGE RETURN byte to the string, "\\r",
   and each backslash to "\\\\".  */
ಸ್ಥಿರ ಶೂನ್ಯ
print_filename (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file, bool escape)
{
  ಆದರೆ (! escape)
    {
      fputs (file, stdout);
      ಹಿಂತಿರುಗಿ;
    }

  ಆಗ (*file)
    {
      ಆಯ್ಕೆ (*file)
        {
        ಸ್ಥಿತಿ '\n':
          fputs ("\\n", stdout);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ '\r':
          fputs ("\\r", stdout);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ '\\':
          fputs ("\\\\", stdout);
          ನಿಲ್ಲಿಸು;

        ಪೂರ್ವನಿಯೋಜಿತ:
          ಅಕ್ಷರ_ಬರೆ (*file);
          ನಿಲ್ಲಿಸು;
        }
      file++;
    }
}
#ಆದರೆ_ಅಂತ್ಯ

/* An interface to the function, DIGEST_STREAM.
   Operate on FILENAME (it may be "-").

   *BINARY indicates whether the file is binary.  BINARY < 0 means it
   depends on whether binary mode makes any difference and the file is
   a terminal; in that case, clear *BINARY if the file was treated as
   text because it was a terminal.

   Put the checksum in *BIN_RESULT, which must be properly aligned.
   Put true in *MISSING if the file can't be opened due to ENOENT.
   Return true if successful.  */

ಸ್ಥಿರ bool
digest_file (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename, ಪೂರ್ಣಾಂಕ *ದ್ವಿಮಾನ, ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *bin_ಫಲಿತಾಂಶ,
             bool *missing, MAYBE_ಬಳಸದ intmax_t *ಉದ್ದಳತೆ)
{
  FILE *fp;
  ಪೂರ್ಣಾಂಕ err;
  bool is_stdin = streq (filename, "-");

  *missing = false;

  ಆದರೆ (is_stdin)
    {
      have_read_stdin = true;
      fp = stdin;
      ಆದರೆ (O_ದ್ವಿಮಾನ && *ದ್ವಿಮಾನ)
        {
          ಆದರೆ (*ದ್ವಿಮಾನ < 0)
            *ದ್ವಿಮಾನ = ! isatty (STDIN_FILENO);
          ಆದರೆ (*ದ್ವಿಮಾನ)
            xset_ದ್ವಿಮಾನ_ಬಹುಲಕ (STDIN_FILENO, O_ದ್ವಿಮಾನ);
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      fp = fopen (filename, O_ದ್ವಿಮಾನ ? (*ದ್ವಿಮಾನ ? "rb" : "rt") : "r");
      ಆದರೆ (fp == ಖಾಲಿ)
        {
          ಆದರೆ (ignore_missing && errno == ENOENT)
            {
              *missing = true;
              ಹಿಂತಿರುಗಿ true;
            }
          error (0, errno, "%s", quotef (filename));
          ಹಿಂತಿರುಗಿ false;
        }
    }

  fadvise (fp, FADVISE_SEQUENTIAL);

#ಆದರೆ HASH_ALGO_CKSUM
  ಆದರೆ (cksum_algorithm == blake2b
      || cksum_algorithm == sha2 || cksum_algorithm == sha3)
    *ಉದ್ದಳತೆ = digest_ಉದ್ದಳತೆ >> 3;
  err = DIGEST_ಹರಿವು (fp, bin_ಫಲಿತಾಂಶ, ಉದ್ದಳತೆ);
#ಅಥವಾಆದರೆ HASH_ALGO_ಮೊತ್ತ
  err = DIGEST_ಹರಿವು (fp, bin_ಫಲಿತಾಂಶ, ಉದ್ದಳತೆ);
#ಅಥವಾಆದರೆ HASH_ALGO_BLAKE2
  err = DIGEST_ಹರಿವು (fp, bin_ಫಲಿತಾಂಶ, digest_ಉದ್ದಳತೆ >> 3);
#ಇಲ್ಲದಿದ್ದರೆ
  err = DIGEST_ಹರಿವು (fp, bin_ಫಲಿತಾಂಶ);
#ಆದರೆ_ಅಂತ್ಯ
  err = err ? errno : 0;
  ಆದರೆ (is_stdin)
    clearerr (fp);
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (fclose (fp) != 0 && !err)
    err = errno;

  ಆದರೆ (err)
    {
      error (0, err, "%s", quotef (filename));
      ಹಿಂತಿರುಗಿ false;
    }

  ಹಿಂತಿರುಗಿ true;
}

#ಆದರೆ !HASH_ALGO_ಮೊತ್ತ
ಸ್ಥಿರ ಶೂನ್ಯ
output_file (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file, ಪೂರ್ಣಾಂಕ ದ್ವಿಮಾನ_file, ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *digest,
             MAYBE_ಬಳಸದ bool raw, bool tagged, ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ delim,
             MAYBE_ಬಳಸದ bool ವಾದಗಳು, MAYBE_ಬಳಸದ intmax_t ಉದ್ದಳತೆ)
{
# ಆದರೆ HASH_ALGO_CKSUM
  ಆದರೆ (raw)
    {
      fwrite (digest, 1, digest_ಉದ್ದಳತೆ >> 3, stdout);
      ಹಿಂತಿರುಗಿ;
    }
# endif

  ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *bin_ಬಫರ್ = digest;

  /* Output a leading backslash if the file name contains problematic chars.  */
  bool needs_escape = delim == '\n' && problematic_chars (file);

  ಆದರೆ (needs_escape)
    ಅಕ್ಷರ_ಬರೆ ('\\');

  ಆದರೆ (tagged)
    {
# ಆದರೆ HASH_ALGO_CKSUM
      ಆದರೆ (cksum_algorithm == sha2)
        ಬರೆ ("SHA%u", (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ) digest_ಉದ್ದಳತೆ);
      ಇಲ್ಲದಿದ್ದರೆ
# endif
      fputs (DIGEST_TYPE_STRING, stdout);
# ಆದರೆ HASH_ALGO_BLAKE2
      ಆದರೆ (digest_ಉದ್ದಳತೆ < DIGEST_MAX_LEN * 8)
        ಬರೆ ("-%u", (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ) digest_ಉದ್ದಳತೆ);
# elif HASH_ALGO_CKSUM
      ಆದರೆ (cksum_algorithm == sha3)
        ಬರೆ ("-%u", (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ) digest_ಉದ್ದಳತೆ);
      ಆದರೆ (cksum_algorithm == blake2b)
        {
          ಆದರೆ (digest_ಉದ್ದಳತೆ < DIGEST_MAX_LEN * 8)
            ಬರೆ ("-%u", (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ) digest_ಉದ್ದಳತೆ);
        }
# endif
      fputs (" (", stdout);
      print_filename (file, needs_escape);
      fputs (") = ", stdout);
    }

# ಆದರೆ HASH_ALGO_CKSUM
  ಆದರೆ (base64_digest)
    {
      ಅಕ್ಷರ b64[BASE64_ಉದ್ದಳತೆ (DIGEST_BIN_ಬೈಟುಗಳು) + 1];
      base64_encode ((ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *) bin_ಬಫರ್, digest_ಉದ್ದಳತೆ >> 3,
                     b64, ಗಾತ್ರ b64);
      fputs (b64, stdout);
    }
  ಇಲ್ಲದಿದ್ದರೆ
# endif
    {
      ಗಾಗಿ (idx_t i = 0; i < (digest_hex_ಬೈಟುಗಳು >> 1); ++i)
        ಬರೆ ("%02x", bin_ಬಫರ್[i]);
    }

  ಆದರೆ (!tagged)
    {
      ಅಕ್ಷರ_ಬರೆ (' ');
      ಅಕ್ಷರ_ಬರೆ (ದ್ವಿಮಾನ_file ? '*' : ' ');
      print_filename (file, needs_escape);
    }

  ಅಕ್ಷರ_ಬರೆ (delim);
}
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ HASH_ALGO_CKSUM
/* Return true if B64_DIGEST is the same as the base64 digest of the
   DIGEST_LENGTH/8 bytes at BIN_BUFFER.  */
ಸ್ಥಿರ bool
b64_equal (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *b64_digest, ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *bin_ಬಫರ್)
{
  idx_t b64_n_ಬೈಟುಗಳು = BASE64_ಉದ್ದಳತೆ (digest_ಉದ್ದಳತೆ >> 3);
  ಅಕ್ಷರ b64[BASE64_ಉದ್ದಳತೆ (DIGEST_BIN_ಬೈಟುಗಳು) + 1];
  base64_encode ((ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *) bin_ಬಫರ್, digest_ಉದ್ದಳತೆ >> 3,
                 b64, ಗಾತ್ರ b64);
  ಹಿಂತಿರುಗಿ memeq (b64_digest, b64, b64_n_ಬೈಟುಗಳು + 1);
}
#ಆದರೆ_ಅಂತ್ಯ

/* Return true if HEX_DIGEST is the same as the hex-encoded digest of the
   DIGEST_LENGTH/8 bytes at BIN_BUFFER.  */
ಸ್ಥಿರ bool
hex_equal (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *hex_digest, ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *bin_ಬಫರ್)
{
  ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ bin2hex[] = { '0', '1', '2', '3',
                                  '4', '5', '6', '7',
                                  '8', '9', 'a', 'b',
                                  'c', 'd', 'e', 'f' };
  idx_t digest_bin_ಬೈಟುಗಳು = digest_hex_ಬೈಟುಗಳು >> 1;

  /* Compare generated binary number with text representation
     in check file.  Ignore case of hex digits.  */
  idx_t cnt;
  ಗಾಗಿ (cnt = 0; cnt < digest_bin_ಬೈಟುಗಳು; ++cnt)
    {
      ಆದರೆ (c_tolower (hex_digest[2 * cnt])
          != bin2hex[bin_ಬಫರ್[cnt] >> 4]
          || (c_tolower (hex_digest[2 * cnt + 1])
              != (bin2hex[bin_ಬಫರ್[cnt] & 0xf])))
        ನಿಲ್ಲಿಸು;
    }
  ಹಿಂತಿರುಗಿ cnt == digest_bin_ಬೈಟುಗಳು;
}

ಸ್ಥಿರ bool
digest_check (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *checkfile_name)
{
  FILE *checkfile_ಹರಿವು;
  intmax_t n_misformatted_ಸಾಲುಗಳು = 0;
  intmax_t n_mismatched_checksums = 0;
  intmax_t n_open_or_read_failures = 0;
  bool properly_formatted_ಸಾಲುಗಳು = false;
  bool matched_checksums = false;
  ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ bin_ಬಫರ್_unaligned[DIGEST_BIN_ಬೈಟುಗಳು + DIGEST_ALIGN];
  /* Make sure bin_buffer is properly aligned. */
  ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *bin_ಬಫರ್ = ptr_align (bin_ಬಫರ್_unaligned, DIGEST_ALIGN);
  intmax_t ಸಾಲು_ಸಂಖ್ಯೆ;
  ಅಕ್ಷರ *ಸಾಲು;
  size_t ಸಾಲು_chars_allocated;
  bool is_stdin = streq (checkfile_name, "-");

  ಆದರೆ (is_stdin)
    {
      have_read_stdin = true;
      checkfile_name = _("standard input");
      checkfile_ಹರಿವು = stdin;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      checkfile_ಹರಿವು = fopen (checkfile_name, "r");
      ಆದರೆ (checkfile_ಹರಿವು == ಖಾಲಿ)
        {
          error (0, errno, "%s", quotef (checkfile_name));
          ಹಿಂತಿರುಗಿ false;
        }
    }

  ಸಾಲು_ಸಂಖ್ಯೆ = 0;
  ಸಾಲು = ಖಾಲಿ;
  ಸಾಲು_chars_allocated = 0;
  ಮಾಡು
    {
      ಅಕ್ಷರ *filename;
      ಪೂರ್ಣಾಂಕ ದ್ವಿಮಾನ;
      ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *digest;
      ssize_t ಸಾಲು_ಉದ್ದಳತೆ;

      ++ಸಾಲು_ಸಂಖ್ಯೆ;
      ಆದರೆ (ಸಾಲು_ಸಂಖ್ಯೆ == 0)
        error (EXIT_FAILURE, 0, _("%s: too many checksum lines"),
               quotef (checkfile_name));

      ಸಾಲು_ಉದ್ದಳತೆ = getline (&ಸಾಲು, &ಸಾಲು_chars_allocated, checkfile_ಹರಿವು);
      ಆದರೆ (ಸಾಲು_ಉದ್ದಳತೆ <= 0)
        ನಿಲ್ಲಿಸು;

      /* Ignore comment lines, which begin with a '#' character.  */
      ಆದರೆ (ಸಾಲು[0] == '#')
        ಮುಂದುವರಿ;

      /* Remove any trailing newline.  */
      ಸಾಲು_ಉದ್ದಳತೆ -= ಸಾಲು[ಸಾಲು_ಉದ್ದಳತೆ - 1] == '\n';
      /* Remove any trailing carriage return.  */
      ಸಾಲು_ಉದ್ದಳತೆ -= ಸಾಲು[ಸಾಲು_ಉದ್ದಳತೆ - (0 < ಸಾಲು_ಉದ್ದಳತೆ)] == '\r';

      /* Ignore empty lines.  */
      ಆದರೆ (ಸಾಲು_ಉದ್ದಳತೆ == 0)
        ಮುಂದುವರಿ;

      ಸಾಲು[ಸಾಲು_ಉದ್ದಳತೆ] = '\0';

      idx_t d_len;
      ಆದರೆ (! (ವಿಭಜಿಸು_3 (ಸಾಲು, ಸಾಲು_ಉದ್ದಳತೆ, &digest, &d_len, &ದ್ವಿಮಾನ, &filename)
             && ! (is_stdin && streq (filename, "-"))))
        {
          ++n_misformatted_ಸಾಲುಗಳು;

          ಆದರೆ (warn)
            {
              error (0, 0,
                     _("%s: %jd"
                       ": improperly formatted %s checksum line"),
                     quotef (checkfile_name), ಸಾಲು_ಸಂಖ್ಯೆ,
                     DIGEST_TYPE_STRING);
            }
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          bool ok;
          bool missing;

          properly_formatted_ಸಾಲುಗಳು = true;

          intmax_t ಉದ್ದಳತೆ;
          ok = digest_file (filename, &ದ್ವಿಮಾನ, bin_ಬಫರ್, &missing, &ಉದ್ದಳತೆ);

          ಆದರೆ (!ok)
            {
              ++n_open_or_read_failures;
              ಆದರೆ (!ಸ್ಥಿತಿ_only)
                ಬರೆ ("%s: %s\n", quotef (filename),_("FAILED open or read"));
            }
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ignore_missing && missing)
            {
              /* Ignore missing files with --ignore-missing.  */
              ;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              bool ಹೊಂದಿಕೆ = false;
#ಆದರೆ HASH_ALGO_CKSUM
              ಆದರೆ (d_len == BASE64_ಉದ್ದಳತೆ (digest_ಉದ್ದಳತೆ >> 3))
                ಹೊಂದಿಕೆ = b64_equal (digest, bin_ಬಫರ್);
              ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ_ಅಂತ್ಯ
                ಆದರೆ (d_len == digest_hex_ಬೈಟುಗಳು)
                  ಹೊಂದಿಕೆ = hex_equal (digest, bin_ಬಫರ್);

              ಆದರೆ (ಹೊಂದಿಕೆ)
                matched_checksums = true;
              ಇಲ್ಲದಿದ್ದರೆ
                ++n_mismatched_checksums;

              ಆದರೆ (!ಸ್ಥಿತಿ_only)
                {
                  ಆದರೆ (! ಹೊಂದಿಕೆ || ! quiet)
                    fputs (quotef (filename), stdout);

                  ಆದರೆ (! ಹೊಂದಿಕೆ)
                    ಬರೆ (": %s\n", _("FAILED"));
                  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!quiet)
                    ಬರೆ (": %s\n", _("OK"));
                }
            }

          ಆದರೆ (ferror (stdout))
            write_error ();
        }
    }
  ಆಗ (!feof (checkfile_ಹರಿವು) && !ferror (checkfile_ಹರಿವು));

  ಬಿಡುಗಡೆ (ಸಾಲು);

  ಪೂರ್ಣಾಂಕ err = ferror (checkfile_ಹರಿವು) ? 0 : -1;
  ಆದರೆ (is_stdin)
    clearerr (checkfile_ಹರಿವು);
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (fclose (checkfile_ಹರಿವು) != 0 && err < 0)
    err = errno;

  ಆದರೆ (0 <= err)
    {
      error (0, err, err ? "%s" : _("%s: read error"),
             quotef (checkfile_name));
      ಹಿಂತಿರುಗಿ false;
    }

  ಆದರೆ (! properly_formatted_ಸಾಲುಗಳು)
    {
      /* Warn if no tests are found.  */
      error (0, 0, _("%s: no properly formatted checksum lines found"),
             quotef (checkfile_name));
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಆದರೆ (!ಸ್ಥಿತಿ_only)
        {
          ಆದರೆ (n_misformatted_ಸಾಲುಗಳು != 0)
            error (0, 0,
                   (ngettext
                    ("WARNING: %jd line is improperly formatted",
                     "WARNING: %jd lines are improperly formatted",
                     select_plural (n_misformatted_ಸಾಲುಗಳು))),
                   n_misformatted_ಸಾಲುಗಳು);

          ಆದರೆ (n_open_or_read_failures != 0)
            error (0, 0,
                   (ngettext
                    ("WARNING: %jd listed file could not be read",
                     "WARNING: %jd listed files could not be read",
                     select_plural (n_open_or_read_failures))),
                   n_open_or_read_failures);

          ಆದರೆ (n_mismatched_checksums != 0)
            error (0, 0,
                   (ngettext
                    ("WARNING: %jd computed checksum did NOT match",
                     "WARNING: %jd computed checksums did NOT match",
                     select_plural (n_mismatched_checksums))),
                   n_mismatched_checksums);

          ಆದರೆ (ignore_missing && ! matched_checksums)
            error (0, 0, _("%s: no file was verified"),
                   quotef (checkfile_name));
        }
    }

  ಹಿಂತಿರುಗಿ (properly_formatted_ಸಾಲುಗಳು
          && matched_checksums
          && n_mismatched_checksums == 0
          && n_open_or_read_failures == 0
          && (!strict || n_misformatted_ಸಾಲುಗಳು == 0));
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ bin_ಬಫರ್_unaligned[DIGEST_BIN_ಬೈಟುಗಳು + DIGEST_ALIGN];
  /* Make sure bin_buffer is properly aligned. */
  ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *bin_ಬಫರ್ = ptr_align (bin_ಬಫರ್_unaligned, DIGEST_ALIGN);
  bool do_check = false;
  ಪೂರ್ಣಾಂಕ opt;
  bool ok = true;
  ಪೂರ್ಣಾಂಕ ದ್ವಿಮಾನ = -1;
  ಪೂರ್ಣಾಂಕ ಪೂರ್ವಪ್ರತ್ಯಯ_ಟ್ಯಾಗ್ = -1;
  ರಚನೆ option ಸ್ಥಿರಾಂಕ *long_opts = long_options;

#ಆದರೆ HASH_ALGO_CKSUM
  ಆದರೆ (cksum_algorithm != ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ)
    {
      legacy_ಬಹುಲಕ = true;
      ಪೂರ್ವಪ್ರತ್ಯಯ_ಟ್ಯಾಗ್ = 0;
      algorithm_specified = true;
      long_opts = legacy_long_options;
    }
#ಆದರೆ_ಅಂತ್ಯ

  /* Setting values of global variables.  */
  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* Line buffer stdout to ensure lines are written atomically and immediately
     so that processes running in parallel do not intersperse their output.  */
  setvbuf (stdout, ಖಾಲಿ, _IOLBF, 0);

#ಆದರೆ HASH_ALGO_ಮೊತ್ತ
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *short_opts = "rs";
#ಅಥವಾಆದರೆ HASH_ALGO_CKSUM
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *short_opts = legacy_ಬಹುಲಕ ? "bctwz" : "a:l:bctwz";
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *digest_ಉದ್ದಳತೆ_str = "";
#ಅಥವಾಆದರೆ HASH_ALGO_BLAKE2
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *short_opts = "l:bctwz";
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *digest_ಉದ್ದಳತೆ_str = "";
#ಇಲ್ಲದಿದ್ದರೆ
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *short_opts = "bctwz";
#ಆದರೆ_ಅಂತ್ಯ

  ಆಗ ((opt = getopt_long (argc, argv, short_opts, long_opts, ಖಾಲಿ))
         != -1)
    ಆಯ್ಕೆ (opt)
      {
#ಆದರೆ HASH_ALGO_CKSUM
      ಸ್ಥಿತಿ 'a':
        cksum_algorithm = XARGMATCH_EXACT ("--algorithm", optarg,
                                           algorithm_ವಾದಗಳು, algorithm_types);
        algorithm_specified = true;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ DEBUG_ಕಾರ್ಯಕ್ರಮ_OPTION:
        cksum_debug = true;
        ನಿಲ್ಲಿಸು;
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
      ಸ್ಥಿತಿ 'l':
        digest_ಉದ್ದಳತೆ = xnumtoimax (optarg, 10, 0, IDX_MAX, "",
                                    _("invalid length"), 0,
                                    XTOINT_MAX_QUIET);
        digest_ಉದ್ದಳತೆ_str = optarg;
        ನಿಲ್ಲಿಸು;
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ !HASH_ALGO_ಮೊತ್ತ
      ಸ್ಥಿತಿ 'c':
        do_check = true;
        ನಿಲ್ಲಿಸು;
      ಸ್ಥಿತಿ ಸ್ಥಿತಿ_OPTION:
        ಸ್ಥಿತಿ_only = true;
        warn = false;
        quiet = false;
        ನಿಲ್ಲಿಸು;
      ಸ್ಥಿತಿ 'b':
        ದ್ವಿಮಾನ = 1;
        ನಿಲ್ಲಿಸು;
      ಸ್ಥಿತಿ 't':
        ದ್ವಿಮಾನ = 0;
        ನಿಲ್ಲಿಸು;
      ಸ್ಥಿತಿ 'w':
        ಸ್ಥಿತಿ_only = false;
        warn = true;
        quiet = false;
        ನಿಲ್ಲಿಸು;
      ಸ್ಥಿತಿ IGNORE_MISSING_OPTION:
        ignore_missing = true;
        ನಿಲ್ಲಿಸು;
      ಸ್ಥಿತಿ QUIET_OPTION:
        ಸ್ಥಿತಿ_only = false;
        warn = false;
        quiet = true;
        ನಿಲ್ಲಿಸು;
      ಸ್ಥಿತಿ STRICT_OPTION:
        strict = true;
        ನಿಲ್ಲಿಸು;
# ಆದರೆ HASH_ALGO_CKSUM
      ಸ್ಥಿತಿ BASE64_OPTION:
        base64_digest = true;
        ನಿಲ್ಲಿಸು;
      ಸ್ಥಿತಿ RAW_OPTION:
        raw_digest = true;
        ನಿಲ್ಲಿಸು;
      ಸ್ಥಿತಿ UNTAG_OPTION:
        ಪೂರ್ವಪ್ರತ್ಯಯ_ಟ್ಯಾಗ್ = 0;
        ನಿಲ್ಲಿಸು;
# endif
      ಸ್ಥಿತಿ ಟ್ಯಾಗ್_OPTION:
        ಪೂರ್ವಪ್ರತ್ಯಯ_ಟ್ಯಾಗ್ = 1;
        ನಿಲ್ಲಿಸು;
      ಸ್ಥಿತಿ 'z':
        digest_delim = '\0';
        ನಿಲ್ಲಿಸು;
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HASH_ALGO_ಮೊತ್ತ
      ಸ್ಥಿತಿ 'r':		/* For SysV compatibility. */
        ಮೊತ್ತ_algorithm = bsd;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ 's':
        ಮೊತ್ತ_algorithm = sysv;
        ನಿಲ್ಲಿಸು;
#ಆದರೆ_ಅಂತ್ಯ
      case_GETOPT_HELP_CHAR;
      case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);
      ಪೂರ್ವನಿಯೋಜಿತ:
        usage (EXIT_FAILURE);
      }

  min_digest_ಸಾಲು_ಉದ್ದಳತೆ = MIN_DIGEST_ಸಾಲು_ಉದ್ದಳತೆ;
#ಆದರೆ HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# ಆದರೆ HASH_ALGO_CKSUM
  ಆದರೆ (digest_ಉದ್ದಳತೆ && (cksum_algorithm != blake2b
                        && cksum_algorithm != sha2
                        && cksum_algorithm != sha3))
    error (EXIT_FAILURE, 0,
           _("--length is only supported with --algorithm "
             "blake2b, sha2, or sha3"));
  ಆದರೆ (cksum_algorithm == sha2 || cksum_algorithm == sha3)
    {
      /* Do not require --length with --check.  */
      ಆದರೆ (digest_ಉದ್ದಳತೆ == 0 && *digest_ಉದ್ದಳತೆ_str == '\0' && ! do_check)
        error (EXIT_FAILURE, 0, _("--algorithm=%s requires specifying "
                                  "--length 224, 256, 384, or 512"),
               algorithm_ವಾದಗಳು[cksum_algorithm]);
      /* If --check and --length are used we verify the digest length.  */
      ಆದರೆ ((! do_check || *digest_ಉದ್ದಳತೆ_str != '\0')
          && digest_ಉದ್ದಳತೆ != SHA224_DIGEST_ಗಾತ್ರ * 8
          && digest_ಉದ್ದಳತೆ != SHA256_DIGEST_ಗಾತ್ರ * 8
          && digest_ಉದ್ದಳತೆ != SHA384_DIGEST_ಗಾತ್ರ * 8
          && digest_ಉದ್ದಳತೆ != SHA512_DIGEST_ಗಾತ್ರ * 8)
        {
          error (0, 0, _("invalid length: %s"), quote (digest_ಉದ್ದಳತೆ_str));
          error (EXIT_FAILURE, 0, _("digest length for %s must be "
                                    "224, 256, 384, or 512"),
                 quote (DIGEST_TYPE_STRING));
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      /* If the digest length checks for SHA-3 are satisfied, the less strict
         checks for BLAKE2 will also be.  */
# ಇಲ್ಲದಿದ್ದರೆ
    {
# endif
      ಆದರೆ (digest_ಉದ್ದಳತೆ > DIGEST_MAX_LEN * 8)
        {
          error (0, 0, _("invalid length: %s"), quote (digest_ಉದ್ದಳತೆ_str));
          error (EXIT_FAILURE, 0,
                 _("maximum digest length for %s is %d bits"),
                 quote (DIGEST_TYPE_STRING),
                 DIGEST_MAX_LEN * 8);
        }
      ಆದರೆ (digest_ಉದ್ದಳತೆ % 8 != 0)
        {
          error (0, 0, _("invalid length: %s"), quote (digest_ಉದ್ದಳತೆ_str));
          error (EXIT_FAILURE, 0, _("length is not a multiple of 8"));
        }
    }
  ಆದರೆ (digest_ಉದ್ದಳತೆ == 0)
    {
# ಆದರೆ HASH_ALGO_BLAKE2
      digest_ಉದ್ದಳತೆ = DIGEST_MAX_LEN * 8;
# ಇಲ್ಲದಿದ್ದರೆ
      digest_ಉದ್ದಳತೆ = algorithm_ಬಿಟ್ಟುಗಳು[cksum_algorithm];
# endif
    }
  digest_hex_ಬೈಟುಗಳು = digest_ಉದ್ದಳತೆ >> 2;
#ಇಲ್ಲದಿದ್ದರೆ
  digest_hex_ಬೈಟುಗಳು = DIGEST_HEX_ಬೈಟುಗಳು;
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ HASH_ALGO_CKSUM
  ಆಯ್ಕೆ (+cksum_algorithm)
    {
    ಸ್ಥಿತಿ bsd:
    ಸ್ಥಿತಿ sysv:
    ಸ್ಥಿತಿ ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ:
    ಸ್ಥಿತಿ crc32b:
        ಆದರೆ (do_check && algorithm_specified)
          error (EXIT_FAILURE, 0,
                 _("--check is not supported with "
                   "--algorithm={bsd,sysv,crc,crc32b}"));
        ನಿಲ್ಲಿಸು;
    }

  ಆದರೆ (base64_digest && raw_digest)
   {
     error (0, 0, _("--base64 and --raw are mutually exclusive"));
     usage (EXIT_FAILURE);
   }
#ಆದರೆ_ಅಂತ್ಯ

  ಆದರೆ (digest_delim != '\n' && do_check)
    {
      error (0, 0, _("the --zero option is not supported when "
                     "verifying checksums"));
      usage (EXIT_FAILURE);
    }
  ಆದರೆ (1 <= ಪೂರ್ವಪ್ರತ್ಯಯ_ಟ್ಯಾಗ್ && do_check)
    {
      /* Note we allow --untagged with --check to more
         seamlessly support --untagged in an emulation wrapper.  */
      error (0, 0, _("the --tag option is meaningless when "
                     "verifying checksums"));
      usage (EXIT_FAILURE);
    }

  ಆದರೆ (0 <= ದ್ವಿಮಾನ && do_check)
    {
      error (0, 0, _("the --binary and --text options are meaningless when "
                     "verifying checksums"));
      usage (EXIT_FAILURE);
    }

  ಆದರೆ (ignore_missing && !do_check)
    {
      error (0, 0,
             _("the --ignore-missing option is meaningful only when "
               "verifying checksums"));
      usage (EXIT_FAILURE);
    }

  ಆದರೆ (ಸ್ಥಿತಿ_only && !do_check)
    {
      error (0, 0,
       _("the --status option is meaningful only when verifying checksums"));
      usage (EXIT_FAILURE);
    }

  ಆದರೆ (warn && !do_check)
    {
      error (0, 0,
       _("the --warn option is meaningful only when verifying checksums"));
      usage (EXIT_FAILURE);
    }

  ಆದರೆ (quiet && !do_check)
    {
      error (0, 0,
       _("the --quiet option is meaningful only when verifying checksums"));
      usage (EXIT_FAILURE);
    }

  ಆದರೆ (strict & !do_check)
   {
     error (0, 0,
        _("the --strict option is meaningful only when verifying checksums"));
     usage (EXIT_FAILURE);
   }

  ಆದರೆ (ಪೂರ್ವಪ್ರತ್ಯಯ_ಟ್ಯಾಗ್ == -1)
    ಪೂರ್ವಪ್ರತ್ಯಯ_ಟ್ಯಾಗ್ = HASH_ALGO_CKSUM;

  ಆದರೆ (ಪೂರ್ವಪ್ರತ್ಯಯ_ಟ್ಯಾಗ್ && !ದ್ವಿಮಾನ)
   {
     /* This could be supported in a backwards compatible way
        by prefixing the output line with a space in text mode.
        However that's invasive enough that it was agreed to
        not support this mode with --tag, as --text use cases
        are adequately supported by the default output format.  */
#ಆದರೆ !HASH_ALGO_CKSUM
     error (0, 0, _("--tag does not support --text mode"));
#ಇಲ್ಲದಿದ್ದರೆ
     error (0, 0, _("--text mode is only supported with --untagged"));
#ಆದರೆ_ಅಂತ್ಯ
     usage (EXIT_FAILURE);
   }

  ಆದರೆ (!O_ದ್ವಿಮಾನ && ದ್ವಿಮಾನ < 0)
    ದ್ವಿಮಾನ = 0;
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಪೂರ್ವಪ್ರತ್ಯಯ_ಟ್ಯಾಗ್)
    ದ್ವಿಮಾನ = 1;

  ಅಕ್ಷರ **ಕಾರ್ಯಾಂಶ_lim = argv + argc;
  ಆದರೆ (optind == argc)
    *ಕಾರ್ಯಾಂಶ_lim++ = bad_cast ("-");
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (1 < argc - optind && raw_digest)
    error (EXIT_FAILURE, 0,
           _("the --raw option is not supported with multiple files"));

  ಗಾಗಿ (ಅಕ್ಷರ **operandp = argv + optind; operandp < ಕಾರ್ಯಾಂಶ_lim; operandp++)
    {
      ಅಕ್ಷರ *file = *operandp;
      ಆದರೆ (do_check)
        ok &= digest_check (file);
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಪೂರ್ಣಾಂಕ ದ್ವಿಮಾನ_file = ದ್ವಿಮಾನ;
          bool missing;
          intmax_t ಉದ್ದಳತೆ;

          ಆದರೆ (! digest_file (file, &ದ್ವಿಮಾನ_file, bin_ಬಫರ್, &missing, &ಉದ್ದಳತೆ))
            ok = false;
          ಇಲ್ಲದಿದ್ದರೆ
            {
              DIGEST_ಹೊರಹೋಗು (file, ದ್ವಿಮಾನ_file, bin_ಬಫರ್, raw_digest, ಪೂರ್ವಪ್ರತ್ಯಯ_ಟ್ಯಾಗ್,
                          digest_delim, optind != argc, ಉದ್ದಳತೆ);
              ಆದರೆ (ferror (stdout))
                write_error ();
            }
        }
    }

  ಆದರೆ (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, _("standard input"));

  ಹಿಂತಿರುಗಿ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
