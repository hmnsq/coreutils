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

#অন্তর্ভাব <config.h>

#অন্তর্ভাব "system.h"
#অন্তর্ভাব "relpath.h"


/* Return the length of the longest common prefix
   of canonical PATH1 and PATH2, ensuring only full path components
   are matched.  Return 0 on no match.  */
ATTRIBUTE_PURE
স্থির পূর্ণসংখ্যা
path_common_prefix (অক্ষর ধ্রুবক *path1, অক্ষর ধ্রুবক *path2)
{
  পূর্ণসংখ্যা i = 0;
  পূর্ণসংখ্যা ret = 0;

  /* We already know path1[0] and path2[0] are '/'.  Special case
     '//', which is only present in a canonical name on platforms
     where it is distinct.  */
  যদি ((path1[1] == '/') != (path2[1] == '/'))
    ফেরত 0;

  যখন (*path1 && *path2)
    {
      যদি (*path1 != *path2)
        বিরতি;
      যদি (*path1 == '/')
        ret = i + 1;
      path1++;
      path2++;
      i++;
    }

  যদি ((!*path1 && !*path2)
      || (!*path1 && *path2 == '/')
      || (!*path2 && *path1 == '/'))
    ret = i;

  ফেরত ret;
}

/* Either output STR to stdout or
   if *PBUF is not null then append STR to *PBUF
   and update *PBUF to point to the end of the buffer
   and adjust *PLEN to reflect the remaining space.
   Return TRUE on failure.  */
স্থির bool
বাফার_or_output (অক্ষর ধ্রুবক *str, অক্ষর **pbuf, size_t *plen)
{
  যদি (*pbuf)
    {
      size_t slen = strlen (str);
      যদি (slen >= *plen)
        ফেরত true;
      memcpy (*pbuf, str, slen + 1);
      *pbuf += slen;
      *plen -= slen;
    }
  অন্যথা
    {
      fputs (str, stdout);
    }

  ফেরত false;
}

/* Output the relative representation if possible.
   If BUF is non-null, write to that buffer rather than to stdout.  */
bool
relpath (অক্ষর ধ্রুবক *can_fname, অক্ষর ধ্রুবক *can_reldir, অক্ষর *buf, size_t len)
{
  bool buf_err = false;

  /* Skip the prefix common to --relative-to and path.  */
  পূর্ণসংখ্যা common_সূচক = path_common_prefix (can_reldir, can_fname);
  যদি (!common_সূচক)
    ফেরত false;

  অক্ষর ধ্রুবক *relto_suffix = can_reldir + common_সূচক;
  অক্ষর ধ্রুবক *fname_suffix = can_fname + common_সূচক;

  /* Skip over extraneous '/'.  */
  যদি (*relto_suffix == '/')
    relto_suffix++;
  যদি (*fname_suffix == '/')
    fname_suffix++;

  /* Replace remaining components of --relative-to with '..', to get
     to a common directory.  Then output the remainder of fname.  */
  যদি (*relto_suffix)
    {
      buf_err |= বাফার_or_output ("..", &buf, &len);
      জন্য (; *relto_suffix; ++relto_suffix)
        {
          যদি (*relto_suffix == '/')
            buf_err |= বাফার_or_output ("/..", &buf, &len);
        }

      যদি (*fname_suffix)
        {
          buf_err |= বাফার_or_output ("/", &buf, &len);
          buf_err |= বাফার_or_output (fname_suffix, &buf, &len);
        }
    }
  অন্যথা
    {
        buf_err |= বাফার_or_output (*fname_suffix ? fname_suffix : ".",
                                     &buf, &len);
    }

  যদি (buf_err)
    error (0, ENAMETOOLONG, "%s", _("generating relative path"));

  ফেরত !buf_err;
}
