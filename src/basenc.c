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

#ಯದಿ BASE_TYPE == 42
# define AUTHORS \
  proper_name ("Simon Josefsson"), \
  proper_name ("Assaf Gordon")
#ಅನ್ಯಥಾ
# define AUTHORS proper_name ("Simon Josefsson")
#endif

#ಯದಿ BASE_TYPE == 32
# include "base32.h"
# define PROGRAM_NAME "base32"
#ನೋಚೇದ್ಯದಿ BASE_TYPE == 64
# include "base64.h"
# define PROGRAM_NAME "base64"
#ನೋಚೇದ್ಯದಿ BASE_TYPE == 42
# include "base32.h"
# include "base64.h"
# include "assure.h"
# define PROGRAM_NAME "basenc"
#ಅನ್ಯಥಾ
# error missing/invalid BASE_TYPE definition
#endif



#ಯದಿ BASE_TYPE == 42
ಗಣನಾ
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
#endif

ಸ್ಥಿರ ಸಂರಚನಾ option ಧ್ರುವ long_options[] =
{
  {"decode", no_argument, ರಿಕ್ತ, 'd'},
  {"wrap", required_argument, ರಿಕ್ತ, 'w'},
  {"ignore-garbage", no_argument, ರಿಕ್ತ, 'i'},
#ಯದಿ BASE_TYPE == 42
  {"base64",    no_argument, ರಿಕ್ತ, BASE64_OPTION},
  {"base64url", no_argument, ರಿಕ್ತ, BASE64URL_OPTION},
  {"base58",    no_argument, ರಿಕ್ತ, BASE58_OPTION},
  {"base32",    no_argument, ರಿಕ್ತ, BASE32_OPTION},
  {"base32hex", no_argument, ರಿಕ್ತ, BASE32HEX_OPTION},
  {"base16",    no_argument, ರಿಕ್ತ, BASE16_OPTION},
  {"base2msbf", no_argument, ರಿಕ್ತ, BASE2MSBF_OPTION},
  {"base2lsbf", no_argument, ರಿಕ್ತ, BASE2LSBF_OPTION},
  {"z85",       no_argument, ರಿಕ್ತ, Z85_OPTION},
#endif
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ರಿಕ್ತ, 0, ರಿಕ್ತ, 0}
};

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]\n\
"), program_name);

#ಯದಿ BASE_TYPE == 42
      fputs (_("\
basenc encode or decode FILE, or standard ಒಳಹರಿವು, to standard output.\n\
"), stdout);
#ಅನ್ಯಥಾ
      printf (_("\
Base%d encode or decode FILE, or standard ಒಳಹರಿವು, to standard output.\n\
"), BASE_TYPE);
#endif

      emit_stdin_note ();
      emit_mandatory_arg_note ();
#ಯದಿ BASE_TYPE == 42
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
#endif
      oputs (_("\
  -d, --decode\n\
         decode data\n\
"));
      oputs (_("\
  -i, --ignore-garbage\n\
         when decoding, ignore non-alphabet characters\n\
"));
      oputs (_("\
  -w, --wrap=COLS\n\
         wrap encoded ಸಾಲುಗಳು after COLS character (ಸ್ವಭಾವ 76).\n\
         Use 0 to disable ಸಾಲು wrapping\n\
"));
#ಯದಿ BASE_TYPE == 42
      oputs (_("\
      --z85\n\
         ascii85-like encoding (ZeroMQ spec:32/Z85);\n\
         when encoding, ಒಳಹರಿವು length must be a multiple of 4;\n\
         when decoding, ಒಳಹರಿವು length must be a multiple of 5\n\
"));
#endif
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
#ಯದಿ BASE_TYPE == 42
      fputs (_("\
\n\
When decoding, the ಒಳಹರಿವು may contain newlines in addition to the bytes of\n\
the formal alphabet.  Use --ignore-garbage to attempt to recover\n\
from any other non-alphabet bytes in the encoded stream.\n\
"), stdout);
#ಅನ್ಯಥಾ
      printf (_("\
\n\
The data are encoded as described ಪ್ರತಿ the %s alphabet in RFC 4648.\n\
When decoding, the ಒಳಹರಿವು may contain newlines in addition to the bytes of\n\
the formal %s alphabet.  Use --ignore-garbage to attempt to recover\n\
from any other non-alphabet bytes in the encoded stream.\n"),
              PROGRAM_NAME, PROGRAM_NAME);
#endif
      emit_ancillary_info (PROGRAM_NAME);
    }

  exit (status);
}

#ಯದಿ BASE_TYPE != 64
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
base32_required_padding (ಪೂರ್ಣಾಙ್ಕ len)
{
  ಪೂರ್ಣಾಙ್ಕ partial = len % 8;
  ಪ್ರತ್ಯಾಗಮ partial ? 8 - partial : 0;
}
#endif

#ಯದಿ BASE_TYPE != 32
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
base64_required_padding (ಪೂರ್ಣಾಙ್ಕ len)
{
  ಪೂರ್ಣಾಙ್ಕ partial = len % 4;
  ಪ್ರತ್ಯಾಗಮ partial ? 4 - partial : 0;
}
#endif

#ಯದಿ BASE_TYPE == 42
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
no_required_padding (MAYBE_UNUSED ಪೂರ್ಣಾಙ್ಕ len)
{
  ಪ್ರತ್ಯಾಗಮ 0;
}
#endif

#define ENC_BLOCKSIZE (1024 * 3 * 10)

#ಯದಿ BASE_TYPE == 32
# define BASE_LENGTH BASE32_LENGTH
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
#ನೋಚೇದ್ಯದಿ BASE_TYPE == 64
# define BASE_LENGTH BASE64_LENGTH
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
#ನೋಚೇದ್ಯದಿ BASE_TYPE == 42


# define BASE_LENGTH base_length
# define REQUIRED_PADDING required_padding

/* Note that increasing this may decrease performance if --ignore-garbage
   is used, because of the memmove operation below.  */
# define DEC_BLOCKSIZE (4200)
static_assert (DEC_BLOCKSIZE % 40 == 0); /* complete encoded blocks for base32*/
static_assert (DEC_BLOCKSIZE % 12 == 0); /* complete encoded blocks for base64*/

ಸ್ಥಿರ idx_t (*base_length) (idx_t len);
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ (*required_padding) (ಪೂರ್ಣಾಙ್ಕ i);
ಸ್ಥಿರ bool (*isubase) (unsigned ವರ್ಣ ch);
ಸ್ಥಿರ ಶೂನ್ಯ (*base_encode) (ವರ್ಣ ಧ್ರುವ *restrict in, idx_t inlen,
                            ವರ್ಣ *restrict out, idx_t outlen);

ಸಂರಚನಾ base16_decode_context
{
  /* Either a 4-bit nibble, or negative if we have no nibble.  */
  signed ವರ್ಣ nibble;
};

ಸಂರಚನಾ z85_decode_context
{
  ಪೂರ್ಣಾಙ್ಕ i;
  unsigned ವರ್ಣ octets[5];
};

