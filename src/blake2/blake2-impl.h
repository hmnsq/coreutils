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

#അന്തര്ഭാവ <endian.h>
#എങ്കിൽ BYTE_ORDER == LITTLE_ENDIAN
# define NATIVE_LITTLE_ENDIAN 1
#endif

#അന്തര്ഭാവ <stdint.h>
#അന്തര്ഭാവ <string.h>

#എങ്കിൽ !defined(__cplusplus) && (!defined(__STDC_VERSION__) || __STDC_VERSION__ < 199901L)
  #എങ്കിൽ   defined(_MSC_VER)
    #define BLAKE2_INLINE __inline
  #അല്ലെങ്കിലെങ്കിൽ defined(__GNUC__)
    #define BLAKE2_INLINE __inline__
  #അല്ലെങ്കിൽ
    #define BLAKE2_INLINE
  #endif
#അല്ലെങ്കിൽ
  #define BLAKE2_INLINE വരിയിൽ
#endif

സ്ഥിതം BLAKE2_INLINE uint32_t load32( സ്ഥിരം ശൂന്യം *src )
{
#എങ്കിൽ defined(NATIVE_LITTLE_ENDIAN)
  uint32_t w;
  memcpy(&w, src, വലുപ്പം w);
  തിരികെ w;
#അല്ലെങ്കിൽ
  സ്ഥിരം uint8_t *p = ( സ്ഥിരം uint8_t * )src;
  തിരികെ (( uint32_t )( p[0] ) <<  0) |
         (( uint32_t )( p[1] ) <<  8) |
         (( uint32_t )( p[2] ) << 16) |
         (( uint32_t )( p[3] ) << 24) ;
#endif
}

സ്ഥിതം BLAKE2_INLINE uint64_t load64( സ്ഥിരം ശൂന്യം *src )
{
#എങ്കിൽ defined(NATIVE_LITTLE_ENDIAN)
  uint64_t w;
  memcpy(&w, src, വലുപ്പം w);
  തിരികെ w;
#അല്ലെങ്കിൽ
  സ്ഥിരം uint8_t *p = ( സ്ഥിരം uint8_t * )src;
  തിരികെ (( uint64_t )( p[0] ) <<  0) |
         (( uint64_t )( p[1] ) <<  8) |
         (( uint64_t )( p[2] ) << 16) |
         (( uint64_t )( p[3] ) << 24) |
         (( uint64_t )( p[4] ) << 32) |
         (( uint64_t )( p[5] ) << 40) |
         (( uint64_t )( p[6] ) << 48) |
         (( uint64_t )( p[7] ) << 56) ;
#endif
}

സ്ഥിതം BLAKE2_INLINE uint16_t load16( സ്ഥിരം ശൂന്യം *src )
{
#എങ്കിൽ defined(NATIVE_LITTLE_ENDIAN)
  uint16_t w;
  memcpy(&w, src, വലുപ്പം w);
  തിരികെ w;
#അല്ലെങ്കിൽ
  സ്ഥിരം uint8_t *p = ( സ്ഥിരം uint8_t * )src;
  തിരികെ ( uint16_t )((( uint32_t )( p[0] ) <<  0) |
                      (( uint32_t )( p[1] ) <<  8));
#endif
}

സ്ഥിതം BLAKE2_INLINE ശൂന്യം store16( ശൂന്യം *dst, uint16_t w )
{
#എങ്കിൽ defined(NATIVE_LITTLE_ENDIAN)
  memcpy(dst, &w, വലുപ്പം w);
#അല്ലെങ്കിൽ
  uint8_t *p = ( uint8_t * )dst;
  *p++ = ( uint8_t )w; w >>= 8;
  *p++ = ( uint8_t )w;
#endif
}

സ്ഥിതം BLAKE2_INLINE ശൂന്യം store32( ശൂന്യം *dst, uint32_t w )
{
#എങ്കിൽ defined(NATIVE_LITTLE_ENDIAN)
  memcpy(dst, &w, വലുപ്പം w);
#അല്ലെങ്കിൽ
  uint8_t *p = ( uint8_t * )dst;
  p[0] = (uint8_t)(w >>  0);
  p[1] = (uint8_t)(w >>  8);
  p[2] = (uint8_t)(w >> 16);
  p[3] = (uint8_t)(w >> 24);
#endif
}

സ്ഥിതം BLAKE2_INLINE ശൂന്യം store64( ശൂന്യം *dst, uint64_t w )
{
#എങ്കിൽ defined(NATIVE_LITTLE_ENDIAN)
  memcpy(dst, &w, വലുപ്പം w);
#അല്ലെങ്കിൽ
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

സ്ഥിതം BLAKE2_INLINE uint64_t load48( സ്ഥിരം ശൂന്യം *src )
{
  സ്ഥിരം uint8_t *p = ( സ്ഥിരം uint8_t * )src;
  തിരികെ (( uint64_t )( p[0] ) <<  0) |
         (( uint64_t )( p[1] ) <<  8) |
         (( uint64_t )( p[2] ) << 16) |
         (( uint64_t )( p[3] ) << 24) |
         (( uint64_t )( p[4] ) << 32) |
         (( uint64_t )( p[5] ) << 40) ;
}

സ്ഥിതം BLAKE2_INLINE ശൂന്യം store48( ശൂന്യം *dst, uint64_t w )
{
  uint8_t *p = ( uint8_t * )dst;
  p[0] = (uint8_t)(w >>  0);
  p[1] = (uint8_t)(w >>  8);
  p[2] = (uint8_t)(w >> 16);
  p[3] = (uint8_t)(w >> 24);
  p[4] = (uint8_t)(w >> 32);
  p[5] = (uint8_t)(w >> 40);
}

സ്ഥിതം BLAKE2_INLINE uint32_t rotr32( സ്ഥിരം uint32_t w, സ്ഥിരം unsigned c )
{
  തിരികെ ( w >> c ) | ( w << ( 32 - c ) );
}

സ്ഥിതം BLAKE2_INLINE uint64_t rotr64( സ്ഥിരം uint64_t w, സ്ഥിരം unsigned c )
{
  തിരികെ ( w >> c ) | ( w << ( 64 - c ) );
}

/* prevents compiler optimizing out memset() */
സ്ഥിതം BLAKE2_INLINE ശൂന്യം secure_zero_memory(ശൂന്യം *v, size_t n)
{
  സ്ഥിതം ശൂന്യം *(*സ്ഥിരം അസ്ഥിര memset_v)(ശൂന്യം *, പൂർണ്ണസംഖ്യ, size_t) = &memset;
  memset_v(v, 0, n);
}

#endif
