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

#అన్తర్భావ <config.h>

#అన్తర్భావ <stdio.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <endian.h>
#అన్తర్భావ "system.h"
#అన్తర్భావ "human.h"
#అన్తర్భావ "sum.h"

/* Calculate the checksum and the size in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

పూర్ణాంకం
bsd_కూడిక_ప్రవాహం (FILE *ప్రవాహం, శూన్య *resstream, intmax_t *నిడివి)
{
  పూర్ణాంకం ret = -1;
  idx_t కూడిక;
  పూర్ణాంకం checksum = 0;	/* The checksum mod 2^16. */
  intmax_t మొత్తం_బైట్లు = 0;	/* The number of bytes. */
  గణన { బఫరు_నిడివి = 32768 };
  uint8_t *బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_నిడివి);

  ఒకవేళ (! బఫరు)
    తిరిగి -1;

  /* Process file */
  అయితే (true)
  {
    కూడిక = 0;

    /* Read block */
    అయితే (true)
    {
      size_t n = fread (బఫరు + కూడిక, 1, బఫరు_నిడివి - కూడిక, ప్రవాహం);
      ఒకవేళ (n == 0)
        {
          ఒకవేళ (ferror (ప్రవాహం))
            వెళ్ళు cleanup_బఫరు;
          వెళ్ళు final_process;
        }

      కూడిక += n;

      ఒకవేళ (బఫరు_నిడివి == కూడిక)
        ఆపు;

      ఒకవేళ (feof (ప్రవాహం))
        వెళ్ళు final_process;
    }

    కొరకు (size_t i = 0; i < కూడిక; i++)
      {
        checksum = (checksum >> 1) + ((checksum & 1) << 15);
        checksum += బఫరు[i];
        checksum &= 0xffff;	/* Keep it within bounds. */
      }
    ఒకవేళ (ckd_కూడు (&మొత్తం_బైట్లు, మొత్తం_బైట్లు, కూడిక))
      {
        errno = EOVERFLOW;
        వెళ్ళు cleanup_బఫరు;
      }
  }

final_process:;

  కొరకు (idx_t i = 0; i < కూడిక; i++)
    {
      checksum = (checksum >> 1) + ((checksum & 1) << 15);
      checksum += బఫరు[i];
      checksum &= 0xffff;	/* Keep it within bounds. */
    }
  ఒకవేళ (ckd_కూడు (&మొత్తం_బైట్లు, మొత్తం_బైట్లు, కూడిక))
    {
      errno = EOVERFLOW;
      వెళ్ళు cleanup_బఫరు;
    }

  జ్ఞాపకం_నకలు (resstream, &checksum, పరిమాణం checksum);
  *నిడివి = మొత్తం_బైట్లు;
  ret = 0;
cleanup_బఫరు:
  విడుదల (బఫరు);
  తిరిగి ret;
}

/* Calculate the checksum and the size in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

పూర్ణాంకం
sysv_కూడిక_ప్రవాహం (FILE *ప్రవాహం, శూన్య *resstream, intmax_t *నిడివి)
{
  పూర్ణాంకం ret = -1;
  idx_t కూడిక;
  intmax_t మొత్తం_బైట్లు = 0;
  గణన { బఫరు_నిడివి = 32768 };
  uint8_t *బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_నిడివి);

  ఒకవేళ (! బఫరు)
    తిరిగి -1;

  /* The sum of all the input bytes, modulo (UINT_MAX + 1).  */
  గుర్తులేని పూర్ణాంకం s = 0;

  /* Process file */
  అయితే (true)
  {
    కూడిక = 0;

    /* Read block */
    అయితే (true)
    {
      size_t n = fread (బఫరు + కూడిక, 1, బఫరు_నిడివి - కూడిక, ప్రవాహం);
      ఒకవేళ (n == 0)
        {
          ఒకవేళ (ferror (ప్రవాహం))
            వెళ్ళు cleanup_బఫరు;
          వెళ్ళు final_process;
        }

      కూడిక += n;

      ఒకవేళ (బఫరు_నిడివి == కూడిక)
        ఆపు;

      ఒకవేళ (feof (ప్రవాహం))
        వెళ్ళు final_process;
    }

    కొరకు (size_t i = 0; i < కూడిక; i++)
      s += బఫరు[i];
    ఒకవేళ (మొత్తం_బైట్లు + కూడిక < మొత్తం_బైట్లు)
      {
        errno = EOVERFLOW;
        వెళ్ళు cleanup_బఫరు;
      }
    మొత్తం_బైట్లు += కూడిక;
  }

