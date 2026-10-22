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

#ଅନ୍ତର୍ଭାଵ <config.h>

#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "relpath.h"


/* Return the length of the longest common prefix
   of canonical PATH1 and PATH2, ensuring only full path components
   are matched.  Return 0 on no match.  */
ATTRIBUTE_PURE
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
path_common_prefix (ଅକ୍ଷର ଅଚଳ *path1, ଅକ୍ଷର ଅଚଳ *path2)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;

  /* We already know path1[0] and path2[0] are '/'.  Special case
     '//', which is only present in a canonical name on platforms
     where it is distinct.  */
  ଯଦି ((path1[1] == '/') != (path2[1] == '/'))
    ଫେରନ୍ତୁ 0;

  ଯେତେବେଳେ (*path1 && *path2)
    {
      ଯଦି (*path1 != *path2)
        ଭାଙ୍ଗ;
      ଯଦି (*path1 == '/')
        ret = i + 1;
      path1++;
      path2++;
      i++;
    }

  ଯଦି ((!*path1 && !*path2)
      || (!*path1 && *path2 == '/')
      || (!*path2 && *path1 == '/'))
    ret = i;

  ଫେରନ୍ତୁ ret;
}

/* Either output STR to stdout or
   if *PBUF is not null then append STR to *PBUF
   and update *PBUF to point to the end of the buffer
   and adjust *PLEN to reflect the remaining space.
   Return TRUE on failure.  */
ସ୍ଥିର bool
buffer_or_output (ଅକ୍ଷର ଅଚଳ *str, ଅକ୍ଷର **pbuf, size_t *plen)
{
  ଯଦି (*pbuf)
    {
      size_t slen = strlen (str);
      ଯଦି (slen >= *plen)
        ଫେରନ୍ତୁ true;
      memcpy (*pbuf, str, slen + 1);
      *pbuf += slen;
      *plen -= slen;
    }
  ଅନ୍ୟଥା
    {
      fputs (str, stdout);
    }

  ଫେରନ୍ତୁ false;
}

/* Output the relative representation if possible.
   If BUF is non-null, write to that buffer rather than to stdout.  */
bool
relpath (ଅକ୍ଷର ଅଚଳ *can_fname, ଅକ୍ଷର ଅଚଳ *can_reldir, ଅକ୍ଷର *buf, size_t len)
{
  bool buf_err = false;

  /* Skip the prefix common to --relative-to and path.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା common_index = path_common_prefix (can_reldir, can_fname);
  ଯଦି (!common_index)
    ଫେରନ୍ତୁ false;

  ଅକ୍ଷର ଅଚଳ *relto_suffix = can_reldir + common_index;
  ଅକ୍ଷର ଅଚଳ *fname_suffix = can_fname + common_index;

  /* Skip over extraneous '/'.  */
  ଯଦି (*relto_suffix == '/')
    relto_suffix++;
  ଯଦି (*fname_suffix == '/')
    fname_suffix++;

  /* Replace remaining components of --relative-to with '..', to get
     to a common directory.  Then output the remainder of fname.  */
  ଯଦି (*relto_suffix)
    {
      buf_err |= buffer_or_output ("..", &buf, &len);
      ପାଇଁ (; *relto_suffix; ++relto_suffix)
        {
          ଯଦି (*relto_suffix == '/')
            buf_err |= buffer_or_output ("/..", &buf, &len);
        }

      ଯଦି (*fname_suffix)
        {
          buf_err |= buffer_or_output ("/", &buf, &len);
          buf_err |= buffer_or_output (fname_suffix, &buf, &len);
        }
    }
  ଅନ୍ୟଥା
    {
        buf_err |= buffer_or_output (*fname_suffix ? fname_suffix : ".",
                                     &buf, &len);
    }

  ଯଦି (buf_err)
    error (0, ENAMETOOLONG, "%s", _("generating relative path"));

  ଫେରନ୍ତୁ !buf_err;
}
