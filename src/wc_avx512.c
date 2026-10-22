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

#شامل <config.h>

#شامل "wc.h"
#شامل "system.h"
#شامل "ioblksize.h"

#شامل <x86intrin.h>

/* Read FD and return a summary.  */
خارجی ساخت wc_سطریں
wc_سطریں_avx512 (صحیح fd)
{
  intmax_t سطریں = 0;
  intmax_t بائٹیں = 0;

  __m512i endlines = _mm512_set1_epi8 ('\n');

  جب (true)
    {
       __m512i avx_buf[IO_BUFSIZE / حجم (__m512i)];
      ssize_t بائٹیں_read = اقرأ (fd, avx_buf, حجم avx_buf);
      اگر (بائٹیں_read <= 0)
        واپس (ساخت wc_سطریں) { بائٹیں_read == 0 ? 0 : errno, سطریں, بائٹیں };

      بائٹیں += بائٹیں_read;
      __m512i *datap = avx_buf;

      جب (بائٹیں_read >= 64)
        {
           __m512i مقصد_مطابقت = mm512_بھرو_si512 (datap);
           طویل طویل matches = mm512_cmpeq_epi8_نقاب (مقصد_مطابقت, endlines);
           سطریں += __builtin_popcountll (matches);
           datap += 1;
           بائٹیں_read -= 64;
        }

      /* Finish up any left over bytes */
      حرف *انتہا = (حرف *) datap + بائٹیں_read;
      برائے (حرف *p = (حرف *) datap; p < انتہا; p++)
        سطریں += *p == '\n';
    }
}
