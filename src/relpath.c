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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "relpath.h"


/* Return the length of the longest common prefix
   of canonical PATH1 and PATH2, ensuring only full path components
   are matched.  Return 0 on no match.  */
ATTRIBUTE_ਸ਼ੁੱਧ
ਸਥਿਰ ਪੂਰਨ
ਰਸਤਾ_common_ਅਗੇਤਰ (ਅੱਖਰ ਅਚਲ *path1, ਅੱਖਰ ਅਚਲ *path2)
{
  ਪੂਰਨ i = 0;
  ਪੂਰਨ ret = 0;

  /* We already know path1[0] and path2[0] are '/'.  Special case
     '//', which is only present in a canonical name on platforms
     where it is distinct.  */
  ਜੇ ((path1[1] == '/') != (path2[1] == '/'))
    ਵਾਪਸ 0;

  ਜਦੋਂ (*path1 && *path2)
    {
      ਜੇ (*path1 != *path2)
        ਰੋਕੋ;
      ਜੇ (*path1 == '/')
        ret = i + 1;
      path1++;
      path2++;
      i++;
    }

  ਜੇ ((!*path1 && !*path2)
      || (!*path1 && *path2 == '/')
      || (!*path2 && *path1 == '/'))
    ret = i;

  ਵਾਪਸ ret;
}

/* Either output STR to stdout or
   if *PBUF is not null then append STR to *PBUF
   and update *PBUF to point to the end of the buffer
   and adjust *PLEN to reflect the remaining space.
   Return TRUE on failure.  */
ਸਥਿਰ bool
ਬਫਰ_or_output (ਅੱਖਰ ਅਚਲ *str, ਅੱਖਰ **pbuf, size_t *plen)
{
  ਜੇ (*pbuf)
    {
      size_t slen = ਲੜੀ_ਲੰਬਾਈ (str);
      ਜੇ (slen >= *plen)
        ਵਾਪਸ true;
      ਸਮ੍ਰਿਤੀ_ਨਕਲ (*pbuf, str, slen + 1);
      *pbuf += slen;
      *plen -= slen;
    }
  ਨਹੀਂਤਾਂ
    {
      fputs (str, stdout);
    }

  ਵਾਪਸ false;
}

/* Output the relative representation if possible.
   If BUF is non-null, write to that buffer rather than to stdout.  */
bool
relpath (ਅੱਖਰ ਅਚਲ *can_fname, ਅੱਖਰ ਅਚਲ *can_reldir, ਅੱਖਰ *buf, size_t len)
{
  bool buf_err = false;

  /* Skip the prefix common to --relative-to and path.  */
  ਪੂਰਨ common_ਸੂਚਕਾਂਕ = ਰਸਤਾ_common_ਅਗੇਤਰ (can_reldir, can_fname);
  ਜੇ (!common_ਸੂਚਕਾਂਕ)
    ਵਾਪਸ false;

  ਅੱਖਰ ਅਚਲ *relto_suffix = can_reldir + common_ਸੂਚਕਾਂਕ;
  ਅੱਖਰ ਅਚਲ *fname_suffix = can_fname + common_ਸੂਚਕਾਂਕ;

  /* Skip over extraneous '/'.  */
  ਜੇ (*relto_suffix == '/')
    relto_suffix++;
  ਜੇ (*fname_suffix == '/')
    fname_suffix++;

  /* Replace remaining components of --relative-to with '..', to get
     to a common directory.  Then output the remainder of fname.  */
  ਜੇ (*relto_suffix)
    {
      buf_err |= ਬਫਰ_or_output ("..", &buf, &len);
      ਲਈ (; *relto_suffix; ++relto_suffix)
        {
          ਜੇ (*relto_suffix == '/')
            buf_err |= ਬਫਰ_or_output ("/..", &buf, &len);
        }

      ਜੇ (*fname_suffix)
        {
          buf_err |= ਬਫਰ_or_output ("/", &buf, &len);
          buf_err |= ਬਫਰ_or_output (fname_suffix, &buf, &len);
        }
    }
  ਨਹੀਂਤਾਂ
    {
        buf_err |= ਬਫਰ_or_output (*fname_suffix ? fname_suffix : ".",
                                     &buf, &len);
    }

  ਜੇ (buf_err)
    error (0, ENAMETOOLONG, "%s", _("generating relative path"));

  ਵਾਪਸ !buf_err;
}