ಸಂರಚನಾ base58_context
{
  idx_t size;
  idx_t capacity;
  unsigned ವರ್ಣ *buf COUNTED_BY (capacity);
};

ಸಂರಚನಾ base2_decode_context
{
  unsigned ವರ್ಣ octet;
  ಪೂರ್ಣಾಙ್ಕ bit_pos;
};

ಸಂರಚನಾ base_decode_context
{
  ಸಙ್ಘ {
    ಸಂರಚನಾ base64_decode_context base64;
    ಸಂರಚನಾ base32_decode_context base32;
    ಸಂರಚನಾ base16_decode_context base16;
    ಸಂರಚನಾ base2_decode_context base2;
    ಸಂರಚನಾ z85_decode_context z85;
    ಸಂರಚನಾ base58_context base58;
  } ctx;
  ವರ್ಣ *inbuf;
  idx_t bufsize;
};
ಸ್ಥಿರ ಶೂನ್ಯ (*base_decode_ctx_init) (ಸಂರಚನಾ base_decode_context *ctx);
ಸ್ಥಿರ bool (*base_decode_ctx) (ಸಂರಚನಾ base_decode_context *ctx,
                                ವರ್ಣ ಧ್ರುವ *restrict in, idx_t inlen,
                                ವರ್ಣ *restrict out, idx_t *outlen);
ಸ್ಥಿರ bool (*base_decode_ctx_finalize) (ಸಂರಚನಾ base_decode_context *ctx,
                                         ವರ್ಣ *restrict *out, idx_t *outlen);

ಸಂರಚನಾ base_encode_context
{
  ಸಙ್ಘ {
    ಸಂರಚನಾ base58_context base58;
  } ctx;
};

ಸ್ಥಿರ ಶೂನ್ಯ (*base_encode_ctx_init) (ಸಂರಚನಾ base_encode_context *ctx);
ಸ್ಥಿರ bool (*base_encode_ctx) (ಸಂರಚನಾ base_encode_context *ctx,
                                ವರ್ಣ ಧ್ರುವ *restrict in, idx_t inlen,
                                ವರ್ಣ *restrict out, idx_t *outlen);
ಸ್ಥಿರ bool (*base_encode_ctx_finalize) (ಸಂರಚನಾ base_encode_context *ctx,
                                         ವರ್ಣ *restrict *out, idx_t *outlen);

