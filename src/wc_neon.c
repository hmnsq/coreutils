/* wc_neon - Count the number of newlines with neon instructions.
   Copyright (C) 2026 Free Software Foundation, Inc.

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

/* Written by Collin Funk <collin.funk1@gmail.com>, 2026.  */

#अन्तर्भाव <config.h>

#अन्तर्भाव "wc.h"
#अन्तर्भाव "system.h"
#अन्तर्भाव "ioblksize.h"

#अन्तर्भाव <arm_neon.h>

/* Read FD and return a summary.  */
बाह्य संरचना wc_पङ्क्तिहरू
wc_पङ्क्तिहरू_neon (पूर्णाङ्क fd)
{
  intmax_t पङ्क्तिहरू = 0;
  intmax_t bytes = 0;

  अचर uint8x16_t endlines = vdupq_n_u8 ('\n');

  जबसम्म (true)
    {
      अचिन्हित वर्ण alignas (16) neon_buf[IO_BUFSIZE];
      ssize_t bytes_read = पठ (fd, neon_buf, परिमाण neon_buf);
      यदि (bytes_read <= 0)
        फर्काऊ (संरचना wc_पङ्क्तिहरू) { bytes_read == 0 ? 0 : त्रुटि_सं, पङ्क्तिहरू, bytes };

      bytes += bytes_read;
      अचिन्हित वर्ण *datap = neon_buf;

      जबसम्म (8192 <= bytes_read)
        {
          /* Accumulator.  */
          int8x16_t acc0 = vdupq_n_s8 (0);
          int8x16_t acc1 = vdupq_n_s8 (0);
          int8x16_t acc2 = vdupq_n_s8 (0);
          int8x16_t acc3 = vdupq_n_s8 (0);

          /* Process all 8192 bytes in 64 byte chunks.  */
          लागि (पूर्णाङ्क i = 0; i < 128; ++i)
            {
              /* Load 64 bytes from DATAP.  */
              uint8x16_t v0 = vld1q_u8 (datap);
              uint8x16_t v1 = vld1q_u8 (datap + 16);
              uint8x16_t v2 = vld1q_u8 (datap + 32);
              uint8x16_t v3 = vld1q_u8 (datap + 48);

              /* Bitwise equal with ENDLINES.  We use a reinterpret cast to
                 convert the 0xff if a newline is found into -1.  */
              int8x16_t c0 = vreinterpretq_s8_u8 (vceqq_u8 (v0, endlines));
              int8x16_t c1 = vreinterpretq_s8_u8 (vceqq_u8 (v1, endlines));
              int8x16_t c2 = vreinterpretq_s8_u8 (vceqq_u8 (v2, endlines));
              int8x16_t c3 = vreinterpretq_s8_u8 (vceqq_u8 (v3, endlines));

              /* Increment the accumulator.  */
              acc0 = vaddq_s8 (acc0, c0);
              acc1 = vaddq_s8 (acc1, c1);
              acc2 = vaddq_s8 (acc2, c2);
              acc3 = vaddq_s8 (acc3, c3);

              datap += 64;
            }

          /* Pairwise sum the vectors.  */
          int16x8_t a0 = vpaddlq_s8 (acc0);
          int16x8_t a1 = vpaddlq_s8 (acc1);
          int16x8_t a2 = vpaddlq_s8 (acc2);
          int16x8_t a3 = vpaddlq_s8 (acc3);
          int32x4_t b0 = vpaddlq_s16 (a0);
          int32x4_t b1 = vpaddlq_s16 (a1);
          int32x4_t b2 = vpaddlq_s16 (a2);
          int32x4_t b3 = vpaddlq_s16 (a3);
          int64x2_t c0 = vpaddlq_s32 (b0);
          int64x2_t c1 = vpaddlq_s32 (b1);
          int64x2_t c2 = vpaddlq_s32 (b2);
          int64x2_t c3 = vpaddlq_s32 (b3);

          /* Extract the lane sums.  Since each newline was counted as -1, we
             subtract the sum of them from LINES to get the total number of
             lines.  */
          पङ्क्तिहरू -= (vgetq_lane_s64 (c0, 0) + vgetq_lane_s64 (c0, 1)
                    + vgetq_lane_s64 (c1, 0) + vgetq_lane_s64 (c1, 1)
                    + vgetq_lane_s64 (c2, 0) + vgetq_lane_s64 (c2, 1)
                    + vgetq_lane_s64 (c3, 0) + vgetq_lane_s64 (c3, 1));

          bytes_read -= 8192;
        }

      /* Finish up any left over bytes.  */
      अचिन्हित वर्ण *end = (अचिन्हित वर्ण *) datap + bytes_read;
      लागि (अचिन्हित वर्ण *p = (अचिन्हित वर्ण *) datap; p < end; p++)
        पङ्क्तिहरू += *p == '\n';
    }
}
