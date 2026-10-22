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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "wc.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ioblksize.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <x86intrin.h>

/* Read FD and return a summary.  */
ਬਾਹਰੀ ਬਣਤਰ wc_ਸਤਰਾਂ
wc_ਸਤਰਾਂ_avx2 (ਪੂਰਨ fd)
{
  intmax_t ਸਤਰਾਂ = 0;
  intmax_t ਬਾਈਟਾਂ = 0;

  __m256i endlines = _mm256_set1_epi8 ('\n');

  ਜਦੋਂ (true)
    {
       __m256i avx_buf[IO_BUFSIZE / ਆਕਾਰ (__m256i)];
      ssize_t ਬਾਈਟਾਂ_read = ਪਠ (fd, avx_buf, ਆਕਾਰ avx_buf);
      ਜੇ (ਬਾਈਟਾਂ_read <= 0)
        ਵਾਪਸ (ਬਣਤਰ wc_ਸਤਰਾਂ) { ਬਾਈਟਾਂ_read == 0 ? 0 : errno, ਸਤਰਾਂ, ਬਾਈਟਾਂ };

      ਬਾਈਟਾਂ += ਬਾਈਟਾਂ_read;
      __m256i *datap = avx_buf;

      ਜਦੋਂ (ਬਾਈਟਾਂ_read >= 32)
        {
           __m256i ਮੰਜ਼ਿਲ_ਮੇਲ = mm256_ਲੱਦੋ_si256 (datap);
           __m256i matches = _mm256_cmpeq_epi8 (ਮੰਜ਼ਿਲ_ਮੇਲ, endlines);
           ਪੂਰਨ ਨਕਾਬ = _mm256_movemask_epi8 (matches);
           ਸਤਰਾਂ += __builtin_popcount (ਨਕਾਬ);
           datap += 1;
           ਬਾਈਟਾਂ_read -= 32;
        }

      /* Finish up any left over bytes */
      ਅੱਖਰ *ਸਿਰਾ = (ਅੱਖਰ *) datap + ਬਾਈਟਾਂ_read;
      ਲਈ (ਅੱਖਰ *p = (ਅੱਖਰ *) datap; p < ਸਿਰਾ; p++)
        ਸਤਰਾਂ += *p == '\n';
    }
}
