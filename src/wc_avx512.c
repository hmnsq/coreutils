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

#అన్తర్భావ <config.h>

#అన్తర్భావ "wc.h"
#అన్తర్భావ "system.h"
#అన్తర్భావ "ioblksize.h"

#అన్తర్భావ <x86intrin.h>

/* Read FD and return a summary.  */
బాహ్య నిర్మాణం wc_పంక్తులు
wc_పంక్తులు_avx512 (పూర్ణాంకం fd)
{
  intmax_t పంక్తులు = 0;
  intmax_t బైట్లు = 0;

  __m512i endlines = _mm512_set1_epi8 ('\n');

  అయితే (true)
    {
       __m512i avx_buf[IO_BUFSIZE / పరిమాణం (__m512i)];
      ssize_t బైట్లు_read = పఠ (fd, avx_buf, పరిమాణం avx_buf);
      ఒకవేళ (బైట్లు_read <= 0)
        తిరిగి (నిర్మాణం wc_పంక్తులు) { బైట్లు_read == 0 ? 0 : errno, పంక్తులు, బైట్లు };

      బైట్లు += బైట్లు_read;
      __m512i *datap = avx_buf;

      అయితే (బైట్లు_read >= 64)
        {
           __m512i గమ్యం_సరిపోలిక = mm512_ఎక్కించు_si512 (datap);
           దీర్ఘ దీర్ఘ matches = mm512_cmpeq_epi8_ముసుగు (గమ్యం_సరిపోలిక, endlines);
           పంక్తులు += __builtin_popcountll (matches);
           datap += 1;
           బైట్లు_read -= 64;
        }

      /* Finish up any left over bytes */
      అక్షరం *చివర = (అక్షరం *) datap + బైట్లు_read;
      కొరకు (అక్షరం *p = (అక్షరం *) datap; p < చివర; p++)
        పంక్తులు += *p == '\n';
    }
}
