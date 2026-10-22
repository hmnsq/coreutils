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

#అన్తర్భావ <config.h>

#అన్తర్భావ <getopt.h>
#అన్తర్భావ <sys/types.h>

#అన్తర్భావ "assure.h"
#అన్తర్భావ "system.h"
#అన్తర్భావ "argmatch.h"
#అన్తర్భావ "c-ctype.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "xdectoint.h"
#అన్తర్భావ "xstrtol.h"

#నిర్వచితం_కాకపోతే HASH_ALGO_CKSUM
# define HASH_ALGO_CKSUM 0
#అయితే_ముగింపు

#ఒకవేళ HASH_ALGO_కూడిక || HASH_ALGO_CKSUM
# include "sum.h"
#అయితే_ముగింపు
#ఒకవేళ HASH_ALGO_CKSUM
# include "cksum.h"
# include "cksum_crc.h"
# include "base64.h"
#అయితే_ముగింపు
#ఒకవేళ HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# include "blake2/b2sum.h"
#అయితే_ముగింపు
#ఒకవేళ HASH_ALGO_MD5 || HASH_ALGO_CKSUM
# include "md5.h"
#అయితే_ముగింపు
#ఒకవేళ HASH_ALGO_SHA1 || HASH_ALGO_CKSUM
# include "sha1.h"
#అయితే_ముగింపు
#ఒకవేళ HASH_ALGO_SHA256 || HASH_ALGO_SHA224 || HASH_ALGO_CKSUM
# include "sha256.h"
#అయితే_ముగింపు
#ఒకవేళ HASH_ALGO_SHA512 || HASH_ALGO_SHA384 || HASH_ALGO_CKSUM
# include "sha512.h"
#అయితే_ముగింపు
#ఒకవేళ HASH_ALGO_CKSUM
# include "sha3.h"
#అయితే_ముగింపు
#ఒకవేళ HASH_ALGO_CKSUM
# include "sm3.h"
#అయితే_ముగింపు
#అన్తర్భావ "fadvise.h"
#అన్తర్భావ "stdio--.h"
#అన్తర్భావ "xbinary-io.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ఒకవేళ HASH_ALGO_కూడిక
# define కార్యక్రమం_NAME "sum"
# define DIGEST_TYPE_STRING "BSD"
# define DIGEST_ప్రవాహం sumfns[కూడిక_algorithm]
# define DIGEST_బహిర్గతం కూడిక_output_fns[కూడిక_algorithm]
# define DIGEST_బిట్లు 16
# define DIGEST_ALIGN 4
#లేదావేళ HASH_ALGO_CKSUM
# define MAX_DIGEST_బిట్లు 512
# define MAX_DIGEST_ALIGN 8
# define DIGEST_TYPE_STRING algorithm_ట్యాగులు[cksum_algorithm]
# define DIGEST_ప్రవాహం cksumfns[cksum_algorithm]
# define DIGEST_బహిర్గతం cksum_output_fns[cksum_algorithm]
# define DIGEST_బిట్లు MAX_DIGEST_బిట్లు
# define DIGEST_ALIGN MAX_DIGEST_ALIGN
#లేదావేళ HASH_ALGO_MD5
# define కార్యక్రమం_NAME "md5sum"
# define DIGEST_TYPE_STRING "MD5"
# define DIGEST_ప్రవాహం md5_ప్రవాహం
# define DIGEST_బిట్లు 128
# define DIGEST_REFERENCE "RFC 1321"
# define DIGEST_ALIGN 4
#లేదావేళ HASH_ALGO_BLAKE2
# define కార్యక్రమం_NAME "b2sum"
# define DIGEST_TYPE_STRING "BLAKE2b"
# define DIGEST_ప్రవాహం blake2b_ప్రవాహం
# define DIGEST_బిట్లు 512
# define DIGEST_REFERENCE "RFC 7693"
# define DIGEST_ALIGN 8
#లేదావేళ HASH_ALGO_SHA1
# define కార్యక్రమం_NAME "sha1sum"
# define DIGEST_TYPE_STRING "SHA1"
# define DIGEST_ప్రవాహం sha1_ప్రవాహం
# define DIGEST_బిట్లు 160
# define DIGEST_REFERENCE "FIPS-180-1"
# define DIGEST_ALIGN 4
#లేదావేళ HASH_ALGO_SHA256
# define కార్యక్రమం_NAME "sha256sum"
# define DIGEST_TYPE_STRING "SHA256"
# define DIGEST_ప్రవాహం sha256_ప్రవాహం
# define DIGEST_బిట్లు 256
# define DIGEST_REFERENCE "FIPS-180-2"
# define DIGEST_ALIGN 4
#లేదావేళ HASH_ALGO_SHA224
# define కార్యక్రమం_NAME "sha224sum"
# define DIGEST_TYPE_STRING "SHA224"
# define DIGEST_ప్రవాహం sha224_ప్రవాహం
# define DIGEST_బిట్లు 224
# define DIGEST_REFERENCE "RFC 3874"
# define DIGEST_ALIGN 4
#లేదావేళ HASH_ALGO_SHA512
# define కార్యక్రమం_NAME "sha512sum"
# define DIGEST_TYPE_STRING "SHA512"
# define DIGEST_ప్రవాహం sha512_ప్రవాహం
# define DIGEST_బిట్లు 512
# define DIGEST_REFERENCE "FIPS-180-2"
# define DIGEST_ALIGN 8
#లేదావేళ HASH_ALGO_SHA384
# define కార్యక్రమం_NAME "sha384sum"
# define DIGEST_TYPE_STRING "SHA384"
# define DIGEST_ప్రవాహం sha384_ప్రవాహం
# define DIGEST_బిట్లు 384
# define DIGEST_REFERENCE "FIPS-180-2"
# define DIGEST_ALIGN 8
#లేకపోతే
# error "Can't decide which hash algorithm to compile."
#అయితే_ముగింపు
#ఒకవేళ !HASH_ALGO_కూడిక && !HASH_ALGO_CKSUM
# define DIGEST_బహిర్గతం output_file
#అయితే_ముగింపు

#ఒకవేళ HASH_ALGO_CKSUM
# define కార్యక్రమం_NAME (!legacy_బహుళకం ? "cksum"			 \
                       : cksum_algorithm == md5 ? "md5sum"	 \
                       : cksum_algorithm == sha1 ? "sha1sum"	 \
                       : cksum_algorithm == sha224 ? "sha224sum" \
                       : cksum_algorithm == sha256 ? "sha256sum" \
                       : cksum_algorithm == sha384 ? "sha384sum" \
                       : cksum_algorithm == sha512 ? "sha512sum" \
                       : "cksum")
#అయితే_ముగింపు

#ఒకవేళ HASH_ALGO_కూడిక
# define AUTHORS \
  proper_name ("Kayvan Aghaiepour"), \
  proper_name ("David MacKenzie")
#లేదావేళ HASH_ALGO_CKSUM
# define AUTHORS \
  proper_name_lite ("Padraig Brady", "P\303\241draig Brady"), \
  proper_name ("Q. Frank Xia")
#లేదావేళ HASH_ALGO_BLAKE2
# define AUTHORS \
  proper_name_lite ("Padraig Brady", "P\303\241draig Brady"), \
  proper_name ("Samuel Neves")
#లేకపోతే
# define AUTHORS \
  proper_name ("Ulrich Drepper"), \
  proper_name ("Scott Miller"), \
  proper_name ("David Madore")
#అయితే_ముగింపు
#ఒకవేళ !HASH_ALGO_BLAKE2 && !HASH_ALGO_CKSUM
# define DIGEST_HEX_బైట్లు (DIGEST_బిట్లు / 4)
#అయితే_ముగింపు
#నిర్వచించు DIGEST_BIN_బైట్లు (DIGEST_బిట్లు / 8)

/* The minimum length of a valid digest line.  This length does
   not include any newline character at the end of a line.  */
#ఒకవేళ HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# define MIN_DIGEST_పంక్తి_నిడివి 3 /* With -l 8.  */
#లేకపోతే
# define MIN_DIGEST_పంక్తి_నిడివి \
   (DIGEST_HEX_బైట్లు /* length of hexadecimal message digest */ \
    + 1 /* blank */ \
    + 1 /* minimum filename length */ )
#అయితే_ముగింపు

#ఒకవేళ !HASH_ALGO_కూడిక
స్థిర శూన్య
output_file (అక్షరం స్థిరరాశి *file, పూర్ణాంకం ద్విమాన_file, శూన్య స్థిరరాశి *digest,
             bool raw, bool tagged, గుర్తులేని అక్షరం delim, bool వాదనలు,
             intmax_t నిడివి);
#అయితే_ముగింపు

/* True if any of the files read were the standard input. */
స్థిర bool have_read_stdin;

/* The minimum length of a valid checksum line for the selected algorithm.  */
స్థిర idx_t min_digest_పంక్తి_నిడివి;

/* Set to the length of a digest hex string for the selected algorithm.  */
స్థిర idx_t digest_hex_బైట్లు;

/* With --check, don't generate any output.
   The exit code indicates success or failure.  */
స్థిర bool స్థితి_only = false;

/* With --check, print a message to standard error warning about each
   improperly formatted checksum line.  */
స్థిర bool warn = false;

/* With --check, ignore missing files.  */
స్థిర bool ignore_missing = false;

/* With --check, suppress the "OK" printed for each verified file.  */
స్థిర bool quiet = false;

