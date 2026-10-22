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

#यदि_परिभाषितम् HAVE_CONFIG_H
# include <config.h>
#यदि_अन्तः

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


स्थिर शून्य blake2b_समुच्चयः_lastnode( blake2b_state *S )
{
  S->f[1] = (uint64_t)-1;
}

/* Some helper functions, not necessarily useful */
स्थिर पूर्णाङ्क blake2b_is_lastblock( ध्रुव blake2b_state *S )
{
  प्रत्यागम S->f[0] != 0;
}

स्थिर शून्य blake2b_समुच्चयः_lastblock( blake2b_state *S )
{
  यदि( S->चरमः_ग्रन्थिः ) blake2b_समुच्चयः_lastnode( S );

  S->f[0] = (uint64_t)-1;
}

स्थिर शून्य blake2b_increment_गणक( blake2b_state *S, ध्रुव uint64_t inc )
{
  S->t[0] += inc;
  S->t[1] += ( S->t[0] < inc );
}

स्थिर शून्य blake2b_init0( blake2b_state *S )
{
  size_t i;
  स्मृतिं_पूरय( S, 0, परिमाण( blake2b_state ) );

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

  S->outlen = P->digest_दीर्घता;
  प्रत्यागम 0;
}



पूर्णाङ्क blake2b_init( blake2b_state *S, size_t outlen )
{
  blake2b_param P[1];

  यदि ( ( !outlen ) || ( outlen > BLAKE2B_OUTBYTES ) ) प्रत्यागम -1;

  P->digest_दीर्घता = (uint8_t)outlen;
  P->कुञ्चिका_दीर्घता    = 0;
  P->fanout        = 1;
  P->गभीरता         = 1;
  store32( &P->पर्णम्_दीर्घता, 0 );
  store32( &P->ग्रन्थिः_विस्थापनम्, 0 );
  store32( &P->xof_दीर्घता, 0 );
  P->ग्रन्थिः_गभीरता    = 0;
  P->inner_दीर्घता  = 0;
  स्मृतिं_पूरय( P->reserved, 0, परिमाण( P->reserved ) );
  स्मृतिं_पूरय( P->salt,     0, परिमाण( P->salt ) );
  स्मृतिं_पूरय( P->personal, 0, परिमाण( P->personal ) );
  प्रत्यागम blake2b_init_param( S, P );
}


पूर्णाङ्क blake2b_init_कुञ्चिका( blake2b_state *S, size_t outlen, ध्रुव शून्य *कुञ्चिका, size_t keylen )
{
  blake2b_param P[1];

  यदि ( ( !outlen ) || ( outlen > BLAKE2B_OUTBYTES ) ) प्रत्यागम -1;

  यदि ( !कुञ्चिका || !keylen || keylen > BLAKE2B_KEYBYTES ) प्रत्यागम -1;

  P->digest_दीर्घता = (uint8_t)outlen;
  P->कुञ्चिका_दीर्घता    = (uint8_t)keylen;
  P->fanout        = 1;
  P->गभीरता         = 1;
  store32( &P->पर्णम्_दीर्घता, 0 );
  store32( &P->ग्रन्थिः_विस्थापनम्, 0 );
  store32( &P->xof_दीर्घता, 0 );
  P->ग्रन्थिः_गभीरता    = 0;
  P->inner_दीर्घता  = 0;
  स्मृतिं_पूरय( P->reserved, 0, परिमाण( P->reserved ) );
  स्मृतिं_पूरय( P->salt,     0, परिमाण( P->salt ) );
  स्मृतिं_पूरय( P->personal, 0, परिमाण( P->personal ) );

  यदि( blake2b_init_param( S, P ) < 0 ) प्रत्यागम -1;

  {
    uint8_t खण्डम्[BLAKE2B_BLOCKBYTES];
    स्मृतिं_पूरय( खण्डम्, 0, BLAKE2B_BLOCKBYTES );
    स्मृतिं_प्रतिलिख( खण्डम्, कुञ्चिका, keylen );
    blake2b_update( S, खण्डम्, BLAKE2B_BLOCKBYTES );
    secure_zero_स्मृतिः( खण्डम्, BLAKE2B_BLOCKBYTES ); /* Burn the key from stack */
  }
  प्रत्यागम 0;
}

