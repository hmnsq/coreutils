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

#ضمن <config.h>

#ضمن <stdio.h>
#ضمن <sys/types.h>
#ضمن "system.h"
#ضمن "c-ctype.h"
#ضمن "fadvise.h"
#ضمن "quote.h"

#ضمن "expand-common.h"

/* If true, convert blanks even after nonblank characters have been
   read on the line.  */
bool convert_entire_سطر = false;

/* If nonzero, the size of all tab stops.  If zero, use 'tab_list' instead.  */
ساكن colno tab_size = 0;

/* If nonzero, the size of all tab stops after the last specified.  */
ساكن colno extend_size = 0;

/* If nonzero, an increment for additional tab stops after the last specified.*/
ساكن colno increment_size = 0;

/* The maximum distance between tab stops.  */
idx_t max_column_width;

/* Array of the explicit column numbers of the tab stops;
   after 'tab_list' is exhausted, each additional tab is replaced
   by a space.  The first column is column 0.  */
ساكن colno *tab_list = عدم;

/* The number of allocated entries in 'tab_list'.  */
ساكن idx_t n_tabs_allocated = 0;

/* The index of the first invalid element of 'tab_list',
   where the next element can be added.  */
ساكن idx_t first_free_tab = 0;

/* Null-terminated array of input filenames.  */
ساكن حرف **file_list = عدم;

/* Default for 'file_list' if no files are given on the command line.  */
ساكن حرف *stdin_argv[] =
{
  (حرف *) "-", عدم
};

/* True if we have ever read standard input.  */
ساكن bool have_read_stdin = false;

/* The desired exit status.  */
صحيح exit_status = EXIT_SUCCESS;


ساكن فراغ
set_max_column_width (colno width)
{
  إذا (max_column_width < width)
    {
      إذا (ckd_add (&max_column_width, width, 0))
        error (EXIT_FAILURE, 0, _("tabs are too far apart"));
    }
}

/* Add tab stop TABVAL to the end of 'tab_list'.  */
خارجي فراغ
add_tab_stop (colno tabval)
{
  colno prev_column = first_free_tab ? tab_list[first_free_tab - 1] : 0;
  colno column_width = prev_column <= tabval ? tabval - prev_column : 0;

  إذا (first_free_tab == n_tabs_allocated)
    tab_list = xpalloc (tab_list, &n_tabs_allocated, 1, -1, حجم *tab_list);
  tab_list[first_free_tab++] = tabval;

  set_max_column_width (column_width);
}

ساكن bool
set_extend_size (colno tabval)
{
  bool ok = true;

  إذا (extend_size)
    {
      error (0, 0,
             _("'/' specifier only allowed"
               " with the last value"));
      ok = false;
    }
  extend_size = tabval;

  set_max_column_width (extend_size);

  أرجع ok;
}

ساكن bool
set_increment_size (colno tabval)
{
  bool ok = true;

  إذا (increment_size)
    {
      error (0,0,
             _("'+' specifier only allowed"
               " with the last value"));
      ok = false;
    }
  increment_size = tabval;

  set_max_column_width (increment_size);

  أرجع ok;
}

/* Add the comma or blank separated list of tab stops STOPS
   to the list of tab stops.  */
خارجي فراغ
parse_tab_stops (حرف ثابت *stops)
{
  bool have_tabval = false;
  colno tabval = 0;
  bool extend_tabval = false;
  bool increment_tabval = false;
  حرف ثابت *عدد_start = عدم;
  bool ok = true;

  لكل (; *stops; stops++)
    {
      إذا (*stops == ',' || isblank (to_uchar (*stops)))
        {
          إذا (have_tabval)
            {
              إذا (extend_tabval)
                {
                  إذا (! set_extend_size (tabval))
                    {
                      ok = false;
                      اكسر;
                    }
                }
              وإلا إذا (increment_tabval)
                {
                  إذا (! set_increment_size (tabval))
                    {
                      ok = false;
                      اكسر;
                    }
                }
              وإلا
                add_tab_stop (tabval);
            }
          have_tabval = false;
        }
      وإلا إذا (*stops == '/')
        {
          إذا (have_tabval)
            {
              error (0, 0, _("'/' specifier not at start of number: %s"),
                     quote (stops));
              ok = false;
            }
          extend_tabval = true;
          increment_tabval = false;
        }
      وإلا إذا (*stops == '+')
        {
          إذا (have_tabval)
            {
              error (0, 0, _("'+' specifier not at start of number: %s"),
                     quote (stops));
              ok = false;
            }
          increment_tabval = true;
          extend_tabval = false;
        }
      وإلا إذا (c_isdigit (*stops))
        {
          إذا (!have_tabval)
            {
              tabval = 0;
              have_tabval = true;
              عدد_start = stops;
            }

          /* Detect overflow.  */
          إذا (!DECIMAL_DIGIT_ACCUMULATE (tabval, *stops - '0'))
            {
              idx_t len = strspn (عدد_start, "0123456789");
              حرف *bad_عدد = ximemdup0 (عدد_start, len);
              error (0, 0, _("tab stop is too large %s"), quote (bad_عدد));
              حرر (bad_عدد);
              ok = false;
              stops = عدد_start + len - 1;
            }
        }
      وإلا
        {
          error (0, 0, _("tab size contains invalid character(s): %s"),
                 quote (stops));
          ok = false;
          اكسر;
        }
    }

  إذا (ok && have_tabval)
    {
      إذا (extend_tabval)
        ok &= set_extend_size (tabval);
      وإلا إذا (increment_tabval)
        ok &= set_increment_size (tabval);
      وإلا
        add_tab_stop (tabval);
    }

  إذا (! ok)
    exit (EXIT_FAILURE);
}

