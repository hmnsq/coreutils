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
#నిర్వచితం_కాకపోతే CRCTAB
# include <config.h>
#అయితే_ముగింపు

#నిర్వచితమైతే CRCTAB

# include <stdio.h>

# define బిట్టు_2(x)	(1u << (x))
# define SBIT	బిట్టు_2 (31)

/* The generating polynomial is

          32   26   23   22   16   12   11   10   8   7   5   4   2   1
    G(X)=X  + X  + X  + X  + X  + X  + X  + X  + X + X + X + X + X + X + 1

  The i bit in GEN is set if X^i is a summand of G(X) except X^32.  */

# define GEN	(బిట్టు_2 (26) | బిట్టు_2 (23) | బిట్టు_2 (22) | బిట్టు_2 (16) | బిట్టు_2 (12) \
                 | బిట్టు_2 (11) | బిట్టు_2 (10) | బిట్టు_2 (8) | బిట్టు_2 (7) | బిట్టు_2 (5) \
                 | బిట్టు_2 (4) | బిట్టు_2 (2) | బిట్టు_2 (1) | బిట్టు_2 (0))

స్థిర గుర్తులేని పూర్ణాంకం r[8];

స్థిర శూన్య
fill_r (శూన్య)
{
  r[0] = GEN;
  కొరకు (పూర్ణాంకం i = 1; i < 8; i++)
    r[i] = (r[i - 1] << 1) ^ ((r[i - 1] & SBIT) ? GEN : 0);
}

స్థిర గుర్తులేని పూర్ణాంకం
చక్రీయ_తనిఖీ_remainder (పూర్ణాంకం m)
{
  గుర్తులేని పూర్ణాంకం rem = 0;

  కొరకు (పూర్ణాంకం i = 0; i < 8; i++)
    ఒకవేళ (బిట్టు_2 (i) & m)
      rem ^= r[i];

  తిరిగి rem & 0xFFFFFFFF;	/* Make it run on 64-bit machine.  */
}

పూర్ణాంకం
ప్రధాన (శూన్య)
{
  స్థిర గుర్తులేని పూర్ణాంకం crctab[8][256];

  fill_r ();

  కొరకు (పూర్ణాంకం i = 0; i < 256; i++)
    crctab[0][i] = చక్రీయ_తనిఖీ_remainder (i);

  /* CRC(0x11 0x22 0x33 0x44)
     is equal to
     CRC(0x11 0x00 0x00 0x00) XOR CRC(0x22 0x00 0x00) XOR
     CRC(0x33 0x00) XOR CRC(0x44)
     We precompute the CRC values for the offset values into
     separate CRC tables. We can then use them to speed up
     CRC calculation by processing multiple bytes at the time. */
  కొరకు (పూర్ణాంకం i = 0; i < 256; i++)
    {
      గుర్తులేని పూర్ణాంకం చక్రీయ_తనిఖీ = 0;

      చక్రీయ_తనిఖీ = (చక్రీయ_తనిఖీ << 8) ^ crctab[0][((చక్రీయ_తనిఖీ >> 24) ^ i) & 0xFF];
      కొరకు (పూర్ణాంకం స్థానభ్రంశం = 1; స్థానభ్రంశం < 8; స్థానభ్రంశం++)
        {
          చక్రీయ_తనిఖీ = (చక్రీయ_తనిఖీ << 8) ^ crctab[0][((చక్రీయ_తనిఖీ >> 24) ^ 0) & 0xFF];
          crctab[స్థానభ్రంశం][i] = చక్రీయ_తనిఖీ & 0xFFFFFFFF;
        }
    }

  రాయి ("#include <config.h>\n");
  రాయి ("#include \"cksum_crc.h\"\n");
  రాయి ("\n");
  రాయి ("uint_fast32_t const crctab[8][256] = {\n");
  కొరకు (పూర్ణాంకం y = 0; y < 8; y++)
    {
      రాయి ("{\n  0x%08x", crctab[y][0]);
      కొరకు (పూర్ణాంకం i = 0; i < 51; i++)
        {
          రాయి (",\n  0x%08x, 0x%08x, 0x%08x, 0x%08x, 0x%08x",
                  crctab[y][i * 5 + 1], crctab[y][i * 5 + 2],
                  crctab[y][i * 5 + 3], crctab[y][i * 5 + 4],
                  crctab[y][i * 5 + 5]);
        }
        రాయి ("\n},\n");
    }
  రాయి ("};\n");
}

