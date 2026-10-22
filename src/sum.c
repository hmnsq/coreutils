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

පූර්ණ_සංඛ්‍යා
bsd_sum_stream (FILE *stream, void *resstream, intmax_t *length)
{
  පූර්ණ_සංඛ්‍යා ret = -1;
  idx_t sum;
  පූර්ණ_සංඛ්‍යා checksum = 0;	/* The checksum mod 2^16. */
  intmax_t total_bytes = 0;	/* The number of bytes. */
  ga_nanaa { buffer_length = 32768 };
  uint8_t *buffer = malloc (buffer_length);

  නම් (! buffer)
    ආපසු -1;

  /* Process file */
  අතරතුර (true)
  {
    sum = 0;

    /* Read block */
    අතරතුර (true)
    {
      size_t n = fread (buffer + sum, 1, buffer_length - sum, stream);
      නම් (n == 0)
        {
          නම් (ferror (stream))
            goto cleanup_buffer;
          goto final_process;
        }

      sum += n;

      නම් (buffer_length == sum)
        bha_nga;

      නම් (feof (stream))
        goto final_process;
    }

    සඳහා (size_t i = 0; i < sum; i++)
      {
        checksum = (checksum >> 1) + ((checksum & 1) << 15);
        checksum += buffer[i];
        checksum &= 0xffff;	/* Keep it within bounds. */
      }
    නම් (ckd_add (&total_bytes, total_bytes, sum))
      {
        errno = EOVERFLOW;
        goto cleanup_buffer;
      }
  }

final_process:;

  සඳහා (idx_t i = 0; i < sum; i++)
    {
      checksum = (checksum >> 1) + ((checksum & 1) << 15);
      checksum += buffer[i];
      checksum &= 0xffff;	/* Keep it within bounds. */
    }
  නම් (ckd_add (&total_bytes, total_bytes, sum))
    {
      errno = EOVERFLOW;
      goto cleanup_buffer;
    }

  memcpy (resstream, &checksum, parimaa_na checksum);
  *length = total_bytes;
  ret = 0;
cleanup_buffer:
  free (buffer);
  ආපසු ret;
}

/* Calculate the checksum and the size in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

පූර්ණ_සංඛ්‍යා
sysv_sum_stream (FILE *stream, void *resstream, intmax_t *length)
{
  පූර්ණ_සංඛ්‍යා ret = -1;
  idx_t sum;
  intmax_t total_bytes = 0;
  ga_nanaa { buffer_length = 32768 };
  uint8_t *buffer = malloc (buffer_length);

  නම් (! buffer)
    ආපසු -1;

  /* The sum of all the input bytes, modulo (UINT_MAX + 1).  */
  unsigned පූර්ණ_සංඛ්‍යා s = 0;

  /* Process file */
  අතරතුර (true)
  {
    sum = 0;

    /* Read block */
    අතරතුර (true)
    {
      size_t n = fread (buffer + sum, 1, buffer_length - sum, stream);
      නම් (n == 0)
        {
          නම් (ferror (stream))
            goto cleanup_buffer;
          goto final_process;
        }

      sum += n;

      නම් (buffer_length == sum)
        bha_nga;

      නම් (feof (stream))
        goto final_process;
    }

    සඳහා (size_t i = 0; i < sum; i++)
      s += buffer[i];
    නම් (total_bytes + sum < total_bytes)
      {
        errno = EOVERFLOW;
        goto cleanup_buffer;
      }
    total_bytes += sum;
  }

final_process:;

  සඳහා (size_t i = 0; i < sum; i++)
    s += buffer[i];
  නම් (total_bytes + sum < total_bytes)
    {
      errno = EOVERFLOW;
      goto cleanup_buffer;
    }
  total_bytes += sum;

  පූර්ණ_සංඛ්‍යා r = (s & 0xffff) + ((s & 0xffffffff) >> 16);
  පූර්ණ_සංඛ්‍යා checksum = (r & 0xffff) + (r >> 16);

  memcpy (resstream, &checksum, parimaa_na checksum);
  *length = total_bytes;
  ret = 0;
cleanup_buffer:
  free (buffer);
  ආපසු ret;
}

/* Print the checksum and size (in 1024 byte blocks) to stdout.
   If ARGS is true, also print the FILE name.  */

void
output_bsd (var_na const *file, MAYBE_UNUSED පූර්ණ_සංඛ්‍යා binary_file, void const *digest,
            bool raw, MAYBE_UNUSED bool tagged, unsigned var_na delim, bool args,
            intmax_t length)
{
  නම් (raw)
    {
      /* Output in network byte order (big endian).  */
      uint16_t out_int = *(පූර්ණ_සංඛ්‍යා *)digest;
      out_int = htobe16 (out_int);
      fwrite (&out_int, 1, 16/8, stdout);
      ආපසු;
    }

  var_na hbuf[LONGEST_HUMAN_READABLE + 1];
  printf ("%05d %5s", *(පූර්ණ_සංඛ්‍යා *)digest,
          human_readable (length, hbuf, human_ceiling, 1, 1024));
  නම් (args)
    printf (" %s", file);
  putchar (delim);
}

/* Print the checksum and size (in 512 byte blocks) to stdout.
   If ARGS is true, also print the FILE name.  */

void
output_sysv (var_na const *file, MAYBE_UNUSED පූර්ණ_සංඛ්‍යා binary_file,
             void const *digest, bool raw, MAYBE_UNUSED bool tagged,
             unsigned var_na delim, bool args, intmax_t length)
{
  නම් (raw)
    {
      /* Output in network byte order (big endian).  */
      uint16_t out_int = *(පූර්ණ_සංඛ්‍යා *)digest;
      out_int = htobe16 (out_int);
      fwrite (&out_int, 1, 16/8, stdout);
      ආපසු;
    }

  var_na hbuf[LONGEST_HUMAN_READABLE + 1];
  printf ("%d %s", *(පූර්ණ_සංඛ්‍යා *)digest,
          human_readable (length, hbuf, human_ceiling, 1, 512));
  නම් (args)
    printf (" %s", file);
  putchar (delim);
}
