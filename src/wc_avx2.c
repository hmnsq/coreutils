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
বাহ্যিক কাঠামো wc_সারিগুলি
wc_সারিগুলি_avx2 (পূর্ণসংখ্যা fd)
{
  intmax_t সারিগুলি = 0;
  intmax_t bytes = 0;

  __m256i endlines = _mm256_set1_epi8 ('\n');

  যখন (true)
    {
       __m256i avx_buf[IO_BUFSIZE / আকার (__m256i)];
      ssize_t bytes_read = পঠ (fd, avx_buf, আকার avx_buf);
      যদি (bytes_read <= 0)
        ফেরত (কাঠামো wc_সারিগুলি) { bytes_read == 0 ? 0 : errno, সারিগুলি, bytes };

      bytes += bytes_read;
      __m256i *datap = avx_buf;

      যখন (bytes_read >= 32)
        {
           __m256i to_match = _mm256_load_si256 (datap);
           __m256i matches = _mm256_cmpeq_epi8 (to_match, endlines);
           পূর্ণসংখ্যা mask = _mm256_movemask_epi8 (matches);
           সারিগুলি += __builtin_popcount (mask);
           datap += 1;
           bytes_read -= 32;
        }

      /* Finish up any left over bytes */
      অক্ষর *end = (অক্ষর *) datap + bytes_read;
      জন্য (অক্ষর *p = (অক্ষর *) datap; p < end; p++)
        সারিগুলি += *p == '\n';
    }
}
