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

#अन्तर्भाव <config.h>

#अन्तर्भाव <stdio.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव "system.h"
#अन्तर्भाव "c-ctype.h"
#अन्तर्भाव "fadvise.h"
#अन्तर्भाव "quote.h"

#अन्तर्भाव "expand-common.h"

/* If true, convert blanks even after nonblank characters have been
   read on the line.  */
bool convert_entire_पङ्क्तिः = false;

/* If nonzero, the size of all tab stops.  If zero, use 'tab_list' instead.  */
स्थिर colno tab_आकार = 0;

/* If nonzero, the size of all tab stops after the last specified.  */
स्थिर colno extend_आकार = 0;

/* If nonzero, an increment for additional tab stops after the last specified.*/
स्थिर colno increment_आकार = 0;

/* The maximum distance between tab stops.  */
idx_t max_column_विस्तारः;

/* Array of the explicit column numbers of the tab stops;
   after 'tab_list' is exhausted, each additional tab is replaced
   by a space.  The first column is column 0.  */
स्थिर colno *tab_list = रिक्त;

/* The number of allocated entries in 'tab_list'.  */
स्थिर idx_t n_tabs_allocated = 0;

/* The index of the first invalid element of 'tab_list',
   where the next element can be added.  */
स्थिर idx_t first_free_tab = 0;

/* Null-terminated array of input filenames.  */
स्थिर वर्ण **file_list = रिक्त;

/* Default for 'file_list' if no files are given on the command line.  */
स्थिर वर्ण *stdin_argv[] =
{
  (वर्ण *) "-", रिक्त
};

/* True if we have ever read standard input.  */
स्थिर bool have_read_stdin = false;

/* The desired exit status.  */
पूर्णाङ्क exit_स्थितिः = EXIT_SUCCESS;


स्थिर शून्य
समुच्चयः_max_column_विस्तारः (colno विस्तारः)
{
  यदि (max_column_विस्तारः < विस्तारः)
    {
      यदि (ckd_सङ्कलय (&max_column_विस्तारः, विस्तारः, 0))
        error (EXIT_FAILURE, 0, _("tabs are too far apart"));
    }
}

/* Add tab stop TABVAL to the end of 'tab_list'.  */
बाह्य शून्य
सङ्कलय_tab_stop (colno tabval)
{
  colno पूर्व_column = first_free_tab ? tab_list[first_free_tab - 1] : 0;
  colno column_विस्तारः = पूर्व_column <= tabval ? tabval - पूर्व_column : 0;

  यदि (first_free_tab == n_tabs_allocated)
    tab_list = xpalloc (tab_list, &n_tabs_allocated, 1, -1, परिमाण *tab_list);
  tab_list[first_free_tab++] = tabval;

  समुच्चयः_max_column_विस्तारः (column_विस्तारः);
}

स्थिर bool
समुच्चयः_extend_आकार (colno tabval)
{
  bool ok = true;

  यदि (extend_आकार)
    {
      error (0, 0,
             _("'/' specifier only allowed"
               " with the last value"));
      ok = false;
    }
  extend_आकार = tabval;

  समुच्चयः_max_column_विस्तारः (extend_आकार);

  प्रत्यागम ok;
}

स्थिर bool
समुच्चयः_increment_आकार (colno tabval)
{
  bool ok = true;

  यदि (increment_आकार)
    {
      error (0,0,
             _("'+' specifier only allowed"
               " with the last value"));
      ok = false;
    }
  increment_आकार = tabval;

  समुच्चयः_max_column_विस्तारः (increment_आकार);

  प्रत्यागम ok;
}

/* Add the comma or blank separated list of tab stops STOPS
   to the list of tab stops.  */
