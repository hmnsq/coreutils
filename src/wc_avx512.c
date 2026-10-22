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
خارجي بنية wc_سطرها
wc_سطرها_avx512 (صحيح fd)
{
  intmax_t سطرها = 0;
  intmax_t بایتها = 0;

  __m512i endlines = _mm512_set1_epi8 ('\n');

  طالما (true)
    {
       __m512i avx_buf[IO_BUFSIZE / حجم (__m512i)];
      ssize_t بایتها_read = اقرأ (fd, avx_buf, حجم avx_buf);
      إذا (بایتها_read <= 0)
        أرجع (بنية wc_سطرها) { بایتها_read == 0 ? 0 : errno, سطرها, بایتها };

      بایتها += بایتها_read;
      __m512i *datap = avx_buf;

      طالما (بایتها_read >= 64)
        {
           __m512i مقصد_تطابق = mm512_بارگیری_si512 (datap);
           دراز دراز matches = mm512_cmpeq_epi8_ماسک (مقصد_تطابق, endlines);
           سطرها += __builtin_popcountll (matches);
           datap += 1;
           بایتها_read -= 64;
        }

      /* Finish up any left over bytes */
      حرف *انتها = (حرف *) datap + بایتها_read;
      لكل (حرف *p = (حرف *) datap; p < انتها; p++)
        سطرها += *p == '\n';
    }
}
