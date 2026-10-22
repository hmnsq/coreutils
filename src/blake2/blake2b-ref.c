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

#నిర్వచితమైతే HAVE_CONFIG_H
# include <config.h>
#అయితే_ముగింపు

#అన్తర్భావ <stdint.h>
#అన్తర్భావ <string.h>
#అన్తర్భావ <stdio.h>

#అన్తర్భావ "blake2.h"
#అన్తర్భావ "blake2-impl.h"

స్థిర స్థిరరాశి uint64_t blake2b_IV[8] =
{
  0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL,
  0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL,
  0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL,
  0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL
};

స్థిర స్థిరరాశి uint8_t blake2b_sigma[12][16] =
{
  {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15 } ,
  { 14, 10,  4,  8,  9, 15, 13,  6,  1, 12,  0,  2, 11,  7,  5,  3 } ,
  { 11,  8, 12,  0,  5,  2, 15, 13, 10, 14,  3,  6,  7,  1,  9,  4 } ,
  {  7,  9,  3,  1, 13, 12, 11, 14,  2,  6,  5, 10,  4,  0, 15,  8 } ,
  {  9,  0,  5,  7,  2,  4, 10, 15, 14,  1, 11, 12,  6,  8,  3, 13 } ,
  {  2, 12,  6, 10,  0, 11,  8,  3,  4, 13,  7,  5, 15, 14,  1,  9 } ,
  { 12,  5,  1, 15, 14, 13,  4, 10,  0,  7,  6,  3,  9,  2,  8, 11 } ,
  { 13, 11,  7, 14, 12,  1,  3,  9,  5,  0, 15,  4,  8,  6,  2, 10 } ,
  {  6, 15, 14,  9, 11,  3,  0,  8, 12,  2, 13,  7,  1,  4, 10,  5 } ,
  { 10,  2,  8,  4,  7,  6,  1,  5, 15, 11,  9, 14,  3, 12, 13 , 0 } ,
  {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15 } ,
  { 14, 10,  4,  8,  9, 15, 13,  6,  1, 12,  0,  2, 11,  7,  5,  3 }
};


స్థిర శూన్య blake2b_సమితి_lastnode( blake2b_state *S )
{
  S->f[1] = (uint64_t)-1;
}

/* Some helper functions, not necessarily useful */
స్థిర పూర్ణాంకం blake2b_is_lastblock( స్థిరరాశి blake2b_state *S )
{
  తిరిగి S->f[0] != 0;
}

స్థిర శూన్య blake2b_సమితి_lastblock( blake2b_state *S )
{
  ఒకవేళ( S->చివరి_కణుపు ) blake2b_సమితి_lastnode( S );

  S->f[0] = (uint64_t)-1;
}

స్థిర శూన్య blake2b_increment_counter( blake2b_state *S, స్థిరరాశి uint64_t inc )
{
  S->t[0] += inc;
  S->t[1] += ( S->t[0] < inc );
}

స్థిర శూన్య blake2b_init0( blake2b_state *S )
{
  size_t i;
  memset( S, 0, పరిమాణం( blake2b_state ) );

  కొరకు( i = 0; i < 8; ++i ) S->h[i] = blake2b_IV[i];
}

/* init xors IV with input parameter block */
పూర్ణాంకం blake2b_init_param( blake2b_state *S, స్థిరరాశి blake2b_param *P )
{
  స్థిరరాశి uint8_t *p = ( స్థిరరాశి uint8_t * )( P );
  size_t i;

  blake2b_init0( S );

  /* IV XOR ParamBlock */
  కొరకు( i = 0; i < 8; ++i )
    S->h[i] ^= load64( p + పరిమాణం( S->h[i] ) * i );

  S->outlen = P->digest_నిడివి;
  తిరిగి 0;
}