final_process:;

  కొరకు (size_t i = 0; i < కూడిక; i++)
    s += బఫరు[i];
  ఒకవేళ (మొత్తం_బైట్లు + కూడిక < మొత్తం_బైట్లు)
    {
      errno = EOVERFLOW;
      వెళ్ళు cleanup_బఫరు;
    }
  మొత్తం_బైట్లు += కూడిక;

  పూర్ణాంకం r = (s & 0xffff) + ((s & 0xffffffff) >> 16);
  పూర్ణాంకం checksum = (r & 0xffff) + (r >> 16);

  జ్ఞాపకం_నకలు (resstream, &checksum, పరిమాణం checksum);
  *నిడివి = మొత్తం_బైట్లు;
  ret = 0;
cleanup_బఫరు:
  విడుదల (బఫరు);
  తిరిగి ret;
}

/* Print the checksum and size (in 1024 byte blocks) to stdout.
   If ARGS is true, also print the FILE name.  */

శూన్య
output_bsd (అక్షరం స్థిరరాశి *file, MAYBE_వాడని పూర్ణాంకం ద్విమాన_file, శూన్య స్థిరరాశి *digest,
            bool raw, MAYBE_వాడని bool tagged, గుర్తులేని అక్షరం delim, bool వాదనలు,
            intmax_t నిడివి)
{
  ఒకవేళ (raw)
    {
      /* Output in network byte order (big endian).  */
      uint16_t బహిర్గతం_int = *(పూర్ణాంకం *)digest;
      బహిర్గతం_int = htobe16 (బహిర్గతం_int);
      fwrite (&బహిర్గతం_int, 1, 16/8, stdout);
      తిరిగి;
    }

  అక్షరం hbuf[LONGEST_HUMAN_READABLE + 1];
  రాయి ("%05d %5s", *(పూర్ణాంకం *)digest,
          human_readable (నిడివి, hbuf, human_ceiling, 1, 1024));
  ఒకవేళ (వాదనలు)
    రాయి (" %s", file);
  అక్షరం_రాయి (delim);
}

/* Print the checksum and size (in 512 byte blocks) to stdout.
   If ARGS is true, also print the FILE name.  */

శూన్య
output_sysv (అక్షరం స్థిరరాశి *file, MAYBE_వాడని పూర్ణాంకం ద్విమాన_file,
             శూన్య స్థిరరాశి *digest, bool raw, MAYBE_వాడని bool tagged,
             గుర్తులేని అక్షరం delim, bool వాదనలు, intmax_t నిడివి)
{
  ఒకవేళ (raw)
    {
      /* Output in network byte order (big endian).  */
      uint16_t బహిర్గతం_int = *(పూర్ణాంకం *)digest;
      బహిర్గతం_int = htobe16 (బహిర్గతం_int);
      fwrite (&బహిర్గతం_int, 1, 16/8, stdout);
      తిరిగి;
    }

  అక్షరం hbuf[LONGEST_HUMAN_READABLE + 1];
  రాయి ("%d %s", *(పూర్ణాంకం *)digest,
          human_readable (నిడివి, hbuf, human_ceiling, 1, 512));
  ఒకవేళ (వాదనలు)
    రాయి (" %s", file);
  అక్షరం_రాయి (delim);
}
