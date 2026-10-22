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

ഘടന selabel_handle;

/* Return true if ERR corresponds to an unsupported request,
   or if there is no context or it's inaccessible.  */
സ്ഥിതം വരിയിൽ bool
ignorable_ctx_err (പൂർണ്ണസംഖ്യ err)
{
  തിരികെ err == ENOTSUP || err == ENODATA;
}

# എങ്കിൽ HAVE_SELINUX_LABEL_H

ബാഹ്യ bool
restorecon (ഘടന selabel_handle *selabel_handle,
            അക്ഷരം സ്ഥിരം *path, bool recurse);
ബാഹ്യ പൂർണ്ണസംഖ്യ
defaultcon (ഘടന selabel_handle *selabel_handle,
            അക്ഷരം സ്ഥിരം *path, mode_t mode);

# അല്ലെങ്കിൽ

സ്ഥിതം വരിയിൽ bool
restorecon (MAYBE_UNUSED ഘടന selabel_handle *selabel_handle,
            MAYBE_UNUSED അക്ഷരം സ്ഥിരം *path, MAYBE_UNUSED bool recurse)
{ errno = ENOTSUP; തിരികെ false; }

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
defaultcon (MAYBE_UNUSED ഘടന selabel_handle *selabel_handle,
            MAYBE_UNUSED അക്ഷരം സ്ഥിരം *path, MAYBE_UNUSED mode_t mode)
{ errno = ENOTSUP; തിരികെ -1; }

# endif

#endif
