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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ PRINTENV_H
# define PRINTENV_H 1

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
print_envvar (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ನಮೂದು, ಅಕ್ಷರ terminator, bool quoted)
{
  ಆದರೆ (! quoted)
    fputs (ನಮೂದು, stdout);
  ಇಲ್ಲದಿದ್ದರೆ
    {
      idx_t ಸ್ಥಿರಾಂಕ ನಮೂದು_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ನಮೂದು);
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *equal = memchr (ನಮೂದು, '=', ನಮೂದು_len);

      /* If the parent process manipulates ENVIRON directly, it is possible
         that an entry does not contain an equal sign.  */
      idx_t ಸ್ಥಿರಾಂಕ var_len = equal ? equal - ನಮೂದು : ನಮೂದು_len;
      fputs (quoteN_mem (ನಮೂದು, var_len), stdout);

      ಆದರೆ (equal)
        {
          ಅಕ್ಷರ_ಬರೆ ('=');
          ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *val = equal + 1;
          idx_t ಸ್ಥಿರಾಂಕ val_len = ನಮೂದು_len - (val - ನಮೂದು);
          /* Prefer "VAR=" over "VAR=''".  */
          ಆದರೆ (0 < val_len)
            fputs (quoteN_mem (val, val_len), stdout);
        }
    }
  ಅಕ್ಷರ_ಬರೆ (terminator);
}

#ಆದರೆ_ಅಂತ್ಯ
