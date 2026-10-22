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
static số_nguyên
path_common_prefix (char const *path1, char const *path2)
{
  số_nguyên i = 0;
  số_nguyên ret = 0;

  /* We already know path1[0] and path2[0] are '/'.  Special case
     '//', which is only present in a canonical name on platforms
     where it is distinct.  */
  nếu ((path1[1] == '/') != (path2[1] == '/'))
    trả_về 0;

  trong_khi (*path1 && *path2)
    {
      nếu (*path1 != *path2)
        break;
      nếu (*path1 == '/')
        ret = i + 1;
      path1++;
      path2++;
      i++;
    }

  nếu ((!*path1 && !*path2)
      || (!*path1 && *path2 == '/')
      || (!*path2 && *path1 == '/'))
    ret = i;

  trả_về ret;
}

/* Either output STR to stdout or
   if *PBUF is not null then append STR to *PBUF
   and update *PBUF to point to the end of the buffer
   and adjust *PLEN to reflect the remaining space.
   Return TRUE on failure.  */
static bool
buffer_or_output (char const *str, char **pbuf, size_t *plen)
{
  nếu (*pbuf)
    {
      size_t slen = strlen (str);
      nếu (slen >= *plen)
        trả_về true;
      memcpy (*pbuf, str, slen + 1);
      *pbuf += slen;
      *plen -= slen;
    }
  không_thì
    {
      fputs (str, stdout);
    }

  trả_về false;
}

/* Output the relative representation if possible.
   If BUF is non-null, write to that buffer rather than to stdout.  */
bool
relpath (char const *can_fname, char const *can_reldir, char *buf, size_t len)
{
  bool buf_err = false;

  /* Skip the prefix common to --relative-to and path.  */
  số_nguyên common_index = path_common_prefix (can_reldir, can_fname);
  nếu (!common_index)
    trả_về false;

  char const *relto_suffix = can_reldir + common_index;
  char const *fname_suffix = can_fname + common_index;

  /* Skip over extraneous '/'.  */
  nếu (*relto_suffix == '/')
    relto_suffix++;
  nếu (*fname_suffix == '/')
    fname_suffix++;

  /* Replace remaining components of --relative-to with '..', to get
     to a common directory.  Then output the remainder of fname.  */
  nếu (*relto_suffix)
    {
      buf_err |= buffer_or_output ("..", &buf, &len);
      cho (; *relto_suffix; ++relto_suffix)
        {
          nếu (*relto_suffix == '/')
            buf_err |= buffer_or_output ("/..", &buf, &len);
        }

      nếu (*fname_suffix)
        {
          buf_err |= buffer_or_output ("/", &buf, &len);
          buf_err |= buffer_or_output (fname_suffix, &buf, &len);
        }
    }
  không_thì
    {
        buf_err |= buffer_or_output (*fname_suffix ? fname_suffix : ".",
                                     &buf, &len);
    }

  nếu (buf_err)
    error (0, ENAMETOOLONG, "%s", _("generating relative path"));

  trả_về !buf_err;
}
