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

#अगर_घोषित HAVE_CONFIG_H
# include <config.h>
#पूर्ण_अगर

#समावेश <stdint.h>
#समावेश <string.h>
#समावेश <stdio.h>

#समावेश "blake2.h"
#समावेश "blake2-impl.h"

अटल स्थिर uint64_t blake2b_IV[8] =
{
  0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL,
  0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL,
  0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL,
  0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL
};

अटल स्थिर uint8_t blake2b_sigma[12][16] =
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


अटल व्योम blake2b_set_lastnode( blake2b_state *S )
{
  S->f[1] = (uint64_t)-1;
}

/* Some helper functions, not necessarily useful */
अटल पूर्णांक blake2b_is_lastblock( स्थिर blake2b_state *S )
{
  वापस S->f[0] != 0;
}

अटल व्योम blake2b_set_lastblock( blake2b_state *S )
{
  अगर( S->last_गाँठ ) blake2b_set_lastnode( S );

  S->f[0] = (uint64_t)-1;
}

अटल व्योम blake2b_increment_गणक( blake2b_state *S, स्थिर uint64_t inc )
{
  S->t[0] += inc;
  S->t[1] += ( S->t[0] < inc );
}

अटल व्योम blake2b_init0( blake2b_state *S )
{
  माप_प्रकार i;
  स_रखो( S, 0, माप( blake2b_state ) );

  क्रम( i = 0; i < 8; ++i ) S->h[i] = blake2b_IV[i];
}

/* init xors IV with input parameter block */
पूर्णांक blake2b_init_param( blake2b_state *S, स्थिर blake2b_param *P )
{
  स्थिर uint8_t *p = ( स्थिर uint8_t * )( P );
  माप_प्रकार i;

  blake2b_init0( S );

  /* IV XOR ParamBlock */
  क्रम( i = 0; i < 8; ++i )
    S->h[i] ^= load64( p + माप( S->h[i] ) * i );

  S->outlen = P->digest_लंबाई;
  वापस 0;
}



पूर्णांक blake2b_init( blake2b_state *S, माप_प्रकार outlen )
{
  blake2b_param P[1];

  अगर ( ( !outlen ) || ( outlen > BLAKE2B_OUTBYTES ) ) वापस -1;

  P->digest_लंबाई = (uint8_t)outlen;
  P->कुंजी_लंबाई    = 0;
  P->fanout        = 1;
  P->depth         = 1;
  store32( &P->leaf_लंबाई, 0 );
  store32( &P->गाँठ_offset, 0 );
  store32( &P->xof_लंबाई, 0 );
  P->गाँठ_depth    = 0;
  P->inner_लंबाई  = 0;
  स_रखो( P->reserved, 0, माप( P->reserved ) );
  स_रखो( P->salt,     0, माप( P->salt ) );
  स_रखो( P->personal, 0, माप( P->personal ) );
  वापस blake2b_init_param( S, P );
}


पूर्णांक blake2b_init_कुंजी( blake2b_state *S, माप_प्रकार outlen, स्थिर व्योम *कुंजी, माप_प्रकार keylen )
{
  blake2b_param P[1];

  अगर ( ( !outlen ) || ( outlen > BLAKE2B_OUTBYTES ) ) वापस -1;

  अगर ( !कुंजी || !keylen || keylen > BLAKE2B_KEYBYTES ) वापस -1;

  P->digest_लंबाई = (uint8_t)outlen;
  P->कुंजी_लंबाई    = (uint8_t)keylen;
  P->fanout        = 1;
  P->depth         = 1;
  store32( &P->leaf_लंबाई, 0 );
  store32( &P->गाँठ_offset, 0 );
  store32( &P->xof_लंबाई, 0 );
  P->गाँठ_depth    = 0;
  P->inner_लंबाई  = 0;
  स_रखो( P->reserved, 0, माप( P->reserved ) );
  स_रखो( P->salt,     0, माप( P->salt ) );
  स_रखो( P->personal, 0, माप( P->personal ) );

  अगर( blake2b_init_param( S, P ) < 0 ) वापस -1;

  {
    uint8_t block[BLAKE2B_BLOCKBYTES];
    स_रखो( block, 0, BLAKE2B_BLOCKBYTES );
    स_नकल( block, कुंजी, keylen );
    blake2b_update( S, block, BLAKE2B_BLOCKBYTES );
    secure_zero_memory( block, BLAKE2B_BLOCKBYTES ); /* Burn the key from stack */
  }
  वापस 0;
}

