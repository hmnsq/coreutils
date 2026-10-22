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

ಸಂರಚನಾ selabel_handle;

/* Return true if ERR corresponds to an unsupported request,
   or if there is no context or it's inaccessible.  */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
ignorable_ctx_err (ಪೂರ್ಣಾಙ್ಕ err)
{
  ಪ್ರತ್ಯಾಗಮ err == ENOTSUP || err == ENODATA;
}

# ಯದಿ HAVE_SELINUX_LABEL_H

ಬಾಹ್ಯ bool
restorecon (ಸಂರಚನಾ selabel_handle *selabel_handle,
            ವರ್ಣ ಧ್ರುವ *path, bool recurse);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ
defaultcon (ಸಂರಚನಾ selabel_handle *selabel_handle,
            ವರ್ಣ ಧ್ರುವ *path, mode_t mode);

# ಅನ್ಯಥಾ

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
restorecon (MAYBE_UNUSED ಸಂರಚನಾ selabel_handle *selabel_handle,
            MAYBE_UNUSED ವರ್ಣ ಧ್ರುವ *path, MAYBE_UNUSED bool recurse)
{ errno = ENOTSUP; ಪ್ರತ್ಯಾಗಮ false; }

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ
defaultcon (MAYBE_UNUSED ಸಂರಚನಾ selabel_handle *selabel_handle,
            MAYBE_UNUSED ವರ್ಣ ಧ್ರುವ *path, MAYBE_UNUSED mode_t mode)
{ errno = ENOTSUP; ಪ್ರತ್ಯಾಗಮ -1; }

# endif

#endif
