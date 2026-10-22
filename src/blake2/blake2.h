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
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ BLAKE2_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BLAKE2_H

#ਅਨ੍ਤਰ੍ਭਾਵ <stddef.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdint.h>

/* Pack a structure if possible.  This might save space, and is not
   needed for correctness.  */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _MSC_VER
# define BLAKE2_PACKED(x) __pragma (pack (ਧੱਕੋ, 1)) x __pragma (pack (ਕੱਢੋ))
#ਨਹੀਂਤਾਂ
# define BLAKE2_PACKED(x) x _GL_ATTRIBUTE_PACKED
#ਜੇ_ਅੰਤ

#ਜੇ defined(__cplusplus)
ਬਾਹਰੀ "C" {
#ਜੇ_ਅੰਤ

  ਗਿਣਤੀ blake2s_constant
  {
    BLAKE2S_BLOCKBYTES = 64,
    BLAKE2S_OUTBYTES   = 32,
    BLAKE2S_KEYBYTES   = 32,
    BLAKE2S_SALTBYTES  = 8,
    BLAKE2S_PERSONALBYTES = 8
  };

  ਗਿਣਤੀ blake2b_constant
  {
    BLAKE2B_BLOCKBYTES = 128,
    BLAKE2B_OUTBYTES   = 64,
    BLAKE2B_KEYBYTES   = 64,
    BLAKE2B_SALTBYTES  = 16,
    BLAKE2B_PERSONALBYTES = 16
  };

  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ blake2s_state__
  {
    uint32_t h[8];
    uint32_t t[2];
    uint32_t f[2];
    uint8_t  buf[BLAKE2S_BLOCKBYTES];
    size_t   buflen;
    size_t   outlen;
    uint8_t  ਆਖਰੀ_ਗੰਢ;
  } blake2s_state;

  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ blake2b_state__
  {
    uint64_t h[8];
    uint64_t t[2];
    uint64_t f[2];
    uint8_t  buf[BLAKE2B_BLOCKBYTES];
    size_t   buflen;
    size_t   outlen;
    uint8_t  ਆਖਰੀ_ਗੰਢ;
  } blake2b_state;

  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ blake2sp_state__
  {
    blake2s_state S[8][1];
    blake2s_state R[1];
    uint8_t       buf[8 * BLAKE2S_BLOCKBYTES];
    size_t        buflen;
    size_t        outlen;
  } blake2sp_state;

  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ blake2bp_state__
  {
    blake2b_state S[4][1];
    blake2b_state R[1];
    uint8_t       buf[4 * BLAKE2B_BLOCKBYTES];
    size_t        buflen;
    size_t        outlen;
  } blake2bp_state;


  BLAKE2_PACKED(ਬਣਤਰ blake2s_param__
  {
    uint8_t  digest_ਲੰਮਾਈ; /* 1 */
    uint8_t  ਕੁੰਜੀ_ਲੰਮਾਈ;    /* 2 */
    uint8_t  fanout;        /* 3 */
    uint8_t  ਡੂੰਘਾਈ;         /* 4 */
    uint32_t ਪੱਤੀ_ਲੰਮਾਈ;   /* 8 */
    uint32_t ਗੰਢ_ਵਿੱਥ;  /* 12 */
    uint16_t xof_ਲੰਮਾਈ;    /* 14 */
    uint8_t  ਗੰਢ_ਡੂੰਘਾਈ;    /* 15 */
    uint8_t  inner_ਲੰਮਾਈ;  /* 16 */
    /* uint8_t  reserved[0]; */
    uint8_t  salt[BLAKE2S_SALTBYTES]; /* 24 */
    uint8_t  personal[BLAKE2S_PERSONALBYTES];  /* 32 */
  });

  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ blake2s_param__ blake2s_param;

  BLAKE2_PACKED(ਬਣਤਰ blake2b_param__
  {
    uint8_t  digest_ਲੰਮਾਈ; /* 1 */
    uint8_t  ਕੁੰਜੀ_ਲੰਮਾਈ;    /* 2 */
    uint8_t  fanout;        /* 3 */
    uint8_t  ਡੂੰਘਾਈ;         /* 4 */
    uint32_t ਪੱਤੀ_ਲੰਮਾਈ;   /* 8 */
    uint32_t ਗੰਢ_ਵਿੱਥ;   /* 12 */
    uint32_t xof_ਲੰਮਾਈ;    /* 16 */
    uint8_t  ਗੰਢ_ਡੂੰਘਾਈ;    /* 17 */
    uint8_t  inner_ਲੰਮਾਈ;  /* 18 */
    uint8_t  reserved[14];  /* 32 */
    uint8_t  salt[BLAKE2B_SALTBYTES]; /* 48 */
    uint8_t  personal[BLAKE2B_PERSONALBYTES];  /* 64 */
  });

  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ blake2b_param__ blake2b_param;

  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ blake2xs_state__
  {
    blake2s_state S[1];
    blake2s_param P[1];
  } blake2xs_state;

  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ blake2xb_state__
  {
    blake2b_state S[1];
    blake2b_param P[1];
  } blake2xb_state;

  /* Padded structs result in a compile-time error */
  ਗਿਣਤੀ {
    BLAKE2_DUMMY_1 = 1 / (ਆਕਾਰ (blake2s_param) == BLAKE2S_OUTBYTES),
    BLAKE2_DUMMY_2 = 1 / (ਆਕਾਰ (blake2b_param) == BLAKE2B_OUTBYTES)
  };

  /* Streaming API */
  ਪੂਰਨ blake2s_init( blake2s_state *S, size_t outlen );
  ਪੂਰਨ blake2s_init_ਕੁੰਜੀ( blake2s_state *S, size_t outlen, ਅਚਲ ਖਾਲੀ *ਕੁੰਜੀ, size_t keylen );
  ਪੂਰਨ blake2s_init_param( blake2s_state *S, ਅਚਲ blake2s_param *P );
  ਪੂਰਨ blake2s_update( blake2s_state *S, ਅਚਲ ਖਾਲੀ *ਵਿੱਚ, size_t inlen );
  ਪੂਰਨ blake2s_final( blake2s_state *S, ਖਾਲੀ *ਬਾਹਰਲਾ, size_t outlen );

  ਪੂਰਨ blake2b_init( blake2b_state *S, size_t outlen );
  ਪੂਰਨ blake2b_init_ਕੁੰਜੀ( blake2b_state *S, size_t outlen, ਅਚਲ ਖਾਲੀ *ਕੁੰਜੀ, size_t keylen );
  ਪੂਰਨ blake2b_init_param (blake2b_state *S, ਅਚਲ blake2b_param *P)
    _GL_ATTRIBUTE_NONNULL ();
  ਪੂਰਨ blake2b_update( blake2b_state *S, ਅਚਲ ਖਾਲੀ *ਵਿੱਚ, size_t inlen );
  ਪੂਰਨ blake2b_final( blake2b_state *S, ਖਾਲੀ *ਬਾਹਰਲਾ, size_t outlen );

  ਪੂਰਨ blake2sp_init( blake2sp_state *S, size_t outlen );
  ਪੂਰਨ blake2sp_init_ਕੁੰਜੀ( blake2sp_state *S, size_t outlen, ਅਚਲ ਖਾਲੀ *ਕੁੰਜੀ, size_t keylen );
  ਪੂਰਨ blake2sp_update( blake2sp_state *S, ਅਚਲ ਖਾਲੀ *ਵਿੱਚ, size_t inlen );
  ਪੂਰਨ blake2sp_final( blake2sp_state *S, ਖਾਲੀ *ਬਾਹਰਲਾ, size_t outlen );

  ਪੂਰਨ blake2bp_init( blake2bp_state *S, size_t outlen );
  ਪੂਰਨ blake2bp_init_ਕੁੰਜੀ( blake2bp_state *S, size_t outlen, ਅਚਲ ਖਾਲੀ *ਕੁੰਜੀ, size_t keylen );
  ਪੂਰਨ blake2bp_update( blake2bp_state *S, ਅਚਲ ਖਾਲੀ *ਵਿੱਚ, size_t inlen );
  ਪੂਰਨ blake2bp_final( blake2bp_state *S, ਖਾਲੀ *ਬਾਹਰਲਾ, size_t outlen );

  /* Variable output length API */
  ਪੂਰਨ blake2xs_init( blake2xs_state *S, ਅਚਲ size_t outlen );
  ਪੂਰਨ blake2xs_init_ਕੁੰਜੀ( blake2xs_state *S, ਅਚਲ size_t outlen, ਅਚਲ ਖਾਲੀ *ਕੁੰਜੀ, size_t keylen );
  ਪੂਰਨ blake2xs_update( blake2xs_state *S, ਅਚਲ ਖਾਲੀ *ਵਿੱਚ, size_t inlen );
  ਪੂਰਨ blake2xs_final(blake2xs_state *S, ਖਾਲੀ *ਬਾਹਰਲਾ, size_t outlen);

  ਪੂਰਨ blake2xb_init( blake2xb_state *S, ਅਚਲ size_t outlen );
  ਪੂਰਨ blake2xb_init_ਕੁੰਜੀ( blake2xb_state *S, ਅਚਲ size_t outlen, ਅਚਲ ਖਾਲੀ *ਕੁੰਜੀ, size_t keylen );
  ਪੂਰਨ blake2xb_update( blake2xb_state *S, ਅਚਲ ਖਾਲੀ *ਵਿੱਚ, size_t inlen );
  ਪੂਰਨ blake2xb_final(blake2xb_state *S, ਖਾਲੀ *ਬਾਹਰਲਾ, size_t outlen);

  /* Simple API */
  ਪੂਰਨ blake2s( ਖਾਲੀ *ਬਾਹਰਲਾ, size_t outlen, ਅਚਲ ਖਾਲੀ *ਵਿੱਚ, size_t inlen, ਅਚਲ ਖਾਲੀ *ਕੁੰਜੀ, size_t keylen );
  ਪੂਰਨ blake2b( ਖਾਲੀ *ਬਾਹਰਲਾ, size_t outlen, ਅਚਲ ਖਾਲੀ *ਵਿੱਚ, size_t inlen, ਅਚਲ ਖਾਲੀ *ਕੁੰਜੀ, size_t keylen );

  ਪੂਰਨ blake2sp( ਖਾਲੀ *ਬਾਹਰਲਾ, size_t outlen, ਅਚਲ ਖਾਲੀ *ਵਿੱਚ, size_t inlen, ਅਚਲ ਖਾਲੀ *ਕੁੰਜੀ, size_t keylen );
  ਪੂਰਨ blake2bp( ਖਾਲੀ *ਬਾਹਰਲਾ, size_t outlen, ਅਚਲ ਖਾਲੀ *ਵਿੱਚ, size_t inlen, ਅਚਲ ਖਾਲੀ *ਕੁੰਜੀ, size_t keylen );

  ਪੂਰਨ blake2xs( ਖਾਲੀ *ਬਾਹਰਲਾ, size_t outlen, ਅਚਲ ਖਾਲੀ *ਵਿੱਚ, size_t inlen, ਅਚਲ ਖਾਲੀ *ਕੁੰਜੀ, size_t keylen );
  ਪੂਰਨ blake2xb( ਖਾਲੀ *ਬਾਹਰਲਾ, size_t outlen, ਅਚਲ ਖਾਲੀ *ਵਿੱਚ, size_t inlen, ਅਚਲ ਖਾਲੀ *ਕੁੰਜੀ, size_t keylen );

  /* This is simply an alias for blake2b */
  ਪੂਰਨ blake2( ਖਾਲੀ *ਬਾਹਰਲਾ, size_t outlen, ਅਚਲ ਖਾਲੀ *ਵਿੱਚ, size_t inlen, ਅਚਲ ਖਾਲੀ *ਕੁੰਜੀ, size_t keylen );

#ਜੇ defined(__cplusplus)
}
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ
