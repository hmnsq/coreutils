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
ATTRIBUTE_PURE
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
path_common_prefix (ವರ್ಣ ಧ್ರುವ *path1, ವರ್ಣ ಧ್ರುವ *path2)
{
  ಪೂರ್ಣಾಙ್ಕ i = 0;
  ಪೂರ್ಣಾಙ್ಕ ret = 0;

  /* We already know path1[0] and path2[0] are '/'.  Special case
     '//', which is only present in a canonical name on platforms
     where it is distinct.  */
  ಯದಿ ((path1[1] == '/') != (path2[1] == '/'))
    ಪ್ರತ್ಯಾಗಮ 0;

  ಯಾವತ್ (*path1 && *path2)
    {
      ಯದಿ (*path1 != *path2)
        ಭಙ್ಗ;
      ಯದಿ (*path1 == '/')
        ret = i + 1;
      path1++;
      path2++;
      i++;
    }

  ಯದಿ ((!*path1 && !*path2)
      || (!*path1 && *path2 == '/')
      || (!*path2 && *path1 == '/'))
    ret = i;

  ಪ್ರತ್ಯಾಗಮ ret;
}

/* Either output STR to stdout or
   if *PBUF is not null then append STR to *PBUF
   and update *PBUF to point to the end of the buffer
   and adjust *PLEN to reflect the remaining space.
   Return TRUE on failure.  */
ಸ್ಥಿರ bool
buffer_or_output (ವರ್ಣ ಧ್ರುವ *str, ವರ್ಣ **pbuf, size_t *plen)
{
  ಯದಿ (*pbuf)
    {
      size_t slen = strlen (str);
      ಯದಿ (slen >= *plen)
        ಪ್ರತ್ಯಾಗಮ true;
      memcpy (*pbuf, str, slen + 1);
      *pbuf += slen;
      *plen -= slen;
    }
  ಅನ್ಯಥಾ
    {
      fputs (str, stdout);
    }

  ಪ್ರತ್ಯಾಗಮ false;
}

/* Output the relative representation if possible.
   If BUF is non-null, write to that buffer rather than to stdout.  */
bool
relpath (ವರ್ಣ ಧ್ರುವ *can_fname, ವರ್ಣ ಧ್ರುವ *can_reldir, ವರ್ಣ *buf, size_t len)
{
  bool buf_err = false;

  /* Skip the prefix common to --relative-to and path.  */
  ಪೂರ್ಣಾಙ್ಕ common_index = path_common_prefix (can_reldir, can_fname);
  ಯದಿ (!common_index)
    ಪ್ರತ್ಯಾಗಮ false;

  ವರ್ಣ ಧ್ರುವ *relto_suffix = can_reldir + common_index;
  ವರ್ಣ ಧ್ರುವ *fname_suffix = can_fname + common_index;

  /* Skip over extraneous '/'.  */
  ಯದಿ (*relto_suffix == '/')
    relto_suffix++;
  ಯದಿ (*fname_suffix == '/')
    fname_suffix++;

  /* Replace remaining components of --relative-to with '..', to get
     to a common directory.  Then output the remainder of fname.  */
  ಯದಿ (*relto_suffix)
    {
      buf_err |= buffer_or_output ("..", &buf, &len);
      ಪ್ರತಿ (; *relto_suffix; ++relto_suffix)
        {
          ಯದಿ (*relto_suffix == '/')
            buf_err |= buffer_or_output ("/..", &buf, &len);
        }

      ಯದಿ (*fname_suffix)
        {
          buf_err |= buffer_or_output ("/", &buf, &len);
          buf_err |= buffer_or_output (fname_suffix, &buf, &len);
        }
    }
  ಅನ್ಯಥಾ
    {
        buf_err |= buffer_or_output (*fname_suffix ? fname_suffix : ".",
                                     &buf, &len);
    }

  ಯದಿ (buf_err)
    error (0, ENAMETOOLONG, "%s", _("generating relative path"));

  ಪ್ರತ್ಯಾಗಮ !buf_err;
}
