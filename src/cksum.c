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

#अन्तर्भाव <config.h>

#अन्तर्भाव <getopt.h>
#अन्तर्भाव <sys/types.h>

#अन्तर्भाव "assure.h"
#अन्तर्भाव "system.h"
#अन्तर्भाव "argmatch.h"
#अन्तर्भाव "c-ctype.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "xdectoint.h"
#अन्तर्भाव "xstrtol.h"

#यदि_अपरिभाषितम् HASH_ALGO_CKSUM
# define HASH_ALGO_CKSUM 0
#यदि_अन्तः

#यदि HASH_ALGO_योग || HASH_ALGO_CKSUM
# include "sum.h"
#यदि_अन्तः
#यदि HASH_ALGO_CKSUM
# include "cksum.h"
# include "cksum_crc.h"
# include "base64.h"
#यदि_अन्तः
#यदि HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# include "blake2/b2sum.h"
#यदि_अन्तः
#यदि HASH_ALGO_MD5 || HASH_ALGO_CKSUM
# include "md5.h"
#यदि_अन्तः
#यदि HASH_ALGO_SHA1 || HASH_ALGO_CKSUM
# include "sha1.h"
#यदि_अन्तः
#यदि HASH_ALGO_SHA256 || HASH_ALGO_SHA224 || HASH_ALGO_CKSUM
# include "sha256.h"
#यदि_अन्तः
#यदि HASH_ALGO_SHA512 || HASH_ALGO_SHA384 || HASH_ALGO_CKSUM
# include "sha512.h"
#यदि_अन्तः
#यदि HASH_ALGO_CKSUM
# include "sha3.h"
#यदि_अन्तः
#यदि HASH_ALGO_CKSUM
# include "sm3.h"
#यदि_अन्तः
#अन्तर्भाव "fadvise.h"
#अन्तर्भाव "stdio--.h"
#अन्तर्भाव "xbinary-io.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#यदि HASH_ALGO_योग
# define क्रमादेशः_NAME "sum"
# define DIGEST_TYPE_STRING "BSD"
# define DIGEST_प्रवाहः sumfns[योग_algorithm]
# define DIGEST_बहिर्गमनम् योग_output_fns[योग_algorithm]
# define DIGEST_द्वयङ्काः 16
# define DIGEST_ALIGN 4
#नोचेद्यदि HASH_ALGO_CKSUM
# define MAX_DIGEST_द्वयङ्काः 512
# define MAX_DIGEST_ALIGN 8
# define DIGEST_TYPE_STRING algorithm_चिह्नकानि[cksum_algorithm]
# define DIGEST_प्रवाहः cksumfns[cksum_algorithm]
# define DIGEST_बहिर्गमनम् cksum_output_fns[cksum_algorithm]
# define DIGEST_द्वयङ्काः MAX_DIGEST_द्वयङ्काः
# define DIGEST_ALIGN MAX_DIGEST_ALIGN
#नोचेद्यदि HASH_ALGO_MD5
# define क्रमादेशः_NAME "md5sum"
# define DIGEST_TYPE_STRING "MD5"
# define DIGEST_प्रवाहः md5_प्रवाहः
# define DIGEST_द्वयङ्काः 128
# define DIGEST_REFERENCE "RFC 1321"
# define DIGEST_ALIGN 4
#नोचेद्यदि HASH_ALGO_BLAKE2
# define क्रमादेशः_NAME "b2sum"
# define DIGEST_TYPE_STRING "BLAKE2b"
# define DIGEST_प्रवाहः blake2b_प्रवाहः
# define DIGEST_द्वयङ्काः 512
# define DIGEST_REFERENCE "RFC 7693"
# define DIGEST_ALIGN 8
#नोचेद्यदि HASH_ALGO_SHA1
# define क्रमादेशः_NAME "sha1sum"
# define DIGEST_TYPE_STRING "SHA1"
# define DIGEST_प्रवाहः sha1_प्रवाहः
# define DIGEST_द्वयङ्काः 160
# define DIGEST_REFERENCE "FIPS-180-1"
# define DIGEST_ALIGN 4
#नोचेद्यदि HASH_ALGO_SHA256
# define क्रमादेशः_NAME "sha256sum"
# define DIGEST_TYPE_STRING "SHA256"
# define DIGEST_प्रवाहः sha256_प्रवाहः
# define DIGEST_द्वयङ्काः 256
# define DIGEST_REFERENCE "FIPS-180-2"
# define DIGEST_ALIGN 4
#नोचेद्यदि HASH_ALGO_SHA224
# define क्रमादेशः_NAME "sha224sum"
# define DIGEST_TYPE_STRING "SHA224"
# define DIGEST_प्रवाहः sha224_प्रवाहः
# define DIGEST_द्वयङ्काः 224
# define DIGEST_REFERENCE "RFC 3874"
# define DIGEST_ALIGN 4
#नोचेद्यदि HASH_ALGO_SHA512
# define क्रमादेशः_NAME "sha512sum"
# define DIGEST_TYPE_STRING "SHA512"
# define DIGEST_प्रवाहः sha512_प्रवाहः
# define DIGEST_द्वयङ्काः 512
# define DIGEST_REFERENCE "FIPS-180-2"
# define DIGEST_ALIGN 8
#नोचेद्यदि HASH_ALGO_SHA384
# define क्रमादेशः_NAME "sha384sum"
# define DIGEST_TYPE_STRING "SHA384"
# define DIGEST_प्रवाहः sha384_प्रवाहः
# define DIGEST_द्वयङ्काः 384
# define DIGEST_REFERENCE "FIPS-180-2"
# define DIGEST_ALIGN 8
#अन्यथा
# error "Can't decide which hash algorithm to compile."
#यदि_अन्तः
#यदि !HASH_ALGO_योग && !HASH_ALGO_CKSUM
# define DIGEST_बहिर्गमनम् output_file
#यदि_अन्तः

#यदि HASH_ALGO_CKSUM
# define क्रमादेशः_NAME (!legacy_बहुलकम् ? "cksum"			 \
                       : cksum_algorithm == md5 ? "md5sum"	 \
                       : cksum_algorithm == sha1 ? "sha1sum"	 \
                       : cksum_algorithm == sha224 ? "sha224sum" \
                       : cksum_algorithm == sha256 ? "sha256sum" \
                       : cksum_algorithm == sha384 ? "sha384sum" \
                       : cksum_algorithm == sha512 ? "sha512sum" \
                       : "cksum")
#यदि_अन्तः

#यदि HASH_ALGO_योग
# define AUTHORS \
  proper_name ("Kayvan Aghaiepour"), \
  proper_name ("David MacKenzie")
#नोचेद्यदि HASH_ALGO_CKSUM
# define AUTHORS \
  proper_name_lite ("Padraig Brady", "P\303\241draig Brady"), \
  proper_name ("Q. Frank Xia")
#नोचेद्यदि HASH_ALGO_BLAKE2
# define AUTHORS \
  proper_name_lite ("Padraig Brady", "P\303\241draig Brady"), \
  proper_name ("Samuel Neves")
#अन्यथा
# define AUTHORS \
  proper_name ("Ulrich Drepper"), \
  proper_name ("Scott Miller"), \
  proper_name ("David Madore")
#यदि_अन्तः
#यदि !HASH_ALGO_BLAKE2 && !HASH_ALGO_CKSUM
# define DIGEST_HEX_अष्टकानि (DIGEST_द्वयङ्काः / 4)
#यदि_अन्तः
#परिभाषय DIGEST_BIN_अष्टकानि (DIGEST_द्वयङ्काः / 8)

/* The minimum length of a valid digest line.  This length does
   not include any newline character at the end of a line.  */
#यदि HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# define MIN_DIGEST_पङ्क्तिः_दीर्घता 3 /* With -l 8.  */
#अन्यथा
# define MIN_DIGEST_पङ्क्तिः_दीर्घता \
   (DIGEST_HEX_अष्टकानि /* length of hexadecimal message digest */ \
    + 1 /* blank */ \
    + 1 /* minimum filename length */ )
#यदि_अन्तः

#यदि !HASH_ALGO_योग
स्थिर शून्य
output_file (वर्ण ध्रुव *file, पूर्णाङ्क द्विमान_file, शून्य ध्रुव *digest,
             bool raw, bool tagged, अचिह्नित वर्ण delim, bool तर्काः,
             intmax_t दीर्घता);
#यदि_अन्तः

/* True if any of the files read were the standard input. */
स्थिर bool have_read_stdin;

/* The minimum length of a valid checksum line for the selected algorithm.  */
स्थिर idx_t min_digest_पङ्क्तिः_दीर्घता;

/* Set to the length of a digest hex string for the selected algorithm.  */
स्थिर idx_t digest_hex_अष्टकानि;

/* With --check, don't generate any output.
   The exit code indicates success or failure.  */
स्थिर bool स्थितिः_only = false;

/* With --check, print a message to standard error warning about each
   improperly formatted checksum line.  */
स्थिर bool warn = false;

