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
bsd_sum_stream (FILE *stream, فراغ *resstream, intmax_t *length)
{
  صحيح ret = -1;
  idx_t sum;
  صحيح checksum = 0;	/* The checksum mod 2^16. */
  intmax_t total_bytes = 0;	/* The number of bytes. */
  تعداد { buffer_length = 32768 };
  uint8_t *buffer = malloc (buffer_length);

  ܐܢ (! buffer)
    ܦܢܐ -1;

  /* Process file */
  ܟܡܐ (true)
  {
    sum = 0;

    /* Read block */
    ܟܡܐ (true)
    {
      size_t n = fread (buffer + sum, 1, buffer_length - sum, stream);
      ܐܢ (n == 0)
        {
          ܐܢ (ferror (stream))
            اذهب cleanup_buffer;
          اذهب final_process;
        }

      sum += n;

      ܐܢ (buffer_length == sum)
        اكسر;

      ܐܢ (feof (stream))
        اذهب final_process;
    }

    لكل (size_t i = 0; i < sum; i++)
      {
        checksum = (checksum >> 1) + ((checksum & 1) << 15);
        checksum += buffer[i];
        checksum &= 0xffff;	/* Keep it within bounds. */
      }
    ܐܢ (ckd_add (&total_bytes, total_bytes, sum))
      {
        errno = EOVERFLOW;
        اذهب cleanup_buffer;
      }
  }

final_process:;

  لكل (idx_t i = 0; i < sum; i++)
    {
      checksum = (checksum >> 1) + ((checksum & 1) << 15);
      checksum += buffer[i];
      checksum &= 0xffff;	/* Keep it within bounds. */
    }
  ܐܢ (ckd_add (&total_bytes, total_bytes, sum))
    {
      errno = EOVERFLOW;
      اذهب cleanup_buffer;
    }

  memcpy (resstream, &checksum, حجم checksum);
  *length = total_bytes;
  ret = 0;
cleanup_buffer:
  حرر (buffer);
  ܦܢܐ ret;
}

/* Calculate the checksum and the size in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

صحيح
sysv_sum_stream (FILE *stream, فراغ *resstream, intmax_t *length)
{
  صحيح ret = -1;
  idx_t sum;
  intmax_t total_bytes = 0;
  تعداد { buffer_length = 32768 };
  uint8_t *buffer = malloc (buffer_length);

  ܐܢ (! buffer)
    ܦܢܐ -1;

  /* The sum of all the input bytes, modulo (UINT_MAX + 1).  */
  unsigned صحيح s = 0;

  /* Process file */
  ܟܡܐ (true)
  {
    sum = 0;

    /* Read block */
    ܟܡܐ (true)
    {
      size_t n = fread (buffer + sum, 1, buffer_length - sum, stream);
      ܐܢ (n == 0)
        {
          ܐܢ (ferror (stream))
            اذهب cleanup_buffer;
          اذهب final_process;
        }

      sum += n;

      ܐܢ (buffer_length == sum)
        اكسر;

      ܐܢ (feof (stream))
        اذهب final_process;
    }

    لكل (size_t i = 0; i < sum; i++)
      s += buffer[i];
    ܐܢ (total_bytes + sum < total_bytes)
      {
        errno = EOVERFLOW;
        اذهب cleanup_buffer;
      }
    total_bytes += sum;
  }

final_process:;

  لكل (size_t i = 0; i < sum; i++)
    s += buffer[i];
  ܐܢ (total_bytes + sum < total_bytes)
    {
      errno = EOVERFLOW;
      اذهب cleanup_buffer;
    }
  total_bytes += sum;

  صحيح r = (s & 0xffff) + ((s & 0xffffffff) >> 16);
  صحيح checksum = (r & 0xffff) + (r >> 16);

  memcpy (resstream, &checksum, حجم checksum);
  *length = total_bytes;
  ret = 0;
cleanup_buffer:
  حرر (buffer);
  ܦܢܐ ret;
}

/* Print the checksum and size (in 1024 byte blocks) to stdout.
   If ARGS is true, also print the FILE name.  */

فراغ
output_bsd (حرف ثابت *file, MAYBE_UNUSED صحيح binary_file, فراغ ثابت *digest,
            bool raw, MAYBE_UNUSED bool tagged, unsigned حرف delim, bool args,
            intmax_t length)
{
  ܐܢ (raw)
    {
      /* Output in network byte order (big endian).  */
      uint16_t out_int = *(صحيح *)digest;
      out_int = htobe16 (out_int);
      fwrite (&out_int, 1, 16/8, stdout);
      ܦܢܐ;
    }

  حرف hbuf[LONGEST_HUMAN_READABLE + 1];
  printf ("%05d %5s", *(صحيح *)digest,
          human_readable (length, hbuf, human_ceiling, 1, 1024));
  ܐܢ (args)
    printf (" %s", file);
  putchar (delim);
}

/* Print the checksum and size (in 512 byte blocks) to stdout.
   If ARGS is true, also print the FILE name.  */

فراغ
output_sysv (حرف ثابت *file, MAYBE_UNUSED صحيح binary_file,
             فراغ ثابت *digest, bool raw, MAYBE_UNUSED bool tagged,
             unsigned حرف delim, bool args, intmax_t length)
{
  ܐܢ (raw)
    {
      /* Output in network byte order (big endian).  */
      uint16_t out_int = *(صحيح *)digest;
      out_int = htobe16 (out_int);
      fwrite (&out_int, 1, 16/8, stdout);
      ܦܢܐ;
    }

  حرف hbuf[LONGEST_HUMAN_READABLE + 1];
  printf ("%d %s", *(صحيح *)digest,
          human_readable (length, hbuf, human_ceiling, 1, 512));
  ܐܢ (args)
    printf (" %s", file);
  putchar (delim);
}
