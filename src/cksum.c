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

#ifndef HASH_ALGO_CKSUM
# define HASH_ALGO_CKSUM 0
#endif

#यदि HASH_ALGO_SUM || HASH_ALGO_CKSUM
# include "sum.h"
#endif
#यदि HASH_ALGO_CKSUM
# include "cksum.h"
# include "cksum_crc.h"
# include "base64.h"
#endif
#यदि HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# include "blake2/b2sum.h"
#endif
#यदि HASH_ALGO_MD5 || HASH_ALGO_CKSUM
# include "md5.h"
#endif
#यदि HASH_ALGO_SHA1 || HASH_ALGO_CKSUM
# include "sha1.h"
#endif
#यदि HASH_ALGO_SHA256 || HASH_ALGO_SHA224 || HASH_ALGO_CKSUM
# include "sha256.h"
#endif
#यदि HASH_ALGO_SHA512 || HASH_ALGO_SHA384 || HASH_ALGO_CKSUM
# include "sha512.h"
#endif
#यदि HASH_ALGO_CKSUM
# include "sha3.h"
#endif
#यदि HASH_ALGO_CKSUM
# include "sm3.h"
#endif
#अन्तर्भाव "fadvise.h"
#अन्तर्भाव "stdio--.h"
#अन्तर्भाव "xbinary-io.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#यदि HASH_ALGO_SUM
# define PROGRAM_NAME "sum"
# define DIGEST_TYPE_STRING "BSD"
# define DIGEST_STREAM sumfns[sum_algorithm]
# define DIGEST_OUT sum_output_fns[sum_algorithm]
# define DIGEST_BITS 16
# define DIGEST_ALIGN 4
#नोचेद्यदि HASH_ALGO_CKSUM
# define MAX_DIGEST_BITS 512
# define MAX_DIGEST_ALIGN 8
# define DIGEST_TYPE_STRING algorithm_tags[cksum_algorithm]
# define DIGEST_STREAM cksumfns[cksum_algorithm]
# define DIGEST_OUT cksum_output_fns[cksum_algorithm]
# define DIGEST_BITS MAX_DIGEST_BITS
# define DIGEST_ALIGN MAX_DIGEST_ALIGN
#नोचेद्यदि HASH_ALGO_MD5
# define PROGRAM_NAME "md5sum"
# define DIGEST_TYPE_STRING "MD5"
# define DIGEST_STREAM md5_stream
# define DIGEST_BITS 128
# define DIGEST_REFERENCE "RFC 1321"
# define DIGEST_ALIGN 4
#नोचेद्यदि HASH_ALGO_BLAKE2
# define PROGRAM_NAME "b2sum"
# define DIGEST_TYPE_STRING "BLAKE2b"
# define DIGEST_STREAM blake2b_stream
# define DIGEST_BITS 512
# define DIGEST_REFERENCE "RFC 7693"
# define DIGEST_ALIGN 8
#नोचेद्यदि HASH_ALGO_SHA1
# define PROGRAM_NAME "sha1sum"
# define DIGEST_TYPE_STRING "SHA1"
# define DIGEST_STREAM sha1_stream
# define DIGEST_BITS 160
# define DIGEST_REFERENCE "FIPS-180-1"
# define DIGEST_ALIGN 4
#नोचेद्यदि HASH_ALGO_SHA256
# define PROGRAM_NAME "sha256sum"
# define DIGEST_TYPE_STRING "SHA256"
# define DIGEST_STREAM sha256_stream
# define DIGEST_BITS 256
# define DIGEST_REFERENCE "FIPS-180-2"
# define DIGEST_ALIGN 4
#नोचेद्यदि HASH_ALGO_SHA224
# define PROGRAM_NAME "sha224sum"
# define DIGEST_TYPE_STRING "SHA224"
# define DIGEST_STREAM sha224_stream
# define DIGEST_BITS 224
# define DIGEST_REFERENCE "RFC 3874"
# define DIGEST_ALIGN 4
#नोचेद्यदि HASH_ALGO_SHA512
# define PROGRAM_NAME "sha512sum"
# define DIGEST_TYPE_STRING "SHA512"
# define DIGEST_STREAM sha512_stream
# define DIGEST_BITS 512
# define DIGEST_REFERENCE "FIPS-180-2"
# define DIGEST_ALIGN 8
#नोचेद्यदि HASH_ALGO_SHA384
# define PROGRAM_NAME "sha384sum"
# define DIGEST_TYPE_STRING "SHA384"
# define DIGEST_STREAM sha384_stream
# define DIGEST_BITS 384
# define DIGEST_REFERENCE "FIPS-180-2"
# define DIGEST_ALIGN 8
#अन्यथा
# error "Can't decide which hash algorithm to compile."
#endif
#यदि !HASH_ALGO_SUM && !HASH_ALGO_CKSUM
# define DIGEST_OUT output_file
#endif

#यदि HASH_ALGO_CKSUM
# define PROGRAM_NAME (!legacy_mode ? "cksum"			 \
                       : cksum_algorithm == md5 ? "md5sum"	 \
                       : cksum_algorithm == sha1 ? "sha1sum"	 \
                       : cksum_algorithm == sha224 ? "sha224sum" \
                       : cksum_algorithm == sha256 ? "sha256sum" \
                       : cksum_algorithm == sha384 ? "sha384sum" \
                       : cksum_algorithm == sha512 ? "sha512sum" \
                       : "cksum")
#endif

#यदि HASH_ALGO_SUM
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
#endif
#यदि !HASH_ALGO_BLAKE2 && !HASH_ALGO_CKSUM
# define DIGEST_HEX_BYTES (DIGEST_BITS / 4)
#endif
#define DIGEST_BIN_BYTES (DIGEST_BITS / 8)

/* The minimum length of a valid digest line.  This length does
   not include any newline character at the end of a line.  */
