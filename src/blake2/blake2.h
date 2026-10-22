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
#यदि_अपरिभाषितम् BLAKE2_H
#परिभाषय BLAKE2_H

#अन्तर्भाव <stddef.h>
#अन्तर्भाव <stdint.h>

/* Pack a structure if possible.  This might save space, and is not
   needed for correctness.  */
#यदि_परिभाषितम् _MSC_VER
# define BLAKE2_PACKED(x) __pragma (pack (प्रक्षिप, 1)) x __pragma (pack (निष्कासय))
#अन्यथा
# define BLAKE2_PACKED(x) x _GL_ATTRIBUTE_PACKED
#यदि_अन्तः

#यदि defined(__cplusplus)
बाह्य "C" {
#यदि_अन्तः

  गणना blake2s_constant
  {
    BLAKE2S_BLOCKBYTES = 64,
    BLAKE2S_OUTBYTES   = 32,
    BLAKE2S_KEYBYTES   = 32,
    BLAKE2S_SALTBYTES  = 8,
    BLAKE2S_PERSONALBYTES = 8
  };

  गणना blake2b_constant
  {
    BLAKE2B_BLOCKBYTES = 128,
    BLAKE2B_OUTBYTES   = 64,
    BLAKE2B_KEYBYTES   = 64,
    BLAKE2B_SALTBYTES  = 16,
    BLAKE2B_PERSONALBYTES = 16
  };

  प्रकारनिर्देश संरचना blake2s_state__
  {
    uint32_t h[8];
    uint32_t t[2];
    uint32_t f[2];
    uint8_t  buf[BLAKE2S_BLOCKBYTES];
    size_t   buflen;
    size_t   outlen;
    uint8_t  चरमः_ग्रन्थिः;
  } blake2s_state;

  प्रकारनिर्देश संरचना blake2b_state__
  {
    uint64_t h[8];
    uint64_t t[2];
    uint64_t f[2];
    uint8_t  buf[BLAKE2B_BLOCKBYTES];
    size_t   buflen;
    size_t   outlen;
    uint8_t  चरमः_ग्रन्थिः;
  } blake2b_state;

  प्रकारनिर्देश संरचना blake2sp_state__
  {
    blake2s_state S[8][1];
    blake2s_state R[1];
    uint8_t       buf[8 * BLAKE2S_BLOCKBYTES];
    size_t        buflen;
    size_t        outlen;
  } blake2sp_state;

  प्रकारनिर्देश संरचना blake2bp_state__
  {
    blake2b_state S[4][1];
    blake2b_state R[1];
    uint8_t       buf[4 * BLAKE2B_BLOCKBYTES];
    size_t        buflen;
    size_t        outlen;
  } blake2bp_state;


  BLAKE2_PACKED(संरचना blake2s_param__
  {
    uint8_t  digest_दीर्घता; /* 1 */
    uint8_t  कुञ्चिका_दीर्घता;    /* 2 */
    uint8_t  fanout;        /* 3 */
    uint8_t  गभीरता;         /* 4 */
    uint32_t पर्णम्_दीर्घता;   /* 8 */
    uint32_t ग्रन्थिः_विस्थापनम्;  /* 12 */
    uint16_t xof_दीर्घता;    /* 14 */
    uint8_t  ग्रन्थिः_गभीरता;    /* 15 */
    uint8_t  inner_दीर्घता;  /* 16 */
    /* uint8_t  reserved[0]; */
    uint8_t  salt[BLAKE2S_SALTBYTES]; /* 24 */
    uint8_t  personal[BLAKE2S_PERSONALBYTES];  /* 32 */
  });

  प्रकारनिर्देश संरचना blake2s_param__ blake2s_param;

  BLAKE2_PACKED(संरचना blake2b_param__
  {
    uint8_t  digest_दीर्घता; /* 1 */
    uint8_t  कुञ्चिका_दीर्घता;    /* 2 */
    uint8_t  fanout;        /* 3 */
    uint8_t  गभीरता;         /* 4 */
    uint32_t पर्णम्_दीर्घता;   /* 8 */
    uint32_t ग्रन्थिः_विस्थापनम्;   /* 12 */
    uint32_t xof_दीर्घता;    /* 16 */
    uint8_t  ग्रन्थिः_गभीरता;    /* 17 */
    uint8_t  inner_दीर्घता;  /* 18 */
    uint8_t  reserved[14];  /* 32 */
    uint8_t  salt[BLAKE2B_SALTBYTES]; /* 48 */
    uint8_t  personal[BLAKE2B_PERSONALBYTES];  /* 64 */
  });

  प्रकारनिर्देश संरचना blake2b_param__ blake2b_param;

  प्रकारनिर्देश संरचना blake2xs_state__
  {
    blake2s_state S[1];
    blake2s_param P[1];
  } blake2xs_state;

  प्रकारनिर्देश संरचना blake2xb_state__
  {
    blake2b_state S[1];
    blake2b_param P[1];
  } blake2xb_state;

  /* Padded structs result in a compile-time error */
  गणना {
    BLAKE2_DUMMY_1 = 1 / (परिमाण (blake2s_param) == BLAKE2S_OUTBYTES),
    BLAKE2_DUMMY_2 = 1 / (परिमाण (blake2b_param) == BLAKE2B_OUTBYTES)
  };

  /* Streaming API */
  पूर्णाङ्क blake2s_init( blake2s_state *S, size_t outlen );
  पूर्णाङ्क blake2s_init_कुञ्चिका( blake2s_state *S, size_t outlen, ध्रुव शून्य *कुञ्चिका, size_t keylen );
  पूर्णाङ्क blake2s_init_param( blake2s_state *S, ध्रुव blake2s_param *P );
  पूर्णाङ्क blake2s_update( blake2s_state *S, ध्रुव शून्य *अन्तर्, size_t inlen );
  पूर्णाङ्क blake2s_final( blake2s_state *S, शून्य *बहिर्गमनम्, size_t outlen );

  पूर्णाङ्क blake2b_init( blake2b_state *S, size_t outlen );
  पूर्णाङ्क blake2b_init_कुञ्चिका( blake2b_state *S, size_t outlen, ध्रुव शून्य *कुञ्चिका, size_t keylen );
  पूर्णाङ्क blake2b_init_param (blake2b_state *S, ध्रुव blake2b_param *P)
    _GL_ATTRIBUTE_NONNULL ();
  पूर्णाङ्क blake2b_update( blake2b_state *S, ध्रुव शून्य *अन्तर्, size_t inlen );
  पूर्णाङ्क blake2b_final( blake2b_state *S, शून्य *बहिर्गमनम्, size_t outlen );

  पूर्णाङ्क blake2sp_init( blake2sp_state *S, size_t outlen );
  पूर्णाङ्क blake2sp_init_कुञ्चिका( blake2sp_state *S, size_t outlen, ध्रुव शून्य *कुञ्चिका, size_t keylen );
  पूर्णाङ्क blake2sp_update( blake2sp_state *S, ध्रुव शून्य *अन्तर्, size_t inlen );
  पूर्णाङ्क blake2sp_final( blake2sp_state *S, शून्य *बहिर्गमनम्, size_t outlen );

  पूर्णाङ्क blake2bp_init( blake2bp_state *S, size_t outlen );
  पूर्णाङ्क blake2bp_init_कुञ्चिका( blake2bp_state *S, size_t outlen, ध्रुव शून्य *कुञ्चिका, size_t keylen );
  पूर्णाङ्क blake2bp_update( blake2bp_state *S, ध्रुव शून्य *अन्तर्, size_t inlen );
  पूर्णाङ्क blake2bp_final( blake2bp_state *S, शून्य *बहिर्गमनम्, size_t outlen );

  /* Variable output length API */
  पूर्णाङ्क blake2xs_init( blake2xs_state *S, ध्रुव size_t outlen );
  पूर्णाङ्क blake2xs_init_कुञ्चिका( blake2xs_state *S, ध्रुव size_t outlen, ध्रुव शून्य *कुञ्चिका, size_t keylen );
  पूर्णाङ्क blake2xs_update( blake2xs_state *S, ध्रुव शून्य *अन्तर्, size_t inlen );
  पूर्णाङ्क blake2xs_final(blake2xs_state *S, शून्य *बहिर्गमनम्, size_t outlen);

  पूर्णाङ्क blake2xb_init( blake2xb_state *S, ध्रुव size_t outlen );
  पूर्णाङ्क blake2xb_init_कुञ्चिका( blake2xb_state *S, ध्रुव size_t outlen, ध्रुव शून्य *कुञ्चिका, size_t keylen );
  पूर्णाङ्क blake2xb_update( blake2xb_state *S, ध्रुव शून्य *अन्तर्, size_t inlen );
  पूर्णाङ्क blake2xb_final(blake2xb_state *S, शून्य *बहिर्गमनम्, size_t outlen);

  /* Simple API */
  पूर्णाङ्क blake2s( शून्य *बहिर्गमनम्, size_t outlen, ध्रुव शून्य *अन्तर्, size_t inlen, ध्रुव शून्य *कुञ्चिका, size_t keylen );
  पूर्णाङ्क blake2b( शून्य *बहिर्गमनम्, size_t outlen, ध्रुव शून्य *अन्तर्, size_t inlen, ध्रुव शून्य *कुञ्चिका, size_t keylen );

  पूर्णाङ्क blake2sp( शून्य *बहिर्गमनम्, size_t outlen, ध्रुव शून्य *अन्तर्, size_t inlen, ध्रुव शून्य *कुञ्चिका, size_t keylen );
  पूर्णाङ्क blake2bp( शून्य *बहिर्गमनम्, size_t outlen, ध्रुव शून्य *अन्तर्, size_t inlen, ध्रुव शून्य *कुञ्चिका, size_t keylen );

  पूर्णाङ्क blake2xs( शून्य *बहिर्गमनम्, size_t outlen, ध्रुव शून्य *अन्तर्, size_t inlen, ध्रुव शून्य *कुञ्चिका, size_t keylen );
  पूर्णाङ्क blake2xb( शून्य *बहिर्गमनम्, size_t outlen, ध्रुव शून्य *अन्तर्, size_t inlen, ध्रुव शून्य *कुञ्चिका, size_t keylen );

  /* This is simply an alias for blake2b */
  पूर्णाङ्क blake2( शून्य *बहिर्गमनम्, size_t outlen, ध्रुव शून्य *अन्तर्, size_t inlen, ध्रुव शून्य *कुञ्चिका, size_t keylen );

#यदि defined(__cplusplus)
}
#यदि_अन्तः

#यदि_अन्तः
