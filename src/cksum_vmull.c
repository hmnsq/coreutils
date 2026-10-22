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

#ضمن <stdio.h>
#ضمن <sys/types.h>
#ضمن <arm_neon.h>
#ضمن "system.h"

/* Number of bytes to read at once.  */
#وصف_ڪريو BUFLEN (1 << 16)

ساكن uint64x2_t
bswap_neon (uint64x2_t اندريون)
{
  uint64x2_t a =
    vreinterpretq_u64_u8 (vrev64q_u8 (vreinterpretq_u8_u64 (اندريون)));
  a = vcombine_u64 (vget_مٿاهون_u64 (a), vget_هيٺيون_u64 (a));
  أرجع a;
}

/* Calculate CRC32 using VMULL CPU instruction found in ARMv8 CPUs */

bool
cksum_vmull (FILE *fp, uint_fast32_t *چڪري_جاچ_ٻاهريون, intmax_t *ڊيگهائي_ٻاهريون)
{
  uint64x2_t buf[BUFLEN / حجم (uint64x2_t)];
  uint_fast32_t چڪري_جاچ = 0;
  intmax_t ڊيگهائي = 0;
  size_t بائيٽون_read;
  poly64x2_t single_mult_constant;
  poly64x2_t four_mult_constant;

  إذا (!fp || !چڪري_جاچ_ٻاهريون || !ڊيگهائي_ٻاهريون)
    أرجع false;

  /* These constants and general algorithms are taken from the Intel whitepaper
     "Fast CRC Computation for Generic Polynomials Using PCLMULQDQ Instruction"
   */
  single_mult_constant =
    vcombine_p64 (vcreate_p64 (0xE8A45605), vcreate_p64 (0xC5B9CD4C));
  four_mult_constant =
    vcombine_p64 (vcreate_p64 (0xE6228B11), vcreate_p64 (0x8833794C));

  طالما ((بائيٽون_read = fread (buf, 1, BUFLEN, fp)) > 0)
    {
      uint64x2_t *datap;
      uint64x2_t معلومات;
      uint64x2_t data2;
      uint64x2_t data3;
      uint64x2_t data4;
      uint64x2_t data5;
      uint64x2_t data6;
      uint64x2_t data7;
      uint64x2_t data8;
      uint64x2_t fold_معلومات;
      uint64x2_t xor_چڪري_جاچ;

      إذا (ckd_جوڙيو (&ڊيگهائي, ڊيگهائي, بائيٽون_read))
        {
          errno = EOVERFLOW;
          أرجع false;
        }

      datap = (uint64x2_t *) buf;

      /* Fold in parallel eight 16-byte blocks into four 16-byte blocks */
      إذا (بائيٽون_read >= 16 * 8)
        {
          معلومات = vld1q_u64 ((uint64_t *) (datap));
          معلومات = bswap_neon (معلومات);
          /* XOR in initial CRC value (for us 0 so no effect), or CRC value
             calculated for previous BUFLEN buffer from fread */
          uint64_t wcrc = چڪري_جاچ;
          xor_چڪري_جاچ = vcombine_u64 (vcreate_u64 (0), vcreate_u64 (wcrc << 32));
          چڪري_جاچ = 0;
          معلومات = veorq_u64 (معلومات, xor_چڪري_جاچ);
          data3 = vld1q_u64 ((uint64_t *) (datap + 1));
          data3 = bswap_neon (data3);
          data5 = vld1q_u64 ((uint64_t *) (datap + 2));
          data5 = bswap_neon (data5);
          data7 = vld1q_u64 ((uint64_t *) (datap + 3));
          data7 = bswap_neon (data7);


          طالما (بائيٽون_read >= 16 * 8)
            {
              datap += 4;

              /* Do multiplication here for four consecutive 16 byte blocks */
              data2 =
                vreinterpretq_u64_p128 (vmull_p64
                                        (vgetq_lane_p64
                                         (vreinterpretq_p64_u64 (معلومات), 0),
                                         vgetq_lane_p64 (four_mult_constant,
                                                         0)));
              معلومات =
                vreinterpretq_u64_p128 (vmull_مٿاهون_p64
                                        (vreinterpretq_p64_u64 (معلومات),
                                         four_mult_constant));
              data4 =
                vreinterpretq_u64_p128 (vmull_p64
                                        (vgetq_lane_p64
                                         (vreinterpretq_p64_u64 (data3), 0),
                                         vgetq_lane_p64 (four_mult_constant,
                                                         0)));
              data3 =
                vreinterpretq_u64_p128 (vmull_مٿاهون_p64
                                        (vreinterpretq_p64_u64 (data3),
                                         four_mult_constant));
              data6 =
                vreinterpretq_u64_p128 (vmull_p64
                                        (vgetq_lane_p64
                                         (vreinterpretq_p64_u64 (data5), 0),
                                         vgetq_lane_p64 (four_mult_constant,
                                                         0)));
              data5 =
                vreinterpretq_u64_p128 (vmull_مٿاهون_p64
                                        (vreinterpretq_p64_u64 (data5),
                                         four_mult_constant));
              data8 =
                vreinterpretq_u64_p128 (vmull_p64
                                        (vgetq_lane_p64
                                         (vreinterpretq_p64_u64 (data7), 0),
                                         vgetq_lane_p64 (four_mult_constant,
                                                         0)));
              data7 =
                vreinterpretq_u64_p128 (vmull_مٿاهون_p64
                                        (vreinterpretq_p64_u64 (data7),
                                         four_mult_constant));

              /* Now multiplication results for the four blocks is xor:ed with
                 next four 16 byte blocks from the buffer. This effectively
                 "consumes" the first four blocks from the buffer.
                 Keep xor result in variables for multiplication in next
                 round of loop. */
              معلومات = veorq_u64 (معلومات, data2);
              data2 = vld1q_u64 ((uint64_t *) (datap));
              data2 = bswap_neon (data2);
              معلومات = veorq_u64 (معلومات, data2);

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

              بائيٽون_read -= (16 * 4);
            }
          /* At end of loop we write out results from variables back into
             the buffer, for use in single fold loop */
          معلومات = bswap_neon (معلومات);
          vst1q_u64 ((uint64_t *) (datap), معلومات);
          data3 = bswap_neon (data3);
          vst1q_u64 ((uint64_t *) (datap + 1), data3);
          data5 = bswap_neon (data5);
          vst1q_u64 ((uint64_t *) (datap + 2), data5);
          data7 = bswap_neon (data7);
          vst1q_u64 ((uint64_t *) (datap + 3), data7);
        }

      /* Fold two 16-byte blocks into one 16-byte block */
      إذا (بائيٽون_read >= 32)
        {
          معلومات = vld1q_u64 ((uint64_t *) (datap));
          معلومات = bswap_neon (معلومات);
          uint64_t wcrc = چڪري_جاچ;
          xor_چڪري_جاچ = vcombine_u64 (vcreate_u64 (0), vcreate_u64 (wcrc << 32));
          چڪري_جاچ = 0;
          معلومات = veorq_u64 (معلومات, xor_چڪري_جاچ);
          طالما (بائيٽون_read >= 32)
            {
              datap++;

              data2 =
                vreinterpretq_u64_p128 (vmull_p64
                                        (vgetq_lane_p64
                                         (vreinterpretq_p64_u64 (معلومات), 0),
                                         vgetq_lane_p64 (single_mult_constant,
                                                         0)));
              معلومات =
                vreinterpretq_u64_p128 (vmull_مٿاهون_p64
                                        (vreinterpretq_p64_u64 (معلومات),
                                         single_mult_constant));
              fold_معلومات = vld1q_u64 ((uint64_t *) (datap));
              fold_معلومات = bswap_neon (fold_معلومات);
              معلومات = veorq_u64 (معلومات, data2);
              معلومات = veorq_u64 (معلومات, fold_معلومات);
              بائيٽون_read -= 16;
            }
          معلومات = bswap_neon (معلومات);
          vst1q_u64 ((uint64_t *) (datap), معلومات);
        }

      /* And finish up last 0-31 bytes in a byte by byte fashion */
      بنانشان حرف *cp = (بنانشان حرف *) datap;
      طالما (بائيٽون_read--)
        چڪري_جاچ = (چڪري_جاچ << 8) ^ crctab[0][((چڪري_جاچ >> 24) ^ *cp++) & 0xFF];
      إذا (feof (fp))
        اكسر;
    }

  *چڪري_جاچ_ٻاهريون = چڪري_جاچ;
  *ڊيگهائي_ٻاهريون = ڊيگهائي;

  أرجع !ferror (fp);
}
