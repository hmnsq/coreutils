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

#यदि_अपरिभाषितम् __CKSUM_CRC_H__
# define __CKSUM_CRC_H__

# include <stdint.h>
# include <stdio.h>

बाह्य bool cksum_debug;

बाह्य पूर्णाङ्क
crc_योग_प्रवाहः (FILE *प्रवाहः, शून्य *resstream, intmax_t *दीर्घता);

बाह्य पूर्णाङ्क
crc32b_योग_प्रवाहः (FILE *प्रवाहः, शून्य *resstream, intmax_t *दीर्घता);

बाह्य शून्य
output_crc (वर्ण ध्रुव *file, पूर्णाङ्क द्विमान_file, शून्य ध्रुव *digest, bool raw,
            bool tagged, अचिह्नित वर्ण delim, bool तर्काः, intmax_t दीर्घता)
  _GL_ATTRIBUTE_NONNULL ((3));

बाह्य bool
cksum_vmull (FILE *fp, uint_fast32_t *crc_बहिर्गमनम्, intmax_t *दीर्घता_बहिर्गमनम्);

बाह्य bool
cksum_pclmul (FILE *fp, uint_fast32_t *crc_बहिर्गमनम्, intmax_t *दीर्घता_बहिर्गमनम्);

बाह्य bool
cksum_avx2 (FILE *fp, uint_fast32_t *crc_बहिर्गमनम्, intmax_t *दीर्घता_बहिर्गमनम्);

बाह्य bool
cksum_avx512 (FILE *fp, uint_fast32_t *crc_बहिर्गमनम्, intmax_t *दीर्घता_बहिर्गमनम्);

बाह्य uint_fast32_t ध्रुव crctab[8][256];

#यदि_अन्तः
