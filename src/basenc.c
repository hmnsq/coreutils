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

#شامل <config.h>

#شامل <stdio.h>
#شامل <getopt.h>
#شامل <sys/types.h>
#شامل <gmp.h>

#شامل "system.h"
#شامل "assure.h"
#شامل "c-ctype.h"
#شامل "fadvise.h"
#شامل "quote.h"
#شامل "xstrtol.h"
#شامل "xbinary-io.h"

#اگر بنیاد_TYPE == 42
# define AUTHORS \
  proper_name ("Simon Josefsson"), \
  proper_name ("Assaf Gordon")
#ورنہ
# define AUTHORS proper_name ("Simon Josefsson")
#اگر_ختم

#اگر بنیاد_TYPE == 32
# include "base32.h"
# define پروگرام_NAME "base32"
#وگرنہ_اگر بنیاد_TYPE == 64
# include "base64.h"
# define پروگرام_NAME "base64"
#وگرنہ_اگر بنیاد_TYPE == 42
# include "base32.h"
# include "base64.h"
# include "assure.h"
# define پروگرام_NAME "basenc"
#ورنہ
# error missing/invalid بنیاد_TYPE definition
#اگر_ختم



#اگر بنیاد_TYPE == 42
شمار
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
#اگر_ختم

ساکن ساخت option ثابت long_options[] =
{
  {"decode", no_دلیل, تہی, 'd'},
  {"wrap", required_دلیل, تہی, 'w'},
  {"ignore-garbage", no_دلیل, تہی, 'i'},
#اگر بنیاد_TYPE == 42
  {"base64",    no_دلیل, تہی, BASE64_OPTION},
  {"base64url", no_دلیل, تہی, BASE64URL_OPTION},
  {"base58",    no_دلیل, تہی, BASE58_OPTION},
  {"base32",    no_دلیل, تہی, BASE32_OPTION},
  {"base32hex", no_دلیل, تہی, BASE32HEX_OPTION},
  {"base16",    no_دلیل, تہی, BASE16_OPTION},
  {"base2msbf", no_دلیل, تہی, BASE2MSBF_OPTION},
  {"base2lsbf", no_دلیل, تہی, BASE2LSBF_OPTION},
  {"z85",       no_دلیل, تہی, Z85_OPTION},
#اگر_ختم
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0}
};

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("\
Usage: %s [OPTION]... [FILE]\n\
"), پروگرام_name);

#اگر بنیاد_TYPE == 42
      fputs (_("\
basenc encode or decode FILE, or standard داخلہ, مقصد standard output.\n\
"), stdout);
#ورنہ
      لکھو (_("\
بنیاد%d encode or decode FILE, or standard داخلہ, مقصد standard output.\n\
"), بنیاد_TYPE);
#اگر_ختم

      emit_stdin_note ();
      emit_mandatory_arg_note ();
#اگر بنیاد_TYPE == 42
      oputs (_("\
      --base64\n\
         same as base64(1) پروگرام (RFC4648 section 4)\n\
"));
      oputs (_("\
      --base64url\n\
         file- and url-محفوظ_2 base64 (RFC4648 section 5)\n\
"));
      oputs (_("\
      --base58\n\
         visually unambiguous base58 encoding\n\
"));
      oputs (_("\
      --base32\n\
         same as base32(1) پروگرام (RFC4648 section 6)\n\
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
         ثنائی_ہندسہ string with most significant ثنائی_ہندسہ (msb) پہلا\n\
"));
      oputs (_("\
      --base2lsbf\n\
         ثنائی_ہندسہ string with کم_سے_کم significant ثنائی_ہندسہ (lsb) پہلا\n\
"));
#اگر_ختم
      oputs (_("\
  -d, --decode\n\
         decode ڈیٹا\n\
"));
      oputs (_("\
  -i, --ignore-garbage\n\
         when decoding, ignore non-alphabet characters\n\
"));
      oputs (_("\
  -w, --wrap=ستون_تعداد\n\
         wrap encoded سطریں after ستون_تعداد character (طےشدہ 76).\n\
         Use 0 مقصد disable سطر wrapping\n\
"));
#اگر بنیاد_TYPE == 42
      oputs (_("\
      --z85\n\
         ascii85-like encoding (ZeroMQ spec:32/Z85);\n\
         when encoding, داخلہ لمبائی must be a مضرب کا 4;\n\
         when decoding, داخلہ لمبائی must be a مضرب کا 5\n\
"));
#اگر_ختم
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
#اگر بنیاد_TYPE == 42
      fputs (_("\
\n\
When decoding, the داخلہ may contain newlines اندر addition مقصد the بائٹیں کا\n\
the formal alphabet.  Use --ignore-garbage مقصد attempt مقصد recover\n\
from any other non-alphabet بائٹیں اندر the encoded دھارا.\n\
"), stdout);
#ورنہ
      لکھو (_("\
\n\
The ڈیٹا are encoded as described برائے the %s alphabet اندر RFC 4648.\n\
When decoding, the داخلہ may contain newlines اندر addition مقصد the بائٹیں کا\n\
the formal %s alphabet.  Use --ignore-garbage مقصد attempt مقصد recover\n\
from any other non-alphabet بائٹیں اندر the encoded دھارا.\n"),
              پروگرام_NAME, پروگرام_NAME);
#اگر_ختم
      emit_ancillary_info (پروگرام_NAME);
    }

  باہر_نکلو (حالت_2);
}

#اگر بنیاد_TYPE != 64
ساکن صحیح
base32_required_padding (صحیح len)
{
  صحیح partial = len % 8;
  واپس partial ? 8 - partial : 0;
}
#اگر_ختم

#اگر بنیاد_TYPE != 32
ساکن صحیح
base64_required_padding (صحیح len)
{
  صحیح partial = len % 4;
  واپس partial ? 4 - partial : 0;
}
#اگر_ختم

#اگر بنیاد_TYPE == 42
ساکن صحیح
no_required_padding (MAYBE_غیر_مستعمل صحیح len)
{
  واپس 0;
}
#اگر_ختم

#تعریف_کرو ENC_BLOCKSIZE (1024 * 3 * 10)

#اگر بنیاد_TYPE == 32
# define بنیاد_لمبائی BASE32_لمبائی
# define REQUIRED_PADDING base32_required_padding
/* Note that increasing this may decrease performance if --ignore-garbage
   is used, because of the memmove operation below.  */
# define DEC_BLOCKSIZE (1024 * 5)

/* Ensure that BLOCKSIZE is a multiple of 5 and 8.  */
static_assert (ENC_BLOCKSIZE % 40 == 0); /* Padding chars only on last block. */
static_assert (DEC_BLOCKSIZE % 40 == 0); /* Complete encoded blocks are used. */

