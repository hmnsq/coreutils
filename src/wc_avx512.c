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

#ضمن <config.h>

#ضمن "wc.h"
#ضمن "system.h"
#ضمن "ioblksize.h"

#ضمن <x86intrin.h>

/* Read FD and return a summary.  */
خارجي بنية wc_أسطر
wc_أسطر_avx512 (صحيح fd)
{
  intmax_t أسطر = 0;
  intmax_t bytes = 0;

  __m512i endlines = _mm512_set1_epi8 ('\n');

  طالما (true)
    {
       __m512i avx_buf[IO_BUFSIZE / حجم (__m512i)];
      ssize_t bytes_read = اقرأ (fd, avx_buf, حجم avx_buf);
      إذا (bytes_read <= 0)
        أرجع (بنية wc_أسطر) { bytes_read == 0 ? 0 : errno, أسطر, bytes };

      bytes += bytes_read;
      __m512i *datap = avx_buf;

      طالما (bytes_read >= 64)
        {
           __m512i to_match = _mm512_load_si512 (datap);
           long long matches = _mm512_cmpeq_epi8_mask (to_match, endlines);
           أسطر += __builtin_popcountll (matches);
           datap += 1;
           bytes_read -= 64;
        }

      /* Finish up any left over bytes */
      حرف *end = (حرف *) datap + bytes_read;
      لكل (حرف *p = (حرف *) datap; p < end; p++)
        أسطر += *p == '\n';
    }
}