#यदि HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# define MIN_DIGEST_ओळ_LENGTH 3 /* With -l 8.  */
#अन्यथा
# define MIN_DIGEST_ओळ_LENGTH \
   (DIGEST_HEX_BYTES /* length of hexadecimal message digest */ \
    + 1 /* blank */ \
    + 1 /* minimum filename length */ )
#endif

#यदि !HASH_ALGO_SUM
स्थिर शून्य
output_file (वर्ण ध्रुव *file, पूर्णाङ्क द्विमान_file, शून्य ध्रुव *digest,
             bool raw, bool tagged, unsigned वर्ण delim, bool args,
             intmax_t length);
#endif

/* True if any of the files read were the standard input. */
स्थिर bool have_read_stdin;

/* The minimum length of a valid checksum line for the selected algorithm.  */
स्थिर idx_t min_digest_ओळ_length;

/* Set to the length of a digest hex string for the selected algorithm.  */
स्थिर idx_t digest_hex_bytes;

/* With --check, don't generate any output.
   The exit code indicates success or failure.  */
स्थिर bool status_only = false;

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
स्थिर unsigned वर्ण digest_delim = '\n';

#यदि HASH_ALGO_CKSUM
/* If true, print base64-encoded digests, not hex.  */
स्थिर bool base64_digest = false;
#endif

/* If true, print binary digests, not hex.  */
स्थिर bool raw_digest = false;

/* blake2b and sha3 allow the -l option.  Luckily they both have the same
   maximum digest size.  */
#यदि HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# यदि HASH_ALGO_CKSUM
static_assert (BLAKE2B_OUTBYTES == SHA3_512_DIGEST_SIZE);
# endif
# define DIGEST_MAX_LEN BLAKE2B_OUTBYTES
स्थिर idx_t digest_length;
#endif /* HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM */

प्रकारनिर्देश शून्य (*digest_output_fn)(वर्ण ध्रुव *, पूर्णाङ्क, शून्य ध्रुव *, bool,
                                 bool, unsigned वर्ण, bool, intmax_t);
#यदि HASH_ALGO_SUM
गणना Algorithm
{
  bsd,
  sysv,
};

स्थिर गणना Algorithm sum_algorithm;
स्थिर sumfn sumfns[]=
{
  bsd_sum_stream,
  sysv_sum_stream,
};
स्थिर digest_output_fn sum_output_fns[]=
{
  output_bsd,
  output_sysv,
};
#endif

#यदि HASH_ALGO_CKSUM
स्थिर पूर्णाङ्क
md5_sum_stream (FILE *stream, शून्य *resstream, MAYBE_UNUSED intmax_t *length)
{
  प्रत्यागम md5_stream (stream, resstream);
}
स्थिर पूर्णाङ्क
sha1_sum_stream (FILE *stream, शून्य *resstream, MAYBE_UNUSED intmax_t *length)
{
  प्रत्यागम sha1_stream (stream, resstream);
}
स्थिर पूर्णाङ्क
sha224_sum_stream (FILE *stream, शून्य *resstream,
                   MAYBE_UNUSED intmax_t *length)
{
  प्रत्यागम sha224_stream (stream, resstream);
}
स्थिर पूर्णाङ्क
sha256_sum_stream (FILE *stream, शून्य *resstream,
                   MAYBE_UNUSED intmax_t *length)
{
  प्रत्यागम sha256_stream (stream, resstream);
}
स्थिर पूर्णाङ्क
sha384_sum_stream (FILE *stream, शून्य *resstream,
                   MAYBE_UNUSED intmax_t *length)
{
  प्रत्यागम sha384_stream (stream, resstream);
}
स्थिर पूर्णाङ्क
sha512_sum_stream (FILE *stream, शून्य *resstream,
                   MAYBE_UNUSED intmax_t *length)
{
  प्रत्यागम sha512_stream (stream, resstream);
}
स्थिर पूर्णाङ्क
sha2_sum_stream (FILE *stream, शून्य *resstream, intmax_t *length)
{
  विकल्प (*length)
    {
    पक्ष SHA224_DIGEST_SIZE:
      प्रत्यागम sha224_stream (stream, resstream);
    पक्ष SHA256_DIGEST_SIZE:
      प्रत्यागम sha256_stream (stream, resstream);
    पक्ष SHA384_DIGEST_SIZE:
      प्रत्यागम sha384_stream (stream, resstream);
    पक्ष SHA512_DIGEST_SIZE:
      प्रत्यागम sha512_stream (stream, resstream);
    स्वभाव:
      affirm (false);
    }
}
स्थिर पूर्णाङ्क
sha3_sum_stream (FILE *stream, शून्य *resstream, intmax_t *length)
{
  विकल्प (*length)
    {
    पक्ष SHA3_224_DIGEST_SIZE:
      प्रत्यागम sha3_224_stream (stream, resstream);
    पक्ष SHA3_256_DIGEST_SIZE:
      प्रत्यागम sha3_256_stream (stream, resstream);
    पक्ष SHA3_384_DIGEST_SIZE:
      प्रत्यागम sha3_384_stream (stream, resstream);
    पक्ष SHA3_512_DIGEST_SIZE:
      प्रत्यागम sha3_512_stream (stream, resstream);
    स्वभाव:
      affirm (false);
    }
}
स्थिर पूर्णाङ्क
blake2b_sum_stream (FILE *stream, शून्य *resstream, intmax_t *length)
{
  प्रत्यागम blake2b_stream (stream, resstream, *length);
}
स्थिर पूर्णाङ्क
sm3_sum_stream (FILE *stream, शून्य *resstream, MAYBE_UNUSED intmax_t *length)
{
  प्रत्यागम sm3_stream (stream, resstream);
}

