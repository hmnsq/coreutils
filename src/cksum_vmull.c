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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "cksum_crc.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <arm_neon.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"

/* Number of bytes to read at once.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUFLEN (1 << 16)

ਸਥਿਰ uint64x2_t
bswap_neon (uint64x2_t ਵਿੱਚ)
{
  uint64x2_t a =
    vreinterpretq_u64_u8 (vrev64q_u8 (vreinterpretq_u8_u64 (ਵਿੱਚ)));
  a = vcombine_u64 (vget_ਉੱਚਾ_u64 (a), vget_ਨੀਵਾਂ_u64 (a));
  ਵਾਪਸ a;
}

/* Calculate CRC32 using VMULL CPU instruction found in ARMv8 CPUs */

bool
cksum_vmull (FILE *fp, uint_fast32_t *ਚੱਕਰੀ_ਜਾਂਚ_ਬਾਹਰਲਾ, intmax_t *ਲੰਮਾਈ_ਬਾਹਰਲਾ)
{
  uint64x2_t buf[BUFLEN / ਆਕਾਰ (uint64x2_t)];
  uint_fast32_t ਚੱਕਰੀ_ਜਾਂਚ = 0;
  intmax_t ਲੰਮਾਈ = 0;
  size_t ਬਾਈਟਾਂ_read;
  poly64x2_t single_mult_constant;
  poly64x2_t four_mult_constant;

  ਜੇ (!fp || !ਚੱਕਰੀ_ਜਾਂਚ_ਬਾਹਰਲਾ || !ਲੰਮਾਈ_ਬਾਹਰਲਾ)
    ਵਾਪਸ false;

  /* These constants and general algorithms are taken from the Intel whitepaper
     "Fast CRC Computation for Generic Polynomials Using PCLMULQDQ Instruction"
   */
  single_mult_constant =
    vcombine_p64 (vcreate_p64 (0xE8A45605), vcreate_p64 (0xC5B9CD4C));
  four_mult_constant =
    vcombine_p64 (vcreate_p64 (0xE6228B11), vcreate_p64 (0x8833794C));

  ਜਦੋਂ ((ਬਾਈਟਾਂ_read = fread (buf, 1, BUFLEN, fp)) > 0)
    {
      uint64x2_t *datap;
      uint64x2_t ਅੰਕੜੇ;
      uint64x2_t data2;
      uint64x2_t data3;
      uint64x2_t data4;
      uint64x2_t data5;
      uint64x2_t data6;
      uint64x2_t data7;
      uint64x2_t data8;
      uint64x2_t fold_ਅੰਕੜੇ;
      uint64x2_t xor_ਚੱਕਰੀ_ਜਾਂਚ;

      ਜੇ (ckd_ਜੋੜੋ (&ਲੰਮਾਈ, ਲੰਮਾਈ, ਬਾਈਟਾਂ_read))
        {
          errno = EOVERFLOW;
          ਵਾਪਸ false;
        }

      datap = (uint64x2_t *) buf;

      /* Fold in parallel eight 16-byte blocks into four 16-byte blocks */
      ਜੇ (ਬਾਈਟਾਂ_read >= 16 * 8)
        {
          ਅੰਕੜੇ = vld1q_u64 ((uint64_t *) (datap));
          ਅੰਕੜੇ = bswap_neon (ਅੰਕੜੇ);
          /* XOR in initial CRC value (for us 0 so no effect), or CRC value
             calculated for previous BUFLEN buffer from fread */
          uint64_t wcrc = ਚੱਕਰੀ_ਜਾਂਚ;
          xor_ਚੱਕਰੀ_ਜਾਂਚ = vcombine_u64 (vcreate_u64 (0), vcreate_u64 (wcrc << 32));
          ਚੱਕਰੀ_ਜਾਂਚ = 0;
          ਅੰਕੜੇ = veorq_u64 (ਅੰਕੜੇ, xor_ਚੱਕਰੀ_ਜਾਂਚ);
          data3 = vld1q_u64 ((uint64_t *) (datap + 1));
          data3 = bswap_neon (data3);
          data5 = vld1q_u64 ((uint64_t *) (datap + 2));
          data5 = bswap_neon (data5);
          data7 = vld1q_u64 ((uint64_t *) (datap + 3));
          data7 = bswap_neon (data7);


          ਜਦੋਂ (ਬਾਈਟਾਂ_read >= 16 * 8)
            {
              datap += 4;

              /* Do multiplication here for four consecutive 16 byte blocks */
              data2 =
                vreinterpretq_u64_p128 (vmull_p64
                                        (vgetq_lane_p64
                                         (vreinterpretq_p64_u64 (ਅੰਕੜੇ), 0),
                                         vgetq_lane_p64 (four_mult_constant,
                                                         0)));
              ਅੰਕੜੇ =
                vreinterpretq_u64_p128 (vmull_ਉੱਚਾ_p64
                                        (vreinterpretq_p64_u64 (ਅੰਕੜੇ),
                                         four_mult_constant));
              data4 =
                vreinterpretq_u64_p128 (vmull_p64
                                        (vgetq_lane_p64
                                         (vreinterpretq_p64_u64 (data3), 0),
                                         vgetq_lane_p64 (four_mult_constant,
                                                         0)));
              data3 =
                vreinterpretq_u64_p128 (vmull_ਉੱਚਾ_p64
                                        (vreinterpretq_p64_u64 (data3),
                                         four_mult_constant));
              data6 =
                vreinterpretq_u64_p128 (vmull_p64
                                        (vgetq_lane_p64
                                         (vreinterpretq_p64_u64 (data5), 0),
                                         vgetq_lane_p64 (four_mult_constant,
                                                         0)));
              data5 =
                vreinterpretq_u64_p128 (vmull_ਉੱਚਾ_p64
                                        (vreinterpretq_p64_u64 (data5),
                                         four_mult_constant));
              data8 =
                vreinterpretq_u64_p128 (vmull_p64
                                        (vgetq_lane_p64
                                         (vreinterpretq_p64_u64 (data7), 0),
                                         vgetq_lane_p64 (four_mult_constant,
                                                         0)));
              data7 =
                vreinterpretq_u64_p128 (vmull_ਉੱਚਾ_p64
                                        (vreinterpretq_p64_u64 (data7),
                                         four_mult_constant));

              /* Now multiplication results for the four blocks is xor:ed with
                 next four 16 byte blocks from the buffer. This effectively
                 "consumes" the first four blocks from the buffer.
                 Keep xor result in variables for multiplication in next
                 round of loop. */
              ਅੰਕੜੇ = veorq_u64 (ਅੰਕੜੇ, data2);
              data2 = vld1q_u64 ((uint64_t *) (datap));
              data2 = bswap_neon (data2);
              ਅੰਕੜੇ = veorq_u64 (ਅੰਕੜੇ, data2);

              data3 = veorq_u64 (data3, data4);
              data4 = vld1q_u64 ((uint64_t *) (datap + 1));
              data4 = bswap_neon (data4);
              data3 = veorq_u64 (data3, data4);

              data5 = veorq_u64 (data5, data6);
              data6 = vld1q_u64 ((uint64_t *) (datap + 2));
              data6 = bswap_neon (data6);
              data5 = veorq_u64 (data5, data6);

              data7 = veorq_u64 (data7, data8);
              data8 = vld1q_u64 ((uint64_t *) (datap + 3));
              data8 = bswap_neon (data8);
              data7 = veorq_u64 (data7, data8);

              ਬਾਈਟਾਂ_read -= (16 * 4);
            }
          /* At end of loop we write out results from variables back into
             the buffer, for use in single fold loop */
          ਅੰਕੜੇ = bswap_neon (ਅੰਕੜੇ);
          vst1q_u64 ((uint64_t *) (datap), ਅੰਕੜੇ);
          data3 = bswap_neon (data3);
          vst1q_u64 ((uint64_t *) (datap + 1), data3);
          data5 = bswap_neon (data5);
          vst1q_u64 ((uint64_t *) (datap + 2), data5);
          data7 = bswap_neon (data7);
          vst1q_u64 ((uint64_t *) (datap + 3), data7);
        }

      /* Fold two 16-byte blocks into one 16-byte block */
      ਜੇ (ਬਾਈਟਾਂ_read >= 32)
        {
          ਅੰਕੜੇ = vld1q_u64 ((uint64_t *) (datap));
          ਅੰਕੜੇ = bswap_neon (ਅੰਕੜੇ);
          uint64_t wcrc = ਚੱਕਰੀ_ਜਾਂਚ;
          xor_ਚੱਕਰੀ_ਜਾਂਚ = vcombine_u64 (vcreate_u64 (0), vcreate_u64 (wcrc << 32));
          ਚੱਕਰੀ_ਜਾਂਚ = 0;
          ਅੰਕੜੇ = veorq_u64 (ਅੰਕੜੇ, xor_ਚੱਕਰੀ_ਜਾਂਚ);
          ਜਦੋਂ (ਬਾਈਟਾਂ_read >= 32)
            {
              datap++;

              data2 =
                vreinterpretq_u64_p128 (vmull_p64
                                        (vgetq_lane_p64
                                         (vreinterpretq_p64_u64 (ਅੰਕੜੇ), 0),
                                         vgetq_lane_p64 (single_mult_constant,
                                                         0)));
              ਅੰਕੜੇ =
                vreinterpretq_u64_p128 (vmull_ਉੱਚਾ_p64
                                        (vreinterpretq_p64_u64 (ਅੰਕੜੇ),
                                         single_mult_constant));
              fold_ਅੰਕੜੇ = vld1q_u64 ((uint64_t *) (datap));
              fold_ਅੰਕੜੇ = bswap_neon (fold_ਅੰਕੜੇ);
              ਅੰਕੜੇ = veorq_u64 (ਅੰਕੜੇ, data2);
              ਅੰਕੜੇ = veorq_u64 (ਅੰਕੜੇ, fold_ਅੰਕੜੇ);
              ਬਾਈਟਾਂ_read -= 16;
            }
          ਅੰਕੜੇ = bswap_neon (ਅੰਕੜੇ);
          vst1q_u64 ((uint64_t *) (datap), ਅੰਕੜੇ);
        }

      /* And finish up last 0-31 bytes in a byte by byte fashion */
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *cp = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *) datap;
      ਜਦੋਂ (ਬਾਈਟਾਂ_read--)
        ਚੱਕਰੀ_ਜਾਂਚ = (ਚੱਕਰੀ_ਜਾਂਚ << 8) ^ crctab[0][((ਚੱਕਰੀ_ਜਾਂਚ >> 24) ^ *cp++) & 0xFF];
      ਜੇ (feof (fp))
        ਰੋਕੋ;
    }

  *ਚੱਕਰੀ_ਜਾਂਚ_ਬਾਹਰਲਾ = ਚੱਕਰੀ_ਜਾਂਚ;
  *ਲੰਮਾਈ_ਬਾਹਰਲਾ = ਲੰਮਾਈ;

  ਵਾਪਸ !ferror (fp);
}