/* With --check, exit with a non-zero return code if any line is
   improperly formatted. */
స్థిర bool strict = false;

/* Whether a BSD reversed format checksum is detected.  */
స్థిర పూర్ణాంకం bsd_reversed = -1;

/* line delimiter.  */
స్థిర గుర్తులేని అక్షరం digest_delim = '\n';

#ఒకవేళ HASH_ALGO_CKSUM
/* If true, print base64-encoded digests, not hex.  */
స్థిర bool base64_digest = false;
#అయితే_ముగింపు

/* If true, print binary digests, not hex.  */
స్థిర bool raw_digest = false;

/* blake2b and sha3 allow the -l option.  Luckily they both have the same
   maximum digest size.  */
#ఒకవేళ HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# ఒకవేళ HASH_ALGO_CKSUM
static_assert (BLAKE2B_OUTBYTES == SHA3_512_DIGEST_కొలత);
# endif
# define DIGEST_MAX_LEN BLAKE2B_OUTBYTES
స్థిర idx_t digest_నిడివి;
#అయితే_ముగింపు /* HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM */

రకనిర్వచనం శూన్య (*digest_output_fn)(అక్షరం స్థిరరాశి *, పూర్ణాంకం, శూన్య స్థిరరాశి *, bool,
                                 bool, గుర్తులేని అక్షరం, bool, intmax_t);
#ఒకవేళ HASH_ALGO_కూడిక
గణన Algorithm
{
  bsd,
  sysv,
};

స్థిర గణన Algorithm కూడిక_algorithm;
స్థిర sumfn sumfns[]=
{
  bsd_కూడిక_ప్రవాహం,
  sysv_కూడిక_ప్రవాహం,
};
స్థిర digest_output_fn కూడిక_output_fns[]=
{
  output_bsd,
  output_sysv,
};
#అయితే_ముగింపు

#ఒకవేళ HASH_ALGO_CKSUM
స్థిర పూర్ణాంకం
md5_కూడిక_ప్రవాహం (FILE *ప్రవాహం, శూన్య *resstream, MAYBE_వాడని intmax_t *నిడివి)
{
  తిరిగి md5_ప్రవాహం (ప్రవాహం, resstream);
}
స్థిర పూర్ణాంకం
sha1_కూడిక_ప్రవాహం (FILE *ప్రవాహం, శూన్య *resstream, MAYBE_వాడని intmax_t *నిడివి)
{
  తిరిగి sha1_ప్రవాహం (ప్రవాహం, resstream);
}
స్థిర పూర్ణాంకం
sha224_కూడిక_ప్రవాహం (FILE *ప్రవాహం, శూన్య *resstream,
                   MAYBE_వాడని intmax_t *నిడివి)
{
  తిరిగి sha224_ప్రవాహం (ప్రవాహం, resstream);
}
స్థిర పూర్ణాంకం
sha256_కూడిక_ప్రవాహం (FILE *ప్రవాహం, శూన్య *resstream,
                   MAYBE_వాడని intmax_t *నిడివి)
{
  తిరిగి sha256_ప్రవాహం (ప్రవాహం, resstream);
}
స్థిర పూర్ణాంకం
sha384_కూడిక_ప్రవాహం (FILE *ప్రవాహం, శూన్య *resstream,
                   MAYBE_వాడని intmax_t *నిడివి)
{
  తిరిగి sha384_ప్రవాహం (ప్రవాహం, resstream);
}
స్థిర పూర్ణాంకం
sha512_కూడిక_ప్రవాహం (FILE *ప్రవాహం, శూన్య *resstream,
                   MAYBE_వాడని intmax_t *నిడివి)
{
  తిరిగి sha512_ప్రవాహం (ప్రవాహం, resstream);
}
స్థిర పూర్ణాంకం
sha2_కూడిక_ప్రవాహం (FILE *ప్రవాహం, శూన్య *resstream, intmax_t *నిడివి)
{
  ఎంపిక (*నిడివి)
    {
    స్థితి SHA224_DIGEST_కొలత:
      తిరిగి sha224_ప్రవాహం (ప్రవాహం, resstream);
    స్థితి SHA256_DIGEST_కొలత:
      తిరిగి sha256_ప్రవాహం (ప్రవాహం, resstream);
    స్థితి SHA384_DIGEST_కొలత:
      తిరిగి sha384_ప్రవాహం (ప్రవాహం, resstream);
    స్థితి SHA512_DIGEST_కొలత:
      తిరిగి sha512_ప్రవాహం (ప్రవాహం, resstream);
    అప్రమేయ:
      affirm (false);
    }
}
స్థిర పూర్ణాంకం
sha3_కూడిక_ప్రవాహం (FILE *ప్రవాహం, శూన్య *resstream, intmax_t *నిడివి)
{
  ఎంపిక (*నిడివి)
    {
    స్థితి SHA3_224_DIGEST_కొలత:
      తిరిగి sha3_224_ప్రవాహం (ప్రవాహం, resstream);
    స్థితి SHA3_256_DIGEST_కొలత:
      తిరిగి sha3_256_ప్రవాహం (ప్రవాహం, resstream);
    స్థితి SHA3_384_DIGEST_కొలత:
      తిరిగి sha3_384_ప్రవాహం (ప్రవాహం, resstream);
    స్థితి SHA3_512_DIGEST_కొలత:
      తిరిగి sha3_512_ప్రవాహం (ప్రవాహం, resstream);
    అప్రమేయ:
      affirm (false);
    }
}
స్థిర పూర్ణాంకం
blake2b_కూడిక_ప్రవాహం (FILE *ప్రవాహం, శూన్య *resstream, intmax_t *నిడివి)
{
  తిరిగి blake2b_ప్రవాహం (ప్రవాహం, resstream, *నిడివి);
}
స్థిర పూర్ణాంకం
sm3_కూడిక_ప్రవాహం (FILE *ప్రవాహం, శూన్య *resstream, MAYBE_వాడని intmax_t *నిడివి)
{
  తిరిగి sm3_ప్రవాహం (ప్రవాహం, resstream);
}

స్థిర అక్షరం స్థిరరాశి *స్థిరరాశి algorithm_వాదనలు[] =
{
  "bsd", "sysv", "crc", "crc32b", "md5", "sha1",
  "sha224", "sha256", "sha384", "sha512", /* Legacy naming */
  "sha2", "sha3", "blake2b", "sm3", ఖాళీ
};
స్థిర గణన Algorithm స్థిరరాశి algorithm_types[] =
{
  bsd, sysv, చక్రీయ_తనిఖీ, crc32b, md5, sha1,
  sha224, sha256, sha384, sha512,
  sha2, sha3, blake2b, sm3,
};
ARGMATCH_VERIFY (algorithm_వాదనలు, algorithm_types);

స్థిర అక్షరం స్థిరరాశి *స్థిరరాశి algorithm_ట్యాగులు[] =
{
  "BSD", "SYSV", "CRC", "CRC32B", "MD5", "SHA1",
  "SHA224", "SHA256", "SHA384", "SHA512",
  "SHA2", "SHA3", "BLAKE2b", "SM3", ఖాళీ
};
స్థిర పూర్ణాంకం స్థిరరాశి algorithm_బిట్లు[] =
{
  16, 16, 32, 32, 128, 160,
  224, 256, 384, 512,
  512, 512, 512, 256, 0
};

static_assert (countof (algorithm_బిట్లు) == countof (algorithm_వాదనలు));

స్థిర bool algorithm_specified = false;
స్థిర bool legacy_బహుళకం = false;
గణన Algorithm cksum_algorithm = చక్రీయ_తనిఖీ;

