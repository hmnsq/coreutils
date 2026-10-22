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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ COREUTILS_SELINUX_H
# define COREUTILS_SELINUX_H

ರಚನೆ selabel_handle;

/* Return true if ERR corresponds to an unsupported request,
   or if there is no context or it's inaccessible.  */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
ignorable_ctx_err (ಪೂರ್ಣಾಂಕ err)
{
  ಹಿಂತಿರುಗಿ err == ENOTSUP || err == ENODATA;
}

# ಆದರೆ HAVE_SELINUX_LABEL_H

ಬಾಹ್ಯ bool
restorecon (ರಚನೆ selabel_handle *selabel_handle,
            ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಮಾರ್ಗ, bool recurse);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ
defaultcon (ರಚನೆ selabel_handle *selabel_handle,
            ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಮಾರ್ಗ, mode_t ಬಹುಲಕ_2);

# ಇಲ್ಲದಿದ್ದರೆ

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
restorecon (MAYBE_ಬಳಸದ ರಚನೆ selabel_handle *selabel_handle,
            MAYBE_ಬಳಸದ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಮಾರ್ಗ, MAYBE_ಬಳಸದ bool recurse)
{ errno = ENOTSUP; ಹಿಂತಿರುಗಿ false; }

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
defaultcon (MAYBE_ಬಳಸದ ರಚನೆ selabel_handle *selabel_handle,
            MAYBE_ಬಳಸದ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಮಾರ್ಗ, MAYBE_ಬಳಸದ mode_t ಬಹುಲಕ_2)
{ errno = ENOTSUP; ಹಿಂತಿರುಗಿ -1; }

# endif

#ಆದರೆ_ಅಂತ್ಯ
