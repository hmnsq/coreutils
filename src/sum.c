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
bsd_جوڙ_وهڪرو (FILE *وهڪرو, فراغ *resstream, intmax_t *ڊيگهائي)
{
  صحيح ret = -1;
  idx_t جوڙ;
  صحيح checksum = 0;	/* The checksum mod 2^16. */
  intmax_t ڪل_بائيٽون = 0;	/* The number of bytes. */
  تعداد { بفر_ڊيگهائي = 32768 };
  uint8_t *بفر = يادگيري_ڏيو (بفر_ڊيگهائي);

  إذا (! بفر)
    أرجع -1;

  /* Process file */
  طالما (true)
  {
    جوڙ = 0;

    /* Read block */
    طالما (true)
    {
      size_t n = fread (بفر + جوڙ, 1, بفر_ڊيگهائي - جوڙ, وهڪرو);
      إذا (n == 0)
        {
          إذا (ferror (وهڪرو))
            اذهب cleanup_بفر;
          اذهب final_process;
        }

      جوڙ += n;

      إذا (بفر_ڊيگهائي == جوڙ)
        اكسر;

      إذا (feof (وهڪرو))
        اذهب final_process;
    }

    لكل (size_t i = 0; i < جوڙ; i++)
      {
        checksum = (checksum >> 1) + ((checksum & 1) << 15);
        checksum += بفر[i];
        checksum &= 0xffff;	/* Keep it within bounds. */
      }
    إذا (ckd_جوڙيو (&ڪل_بائيٽون, ڪل_بائيٽون, جوڙ))
      {
        errno = EOVERFLOW;
        اذهب cleanup_بفر;
      }
  }

final_process:;

  لكل (idx_t i = 0; i < جوڙ; i++)
    {
      checksum = (checksum >> 1) + ((checksum & 1) << 15);
      checksum += بفر[i];
      checksum &= 0xffff;	/* Keep it within bounds. */
    }
  إذا (ckd_جوڙيو (&ڪل_بائيٽون, ڪل_بائيٽون, جوڙ))
    {
      errno = EOVERFLOW;
      اذهب cleanup_بفر;
    }

  يادگيري_نقل (resstream, &checksum, حجم checksum);
  *ڊيگهائي = ڪل_بائيٽون;
  ret = 0;
cleanup_بفر:
  حرر (بفر);
  أرجع ret;
}

/* Calculate the checksum and the size in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

صحيح
sysv_جوڙ_وهڪرو (FILE *وهڪرو, فراغ *resstream, intmax_t *ڊيگهائي)
{
  صحيح ret = -1;
  idx_t جوڙ;
  intmax_t ڪل_بائيٽون = 0;
  تعداد { بفر_ڊيگهائي = 32768 };
  uint8_t *بفر = يادگيري_ڏيو (بفر_ڊيگهائي);

  إذا (! بفر)
    أرجع -1;

  /* The sum of all the input bytes, modulo (UINT_MAX + 1).  */
  بنانشان صحيح s = 0;

  /* Process file */
  طالما (true)
  {
    جوڙ = 0;

    /* Read block */
    طالما (true)
    {
      size_t n = fread (بفر + جوڙ, 1, بفر_ڊيگهائي - جوڙ, وهڪرو);
      إذا (n == 0)
        {
          إذا (ferror (وهڪرو))
            اذهب cleanup_بفر;
          اذهب final_process;
        }

      جوڙ += n;

      إذا (بفر_ڊيگهائي == جوڙ)
        اكسر;

      إذا (feof (وهڪرو))
        اذهب final_process;
    }

    لكل (size_t i = 0; i < جوڙ; i++)
      s += بفر[i];
    إذا (ڪل_بائيٽون + جوڙ < ڪل_بائيٽون)
      {
        errno = EOVERFLOW;
        اذهب cleanup_بفر;
      }
    ڪل_بائيٽون += جوڙ;
  }

final_process:;

  لكل (size_t i = 0; i < جوڙ; i++)
    s += بفر[i];
  إذا (ڪل_بائيٽون + جوڙ < ڪل_بائيٽون)
    {
      errno = EOVERFLOW;
      اذهب cleanup_بفر;
    }
  ڪل_بائيٽون += جوڙ;

  صحيح r = (s & 0xffff) + ((s & 0xffffffff) >> 16);
  صحيح checksum = (r & 0xffff) + (r >> 16);

  يادگيري_نقل (resstream, &checksum, حجم checksum);
  *ڊيگهائي = ڪل_بائيٽون;
  ret = 0;
cleanup_بفر:
  حرر (بفر);
  أرجع ret;
}

/* Print the checksum and size (in 1024 byte blocks) to stdout.
   If ARGS is true, also print the FILE name.  */

فراغ
output_bsd (حرف ثابت *file, MAYBE_اڻ_استعمال_ٿيل صحيح ٻٽو_file, فراغ ثابت *digest,
            bool raw, MAYBE_اڻ_استعمال_ٿيل bool tagged, بنانشان حرف delim, bool دليلون,
            intmax_t ڊيگهائي)
{
  إذا (raw)
    {
      /* Output in network byte order (big endian).  */
      uint16_t ٻاهريون_int = *(صحيح *)digest;
      ٻاهريون_int = htobe16 (ٻاهريون_int);
      fwrite (&ٻاهريون_int, 1, 16/8, stdout);
      أرجع;
    }

  حرف hbuf[LONGEST_HUMAN_READABLE + 1];
  لکو ("%05d %5s", *(صحيح *)digest,
          human_readable (ڊيگهائي, hbuf, human_ceiling, 1, 1024));
  إذا (دليلون)
    لکو (" %s", file);
  اکر_لکو (delim);
}

/* Print the checksum and size (in 512 byte blocks) to stdout.
   If ARGS is true, also print the FILE name.  */

فراغ
output_sysv (حرف ثابت *file, MAYBE_اڻ_استعمال_ٿيل صحيح ٻٽو_file,
             فراغ ثابت *digest, bool raw, MAYBE_اڻ_استعمال_ٿيل bool tagged,
             بنانشان حرف delim, bool دليلون, intmax_t ڊيگهائي)
{
  إذا (raw)
    {
      /* Output in network byte order (big endian).  */
      uint16_t ٻاهريون_int = *(صحيح *)digest;
      ٻاهريون_int = htobe16 (ٻاهريون_int);
      fwrite (&ٻاهريون_int, 1, 16/8, stdout);
      أرجع;
    }

  حرف hbuf[LONGEST_HUMAN_READABLE + 1];
  لکو ("%d %s", *(صحيح *)digest,
          human_readable (ڊيگهائي, hbuf, human_ceiling, 1, 512));
  إذا (دليلون)
    لکو (" %s", file);
  اکر_لکو (delim);
}
