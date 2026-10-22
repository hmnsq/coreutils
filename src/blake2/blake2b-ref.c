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

#اگر_تعریف_شدہ HAVE_CONFIG_H
# include <config.h>
#اگر_ختم

#شامل <stdint.h>
#شامل <string.h>
#شامل <stdio.h>

#شامل "blake2.h"
#شامل "blake2-impl.h"

ساکن ثابت uint64_t blake2b_IV[8] =
{
  0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL,
  0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL,
  0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL,
  0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL
};

ساکن ثابت uint8_t blake2b_sigma[12][16] =
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


ساکن خالی blake2b_سیٹ_lastnode( blake2b_state *S )
{
  S->f[1] = (uint64_t)-1;
}

/* Some helper functions, not necessarily useful */
ساکن صحیح blake2b_is_lastblock( ثابت blake2b_state *S )
{
  واپس S->f[0] != 0;
}

ساکن خالی blake2b_سیٹ_lastblock( blake2b_state *S )
{
  اگر( S->آخری_گرہ ) blake2b_سیٹ_lastnode( S );

  S->f[0] = (uint64_t)-1;
}

ساکن خالی blake2b_increment_شمار( blake2b_state *S, ثابت uint64_t inc )
{
  S->t[0] += inc;
  S->t[1] += ( S->t[0] < inc );
}

ساکن خالی blake2b_init0( blake2b_state *S )
{
  size_t i;
  memset( S, 0, حجم( blake2b_state ) );

  برائے( i = 0; i < 8; ++i ) S->h[i] = blake2b_IV[i];
}

/* init xors IV with input parameter block */
صحیح blake2b_init_param( blake2b_state *S, ثابت blake2b_param *P )
{
  ثابت uint8_t *p = ( ثابت uint8_t * )( P );
  size_t i;

  blake2b_init0( S );

  /* IV XOR ParamBlock */
  برائے( i = 0; i < 8; ++i )
    S->h[i] ^= load64( p + حجم( S->h[i] ) * i );

  S->outlen = P->digest_لمبائی;
  واپس 0;
}



صحیح blake2b_init( blake2b_state *S, size_t outlen )
{
  blake2b_param P[1];

  اگر ( ( !outlen ) || ( outlen > BLAKE2B_OUTBYTES ) ) واپس -1;

  P->digest_لمبائی = (uint8_t)outlen;
  P->کلید_لمبائی    = 0;
  P->fanout        = 1;
  P->گہرائی         = 1;
  store32( &P->برگ_لمبائی, 0 );
  store32( &P->گرہ_انحراف, 0 );
  store32( &P->xof_لمبائی, 0 );
  P->گرہ_گہرائی    = 0;
  P->inner_لمبائی  = 0;
  memset( P->reserved, 0, حجم( P->reserved ) );
  memset( P->salt,     0, حجم( P->salt ) );
  memset( P->personal, 0, حجم( P->personal ) );
  واپس blake2b_init_param( S, P );
}


صحیح blake2b_init_کلید( blake2b_state *S, size_t outlen, ثابت خالی *کلید, size_t keylen )
{
  blake2b_param P[1];

  اگر ( ( !outlen ) || ( outlen > BLAKE2B_OUTBYTES ) ) واپس -1;

  اگر ( !کلید || !keylen || keylen > BLAKE2B_KEYBYTES ) واپس -1;

  P->digest_لمبائی = (uint8_t)outlen;
  P->کلید_لمبائی    = (uint8_t)keylen;
  P->fanout        = 1;
  P->گہرائی         = 1;
  store32( &P->برگ_لمبائی, 0 );
  store32( &P->گرہ_انحراف, 0 );
  store32( &P->xof_لمبائی, 0 );
  P->گرہ_گہرائی    = 0;
  P->inner_لمبائی  = 0;
  memset( P->reserved, 0, حجم( P->reserved ) );
  memset( P->salt,     0, حجم( P->salt ) );
  memset( P->personal, 0, حجم( P->personal ) );

  اگر( blake2b_init_param( S, P ) < 0 ) واپس -1;

  {
    uint8_t بلاک[BLAKE2B_BLOCKBYTES];
    memset( بلاک, 0, BLAKE2B_BLOCKBYTES );
    حافظہ_نقل( بلاک, کلید, keylen );
    blake2b_update( S, بلاک, BLAKE2B_BLOCKBYTES );
    secure_zero_حافظہ( بلاک, BLAKE2B_BLOCKBYTES ); /* Burn the key from stack */
  }
  واپس 0;
}