#లేకపోతే /* !CRCTAB */

# include "cksum_crc.h"
# include <sys/types.h>
# include <endian.h>
# include "system.h"

# ifdef USE_VMULL_చక్రీయ_తనిఖీ_32
#  include <sys/auxv.h>
#  include <asm/hwcap.h>
# endif

# include "crc.h"
# include "cpu-supports.h"

/* Number of bytes to read at once.  */
# define BUFLEN (1 << 16)

రకనిర్వచనం bool (*cksum_fp_t) (FILE *, uint_fast32_t *, intmax_t *);

స్థిర cksum_fp_t
pclmul_supported (శూన్య)
{
# ఒకవేళ USE_PCLMUL_చక్రీయ_తనిఖీ_32 || GL_చక్రీయ_తనిఖీ_X86_64_PCLMUL
  bool pclmul_enabled = (cpu_supports ("avx")
                         && cpu_supports ("pclmul"));
  ఒకవేళ (cksum_debug)
    error (0, 0, "%s",
           (pclmul_enabled
            ? _("using pclmul hardware support")
            : _("pclmul support not detected")));
#  ఒకవేళ USE_PCLMUL_చక్రీయ_తనిఖీ_32
  ఒకవేళ (pclmul_enabled)
    తిరిగి cksum_pclmul;
#  endif
# endif

  తిరిగి ఖాళీ;
}

స్థిర cksum_fp_t
avx2_supported (శూన్య)
{
  /* AVX512 processors will not set vpclmulqdq unless they support
     the avx512 version, but it implies that the avx2 version
     is supported  */
# ఒకవేళ USE_AVX2_చక్రీయ_తనిఖీ_32
  bool avx2_enabled = (cpu_supports ("avx2")
                       && cpu_supports ("vpclmulqdq"));
  ఒకవేళ (cksum_debug)
    error (0, 0, "%s",
           (avx2_enabled
            ? _("using avx2 hardware support")
            : _("avx2 support not detected")));
  ఒకవేళ (avx2_enabled)
    తిరిగి cksum_avx2;
# endif

  తిరిగి ఖాళీ;
}

స్థిర cksum_fp_t
avx512_supported (శూన్య)
{
  /* vpclmulqdq for multiplication
     mavx512f for most of the avx512 functions we're using
     mavx512bw for byte swapping  */
# ఒకవేళ USE_AVX512_చక్రీయ_తనిఖీ_32
  bool avx512_enabled = (cpu_supports ("avx512f")
                         && cpu_supports ("avx512bw")
                         && cpu_supports ("vpclmulqdq"));

  ఒకవేళ (cksum_debug)
    error (0, 0, "%s",
           (avx512_enabled
            ? _("using avx512 hardware support")
            : _("avx512 support not detected")));
  ఒకవేళ (avx512_enabled)
    తిరిగి cksum_avx512;
# endif

  తిరిగి ఖాళీ;
}

స్థిర cksum_fp_t
vmull_supported (శూన్య)
{
  /* vmull for multiplication  */
# ఒకవేళ USE_VMULL_చక్రీయ_తనిఖీ_32
  bool vmull_enabled = (cpu_may_support ("pmull")
                        && (getauxval (AT_HWCAP) & HWCAP_PMULL) > 0);
  ఒకవేళ (cksum_debug)
    error (0, 0, "%s",
           (vmull_enabled
            ? _("using vmull hardware support")
            : _("vmull support not detected")));
  ఒకవేళ (vmull_enabled)
    తిరిగి cksum_vmull;
# endif

  తిరిగి ఖాళీ;
}

