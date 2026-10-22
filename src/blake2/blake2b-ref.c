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

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CONFIG_H
# include <config.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ <stdint.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "blake2.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "blake2-impl.h"

ਸਥਿਰ ਅਚਲ uint64_t blake2b_IV[8] =
{
  0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL,
  0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL,
  0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL,
  0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL
};

ਸਥਿਰ ਅਚਲ uint8_t blake2b_sigma[12][16] =
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


ਸਥਿਰ ਖਾਲੀ blake2b_ਸੈੱਟ_lastnode( blake2b_state *S )
{
  S->f[1] = (uint64_t)-1;
}

/* Some helper functions, not necessarily useful */
ਸਥਿਰ ਪੂਰਨ blake2b_is_lastblock( ਅਚਲ blake2b_state *S )
{
  ਵਾਪਸ S->f[0] != 0;
}

ਸਥਿਰ ਖਾਲੀ blake2b_ਸੈੱਟ_lastblock( blake2b_state *S )
{
  ਜੇ( S->ਆਖਰੀ_ਗੰਢ ) blake2b_ਸੈੱਟ_lastnode( S );

  S->f[0] = (uint64_t)-1;
}

ਸਥਿਰ ਖਾਲੀ blake2b_increment_counter( blake2b_state *S, ਅਚਲ uint64_t inc )
{
  S->t[0] += inc;
  S->t[1] += ( S->t[0] < inc );
}

ਸਥਿਰ ਖਾਲੀ blake2b_init0( blake2b_state *S )
{
  size_t i;
  memset( S, 0, ਆਕਾਰ( blake2b_state ) );

  ਲਈ( i = 0; i < 8; ++i ) S->h[i] = blake2b_IV[i];
}

/* init xors IV with input parameter block */
ਪੂਰਨ blake2b_init_param( blake2b_state *S, ਅਚਲ blake2b_param *P )
{
  ਅਚਲ uint8_t *p = ( ਅਚਲ uint8_t * )( P );
  size_t i;

  blake2b_init0( S );

  /* IV XOR ParamBlock */
  ਲਈ( i = 0; i < 8; ++i )
    S->h[i] ^= load64( p + ਆਕਾਰ( S->h[i] ) * i );

  S->outlen = P->digest_ਲੰਮਾਈ;
  ਵਾਪਸ 0;
}



ਪੂਰਨ blake2b_init( blake2b_state *S, size_t outlen )
{
  blake2b_param P[1];

  ਜੇ ( ( !outlen ) || ( outlen > BLAKE2B_OUTBYTES ) ) ਵਾਪਸ -1;

  P->digest_ਲੰਮਾਈ = (uint8_t)outlen;
  P->ਕੁੰਜੀ_ਲੰਮਾਈ    = 0;
  P->fanout        = 1;
  P->ਡੂੰਘਾਈ         = 1;
  store32( &P->ਪੱਤੀ_ਲੰਮਾਈ, 0 );
  store32( &P->ਗੰਢ_ਵਿੱਥ, 0 );
  store32( &P->xof_ਲੰਮਾਈ, 0 );
  P->ਗੰਢ_ਡੂੰਘਾਈ    = 0;
  P->inner_ਲੰਮਾਈ  = 0;
  memset( P->reserved, 0, ਆਕਾਰ( P->reserved ) );
  memset( P->salt,     0, ਆਕਾਰ( P->salt ) );
  memset( P->personal, 0, ਆਕਾਰ( P->personal ) );
  ਵਾਪਸ blake2b_init_param( S, P );
}


