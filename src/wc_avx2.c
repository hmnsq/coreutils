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

#ضمن <config.h>

#ضمن "wc.h"
#ضمن "system.h"
#ضمن "ioblksize.h"

#ضمن <x86intrin.h>

/* Read FD and return a summary.  */
خارجي بنية wc_سٽون
wc_سٽون_avx2 (صحيح fd)
{
  intmax_t سٽون = 0;
  intmax_t بائيٽون = 0;

  __m256i endlines = _mm256_set1_epi8 ('\n');

  طالما (true)
    {
       __m256i avx_buf[IO_BUFSIZE / حجم (__m256i)];
      ssize_t بائيٽون_read = اقرأ (fd, avx_buf, حجم avx_buf);
      إذا (بائيٽون_read <= 0)
        أرجع (بنية wc_سٽون) { بائيٽون_read == 0 ? 0 : errno, سٽون, بائيٽون };

      بائيٽون += بائيٽون_read;
      __m256i *datap = avx_buf;

      طالما (بائيٽون_read >= 32)
        {
           __m256i منزل_ميلاپ = mm256_ڀريو_si256 (datap);
           __m256i matches = _mm256_cmpeq_epi8 (منزل_ميلاپ, endlines);
           صحيح نقاب = _mm256_movemask_epi8 (matches);
           سٽون += __builtin_popcount (نقاب);
           datap += 1;
           بائيٽون_read -= 32;
        }

      /* Finish up any left over bytes */
      حرف *پڇاڙي = (حرف *) datap + بائيٽون_read;
      لكل (حرف *p = (حرف *) datap; p < پڇاڙي; p++)
        سٽون += *p == '\n';
    }
}
