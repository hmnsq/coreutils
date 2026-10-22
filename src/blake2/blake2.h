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
#ifndef BLAKE2_H
#define BLAKE2_H

#অন্তর্ভাব <stddef.h>
#অন্তর্ভাব <stdint.h>

/* Pack a structure if possible.  This might save space, and is not
   needed for correctness.  */
#ifdef _MSC_VER
# define BLAKE2_PACKED(x) __pragma (pack (ঠেলো, 1)) x __pragma (pack (বের_করো))
#অন্যথা
# define BLAKE2_PACKED(x) x _GL_ATTRIBUTE_PACKED
#endif

#যদি defined(__cplusplus)
বাহ্যিক "C" {
#endif

  গণনা blake2s_constant
  {
    BLAKE2S_BLOCKBYTES = 64,
    BLAKE2S_OUTBYTES   = 32,
    BLAKE2S_KEYBYTES   = 32,
    BLAKE2S_SALTBYTES  = 8,
    BLAKE2S_PERSONALBYTES = 8
  };

  গণনা blake2b_constant
  {
    BLAKE2B_BLOCKBYTES = 128,
    BLAKE2B_OUTBYTES   = 64,
    BLAKE2B_KEYBYTES   = 64,
    BLAKE2B_SALTBYTES  = 16,
    BLAKE2B_PERSONALBYTES = 16
  };

  প্রকারলেখ কাঠামো blake2s_state__
  {
    uint32_t h[8];
    uint32_t t[2];
    uint32_t f[2];
    uint8_t  buf[BLAKE2S_BLOCKBYTES];
    size_t   buflen;
    size_t   outlen;
    uint8_t  last_গ্রন্থি;
  } blake2s_state;

  প্রকারলেখ কাঠামো blake2b_state__
  {
    uint64_t h[8];
    uint64_t t[2];
    uint64_t f[2];
    uint8_t  buf[BLAKE2B_BLOCKBYTES];
    size_t   buflen;
    size_t   outlen;
    uint8_t  last_গ্রন্থি;
  } blake2b_state;

  প্রকারলেখ কাঠামো blake2sp_state__
  {
    blake2s_state S[8][1];
    blake2s_state R[1];
    uint8_t       buf[8 * BLAKE2S_BLOCKBYTES];
    size_t        buflen;
    size_t        outlen;
  } blake2sp_state;

  প্রকারলেখ কাঠামো blake2bp_state__
  {
    blake2b_state S[4][1];
    blake2b_state R[1];
    uint8_t       buf[4 * BLAKE2B_BLOCKBYTES];
    size_t        buflen;
    size_t        outlen;
  } blake2bp_state;


  BLAKE2_PACKED(কাঠামো blake2s_param__
  {
    uint8_t  digest_দৈর্ঘ্য; /* 1 */
    uint8_t  চাবি_দৈর্ঘ্য;    /* 2 */
    uint8_t  fanout;        /* 3 */
    uint8_t  depth;         /* 4 */
    uint32_t leaf_দৈর্ঘ্য;   /* 8 */
    uint32_t গ্রন্থি_offset;  /* 12 */
    uint16_t xof_দৈর্ঘ্য;    /* 14 */
    uint8_t  গ্রন্থি_depth;    /* 15 */
    uint8_t  inner_দৈর্ঘ্য;  /* 16 */
    /* uint8_t  reserved[0]; */
    uint8_t  salt[BLAKE2S_SALTBYTES]; /* 24 */
    uint8_t  personal[BLAKE2S_PERSONALBYTES];  /* 32 */
  });

  প্রকারলেখ কাঠামো blake2s_param__ blake2s_param;

  BLAKE2_PACKED(কাঠামো blake2b_param__
  {
    uint8_t  digest_দৈর্ঘ্য; /* 1 */
    uint8_t  চাবি_দৈর্ঘ্য;    /* 2 */
    uint8_t  fanout;        /* 3 */
    uint8_t  depth;         /* 4 */
    uint32_t leaf_দৈর্ঘ্য;   /* 8 */
    uint32_t গ্রন্থি_offset;   /* 12 */
    uint32_t xof_দৈর্ঘ্য;    /* 16 */
    uint8_t  গ্রন্থি_depth;    /* 17 */
    uint8_t  inner_দৈর্ঘ্য;  /* 18 */
    uint8_t  reserved[14];  /* 32 */
    uint8_t  salt[BLAKE2B_SALTBYTES]; /* 48 */
    uint8_t  personal[BLAKE2B_PERSONALBYTES];  /* 64 */
  });

  প্রকারলেখ কাঠামো blake2b_param__ blake2b_param;

  প্রকারলেখ কাঠামো blake2xs_state__
  {
    blake2s_state S[1];
    blake2s_param P[1];
  } blake2xs_state;

  প্রকারলেখ কাঠামো blake2xb_state__
  {
    blake2b_state S[1];
    blake2b_param P[1];
  } blake2xb_state;

  /* Padded structs result in a compile-time error */
  গণনা {
    BLAKE2_DUMMY_1 = 1 / (আকার (blake2s_param) == BLAKE2S_OUTBYTES),
    BLAKE2_DUMMY_2 = 1 / (আকার (blake2b_param) == BLAKE2B_OUTBYTES)
  };

  /* Streaming API */
  পূর্ণসংখ্যা blake2s_init( blake2s_state *S, size_t outlen );
  পূর্ণসংখ্যা blake2s_init_চাবি( blake2s_state *S, size_t outlen, ধ্রুবক শূন্য *চাবি, size_t keylen );
  পূর্ণসংখ্যা blake2s_init_param( blake2s_state *S, ধ্রুবক blake2s_param *P );
  পূর্ণসংখ্যা blake2s_update( blake2s_state *S, ধ্রুবক শূন্য *in, size_t inlen );
  পূর্ণসংখ্যা blake2s_final( blake2s_state *S, শূন্য *out, size_t outlen );

  পূর্ণসংখ্যা blake2b_init( blake2b_state *S, size_t outlen );
  পূর্ণসংখ্যা blake2b_init_চাবি( blake2b_state *S, size_t outlen, ধ্রুবক শূন্য *চাবি, size_t keylen );
  পূর্ণসংখ্যা blake2b_init_param (blake2b_state *S, ধ্রুবক blake2b_param *P)
    _GL_ATTRIBUTE_NONNULL ();
  পূর্ণসংখ্যা blake2b_update( blake2b_state *S, ধ্রুবক শূন্য *in, size_t inlen );
  পূর্ণসংখ্যা blake2b_final( blake2b_state *S, শূন্য *out, size_t outlen );

  পূর্ণসংখ্যা blake2sp_init( blake2sp_state *S, size_t outlen );
  পূর্ণসংখ্যা blake2sp_init_চাবি( blake2sp_state *S, size_t outlen, ধ্রুবক শূন্য *চাবি, size_t keylen );
  পূর্ণসংখ্যা blake2sp_update( blake2sp_state *S, ধ্রুবক শূন্য *in, size_t inlen );
  পূর্ণসংখ্যা blake2sp_final( blake2sp_state *S, শূন্য *out, size_t outlen );

  পূর্ণসংখ্যা blake2bp_init( blake2bp_state *S, size_t outlen );
  পূর্ণসংখ্যা blake2bp_init_চাবি( blake2bp_state *S, size_t outlen, ধ্রুবক শূন্য *চাবি, size_t keylen );
  পূর্ণসংখ্যা blake2bp_update( blake2bp_state *S, ধ্রুবক শূন্য *in, size_t inlen );
  পূর্ণসংখ্যা blake2bp_final( blake2bp_state *S, শূন্য *out, size_t outlen );

  /* Variable output length API */
  পূর্ণসংখ্যা blake2xs_init( blake2xs_state *S, ধ্রুবক size_t outlen );
  পূর্ণসংখ্যা blake2xs_init_চাবি( blake2xs_state *S, ধ্রুবক size_t outlen, ধ্রুবক শূন্য *চাবি, size_t keylen );
  পূর্ণসংখ্যা blake2xs_update( blake2xs_state *S, ধ্রুবক শূন্য *in, size_t inlen );
  পূর্ণসংখ্যা blake2xs_final(blake2xs_state *S, শূন্য *out, size_t outlen);

  পূর্ণসংখ্যা blake2xb_init( blake2xb_state *S, ধ্রুবক size_t outlen );
  পূর্ণসংখ্যা blake2xb_init_চাবি( blake2xb_state *S, ধ্রুবক size_t outlen, ধ্রুবক শূন্য *চাবি, size_t keylen );
  পূর্ণসংখ্যা blake2xb_update( blake2xb_state *S, ধ্রুবক শূন্য *in, size_t inlen );
  পূর্ণসংখ্যা blake2xb_final(blake2xb_state *S, শূন্য *out, size_t outlen);

  /* Simple API */
  পূর্ণসংখ্যা blake2s( শূন্য *out, size_t outlen, ধ্রুবক শূন্য *in, size_t inlen, ধ্রুবক শূন্য *চাবি, size_t keylen );
  পূর্ণসংখ্যা blake2b( শূন্য *out, size_t outlen, ধ্রুবক শূন্য *in, size_t inlen, ধ্রুবক শূন্য *চাবি, size_t keylen );

  পূর্ণসংখ্যা blake2sp( শূন্য *out, size_t outlen, ধ্রুবক শূন্য *in, size_t inlen, ধ্রুবক শূন্য *চাবি, size_t keylen );
  পূর্ণসংখ্যা blake2bp( শূন্য *out, size_t outlen, ধ্রুবক শূন্য *in, size_t inlen, ধ্রুবক শূন্য *চাবি, size_t keylen );

  পূর্ণসংখ্যা blake2xs( শূন্য *out, size_t outlen, ধ্রুবক শূন্য *in, size_t inlen, ধ্রুবক শূন্য *চাবি, size_t keylen );
  পূর্ণসংখ্যা blake2xb( শূন্য *out, size_t outlen, ধ্রুবক শূন্য *in, size_t inlen, ধ্রুবক শূন্য *চাবি, size_t keylen );

  /* This is simply an alias for blake2b */
  পূর্ণসংখ্যা blake2( শূন্য *out, size_t outlen, ধ্রুবক শূন্য *in, size_t inlen, ধ্রুবক শূন্য *চাবি, size_t keylen );

#যদি defined(__cplusplus)
}
#endif

#endif
