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
bsd_sum_stream (खाता *stream, शून्य *resstream, intmax_t *length)
{
  पूर्णाङ्क ret = -1;
  idx_t sum;
  पूर्णाङ्क checksum = 0;	/* The checksum mod 2^16. */
  intmax_t total_bytes = 0;	/* The number of bytes. */
  गणना { buffer_length = 32768 };
  uint8_t *buffer = दो_स्मृति (buffer_length);

  यदि (! buffer)
    फर्काऊ -1;

  /* Process file */
  जबसम्म (true)
  {
    sum = 0;

    /* Read block */
    जबसम्म (true)
    {
      माप_प्रकार n = ख_पढ़ो (buffer + sum, 1, buffer_length - sum, stream);
      यदि (n == 0)
        {
          यदि (ख_त्रुटि (stream))
            गच्छ cleanup_buffer;
          गच्छ final_process;
        }

      sum += n;

      यदि (buffer_length == sum)
        रोक;

      यदि (ख_पूर्ण (stream))
        गच्छ final_process;
    }

    लागि (माप_प्रकार i = 0; i < sum; i++)
      {
        checksum = (checksum >> 1) + ((checksum & 1) << 15);
        checksum += buffer[i];
        checksum &= 0xffff;	/* Keep it within bounds. */
      }
    यदि (ckd_add (&total_bytes, total_bytes, sum))
      {
        त्रुटि_सं = EOVERFLOW;
        गच्छ cleanup_buffer;
      }
  }

final_process:;

  लागि (idx_t i = 0; i < sum; i++)
    {
      checksum = (checksum >> 1) + ((checksum & 1) << 15);
      checksum += buffer[i];
      checksum &= 0xffff;	/* Keep it within bounds. */
    }
  यदि (ckd_add (&total_bytes, total_bytes, sum))
    {
      त्रुटि_सं = EOVERFLOW;
      गच्छ cleanup_buffer;
    }

  स_नकल (resstream, &checksum, परिमाण checksum);
  *length = total_bytes;
  ret = 0;
cleanup_buffer:
  मुक्त (buffer);
  फर्काऊ ret;
}

/* Calculate the checksum and the size in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

पूर्णाङ्क
sysv_sum_stream (खाता *stream, शून्य *resstream, intmax_t *length)
{
  पूर्णाङ्क ret = -1;
  idx_t sum;
  intmax_t total_bytes = 0;
  गणना { buffer_length = 32768 };
  uint8_t *buffer = दो_स्मृति (buffer_length);

  यदि (! buffer)
    फर्काऊ -1;

  /* The sum of all the input bytes, modulo (UINT_MAX + 1).  */
  अचिन्हित पूर्णाङ्क s = 0;

  /* Process file */
  जबसम्म (true)
  {
    sum = 0;

    /* Read block */
    जबसम्म (true)
    {
      माप_प्रकार n = ख_पढ़ो (buffer + sum, 1, buffer_length - sum, stream);
      यदि (n == 0)
        {
          यदि (ख_त्रुटि (stream))
            गच्छ cleanup_buffer;
          गच्छ final_process;
        }

      sum += n;

      यदि (buffer_length == sum)
        रोक;

      यदि (ख_पूर्ण (stream))
        गच्छ final_process;
    }

    लागि (माप_प्रकार i = 0; i < sum; i++)
      s += buffer[i];
    यदि (total_bytes + sum < total_bytes)
      {
        त्रुटि_सं = EOVERFLOW;
        गच्छ cleanup_buffer;
      }
    total_bytes += sum;
  }

final_process:;

  लागि (माप_प्रकार i = 0; i < sum; i++)
    s += buffer[i];
  यदि (total_bytes + sum < total_bytes)
    {
      त्रुटि_सं = EOVERFLOW;
      गच्छ cleanup_buffer;
    }
  total_bytes += sum;

  पूर्णाङ्क r = (s & 0xffff) + ((s & 0xffffffff) >> 16);
  पूर्णाङ्क checksum = (r & 0xffff) + (r >> 16);

  स_नकल (resstream, &checksum, परिमाण checksum);
  *length = total_bytes;
  ret = 0;
cleanup_buffer:
  मुक्त (buffer);
  फर्काऊ ret;
}

/* Print the checksum and size (in 1024 byte blocks) to stdout.
   If ARGS is true, also print the FILE name.  */

शून्य
output_bsd (वर्ण अचर *file, MAYBE_UNUSED पूर्णाङ्क द्विआधारी_file, शून्य अचर *digest,
            bool raw, MAYBE_UNUSED bool tagged, अचिन्हित वर्ण delim, bool args,
            intmax_t length)
{
  यदि (raw)
    {
      /* Output in network byte order (big endian).  */
      uint16_t out_int = *(पूर्णाङ्क *)digest;
      out_int = htobe16 (out_int);
      ख_डालो (&out_int, 1, 16/8, मानक_निकास);
      फर्काऊ;
    }

  वर्ण hbuf[LONGEST_HUMAN_READABLE + 1];
  म_लिखो ("%05d %5s", *(पूर्णाङ्क *)digest,
          human_readable (length, hbuf, human_ceiling, 1, 1024));
  यदि (args)
    म_लिखो (" %s", file);
  अक्षर_दो (delim);
}

/* Print the checksum and size (in 512 byte blocks) to stdout.
   If ARGS is true, also print the FILE name.  */

शून्य
output_sysv (वर्ण अचर *file, MAYBE_UNUSED पूर्णाङ्क द्विआधारी_file,
             शून्य अचर *digest, bool raw, MAYBE_UNUSED bool tagged,
             अचिन्हित वर्ण delim, bool args, intmax_t length)
{
  यदि (raw)
    {
      /* Output in network byte order (big endian).  */
      uint16_t out_int = *(पूर्णाङ्क *)digest;
      out_int = htobe16 (out_int);
      ख_डालो (&out_int, 1, 16/8, मानक_निकास);
      फर्काऊ;
    }

  वर्ण hbuf[LONGEST_HUMAN_READABLE + 1];
  म_लिखो ("%d %s", *(पूर्णाङ्क *)digest,
          human_readable (length, hbuf, human_ceiling, 1, 512));
  यदि (args)
    म_लिखो (" %s", file);
  अक्षर_दो (delim);
}
