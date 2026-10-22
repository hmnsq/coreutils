/* Calculate checksums and sizes of files
   Copyright 2025-2026 Free Software Foundation, Inc.

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

#اگر_غیر_تعریف_شدہ __CKSUM_CRC_H__
# define __CKSUM_CRC_H__

# include <stdint.h>
# include <stdio.h>

خارجی bool cksum_debug;

خارجی صحیح
چکری_جانچ_مجموع_دھارا (FILE *دھارا, خالی *resstream, intmax_t *لمبائی);

خارجی صحیح
crc32b_مجموع_دھارا (FILE *دھارا, خالی *resstream, intmax_t *لمبائی);

خارجی خالی
output_چکری_جانچ (حرف ثابت *file, صحیح ثنائی_file, خالی ثابت *digest, bool raw,
            bool tagged, غیرعلامتی حرف delim, bool دلائل, intmax_t لمبائی)
  _GL_ATTRIBUTE_NONNULL ((3));

خارجی bool
cksum_vmull (FILE *fp, uint_fast32_t *چکری_جانچ_برآمد, intmax_t *لمبائی_برآمد);

خارجی bool
cksum_pclmul (FILE *fp, uint_fast32_t *چکری_جانچ_برآمد, intmax_t *لمبائی_برآمد);

خارجی bool
cksum_avx2 (FILE *fp, uint_fast32_t *چکری_جانچ_برآمد, intmax_t *لمبائی_برآمد);

خارجی bool
cksum_avx512 (FILE *fp, uint_fast32_t *چکری_جانچ_برآمد, intmax_t *لمبائی_برآمد);

خارجی uint_fast32_t ثابت crctab[8][256];

#اگر_ختم
