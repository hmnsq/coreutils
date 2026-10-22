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

#ଅନ୍ତର୍ଭାଵ <config.h>

#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "c-ctype.h"
#ଅନ୍ତର୍ଭାଵ "fadvise.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"

#ଅନ୍ତର୍ଭାଵ "expand-common.h"

/* If true, convert blanks even after nonblank characters have been
   read on the line.  */
bool convert_entire_ଧାଡ଼ି = false;

/* If nonzero, the size of all tab stops.  If zero, use 'tab_list' instead.  */
ସ୍ଥିର colno tab_size = 0;

/* If nonzero, the size of all tab stops after the last specified.  */
ସ୍ଥିର colno extend_size = 0;

/* If nonzero, an increment for additional tab stops after the last specified.*/
ସ୍ଥିର colno increment_size = 0;

/* The maximum distance between tab stops.  */
idx_t max_column_width;

/* Array of the explicit column numbers of the tab stops;
   after 'tab_list' is exhausted, each additional tab is replaced
   by a space.  The first column is column 0.  */
ସ୍ଥିର colno *tab_list = ଖାଲି;

/* The number of allocated entries in 'tab_list'.  */
ସ୍ଥିର idx_t n_tabs_allocated = 0;

/* The index of the first invalid element of 'tab_list',
   where the next element can be added.  */
ସ୍ଥିର idx_t first_free_tab = 0;

/* Null-terminated array of input filenames.  */
ସ୍ଥିର ଅକ୍ଷର **file_list = ଖାଲି;

/* Default for 'file_list' if no files are given on the command line.  */
ସ୍ଥିର ଅକ୍ଷର *stdin_argv[] =
{
  (ଅକ୍ଷର *) "-", ଖାଲି
};

/* True if we have ever read standard input.  */
ସ୍ଥିର bool have_read_stdin = false;

/* The desired exit status.  */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା exit_status = EXIT_SUCCESS;


ସ୍ଥିର ଶୂନ୍ୟ
set_max_column_width (colno width)
{
  ଯଦି (max_column_width < width)
    {
      ଯଦି (ckd_add (&max_column_width, width, 0))
        error (EXIT_FAILURE, 0, _("tabs are too far apart"));
    }
}

/* Add tab stop TABVAL to the end of 'tab_list'.  */
ବାହ୍ୟ ଶୂନ୍ୟ
add_tab_stop (colno tabval)
{
  colno prev_column = first_free_tab ? tab_list[first_free_tab - 1] : 0;
  colno column_width = prev_column <= tabval ? tabval - prev_column : 0;

  ଯଦି (first_free_tab == n_tabs_allocated)
    tab_list = xpalloc (tab_list, &n_tabs_allocated, 1, -1, ଆକାର *tab_list);
  tab_list[first_free_tab++] = tabval;

  set_max_column_width (column_width);
}

ସ୍ଥିର bool
set_extend_size (colno tabval)
{
  bool ok = true;

  ଯଦି (extend_size)
    {
      error (0, 0,
             _("'/' specifier only allowed"
               " with the last value"));
      ok = false;
    }
  extend_size = tabval;

  set_max_column_width (extend_size);

  ଫେରନ୍ତୁ ok;
}

ସ୍ଥିର bool
set_increment_size (colno tabval)
{
  bool ok = true;

  ଯଦି (increment_size)
    {
      error (0,0,
             _("'+' specifier only allowed"
               " with the last value"));
      ok = false;
    }
  increment_size = tabval;

  set_max_column_width (increment_size);

  ଫେରନ୍ତୁ ok;
}

/* Add the comma or blank separated list of tab stops STOPS
   to the list of tab stops.  */
