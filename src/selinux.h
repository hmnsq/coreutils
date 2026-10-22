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

கட்டமைப்பு selabel_handle;

/* Return true if ERR corresponds to an unsupported request,
   or if there is no context or it's inaccessible.  */
நிலையான உள்வரி bool
ignorable_ctx_err (முழு err)
{
  திரும்பு err == ENOTSUP || err == ENODATA;
}

# எனில் HAVE_SELINUX_LABEL_H

வெளி bool
restorecon (கட்டமைப்பு selabel_handle *selabel_handle,
            எழுத்து மாறிலி *path, bool recurse);
வெளி முழு
defaultcon (கட்டமைப்பு selabel_handle *selabel_handle,
            எழுத்து மாறிலி *path, mode_t mode);

# இல்லையெனில்

நிலையான உள்வரி bool
restorecon (MAYBE_UNUSED கட்டமைப்பு selabel_handle *selabel_handle,
            MAYBE_UNUSED எழுத்து மாறிலி *path, MAYBE_UNUSED bool recurse)
{ errno = ENOTSUP; திரும்பு false; }

நிலையான உள்வரி முழு
defaultcon (MAYBE_UNUSED கட்டமைப்பு selabel_handle *selabel_handle,
            MAYBE_UNUSED எழுத்து மாறிலி *path, MAYBE_UNUSED mode_t mode)
{ errno = ENOTSUP; திரும்பு -1; }

# endif

#endif
