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

#అన్తర్భావ <config.h>

#అన్తర్భావ <stdio.h>
#అన్తర్భావ <getopt.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <gmp.h>

#అన్తర్భావ "system.h"
#అన్తర్భావ "assure.h"
#అన్తర్భావ "c-ctype.h"
#అన్తర్భావ "fadvise.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "xstrtol.h"
#అన్తర్భావ "xbinary-io.h"

#ఒకవేళ పునాది_TYPE == 42
# define AUTHORS \
  proper_name ("Simon Josefsson"), \
  proper_name ("Assaf Gordon")
#లేకపోతే
# define AUTHORS proper_name ("Simon Josefsson")
#అయితే_ముగింపు

#ఒకవేళ పునాది_TYPE == 32
# include "base32.h"
# define కార్యక్రమం_NAME "base32"
#లేదావేళ పునాది_TYPE == 64
# include "base64.h"
# define కార్యక్రమం_NAME "base64"
#లేదావేళ పునాది_TYPE == 42
# include "base32.h"
# include "base64.h"
# include "assure.h"
# define కార్యక్రమం_NAME "basenc"
#లేకపోతే
# error missing/invalid పునాది_TYPE definition
#అయితే_ముగింపు



#ఒకవేళ పునాది_TYPE == 42
గణన
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
#అయితే_ముగింపు

స్థిర నిర్మాణం option స్థిరరాశి long_options[] =
{
  {"decode", no_వాదం, ఖాళీ, 'd'},
  {"wrap", required_వాదం, ఖాళీ, 'w'},
  {"ignore-garbage", no_వాదం, ఖాళీ, 'i'},
#ఒకవేళ పునాది_TYPE == 42
  {"base64",    no_వాదం, ఖాళీ, BASE64_OPTION},
  {"base64url", no_వాదం, ఖాళీ, BASE64URL_OPTION},
  {"base58",    no_వాదం, ఖాళీ, BASE58_OPTION},
  {"base32",    no_వాదం, ఖాళీ, BASE32_OPTION},
  {"base32hex", no_వాదం, ఖాళీ, BASE32HEX_OPTION},
  {"base16",    no_వాదం, ఖాళీ, BASE16_OPTION},
  {"base2msbf", no_వాదం, ఖాళీ, BASE2MSBF_OPTION},
  {"base2lsbf", no_వాదం, ఖాళీ, BASE2LSBF_OPTION},
  {"z85",       no_వాదం, ఖాళీ, Z85_OPTION},
#అయితే_ముగింపు
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0}
};

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("\
Usage: %s [OPTION]... [FILE]\n\
"), కార్యక్రమం_name);

#ఒకవేళ పునాది_TYPE == 42
      fputs (_("\
basenc encode or decode FILE, or standard ఆదానం, గమ్యం standard output.\n\
"), stdout);
#లేకపోతే
      రాయి (_("\
పునాది%d encode or decode FILE, or standard ఆదానం, గమ్యం standard output.\n\
"), పునాది_TYPE);
#అయితే_ముగింపు

      emit_stdin_note ();
      emit_mandatory_arg_note ();
#ఒకవేళ పునాది_TYPE == 42
      oputs (_("\
      --base64\n\
         same as base64(1) కార్యక్రమం (RFC4648 section 4)\n\
"));
      oputs (_("\
      --base64url\n\
         file- and url-సురక్షిత base64 (RFC4648 section 5)\n\
"));
      oputs (_("\
      --base58\n\
         visually unambiguous base58 encoding\n\
"));
      oputs (_("\
      --base32\n\
         same as base32(1) కార్యక్రమం (RFC4648 section 6)\n\
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
         బిట్టు string with most significant బిట్టు (msb) మొదటి\n\
"));
      oputs (_("\
      --base2lsbf\n\
         బిట్టు string with కనిష్ఠ significant బిట్టు (lsb) మొదటి\n\
"));
#అయితే_ముగింపు
      oputs (_("\
  -d, --decode\n\
         decode దత్తాంశం\n\
"));
      oputs (_("\
  -i, --ignore-garbage\n\
         when decoding, ignore non-alphabet characters\n\
"));
      oputs (_("\
  -w, --wrap=నిలువుల_సంఖ్య\n\
         wrap encoded పంక్తులు after నిలువుల_సంఖ్య character (అప్రమేయ 76).\n\
         Use 0 గమ్యం disable పంక్తి wrapping\n\
"));
#ఒకవేళ పునాది_TYPE == 42
      oputs (_("\
      --z85\n\
         ascii85-like encoding (ZeroMQ spec:32/Z85);\n\
         when encoding, ఆదానం నిడివి must be a గుణిజం యొక్క 4;\n\
         when decoding, ఆదానం నిడివి must be a గుణిజం యొక్క 5\n\
"));
#అయితే_ముగింపు
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
#ఒకవేళ పునాది_TYPE == 42
      fputs (_("\
\n\
When decoding, the ఆదానం may contain newlines లోపల addition గమ్యం the బైట్లు యొక్క\n\
the formal alphabet.  Use --ignore-garbage గమ్యం attempt గమ్యం recover\n\
from any other non-alphabet బైట్లు లోపల the encoded ప్రవాహం.\n\
"), stdout);
#లేకపోతే
      రాయి (_("\
\n\
The దత్తాంశం are encoded as described కొరకు the %s alphabet లోపల RFC 4648.\n\
When decoding, the ఆదానం may contain newlines లోపల addition గమ్యం the బైట్లు యొక్క\n\
the formal %s alphabet.  Use --ignore-garbage గమ్యం attempt గమ్యం recover\n\
from any other non-alphabet బైట్లు లోపల the encoded ప్రవాహం.\n"),
              కార్యక్రమం_NAME, కార్యక్రమం_NAME);
#అయితే_ముగింపు
      emit_ancillary_info (కార్యక్రమం_NAME);
    }

  నిష్క్రమించు (స్థితి_2);
}

#ఒకవేళ పునాది_TYPE != 64
స్థిర పూర్ణాంకం
base32_required_padding (పూర్ణాంకం len)
{
  పూర్ణాంకం partial = len % 8;
  తిరిగి partial ? 8 - partial : 0;
}
#అయితే_ముగింపు

#ఒకవేళ పునాది_TYPE != 32
స్థిర పూర్ణాంకం
base64_required_padding (పూర్ణాంకం len)
{
  పూర్ణాంకం partial = len % 4;
  తిరిగి partial ? 4 - partial : 0;
}
#అయితే_ముగింపు

#ఒకవేళ పునాది_TYPE == 42
స్థిర పూర్ణాంకం
no_required_padding (MAYBE_వాడని పూర్ణాంకం len)
{
  తిరిగి 0;
}
#అయితే_ముగింపు

#నిర్వచించు ENC_BLOCKSIZE (1024 * 3 * 10)

#ఒకవేళ పునాది_TYPE == 32
# define పునాది_నిడివి BASE32_నిడివి
# define REQUIRED_PADDING base32_required_padding
/* Note that increasing this may decrease performance if --ignore-garbage
   is used, because of the memmove operation below.  */
# define DEC_BLOCKSIZE (1024 * 5)

/* Ensure that BLOCKSIZE is a multiple of 5 and 8.  */
static_assert (ENC_BLOCKSIZE % 40 == 0); /* Padding chars only on last block. */
static_assert (DEC_BLOCKSIZE % 40 == 0); /* Complete encoded blocks are used. */

