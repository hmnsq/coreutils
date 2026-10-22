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

#അന്തര്ഭാവ <config.h>

#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "c-ctype.h"
#അന്തര്ഭാവ "fadvise.h"
#അന്തര്ഭാവ "quote.h"

#അന്തര്ഭാവ "expand-common.h"

/* If true, convert blanks even after nonblank characters have been
   read on the line.  */
bool convert_entire_വരി = false;

/* If nonzero, the size of all tab stops.  If zero, use 'tab_list' instead.  */
സ്ഥിതം colno tab_size = 0;

/* If nonzero, the size of all tab stops after the last specified.  */
സ്ഥിതം colno extend_size = 0;

/* If nonzero, an increment for additional tab stops after the last specified.*/
സ്ഥിതം colno increment_size = 0;

/* The maximum distance between tab stops.  */
idx_t max_column_width;

/* Array of the explicit column numbers of the tab stops;
   after 'tab_list' is exhausted, each additional tab is replaced
   by a space.  The first column is column 0.  */
സ്ഥിതം colno *tab_list = ശൂന്യ;

/* The number of allocated entries in 'tab_list'.  */
സ്ഥിതം idx_t n_tabs_allocated = 0;

/* The index of the first invalid element of 'tab_list',
   where the next element can be added.  */
സ്ഥിതം idx_t first_free_tab = 0;

/* Null-terminated array of input filenames.  */
സ്ഥിതം അക്ഷരം **file_list = ശൂന്യ;

/* Default for 'file_list' if no files are given on the command line.  */
സ്ഥിതം അക്ഷരം *stdin_argv[] =
{
  (അക്ഷരം *) "-", ശൂന്യ
};

/* True if we have ever read standard input.  */
സ്ഥിതം bool have_read_stdin = false;

/* The desired exit status.  */
പൂർണ്ണസംഖ്യ exit_status = EXIT_SUCCESS;


സ്ഥിതം ശൂന്യം
set_max_column_width (colno width)
{
  എങ്കിൽ (max_column_width < width)
    {
      എങ്കിൽ (ckd_add (&max_column_width, width, 0))
        error (EXIT_FAILURE, 0, _("tabs are too far apart"));
    }
}

/* Add tab stop TABVAL to the end of 'tab_list'.  */
ബാഹ്യ ശൂന്യം
add_tab_stop (colno tabval)
{
  colno prev_column = first_free_tab ? tab_list[first_free_tab - 1] : 0;
  colno column_width = prev_column <= tabval ? tabval - prev_column : 0;

  എങ്കിൽ (first_free_tab == n_tabs_allocated)
    tab_list = xpalloc (tab_list, &n_tabs_allocated, 1, -1, വലുപ്പം *tab_list);
  tab_list[first_free_tab++] = tabval;

  set_max_column_width (column_width);
}

സ്ഥിതം bool
set_extend_size (colno tabval)
{
  bool ok = true;

  എങ്കിൽ (extend_size)
    {
      error (0, 0,
             _("'/' specifier only allowed"
               " with the last value"));
      ok = false;
    }
  extend_size = tabval;

  set_max_column_width (extend_size);

  തിരികെ ok;
}

സ്ഥിതം bool
set_increment_size (colno tabval)
{
  bool ok = true;

  എങ്കിൽ (increment_size)
    {
      error (0,0,
             _("'+' specifier only allowed"
               " with the last value"));
      ok = false;
    }
  increment_size = tabval;

  set_max_column_width (increment_size);

  തിരികെ ok;
}

/* Add the comma or blank separated list of tab stops STOPS
   to the list of tab stops.  */
