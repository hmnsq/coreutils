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
#ifndef CRCTAB
# include <config.h>
#endif

#ifdef CRCTAB

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

स्थिर अचिन्हित पूर्णांक r[8];

स्थिर शून्य
fill_r (शून्य)
{
  r[0] = GEN;
  साठी (पूर्णांक i = 1; i < 8; i++)
    r[i] = (r[i - 1] << 1) ^ ((r[i - 1] & SBIT) ? GEN : 0);
}

स्थिर अचिन्हित पूर्णांक
crc_remainder (पूर्णांक m)
{
  अचिन्हित पूर्णांक rem = 0;

  साठी (पूर्णांक i = 0; i < 8; i++)
    जर (BIT (i) & m)
      rem ^= r[i];

  परत rem & 0xFFFFFFFF;	/* Make it run on 64-bit machine.  */
}

पूर्णांक
main (शून्य)
{
  स्थिर अचिन्हित पूर्णांक crctab[8][256];

  fill_r ();

  साठी (पूर्णांक i = 0; i < 256; i++)
    crctab[0][i] = crc_remainder (i);

  /* CRC(0x11 0x22 0x33 0x44)
     is equal to
     CRC(0x11 0x00 0x00 0x00) XOR CRC(0x22 0x00 0x00) XOR
     CRC(0x33 0x00) XOR CRC(0x44)
     We precompute the CRC values for the offset values into
     separate CRC tables. We can then use them to speed up
     CRC calculation by processing multiple bytes at the time. */
  साठी (पूर्णांक i = 0; i < 256; i++)
    {
      अचिन्हित पूर्णांक crc = 0;

      crc = (crc << 8) ^ crctab[0][((crc >> 24) ^ i) & 0xFF];
      साठी (पूर्णांक offset = 1; offset < 8; offset++)
        {
          crc = (crc << 8) ^ crctab[0][((crc >> 24) ^ 0) & 0xFF];
          crctab[offset][i] = crc & 0xFFFFFFFF;
        }
    }

  म_लिखो ("#include <config.h>\n");
  म_लिखो ("#include \"cksum_crc.h\"\n");
  म_लिखो ("\n");
  म_लिखो ("uint_fast32_t const crctab[8][256] = {\n");
  साठी (पूर्णांक y = 0; y < 8; y++)
    {
      म_लिखो ("{\n  0x%08x", crctab[y][0]);
      साठी (पूर्णांक i = 0; i < 51; i++)
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

प्रकारव्याख्या bool (*cksum_fp_t) (खाता *, uint_fast32_t *, intmax_t *);

स्थिर cksum_fp_t
pclmul_supported (शून्य)
{
# जर USE_PCLMUL_CRC32 || GL_CRC_X86_64_PCLMUL
  bool pclmul_enabled = (cpu_supports ("avx")
                         && cpu_supports ("pclmul"));
  जर (cksum_debug)
    error (0, 0, "%s",
           (pclmul_enabled
            ? _("using pclmul hardware support")
            : _("pclmul support not detected")));
#  जर USE_PCLMUL_CRC32
  जर (pclmul_enabled)
    परत cksum_pclmul;
#  endif
# endif

  परत रिक्त;
}

स्थिर cksum_fp_t
avx2_supported (शून्य)
{
  /* AVX512 processors will not set vpclmulqdq unless they support
     the avx512 version, but it implies that the avx2 version
     is supported  */
# जर USE_AVX2_CRC32
  bool avx2_enabled = (cpu_supports ("avx2")
                       && cpu_supports ("vpclmulqdq"));
  जर (cksum_debug)
    error (0, 0, "%s",
           (avx2_enabled
            ? _("using avx2 hardware support")
            : _("avx2 support not detected")));
  जर (avx2_enabled)
    परत cksum_avx2;
# endif

  परत रिक्त;
}

स्थिर cksum_fp_t
avx512_supported (शून्य)
{
  /* vpclmulqdq for multiplication
     mavx512f for most of the avx512 functions we're using
     mavx512bw for byte swapping  */
# जर USE_AVX512_CRC32
  bool avx512_enabled = (cpu_supports ("avx512f")
                         && cpu_supports ("avx512bw")
                         && cpu_supports ("vpclmulqdq"));

  जर (cksum_debug)
    error (0, 0, "%s",
           (avx512_enabled
            ? _("using avx512 hardware support")
            : _("avx512 support not detected")));
  जर (avx512_enabled)
    परत cksum_avx512;
# endif

  परत रिक्त;
}

स्थिर cksum_fp_t
vmull_supported (शून्य)
{
  /* vmull for multiplication  */
# जर USE_VMULL_CRC32
  bool vmull_enabled = (cpu_may_support ("pmull")
                        && (getauxval (AT_HWCAP) & HWCAP_PMULL) > 0);
  जर (cksum_debug)
    error (0, 0, "%s",
           (vmull_enabled
            ? _("using vmull hardware support")
            : _("vmull support not detected")));
  जर (vmull_enabled)
    परत cksum_vmull;
# endif

  परत रिक्त;
}

स्थिर bool
cksum_slice8 (खाता *fp, uint_fast32_t *crc_out, intmax_t *length_out)
{
  uint32_t buf[BUFLEN / आकार (uint32_t)];
  uint_fast32_t crc = 0;
  intmax_t length = 0;
  माप_प्रकार bytes_read;

  जर (!fp || !crc_out || !length_out)
    परत false;

  पर्यंत ((bytes_read = ख_पढ़ो (buf, 1, BUFLEN, fp)) > 0)
    {
      uint32_t *datap;

      जर (ckd_add (&length, length, bytes_read))
        {
          त्रुटि_सं = EOVERFLOW;
          परत false;
        }

      /* Process multiples of 8 bytes */
      datap = buf;
      पर्यंत (bytes_read >= 8)
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
      पर्यंत (bytes_read--)
        crc = (crc << 8) ^ crctab[0][((crc >> 24) ^ *cp++) & 0xFF];
      जर (ख_पूर्ण (fp))
        थांबा;
    }

  *crc_out = crc;
  *length_out = length;

  परत !ख_त्रुटि (fp);
}

/* Calculate the checksum and length in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

पूर्णांक
crc_sum_stream (खाता *stream, शून्य *resstream, intmax_t *length)
{
  intmax_t total_bytes = 0;
  uint_fast32_t crc = 0;

  स्थिर cksum_fp_t cksum_fp;
  जर (! cksum_fp)
    cksum_fp = avx512_supported ();
  जर (! cksum_fp)
    cksum_fp = avx2_supported ();
  जर (! cksum_fp)
    cksum_fp = pclmul_supported ();
  जर (! cksum_fp)
    cksum_fp = vmull_supported ();
  जर (! cksum_fp)
    cksum_fp = cksum_slice8;

  जर (! cksum_fp (stream, &crc, &total_bytes))
    परत -1;

  *length = total_bytes;

  साठी (; total_bytes; total_bytes >>= 8)
    crc = (crc << 8) ^ crctab[0][((crc >> 24) ^ total_bytes) & 0xFF];
  crc = ~crc & 0xFFFFFFFF;

  अचिन्हित पूर्णांक crc_out = crc;
  स_नकल (resstream, &crc_out, आकार crc_out);

  परत 0;
}

/* Calculate the crc32b checksum and length in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

पूर्णांक
crc32b_sum_stream (खाता *stream, शून्य *resstream, intmax_t *reslen)
{
  uint32_t buf[BUFLEN / आकार (uint32_t)];
  uint32_t crc = 0;
  intmax_t len = 0;
  माप_प्रकार bytes_read;

  जर (!stream || !resstream || !reslen)
    परत -1;

# जर GL_CRC_X86_64_PCLMUL
  जर (cksum_debug)
    (शून्य) pclmul_supported ();
# endif

  पर्यंत ((bytes_read = ख_पढ़ो (buf, 1, BUFLEN, stream)) > 0)
    {
      जर (ckd_add (&len, len, bytes_read))
        {
          त्रुटि_सं = EOVERFLOW;
          परत -1;
        }

      crc = crc32_update (crc, (अक्षर स्थिरांक *)buf, bytes_read);

      जर (ख_पूर्ण (stream))
        थांबा;
    }

  अचिन्हित पूर्णांक crc_out = crc;
  स_नकल (resstream, &crc_out, आकार crc_out);

  *reslen = len;

  परत ख_त्रुटि (stream) ? -1 : 0;
}

/* Print the checksum and size to stdout.
   If ARGS is true, also print the FILE name.  */

शून्य
output_crc (अक्षर स्थिरांक *file, MAYBE_UNUSED पूर्णांक द्विमान_file,
            शून्य स्थिरांक *digest, bool raw, MAYBE_UNUSED bool tagged,
            अचिन्हित अक्षर delim, bool args, intmax_t length)
{
  जर (raw)
    {
      /* Output in network byte order (big endian).  */
      uint32_t out_int = htobe32 (*(uint32_t *)digest);
      ख_डालो (&out_int, 1, 32/8, मानक_निकास);
      परत;
    }

  म_लिखो ("%u %jd", *(अचिन्हित पूर्णांक *)digest, length);
  जर (args)
    म_लिखो (" %s", file);
  अक्षर_दो (delim);
}

#endif /* !CRCTAB */
