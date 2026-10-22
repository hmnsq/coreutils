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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <gmp.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "assure.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "c-ctype.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "fadvise.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xstrtol.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xbinary-io.h"

#ਜੇ ਅਧਾਰ_TYPE == 42
# define AUTHORS \
  proper_name ("Simon Josefsson"), \
  proper_name ("Assaf Gordon")
#ਨਹੀਂਤਾਂ
# define AUTHORS proper_name ("Simon Josefsson")
#ਜੇ_ਅੰਤ

#ਜੇ ਅਧਾਰ_TYPE == 32
# include "base32.h"
# define ਪ੍ਰੋਗਰਾਮ_NAME "base32"
#ਜਾਂਜੇ ਅਧਾਰ_TYPE == 64
# include "base64.h"
# define ਪ੍ਰੋਗਰਾਮ_NAME "base64"
#ਜਾਂਜੇ ਅਧਾਰ_TYPE == 42
# include "base32.h"
# include "base64.h"
# include "assure.h"
# define ਪ੍ਰੋਗਰਾਮ_NAME "basenc"
#ਨਹੀਂਤਾਂ
# error missing/invalid ਅਧਾਰ_TYPE definition
#ਜੇ_ਅੰਤ



#ਜੇ ਅਧਾਰ_TYPE == 42
ਗਿਣਤੀ
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
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ long_options[] =
{
  {"decode", no_ਦਲੀਲ, ਨਿਰਾ, 'd'},
  {"wrap", required_ਦਲੀਲ, ਨਿਰਾ, 'w'},
  {"ignore-garbage", no_ਦਲੀਲ, ਨਿਰਾ, 'i'},
#ਜੇ ਅਧਾਰ_TYPE == 42
  {"base64",    no_ਦਲੀਲ, ਨਿਰਾ, BASE64_OPTION},
  {"base64url", no_ਦਲੀਲ, ਨਿਰਾ, BASE64URL_OPTION},
  {"base58",    no_ਦਲੀਲ, ਨਿਰਾ, BASE58_OPTION},
  {"base32",    no_ਦਲੀਲ, ਨਿਰਾ, BASE32_OPTION},
  {"base32hex", no_ਦਲੀਲ, ਨਿਰਾ, BASE32HEX_OPTION},
  {"base16",    no_ਦਲੀਲ, ਨਿਰਾ, BASE16_OPTION},
  {"base2msbf", no_ਦਲੀਲ, ਨਿਰਾ, BASE2MSBF_OPTION},
  {"base2lsbf", no_ਦਲੀਲ, ਨਿਰਾ, BASE2LSBF_OPTION},
  {"z85",       no_ਦਲੀਲ, ਨਿਰਾ, Z85_OPTION},
#ਜੇ_ਅੰਤ
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("\
Usage: %s [OPTION]... [FILE]\n\
"), ਪ੍ਰੋਗਰਾਮ_name);

#ਜੇ ਅਧਾਰ_TYPE == 42
      fputs (_("\
basenc encode or decode FILE, or standard ਨਿਵੇਸ਼, ਮੰਜ਼ਿਲ standard output.\n\
"), stdout);
#ਨਹੀਂਤਾਂ
      ਲਿਖੋ (_("\
ਅਧਾਰ%d encode or decode FILE, or standard ਨਿਵੇਸ਼, ਮੰਜ਼ਿਲ standard output.\n\
"), ਅਧਾਰ_TYPE);
#ਜੇ_ਅੰਤ

      emit_stdin_note ();
      emit_mandatory_arg_note ();
#ਜੇ ਅਧਾਰ_TYPE == 42
      oputs (_("\
      --base64\n\
         same as base64(1) ਪ੍ਰੋਗਰਾਮ (RFC4648 section 4)\n\
"));
      oputs (_("\
      --base64url\n\
         file- and url-ਸੁਰੱਖਿਅਤ_2 base64 (RFC4648 section 5)\n\
"));
      oputs (_("\
      --base58\n\
         visually unambiguous base58 encoding\n\
"));
      oputs (_("\
      --base32\n\
         same as base32(1) ਪ੍ਰੋਗਰਾਮ (RFC4648 section 6)\n\
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
         ਦੋਆਧਾਰੀ_ਅੰਕ string with most significant ਦੋਆਧਾਰੀ_ਅੰਕ (msb) ਪਹਿਲਾ\n\
"));
      oputs (_("\
      --base2lsbf\n\
         ਦੋਆਧਾਰੀ_ਅੰਕ string with ਘੱਟ_ਤੋਂ_ਘੱਟ significant ਦੋਆਧਾਰੀ_ਅੰਕ (lsb) ਪਹਿਲਾ\n\
"));
#ਜੇ_ਅੰਤ
      oputs (_("\
  -d, --decode\n\
         decode ਅੰਕੜੇ\n\
"));
      oputs (_("\
  -i, --ignore-garbage\n\
         when decoding, ignore non-alphabet characters\n\
"));
      oputs (_("\
  -w, --wrap=ਖਾਨੇ_ਗਿਣਤੀ\n\
         wrap encoded ਸਤਰਾਂ after ਖਾਨੇ_ਗਿਣਤੀ character (ਮੂਲ 76).\n\
         Use 0 ਮੰਜ਼ਿਲ disable ਸਤਰ wrapping\n\
"));
#ਜੇ ਅਧਾਰ_TYPE == 42
      oputs (_("\
      --z85\n\
         ascii85-like encoding (ZeroMQ spec:32/Z85);\n\
         when encoding, ਨਿਵੇਸ਼ ਲੰਮਾਈ must be a ਗੁਣਜ ਦਾ 4;\n\
         when decoding, ਨਿਵੇਸ਼ ਲੰਮਾਈ must be a ਗੁਣਜ ਦਾ 5\n\
"));
#ਜੇ_ਅੰਤ
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
#ਜੇ ਅਧਾਰ_TYPE == 42
      fputs (_("\
\n\
When decoding, the ਨਿਵੇਸ਼ may contain newlines ਵਿੱਚ addition ਮੰਜ਼ਿਲ the ਬਾਈਟਾਂ ਦਾ\n\
the formal alphabet.  Use --ignore-garbage ਮੰਜ਼ਿਲ attempt ਮੰਜ਼ਿਲ recover\n\
from any other non-alphabet ਬਾਈਟਾਂ ਵਿੱਚ the encoded ਧਾਰਾ.\n\
"), stdout);
#ਨਹੀਂਤਾਂ
      ਲਿਖੋ (_("\
\n\
The ਅੰਕੜੇ are encoded as described ਲਈ the %s alphabet ਵਿੱਚ RFC 4648.\n\
When decoding, the ਨਿਵੇਸ਼ may contain newlines ਵਿੱਚ addition ਮੰਜ਼ਿਲ the ਬਾਈਟਾਂ ਦਾ\n\
the formal %s alphabet.  Use --ignore-garbage ਮੰਜ਼ਿਲ attempt ਮੰਜ਼ਿਲ recover\n\
from any other non-alphabet ਬਾਈਟਾਂ ਵਿੱਚ the encoded ਧਾਰਾ.\n"),
              ਪ੍ਰੋਗਰਾਮ_NAME, ਪ੍ਰੋਗਰਾਮ_NAME);
#ਜੇ_ਅੰਤ
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }

  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

#ਜੇ ਅਧਾਰ_TYPE != 64
ਸਥਿਰ ਪੂਰਨ
base32_required_padding (ਪੂਰਨ len)
{
  ਪੂਰਨ partial = len % 8;
  ਵਾਪਸ partial ? 8 - partial : 0;
}
#ਜੇ_ਅੰਤ

#ਜੇ ਅਧਾਰ_TYPE != 32
ਸਥਿਰ ਪੂਰਨ
base64_required_padding (ਪੂਰਨ len)
{
  ਪੂਰਨ partial = len % 4;
  ਵਾਪਸ partial ? 4 - partial : 0;
}
#ਜੇ_ਅੰਤ

#ਜੇ ਅਧਾਰ_TYPE == 42
ਸਥਿਰ ਪੂਰਨ
no_required_padding (MAYBE_ਅਣਵਰਤਿਆ ਪੂਰਨ len)
{
  ਵਾਪਸ 0;
}
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ENC_BLOCKSIZE (1024 * 3 * 10)

#ਜੇ ਅਧਾਰ_TYPE == 32
# define ਅਧਾਰ_ਲੰਮਾਈ BASE32_ਲੰਮਾਈ
# define REQUIRED_PADDING base32_required_padding
/* Note that increasing this may decrease performance if --ignore-garbage
   is used, because of the memmove operation below.  */
# define DEC_BLOCKSIZE (1024 * 5)

/* Ensure that BLOCKSIZE is a multiple of 5 and 8.  */
static_assert (ENC_BLOCKSIZE % 40 == 0); /* Padding chars only on last block. */
static_assert (DEC_BLOCKSIZE % 40 == 0); /* Complete encoded blocks are used. */

# define ਅਧਾਰ_encode base32_encode
# define ਅਧਾਰ_decode_context base32_decode_context
# define ਅਧਾਰ_decode_ctx_init base32_decode_ctx_init
# define ਅਧਾਰ_decode_ctx base32_decode_ctx
# define ਅਧਾਰ_decode_ctx_finalize decode_ctx_finalize
# define isubase isubase32
#ਜਾਂਜੇ ਅਧਾਰ_TYPE == 64
# define ਅਧਾਰ_ਲੰਮਾਈ BASE64_ਲੰਮਾਈ
# define REQUIRED_PADDING base64_required_padding
/* Note that increasing this may decrease performance if --ignore-garbage
   is used, because of the memmove operation below.  */
# define DEC_BLOCKSIZE (1024 * 3)

/* Ensure that BLOCKSIZE is a multiple of 3 and 4.  */
static_assert (ENC_BLOCKSIZE % 12 == 0); /* Padding chars only on last block. */
static_assert (DEC_BLOCKSIZE % 12 == 0); /* Complete encoded blocks are used. */

# define ਅਧਾਰ_encode base64_encode
# define ਅਧਾਰ_decode_context base64_decode_context
# define ਅਧਾਰ_decode_ctx_init base64_decode_ctx_init
# define ਅਧਾਰ_decode_ctx base64_decode_ctx
# define ਅਧਾਰ_decode_ctx_finalize decode_ctx_finalize
# define isubase isubase64
#ਜਾਂਜੇ ਅਧਾਰ_TYPE == 42


# define ਅਧਾਰ_ਲੰਮਾਈ ਅਧਾਰ_ਲੰਮਾਈ_2
# define REQUIRED_PADDING required_padding

/* Note that increasing this may decrease performance if --ignore-garbage
   is used, because of the memmove operation below.  */
# define DEC_BLOCKSIZE (4200)
static_assert (DEC_BLOCKSIZE % 40 == 0); /* complete encoded blocks for base32*/
static_assert (DEC_BLOCKSIZE % 12 == 0); /* complete encoded blocks for base64*/

ਸਥਿਰ idx_t (*ਅਧਾਰ_ਲੰਮਾਈ_2) (idx_t len);
ਸਥਿਰ ਪੂਰਨ (*required_padding) (ਪੂਰਨ i);
ਸਥਿਰ bool (*isubase) (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ch);
ਸਥਿਰ ਖਾਲੀ (*ਅਧਾਰ_encode) (ਅੱਖਰ ਅਚਲ *ਸੀਮਿਤ ਵਿੱਚ, idx_t inlen,
                            ਅੱਖਰ *ਸੀਮਿਤ ਬਾਹਰਲਾ, idx_t outlen);

ਬਣਤਰ base16_decode_context
{
  /* Either a 4-bit nibble, or negative if we have no nibble.  */
  ਨਿਸ਼ਾਨੀਵਾਲਾ ਅੱਖਰ nibble;
};

ਬਣਤਰ z85_decode_context
{
  ਪੂਰਨ i;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ octets[5];
};

ਬਣਤਰ base58_context
{
  idx_t ਮਾਪ;
  idx_t ਸਮਰੱਥਾ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *buf COUNTED_BY (ਸਮਰੱਥਾ);
};

ਬਣਤਰ base2_decode_context
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ octet;
  ਪੂਰਨ ਦੋਆਧਾਰੀ_ਅੰਕ_pos;
};

