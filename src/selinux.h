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

sa_nracanaa selabel_handle;

/* Return true if ERR corresponds to an unsupported request,
   or if there is no context or it's inaccessible.  */
static anta_hstha bool
ignorable_ctx_err (จำนวนเต็ม err)
{
  คืน err == ENOTSUP || err == ENODATA;
}

# ถ้า HAVE_SELINUX_LABEL_H

extern bool
restorecon (sa_nracanaa selabel_handle *selabel_handle,
            var_na const *path, bool recurse);
extern จำนวนเต็ม
defaultcon (sa_nracanaa selabel_handle *selabel_handle,
            var_na const *path, mode_t mode);

# มิฉะนั้น

static anta_hstha bool
restorecon (MAYBE_UNUSED sa_nracanaa selabel_handle *selabel_handle,
            MAYBE_UNUSED var_na const *path, MAYBE_UNUSED bool recurse)
{ errno = ENOTSUP; คืน false; }

static anta_hstha จำนวนเต็ม
defaultcon (MAYBE_UNUSED sa_nracanaa selabel_handle *selabel_handle,
            MAYBE_UNUSED var_na const *path, MAYBE_UNUSED mode_t mode)
{ errno = ENOTSUP; คืน -1; }

# endif

#endif
