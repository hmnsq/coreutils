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

स्थिर ओळीत शून्य
print_envvar (अक्षर स्थिरांक *entry, अक्षर terminator, bool quoted)
{
  जर (! quoted)
    ख_माला_दो (entry, मानक_निकास);
  अन्यथा
    {
      idx_t स्थिरांक entry_len = म_माप (entry);
      अक्षर स्थिरांक *equal = स_प्रथम (entry, '=', entry_len);

      /* If the parent process manipulates ENVIRON directly, it is possible
         that an entry does not contain an equal sign.  */
      idx_t स्थिरांक var_len = equal ? equal - entry : entry_len;
      ख_माला_दो (quoteN_mem (entry, var_len), मानक_निकास);

      जर (equal)
        {
          अक्षर_दो ('=');
          अक्षर स्थिरांक *val = equal + 1;
          idx_t स्थिरांक val_len = entry_len - (val - entry);
          /* Prefer "VAR=" over "VAR=''".  */
          जर (0 < val_len)
            ख_माला_दो (quoteN_mem (val, val_len), मानक_निकास);
        }
    }
  अक्षर_दो (terminator);
}

#endif