స్థిర sumfn cksumfns[]=
{
  bsd_కూడిక_ప్రవాహం,
  sysv_కూడిక_ప్రవాహం,
  చక్రీయ_తనిఖీ_కూడిక_ప్రవాహం,
  crc32b_కూడిక_ప్రవాహం,
  md5_కూడిక_ప్రవాహం,
  sha1_కూడిక_ప్రవాహం,
  sha224_కూడిక_ప్రవాహం,
  sha256_కూడిక_ప్రవాహం,
  sha384_కూడిక_ప్రవాహం,
  sha512_కూడిక_ప్రవాహం,
  sha2_కూడిక_ప్రవాహం,
  sha3_కూడిక_ప్రవాహం,
  blake2b_కూడిక_ప్రవాహం,
  sm3_కూడిక_ప్రవాహం,
};
స్థిర digest_output_fn cksum_output_fns[]=
{
  output_bsd,
  output_sysv,
  output_చక్రీయ_తనిఖీ,
  output_చక్రీయ_తనిఖీ,
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
#అయితే_ముగింపు

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */

గణన
{
  IGNORE_MISSING_OPTION = CHAR_MAX + 1,
  స్థితి_OPTION,
  QUIET_OPTION,
  STRICT_OPTION,
  ట్యాగ్_OPTION,
  UNTAG_OPTION,
  DEBUG_కార్యక్రమం_OPTION,
  RAW_OPTION,
  BASE64_OPTION,
};

#ఒకవేళ HASH_ALGO_CKSUM
స్థిర నిర్మాణం option స్థిరరాశి legacy_long_options[] =
{
  { "check", no_వాదం, ఖాళీ, 'c' },
  { "ignore-missing", no_వాదం, ఖాళీ, IGNORE_MISSING_OPTION },
  { "quiet", no_వాదం, ఖాళీ, QUIET_OPTION },
  { "status", no_వాదం, ఖాళీ, స్థితి_OPTION },
  { "warn", no_వాదం, ఖాళీ, 'w' },
  { "strict", no_వాదం, ఖాళీ, STRICT_OPTION },
  { "tag", no_వాదం, ఖాళీ, ట్యాగ్_OPTION },
  { "zero", no_వాదం, ఖాళీ, 'z' },

  { "binary", no_వాదం, ఖాళీ, 'b' }, /* Deprecated.  */
  { "text", no_వాదం, ఖాళీ, 't' }, /* Deprecated.  */

  { GETOPT_HELP_OPTION_DECL },
  { GETOPT_సంస్కరణ_OPTION_DECL },
  { ఖాళీ, 0, ఖాళీ, 0 }
};
#అయితే_ముగింపు

స్థిర నిర్మాణం option స్థిరరాశి long_options[] =
{
#ఒకవేళ HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
  { "length", required_వాదం, ఖాళీ, 'l'},
#అయితే_ముగింపు

#ఒకవేళ !HASH_ALGO_కూడిక
  { "check", no_వాదం, ఖాళీ, 'c' },
  { "ignore-missing", no_వాదం, ఖాళీ, IGNORE_MISSING_OPTION},
  { "quiet", no_వాదం, ఖాళీ, QUIET_OPTION },
  { "status", no_వాదం, ఖాళీ, స్థితి_OPTION },
  { "warn", no_వాదం, ఖాళీ, 'w' },
  { "strict", no_వాదం, ఖాళీ, STRICT_OPTION },
  { "tag", no_వాదం, ఖాళీ, ట్యాగ్_OPTION },
  { "zero", no_వాదం, ఖాళీ, 'z' },

# ఒకవేళ HASH_ALGO_CKSUM
  { "algorithm", required_వాదం, ఖాళీ, 'a'},
  { "base64", no_వాదం, ఖాళీ, BASE64_OPTION },
  { "debug", no_వాదం, ఖాళీ, DEBUG_కార్యక్రమం_OPTION},
  { "raw", no_వాదం, ఖాళీ, RAW_OPTION},
  { "untagged", no_వాదం, ఖాళీ, UNTAG_OPTION },
# endif
  { "binary", no_వాదం, ఖాళీ, 'b' }, /* Deprecated.  */
  { "text", no_వాదం, ఖాళీ, 't' }, /* Deprecated.  */

#లేకపోతే
  {"sysv", no_వాదం, ఖాళీ, 's'},  /* Not in cksum.  */
#అయితే_ముగింపు

  { GETOPT_HELP_OPTION_DECL },
  { GETOPT_సంస్కరణ_OPTION_DECL },
  { ఖాళీ, 0, ఖాళీ, 0 }
};

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("\
Usage: %s [OPTION]... [FILE]...\n\
"), కార్యక్రమం_name);
#ఒకవేళ HASH_ALGO_CKSUM
    ఒకవేళ (legacy_బహుళకం)
      {
        రాయి (_("\
Print or check %s checksums.\n\
"), DIGEST_TYPE_STRING);
        fputs (_("\
Legacy interface గమ్యం the cksum(1) utility.\n\
"), stdout);
      }
    లేకపోతే
      fputs (_("\
Print or verify checksums.\n\
By అప్రమేయ use the 32 బిట్టు చక్రీయ_తనిఖీ_2 algorithm.\n\
"), stdout);
#లేకపోతే
      రాయి (_("\
Print or check %s (%d-బిట్టు) checksums.\n\
"),
              DIGEST_TYPE_STRING,
              DIGEST_బిట్లు);
      fputs (_("\
Legacy interface గమ్యం the cksum(1) utility.\n\
"), stdout);
#అయితే_ముగింపు

      emit_stdin_note ();
#ఒకవేళ HASH_ALGO_కూడిక
      oputs (_("\
  -r\n\
         use BSD కూడిక algorithm (the అప్రమేయ), use 1K ఖండాలు\n\
"));
      oputs (_("\
  -s, --sysv\n\
         use System V కూడిక algorithm, use 512 బైట్లు ఖండాలు\n\
"));
#అయితే_ముగింపు
#ఒకవేళ HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
        emit_mandatory_arg_note ();
#అయితే_ముగింపు
#ఒకవేళ HASH_ALGO_CKSUM
     ఒకవేళ (! legacy_బహుళకం)
        oputs (_("\
  -a, --algorithm=TYPE\n\
         select the digest type గమ్యం use.  See DIGEST క్రింద\n\
"));
     ఒకవేళ (! legacy_బహుళకం)
        oputs (_("\
      --base64\n\
         emit base64-encoded digests, not hexadecimal\n\
"));
#అయితే_ముగింపు
#ఒకవేళ !HASH_ALGO_కూడిక
# ఒకవేళ HASH_ALGO_CKSUM
     ఒకవేళ (legacy_బహుళకం)
       {
# endif
      ఒకవేళ (O_ద్విమాన)
        oputs (_("\
  -b, --ద్విమాన\n\
         పఠ లోపల ద్విమాన బహుళకం_2 (అప్రమేయ unless reading tty stdin)\n\
"));
      లేకపోతే
        oputs (_("\
  -b, --ద్విమాన\n\
         పఠ లోపల ద్విమాన బహుళకం_2\n\
"));
# ఒకవేళ HASH_ALGO_CKSUM
       }
# endif
        oputs (_("\
  -c, --check\n\
         పఠ checksums from the FILEs and check them\n\
"));
# ఒకవేళ HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
#  ఒకవేళ HASH_ALGO_CKSUM
     ఒకవేళ (! legacy_బహుళకం)
#  endif
        oputs (_("\
  -l, --నిడివి=బిట్లు\n\
         digest నిడివి లోపల బిట్లు_2; must not exceed the max కొలత\n\
         and must be a గుణిజం యొక్క 8 కొరకు blake2b;\n\
         must be 224, 256, 384, or 512 కొరకు sha2 or sha3\n\
"));
# endif
# ఒకవేళ HASH_ALGO_CKSUM
    ఒకవేళ (! legacy_బహుళకం)
        oputs (_("\
      --raw\n\
         emit a raw ద్విమాన digest, not hexadecimal\n\
"));
    ఒకవేళ (legacy_బహుళకం)
      oputs (_("\
      --ట్యాగ్\n\
         create a BSD-style checksum\n\
"));
    లేకపోతే
      oputs (_("\
      --ట్యాగ్\n\
         create a BSD-style checksum (the అప్రమేయ)\n\
"));
    ఒకవేళ (! legacy_బహుళకం)
      oputs (_("\
      --untagged\n\
         create a reversed style checksum, without digest type\n\
"));
# లేకపోతే
      oputs (_("\
      --ట్యాగ్\n\
         create a BSD-style checksum\n\
"));
# endif
# ఒకవేళ HASH_ALGO_CKSUM
    ఒకవేళ (legacy_బహుళకం)
      {
# endif
      ఒకవేళ (O_ద్విమాన)
        oputs (_("\
  -t, --పాఠ్యం\n\
         పఠ లోపల పాఠ్యం బహుళకం_2 (అప్రమేయ ఒకవేళ reading tty stdin)\n\
"));
      లేకపోతే
        oputs (_("\
  -t, --పాఠ్యం\n\
         పఠ లోపల పాఠ్యం బహుళకం_2 (అప్రమేయ)\n\
"));
# ఒకవేళ HASH_ALGO_CKSUM
      }
# endif
      oputs (_("\
  -z, --zero\n\
         చివర each output పంక్తి with NUL, not newline,\n\
         and disable file name escaping\n\
"));
      fputs (_("\
\n\
The following five options are useful only when verifying checksums:\n\
"), stdout);
      oputs (_("\
      --ignore-missing\n\
         don't fail or report స్థితి_2 కొరకు missing files\n\
"));
      oputs (_("\
      --quiet\n\
         don't print OK కొరకు each successfully verified file\n\
"));
      oputs (_("\
      --స్థితి_2\n\
         don't output anything, స్థితి_2 సంకేతం_2 shows success\n\
"));
      oputs (_("\
      --strict\n\
         నిష్క్రమించు non-zero కొరకు improperly formatted checksum పంక్తులు\n\
"));
      oputs (_("\
  -w, --warn\n\
         warn about improperly formatted checksum పంక్తులు\n\
"));
#అయితే_ముగింపు
#ఒకవేళ HASH_ALGO_CKSUM
    ఒకవేళ (! legacy_బహుళకం)
      oputs (_("\
      --debug\n\
         indicate which implementation వాడిన\n\
"));
#అయితే_ముగింపు
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
#ఒకవేళ HASH_ALGO_CKSUM
    ఒకవేళ (! legacy_బహుళకం)
      fputs (_("\
\n\
DIGEST determines the digest algorithm and అప్రమేయ output format:\n\
  sysv      (equivalent గమ్యం కూడిక -s)\n\
  bsd       (equivalent గమ్యం కూడిక -r)\n\
  చక్రీయ_తనిఖీ       (equivalent గమ్యం cksum)\n\
  crc32b    (only అందుబాటు through cksum)\n\
  md5       (equivalent గమ్యం md5sum)\n\
  sha1      (equivalent గమ్యం sha1sum)\n\
  sha2      (equivalent గమ్యం sha{224,256,384,512}కూడిక)\n\
  sha3      (only అందుబాటు through cksum)\n\
  blake2b   (equivalent గమ్యం b2sum)\n\
  sm3       (only అందుబాటు through cksum)\n\
\n"), stdout);
#అయితే_ముగింపు
#ఒకవేళ !HASH_ALGO_కూడిక && !HASH_ALGO_CKSUM
      రాయి (_("\
\n\
The sums are computed as described లోపల %s.\n"), DIGEST_REFERENCE);
      fputs (_("\
When checking, the ఆదానం should be a former output యొక్క this కార్యక్రమం.\n\
The అప్రమేయ బహుళకం_2 is గమ్యం print a పంక్తి with: checksum, a space,\n\
a character indicating ఆదానం బహుళకం_2 ('*' కొరకు ద్విమాన, ' ' కొరకు పాఠ్యం\n\
or where ద్విమాన is insignificant), and name కొరకు each FILE.\n\
\n\
There is no difference between ద్విమాన బహుళకం_2 and పాఠ్యం బహుళకం_2 on GNU systems.\
\n"), stdout);
#అయితే_ముగింపు
#ఒకవేళ HASH_ALGO_CKSUM
      fputs (_("\
When checking, the ఆదానం should be a former output యొక్క this కార్యక్రమం,\n\
or equivalent standalone కార్యక్రమం.\
\n"), stdout);
#అయితే_ముగింపు
      emit_ancillary_info (కార్యక్రమం_NAME);
    }

  నిష్క్రమించు (స్థితి_2);
}

