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

#अगर_अघोषित COREUTILS_SELINUX_H
# define COREUTILS_SELINUX_H

काष्ठा selabel_handle;

/* Return true if ERR corresponds to an unsupported request,
   or if there is no context or it's inaccessible.  */
अटल अंतरभूत bool
ignorable_ctx_err (पूर्णांक err)
{
  वापस err == ENOTSUP || err == ENODATA;
}

# अगर HAVE_SELINUX_LABEL_H

बाह्य bool
restorecon (काष्ठा selabel_handle *selabel_handle,
            अक्षर स्थिर *path, bool recurse);
बाह्य पूर्णांक
defaultcon (काष्ठा selabel_handle *selabel_handle,
            अक्षर स्थिर *path, mode_t mode);

# अन्यथा

अटल अंतरभूत bool
restorecon (MAYBE_UNUSED काष्ठा selabel_handle *selabel_handle,
            MAYBE_UNUSED अक्षर स्थिर *path, MAYBE_UNUSED bool recurse)
{ त्रुटि_सं = ENOTSUP; वापस false; }

अटल अंतरभूत पूर्णांक
defaultcon (MAYBE_UNUSED काष्ठा selabel_handle *selabel_handle,
            MAYBE_UNUSED अक्षर स्थिर *path, MAYBE_UNUSED mode_t mode)
{ त्रुटि_सं = ENOTSUP; वापस -1; }

# endif

#पूर्ण_अगर
