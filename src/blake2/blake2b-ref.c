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

#अन्तर्भाव <stdint.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव <stdio.h>

#अन्तर्भाव "blake2.h"
#अन्तर्भाव "blake2-impl.h"

स्थिर ध्रुव uint64_t blake2b_IV[8] =
{
  0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL,
  0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL,
  0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL,
  0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL
};

स्थिर ध्रुव uint8_t blake2b_sigma[12][16] =
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


स्थिर शून्य blake2b_set_lastnode( blake2b_state *S )
{
  S->f[1] = (uint64_t)-1;
}

/* Some helper functions, not necessarily useful */
स्थिर पूर्णाङ्क blake2b_is_lastblock( ध्रुव blake2b_state *S )
{
  प्रत्यागम S->f[0] != 0;
}

स्थिर शून्य blake2b_set_lastblock( blake2b_state *S )
{
  यदि( S->last_गाठ ) blake2b_set_lastnode( S );

  S->f[0] = (uint64_t)-1;
}

स्थिर शून्य blake2b_increment_counter( blake2b_state *S, ध्रुव uint64_t inc )
{
  S->t[0] += inc;
  S->t[1] += ( S->t[0] < inc );
}

स्थिर शून्य blake2b_init0( blake2b_state *S )
{
  size_t i;
  memset( S, 0, परिमाण( blake2b_state ) );

  प्रति( i = 0; i < 8; ++i ) S->h[i] = blake2b_IV[i];
}

/* init xors IV with input parameter block */
पूर्णाङ्क blake2b_init_param( blake2b_state *S, ध्रुव blake2b_param *P )
{
  ध्रुव uint8_t *p = ( ध्रुव uint8_t * )( P );
  size_t i;

  blake2b_init0( S );

  /* IV XOR ParamBlock */
  प्रति( i = 0; i < 8; ++i )
    S->h[i] ^= load64( p + परिमाण( S->h[i] ) * i );

  S->outlen = P->digest_length;
  प्रत्यागम 0;
}



पूर्णाङ्क blake2b_init( blake2b_state *S, size_t outlen )
{
  blake2b_param P[1];

  यदि ( ( !outlen ) || ( outlen > BLAKE2B_OUTBYTES ) ) प्रत्यागम -1;

  P->digest_length = (uint8_t)outlen;
  P->किल्ली_length    = 0;
  P->fanout        = 1;
  P->depth         = 1;
  store32( &P->leaf_length, 0 );
  store32( &P->गाठ_offset, 0 );
  store32( &P->xof_length, 0 );
  P->गाठ_depth    = 0;
  P->inner_length  = 0;
  memset( P->reserved, 0, परिमाण( P->reserved ) );
  memset( P->salt,     0, परिमाण( P->salt ) );
  memset( P->personal, 0, परिमाण( P->personal ) );
  प्रत्यागम blake2b_init_param( S, P );
}


पूर्णाङ्क blake2b_init_किल्ली( blake2b_state *S, size_t outlen, ध्रुव शून्य *किल्ली, size_t keylen )
{
  blake2b_param P[1];

  यदि ( ( !outlen ) || ( outlen > BLAKE2B_OUTBYTES ) ) प्रत्यागम -1;

  यदि ( !किल्ली || !keylen || keylen > BLAKE2B_KEYBYTES ) प्रत्यागम -1;

  P->digest_length = (uint8_t)outlen;
  P->किल्ली_length    = (uint8_t)keylen;
  P->fanout        = 1;
  P->depth         = 1;
  store32( &P->leaf_length, 0 );
  store32( &P->गाठ_offset, 0 );
  store32( &P->xof_length, 0 );
  P->गाठ_depth    = 0;
  P->inner_length  = 0;
  memset( P->reserved, 0, परिमाण( P->reserved ) );
  memset( P->salt,     0, परिमाण( P->salt ) );
  memset( P->personal, 0, परिमाण( P->personal ) );

  यदि( blake2b_init_param( S, P ) < 0 ) प्रत्यागम -1;

  {
    uint8_t block[BLAKE2B_BLOCKBYTES];
    memset( block, 0, BLAKE2B_BLOCKBYTES );
    memcpy( block, किल्ली, keylen );
    blake2b_update( S, block, BLAKE2B_BLOCKBYTES );
    secure_zero_memory( block, BLAKE2B_BLOCKBYTES ); /* Burn the key from stack */
  }
  प्रत्यागम 0;
}