# define పునాది_encode base32_encode
# define పునాది_decode_context base32_decode_context
# define పునాది_decode_ctx_init base32_decode_ctx_init
# define పునాది_decode_ctx base32_decode_ctx
# define పునాది_decode_ctx_finalize decode_ctx_finalize
# define isubase isubase32
#లేదావేళ పునాది_TYPE == 64
# define పునాది_నిడివి BASE64_నిడివి
# define REQUIRED_PADDING base64_required_padding
/* Note that increasing this may decrease performance if --ignore-garbage
   is used, because of the memmove operation below.  */
# define DEC_BLOCKSIZE (1024 * 3)

/* Ensure that BLOCKSIZE is a multiple of 3 and 4.  */
static_assert (ENC_BLOCKSIZE % 12 == 0); /* Padding chars only on last block. */
static_assert (DEC_BLOCKSIZE % 12 == 0); /* Complete encoded blocks are used. */

# define పునాది_encode base64_encode
# define పునాది_decode_context base64_decode_context
# define పునాది_decode_ctx_init base64_decode_ctx_init
# define పునాది_decode_ctx base64_decode_ctx
# define పునాది_decode_ctx_finalize decode_ctx_finalize
# define isubase isubase64
#లేదావేళ పునాది_TYPE == 42


# define పునాది_నిడివి పునాది_నిడివి_2
# define REQUIRED_PADDING required_padding

/* Note that increasing this may decrease performance if --ignore-garbage
   is used, because of the memmove operation below.  */
# define DEC_BLOCKSIZE (4200)
static_assert (DEC_BLOCKSIZE % 40 == 0); /* complete encoded blocks for base32*/
static_assert (DEC_BLOCKSIZE % 12 == 0); /* complete encoded blocks for base64*/

స్థిర idx_t (*పునాది_నిడివి_2) (idx_t len);
స్థిర పూర్ణాంకం (*required_padding) (పూర్ణాంకం i);
స్థిర bool (*isubase) (గుర్తులేని అక్షరం ch);
స్థిర శూన్య (*పునాది_encode) (అక్షరం స్థిరరాశి *పరిమిత లోపల, idx_t inlen,
                            అక్షరం *పరిమిత బహిర్గతం, idx_t outlen);

నిర్మాణం base16_decode_context
{
  /* Either a 4-bit nibble, or negative if we have no nibble.  */
  గుర్తుగల అక్షరం nibble;
};

నిర్మాణం z85_decode_context
{
  పూర్ణాంకం i;
  గుర్తులేని అక్షరం octets[5];
};

నిర్మాణం base58_context
{
  idx_t కొలత;
  idx_t సామర్థ్యం;
  గుర్తులేని అక్షరం *buf COUNTED_BY (సామర్థ్యం);
};

నిర్మాణం base2_decode_context
{
  గుర్తులేని అక్షరం octet;
  పూర్ణాంకం బిట్టు_pos;
};

నిర్మాణం పునాది_decode_context
{
  సంఘం {
    నిర్మాణం base64_decode_context base64;
    నిర్మాణం base32_decode_context base32;
    నిర్మాణం base16_decode_context base16;
    నిర్మాణం base2_decode_context base2;
    నిర్మాణం z85_decode_context z85;
    నిర్మాణం base58_context base58;
  } ctx;
  అక్షరం *inbuf;
  idx_t bufsize;
};
స్థిర శూన్య (*పునాది_decode_ctx_init) (నిర్మాణం పునాది_decode_context *ctx);
స్థిర bool (*పునాది_decode_ctx) (నిర్మాణం పునాది_decode_context *ctx,
                                అక్షరం స్థిరరాశి *పరిమిత లోపల, idx_t inlen,
                                అక్షరం *పరిమిత బహిర్గతం, idx_t *outlen);
స్థిర bool (*పునాది_decode_ctx_finalize) (నిర్మాణం పునాది_decode_context *ctx,
                                         అక్షరం *పరిమిత *బహిర్గతం, idx_t *outlen);

నిర్మాణం పునాది_encode_context
{
  సంఘం {
    నిర్మాణం base58_context base58;
  } ctx;
};

స్థిర శూన్య (*పునాది_encode_ctx_init) (నిర్మాణం పునాది_encode_context *ctx);
స్థిర bool (*పునాది_encode_ctx) (నిర్మాణం పునాది_encode_context *ctx,
                                అక్షరం స్థిరరాశి *పరిమిత లోపల, idx_t inlen,
                                అక్షరం *పరిమిత బహిర్గతం, idx_t *outlen);
స్థిర bool (*పునాది_encode_ctx_finalize) (నిర్మాణం పునాది_encode_context *ctx,
                                         అక్షరం *పరిమిత *బహిర్గతం, idx_t *outlen);

స్థిర bool
no_padding (MAYBE_వాడని నిర్మాణం పునాది_decode_context *ctx)
{
  తిరిగి false;
}

స్థిర పూర్ణాంకం
no_pending_నిడివి (MAYBE_వాడని నిర్మాణం పునాది_decode_context *ctx)
{
  తిరిగి 0;
}
#అయితే_ముగింపు

#ఒకవేళ పునాది_TYPE == 42
స్థిర bool (*has_padding) (నిర్మాణం పునాది_decode_context *ctx);
స్థిర పూర్ణాంకం (*get_pending_నిడివి) (నిర్మాణం పునాది_decode_context *ctx);

స్థిర bool
base64_ctx_has_padding (నిర్మాణం పునాది_decode_context *ctx)
{
  తిరిగి ctx->ctx.base64.i && ctx->ctx.base64.buf[ctx->ctx.base64.i - 1] == '=';
}

స్థిర bool
base32_ctx_has_padding (నిర్మాణం పునాది_decode_context *ctx)
{
  తిరిగి ctx->ctx.base32.i && ctx->ctx.base32.buf[ctx->ctx.base32.i - 1] == '=';
}

స్థిర పూర్ణాంకం
base64_ctx_get_pending_నిడివి (నిర్మాణం పునాది_decode_context *ctx)
{
  తిరిగి ctx->ctx.base64.i;
}

స్థిర పూర్ణాంకం
base32_ctx_get_pending_నిడివి (నిర్మాణం పునాది_decode_context *ctx)
{
  తిరిగి ctx->ctx.base32.i;
}

స్థిర పూర్ణాంకం
base16_ctx_get_pending_నిడివి (MAYBE_వాడని నిర్మాణం పునాది_decode_context *ctx)
{
  తిరిగి 1; /* Always check nibble state.  */
}

స్థిర పూర్ణాంకం
z85_ctx_get_pending_నిడివి (నిర్మాణం పునాది_decode_context *ctx)
{
  తిరిగి ctx->ctx.z85.i;
}

స్థిర పూర్ణాంకం
base2_ctx_get_pending_నిడివి (నిర్మాణం పునాది_decode_context *ctx)
{
  తిరిగి ctx->ctx.base2.బిట్టు_pos;
}
#లేకపోతే
స్థిర bool
has_padding (నిర్మాణం పునాది_decode_context *ctx)
{
  తిరిగి ctx->i && ctx->buf[ctx->i - 1] == '=';
}

స్థిర పూర్ణాంకం
get_pending_నిడివి (నిర్మాణం పునాది_decode_context *ctx)
{
  తిరిగి ctx->i;
}
#అయితే_ముగింపు


/* Process any pending data in CTX, while auto padding if appropriate.
   Return TRUE on success, FALSE on failure.  */

