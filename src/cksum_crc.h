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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ __CKSUM_CRC_H__
# define __CKSUM_CRC_H__

# include <stdint.h>
# include <stdio.h>

ಬಾಹ್ಯ bool cksum_debug;

ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ
ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_ಮೊತ್ತ_ಹರಿವು (FILE *ಹರಿವು, ಶೂನ್ಯ *resstream, intmax_t *ಉದ್ದಳತೆ);

ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ
crc32b_ಮೊತ್ತ_ಹರಿವು (FILE *ಹರಿವು, ಶೂನ್ಯ *resstream, intmax_t *ಉದ್ದಳತೆ);

ಬಾಹ್ಯ ಶೂನ್ಯ
output_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file, ಪೂರ್ಣಾಂಕ ದ್ವಿಮಾನ_file, ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *digest, bool raw,
            bool tagged, ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ delim, bool ವಾದಗಳು, intmax_t ಉದ್ದಳತೆ)
  _GL_ATTRIBUTE_NONNULL ((3));

ಬಾಹ್ಯ bool
cksum_vmull (FILE *fp, uint_fast32_t *ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_ಹೊರಹೋಗು, intmax_t *ಉದ್ದಳತೆ_ಹೊರಹೋಗು);

ಬಾಹ್ಯ bool
cksum_pclmul (FILE *fp, uint_fast32_t *ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_ಹೊರಹೋಗು, intmax_t *ಉದ್ದಳತೆ_ಹೊರಹೋಗು);

ಬಾಹ್ಯ bool
cksum_avx2 (FILE *fp, uint_fast32_t *ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_ಹೊರಹೋಗು, intmax_t *ಉದ್ದಳತೆ_ಹೊರಹೋಗು);

ಬಾಹ್ಯ bool
cksum_avx512 (FILE *fp, uint_fast32_t *ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_ಹೊರಹೋಗು, intmax_t *ಉದ್ದಳತೆ_ಹೊರಹೋಗು);

ಬಾಹ್ಯ uint_fast32_t ಸ್ಥಿರಾಂಕ crctab[8][256];

#ಆದರೆ_ಅಂತ್ಯ
