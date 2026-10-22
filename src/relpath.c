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

#ضمن <config.h>

#ضمن "system.h"
#ضمن "relpath.h"


/* Return the length of the longest common prefix
   of canonical PATH1 and PATH2, ensuring only full path components
   are matched.  Return 0 on no match.  */
ATTRIBUTE_خالص
ساكن صحيح
مسیر_common_پیشوند (حرف ثابت *path1, حرف ثابت *path2)
{
  صحيح i = 0;
  صحيح ret = 0;

  /* We already know path1[0] and path2[0] are '/'.  Special case
     '//', which is only present in a canonical name on platforms
     where it is distinct.  */
  إذا ((path1[1] == '/') != (path2[1] == '/'))
    أرجع 0;

  طالما (*path1 && *path2)
    {
      إذا (*path1 != *path2)
        اكسر;
      إذا (*path1 == '/')
        ret = i + 1;
      path1++;
      path2++;
      i++;
    }

  إذا ((!*path1 && !*path2)
      || (!*path1 && *path2 == '/')
      || (!*path2 && *path1 == '/'))
    ret = i;

  أرجع ret;
}

/* Either output STR to stdout or
   if *PBUF is not null then append STR to *PBUF
   and update *PBUF to point to the end of the buffer
   and adjust *PLEN to reflect the remaining space.
   Return TRUE on failure.  */
ساكن bool
میانگیر_or_output (حرف ثابت *str, حرف **pbuf, size_t *plen)
{
  إذا (*pbuf)
    {
      size_t slen = طول_رشته (str);
      إذا (slen >= *plen)
        أرجع true;
      کپی_حافظه (*pbuf, str, slen + 1);
      *pbuf += slen;
      *plen -= slen;
    }
  وإلا
    {
      fputs (str, stdout);
    }

  أرجع false;
}

/* Output the relative representation if possible.
   If BUF is non-null, write to that buffer rather than to stdout.  */
bool
relpath (حرف ثابت *can_fname, حرف ثابت *can_reldir, حرف *buf, size_t len)
{
  bool buf_err = false;

  /* Skip the prefix common to --relative-to and path.  */
  صحيح common_شاخص = مسیر_common_پیشوند (can_reldir, can_fname);
  إذا (!common_شاخص)
    أرجع false;

  حرف ثابت *relto_suffix = can_reldir + common_شاخص;
  حرف ثابت *fname_suffix = can_fname + common_شاخص;

  /* Skip over extraneous '/'.  */
  إذا (*relto_suffix == '/')
    relto_suffix++;
  إذا (*fname_suffix == '/')
    fname_suffix++;

  /* Replace remaining components of --relative-to with '..', to get
     to a common directory.  Then output the remainder of fname.  */
  إذا (*relto_suffix)
    {
      buf_err |= میانگیر_or_output ("..", &buf, &len);
      لكل (; *relto_suffix; ++relto_suffix)
        {
          إذا (*relto_suffix == '/')
            buf_err |= میانگیر_or_output ("/..", &buf, &len);
        }

      إذا (*fname_suffix)
        {
          buf_err |= میانگیر_or_output ("/", &buf, &len);
          buf_err |= میانگیر_or_output (fname_suffix, &buf, &len);
        }
    }
  وإلا
    {
        buf_err |= میانگیر_or_output (*fname_suffix ? fname_suffix : ".",
                                     &buf, &len);
    }

  إذا (buf_err)
    error (0, ENAMETOOLONG, "%s", _("generating relative path"));

  أرجع !buf_err;
}