పూర్ణాంకం blake2b_init( blake2b_state *S, size_t outlen )
{
  blake2b_param P[1];

  ఒకవేళ ( ( !outlen ) || ( outlen > BLAKE2B_OUTBYTES ) ) తిరిగి -1;

  P->digest_నిడివి = (uint8_t)outlen;
  P->కీలకం_నిడివి    = 0;
  P->fanout        = 1;
  P->లోతు         = 1;
  store32( &P->ఆకు_నిడివి, 0 );
  store32( &P->కణుపు_స్థానభ్రంశం, 0 );
  store32( &P->xof_నిడివి, 0 );
  P->కణుపు_లోతు    = 0;
  P->inner_నిడివి  = 0;
  memset( P->reserved, 0, పరిమాణం( P->reserved ) );
  memset( P->salt,     0, పరిమాణం( P->salt ) );
  memset( P->personal, 0, పరిమాణం( P->personal ) );
  తిరిగి blake2b_init_param( S, P );
}


పూర్ణాంకం blake2b_init_కీలకం( blake2b_state *S, size_t outlen, స్థిరరాశి శూన్య *కీలకం, size_t keylen )
{
  blake2b_param P[1];

  ఒకవేళ ( ( !outlen ) || ( outlen > BLAKE2B_OUTBYTES ) ) తిరిగి -1;

  ఒకవేళ ( !కీలకం || !keylen || keylen > BLAKE2B_KEYBYTES ) తిరిగి -1;

  P->digest_నిడివి = (uint8_t)outlen;
  P->కీలకం_నిడివి    = (uint8_t)keylen;
  P->fanout        = 1;
  P->లోతు         = 1;
  store32( &P->ఆకు_నిడివి, 0 );
  store32( &P->కణుపు_స్థానభ్రంశం, 0 );
  store32( &P->xof_నిడివి, 0 );
  P->కణుపు_లోతు    = 0;
  P->inner_నిడివి  = 0;
  memset( P->reserved, 0, పరిమాణం( P->reserved ) );
  memset( P->salt,     0, పరిమాణం( P->salt ) );
  memset( P->personal, 0, పరిమాణం( P->personal ) );

  ఒకవేళ( blake2b_init_param( S, P ) < 0 ) తిరిగి -1;

  {
    uint8_t ఖండం[BLAKE2B_BLOCKBYTES];
    memset( ఖండం, 0, BLAKE2B_BLOCKBYTES );
    జ్ఞాపకం_నకలు( ఖండం, కీలకం, keylen );
    blake2b_update( S, ఖండం, BLAKE2B_BLOCKBYTES );
    secure_zero_జ్ఞాపకం( ఖండం, BLAKE2B_BLOCKBYTES ); /* Burn the key from stack */
  }
  తిరిగి 0;
}

#నిర్వచించు G(r,i,a,b,c,d)                      \
  చేయి {                                      \
    a = a + b + m[blake2b_sigma[r][2 * i + 0]]; \
    d = rotr64(d ^ a, 32);                  \
    c = c + d;                              \
    b = rotr64(b ^ c, 24);                  \
    a = a + b + m[blake2b_sigma[r][2 * i + 1]]; \
    d = rotr64(d ^ a, 16);                  \
    c = c + d;                              \
    b = rotr64(b ^ c, 63);                  \
  } అయితే(0)

#నిర్వచించు విడత(r)                    \
  చేయి {                              \
    G(r,0,v[ 0],v[ 4],v[ 8],v[12]); \
    G(r,1,v[ 1],v[ 5],v[ 9],v[13]); \
    G(r,2,v[ 2],v[ 6],v[10],v[14]); \
    G(r,3,v[ 3],v[ 7],v[11],v[15]); \
    G(r,4,v[ 0],v[ 5],v[10],v[15]); \
    G(r,5,v[ 1],v[ 6],v[11],v[12]); \
    G(r,6,v[ 2],v[ 7],v[ 8],v[13]); \
    G(r,7,v[ 3],v[ 4],v[ 9],v[14]); \
  } అయితే(0)

