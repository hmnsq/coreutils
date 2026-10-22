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
#यदि BYTE_ORDER == LITTLE_ENDIAN
# define NATIVE_LITTLE_ENDIAN 1
#endif

#अन्तर्भाव <stdint.h>
#अन्तर्भाव <string.h>

#यदि !defined(__cplusplus) && (!defined(__STDC_VERSION__) || __STDC_VERSION__ < 199901L)
  #यदि   defined(_MSC_VER)
    #define BLAKE2_INLINE __inline
  #अथवायदि defined(__GNUC__)
    #define BLAKE2_INLINE __inline__
  #अन्यथा
    #define BLAKE2_INLINE
  #endif
#अन्यथा
  #define BLAKE2_INLINE अन्तःस्थ
#endif

स्थिर BLAKE2_INLINE uint32_t load32( अचर शून्य *src )
{
#यदि defined(NATIVE_LITTLE_ENDIAN)
  uint32_t w;
  स_नकल(&w, src, परिमाण w);
  फर्काऊ w;
#अन्यथा
  अचर uint8_t *p = ( अचर uint8_t * )src;
  फर्काऊ (( uint32_t )( p[0] ) <<  0) |
         (( uint32_t )( p[1] ) <<  8) |
         (( uint32_t )( p[2] ) << 16) |
         (( uint32_t )( p[3] ) << 24) ;
#endif
}

स्थिर BLAKE2_INLINE uint64_t load64( अचर शून्य *src )
{
#यदि defined(NATIVE_LITTLE_ENDIAN)
  uint64_t w;
  स_नकल(&w, src, परिमाण w);
  फर्काऊ w;
#अन्यथा
  अचर uint8_t *p = ( अचर uint8_t * )src;
  फर्काऊ (( uint64_t )( p[0] ) <<  0) |
         (( uint64_t )( p[1] ) <<  8) |
         (( uint64_t )( p[2] ) << 16) |
         (( uint64_t )( p[3] ) << 24) |
         (( uint64_t )( p[4] ) << 32) |
         (( uint64_t )( p[5] ) << 40) |
         (( uint64_t )( p[6] ) << 48) |
         (( uint64_t )( p[7] ) << 56) ;
#endif
}

स्थिर BLAKE2_INLINE uint16_t load16( अचर शून्य *src )
{
#यदि defined(NATIVE_LITTLE_ENDIAN)
  uint16_t w;
  स_नकल(&w, src, परिमाण w);
  फर्काऊ w;
#अन्यथा
  अचर uint8_t *p = ( अचर uint8_t * )src;
  फर्काऊ ( uint16_t )((( uint32_t )( p[0] ) <<  0) |
                      (( uint32_t )( p[1] ) <<  8));
#endif
}

स्थिर BLAKE2_INLINE शून्य store16( शून्य *dst, uint16_t w )
{
#यदि defined(NATIVE_LITTLE_ENDIAN)
  स_नकल(dst, &w, परिमाण w);
#अन्यथा
  uint8_t *p = ( uint8_t * )dst;
  *p++ = ( uint8_t )w; w >>= 8;
  *p++ = ( uint8_t )w;
#endif
}

स्थिर BLAKE2_INLINE शून्य store32( शून्य *dst, uint32_t w )
{
#यदि defined(NATIVE_LITTLE_ENDIAN)
  स_नकल(dst, &w, परिमाण w);
#अन्यथा
  uint8_t *p = ( uint8_t * )dst;
  p[0] = (uint8_t)(w >>  0);
  p[1] = (uint8_t)(w >>  8);
  p[2] = (uint8_t)(w >> 16);
  p[3] = (uint8_t)(w >> 24);
#endif
}

स्थिर BLAKE2_INLINE शून्य store64( शून्य *dst, uint64_t w )
{
#यदि defined(NATIVE_LITTLE_ENDIAN)
  स_नकल(dst, &w, परिमाण w);
#अन्यथा
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

स्थिर BLAKE2_INLINE uint64_t load48( अचर शून्य *src )
{
  अचर uint8_t *p = ( अचर uint8_t * )src;
  फर्काऊ (( uint64_t )( p[0] ) <<  0) |
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

स्थिर BLAKE2_INLINE uint32_t rotr32( अचर uint32_t w, अचर अचिन्हित c )
{
  फर्काऊ ( w >> c ) | ( w << ( 32 - c ) );
}

स्थिर BLAKE2_INLINE uint64_t rotr64( अचर uint64_t w, अचर अचिन्हित c )
{
  फर्काऊ ( w >> c ) | ( w << ( 64 - c ) );
}

/* prevents compiler optimizing out memset() */
स्थिर BLAKE2_INLINE शून्य secure_zero_memory(शून्य *v, माप_प्रकार n)
{
  स्थिर शून्य *(*अचर चञ्चल memset_v)(शून्य *, पूर्णाङ्क, माप_प्रकार) = &स_रखो;
  memset_v(v, 0, n);
}

#endif
