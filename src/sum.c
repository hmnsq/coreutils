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
bsd_جوڑ_دھارا (FILE *دھارا, فراغ *resstream, intmax_t *لمائی)
{
  صحيح ret = -1;
  idx_t جوڑ;
  صحيح checksum = 0;	/* The checksum mod 2^16. */
  intmax_t کل_بائٹاں = 0;	/* The number of bytes. */
  تعداد { بفر_لمائی = 32768 };
  uint8_t *بفر = حافظہ_دیو (بفر_لمائی);

  إذا (! بفر)
    أرجع -1;

  /* Process file */
  طالما (true)
  {
    جوڑ = 0;

    /* Read block */
    طالما (true)
    {
      size_t n = fread (بفر + جوڑ, 1, بفر_لمائی - جوڑ, دھارا);
      إذا (n == 0)
        {
          إذا (ferror (دھارا))
            اذهب cleanup_بفر;
          اذهب final_process;
        }

      جوڑ += n;

      إذا (بفر_لمائی == جوڑ)
        اكسر;

      إذا (feof (دھارا))
        اذهب final_process;
    }

    لكل (size_t i = 0; i < جوڑ; i++)
      {
        checksum = (checksum >> 1) + ((checksum & 1) << 15);
        checksum += بفر[i];
        checksum &= 0xffff;	/* Keep it within bounds. */
      }
    إذا (ckd_جوڑو (&کل_بائٹاں, کل_بائٹاں, جوڑ))
      {
        errno = EOVERFLOW;
        اذهب cleanup_بفر;
      }
  }

final_process:;

  لكل (idx_t i = 0; i < جوڑ; i++)
    {
      checksum = (checksum >> 1) + ((checksum & 1) << 15);
      checksum += بفر[i];
      checksum &= 0xffff;	/* Keep it within bounds. */
    }
  إذا (ckd_جوڑو (&کل_بائٹاں, کل_بائٹاں, جوڑ))
    {
      errno = EOVERFLOW;
      اذهب cleanup_بفر;
    }

  حافظہ_نقل (resstream, &checksum, حجم checksum);
  *لمائی = کل_بائٹاں;
  ret = 0;
cleanup_بفر:
  حرر (بفر);
  أرجع ret;
}

/* Calculate the checksum and the size in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

صحيح
sysv_جوڑ_دھارا (FILE *دھارا, فراغ *resstream, intmax_t *لمائی)
{
  صحيح ret = -1;
  idx_t جوڑ;
  intmax_t کل_بائٹاں = 0;
  تعداد { بفر_لمائی = 32768 };
  uint8_t *بفر = حافظہ_دیو (بفر_لمائی);

  إذا (! بفر)
    أرجع -1;

  /* The sum of all the input bytes, modulo (UINT_MAX + 1).  */
  بنانشانی صحيح s = 0;

  /* Process file */
  طالما (true)
  {
    جوڑ = 0;

    /* Read block */
    طالما (true)
    {
      size_t n = fread (بفر + جوڑ, 1, بفر_لمائی - جوڑ, دھارا);
      إذا (n == 0)
        {
          إذا (ferror (دھارا))
            اذهب cleanup_بفر;
          اذهب final_process;
        }

      جوڑ += n;

      إذا (بفر_لمائی == جوڑ)
        اكسر;

      إذا (feof (دھارا))
        اذهب final_process;
    }

    لكل (size_t i = 0; i < جوڑ; i++)
      s += بفر[i];
    إذا (کل_بائٹاں + جوڑ < کل_بائٹاں)
      {
        errno = EOVERFLOW;
        اذهب cleanup_بفر;
      }
    کل_بائٹاں += جوڑ;
  }

final_process:;

  لكل (size_t i = 0; i < جوڑ; i++)
    s += بفر[i];
  إذا (کل_بائٹاں + جوڑ < کل_بائٹاں)
    {
      errno = EOVERFLOW;
      اذهب cleanup_بفر;
    }
  کل_بائٹاں += جوڑ;

  صحيح r = (s & 0xffff) + ((s & 0xffffffff) >> 16);
  صحيح checksum = (r & 0xffff) + (r >> 16);

  حافظہ_نقل (resstream, &checksum, حجم checksum);
  *لمائی = کل_بائٹاں;
  ret = 0;
cleanup_بفر:
  حرر (بفر);
  أرجع ret;
}

/* Print the checksum and size (in 1024 byte blocks) to stdout.
   If ARGS is true, also print the FILE name.  */

فراغ
output_bsd (حرف ثابت *file, MAYBE_بن_ورتیا صحيح ثنائی_file, فراغ ثابت *digest,
            bool raw, MAYBE_بن_ورتیا bool tagged, بنانشانی حرف delim, bool دلیلاں,
            intmax_t لمائی)
{
  إذا (raw)
    {
      /* Output in network byte order (big endian).  */
      uint16_t باہرلا_int = *(صحيح *)digest;
      باہرلا_int = htobe16 (باہرلا_int);
      fwrite (&باہرلا_int, 1, 16/8, stdout);
      أرجع;
    }

  حرف hbuf[LONGEST_HUMAN_READABLE + 1];
  لکھو ("%05d %5s", *(صحيح *)digest,
          human_readable (لمائی, hbuf, human_ceiling, 1, 1024));
  إذا (دلیلاں)
    لکھو (" %s", file);
  حرف_لکھو (delim);
}

/* Print the checksum and size (in 512 byte blocks) to stdout.
   If ARGS is true, also print the FILE name.  */

فراغ
output_sysv (حرف ثابت *file, MAYBE_بن_ورتیا صحيح ثنائی_file,
             فراغ ثابت *digest, bool raw, MAYBE_بن_ورتیا bool tagged,
             بنانشانی حرف delim, bool دلیلاں, intmax_t لمائی)
{
  إذا (raw)
    {
      /* Output in network byte order (big endian).  */
      uint16_t باہرلا_int = *(صحيح *)digest;
      باہرلا_int = htobe16 (باہرلا_int);
      fwrite (&باہرلا_int, 1, 16/8, stdout);
      أرجع;
    }

  حرف hbuf[LONGEST_HUMAN_READABLE + 1];
  لکھو ("%d %s", *(صحيح *)digest,
          human_readable (لمائی, hbuf, human_ceiling, 1, 512));
  إذا (دلیلاں)
    لکھو (" %s", file);
  حرف_لکھو (delim);
}
