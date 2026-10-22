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

#समावेश <config.h>

#समावेश "system.h"
#समावेश "relpath.h"


/* Return the length of the longest common prefix
   of canonical PATH1 and PATH2, ensuring only full path components
   are matched.  Return 0 on no match.  */
ATTRIBUTE_PURE
अटल पूर्णांक
path_common_prefix (अक्षर स्थिर *path1, अक्षर स्थिर *path2)
{
  पूर्णांक i = 0;
  पूर्णांक ret = 0;

  /* We already know path1[0] and path2[0] are '/'.  Special case
     '//', which is only present in a canonical name on platforms
     where it is distinct.  */
  अगर ((path1[1] == '/') != (path2[1] == '/'))
    वापस 0;

  जबतक (*path1 && *path2)
    {
      अगर (*path1 != *path2)
        अवरोध;
      अगर (*path1 == '/')
        ret = i + 1;
      path1++;
      path2++;
      i++;
    }

  अगर ((!*path1 && !*path2)
      || (!*path1 && *path2 == '/')
      || (!*path2 && *path1 == '/'))
    ret = i;

  वापस ret;
}

/* Either output STR to stdout or
   if *PBUF is not null then append STR to *PBUF
   and update *PBUF to point to the end of the buffer
   and adjust *PLEN to reflect the remaining space.
   Return TRUE on failure.  */
अटल bool
बफर_or_output (अक्षर स्थिर *str, अक्षर **pbuf, माप_प्रकार *plen)
{
  अगर (*pbuf)
    {
      माप_प्रकार slen = म_माप (str);
      अगर (slen >= *plen)
        वापस true;
      स_नकल (*pbuf, str, slen + 1);
      *pbuf += slen;
      *plen -= slen;
    }
  अन्यथा
    {
      ख_माला_दो (str, मानक_निकास);
    }

  वापस false;
}

/* Output the relative representation if possible.
   If BUF is non-null, write to that buffer rather than to stdout.  */
bool
relpath (अक्षर स्थिर *can_fname, अक्षर स्थिर *can_reldir, अक्षर *buf, माप_प्रकार len)
{
  bool buf_err = false;

  /* Skip the prefix common to --relative-to and path.  */
  पूर्णांक common_सूचकांक = path_common_prefix (can_reldir, can_fname);
  अगर (!common_सूचकांक)
    वापस false;

  अक्षर स्थिर *relto_suffix = can_reldir + common_सूचकांक;
  अक्षर स्थिर *fname_suffix = can_fname + common_सूचकांक;

  /* Skip over extraneous '/'.  */
  अगर (*relto_suffix == '/')
    relto_suffix++;
  अगर (*fname_suffix == '/')
    fname_suffix++;

  /* Replace remaining components of --relative-to with '..', to get
     to a common directory.  Then output the remainder of fname.  */
  अगर (*relto_suffix)
    {
      buf_err |= बफर_or_output ("..", &buf, &len);
      क्रम (; *relto_suffix; ++relto_suffix)
        {
          अगर (*relto_suffix == '/')
            buf_err |= बफर_or_output ("/..", &buf, &len);
        }

      अगर (*fname_suffix)
        {
          buf_err |= बफर_or_output ("/", &buf, &len);
          buf_err |= बफर_or_output (fname_suffix, &buf, &len);
        }
    }
  अन्यथा
    {
        buf_err |= बफर_or_output (*fname_suffix ? fname_suffix : ".",
                                     &buf, &len);
    }

  अगर (buf_err)
    error (0, ENAMETOOLONG, "%s", _("generating relative path"));

  वापस !buf_err;
}
