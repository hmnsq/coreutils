/* cksum_crc -- calculate and print POSIX checksums and sizes of files
   Copyright (C) 1992-2026 Free Software Foundation, Inc.

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

/* Written by Q. Frank Xia, qx@math.columbia.edu.
   Cosmetic changes and reorganization by David MacKenzie, djm@gnu.ai.mit.edu.

  The code segment between "#ifdef CRCTAB" and "#else" is the code
  which generates crctab.c

  This software is compatible with neither the System V nor the BSD
  'sum' program.  It is supposed to conform to POSIX, except perhaps
  for foreign language support.  Any inconsistency with the standard
  (other than foreign language support) is a bug.  */

/* This include must be at the top of the file to satisfy
   sc_require_config_h_first.  */
#अगर_अघोषित CRCTAB
# include <config.h>
#पूर्ण_अगर

#अगर_घोषित CRCTAB

# include <stdio.h>

# define BIT(x)	(1u << (x))
# define SBIT	BIT (31)

/* The generating polynomial is

          32   26   23   22   16   12   11   10   8   7   5   4   2   1
    G(X)=X  + X  + X  + X  + X  + X  + X  + X  + X + X + X + X + X + X + 1

  The i bit in GEN is set if X^i is a summand of G(X) except X^32.  */

# define GEN	(BIT (26) | BIT (23) | BIT (22) | BIT (16) | BIT (12) \
                 | BIT (11) | BIT (10) | BIT (8) | BIT (7) | BIT (5) \
                 | BIT (4) | BIT (2) | BIT (1) | BIT (0))

अटल अचिन्हित पूर्णांक r[8];

अटल व्योम
fill_r (व्योम)
{
  r[0] = GEN;
  क्रम (पूर्णांक i = 1; i < 8; i++)
    r[i] = (r[i - 1] << 1) ^ ((r[i - 1] & SBIT) ? GEN : 0);
}

अटल अचिन्हित पूर्णांक
crc_remainder (पूर्णांक m)
{
  अचिन्हित पूर्णांक rem = 0;

  क्रम (पूर्णांक i = 0; i < 8; i++)
    अगर (BIT (i) & m)
      rem ^= r[i];

  वापस rem & 0xFFFFFFFF;	/* Make it run on 64-bit machine.  */
}

पूर्णांक
मुख्य (व्योम)
{
  अटल अचिन्हित पूर्णांक crctab[8][256];

  fill_r ();

  क्रम (पूर्णांक i = 0; i < 256; i++)
    crctab[0][i] = crc_remainder (i);

  /* CRC(0x11 0x22 0x33 0x44)
     is equal to
     CRC(0x11 0x00 0x00 0x00) XOR CRC(0x22 0x00 0x00) XOR
     CRC(0x33 0x00) XOR CRC(0x44)
     We precompute the CRC values for the offset values into
     separate CRC tables. We can then use them to speed up
     CRC calculation by processing multiple bytes at the time. */
  क्रम (पूर्णांक i = 0; i < 256; i++)
    {
      अचिन्हित पूर्णांक crc = 0;

      crc = (crc << 8) ^ crctab[0][((crc >> 24) ^ i) & 0xFF];
      क्रम (पूर्णांक offset = 1; offset < 8; offset++)
        {
          crc = (crc << 8) ^ crctab[0][((crc >> 24) ^ 0) & 0xFF];
          crctab[offset][i] = crc & 0xFFFFFFFF;
        }
    }

  म_लिखो ("#include <config.h>\n");
  म_लिखो ("#include \"cksum_crc.h\"\n");
  म_लिखो ("\n");
  म_लिखो ("uint_fast32_t const crctab[8][256] = {\n");
  क्रम (पूर्णांक y = 0; y < 8; y++)
    {
      म_लिखो ("{\n  0x%08x", crctab[y][0]);
      क्रम (पूर्णांक i = 0; i < 51; i++)
        {
          म_लिखो (",\n  0x%08x, 0x%08x, 0x%08x, 0x%08x, 0x%08x",
                  crctab[y][i * 5 + 1], crctab[y][i * 5 + 2],
                  crctab[y][i * 5 + 3], crctab[y][i * 5 + 4],
                  crctab[y][i * 5 + 5]);
        }
        म_लिखो ("\n},\n");
    }
  म_लिखो ("};\n");
}

