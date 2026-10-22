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

संरचना selabel_handle;

/* Return true if ERR corresponds to an unsupported request,
   or if there is no context or it's inaccessible.  */
स्थिर अन्तःस्थ bool
ignorable_ctx_err (पूर्णाङ्क err)
{
  पटिनिवत्त err == ENOTSUP || err == ENODATA;
}

# सचे HAVE_SELINUX_LABEL_H

बाह्य bool
restorecon (संरचना selabel_handle *selabel_handle,
            वर्ण ध्रुव *path, bool recurse);
बाह्य पूर्णाङ्क
defaultcon (संरचना selabel_handle *selabel_handle,
            वर्ण ध्रुव *path, mode_t mode);

# अञ्ञथा

स्थिर अन्तःस्थ bool
restorecon (MAYBE_UNUSED संरचना selabel_handle *selabel_handle,
            MAYBE_UNUSED वर्ण ध्रुव *path, MAYBE_UNUSED bool recurse)
{ errno = ENOTSUP; पटिनिवत्त false; }

स्थिर अन्तःस्थ पूर्णाङ्क
defaultcon (MAYBE_UNUSED संरचना selabel_handle *selabel_handle,
            MAYBE_UNUSED वर्ण ध्रुव *path, MAYBE_UNUSED mode_t mode)
{ errno = ENOTSUP; पटिनिवत्त -1; }

# endif

#endif
