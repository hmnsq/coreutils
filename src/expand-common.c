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

#অন্তর্ভাৱ <config.h>

#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <sys/types.h>
#অন্তর্ভাৱ "system.h"
#অন্তর্ভাৱ "c-ctype.h"
#অন্তর্ভাৱ "fadvise.h"
#অন্তর্ভাৱ "quote.h"

#অন্তর্ভাৱ "expand-common.h"

/* If true, convert blanks even after nonblank characters have been
   read on the line.  */
bool convert_entire_সাৰি = false;

/* If nonzero, the size of all tab stops.  If zero, use 'tab_list' instead.  */
স্থির colno tab_size = 0;

/* If nonzero, the size of all tab stops after the last specified.  */
স্থির colno extend_size = 0;

/* If nonzero, an increment for additional tab stops after the last specified.*/
স্থির colno increment_size = 0;

/* The maximum distance between tab stops.  */
idx_t max_column_width;

/* Array of the explicit column numbers of the tab stops;
   after 'tab_list' is exhausted, each additional tab is replaced
   by a space.  The first column is column 0.  */
স্থির colno *tab_list = রিক্ত;

/* The number of allocated entries in 'tab_list'.  */
স্থির idx_t n_tabs_allocated = 0;

/* The index of the first invalid element of 'tab_list',
   where the next element can be added.  */
স্থির idx_t first_free_tab = 0;

/* Null-terminated array of input filenames.  */
স্থির ৱর্ণ **file_list = রিক্ত;

/* Default for 'file_list' if no files are given on the command line.  */
স্থির ৱর্ণ *stdin_argv[] =
{
  (ৱর্ণ *) "-", রিক্ত
};

/* True if we have ever read standard input.  */
স্থির bool have_read_stdin = false;

/* The desired exit status.  */
পূর্ণাঙ্ক exit_status = EXIT_SUCCESS;


স্থির শূন্য
set_max_column_width (colno width)
{
  যদি (max_column_width < width)
    {
      যদি (ckd_add (&max_column_width, width, 0))
        error (EXIT_FAILURE, 0, _("tabs are too far apart"));
    }
}

/* Add tab stop TABVAL to the end of 'tab_list'.  */
বাহ্য শূন্য
add_tab_stop (colno tabval)
{
  colno prev_column = first_free_tab ? tab_list[first_free_tab - 1] : 0;
  colno column_width = prev_column <= tabval ? tabval - prev_column : 0;

  যদি (first_free_tab == n_tabs_allocated)
    tab_list = xpalloc (tab_list, &n_tabs_allocated, 1, -1, পরিমাণ *tab_list);
  tab_list[first_free_tab++] = tabval;

  set_max_column_width (column_width);
}

স্থির bool
set_extend_size (colno tabval)
{
  bool ok = true;

  যদি (extend_size)
    {
      error (0, 0,
             _("'/' specifier only allowed"
               " with the last value"));
      ok = false;
    }
  extend_size = tabval;

  set_max_column_width (extend_size);

  ঘূৰাই ok;
}

স্থির bool
set_increment_size (colno tabval)
{
  bool ok = true;

  যদি (increment_size)
    {
      error (0,0,
             _("'+' specifier only allowed"
               " with the last value"));
      ok = false;
    }
  increment_size = tabval;

  set_max_column_width (increment_size);

  ঘূৰাই ok;
}

/* Add the comma or blank separated list of tab stops STOPS
   to the list of tab stops.  */
বাহ্য শূন্য
parse_tab_stops (ৱর্ণ ধ্রুৱ *stops)
{
  bool have_tabval = false;
  colno tabval = 0;
  bool extend_tabval = false;
  bool increment_tabval = false;
  ৱর্ণ ধ্রুৱ *অঙ্ক_start = রিক্ত;
  bool ok = true;

  বাবে (; *stops; stops++)
    {
      যদি (*stops == ',' || isblank (to_uchar (*stops)))
        {
          যদি (have_tabval)
            {
              যদি (extend_tabval)
                {
                  যদি (! set_extend_size (tabval))
                    {
                      ok = false;
                      ভঙ্গ;
                    }
                }
              অন্যথা যদি (increment_tabval)
                {
                  যদি (! set_increment_size (tabval))
                    {
                      ok = false;
                      ভঙ্গ;
                    }
                }
              অন্যথা
                add_tab_stop (tabval);
            }
          have_tabval = false;
        }
      অন্যথা যদি (*stops == '/')
        {
          যদি (have_tabval)
            {
              error (0, 0, _("'/' specifier not at start of number: %s"),
                     quote (stops));
              ok = false;
            }
          extend_tabval = true;
          increment_tabval = false;
        }
      অন্যথা যদি (*stops == '+')
        {
          যদি (have_tabval)
            {
              error (0, 0, _("'+' specifier not at start of number: %s"),
                     quote (stops));
              ok = false;
            }
          increment_tabval = true;
          extend_tabval = false;
        }
      অন্যথা যদি (c_isdigit (*stops))
        {
          যদি (!have_tabval)
            {
              tabval = 0;
              have_tabval = true;
              অঙ্ক_start = stops;
            }

          /* Detect overflow.  */
          যদি (!DECIMAL_DIGIT_ACCUMULATE (tabval, *stops - '0'))
            {
              idx_t len = strspn (অঙ্ক_start, "0123456789");
              ৱর্ণ *bad_অঙ্ক = ximemdup0 (অঙ্ক_start, len);
              error (0, 0, _("tab stop is too large %s"), quote (bad_অঙ্ক));
              মুক্ত (bad_অঙ্ক);
              ok = false;
              stops = অঙ্ক_start + len - 1;
            }
        }
      অন্যথা
        {
          error (0, 0, _("tab size contains invalid character(s): %s"),
                 quote (stops));
          ok = false;
          ভঙ্গ;
        }
    }

  যদি (ok && have_tabval)
    {
      যদি (extend_tabval)
        ok &= set_extend_size (tabval);
      অন্যথা যদি (increment_tabval)
        ok &= set_increment_size (tabval);
      অন্যথা
        add_tab_stop (tabval);
    }

  যদি (! ok)
    exit (EXIT_FAILURE);
}

