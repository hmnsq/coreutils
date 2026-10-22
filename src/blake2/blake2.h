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
#अगर_अघोषित BLAKE2_H
#घोषणा BLAKE2_H

#समावेश <stddef.h>
#समावेश <stdint.h>

/* Pack a structure if possible.  This might save space, and is not
   needed for correctness.  */
#अगर_घोषित _MSC_VER
# define BLAKE2_PACKED(x) __pragma (pack (धकेलो, 1)) x __pragma (pack (निकालो))
#अन्यथा
# define BLAKE2_PACKED(x) x _GL_ATTRIBUTE_PACKED
#पूर्ण_अगर

#अगर defined(__cplusplus)
बाह्य "C" {
#पूर्ण_अगर

  क्रमागत blake2s_constant
  {
    BLAKE2S_BLOCKBYTES = 64,
    BLAKE2S_OUTBYTES   = 32,
    BLAKE2S_KEYBYTES   = 32,
    BLAKE2S_SALTBYTES  = 8,
    BLAKE2S_PERSONALBYTES = 8
  };

  क्रमागत blake2b_constant
  {
    BLAKE2B_BLOCKBYTES = 128,
    BLAKE2B_OUTBYTES   = 64,
    BLAKE2B_KEYBYTES   = 64,
    BLAKE2B_SALTBYTES  = 16,
    BLAKE2B_PERSONALBYTES = 16
  };

  प्रकार काष्ठा blake2s_state__
  {
    uint32_t h[8];
    uint32_t t[2];
    uint32_t f[2];
    uint8_t  buf[BLAKE2S_BLOCKBYTES];
    माप_प्रकार   buflen;
    माप_प्रकार   outlen;
    uint8_t  last_गाँठ;
  } blake2s_state;

  प्रकार काष्ठा blake2b_state__
  {
    uint64_t h[8];
    uint64_t t[2];
    uint64_t f[2];
    uint8_t  buf[BLAKE2B_BLOCKBYTES];
    माप_प्रकार   buflen;
    माप_प्रकार   outlen;
    uint8_t  last_गाँठ;
  } blake2b_state;

  प्रकार काष्ठा blake2sp_state__
  {
    blake2s_state S[8][1];
    blake2s_state R[1];
    uint8_t       buf[8 * BLAKE2S_BLOCKBYTES];
    माप_प्रकार        buflen;
    माप_प्रकार        outlen;
  } blake2sp_state;

  प्रकार काष्ठा blake2bp_state__
  {
    blake2b_state S[4][1];
    blake2b_state R[1];
    uint8_t       buf[4 * BLAKE2B_BLOCKBYTES];
    माप_प्रकार        buflen;
    माप_प्रकार        outlen;
  } blake2bp_state;


  BLAKE2_PACKED(काष्ठा blake2s_param__
  {
    uint8_t  digest_लंबाई; /* 1 */
    uint8_t  कुंजी_लंबाई;    /* 2 */
    uint8_t  fanout;        /* 3 */
    uint8_t  depth;         /* 4 */
    uint32_t leaf_लंबाई;   /* 8 */
    uint32_t गाँठ_offset;  /* 12 */
    uint16_t xof_लंबाई;    /* 14 */
    uint8_t  गाँठ_depth;    /* 15 */
    uint8_t  inner_लंबाई;  /* 16 */
    /* uint8_t  reserved[0]; */
    uint8_t  salt[BLAKE2S_SALTBYTES]; /* 24 */
    uint8_t  personal[BLAKE2S_PERSONALBYTES];  /* 32 */
  });

  प्रकार काष्ठा blake2s_param__ blake2s_param;

  BLAKE2_PACKED(काष्ठा blake2b_param__
  {
    uint8_t  digest_लंबाई; /* 1 */
    uint8_t  कुंजी_लंबाई;    /* 2 */
    uint8_t  fanout;        /* 3 */
    uint8_t  depth;         /* 4 */
    uint32_t leaf_लंबाई;   /* 8 */
    uint32_t गाँठ_offset;   /* 12 */
    uint32_t xof_लंबाई;    /* 16 */
    uint8_t  गाँठ_depth;    /* 17 */
    uint8_t  inner_लंबाई;  /* 18 */
    uint8_t  reserved[14];  /* 32 */
    uint8_t  salt[BLAKE2B_SALTBYTES]; /* 48 */
    uint8_t  personal[BLAKE2B_PERSONALBYTES];  /* 64 */
  });

  प्रकार काष्ठा blake2b_param__ blake2b_param;

  प्रकार काष्ठा blake2xs_state__
  {
    blake2s_state S[1];
    blake2s_param P[1];
  } blake2xs_state;

  प्रकार काष्ठा blake2xb_state__
  {
    blake2b_state S[1];
    blake2b_param P[1];
  } blake2xb_state;

  /* Padded structs result in a compile-time error */
  क्रमागत {
    BLAKE2_DUMMY_1 = 1 / (माप (blake2s_param) == BLAKE2S_OUTBYTES),
    BLAKE2_DUMMY_2 = 1 / (माप (blake2b_param) == BLAKE2B_OUTBYTES)
  };

  /* Streaming API */
  पूर्णांक blake2s_init( blake2s_state *S, माप_प्रकार outlen );
  पूर्णांक blake2s_init_कुंजी( blake2s_state *S, माप_प्रकार outlen, स्थिर व्योम *कुंजी, माप_प्रकार keylen );
  पूर्णांक blake2s_init_param( blake2s_state *S, स्थिर blake2s_param *P );
  पूर्णांक blake2s_update( blake2s_state *S, स्थिर व्योम *in, माप_प्रकार inlen );
  पूर्णांक blake2s_final( blake2s_state *S, व्योम *out, माप_प्रकार outlen );

  पूर्णांक blake2b_init( blake2b_state *S, माप_प्रकार outlen );
  पूर्णांक blake2b_init_कुंजी( blake2b_state *S, माप_प्रकार outlen, स्थिर व्योम *कुंजी, माप_प्रकार keylen );
  पूर्णांक blake2b_init_param (blake2b_state *S, स्थिर blake2b_param *P)
    _GL_ATTRIBUTE_NONNULL ();
  पूर्णांक blake2b_update( blake2b_state *S, स्थिर व्योम *in, माप_प्रकार inlen );
  पूर्णांक blake2b_final( blake2b_state *S, व्योम *out, माप_प्रकार outlen );

  पूर्णांक blake2sp_init( blake2sp_state *S, माप_प्रकार outlen );
  पूर्णांक blake2sp_init_कुंजी( blake2sp_state *S, माप_प्रकार outlen, स्थिर व्योम *कुंजी, माप_प्रकार keylen );
  पूर्णांक blake2sp_update( blake2sp_state *S, स्थिर व्योम *in, माप_प्रकार inlen );
  पूर्णांक blake2sp_final( blake2sp_state *S, व्योम *out, माप_प्रकार outlen );

  पूर्णांक blake2bp_init( blake2bp_state *S, माप_प्रकार outlen );
  पूर्णांक blake2bp_init_कुंजी( blake2bp_state *S, माप_प्रकार outlen, स्थिर व्योम *कुंजी, माप_प्रकार keylen );
  पूर्णांक blake2bp_update( blake2bp_state *S, स्थिर व्योम *in, माप_प्रकार inlen );
  पूर्णांक blake2bp_final( blake2bp_state *S, व्योम *out, माप_प्रकार outlen );

  /* Variable output length API */
  पूर्णांक blake2xs_init( blake2xs_state *S, स्थिर माप_प्रकार outlen );
  पूर्णांक blake2xs_init_कुंजी( blake2xs_state *S, स्थिर माप_प्रकार outlen, स्थिर व्योम *कुंजी, माप_प्रकार keylen );
  पूर्णांक blake2xs_update( blake2xs_state *S, स्थिर व्योम *in, माप_प्रकार inlen );
  पूर्णांक blake2xs_final(blake2xs_state *S, व्योम *out, माप_प्रकार outlen);

  पूर्णांक blake2xb_init( blake2xb_state *S, स्थिर माप_प्रकार outlen );
  पूर्णांक blake2xb_init_कुंजी( blake2xb_state *S, स्थिर माप_प्रकार outlen, स्थिर व्योम *कुंजी, माप_प्रकार keylen );
  पूर्णांक blake2xb_update( blake2xb_state *S, स्थिर व्योम *in, माप_प्रकार inlen );
  पूर्णांक blake2xb_final(blake2xb_state *S, व्योम *out, माप_प्रकार outlen);

  /* Simple API */
  पूर्णांक blake2s( व्योम *out, माप_प्रकार outlen, स्थिर व्योम *in, माप_प्रकार inlen, स्थिर व्योम *कुंजी, माप_प्रकार keylen );
  पूर्णांक blake2b( व्योम *out, माप_प्रकार outlen, स्थिर व्योम *in, माप_प्रकार inlen, स्थिर व्योम *कुंजी, माप_प्रकार keylen );

  पूर्णांक blake2sp( व्योम *out, माप_प्रकार outlen, स्थिर व्योम *in, माप_प्रकार inlen, स्थिर व्योम *कुंजी, माप_प्रकार keylen );
  पूर्णांक blake2bp( व्योम *out, माप_प्रकार outlen, स्थिर व्योम *in, माप_प्रकार inlen, स्थिर व्योम *कुंजी, माप_प्रकार keylen );

  पूर्णांक blake2xs( व्योम *out, माप_प्रकार outlen, स्थिर व्योम *in, माप_प्रकार inlen, स्थिर व्योम *कुंजी, माप_प्रकार keylen );
  पूर्णांक blake2xb( व्योम *out, माप_प्रकार outlen, स्थिर व्योम *in, माप_प्रकार inlen, स्थिर व्योम *कुंजी, माप_प्रकार keylen );

  /* This is simply an alias for blake2b */
  पूर्णांक blake2( व्योम *out, माप_प्रकार outlen, स्थिर व्योम *in, माप_प्रकार inlen, स्थिर व्योम *कुंजी, माप_प्रकार keylen );

#अगर defined(__cplusplus)
}
#पूर्ण_अगर

#पूर्ण_अगर
