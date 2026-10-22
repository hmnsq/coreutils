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

ಬಾಹ್ಯ bool cksum_debug;

ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ
crc_sum_stream (FILE *stream, ಶೂನ್ಯ *resstream, intmax_t *length);

ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ
crc32b_sum_stream (FILE *stream, ಶೂನ್ಯ *resstream, intmax_t *length);

ಬಾಹ್ಯ ಶೂನ್ಯ
output_crc (ವರ್ಣ ಧ್ರುವ *file, ಪೂರ್ಣಾಙ್ಕ ದ್ವಿಮಾನ_file, ಶೂನ್ಯ ಧ್ರುವ *digest, bool raw,
            bool tagged, unsigned ವರ್ಣ delim, bool args, intmax_t length)
  _GL_ATTRIBUTE_NONNULL ((3));

ಬಾಹ್ಯ bool
cksum_vmull (FILE *fp, uint_fast32_t *crc_out, intmax_t *length_out);

ಬಾಹ್ಯ bool
cksum_pclmul (FILE *fp, uint_fast32_t *crc_out, intmax_t *length_out);

ಬಾಹ್ಯ bool
cksum_avx2 (FILE *fp, uint_fast32_t *crc_out, intmax_t *length_out);

ಬಾಹ್ಯ bool
cksum_avx512 (FILE *fp, uint_fast32_t *crc_out, intmax_t *length_out);

ಬಾಹ್ಯ uint_fast32_t ಧ್ರುವ crctab[8][256];

#endif