ਬਣਤਰ ਅਧਾਰ_decode_context
{
  ਸੰਘ {
    ਬਣਤਰ base64_decode_context base64;
    ਬਣਤਰ base32_decode_context base32;
    ਬਣਤਰ base16_decode_context base16;
    ਬਣਤਰ base2_decode_context base2;
    ਬਣਤਰ z85_decode_context z85;
    ਬਣਤਰ base58_context base58;
  } ctx;
  ਅੱਖਰ *inbuf;
  idx_t bufsize;
};
ਸਥਿਰ ਖਾਲੀ (*ਅਧਾਰ_decode_ctx_init) (ਬਣਤਰ ਅਧਾਰ_decode_context *ctx);
ਸਥਿਰ bool (*ਅਧਾਰ_decode_ctx) (ਬਣਤਰ ਅਧਾਰ_decode_context *ctx,
                                ਅੱਖਰ ਅਚਲ *ਸੀਮਿਤ ਵਿੱਚ, idx_t inlen,
                                ਅੱਖਰ *ਸੀਮਿਤ ਬਾਹਰਲਾ, idx_t *outlen);
ਸਥਿਰ bool (*ਅਧਾਰ_decode_ctx_finalize) (ਬਣਤਰ ਅਧਾਰ_decode_context *ctx,
                                         ਅੱਖਰ *ਸੀਮਿਤ *ਬਾਹਰਲਾ, idx_t *outlen);

ਬਣਤਰ ਅਧਾਰ_encode_context
{
  ਸੰਘ {
    ਬਣਤਰ base58_context base58;
  } ctx;
};

ਸਥਿਰ ਖਾਲੀ (*ਅਧਾਰ_encode_ctx_init) (ਬਣਤਰ ਅਧਾਰ_encode_context *ctx);
ਸਥਿਰ bool (*ਅਧਾਰ_encode_ctx) (ਬਣਤਰ ਅਧਾਰ_encode_context *ctx,
                                ਅੱਖਰ ਅਚਲ *ਸੀਮਿਤ ਵਿੱਚ, idx_t inlen,
                                ਅੱਖਰ *ਸੀਮਿਤ ਬਾਹਰਲਾ, idx_t *outlen);
ਸਥਿਰ bool (*ਅਧਾਰ_encode_ctx_finalize) (ਬਣਤਰ ਅਧਾਰ_encode_context *ctx,
                                         ਅੱਖਰ *ਸੀਮਿਤ *ਬਾਹਰਲਾ, idx_t *outlen);

ਸਥਿਰ bool
no_padding (MAYBE_ਅਣਵਰਤਿਆ ਬਣਤਰ ਅਧਾਰ_decode_context *ctx)
{
  ਵਾਪਸ false;
}

ਸਥਿਰ ਪੂਰਨ
no_pending_ਲੰਮਾਈ (MAYBE_ਅਣਵਰਤਿਆ ਬਣਤਰ ਅਧਾਰ_decode_context *ctx)
{
  ਵਾਪਸ 0;
}
#ਜੇ_ਅੰਤ

#ਜੇ ਅਧਾਰ_TYPE == 42
ਸਥਿਰ bool (*has_padding) (ਬਣਤਰ ਅਧਾਰ_decode_context *ctx);
ਸਥਿਰ ਪੂਰਨ (*get_pending_ਲੰਮਾਈ) (ਬਣਤਰ ਅਧਾਰ_decode_context *ctx);

ਸਥਿਰ bool
base64_ctx_has_padding (ਬਣਤਰ ਅਧਾਰ_decode_context *ctx)
{
  ਵਾਪਸ ctx->ctx.base64.i && ctx->ctx.base64.buf[ctx->ctx.base64.i - 1] == '=';
}

ਸਥਿਰ bool
base32_ctx_has_padding (ਬਣਤਰ ਅਧਾਰ_decode_context *ctx)
{
  ਵਾਪਸ ctx->ctx.base32.i && ctx->ctx.base32.buf[ctx->ctx.base32.i - 1] == '=';
}

ਸਥਿਰ ਪੂਰਨ
base64_ctx_get_pending_ਲੰਮਾਈ (ਬਣਤਰ ਅਧਾਰ_decode_context *ctx)
{
  ਵਾਪਸ ctx->ctx.base64.i;
}

ਸਥਿਰ ਪੂਰਨ
base32_ctx_get_pending_ਲੰਮਾਈ (ਬਣਤਰ ਅਧਾਰ_decode_context *ctx)
{
  ਵਾਪਸ ctx->ctx.base32.i;
}

ਸਥਿਰ ਪੂਰਨ
base16_ctx_get_pending_ਲੰਮਾਈ (MAYBE_ਅਣਵਰਤਿਆ ਬਣਤਰ ਅਧਾਰ_decode_context *ctx)
{
  ਵਾਪਸ 1; /* Always check nibble state.  */
}

ਸਥਿਰ ਪੂਰਨ
z85_ctx_get_pending_ਲੰਮਾਈ (ਬਣਤਰ ਅਧਾਰ_decode_context *ctx)
{
  ਵਾਪਸ ctx->ctx.z85.i;
}

ਸਥਿਰ ਪੂਰਨ
base2_ctx_get_pending_ਲੰਮਾਈ (ਬਣਤਰ ਅਧਾਰ_decode_context *ctx)
{
  ਵਾਪਸ ctx->ctx.base2.ਦੋਆਧਾਰੀ_ਅੰਕ_pos;
}
#ਨਹੀਂਤਾਂ
ਸਥਿਰ bool
has_padding (ਬਣਤਰ ਅਧਾਰ_decode_context *ctx)
{
  ਵਾਪਸ ctx->i && ctx->buf[ctx->i - 1] == '=';
}

ਸਥਿਰ ਪੂਰਨ
get_pending_ਲੰਮਾਈ (ਬਣਤਰ ਅਧਾਰ_decode_context *ctx)
{
  ਵਾਪਸ ctx->i;
}
#ਜੇ_ਅੰਤ


/* Process any pending data in CTX, while auto padding if appropriate.
   Return TRUE on success, FALSE on failure.  */

