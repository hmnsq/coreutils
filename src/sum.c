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

পূর্ণসংখ্যা
bsd_যোগ_stream (FILE *stream, শূন্য *resstream, intmax_t *দৈর্ঘ্য)
{
  পূর্ণসংখ্যা ret = -1;
  idx_t যোগ;
  পূর্ণসংখ্যা checksum = 0;	/* The checksum mod 2^16. */
  intmax_t যোগ_bytes = 0;	/* The number of bytes. */
  গণনা { বাফার_দৈর্ঘ্য = 32768 };
  uint8_t *বাফার = malloc (বাফার_দৈর্ঘ্য);

  যদি (! বাফার)
    ফেরত -1;

  /* Process file */
  যখন (true)
  {
    যোগ = 0;

    /* Read block */
    যখন (true)
    {
      size_t n = fread (বাফার + যোগ, 1, বাফার_দৈর্ঘ্য - যোগ, stream);
      যদি (n == 0)
        {
          যদি (ferror (stream))
            যাও cleanup_বাফার;
          যাও final_process;
        }

      যোগ += n;

      যদি (বাফার_দৈর্ঘ্য == যোগ)
        বিরতি;

      যদি (feof (stream))
        যাও final_process;
    }

    জন্য (size_t i = 0; i < যোগ; i++)
      {
        checksum = (checksum >> 1) + ((checksum & 1) << 15);
        checksum += বাফার[i];
        checksum &= 0xffff;	/* Keep it within bounds. */
      }
    যদি (ckd_add (&যোগ_bytes, যোগ_bytes, যোগ))
      {
        errno = EOVERFLOW;
        যাও cleanup_বাফার;
      }
  }

final_process:;

  জন্য (idx_t i = 0; i < যোগ; i++)
    {
      checksum = (checksum >> 1) + ((checksum & 1) << 15);
      checksum += বাফার[i];
      checksum &= 0xffff;	/* Keep it within bounds. */
    }
  যদি (ckd_add (&যোগ_bytes, যোগ_bytes, যোগ))
    {
      errno = EOVERFLOW;
      যাও cleanup_বাফার;
    }

  memcpy (resstream, &checksum, আকার checksum);
  *দৈর্ঘ্য = যোগ_bytes;
  ret = 0;
cleanup_বাফার:
  মুক্ত (বাফার);
  ফেরত ret;
}

/* Calculate the checksum and the size in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

পূর্ণসংখ্যা
sysv_যোগ_stream (FILE *stream, শূন্য *resstream, intmax_t *দৈর্ঘ্য)
{
  পূর্ণসংখ্যা ret = -1;
  idx_t যোগ;
  intmax_t যোগ_bytes = 0;
  গণনা { বাফার_দৈর্ঘ্য = 32768 };
  uint8_t *বাফার = malloc (বাফার_দৈর্ঘ্য);

  যদি (! বাফার)
    ফেরত -1;

  /* The sum of all the input bytes, modulo (UINT_MAX + 1).  */
  unsigned পূর্ণসংখ্যা s = 0;

  /* Process file */
  যখন (true)
  {
    যোগ = 0;

    /* Read block */
    যখন (true)
    {
      size_t n = fread (বাফার + যোগ, 1, বাফার_দৈর্ঘ্য - যোগ, stream);
      যদি (n == 0)
        {
          যদি (ferror (stream))
            যাও cleanup_বাফার;
          যাও final_process;
        }

      যোগ += n;

      যদি (বাফার_দৈর্ঘ্য == যোগ)
        বিরতি;

      যদি (feof (stream))
        যাও final_process;
    }

    জন্য (size_t i = 0; i < যোগ; i++)
      s += বাফার[i];
    যদি (যোগ_bytes + যোগ < যোগ_bytes)
      {
        errno = EOVERFLOW;
        যাও cleanup_বাফার;
      }
    যোগ_bytes += যোগ;
  }

final_process:;

  জন্য (size_t i = 0; i < যোগ; i++)
    s += বাফার[i];
  যদি (যোগ_bytes + যোগ < যোগ_bytes)
    {
      errno = EOVERFLOW;
      যাও cleanup_বাফার;
    }
  যোগ_bytes += যোগ;

  পূর্ণসংখ্যা r = (s & 0xffff) + ((s & 0xffffffff) >> 16);
  পূর্ণসংখ্যা checksum = (r & 0xffff) + (r >> 16);

  memcpy (resstream, &checksum, আকার checksum);
  *দৈর্ঘ্য = যোগ_bytes;
  ret = 0;
cleanup_বাফার:
  মুক্ত (বাফার);
  ফেরত ret;
}

/* Print the checksum and size (in 1024 byte blocks) to stdout.
   If ARGS is true, also print the FILE name.  */

শূন্য
output_bsd (অক্ষর ধ্রুবক *file, MAYBE_UNUSED পূর্ণসংখ্যা দ্বিমিক_file, শূন্য ধ্রুবক *digest,
            bool raw, MAYBE_UNUSED bool tagged, unsigned অক্ষর delim, bool args,
            intmax_t দৈর্ঘ্য)
{
  যদি (raw)
    {
      /* Output in network byte order (big endian).  */
      uint16_t out_int = *(পূর্ণসংখ্যা *)digest;
      out_int = htobe16 (out_int);
      fwrite (&out_int, 1, 16/8, stdout);
      ফেরত;
    }

  অক্ষর hbuf[LONGEST_HUMAN_READABLE + 1];
  printf ("%05d %5s", *(পূর্ণসংখ্যা *)digest,
          human_readable (দৈর্ঘ্য, hbuf, human_ceiling, 1, 1024));
  যদি (args)
    printf (" %s", file);
  putchar (delim);
}

/* Print the checksum and size (in 512 byte blocks) to stdout.
   If ARGS is true, also print the FILE name.  */

শূন্য
output_sysv (অক্ষর ধ্রুবক *file, MAYBE_UNUSED পূর্ণসংখ্যা দ্বিমিক_file,
             শূন্য ধ্রুবক *digest, bool raw, MAYBE_UNUSED bool tagged,
             unsigned অক্ষর delim, bool args, intmax_t দৈর্ঘ্য)
{
  যদি (raw)
    {
      /* Output in network byte order (big endian).  */
      uint16_t out_int = *(পূর্ণসংখ্যা *)digest;
      out_int = htobe16 (out_int);
      fwrite (&out_int, 1, 16/8, stdout);
      ফেরত;
    }

  অক্ষর hbuf[LONGEST_HUMAN_READABLE + 1];
  printf ("%d %s", *(পূর্ণসংখ্যা *)digest,
          human_readable (দৈর্ঘ্য, hbuf, human_ceiling, 1, 512));
  যদি (args)
    printf (" %s", file);
  putchar (delim);
}