ਪੂਰਨ blake2b_init_ਕੁੰਜੀ( blake2b_state *S, size_t outlen, ਅਚਲ ਖਾਲੀ *ਕੁੰਜੀ, size_t keylen )
{
  blake2b_param P[1];

  ਜੇ ( ( !outlen ) || ( outlen > BLAKE2B_OUTBYTES ) ) ਵਾਪਸ -1;

  ਜੇ ( !ਕੁੰਜੀ || !keylen || keylen > BLAKE2B_KEYBYTES ) ਵਾਪਸ -1;

  P->digest_ਲੰਮਾਈ = (uint8_t)outlen;
  P->ਕੁੰਜੀ_ਲੰਮਾਈ    = (uint8_t)keylen;
  P->fanout        = 1;
  P->ਡੂੰਘਾਈ         = 1;
  store32( &P->ਪੱਤੀ_ਲੰਮਾਈ, 0 );
  store32( &P->ਗੰਢ_ਵਿੱਥ, 0 );
  store32( &P->xof_ਲੰਮਾਈ, 0 );
  P->ਗੰਢ_ਡੂੰਘਾਈ    = 0;
  P->inner_ਲੰਮਾਈ  = 0;
  memset( P->reserved, 0, ਆਕਾਰ( P->reserved ) );
  memset( P->salt,     0, ਆਕਾਰ( P->salt ) );
  memset( P->personal, 0, ਆਕਾਰ( P->personal ) );

  ਜੇ( blake2b_init_param( S, P ) < 0 ) ਵਾਪਸ -1;

  {
    uint8_t ਖੰਡ[BLAKE2B_BLOCKBYTES];
    memset( ਖੰਡ, 0, BLAKE2B_BLOCKBYTES );
    ਸਮ੍ਰਿਤੀ_ਨਕਲ( ਖੰਡ, ਕੁੰਜੀ, keylen );
    blake2b_update( S, ਖੰਡ, BLAKE2B_BLOCKBYTES );
    secure_zero_ਸਮ੍ਰਿਤੀ( ਖੰਡ, BLAKE2B_BLOCKBYTES ); /* Burn the key from stack */
  }
  ਵਾਪਸ 0;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ G(r,i,a,b,c,d)                      \
  ਕਰੋ {                                      \
    a = a + b + m[blake2b_sigma[r][2 * i + 0]]; \
    d = rotr64(d ^ a, 32);                  \
    c = c + d;                              \
    b = rotr64(b ^ c, 24);                  \
    a = a + b + m[blake2b_sigma[r][2 * i + 1]]; \
    d = rotr64(d ^ a, 16);                  \
    c = c + d;                              \
    b = rotr64(b ^ c, 63);                  \
  } ਜਦੋਂ(0)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗੇੜਾ(r)                    \
  ਕਰੋ {                              \
    G(r,0,v[ 0],v[ 4],v[ 8],v[12]); \
    G(r,1,v[ 1],v[ 5],v[ 9],v[13]); \
    G(r,2,v[ 2],v[ 6],v[10],v[14]); \
    G(r,3,v[ 3],v[ 7],v[11],v[15]); \
    G(r,4,v[ 0],v[ 5],v[10],v[15]); \
    G(r,5,v[ 1],v[ 6],v[11],v[12]); \
    G(r,6,v[ 2],v[ 7],v[ 8],v[13]); \
    G(r,7,v[ 3],v[ 4],v[ 9],v[14]); \
  } ਜਦੋਂ(0)

ਸਥਿਰ ਖਾਲੀ blake2b_compress( blake2b_state *S, ਅਚਲ uint8_t ਖੰਡ[BLAKE2B_BLOCKBYTES] )
{
  uint64_t m[16];
  uint64_t v[16];
  size_t i;

  ਲਈ( i = 0; i < 16; ++i ) {
    m[i] = load64( ਖੰਡ + i * ਆਕਾਰ( m[i] ) );
  }

  ਲਈ( i = 0; i < 8; ++i ) {
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

  ਗੇੜਾ( 0 );
  ਗੇੜਾ( 1 );
  ਗੇੜਾ( 2 );
  ਗੇੜਾ( 3 );
  ਗੇੜਾ( 4 );
  ਗੇੜਾ( 5 );
  ਗੇੜਾ( 6 );
  ਗੇੜਾ( 7 );
  ਗੇੜਾ( 8 );
  ਗੇੜਾ( 9 );
  ਗੇੜਾ( 10 );
  ਗੇੜਾ( 11 );

  ਲਈ( i = 0; i < 8; ++i ) {
    S->h[i] = S->h[i] ^ v[i] ^ v[i + 8];
  }
}

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ G
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ ਗੇੜਾ

ਪੂਰਨ blake2b_update( blake2b_state *S, ਅਚਲ ਖਾਲੀ *pin, size_t inlen )
{
  ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ * ਵਿੱਚ = (ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)pin;
  ਜੇ( inlen > 0 )
  {
    size_t ਖੱਬਾ = S->buflen;
    size_t fill = BLAKE2B_BLOCKBYTES - ਖੱਬਾ;
    ਜੇ( inlen > fill )
    {
      S->buflen = 0;
      ਸਮ੍ਰਿਤੀ_ਨਕਲ( S->buf + ਖੱਬਾ, ਵਿੱਚ, fill ); /* Fill buffer */
      blake2b_increment_counter( S, BLAKE2B_BLOCKBYTES );
      blake2b_compress( S, S->buf ); /* Compress */
      ਵਿੱਚ += fill; inlen -= fill;
      ਜਦੋਂ(inlen > BLAKE2B_BLOCKBYTES) {
        blake2b_increment_counter(S, BLAKE2B_BLOCKBYTES);
        blake2b_compress( S, ਵਿੱਚ );
        ਵਿੱਚ += BLAKE2B_BLOCKBYTES;
        inlen -= BLAKE2B_BLOCKBYTES;
      }
    }
    ਸਮ੍ਰਿਤੀ_ਨਕਲ( S->buf + S->buflen, ਵਿੱਚ, inlen );
    S->buflen += inlen;
  }
  ਵਾਪਸ 0;
}

ਪੂਰਨ blake2b_final( blake2b_state *S, ਖਾਲੀ *ਬਾਹਰਲਾ, size_t outlen )
{
  uint8_t ਬਫਰ[BLAKE2B_OUTBYTES] = {0};
  size_t i;

  ਜੇ( ਬਾਹਰਲਾ == ਨਿਰਾ || outlen < S->outlen )
    ਵਾਪਸ -1;

  ਜੇ( blake2b_is_lastblock( S ) )
    ਵਾਪਸ -1;

  blake2b_increment_counter( S, S->buflen );
  blake2b_ਸੈੱਟ_lastblock( S );
  memset( S->buf + S->buflen, 0, BLAKE2B_BLOCKBYTES - S->buflen ); /* Padding */
  blake2b_compress( S, S->buf );

  ਲਈ( i = 0; i < 8; ++i ) /* Output full hash to temp buffer */
    store64( ਬਫਰ + ਆਕਾਰ( S->h[i] ) * i, S->h[i] );

  ਸਮ੍ਰਿਤੀ_ਨਕਲ( ਬਾਹਰਲਾ, ਬਫਰ, S->outlen );
  secure_zero_ਸਮ੍ਰਿਤੀ(ਬਫਰ, ਆਕਾਰ(ਬਫਰ));
  ਵਾਪਸ 0;
}

/* inlen, at least, should be uint64_t. Others can be size_t. */
ਪੂਰਨ blake2b( ਖਾਲੀ *ਬਾਹਰਲਾ, size_t outlen, ਅਚਲ ਖਾਲੀ *ਵਿੱਚ, size_t inlen, ਅਚਲ ਖਾਲੀ *ਕੁੰਜੀ, size_t keylen )
{
  blake2b_state S[1];

  /* Verify parameters */
  ਜੇ ( ਨਿਰਾ == ਵਿੱਚ && inlen > 0 ) ਵਾਪਸ -1;

  ਜੇ ( ਨਿਰਾ == ਬਾਹਰਲਾ ) ਵਾਪਸ -1;

  ਜੇ( ਨਿਰਾ == ਕੁੰਜੀ && keylen > 0 ) ਵਾਪਸ -1;

  ਜੇ( !outlen || outlen > BLAKE2B_OUTBYTES ) ਵਾਪਸ -1;

  ਜੇ( keylen > BLAKE2B_KEYBYTES ) ਵਾਪਸ -1;

  ਜੇ( keylen > 0 )
  {
    ਜੇ( blake2b_init_ਕੁੰਜੀ( S, outlen, ਕੁੰਜੀ, keylen ) < 0 ) ਵਾਪਸ -1;
  }
  ਨਹੀਂਤਾਂ
  {
    ਜੇ( blake2b_init( S, outlen ) < 0 ) ਵਾਪਸ -1;
  }

  blake2b_update( S, ( ਅਚਲ uint8_t * )ਵਿੱਚ, inlen );
  blake2b_final( S, ਬਾਹਰਲਾ, outlen );
  ਵਾਪਸ 0;
}

ਪੂਰਨ blake2( ਖਾਲੀ *ਬਾਹਰਲਾ, size_t outlen, ਅਚਲ ਖਾਲੀ *ਵਿੱਚ, size_t inlen, ਅਚਲ ਖਾਲੀ *ਕੁੰਜੀ, size_t keylen ) {
  ਵਾਪਸ blake2b(ਬਾਹਰਲਾ, outlen, ਵਿੱਚ, inlen, ਕੁੰਜੀ, keylen);
}

#ਜੇ defined(SUPERCOP)
ਪੂਰਨ crypto_hash( ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *ਬਾਹਰਲਾ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *ਵਿੱਚ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ inlen )
{
  ਵਾਪਸ blake2b( ਬਾਹਰਲਾ, BLAKE2B_OUTBYTES, ਵਿੱਚ, inlen, ਨਿਰਾ, 0 );
}
#ਜੇ_ਅੰਤ

#ਜੇ defined(BLAKE2B_SELFTEST)
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "blake2-kat.h"
ਪੂਰਨ ਮੁੱਖ( ਖਾਲੀ )
{
  uint8_t ਕੁੰਜੀ[BLAKE2B_KEYBYTES];
  uint8_t buf[BLAKE2_KAT_ਲੰਮਾਈ];
  size_t i, ਕਦਮ;

  ਲਈ( i = 0; i < BLAKE2B_KEYBYTES; ++i )
    ਕੁੰਜੀ[i] = ( uint8_t )i;

  ਲਈ( i = 0; i < BLAKE2_KAT_ਲੰਮਾਈ; ++i )
    buf[i] = ( uint8_t )i;

  /* Test simple API */
  ਲਈ( i = 0; i < BLAKE2_KAT_ਲੰਮਾਈ; ++i )
  {
    uint8_t hash[BLAKE2B_OUTBYTES];
    blake2b( hash, BLAKE2B_OUTBYTES, buf, i, ਕੁੰਜੀ, BLAKE2B_KEYBYTES );

    ਜੇ( 0 != memcmp( hash, blake2b_keyed_kat[i], BLAKE2B_OUTBYTES ) )
    {
      ਜਾਓ fail;
    }
  }

  /* Test streaming API */
  ਲਈ(ਕਦਮ = 1; ਕਦਮ < BLAKE2B_BLOCKBYTES; ++ਕਦਮ) {
    ਲਈ (i = 0; i < BLAKE2_KAT_ਲੰਮਾਈ; ++i) {
      uint8_t hash[BLAKE2B_OUTBYTES];
      blake2b_state S;
      uint8_t * p = buf;
      size_t mlen = i;
      ਪੂਰਨ err = 0;

      ਜੇ( (err = blake2b_init_ਕੁੰਜੀ(&S, BLAKE2B_OUTBYTES, ਕੁੰਜੀ, BLAKE2B_KEYBYTES)) < 0 ) {
        ਜਾਓ fail;
      }

      ਜਦੋਂ (mlen >= ਕਦਮ) {
        ਜੇ ( (err = blake2b_update(&S, p, ਕਦਮ)) < 0 ) {
          ਜਾਓ fail;
        }
        mlen -= ਕਦਮ;
        p += ਕਦਮ;
      }
      ਜੇ ( (err = blake2b_update(&S, p, mlen)) < 0) {
        ਜਾਓ fail;
      }
      ਜੇ ( (err = blake2b_final(&S, hash, BLAKE2B_OUTBYTES)) < 0) {
        ਜਾਓ fail;
      }

      ਜੇ (0 != memcmp(hash, blake2b_keyed_kat[i], BLAKE2B_OUTBYTES)) {
        ਜਾਓ fail;
      }
    }
  }

  puts( "ok" );
  ਵਾਪਸ 0;
fail:
  puts("error");
  ਵਾਪਸ -1;
}
#ਜੇ_ਅੰਤ