ബാഹ്യ ശൂന്യം
parse_tab_stops (അക്ഷരം സ്ഥിരം *stops)
{
  bool have_tabval = false;
  colno tabval = 0;
  bool extend_tabval = false;
  bool increment_tabval = false;
  അക്ഷരം സ്ഥിരം *അക്കം_start = ശൂന്യ;
  bool ok = true;

  വേണ്ടി (; *stops; stops++)
    {
      എങ്കിൽ (*stops == ',' || isblank (to_uchar (*stops)))
        {
          എങ്കിൽ (have_tabval)
            {
              എങ്കിൽ (extend_tabval)
                {
                  എങ്കിൽ (! set_extend_size (tabval))
                    {
                      ok = false;
                      നിർത്തുക;
                    }
                }
              അല്ലെങ്കിൽ എങ്കിൽ (increment_tabval)
                {
                  എങ്കിൽ (! set_increment_size (tabval))
                    {
                      ok = false;
                      നിർത്തുക;
                    }
                }
              അല്ലെങ്കിൽ
                add_tab_stop (tabval);
            }
          have_tabval = false;
        }
      അല്ലെങ്കിൽ എങ്കിൽ (*stops == '/')
        {
          എങ്കിൽ (have_tabval)
            {
              error (0, 0, _("'/' specifier not at start of number: %s"),
                     quote (stops));
              ok = false;
            }
          extend_tabval = true;
          increment_tabval = false;
        }
      അല്ലെങ്കിൽ എങ്കിൽ (*stops == '+')
        {
          എങ്കിൽ (have_tabval)
            {
              error (0, 0, _("'+' specifier not at start of number: %s"),
                     quote (stops));
              ok = false;
            }
          increment_tabval = true;
          extend_tabval = false;
        }
      അല്ലെങ്കിൽ എങ്കിൽ (c_isdigit (*stops))
        {
          എങ്കിൽ (!have_tabval)
            {
              tabval = 0;
              have_tabval = true;
              അക്കം_start = stops;
            }

          /* Detect overflow.  */
          എങ്കിൽ (!DECIMAL_DIGIT_ACCUMULATE (tabval, *stops - '0'))
            {
              idx_t len = strspn (അക്കം_start, "0123456789");
              അക്ഷരം *bad_അക്കം = ximemdup0 (അക്കം_start, len);
              error (0, 0, _("tab stop is too large %s"), quote (bad_അക്കം));
              സ്വതന്ത്രം (bad_അക്കം);
              ok = false;
              stops = അക്കം_start + len - 1;
            }
        }
      അല്ലെങ്കിൽ
        {
          error (0, 0, _("tab size contains invalid character(s): %s"),
                 quote (stops));
          ok = false;
          നിർത്തുക;
        }
    }

  എങ്കിൽ (ok && have_tabval)
    {
      എങ്കിൽ (extend_tabval)
        ok &= set_extend_size (tabval);
      അല്ലെങ്കിൽ എങ്കിൽ (increment_tabval)
        ok &= set_increment_size (tabval);
      അല്ലെങ്കിൽ
        add_tab_stop (tabval);
    }

  എങ്കിൽ (! ok)
    exit (EXIT_FAILURE);
}

/* Check that the list of tab stops TABS, with ENTRIES entries,
   contains only nonzero, ascending values.  */

സ്ഥിതം ശൂന്യം
validate_tab_stops (colno സ്ഥിരം *tabs, idx_t entries)
{
  colno prev_tab = 0;

  വേണ്ടി (idx_t i = 0; i < entries; i++)
    {
      എങ്കിൽ (tabs[i] == 0)
        error (EXIT_FAILURE, 0, _("tab size cannot be 0"));
      എങ്കിൽ (tabs[i] <= prev_tab)
        error (EXIT_FAILURE, 0, _("tab sizes must be ascending"));
      prev_tab = tabs[i];
    }

  എങ്കിൽ (increment_size && extend_size)
    error (EXIT_FAILURE, 0, _("'/' specifier is mutually exclusive with '+'"));
}

/* Called after all command-line options have been parsed,
   and add_tab_stop/parse_tab_stops have been called.
   Will validate the tab-stop values,
   and set the final values to:
   tab-stops = 8 (if no tab-stops given on command line)
   tab-stops = N (if value N specified as the only value).
   tab-stops = distinct values given on command line (if multiple values given).
*/
ബാഹ്യ ശൂന്യം
finalize_tab_stops (ശൂന്യം)
{
  validate_tab_stops (tab_list, first_free_tab);

  എങ്കിൽ (first_free_tab == 0)
    tab_size = max_column_width = extend_size
                                  ? extend_size : increment_size
                                                  ? increment_size : 8;
  അല്ലെങ്കിൽ എങ്കിൽ (first_free_tab == 1 && ! extend_size && ! increment_size)
    tab_size = tab_list[0];
  അല്ലെങ്കിൽ
    tab_size = 0;
}


/* Return number of first tab stop after COLUMN.  TAB_INDEX specifies
   many multiple tab-sizes.  Set *LAST_TAB depending on whether we are
   returning COLUMN + 1 merely because we're past the last tab.
   If the number would overflow, diagnose this and exit.  */
