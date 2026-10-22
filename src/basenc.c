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

#अन्तर्भाव <config.h>

#अन्तर्भाव <stdio.h>
#अन्तर्भाव <getopt.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <gmp.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "assure.h"
#अन्तर्भाव "c-ctype.h"
#अन्तर्भाव "fadvise.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "xstrtol.h"
#अन्तर्भाव "xbinary-io.h"

#यदि आधारः_TYPE == 42
# define AUTHORS \
  proper_name ("Simon Josefsson"), \
  proper_name ("Assaf Gordon")
#अन्यथा
# define AUTHORS proper_name ("Simon Josefsson")
#यदि_अन्तः

#यदि आधारः_TYPE == 32
# include "base32.h"
# define क्रमादेशः_NAME "base32"
#नोचेद्यदि आधारः_TYPE == 64
# include "base64.h"
# define क्रमादेशः_NAME "base64"
#नोचेद्यदि आधारः_TYPE == 42
# include "base32.h"
# include "base64.h"
# include "assure.h"
# define क्रमादेशः_NAME "basenc"
#अन्यथा
# error missing/invalid आधारः_TYPE definition
#यदि_अन्तः



#यदि आधारः_TYPE == 42
गणना
{
  BASE64_OPTION = CHAR_MAX + 1,
  BASE64URL_OPTION,
  BASE58_OPTION,
  BASE32_OPTION,
  BASE32HEX_OPTION,
  BASE16_OPTION,
  BASE2MSBF_OPTION,
  BASE2LSBF_OPTION,
  Z85_OPTION
};
#यदि_अन्तः

