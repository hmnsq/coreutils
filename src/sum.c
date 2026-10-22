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

#அந்தர்பாவ <config.h>

#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <endian.h>
#அந்தர்பாவ "system.h"
#அந்தர்பாவ "human.h"
#அந்தர்பாவ "sum.h"

/* Calculate the checksum and the size in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

முழு
bsd_sum_stream (FILE *stream, வெறுமை *resstream, intmax_t *length)
{
  முழு ret = -1;
  idx_t sum;
  முழு checksum = 0;	/* The checksum mod 2^16. */
  intmax_t total_bytes = 0;	/* The number of bytes. */
  எண்ணிக்கை { buffer_length = 32768 };
  uint8_t *buffer = malloc (buffer_length);

  எனில் (! buffer)
    திரும்பு -1;

  /* Process file */
  போது (true)
  {
    sum = 0;

    /* Read block */
    போது (true)
    {
      size_t n = fread (buffer + sum, 1, buffer_length - sum, stream);
      எனில் (n == 0)
        {
          எனில் (ferror (stream))
            செல் cleanup_buffer;
          செல் final_process;
        }

      sum += n;

      எனில் (buffer_length == sum)
        நிறுத்து;

      எனில் (feof (stream))
        செல் final_process;
    }

    க்கு (size_t i = 0; i < sum; i++)
      {
        checksum = (checksum >> 1) + ((checksum & 1) << 15);
        checksum += buffer[i];
        checksum &= 0xffff;	/* Keep it within bounds. */
      }
    எனில் (ckd_add (&total_bytes, total_bytes, sum))
      {
        errno = EOVERFLOW;
        செல் cleanup_buffer;
      }
  }

final_process:;

  க்கு (idx_t i = 0; i < sum; i++)
    {
      checksum = (checksum >> 1) + ((checksum & 1) << 15);
      checksum += buffer[i];
      checksum &= 0xffff;	/* Keep it within bounds. */
    }
  எனில் (ckd_add (&total_bytes, total_bytes, sum))
    {
      errno = EOVERFLOW;
      செல் cleanup_buffer;
    }

  memcpy (resstream, &checksum, அளவு checksum);
  *length = total_bytes;
  ret = 0;
cleanup_buffer:
  விடுவி (buffer);
  திரும்பு ret;
}

/* Calculate the checksum and the size in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

முழு
sysv_sum_stream (FILE *stream, வெறுமை *resstream, intmax_t *length)
{
  முழு ret = -1;
  idx_t sum;
  intmax_t total_bytes = 0;
  எண்ணிக்கை { buffer_length = 32768 };
  uint8_t *buffer = malloc (buffer_length);

  எனில் (! buffer)
    திரும்பு -1;

  /* The sum of all the input bytes, modulo (UINT_MAX + 1).  */
  unsigned முழு s = 0;

  /* Process file */
  போது (true)
  {
    sum = 0;

    /* Read block */
    போது (true)
    {
      size_t n = fread (buffer + sum, 1, buffer_length - sum, stream);
      எனில் (n == 0)
        {
          எனில் (ferror (stream))
            செல் cleanup_buffer;
          செல் final_process;
        }

      sum += n;

      எனில் (buffer_length == sum)
        நிறுத்து;

      எனில் (feof (stream))
        செல் final_process;
    }

    க்கு (size_t i = 0; i < sum; i++)
      s += buffer[i];
    எனில் (total_bytes + sum < total_bytes)
      {
        errno = EOVERFLOW;
        செல் cleanup_buffer;
      }
    total_bytes += sum;
  }

final_process:;

  க்கு (size_t i = 0; i < sum; i++)
    s += buffer[i];
  எனில் (total_bytes + sum < total_bytes)
    {
      errno = EOVERFLOW;
      செல் cleanup_buffer;
    }
  total_bytes += sum;

  முழு r = (s & 0xffff) + ((s & 0xffffffff) >> 16);
  முழு checksum = (r & 0xffff) + (r >> 16);

  memcpy (resstream, &checksum, அளவு checksum);
  *length = total_bytes;
  ret = 0;
cleanup_buffer:
  விடுவி (buffer);
  திரும்பு ret;
}

/* Print the checksum and size (in 1024 byte blocks) to stdout.
   If ARGS is true, also print the FILE name.  */

வெறுமை
output_bsd (எழுத்து மாறிலி *file, MAYBE_UNUSED முழு இருமம்_file, வெறுமை மாறிலி *digest,
            bool raw, MAYBE_UNUSED bool tagged, unsigned எழுத்து delim, bool args,
            intmax_t length)
{
  எனில் (raw)
    {
      /* Output in network byte order (big endian).  */
      uint16_t out_int = *(முழு *)digest;
      out_int = htobe16 (out_int);
      fwrite (&out_int, 1, 16/8, stdout);
      திரும்பு;
    }

  எழுத்து hbuf[LONGEST_HUMAN_READABLE + 1];
  printf ("%05d %5s", *(முழு *)digest,
          human_readable (length, hbuf, human_ceiling, 1, 1024));
  எனில் (args)
    printf (" %s", file);
  putchar (delim);
}

/* Print the checksum and size (in 512 byte blocks) to stdout.
   If ARGS is true, also print the FILE name.  */

வெறுமை
output_sysv (எழுத்து மாறிலி *file, MAYBE_UNUSED முழு இருமம்_file,
             வெறுமை மாறிலி *digest, bool raw, MAYBE_UNUSED bool tagged,
             unsigned எழுத்து delim, bool args, intmax_t length)
{
  எனில் (raw)
    {
      /* Output in network byte order (big endian).  */
      uint16_t out_int = *(முழு *)digest;
      out_int = htobe16 (out_int);
      fwrite (&out_int, 1, 16/8, stdout);
      திரும்பு;
    }

  எழுத்து hbuf[LONGEST_HUMAN_READABLE + 1];
  printf ("%d %s", *(முழு *)digest,
          human_readable (length, hbuf, human_ceiling, 1, 512));
  எனில் (args)
    printf (" %s", file);
  putchar (delim);
}
