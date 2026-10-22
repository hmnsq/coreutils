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

#समावेश <config.h>

#समावेश "wc.h"
#समावेश "system.h"
#समावेश "ioblksize.h"

#समावेश <x86intrin.h>

/* Read FD and return a summary.  */
बाह्य काष्ठा wc_पंक्तियाँ
wc_पंक्तियाँ_avx512 (पूर्णांक fd)
{
  intmax_t पंक्तियाँ = 0;
  intmax_t bytes = 0;

  __m512i endlines = _mm512_set1_epi8 ('\n');

  जबतक (true)
    {
       __m512i avx_buf[IO_BUFSIZE / माप (__m512i)];
      ssize_t bytes_read = पढ़ो (fd, avx_buf, माप avx_buf);
      अगर (bytes_read <= 0)
        वापस (काष्ठा wc_पंक्तियाँ) { bytes_read == 0 ? 0 : त्रुटि_सं, पंक्तियाँ, bytes };

      bytes += bytes_read;
      __m512i *datap = avx_buf;

      जबतक (bytes_read >= 64)
        {
           __m512i to_match = _mm512_load_si512 (datap);
           दीर्घ दीर्घ matches = _mm512_cmpeq_epi8_mask (to_match, endlines);
           पंक्तियाँ += __builtin_popcountll (matches);
           datap += 1;
           bytes_read -= 64;
        }

      /* Finish up any left over bytes */
      अक्षर *end = (अक्षर *) datap + bytes_read;
      क्रम (अक्षर *p = (अक्षर *) datap; p < end; p++)
        पंक्तियाँ += *p == '\n';
    }
}