#घोषणा G(r,i,a,b,c,d)                      \
  करो {                                      \
    a = a + b + m[blake2b_sigma[r][2 * i + 0]]; \
    d = rotr64(d ^ a, 32);                  \
    c = c + d;                              \
    b = rotr64(b ^ c, 24);                  \
    a = a + b + m[blake2b_sigma[r][2 * i + 1]]; \
    d = rotr64(d ^ a, 16);                  \
    c = c + d;                              \
    b = rotr64(b ^ c, 63);                  \
  } जबतक(0)

#घोषणा ROUND(r)                    \
  करो {                              \
    G(r,0,v[ 0],v[ 4],v[ 8],v[12]); \
    G(r,1,v[ 1],v[ 5],v[ 9],v[13]); \
    G(r,2,v[ 2],v[ 6],v[10],v[14]); \
    G(r,3,v[ 3],v[ 7],v[11],v[15]); \
    G(r,4,v[ 0],v[ 5],v[10],v[15]); \
    G(r,5,v[ 1],v[ 6],v[11],v[12]); \
    G(r,6,v[ 2],v[ 7],v[ 8],v[13]); \
    G(r,7,v[ 3],v[ 4],v[ 9],v[14]); \
  } जबतक(0)

अटल व्योम blake2b_compress( blake2b_state *S, स्थिर uint8_t block[BLAKE2B_BLOCKBYTES] )
{
  uint64_t m[16];
  uint64_t v[16];
  माप_प्रकार i;

  क्रम( i = 0; i < 16; ++i ) {
    m[i] = load64( block + i * माप( m[i] ) );
  }

  क्रम( i = 0; i < 8; ++i ) {
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

  क्रम( i = 0; i < 8; ++i ) {
    S->h[i] = S->h[i] ^ v[i] ^ v[i + 8];
  }
}

#अघोषित G
#अघोषित ROUND

पूर्णांक blake2b_update( blake2b_state *S, स्थिर व्योम *pin, माप_प्रकार inlen )
{
  स्थिर अचिन्हित अक्षर * in = (स्थिर अचिन्हित अक्षर *)pin;
  अगर( inlen > 0 )
  {
    माप_प्रकार बायाँ = S->buflen;
    माप_प्रकार fill = BLAKE2B_BLOCKBYTES - बायाँ;
    अगर( inlen > fill )
    {
      S->buflen = 0;
      स_नकल( S->buf + बायाँ, in, fill ); /* Fill buffer */
      blake2b_increment_गणक( S, BLAKE2B_BLOCKBYTES );
      blake2b_compress( S, S->buf ); /* Compress */
      in += fill; inlen -= fill;
      जबतक(inlen > BLAKE2B_BLOCKBYTES) {
        blake2b_increment_गणक(S, BLAKE2B_BLOCKBYTES);
        blake2b_compress( S, in );
        in += BLAKE2B_BLOCKBYTES;
        inlen -= BLAKE2B_BLOCKBYTES;
      }
    }
    स_नकल( S->buf + S->buflen, in, inlen );
    S->buflen += inlen;
  }
  वापस 0;
}

पूर्णांक blake2b_final( blake2b_state *S, व्योम *out, माप_प्रकार outlen )
{
  uint8_t बफर[BLAKE2B_OUTBYTES] = {0};
  माप_प्रकार i;

  अगर( out == शून्य || outlen < S->outlen )
    वापस -1;

  अगर( blake2b_is_lastblock( S ) )
    वापस -1;

  blake2b_increment_गणक( S, S->buflen );
  blake2b_set_lastblock( S );
  स_रखो( S->buf + S->buflen, 0, BLAKE2B_BLOCKBYTES - S->buflen ); /* Padding */
  blake2b_compress( S, S->buf );

  क्रम( i = 0; i < 8; ++i ) /* Output full hash to temp buffer */
    store64( बफर + माप( S->h[i] ) * i, S->h[i] );

  स_नकल( out, बफर, S->outlen );
  secure_zero_memory(बफर, माप(बफर));
  वापस 0;
}

/* inlen, at least, should be uint64_t. Others can be size_t. */
पूर्णांक blake2b( व्योम *out, माप_प्रकार outlen, स्थिर व्योम *in, माप_प्रकार inlen, स्थिर व्योम *कुंजी, माप_प्रकार keylen )
{
  blake2b_state S[1];

  /* Verify parameters */
  अगर ( शून्य == in && inlen > 0 ) वापस -1;

  अगर ( शून्य == out ) वापस -1;

  अगर( शून्य == कुंजी && keylen > 0 ) वापस -1;

  अगर( !outlen || outlen > BLAKE2B_OUTBYTES ) वापस -1;

  अगर( keylen > BLAKE2B_KEYBYTES ) वापस -1;

  अगर( keylen > 0 )
  {
    अगर( blake2b_init_कुंजी( S, outlen, कुंजी, keylen ) < 0 ) वापस -1;
  }
  अन्यथा
  {
    अगर( blake2b_init( S, outlen ) < 0 ) वापस -1;
  }

  blake2b_update( S, ( स्थिर uint8_t * )in, inlen );
  blake2b_final( S, out, outlen );
  वापस 0;
}

पूर्णांक blake2( व्योम *out, माप_प्रकार outlen, स्थिर व्योम *in, माप_प्रकार inlen, स्थिर व्योम *कुंजी, माप_प्रकार keylen ) {
  वापस blake2b(out, outlen, in, inlen, कुंजी, keylen);
}

#अगर defined(SUPERCOP)
पूर्णांक crypto_hash( अचिन्हित अक्षर *out, अचिन्हित अक्षर *in, अचिन्हित दीर्घ दीर्घ inlen )
{
  वापस blake2b( out, BLAKE2B_OUTBYTES, in, inlen, शून्य, 0 );
}
#पूर्ण_अगर

#अगर defined(BLAKE2B_SELFTEST)
#समावेश <string.h>
#समावेश "blake2-kat.h"
पूर्णांक मुख्य( व्योम )
{
  uint8_t कुंजी[BLAKE2B_KEYBYTES];
  uint8_t buf[BLAKE2_KAT_लंबाई];
  माप_प्रकार i, step;

  क्रम( i = 0; i < BLAKE2B_KEYBYTES; ++i )
    कुंजी[i] = ( uint8_t )i;

  क्रम( i = 0; i < BLAKE2_KAT_लंबाई; ++i )
    buf[i] = ( uint8_t )i;

  /* Test simple API */
  क्रम( i = 0; i < BLAKE2_KAT_लंबाई; ++i )
  {
    uint8_t hash[BLAKE2B_OUTBYTES];
    blake2b( hash, BLAKE2B_OUTBYTES, buf, i, कुंजी, BLAKE2B_KEYBYTES );

    अगर( 0 != स_भेद( hash, blake2b_keyed_kat[i], BLAKE2B_OUTBYTES ) )
    {
      जाओ fail;
    }
  }

  /* Test streaming API */
  क्रम(step = 1; step < BLAKE2B_BLOCKBYTES; ++step) {
    क्रम (i = 0; i < BLAKE2_KAT_लंबाई; ++i) {
      uint8_t hash[BLAKE2B_OUTBYTES];
      blake2b_state S;
      uint8_t * p = buf;
      माप_प्रकार mlen = i;
      पूर्णांक err = 0;

      अगर( (err = blake2b_init_कुंजी(&S, BLAKE2B_OUTBYTES, कुंजी, BLAKE2B_KEYBYTES)) < 0 ) {
        जाओ fail;
      }

      जबतक (mlen >= step) {
        अगर ( (err = blake2b_update(&S, p, step)) < 0 ) {
          जाओ fail;
        }
        mlen -= step;
        p += step;
      }
      अगर ( (err = blake2b_update(&S, p, mlen)) < 0) {
        जाओ fail;
      }
      अगर ( (err = blake2b_final(&S, hash, BLAKE2B_OUTBYTES)) < 0) {
        जाओ fail;
      }

      अगर (0 != स_भेद(hash, blake2b_keyed_kat[i], BLAKE2B_OUTBYTES)) {
        जाओ fail;
      }
    }
  }

  माला_दो( "ok" );
  वापस 0;
fail:
  माला_दो("error");
  वापस -1;
}
#पूर्ण_अगर