#अन्यथा /* !CRCTAB */

# include "cksum_crc.h"
# include <sys/types.h>
# include <endian.h>
# include "system.h"

# ifdef USE_VMULL_CRC32
#  include <sys/auxv.h>
#  include <asm/hwcap.h>
# endif

# include "crc.h"
# include "cpu-supports.h"

/* Number of bytes to read at once.  */
# define BUFLEN (1 << 16)

प्रकार bool (*cksum_fp_t) (खाता *, uint_fast32_t *, intmax_t *);

अटल cksum_fp_t
pclmul_supported (व्योम)
{
# अगर USE_PCLMUL_CRC32 || GL_CRC_X86_64_PCLMUL
  bool pclmul_enabled = (cpu_supports ("avx")
                         && cpu_supports ("pclmul"));
  अगर (cksum_debug)
    error (0, 0, "%s",
           (pclmul_enabled
            ? _("using pclmul hardware support")
            : _("pclmul support not detected")));
#  अगर USE_PCLMUL_CRC32
  अगर (pclmul_enabled)
    वापस cksum_pclmul;
#  endif
# endif

  वापस शून्य;
}

अटल cksum_fp_t
avx2_supported (व्योम)
{
  /* AVX512 processors will not set vpclmulqdq unless they support
     the avx512 version, but it implies that the avx2 version
     is supported  */
# अगर USE_AVX2_CRC32
  bool avx2_enabled = (cpu_supports ("avx2")
                       && cpu_supports ("vpclmulqdq"));
  अगर (cksum_debug)
    error (0, 0, "%s",
           (avx2_enabled
            ? _("using avx2 hardware support")
            : _("avx2 support not detected")));
  अगर (avx2_enabled)
    वापस cksum_avx2;
# endif

  वापस शून्य;
}

अटल cksum_fp_t
avx512_supported (व्योम)
{
  /* vpclmulqdq for multiplication
     mavx512f for most of the avx512 functions we're using
     mavx512bw for byte swapping  */
# अगर USE_AVX512_CRC32
  bool avx512_enabled = (cpu_supports ("avx512f")
                         && cpu_supports ("avx512bw")
                         && cpu_supports ("vpclmulqdq"));

  अगर (cksum_debug)
    error (0, 0, "%s",
           (avx512_enabled
            ? _("using avx512 hardware support")
            : _("avx512 support not detected")));
  अगर (avx512_enabled)
    वापस cksum_avx512;
# endif

  वापस शून्य;
}

अटल cksum_fp_t
vmull_supported (व्योम)
{
  /* vmull for multiplication  */
# अगर USE_VMULL_CRC32
  bool vmull_enabled = (cpu_may_support ("pmull")
                        && (getauxval (AT_HWCAP) & HWCAP_PMULL) > 0);
  अगर (cksum_debug)
    error (0, 0, "%s",
           (vmull_enabled
            ? _("using vmull hardware support")
            : _("vmull support not detected")));
  अगर (vmull_enabled)
    वापस cksum_vmull;
# endif

  वापस शून्य;
}

अटल bool
cksum_slice8 (खाता *fp, uint_fast32_t *crc_out, intmax_t *लंबाई_out)
{
  uint32_t buf[BUFLEN / माप (uint32_t)];
  uint_fast32_t crc = 0;
  intmax_t लंबाई = 0;
  माप_प्रकार bytes_read;

  अगर (!fp || !crc_out || !लंबाई_out)
    वापस false;

  जबतक ((bytes_read = ख_पढ़ो (buf, 1, BUFLEN, fp)) > 0)
    {
      uint32_t *datap;

      अगर (ckd_add (&लंबाई, लंबाई, bytes_read))
        {
          त्रुटि_सं = EOVERFLOW;
          वापस false;
        }

      /* Process multiples of 8 bytes */
      datap = buf;
      जबतक (bytes_read >= 8)
        {
          uint32_t first = *datap++, second = *datap++;
          crc ^= htobe32 (first);
          second = htobe32 (second);
          crc = (crctab[7][(crc >> 24) & 0xFF]
                 ^ crctab[6][(crc >> 16) & 0xFF]
                 ^ crctab[5][(crc >> 8) & 0xFF]
                 ^ crctab[4][(crc) & 0xFF]
                 ^ crctab[3][(second >> 24) & 0xFF]
                 ^ crctab[2][(second >> 16) & 0xFF]
                 ^ crctab[1][(second >> 8) & 0xFF]
                 ^ crctab[0][(second) & 0xFF]);
          bytes_read -= 8;
        }

      /* And finish up last 0-7 bytes in a byte by byte fashion */
      अचिन्हित अक्षर *cp = (अचिन्हित अक्षर *)datap;
      जबतक (bytes_read--)
        crc = (crc << 8) ^ crctab[0][((crc >> 24) ^ *cp++) & 0xFF];
      अगर (ख_पूर्ण (fp))
        अवरोध;
    }

  *crc_out = crc;
  *लंबाई_out = लंबाई;

  वापस !ख_त्रुटि (fp);
}

