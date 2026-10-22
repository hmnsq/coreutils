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

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#அந்தர்பாவ <stdint.h>
#அந்தர்பாவ <string.h>
#அந்தர்பாவ <stdio.h>

#அந்தர்பாவ "blake2.h"
#அந்தர்பாவ "blake2-impl.h"

நிலையான மாறிலி uint64_t blake2b_IV[8] =
{
  0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL,
  0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL,
  0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL,
  0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL
};

நிலையான மாறிலி uint8_t blake2b_sigma[12][16] =
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


நிலையான வெறுமை blake2b_set_lastnode( blake2b_state *S )
{
  S->f[1] = (uint64_t)-1;
}

/* Some helper functions, not necessarily useful */
நிலையான முழு blake2b_is_lastblock( மாறிலி blake2b_state *S )
{
  திரும்பு S->f[0] != 0;
}

நிலையான வெறுமை blake2b_set_lastblock( blake2b_state *S )
{
  எனில்( S->last_முனை ) blake2b_set_lastnode( S );

  S->f[0] = (uint64_t)-1;
}

நிலையான வெறுமை blake2b_increment_counter( blake2b_state *S, மாறிலி uint64_t inc )
{
  S->t[0] += inc;
  S->t[1] += ( S->t[0] < inc );
}

நிலையான வெறுமை blake2b_init0( blake2b_state *S )
{
  size_t i;
  memset( S, 0, அளவு( blake2b_state ) );

  க்கு( i = 0; i < 8; ++i ) S->h[i] = blake2b_IV[i];
}

/* init xors IV with input parameter block */
முழு blake2b_init_param( blake2b_state *S, மாறிலி blake2b_param *P )
{
  மாறிலி uint8_t *p = ( மாறிலி uint8_t * )( P );
  size_t i;

  blake2b_init0( S );

  /* IV XOR ParamBlock */
  க்கு( i = 0; i < 8; ++i )
    S->h[i] ^= load64( p + அளவு( S->h[i] ) * i );

  S->outlen = P->digest_length;
  திரும்பு 0;
}



முழு blake2b_init( blake2b_state *S, size_t outlen )
{
  blake2b_param P[1];

  எனில் ( ( !outlen ) || ( outlen > BLAKE2B_OUTBYTES ) ) திரும்பு -1;

  P->digest_length = (uint8_t)outlen;
  P->சாவி_length    = 0;
  P->fanout        = 1;
  P->depth         = 1;
  store32( &P->leaf_length, 0 );
  store32( &P->முனை_offset, 0 );
  store32( &P->xof_length, 0 );
  P->முனை_depth    = 0;
  P->inner_length  = 0;
  memset( P->reserved, 0, அளவு( P->reserved ) );
  memset( P->salt,     0, அளவு( P->salt ) );
  memset( P->personal, 0, அளவு( P->personal ) );
  திரும்பு blake2b_init_param( S, P );
}


முழு blake2b_init_சாவி( blake2b_state *S, size_t outlen, மாறிலி வெறுமை *சாவி, size_t keylen )
{
  blake2b_param P[1];

  எனில் ( ( !outlen ) || ( outlen > BLAKE2B_OUTBYTES ) ) திரும்பு -1;

  எனில் ( !சாவி || !keylen || keylen > BLAKE2B_KEYBYTES ) திரும்பு -1;

  P->digest_length = (uint8_t)outlen;
  P->சாவி_length    = (uint8_t)keylen;
  P->fanout        = 1;
  P->depth         = 1;
  store32( &P->leaf_length, 0 );
  store32( &P->முனை_offset, 0 );
  store32( &P->xof_length, 0 );
  P->முனை_depth    = 0;
  P->inner_length  = 0;
  memset( P->reserved, 0, அளவு( P->reserved ) );
  memset( P->salt,     0, அளவு( P->salt ) );
  memset( P->personal, 0, அளவு( P->personal ) );

  எனில்( blake2b_init_param( S, P ) < 0 ) திரும்பு -1;

  {
    uint8_t block[BLAKE2B_BLOCKBYTES];
    memset( block, 0, BLAKE2B_BLOCKBYTES );
    memcpy( block, சாவி, keylen );
    blake2b_update( S, block, BLAKE2B_BLOCKBYTES );
    secure_zero_memory( block, BLAKE2B_BLOCKBYTES ); /* Burn the key from stack */
  }
  திரும்பு 0;
}

