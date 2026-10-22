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

#അന്തര്ഭാവ <config.h>

#അന്തര്ഭാവ "wc.h"
#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "ioblksize.h"

#അന്തര്ഭാവ <x86intrin.h>

/* Read FD and return a summary.  */
ബാഹ്യ ഘടന wc_വരികൾ
wc_വരികൾ_avx2 (പൂർണ്ണസംഖ്യ fd)
{
  intmax_t വരികൾ = 0;
  intmax_t bytes = 0;

  __m256i endlines = _mm256_set1_epi8 ('\n');

  ആകുമ്പോൾ (true)
    {
       __m256i avx_buf[IO_BUFSIZE / വലുപ്പം (__m256i)];
      ssize_t bytes_read = പഠ (fd, avx_buf, വലുപ്പം avx_buf);
      എങ്കിൽ (bytes_read <= 0)
        തിരികെ (ഘടന wc_വരികൾ) { bytes_read == 0 ? 0 : errno, വരികൾ, bytes };

      bytes += bytes_read;
      __m256i *datap = avx_buf;

      ആകുമ്പോൾ (bytes_read >= 32)
        {
           __m256i to_match = _mm256_load_si256 (datap);
           __m256i matches = _mm256_cmpeq_epi8 (to_match, endlines);
           പൂർണ്ണസംഖ്യ mask = _mm256_movemask_epi8 (matches);
           വരികൾ += __builtin_popcount (mask);
           datap += 1;
           bytes_read -= 32;
        }

      /* Finish up any left over bytes */
      അക്ഷരം *end = (അക്ഷരം *) datap + bytes_read;
      വേണ്ടി (അക്ഷരം *p = (അക്ഷരം *) datap; p < end; p++)
        വരികൾ += *p == '\n';
    }
}