स्थिर वर्ण ध्रुव *ध्रुव algorithm_args[] =
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
ARGMATCH_VERIFY (algorithm_args, algorithm_types);

स्थिर वर्ण ध्रुव *ध्रुव algorithm_tags[] =
{
  "BSD", "SYSV", "CRC", "CRC32B", "MD5", "SHA1",
  "SHA224", "SHA256", "SHA384", "SHA512",
  "SHA2", "SHA3", "BLAKE2b", "SM3", रिक्त
};
स्थिर पूर्णाङ्क ध्रुव algorithm_bits[] =
{
  16, 16, 32, 32, 128, 160,
  224, 256, 384, 512,
  512, 512, 512, 256, 0
};

static_assert (countof (algorithm_bits) == countof (algorithm_args));

स्थिर bool algorithm_specified = false;
स्थिर bool legacy_mode = false;
गणना Algorithm cksum_algorithm = crc;

स्थिर sumfn cksumfns[]=
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
#endif

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */

गणना
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

#यदि HASH_ALGO_CKSUM
स्थिर संरचना option ध्रुव legacy_long_options[] =
{
  { "check", no_argument, रिक्त, 'c' },
  { "ignore-missing", no_argument, रिक्त, IGNORE_MISSING_OPTION },
  { "quiet", no_argument, रिक्त, QUIET_OPTION },
  { "status", no_argument, रिक्त, STATUS_OPTION },
  { "warn", no_argument, रिक्त, 'w' },
  { "strict", no_argument, रिक्त, STRICT_OPTION },
  { "tag", no_argument, रिक्त, TAG_OPTION },
  { "zero", no_argument, रिक्त, 'z' },

  { "binary", no_argument, रिक्त, 'b' }, /* Deprecated.  */
  { "text", no_argument, रिक्त, 't' }, /* Deprecated.  */

  { GETOPT_HELP_OPTION_DECL },
  { GETOPT_VERSION_OPTION_DECL },
  { रिक्त, 0, रिक्त, 0 }
};
#endif

स्थिर संरचना option ध्रुव long_options[] =
{
#यदि HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
  { "length", required_argument, रिक्त, 'l'},
#endif

#यदि !HASH_ALGO_SUM
  { "check", no_argument, रिक्त, 'c' },
  { "ignore-missing", no_argument, रिक्त, IGNORE_MISSING_OPTION},
  { "quiet", no_argument, रिक्त, QUIET_OPTION },
  { "status", no_argument, रिक्त, STATUS_OPTION },
  { "warn", no_argument, रिक्त, 'w' },
  { "strict", no_argument, रिक्त, STRICT_OPTION },
  { "tag", no_argument, रिक्त, TAG_OPTION },
  { "zero", no_argument, रिक्त, 'z' },

# यदि HASH_ALGO_CKSUM
  { "algorithm", required_argument, रिक्त, 'a'},
  { "base64", no_argument, रिक्त, BASE64_OPTION },
  { "debug", no_argument, रिक्त, DEBUG_PROGRAM_OPTION},
  { "raw", no_argument, रिक्त, RAW_OPTION},
  { "untagged", no_argument, रिक्त, UNTAG_OPTION },
# endif
  { "binary", no_argument, रिक्त, 'b' }, /* Deprecated.  */
  { "text", no_argument, रिक्त, 't' }, /* Deprecated.  */

#अन्यथा
  {"sysv", no_argument, रिक्त, 's'},  /* Not in cksum.  */
#endif

  { GETOPT_HELP_OPTION_DECL },
  { GETOPT_VERSION_OPTION_DECL },
  { रिक्त, 0, रिक्त, 0 }
};

