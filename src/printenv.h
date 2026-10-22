/* Common definitions for 'printenv' and 'env'
   Copyright (C) 2026 Free Software Foundation, Inc.

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

#ifndef PRINTENV_H
# define PRINTENV_H 1

स्थिर अन्तःस्थ शून्य
print_envvar (वर्ण अचर *entry, वर्ण terminator, bool quoted)
{
  यदि (! quoted)
    ख_माला_दो (entry, मानक_निकास);
  अन्यथा
    {
      idx_t अचर entry_len = म_माप (entry);
      वर्ण अचर *equal = स_प्रथम (entry, '=', entry_len);

      /* If the parent process manipulates ENVIRON directly, it is possible
         that an entry does not contain an equal sign.  */
      idx_t अचर var_len = equal ? equal - entry : entry_len;
      ख_माला_दो (quoteN_mem (entry, var_len), मानक_निकास);

      यदि (equal)
        {
          अक्षर_दो ('=');
          वर्ण अचर *val = equal + 1;
          idx_t अचर val_len = entry_len - (val - entry);
          /* Prefer "VAR=" over "VAR=''".  */
          यदि (0 < val_len)
            ख_माला_दो (quoteN_mem (val, val_len), मानक_निकास);
        }
    }
  अक्षर_दो (terminator);
}

#endif
