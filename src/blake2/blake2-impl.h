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

#अन्तर्भाव <endian.h>
#जइ BYTE_ORDER == LITTLE_ENDIAN
# define NATIVE_LITTLE_ENDIAN 1
#endif

#अन्तर्भाव <stdint.h>
#अन्तर्भाव <string.h>

#जइ !defined(__cplusplus) && (!defined(__STDC_VERSION__) || __STDC_VERSION__ < 199901L)
  #जइ   defined(_MSC_VER)
    #define BLAKE2_INLINE __inline
  #नोचेद्यदि defined(__GNUC__)
    #define BLAKE2_INLINE __inline__
  #अण्णहा
    #define BLAKE2_INLINE
  #endif
#अण्णहा
  #define BLAKE2_INLINE अन्तःस्थ
#endif

स्थिर BLAKE2_INLINE uint32_t load32( ध्रुव शून्य *src )
{
#जइ defined(NATIVE_LITTLE_ENDIAN)
  uint32_t w;
  memcpy(&w, src, परिमाण w);
  प्रत्यागम w;
#अण्णहा
  ध्रुव uint8_t *p = ( ध्रुव uint8_t * )src;
  प्रत्यागम (( uint32_t )( p[0] ) <<  0) |
         (( uint32_t )( p[1] ) <<  8) |
         (( uint32_t )( p[2] ) << 16) |
         (( uint32_t )( p[3] ) << 24) ;
#endif
}

स्थिर BLAKE2_INLINE uint64_t load64( ध्रुव शून्य *src )
{
#जइ defined(NATIVE_LITTLE_ENDIAN)
  uint64_t w;
  memcpy(&w, src, परिमाण w);
  प्रत्यागम w;
#अण्णहा
  ध्रुव uint8_t *p = ( ध्रुव uint8_t * )src;
  प्रत्यागम (( uint64_t )( p[0] ) <<  0) |
         (( uint64_t )( p[1] ) <<  8) |
         (( uint64_t )( p[2] ) << 16) |
         (( uint64_t )( p[3] ) << 24) |
         (( uint64_t )( p[4] ) << 32) |
         (( uint64_t )( p[5] ) << 40) |
         (( uint64_t )( p[6] ) << 48) |
         (( uint64_t )( p[7] ) << 56) ;
#endif
}

स्थिर BLAKE2_INLINE uint16_t load16( ध्रुव शून्य *src )
{
#जइ defined(NATIVE_LITTLE_ENDIAN)
  uint16_t w;
  memcpy(&w, src, परिमाण w);
  प्रत्यागम w;
#अण्णहा
  ध्रुव uint8_t *p = ( ध्रुव uint8_t * )src;
  प्रत्यागम ( uint16_t )((( uint32_t )( p[0] ) <<  0) |
                      (( uint32_t )( p[1] ) <<  8));
#endif
}

स्थिर BLAKE2_INLINE शून्य store16( शून्य *dst, uint16_t w )
{
#जइ defined(NATIVE_LITTLE_ENDIAN)
  memcpy(dst, &w, परिमाण w);
#अण्णहा
  uint8_t *p = ( uint8_t * )dst;
  *p++ = ( uint8_t )w; w >>= 8;
  *p++ = ( uint8_t )w;
#endif
}

स्थिर BLAKE2_INLINE शून्य store32( शून्य *dst, uint32_t w )
{
#जइ defined(NATIVE_LITTLE_ENDIAN)
  memcpy(dst, &w, परिमाण w);
#अण्णहा
  uint8_t *p = ( uint8_t * )dst;
  p[0] = (uint8_t)(w >>  0);
  p[1] = (uint8_t)(w >>  8);
  p[2] = (uint8_t)(w >> 16);
  p[3] = (uint8_t)(w >> 24);
#endif
}

स्थिर BLAKE2_INLINE शून्य store64( शून्य *dst, uint64_t w )
{
#जइ defined(NATIVE_LITTLE_ENDIAN)
  memcpy(dst, &w, परिमाण w);
#अण्णहा
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

स्थिर BLAKE2_INLINE uint64_t load48( ध्रुव शून्य *src )
{
  ध्रुव uint8_t *p = ( ध्रुव uint8_t * )src;
  प्रत्यागम (( uint64_t )( p[0] ) <<  0) |
         (( uint64_t )( p[1] ) <<  8) |
         (( uint64_t )( p[2] ) << 16) |
         (( uint64_t )( p[3] ) << 24) |
         (( uint64_t )( p[4] ) << 32) |
         (( uint64_t )( p[5] ) << 40) ;
}

स्थिर BLAKE2_INLINE शून्य store48( शून्य *dst, uint64_t w )
{
  uint8_t *p = ( uint8_t * )dst;
  p[0] = (uint8_t)(w >>  0);
  p[1] = (uint8_t)(w >>  8);
  p[2] = (uint8_t)(w >> 16);
  p[3] = (uint8_t)(w >> 24);
  p[4] = (uint8_t)(w >> 32);
  p[5] = (uint8_t)(w >> 40);
}

स्थिर BLAKE2_INLINE uint32_t rotr32( ध्रुव uint32_t w, ध्रुव unsigned c )
{
  प्रत्यागम ( w >> c ) | ( w << ( 32 - c ) );
}

स्थिर BLAKE2_INLINE uint64_t rotr64( ध्रुव uint64_t w, ध्रुव unsigned c )
{
  प्रत्यागम ( w >> c ) | ( w << ( 64 - c ) );
}

/* prevents compiler optimizing out memset() */
स्थिर BLAKE2_INLINE शून्य secure_zero_memory(शून्य *v, size_t n)
{
  स्थिर शून्य *(*ध्रुव चञ्चल memset_v)(शून्य *, पूर्णाङ्क, size_t) = &memset;
  memset_v(v, 0, n);
}

#endif
