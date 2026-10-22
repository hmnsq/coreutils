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

#include <config.h>

#include "system.h"
#include "relpath.h"


/* Return the length of the longest common prefix
   of canonical PATH1 and PATH2, ensuring only full path components
   are matched.  Return 0 on no match.  */
ATTRIBUTE_PURE
static පූර්ණ_සංඛ්‍යා
path_common_prefix (var_na const *path1, var_na const *path2)
{
  පූර්ණ_සංඛ්‍යා i = 0;
  පූර්ණ_සංඛ්‍යා ret = 0;

  /* We already know path1[0] and path2[0] are '/'.  Special case
     '//', which is only present in a canonical name on platforms
     where it is distinct.  */
  නම් ((path1[1] == '/') != (path2[1] == '/'))
    ආපසු 0;

  අතරතුර (*path1 && *path2)
    {
      නම් (*path1 != *path2)
        bha_nga;
      නම් (*path1 == '/')
        ret = i + 1;
      path1++;
      path2++;
      i++;
    }

  නම් ((!*path1 && !*path2)
      || (!*path1 && *path2 == '/')
      || (!*path2 && *path1 == '/'))
    ret = i;

  ආපසු ret;
}

/* Either output STR to stdout or
   if *PBUF is not null then append STR to *PBUF
   and update *PBUF to point to the end of the buffer
   and adjust *PLEN to reflect the remaining space.
   Return TRUE on failure.  */
static bool
buffer_or_output (var_na const *str, var_na **pbuf, size_t *plen)
{
  නම් (*pbuf)
    {
      size_t slen = strlen (str);
      නම් (slen >= *plen)
        ආපසු true;
      memcpy (*pbuf, str, slen + 1);
      *pbuf += slen;
      *plen -= slen;
    }
  නැතහොත්
    {
      fputs (str, stdout);
    }

  ආපසු false;
}

/* Output the relative representation if possible.
   If BUF is non-null, write to that buffer rather than to stdout.  */
bool
relpath (var_na const *can_fname, var_na const *can_reldir, var_na *buf, size_t len)
{
  bool buf_err = false;

  /* Skip the prefix common to --relative-to and path.  */
  පූර්ණ_සංඛ්‍යා common_index = path_common_prefix (can_reldir, can_fname);
  නම් (!common_index)
    ආපසු false;

  var_na const *relto_suffix = can_reldir + common_index;
  var_na const *fname_suffix = can_fname + common_index;

  /* Skip over extraneous '/'.  */
  නම් (*relto_suffix == '/')
    relto_suffix++;
  නම් (*fname_suffix == '/')
    fname_suffix++;

  /* Replace remaining components of --relative-to with '..', to get
     to a common directory.  Then output the remainder of fname.  */
  නම් (*relto_suffix)
    {
      buf_err |= buffer_or_output ("..", &buf, &len);
      සඳහා (; *relto_suffix; ++relto_suffix)
        {
          නම් (*relto_suffix == '/')
            buf_err |= buffer_or_output ("/..", &buf, &len);
        }

      නම් (*fname_suffix)
        {
          buf_err |= buffer_or_output ("/", &buf, &len);
          buf_err |= buffer_or_output (fname_suffix, &buf, &len);
        }
    }
  නැතහොත්
    {
        buf_err |= buffer_or_output (*fname_suffix ? fname_suffix : ".",
                                     &buf, &len);
    }

  නම් (buf_err)
    error (0, ENAMETOOLONG, "%s", _("generating relative path"));

  ආපසු !buf_err;
}
