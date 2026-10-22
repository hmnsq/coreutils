/* sum -- checksum and count the blocks in a file
   Copyright (C) 1986-2026 Free Software Foundation, Inc.

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

/* Like BSD sum or SysV sum -r, except like SysV sum if -s option is given. */

/* Written by Kayvan Aghaiepour and David MacKenzie. */

#ಅನ್ತರ್ಭಾವ <config.h>

#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <endian.h>
#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "human.h"
#ಅನ್ತರ್ಭಾವ "sum.h"

/* Calculate the checksum and the size in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

ಪೂರ್ಣಾಂಕ
bsd_ಮೊತ್ತ_ಹರಿವು (FILE *ಹರಿವು, ಶೂನ್ಯ *resstream, intmax_t *ಉದ್ದಳತೆ)
{
  ಪೂರ್ಣಾಂಕ ret = -1;
  idx_t ಮೊತ್ತ;
  ಪೂರ್ಣಾಂಕ checksum = 0;	/* The checksum mod 2^16. */
  intmax_t ಒಟ್ಟು_ಬೈಟುಗಳು = 0;	/* The number of bytes. */
  ಎಣಿಕೆ { ಬಫರ್_ಉದ್ದಳತೆ = 32768 };
  uint8_t *ಬಫರ್ = ಸ್ಮರಣೆ_ಕೊಡು (ಬಫರ್_ಉದ್ದಳತೆ);

  ಆದರೆ (! ಬಫರ್)
    ಹಿಂತಿರುಗಿ -1;

  /* Process file */
  ಆಗ (true)
  {
    ಮೊತ್ತ = 0;

    /* Read block */
    ಆಗ (true)
    {
      size_t n = fread (ಬಫರ್ + ಮೊತ್ತ, 1, ಬಫರ್_ಉದ್ದಳತೆ - ಮೊತ್ತ, ಹರಿವು);
      ಆದರೆ (n == 0)
        {
          ಆದರೆ (ferror (ಹರಿವು))
            ಹೋಗು cleanup_ಬಫರ್;
          ಹೋಗು final_process;
        }

      ಮೊತ್ತ += n;

      ಆದರೆ (ಬಫರ್_ಉದ್ದಳತೆ == ಮೊತ್ತ)
        ನಿಲ್ಲಿಸು;

      ಆದರೆ (feof (ಹರಿವು))
        ಹೋಗು final_process;
    }

    ಗಾಗಿ (size_t i = 0; i < ಮೊತ್ತ; i++)
      {
        checksum = (checksum >> 1) + ((checksum & 1) << 15);
        checksum += ಬಫರ್[i];
        checksum &= 0xffff;	/* Keep it within bounds. */
      }
    ಆದರೆ (ckd_ಕೂಡಿಸು (&ಒಟ್ಟು_ಬೈಟುಗಳು, ಒಟ್ಟು_ಬೈಟುಗಳು, ಮೊತ್ತ))
      {
        errno = EOVERFLOW;
        ಹೋಗು cleanup_ಬಫರ್;
      }
  }

final_process:;

  ಗಾಗಿ (idx_t i = 0; i < ಮೊತ್ತ; i++)
    {
      checksum = (checksum >> 1) + ((checksum & 1) << 15);
      checksum += ಬಫರ್[i];
      checksum &= 0xffff;	/* Keep it within bounds. */
    }
  ಆದರೆ (ckd_ಕೂಡಿಸು (&ಒಟ್ಟು_ಬೈಟುಗಳು, ಒಟ್ಟು_ಬೈಟುಗಳು, ಮೊತ್ತ))
    {
      errno = EOVERFLOW;
      ಹೋಗು cleanup_ಬಫರ್;
    }

  ಸ್ಮರಣೆ_ನಕಲು (resstream, &checksum, ಗಾತ್ರ checksum);
  *ಉದ್ದಳತೆ = ಒಟ್ಟು_ಬೈಟುಗಳು;
  ret = 0;
cleanup_ಬಫರ್:
  ಬಿಡುಗಡೆ (ಬಫರ್);
  ಹಿಂತಿರುಗಿ ret;
}