#ఒకవేళ !HASH_ALGO_కూడిక
/* Given a string S, return TRUE if it contains problematic characters
   that need escaping.  Note we escape '\' itself to provide some forward
   compat to introduce escaping of other characters.  */

ATTRIBUTE_శుద్ధ
స్థిర bool
problematic_chars (అక్షరం స్థిరరాశి *s)
{
  idx_t నిడివి = strcspn (s, "\\\n\r");
  తిరిగి s[నిడివి] != '\0';
}
#అయితే_ముగింపు

/* Given a file name, S of length S_LEN, that is not NUL-terminated,
   modify it in place, performing the equivalent of this sed substitution:
   's/\\n/\n/g;s/\\r/\r/g;s/\\\\/\\/g' i.e., replacing each "\\n" string
   with a newline, each "\\r" string with a carriage return,
   and each "\\\\" with a single backslash, NUL-terminate it and return S.
   If S is not a valid escaped file name, i.e., if it ends with an odd number
   of backslashes or if it contains a backslash followed by anything other
   than "n" or another backslash, return NULL.  */

స్థిర అక్షరం *
filename_unescape (అక్షరం *s, idx_t s_len)
{
  అక్షరం *dst = s;

  కొరకు (idx_t i = 0; i < s_len; i++)
    {
      ఎంపిక (s[i])
        {
        స్థితి '\\':
          ఒకవేళ (i == s_len - 1)
            {
              /* File name ends with an unescaped backslash: invalid.  */
              తిరిగి ఖాళీ;
            }
          ++i;
          ఎంపిక (s[i])
            {
            స్థితి 'n':
              *dst++ = '\n';
              ఆపు;
            స్థితి 'r':
              *dst++ = '\r';
              ఆపు;
            స్థితి '\\':
              *dst++ = '\\';
              ఆపు;
            అప్రమేయ:
              /* Only '\', 'n' or 'r' may follow a backslash.  */
              తిరిగి ఖాళీ;
            }
          ఆపు;

        స్థితి '\0':
          /* The file name may not contain a NUL.  */
          తిరిగి ఖాళీ;

        అప్రమేయ:
          *dst++ = s[i];
          ఆపు;
        }
    }
  ఒకవేళ (dst < s + s_len)
    *dst = '\0';

  తిరిగి s;
}

/* Return true if S is a LEN-byte NUL-terminated string of hex or base64
   digits and has the expected length.  Otherwise, return false.  */
ATTRIBUTE_శుద్ధ
స్థిర bool
valid_digits (గుర్తులేని అక్షరం స్థిరరాశి *s, idx_t len)
{
#ఒకవేళ HASH_ALGO_CKSUM
  ఒకవేళ (len == BASE64_నిడివి (digest_నిడివి >> 3))
    {
      idx_t i;
      కొరకు (i = 0; i < len - digest_నిడివి % 3; i++)
        {
          ఒకవేళ (!isbase64 (*s))
            తిరిగి false;
          ++s;
        }
      కొరకు ( ; i < len; i++)
        {
          ఒకవేళ (*s != '=')
            తిరిగి false;
          ++s;
        }
    }
  లేకపోతే
#అయితే_ముగింపు
  ఒకవేళ (len == digest_hex_బైట్లు)
    {
      కొరకు (idx_t i = 0; i < digest_hex_బైట్లు; i++)
        {
          ఒకవేళ (!c_isxdigit (*s))
            తిరిగి false;
          ++s;
        }
    }
  లేకపోతే
    తిరిగి false;

  తిరిగి *s == '\0';
}

/* Split the checksum string S (of length S_LEN) from a BSD 'md5' or
   'sha1' command into two parts: a hexadecimal digest, and the file
   name.  S is modified.  Set *D_LEN to the length of the digest string.
   Return true if successful.  */

స్థిర bool
bsd_విభజించు_3 (అక్షరం *s, idx_t s_len,
             గుర్తులేని అక్షరం **digest, idx_t *d_len,
             అక్షరం **file_name, bool escaped_filename)
{
  ఒకవేళ (s_len == 0)
    తిరిగి false;

  /* Find end of filename.  */
  idx_t i = s_len - 1;
  అయితే (i && s[i] != ')')
    i--;

  ఒకవేళ (s[i] != ')')
    తిరిగి false;

  *file_name = s;

  ఒకవేళ (escaped_filename && filename_unescape (s, i) == ఖాళీ)
    తిరిగి false;

  s[i++] = '\0';

  అయితే (c_isblank (s[i]))
    i++;

  ఒకవేళ (s[i] != '=')
    తిరిగి false;

  i++;

  అయితే (c_isblank (s[i]))
    i++;

  *digest = (గుర్తులేని అక్షరం *) &s[i];

  *d_len = s_len - i;
  తిరిగి valid_digits (*digest, *d_len);
}

#ఒకవేళ HASH_ALGO_CKSUM
/* Return the corresponding Algorithm for the string S,
   or -1 for no match.  */

స్థిర ptrdiff_t
algorithm_from_ట్యాగ్ (అక్షరం *s)
{
  /* Limit check size to this length for perf reasons.  */
  స్థిర idx_t max_ట్యాగ్_len;
  ఒకవేళ (! max_ట్యాగ్_len)
    {
      అక్షరం స్థిరరాశి * స్థిరరాశి * ట్యాగ్ = algorithm_ట్యాగులు;
      అయితే (*ట్యాగ్)
        {
          idx_t ట్యాగ్_len = అక్షరశ్రేణి_పొడవు (*ట్యాగ్++);
          max_ట్యాగ్_len = MAX (ట్యాగ్_len, max_ట్యాగ్_len);
        }
    }

  idx_t i = 0;

  /* Find end of tag */
  అయితే (i <= max_ట్యాగ్_len && s[i] && ! c_isblank (s[i])
         && s[i] != '-' && s[i] != '(')
    ++i;

  ఒకవేళ (i > max_ట్యాగ్_len)
    తిరిగి -1;

  /* Terminate tag, and lookup.  */
  అక్షరం sep = s[i];
  s[i] = '\0';
  ptrdiff_t algo = argmatch_exact (s, algorithm_ట్యాగులు);
  s[i] = sep;

  తిరిగి algo;
}
#అయితే_ముగింపు

/* Split the string S (of length S_LEN) into three parts:
   a hexadecimal digest, binary flag, and the file name.
   S is modified.  Set *D_LEN to the length of the digest string.
   Return true if successful.  */

