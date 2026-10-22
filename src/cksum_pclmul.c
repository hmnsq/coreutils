/* cksum_crc -- calculate and print POSIX checksums and sizes of files
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

#अन्तर्भाव "cksum_crc.h"

#अन्तर्भाव <stdio.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <x86intrin.h>
#अन्तर्भाव "system.h"

/* Number of bytes to read at once.  */
#परिभाषय BUFLEN (1 << 16)

/* Calculate CRC32 using PCLMULQDQ CPU instruction found in x86/x64 CPUs */

bool
cksum_pclmul (FILE *fp, uint_fast32_t *crc_बहिर्गमनम्, intmax_t *दीर्घता_बहिर्गमनम्)
{
  __m128i buf[BUFLEN / परिमाण (__m128i)];
  uint_fast32_t crc = 0;
  intmax_t दीर्घता = 0;
  size_t अष्टकानि_read;
  __m128i single_mult_constant;
  __m128i four_mult_constant;
  __m128i shuffle_constant;

  यदि (!fp || !crc_बहिर्गमनम् || !दीर्घता_बहिर्गमनम्)
    प्रत्यागम false;

  /* These constants and general algorithms are taken from the Intel whitepaper
     "Fast CRC Computation for Generic Polynomials Using PCLMULQDQ Instruction"
  */
  single_mult_constant = mm_समुच्चयः_epi64x (0xC5B9CD4C, 0xE8A45605);
  four_mult_constant = mm_समुच्चयः_epi64x (0x8833794C, 0xE6228B11);

  /* Constant to byteswap a full SSE register */
  shuffle_constant = mm_समुच्चयः_epi8 (0, 1, 2, 3, 4, 5, 6, 7, 8,
                                   9, 10, 11, 12, 13, 14, 15);

  यावत् ((अष्टकानि_read = fread (buf, 1, BUFLEN, fp)) > 0)
    {
      __m128i *datap;
      __m128i दत्त;
      __m128i data2;
      __m128i data3;
      __m128i data4;
      __m128i data5;
      __m128i data6;
      __m128i data7;
      __m128i data8;
      __m128i fold_दत्त;
      __m128i xor_crc;

      यदि (ckd_सङ्कलय (&दीर्घता, दीर्घता, अष्टकानि_read))
        {
          errno = EOVERFLOW;
          प्रत्यागम false;
        }

      datap = buf;

      /* Fold in parallel eight 16-byte blocks into four 16-byte blocks */
      यदि (अष्टकानि_read >= 16 * 8)
        {
          दत्त = _mm_loadu_si128 (datap);
          दत्त = _mm_shuffle_epi8 (दत्त, shuffle_constant);
          /* XOR in initial CRC value (for us 0 so no effect), or CRC value
             calculated for previous BUFLEN buffer from fread */
          xor_crc = mm_समुच्चयः_epi32 (crc, 0, 0, 0);
          crc = 0;
          दत्त = _mm_xor_si128 (दत्त, xor_crc);
          data3 = _mm_loadu_si128 (datap + 1);
          data3 = _mm_shuffle_epi8 (data3, shuffle_constant);
          data5 = _mm_loadu_si128 (datap + 2);
          data5 = _mm_shuffle_epi8 (data5, shuffle_constant);
          data7 = _mm_loadu_si128 (datap + 3);
          data7 = _mm_shuffle_epi8 (data7, shuffle_constant);


          यावत् (अष्टकानि_read >= 16 * 8)
            {
              datap += 4;

              /* Do multiplication here for four consecutive 16 byte blocks */
              data2 = _mm_clmulepi64_si128 (दत्त, four_mult_constant, 0x00);
              दत्त = _mm_clmulepi64_si128 (दत्त, four_mult_constant, 0x11);
              data4 = _mm_clmulepi64_si128 (data3, four_mult_constant, 0x00);
              data3 = _mm_clmulepi64_si128 (data3, four_mult_constant, 0x11);
              data6 = _mm_clmulepi64_si128 (data5, four_mult_constant, 0x00);
              data5 = _mm_clmulepi64_si128 (data5, four_mult_constant, 0x11);
              data8 = _mm_clmulepi64_si128 (data7, four_mult_constant, 0x00);
              data7 = _mm_clmulepi64_si128 (data7, four_mult_constant, 0x11);

              /* Now multiplication results for the four blocks is xor:ed with
                 next four 16 byte blocks from the buffer. This effectively
                 "consumes" the first four blocks from the buffer.
                 Keep xor result in variables for multiplication in next
                 round of loop. */
              दत्त = _mm_xor_si128 (दत्त, data2);
              data2 = _mm_loadu_si128 (datap);
              data2 = _mm_shuffle_epi8 (data2, shuffle_constant);
              दत्त = _mm_xor_si128 (दत्त, data2);

              data3 = _mm_xor_si128 (data3, data4);
              data4 = _mm_loadu_si128 (datap + 1);
              data4 = _mm_shuffle_epi8 (data4, shuffle_constant);
              data3 = _mm_xor_si128 (data3, data4);

              data5 = _mm_xor_si128 (data5, data6);
              data6 = _mm_loadu_si128 (datap + 2);
              data6 = _mm_shuffle_epi8 (data6, shuffle_constant);
              data5 = _mm_xor_si128 (data5, data6);

              data7 = _mm_xor_si128 (data7, data8);
              data8 = _mm_loadu_si128 (datap + 3);
              data8 = _mm_shuffle_epi8 (data8, shuffle_constant);
              data7 = _mm_xor_si128 (data7, data8);

              अष्टकानि_read -= (16 * 4);
            }
          /* At end of loop we write out results from variables back into
             the buffer, for use in single fold loop */
          दत्त = _mm_shuffle_epi8 (दत्त, shuffle_constant);
          _mm_storeu_si128 (datap, दत्त);
          data3 = _mm_shuffle_epi8 (data3, shuffle_constant);
          _mm_storeu_si128 (datap + 1, data3);
          data5 = _mm_shuffle_epi8 (data5, shuffle_constant);
          _mm_storeu_si128 (datap + 2, data5);
          data7 = _mm_shuffle_epi8 (data7, shuffle_constant);
          _mm_storeu_si128 (datap + 3, data7);
        }

      /* Fold two 16-byte blocks into one 16-byte block */
      यदि (अष्टकानि_read >= 32)
        {
          दत्त = _mm_loadu_si128 (datap);
          दत्त = _mm_shuffle_epi8 (दत्त, shuffle_constant);
          xor_crc = mm_समुच्चयः_epi32 (crc, 0, 0, 0);
          crc = 0;
          दत्त = _mm_xor_si128 (दत्त, xor_crc);
          यावत् (अष्टकानि_read >= 32)
            {
              datap++;

              data2 = _mm_clmulepi64_si128 (दत्त, single_mult_constant, 0x00);
              दत्त = _mm_clmulepi64_si128 (दत्त, single_mult_constant, 0x11);
              fold_दत्त = _mm_loadu_si128 (datap);
              fold_दत्त = _mm_shuffle_epi8 (fold_दत्त, shuffle_constant);
              दत्त = _mm_xor_si128 (दत्त, data2);
              दत्त = _mm_xor_si128 (दत्त, fold_दत्त);
              अष्टकानि_read -= 16;
            }
          दत्त = _mm_shuffle_epi8 (दत्त, shuffle_constant);
          _mm_storeu_si128 (datap, दत्त);
        }

      /* And finish up last 0-31 bytes in a byte by byte fashion */
      अचिह्नित वर्ण *cp = (अचिह्नित वर्ण *)datap;
      यावत् (अष्टकानि_read--)
        crc = (crc << 8) ^ crctab[0][((crc >> 24) ^ *cp++) & 0xFF];
      यदि (feof (fp))
        भङ्ग;
    }

  *crc_बहिर्गमनम् = crc;
  *दीर्घता_बहिर्गमनम् = दीर्घता;

  प्रत्यागम !ferror (fp);
}
