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
#اگر_تعریف_نشده BLAKE2_IMPL_H
#تعریف_کن BLAKE2_IMPL_H

#ضمن <endian.h>
#إذا BYTE_ORDER == LITTLE_ENDIAN
# define NATIVE_LITTLE_ENDIAN 1
#پایان_اگر

#ضمن <stdint.h>
#ضمن <string.h>

#إذا !defined(__cplusplus) && (!defined(__STDC_VERSION__) || __STDC_VERSION__ < 199901L)
  #إذا   defined(_MSC_VER)
    #تعریف_کن BLAKE2_INLINE __inline
  #وإلاإذا defined(__GNUC__)
    #تعریف_کن BLAKE2_INLINE __inline__
  #وإلا
    #تعریف_کن BLAKE2_INLINE
  #پایان_اگر
#وإلا
  #تعریف_کن BLAKE2_INLINE مضمن
#پایان_اگر

ساكن BLAKE2_INLINE uint32_t load32( ثابت فراغ *src )
{
#إذا defined(NATIVE_LITTLE_ENDIAN)
  uint32_t w;
  کپی_حافظه(&w, src, حجم w);
  أرجع w;
#وإلا
  ثابت uint8_t *p = ( ثابت uint8_t * )src;
  أرجع (( uint32_t )( p[0] ) <<  0) |
         (( uint32_t )( p[1] ) <<  8) |
         (( uint32_t )( p[2] ) << 16) |
         (( uint32_t )( p[3] ) << 24) ;
#پایان_اگر
}

ساكن BLAKE2_INLINE uint64_t load64( ثابت فراغ *src )
{
#إذا defined(NATIVE_LITTLE_ENDIAN)
  uint64_t w;
  کپی_حافظه(&w, src, حجم w);
  أرجع w;
#وإلا
  ثابت uint8_t *p = ( ثابت uint8_t * )src;
  أرجع (( uint64_t )( p[0] ) <<  0) |
         (( uint64_t )( p[1] ) <<  8) |
         (( uint64_t )( p[2] ) << 16) |
         (( uint64_t )( p[3] ) << 24) |
         (( uint64_t )( p[4] ) << 32) |
         (( uint64_t )( p[5] ) << 40) |
         (( uint64_t )( p[6] ) << 48) |
         (( uint64_t )( p[7] ) << 56) ;
#پایان_اگر
}

ساكن BLAKE2_INLINE uint16_t load16( ثابت فراغ *src )
{
#إذا defined(NATIVE_LITTLE_ENDIAN)
  uint16_t w;
  کپی_حافظه(&w, src, حجم w);
  أرجع w;
#وإلا
  ثابت uint8_t *p = ( ثابت uint8_t * )src;
  أرجع ( uint16_t )((( uint32_t )( p[0] ) <<  0) |
                      (( uint32_t )( p[1] ) <<  8));
#پایان_اگر
}

ساكن BLAKE2_INLINE فراغ store16( فراغ *dst, uint16_t w )
{
#إذا defined(NATIVE_LITTLE_ENDIAN)
  کپی_حافظه(dst, &w, حجم w);
#وإلا
  uint8_t *p = ( uint8_t * )dst;
  *p++ = ( uint8_t )w; w >>= 8;
  *p++ = ( uint8_t )w;
#پایان_اگر
}

ساكن BLAKE2_INLINE فراغ store32( فراغ *dst, uint32_t w )
{
#إذا defined(NATIVE_LITTLE_ENDIAN)
  کپی_حافظه(dst, &w, حجم w);
#وإلا
  uint8_t *p = ( uint8_t * )dst;
  p[0] = (uint8_t)(w >>  0);
  p[1] = (uint8_t)(w >>  8);
  p[2] = (uint8_t)(w >> 16);
  p[3] = (uint8_t)(w >> 24);
#پایان_اگر
}

ساكن BLAKE2_INLINE فراغ store64( فراغ *dst, uint64_t w )
{
#إذا defined(NATIVE_LITTLE_ENDIAN)
  کپی_حافظه(dst, &w, حجم w);
#وإلا
  uint8_t *p = ( uint8_t * )dst;
  p[0] = (uint8_t)(w >>  0);
  p[1] = (uint8_t)(w >>  8);
  p[2] = (uint8_t)(w >> 16);
  p[3] = (uint8_t)(w >> 24);
  p[4] = (uint8_t)(w >> 32);
  p[5] = (uint8_t)(w >> 40);
  p[6] = (uint8_t)(w >> 48);
  p[7] = (uint8_t)(w >> 56);
#پایان_اگر
}

ساكن BLAKE2_INLINE uint64_t load48( ثابت فراغ *src )
{
  ثابت uint8_t *p = ( ثابت uint8_t * )src;
  أرجع (( uint64_t )( p[0] ) <<  0) |
         (( uint64_t )( p[1] ) <<  8) |
         (( uint64_t )( p[2] ) << 16) |
         (( uint64_t )( p[3] ) << 24) |
         (( uint64_t )( p[4] ) << 32) |
         (( uint64_t )( p[5] ) << 40) ;
}

ساكن BLAKE2_INLINE فراغ store48( فراغ *dst, uint64_t w )
{
  uint8_t *p = ( uint8_t * )dst;
  p[0] = (uint8_t)(w >>  0);
  p[1] = (uint8_t)(w >>  8);
  p[2] = (uint8_t)(w >> 16);
  p[3] = (uint8_t)(w >> 24);
  p[4] = (uint8_t)(w >> 32);
  p[5] = (uint8_t)(w >> 40);
}

ساكن BLAKE2_INLINE uint32_t rotr32( ثابت uint32_t w, ثابت بیعلامت c )
{
  أرجع ( w >> c ) | ( w << ( 32 - c ) );
}

ساكن BLAKE2_INLINE uint64_t rotr64( ثابت uint64_t w, ثابت بیعلامت c )
{
  أرجع ( w >> c ) | ( w << ( 64 - c ) );
}

/* prevents compiler optimizing out memset() */
ساكن BLAKE2_INLINE فراغ secure_zero_حافظه(فراغ *v, size_t n)
{
  ساكن فراغ *(*ثابت متطاير memset_v)(فراغ *, صحيح, size_t) = &memset;
  memset_v(v, 0, n);
}

#پایان_اگر
