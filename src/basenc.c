/* Base64, base32, and similar encoding/decoding strings or files.
   Copyright (C) 2004-2026 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>. */

/* Written by Simon Josefsson <simon@josefsson.org>.  */

#समावेश <config.h>

#समावेश <stdio.h>
#समावेश <getopt.h>
#समावेश <sys/types.h>
#समावेश <gmp.h>

#समावेश "system.h"
#समावेश "assure.h"
#समावेश "c-ctype.h"
#समावेश "fadvise.h"
#समावेश "quote.h"
#समावेश "xstrtol.h"
#समावेश "xbinary-io.h"

#अगर BASE_TYPE == 42
# define AUTHORS \
  proper_name ("Simon Josefsson"), \
  proper_name ("Assaf Gordon")
#अन्यथा
# define AUTHORS proper_name ("Simon Josefsson")
#पूर्ण_अगर

#अगर BASE_TYPE == 32
# include "base32.h"
# define PROGRAM_NAME "base32"
#या_अगर BASE_TYPE == 64
# include "base64.h"
# define PROGRAM_NAME "base64"
#या_अगर BASE_TYPE == 42
# include "base32.h"
# include "base64.h"
# include "assure.h"
# define PROGRAM_NAME "basenc"
#अन्यथा
# error missing/invalid BASE_TYPE definition
#पूर्ण_अगर



#अगर BASE_TYPE == 42
क्रमागत
{
  BASE64_OPTION = अक्षर_उच्च + 1,
  BASE64URL_OPTION,
  BASE58_OPTION,
  BASE32_OPTION,
  BASE32HEX_OPTION,
  BASE16_OPTION,
  BASE2MSBF_OPTION,
  BASE2LSBF_OPTION,
  Z85_OPTION
};
#पूर्ण_अगर

अटल काष्ठा option स्थिर long_options[] =
{
  {"decode", no_argument, शून्य, 'd'},
  {"wrap", required_argument, शून्य, 'w'},
  {"ignore-garbage", no_argument, शून्य, 'i'},
#अगर BASE_TYPE == 42
  {"base64",    no_argument, शून्य, BASE64_OPTION},
  {"base64url", no_argument, शून्य, BASE64URL_OPTION},
  {"base58",    no_argument, शून्य, BASE58_OPTION},
  {"base32",    no_argument, शून्य, BASE32_OPTION},
  {"base32hex", no_argument, शून्य, BASE32HEX_OPTION},
  {"base16",    no_argument, शून्य, BASE16_OPTION},
  {"base2msbf", no_argument, शून्य, BASE2MSBF_OPTION},
  {"base2lsbf", no_argument, शून्य, BASE2LSBF_OPTION},
  {"z85",       no_argument, शून्य, Z85_OPTION},
#पूर्ण_अगर
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {शून्य, 0, शून्य, 0}
};

व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s [OPTION]... [खाता]\n\
"), program_name);

#अगर BASE_TYPE == 42
      ख_माला_दो (_("\
basenc encode or decode खाता, or standard निवेश, to standard output.\n\
"), मानक_निकास);
#अन्यथा
      म_लिखो (_("\
Base%d encode or decode खाता, or standard निवेश, to standard output.\n\
"), BASE_TYPE);
#पूर्ण_अगर

      emit_stdin_note ();
      emit_mandatory_arg_note ();
#अगर BASE_TYPE == 42
      oputs (_("\
      --base64\n\
         same as base64(1) program (RFC4648 section 4)\n\
"));
      oputs (_("\
      --base64url\n\
         file- and url-safe base64 (RFC4648 section 5)\n\
"));
      oputs (_("\
      --base58\n\
         visually unambiguous base58 encoding\n\
"));
      oputs (_("\
      --base32\n\
         same as base32(1) program (RFC4648 section 6)\n\
"));
      oputs (_("\
      --base32hex\n\
         extended hex alphabet base32 (RFC4648 section 7)\n\
"));
      oputs (_("\
      --base16\n\
         hex encoding (RFC4648 section 8)\n\
"));
      oputs (_("\
      --base2msbf\n\
         bit string with most significant bit (msb) first\n\
"));
      oputs (_("\
      --base2lsbf\n\
         bit string with least significant bit (lsb) first\n\
"));
#पूर्ण_अगर
      oputs (_("\
  -d, --decode\n\
         decode तथ्य\n\
"));
      oputs (_("\
  -i, --ignore-garbage\n\
         when decoding, ignore non-alphabet characters\n\
"));
      oputs (_("\
  -w, --wrap=COLS\n\
         wrap encoded पंक्तियाँ after COLS character (शेष 76).\n\
         Use 0 to disable पंक्ति wrapping\n\
"));
#अगर BASE_TYPE == 42
      oputs (_("\
      --z85\n\
         ascii85-like encoding (ZeroMQ spec:32/Z85);\n\
         when encoding, निवेश लंबाई must be a multiple of 4;\n\
         when decoding, निवेश लंबाई must be a multiple of 5\n\
"));
#पूर्ण_अगर
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
#अगर BASE_TYPE == 42
      ख_माला_दो (_("\
\n\
When decoding, the निवेश may contain newlines in addition to the bytes of\n\
the formal alphabet.  Use --ignore-garbage to attempt to recover\n\
from any other non-alphabet bytes in the encoded stream.\n\
"), मानक_निकास);
#अन्यथा
      म_लिखो (_("\
\n\
The तथ्य are encoded as described क्रम the %s alphabet in RFC 4648.\n\
When decoding, the निवेश may contain newlines in addition to the bytes of\n\
the formal %s alphabet.  Use --ignore-garbage to attempt to recover\n\
from any other non-alphabet bytes in the encoded stream.\n"),
              PROGRAM_NAME, PROGRAM_NAME);
#पूर्ण_अगर
      emit_ancillary_info (PROGRAM_NAME);
    }

  निकास (status);
}

#अगर BASE_TYPE != 64
अटल पूर्णांक
base32_required_padding (पूर्णांक len)
{
  पूर्णांक partial = len % 8;
  वापस partial ? 8 - partial : 0;
}
#पूर्ण_अगर

#अगर BASE_TYPE != 32
अटल पूर्णांक
base64_required_padding (पूर्णांक len)
{
  पूर्णांक partial = len % 4;
  वापस partial ? 4 - partial : 0;
}
#पूर्ण_अगर

#अगर BASE_TYPE == 42
अटल पूर्णांक
no_required_padding (MAYBE_UNUSED पूर्णांक len)
{
  वापस 0;
}
#पूर्ण_अगर

#घोषणा ENC_BLOCKSIZE (1024 * 3 * 10)

#अगर BASE_TYPE == 32
# define BASE_लंबाई BASE32_लंबाई
# define REQUIRED_PADDING base32_required_padding
/* Note that increasing this may decrease performance if --ignore-garbage
   is used, because of the memmove operation below.  */
# define DEC_BLOCKSIZE (1024 * 5)

/* Ensure that BLOCKSIZE is a multiple of 5 and 8.  */
static_assert (ENC_BLOCKSIZE % 40 == 0); /* Padding chars only on last block. */
static_assert (DEC_BLOCKSIZE % 40 == 0); /* Complete encoded blocks are used. */

# define base_encode base32_encode
# define base_decode_context base32_decode_context
# define base_decode_ctx_init base32_decode_ctx_init
# define base_decode_ctx base32_decode_ctx
# define base_decode_ctx_finalize decode_ctx_finalize
# define isubase isubase32
#या_अगर BASE_TYPE == 64
# define BASE_लंबाई BASE64_लंबाई
# define REQUIRED_PADDING base64_required_padding
/* Note that increasing this may decrease performance if --ignore-garbage
   is used, because of the memmove operation below.  */
