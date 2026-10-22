/* wc_avx - Count the number of newlines with avx2 instructions.
   Copyright (C) 2021-2026 Free Software Foundation, Inc.

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

#அந்தர்பாவ <config.h>

#அந்தர்பாவ "wc.h"
#அந்தர்பாவ "system.h"
#அந்தர்பாவ "ioblksize.h"

#அந்தர்பாவ <x86intrin.h>

/* Read FD and return a summary.  */
வெளி கட்டமைப்பு wc_வரிகள்
wc_வரிகள்_avx2 (முழு fd)
{
  intmax_t வரிகள் = 0;
  intmax_t bytes = 0;

  __m256i endlines = _mm256_set1_epi8 ('\n');

  போது (true)
    {
       __m256i avx_buf[IO_BUFSIZE / அளவு (__m256i)];
      ssize_t bytes_read = பட (fd, avx_buf, அளவு avx_buf);
      எனில் (bytes_read <= 0)
        திரும்பு (கட்டமைப்பு wc_வரிகள்) { bytes_read == 0 ? 0 : errno, வரிகள், bytes };

      bytes += bytes_read;
      __m256i *datap = avx_buf;

      போது (bytes_read >= 32)
        {
           __m256i to_match = _mm256_load_si256 (datap);
           __m256i matches = _mm256_cmpeq_epi8 (to_match, endlines);
           முழு mask = _mm256_movemask_epi8 (matches);
           வரிகள் += __builtin_popcount (mask);
           datap += 1;
           bytes_read -= 32;
        }

      /* Finish up any left over bytes */
      எழுத்து *end = (எழுத்து *) datap + bytes_read;
      க்கு (எழுத்து *p = (எழுத்து *) datap; p < end; p++)
        வரிகள் += *p == '\n';
    }
}
