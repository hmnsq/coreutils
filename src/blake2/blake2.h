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

#ಅನ್ತರ್ಭಾವ <stddef.h>
#ಅನ್ತರ್ಭಾವ <stdint.h>

/* Pack a structure if possible.  This might save space, and is not
   needed for correctness.  */
#ifdef _MSC_VER
# define BLAKE2_PACKED(x) __pragma (pack (ತಳ್ಳು, 1)) x __pragma (pack (ತೆಗೆ))
#ಅನ್ಯಥಾ
# define BLAKE2_PACKED(x) x _GL_ATTRIBUTE_PACKED
#endif

#ಯದಿ defined(__cplusplus)
ಬಾಹ್ಯ "C" {
#endif

  ಗಣನಾ blake2s_constant
  {
    BLAKE2S_BLOCKBYTES = 64,
    BLAKE2S_OUTBYTES   = 32,
    BLAKE2S_KEYBYTES   = 32,
    BLAKE2S_SALTBYTES  = 8,
    BLAKE2S_PERSONALBYTES = 8
  };

  ಗಣನಾ blake2b_constant
  {
    BLAKE2B_BLOCKBYTES = 128,
    BLAKE2B_OUTBYTES   = 64,
    BLAKE2B_KEYBYTES   = 64,
    BLAKE2B_SALTBYTES  = 16,
    BLAKE2B_PERSONALBYTES = 16
  };

  ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ blake2s_state__
  {
    uint32_t h[8];
    uint32_t t[2];
    uint32_t f[2];
    uint8_t  buf[BLAKE2S_BLOCKBYTES];
    size_t   buflen;
    size_t   outlen;
    uint8_t  last_ಗಂಟು;
  } blake2s_state;

  ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ blake2b_state__
  {
    uint64_t h[8];
    uint64_t t[2];
    uint64_t f[2];
    uint8_t  buf[BLAKE2B_BLOCKBYTES];
    size_t   buflen;
    size_t   outlen;
    uint8_t  last_ಗಂಟು;
  } blake2b_state;

  ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ blake2sp_state__
  {
    blake2s_state S[8][1];
    blake2s_state R[1];
    uint8_t       buf[8 * BLAKE2S_BLOCKBYTES];
    size_t        buflen;
    size_t        outlen;
  } blake2sp_state;

  ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ blake2bp_state__
  {
    blake2b_state S[4][1];
    blake2b_state R[1];
    uint8_t       buf[4 * BLAKE2B_BLOCKBYTES];
    size_t        buflen;
    size_t        outlen;
  } blake2bp_state;


  BLAKE2_PACKED(ಸಂರಚನಾ blake2s_param__
  {
    uint8_t  digest_length; /* 1 */
    uint8_t  ಕೀಲಿ_length;    /* 2 */
    uint8_t  fanout;        /* 3 */
    uint8_t  depth;         /* 4 */
    uint32_t leaf_length;   /* 8 */
    uint32_t ಗಂಟು_offset;  /* 12 */
    uint16_t xof_length;    /* 14 */
    uint8_t  ಗಂಟು_depth;    /* 15 */
    uint8_t  inner_length;  /* 16 */
    /* uint8_t  reserved[0]; */
    uint8_t  salt[BLAKE2S_SALTBYTES]; /* 24 */
    uint8_t  personal[BLAKE2S_PERSONALBYTES];  /* 32 */
  });

  ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ blake2s_param__ blake2s_param;

  BLAKE2_PACKED(ಸಂರಚನಾ blake2b_param__
  {
    uint8_t  digest_length; /* 1 */
    uint8_t  ಕೀಲಿ_length;    /* 2 */
    uint8_t  fanout;        /* 3 */
    uint8_t  depth;         /* 4 */
    uint32_t leaf_length;   /* 8 */
    uint32_t ಗಂಟು_offset;   /* 12 */
    uint32_t xof_length;    /* 16 */
    uint8_t  ಗಂಟು_depth;    /* 17 */
    uint8_t  inner_length;  /* 18 */
    uint8_t  reserved[14];  /* 32 */
    uint8_t  salt[BLAKE2B_SALTBYTES]; /* 48 */
    uint8_t  personal[BLAKE2B_PERSONALBYTES];  /* 64 */
  });

  ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ blake2b_param__ blake2b_param;

  ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ blake2xs_state__
  {
    blake2s_state S[1];
    blake2s_param P[1];
  } blake2xs_state;

  ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ blake2xb_state__
  {
    blake2b_state S[1];
    blake2b_param P[1];
  } blake2xb_state;

  /* Padded structs result in a compile-time error */
  ಗಣನಾ {
    BLAKE2_DUMMY_1 = 1 / (ಪರಿಮಾಣ (blake2s_param) == BLAKE2S_OUTBYTES),
    BLAKE2_DUMMY_2 = 1 / (ಪರಿಮಾಣ (blake2b_param) == BLAKE2B_OUTBYTES)
  };

  /* Streaming API */
  ಪೂರ್ಣಾಙ್ಕ blake2s_init( blake2s_state *S, size_t outlen );
  ಪೂರ್ಣಾಙ್ಕ blake2s_init_ಕೀಲಿ( blake2s_state *S, size_t outlen, ಧ್ರುವ ಶೂನ್ಯ *ಕೀಲಿ, size_t keylen );
  ಪೂರ್ಣಾಙ್ಕ blake2s_init_param( blake2s_state *S, ಧ್ರುವ blake2s_param *P );
  ಪೂರ್ಣಾಙ್ಕ blake2s_update( blake2s_state *S, ಧ್ರುವ ಶೂನ್ಯ *in, size_t inlen );
  ಪೂರ್ಣಾಙ್ಕ blake2s_final( blake2s_state *S, ಶೂನ್ಯ *out, size_t outlen );

  ಪೂರ್ಣಾಙ್ಕ blake2b_init( blake2b_state *S, size_t outlen );
  ಪೂರ್ಣಾಙ್ಕ blake2b_init_ಕೀಲಿ( blake2b_state *S, size_t outlen, ಧ್ರುವ ಶೂನ್ಯ *ಕೀಲಿ, size_t keylen );
  ಪೂರ್ಣಾಙ್ಕ blake2b_init_param (blake2b_state *S, ಧ್ರುವ blake2b_param *P)
    _GL_ATTRIBUTE_NONNULL ();
  ಪೂರ್ಣಾಙ್ಕ blake2b_update( blake2b_state *S, ಧ್ರುವ ಶೂನ್ಯ *in, size_t inlen );
  ಪೂರ್ಣಾಙ್ಕ blake2b_final( blake2b_state *S, ಶೂನ್ಯ *out, size_t outlen );

  ಪೂರ್ಣಾಙ್ಕ blake2sp_init( blake2sp_state *S, size_t outlen );
  ಪೂರ್ಣಾಙ್ಕ blake2sp_init_ಕೀಲಿ( blake2sp_state *S, size_t outlen, ಧ್ರುವ ಶೂನ್ಯ *ಕೀಲಿ, size_t keylen );
  ಪೂರ್ಣಾಙ್ಕ blake2sp_update( blake2sp_state *S, ಧ್ರುವ ಶೂನ್ಯ *in, size_t inlen );
  ಪೂರ್ಣಾಙ್ಕ blake2sp_final( blake2sp_state *S, ಶೂನ್ಯ *out, size_t outlen );

  ಪೂರ್ಣಾಙ್ಕ blake2bp_init( blake2bp_state *S, size_t outlen );
  ಪೂರ್ಣಾಙ್ಕ blake2bp_init_ಕೀಲಿ( blake2bp_state *S, size_t outlen, ಧ್ರುವ ಶೂನ್ಯ *ಕೀಲಿ, size_t keylen );
  ಪೂರ್ಣಾಙ್ಕ blake2bp_update( blake2bp_state *S, ಧ್ರುವ ಶೂನ್ಯ *in, size_t inlen );
  ಪೂರ್ಣಾಙ್ಕ blake2bp_final( blake2bp_state *S, ಶೂನ್ಯ *out, size_t outlen );

  /* Variable output length API */
  ಪೂರ್ಣಾಙ್ಕ blake2xs_init( blake2xs_state *S, ಧ್ರುವ size_t outlen );
  ಪೂರ್ಣಾಙ್ಕ blake2xs_init_ಕೀಲಿ( blake2xs_state *S, ಧ್ರುವ size_t outlen, ಧ್ರುವ ಶೂನ್ಯ *ಕೀಲಿ, size_t keylen );
  ಪೂರ್ಣಾಙ್ಕ blake2xs_update( blake2xs_state *S, ಧ್ರುವ ಶೂನ್ಯ *in, size_t inlen );
  ಪೂರ್ಣಾಙ್ಕ blake2xs_final(blake2xs_state *S, ಶೂನ್ಯ *out, size_t outlen);

  ಪೂರ್ಣಾಙ್ಕ blake2xb_init( blake2xb_state *S, ಧ್ರುವ size_t outlen );
  ಪೂರ್ಣಾಙ್ಕ blake2xb_init_ಕೀಲಿ( blake2xb_state *S, ಧ್ರುವ size_t outlen, ಧ್ರುವ ಶೂನ್ಯ *ಕೀಲಿ, size_t keylen );
  ಪೂರ್ಣಾಙ್ಕ blake2xb_update( blake2xb_state *S, ಧ್ರುವ ಶೂನ್ಯ *in, size_t inlen );
  ಪೂರ್ಣಾಙ್ಕ blake2xb_final(blake2xb_state *S, ಶೂನ್ಯ *out, size_t outlen);

  /* Simple API */
  ಪೂರ್ಣಾಙ್ಕ blake2s( ಶೂನ್ಯ *out, size_t outlen, ಧ್ರುವ ಶೂನ್ಯ *in, size_t inlen, ಧ್ರುವ ಶೂನ್ಯ *ಕೀಲಿ, size_t keylen );
  ಪೂರ್ಣಾಙ್ಕ blake2b( ಶೂನ್ಯ *out, size_t outlen, ಧ್ರುವ ಶೂನ್ಯ *in, size_t inlen, ಧ್ರುವ ಶೂನ್ಯ *ಕೀಲಿ, size_t keylen );

  ಪೂರ್ಣಾಙ್ಕ blake2sp( ಶೂನ್ಯ *out, size_t outlen, ಧ್ರುವ ಶೂನ್ಯ *in, size_t inlen, ಧ್ರುವ ಶೂನ್ಯ *ಕೀಲಿ, size_t keylen );
  ಪೂರ್ಣಾಙ್ಕ blake2bp( ಶೂನ್ಯ *out, size_t outlen, ಧ್ರುವ ಶೂನ್ಯ *in, size_t inlen, ಧ್ರುವ ಶೂನ್ಯ *ಕೀಲಿ, size_t keylen );

  ಪೂರ್ಣಾಙ್ಕ blake2xs( ಶೂನ್ಯ *out, size_t outlen, ಧ್ರುವ ಶೂನ್ಯ *in, size_t inlen, ಧ್ರುವ ಶೂನ್ಯ *ಕೀಲಿ, size_t keylen );
  ಪೂರ್ಣಾಙ್ಕ blake2xb( ಶೂನ್ಯ *out, size_t outlen, ಧ್ರುವ ಶೂನ್ಯ *in, size_t inlen, ಧ್ರುವ ಶೂನ್ಯ *ಕೀಲಿ, size_t keylen );

  /* This is simply an alias for blake2b */
  ಪೂರ್ಣಾಙ್ಕ blake2( ಶೂನ್ಯ *out, size_t outlen, ಧ್ರುವ ಶೂನ್ಯ *in, size_t inlen, ಧ್ರುವ ಶೂನ್ಯ *ಕೀಲಿ, size_t keylen );

#ಯದಿ defined(__cplusplus)
}
#endif

#endif