# define DEC_BLOCKSIZE (1024 * 3)

/* Ensure that BLOCKSIZE is a multiple of 3 and 4.  */
static_assert (ENC_BLOCKSIZE % 12 == 0); /* Padding chars only on last block. */
static_assert (DEC_BLOCKSIZE % 12 == 0); /* Complete encoded blocks are used. */

# define base_encode base64_encode
# define base_decode_context base64_decode_context
# define base_decode_ctx_init base64_decode_ctx_init
# define base_decode_ctx base64_decode_ctx
# define base_decode_ctx_finalize decode_ctx_finalize
# define isubase isubase64
#या_अगर BASE_TYPE == 42


# define BASE_लंबाई base_लंबाई
# define REQUIRED_PADDING required_padding

/* Note that increasing this may decrease performance if --ignore-garbage
   is used, because of the memmove operation below.  */
# define DEC_BLOCKSIZE (4200)
static_assert (DEC_BLOCKSIZE % 40 == 0); /* complete encoded blocks for base32*/
static_assert (DEC_BLOCKSIZE % 12 == 0); /* complete encoded blocks for base64*/

अटल idx_t (*base_लंबाई) (idx_t len);
अटल पूर्णांक (*required_padding) (पूर्णांक i);
अटल bool (*isubase) (अचिन्हित अक्षर ch);
अटल व्योम (*base_encode) (अक्षर स्थिर *restrict in, idx_t inlen,
                            अक्षर *restrict out, idx_t outlen);

काष्ठा base16_decode_context
{
  /* Either a 4-bit nibble, or negative if we have no nibble.  */
  चिन्हित अक्षर nibble;
};

काष्ठा z85_decode_context
{
  पूर्णांक i;
  अचिन्हित अक्षर octets[5];
};

काष्ठा base58_context
{
  idx_t आकार;
  idx_t capacity;
  अचिन्हित अक्षर *buf COUNTED_BY (capacity);
};

काष्ठा base2_decode_context
{
  अचिन्हित अक्षर octet;
  पूर्णांक bit_pos;
};

काष्ठा base_decode_context
{
  जोड़ {
    काष्ठा base64_decode_context base64;
    काष्ठा base32_decode_context base32;
    काष्ठा base16_decode_context base16;
    काष्ठा base2_decode_context base2;
    काष्ठा z85_decode_context z85;
    काष्ठा base58_context base58;
  } ctx;
  अक्षर *inbuf;
  idx_t bufsize;
};
अटल व्योम (*base_decode_ctx_init) (काष्ठा base_decode_context *ctx);
अटल bool (*base_decode_ctx) (काष्ठा base_decode_context *ctx,
                                अक्षर स्थिर *restrict in, idx_t inlen,
                                अक्षर *restrict out, idx_t *outlen);
अटल bool (*base_decode_ctx_finalize) (काष्ठा base_decode_context *ctx,
                                         अक्षर *restrict *out, idx_t *outlen);

काष्ठा base_encode_context
{
  जोड़ {
    काष्ठा base58_context base58;
  } ctx;
};

अटल व्योम (*base_encode_ctx_init) (काष्ठा base_encode_context *ctx);
अटल bool (*base_encode_ctx) (काष्ठा base_encode_context *ctx,
                                अक्षर स्थिर *restrict in, idx_t inlen,
                                अक्षर *restrict out, idx_t *outlen);
अटल bool (*base_encode_ctx_finalize) (काष्ठा base_encode_context *ctx,
                                         अक्षर *restrict *out, idx_t *outlen);

अटल bool
no_padding (MAYBE_UNUSED काष्ठा base_decode_context *ctx)
{
  वापस false;
}

अटल पूर्णांक
no_pending_लंबाई (MAYBE_UNUSED काष्ठा base_decode_context *ctx)
{
  वापस 0;
}
#पूर्ण_अगर

#अगर BASE_TYPE == 42
अटल bool (*has_padding) (काष्ठा base_decode_context *ctx);
अटल पूर्णांक (*get_pending_लंबाई) (काष्ठा base_decode_context *ctx);

अटल bool
base64_ctx_has_padding (काष्ठा base_decode_context *ctx)
{
  वापस ctx->ctx.base64.i && ctx->ctx.base64.buf[ctx->ctx.base64.i - 1] == '=';
}

अटल bool
base32_ctx_has_padding (काष्ठा base_decode_context *ctx)
{
  वापस ctx->ctx.base32.i && ctx->ctx.base32.buf[ctx->ctx.base32.i - 1] == '=';
}

अटल पूर्णांक
base64_ctx_get_pending_लंबाई (काष्ठा base_decode_context *ctx)
{
  वापस ctx->ctx.base64.i;
}

अटल पूर्णांक
base32_ctx_get_pending_लंबाई (काष्ठा base_decode_context *ctx)
{
  वापस ctx->ctx.base32.i;
}

अटल पूर्णांक
base16_ctx_get_pending_लंबाई (MAYBE_UNUSED काष्ठा base_decode_context *ctx)
{
  वापस 1; /* Always check nibble state.  */
}

अटल पूर्णांक
z85_ctx_get_pending_लंबाई (काष्ठा base_decode_context *ctx)
{
  वापस ctx->ctx.z85.i;
}

अटल पूर्णांक
base2_ctx_get_pending_लंबाई (काष्ठा base_decode_context *ctx)
{
  वापस ctx->ctx.base2.bit_pos;
}
#अन्यथा
अटल bool
has_padding (काष्ठा base_decode_context *ctx)
{
  वापस ctx->i && ctx->buf[ctx->i - 1] == '=';
}

अटल पूर्णांक
get_pending_लंबाई (काष्ठा base_decode_context *ctx)
{
  वापस ctx->i;
}
#पूर्ण_अगर


/* Process any pending data in CTX, while auto padding if appropriate.
   Return TRUE on success, FALSE on failure.  */

अटल bool
decode_ctx_finalize (काष्ठा base_decode_context *ctx,
                     अक्षर *restrict *out, idx_t *outlen)
{
  अगर (get_pending_लंबाई (ctx) == 0)
    {
      *outlen = 0;
      वापस true;
    }

  /* Auto-pad input and flush the context */
  अक्षर padbuf[8] ATTRIBUTE_NONSTRING = "========";
  idx_t pending_len = get_pending_लंबाई (ctx);
  idx_t auto_padding = REQUIRED_PADDING (pending_len);
  idx_t n = *outlen;
  bool परिणाम;

  अगर (auto_padding && ! has_padding (ctx))
    {
      affirm (auto_padding <= माप (padbuf));
      परिणाम = base_decode_ctx (ctx, padbuf, auto_padding, *out, &n);
    }
  अन्यथा
    {
      परिणाम = base_decode_ctx (ctx, "", 0, *out, &n);
    }

  *outlen = n;
  वापस परिणाम;
}

#अगर BASE_TYPE == 42

अटल idx_t
base64_लंबाई_wrapper (idx_t len)
{
  वापस BASE64_लंबाई (len);
}

अटल व्योम
base64_decode_ctx_init_wrapper (काष्ठा base_decode_context *ctx)
{
  base64_decode_ctx_init (&ctx->ctx.base64);
}

अटल bool
base64_decode_ctx_wrapper (काष्ठा base_decode_context *ctx,
                           अक्षर स्थिर *restrict in, idx_t inlen,
                           अक्षर *restrict out, idx_t *outlen)
{
  वापस base64_decode_ctx (&ctx->ctx.base64, in, inlen, out, outlen);
}

अटल व्योम
init_inbuf (काष्ठा base_decode_context *ctx)
{
  ctx->bufsize = DEC_BLOCKSIZE;
  ctx->inbuf = xcharalloc (ctx->bufsize);
}

