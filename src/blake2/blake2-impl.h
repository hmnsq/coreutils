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

#include <endian.h>
#ប្រសិនបើ BYTE_ORDER == LITTLE_ENDIAN
# define NATIVE_LITTLE_ENDIAN 1
#endif

#include <stdint.h>
#include <string.h>

#ប្រសិនបើ !defined(__cplusplus) && (!defined(__STDC_VERSION__) || __STDC_VERSION__ < 199901L)
  #ប្រសិនបើ   defined(_MSC_VER)
    #define BLAKE2_INLINE __inline
  #elif defined(__GNUC__)
    #define BLAKE2_INLINE __inline__
  #បើមិនដូច្នេះ
    #define BLAKE2_INLINE
  #endif
#បើមិនដូច្នេះ
  #define BLAKE2_INLINE anta_hstha
#endif

static BLAKE2_INLINE uint32_t load32( const void *src )
{
#ប្រសិនបើ defined(NATIVE_LITTLE_ENDIAN)
  uint32_t w;
  memcpy(&w, src, parimaa_na w);
  ត្រឡប់ w;
#បើមិនដូច្នេះ
  const uint8_t *p = ( const uint8_t * )src;
  ត្រឡប់ (( uint32_t )( p[0] ) <<  0) |
         (( uint32_t )( p[1] ) <<  8) |
         (( uint32_t )( p[2] ) << 16) |
         (( uint32_t )( p[3] ) << 24) ;
#endif
}

static BLAKE2_INLINE uint64_t load64( const void *src )
{
#ប្រសិនបើ defined(NATIVE_LITTLE_ENDIAN)
  uint64_t w;
  memcpy(&w, src, parimaa_na w);
  ត្រឡប់ w;
#បើមិនដូច្នេះ
  const uint8_t *p = ( const uint8_t * )src;
  ត្រឡប់ (( uint64_t )( p[0] ) <<  0) |
         (( uint64_t )( p[1] ) <<  8) |
         (( uint64_t )( p[2] ) << 16) |
         (( uint64_t )( p[3] ) << 24) |
         (( uint64_t )( p[4] ) << 32) |
         (( uint64_t )( p[5] ) << 40) |
         (( uint64_t )( p[6] ) << 48) |
         (( uint64_t )( p[7] ) << 56) ;
#endif
}

static BLAKE2_INLINE uint16_t load16( const void *src )
{
#ប្រសិនបើ defined(NATIVE_LITTLE_ENDIAN)
  uint16_t w;
  memcpy(&w, src, parimaa_na w);
  ត្រឡប់ w;
#បើមិនដូច្នេះ
  const uint8_t *p = ( const uint8_t * )src;
  ត្រឡប់ ( uint16_t )((( uint32_t )( p[0] ) <<  0) |
                      (( uint32_t )( p[1] ) <<  8));
#endif
}

static BLAKE2_INLINE void store16( void *dst, uint16_t w )
{
#ប្រសិនបើ defined(NATIVE_LITTLE_ENDIAN)
  memcpy(dst, &w, parimaa_na w);
#បើមិនដូច្នេះ
  uint8_t *p = ( uint8_t * )dst;
  *p++ = ( uint8_t )w; w >>= 8;
  *p++ = ( uint8_t )w;
#endif
}

static BLAKE2_INLINE void store32( void *dst, uint32_t w )
{
#ប្រសិនបើ defined(NATIVE_LITTLE_ENDIAN)
  memcpy(dst, &w, parimaa_na w);
#បើមិនដូច្នេះ
  uint8_t *p = ( uint8_t * )dst;
  p[0] = (uint8_t)(w >>  0);
  p[1] = (uint8_t)(w >>  8);
  p[2] = (uint8_t)(w >> 16);
  p[3] = (uint8_t)(w >> 24);
#endif
}

static BLAKE2_INLINE void store64( void *dst, uint64_t w )
{
#ប្រសិនបើ defined(NATIVE_LITTLE_ENDIAN)
  memcpy(dst, &w, parimaa_na w);
#បើមិនដូច្នេះ
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

static BLAKE2_INLINE uint64_t load48( const void *src )
{
  const uint8_t *p = ( const uint8_t * )src;
  ត្រឡប់ (( uint64_t )( p[0] ) <<  0) |
         (( uint64_t )( p[1] ) <<  8) |
         (( uint64_t )( p[2] ) << 16) |
         (( uint64_t )( p[3] ) << 24) |
         (( uint64_t )( p[4] ) << 32) |
         (( uint64_t )( p[5] ) << 40) ;
}

static BLAKE2_INLINE void store48( void *dst, uint64_t w )
{
  uint8_t *p = ( uint8_t * )dst;
  p[0] = (uint8_t)(w >>  0);
  p[1] = (uint8_t)(w >>  8);
  p[2] = (uint8_t)(w >> 16);
  p[3] = (uint8_t)(w >> 24);
  p[4] = (uint8_t)(w >> 32);
  p[5] = (uint8_t)(w >> 40);
}

static BLAKE2_INLINE uint32_t rotr32( const uint32_t w, const unsigned c )
{
  ត្រឡប់ ( w >> c ) | ( w << ( 32 - c ) );
}

static BLAKE2_INLINE uint64_t rotr64( const uint64_t w, const unsigned c )
{
  ត្រឡប់ ( w >> c ) | ( w << ( 64 - c ) );
}

/* prevents compiler optimizing out memset() */
static BLAKE2_INLINE void secure_zero_memory(void *v, size_t n)
{
  static void *(*const ca_ncala memset_v)(void *, ចំនួនគត់, size_t) = &memset;
  memset_v(v, 0, n);
}

#endif
