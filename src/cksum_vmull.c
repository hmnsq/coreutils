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

#समावेश <config.h>

#समावेश "cksum_crc.h"

#समावेश <stdio.h>
#समावेश <sys/types.h>
#समावेश <arm_neon.h>
#समावेश "system.h"

/* Number of bytes to read at once.  */
#घोषणा BUFLEN (1 << 16)

अटल uint64x2_t
bswap_neon (uint64x2_t in)
{
  uint64x2_t a =
    vreinterpretq_u64_u8 (vrev64q_u8 (vreinterpretq_u8_u64 (in)));
  a = vcombine_u64 (vget_ऊपरी_u64 (a), vget_निचला_u64 (a));
  वापस a;
}

/* Calculate CRC32 using VMULL CPU instruction found in ARMv8 CPUs */

bool
cksum_vmull (खाता *fp, uint_fast32_t *crc_out, intmax_t *लंबाई_out)
{
  uint64x2_t buf[BUFLEN / माप (uint64x2_t)];
  uint_fast32_t crc = 0;
  intmax_t लंबाई = 0;
  माप_प्रकार bytes_read;
  poly64x2_t single_mult_constant;
  poly64x2_t four_mult_constant;

  अगर (!fp || !crc_out || !लंबाई_out)
    वापस false;

  /* These constants and general algorithms are taken from the Intel whitepaper
     "Fast CRC Computation for Generic Polynomials Using PCLMULQDQ Instruction"
   */
  single_mult_constant =
    vcombine_p64 (vcreate_p64 (0xE8A45605), vcreate_p64 (0xC5B9CD4C));
  four_mult_constant =
    vcombine_p64 (vcreate_p64 (0xE6228B11), vcreate_p64 (0x8833794C));

  जबतक ((bytes_read = ख_पढ़ो (buf, 1, BUFLEN, fp)) > 0)
    {
      uint64x2_t *datap;
      uint64x2_t तथ्य;
      uint64x2_t data2;
      uint64x2_t data3;
      uint64x2_t data4;
      uint64x2_t data5;
      uint64x2_t data6;
      uint64x2_t data7;
      uint64x2_t data8;
      uint64x2_t fold_तथ्य;
      uint64x2_t xor_crc;

      अगर (ckd_add (&लंबाई, लंबाई, bytes_read))
        {
          त्रुटि_सं = EOVERFLOW;
          वापस false;
        }

      datap = (uint64x2_t *) buf;

      /* Fold in parallel eight 16-byte blocks into four 16-byte blocks */
      अगर (bytes_read >= 16 * 8)
        {
          तथ्य = vld1q_u64 ((uint64_t *) (datap));
          तथ्य = bswap_neon (तथ्य);
          /* XOR in initial CRC value (for us 0 so no effect), or CRC value
             calculated for previous BUFLEN buffer from fread */
          uint64_t wcrc = crc;
          xor_crc = vcombine_u64 (vcreate_u64 (0), vcreate_u64 (wcrc << 32));
          crc = 0;
          तथ्य = veorq_u64 (तथ्य, xor_crc);
          data3 = vld1q_u64 ((uint64_t *) (datap + 1));
          data3 = bswap_neon (data3);
          data5 = vld1q_u64 ((uint64_t *) (datap + 2));
          data5 = bswap_neon (data5);
          data7 = vld1q_u64 ((uint64_t *) (datap + 3));
          data7 = bswap_neon (data7);


          जबतक (bytes_read >= 16 * 8)
            {
              datap += 4;

              /* Do multiplication here for four consecutive 16 byte blocks */
              data2 =
                vreinterpretq_u64_p128 (vmull_p64
                                        (vgetq_lane_p64
                                         (vreinterpretq_p64_u64 (तथ्य), 0),
                                         vgetq_lane_p64 (four_mult_constant,
                                                         0)));
              तथ्य =
                vreinterpretq_u64_p128 (vmull_ऊपरी_p64
                                        (vreinterpretq_p64_u64 (तथ्य),
                                         four_mult_constant));
              data4 =
                vreinterpretq_u64_p128 (vmull_p64
                                        (vgetq_lane_p64
                                         (vreinterpretq_p64_u64 (data3), 0),
                                         vgetq_lane_p64 (four_mult_constant,
                                                         0)));
              data3 =
                vreinterpretq_u64_p128 (vmull_ऊपरी_p64
                                        (vreinterpretq_p64_u64 (data3),
                                         four_mult_constant));
              data6 =
                vreinterpretq_u64_p128 (vmull_p64
                                        (vgetq_lane_p64
                                         (vreinterpretq_p64_u64 (data5), 0),
                                         vgetq_lane_p64 (four_mult_constant,
                                                         0)));
              data5 =
                vreinterpretq_u64_p128 (vmull_ऊपरी_p64
                                        (vreinterpretq_p64_u64 (data5),
                                         four_mult_constant));
              data8 =
                vreinterpretq_u64_p128 (vmull_p64
                                        (vgetq_lane_p64
                                         (vreinterpretq_p64_u64 (data7), 0),
                                         vgetq_lane_p64 (four_mult_constant,
                                                         0)));
              data7 =
                vreinterpretq_u64_p128 (vmull_ऊपरी_p64
                                        (vreinterpretq_p64_u64 (data7),
                                         four_mult_constant));

              /* Now multiplication results for the four blocks is xor:ed with
                 next four 16 byte blocks from the buffer. This effectively
                 "consumes" the first four blocks from the buffer.
                 Keep xor result in variables for multiplication in next
                 round of loop. */
              तथ्य = veorq_u64 (तथ्य, data2);
              data2 = vld1q_u64 ((uint64_t *) (datap));
              data2 = bswap_neon (data2);
              तथ्य = veorq_u64 (तथ्य, data2);

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

              bytes_read -= (16 * 4);
            }
          /* At end of loop we write out results from variables back into
             the buffer, for use in single fold loop */
          तथ्य = bswap_neon (तथ्य);
          vst1q_u64 ((uint64_t *) (datap), तथ्य);
          data3 = bswap_neon (data3);
          vst1q_u64 ((uint64_t *) (datap + 1), data3);
          data5 = bswap_neon (data5);
          vst1q_u64 ((uint64_t *) (datap + 2), data5);
          data7 = bswap_neon (data7);
          vst1q_u64 ((uint64_t *) (datap + 3), data7);
        }

      /* Fold two 16-byte blocks into one 16-byte block */
      अगर (bytes_read >= 32)
        {
          तथ्य = vld1q_u64 ((uint64_t *) (datap));
          तथ्य = bswap_neon (तथ्य);
          uint64_t wcrc = crc;
          xor_crc = vcombine_u64 (vcreate_u64 (0), vcreate_u64 (wcrc << 32));
          crc = 0;
          तथ्य = veorq_u64 (तथ्य, xor_crc);
          जबतक (bytes_read >= 32)
            {
              datap++;

              data2 =
                vreinterpretq_u64_p128 (vmull_p64
                                        (vgetq_lane_p64
                                         (vreinterpretq_p64_u64 (तथ्य), 0),
                                         vgetq_lane_p64 (single_mult_constant,
                                                         0)));
              तथ्य =
                vreinterpretq_u64_p128 (vmull_ऊपरी_p64
                                        (vreinterpretq_p64_u64 (तथ्य),
                                         single_mult_constant));
              fold_तथ्य = vld1q_u64 ((uint64_t *) (datap));
              fold_तथ्य = bswap_neon (fold_तथ्य);
              तथ्य = veorq_u64 (तथ्य, data2);
              तथ्य = veorq_u64 (तथ्य, fold_तथ्य);
              bytes_read -= 16;
            }
          तथ्य = bswap_neon (तथ्य);
          vst1q_u64 ((uint64_t *) (datap), तथ्य);
        }

      /* And finish up last 0-31 bytes in a byte by byte fashion */
      अचिन्हित अक्षर *cp = (अचिन्हित अक्षर *) datap;
      जबतक (bytes_read--)
        crc = (crc << 8) ^ crctab[0][((crc >> 24) ^ *cp++) & 0xFF];
      अगर (ख_पूर्ण (fp))
        अवरोध;
    }

  *crc_out = crc;
  *लंबाई_out = लंबाई;

  वापस !ख_त्रुटि (fp);
}