अटल व्योम
prepare_inbuf (काष्ठा base_decode_context *ctx, idx_t inlen)
{
  अगर (ctx->bufsize < inlen)
    ctx->inbuf = xpalloc (ctx->inbuf, &ctx->bufsize,
                          inlen - ctx->bufsize, -1, माप *ctx->inbuf);
}


अटल व्योम
base64url_encode (अक्षर स्थिर *restrict in, idx_t inlen,
                  अक्षर *restrict out, idx_t outlen)
{
  base64_encode (in, inlen, out, outlen);
  /* translate 62nd and 63rd characters */
  अक्षर *p = out;
  जबतक (outlen--)
    {
      अगर (*p == '+')
        *p = '-';
      अन्यथा अगर (*p == '/')
        *p = '_';
      ++p;
    }
}

अटल bool
isubase64url (अचिन्हित अक्षर ch)
{
  वापस (ch == '-' || ch == '_'
          || (ch != '+' && ch != '/' && isubase64 (ch)));
}

अटल व्योम
base64url_decode_ctx_init_wrapper (काष्ठा base_decode_context *ctx)
{
  base64_decode_ctx_init (&ctx->ctx.base64);
  init_inbuf (ctx);
}


अटल bool
base64url_decode_ctx_wrapper (काष्ठा base_decode_context *ctx,
                              अक्षर स्थिर *restrict in, idx_t inlen,
                              अक्षर *restrict out, idx_t *outlen)
{
  prepare_inbuf (ctx, inlen);
  स_नकल (ctx->inbuf, in, inlen);

  /* translate 62nd and 63rd characters */
  idx_t i = inlen;
  अक्षर *p = ctx->inbuf;
  जबतक (i--)
    {
      अगर (*p == '+' || *p == '/')
        {
          *outlen = 0;
          वापस false; /* reject base64 input */
        }
      अन्यथा अगर (*p == '-')
        *p = '+';
      अन्यथा अगर (*p == '_')
        *p = '/';
      ++p;
    }

  वापस base64_decode_ctx (&ctx->ctx.base64, ctx->inbuf, inlen,
                            out, outlen);
}



अटल idx_t
base32_लंबाई_wrapper (idx_t len)
{
  वापस BASE32_लंबाई (len);
}

अटल व्योम
base32_decode_ctx_init_wrapper (काष्ठा base_decode_context *ctx)
{
  base32_decode_ctx_init (&ctx->ctx.base32);
}

अटल bool
base32_decode_ctx_wrapper (काष्ठा base_decode_context *ctx,
                           अक्षर स्थिर *restrict in, idx_t inlen,
                           अक्षर *restrict out, idx_t *outlen)
{
  वापस base32_decode_ctx (&ctx->ctx.base32, in, inlen, out, outlen);
}

/* ABCDEFGHIJKLMNOPQRSTUVWXYZ234567
     to
   0123456789ABCDEFGHIJKLMNOPQRSTUV */
अटल स्थिर अक्षर base32_norm_to_hex[32 + 9] = {
/*0x32, 0x33, 0x34, 0x35, 0x36, 0x37, */
  'Q',  'R',  'S',  'T',  'U',  'V',

  0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x40,

/*0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, */
  '0',  '1',  '2',  '3',  '4',  '5',  '6',  '7',

/*0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50, */
  '8',  '9',  'A',  'B',  'C',  'D',  'E',  'F',

/*0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, */
  'G',  'H',  'I',  'J',  'K',  'L',  'M',  'N',

/*0x59, 0x5a, */
  'O',  'P',
};

/* 0123456789ABCDEFGHIJKLMNOPQRSTUV
     to
   ABCDEFGHIJKLMNOPQRSTUVWXYZ234567 */
अटल स्थिर अक्षर base32_hex_to_norm[32 + 9] = {
  /* from: 0x30 .. 0x39 ('0' to '9') */
  /* to:*/ 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',

  0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x40,

  /* from: 0x41 .. 0x4A ('A' to 'J') */
  /* to:*/ 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',

  /* from: 0x4B .. 0x54 ('K' to 'T') */
  /* to:*/ 'U', 'V', 'W', 'X', 'Y', 'Z', '2', '3', '4', '5',

  /* from: 0x55 .. 0x56 ('U' to 'V') */
  /* to:*/ '6', '7'
};


अंतरभूत अटल bool
isubase32hex (अचिन्हित अक्षर ch)
{
  वापस ('0' <= ch && ch <= '9') || ('A' <= ch && ch <= 'V');
}


अटल व्योम
base32hex_encode (अक्षर स्थिर *restrict in, idx_t inlen,
                  अक्षर *restrict out, idx_t outlen)
{
  base32_encode (in, inlen, out, outlen);

  क्रम (अक्षर *p = out; outlen--; p++)
    {
      affirm (0x32 <= *p && *p <= 0x5a);          /* LCOV_EXCL_LINE */
      *p = base32_norm_to_hex[*p - 0x32];
    }
}


अटल व्योम
base32hex_decode_ctx_init_wrapper (काष्ठा base_decode_context *ctx)
{
  base32_decode_ctx_init (&ctx->ctx.base32);
  init_inbuf (ctx);
}


अटल bool
base32hex_decode_ctx_wrapper (काष्ठा base_decode_context *ctx,
                              अक्षर स्थिर *restrict in, idx_t inlen,
                              अक्षर *restrict out, idx_t *outlen)
{
  prepare_inbuf (ctx, inlen);

  idx_t i = inlen;
  अक्षर *p = ctx->inbuf;
  जबतक (i--)
    {
      अगर (isubase32hex (*in))
        *p = base32_hex_to_norm[*in - 0x30];
      अन्यथा
        *p = *in;
      ++p;
      ++in;
    }

  वापस base32_decode_ctx (&ctx->ctx.base32, ctx->inbuf, inlen,
                            out, outlen);
}
/* With this approach this file works independent of the charset used
   (think EBCDIC).  However, it does assume that the characters in the
   Base32 alphabet (A-Z2-7) are encoded in 0..255.  POSIX
   1003.1-2001 require that char and unsigned char are 8-bit
   quantities, though, taking care of that problem.  But this may be a
   potential problem on non-POSIX C99 platforms.

   IBM C V6 for AIX mishandles "#define B32(x) ...'x'...", so use "_"
   as the formal parameter rather than "x".  */
# define B16(_)                                 \
  ((_) == '0' ? 0                               \
   : (_) == '1' ? 1                             \
   : (_) == '2' ? 2                             \
   : (_) == '3' ? 3                             \
   : (_) == '4' ? 4                             \
   : (_) == '5' ? 5                             \
   : (_) == '6' ? 6                             \
   : (_) == '7' ? 7                             \
   : (_) == '8' ? 8                             \
   : (_) == '9' ? 9                             \
   : (_) == 'A' || (_) == 'a' ? 10              \
   : (_) == 'B' || (_) == 'b' ? 11              \
   : (_) == 'C' || (_) == 'c' ? 12              \
   : (_) == 'D' || (_) == 'd' ? 13              \
   : (_) == 'E' || (_) == 'e' ? 14              \
   : (_) == 'F' || (_) == 'f' ? 15              \
   : -1)

