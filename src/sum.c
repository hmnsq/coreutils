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

#ଅନ୍ତର୍ଭାଵ <config.h>

#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <endian.h>
#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "human.h"
#ଅନ୍ତର୍ଭାଵ "sum.h"

/* Calculate the checksum and the size in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
bsd_sum_stream (FILE *stream, ଶୂନ୍ୟ *resstream, intmax_t *length)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = -1;
  idx_t sum;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା checksum = 0;	/* The checksum mod 2^16. */
  intmax_t total_bytes = 0;	/* The number of bytes. */
  ଗଣନା { buffer_length = 32768 };
  uint8_t *buffer = malloc (buffer_length);

  ଯଦି (! buffer)
    ଫେରନ୍ତୁ -1;

  /* Process file */
  ଯେତେବେଳେ (true)
  {
    sum = 0;

    /* Read block */
    ଯେତେବେଳେ (true)
    {
      size_t n = fread (buffer + sum, 1, buffer_length - sum, stream);
      ଯଦି (n == 0)
        {
          ଯଦି (ferror (stream))
            ଯାଅ cleanup_buffer;
          ଯାଅ final_process;
        }

      sum += n;

      ଯଦି (buffer_length == sum)
        ଭାଙ୍ଗ;

      ଯଦି (feof (stream))
        ଯାଅ final_process;
    }

    ପାଇଁ (size_t i = 0; i < sum; i++)
      {
        checksum = (checksum >> 1) + ((checksum & 1) << 15);
        checksum += buffer[i];
        checksum &= 0xffff;	/* Keep it within bounds. */
      }
    ଯଦି (ckd_add (&total_bytes, total_bytes, sum))
      {
        errno = EOVERFLOW;
        ଯାଅ cleanup_buffer;
      }
  }

final_process:;

  ପାଇଁ (idx_t i = 0; i < sum; i++)
    {
      checksum = (checksum >> 1) + ((checksum & 1) << 15);
      checksum += buffer[i];
      checksum &= 0xffff;	/* Keep it within bounds. */
    }
  ଯଦି (ckd_add (&total_bytes, total_bytes, sum))
    {
      errno = EOVERFLOW;
      ଯାଅ cleanup_buffer;
    }

  memcpy (resstream, &checksum, ଆକାର checksum);
  *length = total_bytes;
  ret = 0;
cleanup_buffer:
  ମୁକ୍ତ (buffer);
  ଫେରନ୍ତୁ ret;
}

/* Calculate the checksum and the size in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
sysv_sum_stream (FILE *stream, ଶୂନ୍ୟ *resstream, intmax_t *length)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = -1;
  idx_t sum;
  intmax_t total_bytes = 0;
  ଗଣନା { buffer_length = 32768 };
  uint8_t *buffer = malloc (buffer_length);

  ଯଦି (! buffer)
    ଫେରନ୍ତୁ -1;

  /* The sum of all the input bytes, modulo (UINT_MAX + 1).  */
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା s = 0;

  /* Process file */
  ଯେତେବେଳେ (true)
  {
    sum = 0;

    /* Read block */
    ଯେତେବେଳେ (true)
    {
      size_t n = fread (buffer + sum, 1, buffer_length - sum, stream);
      ଯଦି (n == 0)
        {
          ଯଦି (ferror (stream))
            ଯାଅ cleanup_buffer;
          ଯାଅ final_process;
        }

      sum += n;

      ଯଦି (buffer_length == sum)
        ଭାଙ୍ଗ;

      ଯଦି (feof (stream))
        ଯାଅ final_process;
    }

    ପାଇଁ (size_t i = 0; i < sum; i++)
      s += buffer[i];
    ଯଦି (total_bytes + sum < total_bytes)
      {
        errno = EOVERFLOW;
        ଯାଅ cleanup_buffer;
      }
    total_bytes += sum;
  }

final_process:;

  ପାଇଁ (size_t i = 0; i < sum; i++)
    s += buffer[i];
  ଯଦି (total_bytes + sum < total_bytes)
    {
      errno = EOVERFLOW;
      ଯାଅ cleanup_buffer;
    }
  total_bytes += sum;

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା r = (s & 0xffff) + ((s & 0xffffffff) >> 16);
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା checksum = (r & 0xffff) + (r >> 16);

  memcpy (resstream, &checksum, ଆକାର checksum);
  *length = total_bytes;
  ret = 0;
cleanup_buffer:
  ମୁକ୍ତ (buffer);
  ଫେରନ୍ତୁ ret;
}

/* Print the checksum and size (in 1024 byte blocks) to stdout.
   If ARGS is true, also print the FILE name.  */

ଶୂନ୍ୟ
output_bsd (ଅକ୍ଷର ଅଚଳ *file, MAYBE_UNUSED ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଦ୍ୱିମିକ_file, ଶୂନ୍ୟ ଅଚଳ *digest,
            bool raw, MAYBE_UNUSED bool tagged, unsigned ଅକ୍ଷର delim, bool args,
            intmax_t length)
{
  ଯଦି (raw)
    {
      /* Output in network byte order (big endian).  */
      uint16_t out_int = *(ପୂର୍ଣ୍ଣସଂଖ୍ୟା *)digest;
      out_int = htobe16 (out_int);
      fwrite (&out_int, 1, 16/8, stdout);
      ଫେରନ୍ତୁ;
    }

  ଅକ୍ଷର hbuf[LONGEST_HUMAN_READABLE + 1];
  printf ("%05d %5s", *(ପୂର୍ଣ୍ଣସଂଖ୍ୟା *)digest,
          human_readable (length, hbuf, human_ceiling, 1, 1024));
  ଯଦି (args)
    printf (" %s", file);
  putchar (delim);
}

/* Print the checksum and size (in 512 byte blocks) to stdout.
   If ARGS is true, also print the FILE name.  */

ଶୂନ୍ୟ
output_sysv (ଅକ୍ଷର ଅଚଳ *file, MAYBE_UNUSED ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଦ୍ୱିମିକ_file,
             ଶୂନ୍ୟ ଅଚଳ *digest, bool raw, MAYBE_UNUSED bool tagged,
             unsigned ଅକ୍ଷର delim, bool args, intmax_t length)
{
  ଯଦି (raw)
    {
      /* Output in network byte order (big endian).  */
      uint16_t out_int = *(ପୂର୍ଣ୍ଣସଂଖ୍ୟା *)digest;
      out_int = htobe16 (out_int);
      fwrite (&out_int, 1, 16/8, stdout);
      ଫେରନ୍ତୁ;
    }

  ଅକ୍ଷର hbuf[LONGEST_HUMAN_READABLE + 1];
  printf ("%d %s", *(ପୂର୍ଣ୍ଣସଂଖ୍ୟା *)digest,
          human_readable (length, hbuf, human_ceiling, 1, 512));
  ଯଦି (args)
    printf (" %s", file);
  putchar (delim);
}
