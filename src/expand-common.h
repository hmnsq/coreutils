/* expand-common - common functionality for expand/unexpand

   Copyright (C) 1989-2026 Free Software Foundation, Inc.

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

/* Column numbers are nonnegative, with the leftmost column being zero.
   Use a signed type, as that allows for better checking.  */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ intmax_t colno;

/* If true, convert blanks even after nonblank characters have been
   read on the line.  */
ಬಾಹ್ಯ bool convert_entire_ಸಾಲು;

/* The maximum distance between tab stops.  */
ಬಾಹ್ಯ idx_t max_ಸ್ತಂಭ_ಅಗಲ;

/* The desired exit status.  */
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ exit_ಸ್ಥಿತಿ;

/* Add tab stop TABVAL to the end of 'tab_list'.  */
ಬಾಹ್ಯ ಶೂನ್ಯ
ಕೂಡಿಸು_tab_stop (colno tabval);

/* Add the comma or blank separated list of tab stops STOPS
   to the list of tab stops.  */
ಬಾಹ್ಯ ಶೂನ್ಯ
parse_tab_stops (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *stops) _GL_ATTRIBUTE_NONNULL ();

/* Return number of first tab stop after COLUMN.  TAB_INDEX specifies
   many multiple tab-sizes.  Set *LAST_TAB depending on whether we are
   returning COLUMN + 1 merely because we're past the last tab.
   If the number would overflow, diagnose this and exit.  */
ಬಾಹ್ಯ colno
get_ಮುಂದಿನ_tab_ಸ್ತಂಭ (ಸ್ಥಿರಾಂಕ colno ಸ್ತಂಭ, idx_t *tab_ಸೂಚ್ಯಂಕ,
                     bool *ಕೊನೆಯ_tab)
  _GL_ATTRIBUTE_NONNULL ((3));

/* Called after all command-line options have been parsed,
   sets the final tab-stops values */
ಬಾಹ್ಯ ಶೂನ್ಯ
finalize_tab_stops (ಶೂನ್ಯ);




/* Sets new file-list */
ಬಾಹ್ಯ ಶೂನ್ಯ
ಗಣ_file_list (ಅಕ್ಷರ **file_list);

/* Close the old stream pointer FP if it is non-null,
   and return a new one opened to read the next input file.
   Open a filename of '-' as the standard input.
   Return NULL if there are no more input files.  */
ಬಾಹ್ಯ FILE *
ಮುಂದಿನ_file (FILE *fp);

/* Close standard input if we have read from it.  */
ಬಾಹ್ಯ ಶೂನ್ಯ
cleanup_file_list_stdin (ಶೂನ್ಯ);

/* Emit the --help output for --tabs=LIST option accepted by expand and
   unexpand.  */
ಬಾಹ್ಯ ಶೂನ್ಯ
emit_tab_list_info (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಕಾರ್ಯಕ್ರಮ);