अटल चिन्हित अक्षर स्थिर base16_to_int[256] = {
  B16 (0), B16 (1), B16 (2), B16 (3),
  B16 (4), B16 (5), B16 (6), B16 (7),
  B16 (8), B16 (9), B16 (10), B16 (11),
  B16 (12), B16 (13), B16 (14), B16 (15),
  B16 (16), B16 (17), B16 (18), B16 (19),
  B16 (20), B16 (21), B16 (22), B16 (23),
  B16 (24), B16 (25), B16 (26), B16 (27),
  B16 (28), B16 (29), B16 (30), B16 (31),
  B16 (32), B16 (33), B16 (34), B16 (35),
  B16 (36), B16 (37), B16 (38), B16 (39),
  B16 (40), B16 (41), B16 (42), B16 (43),
  B16 (44), B16 (45), B16 (46), B16 (47),
  B16 (48), B16 (49), B16 (50), B16 (51),
  B16 (52), B16 (53), B16 (54), B16 (55),
  B16 (56), B16 (57), B16 (58), B16 (59),
  B16 (60), B16 (61), B16 (62), B16 (63),
  B16 (32), B16 (65), B16 (66), B16 (67),
  B16 (68), B16 (69), B16 (70), B16 (71),
  B16 (72), B16 (73), B16 (74), B16 (75),
  B16 (76), B16 (77), B16 (78), B16 (79),
  B16 (80), B16 (81), B16 (82), B16 (83),
  B16 (84), B16 (85), B16 (86), B16 (87),
  B16 (88), B16 (89), B16 (90), B16 (91),
  B16 (92), B16 (93), B16 (94), B16 (95),
  B16 (96), B16 (97), B16 (98), B16 (99),
  B16 (100), B16 (101), B16 (102), B16 (103),
  B16 (104), B16 (105), B16 (106), B16 (107),
  B16 (108), B16 (109), B16 (110), B16 (111),
  B16 (112), B16 (113), B16 (114), B16 (115),
  B16 (116), B16 (117), B16 (118), B16 (119),
  B16 (120), B16 (121), B16 (122), B16 (123),
  B16 (124), B16 (125), B16 (126), B16 (127),
  B16 (128), B16 (129), B16 (130), B16 (131),
  B16 (132), B16 (133), B16 (134), B16 (135),
  B16 (136), B16 (137), B16 (138), B16 (139),
  B16 (140), B16 (141), B16 (142), B16 (143),
  B16 (144), B16 (145), B16 (146), B16 (147),
  B16 (148), B16 (149), B16 (150), B16 (151),
  B16 (152), B16 (153), B16 (154), B16 (155),
  B16 (156), B16 (157), B16 (158), B16 (159),
  B16 (160), B16 (161), B16 (162), B16 (163),
  B16 (132), B16 (165), B16 (166), B16 (167),
  B16 (168), B16 (169), B16 (170), B16 (171),
  B16 (172), B16 (173), B16 (174), B16 (175),
  B16 (176), B16 (177), B16 (178), B16 (179),
  B16 (180), B16 (181), B16 (182), B16 (183),
  B16 (184), B16 (185), B16 (186), B16 (187),
  B16 (188), B16 (189), B16 (190), B16 (191),
  B16 (192), B16 (193), B16 (194), B16 (195),
  B16 (196), B16 (197), B16 (198), B16 (199),
  B16 (200), B16 (201), B16 (202), B16 (203),
  B16 (204), B16 (205), B16 (206), B16 (207),
  B16 (208), B16 (209), B16 (210), B16 (211),
  B16 (212), B16 (213), B16 (214), B16 (215),
  B16 (216), B16 (217), B16 (218), B16 (219),
  B16 (220), B16 (221), B16 (222), B16 (223),
  B16 (224), B16 (225), B16 (226), B16 (227),
  B16 (228), B16 (229), B16 (230), B16 (231),
  B16 (232), B16 (233), B16 (234), B16 (235),
  B16 (236), B16 (237), B16 (238), B16 (239),
  B16 (240), B16 (241), B16 (242), B16 (243),
  B16 (244), B16 (245), B16 (246), B16 (247),
  B16 (248), B16 (249), B16 (250), B16 (251),
  B16 (252), B16 (253), B16 (254), B16 (255)
};

अटल bool
isubase16 (अचिन्हित अक्षर ch)
{
  वापस ch < माप base16_to_int && 0 <= base16_to_int[ch];
}

अटल idx_t
base16_लंबाई (idx_t len)
{
  वापस len * 2;
}


अटल व्योम
base16_encode (अक्षर स्थिर *restrict in, idx_t inlen,
               अक्षर *restrict out, idx_t outlen)
{
  अटल स्थिर अक्षर base16[16] ATTRIBUTE_NONSTRING = "0123456789ABCDEF";

  जबतक (inlen && outlen)
    {
      अचिन्हित अक्षर c = *in;
      *out++ = base16[c >> 4];
      *out++ = base16[c & 0x0F];
      ++in;
      inlen--;
      outlen -= 2;
    }
}


अटल व्योम
base16_decode_ctx_init (काष्ठा base_decode_context *ctx)
{
  init_inbuf (ctx);
  ctx->ctx.base16.nibble = -1;
}


अटल bool
base16_decode_ctx (काष्ठा base_decode_context *ctx,
                   अक्षर स्थिर *restrict in, idx_t inlen,
                   अक्षर *restrict out, idx_t *outlen)
{
  bool ignore_पंक्तियाँ = true;  /* for now, always ignore them */
  अक्षर *out0 = out;
  चिन्हित अक्षर nibble = ctx->ctx.base16.nibble;

  /* inlen==0 is request to flush output.
     if there is a dangling high nibble - we are missing the low nibble,
     so return false - indicating an invalid input.  */
  अगर (inlen == 0)
    {
      *outlen = 0;
      वापस nibble < 0;
    }

  जबतक (inlen--)
    {
      अचिन्हित अक्षर c = *in++;
      अगर (ignore_पंक्तियाँ && c == '\n')
        जारी;

      अगर (माप base16_to_int <= c || base16_to_int[c] < 0)
        {
          *outlen = out - out0;
          वापस false; /* garbage - return false */
        }

      अगर (nibble < 0)
        nibble = base16_to_int[c];
      अन्यथा
        {
          /* have both nibbles, write octet */
          *out++ = (nibble << 4) + base16_to_int[c];
          nibble = -1;
        }
    }

  ctx->ctx.base16.nibble = nibble;
  *outlen = out - out0;
  वापस true;
}



ATTRIBUTE_PURE
अटल idx_t
z85_लंबाई (idx_t len)
{
  /* Z85 does not allow padding, so no need to round to highest integer.  */
  idx_t z85_len = (len * 5) / 4;
  affirm (0 <= z85_len);
  वापस z85_len;
}

अटल bool
isuz85 (अचिन्हित अक्षर ch)
{
  वापस c_isalnum (ch) || म_अक्षर (".-:+=^!/*?&<>()[]{}@%$#", ch) != शून्य;
}

अटल अक्षर स्थिर z85_encoding[85] ATTRIBUTE_NONSTRING =
  "0123456789"
  "abcdefghijklmnopqrstuvwxyz"
  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
  ".-:+=^!/*?&<>()[]{}@%$#";

अटल व्योम
z85_encode (अक्षर स्थिर *restrict in, idx_t inlen,
            अक्षर *restrict out, idx_t outlen)
{
  पूर्णांक i = 0;
  अचिन्हित अक्षर quad[4];
  idx_t outidx = 0;

  जबतक (true)
    {
      अगर (inlen == 0)
        {
          /* no more input, exactly on 4 octet boundary. */
          अगर (i == 0)
            वापस;

          /* currently, there's no way to return an error in encoding.  */
          error (निकास_त्रुटि, 0,
                 _("invalid input (length must be multiple of 4 characters)"));
        }
      अन्यथा
        {
          quad[i++] = *in++;
          --inlen;
        }

      /* Got a quad, encode it */
      अगर (i == 4)
        {
          int_fast64_t val = quad[0];
          val = (val << 24) + (quad[1] << 16) + (quad[2] << 8) + quad[3];

          क्रम (पूर्णांक j = 4; j >= 0; --j)
            {
              पूर्णांक c = val % 85;
              val /= 85;

              /* NOTE: if there is padding (which is trimmed by z85
                 before outputting the result), the output buffer 'out'
                 might not include enough allocated bytes for the padding,
                 so don't store them. */
              अगर (outidx + j < outlen)
                out[j] = z85_encoding[c];
            }
          out += 5;
          outidx += 5;
          i = 0;
        }
    }
}

