/*
   BLAKE2 reference source code package - reference C implementations

   Copyright 2012, Samuel Neves <sneves@dei.uc.pt>.  You may use this under the
   terms of the CC0, the OpenSSL Licence, or the Apache Public License 2.0, at
   your option.  The terms of these licenses can be found at:

   - CC0 1.0 Universal : https://creativecommons.org/publicdomain/zero/1.0
   - OpenSSL license   : https://www.openssl.org/source/license.html
   - Apache 2.0        : https://www.apache.org/licenses/LICENSE-2.0

   More information about the BLAKE2 hash function can be found at
   https://blake2.net.
*/
#که_نه_تعریف_شوی BLAKE2_H
#تعریف_کړه BLAKE2_H

#ضمن <stddef.h>
#ضمن <stdint.h>

/* Pack a structure if possible.  This might save space, and is not
   needed for correctness.  */
#که_تعریف_شوی _MSC_VER
# define BLAKE2_PACKED(x) __pragma (pack (ټېل, 1)) x __pragma (pack (راوباسه))
#وإلا
# define BLAKE2_PACKED(x) x _GL_ATTRIBUTE_PACKED
#د_که_پای

#إذا defined(__cplusplus)
خارجي "C" {
#د_که_پای

  تعداد blake2s_constant
  {
    BLAKE2S_BLOCKBYTES = 64,
    BLAKE2S_OUTBYTES   = 32,
    BLAKE2S_KEYBYTES   = 32,
    BLAKE2S_SALTBYTES  = 8,
    BLAKE2S_PERSONALBYTES = 8
  };

  تعداد blake2b_constant
  {
    BLAKE2B_BLOCKBYTES = 128,
    BLAKE2B_OUTBYTES   = 64,
    BLAKE2B_KEYBYTES   = 64,
    BLAKE2B_SALTBYTES  = 16,
    BLAKE2B_PERSONALBYTES = 16
  };

  تعريفنوع بنية blake2s_state__
  {
    uint32_t h[8];
    uint32_t t[2];
    uint32_t f[2];
    uint8_t  buf[BLAKE2S_BLOCKBYTES];
    size_t   buflen;
    size_t   outlen;
    uint8_t  وروستی_غوټه;
  } blake2s_state;

  تعريفنوع بنية blake2b_state__
  {
    uint64_t h[8];
    uint64_t t[2];
    uint64_t f[2];
    uint8_t  buf[BLAKE2B_BLOCKBYTES];
    size_t   buflen;
    size_t   outlen;
    uint8_t  وروستی_غوټه;
  } blake2b_state;

  تعريفنوع بنية blake2sp_state__
  {
    blake2s_state S[8][1];
    blake2s_state R[1];
    uint8_t       buf[8 * BLAKE2S_BLOCKBYTES];
    size_t        buflen;
    size_t        outlen;
  } blake2sp_state;

  تعريفنوع بنية blake2bp_state__
  {
    blake2b_state S[4][1];
    blake2b_state R[1];
    uint8_t       buf[4 * BLAKE2B_BLOCKBYTES];
    size_t        buflen;
    size_t        outlen;
  } blake2bp_state;


  BLAKE2_PACKED(بنية blake2s_param__
  {
    uint8_t  digest_اوږدتیا; /* 1 */
    uint8_t  کلی_اوږدتیا;    /* 2 */
    uint8_t  fanout;        /* 3 */
    uint8_t  ژوروالی;         /* 4 */
    uint32_t ونې_پاڼه_اوږدتیا;   /* 8 */
    uint32_t غوټه_دننه_واټن;  /* 12 */
    uint16_t xof_اوږدتیا;    /* 14 */
    uint8_t  غوټه_ژوروالی;    /* 15 */
    uint8_t  inner_اوږدتیا;  /* 16 */
    /* uint8_t  reserved[0]; */
    uint8_t  salt[BLAKE2S_SALTBYTES]; /* 24 */
    uint8_t  personal[BLAKE2S_PERSONALBYTES];  /* 32 */
  });

  تعريفنوع بنية blake2s_param__ blake2s_param;

  BLAKE2_PACKED(بنية blake2b_param__
  {
    uint8_t  digest_اوږدتیا; /* 1 */
    uint8_t  کلی_اوږدتیا;    /* 2 */
    uint8_t  fanout;        /* 3 */
    uint8_t  ژوروالی;         /* 4 */
    uint32_t ونې_پاڼه_اوږدتیا;   /* 8 */
    uint32_t غوټه_دننه_واټن;   /* 12 */
    uint32_t xof_اوږدتیا;    /* 16 */
    uint8_t  غوټه_ژوروالی;    /* 17 */
    uint8_t  inner_اوږدتیا;  /* 18 */
    uint8_t  reserved[14];  /* 32 */
    uint8_t  salt[BLAKE2B_SALTBYTES]; /* 48 */
    uint8_t  personal[BLAKE2B_PERSONALBYTES];  /* 64 */
  });

  تعريفنوع بنية blake2b_param__ blake2b_param;

  تعريفنوع بنية blake2xs_state__
  {
    blake2s_state S[1];
    blake2s_param P[1];
  } blake2xs_state;

  تعريفنوع بنية blake2xb_state__
  {
    blake2b_state S[1];
    blake2b_param P[1];
  } blake2xb_state;

  /* Padded structs result in a compile-time error */
  تعداد {
    BLAKE2_DUMMY_1 = 1 / (حجم (blake2s_param) == BLAKE2S_OUTBYTES),
    BLAKE2_DUMMY_2 = 1 / (حجم (blake2b_param) == BLAKE2B_OUTBYTES)
  };

  /* Streaming API */
  صحيح blake2s_init( blake2s_state *S, size_t outlen );
  صحيح blake2s_init_کلی( blake2s_state *S, size_t outlen, ثابت فراغ *کلی, size_t keylen );
  صحيح blake2s_init_param( blake2s_state *S, ثابت blake2s_param *P );
  صحيح blake2s_update( blake2s_state *S, ثابت فراغ *دننه, size_t inlen );
  صحيح blake2s_final( blake2s_state *S, فراغ *بهر, size_t outlen );

  صحيح blake2b_init( blake2b_state *S, size_t outlen );
  صحيح blake2b_init_کلی( blake2b_state *S, size_t outlen, ثابت فراغ *کلی, size_t keylen );
  صحيح blake2b_init_param (blake2b_state *S, ثابت blake2b_param *P)
    _GL_ATTRIBUTE_NONNULL ();
  صحيح blake2b_update( blake2b_state *S, ثابت فراغ *دننه, size_t inlen );
  صحيح blake2b_final( blake2b_state *S, فراغ *بهر, size_t outlen );

  صحيح blake2sp_init( blake2sp_state *S, size_t outlen );
  صحيح blake2sp_init_کلی( blake2sp_state *S, size_t outlen, ثابت فراغ *کلی, size_t keylen );
  صحيح blake2sp_update( blake2sp_state *S, ثابت فراغ *دننه, size_t inlen );
  صحيح blake2sp_final( blake2sp_state *S, فراغ *بهر, size_t outlen );

  صحيح blake2bp_init( blake2bp_state *S, size_t outlen );
  صحيح blake2bp_init_کلی( blake2bp_state *S, size_t outlen, ثابت فراغ *کلی, size_t keylen );
  صحيح blake2bp_update( blake2bp_state *S, ثابت فراغ *دننه, size_t inlen );
  صحيح blake2bp_final( blake2bp_state *S, فراغ *بهر, size_t outlen );

  /* Variable output length API */
  صحيح blake2xs_init( blake2xs_state *S, ثابت size_t outlen );
  صحيح blake2xs_init_کلی( blake2xs_state *S, ثابت size_t outlen, ثابت فراغ *کلی, size_t keylen );
  صحيح blake2xs_update( blake2xs_state *S, ثابت فراغ *دننه, size_t inlen );
  صحيح blake2xs_final(blake2xs_state *S, فراغ *بهر, size_t outlen);

  صحيح blake2xb_init( blake2xb_state *S, ثابت size_t outlen );
  صحيح blake2xb_init_کلی( blake2xb_state *S, ثابت size_t outlen, ثابت فراغ *کلی, size_t keylen );
  صحيح blake2xb_update( blake2xb_state *S, ثابت فراغ *دننه, size_t inlen );
  صحيح blake2xb_final(blake2xb_state *S, فراغ *بهر, size_t outlen);

  /* Simple API */
  صحيح blake2s( فراغ *بهر, size_t outlen, ثابت فراغ *دننه, size_t inlen, ثابت فراغ *کلی, size_t keylen );
  صحيح blake2b( فراغ *بهر, size_t outlen, ثابت فراغ *دننه, size_t inlen, ثابت فراغ *کلی, size_t keylen );

  صحيح blake2sp( فراغ *بهر, size_t outlen, ثابت فراغ *دننه, size_t inlen, ثابت فراغ *کلی, size_t keylen );
  صحيح blake2bp( فراغ *بهر, size_t outlen, ثابت فراغ *دننه, size_t inlen, ثابت فراغ *کلی, size_t keylen );

  صحيح blake2xs( فراغ *بهر, size_t outlen, ثابت فراغ *دننه, size_t inlen, ثابت فراغ *کلی, size_t keylen );
  صحيح blake2xb( فراغ *بهر, size_t outlen, ثابت فراغ *دننه, size_t inlen, ثابت فراغ *کلی, size_t keylen );

  /* This is simply an alias for blake2b */
  صحيح blake2( فراغ *بهر, size_t outlen, ثابت فراغ *دننه, size_t inlen, ثابت فراغ *کلی, size_t keylen );

#إذا defined(__cplusplus)
}
#د_که_پای

#د_که_پای
