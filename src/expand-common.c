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

#ಅನ್ತರ್ಭಾವ <config.h>

#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "c-ctype.h"
#ಅನ್ತರ್ಭಾವ "fadvise.h"
#ಅನ್ತರ್ಭಾವ "quote.h"

#ಅನ್ತರ್ಭಾವ "expand-common.h"

/* If true, convert blanks even after nonblank characters have been
   read on the line.  */
bool convert_entire_ಸಾಲು = false;

/* If nonzero, the size of all tab stops.  If zero, use 'tab_list' instead.  */
ಸ್ಥಿರ colno tab_size = 0;

/* If nonzero, the size of all tab stops after the last specified.  */
ಸ್ಥಿರ colno extend_size = 0;

/* If nonzero, an increment for additional tab stops after the last specified.*/
ಸ್ಥಿರ colno increment_size = 0;

/* The maximum distance between tab stops.  */
idx_t max_column_width;

/* Array of the explicit column numbers of the tab stops;
   after 'tab_list' is exhausted, each additional tab is replaced
   by a space.  The first column is column 0.  */
ಸ್ಥಿರ colno *tab_list = ರಿಕ್ತ;

/* The number of allocated entries in 'tab_list'.  */
ಸ್ಥಿರ idx_t n_tabs_allocated = 0;

/* The index of the first invalid element of 'tab_list',
   where the next element can be added.  */
ಸ್ಥಿರ idx_t first_free_tab = 0;

/* Null-terminated array of input filenames.  */
ಸ್ಥಿರ ವರ್ಣ **file_list = ರಿಕ್ತ;

/* Default for 'file_list' if no files are given on the command line.  */
ಸ್ಥಿರ ವರ್ಣ *stdin_argv[] =
{
  (ವರ್ಣ *) "-", ರಿಕ್ತ
};

/* True if we have ever read standard input.  */
ಸ್ಥಿರ bool have_read_stdin = false;

/* The desired exit status.  */
ಪೂರ್ಣಾಙ್ಕ exit_status = EXIT_SUCCESS;


ಸ್ಥಿರ ಶೂನ್ಯ
set_max_column_width (colno width)
{
  ಯದಿ (max_column_width < width)
    {
      ಯದಿ (ckd_add (&max_column_width, width, 0))
        error (EXIT_FAILURE, 0, _("tabs are too far apart"));
    }
}

/* Add tab stop TABVAL to the end of 'tab_list'.  */
ಬಾಹ್ಯ ಶೂನ್ಯ
add_tab_stop (colno tabval)
{
  colno prev_column = first_free_tab ? tab_list[first_free_tab - 1] : 0;
  colno column_width = prev_column <= tabval ? tabval - prev_column : 0;

  ಯದಿ (first_free_tab == n_tabs_allocated)
    tab_list = xpalloc (tab_list, &n_tabs_allocated, 1, -1, ಪರಿಮಾಣ *tab_list);
  tab_list[first_free_tab++] = tabval;

  set_max_column_width (column_width);
}

ಸ್ಥಿರ bool
set_extend_size (colno tabval)
{
  bool ok = true;

  ಯದಿ (extend_size)
    {
      error (0, 0,
             _("'/' specifier only allowed"
               " with the last value"));
      ok = false;
    }
  extend_size = tabval;

  set_max_column_width (extend_size);

  ಪ್ರತ್ಯಾಗಮ ok;
}

ಸ್ಥಿರ bool
set_increment_size (colno tabval)
{
  bool ok = true;

  ಯದಿ (increment_size)
    {
      error (0,0,
             _("'+' specifier only allowed"
               " with the last value"));
      ok = false;
    }
  increment_size = tabval;

  set_max_column_width (increment_size);

  ಪ್ರತ್ಯಾಗಮ ok;
}

/* Add the comma or blank separated list of tab stops STOPS
   to the list of tab stops.  */
