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

#جے_بنا_تعریف __CKSUM_CRC_H__
# define __CKSUM_CRC_H__

# include <stdint.h>
# include <stdio.h>

خارجي bool cksum_debug;

خارجي صحيح
چکری_جانچ_جوڑ_دھارا (FILE *دھارا, فراغ *resstream, intmax_t *لمائی);

خارجي صحيح
crc32b_جوڑ_دھارا (FILE *دھارا, فراغ *resstream, intmax_t *لمائی);

خارجي فراغ
output_چکری_جانچ (حرف ثابت *file, صحيح ثنائی_file, فراغ ثابت *digest, bool raw,
            bool tagged, بنانشانی حرف delim, bool دلیلاں, intmax_t لمائی)
  _GL_ATTRIBUTE_NONNULL ((3));

خارجي bool
cksum_vmull (FILE *fp, uint_fast32_t *چکری_جانچ_باہرلا, intmax_t *لمائی_باہرلا);

خارجي bool
cksum_pclmul (FILE *fp, uint_fast32_t *چکری_جانچ_باہرلا, intmax_t *لمائی_باہرلا);

خارجي bool
cksum_avx2 (FILE *fp, uint_fast32_t *چکری_جانچ_باہرلا, intmax_t *لمائی_باہرلا);

خارجي bool
cksum_avx512 (FILE *fp, uint_fast32_t *چکری_جانچ_باہرلا, intmax_t *لمائی_باہرلا);

خارجي uint_fast32_t ثابت crctab[8][256];

#جے_انت
