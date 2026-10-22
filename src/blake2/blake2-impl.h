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
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ BLAKE2_IMPL_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BLAKE2_IMPL_H

#ਅਨ੍ਤਰ੍ਭਾਵ <endian.h>
#ਜੇ BYTE_ORDER == LITTLE_ENDIAN
# define NATIVE_LITTLE_ENDIAN 1
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ <stdint.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>

#ਜੇ !defined(__cplusplus) && (!defined(__STDC_VERSION__) || __STDC_VERSION__ < 199901L)
  #ਜੇ   defined(_MSC_VER)
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BLAKE2_INLINE __inline
  #ਜਾਂਜੇ defined(__GNUC__)
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BLAKE2_INLINE __inline__
  #ਨਹੀਂਤਾਂ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BLAKE2_INLINE
  #ਜੇ_ਅੰਤ
#ਨਹੀਂਤਾਂ
  #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BLAKE2_INLINE ਸਤਰਅੰਦਰ
#ਜੇ_ਅੰਤ

ਸਥਿਰ BLAKE2_INLINE uint32_t load32( ਅਚਲ ਖਾਲੀ *src )
{
#ਜੇ defined(NATIVE_LITTLE_ENDIAN)
  uint32_t w;
  ਸਮ੍ਰਿਤੀ_ਨਕਲ(&w, src, ਆਕਾਰ w);
  ਵਾਪਸ w;
#ਨਹੀਂਤਾਂ
  ਅਚਲ uint8_t *p = ( ਅਚਲ uint8_t * )src;
  ਵਾਪਸ (( uint32_t )( p[0] ) <<  0) |
         (( uint32_t )( p[1] ) <<  8) |
         (( uint32_t )( p[2] ) << 16) |
         (( uint32_t )( p[3] ) << 24) ;
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ BLAKE2_INLINE uint64_t load64( ਅਚਲ ਖਾਲੀ *src )
{
#ਜੇ defined(NATIVE_LITTLE_ENDIAN)
  uint64_t w;
  ਸਮ੍ਰਿਤੀ_ਨਕਲ(&w, src, ਆਕਾਰ w);
  ਵਾਪਸ w;
#ਨਹੀਂਤਾਂ
  ਅਚਲ uint8_t *p = ( ਅਚਲ uint8_t * )src;
  ਵਾਪਸ (( uint64_t )( p[0] ) <<  0) |
         (( uint64_t )( p[1] ) <<  8) |
         (( uint64_t )( p[2] ) << 16) |
         (( uint64_t )( p[3] ) << 24) |
         (( uint64_t )( p[4] ) << 32) |
         (( uint64_t )( p[5] ) << 40) |
         (( uint64_t )( p[6] ) << 48) |
         (( uint64_t )( p[7] ) << 56) ;
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ BLAKE2_INLINE uint16_t load16( ਅਚਲ ਖਾਲੀ *src )
{
#ਜੇ defined(NATIVE_LITTLE_ENDIAN)
  uint16_t w;
  ਸਮ੍ਰਿਤੀ_ਨਕਲ(&w, src, ਆਕਾਰ w);
  ਵਾਪਸ w;
#ਨਹੀਂਤਾਂ
  ਅਚਲ uint8_t *p = ( ਅਚਲ uint8_t * )src;
  ਵਾਪਸ ( uint16_t )((( uint32_t )( p[0] ) <<  0) |
                      (( uint32_t )( p[1] ) <<  8));
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ BLAKE2_INLINE ਖਾਲੀ store16( ਖਾਲੀ *dst, uint16_t w )
{
#ਜੇ defined(NATIVE_LITTLE_ENDIAN)
  ਸਮ੍ਰਿਤੀ_ਨਕਲ(dst, &w, ਆਕਾਰ w);
#ਨਹੀਂਤਾਂ
  uint8_t *p = ( uint8_t * )dst;
  *p++ = ( uint8_t )w; w >>= 8;
  *p++ = ( uint8_t )w;
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ BLAKE2_INLINE ਖਾਲੀ store32( ਖਾਲੀ *dst, uint32_t w )
{
#ਜੇ defined(NATIVE_LITTLE_ENDIAN)
  ਸਮ੍ਰਿਤੀ_ਨਕਲ(dst, &w, ਆਕਾਰ w);
#ਨਹੀਂਤਾਂ
  uint8_t *p = ( uint8_t * )dst;
  p[0] = (uint8_t)(w >>  0);
  p[1] = (uint8_t)(w >>  8);
  p[2] = (uint8_t)(w >> 16);
  p[3] = (uint8_t)(w >> 24);
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ BLAKE2_INLINE ਖਾਲੀ store64( ਖਾਲੀ *dst, uint64_t w )
{
#ਜੇ defined(NATIVE_LITTLE_ENDIAN)
  ਸਮ੍ਰਿਤੀ_ਨਕਲ(dst, &w, ਆਕਾਰ w);
#ਨਹੀਂਤਾਂ
  uint8_t *p = ( uint8_t * )dst;
  p[0] = (uint8_t)(w >>  0);
  p[1] = (uint8_t)(w >>  8);
  p[2] = (uint8_t)(w >> 16);
  p[3] = (uint8_t)(w >> 24);
  p[4] = (uint8_t)(w >> 32);
  p[5] = (uint8_t)(w >> 40);
  p[6] = (uint8_t)(w >> 48);
  p[7] = (uint8_t)(w >> 56);
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ BLAKE2_INLINE uint64_t load48( ਅਚਲ ਖਾਲੀ *src )
{
  ਅਚਲ uint8_t *p = ( ਅਚਲ uint8_t * )src;
  ਵਾਪਸ (( uint64_t )( p[0] ) <<  0) |
         (( uint64_t )( p[1] ) <<  8) |
         (( uint64_t )( p[2] ) << 16) |
         (( uint64_t )( p[3] ) << 24) |
         (( uint64_t )( p[4] ) << 32) |
         (( uint64_t )( p[5] ) << 40) ;
}

ਸਥਿਰ BLAKE2_INLINE ਖਾਲੀ store48( ਖਾਲੀ *dst, uint64_t w )
{
  uint8_t *p = ( uint8_t * )dst;
  p[0] = (uint8_t)(w >>  0);
  p[1] = (uint8_t)(w >>  8);
  p[2] = (uint8_t)(w >> 16);
  p[3] = (uint8_t)(w >> 24);
  p[4] = (uint8_t)(w >> 32);
  p[5] = (uint8_t)(w >> 40);
}

ਸਥਿਰ BLAKE2_INLINE uint32_t rotr32( ਅਚਲ uint32_t w, ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ c )
{
  ਵਾਪਸ ( w >> c ) | ( w << ( 32 - c ) );
}

ਸਥਿਰ BLAKE2_INLINE uint64_t rotr64( ਅਚਲ uint64_t w, ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ c )
{
  ਵਾਪਸ ( w >> c ) | ( w << ( 64 - c ) );
}

/* prevents compiler optimizing out memset() */
ਸਥਿਰ BLAKE2_INLINE ਖਾਲੀ secure_zero_ਸਮ੍ਰਿਤੀ(ਖਾਲੀ *v, size_t n)
{
  ਸਥਿਰ ਖਾਲੀ *(*ਅਚਲ ਅਸਥਿਰ memset_v)(ਖਾਲੀ *, ਪੂਰਨ, size_t) = &memset;
  memset_v(v, 0, n);
}

#ਜੇ_ਅੰਤ
