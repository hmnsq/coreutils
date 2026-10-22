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

বাহ্যিক bool cksum_debug;

বাহ্যিক পূর্ণসংখ্যা
crc_যোগ_stream (FILE *stream, শূন্য *resstream, intmax_t *দৈর্ঘ্য);

বাহ্যিক পূর্ণসংখ্যা
crc32b_যোগ_stream (FILE *stream, শূন্য *resstream, intmax_t *দৈর্ঘ্য);

বাহ্যিক শূন্য
output_crc (অক্ষর ধ্রুবক *file, পূর্ণসংখ্যা দ্বিমিক_file, শূন্য ধ্রুবক *digest, bool raw,
            bool tagged, unsigned অক্ষর delim, bool args, intmax_t দৈর্ঘ্য)
  _GL_ATTRIBUTE_NONNULL ((3));

বাহ্যিক bool
cksum_vmull (FILE *fp, uint_fast32_t *crc_out, intmax_t *দৈর্ঘ্য_out);

বাহ্যিক bool
cksum_pclmul (FILE *fp, uint_fast32_t *crc_out, intmax_t *দৈর্ঘ্য_out);

বাহ্যিক bool
cksum_avx2 (FILE *fp, uint_fast32_t *crc_out, intmax_t *দৈর্ঘ্য_out);

বাহ্যিক bool
cksum_avx512 (FILE *fp, uint_fast32_t *crc_out, intmax_t *দৈর্ঘ্য_out);

বাহ্যিক uint_fast32_t ধ্রুবক crctab[8][256];

#endif
