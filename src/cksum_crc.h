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

#अगर_अघोषित __CKSUM_CRC_H__
# define __CKSUM_CRC_H__

# include <stdint.h>
# include <stdio.h>

बाह्य bool cksum_debug;

बाह्य पूर्णांक
crc_योग_stream (खाता *stream, व्योम *resstream, intmax_t *लंबाई);

बाह्य पूर्णांक
crc32b_योग_stream (खाता *stream, व्योम *resstream, intmax_t *लंबाई);

बाह्य व्योम
output_crc (अक्षर स्थिर *file, पूर्णांक द्विआधारी_file, व्योम स्थिर *digest, bool raw,
            bool tagged, अचिन्हित अक्षर delim, bool args, intmax_t लंबाई)
  _GL_ATTRIBUTE_NONNULL ((3));

बाह्य bool
cksum_vmull (खाता *fp, uint_fast32_t *crc_out, intmax_t *लंबाई_out);

बाह्य bool
cksum_pclmul (खाता *fp, uint_fast32_t *crc_out, intmax_t *लंबाई_out);

बाह्य bool
cksum_avx2 (खाता *fp, uint_fast32_t *crc_out, intmax_t *लंबाई_out);

बाह्य bool
cksum_avx512 (खाता *fp, uint_fast32_t *crc_out, intmax_t *लंबाई_out);

बाह्य uint_fast32_t स्थिर crctab[8][256];

#पूर्ण_अगर