/* Check that the list of tab stops TABS, with ENTRIES entries,
   contains only nonzero, ascending values.  */

স্থির শূন্য
validate_tab_stops (colno ধ্রুৱ *tabs, idx_t entries)
{
  colno prev_tab = 0;

  বাবে (idx_t i = 0; i < entries; i++)
    {
      যদি (tabs[i] == 0)
        error (EXIT_FAILURE, 0, _("tab size cannot be 0"));
      যদি (tabs[i] <= prev_tab)
        error (EXIT_FAILURE, 0, _("tab sizes must be ascending"));
      prev_tab = tabs[i];
    }

  যদি (increment_size && extend_size)
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
বাহ্য শূন্য
finalize_tab_stops (শূন্য)
{
  validate_tab_stops (tab_list, first_free_tab);

  যদি (first_free_tab == 0)
    tab_size = max_column_width = extend_size
                                  ? extend_size : increment_size
                                                  ? increment_size : 8;
  অন্যথা যদি (first_free_tab == 1 && ! extend_size && ! increment_size)
    tab_size = tab_list[0];
  অন্যথা
    tab_size = 0;
}


/* Return number of first tab stop after COLUMN.  TAB_INDEX specifies
   many multiple tab-sizes.  Set *LAST_TAB depending on whether we are
   returning COLUMN + 1 merely because we're past the last tab.
   If the number would overflow, diagnose this and exit.  */
বাহ্য colno
get_next_tab_column (colno column, idx_t *tab_index, bool *last_tab)
{
  *last_tab = false;
  colno tab_distance;

  /* single tab-size - return multiples of it */
  যদি (tab_size)
    tab_distance = tab_size - column % tab_size;
  অন্যথা
    {
      /* multiple tab-sizes - iterate them until the tab position is beyond
         the current input column. */
      বাবে ( ; *tab_index < first_free_tab ; (*tab_index)++ )
        {
          colno tab = tab_list[*tab_index];
          যদি (column < tab)
            ঘূৰাই tab;
        }

      /* relative last tab - return multiples of it */
      যদি (extend_size)
        tab_distance = extend_size - column % extend_size;
      অন্যথা যদি (increment_size)
        {
          /* incremental last tab - add increment_size to the previous
             tab stop */
          colno end_tab = tab_list[first_free_tab - 1];
          tab_distance = increment_size - ((column - end_tab) % increment_size);
        }
      অন্যথা
        {
          *last_tab = true;
          tab_distance = 1;
        }
    }

  colno tab_stop;
  যদি (ckd_add (&tab_stop, column, tab_distance))
    error (EXIT_FAILURE, 0, _("input line is too long"));
  ঘূৰাই tab_stop;
}


/* Sets new file-list */
বাহ্য শূন্য
set_file_list (ৱর্ণ **list)
{
  have_read_stdin = false;

  যদি (!list)
    file_list = stdin_argv;
  অন্যথা
    file_list = list;
}

/* Close the old stream pointer FP if it is non-null,
   and return a new one opened to read the next input file.
   Open a filename of '-' as the standard input.
   Return NULL if there are no more input files.  */

বাহ্য FILE *
next_file (FILE *fp)
{
  স্থির ৱর্ণ *prev_file;
  ৱর্ণ *file;

  যদি (fp)
    {
      পূর্ণাঙ্ক err = errno;
      যদি (!ferror (fp))
        err = 0;
      যদি (streq (prev_file, "-"))
        clearerr (fp);		/* Also clear EOF.  */
      অন্যথা যদি (fclose (fp) != 0)
        err = errno;
      যদি (err)
        {
          error (0, err, "%s", quotef (prev_file));
          exit_status = EXIT_FAILURE;
        }
    }

  যেতিয়ালৈ ((file = *file_list++) != রিক্ত)
    {
      যদি (streq (file, "-"))
        {
          have_read_stdin = true;
          fp = stdin;
        }
      অন্যথা
        fp = fopen (file, "r");
      যদি (fp)
        {
          prev_file = file;
          fadvise (fp, FADVISE_SEQUENTIAL);
          ঘূৰাই fp;
        }
      error (0, errno, "%s", quotef (file));
      exit_status = EXIT_FAILURE;
    }
  ঘূৰাই রিক্ত;
}

/* Close standard input if we have read from it.  */
বাহ্য শূন্য
cleanup_file_list_stdin (শূন্য)
{
    যদি (have_read_stdin && fclose (stdin) != 0)
      error (EXIT_FAILURE, errno, "-");
}

/* Emit the --help output for --tabs=LIST option accepted by expand and
   unexpand.  */
বাহ্য শূন্য
emit_tab_list_info (ৱর্ণ ধ্রুৱ *program)
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
