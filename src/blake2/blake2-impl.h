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
#اگر_غیر_تعریف_شدہ BLAKE2_IMPL_H
#تعریف_کرو BLAKE2_IMPL_H

#شامل <endian.h>
#اگر BYTE_ORDER == LITTLE_ENDIAN
# define NATIVE_LITTLE_ENDIAN 1
#اگر_ختم

#شامل <stdint.h>
#شامل <string.h>

#اگر !defined(__cplusplus) && (!defined(__STDC_VERSION__) || __STDC_VERSION__ < 199901L)
  #اگر   defined(_MSC_VER)
    #تعریف_کرو BLAKE2_INLINE __inline
  #وگرنہ_اگر defined(__GNUC__)
    #تعریف_کرو BLAKE2_INLINE __inline__
  #ورنہ
    #تعریف_کرو BLAKE2_INLINE
  #اگر_ختم
#ورنہ
  #تعریف_کرو BLAKE2_INLINE اندرونی
#اگر_ختم

ساکن BLAKE2_INLINE uint32_t load32( ثابت خالی *src )
{
#اگر defined(NATIVE_LITTLE_ENDIAN)
  uint32_t w;
  حافظہ_نقل(&w, src, حجم w);
  واپس w;
#ورنہ
  ثابت uint8_t *p = ( ثابت uint8_t * )src;
  واپس (( uint32_t )( p[0] ) <<  0) |
         (( uint32_t )( p[1] ) <<  8) |
         (( uint32_t )( p[2] ) << 16) |
         (( uint32_t )( p[3] ) << 24) ;
#اگر_ختم
}

ساکن BLAKE2_INLINE uint64_t load64( ثابت خالی *src )
{
#اگر defined(NATIVE_LITTLE_ENDIAN)
  uint64_t w;
  حافظہ_نقل(&w, src, حجم w);
  واپس w;
#ورنہ
  ثابت uint8_t *p = ( ثابت uint8_t * )src;
  واپس (( uint64_t )( p[0] ) <<  0) |
         (( uint64_t )( p[1] ) <<  8) |
         (( uint64_t )( p[2] ) << 16) |
         (( uint64_t )( p[3] ) << 24) |
         (( uint64_t )( p[4] ) << 32) |
         (( uint64_t )( p[5] ) << 40) |
         (( uint64_t )( p[6] ) << 48) |
         (( uint64_t )( p[7] ) << 56) ;
#اگر_ختم
}

ساکن BLAKE2_INLINE uint16_t load16( ثابت خالی *src )
{
#اگر defined(NATIVE_LITTLE_ENDIAN)
  uint16_t w;
  حافظہ_نقل(&w, src, حجم w);
  واپس w;
#ورنہ
  ثابت uint8_t *p = ( ثابت uint8_t * )src;
  واپس ( uint16_t )((( uint32_t )( p[0] ) <<  0) |
                      (( uint32_t )( p[1] ) <<  8));
#اگر_ختم
}

ساکن BLAKE2_INLINE خالی store16( خالی *dst, uint16_t w )
{
#اگر defined(NATIVE_LITTLE_ENDIAN)
  حافظہ_نقل(dst, &w, حجم w);
#ورنہ
  uint8_t *p = ( uint8_t * )dst;
  *p++ = ( uint8_t )w; w >>= 8;
  *p++ = ( uint8_t )w;
#اگر_ختم
}

ساکن BLAKE2_INLINE خالی store32( خالی *dst, uint32_t w )
{
#اگر defined(NATIVE_LITTLE_ENDIAN)
  حافظہ_نقل(dst, &w, حجم w);
#ورنہ
  uint8_t *p = ( uint8_t * )dst;
  p[0] = (uint8_t)(w >>  0);
  p[1] = (uint8_t)(w >>  8);
  p[2] = (uint8_t)(w >> 16);
  p[3] = (uint8_t)(w >> 24);
#اگر_ختم
}

ساکن BLAKE2_INLINE خالی store64( خالی *dst, uint64_t w )
{
#اگر defined(NATIVE_LITTLE_ENDIAN)
  حافظہ_نقل(dst, &w, حجم w);
#ورنہ
  uint8_t *p = ( uint8_t * )dst;
  p[0] = (uint8_t)(w >>  0);
  p[1] = (uint8_t)(w >>  8);
  p[2] = (uint8_t)(w >> 16);
  p[3] = (uint8_t)(w >> 24);
  p[4] = (uint8_t)(w >> 32);
  p[5] = (uint8_t)(w >> 40);
  p[6] = (uint8_t)(w >> 48);
  p[7] = (uint8_t)(w >> 56);
#اگر_ختم
}

ساکن BLAKE2_INLINE uint64_t load48( ثابت خالی *src )
{
  ثابت uint8_t *p = ( ثابت uint8_t * )src;
  واپس (( uint64_t )( p[0] ) <<  0) |
         (( uint64_t )( p[1] ) <<  8) |
         (( uint64_t )( p[2] ) << 16) |
         (( uint64_t )( p[3] ) << 24) |
         (( uint64_t )( p[4] ) << 32) |
         (( uint64_t )( p[5] ) << 40) ;
}

ساکن BLAKE2_INLINE خالی store48( خالی *dst, uint64_t w )
{
  uint8_t *p = ( uint8_t * )dst;
  p[0] = (uint8_t)(w >>  0);
  p[1] = (uint8_t)(w >>  8);
  p[2] = (uint8_t)(w >> 16);
  p[3] = (uint8_t)(w >> 24);
  p[4] = (uint8_t)(w >> 32);
  p[5] = (uint8_t)(w >> 40);
}

ساکن BLAKE2_INLINE uint32_t rotr32( ثابت uint32_t w, ثابت غیرعلامتی c )
{
  واپس ( w >> c ) | ( w << ( 32 - c ) );
}

ساکن BLAKE2_INLINE uint64_t rotr64( ثابت uint64_t w, ثابت غیرعلامتی c )
{
  واپس ( w >> c ) | ( w << ( 64 - c ) );
}

/* prevents compiler optimizing out memset() */
ساکن BLAKE2_INLINE خالی secure_zero_حافظہ(خالی *v, size_t n)
{
  ساکن خالی *(*ثابت متلون memset_v)(خالی *, صحیح, size_t) = &memset;
  memset_v(v, 0, n);
}

#اگر_ختم
