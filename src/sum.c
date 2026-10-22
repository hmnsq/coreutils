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
bsd_sum_stream (FILE *stream, تهی *resstream, intmax_t *length)
{
  صحیح ret = -1;
  idx_t sum;
  صحیح checksum = 0;	/* The checksum mod 2^16. */
  intmax_t total_bytes = 0;	/* The number of bytes. */
  شمارش { buffer_length = 32768 };
  uint8_t *buffer = malloc (buffer_length);

  اگر (! buffer)
    بازگشت -1;

  /* Process file */
  وقتی (true)
  {
    sum = 0;

    /* Read block */
    وقتی (true)
    {
      size_t n = fread (buffer + sum, 1, buffer_length - sum, stream);
      اگر (n == 0)
        {
          اگر (ferror (stream))
            برو cleanup_buffer;
          برو final_process;
        }

      sum += n;

      اگر (buffer_length == sum)
        بشکن;

      اگر (feof (stream))
        برو final_process;
    }

    برای (size_t i = 0; i < sum; i++)
      {
        checksum = (checksum >> 1) + ((checksum & 1) << 15);
        checksum += buffer[i];
        checksum &= 0xffff;	/* Keep it within bounds. */
      }
    اگر (ckd_add (&total_bytes, total_bytes, sum))
      {
        errno = EOVERFLOW;
        برو cleanup_buffer;
      }
  }

final_process:;

  برای (idx_t i = 0; i < sum; i++)
    {
      checksum = (checksum >> 1) + ((checksum & 1) << 15);
      checksum += buffer[i];
      checksum &= 0xffff;	/* Keep it within bounds. */
    }
  اگر (ckd_add (&total_bytes, total_bytes, sum))
    {
      errno = EOVERFLOW;
      برو cleanup_buffer;
    }

  memcpy (resstream, &checksum, اندازه checksum);
  *length = total_bytes;
  ret = 0;
cleanup_buffer:
  آزاد (buffer);
  بازگشت ret;
}

/* Calculate the checksum and the size in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

صحیح
sysv_sum_stream (FILE *stream, تهی *resstream, intmax_t *length)
{
  صحیح ret = -1;
  idx_t sum;
  intmax_t total_bytes = 0;
  شمارش { buffer_length = 32768 };
  uint8_t *buffer = malloc (buffer_length);

  اگر (! buffer)
    بازگشت -1;

  /* The sum of all the input bytes, modulo (UINT_MAX + 1).  */
  unsigned صحیح s = 0;

  /* Process file */
  وقتی (true)
  {
    sum = 0;

    /* Read block */
    وقتی (true)
    {
      size_t n = fread (buffer + sum, 1, buffer_length - sum, stream);
      اگر (n == 0)
        {
          اگر (ferror (stream))
            برو cleanup_buffer;
          برو final_process;
        }

      sum += n;

      اگر (buffer_length == sum)
        بشکن;

      اگر (feof (stream))
        برو final_process;
    }

    برای (size_t i = 0; i < sum; i++)
      s += buffer[i];
    اگر (total_bytes + sum < total_bytes)
      {
        errno = EOVERFLOW;
        برو cleanup_buffer;
      }
    total_bytes += sum;
  }

final_process:;

  برای (size_t i = 0; i < sum; i++)
    s += buffer[i];
  اگر (total_bytes + sum < total_bytes)
    {
      errno = EOVERFLOW;
      برو cleanup_buffer;
    }
  total_bytes += sum;

  صحیح r = (s & 0xffff) + ((s & 0xffffffff) >> 16);
  صحیح checksum = (r & 0xffff) + (r >> 16);

  memcpy (resstream, &checksum, اندازه checksum);
  *length = total_bytes;
  ret = 0;
cleanup_buffer:
  آزاد (buffer);
  بازگشت ret;
}

/* Print the checksum and size (in 1024 byte blocks) to stdout.
   If ARGS is true, also print the FILE name.  */

تهی
output_bsd (نویسه ثابت *file, MAYBE_UNUSED صحیح دودویی_file, تهی ثابت *digest,
            bool raw, MAYBE_UNUSED bool tagged, unsigned نویسه delim, bool args,
            intmax_t length)
{
  اگر (raw)
    {
      /* Output in network byte order (big endian).  */
      uint16_t out_int = *(صحیح *)digest;
      out_int = htobe16 (out_int);
      fwrite (&out_int, 1, 16/8, stdout);
      بازگشت;
    }

  نویسه hbuf[LONGEST_HUMAN_READABLE + 1];
  printf ("%05d %5s", *(صحیح *)digest,
          human_readable (length, hbuf, human_ceiling, 1, 1024));
  اگر (args)
    printf (" %s", file);
  putchar (delim);
}

/* Print the checksum and size (in 512 byte blocks) to stdout.
   If ARGS is true, also print the FILE name.  */

تهی
output_sysv (نویسه ثابت *file, MAYBE_UNUSED صحیح دودویی_file,
             تهی ثابت *digest, bool raw, MAYBE_UNUSED bool tagged,
             unsigned نویسه delim, bool args, intmax_t length)
{
  اگر (raw)
    {
      /* Output in network byte order (big endian).  */
      uint16_t out_int = *(صحیح *)digest;
      out_int = htobe16 (out_int);
      fwrite (&out_int, 1, 16/8, stdout);
      بازگشت;
    }

  نویسه hbuf[LONGEST_HUMAN_READABLE + 1];
  printf ("%d %s", *(صحیح *)digest,
          human_readable (length, hbuf, human_ceiling, 1, 512));
  اگر (args)
    printf (" %s", file);
  putchar (delim);
}
