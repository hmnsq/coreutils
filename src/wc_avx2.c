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
wc_ओळी_avx2 (पूर्णांक fd)
{
  intmax_t ओळी = 0;
  intmax_t bytes = 0;

  __m256i endlines = _mm256_set1_epi8 ('\n');

  पर्यंत (true)
    {
       __m256i avx_buf[IO_BUFSIZE / आकार (__m256i)];
      ssize_t bytes_read = पठ (fd, avx_buf, आकार avx_buf);
      जर (bytes_read <= 0)
        परत (संरचना wc_ओळी) { bytes_read == 0 ? 0 : त्रुटि_सं, ओळी, bytes };

      bytes += bytes_read;
      __m256i *datap = avx_buf;

      पर्यंत (bytes_read >= 32)
        {
           __m256i to_match = _mm256_load_si256 (datap);
           __m256i matches = _mm256_cmpeq_epi8 (to_match, endlines);
           पूर्णांक mask = _mm256_movemask_epi8 (matches);
           ओळी += __builtin_popcount (mask);
           datap += 1;
           bytes_read -= 32;
        }

      /* Finish up any left over bytes */
      अक्षर *end = (अक्षर *) datap + bytes_read;
      साठी (अक्षर *p = (अक्षर *) datap; p < end; p++)
        ओळी += *p == '\n';
    }
}
