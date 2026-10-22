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

#समावेश <config.h>

#समावेश <stdio.h>
#समावेश <sys/types.h>
#समावेश "system.h"
#समावेश "c-ctype.h"
#समावेश "fadvise.h"
#समावेश "quote.h"

#समावेश "expand-common.h"

/* If true, convert blanks even after nonblank characters have been
   read on the line.  */
bool convert_entire_पंक्ति = false;

/* If nonzero, the size of all tab stops.  If zero, use 'tab_list' instead.  */
अटल colno tab_आकार = 0;

/* If nonzero, the size of all tab stops after the last specified.  */
अटल colno extend_आकार = 0;

/* If nonzero, an increment for additional tab stops after the last specified.*/
अटल colno increment_आकार = 0;

/* The maximum distance between tab stops.  */
idx_t max_column_width;

/* Array of the explicit column numbers of the tab stops;
   after 'tab_list' is exhausted, each additional tab is replaced
   by a space.  The first column is column 0.  */
अटल colno *tab_list = शून्य;

/* The number of allocated entries in 'tab_list'.  */
अटल idx_t n_tabs_allocated = 0;

/* The index of the first invalid element of 'tab_list',
   where the next element can be added.  */
अटल idx_t first_free_tab = 0;

/* Null-terminated array of input filenames.  */
अटल अक्षर **file_list = शून्य;

/* Default for 'file_list' if no files are given on the command line.  */
अटल अक्षर *stdin_argv[] =
{
  (अक्षर *) "-", शून्य
};

/* True if we have ever read standard input.  */
अटल bool have_read_stdin = false;

/* The desired exit status.  */
पूर्णांक exit_status = निकास_सफल;


अटल व्योम
set_max_column_width (colno width)
{
  अगर (max_column_width < width)
    {
      अगर (ckd_add (&max_column_width, width, 0))
        error (निकास_त्रुटि, 0, _("tabs are too far apart"));
    }
}

/* Add tab stop TABVAL to the end of 'tab_list'.  */
बाह्य व्योम
add_tab_stop (colno tabval)
{
  colno पूर्व_column = first_free_tab ? tab_list[first_free_tab - 1] : 0;
  colno column_width = पूर्व_column <= tabval ? tabval - पूर्व_column : 0;

  अगर (first_free_tab == n_tabs_allocated)
    tab_list = xpalloc (tab_list, &n_tabs_allocated, 1, -1, माप *tab_list);
  tab_list[first_free_tab++] = tabval;

  set_max_column_width (column_width);
}

अटल bool
set_extend_आकार (colno tabval)
{
  bool ok = true;

  अगर (extend_आकार)
    {
      error (0, 0,
             _("'/' specifier only allowed"
               " with the last value"));
      ok = false;
    }
  extend_आकार = tabval;

  set_max_column_width (extend_आकार);

  वापस ok;
}

अटल bool
set_increment_आकार (colno tabval)
{
  bool ok = true;

  अगर (increment_आकार)
    {
      error (0,0,
             _("'+' specifier only allowed"
               " with the last value"));
      ok = false;
    }
  increment_आकार = tabval;

  set_max_column_width (increment_आकार);

  वापस ok;
}

/* Add the comma or blank separated list of tab stops STOPS
   to the list of tab stops.  */
