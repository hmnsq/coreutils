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

#جيڪڏهن_وصف_ٿيل HAVE_CONFIG_H
# include <config.h>
#جيڪڏهن_پورو

#ضمن <stdint.h>
#ضمن <string.h>
#ضمن <stdio.h>

#ضمن "blake2.h"
#ضمن "blake2-impl.h"

ساكن ثابت uint64_t blake2b_IV[8] =
{
  0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL,
  0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL,
  0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL,
  0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL
};

ساكن ثابت uint8_t blake2b_sigma[12][16] =
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


ساكن فراغ blake2b_سيٽ_lastnode( blake2b_state *S )
{
  S->f[1] = (uint64_t)-1;
}

/* Some helper functions, not necessarily useful */
ساكن صحيح blake2b_is_lastblock( ثابت blake2b_state *S )
{
  أرجع S->f[0] != 0;
}

ساكن فراغ blake2b_سيٽ_lastblock( blake2b_state *S )
{
  إذا( S->آخري_ڳنڍ ) blake2b_سيٽ_lastnode( S );

  S->f[0] = (uint64_t)-1;
}

ساكن فراغ blake2b_increment_counter( blake2b_state *S, ثابت uint64_t inc )
{
  S->t[0] += inc;
  S->t[1] += ( S->t[0] < inc );
}

ساكن فراغ blake2b_init0( blake2b_state *S )
{
  size_t i;
  memset( S, 0, حجم( blake2b_state ) );

  لكل( i = 0; i < 8; ++i ) S->h[i] = blake2b_IV[i];
}

/* init xors IV with input parameter block */
صحيح blake2b_init_param( blake2b_state *S, ثابت blake2b_param *P )
{
  ثابت uint8_t *p = ( ثابت uint8_t * )( P );
  size_t i;

  blake2b_init0( S );

  /* IV XOR ParamBlock */
  لكل( i = 0; i < 8; ++i )
    S->h[i] ^= load64( p + حجم( S->h[i] ) * i );

  S->outlen = P->digest_ڊيگهائي;
  أرجع 0;
}



صحيح blake2b_init( blake2b_state *S, size_t outlen )
{
  blake2b_param P[1];

  إذا ( ( !outlen ) || ( outlen > BLAKE2B_OUTBYTES ) ) أرجع -1;

  P->digest_ڊيگهائي = (uint8_t)outlen;
  P->ڪنجي_ڊيگهائي    = 0;
  P->fanout        = 1;
  P->اونهائي         = 1;
  store32( &P->پن_ڊيگهائي, 0 );
  store32( &P->ڳنڍ_وٿي, 0 );
  store32( &P->xof_ڊيگهائي, 0 );
  P->ڳنڍ_اونهائي    = 0;
  P->inner_ڊيگهائي  = 0;
  memset( P->reserved, 0, حجم( P->reserved ) );
  memset( P->salt,     0, حجم( P->salt ) );
  memset( P->personal, 0, حجم( P->personal ) );
  أرجع blake2b_init_param( S, P );
}


صحيح blake2b_init_ڪنجي( blake2b_state *S, size_t outlen, ثابت فراغ *ڪنجي, size_t keylen )
{
  blake2b_param P[1];

  إذا ( ( !outlen ) || ( outlen > BLAKE2B_OUTBYTES ) ) أرجع -1;

  إذا ( !ڪنجي || !keylen || keylen > BLAKE2B_KEYBYTES ) أرجع -1;

  P->digest_ڊيگهائي = (uint8_t)outlen;
  P->ڪنجي_ڊيگهائي    = (uint8_t)keylen;
  P->fanout        = 1;
  P->اونهائي         = 1;
  store32( &P->پن_ڊيگهائي, 0 );
  store32( &P->ڳنڍ_وٿي, 0 );
  store32( &P->xof_ڊيگهائي, 0 );
  P->ڳنڍ_اونهائي    = 0;
  P->inner_ڊيگهائي  = 0;
  memset( P->reserved, 0, حجم( P->reserved ) );
  memset( P->salt,     0, حجم( P->salt ) );
  memset( P->personal, 0, حجم( P->personal ) );

  إذا( blake2b_init_param( S, P ) < 0 ) أرجع -1;

  {
    uint8_t بلاڪ[BLAKE2B_BLOCKBYTES];
    memset( بلاڪ, 0, BLAKE2B_BLOCKBYTES );
    يادگيري_نقل( بلاڪ, ڪنجي, keylen );
    blake2b_update( S, بلاڪ, BLAKE2B_BLOCKBYTES );
    secure_zero_يادگيري( بلاڪ, BLAKE2B_BLOCKBYTES ); /* Burn the key from stack */
  }
  أرجع 0;
}

