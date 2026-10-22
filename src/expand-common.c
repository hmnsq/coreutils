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

#அந்தர்பாவ <config.h>

#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ "system.h"
#அந்தர்பாவ "c-ctype.h"
#அந்தர்பாவ "fadvise.h"
#அந்தர்பாவ "quote.h"

#அந்தர்பாவ "expand-common.h"

/* If true, convert blanks even after nonblank characters have been
   read on the line.  */
bool convert_entire_வரி = false;

/* If nonzero, the size of all tab stops.  If zero, use 'tab_list' instead.  */
நிலையான colno tab_size = 0;

/* If nonzero, the size of all tab stops after the last specified.  */
நிலையான colno extend_size = 0;

/* If nonzero, an increment for additional tab stops after the last specified.*/
நிலையான colno increment_size = 0;

/* The maximum distance between tab stops.  */
idx_t max_column_width;

/* Array of the explicit column numbers of the tab stops;
   after 'tab_list' is exhausted, each additional tab is replaced
   by a space.  The first column is column 0.  */
நிலையான colno *tab_list = வெற்று;

/* The number of allocated entries in 'tab_list'.  */
நிலையான idx_t n_tabs_allocated = 0;

/* The index of the first invalid element of 'tab_list',
   where the next element can be added.  */
நிலையான idx_t first_free_tab = 0;

/* Null-terminated array of input filenames.  */
நிலையான எழுத்து **file_list = வெற்று;

/* Default for 'file_list' if no files are given on the command line.  */
நிலையான எழுத்து *stdin_argv[] =
{
  (எழுத்து *) "-", வெற்று
};

/* True if we have ever read standard input.  */
நிலையான bool have_read_stdin = false;

/* The desired exit status.  */
முழு exit_status = EXIT_SUCCESS;


நிலையான வெறுமை
set_max_column_width (colno width)
{
  எனில் (max_column_width < width)
    {
      எனில் (ckd_add (&max_column_width, width, 0))
        error (EXIT_FAILURE, 0, _("tabs are too far apart"));
    }
}

/* Add tab stop TABVAL to the end of 'tab_list'.  */
வெளி வெறுமை
add_tab_stop (colno tabval)
{
  colno prev_column = first_free_tab ? tab_list[first_free_tab - 1] : 0;
  colno column_width = prev_column <= tabval ? tabval - prev_column : 0;

  எனில் (first_free_tab == n_tabs_allocated)
    tab_list = xpalloc (tab_list, &n_tabs_allocated, 1, -1, அளவு *tab_list);
  tab_list[first_free_tab++] = tabval;

  set_max_column_width (column_width);
}

நிலையான bool
set_extend_size (colno tabval)
{
  bool ok = true;

  எனில் (extend_size)
    {
      error (0, 0,
             _("'/' specifier only allowed"
               " with the last value"));
      ok = false;
    }
  extend_size = tabval;

  set_max_column_width (extend_size);

  திரும்பு ok;
}

நிலையான bool
set_increment_size (colno tabval)
{
  bool ok = true;

  எனில் (increment_size)
    {
      error (0,0,
             _("'+' specifier only allowed"
               " with the last value"));
      ok = false;
    }
  increment_size = tabval;

  set_max_column_width (increment_size);

  திரும்பு ok;
}

/* Add the comma or blank separated list of tab stops STOPS
   to the list of tab stops.  */
வெளி வெறுமை
parse_tab_stops (எழுத்து மாறிலி *stops)
{
  bool have_tabval = false;
  colno tabval = 0;
  bool extend_tabval = false;
  bool increment_tabval = false;
  எழுத்து மாறிலி *இலக்கம்_start = வெற்று;
  bool ok = true;

  க்கு (; *stops; stops++)
    {
      எனில் (*stops == ',' || isblank (to_uchar (*stops)))
        {
          எனில் (have_tabval)
            {
              எனில் (extend_tabval)
                {
                  எனில் (! set_extend_size (tabval))
                    {
                      ok = false;
                      நிறுத்து;
                    }
                }
              இல்லையெனில் எனில் (increment_tabval)
                {
                  எனில் (! set_increment_size (tabval))
                    {
                      ok = false;
                      நிறுத்து;
                    }
                }
              இல்லையெனில்
                add_tab_stop (tabval);
            }
          have_tabval = false;
        }
      இல்லையெனில் எனில் (*stops == '/')
        {
          எனில் (have_tabval)
            {
              error (0, 0, _("'/' specifier not at start of number: %s"),
                     quote (stops));
              ok = false;
            }
          extend_tabval = true;
          increment_tabval = false;
        }
      இல்லையெனில் எனில் (*stops == '+')
        {
          எனில் (have_tabval)
            {
              error (0, 0, _("'+' specifier not at start of number: %s"),
                     quote (stops));
              ok = false;
            }
          increment_tabval = true;
          extend_tabval = false;
        }
      இல்லையெனில் எனில் (c_isdigit (*stops))
        {
          எனில் (!have_tabval)
            {
              tabval = 0;
              have_tabval = true;
              இலக்கம்_start = stops;
            }

          /* Detect overflow.  */
          எனில் (!DECIMAL_DIGIT_ACCUMULATE (tabval, *stops - '0'))
            {
              idx_t len = strspn (இலக்கம்_start, "0123456789");
              எழுத்து *bad_இலக்கம் = ximemdup0 (இலக்கம்_start, len);
              error (0, 0, _("tab stop is too large %s"), quote (bad_இலக்கம்));
              விடுவி (bad_இலக்கம்);
              ok = false;
              stops = இலக்கம்_start + len - 1;
            }
        }
      இல்லையெனில்
        {
          error (0, 0, _("tab size contains invalid character(s): %s"),
                 quote (stops));
          ok = false;
          நிறுத்து;
        }
    }

  எனில் (ok && have_tabval)
    {
      எனில் (extend_tabval)
        ok &= set_extend_size (tabval);
      இல்லையெனில் எனில் (increment_tabval)
        ok &= set_increment_size (tabval);
      இல்லையெனில்
        add_tab_stop (tabval);
    }

  எனில் (! ok)
    exit (EXIT_FAILURE);
}

