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
خارجي بنية wc_سطریں
wc_سطریں_avx512 (صحيح fd)
{
  intmax_t سطریں = 0;
  intmax_t بائٹاں = 0;

  __m512i endlines = _mm512_set1_epi8 ('\n');

  طالما (true)
    {
       __m512i avx_buf[IO_BUFSIZE / حجم (__m512i)];
      ssize_t بائٹاں_read = اقرأ (fd, avx_buf, حجم avx_buf);
      إذا (بائٹاں_read <= 0)
        أرجع (بنية wc_سطریں) { بائٹاں_read == 0 ? 0 : errno, سطریں, بائٹاں };

      بائٹاں += بائٹاں_read;
      __m512i *datap = avx_buf;

      طالما (بائٹاں_read >= 64)
        {
           __m512i منزل_میل = mm512_لدو_si512 (datap);
           لما لما matches = mm512_cmpeq_epi8_نقاب (منزل_میل, endlines);
           سطریں += __builtin_popcountll (matches);
           datap += 1;
           بائٹاں_read -= 64;
        }

      /* Finish up any left over bytes */
      حرف *سرا = (حرف *) datap + بائٹاں_read;
      لكل (حرف *p = (حرف *) datap; p < سرا; p++)
        سطریں += *p == '\n';
    }
}
