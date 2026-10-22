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
ಬಾಹ್ಯ ಸಂರಚನಾ wc_ಸಾಲುಗಳು
wc_ಸಾಲುಗಳು_avx512 (ಪೂರ್ಣಾಙ್ಕ fd)
{
  intmax_t ಸಾಲುಗಳು = 0;
  intmax_t bytes = 0;

  __m512i endlines = _mm512_set1_epi8 ('\n');

  ಯಾವತ್ (true)
    {
       __m512i avx_buf[IO_BUFSIZE / ಪರಿಮಾಣ (__m512i)];
      ssize_t bytes_read = ಪಠ (fd, avx_buf, ಪರಿಮಾಣ avx_buf);
      ಯದಿ (bytes_read <= 0)
        ಪ್ರತ್ಯಾಗಮ (ಸಂರಚನಾ wc_ಸಾಲುಗಳು) { bytes_read == 0 ? 0 : errno, ಸಾಲುಗಳು, bytes };

      bytes += bytes_read;
      __m512i *datap = avx_buf;

      ಯಾವತ್ (bytes_read >= 64)
        {
           __m512i to_match = _mm512_load_si512 (datap);
           long long matches = _mm512_cmpeq_epi8_mask (to_match, endlines);
           ಸಾಲುಗಳು += __builtin_popcountll (matches);
           datap += 1;
           bytes_read -= 64;
        }

      /* Finish up any left over bytes */
      ವರ್ಣ *end = (ವರ್ಣ *) datap + bytes_read;
      ಪ್ರತಿ (ವರ್ಣ *p = (ವರ್ಣ *) datap; p < end; p++)
        ಸಾಲುಗಳು += *p == '\n';
    }
}