#define G(r,i,a,b,c,d)                      \
  कुरु {                                      \
    a = a + b + m[blake2b_sigma[r][2 * i + 0]]; \
    d = rotr64(d ^ a, 32);                  \
    c = c + d;                              \
    b = rotr64(b ^ c, 24);                  \
    a = a + b + m[blake2b_sigma[r][2 * i + 1]]; \
    d = rotr64(d ^ a, 16);                  \
    c = c + d;                              \
    b = rotr64(b ^ c, 63);                  \
  } यावत्(0)

#define ROUND(r)                    \
  कुरु {                              \
    G(r,0,v[ 0],v[ 4],v[ 8],v[12]); \
    G(r,1,v[ 1],v[ 5],v[ 9],v[13]); \
    G(r,2,v[ 2],v[ 6],v[10],v[14]); \
    G(r,3,v[ 3],v[ 7],v[11],v[15]); \
    G(r,4,v[ 0],v[ 5],v[10],v[15]); \
    G(r,5,v[ 1],v[ 6],v[11],v[12]); \
    G(r,6,v[ 2],v[ 7],v[ 8],v[13]); \
    G(r,7,v[ 3],v[ 4],v[ 9],v[14]); \
  } यावत्(0)

स्थिर शून्य blake2b_compress( blake2b_state *S, ध्रुव uint8_t block[BLAKE2B_BLOCKBYTES] )
{
  uint64_t m[16];
  uint64_t v[16];
  size_t i;

  प्रति( i = 0; i < 16; ++i ) {
    m[i] = load64( block + i * परिमाण( m[i] ) );
  }

  प्रति( i = 0; i < 8; ++i ) {
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

  प्रति( i = 0; i < 8; ++i ) {
    S->h[i] = S->h[i] ^ v[i] ^ v[i + 8];
  }
}

#undef G
#undef ROUND

पूर्णाङ्क blake2b_update( blake2b_state *S, ध्रुव शून्य *pin, size_t inlen )
{
  ध्रुव unsigned वर्ण * in = (ध्रुव unsigned वर्ण *)pin;
  यदि( inlen > 0 )
  {
    size_t डावा = S->buflen;
    size_t fill = BLAKE2B_BLOCKBYTES - डावा;
    यदि( inlen > fill )
    {
      S->buflen = 0;
      memcpy( S->buf + डावा, in, fill ); /* Fill buffer */
      blake2b_increment_counter( S, BLAKE2B_BLOCKBYTES );
      blake2b_compress( S, S->buf ); /* Compress */
      in += fill; inlen -= fill;
      यावत्(inlen > BLAKE2B_BLOCKBYTES) {
        blake2b_increment_counter(S, BLAKE2B_BLOCKBYTES);
        blake2b_compress( S, in );
        in += BLAKE2B_BLOCKBYTES;
        inlen -= BLAKE2B_BLOCKBYTES;
      }
    }
    memcpy( S->buf + S->buflen, in, inlen );
    S->buflen += inlen;
  }
  प्रत्यागम 0;
}

पूर्णाङ्क blake2b_final( blake2b_state *S, शून्य *out, size_t outlen )
{
  uint8_t buffer[BLAKE2B_OUTBYTES] = {0};
  size_t i;

  यदि( out == रिक्त || outlen < S->outlen )
    प्रत्यागम -1;

  यदि( blake2b_is_lastblock( S ) )
    प्रत्यागम -1;

  blake2b_increment_counter( S, S->buflen );
  blake2b_set_lastblock( S );
  memset( S->buf + S->buflen, 0, BLAKE2B_BLOCKBYTES - S->buflen ); /* Padding */
  blake2b_compress( S, S->buf );

  प्रति( i = 0; i < 8; ++i ) /* Output full hash to temp buffer */
    store64( buffer + परिमाण( S->h[i] ) * i, S->h[i] );

  memcpy( out, buffer, S->outlen );
  secure_zero_memory(buffer, परिमाण(buffer));
  प्रत्यागम 0;
}

/* inlen, at least, should be uint64_t. Others can be size_t. */
पूर्णाङ्क blake2b( शून्य *out, size_t outlen, ध्रुव शून्य *in, size_t inlen, ध्रुव शून्य *किल्ली, size_t keylen )
{
  blake2b_state S[1];

  /* Verify parameters */
  यदि ( रिक्त == in && inlen > 0 ) प्रत्यागम -1;

  यदि ( रिक्त == out ) प्रत्यागम -1;

  यदि( रिक्त == किल्ली && keylen > 0 ) प्रत्यागम -1;

  यदि( !outlen || outlen > BLAKE2B_OUTBYTES ) प्रत्यागम -1;

  यदि( keylen > BLAKE2B_KEYBYTES ) प्रत्यागम -1;

  यदि( keylen > 0 )
  {
    यदि( blake2b_init_किल्ली( S, outlen, किल्ली, keylen ) < 0 ) प्रत्यागम -1;
  }
  अन्यथा
  {
    यदि( blake2b_init( S, outlen ) < 0 ) प्रत्यागम -1;
  }

  blake2b_update( S, ( ध्रुव uint8_t * )in, inlen );
  blake2b_final( S, out, outlen );
  प्रत्यागम 0;
}

पूर्णाङ्क blake2( शून्य *out, size_t outlen, ध्रुव शून्य *in, size_t inlen, ध्रुव शून्य *किल्ली, size_t keylen ) {
  प्रत्यागम blake2b(out, outlen, in, inlen, किल्ली, keylen);
}

#यदि defined(SUPERCOP)
पूर्णाङ्क crypto_hash( unsigned वर्ण *out, unsigned वर्ण *in, unsigned long long inlen )
{
  प्रत्यागम blake2b( out, BLAKE2B_OUTBYTES, in, inlen, रिक्त, 0 );
}
#endif

#यदि defined(BLAKE2B_SELFTEST)
#अन्तर्भाव <string.h>
#अन्तर्भाव "blake2-kat.h"
पूर्णाङ्क main( शून्य )
{
  uint8_t किल्ली[BLAKE2B_KEYBYTES];
  uint8_t buf[BLAKE2_KAT_LENGTH];
  size_t i, step;

  प्रति( i = 0; i < BLAKE2B_KEYBYTES; ++i )
    किल्ली[i] = ( uint8_t )i;

  प्रति( i = 0; i < BLAKE2_KAT_LENGTH; ++i )
    buf[i] = ( uint8_t )i;

  /* Test simple API */
  प्रति( i = 0; i < BLAKE2_KAT_LENGTH; ++i )
  {
    uint8_t hash[BLAKE2B_OUTBYTES];
    blake2b( hash, BLAKE2B_OUTBYTES, buf, i, किल्ली, BLAKE2B_KEYBYTES );

    यदि( 0 != memcmp( hash, blake2b_keyed_kat[i], BLAKE2B_OUTBYTES ) )
    {
      गच्छ fail;
    }
  }

  /* Test streaming API */
  प्रति(step = 1; step < BLAKE2B_BLOCKBYTES; ++step) {
    प्रति (i = 0; i < BLAKE2_KAT_LENGTH; ++i) {
      uint8_t hash[BLAKE2B_OUTBYTES];
      blake2b_state S;
      uint8_t * p = buf;
      size_t mlen = i;
      पूर्णाङ्क err = 0;

      यदि( (err = blake2b_init_किल्ली(&S, BLAKE2B_OUTBYTES, किल्ली, BLAKE2B_KEYBYTES)) < 0 ) {
        गच्छ fail;
      }

      यावत् (mlen >= step) {
        यदि ( (err = blake2b_update(&S, p, step)) < 0 ) {
          गच्छ fail;
        }
        mlen -= step;
        p += step;
      }
      यदि ( (err = blake2b_update(&S, p, mlen)) < 0) {
        गच्छ fail;
      }
      यदि ( (err = blake2b_final(&S, hash, BLAKE2B_OUTBYTES)) < 0) {
        गच्छ fail;
      }

      यदि (0 != memcmp(hash, blake2b_keyed_kat[i], BLAKE2B_OUTBYTES)) {
        गच्छ fail;
      }
    }
  }

  puts( "ok" );
  प्रत्यागम 0;
fail:
  puts("error");
  प्रत्यागम -1;
}
#endif
