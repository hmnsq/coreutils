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

बाह्य bool cksum_debug;

बाह्य पूर्णाङ्क
crc_sum_stream (खाता *stream, शून्य *resstream, intmax_t *length);

बाह्य पूर्णाङ्क
crc32b_sum_stream (खाता *stream, शून्य *resstream, intmax_t *length);

बाह्य शून्य
output_crc (वर्ण अचर *file, पूर्णाङ्क द्विआधारी_file, शून्य अचर *digest, bool raw,
            bool tagged, अचिन्हित वर्ण delim, bool args, intmax_t length)
  _GL_ATTRIBUTE_NONNULL ((3));

बाह्य bool
cksum_vmull (खाता *fp, uint_fast32_t *crc_out, intmax_t *length_out);

बाह्य bool
cksum_pclmul (खाता *fp, uint_fast32_t *crc_out, intmax_t *length_out);

बाह्य bool
cksum_avx2 (खाता *fp, uint_fast32_t *crc_out, intmax_t *length_out);

बाह्य bool
cksum_avx512 (खाता *fp, uint_fast32_t *crc_out, intmax_t *length_out);

बाह्य uint_fast32_t अचर crctab[8][256];

#endif