# define بنیاد_encode base32_encode
# define بنیاد_decode_context base32_decode_context
# define بنیاد_decode_ctx_init base32_decode_ctx_init
# define بنیاد_decode_ctx base32_decode_ctx
# define بنیاد_decode_ctx_finalize decode_ctx_finalize
# define isubase isubase32
#وگرنہ_اگر بنیاد_TYPE == 64
# define بنیاد_لمبائی BASE64_لمبائی
# define REQUIRED_PADDING base64_required_padding
/* Note that increasing this may decrease performance if --ignore-garbage
   is used, because of the memmove operation below.  */
# define DEC_BLOCKSIZE (1024 * 3)

/* Ensure that BLOCKSIZE is a multiple of 3 and 4.  */
static_assert (ENC_BLOCKSIZE % 12 == 0); /* Padding chars only on last block. */
static_assert (DEC_BLOCKSIZE % 12 == 0); /* Complete encoded blocks are used. */

# define بنیاد_encode base64_encode
# define بنیاد_decode_context base64_decode_context
# define بنیاد_decode_ctx_init base64_decode_ctx_init
# define بنیاد_decode_ctx base64_decode_ctx
# define بنیاد_decode_ctx_finalize decode_ctx_finalize
# define isubase isubase64
#وگرنہ_اگر بنیاد_TYPE == 42


# define بنیاد_لمبائی بنیاد_لمبائی_2
# define REQUIRED_PADDING required_padding

/* Note that increasing this may decrease performance if --ignore-garbage
   is used, because of the memmove operation below.  */
# define DEC_BLOCKSIZE (4200)
static_assert (DEC_BLOCKSIZE % 40 == 0); /* complete encoded blocks for base32*/
static_assert (DEC_BLOCKSIZE % 12 == 0); /* complete encoded blocks for base64*/

ساکن idx_t (*بنیاد_لمبائی_2) (idx_t len);
ساکن صحیح (*required_padding) (صحیح i);
ساکن bool (*isubase) (غیرعلامتی حرف ch);
ساکن خالی (*بنیاد_encode) (حرف ثابت *محدود اندر, idx_t inlen,
                            حرف *محدود برآمد, idx_t outlen);

ساخت base16_decode_context
{
  /* Either a 4-bit nibble, or negative if we have no nibble.  */
  علامتی حرف nibble;
};

ساخت z85_decode_context
{
  صحیح i;
  غیرعلامتی حرف octets[5];
};

ساخت base58_context
{
  idx_t سائز;
  idx_t استعداد;
  غیرعلامتی حرف *buf COUNTED_BY (استعداد);
};

ساخت base2_decode_context
{
  غیرعلامتی حرف octet;
  صحیح ثنائی_ہندسہ_pos;
};

ساخت بنیاد_decode_context
{
  اتحاد {
    ساخت base64_decode_context base64;
    ساخت base32_decode_context base32;
    ساخت base16_decode_context base16;
    ساخت base2_decode_context base2;
    ساخت z85_decode_context z85;
    ساخت base58_context base58;
  } ctx;
  حرف *inbuf;
  idx_t bufsize;
};
ساکن خالی (*بنیاد_decode_ctx_init) (ساخت بنیاد_decode_context *ctx);
ساکن bool (*بنیاد_decode_ctx) (ساخت بنیاد_decode_context *ctx,
                                حرف ثابت *محدود اندر, idx_t inlen,
                                حرف *محدود برآمد, idx_t *outlen);
ساکن bool (*بنیاد_decode_ctx_finalize) (ساخت بنیاد_decode_context *ctx,
                                         حرف *محدود *برآمد, idx_t *outlen);

ساخت بنیاد_encode_context
{
  اتحاد {
    ساخت base58_context base58;
  } ctx;
};

ساکن خالی (*بنیاد_encode_ctx_init) (ساخت بنیاد_encode_context *ctx);
ساکن bool (*بنیاد_encode_ctx) (ساخت بنیاد_encode_context *ctx,
                                حرف ثابت *محدود اندر, idx_t inlen,
                                حرف *محدود برآمد, idx_t *outlen);
ساکن bool (*بنیاد_encode_ctx_finalize) (ساخت بنیاد_encode_context *ctx,
                                         حرف *محدود *برآمد, idx_t *outlen);

ساکن bool
no_padding (MAYBE_غیر_مستعمل ساخت بنیاد_decode_context *ctx)
{
  واپس false;
}

ساکن صحیح
no_pending_لمبائی (MAYBE_غیر_مستعمل ساخت بنیاد_decode_context *ctx)
{
  واپس 0;
}
#اگر_ختم

#اگر بنیاد_TYPE == 42
ساکن bool (*has_padding) (ساخت بنیاد_decode_context *ctx);
ساکن صحیح (*get_pending_لمبائی) (ساخت بنیاد_decode_context *ctx);

ساکن bool
base64_ctx_has_padding (ساخت بنیاد_decode_context *ctx)
{
  واپس ctx->ctx.base64.i && ctx->ctx.base64.buf[ctx->ctx.base64.i - 1] == '=';
}

ساکن bool
base32_ctx_has_padding (ساخت بنیاد_decode_context *ctx)
{
  واپس ctx->ctx.base32.i && ctx->ctx.base32.buf[ctx->ctx.base32.i - 1] == '=';
}

ساکن صحیح
base64_ctx_get_pending_لمبائی (ساخت بنیاد_decode_context *ctx)
{
  واپس ctx->ctx.base64.i;
}

ساکن صحیح
base32_ctx_get_pending_لمبائی (ساخت بنیاد_decode_context *ctx)
{
  واپس ctx->ctx.base32.i;
}

ساکن صحیح
base16_ctx_get_pending_لمبائی (MAYBE_غیر_مستعمل ساخت بنیاد_decode_context *ctx)
{
  واپس 1; /* Always check nibble state.  */
}

ساکن صحیح
z85_ctx_get_pending_لمبائی (ساخت بنیاد_decode_context *ctx)
{
  واپس ctx->ctx.z85.i;
}

ساکن صحیح
base2_ctx_get_pending_لمبائی (ساخت بنیاد_decode_context *ctx)
{
  واپس ctx->ctx.base2.ثنائی_ہندسہ_pos;
}
#ورنہ
ساکن bool
has_padding (ساخت بنیاد_decode_context *ctx)
{
  واپس ctx->i && ctx->buf[ctx->i - 1] == '=';
}

ساکن صحیح
get_pending_لمبائی (ساخت بنیاد_decode_context *ctx)
{
  واپس ctx->i;
}
#اگر_ختم


/* Process any pending data in CTX, while auto padding if appropriate.
   Return TRUE on success, FALSE on failure.  */

