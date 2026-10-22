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

số_nguyên
bsd_sum_stream (FILE *stream, void *resstream, intmax_t *length)
{
  số_nguyên ret = -1;
  idx_t sum;
  số_nguyên checksum = 0;	/* The checksum mod 2^16. */
  intmax_t total_bytes = 0;	/* The number of bytes. */
  enum { buffer_length = 32768 };
  uint8_t *buffer = malloc (buffer_length);

  nếu (! buffer)
    trả_về -1;

  /* Process file */
  trong_khi (true)
  {
    sum = 0;

    /* Read block */
    trong_khi (true)
    {
      size_t n = fread (buffer + sum, 1, buffer_length - sum, stream);
      nếu (n == 0)
        {
          nếu (ferror (stream))
            goto cleanup_buffer;
          goto final_process;
        }

      sum += n;

      nếu (buffer_length == sum)
        break;

      nếu (feof (stream))
        goto final_process;
    }

    cho (size_t i = 0; i < sum; i++)
      {
        checksum = (checksum >> 1) + ((checksum & 1) << 15);
        checksum += buffer[i];
        checksum &= 0xffff;	/* Keep it within bounds. */
      }
    nếu (ckd_add (&total_bytes, total_bytes, sum))
      {
        errno = EOVERFLOW;
        goto cleanup_buffer;
      }
  }

final_process:;

  cho (idx_t i = 0; i < sum; i++)
    {
      checksum = (checksum >> 1) + ((checksum & 1) << 15);
      checksum += buffer[i];
      checksum &= 0xffff;	/* Keep it within bounds. */
    }
  nếu (ckd_add (&total_bytes, total_bytes, sum))
    {
      errno = EOVERFLOW;
      goto cleanup_buffer;
    }

  memcpy (resstream, &checksum, sizeof checksum);
  *length = total_bytes;
  ret = 0;
cleanup_buffer:
  free (buffer);
  trả_về ret;
}

/* Calculate the checksum and the size in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

số_nguyên
sysv_sum_stream (FILE *stream, void *resstream, intmax_t *length)
{
  số_nguyên ret = -1;
  idx_t sum;
  intmax_t total_bytes = 0;
  enum { buffer_length = 32768 };
  uint8_t *buffer = malloc (buffer_length);

  nếu (! buffer)
    trả_về -1;

  /* The sum of all the input bytes, modulo (UINT_MAX + 1).  */
  unsigned số_nguyên s = 0;

  /* Process file */
  trong_khi (true)
  {
    sum = 0;

    /* Read block */
    trong_khi (true)
    {
      size_t n = fread (buffer + sum, 1, buffer_length - sum, stream);
      nếu (n == 0)
        {
          nếu (ferror (stream))
            goto cleanup_buffer;
          goto final_process;
        }

      sum += n;

      nếu (buffer_length == sum)
        break;

      nếu (feof (stream))
        goto final_process;
    }

    cho (size_t i = 0; i < sum; i++)
      s += buffer[i];
    nếu (total_bytes + sum < total_bytes)
      {
        errno = EOVERFLOW;
        goto cleanup_buffer;
      }
    total_bytes += sum;
  }

final_process:;

  cho (size_t i = 0; i < sum; i++)
    s += buffer[i];
  nếu (total_bytes + sum < total_bytes)
    {
      errno = EOVERFLOW;
      goto cleanup_buffer;
    }
  total_bytes += sum;

  số_nguyên r = (s & 0xffff) + ((s & 0xffffffff) >> 16);
  số_nguyên checksum = (r & 0xffff) + (r >> 16);

  memcpy (resstream, &checksum, sizeof checksum);
  *length = total_bytes;
  ret = 0;
cleanup_buffer:
  free (buffer);
  trả_về ret;
}

/* Print the checksum and size (in 1024 byte blocks) to stdout.
   If ARGS is true, also print the FILE name.  */

void
output_bsd (char const *file, MAYBE_UNUSED số_nguyên binary_file, void const *digest,
            bool raw, MAYBE_UNUSED bool tagged, unsigned char delim, bool args,
            intmax_t length)
{
  nếu (raw)
    {
      /* Output in network byte order (big endian).  */
      uint16_t out_int = *(số_nguyên *)digest;
      out_int = htobe16 (out_int);
      fwrite (&out_int, 1, 16/8, stdout);
      trả_về;
    }

  char hbuf[LONGEST_HUMAN_READABLE + 1];
  printf ("%05d %5s", *(số_nguyên *)digest,
          human_readable (length, hbuf, human_ceiling, 1, 1024));
  nếu (args)
    printf (" %s", file);
  putchar (delim);
}

/* Print the checksum and size (in 512 byte blocks) to stdout.
   If ARGS is true, also print the FILE name.  */

void
output_sysv (char const *file, MAYBE_UNUSED số_nguyên binary_file,
             void const *digest, bool raw, MAYBE_UNUSED bool tagged,
             unsigned char delim, bool args, intmax_t length)
{
  nếu (raw)
    {
      /* Output in network byte order (big endian).  */
      uint16_t out_int = *(số_nguyên *)digest;
      out_int = htobe16 (out_int);
      fwrite (&out_int, 1, 16/8, stdout);
      trả_về;
    }

  char hbuf[LONGEST_HUMAN_READABLE + 1];
  printf ("%d %s", *(số_nguyên *)digest,
          human_readable (length, hbuf, human_ceiling, 1, 512));
  nếu (args)
    printf (" %s", file);
  putchar (delim);
}
