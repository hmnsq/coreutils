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

#ضمن <config.h>

#ضمن <stdio.h>
#ضمن <getopt.h>
#ضمن <sys/types.h>
#ضمن <gmp.h>

#ضمن "system.h"
#ضمن "assure.h"
#ضمن "c-ctype.h"
#ضمن "fadvise.h"
#ضمن "quote.h"
#ضمن "xstrtol.h"
#ضمن "xbinary-io.h"

#إذا بنياد_TYPE == 42
# define AUTHORS \
  proper_name ("Simon Josefsson"), \
  proper_name ("Assaf Gordon")
#وإلا
# define AUTHORS proper_name ("Simon Josefsson")
#جيڪڏهن_پورو

#إذا بنياد_TYPE == 32
# include "base32.h"
# define پروگرام_NAME "base32"
#وإلاإذا بنياد_TYPE == 64
# include "base64.h"
# define پروگرام_NAME "base64"
#وإلاإذا بنياد_TYPE == 42
# include "base32.h"
# include "base64.h"
# include "assure.h"
# define پروگرام_NAME "basenc"
#وإلا
# error missing/invalid بنياد_TYPE definition
#جيڪڏهن_پورو



#إذا بنياد_TYPE == 42
تعداد
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
#جيڪڏهن_پورو

ساكن بنية option ثابت long_options[] =
{
  {"decode", no_دليل, عدم, 'd'},
  {"wrap", required_دليل, عدم, 'w'},
  {"ignore-garbage", no_دليل, عدم, 'i'},
#إذا بنياد_TYPE == 42
  {"base64",    no_دليل, عدم, BASE64_OPTION},
  {"base64url", no_دليل, عدم, BASE64URL_OPTION},
  {"base58",    no_دليل, عدم, BASE58_OPTION},
  {"base32",    no_دليل, عدم, BASE32_OPTION},
  {"base32hex", no_دليل, عدم, BASE32HEX_OPTION},
  {"base16",    no_دليل, عدم, BASE16_OPTION},
  {"base2msbf", no_دليل, عدم, BASE2MSBF_OPTION},
  {"base2lsbf", no_دليل, عدم, BASE2LSBF_OPTION},
  {"z85",       no_دليل, عدم, Z85_OPTION},
#جيڪڏهن_پورو
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخو_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکو (_("\
Usage: %s [OPTION]... [FILE]\n\
"), پروگرام_name);

#إذا بنياد_TYPE == 42
      fputs (_("\
basenc encode or decode FILE, or standard داخلا, منزل standard output.\n\
"), stdout);
#وإلا
      لکو (_("\
بنياد%d encode or decode FILE, or standard داخلا, منزل standard output.\n\
"), بنياد_TYPE);
#جيڪڏهن_پورو

      emit_stdin_note ();
      emit_mandatory_arg_note ();
#إذا بنياد_TYPE == 42
      oputs (_("\
      --base64\n\
         same as base64(1) پروگرام (RFC4648 section 4)\n\
"));
      oputs (_("\
      --base64url\n\
         file- and url-محفوظ base64 (RFC4648 section 5)\n\
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
         ٻٽو_انگ string with most significant ٻٽو_انگ (msb) پهريون\n\
"));
      oputs (_("\
      --base2lsbf\n\
         ٻٽو_انگ string with گهٽ_کان_گهٽ significant ٻٽو_انگ (lsb) پهريون\n\
"));
#جيڪڏهن_پورو
      oputs (_("\
  -d, --decode\n\
         decode معلومات\n\
"));
      oputs (_("\
  -i, --ignore-garbage\n\
         when decoding, ignore non-alphabet characters\n\
"));
      oputs (_("\
  -w, --wrap=کانن_ڳڻپ\n\
         wrap encoded سٽون after کانن_ڳڻپ character (افتراضي 76).\n\
         Use 0 منزل disable سٽ wrapping\n\
"));
#إذا بنياد_TYPE == 42
      oputs (_("\
      --z85\n\
         ascii85-like encoding (ZeroMQ spec:32/Z85);\n\
         when encoding, داخلا ڊيگهائي must be a گڻو جو 4;\n\
         when decoding, داخلا ڊيگهائي must be a گڻو جو 5\n\
"));
#جيڪڏهن_پورو
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخو_OPTION_DESCRIPTION);
#إذا بنياد_TYPE == 42
      fputs (_("\
\n\
When decoding, the داخلا may contain newlines اندريون addition منزل the بائيٽون جو\n\
the formal alphabet.  Use --ignore-garbage منزل attempt منزل recover\n\
from any other non-alphabet بائيٽون اندريون the encoded وهڪرو.\n\
"), stdout);
#وإلا
      لکو (_("\
\n\
The معلومات are encoded as described لكل the %s alphabet اندريون RFC 4648.\n\
When decoding, the داخلا may contain newlines اندريون addition منزل the بائيٽون جو\n\
the formal %s alphabet.  Use --ignore-garbage منزل attempt منزل recover\n\
from any other non-alphabet بائيٽون اندريون the encoded وهڪرو.\n"),
              پروگرام_NAME, پروگرام_NAME);
#جيڪڏهن_پورو
      emit_ancillary_info (پروگرام_NAME);
    }

  ٻاهر_نڪرو (حالت);
}

#إذا بنياد_TYPE != 64
ساكن صحيح
base32_required_padding (صحيح len)
{
  صحيح partial = len % 8;
  أرجع partial ? 8 - partial : 0;
}
#جيڪڏهن_پورو

#إذا بنياد_TYPE != 32
ساكن صحيح
base64_required_padding (صحيح len)
{
  صحيح partial = len % 4;
  أرجع partial ? 4 - partial : 0;
}
#جيڪڏهن_پورو

#إذا بنياد_TYPE == 42
ساكن صحيح
no_required_padding (MAYBE_اڻ_استعمال_ٿيل صحيح len)
{
  أرجع 0;
}
#جيڪڏهن_پورو

#وصف_ڪريو ENC_BLOCKSIZE (1024 * 3 * 10)

#إذا بنياد_TYPE == 32
# define بنياد_ڊيگهائي BASE32_ڊيگهائي
# define REQUIRED_PADDING base32_required_padding
/* Note that increasing this may decrease performance if --ignore-garbage
   is used, because of the memmove operation below.  */
# define DEC_BLOCKSIZE (1024 * 5)

/* Ensure that BLOCKSIZE is a multiple of 5 and 8.  */
static_assert (ENC_BLOCKSIZE % 40 == 0); /* Padding chars only on last block. */
static_assert (DEC_BLOCKSIZE % 40 == 0); /* Complete encoded blocks are used. */

