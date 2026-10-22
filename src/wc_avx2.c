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

#अन्तर्भाव <config.h>

#अन्तर्भाव "wc.h"
#अन्तर्भाव "system.h"
#अन्तर्भाव "ioblksize.h"

#अन्तर्भाव <x86intrin.h>

/* Read FD and return a summary.  */
बाह्य संरचना wc_ओळी
wc_ओळी_avx2 (पूर्णाङ्क fd)
{
  intmax_t ओळी = 0;
  intmax_t bytes = 0;

  __m256i endlines = _mm256_set1_epi8 ('\n');

  यावत् (true)
    {
       __m256i avx_buf[IO_BUFSIZE / परिमाण (__m256i)];
      ssize_t bytes_read = पठ (fd, avx_buf, परिमाण avx_buf);
      यदि (bytes_read <= 0)
        प्रत्यागम (संरचना wc_ओळी) { bytes_read == 0 ? 0 : errno, ओळी, bytes };

      bytes += bytes_read;
      __m256i *datap = avx_buf;

      यावत् (bytes_read >= 32)
        {
           __m256i to_match = _mm256_load_si256 (datap);
           __m256i matches = _mm256_cmpeq_epi8 (to_match, endlines);
           पूर्णाङ्क mask = _mm256_movemask_epi8 (matches);
           ओळी += __builtin_popcount (mask);
           datap += 1;
           bytes_read -= 32;
        }

      /* Finish up any left over bytes */
      वर्ण *end = (वर्ण *) datap + bytes_read;
      प्रति (वर्ण *p = (वर्ण *) datap; p < end; p++)
        ओळी += *p == '\n';
    }
}
