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

#ಅನ್ತರ್ಭಾವ <config.h>

#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <endian.h>
#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "human.h"
#ಅನ್ತರ್ಭಾವ "sum.h"

/* Calculate the checksum and the size in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

ಪೂರ್ಣಾಙ್ಕ
bsd_sum_stream (FILE *stream, ಶೂನ್ಯ *resstream, intmax_t *length)
{
  ಪೂರ್ಣಾಙ್ಕ ret = -1;
  idx_t sum;
  ಪೂರ್ಣಾಙ್ಕ checksum = 0;	/* The checksum mod 2^16. */
  intmax_t total_bytes = 0;	/* The number of bytes. */
  ಗಣನಾ { buffer_length = 32768 };
  uint8_t *buffer = malloc (buffer_length);

  ಯದಿ (! buffer)
    ಪ್ರತ್ಯಾಗಮ -1;

  /* Process file */
  ಯಾವತ್ (true)
  {
    sum = 0;

    /* Read block */
    ಯಾವತ್ (true)
    {
      size_t n = fread (buffer + sum, 1, buffer_length - sum, stream);
      ಯದಿ (n == 0)
        {
          ಯದಿ (ferror (stream))
            ಗಚ್ಛ cleanup_buffer;
          ಗಚ್ಛ final_process;
        }

      sum += n;

      ಯದಿ (buffer_length == sum)
        ಭಙ್ಗ;

      ಯದಿ (feof (stream))
        ಗಚ್ಛ final_process;
    }

    ಪ್ರತಿ (size_t i = 0; i < sum; i++)
      {
        checksum = (checksum >> 1) + ((checksum & 1) << 15);
        checksum += buffer[i];
        checksum &= 0xffff;	/* Keep it within bounds. */
      }
    ಯದಿ (ckd_add (&total_bytes, total_bytes, sum))
      {
        errno = EOVERFLOW;
        ಗಚ್ಛ cleanup_buffer;
      }
  }

final_process:;

  ಪ್ರತಿ (idx_t i = 0; i < sum; i++)
    {
      checksum = (checksum >> 1) + ((checksum & 1) << 15);
      checksum += buffer[i];
      checksum &= 0xffff;	/* Keep it within bounds. */
    }
  ಯದಿ (ckd_add (&total_bytes, total_bytes, sum))
    {
      errno = EOVERFLOW;
      ಗಚ್ಛ cleanup_buffer;
    }

  memcpy (resstream, &checksum, ಪರಿಮಾಣ checksum);
  *length = total_bytes;
  ret = 0;
cleanup_buffer:
  ಮುಕ್ತ (buffer);
  ಪ್ರತ್ಯಾಗಮ ret;
}

/* Calculate the checksum and the size in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

ಪೂರ್ಣಾಙ್ಕ
sysv_sum_stream (FILE *stream, ಶೂನ್ಯ *resstream, intmax_t *length)
{
  ಪೂರ್ಣಾಙ್ಕ ret = -1;
  idx_t sum;
  intmax_t total_bytes = 0;
  ಗಣನಾ { buffer_length = 32768 };
  uint8_t *buffer = malloc (buffer_length);

  ಯದಿ (! buffer)
    ಪ್ರತ್ಯಾಗಮ -1;

  /* The sum of all the input bytes, modulo (UINT_MAX + 1).  */
  unsigned ಪೂರ್ಣಾಙ್ಕ s = 0;

  /* Process file */
  ಯಾವತ್ (true)
  {
    sum = 0;

    /* Read block */
    ಯಾವತ್ (true)
    {
      size_t n = fread (buffer + sum, 1, buffer_length - sum, stream);
      ಯದಿ (n == 0)
        {
          ಯದಿ (ferror (stream))
            ಗಚ್ಛ cleanup_buffer;
          ಗಚ್ಛ final_process;
        }

      sum += n;

      ಯದಿ (buffer_length == sum)
        ಭಙ್ಗ;

      ಯದಿ (feof (stream))
        ಗಚ್ಛ final_process;
    }

    ಪ್ರತಿ (size_t i = 0; i < sum; i++)
      s += buffer[i];
    ಯದಿ (total_bytes + sum < total_bytes)
      {
        errno = EOVERFLOW;
        ಗಚ್ಛ cleanup_buffer;
      }
    total_bytes += sum;
  }

final_process:;

  ಪ್ರತಿ (size_t i = 0; i < sum; i++)
    s += buffer[i];
  ಯದಿ (total_bytes + sum < total_bytes)
    {
      errno = EOVERFLOW;
      ಗಚ್ಛ cleanup_buffer;
    }
  total_bytes += sum;

  ಪೂರ್ಣಾಙ್ಕ r = (s & 0xffff) + ((s & 0xffffffff) >> 16);
  ಪೂರ್ಣಾಙ್ಕ checksum = (r & 0xffff) + (r >> 16);

  memcpy (resstream, &checksum, ಪರಿಮಾಣ checksum);
  *length = total_bytes;
  ret = 0;
cleanup_buffer:
  ಮುಕ್ತ (buffer);
  ಪ್ರತ್ಯಾಗಮ ret;
}

/* Print the checksum and size (in 1024 byte blocks) to stdout.
   If ARGS is true, also print the FILE name.  */

ಶೂನ್ಯ
output_bsd (ವರ್ಣ ಧ್ರುವ *file, MAYBE_UNUSED ಪೂರ್ಣಾಙ್ಕ ದ್ವಿಮಾನ_file, ಶೂನ್ಯ ಧ್ರುವ *digest,
            bool raw, MAYBE_UNUSED bool tagged, unsigned ವರ್ಣ delim, bool args,
            intmax_t length)
{
  ಯದಿ (raw)
    {
      /* Output in network byte order (big endian).  */
      uint16_t out_int = *(ಪೂರ್ಣಾಙ್ಕ *)digest;
      out_int = htobe16 (out_int);
      fwrite (&out_int, 1, 16/8, stdout);
      ಪ್ರತ್ಯಾಗಮ;
    }

  ವರ್ಣ hbuf[LONGEST_HUMAN_READABLE + 1];
  printf ("%05d %5s", *(ಪೂರ್ಣಾಙ್ಕ *)digest,
          human_readable (length, hbuf, human_ceiling, 1, 1024));
  ಯದಿ (args)
    printf (" %s", file);
  putchar (delim);
}

/* Print the checksum and size (in 512 byte blocks) to stdout.
   If ARGS is true, also print the FILE name.  */

ಶೂನ್ಯ
output_sysv (ವರ್ಣ ಧ್ರುವ *file, MAYBE_UNUSED ಪೂರ್ಣಾಙ್ಕ ದ್ವಿಮಾನ_file,
             ಶೂನ್ಯ ಧ್ರುವ *digest, bool raw, MAYBE_UNUSED bool tagged,
             unsigned ವರ್ಣ delim, bool args, intmax_t length)
{
  ಯದಿ (raw)
    {
      /* Output in network byte order (big endian).  */
      uint16_t out_int = *(ಪೂರ್ಣಾಙ್ಕ *)digest;
      out_int = htobe16 (out_int);
      fwrite (&out_int, 1, 16/8, stdout);
      ಪ್ರತ್ಯಾಗಮ;
    }

  ವರ್ಣ hbuf[LONGEST_HUMAN_READABLE + 1];
  printf ("%d %s", *(ಪೂರ್ಣಾಙ್ಕ *)digest,
          human_readable (length, hbuf, human_ceiling, 1, 512));
  ಯದಿ (args)
    printf (" %s", file);
  putchar (delim);
}
