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

#अन्तर्भाव <config.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "relpath.h"


/* Return the length of the longest common prefix
   of canonical PATH1 and PATH2, ensuring only full path components
   are matched.  Return 0 on no match.  */
ATTRIBUTE_PURE
स्थिर पूर्णाङ्क
path_common_prefix (वर्ण ध्रुव *path1, वर्ण ध्रुव *path2)
{
  पूर्णाङ्क i = 0;
  पूर्णाङ्क ret = 0;

  /* We already know path1[0] and path2[0] are '/'.  Special case
     '//', which is only present in a canonical name on platforms
     where it is distinct.  */
  सचे ((path1[1] == '/') != (path2[1] == '/'))
    पटिनिवत्त 0;

  याव (*path1 && *path2)
    {
      सचे (*path1 != *path2)
        भङ्ग;
      सचे (*path1 == '/')
        ret = i + 1;
      path1++;
      path2++;
      i++;
    }

  सचे ((!*path1 && !*path2)
      || (!*path1 && *path2 == '/')
      || (!*path2 && *path1 == '/'))
    ret = i;

  पटिनिवत्त ret;
}

/* Either output STR to stdout or
   if *PBUF is not null then append STR to *PBUF
   and update *PBUF to point to the end of the buffer
   and adjust *PLEN to reflect the remaining space.
   Return TRUE on failure.  */
स्थिर bool
buffer_or_output (वर्ण ध्रुव *str, वर्ण **pbuf, size_t *plen)
{
  सचे (*pbuf)
    {
      size_t slen = strlen (str);
      सचे (slen >= *plen)
        पटिनिवत्त true;
      memcpy (*pbuf, str, slen + 1);
      *pbuf += slen;
      *plen -= slen;
    }
  अञ्ञथा
    {
      fputs (str, stdout);
    }

  पटिनिवत्त false;
}

/* Output the relative representation if possible.
   If BUF is non-null, write to that buffer rather than to stdout.  */
bool
relpath (वर्ण ध्रुव *can_fname, वर्ण ध्रुव *can_reldir, वर्ण *buf, size_t len)
{
  bool buf_err = false;

  /* Skip the prefix common to --relative-to and path.  */
  पूर्णाङ्क common_index = path_common_prefix (can_reldir, can_fname);
  सचे (!common_index)
    पटिनिवत्त false;

  वर्ण ध्रुव *relto_suffix = can_reldir + common_index;
  वर्ण ध्रुव *fname_suffix = can_fname + common_index;

  /* Skip over extraneous '/'.  */
  सचे (*relto_suffix == '/')
    relto_suffix++;
  सचे (*fname_suffix == '/')
    fname_suffix++;

  /* Replace remaining components of --relative-to with '..', to get
     to a common directory.  Then output the remainder of fname.  */
  सचे (*relto_suffix)
    {
      buf_err |= buffer_or_output ("..", &buf, &len);
      अत्थाय (; *relto_suffix; ++relto_suffix)
        {
          सचे (*relto_suffix == '/')
            buf_err |= buffer_or_output ("/..", &buf, &len);
        }

      सचे (*fname_suffix)
        {
          buf_err |= buffer_or_output ("/", &buf, &len);
          buf_err |= buffer_or_output (fname_suffix, &buf, &len);
        }
    }
  अञ्ञथा
    {
        buf_err |= buffer_or_output (*fname_suffix ? fname_suffix : ".",
                                     &buf, &len);
    }

  सचे (buf_err)
    error (0, ENAMETOOLONG, "%s", _("generating relative path"));

  पटिनिवत्त !buf_err;
}
