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
#నిర్వచితం_కాకపోతే BLAKE2_H
#నిర్వచించు BLAKE2_H

#అన్తర్భావ <stddef.h>
#అన్తర్భావ <stdint.h>

/* Pack a structure if possible.  This might save space, and is not
   needed for correctness.  */
#నిర్వచితమైతే _MSC_VER
# define BLAKE2_PACKED(x) __pragma (pack (నెట్టు, 1)) x __pragma (pack (తీయి))
#లేకపోతే
# define BLAKE2_PACKED(x) x _GL_ATTRIBUTE_PACKED
#అయితే_ముగింపు

#ఒకవేళ defined(__cplusplus)
బాహ్య "C" {
#అయితే_ముగింపు

  గణన blake2s_constant
  {
    BLAKE2S_BLOCKBYTES = 64,
    BLAKE2S_OUTBYTES   = 32,
    BLAKE2S_KEYBYTES   = 32,
    BLAKE2S_SALTBYTES  = 8,
    BLAKE2S_PERSONALBYTES = 8
  };

  గణన blake2b_constant
  {
    BLAKE2B_BLOCKBYTES = 128,
    BLAKE2B_OUTBYTES   = 64,
    BLAKE2B_KEYBYTES   = 64,
    BLAKE2B_SALTBYTES  = 16,
    BLAKE2B_PERSONALBYTES = 16
  };

  రకనిర్వచనం నిర్మాణం blake2s_state__
  {
    uint32_t h[8];
    uint32_t t[2];
    uint32_t f[2];
    uint8_t  buf[BLAKE2S_BLOCKBYTES];
    size_t   buflen;
    size_t   outlen;
    uint8_t  చివరి_కణుపు;
  } blake2s_state;

  రకనిర్వచనం నిర్మాణం blake2b_state__
  {
    uint64_t h[8];
    uint64_t t[2];
    uint64_t f[2];
    uint8_t  buf[BLAKE2B_BLOCKBYTES];
    size_t   buflen;
    size_t   outlen;
    uint8_t  చివరి_కణుపు;
  } blake2b_state;

  రకనిర్వచనం నిర్మాణం blake2sp_state__
  {
    blake2s_state S[8][1];
    blake2s_state R[1];
    uint8_t       buf[8 * BLAKE2S_BLOCKBYTES];
    size_t        buflen;
    size_t        outlen;
  } blake2sp_state;

  రకనిర్వచనం నిర్మాణం blake2bp_state__
  {
    blake2b_state S[4][1];
    blake2b_state R[1];
    uint8_t       buf[4 * BLAKE2B_BLOCKBYTES];
    size_t        buflen;
    size_t        outlen;
  } blake2bp_state;


  BLAKE2_PACKED(నిర్మాణం blake2s_param__
  {
    uint8_t  digest_నిడివి; /* 1 */
    uint8_t  కీలకం_నిడివి;    /* 2 */
    uint8_t  fanout;        /* 3 */
    uint8_t  లోతు;         /* 4 */
    uint32_t ఆకు_నిడివి;   /* 8 */
    uint32_t కణుపు_స్థానభ్రంశం;  /* 12 */
    uint16_t xof_నిడివి;    /* 14 */
    uint8_t  కణుపు_లోతు;    /* 15 */
    uint8_t  inner_నిడివి;  /* 16 */
    /* uint8_t  reserved[0]; */
    uint8_t  salt[BLAKE2S_SALTBYTES]; /* 24 */
    uint8_t  personal[BLAKE2S_PERSONALBYTES];  /* 32 */
  });

  రకనిర్వచనం నిర్మాణం blake2s_param__ blake2s_param;

  BLAKE2_PACKED(నిర్మాణం blake2b_param__
  {
    uint8_t  digest_నిడివి; /* 1 */
    uint8_t  కీలకం_నిడివి;    /* 2 */
    uint8_t  fanout;        /* 3 */
    uint8_t  లోతు;         /* 4 */
    uint32_t ఆకు_నిడివి;   /* 8 */
    uint32_t కణుపు_స్థానభ్రంశం;   /* 12 */
    uint32_t xof_నిడివి;    /* 16 */
    uint8_t  కణుపు_లోతు;    /* 17 */
    uint8_t  inner_నిడివి;  /* 18 */
    uint8_t  reserved[14];  /* 32 */
    uint8_t  salt[BLAKE2B_SALTBYTES]; /* 48 */
    uint8_t  personal[BLAKE2B_PERSONALBYTES];  /* 64 */
  });

  రకనిర్వచనం నిర్మాణం blake2b_param__ blake2b_param;

  రకనిర్వచనం నిర్మాణం blake2xs_state__
  {
    blake2s_state S[1];
    blake2s_param P[1];
  } blake2xs_state;

  రకనిర్వచనం నిర్మాణం blake2xb_state__
  {
    blake2b_state S[1];
    blake2b_param P[1];
  } blake2xb_state;

  /* Padded structs result in a compile-time error */
  గణన {
    BLAKE2_DUMMY_1 = 1 / (పరిమాణం (blake2s_param) == BLAKE2S_OUTBYTES),
    BLAKE2_DUMMY_2 = 1 / (పరిమాణం (blake2b_param) == BLAKE2B_OUTBYTES)
  };

  /* Streaming API */
  పూర్ణాంకం blake2s_init( blake2s_state *S, size_t outlen );
  పూర్ణాంకం blake2s_init_కీలకం( blake2s_state *S, size_t outlen, స్థిరరాశి శూన్య *కీలకం, size_t keylen );
  పూర్ణాంకం blake2s_init_param( blake2s_state *S, స్థిరరాశి blake2s_param *P );
  పూర్ణాంకం blake2s_update( blake2s_state *S, స్థిరరాశి శూన్య *లోపల, size_t inlen );
  పూర్ణాంకం blake2s_final( blake2s_state *S, శూన్య *బహిర్గతం, size_t outlen );

  పూర్ణాంకం blake2b_init( blake2b_state *S, size_t outlen );
  పూర్ణాంకం blake2b_init_కీలకం( blake2b_state *S, size_t outlen, స్థిరరాశి శూన్య *కీలకం, size_t keylen );
  పూర్ణాంకం blake2b_init_param (blake2b_state *S, స్థిరరాశి blake2b_param *P)
    _GL_ATTRIBUTE_NONNULL ();
  పూర్ణాంకం blake2b_update( blake2b_state *S, స్థిరరాశి శూన్య *లోపల, size_t inlen );
  పూర్ణాంకం blake2b_final( blake2b_state *S, శూన్య *బహిర్గతం, size_t outlen );

  పూర్ణాంకం blake2sp_init( blake2sp_state *S, size_t outlen );
  పూర్ణాంకం blake2sp_init_కీలకం( blake2sp_state *S, size_t outlen, స్థిరరాశి శూన్య *కీలకం, size_t keylen );
  పూర్ణాంకం blake2sp_update( blake2sp_state *S, స్థిరరాశి శూన్య *లోపల, size_t inlen );
  పూర్ణాంకం blake2sp_final( blake2sp_state *S, శూన్య *బహిర్గతం, size_t outlen );

  పూర్ణాంకం blake2bp_init( blake2bp_state *S, size_t outlen );
  పూర్ణాంకం blake2bp_init_కీలకం( blake2bp_state *S, size_t outlen, స్థిరరాశి శూన్య *కీలకం, size_t keylen );
  పూర్ణాంకం blake2bp_update( blake2bp_state *S, స్థిరరాశి శూన్య *లోపల, size_t inlen );
  పూర్ణాంకం blake2bp_final( blake2bp_state *S, శూన్య *బహిర్గతం, size_t outlen );

  /* Variable output length API */
  పూర్ణాంకం blake2xs_init( blake2xs_state *S, స్థిరరాశి size_t outlen );
  పూర్ణాంకం blake2xs_init_కీలకం( blake2xs_state *S, స్థిరరాశి size_t outlen, స్థిరరాశి శూన్య *కీలకం, size_t keylen );
  పూర్ణాంకం blake2xs_update( blake2xs_state *S, స్థిరరాశి శూన్య *లోపల, size_t inlen );
  పూర్ణాంకం blake2xs_final(blake2xs_state *S, శూన్య *బహిర్గతం, size_t outlen);

  పూర్ణాంకం blake2xb_init( blake2xb_state *S, స్థిరరాశి size_t outlen );
  పూర్ణాంకం blake2xb_init_కీలకం( blake2xb_state *S, స్థిరరాశి size_t outlen, స్థిరరాశి శూన్య *కీలకం, size_t keylen );
  పూర్ణాంకం blake2xb_update( blake2xb_state *S, స్థిరరాశి శూన్య *లోపల, size_t inlen );
  పూర్ణాంకం blake2xb_final(blake2xb_state *S, శూన్య *బహిర్గతం, size_t outlen);

  /* Simple API */
  పూర్ణాంకం blake2s( శూన్య *బహిర్గతం, size_t outlen, స్థిరరాశి శూన్య *లోపల, size_t inlen, స్థిరరాశి శూన్య *కీలకం, size_t keylen );
  పూర్ణాంకం blake2b( శూన్య *బహిర్గతం, size_t outlen, స్థిరరాశి శూన్య *లోపల, size_t inlen, స్థిరరాశి శూన్య *కీలకం, size_t keylen );

  పూర్ణాంకం blake2sp( శూన్య *బహిర్గతం, size_t outlen, స్థిరరాశి శూన్య *లోపల, size_t inlen, స్థిరరాశి శూన్య *కీలకం, size_t keylen );
  పూర్ణాంకం blake2bp( శూన్య *బహిర్గతం, size_t outlen, స్థిరరాశి శూన్య *లోపల, size_t inlen, స్థిరరాశి శూన్య *కీలకం, size_t keylen );

  పూర్ణాంకం blake2xs( శూన్య *బహిర్గతం, size_t outlen, స్థిరరాశి శూన్య *లోపల, size_t inlen, స్థిరరాశి శూన్య *కీలకం, size_t keylen );
  పూర్ణాంకం blake2xb( శూన్య *బహిర్గతం, size_t outlen, స్థిరరాశి శూన్య *లోపల, size_t inlen, స్థిరరాశి శూన్య *కీలకం, size_t keylen );

  /* This is simply an alias for blake2b */
  పూర్ణాంకం blake2( శూన్య *బహిర్గతం, size_t outlen, స్థిరరాశి శూన్య *లోపల, size_t inlen, స్థిరరాశి శూన్య *కీలకం, size_t keylen );

#ఒకవేళ defined(__cplusplus)
}
#అయితే_ముగింపు

#అయితే_ముగింపు
