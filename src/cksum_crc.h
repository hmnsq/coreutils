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

#నిర్వచితం_కాకపోతే __CKSUM_CRC_H__
# define __CKSUM_CRC_H__

# include <stdint.h>
# include <stdio.h>

బాహ్య bool cksum_debug;

బాహ్య పూర్ణాంకం
చక్రీయ_తనిఖీ_కూడిక_ప్రవాహం (FILE *ప్రవాహం, శూన్య *resstream, intmax_t *నిడివి);

బాహ్య పూర్ణాంకం
crc32b_కూడిక_ప్రవాహం (FILE *ప్రవాహం, శూన్య *resstream, intmax_t *నిడివి);

బాహ్య శూన్య
output_చక్రీయ_తనిఖీ (అక్షరం స్థిరరాశి *file, పూర్ణాంకం ద్విమాన_file, శూన్య స్థిరరాశి *digest, bool raw,
            bool tagged, గుర్తులేని అక్షరం delim, bool వాదనలు, intmax_t నిడివి)
  _GL_ATTRIBUTE_NONNULL ((3));

బాహ్య bool
cksum_vmull (FILE *fp, uint_fast32_t *చక్రీయ_తనిఖీ_బహిర్గతం, intmax_t *నిడివి_బహిర్గతం);

బాహ్య bool
cksum_pclmul (FILE *fp, uint_fast32_t *చక్రీయ_తనిఖీ_బహిర్గతం, intmax_t *నిడివి_బహిర్గతం);

బాహ్య bool
cksum_avx2 (FILE *fp, uint_fast32_t *చక్రీయ_తనిఖీ_బహిర్గతం, intmax_t *నిడివి_బహిర్గతం);

బాహ్య bool
cksum_avx512 (FILE *fp, uint_fast32_t *చక్రీయ_తనిఖీ_బహిర్గతం, intmax_t *నిడివి_బహిర్గతం);

బాహ్య uint_fast32_t స్థిరరాశి crctab[8][256];

#అయితే_ముగింపు
