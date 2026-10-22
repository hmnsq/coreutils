/* set-fields.h -- parse field list argument

   Copyright (C) 2015-2026 Free Software Foundation, Inc.

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
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಗಣ_FIELDS_H
# define ಗಣ_FIELDS_H

ರಚನೆ ಕ್ಷೇತ್ರ_range_pair
  {
    uintmax_t lo;
    uintmax_t hi;
  };

/* Array of `struct range_pair' holding all the finite ranges. */
ಬಾಹ್ಯ ರಚನೆ ಕ್ಷೇತ್ರ_range_pair *frp;

/* Number of finite ranges specified by the user. */
ಬಾಹ್ಯ idx_t n_frp;

/* field list parsing options */
ಎಣಿಕೆ
{
  SETFLD_ALLOW_DASH = 0x01,     /* allow single dash meaning 'all fields' */
  SETFLD_COMPLEMENT = 0x02,     /* complement the field list */
  SETFLD_ERRMSG_USE_POS = 0x04  /* when reporting errors, say 'position' instead
                                   of 'field' (used with cut -b/-c) */
};

/* allocates and initializes the FRP array and N_FRP count */
ಬಾಹ್ಯ ಶೂನ್ಯ ಗಣ_fields (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *fieldstr, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ options);

#ಆದರೆ_ಅಂತ್ಯ
