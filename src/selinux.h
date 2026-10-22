/* selinux - core functions for maintaining SELinux labeling
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

/* Written by Daniel Walsh <dwalsh@redhat.com> */

#ifndef COREUTILS_SELINUX_H
# define COREUTILS_SELINUX_H

સંરચના selabel_handle;

/* Return true if ERR corresponds to an unsupported request,
   or if there is no context or it's inaccessible.  */
સ્થિર અંતઃરેખ bool
ignorable_ctx_err (પૂર્ણાંક err)
{
  પાછા err == ENOTSUP || err == ENODATA;
}

# જો HAVE_SELINUX_LABEL_H

બાહ્ય bool
restorecon (સંરચના selabel_handle *selabel_handle,
            અક્ષર અચલ *path, bool recurse);
બાહ્ય પૂર્ણાંક
defaultcon (સંરચના selabel_handle *selabel_handle,
            અક્ષર અચલ *path, mode_t mode);

# નહીંતર

સ્થિર અંતઃરેખ bool
restorecon (MAYBE_UNUSED સંરચના selabel_handle *selabel_handle,
            MAYBE_UNUSED અક્ષર અચલ *path, MAYBE_UNUSED bool recurse)
{ errno = ENOTSUP; પાછા false; }

સ્થિર અંતઃરેખ પૂર્ણાંક
defaultcon (MAYBE_UNUSED સંરચના selabel_handle *selabel_handle,
            MAYBE_UNUSED અક્ષર અચલ *path, MAYBE_UNUSED mode_t mode)
{ errno = ENOTSUP; પાછા -1; }

# endif

#endif