बाह्य शून्य
parse_tab_stops (वर्ण ध्रुव *stops)
{
  bool have_tabval = false;
  colno tabval = 0;
  bool extend_tabval = false;
  bool increment_tabval = false;
  वर्ण ध्रुव *अङ्कः_start = रिक्त;
  bool ok = true;

  प्रति (; *stops; stops++)
    {
      यदि (*stops == ',' || isblank (गन्तव्यम्_uchar (*stops)))
        {
          यदि (have_tabval)
            {
              यदि (extend_tabval)
                {
                  यदि (! समुच्चयः_extend_आकार (tabval))
                    {
                      ok = false;
                      भङ्ग;
                    }
                }
              अन्यथा यदि (increment_tabval)
                {
                  यदि (! समुच्चयः_increment_आकार (tabval))
                    {
                      ok = false;
                      भङ्ग;
                    }
                }
              अन्यथा
                सङ्कलय_tab_stop (tabval);
            }
          have_tabval = false;
        }
      अन्यथा यदि (*stops == '/')
        {
          यदि (have_tabval)
            {
              error (0, 0, _("'/' specifier not at start of number: %s"),
                     quote (stops));
              ok = false;
            }
          extend_tabval = true;
          increment_tabval = false;
        }
      अन्यथा यदि (*stops == '+')
        {
          यदि (have_tabval)
            {
              error (0, 0, _("'+' specifier not at start of number: %s"),
                     quote (stops));
              ok = false;
            }
          increment_tabval = true;
          extend_tabval = false;
        }
      अन्यथा यदि (c_isdigit (*stops))
        {
          यदि (!have_tabval)
            {
              tabval = 0;
              have_tabval = true;
              अङ्कः_start = stops;
            }

          /* Detect overflow.  */
          यदि (!DECIMAL_DIGIT_ACCUMULATE (tabval, *stops - '0'))
            {
              idx_t len = strspn (अङ्कः_start, "0123456789");
              वर्ण *bad_अङ्कः = ximemdup0 (अङ्कः_start, len);
              error (0, 0, _("tab stop is too large %s"), quote (bad_अङ्कः));
              मुक्त (bad_अङ्कः);
              ok = false;
              stops = अङ्कः_start + len - 1;
            }
        }
      अन्यथा
        {
          error (0, 0, _("tab size contains invalid character(s): %s"),
                 quote (stops));
          ok = false;
          भङ्ग;
        }
    }

  यदि (ok && have_tabval)
    {
      यदि (extend_tabval)
        ok &= समुच्चयः_extend_आकार (tabval);
      अन्यथा यदि (increment_tabval)
        ok &= समुच्चयः_increment_आकार (tabval);
      अन्यथा
        सङ्कलय_tab_stop (tabval);
    }

  यदि (! ok)
    निर्गच्छ (EXIT_FAILURE);
}

/* Check that the list of tab stops TABS, with ENTRIES entries,
   contains only nonzero, ascending values.  */

