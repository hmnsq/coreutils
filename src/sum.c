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

#অন্তর্ভাব <config.h>

#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <endian.h>
#অন্তর্ভাব "system.h"
#অন্তর্ভাব "human.h"
#অন্তর্ভাব "sum.h"

/* Calculate the checksum and the size in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

পূর্ণাঙ্ক
bsd_sum_stream (FILE *stream, শূন্য *resstream, intmax_t *length)
{
  পূর্ণাঙ্ক ret = -1;
  idx_t sum;
  পূর্ণাঙ্ক checksum = 0;	/* The checksum mod 2^16. */
  intmax_t total_bytes = 0;	/* The number of bytes. */
  গণনা { buffer_length = 32768 };
  uint8_t *buffer = malloc (buffer_length);

  যদি (! buffer)
    প্রত্যাগম -1;

  /* Process file */
  যাবত্ (true)
  {
    sum = 0;

    /* Read block */
    যাবত্ (true)
    {
      size_t n = fread (buffer + sum, 1, buffer_length - sum, stream);
      যদি (n == 0)
        {
          যদি (ferror (stream))
            গচ্ছ cleanup_buffer;
          গচ্ছ final_process;
        }

      sum += n;

      যদি (buffer_length == sum)
        ভঙ্গ;

      যদি (feof (stream))
        গচ্ছ final_process;
    }

    প্রতি (size_t i = 0; i < sum; i++)
      {
        checksum = (checksum >> 1) + ((checksum & 1) << 15);
        checksum += buffer[i];
        checksum &= 0xffff;	/* Keep it within bounds. */
      }
    যদি (ckd_add (&total_bytes, total_bytes, sum))
      {
        errno = EOVERFLOW;
        গচ্ছ cleanup_buffer;
      }
  }

final_process:;

  প্রতি (idx_t i = 0; i < sum; i++)
    {
      checksum = (checksum >> 1) + ((checksum & 1) << 15);
      checksum += buffer[i];
      checksum &= 0xffff;	/* Keep it within bounds. */
    }
  যদি (ckd_add (&total_bytes, total_bytes, sum))
    {
      errno = EOVERFLOW;
      গচ্ছ cleanup_buffer;
    }

  memcpy (resstream, &checksum, পরিমাণ checksum);
  *length = total_bytes;
  ret = 0;
cleanup_buffer:
  মুক্ত (buffer);
  প্রত্যাগম ret;
}

/* Calculate the checksum and the size in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

পূর্ণাঙ্ক
sysv_sum_stream (FILE *stream, শূন্য *resstream, intmax_t *length)
{
  পূর্ণাঙ্ক ret = -1;
  idx_t sum;
  intmax_t total_bytes = 0;
  গণনা { buffer_length = 32768 };
  uint8_t *buffer = malloc (buffer_length);

  যদি (! buffer)
    প্রত্যাগম -1;

  /* The sum of all the input bytes, modulo (UINT_MAX + 1).  */
  unsigned পূর্ণাঙ্ক s = 0;

  /* Process file */
  যাবত্ (true)
  {
    sum = 0;

    /* Read block */
    যাবত্ (true)
    {
      size_t n = fread (buffer + sum, 1, buffer_length - sum, stream);
      যদি (n == 0)
        {
          যদি (ferror (stream))
            গচ্ছ cleanup_buffer;
          গচ্ছ final_process;
        }

      sum += n;

      যদি (buffer_length == sum)
        ভঙ্গ;

      যদি (feof (stream))
        গচ্ছ final_process;
    }

    প্রতি (size_t i = 0; i < sum; i++)
      s += buffer[i];
    যদি (total_bytes + sum < total_bytes)
      {
        errno = EOVERFLOW;
        গচ্ছ cleanup_buffer;
      }
    total_bytes += sum;
  }

final_process:;

  প্রতি (size_t i = 0; i < sum; i++)
    s += buffer[i];
  যদি (total_bytes + sum < total_bytes)
    {
      errno = EOVERFLOW;
      গচ্ছ cleanup_buffer;
    }
  total_bytes += sum;

  পূর্ণাঙ্ক r = (s & 0xffff) + ((s & 0xffffffff) >> 16);
  পূর্ণাঙ্ক checksum = (r & 0xffff) + (r >> 16);

  memcpy (resstream, &checksum, পরিমাণ checksum);
  *length = total_bytes;
  ret = 0;
cleanup_buffer:
  মুক্ত (buffer);
  প্রত্যাগম ret;
}

/* Print the checksum and size (in 1024 byte blocks) to stdout.
   If ARGS is true, also print the FILE name.  */

শূন্য
output_bsd (বর্ণ ধ্রুব *file, MAYBE_UNUSED পূর্ণাঙ্ক দ্বিমিক_file, শূন্য ধ্রুব *digest,
            bool raw, MAYBE_UNUSED bool tagged, unsigned বর্ণ delim, bool args,
            intmax_t length)
{
  যদি (raw)
    {
      /* Output in network byte order (big endian).  */
      uint16_t out_int = *(পূর্ণাঙ্ক *)digest;
      out_int = htobe16 (out_int);
      fwrite (&out_int, 1, 16/8, stdout);
      প্রত্যাগম;
    }

  বর্ণ hbuf[LONGEST_HUMAN_READABLE + 1];
  printf ("%05d %5s", *(পূর্ণাঙ্ক *)digest,
          human_readable (length, hbuf, human_ceiling, 1, 1024));
  যদি (args)
    printf (" %s", file);
  putchar (delim);
}

/* Print the checksum and size (in 512 byte blocks) to stdout.
   If ARGS is true, also print the FILE name.  */

শূন্য
output_sysv (বর্ণ ধ্রুব *file, MAYBE_UNUSED পূর্ণাঙ্ক দ্বিমিক_file,
             শূন্য ধ্রুব *digest, bool raw, MAYBE_UNUSED bool tagged,
             unsigned বর্ণ delim, bool args, intmax_t length)
{
  যদি (raw)
    {
      /* Output in network byte order (big endian).  */
      uint16_t out_int = *(পূর্ণাঙ্ক *)digest;
      out_int = htobe16 (out_int);
      fwrite (&out_int, 1, 16/8, stdout);
      প্রত্যাগম;
    }

  বর্ণ hbuf[LONGEST_HUMAN_READABLE + 1];
  printf ("%d %s", *(পূর্ণাঙ্ক *)digest,
          human_readable (length, hbuf, human_ceiling, 1, 512));
  যদি (args)
    printf (" %s", file);
  putchar (delim);
}
