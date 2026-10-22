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

#అన్తర్భావ <config.h>

#అన్తర్భావ <stdio.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ "system.h"
#అన్తర్భావ "c-ctype.h"
#అన్తర్భావ "fadvise.h"
#అన్తర్భావ "quote.h"

#అన్తర్భావ "expand-common.h"

/* If true, convert blanks even after nonblank characters have been
   read on the line.  */
bool convert_entire_పంక్తి = false;

/* If nonzero, the size of all tab stops.  If zero, use 'tab_list' instead.  */
స్థిర colno tab_కొలత = 0;

/* If nonzero, the size of all tab stops after the last specified.  */
స్థిర colno extend_కొలత = 0;

/* If nonzero, an increment for additional tab stops after the last specified.*/
స్థిర colno increment_కొలత = 0;

/* The maximum distance between tab stops.  */
idx_t max_స్తంభం_వెడల్పు;

/* Array of the explicit column numbers of the tab stops;
   after 'tab_list' is exhausted, each additional tab is replaced
   by a space.  The first column is column 0.  */
స్థిర colno *tab_list = ఖాళీ;

/* The number of allocated entries in 'tab_list'.  */
స్థిర idx_t n_tabs_allocated = 0;

/* The index of the first invalid element of 'tab_list',
   where the next element can be added.  */
స్థిర idx_t మొదటి_free_tab = 0;

/* Null-terminated array of input filenames.  */
స్థిర అక్షరం **file_list = ఖాళీ;

/* Default for 'file_list' if no files are given on the command line.  */
స్థిర అక్షరం *stdin_argv[] =
{
  (అక్షరం *) "-", ఖాళీ
};

/* True if we have ever read standard input.  */
స్థిర bool have_read_stdin = false;

/* The desired exit status.  */
పూర్ణాంకం exit_స్థితి = EXIT_SUCCESS;


స్థిర శూన్య
సమితి_max_స్తంభం_వెడల్పు (colno వెడల్పు)
{
  ఒకవేళ (max_స్తంభం_వెడల్పు < వెడల్పు)
    {
      ఒకవేళ (ckd_కూడు (&max_స్తంభం_వెడల్పు, వెడల్పు, 0))
        error (EXIT_FAILURE, 0, _("tabs are too far apart"));
    }
}

/* Add tab stop TABVAL to the end of 'tab_list'.  */
బాహ్య శూన్య
కూడు_tab_stop (colno tabval)
{
  colno prev_స్తంభం = మొదటి_free_tab ? tab_list[మొదటి_free_tab - 1] : 0;
  colno స్తంభం_వెడల్పు = prev_స్తంభం <= tabval ? tabval - prev_స్తంభం : 0;

  ఒకవేళ (మొదటి_free_tab == n_tabs_allocated)
    tab_list = xpalloc (tab_list, &n_tabs_allocated, 1, -1, పరిమాణం *tab_list);
  tab_list[మొదటి_free_tab++] = tabval;

  సమితి_max_స్తంభం_వెడల్పు (స్తంభం_వెడల్పు);
}

స్థిర bool
సమితి_extend_కొలత (colno tabval)
{
  bool ok = true;

  ఒకవేళ (extend_కొలత)
    {
      error (0, 0,
             _("'/' specifier only allowed"
               " with the last value"));
      ok = false;
    }
  extend_కొలత = tabval;

  సమితి_max_స్తంభం_వెడల్పు (extend_కొలత);

  తిరిగి ok;
}

స్థిర bool
సమితి_increment_కొలత (colno tabval)
{
  bool ok = true;

  ఒకవేళ (increment_కొలత)
    {
      error (0,0,
             _("'+' specifier only allowed"
               " with the last value"));
      ok = false;
    }
  increment_కొలత = tabval;

  సమితి_max_స్తంభం_వెడల్పు (increment_కొలత);

  తిరిగి ok;
}

/* Add the comma or blank separated list of tab stops STOPS
   to the list of tab stops.  */