ବାହ୍ୟ ଶୂନ୍ୟ
parse_tab_stops (ଅକ୍ଷର ଅଚଳ *stops)
{
  bool have_tabval = false;
  colno tabval = 0;
  bool extend_tabval = false;
  bool increment_tabval = false;
  ଅକ୍ଷର ଅଚଳ *ଅଙ୍କ_start = ଖାଲି;
  bool ok = true;

  ପାଇଁ (; *stops; stops++)
    {
      ଯଦି (*stops == ',' || isblank (to_uchar (*stops)))
        {
          ଯଦି (have_tabval)
            {
              ଯଦି (extend_tabval)
                {
                  ଯଦି (! set_extend_size (tabval))
                    {
                      ok = false;
                      ଭାଙ୍ଗ;
                    }
                }
              ଅନ୍ୟଥା ଯଦି (increment_tabval)
                {
                  ଯଦି (! set_increment_size (tabval))
                    {
                      ok = false;
                      ଭାଙ୍ଗ;
                    }
                }
              ଅନ୍ୟଥା
                add_tab_stop (tabval);
            }
          have_tabval = false;
        }
      ଅନ୍ୟଥା ଯଦି (*stops == '/')
        {
          ଯଦି (have_tabval)
            {
              error (0, 0, _("'/' specifier not at start of number: %s"),
                     quote (stops));
              ok = false;
            }
          extend_tabval = true;
          increment_tabval = false;
        }
      ଅନ୍ୟଥା ଯଦି (*stops == '+')
        {
          ଯଦି (have_tabval)
            {
              error (0, 0, _("'+' specifier not at start of number: %s"),
                     quote (stops));
              ok = false;
            }
          increment_tabval = true;
          extend_tabval = false;
        }
      ଅନ୍ୟଥା ଯଦି (c_isdigit (*stops))
        {
          ଯଦି (!have_tabval)
            {
              tabval = 0;
              have_tabval = true;
              ଅଙ୍କ_start = stops;
            }

          /* Detect overflow.  */
          ଯଦି (!DECIMAL_DIGIT_ACCUMULATE (tabval, *stops - '0'))
            {
              idx_t len = strspn (ଅଙ୍କ_start, "0123456789");
              ଅକ୍ଷର *bad_ଅଙ୍କ = ximemdup0 (ଅଙ୍କ_start, len);
              error (0, 0, _("tab stop is too large %s"), quote (bad_ଅଙ୍କ));
              ମୁକ୍ତ (bad_ଅଙ୍କ);
              ok = false;
              stops = ଅଙ୍କ_start + len - 1;
            }
        }
      ଅନ୍ୟଥା
        {
          error (0, 0, _("tab size contains invalid character(s): %s"),
                 quote (stops));
          ok = false;
          ଭାଙ୍ଗ;
        }
    }

  ଯଦି (ok && have_tabval)
    {
      ଯଦି (extend_tabval)
        ok &= set_extend_size (tabval);
      ଅନ୍ୟଥା ଯଦି (increment_tabval)
        ok &= set_increment_size (tabval);
      ଅନ୍ୟଥା
        add_tab_stop (tabval);
    }

  ଯଦି (! ok)
    exit (EXIT_FAILURE);
}

/* Check that the list of tab stops TABS, with ENTRIES entries,
   contains only nonzero, ascending values.  */