స్థిర bool
విభజించు_3 (అక్షరం *s, idx_t s_len,
         గుర్తులేని అక్షరం **digest, idx_t *d_len, పూర్ణాంకం *ద్విమాన, అక్షరం **file_name)
{
  bool escaped_filename = false;
  idx_t algo_name_len;

  idx_t i = 0;
  అయితే (c_isblank (s[i]))
    ++i;

  ఒకవేళ (s[i] == '\\')
    {
      ++i;
      escaped_filename = true;
    }

  /* Check for BSD-style checksum line. */

#ఒకవేళ HASH_ALGO_CKSUM
  ఒకవేళ (! algorithm_specified || cksum_algorithm == sha2)
    {
      ptrdiff_t algo_ట్యాగ్ = algorithm_from_ట్యాగ్ (s + i);
      ఒకవేళ (! algorithm_specified)
        {
          ఒకవేళ (algo_ట్యాగ్ >= 0)
            {
              ఒకవేళ (algo_ట్యాగ్ <= crc32b)
                తిరిగి false;  /* We don't support checking these formats.  */
              cksum_algorithm = algo_ట్యాగ్;
            }
          లేకపోతే
            తిరిగి false;      /* We only support tagged format without -a.  */
        }
      లేకపోతే
        {
          ఒకవేళ (cksum_algorithm == sha2 && (algo_ట్యాగ్ == sha2
              || algo_ట్యాగ్ == sha224 || algo_ట్యాగ్ == sha256
              || algo_ట్యాగ్ == sha384 || algo_ట్యాగ్ == sha512))
            cksum_algorithm = algo_ట్యాగ్;
        }
    }
#అయితే_ముగింపు

  /* Try to parse BSD or OpenSSL tagged format.  I.e.:
     openssl: MD5(f)= d41d8cd98f00b204e9800998ecf8427e
     bsd:     MD5 (f) = d41d8cd98f00b204e9800998ecf8427e  */

  idx_t parse_స్థానభ్రంశం = i;
  algo_name_len = అక్షరశ్రేణి_పొడవు (DIGEST_TYPE_STRING);
  ఒకవేళ (STREQ_LEN (s + i, DIGEST_TYPE_STRING, algo_name_len))
    {
      i += algo_name_len;
#ఒకవేళ HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM

# ఒకవేళ HASH_ALGO_BLAKE2
      digest_నిడివి = DIGEST_MAX_LEN * 8;
# లేకపోతే
      digest_నిడివి = algorithm_బిట్లు[cksum_algorithm];
# endif
      ఒకవేళ (s[i] == '-')  /* length specified. Not base64 */
        {
          ++i;
          intmax_t నిడివి;
          అక్షరం *siend;
          ఒకవేళ (!c_isdigit (s[i])
              || xstrtoimax (s + i, &siend, 0, &నిడివి, ఖాళీ) != LONGINT_OK)
            తిరిగి false;
# ఒకవేళ HASH_ALGO_CKSUM
          లేకపోతే ఒకవేళ (cksum_algorithm == sha2 || cksum_algorithm == sha3)
            {
              ఒకవేళ (నిడివి != SHA224_DIGEST_కొలత * 8
                  && నిడివి != SHA256_DIGEST_కొలత * 8
                  && నిడివి != SHA384_DIGEST_కొలత * 8
                  && నిడివి != SHA512_DIGEST_కొలత * 8)
                తిరిగి false;
            }
# endif
          లేకపోతే ఒకవేళ (!(0 < నిడివి && నిడివి <= digest_నిడివి && నిడివి % 8 == 0))
            తిరిగి false;

          i = siend - s;
          digest_నిడివి = నిడివి;
        }
      digest_hex_బైట్లు = digest_నిడివి >> 2;
#అయితే_ముగింపు
      ఒకవేళ (s[i] == ' ')
        ++i;
      ఒకవేళ (s[i] == '(')  /* not base64 */
        {
          ++i;
          *ద్విమాన = 0;
          తిరిగి bsd_విభజించు_3 (s + i, s_len - i,
                              digest, d_len, file_name, escaped_filename);
        }

      /* Note with --base64 --untagged format, we may have matched a "tag".
         Even very short digests with: cksum -a blake2b -l24 --untagged --base64
         So fallback to checking untagged format if issues detecting tags.  */
      i = parse_స్థానభ్రంశం;
    }

  /* Ignore this line if it is too short.
     Each line must have at least 'min_digest_line_length - 1' (or one more, if
     the first is a backslash) more characters to contain correct message digest
     information.  */
  ఒకవేళ (s_len - i < min_digest_పంక్తి_నిడివి + (s[i] == '\\'))
    తిరిగి false;

  *digest = (గుర్తులేని అక్షరం *) &s[i];

#ఒకవేళ HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
  /* Auto determine length.  */
# ఒకవేళ HASH_ALGO_CKSUM
  ఒకవేళ (cksum_algorithm == blake2b
      || cksum_algorithm == sha2 || cksum_algorithm == sha3) {
# endif
  గుర్తులేని అక్షరం స్థిరరాశి *hp = *digest;
  digest_hex_బైట్లు = 0;
  కొరకు (; c_isxdigit (*hp); ++hp, ++digest_hex_బైట్లు)
    ;
# ఒకవేళ HASH_ALGO_CKSUM
  /* Check the number of base64 characters.  This works because the hexadecimal
     character set is a subset of the base64 character set.
     Note there is the ambiguity that all characters are hex when they
     are actually base64 encoded, which could be ambiguous with:
        cksum -a sha2 -l 384 --base64 --untagged
        cksum -a sha2 -l 256 --untagged
     Similarly for sha3 and blake2b.
     However at this length the chances are exceedingly rare (1 in 480R),
     and smaller blake2b lengths aren't practical for verification anyway.  */
  idx_t digest_base64_బైట్లు = digest_hex_బైట్లు;
  idx_t trailing_equals = 0;
  కొరకు (; isubase64 (*hp); ++hp, ++digest_base64_బైట్లు)
    ;
  కొరకు (; *hp == '='; ++hp, ++trailing_equals)
    ;
  ఒకవేళ ((cksum_algorithm == sha2 || cksum_algorithm == sha3)
      && digest_hex_బైట్లు >> 1 != SHA224_DIGEST_కొలత
      && digest_hex_బైట్లు >> 1 != SHA256_DIGEST_కొలత
      && digest_hex_బైట్లు >> 1 != SHA384_DIGEST_కొలత
      && digest_hex_బైట్లు >> 1 != SHA512_DIGEST_కొలత)
    {
      ఒకవేళ (digest_base64_బైట్లు + trailing_equals
          == BASE64_నిడివి (SHA224_DIGEST_కొలత))
        digest_hex_బైట్లు = SHA224_DIGEST_కొలత * 2;
      లేకపోతే ఒకవేళ (digest_base64_బైట్లు + trailing_equals
               == BASE64_నిడివి (SHA256_DIGEST_కొలత))
        digest_hex_బైట్లు = SHA256_DIGEST_కొలత * 2;
      లేకపోతే ఒకవేళ (digest_base64_బైట్లు + trailing_equals
               == BASE64_నిడివి (SHA384_DIGEST_కొలత))
        digest_hex_బైట్లు = SHA384_DIGEST_కొలత * 2;
      లేకపోతే ఒకవేళ (digest_base64_బైట్లు + trailing_equals
               == BASE64_నిడివి (SHA512_DIGEST_కొలత))
        digest_hex_బైట్లు = SHA512_DIGEST_కొలత * 2;
      లేకపోతే
        తిరిగి false;
    }
  లేకపోతే ఒకవేళ (cksum_algorithm == blake2b
           && digest_hex_బైట్లు < digest_base64_బైట్లు)
    {
      కొరకు (పూర్ణాంకం j = 8; j <= DIGEST_MAX_LEN * 8; j += 8)
        {
          ఒకవేళ (BASE64_నిడివి (j / 8) == digest_base64_బైట్లు + trailing_equals
              && j % 3 == trailing_equals)
            {
              digest_hex_బైట్లు = j / 4;
              ఆపు;
            }
        }
    }
# endif
  ఒకవేళ (digest_hex_బైట్లు < 2 || digest_hex_బైట్లు % 2
      || DIGEST_MAX_LEN * 2 < digest_hex_బైట్లు)
    తిరిగి false;
  digest_నిడివి = digest_hex_బైట్లు * 4;
# ఒకవేళ HASH_ALGO_CKSUM
  }
# endif
#అయితే_ముగింపు

  /* This field must be the hexadecimal or base64 representation
     of the message digest.  */
  అయితే (s[i] && !c_isblank (s[i]))
    i++;

  /* The digest must be followed by at least one whitespace character.  */
  ఒకవేళ (i == s_len)
    తిరిగి false;

  *d_len = &s[i] - (అక్షరం *) *digest;
  s[i++] = '\0';

  ఒకవేళ (! valid_digits (*digest, *d_len))
    తిరిగి false;

  /* If "bsd reversed" format detected.  */
  ఒకవేళ ((s_len - i == 1) || (s[i] != ' ' && s[i] != '*'))
    {
      /* Don't allow mixing bsd and standard formats,
         to minimize security issues with attackers
         renaming files with leading spaces.
         This assumes that with bsd format checksums
         that the first file name does not have
         a leading ' ' or '*'.  */
      ఒకవేళ (bsd_reversed == 0)
        తిరిగి false;
      bsd_reversed = 1;
    }
  లేకపోతే ఒకవేళ (bsd_reversed != 1)
    {
      bsd_reversed = 0;
      *ద్విమాన = (s[i++] == '*');
    }

  /* All characters between the type indicator and end of line are
     significant -- that includes leading and trailing white space.  */
  *file_name = &s[i];

  ఒకవేళ (escaped_filename)
    తిరిగి filename_unescape (&s[i], s_len - i) != ఖాళీ;

  తిరిగి true;
}

#ఒకవేళ !HASH_ALGO_కూడిక
/* If ESCAPE is true, then translate each:
   NEWLINE byte to the string, "\\n",
   CARRIAGE RETURN byte to the string, "\\r",
   and each backslash to "\\\\".  */
స్థిర శూన్య
print_filename (అక్షరం స్థిరరాశి *file, bool escape)
{
  ఒకవేళ (! escape)
    {
      fputs (file, stdout);
      తిరిగి;
    }

  అయితే (*file)
    {
      ఎంపిక (*file)
        {
        స్థితి '\n':
          fputs ("\\n", stdout);
          ఆపు;

        స్థితి '\r':
          fputs ("\\r", stdout);
          ఆపు;

        స్థితి '\\':
          fputs ("\\\\", stdout);
          ఆపు;

        అప్రమేయ:
          అక్షరం_రాయి (*file);
          ఆపు;
        }
      file++;
    }
}
#అయితే_ముగింపు

