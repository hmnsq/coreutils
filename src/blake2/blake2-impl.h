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
#ifndef BLAKE2_IMPL_H
#define BLAKE2_IMPL_H

#ଅନ୍ତର୍ଭାଵ <endian.h>
#ଯଦି BYTE_ORDER == LITTLE_ENDIAN
# define NATIVE_LITTLE_ENDIAN 1
#endif

#ଅନ୍ତର୍ଭାଵ <stdint.h>
#ଅନ୍ତର୍ଭାଵ <string.h>

#ଯଦି !defined(__cplusplus) && (!defined(__STDC_VERSION__) || __STDC_VERSION__ < 199901L)
  #ଯଦି   defined(_MSC_VER)
    #define BLAKE2_INLINE __inline
  #ନଚେତଯଦି defined(__GNUC__)
    #define BLAKE2_INLINE __inline__
  #ଅନ୍ୟଥା
    #define BLAKE2_INLINE
  #endif
#ଅନ୍ୟଥା
  #define BLAKE2_INLINE ଧାଡ଼ିଭିତରେ
#endif

ସ୍ଥିର BLAKE2_INLINE uint32_t load32( ଅଚଳ ଶୂନ୍ୟ *src )
{
#ଯଦି defined(NATIVE_LITTLE_ENDIAN)
  uint32_t w;
  memcpy(&w, src, ଆକାର w);
  ଫେରନ୍ତୁ w;
#ଅନ୍ୟଥା
  ଅଚଳ uint8_t *p = ( ଅଚଳ uint8_t * )src;
  ଫେରନ୍ତୁ (( uint32_t )( p[0] ) <<  0) |
         (( uint32_t )( p[1] ) <<  8) |
         (( uint32_t )( p[2] ) << 16) |
         (( uint32_t )( p[3] ) << 24) ;
#endif
}

ସ୍ଥିର BLAKE2_INLINE uint64_t load64( ଅଚଳ ଶୂନ୍ୟ *src )
{
#ଯଦି defined(NATIVE_LITTLE_ENDIAN)
  uint64_t w;
  memcpy(&w, src, ଆକାର w);
  ଫେରନ୍ତୁ w;
#ଅନ୍ୟଥା
  ଅଚଳ uint8_t *p = ( ଅଚଳ uint8_t * )src;
  ଫେରନ୍ତୁ (( uint64_t )( p[0] ) <<  0) |
         (( uint64_t )( p[1] ) <<  8) |
         (( uint64_t )( p[2] ) << 16) |
         (( uint64_t )( p[3] ) << 24) |
         (( uint64_t )( p[4] ) << 32) |
         (( uint64_t )( p[5] ) << 40) |
         (( uint64_t )( p[6] ) << 48) |
         (( uint64_t )( p[7] ) << 56) ;
#endif
}

ସ୍ଥିର BLAKE2_INLINE uint16_t load16( ଅଚଳ ଶୂନ୍ୟ *src )
{
#ଯଦି defined(NATIVE_LITTLE_ENDIAN)
  uint16_t w;
  memcpy(&w, src, ଆକାର w);
  ଫେରନ୍ତୁ w;
#ଅନ୍ୟଥା
  ଅଚଳ uint8_t *p = ( ଅଚଳ uint8_t * )src;
  ଫେରନ୍ତୁ ( uint16_t )((( uint32_t )( p[0] ) <<  0) |
                      (( uint32_t )( p[1] ) <<  8));
#endif
}

ସ୍ଥିର BLAKE2_INLINE ଶୂନ୍ୟ store16( ଶୂନ୍ୟ *dst, uint16_t w )
{
#ଯଦି defined(NATIVE_LITTLE_ENDIAN)
  memcpy(dst, &w, ଆକାର w);
#ଅନ୍ୟଥା
  uint8_t *p = ( uint8_t * )dst;
  *p++ = ( uint8_t )w; w >>= 8;
  *p++ = ( uint8_t )w;
#endif
}

ସ୍ଥିର BLAKE2_INLINE ଶୂନ୍ୟ store32( ଶୂନ୍ୟ *dst, uint32_t w )
{
#ଯଦି defined(NATIVE_LITTLE_ENDIAN)
  memcpy(dst, &w, ଆକାର w);
#ଅନ୍ୟଥା
  uint8_t *p = ( uint8_t * )dst;
  p[0] = (uint8_t)(w >>  0);
  p[1] = (uint8_t)(w >>  8);
  p[2] = (uint8_t)(w >> 16);
  p[3] = (uint8_t)(w >> 24);
#endif
}

ସ୍ଥିର BLAKE2_INLINE ଶୂନ୍ୟ store64( ଶୂନ୍ୟ *dst, uint64_t w )
{
#ଯଦି defined(NATIVE_LITTLE_ENDIAN)
  memcpy(dst, &w, ଆକାର w);
#ଅନ୍ୟଥା
  uint8_t *p = ( uint8_t * )dst;
  p[0] = (uint8_t)(w >>  0);
  p[1] = (uint8_t)(w >>  8);
  p[2] = (uint8_t)(w >> 16);
  p[3] = (uint8_t)(w >> 24);
  p[4] = (uint8_t)(w >> 32);
  p[5] = (uint8_t)(w >> 40);
  p[6] = (uint8_t)(w >> 48);
  p[7] = (uint8_t)(w >> 56);
#endif
}

ସ୍ଥିର BLAKE2_INLINE uint64_t load48( ଅଚଳ ଶୂନ୍ୟ *src )
{
  ଅଚଳ uint8_t *p = ( ଅଚଳ uint8_t * )src;
  ଫେରନ୍ତୁ (( uint64_t )( p[0] ) <<  0) |
         (( uint64_t )( p[1] ) <<  8) |
         (( uint64_t )( p[2] ) << 16) |
         (( uint64_t )( p[3] ) << 24) |
         (( uint64_t )( p[4] ) << 32) |
         (( uint64_t )( p[5] ) << 40) ;
}

ସ୍ଥିର BLAKE2_INLINE ଶୂନ୍ୟ store48( ଶୂନ୍ୟ *dst, uint64_t w )
{
  uint8_t *p = ( uint8_t * )dst;
  p[0] = (uint8_t)(w >>  0);
  p[1] = (uint8_t)(w >>  8);
  p[2] = (uint8_t)(w >> 16);
  p[3] = (uint8_t)(w >> 24);
  p[4] = (uint8_t)(w >> 32);
  p[5] = (uint8_t)(w >> 40);
}

ସ୍ଥିର BLAKE2_INLINE uint32_t rotr32( ଅଚଳ uint32_t w, ଅଚଳ unsigned c )
{
  ଫେରନ୍ତୁ ( w >> c ) | ( w << ( 32 - c ) );
}

ସ୍ଥିର BLAKE2_INLINE uint64_t rotr64( ଅଚଳ uint64_t w, ଅଚଳ unsigned c )
{
  ଫେରନ୍ତୁ ( w >> c ) | ( w << ( 64 - c ) );
}

/* prevents compiler optimizing out memset() */
ସ୍ଥିର BLAKE2_INLINE ଶୂନ୍ୟ secure_zero_memory(ଶୂନ୍ୟ *v, size_t n)
{
  ସ୍ଥିର ଶୂନ୍ୟ *(*ଅଚଳ ଅସ୍ଥିର memset_v)(ଶୂନ୍ୟ *, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, size_t) = &memset;
  memset_v(v, 0, n);
}

#endif