ساکن bool
decode_ctx_finalize (ساخت بنیاد_decode_context *ctx,
                     حرف *محدود *برآمد, idx_t *outlen)
{
  اگر (get_pending_لمبائی (ctx) == 0)
    {
      *outlen = 0;
      واپس true;
    }

  /* Auto-pad input and flush the context */
  حرف padbuf[8] ATTRIBUTE_NONSTRING = "========";
  idx_t pending_len = get_pending_لمبائی (ctx);
  idx_t auto_padding = REQUIRED_PADDING (pending_len);
  idx_t n = *outlen;
  bool نتیجہ;

  اگر (auto_padding && ! has_padding (ctx))
    {
      affirm (auto_padding <= حجم (padbuf));
      نتیجہ = بنیاد_decode_ctx (ctx, padbuf, auto_padding, *برآمد, &n);
    }
  ورنہ
    {
      نتیجہ = بنیاد_decode_ctx (ctx, "", 0, *برآمد, &n);
    }

  *outlen = n;
  واپس نتیجہ;
}

#اگر بنیاد_TYPE == 42

ساکن idx_t
base64_لمبائی_wrapper (idx_t len)
{
  واپس BASE64_لمبائی (len);
}

ساکن خالی
base64_decode_ctx_init_wrapper (ساخت بنیاد_decode_context *ctx)
{
  base64_decode_ctx_init (&ctx->ctx.base64);
}

ساکن bool
base64_decode_ctx_wrapper (ساخت بنیاد_decode_context *ctx,
                           حرف ثابت *محدود اندر, idx_t inlen,
                           حرف *محدود برآمد, idx_t *outlen)
{
  واپس base64_decode_ctx (&ctx->ctx.base64, اندر, inlen, برآمد, outlen);
}

ساکن خالی
init_inbuf (ساخت بنیاد_decode_context *ctx)
{
  ctx->bufsize = DEC_BLOCKSIZE;
  ctx->inbuf = xcharalloc (ctx->bufsize);
}

ساکن خالی
prepare_inbuf (ساخت بنیاد_decode_context *ctx, idx_t inlen)
{
  اگر (ctx->bufsize < inlen)
    ctx->inbuf = xpalloc (ctx->inbuf, &ctx->bufsize,
                          inlen - ctx->bufsize, -1, حجم *ctx->inbuf);
}


ساکن خالی
base64url_encode (حرف ثابت *محدود اندر, idx_t inlen,
                  حرف *محدود برآمد, idx_t outlen)
{
  base64_encode (اندر, inlen, برآمد, outlen);
  /* translate 62nd and 63rd characters */
  حرف *p = برآمد;
  جب (outlen--)
    {
      اگر (*p == '+')
        *p = '-';
      ورنہ اگر (*p == '/')
        *p = '_';
      ++p;
    }
}

ساکن bool
isubase64url (غیرعلامتی حرف ch)
{
  واپس (ch == '-' || ch == '_'
          || (ch != '+' && ch != '/' && isubase64 (ch)));
}

ساکن خالی
base64url_decode_ctx_init_wrapper (ساخت بنیاد_decode_context *ctx)
{
  base64_decode_ctx_init (&ctx->ctx.base64);
  init_inbuf (ctx);
}


ساکن bool
base64url_decode_ctx_wrapper (ساخت بنیاد_decode_context *ctx,
                              حرف ثابت *محدود اندر, idx_t inlen,
                              حرف *محدود برآمد, idx_t *outlen)
{
  prepare_inbuf (ctx, inlen);
  حافظہ_نقل (ctx->inbuf, اندر, inlen);

  /* translate 62nd and 63rd characters */
  idx_t i = inlen;
  حرف *p = ctx->inbuf;
  جب (i--)
    {
      اگر (*p == '+' || *p == '/')
        {
          *outlen = 0;
          واپس false; /* reject base64 input */
        }
      ورنہ اگر (*p == '-')
        *p = '+';
      ورنہ اگر (*p == '_')
        *p = '/';
      ++p;
    }

  واپس base64_decode_ctx (&ctx->ctx.base64, ctx->inbuf, inlen,
                            برآمد, outlen);
}



ساکن idx_t
base32_لمبائی_wrapper (idx_t len)
{
  واپس BASE32_لمبائی (len);
}

ساکن خالی
base32_decode_ctx_init_wrapper (ساخت بنیاد_decode_context *ctx)
{
  base32_decode_ctx_init (&ctx->ctx.base32);
}

ساکن bool
base32_decode_ctx_wrapper (ساخت بنیاد_decode_context *ctx,
                           حرف ثابت *محدود اندر, idx_t inlen,
                           حرف *محدود برآمد, idx_t *outlen)
{
  واپس base32_decode_ctx (&ctx->ctx.base32, اندر, inlen, برآمد, outlen);
}

/* ABCDEFGHIJKLMNOPQRSTUVWXYZ234567
مقصد
   0123456789ABCDEFGHIJKLMNOPQRSTUV */