/* An interface to the function, DIGEST_STREAM.
   Operate on FILENAME (it may be "-").

   *BINARY indicates whether the file is binary.  BINARY < 0 means it
   depends on whether binary mode makes any difference and the file is
   a terminal; in that case, clear *BINARY if the file was treated as
   text because it was a terminal.

   Put the checksum in *BIN_RESULT, which must be properly aligned.
   Put true in *MISSING if the file can't be opened due to ENOENT.
   Return true if successful.  */

స్థిర bool
digest_file (అక్షరం స్థిరరాశి *filename, పూర్ణాంకం *ద్విమాన, గుర్తులేని అక్షరం *bin_ఫలితం,
             bool *missing, MAYBE_వాడని intmax_t *నిడివి)
{
  FILE *fp;
  పూర్ణాంకం err;
  bool is_stdin = streq (filename, "-");

  *missing = false;

  ఒకవేళ (is_stdin)
    {
      have_read_stdin = true;
      fp = stdin;
      ఒకవేళ (O_ద్విమాన && *ద్విమాన)
        {
          ఒకవేళ (*ద్విమాన < 0)
            *ద్విమాన = ! isatty (STDIN_FILENO);
          ఒకవేళ (*ద్విమాన)
            xset_ద్విమాన_బహుళకం (STDIN_FILENO, O_ద్విమాన);
        }
    }
  లేకపోతే
    {
      fp = fopen (filename, O_ద్విమాన ? (*ద్విమాన ? "rb" : "rt") : "r");
      ఒకవేళ (fp == ఖాళీ)
        {
          ఒకవేళ (ignore_missing && errno == ENOENT)
            {
              *missing = true;
              తిరిగి true;
            }
          error (0, errno, "%s", quotef (filename));
          తిరిగి false;
        }
    }

  fadvise (fp, FADVISE_SEQUENTIAL);

#ఒకవేళ HASH_ALGO_CKSUM
  ఒకవేళ (cksum_algorithm == blake2b
      || cksum_algorithm == sha2 || cksum_algorithm == sha3)
    *నిడివి = digest_నిడివి >> 3;
  err = DIGEST_ప్రవాహం (fp, bin_ఫలితం, నిడివి);
#లేదావేళ HASH_ALGO_కూడిక
  err = DIGEST_ప్రవాహం (fp, bin_ఫలితం, నిడివి);
#లేదావేళ HASH_ALGO_BLAKE2
  err = DIGEST_ప్రవాహం (fp, bin_ఫలితం, digest_నిడివి >> 3);
#లేకపోతే
  err = DIGEST_ప్రవాహం (fp, bin_ఫలితం);
#అయితే_ముగింపు
  err = err ? errno : 0;
  ఒకవేళ (is_stdin)
    clearerr (fp);
  లేకపోతే ఒకవేళ (fclose (fp) != 0 && !err)
    err = errno;

  ఒకవేళ (err)
    {
      error (0, err, "%s", quotef (filename));
      తిరిగి false;
    }

  తిరిగి true;
}

#ఒకవేళ !HASH_ALGO_కూడిక
స్థిర శూన్య
output_file (అక్షరం స్థిరరాశి *file, పూర్ణాంకం ద్విమాన_file, శూన్య స్థిరరాశి *digest,
             MAYBE_వాడని bool raw, bool tagged, గుర్తులేని అక్షరం delim,
             MAYBE_వాడని bool వాదనలు, MAYBE_వాడని intmax_t నిడివి)
{
# ఒకవేళ HASH_ALGO_CKSUM
  ఒకవేళ (raw)
    {
      fwrite (digest, 1, digest_నిడివి >> 3, stdout);
      తిరిగి;
    }
# endif

  గుర్తులేని అక్షరం స్థిరరాశి *bin_బఫరు = digest;

  /* Output a leading backslash if the file name contains problematic chars.  */
  bool needs_escape = delim == '\n' && problematic_chars (file);

  ఒకవేళ (needs_escape)
    అక్షరం_రాయి ('\\');

  ఒకవేళ (tagged)
    {
# ఒకవేళ HASH_ALGO_CKSUM
      ఒకవేళ (cksum_algorithm == sha2)
        రాయి ("SHA%u", (గుర్తులేని పూర్ణాంకం) digest_నిడివి);
      లేకపోతే
# endif
      fputs (DIGEST_TYPE_STRING, stdout);
# ఒకవేళ HASH_ALGO_BLAKE2
      ఒకవేళ (digest_నిడివి < DIGEST_MAX_LEN * 8)
        రాయి ("-%u", (గుర్తులేని పూర్ణాంకం) digest_నిడివి);
# elif HASH_ALGO_CKSUM
      ఒకవేళ (cksum_algorithm == sha3)
        రాయి ("-%u", (గుర్తులేని పూర్ణాంకం) digest_నిడివి);
      ఒకవేళ (cksum_algorithm == blake2b)
        {
          ఒకవేళ (digest_నిడివి < DIGEST_MAX_LEN * 8)
            రాయి ("-%u", (గుర్తులేని పూర్ణాంకం) digest_నిడివి);
        }
# endif
      fputs (" (", stdout);
      print_filename (file, needs_escape);
      fputs (") = ", stdout);
    }

# ఒకవేళ HASH_ALGO_CKSUM
  ఒకవేళ (base64_digest)
    {
      అక్షరం b64[BASE64_నిడివి (DIGEST_BIN_బైట్లు) + 1];
      base64_encode ((అక్షరం స్థిరరాశి *) bin_బఫరు, digest_నిడివి >> 3,
                     b64, పరిమాణం b64);
      fputs (b64, stdout);
    }
  లేకపోతే
# endif
    {
      కొరకు (idx_t i = 0; i < (digest_hex_బైట్లు >> 1); ++i)
        రాయి ("%02x", bin_బఫరు[i]);
    }

  ఒకవేళ (!tagged)
    {
      అక్షరం_రాయి (' ');
      అక్షరం_రాయి (ద్విమాన_file ? '*' : ' ');
      print_filename (file, needs_escape);
    }

  అక్షరం_రాయి (delim);
}
#అయితే_ముగింపు

#ఒకవేళ HASH_ALGO_CKSUM
/* Return true if B64_DIGEST is the same as the base64 digest of the
   DIGEST_LENGTH/8 bytes at BIN_BUFFER.  */
స్థిర bool
b64_equal (గుర్తులేని అక్షరం స్థిరరాశి *b64_digest, గుర్తులేని అక్షరం స్థిరరాశి *bin_బఫరు)
{
  idx_t b64_n_బైట్లు = BASE64_నిడివి (digest_నిడివి >> 3);
  అక్షరం b64[BASE64_నిడివి (DIGEST_BIN_బైట్లు) + 1];
  base64_encode ((అక్షరం స్థిరరాశి *) bin_బఫరు, digest_నిడివి >> 3,
                 b64, పరిమాణం b64);
  తిరిగి memeq (b64_digest, b64, b64_n_బైట్లు + 1);
}
#అయితే_ముగింపు

/* Return true if HEX_DIGEST is the same as the hex-encoded digest of the
   DIGEST_LENGTH/8 bytes at BIN_BUFFER.  */
స్థిర bool
hex_equal (గుర్తులేని అక్షరం స్థిరరాశి *hex_digest, గుర్తులేని అక్షరం స్థిరరాశి *bin_బఫరు)
{
  స్థిర స్థిరరాశి అక్షరం bin2hex[] = { '0', '1', '2', '3',
                                  '4', '5', '6', '7',
                                  '8', '9', 'a', 'b',
                                  'c', 'd', 'e', 'f' };
  idx_t digest_bin_బైట్లు = digest_hex_బైట్లు >> 1;

  /* Compare generated binary number with text representation
     in check file.  Ignore case of hex digits.  */
  idx_t cnt;
  కొరకు (cnt = 0; cnt < digest_bin_బైట్లు; ++cnt)
    {
      ఒకవేళ (c_tolower (hex_digest[2 * cnt])
          != bin2hex[bin_బఫరు[cnt] >> 4]
          || (c_tolower (hex_digest[2 * cnt + 1])
              != (bin2hex[bin_బఫరు[cnt] & 0xf])))
        ఆపు;
    }
  తిరిగి cnt == digest_bin_బైట్లు;
}