अटल व्योम
z85_decode_ctx_init (काष्ठा base_decode_context *ctx)
{
  init_inbuf (ctx);
  ctx->ctx.z85.i = 0;
}


# define Z85_LO_CTX_TO_32BIT_VAL(ctx) \
  (((ctx)->ctx.z85.octets[1] * 85 * 85 * 85) +      \
   ((ctx)->ctx.z85.octets[2] * 85 * 85) +	    \
   ((ctx)->ctx.z85.octets[3] * 85) +		    \
   ((ctx)->ctx.z85.octets[4]))


# define Z85_HI_CTX_TO_32BIT_VAL(ctx) \
  ((int_fast64_t) (ctx)->ctx.z85.octets[0] * 85 * 85 * 85 * 85 )

/*
 0 -  9:  0 1 2 3 4 5 6 7 8 9
 10 - 19:  a b c d e f g h i j
 20 - 29:  k l m n o p q r s t
 30 - 39:  u v w x y z A B C D
 40 - 49:  E F G H I J K L M N
 50 - 59:  O P Q R S T U V W X
 60 - 69:  Y Z . - : + = ^ ! /   #dummy comment to workaround syntax-check
 70 - 79:  * ? & < > ( ) [ ] {
 80 - 84:  } @ % $ #
*/
अटल चिन्हित अक्षर स्थिर z85_decoding[93] = {
  68, -1,  84,  83, 82,  72, -1,               /* ! " # $ % & ' */
  75, 76,  70,  65, -1,  63, 62, 69,           /* ( ) * + , - . / */
  0,  1,   2,   3,  4,   5,  6,   7,  8,  9,   /* '0' to '9' */
  64, -1,  73,  66, 74,  71, 81,               /* : ; < =  > ? @ */
  36, 37,  38,  39, 40,  41, 42,  43, 44, 45,  /* 'A' to 'J' */
  46, 47,  48,  49, 50,  51, 52,  53, 54, 55,  /* 'K' to 'T' */
  56, 57,  58,  59, 60,  61,                   /* 'U' to 'Z' */
  77,  -1, 78,  67,  -1,  -1,                  /* [ \ ] ^ _ ` */
  10, 11,  12,  13, 14,  15, 16,  17, 18, 19,  /* 'a' to 'j' */
  20, 21,  22,  23, 24,  25, 26,  27, 28, 29,  /* 'k' to 't' */
  30, 31,  32,  33, 34,  35,                   /* 'u' to 'z' */
  79, -1,  80                                  /* { | } */
};

अटल bool
z85_decode_ctx (काष्ठा base_decode_context *ctx,
                अक्षर स्थिर *restrict in, idx_t inlen,
                अक्षर *restrict out, idx_t *outlen)
{
  bool ignore_पंक्तियाँ = true;  /* for now, always ignore them */

  *outlen = 0;

  /* inlen==0 is request to flush output.
     if there are dangling values - we are missing entries,
     so return false - indicating an invalid input.  */
  अगर (inlen == 0)
    {
      अगर (ctx->ctx.z85.i > 0)
        {
          /* Z85 variant does not allow padding - input must
             be a multiple of 5 - so return error.  */
          वापस false;
        }
      वापस true;
    }

  जबतक (inlen--)
    {
      अगर (ignore_पंक्तियाँ && *in == '\n')
        {
          ++in;
          जारी;
        }

      /* z85 decoding */
      अचिन्हित अक्षर c = *in;

      अगर (c >= 33 && c <= 125)
        {
          चिन्हित अक्षर ch = z85_decoding[c - 33];
          अगर (ch < 0)
            वापस false; /* garbage - return false */
          c = ch;
        }
      अन्यथा
        वापस false; /* garbage - return false */

      ++in;

      ctx->ctx.z85.octets[ctx->ctx.z85.i++] = c;
      अगर (ctx->ctx.z85.i == 5)
        {
          /* decode the lowest 4 octets, then check for overflows.  */
          int_fast64_t val = Z85_LO_CTX_TO_32BIT_VAL (ctx);

          /* The Z85 spec and the reference implementation say nothing
             about overflows. To be on the safe side, reject them.  */

          val += Z85_HI_CTX_TO_32BIT_VAL (ctx);
          अगर ((val >> 24) & ~0xFF)
            वापस false;

          *out++ = val >> 24;
          *out++ = (val >> 16) & 0xFF;
          *out++ = (val >> 8) & 0xFF;
          *out++ = val & 0xFF;

          *outlen += 4;

          ctx->ctx.z85.i = 0;
        }
    }
  वापस true;
}


अंतरभूत अटल bool
isubase2 (अचिन्हित अक्षर ch)
{
  वापस ch == '0' || ch == '1';
}

अटल idx_t
base2_लंबाई (idx_t len)
{
  वापस len * 8;
}


अंतरभूत अटल व्योम
base2msbf_encode (अक्षर स्थिर *restrict in, idx_t inlen,
                  अक्षर *restrict out, idx_t outlen)
{
  जबतक (inlen && outlen)
    {
      अचिन्हित अक्षर c = *in;
      क्रम (पूर्णांक i = 0; i < 8; i++)
        {
          *out++ = c & 0x80 ? '1' : '0';
          c <<= 1;
        }
      inlen--;
      outlen -= 8;
      ++in;
    }
}

अंतरभूत अटल व्योम
base2lsbf_encode (अक्षर स्थिर *restrict in, idx_t inlen,
                  अक्षर *restrict out, idx_t outlen)
{
  जबतक (inlen && outlen)
    {
      अचिन्हित अक्षर c = *in;
      क्रम (पूर्णांक i = 0; i < 8; i++)
        {
          *out++ = c & 0x01 ? '1' : '0';
          c >>= 1;
        }
      inlen--;
      outlen -= 8;
      ++in;
    }
}


अटल व्योम
base2_decode_ctx_init (काष्ठा base_decode_context *ctx)
{
  init_inbuf (ctx);
  ctx->ctx.base2.octet = 0;
  ctx->ctx.base2.bit_pos = 0;
}


अटल bool
base2lsbf_decode_ctx (काष्ठा base_decode_context *ctx,
                      अक्षर स्थिर *restrict in, idx_t inlen,
                      अक्षर *restrict out, idx_t *outlen)
{
  bool ignore_पंक्तियाँ = true;  /* for now, always ignore them */

  *outlen = 0;

  /* inlen==0 is request to flush output.
     if there is a dangling bit - we are missing some bits,
     so return false - indicating an invalid input.  */
  अगर (inlen == 0)
    वापस ctx->ctx.base2.bit_pos == 0;

  जबतक (inlen--)
    {
      अगर (ignore_पंक्तियाँ && *in == '\n')
        {
          ++in;
          जारी;
        }

      अगर (!isubase2 (*in))
        वापस false;

      bool bit = (*in == '1');
      ctx->ctx.base2.octet |= bit << ctx->ctx.base2.bit_pos;
      ++ctx->ctx.base2.bit_pos;

      अगर (ctx->ctx.base2.bit_pos == 8)
        {
          *out++ = ctx->ctx.base2.octet;
          ctx->ctx.base2.octet = 0;
          ++*outlen;
          ctx->ctx.base2.bit_pos = 0;
        }

      ++in;
    }

  वापस true;
}