# define بنياد_encode base32_encode
# define بنياد_decode_context base32_decode_context
# define بنياد_decode_ctx_init base32_decode_ctx_init
# define بنياد_decode_ctx base32_decode_ctx
# define بنياد_decode_ctx_finalize decode_ctx_finalize
# define isubase isubase32
#وإلاإذا بنياد_TYPE == 64
# define بنياد_ڊيگهائي BASE64_ڊيگهائي
# define REQUIRED_PADDING base64_required_padding
/* Note that increasing this may decrease performance if --ignore-garbage
   is used, because of the memmove operation below.  */
# define DEC_BLOCKSIZE (1024 * 3)

/* Ensure that BLOCKSIZE is a multiple of 3 and 4.  */
static_assert (ENC_BLOCKSIZE % 12 == 0); /* Padding chars only on last block. */
static_assert (DEC_BLOCKSIZE % 12 == 0); /* Complete encoded blocks are used. */

# define بنياد_encode base64_encode
# define بنياد_decode_context base64_decode_context
# define بنياد_decode_ctx_init base64_decode_ctx_init
# define بنياد_decode_ctx base64_decode_ctx
# define بنياد_decode_ctx_finalize decode_ctx_finalize
# define isubase isubase64
#وإلاإذا بنياد_TYPE == 42


# define بنياد_ڊيگهائي بنياد_ڊيگهائي_2
# define REQUIRED_PADDING required_padding

/* Note that increasing this may decrease performance if --ignore-garbage
   is used, because of the memmove operation below.  */
# define DEC_BLOCKSIZE (4200)
static_assert (DEC_BLOCKSIZE % 40 == 0); /* complete encoded blocks for base32*/
static_assert (DEC_BLOCKSIZE % 12 == 0); /* complete encoded blocks for base64*/

ساكن idx_t (*بنياد_ڊيگهائي_2) (idx_t len);
ساكن صحيح (*required_padding) (صحيح i);
ساكن bool (*isubase) (بنانشان حرف ch);
ساكن فراغ (*بنياد_encode) (حرف ثابت *محدود اندريون, idx_t inlen,
                            حرف *محدود ٻاهريون, idx_t outlen);

بنية base16_decode_context
{
  /* Either a 4-bit nibble, or negative if we have no nibble.  */
  نشانوارو حرف nibble;
};

بنية z85_decode_context
{
  صحيح i;
  بنانشان حرف octets[5];
};

بنية base58_context
{
  idx_t سائيز;
  idx_t صلاحيت;
  بنانشان حرف *buf COUNTED_BY (صلاحيت);
};

بنية base2_decode_context
{
  بنانشان حرف octet;
  صحيح ٻٽو_انگ_pos;
};

بنية بنياد_decode_context
{
  اتحاد {
    بنية base64_decode_context base64;
    بنية base32_decode_context base32;
    بنية base16_decode_context base16;
    بنية base2_decode_context base2;
    بنية z85_decode_context z85;
    بنية base58_context base58;
  } ctx;
  حرف *inbuf;
  idx_t bufsize;
};
ساكن فراغ (*بنياد_decode_ctx_init) (بنية بنياد_decode_context *ctx);
ساكن bool (*بنياد_decode_ctx) (بنية بنياد_decode_context *ctx,
                                حرف ثابت *محدود اندريون, idx_t inlen,
                                حرف *محدود ٻاهريون, idx_t *outlen);
ساكن bool (*بنياد_decode_ctx_finalize) (بنية بنياد_decode_context *ctx,
                                         حرف *محدود *ٻاهريون, idx_t *outlen);

بنية بنياد_encode_context
{
  اتحاد {
    بنية base58_context base58;
  } ctx;
};

ساكن فراغ (*بنياد_encode_ctx_init) (بنية بنياد_encode_context *ctx);
ساكن bool (*بنياد_encode_ctx) (بنية بنياد_encode_context *ctx,
                                حرف ثابت *محدود اندريون, idx_t inlen,
                                حرف *محدود ٻاهريون, idx_t *outlen);
ساكن bool (*بنياد_encode_ctx_finalize) (بنية بنياد_encode_context *ctx,
                                         حرف *محدود *ٻاهريون, idx_t *outlen);

ساكن bool
no_padding (MAYBE_اڻ_استعمال_ٿيل بنية بنياد_decode_context *ctx)
{
  أرجع false;
}

ساكن صحيح
no_pending_ڊيگهائي (MAYBE_اڻ_استعمال_ٿيل بنية بنياد_decode_context *ctx)
{
  أرجع 0;
}
#جيڪڏهن_پورو

#إذا بنياد_TYPE == 42
ساكن bool (*has_padding) (بنية بنياد_decode_context *ctx);
ساكن صحيح (*get_pending_ڊيگهائي) (بنية بنياد_decode_context *ctx);

ساكن bool
base64_ctx_has_padding (بنية بنياد_decode_context *ctx)
{
  أرجع ctx->ctx.base64.i && ctx->ctx.base64.buf[ctx->ctx.base64.i - 1] == '=';
}

ساكن bool
base32_ctx_has_padding (بنية بنياد_decode_context *ctx)
{
  أرجع ctx->ctx.base32.i && ctx->ctx.base32.buf[ctx->ctx.base32.i - 1] == '=';
}

ساكن صحيح
base64_ctx_get_pending_ڊيگهائي (بنية بنياد_decode_context *ctx)
{
  أرجع ctx->ctx.base64.i;
}

ساكن صحيح
base32_ctx_get_pending_ڊيگهائي (بنية بنياد_decode_context *ctx)
{
  أرجع ctx->ctx.base32.i;
}

ساكن صحيح
base16_ctx_get_pending_ڊيگهائي (MAYBE_اڻ_استعمال_ٿيل بنية بنياد_decode_context *ctx)
{
  أرجع 1; /* Always check nibble state.  */
}

ساكن صحيح
z85_ctx_get_pending_ڊيگهائي (بنية بنياد_decode_context *ctx)
{
  أرجع ctx->ctx.z85.i;
}

ساكن صحيح
base2_ctx_get_pending_ڊيگهائي (بنية بنياد_decode_context *ctx)
{
  أرجع ctx->ctx.base2.ٻٽو_انگ_pos;
}
#وإلا
ساكن bool
has_padding (بنية بنياد_decode_context *ctx)
{
  أرجع ctx->i && ctx->buf[ctx->i - 1] == '=';
}

ساكن صحيح
get_pending_ڊيگهائي (بنية بنياد_decode_context *ctx)
{
  أرجع ctx->i;
}
#جيڪڏهن_پورو


/* Process any pending data in CTX, while auto padding if appropriate.
   Return TRUE on success, FALSE on failure.  */

