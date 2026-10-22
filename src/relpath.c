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

#شامل <config.h>

#شامل "system.h"
#شامل "relpath.h"


/* Return the length of the longest common prefix
   of canonical PATH1 and PATH2, ensuring only full path components
   are matched.  Return 0 on no match.  */
ATTRIBUTE_PURE
ایستا صحیح
path_common_prefix (نویسه ثابت *path1, نویسه ثابت *path2)
{
  صحیح i = 0;
  صحیح ret = 0;

  /* We already know path1[0] and path2[0] are '/'.  Special case
     '//', which is only present in a canonical name on platforms
     where it is distinct.  */
  اگر ((path1[1] == '/') != (path2[1] == '/'))
    بازگشت 0;

  وقتی (*path1 && *path2)
    {
      اگر (*path1 != *path2)
        بشکن;
      اگر (*path1 == '/')
        ret = i + 1;
      path1++;
      path2++;
      i++;
    }

  اگر ((!*path1 && !*path2)
      || (!*path1 && *path2 == '/')
      || (!*path2 && *path1 == '/'))
    ret = i;

  بازگشت ret;
}

/* Either output STR to stdout or
   if *PBUF is not null then append STR to *PBUF
   and update *PBUF to point to the end of the buffer
   and adjust *PLEN to reflect the remaining space.
   Return TRUE on failure.  */
ایستا bool
buffer_or_output (نویسه ثابت *str, نویسه **pbuf, size_t *plen)
{
  اگر (*pbuf)
    {
      size_t slen = strlen (str);
      اگر (slen >= *plen)
        بازگشت true;
      memcpy (*pbuf, str, slen + 1);
      *pbuf += slen;
      *plen -= slen;
    }
  وگرنه
    {
      fputs (str, stdout);
    }

  بازگشت false;
}

/* Output the relative representation if possible.
   If BUF is non-null, write to that buffer rather than to stdout.  */
bool
relpath (نویسه ثابت *can_fname, نویسه ثابت *can_reldir, نویسه *buf, size_t len)
{
  bool buf_err = false;

  /* Skip the prefix common to --relative-to and path.  */
  صحیح common_index = path_common_prefix (can_reldir, can_fname);
  اگر (!common_index)
    بازگشت false;

  نویسه ثابت *relto_suffix = can_reldir + common_index;
  نویسه ثابت *fname_suffix = can_fname + common_index;

  /* Skip over extraneous '/'.  */
  اگر (*relto_suffix == '/')
    relto_suffix++;
  اگر (*fname_suffix == '/')
    fname_suffix++;

  /* Replace remaining components of --relative-to with '..', to get
     to a common directory.  Then output the remainder of fname.  */
  اگر (*relto_suffix)
    {
      buf_err |= buffer_or_output ("..", &buf, &len);
      برای (; *relto_suffix; ++relto_suffix)
        {
          اگر (*relto_suffix == '/')
            buf_err |= buffer_or_output ("/..", &buf, &len);
        }

      اگر (*fname_suffix)
        {
          buf_err |= buffer_or_output ("/", &buf, &len);
          buf_err |= buffer_or_output (fname_suffix, &buf, &len);
        }
    }
  وگرنه
    {
        buf_err |= buffer_or_output (*fname_suffix ? fname_suffix : ".",
                                     &buf, &len);
    }

  اگر (buf_err)
    error (0, ENAMETOOLONG, "%s", _("generating relative path"));

  بازگشت !buf_err;
}
