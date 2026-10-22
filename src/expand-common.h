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
قسمتعریف intmax_t colno;

/* If true, convert blanks even after nonblank characters have been
   read on the line.  */
خارجی bool convert_entire_سطر;

/* The maximum distance between tab stops.  */
خارجی idx_t max_عمود_چوڑائی;

/* The desired exit status.  */
خارجی صحیح exit_حالت;

/* Add tab stop TABVAL to the end of 'tab_list'.  */
خارجی خالی
جوڑو_tab_stop (colno tabval);

/* Add the comma or blank separated list of tab stops STOPS
   to the list of tab stops.  */
خارجی خالی
parse_tab_stops (حرف ثابت *stops) _GL_ATTRIBUTE_NONNULL ();

/* Return number of first tab stop after COLUMN.  TAB_INDEX specifies
   many multiple tab-sizes.  Set *LAST_TAB depending on whether we are
   returning COLUMN + 1 merely because we're past the last tab.
   If the number would overflow, diagnose this and exit.  */
خارجی colno
get_اگلا_tab_عمود (ثابت colno عمود, idx_t *tab_اشاریہ,
                     bool *آخری_tab)
  _GL_ATTRIBUTE_NONNULL ((3));

/* Called after all command-line options have been parsed,
   sets the final tab-stops values */
خارجی خالی
finalize_tab_stops (خالی);




/* Sets new file-list */
خارجی خالی
سیٹ_file_list (حرف **file_list);

/* Close the old stream pointer FP if it is non-null,
   and return a new one opened to read the next input file.
   Open a filename of '-' as the standard input.
   Return NULL if there are no more input files.  */
خارجی FILE *
اگلا_file (FILE *fp);

/* Close standard input if we have read from it.  */
خارجی خالی
cleanup_file_list_stdin (خالی);

/* Emit the --help output for --tabs=LIST option accepted by expand and
   unexpand.  */
خارجی خالی
emit_tab_list_info (حرف ثابت *پروگرام);