స్థిర bool
cksum_slice8 (FILE *fp, uint_fast32_t *చక్రీయ_తనిఖీ_బహిర్గతం, intmax_t *నిడివి_బహిర్గతం)
{
  uint32_t buf[BUFLEN / పరిమాణం (uint32_t)];
  uint_fast32_t చక్రీయ_తనిఖీ = 0;
  intmax_t నిడివి = 0;
  size_t బైట్లు_read;

  ఒకవేళ (!fp || !చక్రీయ_తనిఖీ_బహిర్గతం || !నిడివి_బహిర్గతం)
    తిరిగి false;

  అయితే ((బైట్లు_read = fread (buf, 1, BUFLEN, fp)) > 0)
    {
      uint32_t *datap;

      ఒకవేళ (ckd_కూడు (&నిడివి, నిడివి, బైట్లు_read))
        {
          errno = EOVERFLOW;
          తిరిగి false;
        }

      /* Process multiples of 8 bytes */
      datap = buf;
      అయితే (బైట్లు_read >= 8)
        {
          uint32_t మొదటి = *datap++, రెండవ = *datap++;
          చక్రీయ_తనిఖీ ^= htobe32 (మొదటి);
          రెండవ = htobe32 (రెండవ);
          చక్రీయ_తనిఖీ = (crctab[7][(చక్రీయ_తనిఖీ >> 24) & 0xFF]
                 ^ crctab[6][(చక్రీయ_తనిఖీ >> 16) & 0xFF]
                 ^ crctab[5][(చక్రీయ_తనిఖీ >> 8) & 0xFF]
                 ^ crctab[4][(చక్రీయ_తనిఖీ) & 0xFF]
                 ^ crctab[3][(రెండవ >> 24) & 0xFF]
                 ^ crctab[2][(రెండవ >> 16) & 0xFF]
                 ^ crctab[1][(రెండవ >> 8) & 0xFF]
                 ^ crctab[0][(రెండవ) & 0xFF]);
          బైట్లు_read -= 8;
        }

      /* And finish up last 0-7 bytes in a byte by byte fashion */
      గుర్తులేని అక్షరం *cp = (గుర్తులేని అక్షరం *)datap;
      అయితే (బైట్లు_read--)
        చక్రీయ_తనిఖీ = (చక్రీయ_తనిఖీ << 8) ^ crctab[0][((చక్రీయ_తనిఖీ >> 24) ^ *cp++) & 0xFF];
      ఒకవేళ (feof (fp))
        ఆపు;
    }

  *చక్రీయ_తనిఖీ_బహిర్గతం = చక్రీయ_తనిఖీ;
  *నిడివి_బహిర్గతం = నిడివి;

  తిరిగి !ferror (fp);
}

