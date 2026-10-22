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

#அந்தர்பாவ <stddef.h>
#அந்தர்பாவ <stdint.h>

/* Pack a structure if possible.  This might save space, and is not
   needed for correctness.  */
#ifdef _MSC_VER
# define BLAKE2_PACKED(x) __pragma (pack (தள்ளு, 1)) x __pragma (pack (எடு))
#இல்லையெனில்
# define BLAKE2_PACKED(x) x _GL_ATTRIBUTE_PACKED
#endif

#எனில் defined(__cplusplus)
வெளி "C" {
#endif

  எண்ணிக்கை blake2s_constant
  {
    BLAKE2S_BLOCKBYTES = 64,
    BLAKE2S_OUTBYTES   = 32,
    BLAKE2S_KEYBYTES   = 32,
    BLAKE2S_SALTBYTES  = 8,
    BLAKE2S_PERSONALBYTES = 8
  };

  எண்ணிக்கை blake2b_constant
  {
    BLAKE2B_BLOCKBYTES = 128,
    BLAKE2B_OUTBYTES   = 64,
    BLAKE2B_KEYBYTES   = 64,
    BLAKE2B_SALTBYTES  = 16,
    BLAKE2B_PERSONALBYTES = 16
  };

  வகைவரை கட்டமைப்பு blake2s_state__
  {
    uint32_t h[8];
    uint32_t t[2];
    uint32_t f[2];
    uint8_t  buf[BLAKE2S_BLOCKBYTES];
    size_t   buflen;
    size_t   outlen;
    uint8_t  last_முனை;
  } blake2s_state;

  வகைவரை கட்டமைப்பு blake2b_state__
  {
    uint64_t h[8];
    uint64_t t[2];
    uint64_t f[2];
    uint8_t  buf[BLAKE2B_BLOCKBYTES];
    size_t   buflen;
    size_t   outlen;
    uint8_t  last_முனை;
  } blake2b_state;

  வகைவரை கட்டமைப்பு blake2sp_state__
  {
    blake2s_state S[8][1];
    blake2s_state R[1];
    uint8_t       buf[8 * BLAKE2S_BLOCKBYTES];
    size_t        buflen;
    size_t        outlen;
  } blake2sp_state;

  வகைவரை கட்டமைப்பு blake2bp_state__
  {
    blake2b_state S[4][1];
    blake2b_state R[1];
    uint8_t       buf[4 * BLAKE2B_BLOCKBYTES];
    size_t        buflen;
    size_t        outlen;
  } blake2bp_state;


  BLAKE2_PACKED(கட்டமைப்பு blake2s_param__
  {
    uint8_t  digest_length; /* 1 */
    uint8_t  சாவி_length;    /* 2 */
    uint8_t  fanout;        /* 3 */
    uint8_t  depth;         /* 4 */
    uint32_t leaf_length;   /* 8 */
    uint32_t முனை_offset;  /* 12 */
    uint16_t xof_length;    /* 14 */
    uint8_t  முனை_depth;    /* 15 */
    uint8_t  inner_length;  /* 16 */
    /* uint8_t  reserved[0]; */
    uint8_t  salt[BLAKE2S_SALTBYTES]; /* 24 */
    uint8_t  personal[BLAKE2S_PERSONALBYTES];  /* 32 */
  });

  வகைவரை கட்டமைப்பு blake2s_param__ blake2s_param;

  BLAKE2_PACKED(கட்டமைப்பு blake2b_param__
  {
    uint8_t  digest_length; /* 1 */
    uint8_t  சாவி_length;    /* 2 */
    uint8_t  fanout;        /* 3 */
    uint8_t  depth;         /* 4 */
    uint32_t leaf_length;   /* 8 */
    uint32_t முனை_offset;   /* 12 */
    uint32_t xof_length;    /* 16 */
    uint8_t  முனை_depth;    /* 17 */
    uint8_t  inner_length;  /* 18 */
    uint8_t  reserved[14];  /* 32 */
    uint8_t  salt[BLAKE2B_SALTBYTES]; /* 48 */
    uint8_t  personal[BLAKE2B_PERSONALBYTES];  /* 64 */
  });

  வகைவரை கட்டமைப்பு blake2b_param__ blake2b_param;

  வகைவரை கட்டமைப்பு blake2xs_state__
  {
    blake2s_state S[1];
    blake2s_param P[1];
  } blake2xs_state;

  வகைவரை கட்டமைப்பு blake2xb_state__
  {
    blake2b_state S[1];
    blake2b_param P[1];
  } blake2xb_state;

  /* Padded structs result in a compile-time error */
  எண்ணிக்கை {
    BLAKE2_DUMMY_1 = 1 / (அளவு (blake2s_param) == BLAKE2S_OUTBYTES),
    BLAKE2_DUMMY_2 = 1 / (அளவு (blake2b_param) == BLAKE2B_OUTBYTES)
  };

  /* Streaming API */
  முழு blake2s_init( blake2s_state *S, size_t outlen );
  முழு blake2s_init_சாவி( blake2s_state *S, size_t outlen, மாறிலி வெறுமை *சாவி, size_t keylen );
  முழு blake2s_init_param( blake2s_state *S, மாறிலி blake2s_param *P );
  முழு blake2s_update( blake2s_state *S, மாறிலி வெறுமை *in, size_t inlen );
  முழு blake2s_final( blake2s_state *S, வெறுமை *out, size_t outlen );

  முழு blake2b_init( blake2b_state *S, size_t outlen );
  முழு blake2b_init_சாவி( blake2b_state *S, size_t outlen, மாறிலி வெறுமை *சாவி, size_t keylen );
  முழு blake2b_init_param (blake2b_state *S, மாறிலி blake2b_param *P)
    _GL_ATTRIBUTE_NONNULL ();
  முழு blake2b_update( blake2b_state *S, மாறிலி வெறுமை *in, size_t inlen );
  முழு blake2b_final( blake2b_state *S, வெறுமை *out, size_t outlen );

  முழு blake2sp_init( blake2sp_state *S, size_t outlen );
  முழு blake2sp_init_சாவி( blake2sp_state *S, size_t outlen, மாறிலி வெறுமை *சாவி, size_t keylen );
  முழு blake2sp_update( blake2sp_state *S, மாறிலி வெறுமை *in, size_t inlen );
  முழு blake2sp_final( blake2sp_state *S, வெறுமை *out, size_t outlen );

  முழு blake2bp_init( blake2bp_state *S, size_t outlen );
  முழு blake2bp_init_சாவி( blake2bp_state *S, size_t outlen, மாறிலி வெறுமை *சாவி, size_t keylen );
  முழு blake2bp_update( blake2bp_state *S, மாறிலி வெறுமை *in, size_t inlen );
  முழு blake2bp_final( blake2bp_state *S, வெறுமை *out, size_t outlen );

  /* Variable output length API */
  முழு blake2xs_init( blake2xs_state *S, மாறிலி size_t outlen );
  முழு blake2xs_init_சாவி( blake2xs_state *S, மாறிலி size_t outlen, மாறிலி வெறுமை *சாவி, size_t keylen );
  முழு blake2xs_update( blake2xs_state *S, மாறிலி வெறுமை *in, size_t inlen );
  முழு blake2xs_final(blake2xs_state *S, வெறுமை *out, size_t outlen);

  முழு blake2xb_init( blake2xb_state *S, மாறிலி size_t outlen );
  முழு blake2xb_init_சாவி( blake2xb_state *S, மாறிலி size_t outlen, மாறிலி வெறுமை *சாவி, size_t keylen );
  முழு blake2xb_update( blake2xb_state *S, மாறிலி வெறுமை *in, size_t inlen );
  முழு blake2xb_final(blake2xb_state *S, வெறுமை *out, size_t outlen);

  /* Simple API */
  முழு blake2s( வெறுமை *out, size_t outlen, மாறிலி வெறுமை *in, size_t inlen, மாறிலி வெறுமை *சாவி, size_t keylen );
  முழு blake2b( வெறுமை *out, size_t outlen, மாறிலி வெறுமை *in, size_t inlen, மாறிலி வெறுமை *சாவி, size_t keylen );

  முழு blake2sp( வெறுமை *out, size_t outlen, மாறிலி வெறுமை *in, size_t inlen, மாறிலி வெறுமை *சாவி, size_t keylen );
  முழு blake2bp( வெறுமை *out, size_t outlen, மாறிலி வெறுமை *in, size_t inlen, மாறிலி வெறுமை *சாவி, size_t keylen );

  முழு blake2xs( வெறுமை *out, size_t outlen, மாறிலி வெறுமை *in, size_t inlen, மாறிலி வெறுமை *சாவி, size_t keylen );
  முழு blake2xb( வெறுமை *out, size_t outlen, மாறிலி வெறுமை *in, size_t inlen, மாறிலி வெறுமை *சாவி, size_t keylen );

  /* This is simply an alias for blake2b */
  முழு blake2( வெறுமை *out, size_t outlen, மாறிலி வெறுமை *in, size_t inlen, மாறிலி வெறுமை *சாவி, size_t keylen );

#எனில் defined(__cplusplus)
}
#endif

#endif
