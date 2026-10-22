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

#अन्तर्भाव <config.h>

#अन्तर्भाव "wc.h"
#अन्तर्भाव "system.h"
#अन्तर्भाव "ioblksize.h"

#अन्तर्भाव <x86intrin.h>

/* Read FD and return a summary.  */
बाह्य संरचना wc_पङ्क्तयः
wc_पङ्क्तयः_avx512 (पूर्णाङ्क fd)
{
  intmax_t पङ्क्तयः = 0;
  intmax_t अष्टकानि = 0;

  __m512i endlines = _mm512_set1_epi8 ('\n');

  यावत् (true)
    {
       __m512i avx_buf[IO_BUFSIZE / परिमाण (__m512i)];
      ssize_t अष्टकानि_read = पठ (fd, avx_buf, परिमाण avx_buf);
      यदि (अष्टकानि_read <= 0)
        प्रत्यागम (संरचना wc_पङ्क्तयः) { अष्टकानि_read == 0 ? 0 : errno, पङ्क्तयः, अष्टकानि };

      अष्टकानि += अष्टकानि_read;
      __m512i *datap = avx_buf;

      यावत् (अष्टकानि_read >= 64)
        {
           __m512i गन्तव्यम्_मेलनम् = mm512_आरोपय_si512 (datap);
           दीर्घ दीर्घ matches = mm512_cmpeq_epi8_आवरणम् (गन्तव्यम्_मेलनम्, endlines);
           पङ्क्तयः += __builtin_popcountll (matches);
           datap += 1;
           अष्टकानि_read -= 64;
        }

      /* Finish up any left over bytes */
      वर्ण *अन्तः = (वर्ण *) datap + अष्टकानि_read;
      प्रति (वर्ण *p = (वर्ण *) datap; p < अन्तः; p++)
        पङ्क्तयः += *p == '\n';
    }
}
