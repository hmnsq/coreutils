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

#അന്തര്ഭാവ <config.h>

#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "relpath.h"


/* Return the length of the longest common prefix
   of canonical PATH1 and PATH2, ensuring only full path components
   are matched.  Return 0 on no match.  */
ATTRIBUTE_PURE
സ്ഥിതം പൂർണ്ണസംഖ്യ
path_common_prefix (അക്ഷരം സ്ഥിരം *path1, അക്ഷരം സ്ഥിരം *path2)
{
  പൂർണ്ണസംഖ്യ i = 0;
  പൂർണ്ണസംഖ്യ ret = 0;

  /* We already know path1[0] and path2[0] are '/'.  Special case
     '//', which is only present in a canonical name on platforms
     where it is distinct.  */
  എങ്കിൽ ((path1[1] == '/') != (path2[1] == '/'))
    തിരികെ 0;

  ആകുമ്പോൾ (*path1 && *path2)
    {
      എങ്കിൽ (*path1 != *path2)
        നിർത്തുക;
      എങ്കിൽ (*path1 == '/')
        ret = i + 1;
      path1++;
      path2++;
      i++;
    }

  എങ്കിൽ ((!*path1 && !*path2)
      || (!*path1 && *path2 == '/')
      || (!*path2 && *path1 == '/'))
    ret = i;

  തിരികെ ret;
}

/* Either output STR to stdout or
   if *PBUF is not null then append STR to *PBUF
   and update *PBUF to point to the end of the buffer
   and adjust *PLEN to reflect the remaining space.
   Return TRUE on failure.  */
സ്ഥിതം bool
buffer_or_output (അക്ഷരം സ്ഥിരം *str, അക്ഷരം **pbuf, size_t *plen)
{
  എങ്കിൽ (*pbuf)
    {
      size_t slen = strlen (str);
      എങ്കിൽ (slen >= *plen)
        തിരികെ true;
      memcpy (*pbuf, str, slen + 1);
      *pbuf += slen;
      *plen -= slen;
    }
  അല്ലെങ്കിൽ
    {
      fputs (str, stdout);
    }

  തിരികെ false;
}

/* Output the relative representation if possible.
   If BUF is non-null, write to that buffer rather than to stdout.  */
bool
relpath (അക്ഷരം സ്ഥിരം *can_fname, അക്ഷരം സ്ഥിരം *can_reldir, അക്ഷരം *buf, size_t len)
{
  bool buf_err = false;

  /* Skip the prefix common to --relative-to and path.  */
  പൂർണ്ണസംഖ്യ common_index = path_common_prefix (can_reldir, can_fname);
  എങ്കിൽ (!common_index)
    തിരികെ false;

  അക്ഷരം സ്ഥിരം *relto_suffix = can_reldir + common_index;
  അക്ഷരം സ്ഥിരം *fname_suffix = can_fname + common_index;

  /* Skip over extraneous '/'.  */
  എങ്കിൽ (*relto_suffix == '/')
    relto_suffix++;
  എങ്കിൽ (*fname_suffix == '/')
    fname_suffix++;

  /* Replace remaining components of --relative-to with '..', to get
     to a common directory.  Then output the remainder of fname.  */
  എങ്കിൽ (*relto_suffix)
    {
      buf_err |= buffer_or_output ("..", &buf, &len);
      വേണ്ടി (; *relto_suffix; ++relto_suffix)
        {
          എങ്കിൽ (*relto_suffix == '/')
            buf_err |= buffer_or_output ("/..", &buf, &len);
        }

      എങ്കിൽ (*fname_suffix)
        {
          buf_err |= buffer_or_output ("/", &buf, &len);
          buf_err |= buffer_or_output (fname_suffix, &buf, &len);
        }
    }
  അല്ലെങ്കിൽ
    {
        buf_err |= buffer_or_output (*fname_suffix ? fname_suffix : ".",
                                     &buf, &len);
    }

  എങ്കിൽ (buf_err)
    error (0, ENAMETOOLONG, "%s", _("generating relative path"));

  തിരികെ !buf_err;
}
