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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <endian.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "human.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "sum.h"

/* Calculate the checksum and the size in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

ਪੂਰਨ
bsd_ਜੋੜ_ਧਾਰਾ (FILE *ਧਾਰਾ, ਖਾਲੀ *resstream, intmax_t *ਲੰਮਾਈ)
{
  ਪੂਰਨ ret = -1;
  idx_t ਜੋੜ;
  ਪੂਰਨ checksum = 0;	/* The checksum mod 2^16. */
  intmax_t ਕੁੱਲ_ਬਾਈਟਾਂ = 0;	/* The number of bytes. */
  ਗਿਣਤੀ { ਬਫਰ_ਲੰਮਾਈ = 32768 };
  uint8_t *ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਲੰਮਾਈ);

  ਜੇ (! ਬਫਰ)
    ਵਾਪਸ -1;

  /* Process file */
  ਜਦੋਂ (true)
  {
    ਜੋੜ = 0;

    /* Read block */
    ਜਦੋਂ (true)
    {
      size_t n = fread (ਬਫਰ + ਜੋੜ, 1, ਬਫਰ_ਲੰਮਾਈ - ਜੋੜ, ਧਾਰਾ);
      ਜੇ (n == 0)
        {
          ਜੇ (ferror (ਧਾਰਾ))
            ਜਾਓ cleanup_ਬਫਰ;
          ਜਾਓ final_process;
        }

      ਜੋੜ += n;

      ਜੇ (ਬਫਰ_ਲੰਮਾਈ == ਜੋੜ)
        ਰੋਕੋ;

      ਜੇ (feof (ਧਾਰਾ))
        ਜਾਓ final_process;
    }

    ਲਈ (size_t i = 0; i < ਜੋੜ; i++)
      {
        checksum = (checksum >> 1) + ((checksum & 1) << 15);
        checksum += ਬਫਰ[i];
        checksum &= 0xffff;	/* Keep it within bounds. */
      }
    ਜੇ (ckd_ਜੋੜੋ (&ਕੁੱਲ_ਬਾਈਟਾਂ, ਕੁੱਲ_ਬਾਈਟਾਂ, ਜੋੜ))
      {
        errno = EOVERFLOW;
        ਜਾਓ cleanup_ਬਫਰ;
      }
  }

final_process:;

  ਲਈ (idx_t i = 0; i < ਜੋੜ; i++)
    {
      checksum = (checksum >> 1) + ((checksum & 1) << 15);
      checksum += ਬਫਰ[i];
      checksum &= 0xffff;	/* Keep it within bounds. */
    }
  ਜੇ (ckd_ਜੋੜੋ (&ਕੁੱਲ_ਬਾਈਟਾਂ, ਕੁੱਲ_ਬਾਈਟਾਂ, ਜੋੜ))
    {
      errno = EOVERFLOW;
      ਜਾਓ cleanup_ਬਫਰ;
    }

  ਸਮ੍ਰਿਤੀ_ਨਕਲ (resstream, &checksum, ਆਕਾਰ checksum);
  *ਲੰਮਾਈ = ਕੁੱਲ_ਬਾਈਟਾਂ;
  ret = 0;
cleanup_ਬਫਰ:
  ਮੁਕਤ (ਬਫਰ);
  ਵਾਪਸ ret;
}

/* Calculate the checksum and the size in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

ਪੂਰਨ
sysv_ਜੋੜ_ਧਾਰਾ (FILE *ਧਾਰਾ, ਖਾਲੀ *resstream, intmax_t *ਲੰਮਾਈ)
{
  ਪੂਰਨ ret = -1;
  idx_t ਜੋੜ;
  intmax_t ਕੁੱਲ_ਬਾਈਟਾਂ = 0;
  ਗਿਣਤੀ { ਬਫਰ_ਲੰਮਾਈ = 32768 };
  uint8_t *ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਲੰਮਾਈ);

  ਜੇ (! ਬਫਰ)
    ਵਾਪਸ -1;

  /* The sum of all the input bytes, modulo (UINT_MAX + 1).  */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ s = 0;

  /* Process file */
  ਜਦੋਂ (true)
  {
    ਜੋੜ = 0;

    /* Read block */
    ਜਦੋਂ (true)
    {
      size_t n = fread (ਬਫਰ + ਜੋੜ, 1, ਬਫਰ_ਲੰਮਾਈ - ਜੋੜ, ਧਾਰਾ);
      ਜੇ (n == 0)
        {
          ਜੇ (ferror (ਧਾਰਾ))
            ਜਾਓ cleanup_ਬਫਰ;
          ਜਾਓ final_process;
        }

      ਜੋੜ += n;

      ਜੇ (ਬਫਰ_ਲੰਮਾਈ == ਜੋੜ)
        ਰੋਕੋ;

      ਜੇ (feof (ਧਾਰਾ))
        ਜਾਓ final_process;
    }

    ਲਈ (size_t i = 0; i < ਜੋੜ; i++)
      s += ਬਫਰ[i];
    ਜੇ (ਕੁੱਲ_ਬਾਈਟਾਂ + ਜੋੜ < ਕੁੱਲ_ਬਾਈਟਾਂ)
      {
        errno = EOVERFLOW;
        ਜਾਓ cleanup_ਬਫਰ;
      }
    ਕੁੱਲ_ਬਾਈਟਾਂ += ਜੋੜ;
  }

