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

ຈຳນວນເຕັມ
bsd_sum_stream (FILE *stream, void *resstream, intmax_t *length)
{
  ຈຳນວນເຕັມ ret = -1;
  idx_t sum;
  ຈຳນວນເຕັມ checksum = 0;	/* The checksum mod 2^16. */
  intmax_t total_bytes = 0;	/* The number of bytes. */
  ga_nanaa { buffer_length = 32768 };
  uint8_t *buffer = malloc (buffer_length);

  ຖ້າ (! buffer)
    ສົ່ງຄືນ -1;

  /* Process file */
  ໃນຂະນະທີ່ (true)
  {
    sum = 0;

    /* Read block */
    ໃນຂະນະທີ່ (true)
    {
      size_t n = fread (buffer + sum, 1, buffer_length - sum, stream);
      ຖ້າ (n == 0)
        {
          ຖ້າ (ferror (stream))
            goto cleanup_buffer;
          goto final_process;
        }

      sum += n;

      ຖ້າ (buffer_length == sum)
        bha_nga;

      ຖ້າ (feof (stream))
        goto final_process;
    }

    ສຳລັບ (size_t i = 0; i < sum; i++)
      {
        checksum = (checksum >> 1) + ((checksum & 1) << 15);
        checksum += buffer[i];
        checksum &= 0xffff;	/* Keep it within bounds. */
      }
    ຖ້າ (ckd_add (&total_bytes, total_bytes, sum))
      {
        errno = EOVERFLOW;
        goto cleanup_buffer;
      }
  }

final_process:;

  ສຳລັບ (idx_t i = 0; i < sum; i++)
    {
      checksum = (checksum >> 1) + ((checksum & 1) << 15);
      checksum += buffer[i];
      checksum &= 0xffff;	/* Keep it within bounds. */
    }
  ຖ້າ (ckd_add (&total_bytes, total_bytes, sum))
    {
      errno = EOVERFLOW;
      goto cleanup_buffer;
    }

  memcpy (resstream, &checksum, parimaa_na checksum);
  *length = total_bytes;
  ret = 0;
cleanup_buffer:
  free (buffer);
  ສົ່ງຄືນ ret;
}

/* Calculate the checksum and the size in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

ຈຳນວນເຕັມ
sysv_sum_stream (FILE *stream, void *resstream, intmax_t *length)
{
  ຈຳນວນເຕັມ ret = -1;
  idx_t sum;
  intmax_t total_bytes = 0;
  ga_nanaa { buffer_length = 32768 };
  uint8_t *buffer = malloc (buffer_length);

  ຖ້າ (! buffer)
    ສົ່ງຄືນ -1;

  /* The sum of all the input bytes, modulo (UINT_MAX + 1).  */
  unsigned ຈຳນວນເຕັມ s = 0;

  /* Process file */
  ໃນຂະນະທີ່ (true)
  {
    sum = 0;

    /* Read block */
    ໃນຂະນະທີ່ (true)
    {
      size_t n = fread (buffer + sum, 1, buffer_length - sum, stream);
      ຖ້າ (n == 0)
        {
          ຖ້າ (ferror (stream))
            goto cleanup_buffer;
          goto final_process;
        }

      sum += n;

      ຖ້າ (buffer_length == sum)
        bha_nga;

      ຖ້າ (feof (stream))
        goto final_process;
    }

    ສຳລັບ (size_t i = 0; i < sum; i++)
      s += buffer[i];
    ຖ້າ (total_bytes + sum < total_bytes)
      {
        errno = EOVERFLOW;
        goto cleanup_buffer;
      }
    total_bytes += sum;
  }

final_process:;

  ສຳລັບ (size_t i = 0; i < sum; i++)
    s += buffer[i];
  ຖ້າ (total_bytes + sum < total_bytes)
    {
      errno = EOVERFLOW;
      goto cleanup_buffer;
    }
  total_bytes += sum;

  ຈຳນວນເຕັມ r = (s & 0xffff) + ((s & 0xffffffff) >> 16);
  ຈຳນວນເຕັມ checksum = (r & 0xffff) + (r >> 16);

  memcpy (resstream, &checksum, parimaa_na checksum);
  *length = total_bytes;
  ret = 0;
cleanup_buffer:
  free (buffer);
  ສົ່ງຄືນ ret;
}

/* Print the checksum and size (in 1024 byte blocks) to stdout.
   If ARGS is true, also print the FILE name.  */

void
output_bsd (var_na const *file, MAYBE_UNUSED ຈຳນວນເຕັມ binary_file, void const *digest,
            bool raw, MAYBE_UNUSED bool tagged, unsigned var_na delim, bool args,
            intmax_t length)
{
  ຖ້າ (raw)
    {
      /* Output in network byte order (big endian).  */
      uint16_t out_int = *(ຈຳນວນເຕັມ *)digest;
      out_int = htobe16 (out_int);
      fwrite (&out_int, 1, 16/8, stdout);
      ສົ່ງຄືນ;
    }

  var_na hbuf[LONGEST_HUMAN_READABLE + 1];
  printf ("%05d %5s", *(ຈຳນວນເຕັມ *)digest,
          human_readable (length, hbuf, human_ceiling, 1, 1024));
  ຖ້າ (args)
    printf (" %s", file);
  putchar (delim);
}

/* Print the checksum and size (in 512 byte blocks) to stdout.
   If ARGS is true, also print the FILE name.  */

void
output_sysv (var_na const *file, MAYBE_UNUSED ຈຳນວນເຕັມ binary_file,
             void const *digest, bool raw, MAYBE_UNUSED bool tagged,
             unsigned var_na delim, bool args, intmax_t length)
{
  ຖ້າ (raw)
    {
      /* Output in network byte order (big endian).  */
      uint16_t out_int = *(ຈຳນວນເຕັມ *)digest;
      out_int = htobe16 (out_int);
      fwrite (&out_int, 1, 16/8, stdout);
      ສົ່ງຄືນ;
    }

  var_na hbuf[LONGEST_HUMAN_READABLE + 1];
  printf ("%d %s", *(ຈຳນວນເຕັມ *)digest,
          human_readable (length, hbuf, human_ceiling, 1, 512));
  ຖ້າ (args)
    printf (" %s", file);
  putchar (delim);
}
