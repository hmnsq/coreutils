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

#include <config.h>

#include <stdio.h>
#include <sys/types.h>
#include "system.h"
#include "c-ctype.h"
#include "fadvise.h"
#include "quote.h"

#include "expand-common.h"

/* If true, convert blanks even after nonblank characters have been
   read on the line.  */
bool convert_entire_line = false;

/* If nonzero, the size of all tab stops.  If zero, use 'tab_list' instead.  */
static colno tab_size = 0;

/* If nonzero, the size of all tab stops after the last specified.  */
static colno extend_size = 0;

/* If nonzero, an increment for additional tab stops after the last specified.*/
static colno increment_size = 0;

/* The maximum distance between tab stops.  */
idx_t max_column_width;

/* Array of the explicit column numbers of the tab stops;
   after 'tab_list' is exhausted, each additional tab is replaced
   by a space.  The first column is column 0.  */
static colno *tab_list = NULL;

/* The number of allocated entries in 'tab_list'.  */
static idx_t n_tabs_allocated = 0;

/* The index of the first invalid element of 'tab_list',
   where the next element can be added.  */
static idx_t first_free_tab = 0;

/* Null-terminated array of input filenames.  */
static var_na **file_list = NULL;

/* Default for 'file_list' if no files are given on the command line.  */
static var_na *stdin_argv[] =
{
  (var_na *) "-", NULL
};

/* True if we have ever read standard input.  */
static bool have_read_stdin = false;

/* The desired exit status.  */
ကိန်းပြည့် exit_status = EXIT_SUCCESS;


static void
set_max_column_width (colno width)
{
  အကယ်၍ (max_column_width < width)
    {
      အကယ်၍ (ckd_add (&max_column_width, width, 0))
        error (EXIT_FAILURE, 0, _("tabs are too far apart"));
    }
}

/* Add tab stop TABVAL to the end of 'tab_list'.  */
extern void
add_tab_stop (colno tabval)
{
  colno prev_column = first_free_tab ? tab_list[first_free_tab - 1] : 0;
  colno column_width = prev_column <= tabval ? tabval - prev_column : 0;

  အကယ်၍ (first_free_tab == n_tabs_allocated)
    tab_list = xpalloc (tab_list, &n_tabs_allocated, 1, -1, parimaa_na *tab_list);
  tab_list[first_free_tab++] = tabval;

  set_max_column_width (column_width);
}

static bool
set_extend_size (colno tabval)
{
  bool ok = true;

  အကယ်၍ (extend_size)
    {
      error (0, 0,
             _("'/' specifier only allowed"
               " with the last value"));
      ok = false;
    }
  extend_size = tabval;

  set_max_column_width (extend_size);

  ပြန်ပေး ok;
}

static bool
set_increment_size (colno tabval)
{
  bool ok = true;

  အကယ်၍ (increment_size)
    {
      error (0,0,
             _("'+' specifier only allowed"
               " with the last value"));
      ok = false;
    }
  increment_size = tabval;

  set_max_column_width (increment_size);

  ပြန်ပေး ok;
}

/* Add the comma or blank separated list of tab stops STOPS
   to the list of tab stops.  */
extern void
parse_tab_stops (var_na const *stops)
{
  bool have_tabval = false;
  colno tabval = 0;
  bool extend_tabval = false;
  bool increment_tabval = false;
  var_na const *num_start = NULL;
  bool ok = true;

  အတွက် (; *stops; stops++)
    {
      အကယ်၍ (*stops == ',' || isblank (to_uchar (*stops)))
        {
          အကယ်၍ (have_tabval)
            {
              အကယ်၍ (extend_tabval)
                {
                  အကယ်၍ (! set_extend_size (tabval))
                    {
                      ok = false;
                      bha_nga;
                    }
                }
              မဟုတ်ပါက အကယ်၍ (increment_tabval)
                {
                  အကယ်၍ (! set_increment_size (tabval))
                    {
                      ok = false;
                      bha_nga;
                    }
                }
              မဟုတ်ပါက
                add_tab_stop (tabval);
            }
          have_tabval = false;
        }
      မဟုတ်ပါက အကယ်၍ (*stops == '/')
        {
          အကယ်၍ (have_tabval)
            {
              error (0, 0, _("'/' specifier not at start of number: %s"),
                     quote (stops));
              ok = false;
            }
          extend_tabval = true;
          increment_tabval = false;
        }
      မဟုတ်ပါက အကယ်၍ (*stops == '+')
        {
          အကယ်၍ (have_tabval)
            {
              error (0, 0, _("'+' specifier not at start of number: %s"),
                     quote (stops));
              ok = false;
            }
          increment_tabval = true;
          extend_tabval = false;
        }
      မဟုတ်ပါက အကယ်၍ (c_isdigit (*stops))
        {
          အကယ်၍ (!have_tabval)
            {
              tabval = 0;
              have_tabval = true;
              num_start = stops;
            }

          /* Detect overflow.  */
          အကယ်၍ (!DECIMAL_DIGIT_ACCUMULATE (tabval, *stops - '0'))
            {
              idx_t len = strspn (num_start, "0123456789");
              var_na *bad_num = ximemdup0 (num_start, len);
              error (0, 0, _("tab stop is too large %s"), quote (bad_num));
              free (bad_num);
              ok = false;
              stops = num_start + len - 1;
            }
        }
      မဟုတ်ပါက
        {
          error (0, 0, _("tab size contains invalid character(s): %s"),
                 quote (stops));
          ok = false;
          bha_nga;
        }
    }

  အကယ်၍ (ok && have_tabval)
    {
      အကယ်၍ (extend_tabval)
        ok &= set_extend_size (tabval);
      မဟုတ်ပါက အကယ်၍ (increment_tabval)
        ok &= set_increment_size (tabval);
      မဟုတ်ပါက
        add_tab_stop (tabval);
    }

  အကယ်၍ (! ok)
    exit (EXIT_FAILURE);
}

