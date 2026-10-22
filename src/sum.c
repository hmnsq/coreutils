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

#અન્તર્ભાવ <config.h>

#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <endian.h>
#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "human.h"
#અન્તર્ભાવ "sum.h"

/* Calculate the checksum and the size in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

પૂર્ણાંક
bsd_sum_stream (FILE *stream, શૂન્ય *resstream, intmax_t *length)
{
  પૂર્ણાંક ret = -1;
  idx_t sum;
  પૂર્ણાંક checksum = 0;	/* The checksum mod 2^16. */
  intmax_t total_bytes = 0;	/* The number of bytes. */
  ગણના { buffer_length = 32768 };
  uint8_t *buffer = malloc (buffer_length);

  જો (! buffer)
    પાછા -1;

  /* Process file */
  જ્યારે (true)
  {
    sum = 0;

    /* Read block */
    જ્યારે (true)
    {
      size_t n = fread (buffer + sum, 1, buffer_length - sum, stream);
      જો (n == 0)
        {
          જો (ferror (stream))
            જાઓ cleanup_buffer;
          જાઓ final_process;
        }

      sum += n;

      જો (buffer_length == sum)
        તોડો;

      જો (feof (stream))
        જાઓ final_process;
    }

    માટે (size_t i = 0; i < sum; i++)
      {
        checksum = (checksum >> 1) + ((checksum & 1) << 15);
        checksum += buffer[i];
        checksum &= 0xffff;	/* Keep it within bounds. */
      }
    જો (ckd_add (&total_bytes, total_bytes, sum))
      {
        errno = EOVERFLOW;
        જાઓ cleanup_buffer;
      }
  }

final_process:;

  માટે (idx_t i = 0; i < sum; i++)
    {
      checksum = (checksum >> 1) + ((checksum & 1) << 15);
      checksum += buffer[i];
      checksum &= 0xffff;	/* Keep it within bounds. */
    }
  જો (ckd_add (&total_bytes, total_bytes, sum))
    {
      errno = EOVERFLOW;
      જાઓ cleanup_buffer;
    }

  memcpy (resstream, &checksum, કદ checksum);
  *length = total_bytes;
  ret = 0;
cleanup_buffer:
  મુક્ત (buffer);
  પાછા ret;
}

/* Calculate the checksum and the size in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

પૂર્ણાંક
sysv_sum_stream (FILE *stream, શૂન્ય *resstream, intmax_t *length)
{
  પૂર્ણાંક ret = -1;
  idx_t sum;
  intmax_t total_bytes = 0;
  ગણના { buffer_length = 32768 };
  uint8_t *buffer = malloc (buffer_length);

  જો (! buffer)
    પાછા -1;

  /* The sum of all the input bytes, modulo (UINT_MAX + 1).  */
  unsigned પૂર્ણાંક s = 0;

  /* Process file */
  જ્યારે (true)
  {
    sum = 0;

    /* Read block */
    જ્યારે (true)
    {
      size_t n = fread (buffer + sum, 1, buffer_length - sum, stream);
      જો (n == 0)
        {
          જો (ferror (stream))
            જાઓ cleanup_buffer;
          જાઓ final_process;
        }

      sum += n;

      જો (buffer_length == sum)
        તોડો;

      જો (feof (stream))
        જાઓ final_process;
    }

    માટે (size_t i = 0; i < sum; i++)
      s += buffer[i];
    જો (total_bytes + sum < total_bytes)
      {
        errno = EOVERFLOW;
        જાઓ cleanup_buffer;
      }
    total_bytes += sum;
  }

final_process:;

  માટે (size_t i = 0; i < sum; i++)
    s += buffer[i];
  જો (total_bytes + sum < total_bytes)
    {
      errno = EOVERFLOW;
      જાઓ cleanup_buffer;
    }
  total_bytes += sum;

  પૂર્ણાંક r = (s & 0xffff) + ((s & 0xffffffff) >> 16);
  પૂર્ણાંક checksum = (r & 0xffff) + (r >> 16);

  memcpy (resstream, &checksum, કદ checksum);
  *length = total_bytes;
  ret = 0;
cleanup_buffer:
  મુક્ત (buffer);
  પાછા ret;
}

/* Print the checksum and size (in 1024 byte blocks) to stdout.
   If ARGS is true, also print the FILE name.  */

શૂન્ય
output_bsd (અક્ષર અચલ *file, MAYBE_UNUSED પૂર્ણાંક દ્વિઅંકી_file, શૂન્ય અચલ *digest,
            bool raw, MAYBE_UNUSED bool tagged, unsigned અક્ષર delim, bool args,
            intmax_t length)
{
  જો (raw)
    {
      /* Output in network byte order (big endian).  */
      uint16_t out_int = *(પૂર્ણાંક *)digest;
      out_int = htobe16 (out_int);
      fwrite (&out_int, 1, 16/8, stdout);
      પાછા;
    }

  અક્ષર hbuf[LONGEST_HUMAN_READABLE + 1];
  printf ("%05d %5s", *(પૂર્ણાંક *)digest,
          human_readable (length, hbuf, human_ceiling, 1, 1024));
  જો (args)
    printf (" %s", file);
  putchar (delim);
}

/* Print the checksum and size (in 512 byte blocks) to stdout.
   If ARGS is true, also print the FILE name.  */

શૂન્ય
output_sysv (અક્ષર અચલ *file, MAYBE_UNUSED પૂર્ણાંક દ્વિઅંકી_file,
             શૂન્ય અચલ *digest, bool raw, MAYBE_UNUSED bool tagged,
             unsigned અક્ષર delim, bool args, intmax_t length)
{
  જો (raw)
    {
      /* Output in network byte order (big endian).  */
      uint16_t out_int = *(પૂર્ણાંક *)digest;
      out_int = htobe16 (out_int);
      fwrite (&out_int, 1, 16/8, stdout);
      પાછા;
    }

  અક્ષર hbuf[LONGEST_HUMAN_READABLE + 1];
  printf ("%d %s", *(પૂર્ણાંક *)digest,
          human_readable (length, hbuf, human_ceiling, 1, 512));
  જો (args)
    printf (" %s", file);
  putchar (delim);
}
