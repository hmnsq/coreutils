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

#ಅನ್ತರ್ಭಾವ <config.h>

#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <gmp.h>

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "assure.h"
#ಅನ್ತರ್ಭಾವ "c-ctype.h"
#ಅನ್ತರ್ಭಾವ "fadvise.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "xstrtol.h"
#ಅನ್ತರ್ಭಾವ "xbinary-io.h"

#ಆದರೆ ತಳ_TYPE == 42
# define AUTHORS \
  proper_name ("Simon Josefsson"), \
  proper_name ("Assaf Gordon")
#ಇಲ್ಲದಿದ್ದರೆ
# define AUTHORS proper_name ("Simon Josefsson")
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ ತಳ_TYPE == 32
# include "base32.h"
# define ಕಾರ್ಯಕ್ರಮ_NAME "base32"
#ಅಥವಾಆದರೆ ತಳ_TYPE == 64
# include "base64.h"
# define ಕಾರ್ಯಕ್ರಮ_NAME "base64"
#ಅಥವಾಆದರೆ ತಳ_TYPE == 42
# include "base32.h"
# include "base64.h"
# include "assure.h"
# define ಕಾರ್ಯಕ್ರಮ_NAME "basenc"
#ಇಲ್ಲದಿದ್ದರೆ
# error missing/invalid ತಳ_TYPE definition
#ಆದರೆ_ಅಂತ್ಯ



#ಆದರೆ ತಳ_TYPE == 42
ಎಣಿಕೆ
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
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ long_options[] =
{
  {"decode", no_ವಾದ, ಖಾಲಿ, 'd'},
  {"wrap", required_ವಾದ, ಖಾಲಿ, 'w'},
  {"ignore-garbage", no_ವಾದ, ಖಾಲಿ, 'i'},
#ಆದರೆ ತಳ_TYPE == 42
  {"base64",    no_ವಾದ, ಖಾಲಿ, BASE64_OPTION},
  {"base64url", no_ವಾದ, ಖಾಲಿ, BASE64URL_OPTION},
  {"base58",    no_ವಾದ, ಖಾಲಿ, BASE58_OPTION},
  {"base32",    no_ವಾದ, ಖಾಲಿ, BASE32_OPTION},
  {"base32hex", no_ವಾದ, ಖಾಲಿ, BASE32HEX_OPTION},
  {"base16",    no_ವಾದ, ಖಾಲಿ, BASE16_OPTION},
  {"base2msbf", no_ವಾದ, ಖಾಲಿ, BASE2MSBF_OPTION},
  {"base2lsbf", no_ವಾದ, ಖಾಲಿ, BASE2LSBF_OPTION},
  {"z85",       no_ವಾದ, ಖಾಲಿ, Z85_OPTION},
#ಆದರೆ_ಅಂತ್ಯ
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("\
Usage: %s [OPTION]... [FILE]\n\
"), ಕಾರ್ಯಕ್ರಮ_name);

#ಆದರೆ ತಳ_TYPE == 42
      fputs (_("\
basenc encode or decode FILE, or standard ಒಳಹರಿವು, ಗಮ್ಯ standard output.\n\
"), stdout);
#ಇಲ್ಲದಿದ್ದರೆ
      ಬರೆ (_("\
ತಳ%d encode or decode FILE, or standard ಒಳಹರಿವು, ಗಮ್ಯ standard output.\n\
"), ತಳ_TYPE);
#ಆದರೆ_ಅಂತ್ಯ

      emit_stdin_note ();
      emit_mandatory_arg_note ();