బాహ్య శూన్య
parse_tab_stops (అక్షరం స్థిరరాశి *stops)
{
  bool have_tabval = false;
  colno tabval = 0;
  bool extend_tabval = false;
  bool increment_tabval = false;
  అక్షరం స్థిరరాశి *అంకె_ప్రారంభం = ఖాళీ;
  bool ok = true;

  కొరకు (; *stops; stops++)
    {
      ఒకవేళ (*stops == ',' || isblank (గమ్యం_uchar (*stops)))
        {
          ఒకవేళ (have_tabval)
            {
              ఒకవేళ (extend_tabval)
                {
                  ఒకవేళ (! సమితి_extend_కొలత (tabval))
                    {
                      ok = false;
                      ఆపు;
                    }
                }
              లేకపోతే ఒకవేళ (increment_tabval)
                {
                  ఒకవేళ (! సమితి_increment_కొలత (tabval))
                    {
                      ok = false;
                      ఆపు;
                    }
                }
              లేకపోతే
                కూడు_tab_stop (tabval);
            }
          have_tabval = false;
        }
      లేకపోతే ఒకవేళ (*stops == '/')
        {
          ఒకవేళ (have_tabval)
            {
              error (0, 0, _("'/' specifier not at start of number: %s"),
                     quote (stops));
              ok = false;
            }
          extend_tabval = true;
          increment_tabval = false;
        }
      లేకపోతే ఒకవేళ (*stops == '+')
        {
          ఒకవేళ (have_tabval)
            {
              error (0, 0, _("'+' specifier not at start of number: %s"),
                     quote (stops));
              ok = false;
            }
          increment_tabval = true;
          extend_tabval = false;
        }
      లేకపోతే ఒకవేళ (c_isdigit (*stops))
        {
          ఒకవేళ (!have_tabval)
            {
              tabval = 0;
              have_tabval = true;
              అంకె_ప్రారంభం = stops;
            }

          /* Detect overflow.  */
          ఒకవేళ (!DECIMAL_DIGIT_ACCUMULATE (tabval, *stops - '0'))
            {
              idx_t len = strspn (అంకె_ప్రారంభం, "0123456789");
              అక్షరం *bad_అంకె = ximemdup0 (అంకె_ప్రారంభం, len);
              error (0, 0, _("tab stop is too large %s"), quote (bad_అంకె));
              విడుదల (bad_అంకె);
              ok = false;
              stops = అంకె_ప్రారంభం + len - 1;
            }
        }
      లేకపోతే
        {
          error (0, 0, _("tab size contains invalid character(s): %s"),
                 quote (stops));
          ok = false;
          ఆపు;
        }
    }

  ఒకవేళ (ok && have_tabval)
    {
      ఒకవేళ (extend_tabval)
        ok &= సమితి_extend_కొలత (tabval);
      లేకపోతే ఒకవేళ (increment_tabval)
        ok &= సమితి_increment_కొలత (tabval);
      లేకపోతే
        కూడు_tab_stop (tabval);
    }

  ఒకవేళ (! ok)
    నిష్క్రమించు (EXIT_FAILURE);
}

/* Check that the list of tab stops TABS, with ENTRIES entries,
   contains only nonzero, ascending values.  */

