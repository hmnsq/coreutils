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

#അന്തര്ഭാവ <config.h>

#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <endian.h>
#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "human.h"
#അന്തര്ഭാവ "sum.h"

/* Calculate the checksum and the size in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

പൂർണ്ണസംഖ്യ
bsd_sum_stream (FILE *stream, ശൂന്യം *resstream, intmax_t *length)
{
  പൂർണ്ണസംഖ്യ ret = -1;
  idx_t sum;
  പൂർണ്ണസംഖ്യ checksum = 0;	/* The checksum mod 2^16. */
  intmax_t total_bytes = 0;	/* The number of bytes. */
  എണ്ണൽ { buffer_length = 32768 };
  uint8_t *buffer = malloc (buffer_length);

  എങ്കിൽ (! buffer)
    തിരികെ -1;

  /* Process file */
  ആകുമ്പോൾ (true)
  {
    sum = 0;

    /* Read block */
    ആകുമ്പോൾ (true)
    {
      size_t n = fread (buffer + sum, 1, buffer_length - sum, stream);
      എങ്കിൽ (n == 0)
        {
          എങ്കിൽ (ferror (stream))
            പോകുക cleanup_buffer;
          പോകുക final_process;
        }

      sum += n;

      എങ്കിൽ (buffer_length == sum)
        നിർത്തുക;

      എങ്കിൽ (feof (stream))
        പോകുക final_process;
    }

    വേണ്ടി (size_t i = 0; i < sum; i++)
      {
        checksum = (checksum >> 1) + ((checksum & 1) << 15);
        checksum += buffer[i];
        checksum &= 0xffff;	/* Keep it within bounds. */
      }
    എങ്കിൽ (ckd_add (&total_bytes, total_bytes, sum))
      {
        errno = EOVERFLOW;
        പോകുക cleanup_buffer;
      }
  }

final_process:;

  വേണ്ടി (idx_t i = 0; i < sum; i++)
    {
      checksum = (checksum >> 1) + ((checksum & 1) << 15);
      checksum += buffer[i];
      checksum &= 0xffff;	/* Keep it within bounds. */
    }
  എങ്കിൽ (ckd_add (&total_bytes, total_bytes, sum))
    {
      errno = EOVERFLOW;
      പോകുക cleanup_buffer;
    }

  memcpy (resstream, &checksum, വലുപ്പം checksum);
  *length = total_bytes;
  ret = 0;
cleanup_buffer:
  സ്വതന്ത്രം (buffer);
  തിരികെ ret;
}

/* Calculate the checksum and the size in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

പൂർണ്ണസംഖ്യ
sysv_sum_stream (FILE *stream, ശൂന്യം *resstream, intmax_t *length)
{
  പൂർണ്ണസംഖ്യ ret = -1;
  idx_t sum;
  intmax_t total_bytes = 0;
  എണ്ണൽ { buffer_length = 32768 };
  uint8_t *buffer = malloc (buffer_length);

  എങ്കിൽ (! buffer)
    തിരികെ -1;

  /* The sum of all the input bytes, modulo (UINT_MAX + 1).  */
  unsigned പൂർണ്ണസംഖ്യ s = 0;

  /* Process file */
  ആകുമ്പോൾ (true)
  {
    sum = 0;

    /* Read block */
    ആകുമ്പോൾ (true)
    {
      size_t n = fread (buffer + sum, 1, buffer_length - sum, stream);
      എങ്കിൽ (n == 0)
        {
          എങ്കിൽ (ferror (stream))
            പോകുക cleanup_buffer;
          പോകുക final_process;
        }

      sum += n;

      എങ്കിൽ (buffer_length == sum)
        നിർത്തുക;

      എങ്കിൽ (feof (stream))
        പോകുക final_process;
    }

    വേണ്ടി (size_t i = 0; i < sum; i++)
      s += buffer[i];
    എങ്കിൽ (total_bytes + sum < total_bytes)
      {
        errno = EOVERFLOW;
        പോകുക cleanup_buffer;
      }
    total_bytes += sum;
  }

final_process:;

  വേണ്ടി (size_t i = 0; i < sum; i++)
    s += buffer[i];
  എങ്കിൽ (total_bytes + sum < total_bytes)
    {
      errno = EOVERFLOW;
      പോകുക cleanup_buffer;
    }
  total_bytes += sum;

  പൂർണ്ണസംഖ്യ r = (s & 0xffff) + ((s & 0xffffffff) >> 16);
  പൂർണ്ണസംഖ്യ checksum = (r & 0xffff) + (r >> 16);

  memcpy (resstream, &checksum, വലുപ്പം checksum);
  *length = total_bytes;
  ret = 0;
cleanup_buffer:
  സ്വതന്ത്രം (buffer);
  തിരികെ ret;
}

/* Print the checksum and size (in 1024 byte blocks) to stdout.
   If ARGS is true, also print the FILE name.  */

ശൂന്യം
output_bsd (അക്ഷരം സ്ഥിരം *file, MAYBE_UNUSED പൂർണ്ണസംഖ്യ ദ്വിമാന_file, ശൂന്യം സ്ഥിരം *digest,
            bool raw, MAYBE_UNUSED bool tagged, unsigned അക്ഷരം delim, bool args,
            intmax_t length)
{
  എങ്കിൽ (raw)
    {
      /* Output in network byte order (big endian).  */
      uint16_t out_int = *(പൂർണ്ണസംഖ്യ *)digest;
      out_int = htobe16 (out_int);
      fwrite (&out_int, 1, 16/8, stdout);
      തിരികെ;
    }

  അക്ഷരം hbuf[LONGEST_HUMAN_READABLE + 1];
  printf ("%05d %5s", *(പൂർണ്ണസംഖ്യ *)digest,
          human_readable (length, hbuf, human_ceiling, 1, 1024));
  എങ്കിൽ (args)
    printf (" %s", file);
  putchar (delim);
}

/* Print the checksum and size (in 512 byte blocks) to stdout.
   If ARGS is true, also print the FILE name.  */

ശൂന്യം
output_sysv (അക്ഷരം സ്ഥിരം *file, MAYBE_UNUSED പൂർണ്ണസംഖ്യ ദ്വിമാന_file,
             ശൂന്യം സ്ഥിരം *digest, bool raw, MAYBE_UNUSED bool tagged,
             unsigned അക്ഷരം delim, bool args, intmax_t length)
{
  എങ്കിൽ (raw)
    {
      /* Output in network byte order (big endian).  */
      uint16_t out_int = *(പൂർണ്ണസംഖ്യ *)digest;
      out_int = htobe16 (out_int);
      fwrite (&out_int, 1, 16/8, stdout);
      തിരികെ;
    }

  അക്ഷരം hbuf[LONGEST_HUMAN_READABLE + 1];
  printf ("%d %s", *(പൂർണ്ണസംഖ്യ *)digest,
          human_readable (length, hbuf, human_ceiling, 1, 512));
  എങ്കിൽ (args)
    printf (" %s", file);
  putchar (delim);
}
