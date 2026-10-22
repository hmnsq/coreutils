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

#شامل <config.h>

#شامل <stdio.h>
#شامل <sys/types.h>
#شامل "system.h"
#شامل "c-ctype.h"
#شامل "fadvise.h"
#شامل "quote.h"

#شامل "expand-common.h"

/* If true, convert blanks even after nonblank characters have been
   read on the line.  */
bool convert_entire_سطر = false;

/* If nonzero, the size of all tab stops.  If zero, use 'tab_list' instead.  */
ایستا colno tab_size = 0;

/* If nonzero, the size of all tab stops after the last specified.  */
ایستا colno extend_size = 0;

/* If nonzero, an increment for additional tab stops after the last specified.*/
ایستا colno increment_size = 0;

/* The maximum distance between tab stops.  */
idx_t max_column_width;

/* Array of the explicit column numbers of the tab stops;
   after 'tab_list' is exhausted, each additional tab is replaced
   by a space.  The first column is column 0.  */
ایستا colno *tab_list = پوچ;

/* The number of allocated entries in 'tab_list'.  */
ایستا idx_t n_tabs_allocated = 0;

/* The index of the first invalid element of 'tab_list',
   where the next element can be added.  */
ایستا idx_t first_free_tab = 0;

/* Null-terminated array of input filenames.  */
ایستا نویسه **file_list = پوچ;

/* Default for 'file_list' if no files are given on the command line.  */
ایستا نویسه *stdin_argv[] =
{
  (نویسه *) "-", پوچ
};

/* True if we have ever read standard input.  */
ایستا bool have_read_stdin = false;

/* The desired exit status.  */
صحیح exit_status = EXIT_SUCCESS;


ایستا تهی
set_max_column_width (colno width)
{
  اگر (max_column_width < width)
    {
      اگر (ckd_add (&max_column_width, width, 0))
        error (EXIT_FAILURE, 0, _("tabs are too far apart"));
    }
}

/* Add tab stop TABVAL to the end of 'tab_list'.  */
بیرونی تهی
add_tab_stop (colno tabval)
{
  colno prev_column = first_free_tab ? tab_list[first_free_tab - 1] : 0;
  colno column_width = prev_column <= tabval ? tabval - prev_column : 0;

  اگر (first_free_tab == n_tabs_allocated)
    tab_list = xpalloc (tab_list, &n_tabs_allocated, 1, -1, اندازه *tab_list);
  tab_list[first_free_tab++] = tabval;

  set_max_column_width (column_width);
}

ایستا bool
set_extend_size (colno tabval)
{
  bool ok = true;

  اگر (extend_size)
    {
      error (0, 0,
             _("'/' specifier only allowed"
               " with the last value"));
      ok = false;
    }
  extend_size = tabval;

  set_max_column_width (extend_size);

  بازگشت ok;
}

ایستا bool
set_increment_size (colno tabval)
{
  bool ok = true;

  اگر (increment_size)
    {
      error (0,0,
             _("'+' specifier only allowed"
               " with the last value"));
      ok = false;
    }
  increment_size = tabval;

  set_max_column_width (increment_size);

  بازگشت ok;
}

/* Add the comma or blank separated list of tab stops STOPS
   to the list of tab stops.  */
بیرونی تهی
parse_tab_stops (نویسه ثابت *stops)
{
  bool have_tabval = false;
  colno tabval = 0;
  bool extend_tabval = false;
  bool increment_tabval = false;
  نویسه ثابت *رقم_start = پوچ;
  bool ok = true;

  برای (; *stops; stops++)
    {
      اگر (*stops == ',' || isblank (to_uchar (*stops)))
        {
          اگر (have_tabval)
            {
              اگر (extend_tabval)
                {
                  اگر (! set_extend_size (tabval))
                    {
                      ok = false;
                      بشکن;
                    }
                }
              وگرنه اگر (increment_tabval)
                {
                  اگر (! set_increment_size (tabval))
                    {
                      ok = false;
                      بشکن;
                    }
                }
              وگرنه
                add_tab_stop (tabval);
            }
          have_tabval = false;
        }
      وگرنه اگر (*stops == '/')
        {
          اگر (have_tabval)
            {
              error (0, 0, _("'/' specifier not at start of number: %s"),
                     quote (stops));
              ok = false;
            }
          extend_tabval = true;
          increment_tabval = false;
        }
      وگرنه اگر (*stops == '+')
        {
          اگر (have_tabval)
            {
              error (0, 0, _("'+' specifier not at start of number: %s"),
                     quote (stops));
              ok = false;
            }
          increment_tabval = true;
          extend_tabval = false;
        }
      وگرنه اگر (c_isdigit (*stops))
        {
          اگر (!have_tabval)
            {
              tabval = 0;
              have_tabval = true;
              رقم_start = stops;
            }

          /* Detect overflow.  */
          اگر (!DECIMAL_DIGIT_ACCUMULATE (tabval, *stops - '0'))
            {
              idx_t len = strspn (رقم_start, "0123456789");
              نویسه *bad_رقم = ximemdup0 (رقم_start, len);
              error (0, 0, _("tab stop is too large %s"), quote (bad_رقم));
              آزاد (bad_رقم);
              ok = false;
              stops = رقم_start + len - 1;
            }
        }
      وگرنه
        {
          error (0, 0, _("tab size contains invalid character(s): %s"),
                 quote (stops));
          ok = false;
          بشکن;
        }
    }

  اگر (ok && have_tabval)
    {
      اگر (extend_tabval)
        ok &= set_extend_size (tabval);
      وگرنه اگر (increment_tabval)
        ok &= set_increment_size (tabval);
      وگرنه
        add_tab_stop (tabval);
    }

  اگر (! ok)
    exit (EXIT_FAILURE);
}