/* Check that the list of tab stops TABS, with ENTRIES entries,
   contains only nonzero, ascending values.  */

ساكن فراغ
validate_tab_stops (colno ثابت *tabs, idx_t entries)
{
  colno prev_tab = 0;

  لكل (idx_t i = 0; i < entries; i++)
    {
      إذا (tabs[i] == 0)
        error (EXIT_FAILURE, 0, _("tab size cannot be 0"));
      إذا (tabs[i] <= prev_tab)
        error (EXIT_FAILURE, 0, _("tab sizes must be ascending"));
      prev_tab = tabs[i];
    }

  إذا (increment_size && extend_size)
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
خارجي فراغ
finalize_tab_stops (فراغ)
{
  validate_tab_stops (tab_list, first_free_tab);

  إذا (first_free_tab == 0)
    tab_size = max_column_width = extend_size
                                  ? extend_size : increment_size
                                                  ? increment_size : 8;
  وإلا إذا (first_free_tab == 1 && ! extend_size && ! increment_size)
    tab_size = tab_list[0];
  وإلا
    tab_size = 0;
}


/* Return number of first tab stop after COLUMN.  TAB_INDEX specifies
   many multiple tab-sizes.  Set *LAST_TAB depending on whether we are
   returning COLUMN + 1 merely because we're past the last tab.
   If the number would overflow, diagnose this and exit.  */
خارجي colno
get_next_tab_column (colno column, idx_t *tab_index, bool *last_tab)
{
  *last_tab = false;
  colno tab_distance;

  /* single tab-size - return multiples of it */
  إذا (tab_size)
    tab_distance = tab_size - column % tab_size;
  وإلا
    {
      /* multiple tab-sizes - iterate them until the tab position is beyond
         the current input column. */
      لكل ( ; *tab_index < first_free_tab ; (*tab_index)++ )
        {
          colno tab = tab_list[*tab_index];
          إذا (column < tab)
            أرجع tab;
        }

      /* relative last tab - return multiples of it */
      إذا (extend_size)
        tab_distance = extend_size - column % extend_size;
      وإلا إذا (increment_size)
        {
          /* incremental last tab - add increment_size to the previous
             tab stop */
          colno end_tab = tab_list[first_free_tab - 1];
          tab_distance = increment_size - ((column - end_tab) % increment_size);
        }
      وإلا
        {
          *last_tab = true;
          tab_distance = 1;
        }
    }

  colno tab_stop;
  إذا (ckd_add (&tab_stop, column, tab_distance))
    error (EXIT_FAILURE, 0, _("input line is too long"));
  أرجع tab_stop;
}


/* Sets new file-list */
خارجي فراغ
set_file_list (حرف **list)
{
  have_read_stdin = false;

  إذا (!list)
    file_list = stdin_argv;
  وإلا
    file_list = list;
}

/* Close the old stream pointer FP if it is non-null,
   and return a new one opened to read the next input file.
   Open a filename of '-' as the standard input.
   Return NULL if there are no more input files.  */

خارجي FILE *
next_file (FILE *fp)
{
  ساكن حرف *prev_file;
  حرف *file;

  إذا (fp)
    {
      صحيح err = errno;
      إذا (!ferror (fp))
        err = 0;
      إذا (streq (prev_file, "-"))
        clearerr (fp);		/* Also clear EOF.  */
      وإلا إذا (fclose (fp) != 0)
        err = errno;
      إذا (err)
        {
          error (0, err, "%s", quotef (prev_file));
          exit_status = EXIT_FAILURE;
        }
    }

  طالما ((file = *file_list++) != عدم)
    {
      إذا (streq (file, "-"))
        {
          have_read_stdin = true;
          fp = stdin;
        }
      وإلا
        fp = fopen (file, "r");
      إذا (fp)
        {
          prev_file = file;
          fadvise (fp, FADVISE_SEQUENTIAL);
          أرجع fp;
        }
      error (0, errno, "%s", quotef (file));
      exit_status = EXIT_FAILURE;
    }
  أرجع عدم;
}

/* Close standard input if we have read from it.  */
خارجي فراغ
cleanup_file_list_stdin (فراغ)
{
    إذا (have_read_stdin && fclose (stdin) != 0)
      error (EXIT_FAILURE, errno, "-");
}

/* Emit the --help output for --tabs=LIST option accepted by expand and
   unexpand.  */
خارجي فراغ
emit_tab_list_info (حرف ثابت *program)
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
