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

#ifndef __CKSUM_CRC_H__
# define __CKSUM_CRC_H__

# include <stdint.h>
# include <stdio.h>

بیرونی bool cksum_debug;

بیرونی صحیح
crc_sum_stream (FILE *stream, تهی *resstream, intmax_t *length);

بیرونی صحیح
crc32b_sum_stream (FILE *stream, تهی *resstream, intmax_t *length);

بیرونی تهی
output_crc (نویسه ثابت *file, صحیح دودویی_file, تهی ثابت *digest, bool raw,
            bool tagged, unsigned نویسه delim, bool args, intmax_t length)
  _GL_ATTRIBUTE_NONNULL ((3));

بیرونی bool
cksum_vmull (FILE *fp, uint_fast32_t *crc_out, intmax_t *length_out);

بیرونی bool
cksum_pclmul (FILE *fp, uint_fast32_t *crc_out, intmax_t *length_out);

بیرونی bool
cksum_avx2 (FILE *fp, uint_fast32_t *crc_out, intmax_t *length_out);

بیرونی bool
cksum_avx512 (FILE *fp, uint_fast32_t *crc_out, intmax_t *length_out);

بیرونی uint_fast32_t ثابت crctab[8][256];

#endif
