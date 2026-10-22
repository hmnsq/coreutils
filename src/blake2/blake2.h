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
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ BLAKE2_H
#ವ್ಯಾಖ್ಯಾನಿಸು BLAKE2_H

#ಅನ್ತರ್ಭಾವ <stddef.h>
#ಅನ್ತರ್ಭಾವ <stdint.h>

/* Pack a structure if possible.  This might save space, and is not
   needed for correctness.  */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _MSC_VER
# define BLAKE2_PACKED(x) __pragma (pack (ತಳ್ಳು, 1)) x __pragma (pack (ತೆಗೆ))
#ಇಲ್ಲದಿದ್ದರೆ
# define BLAKE2_PACKED(x) x _GL_ATTRIBUTE_PACKED
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined(__cplusplus)
ಬಾಹ್ಯ "C" {
#ಆದರೆ_ಅಂತ್ಯ

  ಎಣಿಕೆ blake2s_constant
  {
    BLAKE2S_BLOCKBYTES = 64,
    BLAKE2S_OUTBYTES   = 32,
    BLAKE2S_KEYBYTES   = 32,
    BLAKE2S_SALTBYTES  = 8,
    BLAKE2S_PERSONALBYTES = 8
  };

  ಎಣಿಕೆ blake2b_constant
  {
    BLAKE2B_BLOCKBYTES = 128,
    BLAKE2B_OUTBYTES   = 64,
    BLAKE2B_KEYBYTES   = 64,
    BLAKE2B_SALTBYTES  = 16,
    BLAKE2B_PERSONALBYTES = 16
  };

  ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ blake2s_state__
  {
    uint32_t h[8];
    uint32_t t[2];
    uint32_t f[2];
    uint8_t  buf[BLAKE2S_BLOCKBYTES];
    size_t   buflen;
    size_t   outlen;
    uint8_t  ಕೊನೆಯ_ಗಂಟು;
  } blake2s_state;

  ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ blake2b_state__
  {
    uint64_t h[8];
    uint64_t t[2];
    uint64_t f[2];
    uint8_t  buf[BLAKE2B_BLOCKBYTES];
    size_t   buflen;
    size_t   outlen;
    uint8_t  ಕೊನೆಯ_ಗಂಟು;
  } blake2b_state;

  ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ blake2sp_state__
  {
    blake2s_state S[8][1];
    blake2s_state R[1];
    uint8_t       buf[8 * BLAKE2S_BLOCKBYTES];
    size_t        buflen;
    size_t        outlen;
  } blake2sp_state;

  ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ blake2bp_state__
  {
    blake2b_state S[4][1];
    blake2b_state R[1];
    uint8_t       buf[4 * BLAKE2B_BLOCKBYTES];
    size_t        buflen;
    size_t        outlen;
  } blake2bp_state;


  BLAKE2_PACKED(ರಚನೆ blake2s_param__
  {
    uint8_t  digest_ಉದ್ದಳತೆ; /* 1 */
    uint8_t  ಕೀಲಿ_ಉದ್ದಳತೆ;    /* 2 */
    uint8_t  fanout;        /* 3 */
    uint8_t  ಆಳ;         /* 4 */
    uint32_t ಎಲೆ_ಉದ್ದಳತೆ;   /* 8 */
    uint32_t ಗಂಟು_ಸ್ಥಾನಾಂತರ;  /* 12 */
    uint16_t xof_ಉದ್ದಳತೆ;    /* 14 */
    uint8_t  ಗಂಟು_ಆಳ;    /* 15 */
    uint8_t  inner_ಉದ್ದಳತೆ;  /* 16 */
    /* uint8_t  reserved[0]; */
    uint8_t  salt[BLAKE2S_SALTBYTES]; /* 24 */
    uint8_t  personal[BLAKE2S_PERSONALBYTES];  /* 32 */
  });

  ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ blake2s_param__ blake2s_param;

  BLAKE2_PACKED(ರಚನೆ blake2b_param__
  {
    uint8_t  digest_ಉದ್ದಳತೆ; /* 1 */
    uint8_t  ಕೀಲಿ_ಉದ್ದಳತೆ;    /* 2 */
    uint8_t  fanout;        /* 3 */
    uint8_t  ಆಳ;         /* 4 */
    uint32_t ಎಲೆ_ಉದ್ದಳತೆ;   /* 8 */
    uint32_t ಗಂಟು_ಸ್ಥಾನಾಂತರ;   /* 12 */
    uint32_t xof_ಉದ್ದಳತೆ;    /* 16 */
    uint8_t  ಗಂಟು_ಆಳ;    /* 17 */
    uint8_t  inner_ಉದ್ದಳತೆ;  /* 18 */
    uint8_t  reserved[14];  /* 32 */
    uint8_t  salt[BLAKE2B_SALTBYTES]; /* 48 */
    uint8_t  personal[BLAKE2B_PERSONALBYTES];  /* 64 */
  });

  ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ blake2b_param__ blake2b_param;

  ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ blake2xs_state__
  {
    blake2s_state S[1];
    blake2s_param P[1];
  } blake2xs_state;

  ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ blake2xb_state__
  {
    blake2b_state S[1];
    blake2b_param P[1];
  } blake2xb_state;

  /* Padded structs result in a compile-time error */
  ಎಣಿಕೆ {
    BLAKE2_DUMMY_1 = 1 / (ಗಾತ್ರ (blake2s_param) == BLAKE2S_OUTBYTES),
    BLAKE2_DUMMY_2 = 1 / (ಗಾತ್ರ (blake2b_param) == BLAKE2B_OUTBYTES)
  };

  /* Streaming API */
  ಪೂರ್ಣಾಂಕ blake2s_init( blake2s_state *S, size_t outlen );
  ಪೂರ್ಣಾಂಕ blake2s_init_ಕೀಲಿ( blake2s_state *S, size_t outlen, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಕೀಲಿ, size_t keylen );
  ಪೂರ್ಣಾಂಕ blake2s_init_param( blake2s_state *S, ಸ್ಥಿರಾಂಕ blake2s_param *P );
  ಪೂರ್ಣಾಂಕ blake2s_update( blake2s_state *S, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಒಳ, size_t inlen );
  ಪೂರ್ಣಾಂಕ blake2s_final( blake2s_state *S, ಶೂನ್ಯ *ಹೊರಹೋಗು, size_t outlen );

  ಪೂರ್ಣಾಂಕ blake2b_init( blake2b_state *S, size_t outlen );
  ಪೂರ್ಣಾಂಕ blake2b_init_ಕೀಲಿ( blake2b_state *S, size_t outlen, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಕೀಲಿ, size_t keylen );
  ಪೂರ್ಣಾಂಕ blake2b_init_param (blake2b_state *S, ಸ್ಥಿರಾಂಕ blake2b_param *P)
    _GL_ATTRIBUTE_NONNULL ();
  ಪೂರ್ಣಾಂಕ blake2b_update( blake2b_state *S, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಒಳ, size_t inlen );
  ಪೂರ್ಣಾಂಕ blake2b_final( blake2b_state *S, ಶೂನ್ಯ *ಹೊರಹೋಗು, size_t outlen );

  ಪೂರ್ಣಾಂಕ blake2sp_init( blake2sp_state *S, size_t outlen );
  ಪೂರ್ಣಾಂಕ blake2sp_init_ಕೀಲಿ( blake2sp_state *S, size_t outlen, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಕೀಲಿ, size_t keylen );
  ಪೂರ್ಣಾಂಕ blake2sp_update( blake2sp_state *S, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಒಳ, size_t inlen );
  ಪೂರ್ಣಾಂಕ blake2sp_final( blake2sp_state *S, ಶೂನ್ಯ *ಹೊರಹೋಗು, size_t outlen );

  ಪೂರ್ಣಾಂಕ blake2bp_init( blake2bp_state *S, size_t outlen );
  ಪೂರ್ಣಾಂಕ blake2bp_init_ಕೀಲಿ( blake2bp_state *S, size_t outlen, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಕೀಲಿ, size_t keylen );
  ಪೂರ್ಣಾಂಕ blake2bp_update( blake2bp_state *S, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಒಳ, size_t inlen );
  ಪೂರ್ಣಾಂಕ blake2bp_final( blake2bp_state *S, ಶೂನ್ಯ *ಹೊರಹೋಗು, size_t outlen );

  /* Variable output length API */
  ಪೂರ್ಣಾಂಕ blake2xs_init( blake2xs_state *S, ಸ್ಥಿರಾಂಕ size_t outlen );
  ಪೂರ್ಣಾಂಕ blake2xs_init_ಕೀಲಿ( blake2xs_state *S, ಸ್ಥಿರಾಂಕ size_t outlen, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಕೀಲಿ, size_t keylen );
  ಪೂರ್ಣಾಂಕ blake2xs_update( blake2xs_state *S, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಒಳ, size_t inlen );
  ಪೂರ್ಣಾಂಕ blake2xs_final(blake2xs_state *S, ಶೂನ್ಯ *ಹೊರಹೋಗು, size_t outlen);

  ಪೂರ್ಣಾಂಕ blake2xb_init( blake2xb_state *S, ಸ್ಥಿರಾಂಕ size_t outlen );
  ಪೂರ್ಣಾಂಕ blake2xb_init_ಕೀಲಿ( blake2xb_state *S, ಸ್ಥಿರಾಂಕ size_t outlen, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಕೀಲಿ, size_t keylen );
  ಪೂರ್ಣಾಂಕ blake2xb_update( blake2xb_state *S, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಒಳ, size_t inlen );
  ಪೂರ್ಣಾಂಕ blake2xb_final(blake2xb_state *S, ಶೂನ್ಯ *ಹೊರಹೋಗು, size_t outlen);

  /* Simple API */
  ಪೂರ್ಣಾಂಕ blake2s( ಶೂನ್ಯ *ಹೊರಹೋಗು, size_t outlen, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಒಳ, size_t inlen, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಕೀಲಿ, size_t keylen );
  ಪೂರ್ಣಾಂಕ blake2b( ಶೂನ್ಯ *ಹೊರಹೋಗು, size_t outlen, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಒಳ, size_t inlen, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಕೀಲಿ, size_t keylen );

  ಪೂರ್ಣಾಂಕ blake2sp( ಶೂನ್ಯ *ಹೊರಹೋಗು, size_t outlen, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಒಳ, size_t inlen, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಕೀಲಿ, size_t keylen );
  ಪೂರ್ಣಾಂಕ blake2bp( ಶೂನ್ಯ *ಹೊರಹೋಗು, size_t outlen, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಒಳ, size_t inlen, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಕೀಲಿ, size_t keylen );

  ಪೂರ್ಣಾಂಕ blake2xs( ಶೂನ್ಯ *ಹೊರಹೋಗು, size_t outlen, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಒಳ, size_t inlen, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಕೀಲಿ, size_t keylen );
  ಪೂರ್ಣಾಂಕ blake2xb( ಶೂನ್ಯ *ಹೊರಹೋಗು, size_t outlen, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಒಳ, size_t inlen, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಕೀಲಿ, size_t keylen );

  /* This is simply an alias for blake2b */
  ಪೂರ್ಣಾಂಕ blake2( ಶೂನ್ಯ *ಹೊರಹೋಗು, size_t outlen, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಒಳ, size_t inlen, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಕೀಲಿ, size_t keylen );

#ಆದರೆ defined(__cplusplus)
}
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ
