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

সংরচনা selabel_handle;

/* Return true if ERR corresponds to an unsupported request,
   or if there is no context or it's inaccessible.  */
স্থির অন্তঃস্থ bool
ignorable_ctx_err (পূর্ণাঙ্ক err)
{
  ঘূৰাই err == ENOTSUP || err == ENODATA;
}

# যদি HAVE_SELINUX_LABEL_H

বাহ্য bool
restorecon (সংরচনা selabel_handle *selabel_handle,
            ৱর্ণ ধ্রুৱ *path, bool recurse);
বাহ্য পূর্ণাঙ্ক
defaultcon (সংরচনা selabel_handle *selabel_handle,
            ৱর্ণ ধ্রুৱ *path, mode_t mode);

# অন্যথা

স্থির অন্তঃস্থ bool
restorecon (MAYBE_UNUSED সংরচনা selabel_handle *selabel_handle,
            MAYBE_UNUSED ৱর্ণ ধ্রুৱ *path, MAYBE_UNUSED bool recurse)
{ errno = ENOTSUP; ঘূৰাই false; }

স্থির অন্তঃস্থ পূর্ণাঙ্ক
defaultcon (MAYBE_UNUSED সংরচনা selabel_handle *selabel_handle,
            MAYBE_UNUSED ৱর্ণ ধ্রুৱ *path, MAYBE_UNUSED mode_t mode)
{ errno = ENOTSUP; ঘূৰাই -1; }

# endif

#endif