ಬಾಹ್ಯ ಶೂನ್ಯ
parse_tab_stops (ವರ್ಣ ಧ್ರುವ *stops)
{
  bool have_tabval = false;
  colno tabval = 0;
  bool extend_tabval = false;
  bool increment_tabval = false;
  ವರ್ಣ ಧ್ರುವ *ಅಂಕೆ_start = ರಿಕ್ತ;
  bool ok = true;

  ಪ್ರತಿ (; *stops; stops++)
    {
      ಯದಿ (*stops == ',' || isblank (to_uchar (*stops)))
        {
          ಯದಿ (have_tabval)
            {
              ಯದಿ (extend_tabval)
                {
                  ಯದಿ (! set_extend_size (tabval))
                    {
                      ok = false;
                      ಭಙ್ಗ;
                    }
                }
              ಅನ್ಯಥಾ ಯದಿ (increment_tabval)
                {
                  ಯದಿ (! set_increment_size (tabval))
                    {
                      ok = false;
                      ಭಙ್ಗ;
                    }
                }
              ಅನ್ಯಥಾ
                add_tab_stop (tabval);
            }
          have_tabval = false;
        }
      ಅನ್ಯಥಾ ಯದಿ (*stops == '/')
        {
          ಯದಿ (have_tabval)
            {
              error (0, 0, _("'/' specifier not at start of number: %s"),
                     quote (stops));
              ok = false;
            }
          extend_tabval = true;
          increment_tabval = false;
        }
      ಅನ್ಯಥಾ ಯದಿ (*stops == '+')
        {
          ಯದಿ (have_tabval)
            {
              error (0, 0, _("'+' specifier not at start of number: %s"),
                     quote (stops));
              ok = false;
            }
          increment_tabval = true;
          extend_tabval = false;
        }
      ಅನ್ಯಥಾ ಯದಿ (c_isdigit (*stops))
        {
          ಯದಿ (!have_tabval)
            {
              tabval = 0;
              have_tabval = true;
              ಅಂಕೆ_start = stops;
            }

          /* Detect overflow.  */
          ಯದಿ (!DECIMAL_DIGIT_ACCUMULATE (tabval, *stops - '0'))
            {
              idx_t len = strspn (ಅಂಕೆ_start, "0123456789");
              ವರ್ಣ *bad_ಅಂಕೆ = ximemdup0 (ಅಂಕೆ_start, len);
              error (0, 0, _("tab stop is too large %s"), quote (bad_ಅಂಕೆ));
              ಮುಕ್ತ (bad_ಅಂಕೆ);
              ok = false;
              stops = ಅಂಕೆ_start + len - 1;
            }
        }
      ಅನ್ಯಥಾ
        {
          error (0, 0, _("tab size contains invalid character(s): %s"),
                 quote (stops));
          ok = false;
          ಭಙ್ಗ;
        }
    }

  ಯದಿ (ok && have_tabval)
    {
      ಯದಿ (extend_tabval)
        ok &= set_extend_size (tabval);
      ಅನ್ಯಥಾ ಯದಿ (increment_tabval)
        ok &= set_increment_size (tabval);
      ಅನ್ಯಥಾ
        add_tab_stop (tabval);
    }

  ಯದಿ (! ok)
    exit (EXIT_FAILURE);
}

/* Check that the list of tab stops TABS, with ENTRIES entries,
   contains only nonzero, ascending values.  */