/* Check that the list of tab stops TABS, with ENTRIES entries,
   contains only nonzero, ascending values.  */

ایستا تهی
validate_tab_stops (colno ثابت *tabs, idx_t entries)
{
  colno prev_tab = 0;

  برای (idx_t i = 0; i < entries; i++)
    {
      اگر (tabs[i] == 0)
        error (EXIT_FAILURE, 0, _("tab size cannot be 0"));
      اگر (tabs[i] <= prev_tab)
        error (EXIT_FAILURE, 0, _("tab sizes must be ascending"));
      prev_tab = tabs[i];
    }

  اگر (increment_size && extend_size)
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
بیرونی تهی
finalize_tab_stops (تهی)
{
  validate_tab_stops (tab_list, first_free_tab);

  اگر (first_free_tab == 0)
    tab_size = max_column_width = extend_size
                                  ? extend_size : increment_size
                                                  ? increment_size : 8;
  وگرنه اگر (first_free_tab == 1 && ! extend_size && ! increment_size)
    tab_size = tab_list[0];
  وگرنه
    tab_size = 0;
}


/* Return number of first tab stop after COLUMN.  TAB_INDEX specifies
   many multiple tab-sizes.  Set *LAST_TAB depending on whether we are
   returning COLUMN + 1 merely because we're past the last tab.
   If the number would overflow, diagnose this and exit.  */
بیرونی colno
get_next_tab_column (colno column, idx_t *tab_index, bool *last_tab)
{
  *last_tab = false;
  colno tab_distance;

  /* single tab-size - return multiples of it */
  اگر (tab_size)
    tab_distance = tab_size - column % tab_size;
  وگرنه
    {
      /* multiple tab-sizes - iterate them until the tab position is beyond
         the current input column. */
      برای ( ; *tab_index < first_free_tab ; (*tab_index)++ )
        {
          colno tab = tab_list[*tab_index];
          اگر (column < tab)
            بازگشت tab;
        }

      /* relative last tab - return multiples of it */
      اگر (extend_size)
        tab_distance = extend_size - column % extend_size;
      وگرنه اگر (increment_size)
        {
          /* incremental last tab - add increment_size to the previous
             tab stop */
          colno end_tab = tab_list[first_free_tab - 1];
          tab_distance = increment_size - ((column - end_tab) % increment_size);
        }
      وگرنه
        {
          *last_tab = true;
          tab_distance = 1;
        }
    }

  colno tab_stop;
  اگر (ckd_add (&tab_stop, column, tab_distance))
    error (EXIT_FAILURE, 0, _("input line is too long"));
  بازگشت tab_stop;
}


/* Sets new file-list */
بیرونی تهی
set_file_list (نویسه **list)
{
  have_read_stdin = false;

  اگر (!list)
    file_list = stdin_argv;
  وگرنه
    file_list = list;
}

/* Close the old stream pointer FP if it is non-null,
   and return a new one opened to read the next input file.
   Open a filename of '-' as the standard input.
   Return NULL if there are no more input files.  */

بیرونی FILE *
next_file (FILE *fp)
{
  ایستا نویسه *prev_file;
  نویسه *file;

  اگر (fp)
    {
      صحیح err = errno;
      اگر (!ferror (fp))
        err = 0;
      اگر (streq (prev_file, "-"))
        clearerr (fp);		/* Also clear EOF.  */
      وگرنه اگر (fclose (fp) != 0)
        err = errno;
      اگر (err)
        {
          error (0, err, "%s", quotef (prev_file));
          exit_status = EXIT_FAILURE;
        }
    }

  وقتی ((file = *file_list++) != پوچ)
    {
      اگر (streq (file, "-"))
        {
          have_read_stdin = true;
          fp = stdin;
        }
      وگرنه
        fp = fopen (file, "r");
      اگر (fp)
        {
          prev_file = file;
          fadvise (fp, FADVISE_SEQUENTIAL);
          بازگشت fp;
        }
      error (0, errno, "%s", quotef (file));
      exit_status = EXIT_FAILURE;
    }
  بازگشت پوچ;
}

/* Close standard input if we have read from it.  */
بیرونی تهی
cleanup_file_list_stdin (تهی)
{
    اگر (have_read_stdin && fclose (stdin) != 0)
      error (EXIT_FAILURE, errno, "-");
}

/* Emit the --help output for --tabs=LIST option accepted by expand and
   unexpand.  */
بیرونی تهی
emit_tab_list_info (نویسه ثابت *program)
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
