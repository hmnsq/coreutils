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

#जर HASH_ALGO_SUM || HASH_ALGO_CKSUM
# include "sum.h"
#endif
#जर HASH_ALGO_CKSUM
# include "cksum.h"
# include "cksum_crc.h"
# include "base64.h"
#endif
#जर HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# include "blake2/b2sum.h"
#endif
#जर HASH_ALGO_MD5 || HASH_ALGO_CKSUM
# include "md5.h"
#endif
#जर HASH_ALGO_SHA1 || HASH_ALGO_CKSUM
# include "sha1.h"
#endif
#जर HASH_ALGO_SHA256 || HASH_ALGO_SHA224 || HASH_ALGO_CKSUM
# include "sha256.h"
#endif
#जर HASH_ALGO_SHA512 || HASH_ALGO_SHA384 || HASH_ALGO_CKSUM
# include "sha512.h"
#endif
#जर HASH_ALGO_CKSUM
# include "sha3.h"
#endif
#जर HASH_ALGO_CKSUM
# include "sm3.h"
#endif
#अन्तर्भाव "fadvise.h"
#अन्तर्भाव "stdio--.h"
#अन्तर्भाव "xbinary-io.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#जर HASH_ALGO_SUM
# define PROGRAM_NAME "sum"
# define DIGEST_TYPE_STRING "BSD"
# define DIGEST_STREAM sumfns[sum_algorithm]
# define DIGEST_OUT sum_output_fns[sum_algorithm]
# define DIGEST_BITS 16
# define DIGEST_ALIGN 4
#किंवाजर HASH_ALGO_CKSUM
# define MAX_DIGEST_BITS 512
# define MAX_DIGEST_ALIGN 8
# define DIGEST_TYPE_STRING algorithm_tags[cksum_algorithm]
# define DIGEST_STREAM cksumfns[cksum_algorithm]
# define DIGEST_OUT cksum_output_fns[cksum_algorithm]
# define DIGEST_BITS MAX_DIGEST_BITS
# define DIGEST_ALIGN MAX_DIGEST_ALIGN
#किंवाजर HASH_ALGO_MD5
# define PROGRAM_NAME "md5sum"
# define DIGEST_TYPE_STRING "MD5"
# define DIGEST_STREAM md5_stream
# define DIGEST_BITS 128
# define DIGEST_REFERENCE "RFC 1321"
# define DIGEST_ALIGN 4
#किंवाजर HASH_ALGO_BLAKE2
# define PROGRAM_NAME "b2sum"
# define DIGEST_TYPE_STRING "BLAKE2b"
# define DIGEST_STREAM blake2b_stream
# define DIGEST_BITS 512
# define DIGEST_REFERENCE "RFC 7693"
# define DIGEST_ALIGN 8
#किंवाजर HASH_ALGO_SHA1
# define PROGRAM_NAME "sha1sum"
# define DIGEST_TYPE_STRING "SHA1"
# define DIGEST_STREAM sha1_stream
# define DIGEST_BITS 160
# define DIGEST_REFERENCE "FIPS-180-1"
# define DIGEST_ALIGN 4
#किंवाजर HASH_ALGO_SHA256
# define PROGRAM_NAME "sha256sum"
# define DIGEST_TYPE_STRING "SHA256"
# define DIGEST_STREAM sha256_stream
# define DIGEST_BITS 256
# define DIGEST_REFERENCE "FIPS-180-2"
# define DIGEST_ALIGN 4
#किंवाजर HASH_ALGO_SHA224
# define PROGRAM_NAME "sha224sum"
# define DIGEST_TYPE_STRING "SHA224"
# define DIGEST_STREAM sha224_stream
# define DIGEST_BITS 224
# define DIGEST_REFERENCE "RFC 3874"
# define DIGEST_ALIGN 4
#किंवाजर HASH_ALGO_SHA512
# define PROGRAM_NAME "sha512sum"
# define DIGEST_TYPE_STRING "SHA512"
# define DIGEST_STREAM sha512_stream
# define DIGEST_BITS 512
# define DIGEST_REFERENCE "FIPS-180-2"
# define DIGEST_ALIGN 8
#किंवाजर HASH_ALGO_SHA384
# define PROGRAM_NAME "sha384sum"
# define DIGEST_TYPE_STRING "SHA384"
# define DIGEST_STREAM sha384_stream
# define DIGEST_BITS 384
# define DIGEST_REFERENCE "FIPS-180-2"
# define DIGEST_ALIGN 8
#अन्यथा
# error "Can't decide which hash algorithm to compile."
#endif
#जर !HASH_ALGO_SUM && !HASH_ALGO_CKSUM
# define DIGEST_OUT output_file
#endif

#जर HASH_ALGO_CKSUM
# define PROGRAM_NAME (!legacy_mode ? "cksum"			 \
                       : cksum_algorithm == md5 ? "md5sum"	 \
                       : cksum_algorithm == sha1 ? "sha1sum"	 \
                       : cksum_algorithm == sha224 ? "sha224sum" \
                       : cksum_algorithm == sha256 ? "sha256sum" \
                       : cksum_algorithm == sha384 ? "sha384sum" \
                       : cksum_algorithm == sha512 ? "sha512sum" \
                       : "cksum")
#endif

#जर HASH_ALGO_SUM
# define AUTHORS \
  proper_name ("Kayvan Aghaiepour"), \
  proper_name ("David MacKenzie")
#किंवाजर HASH_ALGO_CKSUM
# define AUTHORS \
  proper_name_lite ("Padraig Brady", "P\303\241draig Brady"), \
  proper_name ("Q. Frank Xia")
#किंवाजर HASH_ALGO_BLAKE2
# define AUTHORS \
  proper_name_lite ("Padraig Brady", "P\303\241draig Brady"), \
  proper_name ("Samuel Neves")
#अन्यथा
# define AUTHORS \
  proper_name ("Ulrich Drepper"), \
  proper_name ("Scott Miller"), \
  proper_name ("David Madore")
#endif
#जर !HASH_ALGO_BLAKE2 && !HASH_ALGO_CKSUM
# define DIGEST_HEX_BYTES (DIGEST_BITS / 4)
#endif
#define DIGEST_BIN_BYTES (DIGEST_BITS / 8)

/* The minimum length of a valid digest line.  This length does
   not include any newline character at the end of a line.  */
#जर HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# define MIN_DIGEST_ओळ_LENGTH 3 /* With -l 8.  */
#अन्यथा
# define MIN_DIGEST_ओळ_LENGTH \
   (DIGEST_HEX_BYTES /* length of hexadecimal message digest */ \
    + 1 /* blank */ \
    + 1 /* minimum filename length */ )
#endif