#define G(r,i,a,b,c,d)                      \
  செய் {                                      \
    a = a + b + m[blake2b_sigma[r][2 * i + 0]]; \
    d = rotr64(d ^ a, 32);                  \
    c = c + d;                              \
    b = rotr64(b ^ c, 24);                  \
    a = a + b + m[blake2b_sigma[r][2 * i + 1]]; \
    d = rotr64(d ^ a, 16);                  \
    c = c + d;                              \
    b = rotr64(b ^ c, 63);                  \
  } போது(0)

#define ROUND(r)                    \
  செய் {                              \
    G(r,0,v[ 0],v[ 4],v[ 8],v[12]); \
    G(r,1,v[ 1],v[ 5],v[ 9],v[13]); \
    G(r,2,v[ 2],v[ 6],v[10],v[14]); \
    G(r,3,v[ 3],v[ 7],v[11],v[15]); \
    G(r,4,v[ 0],v[ 5],v[10],v[15]); \
    G(r,5,v[ 1],v[ 6],v[11],v[12]); \
    G(r,6,v[ 2],v[ 7],v[ 8],v[13]); \
    G(r,7,v[ 3],v[ 4],v[ 9],v[14]); \
  } போது(0)

நிலையான வெறுமை blake2b_compress( blake2b_state *S, மாறிலி uint8_t block[BLAKE2B_BLOCKBYTES] )
{
  uint64_t m[16];
  uint64_t v[16];
  size_t i;

  க்கு( i = 0; i < 16; ++i ) {
    m[i] = load64( block + i * அளவு( m[i] ) );
  }

  க்கு( i = 0; i < 8; ++i ) {
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

  ROUND( 0 );
  ROUND( 1 );
  ROUND( 2 );
  ROUND( 3 );
  ROUND( 4 );
  ROUND( 5 );
  ROUND( 6 );
  ROUND( 7 );
  ROUND( 8 );
  ROUND( 9 );
  ROUND( 10 );
  ROUND( 11 );

  க்கு( i = 0; i < 8; ++i ) {
    S->h[i] = S->h[i] ^ v[i] ^ v[i + 8];
  }
}

#undef G
#undef ROUND

முழு blake2b_update( blake2b_state *S, மாறிலி வெறுமை *pin, size_t inlen )
{
  மாறிலி unsigned எழுத்து * in = (மாறிலி unsigned எழுத்து *)pin;
  எனில்( inlen > 0 )
  {
    size_t இடது = S->buflen;
    size_t fill = BLAKE2B_BLOCKBYTES - இடது;
    எனில்( inlen > fill )
    {
      S->buflen = 0;
      memcpy( S->buf + இடது, in, fill ); /* Fill buffer */
      blake2b_increment_counter( S, BLAKE2B_BLOCKBYTES );
      blake2b_compress( S, S->buf ); /* Compress */
      in += fill; inlen -= fill;
      போது(inlen > BLAKE2B_BLOCKBYTES) {
        blake2b_increment_counter(S, BLAKE2B_BLOCKBYTES);
        blake2b_compress( S, in );
        in += BLAKE2B_BLOCKBYTES;
        inlen -= BLAKE2B_BLOCKBYTES;
      }
    }
    memcpy( S->buf + S->buflen, in, inlen );
    S->buflen += inlen;
  }
  திரும்பு 0;
}

முழு blake2b_final( blake2b_state *S, வெறுமை *out, size_t outlen )
{
  uint8_t buffer[BLAKE2B_OUTBYTES] = {0};
  size_t i;

  எனில்( out == வெற்று || outlen < S->outlen )
    திரும்பு -1;

  எனில்( blake2b_is_lastblock( S ) )
    திரும்பு -1;

  blake2b_increment_counter( S, S->buflen );
  blake2b_set_lastblock( S );
  memset( S->buf + S->buflen, 0, BLAKE2B_BLOCKBYTES - S->buflen ); /* Padding */
  blake2b_compress( S, S->buf );

  க்கு( i = 0; i < 8; ++i ) /* Output full hash to temp buffer */
    store64( buffer + அளவு( S->h[i] ) * i, S->h[i] );

  memcpy( out, buffer, S->outlen );
  secure_zero_memory(buffer, அளவு(buffer));
  திரும்பு 0;
}

/* inlen, at least, should be uint64_t. Others can be size_t. */
முழு blake2b( வெறுமை *out, size_t outlen, மாறிலி வெறுமை *in, size_t inlen, மாறிலி வெறுமை *சாவி, size_t keylen )
{
  blake2b_state S[1];

  /* Verify parameters */
  எனில் ( வெற்று == in && inlen > 0 ) திரும்பு -1;

  எனில் ( வெற்று == out ) திரும்பு -1;

  எனில்( வெற்று == சாவி && keylen > 0 ) திரும்பு -1;

  எனில்( !outlen || outlen > BLAKE2B_OUTBYTES ) திரும்பு -1;

  எனில்( keylen > BLAKE2B_KEYBYTES ) திரும்பு -1;

  எனில்( keylen > 0 )
  {
    எனில்( blake2b_init_சாவி( S, outlen, சாவி, keylen ) < 0 ) திரும்பு -1;
  }
  இல்லையெனில்
  {
    எனில்( blake2b_init( S, outlen ) < 0 ) திரும்பு -1;
  }

  blake2b_update( S, ( மாறிலி uint8_t * )in, inlen );
  blake2b_final( S, out, outlen );
  திரும்பு 0;
}

முழு blake2( வெறுமை *out, size_t outlen, மாறிலி வெறுமை *in, size_t inlen, மாறிலி வெறுமை *சாவி, size_t keylen ) {
  திரும்பு blake2b(out, outlen, in, inlen, சாவி, keylen);
}

#எனில் defined(SUPERCOP)
முழு crypto_hash( unsigned எழுத்து *out, unsigned எழுத்து *in, unsigned long long inlen )
{
  திரும்பு blake2b( out, BLAKE2B_OUTBYTES, in, inlen, வெற்று, 0 );
}
#endif

#எனில் defined(BLAKE2B_SELFTEST)
#அந்தர்பாவ <string.h>
#அந்தர்பாவ "blake2-kat.h"
முழு முதன்மை( வெறுமை )
{
  uint8_t சாவி[BLAKE2B_KEYBYTES];
  uint8_t buf[BLAKE2_KAT_LENGTH];
  size_t i, step;

  க்கு( i = 0; i < BLAKE2B_KEYBYTES; ++i )
    சாவி[i] = ( uint8_t )i;

  க்கு( i = 0; i < BLAKE2_KAT_LENGTH; ++i )
    buf[i] = ( uint8_t )i;

  /* Test simple API */
  க்கு( i = 0; i < BLAKE2_KAT_LENGTH; ++i )
  {
    uint8_t hash[BLAKE2B_OUTBYTES];
    blake2b( hash, BLAKE2B_OUTBYTES, buf, i, சாவி, BLAKE2B_KEYBYTES );

    எனில்( 0 != memcmp( hash, blake2b_keyed_kat[i], BLAKE2B_OUTBYTES ) )
    {
      செல் fail;
    }
  }

  /* Test streaming API */
  க்கு(step = 1; step < BLAKE2B_BLOCKBYTES; ++step) {
    க்கு (i = 0; i < BLAKE2_KAT_LENGTH; ++i) {
      uint8_t hash[BLAKE2B_OUTBYTES];
      blake2b_state S;
      uint8_t * p = buf;
      size_t mlen = i;
      முழு err = 0;

      எனில்( (err = blake2b_init_சாவி(&S, BLAKE2B_OUTBYTES, சாவி, BLAKE2B_KEYBYTES)) < 0 ) {
        செல் fail;
      }

      போது (mlen >= step) {
        எனில் ( (err = blake2b_update(&S, p, step)) < 0 ) {
          செல் fail;
        }
        mlen -= step;
        p += step;
      }
      எனில் ( (err = blake2b_update(&S, p, mlen)) < 0) {
        செல் fail;
      }
      எனில் ( (err = blake2b_final(&S, hash, BLAKE2B_OUTBYTES)) < 0) {
        செல் fail;
      }

      எனில் (0 != memcmp(hash, blake2b_keyed_kat[i], BLAKE2B_OUTBYTES)) {
        செல் fail;
      }
    }
  }

  puts( "ok" );
  திரும்பு 0;
fail:
  puts("error");
  திரும்பு -1;
}
#endif