#وصف_ڪريو G(r,i,a,b,c,d)                      \
  افعل {                                      \
    a = a + b + m[blake2b_sigma[r][2 * i + 0]]; \
    d = rotr64(d ^ a, 32);                  \
    c = c + d;                              \
    b = rotr64(b ^ c, 24);                  \
    a = a + b + m[blake2b_sigma[r][2 * i + 1]]; \
    d = rotr64(d ^ a, 16);                  \
    c = c + d;                              \
    b = rotr64(b ^ c, 63);                  \
  } طالما(0)

#وصف_ڪريو ڦيرو(r)                    \
  افعل {                              \
    G(r,0,v[ 0],v[ 4],v[ 8],v[12]); \
    G(r,1,v[ 1],v[ 5],v[ 9],v[13]); \
    G(r,2,v[ 2],v[ 6],v[10],v[14]); \
    G(r,3,v[ 3],v[ 7],v[11],v[15]); \
    G(r,4,v[ 0],v[ 5],v[10],v[15]); \
    G(r,5,v[ 1],v[ 6],v[11],v[12]); \
    G(r,6,v[ 2],v[ 7],v[ 8],v[13]); \
    G(r,7,v[ 3],v[ 4],v[ 9],v[14]); \
  } طالما(0)

ساكن فراغ blake2b_compress( blake2b_state *S, ثابت uint8_t بلاڪ[BLAKE2B_BLOCKBYTES] )
{
  uint64_t m[16];
  uint64_t v[16];
  size_t i;

  لكل( i = 0; i < 16; ++i ) {
    m[i] = load64( بلاڪ + i * حجم( m[i] ) );
  }

  لكل( i = 0; i < 8; ++i ) {
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

  ڦيرو( 0 );
  ڦيرو( 1 );
  ڦيرو( 2 );
  ڦيرو( 3 );
  ڦيرو( 4 );
  ڦيرو( 5 );
  ڦيرو( 6 );
  ڦيرو( 7 );
  ڦيرو( 8 );
  ڦيرو( 9 );
  ڦيرو( 10 );
  ڦيرو( 11 );

  لكل( i = 0; i < 8; ++i ) {
    S->h[i] = S->h[i] ^ v[i] ^ v[i + 8];
  }
}

#وصف_رد G
#وصف_رد ڦيرو

صحيح blake2b_update( blake2b_state *S, ثابت فراغ *pin, size_t inlen )
{
  ثابت بنانشان حرف * اندريون = (ثابت بنانشان حرف *)pin;
  إذا( inlen > 0 )
  {
    size_t کاٻو = S->buflen;
    size_t fill = BLAKE2B_BLOCKBYTES - کاٻو;
    إذا( inlen > fill )
    {
      S->buflen = 0;
      يادگيري_نقل( S->buf + کاٻو, اندريون, fill ); /* Fill buffer */
      blake2b_increment_counter( S, BLAKE2B_BLOCKBYTES );
      blake2b_compress( S, S->buf ); /* Compress */
      اندريون += fill; inlen -= fill;
      طالما(inlen > BLAKE2B_BLOCKBYTES) {
        blake2b_increment_counter(S, BLAKE2B_BLOCKBYTES);
        blake2b_compress( S, اندريون );
        اندريون += BLAKE2B_BLOCKBYTES;
        inlen -= BLAKE2B_BLOCKBYTES;
      }
    }
    يادگيري_نقل( S->buf + S->buflen, اندريون, inlen );
    S->buflen += inlen;
  }
  أرجع 0;
}

صحيح blake2b_final( blake2b_state *S, فراغ *ٻاهريون, size_t outlen )
{
  uint8_t بفر[BLAKE2B_OUTBYTES] = {0};
  size_t i;

  إذا( ٻاهريون == عدم || outlen < S->outlen )
    أرجع -1;

  إذا( blake2b_is_lastblock( S ) )
    أرجع -1;

  blake2b_increment_counter( S, S->buflen );
  blake2b_سيٽ_lastblock( S );
  memset( S->buf + S->buflen, 0, BLAKE2B_BLOCKBYTES - S->buflen ); /* Padding */
  blake2b_compress( S, S->buf );

  لكل( i = 0; i < 8; ++i ) /* Output full hash to temp buffer */
    store64( بفر + حجم( S->h[i] ) * i, S->h[i] );

  يادگيري_نقل( ٻاهريون, بفر, S->outlen );
  secure_zero_يادگيري(بفر, حجم(بفر));
  أرجع 0;
}

/* inlen, at least, should be uint64_t. Others can be size_t. */
صحيح blake2b( فراغ *ٻاهريون, size_t outlen, ثابت فراغ *اندريون, size_t inlen, ثابت فراغ *ڪنجي, size_t keylen )
{
  blake2b_state S[1];

  /* Verify parameters */
  إذا ( عدم == اندريون && inlen > 0 ) أرجع -1;

  إذا ( عدم == ٻاهريون ) أرجع -1;

  إذا( عدم == ڪنجي && keylen > 0 ) أرجع -1;

  إذا( !outlen || outlen > BLAKE2B_OUTBYTES ) أرجع -1;

  إذا( keylen > BLAKE2B_KEYBYTES ) أرجع -1;

  إذا( keylen > 0 )
  {
    إذا( blake2b_init_ڪنجي( S, outlen, ڪنجي, keylen ) < 0 ) أرجع -1;
  }
  وإلا
  {
    إذا( blake2b_init( S, outlen ) < 0 ) أرجع -1;
  }

  blake2b_update( S, ( ثابت uint8_t * )اندريون, inlen );
  blake2b_final( S, ٻاهريون, outlen );
  أرجع 0;
}

صحيح blake2( فراغ *ٻاهريون, size_t outlen, ثابت فراغ *اندريون, size_t inlen, ثابت فراغ *ڪنجي, size_t keylen ) {
  أرجع blake2b(ٻاهريون, outlen, اندريون, inlen, ڪنجي, keylen);
}

#إذا defined(SUPERCOP)
صحيح crypto_hash( بنانشان حرف *ٻاهريون, بنانشان حرف *اندريون, بنانشان ڊگهو ڊگهو inlen )
{
  أرجع blake2b( ٻاهريون, BLAKE2B_OUTBYTES, اندريون, inlen, عدم, 0 );
}
#جيڪڏهن_پورو

#إذا defined(BLAKE2B_SELFTEST)
#ضمن <string.h>
#ضمن "blake2-kat.h"
صحيح مکيه( فراغ )
{
  uint8_t ڪنجي[BLAKE2B_KEYBYTES];
  uint8_t buf[BLAKE2_KAT_ڊيگهائي];
  size_t i, قدم;

  لكل( i = 0; i < BLAKE2B_KEYBYTES; ++i )
    ڪنجي[i] = ( uint8_t )i;

  لكل( i = 0; i < BLAKE2_KAT_ڊيگهائي; ++i )
    buf[i] = ( uint8_t )i;

  /* Test simple API */
  لكل( i = 0; i < BLAKE2_KAT_ڊيگهائي; ++i )
  {
    uint8_t hash[BLAKE2B_OUTBYTES];
    blake2b( hash, BLAKE2B_OUTBYTES, buf, i, ڪنجي, BLAKE2B_KEYBYTES );

    إذا( 0 != memcmp( hash, blake2b_keyed_kat[i], BLAKE2B_OUTBYTES ) )
    {
      اذهب fail;
    }
  }

  /* Test streaming API */
  لكل(قدم = 1; قدم < BLAKE2B_BLOCKBYTES; ++قدم) {
    لكل (i = 0; i < BLAKE2_KAT_ڊيگهائي; ++i) {
      uint8_t hash[BLAKE2B_OUTBYTES];
      blake2b_state S;
      uint8_t * p = buf;
      size_t mlen = i;
      صحيح err = 0;

      إذا( (err = blake2b_init_ڪنجي(&S, BLAKE2B_OUTBYTES, ڪنجي, BLAKE2B_KEYBYTES)) < 0 ) {
        اذهب fail;
      }

      طالما (mlen >= قدم) {
        إذا ( (err = blake2b_update(&S, p, قدم)) < 0 ) {
          اذهب fail;
        }
        mlen -= قدم;
        p += قدم;
      }
      إذا ( (err = blake2b_update(&S, p, mlen)) < 0) {
        اذهب fail;
      }
      إذا ( (err = blake2b_final(&S, hash, BLAKE2B_OUTBYTES)) < 0) {
        اذهب fail;
      }

      إذا (0 != memcmp(hash, blake2b_keyed_kat[i], BLAKE2B_OUTBYTES)) {
        اذهب fail;
      }
    }
  }

  puts( "ok" );
  أرجع 0;
fail:
  puts("error");
  أرجع -1;
}
#جيڪڏهن_پورو
