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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "cksum_crc.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <x86intrin.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"

/* Number of bytes to read at once.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUFLEN (1 << 16)

/* Calculate CRC32 using PCLMULQDQ CPU instruction found in x86/x64 CPUs */

bool
cksum_pclmul (FILE *fp, uint_fast32_t *ਚੱਕਰੀ_ਜਾਂਚ_ਬਾਹਰਲਾ, intmax_t *ਲੰਮਾਈ_ਬਾਹਰਲਾ)
{
  __m128i buf[BUFLEN / ਆਕਾਰ (__m128i)];
  uint_fast32_t ਚੱਕਰੀ_ਜਾਂਚ = 0;
  intmax_t ਲੰਮਾਈ = 0;
  size_t ਬਾਈਟਾਂ_read;
  __m128i single_mult_constant;
  __m128i four_mult_constant;
  __m128i shuffle_constant;

  ਜੇ (!fp || !ਚੱਕਰੀ_ਜਾਂਚ_ਬਾਹਰਲਾ || !ਲੰਮਾਈ_ਬਾਹਰਲਾ)
    ਵਾਪਸ false;

  /* These constants and general algorithms are taken from the Intel whitepaper
     "Fast CRC Computation for Generic Polynomials Using PCLMULQDQ Instruction"
  */
  single_mult_constant = mm_ਸੈੱਟ_epi64x (0xC5B9CD4C, 0xE8A45605);
  four_mult_constant = mm_ਸੈੱਟ_epi64x (0x8833794C, 0xE6228B11);

  /* Constant to byteswap a full SSE register */
  shuffle_constant = mm_ਸੈੱਟ_epi8 (0, 1, 2, 3, 4, 5, 6, 7, 8,
                                   9, 10, 11, 12, 13, 14, 15);

  ਜਦੋਂ ((ਬਾਈਟਾਂ_read = fread (buf, 1, BUFLEN, fp)) > 0)
    {
      __m128i *datap;
      __m128i ਅੰਕੜੇ;
      __m128i data2;
      __m128i data3;
      __m128i data4;
      __m128i data5;
      __m128i data6;
      __m128i data7;
      __m128i data8;
      __m128i fold_ਅੰਕੜੇ;
      __m128i xor_ਚੱਕਰੀ_ਜਾਂਚ;

      ਜੇ (ckd_ਜੋੜੋ (&ਲੰਮਾਈ, ਲੰਮਾਈ, ਬਾਈਟਾਂ_read))
        {
          errno = EOVERFLOW;
          ਵਾਪਸ false;
        }

      datap = buf;

      /* Fold in parallel eight 16-byte blocks into four 16-byte blocks */
      ਜੇ (ਬਾਈਟਾਂ_read >= 16 * 8)
        {
          ਅੰਕੜੇ = _mm_loadu_si128 (datap);
          ਅੰਕੜੇ = _mm_shuffle_epi8 (ਅੰਕੜੇ, shuffle_constant);
          /* XOR in initial CRC value (for us 0 so no effect), or CRC value
             calculated for previous BUFLEN buffer from fread */
          xor_ਚੱਕਰੀ_ਜਾਂਚ = mm_ਸੈੱਟ_epi32 (ਚੱਕਰੀ_ਜਾਂਚ, 0, 0, 0);
          ਚੱਕਰੀ_ਜਾਂਚ = 0;
          ਅੰਕੜੇ = _mm_xor_si128 (ਅੰਕੜੇ, xor_ਚੱਕਰੀ_ਜਾਂਚ);
          data3 = _mm_loadu_si128 (datap + 1);
          data3 = _mm_shuffle_epi8 (data3, shuffle_constant);
          data5 = _mm_loadu_si128 (datap + 2);
          data5 = _mm_shuffle_epi8 (data5, shuffle_constant);
          data7 = _mm_loadu_si128 (datap + 3);
          data7 = _mm_shuffle_epi8 (data7, shuffle_constant);


          ਜਦੋਂ (ਬਾਈਟਾਂ_read >= 16 * 8)
            {
              datap += 4;

              /* Do multiplication here for four consecutive 16 byte blocks */
              data2 = _mm_clmulepi64_si128 (ਅੰਕੜੇ, four_mult_constant, 0x00);
              ਅੰਕੜੇ = _mm_clmulepi64_si128 (ਅੰਕੜੇ, four_mult_constant, 0x11);
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
              ਅੰਕੜੇ = _mm_xor_si128 (ਅੰਕੜੇ, data2);
              data2 = _mm_loadu_si128 (datap);
              data2 = _mm_shuffle_epi8 (data2, shuffle_constant);
              ਅੰਕੜੇ = _mm_xor_si128 (ਅੰਕੜੇ, data2);

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

              ਬਾਈਟਾਂ_read -= (16 * 4);
            }
          /* At end of loop we write out results from variables back into
             the buffer, for use in single fold loop */
          ਅੰਕੜੇ = _mm_shuffle_epi8 (ਅੰਕੜੇ, shuffle_constant);
          _mm_storeu_si128 (datap, ਅੰਕੜੇ);
          data3 = _mm_shuffle_epi8 (data3, shuffle_constant);
          _mm_storeu_si128 (datap + 1, data3);
          data5 = _mm_shuffle_epi8 (data5, shuffle_constant);
          _mm_storeu_si128 (datap + 2, data5);
          data7 = _mm_shuffle_epi8 (data7, shuffle_constant);
          _mm_storeu_si128 (datap + 3, data7);
        }

      /* Fold two 16-byte blocks into one 16-byte block */
      ਜੇ (ਬਾਈਟਾਂ_read >= 32)
        {
          ਅੰਕੜੇ = _mm_loadu_si128 (datap);
          ਅੰਕੜੇ = _mm_shuffle_epi8 (ਅੰਕੜੇ, shuffle_constant);
          xor_ਚੱਕਰੀ_ਜਾਂਚ = mm_ਸੈੱਟ_epi32 (ਚੱਕਰੀ_ਜਾਂਚ, 0, 0, 0);
          ਚੱਕਰੀ_ਜਾਂਚ = 0;
          ਅੰਕੜੇ = _mm_xor_si128 (ਅੰਕੜੇ, xor_ਚੱਕਰੀ_ਜਾਂਚ);
          ਜਦੋਂ (ਬਾਈਟਾਂ_read >= 32)
            {
              datap++;

              data2 = _mm_clmulepi64_si128 (ਅੰਕੜੇ, single_mult_constant, 0x00);
              ਅੰਕੜੇ = _mm_clmulepi64_si128 (ਅੰਕੜੇ, single_mult_constant, 0x11);
              fold_ਅੰਕੜੇ = _mm_loadu_si128 (datap);
              fold_ਅੰਕੜੇ = _mm_shuffle_epi8 (fold_ਅੰਕੜੇ, shuffle_constant);
              ਅੰਕੜੇ = _mm_xor_si128 (ਅੰਕੜੇ, data2);
              ਅੰਕੜੇ = _mm_xor_si128 (ਅੰਕੜੇ, fold_ਅੰਕੜੇ);
              ਬਾਈਟਾਂ_read -= 16;
            }
          ਅੰਕੜੇ = _mm_shuffle_epi8 (ਅੰਕੜੇ, shuffle_constant);
          _mm_storeu_si128 (datap, ਅੰਕੜੇ);
        }

      /* And finish up last 0-31 bytes in a byte by byte fashion */
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *cp = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)datap;
      ਜਦੋਂ (ਬਾਈਟਾਂ_read--)
        ਚੱਕਰੀ_ਜਾਂਚ = (ਚੱਕਰੀ_ਜਾਂਚ << 8) ^ crctab[0][((ਚੱਕਰੀ_ਜਾਂਚ >> 24) ^ *cp++) & 0xFF];
      ਜੇ (feof (fp))
        ਰੋਕੋ;
    }

  *ਚੱਕਰੀ_ਜਾਂਚ_ਬਾਹਰਲਾ = ਚੱਕਰੀ_ਜਾਂਚ;
  *ਲੰਮਾਈ_ਬਾਹਰਲਾ = ਲੰਮਾਈ;

  ਵਾਪਸ !ferror (fp);
}
