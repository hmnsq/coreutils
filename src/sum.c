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
bsd_योग_प्रवाहः (FILE *प्रवाहः, शून्य *resstream, intmax_t *दीर्घता)
{
  पूर्णाङ्क ret = -1;
  idx_t योग;
  पूर्णाङ्क checksum = 0;	/* The checksum mod 2^16. */
  intmax_t योग_अष्टकानि = 0;	/* The number of bytes. */
  गणना { बफर_दीर्घता = 32768 };
  uint8_t *बफर = स्मृतिं_देहि (बफर_दीर्घता);

  यदि (! बफर)
    प्रत्यागम -1;

  /* Process file */
  यावत् (true)
  {
    योग = 0;

    /* Read block */
    यावत् (true)
    {
      size_t n = fread (बफर + योग, 1, बफर_दीर्घता - योग, प्रवाहः);
      यदि (n == 0)
        {
          यदि (ferror (प्रवाहः))
            गच्छ cleanup_बफर;
          गच्छ final_process;
        }

      योग += n;

      यदि (बफर_दीर्घता == योग)
        भङ्ग;

      यदि (feof (प्रवाहः))
        गच्छ final_process;
    }

    प्रति (size_t i = 0; i < योग; i++)
      {
        checksum = (checksum >> 1) + ((checksum & 1) << 15);
        checksum += बफर[i];
        checksum &= 0xffff;	/* Keep it within bounds. */
      }
    यदि (ckd_सङ्कलय (&योग_अष्टकानि, योग_अष्टकानि, योग))
      {
        errno = EOVERFLOW;
        गच्छ cleanup_बफर;
      }
  }

final_process:;

  प्रति (idx_t i = 0; i < योग; i++)
    {
      checksum = (checksum >> 1) + ((checksum & 1) << 15);
      checksum += बफर[i];
      checksum &= 0xffff;	/* Keep it within bounds. */
    }
  यदि (ckd_सङ्कलय (&योग_अष्टकानि, योग_अष्टकानि, योग))
    {
      errno = EOVERFLOW;
      गच्छ cleanup_बफर;
    }

  स्मृतिं_प्रतिलिख (resstream, &checksum, परिमाण checksum);
  *दीर्घता = योग_अष्टकानि;
  ret = 0;
cleanup_बफर:
  मुक्त (बफर);
  प्रत्यागम ret;
}

/* Calculate the checksum and the size in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

पूर्णाङ्क
sysv_योग_प्रवाहः (FILE *प्रवाहः, शून्य *resstream, intmax_t *दीर्घता)
{
  पूर्णाङ्क ret = -1;
  idx_t योग;
  intmax_t योग_अष्टकानि = 0;
  गणना { बफर_दीर्घता = 32768 };
  uint8_t *बफर = स्मृतिं_देहि (बफर_दीर्घता);

  यदि (! बफर)
    प्रत्यागम -1;

  /* The sum of all the input bytes, modulo (UINT_MAX + 1).  */
  अचिह्नित पूर्णाङ्क s = 0;

  /* Process file */
  यावत् (true)
  {
    योग = 0;

    /* Read block */
    यावत् (true)
    {
      size_t n = fread (बफर + योग, 1, बफर_दीर्घता - योग, प्रवाहः);
      यदि (n == 0)
        {
          यदि (ferror (प्रवाहः))
            गच्छ cleanup_बफर;
          गच्छ final_process;
        }

      योग += n;

      यदि (बफर_दीर्घता == योग)
        भङ्ग;

      यदि (feof (प्रवाहः))
        गच्छ final_process;
    }

    प्रति (size_t i = 0; i < योग; i++)
      s += बफर[i];
    यदि (योग_अष्टकानि + योग < योग_अष्टकानि)
      {
        errno = EOVERFLOW;
        गच्छ cleanup_बफर;
      }
    योग_अष्टकानि += योग;
  }

final_process:;

  प्रति (size_t i = 0; i < योग; i++)
    s += बफर[i];
  यदि (योग_अष्टकानि + योग < योग_अष्टकानि)
    {
      errno = EOVERFLOW;
      गच्छ cleanup_बफर;
    }
  योग_अष्टकानि += योग;

  पूर्णाङ्क r = (s & 0xffff) + ((s & 0xffffffff) >> 16);
  पूर्णाङ्क checksum = (r & 0xffff) + (r >> 16);

  स्मृतिं_प्रतिलिख (resstream, &checksum, परिमाण checksum);
  *दीर्घता = योग_अष्टकानि;
  ret = 0;
cleanup_बफर:
  मुक्त (बफर);
  प्रत्यागम ret;
}

/* Print the checksum and size (in 1024 byte blocks) to stdout.
   If ARGS is true, also print the FILE name.  */

शून्य
output_bsd (वर्ण ध्रुव *file, MAYBE_अप्रयुक्तम् पूर्णाङ्क द्विमान_file, शून्य ध्रुव *digest,
            bool raw, MAYBE_अप्रयुक्तम् bool tagged, अचिह्नित वर्ण delim, bool तर्काः,
            intmax_t दीर्घता)
{
  यदि (raw)
    {
      /* Output in network byte order (big endian).  */
      uint16_t बहिर्गमनम्_int = *(पूर्णाङ्क *)digest;
      बहिर्गमनम्_int = htobe16 (बहिर्गमनम्_int);
      fwrite (&बहिर्गमनम्_int, 1, 16/8, stdout);
      प्रत्यागम;
    }

  वर्ण hbuf[LONGEST_HUMAN_READABLE + 1];
  प्रारूप_लिख ("%05d %5s", *(पूर्णाङ्क *)digest,
          human_readable (दीर्घता, hbuf, human_ceiling, 1, 1024));
  यदि (तर्काः)
    प्रारूप_लिख (" %s", file);
  अक्षरं_लिख (delim);
}

/* Print the checksum and size (in 512 byte blocks) to stdout.
   If ARGS is true, also print the FILE name.  */

शून्य
output_sysv (वर्ण ध्रुव *file, MAYBE_अप्रयुक्तम् पूर्णाङ्क द्विमान_file,
             शून्य ध्रुव *digest, bool raw, MAYBE_अप्रयुक्तम् bool tagged,
             अचिह्नित वर्ण delim, bool तर्काः, intmax_t दीर्घता)
{
  यदि (raw)
    {
      /* Output in network byte order (big endian).  */
      uint16_t बहिर्गमनम्_int = *(पूर्णाङ्क *)digest;
      बहिर्गमनम्_int = htobe16 (बहिर्गमनम्_int);
      fwrite (&बहिर्गमनम्_int, 1, 16/8, stdout);
      प्रत्यागम;
    }

  वर्ण hbuf[LONGEST_HUMAN_READABLE + 1];
  प्रारूप_लिख ("%d %s", *(पूर्णाङ्क *)digest,
          human_readable (दीर्घता, hbuf, human_ceiling, 1, 512));
  यदि (तर्काः)
    प्रारूप_लिख (" %s", file);
  अक्षरं_लिख (delim);
}
