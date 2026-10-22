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

#اگر_غیر_تعریف_شدہ PRINTENV_H
# define PRINTENV_H 1

ساکن اندرونی خالی
print_envvar (حرف ثابت *اندراج_2, حرف terminator, bool quoted)
{
  اگر (! quoted)
    fputs (اندراج_2, stdout);
  ورنہ
    {
      idx_t ثابت اندراج_len = لڑی_لمبائی (اندراج_2);
      حرف ثابت *equal = memchr (اندراج_2, '=', اندراج_len);

      /* If the parent process manipulates ENVIRON directly, it is possible
         that an entry does not contain an equal sign.  */
      idx_t ثابت var_len = equal ? equal - اندراج_2 : اندراج_len;
      fputs (quoteN_mem (اندراج_2, var_len), stdout);

      اگر (equal)
        {
          حرف_لکھو ('=');
          حرف ثابت *val = equal + 1;
          idx_t ثابت val_len = اندراج_len - (val - اندراج_2);
          /* Prefer "VAR=" over "VAR=''".  */
          اگر (0 < val_len)
            fputs (quoteN_mem (val, val_len), stdout);
        }
    }
  حرف_لکھو (terminator);
}

#اگر_ختم
