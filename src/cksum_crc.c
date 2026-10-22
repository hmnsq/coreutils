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

സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ r[8];

സ്ഥിതം ശൂന്യം
fill_r (ശൂന്യം)
{
  r[0] = GEN;
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 1; i < 8; i++)
    r[i] = (r[i - 1] << 1) ^ ((r[i - 1] & SBIT) ? GEN : 0);
}

സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ
crc_remainder (പൂർണ്ണസംഖ്യ m)
{
  unsigned പൂർണ്ണസംഖ്യ rem = 0;

  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < 8; i++)
    എങ്കിൽ (BIT (i) & m)
      rem ^= r[i];

  തിരികെ rem & 0xFFFFFFFF;	/* Make it run on 64-bit machine.  */
}

പൂർണ്ണസംഖ്യ
പ്രധാന (ശൂന്യം)
{
  സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ crctab[8][256];

  fill_r ();

  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < 256; i++)
    crctab[0][i] = crc_remainder (i);

  /* CRC(0x11 0x22 0x33 0x44)
     is equal to
     CRC(0x11 0x00 0x00 0x00) XOR CRC(0x22 0x00 0x00) XOR
     CRC(0x33 0x00) XOR CRC(0x44)
     We precompute the CRC values for the offset values into
     separate CRC tables. We can then use them to speed up
     CRC calculation by processing multiple bytes at the time. */
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < 256; i++)
    {
      unsigned പൂർണ്ണസംഖ്യ crc = 0;

      crc = (crc << 8) ^ crctab[0][((crc >> 24) ^ i) & 0xFF];
      വേണ്ടി (പൂർണ്ണസംഖ്യ offset = 1; offset < 8; offset++)
        {
          crc = (crc << 8) ^ crctab[0][((crc >> 24) ^ 0) & 0xFF];
          crctab[offset][i] = crc & 0xFFFFFFFF;
        }
    }

  printf ("#include <config.h>\n");
  printf ("#include \"cksum_crc.h\"\n");
  printf ("\n");
  printf ("uint_fast32_t const crctab[8][256] = {\n");
  വേണ്ടി (പൂർണ്ണസംഖ്യ y = 0; y < 8; y++)
    {
      printf ("{\n  0x%08x", crctab[y][0]);
      വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < 51; i++)
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

#അല്ലെങ്കിൽ /* !CRCTAB */

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

തരനിർവചനം bool (*cksum_fp_t) (FILE *, uint_fast32_t *, intmax_t *);

സ്ഥിതം cksum_fp_t
pclmul_supported (ശൂന്യം)
{
# എങ്കിൽ USE_PCLMUL_CRC32 || GL_CRC_X86_64_PCLMUL
  bool pclmul_enabled = (cpu_supports ("avx")
                         && cpu_supports ("pclmul"));
  എങ്കിൽ (cksum_debug)
    error (0, 0, "%s",
           (pclmul_enabled
            ? _("using pclmul hardware support")
            : _("pclmul support not detected")));
#  എങ്കിൽ USE_PCLMUL_CRC32
  എങ്കിൽ (pclmul_enabled)
    തിരികെ cksum_pclmul;
#  endif
# endif

  തിരികെ ശൂന്യ;
}

സ്ഥിതം cksum_fp_t
avx2_supported (ശൂന്യം)
{
  /* AVX512 processors will not set vpclmulqdq unless they support
     the avx512 version, but it implies that the avx2 version
     is supported  */
# എങ്കിൽ USE_AVX2_CRC32
  bool avx2_enabled = (cpu_supports ("avx2")
                       && cpu_supports ("vpclmulqdq"));
  എങ്കിൽ (cksum_debug)
    error (0, 0, "%s",
           (avx2_enabled
            ? _("using avx2 hardware support")
            : _("avx2 support not detected")));
  എങ്കിൽ (avx2_enabled)
    തിരികെ cksum_avx2;
# endif

  തിരികെ ശൂന്യ;
}

സ്ഥിതം cksum_fp_t
avx512_supported (ശൂന്യം)
{
  /* vpclmulqdq for multiplication
     mavx512f for most of the avx512 functions we're using
     mavx512bw for byte swapping  */
# എങ്കിൽ USE_AVX512_CRC32
  bool avx512_enabled = (cpu_supports ("avx512f")
                         && cpu_supports ("avx512bw")
                         && cpu_supports ("vpclmulqdq"));

  എങ്കിൽ (cksum_debug)
    error (0, 0, "%s",
           (avx512_enabled
            ? _("using avx512 hardware support")
            : _("avx512 support not detected")));
  എങ്കിൽ (avx512_enabled)
    തിരികെ cksum_avx512;
# endif

  തിരികെ ശൂന്യ;
}

സ്ഥിതം cksum_fp_t
vmull_supported (ശൂന്യം)
{
  /* vmull for multiplication  */
# എങ്കിൽ USE_VMULL_CRC32
  bool vmull_enabled = (cpu_may_support ("pmull")
                        && (getauxval (AT_HWCAP) & HWCAP_PMULL) > 0);
  എങ്കിൽ (cksum_debug)
    error (0, 0, "%s",
           (vmull_enabled
            ? _("using vmull hardware support")
            : _("vmull support not detected")));
  എങ്കിൽ (vmull_enabled)
    തിരികെ cksum_vmull;
# endif

  തിരികെ ശൂന്യ;
}

