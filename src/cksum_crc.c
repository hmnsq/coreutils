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

स्थिर unsigned पूर्णाङ्क r[8];

स्थिर शून्य
fill_r (शून्य)
{
  r[0] = GEN;
  प्रति (पूर्णाङ्क i = 1; i < 8; i++)
    r[i] = (r[i - 1] << 1) ^ ((r[i - 1] & SBIT) ? GEN : 0);
}

स्थिर unsigned पूर्णाङ्क
crc_remainder (पूर्णाङ्क m)
{
  unsigned पूर्णाङ्क rem = 0;

  प्रति (पूर्णाङ्क i = 0; i < 8; i++)
    जइ (BIT (i) & m)
      rem ^= r[i];

  प्रत्यागम rem & 0xFFFFFFFF;	/* Make it run on 64-bit machine.  */
}

पूर्णाङ्क
main (शून्य)
{
  स्थिर unsigned पूर्णाङ्क crctab[8][256];

  fill_r ();

  प्रति (पूर्णाङ्क i = 0; i < 256; i++)
    crctab[0][i] = crc_remainder (i);

  /* CRC(0x11 0x22 0x33 0x44)
     is equal to
     CRC(0x11 0x00 0x00 0x00) XOR CRC(0x22 0x00 0x00) XOR
     CRC(0x33 0x00) XOR CRC(0x44)
     We precompute the CRC values for the offset values into
     separate CRC tables. We can then use them to speed up
     CRC calculation by processing multiple bytes at the time. */
  प्रति (पूर्णाङ्क i = 0; i < 256; i++)
    {
      unsigned पूर्णाङ्क crc = 0;

      crc = (crc << 8) ^ crctab[0][((crc >> 24) ^ i) & 0xFF];
      प्रति (पूर्णाङ्क offset = 1; offset < 8; offset++)
        {
          crc = (crc << 8) ^ crctab[0][((crc >> 24) ^ 0) & 0xFF];
          crctab[offset][i] = crc & 0xFFFFFFFF;
        }
    }

  printf ("#include <config.h>\n");
  printf ("#include \"cksum_crc.h\"\n");
  printf ("\n");
  printf ("uint_fast32_t const crctab[8][256] = {\n");
  प्रति (पूर्णाङ्क y = 0; y < 8; y++)
    {
      printf ("{\n  0x%08x", crctab[y][0]);
      प्रति (पूर्णाङ्क i = 0; i < 51; i++)
        {
          printf (",\n  0x%08x, 0x%08x, 0x%08x, 0x%08x, 0x%08x",
                  crctab[y][i * 5 + 1], crctab[y][i * 5 + 2],
                  crctab[y][i * 5 + 3], crctab[y][i * 5 + 4],
                  crctab[y][i * 5 + 5]);
        }
        printf ("\n},\n");
    }
  printf ("};\n");
}

#अण्णहा /* !CRCTAB */

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

प्रकारनिर्देश bool (*cksum_fp_t) (FILE *, uint_fast32_t *, intmax_t *);

स्थिर cksum_fp_t
pclmul_supported (शून्य)
{
# जइ USE_PCLMUL_CRC32 || GL_CRC_X86_64_PCLMUL
  bool pclmul_enabled = (cpu_supports ("avx")
                         && cpu_supports ("pclmul"));
  जइ (cksum_debug)
    error (0, 0, "%s",
           (pclmul_enabled
            ? _("using pclmul hardware support")
            : _("pclmul support not detected")));
#  जइ USE_PCLMUL_CRC32
  जइ (pclmul_enabled)
    प्रत्यागम cksum_pclmul;
#  endif
# endif

  प्रत्यागम रिक्त;
}

स्थिर cksum_fp_t
avx2_supported (शून्य)
{
  /* AVX512 processors will not set vpclmulqdq unless they support
     the avx512 version, but it implies that the avx2 version
     is supported  */
# जइ USE_AVX2_CRC32
  bool avx2_enabled = (cpu_supports ("avx2")
                       && cpu_supports ("vpclmulqdq"));
  जइ (cksum_debug)
    error (0, 0, "%s",
           (avx2_enabled
            ? _("using avx2 hardware support")
            : _("avx2 support not detected")));
  जइ (avx2_enabled)
    प्रत्यागम cksum_avx2;
# endif

  प्रत्यागम रिक्त;
}

स्थिर cksum_fp_t
avx512_supported (शून्य)
{
  /* vpclmulqdq for multiplication
     mavx512f for most of the avx512 functions we're using
     mavx512bw for byte swapping  */
# जइ USE_AVX512_CRC32
  bool avx512_enabled = (cpu_supports ("avx512f")
                         && cpu_supports ("avx512bw")
                         && cpu_supports ("vpclmulqdq"));

  जइ (cksum_debug)
    error (0, 0, "%s",
           (avx512_enabled
            ? _("using avx512 hardware support")
            : _("avx512 support not detected")));
  जइ (avx512_enabled)
    प्रत्यागम cksum_avx512;
# endif

  प्रत्यागम रिक्त;
}