/* With --check, ignore missing files.  */
स्थिर bool ignore_missing = false;

/* With --check, suppress the "OK" printed for each verified file.  */
स्थिर bool quiet = false;

/* With --check, exit with a non-zero return code if any line is
   improperly formatted. */
स्थिर bool strict = false;

/* Whether a BSD reversed format checksum is detected.  */
स्थिर पूर्णाङ्क bsd_reversed = -1;

/* line delimiter.  */
स्थिर अचिह्नित वर्ण digest_delim = '\n';

#यदि HASH_ALGO_CKSUM
/* If true, print base64-encoded digests, not hex.  */
स्थिर bool base64_digest = false;
#यदि_अन्तः

/* If true, print binary digests, not hex.  */
स्थिर bool raw_digest = false;

/* blake2b and sha3 allow the -l option.  Luckily they both have the same
   maximum digest size.  */
#यदि HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# यदि HASH_ALGO_CKSUM
static_assert (BLAKE2B_OUTBYTES == SHA3_512_DIGEST_आकार);
# endif
# define DIGEST_MAX_LEN BLAKE2B_OUTBYTES
स्थिर idx_t digest_दीर्घता;
#यदि_अन्तः /* HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM */

प्रकारनिर्देश शून्य (*digest_output_fn)(वर्ण ध्रुव *, पूर्णाङ्क, शून्य ध्रुव *, bool,
                                 bool, अचिह्नित वर्ण, bool, intmax_t);
#यदि HASH_ALGO_योग
गणना Algorithm
{
  bsd,
  sysv,
};

स्थिर गणना Algorithm योग_algorithm;
स्थिर sumfn sumfns[]=
{
  bsd_योग_प्रवाहः,
  sysv_योग_प्रवाहः,
};
स्थिर digest_output_fn योग_output_fns[]=
{
  output_bsd,
  output_sysv,
};
#यदि_अन्तः

#यदि HASH_ALGO_CKSUM
स्थिर पूर्णाङ्क
md5_योग_प्रवाहः (FILE *प्रवाहः, शून्य *resstream, MAYBE_अप्रयुक्तम् intmax_t *दीर्घता)
{
  प्रत्यागम md5_प्रवाहः (प्रवाहः, resstream);
}
स्थिर पूर्णाङ्क
sha1_योग_प्रवाहः (FILE *प्रवाहः, शून्य *resstream, MAYBE_अप्रयुक्तम् intmax_t *दीर्घता)
{
  प्रत्यागम sha1_प्रवाहः (प्रवाहः, resstream);
}
स्थिर पूर्णाङ्क
sha224_योग_प्रवाहः (FILE *प्रवाहः, शून्य *resstream,
                   MAYBE_अप्रयुक्तम् intmax_t *दीर्घता)
{
  प्रत्यागम sha224_प्रवाहः (प्रवाहः, resstream);
}
स्थिर पूर्णाङ्क
sha256_योग_प्रवाहः (FILE *प्रवाहः, शून्य *resstream,
                   MAYBE_अप्रयुक्तम् intmax_t *दीर्घता)
{
  प्रत्यागम sha256_प्रवाहः (प्रवाहः, resstream);
}
स्थिर पूर्णाङ्क
sha384_योग_प्रवाहः (FILE *प्रवाहः, शून्य *resstream,
                   MAYBE_अप्रयुक्तम् intmax_t *दीर्घता)
{
  प्रत्यागम sha384_प्रवाहः (प्रवाहः, resstream);
}
स्थिर पूर्णाङ्क
sha512_योग_प्रवाहः (FILE *प्रवाहः, शून्य *resstream,
                   MAYBE_अप्रयुक्तम् intmax_t *दीर्घता)
{
  प्रत्यागम sha512_प्रवाहः (प्रवाहः, resstream);
}
स्थिर पूर्णाङ्क
sha2_योग_प्रवाहः (FILE *प्रवाहः, शून्य *resstream, intmax_t *दीर्घता)
{
  विकल्प (*दीर्घता)
    {
    पक्ष SHA224_DIGEST_आकार:
      प्रत्यागम sha224_प्रवाहः (प्रवाहः, resstream);
    पक्ष SHA256_DIGEST_आकार:
      प्रत्यागम sha256_प्रवाहः (प्रवाहः, resstream);
    पक्ष SHA384_DIGEST_आकार:
      प्रत्यागम sha384_प्रवाहः (प्रवाहः, resstream);
    पक्ष SHA512_DIGEST_आकार:
      प्रत्यागम sha512_प्रवाहः (प्रवाहः, resstream);
    स्वभाव:
      affirm (false);
    }
}
स्थिर पूर्णाङ्क
sha3_योग_प्रवाहः (FILE *प्रवाहः, शून्य *resstream, intmax_t *दीर्घता)
{
  विकल्प (*दीर्घता)
    {
    पक्ष SHA3_224_DIGEST_आकार:
      प्रत्यागम sha3_224_प्रवाहः (प्रवाहः, resstream);
    पक्ष SHA3_256_DIGEST_आकार:
      प्रत्यागम sha3_256_प्रवाहः (प्रवाहः, resstream);
    पक्ष SHA3_384_DIGEST_आकार:
      प्रत्यागम sha3_384_प्रवाहः (प्रवाहः, resstream);
    पक्ष SHA3_512_DIGEST_आकार:
      प्रत्यागम sha3_512_प्रवाहः (प्रवाहः, resstream);
    स्वभाव:
      affirm (false);
    }
}
स्थिर पूर्णाङ्क
blake2b_योग_प्रवाहः (FILE *प्रवाहः, शून्य *resstream, intmax_t *दीर्घता)
{
  प्रत्यागम blake2b_प्रवाहः (प्रवाहः, resstream, *दीर्घता);
}
स्थिर पूर्णाङ्क
sm3_योग_प्रवाहः (FILE *प्रवाहः, शून्य *resstream, MAYBE_अप्रयुक्तम् intmax_t *दीर्घता)
{
  प्रत्यागम sm3_प्रवाहः (प्रवाहः, resstream);
}

स्थिर वर्ण ध्रुव *ध्रुव algorithm_तर्काः[] =
{
  "bsd", "sysv", "crc", "crc32b", "md5", "sha1",
  "sha224", "sha256", "sha384", "sha512", /* Legacy naming */
  "sha2", "sha3", "blake2b", "sm3", रिक्त
};
स्थिर गणना Algorithm ध्रुव algorithm_types[] =
{
  bsd, sysv, crc, crc32b, md5, sha1,
  sha224, sha256, sha384, sha512,
  sha2, sha3, blake2b, sm3,
};
ARGMATCH_VERIFY (algorithm_तर्काः, algorithm_types);

स्थिर वर्ण ध्रुव *ध्रुव algorithm_चिह्नकानि[] =
{
  "BSD", "SYSV", "CRC", "CRC32B", "MD5", "SHA1",
  "SHA224", "SHA256", "SHA384", "SHA512",
  "SHA2", "SHA3", "BLAKE2b", "SM3", रिक्त
};
स्थिर पूर्णाङ्क ध्रुव algorithm_द्वयङ्काः[] =
{
  16, 16, 32, 32, 128, 160,
  224, 256, 384, 512,
  512, 512, 512, 256, 0
};

static_assert (countof (algorithm_द्वयङ्काः) == countof (algorithm_तर्काः));

स्थिर bool algorithm_specified = false;
स्थिर bool legacy_बहुलकम् = false;
गणना Algorithm cksum_algorithm = crc;

स्थिर sumfn cksumfns[]=
{
  bsd_योग_प्रवाहः,
  sysv_योग_प्रवाहः,
  crc_योग_प्रवाहः,
  crc32b_योग_प्रवाहः,
  md5_योग_प्रवाहः,
  sha1_योग_प्रवाहः,
  sha224_योग_प्रवाहः,
  sha256_योग_प्रवाहः,
  sha384_योग_प्रवाहः,
  sha512_योग_प्रवाहः,
  sha2_योग_प्रवाहः,
  sha3_योग_प्रवाहः,
  blake2b_योग_प्रवाहः,
  sm3_योग_प्रवाहः,
};
स्थिर digest_output_fn cksum_output_fns[]=
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
#यदि_अन्तः

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */

गणना
{
  IGNORE_MISSING_OPTION = CHAR_MAX + 1,
  स्थितिः_OPTION,
  QUIET_OPTION,
  STRICT_OPTION,
  चिह्नकम्_OPTION,
  UNTAG_OPTION,
  DEBUG_क्रमादेशः_OPTION,
  RAW_OPTION,
  BASE64_OPTION,
};