ସ୍ଥିର ଶୂନ୍ୟ
validate_tab_stops (colno ଅଚଳ *tabs, idx_t entries)
{
  colno prev_tab = 0;

  ପାଇଁ (idx_t i = 0; i < entries; i++)
    {
      ଯଦି (tabs[i] == 0)
        error (EXIT_FAILURE, 0, _("tab size cannot be 0"));
      ଯଦି (tabs[i] <= prev_tab)
        error (EXIT_FAILURE, 0, _("tab sizes must be ascending"));
      prev_tab = tabs[i];
    }

  ଯଦି (increment_size && extend_size)
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
ବାହ୍ୟ ଶୂନ୍ୟ
finalize_tab_stops (ଶୂନ୍ୟ)
{
  validate_tab_stops (tab_list, first_free_tab);

  ଯଦି (first_free_tab == 0)
    tab_size = max_column_width = extend_size
                                  ? extend_size : increment_size
                                                  ? increment_size : 8;
  ଅନ୍ୟଥା ଯଦି (first_free_tab == 1 && ! extend_size && ! increment_size)
    tab_size = tab_list[0];
  ଅନ୍ୟଥା
    tab_size = 0;
}


/* Return number of first tab stop after COLUMN.  TAB_INDEX specifies
   many multiple tab-sizes.  Set *LAST_TAB depending on whether we are
   returning COLUMN + 1 merely because we're past the last tab.
   If the number would overflow, diagnose this and exit.  */
ବାହ୍ୟ colno
get_next_tab_column (colno column, idx_t *tab_index, bool *last_tab)
{
  *last_tab = false;
  colno tab_distance;

  /* single tab-size - return multiples of it */
  ଯଦି (tab_size)
    tab_distance = tab_size - column % tab_size;
  ଅନ୍ୟଥା
    {
      /* multiple tab-sizes - iterate them until the tab position is beyond
         the current input column. */
      ପାଇଁ ( ; *tab_index < first_free_tab ; (*tab_index)++ )
        {
          colno tab = tab_list[*tab_index];
          ଯଦି (column < tab)
            ଫେରନ୍ତୁ tab;
        }

      /* relative last tab - return multiples of it */
      ଯଦି (extend_size)
        tab_distance = extend_size - column % extend_size;
      ଅନ୍ୟଥା ଯଦି (increment_size)
        {
          /* incremental last tab - add increment_size to the previous
             tab stop */
          colno end_tab = tab_list[first_free_tab - 1];
          tab_distance = increment_size - ((column - end_tab) % increment_size);
        }
      ଅନ୍ୟଥା
        {
          *last_tab = true;
          tab_distance = 1;
        }
    }

  colno tab_stop;
  ଯଦି (ckd_add (&tab_stop, column, tab_distance))
    error (EXIT_FAILURE, 0, _("input line is too long"));
  ଫେରନ୍ତୁ tab_stop;
}


/* Sets new file-list */
ବାହ୍ୟ ଶୂନ୍ୟ
set_file_list (ଅକ୍ଷର **list)
{
  have_read_stdin = false;

  ଯଦି (!list)
    file_list = stdin_argv;
  ଅନ୍ୟଥା
    file_list = list;
}

/* Close the old stream pointer FP if it is non-null,
   and return a new one opened to read the next input file.
   Open a filename of '-' as the standard input.
   Return NULL if there are no more input files.  */

ବାହ୍ୟ FILE *
next_file (FILE *fp)
{
  ସ୍ଥିର ଅକ୍ଷର *prev_file;
  ଅକ୍ଷର *file;

  ଯଦି (fp)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା err = errno;
      ଯଦି (!ferror (fp))
        err = 0;
      ଯଦି (streq (prev_file, "-"))
        clearerr (fp);		/* Also clear EOF.  */
      ଅନ୍ୟଥା ଯଦି (fclose (fp) != 0)
        err = errno;
      ଯଦି (err)
        {
          error (0, err, "%s", quotef (prev_file));
          exit_status = EXIT_FAILURE;
        }
    }

  ଯେତେବେଳେ ((file = *file_list++) != ଖାଲି)
    {
      ଯଦି (streq (file, "-"))
        {
          have_read_stdin = true;
          fp = stdin;
        }
      ଅନ୍ୟଥା
        fp = fopen (file, "r");
      ଯଦି (fp)
        {
          prev_file = file;
          fadvise (fp, FADVISE_SEQUENTIAL);
          ଫେରନ୍ତୁ fp;
        }
      error (0, errno, "%s", quotef (file));
      exit_status = EXIT_FAILURE;
    }
  ଫେରନ୍ତୁ ଖାଲି;
}

/* Close standard input if we have read from it.  */
ବାହ୍ୟ ଶୂନ୍ୟ
cleanup_file_list_stdin (ଶୂନ୍ୟ)
{
    ଯଦି (have_read_stdin && fclose (stdin) != 0)
      error (EXIT_FAILURE, errno, "-");
}

/* Emit the --help output for --tabs=LIST option accepted by expand and
   unexpand.  */
ବାହ୍ୟ ଶୂନ୍ୟ
emit_tab_list_info (ଅକ୍ଷର ଅଚଳ *program)
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