ساكن bool
decode_ctx_finalize (بنية بنياد_decode_context *ctx,
                     حرف *محدود *ٻاهريون, idx_t *outlen)
{
  إذا (get_pending_ڊيگهائي (ctx) == 0)
    {
      *outlen = 0;
      أرجع true;
    }

  /* Auto-pad input and flush the context */
  حرف padbuf[8] ATTRIBUTE_NONSTRING = "========";
  idx_t pending_len = get_pending_ڊيگهائي (ctx);
  idx_t auto_padding = REQUIRED_PADDING (pending_len);
  idx_t n = *outlen;
  bool نتيجو;

  إذا (auto_padding && ! has_padding (ctx))
    {
      affirm (auto_padding <= حجم (padbuf));
      نتيجو = بنياد_decode_ctx (ctx, padbuf, auto_padding, *ٻاهريون, &n);
    }
  وإلا
    {
      نتيجو = بنياد_decode_ctx (ctx, "", 0, *ٻاهريون, &n);
    }

  *outlen = n;
  أرجع نتيجو;
}

#إذا بنياد_TYPE == 42

ساكن idx_t
base64_ڊيگهائي_wrapper (idx_t len)
{
  أرجع BASE64_ڊيگهائي (len);
}

ساكن فراغ
base64_decode_ctx_init_wrapper (بنية بنياد_decode_context *ctx)
{
  base64_decode_ctx_init (&ctx->ctx.base64);
}

ساكن bool
base64_decode_ctx_wrapper (بنية بنياد_decode_context *ctx,
                           حرف ثابت *محدود اندريون, idx_t inlen,
                           حرف *محدود ٻاهريون, idx_t *outlen)
{
  أرجع base64_decode_ctx (&ctx->ctx.base64, اندريون, inlen, ٻاهريون, outlen);
}

ساكن فراغ
init_inbuf (بنية بنياد_decode_context *ctx)
{
  ctx->bufsize = DEC_BLOCKSIZE;
  ctx->inbuf = xcharalloc (ctx->bufsize);
}

ساكن فراغ
prepare_inbuf (بنية بنياد_decode_context *ctx, idx_t inlen)
{
  إذا (ctx->bufsize < inlen)
    ctx->inbuf = xpalloc (ctx->inbuf, &ctx->bufsize,
                          inlen - ctx->bufsize, -1, حجم *ctx->inbuf);
}


ساكن فراغ
base64url_encode (حرف ثابت *محدود اندريون, idx_t inlen,
                  حرف *محدود ٻاهريون, idx_t outlen)
{
  base64_encode (اندريون, inlen, ٻاهريون, outlen);
  /* translate 62nd and 63rd characters */
  حرف *p = ٻاهريون;
  طالما (outlen--)
    {
      إذا (*p == '+')
        *p = '-';
      وإلا إذا (*p == '/')
        *p = '_';
      ++p;
    }
}

ساكن bool
isubase64url (بنانشان حرف ch)
{
  أرجع (ch == '-' || ch == '_'
          || (ch != '+' && ch != '/' && isubase64 (ch)));
}

ساكن فراغ
base64url_decode_ctx_init_wrapper (بنية بنياد_decode_context *ctx)
{
  base64_decode_ctx_init (&ctx->ctx.base64);
  init_inbuf (ctx);
}


ساكن bool
base64url_decode_ctx_wrapper (بنية بنياد_decode_context *ctx,
                              حرف ثابت *محدود اندريون, idx_t inlen,
                              حرف *محدود ٻاهريون, idx_t *outlen)
{
  prepare_inbuf (ctx, inlen);
  يادگيري_نقل (ctx->inbuf, اندريون, inlen);

  /* translate 62nd and 63rd characters */
  idx_t i = inlen;
  حرف *p = ctx->inbuf;
  طالما (i--)
    {
      إذا (*p == '+' || *p == '/')
        {
          *outlen = 0;
          أرجع false; /* reject base64 input */
        }
      وإلا إذا (*p == '-')
        *p = '+';
      وإلا إذا (*p == '_')
        *p = '/';
      ++p;
    }

  أرجع base64_decode_ctx (&ctx->ctx.base64, ctx->inbuf, inlen,
                            ٻاهريون, outlen);
}



ساكن idx_t
base32_ڊيگهائي_wrapper (idx_t len)
{
  أرجع BASE32_ڊيگهائي (len);
}

ساكن فراغ
base32_decode_ctx_init_wrapper (بنية بنياد_decode_context *ctx)
{
  base32_decode_ctx_init (&ctx->ctx.base32);
}

ساكن bool
base32_decode_ctx_wrapper (بنية بنياد_decode_context *ctx,
                           حرف ثابت *محدود اندريون, idx_t inlen,
                           حرف *محدود ٻاهريون, idx_t *outlen)
{
  أرجع base32_decode_ctx (&ctx->ctx.base32, اندريون, inlen, ٻاهريون, outlen);
}

/* ABCDEFGHIJKLMNOPQRSTUVWXYZ234567
منزل
   0123456789ABCDEFGHIJKLMNOPQRSTUV */
