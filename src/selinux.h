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

#که_نه_تعریف_شوی COREUTILS_SELINUX_H
# define COREUTILS_SELINUX_H

بنية selabel_handle;

/* Return true if ERR corresponds to an unsupported request,
   or if there is no context or it's inaccessible.  */
ساكن مضمن bool
ignorable_ctx_err (صحيح err)
{
  أرجع err == ENOTSUP || err == ENODATA;
}

# إذا HAVE_SELINUX_LABEL_H

خارجي bool
restorecon (بنية selabel_handle *selabel_handle,
            حرف ثابت *لار, bool recurse);
خارجي صحيح
defaultcon (بنية selabel_handle *selabel_handle,
            حرف ثابت *لار, mode_t ډېرپېښ_2);

# وإلا

ساكن مضمن bool
restorecon (MAYBE_ناکارول_شوی بنية selabel_handle *selabel_handle,
            MAYBE_ناکارول_شوی حرف ثابت *لار, MAYBE_ناکارول_شوی bool recurse)
{ errno = ENOTSUP; أرجع false; }

ساكن مضمن صحيح
defaultcon (MAYBE_ناکارول_شوی بنية selabel_handle *selabel_handle,
            MAYBE_ناکارول_شوی حرف ثابت *لار, MAYBE_ناکارول_شوی mode_t ډېرپېښ_2)
{ errno = ENOTSUP; أرجع -1; }

# endif

#د_که_پای
