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
bsd_جمع_جریان (FILE *جریان, فراغ *resstream, intmax_t *درازی)
{
  صحيح ret = -1;
  idx_t جمع;
  صحيح checksum = 0;	/* The checksum mod 2^16. */
  intmax_t مجموع_بایتها = 0;	/* The number of bytes. */
  تعداد { میانگیر_درازی = 32768 };
  uint8_t *میانگیر = حافظه_بده (میانگیر_درازی);

  إذا (! میانگیر)
    أرجع -1;

  /* Process file */
  طالما (true)
  {
    جمع = 0;

    /* Read block */
    طالما (true)
    {
      size_t n = fread (میانگیر + جمع, 1, میانگیر_درازی - جمع, جریان);
      إذا (n == 0)
        {
          إذا (ferror (جریان))
            اذهب cleanup_میانگیر;
          اذهب final_process;
        }

      جمع += n;

      إذا (میانگیر_درازی == جمع)
        اكسر;

      إذا (feof (جریان))
        اذهب final_process;
    }

    لكل (size_t i = 0; i < جمع; i++)
      {
        checksum = (checksum >> 1) + ((checksum & 1) << 15);
        checksum += میانگیر[i];
        checksum &= 0xffff;	/* Keep it within bounds. */
      }
    إذا (ckd_جمع_کن (&مجموع_بایتها, مجموع_بایتها, جمع))
      {
        errno = EOVERFLOW;
        اذهب cleanup_میانگیر;
      }
  }

final_process:;

  لكل (idx_t i = 0; i < جمع; i++)
    {
      checksum = (checksum >> 1) + ((checksum & 1) << 15);
      checksum += میانگیر[i];
      checksum &= 0xffff;	/* Keep it within bounds. */
    }
  إذا (ckd_جمع_کن (&مجموع_بایتها, مجموع_بایتها, جمع))
    {
      errno = EOVERFLOW;
      اذهب cleanup_میانگیر;
    }

  کپی_حافظه (resstream, &checksum, حجم checksum);
  *درازی = مجموع_بایتها;
  ret = 0;
cleanup_میانگیر:
  حرر (میانگیر);
  أرجع ret;
}

/* Calculate the checksum and the size in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

صحيح
sysv_جمع_جریان (FILE *جریان, فراغ *resstream, intmax_t *درازی)
{
  صحيح ret = -1;
  idx_t جمع;
  intmax_t مجموع_بایتها = 0;
  تعداد { میانگیر_درازی = 32768 };
  uint8_t *میانگیر = حافظه_بده (میانگیر_درازی);

  إذا (! میانگیر)
    أرجع -1;

  /* The sum of all the input bytes, modulo (UINT_MAX + 1).  */
  بیعلامت صحيح s = 0;

  /* Process file */
  طالما (true)
  {
    جمع = 0;

    /* Read block */
    طالما (true)
    {
      size_t n = fread (میانگیر + جمع, 1, میانگیر_درازی - جمع, جریان);
      إذا (n == 0)
        {
          إذا (ferror (جریان))
            اذهب cleanup_میانگیر;
          اذهب final_process;
        }

      جمع += n;

      إذا (میانگیر_درازی == جمع)
        اكسر;

      إذا (feof (جریان))
        اذهب final_process;
    }

    لكل (size_t i = 0; i < جمع; i++)
      s += میانگیر[i];
    إذا (مجموع_بایتها + جمع < مجموع_بایتها)
      {
        errno = EOVERFLOW;
        اذهب cleanup_میانگیر;
      }
    مجموع_بایتها += جمع;
  }

final_process:;

  لكل (size_t i = 0; i < جمع; i++)
    s += میانگیر[i];
  إذا (مجموع_بایتها + جمع < مجموع_بایتها)
    {
      errno = EOVERFLOW;
      اذهب cleanup_میانگیر;
    }
  مجموع_بایتها += جمع;

  صحيح r = (s & 0xffff) + ((s & 0xffffffff) >> 16);
  صحيح checksum = (r & 0xffff) + (r >> 16);

  کپی_حافظه (resstream, &checksum, حجم checksum);
  *درازی = مجموع_بایتها;
  ret = 0;
cleanup_میانگیر:
  حرر (میانگیر);
  أرجع ret;
}

/* Print the checksum and size (in 1024 byte blocks) to stdout.
   If ARGS is true, also print the FILE name.  */

فراغ
output_bsd (حرف ثابت *file, MAYBE_استفادهنشده صحيح دودویی_file, فراغ ثابت *digest,
            bool raw, MAYBE_استفادهنشده bool tagged, بیعلامت حرف delim, bool آرگومانها,
            intmax_t درازی)
{
  إذا (raw)
    {
      /* Output in network byte order (big endian).  */
      uint16_t بیرون_int = *(صحيح *)digest;
      بیرون_int = htobe16 (بیرون_int);
      fwrite (&بیرون_int, 1, 16/8, stdout);
      أرجع;
    }

  حرف hbuf[LONGEST_HUMAN_READABLE + 1];
  چاپ_کن ("%05d %5s", *(صحيح *)digest,
          human_readable (درازی, hbuf, human_ceiling, 1, 1024));
  إذا (آرگومانها)
    چاپ_کن (" %s", file);
  حرف_چاپ_کن (delim);
}

/* Print the checksum and size (in 512 byte blocks) to stdout.
   If ARGS is true, also print the FILE name.  */

فراغ
output_sysv (حرف ثابت *file, MAYBE_استفادهنشده صحيح دودویی_file,
             فراغ ثابت *digest, bool raw, MAYBE_استفادهنشده bool tagged,
             بیعلامت حرف delim, bool آرگومانها, intmax_t درازی)
{
  إذا (raw)
    {
      /* Output in network byte order (big endian).  */
      uint16_t بیرون_int = *(صحيح *)digest;
      بیرون_int = htobe16 (بیرون_int);
      fwrite (&بیرون_int, 1, 16/8, stdout);
      أرجع;
    }

  حرف hbuf[LONGEST_HUMAN_READABLE + 1];
  چاپ_کن ("%d %s", *(صحيح *)digest,
          human_readable (درازی, hbuf, human_ceiling, 1, 512));
  إذا (آرگومانها)
    چاپ_کن (" %s", file);
  حرف_چاپ_کن (delim);
}