/* Calculate the checksum and the size in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

ಪೂರ್ಣಾಂಕ
sysv_ಮೊತ್ತ_ಹರಿವು (FILE *ಹರಿವು, ಶೂನ್ಯ *resstream, intmax_t *ಉದ್ದಳತೆ)
{
  ಪೂರ್ಣಾಂಕ ret = -1;
  idx_t ಮೊತ್ತ;
  intmax_t ಒಟ್ಟು_ಬೈಟುಗಳು = 0;
  ಎಣಿಕೆ { ಬಫರ್_ಉದ್ದಳತೆ = 32768 };
  uint8_t *ಬಫರ್ = ಸ್ಮರಣೆ_ಕೊಡು (ಬಫರ್_ಉದ್ದಳತೆ);

  ಆದರೆ (! ಬಫರ್)
    ಹಿಂತಿರುಗಿ -1;

  /* The sum of all the input bytes, modulo (UINT_MAX + 1).  */
  ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ s = 0;

  /* Process file */
  ಆಗ (true)
  {
    ಮೊತ್ತ = 0;

    /* Read block */
    ಆಗ (true)
    {
      size_t n = fread (ಬಫರ್ + ಮೊತ್ತ, 1, ಬಫರ್_ಉದ್ದಳತೆ - ಮೊತ್ತ, ಹರಿವು);
      ಆದರೆ (n == 0)
        {
          ಆದರೆ (ferror (ಹರಿವು))
            ಹೋಗು cleanup_ಬಫರ್;
          ಹೋಗು final_process;
        }

      ಮೊತ್ತ += n;

      ಆದರೆ (ಬಫರ್_ಉದ್ದಳತೆ == ಮೊತ್ತ)
        ನಿಲ್ಲಿಸು;

      ಆದರೆ (feof (ಹರಿವು))
        ಹೋಗು final_process;
    }

    ಗಾಗಿ (size_t i = 0; i < ಮೊತ್ತ; i++)
      s += ಬಫರ್[i];
    ಆದರೆ (ಒಟ್ಟು_ಬೈಟುಗಳು + ಮೊತ್ತ < ಒಟ್ಟು_ಬೈಟುಗಳು)
      {
        errno = EOVERFLOW;
        ಹೋಗು cleanup_ಬಫರ್;
      }
    ಒಟ್ಟು_ಬೈಟುಗಳು += ಮೊತ್ತ;
  }

final_process:;

  ಗಾಗಿ (size_t i = 0; i < ಮೊತ್ತ; i++)
    s += ಬಫರ್[i];
  ಆದರೆ (ಒಟ್ಟು_ಬೈಟುಗಳು + ಮೊತ್ತ < ಒಟ್ಟು_ಬೈಟುಗಳು)
    {
      errno = EOVERFLOW;
      ಹೋಗು cleanup_ಬಫರ್;
    }
  ಒಟ್ಟು_ಬೈಟುಗಳು += ಮೊತ್ತ;

  ಪೂರ್ಣಾಂಕ r = (s & 0xffff) + ((s & 0xffffffff) >> 16);
  ಪೂರ್ಣಾಂಕ checksum = (r & 0xffff) + (r >> 16);

  ಸ್ಮರಣೆ_ನಕಲು (resstream, &checksum, ಗಾತ್ರ checksum);
  *ಉದ್ದಳತೆ = ಒಟ್ಟು_ಬೈಟುಗಳು;
  ret = 0;
cleanup_ಬಫರ್:
  ಬಿಡುಗಡೆ (ಬಫರ್);
  ಹಿಂತಿರುಗಿ ret;
}

/* Print the checksum and size (in 1024 byte blocks) to stdout.
   If ARGS is true, also print the FILE name.  */

ಶೂನ್ಯ
output_bsd (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file, MAYBE_ಬಳಸದ ಪೂರ್ಣಾಂಕ ದ್ವಿಮಾನ_file, ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *digest,
            bool raw, MAYBE_ಬಳಸದ bool tagged, ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ delim, bool ವಾದಗಳು,
            intmax_t ಉದ್ದಳತೆ)
{
  ಆದರೆ (raw)
    {
      /* Output in network byte order (big endian).  */
      uint16_t ಹೊರಹೋಗು_int = *(ಪೂರ್ಣಾಂಕ *)digest;
      ಹೊರಹೋಗು_int = htobe16 (ಹೊರಹೋಗು_int);
      fwrite (&ಹೊರಹೋಗು_int, 1, 16/8, stdout);
      ಹಿಂತಿರುಗಿ;
    }

  ಅಕ್ಷರ hbuf[LONGEST_HUMAN_READABLE + 1];
  ಬರೆ ("%05d %5s", *(ಪೂರ್ಣಾಂಕ *)digest,
          human_readable (ಉದ್ದಳತೆ, hbuf, human_ceiling, 1, 1024));
  ಆದರೆ (ವಾದಗಳು)
    ಬರೆ (" %s", file);
  ಅಕ್ಷರ_ಬರೆ (delim);
}

/* Print the checksum and size (in 512 byte blocks) to stdout.
   If ARGS is true, also print the FILE name.  */

ಶೂನ್ಯ
output_sysv (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file, MAYBE_ಬಳಸದ ಪೂರ್ಣಾಂಕ ದ್ವಿಮಾನ_file,
             ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *digest, bool raw, MAYBE_ಬಳಸದ bool tagged,
             ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ delim, bool ವಾದಗಳು, intmax_t ಉದ್ದಳತೆ)
{
  ಆದರೆ (raw)
    {
      /* Output in network byte order (big endian).  */
      uint16_t ಹೊರಹೋಗು_int = *(ಪೂರ್ಣಾಂಕ *)digest;
      ಹೊರಹೋಗು_int = htobe16 (ಹೊರಹೋಗು_int);
      fwrite (&ಹೊರಹೋಗು_int, 1, 16/8, stdout);
      ಹಿಂತಿರುಗಿ;
    }

  ಅಕ್ಷರ hbuf[LONGEST_HUMAN_READABLE + 1];
  ಬರೆ ("%d %s", *(ಪೂರ್ಣಾಂಕ *)digest,
          human_readable (ಉದ್ದಳತೆ, hbuf, human_ceiling, 1, 512));
  ಆದರೆ (ವಾದಗಳು)
    ಬರೆ (" %s", file);
  ಅಕ್ಷರ_ಬರೆ (delim);
}