ਸਥਿਰ bool
decode_ctx_finalize (ਬਣਤਰ ਅਧਾਰ_decode_context *ctx,
                     ਅੱਖਰ *ਸੀਮਿਤ *ਬਾਹਰਲਾ, idx_t *outlen)
{
  ਜੇ (get_pending_ਲੰਮਾਈ (ctx) == 0)
    {
      *outlen = 0;
      ਵਾਪਸ true;
    }

  /* Auto-pad input and flush the context */
  ਅੱਖਰ padbuf[8] ATTRIBUTE_NONSTRING = "========";
  idx_t pending_len = get_pending_ਲੰਮਾਈ (ctx);
  idx_t auto_padding = REQUIRED_PADDING (pending_len);
  idx_t n = *outlen;
  bool ਨਤੀਜਾ;

  ਜੇ (auto_padding && ! has_padding (ctx))
    {
      affirm (auto_padding <= ਆਕਾਰ (padbuf));
      ਨਤੀਜਾ = ਅਧਾਰ_decode_ctx (ctx, padbuf, auto_padding, *ਬਾਹਰਲਾ, &n);
    }
  ਨਹੀਂਤਾਂ
    {
      ਨਤੀਜਾ = ਅਧਾਰ_decode_ctx (ctx, "", 0, *ਬਾਹਰਲਾ, &n);
    }

  *outlen = n;
  ਵਾਪਸ ਨਤੀਜਾ;
}

#ਜੇ ਅਧਾਰ_TYPE == 42

ਸਥਿਰ idx_t
base64_ਲੰਮਾਈ_wrapper (idx_t len)
{
  ਵਾਪਸ BASE64_ਲੰਮਾਈ (len);
}

ਸਥਿਰ ਖਾਲੀ
base64_decode_ctx_init_wrapper (ਬਣਤਰ ਅਧਾਰ_decode_context *ctx)
{
  base64_decode_ctx_init (&ctx->ctx.base64);
}

ਸਥਿਰ bool
base64_decode_ctx_wrapper (ਬਣਤਰ ਅਧਾਰ_decode_context *ctx,
                           ਅੱਖਰ ਅਚਲ *ਸੀਮਿਤ ਵਿੱਚ, idx_t inlen,
                           ਅੱਖਰ *ਸੀਮਿਤ ਬਾਹਰਲਾ, idx_t *outlen)
{
  ਵਾਪਸ base64_decode_ctx (&ctx->ctx.base64, ਵਿੱਚ, inlen, ਬਾਹਰਲਾ, outlen);
}

ਸਥਿਰ ਖਾਲੀ
init_inbuf (ਬਣਤਰ ਅਧਾਰ_decode_context *ctx)
{
  ctx->bufsize = DEC_BLOCKSIZE;
  ctx->inbuf = xcharalloc (ctx->bufsize);
}

ਸਥਿਰ ਖਾਲੀ
prepare_inbuf (ਬਣਤਰ ਅਧਾਰ_decode_context *ctx, idx_t inlen)
{
  ਜੇ (ctx->bufsize < inlen)
    ctx->inbuf = xpalloc (ctx->inbuf, &ctx->bufsize,
                          inlen - ctx->bufsize, -1, ਆਕਾਰ *ctx->inbuf);
}


ਸਥਿਰ ਖਾਲੀ
base64url_encode (ਅੱਖਰ ਅਚਲ *ਸੀਮਿਤ ਵਿੱਚ, idx_t inlen,
                  ਅੱਖਰ *ਸੀਮਿਤ ਬਾਹਰਲਾ, idx_t outlen)
{
  base64_encode (ਵਿੱਚ, inlen, ਬਾਹਰਲਾ, outlen);
  /* translate 62nd and 63rd characters */
  ਅੱਖਰ *p = ਬਾਹਰਲਾ;
  ਜਦੋਂ (outlen--)
    {
      ਜੇ (*p == '+')
        *p = '-';
      ਨਹੀਂਤਾਂ ਜੇ (*p == '/')
        *p = '_';
      ++p;
    }
}

ਸਥਿਰ bool
isubase64url (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ch)
{
  ਵਾਪਸ (ch == '-' || ch == '_'
          || (ch != '+' && ch != '/' && isubase64 (ch)));
}

ਸਥਿਰ ਖਾਲੀ
base64url_decode_ctx_init_wrapper (ਬਣਤਰ ਅਧਾਰ_decode_context *ctx)
{
  base64_decode_ctx_init (&ctx->ctx.base64);
  init_inbuf (ctx);
}


ਸਥਿਰ bool
base64url_decode_ctx_wrapper (ਬਣਤਰ ਅਧਾਰ_decode_context *ctx,
                              ਅੱਖਰ ਅਚਲ *ਸੀਮਿਤ ਵਿੱਚ, idx_t inlen,
                              ਅੱਖਰ *ਸੀਮਿਤ ਬਾਹਰਲਾ, idx_t *outlen)
{
  prepare_inbuf (ctx, inlen);
  ਸਮ੍ਰਿਤੀ_ਨਕਲ (ctx->inbuf, ਵਿੱਚ, inlen);

  /* translate 62nd and 63rd characters */
  idx_t i = inlen;
  ਅੱਖਰ *p = ctx->inbuf;
  ਜਦੋਂ (i--)
    {
      ਜੇ (*p == '+' || *p == '/')
        {
          *outlen = 0;
          ਵਾਪਸ false; /* reject base64 input */
        }
      ਨਹੀਂਤਾਂ ਜੇ (*p == '-')
        *p = '+';
      ਨਹੀਂਤਾਂ ਜੇ (*p == '_')
        *p = '/';
      ++p;
    }

  ਵਾਪਸ base64_decode_ctx (&ctx->ctx.base64, ctx->inbuf, inlen,
                            ਬਾਹਰਲਾ, outlen);
}



ਸਥਿਰ idx_t
base32_ਲੰਮਾਈ_wrapper (idx_t len)
{
  ਵਾਪਸ BASE32_ਲੰਮਾਈ (len);
}

ਸਥਿਰ ਖਾਲੀ
base32_decode_ctx_init_wrapper (ਬਣਤਰ ਅਧਾਰ_decode_context *ctx)
{
  base32_decode_ctx_init (&ctx->ctx.base32);
}

ਸਥਿਰ bool
base32_decode_ctx_wrapper (ਬਣਤਰ ਅਧਾਰ_decode_context *ctx,
                           ਅੱਖਰ ਅਚਲ *ਸੀਮਿਤ ਵਿੱਚ, idx_t inlen,
                           ਅੱਖਰ *ਸੀਮਿਤ ਬਾਹਰਲਾ, idx_t *outlen)
{
  ਵਾਪਸ base32_decode_ctx (&ctx->ctx.base32, ਵਿੱਚ, inlen, ਬਾਹਰਲਾ, outlen);
}

/* ABCDEFGHIJKLMNOPQRSTUVWXYZ234567
ਮੰਜ਼ਿਲ
   0123456789ABCDEFGHIJKLMNOPQRSTUV */
