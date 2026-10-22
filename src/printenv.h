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

#నిర్వచితం_కాకపోతే PRINTENV_H
# define PRINTENV_H 1

స్థిర అంతఃవరుస శూన్య
print_envvar (అక్షరం స్థిరరాశి *నమోదు_2, అక్షరం terminator, bool quoted)
{
  ఒకవేళ (! quoted)
    fputs (నమోదు_2, stdout);
  లేకపోతే
    {
      idx_t స్థిరరాశి నమోదు_len = అక్షరశ్రేణి_పొడవు (నమోదు_2);
      అక్షరం స్థిరరాశి *equal = memchr (నమోదు_2, '=', నమోదు_len);

      /* If the parent process manipulates ENVIRON directly, it is possible
         that an entry does not contain an equal sign.  */
      idx_t స్థిరరాశి var_len = equal ? equal - నమోదు_2 : నమోదు_len;
      fputs (quoteN_mem (నమోదు_2, var_len), stdout);

      ఒకవేళ (equal)
        {
          అక్షరం_రాయి ('=');
          అక్షరం స్థిరరాశి *val = equal + 1;
          idx_t స్థిరరాశి val_len = నమోదు_len - (val - నమోదు_2);
          /* Prefer "VAR=" over "VAR=''".  */
          ఒకవేళ (0 < val_len)
            fputs (quoteN_mem (val, val_len), stdout);
        }
    }
  అక్షరం_రాయి (terminator);
}

#అయితే_ముగింపు
