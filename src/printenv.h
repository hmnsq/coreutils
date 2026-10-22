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

સ્થિર અંતઃરેખ શૂન્ય
print_envvar (અક્ષર અચલ *entry, અક્ષર terminator, bool quoted)
{
  જો (! quoted)
    fputs (entry, stdout);
  નહીંતર
    {
      idx_t અચલ entry_len = strlen (entry);
      અક્ષર અચલ *equal = memchr (entry, '=', entry_len);

      /* If the parent process manipulates ENVIRON directly, it is possible
         that an entry does not contain an equal sign.  */
      idx_t અચલ var_len = equal ? equal - entry : entry_len;
      fputs (quoteN_mem (entry, var_len), stdout);

      જો (equal)
        {
          putchar ('=');
          અક્ષર અચલ *val = equal + 1;
          idx_t અચલ val_len = entry_len - (val - entry);
          /* Prefer "VAR=" over "VAR=''".  */
          જો (0 < val_len)
            fputs (quoteN_mem (val, val_len), stdout);
        }
    }
  putchar (terminator);
}

#endif
