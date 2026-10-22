/* cksum_crc -- calculate and print POSIX checksums and sizes of files
   Copyright (C) 2024-2026 Free Software Foundation, Inc.

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

#شامل <config.h>

#شامل "cksum_crc.h"

#شامل <stdio.h>
#شامل <sys/types.h>
#شامل <x86intrin.h>
#شامل "system.h"

/* Number of bytes to read at once.  */
#تعریف_کرو BUFLEN (1 << 16)

bool
cksum_avx2 (FILE *fp, uint_fast32_t *چکری_جانچ_برآمد, intmax_t *لمبائی_برآمد)
{
  __m256i buf[BUFLEN / حجم (__m256i)];
  uint_fast32_t چکری_جانچ = 0;
  intmax_t لمبائی = 0;
  size_t بائٹیں_read;
  __m256i single_mult_constant;
  __m256i four_mult_constant;
  __m256i shuffle_constant;

  اگر (!fp || !چکری_جانچ_برآمد || !لمبائی_برآمد)
    واپس false;

  /* These constants and general algorithms are taken from the Intel whitepaper
     "Fast CRC Computation for Generic Polynomials Using PCLMULQDQ Instruction"
  */
  single_mult_constant = mm256_سیٹ_epi64x (0x569700E5, 0x75BE46B7,
                                            0x569700E5, 0x75BE46B7);
  four_mult_constant = mm256_سیٹ_epi64x (0x10BD4D7C, 0x567FDDEB,
                                            0x10BD4D7C, 0x567FDDEB);

  /* Constant to byteswap a full AVX2 register */
  shuffle_constant = mm256_سیٹ_epi8 (0, 1, 2, 3, 4, 5, 6, 7, 8,
                                      9, 10, 11, 12, 13, 14, 15,
                                      0, 1, 2, 3, 4, 5, 6, 7, 8,
                                      9, 10, 11, 12, 13, 14, 15);
  جب ((بائٹیں_read = fread (buf, 1, BUFLEN, fp)) > 0)
    {
      __m256i ڈیٹا;
      __m256i data2;
      __m256i data3;
      __m256i data4;
      __m256i data5;
      __m256i data6;
      __m256i data7;
      __m256i data8;
      __m256i fold_ڈیٹا;
      __m256i xor_چکری_جانچ;

      __m256i *datap;

      اگر (ckd_جوڑو (&لمبائی, لمبائی, بائٹیں_read))
        {
          errno = EOVERFLOW;
          واپس false;
        }

      datap = buf;

      /* Fold in parallel 16x 16-byte blocks into 8x 16-byte blocks */
      اگر (بائٹیں_read >= 16 * 8 * 2)
        {
          ڈیٹا = _mm256_loadu_si256 (datap);
          ڈیٹا = _mm256_shuffle_epi8 (ڈیٹا, shuffle_constant);
          /* XOR in initial CRC value (for us 0 so no effect), or CRC value
             calculated for previous BUFLEN buffer from fread */
          xor_چکری_جانچ = mm256_سیٹ_epi32 (0, 0, 0, 0, چکری_جانچ, 0, 0, 0);
          چکری_جانچ = 0;
          ڈیٹا = _mm256_xor_si256 (ڈیٹا, xor_چکری_جانچ);
          data3 = _mm256_loadu_si256 (datap + 1);
          data3 = _mm256_shuffle_epi8 (data3, shuffle_constant);
          data5 = _mm256_loadu_si256 (datap + 2);
          data5 = _mm256_shuffle_epi8 (data5, shuffle_constant);
          data7 = _mm256_loadu_si256 (datap + 3);
          data7 = _mm256_shuffle_epi8 (data7, shuffle_constant);

          جب (بائٹیں_read >= 16 * 8 * 2)
            {
              datap += 4;

              /* Do multiplication here for 8x consecutive 16 byte blocks */
              data2 = _mm256_clmulepi64_epi128 (ڈیٹا, four_mult_constant,
                                                0x00);
              ڈیٹا = _mm256_clmulepi64_epi128 (ڈیٹا, four_mult_constant,
                                               0x11);
              data4 = _mm256_clmulepi64_epi128 (data3, four_mult_constant,
                                                0x00);
              data3 = _mm256_clmulepi64_epi128 (data3, four_mult_constant,
                                                0x11);
              data6 = _mm256_clmulepi64_epi128 (data5, four_mult_constant,
                                                0x00);
              data5 = _mm256_clmulepi64_epi128 (data5, four_mult_constant,
                                                0x11);
              data8 = _mm256_clmulepi64_epi128 (data7, four_mult_constant,
                                                0x00);
              data7 = _mm256_clmulepi64_epi128 (data7, four_mult_constant,
                                                0x11);

              /* Now multiplication results for the 8x blocks is xor:ed with
                 next 8x 16 byte blocks from the buffer. This effectively
                 "consumes" the first 8x blocks from the buffer.
                 Keep xor result in variables for multiplication in next
                 round of loop. */
              ڈیٹا = _mm256_xor_si256 (ڈیٹا, data2);
              data2 = _mm256_loadu_si256 (datap);
              data2 = _mm256_shuffle_epi8 (data2, shuffle_constant);
              ڈیٹا = _mm256_xor_si256 (ڈیٹا, data2);

              data3 = _mm256_xor_si256 (data3, data4);
              data4 = _mm256_loadu_si256 (datap + 1);
              data4 = _mm256_shuffle_epi8 (data4, shuffle_constant);
              data3 = _mm256_xor_si256 (data3, data4);

              data5 = _mm256_xor_si256 (data5, data6);
              data6 = _mm256_loadu_si256 (datap + 2);
              data6 = _mm256_shuffle_epi8 (data6, shuffle_constant);
              data5 = _mm256_xor_si256 (data5, data6);

              data7 = _mm256_xor_si256 (data7, data8);
              data8 = _mm256_loadu_si256 (datap + 3);
              data8 = _mm256_shuffle_epi8 (data8, shuffle_constant);
              data7 = _mm256_xor_si256 (data7, data8);

              بائٹیں_read -= (16 * 4 * 2);
            }
          /* At end of loop we write out results from variables back into
             the buffer, for use in single fold loop */
          ڈیٹا = _mm256_shuffle_epi8 (ڈیٹا, shuffle_constant);
          _mm256_storeu_si256 (datap, ڈیٹا);
          data3 = _mm256_shuffle_epi8 (data3, shuffle_constant);
          _mm256_storeu_si256 (datap + 1, data3);
          data5 = _mm256_shuffle_epi8 (data5, shuffle_constant);
          _mm256_storeu_si256 (datap + 2, data5);
          data7 = _mm256_shuffle_epi8 (data7, shuffle_constant);
          _mm256_storeu_si256 (datap + 3, data7);
        }

      /* Fold two 32-byte blocks into one 32-byte block */
      اگر (بائٹیں_read >= 64)
        {
          ڈیٹا = _mm256_loadu_si256 (datap);
          ڈیٹا = _mm256_shuffle_epi8 (ڈیٹا, shuffle_constant);
          xor_چکری_جانچ = mm256_سیٹ_epi32 (0, 0, 0, 0, چکری_جانچ, 0, 0, 0);
          چکری_جانچ = 0;
          ڈیٹا = _mm256_xor_si256 (ڈیٹا, xor_چکری_جانچ);
          جب (بائٹیں_read >= 64)
            {
              datap++;

              data2 = _mm256_clmulepi64_epi128 (ڈیٹا, single_mult_constant,
                                                0x00);
              ڈیٹا = _mm256_clmulepi64_epi128 (ڈیٹا, single_mult_constant,
                                               0x11);
              fold_ڈیٹا = _mm256_loadu_si256 (datap);
              fold_ڈیٹا = _mm256_shuffle_epi8 (fold_ڈیٹا, shuffle_constant);
              ڈیٹا = _mm256_xor_si256 (ڈیٹا, data2);
              ڈیٹا = _mm256_xor_si256 (ڈیٹا, fold_ڈیٹا);
              بائٹیں_read -= 32;
            }
          ڈیٹا = _mm256_shuffle_epi8 (ڈیٹا, shuffle_constant);
          _mm256_storeu_si256 (datap, ڈیٹا);
        }

      /* And finish up last 0-63 bytes in a byte by byte fashion */
      غیرعلامتی حرف *cp = (غیرعلامتی حرف *)datap;
      جب (بائٹیں_read--)
        چکری_جانچ = (چکری_جانچ << 8) ^ crctab[0][((چکری_جانچ >> 24) ^ *cp++) & 0xFF];
      اگر (feof (fp))
        توڑو;
    }

  *چکری_جانچ_برآمد = چکری_جانچ;
  *لمبائی_برآمد = لمبائی;

  واپس !ferror (fp);
}
