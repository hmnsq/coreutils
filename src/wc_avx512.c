/* wc_avx512 - Count the number of newlines with avx512 instructions.
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
wc_வரிகள்_avx512 (முழு fd)
{
  intmax_t வரிகள் = 0;
  intmax_t bytes = 0;

  __m512i endlines = _mm512_set1_epi8 ('\n');

  போது (true)
    {
       __m512i avx_buf[IO_BUFSIZE / அளவு (__m512i)];
      ssize_t bytes_read = பட (fd, avx_buf, அளவு avx_buf);
      எனில் (bytes_read <= 0)
        திரும்பு (கட்டமைப்பு wc_வரிகள்) { bytes_read == 0 ? 0 : errno, வரிகள், bytes };

      bytes += bytes_read;
      __m512i *datap = avx_buf;

      போது (bytes_read >= 64)
        {
           __m512i to_match = _mm512_load_si512 (datap);
           long long matches = _mm512_cmpeq_epi8_mask (to_match, endlines);
           வரிகள் += __builtin_popcountll (matches);
           datap += 1;
           bytes_read -= 64;
        }

      /* Finish up any left over bytes */
      எழுத்து *end = (எழுத்து *) datap + bytes_read;
      க்கு (எழுத்து *p = (எழுத்து *) datap; p < end; p++)
        வரிகள் += *p == '\n';
    }
}
