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

#समावेश <config.h>

#समावेश "wc.h"
#समावेश "system.h"
#समावेश "ioblksize.h"

#समावेश <x86intrin.h>

/* Read FD and return a summary.  */
बाह्य काष्ठा wc_पंक्तियाँ
wc_पंक्तियाँ_avx2 (पूर्णांक fd)
{
  intmax_t पंक्तियाँ = 0;
  intmax_t bytes = 0;

  __m256i endlines = _mm256_set1_epi8 ('\n');

  जबतक (true)
    {
       __m256i avx_buf[IO_BUFSIZE / माप (__m256i)];
      ssize_t bytes_read = पढ़ो (fd, avx_buf, माप avx_buf);
      अगर (bytes_read <= 0)
        वापस (काष्ठा wc_पंक्तियाँ) { bytes_read == 0 ? 0 : त्रुटि_सं, पंक्तियाँ, bytes };

      bytes += bytes_read;
      __m256i *datap = avx_buf;

      जबतक (bytes_read >= 32)
        {
           __m256i to_match = _mm256_load_si256 (datap);
           __m256i matches = _mm256_cmpeq_epi8 (to_match, endlines);
           पूर्णांक mask = _mm256_movemask_epi8 (matches);
           पंक्तियाँ += __builtin_popcount (mask);
           datap += 1;
           bytes_read -= 32;
        }

      /* Finish up any left over bytes */
      अक्षर *end = (अक्षर *) datap + bytes_read;
      क्रम (अक्षर *p = (अक्षर *) datap; p < end; p++)
        पंक्तियाँ += *p == '\n';
    }
}
