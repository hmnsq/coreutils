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

ساختار selabel_handle;

/* Return true if ERR corresponds to an unsupported request,
   or if there is no context or it's inaccessible.  */
ایستا درونخطی bool
ignorable_ctx_err (صحیح err)
{
  بازگشت err == ENOTSUP || err == ENODATA;
}

# اگر HAVE_SELINUX_LABEL_H

بیرونی bool
restorecon (ساختار selabel_handle *selabel_handle,
            نویسه ثابت *path, bool recurse);
بیرونی صحیح
defaultcon (ساختار selabel_handle *selabel_handle,
            نویسه ثابت *path, mode_t mode);

# وگرنه

ایستا درونخطی bool
restorecon (MAYBE_UNUSED ساختار selabel_handle *selabel_handle,
            MAYBE_UNUSED نویسه ثابت *path, MAYBE_UNUSED bool recurse)
{ errno = ENOTSUP; بازگشت false; }

ایستا درونخطی صحیح
defaultcon (MAYBE_UNUSED ساختار selabel_handle *selabel_handle,
            MAYBE_UNUSED نویسه ثابت *path, MAYBE_UNUSED mode_t mode)
{ errno = ENOTSUP; بازگشت -1; }

# endif

#endif