#ಆದರೆ ತಳ_TYPE == 42
      oputs (_("\
      --base64\n\
         same as base64(1) ಕಾರ್ಯಕ್ರಮ (RFC4648 section 4)\n\
"));
      oputs (_("\
      --base64url\n\
         file- and url-ಸುರಕ್ಷಿತ base64 (RFC4648 section 5)\n\
"));
      oputs (_("\
      --base58\n\
         visually unambiguous base58 encoding\n\
"));
      oputs (_("\
      --base32\n\
         same as base32(1) ಕಾರ್ಯಕ್ರಮ (RFC4648 section 6)\n\
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
         ಬಿಟ್ string with most significant ಬಿಟ್ (msb) ಮೊದಲ\n\
"));
      oputs (_("\
      --base2lsbf\n\
         ಬಿಟ್ string with ಕನಿಷ್ಠ significant ಬಿಟ್ (lsb) ಮೊದಲ\n\
"));
#ಆದರೆ_ಅಂತ್ಯ
      oputs (_("\
  -d, --decode\n\
         decode ದತ್ತಾಂಶ\n\
"));
      oputs (_("\
  -i, --ignore-garbage\n\
         when decoding, ignore non-alphabet characters\n\
"));
      oputs (_("\
  -w, --wrap=ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ\n\
         wrap encoded ಸಾಲುಗಳು after ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ character (ಪೂರ್ವನಿಯೋಜಿತ 76).\n\
         Use 0 ಗಮ್ಯ disable ಸಾಲು wrapping\n\
"));
#ಆದರೆ ತಳ_TYPE == 42
      oputs (_("\
      --z85\n\
         ascii85-like encoding (ZeroMQ spec:32/Z85);\n\
         when encoding, ಒಳಹರಿವು ಉದ್ದಳತೆ must be a ಅಪವರ್ತ್ಯ ದ 4;\n\
         when decoding, ಒಳಹರಿವು ಉದ್ದಳತೆ must be a ಅಪವರ್ತ್ಯ ದ 5\n\
"));
#ಆದರೆ_ಅಂತ್ಯ
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
#ಆದರೆ ತಳ_TYPE == 42
      fputs (_("\
\n\
When decoding, the ಒಳಹರಿವು may contain newlines ಒಳ addition ಗಮ್ಯ the ಬೈಟುಗಳು ದ\n\
the formal alphabet.  Use --ignore-garbage ಗಮ್ಯ attempt ಗಮ್ಯ recover\n\
from any other non-alphabet ಬೈಟುಗಳು ಒಳ the encoded ಹರಿವು.\n\
"), stdout);
#ಇಲ್ಲದಿದ್ದರೆ
      ಬರೆ (_("\
\n\
The ದತ್ತಾಂಶ are encoded as described ಗಾಗಿ the %s alphabet ಒಳ RFC 4648.\n\
When decoding, the ಒಳಹರಿವು may contain newlines ಒಳ addition ಗಮ್ಯ the ಬೈಟುಗಳು ದ\n\
the formal %s alphabet.  Use --ignore-garbage ಗಮ್ಯ attempt ಗಮ್ಯ recover\n\
from any other non-alphabet ಬೈಟುಗಳು ಒಳ the encoded ಹರಿವು.\n"),
              ಕಾರ್ಯಕ್ರಮ_NAME, ಕಾರ್ಯಕ್ರಮ_NAME);
#ಆದರೆ_ಅಂತ್ಯ
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }

  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

#ಆದರೆ ತಳ_TYPE != 64
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
base32_required_padding (ಪೂರ್ಣಾಂಕ len)
{
  ಪೂರ್ಣಾಂಕ partial = len % 8;
  ಹಿಂತಿರುಗಿ partial ? 8 - partial : 0;
}
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ ತಳ_TYPE != 32
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
base64_required_padding (ಪೂರ್ಣಾಂಕ len)
{
  ಪೂರ್ಣಾಂಕ partial = len % 4;
  ಹಿಂತಿರುಗಿ partial ? 4 - partial : 0;
}
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ ತಳ_TYPE == 42
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
no_required_padding (MAYBE_ಬಳಸದ ಪೂರ್ಣಾಂಕ len)
{
  ಹಿಂತಿರುಗಿ 0;
}
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು ENC_BLOCKSIZE (1024 * 3 * 10)

#ಆದರೆ ತಳ_TYPE == 32
# define ತಳ_ಉದ್ದಳತೆ BASE32_ಉದ್ದಳತೆ
# define REQUIRED_PADDING base32_required_padding
/* Note that increasing this may decrease performance if --ignore-garbage
   is used, because of the memmove operation below.  */
# define DEC_BLOCKSIZE (1024 * 5)

/* Ensure that BLOCKSIZE is a multiple of 5 and 8.  */
static_assert (ENC_BLOCKSIZE % 40 == 0); /* Padding chars only on last block. */
static_assert (DEC_BLOCKSIZE % 40 == 0); /* Complete encoded blocks are used. */

# define ತಳ_encode base32_encode
# define ತಳ_decode_context base32_decode_context
# define ತಳ_decode_ctx_init base32_decode_ctx_init
# define ತಳ_decode_ctx base32_decode_ctx
# define ತಳ_decode_ctx_finalize decode_ctx_finalize
# define isubase isubase32
#ಅಥವಾಆದರೆ ತಳ_TYPE == 64
# define ತಳ_ಉದ್ದಳತೆ BASE64_ಉದ್ದಳತೆ
# define REQUIRED_PADDING base64_required_padding
/* Note that increasing this may decrease performance if --ignore-garbage
   is used, because of the memmove operation below.  */
# define DEC_BLOCKSIZE (1024 * 3)

/* Ensure that BLOCKSIZE is a multiple of 3 and 4.  */
static_assert (ENC_BLOCKSIZE % 12 == 0); /* Padding chars only on last block. */
static_assert (DEC_BLOCKSIZE % 12 == 0); /* Complete encoded blocks are used. */

# define ತಳ_encode base64_encode
# define ತಳ_decode_context base64_decode_context
# define ತಳ_decode_ctx_init base64_decode_ctx_init
# define ತಳ_decode_ctx base64_decode_ctx
# define ತಳ_decode_ctx_finalize decode_ctx_finalize
# define isubase isubase64
#ಅಥವಾಆದರೆ ತಳ_TYPE == 42


# define ತಳ_ಉದ್ದಳತೆ ತಳ_ಉದ್ದಳತೆ_2
# define REQUIRED_PADDING required_padding

/* Note that increasing this may decrease performance if --ignore-garbage
   is used, because of the memmove operation below.  */
# define DEC_BLOCKSIZE (4200)
static_assert (DEC_BLOCKSIZE % 40 == 0); /* complete encoded blocks for base32*/
static_assert (DEC_BLOCKSIZE % 12 == 0); /* complete encoded blocks for base64*/

ಸ್ಥಿರ idx_t (*ತಳ_ಉದ್ದಳತೆ_2) (idx_t len);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ (*required_padding) (ಪೂರ್ಣಾಂಕ i);
ಸ್ಥಿರ bool (*isubase) (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ch);
ಸ್ಥಿರ ಶೂನ್ಯ (*ತಳ_encode) (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸೀಮಿತ ಒಳ, idx_t inlen,
                            ಅಕ್ಷರ *ಸೀಮಿತ ಹೊರಹೋಗು, idx_t outlen);

ರಚನೆ base16_decode_context
{
  /* Either a 4-bit nibble, or negative if we have no nibble.  */
  ಚಿಹ್ನೆಯುಳ್ಳ ಅಕ್ಷರ nibble;
};

ರಚನೆ z85_decode_context
{
  ಪೂರ್ಣಾಂಕ i;
  ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ octets[5];
};

ರಚನೆ base58_context
{
  idx_t ಗಾತ್ರ_2;
  idx_t ಸಾಮರ್ಥ್ಯ;
  ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *buf COUNTED_BY (ಸಾಮರ್ಥ್ಯ);
};

ರಚನೆ base2_decode_context
{
  ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ octet;
  ಪೂರ್ಣಾಂಕ ಬಿಟ್_pos;
};

ರಚನೆ ತಳ_decode_context
{
  ಒಕ್ಕೂಟ {
    ರಚನೆ base64_decode_context base64;
    ರಚನೆ base32_decode_context base32;
    ರಚನೆ base16_decode_context base16;
    ರಚನೆ base2_decode_context base2;
    ರಚನೆ z85_decode_context z85;
    ರಚನೆ base58_context base58;
  } ctx;
  ಅಕ್ಷರ *inbuf;
  idx_t bufsize;
};
ಸ್ಥಿರ ಶೂನ್ಯ (*ತಳ_decode_ctx_init) (ರಚನೆ ತಳ_decode_context *ctx);
ಸ್ಥಿರ bool (*ತಳ_decode_ctx) (ರಚನೆ ತಳ_decode_context *ctx,
                                ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸೀಮಿತ ಒಳ, idx_t inlen,
                                ಅಕ್ಷರ *ಸೀಮಿತ ಹೊರಹೋಗು, idx_t *outlen);
ಸ್ಥಿರ bool (*ತಳ_decode_ctx_finalize) (ರಚನೆ ತಳ_decode_context *ctx,
                                         ಅಕ್ಷರ *ಸೀಮಿತ *ಹೊರಹೋಗು, idx_t *outlen);

ರಚನೆ ತಳ_encode_context
{
  ಒಕ್ಕೂಟ {
    ರಚನೆ base58_context base58;
  } ctx;
};

ಸ್ಥಿರ ಶೂನ್ಯ (*ತಳ_encode_ctx_init) (ರಚನೆ ತಳ_encode_context *ctx);
ಸ್ಥಿರ bool (*ತಳ_encode_ctx) (ರಚನೆ ತಳ_encode_context *ctx,
                                ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸೀಮಿತ ಒಳ, idx_t inlen,
                                ಅಕ್ಷರ *ಸೀಮಿತ ಹೊರಹೋಗು, idx_t *outlen);
ಸ್ಥಿರ bool (*ತಳ_encode_ctx_finalize) (ರಚನೆ ತಳ_encode_context *ctx,
                                         ಅಕ್ಷರ *ಸೀಮಿತ *ಹೊರಹೋಗು, idx_t *outlen);

ಸ್ಥಿರ bool
no_padding (MAYBE_ಬಳಸದ ರಚನೆ ತಳ_decode_context *ctx)
{
  ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
no_pending_ಉದ್ದಳತೆ (MAYBE_ಬಳಸದ ರಚನೆ ತಳ_decode_context *ctx)
{
  ಹಿಂತಿರುಗಿ 0;
}
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ ತಳ_TYPE == 42
ಸ್ಥಿರ bool (*has_padding) (ರಚನೆ ತಳ_decode_context *ctx);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ (*get_pending_ಉದ್ದಳತೆ) (ರಚನೆ ತಳ_decode_context *ctx);

ಸ್ಥಿರ bool
base64_ctx_has_padding (ರಚನೆ ತಳ_decode_context *ctx)
{
  ಹಿಂತಿರುಗಿ ctx->ctx.base64.i && ctx->ctx.base64.buf[ctx->ctx.base64.i - 1] == '=';
}

ಸ್ಥಿರ bool
base32_ctx_has_padding (ರಚನೆ ತಳ_decode_context *ctx)
{
  ಹಿಂತಿರುಗಿ ctx->ctx.base32.i && ctx->ctx.base32.buf[ctx->ctx.base32.i - 1] == '=';
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
base64_ctx_get_pending_ಉದ್ದಳತೆ (ರಚನೆ ತಳ_decode_context *ctx)
{
  ಹಿಂತಿರುಗಿ ctx->ctx.base64.i;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
base32_ctx_get_pending_ಉದ್ದಳತೆ (ರಚನೆ ತಳ_decode_context *ctx)
{
  ಹಿಂತಿರುಗಿ ctx->ctx.base32.i;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
base16_ctx_get_pending_ಉದ್ದಳತೆ (MAYBE_ಬಳಸದ ರಚನೆ ತಳ_decode_context *ctx)
{
  ಹಿಂತಿರುಗಿ 1; /* Always check nibble state.  */
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
z85_ctx_get_pending_ಉದ್ದಳತೆ (ರಚನೆ ತಳ_decode_context *ctx)
{
  ಹಿಂತಿರುಗಿ ctx->ctx.z85.i;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
base2_ctx_get_pending_ಉದ್ದಳತೆ (ರಚನೆ ತಳ_decode_context *ctx)
{
  ಹಿಂತಿರುಗಿ ctx->ctx.base2.ಬಿಟ್_pos;
}
#ಇಲ್ಲದಿದ್ದರೆ
ಸ್ಥಿರ bool
has_padding (ರಚನೆ ತಳ_decode_context *ctx)
{
  ಹಿಂತಿರುಗಿ ctx->i && ctx->buf[ctx->i - 1] == '=';
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
get_pending_ಉದ್ದಳತೆ (ರಚನೆ ತಳ_decode_context *ctx)
{
  ಹಿಂತಿರುಗಿ ctx->i;
}
#ಆದರೆ_ಅಂತ್ಯ


/* Process any pending data in CTX, while auto padding if appropriate.
   Return TRUE on success, FALSE on failure.  */

ಸ್ಥಿರ bool
decode_ctx_finalize (ರಚನೆ ತಳ_decode_context *ctx,
                     ಅಕ್ಷರ *ಸೀಮಿತ *ಹೊರಹೋಗು, idx_t *outlen)
{
  ಆದರೆ (get_pending_ಉದ್ದಳತೆ (ctx) == 0)
    {
      *outlen = 0;
      ಹಿಂತಿರುಗಿ true;
    }

  /* Auto-pad input and flush the context */
  ಅಕ್ಷರ padbuf[8] ATTRIBUTE_NONSTRING = "========";
  idx_t pending_len = get_pending_ಉದ್ದಳತೆ (ctx);
  idx_t auto_padding = REQUIRED_PADDING (pending_len);
  idx_t n = *outlen;
  bool ಫಲಿತಾಂಶ;

  ಆದರೆ (auto_padding && ! has_padding (ctx))
    {
      affirm (auto_padding <= ಗಾತ್ರ (padbuf));
      ಫಲಿತಾಂಶ = ತಳ_decode_ctx (ctx, padbuf, auto_padding, *ಹೊರಹೋಗು, &n);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಫಲಿತಾಂಶ = ತಳ_decode_ctx (ctx, "", 0, *ಹೊರಹೋಗು, &n);
    }

  *outlen = n;
  ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

#ಆದರೆ ತಳ_TYPE == 42

ಸ್ಥಿರ idx_t
base64_ಉದ್ದಳತೆ_wrapper (idx_t len)
{
  ಹಿಂತಿರುಗಿ BASE64_ಉದ್ದಳತೆ (len);
}

ಸ್ಥಿರ ಶೂನ್ಯ
base64_decode_ctx_init_wrapper (ರಚನೆ ತಳ_decode_context *ctx)
{
  base64_decode_ctx_init (&ctx->ctx.base64);
}

ಸ್ಥಿರ bool
base64_decode_ctx_wrapper (ರಚನೆ ತಳ_decode_context *ctx,
                           ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸೀಮಿತ ಒಳ, idx_t inlen,
                           ಅಕ್ಷರ *ಸೀಮಿತ ಹೊರಹೋಗು, idx_t *outlen)
{
  ಹಿಂತಿರುಗಿ base64_decode_ctx (&ctx->ctx.base64, ಒಳ, inlen, ಹೊರಹೋಗು, outlen);
}

ಸ್ಥಿರ ಶೂನ್ಯ
init_inbuf (ರಚನೆ ತಳ_decode_context *ctx)
{
  ctx->bufsize = DEC_BLOCKSIZE;
  ctx->inbuf = xcharalloc (ctx->bufsize);
}

ಸ್ಥಿರ ಶೂನ್ಯ
prepare_inbuf (ರಚನೆ ತಳ_decode_context *ctx, idx_t inlen)
{
  ಆದರೆ (ctx->bufsize < inlen)
    ctx->inbuf = xpalloc (ctx->inbuf, &ctx->bufsize,
                          inlen - ctx->bufsize, -1, ಗಾತ್ರ *ctx->inbuf);
}


ಸ್ಥಿರ ಶೂನ್ಯ
base64url_encode (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸೀಮಿತ ಒಳ, idx_t inlen,
                  ಅಕ್ಷರ *ಸೀಮಿತ ಹೊರಹೋಗು, idx_t outlen)
{
  base64_encode (ಒಳ, inlen, ಹೊರಹೋಗು, outlen);
  /* translate 62nd and 63rd characters */
  ಅಕ್ಷರ *p = ಹೊರಹೋಗು;
  ಆಗ (outlen--)
    {
      ಆದರೆ (*p == '+')
        *p = '-';
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (*p == '/')
        *p = '_';
      ++p;
    }
}

ಸ್ಥಿರ bool
isubase64url (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ch)
{
  ಹಿಂತಿರುಗಿ (ch == '-' || ch == '_'
          || (ch != '+' && ch != '/' && isubase64 (ch)));
}

ಸ್ಥಿರ ಶೂನ್ಯ
base64url_decode_ctx_init_wrapper (ರಚನೆ ತಳ_decode_context *ctx)
{
  base64_decode_ctx_init (&ctx->ctx.base64);
  init_inbuf (ctx);
}


ಸ್ಥಿರ bool
base64url_decode_ctx_wrapper (ರಚನೆ ತಳ_decode_context *ctx,
                              ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸೀಮಿತ ಒಳ, idx_t inlen,
                              ಅಕ್ಷರ *ಸೀಮಿತ ಹೊರಹೋಗು, idx_t *outlen)
{
  prepare_inbuf (ctx, inlen);
  ಸ್ಮರಣೆ_ನಕಲು (ctx->inbuf, ಒಳ, inlen);

  /* translate 62nd and 63rd characters */
  idx_t i = inlen;
  ಅಕ್ಷರ *p = ctx->inbuf;
  ಆಗ (i--)
    {
      ಆದರೆ (*p == '+' || *p == '/')
        {
          *outlen = 0;
          ಹಿಂತಿರುಗಿ false; /* reject base64 input */
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (*p == '-')
        *p = '+';
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (*p == '_')
        *p = '/';
      ++p;
    }

  ಹಿಂತಿರುಗಿ base64_decode_ctx (&ctx->ctx.base64, ctx->inbuf, inlen,
                            ಹೊರಹೋಗು, outlen);
}



ಸ್ಥಿರ idx_t
base32_ಉದ್ದಳತೆ_wrapper (idx_t len)
{
  ಹಿಂತಿರುಗಿ BASE32_ಉದ್ದಳತೆ (len);
}

ಸ್ಥಿರ ಶೂನ್ಯ
base32_decode_ctx_init_wrapper (ರಚನೆ ತಳ_decode_context *ctx)
{
  base32_decode_ctx_init (&ctx->ctx.base32);
}

ಸ್ಥಿರ bool
base32_decode_ctx_wrapper (ರಚನೆ ತಳ_decode_context *ctx,
                           ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸೀಮಿತ ಒಳ, idx_t inlen,
                           ಅಕ್ಷರ *ಸೀಮಿತ ಹೊರಹೋಗು, idx_t *outlen)
{
  ಹಿಂತಿರುಗಿ base32_decode_ctx (&ctx->ctx.base32, ಒಳ, inlen, ಹೊರಹೋಗು, outlen);
}

/* ABCDEFGHIJKLMNOPQRSTUVWXYZ234567
ಗಮ್ಯ
   0123456789ABCDEFGHIJKLMNOPQRSTUV */
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ base32_norm_ಗಮ್ಯ_hex[32 + 9] = {
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
ಗಮ್ಯ
   ABCDEFGHIJKLMNOPQRSTUVWXYZ234567 */
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ base32_hex_ಗಮ್ಯ_norm[32 + 9] = {
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


ಸಾಲೊಳಗೆ ಸ್ಥಿರ bool
isubase32hex (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ch)
{
  ಹಿಂತಿರುಗಿ ('0' <= ch && ch <= '9') || ('A' <= ch && ch <= 'V');
}


ಸ್ಥಿರ ಶೂನ್ಯ
base32hex_encode (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸೀಮಿತ ಒಳ, idx_t inlen,
                  ಅಕ್ಷರ *ಸೀಮಿತ ಹೊರಹೋಗು, idx_t outlen)
{
  base32_encode (ಒಳ, inlen, ಹೊರಹೋಗು, outlen);

  ಗಾಗಿ (ಅಕ್ಷರ *p = ಹೊರಹೋಗು; outlen--; p++)
    {
      affirm (0x32 <= *p && *p <= 0x5a);          /* LCOV_EXCL_LINE */
      *p = base32_norm_ಗಮ್ಯ_hex[*p - 0x32];
    }
}


ಸ್ಥಿರ ಶೂನ್ಯ
base32hex_decode_ctx_init_wrapper (ರಚನೆ ತಳ_decode_context *ctx)
{
  base32_decode_ctx_init (&ctx->ctx.base32);
  init_inbuf (ctx);
}


ಸ್ಥಿರ bool
base32hex_decode_ctx_wrapper (ರಚನೆ ತಳ_decode_context *ctx,
                              ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸೀಮಿತ ಒಳ, idx_t inlen,
                              ಅಕ್ಷರ *ಸೀಮಿತ ಹೊರಹೋಗು, idx_t *outlen)
{
  prepare_inbuf (ctx, inlen);

  idx_t i = inlen;
  ಅಕ್ಷರ *p = ctx->inbuf;
  ಆಗ (i--)
    {
      ಆದರೆ (isubase32hex (*ಒಳ))
        *p = base32_hex_ಗಮ್ಯ_norm[*ಒಳ - 0x30];
      ಇಲ್ಲದಿದ್ದರೆ
        *p = *ಒಳ;
      ++p;
      ++ಒಳ;
    }

  ಹಿಂತಿರುಗಿ base32_decode_ctx (&ctx->ctx.base32, ctx->inbuf, inlen,
                            ಹೊರಹೋಗು, outlen);
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

ಸ್ಥಿರ ಚಿಹ್ನೆಯುಳ್ಳ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ base16_ಗಮ್ಯ_int[256] = {
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

ಸ್ಥಿರ bool
isubase16 (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ch)
{
  ಹಿಂತಿರುಗಿ ch < ಗಾತ್ರ base16_ಗಮ್ಯ_int && 0 <= base16_ಗಮ್ಯ_int[ch];
}

ಸ್ಥಿರ idx_t
base16_ಉದ್ದಳತೆ (idx_t len)
{
  ಹಿಂತಿರುಗಿ len * 2;
}


ಸ್ಥಿರ ಶೂನ್ಯ
base16_encode (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸೀಮಿತ ಒಳ, idx_t inlen,
               ಅಕ್ಷರ *ಸೀಮಿತ ಹೊರಹೋಗು, idx_t outlen)
{
  ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ base16[16] ATTRIBUTE_NONSTRING = "0123456789ABCDEF";

  ಆಗ (inlen && outlen)
    {
      ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ c = *ಒಳ;
      *ಹೊರಹೋಗು++ = base16[c >> 4];
      *ಹೊರಹೋಗು++ = base16[c & 0x0F];
      ++ಒಳ;
      inlen--;
      outlen -= 2;
    }
}


ಸ್ಥಿರ ಶೂನ್ಯ
base16_decode_ctx_init (ರಚನೆ ತಳ_decode_context *ctx)
{
  init_inbuf (ctx);
  ctx->ctx.base16.nibble = -1;
}


ಸ್ಥಿರ bool
base16_decode_ctx (ರಚನೆ ತಳ_decode_context *ctx,
                   ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸೀಮಿತ ಒಳ, idx_t inlen,
                   ಅಕ್ಷರ *ಸೀಮಿತ ಹೊರಹೋಗು, idx_t *outlen)
{
  bool ignore_ಸಾಲುಗಳು = true;  /* for now, always ignore them */
  ಅಕ್ಷರ *out0 = ಹೊರಹೋಗು;
  ಚಿಹ್ನೆಯುಳ್ಳ ಅಕ್ಷರ nibble = ctx->ctx.base16.nibble;

  /* inlen==0 is request to flush output.
     if there is a dangling high nibble - we are missing the low nibble,
     so return false - indicating an invalid input.  */
  ಆದರೆ (inlen == 0)
    {
      *outlen = 0;
      ಹಿಂತಿರುಗಿ nibble < 0;
    }

  ಆಗ (inlen--)
    {
      ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ c = *ಒಳ++;
      ಆದರೆ (ignore_ಸಾಲುಗಳು && c == '\n')
        ಮುಂದುವರಿ;

      ಆದರೆ (ಗಾತ್ರ base16_ಗಮ್ಯ_int <= c || base16_ಗಮ್ಯ_int[c] < 0)
        {
          *outlen = ಹೊರಹೋಗು - out0;
          ಹಿಂತಿರುಗಿ false; /* garbage - return false */
        }

      ಆದರೆ (nibble < 0)
        nibble = base16_ಗಮ್ಯ_int[c];
      ಇಲ್ಲದಿದ್ದರೆ
        {
          /* have both nibbles, write octet */
          *ಹೊರಹೋಗು++ = (nibble << 4) + base16_ಗಮ್ಯ_int[c];
          nibble = -1;
        }
    }

  ctx->ctx.base16.nibble = nibble;
  *outlen = ಹೊರಹೋಗು - out0;
  ಹಿಂತಿರುಗಿ true;
}



ATTRIBUTE_ಶುದ್ಧ
ಸ್ಥಿರ idx_t
z85_ಉದ್ದಳತೆ (idx_t len)
{
  /* Z85 does not allow padding, so no need to round to highest integer.  */
  idx_t z85_len = (len * 5) / 4;
  affirm (0 <= z85_len);
  ಹಿಂತಿರುಗಿ z85_len;
}

ಸ್ಥಿರ bool
isuz85 (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ch)
{
  ಹಿಂತಿರುಗಿ c_isalnum (ch) || strchr (".-:+=^!/*?&<>()[]{}@%$#", ch) != ಖಾಲಿ;
}

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ z85_encoding[85] ATTRIBUTE_NONSTRING =
  "0123456789"
  "abcdefghijklmnopqrstuvwxyz"
  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
  ".-:+=^!/*?&<>()[]{}@%$#";

ಸ್ಥಿರ ಶೂನ್ಯ
z85_encode (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸೀಮಿತ ಒಳ, idx_t inlen,
            ಅಕ್ಷರ *ಸೀಮಿತ ಹೊರಹೋಗು, idx_t outlen)
{
  ಪೂರ್ಣಾಂಕ i = 0;
  ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ quad[4];
  idx_t outidx = 0;

  ಆಗ (true)
    {
      ಆದರೆ (inlen == 0)
        {
          /* no more input, exactly on 4 octet boundary. */
          ಆದರೆ (i == 0)
            ಹಿಂತಿರುಗಿ;

          /* currently, there's no way to return an error in encoding.  */
          error (EXIT_FAILURE, 0,
                 _("invalid input (length must be multiple of 4 characters)"));
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          quad[i++] = *ಒಳ++;
          --inlen;
        }

      /* Got a quad, encode it */
      ಆದರೆ (i == 4)
        {
          int_fast64_t val = quad[0];
          val = (val << 24) + (quad[1] << 16) + (quad[2] << 8) + quad[3];

          ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 4; j >= 0; --j)
            {
              ಪೂರ್ಣಾಂಕ c = val % 85;
              val /= 85;

              /* NOTE: if there is padding (which is trimmed by z85
                 before outputting the result), the output buffer 'out'
                 might not include enough allocated bytes for the padding,
                 so don't store them. */
              ಆದರೆ (outidx + j < outlen)
                ಹೊರಹೋಗು[j] = z85_encoding[c];
            }
          ಹೊರಹೋಗು += 5;
          outidx += 5;
          i = 0;
        }
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
z85_decode_ctx_init (ರಚನೆ ತಳ_decode_context *ctx)
{
  init_inbuf (ctx);
  ctx->ctx.z85.i = 0;
}


# define Z85_LO_CTX_ಗಮ್ಯ_32_ಬಿಟ್_VAL(ctx) \
  (((ctx)->ctx.z85.octets[1] * 85 * 85 * 85) +      \
   ((ctx)->ctx.z85.octets[2] * 85 * 85) +	    \
   ((ctx)->ctx.z85.octets[3] * 85) +		    \
   ((ctx)->ctx.z85.octets[4]))


# define Z85_HI_CTX_ಗಮ್ಯ_32_ಬಿಟ್_VAL(ctx) \
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
ಸ್ಥಿರ ಚಿಹ್ನೆಯುಳ್ಳ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ z85_decoding[93] = {
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

ಸ್ಥಿರ bool
z85_decode_ctx (ರಚನೆ ತಳ_decode_context *ctx,
                ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸೀಮಿತ ಒಳ, idx_t inlen,
                ಅಕ್ಷರ *ಸೀಮಿತ ಹೊರಹೋಗು, idx_t *outlen)
{
  bool ignore_ಸಾಲುಗಳು = true;  /* for now, always ignore them */

  *outlen = 0;

  /* inlen==0 is request to flush output.
     if there are dangling values - we are missing entries,
     so return false - indicating an invalid input.  */
  ಆದರೆ (inlen == 0)
    {
      ಆದರೆ (ctx->ctx.z85.i > 0)
        {
          /* Z85 variant does not allow padding - input must
             be a multiple of 5 - so return error.  */
          ಹಿಂತಿರುಗಿ false;
        }
      ಹಿಂತಿರುಗಿ true;
    }

  ಆಗ (inlen--)
    {
      ಆದರೆ (ignore_ಸಾಲುಗಳು && *ಒಳ == '\n')
        {
          ++ಒಳ;
          ಮುಂದುವರಿ;
        }

      /* z85 decoding */
      ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ c = *ಒಳ;

      ಆದರೆ (c >= 33 && c <= 125)
        {
          ಚಿಹ್ನೆಯುಳ್ಳ ಅಕ್ಷರ ch = z85_decoding[c - 33];
          ಆದರೆ (ch < 0)
            ಹಿಂತಿರುಗಿ false; /* garbage - return false */
          c = ch;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        ಹಿಂತಿರುಗಿ false; /* garbage - return false */

      ++ಒಳ;

      ctx->ctx.z85.octets[ctx->ctx.z85.i++] = c;
      ಆದರೆ (ctx->ctx.z85.i == 5)
        {
          /* decode the lowest 4 octets, then check for overflows.  */
          int_fast64_t val = Z85_LO_CTX_ಗಮ್ಯ_32_ಬಿಟ್_VAL (ctx);

          /* The Z85 spec and the reference implementation say nothing
             about overflows. To be on the safe side, reject them.  */

          val += Z85_HI_CTX_ಗಮ್ಯ_32_ಬಿಟ್_VAL (ctx);
          ಆದರೆ ((val >> 24) & ~0xFF)
            ಹಿಂತಿರುಗಿ false;

          *ಹೊರಹೋಗು++ = val >> 24;
          *ಹೊರಹೋಗು++ = (val >> 16) & 0xFF;
          *ಹೊರಹೋಗು++ = (val >> 8) & 0xFF;
          *ಹೊರಹೋಗು++ = val & 0xFF;

          *outlen += 4;

          ctx->ctx.z85.i = 0;
        }
    }
  ಹಿಂತಿರುಗಿ true;
}


ಸಾಲೊಳಗೆ ಸ್ಥಿರ bool
isubase2 (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ch)
{
  ಹಿಂತಿರುಗಿ ch == '0' || ch == '1';
}

ಸ್ಥಿರ idx_t
base2_ಉದ್ದಳತೆ (idx_t len)
{
  ಹಿಂತಿರುಗಿ len * 8;
}


ಸಾಲೊಳಗೆ ಸ್ಥಿರ ಶೂನ್ಯ
base2msbf_encode (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸೀಮಿತ ಒಳ, idx_t inlen,
                  ಅಕ್ಷರ *ಸೀಮಿತ ಹೊರಹೋಗು, idx_t outlen)
{
  ಆಗ (inlen && outlen)
    {
      ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ c = *ಒಳ;
      ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < 8; i++)
        {
          *ಹೊರಹೋಗು++ = c & 0x80 ? '1' : '0';
          c <<= 1;
        }
      inlen--;
      outlen -= 8;
      ++ಒಳ;
    }
}

ಸಾಲೊಳಗೆ ಸ್ಥಿರ ಶೂನ್ಯ
base2lsbf_encode (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸೀಮಿತ ಒಳ, idx_t inlen,
                  ಅಕ್ಷರ *ಸೀಮಿತ ಹೊರಹೋಗು, idx_t outlen)
{
  ಆಗ (inlen && outlen)
    {
      ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ c = *ಒಳ;
      ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < 8; i++)
        {
          *ಹೊರಹೋಗು++ = c & 0x01 ? '1' : '0';
          c >>= 1;
        }
      inlen--;
      outlen -= 8;
      ++ಒಳ;
    }
}


ಸ್ಥಿರ ಶೂನ್ಯ
base2_decode_ctx_init (ರಚನೆ ತಳ_decode_context *ctx)
{
  init_inbuf (ctx);
  ctx->ctx.base2.octet = 0;
  ctx->ctx.base2.ಬಿಟ್_pos = 0;
}


ಸ್ಥಿರ bool
base2lsbf_decode_ctx (ರಚನೆ ತಳ_decode_context *ctx,
                      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸೀಮಿತ ಒಳ, idx_t inlen,
                      ಅಕ್ಷರ *ಸೀಮಿತ ಹೊರಹೋಗು, idx_t *outlen)
{
  bool ignore_ಸಾಲುಗಳು = true;  /* for now, always ignore them */

  *outlen = 0;

  /* inlen==0 is request to flush output.
     if there is a dangling bit - we are missing some bits,
     so return false - indicating an invalid input.  */
  ಆದರೆ (inlen == 0)
    ಹಿಂತಿರುಗಿ ctx->ctx.base2.ಬಿಟ್_pos == 0;

  ಆಗ (inlen--)
    {
      ಆದರೆ (ignore_ಸಾಲುಗಳು && *ಒಳ == '\n')
        {
          ++ಒಳ;
          ಮುಂದುವರಿ;
        }

      ಆದರೆ (!isubase2 (*ಒಳ))
        ಹಿಂತಿರುಗಿ false;

      bool ಬಿಟ್ = (*ಒಳ == '1');
      ctx->ctx.base2.octet |= ಬಿಟ್ << ctx->ctx.base2.ಬಿಟ್_pos;
      ++ctx->ctx.base2.ಬಿಟ್_pos;

      ಆದರೆ (ctx->ctx.base2.ಬಿಟ್_pos == 8)
        {
          *ಹೊರಹೋಗು++ = ctx->ctx.base2.octet;
          ctx->ctx.base2.octet = 0;
          ++*outlen;
          ctx->ctx.base2.ಬಿಟ್_pos = 0;
        }

      ++ಒಳ;
    }

  ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ bool
base2msbf_decode_ctx (ರಚನೆ ತಳ_decode_context *ctx,
                      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸೀಮಿತ ಒಳ, idx_t inlen,
                      ಅಕ್ಷರ *ಸೀಮಿತ ಹೊರಹೋಗು, idx_t *outlen)
{
  bool ignore_ಸಾಲುಗಳು = true;  /* for now, always ignore them */

  *outlen = 0;

  /* inlen==0 is request to flush output.
     if there is a dangling bit - we are missing some bits,
     so return false - indicating an invalid input.  */
  ಆದರೆ (inlen == 0)
    ಹಿಂತಿರುಗಿ ctx->ctx.base2.ಬಿಟ್_pos == 0;

  ಆಗ (inlen--)
    {
      ಆದರೆ (ignore_ಸಾಲುಗಳು && *ಒಳ == '\n')
        {
          ++ಒಳ;
          ಮುಂದುವರಿ;
        }

      ಆದರೆ (!isubase2 (*ಒಳ))
        ಹಿಂತಿರುಗಿ false;

      bool ಬಿಟ್ = (*ಒಳ == '1');
      ಆದರೆ (ctx->ctx.base2.ಬಿಟ್_pos == 0)
        ctx->ctx.base2.ಬಿಟ್_pos = 8;
      --ctx->ctx.base2.ಬಿಟ್_pos;
      ctx->ctx.base2.octet |= ಬಿಟ್ << ctx->ctx.base2.ಬಿಟ್_pos;

      ಆದರೆ (ctx->ctx.base2.ಬಿಟ್_pos == 0)
        {
          *ಹೊರಹೋಗು++ = ctx->ctx.base2.octet;
          ctx->ctx.base2.octet = 0;
          ++*outlen;
        }

      ++ಒಳ;
    }

  ಹಿಂತಿರುಗಿ true;
}

/* Map from GMP (up to base 62):
   "0123456789 ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuv";
   to base 58:
   "123456789A BCDEFGHJKLMNPQRSTUVWXYZabc defghijkmnopqrstuvwxyz";  */
ಸ್ಥಿರ ಚಿಹ್ನೆಯುಳ್ಳ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ gmp_ಗಮ್ಯ_base58[256] = {
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

ಸ್ಥಿರ ಚಿಹ್ನೆಯುಳ್ಳ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ base58_ಗಮ್ಯ_gmp[256] = {
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

ಸ್ಥಿರ bool
isubase58 (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ch)
{
  ಹಿಂತಿರುಗಿ ch < ಗಾತ್ರ base58_ಗಮ್ಯ_gmp && 0 <= base58_ಗಮ್ಯ_gmp[ch];
}


ATTRIBUTE_ಶುದ್ಧ
ಸ್ಥಿರ idx_t
base58_ಉದ್ದಳತೆ (idx_t len)
{
  /* Base58 output length is approximately log(256)/log(58),
     which is approximately len * 138 / 100,
     which is at most ((len + 100 - 1) / 100) * 138
     +1 to ensure we've enough place for NUL  */
  idx_t base58_len = ((len + 99) / 100) * 138 + 1;
  affirm (0 < base58_len);
  ಹಿಂತಿರುಗಿ base58_len;
}


ಸ್ಥಿರ ಶೂನ್ಯ
base58_encode_ctx_init (ರಚನೆ ತಳ_encode_context *ctx)
{
  ctx->ctx.base58.buf = ಖಾಲಿ;
  ctx->ctx.base58.ಗಾತ್ರ_2 = 0;
  ctx->ctx.base58.ಸಾಮರ್ಥ್ಯ = 0;
}


ಸ್ಥಿರ bool
base58_encode_ctx (ರಚನೆ ತಳ_encode_context *ctx,
                   ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸೀಮಿತ ಒಳ, idx_t inlen,
                   MAYBE_ಬಳಸದ ಅಕ್ಷರ *ಸೀಮಿತ ಹೊರಹೋಗು, idx_t *outlen)
{
  *outlen = 0;   /* Only accumulate input in this function.  */

  ಆದರೆ (inlen == 0)
    ಹಿಂತಿರುಗಿ true;

  idx_t free_space = ctx->ctx.base58.ಸಾಮರ್ಥ್ಯ - ctx->ctx.base58.ಗಾತ್ರ_2;
  ಆದರೆ (free_space < inlen)
    {
      ctx->ctx.base58.buf = xpalloc (ctx->ctx.base58.buf,
                                     &ctx->ctx.base58.ಸಾಮರ್ಥ್ಯ,
                                     inlen - free_space,
                                     -1, ಗಾತ್ರ *ctx->ctx.base58.buf);
    }

  ಸ್ಮರಣೆ_ನಕಲು (ctx->ctx.base58.buf + ctx->ctx.base58.ಗಾತ್ರ_2, ಒಳ, inlen);
  ctx->ctx.base58.ಗಾತ್ರ_2 += inlen;

  ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ ಶೂನ್ಯ
base58_encode (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ದತ್ತಾಂಶ, size_t ದತ್ತಾಂಶ_len,
               ಅಕ್ಷರ *ಹೊರಹೋಗು, idx_t *outlen)
{
  affirm (ತಳ_ಉದ್ದಳತೆ_2 (ದತ್ತಾಂಶ_len) <= *outlen);

  size_t zeros = 0;
  ಆಗ (zeros < ದತ್ತಾಂಶ_len && ದತ್ತಾಂಶ[zeros] == 0)
    zeros++;

  memset (ಹೊರಹೋಗು, '1', zeros);
  ಅಕ್ಷರ *p = ಹೊರಹೋಗು + zeros;

  /* Use GMP to convert from base 256 to base 58.  */
  mpz_t ಅಂಕೆ;
  mpz_init (ಅಂಕೆ);
  ಆದರೆ (ದತ್ತಾಂಶ_len - zeros)
    {
      mpz_import (ಅಂಕೆ, ದತ್ತಾಂಶ_len - zeros, 1, 1, 0, 0, ದತ್ತಾಂಶ + zeros);
      ಗಾಗಿ (p = mpz_get_str (p, 58, ಅಂಕೆ); *p; p++)
        *p = gmp_ಗಮ್ಯ_base58[ಗಮ್ಯ_uchar (*p)];
    }
  mpz_clear (ಅಂಕೆ);

  *outlen = p - ಹೊರಹೋಗು;
}


ಸ್ಥಿರ bool
base58_encode_ctx_finalize (ರಚನೆ ತಳ_encode_context *ctx,
                            ಅಕ್ಷರ *ಸೀಮಿತ *ಹೊರಹೋಗು, idx_t *outlen)
{
  /* Ensure output buffer is large enough.  */
  idx_t max_outlen = ತಳ_ಉದ್ದಳತೆ_2 (ctx->ctx.base58.ಗಾತ್ರ_2);
  ಆದರೆ (max_outlen > *outlen)
    {
      *ಹೊರಹೋಗು = xrealloc (*ಹೊರಹೋಗು, max_outlen);
      *outlen = max_outlen;
    }

  base58_encode ((ಅಕ್ಷರ *)ctx->ctx.base58.buf, ctx->ctx.base58.ಗಾತ್ರ_2,
                 *ಹೊರಹೋಗು, outlen);

  ಬಿಡುಗಡೆ (ctx->ctx.base58.buf);
  ctx->ctx.base58.buf = ಖಾಲಿ;
  ctx->ctx.base58.ಗಾತ್ರ_2 = 0;
  ctx->ctx.base58.ಸಾಮರ್ಥ್ಯ = 0;

  ಹಿಂತಿರುಗಿ true;
}


ಸ್ಥಿರ ಶೂನ್ಯ
base58_decode_ctx_init (ರಚನೆ ತಳ_decode_context *ctx)
{
  ctx->ctx.base58.ಗಾತ್ರ_2 = 0;
  ctx->ctx.base58.ಸಾಮರ್ಥ್ಯ = 0;
  ctx->ctx.base58.buf = ಖಾಲಿ;
}

ಸ್ಥಿರ bool
base58_decode_ctx (ರಚನೆ ತಳ_decode_context *ctx,
                   ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸೀಮಿತ ಒಳ, idx_t inlen,
                   MAYBE_ಬಳಸದ ಅಕ್ಷರ *ಸೀಮಿತ ಹೊರಹೋಗು, idx_t *outlen)
{
  bool ignore_ಸಾಲುಗಳು = true;  /* for now, always ignore them */

  *outlen = 0;   /* Only accumulate input in this function.  */

  ಆದರೆ (inlen == 0)
    ಹಿಂತಿರುಗಿ true;

  idx_t free_space = ctx->ctx.base58.ಸಾಮರ್ಥ್ಯ - ctx->ctx.base58.ಗಾತ್ರ_2;
  free_space -= 1;  /* Ensure we leave space for NUL (for mpz_set_str).  */
  ಆದರೆ (free_space < inlen)
    {
      ctx->ctx.base58.buf = xpalloc (ctx->ctx.base58.buf,
                                     &ctx->ctx.base58.ಸಾಮರ್ಥ್ಯ,
                                     inlen - free_space,
                                     -1, ಗಾತ್ರ *ctx->ctx.base58.buf);
    }

  /* Accumulate all valid input characters in our buffer.
     Note we don't rely on mpz_set_str() for validation
     as that allows (skips) all whitespace.  */
  ಗಾಗಿ (idx_t i = 0; i < inlen; i++)
    {
      ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ c = ಒಳ[i];

      ಆದರೆ (ignore_ಸಾಲುಗಳು && c == '\n')
        ಮುಂದುವರಿ;

      ಆದರೆ (!isubase58 (c))
        ಹಿಂತಿರುಗಿ false;

      ctx->ctx.base58.buf[ctx->ctx.base58.ಗಾತ್ರ_2++] = base58_ಗಮ್ಯ_gmp[ಗಮ್ಯ_uchar (c)];
    }

  ಹಿಂತಿರುಗಿ true;
}


ಸ್ಥಿರ bool
base58_decode (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ದತ್ತಾಂಶ, size_t ದತ್ತಾಂಶ_len,
               ಅಕ್ಷರ *ಸೀಮಿತ ಹೊರಹೋಗು, idx_t *outlen)
{
  affirm (ದತ್ತಾಂಶ_len <= *outlen);

  size_t ones = 0;
  ಆಗ (ones < ದತ್ತಾಂಶ_len && ದತ್ತಾಂಶ[ones] == base58_ಗಮ್ಯ_gmp['1'])
    ones++;

  memset (ಹೊರಹೋಗು, 0, ones);

  /* Use GMP to convert from base 58 to base 256.  */
  mpz_t ಅಂಕೆ;
  mpz_init (ಅಂಕೆ);

  ಆದರೆ ((ದತ್ತಾಂಶ_len - ones) && mpz_ಗಣ_str (ಅಂಕೆ, ದತ್ತಾಂಶ + ones, 58) != 0)
    {
      mpz_clear (ಅಂಕೆ);
      *outlen = 0;
      ಹಿಂತಿರುಗಿ false;
    }

  size_t exported_ಗಾತ್ರ = 0;
  ಆದರೆ (ದತ್ತಾಂಶ_len - ones)
    {
      size_t ದ್ವಿಮಾನ_ಗಾತ್ರ = (mpz_sizeinbase (ಅಂಕೆ, 2) + 7) / 8;
      affirm (*outlen - ones >= ದ್ವಿಮಾನ_ಗಾತ್ರ);
      mpz_export (ಹೊರಹೋಗು + ones, &exported_ಗಾತ್ರ, 1, 1, 0, 0, ಅಂಕೆ);
    }

  mpz_clear (ಅಂಕೆ);
  *outlen = ones + exported_ಗಾತ್ರ;
  ಹಿಂತಿರುಗಿ true;
}


ಸ್ಥಿರ bool
base58_decode_ctx_finalize (ರಚನೆ ತಳ_decode_context *ctx,
                            ಅಕ್ಷರ *ಸೀಮಿತ *ಹೊರಹೋಗು, idx_t *outlen)
{
  /* Ensure output buffer is large enough.
     Worst case is input is all '1's.  */
  idx_t max_outlen = ctx->ctx.base58.ಗಾತ್ರ_2;
  ಆದರೆ (max_outlen > *outlen)
    {
      *ಹೊರಹೋಗು = xrealloc (*ಹೊರಹೋಗು, max_outlen);
      *outlen = max_outlen;
    }

  /* Ensure input buffer is NUL terminated (for mpz_get_str).  */
  ಆದರೆ (ctx->ctx.base58.ಗಾತ್ರ_2)
    ctx->ctx.base58.buf[ctx->ctx.base58.ಗಾತ್ರ_2] = '\0';

  bool ret = base58_decode ((ಅಕ್ಷರ *)ctx->ctx.base58.buf, ctx->ctx.base58.ಗಾತ್ರ_2,
                            *ಹೊರಹೋಗು, outlen);

  ಬಿಡುಗಡೆ (ctx->ctx.base58.buf);
  ctx->ctx.base58.buf = ಖಾಲಿ;
  ctx->ctx.base58.ಗಾತ್ರ_2 = 0;
  ctx->ctx.base58.ಸಾಮರ್ಥ್ಯ = 0;

  ಹಿಂತಿರುಗಿ ret;
}

#ಆದರೆ_ಅಂತ್ಯ /* BASE_TYPE == 42, i.e., "basenc"*/



ಸ್ಥಿರ ಶೂನ್ಯ
wrap_write (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಬಫರ್, idx_t len,
            idx_t wrap_ಸ್ತಂಭ, idx_t *current_ಸ್ತಂಭ, FILE *ಹೊರಹೋಗು)
{
  ಆದರೆ (wrap_ಸ್ತಂಭ == 0)
    {
      /* Simple write. */
      ಆದರೆ (fwrite (ಬಫರ್, 1, len, stdout) < len)
        write_error ();
    }
  ಇಲ್ಲದಿದ್ದರೆ
    ಗಾಗಿ (idx_t written = 0; written < len; )
      {
        idx_t ಗಮ್ಯ_write = MIN (wrap_ಸ್ತಂಭ - *current_ಸ್ತಂಭ, len - written);

        ಆದರೆ (ಗಮ್ಯ_write == 0)
          {
            ಆದರೆ (fputc ('\n', ಹೊರಹೋಗು) == EOF)
              write_error ();
            *current_ಸ್ತಂಭ = 0;
          }
        ಇಲ್ಲದಿದ್ದರೆ
          {
            ಆದರೆ (fwrite (ಬಫರ್ + written, 1, ಗಮ್ಯ_write, stdout) < ಗಮ್ಯ_write)
              write_error ();
            *current_ಸ್ತಂಭ += ಗಮ್ಯ_write;
            written += ಗಮ್ಯ_write;
          }
      }
}

ಸ್ಥಿರ _Noreturn ಶೂನ್ಯ
ಮುಕ್ತಾಯ_and_exit (FILE *ಒಳ, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *infile)
{
  ಆದರೆ (fclose (ಒಳ) != 0)
    {
      ಆದರೆ (streq (infile, "-"))
        error (EXIT_FAILURE, errno, _("closing standard input"));
      ಇಲ್ಲದಿದ್ದರೆ
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
    }

  ನಿರ್ಗಮಿಸು (EXIT_SUCCESS);
}

ಸ್ಥಿರ _Noreturn ಶೂನ್ಯ
do_encode (FILE *ಒಳ, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *infile, FILE *ಹೊರಹೋಗು, idx_t wrap_ಸ್ತಂಭ)
{
  idx_t current_ಸ್ತಂಭ = 0;
  ಅಕ್ಷರ *inbuf, *outbuf;
  idx_t ಮೊತ್ತ;

  inbuf = xmalloc (ENC_BLOCKSIZE);
  outbuf = xmalloc (ತಳ_ಉದ್ದಳತೆ (ENC_BLOCKSIZE));

#ಆದರೆ ತಳ_TYPE == 42
  /* Initialize encoding context if needed (for base58) */
  ರಚನೆ ತಳ_encode_context encode_ctx;
  bool use_ctx = (ತಳ_encode_ctx_init != ಖಾಲಿ);
  ಆದರೆ (use_ctx)
    ತಳ_encode_ctx_init (&encode_ctx);
#ಆದರೆ_ಅಂತ್ಯ

  ಮಾಡು
    {
      idx_t n;

      ಮೊತ್ತ = 0;
      ಮಾಡು
        {
          n = fread (inbuf + ಮೊತ್ತ, 1, ENC_BLOCKSIZE - ಮೊತ್ತ, ಒಳ);
          ಮೊತ್ತ += n;
        }
      ಆಗ (!feof (ಒಳ) && !ferror (ಒಳ) && ಮೊತ್ತ < ENC_BLOCKSIZE);

      ಆದರೆ (ಮೊತ್ತ > 0)
        {
#ಆದರೆ ತಳ_TYPE == 42
          ಆದರೆ (use_ctx)
            {
              idx_t outlen = 0;
              ತಳ_encode_ctx (&encode_ctx, inbuf, ಮೊತ್ತ, outbuf, &outlen);

              wrap_write (outbuf, outlen, wrap_ಸ್ತಂಭ, &current_ಸ್ತಂಭ, ಹೊರಹೋಗು);
            }
          ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ_ಅಂತ್ಯ
            {
              /* Process input one block at a time.  Note that ENC_BLOCKSIZE
                 is sized so that no pad chars will appear in output. */
              ತಳ_encode (inbuf, ಮೊತ್ತ, outbuf, ತಳ_ಉದ್ದಳತೆ (ಮೊತ್ತ));

              wrap_write (outbuf, ತಳ_ಉದ್ದಳತೆ (ಮೊತ್ತ), wrap_ಸ್ತಂಭ,
                          &current_ಸ್ತಂಭ, ಹೊರಹೋಗು);
            }
        }
    }
  ಆಗ (!feof (ಒಳ) && !ferror (ಒಳ) && ಮೊತ್ತ == ENC_BLOCKSIZE);

#ಆದರೆ ತಳ_TYPE == 42
  ಆದರೆ (use_ctx && ತಳ_encode_ctx_finalize)
    {
      idx_t outlen = ತಳ_ಉದ್ದಳತೆ (ENC_BLOCKSIZE);
      ತಳ_encode_ctx_finalize (&encode_ctx, &outbuf, &outlen);

      wrap_write (outbuf, outlen, wrap_ಸ್ತಂಭ, &current_ಸ್ತಂಭ, ಹೊರಹೋಗು);
    }
#ಆದರೆ_ಅಂತ್ಯ

  /* When wrapping, terminate last line. */
  ಆದರೆ (wrap_ಸ್ತಂಭ && current_ಸ್ತಂಭ > 0 && fputc ('\n', ಹೊರಹೋಗು) == EOF)
    write_error ();

  ಆದರೆ (ferror (ಒಳ))
    error (EXIT_FAILURE, errno, _("read error"));

  ಮುಕ್ತಾಯ_and_exit (ಒಳ, infile);
}

ಸ್ಥಿರ _Noreturn ಶೂನ್ಯ
do_decode (FILE *ಒಳ, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *infile, FILE *ಹೊರಹೋಗು, bool ignore_garbage)
{
  ಅಕ್ಷರ *inbuf, *outbuf;
  idx_t ಮೊತ್ತ;
  ರಚನೆ ತಳ_decode_context ctx;

  inbuf = xmalloc (ತಳ_ಉದ್ದಳತೆ (DEC_BLOCKSIZE));
  outbuf = xmalloc (DEC_BLOCKSIZE);

#ಆದರೆ ತಳ_TYPE == 42
  ctx.inbuf = ಖಾಲಿ;
#ಆದರೆ_ಅಂತ್ಯ
  ತಳ_decode_ctx_init (&ctx);

  ಮಾಡು
    {
      bool ok;

      ಮೊತ್ತ = 0;
      ಮಾಡು
        {
          idx_t n = fread (inbuf + ಮೊತ್ತ,
                           1, ತಳ_ಉದ್ದಳತೆ (DEC_BLOCKSIZE) - ಮೊತ್ತ, ಒಳ);

          ಆದರೆ (ignore_garbage)
            {
              ಗಾಗಿ (idx_t i = 0; n > 0 && i < n;)
                {
                  ಆದರೆ (isubase (inbuf[ಮೊತ್ತ + i])
                      || (REQUIRED_PADDING (1) && inbuf[ಮೊತ್ತ + i] == '='))
                    i++;
                  ಇಲ್ಲದಿದ್ದರೆ
                    memmove (inbuf + ಮೊತ್ತ + i, inbuf + ಮೊತ್ತ + i + 1, --n - i);
                }
            }

          ಮೊತ್ತ += n;

          ಆದರೆ (ferror (ಒಳ))
            error (EXIT_FAILURE, errno, _("read error"));
        }
      ಆಗ (ಮೊತ್ತ < ತಳ_ಉದ್ದಳತೆ (DEC_BLOCKSIZE) && !feof (ಒಳ));

      ಆಗ (ಮೊತ್ತ || feof (ಒಳ))
        {
          idx_t n = DEC_BLOCKSIZE;
          ಆದರೆ (ಮೊತ್ತ)
            ok = ತಳ_decode_ctx (&ctx, inbuf, ಮೊತ್ತ, outbuf, &n);
          ಇಲ್ಲದಿದ್ದರೆ
            ok = ತಳ_decode_ctx_finalize (&ctx, &outbuf, &n);

          ಆದರೆ (fwrite (outbuf, 1, n, ಹೊರಹೋಗು) < n)
            write_error ();

          ಆದರೆ (! ok)
            error (EXIT_FAILURE, 0, _("invalid input"));

          ಆದರೆ (ಮೊತ್ತ == 0)
            ನಿಲ್ಲಿಸು;
          ಮೊತ್ತ = 0;
        }
    }
  ಆಗ (!feof (ಒಳ));

  ಮುಕ್ತಾಯ_and_exit (ಒಳ, infile);
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  ಪೂರ್ಣಾಂಕ opt;
  FILE *ಒಳಹರಿವು_fh;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *infile;

  /* True if --decode has been given and we should decode data. */
  bool decode = false;
  /* True if we should ignore non-base-alphabetic characters. */
  bool ignore_garbage = false;
  /* Wrap encoded data around the 76th column, by default. */
  idx_t wrap_ಸ್ತಂಭ = 76;

#ಆದರೆ ತಳ_TYPE == 42
  ಪೂರ್ಣಾಂಕ ತಳ_type = 0;
#ಆದರೆ_ಅಂತ್ಯ

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಆಗ ((opt = getopt_long (argc, argv, "diw:", long_options, ಖಾಲಿ)) != -1)
    ಆಯ್ಕೆ (opt)
      {
      ಸ್ಥಿತಿ 'd':
        decode = true;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ 'w':
        {
          intmax_t w;
          strtol_error s_err = xstrtoimax (optarg, ಖಾಲಿ, 10, &w, "");
          ಆದರೆ (LONGINT_ಅತಿಪ್ರವಾಹ < s_err || w < 0)
            error (EXIT_FAILURE, 0, "%s: %s",
                   _("invalid wrap size"), quote (optarg));
          wrap_ಸ್ತಂಭ = s_err == LONGINT_ಅತಿಪ್ರವಾಹ || IDX_MAX < w ? 0 : w;
        }
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ 'i':
        ignore_garbage = true;
        ನಿಲ್ಲಿಸು;

#ಆದರೆ ತಳ_TYPE == 42
      ಸ್ಥಿತಿ BASE64_OPTION:
      ಸ್ಥಿತಿ BASE64URL_OPTION:
      ಸ್ಥಿತಿ BASE32_OPTION:
      ಸ್ಥಿತಿ BASE32HEX_OPTION:
      ಸ್ಥಿತಿ BASE16_OPTION:
      ಸ್ಥಿತಿ BASE2MSBF_OPTION:
      ಸ್ಥಿತಿ BASE2LSBF_OPTION:
      ಸ್ಥಿತಿ Z85_OPTION:
      ಸ್ಥಿತಿ BASE58_OPTION:
        ತಳ_type = opt;
        ನಿಲ್ಲಿಸು;
#ಆದರೆ_ಅಂತ್ಯ

      case_GETOPT_HELP_CHAR;

      case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);

      ಪೂರ್ವನಿಯೋಜಿತ:
        usage (EXIT_FAILURE);
        ನಿಲ್ಲಿಸು;
      }

#ಆದರೆ ತಳ_TYPE == 42
  required_padding = no_required_padding;
  has_padding = no_padding;
  get_pending_ಉದ್ದಳತೆ = no_pending_ಉದ್ದಳತೆ;
  ತಳ_decode_ctx_finalize = decode_ctx_finalize;

  ಆಯ್ಕೆ (ತಳ_type)
    {
    ಸ್ಥಿತಿ BASE64_OPTION:
      ತಳ_ಉದ್ದಳತೆ_2 = base64_ಉದ್ದಳತೆ_wrapper;
      required_padding = base64_required_padding;
      has_padding = base64_ctx_has_padding;
      get_pending_ಉದ್ದಳತೆ = base64_ctx_get_pending_ಉದ್ದಳತೆ;
      isubase = isubase64;
      ತಳ_encode = base64_encode;
      ತಳ_decode_ctx_init = base64_decode_ctx_init_wrapper;
      ತಳ_decode_ctx = base64_decode_ctx_wrapper;
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ BASE64URL_OPTION:
      ತಳ_ಉದ್ದಳತೆ_2 = base64_ಉದ್ದಳತೆ_wrapper;
      required_padding = base64_required_padding;
      has_padding = base64_ctx_has_padding;
      get_pending_ಉದ್ದಳತೆ = base64_ctx_get_pending_ಉದ್ದಳತೆ;
      isubase = isubase64url;
      ತಳ_encode = base64url_encode;
      ತಳ_decode_ctx_init = base64url_decode_ctx_init_wrapper;
      ತಳ_decode_ctx = base64url_decode_ctx_wrapper;
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ BASE32_OPTION:
      ತಳ_ಉದ್ದಳತೆ_2 = base32_ಉದ್ದಳತೆ_wrapper;
      required_padding = base32_required_padding;
      has_padding = base32_ctx_has_padding;
      get_pending_ಉದ್ದಳತೆ = base32_ctx_get_pending_ಉದ್ದಳತೆ;
      isubase = isubase32;
      ತಳ_encode = base32_encode;
      ತಳ_decode_ctx_init = base32_decode_ctx_init_wrapper;
      ತಳ_decode_ctx = base32_decode_ctx_wrapper;
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ BASE32HEX_OPTION:
      ತಳ_ಉದ್ದಳತೆ_2 = base32_ಉದ್ದಳತೆ_wrapper;
      required_padding = base32_required_padding;
      has_padding = base32_ctx_has_padding;
      get_pending_ಉದ್ದಳತೆ = base32_ctx_get_pending_ಉದ್ದಳತೆ;
      isubase = isubase32hex;
      ತಳ_encode = base32hex_encode;
      ತಳ_decode_ctx_init = base32hex_decode_ctx_init_wrapper;
      ತಳ_decode_ctx = base32hex_decode_ctx_wrapper;
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ BASE16_OPTION:
      ತಳ_ಉದ್ದಳತೆ_2 = base16_ಉದ್ದಳತೆ;
      get_pending_ಉದ್ದಳತೆ = base16_ctx_get_pending_ಉದ್ದಳತೆ;
      isubase = isubase16;
      ತಳ_encode = base16_encode;
      ತಳ_decode_ctx_init = base16_decode_ctx_init;
      ತಳ_decode_ctx = base16_decode_ctx;
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ BASE2MSBF_OPTION:
      ತಳ_ಉದ್ದಳತೆ_2 = base2_ಉದ್ದಳತೆ;
      get_pending_ಉದ್ದಳತೆ = base2_ctx_get_pending_ಉದ್ದಳತೆ;
      isubase = isubase2;
      ತಳ_encode = base2msbf_encode;
      ತಳ_decode_ctx_init = base2_decode_ctx_init;
      ತಳ_decode_ctx = base2msbf_decode_ctx;
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ BASE2LSBF_OPTION:
      ತಳ_ಉದ್ದಳತೆ_2 = base2_ಉದ್ದಳತೆ;
      get_pending_ಉದ್ದಳತೆ = base2_ctx_get_pending_ಉದ್ದಳತೆ;
      isubase = isubase2;
      ತಳ_encode = base2lsbf_encode;
      ತಳ_decode_ctx_init = base2_decode_ctx_init;
      ತಳ_decode_ctx = base2lsbf_decode_ctx;
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ Z85_OPTION:
      ತಳ_ಉದ್ದಳತೆ_2 = z85_ಉದ್ದಳತೆ;
      get_pending_ಉದ್ದಳತೆ = z85_ctx_get_pending_ಉದ್ದಳತೆ;
      isubase = isuz85;
      ತಳ_encode = z85_encode;
      ತಳ_decode_ctx_init = z85_decode_ctx_init;
      ತಳ_decode_ctx = z85_decode_ctx;
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ BASE58_OPTION:
      ತಳ_ಉದ್ದಳತೆ_2 = base58_ಉದ್ದಳತೆ;
      isubase = isubase58;
      ತಳ_encode_ctx_init = base58_encode_ctx_init;
      ತಳ_encode_ctx = base58_encode_ctx;
      ತಳ_encode_ctx_finalize = base58_encode_ctx_finalize;
      ತಳ_decode_ctx_init = base58_decode_ctx_init;
      ತಳ_decode_ctx = base58_decode_ctx;
      ತಳ_decode_ctx_finalize = base58_decode_ctx_finalize;
      ನಿಲ್ಲಿಸು;

    ಪೂರ್ವನಿಯೋಜಿತ:
      error (0, 0, _("missing encoding type"));
      usage (EXIT_FAILURE);
    }
#ಆದರೆ_ಅಂತ್ಯ

  ಆದರೆ (argc - optind > 1)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }

  ಆದರೆ (optind < argc)
    infile = argv[optind];
  ಇಲ್ಲದಿದ್ದರೆ
    infile = "-";

  ಆದರೆ (streq (infile, "-"))
    {
      xset_ದ್ವಿಮಾನ_ಬಹುಲಕ (STDIN_FILENO, O_ದ್ವಿಮಾನ);
      ಒಳಹರಿವು_fh = stdin;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಒಳಹರಿವು_fh = fopen (infile, "rb");
      ಆದರೆ (ಒಳಹರಿವು_fh == ಖಾಲಿ)
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
    }

  fadvise (ಒಳಹರಿವು_fh, FADVISE_SEQUENTIAL);

  ಆದರೆ (decode)
    do_decode (ಒಳಹರಿವು_fh, infile, stdout, ignore_garbage);
  ಇಲ್ಲದಿದ್ದರೆ
    do_encode (ಒಳಹರಿವು_fh, infile, stdout, wrap_ಸ್ತಂಭ);
}
