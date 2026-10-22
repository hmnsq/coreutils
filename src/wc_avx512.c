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

#અન્તર્ભાવ <config.h>

#અન્તર્ભાવ "wc.h"
#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "ioblksize.h"

#અન્તર્ભાવ <x86intrin.h>

/* Read FD and return a summary.  */
બાહ્ય સંરચના wc_લીટીઓ
wc_લીટીઓ_avx512 (પૂર્ણાંક fd)
{
  intmax_t લીટીઓ = 0;
  intmax_t bytes = 0;

  __m512i endlines = _mm512_set1_epi8 ('\n');

  જ્યારે (true)
    {
       __m512i avx_buf[IO_BUFSIZE / કદ (__m512i)];
      ssize_t bytes_read = પઠ (fd, avx_buf, કદ avx_buf);
      જો (bytes_read <= 0)
        પાછા (સંરચના wc_લીટીઓ) { bytes_read == 0 ? 0 : errno, લીટીઓ, bytes };

      bytes += bytes_read;
      __m512i *datap = avx_buf;

      જ્યારે (bytes_read >= 64)
        {
           __m512i to_match = _mm512_load_si512 (datap);
           long long matches = _mm512_cmpeq_epi8_mask (to_match, endlines);
           લીટીઓ += __builtin_popcountll (matches);
           datap += 1;
           bytes_read -= 64;
        }

      /* Finish up any left over bytes */
      અક્ષર *end = (અક્ષર *) datap + bytes_read;
      માટે (અક્ષર *p = (અક્ષર *) datap; p < end; p++)
        લીટીઓ += *p == '\n';
    }
}
