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

#اگر_تعریف_نشده PRINTENV_H
# define PRINTENV_H 1

ساكن مضمن فراغ
print_envvar (حرف ثابت *مدخل, حرف terminator, bool quoted)
{
  إذا (! quoted)
    fputs (مدخل, stdout);
  وإلا
    {
      idx_t ثابت مدخل_len = طول_رشته (مدخل);
      حرف ثابت *equal = memchr (مدخل, '=', مدخل_len);

      /* If the parent process manipulates ENVIRON directly, it is possible
         that an entry does not contain an equal sign.  */
      idx_t ثابت var_len = equal ? equal - مدخل : مدخل_len;
      fputs (quoteN_mem (مدخل, var_len), stdout);

      إذا (equal)
        {
          حرف_چاپ_کن ('=');
          حرف ثابت *val = equal + 1;
          idx_t ثابت val_len = مدخل_len - (val - مدخل);
          /* Prefer "VAR=" over "VAR=''".  */
          إذا (0 < val_len)
            fputs (quoteN_mem (val, val_len), stdout);
        }
    }
  حرف_چاپ_کن (terminator);
}

#پایان_اگر