अटल bool
base2msbf_decode_ctx (काष्ठा base_decode_context *ctx,
                      अक्षर स्थिर *restrict in, idx_t inlen,
                      अक्षर *restrict out, idx_t *outlen)
{
  bool ignore_पंक्तियाँ = true;  /* for now, always ignore them */

  *outlen = 0;

  /* inlen==0 is request to flush output.
     if there is a dangling bit - we are missing some bits,
     so return false - indicating an invalid input.  */
  अगर (inlen == 0)
    वापस ctx->ctx.base2.bit_pos == 0;

  जबतक (inlen--)
    {
      अगर (ignore_पंक्तियाँ && *in == '\n')
        {
          ++in;
          जारी;
        }

      अगर (!isubase2 (*in))
        वापस false;

      bool bit = (*in == '1');
      अगर (ctx->ctx.base2.bit_pos == 0)
        ctx->ctx.base2.bit_pos = 8;
      --ctx->ctx.base2.bit_pos;
      ctx->ctx.base2.octet |= bit << ctx->ctx.base2.bit_pos;

      अगर (ctx->ctx.base2.bit_pos == 0)
        {
          *out++ = ctx->ctx.base2.octet;
          ctx->ctx.base2.octet = 0;
          ++*outlen;
        }

      ++in;
    }

  वापस true;
}

/* Map from GMP (up to base 62):
   "0123456789 ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuv";
   to base 58:
   "123456789A BCDEFGHJKLMNPQRSTUVWXYZabc defghijkmnopqrstuvwxyz";  */
अटल चिन्हित अक्षर स्थिर gmp_to_base58[256] = {
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  '1','2','3','4','5','6','7','8','9','A',-1, -1, -1, -1, -1, -1,
  -1, 'B','C','D','E','F','G','H','J','K','L','M','N','P','Q','R',
  'S','T','U','V','W','X','Y','Z','a','b','c',-1, -1, -1, -1, -1,
  -1, 'd','e','f','g','h','i','j','k','m','n','o','p','q','r','s',
  't','u','v','w','x','y','z',-1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};

अटल चिन्हित अक्षर स्थिर base58_to_gmp[256] = {
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, '0','1','2','3','4','5','6','7','8',-1, -1, -1, -1, -1, -1,
  -1, '9','A','B','C','D','E','F','G', -1,'H','I','J','K','L',-1,
  'M','N','O','P','Q','R','S','T','U','V','W',-1, -1, -1, -1, -1,
  -1, 'X','Y','Z','a','b','c','d','e','f','g','h',-1, 'i','j','k',
  'l','m','n','o','p','q','r','s','t','u','v',-1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};

अटल bool
isubase58 (अचिन्हित अक्षर ch)
{
  वापस ch < माप base58_to_gmp && 0 <= base58_to_gmp[ch];
}


ATTRIBUTE_PURE
अटल idx_t
base58_लंबाई (idx_t len)
{
  /* Base58 output length is approximately log(256)/log(58),
     which is approximately len * 138 / 100,
     which is at most ((len + 100 - 1) / 100) * 138
     +1 to ensure we've enough place for NUL  */
  idx_t base58_len = ((len + 99) / 100) * 138 + 1;
  affirm (0 < base58_len);
  वापस base58_len;
}


अटल व्योम
base58_encode_ctx_init (काष्ठा base_encode_context *ctx)
{
  ctx->ctx.base58.buf = शून्य;
  ctx->ctx.base58.आकार = 0;
  ctx->ctx.base58.capacity = 0;
}


अटल bool
base58_encode_ctx (काष्ठा base_encode_context *ctx,
                   अक्षर स्थिर *restrict in, idx_t inlen,
                   MAYBE_UNUSED अक्षर *restrict out, idx_t *outlen)
{
  *outlen = 0;   /* Only accumulate input in this function.  */

  अगर (inlen == 0)
    वापस true;

  idx_t free_space = ctx->ctx.base58.capacity - ctx->ctx.base58.आकार;
  अगर (free_space < inlen)
    {
      ctx->ctx.base58.buf = xpalloc (ctx->ctx.base58.buf,
                                     &ctx->ctx.base58.capacity,
                                     inlen - free_space,
                                     -1, माप *ctx->ctx.base58.buf);
    }

  स_नकल (ctx->ctx.base58.buf + ctx->ctx.base58.आकार, in, inlen);
  ctx->ctx.base58.आकार += inlen;

  वापस true;
}

अटल व्योम
base58_encode (अक्षर स्थिर *तथ्य, माप_प्रकार तथ्य_len,
               अक्षर *out, idx_t *outlen)
{
  affirm (base_लंबाई (तथ्य_len) <= *outlen);

  माप_प्रकार zeros = 0;
  जबतक (zeros < तथ्य_len && तथ्य[zeros] == 0)
    zeros++;

  स_रखो (out, '1', zeros);
  अक्षर *p = out + zeros;

  /* Use GMP to convert from base 256 to base 58.  */
  mpz_t अंक;
  mpz_init (अंक);
  अगर (तथ्य_len - zeros)
    {
      mpz_import (अंक, तथ्य_len - zeros, 1, 1, 0, 0, तथ्य + zeros);
      क्रम (p = mpz_get_str (p, 58, अंक); *p; p++)
        *p = gmp_to_base58[to_uchar (*p)];
    }
  mpz_clear (अंक);

  *outlen = p - out;
}


अटल bool
base58_encode_ctx_finalize (काष्ठा base_encode_context *ctx,
                            अक्षर *restrict *out, idx_t *outlen)
{
  /* Ensure output buffer is large enough.  */
  idx_t max_outlen = base_लंबाई (ctx->ctx.base58.आकार);
  अगर (max_outlen > *outlen)
    {
      *out = xrealloc (*out, max_outlen);
      *outlen = max_outlen;
    }

  base58_encode ((अक्षर *)ctx->ctx.base58.buf, ctx->ctx.base58.आकार,
                 *out, outlen);

  मुक्त (ctx->ctx.base58.buf);
  ctx->ctx.base58.buf = शून्य;
  ctx->ctx.base58.आकार = 0;
  ctx->ctx.base58.capacity = 0;

  वापस true;
}


अटल व्योम
base58_decode_ctx_init (काष्ठा base_decode_context *ctx)
{
  ctx->ctx.base58.आकार = 0;
  ctx->ctx.base58.capacity = 0;
  ctx->ctx.base58.buf = शून्य;
}

अटल bool
base58_decode_ctx (काष्ठा base_decode_context *ctx,
                   अक्षर स्थिर *restrict in, idx_t inlen,
                   MAYBE_UNUSED अक्षर *restrict out, idx_t *outlen)
{
  bool ignore_पंक्तियाँ = true;  /* for now, always ignore them */

  *outlen = 0;   /* Only accumulate input in this function.  */

  अगर (inlen == 0)
    वापस true;

  idx_t free_space = ctx->ctx.base58.capacity - ctx->ctx.base58.आकार;
  free_space -= 1;  /* Ensure we leave space for NUL (for mpz_set_str).  */
  अगर (free_space < inlen)
    {
      ctx->ctx.base58.buf = xpalloc (ctx->ctx.base58.buf,
                                     &ctx->ctx.base58.capacity,
                                     inlen - free_space,
                                     -1, माप *ctx->ctx.base58.buf);
    }

  /* Accumulate all valid input characters in our buffer.
     Note we don't rely on mpz_set_str() for validation
     as that allows (skips) all whitespace.  */
  क्रम (idx_t i = 0; i < inlen; i++)
    {
      अचिन्हित अक्षर c = in[i];

      अगर (ignore_पंक्तियाँ && c == '\n')
        जारी;

      अगर (!isubase58 (c))
        वापस false;

      ctx->ctx.base58.buf[ctx->ctx.base58.आकार++] = base58_to_gmp[to_uchar (c)];
    }

  वापस true;
}