शून्य
usage (पूर्णाङ्क status)
{
  यदि (status != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"), program_name);
#यदि HASH_ALGO_CKSUM
    यदि (legacy_mode)
      {
        printf (_("\
Print or check %s checksums.\n\
"), DIGEST_TYPE_STRING);
        fputs (_("\
Legacy interface to the cksum(1) utility.\n\
"), stdout);
      }
    अन्यथा
      fputs (_("\
Print or verify checksums.\n\
By स्वभाव use the 32 bit CRC algorithm.\n\
"), stdout);
#अन्यथा
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
#यदि HASH_ALGO_SUM
      oputs (_("\
  -r\n\
         use BSD sum algorithm (the स्वभाव), use 1K blocks\n\
"));
      oputs (_("\
  -s, --sysv\n\
         use System V sum algorithm, use 512 bytes blocks\n\
"));
#endif
#यदि HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
        emit_mandatory_arg_note ();
#endif
#यदि HASH_ALGO_CKSUM
     यदि (! legacy_mode)
        oputs (_("\
  -a, --algorithm=TYPE\n\
         select the digest type to use.  See DIGEST below\n\
"));
     यदि (! legacy_mode)
        oputs (_("\
      --base64\n\
         emit base64-encoded digests, not hexadecimal\n\
"));
#endif
#यदि !HASH_ALGO_SUM
# यदि HASH_ALGO_CKSUM
     यदि (legacy_mode)
       {
# endif
      यदि (O_द्विमान)
        oputs (_("\
  -b, --द्विमान\n\
         पठ in द्विमान mode (स्वभाव unless reading tty stdin)\n\
"));
      अन्यथा
        oputs (_("\
  -b, --द्विमान\n\
         पठ in द्विमान mode\n\
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
     यदि (! legacy_mode)
#  endif
        oputs (_("\
  -l, --length=BITS\n\
         digest length in bits; must not exceed the max size\n\
         and must be a multiple of 8 प्रति blake2b;\n\
         must be 224, 256, 384, or 512 प्रति sha2 or sha3\n\
"));
# endif
# यदि HASH_ALGO_CKSUM
    यदि (! legacy_mode)
        oputs (_("\
      --raw\n\
         emit a raw द्विमान digest, not hexadecimal\n\
"));
    यदि (legacy_mode)
      oputs (_("\
      --tag\n\
         create a BSD-style checksum\n\
"));
    अन्यथा
      oputs (_("\
      --tag\n\
         create a BSD-style checksum (the स्वभाव)\n\
"));
    यदि (! legacy_mode)
      oputs (_("\
      --untagged\n\
         create a reversed style checksum, without digest type\n\
"));
# अन्यथा
      oputs (_("\
      --tag\n\
         create a BSD-style checksum\n\
"));
# endif
# यदि HASH_ALGO_CKSUM
    यदि (legacy_mode)
      {
# endif
      यदि (O_द्विमान)
        oputs (_("\
  -t, --text\n\
         पठ in text mode (स्वभाव यदि reading tty stdin)\n\
"));
      अन्यथा
        oputs (_("\
  -t, --text\n\
         पठ in text mode (स्वभाव)\n\
"));
# यदि HASH_ALGO_CKSUM
      }
# endif
      oputs (_("\
  -z, --zero\n\
         end each output ओळ with NUL, not newline,\n\
         and disable file name escaping\n\
"));
      fputs (_("\
\n\
The following five options are useful only when verifying checksums:\n\
"), stdout);
      oputs (_("\
      --ignore-missing\n\
         don't fail or report status प्रति missing files\n\
"));
      oputs (_("\
      --quiet\n\
         don't print OK प्रति each successfully verified file\n\
"));
      oputs (_("\
      --status\n\
         don't output anything, status code shows success\n\
"));
      oputs (_("\
      --strict\n\
         exit non-zero प्रति improperly formatted checksum ओळी\n\
"));
      oputs (_("\
  -w, --warn\n\
         warn about improperly formatted checksum ओळी\n\
"));
#endif
#यदि HASH_ALGO_CKSUM
    यदि (! legacy_mode)
      oputs (_("\
      --debug\n\
         indicate which implementation used\n\
"));
#endif
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
#यदि HASH_ALGO_CKSUM
    यदि (! legacy_mode)
      fputs (_("\
\n\
DIGEST determines the digest algorithm and स्वभाव output format:\n\
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
#यदि !HASH_ALGO_SUM && !HASH_ALGO_CKSUM
      printf (_("\
\n\
The sums are computed as described in %s.\n"), DIGEST_REFERENCE);
      fputs (_("\
When checking, the आदान should be a former output of this program.\n\
The स्वभाव mode is to print a ओळ with: checksum, a space,\n\
a character indicating आदान mode ('*' प्रति द्विमान, ' ' प्रति text\n\
or where द्विमान is insignificant), and name प्रति each FILE.\n\
\n\
There is no difference between द्विमान mode and text mode on GNU systems.\
\n"), stdout);
#endif
#यदि HASH_ALGO_CKSUM
      fputs (_("\
When checking, the आदान should be a former output of this program,\n\
or equivalent standalone program.\
\n"), stdout);
#endif
      emit_ancillary_info (PROGRAM_NAME);
    }

  exit (status);
}

#यदि !HASH_ALGO_SUM
/* Given a string S, return TRUE if it contains problematic characters
   that need escaping.  Note we escape '\' itself to provide some forward
   compat to introduce escaping of other characters.  */

ATTRIBUTE_PURE
स्थिर bool
problematic_chars (वर्ण ध्रुव *s)
{
  idx_t length = strcspn (s, "\\\n\r");
  प्रत्यागम s[length] != '\0';
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
ATTRIBUTE_PURE
स्थिर bool
valid_digits (unsigned वर्ण ध्रुव *s, idx_t len)
{
#यदि HASH_ALGO_CKSUM
  यदि (len == BASE64_LENGTH (digest_length >> 3))
    {
      idx_t i;
      प्रति (i = 0; i < len - digest_length % 3; i++)
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
#endif
  यदि (len == digest_hex_bytes)
    {
      प्रति (idx_t i = 0; i < digest_hex_bytes; i++)
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
bsd_split_3 (वर्ण *s, idx_t s_len,
             unsigned वर्ण **digest, idx_t *d_len,
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

  *digest = (unsigned वर्ण *) &s[i];

  *d_len = s_len - i;
  प्रत्यागम valid_digits (*digest, *d_len);
}

#यदि HASH_ALGO_CKSUM
/* Return the corresponding Algorithm for the string S,
   or -1 for no match.  */

स्थिर ptrdiff_t
algorithm_from_tag (वर्ण *s)
{
  /* Limit check size to this length for perf reasons.  */
  स्थिर idx_t max_tag_len;
  यदि (! max_tag_len)
    {
      वर्ण ध्रुव * ध्रुव * tag = algorithm_tags;
      यावत् (*tag)
        {
          idx_t tag_len = strlen (*tag++);
          max_tag_len = MAX (tag_len, max_tag_len);
        }
    }

  idx_t i = 0;

  /* Find end of tag */
  यावत् (i <= max_tag_len && s[i] && ! c_isblank (s[i])
         && s[i] != '-' && s[i] != '(')
    ++i;

  यदि (i > max_tag_len)
    प्रत्यागम -1;

  /* Terminate tag, and lookup.  */
  वर्ण sep = s[i];
  s[i] = '\0';
  ptrdiff_t algo = argmatch_exact (s, algorithm_tags);
  s[i] = sep;

  प्रत्यागम algo;
}
#endif

/* Split the string S (of length S_LEN) into three parts:
   a hexadecimal digest, binary flag, and the file name.
   S is modified.  Set *D_LEN to the length of the digest string.
   Return true if successful.  */

स्थिर bool
split_3 (वर्ण *s, idx_t s_len,
         unsigned वर्ण **digest, idx_t *d_len, पूर्णाङ्क *द्विमान, वर्ण **file_name)
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
      ptrdiff_t algo_tag = algorithm_from_tag (s + i);
      यदि (! algorithm_specified)
        {
          यदि (algo_tag >= 0)
            {
              यदि (algo_tag <= crc32b)
                प्रत्यागम false;  /* We don't support checking these formats.  */
              cksum_algorithm = algo_tag;
            }
          अन्यथा
            प्रत्यागम false;      /* We only support tagged format without -a.  */
        }
      अन्यथा
        {
          यदि (cksum_algorithm == sha2 && (algo_tag == sha2
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
  यदि (STREQ_LEN (s + i, DIGEST_TYPE_STRING, algo_name_len))
    {
      i += algo_name_len;
#यदि HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM

# यदि HASH_ALGO_BLAKE2
      digest_length = DIGEST_MAX_LEN * 8;
# अन्यथा
      digest_length = algorithm_bits[cksum_algorithm];
# endif
      यदि (s[i] == '-')  /* length specified. Not base64 */
        {
          ++i;
          intmax_t length;
          वर्ण *siend;
          यदि (!c_isdigit (s[i])
              || xstrtoimax (s + i, &siend, 0, &length, रिक्त) != LONGINT_OK)
            प्रत्यागम false;
# यदि HASH_ALGO_CKSUM
          अन्यथा यदि (cksum_algorithm == sha2 || cksum_algorithm == sha3)
            {
              यदि (length != SHA224_DIGEST_SIZE * 8
                  && length != SHA256_DIGEST_SIZE * 8
                  && length != SHA384_DIGEST_SIZE * 8
                  && length != SHA512_DIGEST_SIZE * 8)
                प्रत्यागम false;
            }
# endif
          अन्यथा यदि (!(0 < length && length <= digest_length && length % 8 == 0))
            प्रत्यागम false;

          i = siend - s;
          digest_length = length;
        }
      digest_hex_bytes = digest_length >> 2;
#endif
      यदि (s[i] == ' ')
        ++i;
      यदि (s[i] == '(')  /* not base64 */
        {
          ++i;
          *द्विमान = 0;
          प्रत्यागम bsd_split_3 (s + i, s_len - i,
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
  यदि (s_len - i < min_digest_ओळ_length + (s[i] == '\\'))
    प्रत्यागम false;

  *digest = (unsigned वर्ण *) &s[i];

#यदि HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
  /* Auto determine length.  */
# यदि HASH_ALGO_CKSUM
  यदि (cksum_algorithm == blake2b
      || cksum_algorithm == sha2 || cksum_algorithm == sha3) {
# endif
  unsigned वर्ण ध्रुव *hp = *digest;
  digest_hex_bytes = 0;
  प्रति (; c_isxdigit (*hp); ++hp, ++digest_hex_bytes)
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
  idx_t digest_base64_bytes = digest_hex_bytes;
  idx_t trailing_equals = 0;
  प्रति (; isubase64 (*hp); ++hp, ++digest_base64_bytes)
    ;
  प्रति (; *hp == '='; ++hp, ++trailing_equals)
    ;
  यदि ((cksum_algorithm == sha2 || cksum_algorithm == sha3)
      && digest_hex_bytes >> 1 != SHA224_DIGEST_SIZE
      && digest_hex_bytes >> 1 != SHA256_DIGEST_SIZE
      && digest_hex_bytes >> 1 != SHA384_DIGEST_SIZE
      && digest_hex_bytes >> 1 != SHA512_DIGEST_SIZE)
    {
      यदि (digest_base64_bytes + trailing_equals
          == BASE64_LENGTH (SHA224_DIGEST_SIZE))
        digest_hex_bytes = SHA224_DIGEST_SIZE * 2;
      अन्यथा यदि (digest_base64_bytes + trailing_equals
               == BASE64_LENGTH (SHA256_DIGEST_SIZE))
        digest_hex_bytes = SHA256_DIGEST_SIZE * 2;
      अन्यथा यदि (digest_base64_bytes + trailing_equals
               == BASE64_LENGTH (SHA384_DIGEST_SIZE))
        digest_hex_bytes = SHA384_DIGEST_SIZE * 2;
      अन्यथा यदि (digest_base64_bytes + trailing_equals
               == BASE64_LENGTH (SHA512_DIGEST_SIZE))
        digest_hex_bytes = SHA512_DIGEST_SIZE * 2;
      अन्यथा
        प्रत्यागम false;
    }
  अन्यथा यदि (cksum_algorithm == blake2b
           && digest_hex_bytes < digest_base64_bytes)
    {
      प्रति (पूर्णाङ्क j = 8; j <= DIGEST_MAX_LEN * 8; j += 8)
        {
          यदि (BASE64_LENGTH (j / 8) == digest_base64_bytes + trailing_equals
              && j % 3 == trailing_equals)
            {
              digest_hex_bytes = j / 4;
              भङ्ग;
            }
        }
    }
# endif
  यदि (digest_hex_bytes < 2 || digest_hex_bytes % 2
      || DIGEST_MAX_LEN * 2 < digest_hex_bytes)
    प्रत्यागम false;
  digest_length = digest_hex_bytes * 4;
# यदि HASH_ALGO_CKSUM
  }
# endif
#endif

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

#यदि !HASH_ALGO_SUM
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
          putchar (*file);
          भङ्ग;
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

स्थिर bool
digest_file (वर्ण ध्रुव *filename, पूर्णाङ्क *द्विमान, unsigned वर्ण *bin_result,
             bool *missing, MAYBE_UNUSED intmax_t *length)
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
            xset_द्विमान_mode (STDIN_FILENO, O_द्विमान);
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
    *length = digest_length >> 3;
  err = DIGEST_STREAM (fp, bin_result, length);
#नोचेद्यदि HASH_ALGO_SUM
  err = DIGEST_STREAM (fp, bin_result, length);
#नोचेद्यदि HASH_ALGO_BLAKE2
  err = DIGEST_STREAM (fp, bin_result, digest_length >> 3);
#अन्यथा
  err = DIGEST_STREAM (fp, bin_result);
#endif
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

#यदि !HASH_ALGO_SUM
स्थिर शून्य
output_file (वर्ण ध्रुव *file, पूर्णाङ्क द्विमान_file, शून्य ध्रुव *digest,
             MAYBE_UNUSED bool raw, bool tagged, unsigned वर्ण delim,
             MAYBE_UNUSED bool args, MAYBE_UNUSED intmax_t length)
{
# यदि HASH_ALGO_CKSUM
  यदि (raw)
    {
      fwrite (digest, 1, digest_length >> 3, stdout);
      प्रत्यागम;
    }
# endif

  unsigned वर्ण ध्रुव *bin_buffer = digest;

  /* Output a leading backslash if the file name contains problematic chars.  */
  bool needs_escape = delim == '\n' && problematic_chars (file);

  यदि (needs_escape)
    putchar ('\\');

  यदि (tagged)
    {
# यदि HASH_ALGO_CKSUM
      यदि (cksum_algorithm == sha2)
        printf ("SHA%u", (unsigned पूर्णाङ्क) digest_length);
      अन्यथा
# endif
      fputs (DIGEST_TYPE_STRING, stdout);
# यदि HASH_ALGO_BLAKE2
      यदि (digest_length < DIGEST_MAX_LEN * 8)
        printf ("-%u", (unsigned पूर्णाङ्क) digest_length);
# elif HASH_ALGO_CKSUM
      यदि (cksum_algorithm == sha3)
        printf ("-%u", (unsigned पूर्णाङ्क) digest_length);
      यदि (cksum_algorithm == blake2b)
        {
          यदि (digest_length < DIGEST_MAX_LEN * 8)
            printf ("-%u", (unsigned पूर्णाङ्क) digest_length);
        }
# endif
      fputs (" (", stdout);
      print_filename (file, needs_escape);
      fputs (") = ", stdout);
    }

# यदि HASH_ALGO_CKSUM
  यदि (base64_digest)
    {
      वर्ण b64[BASE64_LENGTH (DIGEST_BIN_BYTES) + 1];
      base64_encode ((वर्ण ध्रुव *) bin_buffer, digest_length >> 3,
                     b64, परिमाण b64);
      fputs (b64, stdout);
    }
  अन्यथा
# endif
    {
      प्रति (idx_t i = 0; i < (digest_hex_bytes >> 1); ++i)
        printf ("%02x", bin_buffer[i]);
    }

  यदि (!tagged)
    {
      putchar (' ');
      putchar (द्विमान_file ? '*' : ' ');
      print_filename (file, needs_escape);
    }

  putchar (delim);
}
#endif

#यदि HASH_ALGO_CKSUM
/* Return true if B64_DIGEST is the same as the base64 digest of the
   DIGEST_LENGTH/8 bytes at BIN_BUFFER.  */
स्थिर bool
b64_equal (unsigned वर्ण ध्रुव *b64_digest, unsigned वर्ण ध्रुव *bin_buffer)
{
  idx_t b64_n_bytes = BASE64_LENGTH (digest_length >> 3);
  वर्ण b64[BASE64_LENGTH (DIGEST_BIN_BYTES) + 1];
  base64_encode ((वर्ण ध्रुव *) bin_buffer, digest_length >> 3,
                 b64, परिमाण b64);
  प्रत्यागम memeq (b64_digest, b64, b64_n_bytes + 1);
}
#endif

/* Return true if HEX_DIGEST is the same as the hex-encoded digest of the
   DIGEST_LENGTH/8 bytes at BIN_BUFFER.  */
स्थिर bool
hex_equal (unsigned वर्ण ध्रुव *hex_digest, unsigned वर्ण ध्रुव *bin_buffer)
{
  स्थिर ध्रुव वर्ण bin2hex[] = { '0', '1', '2', '3',
                                  '4', '5', '6', '7',
                                  '8', '9', 'a', 'b',
                                  'c', 'd', 'e', 'f' };
  idx_t digest_bin_bytes = digest_hex_bytes >> 1;

  /* Compare generated binary number with text representation
     in check file.  Ignore case of hex digits.  */
  idx_t cnt;
  प्रति (cnt = 0; cnt < digest_bin_bytes; ++cnt)
    {
      यदि (c_tolower (hex_digest[2 * cnt])
          != bin2hex[bin_buffer[cnt] >> 4]
          || (c_tolower (hex_digest[2 * cnt + 1])
              != (bin2hex[bin_buffer[cnt] & 0xf])))
        भङ्ग;
    }
  प्रत्यागम cnt == digest_bin_bytes;
}

स्थिर bool
digest_check (वर्ण ध्रुव *checkfile_name)
{
  FILE *checkfile_stream;
  intmax_t n_misformatted_ओळी = 0;
  intmax_t n_mismatched_checksums = 0;
  intmax_t n_open_or_read_failures = 0;
  bool properly_formatted_ओळी = false;
  bool matched_checksums = false;
  unsigned वर्ण bin_buffer_unaligned[DIGEST_BIN_BYTES + DIGEST_ALIGN];
  /* Make sure bin_buffer is properly aligned. */
  unsigned वर्ण *bin_buffer = ptr_align (bin_buffer_unaligned, DIGEST_ALIGN);
  intmax_t ओळ_संख्या;
  वर्ण *ओळ;
  size_t ओळ_chars_allocated;
  bool is_stdin = streq (checkfile_name, "-");

  यदि (is_stdin)
    {
      have_read_stdin = true;
      checkfile_name = _("standard input");
      checkfile_stream = stdin;
    }
  अन्यथा
    {
      checkfile_stream = fopen (checkfile_name, "r");
      यदि (checkfile_stream == रिक्त)
        {
          error (0, errno, "%s", quotef (checkfile_name));
          प्रत्यागम false;
        }
    }

  ओळ_संख्या = 0;
  ओळ = रिक्त;
  ओळ_chars_allocated = 0;
  कुरु
    {
      वर्ण *filename;
      पूर्णाङ्क द्विमान;
      unsigned वर्ण *digest;
      ssize_t ओळ_length;

      ++ओळ_संख्या;
      यदि (ओळ_संख्या == 0)
        error (EXIT_FAILURE, 0, _("%s: too many checksum lines"),
               quotef (checkfile_name));

      ओळ_length = getline (&ओळ, &ओळ_chars_allocated, checkfile_stream);
      यदि (ओळ_length <= 0)
        भङ्ग;

      /* Ignore comment lines, which begin with a '#' character.  */
      यदि (ओळ[0] == '#')
        अनुवर्त;

      /* Remove any trailing newline.  */
      ओळ_length -= ओळ[ओळ_length - 1] == '\n';
      /* Remove any trailing carriage return.  */
      ओळ_length -= ओळ[ओळ_length - (0 < ओळ_length)] == '\r';

      /* Ignore empty lines.  */
      यदि (ओळ_length == 0)
        अनुवर्त;

      ओळ[ओळ_length] = '\0';

      idx_t d_len;
      यदि (! (split_3 (ओळ, ओळ_length, &digest, &d_len, &द्विमान, &filename)
             && ! (is_stdin && streq (filename, "-"))))
        {
          ++n_misformatted_ओळी;

          यदि (warn)
            {
              error (0, 0,
                     _("%s: %jd"
                       ": improperly formatted %s checksum line"),
                     quotef (checkfile_name), ओळ_संख्या,
                     DIGEST_TYPE_STRING);
            }
        }
      अन्यथा
        {
          bool ok;
          bool missing;

          properly_formatted_ओळी = true;

          intmax_t length;
          ok = digest_file (filename, &द्विमान, bin_buffer, &missing, &length);

          यदि (!ok)
            {
              ++n_open_or_read_failures;
              यदि (!status_only)
                printf ("%s: %s\n", quotef (filename),_("FAILED open or read"));
            }
          अन्यथा यदि (ignore_missing && missing)
            {
              /* Ignore missing files with --ignore-missing.  */
              ;
            }
          अन्यथा
            {
              bool match = false;
#यदि HASH_ALGO_CKSUM
              यदि (d_len == BASE64_LENGTH (digest_length >> 3))
                match = b64_equal (digest, bin_buffer);
              अन्यथा
#endif
                यदि (d_len == digest_hex_bytes)
                  match = hex_equal (digest, bin_buffer);

              यदि (match)
                matched_checksums = true;
              अन्यथा
                ++n_mismatched_checksums;

              यदि (!status_only)
                {
                  यदि (! match || ! quiet)
                    fputs (quotef (filename), stdout);

                  यदि (! match)
                    printf (": %s\n", _("FAILED"));
                  अन्यथा यदि (!quiet)
                    printf (": %s\n", _("OK"));
                }
            }

          यदि (ferror (stdout))
            write_error ();
        }
    }
  यावत् (!feof (checkfile_stream) && !ferror (checkfile_stream));

  मुक्त (ओळ);

  पूर्णाङ्क err = ferror (checkfile_stream) ? 0 : -1;
  यदि (is_stdin)
    clearerr (checkfile_stream);
  अन्यथा यदि (fclose (checkfile_stream) != 0 && err < 0)
    err = errno;

  यदि (0 <= err)
    {
      error (0, err, err ? "%s" : _("%s: read error"),
             quotef (checkfile_name));
      प्रत्यागम false;
    }

  यदि (! properly_formatted_ओळी)
    {
      /* Warn if no tests are found.  */
      error (0, 0, _("%s: no properly formatted checksum lines found"),
             quotef (checkfile_name));
    }
  अन्यथा
    {
      यदि (!status_only)
        {
          यदि (n_misformatted_ओळी != 0)
            error (0, 0,
                   (ngettext
                    ("WARNING: %jd line is improperly formatted",
                     "WARNING: %jd lines are improperly formatted",
                     select_plural (n_misformatted_ओळी))),
                   n_misformatted_ओळी);

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

  प्रत्यागम (properly_formatted_ओळी
          && matched_checksums
          && n_mismatched_checksums == 0
          && n_open_or_read_failures == 0
          && (!strict || n_misformatted_ओळी == 0));
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  unsigned वर्ण bin_buffer_unaligned[DIGEST_BIN_BYTES + DIGEST_ALIGN];
  /* Make sure bin_buffer is properly aligned. */
  unsigned वर्ण *bin_buffer = ptr_align (bin_buffer_unaligned, DIGEST_ALIGN);
  bool do_check = false;
  पूर्णाङ्क opt;
  bool ok = true;
  पूर्णाङ्क द्विमान = -1;
  पूर्णाङ्क prefix_tag = -1;
  संरचना option ध्रुव *long_opts = long_options;

#यदि HASH_ALGO_CKSUM
  यदि (cksum_algorithm != crc)
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
  setvbuf (stdout, रिक्त, _IOLBF, 0);

#यदि HASH_ALGO_SUM
  वर्ण ध्रुव *short_opts = "rs";
#नोचेद्यदि HASH_ALGO_CKSUM
  वर्ण ध्रुव *short_opts = legacy_mode ? "bctwz" : "a:l:bctwz";
  वर्ण ध्रुव *digest_length_str = "";
#नोचेद्यदि HASH_ALGO_BLAKE2
  वर्ण ध्रुव *short_opts = "l:bctwz";
  वर्ण ध्रुव *digest_length_str = "";
#अन्यथा
  वर्ण ध्रुव *short_opts = "bctwz";
#endif

  यावत् ((opt = getopt_long (argc, argv, short_opts, long_opts, रिक्त))
         != -1)
    विकल्प (opt)
      {
#यदि HASH_ALGO_CKSUM
      पक्ष 'a':
        cksum_algorithm = XARGMATCH_EXACT ("--algorithm", optarg,
                                           algorithm_args, algorithm_types);
        algorithm_specified = true;
        भङ्ग;

      पक्ष DEBUG_PROGRAM_OPTION:
        cksum_debug = true;
        भङ्ग;
#endif
#यदि HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
      पक्ष 'l':
        digest_length = xnumtoimax (optarg, 10, 0, IDX_MAX, "",
                                    _("invalid length"), 0,
                                    XTOINT_MAX_QUIET);
        digest_length_str = optarg;
        भङ्ग;
#endif
#यदि !HASH_ALGO_SUM
      पक्ष 'c':
        do_check = true;
        भङ्ग;
      पक्ष STATUS_OPTION:
        status_only = true;
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
        status_only = false;
        warn = true;
        quiet = false;
        भङ्ग;
      पक्ष IGNORE_MISSING_OPTION:
        ignore_missing = true;
        भङ्ग;
      पक्ष QUIET_OPTION:
        status_only = false;
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
        prefix_tag = 0;
        भङ्ग;
# endif
      पक्ष TAG_OPTION:
        prefix_tag = 1;
        भङ्ग;
      पक्ष 'z':
        digest_delim = '\0';
        भङ्ग;
#endif
#यदि HASH_ALGO_SUM
      पक्ष 'r':		/* For SysV compatibility. */
        sum_algorithm = bsd;
        भङ्ग;

      पक्ष 's':
        sum_algorithm = sysv;
        भङ्ग;
#endif
      case_GETOPT_HELP_CHAR;
      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
      स्वभाव:
        usage (EXIT_FAILURE);
      }

  min_digest_ओळ_length = MIN_DIGEST_ओळ_LENGTH;
#यदि HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# यदि HASH_ALGO_CKSUM
  यदि (digest_length && (cksum_algorithm != blake2b
                        && cksum_algorithm != sha2
                        && cksum_algorithm != sha3))
    error (EXIT_FAILURE, 0,
           _("--length is only supported with --algorithm "
             "blake2b, sha2, or sha3"));
  यदि (cksum_algorithm == sha2 || cksum_algorithm == sha3)
    {
      /* Do not require --length with --check.  */
      यदि (digest_length == 0 && *digest_length_str == '\0' && ! do_check)
        error (EXIT_FAILURE, 0, _("--algorithm=%s requires specifying "
                                  "--length 224, 256, 384, or 512"),
               algorithm_args[cksum_algorithm]);
      /* If --check and --length are used we verify the digest length.  */
      यदि ((! do_check || *digest_length_str != '\0')
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
  अन्यथा
    {
      /* If the digest length checks for SHA-3 are satisfied, the less strict
         checks for BLAKE2 will also be.  */
# अन्यथा
    {
# endif
      यदि (digest_length > DIGEST_MAX_LEN * 8)
        {
          error (0, 0, _("invalid length: %s"), quote (digest_length_str));
          error (EXIT_FAILURE, 0,
                 _("maximum digest length for %s is %d bits"),
                 quote (DIGEST_TYPE_STRING),
                 DIGEST_MAX_LEN * 8);
        }
      यदि (digest_length % 8 != 0)
        {
          error (0, 0, _("invalid length: %s"), quote (digest_length_str));
          error (EXIT_FAILURE, 0, _("length is not a multiple of 8"));
        }
    }
  यदि (digest_length == 0)
    {
# यदि HASH_ALGO_BLAKE2
      digest_length = DIGEST_MAX_LEN * 8;
# अन्यथा
      digest_length = algorithm_bits[cksum_algorithm];
# endif
    }
  digest_hex_bytes = digest_length >> 2;
#अन्यथा
  digest_hex_bytes = DIGEST_HEX_BYTES;
#endif

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
#endif

  यदि (digest_delim != '\n' && do_check)
    {
      error (0, 0, _("the --zero option is not supported when "
                     "verifying checksums"));
      usage (EXIT_FAILURE);
    }
  यदि (1 <= prefix_tag && do_check)
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

  यदि (status_only && !do_check)
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

  यदि (prefix_tag == -1)
    prefix_tag = HASH_ALGO_CKSUM;

  यदि (prefix_tag && !द्विमान)
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
#endif
     usage (EXIT_FAILURE);
   }

  यदि (!O_द्विमान && द्विमान < 0)
    द्विमान = 0;
  अन्यथा यदि (prefix_tag)
    द्विमान = 1;

  वर्ण **operand_lim = argv + argc;
  यदि (optind == argc)
    *operand_lim++ = bad_cast ("-");
  अन्यथा यदि (1 < argc - optind && raw_digest)
    error (EXIT_FAILURE, 0,
           _("the --raw option is not supported with multiple files"));

  प्रति (वर्ण **operandp = argv + optind; operandp < operand_lim; operandp++)
    {
      वर्ण *file = *operandp;
      यदि (do_check)
        ok &= digest_check (file);
      अन्यथा
        {
          पूर्णाङ्क द्विमान_file = द्विमान;
          bool missing;
          intmax_t length;

          यदि (! digest_file (file, &द्विमान_file, bin_buffer, &missing, &length))
            ok = false;
          अन्यथा
            {
              DIGEST_OUT (file, द्विमान_file, bin_buffer, raw_digest, prefix_tag,
                          digest_delim, optind != argc, length);
              यदि (ferror (stdout))
                write_error ();
            }
        }
    }

  यदि (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, _("standard input"));

  प्रत्यागम ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
