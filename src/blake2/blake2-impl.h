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

#அந்தர்பாவ <endian.h>
#எனில் BYTE_ORDER == LITTLE_ENDIAN
# define NATIVE_LITTLE_ENDIAN 1
#endif

#அந்தர்பாவ <stdint.h>
#அந்தர்பாவ <string.h>

#எனில் !defined(__cplusplus) && (!defined(__STDC_VERSION__) || __STDC_VERSION__ < 199901L)
  #எனில்   defined(_MSC_VER)
    #define BLAKE2_INLINE __inline
  #அல்லதெனில் defined(__GNUC__)
    #define BLAKE2_INLINE __inline__
  #இல்லையெனில்
    #define BLAKE2_INLINE
  #endif
#இல்லையெனில்
  #define BLAKE2_INLINE உள்வரி
#endif

நிலையான BLAKE2_INLINE uint32_t load32( மாறிலி வெறுமை *src )
{
#எனில் defined(NATIVE_LITTLE_ENDIAN)
  uint32_t w;
  memcpy(&w, src, அளவு w);
  திரும்பு w;
#இல்லையெனில்
  மாறிலி uint8_t *p = ( மாறிலி uint8_t * )src;
  திரும்பு (( uint32_t )( p[0] ) <<  0) |
         (( uint32_t )( p[1] ) <<  8) |
         (( uint32_t )( p[2] ) << 16) |
         (( uint32_t )( p[3] ) << 24) ;
#endif
}

நிலையான BLAKE2_INLINE uint64_t load64( மாறிலி வெறுமை *src )
{
#எனில் defined(NATIVE_LITTLE_ENDIAN)
  uint64_t w;
  memcpy(&w, src, அளவு w);
  திரும்பு w;
#இல்லையெனில்
  மாறிலி uint8_t *p = ( மாறிலி uint8_t * )src;
  திரும்பு (( uint64_t )( p[0] ) <<  0) |
         (( uint64_t )( p[1] ) <<  8) |
         (( uint64_t )( p[2] ) << 16) |
         (( uint64_t )( p[3] ) << 24) |
         (( uint64_t )( p[4] ) << 32) |
         (( uint64_t )( p[5] ) << 40) |
         (( uint64_t )( p[6] ) << 48) |
         (( uint64_t )( p[7] ) << 56) ;
#endif
}

நிலையான BLAKE2_INLINE uint16_t load16( மாறிலி வெறுமை *src )
{
#எனில் defined(NATIVE_LITTLE_ENDIAN)
  uint16_t w;
  memcpy(&w, src, அளவு w);
  திரும்பு w;
#இல்லையெனில்
  மாறிலி uint8_t *p = ( மாறிலி uint8_t * )src;
  திரும்பு ( uint16_t )((( uint32_t )( p[0] ) <<  0) |
                      (( uint32_t )( p[1] ) <<  8));
#endif
}

நிலையான BLAKE2_INLINE வெறுமை store16( வெறுமை *dst, uint16_t w )
{
#எனில் defined(NATIVE_LITTLE_ENDIAN)
  memcpy(dst, &w, அளவு w);
#இல்லையெனில்
  uint8_t *p = ( uint8_t * )dst;
  *p++ = ( uint8_t )w; w >>= 8;
  *p++ = ( uint8_t )w;
#endif
}

நிலையான BLAKE2_INLINE வெறுமை store32( வெறுமை *dst, uint32_t w )
{
#எனில் defined(NATIVE_LITTLE_ENDIAN)
  memcpy(dst, &w, அளவு w);
#இல்லையெனில்
  uint8_t *p = ( uint8_t * )dst;
  p[0] = (uint8_t)(w >>  0);
  p[1] = (uint8_t)(w >>  8);
  p[2] = (uint8_t)(w >> 16);
  p[3] = (uint8_t)(w >> 24);
#endif
}

நிலையான BLAKE2_INLINE வெறுமை store64( வெறுமை *dst, uint64_t w )
{
#எனில் defined(NATIVE_LITTLE_ENDIAN)
  memcpy(dst, &w, அளவு w);
#இல்லையெனில்
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

நிலையான BLAKE2_INLINE uint64_t load48( மாறிலி வெறுமை *src )
{
  மாறிலி uint8_t *p = ( மாறிலி uint8_t * )src;
  திரும்பு (( uint64_t )( p[0] ) <<  0) |
         (( uint64_t )( p[1] ) <<  8) |
         (( uint64_t )( p[2] ) << 16) |
         (( uint64_t )( p[3] ) << 24) |
         (( uint64_t )( p[4] ) << 32) |
         (( uint64_t )( p[5] ) << 40) ;
}

நிலையான BLAKE2_INLINE வெறுமை store48( வெறுமை *dst, uint64_t w )
{
  uint8_t *p = ( uint8_t * )dst;
  p[0] = (uint8_t)(w >>  0);
  p[1] = (uint8_t)(w >>  8);
  p[2] = (uint8_t)(w >> 16);
  p[3] = (uint8_t)(w >> 24);
  p[4] = (uint8_t)(w >> 32);
  p[5] = (uint8_t)(w >> 40);
}

நிலையான BLAKE2_INLINE uint32_t rotr32( மாறிலி uint32_t w, மாறிலி unsigned c )
{
  திரும்பு ( w >> c ) | ( w << ( 32 - c ) );
}

நிலையான BLAKE2_INLINE uint64_t rotr64( மாறிலி uint64_t w, மாறிலி unsigned c )
{
  திரும்பு ( w >> c ) | ( w << ( 64 - c ) );
}

/* prevents compiler optimizing out memset() */
நிலையான BLAKE2_INLINE வெறுமை secure_zero_memory(வெறுமை *v, size_t n)
{
  நிலையான வெறுமை *(*மாறிலி நிலையற்ற memset_v)(வெறுமை *, முழு, size_t) = &memset;
  memset_v(v, 0, n);
}

#endif
