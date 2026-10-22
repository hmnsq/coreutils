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

#ضمن <config.h>

#ضمن <stdio.h>
#ضمن <sys/types.h>
#ضمن <endian.h>
#ضمن "system.h"
#ضمن "human.h"
#ضمن "sum.h"

/* Calculate the checksum and the size in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

صحيح
bsd_مجموعه_بهیر (FILE *بهیر, فراغ *resstream, intmax_t *اوږدتیا)
{
  صحيح ret = -1;
  idx_t مجموعه;
  صحيح checksum = 0;	/* The checksum mod 2^16. */
  intmax_t ټول_بایټونه = 0;	/* The number of bytes. */
  تعداد { بفر_اوږدتیا = 32768 };
  uint8_t *بفر = حافظه_ورکړه (بفر_اوږدتیا);

  إذا (! بفر)
    أرجع -1;

  /* Process file */
  طالما (true)
  {
    مجموعه = 0;

    /* Read block */
    طالما (true)
    {
      size_t n = fread (بفر + مجموعه, 1, بفر_اوږدتیا - مجموعه, بهیر);
      إذا (n == 0)
        {
          إذا (ferror (بهیر))
            اذهب cleanup_بفر;
          اذهب final_process;
        }

      مجموعه += n;

      إذا (بفر_اوږدتیا == مجموعه)
        اكسر;

      إذا (feof (بهیر))
        اذهب final_process;
    }

    لكل (size_t i = 0; i < مجموعه; i++)
      {
        checksum = (checksum >> 1) + ((checksum & 1) << 15);
        checksum += بفر[i];
        checksum &= 0xffff;	/* Keep it within bounds. */
      }
    إذا (ckd_جمع_کړه (&ټول_بایټونه, ټول_بایټونه, مجموعه))
      {
        errno = EOVERFLOW;
        اذهب cleanup_بفر;
      }
  }

final_process:;

  لكل (idx_t i = 0; i < مجموعه; i++)
    {
      checksum = (checksum >> 1) + ((checksum & 1) << 15);
      checksum += بفر[i];
      checksum &= 0xffff;	/* Keep it within bounds. */
    }
  إذا (ckd_جمع_کړه (&ټول_بایټونه, ټول_بایټونه, مجموعه))
    {
      errno = EOVERFLOW;
      اذهب cleanup_بفر;
    }

  حافظه_نقل (resstream, &checksum, حجم checksum);
  *اوږدتیا = ټول_بایټونه;
  ret = 0;
cleanup_بفر:
  حرر (بفر);
  أرجع ret;
}

/* Calculate the checksum and the size in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

صحيح
sysv_مجموعه_بهیر (FILE *بهیر, فراغ *resstream, intmax_t *اوږدتیا)
{
  صحيح ret = -1;
  idx_t مجموعه;
  intmax_t ټول_بایټونه = 0;
  تعداد { بفر_اوږدتیا = 32768 };
  uint8_t *بفر = حافظه_ورکړه (بفر_اوږدتیا);

  إذا (! بفر)
    أرجع -1;

  /* The sum of all the input bytes, modulo (UINT_MAX + 1).  */
  بینښه صحيح s = 0;

  /* Process file */
  طالما (true)
  {
    مجموعه = 0;

    /* Read block */
    طالما (true)
    {
      size_t n = fread (بفر + مجموعه, 1, بفر_اوږدتیا - مجموعه, بهیر);
      إذا (n == 0)
        {
          إذا (ferror (بهیر))
            اذهب cleanup_بفر;
          اذهب final_process;
        }

      مجموعه += n;

      إذا (بفر_اوږدتیا == مجموعه)
        اكسر;

      إذا (feof (بهیر))
        اذهب final_process;
    }

    لكل (size_t i = 0; i < مجموعه; i++)
      s += بفر[i];
    إذا (ټول_بایټونه + مجموعه < ټول_بایټونه)
      {
        errno = EOVERFLOW;
        اذهب cleanup_بفر;
      }
    ټول_بایټونه += مجموعه;
  }

final_process:;

  لكل (size_t i = 0; i < مجموعه; i++)
    s += بفر[i];
  إذا (ټول_بایټونه + مجموعه < ټول_بایټونه)
    {
      errno = EOVERFLOW;
      اذهب cleanup_بفر;
    }
  ټول_بایټونه += مجموعه;

  صحيح r = (s & 0xffff) + ((s & 0xffffffff) >> 16);
  صحيح checksum = (r & 0xffff) + (r >> 16);

  حافظه_نقل (resstream, &checksum, حجم checksum);
  *اوږدتیا = ټول_بایټونه;
  ret = 0;
cleanup_بفر:
  حرر (بفر);
  أرجع ret;
}

/* Print the checksum and size (in 1024 byte blocks) to stdout.
   If ARGS is true, also print the FILE name.  */

فراغ
output_bsd (حرف ثابت *file, MAYBE_ناکارول_شوی صحيح دوګونی_file, فراغ ثابت *digest,
            bool raw, MAYBE_ناکارول_شوی bool tagged, بینښه حرف delim, bool دلیلونه,
            intmax_t اوږدتیا)
{
  إذا (raw)
    {
      /* Output in network byte order (big endian).  */
      uint16_t بهر_int = *(صحيح *)digest;
      بهر_int = htobe16 (بهر_int);
      fwrite (&بهر_int, 1, 16/8, stdout);
      أرجع;
    }

  حرف hbuf[LONGEST_HUMAN_READABLE + 1];
  چاپ_کړه ("%05d %5s", *(صحيح *)digest,
          human_readable (اوږدتیا, hbuf, human_ceiling, 1, 1024));
  إذا (دلیلونه)
    چاپ_کړه (" %s", file);
  توری_چاپ_کړه (delim);
}

/* Print the checksum and size (in 512 byte blocks) to stdout.
   If ARGS is true, also print the FILE name.  */

فراغ
output_sysv (حرف ثابت *file, MAYBE_ناکارول_شوی صحيح دوګونی_file,
             فراغ ثابت *digest, bool raw, MAYBE_ناکارول_شوی bool tagged,
             بینښه حرف delim, bool دلیلونه, intmax_t اوږدتیا)
{
  إذا (raw)
    {
      /* Output in network byte order (big endian).  */
      uint16_t بهر_int = *(صحيح *)digest;
      بهر_int = htobe16 (بهر_int);
      fwrite (&بهر_int, 1, 16/8, stdout);
      أرجع;
    }

  حرف hbuf[LONGEST_HUMAN_READABLE + 1];
  چاپ_کړه ("%d %s", *(صحيح *)digest,
          human_readable (اوږدتیا, hbuf, human_ceiling, 1, 512));
  إذا (دلیلونه)
    چاپ_کړه (" %s", file);
  توری_چاپ_کړه (delim);
}