बाह्य व्योम
parse_tab_stops (अक्षर स्थिर *stops)
{
  bool have_tabval = false;
  colno tabval = 0;
  bool extend_tabval = false;
  bool increment_tabval = false;
  अक्षर स्थिर *अंक_start = शून्य;
  bool ok = true;

  क्रम (; *stops; stops++)
    {
      अगर (*stops == ',' || isblank (to_uchar (*stops)))
        {
          अगर (have_tabval)
            {
              अगर (extend_tabval)
                {
                  अगर (! set_extend_आकार (tabval))
                    {
                      ok = false;
                      अवरोध;
                    }
                }
              अन्यथा अगर (increment_tabval)
                {
                  अगर (! set_increment_आकार (tabval))
                    {
                      ok = false;
                      अवरोध;
                    }
                }
              अन्यथा
                add_tab_stop (tabval);
            }
          have_tabval = false;
        }
      अन्यथा अगर (*stops == '/')
        {
          अगर (have_tabval)
            {
              error (0, 0, _("'/' specifier not at start of number: %s"),
                     quote (stops));
              ok = false;
            }
          extend_tabval = true;
          increment_tabval = false;
        }
      अन्यथा अगर (*stops == '+')
        {
          अगर (have_tabval)
            {
              error (0, 0, _("'+' specifier not at start of number: %s"),
                     quote (stops));
              ok = false;
            }
          increment_tabval = true;
          extend_tabval = false;
        }
      अन्यथा अगर (c_isdigit (*stops))
        {
          अगर (!have_tabval)
            {
              tabval = 0;
              have_tabval = true;
              अंक_start = stops;
            }

          /* Detect overflow.  */
          अगर (!DECIMAL_DIGIT_ACCUMULATE (tabval, *stops - '0'))
            {
              idx_t len = म_अखोज (अंक_start, "0123456789");
              अक्षर *bad_अंक = ximemdup0 (अंक_start, len);
              error (0, 0, _("tab stop is too large %s"), quote (bad_अंक));
              मुक्त (bad_अंक);
              ok = false;
              stops = अंक_start + len - 1;
            }
        }
      अन्यथा
        {
          error (0, 0, _("tab size contains invalid character(s): %s"),
                 quote (stops));
          ok = false;
          अवरोध;
        }
    }

  अगर (ok && have_tabval)
    {
      अगर (extend_tabval)
        ok &= set_extend_आकार (tabval);
      अन्यथा अगर (increment_tabval)
        ok &= set_increment_आकार (tabval);
      अन्यथा
        add_tab_stop (tabval);
    }

  अगर (! ok)
    निकास (निकास_त्रुटि);
}

/* Check that the list of tab stops TABS, with ENTRIES entries,
   contains only nonzero, ascending values.  */

अटल व्योम
validate_tab_stops (colno स्थिर *tabs, idx_t entries)
{
  colno पूर्व_tab = 0;

  क्रम (idx_t i = 0; i < entries; i++)
    {
      अगर (tabs[i] == 0)
        error (निकास_त्रुटि, 0, _("tab size cannot be 0"));
      अगर (tabs[i] <= पूर्व_tab)
        error (निकास_त्रुटि, 0, _("tab sizes must be ascending"));
      पूर्व_tab = tabs[i];
    }

  अगर (increment_आकार && extend_आकार)
    error (निकास_त्रुटि, 0, _("'/' specifier is mutually exclusive with '+'"));
}

/* Called after all command-line options have been parsed,
   and add_tab_stop/parse_tab_stops have been called.
   Will validate the tab-stop values,
   and set the final values to:
   tab-stops = 8 (if no tab-stops given on command line)
   tab-stops = N (if value N specified as the only value).
   tab-stops = distinct values given on command line (if multiple values given).
*/
बाह्य व्योम
finalize_tab_stops (व्योम)
{
  validate_tab_stops (tab_list, first_free_tab);

  अगर (first_free_tab == 0)
    tab_आकार = max_column_width = extend_आकार
                                  ? extend_आकार : increment_आकार
                                                  ? increment_आकार : 8;
  अन्यथा अगर (first_free_tab == 1 && ! extend_आकार && ! increment_आकार)
    tab_आकार = tab_list[0];
  अन्यथा
    tab_आकार = 0;
}


/* Return number of first tab stop after COLUMN.  TAB_INDEX specifies
   many multiple tab-sizes.  Set *LAST_TAB depending on whether we are
   returning COLUMN + 1 merely because we're past the last tab.
   If the number would overflow, diagnose this and exit.  */