ಸ್ಥಿರ bool
no_padding (MAYBE_UNUSED ಸಂರಚನಾ base_decode_context *ctx)
{
  ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
no_pending_length (MAYBE_UNUSED ಸಂರಚನಾ base_decode_context *ctx)
{
  ಪ್ರತ್ಯಾಗಮ 0;
}
#endif

#ಯದಿ BASE_TYPE == 42
ಸ್ಥಿರ bool (*has_padding) (ಸಂರಚನಾ base_decode_context *ctx);
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ (*get_pending_length) (ಸಂರಚನಾ base_decode_context *ctx);

ಸ್ಥಿರ bool
base64_ctx_has_padding (ಸಂರಚನಾ base_decode_context *ctx)
{
  ಪ್ರತ್ಯಾಗಮ ctx->ctx.base64.i && ctx->ctx.base64.buf[ctx->ctx.base64.i - 1] == '=';
}

ಸ್ಥಿರ bool
base32_ctx_has_padding (ಸಂರಚನಾ base_decode_context *ctx)
{
  ಪ್ರತ್ಯಾಗಮ ctx->ctx.base32.i && ctx->ctx.base32.buf[ctx->ctx.base32.i - 1] == '=';
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
base64_ctx_get_pending_length (ಸಂರಚನಾ base_decode_context *ctx)
{
  ಪ್ರತ್ಯಾಗಮ ctx->ctx.base64.i;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
base32_ctx_get_pending_length (ಸಂರಚನಾ base_decode_context *ctx)
{
  ಪ್ರತ್ಯಾಗಮ ctx->ctx.base32.i;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
base16_ctx_get_pending_length (MAYBE_UNUSED ಸಂರಚನಾ base_decode_context *ctx)
{
  ಪ್ರತ್ಯಾಗಮ 1; /* Always check nibble state.  */
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
z85_ctx_get_pending_length (ಸಂರಚನಾ base_decode_context *ctx)
{
  ಪ್ರತ್ಯಾಗಮ ctx->ctx.z85.i;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
base2_ctx_get_pending_length (ಸಂರಚನಾ base_decode_context *ctx)
{
  ಪ್ರತ್ಯಾಗಮ ctx->ctx.base2.bit_pos;
}
#ಅನ್ಯಥಾ
ಸ್ಥಿರ bool
has_padding (ಸಂರಚನಾ base_decode_context *ctx)
{
  ಪ್ರತ್ಯಾಗಮ ctx->i && ctx->buf[ctx->i - 1] == '=';
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
get_pending_length (ಸಂರಚನಾ base_decode_context *ctx)
{
  ಪ್ರತ್ಯಾಗಮ ctx->i;
}
#endif


/* Process any pending data in CTX, while auto padding if appropriate.
   Return TRUE on success, FALSE on failure.  */

ಸ್ಥಿರ bool
decode_ctx_finalize (ಸಂರಚನಾ base_decode_context *ctx,
                     ವರ್ಣ *restrict *out, idx_t *outlen)
{
  ಯದಿ (get_pending_length (ctx) == 0)
    {
      *outlen = 0;
      ಪ್ರತ್ಯಾಗಮ true;
    }

  /* Auto-pad input and flush the context */
  ವರ್ಣ padbuf[8] ATTRIBUTE_NONSTRING = "========";
  idx_t pending_len = get_pending_length (ctx);
  idx_t auto_padding = REQUIRED_PADDING (pending_len);
  idx_t n = *outlen;
  bool result;

  ಯದಿ (auto_padding && ! has_padding (ctx))
    {
      affirm (auto_padding <= ಪರಿಮಾಣ (padbuf));
      result = base_decode_ctx (ctx, padbuf, auto_padding, *out, &n);
    }
  ಅನ್ಯಥಾ
    {
      result = base_decode_ctx (ctx, "", 0, *out, &n);
    }

  *outlen = n;
  ಪ್ರತ್ಯಾಗಮ result;
}

#ಯದಿ BASE_TYPE == 42

ಸ್ಥಿರ idx_t
base64_length_wrapper (idx_t len)
{
  ಪ್ರತ್ಯಾಗಮ BASE64_LENGTH (len);
}

ಸ್ಥಿರ ಶೂನ್ಯ
base64_decode_ctx_init_wrapper (ಸಂರಚನಾ base_decode_context *ctx)
{
  base64_decode_ctx_init (&ctx->ctx.base64);
}

ಸ್ಥಿರ bool
base64_decode_ctx_wrapper (ಸಂರಚನಾ base_decode_context *ctx,
                           ವರ್ಣ ಧ್ರುವ *restrict in, idx_t inlen,
                           ವರ್ಣ *restrict out, idx_t *outlen)
{
  ಪ್ರತ್ಯಾಗಮ base64_decode_ctx (&ctx->ctx.base64, in, inlen, out, outlen);
}

ಸ್ಥಿರ ಶೂನ್ಯ
init_inbuf (ಸಂರಚನಾ base_decode_context *ctx)
{
  ctx->bufsize = DEC_BLOCKSIZE;
  ctx->inbuf = xcharalloc (ctx->bufsize);
}

ಸ್ಥಿರ ಶೂನ್ಯ
prepare_inbuf (ಸಂರಚನಾ base_decode_context *ctx, idx_t inlen)
{
  ಯದಿ (ctx->bufsize < inlen)
    ctx->inbuf = xpalloc (ctx->inbuf, &ctx->bufsize,
                          inlen - ctx->bufsize, -1, ಪರಿಮಾಣ *ctx->inbuf);
}


ಸ್ಥಿರ ಶೂನ್ಯ
base64url_encode (ವರ್ಣ ಧ್ರುವ *restrict in, idx_t inlen,
                  ವರ್ಣ *restrict out, idx_t outlen)
{
  base64_encode (in, inlen, out, outlen);
  /* translate 62nd and 63rd characters */
  ವರ್ಣ *p = out;
  ಯಾವತ್ (outlen--)
    {
      ಯದಿ (*p == '+')
        *p = '-';
      ಅನ್ಯಥಾ ಯದಿ (*p == '/')
        *p = '_';
      ++p;
    }
}

ಸ್ಥಿರ bool
isubase64url (unsigned ವರ್ಣ ch)
{
  ಪ್ರತ್ಯಾಗಮ (ch == '-' || ch == '_'
          || (ch != '+' && ch != '/' && isubase64 (ch)));
}

ಸ್ಥಿರ ಶೂನ್ಯ
base64url_decode_ctx_init_wrapper (ಸಂರಚನಾ base_decode_context *ctx)
{
  base64_decode_ctx_init (&ctx->ctx.base64);
  init_inbuf (ctx);
}


ಸ್ಥಿರ bool
base64url_decode_ctx_wrapper (ಸಂರಚನಾ base_decode_context *ctx,
                              ವರ್ಣ ಧ್ರುವ *restrict in, idx_t inlen,
                              ವರ್ಣ *restrict out, idx_t *outlen)
{
  prepare_inbuf (ctx, inlen);
  memcpy (ctx->inbuf, in, inlen);

  /* translate 62nd and 63rd characters */
  idx_t i = inlen;
  ವರ್ಣ *p = ctx->inbuf;
  ಯಾವತ್ (i--)
    {
      ಯದಿ (*p == '+' || *p == '/')
        {
          *outlen = 0;
          ಪ್ರತ್ಯಾಗಮ false; /* reject base64 input */
        }
      ಅನ್ಯಥಾ ಯದಿ (*p == '-')
        *p = '+';
      ಅನ್ಯಥಾ ಯದಿ (*p == '_')
        *p = '/';
      ++p;
    }

  ಪ್ರತ್ಯಾಗಮ base64_decode_ctx (&ctx->ctx.base64, ctx->inbuf, inlen,
                            out, outlen);
}



ಸ್ಥಿರ idx_t
base32_length_wrapper (idx_t len)
{
  ಪ್ರತ್ಯಾಗಮ BASE32_LENGTH (len);
}

ಸ್ಥಿರ ಶೂನ್ಯ
base32_decode_ctx_init_wrapper (ಸಂರಚನಾ base_decode_context *ctx)
{
  base32_decode_ctx_init (&ctx->ctx.base32);
}

ಸ್ಥಿರ bool
base32_decode_ctx_wrapper (ಸಂರಚನಾ base_decode_context *ctx,
                           ವರ್ಣ ಧ್ರುವ *restrict in, idx_t inlen,
                           ವರ್ಣ *restrict out, idx_t *outlen)
{
  ಪ್ರತ್ಯಾಗಮ base32_decode_ctx (&ctx->ctx.base32, in, inlen, out, outlen);
}

/* ABCDEFGHIJKLMNOPQRSTUVWXYZ234567
     to
   0123456789ABCDEFGHIJKLMNOPQRSTUV */
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ base32_norm_to_hex[32 + 9] = {
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
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ base32_hex_to_norm[32 + 9] = {
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


ಅನ್ತಃಸ್ಥ ಸ್ಥಿರ bool
isubase32hex (unsigned ವರ್ಣ ch)
{
  ಪ್ರತ್ಯಾಗಮ ('0' <= ch && ch <= '9') || ('A' <= ch && ch <= 'V');
}


ಸ್ಥಿರ ಶೂನ್ಯ
base32hex_encode (ವರ್ಣ ಧ್ರುವ *restrict in, idx_t inlen,
                  ವರ್ಣ *restrict out, idx_t outlen)
{
  base32_encode (in, inlen, out, outlen);

  ಪ್ರತಿ (ವರ್ಣ *p = out; outlen--; p++)
    {
      affirm (0x32 <= *p && *p <= 0x5a);          /* LCOV_EXCL_LINE */
      *p = base32_norm_to_hex[*p - 0x32];
    }
}


ಸ್ಥಿರ ಶೂನ್ಯ
base32hex_decode_ctx_init_wrapper (ಸಂರಚನಾ base_decode_context *ctx)
{
  base32_decode_ctx_init (&ctx->ctx.base32);
  init_inbuf (ctx);
}


ಸ್ಥಿರ bool
base32hex_decode_ctx_wrapper (ಸಂರಚನಾ base_decode_context *ctx,
                              ವರ್ಣ ಧ್ರುವ *restrict in, idx_t inlen,
                              ವರ್ಣ *restrict out, idx_t *outlen)
{
  prepare_inbuf (ctx, inlen);

  idx_t i = inlen;
  ವರ್ಣ *p = ctx->inbuf;
  ಯಾವತ್ (i--)
    {
      ಯದಿ (isubase32hex (*in))
        *p = base32_hex_to_norm[*in - 0x30];
      ಅನ್ಯಥಾ
        *p = *in;
      ++p;
      ++in;
    }

  ಪ್ರತ್ಯಾಗಮ base32_decode_ctx (&ctx->ctx.base32, ctx->inbuf, inlen,
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

ಸ್ಥಿರ signed ವರ್ಣ ಧ್ರುವ base16_to_int[256] = {
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
isubase16 (unsigned ವರ್ಣ ch)
{
  ಪ್ರತ್ಯಾಗಮ ch < ಪರಿಮಾಣ base16_to_int && 0 <= base16_to_int[ch];
}

ಸ್ಥಿರ idx_t
base16_length (idx_t len)
{
  ಪ್ರತ್ಯಾಗಮ len * 2;
}


ಸ್ಥಿರ ಶೂನ್ಯ
base16_encode (ವರ್ಣ ಧ್ರುವ *restrict in, idx_t inlen,
               ವರ್ಣ *restrict out, idx_t outlen)
{
  ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ base16[16] ATTRIBUTE_NONSTRING = "0123456789ABCDEF";

  ಯಾವತ್ (inlen && outlen)
    {
      unsigned ವರ್ಣ c = *in;
      *out++ = base16[c >> 4];
      *out++ = base16[c & 0x0F];
      ++in;
      inlen--;
      outlen -= 2;
    }
}


ಸ್ಥಿರ ಶೂನ್ಯ
base16_decode_ctx_init (ಸಂರಚನಾ base_decode_context *ctx)
{
  init_inbuf (ctx);
  ctx->ctx.base16.nibble = -1;
}


ಸ್ಥಿರ bool
base16_decode_ctx (ಸಂರಚನಾ base_decode_context *ctx,
                   ವರ್ಣ ಧ್ರುವ *restrict in, idx_t inlen,
                   ವರ್ಣ *restrict out, idx_t *outlen)
{
  bool ignore_ಸಾಲುಗಳು = true;  /* for now, always ignore them */
  ವರ್ಣ *out0 = out;
  signed ವರ್ಣ nibble = ctx->ctx.base16.nibble;

  /* inlen==0 is request to flush output.
     if there is a dangling high nibble - we are missing the low nibble,
     so return false - indicating an invalid input.  */
  ಯದಿ (inlen == 0)
    {
      *outlen = 0;
      ಪ್ರತ್ಯಾಗಮ nibble < 0;
    }

  ಯಾವತ್ (inlen--)
    {
      unsigned ವರ್ಣ c = *in++;
      ಯದಿ (ignore_ಸಾಲುಗಳು && c == '\n')
        ಅನುವರ್ತ;

      ಯದಿ (ಪರಿಮಾಣ base16_to_int <= c || base16_to_int[c] < 0)
        {
          *outlen = out - out0;
          ಪ್ರತ್ಯಾಗಮ false; /* garbage - return false */
        }

      ಯದಿ (nibble < 0)
        nibble = base16_to_int[c];
      ಅನ್ಯಥಾ
        {
          /* have both nibbles, write octet */
          *out++ = (nibble << 4) + base16_to_int[c];
          nibble = -1;
        }
    }

  ctx->ctx.base16.nibble = nibble;
  *outlen = out - out0;
  ಪ್ರತ್ಯಾಗಮ true;
}



ATTRIBUTE_PURE
ಸ್ಥಿರ idx_t
z85_length (idx_t len)
{
  /* Z85 does not allow padding, so no need to round to highest integer.  */
  idx_t z85_len = (len * 5) / 4;
  affirm (0 <= z85_len);
  ಪ್ರತ್ಯಾಗಮ z85_len;
}

ಸ್ಥಿರ bool
isuz85 (unsigned ವರ್ಣ ch)
{
  ಪ್ರತ್ಯಾಗಮ c_isalnum (ch) || strchr (".-:+=^!/*?&<>()[]{}@%$#", ch) != ರಿಕ್ತ;
}

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ z85_encoding[85] ATTRIBUTE_NONSTRING =
  "0123456789"
  "abcdefghijklmnopqrstuvwxyz"
  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
  ".-:+=^!/*?&<>()[]{}@%$#";

ಸ್ಥಿರ ಶೂನ್ಯ
z85_encode (ವರ್ಣ ಧ್ರುವ *restrict in, idx_t inlen,
            ವರ್ಣ *restrict out, idx_t outlen)
{
  ಪೂರ್ಣಾಙ್ಕ i = 0;
  unsigned ವರ್ಣ quad[4];
  idx_t outidx = 0;

  ಯಾವತ್ (true)
    {
      ಯದಿ (inlen == 0)
        {
          /* no more input, exactly on 4 octet boundary. */
          ಯದಿ (i == 0)
            ಪ್ರತ್ಯಾಗಮ;

          /* currently, there's no way to return an error in encoding.  */
          error (EXIT_FAILURE, 0,
                 _("invalid input (length must be multiple of 4 characters)"));
        }
      ಅನ್ಯಥಾ
        {
          quad[i++] = *in++;
          --inlen;
        }

      /* Got a quad, encode it */
      ಯದಿ (i == 4)
        {
          int_fast64_t val = quad[0];
          val = (val << 24) + (quad[1] << 16) + (quad[2] << 8) + quad[3];

          ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 4; j >= 0; --j)
            {
              ಪೂರ್ಣಾಙ್ಕ c = val % 85;
              val /= 85;

              /* NOTE: if there is padding (which is trimmed by z85
                 before outputting the result), the output buffer 'out'
                 might not include enough allocated bytes for the padding,
                 so don't store them. */
              ಯದಿ (outidx + j < outlen)
                out[j] = z85_encoding[c];
            }
          out += 5;
          outidx += 5;
          i = 0;
        }
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
z85_decode_ctx_init (ಸಂರಚನಾ base_decode_context *ctx)
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
ಸ್ಥಿರ signed ವರ್ಣ ಧ್ರುವ z85_decoding[93] = {
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
z85_decode_ctx (ಸಂರಚನಾ base_decode_context *ctx,
                ವರ್ಣ ಧ್ರುವ *restrict in, idx_t inlen,
                ವರ್ಣ *restrict out, idx_t *outlen)
{
  bool ignore_ಸಾಲುಗಳು = true;  /* for now, always ignore them */

  *outlen = 0;

  /* inlen==0 is request to flush output.
     if there are dangling values - we are missing entries,
     so return false - indicating an invalid input.  */
  ಯದಿ (inlen == 0)
    {
      ಯದಿ (ctx->ctx.z85.i > 0)
        {
          /* Z85 variant does not allow padding - input must
             be a multiple of 5 - so return error.  */
          ಪ್ರತ್ಯಾಗಮ false;
        }
      ಪ್ರತ್ಯಾಗಮ true;
    }

  ಯಾವತ್ (inlen--)
    {
      ಯದಿ (ignore_ಸಾಲುಗಳು && *in == '\n')
        {
          ++in;
          ಅನುವರ್ತ;
        }

      /* z85 decoding */
      unsigned ವರ್ಣ c = *in;

      ಯದಿ (c >= 33 && c <= 125)
        {
          signed ವರ್ಣ ch = z85_decoding[c - 33];
          ಯದಿ (ch < 0)
            ಪ್ರತ್ಯಾಗಮ false; /* garbage - return false */
          c = ch;
        }
      ಅನ್ಯಥಾ
        ಪ್ರತ್ಯಾಗಮ false; /* garbage - return false */

      ++in;

      ctx->ctx.z85.octets[ctx->ctx.z85.i++] = c;
      ಯದಿ (ctx->ctx.z85.i == 5)
        {
          /* decode the lowest 4 octets, then check for overflows.  */
          int_fast64_t val = Z85_LO_CTX_TO_32BIT_VAL (ctx);

          /* The Z85 spec and the reference implementation say nothing
             about overflows. To be on the safe side, reject them.  */

          val += Z85_HI_CTX_TO_32BIT_VAL (ctx);
          ಯದಿ ((val >> 24) & ~0xFF)
            ಪ್ರತ್ಯಾಗಮ false;

          *out++ = val >> 24;
          *out++ = (val >> 16) & 0xFF;
          *out++ = (val >> 8) & 0xFF;
          *out++ = val & 0xFF;

          *outlen += 4;

          ctx->ctx.z85.i = 0;
        }
    }
  ಪ್ರತ್ಯಾಗಮ true;
}


ಅನ್ತಃಸ್ಥ ಸ್ಥಿರ bool
isubase2 (unsigned ವರ್ಣ ch)
{
  ಪ್ರತ್ಯಾಗಮ ch == '0' || ch == '1';
}

ಸ್ಥಿರ idx_t
base2_length (idx_t len)
{
  ಪ್ರತ್ಯಾಗಮ len * 8;
}


ಅನ್ತಃಸ್ಥ ಸ್ಥಿರ ಶೂನ್ಯ
base2msbf_encode (ವರ್ಣ ಧ್ರುವ *restrict in, idx_t inlen,
                  ವರ್ಣ *restrict out, idx_t outlen)
{
  ಯಾವತ್ (inlen && outlen)
    {
      unsigned ವರ್ಣ c = *in;
      ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < 8; i++)
        {
          *out++ = c & 0x80 ? '1' : '0';
          c <<= 1;
        }
      inlen--;
      outlen -= 8;
      ++in;
    }
}

ಅನ್ತಃಸ್ಥ ಸ್ಥಿರ ಶೂನ್ಯ
base2lsbf_encode (ವರ್ಣ ಧ್ರುವ *restrict in, idx_t inlen,
                  ವರ್ಣ *restrict out, idx_t outlen)
{
  ಯಾವತ್ (inlen && outlen)
    {
      unsigned ವರ್ಣ c = *in;
      ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < 8; i++)
        {
          *out++ = c & 0x01 ? '1' : '0';
          c >>= 1;
        }
      inlen--;
      outlen -= 8;
      ++in;
    }
}


ಸ್ಥಿರ ಶೂನ್ಯ
base2_decode_ctx_init (ಸಂರಚನಾ base_decode_context *ctx)
{
  init_inbuf (ctx);
  ctx->ctx.base2.octet = 0;
  ctx->ctx.base2.bit_pos = 0;
}


ಸ್ಥಿರ bool
base2lsbf_decode_ctx (ಸಂರಚನಾ base_decode_context *ctx,
                      ವರ್ಣ ಧ್ರುವ *restrict in, idx_t inlen,
                      ವರ್ಣ *restrict out, idx_t *outlen)
{
  bool ignore_ಸಾಲುಗಳು = true;  /* for now, always ignore them */

  *outlen = 0;

  /* inlen==0 is request to flush output.
     if there is a dangling bit - we are missing some bits,
     so return false - indicating an invalid input.  */
  ಯದಿ (inlen == 0)
    ಪ್ರತ್ಯಾಗಮ ctx->ctx.base2.bit_pos == 0;

  ಯಾವತ್ (inlen--)
    {
      ಯದಿ (ignore_ಸಾಲುಗಳು && *in == '\n')
        {
          ++in;
          ಅನುವರ್ತ;
        }

      ಯದಿ (!isubase2 (*in))
        ಪ್ರತ್ಯಾಗಮ false;

      bool bit = (*in == '1');
      ctx->ctx.base2.octet |= bit << ctx->ctx.base2.bit_pos;
      ++ctx->ctx.base2.bit_pos;

      ಯದಿ (ctx->ctx.base2.bit_pos == 8)
        {
          *out++ = ctx->ctx.base2.octet;
          ctx->ctx.base2.octet = 0;
          ++*outlen;
          ctx->ctx.base2.bit_pos = 0;
        }

      ++in;
    }

  ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ bool
base2msbf_decode_ctx (ಸಂರಚನಾ base_decode_context *ctx,
                      ವರ್ಣ ಧ್ರುವ *restrict in, idx_t inlen,
                      ವರ್ಣ *restrict out, idx_t *outlen)
{
  bool ignore_ಸಾಲುಗಳು = true;  /* for now, always ignore them */

  *outlen = 0;

  /* inlen==0 is request to flush output.
     if there is a dangling bit - we are missing some bits,
     so return false - indicating an invalid input.  */
  ಯದಿ (inlen == 0)
    ಪ್ರತ್ಯಾಗಮ ctx->ctx.base2.bit_pos == 0;

  ಯಾವತ್ (inlen--)
    {
      ಯದಿ (ignore_ಸಾಲುಗಳು && *in == '\n')
        {
          ++in;
          ಅನುವರ್ತ;
        }

      ಯದಿ (!isubase2 (*in))
        ಪ್ರತ್ಯಾಗಮ false;

      bool bit = (*in == '1');
      ಯದಿ (ctx->ctx.base2.bit_pos == 0)
        ctx->ctx.base2.bit_pos = 8;
      --ctx->ctx.base2.bit_pos;
      ctx->ctx.base2.octet |= bit << ctx->ctx.base2.bit_pos;

      ಯದಿ (ctx->ctx.base2.bit_pos == 0)
        {
          *out++ = ctx->ctx.base2.octet;
          ctx->ctx.base2.octet = 0;
          ++*outlen;
        }

      ++in;
    }

  ಪ್ರತ್ಯಾಗಮ true;
}

/* Map from GMP (up to base 62):
   "0123456789 ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuv";
   to base 58:
   "123456789A BCDEFGHJKLMNPQRSTUVWXYZabc defghijkmnopqrstuvwxyz";  */
ಸ್ಥಿರ signed ವರ್ಣ ಧ್ರುವ gmp_to_base58[256] = {
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

ಸ್ಥಿರ signed ವರ್ಣ ಧ್ರುವ base58_to_gmp[256] = {
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
isubase58 (unsigned ವರ್ಣ ch)
{
  ಪ್ರತ್ಯಾಗಮ ch < ಪರಿಮಾಣ base58_to_gmp && 0 <= base58_to_gmp[ch];
}


ATTRIBUTE_PURE
ಸ್ಥಿರ idx_t
base58_length (idx_t len)
{
  /* Base58 output length is approximately log(256)/log(58),
     which is approximately len * 138 / 100,
     which is at most ((len + 100 - 1) / 100) * 138
     +1 to ensure we've enough place for NUL  */
  idx_t base58_len = ((len + 99) / 100) * 138 + 1;
  affirm (0 < base58_len);
  ಪ್ರತ್ಯಾಗಮ base58_len;
}


ಸ್ಥಿರ ಶೂನ್ಯ
base58_encode_ctx_init (ಸಂರಚನಾ base_encode_context *ctx)
{
  ctx->ctx.base58.buf = ರಿಕ್ತ;
  ctx->ctx.base58.size = 0;
  ctx->ctx.base58.capacity = 0;
}


ಸ್ಥಿರ bool
base58_encode_ctx (ಸಂರಚನಾ base_encode_context *ctx,
                   ವರ್ಣ ಧ್ರುವ *restrict in, idx_t inlen,
                   MAYBE_UNUSED ವರ್ಣ *restrict out, idx_t *outlen)
{
  *outlen = 0;   /* Only accumulate input in this function.  */

  ಯದಿ (inlen == 0)
    ಪ್ರತ್ಯಾಗಮ true;

  idx_t free_space = ctx->ctx.base58.capacity - ctx->ctx.base58.size;
  ಯದಿ (free_space < inlen)
    {
      ctx->ctx.base58.buf = xpalloc (ctx->ctx.base58.buf,
                                     &ctx->ctx.base58.capacity,
                                     inlen - free_space,
                                     -1, ಪರಿಮಾಣ *ctx->ctx.base58.buf);
    }

  memcpy (ctx->ctx.base58.buf + ctx->ctx.base58.size, in, inlen);
  ctx->ctx.base58.size += inlen;

  ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ ಶೂನ್ಯ
base58_encode (ವರ್ಣ ಧ್ರುವ *data, size_t data_len,
               ವರ್ಣ *out, idx_t *outlen)
{
  affirm (base_length (data_len) <= *outlen);

  size_t zeros = 0;
  ಯಾವತ್ (zeros < data_len && data[zeros] == 0)
    zeros++;

  memset (out, '1', zeros);
  ವರ್ಣ *p = out + zeros;

  /* Use GMP to convert from base 256 to base 58.  */
  mpz_t ಅಂಕೆ;
  mpz_init (ಅಂಕೆ);
  ಯದಿ (data_len - zeros)
    {
      mpz_import (ಅಂಕೆ, data_len - zeros, 1, 1, 0, 0, data + zeros);
      ಪ್ರತಿ (p = mpz_get_str (p, 58, ಅಂಕೆ); *p; p++)
        *p = gmp_to_base58[to_uchar (*p)];
    }
  mpz_clear (ಅಂಕೆ);

  *outlen = p - out;
}


ಸ್ಥಿರ bool
base58_encode_ctx_finalize (ಸಂರಚನಾ base_encode_context *ctx,
                            ವರ್ಣ *restrict *out, idx_t *outlen)
{
  /* Ensure output buffer is large enough.  */
  idx_t max_outlen = base_length (ctx->ctx.base58.size);
  ಯದಿ (max_outlen > *outlen)
    {
      *out = xrealloc (*out, max_outlen);
      *outlen = max_outlen;
    }

  base58_encode ((ವರ್ಣ *)ctx->ctx.base58.buf, ctx->ctx.base58.size,
                 *out, outlen);

  ಮುಕ್ತ (ctx->ctx.base58.buf);
  ctx->ctx.base58.buf = ರಿಕ್ತ;
  ctx->ctx.base58.size = 0;
  ctx->ctx.base58.capacity = 0;

  ಪ್ರತ್ಯಾಗಮ true;
}


ಸ್ಥಿರ ಶೂನ್ಯ
base58_decode_ctx_init (ಸಂರಚನಾ base_decode_context *ctx)
{
  ctx->ctx.base58.size = 0;
  ctx->ctx.base58.capacity = 0;
  ctx->ctx.base58.buf = ರಿಕ್ತ;
}

ಸ್ಥಿರ bool
base58_decode_ctx (ಸಂರಚನಾ base_decode_context *ctx,
                   ವರ್ಣ ಧ್ರುವ *restrict in, idx_t inlen,
                   MAYBE_UNUSED ವರ್ಣ *restrict out, idx_t *outlen)
{
  bool ignore_ಸಾಲುಗಳು = true;  /* for now, always ignore them */

  *outlen = 0;   /* Only accumulate input in this function.  */

  ಯದಿ (inlen == 0)
    ಪ್ರತ್ಯಾಗಮ true;

  idx_t free_space = ctx->ctx.base58.capacity - ctx->ctx.base58.size;
  free_space -= 1;  /* Ensure we leave space for NUL (for mpz_set_str).  */
  ಯದಿ (free_space < inlen)
    {
      ctx->ctx.base58.buf = xpalloc (ctx->ctx.base58.buf,
                                     &ctx->ctx.base58.capacity,
                                     inlen - free_space,
                                     -1, ಪರಿಮಾಣ *ctx->ctx.base58.buf);
    }

  /* Accumulate all valid input characters in our buffer.
     Note we don't rely on mpz_set_str() for validation
     as that allows (skips) all whitespace.  */
  ಪ್ರತಿ (idx_t i = 0; i < inlen; i++)
    {
      unsigned ವರ್ಣ c = in[i];

      ಯದಿ (ignore_ಸಾಲುಗಳು && c == '\n')
        ಅನುವರ್ತ;

      ಯದಿ (!isubase58 (c))
        ಪ್ರತ್ಯಾಗಮ false;

      ctx->ctx.base58.buf[ctx->ctx.base58.size++] = base58_to_gmp[to_uchar (c)];
    }

  ಪ್ರತ್ಯಾಗಮ true;
}


ಸ್ಥಿರ bool
base58_decode (ವರ್ಣ ಧ್ರುವ *data, size_t data_len,
               ವರ್ಣ *restrict out, idx_t *outlen)
{
  affirm (data_len <= *outlen);

  size_t ones = 0;
  ಯಾವತ್ (ones < data_len && data[ones] == base58_to_gmp['1'])
    ones++;

  memset (out, 0, ones);

  /* Use GMP to convert from base 58 to base 256.  */
  mpz_t ಅಂಕೆ;
  mpz_init (ಅಂಕೆ);

  ಯದಿ ((data_len - ones) && mpz_set_str (ಅಂಕೆ, data + ones, 58) != 0)
    {
      mpz_clear (ಅಂಕೆ);
      *outlen = 0;
      ಪ್ರತ್ಯಾಗಮ false;
    }

  size_t exported_size = 0;
  ಯದಿ (data_len - ones)
    {
      size_t ದ್ವಿಮಾನ_size = (mpz_sizeinbase (ಅಂಕೆ, 2) + 7) / 8;
      affirm (*outlen - ones >= ದ್ವಿಮಾನ_size);
      mpz_export (out + ones, &exported_size, 1, 1, 0, 0, ಅಂಕೆ);
    }

  mpz_clear (ಅಂಕೆ);
  *outlen = ones + exported_size;
  ಪ್ರತ್ಯಾಗಮ true;
}


ಸ್ಥಿರ bool
base58_decode_ctx_finalize (ಸಂರಚನಾ base_decode_context *ctx,
                            ವರ್ಣ *restrict *out, idx_t *outlen)
{
  /* Ensure output buffer is large enough.
     Worst case is input is all '1's.  */
  idx_t max_outlen = ctx->ctx.base58.size;
  ಯದಿ (max_outlen > *outlen)
    {
      *out = xrealloc (*out, max_outlen);
      *outlen = max_outlen;
    }

  /* Ensure input buffer is NUL terminated (for mpz_get_str).  */
  ಯದಿ (ctx->ctx.base58.size)
    ctx->ctx.base58.buf[ctx->ctx.base58.size] = '\0';

  bool ret = base58_decode ((ವರ್ಣ *)ctx->ctx.base58.buf, ctx->ctx.base58.size,
                            *out, outlen);

  ಮುಕ್ತ (ctx->ctx.base58.buf);
  ctx->ctx.base58.buf = ರಿಕ್ತ;
  ctx->ctx.base58.size = 0;
  ctx->ctx.base58.capacity = 0;

  ಪ್ರತ್ಯಾಗಮ ret;
}

#endif /* BASE_TYPE == 42, i.e., "basenc"*/



ಸ್ಥಿರ ಶೂನ್ಯ
wrap_write (ವರ್ಣ ಧ್ರುವ *buffer, idx_t len,
            idx_t wrap_column, idx_t *current_column, FILE *out)
{
  ಯದಿ (wrap_column == 0)
    {
      /* Simple write. */
      ಯದಿ (fwrite (buffer, 1, len, stdout) < len)
        write_error ();
    }
  ಅನ್ಯಥಾ
    ಪ್ರತಿ (idx_t written = 0; written < len; )
      {
        idx_t to_write = MIN (wrap_column - *current_column, len - written);

        ಯದಿ (to_write == 0)
          {
            ಯದಿ (fputc ('\n', out) == EOF)
              write_error ();
            *current_column = 0;
          }
        ಅನ್ಯಥಾ
          {
            ಯದಿ (fwrite (buffer + written, 1, to_write, stdout) < to_write)
              write_error ();
            *current_column += to_write;
            written += to_write;
          }
      }
}

ಸ್ಥಿರ _Noreturn ಶೂನ್ಯ
finish_and_exit (FILE *in, ವರ್ಣ ಧ್ರುವ *infile)
{
  ಯದಿ (fclose (in) != 0)
    {
      ಯದಿ (streq (infile, "-"))
        error (EXIT_FAILURE, errno, _("closing standard input"));
      ಅನ್ಯಥಾ
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
    }

  exit (EXIT_SUCCESS);
}

ಸ್ಥಿರ _Noreturn ಶೂನ್ಯ
do_encode (FILE *in, ವರ್ಣ ಧ್ರುವ *infile, FILE *out, idx_t wrap_column)
{
  idx_t current_column = 0;
  ವರ್ಣ *inbuf, *outbuf;
  idx_t sum;

  inbuf = xmalloc (ENC_BLOCKSIZE);
  outbuf = xmalloc (BASE_LENGTH (ENC_BLOCKSIZE));

#ಯದಿ BASE_TYPE == 42
  /* Initialize encoding context if needed (for base58) */
  ಸಂರಚನಾ base_encode_context encode_ctx;
  bool use_ctx = (base_encode_ctx_init != ರಿಕ್ತ);
  ಯದಿ (use_ctx)
    base_encode_ctx_init (&encode_ctx);
#endif

  ಕುರು
    {
      idx_t n;

      sum = 0;
      ಕುರು
        {
          n = fread (inbuf + sum, 1, ENC_BLOCKSIZE - sum, in);
          sum += n;
        }
      ಯಾವತ್ (!feof (in) && !ferror (in) && sum < ENC_BLOCKSIZE);

      ಯದಿ (sum > 0)
        {
#ಯದಿ BASE_TYPE == 42
          ಯದಿ (use_ctx)
            {
              idx_t outlen = 0;
              base_encode_ctx (&encode_ctx, inbuf, sum, outbuf, &outlen);

              wrap_write (outbuf, outlen, wrap_column, &current_column, out);
            }
          ಅನ್ಯಥಾ
#endif
            {
              /* Process input one block at a time.  Note that ENC_BLOCKSIZE
                 is sized so that no pad chars will appear in output. */
              base_encode (inbuf, sum, outbuf, BASE_LENGTH (sum));

              wrap_write (outbuf, BASE_LENGTH (sum), wrap_column,
                          &current_column, out);
            }
        }
    }
  ಯಾವತ್ (!feof (in) && !ferror (in) && sum == ENC_BLOCKSIZE);

#ಯದಿ BASE_TYPE == 42
  ಯದಿ (use_ctx && base_encode_ctx_finalize)
    {
      idx_t outlen = BASE_LENGTH (ENC_BLOCKSIZE);
      base_encode_ctx_finalize (&encode_ctx, &outbuf, &outlen);

      wrap_write (outbuf, outlen, wrap_column, &current_column, out);
    }
#endif

  /* When wrapping, terminate last line. */
  ಯದಿ (wrap_column && current_column > 0 && fputc ('\n', out) == EOF)
    write_error ();

  ಯದಿ (ferror (in))
    error (EXIT_FAILURE, errno, _("read error"));

  finish_and_exit (in, infile);
}

ಸ್ಥಿರ _Noreturn ಶೂನ್ಯ
do_decode (FILE *in, ವರ್ಣ ಧ್ರುವ *infile, FILE *out, bool ignore_garbage)
{
  ವರ್ಣ *inbuf, *outbuf;
  idx_t sum;
  ಸಂರಚನಾ base_decode_context ctx;

  inbuf = xmalloc (BASE_LENGTH (DEC_BLOCKSIZE));
  outbuf = xmalloc (DEC_BLOCKSIZE);

#ಯದಿ BASE_TYPE == 42
  ctx.inbuf = ರಿಕ್ತ;
#endif
  base_decode_ctx_init (&ctx);

  ಕುರು
    {
      bool ok;

      sum = 0;
      ಕುರು
        {
          idx_t n = fread (inbuf + sum,
                           1, BASE_LENGTH (DEC_BLOCKSIZE) - sum, in);

          ಯದಿ (ignore_garbage)
            {
              ಪ್ರತಿ (idx_t i = 0; n > 0 && i < n;)
                {
                  ಯದಿ (isubase (inbuf[sum + i])
                      || (REQUIRED_PADDING (1) && inbuf[sum + i] == '='))
                    i++;
                  ಅನ್ಯಥಾ
                    memmove (inbuf + sum + i, inbuf + sum + i + 1, --n - i);
                }
            }

          sum += n;

          ಯದಿ (ferror (in))
            error (EXIT_FAILURE, errno, _("read error"));
        }
      ಯಾವತ್ (sum < BASE_LENGTH (DEC_BLOCKSIZE) && !feof (in));

      ಯಾವತ್ (sum || feof (in))
        {
          idx_t n = DEC_BLOCKSIZE;
          ಯದಿ (sum)
            ok = base_decode_ctx (&ctx, inbuf, sum, outbuf, &n);
          ಅನ್ಯಥಾ
            ok = base_decode_ctx_finalize (&ctx, &outbuf, &n);

          ಯದಿ (fwrite (outbuf, 1, n, out) < n)
            write_error ();

          ಯದಿ (! ok)
            error (EXIT_FAILURE, 0, _("invalid input"));

          ಯದಿ (sum == 0)
            ಭಙ್ಗ;
          sum = 0;
        }
    }
  ಯಾವತ್ (!feof (in));

  finish_and_exit (in, infile);
}

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  ಪೂರ್ಣಾಙ್ಕ opt;
  FILE *ಒಳಹರಿವು_fh;
  ವರ್ಣ ಧ್ರುವ *infile;

  /* True if --decode has been given and we should decode data. */
  bool decode = false;
  /* True if we should ignore non-base-alphabetic characters. */
  bool ignore_garbage = false;
  /* Wrap encoded data around the 76th column, by default. */
  idx_t wrap_column = 76;

#ಯದಿ BASE_TYPE == 42
  ಪೂರ್ಣಾಙ್ಕ base_type = 0;
#endif

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಯಾವತ್ ((opt = getopt_long (argc, argv, "diw:", long_options, ರಿಕ್ತ)) != -1)
    ವಿಕಲ್ಪ (opt)
      {
      ಪಕ್ಷ 'd':
        decode = true;
        ಭಙ್ಗ;

      ಪಕ್ಷ 'w':
        {
          intmax_t w;
          strtol_error s_err = xstrtoimax (optarg, ರಿಕ್ತ, 10, &w, "");
          ಯದಿ (LONGINT_OVERFLOW < s_err || w < 0)
            error (EXIT_FAILURE, 0, "%s: %s",
                   _("invalid wrap size"), quote (optarg));
          wrap_column = s_err == LONGINT_OVERFLOW || IDX_MAX < w ? 0 : w;
        }
        ಭಙ್ಗ;

      ಪಕ್ಷ 'i':
        ignore_garbage = true;
        ಭಙ್ಗ;

#ಯದಿ BASE_TYPE == 42
      ಪಕ್ಷ BASE64_OPTION:
      ಪಕ್ಷ BASE64URL_OPTION:
      ಪಕ್ಷ BASE32_OPTION:
      ಪಕ್ಷ BASE32HEX_OPTION:
      ಪಕ್ಷ BASE16_OPTION:
      ಪಕ್ಷ BASE2MSBF_OPTION:
      ಪಕ್ಷ BASE2LSBF_OPTION:
      ಪಕ್ಷ Z85_OPTION:
      ಪಕ್ಷ BASE58_OPTION:
        base_type = opt;
        ಭಙ್ಗ;
#endif

      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

      ಸ್ವಭಾವ:
        usage (EXIT_FAILURE);
        ಭಙ್ಗ;
      }

#ಯದಿ BASE_TYPE == 42
  required_padding = no_required_padding;
  has_padding = no_padding;
  get_pending_length = no_pending_length;
  base_decode_ctx_finalize = decode_ctx_finalize;

  ವಿಕಲ್ಪ (base_type)
    {
    ಪಕ್ಷ BASE64_OPTION:
      base_length = base64_length_wrapper;
      required_padding = base64_required_padding;
      has_padding = base64_ctx_has_padding;
      get_pending_length = base64_ctx_get_pending_length;
      isubase = isubase64;
      base_encode = base64_encode;
      base_decode_ctx_init = base64_decode_ctx_init_wrapper;
      base_decode_ctx = base64_decode_ctx_wrapper;
      ಭಙ್ಗ;

    ಪಕ್ಷ BASE64URL_OPTION:
      base_length = base64_length_wrapper;
      required_padding = base64_required_padding;
      has_padding = base64_ctx_has_padding;
      get_pending_length = base64_ctx_get_pending_length;
      isubase = isubase64url;
      base_encode = base64url_encode;
      base_decode_ctx_init = base64url_decode_ctx_init_wrapper;
      base_decode_ctx = base64url_decode_ctx_wrapper;
      ಭಙ್ಗ;

    ಪಕ್ಷ BASE32_OPTION:
      base_length = base32_length_wrapper;
      required_padding = base32_required_padding;
      has_padding = base32_ctx_has_padding;
      get_pending_length = base32_ctx_get_pending_length;
      isubase = isubase32;
      base_encode = base32_encode;
      base_decode_ctx_init = base32_decode_ctx_init_wrapper;
      base_decode_ctx = base32_decode_ctx_wrapper;
      ಭಙ್ಗ;

    ಪಕ್ಷ BASE32HEX_OPTION:
      base_length = base32_length_wrapper;
      required_padding = base32_required_padding;
      has_padding = base32_ctx_has_padding;
      get_pending_length = base32_ctx_get_pending_length;
      isubase = isubase32hex;
      base_encode = base32hex_encode;
      base_decode_ctx_init = base32hex_decode_ctx_init_wrapper;
      base_decode_ctx = base32hex_decode_ctx_wrapper;
      ಭಙ್ಗ;

    ಪಕ್ಷ BASE16_OPTION:
      base_length = base16_length;
      get_pending_length = base16_ctx_get_pending_length;
      isubase = isubase16;
      base_encode = base16_encode;
      base_decode_ctx_init = base16_decode_ctx_init;
      base_decode_ctx = base16_decode_ctx;
      ಭಙ್ಗ;

    ಪಕ್ಷ BASE2MSBF_OPTION:
      base_length = base2_length;
      get_pending_length = base2_ctx_get_pending_length;
      isubase = isubase2;
      base_encode = base2msbf_encode;
      base_decode_ctx_init = base2_decode_ctx_init;
      base_decode_ctx = base2msbf_decode_ctx;
      ಭಙ್ಗ;

    ಪಕ್ಷ BASE2LSBF_OPTION:
      base_length = base2_length;
      get_pending_length = base2_ctx_get_pending_length;
      isubase = isubase2;
      base_encode = base2lsbf_encode;
      base_decode_ctx_init = base2_decode_ctx_init;
      base_decode_ctx = base2lsbf_decode_ctx;
      ಭಙ್ಗ;

    ಪಕ್ಷ Z85_OPTION:
      base_length = z85_length;
      get_pending_length = z85_ctx_get_pending_length;
      isubase = isuz85;
      base_encode = z85_encode;
      base_decode_ctx_init = z85_decode_ctx_init;
      base_decode_ctx = z85_decode_ctx;
      ಭಙ್ಗ;

    ಪಕ್ಷ BASE58_OPTION:
      base_length = base58_length;
      isubase = isubase58;
      base_encode_ctx_init = base58_encode_ctx_init;
      base_encode_ctx = base58_encode_ctx;
      base_encode_ctx_finalize = base58_encode_ctx_finalize;
      base_decode_ctx_init = base58_decode_ctx_init;
      base_decode_ctx = base58_decode_ctx;
      base_decode_ctx_finalize = base58_decode_ctx_finalize;
      ಭಙ್ಗ;

    ಸ್ವಭಾವ:
      error (0, 0, _("missing encoding type"));
      usage (EXIT_FAILURE);
    }
#endif

  ಯದಿ (argc - optind > 1)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }

  ಯದಿ (optind < argc)
    infile = argv[optind];
  ಅನ್ಯಥಾ
    infile = "-";

  ಯದಿ (streq (infile, "-"))
    {
      xset_ದ್ವಿಮಾನ_mode (STDIN_FILENO, O_ದ್ವಿಮಾನ);
      ಒಳಹರಿವು_fh = stdin;
    }
  ಅನ್ಯಥಾ
    {
      ಒಳಹರಿವು_fh = fopen (infile, "rb");
      ಯದಿ (ಒಳಹರಿವು_fh == ರಿಕ್ತ)
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
    }

  fadvise (ಒಳಹರಿವು_fh, FADVISE_SEQUENTIAL);

  ಯದಿ (decode)
    do_decode (ಒಳಹರಿವು_fh, infile, stdout, ignore_garbage);
  ಅನ್ಯಥಾ
    do_encode (ಒಳಹರಿವು_fh, infile, stdout, wrap_column);
}