స్థిర bool
decode_ctx_finalize (నిర్మాణం పునాది_decode_context *ctx,
                     అక్షరం *పరిమిత *బహిర్గతం, idx_t *outlen)
{
  ఒకవేళ (get_pending_నిడివి (ctx) == 0)
    {
      *outlen = 0;
      తిరిగి true;
    }

  /* Auto-pad input and flush the context */
  అక్షరం padbuf[8] ATTRIBUTE_NONSTRING = "========";
  idx_t pending_len = get_pending_నిడివి (ctx);
  idx_t auto_padding = REQUIRED_PADDING (pending_len);
  idx_t n = *outlen;
  bool ఫలితం;

  ఒకవేళ (auto_padding && ! has_padding (ctx))
    {
      affirm (auto_padding <= పరిమాణం (padbuf));
      ఫలితం = పునాది_decode_ctx (ctx, padbuf, auto_padding, *బహిర్గతం, &n);
    }
  లేకపోతే
    {
      ఫలితం = పునాది_decode_ctx (ctx, "", 0, *బహిర్గతం, &n);
    }

  *outlen = n;
  తిరిగి ఫలితం;
}

#ఒకవేళ పునాది_TYPE == 42

స్థిర idx_t
base64_నిడివి_wrapper (idx_t len)
{
  తిరిగి BASE64_నిడివి (len);
}

స్థిర శూన్య
base64_decode_ctx_init_wrapper (నిర్మాణం పునాది_decode_context *ctx)
{
  base64_decode_ctx_init (&ctx->ctx.base64);
}

స్థిర bool
base64_decode_ctx_wrapper (నిర్మాణం పునాది_decode_context *ctx,
                           అక్షరం స్థిరరాశి *పరిమిత లోపల, idx_t inlen,
                           అక్షరం *పరిమిత బహిర్గతం, idx_t *outlen)
{
  తిరిగి base64_decode_ctx (&ctx->ctx.base64, లోపల, inlen, బహిర్గతం, outlen);
}

స్థిర శూన్య
init_inbuf (నిర్మాణం పునాది_decode_context *ctx)
{
  ctx->bufsize = DEC_BLOCKSIZE;
  ctx->inbuf = xcharalloc (ctx->bufsize);
}

స్థిర శూన్య
prepare_inbuf (నిర్మాణం పునాది_decode_context *ctx, idx_t inlen)
{
  ఒకవేళ (ctx->bufsize < inlen)
    ctx->inbuf = xpalloc (ctx->inbuf, &ctx->bufsize,
                          inlen - ctx->bufsize, -1, పరిమాణం *ctx->inbuf);
}


స్థిర శూన్య
base64url_encode (అక్షరం స్థిరరాశి *పరిమిత లోపల, idx_t inlen,
                  అక్షరం *పరిమిత బహిర్గతం, idx_t outlen)
{
  base64_encode (లోపల, inlen, బహిర్గతం, outlen);
  /* translate 62nd and 63rd characters */
  అక్షరం *p = బహిర్గతం;
  అయితే (outlen--)
    {
      ఒకవేళ (*p == '+')
        *p = '-';
      లేకపోతే ఒకవేళ (*p == '/')
        *p = '_';
      ++p;
    }
}

స్థిర bool
isubase64url (గుర్తులేని అక్షరం ch)
{
  తిరిగి (ch == '-' || ch == '_'
          || (ch != '+' && ch != '/' && isubase64 (ch)));
}

స్థిర శూన్య
base64url_decode_ctx_init_wrapper (నిర్మాణం పునాది_decode_context *ctx)
{
  base64_decode_ctx_init (&ctx->ctx.base64);
  init_inbuf (ctx);
}


స్థిర bool
base64url_decode_ctx_wrapper (నిర్మాణం పునాది_decode_context *ctx,
                              అక్షరం స్థిరరాశి *పరిమిత లోపల, idx_t inlen,
                              అక్షరం *పరిమిత బహిర్గతం, idx_t *outlen)
{
  prepare_inbuf (ctx, inlen);
  జ్ఞాపకం_నకలు (ctx->inbuf, లోపల, inlen);

  /* translate 62nd and 63rd characters */
  idx_t i = inlen;
  అక్షరం *p = ctx->inbuf;
  అయితే (i--)
    {
      ఒకవేళ (*p == '+' || *p == '/')
        {
          *outlen = 0;
          తిరిగి false; /* reject base64 input */
        }
      లేకపోతే ఒకవేళ (*p == '-')
        *p = '+';
      లేకపోతే ఒకవేళ (*p == '_')
        *p = '/';
      ++p;
    }

  తిరిగి base64_decode_ctx (&ctx->ctx.base64, ctx->inbuf, inlen,
                            బహిర్గతం, outlen);
}



స్థిర idx_t
base32_నిడివి_wrapper (idx_t len)
{
  తిరిగి BASE32_నిడివి (len);
}

స్థిర శూన్య
base32_decode_ctx_init_wrapper (నిర్మాణం పునాది_decode_context *ctx)
{
  base32_decode_ctx_init (&ctx->ctx.base32);
}

స్థిర bool
base32_decode_ctx_wrapper (నిర్మాణం పునాది_decode_context *ctx,
                           అక్షరం స్థిరరాశి *పరిమిత లోపల, idx_t inlen,
                           అక్షరం *పరిమిత బహిర్గతం, idx_t *outlen)
{
  తిరిగి base32_decode_ctx (&ctx->ctx.base32, లోపల, inlen, బహిర్గతం, outlen);
}

/* ABCDEFGHIJKLMNOPQRSTUVWXYZ234567
గమ్యం
   0123456789ABCDEFGHIJKLMNOPQRSTUV */