/* Calculate the checksum and length in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

पूर्णांक
crc_योग_stream (खाता *stream, व्योम *resstream, intmax_t *लंबाई)
{
  intmax_t योग_bytes = 0;
  uint_fast32_t crc = 0;

  अटल cksum_fp_t cksum_fp;
  अगर (! cksum_fp)
    cksum_fp = avx512_supported ();
  अगर (! cksum_fp)
    cksum_fp = avx2_supported ();
  अगर (! cksum_fp)
    cksum_fp = pclmul_supported ();
  अगर (! cksum_fp)
    cksum_fp = vmull_supported ();
  अगर (! cksum_fp)
    cksum_fp = cksum_slice8;

  अगर (! cksum_fp (stream, &crc, &योग_bytes))
    वापस -1;

  *लंबाई = योग_bytes;

  क्रम (; योग_bytes; योग_bytes >>= 8)
    crc = (crc << 8) ^ crctab[0][((crc >> 24) ^ योग_bytes) & 0xFF];
  crc = ~crc & 0xFFFFFFFF;

  अचिन्हित पूर्णांक crc_out = crc;
  स_नकल (resstream, &crc_out, माप crc_out);

  वापस 0;
}

/* Calculate the crc32b checksum and length in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

पूर्णांक
crc32b_योग_stream (खाता *stream, व्योम *resstream, intmax_t *reslen)
{
  uint32_t buf[BUFLEN / माप (uint32_t)];
  uint32_t crc = 0;
  intmax_t len = 0;
  माप_प्रकार bytes_read;

  अगर (!stream || !resstream || !reslen)
    वापस -1;

# अगर GL_CRC_X86_64_PCLMUL
  अगर (cksum_debug)
    (व्योम) pclmul_supported ();
# endif

  जबतक ((bytes_read = ख_पढ़ो (buf, 1, BUFLEN, stream)) > 0)
    {
      अगर (ckd_add (&len, len, bytes_read))
        {
          त्रुटि_सं = EOVERFLOW;
          वापस -1;
        }

      crc = crc32_update (crc, (अक्षर स्थिर *)buf, bytes_read);

      अगर (ख_पूर्ण (stream))
        अवरोध;
    }

  अचिन्हित पूर्णांक crc_out = crc;
  स_नकल (resstream, &crc_out, माप crc_out);

  *reslen = len;

  वापस ख_त्रुटि (stream) ? -1 : 0;
}

/* Print the checksum and size to stdout.
   If ARGS is true, also print the FILE name.  */

व्योम
output_crc (अक्षर स्थिर *file, MAYBE_UNUSED पूर्णांक द्विआधारी_file,
            व्योम स्थिर *digest, bool raw, MAYBE_UNUSED bool tagged,
            अचिन्हित अक्षर delim, bool args, intmax_t लंबाई)
{
  अगर (raw)
    {
      /* Output in network byte order (big endian).  */
      uint32_t out_int = htobe32 (*(uint32_t *)digest);
      ख_डालो (&out_int, 1, 32/8, मानक_निकास);
      वापस;
    }

  म_लिखो ("%u %jd", *(अचिन्हित पूर्णांक *)digest, लंबाई);
  अगर (args)
    म_लिखो (" %s", file);
  अक्षर_दो (delim);
}

#पूर्ण_अगर /* !CRCTAB */
