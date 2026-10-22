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
स्थिर पूर्णांक
path_common_prefix (अक्षर स्थिरांक *path1, अक्षर स्थिरांक *path2)
{
  पूर्णांक i = 0;
  पूर्णांक ret = 0;

  /* We already know path1[0] and path2[0] are '/'.  Special case
     '//', which is only present in a canonical name on platforms
     where it is distinct.  */
  जर ((path1[1] == '/') != (path2[1] == '/'))
    परत 0;

  पर्यंत (*path1 && *path2)
    {
      जर (*path1 != *path2)
        थांबा;
      जर (*path1 == '/')
        ret = i + 1;
      path1++;
      path2++;
      i++;
    }

  जर ((!*path1 && !*path2)
      || (!*path1 && *path2 == '/')
      || (!*path2 && *path1 == '/'))
    ret = i;

  परत ret;
}

/* Either output STR to stdout or
   if *PBUF is not null then append STR to *PBUF
   and update *PBUF to point to the end of the buffer
   and adjust *PLEN to reflect the remaining space.
   Return TRUE on failure.  */
स्थिर bool
buffer_or_output (अक्षर स्थिरांक *str, अक्षर **pbuf, माप_प्रकार *plen)
{
  जर (*pbuf)
    {
      माप_प्रकार slen = म_माप (str);
      जर (slen >= *plen)
        परत true;
      स_नकल (*pbuf, str, slen + 1);
      *pbuf += slen;
      *plen -= slen;
    }
  अन्यथा
    {
      ख_माला_दो (str, मानक_निकास);
    }

  परत false;
}

/* Output the relative representation if possible.
   If BUF is non-null, write to that buffer rather than to stdout.  */
bool
relpath (अक्षर स्थिरांक *can_fname, अक्षर स्थिरांक *can_reldir, अक्षर *buf, माप_प्रकार len)
{
  bool buf_err = false;

  /* Skip the prefix common to --relative-to and path.  */
  पूर्णांक common_index = path_common_prefix (can_reldir, can_fname);
  जर (!common_index)
    परत false;

  अक्षर स्थिरांक *relto_suffix = can_reldir + common_index;
  अक्षर स्थिरांक *fname_suffix = can_fname + common_index;

  /* Skip over extraneous '/'.  */
  जर (*relto_suffix == '/')
    relto_suffix++;
  जर (*fname_suffix == '/')
    fname_suffix++;

  /* Replace remaining components of --relative-to with '..', to get
     to a common directory.  Then output the remainder of fname.  */
  जर (*relto_suffix)
    {
      buf_err |= buffer_or_output ("..", &buf, &len);
      साठी (; *relto_suffix; ++relto_suffix)
        {
          जर (*relto_suffix == '/')
            buf_err |= buffer_or_output ("/..", &buf, &len);
        }

      जर (*fname_suffix)
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

  जर (buf_err)
    error (0, ENAMETOOLONG, "%s", _("generating relative path"));

  परत !buf_err;
}