#تعریف_کرو G(r,i,a,b,c,d)                      \
  کرو {                                      \
    a = a + b + m[blake2b_sigma[r][2 * i + 0]]; \
    d = rotr64(d ^ a, 32);                  \
    c = c + d;                              \
    b = rotr64(b ^ c, 24);                  \
    a = a + b + m[blake2b_sigma[r][2 * i + 1]]; \
    d = rotr64(d ^ a, 16);                  \
    c = c + d;                              \
    b = rotr64(b ^ c, 63);                  \
  } جب(0)

#تعریف_کرو باری(r)                    \
  کرو {                              \
    G(r,0,v[ 0],v[ 4],v[ 8],v[12]); \
    G(r,1,v[ 1],v[ 5],v[ 9],v[13]); \
    G(r,2,v[ 2],v[ 6],v[10],v[14]); \
    G(r,3,v[ 3],v[ 7],v[11],v[15]); \
    G(r,4,v[ 0],v[ 5],v[10],v[15]); \
    G(r,5,v[ 1],v[ 6],v[11],v[12]); \
    G(r,6,v[ 2],v[ 7],v[ 8],v[13]); \
    G(r,7,v[ 3],v[ 4],v[ 9],v[14]); \
  } جب(0)

ساکن خالی blake2b_compress( blake2b_state *S, ثابت uint8_t بلاک[BLAKE2B_BLOCKBYTES] )
{
  uint64_t m[16];
  uint64_t v[16];
  size_t i;

  برائے( i = 0; i < 16; ++i ) {
    m[i] = load64( بلاک + i * حجم( m[i] ) );
  }

  برائے( i = 0; i < 8; ++i ) {
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

  باری( 0 );
  باری( 1 );
  باری( 2 );
  باری( 3 );
  باری( 4 );
  باری( 5 );
  باری( 6 );
  باری( 7 );
  باری( 8 );
  باری( 9 );
  باری( 10 );
  باری( 11 );

  برائے( i = 0; i < 8; ++i ) {
    S->h[i] = S->h[i] ^ v[i] ^ v[i + 8];
  }
}

#تعریف_منسوخ G
#تعریف_منسوخ باری

صحیح blake2b_update( blake2b_state *S, ثابت خالی *pin, size_t inlen )
{
  ثابت غیرعلامتی حرف * اندر = (ثابت غیرعلامتی حرف *)pin;
  اگر( inlen > 0 )
  {
    size_t بایاں = S->buflen;
    size_t fill = BLAKE2B_BLOCKBYTES - بایاں;
    اگر( inlen > fill )
    {
      S->buflen = 0;
      حافظہ_نقل( S->buf + بایاں, اندر, fill ); /* Fill buffer */
      blake2b_increment_شمار( S, BLAKE2B_BLOCKBYTES );
      blake2b_compress( S, S->buf ); /* Compress */
      اندر += fill; inlen -= fill;
      جب(inlen > BLAKE2B_BLOCKBYTES) {
        blake2b_increment_شمار(S, BLAKE2B_BLOCKBYTES);
        blake2b_compress( S, اندر );
        اندر += BLAKE2B_BLOCKBYTES;
        inlen -= BLAKE2B_BLOCKBYTES;
      }
    }
    حافظہ_نقل( S->buf + S->buflen, اندر, inlen );
    S->buflen += inlen;
  }
  واپس 0;
}

صحیح blake2b_final( blake2b_state *S, خالی *برآمد, size_t outlen )
{
  uint8_t بفر[BLAKE2B_OUTBYTES] = {0};
  size_t i;

  اگر( برآمد == تہی || outlen < S->outlen )
    واپس -1;

  اگر( blake2b_is_lastblock( S ) )
    واپس -1;

  blake2b_increment_شمار( S, S->buflen );
  blake2b_سیٹ_lastblock( S );
  memset( S->buf + S->buflen, 0, BLAKE2B_BLOCKBYTES - S->buflen ); /* Padding */
  blake2b_compress( S, S->buf );

  برائے( i = 0; i < 8; ++i ) /* Output full hash to temp buffer */
    store64( بفر + حجم( S->h[i] ) * i, S->h[i] );

  حافظہ_نقل( برآمد, بفر, S->outlen );
  secure_zero_حافظہ(بفر, حجم(بفر));
  واپس 0;
}

/* inlen, at least, should be uint64_t. Others can be size_t. */
صحیح blake2b( خالی *برآمد, size_t outlen, ثابت خالی *اندر, size_t inlen, ثابت خالی *کلید, size_t keylen )
{
  blake2b_state S[1];

  /* Verify parameters */
  اگر ( تہی == اندر && inlen > 0 ) واپس -1;

  اگر ( تہی == برآمد ) واپس -1;

  اگر( تہی == کلید && keylen > 0 ) واپس -1;

  اگر( !outlen || outlen > BLAKE2B_OUTBYTES ) واپس -1;

  اگر( keylen > BLAKE2B_KEYBYTES ) واپس -1;

  اگر( keylen > 0 )
  {
    اگر( blake2b_init_کلید( S, outlen, کلید, keylen ) < 0 ) واپس -1;
  }
  ورنہ
  {
    اگر( blake2b_init( S, outlen ) < 0 ) واپس -1;
  }

  blake2b_update( S, ( ثابت uint8_t * )اندر, inlen );
  blake2b_final( S, برآمد, outlen );
  واپس 0;
}

صحیح blake2( خالی *برآمد, size_t outlen, ثابت خالی *اندر, size_t inlen, ثابت خالی *کلید, size_t keylen ) {
  واپس blake2b(برآمد, outlen, اندر, inlen, کلید, keylen);
}

#اگر defined(SUPERCOP)
صحیح crypto_hash( غیرعلامتی حرف *برآمد, غیرعلامتی حرف *اندر, غیرعلامتی طویل طویل inlen )
{
  واپس blake2b( برآمد, BLAKE2B_OUTBYTES, اندر, inlen, تہی, 0 );
}
#اگر_ختم

#اگر defined(BLAKE2B_SELFTEST)
#شامل <string.h>
#شامل "blake2-kat.h"
صحیح مین( خالی )
{
  uint8_t کلید[BLAKE2B_KEYBYTES];
  uint8_t buf[BLAKE2_KAT_لمبائی];
  size_t i, قدم;

  برائے( i = 0; i < BLAKE2B_KEYBYTES; ++i )
    کلید[i] = ( uint8_t )i;

  برائے( i = 0; i < BLAKE2_KAT_لمبائی; ++i )
    buf[i] = ( uint8_t )i;

  /* Test simple API */
  برائے( i = 0; i < BLAKE2_KAT_لمبائی; ++i )
  {
    uint8_t hash[BLAKE2B_OUTBYTES];
    blake2b( hash, BLAKE2B_OUTBYTES, buf, i, کلید, BLAKE2B_KEYBYTES );

    اگر( 0 != memcmp( hash, blake2b_keyed_kat[i], BLAKE2B_OUTBYTES ) )
    {
      جاؤ fail;
    }
  }

  /* Test streaming API */
  برائے(قدم = 1; قدم < BLAKE2B_BLOCKBYTES; ++قدم) {
    برائے (i = 0; i < BLAKE2_KAT_لمبائی; ++i) {
      uint8_t hash[BLAKE2B_OUTBYTES];
      blake2b_state S;
      uint8_t * p = buf;
      size_t mlen = i;
      صحیح err = 0;

      اگر( (err = blake2b_init_کلید(&S, BLAKE2B_OUTBYTES, کلید, BLAKE2B_KEYBYTES)) < 0 ) {
        جاؤ fail;
      }

      جب (mlen >= قدم) {
        اگر ( (err = blake2b_update(&S, p, قدم)) < 0 ) {
          جاؤ fail;
        }
        mlen -= قدم;
        p += قدم;
      }
      اگر ( (err = blake2b_update(&S, p, mlen)) < 0) {
        جاؤ fail;
      }
      اگر ( (err = blake2b_final(&S, hash, BLAKE2B_OUTBYTES)) < 0) {
        جاؤ fail;
      }

      اگر (0 != memcmp(hash, blake2b_keyed_kat[i], BLAKE2B_OUTBYTES)) {
        جاؤ fail;
      }
    }
  }

  puts( "ok" );
  واپس 0;
fail:
  puts("error");
  واپس -1;
}
#اگر_ختم