स्थिर शून्य
validate_tab_stops (colno ध्रुव *tabs, idx_t entries)
{
  colno पूर्व_tab = 0;

  प्रति (idx_t i = 0; i < entries; i++)
    {
      यदि (tabs[i] == 0)
        error (EXIT_FAILURE, 0, _("tab size cannot be 0"));
      यदि (tabs[i] <= पूर्व_tab)
        error (EXIT_FAILURE, 0, _("tab sizes must be ascending"));
      पूर्व_tab = tabs[i];
    }

  यदि (increment_आकार && extend_आकार)
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
बाह्य शून्य
finalize_tab_stops (शून्य)
{
  validate_tab_stops (tab_list, first_free_tab);

  यदि (first_free_tab == 0)
    tab_आकार = max_column_विस्तारः = extend_आकार
                                  ? extend_आकार : increment_आकार
                                                  ? increment_आकार : 8;
  अन्यथा यदि (first_free_tab == 1 && ! extend_आकार && ! increment_आकार)
    tab_आकार = tab_list[0];
  अन्यथा
    tab_आकार = 0;
}


/* Return number of first tab stop after COLUMN.  TAB_INDEX specifies
   many multiple tab-sizes.  Set *LAST_TAB depending on whether we are
   returning COLUMN + 1 merely because we're past the last tab.
   If the number would overflow, diagnose this and exit.  */
बाह्य colno
get_अग्र_tab_column (colno column, idx_t *tab_सूचकांक, bool *चरमः_tab)
{
  *चरमः_tab = false;
  colno tab_distance;

  /* single tab-size - return multiples of it */
  यदि (tab_आकार)
    tab_distance = tab_आकार - column % tab_आकार;
  अन्यथा
    {
      /* multiple tab-sizes - iterate them until the tab position is beyond
         the current input column. */
      प्रति ( ; *tab_सूचकांक < first_free_tab ; (*tab_सूचकांक)++ )
        {
          colno tab = tab_list[*tab_सूचकांक];
          यदि (column < tab)
            प्रत्यागम tab;
        }

      /* relative last tab - return multiples of it */
      यदि (extend_आकार)
        tab_distance = extend_आकार - column % extend_आकार;
      अन्यथा यदि (increment_आकार)
        {
          /* incremental last tab - add increment_size to the previous
             tab stop */
          colno अन्तः_tab = tab_list[first_free_tab - 1];
          tab_distance = increment_आकार - ((column - अन्तः_tab) % increment_आकार);
        }
      अन्यथा
        {
          *चरमः_tab = true;
          tab_distance = 1;
        }
    }

  colno tab_stop;
  यदि (ckd_सङ्कलय (&tab_stop, column, tab_distance))
    error (EXIT_FAILURE, 0, _("input line is too long"));
  प्रत्यागम tab_stop;
}


/* Sets new file-list */
बाह्य शून्य
समुच्चयः_file_list (वर्ण **list)
{
  have_read_stdin = false;

  यदि (!list)
    file_list = stdin_argv;
  अन्यथा
    file_list = list;
}

/* Close the old stream pointer FP if it is non-null,
   and return a new one opened to read the next input file.
   Open a filename of '-' as the standard input.
   Return NULL if there are no more input files.  */

बाह्य FILE *
अग्र_file (FILE *fp)
{
  स्थिर वर्ण *पूर्व_file;
  वर्ण *file;

  यदि (fp)
    {
      पूर्णाङ्क err = errno;
      यदि (!ferror (fp))
        err = 0;
      यदि (streq (पूर्व_file, "-"))
        clearerr (fp);		/* Also clear EOF.  */
      अन्यथा यदि (fclose (fp) != 0)
        err = errno;
      यदि (err)
        {
          error (0, err, "%s", quotef (पूर्व_file));
          exit_स्थितिः = EXIT_FAILURE;
        }
    }

  यावत् ((file = *file_list++) != रिक्त)
    {
      यदि (streq (file, "-"))
        {
          have_read_stdin = true;
          fp = stdin;
        }
      अन्यथा
        fp = fopen (file, "r");
      यदि (fp)
        {
          पूर्व_file = file;
          fadvise (fp, FADVISE_SEQUENTIAL);
          प्रत्यागम fp;
        }
      error (0, errno, "%s", quotef (file));
      exit_स्थितिः = EXIT_FAILURE;
    }
  प्रत्यागम रिक्त;
}

/* Close standard input if we have read from it.  */
बाह्य शून्य
cleanup_file_list_stdin (शून्य)
{
    यदि (have_read_stdin && fclose (stdin) != 0)
      error (EXIT_FAILURE, errno, "-");
}

/* Emit the --help output for --tabs=LIST option accepted by expand and
   unexpand.  */
बाह्य शून्य
emit_tab_list_info (वर्ण ध्रुव *क्रमादेशः)
{
  /* suppress syntax check for emit_mandatory_arg_note() */
  oputs_ (क्रमादेशः, _("\
  -t, --tabs=LIST\n\
         use comma separated list सम्बन्धि tab positions.\n\
"));
  fputs (_("\
         The चरमः specified position can be prefixed with '/'\n\
         गन्तव्यम् specify a tab आकार गन्तव्यम् use after the चरमः\n\
         explicitly specified tab stop.  Also a उपसर्गः सम्बन्धि '+'\n\
         can be प्रयुक्तम् गन्तव्यम् align remaining tab stops relative गन्तव्यम्\n\
         the चरमः specified tab stop instead सम्बन्धि the first column\n\
"), stdout);
}
