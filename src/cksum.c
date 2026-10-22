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

#समावेश <config.h>

#समावेश <getopt.h>
#समावेश <sys/types.h>

#समावेश "assure.h"
#समावेश "system.h"
#समावेश "argmatch.h"
#समावेश "c-ctype.h"
#समावेश "quote.h"
#समावेश "xdectoint.h"
#समावेश "xstrtol.h"

#अगर_अघोषित HASH_ALGO_CKSUM
# define HASH_ALGO_CKSUM 0
#पूर्ण_अगर

#अगर HASH_ALGO_योग || HASH_ALGO_CKSUM
# include "sum.h"
#पूर्ण_अगर
#अगर HASH_ALGO_CKSUM
# include "cksum.h"
# include "cksum_crc.h"
# include "base64.h"
#पूर्ण_अगर
#अगर HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# include "blake2/b2sum.h"
#पूर्ण_अगर
#अगर HASH_ALGO_MD5 || HASH_ALGO_CKSUM
# include "md5.h"
#पूर्ण_अगर
#अगर HASH_ALGO_SHA1 || HASH_ALGO_CKSUM
# include "sha1.h"
#पूर्ण_अगर
#अगर HASH_ALGO_SHA256 || HASH_ALGO_SHA224 || HASH_ALGO_CKSUM
# include "sha256.h"
#पूर्ण_अगर
#अगर HASH_ALGO_SHA512 || HASH_ALGO_SHA384 || HASH_ALGO_CKSUM
# include "sha512.h"
#पूर्ण_अगर
#अगर HASH_ALGO_CKSUM
# include "sha3.h"
#पूर्ण_अगर
#अगर HASH_ALGO_CKSUM
# include "sm3.h"
#पूर्ण_अगर
#समावेश "fadvise.h"
#समावेश "stdio--.h"
#समावेश "xbinary-io.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#अगर HASH_ALGO_योग
# define PROGRAM_NAME "sum"
# define DIGEST_TYPE_STRING "BSD"
# define DIGEST_STREAM sumfns[योग_algorithm]
# define DIGEST_OUT योग_output_fns[योग_algorithm]
# define DIGEST_BITS 16
# define DIGEST_ALIGN 4
#या_अगर HASH_ALGO_CKSUM
# define MAX_DIGEST_BITS 512
# define MAX_DIGEST_ALIGN 8
# define DIGEST_TYPE_STRING algorithm_tags[cksum_algorithm]
# define DIGEST_STREAM cksumfns[cksum_algorithm]
# define DIGEST_OUT cksum_output_fns[cksum_algorithm]
# define DIGEST_BITS MAX_DIGEST_BITS
# define DIGEST_ALIGN MAX_DIGEST_ALIGN
#या_अगर HASH_ALGO_MD5
# define PROGRAM_NAME "md5sum"
# define DIGEST_TYPE_STRING "MD5"
# define DIGEST_STREAM md5_stream
# define DIGEST_BITS 128
# define DIGEST_REFERENCE "RFC 1321"
# define DIGEST_ALIGN 4
#या_अगर HASH_ALGO_BLAKE2
# define PROGRAM_NAME "b2sum"
# define DIGEST_TYPE_STRING "BLAKE2b"
# define DIGEST_STREAM blake2b_stream
# define DIGEST_BITS 512
# define DIGEST_REFERENCE "RFC 7693"
# define DIGEST_ALIGN 8
#या_अगर HASH_ALGO_SHA1
# define PROGRAM_NAME "sha1sum"
# define DIGEST_TYPE_STRING "SHA1"
# define DIGEST_STREAM sha1_stream
# define DIGEST_BITS 160
# define DIGEST_REFERENCE "FIPS-180-1"
# define DIGEST_ALIGN 4
#या_अगर HASH_ALGO_SHA256
# define PROGRAM_NAME "sha256sum"
# define DIGEST_TYPE_STRING "SHA256"
# define DIGEST_STREAM sha256_stream
# define DIGEST_BITS 256
# define DIGEST_REFERENCE "FIPS-180-2"
# define DIGEST_ALIGN 4
#या_अगर HASH_ALGO_SHA224
# define PROGRAM_NAME "sha224sum"
# define DIGEST_TYPE_STRING "SHA224"
# define DIGEST_STREAM sha224_stream
# define DIGEST_BITS 224
# define DIGEST_REFERENCE "RFC 3874"
# define DIGEST_ALIGN 4
#या_अगर HASH_ALGO_SHA512
# define PROGRAM_NAME "sha512sum"
# define DIGEST_TYPE_STRING "SHA512"
# define DIGEST_STREAM sha512_stream
# define DIGEST_BITS 512
# define DIGEST_REFERENCE "FIPS-180-2"
# define DIGEST_ALIGN 8
#या_अगर HASH_ALGO_SHA384
# define PROGRAM_NAME "sha384sum"
# define DIGEST_TYPE_STRING "SHA384"
# define DIGEST_STREAM sha384_stream
# define DIGEST_BITS 384
# define DIGEST_REFERENCE "FIPS-180-2"
# define DIGEST_ALIGN 8
#अन्यथा
# error "Can't decide which hash algorithm to compile."
#पूर्ण_अगर
#अगर !HASH_ALGO_योग && !HASH_ALGO_CKSUM
# define DIGEST_OUT output_file
#पूर्ण_अगर

#अगर HASH_ALGO_CKSUM
# define PROGRAM_NAME (!legacy_mode ? "cksum"			 \
                       : cksum_algorithm == md5 ? "md5sum"	 \
                       : cksum_algorithm == sha1 ? "sha1sum"	 \
                       : cksum_algorithm == sha224 ? "sha224sum" \
                       : cksum_algorithm == sha256 ? "sha256sum" \
                       : cksum_algorithm == sha384 ? "sha384sum" \
                       : cksum_algorithm == sha512 ? "sha512sum" \
                       : "cksum")
#पूर्ण_अगर

#अगर HASH_ALGO_योग
# define AUTHORS \
  proper_name ("Kayvan Aghaiepour"), \
  proper_name ("David MacKenzie")
#या_अगर HASH_ALGO_CKSUM
# define AUTHORS \
  proper_name_lite ("Padraig Brady", "P\303\241draig Brady"), \
  proper_name ("Q. Frank Xia")
#या_अगर HASH_ALGO_BLAKE2
# define AUTHORS \
  proper_name_lite ("Padraig Brady", "P\303\241draig Brady"), \
  proper_name ("Samuel Neves")
#अन्यथा
# define AUTHORS \
  proper_name ("Ulrich Drepper"), \
  proper_name ("Scott Miller"), \
  proper_name ("David Madore")
#पूर्ण_अगर
#अगर !HASH_ALGO_BLAKE2 && !HASH_ALGO_CKSUM
# define DIGEST_HEX_BYTES (DIGEST_BITS / 4)
#पूर्ण_अगर
#घोषणा DIGEST_BIN_BYTES (DIGEST_BITS / 8)

/* The minimum length of a valid digest line.  This length does
   not include any newline character at the end of a line.  */
#अगर HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# define MIN_DIGEST_पंक्ति_लंबाई 3 /* With -l 8.  */
#अन्यथा
# define MIN_DIGEST_पंक्ति_लंबाई \
   (DIGEST_HEX_BYTES /* length of hexadecimal message digest */ \
    + 1 /* blank */ \
    + 1 /* minimum filename length */ )
#पूर्ण_अगर

#अगर !HASH_ALGO_योग
अटल व्योम
output_file (अक्षर स्थिर *file, पूर्णांक द्विआधारी_file, व्योम स्थिर *digest,
             bool raw, bool tagged, अचिन्हित अक्षर delim, bool args,
             intmax_t लंबाई);
#पूर्ण_अगर

/* True if any of the files read were the standard input. */
अटल bool have_read_stdin;