final_process:;

  ਲਈ (size_t i = 0; i < ਜੋੜ; i++)
    s += ਬਫਰ[i];
  ਜੇ (ਕੁੱਲ_ਬਾਈਟਾਂ + ਜੋੜ < ਕੁੱਲ_ਬਾਈਟਾਂ)
    {
      errno = EOVERFLOW;
      ਜਾਓ cleanup_ਬਫਰ;
    }
  ਕੁੱਲ_ਬਾਈਟਾਂ += ਜੋੜ;

  ਪੂਰਨ r = (s & 0xffff) + ((s & 0xffffffff) >> 16);
  ਪੂਰਨ checksum = (r & 0xffff) + (r >> 16);

  ਸਮ੍ਰਿਤੀ_ਨਕਲ (resstream, &checksum, ਆਕਾਰ checksum);
  *ਲੰਮਾਈ = ਕੁੱਲ_ਬਾਈਟਾਂ;
  ret = 0;
cleanup_ਬਫਰ:
  ਮੁਕਤ (ਬਫਰ);
  ਵਾਪਸ ret;
}

/* Print the checksum and size (in 1024 byte blocks) to stdout.
   If ARGS is true, also print the FILE name.  */

ਖਾਲੀ
output_bsd (ਅੱਖਰ ਅਚਲ *file, MAYBE_ਅਣਵਰਤਿਆ ਪੂਰਨ ਦੁਆਧਾਰੀ_file, ਖਾਲੀ ਅਚਲ *digest,
            bool raw, MAYBE_ਅਣਵਰਤਿਆ bool tagged, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ delim, bool ਦਲੀਲਾਂ,
            intmax_t ਲੰਮਾਈ)
{
  ਜੇ (raw)
    {
      /* Output in network byte order (big endian).  */
      uint16_t ਬਾਹਰਲਾ_int = *(ਪੂਰਨ *)digest;
      ਬਾਹਰਲਾ_int = htobe16 (ਬਾਹਰਲਾ_int);
      fwrite (&ਬਾਹਰਲਾ_int, 1, 16/8, stdout);
      ਵਾਪਸ;
    }

  ਅੱਖਰ hbuf[LONGEST_HUMAN_READABLE + 1];
  ਲਿਖੋ ("%05d %5s", *(ਪੂਰਨ *)digest,
          human_readable (ਲੰਮਾਈ, hbuf, human_ceiling, 1, 1024));
  ਜੇ (ਦਲੀਲਾਂ)
    ਲਿਖੋ (" %s", file);
  ਅੱਖਰ_ਲਿਖੋ (delim);
}

/* Print the checksum and size (in 512 byte blocks) to stdout.
   If ARGS is true, also print the FILE name.  */

ਖਾਲੀ
output_sysv (ਅੱਖਰ ਅਚਲ *file, MAYBE_ਅਣਵਰਤਿਆ ਪੂਰਨ ਦੁਆਧਾਰੀ_file,
             ਖਾਲੀ ਅਚਲ *digest, bool raw, MAYBE_ਅਣਵਰਤਿਆ bool tagged,
             ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ delim, bool ਦਲੀਲਾਂ, intmax_t ਲੰਮਾਈ)
{
  ਜੇ (raw)
    {
      /* Output in network byte order (big endian).  */
      uint16_t ਬਾਹਰਲਾ_int = *(ਪੂਰਨ *)digest;
      ਬਾਹਰਲਾ_int = htobe16 (ਬਾਹਰਲਾ_int);
      fwrite (&ਬਾਹਰਲਾ_int, 1, 16/8, stdout);
      ਵਾਪਸ;
    }

  ਅੱਖਰ hbuf[LONGEST_HUMAN_READABLE + 1];
  ਲਿਖੋ ("%d %s", *(ਪੂਰਨ *)digest,
          human_readable (ਲੰਮਾਈ, hbuf, human_ceiling, 1, 512));
  ਜੇ (ਦਲੀਲਾਂ)
    ਲਿਖੋ (" %s", file);
  ਅੱਖਰ_ਲਿਖੋ (delim);
}
