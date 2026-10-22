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

#অন্তর্ভাৱ <stdint.h>
#অন্তর্ভাৱ <string.h>
#অন্তর্ভাৱ <stdio.h>

#অন্তর্ভাৱ "blake2.h"
#অন্তর্ভাৱ "blake2-impl.h"

স্থির ধ্রুৱ uint64_t blake2b_IV[8] =
{
  0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL,
  0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL,
  0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL,
  0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL
};

স্থির ধ্রুৱ uint8_t blake2b_sigma[12][16] =
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


স্থির শূন্য blake2b_set_lastnode( blake2b_state *S )
{
  S->f[1] = (uint64_t)-1;
}

/* Some helper functions, not necessarily useful */
স্থির পূর্ণাঙ্ক blake2b_is_lastblock( ধ্রুৱ blake2b_state *S )
{
  ঘূৰাই S->f[0] != 0;
}

স্থির শূন্য blake2b_set_lastblock( blake2b_state *S )
{
  যদি( S->last_গ্ৰন্থি ) blake2b_set_lastnode( S );

  S->f[0] = (uint64_t)-1;
}

স্থির শূন্য blake2b_increment_counter( blake2b_state *S, ধ্রুৱ uint64_t inc )
{
  S->t[0] += inc;
  S->t[1] += ( S->t[0] < inc );
}

স্থির শূন্য blake2b_init0( blake2b_state *S )
{
  size_t i;
  memset( S, 0, পরিমাণ( blake2b_state ) );

  বাবে( i = 0; i < 8; ++i ) S->h[i] = blake2b_IV[i];
}

/* init xors IV with input parameter block */
পূর্ণাঙ্ক blake2b_init_param( blake2b_state *S, ধ্রুৱ blake2b_param *P )
{
  ধ্রুৱ uint8_t *p = ( ধ্রুৱ uint8_t * )( P );
  size_t i;

  blake2b_init0( S );

  /* IV XOR ParamBlock */
  বাবে( i = 0; i < 8; ++i )
    S->h[i] ^= load64( p + পরিমাণ( S->h[i] ) * i );

  S->outlen = P->digest_length;
  ঘূৰাই 0;
}



পূর্ণাঙ্ক blake2b_init( blake2b_state *S, size_t outlen )
{
  blake2b_param P[1];

  যদি ( ( !outlen ) || ( outlen > BLAKE2B_OUTBYTES ) ) ঘূৰাই -1;

  P->digest_length = (uint8_t)outlen;
  P->চাবি_length    = 0;
  P->fanout        = 1;
  P->depth         = 1;
  store32( &P->leaf_length, 0 );
  store32( &P->গ্ৰন্থি_offset, 0 );
  store32( &P->xof_length, 0 );
  P->গ্ৰন্থি_depth    = 0;
  P->inner_length  = 0;
  memset( P->reserved, 0, পরিমাণ( P->reserved ) );
  memset( P->salt,     0, পরিমাণ( P->salt ) );
  memset( P->personal, 0, পরিমাণ( P->personal ) );
  ঘূৰাই blake2b_init_param( S, P );
}


পূর্ণাঙ্ক blake2b_init_চাবি( blake2b_state *S, size_t outlen, ধ্রুৱ শূন্য *চাবি, size_t keylen )
{
  blake2b_param P[1];

  যদি ( ( !outlen ) || ( outlen > BLAKE2B_OUTBYTES ) ) ঘূৰাই -1;

  যদি ( !চাবি || !keylen || keylen > BLAKE2B_KEYBYTES ) ঘূৰাই -1;

  P->digest_length = (uint8_t)outlen;
  P->চাবি_length    = (uint8_t)keylen;
  P->fanout        = 1;
  P->depth         = 1;
  store32( &P->leaf_length, 0 );
  store32( &P->গ্ৰন্থি_offset, 0 );
  store32( &P->xof_length, 0 );
  P->গ্ৰন্থি_depth    = 0;
  P->inner_length  = 0;
  memset( P->reserved, 0, পরিমাণ( P->reserved ) );
  memset( P->salt,     0, পরিমাণ( P->salt ) );
  memset( P->personal, 0, পরিমাণ( P->personal ) );

  যদি( blake2b_init_param( S, P ) < 0 ) ঘূৰাই -1;

  {
    uint8_t block[BLAKE2B_BLOCKBYTES];
    memset( block, 0, BLAKE2B_BLOCKBYTES );
    memcpy( block, চাবি, keylen );
    blake2b_update( S, block, BLAKE2B_BLOCKBYTES );
    secure_zero_memory( block, BLAKE2B_BLOCKBYTES ); /* Burn the key from stack */
  }
  ঘূৰাই 0;
}

