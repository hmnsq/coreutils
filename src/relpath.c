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

#અન્તર્ભાવ <config.h>

#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "relpath.h"


/* Return the length of the longest common prefix
   of canonical PATH1 and PATH2, ensuring only full path components
   are matched.  Return 0 on no match.  */
ATTRIBUTE_PURE
સ્થિર પૂર્ણાંક
path_common_prefix (અક્ષર અચલ *path1, અક્ષર અચલ *path2)
{
  પૂર્ણાંક i = 0;
  પૂર્ણાંક ret = 0;

  /* We already know path1[0] and path2[0] are '/'.  Special case
     '//', which is only present in a canonical name on platforms
     where it is distinct.  */
  જો ((path1[1] == '/') != (path2[1] == '/'))
    પાછા 0;

  જ્યારે (*path1 && *path2)
    {
      જો (*path1 != *path2)
        તોડો;
      જો (*path1 == '/')
        ret = i + 1;
      path1++;
      path2++;
      i++;
    }

  જો ((!*path1 && !*path2)
      || (!*path1 && *path2 == '/')
      || (!*path2 && *path1 == '/'))
    ret = i;

  પાછા ret;
}

/* Either output STR to stdout or
   if *PBUF is not null then append STR to *PBUF
   and update *PBUF to point to the end of the buffer
   and adjust *PLEN to reflect the remaining space.
   Return TRUE on failure.  */
સ્થિર bool
buffer_or_output (અક્ષર અચલ *str, અક્ષર **pbuf, size_t *plen)
{
  જો (*pbuf)
    {
      size_t slen = strlen (str);
      જો (slen >= *plen)
        પાછા true;
      memcpy (*pbuf, str, slen + 1);
      *pbuf += slen;
      *plen -= slen;
    }
  નહીંતર
    {
      fputs (str, stdout);
    }

  પાછા false;
}

/* Output the relative representation if possible.
   If BUF is non-null, write to that buffer rather than to stdout.  */
bool
relpath (અક્ષર અચલ *can_fname, અક્ષર અચલ *can_reldir, અક્ષર *buf, size_t len)
{
  bool buf_err = false;

  /* Skip the prefix common to --relative-to and path.  */
  પૂર્ણાંક common_index = path_common_prefix (can_reldir, can_fname);
  જો (!common_index)
    પાછા false;

  અક્ષર અચલ *relto_suffix = can_reldir + common_index;
  અક્ષર અચલ *fname_suffix = can_fname + common_index;

  /* Skip over extraneous '/'.  */
  જો (*relto_suffix == '/')
    relto_suffix++;
  જો (*fname_suffix == '/')
    fname_suffix++;

  /* Replace remaining components of --relative-to with '..', to get
     to a common directory.  Then output the remainder of fname.  */
  જો (*relto_suffix)
    {
      buf_err |= buffer_or_output ("..", &buf, &len);
      માટે (; *relto_suffix; ++relto_suffix)
        {
          જો (*relto_suffix == '/')
            buf_err |= buffer_or_output ("/..", &buf, &len);
        }

      જો (*fname_suffix)
        {
          buf_err |= buffer_or_output ("/", &buf, &len);
          buf_err |= buffer_or_output (fname_suffix, &buf, &len);
        }
    }
  નહીંતર
    {
        buf_err |= buffer_or_output (*fname_suffix ? fname_suffix : ".",
                                     &buf, &len);
    }

  જો (buf_err)
    error (0, ENAMETOOLONG, "%s", _("generating relative path"));

  પાછા !buf_err;
}
