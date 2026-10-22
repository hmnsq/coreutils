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

#அந்தர்பாவ <config.h>

#அந்தர்பாவ "system.h"
#அந்தர்பாவ "relpath.h"


/* Return the length of the longest common prefix
   of canonical PATH1 and PATH2, ensuring only full path components
   are matched.  Return 0 on no match.  */
ATTRIBUTE_PURE
நிலையான முழு
path_common_prefix (எழுத்து மாறிலி *path1, எழுத்து மாறிலி *path2)
{
  முழு i = 0;
  முழு ret = 0;

  /* We already know path1[0] and path2[0] are '/'.  Special case
     '//', which is only present in a canonical name on platforms
     where it is distinct.  */
  எனில் ((path1[1] == '/') != (path2[1] == '/'))
    திரும்பு 0;

  போது (*path1 && *path2)
    {
      எனில் (*path1 != *path2)
        நிறுத்து;
      எனில் (*path1 == '/')
        ret = i + 1;
      path1++;
      path2++;
      i++;
    }

  எனில் ((!*path1 && !*path2)
      || (!*path1 && *path2 == '/')
      || (!*path2 && *path1 == '/'))
    ret = i;

  திரும்பு ret;
}

/* Either output STR to stdout or
   if *PBUF is not null then append STR to *PBUF
   and update *PBUF to point to the end of the buffer
   and adjust *PLEN to reflect the remaining space.
   Return TRUE on failure.  */
நிலையான bool
buffer_or_output (எழுத்து மாறிலி *str, எழுத்து **pbuf, size_t *plen)
{
  எனில் (*pbuf)
    {
      size_t slen = strlen (str);
      எனில் (slen >= *plen)
        திரும்பு true;
      memcpy (*pbuf, str, slen + 1);
      *pbuf += slen;
      *plen -= slen;
    }
  இல்லையெனில்
    {
      fputs (str, stdout);
    }

  திரும்பு false;
}

/* Output the relative representation if possible.
   If BUF is non-null, write to that buffer rather than to stdout.  */
bool
relpath (எழுத்து மாறிலி *can_fname, எழுத்து மாறிலி *can_reldir, எழுத்து *buf, size_t len)
{
  bool buf_err = false;

  /* Skip the prefix common to --relative-to and path.  */
  முழு common_index = path_common_prefix (can_reldir, can_fname);
  எனில் (!common_index)
    திரும்பு false;

  எழுத்து மாறிலி *relto_suffix = can_reldir + common_index;
  எழுத்து மாறிலி *fname_suffix = can_fname + common_index;

  /* Skip over extraneous '/'.  */
  எனில் (*relto_suffix == '/')
    relto_suffix++;
  எனில் (*fname_suffix == '/')
    fname_suffix++;

  /* Replace remaining components of --relative-to with '..', to get
     to a common directory.  Then output the remainder of fname.  */
  எனில் (*relto_suffix)
    {
      buf_err |= buffer_or_output ("..", &buf, &len);
      க்கு (; *relto_suffix; ++relto_suffix)
        {
          எனில் (*relto_suffix == '/')
            buf_err |= buffer_or_output ("/..", &buf, &len);
        }

      எனில் (*fname_suffix)
        {
          buf_err |= buffer_or_output ("/", &buf, &len);
          buf_err |= buffer_or_output (fname_suffix, &buf, &len);
        }
    }
  இல்லையெனில்
    {
        buf_err |= buffer_or_output (*fname_suffix ? fname_suffix : ".",
                                     &buf, &len);
    }

  எனில் (buf_err)
    error (0, ENAMETOOLONG, "%s", _("generating relative path"));

  திரும்பு !buf_err;
}