స్థిర bool
digest_check (అక్షరం స్థిరరాశి *checkfile_name)
{
  FILE *checkfile_ప్రవాహం;
  intmax_t n_misformatted_పంక్తులు = 0;
  intmax_t n_mismatched_checksums = 0;
  intmax_t n_open_or_read_failures = 0;
  bool properly_formatted_పంక్తులు = false;
  bool matched_checksums = false;
  గుర్తులేని అక్షరం bin_బఫరు_unaligned[DIGEST_BIN_బైట్లు + DIGEST_ALIGN];
  /* Make sure bin_buffer is properly aligned. */
  గుర్తులేని అక్షరం *bin_బఫరు = ptr_align (bin_బఫరు_unaligned, DIGEST_ALIGN);
  intmax_t పంక్తి_సంఖ్య;
  అక్షరం *పంక్తి;
  size_t పంక్తి_chars_allocated;
  bool is_stdin = streq (checkfile_name, "-");

  ఒకవేళ (is_stdin)
    {
      have_read_stdin = true;
      checkfile_name = _("standard input");
      checkfile_ప్రవాహం = stdin;
    }
  లేకపోతే
    {
      checkfile_ప్రవాహం = fopen (checkfile_name, "r");
      ఒకవేళ (checkfile_ప్రవాహం == ఖాళీ)
        {
          error (0, errno, "%s", quotef (checkfile_name));
          తిరిగి false;
        }
    }

  పంక్తి_సంఖ్య = 0;
  పంక్తి = ఖాళీ;
  పంక్తి_chars_allocated = 0;
  చేయి
    {
      అక్షరం *filename;
      పూర్ణాంకం ద్విమాన;
      గుర్తులేని అక్షరం *digest;
      ssize_t పంక్తి_నిడివి;

      ++పంక్తి_సంఖ్య;
      ఒకవేళ (పంక్తి_సంఖ్య == 0)
        error (EXIT_FAILURE, 0, _("%s: too many checksum lines"),
               quotef (checkfile_name));

      పంక్తి_నిడివి = getline (&పంక్తి, &పంక్తి_chars_allocated, checkfile_ప్రవాహం);
      ఒకవేళ (పంక్తి_నిడివి <= 0)
        ఆపు;

      /* Ignore comment lines, which begin with a '#' character.  */
      ఒకవేళ (పంక్తి[0] == '#')
        కొనసాగు;

      /* Remove any trailing newline.  */
      పంక్తి_నిడివి -= పంక్తి[పంక్తి_నిడివి - 1] == '\n';
      /* Remove any trailing carriage return.  */
      పంక్తి_నిడివి -= పంక్తి[పంక్తి_నిడివి - (0 < పంక్తి_నిడివి)] == '\r';

      /* Ignore empty lines.  */
      ఒకవేళ (పంక్తి_నిడివి == 0)
        కొనసాగు;

      పంక్తి[పంక్తి_నిడివి] = '\0';

      idx_t d_len;
      ఒకవేళ (! (విభజించు_3 (పంక్తి, పంక్తి_నిడివి, &digest, &d_len, &ద్విమాన, &filename)
             && ! (is_stdin && streq (filename, "-"))))
        {
          ++n_misformatted_పంక్తులు;

          ఒకవేళ (warn)
            {
              error (0, 0,
                     _("%s: %jd"
                       ": improperly formatted %s checksum line"),
                     quotef (checkfile_name), పంక్తి_సంఖ్య,
                     DIGEST_TYPE_STRING);
            }
        }
      లేకపోతే
        {
          bool ok;
          bool missing;

          properly_formatted_పంక్తులు = true;

          intmax_t నిడివి;
          ok = digest_file (filename, &ద్విమాన, bin_బఫరు, &missing, &నిడివి);

          ఒకవేళ (!ok)
            {
              ++n_open_or_read_failures;
              ఒకవేళ (!స్థితి_only)
                రాయి ("%s: %s\n", quotef (filename),_("FAILED open or read"));
            }
          లేకపోతే ఒకవేళ (ignore_missing && missing)
            {
              /* Ignore missing files with --ignore-missing.  */
              ;
            }
          లేకపోతే
            {
              bool సరిపోలిక = false;
#ఒకవేళ HASH_ALGO_CKSUM
              ఒకవేళ (d_len == BASE64_నిడివి (digest_నిడివి >> 3))
                సరిపోలిక = b64_equal (digest, bin_బఫరు);
              లేకపోతే
#అయితే_ముగింపు
                ఒకవేళ (d_len == digest_hex_బైట్లు)
                  సరిపోలిక = hex_equal (digest, bin_బఫరు);

              ఒకవేళ (సరిపోలిక)
                matched_checksums = true;
              లేకపోతే
                ++n_mismatched_checksums;

              ఒకవేళ (!స్థితి_only)
                {
                  ఒకవేళ (! సరిపోలిక || ! quiet)
                    fputs (quotef (filename), stdout);

                  ఒకవేళ (! సరిపోలిక)
                    రాయి (": %s\n", _("FAILED"));
                  లేకపోతే ఒకవేళ (!quiet)
                    రాయి (": %s\n", _("OK"));
                }
            }

          ఒకవేళ (ferror (stdout))
            write_error ();
        }
    }
  అయితే (!feof (checkfile_ప్రవాహం) && !ferror (checkfile_ప్రవాహం));

  విడుదల (పంక్తి);

  పూర్ణాంకం err = ferror (checkfile_ప్రవాహం) ? 0 : -1;
  ఒకవేళ (is_stdin)
    clearerr (checkfile_ప్రవాహం);
  లేకపోతే ఒకవేళ (fclose (checkfile_ప్రవాహం) != 0 && err < 0)
    err = errno;

  ఒకవేళ (0 <= err)
    {
      error (0, err, err ? "%s" : _("%s: read error"),
             quotef (checkfile_name));
      తిరిగి false;
    }

  ఒకవేళ (! properly_formatted_పంక్తులు)
    {
      /* Warn if no tests are found.  */
      error (0, 0, _("%s: no properly formatted checksum lines found"),
             quotef (checkfile_name));
    }
  లేకపోతే
    {
      ఒకవేళ (!స్థితి_only)
        {
          ఒకవేళ (n_misformatted_పంక్తులు != 0)
            error (0, 0,
                   (ngettext
                    ("WARNING: %jd line is improperly formatted",
                     "WARNING: %jd lines are improperly formatted",
                     select_plural (n_misformatted_పంక్తులు))),
                   n_misformatted_పంక్తులు);

          ఒకవేళ (n_open_or_read_failures != 0)
            error (0, 0,
                   (ngettext
                    ("WARNING: %jd listed file could not be read",
                     "WARNING: %jd listed files could not be read",
                     select_plural (n_open_or_read_failures))),
                   n_open_or_read_failures);

          ఒకవేళ (n_mismatched_checksums != 0)
            error (0, 0,
                   (ngettext
                    ("WARNING: %jd computed checksum did NOT match",
                     "WARNING: %jd computed checksums did NOT match",
                     select_plural (n_mismatched_checksums))),
                   n_mismatched_checksums);

          ఒకవేళ (ignore_missing && ! matched_checksums)
            error (0, 0, _("%s: no file was verified"),
                   quotef (checkfile_name));
        }
    }

  తిరిగి (properly_formatted_పంక్తులు
          && matched_checksums
          && n_mismatched_checksums == 0
          && n_open_or_read_failures == 0
          && (!strict || n_misformatted_పంక్తులు == 0));
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  గుర్తులేని అక్షరం bin_బఫరు_unaligned[DIGEST_BIN_బైట్లు + DIGEST_ALIGN];
  /* Make sure bin_buffer is properly aligned. */
  గుర్తులేని అక్షరం *bin_బఫరు = ptr_align (bin_బఫరు_unaligned, DIGEST_ALIGN);
  bool do_check = false;
  పూర్ణాంకం opt;
  bool ok = true;
  పూర్ణాంకం ద్విమాన = -1;
  పూర్ణాంకం ఉపసర్గ_ట్యాగ్ = -1;
  నిర్మాణం option స్థిరరాశి *long_opts = long_options;

#ఒకవేళ HASH_ALGO_CKSUM
  ఒకవేళ (cksum_algorithm != చక్రీయ_తనిఖీ)
    {
      legacy_బహుళకం = true;
      ఉపసర్గ_ట్యాగ్ = 0;
      algorithm_specified = true;
      long_opts = legacy_long_options;
    }
#అయితే_ముగింపు

  /* Setting values of global variables.  */
  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* Line buffer stdout to ensure lines are written atomically and immediately
     so that processes running in parallel do not intersperse their output.  */
  setvbuf (stdout, ఖాళీ, _IOLBF, 0);

#ఒకవేళ HASH_ALGO_కూడిక
  అక్షరం స్థిరరాశి *short_opts = "rs";
#లేదావేళ HASH_ALGO_CKSUM
  అక్షరం స్థిరరాశి *short_opts = legacy_బహుళకం ? "bctwz" : "a:l:bctwz";
  అక్షరం స్థిరరాశి *digest_నిడివి_str = "";
#లేదావేళ HASH_ALGO_BLAKE2
  అక్షరం స్థిరరాశి *short_opts = "l:bctwz";
  అక్షరం స్థిరరాశి *digest_నిడివి_str = "";
#లేకపోతే
  అక్షరం స్థిరరాశి *short_opts = "bctwz";
#అయితే_ముగింపు

  అయితే ((opt = getopt_long (argc, argv, short_opts, long_opts, ఖాళీ))
         != -1)
    ఎంపిక (opt)
      {
#ఒకవేళ HASH_ALGO_CKSUM
      స్థితి 'a':
        cksum_algorithm = XARGMATCH_EXACT ("--algorithm", optarg,
                                           algorithm_వాదనలు, algorithm_types);
        algorithm_specified = true;
        ఆపు;

      స్థితి DEBUG_కార్యక్రమం_OPTION:
        cksum_debug = true;
        ఆపు;
#అయితే_ముగింపు
#ఒకవేళ HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
      స్థితి 'l':
        digest_నిడివి = xnumtoimax (optarg, 10, 0, IDX_MAX, "",
                                    _("invalid length"), 0,
                                    XTOINT_MAX_QUIET);
        digest_నిడివి_str = optarg;
        ఆపు;
#అయితే_ముగింపు
#ఒకవేళ !HASH_ALGO_కూడిక
      స్థితి 'c':
        do_check = true;
        ఆపు;
      స్థితి స్థితి_OPTION:
        స్థితి_only = true;
        warn = false;
        quiet = false;
        ఆపు;
      స్థితి 'b':
        ద్విమాన = 1;
        ఆపు;
      స్థితి 't':
        ద్విమాన = 0;
        ఆపు;
      స్థితి 'w':
        స్థితి_only = false;
        warn = true;
        quiet = false;
        ఆపు;
      స్థితి IGNORE_MISSING_OPTION:
        ignore_missing = true;
        ఆపు;
      స్థితి QUIET_OPTION:
        స్థితి_only = false;
        warn = false;
        quiet = true;
        ఆపు;
      స్థితి STRICT_OPTION:
        strict = true;
        ఆపు;
# ఒకవేళ HASH_ALGO_CKSUM
      స్థితి BASE64_OPTION:
        base64_digest = true;
        ఆపు;
      స్థితి RAW_OPTION:
        raw_digest = true;
        ఆపు;
      స్థితి UNTAG_OPTION:
        ఉపసర్గ_ట్యాగ్ = 0;
        ఆపు;
# endif
      స్థితి ట్యాగ్_OPTION:
        ఉపసర్గ_ట్యాగ్ = 1;
        ఆపు;
      స్థితి 'z':
        digest_delim = '\0';
        ఆపు;
#అయితే_ముగింపు
#ఒకవేళ HASH_ALGO_కూడిక
      స్థితి 'r':		/* For SysV compatibility. */
        కూడిక_algorithm = bsd;
        ఆపు;

      స్థితి 's':
        కూడిక_algorithm = sysv;
        ఆపు;
#అయితే_ముగింపు
      case_GETOPT_HELP_CHAR;
      case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);
      అప్రమేయ:
        usage (EXIT_FAILURE);
      }

  min_digest_పంక్తి_నిడివి = MIN_DIGEST_పంక్తి_నిడివి;