స్థిర శూన్య blake2b_compress( blake2b_state *S, స్థిరరాశి uint8_t ఖండం[BLAKE2B_BLOCKBYTES] )
{
  uint64_t m[16];
  uint64_t v[16];
  size_t i;

  కొరకు( i = 0; i < 16; ++i ) {
    m[i] = load64( ఖండం + i * పరిమాణం( m[i] ) );
  }

  కొరకు( i = 0; i < 8; ++i ) {
    v[i] = S->h[i];
  }

  v[ 8] = blake2b_IV[0];
  v[ 9] = blake2b_IV[1];
  v[10] = blake2b_IV[2];
  v[11] = blake2b_IV[3];
  v[12] = blake2b_IV[4] ^ S->t[0];
  v[13] = blake2b_IV[5] ^ S->t[1];
  v[14] = blake2b_IV[6] ^ S->f[0];
  v[15] = blake2b_IV[7] ^ S->f[1];

  విడత( 0 );
  విడత( 1 );
  విడత( 2 );
  విడత( 3 );
  విడత( 4 );
  విడత( 5 );
  విడత( 6 );
  విడత( 7 );
  విడత( 8 );
  విడత( 9 );
  విడత( 10 );
  విడత( 11 );

  కొరకు( i = 0; i < 8; ++i ) {
    S->h[i] = S->h[i] ^ v[i] ^ v[i + 8];
  }
}

#నిర్వచనం_రద్దు G
#నిర్వచనం_రద్దు విడత

పూర్ణాంకం blake2b_update( blake2b_state *S, స్థిరరాశి శూన్య *pin, size_t inlen )
{
  స్థిరరాశి గుర్తులేని అక్షరం * లోపల = (స్థిరరాశి గుర్తులేని అక్షరం *)pin;
  ఒకవేళ( inlen > 0 )
  {
    size_t ఎడమ = S->buflen;
    size_t fill = BLAKE2B_BLOCKBYTES - ఎడమ;
    ఒకవేళ( inlen > fill )
    {
      S->buflen = 0;
      జ్ఞాపకం_నకలు( S->buf + ఎడమ, లోపల, fill ); /* Fill buffer */
      blake2b_increment_counter( S, BLAKE2B_BLOCKBYTES );
      blake2b_compress( S, S->buf ); /* Compress */
      లోపల += fill; inlen -= fill;
      అయితే(inlen > BLAKE2B_BLOCKBYTES) {
        blake2b_increment_counter(S, BLAKE2B_BLOCKBYTES);
        blake2b_compress( S, లోపల );
        లోపల += BLAKE2B_BLOCKBYTES;
        inlen -= BLAKE2B_BLOCKBYTES;
      }
    }
    జ్ఞాపకం_నకలు( S->buf + S->buflen, లోపల, inlen );
    S->buflen += inlen;
  }
  తిరిగి 0;
}

పూర్ణాంకం blake2b_final( blake2b_state *S, శూన్య *బహిర్గతం, size_t outlen )
{
  uint8_t బఫరు[BLAKE2B_OUTBYTES] = {0};
  size_t i;

  ఒకవేళ( బహిర్గతం == ఖాళీ || outlen < S->outlen )
    తిరిగి -1;

  ఒకవేళ( blake2b_is_lastblock( S ) )
    తిరిగి -1;

  blake2b_increment_counter( S, S->buflen );
  blake2b_సమితి_lastblock( S );
  memset( S->buf + S->buflen, 0, BLAKE2B_BLOCKBYTES - S->buflen ); /* Padding */
  blake2b_compress( S, S->buf );

  కొరకు( i = 0; i < 8; ++i ) /* Output full hash to temp buffer */
    store64( బఫరు + పరిమాణం( S->h[i] ) * i, S->h[i] );

  జ్ఞాపకం_నకలు( బహిర్గతం, బఫరు, S->outlen );
  secure_zero_జ్ఞాపకం(బఫరు, పరిమాణం(బఫరు));
  తిరిగి 0;
}

/* inlen, at least, should be uint64_t. Others can be size_t. */
పూర్ణాంకం blake2b( శూన్య *బహిర్గతం, size_t outlen, స్థిరరాశి శూన్య *లోపల, size_t inlen, స్థిరరాశి శూన్య *కీలకం, size_t keylen )
{
  blake2b_state S[1];

  /* Verify parameters */
  ఒకవేళ ( ఖాళీ == లోపల && inlen > 0 ) తిరిగి -1;

  ఒకవేళ ( ఖాళీ == బహిర్గతం ) తిరిగి -1;

  ఒకవేళ( ఖాళీ == కీలకం && keylen > 0 ) తిరిగి -1;

  ఒకవేళ( !outlen || outlen > BLAKE2B_OUTBYTES ) తిరిగి -1;

  ఒకవేళ( keylen > BLAKE2B_KEYBYTES ) తిరిగి -1;

  ఒకవేళ( keylen > 0 )
  {
    ఒకవేళ( blake2b_init_కీలకం( S, outlen, కీలకం, keylen ) < 0 ) తిరిగి -1;
  }
  లేకపోతే
  {
    ఒకవేళ( blake2b_init( S, outlen ) < 0 ) తిరిగి -1;
  }

  blake2b_update( S, ( స్థిరరాశి uint8_t * )లోపల, inlen );
  blake2b_final( S, బహిర్గతం, outlen );
  తిరిగి 0;
}