/* The minimum length of a valid checksum line for the selected algorithm.  */
अटल idx_t min_digest_पंक्ति_लंबाई;

/* Set to the length of a digest hex string for the selected algorithm.  */
अटल idx_t digest_hex_bytes;

/* With --check, don't generate any output.
   The exit code indicates success or failure.  */
अटल bool status_only = false;

/* With --check, print a message to standard error warning about each
   improperly formatted checksum line.  */
अटल bool warn = false;

/* With --check, ignore missing files.  */
अटल bool ignore_missing = false;

/* With --check, suppress the "OK" printed for each verified file.  */
अटल bool quiet = false;

/* With --check, exit with a non-zero return code if any line is
   improperly formatted. */
अटल bool strict = false;

/* Whether a BSD reversed format checksum is detected.  */
अटल पूर्णांक bsd_reversed = -1;

/* line delimiter.  */
अटल अचिन्हित अक्षर digest_delim = '\n';

#अगर HASH_ALGO_CKSUM
/* If true, print base64-encoded digests, not hex.  */
अटल bool base64_digest = false;
#पूर्ण_अगर

/* If true, print binary digests, not hex.  */
अटल bool raw_digest = false;

/* blake2b and sha3 allow the -l option.  Luckily they both have the same
   maximum digest size.  */
#अगर HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# अगर HASH_ALGO_CKSUM
static_assert (BLAKE2B_OUTBYTES == SHA3_512_DIGEST_आकार);
# endif
# define DIGEST_MAX_LEN BLAKE2B_OUTBYTES
अटल idx_t digest_लंबाई;
#पूर्ण_अगर /* HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM */

प्रकार व्योम (*digest_output_fn)(अक्षर स्थिर *, पूर्णांक, व्योम स्थिर *, bool,
                                 bool, अचिन्हित अक्षर, bool, intmax_t);
#अगर HASH_ALGO_योग
क्रमागत Algorithm
{
  bsd,
  sysv,
};

अटल क्रमागत Algorithm योग_algorithm;
अटल sumfn sumfns[]=
{
  bsd_योग_stream,
  sysv_योग_stream,
};
अटल digest_output_fn योग_output_fns[]=
{
  output_bsd,
  output_sysv,
};
#पूर्ण_अगर

#अगर HASH_ALGO_CKSUM
अटल पूर्णांक
md5_योग_stream (खाता *stream, व्योम *resstream, MAYBE_UNUSED intmax_t *लंबाई)
{
  वापस md5_stream (stream, resstream);
}
अटल पूर्णांक
sha1_योग_stream (खाता *stream, व्योम *resstream, MAYBE_UNUSED intmax_t *लंबाई)
{
  वापस sha1_stream (stream, resstream);
}
अटल पूर्णांक
sha224_योग_stream (खाता *stream, व्योम *resstream,
                   MAYBE_UNUSED intmax_t *लंबाई)
{
  वापस sha224_stream (stream, resstream);
}
अटल पूर्णांक
sha256_योग_stream (खाता *stream, व्योम *resstream,
                   MAYBE_UNUSED intmax_t *लंबाई)
{
  वापस sha256_stream (stream, resstream);
}
अटल पूर्णांक
sha384_योग_stream (खाता *stream, व्योम *resstream,
                   MAYBE_UNUSED intmax_t *लंबाई)
{
  वापस sha384_stream (stream, resstream);
}
अटल पूर्णांक
sha512_योग_stream (खाता *stream, व्योम *resstream,
                   MAYBE_UNUSED intmax_t *लंबाई)
{
  वापस sha512_stream (stream, resstream);
}
अटल पूर्णांक
sha2_योग_stream (खाता *stream, व्योम *resstream, intmax_t *लंबाई)
{
  चयन (*लंबाई)
    {
    हाल SHA224_DIGEST_आकार:
      वापस sha224_stream (stream, resstream);
    हाल SHA256_DIGEST_आकार:
      वापस sha256_stream (stream, resstream);
    हाल SHA384_DIGEST_आकार:
      वापस sha384_stream (stream, resstream);
    हाल SHA512_DIGEST_आकार:
      वापस sha512_stream (stream, resstream);
    शेष:
      affirm (false);
    }
}
अटल पूर्णांक
sha3_योग_stream (खाता *stream, व्योम *resstream, intmax_t *लंबाई)
{
  चयन (*लंबाई)
    {
    हाल SHA3_224_DIGEST_आकार:
      वापस sha3_224_stream (stream, resstream);
    हाल SHA3_256_DIGEST_आकार:
      वापस sha3_256_stream (stream, resstream);
    हाल SHA3_384_DIGEST_आकार:
      वापस sha3_384_stream (stream, resstream);
    हाल SHA3_512_DIGEST_आकार:
      वापस sha3_512_stream (stream, resstream);
    शेष:
      affirm (false);
    }
}
अटल पूर्णांक
blake2b_योग_stream (खाता *stream, व्योम *resstream, intmax_t *लंबाई)
{
  वापस blake2b_stream (stream, resstream, *लंबाई);
}
अटल पूर्णांक
sm3_योग_stream (खाता *stream, व्योम *resstream, MAYBE_UNUSED intmax_t *लंबाई)
{
  वापस sm3_stream (stream, resstream);
}

अटल अक्षर स्थिर *स्थिर algorithm_args[] =
{
  "bsd", "sysv", "crc", "crc32b", "md5", "sha1",
  "sha224", "sha256", "sha384", "sha512", /* Legacy naming */
  "sha2", "sha3", "blake2b", "sm3", शून्य
};
अटल क्रमागत Algorithm स्थिर algorithm_types[] =
{
  bsd, sysv, crc, crc32b, md5, sha1,
  sha224, sha256, sha384, sha512,
  sha2, sha3, blake2b, sm3,
};
ARGMATCH_VERIFY (algorithm_args, algorithm_types);

अटल अक्षर स्थिर *स्थिर algorithm_tags[] =
{
  "BSD", "SYSV", "CRC", "CRC32B", "MD5", "SHA1",
  "SHA224", "SHA256", "SHA384", "SHA512",
  "SHA2", "SHA3", "BLAKE2b", "SM3", शून्य
};
अटल पूर्णांक स्थिर algorithm_bits[] =
{
  16, 16, 32, 32, 128, 160,
  224, 256, 384, 512,
  512, 512, 512, 256, 0
};

static_assert (countof (algorithm_bits) == countof (algorithm_args));

अटल bool algorithm_specified = false;
अटल bool legacy_mode = false;
क्रमागत Algorithm cksum_algorithm = crc;

अटल sumfn cksumfns[]=
{
  bsd_योग_stream,
  sysv_योग_stream,
  crc_योग_stream,
  crc32b_योग_stream,
  md5_योग_stream,
  sha1_योग_stream,
  sha224_योग_stream,
  sha256_योग_stream,
  sha384_योग_stream,
  sha512_योग_stream,
  sha2_योग_stream,
  sha3_योग_stream,
  blake2b_योग_stream,
  sm3_योग_stream,
};
अटल digest_output_fn cksum_output_fns[]=
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
#पूर्ण_अगर

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */

क्रमागत
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

#अगर HASH_ALGO_CKSUM
अटल काष्ठा option स्थिर legacy_long_options[] =
{
  { "check", no_argument, शून्य, 'c' },
  { "ignore-missing", no_argument, शून्य, IGNORE_MISSING_OPTION },
  { "quiet", no_argument, शून्य, QUIET_OPTION },
  { "status", no_argument, शून्य, STATUS_OPTION },
  { "warn", no_argument, शून्य, 'w' },
  { "strict", no_argument, शून्य, STRICT_OPTION },
  { "tag", no_argument, शून्य, TAG_OPTION },
  { "zero", no_argument, शून्य, 'z' },

  { "binary", no_argument, शून्य, 'b' }, /* Deprecated.  */
  { "text", no_argument, शून्य, 't' }, /* Deprecated.  */

  { GETOPT_HELP_OPTION_DECL },
  { GETOPT_VERSION_OPTION_DECL },
  { शून्य, 0, शून्य, 0 }
};
#पूर्ण_अगर