#जर !HASH_ALGO_SUM
स्थिर शून्य
output_file (अक्षर स्थिरांक *file, पूर्णांक द्विमान_file, शून्य स्थिरांक *digest,
             bool raw, bool tagged, अचिन्हित अक्षर delim, bool args,
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
स्थिर पूर्णांक bsd_reversed = -1;

/* line delimiter.  */
स्थिर अचिन्हित अक्षर digest_delim = '\n';

#जर HASH_ALGO_CKSUM
/* If true, print base64-encoded digests, not hex.  */
स्थिर bool base64_digest = false;
#endif

/* If true, print binary digests, not hex.  */
स्थिर bool raw_digest = false;

/* blake2b and sha3 allow the -l option.  Luckily they both have the same
   maximum digest size.  */
#जर HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# जर HASH_ALGO_CKSUM
static_assert (BLAKE2B_OUTBYTES == SHA3_512_DIGEST_SIZE);
# endif
# define DIGEST_MAX_LEN BLAKE2B_OUTBYTES
स्थिर idx_t digest_length;
#endif /* HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM */

प्रकारव्याख्या शून्य (*digest_output_fn)(अक्षर स्थिरांक *, पूर्णांक, शून्य स्थिरांक *, bool,
                                 bool, अचिन्हित अक्षर, bool, intmax_t);
#जर HASH_ALGO_SUM
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

#जर HASH_ALGO_CKSUM
स्थिर पूर्णांक
md5_sum_stream (खाता *stream, शून्य *resstream, MAYBE_UNUSED intmax_t *length)
{
  परत md5_stream (stream, resstream);
}
स्थिर पूर्णांक
sha1_sum_stream (खाता *stream, शून्य *resstream, MAYBE_UNUSED intmax_t *length)
{
  परत sha1_stream (stream, resstream);
}
स्थिर पूर्णांक
sha224_sum_stream (खाता *stream, शून्य *resstream,
                   MAYBE_UNUSED intmax_t *length)
{
  परत sha224_stream (stream, resstream);
}
स्थिर पूर्णांक
sha256_sum_stream (खाता *stream, शून्य *resstream,
                   MAYBE_UNUSED intmax_t *length)
{
  परत sha256_stream (stream, resstream);
}
स्थिर पूर्णांक
sha384_sum_stream (खाता *stream, शून्य *resstream,
                   MAYBE_UNUSED intmax_t *length)
{
  परत sha384_stream (stream, resstream);
}
स्थिर पूर्णांक
sha512_sum_stream (खाता *stream, शून्य *resstream,
                   MAYBE_UNUSED intmax_t *length)
{
  परत sha512_stream (stream, resstream);
}
स्थिर पूर्णांक
sha2_sum_stream (खाता *stream, शून्य *resstream, intmax_t *length)
{
  निवड (*length)
    {
    स्थिती SHA224_DIGEST_SIZE:
      परत sha224_stream (stream, resstream);
    स्थिती SHA256_DIGEST_SIZE:
      परत sha256_stream (stream, resstream);
    स्थिती SHA384_DIGEST_SIZE:
      परत sha384_stream (stream, resstream);
    स्थिती SHA512_DIGEST_SIZE:
      परत sha512_stream (stream, resstream);
    पूर्वनिर्धारित:
      affirm (false);
    }
}
स्थिर पूर्णांक
sha3_sum_stream (खाता *stream, शून्य *resstream, intmax_t *length)
{
  निवड (*length)
    {
    स्थिती SHA3_224_DIGEST_SIZE:
      परत sha3_224_stream (stream, resstream);
    स्थिती SHA3_256_DIGEST_SIZE:
      परत sha3_256_stream (stream, resstream);
    स्थिती SHA3_384_DIGEST_SIZE:
      परत sha3_384_stream (stream, resstream);
    स्थिती SHA3_512_DIGEST_SIZE:
      परत sha3_512_stream (stream, resstream);
    पूर्वनिर्धारित:
      affirm (false);
    }
}
स्थिर पूर्णांक
blake2b_sum_stream (खाता *stream, शून्य *resstream, intmax_t *length)
{
  परत blake2b_stream (stream, resstream, *length);
}
स्थिर पूर्णांक
sm3_sum_stream (खाता *stream, शून्य *resstream, MAYBE_UNUSED intmax_t *length)
{
  परत sm3_stream (stream, resstream);
}

स्थिर अक्षर स्थिरांक *स्थिरांक algorithm_args[] =
{
  "bsd", "sysv", "crc", "crc32b", "md5", "sha1",
  "sha224", "sha256", "sha384", "sha512", /* Legacy naming */
  "sha2", "sha3", "blake2b", "sm3", रिक्त
};
स्थिर गणना Algorithm स्थिरांक algorithm_types[] =
{
  bsd, sysv, crc, crc32b, md5, sha1,
  sha224, sha256, sha384, sha512,
  sha2, sha3, blake2b, sm3,
};
ARGMATCH_VERIFY (algorithm_args, algorithm_types);

स्थिर अक्षर स्थिरांक *स्थिरांक algorithm_tags[] =
{
  "BSD", "SYSV", "CRC", "CRC32B", "MD5", "SHA1",
  "SHA224", "SHA256", "SHA384", "SHA512",
  "SHA2", "SHA3", "BLAKE2b", "SM3", रिक्त
};
स्थिर पूर्णांक स्थिरांक algorithm_bits[] =
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
  IGNORE_MISSING_OPTION = अक्षर_उच्च + 1,
  STATUS_OPTION,
  QUIET_OPTION,
  STRICT_OPTION,
  TAG_OPTION,
  UNTAG_OPTION,
  DEBUG_PROGRAM_OPTION,
  RAW_OPTION,
  BASE64_OPTION,
};

#जर HASH_ALGO_CKSUM
स्थिर संरचना option स्थिरांक legacy_long_options[] =
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

