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

#ಅನ್ತರ್ಭಾವ <config.h>

#ಅನ್ತರ್ಭಾವ "wc.h"
#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "ioblksize.h"

#ಅನ್ತರ್ಭಾವ <x86intrin.h>

/* Read FD and return a summary.  */
ಬಾಹ್ಯ ರಚನೆ wc_ಸಾಲುಗಳು
wc_ಸಾಲುಗಳು_avx512 (ಪೂರ್ಣಾಂಕ fd)
{
  intmax_t ಸಾಲುಗಳು = 0;
  intmax_t ಬೈಟುಗಳು = 0;

  __m512i endlines = _mm512_set1_epi8 ('\n');

  ಆಗ (true)
    {
       __m512i avx_buf[IO_BUFSIZE / ಗಾತ್ರ (__m512i)];
      ssize_t ಬೈಟುಗಳು_read = ಪಠ (fd, avx_buf, ಗಾತ್ರ avx_buf);
      ಆದರೆ (ಬೈಟುಗಳು_read <= 0)
        ಹಿಂತಿರುಗಿ (ರಚನೆ wc_ಸಾಲುಗಳು) { ಬೈಟುಗಳು_read == 0 ? 0 : errno, ಸಾಲುಗಳು, ಬೈಟುಗಳು };

      ಬೈಟುಗಳು += ಬೈಟುಗಳು_read;
      __m512i *datap = avx_buf;

      ಆಗ (ಬೈಟುಗಳು_read >= 64)
        {
           __m512i ಗಮ್ಯ_ಹೊಂದಿಕೆ = mm512_ತುಂಬು_si512 (datap);
           ದೀರ್ಘ ದೀರ್ಘ matches = mm512_cmpeq_epi8_ಮುಖವಾಡ (ಗಮ್ಯ_ಹೊಂದಿಕೆ, endlines);
           ಸಾಲುಗಳು += __builtin_popcountll (matches);
           datap += 1;
           ಬೈಟುಗಳು_read -= 64;
        }

      /* Finish up any left over bytes */
      ಅಕ್ಷರ *ಕೊನೆ = (ಅಕ್ಷರ *) datap + ಬೈಟುಗಳು_read;
      ಗಾಗಿ (ಅಕ್ಷರ *p = (ಅಕ್ಷರ *) datap; p < ಕೊನೆ; p++)
        ಸಾಲುಗಳು += *p == '\n';
    }
}