अटल काष्ठा option स्थिर long_options[] =
{
#अगर HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
  { "length", required_argument, शून्य, 'l'},
#पूर्ण_अगर

#अगर !HASH_ALGO_योग
  { "check", no_argument, शून्य, 'c' },
  { "ignore-missing", no_argument, शून्य, IGNORE_MISSING_OPTION},
  { "quiet", no_argument, शून्य, QUIET_OPTION },
  { "status", no_argument, शून्य, STATUS_OPTION },
  { "warn", no_argument, शून्य, 'w' },
  { "strict", no_argument, शून्य, STRICT_OPTION },
  { "tag", no_argument, शून्य, TAG_OPTION },
  { "zero", no_argument, शून्य, 'z' },

# अगर HASH_ALGO_CKSUM
  { "algorithm", required_argument, शून्य, 'a'},
  { "base64", no_argument, शून्य, BASE64_OPTION },
  { "debug", no_argument, शून्य, DEBUG_PROGRAM_OPTION},
  { "raw", no_argument, शून्य, RAW_OPTION},
  { "untagged", no_argument, शून्य, UNTAG_OPTION },
# endif
  { "binary", no_argument, शून्य, 'b' }, /* Deprecated.  */
  { "text", no_argument, शून्य, 't' }, /* Deprecated.  */

#अन्यथा
  {"sysv", no_argument, शून्य, 's'},  /* Not in cksum.  */
#पूर्ण_अगर

  { GETOPT_HELP_OPTION_DECL },
  { GETOPT_VERSION_OPTION_DECL },
  { शून्य, 0, शून्य, 0 }
};

व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s [OPTION]... [खाता]...\n\
"), program_name);
#अगर HASH_ALGO_CKSUM
    अगर (legacy_mode)
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
By शेष use the 32 bit CRC algorithm.\n\
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
#पूर्ण_अगर

      emit_stdin_note ();
#अगर HASH_ALGO_योग
      oputs (_("\
  -r\n\
         use BSD योग algorithm (the शेष), use 1K blocks\n\
"));
      oputs (_("\
  -s, --sysv\n\
         use System V योग algorithm, use 512 bytes blocks\n\
"));
#पूर्ण_अगर
#अगर HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
        emit_mandatory_arg_note ();
#पूर्ण_अगर
#अगर HASH_ALGO_CKSUM
     अगर (! legacy_mode)
        oputs (_("\
  -a, --algorithm=TYPE\n\
         select the digest type to use.  See DIGEST below\n\
"));
     अगर (! legacy_mode)
        oputs (_("\
      --base64\n\
         emit base64-encoded digests, not hexadecimal\n\
"));
#पूर्ण_अगर
#अगर !HASH_ALGO_योग
# अगर HASH_ALGO_CKSUM
     अगर (legacy_mode)
       {
# endif
      अगर (O_द्विआधारी)
        oputs (_("\
  -b, --द्विआधारी\n\
         पढ़ो in द्विआधारी mode (शेष unless reading tty मानक_निवेश)\n\
"));
      अन्यथा
        oputs (_("\
  -b, --द्विआधारी\n\
         पढ़ो in द्विआधारी mode\n\
"));
# अगर HASH_ALGO_CKSUM
       }
# endif
        oputs (_("\
  -c, --check\n\
         पढ़ो checksums from the FILEs and check them\n\
"));
# अगर HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
#  अगर HASH_ALGO_CKSUM
     अगर (! legacy_mode)
#  endif
        oputs (_("\
  -l, --लंबाई=BITS\n\
         digest लंबाई in bits; must not exceed the max आकार\n\
         and must be a multiple of 8 क्रम blake2b;\n\
         must be 224, 256, 384, or 512 क्रम sha2 or sha3\n\
"));
# endif
# अगर HASH_ALGO_CKSUM
    अगर (! legacy_mode)
        oputs (_("\
      --raw\n\
         emit a raw द्विआधारी digest, not hexadecimal\n\
"));
    अगर (legacy_mode)
      oputs (_("\
      --tag\n\
         create a BSD-style checksum\n\
"));
    अन्यथा
      oputs (_("\
      --tag\n\
         create a BSD-style checksum (the शेष)\n\
"));
    अगर (! legacy_mode)
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
# अगर HASH_ALGO_CKSUM
    अगर (legacy_mode)
      {
# endif
      अगर (O_द्विआधारी)
        oputs (_("\
  -t, --text\n\
         पढ़ो in text mode (शेष अगर reading tty मानक_निवेश)\n\
"));
      अन्यथा
        oputs (_("\
  -t, --text\n\
         पढ़ो in text mode (शेष)\n\
"));
# अगर HASH_ALGO_CKSUM
      }
# endif
      oputs (_("\
  -z, --zero\n\
         end each output पंक्ति with NUL, not newline,\n\
         and disable file name escaping\n\
"));
      ख_माला_दो (_("\
\n\
The following five options are useful only when verifying checksums:\n\
"), मानक_निकास);
      oputs (_("\
      --ignore-missing\n\
         don't fail or report status क्रम missing files\n\
"));
      oputs (_("\
      --quiet\n\
         don't print OK क्रम each successfully verified file\n\
"));
      oputs (_("\
      --status\n\
         don't output anything, status code shows success\n\
"));
      oputs (_("\
      --strict\n\
         निकास non-zero क्रम improperly formatted checksum पंक्तियाँ\n\
"));
      oputs (_("\
  -w, --warn\n\
         warn about improperly formatted checksum पंक्तियाँ\n\
"));
#पूर्ण_अगर
#अगर HASH_ALGO_CKSUM
    अगर (! legacy_mode)
      oputs (_("\
      --debug\n\
         indicate which implementation used\n\
"));
#पूर्ण_अगर
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
#अगर HASH_ALGO_CKSUM
    अगर (! legacy_mode)
      ख_माला_दो (_("\
\n\
DIGEST determines the digest algorithm and शेष output format:\n\
  sysv      (equivalent to योग -s)\n\
  bsd       (equivalent to योग -r)\n\
  crc       (equivalent to cksum)\n\
  crc32b    (only available through cksum)\n\
  md5       (equivalent to md5sum)\n\
  sha1      (equivalent to sha1sum)\n\
  sha2      (equivalent to sha{224,256,384,512}योग)\n\
  sha3      (only available through cksum)\n\
  blake2b   (equivalent to b2sum)\n\
  sm3       (only available through cksum)\n\
\n"), मानक_निकास);
#पूर्ण_अगर
#अगर !HASH_ALGO_योग && !HASH_ALGO_CKSUM
      म_लिखो (_("\
\n\
The sums are computed as described in %s.\n"), DIGEST_REFERENCE);
      ख_माला_दो (_("\
When checking, the निवेश should be a former output of this program.\n\
The शेष mode is to print a पंक्ति with: checksum, a space,\n\
a character indicating निवेश mode ('*' क्रम द्विआधारी, ' ' क्रम text\n\
or where द्विआधारी is insignificant), and name क्रम each खाता.\n\
\n\
There is no difference between द्विआधारी mode and text mode on GNU systems.\
\n"), मानक_निकास);
#पूर्ण_अगर
#अगर HASH_ALGO_CKSUM
      ख_माला_दो (_("\
When checking, the निवेश should be a former output of this program,\n\
or equivalent standalone program.\
\n"), मानक_निकास);
#पूर्ण_अगर
      emit_ancillary_info (PROGRAM_NAME);
    }

  निकास (status);
}

#अगर !HASH_ALGO_योग
/* Given a string S, return TRUE if it contains problematic characters
   that need escaping.  Note we escape '\' itself to provide some forward
   compat to introduce escaping of other characters.  */