#ఒకవేళ HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# ఒకవేళ HASH_ALGO_CKSUM
  ఒకవేళ (digest_నిడివి && (cksum_algorithm != blake2b
                        && cksum_algorithm != sha2
                        && cksum_algorithm != sha3))
    error (EXIT_FAILURE, 0,
           _("--length is only supported with --algorithm "
             "blake2b, sha2, or sha3"));
  ఒకవేళ (cksum_algorithm == sha2 || cksum_algorithm == sha3)
    {
      /* Do not require --length with --check.  */
      ఒకవేళ (digest_నిడివి == 0 && *digest_నిడివి_str == '\0' && ! do_check)
        error (EXIT_FAILURE, 0, _("--algorithm=%s requires specifying "
                                  "--length 224, 256, 384, or 512"),
               algorithm_వాదనలు[cksum_algorithm]);
      /* If --check and --length are used we verify the digest length.  */
      ఒకవేళ ((! do_check || *digest_నిడివి_str != '\0')
          && digest_నిడివి != SHA224_DIGEST_కొలత * 8
          && digest_నిడివి != SHA256_DIGEST_కొలత * 8
          && digest_నిడివి != SHA384_DIGEST_కొలత * 8
          && digest_నిడివి != SHA512_DIGEST_కొలత * 8)
        {
          error (0, 0, _("invalid length: %s"), quote (digest_నిడివి_str));
          error (EXIT_FAILURE, 0, _("digest length for %s must be "
                                    "224, 256, 384, or 512"),
                 quote (DIGEST_TYPE_STRING));
        }
    }
  లేకపోతే
    {
      /* If the digest length checks for SHA-3 are satisfied, the less strict
         checks for BLAKE2 will also be.  */
# లేకపోతే
    {
# endif
      ఒకవేళ (digest_నిడివి > DIGEST_MAX_LEN * 8)
        {
          error (0, 0, _("invalid length: %s"), quote (digest_నిడివి_str));
          error (EXIT_FAILURE, 0,
                 _("maximum digest length for %s is %d bits"),
                 quote (DIGEST_TYPE_STRING),
                 DIGEST_MAX_LEN * 8);
        }
      ఒకవేళ (digest_నిడివి % 8 != 0)
        {
          error (0, 0, _("invalid length: %s"), quote (digest_నిడివి_str));
          error (EXIT_FAILURE, 0, _("length is not a multiple of 8"));
        }
    }
  ఒకవేళ (digest_నిడివి == 0)
    {
# ఒకవేళ HASH_ALGO_BLAKE2
      digest_నిడివి = DIGEST_MAX_LEN * 8;
# లేకపోతే
      digest_నిడివి = algorithm_బిట్లు[cksum_algorithm];
# endif
    }
  digest_hex_బైట్లు = digest_నిడివి >> 2;
#లేకపోతే
  digest_hex_బైట్లు = DIGEST_HEX_బైట్లు;
#అయితే_ముగింపు

#ఒకవేళ HASH_ALGO_CKSUM
  ఎంపిక (+cksum_algorithm)
    {
    స్థితి bsd:
    స్థితి sysv:
    స్థితి చక్రీయ_తనిఖీ:
    స్థితి crc32b:
        ఒకవేళ (do_check && algorithm_specified)
          error (EXIT_FAILURE, 0,
                 _("--check is not supported with "
                   "--algorithm={bsd,sysv,crc,crc32b}"));
        ఆపు;
    }

  ఒకవేళ (base64_digest && raw_digest)
   {
     error (0, 0, _("--base64 and --raw are mutually exclusive"));
     usage (EXIT_FAILURE);
   }
#అయితే_ముగింపు

  ఒకవేళ (digest_delim != '\n' && do_check)
    {
      error (0, 0, _("the --zero option is not supported when "
                     "verifying checksums"));
      usage (EXIT_FAILURE);
    }
  ఒకవేళ (1 <= ఉపసర్గ_ట్యాగ్ && do_check)
    {
      /* Note we allow --untagged with --check to more
         seamlessly support --untagged in an emulation wrapper.  */
      error (0, 0, _("the --tag option is meaningless when "
                     "verifying checksums"));
      usage (EXIT_FAILURE);
    }

  ఒకవేళ (0 <= ద్విమాన && do_check)
    {
      error (0, 0, _("the --binary and --text options are meaningless when "
                     "verifying checksums"));
      usage (EXIT_FAILURE);
    }

  ఒకవేళ (ignore_missing && !do_check)
    {
      error (0, 0,
             _("the --ignore-missing option is meaningful only when "
               "verifying checksums"));
      usage (EXIT_FAILURE);
    }

  ఒకవేళ (స్థితి_only && !do_check)
    {
      error (0, 0,
       _("the --status option is meaningful only when verifying checksums"));
      usage (EXIT_FAILURE);
    }

  ఒకవేళ (warn && !do_check)
    {
      error (0, 0,
       _("the --warn option is meaningful only when verifying checksums"));
      usage (EXIT_FAILURE);
    }

  ఒకవేళ (quiet && !do_check)
    {
      error (0, 0,
       _("the --quiet option is meaningful only when verifying checksums"));
      usage (EXIT_FAILURE);
    }

  ఒకవేళ (strict & !do_check)
   {
     error (0, 0,
        _("the --strict option is meaningful only when verifying checksums"));
     usage (EXIT_FAILURE);
   }

  ఒకవేళ (ఉపసర్గ_ట్యాగ్ == -1)
    ఉపసర్గ_ట్యాగ్ = HASH_ALGO_CKSUM;

  ఒకవేళ (ఉపసర్గ_ట్యాగ్ && !ద్విమాన)
   {
     /* This could be supported in a backwards compatible way
        by prefixing the output line with a space in text mode.
        However that's invasive enough that it was agreed to
        not support this mode with --tag, as --text use cases
        are adequately supported by the default output format.  */
#ఒకవేళ !HASH_ALGO_CKSUM
     error (0, 0, _("--tag does not support --text mode"));
#లేకపోతే
     error (0, 0, _("--text mode is only supported with --untagged"));
#అయితే_ముగింపు
     usage (EXIT_FAILURE);
   }

  ఒకవేళ (!O_ద్విమాన && ద్విమాన < 0)
    ద్విమాన = 0;
  లేకపోతే ఒకవేళ (ఉపసర్గ_ట్యాగ్)
    ద్విమాన = 1;

  అక్షరం **నిర్వాహ్యం_lim = argv + argc;
  ఒకవేళ (optind == argc)
    *నిర్వాహ్యం_lim++ = bad_cast ("-");
  లేకపోతే ఒకవేళ (1 < argc - optind && raw_digest)
    error (EXIT_FAILURE, 0,
           _("the --raw option is not supported with multiple files"));

  కొరకు (అక్షరం **operandp = argv + optind; operandp < నిర్వాహ్యం_lim; operandp++)
    {
      అక్షరం *file = *operandp;
      ఒకవేళ (do_check)
        ok &= digest_check (file);
      లేకపోతే
        {
          పూర్ణాంకం ద్విమాన_file = ద్విమాన;
          bool missing;
          intmax_t నిడివి;

          ఒకవేళ (! digest_file (file, &ద్విమాన_file, bin_బఫరు, &missing, &నిడివి))
            ok = false;
          లేకపోతే
            {
              DIGEST_బహిర్గతం (file, ద్విమాన_file, bin_బఫరు, raw_digest, ఉపసర్గ_ట్యాగ్,
                          digest_delim, optind != argc, నిడివి);
              ఒకవేళ (ferror (stdout))
                write_error ();
            }
        }
    }

  ఒకవేళ (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, _("standard input"));

  తిరిగి ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