बाह्य colno
get_अग्र_tab_column (colno column, idx_t *tab_सूचकांक, bool *last_tab)
{
  *last_tab = false;
  colno tab_distance;

  /* single tab-size - return multiples of it */
  अगर (tab_आकार)
    tab_distance = tab_आकार - column % tab_आकार;
  अन्यथा
    {
      /* multiple tab-sizes - iterate them until the tab position is beyond
         the current input column. */
      क्रम ( ; *tab_सूचकांक < first_free_tab ; (*tab_सूचकांक)++ )
        {
          colno tab = tab_list[*tab_सूचकांक];
          अगर (column < tab)
            वापस tab;
        }

      /* relative last tab - return multiples of it */
      अगर (extend_आकार)
        tab_distance = extend_आकार - column % extend_आकार;
      अन्यथा अगर (increment_आकार)
        {
          /* incremental last tab - add increment_size to the previous
             tab stop */
          colno end_tab = tab_list[first_free_tab - 1];
          tab_distance = increment_आकार - ((column - end_tab) % increment_आकार);
        }
      अन्यथा
        {
          *last_tab = true;
          tab_distance = 1;
        }
    }

  colno tab_stop;
  अगर (ckd_add (&tab_stop, column, tab_distance))
    error (निकास_त्रुटि, 0, _("input line is too long"));
  वापस tab_stop;
}


/* Sets new file-list */
बाह्य व्योम
set_file_list (अक्षर **list)
{
  have_read_stdin = false;

  अगर (!list)
    file_list = stdin_argv;
  अन्यथा
    file_list = list;
}

/* Close the old stream pointer FP if it is non-null,
   and return a new one opened to read the next input file.
   Open a filename of '-' as the standard input.
   Return NULL if there are no more input files.  */

बाह्य खाता *
अग्र_file (खाता *fp)
{
  अटल अक्षर *पूर्व_file;
  अक्षर *file;

  अगर (fp)
    {
      पूर्णांक err = त्रुटि_सं;
      अगर (!ख_त्रुटि (fp))
        err = 0;
      अगर (streq (पूर्व_file, "-"))
        साफ_त्रुटि (fp);		/* Also clear EOF.  */
      अन्यथा अगर (ख_बंद (fp) != 0)
        err = त्रुटि_सं;
      अगर (err)
        {
          error (0, err, "%s", quotef (पूर्व_file));
          exit_status = निकास_त्रुटि;
        }
    }

  जबतक ((file = *file_list++) != शून्य)
    {
      अगर (streq (file, "-"))
        {
          have_read_stdin = true;
          fp = मानक_निवेश;
        }
      अन्यथा
        fp = ख_खोलो (file, "r");
      अगर (fp)
        {
          पूर्व_file = file;
          fadvise (fp, FADVISE_SEQUENTIAL);
          वापस fp;
        }
      error (0, त्रुटि_सं, "%s", quotef (file));
      exit_status = निकास_त्रुटि;
    }
  वापस शून्य;
}

/* Close standard input if we have read from it.  */
बाह्य व्योम
cleanup_file_list_stdin (व्योम)
{
    अगर (have_read_stdin && ख_बंद (मानक_निवेश) != 0)
      error (निकास_त्रुटि, त्रुटि_सं, "-");
}

/* Emit the --help output for --tabs=LIST option accepted by expand and
   unexpand.  */
बाह्य व्योम
emit_tab_list_info (अक्षर स्थिर *program)
{
  /* suppress syntax check for emit_mandatory_arg_note() */
  oputs_ (program, _("\
  -t, --tabs=LIST\n\
         use comma separated list of tab positions.\n\
"));
  ख_माला_दो (_("\
         The last specified position can be prefixed with '/'\n\
         to specify a tab आकार to use after the last\n\
         explicitly specified tab stop.  Also a prefix of '+'\n\
         can be used to align remaining tab stops relative to\n\
         the last specified tab stop instead of the first column\n\
"), मानक_निकास);
}