ATTRIBUTE_PURE
अटल bool
problematic_chars (अक्षर स्थिर *s)
{
  idx_t लंबाई = म_खोज (s, "\\\n\r");
  वापस s[लंबाई] != '\0';
}
#पूर्ण_अगर

/* Given a file name, S of length S_LEN, that is not NUL-terminated,
   modify it in place, performing the equivalent of this sed substitution:
   's/\\n/\n/g;s/\\r/\r/g;s/\\\\/\\/g' i.e., replacing each "\\n" string
   with a newline, each "\\r" string with a carriage return,
   and each "\\\\" with a single backslash, NUL-terminate it and return S.
   If S is not a valid escaped file name, i.e., if it ends with an odd number
   of backslashes or if it contains a backslash followed by anything other
   than "n" or another backslash, return NULL.  */

अटल अक्षर *
filename_unescape (अक्षर *s, idx_t s_len)
{
  अक्षर *dst = s;

  क्रम (idx_t i = 0; i < s_len; i++)
    {
      चयन (s[i])
        {
        हाल '\\':
          अगर (i == s_len - 1)
            {
              /* File name ends with an unescaped backslash: invalid.  */
              वापस शून्य;
            }
          ++i;
          चयन (s[i])
            {
            हाल 'n':
              *dst++ = '\n';
              अवरोध;
            हाल 'r':
              *dst++ = '\r';
              अवरोध;
            हाल '\\':
              *dst++ = '\\';
              अवरोध;
            शेष:
              /* Only '\', 'n' or 'r' may follow a backslash.  */
              वापस शून्य;
            }
          अवरोध;

        हाल '\0':
          /* The file name may not contain a NUL.  */
          वापस शून्य;

        शेष:
          *dst++ = s[i];
          अवरोध;
        }
    }
  अगर (dst < s + s_len)
    *dst = '\0';

  वापस s;
}

/* Return true if S is a LEN-byte NUL-terminated string of hex or base64
   digits and has the expected length.  Otherwise, return false.  */
ATTRIBUTE_PURE
अटल bool
valid_digits (अचिन्हित अक्षर स्थिर *s, idx_t len)
{
#अगर HASH_ALGO_CKSUM
  अगर (len == BASE64_लंबाई (digest_लंबाई >> 3))
    {
      idx_t i;
      क्रम (i = 0; i < len - digest_लंबाई % 3; i++)
        {
          अगर (!isbase64 (*s))
            वापस false;
          ++s;
        }
      क्रम ( ; i < len; i++)
        {
          अगर (*s != '=')
            वापस false;
          ++s;
        }
    }
  अन्यथा
#पूर्ण_अगर
  अगर (len == digest_hex_bytes)
    {
      क्रम (idx_t i = 0; i < digest_hex_bytes; i++)
        {
          अगर (!c_isxdigit (*s))
            वापस false;
          ++s;
        }
    }
  अन्यथा
    वापस false;

  वापस *s == '\0';
}

/* Split the checksum string S (of length S_LEN) from a BSD 'md5' or
   'sha1' command into two parts: a hexadecimal digest, and the file
   name.  S is modified.  Set *D_LEN to the length of the digest string.
   Return true if successful.  */

अटल bool
bsd_split_3 (अक्षर *s, idx_t s_len,
             अचिन्हित अक्षर **digest, idx_t *d_len,
             अक्षर **file_name, bool escaped_filename)
{
  अगर (s_len == 0)
    वापस false;

  /* Find end of filename.  */
  idx_t i = s_len - 1;
  जबतक (i && s[i] != ')')
    i--;

  अगर (s[i] != ')')
    वापस false;

  *file_name = s;

  अगर (escaped_filename && filename_unescape (s, i) == शून्य)
    वापस false;

  s[i++] = '\0';

  जबतक (c_isblank (s[i]))
    i++;

  अगर (s[i] != '=')
    वापस false;

  i++;

  जबतक (c_isblank (s[i]))
    i++;

  *digest = (अचिन्हित अक्षर *) &s[i];

  *d_len = s_len - i;
  वापस valid_digits (*digest, *d_len);
}

#अगर HASH_ALGO_CKSUM
/* Return the corresponding Algorithm for the string S,
   or -1 for no match.  */

अटल सूचक_भेद_प्रकार
algorithm_from_tag (अक्षर *s)
{
  /* Limit check size to this length for perf reasons.  */
  अटल idx_t max_tag_len;
  अगर (! max_tag_len)
    {
      अक्षर स्थिर * स्थिर * tag = algorithm_tags;
      जबतक (*tag)
        {
          idx_t tag_len = म_माप (*tag++);
          max_tag_len = MAX (tag_len, max_tag_len);
        }
    }

  idx_t i = 0;

  /* Find end of tag */
  जबतक (i <= max_tag_len && s[i] && ! c_isblank (s[i])
         && s[i] != '-' && s[i] != '(')
    ++i;

  अगर (i > max_tag_len)
    वापस -1;

  /* Terminate tag, and lookup.  */
  अक्षर sep = s[i];
  s[i] = '\0';
  सूचक_भेद_प्रकार algo = argmatch_exact (s, algorithm_tags);
  s[i] = sep;

  वापस algo;
}
#पूर्ण_अगर

/* Split the string S (of length S_LEN) into three parts:
   a hexadecimal digest, binary flag, and the file name.
   S is modified.  Set *D_LEN to the length of the digest string.
   Return true if successful.  */

