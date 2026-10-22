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

#ಅನ್ತರ್ಭಾವ <config.h>

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "relpath.h"


/* Return the length of the longest common prefix
   of canonical PATH1 and PATH2, ensuring only full path components
   are matched.  Return 0 on no match.  */
ATTRIBUTE_ಶುದ್ಧ
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ಮಾರ್ಗ_common_ಪೂರ್ವಪ್ರತ್ಯಯ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *path1, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *path2)
{
  ಪೂರ್ಣಾಂಕ i = 0;
  ಪೂರ್ಣಾಂಕ ret = 0;

  /* We already know path1[0] and path2[0] are '/'.  Special case
     '//', which is only present in a canonical name on platforms
     where it is distinct.  */
  ಆದರೆ ((path1[1] == '/') != (path2[1] == '/'))
    ಹಿಂತಿರುಗಿ 0;

  ಆಗ (*path1 && *path2)
    {
      ಆದರೆ (*path1 != *path2)
        ನಿಲ್ಲಿಸು;
      ಆದರೆ (*path1 == '/')
        ret = i + 1;
      path1++;
      path2++;
      i++;
    }

  ಆದರೆ ((!*path1 && !*path2)
      || (!*path1 && *path2 == '/')
      || (!*path2 && *path1 == '/'))
    ret = i;

  ಹಿಂತಿರುಗಿ ret;
}

/* Either output STR to stdout or
   if *PBUF is not null then append STR to *PBUF
   and update *PBUF to point to the end of the buffer
   and adjust *PLEN to reflect the remaining space.
   Return TRUE on failure.  */
ಸ್ಥಿರ bool
ಬಫರ್_or_output (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *str, ಅಕ್ಷರ **pbuf, size_t *plen)
{
  ಆದರೆ (*pbuf)
    {
      size_t slen = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (str);
      ಆದರೆ (slen >= *plen)
        ಹಿಂತಿರುಗಿ true;
      ಸ್ಮರಣೆ_ನಕಲು (*pbuf, str, slen + 1);
      *pbuf += slen;
      *plen -= slen;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      fputs (str, stdout);
    }

  ಹಿಂತಿರುಗಿ false;
}

/* Output the relative representation if possible.
   If BUF is non-null, write to that buffer rather than to stdout.  */
bool
relpath (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *can_fname, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *can_reldir, ಅಕ್ಷರ *buf, size_t len)
{
  bool buf_err = false;

  /* Skip the prefix common to --relative-to and path.  */
  ಪೂರ್ಣಾಂಕ common_ಸೂಚ್ಯಂಕ = ಮಾರ್ಗ_common_ಪೂರ್ವಪ್ರತ್ಯಯ (can_reldir, can_fname);
  ಆದರೆ (!common_ಸೂಚ್ಯಂಕ)
    ಹಿಂತಿರುಗಿ false;

  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *relto_suffix = can_reldir + common_ಸೂಚ್ಯಂಕ;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *fname_suffix = can_fname + common_ಸೂಚ್ಯಂಕ;

  /* Skip over extraneous '/'.  */
  ಆದರೆ (*relto_suffix == '/')
    relto_suffix++;
  ಆದರೆ (*fname_suffix == '/')
    fname_suffix++;

  /* Replace remaining components of --relative-to with '..', to get
     to a common directory.  Then output the remainder of fname.  */
  ಆದರೆ (*relto_suffix)
    {
      buf_err |= ಬಫರ್_or_output ("..", &buf, &len);
      ಗಾಗಿ (; *relto_suffix; ++relto_suffix)
        {
          ಆದರೆ (*relto_suffix == '/')
            buf_err |= ಬಫರ್_or_output ("/..", &buf, &len);
        }

      ಆದರೆ (*fname_suffix)
        {
          buf_err |= ಬಫರ್_or_output ("/", &buf, &len);
          buf_err |= ಬಫರ್_or_output (fname_suffix, &buf, &len);
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
        buf_err |= ಬಫರ್_or_output (*fname_suffix ? fname_suffix : ".",
                                     &buf, &len);
    }

  ಆದರೆ (buf_err)
    error (0, ENAMETOOLONG, "%s", _("generating relative path"));

  ಹಿಂತಿರುಗಿ !buf_err;
}
