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

#अन्तर्भाव <config.h>

#अन्तर्भाव <stdio.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <endian.h>
#अन्तर्भाव "system.h"
#अन्तर्भाव "human.h"
#अन्तर्भाव "sum.h"

/* Calculate the checksum and the size in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

पूर्णाङ्क
bsd_sum_stream (FILE *stream, शून्य *resstream, intmax_t *length)
{
  पूर्णाङ्क ret = -1;
  idx_t sum;
  पूर्णाङ्क checksum = 0;	/* The checksum mod 2^16. */
  intmax_t total_bytes = 0;	/* The number of bytes. */
  गणना { buffer_length = 32768 };
  uint8_t *buffer = malloc (buffer_length);

  यदि (! buffer)
    प्रत्यागम -1;

  /* Process file */
  यावत् (true)
  {
    sum = 0;

    /* Read block */
    यावत् (true)
    {
      size_t n = fread (buffer + sum, 1, buffer_length - sum, stream);
      यदि (n == 0)
        {
          यदि (ferror (stream))
            गच्छ cleanup_buffer;
          गच्छ final_process;
        }

      sum += n;

      यदि (buffer_length == sum)
        भङ्ग;

      यदि (feof (stream))
        गच्छ final_process;
    }

    प्रति (size_t i = 0; i < sum; i++)
      {
        checksum = (checksum >> 1) + ((checksum & 1) << 15);
        checksum += buffer[i];
        checksum &= 0xffff;	/* Keep it within bounds. */
      }
    यदि (ckd_add (&total_bytes, total_bytes, sum))
      {
        errno = EOVERFLOW;
        गच्छ cleanup_buffer;
      }
  }

final_process:;

  प्रति (idx_t i = 0; i < sum; i++)
    {
      checksum = (checksum >> 1) + ((checksum & 1) << 15);
      checksum += buffer[i];
      checksum &= 0xffff;	/* Keep it within bounds. */
    }
  यदि (ckd_add (&total_bytes, total_bytes, sum))
    {
      errno = EOVERFLOW;
      गच्छ cleanup_buffer;
    }

  memcpy (resstream, &checksum, परिमाण checksum);
  *length = total_bytes;
  ret = 0;
cleanup_buffer:
  मुक्त (buffer);
  प्रत्यागम ret;
}

/* Calculate the checksum and the size in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

पूर्णाङ्क
sysv_sum_stream (FILE *stream, शून्य *resstream, intmax_t *length)
{
  पूर्णाङ्क ret = -1;
  idx_t sum;
  intmax_t total_bytes = 0;
  गणना { buffer_length = 32768 };
  uint8_t *buffer = malloc (buffer_length);

  यदि (! buffer)
    प्रत्यागम -1;

  /* The sum of all the input bytes, modulo (UINT_MAX + 1).  */
  unsigned पूर्णाङ्क s = 0;

  /* Process file */
  यावत् (true)
  {
    sum = 0;

    /* Read block */
    यावत् (true)
    {
      size_t n = fread (buffer + sum, 1, buffer_length - sum, stream);
      यदि (n == 0)
        {
          यदि (ferror (stream))
            गच्छ cleanup_buffer;
          गच्छ final_process;
        }

      sum += n;

      यदि (buffer_length == sum)
        भङ्ग;

      यदि (feof (stream))
        गच्छ final_process;
    }

    प्रति (size_t i = 0; i < sum; i++)
      s += buffer[i];
    यदि (total_bytes + sum < total_bytes)
      {
        errno = EOVERFLOW;
        गच्छ cleanup_buffer;
      }
    total_bytes += sum;
  }

final_process:;

  प्रति (size_t i = 0; i < sum; i++)
    s += buffer[i];
  यदि (total_bytes + sum < total_bytes)
    {
      errno = EOVERFLOW;
      गच्छ cleanup_buffer;
    }
  total_bytes += sum;

  पूर्णाङ्क r = (s & 0xffff) + ((s & 0xffffffff) >> 16);
  पूर्णाङ्क checksum = (r & 0xffff) + (r >> 16);

  memcpy (resstream, &checksum, परिमाण checksum);
  *length = total_bytes;
  ret = 0;
cleanup_buffer:
  मुक्त (buffer);
  प्रत्यागम ret;
}

/* Print the checksum and size (in 1024 byte blocks) to stdout.
   If ARGS is true, also print the FILE name.  */

शून्य
output_bsd (वर्ण ध्रुव *file, MAYBE_UNUSED पूर्णाङ्क द्विआधारी_file, शून्य ध्रुव *digest,
            bool raw, MAYBE_UNUSED bool tagged, unsigned वर्ण delim, bool args,
            intmax_t length)
{
  यदि (raw)
    {
      /* Output in network byte order (big endian).  */
      uint16_t out_int = *(पूर्णाङ्क *)digest;
      out_int = htobe16 (out_int);
      fwrite (&out_int, 1, 16/8, stdout);
      प्रत्यागम;
    }

  वर्ण hbuf[LONGEST_HUMAN_READABLE + 1];
  printf ("%05d %5s", *(पूर्णाङ्क *)digest,
          human_readable (length, hbuf, human_ceiling, 1, 1024));
  यदि (args)
    printf (" %s", file);
  putchar (delim);
}

/* Print the checksum and size (in 512 byte blocks) to stdout.
   If ARGS is true, also print the FILE name.  */

शून्य
output_sysv (वर्ण ध्रुव *file, MAYBE_UNUSED पूर्णाङ्क द्विआधारी_file,
             शून्य ध्रुव *digest, bool raw, MAYBE_UNUSED bool tagged,
             unsigned वर्ण delim, bool args, intmax_t length)
{
  यदि (raw)
    {
      /* Output in network byte order (big endian).  */
      uint16_t out_int = *(पूर्णाङ्क *)digest;
      out_int = htobe16 (out_int);
      fwrite (&out_int, 1, 16/8, stdout);
      प्रत्यागम;
    }

  वर्ण hbuf[LONGEST_HUMAN_READABLE + 1];
  printf ("%d %s", *(पूर्णाङ्क *)digest,
          human_readable (length, hbuf, human_ceiling, 1, 512));
  यदि (args)
    printf (" %s", file);
  putchar (delim);
}
