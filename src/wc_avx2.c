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

#অন্তর্ভাব <config.h>

#অন্তর্ভাব "wc.h"
#অন্তর্ভাব "system.h"
#অন্তর্ভাব "ioblksize.h"

#অন্তর্ভাব <x86intrin.h>

/* Read FD and return a summary.  */
বাহ্য সংরচনা wc_সারিগুলি
wc_সারিগুলি_avx2 (পূর্ণাঙ্ক fd)
{
  intmax_t সারিগুলি = 0;
  intmax_t bytes = 0;

  __m256i endlines = _mm256_set1_epi8 ('\n');

  যাবত্ (true)
    {
       __m256i avx_buf[IO_BUFSIZE / পরিমাণ (__m256i)];
      ssize_t bytes_read = পঠ (fd, avx_buf, পরিমাণ avx_buf);
      যদি (bytes_read <= 0)
        প্রত্যাগম (সংরচনা wc_সারিগুলি) { bytes_read == 0 ? 0 : errno, সারিগুলি, bytes };

      bytes += bytes_read;
      __m256i *datap = avx_buf;

      যাবত্ (bytes_read >= 32)
        {
           __m256i to_match = _mm256_load_si256 (datap);
           __m256i matches = _mm256_cmpeq_epi8 (to_match, endlines);
           পূর্ণাঙ্ক mask = _mm256_movemask_epi8 (matches);
           সারিগুলি += __builtin_popcount (mask);
           datap += 1;
           bytes_read -= 32;
        }

      /* Finish up any left over bytes */
      বর্ণ *end = (বর্ণ *) datap + bytes_read;
      প্রতি (বর্ণ *p = (বর্ণ *) datap; p < end; p++)
        সারিগুলি += *p == '\n';
    }
}
