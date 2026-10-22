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

#অন্তর্ভাৱ <config.h>

#অন্তর্ভাৱ "wc.h"
#অন্তর্ভাৱ "system.h"
#অন্তর্ভাৱ "ioblksize.h"

#অন্তর্ভাৱ <x86intrin.h>

/* Read FD and return a summary.  */
বাহ্য সংরচনা wc_সাৰিগুলি
wc_সাৰিগুলি_avx2 (পূর্ণাঙ্ক fd)
{
  intmax_t সাৰিগুলি = 0;
  intmax_t bytes = 0;

  __m256i endlines = _mm256_set1_epi8 ('\n');

  যেতিয়ালৈ (true)
    {
       __m256i avx_buf[IO_BUFSIZE / পরিমাণ (__m256i)];
      ssize_t bytes_read = পঠ (fd, avx_buf, পরিমাণ avx_buf);
      যদি (bytes_read <= 0)
        ঘূৰাই (সংরচনা wc_সাৰিগুলি) { bytes_read == 0 ? 0 : errno, সাৰিগুলি, bytes };

      bytes += bytes_read;
      __m256i *datap = avx_buf;

      যেতিয়ালৈ (bytes_read >= 32)
        {
           __m256i to_match = _mm256_load_si256 (datap);
           __m256i matches = _mm256_cmpeq_epi8 (to_match, endlines);
           পূর্ণাঙ্ক mask = _mm256_movemask_epi8 (matches);
           সাৰিগুলি += __builtin_popcount (mask);
           datap += 1;
           bytes_read -= 32;
        }

      /* Finish up any left over bytes */
      ৱর্ণ *end = (ৱর্ণ *) datap + bytes_read;
      বাবে (ৱর্ণ *p = (ৱর্ণ *) datap; p < end; p++)
        সাৰিগুলি += *p == '\n';
    }
}