#define G(r,i,a,b,c,d)                      \
  কৰা {                                      \
    a = a + b + m[blake2b_sigma[r][2 * i + 0]]; \
    d = rotr64(d ^ a, 32);                  \
    c = c + d;                              \
    b = rotr64(b ^ c, 24);                  \
    a = a + b + m[blake2b_sigma[r][2 * i + 1]]; \
    d = rotr64(d ^ a, 16);                  \
    c = c + d;                              \
    b = rotr64(b ^ c, 63);                  \
  } যেতিয়ালৈ(0)

#define ROUND(r)                    \
  কৰা {                              \
    G(r,0,v[ 0],v[ 4],v[ 8],v[12]); \
    G(r,1,v[ 1],v[ 5],v[ 9],v[13]); \
    G(r,2,v[ 2],v[ 6],v[10],v[14]); \
    G(r,3,v[ 3],v[ 7],v[11],v[15]); \
    G(r,4,v[ 0],v[ 5],v[10],v[15]); \
    G(r,5,v[ 1],v[ 6],v[11],v[12]); \
    G(r,6,v[ 2],v[ 7],v[ 8],v[13]); \
    G(r,7,v[ 3],v[ 4],v[ 9],v[14]); \
  } যেতিয়ালৈ(0)

স্থির শূন্য blake2b_compress( blake2b_state *S, ধ্রুৱ uint8_t block[BLAKE2B_BLOCKBYTES] )
{
  uint64_t m[16];
  uint64_t v[16];
  size_t i;

  বাবে( i = 0; i < 16; ++i ) {
    m[i] = load64( block + i * পরিমাণ( m[i] ) );
  }

  বাবে( i = 0; i < 8; ++i ) {
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

  বাবে( i = 0; i < 8; ++i ) {
    S->h[i] = S->h[i] ^ v[i] ^ v[i + 8];
  }
}

#undef G
#undef ROUND

পূর্ণাঙ্ক blake2b_update( blake2b_state *S, ধ্রুৱ শূন্য *pin, size_t inlen )
{
  ধ্রুৱ unsigned ৱর্ণ * in = (ধ্রুৱ unsigned ৱর্ণ *)pin;
  যদি( inlen > 0 )
  {
    size_t বাম = S->buflen;
    size_t fill = BLAKE2B_BLOCKBYTES - বাম;
    যদি( inlen > fill )
    {
      S->buflen = 0;
      memcpy( S->buf + বাম, in, fill ); /* Fill buffer */
      blake2b_increment_counter( S, BLAKE2B_BLOCKBYTES );
      blake2b_compress( S, S->buf ); /* Compress */
      in += fill; inlen -= fill;
      যেতিয়ালৈ(inlen > BLAKE2B_BLOCKBYTES) {
        blake2b_increment_counter(S, BLAKE2B_BLOCKBYTES);
        blake2b_compress( S, in );
        in += BLAKE2B_BLOCKBYTES;
        inlen -= BLAKE2B_BLOCKBYTES;
      }
    }
    memcpy( S->buf + S->buflen, in, inlen );
    S->buflen += inlen;
  }
  ঘূৰাই 0;
}

পূর্ণাঙ্ক blake2b_final( blake2b_state *S, শূন্য *out, size_t outlen )
{
  uint8_t buffer[BLAKE2B_OUTBYTES] = {0};
  size_t i;

  যদি( out == রিক্ত || outlen < S->outlen )
    ঘূৰাই -1;

  যদি( blake2b_is_lastblock( S ) )
    ঘূৰাই -1;

  blake2b_increment_counter( S, S->buflen );
  blake2b_set_lastblock( S );
  memset( S->buf + S->buflen, 0, BLAKE2B_BLOCKBYTES - S->buflen ); /* Padding */
  blake2b_compress( S, S->buf );

  বাবে( i = 0; i < 8; ++i ) /* Output full hash to temp buffer */
    store64( buffer + পরিমাণ( S->h[i] ) * i, S->h[i] );

  memcpy( out, buffer, S->outlen );
  secure_zero_memory(buffer, পরিমাণ(buffer));
  ঘূৰাই 0;
}

/* inlen, at least, should be uint64_t. Others can be size_t. */
পূর্ণাঙ্ক blake2b( শূন্য *out, size_t outlen, ধ্রুৱ শূন্য *in, size_t inlen, ধ্রুৱ শূন্য *চাবি, size_t keylen )
{
  blake2b_state S[1];

  /* Verify parameters */
  যদি ( রিক্ত == in && inlen > 0 ) ঘূৰাই -1;

  যদি ( রিক্ত == out ) ঘূৰাই -1;

  যদি( রিক্ত == চাবি && keylen > 0 ) ঘূৰাই -1;

  যদি( !outlen || outlen > BLAKE2B_OUTBYTES ) ঘূৰাই -1;

  যদি( keylen > BLAKE2B_KEYBYTES ) ঘূৰাই -1;

  যদি( keylen > 0 )
  {
    যদি( blake2b_init_চাবি( S, outlen, চাবি, keylen ) < 0 ) ঘূৰাই -1;
  }
  অন্যথা
  {
    যদি( blake2b_init( S, outlen ) < 0 ) ঘূৰাই -1;
  }

  blake2b_update( S, ( ধ্রুৱ uint8_t * )in, inlen );
  blake2b_final( S, out, outlen );
  ঘূৰাই 0;
}

পূর্ণাঙ্ক blake2( শূন্য *out, size_t outlen, ধ্রুৱ শূন্য *in, size_t inlen, ধ্রুৱ শূন্য *চাবি, size_t keylen ) {
  ঘূৰাই blake2b(out, outlen, in, inlen, চাবি, keylen);
}

#যদি defined(SUPERCOP)
পূর্ণাঙ্ক crypto_hash( unsigned ৱর্ণ *out, unsigned ৱর্ণ *in, unsigned long long inlen )
{
  ঘূৰাই blake2b( out, BLAKE2B_OUTBYTES, in, inlen, রিক্ত, 0 );
}
#endif

#যদি defined(BLAKE2B_SELFTEST)
#অন্তর্ভাৱ <string.h>
#অন্তর্ভাৱ "blake2-kat.h"
পূর্ণাঙ্ক main( শূন্য )
{
  uint8_t চাবি[BLAKE2B_KEYBYTES];
  uint8_t buf[BLAKE2_KAT_LENGTH];
  size_t i, step;

  বাবে( i = 0; i < BLAKE2B_KEYBYTES; ++i )
    চাবি[i] = ( uint8_t )i;

  বাবে( i = 0; i < BLAKE2_KAT_LENGTH; ++i )
    buf[i] = ( uint8_t )i;

  /* Test simple API */
  বাবে( i = 0; i < BLAKE2_KAT_LENGTH; ++i )
  {
    uint8_t hash[BLAKE2B_OUTBYTES];
    blake2b( hash, BLAKE2B_OUTBYTES, buf, i, চাবি, BLAKE2B_KEYBYTES );

    যদি( 0 != memcmp( hash, blake2b_keyed_kat[i], BLAKE2B_OUTBYTES ) )
    {
      গচ্ছ fail;
    }
  }

  /* Test streaming API */
  বাবে(step = 1; step < BLAKE2B_BLOCKBYTES; ++step) {
    বাবে (i = 0; i < BLAKE2_KAT_LENGTH; ++i) {
      uint8_t hash[BLAKE2B_OUTBYTES];
      blake2b_state S;
      uint8_t * p = buf;
      size_t mlen = i;
      পূর্ণাঙ্ক err = 0;

      যদি( (err = blake2b_init_চাবি(&S, BLAKE2B_OUTBYTES, চাবি, BLAKE2B_KEYBYTES)) < 0 ) {
        গচ্ছ fail;
      }

      যেতিয়ালৈ (mlen >= step) {
        যদি ( (err = blake2b_update(&S, p, step)) < 0 ) {
          গচ্ছ fail;
        }
        mlen -= step;
        p += step;
      }
      যদি ( (err = blake2b_update(&S, p, mlen)) < 0) {
        গচ্ছ fail;
      }
      যদি ( (err = blake2b_final(&S, hash, BLAKE2B_OUTBYTES)) < 0) {
        গচ্ছ fail;
      }

      যদি (0 != memcmp(hash, blake2b_keyed_kat[i], BLAKE2B_OUTBYTES)) {
        গচ্ছ fail;
      }
    }
  }

  puts( "ok" );
  ঘূৰাই 0;
fail:
  puts("error");
  ঘূৰাই -1;
}
#endif
