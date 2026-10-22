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

#ଅନ୍ତର୍ଭାଵ <config.h>

#ଅନ୍ତର୍ଭାଵ "wc.h"
#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "ioblksize.h"

#ଅନ୍ତର୍ଭାଵ <x86intrin.h>

/* Read FD and return a summary.  */
ବାହ୍ୟ ସଂରଚନା wc_ଧାଡ଼ିଗୁଡ଼ିକ
wc_ଧାଡ଼ିଗୁଡ଼ିକ_avx512 (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd)
{
  intmax_t ଧାଡ଼ିଗୁଡ଼ିକ = 0;
  intmax_t bytes = 0;

  __m512i endlines = _mm512_set1_epi8 ('\n');

  ଯେତେବେଳେ (true)
    {
       __m512i avx_buf[IO_BUFSIZE / ଆକାର (__m512i)];
      ssize_t bytes_read = ପଠ (fd, avx_buf, ଆକାର avx_buf);
      ଯଦି (bytes_read <= 0)
        ଫେରନ୍ତୁ (ସଂରଚନା wc_ଧାଡ଼ିଗୁଡ଼ିକ) { bytes_read == 0 ? 0 : errno, ଧାଡ଼ିଗୁଡ଼ିକ, bytes };

      bytes += bytes_read;
      __m512i *datap = avx_buf;

      ଯେତେବେଳେ (bytes_read >= 64)
        {
           __m512i to_match = _mm512_load_si512 (datap);
           long long matches = _mm512_cmpeq_epi8_mask (to_match, endlines);
           ଧାଡ଼ିଗୁଡ଼ିକ += __builtin_popcountll (matches);
           datap += 1;
           bytes_read -= 64;
        }

      /* Finish up any left over bytes */
      ଅକ୍ଷର *end = (ଅକ୍ଷର *) datap + bytes_read;
      ପାଇଁ (ଅକ୍ଷର *p = (ଅକ୍ଷର *) datap; p < end; p++)
        ଧାଡ଼ିଗୁଡ଼ିକ += *p == '\n';
    }
}