స్థిర స్థిరరాశి అక్షరం base32_norm_గమ్యం_hex[32 + 9] = {
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
గమ్యం
   ABCDEFGHIJKLMNOPQRSTUVWXYZ234567 */
స్థిర స్థిరరాశి అక్షరం base32_hex_గమ్యం_norm[32 + 9] = {
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


అంతఃవరుస స్థిర bool
isubase32hex (గుర్తులేని అక్షరం ch)
{
  తిరిగి ('0' <= ch && ch <= '9') || ('A' <= ch && ch <= 'V');
}


స్థిర శూన్య
base32hex_encode (అక్షరం స్థిరరాశి *పరిమిత లోపల, idx_t inlen,
                  అక్షరం *పరిమిత బహిర్గతం, idx_t outlen)
{
  base32_encode (లోపల, inlen, బహిర్గతం, outlen);

  కొరకు (అక్షరం *p = బహిర్గతం; outlen--; p++)
    {
      affirm (0x32 <= *p && *p <= 0x5a);          /* LCOV_EXCL_LINE */
      *p = base32_norm_గమ్యం_hex[*p - 0x32];
    }
}


స్థిర శూన్య
base32hex_decode_ctx_init_wrapper (నిర్మాణం పునాది_decode_context *ctx)
{
  base32_decode_ctx_init (&ctx->ctx.base32);
  init_inbuf (ctx);
}


స్థిర bool
base32hex_decode_ctx_wrapper (నిర్మాణం పునాది_decode_context *ctx,
                              అక్షరం స్థిరరాశి *పరిమిత లోపల, idx_t inlen,
                              అక్షరం *పరిమిత బహిర్గతం, idx_t *outlen)
{
  prepare_inbuf (ctx, inlen);

  idx_t i = inlen;
  అక్షరం *p = ctx->inbuf;
  అయితే (i--)
    {
      ఒకవేళ (isubase32hex (*లోపల))
        *p = base32_hex_గమ్యం_norm[*లోపల - 0x30];
      లేకపోతే
        *p = *లోపల;
      ++p;
      ++లోపల;
    }

  తిరిగి base32_decode_ctx (&ctx->ctx.base32, ctx->inbuf, inlen,
                            బహిర్గతం, outlen);
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

స్థిర గుర్తుగల అక్షరం స్థిరరాశి base16_గమ్యం_int[256] = {
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

స్థిర bool
isubase16 (గుర్తులేని అక్షరం ch)
{
  తిరిగి ch < పరిమాణం base16_గమ్యం_int && 0 <= base16_గమ్యం_int[ch];
}

స్థిర idx_t
base16_నిడివి (idx_t len)
{
  తిరిగి len * 2;
}


స్థిర శూన్య
base16_encode (అక్షరం స్థిరరాశి *పరిమిత లోపల, idx_t inlen,
               అక్షరం *పరిమిత బహిర్గతం, idx_t outlen)
{
  స్థిర స్థిరరాశి అక్షరం base16[16] ATTRIBUTE_NONSTRING = "0123456789ABCDEF";

  అయితే (inlen && outlen)
    {
      గుర్తులేని అక్షరం c = *లోపల;
      *బహిర్గతం++ = base16[c >> 4];
      *బహిర్గతం++ = base16[c & 0x0F];
      ++లోపల;
      inlen--;
      outlen -= 2;
    }
}


స్థిర శూన్య
base16_decode_ctx_init (నిర్మాణం పునాది_decode_context *ctx)
{
  init_inbuf (ctx);
  ctx->ctx.base16.nibble = -1;
}


స్థిర bool
base16_decode_ctx (నిర్మాణం పునాది_decode_context *ctx,
                   అక్షరం స్థిరరాశి *పరిమిత లోపల, idx_t inlen,
                   అక్షరం *పరిమిత బహిర్గతం, idx_t *outlen)
{
  bool ignore_పంక్తులు = true;  /* for now, always ignore them */
  అక్షరం *out0 = బహిర్గతం;
  గుర్తుగల అక్షరం nibble = ctx->ctx.base16.nibble;

  /* inlen==0 is request to flush output.
     if there is a dangling high nibble - we are missing the low nibble,
     so return false - indicating an invalid input.  */
  ఒకవేళ (inlen == 0)
    {
      *outlen = 0;
      తిరిగి nibble < 0;
    }

  అయితే (inlen--)
    {
      గుర్తులేని అక్షరం c = *లోపల++;
      ఒకవేళ (ignore_పంక్తులు && c == '\n')
        కొనసాగు;

      ఒకవేళ (పరిమాణం base16_గమ్యం_int <= c || base16_గమ్యం_int[c] < 0)
        {
          *outlen = బహిర్గతం - out0;
          తిరిగి false; /* garbage - return false */
        }

      ఒకవేళ (nibble < 0)
        nibble = base16_గమ్యం_int[c];
      లేకపోతే
        {
          /* have both nibbles, write octet */
          *బహిర్గతం++ = (nibble << 4) + base16_గమ్యం_int[c];
          nibble = -1;
        }
    }

  ctx->ctx.base16.nibble = nibble;
  *outlen = బహిర్గతం - out0;
  తిరిగి true;
}



ATTRIBUTE_శుద్ధ
స్థిర idx_t
z85_నిడివి (idx_t len)
{
  /* Z85 does not allow padding, so no need to round to highest integer.  */
  idx_t z85_len = (len * 5) / 4;
  affirm (0 <= z85_len);
  తిరిగి z85_len;
}

స్థిర bool
isuz85 (గుర్తులేని అక్షరం ch)
{
  తిరిగి c_isalnum (ch) || strchr (".-:+=^!/*?&<>()[]{}@%$#", ch) != ఖాళీ;
}

స్థిర అక్షరం స్థిరరాశి z85_encoding[85] ATTRIBUTE_NONSTRING =
  "0123456789"
  "abcdefghijklmnopqrstuvwxyz"
  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
  ".-:+=^!/*?&<>()[]{}@%$#";

స్థిర శూన్య
z85_encode (అక్షరం స్థిరరాశి *పరిమిత లోపల, idx_t inlen,
            అక్షరం *పరిమిత బహిర్గతం, idx_t outlen)
{
  పూర్ణాంకం i = 0;
  గుర్తులేని అక్షరం quad[4];
  idx_t outidx = 0;

  అయితే (true)
    {
      ఒకవేళ (inlen == 0)
        {
          /* no more input, exactly on 4 octet boundary. */
          ఒకవేళ (i == 0)
            తిరిగి;

          /* currently, there's no way to return an error in encoding.  */
          error (EXIT_FAILURE, 0,
                 _("invalid input (length must be multiple of 4 characters)"));
        }
      లేకపోతే
        {
          quad[i++] = *లోపల++;
          --inlen;
        }

      /* Got a quad, encode it */
      ఒకవేళ (i == 4)
        {
          int_fast64_t val = quad[0];
          val = (val << 24) + (quad[1] << 16) + (quad[2] << 8) + quad[3];

          కొరకు (పూర్ణాంకం j = 4; j >= 0; --j)
            {
              పూర్ణాంకం c = val % 85;
              val /= 85;

              /* NOTE: if there is padding (which is trimmed by z85
                 before outputting the result), the output buffer 'out'
                 might not include enough allocated bytes for the padding,
                 so don't store them. */
              ఒకవేళ (outidx + j < outlen)
                బహిర్గతం[j] = z85_encoding[c];
            }
          బహిర్గతం += 5;
          outidx += 5;
          i = 0;
        }
    }
}

స్థిర శూన్య
z85_decode_ctx_init (నిర్మాణం పునాది_decode_context *ctx)
{
  init_inbuf (ctx);
  ctx->ctx.z85.i = 0;
}


# define Z85_LO_CTX_గమ్యం_32_బిట్టు_VAL(ctx) \
  (((ctx)->ctx.z85.octets[1] * 85 * 85 * 85) +      \
   ((ctx)->ctx.z85.octets[2] * 85 * 85) +	    \
   ((ctx)->ctx.z85.octets[3] * 85) +		    \
   ((ctx)->ctx.z85.octets[4]))


# define Z85_HI_CTX_గమ్యం_32_బిట్టు_VAL(ctx) \
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
స్థిర గుర్తుగల అక్షరం స్థిరరాశి z85_decoding[93] = {
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

స్థిర bool
z85_decode_ctx (నిర్మాణం పునాది_decode_context *ctx,
                అక్షరం స్థిరరాశి *పరిమిత లోపల, idx_t inlen,
                అక్షరం *పరిమిత బహిర్గతం, idx_t *outlen)
{
  bool ignore_పంక్తులు = true;  /* for now, always ignore them */

  *outlen = 0;

  /* inlen==0 is request to flush output.
     if there are dangling values - we are missing entries,
     so return false - indicating an invalid input.  */
  ఒకవేళ (inlen == 0)
    {
      ఒకవేళ (ctx->ctx.z85.i > 0)
        {
          /* Z85 variant does not allow padding - input must
             be a multiple of 5 - so return error.  */
          తిరిగి false;
        }
      తిరిగి true;
    }

  అయితే (inlen--)
    {
      ఒకవేళ (ignore_పంక్తులు && *లోపల == '\n')
        {
          ++లోపల;
          కొనసాగు;
        }

      /* z85 decoding */
      గుర్తులేని అక్షరం c = *లోపల;

      ఒకవేళ (c >= 33 && c <= 125)
        {
          గుర్తుగల అక్షరం ch = z85_decoding[c - 33];
          ఒకవేళ (ch < 0)
            తిరిగి false; /* garbage - return false */
          c = ch;
        }
      లేకపోతే
        తిరిగి false; /* garbage - return false */

      ++లోపల;

      ctx->ctx.z85.octets[ctx->ctx.z85.i++] = c;
      ఒకవేళ (ctx->ctx.z85.i == 5)
        {
          /* decode the lowest 4 octets, then check for overflows.  */
          int_fast64_t val = Z85_LO_CTX_గమ్యం_32_బిట్టు_VAL (ctx);

          /* The Z85 spec and the reference implementation say nothing
             about overflows. To be on the safe side, reject them.  */

          val += Z85_HI_CTX_గమ్యం_32_బిట్టు_VAL (ctx);
          ఒకవేళ ((val >> 24) & ~0xFF)
            తిరిగి false;

          *బహిర్గతం++ = val >> 24;
          *బహిర్గతం++ = (val >> 16) & 0xFF;
          *బహిర్గతం++ = (val >> 8) & 0xFF;
          *బహిర్గతం++ = val & 0xFF;

          *outlen += 4;

          ctx->ctx.z85.i = 0;
        }
    }
  తిరిగి true;
}


అంతఃవరుస స్థిర bool
isubase2 (గుర్తులేని అక్షరం ch)
{
  తిరిగి ch == '0' || ch == '1';
}

స్థిర idx_t
base2_నిడివి (idx_t len)
{
  తిరిగి len * 8;
}


అంతఃవరుస స్థిర శూన్య
base2msbf_encode (అక్షరం స్థిరరాశి *పరిమిత లోపల, idx_t inlen,
                  అక్షరం *పరిమిత బహిర్గతం, idx_t outlen)
{
  అయితే (inlen && outlen)
    {
      గుర్తులేని అక్షరం c = *లోపల;
      కొరకు (పూర్ణాంకం i = 0; i < 8; i++)
        {
          *బహిర్గతం++ = c & 0x80 ? '1' : '0';
          c <<= 1;
        }
      inlen--;
      outlen -= 8;
      ++లోపల;
    }
}

అంతఃవరుస స్థిర శూన్య
base2lsbf_encode (అక్షరం స్థిరరాశి *పరిమిత లోపల, idx_t inlen,
                  అక్షరం *పరిమిత బహిర్గతం, idx_t outlen)
{
  అయితే (inlen && outlen)
    {
      గుర్తులేని అక్షరం c = *లోపల;
      కొరకు (పూర్ణాంకం i = 0; i < 8; i++)
        {
          *బహిర్గతం++ = c & 0x01 ? '1' : '0';
          c >>= 1;
        }
      inlen--;
      outlen -= 8;
      ++లోపల;
    }
}


స్థిర శూన్య
base2_decode_ctx_init (నిర్మాణం పునాది_decode_context *ctx)
{
  init_inbuf (ctx);
  ctx->ctx.base2.octet = 0;
  ctx->ctx.base2.బిట్టు_pos = 0;
}


స్థిర bool
base2lsbf_decode_ctx (నిర్మాణం పునాది_decode_context *ctx,
                      అక్షరం స్థిరరాశి *పరిమిత లోపల, idx_t inlen,
                      అక్షరం *పరిమిత బహిర్గతం, idx_t *outlen)
{
  bool ignore_పంక్తులు = true;  /* for now, always ignore them */

  *outlen = 0;

  /* inlen==0 is request to flush output.
     if there is a dangling bit - we are missing some bits,
     so return false - indicating an invalid input.  */
  ఒకవేళ (inlen == 0)
    తిరిగి ctx->ctx.base2.బిట్టు_pos == 0;

  అయితే (inlen--)
    {
      ఒకవేళ (ignore_పంక్తులు && *లోపల == '\n')
        {
          ++లోపల;
          కొనసాగు;
        }

      ఒకవేళ (!isubase2 (*లోపల))
        తిరిగి false;

      bool బిట్టు = (*లోపల == '1');
      ctx->ctx.base2.octet |= బిట్టు << ctx->ctx.base2.బిట్టు_pos;
      ++ctx->ctx.base2.బిట్టు_pos;

      ఒకవేళ (ctx->ctx.base2.బిట్టు_pos == 8)
        {
          *బహిర్గతం++ = ctx->ctx.base2.octet;
          ctx->ctx.base2.octet = 0;
          ++*outlen;
          ctx->ctx.base2.బిట్టు_pos = 0;
        }

      ++లోపల;
    }

  తిరిగి true;
}

స్థిర bool
base2msbf_decode_ctx (నిర్మాణం పునాది_decode_context *ctx,
                      అక్షరం స్థిరరాశి *పరిమిత లోపల, idx_t inlen,
                      అక్షరం *పరిమిత బహిర్గతం, idx_t *outlen)
{
  bool ignore_పంక్తులు = true;  /* for now, always ignore them */

  *outlen = 0;

  /* inlen==0 is request to flush output.
     if there is a dangling bit - we are missing some bits,
     so return false - indicating an invalid input.  */
  ఒకవేళ (inlen == 0)
    తిరిగి ctx->ctx.base2.బిట్టు_pos == 0;

  అయితే (inlen--)
    {
      ఒకవేళ (ignore_పంక్తులు && *లోపల == '\n')
        {
          ++లోపల;
          కొనసాగు;
        }

      ఒకవేళ (!isubase2 (*లోపల))
        తిరిగి false;

      bool బిట్టు = (*లోపల == '1');
      ఒకవేళ (ctx->ctx.base2.బిట్టు_pos == 0)
        ctx->ctx.base2.బిట్టు_pos = 8;
      --ctx->ctx.base2.బిట్టు_pos;
      ctx->ctx.base2.octet |= బిట్టు << ctx->ctx.base2.బిట్టు_pos;

      ఒకవేళ (ctx->ctx.base2.బిట్టు_pos == 0)
        {
          *బహిర్గతం++ = ctx->ctx.base2.octet;
          ctx->ctx.base2.octet = 0;
          ++*outlen;
        }

      ++లోపల;
    }

  తిరిగి true;
}

/* Map from GMP (up to base 62):
   "0123456789 ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuv";
   to base 58:
   "123456789A BCDEFGHJKLMNPQRSTUVWXYZabc defghijkmnopqrstuvwxyz";  */
స్థిర గుర్తుగల అక్షరం స్థిరరాశి gmp_గమ్యం_base58[256] = {
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

స్థిర గుర్తుగల అక్షరం స్థిరరాశి base58_గమ్యం_gmp[256] = {
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

స్థిర bool
isubase58 (గుర్తులేని అక్షరం ch)
{
  తిరిగి ch < పరిమాణం base58_గమ్యం_gmp && 0 <= base58_గమ్యం_gmp[ch];
}


ATTRIBUTE_శుద్ధ
స్థిర idx_t
base58_నిడివి (idx_t len)
{
  /* Base58 output length is approximately log(256)/log(58),
     which is approximately len * 138 / 100,
     which is at most ((len + 100 - 1) / 100) * 138
     +1 to ensure we've enough place for NUL  */
  idx_t base58_len = ((len + 99) / 100) * 138 + 1;
  affirm (0 < base58_len);
  తిరిగి base58_len;
}


స్థిర శూన్య
base58_encode_ctx_init (నిర్మాణం పునాది_encode_context *ctx)
{
  ctx->ctx.base58.buf = ఖాళీ;
  ctx->ctx.base58.కొలత = 0;
  ctx->ctx.base58.సామర్థ్యం = 0;
}


స్థిర bool
base58_encode_ctx (నిర్మాణం పునాది_encode_context *ctx,
                   అక్షరం స్థిరరాశి *పరిమిత లోపల, idx_t inlen,
                   MAYBE_వాడని అక్షరం *పరిమిత బహిర్గతం, idx_t *outlen)
{
  *outlen = 0;   /* Only accumulate input in this function.  */

  ఒకవేళ (inlen == 0)
    తిరిగి true;

  idx_t free_space = ctx->ctx.base58.సామర్థ్యం - ctx->ctx.base58.కొలత;
  ఒకవేళ (free_space < inlen)
    {
      ctx->ctx.base58.buf = xpalloc (ctx->ctx.base58.buf,
                                     &ctx->ctx.base58.సామర్థ్యం,
                                     inlen - free_space,
                                     -1, పరిమాణం *ctx->ctx.base58.buf);
    }

  జ్ఞాపకం_నకలు (ctx->ctx.base58.buf + ctx->ctx.base58.కొలత, లోపల, inlen);
  ctx->ctx.base58.కొలత += inlen;

  తిరిగి true;
}

స్థిర శూన్య
base58_encode (అక్షరం స్థిరరాశి *దత్తాంశం, size_t దత్తాంశం_len,
               అక్షరం *బహిర్గతం, idx_t *outlen)
{
  affirm (పునాది_నిడివి_2 (దత్తాంశం_len) <= *outlen);

  size_t zeros = 0;
  అయితే (zeros < దత్తాంశం_len && దత్తాంశం[zeros] == 0)
    zeros++;

  memset (బహిర్గతం, '1', zeros);
  అక్షరం *p = బహిర్గతం + zeros;

  /* Use GMP to convert from base 256 to base 58.  */
  mpz_t అంకె;
  mpz_init (అంకె);
  ఒకవేళ (దత్తాంశం_len - zeros)
    {
      mpz_import (అంకె, దత్తాంశం_len - zeros, 1, 1, 0, 0, దత్తాంశం + zeros);
      కొరకు (p = mpz_get_str (p, 58, అంకె); *p; p++)
        *p = gmp_గమ్యం_base58[గమ్యం_uchar (*p)];
    }
  mpz_clear (అంకె);

  *outlen = p - బహిర్గతం;
}


స్థిర bool
base58_encode_ctx_finalize (నిర్మాణం పునాది_encode_context *ctx,
                            అక్షరం *పరిమిత *బహిర్గతం, idx_t *outlen)
{
  /* Ensure output buffer is large enough.  */
  idx_t max_outlen = పునాది_నిడివి_2 (ctx->ctx.base58.కొలత);
  ఒకవేళ (max_outlen > *outlen)
    {
      *బహిర్గతం = xrealloc (*బహిర్గతం, max_outlen);
      *outlen = max_outlen;
    }

  base58_encode ((అక్షరం *)ctx->ctx.base58.buf, ctx->ctx.base58.కొలత,
                 *బహిర్గతం, outlen);

  విడుదల (ctx->ctx.base58.buf);
  ctx->ctx.base58.buf = ఖాళీ;
  ctx->ctx.base58.కొలత = 0;
  ctx->ctx.base58.సామర్థ్యం = 0;

  తిరిగి true;
}


స్థిర శూన్య
base58_decode_ctx_init (నిర్మాణం పునాది_decode_context *ctx)
{
  ctx->ctx.base58.కొలత = 0;
  ctx->ctx.base58.సామర్థ్యం = 0;
  ctx->ctx.base58.buf = ఖాళీ;
}

స్థిర bool
base58_decode_ctx (నిర్మాణం పునాది_decode_context *ctx,
                   అక్షరం స్థిరరాశి *పరిమిత లోపల, idx_t inlen,
                   MAYBE_వాడని అక్షరం *పరిమిత బహిర్గతం, idx_t *outlen)
{
  bool ignore_పంక్తులు = true;  /* for now, always ignore them */

  *outlen = 0;   /* Only accumulate input in this function.  */

  ఒకవేళ (inlen == 0)
    తిరిగి true;

  idx_t free_space = ctx->ctx.base58.సామర్థ్యం - ctx->ctx.base58.కొలత;
  free_space -= 1;  /* Ensure we leave space for NUL (for mpz_set_str).  */
  ఒకవేళ (free_space < inlen)
    {
      ctx->ctx.base58.buf = xpalloc (ctx->ctx.base58.buf,
                                     &ctx->ctx.base58.సామర్థ్యం,
                                     inlen - free_space,
                                     -1, పరిమాణం *ctx->ctx.base58.buf);
    }

  /* Accumulate all valid input characters in our buffer.
     Note we don't rely on mpz_set_str() for validation
     as that allows (skips) all whitespace.  */
  కొరకు (idx_t i = 0; i < inlen; i++)
    {
      గుర్తులేని అక్షరం c = లోపల[i];

      ఒకవేళ (ignore_పంక్తులు && c == '\n')
        కొనసాగు;

      ఒకవేళ (!isubase58 (c))
        తిరిగి false;

      ctx->ctx.base58.buf[ctx->ctx.base58.కొలత++] = base58_గమ్యం_gmp[గమ్యం_uchar (c)];
    }

  తిరిగి true;
}


స్థిర bool
base58_decode (అక్షరం స్థిరరాశి *దత్తాంశం, size_t దత్తాంశం_len,
               అక్షరం *పరిమిత బహిర్గతం, idx_t *outlen)
{
  affirm (దత్తాంశం_len <= *outlen);

  size_t ones = 0;
  అయితే (ones < దత్తాంశం_len && దత్తాంశం[ones] == base58_గమ్యం_gmp['1'])
    ones++;

  memset (బహిర్గతం, 0, ones);

  /* Use GMP to convert from base 58 to base 256.  */
  mpz_t అంకె;
  mpz_init (అంకె);

  ఒకవేళ ((దత్తాంశం_len - ones) && mpz_సమితి_str (అంకె, దత్తాంశం + ones, 58) != 0)
    {
      mpz_clear (అంకె);
      *outlen = 0;
      తిరిగి false;
    }

  size_t exported_కొలత = 0;
  ఒకవేళ (దత్తాంశం_len - ones)
    {
      size_t ద్విమాన_కొలత = (mpz_sizeinbase (అంకె, 2) + 7) / 8;
      affirm (*outlen - ones >= ద్విమాన_కొలత);
      mpz_export (బహిర్గతం + ones, &exported_కొలత, 1, 1, 0, 0, అంకె);
    }

  mpz_clear (అంకె);
  *outlen = ones + exported_కొలత;
  తిరిగి true;
}


స్థిర bool
base58_decode_ctx_finalize (నిర్మాణం పునాది_decode_context *ctx,
                            అక్షరం *పరిమిత *బహిర్గతం, idx_t *outlen)
{
  /* Ensure output buffer is large enough.
     Worst case is input is all '1's.  */
  idx_t max_outlen = ctx->ctx.base58.కొలత;
  ఒకవేళ (max_outlen > *outlen)
    {
      *బహిర్గతం = xrealloc (*బహిర్గతం, max_outlen);
      *outlen = max_outlen;
    }

  /* Ensure input buffer is NUL terminated (for mpz_get_str).  */
  ఒకవేళ (ctx->ctx.base58.కొలత)
    ctx->ctx.base58.buf[ctx->ctx.base58.కొలత] = '\0';

  bool ret = base58_decode ((అక్షరం *)ctx->ctx.base58.buf, ctx->ctx.base58.కొలత,
                            *బహిర్గతం, outlen);

  విడుదల (ctx->ctx.base58.buf);
  ctx->ctx.base58.buf = ఖాళీ;
  ctx->ctx.base58.కొలత = 0;
  ctx->ctx.base58.సామర్థ్యం = 0;

  తిరిగి ret;
}

#అయితే_ముగింపు /* BASE_TYPE == 42, i.e., "basenc"*/



స్థిర శూన్య
wrap_write (అక్షరం స్థిరరాశి *బఫరు, idx_t len,
            idx_t wrap_స్తంభం, idx_t *current_స్తంభం, FILE *బహిర్గతం)
{
  ఒకవేళ (wrap_స్తంభం == 0)
    {
      /* Simple write. */
      ఒకవేళ (fwrite (బఫరు, 1, len, stdout) < len)
        write_error ();
    }
  లేకపోతే
    కొరకు (idx_t written = 0; written < len; )
      {
        idx_t గమ్యం_write = MIN (wrap_స్తంభం - *current_స్తంభం, len - written);

        ఒకవేళ (గమ్యం_write == 0)
          {
            ఒకవేళ (fputc ('\n', బహిర్గతం) == EOF)
              write_error ();
            *current_స్తంభం = 0;
          }
        లేకపోతే
          {
            ఒకవేళ (fwrite (బఫరు + written, 1, గమ్యం_write, stdout) < గమ్యం_write)
              write_error ();
            *current_స్తంభం += గమ్యం_write;
            written += గమ్యం_write;
          }
      }
}

స్థిర _Noreturn శూన్య
ముగింపు_and_exit (FILE *లోపల, అక్షరం స్థిరరాశి *infile)
{
  ఒకవేళ (fclose (లోపల) != 0)
    {
      ఒకవేళ (streq (infile, "-"))
        error (EXIT_FAILURE, errno, _("closing standard input"));
      లేకపోతే
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
    }

  నిష్క్రమించు (EXIT_SUCCESS);
}

స్థిర _Noreturn శూన్య
do_encode (FILE *లోపల, అక్షరం స్థిరరాశి *infile, FILE *బహిర్గతం, idx_t wrap_స్తంభం)
{
  idx_t current_స్తంభం = 0;
  అక్షరం *inbuf, *outbuf;
  idx_t కూడిక;

  inbuf = xmalloc (ENC_BLOCKSIZE);
  outbuf = xmalloc (పునాది_నిడివి (ENC_BLOCKSIZE));

#ఒకవేళ పునాది_TYPE == 42
  /* Initialize encoding context if needed (for base58) */
  నిర్మాణం పునాది_encode_context encode_ctx;
  bool use_ctx = (పునాది_encode_ctx_init != ఖాళీ);
  ఒకవేళ (use_ctx)
    పునాది_encode_ctx_init (&encode_ctx);
#అయితే_ముగింపు

  చేయి
    {
      idx_t n;

      కూడిక = 0;
      చేయి
        {
          n = fread (inbuf + కూడిక, 1, ENC_BLOCKSIZE - కూడిక, లోపల);
          కూడిక += n;
        }
      అయితే (!feof (లోపల) && !ferror (లోపల) && కూడిక < ENC_BLOCKSIZE);

      ఒకవేళ (కూడిక > 0)
        {
#ఒకవేళ పునాది_TYPE == 42
          ఒకవేళ (use_ctx)
            {
              idx_t outlen = 0;
              పునాది_encode_ctx (&encode_ctx, inbuf, కూడిక, outbuf, &outlen);

              wrap_write (outbuf, outlen, wrap_స్తంభం, &current_స్తంభం, బహిర్గతం);
            }
          లేకపోతే
#అయితే_ముగింపు
            {
              /* Process input one block at a time.  Note that ENC_BLOCKSIZE
                 is sized so that no pad chars will appear in output. */
              పునాది_encode (inbuf, కూడిక, outbuf, పునాది_నిడివి (కూడిక));

              wrap_write (outbuf, పునాది_నిడివి (కూడిక), wrap_స్తంభం,
                          &current_స్తంభం, బహిర్గతం);
            }
        }
    }
  అయితే (!feof (లోపల) && !ferror (లోపల) && కూడిక == ENC_BLOCKSIZE);

#ఒకవేళ పునాది_TYPE == 42
  ఒకవేళ (use_ctx && పునాది_encode_ctx_finalize)
    {
      idx_t outlen = పునాది_నిడివి (ENC_BLOCKSIZE);
      పునాది_encode_ctx_finalize (&encode_ctx, &outbuf, &outlen);

      wrap_write (outbuf, outlen, wrap_స్తంభం, &current_స్తంభం, బహిర్గతం);
    }
#అయితే_ముగింపు

  /* When wrapping, terminate last line. */
  ఒకవేళ (wrap_స్తంభం && current_స్తంభం > 0 && fputc ('\n', బహిర్గతం) == EOF)
    write_error ();

  ఒకవేళ (ferror (లోపల))
    error (EXIT_FAILURE, errno, _("read error"));

  ముగింపు_and_exit (లోపల, infile);
}

స్థిర _Noreturn శూన్య
do_decode (FILE *లోపల, అక్షరం స్థిరరాశి *infile, FILE *బహిర్గతం, bool ignore_garbage)
{
  అక్షరం *inbuf, *outbuf;
  idx_t కూడిక;
  నిర్మాణం పునాది_decode_context ctx;

  inbuf = xmalloc (పునాది_నిడివి (DEC_BLOCKSIZE));
  outbuf = xmalloc (DEC_BLOCKSIZE);

#ఒకవేళ పునాది_TYPE == 42
  ctx.inbuf = ఖాళీ;
#అయితే_ముగింపు
  పునాది_decode_ctx_init (&ctx);

  చేయి
    {
      bool ok;

      కూడిక = 0;
      చేయి
        {
          idx_t n = fread (inbuf + కూడిక,
                           1, పునాది_నిడివి (DEC_BLOCKSIZE) - కూడిక, లోపల);

          ఒకవేళ (ignore_garbage)
            {
              కొరకు (idx_t i = 0; n > 0 && i < n;)
                {
                  ఒకవేళ (isubase (inbuf[కూడిక + i])
                      || (REQUIRED_PADDING (1) && inbuf[కూడిక + i] == '='))
                    i++;
                  లేకపోతే
                    memmove (inbuf + కూడిక + i, inbuf + కూడిక + i + 1, --n - i);
                }
            }

          కూడిక += n;

          ఒకవేళ (ferror (లోపల))
            error (EXIT_FAILURE, errno, _("read error"));
        }
      అయితే (కూడిక < పునాది_నిడివి (DEC_BLOCKSIZE) && !feof (లోపల));

      అయితే (కూడిక || feof (లోపల))
        {
          idx_t n = DEC_BLOCKSIZE;
          ఒకవేళ (కూడిక)
            ok = పునాది_decode_ctx (&ctx, inbuf, కూడిక, outbuf, &n);
          లేకపోతే
            ok = పునాది_decode_ctx_finalize (&ctx, &outbuf, &n);

          ఒకవేళ (fwrite (outbuf, 1, n, బహిర్గతం) < n)
            write_error ();

          ఒకవేళ (! ok)
            error (EXIT_FAILURE, 0, _("invalid input"));

          ఒకవేళ (కూడిక == 0)
            ఆపు;
          కూడిక = 0;
        }
    }
  అయితే (!feof (లోపల));

  ముగింపు_and_exit (లోపల, infile);
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  పూర్ణాంకం opt;
  FILE *ఆదానం_fh;
  అక్షరం స్థిరరాశి *infile;

  /* True if --decode has been given and we should decode data. */
  bool decode = false;
  /* True if we should ignore non-base-alphabetic characters. */
  bool ignore_garbage = false;
  /* Wrap encoded data around the 76th column, by default. */
  idx_t wrap_స్తంభం = 76;

#ఒకవేళ పునాది_TYPE == 42
  పూర్ణాంకం పునాది_type = 0;
#అయితే_ముగింపు

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  అయితే ((opt = getopt_long (argc, argv, "diw:", long_options, ఖాళీ)) != -1)
    ఎంపిక (opt)
      {
      స్థితి 'd':
        decode = true;
        ఆపు;

      స్థితి 'w':
        {
          intmax_t w;
          strtol_error s_err = xstrtoimax (optarg, ఖాళీ, 10, &w, "");
          ఒకవేళ (LONGINT_అధికప్రవాహం < s_err || w < 0)
            error (EXIT_FAILURE, 0, "%s: %s",
                   _("invalid wrap size"), quote (optarg));
          wrap_స్తంభం = s_err == LONGINT_అధికప్రవాహం || IDX_MAX < w ? 0 : w;
        }
        ఆపు;

      స్థితి 'i':
        ignore_garbage = true;
        ఆపు;

#ఒకవేళ పునాది_TYPE == 42
      స్థితి BASE64_OPTION:
      స్థితి BASE64URL_OPTION:
      స్థితి BASE32_OPTION:
      స్థితి BASE32HEX_OPTION:
      స్థితి BASE16_OPTION:
      స్థితి BASE2MSBF_OPTION:
      స్థితి BASE2LSBF_OPTION:
      స్థితి Z85_OPTION:
      స్థితి BASE58_OPTION:
        పునాది_type = opt;
        ఆపు;
#అయితే_ముగింపు

      case_GETOPT_HELP_CHAR;

      case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);

      అప్రమేయ:
        usage (EXIT_FAILURE);
        ఆపు;
      }

#ఒకవేళ పునాది_TYPE == 42
  required_padding = no_required_padding;
  has_padding = no_padding;
  get_pending_నిడివి = no_pending_నిడివి;
  పునాది_decode_ctx_finalize = decode_ctx_finalize;

  ఎంపిక (పునాది_type)
    {
    స్థితి BASE64_OPTION:
      పునాది_నిడివి_2 = base64_నిడివి_wrapper;
      required_padding = base64_required_padding;
      has_padding = base64_ctx_has_padding;
      get_pending_నిడివి = base64_ctx_get_pending_నిడివి;
      isubase = isubase64;
      పునాది_encode = base64_encode;
      పునాది_decode_ctx_init = base64_decode_ctx_init_wrapper;
      పునాది_decode_ctx = base64_decode_ctx_wrapper;
      ఆపు;

    స్థితి BASE64URL_OPTION:
      పునాది_నిడివి_2 = base64_నిడివి_wrapper;
      required_padding = base64_required_padding;
      has_padding = base64_ctx_has_padding;
      get_pending_నిడివి = base64_ctx_get_pending_నిడివి;
      isubase = isubase64url;
      పునాది_encode = base64url_encode;
      పునాది_decode_ctx_init = base64url_decode_ctx_init_wrapper;
      పునాది_decode_ctx = base64url_decode_ctx_wrapper;
      ఆపు;

    స్థితి BASE32_OPTION:
      పునాది_నిడివి_2 = base32_నిడివి_wrapper;
      required_padding = base32_required_padding;
      has_padding = base32_ctx_has_padding;
      get_pending_నిడివి = base32_ctx_get_pending_నిడివి;
      isubase = isubase32;
      పునాది_encode = base32_encode;
      పునాది_decode_ctx_init = base32_decode_ctx_init_wrapper;
      పునాది_decode_ctx = base32_decode_ctx_wrapper;
      ఆపు;

    స్థితి BASE32HEX_OPTION:
      పునాది_నిడివి_2 = base32_నిడివి_wrapper;
      required_padding = base32_required_padding;
      has_padding = base32_ctx_has_padding;
      get_pending_నిడివి = base32_ctx_get_pending_నిడివి;
      isubase = isubase32hex;
      పునాది_encode = base32hex_encode;
      పునాది_decode_ctx_init = base32hex_decode_ctx_init_wrapper;
      పునాది_decode_ctx = base32hex_decode_ctx_wrapper;
      ఆపు;

    స్థితి BASE16_OPTION:
      పునాది_నిడివి_2 = base16_నిడివి;
      get_pending_నిడివి = base16_ctx_get_pending_నిడివి;
      isubase = isubase16;
      పునాది_encode = base16_encode;
      పునాది_decode_ctx_init = base16_decode_ctx_init;
      పునాది_decode_ctx = base16_decode_ctx;
      ఆపు;

    స్థితి BASE2MSBF_OPTION:
      పునాది_నిడివి_2 = base2_నిడివి;
      get_pending_నిడివి = base2_ctx_get_pending_నిడివి;
      isubase = isubase2;
      పునాది_encode = base2msbf_encode;
      పునాది_decode_ctx_init = base2_decode_ctx_init;
      పునాది_decode_ctx = base2msbf_decode_ctx;
      ఆపు;

    స్థితి BASE2LSBF_OPTION:
      పునాది_నిడివి_2 = base2_నిడివి;
      get_pending_నిడివి = base2_ctx_get_pending_నిడివి;
      isubase = isubase2;
      పునాది_encode = base2lsbf_encode;
      పునాది_decode_ctx_init = base2_decode_ctx_init;
      పునాది_decode_ctx = base2lsbf_decode_ctx;
      ఆపు;

    స్థితి Z85_OPTION:
      పునాది_నిడివి_2 = z85_నిడివి;
      get_pending_నిడివి = z85_ctx_get_pending_నిడివి;
      isubase = isuz85;
      పునాది_encode = z85_encode;
      పునాది_decode_ctx_init = z85_decode_ctx_init;
      పునాది_decode_ctx = z85_decode_ctx;
      ఆపు;

    స్థితి BASE58_OPTION:
      పునాది_నిడివి_2 = base58_నిడివి;
      isubase = isubase58;
      పునాది_encode_ctx_init = base58_encode_ctx_init;
      పునాది_encode_ctx = base58_encode_ctx;
      పునాది_encode_ctx_finalize = base58_encode_ctx_finalize;
      పునాది_decode_ctx_init = base58_decode_ctx_init;
      పునాది_decode_ctx = base58_decode_ctx;
      పునాది_decode_ctx_finalize = base58_decode_ctx_finalize;
      ఆపు;

    అప్రమేయ:
      error (0, 0, _("missing encoding type"));
      usage (EXIT_FAILURE);
    }
#అయితే_ముగింపు

  ఒకవేళ (argc - optind > 1)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }

  ఒకవేళ (optind < argc)
    infile = argv[optind];
  లేకపోతే
    infile = "-";

  ఒకవేళ (streq (infile, "-"))
    {
      xset_ద్విమాన_బహుళకం (STDIN_FILENO, O_ద్విమాన);
      ఆదానం_fh = stdin;
    }
  లేకపోతే
    {
      ఆదానం_fh = fopen (infile, "rb");
      ఒకవేళ (ఆదానం_fh == ఖాళీ)
        error (EXIT_FAILURE, errno, "%s", quotef (infile));
    }

  fadvise (ఆదానం_fh, FADVISE_SEQUENTIAL);

  ఒకవేళ (decode)
    do_decode (ఆదానం_fh, infile, stdout, ignore_garbage);
  లేకపోతే
    do_encode (ఆదానం_fh, infile, stdout, wrap_స్తంభం);
}
