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

#समावेश <config.h>

#समावेश <stdio.h>
#समावेश <sys/types.h>
#समावेश <endian.h>
#समावेश "system.h"
#समावेश "human.h"
#समावेश "sum.h"

/* Calculate the checksum and the size in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

पूर्णांक
bsd_योग_stream (खाता *stream, व्योम *resstream, intmax_t *लंबाई)
{
  पूर्णांक ret = -1;
  idx_t योग;
  पूर्णांक checksum = 0;	/* The checksum mod 2^16. */
  intmax_t योग_bytes = 0;	/* The number of bytes. */
  क्रमागत { बफर_लंबाई = 32768 };
  uint8_t *बफर = दो_स्मृति (बफर_लंबाई);

  अगर (! बफर)
    वापस -1;

  /* Process file */
  जबतक (true)
  {
    योग = 0;

    /* Read block */
    जबतक (true)
    {
      माप_प्रकार n = ख_पढ़ो (बफर + योग, 1, बफर_लंबाई - योग, stream);
      अगर (n == 0)
        {
          अगर (ख_त्रुटि (stream))
            जाओ cleanup_बफर;
          जाओ final_process;
        }

      योग += n;

      अगर (बफर_लंबाई == योग)
        अवरोध;

      अगर (ख_पूर्ण (stream))
        जाओ final_process;
    }

    क्रम (माप_प्रकार i = 0; i < योग; i++)
      {
        checksum = (checksum >> 1) + ((checksum & 1) << 15);
        checksum += बफर[i];
        checksum &= 0xffff;	/* Keep it within bounds. */
      }
    अगर (ckd_add (&योग_bytes, योग_bytes, योग))
      {
        त्रुटि_सं = EOVERFLOW;
        जाओ cleanup_बफर;
      }
  }

final_process:;

  क्रम (idx_t i = 0; i < योग; i++)
    {
      checksum = (checksum >> 1) + ((checksum & 1) << 15);
      checksum += बफर[i];
      checksum &= 0xffff;	/* Keep it within bounds. */
    }
  अगर (ckd_add (&योग_bytes, योग_bytes, योग))
    {
      त्रुटि_सं = EOVERFLOW;
      जाओ cleanup_बफर;
    }

  स_नकल (resstream, &checksum, माप checksum);
  *लंबाई = योग_bytes;
  ret = 0;
cleanup_बफर:
  मुक्त (बफर);
  वापस ret;
}

/* Calculate the checksum and the size in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

पूर्णांक
sysv_योग_stream (खाता *stream, व्योम *resstream, intmax_t *लंबाई)
{
  पूर्णांक ret = -1;
  idx_t योग;
  intmax_t योग_bytes = 0;
  क्रमागत { बफर_लंबाई = 32768 };
  uint8_t *बफर = दो_स्मृति (बफर_लंबाई);

  अगर (! बफर)
    वापस -1;

  /* The sum of all the input bytes, modulo (UINT_MAX + 1).  */
  अचिन्हित पूर्णांक s = 0;

  /* Process file */
  जबतक (true)
  {
    योग = 0;

    /* Read block */
    जबतक (true)
    {
      माप_प्रकार n = ख_पढ़ो (बफर + योग, 1, बफर_लंबाई - योग, stream);
      अगर (n == 0)
        {
          अगर (ख_त्रुटि (stream))
            जाओ cleanup_बफर;
          जाओ final_process;
        }

      योग += n;

      अगर (बफर_लंबाई == योग)
        अवरोध;

      अगर (ख_पूर्ण (stream))
        जाओ final_process;
    }

    क्रम (माप_प्रकार i = 0; i < योग; i++)
      s += बफर[i];
    अगर (योग_bytes + योग < योग_bytes)
      {
        त्रुटि_सं = EOVERFLOW;
        जाओ cleanup_बफर;
      }
    योग_bytes += योग;
  }

final_process:;

  क्रम (माप_प्रकार i = 0; i < योग; i++)
    s += बफर[i];
  अगर (योग_bytes + योग < योग_bytes)
    {
      त्रुटि_सं = EOVERFLOW;
      जाओ cleanup_बफर;
    }
  योग_bytes += योग;

  पूर्णांक r = (s & 0xffff) + ((s & 0xffffffff) >> 16);
  पूर्णांक checksum = (r & 0xffff) + (r >> 16);

  स_नकल (resstream, &checksum, माप checksum);
  *लंबाई = योग_bytes;
  ret = 0;
cleanup_बफर:
  मुक्त (बफर);
  वापस ret;
}

/* Print the checksum and size (in 1024 byte blocks) to stdout.
   If ARGS is true, also print the FILE name.  */

व्योम
output_bsd (अक्षर स्थिर *file, MAYBE_UNUSED पूर्णांक द्विआधारी_file, व्योम स्थिर *digest,
            bool raw, MAYBE_UNUSED bool tagged, अचिन्हित अक्षर delim, bool args,
            intmax_t लंबाई)
{
  अगर (raw)
    {
      /* Output in network byte order (big endian).  */
      uint16_t out_int = *(पूर्णांक *)digest;
      out_int = htobe16 (out_int);
      ख_डालो (&out_int, 1, 16/8, मानक_निकास);
      वापस;
    }

  अक्षर hbuf[LONGEST_HUMAN_READABLE + 1];
  म_लिखो ("%05d %5s", *(पूर्णांक *)digest,
          human_readable (लंबाई, hbuf, human_ceiling, 1, 1024));
  अगर (args)
    म_लिखो (" %s", file);
  अक्षर_दो (delim);
}

/* Print the checksum and size (in 512 byte blocks) to stdout.
   If ARGS is true, also print the FILE name.  */

व्योम
output_sysv (अक्षर स्थिर *file, MAYBE_UNUSED पूर्णांक द्विआधारी_file,
             व्योम स्थिर *digest, bool raw, MAYBE_UNUSED bool tagged,
             अचिन्हित अक्षर delim, bool args, intmax_t लंबाई)
{
  अगर (raw)
    {
      /* Output in network byte order (big endian).  */
      uint16_t out_int = *(पूर्णांक *)digest;
      out_int = htobe16 (out_int);
      ख_डालो (&out_int, 1, 16/8, मानक_निकास);
      वापस;
    }

  अक्षर hbuf[LONGEST_HUMAN_READABLE + 1];
  म_लिखो ("%d %s", *(पूर्णांक *)digest,
          human_readable (लंबाई, hbuf, human_ceiling, 1, 512));
  अगर (args)
    म_लिखो (" %s", file);
  अक्षर_दो (delim);
}
