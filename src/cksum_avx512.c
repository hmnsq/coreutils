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

#ضمن <config.h>

#ضمن "cksum_crc.h"

#ضمن <sys/types.h>
#ضمن <x86intrin.h>
#ضمن "system.h"

/* Number of bytes to read at once.  */
#تعریف_کرو BUFLEN (1 << 16)

bool
cksum_avx512 (FILE *fp, uint_fast32_t *چکری_جانچ_باہرلا, intmax_t *لمائی_باہرلا)
{
  __m512i buf[BUFLEN / حجم (__m512i)];
  uint_fast32_t چکری_جانچ = 0;
  intmax_t لمائی = 0;
  size_t بائٹاں_read;
  __m512i single_mult_constant;
  __m512i four_mult_constant;
  __m512i shuffle_constant;

  إذا (!fp || !چکری_جانچ_باہرلا || !لمائی_باہرلا)
    أرجع false;

  /* These constants and general algorithms are taken from the Intel whitepaper
     "Fast CRC Computation for Generic Polynomials Using PCLMULQDQ Instruction"
  */
  single_mult_constant = mm512_سیٹ_epi64 (0x8833794C, 0xE6228B11,
                                           0x8833794C, 0xE6228B11,
                                           0x8833794C, 0xE6228B11,
                                           0x8833794C, 0xE6228B11);
  four_mult_constant = mm512_سیٹ_epi64 (0xCBCF3BCB, 0x88FE2237,
                                         0xCBCF3BCB, 0x88FE2237,
                                         0xCBCF3BCB, 0x88FE2237,
                                         0xCBCF3BCB, 0x88FE2237);

  /* Constant to byteswap a full AVX512 register */
  shuffle_constant = mm512_سیٹ_epi8 (0, 1, 2, 3, 4, 5, 6, 7, 8,
                                      9, 10, 11, 12, 13, 14, 15,
                                      0, 1, 2, 3, 4, 5, 6, 7, 8,
                                      9, 10, 11, 12, 13, 14, 15,
                                      0, 1, 2, 3, 4, 5, 6, 7, 8,
                                      9, 10, 11, 12, 13, 14, 15,
                                      0, 1, 2, 3, 4, 5, 6, 7, 8,
                                      9, 10, 11, 12, 13, 14, 15);
  طالما ((بائٹاں_read = fread (buf, 1, BUFLEN, fp)) > 0)
    {
      __m512i انکڑے;
      __m512i data2;
      __m512i data3;
      __m512i data4;
      __m512i data5;
      __m512i data6;
      __m512i data7;
      __m512i data8;
      __m512i fold_انکڑے;
      __m512i xor_چکری_جانچ;

      __m512i *datap;

      إذا (ckd_جوڑو (&لمائی, لمائی, بائٹاں_read))
        {
          errno = EOVERFLOW;
          أرجع false;
        }

      datap = buf;

      /* Fold in parallel 32x 16-byte blocks into 16x 16-byte blocks */
      إذا (بائٹاں_read >= 16 * 8 * 4)
        {
          انکڑے = _mm512_loadu_si512 (datap);
          انکڑے = _mm512_shuffle_epi8 (انکڑے, shuffle_constant);
          /* XOR in initial CRC value (for us 0 so no effect), or CRC value
             calculated for previous BUFLEN buffer from fread */
          xor_چکری_جانچ = mm512_سیٹ_epi32 (0, 0, 0, 0, 0, 0, 0, 0,
                                      0, 0, 0, 0, چکری_جانچ, 0, 0, 0);
          چکری_جانچ = 0;
          انکڑے = _mm512_xor_si512 (انکڑے, xor_چکری_جانچ);
          data3 = _mm512_loadu_si512 (datap + 1);
          data3 = _mm512_shuffle_epi8 (data3, shuffle_constant);
          data5 = _mm512_loadu_si512 (datap + 2);
          data5 = _mm512_shuffle_epi8 (data5, shuffle_constant);
          data7 = _mm512_loadu_si512 (datap + 3);
          data7 = _mm512_shuffle_epi8 (data7, shuffle_constant);

          طالما (بائٹاں_read >= 16 * 8 * 4)
            {
              datap += 4;

              /* Do multiplication here for 16x consecutive 16 byte blocks */
              data2 = _mm512_clmulepi64_epi128 (انکڑے, four_mult_constant,
                                                0x00);
              انکڑے = _mm512_clmulepi64_epi128 (انکڑے, four_mult_constant,
                                               0x11);
              data4 = _mm512_clmulepi64_epi128 (data3, four_mult_constant,
                                                0x00);
              data3 = _mm512_clmulepi64_epi128 (data3, four_mult_constant,
                                                0x11);
              data6 = _mm512_clmulepi64_epi128 (data5, four_mult_constant,
                                                0x00);
              data5 = _mm512_clmulepi64_epi128 (data5, four_mult_constant,
                                                0x11);
              data8 = _mm512_clmulepi64_epi128 (data7, four_mult_constant,
                                                0x00);
              data7 = _mm512_clmulepi64_epi128 (data7, four_mult_constant,
                                                0x11);

              /* Now multiplication results for the 16x blocks is xor:ed with
                 next 16x 16 byte blocks from the buffer. This effectively
                 "consumes" the first 16x blocks from the buffer.
                 Keep xor result in variables for multiplication in next
                 round of loop. */
              انکڑے = _mm512_xor_si512 (انکڑے, data2);
              data2 = _mm512_loadu_si512 (datap);
              data2 = _mm512_shuffle_epi8 (data2, shuffle_constant);
              انکڑے = _mm512_xor_si512 (انکڑے, data2);

              data3 = _mm512_xor_si512 (data3, data4);
              data4 = _mm512_loadu_si512 (datap + 1);
              data4 = _mm512_shuffle_epi8 (data4, shuffle_constant);
              data3 = _mm512_xor_si512 (data3, data4);

              data5 = _mm512_xor_si512 (data5, data6);
              data6 = _mm512_loadu_si512 (datap + 2);
              data6 = _mm512_shuffle_epi8 (data6, shuffle_constant);
              data5 = _mm512_xor_si512 (data5, data6);

              data7 = _mm512_xor_si512 (data7, data8);
              data8 = _mm512_loadu_si512 (datap + 3);
              data8 = _mm512_shuffle_epi8 (data8, shuffle_constant);
              data7 = _mm512_xor_si512 (data7, data8);

              بائٹاں_read -= (16 * 4 * 4);
            }
          /* At end of loop we write out results from variables back into
             the buffer, for use in single fold loop */
          انکڑے = _mm512_shuffle_epi8 (انکڑے, shuffle_constant);
          _mm512_storeu_si512 (datap, انکڑے);
          data3 = _mm512_shuffle_epi8 (data3, shuffle_constant);
          _mm512_storeu_si512 (datap + 1, data3);
          data5 = _mm512_shuffle_epi8 (data5, shuffle_constant);
          _mm512_storeu_si512 (datap + 2, data5);
          data7 = _mm512_shuffle_epi8 (data7, shuffle_constant);
          _mm512_storeu_si512 (datap + 3, data7);
        }

      /* Fold two 64-byte blocks into one 64-byte block */
      إذا (بائٹاں_read >= 128)
        {
          انکڑے = _mm512_loadu_si512 (datap);
          انکڑے = _mm512_shuffle_epi8 (انکڑے, shuffle_constant);
          xor_چکری_جانچ = mm512_سیٹ_epi32 (0, 0, 0, 0, 0, 0, 0, 0,
                                      0, 0, 0, 0, چکری_جانچ, 0, 0, 0);
          چکری_جانچ = 0;
          انکڑے = _mm512_xor_si512 (انکڑے, xor_چکری_جانچ);
          طالما (بائٹاں_read >= 128)
            {
              datap++;

              data2 = _mm512_clmulepi64_epi128 (انکڑے, single_mult_constant,
                                                0x00);
              انکڑے = _mm512_clmulepi64_epi128 (انکڑے, single_mult_constant,
                                               0x11);
              fold_انکڑے = _mm512_loadu_si512 (datap);
              fold_انکڑے = _mm512_shuffle_epi8 (fold_انکڑے, shuffle_constant);
              انکڑے = _mm512_xor_si512 (انکڑے, data2);
              انکڑے = _mm512_xor_si512 (انکڑے, fold_انکڑے);
              بائٹاں_read -= 64;
            }
          انکڑے = _mm512_shuffle_epi8 (انکڑے, shuffle_constant);
          _mm512_storeu_si512 (datap, انکڑے);
        }

      /* And finish up last 0-127 bytes in a byte by byte fashion */
      بنانشانی حرف *cp = (بنانشانی حرف *)datap;
      طالما (بائٹاں_read--)
        چکری_جانچ = (چکری_جانچ << 8) ^ crctab[0][((چکری_جانچ >> 24) ^ *cp++) & 0xFF];
      إذا (feof (fp))
        اكسر;
    }

  *چکری_جانچ_باہرلا = چکری_جانچ;
  *لمائی_باہرلا = لمائی;

  أرجع !ferror (fp);
}
