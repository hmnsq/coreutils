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
પ્રકારવ્યાખ્યા intmax_t colno;

/* If true, convert blanks even after nonblank characters have been
   read on the line.  */
બાહ્ય bool convert_entire_લીટી;

/* The maximum distance between tab stops.  */
બાહ્ય idx_t max_column_width;

/* The desired exit status.  */
બાહ્ય પૂર્ણાંક exit_status;

/* Add tab stop TABVAL to the end of 'tab_list'.  */
બાહ્ય શૂન્ય
add_tab_stop (colno tabval);

/* Add the comma or blank separated list of tab stops STOPS
   to the list of tab stops.  */
બાહ્ય શૂન્ય
parse_tab_stops (અક્ષર અચલ *stops) _GL_ATTRIBUTE_NONNULL ();

/* Return number of first tab stop after COLUMN.  TAB_INDEX specifies
   many multiple tab-sizes.  Set *LAST_TAB depending on whether we are
   returning COLUMN + 1 merely because we're past the last tab.
   If the number would overflow, diagnose this and exit.  */
બાહ્ય colno
get_next_tab_column (અચલ colno column, idx_t *tab_index,
                     bool *last_tab)
  _GL_ATTRIBUTE_NONNULL ((3));

/* Called after all command-line options have been parsed,
   sets the final tab-stops values */
બાહ્ય શૂન્ય
finalize_tab_stops (શૂન્ય);




/* Sets new file-list */
બાહ્ય શૂન્ય
set_file_list (અક્ષર **file_list);

/* Close the old stream pointer FP if it is non-null,
   and return a new one opened to read the next input file.
   Open a filename of '-' as the standard input.
   Return NULL if there are no more input files.  */
બાહ્ય FILE *
next_file (FILE *fp);

/* Close standard input if we have read from it.  */
બાહ્ય શૂન્ય
cleanup_file_list_stdin (શૂન્ય);

/* Emit the --help output for --tabs=LIST option accepted by expand and
   unexpand.  */
બાહ્ય શૂન્ય
emit_tab_list_info (અક્ષર અચલ *program);