अटल bool
split_3 (अक्षर *s, idx_t s_len,
         अचिन्हित अक्षर **digest, idx_t *d_len, पूर्णांक *द्विआधारी, अक्षर **file_name)
{
  bool escaped_filename = false;
  idx_t algo_name_len;

  idx_t i = 0;
  जबतक (c_isblank (s[i]))
    ++i;

  अगर (s[i] == '\\')
    {
      ++i;
      escaped_filename = true;
    }

  /* Check for BSD-style checksum line. */

#अगर HASH_ALGO_CKSUM
  अगर (! algorithm_specified || cksum_algorithm == sha2)
    {
      सूचक_भेद_प्रकार algo_tag = algorithm_from_tag (s + i);
      अगर (! algorithm_specified)
        {
          अगर (algo_tag >= 0)
            {
              अगर (algo_tag <= crc32b)
                वापस false;  /* We don't support checking these formats.  */
              cksum_algorithm = algo_tag;
            }
          अन्यथा
            वापस false;      /* We only support tagged format without -a.  */
        }
      अन्यथा
        {
          अगर (cksum_algorithm == sha2 && (algo_tag == sha2
              || algo_tag == sha224 || algo_tag == sha256
              || algo_tag == sha384 || algo_tag == sha512))
            cksum_algorithm = algo_tag;
        }
    }
#पूर्ण_अगर

  /* Try to parse BSD or OpenSSL tagged format.  I.e.:
     openssl: MD5(f)= d41d8cd98f00b204e9800998ecf8427e
     bsd:     MD5 (f) = d41d8cd98f00b204e9800998ecf8427e  */

  idx_t parse_offset = i;
  algo_name_len = म_माप (DIGEST_TYPE_STRING);
  अगर (STREQ_LEN (s + i, DIGEST_TYPE_STRING, algo_name_len))
    {
      i += algo_name_len;
#अगर HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM

# अगर HASH_ALGO_BLAKE2
      digest_लंबाई = DIGEST_MAX_LEN * 8;
# अन्यथा
      digest_लंबाई = algorithm_bits[cksum_algorithm];
# endif
      अगर (s[i] == '-')  /* length specified. Not base64 */
        {
          ++i;
          intmax_t लंबाई;
          अक्षर *siend;
          अगर (!c_isdigit (s[i])
              || xstrtoimax (s + i, &siend, 0, &लंबाई, शून्य) != LONGINT_OK)
            वापस false;
# अगर HASH_ALGO_CKSUM
          अन्यथा अगर (cksum_algorithm == sha2 || cksum_algorithm == sha3)
            {
              अगर (लंबाई != SHA224_DIGEST_आकार * 8
                  && लंबाई != SHA256_DIGEST_आकार * 8
                  && लंबाई != SHA384_DIGEST_आकार * 8
                  && लंबाई != SHA512_DIGEST_आकार * 8)
                वापस false;
            }
# endif
          अन्यथा अगर (!(0 < लंबाई && लंबाई <= digest_लंबाई && लंबाई % 8 == 0))
            वापस false;

          i = siend - s;
          digest_लंबाई = लंबाई;
        }
      digest_hex_bytes = digest_लंबाई >> 2;
#पूर्ण_अगर
      अगर (s[i] == ' ')
        ++i;
      अगर (s[i] == '(')  /* not base64 */
        {
          ++i;
          *द्विआधारी = 0;
          वापस bsd_split_3 (s + i, s_len - i,
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
  अगर (s_len - i < min_digest_पंक्ति_लंबाई + (s[i] == '\\'))
    वापस false;

  *digest = (अचिन्हित अक्षर *) &s[i];

#अगर HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
  /* Auto determine length.  */
# अगर HASH_ALGO_CKSUM
  अगर (cksum_algorithm == blake2b
      || cksum_algorithm == sha2 || cksum_algorithm == sha3) {
# endif
  अचिन्हित अक्षर स्थिर *hp = *digest;
  digest_hex_bytes = 0;
  क्रम (; c_isxdigit (*hp); ++hp, ++digest_hex_bytes)
    ;
# अगर HASH_ALGO_CKSUM
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
  क्रम (; isubase64 (*hp); ++hp, ++digest_base64_bytes)
    ;
  क्रम (; *hp == '='; ++hp, ++trailing_equals)
    ;
  अगर ((cksum_algorithm == sha2 || cksum_algorithm == sha3)
      && digest_hex_bytes >> 1 != SHA224_DIGEST_आकार
      && digest_hex_bytes >> 1 != SHA256_DIGEST_आकार
      && digest_hex_bytes >> 1 != SHA384_DIGEST_आकार
      && digest_hex_bytes >> 1 != SHA512_DIGEST_आकार)
    {
      अगर (digest_base64_bytes + trailing_equals
          == BASE64_लंबाई (SHA224_DIGEST_आकार))
        digest_hex_bytes = SHA224_DIGEST_आकार * 2;
      अन्यथा अगर (digest_base64_bytes + trailing_equals
               == BASE64_लंबाई (SHA256_DIGEST_आकार))
        digest_hex_bytes = SHA256_DIGEST_आकार * 2;
      अन्यथा अगर (digest_base64_bytes + trailing_equals
               == BASE64_लंबाई (SHA384_DIGEST_आकार))
        digest_hex_bytes = SHA384_DIGEST_आकार * 2;
      अन्यथा अगर (digest_base64_bytes + trailing_equals
               == BASE64_लंबाई (SHA512_DIGEST_आकार))
        digest_hex_bytes = SHA512_DIGEST_आकार * 2;
      अन्यथा
        वापस false;
    }
  अन्यथा अगर (cksum_algorithm == blake2b
           && digest_hex_bytes < digest_base64_bytes)
    {
      क्रम (पूर्णांक j = 8; j <= DIGEST_MAX_LEN * 8; j += 8)
        {
          अगर (BASE64_लंबाई (j / 8) == digest_base64_bytes + trailing_equals
              && j % 3 == trailing_equals)
            {
              digest_hex_bytes = j / 4;
              अवरोध;
            }
        }
    }
# endif
  अगर (digest_hex_bytes < 2 || digest_hex_bytes % 2
      || DIGEST_MAX_LEN * 2 < digest_hex_bytes)
    वापस false;
  digest_लंबाई = digest_hex_bytes * 4;
# अगर HASH_ALGO_CKSUM
  }
# endif
#पूर्ण_अगर

  /* This field must be the hexadecimal or base64 representation
     of the message digest.  */
  जबतक (s[i] && !c_isblank (s[i]))
    i++;

  /* The digest must be followed by at least one whitespace character.  */
  अगर (i == s_len)
    वापस false;

  *d_len = &s[i] - (अक्षर *) *digest;
  s[i++] = '\0';

  अगर (! valid_digits (*digest, *d_len))
    वापस false;

  /* If "bsd reversed" format detected.  */
  अगर ((s_len - i == 1) || (s[i] != ' ' && s[i] != '*'))
    {
      /* Don't allow mixing bsd and standard formats,
         to minimize security issues with attackers
         renaming files with leading spaces.
         This assumes that with bsd format checksums
         that the first file name does not have
         a leading ' ' or '*'.  */
      अगर (bsd_reversed == 0)
        वापस false;
      bsd_reversed = 1;
    }
  अन्यथा अगर (bsd_reversed != 1)
    {
      bsd_reversed = 0;
      *द्विआधारी = (s[i++] == '*');
    }

  /* All characters between the type indicator and end of line are
     significant -- that includes leading and trailing white space.  */
  *file_name = &s[i];

  अगर (escaped_filename)
    वापस filename_unescape (&s[i], s_len - i) != शून्य;

  वापस true;
}

#अगर !HASH_ALGO_योग
/* If ESCAPE is true, then translate each:
   NEWLINE byte to the string, "\\n",
   CARRIAGE RETURN byte to the string, "\\r",
   and each backslash to "\\\\".  */
अटल व्योम
print_filename (अक्षर स्थिर *file, bool escape)
{
  अगर (! escape)
    {
      ख_माला_दो (file, मानक_निकास);
      वापस;
    }

  जबतक (*file)
    {
      चयन (*file)
        {
        हाल '\n':
          ख_माला_दो ("\\n", मानक_निकास);
          अवरोध;

        हाल '\r':
          ख_माला_दो ("\\r", मानक_निकास);
          अवरोध;

        हाल '\\':
          ख_माला_दो ("\\\\", मानक_निकास);
          अवरोध;

        शेष:
          अक्षर_दो (*file);
          अवरोध;
        }
      file++;
    }
}
#पूर्ण_अगर

/* An interface to the function, DIGEST_STREAM.
   Operate on FILENAME (it may be "-").

   *BINARY indicates whether the file is binary.  BINARY < 0 means it
   depends on whether binary mode makes any difference and the file is
   a terminal; in that case, clear *BINARY if the file was treated as
   text because it was a terminal.

   Put the checksum in *BIN_RESULT, which must be properly aligned.
   Put true in *MISSING if the file can't be opened due to ENOENT.
   Return true if successful.  */

अटल bool
digest_file (अक्षर स्थिर *filename, पूर्णांक *द्विआधारी, अचिन्हित अक्षर *bin_परिणाम,
             bool *missing, MAYBE_UNUSED intmax_t *लंबाई)
{
  खाता *fp;
  पूर्णांक err;
  bool is_stdin = streq (filename, "-");

  *missing = false;

  अगर (is_stdin)
    {
      have_read_stdin = true;
      fp = मानक_निवेश;
      अगर (O_द्विआधारी && *द्विआधारी)
        {
          अगर (*द्विआधारी < 0)
            *द्विआधारी = ! isatty (STDIN_FILENO);
          अगर (*द्विआधारी)
            xset_द्विआधारी_mode (STDIN_FILENO, O_द्विआधारी);
        }
    }
  अन्यथा
    {
      fp = ख_खोलो (filename, O_द्विआधारी ? (*द्विआधारी ? "rb" : "rt") : "r");
      अगर (fp == शून्य)
        {
          अगर (ignore_missing && त्रुटि_सं == ENOENT)
            {
              *missing = true;
              वापस true;
            }
          error (0, त्रुटि_सं, "%s", quotef (filename));
          वापस false;
        }
    }

  fadvise (fp, FADVISE_SEQUENTIAL);

#अगर HASH_ALGO_CKSUM
  अगर (cksum_algorithm == blake2b
      || cksum_algorithm == sha2 || cksum_algorithm == sha3)
    *लंबाई = digest_लंबाई >> 3;
  err = DIGEST_STREAM (fp, bin_परिणाम, लंबाई);
#या_अगर HASH_ALGO_योग
  err = DIGEST_STREAM (fp, bin_परिणाम, लंबाई);
#या_अगर HASH_ALGO_BLAKE2
  err = DIGEST_STREAM (fp, bin_परिणाम, digest_लंबाई >> 3);
#अन्यथा
  err = DIGEST_STREAM (fp, bin_परिणाम);
#पूर्ण_अगर
  err = err ? त्रुटि_सं : 0;
  अगर (is_stdin)
    साफ_त्रुटि (fp);
  अन्यथा अगर (ख_बंद (fp) != 0 && !err)
    err = त्रुटि_सं;

  अगर (err)
    {
      error (0, err, "%s", quotef (filename));
      वापस false;
    }

  वापस true;
}

#अगर !HASH_ALGO_योग
अटल व्योम
output_file (अक्षर स्थिर *file, पूर्णांक द्विआधारी_file, व्योम स्थिर *digest,
             MAYBE_UNUSED bool raw, bool tagged, अचिन्हित अक्षर delim,
             MAYBE_UNUSED bool args, MAYBE_UNUSED intmax_t लंबाई)
{
# अगर HASH_ALGO_CKSUM
  अगर (raw)
    {
      ख_डालो (digest, 1, digest_लंबाई >> 3, मानक_निकास);
      वापस;
    }
# endif

  अचिन्हित अक्षर स्थिर *bin_बफर = digest;

  /* Output a leading backslash if the file name contains problematic chars.  */
  bool needs_escape = delim == '\n' && problematic_chars (file);

  अगर (needs_escape)
    अक्षर_दो ('\\');

  अगर (tagged)
    {
# अगर HASH_ALGO_CKSUM
      अगर (cksum_algorithm == sha2)
        म_लिखो ("SHA%u", (अचिन्हित पूर्णांक) digest_लंबाई);
      अन्यथा
# endif
      ख_माला_दो (DIGEST_TYPE_STRING, मानक_निकास);
# अगर HASH_ALGO_BLAKE2
      अगर (digest_लंबाई < DIGEST_MAX_LEN * 8)
        म_लिखो ("-%u", (अचिन्हित पूर्णांक) digest_लंबाई);
# elif HASH_ALGO_CKSUM
      अगर (cksum_algorithm == sha3)
        म_लिखो ("-%u", (अचिन्हित पूर्णांक) digest_लंबाई);
      अगर (cksum_algorithm == blake2b)
        {
          अगर (digest_लंबाई < DIGEST_MAX_LEN * 8)
            म_लिखो ("-%u", (अचिन्हित पूर्णांक) digest_लंबाई);
        }
# endif
      ख_माला_दो (" (", मानक_निकास);
      print_filename (file, needs_escape);
      ख_माला_दो (") = ", मानक_निकास);
    }

# अगर HASH_ALGO_CKSUM
  अगर (base64_digest)
    {
      अक्षर b64[BASE64_लंबाई (DIGEST_BIN_BYTES) + 1];
      base64_encode ((अक्षर स्थिर *) bin_बफर, digest_लंबाई >> 3,
                     b64, माप b64);
      ख_माला_दो (b64, मानक_निकास);
    }
  अन्यथा
# endif
    {
      क्रम (idx_t i = 0; i < (digest_hex_bytes >> 1); ++i)
        म_लिखो ("%02x", bin_बफर[i]);
    }

  अगर (!tagged)
    {
      अक्षर_दो (' ');
      अक्षर_दो (द्विआधारी_file ? '*' : ' ');
      print_filename (file, needs_escape);
    }

  अक्षर_दो (delim);
}
#पूर्ण_अगर

#अगर HASH_ALGO_CKSUM
/* Return true if B64_DIGEST is the same as the base64 digest of the
   DIGEST_LENGTH/8 bytes at BIN_BUFFER.  */
अटल bool
b64_equal (अचिन्हित अक्षर स्थिर *b64_digest, अचिन्हित अक्षर स्थिर *bin_बफर)
{
  idx_t b64_n_bytes = BASE64_लंबाई (digest_लंबाई >> 3);
  अक्षर b64[BASE64_लंबाई (DIGEST_BIN_BYTES) + 1];
  base64_encode ((अक्षर स्थिर *) bin_बफर, digest_लंबाई >> 3,
                 b64, माप b64);
  वापस memeq (b64_digest, b64, b64_n_bytes + 1);
}
#पूर्ण_अगर

/* Return true if HEX_DIGEST is the same as the hex-encoded digest of the
   DIGEST_LENGTH/8 bytes at BIN_BUFFER.  */
अटल bool
hex_equal (अचिन्हित अक्षर स्थिर *hex_digest, अचिन्हित अक्षर स्थिर *bin_बफर)
{
  अटल स्थिर अक्षर bin2hex[] = { '0', '1', '2', '3',
                                  '4', '5', '6', '7',
                                  '8', '9', 'a', 'b',
                                  'c', 'd', 'e', 'f' };
  idx_t digest_bin_bytes = digest_hex_bytes >> 1;

  /* Compare generated binary number with text representation
     in check file.  Ignore case of hex digits.  */
  idx_t cnt;
  क्रम (cnt = 0; cnt < digest_bin_bytes; ++cnt)
    {
      अगर (c_tolower (hex_digest[2 * cnt])
          != bin2hex[bin_बफर[cnt] >> 4]
          || (c_tolower (hex_digest[2 * cnt + 1])
              != (bin2hex[bin_बफर[cnt] & 0xf])))
        अवरोध;
    }
  वापस cnt == digest_bin_bytes;
}

अटल bool
digest_check (अक्षर स्थिर *checkfile_name)
{
  खाता *checkfile_stream;
  intmax_t n_misformatted_पंक्तियाँ = 0;
  intmax_t n_mismatched_checksums = 0;
  intmax_t n_open_or_read_failures = 0;
  bool properly_formatted_पंक्तियाँ = false;
  bool matched_checksums = false;
  अचिन्हित अक्षर bin_बफर_unaligned[DIGEST_BIN_BYTES + DIGEST_ALIGN];
  /* Make sure bin_buffer is properly aligned. */
  अचिन्हित अक्षर *bin_बफर = ptr_align (bin_बफर_unaligned, DIGEST_ALIGN);
  intmax_t पंक्ति_संख्या;
  अक्षर *पंक्ति;
  माप_प्रकार पंक्ति_chars_allocated;
  bool is_stdin = streq (checkfile_name, "-");

  अगर (is_stdin)
    {
      have_read_stdin = true;
      checkfile_name = _("standard input");
      checkfile_stream = मानक_निवेश;
    }
  अन्यथा
    {
      checkfile_stream = ख_खोलो (checkfile_name, "r");
      अगर (checkfile_stream == शून्य)
        {
          error (0, त्रुटि_सं, "%s", quotef (checkfile_name));
          वापस false;
        }
    }

  पंक्ति_संख्या = 0;
  पंक्ति = शून्य;
  पंक्ति_chars_allocated = 0;
  करो
    {
      अक्षर *filename;
      पूर्णांक द्विआधारी;
      अचिन्हित अक्षर *digest;
      ssize_t पंक्ति_लंबाई;

      ++पंक्ति_संख्या;
      अगर (पंक्ति_संख्या == 0)
        error (निकास_त्रुटि, 0, _("%s: too many checksum lines"),
               quotef (checkfile_name));

      पंक्ति_लंबाई = getline (&पंक्ति, &पंक्ति_chars_allocated, checkfile_stream);
      अगर (पंक्ति_लंबाई <= 0)
        अवरोध;

      /* Ignore comment lines, which begin with a '#' character.  */
      अगर (पंक्ति[0] == '#')
        जारी;

      /* Remove any trailing newline.  */
      पंक्ति_लंबाई -= पंक्ति[पंक्ति_लंबाई - 1] == '\n';
      /* Remove any trailing carriage return.  */
      पंक्ति_लंबाई -= पंक्ति[पंक्ति_लंबाई - (0 < पंक्ति_लंबाई)] == '\r';

      /* Ignore empty lines.  */
      अगर (पंक्ति_लंबाई == 0)
        जारी;

      पंक्ति[पंक्ति_लंबाई] = '\0';

      idx_t d_len;
      अगर (! (split_3 (पंक्ति, पंक्ति_लंबाई, &digest, &d_len, &द्विआधारी, &filename)
             && ! (is_stdin && streq (filename, "-"))))
        {
          ++n_misformatted_पंक्तियाँ;

          अगर (warn)
            {
              error (0, 0,
                     _("%s: %jd"
                       ": improperly formatted %s checksum line"),
                     quotef (checkfile_name), पंक्ति_संख्या,
                     DIGEST_TYPE_STRING);
            }
        }
      अन्यथा
        {
          bool ok;
          bool missing;

          properly_formatted_पंक्तियाँ = true;

          intmax_t लंबाई;
          ok = digest_file (filename, &द्विआधारी, bin_बफर, &missing, &लंबाई);

          अगर (!ok)
            {
              ++n_open_or_read_failures;
              अगर (!status_only)
                म_लिखो ("%s: %s\n", quotef (filename),_("FAILED open or read"));
            }
          अन्यथा अगर (ignore_missing && missing)
            {
              /* Ignore missing files with --ignore-missing.  */
              ;
            }
          अन्यथा
            {
              bool match = false;
#अगर HASH_ALGO_CKSUM
              अगर (d_len == BASE64_लंबाई (digest_लंबाई >> 3))
                match = b64_equal (digest, bin_बफर);
              अन्यथा
#पूर्ण_अगर
                अगर (d_len == digest_hex_bytes)
                  match = hex_equal (digest, bin_बफर);

              अगर (match)
                matched_checksums = true;
              अन्यथा
                ++n_mismatched_checksums;

              अगर (!status_only)
                {
                  अगर (! match || ! quiet)
                    ख_माला_दो (quotef (filename), मानक_निकास);

                  अगर (! match)
                    म_लिखो (": %s\n", _("FAILED"));
                  अन्यथा अगर (!quiet)
                    म_लिखो (": %s\n", _("OK"));
                }
            }

          अगर (ख_त्रुटि (मानक_निकास))
            write_error ();
        }
    }
  जबतक (!ख_पूर्ण (checkfile_stream) && !ख_त्रुटि (checkfile_stream));

  मुक्त (पंक्ति);

  पूर्णांक err = ख_त्रुटि (checkfile_stream) ? 0 : -1;
  अगर (is_stdin)
    साफ_त्रुटि (checkfile_stream);
  अन्यथा अगर (ख_बंद (checkfile_stream) != 0 && err < 0)
    err = त्रुटि_सं;

  अगर (0 <= err)
    {
      error (0, err, err ? "%s" : _("%s: read error"),
             quotef (checkfile_name));
      वापस false;
    }

  अगर (! properly_formatted_पंक्तियाँ)
    {
      /* Warn if no tests are found.  */
      error (0, 0, _("%s: no properly formatted checksum lines found"),
             quotef (checkfile_name));
    }
  अन्यथा
    {
      अगर (!status_only)
        {
          अगर (n_misformatted_पंक्तियाँ != 0)
            error (0, 0,
                   (ngettext
                    ("WARNING: %jd line is improperly formatted",
                     "WARNING: %jd lines are improperly formatted",
                     select_plural (n_misformatted_पंक्तियाँ))),
                   n_misformatted_पंक्तियाँ);

          अगर (n_open_or_read_failures != 0)
            error (0, 0,
                   (ngettext
                    ("WARNING: %jd listed file could not be read",
                     "WARNING: %jd listed files could not be read",
                     select_plural (n_open_or_read_failures))),
                   n_open_or_read_failures);

          अगर (n_mismatched_checksums != 0)
            error (0, 0,
                   (ngettext
                    ("WARNING: %jd computed checksum did NOT match",
                     "WARNING: %jd computed checksums did NOT match",
                     select_plural (n_mismatched_checksums))),
                   n_mismatched_checksums);

          अगर (ignore_missing && ! matched_checksums)
            error (0, 0, _("%s: no file was verified"),
                   quotef (checkfile_name));
        }
    }

  वापस (properly_formatted_पंक्तियाँ
          && matched_checksums
          && n_mismatched_checksums == 0
          && n_open_or_read_failures == 0
          && (!strict || n_misformatted_पंक्तियाँ == 0));
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  अचिन्हित अक्षर bin_बफर_unaligned[DIGEST_BIN_BYTES + DIGEST_ALIGN];
  /* Make sure bin_buffer is properly aligned. */
  अचिन्हित अक्षर *bin_बफर = ptr_align (bin_बफर_unaligned, DIGEST_ALIGN);
  bool do_check = false;
  पूर्णांक opt;
  bool ok = true;
  पूर्णांक द्विआधारी = -1;
  पूर्णांक prefix_tag = -1;
  काष्ठा option स्थिर *long_opts = long_options;

#अगर HASH_ALGO_CKSUM
  अगर (cksum_algorithm != crc)
    {
      legacy_mode = true;
      prefix_tag = 0;
      algorithm_specified = true;
      long_opts = legacy_long_options;
    }
#पूर्ण_अगर

  /* Setting values of global variables.  */
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  /* Line buffer stdout to ensure lines are written atomically and immediately
     so that processes running in parallel do not intersperse their output.  */
  रखो_भबफ (मानक_निकास, शून्य, _IOLBF, 0);

#अगर HASH_ALGO_योग
  अक्षर स्थिर *short_opts = "rs";
#या_अगर HASH_ALGO_CKSUM
  अक्षर स्थिर *short_opts = legacy_mode ? "bctwz" : "a:l:bctwz";
  अक्षर स्थिर *digest_लंबाई_str = "";
#या_अगर HASH_ALGO_BLAKE2
  अक्षर स्थिर *short_opts = "l:bctwz";
  अक्षर स्थिर *digest_लंबाई_str = "";
#अन्यथा
  अक्षर स्थिर *short_opts = "bctwz";
#पूर्ण_अगर

  जबतक ((opt = getopt_long (argc, argv, short_opts, long_opts, शून्य))
         != -1)
    चयन (opt)
      {
#अगर HASH_ALGO_CKSUM
      हाल 'a':
        cksum_algorithm = XARGMATCH_EXACT ("--algorithm", optarg,
                                           algorithm_args, algorithm_types);
        algorithm_specified = true;
        अवरोध;

      हाल DEBUG_PROGRAM_OPTION:
        cksum_debug = true;
        अवरोध;
#पूर्ण_अगर
#अगर HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
      हाल 'l':
        digest_लंबाई = xnumtoimax (optarg, 10, 0, IDX_MAX, "",
                                    _("invalid length"), 0,
                                    XTOINT_MAX_QUIET);
        digest_लंबाई_str = optarg;
        अवरोध;
#पूर्ण_अगर
#अगर !HASH_ALGO_योग
      हाल 'c':
        do_check = true;
        अवरोध;
      हाल STATUS_OPTION:
        status_only = true;
        warn = false;
        quiet = false;
        अवरोध;
      हाल 'b':
        द्विआधारी = 1;
        अवरोध;
      हाल 't':
        द्विआधारी = 0;
        अवरोध;
      हाल 'w':
        status_only = false;
        warn = true;
        quiet = false;
        अवरोध;
      हाल IGNORE_MISSING_OPTION:
        ignore_missing = true;
        अवरोध;
      हाल QUIET_OPTION:
        status_only = false;
        warn = false;
        quiet = true;
        अवरोध;
      हाल STRICT_OPTION:
        strict = true;
        अवरोध;
# अगर HASH_ALGO_CKSUM
      हाल BASE64_OPTION:
        base64_digest = true;
        अवरोध;
      हाल RAW_OPTION:
        raw_digest = true;
        अवरोध;
      हाल UNTAG_OPTION:
        prefix_tag = 0;
        अवरोध;
# endif
      हाल TAG_OPTION:
        prefix_tag = 1;
        अवरोध;
      हाल 'z':
        digest_delim = '\0';
        अवरोध;
#पूर्ण_अगर
#अगर HASH_ALGO_योग
      हाल 'r':		/* For SysV compatibility. */
        योग_algorithm = bsd;
        अवरोध;

      हाल 's':
        योग_algorithm = sysv;
        अवरोध;
#पूर्ण_अगर
      case_GETOPT_HELP_CHAR;
      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
      शेष:
        usage (निकास_त्रुटि);
      }

  min_digest_पंक्ति_लंबाई = MIN_DIGEST_पंक्ति_लंबाई;
#अगर HASH_ALGO_BLAKE2 || HASH_ALGO_CKSUM
# अगर HASH_ALGO_CKSUM
  अगर (digest_लंबाई && (cksum_algorithm != blake2b
                        && cksum_algorithm != sha2
                        && cksum_algorithm != sha3))
    error (निकास_त्रुटि, 0,
           _("--length is only supported with --algorithm "
             "blake2b, sha2, or sha3"));
  अगर (cksum_algorithm == sha2 || cksum_algorithm == sha3)
    {
      /* Do not require --length with --check.  */
      अगर (digest_लंबाई == 0 && *digest_लंबाई_str == '\0' && ! do_check)
        error (निकास_त्रुटि, 0, _("--algorithm=%s requires specifying "
                                  "--length 224, 256, 384, or 512"),
               algorithm_args[cksum_algorithm]);
      /* If --check and --length are used we verify the digest length.  */
      अगर ((! do_check || *digest_लंबाई_str != '\0')
          && digest_लंबाई != SHA224_DIGEST_आकार * 8
          && digest_लंबाई != SHA256_DIGEST_आकार * 8
          && digest_लंबाई != SHA384_DIGEST_आकार * 8
          && digest_लंबाई != SHA512_DIGEST_आकार * 8)
        {
          error (0, 0, _("invalid length: %s"), quote (digest_लंबाई_str));
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
      अगर (digest_लंबाई > DIGEST_MAX_LEN * 8)
        {
          error (0, 0, _("invalid length: %s"), quote (digest_लंबाई_str));
          error (निकास_त्रुटि, 0,
                 _("maximum digest length for %s is %d bits"),
                 quote (DIGEST_TYPE_STRING),
                 DIGEST_MAX_LEN * 8);
        }
      अगर (digest_लंबाई % 8 != 0)
        {
          error (0, 0, _("invalid length: %s"), quote (digest_लंबाई_str));
          error (निकास_त्रुटि, 0, _("length is not a multiple of 8"));
        }
    }
  अगर (digest_लंबाई == 0)
    {
# अगर HASH_ALGO_BLAKE2
      digest_लंबाई = DIGEST_MAX_LEN * 8;
# अन्यथा
      digest_लंबाई = algorithm_bits[cksum_algorithm];
# endif
    }
  digest_hex_bytes = digest_लंबाई >> 2;
#अन्यथा
  digest_hex_bytes = DIGEST_HEX_BYTES;
#पूर्ण_अगर

#अगर HASH_ALGO_CKSUM
  चयन (+cksum_algorithm)
    {
    हाल bsd:
    हाल sysv:
    हाल crc:
    हाल crc32b:
        अगर (do_check && algorithm_specified)
          error (निकास_त्रुटि, 0,
                 _("--check is not supported with "
                   "--algorithm={bsd,sysv,crc,crc32b}"));
        अवरोध;
    }

  अगर (base64_digest && raw_digest)
   {
     error (0, 0, _("--base64 and --raw are mutually exclusive"));
     usage (निकास_त्रुटि);
   }
#पूर्ण_अगर

  अगर (digest_delim != '\n' && do_check)
    {
      error (0, 0, _("the --zero option is not supported when "
                     "verifying checksums"));
      usage (निकास_त्रुटि);
    }
  अगर (1 <= prefix_tag && do_check)
    {
      /* Note we allow --untagged with --check to more
         seamlessly support --untagged in an emulation wrapper.  */
      error (0, 0, _("the --tag option is meaningless when "
                     "verifying checksums"));
      usage (निकास_त्रुटि);
    }

  अगर (0 <= द्विआधारी && do_check)
    {
      error (0, 0, _("the --binary and --text options are meaningless when "
                     "verifying checksums"));
      usage (निकास_त्रुटि);
    }

  अगर (ignore_missing && !do_check)
    {
      error (0, 0,
             _("the --ignore-missing option is meaningful only when "
               "verifying checksums"));
      usage (निकास_त्रुटि);
    }

  अगर (status_only && !do_check)
    {
      error (0, 0,
       _("the --status option is meaningful only when verifying checksums"));
      usage (निकास_त्रुटि);
    }

  अगर (warn && !do_check)
    {
      error (0, 0,
       _("the --warn option is meaningful only when verifying checksums"));
      usage (निकास_त्रुटि);
    }

  अगर (quiet && !do_check)
    {
      error (0, 0,
       _("the --quiet option is meaningful only when verifying checksums"));
      usage (निकास_त्रुटि);
    }

  अगर (strict & !do_check)
   {
     error (0, 0,
        _("the --strict option is meaningful only when verifying checksums"));
     usage (निकास_त्रुटि);
   }

  अगर (prefix_tag == -1)
    prefix_tag = HASH_ALGO_CKSUM;

  अगर (prefix_tag && !द्विआधारी)
   {
     /* This could be supported in a backwards compatible way
        by prefixing the output line with a space in text mode.
        However that's invasive enough that it was agreed to
        not support this mode with --tag, as --text use cases
        are adequately supported by the default output format.  */
#अगर !HASH_ALGO_CKSUM
     error (0, 0, _("--tag does not support --text mode"));
#अन्यथा
     error (0, 0, _("--text mode is only supported with --untagged"));
#पूर्ण_अगर
     usage (निकास_त्रुटि);
   }

  अगर (!O_द्विआधारी && द्विआधारी < 0)
    द्विआधारी = 0;
  अन्यथा अगर (prefix_tag)
    द्विआधारी = 1;

  अक्षर **operand_lim = argv + argc;
  अगर (optind == argc)
    *operand_lim++ = bad_cast ("-");
  अन्यथा अगर (1 < argc - optind && raw_digest)
    error (निकास_त्रुटि, 0,
           _("the --raw option is not supported with multiple files"));

  क्रम (अक्षर **operandp = argv + optind; operandp < operand_lim; operandp++)
    {
      अक्षर *file = *operandp;
      अगर (do_check)
        ok &= digest_check (file);
      अन्यथा
        {
          पूर्णांक द्विआधारी_file = द्विआधारी;
          bool missing;
          intmax_t लंबाई;

          अगर (! digest_file (file, &द्विआधारी_file, bin_बफर, &missing, &लंबाई))
            ok = false;
          अन्यथा
            {
              DIGEST_OUT (file, द्विआधारी_file, bin_बफर, raw_digest, prefix_tag,
                          digest_delim, optind != argc, लंबाई);
              अगर (ख_त्रुटि (मानक_निकास))
                write_error ();
            }
        }
    }

  अगर (have_read_stdin && ख_बंद (मानक_निवेश) == खातापूर्ण)
    error (निकास_त्रुटि, त्रुटि_सं, _("standard input"));

  वापस ok ? निकास_सफल : निकास_त्रुटि;
}
