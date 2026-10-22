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
#अगर_अघोषित BLAKE2_IMPL_H
#घोषणा BLAKE2_IMPL_H

#समावेश <endian.h>
#अगर BYTE_ORDER == LITTLE_ENDIAN
# define NATIVE_LITTLE_ENDIAN 1
#पूर्ण_अगर

#समावेश <stdint.h>
#समावेश <string.h>

#अगर !defined(__cplusplus) && (!defined(__STDC_VERSION__) || __STDC_VERSION__ < 199901L)
  #अगर   defined(_MSC_VER)
    #घोषणा BLAKE2_INLINE __inline
  #या_अगर defined(__GNUC__)
    #घोषणा BLAKE2_INLINE __inline__
  #अन्यथा
    #घोषणा BLAKE2_INLINE
  #पूर्ण_अगर
#अन्यथा
  #घोषणा BLAKE2_INLINE अंतरभूत
#पूर्ण_अगर

अटल BLAKE2_INLINE uint32_t load32( स्थिर व्योम *src )
{
#अगर defined(NATIVE_LITTLE_ENDIAN)
  uint32_t w;
  स_नकल(&w, src, माप w);
  वापस w;
#अन्यथा
  स्थिर uint8_t *p = ( स्थिर uint8_t * )src;
  वापस (( uint32_t )( p[0] ) <<  0) |
         (( uint32_t )( p[1] ) <<  8) |
         (( uint32_t )( p[2] ) << 16) |
         (( uint32_t )( p[3] ) << 24) ;
#पूर्ण_अगर
}

अटल BLAKE2_INLINE uint64_t load64( स्थिर व्योम *src )
{
#अगर defined(NATIVE_LITTLE_ENDIAN)
  uint64_t w;
  स_नकल(&w, src, माप w);
  वापस w;
#अन्यथा
  स्थिर uint8_t *p = ( स्थिर uint8_t * )src;
  वापस (( uint64_t )( p[0] ) <<  0) |
         (( uint64_t )( p[1] ) <<  8) |
         (( uint64_t )( p[2] ) << 16) |
         (( uint64_t )( p[3] ) << 24) |
         (( uint64_t )( p[4] ) << 32) |
         (( uint64_t )( p[5] ) << 40) |
         (( uint64_t )( p[6] ) << 48) |
         (( uint64_t )( p[7] ) << 56) ;
#पूर्ण_अगर
}

अटल BLAKE2_INLINE uint16_t load16( स्थिर व्योम *src )
{
#अगर defined(NATIVE_LITTLE_ENDIAN)
  uint16_t w;
  स_नकल(&w, src, माप w);
  वापस w;
#अन्यथा
  स्थिर uint8_t *p = ( स्थिर uint8_t * )src;
  वापस ( uint16_t )((( uint32_t )( p[0] ) <<  0) |
                      (( uint32_t )( p[1] ) <<  8));
#पूर्ण_अगर
}

अटल BLAKE2_INLINE व्योम store16( व्योम *dst, uint16_t w )
{
#अगर defined(NATIVE_LITTLE_ENDIAN)
  स_नकल(dst, &w, माप w);
#अन्यथा
  uint8_t *p = ( uint8_t * )dst;
  *p++ = ( uint8_t )w; w >>= 8;
  *p++ = ( uint8_t )w;
#पूर्ण_अगर
}

अटल BLAKE2_INLINE व्योम store32( व्योम *dst, uint32_t w )
{
#अगर defined(NATIVE_LITTLE_ENDIAN)
  स_नकल(dst, &w, माप w);
#अन्यथा
  uint8_t *p = ( uint8_t * )dst;
  p[0] = (uint8_t)(w >>  0);
  p[1] = (uint8_t)(w >>  8);
  p[2] = (uint8_t)(w >> 16);
  p[3] = (uint8_t)(w >> 24);
#पूर्ण_अगर
}

अटल BLAKE2_INLINE व्योम store64( व्योम *dst, uint64_t w )
{
#अगर defined(NATIVE_LITTLE_ENDIAN)
  स_नकल(dst, &w, माप w);
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
#पूर्ण_अगर
}

अटल BLAKE2_INLINE uint64_t load48( स्थिर व्योम *src )
{
  स्थिर uint8_t *p = ( स्थिर uint8_t * )src;
  वापस (( uint64_t )( p[0] ) <<  0) |
         (( uint64_t )( p[1] ) <<  8) |
         (( uint64_t )( p[2] ) << 16) |
         (( uint64_t )( p[3] ) << 24) |
         (( uint64_t )( p[4] ) << 32) |
         (( uint64_t )( p[5] ) << 40) ;
}

अटल BLAKE2_INLINE व्योम store48( व्योम *dst, uint64_t w )
{
  uint8_t *p = ( uint8_t * )dst;
  p[0] = (uint8_t)(w >>  0);
  p[1] = (uint8_t)(w >>  8);
  p[2] = (uint8_t)(w >> 16);
  p[3] = (uint8_t)(w >> 24);
  p[4] = (uint8_t)(w >> 32);
  p[5] = (uint8_t)(w >> 40);
}

अटल BLAKE2_INLINE uint32_t rotr32( स्थिर uint32_t w, स्थिर अचिन्हित c )
{
  वापस ( w >> c ) | ( w << ( 32 - c ) );
}

अटल BLAKE2_INLINE uint64_t rotr64( स्थिर uint64_t w, स्थिर अचिन्हित c )
{
  वापस ( w >> c ) | ( w << ( 64 - c ) );
}

/* prevents compiler optimizing out memset() */
अटल BLAKE2_INLINE व्योम secure_zero_memory(व्योम *v, माप_प्रकार n)
{
  अटल व्योम *(*स्थिर अस्थिर memset_v)(व्योम *, पूर्णांक, माप_प्रकार) = &स_रखो;
  memset_v(v, 0, n);
}

#पूर्ण_अगर
