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
تعريفنوع intmax_t colno;

/* If true, convert blanks even after nonblank characters have been
   read on the line.  */
خارجي bool convert_entire_لي;

/* The maximum distance between tab stops.  */
خارجي idx_t max_column_width;

/* The desired exit status.  */
خارجي صحيح exit_status;

/* Add tab stop TABVAL to the end of 'tab_list'.  */
خارجي فراغ
add_tab_stop (colno tabval);

/* Add the comma or blank separated list of tab stops STOPS
   to the list of tab stops.  */
خارجي فراغ
parse_tab_stops (حرف ثابت *stops) _GL_ATTRIBUTE_NONNULL ();

/* Return number of first tab stop after COLUMN.  TAB_INDEX specifies
   many multiple tab-sizes.  Set *LAST_TAB depending on whether we are
   returning COLUMN + 1 merely because we're past the last tab.
   If the number would overflow, diagnose this and exit.  */
خارجي colno
get_next_tab_column (ثابت colno column, idx_t *tab_index,
                     bool *last_tab)
  _GL_ATTRIBUTE_NONNULL ((3));

/* Called after all command-line options have been parsed,
   sets the final tab-stops values */
خارجي فراغ
finalize_tab_stops (فراغ);




/* Sets new file-list */
خارجي فراغ
set_file_list (حرف **file_list);

/* Close the old stream pointer FP if it is non-null,
   and return a new one opened to read the next input file.
   Open a filename of '-' as the standard input.
   Return NULL if there are no more input files.  */
خارجي FILE *
next_file (FILE *fp);

/* Close standard input if we have read from it.  */
خارجي فراغ
cleanup_file_list_stdin (فراغ);

/* Emit the --help output for --tabs=LIST option accepted by expand and
   unexpand.  */
خارجي فراغ
emit_tab_list_info (حرف ثابت *program);
