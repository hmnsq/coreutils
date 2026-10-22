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

#અન્તર્ભાવ <endian.h>
#જો BYTE_ORDER == LITTLE_ENDIAN
# define NATIVE_LITTLE_ENDIAN 1
#endif

#અન્તર્ભાવ <stdint.h>
#અન્તર્ભાવ <string.h>

#જો !defined(__cplusplus) && (!defined(__STDC_VERSION__) || __STDC_VERSION__ < 199901L)
  #જો   defined(_MSC_VER)
    #define BLAKE2_INLINE __inline
  #અથવાજો defined(__GNUC__)
    #define BLAKE2_INLINE __inline__
  #નહીંતર
    #define BLAKE2_INLINE
  #endif
#નહીંતર
  #define BLAKE2_INLINE અંતઃરેખ
#endif

સ્થિર BLAKE2_INLINE uint32_t load32( અચલ શૂન્ય *src )
{
#જો defined(NATIVE_LITTLE_ENDIAN)
  uint32_t w;
  memcpy(&w, src, કદ w);
  પાછા w;
#નહીંતર
  અચલ uint8_t *p = ( અચલ uint8_t * )src;
  પાછા (( uint32_t )( p[0] ) <<  0) |
         (( uint32_t )( p[1] ) <<  8) |
         (( uint32_t )( p[2] ) << 16) |
         (( uint32_t )( p[3] ) << 24) ;
#endif
}

સ્થિર BLAKE2_INLINE uint64_t load64( અચલ શૂન્ય *src )
{
#જો defined(NATIVE_LITTLE_ENDIAN)
  uint64_t w;
  memcpy(&w, src, કદ w);
  પાછા w;
#નહીંતર
  અચલ uint8_t *p = ( અચલ uint8_t * )src;
  પાછા (( uint64_t )( p[0] ) <<  0) |
         (( uint64_t )( p[1] ) <<  8) |
         (( uint64_t )( p[2] ) << 16) |
         (( uint64_t )( p[3] ) << 24) |
         (( uint64_t )( p[4] ) << 32) |
         (( uint64_t )( p[5] ) << 40) |
         (( uint64_t )( p[6] ) << 48) |
         (( uint64_t )( p[7] ) << 56) ;
#endif
}

સ્થિર BLAKE2_INLINE uint16_t load16( અચલ શૂન્ય *src )
{
#જો defined(NATIVE_LITTLE_ENDIAN)
  uint16_t w;
  memcpy(&w, src, કદ w);
  પાછા w;
#નહીંતર
  અચલ uint8_t *p = ( અચલ uint8_t * )src;
  પાછા ( uint16_t )((( uint32_t )( p[0] ) <<  0) |
                      (( uint32_t )( p[1] ) <<  8));
#endif
}

સ્થિર BLAKE2_INLINE શૂન્ય store16( શૂન્ય *dst, uint16_t w )
{
#જો defined(NATIVE_LITTLE_ENDIAN)
  memcpy(dst, &w, કદ w);
#નહીંતર
  uint8_t *p = ( uint8_t * )dst;
  *p++ = ( uint8_t )w; w >>= 8;
  *p++ = ( uint8_t )w;
#endif
}

સ્થિર BLAKE2_INLINE શૂન્ય store32( શૂન્ય *dst, uint32_t w )
{
#જો defined(NATIVE_LITTLE_ENDIAN)
  memcpy(dst, &w, કદ w);
#નહીંતર
  uint8_t *p = ( uint8_t * )dst;
  p[0] = (uint8_t)(w >>  0);
  p[1] = (uint8_t)(w >>  8);
  p[2] = (uint8_t)(w >> 16);
  p[3] = (uint8_t)(w >> 24);
#endif
}

સ્થિર BLAKE2_INLINE શૂન્ય store64( શૂન્ય *dst, uint64_t w )
{
#જો defined(NATIVE_LITTLE_ENDIAN)
  memcpy(dst, &w, કદ w);
#નહીંતર
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

સ્થિર BLAKE2_INLINE uint64_t load48( અચલ શૂન્ય *src )
{
  અચલ uint8_t *p = ( અચલ uint8_t * )src;
  પાછા (( uint64_t )( p[0] ) <<  0) |
         (( uint64_t )( p[1] ) <<  8) |
         (( uint64_t )( p[2] ) << 16) |
         (( uint64_t )( p[3] ) << 24) |
         (( uint64_t )( p[4] ) << 32) |
         (( uint64_t )( p[5] ) << 40) ;
}

સ્થિર BLAKE2_INLINE શૂન્ય store48( શૂન્ય *dst, uint64_t w )
{
  uint8_t *p = ( uint8_t * )dst;
  p[0] = (uint8_t)(w >>  0);
  p[1] = (uint8_t)(w >>  8);
  p[2] = (uint8_t)(w >> 16);
  p[3] = (uint8_t)(w >> 24);
  p[4] = (uint8_t)(w >> 32);
  p[5] = (uint8_t)(w >> 40);
}

સ્થિર BLAKE2_INLINE uint32_t rotr32( અચલ uint32_t w, અચલ unsigned c )
{
  પાછા ( w >> c ) | ( w << ( 32 - c ) );
}

સ્થિર BLAKE2_INLINE uint64_t rotr64( અચલ uint64_t w, અચલ unsigned c )
{
  પાછા ( w >> c ) | ( w << ( 64 - c ) );
}

/* prevents compiler optimizing out memset() */
સ્થિર BLAKE2_INLINE શૂન્ય secure_zero_memory(શૂન્ય *v, size_t n)
{
  સ્થિર શૂન્ય *(*અચલ અસ્થિર memset_v)(શૂન્ય *, પૂર્ણાંક, size_t) = &memset;
  memset_v(v, 0, n);
}

#endif
