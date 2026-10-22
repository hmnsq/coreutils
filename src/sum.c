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

#شامل <config.h>

#شامل <stdio.h>
#شامل <sys/types.h>
#شامل <endian.h>
#شامل "system.h"
#شامل "human.h"
#شامل "sum.h"

/* Calculate the checksum and the size in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

صحیح
bsd_مجموع_دھارا (FILE *دھارا, خالی *resstream, intmax_t *لمبائی)
{
  صحیح ret = -1;
  idx_t مجموع;
  صحیح checksum = 0;	/* The checksum mod 2^16. */
  intmax_t کل_بائٹیں = 0;	/* The number of bytes. */
  شمار { بفر_لمبائی = 32768 };
  uint8_t *بفر = حافظہ_دو (بفر_لمبائی);

  اگر (! بفر)
    واپس -1;

  /* Process file */
  جب (true)
  {
    مجموع = 0;

    /* Read block */
    جب (true)
    {
      size_t n = fread (بفر + مجموع, 1, بفر_لمبائی - مجموع, دھارا);
      اگر (n == 0)
        {
          اگر (ferror (دھارا))
            جاؤ cleanup_بفر;
          جاؤ final_process;
        }

      مجموع += n;

      اگر (بفر_لمبائی == مجموع)
        توڑو;

      اگر (feof (دھارا))
        جاؤ final_process;
    }

    برائے (size_t i = 0; i < مجموع; i++)
      {
        checksum = (checksum >> 1) + ((checksum & 1) << 15);
        checksum += بفر[i];
        checksum &= 0xffff;	/* Keep it within bounds. */
      }
    اگر (ckd_جوڑو (&کل_بائٹیں, کل_بائٹیں, مجموع))
      {
        errno = EOVERFLOW;
        جاؤ cleanup_بفر;
      }
  }

final_process:;

  برائے (idx_t i = 0; i < مجموع; i++)
    {
      checksum = (checksum >> 1) + ((checksum & 1) << 15);
      checksum += بفر[i];
      checksum &= 0xffff;	/* Keep it within bounds. */
    }
  اگر (ckd_جوڑو (&کل_بائٹیں, کل_بائٹیں, مجموع))
    {
      errno = EOVERFLOW;
      جاؤ cleanup_بفر;
    }

  حافظہ_نقل (resstream, &checksum, حجم checksum);
  *لمبائی = کل_بائٹیں;
  ret = 0;
cleanup_بفر:
  آزاد (بفر);
  واپس ret;
}

/* Calculate the checksum and the size in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

صحیح
sysv_مجموع_دھارا (FILE *دھارا, خالی *resstream, intmax_t *لمبائی)
{
  صحیح ret = -1;
  idx_t مجموع;
  intmax_t کل_بائٹیں = 0;
  شمار { بفر_لمبائی = 32768 };
  uint8_t *بفر = حافظہ_دو (بفر_لمبائی);

  اگر (! بفر)
    واپس -1;

  /* The sum of all the input bytes, modulo (UINT_MAX + 1).  */
  غیرعلامتی صحیح s = 0;

  /* Process file */
  جب (true)
  {
    مجموع = 0;

    /* Read block */
    جب (true)
    {
      size_t n = fread (بفر + مجموع, 1, بفر_لمبائی - مجموع, دھارا);
      اگر (n == 0)
        {
          اگر (ferror (دھارا))
            جاؤ cleanup_بفر;
          جاؤ final_process;
        }

      مجموع += n;

      اگر (بفر_لمبائی == مجموع)
        توڑو;

      اگر (feof (دھارا))
        جاؤ final_process;
    }

    برائے (size_t i = 0; i < مجموع; i++)
      s += بفر[i];
    اگر (کل_بائٹیں + مجموع < کل_بائٹیں)
      {
        errno = EOVERFLOW;
        جاؤ cleanup_بفر;
      }
    کل_بائٹیں += مجموع;
  }

final_process:;

  برائے (size_t i = 0; i < مجموع; i++)
    s += بفر[i];
  اگر (کل_بائٹیں + مجموع < کل_بائٹیں)
    {
      errno = EOVERFLOW;
      جاؤ cleanup_بفر;
    }
  کل_بائٹیں += مجموع;

  صحیح r = (s & 0xffff) + ((s & 0xffffffff) >> 16);
  صحیح checksum = (r & 0xffff) + (r >> 16);

  حافظہ_نقل (resstream, &checksum, حجم checksum);
  *لمبائی = کل_بائٹیں;
  ret = 0;
cleanup_بفر:
  آزاد (بفر);
  واپس ret;
}

/* Print the checksum and size (in 1024 byte blocks) to stdout.
   If ARGS is true, also print the FILE name.  */

خالی
output_bsd (حرف ثابت *file, MAYBE_غیر_مستعمل صحیح ثنائی_file, خالی ثابت *digest,
            bool raw, MAYBE_غیر_مستعمل bool tagged, غیرعلامتی حرف delim, bool دلائل,
            intmax_t لمبائی)
{
  اگر (raw)
    {
      /* Output in network byte order (big endian).  */
      uint16_t برآمد_int = *(صحیح *)digest;
      برآمد_int = htobe16 (برآمد_int);
      fwrite (&برآمد_int, 1, 16/8, stdout);
      واپس;
    }

  حرف hbuf[LONGEST_HUMAN_READABLE + 1];
  لکھو ("%05d %5s", *(صحیح *)digest,
          human_readable (لمبائی, hbuf, human_ceiling, 1, 1024));
  اگر (دلائل)
    لکھو (" %s", file);
  حرف_لکھو (delim);
}

/* Print the checksum and size (in 512 byte blocks) to stdout.
   If ARGS is true, also print the FILE name.  */

خالی
output_sysv (حرف ثابت *file, MAYBE_غیر_مستعمل صحیح ثنائی_file,
             خالی ثابت *digest, bool raw, MAYBE_غیر_مستعمل bool tagged,
             غیرعلامتی حرف delim, bool دلائل, intmax_t لمبائی)
{
  اگر (raw)
    {
      /* Output in network byte order (big endian).  */
      uint16_t برآمد_int = *(صحیح *)digest;
      برآمد_int = htobe16 (برآمد_int);
      fwrite (&برآمد_int, 1, 16/8, stdout);
      واپس;
    }

  حرف hbuf[LONGEST_HUMAN_READABLE + 1];
  لکھو ("%d %s", *(صحیح *)digest,
          human_readable (لمبائی, hbuf, human_ceiling, 1, 512));
  اگر (دلائل)
    لکھو (" %s", file);
  حرف_لکھو (delim);
}
