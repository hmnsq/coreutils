/* Calculate checksums and sizes of files
   Copyright 2025-2026 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ __CKSUM_CRC_H__
# define __CKSUM_CRC_H__

# include <stdint.h>
# include <stdio.h>

ਬਾਹਰੀ bool cksum_debug;

ਬਾਹਰੀ ਪੂਰਨ
ਚੱਕਰੀ_ਜਾਂਚ_ਜੋੜ_ਧਾਰਾ (FILE *ਧਾਰਾ, ਖਾਲੀ *resstream, intmax_t *ਲੰਮਾਈ);

ਬਾਹਰੀ ਪੂਰਨ
crc32b_ਜੋੜ_ਧਾਰਾ (FILE *ਧਾਰਾ, ਖਾਲੀ *resstream, intmax_t *ਲੰਮਾਈ);

ਬਾਹਰੀ ਖਾਲੀ
output_ਚੱਕਰੀ_ਜਾਂਚ (ਅੱਖਰ ਅਚਲ *file, ਪੂਰਨ ਦੁਆਧਾਰੀ_file, ਖਾਲੀ ਅਚਲ *digest, bool raw,
            bool tagged, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ delim, bool ਦਲੀਲਾਂ, intmax_t ਲੰਮਾਈ)
  _GL_ATTRIBUTE_NONNULL ((3));

ਬਾਹਰੀ bool
cksum_vmull (FILE *fp, uint_fast32_t *ਚੱਕਰੀ_ਜਾਂਚ_ਬਾਹਰਲਾ, intmax_t *ਲੰਮਾਈ_ਬਾਹਰਲਾ);

ਬਾਹਰੀ bool
cksum_pclmul (FILE *fp, uint_fast32_t *ਚੱਕਰੀ_ਜਾਂਚ_ਬਾਹਰਲਾ, intmax_t *ਲੰਮਾਈ_ਬਾਹਰਲਾ);

ਬਾਹਰੀ bool
cksum_avx2 (FILE *fp, uint_fast32_t *ਚੱਕਰੀ_ਜਾਂਚ_ਬਾਹਰਲਾ, intmax_t *ਲੰਮਾਈ_ਬਾਹਰਲਾ);

ਬਾਹਰੀ bool
cksum_avx512 (FILE *fp, uint_fast32_t *ਚੱਕਰੀ_ਜਾਂਚ_ਬਾਹਰਲਾ, intmax_t *ਲੰਮਾਈ_ਬਾਹਰਲਾ);

ਬਾਹਰੀ uint_fast32_t ਅਚਲ crctab[8][256];

#ਜੇ_ਅੰਤ
