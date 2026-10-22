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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ COREUTILS_SELINUX_H
# define COREUTILS_SELINUX_H

ਬਣਤਰ selabel_handle;

/* Return true if ERR corresponds to an unsupported request,
   or if there is no context or it's inaccessible.  */
ਸਥਿਰ ਸਤਰਅੰਦਰ bool
ignorable_ctx_err (ਪੂਰਨ err)
{
  ਵਾਪਸ err == ENOTSUP || err == ENODATA;
}

# ਜੇ HAVE_SELINUX_LABEL_H

ਬਾਹਰੀ bool
restorecon (ਬਣਤਰ selabel_handle *selabel_handle,
            ਅੱਖਰ ਅਚਲ *ਰਸਤਾ, bool recurse);
ਬਾਹਰੀ ਪੂਰਨ
defaultcon (ਬਣਤਰ selabel_handle *selabel_handle,
            ਅੱਖਰ ਅਚਲ *ਰਸਤਾ, mode_t ਬਹੁਲਕ_2);

# ਨਹੀਂਤਾਂ

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
restorecon (MAYBE_ਅਣਵਰਤਿਆ ਬਣਤਰ selabel_handle *selabel_handle,
            MAYBE_ਅਣਵਰਤਿਆ ਅੱਖਰ ਅਚਲ *ਰਸਤਾ, MAYBE_ਅਣਵਰਤਿਆ bool recurse)
{ errno = ENOTSUP; ਵਾਪਸ false; }

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
defaultcon (MAYBE_ਅਣਵਰਤਿਆ ਬਣਤਰ selabel_handle *selabel_handle,
            MAYBE_ਅਣਵਰਤਿਆ ਅੱਖਰ ਅਚਲ *ਰਸਤਾ, MAYBE_ਅਣਵਰਤਿਆ mode_t ਬਹੁਲਕ_2)
{ errno = ENOTSUP; ਵਾਪਸ -1; }

# endif

#ਜੇ_ਅੰਤ