/* Calculate the checksum and length in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

పూర్ణాంకం
చక్రీయ_తనిఖీ_కూడిక_ప్రవాహం (FILE *ప్రవాహం, శూన్య *resstream, intmax_t *నిడివి)
{
  intmax_t మొత్తం_బైట్లు = 0;
  uint_fast32_t చక్రీయ_తనిఖీ = 0;

  స్థిర cksum_fp_t cksum_fp;
  ఒకవేళ (! cksum_fp)
    cksum_fp = avx512_supported ();
  ఒకవేళ (! cksum_fp)
    cksum_fp = avx2_supported ();
  ఒకవేళ (! cksum_fp)
    cksum_fp = pclmul_supported ();
  ఒకవేళ (! cksum_fp)
    cksum_fp = vmull_supported ();
  ఒకవేళ (! cksum_fp)
    cksum_fp = cksum_slice8;

  ఒకవేళ (! cksum_fp (ప్రవాహం, &చక్రీయ_తనిఖీ, &మొత్తం_బైట్లు))
    తిరిగి -1;

  *నిడివి = మొత్తం_బైట్లు;

  కొరకు (; మొత్తం_బైట్లు; మొత్తం_బైట్లు >>= 8)
    చక్రీయ_తనిఖీ = (చక్రీయ_తనిఖీ << 8) ^ crctab[0][((చక్రీయ_తనిఖీ >> 24) ^ మొత్తం_బైట్లు) & 0xFF];
  చక్రీయ_తనిఖీ = ~చక్రీయ_తనిఖీ & 0xFFFFFFFF;

  గుర్తులేని పూర్ణాంకం చక్రీయ_తనిఖీ_బహిర్గతం = చక్రీయ_తనిఖీ;
  జ్ఞాపకం_నకలు (resstream, &చక్రీయ_తనిఖీ_బహిర్గతం, పరిమాణం చక్రీయ_తనిఖీ_బహిర్గతం);

  తిరిగి 0;
}

/* Calculate the crc32b checksum and length in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

పూర్ణాంకం
crc32b_కూడిక_ప్రవాహం (FILE *ప్రవాహం, శూన్య *resstream, intmax_t *reslen)
{
  uint32_t buf[BUFLEN / పరిమాణం (uint32_t)];
  uint32_t చక్రీయ_తనిఖీ = 0;
  intmax_t len = 0;
  size_t బైట్లు_read;

  ఒకవేళ (!ప్రవాహం || !resstream || !reslen)
    తిరిగి -1;

# ఒకవేళ GL_చక్రీయ_తనిఖీ_X86_64_PCLMUL
  ఒకవేళ (cksum_debug)
    (శూన్య) pclmul_supported ();
# endif

  అయితే ((బైట్లు_read = fread (buf, 1, BUFLEN, ప్రవాహం)) > 0)
    {
      ఒకవేళ (ckd_కూడు (&len, len, బైట్లు_read))
        {
          errno = EOVERFLOW;
          తిరిగి -1;
        }

      చక్రీయ_తనిఖీ = చక్రీయ_తనిఖీ_32_update (చక్రీయ_తనిఖీ, (అక్షరం స్థిరరాశి *)buf, బైట్లు_read);

      ఒకవేళ (feof (ప్రవాహం))
        ఆపు;
    }

  గుర్తులేని పూర్ణాంకం చక్రీయ_తనిఖీ_బహిర్గతం = చక్రీయ_తనిఖీ;
  జ్ఞాపకం_నకలు (resstream, &చక్రీయ_తనిఖీ_బహిర్గతం, పరిమాణం చక్రీయ_తనిఖీ_బహిర్గతం);

  *reslen = len;

  తిరిగి ferror (ప్రవాహం) ? -1 : 0;
}

/* Print the checksum and size to stdout.
   If ARGS is true, also print the FILE name.  */

శూన్య
output_చక్రీయ_తనిఖీ (అక్షరం స్థిరరాశి *file, MAYBE_వాడని పూర్ణాంకం ద్విమాన_file,
            శూన్య స్థిరరాశి *digest, bool raw, MAYBE_వాడని bool tagged,
            గుర్తులేని అక్షరం delim, bool వాదనలు, intmax_t నిడివి)
{
  ఒకవేళ (raw)
    {
      /* Output in network byte order (big endian).  */
      uint32_t బహిర్గతం_int = htobe32 (*(uint32_t *)digest);
      fwrite (&బహిర్గతం_int, 1, 32/8, stdout);
      తిరిగి;
    }

  రాయి ("%u %jd", *(గుర్తులేని పూర్ణాంకం *)digest, నిడివి);
  ఒకవేళ (వాదనలు)
    రాయి (" %s", file);
  అక్షరం_రాయి (delim);
}

#అయితే_ముగింపు /* !CRCTAB */