/* Check that the list of tab stops TABS, with ENTRIES entries,
   contains only nonzero, ascending values.  */

நிலையான வெறுமை
validate_tab_stops (colno மாறிலி *tabs, idx_t entries)
{
  colno prev_tab = 0;

  க்கு (idx_t i = 0; i < entries; i++)
    {
      எனில் (tabs[i] == 0)
        error (EXIT_FAILURE, 0, _("tab size cannot be 0"));
      எனில் (tabs[i] <= prev_tab)
        error (EXIT_FAILURE, 0, _("tab sizes must be ascending"));
      prev_tab = tabs[i];
    }

  எனில் (increment_size && extend_size)
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
வெளி வெறுமை
finalize_tab_stops (வெறுமை)
{
  validate_tab_stops (tab_list, first_free_tab);

  எனில் (first_free_tab == 0)
    tab_size = max_column_width = extend_size
                                  ? extend_size : increment_size
                                                  ? increment_size : 8;
  இல்லையெனில் எனில் (first_free_tab == 1 && ! extend_size && ! increment_size)
    tab_size = tab_list[0];
  இல்லையெனில்
    tab_size = 0;
}


/* Return number of first tab stop after COLUMN.  TAB_INDEX specifies
   many multiple tab-sizes.  Set *LAST_TAB depending on whether we are
   returning COLUMN + 1 merely because we're past the last tab.
   If the number would overflow, diagnose this and exit.  */
வெளி colno
get_next_tab_column (colno column, idx_t *tab_index, bool *last_tab)
{
  *last_tab = false;
  colno tab_distance;

  /* single tab-size - return multiples of it */
  எனில் (tab_size)
    tab_distance = tab_size - column % tab_size;
  இல்லையெனில்
    {
      /* multiple tab-sizes - iterate them until the tab position is beyond
         the current input column. */
      க்கு ( ; *tab_index < first_free_tab ; (*tab_index)++ )
        {
          colno tab = tab_list[*tab_index];
          எனில் (column < tab)
            திரும்பு tab;
        }

      /* relative last tab - return multiples of it */
      எனில் (extend_size)
        tab_distance = extend_size - column % extend_size;
      இல்லையெனில் எனில் (increment_size)
        {
          /* incremental last tab - add increment_size to the previous
             tab stop */
          colno end_tab = tab_list[first_free_tab - 1];
          tab_distance = increment_size - ((column - end_tab) % increment_size);
        }
      இல்லையெனில்
        {
          *last_tab = true;
          tab_distance = 1;
        }
    }

  colno tab_stop;
  எனில் (ckd_add (&tab_stop, column, tab_distance))
    error (EXIT_FAILURE, 0, _("input line is too long"));
  திரும்பு tab_stop;
}


/* Sets new file-list */
வெளி வெறுமை
set_file_list (எழுத்து **list)
{
  have_read_stdin = false;

  எனில் (!list)
    file_list = stdin_argv;
  இல்லையெனில்
    file_list = list;
}

/* Close the old stream pointer FP if it is non-null,
   and return a new one opened to read the next input file.
   Open a filename of '-' as the standard input.
   Return NULL if there are no more input files.  */

வெளி FILE *
next_file (FILE *fp)
{
  நிலையான எழுத்து *prev_file;
  எழுத்து *file;

  எனில் (fp)
    {
      முழு err = errno;
      எனில் (!ferror (fp))
        err = 0;
      எனில் (streq (prev_file, "-"))
        clearerr (fp);		/* Also clear EOF.  */
      இல்லையெனில் எனில் (fclose (fp) != 0)
        err = errno;
      எனில் (err)
        {
          error (0, err, "%s", quotef (prev_file));
          exit_status = EXIT_FAILURE;
        }
    }

  போது ((file = *file_list++) != வெற்று)
    {
      எனில் (streq (file, "-"))
        {
          have_read_stdin = true;
          fp = stdin;
        }
      இல்லையெனில்
        fp = fopen (file, "r");
      எனில் (fp)
        {
          prev_file = file;
          fadvise (fp, FADVISE_SEQUENTIAL);
          திரும்பு fp;
        }
      error (0, errno, "%s", quotef (file));
      exit_status = EXIT_FAILURE;
    }
  திரும்பு வெற்று;
}

/* Close standard input if we have read from it.  */
வெளி வெறுமை
cleanup_file_list_stdin (வெறுமை)
{
    எனில் (have_read_stdin && fclose (stdin) != 0)
      error (EXIT_FAILURE, errno, "-");
}

/* Emit the --help output for --tabs=LIST option accepted by expand and
   unexpand.  */
வெளி வெறுமை
emit_tab_list_info (எழுத்து மாறிலி *program)
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