ساكن ثابت حرف base32_norm_منزل_hex[32 + 9] = {
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
منزل
   ABCDEFGHIJKLMNOPQRSTUVWXYZ234567 */
ساكن ثابت حرف base32_hex_منزل_norm[32 + 9] = {
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


مضمن ساكن bool
isubase32hex (بنانشان حرف ch)
{
  أرجع ('0' <= ch && ch <= '9') || ('A' <= ch && ch <= 'V');
}


ساكن فراغ
base32hex_encode (حرف ثابت *محدود اندريون, idx_t inlen,
                  حرف *محدود ٻاهريون, idx_t outlen)
{
  base32_encode (اندريون, inlen, ٻاهريون, outlen);

  لكل (حرف *p = ٻاهريون; outlen--; p++)
    {
      affirm (0x32 <= *p && *p <= 0x5a);          /* LCOV_EXCL_LINE */
      *p = base32_norm_منزل_hex[*p - 0x32];
    }
}


ساكن فراغ
base32hex_decode_ctx_init_wrapper (بنية بنياد_decode_context *ctx)
{
  base32_decode_ctx_init (&ctx->ctx.base32);
  init_inbuf (ctx);
}


ساكن bool
base32hex_decode_ctx_wrapper (بنية بنياد_decode_context *ctx,
                              حرف ثابت *محدود اندريون, idx_t inlen,
                              حرف *محدود ٻاهريون, idx_t *outlen)
{
  prepare_inbuf (ctx, inlen);

  idx_t i = inlen;
  حرف *p = ctx->inbuf;
  طالما (i--)
    {
      إذا (isubase32hex (*اندريون))
        *p = base32_hex_منزل_norm[*اندريون - 0x30];
      وإلا
        *p = *اندريون;
      ++p;
      ++اندريون;
    }

  أرجع base32_decode_ctx (&ctx->ctx.base32, ctx->inbuf, inlen,
                            ٻاهريون, outlen);
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

ساكن نشانوارو حرف ثابت base16_منزل_int[256] = {
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

ساكن bool
isubase16 (بنانشان حرف ch)
{
  أرجع ch < حجم base16_منزل_int && 0 <= base16_منزل_int[ch];
}

ساكن idx_t
base16_ڊيگهائي (idx_t len)
{
  أرجع len * 2;
}


ساكن فراغ
base16_encode (حرف ثابت *محدود اندريون, idx_t inlen,
               حرف *محدود ٻاهريون, idx_t outlen)
{
  ساكن ثابت حرف base16[16] ATTRIBUTE_NONSTRING = "0123456789ABCDEF";

  طالما (inlen && outlen)
    {
      بنانشان حرف c = *اندريون;
      *ٻاهريون++ = base16[c >> 4];
      *ٻاهريون++ = base16[c & 0x0F];
      ++اندريون;
      inlen--;
      outlen -= 2;
    }
}


ساكن فراغ
base16_decode_ctx_init (بنية بنياد_decode_context *ctx)
{
  init_inbuf (ctx);
  ctx->ctx.base16.nibble = -1;
}


ساكن bool
base16_decode_ctx (بنية بنياد_decode_context *ctx,
                   حرف ثابت *محدود اندريون, idx_t inlen,
                   حرف *محدود ٻاهريون, idx_t *outlen)
{
  bool ignore_سٽون = true;  /* for now, always ignore them */
  حرف *out0 = ٻاهريون;
  نشانوارو حرف nibble = ctx->ctx.base16.nibble;

  /* inlen==0 is request to flush output.
     if there is a dangling high nibble - we are missing the low nibble,
     so return false - indicating an invalid input.  */
  إذا (inlen == 0)
    {
      *outlen = 0;
      أرجع nibble < 0;
    }

  طالما (inlen--)
    {
      بنانشان حرف c = *اندريون++;
      إذا (ignore_سٽون && c == '\n')
        استمر;

      إذا (حجم base16_منزل_int <= c || base16_منزل_int[c] < 0)
        {
          *outlen = ٻاهريون - out0;
          أرجع false; /* garbage - return false */
        }

      إذا (nibble < 0)
        nibble = base16_منزل_int[c];
      وإلا
        {
          /* have both nibbles, write octet */
          *ٻاهريون++ = (nibble << 4) + base16_منزل_int[c];
          nibble = -1;
        }
    }

  ctx->ctx.base16.nibble = nibble;
  *outlen = ٻاهريون - out0;
  أرجع true;
}



ATTRIBUTE_خالص
ساكن idx_t
z85_ڊيگهائي (idx_t len)
{
  /* Z85 does not allow padding, so no need to round to highest integer.  */
  idx_t z85_len = (len * 5) / 4;
  affirm (0 <= z85_len);
  أرجع z85_len;
}

ساكن bool
isuz85 (بنانشان حرف ch)
{
  أرجع c_isalnum (ch) || strchr (".-:+=^!/*?&<>()[]{}@%$#", ch) != عدم;
}

ساكن حرف ثابت z85_encoding[85] ATTRIBUTE_NONSTRING =
  "0123456789"
  "abcdefghijklmnopqrstuvwxyz"
  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
  ".-:+=^!/*?&<>()[]{}@%$#";

ساكن فراغ
z85_encode (حرف ثابت *محدود اندريون, idx_t inlen,
            حرف *محدود ٻاهريون, idx_t outlen)
{
  صحيح i = 0;
  بنانشان حرف quad[4];
  idx_t outidx = 0;

  طالما (true)
    {
      إذا (inlen == 0)
        {
          /* no more input, exactly on 4 octet boundary. */
          إذا (i == 0)
            أرجع;

          /* currently, there's no way to return an error in encoding.  */
          error (EXIT_FAILURE, 0,
                 _("invalid input (length must be multiple of 4 characters)"));
        }
      وإلا
        {
          quad[i++] = *اندريون++;
          --inlen;
        }

      /* Got a quad, encode it */
      إذا (i == 4)
        {
          int_fast64_t val = quad[0];
          val = (val << 24) + (quad[1] << 16) + (quad[2] << 8) + quad[3];

          لكل (صحيح j = 4; j >= 0; --j)
            {
              صحيح c = val % 85;
              val /= 85;

              /* NOTE: if there is padding (which is trimmed by z85
                 before outputting the result), the output buffer 'out'
                 might not include enough allocated bytes for the padding,
                 so don't store them. */
              إذا (outidx + j < outlen)
                ٻاهريون[j] = z85_encoding[c];
            }
          ٻاهريون += 5;
          outidx += 5;
          i = 0;
        }
    }
}

ساكن فراغ
z85_decode_ctx_init (بنية بنياد_decode_context *ctx)
{
  init_inbuf (ctx);
  ctx->ctx.z85.i = 0;
}


# define Z85_LO_CTX_منزل_32_ٻٽو_انگ_VAL(ctx) \
  (((ctx)->ctx.z85.octets[1] * 85 * 85 * 85) +      \
   ((ctx)->ctx.z85.octets[2] * 85 * 85) +	    \
   ((ctx)->ctx.z85.octets[3] * 85) +		    \
   ((ctx)->ctx.z85.octets[4]))


# define Z85_HI_CTX_منزل_32_ٻٽو_انگ_VAL(ctx) \
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
ساكن نشانوارو حرف ثابت z85_decoding[93] = {
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

ساكن bool
z85_decode_ctx (بنية بنياد_decode_context *ctx,
                حرف ثابت *محدود اندريون, idx_t inlen,
                حرف *محدود ٻاهريون, idx_t *outlen)
{
  bool ignore_سٽون = true;  /* for now, always ignore them */

  *outlen = 0;

  /* inlen==0 is request to flush output.
     if there are dangling values - we are missing entries,
     so return false - indicating an invalid input.  */
  إذا (inlen == 0)
    {
      إذا (ctx->ctx.z85.i > 0)
        {
          /* Z85 variant does not allow padding - input must
             be a multiple of 5 - so return error.  */
          أرجع false;
        }
      أرجع true;
    }

  طالما (inlen--)
    {
      إذا (ignore_سٽون && *اندريون == '\n')
        {
          ++اندريون;
          استمر;
        }

      /* z85 decoding */
      بنانشان حرف c = *اندريون;

      إذا (c >= 33 && c <= 125)
        {
          نشانوارو حرف ch = z85_decoding[c - 33];
          إذا (ch < 0)
            أرجع false; /* garbage - return false */
          c = ch;
        }
      وإلا
        أرجع false; /* garbage - return false */

      ++اندريون;

      ctx->ctx.z85.octets[ctx->ctx.z85.i++] = c;
      إذا (ctx->ctx.z85.i == 5)
        {
          /* decode the lowest 4 octets, then check for overflows.  */
          int_fast64_t val = Z85_LO_CTX_منزل_32_ٻٽو_انگ_VAL (ctx);

          /* The Z85 spec and the reference implementation say nothing
             about overflows. To be on the safe side, reject them.  */

          val += Z85_HI_CTX_منزل_32_ٻٽو_انگ_VAL (ctx);
          إذا ((val >> 24) & ~0xFF)
            أرجع false;

          *ٻاهريون++ = val >> 24;
          *ٻاهريون++ = (val >> 16) & 0xFF;
          *ٻاهريون++ = (val >> 8) & 0xFF;
          *ٻاهريون++ = val & 0xFF;

          *outlen += 4;

          ctx->ctx.z85.i = 0;
        }
    }
  أرجع true;
}


مضمن ساكن bool
isubase2 (بنانشان حرف ch)
{
  أرجع ch == '0' || ch == '1';
}

ساكن idx_t
base2_ڊيگهائي (idx_t len)
{
  أرجع len * 8;
}


مضمن ساكن فراغ
base2msbf_encode (حرف ثابت *محدود اندريون, idx_t inlen,
                  حرف *محدود ٻاهريون, idx_t outlen)
{
  طالما (inlen && outlen)
    {
      بنانشان حرف c = *اندريون;
      لكل (صحيح i = 0; i < 8; i++)
        {
          *ٻاهريون++ = c & 0x80 ? '1' : '0';
          c <<= 1;
        }
      inlen--;
      outlen -= 8;
      ++اندريون;
    }
}

مضمن ساكن فراغ
base2lsbf_encode (حرف ثابت *محدود اندريون, idx_t inlen,
                  حرف *محدود ٻاهريون, idx_t outlen)
{
  طالما (inlen && outlen)
    {
      بنانشان حرف c = *اندريون;
      لكل (صحيح i = 0; i < 8; i++)
        {
          *ٻاهريون++ = c & 0x01 ? '1' : '0';
          c >>= 1;
        }
      inlen--;
      outlen -= 8;
      ++اندريون;
    }
}


ساكن فراغ
base2_decode_ctx_init (بنية بنياد_decode_context *ctx)
{
  init_inbuf (ctx);
  ctx->ctx.base2.octet = 0;
  ctx->ctx.base2.ٻٽو_انگ_pos = 0;
}


ساكن bool
base2lsbf_decode_ctx (بنية بنياد_decode_context *ctx,
                      حرف ثابت *محدود اندريون, idx_t inlen,
                      حرف *محدود ٻاهريون, idx_t *outlen)
{
  bool ignore_سٽون = true;  /* for now, always ignore them */

  *outlen = 0;

  /* inlen==0 is request to flush output.
     if there is a dangling bit - we are missing some bits,
     so return false - indicating an invalid input.  */
  إذا (inlen == 0)
    أرجع ctx->ctx.base2.ٻٽو_انگ_pos == 0;

  طالما (inlen--)
    {
      إذا (ignore_سٽون && *اندريون == '\n')
        {
          ++اندريون;
          استمر;
        }

      إذا (!isubase2 (*اندريون))
        أرجع false;

      bool ٻٽو_انگ = (*اندريون == '1');
      ctx->ctx.base2.octet |= ٻٽو_انگ << ctx->ctx.base2.ٻٽو_انگ_pos;
      ++ctx->ctx.base2.ٻٽو_انگ_pos;

      إذا (ctx->ctx.base2.ٻٽو_انگ_pos == 8)
        {
          *ٻاهريون++ = ctx->ctx.base2.octet;
          ctx->ctx.base2.octet = 0;
          ++*outlen;
          ctx->ctx.base2.ٻٽو_انگ_pos = 0;
        }

      ++اندريون;
    }

  أرجع true;
}

ساكن bool
base2msbf_decode_ctx (بنية بنياد_decode_context *ctx,
                      حرف ثابت *محدود اندريون, idx_t inlen,
                      حرف *محدود ٻاهريون, idx_t *outlen)
{
  bool ignore_سٽون = true;  /* for now, always ignore them */

  *outlen = 0;

  /* inlen==0 is request to flush output.
     if there is a dangling bit - we are missing some bits,
     so return false - indicating an invalid input.  */
  إذا (inlen == 0)
    أرجع ctx->ctx.base2.ٻٽو_انگ_pos == 0;

  طالما (inlen--)
    {
      إذا (ignore_سٽون && *اندريون == '\n')
        {
          ++اندريون;
          استمر;
        }

      إذا (!isubase2 (*اندريون))
        أرجع false;

      bool ٻٽو_انگ = (*اندريون == '1');
      إذا (ctx->ctx.base2.ٻٽو_انگ_pos == 0)
        ctx->ctx.base2.ٻٽو_انگ_pos = 8;
      --ctx->ctx.base2.ٻٽو_انگ_pos;
      ctx->ctx.base2.octet |= ٻٽو_انگ << ctx->ctx.base2.ٻٽو_انگ_pos;

      إذا (ctx->ctx.base2.ٻٽو_انگ_pos == 0)
        {
          *ٻاهريون++ = ctx->ctx.base2.octet;
          ctx->ctx.base2.octet = 0;
          ++*outlen;
        }

      ++اندريون;
    }

  أرجع true;
}

/* Map from GMP (up to base 62):
   "0123456789 ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuv";
   to base 58:
   "123456789A BCDEFGHJKLMNPQRSTUVWXYZabc defghijkmnopqrstuvwxyz";  */
ساكن نشانوارو حرف ثابت gmp_منزل_base58[256] = {
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

ساكن نشانوارو حرف ثابت base58_منزل_gmp[256] = {
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

ساكن bool
isubase58 (بنانشان حرف ch)
{
  أرجع ch < حجم base58_منزل_gmp && 0 <= base58_منزل_gmp[ch];
}


ATTRIBUTE_خالص
ساكن idx_t
base58_ڊيگهائي (idx_t len)
{
  /* Base58 output length is approximately log(256)/log(58),
     which is approximately len * 138 / 100,
     which is at most ((len + 100 - 1) / 100) * 138
     +1 to ensure we've enough place for NUL  */
  idx_t base58_len = ((len + 99) / 100) * 138 + 1;
  affirm (0 < base58_len);
  أرجع base58_len;
}


ساكن فراغ
base58_encode_ctx_init (بنية بنياد_encode_context *ctx)
{
  ctx->ctx.base58.buf = عدم;
  ctx->ctx.base58.سائيز = 0;
  ctx->ctx.base58.صلاحيت = 0;
}


ساكن bool
base58_encode_ctx (بنية بنياد_encode_context *ctx,
                   حرف ثابت *محدود اندريون, idx_t inlen,
                   MAYBE_اڻ_استعمال_ٿيل حرف *محدود ٻاهريون, idx_t *outlen)
{
  *outlen = 0;   /* Only accumulate input in this function.  */

  إذا (inlen == 0)
    أرجع true;

  idx_t free_space = ctx->ctx.base58.صلاحيت - ctx->ctx.base58.سائيز;
  إذا (free_space < inlen)
    {
      ctx->ctx.base58.buf = xpalloc (ctx->ctx.base58.buf,
                                     &ctx->ctx.base58.صلاحيت,
                                     inlen - free_space,
                                     -1, حجم *ctx->ctx.base58.buf);
    }

  يادگيري_نقل (ctx->ctx.base58.buf + ctx->ctx.base58.سائيز, اندريون, inlen);
  ctx->ctx.base58.سائيز += inlen;

  أرجع true;
}

ساكن فراغ
base58_encode (حرف ثابت *معلومات, size_t معلومات_len,
               حرف *ٻاهريون, idx_t *outlen)
{
  affirm (بنياد_ڊيگهائي_2 (معلومات_len) <= *outlen);

  size_t zeros = 0;
  طالما (zeros < معلومات_len && معلومات[zeros] == 0)
    zeros++;

  memset (ٻاهريون, '1', zeros);
  حرف *p = ٻاهريون + zeros;

  /* Use GMP to convert from base 256 to base 58.  */
  mpz_t انگ;
  mpz_init (انگ);
  إذا (معلومات_len - zeros)
    {
      mpz_import (انگ, معلومات_len - zeros, 1, 1, 0, 0, معلومات + zeros);
      لكل (p = mpz_get_str (p, 58, انگ); *p; p++)
        *p = gmp_منزل_base58[منزل_uchar (*p)];
    }
  mpz_clear (انگ);

  *outlen = p - ٻاهريون;
}


ساكن bool
base58_encode_ctx_finalize (بنية بنياد_encode_context *ctx,
                            حرف *محدود *ٻاهريون, idx_t *outlen)
{
  /* Ensure output buffer is large enough.  */
  idx_t max_outlen = بنياد_ڊيگهائي_2 (ctx->ctx.base58.سائيز);
  إذا (max_outlen > *outlen)
    {
      *ٻاهريون = xrealloc (*ٻاهريون, max_outlen);
      *outlen = max_outlen;
    }

  base58_encode ((حرف *)ctx->ctx.base58.buf, ctx->ctx.base58.سائيز,
                 *ٻاهريون, outlen);

  حرر (ctx->ctx.base58.buf);
  ctx->ctx.base58.buf = عدم;
  ctx->ctx.base58.سائيز = 0;
  ctx->ctx.base58.صلاحيت = 0;

  أرجع true;
}


ساكن فراغ
base58_decode_ctx_init (بنية بنياد_decode_context *ctx)
{
  ctx->ctx.base58.سائيز = 0;
  ctx->ctx.base58.صلاحيت = 0;
  ctx->ctx.base58.buf = عدم;
}

ساكن bool
base58_decode_ctx (بنية بنياد_decode_context *ctx,
                   حرف ثابت *محدود اندريون, idx_t inlen,
                   MAYBE_اڻ_استعمال_ٿيل حرف *محدود ٻاهريون, idx_t *outlen)
{
  bool ignore_سٽون = true;  /* for now, always ignore them */

  *outlen = 0;   /* Only accumulate input in this function.  */

  إذا (inlen == 0)
    أرجع true;

  idx_t free_space = ctx->ctx.base58.صلاحيت - ctx->ctx.base58.سائيز;
  free_space -= 1;  /* Ensure we leave space for NUL (for mpz_set_str).  */
  إذا (free_space < inlen)
    {
      ctx->ctx.base58.buf = xpalloc (ctx->ctx.base58.buf,
                                     &ctx->ctx.base58.صلاحيت,
                                     inlen - free_space,
                                     -1, حجم *ctx->ctx.base58.buf);
    }

  /* Accumulate all valid input characters in our buffer.
     Note we don't rely on mpz_set_str() for validation
     as that allows (skips) all whitespace.  */
  لكل (idx_t i = 0; i < inlen; i++)
    {
      بنانشان حرف c = اندريون[i];

      إذا (ignore_سٽون && c == '\n')
        استمر;

      إذا (!isubase58 (c))
        أرجع false;

      ctx->ctx.base58.buf[ctx->ctx.base58.سائيز++] = base58_منزل_gmp[منزل_uchar (c)];
    }

  أرجع true;
}


ساكن bool
base58_decode (حرف ثابت *معلومات, size_t معلومات_len,
               حرف *محدود ٻاهريون, idx_t *outlen)
{
  affirm (معلومات_len <= *outlen);

  size_t ones = 0;
  طالما (ones < معلومات_len && معلومات[ones] == base58_منزل_gmp['1'])
    ones++;

  memset (ٻاهريون, 0, ones);

  /* Use GMP to convert from base 58 to base 256.  */
  mpz_t انگ;
  mpz_init (انگ);

  إذا ((معلومات_len - ones) && mpz_سيٽ_str (انگ, معلومات + ones, 58) != 0)
    {
      mpz_clear (انگ);
      *outlen = 0;
      أرجع false;
    }

  size_t exported_سائيز = 0;
  إذا (معلومات_len - ones)
    {
      size_t ٻٽو_سائيز = (mpz_sizeinbase (انگ, 2) + 7) / 8;
      affirm (*outlen - ones >= ٻٽو_سائيز);
      mpz_export (ٻاهريون + ones, &exported_سائيز, 1, 1, 0, 0, انگ);
    }

  mpz_clear (انگ);
  *outlen = ones + exported_سائيز;
  أرجع true;
}


ساكن bool
base58_decode_ctx_finalize (بنية بنياد_decode_context *ctx,
                            حرف *محدود *ٻاهريون, idx_t *outlen)
{
  /* Ensure output buffer is large enough.
     Worst case is input is all '1's.  */
  idx_t max_outlen = ctx->ctx.base58.سائيز;
  إذا (max_outlen > *outlen)
    {
      *ٻاهريون = xrealloc (*ٻاهريون, max_outlen);
      *outlen = max_outlen;
    }

  /* Ensure input buffer is NUL terminated (for mpz_get_str).  */
  إذا (ctx->ctx.base58.سائيز)
    ctx->ctx.base58.buf[ctx->ctx.base58.سائيز] = '\0';

  bool ret = base58_decode ((حرف *)ctx->ctx.base58.buf, ctx->ctx.base58.سائيز,
                            *ٻاهريون, outlen);

  حرر (ctx->ctx.base58.buf);
  ctx->ctx.base58.buf = عدم;
  ctx->ctx.base58.سائيز = 0;
  ctx->ctx.base58.صلاحيت = 0;

  أرجع ret;
}

#جيڪڏهن_پورو /* BASE_TYPE == 42, i.e., "basenc"*/



ساكن فراغ
wrap_write (حرف ثابت *بفر, idx_t len,
            idx_t wrap_ڪالم, idx_t *current_ڪالم, FILE *ٻاهريون)
{
  إذا (wrap_ڪالم == 0)
    {
      /* Simple write. */
      إذا (fwrite (بفر, 1, len, stdout) < len)
        write_error ();
    }
  وإلا
    لكل (idx_t written = 0; written < len; )
      {
        idx_t منزل_write = MIN (wrap_ڪالم - *current_ڪالم, len - written);

        إذا (منزل_write == 0)
          {
            إذا (fputc ('\n', ٻاهريون) == EOF)
              write_error ();
            *current_ڪالم = 0;
          }
        وإلا
          {
            إذا (fwrite (بفر + written, 1, منزل_write, stdout) < منزل_write)
              write_error ();
            *current_ڪالم += منزل_write;
            written += منزل_write;
          }
      }
}

ساكن _Noreturn فراغ
پڄاڻي_and_exit (FILE *اندريون, حرف ثابت *infile)
{
  إذا (fclose (اندريون) != 0)
    {
      إذا (streq (infile, "-"))
        error (EXIT_FAILURE, errno, _("closing standard input"));
      وإلا
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
    }

  ٻاهر_نڪرو (EXIT_SUCCESS);
}

ساكن _Noreturn فراغ
do_encode (FILE *اندريون, حرف ثابت *infile, FILE *ٻاهريون, idx_t wrap_ڪالم)
{
  idx_t current_ڪالم = 0;
  حرف *inbuf, *outbuf;
  idx_t جوڙ;

  inbuf = xmalloc (ENC_BLOCKSIZE);
  outbuf = xmalloc (بنياد_ڊيگهائي (ENC_BLOCKSIZE));

#إذا بنياد_TYPE == 42
  /* Initialize encoding context if needed (for base58) */
  بنية بنياد_encode_context encode_ctx;
  bool use_ctx = (بنياد_encode_ctx_init != عدم);
  إذا (use_ctx)
    بنياد_encode_ctx_init (&encode_ctx);
#جيڪڏهن_پورو

  افعل
    {
      idx_t n;

      جوڙ = 0;
      افعل
        {
          n = fread (inbuf + جوڙ, 1, ENC_BLOCKSIZE - جوڙ, اندريون);
          جوڙ += n;
        }
      طالما (!feof (اندريون) && !ferror (اندريون) && جوڙ < ENC_BLOCKSIZE);

      إذا (جوڙ > 0)
        {
#إذا بنياد_TYPE == 42
          إذا (use_ctx)
            {
              idx_t outlen = 0;
              بنياد_encode_ctx (&encode_ctx, inbuf, جوڙ, outbuf, &outlen);

              wrap_write (outbuf, outlen, wrap_ڪالم, &current_ڪالم, ٻاهريون);
            }
          وإلا
#جيڪڏهن_پورو
            {
              /* Process input one block at a time.  Note that ENC_BLOCKSIZE
                 is sized so that no pad chars will appear in output. */
              بنياد_encode (inbuf, جوڙ, outbuf, بنياد_ڊيگهائي (جوڙ));

              wrap_write (outbuf, بنياد_ڊيگهائي (جوڙ), wrap_ڪالم,
                          &current_ڪالم, ٻاهريون);
            }
        }
    }
  طالما (!feof (اندريون) && !ferror (اندريون) && جوڙ == ENC_BLOCKSIZE);

#إذا بنياد_TYPE == 42
  إذا (use_ctx && بنياد_encode_ctx_finalize)
    {
      idx_t outlen = بنياد_ڊيگهائي (ENC_BLOCKSIZE);
      بنياد_encode_ctx_finalize (&encode_ctx, &outbuf, &outlen);

      wrap_write (outbuf, outlen, wrap_ڪالم, &current_ڪالم, ٻاهريون);
    }
#جيڪڏهن_پورو

  /* When wrapping, terminate last line. */
  إذا (wrap_ڪالم && current_ڪالم > 0 && fputc ('\n', ٻاهريون) == EOF)
    write_error ();

  إذا (ferror (اندريون))
    error (EXIT_FAILURE, errno, _("read error"));

  پڄاڻي_and_exit (اندريون, infile);
}

ساكن _Noreturn فراغ
do_decode (FILE *اندريون, حرف ثابت *infile, FILE *ٻاهريون, bool ignore_garbage)
{
  حرف *inbuf, *outbuf;
  idx_t جوڙ;
  بنية بنياد_decode_context ctx;

  inbuf = xmalloc (بنياد_ڊيگهائي (DEC_BLOCKSIZE));
  outbuf = xmalloc (DEC_BLOCKSIZE);

#إذا بنياد_TYPE == 42
  ctx.inbuf = عدم;
#جيڪڏهن_پورو
  بنياد_decode_ctx_init (&ctx);

  افعل
    {
      bool ok;

      جوڙ = 0;
      افعل
        {
          idx_t n = fread (inbuf + جوڙ,
                           1, بنياد_ڊيگهائي (DEC_BLOCKSIZE) - جوڙ, اندريون);

          إذا (ignore_garbage)
            {
              لكل (idx_t i = 0; n > 0 && i < n;)
                {
                  إذا (isubase (inbuf[جوڙ + i])
                      || (REQUIRED_PADDING (1) && inbuf[جوڙ + i] == '='))
                    i++;
                  وإلا
                    memmove (inbuf + جوڙ + i, inbuf + جوڙ + i + 1, --n - i);
                }
            }

          جوڙ += n;

          إذا (ferror (اندريون))
            error (EXIT_FAILURE, errno, _("read error"));
        }
      طالما (جوڙ < بنياد_ڊيگهائي (DEC_BLOCKSIZE) && !feof (اندريون));

      طالما (جوڙ || feof (اندريون))
        {
          idx_t n = DEC_BLOCKSIZE;
          إذا (جوڙ)
            ok = بنياد_decode_ctx (&ctx, inbuf, جوڙ, outbuf, &n);
          وإلا
            ok = بنياد_decode_ctx_finalize (&ctx, &outbuf, &n);

          إذا (fwrite (outbuf, 1, n, ٻاهريون) < n)
            write_error ();

          إذا (! ok)
            error (EXIT_FAILURE, 0, _("invalid input"));

          إذا (جوڙ == 0)
            اكسر;
          جوڙ = 0;
        }
    }
  طالما (!feof (اندريون));

  پڄاڻي_and_exit (اندريون, infile);
}

صحيح
مکيه (صحيح argc, حرف **argv)
{
  صحيح opt;
  FILE *داخلا_fh;
  حرف ثابت *infile;

  /* True if --decode has been given and we should decode data. */
  bool decode = false;
  /* True if we should ignore non-base-alphabetic characters. */
  bool ignore_garbage = false;
  /* Wrap encoded data around the 76th column, by default. */
  idx_t wrap_ڪالم = 76;

#إذا بنياد_TYPE == 42
  صحيح بنياد_type = 0;
#جيڪڏهن_پورو

  initialize_main (&argc, &argv);
  سيٽ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  طالما ((opt = getopt_long (argc, argv, "diw:", long_options, عدم)) != -1)
    بدل (opt)
      {
      حالة 'd':
        decode = true;
        اكسر;

      حالة 'w':
        {
          intmax_t w;
          strtol_error s_err = xstrtoimax (optarg, عدم, 10, &w, "");
          إذا (LONGINT_حد_کان_مٿي < s_err || w < 0)
            error (EXIT_FAILURE, 0, "%s: %s",
                   _("invalid wrap size"), quote (optarg));
          wrap_ڪالم = s_err == LONGINT_حد_کان_مٿي || IDX_MAX < w ? 0 : w;
        }
        اكسر;

      حالة 'i':
        ignore_garbage = true;
        اكسر;

#إذا بنياد_TYPE == 42
      حالة BASE64_OPTION:
      حالة BASE64URL_OPTION:
      حالة BASE32_OPTION:
      حالة BASE32HEX_OPTION:
      حالة BASE16_OPTION:
      حالة BASE2MSBF_OPTION:
      حالة BASE2LSBF_OPTION:
      حالة Z85_OPTION:
      حالة BASE58_OPTION:
        بنياد_type = opt;
        اكسر;
#جيڪڏهن_پورو

      case_GETOPT_HELP_CHAR;

      case_GETOPT_نسخو_CHAR (پروگرام_NAME, AUTHORS);

      افتراضي:
        usage (EXIT_FAILURE);
        اكسر;
      }

#إذا بنياد_TYPE == 42
  required_padding = no_required_padding;
  has_padding = no_padding;
  get_pending_ڊيگهائي = no_pending_ڊيگهائي;
  بنياد_decode_ctx_finalize = decode_ctx_finalize;

  بدل (بنياد_type)
    {
    حالة BASE64_OPTION:
      بنياد_ڊيگهائي_2 = base64_ڊيگهائي_wrapper;
      required_padding = base64_required_padding;
      has_padding = base64_ctx_has_padding;
      get_pending_ڊيگهائي = base64_ctx_get_pending_ڊيگهائي;
      isubase = isubase64;
      بنياد_encode = base64_encode;
      بنياد_decode_ctx_init = base64_decode_ctx_init_wrapper;
      بنياد_decode_ctx = base64_decode_ctx_wrapper;
      اكسر;

    حالة BASE64URL_OPTION:
      بنياد_ڊيگهائي_2 = base64_ڊيگهائي_wrapper;
      required_padding = base64_required_padding;
      has_padding = base64_ctx_has_padding;
      get_pending_ڊيگهائي = base64_ctx_get_pending_ڊيگهائي;
      isubase = isubase64url;
      بنياد_encode = base64url_encode;
      بنياد_decode_ctx_init = base64url_decode_ctx_init_wrapper;
      بنياد_decode_ctx = base64url_decode_ctx_wrapper;
      اكسر;

    حالة BASE32_OPTION:
      بنياد_ڊيگهائي_2 = base32_ڊيگهائي_wrapper;
      required_padding = base32_required_padding;
      has_padding = base32_ctx_has_padding;
      get_pending_ڊيگهائي = base32_ctx_get_pending_ڊيگهائي;
      isubase = isubase32;
      بنياد_encode = base32_encode;
      بنياد_decode_ctx_init = base32_decode_ctx_init_wrapper;
      بنياد_decode_ctx = base32_decode_ctx_wrapper;
      اكسر;

    حالة BASE32HEX_OPTION:
      بنياد_ڊيگهائي_2 = base32_ڊيگهائي_wrapper;
      required_padding = base32_required_padding;
      has_padding = base32_ctx_has_padding;
      get_pending_ڊيگهائي = base32_ctx_get_pending_ڊيگهائي;
      isubase = isubase32hex;
      بنياد_encode = base32hex_encode;
      بنياد_decode_ctx_init = base32hex_decode_ctx_init_wrapper;
      بنياد_decode_ctx = base32hex_decode_ctx_wrapper;
      اكسر;

    حالة BASE16_OPTION:
      بنياد_ڊيگهائي_2 = base16_ڊيگهائي;
      get_pending_ڊيگهائي = base16_ctx_get_pending_ڊيگهائي;
      isubase = isubase16;
      بنياد_encode = base16_encode;
      بنياد_decode_ctx_init = base16_decode_ctx_init;
      بنياد_decode_ctx = base16_decode_ctx;
      اكسر;

    حالة BASE2MSBF_OPTION:
      بنياد_ڊيگهائي_2 = base2_ڊيگهائي;
      get_pending_ڊيگهائي = base2_ctx_get_pending_ڊيگهائي;
      isubase = isubase2;
      بنياد_encode = base2msbf_encode;
      بنياد_decode_ctx_init = base2_decode_ctx_init;
      بنياد_decode_ctx = base2msbf_decode_ctx;
      اكسر;

    حالة BASE2LSBF_OPTION:
      بنياد_ڊيگهائي_2 = base2_ڊيگهائي;
      get_pending_ڊيگهائي = base2_ctx_get_pending_ڊيگهائي;
      isubase = isubase2;
      بنياد_encode = base2lsbf_encode;
      بنياد_decode_ctx_init = base2_decode_ctx_init;
      بنياد_decode_ctx = base2lsbf_decode_ctx;
      اكسر;

    حالة Z85_OPTION:
      بنياد_ڊيگهائي_2 = z85_ڊيگهائي;
      get_pending_ڊيگهائي = z85_ctx_get_pending_ڊيگهائي;
      isubase = isuz85;
      بنياد_encode = z85_encode;
      بنياد_decode_ctx_init = z85_decode_ctx_init;
      بنياد_decode_ctx = z85_decode_ctx;
      اكسر;

    حالة BASE58_OPTION:
      بنياد_ڊيگهائي_2 = base58_ڊيگهائي;
      isubase = isubase58;
      بنياد_encode_ctx_init = base58_encode_ctx_init;
      بنياد_encode_ctx = base58_encode_ctx;
      بنياد_encode_ctx_finalize = base58_encode_ctx_finalize;
      بنياد_decode_ctx_init = base58_decode_ctx_init;
      بنياد_decode_ctx = base58_decode_ctx;
      بنياد_decode_ctx_finalize = base58_decode_ctx_finalize;
      اكسر;

    افتراضي:
      error (0, 0, _("missing encoding type"));
      usage (EXIT_FAILURE);
    }
#جيڪڏهن_پورو

  إذا (argc - optind > 1)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }

  إذا (optind < argc)
    infile = argv[optind];
  وإلا
    infile = "-";

  إذا (streq (infile, "-"))
    {
      xset_ٻٽو_گهڻائي (STDIN_FILENO, O_ٻٽو);
      داخلا_fh = stdin;
    }
  وإلا
    {
      داخلا_fh = fopen (infile, "rb");
      إذا (داخلا_fh == عدم)
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
    }

  fadvise (داخلا_fh, FADVISE_SEQUENTIAL);

  إذا (decode)
    do_decode (داخلا_fh, infile, stdout, ignore_garbage);
  وإلا
    do_encode (داخلا_fh, infile, stdout, wrap_ڪالم);
}
