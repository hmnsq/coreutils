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
path_common_prefix (वर्ण अचर *path1, वर्ण अचर *path2)
{
  पूर्णाङ्क i = 0;
  पूर्णाङ्क ret = 0;

  /* We already know path1[0] and path2[0] are '/'.  Special case
     '//', which is only present in a canonical name on platforms
     where it is distinct.  */
  यदि ((path1[1] == '/') != (path2[1] == '/'))
    फर्काऊ 0;

  जबसम्म (*path1 && *path2)
    {
      यदि (*path1 != *path2)
        रोक;
      यदि (*path1 == '/')
        ret = i + 1;
      path1++;
      path2++;
      i++;
    }

  यदि ((!*path1 && !*path2)
      || (!*path1 && *path2 == '/')
      || (!*path2 && *path1 == '/'))
    ret = i;

  फर्काऊ ret;
}

/* Either output STR to stdout or
   if *PBUF is not null then append STR to *PBUF
   and update *PBUF to point to the end of the buffer
   and adjust *PLEN to reflect the remaining space.
   Return TRUE on failure.  */
स्थिर bool
buffer_or_output (वर्ण अचर *str, वर्ण **pbuf, माप_प्रकार *plen)
{
  यदि (*pbuf)
    {
      माप_प्रकार slen = म_माप (str);
      यदि (slen >= *plen)
        फर्काऊ true;
      स_नकल (*pbuf, str, slen + 1);
      *pbuf += slen;
      *plen -= slen;
    }
  अन्यथा
    {
      ख_माला_दो (str, मानक_निकास);
    }

  फर्काऊ false;
}

/* Output the relative representation if possible.
   If BUF is non-null, write to that buffer rather than to stdout.  */
bool
relpath (वर्ण अचर *can_fname, वर्ण अचर *can_reldir, वर्ण *buf, माप_प्रकार len)
{
  bool buf_err = false;

  /* Skip the prefix common to --relative-to and path.  */
  पूर्णाङ्क common_index = path_common_prefix (can_reldir, can_fname);
  यदि (!common_index)
    फर्काऊ false;

  वर्ण अचर *relto_suffix = can_reldir + common_index;
  वर्ण अचर *fname_suffix = can_fname + common_index;

  /* Skip over extraneous '/'.  */
  यदि (*relto_suffix == '/')
    relto_suffix++;
  यदि (*fname_suffix == '/')
    fname_suffix++;

  /* Replace remaining components of --relative-to with '..', to get
     to a common directory.  Then output the remainder of fname.  */
  यदि (*relto_suffix)
    {
      buf_err |= buffer_or_output ("..", &buf, &len);
      लागि (; *relto_suffix; ++relto_suffix)
        {
          यदि (*relto_suffix == '/')
            buf_err |= buffer_or_output ("/..", &buf, &len);
        }

      यदि (*fname_suffix)
        {
          buf_err |= buffer_or_output ("/", &buf, &len);
          buf_err |= buffer_or_output (fname_suffix, &buf, &len);
        }
    }
  अन्यथा
    {
        buf_err |= buffer_or_output (*fname_suffix ? fname_suffix : ".",
                                     &buf, &len);
    }

  यदि (buf_err)
    error (0, ENAMETOOLONG, "%s", _("generating relative path"));

  फर्काऊ !buf_err;
}
