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

#ಅನ್ತರ್ಭಾವ <endian.h>
#ಯದಿ BYTE_ORDER == LITTLE_ENDIAN
# define NATIVE_LITTLE_ENDIAN 1
#endif

#ಅನ್ತರ್ಭಾವ <stdint.h>
#ಅನ್ತರ್ಭಾವ <string.h>

#ಯದಿ !defined(__cplusplus) && (!defined(__STDC_VERSION__) || __STDC_VERSION__ < 199901L)
  #ಯದಿ   defined(_MSC_VER)
    #define BLAKE2_INLINE __inline
  #ನೋಚೇದ್ಯದಿ defined(__GNUC__)
    #define BLAKE2_INLINE __inline__
  #ಅನ್ಯಥಾ
    #define BLAKE2_INLINE
  #endif
#ಅನ್ಯಥಾ
  #define BLAKE2_INLINE ಅನ್ತಃಸ್ಥ
#endif

ಸ್ಥಿರ BLAKE2_INLINE uint32_t load32( ಧ್ರುವ ಶೂನ್ಯ *src )
{
#ಯದಿ defined(NATIVE_LITTLE_ENDIAN)
  uint32_t w;
  memcpy(&w, src, ಪರಿಮಾಣ w);
  ಪ್ರತ್ಯಾಗಮ w;
#ಅನ್ಯಥಾ
  ಧ್ರುವ uint8_t *p = ( ಧ್ರುವ uint8_t * )src;
  ಪ್ರತ್ಯಾಗಮ (( uint32_t )( p[0] ) <<  0) |
         (( uint32_t )( p[1] ) <<  8) |
         (( uint32_t )( p[2] ) << 16) |
         (( uint32_t )( p[3] ) << 24) ;
#endif
}

ಸ್ಥಿರ BLAKE2_INLINE uint64_t load64( ಧ್ರುವ ಶೂನ್ಯ *src )
{
#ಯದಿ defined(NATIVE_LITTLE_ENDIAN)
  uint64_t w;
  memcpy(&w, src, ಪರಿಮಾಣ w);
  ಪ್ರತ್ಯಾಗಮ w;
#ಅನ್ಯಥಾ
  ಧ್ರುವ uint8_t *p = ( ಧ್ರುವ uint8_t * )src;
  ಪ್ರತ್ಯಾಗಮ (( uint64_t )( p[0] ) <<  0) |
         (( uint64_t )( p[1] ) <<  8) |
         (( uint64_t )( p[2] ) << 16) |
         (( uint64_t )( p[3] ) << 24) |
         (( uint64_t )( p[4] ) << 32) |
         (( uint64_t )( p[5] ) << 40) |
         (( uint64_t )( p[6] ) << 48) |
         (( uint64_t )( p[7] ) << 56) ;
#endif
}

ಸ್ಥಿರ BLAKE2_INLINE uint16_t load16( ಧ್ರುವ ಶೂನ್ಯ *src )
{
#ಯದಿ defined(NATIVE_LITTLE_ENDIAN)
  uint16_t w;
  memcpy(&w, src, ಪರಿಮಾಣ w);
  ಪ್ರತ್ಯಾಗಮ w;
#ಅನ್ಯಥಾ
  ಧ್ರುವ uint8_t *p = ( ಧ್ರುವ uint8_t * )src;
  ಪ್ರತ್ಯಾಗಮ ( uint16_t )((( uint32_t )( p[0] ) <<  0) |
                      (( uint32_t )( p[1] ) <<  8));
#endif
}

ಸ್ಥಿರ BLAKE2_INLINE ಶೂನ್ಯ store16( ಶೂನ್ಯ *dst, uint16_t w )
{
#ಯದಿ defined(NATIVE_LITTLE_ENDIAN)
  memcpy(dst, &w, ಪರಿಮಾಣ w);
#ಅನ್ಯಥಾ
  uint8_t *p = ( uint8_t * )dst;
  *p++ = ( uint8_t )w; w >>= 8;
  *p++ = ( uint8_t )w;
#endif
}

ಸ್ಥಿರ BLAKE2_INLINE ಶೂನ್ಯ store32( ಶೂನ್ಯ *dst, uint32_t w )
{
#ಯದಿ defined(NATIVE_LITTLE_ENDIAN)
  memcpy(dst, &w, ಪರಿಮಾಣ w);
#ಅನ್ಯಥಾ
  uint8_t *p = ( uint8_t * )dst;
  p[0] = (uint8_t)(w >>  0);
  p[1] = (uint8_t)(w >>  8);
  p[2] = (uint8_t)(w >> 16);
  p[3] = (uint8_t)(w >> 24);
#endif
}

ಸ್ಥಿರ BLAKE2_INLINE ಶೂನ್ಯ store64( ಶೂನ್ಯ *dst, uint64_t w )
{
#ಯದಿ defined(NATIVE_LITTLE_ENDIAN)
  memcpy(dst, &w, ಪರಿಮಾಣ w);
#ಅನ್ಯಥಾ
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

ಸ್ಥಿರ BLAKE2_INLINE uint64_t load48( ಧ್ರುವ ಶೂನ್ಯ *src )
{
  ಧ್ರುವ uint8_t *p = ( ಧ್ರುವ uint8_t * )src;
  ಪ್ರತ್ಯಾಗಮ (( uint64_t )( p[0] ) <<  0) |
         (( uint64_t )( p[1] ) <<  8) |
         (( uint64_t )( p[2] ) << 16) |
         (( uint64_t )( p[3] ) << 24) |
         (( uint64_t )( p[4] ) << 32) |
         (( uint64_t )( p[5] ) << 40) ;
}

ಸ್ಥಿರ BLAKE2_INLINE ಶೂನ್ಯ store48( ಶೂನ್ಯ *dst, uint64_t w )
{
  uint8_t *p = ( uint8_t * )dst;
  p[0] = (uint8_t)(w >>  0);
  p[1] = (uint8_t)(w >>  8);
  p[2] = (uint8_t)(w >> 16);
  p[3] = (uint8_t)(w >> 24);
  p[4] = (uint8_t)(w >> 32);
  p[5] = (uint8_t)(w >> 40);
}

ಸ್ಥಿರ BLAKE2_INLINE uint32_t rotr32( ಧ್ರುವ uint32_t w, ಧ್ರುವ unsigned c )
{
  ಪ್ರತ್ಯಾಗಮ ( w >> c ) | ( w << ( 32 - c ) );
}

ಸ್ಥಿರ BLAKE2_INLINE uint64_t rotr64( ಧ್ರುವ uint64_t w, ಧ್ರುವ unsigned c )
{
  ಪ್ರತ್ಯಾಗಮ ( w >> c ) | ( w << ( 64 - c ) );
}

/* prevents compiler optimizing out memset() */
ಸ್ಥಿರ BLAKE2_INLINE ಶೂನ್ಯ secure_zero_memory(ಶೂನ್ಯ *v, size_t n)
{
  ಸ್ಥಿರ ಶೂನ್ಯ *(*ಧ್ರುವ ಚಞ್ಚಲ memset_v)(ಶೂನ್ಯ *, ಪೂರ್ಣಾಙ್ಕ, size_t) = &memset;
  memset_v(v, 0, n);
}

#endif
