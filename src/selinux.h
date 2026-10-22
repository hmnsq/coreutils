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

কাঠামো selabel_handle;

/* Return true if ERR corresponds to an unsupported request,
   or if there is no context or it's inaccessible.  */
স্থির অন্তঃরেখ bool
ignorable_ctx_err (পূর্ণসংখ্যা err)
{
  ফেরত err == ENOTSUP || err == ENODATA;
}

# যদি HAVE_SELINUX_LABEL_H

বাহ্যিক bool
restorecon (কাঠামো selabel_handle *selabel_handle,
            অক্ষর ধ্রুবক *path, bool recurse);
বাহ্যিক পূর্ণসংখ্যা
defaultcon (কাঠামো selabel_handle *selabel_handle,
            অক্ষর ধ্রুবক *path, mode_t mode);

# অন্যথা

স্থির অন্তঃরেখ bool
restorecon (MAYBE_UNUSED কাঠামো selabel_handle *selabel_handle,
            MAYBE_UNUSED অক্ষর ধ্রুবক *path, MAYBE_UNUSED bool recurse)
{ errno = ENOTSUP; ফেরত false; }

স্থির অন্তঃরেখ পূর্ণসংখ্যা
defaultcon (MAYBE_UNUSED কাঠামো selabel_handle *selabel_handle,
            MAYBE_UNUSED অক্ষর ধ্রুবক *path, MAYBE_UNUSED mode_t mode)
{ errno = ENOTSUP; ফেরত -1; }

# endif

#endif