ਸਥਿਰ ਅਚਲ ਅੱਖਰ base32_norm_ਮੰਜ਼ਿਲ_hex[32 + 9] = {
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
ਮੰਜ਼ਿਲ
   ABCDEFGHIJKLMNOPQRSTUVWXYZ234567 */
ਸਥਿਰ ਅਚਲ ਅੱਖਰ base32_hex_ਮੰਜ਼ਿਲ_norm[32 + 9] = {
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


ਸਤਰਅੰਦਰ ਸਥਿਰ bool
isubase32hex (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ch)
{
  ਵਾਪਸ ('0' <= ch && ch <= '9') || ('A' <= ch && ch <= 'V');
}


ਸਥਿਰ ਖਾਲੀ
base32hex_encode (ਅੱਖਰ ਅਚਲ *ਸੀਮਿਤ ਵਿੱਚ, idx_t inlen,
                  ਅੱਖਰ *ਸੀਮਿਤ ਬਾਹਰਲਾ, idx_t outlen)
{
  base32_encode (ਵਿੱਚ, inlen, ਬਾਹਰਲਾ, outlen);

  ਲਈ (ਅੱਖਰ *p = ਬਾਹਰਲਾ; outlen--; p++)
    {
      affirm (0x32 <= *p && *p <= 0x5a);          /* LCOV_EXCL_LINE */
      *p = base32_norm_ਮੰਜ਼ਿਲ_hex[*p - 0x32];
    }
}


ਸਥਿਰ ਖਾਲੀ
base32hex_decode_ctx_init_wrapper (ਬਣਤਰ ਅਧਾਰ_decode_context *ctx)
{
  base32_decode_ctx_init (&ctx->ctx.base32);
  init_inbuf (ctx);
}


ਸਥਿਰ bool
base32hex_decode_ctx_wrapper (ਬਣਤਰ ਅਧਾਰ_decode_context *ctx,
                              ਅੱਖਰ ਅਚਲ *ਸੀਮਿਤ ਵਿੱਚ, idx_t inlen,
                              ਅੱਖਰ *ਸੀਮਿਤ ਬਾਹਰਲਾ, idx_t *outlen)
{
  prepare_inbuf (ctx, inlen);

  idx_t i = inlen;
  ਅੱਖਰ *p = ctx->inbuf;
  ਜਦੋਂ (i--)
    {
      ਜੇ (isubase32hex (*ਵਿੱਚ))
        *p = base32_hex_ਮੰਜ਼ਿਲ_norm[*ਵਿੱਚ - 0x30];
      ਨਹੀਂਤਾਂ
        *p = *ਵਿੱਚ;
      ++p;
      ++ਵਿੱਚ;
    }

  ਵਾਪਸ base32_decode_ctx (&ctx->ctx.base32, ctx->inbuf, inlen,
                            ਬਾਹਰਲਾ, outlen);
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

ਸਥਿਰ ਨਿਸ਼ਾਨੀਵਾਲਾ ਅੱਖਰ ਅਚਲ base16_ਮੰਜ਼ਿਲ_int[256] = {
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

ਸਥਿਰ bool
isubase16 (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ch)
{
  ਵਾਪਸ ch < ਆਕਾਰ base16_ਮੰਜ਼ਿਲ_int && 0 <= base16_ਮੰਜ਼ਿਲ_int[ch];
}

ਸਥਿਰ idx_t
base16_ਲੰਮਾਈ (idx_t len)
{
  ਵਾਪਸ len * 2;
}


ਸਥਿਰ ਖਾਲੀ
base16_encode (ਅੱਖਰ ਅਚਲ *ਸੀਮਿਤ ਵਿੱਚ, idx_t inlen,
               ਅੱਖਰ *ਸੀਮਿਤ ਬਾਹਰਲਾ, idx_t outlen)
{
  ਸਥਿਰ ਅਚਲ ਅੱਖਰ base16[16] ATTRIBUTE_NONSTRING = "0123456789ABCDEF";

  ਜਦੋਂ (inlen && outlen)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ c = *ਵਿੱਚ;
      *ਬਾਹਰਲਾ++ = base16[c >> 4];
      *ਬਾਹਰਲਾ++ = base16[c & 0x0F];
      ++ਵਿੱਚ;
      inlen--;
      outlen -= 2;
    }
}


ਸਥਿਰ ਖਾਲੀ
base16_decode_ctx_init (ਬਣਤਰ ਅਧਾਰ_decode_context *ctx)
{
  init_inbuf (ctx);
  ctx->ctx.base16.nibble = -1;
}


ਸਥਿਰ bool
base16_decode_ctx (ਬਣਤਰ ਅਧਾਰ_decode_context *ctx,
                   ਅੱਖਰ ਅਚਲ *ਸੀਮਿਤ ਵਿੱਚ, idx_t inlen,
                   ਅੱਖਰ *ਸੀਮਿਤ ਬਾਹਰਲਾ, idx_t *outlen)
{
  bool ignore_ਸਤਰਾਂ = true;  /* for now, always ignore them */
  ਅੱਖਰ *out0 = ਬਾਹਰਲਾ;
  ਨਿਸ਼ਾਨੀਵਾਲਾ ਅੱਖਰ nibble = ctx->ctx.base16.nibble;

  /* inlen==0 is request to flush output.
     if there is a dangling high nibble - we are missing the low nibble,
     so return false - indicating an invalid input.  */
  ਜੇ (inlen == 0)
    {
      *outlen = 0;
      ਵਾਪਸ nibble < 0;
    }

  ਜਦੋਂ (inlen--)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ c = *ਵਿੱਚ++;
      ਜੇ (ignore_ਸਤਰਾਂ && c == '\n')
        ਜਾਰੀ;

      ਜੇ (ਆਕਾਰ base16_ਮੰਜ਼ਿਲ_int <= c || base16_ਮੰਜ਼ਿਲ_int[c] < 0)
        {
          *outlen = ਬਾਹਰਲਾ - out0;
          ਵਾਪਸ false; /* garbage - return false */
        }

      ਜੇ (nibble < 0)
        nibble = base16_ਮੰਜ਼ਿਲ_int[c];
      ਨਹੀਂਤਾਂ
        {
          /* have both nibbles, write octet */
          *ਬਾਹਰਲਾ++ = (nibble << 4) + base16_ਮੰਜ਼ਿਲ_int[c];
          nibble = -1;
        }
    }

  ctx->ctx.base16.nibble = nibble;
  *outlen = ਬਾਹਰਲਾ - out0;
  ਵਾਪਸ true;
}



ATTRIBUTE_ਸ਼ੁੱਧ
ਸਥਿਰ idx_t
z85_ਲੰਮਾਈ (idx_t len)
{
  /* Z85 does not allow padding, so no need to round to highest integer.  */
  idx_t z85_len = (len * 5) / 4;
  affirm (0 <= z85_len);
  ਵਾਪਸ z85_len;
}

ਸਥਿਰ bool
isuz85 (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ch)
{
  ਵਾਪਸ c_isalnum (ch) || strchr (".-:+=^!/*?&<>()[]{}@%$#", ch) != ਨਿਰਾ;
}

ਸਥਿਰ ਅੱਖਰ ਅਚਲ z85_encoding[85] ATTRIBUTE_NONSTRING =
  "0123456789"
  "abcdefghijklmnopqrstuvwxyz"
  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
  ".-:+=^!/*?&<>()[]{}@%$#";

ਸਥਿਰ ਖਾਲੀ
z85_encode (ਅੱਖਰ ਅਚਲ *ਸੀਮਿਤ ਵਿੱਚ, idx_t inlen,
            ਅੱਖਰ *ਸੀਮਿਤ ਬਾਹਰਲਾ, idx_t outlen)
{
  ਪੂਰਨ i = 0;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ quad[4];
  idx_t outidx = 0;

  ਜਦੋਂ (true)
    {
      ਜੇ (inlen == 0)
        {
          /* no more input, exactly on 4 octet boundary. */
          ਜੇ (i == 0)
            ਵਾਪਸ;

          /* currently, there's no way to return an error in encoding.  */
          error (EXIT_FAILURE, 0,
                 _("invalid input (length must be multiple of 4 characters)"));
        }
      ਨਹੀਂਤਾਂ
        {
          quad[i++] = *ਵਿੱਚ++;
          --inlen;
        }

      /* Got a quad, encode it */
      ਜੇ (i == 4)
        {
          int_fast64_t val = quad[0];
          val = (val << 24) + (quad[1] << 16) + (quad[2] << 8) + quad[3];

          ਲਈ (ਪੂਰਨ j = 4; j >= 0; --j)
            {
              ਪੂਰਨ c = val % 85;
              val /= 85;

              /* NOTE: if there is padding (which is trimmed by z85
                 before outputting the result), the output buffer 'out'
                 might not include enough allocated bytes for the padding,
                 so don't store them. */
              ਜੇ (outidx + j < outlen)
                ਬਾਹਰਲਾ[j] = z85_encoding[c];
            }
          ਬਾਹਰਲਾ += 5;
          outidx += 5;
          i = 0;
        }
    }
}

ਸਥਿਰ ਖਾਲੀ
z85_decode_ctx_init (ਬਣਤਰ ਅਧਾਰ_decode_context *ctx)
{
  init_inbuf (ctx);
  ctx->ctx.z85.i = 0;
}


# define Z85_LO_CTX_ਮੰਜ਼ਿਲ_32_ਦੋਆਧਾਰੀ_ਅੰਕ_VAL(ctx) \
  (((ctx)->ctx.z85.octets[1] * 85 * 85 * 85) +      \
   ((ctx)->ctx.z85.octets[2] * 85 * 85) +	    \
   ((ctx)->ctx.z85.octets[3] * 85) +		    \
   ((ctx)->ctx.z85.octets[4]))


# define Z85_HI_CTX_ਮੰਜ਼ਿਲ_32_ਦੋਆਧਾਰੀ_ਅੰਕ_VAL(ctx) \
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
ਸਥਿਰ ਨਿਸ਼ਾਨੀਵਾਲਾ ਅੱਖਰ ਅਚਲ z85_decoding[93] = {
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

ਸਥਿਰ bool
z85_decode_ctx (ਬਣਤਰ ਅਧਾਰ_decode_context *ctx,
                ਅੱਖਰ ਅਚਲ *ਸੀਮਿਤ ਵਿੱਚ, idx_t inlen,
                ਅੱਖਰ *ਸੀਮਿਤ ਬਾਹਰਲਾ, idx_t *outlen)
{
  bool ignore_ਸਤਰਾਂ = true;  /* for now, always ignore them */

  *outlen = 0;

  /* inlen==0 is request to flush output.
     if there are dangling values - we are missing entries,
     so return false - indicating an invalid input.  */
  ਜੇ (inlen == 0)
    {
      ਜੇ (ctx->ctx.z85.i > 0)
        {
          /* Z85 variant does not allow padding - input must
             be a multiple of 5 - so return error.  */
          ਵਾਪਸ false;
        }
      ਵਾਪਸ true;
    }

  ਜਦੋਂ (inlen--)
    {
      ਜੇ (ignore_ਸਤਰਾਂ && *ਵਿੱਚ == '\n')
        {
          ++ਵਿੱਚ;
          ਜਾਰੀ;
        }

      /* z85 decoding */
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ c = *ਵਿੱਚ;

      ਜੇ (c >= 33 && c <= 125)
        {
          ਨਿਸ਼ਾਨੀਵਾਲਾ ਅੱਖਰ ch = z85_decoding[c - 33];
          ਜੇ (ch < 0)
            ਵਾਪਸ false; /* garbage - return false */
          c = ch;
        }
      ਨਹੀਂਤਾਂ
        ਵਾਪਸ false; /* garbage - return false */

      ++ਵਿੱਚ;

      ctx->ctx.z85.octets[ctx->ctx.z85.i++] = c;
      ਜੇ (ctx->ctx.z85.i == 5)
        {
          /* decode the lowest 4 octets, then check for overflows.  */
          int_fast64_t val = Z85_LO_CTX_ਮੰਜ਼ਿਲ_32_ਦੋਆਧਾਰੀ_ਅੰਕ_VAL (ctx);

          /* The Z85 spec and the reference implementation say nothing
             about overflows. To be on the safe side, reject them.  */

          val += Z85_HI_CTX_ਮੰਜ਼ਿਲ_32_ਦੋਆਧਾਰੀ_ਅੰਕ_VAL (ctx);
          ਜੇ ((val >> 24) & ~0xFF)
            ਵਾਪਸ false;

          *ਬਾਹਰਲਾ++ = val >> 24;
          *ਬਾਹਰਲਾ++ = (val >> 16) & 0xFF;
          *ਬਾਹਰਲਾ++ = (val >> 8) & 0xFF;
          *ਬਾਹਰਲਾ++ = val & 0xFF;

          *outlen += 4;

          ctx->ctx.z85.i = 0;
        }
    }
  ਵਾਪਸ true;
}


ਸਤਰਅੰਦਰ ਸਥਿਰ bool
isubase2 (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ch)
{
  ਵਾਪਸ ch == '0' || ch == '1';
}

ਸਥਿਰ idx_t
base2_ਲੰਮਾਈ (idx_t len)
{
  ਵਾਪਸ len * 8;
}


ਸਤਰਅੰਦਰ ਸਥਿਰ ਖਾਲੀ
base2msbf_encode (ਅੱਖਰ ਅਚਲ *ਸੀਮਿਤ ਵਿੱਚ, idx_t inlen,
                  ਅੱਖਰ *ਸੀਮਿਤ ਬਾਹਰਲਾ, idx_t outlen)
{
  ਜਦੋਂ (inlen && outlen)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ c = *ਵਿੱਚ;
      ਲਈ (ਪੂਰਨ i = 0; i < 8; i++)
        {
          *ਬਾਹਰਲਾ++ = c & 0x80 ? '1' : '0';
          c <<= 1;
        }
      inlen--;
      outlen -= 8;
      ++ਵਿੱਚ;
    }
}

ਸਤਰਅੰਦਰ ਸਥਿਰ ਖਾਲੀ
base2lsbf_encode (ਅੱਖਰ ਅਚਲ *ਸੀਮਿਤ ਵਿੱਚ, idx_t inlen,
                  ਅੱਖਰ *ਸੀਮਿਤ ਬਾਹਰਲਾ, idx_t outlen)
{
  ਜਦੋਂ (inlen && outlen)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ c = *ਵਿੱਚ;
      ਲਈ (ਪੂਰਨ i = 0; i < 8; i++)
        {
          *ਬਾਹਰਲਾ++ = c & 0x01 ? '1' : '0';
          c >>= 1;
        }
      inlen--;
      outlen -= 8;
      ++ਵਿੱਚ;
    }
}


ਸਥਿਰ ਖਾਲੀ
base2_decode_ctx_init (ਬਣਤਰ ਅਧਾਰ_decode_context *ctx)
{
  init_inbuf (ctx);
  ctx->ctx.base2.octet = 0;
  ctx->ctx.base2.ਦੋਆਧਾਰੀ_ਅੰਕ_pos = 0;
}


ਸਥਿਰ bool
base2lsbf_decode_ctx (ਬਣਤਰ ਅਧਾਰ_decode_context *ctx,
                      ਅੱਖਰ ਅਚਲ *ਸੀਮਿਤ ਵਿੱਚ, idx_t inlen,
                      ਅੱਖਰ *ਸੀਮਿਤ ਬਾਹਰਲਾ, idx_t *outlen)
{
  bool ignore_ਸਤਰਾਂ = true;  /* for now, always ignore them */

  *outlen = 0;

  /* inlen==0 is request to flush output.
     if there is a dangling bit - we are missing some bits,
     so return false - indicating an invalid input.  */
  ਜੇ (inlen == 0)
    ਵਾਪਸ ctx->ctx.base2.ਦੋਆਧਾਰੀ_ਅੰਕ_pos == 0;

  ਜਦੋਂ (inlen--)
    {
      ਜੇ (ignore_ਸਤਰਾਂ && *ਵਿੱਚ == '\n')
        {
          ++ਵਿੱਚ;
          ਜਾਰੀ;
        }

      ਜੇ (!isubase2 (*ਵਿੱਚ))
        ਵਾਪਸ false;

      bool ਦੋਆਧਾਰੀ_ਅੰਕ = (*ਵਿੱਚ == '1');
      ctx->ctx.base2.octet |= ਦੋਆਧਾਰੀ_ਅੰਕ << ctx->ctx.base2.ਦੋਆਧਾਰੀ_ਅੰਕ_pos;
      ++ctx->ctx.base2.ਦੋਆਧਾਰੀ_ਅੰਕ_pos;

      ਜੇ (ctx->ctx.base2.ਦੋਆਧਾਰੀ_ਅੰਕ_pos == 8)
        {
          *ਬਾਹਰਲਾ++ = ctx->ctx.base2.octet;
          ctx->ctx.base2.octet = 0;
          ++*outlen;
          ctx->ctx.base2.ਦੋਆਧਾਰੀ_ਅੰਕ_pos = 0;
        }

      ++ਵਿੱਚ;
    }

  ਵਾਪਸ true;
}

ਸਥਿਰ bool
base2msbf_decode_ctx (ਬਣਤਰ ਅਧਾਰ_decode_context *ctx,
                      ਅੱਖਰ ਅਚਲ *ਸੀਮਿਤ ਵਿੱਚ, idx_t inlen,
                      ਅੱਖਰ *ਸੀਮਿਤ ਬਾਹਰਲਾ, idx_t *outlen)
{
  bool ignore_ਸਤਰਾਂ = true;  /* for now, always ignore them */

  *outlen = 0;

  /* inlen==0 is request to flush output.
     if there is a dangling bit - we are missing some bits,
     so return false - indicating an invalid input.  */
  ਜੇ (inlen == 0)
    ਵਾਪਸ ctx->ctx.base2.ਦੋਆਧਾਰੀ_ਅੰਕ_pos == 0;

  ਜਦੋਂ (inlen--)
    {
      ਜੇ (ignore_ਸਤਰਾਂ && *ਵਿੱਚ == '\n')
        {
          ++ਵਿੱਚ;
          ਜਾਰੀ;
        }

      ਜੇ (!isubase2 (*ਵਿੱਚ))
        ਵਾਪਸ false;

      bool ਦੋਆਧਾਰੀ_ਅੰਕ = (*ਵਿੱਚ == '1');
      ਜੇ (ctx->ctx.base2.ਦੋਆਧਾਰੀ_ਅੰਕ_pos == 0)
        ctx->ctx.base2.ਦੋਆਧਾਰੀ_ਅੰਕ_pos = 8;
      --ctx->ctx.base2.ਦੋਆਧਾਰੀ_ਅੰਕ_pos;
      ctx->ctx.base2.octet |= ਦੋਆਧਾਰੀ_ਅੰਕ << ctx->ctx.base2.ਦੋਆਧਾਰੀ_ਅੰਕ_pos;

      ਜੇ (ctx->ctx.base2.ਦੋਆਧਾਰੀ_ਅੰਕ_pos == 0)
        {
          *ਬਾਹਰਲਾ++ = ctx->ctx.base2.octet;
          ctx->ctx.base2.octet = 0;
          ++*outlen;
        }

      ++ਵਿੱਚ;
    }

  ਵਾਪਸ true;
}

/* Map from GMP (up to base 62):
   "0123456789 ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuv";
   to base 58:
   "123456789A BCDEFGHJKLMNPQRSTUVWXYZabc defghijkmnopqrstuvwxyz";  */
ਸਥਿਰ ਨਿਸ਼ਾਨੀਵਾਲਾ ਅੱਖਰ ਅਚਲ gmp_ਮੰਜ਼ਿਲ_base58[256] = {
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

ਸਥਿਰ ਨਿਸ਼ਾਨੀਵਾਲਾ ਅੱਖਰ ਅਚਲ base58_ਮੰਜ਼ਿਲ_gmp[256] = {
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

ਸਥਿਰ bool
isubase58 (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ch)
{
  ਵਾਪਸ ch < ਆਕਾਰ base58_ਮੰਜ਼ਿਲ_gmp && 0 <= base58_ਮੰਜ਼ਿਲ_gmp[ch];
}


ATTRIBUTE_ਸ਼ੁੱਧ
ਸਥਿਰ idx_t
base58_ਲੰਮਾਈ (idx_t len)
{
  /* Base58 output length is approximately log(256)/log(58),
     which is approximately len * 138 / 100,
     which is at most ((len + 100 - 1) / 100) * 138
     +1 to ensure we've enough place for NUL  */
  idx_t base58_len = ((len + 99) / 100) * 138 + 1;
  affirm (0 < base58_len);
  ਵਾਪਸ base58_len;
}


ਸਥਿਰ ਖਾਲੀ
base58_encode_ctx_init (ਬਣਤਰ ਅਧਾਰ_encode_context *ctx)
{
  ctx->ctx.base58.buf = ਨਿਰਾ;
  ctx->ctx.base58.ਮਾਪ = 0;
  ctx->ctx.base58.ਸਮਰੱਥਾ = 0;
}


ਸਥਿਰ bool
base58_encode_ctx (ਬਣਤਰ ਅਧਾਰ_encode_context *ctx,
                   ਅੱਖਰ ਅਚਲ *ਸੀਮਿਤ ਵਿੱਚ, idx_t inlen,
                   MAYBE_ਅਣਵਰਤਿਆ ਅੱਖਰ *ਸੀਮਿਤ ਬਾਹਰਲਾ, idx_t *outlen)
{
  *outlen = 0;   /* Only accumulate input in this function.  */

  ਜੇ (inlen == 0)
    ਵਾਪਸ true;

  idx_t free_space = ctx->ctx.base58.ਸਮਰੱਥਾ - ctx->ctx.base58.ਮਾਪ;
  ਜੇ (free_space < inlen)
    {
      ctx->ctx.base58.buf = xpalloc (ctx->ctx.base58.buf,
                                     &ctx->ctx.base58.ਸਮਰੱਥਾ,
                                     inlen - free_space,
                                     -1, ਆਕਾਰ *ctx->ctx.base58.buf);
    }

  ਸਮ੍ਰਿਤੀ_ਨਕਲ (ctx->ctx.base58.buf + ctx->ctx.base58.ਮਾਪ, ਵਿੱਚ, inlen);
  ctx->ctx.base58.ਮਾਪ += inlen;

  ਵਾਪਸ true;
}

ਸਥਿਰ ਖਾਲੀ
base58_encode (ਅੱਖਰ ਅਚਲ *ਅੰਕੜੇ, size_t ਅੰਕੜੇ_len,
               ਅੱਖਰ *ਬਾਹਰਲਾ, idx_t *outlen)
{
  affirm (ਅਧਾਰ_ਲੰਮਾਈ_2 (ਅੰਕੜੇ_len) <= *outlen);

  size_t zeros = 0;
  ਜਦੋਂ (zeros < ਅੰਕੜੇ_len && ਅੰਕੜੇ[zeros] == 0)
    zeros++;

  memset (ਬਾਹਰਲਾ, '1', zeros);
  ਅੱਖਰ *p = ਬਾਹਰਲਾ + zeros;

  /* Use GMP to convert from base 256 to base 58.  */
  mpz_t ਅੰਕ;
  mpz_init (ਅੰਕ);
  ਜੇ (ਅੰਕੜੇ_len - zeros)
    {
      mpz_import (ਅੰਕ, ਅੰਕੜੇ_len - zeros, 1, 1, 0, 0, ਅੰਕੜੇ + zeros);
      ਲਈ (p = mpz_get_str (p, 58, ਅੰਕ); *p; p++)
        *p = gmp_ਮੰਜ਼ਿਲ_base58[ਮੰਜ਼ਿਲ_uchar (*p)];
    }
  mpz_clear (ਅੰਕ);

  *outlen = p - ਬਾਹਰਲਾ;
}


ਸਥਿਰ bool
base58_encode_ctx_finalize (ਬਣਤਰ ਅਧਾਰ_encode_context *ctx,
                            ਅੱਖਰ *ਸੀਮਿਤ *ਬਾਹਰਲਾ, idx_t *outlen)
{
  /* Ensure output buffer is large enough.  */
  idx_t max_outlen = ਅਧਾਰ_ਲੰਮਾਈ_2 (ctx->ctx.base58.ਮਾਪ);
  ਜੇ (max_outlen > *outlen)
    {
      *ਬਾਹਰਲਾ = xrealloc (*ਬਾਹਰਲਾ, max_outlen);
      *outlen = max_outlen;
    }

  base58_encode ((ਅੱਖਰ *)ctx->ctx.base58.buf, ctx->ctx.base58.ਮਾਪ,
                 *ਬਾਹਰਲਾ, outlen);

  ਮੁਕਤ (ctx->ctx.base58.buf);
  ctx->ctx.base58.buf = ਨਿਰਾ;
  ctx->ctx.base58.ਮਾਪ = 0;
  ctx->ctx.base58.ਸਮਰੱਥਾ = 0;

  ਵਾਪਸ true;
}


ਸਥਿਰ ਖਾਲੀ
base58_decode_ctx_init (ਬਣਤਰ ਅਧਾਰ_decode_context *ctx)
{
  ctx->ctx.base58.ਮਾਪ = 0;
  ctx->ctx.base58.ਸਮਰੱਥਾ = 0;
  ctx->ctx.base58.buf = ਨਿਰਾ;
}

ਸਥਿਰ bool
base58_decode_ctx (ਬਣਤਰ ਅਧਾਰ_decode_context *ctx,
                   ਅੱਖਰ ਅਚਲ *ਸੀਮਿਤ ਵਿੱਚ, idx_t inlen,
                   MAYBE_ਅਣਵਰਤਿਆ ਅੱਖਰ *ਸੀਮਿਤ ਬਾਹਰਲਾ, idx_t *outlen)
{
  bool ignore_ਸਤਰਾਂ = true;  /* for now, always ignore them */

  *outlen = 0;   /* Only accumulate input in this function.  */

  ਜੇ (inlen == 0)
    ਵਾਪਸ true;

  idx_t free_space = ctx->ctx.base58.ਸਮਰੱਥਾ - ctx->ctx.base58.ਮਾਪ;
  free_space -= 1;  /* Ensure we leave space for NUL (for mpz_set_str).  */
  ਜੇ (free_space < inlen)
    {
      ctx->ctx.base58.buf = xpalloc (ctx->ctx.base58.buf,
                                     &ctx->ctx.base58.ਸਮਰੱਥਾ,
                                     inlen - free_space,
                                     -1, ਆਕਾਰ *ctx->ctx.base58.buf);
    }

  /* Accumulate all valid input characters in our buffer.
     Note we don't rely on mpz_set_str() for validation
     as that allows (skips) all whitespace.  */
  ਲਈ (idx_t i = 0; i < inlen; i++)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ c = ਵਿੱਚ[i];

      ਜੇ (ignore_ਸਤਰਾਂ && c == '\n')
        ਜਾਰੀ;

      ਜੇ (!isubase58 (c))
        ਵਾਪਸ false;

      ctx->ctx.base58.buf[ctx->ctx.base58.ਮਾਪ++] = base58_ਮੰਜ਼ਿਲ_gmp[ਮੰਜ਼ਿਲ_uchar (c)];
    }

  ਵਾਪਸ true;
}


ਸਥਿਰ bool
base58_decode (ਅੱਖਰ ਅਚਲ *ਅੰਕੜੇ, size_t ਅੰਕੜੇ_len,
               ਅੱਖਰ *ਸੀਮਿਤ ਬਾਹਰਲਾ, idx_t *outlen)
{
  affirm (ਅੰਕੜੇ_len <= *outlen);

  size_t ones = 0;
  ਜਦੋਂ (ones < ਅੰਕੜੇ_len && ਅੰਕੜੇ[ones] == base58_ਮੰਜ਼ਿਲ_gmp['1'])
    ones++;

  memset (ਬਾਹਰਲਾ, 0, ones);

  /* Use GMP to convert from base 58 to base 256.  */
  mpz_t ਅੰਕ;
  mpz_init (ਅੰਕ);

  ਜੇ ((ਅੰਕੜੇ_len - ones) && mpz_ਸੈੱਟ_str (ਅੰਕ, ਅੰਕੜੇ + ones, 58) != 0)
    {
      mpz_clear (ਅੰਕ);
      *outlen = 0;
      ਵਾਪਸ false;
    }

  size_t exported_ਮਾਪ = 0;
  ਜੇ (ਅੰਕੜੇ_len - ones)
    {
      size_t ਦੁਆਧਾਰੀ_ਮਾਪ = (mpz_sizeinbase (ਅੰਕ, 2) + 7) / 8;
      affirm (*outlen - ones >= ਦੁਆਧਾਰੀ_ਮਾਪ);
      mpz_export (ਬਾਹਰਲਾ + ones, &exported_ਮਾਪ, 1, 1, 0, 0, ਅੰਕ);
    }

  mpz_clear (ਅੰਕ);
  *outlen = ones + exported_ਮਾਪ;
  ਵਾਪਸ true;
}


ਸਥਿਰ bool
base58_decode_ctx_finalize (ਬਣਤਰ ਅਧਾਰ_decode_context *ctx,
                            ਅੱਖਰ *ਸੀਮਿਤ *ਬਾਹਰਲਾ, idx_t *outlen)
{
  /* Ensure output buffer is large enough.
     Worst case is input is all '1's.  */
  idx_t max_outlen = ctx->ctx.base58.ਮਾਪ;
  ਜੇ (max_outlen > *outlen)
    {
      *ਬਾਹਰਲਾ = xrealloc (*ਬਾਹਰਲਾ, max_outlen);
      *outlen = max_outlen;
    }

  /* Ensure input buffer is NUL terminated (for mpz_get_str).  */
  ਜੇ (ctx->ctx.base58.ਮਾਪ)
    ctx->ctx.base58.buf[ctx->ctx.base58.ਮਾਪ] = '\0';

  bool ret = base58_decode ((ਅੱਖਰ *)ctx->ctx.base58.buf, ctx->ctx.base58.ਮਾਪ,
                            *ਬਾਹਰਲਾ, outlen);

  ਮੁਕਤ (ctx->ctx.base58.buf);
  ctx->ctx.base58.buf = ਨਿਰਾ;
  ctx->ctx.base58.ਮਾਪ = 0;
  ctx->ctx.base58.ਸਮਰੱਥਾ = 0;

  ਵਾਪਸ ret;
}

#ਜੇ_ਅੰਤ /* BASE_TYPE == 42, i.e., "basenc"*/



ਸਥਿਰ ਖਾਲੀ
wrap_write (ਅੱਖਰ ਅਚਲ *ਬਫਰ, idx_t len,
            idx_t wrap_ਕਾਲਮ, idx_t *current_ਕਾਲਮ, FILE *ਬਾਹਰਲਾ)
{
  ਜੇ (wrap_ਕਾਲਮ == 0)
    {
      /* Simple write. */
      ਜੇ (fwrite (ਬਫਰ, 1, len, stdout) < len)
        write_error ();
    }
  ਨਹੀਂਤਾਂ
    ਲਈ (idx_t written = 0; written < len; )
      {
        idx_t ਮੰਜ਼ਿਲ_write = MIN (wrap_ਕਾਲਮ - *current_ਕਾਲਮ, len - written);

        ਜੇ (ਮੰਜ਼ਿਲ_write == 0)
          {
            ਜੇ (fputc ('\n', ਬਾਹਰਲਾ) == EOF)
              write_error ();
            *current_ਕਾਲਮ = 0;
          }
        ਨਹੀਂਤਾਂ
          {
            ਜੇ (fwrite (ਬਫਰ + written, 1, ਮੰਜ਼ਿਲ_write, stdout) < ਮੰਜ਼ਿਲ_write)
              write_error ();
            *current_ਕਾਲਮ += ਮੰਜ਼ਿਲ_write;
            written += ਮੰਜ਼ਿਲ_write;
          }
      }
}

ਸਥਿਰ _Noreturn ਖਾਲੀ
ਅੰਤ_and_exit (FILE *ਵਿੱਚ, ਅੱਖਰ ਅਚਲ *infile)
{
  ਜੇ (fclose (ਵਿੱਚ) != 0)
    {
      ਜੇ (streq (infile, "-"))
        error (EXIT_FAILURE, errno, _("closing standard input"));
      ਨਹੀਂਤਾਂ
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
    }

  ਬਾਹਰ_ਨਿਕਲੋ (EXIT_SUCCESS);
}

ਸਥਿਰ _Noreturn ਖਾਲੀ
do_encode (FILE *ਵਿੱਚ, ਅੱਖਰ ਅਚਲ *infile, FILE *ਬਾਹਰਲਾ, idx_t wrap_ਕਾਲਮ)
{
  idx_t current_ਕਾਲਮ = 0;
  ਅੱਖਰ *inbuf, *outbuf;
  idx_t ਜੋੜ;

  inbuf = xmalloc (ENC_BLOCKSIZE);
  outbuf = xmalloc (ਅਧਾਰ_ਲੰਮਾਈ (ENC_BLOCKSIZE));

#ਜੇ ਅਧਾਰ_TYPE == 42
  /* Initialize encoding context if needed (for base58) */
  ਬਣਤਰ ਅਧਾਰ_encode_context encode_ctx;
  bool use_ctx = (ਅਧਾਰ_encode_ctx_init != ਨਿਰਾ);
  ਜੇ (use_ctx)
    ਅਧਾਰ_encode_ctx_init (&encode_ctx);
#ਜੇ_ਅੰਤ

  ਕਰੋ
    {
      idx_t n;

      ਜੋੜ = 0;
      ਕਰੋ
        {
          n = fread (inbuf + ਜੋੜ, 1, ENC_BLOCKSIZE - ਜੋੜ, ਵਿੱਚ);
          ਜੋੜ += n;
        }
      ਜਦੋਂ (!feof (ਵਿੱਚ) && !ferror (ਵਿੱਚ) && ਜੋੜ < ENC_BLOCKSIZE);

      ਜੇ (ਜੋੜ > 0)
        {
#ਜੇ ਅਧਾਰ_TYPE == 42
          ਜੇ (use_ctx)
            {
              idx_t outlen = 0;
              ਅਧਾਰ_encode_ctx (&encode_ctx, inbuf, ਜੋੜ, outbuf, &outlen);

              wrap_write (outbuf, outlen, wrap_ਕਾਲਮ, &current_ਕਾਲਮ, ਬਾਹਰਲਾ);
            }
          ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
            {
              /* Process input one block at a time.  Note that ENC_BLOCKSIZE
                 is sized so that no pad chars will appear in output. */
              ਅਧਾਰ_encode (inbuf, ਜੋੜ, outbuf, ਅਧਾਰ_ਲੰਮਾਈ (ਜੋੜ));

              wrap_write (outbuf, ਅਧਾਰ_ਲੰਮਾਈ (ਜੋੜ), wrap_ਕਾਲਮ,
                          &current_ਕਾਲਮ, ਬਾਹਰਲਾ);
            }
        }
    }
  ਜਦੋਂ (!feof (ਵਿੱਚ) && !ferror (ਵਿੱਚ) && ਜੋੜ == ENC_BLOCKSIZE);

#ਜੇ ਅਧਾਰ_TYPE == 42
  ਜੇ (use_ctx && ਅਧਾਰ_encode_ctx_finalize)
    {
      idx_t outlen = ਅਧਾਰ_ਲੰਮਾਈ (ENC_BLOCKSIZE);
      ਅਧਾਰ_encode_ctx_finalize (&encode_ctx, &outbuf, &outlen);

      wrap_write (outbuf, outlen, wrap_ਕਾਲਮ, &current_ਕਾਲਮ, ਬਾਹਰਲਾ);
    }
#ਜੇ_ਅੰਤ

  /* When wrapping, terminate last line. */
  ਜੇ (wrap_ਕਾਲਮ && current_ਕਾਲਮ > 0 && fputc ('\n', ਬਾਹਰਲਾ) == EOF)
    write_error ();

  ਜੇ (ferror (ਵਿੱਚ))
    error (EXIT_FAILURE, errno, _("read error"));

  ਅੰਤ_and_exit (ਵਿੱਚ, infile);
}

ਸਥਿਰ _Noreturn ਖਾਲੀ
do_decode (FILE *ਵਿੱਚ, ਅੱਖਰ ਅਚਲ *infile, FILE *ਬਾਹਰਲਾ, bool ignore_garbage)
{
  ਅੱਖਰ *inbuf, *outbuf;
  idx_t ਜੋੜ;
  ਬਣਤਰ ਅਧਾਰ_decode_context ctx;

  inbuf = xmalloc (ਅਧਾਰ_ਲੰਮਾਈ (DEC_BLOCKSIZE));
  outbuf = xmalloc (DEC_BLOCKSIZE);

#ਜੇ ਅਧਾਰ_TYPE == 42
  ctx.inbuf = ਨਿਰਾ;
#ਜੇ_ਅੰਤ
  ਅਧਾਰ_decode_ctx_init (&ctx);

  ਕਰੋ
    {
      bool ok;

      ਜੋੜ = 0;
      ਕਰੋ
        {
          idx_t n = fread (inbuf + ਜੋੜ,
                           1, ਅਧਾਰ_ਲੰਮਾਈ (DEC_BLOCKSIZE) - ਜੋੜ, ਵਿੱਚ);

          ਜੇ (ignore_garbage)
            {
              ਲਈ (idx_t i = 0; n > 0 && i < n;)
                {
                  ਜੇ (isubase (inbuf[ਜੋੜ + i])
                      || (REQUIRED_PADDING (1) && inbuf[ਜੋੜ + i] == '='))
                    i++;
                  ਨਹੀਂਤਾਂ
                    memmove (inbuf + ਜੋੜ + i, inbuf + ਜੋੜ + i + 1, --n - i);
                }
            }

          ਜੋੜ += n;

          ਜੇ (ferror (ਵਿੱਚ))
            error (EXIT_FAILURE, errno, _("read error"));
        }
      ਜਦੋਂ (ਜੋੜ < ਅਧਾਰ_ਲੰਮਾਈ (DEC_BLOCKSIZE) && !feof (ਵਿੱਚ));

      ਜਦੋਂ (ਜੋੜ || feof (ਵਿੱਚ))
        {
          idx_t n = DEC_BLOCKSIZE;
          ਜੇ (ਜੋੜ)
            ok = ਅਧਾਰ_decode_ctx (&ctx, inbuf, ਜੋੜ, outbuf, &n);
          ਨਹੀਂਤਾਂ
            ok = ਅਧਾਰ_decode_ctx_finalize (&ctx, &outbuf, &n);

          ਜੇ (fwrite (outbuf, 1, n, ਬਾਹਰਲਾ) < n)
            write_error ();

          ਜੇ (! ok)
            error (EXIT_FAILURE, 0, _("invalid input"));

          ਜੇ (ਜੋੜ == 0)
            ਰੋਕੋ;
          ਜੋੜ = 0;
        }
    }
  ਜਦੋਂ (!feof (ਵਿੱਚ));

  ਅੰਤ_and_exit (ਵਿੱਚ, infile);
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਪੂਰਨ opt;
  FILE *ਨਿਵੇਸ਼_fh;
  ਅੱਖਰ ਅਚਲ *infile;

  /* True if --decode has been given and we should decode data. */
  bool decode = false;
  /* True if we should ignore non-base-alphabetic characters. */
  bool ignore_garbage = false;
  /* Wrap encoded data around the 76th column, by default. */
  idx_t wrap_ਕਾਲਮ = 76;

#ਜੇ ਅਧਾਰ_TYPE == 42
  ਪੂਰਨ ਅਧਾਰ_type = 0;
#ਜੇ_ਅੰਤ

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ਜਦੋਂ ((opt = getopt_long (argc, argv, "diw:", long_options, ਨਿਰਾ)) != -1)
    ਚੋਣ (opt)
      {
      ਹਾਲਤ 'd':
        decode = true;
        ਰੋਕੋ;

      ਹਾਲਤ 'w':
        {
          intmax_t w;
          strtol_error s_err = xstrtoimax (optarg, ਨਿਰਾ, 10, &w, "");
          ਜੇ (LONGINT_ਉਛਾਲ < s_err || w < 0)
            error (EXIT_FAILURE, 0, "%s: %s",
                   _("invalid wrap size"), quote (optarg));
          wrap_ਕਾਲਮ = s_err == LONGINT_ਉਛਾਲ || IDX_MAX < w ? 0 : w;
        }
        ਰੋਕੋ;

      ਹਾਲਤ 'i':
        ignore_garbage = true;
        ਰੋਕੋ;

#ਜੇ ਅਧਾਰ_TYPE == 42
      ਹਾਲਤ BASE64_OPTION:
      ਹਾਲਤ BASE64URL_OPTION:
      ਹਾਲਤ BASE32_OPTION:
      ਹਾਲਤ BASE32HEX_OPTION:
      ਹਾਲਤ BASE16_OPTION:
      ਹਾਲਤ BASE2MSBF_OPTION:
      ਹਾਲਤ BASE2LSBF_OPTION:
      ਹਾਲਤ Z85_OPTION:
      ਹਾਲਤ BASE58_OPTION:
        ਅਧਾਰ_type = opt;
        ਰੋਕੋ;
#ਜੇ_ਅੰਤ

      case_GETOPT_HELP_CHAR;

      case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);

      ਮੂਲ:
        usage (EXIT_FAILURE);
        ਰੋਕੋ;
      }

#ਜੇ ਅਧਾਰ_TYPE == 42
  required_padding = no_required_padding;
  has_padding = no_padding;
  get_pending_ਲੰਮਾਈ = no_pending_ਲੰਮਾਈ;
  ਅਧਾਰ_decode_ctx_finalize = decode_ctx_finalize;

  ਚੋਣ (ਅਧਾਰ_type)
    {
    ਹਾਲਤ BASE64_OPTION:
      ਅਧਾਰ_ਲੰਮਾਈ_2 = base64_ਲੰਮਾਈ_wrapper;
      required_padding = base64_required_padding;
      has_padding = base64_ctx_has_padding;
      get_pending_ਲੰਮਾਈ = base64_ctx_get_pending_ਲੰਮਾਈ;
      isubase = isubase64;
      ਅਧਾਰ_encode = base64_encode;
      ਅਧਾਰ_decode_ctx_init = base64_decode_ctx_init_wrapper;
      ਅਧਾਰ_decode_ctx = base64_decode_ctx_wrapper;
      ਰੋਕੋ;

    ਹਾਲਤ BASE64URL_OPTION:
      ਅਧਾਰ_ਲੰਮਾਈ_2 = base64_ਲੰਮਾਈ_wrapper;
      required_padding = base64_required_padding;
      has_padding = base64_ctx_has_padding;
      get_pending_ਲੰਮਾਈ = base64_ctx_get_pending_ਲੰਮਾਈ;
      isubase = isubase64url;
      ਅਧਾਰ_encode = base64url_encode;
      ਅਧਾਰ_decode_ctx_init = base64url_decode_ctx_init_wrapper;
      ਅਧਾਰ_decode_ctx = base64url_decode_ctx_wrapper;
      ਰੋਕੋ;

    ਹਾਲਤ BASE32_OPTION:
      ਅਧਾਰ_ਲੰਮਾਈ_2 = base32_ਲੰਮਾਈ_wrapper;
      required_padding = base32_required_padding;
      has_padding = base32_ctx_has_padding;
      get_pending_ਲੰਮਾਈ = base32_ctx_get_pending_ਲੰਮਾਈ;
      isubase = isubase32;
      ਅਧਾਰ_encode = base32_encode;
      ਅਧਾਰ_decode_ctx_init = base32_decode_ctx_init_wrapper;
      ਅਧਾਰ_decode_ctx = base32_decode_ctx_wrapper;
      ਰੋਕੋ;

    ਹਾਲਤ BASE32HEX_OPTION:
      ਅਧਾਰ_ਲੰਮਾਈ_2 = base32_ਲੰਮਾਈ_wrapper;
      required_padding = base32_required_padding;
      has_padding = base32_ctx_has_padding;
      get_pending_ਲੰਮਾਈ = base32_ctx_get_pending_ਲੰਮਾਈ;
      isubase = isubase32hex;
      ਅਧਾਰ_encode = base32hex_encode;
      ਅਧਾਰ_decode_ctx_init = base32hex_decode_ctx_init_wrapper;
      ਅਧਾਰ_decode_ctx = base32hex_decode_ctx_wrapper;
      ਰੋਕੋ;

    ਹਾਲਤ BASE16_OPTION:
      ਅਧਾਰ_ਲੰਮਾਈ_2 = base16_ਲੰਮਾਈ;
      get_pending_ਲੰਮਾਈ = base16_ctx_get_pending_ਲੰਮਾਈ;
      isubase = isubase16;
      ਅਧਾਰ_encode = base16_encode;
      ਅਧਾਰ_decode_ctx_init = base16_decode_ctx_init;
      ਅਧਾਰ_decode_ctx = base16_decode_ctx;
      ਰੋਕੋ;

    ਹਾਲਤ BASE2MSBF_OPTION:
      ਅਧਾਰ_ਲੰਮਾਈ_2 = base2_ਲੰਮਾਈ;
      get_pending_ਲੰਮਾਈ = base2_ctx_get_pending_ਲੰਮਾਈ;
      isubase = isubase2;
      ਅਧਾਰ_encode = base2msbf_encode;
      ਅਧਾਰ_decode_ctx_init = base2_decode_ctx_init;
      ਅਧਾਰ_decode_ctx = base2msbf_decode_ctx;
      ਰੋਕੋ;

    ਹਾਲਤ BASE2LSBF_OPTION:
      ਅਧਾਰ_ਲੰਮਾਈ_2 = base2_ਲੰਮਾਈ;
      get_pending_ਲੰਮਾਈ = base2_ctx_get_pending_ਲੰਮਾਈ;
      isubase = isubase2;
      ਅਧਾਰ_encode = base2lsbf_encode;
      ਅਧਾਰ_decode_ctx_init = base2_decode_ctx_init;
      ਅਧਾਰ_decode_ctx = base2lsbf_decode_ctx;
      ਰੋਕੋ;

    ਹਾਲਤ Z85_OPTION:
      ਅਧਾਰ_ਲੰਮਾਈ_2 = z85_ਲੰਮਾਈ;
      get_pending_ਲੰਮਾਈ = z85_ctx_get_pending_ਲੰਮਾਈ;
      isubase = isuz85;
      ਅਧਾਰ_encode = z85_encode;
      ਅਧਾਰ_decode_ctx_init = z85_decode_ctx_init;
      ਅਧਾਰ_decode_ctx = z85_decode_ctx;
      ਰੋਕੋ;

    ਹਾਲਤ BASE58_OPTION:
      ਅਧਾਰ_ਲੰਮਾਈ_2 = base58_ਲੰਮਾਈ;
      isubase = isubase58;
      ਅਧਾਰ_encode_ctx_init = base58_encode_ctx_init;
      ਅਧਾਰ_encode_ctx = base58_encode_ctx;
      ਅਧਾਰ_encode_ctx_finalize = base58_encode_ctx_finalize;
      ਅਧਾਰ_decode_ctx_init = base58_decode_ctx_init;
      ਅਧਾਰ_decode_ctx = base58_decode_ctx;
      ਅਧਾਰ_decode_ctx_finalize = base58_decode_ctx_finalize;
      ਰੋਕੋ;

    ਮੂਲ:
      error (0, 0, _("missing encoding type"));
      usage (EXIT_FAILURE);
    }
#ਜੇ_ਅੰਤ

  ਜੇ (argc - optind > 1)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }

  ਜੇ (optind < argc)
    infile = argv[optind];
  ਨਹੀਂਤਾਂ
    infile = "-";

  ਜੇ (streq (infile, "-"))
    {
      xset_ਦੁਆਧਾਰੀ_ਬਹੁਲਕ (STDIN_FILENO, O_ਦੁਆਧਾਰੀ);
      ਨਿਵੇਸ਼_fh = stdin;
    }
  ਨਹੀਂਤਾਂ
    {
      ਨਿਵੇਸ਼_fh = fopen (infile, "rb");
      ਜੇ (ਨਿਵੇਸ਼_fh == ਨਿਰਾ)
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
    }

  fadvise (ਨਿਵੇਸ਼_fh, FADVISE_SEQUENTIAL);

  ਜੇ (decode)
    do_decode (ਨਿਵੇਸ਼_fh, infile, stdout, ignore_garbage);
  ਨਹੀਂਤਾਂ
    do_encode (ਨਿਵੇਸ਼_fh, infile, stdout, wrap_ਕਾਲਮ);
}
