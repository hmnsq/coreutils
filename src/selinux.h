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

#यदि_अपरिभाषितम् COREUTILS_SELINUX_H
# define COREUTILS_SELINUX_H

संरचना selabel_handle;

/* Return true if ERR corresponds to an unsupported request,
   or if there is no context or it's inaccessible.  */
स्थिर अन्तःस्थ bool
ignorable_ctx_err (पूर्णाङ्क err)
{
  प्रत्यागम err == ENOTSUP || err == ENODATA;
}

# यदि HAVE_SELINUX_LABEL_H

बाह्य bool
restorecon (संरचना selabel_handle *selabel_handle,
            वर्ण ध्रुव *मार्गः, bool recurse);
बाह्य पूर्णाङ्क
defaultcon (संरचना selabel_handle *selabel_handle,
            वर्ण ध्रुव *मार्गः, mode_t बहुलकम्_2);

# अन्यथा

स्थिर अन्तःस्थ bool
restorecon (MAYBE_अप्रयुक्तम् संरचना selabel_handle *selabel_handle,
            MAYBE_अप्रयुक्तम् वर्ण ध्रुव *मार्गः, MAYBE_अप्रयुक्तम् bool recurse)
{ errno = ENOTSUP; प्रत्यागम false; }

स्थिर अन्तःस्थ पूर्णाङ्क
defaultcon (MAYBE_अप्रयुक्तम् संरचना selabel_handle *selabel_handle,
            MAYBE_अप्रयुक्तम् वर्ण ध्रुव *मार्गः, MAYBE_अप्रयुक्तम् mode_t बहुलकम्_2)
{ errno = ENOTSUP; प्रत्यागम -1; }

# endif

#यदि_अन्तः