ساکن ثابت حرف base32_norm_مقصد_hex[32 + 9] = {
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
مقصد
   ABCDEFGHIJKLMNOPQRSTUVWXYZ234567 */
ساکن ثابت حرف base32_hex_مقصد_norm[32 + 9] = {
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


اندرونی ساکن bool
isubase32hex (غیرعلامتی حرف ch)
{
  واپس ('0' <= ch && ch <= '9') || ('A' <= ch && ch <= 'V');
}


ساکن خالی
base32hex_encode (حرف ثابت *محدود اندر, idx_t inlen,
                  حرف *محدود برآمد, idx_t outlen)
{
  base32_encode (اندر, inlen, برآمد, outlen);

  برائے (حرف *p = برآمد; outlen--; p++)
    {
      affirm (0x32 <= *p && *p <= 0x5a);          /* LCOV_EXCL_LINE */
      *p = base32_norm_مقصد_hex[*p - 0x32];
    }
}


ساکن خالی
base32hex_decode_ctx_init_wrapper (ساخت بنیاد_decode_context *ctx)
{
  base32_decode_ctx_init (&ctx->ctx.base32);
  init_inbuf (ctx);
}


ساکن bool
base32hex_decode_ctx_wrapper (ساخت بنیاد_decode_context *ctx,
                              حرف ثابت *محدود اندر, idx_t inlen,
                              حرف *محدود برآمد, idx_t *outlen)
{
  prepare_inbuf (ctx, inlen);

  idx_t i = inlen;
  حرف *p = ctx->inbuf;
  جب (i--)
    {
      اگر (isubase32hex (*اندر))
        *p = base32_hex_مقصد_norm[*اندر - 0x30];
      ورنہ
        *p = *اندر;
      ++p;
      ++اندر;
    }

  واپس base32_decode_ctx (&ctx->ctx.base32, ctx->inbuf, inlen,
                            برآمد, outlen);
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

ساکن علامتی حرف ثابت base16_مقصد_int[256] = {
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

ساکن bool
isubase16 (غیرعلامتی حرف ch)
{
  واپس ch < حجم base16_مقصد_int && 0 <= base16_مقصد_int[ch];
}

ساکن idx_t
base16_لمبائی (idx_t len)
{
  واپس len * 2;
}


ساکن خالی
base16_encode (حرف ثابت *محدود اندر, idx_t inlen,
               حرف *محدود برآمد, idx_t outlen)
{
  ساکن ثابت حرف base16[16] ATTRIBUTE_NONSTRING = "0123456789ABCDEF";

  جب (inlen && outlen)
    {
      غیرعلامتی حرف c = *اندر;
      *برآمد++ = base16[c >> 4];
      *برآمد++ = base16[c & 0x0F];
      ++اندر;
      inlen--;
      outlen -= 2;
    }
}


ساکن خالی
base16_decode_ctx_init (ساخت بنیاد_decode_context *ctx)
{
  init_inbuf (ctx);
  ctx->ctx.base16.nibble = -1;
}


ساکن bool
base16_decode_ctx (ساخت بنیاد_decode_context *ctx,
                   حرف ثابت *محدود اندر, idx_t inlen,
                   حرف *محدود برآمد, idx_t *outlen)
{
  bool ignore_سطریں = true;  /* for now, always ignore them */
  حرف *out0 = برآمد;
  علامتی حرف nibble = ctx->ctx.base16.nibble;

  /* inlen==0 is request to flush output.
     if there is a dangling high nibble - we are missing the low nibble,
     so return false - indicating an invalid input.  */
  اگر (inlen == 0)
    {
      *outlen = 0;
      واپس nibble < 0;
    }

  جب (inlen--)
    {
      غیرعلامتی حرف c = *اندر++;
      اگر (ignore_سطریں && c == '\n')
        جاری;

      اگر (حجم base16_مقصد_int <= c || base16_مقصد_int[c] < 0)
        {
          *outlen = برآمد - out0;
          واپس false; /* garbage - return false */
        }

      اگر (nibble < 0)
        nibble = base16_مقصد_int[c];
      ورنہ
        {
          /* have both nibbles, write octet */
          *برآمد++ = (nibble << 4) + base16_مقصد_int[c];
          nibble = -1;
        }
    }

  ctx->ctx.base16.nibble = nibble;
  *outlen = برآمد - out0;
  واپس true;
}



ATTRIBUTE_خالص
ساکن idx_t
z85_لمبائی (idx_t len)
{
  /* Z85 does not allow padding, so no need to round to highest integer.  */
  idx_t z85_len = (len * 5) / 4;
  affirm (0 <= z85_len);
  واپس z85_len;
}

ساکن bool
isuz85 (غیرعلامتی حرف ch)
{
  واپس c_isalnum (ch) || strchr (".-:+=^!/*?&<>()[]{}@%$#", ch) != تہی;
}

ساکن حرف ثابت z85_encoding[85] ATTRIBUTE_NONSTRING =
  "0123456789"
  "abcdefghijklmnopqrstuvwxyz"
  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
  ".-:+=^!/*?&<>()[]{}@%$#";

ساکن خالی
z85_encode (حرف ثابت *محدود اندر, idx_t inlen,
            حرف *محدود برآمد, idx_t outlen)
{
  صحیح i = 0;
  غیرعلامتی حرف quad[4];
  idx_t outidx = 0;

  جب (true)
    {
      اگر (inlen == 0)
        {
          /* no more input, exactly on 4 octet boundary. */
          اگر (i == 0)
            واپس;

          /* currently, there's no way to return an error in encoding.  */
          error (EXIT_FAILURE, 0,
                 _("invalid input (length must be multiple of 4 characters)"));
        }
      ورنہ
        {
          quad[i++] = *اندر++;
          --inlen;
        }

      /* Got a quad, encode it */
      اگر (i == 4)
        {
          int_fast64_t val = quad[0];
          val = (val << 24) + (quad[1] << 16) + (quad[2] << 8) + quad[3];

          برائے (صحیح j = 4; j >= 0; --j)
            {
              صحیح c = val % 85;
              val /= 85;

              /* NOTE: if there is padding (which is trimmed by z85
                 before outputting the result), the output buffer 'out'
                 might not include enough allocated bytes for the padding,
                 so don't store them. */
              اگر (outidx + j < outlen)
                برآمد[j] = z85_encoding[c];
            }
          برآمد += 5;
          outidx += 5;
          i = 0;
        }
    }
}

ساکن خالی
z85_decode_ctx_init (ساخت بنیاد_decode_context *ctx)
{
  init_inbuf (ctx);
  ctx->ctx.z85.i = 0;
}


# define Z85_LO_CTX_مقصد_32_ثنائی_ہندسہ_VAL(ctx) \
  (((ctx)->ctx.z85.octets[1] * 85 * 85 * 85) +      \
   ((ctx)->ctx.z85.octets[2] * 85 * 85) +	    \
   ((ctx)->ctx.z85.octets[3] * 85) +		    \
   ((ctx)->ctx.z85.octets[4]))


# define Z85_HI_CTX_مقصد_32_ثنائی_ہندسہ_VAL(ctx) \
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
ساکن علامتی حرف ثابت z85_decoding[93] = {
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

ساکن bool
z85_decode_ctx (ساخت بنیاد_decode_context *ctx,
                حرف ثابت *محدود اندر, idx_t inlen,
                حرف *محدود برآمد, idx_t *outlen)
{
  bool ignore_سطریں = true;  /* for now, always ignore them */

  *outlen = 0;

  /* inlen==0 is request to flush output.
     if there are dangling values - we are missing entries,
     so return false - indicating an invalid input.  */
  اگر (inlen == 0)
    {
      اگر (ctx->ctx.z85.i > 0)
        {
          /* Z85 variant does not allow padding - input must
             be a multiple of 5 - so return error.  */
          واپس false;
        }
      واپس true;
    }

  جب (inlen--)
    {
      اگر (ignore_سطریں && *اندر == '\n')
        {
          ++اندر;
          جاری;
        }

      /* z85 decoding */
      غیرعلامتی حرف c = *اندر;

      اگر (c >= 33 && c <= 125)
        {
          علامتی حرف ch = z85_decoding[c - 33];
          اگر (ch < 0)
            واپس false; /* garbage - return false */
          c = ch;
        }
      ورنہ
        واپس false; /* garbage - return false */

      ++اندر;

      ctx->ctx.z85.octets[ctx->ctx.z85.i++] = c;
      اگر (ctx->ctx.z85.i == 5)
        {
          /* decode the lowest 4 octets, then check for overflows.  */
          int_fast64_t val = Z85_LO_CTX_مقصد_32_ثنائی_ہندسہ_VAL (ctx);

          /* The Z85 spec and the reference implementation say nothing
             about overflows. To be on the safe side, reject them.  */

          val += Z85_HI_CTX_مقصد_32_ثنائی_ہندسہ_VAL (ctx);
          اگر ((val >> 24) & ~0xFF)
            واپس false;

          *برآمد++ = val >> 24;
          *برآمد++ = (val >> 16) & 0xFF;
          *برآمد++ = (val >> 8) & 0xFF;
          *برآمد++ = val & 0xFF;

          *outlen += 4;

          ctx->ctx.z85.i = 0;
        }
    }
  واپس true;
}


اندرونی ساکن bool
isubase2 (غیرعلامتی حرف ch)
{
  واپس ch == '0' || ch == '1';
}

ساکن idx_t
base2_لمبائی (idx_t len)
{
  واپس len * 8;
}


اندرونی ساکن خالی
base2msbf_encode (حرف ثابت *محدود اندر, idx_t inlen,
                  حرف *محدود برآمد, idx_t outlen)
{
  جب (inlen && outlen)
    {
      غیرعلامتی حرف c = *اندر;
      برائے (صحیح i = 0; i < 8; i++)
        {
          *برآمد++ = c & 0x80 ? '1' : '0';
          c <<= 1;
        }
      inlen--;
      outlen -= 8;
      ++اندر;
    }
}

اندرونی ساکن خالی
base2lsbf_encode (حرف ثابت *محدود اندر, idx_t inlen,
                  حرف *محدود برآمد, idx_t outlen)
{
  جب (inlen && outlen)
    {
      غیرعلامتی حرف c = *اندر;
      برائے (صحیح i = 0; i < 8; i++)
        {
          *برآمد++ = c & 0x01 ? '1' : '0';
          c >>= 1;
        }
      inlen--;
      outlen -= 8;
      ++اندر;
    }
}


ساکن خالی
base2_decode_ctx_init (ساخت بنیاد_decode_context *ctx)
{
  init_inbuf (ctx);
  ctx->ctx.base2.octet = 0;
  ctx->ctx.base2.ثنائی_ہندسہ_pos = 0;
}


ساکن bool
base2lsbf_decode_ctx (ساخت بنیاد_decode_context *ctx,
                      حرف ثابت *محدود اندر, idx_t inlen,
                      حرف *محدود برآمد, idx_t *outlen)
{
  bool ignore_سطریں = true;  /* for now, always ignore them */

  *outlen = 0;

  /* inlen==0 is request to flush output.
     if there is a dangling bit - we are missing some bits,
     so return false - indicating an invalid input.  */
  اگر (inlen == 0)
    واپس ctx->ctx.base2.ثنائی_ہندسہ_pos == 0;

  جب (inlen--)
    {
      اگر (ignore_سطریں && *اندر == '\n')
        {
          ++اندر;
          جاری;
        }

      اگر (!isubase2 (*اندر))
        واپس false;

      bool ثنائی_ہندسہ = (*اندر == '1');
      ctx->ctx.base2.octet |= ثنائی_ہندسہ << ctx->ctx.base2.ثنائی_ہندسہ_pos;
      ++ctx->ctx.base2.ثنائی_ہندسہ_pos;

      اگر (ctx->ctx.base2.ثنائی_ہندسہ_pos == 8)
        {
          *برآمد++ = ctx->ctx.base2.octet;
          ctx->ctx.base2.octet = 0;
          ++*outlen;
          ctx->ctx.base2.ثنائی_ہندسہ_pos = 0;
        }

      ++اندر;
    }

  واپس true;
}

ساکن bool
base2msbf_decode_ctx (ساخت بنیاد_decode_context *ctx,
                      حرف ثابت *محدود اندر, idx_t inlen,
                      حرف *محدود برآمد, idx_t *outlen)
{
  bool ignore_سطریں = true;  /* for now, always ignore them */

  *outlen = 0;

  /* inlen==0 is request to flush output.
     if there is a dangling bit - we are missing some bits,
     so return false - indicating an invalid input.  */
  اگر (inlen == 0)
    واپس ctx->ctx.base2.ثنائی_ہندسہ_pos == 0;

  جب (inlen--)
    {
      اگر (ignore_سطریں && *اندر == '\n')
        {
          ++اندر;
          جاری;
        }

      اگر (!isubase2 (*اندر))
        واپس false;

      bool ثنائی_ہندسہ = (*اندر == '1');
      اگر (ctx->ctx.base2.ثنائی_ہندسہ_pos == 0)
        ctx->ctx.base2.ثنائی_ہندسہ_pos = 8;
      --ctx->ctx.base2.ثنائی_ہندسہ_pos;
      ctx->ctx.base2.octet |= ثنائی_ہندسہ << ctx->ctx.base2.ثنائی_ہندسہ_pos;

      اگر (ctx->ctx.base2.ثنائی_ہندسہ_pos == 0)
        {
          *برآمد++ = ctx->ctx.base2.octet;
          ctx->ctx.base2.octet = 0;
          ++*outlen;
        }

      ++اندر;
    }

  واپس true;
}

/* Map from GMP (up to base 62):
   "0123456789 ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuv";
   to base 58:
   "123456789A BCDEFGHJKLMNPQRSTUVWXYZabc defghijkmnopqrstuvwxyz";  */
ساکن علامتی حرف ثابت gmp_مقصد_base58[256] = {
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

ساکن علامتی حرف ثابت base58_مقصد_gmp[256] = {
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

ساکن bool
isubase58 (غیرعلامتی حرف ch)
{
  واپس ch < حجم base58_مقصد_gmp && 0 <= base58_مقصد_gmp[ch];
}


ATTRIBUTE_خالص
ساکن idx_t
base58_لمبائی (idx_t len)
{
  /* Base58 output length is approximately log(256)/log(58),
     which is approximately len * 138 / 100,
     which is at most ((len + 100 - 1) / 100) * 138
     +1 to ensure we've enough place for NUL  */
  idx_t base58_len = ((len + 99) / 100) * 138 + 1;
  affirm (0 < base58_len);
  واپس base58_len;
}


ساکن خالی
base58_encode_ctx_init (ساخت بنیاد_encode_context *ctx)
{
  ctx->ctx.base58.buf = تہی;
  ctx->ctx.base58.سائز = 0;
  ctx->ctx.base58.استعداد = 0;
}


ساکن bool
base58_encode_ctx (ساخت بنیاد_encode_context *ctx,
                   حرف ثابت *محدود اندر, idx_t inlen,
                   MAYBE_غیر_مستعمل حرف *محدود برآمد, idx_t *outlen)
{
  *outlen = 0;   /* Only accumulate input in this function.  */

  اگر (inlen == 0)
    واپس true;

  idx_t free_space = ctx->ctx.base58.استعداد - ctx->ctx.base58.سائز;
  اگر (free_space < inlen)
    {
      ctx->ctx.base58.buf = xpalloc (ctx->ctx.base58.buf,
                                     &ctx->ctx.base58.استعداد,
                                     inlen - free_space,
                                     -1, حجم *ctx->ctx.base58.buf);
    }

  حافظہ_نقل (ctx->ctx.base58.buf + ctx->ctx.base58.سائز, اندر, inlen);
  ctx->ctx.base58.سائز += inlen;

  واپس true;
}

ساکن خالی
base58_encode (حرف ثابت *ڈیٹا, size_t ڈیٹا_len,
               حرف *برآمد, idx_t *outlen)
{
  affirm (بنیاد_لمبائی_2 (ڈیٹا_len) <= *outlen);

  size_t zeros = 0;
  جب (zeros < ڈیٹا_len && ڈیٹا[zeros] == 0)
    zeros++;

  memset (برآمد, '1', zeros);
  حرف *p = برآمد + zeros;

  /* Use GMP to convert from base 256 to base 58.  */
  mpz_t عدد;
  mpz_init (عدد);
  اگر (ڈیٹا_len - zeros)
    {
      mpz_import (عدد, ڈیٹا_len - zeros, 1, 1, 0, 0, ڈیٹا + zeros);
      برائے (p = mpz_get_str (p, 58, عدد); *p; p++)
        *p = gmp_مقصد_base58[مقصد_uchar (*p)];
    }
  mpz_clear (عدد);

  *outlen = p - برآمد;
}


ساکن bool
base58_encode_ctx_finalize (ساخت بنیاد_encode_context *ctx,
                            حرف *محدود *برآمد, idx_t *outlen)
{
  /* Ensure output buffer is large enough.  */
  idx_t max_outlen = بنیاد_لمبائی_2 (ctx->ctx.base58.سائز);
  اگر (max_outlen > *outlen)
    {
      *برآمد = xrealloc (*برآمد, max_outlen);
      *outlen = max_outlen;
    }

  base58_encode ((حرف *)ctx->ctx.base58.buf, ctx->ctx.base58.سائز,
                 *برآمد, outlen);

  آزاد (ctx->ctx.base58.buf);
  ctx->ctx.base58.buf = تہی;
  ctx->ctx.base58.سائز = 0;
  ctx->ctx.base58.استعداد = 0;

  واپس true;
}


ساکن خالی
base58_decode_ctx_init (ساخت بنیاد_decode_context *ctx)
{
  ctx->ctx.base58.سائز = 0;
  ctx->ctx.base58.استعداد = 0;
  ctx->ctx.base58.buf = تہی;
}

ساکن bool
base58_decode_ctx (ساخت بنیاد_decode_context *ctx,
                   حرف ثابت *محدود اندر, idx_t inlen,
                   MAYBE_غیر_مستعمل حرف *محدود برآمد, idx_t *outlen)
{
  bool ignore_سطریں = true;  /* for now, always ignore them */

  *outlen = 0;   /* Only accumulate input in this function.  */

  اگر (inlen == 0)
    واپس true;

  idx_t free_space = ctx->ctx.base58.استعداد - ctx->ctx.base58.سائز;
  free_space -= 1;  /* Ensure we leave space for NUL (for mpz_set_str).  */
  اگر (free_space < inlen)
    {
      ctx->ctx.base58.buf = xpalloc (ctx->ctx.base58.buf,
                                     &ctx->ctx.base58.استعداد,
                                     inlen - free_space,
                                     -1, حجم *ctx->ctx.base58.buf);
    }

  /* Accumulate all valid input characters in our buffer.
     Note we don't rely on mpz_set_str() for validation
     as that allows (skips) all whitespace.  */
  برائے (idx_t i = 0; i < inlen; i++)
    {
      غیرعلامتی حرف c = اندر[i];

      اگر (ignore_سطریں && c == '\n')
        جاری;

      اگر (!isubase58 (c))
        واپس false;

      ctx->ctx.base58.buf[ctx->ctx.base58.سائز++] = base58_مقصد_gmp[مقصد_uchar (c)];
    }

  واپس true;
}


ساکن bool
base58_decode (حرف ثابت *ڈیٹا, size_t ڈیٹا_len,
               حرف *محدود برآمد, idx_t *outlen)
{
  affirm (ڈیٹا_len <= *outlen);

  size_t ones = 0;
  جب (ones < ڈیٹا_len && ڈیٹا[ones] == base58_مقصد_gmp['1'])
    ones++;

  memset (برآمد, 0, ones);

  /* Use GMP to convert from base 58 to base 256.  */
  mpz_t عدد;
  mpz_init (عدد);

  اگر ((ڈیٹا_len - ones) && mpz_سیٹ_str (عدد, ڈیٹا + ones, 58) != 0)
    {
      mpz_clear (عدد);
      *outlen = 0;
      واپس false;
    }

  size_t exported_سائز = 0;
  اگر (ڈیٹا_len - ones)
    {
      size_t ثنائی_سائز = (mpz_sizeinbase (عدد, 2) + 7) / 8;
      affirm (*outlen - ones >= ثنائی_سائز);
      mpz_export (برآمد + ones, &exported_سائز, 1, 1, 0, 0, عدد);
    }

  mpz_clear (عدد);
  *outlen = ones + exported_سائز;
  واپس true;
}


ساکن bool
base58_decode_ctx_finalize (ساخت بنیاد_decode_context *ctx,
                            حرف *محدود *برآمد, idx_t *outlen)
{
  /* Ensure output buffer is large enough.
     Worst case is input is all '1's.  */
  idx_t max_outlen = ctx->ctx.base58.سائز;
  اگر (max_outlen > *outlen)
    {
      *برآمد = xrealloc (*برآمد, max_outlen);
      *outlen = max_outlen;
    }

  /* Ensure input buffer is NUL terminated (for mpz_get_str).  */
  اگر (ctx->ctx.base58.سائز)
    ctx->ctx.base58.buf[ctx->ctx.base58.سائز] = '\0';

  bool ret = base58_decode ((حرف *)ctx->ctx.base58.buf, ctx->ctx.base58.سائز,
                            *برآمد, outlen);

  آزاد (ctx->ctx.base58.buf);
  ctx->ctx.base58.buf = تہی;
  ctx->ctx.base58.سائز = 0;
  ctx->ctx.base58.استعداد = 0;

  واپس ret;
}

#اگر_ختم /* BASE_TYPE == 42, i.e., "basenc"*/



ساکن خالی
wrap_write (حرف ثابت *بفر, idx_t len,
            idx_t wrap_عمود, idx_t *current_عمود, FILE *برآمد)
{
  اگر (wrap_عمود == 0)
    {
      /* Simple write. */
      اگر (fwrite (بفر, 1, len, stdout) < len)
        write_error ();
    }
  ورنہ
    برائے (idx_t written = 0; written < len; )
      {
        idx_t مقصد_write = MIN (wrap_عمود - *current_عمود, len - written);

        اگر (مقصد_write == 0)
          {
            اگر (fputc ('\n', برآمد) == EOF)
              write_error ();
            *current_عمود = 0;
          }
        ورنہ
          {
            اگر (fwrite (بفر + written, 1, مقصد_write, stdout) < مقصد_write)
              write_error ();
            *current_عمود += مقصد_write;
            written += مقصد_write;
          }
      }
}

ساکن _Noreturn خالی
اختتام_and_exit (FILE *اندر, حرف ثابت *infile)
{
  اگر (fclose (اندر) != 0)
    {
      اگر (streq (infile, "-"))
        error (EXIT_FAILURE, errno, _("closing standard input"));
      ورنہ
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
    }

  باہر_نکلو (EXIT_SUCCESS);
}

ساکن _Noreturn خالی
do_encode (FILE *اندر, حرف ثابت *infile, FILE *برآمد, idx_t wrap_عمود)
{
  idx_t current_عمود = 0;
  حرف *inbuf, *outbuf;
  idx_t مجموع;

  inbuf = xmalloc (ENC_BLOCKSIZE);
  outbuf = xmalloc (بنیاد_لمبائی (ENC_BLOCKSIZE));

#اگر بنیاد_TYPE == 42
  /* Initialize encoding context if needed (for base58) */
  ساخت بنیاد_encode_context encode_ctx;
  bool use_ctx = (بنیاد_encode_ctx_init != تہی);
  اگر (use_ctx)
    بنیاد_encode_ctx_init (&encode_ctx);
#اگر_ختم

  کرو
    {
      idx_t n;

      مجموع = 0;
      کرو
        {
          n = fread (inbuf + مجموع, 1, ENC_BLOCKSIZE - مجموع, اندر);
          مجموع += n;
        }
      جب (!feof (اندر) && !ferror (اندر) && مجموع < ENC_BLOCKSIZE);

      اگر (مجموع > 0)
        {
#اگر بنیاد_TYPE == 42
          اگر (use_ctx)
            {
              idx_t outlen = 0;
              بنیاد_encode_ctx (&encode_ctx, inbuf, مجموع, outbuf, &outlen);

              wrap_write (outbuf, outlen, wrap_عمود, &current_عمود, برآمد);
            }
          ورنہ
#اگر_ختم
            {
              /* Process input one block at a time.  Note that ENC_BLOCKSIZE
                 is sized so that no pad chars will appear in output. */
              بنیاد_encode (inbuf, مجموع, outbuf, بنیاد_لمبائی (مجموع));

              wrap_write (outbuf, بنیاد_لمبائی (مجموع), wrap_عمود,
                          &current_عمود, برآمد);
            }
        }
    }
  جب (!feof (اندر) && !ferror (اندر) && مجموع == ENC_BLOCKSIZE);

#اگر بنیاد_TYPE == 42
  اگر (use_ctx && بنیاد_encode_ctx_finalize)
    {
      idx_t outlen = بنیاد_لمبائی (ENC_BLOCKSIZE);
      بنیاد_encode_ctx_finalize (&encode_ctx, &outbuf, &outlen);

      wrap_write (outbuf, outlen, wrap_عمود, &current_عمود, برآمد);
    }
#اگر_ختم

  /* When wrapping, terminate last line. */
  اگر (wrap_عمود && current_عمود > 0 && fputc ('\n', برآمد) == EOF)
    write_error ();

  اگر (ferror (اندر))
    error (EXIT_FAILURE, errno, _("read error"));

  اختتام_and_exit (اندر, infile);
}

ساکن _Noreturn خالی
do_decode (FILE *اندر, حرف ثابت *infile, FILE *برآمد, bool ignore_garbage)
{
  حرف *inbuf, *outbuf;
  idx_t مجموع;
  ساخت بنیاد_decode_context ctx;

  inbuf = xmalloc (بنیاد_لمبائی (DEC_BLOCKSIZE));
  outbuf = xmalloc (DEC_BLOCKSIZE);

#اگر بنیاد_TYPE == 42
  ctx.inbuf = تہی;
#اگر_ختم
  بنیاد_decode_ctx_init (&ctx);

  کرو
    {
      bool ok;

      مجموع = 0;
      کرو
        {
          idx_t n = fread (inbuf + مجموع,
                           1, بنیاد_لمبائی (DEC_BLOCKSIZE) - مجموع, اندر);

          اگر (ignore_garbage)
            {
              برائے (idx_t i = 0; n > 0 && i < n;)
                {
                  اگر (isubase (inbuf[مجموع + i])
                      || (REQUIRED_PADDING (1) && inbuf[مجموع + i] == '='))
                    i++;
                  ورنہ
                    memmove (inbuf + مجموع + i, inbuf + مجموع + i + 1, --n - i);
                }
            }

          مجموع += n;

          اگر (ferror (اندر))
            error (EXIT_FAILURE, errno, _("read error"));
        }
      جب (مجموع < بنیاد_لمبائی (DEC_BLOCKSIZE) && !feof (اندر));

      جب (مجموع || feof (اندر))
        {
          idx_t n = DEC_BLOCKSIZE;
          اگر (مجموع)
            ok = بنیاد_decode_ctx (&ctx, inbuf, مجموع, outbuf, &n);
          ورنہ
            ok = بنیاد_decode_ctx_finalize (&ctx, &outbuf, &n);

          اگر (fwrite (outbuf, 1, n, برآمد) < n)
            write_error ();

          اگر (! ok)
            error (EXIT_FAILURE, 0, _("invalid input"));

          اگر (مجموع == 0)
            توڑو;
          مجموع = 0;
        }
    }
  جب (!feof (اندر));

  اختتام_and_exit (اندر, infile);
}

صحیح
مین (صحیح argc, حرف **argv)
{
  صحیح opt;
  FILE *داخلہ_fh;
  حرف ثابت *infile;

  /* True if --decode has been given and we should decode data. */
  bool decode = false;
  /* True if we should ignore non-base-alphabetic characters. */
  bool ignore_garbage = false;
  /* Wrap encoded data around the 76th column, by default. */
  idx_t wrap_عمود = 76;

#اگر بنیاد_TYPE == 42
  صحیح بنیاد_type = 0;
#اگر_ختم

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  جب ((opt = getopt_long (argc, argv, "diw:", long_options, تہی)) != -1)
    انتخاب (opt)
      {
      حالت 'd':
        decode = true;
        توڑو;

      حالت 'w':
        {
          intmax_t w;
          strtol_error s_err = xstrtoimax (optarg, تہی, 10, &w, "");
          اگر (LONGINT_حد_سے_تجاوز < s_err || w < 0)
            error (EXIT_FAILURE, 0, "%s: %s",
                   _("invalid wrap size"), quote (optarg));
          wrap_عمود = s_err == LONGINT_حد_سے_تجاوز || IDX_MAX < w ? 0 : w;
        }
        توڑو;

      حالت 'i':
        ignore_garbage = true;
        توڑو;

#اگر بنیاد_TYPE == 42
      حالت BASE64_OPTION:
      حالت BASE64URL_OPTION:
      حالت BASE32_OPTION:
      حالت BASE32HEX_OPTION:
      حالت BASE16_OPTION:
      حالت BASE2MSBF_OPTION:
      حالت BASE2LSBF_OPTION:
      حالت Z85_OPTION:
      حالت BASE58_OPTION:
        بنیاد_type = opt;
        توڑو;
#اگر_ختم

      case_GETOPT_HELP_CHAR;

      case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

      طےشدہ:
        usage (EXIT_FAILURE);
        توڑو;
      }

#اگر بنیاد_TYPE == 42
  required_padding = no_required_padding;
  has_padding = no_padding;
  get_pending_لمبائی = no_pending_لمبائی;
  بنیاد_decode_ctx_finalize = decode_ctx_finalize;

  انتخاب (بنیاد_type)
    {
    حالت BASE64_OPTION:
      بنیاد_لمبائی_2 = base64_لمبائی_wrapper;
      required_padding = base64_required_padding;
      has_padding = base64_ctx_has_padding;
      get_pending_لمبائی = base64_ctx_get_pending_لمبائی;
      isubase = isubase64;
      بنیاد_encode = base64_encode;
      بنیاد_decode_ctx_init = base64_decode_ctx_init_wrapper;
      بنیاد_decode_ctx = base64_decode_ctx_wrapper;
      توڑو;

    حالت BASE64URL_OPTION:
      بنیاد_لمبائی_2 = base64_لمبائی_wrapper;
      required_padding = base64_required_padding;
      has_padding = base64_ctx_has_padding;
      get_pending_لمبائی = base64_ctx_get_pending_لمبائی;
      isubase = isubase64url;
      بنیاد_encode = base64url_encode;
      بنیاد_decode_ctx_init = base64url_decode_ctx_init_wrapper;
      بنیاد_decode_ctx = base64url_decode_ctx_wrapper;
      توڑو;

    حالت BASE32_OPTION:
      بنیاد_لمبائی_2 = base32_لمبائی_wrapper;
      required_padding = base32_required_padding;
      has_padding = base32_ctx_has_padding;
      get_pending_لمبائی = base32_ctx_get_pending_لمبائی;
      isubase = isubase32;
      بنیاد_encode = base32_encode;
      بنیاد_decode_ctx_init = base32_decode_ctx_init_wrapper;
      بنیاد_decode_ctx = base32_decode_ctx_wrapper;
      توڑو;

    حالت BASE32HEX_OPTION:
      بنیاد_لمبائی_2 = base32_لمبائی_wrapper;
      required_padding = base32_required_padding;
      has_padding = base32_ctx_has_padding;
      get_pending_لمبائی = base32_ctx_get_pending_لمبائی;
      isubase = isubase32hex;
      بنیاد_encode = base32hex_encode;
      بنیاد_decode_ctx_init = base32hex_decode_ctx_init_wrapper;
      بنیاد_decode_ctx = base32hex_decode_ctx_wrapper;
      توڑو;

    حالت BASE16_OPTION:
      بنیاد_لمبائی_2 = base16_لمبائی;
      get_pending_لمبائی = base16_ctx_get_pending_لمبائی;
      isubase = isubase16;
      بنیاد_encode = base16_encode;
      بنیاد_decode_ctx_init = base16_decode_ctx_init;
      بنیاد_decode_ctx = base16_decode_ctx;
      توڑو;

    حالت BASE2MSBF_OPTION:
      بنیاد_لمبائی_2 = base2_لمبائی;
      get_pending_لمبائی = base2_ctx_get_pending_لمبائی;
      isubase = isubase2;
      بنیاد_encode = base2msbf_encode;
      بنیاد_decode_ctx_init = base2_decode_ctx_init;
      بنیاد_decode_ctx = base2msbf_decode_ctx;
      توڑو;

    حالت BASE2LSBF_OPTION:
      بنیاد_لمبائی_2 = base2_لمبائی;
      get_pending_لمبائی = base2_ctx_get_pending_لمبائی;
      isubase = isubase2;
      بنیاد_encode = base2lsbf_encode;
      بنیاد_decode_ctx_init = base2_decode_ctx_init;
      بنیاد_decode_ctx = base2lsbf_decode_ctx;
      توڑو;

    حالت Z85_OPTION:
      بنیاد_لمبائی_2 = z85_لمبائی;
      get_pending_لمبائی = z85_ctx_get_pending_لمبائی;
      isubase = isuz85;
      بنیاد_encode = z85_encode;
      بنیاد_decode_ctx_init = z85_decode_ctx_init;
      بنیاد_decode_ctx = z85_decode_ctx;
      توڑو;

    حالت BASE58_OPTION:
      بنیاد_لمبائی_2 = base58_لمبائی;
      isubase = isubase58;
      بنیاد_encode_ctx_init = base58_encode_ctx_init;
      بنیاد_encode_ctx = base58_encode_ctx;
      بنیاد_encode_ctx_finalize = base58_encode_ctx_finalize;
      بنیاد_decode_ctx_init = base58_decode_ctx_init;
      بنیاد_decode_ctx = base58_decode_ctx;
      بنیاد_decode_ctx_finalize = base58_decode_ctx_finalize;
      توڑو;

    طےشدہ:
      error (0, 0, _("missing encoding type"));
      usage (EXIT_FAILURE);
    }
#اگر_ختم

  اگر (argc - optind > 1)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }

  اگر (optind < argc)
    infile = argv[optind];
  ورنہ
    infile = "-";

  اگر (streq (infile, "-"))
    {
      xset_ثنائی_منوال (STDIN_FILENO, O_ثنائی);
      داخلہ_fh = stdin;
    }
  ورنہ
    {
      داخلہ_fh = fopen (infile, "rb");
      اگر (داخلہ_fh == تہی)
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
    }

  fadvise (داخلہ_fh, FADVISE_SEQUENTIAL);

  اگر (decode)
    do_decode (داخلہ_fh, infile, stdout, ignore_garbage);
  ورنہ
    do_encode (داخلہ_fh, infile, stdout, wrap_عمود);
}
