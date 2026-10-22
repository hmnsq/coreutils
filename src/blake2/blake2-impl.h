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
#నిర్వచితం_కాకపోతే BLAKE2_IMPL_H
#నిర్వచించు BLAKE2_IMPL_H

#అన్తర్భావ <endian.h>
#ఒకవేళ BYTE_ORDER == LITTLE_ENDIAN
# define NATIVE_LITTLE_ENDIAN 1
#అయితే_ముగింపు

#అన్తర్భావ <stdint.h>
#అన్తర్భావ <string.h>

#ఒకవేళ !defined(__cplusplus) && (!defined(__STDC_VERSION__) || __STDC_VERSION__ < 199901L)
  #ఒకవేళ   defined(_MSC_VER)
    #నిర్వచించు BLAKE2_INLINE __inline
  #లేదావేళ defined(__GNUC__)
    #నిర్వచించు BLAKE2_INLINE __inline__
  #లేకపోతే
    #నిర్వచించు BLAKE2_INLINE
  #అయితే_ముగింపు
#లేకపోతే
  #నిర్వచించు BLAKE2_INLINE అంతఃవరుస
#అయితే_ముగింపు

స్థిర BLAKE2_INLINE uint32_t load32( స్థిరరాశి శూన్య *src )
{
#ఒకవేళ defined(NATIVE_LITTLE_ENDIAN)
  uint32_t w;
  జ్ఞాపకం_నకలు(&w, src, పరిమాణం w);
  తిరిగి w;
#లేకపోతే
  స్థిరరాశి uint8_t *p = ( స్థిరరాశి uint8_t * )src;
  తిరిగి (( uint32_t )( p[0] ) <<  0) |
         (( uint32_t )( p[1] ) <<  8) |
         (( uint32_t )( p[2] ) << 16) |
         (( uint32_t )( p[3] ) << 24) ;
#అయితే_ముగింపు
}

స్థిర BLAKE2_INLINE uint64_t load64( స్థిరరాశి శూన్య *src )
{
#ఒకవేళ defined(NATIVE_LITTLE_ENDIAN)
  uint64_t w;
  జ్ఞాపకం_నకలు(&w, src, పరిమాణం w);
  తిరిగి w;
#లేకపోతే
  స్థిరరాశి uint8_t *p = ( స్థిరరాశి uint8_t * )src;
  తిరిగి (( uint64_t )( p[0] ) <<  0) |
         (( uint64_t )( p[1] ) <<  8) |
         (( uint64_t )( p[2] ) << 16) |
         (( uint64_t )( p[3] ) << 24) |
         (( uint64_t )( p[4] ) << 32) |
         (( uint64_t )( p[5] ) << 40) |
         (( uint64_t )( p[6] ) << 48) |
         (( uint64_t )( p[7] ) << 56) ;
#అయితే_ముగింపు
}

స్థిర BLAKE2_INLINE uint16_t load16( స్థిరరాశి శూన్య *src )
{
#ఒకవేళ defined(NATIVE_LITTLE_ENDIAN)
  uint16_t w;
  జ్ఞాపకం_నకలు(&w, src, పరిమాణం w);
  తిరిగి w;
#లేకపోతే
  స్థిరరాశి uint8_t *p = ( స్థిరరాశి uint8_t * )src;
  తిరిగి ( uint16_t )((( uint32_t )( p[0] ) <<  0) |
                      (( uint32_t )( p[1] ) <<  8));
#అయితే_ముగింపు
}

స్థిర BLAKE2_INLINE శూన్య store16( శూన్య *dst, uint16_t w )
{
#ఒకవేళ defined(NATIVE_LITTLE_ENDIAN)
  జ్ఞాపకం_నకలు(dst, &w, పరిమాణం w);
#లేకపోతే
  uint8_t *p = ( uint8_t * )dst;
  *p++ = ( uint8_t )w; w >>= 8;
  *p++ = ( uint8_t )w;
#అయితే_ముగింపు
}

స్థిర BLAKE2_INLINE శూన్య store32( శూన్య *dst, uint32_t w )
{
#ఒకవేళ defined(NATIVE_LITTLE_ENDIAN)
  జ్ఞాపకం_నకలు(dst, &w, పరిమాణం w);
#లేకపోతే
  uint8_t *p = ( uint8_t * )dst;
  p[0] = (uint8_t)(w >>  0);
  p[1] = (uint8_t)(w >>  8);
  p[2] = (uint8_t)(w >> 16);
  p[3] = (uint8_t)(w >> 24);
#అయితే_ముగింపు
}

స్థిర BLAKE2_INLINE శూన్య store64( శూన్య *dst, uint64_t w )
{
#ఒకవేళ defined(NATIVE_LITTLE_ENDIAN)
  జ్ఞాపకం_నకలు(dst, &w, పరిమాణం w);
#లేకపోతే
  uint8_t *p = ( uint8_t * )dst;
  p[0] = (uint8_t)(w >>  0);
  p[1] = (uint8_t)(w >>  8);
  p[2] = (uint8_t)(w >> 16);
  p[3] = (uint8_t)(w >> 24);
  p[4] = (uint8_t)(w >> 32);
  p[5] = (uint8_t)(w >> 40);
  p[6] = (uint8_t)(w >> 48);
  p[7] = (uint8_t)(w >> 56);
#అయితే_ముగింపు
}

స్థిర BLAKE2_INLINE uint64_t load48( స్థిరరాశి శూన్య *src )
{
  స్థిరరాశి uint8_t *p = ( స్థిరరాశి uint8_t * )src;
  తిరిగి (( uint64_t )( p[0] ) <<  0) |
         (( uint64_t )( p[1] ) <<  8) |
         (( uint64_t )( p[2] ) << 16) |
         (( uint64_t )( p[3] ) << 24) |
         (( uint64_t )( p[4] ) << 32) |
         (( uint64_t )( p[5] ) << 40) ;
}

స్థిర BLAKE2_INLINE శూన్య store48( శూన్య *dst, uint64_t w )
{
  uint8_t *p = ( uint8_t * )dst;
  p[0] = (uint8_t)(w >>  0);
  p[1] = (uint8_t)(w >>  8);
  p[2] = (uint8_t)(w >> 16);
  p[3] = (uint8_t)(w >> 24);
  p[4] = (uint8_t)(w >> 32);
  p[5] = (uint8_t)(w >> 40);
}

స్థిర BLAKE2_INLINE uint32_t rotr32( స్థిరరాశి uint32_t w, స్థిరరాశి గుర్తులేని c )
{
  తిరిగి ( w >> c ) | ( w << ( 32 - c ) );
}

స్థిర BLAKE2_INLINE uint64_t rotr64( స్థిరరాశి uint64_t w, స్థిరరాశి గుర్తులేని c )
{
  తిరిగి ( w >> c ) | ( w << ( 64 - c ) );
}

/* prevents compiler optimizing out memset() */
స్థిర BLAKE2_INLINE శూన్య secure_zero_జ్ఞాపకం(శూన్య *v, size_t n)
{
  స్థిర శూన్య *(*స్థిరరాశి అస్థిర memset_v)(శూన్య *, పూర్ణాంకం, size_t) = &memset;
  memset_v(v, 0, n);
}

#అయితే_ముగింపు
