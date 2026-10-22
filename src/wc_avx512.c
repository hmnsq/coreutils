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

#অন্তর্ভাব <config.h>

#অন্তর্ভাব "wc.h"
#অন্তর্ভাব "system.h"
#অন্তর্ভাব "ioblksize.h"

#অন্তর্ভাব <x86intrin.h>

/* Read FD and return a summary.  */
বাহ্য সংরচনা wc_সারিগুলি
wc_সারিগুলি_avx512 (পূর্ণাঙ্ক fd)
{
  intmax_t সারিগুলি = 0;
  intmax_t bytes = 0;

  __m512i endlines = _mm512_set1_epi8 ('\n');

  যাবত্ (true)
    {
       __m512i avx_buf[IO_BUFSIZE / পরিমাণ (__m512i)];
      ssize_t bytes_read = পঠ (fd, avx_buf, পরিমাণ avx_buf);
      যদি (bytes_read <= 0)
        প্রত্যাগম (সংরচনা wc_সারিগুলি) { bytes_read == 0 ? 0 : errno, সারিগুলি, bytes };

      bytes += bytes_read;
      __m512i *datap = avx_buf;

      যাবত্ (bytes_read >= 64)
        {
           __m512i to_match = _mm512_load_si512 (datap);
           long long matches = _mm512_cmpeq_epi8_mask (to_match, endlines);
           সারিগুলি += __builtin_popcountll (matches);
           datap += 1;
           bytes_read -= 64;
        }

      /* Finish up any left over bytes */
      বর্ণ *end = (বর্ণ *) datap + bytes_read;
      প্রতি (বর্ণ *p = (বর্ণ *) datap; p < end; p++)
        সারিগুলি += *p == '\n';
    }
}
