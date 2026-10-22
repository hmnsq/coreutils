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

خارجي bool cksum_debug;

خارجي صحيح
crc_sum_stream (FILE *stream, فراغ *resstream, intmax_t *length);

خارجي صحيح
crc32b_sum_stream (FILE *stream, فراغ *resstream, intmax_t *length);

خارجي فراغ
output_crc (حرف ثابت *file, صحيح نبيو_file, فراغ ثابت *digest, bool raw,
            bool tagged, unsigned حرف delim, bool حجوجي, intmax_t length)
  _GL_ATTRIBUTE_NONNULL ((3));

خارجي bool
cksum_vmull (FILE *fp, uint_fast32_t *crc_out, intmax_t *length_out);

خارجي bool
cksum_pclmul (FILE *fp, uint_fast32_t *crc_out, intmax_t *length_out);

خارجي bool
cksum_avx2 (FILE *fp, uint_fast32_t *crc_out, intmax_t *length_out);

خارجي bool
cksum_avx512 (FILE *fp, uint_fast32_t *crc_out, intmax_t *length_out);

خارجي uint_fast32_t ثابت crctab[8][256];

#endif