#यदि HASH_ALGO_CKSUM
स्थिर संरचना option ध्रुव legacy_long_options[] =
{
  { "check", no_तर्कः, रिक्त, 'c' },
  { "ignore-missing", no_तर्कः, रिक्त, IGNORE_MISSING_OPTION },
  { "quiet", no_तर्कः, रिक्त, QUIET_OPTION },
  { "status", no_तर्कः, रिक्त, स्थितिः_OPTION },
  { "warn", no_तर्कः, रिक्त, 'w' },
  { "strict", no_तर्कः, रिक्त, STRICT_OPTION },
  { "tag", no_तर्कः, रिक्त, चिह्नकम्_OPTION },
  { "zero", no_तर्कः, रिक्त, 'z' },

  { "binary", no_तर्कः, रिक्त, 'b' }, /* Deprecated.  */
  { "text", no_तर्कः, रिक्त, 't' }, /* Deprecated.  */

  { GETOPT_HELP_OPTION_DECL },
  { GETOPT_संस्करणम्_OPTION_DECL },
  { रिक्त, 0, रिक्त, 0 }
};
#यदि_अन्तः

स्थिर संरचना option ध्रुव long_options[] =
{
#यदि HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
  { "length", required_तर्कः, रिक्त, 'l'},
#यदि_अन्तः

#यदि !HASH_ALGO_योग
  { "check", no_तर्कः, रिक्त, 'c' },
  { "ignore-missing", no_तर्कः, रिक्त, IGNORE_MISSING_OPTION},
  { "quiet", no_तर्कः, रिक्त, QUIET_OPTION },
  { "status", no_तर्कः, रिक्त, स्थितिः_OPTION },
  { "warn", no_तर्कः, रिक्त, 'w' },
  { "strict", no_तर्कः, रिक्त, STRICT_OPTION },
  { "tag", no_तर्कः, रिक्त, चिह्नकम्_OPTION },
  { "zero", no_तर्कः, रिक्त, 'z' },

# यदि HASH_ALGO_CKSUM
  { "algorithm", required_तर्कः, रिक्त, 'a'},
  { "base64", no_तर्कः, रिक्त, BASE64_OPTION },
  { "debug", no_तर्कः, रिक्त, DEBUG_क्रमादेशः_OPTION},
  { "raw", no_तर्कः, रिक्त, RAW_OPTION},
  { "untagged", no_तर्कः, रिक्त, UNTAG_OPTION },
# endif
  { "binary", no_तर्कः, रिक्त, 'b' }, /* Deprecated.  */
  { "text", no_तर्कः, रिक्त, 't' }, /* Deprecated.  */

#अन्यथा
  {"sysv", no_तर्कः, रिक्त, 's'},  /* Not in cksum.  */
#यदि_अन्तः

  { GETOPT_HELP_OPTION_DECL },
  { GETOPT_संस्करणम्_OPTION_DECL },
  { रिक्त, 0, रिक्त, 0 }
};

