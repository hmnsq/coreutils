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

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CONFIG_H
# include <config.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ <stdint.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>

#ಅನ್ತರ್ಭಾವ "blake2.h"
#ಅನ್ತರ್ಭಾವ "blake2-impl.h"

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ uint64_t blake2b_IV[8] =
{
  0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL,
  0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL,
  0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL,
  0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ uint8_t blake2b_sigma[12][16] =
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


ಸ್ಥಿರ ಶೂನ್ಯ blake2b_ಗಣ_lastnode( blake2b_state *S )
{
  S->f[1] = (uint64_t)-1;
}

/* Some helper functions, not necessarily useful */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ blake2b_is_lastblock( ಸ್ಥಿರಾಂಕ blake2b_state *S )
{
  ಹಿಂತಿರುಗಿ S->f[0] != 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ blake2b_ಗಣ_lastblock( blake2b_state *S )
{
  ಆದರೆ( S->ಕೊನೆಯ_ಗಂಟು ) blake2b_ಗಣ_lastnode( S );

  S->f[0] = (uint64_t)-1;
}

ಸ್ಥಿರ ಶೂನ್ಯ blake2b_increment_counter( blake2b_state *S, ಸ್ಥಿರಾಂಕ uint64_t inc )
{
  S->t[0] += inc;
  S->t[1] += ( S->t[0] < inc );
}

ಸ್ಥಿರ ಶೂನ್ಯ blake2b_init0( blake2b_state *S )
{
  size_t i;
  memset( S, 0, ಗಾತ್ರ( blake2b_state ) );

  ಗಾಗಿ( i = 0; i < 8; ++i ) S->h[i] = blake2b_IV[i];
}

/* init xors IV with input parameter block */
ಪೂರ್ಣಾಂಕ blake2b_init_param( blake2b_state *S, ಸ್ಥಿರಾಂಕ blake2b_param *P )
{
  ಸ್ಥಿರಾಂಕ uint8_t *p = ( ಸ್ಥಿರಾಂಕ uint8_t * )( P );
  size_t i;

  blake2b_init0( S );

  /* IV XOR ParamBlock */
  ಗಾಗಿ( i = 0; i < 8; ++i )
    S->h[i] ^= load64( p + ಗಾತ್ರ( S->h[i] ) * i );

  S->outlen = P->digest_ಉದ್ದಳತೆ;
  ಹಿಂತಿರುಗಿ 0;
}



ಪೂರ್ಣಾಂಕ blake2b_init( blake2b_state *S, size_t outlen )
{
  blake2b_param P[1];

  ಆದರೆ ( ( !outlen ) || ( outlen > BLAKE2B_OUTBYTES ) ) ಹಿಂತಿರುಗಿ -1;

  P->digest_ಉದ್ದಳತೆ = (uint8_t)outlen;
  P->ಕೀಲಿ_ಉದ್ದಳತೆ    = 0;
  P->fanout        = 1;
  P->ಆಳ         = 1;
  store32( &P->ಎಲೆ_ಉದ್ದಳತೆ, 0 );
  store32( &P->ಗಂಟು_ಸ್ಥಾನಾಂತರ, 0 );
  store32( &P->xof_ಉದ್ದಳತೆ, 0 );
  P->ಗಂಟು_ಆಳ    = 0;
  P->inner_ಉದ್ದಳತೆ  = 0;
  memset( P->reserved, 0, ಗಾತ್ರ( P->reserved ) );
  memset( P->salt,     0, ಗಾತ್ರ( P->salt ) );
  memset( P->personal, 0, ಗಾತ್ರ( P->personal ) );
  ಹಿಂತಿರುಗಿ blake2b_init_param( S, P );
}


ಪೂರ್ಣಾಂಕ blake2b_init_ಕೀಲಿ( blake2b_state *S, size_t outlen, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಕೀಲಿ, size_t keylen )
{
  blake2b_param P[1];

  ಆದರೆ ( ( !outlen ) || ( outlen > BLAKE2B_OUTBYTES ) ) ಹಿಂತಿರುಗಿ -1;

  ಆದರೆ ( !ಕೀಲಿ || !keylen || keylen > BLAKE2B_KEYBYTES ) ಹಿಂತಿರುಗಿ -1;

  P->digest_ಉದ್ದಳತೆ = (uint8_t)outlen;
  P->ಕೀಲಿ_ಉದ್ದಳತೆ    = (uint8_t)keylen;
  P->fanout        = 1;
  P->ಆಳ         = 1;
  store32( &P->ಎಲೆ_ಉದ್ದಳತೆ, 0 );
  store32( &P->ಗಂಟು_ಸ್ಥಾನಾಂತರ, 0 );
  store32( &P->xof_ಉದ್ದಳತೆ, 0 );
  P->ಗಂಟು_ಆಳ    = 0;
  P->inner_ಉದ್ದಳತೆ  = 0;
  memset( P->reserved, 0, ಗಾತ್ರ( P->reserved ) );
  memset( P->salt,     0, ಗಾತ್ರ( P->salt ) );
  memset( P->personal, 0, ಗಾತ್ರ( P->personal ) );

  ಆದರೆ( blake2b_init_param( S, P ) < 0 ) ಹಿಂತಿರುಗಿ -1;

  {
    uint8_t ಖಂಡ[BLAKE2B_BLOCKBYTES];
    memset( ಖಂಡ, 0, BLAKE2B_BLOCKBYTES );
    ಸ್ಮರಣೆ_ನಕಲು( ಖಂಡ, ಕೀಲಿ, keylen );
    blake2b_update( S, ಖಂಡ, BLAKE2B_BLOCKBYTES );
    secure_zero_ಸ್ಮರಣೆ( ಖಂಡ, BLAKE2B_BLOCKBYTES ); /* Burn the key from stack */
  }
  ಹಿಂತಿರುಗಿ 0;
}

#ವ್ಯಾಖ್ಯಾನಿಸು G(r,i,a,b,c,d)                      \
  ಮಾಡು {                                      \
    a = a + b + m[blake2b_sigma[r][2 * i + 0]]; \
    d = rotr64(d ^ a, 32);                  \
    c = c + d;                              \
    b = rotr64(b ^ c, 24);                  \
    a = a + b + m[blake2b_sigma[r][2 * i + 1]]; \
    d = rotr64(d ^ a, 16);                  \
    c = c + d;                              \
    b = rotr64(b ^ c, 63);                  \
  } ಆಗ(0)

#ವ್ಯಾಖ್ಯಾನಿಸು ಸುತ್ತು(r)                    \
  ಮಾಡು {                              \
    G(r,0,v[ 0],v[ 4],v[ 8],v[12]); \
    G(r,1,v[ 1],v[ 5],v[ 9],v[13]); \
    G(r,2,v[ 2],v[ 6],v[10],v[14]); \
    G(r,3,v[ 3],v[ 7],v[11],v[15]); \
    G(r,4,v[ 0],v[ 5],v[10],v[15]); \
    G(r,5,v[ 1],v[ 6],v[11],v[12]); \
    G(r,6,v[ 2],v[ 7],v[ 8],v[13]); \
    G(r,7,v[ 3],v[ 4],v[ 9],v[14]); \
  } ಆಗ(0)

ಸ್ಥಿರ ಶೂನ್ಯ blake2b_compress( blake2b_state *S, ಸ್ಥಿರಾಂಕ uint8_t ಖಂಡ[BLAKE2B_BLOCKBYTES] )
{
  uint64_t m[16];
  uint64_t v[16];
  size_t i;

  ಗಾಗಿ( i = 0; i < 16; ++i ) {
    m[i] = load64( ಖಂಡ + i * ಗಾತ್ರ( m[i] ) );
  }

  ಗಾಗಿ( i = 0; i < 8; ++i ) {
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

  ಸುತ್ತು( 0 );
  ಸುತ್ತು( 1 );
  ಸುತ್ತು( 2 );
  ಸುತ್ತು( 3 );
  ಸುತ್ತು( 4 );
  ಸುತ್ತು( 5 );
  ಸುತ್ತು( 6 );
  ಸುತ್ತು( 7 );
  ಸುತ್ತು( 8 );
  ಸುತ್ತು( 9 );
  ಸುತ್ತು( 10 );
  ಸುತ್ತು( 11 );

  ಗಾಗಿ( i = 0; i < 8; ++i ) {
    S->h[i] = S->h[i] ^ v[i] ^ v[i + 8];
  }
}

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು G
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು ಸುತ್ತು

ಪೂರ್ಣಾಂಕ blake2b_update( blake2b_state *S, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *pin, size_t inlen )
{
  ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ * ಒಳ = (ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)pin;
  ಆದರೆ( inlen > 0 )
  {
    size_t ಎಡ = S->buflen;
    size_t fill = BLAKE2B_BLOCKBYTES - ಎಡ;
    ಆದರೆ( inlen > fill )
    {
      S->buflen = 0;
      ಸ್ಮರಣೆ_ನಕಲು( S->buf + ಎಡ, ಒಳ, fill ); /* Fill buffer */
      blake2b_increment_counter( S, BLAKE2B_BLOCKBYTES );
      blake2b_compress( S, S->buf ); /* Compress */
      ಒಳ += fill; inlen -= fill;
      ಆಗ(inlen > BLAKE2B_BLOCKBYTES) {
        blake2b_increment_counter(S, BLAKE2B_BLOCKBYTES);
        blake2b_compress( S, ಒಳ );
        ಒಳ += BLAKE2B_BLOCKBYTES;
        inlen -= BLAKE2B_BLOCKBYTES;
      }
    }
    ಸ್ಮರಣೆ_ನಕಲು( S->buf + S->buflen, ಒಳ, inlen );
    S->buflen += inlen;
  }
  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ blake2b_final( blake2b_state *S, ಶೂನ್ಯ *ಹೊರಹೋಗು, size_t outlen )
{
  uint8_t ಬಫರ್[BLAKE2B_OUTBYTES] = {0};
  size_t i;

  ಆದರೆ( ಹೊರಹೋಗು == ಖಾಲಿ || outlen < S->outlen )
    ಹಿಂತಿರುಗಿ -1;

  ಆದರೆ( blake2b_is_lastblock( S ) )
    ಹಿಂತಿರುಗಿ -1;

  blake2b_increment_counter( S, S->buflen );
  blake2b_ಗಣ_lastblock( S );
  memset( S->buf + S->buflen, 0, BLAKE2B_BLOCKBYTES - S->buflen ); /* Padding */
  blake2b_compress( S, S->buf );

  ಗಾಗಿ( i = 0; i < 8; ++i ) /* Output full hash to temp buffer */
    store64( ಬಫರ್ + ಗಾತ್ರ( S->h[i] ) * i, S->h[i] );

  ಸ್ಮರಣೆ_ನಕಲು( ಹೊರಹೋಗು, ಬಫರ್, S->outlen );
  secure_zero_ಸ್ಮರಣೆ(ಬಫರ್, ಗಾತ್ರ(ಬಫರ್));
  ಹಿಂತಿರುಗಿ 0;
}

/* inlen, at least, should be uint64_t. Others can be size_t. */
ಪೂರ್ಣಾಂಕ blake2b( ಶೂನ್ಯ *ಹೊರಹೋಗು, size_t outlen, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಒಳ, size_t inlen, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಕೀಲಿ, size_t keylen )
{
  blake2b_state S[1];

  /* Verify parameters */
  ಆದರೆ ( ಖಾಲಿ == ಒಳ && inlen > 0 ) ಹಿಂತಿರುಗಿ -1;

  ಆದರೆ ( ಖಾಲಿ == ಹೊರಹೋಗು ) ಹಿಂತಿರುಗಿ -1;

  ಆದರೆ( ಖಾಲಿ == ಕೀಲಿ && keylen > 0 ) ಹಿಂತಿರುಗಿ -1;

  ಆದರೆ( !outlen || outlen > BLAKE2B_OUTBYTES ) ಹಿಂತಿರುಗಿ -1;

  ಆದರೆ( keylen > BLAKE2B_KEYBYTES ) ಹಿಂತಿರುಗಿ -1;

  ಆದರೆ( keylen > 0 )
  {
    ಆದರೆ( blake2b_init_ಕೀಲಿ( S, outlen, ಕೀಲಿ, keylen ) < 0 ) ಹಿಂತಿರುಗಿ -1;
  }
  ಇಲ್ಲದಿದ್ದರೆ
  {
    ಆದರೆ( blake2b_init( S, outlen ) < 0 ) ಹಿಂತಿರುಗಿ -1;
  }

  blake2b_update( S, ( ಸ್ಥಿರಾಂಕ uint8_t * )ಒಳ, inlen );
  blake2b_final( S, ಹೊರಹೋಗು, outlen );
  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ blake2( ಶೂನ್ಯ *ಹೊರಹೋಗು, size_t outlen, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಒಳ, size_t inlen, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಕೀಲಿ, size_t keylen ) {
  ಹಿಂತಿರುಗಿ blake2b(ಹೊರಹೋಗು, outlen, ಒಳ, inlen, ಕೀಲಿ, keylen);
}

#ಆದರೆ defined(SUPERCOP)
ಪೂರ್ಣಾಂಕ crypto_hash( ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *ಹೊರಹೋಗು, ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *ಒಳ, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ inlen )
{
  ಹಿಂತಿರುಗಿ blake2b( ಹೊರಹೋಗು, BLAKE2B_OUTBYTES, ಒಳ, inlen, ಖಾಲಿ, 0 );
}
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined(BLAKE2B_SELFTEST)
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ "blake2-kat.h"
ಪೂರ್ಣಾಂಕ ಮುಖ್ಯ( ಶೂನ್ಯ )
{
  uint8_t ಕೀಲಿ[BLAKE2B_KEYBYTES];
  uint8_t buf[BLAKE2_KAT_ಉದ್ದಳತೆ];
  size_t i, ಹೆಜ್ಜೆ;

  ಗಾಗಿ( i = 0; i < BLAKE2B_KEYBYTES; ++i )
    ಕೀಲಿ[i] = ( uint8_t )i;

  ಗಾಗಿ( i = 0; i < BLAKE2_KAT_ಉದ್ದಳತೆ; ++i )
    buf[i] = ( uint8_t )i;

  /* Test simple API */
  ಗಾಗಿ( i = 0; i < BLAKE2_KAT_ಉದ್ದಳತೆ; ++i )
  {
    uint8_t hash[BLAKE2B_OUTBYTES];
    blake2b( hash, BLAKE2B_OUTBYTES, buf, i, ಕೀಲಿ, BLAKE2B_KEYBYTES );

    ಆದರೆ( 0 != memcmp( hash, blake2b_keyed_kat[i], BLAKE2B_OUTBYTES ) )
    {
      ಹೋಗು fail;
    }
  }

  /* Test streaming API */
  ಗಾಗಿ(ಹೆಜ್ಜೆ = 1; ಹೆಜ್ಜೆ < BLAKE2B_BLOCKBYTES; ++ಹೆಜ್ಜೆ) {
    ಗಾಗಿ (i = 0; i < BLAKE2_KAT_ಉದ್ದಳತೆ; ++i) {
      uint8_t hash[BLAKE2B_OUTBYTES];
      blake2b_state S;
      uint8_t * p = buf;
      size_t mlen = i;
      ಪೂರ್ಣಾಂಕ err = 0;

      ಆದರೆ( (err = blake2b_init_ಕೀಲಿ(&S, BLAKE2B_OUTBYTES, ಕೀಲಿ, BLAKE2B_KEYBYTES)) < 0 ) {
        ಹೋಗು fail;
      }

      ಆಗ (mlen >= ಹೆಜ್ಜೆ) {
        ಆದರೆ ( (err = blake2b_update(&S, p, ಹೆಜ್ಜೆ)) < 0 ) {
          ಹೋಗು fail;
        }
        mlen -= ಹೆಜ್ಜೆ;
        p += ಹೆಜ್ಜೆ;
      }
      ಆದರೆ ( (err = blake2b_update(&S, p, mlen)) < 0) {
        ಹೋಗು fail;
      }
      ಆದರೆ ( (err = blake2b_final(&S, hash, BLAKE2B_OUTBYTES)) < 0) {
        ಹೋಗು fail;
      }

      ಆದರೆ (0 != memcmp(hash, blake2b_keyed_kat[i], BLAKE2B_OUTBYTES)) {
        ಹೋಗು fail;
      }
    }
  }

  puts( "ok" );
  ಹಿಂತಿರುಗಿ 0;
fail:
  puts("error");
  ಹಿಂತಿರುಗಿ -1;
}
#ಆದರೆ_ಅಂತ್ಯ