स्थिर संरचना option स्थिरांक long_options[] =
{
#जर HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
  { "length", required_argument, रिक्त, 'l'},
#endif

#जर !HASH_ALGO_SUM
  { "check", no_argument, रिक्त, 'c' },
  { "ignore-missing", no_argument, रिक्त, IGNORE_MISSING_OPTION},
  { "quiet", no_argument, रिक्त, QUIET_OPTION },
  { "status", no_argument, रिक्त, STATUS_OPTION },
  { "warn", no_argument, रिक्त, 'w' },
  { "strict", no_argument, रिक्त, STRICT_OPTION },
  { "tag", no_argument, रिक्त, TAG_OPTION },
  { "zero", no_argument, रिक्त, 'z' },

# जर HASH_ALGO_CKSUM
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
usage (पूर्णांक status)
{
  जर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s [OPTION]... [खाता]...\n\
"), program_name);
#जर HASH_ALGO_CKSUM
    जर (legacy_mode)
      {
        म_लिखो (_("\
Print or check %s checksums.\n\
"), DIGEST_TYPE_STRING);
        ख_माला_दो (_("\
Legacy interface to the cksum(1) utility.\n\
"), मानक_निकास);
      }
    अन्यथा
      ख_माला_दो (_("\
Print or verify checksums.\n\
By पूर्वनिर्धारित use the 32 bit CRC algorithm.\n\
"), मानक_निकास);
#अन्यथा
      म_लिखो (_("\
Print or check %s (%d-bit) checksums.\n\
"),
              DIGEST_TYPE_STRING,
              DIGEST_BITS);
      ख_माला_दो (_("\
Legacy interface to the cksum(1) utility.\n\
"), मानक_निकास);
#endif

      emit_stdin_note ();
#जर HASH_ALGO_SUM
      oputs (_("\
  -r\n\
         use BSD sum algorithm (the पूर्वनिर्धारित), use 1K blocks\n\
"));
      oputs (_("\
  -s, --sysv\n\
         use System V sum algorithm, use 512 bytes blocks\n\
"));
#endif
#जर HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
        emit_mandatory_arg_note ();
#endif
#जर HASH_ALGO_CKSUM
     जर (! legacy_mode)
        oputs (_("\
  -a, --algorithm=TYPE\n\
         select the digest type to use.  See DIGEST below\n\
"));
     जर (! legacy_mode)
        oputs (_("\
      --base64\n\
         emit base64-encoded digests, not hexadecimal\n\
"));
#endif
#जर !HASH_ALGO_SUM
# जर HASH_ALGO_CKSUM
     जर (legacy_mode)
       {
# endif
      जर (O_द्विमान)
        oputs (_("\
  -b, --द्विमान\n\
         पठ in द्विमान mode (पूर्वनिर्धारित unless reading tty मानक_निवेश)\n\
"));
      अन्यथा
        oputs (_("\
  -b, --द्विमान\n\
         पठ in द्विमान mode\n\
"));
# जर HASH_ALGO_CKSUM
       }
# endif
        oputs (_("\
  -c, --check\n\
         पठ checksums from the FILEs and check them\n\
"));
# जर HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
#  जर HASH_ALGO_CKSUM
     जर (! legacy_mode)
#  endif
        oputs (_("\
  -l, --length=BITS\n\
         digest length in bits; must not exceed the max size\n\
         and must be a multiple of 8 साठी blake2b;\n\
         must be 224, 256, 384, or 512 साठी sha2 or sha3\n\
"));
# endif
# जर HASH_ALGO_CKSUM
    जर (! legacy_mode)
        oputs (_("\
      --raw\n\
         emit a raw द्विमान digest, not hexadecimal\n\
"));
    जर (legacy_mode)
      oputs (_("\
      --tag\n\
         create a BSD-style checksum\n\
"));
    अन्यथा
      oputs (_("\
      --tag\n\
         create a BSD-style checksum (the पूर्वनिर्धारित)\n\
"));
    जर (! legacy_mode)
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
# जर HASH_ALGO_CKSUM
    जर (legacy_mode)
      {
# endif
      जर (O_द्विमान)
        oputs (_("\
  -t, --text\n\
         पठ in text mode (पूर्वनिर्धारित जर reading tty मानक_निवेश)\n\
"));
      अन्यथा
        oputs (_("\
  -t, --text\n\
         पठ in text mode (पूर्वनिर्धारित)\n\
"));
# जर HASH_ALGO_CKSUM
      }
# endif
      oputs (_("\
  -z, --zero\n\
         end each output ओळ with NUL, not newline,\n\
         and disable file name escaping\n\
"));
      ख_माला_दो (_("\
\n\
The following five options are useful only when verifying checksums:\n\
"), मानक_निकास);
      oputs (_("\
      --ignore-missing\n\
         don't fail or report status साठी missing files\n\
"));
      oputs (_("\
      --quiet\n\
         don't print OK साठी each successfully verified file\n\
"));
      oputs (_("\
      --status\n\
         don't output anything, status code shows success\n\
"));
      oputs (_("\
      --strict\n\
         निकास non-zero साठी improperly formatted checksum ओळी\n\
"));
      oputs (_("\
  -w, --warn\n\
         warn about improperly formatted checksum ओळी\n\
"));
#endif
#जर HASH_ALGO_CKSUM
    जर (! legacy_mode)
      oputs (_("\
      --debug\n\
         indicate which implementation used\n\
"));
#endif
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
#जर HASH_ALGO_CKSUM
    जर (! legacy_mode)
      ख_माला_दो (_("\
\n\
DIGEST determines the digest algorithm and पूर्वनिर्धारित output format:\n\
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
\n"), मानक_निकास);
#endif
#जर !HASH_ALGO_SUM && !HASH_ALGO_CKSUM
      म_लिखो (_("\
\n\
The sums are computed as described in %s.\n"), DIGEST_REFERENCE);
      ख_माला_दो (_("\
When checking, the आदान should be a former output of this program.\n\
The पूर्वनिर्धारित mode is to print a ओळ with: checksum, a space,\n\
a character indicating आदान mode ('*' साठी द्विमान, ' ' साठी text\n\
or where द्विमान is insignificant), and name साठी each खाता.\n\
\n\
There is no difference between द्विमान mode and text mode on GNU systems.\
\n"), मानक_निकास);
#endif
#जर HASH_ALGO_CKSUM
      ख_माला_दो (_("\
When checking, the आदान should be a former output of this program,\n\
or equivalent standalone program.\
\n"), मानक_निकास);
#endif
      emit_ancillary_info (PROGRAM_NAME);
    }

  निकास (status);
}

#जर !HASH_ALGO_SUM
/* Given a string S, return TRUE if it contains problematic characters
   that need escaping.  Note we escape '\' itself to provide some forward
   compat to introduce escaping of other characters.  */

ATTRIBUTE_PURE
स्थिर bool
problematic_chars (अक्षर स्थिरांक *s)
{
  idx_t length = म_खोज (s, "\\\n\r");
  परत s[length] != '\0';
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

स्थिर अक्षर *
filename_unescape (अक्षर *s, idx_t s_len)
{
  अक्षर *dst = s;

  साठी (idx_t i = 0; i < s_len; i++)
    {
      निवड (s[i])
        {
        स्थिती '\\':
          जर (i == s_len - 1)
            {
              /* File name ends with an unescaped backslash: invalid.  */
              परत रिक्त;
            }
          ++i;
          निवड (s[i])
            {
            स्थिती 'n':
              *dst++ = '\n';
              थांबा;
            स्थिती 'r':
              *dst++ = '\r';
              थांबा;
            स्थिती '\\':
              *dst++ = '\\';
              थांबा;
            पूर्वनिर्धारित:
              /* Only '\', 'n' or 'r' may follow a backslash.  */
              परत रिक्त;
            }
          थांबा;

        स्थिती '\0':
          /* The file name may not contain a NUL.  */
          परत रिक्त;

        पूर्वनिर्धारित:
          *dst++ = s[i];
          थांबा;
        }
    }
  जर (dst < s + s_len)
    *dst = '\0';

  परत s;
}

/* Return true if S is a LEN-byte NUL-terminated string of hex or base64
   digits and has the expected length.  Otherwise, return false.  */
ATTRIBUTE_PURE
स्थिर bool
valid_digits (अचिन्हित अक्षर स्थिरांक *s, idx_t len)
{
#जर HASH_ALGO_CKSUM
  जर (len == BASE64_LENGTH (digest_length >> 3))
    {
      idx_t i;
      साठी (i = 0; i < len - digest_length % 3; i++)
        {
          जर (!isbase64 (*s))
            परत false;
          ++s;
        }
      साठी ( ; i < len; i++)
        {
          जर (*s != '=')
            परत false;
          ++s;
        }
    }
  अन्यथा
#endif
  जर (len == digest_hex_bytes)
    {
      साठी (idx_t i = 0; i < digest_hex_bytes; i++)
        {
          जर (!c_isxdigit (*s))
            परत false;
          ++s;
        }
    }
  अन्यथा
    परत false;

  परत *s == '\0';
}

/* Split the checksum string S (of length S_LEN) from a BSD 'md5' or
   'sha1' command into two parts: a hexadecimal digest, and the file
   name.  S is modified.  Set *D_LEN to the length of the digest string.
   Return true if successful.  */

स्थिर bool
bsd_split_3 (अक्षर *s, idx_t s_len,
             अचिन्हित अक्षर **digest, idx_t *d_len,
             अक्षर **file_name, bool escaped_filename)
{
  जर (s_len == 0)
    परत false;

  /* Find end of filename.  */
  idx_t i = s_len - 1;
  पर्यंत (i && s[i] != ')')
    i--;

  जर (s[i] != ')')
    परत false;

  *file_name = s;

  जर (escaped_filename && filename_unescape (s, i) == रिक्त)
    परत false;

  s[i++] = '\0';

  पर्यंत (c_isblank (s[i]))
    i++;

  जर (s[i] != '=')
    परत false;

  i++;

  पर्यंत (c_isblank (s[i]))
    i++;

  *digest = (अचिन्हित अक्षर *) &s[i];

  *d_len = s_len - i;
  परत valid_digits (*digest, *d_len);
}

#जर HASH_ALGO_CKSUM
/* Return the corresponding Algorithm for the string S,
   or -1 for no match.  */

स्थिर सूचक_भेद_प्रकार
algorithm_from_tag (अक्षर *s)
{
  /* Limit check size to this length for perf reasons.  */
  स्थिर idx_t max_tag_len;
  जर (! max_tag_len)
    {
      अक्षर स्थिरांक * स्थिरांक * tag = algorithm_tags;
      पर्यंत (*tag)
        {
          idx_t tag_len = म_माप (*tag++);
          max_tag_len = MAX (tag_len, max_tag_len);
        }
    }

  idx_t i = 0;

  /* Find end of tag */
  पर्यंत (i <= max_tag_len && s[i] && ! c_isblank (s[i])
         && s[i] != '-' && s[i] != '(')
    ++i;

  जर (i > max_tag_len)
    परत -1;

  /* Terminate tag, and lookup.  */
  अक्षर sep = s[i];
  s[i] = '\0';
  सूचक_भेद_प्रकार algo = argmatch_exact (s, algorithm_tags);
  s[i] = sep;

  परत algo;
}
#endif

/* Split the string S (of length S_LEN) into three parts:
   a hexadecimal digest, binary flag, and the file name.
   S is modified.  Set *D_LEN to the length of the digest string.
   Return true if successful.  */

स्थिर bool
split_3 (अक्षर *s, idx_t s_len,
         अचिन्हित अक्षर **digest, idx_t *d_len, पूर्णांक *द्विमान, अक्षर **file_name)
{
  bool escaped_filename = false;
  idx_t algo_name_len;

  idx_t i = 0;
  पर्यंत (c_isblank (s[i]))
    ++i;

  जर (s[i] == '\\')
    {
      ++i;
      escaped_filename = true;
    }

  /* Check for BSD-style checksum line. */

#जर HASH_ALGO_CKSUM
  जर (! algorithm_specified || cksum_algorithm == sha2)
    {
      सूचक_भेद_प्रकार algo_tag = algorithm_from_tag (s + i);
      जर (! algorithm_specified)
        {
          जर (algo_tag >= 0)
            {
              जर (algo_tag <= crc32b)
                परत false;  /* We don't support checking these formats.  */
              cksum_algorithm = algo_tag;
            }
          अन्यथा
            परत false;      /* We only support tagged format without -a.  */
        }
      अन्यथा
        {
          जर (cksum_algorithm == sha2 && (algo_tag == sha2
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
  algo_name_len = म_माप (DIGEST_TYPE_STRING);
  जर (STREQ_LEN (s + i, DIGEST_TYPE_STRING, algo_name_len))
    {
      i += algo_name_len;
#जर HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM

# जर HASH_ALGO_BLAKE2
      digest_length = DIGEST_MAX_LEN * 8;
# अन्यथा
      digest_length = algorithm_bits[cksum_algorithm];
# endif
      जर (s[i] == '-')  /* length specified. Not base64 */
        {
          ++i;
          intmax_t length;
          अक्षर *siend;
          जर (!c_isdigit (s[i])
              || xstrtoimax (s + i, &siend, 0, &length, रिक्त) != LONGINT_OK)
            परत false;
# जर HASH_ALGO_CKSUM
          अन्यथा जर (cksum_algorithm == sha2 || cksum_algorithm == sha3)
            {
              जर (length != SHA224_DIGEST_SIZE * 8
                  && length != SHA256_DIGEST_SIZE * 8
                  && length != SHA384_DIGEST_SIZE * 8
                  && length != SHA512_DIGEST_SIZE * 8)
                परत false;
            }
# endif
          अन्यथा जर (!(0 < length && length <= digest_length && length % 8 == 0))
            परत false;

          i = siend - s;
          digest_length = length;
        }
      digest_hex_bytes = digest_length >> 2;
#endif
      जर (s[i] == ' ')
        ++i;
      जर (s[i] == '(')  /* not base64 */
        {
          ++i;
          *द्विमान = 0;
          परत bsd_split_3 (s + i, s_len - i,
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
  जर (s_len - i < min_digest_ओळ_length + (s[i] == '\\'))
    परत false;

  *digest = (अचिन्हित अक्षर *) &s[i];

#जर HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
  /* Auto determine length.  */
# जर HASH_ALGO_CKSUM
  जर (cksum_algorithm == blake2b
      || cksum_algorithm == sha2 || cksum_algorithm == sha3) {
# endif
  अचिन्हित अक्षर स्थिरांक *hp = *digest;
  digest_hex_bytes = 0;
  साठी (; c_isxdigit (*hp); ++hp, ++digest_hex_bytes)
    ;
# जर HASH_ALGO_CKSUM
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
  साठी (; isubase64 (*hp); ++hp, ++digest_base64_bytes)
    ;
  साठी (; *hp == '='; ++hp, ++trailing_equals)
    ;
  जर ((cksum_algorithm == sha2 || cksum_algorithm == sha3)
      && digest_hex_bytes >> 1 != SHA224_DIGEST_SIZE
      && digest_hex_bytes >> 1 != SHA256_DIGEST_SIZE
      && digest_hex_bytes >> 1 != SHA384_DIGEST_SIZE
      && digest_hex_bytes >> 1 != SHA512_DIGEST_SIZE)
    {
      जर (digest_base64_bytes + trailing_equals
          == BASE64_LENGTH (SHA224_DIGEST_SIZE))
        digest_hex_bytes = SHA224_DIGEST_SIZE * 2;
      अन्यथा जर (digest_base64_bytes + trailing_equals
               == BASE64_LENGTH (SHA256_DIGEST_SIZE))
        digest_hex_bytes = SHA256_DIGEST_SIZE * 2;
      अन्यथा जर (digest_base64_bytes + trailing_equals
               == BASE64_LENGTH (SHA384_DIGEST_SIZE))
        digest_hex_bytes = SHA384_DIGEST_SIZE * 2;
      अन्यथा जर (digest_base64_bytes + trailing_equals
               == BASE64_LENGTH (SHA512_DIGEST_SIZE))
        digest_hex_bytes = SHA512_DIGEST_SIZE * 2;
      अन्यथा
        परत false;
    }
  अन्यथा जर (cksum_algorithm == blake2b
           && digest_hex_bytes < digest_base64_bytes)
    {
      साठी (पूर्णांक j = 8; j <= DIGEST_MAX_LEN * 8; j += 8)
        {
          जर (BASE64_LENGTH (j / 8) == digest_base64_bytes + trailing_equals
              && j % 3 == trailing_equals)
            {
              digest_hex_bytes = j / 4;
              थांबा;
            }
        }
    }
# endif
  जर (digest_hex_bytes < 2 || digest_hex_bytes % 2
      || DIGEST_MAX_LEN * 2 < digest_hex_bytes)
    परत false;
  digest_length = digest_hex_bytes * 4;
# जर HASH_ALGO_CKSUM
  }
# endif
#endif

  /* This field must be the hexadecimal or base64 representation
     of the message digest.  */
  पर्यंत (s[i] && !c_isblank (s[i]))
    i++;

  /* The digest must be followed by at least one whitespace character.  */
  जर (i == s_len)
    परत false;

  *d_len = &s[i] - (अक्षर *) *digest;
  s[i++] = '\0';

  जर (! valid_digits (*digest, *d_len))
    परत false;

  /* If "bsd reversed" format detected.  */
  जर ((s_len - i == 1) || (s[i] != ' ' && s[i] != '*'))
    {
      /* Don't allow mixing bsd and standard formats,
         to minimize security issues with attackers
         renaming files with leading spaces.
         This assumes that with bsd format checksums
         that the first file name does not have
         a leading ' ' or '*'.  */
      जर (bsd_reversed == 0)
        परत false;
      bsd_reversed = 1;
    }
  अन्यथा जर (bsd_reversed != 1)
    {
      bsd_reversed = 0;
      *द्विमान = (s[i++] == '*');
    }

  /* All characters between the type indicator and end of line are
     significant -- that includes leading and trailing white space.  */
  *file_name = &s[i];

  जर (escaped_filename)
    परत filename_unescape (&s[i], s_len - i) != रिक्त;

  परत true;
}

#जर !HASH_ALGO_SUM
/* If ESCAPE is true, then translate each:
   NEWLINE byte to the string, "\\n",
   CARRIAGE RETURN byte to the string, "\\r",
   and each backslash to "\\\\".  */
स्थिर शून्य
print_filename (अक्षर स्थिरांक *file, bool escape)
{
  जर (! escape)
    {
      ख_माला_दो (file, मानक_निकास);
      परत;
    }

  पर्यंत (*file)
    {
      निवड (*file)
        {
        स्थिती '\n':
          ख_माला_दो ("\\n", मानक_निकास);
          थांबा;

        स्थिती '\r':
          ख_माला_दो ("\\r", मानक_निकास);
          थांबा;

        स्थिती '\\':
          ख_माला_दो ("\\\\", मानक_निकास);
          थांबा;

        पूर्वनिर्धारित:
          अक्षर_दो (*file);
          थांबा;
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
digest_file (अक्षर स्थिरांक *filename, पूर्णांक *द्विमान, अचिन्हित अक्षर *bin_result,
             bool *missing, MAYBE_UNUSED intmax_t *length)
{
  खाता *fp;
  पूर्णांक err;
  bool is_stdin = streq (filename, "-");

  *missing = false;

  जर (is_stdin)
    {
      have_read_stdin = true;
      fp = मानक_निवेश;
      जर (O_द्विमान && *द्विमान)
        {
          जर (*द्विमान < 0)
            *द्विमान = ! isatty (STDIN_FILENO);
          जर (*द्विमान)
            xset_द्विमान_mode (STDIN_FILENO, O_द्विमान);
        }
    }
  अन्यथा
    {
      fp = ख_खोलो (filename, O_द्विमान ? (*द्विमान ? "rb" : "rt") : "r");
      जर (fp == रिक्त)
        {
          जर (ignore_missing && त्रुटि_सं == ENOENT)
            {
              *missing = true;
              परत true;
            }
          error (0, त्रुटि_सं, "%s", quotef (filename));
          परत false;
        }
    }

  fadvise (fp, FADVISE_SEQUENTIAL);

#जर HASH_ALGO_CKSUM
  जर (cksum_algorithm == blake2b
      || cksum_algorithm == sha2 || cksum_algorithm == sha3)
    *length = digest_length >> 3;
  err = DIGEST_STREAM (fp, bin_result, length);
#किंवाजर HASH_ALGO_SUM
  err = DIGEST_STREAM (fp, bin_result, length);
#किंवाजर HASH_ALGO_BLAKE2
  err = DIGEST_STREAM (fp, bin_result, digest_length >> 3);
#अन्यथा
  err = DIGEST_STREAM (fp, bin_result);
#endif
  err = err ? त्रुटि_सं : 0;
  जर (is_stdin)
    साफ_त्रुटि (fp);
  अन्यथा जर (ख_बंद (fp) != 0 && !err)
    err = त्रुटि_सं;

  जर (err)
    {
      error (0, err, "%s", quotef (filename));
      परत false;
    }

  परत true;
}

#जर !HASH_ALGO_SUM
स्थिर शून्य
output_file (अक्षर स्थिरांक *file, पूर्णांक द्विमान_file, शून्य स्थिरांक *digest,
             MAYBE_UNUSED bool raw, bool tagged, अचिन्हित अक्षर delim,
             MAYBE_UNUSED bool args, MAYBE_UNUSED intmax_t length)
{
# जर HASH_ALGO_CKSUM
  जर (raw)
    {
      ख_डालो (digest, 1, digest_length >> 3, मानक_निकास);
      परत;
    }
# endif

  अचिन्हित अक्षर स्थिरांक *bin_buffer = digest;

  /* Output a leading backslash if the file name contains problematic chars.  */
  bool needs_escape = delim == '\n' && problematic_chars (file);

  जर (needs_escape)
    अक्षर_दो ('\\');

  जर (tagged)
    {
# जर HASH_ALGO_CKSUM
      जर (cksum_algorithm == sha2)
        म_लिखो ("SHA%u", (अचिन्हित पूर्णांक) digest_length);
      अन्यथा
# endif
      ख_माला_दो (DIGEST_TYPE_STRING, मानक_निकास);
# जर HASH_ALGO_BLAKE2
      जर (digest_length < DIGEST_MAX_LEN * 8)
        म_लिखो ("-%u", (अचिन्हित पूर्णांक) digest_length);
# elif HASH_ALGO_CKSUM
      जर (cksum_algorithm == sha3)
        म_लिखो ("-%u", (अचिन्हित पूर्णांक) digest_length);
      जर (cksum_algorithm == blake2b)
        {
          जर (digest_length < DIGEST_MAX_LEN * 8)
            म_लिखो ("-%u", (अचिन्हित पूर्णांक) digest_length);
        }
# endif
      ख_माला_दो (" (", मानक_निकास);
      print_filename (file, needs_escape);
      ख_माला_दो (") = ", मानक_निकास);
    }

# जर HASH_ALGO_CKSUM
  जर (base64_digest)
    {
      अक्षर b64[BASE64_LENGTH (DIGEST_BIN_BYTES) + 1];
      base64_encode ((अक्षर स्थिरांक *) bin_buffer, digest_length >> 3,
                     b64, आकार b64);
      ख_माला_दो (b64, मानक_निकास);
    }
  अन्यथा
# endif
    {
      साठी (idx_t i = 0; i < (digest_hex_bytes >> 1); ++i)
        म_लिखो ("%02x", bin_buffer[i]);
    }

  जर (!tagged)
    {
      अक्षर_दो (' ');
      अक्षर_दो (द्विमान_file ? '*' : ' ');
      print_filename (file, needs_escape);
    }

  अक्षर_दो (delim);
}
#endif

#जर HASH_ALGO_CKSUM
/* Return true if B64_DIGEST is the same as the base64 digest of the
   DIGEST_LENGTH/8 bytes at BIN_BUFFER.  */
स्थिर bool
b64_equal (अचिन्हित अक्षर स्थिरांक *b64_digest, अचिन्हित अक्षर स्थिरांक *bin_buffer)
{
  idx_t b64_n_bytes = BASE64_LENGTH (digest_length >> 3);
  अक्षर b64[BASE64_LENGTH (DIGEST_BIN_BYTES) + 1];
  base64_encode ((अक्षर स्थिरांक *) bin_buffer, digest_length >> 3,
                 b64, आकार b64);
  परत memeq (b64_digest, b64, b64_n_bytes + 1);
}
#endif

/* Return true if HEX_DIGEST is the same as the hex-encoded digest of the
   DIGEST_LENGTH/8 bytes at BIN_BUFFER.  */
स्थिर bool
hex_equal (अचिन्हित अक्षर स्थिरांक *hex_digest, अचिन्हित अक्षर स्थिरांक *bin_buffer)
{
  स्थिर स्थिरांक अक्षर bin2hex[] = { '0', '1', '2', '3',
                                  '4', '5', '6', '7',
                                  '8', '9', 'a', 'b',
                                  'c', 'd', 'e', 'f' };
  idx_t digest_bin_bytes = digest_hex_bytes >> 1;

  /* Compare generated binary number with text representation
     in check file.  Ignore case of hex digits.  */
  idx_t cnt;
  साठी (cnt = 0; cnt < digest_bin_bytes; ++cnt)
    {
      जर (c_tolower (hex_digest[2 * cnt])
          != bin2hex[bin_buffer[cnt] >> 4]
          || (c_tolower (hex_digest[2 * cnt + 1])
              != (bin2hex[bin_buffer[cnt] & 0xf])))
        थांबा;
    }
  परत cnt == digest_bin_bytes;
}

स्थिर bool
digest_check (अक्षर स्थिरांक *checkfile_name)
{
  खाता *checkfile_stream;
  intmax_t n_misformatted_ओळी = 0;
  intmax_t n_mismatched_checksums = 0;
  intmax_t n_open_or_read_failures = 0;
  bool properly_formatted_ओळी = false;
  bool matched_checksums = false;
  अचिन्हित अक्षर bin_buffer_unaligned[DIGEST_BIN_BYTES + DIGEST_ALIGN];
  /* Make sure bin_buffer is properly aligned. */
  अचिन्हित अक्षर *bin_buffer = ptr_align (bin_buffer_unaligned, DIGEST_ALIGN);
  intmax_t ओळ_संख्या;
  अक्षर *ओळ;
  माप_प्रकार ओळ_chars_allocated;
  bool is_stdin = streq (checkfile_name, "-");

  जर (is_stdin)
    {
      have_read_stdin = true;
      checkfile_name = _("standard input");
      checkfile_stream = मानक_निवेश;
    }
  अन्यथा
    {
      checkfile_stream = ख_खोलो (checkfile_name, "r");
      जर (checkfile_stream == रिक्त)
        {
          error (0, त्रुटि_सं, "%s", quotef (checkfile_name));
          परत false;
        }
    }

  ओळ_संख्या = 0;
  ओळ = रिक्त;
  ओळ_chars_allocated = 0;
  करा
    {
      अक्षर *filename;
      पूर्णांक द्विमान;
      अचिन्हित अक्षर *digest;
      ssize_t ओळ_length;

      ++ओळ_संख्या;
      जर (ओळ_संख्या == 0)
        error (निकास_त्रुटि, 0, _("%s: too many checksum lines"),
               quotef (checkfile_name));

      ओळ_length = getline (&ओळ, &ओळ_chars_allocated, checkfile_stream);
      जर (ओळ_length <= 0)
        थांबा;

      /* Ignore comment lines, which begin with a '#' character.  */
      जर (ओळ[0] == '#')
        सुरूठेवा;

      /* Remove any trailing newline.  */
      ओळ_length -= ओळ[ओळ_length - 1] == '\n';
      /* Remove any trailing carriage return.  */
      ओळ_length -= ओळ[ओळ_length - (0 < ओळ_length)] == '\r';

      /* Ignore empty lines.  */
      जर (ओळ_length == 0)
        सुरूठेवा;

      ओळ[ओळ_length] = '\0';

      idx_t d_len;
      जर (! (split_3 (ओळ, ओळ_length, &digest, &d_len, &द्विमान, &filename)
             && ! (is_stdin && streq (filename, "-"))))
        {
          ++n_misformatted_ओळी;

          जर (warn)
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

          जर (!ok)
            {
              ++n_open_or_read_failures;
              जर (!status_only)
                म_लिखो ("%s: %s\n", quotef (filename),_("FAILED open or read"));
            }
          अन्यथा जर (ignore_missing && missing)
            {
              /* Ignore missing files with --ignore-missing.  */
              ;
            }
          अन्यथा
            {
              bool match = false;
#जर HASH_ALGO_CKSUM
              जर (d_len == BASE64_LENGTH (digest_length >> 3))
                match = b64_equal (digest, bin_buffer);
              अन्यथा
#endif
                जर (d_len == digest_hex_bytes)
                  match = hex_equal (digest, bin_buffer);

              जर (match)
                matched_checksums = true;
              अन्यथा
                ++n_mismatched_checksums;

              जर (!status_only)
                {
                  जर (! match || ! quiet)
                    ख_माला_दो (quotef (filename), मानक_निकास);

                  जर (! match)
                    म_लिखो (": %s\n", _("FAILED"));
                  अन्यथा जर (!quiet)
                    म_लिखो (": %s\n", _("OK"));
                }
            }

          जर (ख_त्रुटि (मानक_निकास))
            write_error ();
        }
    }
  पर्यंत (!ख_पूर्ण (checkfile_stream) && !ख_त्रुटि (checkfile_stream));

  मुक्त (ओळ);

  पूर्णांक err = ख_त्रुटि (checkfile_stream) ? 0 : -1;
  जर (is_stdin)
    साफ_त्रुटि (checkfile_stream);
  अन्यथा जर (ख_बंद (checkfile_stream) != 0 && err < 0)
    err = त्रुटि_सं;

  जर (0 <= err)
    {
      error (0, err, err ? "%s" : _("%s: read error"),
             quotef (checkfile_name));
      परत false;
    }

  जर (! properly_formatted_ओळी)
    {
      /* Warn if no tests are found.  */
      error (0, 0, _("%s: no properly formatted checksum lines found"),
             quotef (checkfile_name));
    }
  अन्यथा
    {
      जर (!status_only)
        {
          जर (n_misformatted_ओळी != 0)
            error (0, 0,
                   (ngettext
                    ("WARNING: %jd line is improperly formatted",
                     "WARNING: %jd lines are improperly formatted",
                     select_plural (n_misformatted_ओळी))),
                   n_misformatted_ओळी);

          जर (n_open_or_read_failures != 0)
            error (0, 0,
                   (ngettext
                    ("WARNING: %jd listed file could not be read",
                     "WARNING: %jd listed files could not be read",
                     select_plural (n_open_or_read_failures))),
                   n_open_or_read_failures);

          जर (n_mismatched_checksums != 0)
            error (0, 0,
                   (ngettext
                    ("WARNING: %jd computed checksum did NOT match",
                     "WARNING: %jd computed checksums did NOT match",
                     select_plural (n_mismatched_checksums))),
                   n_mismatched_checksums);

          जर (ignore_missing && ! matched_checksums)
            error (0, 0, _("%s: no file was verified"),
                   quotef (checkfile_name));
        }
    }

  परत (properly_formatted_ओळी
          && matched_checksums
          && n_mismatched_checksums == 0
          && n_open_or_read_failures == 0
          && (!strict || n_misformatted_ओळी == 0));
}

पूर्णांक
main (पूर्णांक argc, अक्षर **argv)
{
  अचिन्हित अक्षर bin_buffer_unaligned[DIGEST_BIN_BYTES + DIGEST_ALIGN];
  /* Make sure bin_buffer is properly aligned. */
  अचिन्हित अक्षर *bin_buffer = ptr_align (bin_buffer_unaligned, DIGEST_ALIGN);
  bool do_check = false;
  पूर्णांक opt;
  bool ok = true;
  पूर्णांक द्विमान = -1;
  पूर्णांक prefix_tag = -1;
  संरचना option स्थिरांक *long_opts = long_options;

#जर HASH_ALGO_CKSUM
  जर (cksum_algorithm != crc)
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
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  /* Line buffer stdout to ensure lines are written atomically and immediately
     so that processes running in parallel do not intersperse their output.  */
  रखो_भबफ (मानक_निकास, रिक्त, _IOLBF, 0);

#जर HASH_ALGO_SUM
  अक्षर स्थिरांक *short_opts = "rs";
#किंवाजर HASH_ALGO_CKSUM
  अक्षर स्थिरांक *short_opts = legacy_mode ? "bctwz" : "a:l:bctwz";
  अक्षर स्थिरांक *digest_length_str = "";
#किंवाजर HASH_ALGO_BLAKE2
  अक्षर स्थिरांक *short_opts = "l:bctwz";
  अक्षर स्थिरांक *digest_length_str = "";
#अन्यथा
  अक्षर स्थिरांक *short_opts = "bctwz";
#endif

  पर्यंत ((opt = getopt_long (argc, argv, short_opts, long_opts, रिक्त))
         != -1)
    निवड (opt)
      {
#जर HASH_ALGO_CKSUM
      स्थिती 'a':
        cksum_algorithm = XARGMATCH_EXACT ("--algorithm", optarg,
                                           algorithm_args, algorithm_types);
        algorithm_specified = true;
        थांबा;

      स्थिती DEBUG_PROGRAM_OPTION:
        cksum_debug = true;
        थांबा;
#endif
#जर HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
      स्थिती 'l':
        digest_length = xnumtoimax (optarg, 10, 0, IDX_MAX, "",
                                    _("invalid length"), 0,
                                    XTOINT_MAX_QUIET);
        digest_length_str = optarg;
        थांबा;
#endif
#जर !HASH_ALGO_SUM
      स्थिती 'c':
        do_check = true;
        थांबा;
      स्थिती STATUS_OPTION:
        status_only = true;
        warn = false;
        quiet = false;
        थांबा;
      स्थिती 'b':
        द्विमान = 1;
        थांबा;
      स्थिती 't':
        द्विमान = 0;
        थांबा;
      स्थिती 'w':
        status_only = false;
        warn = true;
        quiet = false;
        थांबा;
      स्थिती IGNORE_MISSING_OPTION:
        ignore_missing = true;
        थांबा;
      स्थिती QUIET_OPTION:
        status_only = false;
        warn = false;
        quiet = true;
        थांबा;
      स्थिती STRICT_OPTION:
        strict = true;
        थांबा;
# जर HASH_ALGO_CKSUM
      स्थिती BASE64_OPTION:
        base64_digest = true;
        थांबा;
      स्थिती RAW_OPTION:
        raw_digest = true;
        थांबा;
      स्थिती UNTAG_OPTION:
        prefix_tag = 0;
        थांबा;
# endif
      स्थिती TAG_OPTION:
        prefix_tag = 1;
        थांबा;
      स्थिती 'z':
        digest_delim = '\0';
        थांबा;
#endif
#जर HASH_ALGO_SUM
      स्थिती 'r':		/* For SysV compatibility. */
        sum_algorithm = bsd;
        थांबा;

      स्थिती 's':
        sum_algorithm = sysv;
        थांबा;
#endif
      case_GETOPT_HELP_CHAR;
      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
      पूर्वनिर्धारित:
        usage (निकास_त्रुटि);
      }

  min_digest_ओळ_length = MIN_DIGEST_ओळ_LENGTH;
#जर HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# जर HASH_ALGO_CKSUM
  जर (digest_length && (cksum_algorithm != blake2b
                        && cksum_algorithm != sha2
                        && cksum_algorithm != sha3))
    error (निकास_त्रुटि, 0,
           _("--length is only supported with --algorithm "
             "blake2b, sha2, or sha3"));
  जर (cksum_algorithm == sha2 || cksum_algorithm == sha3)
    {
      /* Do not require --length with --check.  */
      जर (digest_length == 0 && *digest_length_str == '\0' && ! do_check)
        error (निकास_त्रुटि, 0, _("--algorithm=%s requires specifying "
                                  "--length 224, 256, 384, or 512"),
               algorithm_args[cksum_algorithm]);
      /* If --check and --length are used we verify the digest length.  */
      जर ((! do_check || *digest_length_str != '\0')
          && digest_length != SHA224_DIGEST_SIZE * 8
          && digest_length != SHA256_DIGEST_SIZE * 8
          && digest_length != SHA384_DIGEST_SIZE * 8
          && digest_length != SHA512_DIGEST_SIZE * 8)
        {
          error (0, 0, _("invalid length: %s"), quote (digest_length_str));
          error (निकास_त्रुटि, 0, _("digest length for %s must be "
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
      जर (digest_length > DIGEST_MAX_LEN * 8)
        {
          error (0, 0, _("invalid length: %s"), quote (digest_length_str));
          error (निकास_त्रुटि, 0,
                 _("maximum digest length for %s is %d bits"),
                 quote (DIGEST_TYPE_STRING),
                 DIGEST_MAX_LEN * 8);
        }
      जर (digest_length % 8 != 0)
        {
          error (0, 0, _("invalid length: %s"), quote (digest_length_str));
          error (निकास_त्रुटि, 0, _("length is not a multiple of 8"));
        }
    }
  जर (digest_length == 0)
    {
# जर HASH_ALGO_BLAKE2
      digest_length = DIGEST_MAX_LEN * 8;
# अन्यथा
      digest_length = algorithm_bits[cksum_algorithm];
# endif
    }
  digest_hex_bytes = digest_length >> 2;
#अन्यथा
  digest_hex_bytes = DIGEST_HEX_BYTES;
#endif

#जर HASH_ALGO_CKSUM
  निवड (+cksum_algorithm)
    {
    स्थिती bsd:
    स्थिती sysv:
    स्थिती crc:
    स्थिती crc32b:
        जर (do_check && algorithm_specified)
          error (निकास_त्रुटि, 0,
                 _("--check is not supported with "
                   "--algorithm={bsd,sysv,crc,crc32b}"));
        थांबा;
    }

  जर (base64_digest && raw_digest)
   {
     error (0, 0, _("--base64 and --raw are mutually exclusive"));
     usage (निकास_त्रुटि);
   }
#endif

  जर (digest_delim != '\n' && do_check)
    {
      error (0, 0, _("the --zero option is not supported when "
                     "verifying checksums"));
      usage (निकास_त्रुटि);
    }
  जर (1 <= prefix_tag && do_check)
    {
      /* Note we allow --untagged with --check to more
         seamlessly support --untagged in an emulation wrapper.  */
      error (0, 0, _("the --tag option is meaningless when "
                     "verifying checksums"));
      usage (निकास_त्रुटि);
    }

  जर (0 <= द्विमान && do_check)
    {
      error (0, 0, _("the --binary and --text options are meaningless when "
                     "verifying checksums"));
      usage (निकास_त्रुटि);
    }

  जर (ignore_missing && !do_check)
    {
      error (0, 0,
             _("the --ignore-missing option is meaningful only when "
               "verifying checksums"));
      usage (निकास_त्रुटि);
    }

  जर (status_only && !do_check)
    {
      error (0, 0,
       _("the --status option is meaningful only when verifying checksums"));
      usage (निकास_त्रुटि);
    }

  जर (warn && !do_check)
    {
      error (0, 0,
       _("the --warn option is meaningful only when verifying checksums"));
      usage (निकास_त्रुटि);
    }

  जर (quiet && !do_check)
    {
      error (0, 0,
       _("the --quiet option is meaningful only when verifying checksums"));
      usage (निकास_त्रुटि);
    }

  जर (strict & !do_check)
   {
     error (0, 0,
        _("the --strict option is meaningful only when verifying checksums"));
     usage (निकास_त्रुटि);
   }

  जर (prefix_tag == -1)
    prefix_tag = HASH_ALGO_CKSUM;

  जर (prefix_tag && !द्विमान)
   {
     /* This could be supported in a backwards compatible way
        by prefixing the output line with a space in text mode.
        However that's invasive enough that it was agreed to
        not support this mode with --tag, as --text use cases
        are adequately supported by the default output format.  */
#जर !HASH_ALGO_CKSUM
     error (0, 0, _("--tag does not support --text mode"));
#अन्यथा
     error (0, 0, _("--text mode is only supported with --untagged"));
#endif
     usage (निकास_त्रुटि);
   }

  जर (!O_द्विमान && द्विमान < 0)
    द्विमान = 0;
  अन्यथा जर (prefix_tag)
    द्विमान = 1;

  अक्षर **operand_lim = argv + argc;
  जर (optind == argc)
    *operand_lim++ = bad_cast ("-");
  अन्यथा जर (1 < argc - optind && raw_digest)
    error (निकास_त्रुटि, 0,
           _("the --raw option is not supported with multiple files"));

  साठी (अक्षर **operandp = argv + optind; operandp < operand_lim; operandp++)
    {
      अक्षर *file = *operandp;
      जर (do_check)
        ok &= digest_check (file);
      अन्यथा
        {
          पूर्णांक द्विमान_file = द्विमान;
          bool missing;
          intmax_t length;

          जर (! digest_file (file, &द्विमान_file, bin_buffer, &missing, &length))
            ok = false;
          अन्यथा
            {
              DIGEST_OUT (file, द्विमान_file, bin_buffer, raw_digest, prefix_tag,
                          digest_delim, optind != argc, length);
              जर (ख_त्रुटि (मानक_निकास))
                write_error ();
            }
        }
    }

  जर (have_read_stdin && ख_बंद (मानक_निवेश) == खातापूर्ण)
    error (निकास_त्रुटि, त्रुटि_सं, _("standard input"));

  परत ok ? निकास_सफल : निकास_त्रुटि;
}
