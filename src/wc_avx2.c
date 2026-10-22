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

#شامل <config.h>

#شامل "wc.h"
#شامل "system.h"
#شامل "ioblksize.h"

#شامل <x86intrin.h>

/* Read FD and return a summary.  */
بیرونی ساختار wc_سطرها
wc_سطرها_avx2 (صحیح fd)
{
  intmax_t سطرها = 0;
  intmax_t bytes = 0;

  __m256i endlines = _mm256_set1_epi8 ('\n');

  وقتی (true)
    {
       __m256i avx_buf[IO_BUFSIZE / اندازه (__m256i)];
      ssize_t bytes_read = اقرأ (fd, avx_buf, اندازه avx_buf);
      اگر (bytes_read <= 0)
        بازگشت (ساختار wc_سطرها) { bytes_read == 0 ? 0 : errno, سطرها, bytes };

      bytes += bytes_read;
      __m256i *datap = avx_buf;

      وقتی (bytes_read >= 32)
        {
           __m256i to_match = _mm256_load_si256 (datap);
           __m256i matches = _mm256_cmpeq_epi8 (to_match, endlines);
           صحیح mask = _mm256_movemask_epi8 (matches);
           سطرها += __builtin_popcount (mask);
           datap += 1;
           bytes_read -= 32;
        }

      /* Finish up any left over bytes */
      نویسه *end = (نویسه *) datap + bytes_read;
      برای (نویسه *p = (نویسه *) datap; p < end; p++)
        سطرها += *p == '\n';
    }
}