शून्य
usage (पूर्णाङ्क स्थितिः)
{
  यदि (स्थितिः != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
    {
      प्रारूप_लिख (_("\
Usage: %s [OPTION]... [FILE]...\n\
"), क्रमादेशः_name);
#यदि HASH_ALGO_CKSUM
    यदि (legacy_बहुलकम्)
      {
        प्रारूप_लिख (_("\
Print or check %s checksums.\n\
"), DIGEST_TYPE_STRING);
        fputs (_("\
Legacy interface गन्तव्यम् the cksum(1) utility.\n\
"), stdout);
      }
    अन्यथा
      fputs (_("\
Print or verify checksums.\n\
By स्वभाव use the 32 bit CRC algorithm.\n\
"), stdout);
#अन्यथा
      प्रारूप_लिख (_("\
Print or check %s (%d-bit) checksums.\n\
"),
              DIGEST_TYPE_STRING,
              DIGEST_द्वयङ्काः);
      fputs (_("\
Legacy interface गन्तव्यम् the cksum(1) utility.\n\
"), stdout);
#यदि_अन्तः

      emit_stdin_note ();
#यदि HASH_ALGO_योग
      oputs (_("\
  -r\n\
         use BSD योग algorithm (the स्वभाव), use 1K खण्डानि\n\
"));
      oputs (_("\
  -s, --sysv\n\
         use System V योग algorithm, use 512 अष्टकानि खण्डानि\n\
"));
#यदि_अन्तः
#यदि HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
        emit_mandatory_arg_note ();
#यदि_अन्तः
#यदि HASH_ALGO_CKSUM
     यदि (! legacy_बहुलकम्)
        oputs (_("\
  -a, --algorithm=TYPE\n\
         select the digest type गन्तव्यम् use.  See DIGEST अधः\n\
"));
     यदि (! legacy_बहुलकम्)
        oputs (_("\
      --base64\n\
         emit base64-encoded digests, not hexadecimal\n\
"));
#यदि_अन्तः
#यदि !HASH_ALGO_योग
# यदि HASH_ALGO_CKSUM
     यदि (legacy_बहुलकम्)
       {
# endif
      यदि (O_द्विमान)
        oputs (_("\
  -b, --द्विमान\n\
         पठ अन्तर् द्विमान बहुलकम्_2 (स्वभाव unless reading tty stdin)\n\
"));
      अन्यथा
        oputs (_("\
  -b, --द्विमान\n\
         पठ अन्तर् द्विमान बहुलकम्_2\n\
"));
# यदि HASH_ALGO_CKSUM
       }
# endif
        oputs (_("\
  -c, --check\n\
         पठ checksums from the FILEs and check them\n\
"));
# यदि HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
#  यदि HASH_ALGO_CKSUM
     यदि (! legacy_बहुलकम्)
#  endif
        oputs (_("\
  -l, --दीर्घता=द्वयङ्काः\n\
         digest दीर्घता अन्तर् द्वयङ्काः_2; must not exceed the max आकार\n\
         and must be a गुणजः सम्बन्धि 8 प्रति blake2b;\n\
         must be 224, 256, 384, or 512 प्रति sha2 or sha3\n\
"));
# endif
# यदि HASH_ALGO_CKSUM
    यदि (! legacy_बहुलकम्)
        oputs (_("\
      --raw\n\
         emit a raw द्विमान digest, not hexadecimal\n\
"));
    यदि (legacy_बहुलकम्)
      oputs (_("\
      --चिह्नकम्\n\
         create a BSD-style checksum\n\
"));
    अन्यथा
      oputs (_("\
      --चिह्नकम्\n\
         create a BSD-style checksum (the स्वभाव)\n\
"));
    यदि (! legacy_बहुलकम्)
      oputs (_("\
      --untagged\n\
         create a reversed style checksum, without digest type\n\
"));
# अन्यथा
      oputs (_("\
      --चिह्नकम्\n\
         create a BSD-style checksum\n\
"));
# endif
# यदि HASH_ALGO_CKSUM
    यदि (legacy_बहुलकम्)
      {
# endif
      यदि (O_द्विमान)
        oputs (_("\
  -t, --text\n\
         पठ अन्तर् text बहुलकम्_2 (स्वभाव यदि reading tty stdin)\n\
"));
      अन्यथा
        oputs (_("\
  -t, --text\n\
         पठ अन्तर् text बहुलकम्_2 (स्वभाव)\n\
"));
# यदि HASH_ALGO_CKSUM
      }
# endif
      oputs (_("\
  -z, --zero\n\
         अन्तः each output पङ्क्तिः with NUL, not newline,\n\
         and disable file name escaping\n\
"));
      fputs (_("\
\n\
The following five options are useful only when verifying checksums:\n\
"), stdout);
      oputs (_("\
      --ignore-missing\n\
         don't fail or report स्थितिः प्रति missing files\n\
"));
      oputs (_("\
      --quiet\n\
         don't print OK प्रति each successfully verified file\n\
"));
      oputs (_("\
      --स्थितिः\n\
         don't output anything, स्थितिः संहिता shows success\n\
"));
      oputs (_("\
      --strict\n\
         निर्गच्छ non-zero प्रति improperly formatted checksum पङ्क्तयः\n\
"));
      oputs (_("\
  -w, --warn\n\
         warn about improperly formatted checksum पङ्क्तयः\n\
"));
#यदि_अन्तः
#यदि HASH_ALGO_CKSUM
    यदि (! legacy_बहुलकम्)
      oputs (_("\
      --debug\n\
         indicate which implementation प्रयुक्तम्\n\
"));
#यदि_अन्तः
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (संस्करणम्_OPTION_DESCRIPTION);
#यदि HASH_ALGO_CKSUM
    यदि (! legacy_बहुलकम्)
      fputs (_("\
\n\
DIGEST determines the digest algorithm and स्वभाव output format:\n\
  sysv      (equivalent गन्तव्यम् योग -s)\n\
  bsd       (equivalent गन्तव्यम् योग -r)\n\
  crc       (equivalent गन्तव्यम् cksum)\n\
  crc32b    (only उपलब्धम् through cksum)\n\
  md5       (equivalent गन्तव्यम् md5sum)\n\
  sha1      (equivalent गन्तव्यम् sha1sum)\n\
  sha2      (equivalent गन्तव्यम् sha{224,256,384,512}योग)\n\
  sha3      (only उपलब्धम् through cksum)\n\
  blake2b   (equivalent गन्तव्यम् b2sum)\n\
  sm3       (only उपलब्धम् through cksum)\n\
\n"), stdout);
#यदि_अन्तः
#यदि !HASH_ALGO_योग && !HASH_ALGO_CKSUM
      प्रारूप_लिख (_("\
\n\
The sums are computed as described अन्तर् %s.\n"), DIGEST_REFERENCE);
      fputs (_("\
When checking, the निवेशः should be a former output सम्बन्धि this क्रमादेशः.\n\
The स्वभाव बहुलकम्_2 is गन्तव्यम् print a पङ्क्तिः with: checksum, a space,\n\
a character indicating निवेशः बहुलकम्_2 ('*' प्रति द्विमान, ' ' प्रति text\n\
or where द्विमान is insignificant), and name प्रति each FILE.\n\
\n\
There is no difference between द्विमान बहुलकम्_2 and text बहुलकम्_2 on GNU systems.\
\n"), stdout);
#यदि_अन्तः
#यदि HASH_ALGO_CKSUM
      fputs (_("\
When checking, the निवेशः should be a former output सम्बन्धि this क्रमादेशः,\n\
or equivalent standalone क्रमादेशः.\
\n"), stdout);
#यदि_अन्तः
      emit_ancillary_info (क्रमादेशः_NAME);
    }

  निर्गच्छ (स्थितिः);
}

#यदि !HASH_ALGO_योग
/* Given a string S, return TRUE if it contains problematic characters
   that need escaping.  Note we escape '\' itself to provide some forward
   compat to introduce escaping of other characters.  */

ATTRIBUTE_विशुद्धम्
स्थिर bool
problematic_chars (वर्ण ध्रुव *s)
{
  idx_t दीर्घता = strcspn (s, "\\\n\r");
  प्रत्यागम s[दीर्घता] != '\0';
}
#यदि_अन्तः

/* Given a file name, S of length S_LEN, that is not NUL-terminated,
   modify it in place, performing the equivalent of this sed substitution:
   's/\\n/\n/g;s/\\r/\r/g;s/\\\\/\\/g' i.e., replacing each "\\n" string
   with a newline, each "\\r" string with a carriage return,
   and each "\\\\" with a single backslash, NUL-terminate it and return S.
   If S is not a valid escaped file name, i.e., if it ends with an odd number
   of backslashes or if it contains a backslash followed by anything other
   than "n" or another backslash, return NULL.  */

स्थिर वर्ण *
filename_unescape (वर्ण *s, idx_t s_len)
{
  वर्ण *dst = s;

  प्रति (idx_t i = 0; i < s_len; i++)
    {
      विकल्प (s[i])
        {
        पक्ष '\\':
          यदि (i == s_len - 1)
            {
              /* File name ends with an unescaped backslash: invalid.  */
              प्रत्यागम रिक्त;
            }
          ++i;
          विकल्प (s[i])
            {
            पक्ष 'n':
              *dst++ = '\n';
              भङ्ग;
            पक्ष 'r':
              *dst++ = '\r';
              भङ्ग;
            पक्ष '\\':
              *dst++ = '\\';
              भङ्ग;
            स्वभाव:
              /* Only '\', 'n' or 'r' may follow a backslash.  */
              प्रत्यागम रिक्त;
            }
          भङ्ग;

        पक्ष '\0':
          /* The file name may not contain a NUL.  */
          प्रत्यागम रिक्त;

        स्वभाव:
          *dst++ = s[i];
          भङ्ग;
        }
    }
  यदि (dst < s + s_len)
    *dst = '\0';

  प्रत्यागम s;
}

/* Return true if S is a LEN-byte NUL-terminated string of hex or base64
   digits and has the expected length.  Otherwise, return false.  */
ATTRIBUTE_विशुद्धम्
स्थिर bool
valid_digits (अचिह्नित वर्ण ध्रुव *s, idx_t len)
{
#यदि HASH_ALGO_CKSUM
  यदि (len == BASE64_दीर्घता (digest_दीर्घता >> 3))
    {
      idx_t i;
      प्रति (i = 0; i < len - digest_दीर्घता % 3; i++)
        {
          यदि (!isbase64 (*s))
            प्रत्यागम false;
          ++s;
        }
      प्रति ( ; i < len; i++)
        {
          यदि (*s != '=')
            प्रत्यागम false;
          ++s;
        }
    }
  अन्यथा
#यदि_अन्तः
  यदि (len == digest_hex_अष्टकानि)
    {
      प्रति (idx_t i = 0; i < digest_hex_अष्टकानि; i++)
        {
          यदि (!c_isxdigit (*s))
            प्रत्यागम false;
          ++s;
        }
    }
  अन्यथा
    प्रत्यागम false;

  प्रत्यागम *s == '\0';
}

/* Split the checksum string S (of length S_LEN) from a BSD 'md5' or
   'sha1' command into two parts: a hexadecimal digest, and the file
   name.  S is modified.  Set *D_LEN to the length of the digest string.
   Return true if successful.  */

स्थिर bool
bsd_विभज_3 (वर्ण *s, idx_t s_len,
             अचिह्नित वर्ण **digest, idx_t *d_len,
             वर्ण **file_name, bool escaped_filename)
{
  यदि (s_len == 0)
    प्रत्यागम false;

  /* Find end of filename.  */
  idx_t i = s_len - 1;
  यावत् (i && s[i] != ')')
    i--;

  यदि (s[i] != ')')
    प्रत्यागम false;

  *file_name = s;

  यदि (escaped_filename && filename_unescape (s, i) == रिक्त)
    प्रत्यागम false;

  s[i++] = '\0';

  यावत् (c_isblank (s[i]))
    i++;

  यदि (s[i] != '=')
    प्रत्यागम false;

  i++;

  यावत् (c_isblank (s[i]))
    i++;

  *digest = (अचिह्नित वर्ण *) &s[i];

  *d_len = s_len - i;
  प्रत्यागम valid_digits (*digest, *d_len);
}

#यदि HASH_ALGO_CKSUM
/* Return the corresponding Algorithm for the string S,
   or -1 for no match.  */

स्थिर ptrdiff_t
algorithm_from_चिह्नकम् (वर्ण *s)
{
  /* Limit check size to this length for perf reasons.  */
  स्थिर idx_t max_चिह्नकम्_len;
  यदि (! max_चिह्नकम्_len)
    {
      वर्ण ध्रुव * ध्रुव * चिह्नकम् = algorithm_चिह्नकानि;
      यावत् (*चिह्नकम्)
        {
          idx_t चिह्नकम्_len = वाक्यदैर्घ्यम् (*चिह्नकम्++);
          max_चिह्नकम्_len = MAX (चिह्नकम्_len, max_चिह्नकम्_len);
        }
    }

  idx_t i = 0;

  /* Find end of tag */
  यावत् (i <= max_चिह्नकम्_len && s[i] && ! c_isblank (s[i])
         && s[i] != '-' && s[i] != '(')
    ++i;

  यदि (i > max_चिह्नकम्_len)
    प्रत्यागम -1;

  /* Terminate tag, and lookup.  */
  वर्ण sep = s[i];
  s[i] = '\0';
  ptrdiff_t algo = argmatch_exact (s, algorithm_चिह्नकानि);
  s[i] = sep;

  प्रत्यागम algo;
}
#यदि_अन्तः

/* Split the string S (of length S_LEN) into three parts:
   a hexadecimal digest, binary flag, and the file name.
   S is modified.  Set *D_LEN to the length of the digest string.
   Return true if successful.  */

स्थिर bool
विभज_3 (वर्ण *s, idx_t s_len,
         अचिह्नित वर्ण **digest, idx_t *d_len, पूर्णाङ्क *द्विमान, वर्ण **file_name)
{
  bool escaped_filename = false;
  idx_t algo_name_len;

  idx_t i = 0;
  यावत् (c_isblank (s[i]))
    ++i;

  यदि (s[i] == '\\')
    {
      ++i;
      escaped_filename = true;
    }

  /* Check for BSD-style checksum line. */

#यदि HASH_ALGO_CKSUM
  यदि (! algorithm_specified || cksum_algorithm == sha2)
    {
      ptrdiff_t algo_चिह्नकम् = algorithm_from_चिह्नकम् (s + i);
      यदि (! algorithm_specified)
        {
          यदि (algo_चिह्नकम् >= 0)
            {
              यदि (algo_चिह्नकम् <= crc32b)
                प्रत्यागम false;  /* We don't support checking these formats.  */
              cksum_algorithm = algo_चिह्नकम्;
            }
          अन्यथा
            प्रत्यागम false;      /* We only support tagged format without -a.  */
        }
      अन्यथा
        {
          यदि (cksum_algorithm == sha2 && (algo_चिह्नकम् == sha2
              || algo_चिह्नकम् == sha224 || algo_चिह्नकम् == sha256
              || algo_चिह्नकम् == sha384 || algo_चिह्नकम् == sha512))
            cksum_algorithm = algo_चिह्नकम्;
        }
    }
#यदि_अन्तः

  /* Try to parse BSD or OpenSSL tagged format.  I.e.:
     openssl: MD5(f)= d41d8cd98f00b204e9800998ecf8427e
     bsd:     MD5 (f) = d41d8cd98f00b204e9800998ecf8427e  */

  idx_t parse_विस्थापनम् = i;
  algo_name_len = वाक्यदैर्घ्यम् (DIGEST_TYPE_STRING);
  यदि (STREQ_LEN (s + i, DIGEST_TYPE_STRING, algo_name_len))
    {
      i += algo_name_len;
#यदि HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM

# यदि HASH_ALGO_BLAKE2
      digest_दीर्घता = DIGEST_MAX_LEN * 8;
# अन्यथा
      digest_दीर्घता = algorithm_द्वयङ्काः[cksum_algorithm];
# endif
      यदि (s[i] == '-')  /* length specified. Not base64 */
        {
          ++i;
          intmax_t दीर्घता;
          वर्ण *siend;
          यदि (!c_isdigit (s[i])
              || xstrtoimax (s + i, &siend, 0, &दीर्घता, रिक्त) != LONGINT_OK)
            प्रत्यागम false;
# यदि HASH_ALGO_CKSUM
          अन्यथा यदि (cksum_algorithm == sha2 || cksum_algorithm == sha3)
            {
              यदि (दीर्घता != SHA224_DIGEST_आकार * 8
                  && दीर्घता != SHA256_DIGEST_आकार * 8
                  && दीर्घता != SHA384_DIGEST_आकार * 8
                  && दीर्घता != SHA512_DIGEST_आकार * 8)
                प्रत्यागम false;
            }
# endif
          अन्यथा यदि (!(0 < दीर्घता && दीर्घता <= digest_दीर्घता && दीर्घता % 8 == 0))
            प्रत्यागम false;

          i = siend - s;
          digest_दीर्घता = दीर्घता;
        }
      digest_hex_अष्टकानि = digest_दीर्घता >> 2;
#यदि_अन्तः
      यदि (s[i] == ' ')
        ++i;
      यदि (s[i] == '(')  /* not base64 */
        {
          ++i;
          *द्विमान = 0;
          प्रत्यागम bsd_विभज_3 (s + i, s_len - i,
                              digest, d_len, file_name, escaped_filename);
        }

      /* Note with --base64 --untagged format, we may have matched a "tag".
         Even very short digests with: cksum -a blake2b -l24 --untagged --base64
         So fallback to checking untagged format if issues detecting tags.  */
      i = parse_विस्थापनम्;
    }

  /* Ignore this line if it is too short.
     Each line must have at least 'min_digest_line_length - 1' (or one more, if
     the first is a backslash) more characters to contain correct message digest
     information.  */
  यदि (s_len - i < min_digest_पङ्क्तिः_दीर्घता + (s[i] == '\\'))
    प्रत्यागम false;

  *digest = (अचिह्नित वर्ण *) &s[i];

#यदि HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
  /* Auto determine length.  */
# यदि HASH_ALGO_CKSUM
  यदि (cksum_algorithm == blake2b
      || cksum_algorithm == sha2 || cksum_algorithm == sha3) {
# endif
  अचिह्नित वर्ण ध्रुव *hp = *digest;
  digest_hex_अष्टकानि = 0;
  प्रति (; c_isxdigit (*hp); ++hp, ++digest_hex_अष्टकानि)
    ;
# यदि HASH_ALGO_CKSUM
  /* Check the number of base64 characters.  This works because the hexadecimal
     character set is a subset of the base64 character set.
     Note there is the ambiguity that all characters are hex when they
     are actually base64 encoded, which could be ambiguous with:
        cksum -a sha2 -l 384 --base64 --untagged
        cksum -a sha2 -l 256 --untagged
     Similarly for sha3 and blake2b.
     However at this length the chances are exceedingly rare (1 in 480R),
     and smaller blake2b lengths aren't practical for verification anyway.  */
  idx_t digest_base64_अष्टकानि = digest_hex_अष्टकानि;
  idx_t trailing_equals = 0;
  प्रति (; isubase64 (*hp); ++hp, ++digest_base64_अष्टकानि)
    ;
  प्रति (; *hp == '='; ++hp, ++trailing_equals)
    ;
  यदि ((cksum_algorithm == sha2 || cksum_algorithm == sha3)
      && digest_hex_अष्टकानि >> 1 != SHA224_DIGEST_आकार
      && digest_hex_अष्टकानि >> 1 != SHA256_DIGEST_आकार
      && digest_hex_अष्टकानि >> 1 != SHA384_DIGEST_आकार
      && digest_hex_अष्टकानि >> 1 != SHA512_DIGEST_आकार)
    {
      यदि (digest_base64_अष्टकानि + trailing_equals
          == BASE64_दीर्घता (SHA224_DIGEST_आकार))
        digest_hex_अष्टकानि = SHA224_DIGEST_आकार * 2;
      अन्यथा यदि (digest_base64_अष्टकानि + trailing_equals
               == BASE64_दीर्घता (SHA256_DIGEST_आकार))
        digest_hex_अष्टकानि = SHA256_DIGEST_आकार * 2;
      अन्यथा यदि (digest_base64_अष्टकानि + trailing_equals
               == BASE64_दीर्घता (SHA384_DIGEST_आकार))
        digest_hex_अष्टकानि = SHA384_DIGEST_आकार * 2;
      अन्यथा यदि (digest_base64_अष्टकानि + trailing_equals
               == BASE64_दीर्घता (SHA512_DIGEST_आकार))
        digest_hex_अष्टकानि = SHA512_DIGEST_आकार * 2;
      अन्यथा
        प्रत्यागम false;
    }
  अन्यथा यदि (cksum_algorithm == blake2b
           && digest_hex_अष्टकानि < digest_base64_अष्टकानि)
    {
      प्रति (पूर्णाङ्क j = 8; j <= DIGEST_MAX_LEN * 8; j += 8)
        {
          यदि (BASE64_दीर्घता (j / 8) == digest_base64_अष्टकानि + trailing_equals
              && j % 3 == trailing_equals)
            {
              digest_hex_अष्टकानि = j / 4;
              भङ्ग;
            }
        }
    }
# endif
  यदि (digest_hex_अष्टकानि < 2 || digest_hex_अष्टकानि % 2
      || DIGEST_MAX_LEN * 2 < digest_hex_अष्टकानि)
    प्रत्यागम false;
  digest_दीर्घता = digest_hex_अष्टकानि * 4;
# यदि HASH_ALGO_CKSUM
  }
# endif
#यदि_अन्तः

  /* This field must be the hexadecimal or base64 representation
     of the message digest.  */
  यावत् (s[i] && !c_isblank (s[i]))
    i++;

  /* The digest must be followed by at least one whitespace character.  */
  यदि (i == s_len)
    प्रत्यागम false;

  *d_len = &s[i] - (वर्ण *) *digest;
  s[i++] = '\0';

  यदि (! valid_digits (*digest, *d_len))
    प्रत्यागम false;

  /* If "bsd reversed" format detected.  */
  यदि ((s_len - i == 1) || (s[i] != ' ' && s[i] != '*'))
    {
      /* Don't allow mixing bsd and standard formats,
         to minimize security issues with attackers
         renaming files with leading spaces.
         This assumes that with bsd format checksums
         that the first file name does not have
         a leading ' ' or '*'.  */
      यदि (bsd_reversed == 0)
        प्रत्यागम false;
      bsd_reversed = 1;
    }
  अन्यथा यदि (bsd_reversed != 1)
    {
      bsd_reversed = 0;
      *द्विमान = (s[i++] == '*');
    }

  /* All characters between the type indicator and end of line are
     significant -- that includes leading and trailing white space.  */
  *file_name = &s[i];

  यदि (escaped_filename)
    प्रत्यागम filename_unescape (&s[i], s_len - i) != रिक्त;

  प्रत्यागम true;
}

#यदि !HASH_ALGO_योग
/* If ESCAPE is true, then translate each:
   NEWLINE byte to the string, "\\n",
   CARRIAGE RETURN byte to the string, "\\r",
   and each backslash to "\\\\".  */
स्थिर शून्य
print_filename (वर्ण ध्रुव *file, bool escape)
{
  यदि (! escape)
    {
      fputs (file, stdout);
      प्रत्यागम;
    }

  यावत् (*file)
    {
      विकल्प (*file)
        {
        पक्ष '\n':
          fputs ("\\n", stdout);
          भङ्ग;

        पक्ष '\r':
          fputs ("\\r", stdout);
          भङ्ग;

        पक्ष '\\':
          fputs ("\\\\", stdout);
          भङ्ग;

        स्वभाव:
          अक्षरं_लिख (*file);
          भङ्ग;
        }
      file++;
    }
}
#यदि_अन्तः

/* An interface to the function, DIGEST_STREAM.
   Operate on FILENAME (it may be "-").

   *BINARY indicates whether the file is binary.  BINARY < 0 means it
   depends on whether binary mode makes any difference and the file is
   a terminal; in that case, clear *BINARY if the file was treated as
   text because it was a terminal.

   Put the checksum in *BIN_RESULT, which must be properly aligned.
   Put true in *MISSING if the file can't be opened due to ENOENT.
   Return true if successful.  */

स्थिर bool
digest_file (वर्ण ध्रुव *filename, पूर्णाङ्क *द्विमान, अचिह्नित वर्ण *bin_परिणाम,
             bool *missing, MAYBE_अप्रयुक्तम् intmax_t *दीर्घता)
{
  FILE *fp;
  पूर्णाङ्क err;
  bool is_stdin = streq (filename, "-");

  *missing = false;

  यदि (is_stdin)
    {
      have_read_stdin = true;
      fp = stdin;
      यदि (O_द्विमान && *द्विमान)
        {
          यदि (*द्विमान < 0)
            *द्विमान = ! isatty (STDIN_FILENO);
          यदि (*द्विमान)
            xset_द्विमान_बहुलकम् (STDIN_FILENO, O_द्विमान);
        }
    }
  अन्यथा
    {
      fp = fopen (filename, O_द्विमान ? (*द्विमान ? "rb" : "rt") : "r");
      यदि (fp == रिक्त)
        {
          यदि (ignore_missing && errno == ENOENT)
            {
              *missing = true;
              प्रत्यागम true;
            }
          error (0, errno, "%s", quotef (filename));
          प्रत्यागम false;
        }
    }

  fadvise (fp, FADVISE_SEQUENTIAL);

#यदि HASH_ALGO_CKSUM
  यदि (cksum_algorithm == blake2b
      || cksum_algorithm == sha2 || cksum_algorithm == sha3)
    *दीर्घता = digest_दीर्घता >> 3;
  err = DIGEST_प्रवाहः (fp, bin_परिणाम, दीर्घता);
#नोचेद्यदि HASH_ALGO_योग
  err = DIGEST_प्रवाहः (fp, bin_परिणाम, दीर्घता);
#नोचेद्यदि HASH_ALGO_BLAKE2
  err = DIGEST_प्रवाहः (fp, bin_परिणाम, digest_दीर्घता >> 3);
#अन्यथा
  err = DIGEST_प्रवाहः (fp, bin_परिणाम);
#यदि_अन्तः
  err = err ? errno : 0;
  यदि (is_stdin)
    clearerr (fp);
  अन्यथा यदि (fclose (fp) != 0 && !err)
    err = errno;

  यदि (err)
    {
      error (0, err, "%s", quotef (filename));
      प्रत्यागम false;
    }

  प्रत्यागम true;
}

#यदि !HASH_ALGO_योग
स्थिर शून्य
output_file (वर्ण ध्रुव *file, पूर्णाङ्क द्विमान_file, शून्य ध्रुव *digest,
             MAYBE_अप्रयुक्तम् bool raw, bool tagged, अचिह्नित वर्ण delim,
             MAYBE_अप्रयुक्तम् bool तर्काः, MAYBE_अप्रयुक्तम् intmax_t दीर्घता)
{
# यदि HASH_ALGO_CKSUM
  यदि (raw)
    {
      fwrite (digest, 1, digest_दीर्घता >> 3, stdout);
      प्रत्यागम;
    }
# endif

  अचिह्नित वर्ण ध्रुव *bin_बफर = digest;

  /* Output a leading backslash if the file name contains problematic chars.  */
  bool needs_escape = delim == '\n' && problematic_chars (file);

  यदि (needs_escape)
    अक्षरं_लिख ('\\');

  यदि (tagged)
    {
# यदि HASH_ALGO_CKSUM
      यदि (cksum_algorithm == sha2)
        प्रारूप_लिख ("SHA%u", (अचिह्नित पूर्णाङ्क) digest_दीर्घता);
      अन्यथा
# endif
      fputs (DIGEST_TYPE_STRING, stdout);
# यदि HASH_ALGO_BLAKE2
      यदि (digest_दीर्घता < DIGEST_MAX_LEN * 8)
        प्रारूप_लिख ("-%u", (अचिह्नित पूर्णाङ्क) digest_दीर्घता);
# elif HASH_ALGO_CKSUM
      यदि (cksum_algorithm == sha3)
        प्रारूप_लिख ("-%u", (अचिह्नित पूर्णाङ्क) digest_दीर्घता);
      यदि (cksum_algorithm == blake2b)
        {
          यदि (digest_दीर्घता < DIGEST_MAX_LEN * 8)
            प्रारूप_लिख ("-%u", (अचिह्नित पूर्णाङ्क) digest_दीर्घता);
        }
# endif
      fputs (" (", stdout);
      print_filename (file, needs_escape);
      fputs (") = ", stdout);
    }

# यदि HASH_ALGO_CKSUM
  यदि (base64_digest)
    {
      वर्ण b64[BASE64_दीर्घता (DIGEST_BIN_अष्टकानि) + 1];
      base64_encode ((वर्ण ध्रुव *) bin_बफर, digest_दीर्घता >> 3,
                     b64, परिमाण b64);
      fputs (b64, stdout);
    }
  अन्यथा
# endif
    {
      प्रति (idx_t i = 0; i < (digest_hex_अष्टकानि >> 1); ++i)
        प्रारूप_लिख ("%02x", bin_बफर[i]);
    }

  यदि (!tagged)
    {
      अक्षरं_लिख (' ');
      अक्षरं_लिख (द्विमान_file ? '*' : ' ');
      print_filename (file, needs_escape);
    }

  अक्षरं_लिख (delim);
}
#यदि_अन्तः

#यदि HASH_ALGO_CKSUM
/* Return true if B64_DIGEST is the same as the base64 digest of the
   DIGEST_LENGTH/8 bytes at BIN_BUFFER.  */
स्थिर bool
b64_equal (अचिह्नित वर्ण ध्रुव *b64_digest, अचिह्नित वर्ण ध्रुव *bin_बफर)
{
  idx_t b64_n_अष्टकानि = BASE64_दीर्घता (digest_दीर्घता >> 3);
  वर्ण b64[BASE64_दीर्घता (DIGEST_BIN_अष्टकानि) + 1];
  base64_encode ((वर्ण ध्रुव *) bin_बफर, digest_दीर्घता >> 3,
                 b64, परिमाण b64);
  प्रत्यागम memeq (b64_digest, b64, b64_n_अष्टकानि + 1);
}
#यदि_अन्तः

/* Return true if HEX_DIGEST is the same as the hex-encoded digest of the
   DIGEST_LENGTH/8 bytes at BIN_BUFFER.  */
स्थिर bool
hex_equal (अचिह्नित वर्ण ध्रुव *hex_digest, अचिह्नित वर्ण ध्रुव *bin_बफर)
{
  स्थिर ध्रुव वर्ण bin2hex[] = { '0', '1', '2', '3',
                                  '4', '5', '6', '7',
                                  '8', '9', 'a', 'b',
                                  'c', 'd', 'e', 'f' };
  idx_t digest_bin_अष्टकानि = digest_hex_अष्टकानि >> 1;

  /* Compare generated binary number with text representation
     in check file.  Ignore case of hex digits.  */
  idx_t cnt;
  प्रति (cnt = 0; cnt < digest_bin_अष्टकानि; ++cnt)
    {
      यदि (c_tolower (hex_digest[2 * cnt])
          != bin2hex[bin_बफर[cnt] >> 4]
          || (c_tolower (hex_digest[2 * cnt + 1])
              != (bin2hex[bin_बफर[cnt] & 0xf])))
        भङ्ग;
    }
  प्रत्यागम cnt == digest_bin_अष्टकानि;
}

स्थिर bool
digest_check (वर्ण ध्रुव *checkfile_name)
{
  FILE *checkfile_प्रवाहः;
  intmax_t n_misformatted_पङ्क्तयः = 0;
  intmax_t n_mismatched_checksums = 0;
  intmax_t n_open_or_read_failures = 0;
  bool properly_formatted_पङ्क्तयः = false;
  bool matched_checksums = false;
  अचिह्नित वर्ण bin_बफर_unaligned[DIGEST_BIN_अष्टकानि + DIGEST_ALIGN];
  /* Make sure bin_buffer is properly aligned. */
  अचिह्नित वर्ण *bin_बफर = ptr_align (bin_बफर_unaligned, DIGEST_ALIGN);
  intmax_t पङ्क्तिः_सङ्ख्या;
  वर्ण *पङ्क्तिः;
  size_t पङ्क्तिः_chars_allocated;
  bool is_stdin = streq (checkfile_name, "-");

  यदि (is_stdin)
    {
      have_read_stdin = true;
      checkfile_name = _("standard input");
      checkfile_प्रवाहः = stdin;
    }
  अन्यथा
    {
      checkfile_प्रवाहः = fopen (checkfile_name, "r");
      यदि (checkfile_प्रवाहः == रिक्त)
        {
          error (0, errno, "%s", quotef (checkfile_name));
          प्रत्यागम false;
        }
    }

  पङ्क्तिः_सङ्ख्या = 0;
  पङ्क्तिः = रिक्त;
  पङ्क्तिः_chars_allocated = 0;
  कुरु
    {
      वर्ण *filename;
      पूर्णाङ्क द्विमान;
      अचिह्नित वर्ण *digest;
      ssize_t पङ्क्तिः_दीर्घता;

      ++पङ्क्तिः_सङ्ख्या;
      यदि (पङ्क्तिः_सङ्ख्या == 0)
        error (EXIT_FAILURE, 0, _("%s: too many checksum lines"),
               quotef (checkfile_name));

      पङ्क्तिः_दीर्घता = getline (&पङ्क्तिः, &पङ्क्तिः_chars_allocated, checkfile_प्रवाहः);
      यदि (पङ्क्तिः_दीर्घता <= 0)
        भङ्ग;

      /* Ignore comment lines, which begin with a '#' character.  */
      यदि (पङ्क्तिः[0] == '#')
        अनुवर्त;

      /* Remove any trailing newline.  */
      पङ्क्तिः_दीर्घता -= पङ्क्तिः[पङ्क्तिः_दीर्घता - 1] == '\n';
      /* Remove any trailing carriage return.  */
      पङ्क्तिः_दीर्घता -= पङ्क्तिः[पङ्क्तिः_दीर्घता - (0 < पङ्क्तिः_दीर्घता)] == '\r';

      /* Ignore empty lines.  */
      यदि (पङ्क्तिः_दीर्घता == 0)
        अनुवर्त;

      पङ्क्तिः[पङ्क्तिः_दीर्घता] = '\0';

      idx_t d_len;
      यदि (! (विभज_3 (पङ्क्तिः, पङ्क्तिः_दीर्घता, &digest, &d_len, &द्विमान, &filename)
             && ! (is_stdin && streq (filename, "-"))))
        {
          ++n_misformatted_पङ्क्तयः;

          यदि (warn)
            {
              error (0, 0,
                     _("%s: %jd"
                       ": improperly formatted %s checksum line"),
                     quotef (checkfile_name), पङ्क्तिः_सङ्ख्या,
                     DIGEST_TYPE_STRING);
            }
        }
      अन्यथा
        {
          bool ok;
          bool missing;

          properly_formatted_पङ्क्तयः = true;

          intmax_t दीर्घता;
          ok = digest_file (filename, &द्विमान, bin_बफर, &missing, &दीर्घता);

          यदि (!ok)
            {
              ++n_open_or_read_failures;
              यदि (!स्थितिः_only)
                प्रारूप_लिख ("%s: %s\n", quotef (filename),_("FAILED open or read"));
            }
          अन्यथा यदि (ignore_missing && missing)
            {
              /* Ignore missing files with --ignore-missing.  */
              ;
            }
          अन्यथा
            {
              bool मेलनम् = false;
#यदि HASH_ALGO_CKSUM
              यदि (d_len == BASE64_दीर्घता (digest_दीर्घता >> 3))
                मेलनम् = b64_equal (digest, bin_बफर);
              अन्यथा
#यदि_अन्तः
                यदि (d_len == digest_hex_अष्टकानि)
                  मेलनम् = hex_equal (digest, bin_बफर);

              यदि (मेलनम्)
                matched_checksums = true;
              अन्यथा
                ++n_mismatched_checksums;

              यदि (!स्थितिः_only)
                {
                  यदि (! मेलनम् || ! quiet)
                    fputs (quotef (filename), stdout);

                  यदि (! मेलनम्)
                    प्रारूप_लिख (": %s\n", _("FAILED"));
                  अन्यथा यदि (!quiet)
                    प्रारूप_लिख (": %s\n", _("OK"));
                }
            }

          यदि (ferror (stdout))
            write_error ();
        }
    }
  यावत् (!feof (checkfile_प्रवाहः) && !ferror (checkfile_प्रवाहः));

  मुक्त (पङ्क्तिः);

  पूर्णाङ्क err = ferror (checkfile_प्रवाहः) ? 0 : -1;
  यदि (is_stdin)
    clearerr (checkfile_प्रवाहः);
  अन्यथा यदि (fclose (checkfile_प्रवाहः) != 0 && err < 0)
    err = errno;

  यदि (0 <= err)
    {
      error (0, err, err ? "%s" : _("%s: read error"),
             quotef (checkfile_name));
      प्रत्यागम false;
    }

  यदि (! properly_formatted_पङ्क्तयः)
    {
      /* Warn if no tests are found.  */
      error (0, 0, _("%s: no properly formatted checksum lines found"),
             quotef (checkfile_name));
    }
  अन्यथा
    {
      यदि (!स्थितिः_only)
        {
          यदि (n_misformatted_पङ्क्तयः != 0)
            error (0, 0,
                   (ngettext
                    ("WARNING: %jd line is improperly formatted",
                     "WARNING: %jd lines are improperly formatted",
                     select_plural (n_misformatted_पङ्क्तयः))),
                   n_misformatted_पङ्क्तयः);

          यदि (n_open_or_read_failures != 0)
            error (0, 0,
                   (ngettext
                    ("WARNING: %jd listed file could not be read",
                     "WARNING: %jd listed files could not be read",
                     select_plural (n_open_or_read_failures))),
                   n_open_or_read_failures);

          यदि (n_mismatched_checksums != 0)
            error (0, 0,
                   (ngettext
                    ("WARNING: %jd computed checksum did NOT match",
                     "WARNING: %jd computed checksums did NOT match",
                     select_plural (n_mismatched_checksums))),
                   n_mismatched_checksums);

          यदि (ignore_missing && ! matched_checksums)
            error (0, 0, _("%s: no file was verified"),
                   quotef (checkfile_name));
        }
    }

  प्रत्यागम (properly_formatted_पङ्क्तयः
          && matched_checksums
          && n_mismatched_checksums == 0
          && n_open_or_read_failures == 0
          && (!strict || n_misformatted_पङ्क्तयः == 0));
}

पूर्णाङ्क
मुख्यम् (पूर्णाङ्क argc, वर्ण **argv)
{
  अचिह्नित वर्ण bin_बफर_unaligned[DIGEST_BIN_अष्टकानि + DIGEST_ALIGN];
  /* Make sure bin_buffer is properly aligned. */
  अचिह्नित वर्ण *bin_बफर = ptr_align (bin_बफर_unaligned, DIGEST_ALIGN);
  bool do_check = false;
  पूर्णाङ्क opt;
  bool ok = true;
  पूर्णाङ्क द्विमान = -1;
  पूर्णाङ्क उपसर्गः_चिह्नकम् = -1;
  संरचना option ध्रुव *long_opts = long_options;

#यदि HASH_ALGO_CKSUM
  यदि (cksum_algorithm != crc)
    {
      legacy_बहुलकम् = true;
      उपसर्गः_चिह्नकम् = 0;
      algorithm_specified = true;
      long_opts = legacy_long_options;
    }
#यदि_अन्तः

  /* Setting values of global variables.  */
  initialize_main (&argc, &argv);
  समुच्चयः_क्रमादेशः_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* Line buffer stdout to ensure lines are written atomically and immediately
     so that processes running in parallel do not intersperse their output.  */
  setvbuf (stdout, रिक्त, _IOLBF, 0);

#यदि HASH_ALGO_योग
  वर्ण ध्रुव *short_opts = "rs";
#नोचेद्यदि HASH_ALGO_CKSUM
  वर्ण ध्रुव *short_opts = legacy_बहुलकम् ? "bctwz" : "a:l:bctwz";
  वर्ण ध्रुव *digest_दीर्घता_str = "";
#नोचेद्यदि HASH_ALGO_BLAKE2
  वर्ण ध्रुव *short_opts = "l:bctwz";
  वर्ण ध्रुव *digest_दीर्घता_str = "";
#अन्यथा
  वर्ण ध्रुव *short_opts = "bctwz";
#यदि_अन्तः

  यावत् ((opt = getopt_long (argc, argv, short_opts, long_opts, रिक्त))
         != -1)
    विकल्प (opt)
      {
#यदि HASH_ALGO_CKSUM
      पक्ष 'a':
        cksum_algorithm = XARGMATCH_EXACT ("--algorithm", optarg,
                                           algorithm_तर्काः, algorithm_types);
        algorithm_specified = true;
        भङ्ग;

      पक्ष DEBUG_क्रमादेशः_OPTION:
        cksum_debug = true;
        भङ्ग;
#यदि_अन्तः
#यदि HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
      पक्ष 'l':
        digest_दीर्घता = xnumtoimax (optarg, 10, 0, IDX_MAX, "",
                                    _("invalid length"), 0,
                                    XTOINT_MAX_QUIET);
        digest_दीर्घता_str = optarg;
        भङ्ग;
#यदि_अन्तः
#यदि !HASH_ALGO_योग
      पक्ष 'c':
        do_check = true;
        भङ्ग;
      पक्ष स्थितिः_OPTION:
        स्थितिः_only = true;
        warn = false;
        quiet = false;
        भङ्ग;
      पक्ष 'b':
        द्विमान = 1;
        भङ्ग;
      पक्ष 't':
        द्विमान = 0;
        भङ्ग;
      पक्ष 'w':
        स्थितिः_only = false;
        warn = true;
        quiet = false;
        भङ्ग;
      पक्ष IGNORE_MISSING_OPTION:
        ignore_missing = true;
        भङ्ग;
      पक्ष QUIET_OPTION:
        स्थितिः_only = false;
        warn = false;
        quiet = true;
        भङ्ग;
      पक्ष STRICT_OPTION:
        strict = true;
        भङ्ग;
# यदि HASH_ALGO_CKSUM
      पक्ष BASE64_OPTION:
        base64_digest = true;
        भङ्ग;
      पक्ष RAW_OPTION:
        raw_digest = true;
        भङ्ग;
      पक्ष UNTAG_OPTION:
        उपसर्गः_चिह्नकम् = 0;
        भङ्ग;
# endif
      पक्ष चिह्नकम्_OPTION:
        उपसर्गः_चिह्नकम् = 1;
        भङ्ग;
      पक्ष 'z':
        digest_delim = '\0';
        भङ्ग;
#यदि_अन्तः
#यदि HASH_ALGO_योग
      पक्ष 'r':		/* For SysV compatibility. */
        योग_algorithm = bsd;
        भङ्ग;

      पक्ष 's':
        योग_algorithm = sysv;
        भङ्ग;
#यदि_अन्तः
      case_GETOPT_HELP_CHAR;
      case_GETOPT_संस्करणम्_CHAR (क्रमादेशः_NAME, AUTHORS);
      स्वभाव:
        usage (EXIT_FAILURE);
      }

  min_digest_पङ्क्तिः_दीर्घता = MIN_DIGEST_पङ्क्तिः_दीर्घता;
#यदि HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# यदि HASH_ALGO_CKSUM
  यदि (digest_दीर्घता && (cksum_algorithm != blake2b
                        && cksum_algorithm != sha2
                        && cksum_algorithm != sha3))
    error (EXIT_FAILURE, 0,
           _("--length is only supported with --algorithm "
             "blake2b, sha2, or sha3"));
  यदि (cksum_algorithm == sha2 || cksum_algorithm == sha3)
    {
      /* Do not require --length with --check.  */
      यदि (digest_दीर्घता == 0 && *digest_दीर्घता_str == '\0' && ! do_check)
        error (EXIT_FAILURE, 0, _("--algorithm=%s requires specifying "
                                  "--length 224, 256, 384, or 512"),
               algorithm_तर्काः[cksum_algorithm]);
      /* If --check and --length are used we verify the digest length.  */
      यदि ((! do_check || *digest_दीर्घता_str != '\0')
          && digest_दीर्घता != SHA224_DIGEST_आकार * 8
          && digest_दीर्घता != SHA256_DIGEST_आकार * 8
          && digest_दीर्घता != SHA384_DIGEST_आकार * 8
          && digest_दीर्घता != SHA512_DIGEST_आकार * 8)
        {
          error (0, 0, _("invalid length: %s"), quote (digest_दीर्घता_str));
          error (EXIT_FAILURE, 0, _("digest length for %s must be "
                                    "224, 256, 384, or 512"),
                 quote (DIGEST_TYPE_STRING));
        }
    }
  अन्यथा
    {
      /* If the digest length checks for SHA-3 are satisfied, the less strict
         checks for BLAKE2 will also be.  */
# अन्यथा
    {
# endif
      यदि (digest_दीर्घता > DIGEST_MAX_LEN * 8)
        {
          error (0, 0, _("invalid length: %s"), quote (digest_दीर्घता_str));
          error (EXIT_FAILURE, 0,
                 _("maximum digest length for %s is %d bits"),
                 quote (DIGEST_TYPE_STRING),
                 DIGEST_MAX_LEN * 8);
        }
      यदि (digest_दीर्घता % 8 != 0)
        {
          error (0, 0, _("invalid length: %s"), quote (digest_दीर्घता_str));
          error (EXIT_FAILURE, 0, _("length is not a multiple of 8"));
        }
    }
  यदि (digest_दीर्घता == 0)
    {
# यदि HASH_ALGO_BLAKE2
      digest_दीर्घता = DIGEST_MAX_LEN * 8;
# अन्यथा
      digest_दीर्घता = algorithm_द्वयङ्काः[cksum_algorithm];
# endif
    }
  digest_hex_अष्टकानि = digest_दीर्घता >> 2;
#अन्यथा
  digest_hex_अष्टकानि = DIGEST_HEX_अष्टकानि;
#यदि_अन्तः

#यदि HASH_ALGO_CKSUM
  विकल्प (+cksum_algorithm)
    {
    पक्ष bsd:
    पक्ष sysv:
    पक्ष crc:
    पक्ष crc32b:
        यदि (do_check && algorithm_specified)
          error (EXIT_FAILURE, 0,
                 _("--check is not supported with "
                   "--algorithm={bsd,sysv,crc,crc32b}"));
        भङ्ग;
    }

  यदि (base64_digest && raw_digest)
   {
     error (0, 0, _("--base64 and --raw are mutually exclusive"));
     usage (EXIT_FAILURE);
   }
#यदि_अन्तः

  यदि (digest_delim != '\n' && do_check)
    {
      error (0, 0, _("the --zero option is not supported when "
                     "verifying checksums"));
      usage (EXIT_FAILURE);
    }
  यदि (1 <= उपसर्गः_चिह्नकम् && do_check)
    {
      /* Note we allow --untagged with --check to more
         seamlessly support --untagged in an emulation wrapper.  */
      error (0, 0, _("the --tag option is meaningless when "
                     "verifying checksums"));
      usage (EXIT_FAILURE);
    }

  यदि (0 <= द्विमान && do_check)
    {
      error (0, 0, _("the --binary and --text options are meaningless when "
                     "verifying checksums"));
      usage (EXIT_FAILURE);
    }

  यदि (ignore_missing && !do_check)
    {
      error (0, 0,
             _("the --ignore-missing option is meaningful only when "
               "verifying checksums"));
      usage (EXIT_FAILURE);
    }

  यदि (स्थितिः_only && !do_check)
    {
      error (0, 0,
       _("the --status option is meaningful only when verifying checksums"));
      usage (EXIT_FAILURE);
    }

  यदि (warn && !do_check)
    {
      error (0, 0,
       _("the --warn option is meaningful only when verifying checksums"));
      usage (EXIT_FAILURE);
    }

  यदि (quiet && !do_check)
    {
      error (0, 0,
       _("the --quiet option is meaningful only when verifying checksums"));
      usage (EXIT_FAILURE);
    }

  यदि (strict & !do_check)
   {
     error (0, 0,
        _("the --strict option is meaningful only when verifying checksums"));
     usage (EXIT_FAILURE);
   }

  यदि (उपसर्गः_चिह्नकम् == -1)
    उपसर्गः_चिह्नकम् = HASH_ALGO_CKSUM;

  यदि (उपसर्गः_चिह्नकम् && !द्विमान)
   {
     /* This could be supported in a backwards compatible way
        by prefixing the output line with a space in text mode.
        However that's invasive enough that it was agreed to
        not support this mode with --tag, as --text use cases
        are adequately supported by the default output format.  */
#यदि !HASH_ALGO_CKSUM
     error (0, 0, _("--tag does not support --text mode"));
#अन्यथा
     error (0, 0, _("--text mode is only supported with --untagged"));
#यदि_अन्तः
     usage (EXIT_FAILURE);
   }

  यदि (!O_द्विमान && द्विमान < 0)
    द्विमान = 0;
  अन्यथा यदि (उपसर्गः_चिह्नकम्)
    द्विमान = 1;

  वर्ण **सङ्कार्यम्_lim = argv + argc;
  यदि (optind == argc)
    *सङ्कार्यम्_lim++ = bad_cast ("-");
  अन्यथा यदि (1 < argc - optind && raw_digest)
    error (EXIT_FAILURE, 0,
           _("the --raw option is not supported with multiple files"));

  प्रति (वर्ण **operandp = argv + optind; operandp < सङ्कार्यम्_lim; operandp++)
    {
      वर्ण *file = *operandp;
      यदि (do_check)
        ok &= digest_check (file);
      अन्यथा
        {
          पूर्णाङ्क द्विमान_file = द्विमान;
          bool missing;
          intmax_t दीर्घता;

          यदि (! digest_file (file, &द्विमान_file, bin_बफर, &missing, &दीर्घता))
            ok = false;
          अन्यथा
            {
              DIGEST_बहिर्गमनम् (file, द्विमान_file, bin_बफर, raw_digest, उपसर्गः_चिह्नकम्,
                          digest_delim, optind != argc, दीर्घता);
              यदि (ferror (stdout))
                write_error ();
            }
        }
    }

  यदि (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, _("standard input"));

  प्रत्यागम ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