/* Check that the list of tab stops TABS, with ENTRIES entries,
   contains only nonzero, ascending values.  */

static void
validate_tab_stops (colno const *tabs, idx_t entries)
{
  colno prev_tab = 0;

  အတွက် (idx_t i = 0; i < entries; i++)
    {
      အကယ်၍ (tabs[i] == 0)
        error (EXIT_FAILURE, 0, _("tab size cannot be 0"));
      အကယ်၍ (tabs[i] <= prev_tab)
        error (EXIT_FAILURE, 0, _("tab sizes must be ascending"));
      prev_tab = tabs[i];
    }

  အကယ်၍ (increment_size && extend_size)
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
extern void
finalize_tab_stops (void)
{
  validate_tab_stops (tab_list, first_free_tab);

  အကယ်၍ (first_free_tab == 0)
    tab_size = max_column_width = extend_size
                                  ? extend_size : increment_size
                                                  ? increment_size : 8;
  မဟုတ်ပါက အကယ်၍ (first_free_tab == 1 && ! extend_size && ! increment_size)
    tab_size = tab_list[0];
  မဟုတ်ပါက
    tab_size = 0;
}


/* Return number of first tab stop after COLUMN.  TAB_INDEX specifies
   many multiple tab-sizes.  Set *LAST_TAB depending on whether we are
   returning COLUMN + 1 merely because we're past the last tab.
   If the number would overflow, diagnose this and exit.  */
extern colno
get_next_tab_column (colno column, idx_t *tab_index, bool *last_tab)
{
  *last_tab = false;
  colno tab_distance;

  /* single tab-size - return multiples of it */
  အကယ်၍ (tab_size)
    tab_distance = tab_size - column % tab_size;
  မဟုတ်ပါက
    {
      /* multiple tab-sizes - iterate them until the tab position is beyond
         the current input column. */
      အတွက် ( ; *tab_index < first_free_tab ; (*tab_index)++ )
        {
          colno tab = tab_list[*tab_index];
          အကယ်၍ (column < tab)
            ပြန်ပေး tab;
        }

      /* relative last tab - return multiples of it */
      အကယ်၍ (extend_size)
        tab_distance = extend_size - column % extend_size;
      မဟုတ်ပါက အကယ်၍ (increment_size)
        {
          /* incremental last tab - add increment_size to the previous
             tab stop */
          colno end_tab = tab_list[first_free_tab - 1];
          tab_distance = increment_size - ((column - end_tab) % increment_size);
        }
      မဟုတ်ပါက
        {
          *last_tab = true;
          tab_distance = 1;
        }
    }

  colno tab_stop;
  အကယ်၍ (ckd_add (&tab_stop, column, tab_distance))
    error (EXIT_FAILURE, 0, _("input line is too long"));
  ပြန်ပေး tab_stop;
}


/* Sets new file-list */
extern void
set_file_list (var_na **list)
{
  have_read_stdin = false;

  အကယ်၍ (!list)
    file_list = stdin_argv;
  မဟုတ်ပါက
    file_list = list;
}

/* Close the old stream pointer FP if it is non-null,
   and return a new one opened to read the next input file.
   Open a filename of '-' as the standard input.
   Return NULL if there are no more input files.  */

extern FILE *
next_file (FILE *fp)
{
  static var_na *prev_file;
  var_na *file;

  အကယ်၍ (fp)
    {
      ကိန်းပြည့် err = errno;
      အကယ်၍ (!ferror (fp))
        err = 0;
      အကယ်၍ (streq (prev_file, "-"))
        clearerr (fp);		/* Also clear EOF.  */
      မဟုတ်ပါက အကယ်၍ (fclose (fp) != 0)
        err = errno;
      အကယ်၍ (err)
        {
          error (0, err, "%s", quotef (prev_file));
          exit_status = EXIT_FAILURE;
        }
    }

  နေစဉ် ((file = *file_list++) != NULL)
    {
      အကယ်၍ (streq (file, "-"))
        {
          have_read_stdin = true;
          fp = stdin;
        }
      မဟုတ်ပါက
        fp = fopen (file, "r");
      အကယ်၍ (fp)
        {
          prev_file = file;
          fadvise (fp, FADVISE_SEQUENTIAL);
          ပြန်ပေး fp;
        }
      error (0, errno, "%s", quotef (file));
      exit_status = EXIT_FAILURE;
    }
  ပြန်ပေး NULL;
}

/* Close standard input if we have read from it.  */
extern void
cleanup_file_list_stdin (void)
{
    အကယ်၍ (have_read_stdin && fclose (stdin) != 0)
      error (EXIT_FAILURE, errno, "-");
}

/* Emit the --help output for --tabs=LIST option accepted by expand and
   unexpand.  */
extern void
emit_tab_list_info (var_na const *program)
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
