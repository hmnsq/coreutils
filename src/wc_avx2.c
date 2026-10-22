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

#ಅನ್ತರ್ಭಾವ <config.h>

#ಅನ್ತರ್ಭಾವ "wc.h"
#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "ioblksize.h"

#ಅನ್ತರ್ಭಾವ <x86intrin.h>

/* Read FD and return a summary.  */
ಬಾಹ್ಯ ರಚನೆ wc_ಸಾಲುಗಳು
wc_ಸಾಲುಗಳು_avx2 (ಪೂರ್ಣಾಂಕ fd)
{
  intmax_t ಸಾಲುಗಳು = 0;
  intmax_t ಬೈಟುಗಳು = 0;

  __m256i endlines = _mm256_set1_epi8 ('\n');

  ಆಗ (true)
    {
       __m256i avx_buf[IO_BUFSIZE / ಗಾತ್ರ (__m256i)];
      ssize_t ಬೈಟುಗಳು_read = ಪಠ (fd, avx_buf, ಗಾತ್ರ avx_buf);
      ಆದರೆ (ಬೈಟುಗಳು_read <= 0)
        ಹಿಂತಿರುಗಿ (ರಚನೆ wc_ಸಾಲುಗಳು) { ಬೈಟುಗಳು_read == 0 ? 0 : errno, ಸಾಲುಗಳು, ಬೈಟುಗಳು };

      ಬೈಟುಗಳು += ಬೈಟುಗಳು_read;
      __m256i *datap = avx_buf;

      ಆಗ (ಬೈಟುಗಳು_read >= 32)
        {
           __m256i ಗಮ್ಯ_ಹೊಂದಿಕೆ = mm256_ತುಂಬು_si256 (datap);
           __m256i matches = _mm256_cmpeq_epi8 (ಗಮ್ಯ_ಹೊಂದಿಕೆ, endlines);
           ಪೂರ್ಣಾಂಕ ಮುಖವಾಡ = _mm256_movemask_epi8 (matches);
           ಸಾಲುಗಳು += __builtin_popcount (ಮುಖವಾಡ);
           datap += 1;
           ಬೈಟುಗಳು_read -= 32;
        }

      /* Finish up any left over bytes */
      ಅಕ್ಷರ *ಕೊನೆ = (ಅಕ್ಷರ *) datap + ಬೈಟುಗಳು_read;
      ಗಾಗಿ (ಅಕ್ಷರ *p = (ಅಕ್ಷರ *) datap; p < ಕೊನೆ; p++)
        ಸಾಲುಗಳು += *p == '\n';
    }
}
