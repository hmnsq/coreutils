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
static ціле
path_common_prefix (char const *path1, char const *path2)
{
  ціле i = 0;
  ціле ret = 0;

  /* We already know path1[0] and path2[0] are '/'.  Special case
     '//', which is only present in a canonical name on platforms
     where it is distinct.  */
  якщо ((path1[1] == '/') != (path2[1] == '/'))
    повернути 0;

  поки (*path1 && *path2)
    {
      якщо (*path1 != *path2)
        break;
      якщо (*path1 == '/')
        ret = i + 1;
      path1++;
      path2++;
      i++;
    }

  якщо ((!*path1 && !*path2)
      || (!*path1 && *path2 == '/')
      || (!*path2 && *path1 == '/'))
    ret = i;

  повернути ret;
}

/* Either output STR to stdout or
   if *PBUF is not null then append STR to *PBUF
   and update *PBUF to point to the end of the buffer
   and adjust *PLEN to reflect the remaining space.
   Return TRUE on failure.  */
static bool
buffer_or_output (char const *str, char **pbuf, size_t *plen)
{
  якщо (*pbuf)
    {
      size_t slen = strlen (str);
      якщо (slen >= *plen)
        повернути true;
      memcpy (*pbuf, str, slen + 1);
      *pbuf += slen;
      *plen -= slen;
    }
  інакше
    {
      fputs (str, stdout);
    }

  повернути false;
}

/* Output the relative representation if possible.
   If BUF is non-null, write to that buffer rather than to stdout.  */
bool
relpath (char const *can_fname, char const *can_reldir, char *buf, size_t len)
{
  bool buf_err = false;

  /* Skip the prefix common to --relative-to and path.  */
  ціле common_index = path_common_prefix (can_reldir, can_fname);
  якщо (!common_index)
    повернути false;

  char const *relto_suffix = can_reldir + common_index;
  char const *fname_suffix = can_fname + common_index;

  /* Skip over extraneous '/'.  */
  якщо (*relto_suffix == '/')
    relto_suffix++;
  якщо (*fname_suffix == '/')
    fname_suffix++;

  /* Replace remaining components of --relative-to with '..', to get
     to a common directory.  Then output the remainder of fname.  */
  якщо (*relto_suffix)
    {
      buf_err |= buffer_or_output ("..", &buf, &len);
      для (; *relto_suffix; ++relto_suffix)
        {
          якщо (*relto_suffix == '/')
            buf_err |= buffer_or_output ("/..", &buf, &len);
        }

      якщо (*fname_suffix)
        {
          buf_err |= buffer_or_output ("/", &buf, &len);
          buf_err |= buffer_or_output (fname_suffix, &buf, &len);
        }
    }
  інакше
    {
        buf_err |= buffer_or_output (*fname_suffix ? fname_suffix : ".",
                                     &buf, &len);
    }

  якщо (buf_err)
    error (0, ENAMETOOLONG, "%s", _("generating relative path"));

  повернути !buf_err;
}
