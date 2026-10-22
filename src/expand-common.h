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
প্রকারলেখ intmax_t colno;

/* If true, convert blanks even after nonblank characters have been
   read on the line.  */
বাহ্যিক bool convert_entire_সারি;

/* The maximum distance between tab stops.  */
বাহ্যিক idx_t max_column_width;

/* The desired exit status.  */
বাহ্যিক পূর্ণসংখ্যা exit_status;

/* Add tab stop TABVAL to the end of 'tab_list'.  */
বাহ্যিক শূন্য
add_tab_stop (colno tabval);

/* Add the comma or blank separated list of tab stops STOPS
   to the list of tab stops.  */
বাহ্যিক শূন্য
parse_tab_stops (অক্ষর ধ্রুবক *stops) _GL_ATTRIBUTE_NONNULL ();

/* Return number of first tab stop after COLUMN.  TAB_INDEX specifies
   many multiple tab-sizes.  Set *LAST_TAB depending on whether we are
   returning COLUMN + 1 merely because we're past the last tab.
   If the number would overflow, diagnose this and exit.  */
বাহ্যিক colno
get_পরবর্তী_tab_column (ধ্রুবক colno column, idx_t *tab_সূচক,
                     bool *last_tab)
  _GL_ATTRIBUTE_NONNULL ((3));

/* Called after all command-line options have been parsed,
   sets the final tab-stops values */
বাহ্যিক শূন্য
finalize_tab_stops (শূন্য);




/* Sets new file-list */
বাহ্যিক শূন্য
set_file_list (অক্ষর **file_list);

/* Close the old stream pointer FP if it is non-null,
   and return a new one opened to read the next input file.
   Open a filename of '-' as the standard input.
   Return NULL if there are no more input files.  */
বাহ্যিক FILE *
পরবর্তী_file (FILE *fp);

/* Close standard input if we have read from it.  */
বাহ্যিক শূন্য
cleanup_file_list_stdin (শূন্য);

/* Emit the --help output for --tabs=LIST option accepted by expand and
   unexpand.  */
বাহ্যিক শূন্য
emit_tab_list_info (অক্ষর ধ্রুবক *program);