स्थिर संरचना option ध्रुव long_options[] =
{
  {"decode", no_तर्कः, रिक्त, 'd'},
  {"wrap", required_तर्कः, रिक्त, 'w'},
  {"ignore-garbage", no_तर्कः, रिक्त, 'i'},
#यदि आधारः_TYPE == 42
  {"base64",    no_तर्कः, रिक्त, BASE64_OPTION},
  {"base64url", no_तर्कः, रिक्त, BASE64URL_OPTION},
  {"base58",    no_तर्कः, रिक्त, BASE58_OPTION},
  {"base32",    no_तर्कः, रिक्त, BASE32_OPTION},
  {"base32hex", no_तर्कः, रिक्त, BASE32HEX_OPTION},
  {"base16",    no_तर्कः, रिक्त, BASE16_OPTION},
  {"base2msbf", no_तर्कः, रिक्त, BASE2MSBF_OPTION},
  {"base2lsbf", no_तर्कः, रिक्त, BASE2LSBF_OPTION},
  {"z85",       no_तर्कः, रिक्त, Z85_OPTION},
#यदि_अन्तः
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_संस्करणम्_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

शून्य
usage (पूर्णाङ्क स्थितिः)
{
  यदि (स्थितिः != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
    {
      प्रारूप_लिख (_("\
Usage: %s [OPTION]... [FILE]\n\
"), क्रमादेशः_name);

#यदि आधारः_TYPE == 42
      fputs (_("\
basenc encode or decode FILE, or standard निवेशः, गन्तव्यम् standard output.\n\
"), stdout);
#अन्यथा
      प्रारूप_लिख (_("\
आधारः%d encode or decode FILE, or standard निवेशः, गन्तव्यम् standard output.\n\
"), आधारः_TYPE);
#यदि_अन्तः

      emit_stdin_note ();
      emit_mandatory_arg_note ();
#यदि आधारः_TYPE == 42
      oputs (_("\
      --base64\n\
         same as base64(1) क्रमादेशः (RFC4648 section 4)\n\
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
         same as base32(1) क्रमादेशः (RFC4648 section 6)\n\
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
         bit string with न्यूनतमम् significant bit (lsb) first\n\
"));
#यदि_अन्तः
      oputs (_("\
  -d, --decode\n\
         decode दत्त\n\
"));
      oputs (_("\
  -i, --ignore-garbage\n\
         when decoding, ignore non-alphabet characters\n\
"));
      oputs (_("\
  -w, --wrap=स्तम्भसङ्ख्या\n\
         wrap encoded पङ्क्तयः after स्तम्भसङ्ख्या character (स्वभाव 76).\n\
         Use 0 गन्तव्यम् disable पङ्क्तिः wrapping\n\
"));
#यदि आधारः_TYPE == 42
      oputs (_("\
      --z85\n\
         ascii85-like encoding (ZeroMQ spec:32/Z85);\n\
         when encoding, निवेशः दीर्घता must be a गुणजः सम्बन्धि 4;\n\
         when decoding, निवेशः दीर्घता must be a गुणजः सम्बन्धि 5\n\
"));
#यदि_अन्तः
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (संस्करणम्_OPTION_DESCRIPTION);
#यदि आधारः_TYPE == 42
      fputs (_("\
\n\
When decoding, the निवेशः may contain newlines अन्तर् addition गन्तव्यम् the अष्टकानि सम्बन्धि\n\
the formal alphabet.  Use --ignore-garbage गन्तव्यम् attempt गन्तव्यम् recover\n\
from any other non-alphabet अष्टकानि अन्तर् the encoded प्रवाहः.\n\
"), stdout);
#अन्यथा
      प्रारूप_लिख (_("\
\n\
The दत्त are encoded as described प्रति the %s alphabet अन्तर् RFC 4648.\n\
When decoding, the निवेशः may contain newlines अन्तर् addition गन्तव्यम् the अष्टकानि सम्बन्धि\n\
the formal %s alphabet.  Use --ignore-garbage गन्तव्यम् attempt गन्तव्यम् recover\n\
from any other non-alphabet अष्टकानि अन्तर् the encoded प्रवाहः.\n"),
              क्रमादेशः_NAME, क्रमादेशः_NAME);
#यदि_अन्तः
      emit_ancillary_info (क्रमादेशः_NAME);
    }

  निर्गच्छ (स्थितिः);
}

#यदि आधारः_TYPE != 64
स्थिर पूर्णाङ्क
base32_required_padding (पूर्णाङ्क len)
{
  पूर्णाङ्क partial = len % 8;
  प्रत्यागम partial ? 8 - partial : 0;
}
#यदि_अन्तः

#यदि आधारः_TYPE != 32
स्थिर पूर्णाङ्क
base64_required_padding (पूर्णाङ्क len)
{
  पूर्णाङ्क partial = len % 4;
  प्रत्यागम partial ? 4 - partial : 0;
}
#यदि_अन्तः

#यदि आधारः_TYPE == 42
स्थिर पूर्णाङ्क
no_required_padding (MAYBE_अप्रयुक्तम् पूर्णाङ्क len)
{
  प्रत्यागम 0;
}
#यदि_अन्तः

#परिभाषय ENC_BLOCKSIZE (1024 * 3 * 10)

#यदि आधारः_TYPE == 32
# define आधारः_दीर्घता BASE32_दीर्घता
# define REQUIRED_PADDING base32_required_padding
/* Note that increasing this may decrease performance if --ignore-garbage
   is used, because of the memmove operation below.  */
# define DEC_BLOCKSIZE (1024 * 5)

/* Ensure that BLOCKSIZE is a multiple of 5 and 8.  */
static_assert (ENC_BLOCKSIZE % 40 == 0); /* Padding chars only on last block. */
static_assert (DEC_BLOCKSIZE % 40 == 0); /* Complete encoded blocks are used. */

# define आधारः_encode base32_encode
# define आधारः_decode_context base32_decode_context
# define आधारः_decode_ctx_init base32_decode_ctx_init
# define आधारः_decode_ctx base32_decode_ctx
# define आधारः_decode_ctx_finalize decode_ctx_finalize
# define isubase isubase32
#नोचेद्यदि आधारः_TYPE == 64
# define आधारः_दीर्घता BASE64_दीर्घता
# define REQUIRED_PADDING base64_required_padding
/* Note that increasing this may decrease performance if --ignore-garbage
   is used, because of the memmove operation below.  */
# define DEC_BLOCKSIZE (1024 * 3)

/* Ensure that BLOCKSIZE is a multiple of 3 and 4.  */
static_assert (ENC_BLOCKSIZE % 12 == 0); /* Padding chars only on last block. */
static_assert (DEC_BLOCKSIZE % 12 == 0); /* Complete encoded blocks are used. */

# define आधारः_encode base64_encode
# define आधारः_decode_context base64_decode_context
# define आधारः_decode_ctx_init base64_decode_ctx_init
# define आधारः_decode_ctx base64_decode_ctx
# define आधारः_decode_ctx_finalize decode_ctx_finalize
# define isubase isubase64
#नोचेद्यदि आधारः_TYPE == 42


# define आधारः_दीर्घता आधारः_दीर्घता_2
# define REQUIRED_PADDING required_padding

/* Note that increasing this may decrease performance if --ignore-garbage
   is used, because of the memmove operation below.  */
# define DEC_BLOCKSIZE (4200)
static_assert (DEC_BLOCKSIZE % 40 == 0); /* complete encoded blocks for base32*/
static_assert (DEC_BLOCKSIZE % 12 == 0); /* complete encoded blocks for base64*/

स्थिर idx_t (*आधारः_दीर्घता_2) (idx_t len);
स्थिर पूर्णाङ्क (*required_padding) (पूर्णाङ्क i);
स्थिर bool (*isubase) (अचिह्नित वर्ण ch);
स्थिर शून्य (*आधारः_encode) (वर्ण ध्रुव *सीमित अन्तर्, idx_t inlen,
                            वर्ण *सीमित बहिर्गमनम्, idx_t outlen);

संरचना base16_decode_context
{
  /* Either a 4-bit nibble, or negative if we have no nibble.  */
  चिह्नित वर्ण nibble;
};

संरचना z85_decode_context
{
  पूर्णाङ्क i;
  अचिह्नित वर्ण octets[5];
};

संरचना base58_context
{
  idx_t आकार;
  idx_t क्षमता;
  अचिह्नित वर्ण *buf COUNTED_BY (क्षमता);
};

संरचना base2_decode_context
{
  अचिह्नित वर्ण octet;
  पूर्णाङ्क bit_pos;
};

संरचना आधारः_decode_context
{
  सङ्घ {
    संरचना base64_decode_context base64;
    संरचना base32_decode_context base32;
    संरचना base16_decode_context base16;
    संरचना base2_decode_context base2;
    संरचना z85_decode_context z85;
    संरचना base58_context base58;
  } ctx;
  वर्ण *inbuf;
  idx_t bufsize;
};
स्थिर शून्य (*आधारः_decode_ctx_init) (संरचना आधारः_decode_context *ctx);
स्थिर bool (*आधारः_decode_ctx) (संरचना आधारः_decode_context *ctx,
                                वर्ण ध्रुव *सीमित अन्तर्, idx_t inlen,
                                वर्ण *सीमित बहिर्गमनम्, idx_t *outlen);
स्थिर bool (*आधारः_decode_ctx_finalize) (संरचना आधारः_decode_context *ctx,
                                         वर्ण *सीमित *बहिर्गमनम्, idx_t *outlen);

संरचना आधारः_encode_context
{
  सङ्घ {
    संरचना base58_context base58;
  } ctx;
};

स्थिर शून्य (*आधारः_encode_ctx_init) (संरचना आधारः_encode_context *ctx);
स्थिर bool (*आधारः_encode_ctx) (संरचना आधारः_encode_context *ctx,
                                वर्ण ध्रुव *सीमित अन्तर्, idx_t inlen,
                                वर्ण *सीमित बहिर्गमनम्, idx_t *outlen);
स्थिर bool (*आधारः_encode_ctx_finalize) (संरचना आधारः_encode_context *ctx,
                                         वर्ण *सीमित *बहिर्गमनम्, idx_t *outlen);

स्थिर bool
no_padding (MAYBE_अप्रयुक्तम् संरचना आधारः_decode_context *ctx)
{
  प्रत्यागम false;
}

स्थिर पूर्णाङ्क
no_pending_दीर्घता (MAYBE_अप्रयुक्तम् संरचना आधारः_decode_context *ctx)
{
  प्रत्यागम 0;
}
#यदि_अन्तः

#यदि आधारः_TYPE == 42
स्थिर bool (*has_padding) (संरचना आधारः_decode_context *ctx);
स्थिर पूर्णाङ्क (*get_pending_दीर्घता) (संरचना आधारः_decode_context *ctx);

स्थिर bool
base64_ctx_has_padding (संरचना आधारः_decode_context *ctx)
{
  प्रत्यागम ctx->ctx.base64.i && ctx->ctx.base64.buf[ctx->ctx.base64.i - 1] == '=';
}

स्थिर bool
base32_ctx_has_padding (संरचना आधारः_decode_context *ctx)
{
  प्रत्यागम ctx->ctx.base32.i && ctx->ctx.base32.buf[ctx->ctx.base32.i - 1] == '=';
}

स्थिर पूर्णाङ्क
base64_ctx_get_pending_दीर्घता (संरचना आधारः_decode_context *ctx)
{
  प्रत्यागम ctx->ctx.base64.i;
}

स्थिर पूर्णाङ्क
base32_ctx_get_pending_दीर्घता (संरचना आधारः_decode_context *ctx)
{
  प्रत्यागम ctx->ctx.base32.i;
}

स्थिर पूर्णाङ्क
base16_ctx_get_pending_दीर्घता (MAYBE_अप्रयुक्तम् संरचना आधारः_decode_context *ctx)
{
  प्रत्यागम 1; /* Always check nibble state.  */
}

स्थिर पूर्णाङ्क
z85_ctx_get_pending_दीर्घता (संरचना आधारः_decode_context *ctx)
{
  प्रत्यागम ctx->ctx.z85.i;
}

स्थिर पूर्णाङ्क
base2_ctx_get_pending_दीर्घता (संरचना आधारः_decode_context *ctx)
{
  प्रत्यागम ctx->ctx.base2.bit_pos;
}
#अन्यथा
स्थिर bool
has_padding (संरचना आधारः_decode_context *ctx)
{
  प्रत्यागम ctx->i && ctx->buf[ctx->i - 1] == '=';
}

स्थिर पूर्णाङ्क
get_pending_दीर्घता (संरचना आधारः_decode_context *ctx)
{
  प्रत्यागम ctx->i;
}
#यदि_अन्तः


/* Process any pending data in CTX, while auto padding if appropriate.
   Return TRUE on success, FALSE on failure.  */

स्थिर bool
decode_ctx_finalize (संरचना आधारः_decode_context *ctx,
                     वर्ण *सीमित *बहिर्गमनम्, idx_t *outlen)
{
  यदि (get_pending_दीर्घता (ctx) == 0)
    {
      *outlen = 0;
      प्रत्यागम true;
    }

  /* Auto-pad input and flush the context */
  वर्ण padbuf[8] ATTRIBUTE_NONSTRING = "========";
  idx_t pending_len = get_pending_दीर्घता (ctx);
  idx_t auto_padding = REQUIRED_PADDING (pending_len);
  idx_t n = *outlen;
  bool परिणाम;

  यदि (auto_padding && ! has_padding (ctx))
    {
      affirm (auto_padding <= परिमाण (padbuf));
      परिणाम = आधारः_decode_ctx (ctx, padbuf, auto_padding, *बहिर्गमनम्, &n);
    }
  अन्यथा
    {
      परिणाम = आधारः_decode_ctx (ctx, "", 0, *बहिर्गमनम्, &n);
    }

  *outlen = n;
  प्रत्यागम परिणाम;
}

#यदि आधारः_TYPE == 42

स्थिर idx_t
base64_दीर्घता_wrapper (idx_t len)
{
  प्रत्यागम BASE64_दीर्घता (len);
}

स्थिर शून्य
base64_decode_ctx_init_wrapper (संरचना आधारः_decode_context *ctx)
{
  base64_decode_ctx_init (&ctx->ctx.base64);
}

स्थिर bool
base64_decode_ctx_wrapper (संरचना आधारः_decode_context *ctx,
                           वर्ण ध्रुव *सीमित अन्तर्, idx_t inlen,
                           वर्ण *सीमित बहिर्गमनम्, idx_t *outlen)
{
  प्रत्यागम base64_decode_ctx (&ctx->ctx.base64, अन्तर्, inlen, बहिर्गमनम्, outlen);
}

स्थिर शून्य
init_inbuf (संरचना आधारः_decode_context *ctx)
{
  ctx->bufsize = DEC_BLOCKSIZE;
  ctx->inbuf = xcharalloc (ctx->bufsize);
}

स्थिर शून्य
prepare_inbuf (संरचना आधारः_decode_context *ctx, idx_t inlen)
{
  यदि (ctx->bufsize < inlen)
    ctx->inbuf = xpalloc (ctx->inbuf, &ctx->bufsize,
                          inlen - ctx->bufsize, -1, परिमाण *ctx->inbuf);
}


स्थिर शून्य
base64url_encode (वर्ण ध्रुव *सीमित अन्तर्, idx_t inlen,
                  वर्ण *सीमित बहिर्गमनम्, idx_t outlen)
{
  base64_encode (अन्तर्, inlen, बहिर्गमनम्, outlen);
  /* translate 62nd and 63rd characters */
  वर्ण *p = बहिर्गमनम्;
  यावत् (outlen--)
    {
      यदि (*p == '+')
        *p = '-';
      अन्यथा यदि (*p == '/')
        *p = '_';
      ++p;
    }
}

स्थिर bool
isubase64url (अचिह्नित वर्ण ch)
{
  प्रत्यागम (ch == '-' || ch == '_'
          || (ch != '+' && ch != '/' && isubase64 (ch)));
}

स्थिर शून्य
base64url_decode_ctx_init_wrapper (संरचना आधारः_decode_context *ctx)
{
  base64_decode_ctx_init (&ctx->ctx.base64);
  init_inbuf (ctx);
}


स्थिर bool
base64url_decode_ctx_wrapper (संरचना आधारः_decode_context *ctx,
                              वर्ण ध्रुव *सीमित अन्तर्, idx_t inlen,
                              वर्ण *सीमित बहिर्गमनम्, idx_t *outlen)
{
  prepare_inbuf (ctx, inlen);
  स्मृतिं_प्रतिलिख (ctx->inbuf, अन्तर्, inlen);

  /* translate 62nd and 63rd characters */
  idx_t i = inlen;
  वर्ण *p = ctx->inbuf;
  यावत् (i--)
    {
      यदि (*p == '+' || *p == '/')
        {
          *outlen = 0;
          प्रत्यागम false; /* reject base64 input */
        }
      अन्यथा यदि (*p == '-')
        *p = '+';
      अन्यथा यदि (*p == '_')
        *p = '/';
      ++p;
    }

  प्रत्यागम base64_decode_ctx (&ctx->ctx.base64, ctx->inbuf, inlen,
                            बहिर्गमनम्, outlen);
}



स्थिर idx_t
base32_दीर्घता_wrapper (idx_t len)
{
  प्रत्यागम BASE32_दीर्घता (len);
}

स्थिर शून्य
base32_decode_ctx_init_wrapper (संरचना आधारः_decode_context *ctx)
{
  base32_decode_ctx_init (&ctx->ctx.base32);
}

स्थिर bool
base32_decode_ctx_wrapper (संरचना आधारः_decode_context *ctx,
                           वर्ण ध्रुव *सीमित अन्तर्, idx_t inlen,
                           वर्ण *सीमित बहिर्गमनम्, idx_t *outlen)
{
  प्रत्यागम base32_decode_ctx (&ctx->ctx.base32, अन्तर्, inlen, बहिर्गमनम्, outlen);
}

/* ABCDEFGHIJKLMNOPQRSTUVWXYZ234567
गन्तव्यम्
   0123456789ABCDEFGHIJKLMNOPQRSTUV */
स्थिर ध्रुव वर्ण base32_norm_गन्तव्यम्_hex[32 + 9] = {
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
गन्तव्यम्
   ABCDEFGHIJKLMNOPQRSTUVWXYZ234567 */
स्थिर ध्रुव वर्ण base32_hex_गन्तव्यम्_norm[32 + 9] = {
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


अन्तःस्थ स्थिर bool
isubase32hex (अचिह्नित वर्ण ch)
{
  प्रत्यागम ('0' <= ch && ch <= '9') || ('A' <= ch && ch <= 'V');
}


स्थिर शून्य
base32hex_encode (वर्ण ध्रुव *सीमित अन्तर्, idx_t inlen,
                  वर्ण *सीमित बहिर्गमनम्, idx_t outlen)
{
  base32_encode (अन्तर्, inlen, बहिर्गमनम्, outlen);

  प्रति (वर्ण *p = बहिर्गमनम्; outlen--; p++)
    {
      affirm (0x32 <= *p && *p <= 0x5a);          /* LCOV_EXCL_LINE */
      *p = base32_norm_गन्तव्यम्_hex[*p - 0x32];
    }
}


स्थिर शून्य
base32hex_decode_ctx_init_wrapper (संरचना आधारः_decode_context *ctx)
{
  base32_decode_ctx_init (&ctx->ctx.base32);
  init_inbuf (ctx);
}


स्थिर bool
base32hex_decode_ctx_wrapper (संरचना आधारः_decode_context *ctx,
                              वर्ण ध्रुव *सीमित अन्तर्, idx_t inlen,
                              वर्ण *सीमित बहिर्गमनम्, idx_t *outlen)
{
  prepare_inbuf (ctx, inlen);

  idx_t i = inlen;
  वर्ण *p = ctx->inbuf;
  यावत् (i--)
    {
      यदि (isubase32hex (*अन्तर्))
        *p = base32_hex_गन्तव्यम्_norm[*अन्तर् - 0x30];
      अन्यथा
        *p = *अन्तर्;
      ++p;
      ++अन्तर्;
    }

  प्रत्यागम base32_decode_ctx (&ctx->ctx.base32, ctx->inbuf, inlen,
                            बहिर्गमनम्, outlen);
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

स्थिर चिह्नित वर्ण ध्रुव base16_गन्तव्यम्_int[256] = {
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

स्थिर bool
isubase16 (अचिह्नित वर्ण ch)
{
  प्रत्यागम ch < परिमाण base16_गन्तव्यम्_int && 0 <= base16_गन्तव्यम्_int[ch];
}

स्थिर idx_t
base16_दीर्घता (idx_t len)
{
  प्रत्यागम len * 2;
}


स्थिर शून्य
base16_encode (वर्ण ध्रुव *सीमित अन्तर्, idx_t inlen,
               वर्ण *सीमित बहिर्गमनम्, idx_t outlen)
{
  स्थिर ध्रुव वर्ण base16[16] ATTRIBUTE_NONSTRING = "0123456789ABCDEF";

  यावत् (inlen && outlen)
    {
      अचिह्नित वर्ण c = *अन्तर्;
      *बहिर्गमनम्++ = base16[c >> 4];
      *बहिर्गमनम्++ = base16[c & 0x0F];
      ++अन्तर्;
      inlen--;
      outlen -= 2;
    }
}


स्थिर शून्य
base16_decode_ctx_init (संरचना आधारः_decode_context *ctx)
{
  init_inbuf (ctx);
  ctx->ctx.base16.nibble = -1;
}


स्थिर bool
base16_decode_ctx (संरचना आधारः_decode_context *ctx,
                   वर्ण ध्रुव *सीमित अन्तर्, idx_t inlen,
                   वर्ण *सीमित बहिर्गमनम्, idx_t *outlen)
{
  bool ignore_पङ्क्तयः = true;  /* for now, always ignore them */
  वर्ण *out0 = बहिर्गमनम्;
  चिह्नित वर्ण nibble = ctx->ctx.base16.nibble;

  /* inlen==0 is request to flush output.
     if there is a dangling high nibble - we are missing the low nibble,
     so return false - indicating an invalid input.  */
  यदि (inlen == 0)
    {
      *outlen = 0;
      प्रत्यागम nibble < 0;
    }

  यावत् (inlen--)
    {
      अचिह्नित वर्ण c = *अन्तर्++;
      यदि (ignore_पङ्क्तयः && c == '\n')
        अनुवर्त;

      यदि (परिमाण base16_गन्तव्यम्_int <= c || base16_गन्तव्यम्_int[c] < 0)
        {
          *outlen = बहिर्गमनम् - out0;
          प्रत्यागम false; /* garbage - return false */
        }

      यदि (nibble < 0)
        nibble = base16_गन्तव्यम्_int[c];
      अन्यथा
        {
          /* have both nibbles, write octet */
          *बहिर्गमनम्++ = (nibble << 4) + base16_गन्तव्यम्_int[c];
          nibble = -1;
        }
    }

  ctx->ctx.base16.nibble = nibble;
  *outlen = बहिर्गमनम् - out0;
  प्रत्यागम true;
}



ATTRIBUTE_विशुद्धम्
स्थिर idx_t
z85_दीर्घता (idx_t len)
{
  /* Z85 does not allow padding, so no need to round to highest integer.  */
  idx_t z85_len = (len * 5) / 4;
  affirm (0 <= z85_len);
  प्रत्यागम z85_len;
}

स्थिर bool
isuz85 (अचिह्नित वर्ण ch)
{
  प्रत्यागम c_isalnum (ch) || strchr (".-:+=^!/*?&<>()[]{}@%$#", ch) != रिक्त;
}

स्थिर वर्ण ध्रुव z85_encoding[85] ATTRIBUTE_NONSTRING =
  "0123456789"
  "abcdefghijklmnopqrstuvwxyz"
  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
  ".-:+=^!/*?&<>()[]{}@%$#";

स्थिर शून्य
z85_encode (वर्ण ध्रुव *सीमित अन्तर्, idx_t inlen,
            वर्ण *सीमित बहिर्गमनम्, idx_t outlen)
{
  पूर्णाङ्क i = 0;
  अचिह्नित वर्ण quad[4];
  idx_t outidx = 0;

  यावत् (true)
    {
      यदि (inlen == 0)
        {
          /* no more input, exactly on 4 octet boundary. */
          यदि (i == 0)
            प्रत्यागम;

          /* currently, there's no way to return an error in encoding.  */
          error (EXIT_FAILURE, 0,
                 _("invalid input (length must be multiple of 4 characters)"));
        }
      अन्यथा
        {
          quad[i++] = *अन्तर्++;
          --inlen;
        }

      /* Got a quad, encode it */
      यदि (i == 4)
        {
          int_fast64_t val = quad[0];
          val = (val << 24) + (quad[1] << 16) + (quad[2] << 8) + quad[3];

          प्रति (पूर्णाङ्क j = 4; j >= 0; --j)
            {
              पूर्णाङ्क c = val % 85;
              val /= 85;

              /* NOTE: if there is padding (which is trimmed by z85
                 before outputting the result), the output buffer 'out'
                 might not include enough allocated bytes for the padding,
                 so don't store them. */
              यदि (outidx + j < outlen)
                बहिर्गमनम्[j] = z85_encoding[c];
            }
          बहिर्गमनम् += 5;
          outidx += 5;
          i = 0;
        }
    }
}

स्थिर शून्य
z85_decode_ctx_init (संरचना आधारः_decode_context *ctx)
{
  init_inbuf (ctx);
  ctx->ctx.z85.i = 0;
}


# define Z85_LO_CTX_गन्तव्यम्_32_BIT_VAL(ctx) \
  (((ctx)->ctx.z85.octets[1] * 85 * 85 * 85) +      \
   ((ctx)->ctx.z85.octets[2] * 85 * 85) +	    \
   ((ctx)->ctx.z85.octets[3] * 85) +		    \
   ((ctx)->ctx.z85.octets[4]))


# define Z85_HI_CTX_गन्तव्यम्_32_BIT_VAL(ctx) \
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
स्थिर चिह्नित वर्ण ध्रुव z85_decoding[93] = {
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

स्थिर bool
z85_decode_ctx (संरचना आधारः_decode_context *ctx,
                वर्ण ध्रुव *सीमित अन्तर्, idx_t inlen,
                वर्ण *सीमित बहिर्गमनम्, idx_t *outlen)
{
  bool ignore_पङ्क्तयः = true;  /* for now, always ignore them */

  *outlen = 0;

  /* inlen==0 is request to flush output.
     if there are dangling values - we are missing entries,
     so return false - indicating an invalid input.  */
  यदि (inlen == 0)
    {
      यदि (ctx->ctx.z85.i > 0)
        {
          /* Z85 variant does not allow padding - input must
             be a multiple of 5 - so return error.  */
          प्रत्यागम false;
        }
      प्रत्यागम true;
    }

  यावत् (inlen--)
    {
      यदि (ignore_पङ्क्तयः && *अन्तर् == '\n')
        {
          ++अन्तर्;
          अनुवर्त;
        }

      /* z85 decoding */
      अचिह्नित वर्ण c = *अन्तर्;

      यदि (c >= 33 && c <= 125)
        {
          चिह्नित वर्ण ch = z85_decoding[c - 33];
          यदि (ch < 0)
            प्रत्यागम false; /* garbage - return false */
          c = ch;
        }
      अन्यथा
        प्रत्यागम false; /* garbage - return false */

      ++अन्तर्;

      ctx->ctx.z85.octets[ctx->ctx.z85.i++] = c;
      यदि (ctx->ctx.z85.i == 5)
        {
          /* decode the lowest 4 octets, then check for overflows.  */
          int_fast64_t val = Z85_LO_CTX_गन्तव्यम्_32_BIT_VAL (ctx);

          /* The Z85 spec and the reference implementation say nothing
             about overflows. To be on the safe side, reject them.  */

          val += Z85_HI_CTX_गन्तव्यम्_32_BIT_VAL (ctx);
          यदि ((val >> 24) & ~0xFF)
            प्रत्यागम false;

          *बहिर्गमनम्++ = val >> 24;
          *बहिर्गमनम्++ = (val >> 16) & 0xFF;
          *बहिर्गमनम्++ = (val >> 8) & 0xFF;
          *बहिर्गमनम्++ = val & 0xFF;

          *outlen += 4;

          ctx->ctx.z85.i = 0;
        }
    }
  प्रत्यागम true;
}


अन्तःस्थ स्थिर bool
isubase2 (अचिह्नित वर्ण ch)
{
  प्रत्यागम ch == '0' || ch == '1';
}

स्थिर idx_t
base2_दीर्घता (idx_t len)
{
  प्रत्यागम len * 8;
}


अन्तःस्थ स्थिर शून्य
base2msbf_encode (वर्ण ध्रुव *सीमित अन्तर्, idx_t inlen,
                  वर्ण *सीमित बहिर्गमनम्, idx_t outlen)
{
  यावत् (inlen && outlen)
    {
      अचिह्नित वर्ण c = *अन्तर्;
      प्रति (पूर्णाङ्क i = 0; i < 8; i++)
        {
          *बहिर्गमनम्++ = c & 0x80 ? '1' : '0';
          c <<= 1;
        }
      inlen--;
      outlen -= 8;
      ++अन्तर्;
    }
}

अन्तःस्थ स्थिर शून्य
base2lsbf_encode (वर्ण ध्रुव *सीमित अन्तर्, idx_t inlen,
                  वर्ण *सीमित बहिर्गमनम्, idx_t outlen)
{
  यावत् (inlen && outlen)
    {
      अचिह्नित वर्ण c = *अन्तर्;
      प्रति (पूर्णाङ्क i = 0; i < 8; i++)
        {
          *बहिर्गमनम्++ = c & 0x01 ? '1' : '0';
          c >>= 1;
        }
      inlen--;
      outlen -= 8;
      ++अन्तर्;
    }
}


स्थिर शून्य
base2_decode_ctx_init (संरचना आधारः_decode_context *ctx)
{
  init_inbuf (ctx);
  ctx->ctx.base2.octet = 0;
  ctx->ctx.base2.bit_pos = 0;
}


स्थिर bool
base2lsbf_decode_ctx (संरचना आधारः_decode_context *ctx,
                      वर्ण ध्रुव *सीमित अन्तर्, idx_t inlen,
                      वर्ण *सीमित बहिर्गमनम्, idx_t *outlen)
{
  bool ignore_पङ्क्तयः = true;  /* for now, always ignore them */

  *outlen = 0;

  /* inlen==0 is request to flush output.
     if there is a dangling bit - we are missing some bits,
     so return false - indicating an invalid input.  */
  यदि (inlen == 0)
    प्रत्यागम ctx->ctx.base2.bit_pos == 0;

  यावत् (inlen--)
    {
      यदि (ignore_पङ्क्तयः && *अन्तर् == '\n')
        {
          ++अन्तर्;
          अनुवर्त;
        }

      यदि (!isubase2 (*अन्तर्))
        प्रत्यागम false;

      bool bit = (*अन्तर् == '1');
      ctx->ctx.base2.octet |= bit << ctx->ctx.base2.bit_pos;
      ++ctx->ctx.base2.bit_pos;

      यदि (ctx->ctx.base2.bit_pos == 8)
        {
          *बहिर्गमनम्++ = ctx->ctx.base2.octet;
          ctx->ctx.base2.octet = 0;
          ++*outlen;
          ctx->ctx.base2.bit_pos = 0;
        }

      ++अन्तर्;
    }

  प्रत्यागम true;
}

स्थिर bool
base2msbf_decode_ctx (संरचना आधारः_decode_context *ctx,
                      वर्ण ध्रुव *सीमित अन्तर्, idx_t inlen,
                      वर्ण *सीमित बहिर्गमनम्, idx_t *outlen)
{
  bool ignore_पङ्क्तयः = true;  /* for now, always ignore them */

  *outlen = 0;

  /* inlen==0 is request to flush output.
     if there is a dangling bit - we are missing some bits,
     so return false - indicating an invalid input.  */
  यदि (inlen == 0)
    प्रत्यागम ctx->ctx.base2.bit_pos == 0;

  यावत् (inlen--)
    {
      यदि (ignore_पङ्क्तयः && *अन्तर् == '\n')
        {
          ++अन्तर्;
          अनुवर्त;
        }

      यदि (!isubase2 (*अन्तर्))
        प्रत्यागम false;

      bool bit = (*अन्तर् == '1');
      यदि (ctx->ctx.base2.bit_pos == 0)
        ctx->ctx.base2.bit_pos = 8;
      --ctx->ctx.base2.bit_pos;
      ctx->ctx.base2.octet |= bit << ctx->ctx.base2.bit_pos;

      यदि (ctx->ctx.base2.bit_pos == 0)
        {
          *बहिर्गमनम्++ = ctx->ctx.base2.octet;
          ctx->ctx.base2.octet = 0;
          ++*outlen;
        }

      ++अन्तर्;
    }

  प्रत्यागम true;
}

/* Map from GMP (up to base 62):
   "0123456789 ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuv";
   to base 58:
   "123456789A BCDEFGHJKLMNPQRSTUVWXYZabc defghijkmnopqrstuvwxyz";  */
स्थिर चिह्नित वर्ण ध्रुव gmp_गन्तव्यम्_base58[256] = {
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

स्थिर चिह्नित वर्ण ध्रुव base58_गन्तव्यम्_gmp[256] = {
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

स्थिर bool
isubase58 (अचिह्नित वर्ण ch)
{
  प्रत्यागम ch < परिमाण base58_गन्तव्यम्_gmp && 0 <= base58_गन्तव्यम्_gmp[ch];
}


ATTRIBUTE_विशुद्धम्
स्थिर idx_t
base58_दीर्घता (idx_t len)
{
  /* Base58 output length is approximately log(256)/log(58),
     which is approximately len * 138 / 100,
     which is at most ((len + 100 - 1) / 100) * 138
     +1 to ensure we've enough place for NUL  */
  idx_t base58_len = ((len + 99) / 100) * 138 + 1;
  affirm (0 < base58_len);
  प्रत्यागम base58_len;
}


स्थिर शून्य
base58_encode_ctx_init (संरचना आधारः_encode_context *ctx)
{
  ctx->ctx.base58.buf = रिक्त;
  ctx->ctx.base58.आकार = 0;
  ctx->ctx.base58.क्षमता = 0;
}


स्थिर bool
base58_encode_ctx (संरचना आधारः_encode_context *ctx,
                   वर्ण ध्रुव *सीमित अन्तर्, idx_t inlen,
                   MAYBE_अप्रयुक्तम् वर्ण *सीमित बहिर्गमनम्, idx_t *outlen)
{
  *outlen = 0;   /* Only accumulate input in this function.  */

  यदि (inlen == 0)
    प्रत्यागम true;

  idx_t free_space = ctx->ctx.base58.क्षमता - ctx->ctx.base58.आकार;
  यदि (free_space < inlen)
    {
      ctx->ctx.base58.buf = xpalloc (ctx->ctx.base58.buf,
                                     &ctx->ctx.base58.क्षमता,
                                     inlen - free_space,
                                     -1, परिमाण *ctx->ctx.base58.buf);
    }

  स्मृतिं_प्रतिलिख (ctx->ctx.base58.buf + ctx->ctx.base58.आकार, अन्तर्, inlen);
  ctx->ctx.base58.आकार += inlen;

  प्रत्यागम true;
}

स्थिर शून्य
base58_encode (वर्ण ध्रुव *दत्त, size_t दत्त_len,
               वर्ण *बहिर्गमनम्, idx_t *outlen)
{
  affirm (आधारः_दीर्घता_2 (दत्त_len) <= *outlen);

  size_t zeros = 0;
  यावत् (zeros < दत्त_len && दत्त[zeros] == 0)
    zeros++;

  स्मृतिं_पूरय (बहिर्गमनम्, '1', zeros);
  वर्ण *p = बहिर्गमनम् + zeros;

  /* Use GMP to convert from base 256 to base 58.  */
  mpz_t अङ्कः;
  mpz_init (अङ्कः);
  यदि (दत्त_len - zeros)
    {
      mpz_import (अङ्कः, दत्त_len - zeros, 1, 1, 0, 0, दत्त + zeros);
      प्रति (p = mpz_get_str (p, 58, अङ्कः); *p; p++)
        *p = gmp_गन्तव्यम्_base58[गन्तव्यम्_uchar (*p)];
    }
  mpz_clear (अङ्कः);

  *outlen = p - बहिर्गमनम्;
}


स्थिर bool
base58_encode_ctx_finalize (संरचना आधारः_encode_context *ctx,
                            वर्ण *सीमित *बहिर्गमनम्, idx_t *outlen)
{
  /* Ensure output buffer is large enough.  */
  idx_t max_outlen = आधारः_दीर्घता_2 (ctx->ctx.base58.आकार);
  यदि (max_outlen > *outlen)
    {
      *बहिर्गमनम् = xrealloc (*बहिर्गमनम्, max_outlen);
      *outlen = max_outlen;
    }

  base58_encode ((वर्ण *)ctx->ctx.base58.buf, ctx->ctx.base58.आकार,
                 *बहिर्गमनम्, outlen);

  मुक्त (ctx->ctx.base58.buf);
  ctx->ctx.base58.buf = रिक्त;
  ctx->ctx.base58.आकार = 0;
  ctx->ctx.base58.क्षमता = 0;

  प्रत्यागम true;
}


स्थिर शून्य
base58_decode_ctx_init (संरचना आधारः_decode_context *ctx)
{
  ctx->ctx.base58.आकार = 0;
  ctx->ctx.base58.क्षमता = 0;
  ctx->ctx.base58.buf = रिक्त;
}

स्थिर bool
base58_decode_ctx (संरचना आधारः_decode_context *ctx,
                   वर्ण ध्रुव *सीमित अन्तर्, idx_t inlen,
                   MAYBE_अप्रयुक्तम् वर्ण *सीमित बहिर्गमनम्, idx_t *outlen)
{
  bool ignore_पङ्क्तयः = true;  /* for now, always ignore them */

  *outlen = 0;   /* Only accumulate input in this function.  */

  यदि (inlen == 0)
    प्रत्यागम true;

  idx_t free_space = ctx->ctx.base58.क्षमता - ctx->ctx.base58.आकार;
  free_space -= 1;  /* Ensure we leave space for NUL (for mpz_set_str).  */
  यदि (free_space < inlen)
    {
      ctx->ctx.base58.buf = xpalloc (ctx->ctx.base58.buf,
                                     &ctx->ctx.base58.क्षमता,
                                     inlen - free_space,
                                     -1, परिमाण *ctx->ctx.base58.buf);
    }

  /* Accumulate all valid input characters in our buffer.
     Note we don't rely on mpz_set_str() for validation
     as that allows (skips) all whitespace.  */
  प्रति (idx_t i = 0; i < inlen; i++)
    {
      अचिह्नित वर्ण c = अन्तर्[i];

      यदि (ignore_पङ्क्तयः && c == '\n')
        अनुवर्त;

      यदि (!isubase58 (c))
        प्रत्यागम false;

      ctx->ctx.base58.buf[ctx->ctx.base58.आकार++] = base58_गन्तव्यम्_gmp[गन्तव्यम्_uchar (c)];
    }

  प्रत्यागम true;
}


स्थिर bool
base58_decode (वर्ण ध्रुव *दत्त, size_t दत्त_len,
               वर्ण *सीमित बहिर्गमनम्, idx_t *outlen)
{
  affirm (दत्त_len <= *outlen);

  size_t ones = 0;
  यावत् (ones < दत्त_len && दत्त[ones] == base58_गन्तव्यम्_gmp['1'])
    ones++;

  स्मृतिं_पूरय (बहिर्गमनम्, 0, ones);

  /* Use GMP to convert from base 58 to base 256.  */
  mpz_t अङ्कः;
  mpz_init (अङ्कः);

  यदि ((दत्त_len - ones) && mpz_समुच्चयः_str (अङ्कः, दत्त + ones, 58) != 0)
    {
      mpz_clear (अङ्कः);
      *outlen = 0;
      प्रत्यागम false;
    }

  size_t exported_आकार = 0;
  यदि (दत्त_len - ones)
    {
      size_t द्विमान_आकार = (mpz_sizeinbase (अङ्कः, 2) + 7) / 8;
      affirm (*outlen - ones >= द्विमान_आकार);
      mpz_export (बहिर्गमनम् + ones, &exported_आकार, 1, 1, 0, 0, अङ्कः);
    }

  mpz_clear (अङ्कः);
  *outlen = ones + exported_आकार;
  प्रत्यागम true;
}


स्थिर bool
base58_decode_ctx_finalize (संरचना आधारः_decode_context *ctx,
                            वर्ण *सीमित *बहिर्गमनम्, idx_t *outlen)
{
  /* Ensure output buffer is large enough.
     Worst case is input is all '1's.  */
  idx_t max_outlen = ctx->ctx.base58.आकार;
  यदि (max_outlen > *outlen)
    {
      *बहिर्गमनम् = xrealloc (*बहिर्गमनम्, max_outlen);
      *outlen = max_outlen;
    }

  /* Ensure input buffer is NUL terminated (for mpz_get_str).  */
  यदि (ctx->ctx.base58.आकार)
    ctx->ctx.base58.buf[ctx->ctx.base58.आकार] = '\0';

  bool ret = base58_decode ((वर्ण *)ctx->ctx.base58.buf, ctx->ctx.base58.आकार,
                            *बहिर्गमनम्, outlen);

  मुक्त (ctx->ctx.base58.buf);
  ctx->ctx.base58.buf = रिक्त;
  ctx->ctx.base58.आकार = 0;
  ctx->ctx.base58.क्षमता = 0;

  प्रत्यागम ret;
}

#यदि_अन्तः /* BASE_TYPE == 42, i.e., "basenc"*/



स्थिर शून्य
wrap_write (वर्ण ध्रुव *बफर, idx_t len,
            idx_t wrap_column, idx_t *current_column, FILE *बहिर्गमनम्)
{
  यदि (wrap_column == 0)
    {
      /* Simple write. */
      यदि (fwrite (बफर, 1, len, stdout) < len)
        write_error ();
    }
  अन्यथा
    प्रति (idx_t written = 0; written < len; )
      {
        idx_t गन्तव्यम्_write = MIN (wrap_column - *current_column, len - written);

        यदि (गन्तव्यम्_write == 0)
          {
            यदि (fputc ('\n', बहिर्गमनम्) == EOF)
              write_error ();
            *current_column = 0;
          }
        अन्यथा
          {
            यदि (fwrite (बफर + written, 1, गन्तव्यम्_write, stdout) < गन्तव्यम्_write)
              write_error ();
            *current_column += गन्तव्यम्_write;
            written += गन्तव्यम्_write;
          }
      }
}

स्थिर _Noreturn शून्य
finish_and_exit (FILE *अन्तर्, वर्ण ध्रुव *infile)
{
  यदि (fclose (अन्तर्) != 0)
    {
      यदि (streq (infile, "-"))
        error (EXIT_FAILURE, errno, _("closing standard input"));
      अन्यथा
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
    }

  निर्गच्छ (EXIT_SUCCESS);
}

स्थिर _Noreturn शून्य
do_encode (FILE *अन्तर्, वर्ण ध्रुव *infile, FILE *बहिर्गमनम्, idx_t wrap_column)
{
  idx_t current_column = 0;
  वर्ण *inbuf, *outbuf;
  idx_t योग;

  inbuf = xmalloc (ENC_BLOCKSIZE);
  outbuf = xmalloc (आधारः_दीर्घता (ENC_BLOCKSIZE));

#यदि आधारः_TYPE == 42
  /* Initialize encoding context if needed (for base58) */
  संरचना आधारः_encode_context encode_ctx;
  bool use_ctx = (आधारः_encode_ctx_init != रिक्त);
  यदि (use_ctx)
    आधारः_encode_ctx_init (&encode_ctx);
#यदि_अन्तः

  कुरु
    {
      idx_t n;

      योग = 0;
      कुरु
        {
          n = fread (inbuf + योग, 1, ENC_BLOCKSIZE - योग, अन्तर्);
          योग += n;
        }
      यावत् (!feof (अन्तर्) && !ferror (अन्तर्) && योग < ENC_BLOCKSIZE);

      यदि (योग > 0)
        {
#यदि आधारः_TYPE == 42
          यदि (use_ctx)
            {
              idx_t outlen = 0;
              आधारः_encode_ctx (&encode_ctx, inbuf, योग, outbuf, &outlen);

              wrap_write (outbuf, outlen, wrap_column, &current_column, बहिर्गमनम्);
            }
          अन्यथा
#यदि_अन्तः
            {
              /* Process input one block at a time.  Note that ENC_BLOCKSIZE
                 is sized so that no pad chars will appear in output. */
              आधारः_encode (inbuf, योग, outbuf, आधारः_दीर्घता (योग));

              wrap_write (outbuf, आधारः_दीर्घता (योग), wrap_column,
                          &current_column, बहिर्गमनम्);
            }
        }
    }
  यावत् (!feof (अन्तर्) && !ferror (अन्तर्) && योग == ENC_BLOCKSIZE);

#यदि आधारः_TYPE == 42
  यदि (use_ctx && आधारः_encode_ctx_finalize)
    {
      idx_t outlen = आधारः_दीर्घता (ENC_BLOCKSIZE);
      आधारः_encode_ctx_finalize (&encode_ctx, &outbuf, &outlen);

      wrap_write (outbuf, outlen, wrap_column, &current_column, बहिर्गमनम्);
    }
#यदि_अन्तः

  /* When wrapping, terminate last line. */
  यदि (wrap_column && current_column > 0 && fputc ('\n', बहिर्गमनम्) == EOF)
    write_error ();

  यदि (ferror (अन्तर्))
    error (EXIT_FAILURE, errno, _("read error"));

  finish_and_exit (अन्तर्, infile);
}

स्थिर _Noreturn शून्य
do_decode (FILE *अन्तर्, वर्ण ध्रुव *infile, FILE *बहिर्गमनम्, bool ignore_garbage)
{
  वर्ण *inbuf, *outbuf;
  idx_t योग;
  संरचना आधारः_decode_context ctx;

  inbuf = xmalloc (आधारः_दीर्घता (DEC_BLOCKSIZE));
  outbuf = xmalloc (DEC_BLOCKSIZE);

#यदि आधारः_TYPE == 42
  ctx.inbuf = रिक्त;
#यदि_अन्तः
  आधारः_decode_ctx_init (&ctx);

  कुरु
    {
      bool ok;

      योग = 0;
      कुरु
        {
          idx_t n = fread (inbuf + योग,
                           1, आधारः_दीर्घता (DEC_BLOCKSIZE) - योग, अन्तर्);

          यदि (ignore_garbage)
            {
              प्रति (idx_t i = 0; n > 0 && i < n;)
                {
                  यदि (isubase (inbuf[योग + i])
                      || (REQUIRED_PADDING (1) && inbuf[योग + i] == '='))
                    i++;
                  अन्यथा
                    memmove (inbuf + योग + i, inbuf + योग + i + 1, --n - i);
                }
            }

          योग += n;

          यदि (ferror (अन्तर्))
            error (EXIT_FAILURE, errno, _("read error"));
        }
      यावत् (योग < आधारः_दीर्घता (DEC_BLOCKSIZE) && !feof (अन्तर्));

      यावत् (योग || feof (अन्तर्))
        {
          idx_t n = DEC_BLOCKSIZE;
          यदि (योग)
            ok = आधारः_decode_ctx (&ctx, inbuf, योग, outbuf, &n);
          अन्यथा
            ok = आधारः_decode_ctx_finalize (&ctx, &outbuf, &n);

          यदि (fwrite (outbuf, 1, n, बहिर्गमनम्) < n)
            write_error ();

          यदि (! ok)
            error (EXIT_FAILURE, 0, _("invalid input"));

          यदि (योग == 0)
            भङ्ग;
          योग = 0;
        }
    }
  यावत् (!feof (अन्तर्));

  finish_and_exit (अन्तर्, infile);
}

पूर्णाङ्क
मुख्यम् (पूर्णाङ्क argc, वर्ण **argv)
{
  पूर्णाङ्क opt;
  FILE *निवेशः_fh;
  वर्ण ध्रुव *infile;

  /* True if --decode has been given and we should decode data. */
  bool decode = false;
  /* True if we should ignore non-base-alphabetic characters. */
  bool ignore_garbage = false;
  /* Wrap encoded data around the 76th column, by default. */
  idx_t wrap_column = 76;

#यदि आधारः_TYPE == 42
  पूर्णाङ्क आधारः_type = 0;
#यदि_अन्तः

  initialize_main (&argc, &argv);
  समुच्चयः_क्रमादेशः_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  यावत् ((opt = getopt_long (argc, argv, "diw:", long_options, रिक्त)) != -1)
    विकल्प (opt)
      {
      पक्ष 'd':
        decode = true;
        भङ्ग;

      पक्ष 'w':
        {
          intmax_t w;
          strtol_error s_err = xstrtoimax (optarg, रिक्त, 10, &w, "");
          यदि (LONGINT_अतिप्रवाहः < s_err || w < 0)
            error (EXIT_FAILURE, 0, "%s: %s",
                   _("invalid wrap size"), quote (optarg));
          wrap_column = s_err == LONGINT_अतिप्रवाहः || IDX_MAX < w ? 0 : w;
        }
        भङ्ग;

      पक्ष 'i':
        ignore_garbage = true;
        भङ्ग;

#यदि आधारः_TYPE == 42
      पक्ष BASE64_OPTION:
      पक्ष BASE64URL_OPTION:
      पक्ष BASE32_OPTION:
      पक्ष BASE32HEX_OPTION:
      पक्ष BASE16_OPTION:
      पक्ष BASE2MSBF_OPTION:
      पक्ष BASE2LSBF_OPTION:
      पक्ष Z85_OPTION:
      पक्ष BASE58_OPTION:
        आधारः_type = opt;
        भङ्ग;
#यदि_अन्तः

      case_GETOPT_HELP_CHAR;

      case_GETOPT_संस्करणम्_CHAR (क्रमादेशः_NAME, AUTHORS);

      स्वभाव:
        usage (EXIT_FAILURE);
        भङ्ग;
      }

#यदि आधारः_TYPE == 42
  required_padding = no_required_padding;
  has_padding = no_padding;
  get_pending_दीर्घता = no_pending_दीर्घता;
  आधारः_decode_ctx_finalize = decode_ctx_finalize;

  विकल्प (आधारः_type)
    {
    पक्ष BASE64_OPTION:
      आधारः_दीर्घता_2 = base64_दीर्घता_wrapper;
      required_padding = base64_required_padding;
      has_padding = base64_ctx_has_padding;
      get_pending_दीर्घता = base64_ctx_get_pending_दीर्घता;
      isubase = isubase64;
      आधारः_encode = base64_encode;
      आधारः_decode_ctx_init = base64_decode_ctx_init_wrapper;
      आधारः_decode_ctx = base64_decode_ctx_wrapper;
      भङ्ग;

    पक्ष BASE64URL_OPTION:
      आधारः_दीर्घता_2 = base64_दीर्घता_wrapper;
      required_padding = base64_required_padding;
      has_padding = base64_ctx_has_padding;
      get_pending_दीर्घता = base64_ctx_get_pending_दीर्घता;
      isubase = isubase64url;
      आधारः_encode = base64url_encode;
      आधारः_decode_ctx_init = base64url_decode_ctx_init_wrapper;
      आधारः_decode_ctx = base64url_decode_ctx_wrapper;
      भङ्ग;

    पक्ष BASE32_OPTION:
      आधारः_दीर्घता_2 = base32_दीर्घता_wrapper;
      required_padding = base32_required_padding;
      has_padding = base32_ctx_has_padding;
      get_pending_दीर्घता = base32_ctx_get_pending_दीर्घता;
      isubase = isubase32;
      आधारः_encode = base32_encode;
      आधारः_decode_ctx_init = base32_decode_ctx_init_wrapper;
      आधारः_decode_ctx = base32_decode_ctx_wrapper;
      भङ्ग;

    पक्ष BASE32HEX_OPTION:
      आधारः_दीर्घता_2 = base32_दीर्घता_wrapper;
      required_padding = base32_required_padding;
      has_padding = base32_ctx_has_padding;
      get_pending_दीर्घता = base32_ctx_get_pending_दीर्घता;
      isubase = isubase32hex;
      आधारः_encode = base32hex_encode;
      आधारः_decode_ctx_init = base32hex_decode_ctx_init_wrapper;
      आधारः_decode_ctx = base32hex_decode_ctx_wrapper;
      भङ्ग;

    पक्ष BASE16_OPTION:
      आधारः_दीर्घता_2 = base16_दीर्घता;
      get_pending_दीर्घता = base16_ctx_get_pending_दीर्घता;
      isubase = isubase16;
      आधारः_encode = base16_encode;
      आधारः_decode_ctx_init = base16_decode_ctx_init;
      आधारः_decode_ctx = base16_decode_ctx;
      भङ्ग;

    पक्ष BASE2MSBF_OPTION:
      आधारः_दीर्घता_2 = base2_दीर्घता;
      get_pending_दीर्घता = base2_ctx_get_pending_दीर्घता;
      isubase = isubase2;
      आधारः_encode = base2msbf_encode;
      आधारः_decode_ctx_init = base2_decode_ctx_init;
      आधारः_decode_ctx = base2msbf_decode_ctx;
      भङ्ग;

    पक्ष BASE2LSBF_OPTION:
      आधारः_दीर्घता_2 = base2_दीर्घता;
      get_pending_दीर्घता = base2_ctx_get_pending_दीर्घता;
      isubase = isubase2;
      आधारः_encode = base2lsbf_encode;
      आधारः_decode_ctx_init = base2_decode_ctx_init;
      आधारः_decode_ctx = base2lsbf_decode_ctx;
      भङ्ग;

    पक्ष Z85_OPTION:
      आधारः_दीर्घता_2 = z85_दीर्घता;
      get_pending_दीर्घता = z85_ctx_get_pending_दीर्घता;
      isubase = isuz85;
      आधारः_encode = z85_encode;
      आधारः_decode_ctx_init = z85_decode_ctx_init;
      आधारः_decode_ctx = z85_decode_ctx;
      भङ्ग;

    पक्ष BASE58_OPTION:
      आधारः_दीर्घता_2 = base58_दीर्घता;
      isubase = isubase58;
      आधारः_encode_ctx_init = base58_encode_ctx_init;
      आधारः_encode_ctx = base58_encode_ctx;
      आधारः_encode_ctx_finalize = base58_encode_ctx_finalize;
      आधारः_decode_ctx_init = base58_decode_ctx_init;
      आधारः_decode_ctx = base58_decode_ctx;
      आधारः_decode_ctx_finalize = base58_decode_ctx_finalize;
      भङ्ग;

    स्वभाव:
      error (0, 0, _("missing encoding type"));
      usage (EXIT_FAILURE);
    }
#यदि_अन्तः

  यदि (argc - optind > 1)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }

  यदि (optind < argc)
    infile = argv[optind];
  अन्यथा
    infile = "-";

  यदि (streq (infile, "-"))
    {
      xset_द्विमान_बहुलकम् (STDIN_FILENO, O_द्विमान);
      निवेशः_fh = stdin;
    }
  अन्यथा
    {
      निवेशः_fh = fopen (infile, "rb");
      यदि (निवेशः_fh == रिक्त)
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
    }

  fadvise (निवेशः_fh, FADVISE_SEQUENTIAL);

  यदि (decode)
    do_decode (निवेशः_fh, infile, stdout, ignore_garbage);
  अन्यथा
    do_encode (निवेशः_fh, infile, stdout, wrap_column);
}
