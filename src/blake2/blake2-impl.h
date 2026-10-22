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

#অন্তর্ভাব <endian.h>
#যদি BYTE_ORDER == LITTLE_ENDIAN
# define NATIVE_LITTLE_ENDIAN 1
#endif

#অন্তর্ভাব <stdint.h>
#অন্তর্ভাব <string.h>

#যদি !defined(__cplusplus) && (!defined(__STDC_VERSION__) || __STDC_VERSION__ < 199901L)
  #যদি   defined(_MSC_VER)
    #define BLAKE2_INLINE __inline
  #নতুবা_যদি defined(__GNUC__)
    #define BLAKE2_INLINE __inline__
  #অন্যথা
    #define BLAKE2_INLINE
  #endif
#অন্যথা
  #define BLAKE2_INLINE অন্তঃরেখ
#endif

স্থির BLAKE2_INLINE uint32_t load32( ধ্রুবক শূন্য *src )
{
#যদি defined(NATIVE_LITTLE_ENDIAN)
  uint32_t w;
  memcpy(&w, src, আকার w);
  ফেরত w;
#অন্যথা
  ধ্রুবক uint8_t *p = ( ধ্রুবক uint8_t * )src;
  ফেরত (( uint32_t )( p[0] ) <<  0) |
         (( uint32_t )( p[1] ) <<  8) |
         (( uint32_t )( p[2] ) << 16) |
         (( uint32_t )( p[3] ) << 24) ;
#endif
}

স্থির BLAKE2_INLINE uint64_t load64( ধ্রুবক শূন্য *src )
{
#যদি defined(NATIVE_LITTLE_ENDIAN)
  uint64_t w;
  memcpy(&w, src, আকার w);
  ফেরত w;
#অন্যথা
  ধ্রুবক uint8_t *p = ( ধ্রুবক uint8_t * )src;
  ফেরত (( uint64_t )( p[0] ) <<  0) |
         (( uint64_t )( p[1] ) <<  8) |
         (( uint64_t )( p[2] ) << 16) |
         (( uint64_t )( p[3] ) << 24) |
         (( uint64_t )( p[4] ) << 32) |
         (( uint64_t )( p[5] ) << 40) |
         (( uint64_t )( p[6] ) << 48) |
         (( uint64_t )( p[7] ) << 56) ;
#endif
}

স্থির BLAKE2_INLINE uint16_t load16( ধ্রুবক শূন্য *src )
{
#যদি defined(NATIVE_LITTLE_ENDIAN)
  uint16_t w;
  memcpy(&w, src, আকার w);
  ফেরত w;
#অন্যথা
  ধ্রুবক uint8_t *p = ( ধ্রুবক uint8_t * )src;
  ফেরত ( uint16_t )((( uint32_t )( p[0] ) <<  0) |
                      (( uint32_t )( p[1] ) <<  8));
#endif
}

স্থির BLAKE2_INLINE শূন্য store16( শূন্য *dst, uint16_t w )
{
#যদি defined(NATIVE_LITTLE_ENDIAN)
  memcpy(dst, &w, আকার w);
#অন্যথা
  uint8_t *p = ( uint8_t * )dst;
  *p++ = ( uint8_t )w; w >>= 8;
  *p++ = ( uint8_t )w;
#endif
}

স্থির BLAKE2_INLINE শূন্য store32( শূন্য *dst, uint32_t w )
{
#যদি defined(NATIVE_LITTLE_ENDIAN)
  memcpy(dst, &w, আকার w);
#অন্যথা
  uint8_t *p = ( uint8_t * )dst;
  p[0] = (uint8_t)(w >>  0);
  p[1] = (uint8_t)(w >>  8);
  p[2] = (uint8_t)(w >> 16);
  p[3] = (uint8_t)(w >> 24);
#endif
}

স্থির BLAKE2_INLINE শূন্য store64( শূন্য *dst, uint64_t w )
{
#যদি defined(NATIVE_LITTLE_ENDIAN)
  memcpy(dst, &w, আকার w);
#অন্যথা
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

স্থির BLAKE2_INLINE uint64_t load48( ধ্রুবক শূন্য *src )
{
  ধ্রুবক uint8_t *p = ( ধ্রুবক uint8_t * )src;
  ফেরত (( uint64_t )( p[0] ) <<  0) |
         (( uint64_t )( p[1] ) <<  8) |
         (( uint64_t )( p[2] ) << 16) |
         (( uint64_t )( p[3] ) << 24) |
         (( uint64_t )( p[4] ) << 32) |
         (( uint64_t )( p[5] ) << 40) ;
}

স্থির BLAKE2_INLINE শূন্য store48( শূন্য *dst, uint64_t w )
{
  uint8_t *p = ( uint8_t * )dst;
  p[0] = (uint8_t)(w >>  0);
  p[1] = (uint8_t)(w >>  8);
  p[2] = (uint8_t)(w >> 16);
  p[3] = (uint8_t)(w >> 24);
  p[4] = (uint8_t)(w >> 32);
  p[5] = (uint8_t)(w >> 40);
}

স্থির BLAKE2_INLINE uint32_t rotr32( ধ্রুবক uint32_t w, ধ্রুবক unsigned c )
{
  ফেরত ( w >> c ) | ( w << ( 32 - c ) );
}

স্থির BLAKE2_INLINE uint64_t rotr64( ধ্রুবক uint64_t w, ধ্রুবক unsigned c )
{
  ফেরত ( w >> c ) | ( w << ( 64 - c ) );
}

/* prevents compiler optimizing out memset() */
স্থির BLAKE2_INLINE শূন্য secure_zero_memory(শূন্য *v, size_t n)
{
  স্থির শূন্য *(*ধ্রুবক অস্থির memset_v)(শূন্য *, পূর্ণসংখ্যা, size_t) = &memset;
  memset_v(v, 0, n);
}

#endif