स्थिर cksum_fp_t
vmull_supported (शून्य)
{
  /* vmull for multiplication  */
# जइ USE_VMULL_CRC32
  bool vmull_enabled = (cpu_may_support ("pmull")
                        && (getauxval (AT_HWCAP) & HWCAP_PMULL) > 0);
  जइ (cksum_debug)
    error (0, 0, "%s",
           (vmull_enabled
            ? _("using vmull hardware support")
            : _("vmull support not detected")));
  जइ (vmull_enabled)
    प्रत्यागम cksum_vmull;
# endif

  प्रत्यागम रिक्त;
}

स्थिर bool
cksum_slice8 (FILE *fp, uint_fast32_t *crc_out, intmax_t *length_out)
{
  uint32_t buf[BUFLEN / परिमाण (uint32_t)];
  uint_fast32_t crc = 0;
  intmax_t length = 0;
  size_t bytes_read;

  जइ (!fp || !crc_out || !length_out)
    प्रत्यागम false;

  जाव ((bytes_read = fread (buf, 1, BUFLEN, fp)) > 0)
    {
      uint32_t *datap;

      जइ (ckd_add (&length, length, bytes_read))
        {
          errno = EOVERFLOW;
          प्रत्यागम false;
        }

      /* Process multiples of 8 bytes */
      datap = buf;
      जाव (bytes_read >= 8)
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
      unsigned वर्ण *cp = (unsigned वर्ण *)datap;
      जाव (bytes_read--)
        crc = (crc << 8) ^ crctab[0][((crc >> 24) ^ *cp++) & 0xFF];
      जइ (feof (fp))
        भङ्ग;
    }

  *crc_out = crc;
  *length_out = length;

  प्रत्यागम !ferror (fp);
}

/* Calculate the checksum and length in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

पूर्णाङ्क
crc_sum_stream (FILE *stream, शून्य *resstream, intmax_t *length)
{
  intmax_t total_bytes = 0;
  uint_fast32_t crc = 0;

  स्थिर cksum_fp_t cksum_fp;
  जइ (! cksum_fp)
    cksum_fp = avx512_supported ();
  जइ (! cksum_fp)
    cksum_fp = avx2_supported ();
  जइ (! cksum_fp)
    cksum_fp = pclmul_supported ();
  जइ (! cksum_fp)
    cksum_fp = vmull_supported ();
  जइ (! cksum_fp)
    cksum_fp = cksum_slice8;

  जइ (! cksum_fp (stream, &crc, &total_bytes))
    प्रत्यागम -1;

  *length = total_bytes;

  प्रति (; total_bytes; total_bytes >>= 8)
    crc = (crc << 8) ^ crctab[0][((crc >> 24) ^ total_bytes) & 0xFF];
  crc = ~crc & 0xFFFFFFFF;

  unsigned पूर्णाङ्क crc_out = crc;
  memcpy (resstream, &crc_out, परिमाण crc_out);

  प्रत्यागम 0;
}

/* Calculate the crc32b checksum and length in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

पूर्णाङ्क
crc32b_sum_stream (FILE *stream, शून्य *resstream, intmax_t *reslen)
{
  uint32_t buf[BUFLEN / परिमाण (uint32_t)];
  uint32_t crc = 0;
  intmax_t len = 0;
  size_t bytes_read;

  जइ (!stream || !resstream || !reslen)
    प्रत्यागम -1;

# जइ GL_CRC_X86_64_PCLMUL
  जइ (cksum_debug)
    (शून्य) pclmul_supported ();
# endif

  जाव ((bytes_read = fread (buf, 1, BUFLEN, stream)) > 0)
    {
      जइ (ckd_add (&len, len, bytes_read))
        {
          errno = EOVERFLOW;
          प्रत्यागम -1;
        }

      crc = crc32_update (crc, (वर्ण ध्रुव *)buf, bytes_read);

      जइ (feof (stream))
        भङ्ग;
    }

  unsigned पूर्णाङ्क crc_out = crc;
  memcpy (resstream, &crc_out, परिमाण crc_out);

  *reslen = len;

  प्रत्यागम ferror (stream) ? -1 : 0;
}

/* Print the checksum and size to stdout.
   If ARGS is true, also print the FILE name.  */

शून्य
output_crc (वर्ण ध्रुव *file, MAYBE_UNUSED पूर्णाङ्क द्विमान_file,
            शून्य ध्रुव *digest, bool raw, MAYBE_UNUSED bool tagged,
            unsigned वर्ण delim, bool args, intmax_t length)
{
  जइ (raw)
    {
      /* Output in network byte order (big endian).  */
      uint32_t out_int = htobe32 (*(uint32_t *)digest);
      fwrite (&out_int, 1, 32/8, stdout);
      प्रत्यागम;
    }

  printf ("%u %jd", *(unsigned पूर्णाङ्क *)digest, length);
  जइ (args)
    printf (" %s", file);
  putchar (delim);
}

#endif /* !CRCTAB */