స్థిర శూన్య
validate_tab_stops (colno స్థిరరాశి *tabs, idx_t entries)
{
  colno prev_tab = 0;

  కొరకు (idx_t i = 0; i < entries; i++)
    {
      ఒకవేళ (tabs[i] == 0)
        error (EXIT_FAILURE, 0, _("tab size cannot be 0"));
      ఒకవేళ (tabs[i] <= prev_tab)
        error (EXIT_FAILURE, 0, _("tab sizes must be ascending"));
      prev_tab = tabs[i];
    }

  ఒకవేళ (increment_కొలత && extend_కొలత)
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
బాహ్య శూన్య
finalize_tab_stops (శూన్య)
{
  validate_tab_stops (tab_list, మొదటి_free_tab);

  ఒకవేళ (మొదటి_free_tab == 0)
    tab_కొలత = max_స్తంభం_వెడల్పు = extend_కొలత
                                  ? extend_కొలత : increment_కొలత
                                                  ? increment_కొలత : 8;
  లేకపోతే ఒకవేళ (మొదటి_free_tab == 1 && ! extend_కొలత && ! increment_కొలత)
    tab_కొలత = tab_list[0];
  లేకపోతే
    tab_కొలత = 0;
}


/* Return number of first tab stop after COLUMN.  TAB_INDEX specifies
   many multiple tab-sizes.  Set *LAST_TAB depending on whether we are
   returning COLUMN + 1 merely because we're past the last tab.
   If the number would overflow, diagnose this and exit.  */
బాహ్య colno
get_తదుపరి_tab_స్తంభం (colno స్తంభం, idx_t *tab_సూచీ, bool *చివరి_tab)
{
  *చివరి_tab = false;
  colno tab_దూరం;

  /* single tab-size - return multiples of it */
  ఒకవేళ (tab_కొలత)
    tab_దూరం = tab_కొలత - స్తంభం % tab_కొలత;
  లేకపోతే
    {
      /* multiple tab-sizes - iterate them until the tab position is beyond
         the current input column. */
      కొరకు ( ; *tab_సూచీ < మొదటి_free_tab ; (*tab_సూచీ)++ )
        {
          colno tab = tab_list[*tab_సూచీ];
          ఒకవేళ (స్తంభం < tab)
            తిరిగి tab;
        }

      /* relative last tab - return multiples of it */
      ఒకవేళ (extend_కొలత)
        tab_దూరం = extend_కొలత - స్తంభం % extend_కొలత;
      లేకపోతే ఒకవేళ (increment_కొలత)
        {
          /* incremental last tab - add increment_size to the previous
             tab stop */
          colno చివర_tab = tab_list[మొదటి_free_tab - 1];
          tab_దూరం = increment_కొలత - ((స్తంభం - చివర_tab) % increment_కొలత);
        }
      లేకపోతే
        {
          *చివరి_tab = true;
          tab_దూరం = 1;
        }
    }

  colno tab_stop;
  ఒకవేళ (ckd_కూడు (&tab_stop, స్తంభం, tab_దూరం))
    error (EXIT_FAILURE, 0, _("input line is too long"));
  తిరిగి tab_stop;
}


/* Sets new file-list */
బాహ్య శూన్య
సమితి_file_list (అక్షరం **list)
{
  have_read_stdin = false;

  ఒకవేళ (!list)
    file_list = stdin_argv;
  లేకపోతే
    file_list = list;
}

/* Close the old stream pointer FP if it is non-null,
   and return a new one opened to read the next input file.
   Open a filename of '-' as the standard input.
   Return NULL if there are no more input files.  */

బాహ్య FILE *
తదుపరి_file (FILE *fp)
{
  స్థిర అక్షరం *prev_file;
  అక్షరం *file;

  ఒకవేళ (fp)
    {
      పూర్ణాంకం err = errno;
      ఒకవేళ (!ferror (fp))
        err = 0;
      ఒకవేళ (streq (prev_file, "-"))
        clearerr (fp);		/* Also clear EOF.  */
      లేకపోతే ఒకవేళ (fclose (fp) != 0)
        err = errno;
      ఒకవేళ (err)
        {
          error (0, err, "%s", quotef (prev_file));
          exit_స్థితి = EXIT_FAILURE;
        }
    }

  అయితే ((file = *file_list++) != ఖాళీ)
    {
      ఒకవేళ (streq (file, "-"))
        {
          have_read_stdin = true;
          fp = stdin;
        }
      లేకపోతే
        fp = fopen (file, "r");
      ఒకవేళ (fp)
        {
          prev_file = file;
          fadvise (fp, FADVISE_SEQUENTIAL);
          తిరిగి fp;
        }
      error (0, errno, "%s", quotef (file));
      exit_స్థితి = EXIT_FAILURE;
    }
  తిరిగి ఖాళీ;
}

/* Close standard input if we have read from it.  */
బాహ్య శూన్య
cleanup_file_list_stdin (శూన్య)
{
    ఒకవేళ (have_read_stdin && fclose (stdin) != 0)
      error (EXIT_FAILURE, errno, "-");
}

/* Emit the --help output for --tabs=LIST option accepted by expand and
   unexpand.  */
బాహ్య శూన్య
emit_tab_list_info (అక్షరం స్థిరరాశి *కార్యక్రమం)
{
  /* suppress syntax check for emit_mandatory_arg_note() */
  oputs_ (కార్యక్రమం, _("\
  -t, --tabs=LIST\n\
         use comma separated list యొక్క tab positions.\n\
"));
  fputs (_("\
         The చివరి specified position can be prefixed with '/'\n\
         గమ్యం specify a tab కొలత గమ్యం use after the చివరి\n\
         explicitly specified tab stop.  Also a ఉపసర్గ యొక్క '+'\n\
         can be వాడిన గమ్యం align మిగిలిన tab stops relative గమ్యం\n\
         the చివరి specified tab stop instead యొక్క the మొదటి స్తంభం\n\
"), stdout);
}