ಸ್ಥಿರ ಶೂನ್ಯ
validate_tab_stops (colno ಧ್ರುವ *tabs, idx_t entries)
{
  colno prev_tab = 0;

  ಪ್ರತಿ (idx_t i = 0; i < entries; i++)
    {
      ಯದಿ (tabs[i] == 0)
        error (EXIT_FAILURE, 0, _("tab size cannot be 0"));
      ಯದಿ (tabs[i] <= prev_tab)
        error (EXIT_FAILURE, 0, _("tab sizes must be ascending"));
      prev_tab = tabs[i];
    }

  ಯದಿ (increment_size && extend_size)
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
ಬಾಹ್ಯ ಶೂನ್ಯ
finalize_tab_stops (ಶೂನ್ಯ)
{
  validate_tab_stops (tab_list, first_free_tab);

  ಯದಿ (first_free_tab == 0)
    tab_size = max_column_width = extend_size
                                  ? extend_size : increment_size
                                                  ? increment_size : 8;
  ಅನ್ಯಥಾ ಯದಿ (first_free_tab == 1 && ! extend_size && ! increment_size)
    tab_size = tab_list[0];
  ಅನ್ಯಥಾ
    tab_size = 0;
}


/* Return number of first tab stop after COLUMN.  TAB_INDEX specifies
   many multiple tab-sizes.  Set *LAST_TAB depending on whether we are
   returning COLUMN + 1 merely because we're past the last tab.
   If the number would overflow, diagnose this and exit.  */
ಬಾಹ್ಯ colno
get_next_tab_column (colno column, idx_t *tab_index, bool *last_tab)
{
  *last_tab = false;
  colno tab_distance;

  /* single tab-size - return multiples of it */
  ಯದಿ (tab_size)
    tab_distance = tab_size - column % tab_size;
  ಅನ್ಯಥಾ
    {
      /* multiple tab-sizes - iterate them until the tab position is beyond
         the current input column. */
      ಪ್ರತಿ ( ; *tab_index < first_free_tab ; (*tab_index)++ )
        {
          colno tab = tab_list[*tab_index];
          ಯದಿ (column < tab)
            ಪ್ರತ್ಯಾಗಮ tab;
        }

      /* relative last tab - return multiples of it */
      ಯದಿ (extend_size)
        tab_distance = extend_size - column % extend_size;
      ಅನ್ಯಥಾ ಯದಿ (increment_size)
        {
          /* incremental last tab - add increment_size to the previous
             tab stop */
          colno end_tab = tab_list[first_free_tab - 1];
          tab_distance = increment_size - ((column - end_tab) % increment_size);
        }
      ಅನ್ಯಥಾ
        {
          *last_tab = true;
          tab_distance = 1;
        }
    }

  colno tab_stop;
  ಯದಿ (ckd_add (&tab_stop, column, tab_distance))
    error (EXIT_FAILURE, 0, _("input line is too long"));
  ಪ್ರತ್ಯಾಗಮ tab_stop;
}


/* Sets new file-list */
ಬಾಹ್ಯ ಶೂನ್ಯ
set_file_list (ವರ್ಣ **list)
{
  have_read_stdin = false;

  ಯದಿ (!list)
    file_list = stdin_argv;
  ಅನ್ಯಥಾ
    file_list = list;
}

/* Close the old stream pointer FP if it is non-null,
   and return a new one opened to read the next input file.
   Open a filename of '-' as the standard input.
   Return NULL if there are no more input files.  */

ಬಾಹ್ಯ FILE *
next_file (FILE *fp)
{
  ಸ್ಥಿರ ವರ್ಣ *prev_file;
  ವರ್ಣ *file;

  ಯದಿ (fp)
    {
      ಪೂರ್ಣಾಙ್ಕ err = errno;
      ಯದಿ (!ferror (fp))
        err = 0;
      ಯದಿ (streq (prev_file, "-"))
        clearerr (fp);		/* Also clear EOF.  */
      ಅನ್ಯಥಾ ಯದಿ (fclose (fp) != 0)
        err = errno;
      ಯದಿ (err)
        {
          error (0, err, "%s", quotef (prev_file));
          exit_status = EXIT_FAILURE;
        }
    }

  ಯಾವತ್ ((file = *file_list++) != ರಿಕ್ತ)
    {
      ಯದಿ (streq (file, "-"))
        {
          have_read_stdin = true;
          fp = stdin;
        }
      ಅನ್ಯಥಾ
        fp = fopen (file, "r");
      ಯದಿ (fp)
        {
          prev_file = file;
          fadvise (fp, FADVISE_SEQUENTIAL);
          ಪ್ರತ್ಯಾಗಮ fp;
        }
      error (0, errno, "%s", quotef (file));
      exit_status = EXIT_FAILURE;
    }
  ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

/* Close standard input if we have read from it.  */
ಬಾಹ್ಯ ಶೂನ್ಯ
cleanup_file_list_stdin (ಶೂನ್ಯ)
{
    ಯದಿ (have_read_stdin && fclose (stdin) != 0)
      error (EXIT_FAILURE, errno, "-");
}

/* Emit the --help output for --tabs=LIST option accepted by expand and
   unexpand.  */
ಬಾಹ್ಯ ಶೂನ್ಯ
emit_tab_list_info (ವರ್ಣ ಧ್ರುವ *program)
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
