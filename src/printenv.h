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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ PRINTENV_H
# define PRINTENV_H 1

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
print_envvar (ਅੱਖਰ ਅਚਲ *ਇੰਦਰਾਜ, ਅੱਖਰ terminator, bool quoted)
{
  ਜੇ (! quoted)
    fputs (ਇੰਦਰਾਜ, stdout);
  ਨਹੀਂਤਾਂ
    {
      idx_t ਅਚਲ ਇੰਦਰਾਜ_len = ਲੜੀ_ਲੰਬਾਈ (ਇੰਦਰਾਜ);
      ਅੱਖਰ ਅਚਲ *equal = memchr (ਇੰਦਰਾਜ, '=', ਇੰਦਰਾਜ_len);

      /* If the parent process manipulates ENVIRON directly, it is possible
         that an entry does not contain an equal sign.  */
      idx_t ਅਚਲ var_len = equal ? equal - ਇੰਦਰਾਜ : ਇੰਦਰਾਜ_len;
      fputs (quoteN_mem (ਇੰਦਰਾਜ, var_len), stdout);

      ਜੇ (equal)
        {
          ਅੱਖਰ_ਲਿਖੋ ('=');
          ਅੱਖਰ ਅਚਲ *val = equal + 1;
          idx_t ਅਚਲ val_len = ਇੰਦਰਾਜ_len - (val - ਇੰਦਰਾਜ);
          /* Prefer "VAR=" over "VAR=''".  */
          ਜੇ (0 < val_len)
            fputs (quoteN_mem (val, val_len), stdout);
        }
    }
  ਅੱਖਰ_ਲਿਖੋ (terminator);
}

#ਜੇ_ਅੰਤ