#परिभाषय G(r,i,a,b,c,d)                      \
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

#परिभाषय आवर्तनम्(r)                    \
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

स्थिर शून्य blake2b_compress( blake2b_state *S, ध्रुव uint8_t खण्डम्[BLAKE2B_BLOCKBYTES] )
{
  uint64_t m[16];
  uint64_t v[16];
  size_t i;

  प्रति( i = 0; i < 16; ++i ) {
    m[i] = load64( खण्डम् + i * परिमाण( m[i] ) );
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

  आवर्तनम्( 0 );
  आवर्तनम्( 1 );
  आवर्तनम्( 2 );
  आवर्तनम्( 3 );
  आवर्तनम्( 4 );
  आवर्तनम्( 5 );
  आवर्तनम्( 6 );
  आवर्तनम्( 7 );
  आवर्तनम्( 8 );
  आवर्तनम्( 9 );
  आवर्तनम्( 10 );
  आवर्तनम्( 11 );

  प्रति( i = 0; i < 8; ++i ) {
    S->h[i] = S->h[i] ^ v[i] ^ v[i + 8];
  }
}

#अपरिभाषय G
#अपरिभाषय आवर्तनम्

पूर्णाङ्क blake2b_update( blake2b_state *S, ध्रुव शून्य *pin, size_t inlen )
{
  ध्रुव अचिह्नित वर्ण * अन्तर् = (ध्रुव अचिह्नित वर्ण *)pin;
  यदि( inlen > 0 )
  {
    size_t वामः = S->buflen;
    size_t fill = BLAKE2B_BLOCKBYTES - वामः;
    यदि( inlen > fill )
    {
      S->buflen = 0;
      स्मृतिं_प्रतिलिख( S->buf + वामः, अन्तर्, fill ); /* Fill buffer */
      blake2b_increment_गणक( S, BLAKE2B_BLOCKBYTES );
      blake2b_compress( S, S->buf ); /* Compress */
      अन्तर् += fill; inlen -= fill;
      यावत्(inlen > BLAKE2B_BLOCKBYTES) {
        blake2b_increment_गणक(S, BLAKE2B_BLOCKBYTES);
        blake2b_compress( S, अन्तर् );
        अन्तर् += BLAKE2B_BLOCKBYTES;
        inlen -= BLAKE2B_BLOCKBYTES;
      }
    }
    स्मृतिं_प्रतिलिख( S->buf + S->buflen, अन्तर्, inlen );
    S->buflen += inlen;
  }
  प्रत्यागम 0;
}

पूर्णाङ्क blake2b_final( blake2b_state *S, शून्य *बहिर्गमनम्, size_t outlen )
{
  uint8_t बफर[BLAKE2B_OUTBYTES] = {0};
  size_t i;

  यदि( बहिर्गमनम् == रिक्त || outlen < S->outlen )
    प्रत्यागम -1;

  यदि( blake2b_is_lastblock( S ) )
    प्रत्यागम -1;

  blake2b_increment_गणक( S, S->buflen );
  blake2b_समुच्चयः_lastblock( S );
  स्मृतिं_पूरय( S->buf + S->buflen, 0, BLAKE2B_BLOCKBYTES - S->buflen ); /* Padding */
  blake2b_compress( S, S->buf );

  प्रति( i = 0; i < 8; ++i ) /* Output full hash to temp buffer */
    store64( बफर + परिमाण( S->h[i] ) * i, S->h[i] );

  स्मृतिं_प्रतिलिख( बहिर्गमनम्, बफर, S->outlen );
  secure_zero_स्मृतिः(बफर, परिमाण(बफर));
  प्रत्यागम 0;
}

/* inlen, at least, should be uint64_t. Others can be size_t. */
पूर्णाङ्क blake2b( शून्य *बहिर्गमनम्, size_t outlen, ध्रुव शून्य *अन्तर्, size_t inlen, ध्रुव शून्य *कुञ्चिका, size_t keylen )
{
  blake2b_state S[1];

  /* Verify parameters */
  यदि ( रिक्त == अन्तर् && inlen > 0 ) प्रत्यागम -1;

  यदि ( रिक्त == बहिर्गमनम् ) प्रत्यागम -1;

  यदि( रिक्त == कुञ्चिका && keylen > 0 ) प्रत्यागम -1;

  यदि( !outlen || outlen > BLAKE2B_OUTBYTES ) प्रत्यागम -1;

  यदि( keylen > BLAKE2B_KEYBYTES ) प्रत्यागम -1;

  यदि( keylen > 0 )
  {
    यदि( blake2b_init_कुञ्चिका( S, outlen, कुञ्चिका, keylen ) < 0 ) प्रत्यागम -1;
  }
  अन्यथा
  {
    यदि( blake2b_init( S, outlen ) < 0 ) प्रत्यागम -1;
  }

  blake2b_update( S, ( ध्रुव uint8_t * )अन्तर्, inlen );
  blake2b_final( S, बहिर्गमनम्, outlen );
  प्रत्यागम 0;
}

पूर्णाङ्क blake2( शून्य *बहिर्गमनम्, size_t outlen, ध्रुव शून्य *अन्तर्, size_t inlen, ध्रुव शून्य *कुञ्चिका, size_t keylen ) {
  प्रत्यागम blake2b(बहिर्गमनम्, outlen, अन्तर्, inlen, कुञ्चिका, keylen);
}

#यदि defined(SUPERCOP)
पूर्णाङ्क crypto_hash( अचिह्नित वर्ण *बहिर्गमनम्, अचिह्नित वर्ण *अन्तर्, अचिह्नित दीर्घ दीर्घ inlen )
{
  प्रत्यागम blake2b( बहिर्गमनम्, BLAKE2B_OUTBYTES, अन्तर्, inlen, रिक्त, 0 );
}
#यदि_अन्तः

#यदि defined(BLAKE2B_SELFTEST)
#अन्तर्भाव <string.h>
#अन्तर्भाव "blake2-kat.h"
पूर्णाङ्क मुख्यम्( शून्य )
{
  uint8_t कुञ्चिका[BLAKE2B_KEYBYTES];
  uint8_t buf[BLAKE2_KAT_दीर्घता];
  size_t i, चरणः;

  प्रति( i = 0; i < BLAKE2B_KEYBYTES; ++i )
    कुञ्चिका[i] = ( uint8_t )i;

  प्रति( i = 0; i < BLAKE2_KAT_दीर्घता; ++i )
    buf[i] = ( uint8_t )i;

  /* Test simple API */
  प्रति( i = 0; i < BLAKE2_KAT_दीर्घता; ++i )
  {
    uint8_t hash[BLAKE2B_OUTBYTES];
    blake2b( hash, BLAKE2B_OUTBYTES, buf, i, कुञ्चिका, BLAKE2B_KEYBYTES );

    यदि( 0 != memcmp( hash, blake2b_keyed_kat[i], BLAKE2B_OUTBYTES ) )
    {
      गच्छ fail;
    }
  }

  /* Test streaming API */
  प्रति(चरणः = 1; चरणः < BLAKE2B_BLOCKBYTES; ++चरणः) {
    प्रति (i = 0; i < BLAKE2_KAT_दीर्घता; ++i) {
      uint8_t hash[BLAKE2B_OUTBYTES];
      blake2b_state S;
      uint8_t * p = buf;
      size_t mlen = i;
      पूर्णाङ्क err = 0;

      यदि( (err = blake2b_init_कुञ्चिका(&S, BLAKE2B_OUTBYTES, कुञ्चिका, BLAKE2B_KEYBYTES)) < 0 ) {
        गच्छ fail;
      }

      यावत् (mlen >= चरणः) {
        यदि ( (err = blake2b_update(&S, p, चरणः)) < 0 ) {
          गच्छ fail;
        }
        mlen -= चरणः;
        p += चरणः;
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

  पङ्क्तिं_लिख( "ok" );
  प्रत्यागम 0;
fail:
  पङ्क्तिं_लिख("error");
  प्रत्यागम -1;
}
#यदि_अन्तः