अटल bool
base58_decode (अक्षर स्थिर *तथ्य, माप_प्रकार तथ्य_len,
               अक्षर *restrict out, idx_t *outlen)
{
  affirm (तथ्य_len <= *outlen);

  माप_प्रकार ones = 0;
  जबतक (ones < तथ्य_len && तथ्य[ones] == base58_to_gmp['1'])
    ones++;

  स_रखो (out, 0, ones);

  /* Use GMP to convert from base 58 to base 256.  */
  mpz_t अंक;
  mpz_init (अंक);

  अगर ((तथ्य_len - ones) && mpz_set_str (अंक, तथ्य + ones, 58) != 0)
    {
      mpz_clear (अंक);
      *outlen = 0;
      वापस false;
    }

  माप_प्रकार exported_आकार = 0;
  अगर (तथ्य_len - ones)
    {
      माप_प्रकार द्विआधारी_आकार = (mpz_sizeinbase (अंक, 2) + 7) / 8;
      affirm (*outlen - ones >= द्विआधारी_आकार);
      mpz_export (out + ones, &exported_आकार, 1, 1, 0, 0, अंक);
    }

  mpz_clear (अंक);
  *outlen = ones + exported_आकार;
  वापस true;
}


अटल bool
base58_decode_ctx_finalize (काष्ठा base_decode_context *ctx,
                            अक्षर *restrict *out, idx_t *outlen)
{
  /* Ensure output buffer is large enough.
     Worst case is input is all '1's.  */
  idx_t max_outlen = ctx->ctx.base58.आकार;
  अगर (max_outlen > *outlen)
    {
      *out = xrealloc (*out, max_outlen);
      *outlen = max_outlen;
    }

  /* Ensure input buffer is NUL terminated (for mpz_get_str).  */
  अगर (ctx->ctx.base58.आकार)
    ctx->ctx.base58.buf[ctx->ctx.base58.आकार] = '\0';

  bool ret = base58_decode ((अक्षर *)ctx->ctx.base58.buf, ctx->ctx.base58.आकार,
                            *out, outlen);

  मुक्त (ctx->ctx.base58.buf);
  ctx->ctx.base58.buf = शून्य;
  ctx->ctx.base58.आकार = 0;
  ctx->ctx.base58.capacity = 0;

  वापस ret;
}

#पूर्ण_अगर /* BASE_TYPE == 42, i.e., "basenc"*/



अटल व्योम
wrap_write (अक्षर स्थिर *बफर, idx_t len,
            idx_t wrap_column, idx_t *current_column, खाता *out)
{
  अगर (wrap_column == 0)
    {
      /* Simple write. */
      अगर (ख_डालो (बफर, 1, len, मानक_निकास) < len)
        write_error ();
    }
  अन्यथा
    क्रम (idx_t written = 0; written < len; )
      {
        idx_t to_write = MIN (wrap_column - *current_column, len - written);

        अगर (to_write == 0)
          {
            अगर (ख_अक्षर_दो ('\n', out) == खातापूर्ण)
              write_error ();
            *current_column = 0;
          }
        अन्यथा
          {
            अगर (ख_डालो (बफर + written, 1, to_write, मानक_निकास) < to_write)
              write_error ();
            *current_column += to_write;
            written += to_write;
          }
      }
}

अटल _Noreturn व्योम
finish_and_exit (खाता *in, अक्षर स्थिर *infile)
{
  अगर (ख_बंद (in) != 0)
    {
      अगर (streq (infile, "-"))
        error (निकास_त्रुटि, त्रुटि_सं, _("closing standard input"));
      अन्यथा
        error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (infile));
    }

  निकास (निकास_सफल);
}

अटल _Noreturn व्योम
do_encode (खाता *in, अक्षर स्थिर *infile, खाता *out, idx_t wrap_column)
{
  idx_t current_column = 0;
  अक्षर *inbuf, *outbuf;
  idx_t योग;

  inbuf = xmalloc (ENC_BLOCKSIZE);
  outbuf = xmalloc (BASE_लंबाई (ENC_BLOCKSIZE));

#अगर BASE_TYPE == 42
  /* Initialize encoding context if needed (for base58) */
  काष्ठा base_encode_context encode_ctx;
  bool use_ctx = (base_encode_ctx_init != शून्य);
  अगर (use_ctx)
    base_encode_ctx_init (&encode_ctx);
#पूर्ण_अगर

  करो
    {
      idx_t n;

      योग = 0;
      करो
        {
          n = ख_पढ़ो (inbuf + योग, 1, ENC_BLOCKSIZE - योग, in);
          योग += n;
        }
      जबतक (!ख_पूर्ण (in) && !ख_त्रुटि (in) && योग < ENC_BLOCKSIZE);

      अगर (योग > 0)
        {
#अगर BASE_TYPE == 42
          अगर (use_ctx)
            {
              idx_t outlen = 0;
              base_encode_ctx (&encode_ctx, inbuf, योग, outbuf, &outlen);

              wrap_write (outbuf, outlen, wrap_column, &current_column, out);
            }
          अन्यथा
#पूर्ण_अगर
            {
              /* Process input one block at a time.  Note that ENC_BLOCKSIZE
                 is sized so that no pad chars will appear in output. */
              base_encode (inbuf, योग, outbuf, BASE_लंबाई (योग));

              wrap_write (outbuf, BASE_लंबाई (योग), wrap_column,
                          &current_column, out);
            }
        }
    }
  जबतक (!ख_पूर्ण (in) && !ख_त्रुटि (in) && योग == ENC_BLOCKSIZE);

#अगर BASE_TYPE == 42
  अगर (use_ctx && base_encode_ctx_finalize)
    {
      idx_t outlen = BASE_लंबाई (ENC_BLOCKSIZE);
      base_encode_ctx_finalize (&encode_ctx, &outbuf, &outlen);

      wrap_write (outbuf, outlen, wrap_column, &current_column, out);
    }
#पूर्ण_अगर

  /* When wrapping, terminate last line. */
  अगर (wrap_column && current_column > 0 && ख_अक्षर_दो ('\n', out) == खातापूर्ण)
    write_error ();

  अगर (ख_त्रुटि (in))
    error (निकास_त्रुटि, त्रुटि_सं, _("read error"));

  finish_and_exit (in, infile);
}