സ്ഥിതം bool
cksum_slice8 (FILE *fp, uint_fast32_t *crc_out, intmax_t *length_out)
{
  uint32_t buf[BUFLEN / വലുപ്പം (uint32_t)];
  uint_fast32_t crc = 0;
  intmax_t length = 0;
  size_t bytes_read;

  എങ്കിൽ (!fp || !crc_out || !length_out)
    തിരികെ false;

  ആകുമ്പോൾ ((bytes_read = fread (buf, 1, BUFLEN, fp)) > 0)
    {
      uint32_t *datap;

      എങ്കിൽ (ckd_add (&length, length, bytes_read))
        {
          errno = EOVERFLOW;
          തിരികെ false;
        }

      /* Process multiples of 8 bytes */
      datap = buf;
      ആകുമ്പോൾ (bytes_read >= 8)
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
      unsigned അക്ഷരം *cp = (unsigned അക്ഷരം *)datap;
      ആകുമ്പോൾ (bytes_read--)
        crc = (crc << 8) ^ crctab[0][((crc >> 24) ^ *cp++) & 0xFF];
      എങ്കിൽ (feof (fp))
        നിർത്തുക;
    }

  *crc_out = crc;
  *length_out = length;

  തിരികെ !ferror (fp);
}

/* Calculate the checksum and length in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

പൂർണ്ണസംഖ്യ
crc_sum_stream (FILE *stream, ശൂന്യം *resstream, intmax_t *length)
{
  intmax_t total_bytes = 0;
  uint_fast32_t crc = 0;

  സ്ഥിതം cksum_fp_t cksum_fp;
  എങ്കിൽ (! cksum_fp)
    cksum_fp = avx512_supported ();
  എങ്കിൽ (! cksum_fp)
    cksum_fp = avx2_supported ();
  എങ്കിൽ (! cksum_fp)
    cksum_fp = pclmul_supported ();
  എങ്കിൽ (! cksum_fp)
    cksum_fp = vmull_supported ();
  എങ്കിൽ (! cksum_fp)
    cksum_fp = cksum_slice8;

  എങ്കിൽ (! cksum_fp (stream, &crc, &total_bytes))
    തിരികെ -1;

  *length = total_bytes;

  വേണ്ടി (; total_bytes; total_bytes >>= 8)
    crc = (crc << 8) ^ crctab[0][((crc >> 24) ^ total_bytes) & 0xFF];
  crc = ~crc & 0xFFFFFFFF;

  unsigned പൂർണ്ണസംഖ്യ crc_out = crc;
  memcpy (resstream, &crc_out, വലുപ്പം crc_out);

  തിരികെ 0;
}

/* Calculate the crc32b checksum and length in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

പൂർണ്ണസംഖ്യ
crc32b_sum_stream (FILE *stream, ശൂന്യം *resstream, intmax_t *reslen)
{
  uint32_t buf[BUFLEN / വലുപ്പം (uint32_t)];
  uint32_t crc = 0;
  intmax_t len = 0;
  size_t bytes_read;

  എങ്കിൽ (!stream || !resstream || !reslen)
    തിരികെ -1;

# എങ്കിൽ GL_CRC_X86_64_PCLMUL
  എങ്കിൽ (cksum_debug)
    (ശൂന്യം) pclmul_supported ();
# endif

  ആകുമ്പോൾ ((bytes_read = fread (buf, 1, BUFLEN, stream)) > 0)
    {
      എങ്കിൽ (ckd_add (&len, len, bytes_read))
        {
          errno = EOVERFLOW;
          തിരികെ -1;
        }

      crc = crc32_update (crc, (അക്ഷരം സ്ഥിരം *)buf, bytes_read);

      എങ്കിൽ (feof (stream))
        നിർത്തുക;
    }

  unsigned പൂർണ്ണസംഖ്യ crc_out = crc;
  memcpy (resstream, &crc_out, വലുപ്പം crc_out);

  *reslen = len;

  തിരികെ ferror (stream) ? -1 : 0;
}

/* Print the checksum and size to stdout.
   If ARGS is true, also print the FILE name.  */

ശൂന്യം
output_crc (അക്ഷരം സ്ഥിരം *file, MAYBE_UNUSED പൂർണ്ണസംഖ്യ ദ്വിമാന_file,
            ശൂന്യം സ്ഥിരം *digest, bool raw, MAYBE_UNUSED bool tagged,
            unsigned അക്ഷരം delim, bool args, intmax_t length)
{
  എങ്കിൽ (raw)
    {
      /* Output in network byte order (big endian).  */
      uint32_t out_int = htobe32 (*(uint32_t *)digest);
      fwrite (&out_int, 1, 32/8, stdout);
      തിരികെ;
    }

  printf ("%u %jd", *(unsigned പൂർണ്ണസംഖ്യ *)digest, length);
  എങ്കിൽ (args)
    printf (" %s", file);
  putchar (delim);
}

#endif /* !CRCTAB */
