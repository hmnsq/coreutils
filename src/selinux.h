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

#اگر_غیر_تعریف_شدہ COREUTILS_SELINUX_H
# define COREUTILS_SELINUX_H

ساخت selabel_handle;

/* Return true if ERR corresponds to an unsupported request,
   or if there is no context or it's inaccessible.  */
ساکن اندرونی bool
ignorable_ctx_err (صحیح err)
{
  واپس err == ENOTSUP || err == ENODATA;
}

# اگر HAVE_SELINUX_LABEL_H

خارجی bool
restorecon (ساخت selabel_handle *selabel_handle,
            حرف ثابت *راستہ, bool recurse);
خارجی صحیح
defaultcon (ساخت selabel_handle *selabel_handle,
            حرف ثابت *راستہ, mode_t منوال_2);

# ورنہ

ساکن اندرونی bool
restorecon (MAYBE_غیر_مستعمل ساخت selabel_handle *selabel_handle,
            MAYBE_غیر_مستعمل حرف ثابت *راستہ, MAYBE_غیر_مستعمل bool recurse)
{ errno = ENOTSUP; واپس false; }

ساکن اندرونی صحیح
defaultcon (MAYBE_غیر_مستعمل ساخت selabel_handle *selabel_handle,
            MAYBE_غیر_مستعمل حرف ثابت *راستہ, MAYBE_غیر_مستعمل mode_t منوال_2)
{ errno = ENOTSUP; واپس -1; }

# endif

#اگر_ختم
