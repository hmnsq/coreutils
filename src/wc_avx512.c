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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "wc.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ioblksize.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <x86intrin.h>

/* Read FD and return a summary.  */
ਬਾਹਰੀ ਬਣਤਰ wc_ਸਤਰਾਂ
wc_ਸਤਰਾਂ_avx512 (ਪੂਰਨ fd)
{
  intmax_t ਸਤਰਾਂ = 0;
  intmax_t ਬਾਈਟਾਂ = 0;

  __m512i endlines = _mm512_set1_epi8 ('\n');

  ਜਦੋਂ (true)
    {
       __m512i avx_buf[IO_BUFSIZE / ਆਕਾਰ (__m512i)];
      ssize_t ਬਾਈਟਾਂ_read = ਪਠ (fd, avx_buf, ਆਕਾਰ avx_buf);
      ਜੇ (ਬਾਈਟਾਂ_read <= 0)
        ਵਾਪਸ (ਬਣਤਰ wc_ਸਤਰਾਂ) { ਬਾਈਟਾਂ_read == 0 ? 0 : errno, ਸਤਰਾਂ, ਬਾਈਟਾਂ };

      ਬਾਈਟਾਂ += ਬਾਈਟਾਂ_read;
      __m512i *datap = avx_buf;

      ਜਦੋਂ (ਬਾਈਟਾਂ_read >= 64)
        {
           __m512i ਮੰਜ਼ਿਲ_ਮੇਲ = mm512_ਲੱਦੋ_si512 (datap);
           ਲੰਮਾ ਲੰਮਾ matches = mm512_cmpeq_epi8_ਨਕਾਬ (ਮੰਜ਼ਿਲ_ਮੇਲ, endlines);
           ਸਤਰਾਂ += __builtin_popcountll (matches);
           datap += 1;
           ਬਾਈਟਾਂ_read -= 64;
        }

      /* Finish up any left over bytes */
      ਅੱਖਰ *ਸਿਰਾ = (ਅੱਖਰ *) datap + ਬਾਈਟਾਂ_read;
      ਲਈ (ਅੱਖਰ *p = (ਅੱਖਰ *) datap; p < ਸਿਰਾ; p++)
        ਸਤਰਾਂ += *p == '\n';
    }
}
