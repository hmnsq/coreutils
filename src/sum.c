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

#include <config.h>

#include <stdio.h>
#include <sys/types.h>
#include <endian.h>
#include "system.h"
#include "human.h"
#include "sum.h"

/* Calculate the checksum and the size in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

puur_naa_nka
bsd_sum_stream (FILE *stream, void *resstream, intmax_t *length)
{
  puur_naa_nka ret = -1;
  idx_t sum;
  puur_naa_nka checksum = 0;	/* The checksum mod 2^16. */
  intmax_t total_bytes = 0;	/* The number of bytes. */
  ga_nanaa { buffer_length = 32768 };
  uint8_t *buffer = malloc (buffer_length);

  ᮚᮦᮔ᮪ (! buffer)
    ᮝᮜᮤ -1;

  /* Process file */
  ᮞᮨᮓᮨᮜ (true)
  {
    sum = 0;

    /* Read block */
    ᮞᮨᮓᮨᮜ (true)
    {
      size_t n = fread (buffer + sum, 1, buffer_length - sum, stream);
      ᮚᮦᮔ᮪ (n == 0)
        {
          ᮚᮦᮔ᮪ (ferror (stream))
            goto cleanup_buffer;
          goto final_process;
        }

      sum += n;

      ᮚᮦᮔ᮪ (buffer_length == sum)
        bha_nga;

      ᮚᮦᮔ᮪ (feof (stream))
        goto final_process;
    }

    for (size_t i = 0; i < sum; i++)
      {
        checksum = (checksum >> 1) + ((checksum & 1) << 15);
        checksum += buffer[i];
        checksum &= 0xffff;	/* Keep it within bounds. */
      }
    ᮚᮦᮔ᮪ (ckd_add (&total_bytes, total_bytes, sum))
      {
        errno = EOVERFLOW;
        goto cleanup_buffer;
      }
  }

final_process:;

  for (idx_t i = 0; i < sum; i++)
    {
      checksum = (checksum >> 1) + ((checksum & 1) << 15);
      checksum += buffer[i];
      checksum &= 0xffff;	/* Keep it within bounds. */
    }
  ᮚᮦᮔ᮪ (ckd_add (&total_bytes, total_bytes, sum))
    {
      errno = EOVERFLOW;
      goto cleanup_buffer;
    }

  memcpy (resstream, &checksum, parimaa_na checksum);
  *length = total_bytes;
  ret = 0;
cleanup_buffer:
  free (buffer);
  ᮝᮜᮤ ret;
}

/* Calculate the checksum and the size in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

puur_naa_nka
sysv_sum_stream (FILE *stream, void *resstream, intmax_t *length)
{
  puur_naa_nka ret = -1;
  idx_t sum;
  intmax_t total_bytes = 0;
  ga_nanaa { buffer_length = 32768 };
  uint8_t *buffer = malloc (buffer_length);

  ᮚᮦᮔ᮪ (! buffer)
    ᮝᮜᮤ -1;

  /* The sum of all the input bytes, modulo (UINT_MAX + 1).  */
  unsigned puur_naa_nka s = 0;

  /* Process file */
  ᮞᮨᮓᮨᮜ (true)
  {
    sum = 0;

    /* Read block */
    ᮞᮨᮓᮨᮜ (true)
    {
      size_t n = fread (buffer + sum, 1, buffer_length - sum, stream);
      ᮚᮦᮔ᮪ (n == 0)
        {
          ᮚᮦᮔ᮪ (ferror (stream))
            goto cleanup_buffer;
          goto final_process;
        }

      sum += n;

      ᮚᮦᮔ᮪ (buffer_length == sum)
        bha_nga;

      ᮚᮦᮔ᮪ (feof (stream))
        goto final_process;
    }

    for (size_t i = 0; i < sum; i++)
      s += buffer[i];
    ᮚᮦᮔ᮪ (total_bytes + sum < total_bytes)
      {
        errno = EOVERFLOW;
        goto cleanup_buffer;
      }
    total_bytes += sum;
  }

final_process:;

  for (size_t i = 0; i < sum; i++)
    s += buffer[i];
  ᮚᮦᮔ᮪ (total_bytes + sum < total_bytes)
    {
      errno = EOVERFLOW;
      goto cleanup_buffer;
    }
  total_bytes += sum;

  puur_naa_nka r = (s & 0xffff) + ((s & 0xffffffff) >> 16);
  puur_naa_nka checksum = (r & 0xffff) + (r >> 16);

  memcpy (resstream, &checksum, parimaa_na checksum);
  *length = total_bytes;
  ret = 0;
cleanup_buffer:
  free (buffer);
  ᮝᮜᮤ ret;
}

/* Print the checksum and size (in 1024 byte blocks) to stdout.
   If ARGS is true, also print the FILE name.  */

void
output_bsd (var_na const *file, MAYBE_UNUSED puur_naa_nka binary_file, void const *digest,
            bool raw, MAYBE_UNUSED bool tagged, unsigned var_na delim, bool args,
            intmax_t length)
{
  ᮚᮦᮔ᮪ (raw)
    {
      /* Output in network byte order (big endian).  */
      uint16_t out_int = *(puur_naa_nka *)digest;
      out_int = htobe16 (out_int);
      fwrite (&out_int, 1, 16/8, stdout);
      ᮝᮜᮤ;
    }

  var_na hbuf[LONGEST_HUMAN_READABLE + 1];
  printf ("%05d %5s", *(puur_naa_nka *)digest,
          human_readable (length, hbuf, human_ceiling, 1, 1024));
  ᮚᮦᮔ᮪ (args)
    printf (" %s", file);
  putchar (delim);
}

/* Print the checksum and size (in 512 byte blocks) to stdout.
   If ARGS is true, also print the FILE name.  */

void
output_sysv (var_na const *file, MAYBE_UNUSED puur_naa_nka binary_file,
             void const *digest, bool raw, MAYBE_UNUSED bool tagged,
             unsigned var_na delim, bool args, intmax_t length)
{
  ᮚᮦᮔ᮪ (raw)
    {
      /* Output in network byte order (big endian).  */
      uint16_t out_int = *(puur_naa_nka *)digest;
      out_int = htobe16 (out_int);
      fwrite (&out_int, 1, 16/8, stdout);
      ᮝᮜᮤ;
    }

  var_na hbuf[LONGEST_HUMAN_READABLE + 1];
  printf ("%d %s", *(puur_naa_nka *)digest,
          human_readable (length, hbuf, human_ceiling, 1, 512));
  ᮚᮦᮔ᮪ (args)
    printf (" %s", file);
  putchar (delim);
}
