/* relpath - print the relative path
   Copyright (C) 2012-2026 Free Software Foundation, Inc.

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

/* Written by Pádraig Brady.  */

#అన్తర్భావ <config.h>

#అన్తర్భావ "system.h"
#అన్తర్భావ "relpath.h"


/* Return the length of the longest common prefix
   of canonical PATH1 and PATH2, ensuring only full path components
   are matched.  Return 0 on no match.  */
ATTRIBUTE_శుద్ధ
స్థిర పూర్ణాంకం
మార్గం_common_ఉపసర్గ (అక్షరం స్థిరరాశి *path1, అక్షరం స్థిరరాశి *path2)
{
  పూర్ణాంకం i = 0;
  పూర్ణాంకం ret = 0;

  /* We already know path1[0] and path2[0] are '/'.  Special case
     '//', which is only present in a canonical name on platforms
     where it is distinct.  */
  ఒకవేళ ((path1[1] == '/') != (path2[1] == '/'))
    తిరిగి 0;

  అయితే (*path1 && *path2)
    {
      ఒకవేళ (*path1 != *path2)
        ఆపు;
      ఒకవేళ (*path1 == '/')
        ret = i + 1;
      path1++;
      path2++;
      i++;
    }

  ఒకవేళ ((!*path1 && !*path2)
      || (!*path1 && *path2 == '/')
      || (!*path2 && *path1 == '/'))
    ret = i;

  తిరిగి ret;
}

/* Either output STR to stdout or
   if *PBUF is not null then append STR to *PBUF
   and update *PBUF to point to the end of the buffer
   and adjust *PLEN to reflect the remaining space.
   Return TRUE on failure.  */
స్థిర bool
బఫరు_or_output (అక్షరం స్థిరరాశి *str, అక్షరం **pbuf, size_t *plen)
{
  ఒకవేళ (*pbuf)
    {
      size_t slen = అక్షరశ్రేణి_పొడవు (str);
      ఒకవేళ (slen >= *plen)
        తిరిగి true;
      జ్ఞాపకం_నకలు (*pbuf, str, slen + 1);
      *pbuf += slen;
      *plen -= slen;
    }
  లేకపోతే
    {
      fputs (str, stdout);
    }

  తిరిగి false;
}

/* Output the relative representation if possible.
   If BUF is non-null, write to that buffer rather than to stdout.  */
bool
relpath (అక్షరం స్థిరరాశి *can_fname, అక్షరం స్థిరరాశి *can_reldir, అక్షరం *buf, size_t len)
{
  bool buf_err = false;

  /* Skip the prefix common to --relative-to and path.  */
  పూర్ణాంకం common_సూచీ = మార్గం_common_ఉపసర్గ (can_reldir, can_fname);
  ఒకవేళ (!common_సూచీ)
    తిరిగి false;

  అక్షరం స్థిరరాశి *relto_suffix = can_reldir + common_సూచీ;
  అక్షరం స్థిరరాశి *fname_suffix = can_fname + common_సూచీ;

  /* Skip over extraneous '/'.  */
  ఒకవేళ (*relto_suffix == '/')
    relto_suffix++;
  ఒకవేళ (*fname_suffix == '/')
    fname_suffix++;

  /* Replace remaining components of --relative-to with '..', to get
     to a common directory.  Then output the remainder of fname.  */
  ఒకవేళ (*relto_suffix)
    {
      buf_err |= బఫరు_or_output ("..", &buf, &len);
      కొరకు (; *relto_suffix; ++relto_suffix)
        {
          ఒకవేళ (*relto_suffix == '/')
            buf_err |= బఫరు_or_output ("/..", &buf, &len);
        }

      ఒకవేళ (*fname_suffix)
        {
          buf_err |= బఫరు_or_output ("/", &buf, &len);
          buf_err |= బఫరు_or_output (fname_suffix, &buf, &len);
        }
    }
  లేకపోతే
    {
        buf_err |= బఫరు_or_output (*fname_suffix ? fname_suffix : ".",
                                     &buf, &len);
    }

  ఒకవేళ (buf_err)
    error (0, ENAMETOOLONG, "%s", _("generating relative path"));

  తిరిగి !buf_err;
}