अटल _Noreturn व्योम
do_decode (खाता *in, अक्षर स्थिर *infile, खाता *out, bool ignore_garbage)
{
  अक्षर *inbuf, *outbuf;
  idx_t योग;
  काष्ठा base_decode_context ctx;

  inbuf = xmalloc (BASE_लंबाई (DEC_BLOCKSIZE));
  outbuf = xmalloc (DEC_BLOCKSIZE);

#अगर BASE_TYPE == 42
  ctx.inbuf = शून्य;
#पूर्ण_अगर
  base_decode_ctx_init (&ctx);

  करो
    {
      bool ok;

      योग = 0;
      करो
        {
          idx_t n = ख_पढ़ो (inbuf + योग,
                           1, BASE_लंबाई (DEC_BLOCKSIZE) - योग, in);

          अगर (ignore_garbage)
            {
              क्रम (idx_t i = 0; n > 0 && i < n;)
                {
                  अगर (isubase (inbuf[योग + i])
                      || (REQUIRED_PADDING (1) && inbuf[योग + i] == '='))
                    i++;
                  अन्यथा
                    स_हटाओ (inbuf + योग + i, inbuf + योग + i + 1, --n - i);
                }
            }

          योग += n;

          अगर (ख_त्रुटि (in))
            error (निकास_त्रुटि, त्रुटि_सं, _("read error"));
        }
      जबतक (योग < BASE_लंबाई (DEC_BLOCKSIZE) && !ख_पूर्ण (in));

      जबतक (योग || ख_पूर्ण (in))
        {
          idx_t n = DEC_BLOCKSIZE;
          अगर (योग)
            ok = base_decode_ctx (&ctx, inbuf, योग, outbuf, &n);
          अन्यथा
            ok = base_decode_ctx_finalize (&ctx, &outbuf, &n);

          अगर (ख_डालो (outbuf, 1, n, out) < n)
            write_error ();

          अगर (! ok)
            error (निकास_त्रुटि, 0, _("invalid input"));

          अगर (योग == 0)
            अवरोध;
          योग = 0;
        }
    }
  जबतक (!ख_पूर्ण (in));

  finish_and_exit (in, infile);
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  पूर्णांक opt;
  खाता *निवेश_fh;
  अक्षर स्थिर *infile;

  /* True if --decode has been given and we should decode data. */
  bool decode = false;
  /* True if we should ignore non-base-alphabetic characters. */
  bool ignore_garbage = false;
  /* Wrap encoded data around the 76th column, by default. */
  idx_t wrap_column = 76;

#अगर BASE_TYPE == 42
  पूर्णांक base_type = 0;
#पूर्ण_अगर

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  जबतक ((opt = getopt_long (argc, argv, "diw:", long_options, शून्य)) != -1)
    चयन (opt)
      {
      हाल 'd':
        decode = true;
        अवरोध;

      हाल 'w':
        {
          intmax_t w;
          strtol_error s_err = xstrtoimax (optarg, शून्य, 10, &w, "");
          अगर (LONGINT_OVERFLOW < s_err || w < 0)
            error (निकास_त्रुटि, 0, "%s: %s",
                   _("invalid wrap size"), quote (optarg));
          wrap_column = s_err == LONGINT_OVERFLOW || IDX_MAX < w ? 0 : w;
        }
        अवरोध;

      हाल 'i':
        ignore_garbage = true;
        अवरोध;

#अगर BASE_TYPE == 42
      हाल BASE64_OPTION:
      हाल BASE64URL_OPTION:
      हाल BASE32_OPTION:
      हाल BASE32HEX_OPTION:
      हाल BASE16_OPTION:
      हाल BASE2MSBF_OPTION:
      हाल BASE2LSBF_OPTION:
      हाल Z85_OPTION:
      हाल BASE58_OPTION:
        base_type = opt;
        अवरोध;
#पूर्ण_अगर

      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

      शेष:
        usage (निकास_त्रुटि);
        अवरोध;
      }

#अगर BASE_TYPE == 42
  required_padding = no_required_padding;
  has_padding = no_padding;
  get_pending_लंबाई = no_pending_लंबाई;
  base_decode_ctx_finalize = decode_ctx_finalize;

  चयन (base_type)
    {
    हाल BASE64_OPTION:
      base_लंबाई = base64_लंबाई_wrapper;
      required_padding = base64_required_padding;
      has_padding = base64_ctx_has_padding;
      get_pending_लंबाई = base64_ctx_get_pending_लंबाई;
      isubase = isubase64;
      base_encode = base64_encode;
      base_decode_ctx_init = base64_decode_ctx_init_wrapper;
      base_decode_ctx = base64_decode_ctx_wrapper;
      अवरोध;

    हाल BASE64URL_OPTION:
      base_लंबाई = base64_लंबाई_wrapper;
      required_padding = base64_required_padding;
      has_padding = base64_ctx_has_padding;
      get_pending_लंबाई = base64_ctx_get_pending_लंबाई;
      isubase = isubase64url;
      base_encode = base64url_encode;
      base_decode_ctx_init = base64url_decode_ctx_init_wrapper;
      base_decode_ctx = base64url_decode_ctx_wrapper;
      अवरोध;

    हाल BASE32_OPTION:
      base_लंबाई = base32_लंबाई_wrapper;
      required_padding = base32_required_padding;
      has_padding = base32_ctx_has_padding;
      get_pending_लंबाई = base32_ctx_get_pending_लंबाई;
      isubase = isubase32;
      base_encode = base32_encode;
      base_decode_ctx_init = base32_decode_ctx_init_wrapper;
      base_decode_ctx = base32_decode_ctx_wrapper;
      अवरोध;

    हाल BASE32HEX_OPTION:
      base_लंबाई = base32_लंबाई_wrapper;
      required_padding = base32_required_padding;
      has_padding = base32_ctx_has_padding;
      get_pending_लंबाई = base32_ctx_get_pending_लंबाई;
      isubase = isubase32hex;
      base_encode = base32hex_encode;
      base_decode_ctx_init = base32hex_decode_ctx_init_wrapper;
      base_decode_ctx = base32hex_decode_ctx_wrapper;
      अवरोध;

    हाल BASE16_OPTION:
      base_लंबाई = base16_लंबाई;
      get_pending_लंबाई = base16_ctx_get_pending_लंबाई;
      isubase = isubase16;
      base_encode = base16_encode;
      base_decode_ctx_init = base16_decode_ctx_init;
      base_decode_ctx = base16_decode_ctx;
      अवरोध;

    हाल BASE2MSBF_OPTION:
      base_लंबाई = base2_लंबाई;
      get_pending_लंबाई = base2_ctx_get_pending_लंबाई;
      isubase = isubase2;
      base_encode = base2msbf_encode;
      base_decode_ctx_init = base2_decode_ctx_init;
      base_decode_ctx = base2msbf_decode_ctx;
      अवरोध;

    हाल BASE2LSBF_OPTION:
      base_लंबाई = base2_लंबाई;
      get_pending_लंबाई = base2_ctx_get_pending_लंबाई;
      isubase = isubase2;
      base_encode = base2lsbf_encode;
      base_decode_ctx_init = base2_decode_ctx_init;
      base_decode_ctx = base2lsbf_decode_ctx;
      अवरोध;

    हाल Z85_OPTION:
      base_लंबाई = z85_लंबाई;
      get_pending_लंबाई = z85_ctx_get_pending_लंबाई;
      isubase = isuz85;
      base_encode = z85_encode;
      base_decode_ctx_init = z85_decode_ctx_init;
      base_decode_ctx = z85_decode_ctx;
      अवरोध;

    हाल BASE58_OPTION:
      base_लंबाई = base58_लंबाई;
      isubase = isubase58;
      base_encode_ctx_init = base58_encode_ctx_init;
      base_encode_ctx = base58_encode_ctx;
      base_encode_ctx_finalize = base58_encode_ctx_finalize;
      base_decode_ctx_init = base58_decode_ctx_init;
      base_decode_ctx = base58_decode_ctx;
      base_decode_ctx_finalize = base58_decode_ctx_finalize;
      अवरोध;

    शेष:
      error (0, 0, _("missing encoding type"));
      usage (निकास_त्रुटि);
    }
#पूर्ण_अगर

  अगर (argc - optind > 1)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
      usage (निकास_त्रुटि);
    }

  अगर (optind < argc)
    infile = argv[optind];
  अन्यथा
    infile = "-";

  अगर (streq (infile, "-"))
    {
      xset_द्विआधारी_mode (STDIN_FILENO, O_द्विआधारी);
      निवेश_fh = मानक_निवेश;
    }
  अन्यथा
    {
      निवेश_fh = ख_खोलो (infile, "rb");
      अगर (निवेश_fh == शून्य)
        error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (infile));
    }

  fadvise (निवेश_fh, FADVISE_SEQUENTIAL);

  अगर (decode)
    do_decode (निवेश_fh, infile, मानक_निकास, ignore_garbage);
  अन्यथा
    do_encode (निवेश_fh, infile, मानक_निकास, wrap_column);
}