పూర్ణాంకం blake2( శూన్య *బహిర్గతం, size_t outlen, స్థిరరాశి శూన్య *లోపల, size_t inlen, స్థిరరాశి శూన్య *కీలకం, size_t keylen ) {
  తిరిగి blake2b(బహిర్గతం, outlen, లోపల, inlen, కీలకం, keylen);
}

#ఒకవేళ defined(SUPERCOP)
పూర్ణాంకం crypto_hash( గుర్తులేని అక్షరం *బహిర్గతం, గుర్తులేని అక్షరం *లోపల, గుర్తులేని దీర్ఘ దీర్ఘ inlen )
{
  తిరిగి blake2b( బహిర్గతం, BLAKE2B_OUTBYTES, లోపల, inlen, ఖాళీ, 0 );
}
#అయితే_ముగింపు

#ఒకవేళ defined(BLAKE2B_SELFTEST)
#అన్తర్భావ <string.h>
#అన్తర్భావ "blake2-kat.h"
పూర్ణాంకం ప్రధాన( శూన్య )
{
  uint8_t కీలకం[BLAKE2B_KEYBYTES];
  uint8_t buf[BLAKE2_KAT_నిడివి];
  size_t i, అడుగు;

  కొరకు( i = 0; i < BLAKE2B_KEYBYTES; ++i )
    కీలకం[i] = ( uint8_t )i;

  కొరకు( i = 0; i < BLAKE2_KAT_నిడివి; ++i )
    buf[i] = ( uint8_t )i;

  /* Test simple API */
  కొరకు( i = 0; i < BLAKE2_KAT_నిడివి; ++i )
  {
    uint8_t hash[BLAKE2B_OUTBYTES];
    blake2b( hash, BLAKE2B_OUTBYTES, buf, i, కీలకం, BLAKE2B_KEYBYTES );

    ఒకవేళ( 0 != memcmp( hash, blake2b_keyed_kat[i], BLAKE2B_OUTBYTES ) )
    {
      వెళ్ళు fail;
    }
  }

  /* Test streaming API */
  కొరకు(అడుగు = 1; అడుగు < BLAKE2B_BLOCKBYTES; ++అడుగు) {
    కొరకు (i = 0; i < BLAKE2_KAT_నిడివి; ++i) {
      uint8_t hash[BLAKE2B_OUTBYTES];
      blake2b_state S;
      uint8_t * p = buf;
      size_t mlen = i;
      పూర్ణాంకం err = 0;

      ఒకవేళ( (err = blake2b_init_కీలకం(&S, BLAKE2B_OUTBYTES, కీలకం, BLAKE2B_KEYBYTES)) < 0 ) {
        వెళ్ళు fail;
      }

      అయితే (mlen >= అడుగు) {
        ఒకవేళ ( (err = blake2b_update(&S, p, అడుగు)) < 0 ) {
          వెళ్ళు fail;
        }
        mlen -= అడుగు;
        p += అడుగు;
      }
      ఒకవేళ ( (err = blake2b_update(&S, p, mlen)) < 0) {
        వెళ్ళు fail;
      }
      ఒకవేళ ( (err = blake2b_final(&S, hash, BLAKE2B_OUTBYTES)) < 0) {
        వెళ్ళు fail;
      }

      ఒకవేళ (0 != memcmp(hash, blake2b_keyed_kat[i], BLAKE2B_OUTBYTES)) {
        వెళ్ళు fail;
      }
    }
  }

  puts( "ok" );
  తిరిగి 0;
fail:
  puts("error");
  తిరిగి -1;
}
#అయితే_ముగింపు
