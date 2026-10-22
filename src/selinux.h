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
ignorable_ctx_err (ကိန်းပြည့် err)
{
  ပြန်ပေး err == ENOTSUP || err == ENODATA;
}

# အကယ်၍ HAVE_SELINUX_LABEL_H

extern bool
restorecon (sa_nracanaa selabel_handle *selabel_handle,
            var_na const *path, bool recurse);
extern ကိန်းပြည့်
defaultcon (sa_nracanaa selabel_handle *selabel_handle,
            var_na const *path, mode_t mode);

# မဟုတ်ပါက

static anta_hstha bool
restorecon (MAYBE_UNUSED sa_nracanaa selabel_handle *selabel_handle,
            MAYBE_UNUSED var_na const *path, MAYBE_UNUSED bool recurse)
{ errno = ENOTSUP; ပြန်ပေး false; }

static anta_hstha ကိန်းပြည့်
defaultcon (MAYBE_UNUSED sa_nracanaa selabel_handle *selabel_handle,
            MAYBE_UNUSED var_na const *path, MAYBE_UNUSED mode_t mode)
{ errno = ENOTSUP; ပြန်ပေး -1; }

# endif

#endif
