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

#ಅನ್ತರ್ಭಾವ <config.h>

#ಅನ್ತರ್ಭಾವ "cksum_crc.h"

#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <x86intrin.h>
#ಅನ್ತರ್ಭಾವ "system.h"

/* Number of bytes to read at once.  */
#ವ್ಯಾಖ್ಯಾನಿಸು BUFLEN (1 << 16)

bool
cksum_avx512 (FILE *fp, uint_fast32_t *ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_ಹೊರಹೋಗು, intmax_t *ಉದ್ದಳತೆ_ಹೊರಹೋಗು)
{
  __m512i buf[BUFLEN / ಗಾತ್ರ (__m512i)];
  uint_fast32_t ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = 0;
  intmax_t ಉದ್ದಳತೆ = 0;
  size_t ಬೈಟುಗಳು_read;
  __m512i single_mult_constant;
  __m512i four_mult_constant;
  __m512i shuffle_constant;

  ಆದರೆ (!fp || !ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_ಹೊರಹೋಗು || !ಉದ್ದಳತೆ_ಹೊರಹೋಗು)
    ಹಿಂತಿರುಗಿ false;

  /* These constants and general algorithms are taken from the Intel whitepaper
     "Fast CRC Computation for Generic Polynomials Using PCLMULQDQ Instruction"
  */
  single_mult_constant = mm512_ಗಣ_epi64 (0x8833794C, 0xE6228B11,
                                           0x8833794C, 0xE6228B11,
                                           0x8833794C, 0xE6228B11,
                                           0x8833794C, 0xE6228B11);
  four_mult_constant = mm512_ಗಣ_epi64 (0xCBCF3BCB, 0x88FE2237,
                                         0xCBCF3BCB, 0x88FE2237,
                                         0xCBCF3BCB, 0x88FE2237,
                                         0xCBCF3BCB, 0x88FE2237);

  /* Constant to byteswap a full AVX512 register */
  shuffle_constant = mm512_ಗಣ_epi8 (0, 1, 2, 3, 4, 5, 6, 7, 8,
                                      9, 10, 11, 12, 13, 14, 15,
                                      0, 1, 2, 3, 4, 5, 6, 7, 8,
                                      9, 10, 11, 12, 13, 14, 15,
                                      0, 1, 2, 3, 4, 5, 6, 7, 8,
                                      9, 10, 11, 12, 13, 14, 15,
                                      0, 1, 2, 3, 4, 5, 6, 7, 8,
                                      9, 10, 11, 12, 13, 14, 15);
  ಆಗ ((ಬೈಟುಗಳು_read = fread (buf, 1, BUFLEN, fp)) > 0)
    {
      __m512i ದತ್ತಾಂಶ;
      __m512i data2;
      __m512i data3;
      __m512i data4;
      __m512i data5;
      __m512i data6;
      __m512i data7;
      __m512i data8;
      __m512i fold_ದತ್ತಾಂಶ;
      __m512i xor_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ;

      __m512i *datap;

      ಆದರೆ (ckd_ಕೂಡಿಸು (&ಉದ್ದಳತೆ, ಉದ್ದಳತೆ, ಬೈಟುಗಳು_read))
        {
          errno = EOVERFLOW;
          ಹಿಂತಿರುಗಿ false;
        }

      datap = buf;

      /* Fold in parallel 32x 16-byte blocks into 16x 16-byte blocks */
      ಆದರೆ (ಬೈಟುಗಳು_read >= 16 * 8 * 4)
        {
          ದತ್ತಾಂಶ = _mm512_loadu_si512 (datap);
          ದತ್ತಾಂಶ = _mm512_shuffle_epi8 (ದತ್ತಾಂಶ, shuffle_constant);
          /* XOR in initial CRC value (for us 0 so no effect), or CRC value
             calculated for previous BUFLEN buffer from fread */
          xor_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = mm512_ಗಣ_epi32 (0, 0, 0, 0, 0, 0, 0, 0,
                                      0, 0, 0, 0, ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ, 0, 0, 0);
          ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = 0;
          ದತ್ತಾಂಶ = _mm512_xor_si512 (ದತ್ತಾಂಶ, xor_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ);
          data3 = _mm512_loadu_si512 (datap + 1);
          data3 = _mm512_shuffle_epi8 (data3, shuffle_constant);
          data5 = _mm512_loadu_si512 (datap + 2);
          data5 = _mm512_shuffle_epi8 (data5, shuffle_constant);
          data7 = _mm512_loadu_si512 (datap + 3);
          data7 = _mm512_shuffle_epi8 (data7, shuffle_constant);

          ಆಗ (ಬೈಟುಗಳು_read >= 16 * 8 * 4)
            {
              datap += 4;

              /* Do multiplication here for 16x consecutive 16 byte blocks */
              data2 = _mm512_clmulepi64_epi128 (ದತ್ತಾಂಶ, four_mult_constant,
                                                0x00);
              ದತ್ತಾಂಶ = _mm512_clmulepi64_epi128 (ದತ್ತಾಂಶ, four_mult_constant,
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
              ದತ್ತಾಂಶ = _mm512_xor_si512 (ದತ್ತಾಂಶ, data2);
              data2 = _mm512_loadu_si512 (datap);
              data2 = _mm512_shuffle_epi8 (data2, shuffle_constant);
              ದತ್ತಾಂಶ = _mm512_xor_si512 (ದತ್ತಾಂಶ, data2);

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

              ಬೈಟುಗಳು_read -= (16 * 4 * 4);
            }
          /* At end of loop we write out results from variables back into
             the buffer, for use in single fold loop */
          ದತ್ತಾಂಶ = _mm512_shuffle_epi8 (ದತ್ತಾಂಶ, shuffle_constant);
          _mm512_storeu_si512 (datap, ದತ್ತಾಂಶ);
          data3 = _mm512_shuffle_epi8 (data3, shuffle_constant);
          _mm512_storeu_si512 (datap + 1, data3);
          data5 = _mm512_shuffle_epi8 (data5, shuffle_constant);
          _mm512_storeu_si512 (datap + 2, data5);
          data7 = _mm512_shuffle_epi8 (data7, shuffle_constant);
          _mm512_storeu_si512 (datap + 3, data7);
        }

      /* Fold two 64-byte blocks into one 64-byte block */
      ಆದರೆ (ಬೈಟುಗಳು_read >= 128)
        {
          ದತ್ತಾಂಶ = _mm512_loadu_si512 (datap);
          ದತ್ತಾಂಶ = _mm512_shuffle_epi8 (ದತ್ತಾಂಶ, shuffle_constant);
          xor_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = mm512_ಗಣ_epi32 (0, 0, 0, 0, 0, 0, 0, 0,
                                      0, 0, 0, 0, ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ, 0, 0, 0);
          ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = 0;
          ದತ್ತಾಂಶ = _mm512_xor_si512 (ದತ್ತಾಂಶ, xor_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ);
          ಆಗ (ಬೈಟುಗಳು_read >= 128)
            {
              datap++;

              data2 = _mm512_clmulepi64_epi128 (ದತ್ತಾಂಶ, single_mult_constant,
                                                0x00);
              ದತ್ತಾಂಶ = _mm512_clmulepi64_epi128 (ದತ್ತಾಂಶ, single_mult_constant,
                                               0x11);
              fold_ದತ್ತಾಂಶ = _mm512_loadu_si512 (datap);
              fold_ದತ್ತಾಂಶ = _mm512_shuffle_epi8 (fold_ದತ್ತಾಂಶ, shuffle_constant);
              ದತ್ತಾಂಶ = _mm512_xor_si512 (ದತ್ತಾಂಶ, data2);
              ದತ್ತಾಂಶ = _mm512_xor_si512 (ದತ್ತಾಂಶ, fold_ದತ್ತಾಂಶ);
              ಬೈಟುಗಳು_read -= 64;
            }
          ದತ್ತಾಂಶ = _mm512_shuffle_epi8 (ದತ್ತಾಂಶ, shuffle_constant);
          _mm512_storeu_si512 (datap, ದತ್ತಾಂಶ);
        }

      /* And finish up last 0-127 bytes in a byte by byte fashion */
      ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *cp = (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)datap;
      ಆಗ (ಬೈಟುಗಳು_read--)
        ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = (ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ << 8) ^ crctab[0][((ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ >> 24) ^ *cp++) & 0xFF];
      ಆದರೆ (feof (fp))
        ನಿಲ್ಲಿಸು;
    }

  *ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_ಹೊರಹೋಗು = ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ;
  *ಉದ್ದಳತೆ_ಹೊರಹೋಗು = ಉದ್ದಳತೆ;

  ಹಿಂತಿರುಗಿ !ferror (fp);
}