ബാഹ്യ colno
get_next_tab_column (colno column, idx_t *tab_index, bool *last_tab)
{
  *last_tab = false;
  colno tab_distance;

  /* single tab-size - return multiples of it */
  എങ്കിൽ (tab_size)
    tab_distance = tab_size - column % tab_size;
  അല്ലെങ്കിൽ
    {
      /* multiple tab-sizes - iterate them until the tab position is beyond
         the current input column. */
      വേണ്ടി ( ; *tab_index < first_free_tab ; (*tab_index)++ )
        {
          colno tab = tab_list[*tab_index];
          എങ്കിൽ (column < tab)
            തിരികെ tab;
        }

      /* relative last tab - return multiples of it */
      എങ്കിൽ (extend_size)
        tab_distance = extend_size - column % extend_size;
      അല്ലെങ്കിൽ എങ്കിൽ (increment_size)
        {
          /* incremental last tab - add increment_size to the previous
             tab stop */
          colno end_tab = tab_list[first_free_tab - 1];
          tab_distance = increment_size - ((column - end_tab) % increment_size);
        }
      അല്ലെങ്കിൽ
        {
          *last_tab = true;
          tab_distance = 1;
        }
    }

  colno tab_stop;
  എങ്കിൽ (ckd_add (&tab_stop, column, tab_distance))
    error (EXIT_FAILURE, 0, _("input line is too long"));
  തിരികെ tab_stop;
}


/* Sets new file-list */
ബാഹ്യ ശൂന്യം
set_file_list (അക്ഷരം **list)
{
  have_read_stdin = false;

  എങ്കിൽ (!list)
    file_list = stdin_argv;
  അല്ലെങ്കിൽ
    file_list = list;
}

/* Close the old stream pointer FP if it is non-null,
   and return a new one opened to read the next input file.
   Open a filename of '-' as the standard input.
   Return NULL if there are no more input files.  */

ബാഹ്യ FILE *
next_file (FILE *fp)
{
  സ്ഥിതം അക്ഷരം *prev_file;
  അക്ഷരം *file;

  എങ്കിൽ (fp)
    {
      പൂർണ്ണസംഖ്യ err = errno;
      എങ്കിൽ (!ferror (fp))
        err = 0;
      എങ്കിൽ (streq (prev_file, "-"))
        clearerr (fp);		/* Also clear EOF.  */
      അല്ലെങ്കിൽ എങ്കിൽ (fclose (fp) != 0)
        err = errno;
      എങ്കിൽ (err)
        {
          error (0, err, "%s", quotef (prev_file));
          exit_status = EXIT_FAILURE;
        }
    }

  ആകുമ്പോൾ ((file = *file_list++) != ശൂന്യ)
    {
      എങ്കിൽ (streq (file, "-"))
        {
          have_read_stdin = true;
          fp = stdin;
        }
      അല്ലെങ്കിൽ
        fp = fopen (file, "r");
      എങ്കിൽ (fp)
        {
          prev_file = file;
          fadvise (fp, FADVISE_SEQUENTIAL);
          തിരികെ fp;
        }
      error (0, errno, "%s", quotef (file));
      exit_status = EXIT_FAILURE;
    }
  തിരികെ ശൂന്യ;
}

/* Close standard input if we have read from it.  */
ബാഹ്യ ശൂന്യം
cleanup_file_list_stdin (ശൂന്യം)
{
    എങ്കിൽ (have_read_stdin && fclose (stdin) != 0)
      error (EXIT_FAILURE, errno, "-");
}

/* Emit the --help output for --tabs=LIST option accepted by expand and
   unexpand.  */
ബാഹ്യ ശൂന്യം
emit_tab_list_info (അക്ഷരം സ്ഥിരം *program)
{
  /* suppress syntax check for emit_mandatory_arg_note() */
  oputs_ (program, _("\
  -t, --tabs=LIST\n\
         use comma separated list of tab positions.\n\
"));
  fputs (_("\
         The last specified position can be prefixed with '/'\n\
         to specify a tab size to use after the last\n\
         explicitly specified tab stop.  Also a prefix of '+'\n\
         can be used to align remaining tab stops relative to\n\
         the last specified tab stop instead of the first column\n\
"), stdout);
}
