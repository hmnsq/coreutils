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
bool convert_entire_کرښه = false;

/* If nonzero, the size of all tab stops.  If zero, use 'tab_list' instead.  */
ساكن colno tab_اندازه = 0;

/* If nonzero, the size of all tab stops after the last specified.  */
ساكن colno extend_اندازه = 0;

/* If nonzero, an increment for additional tab stops after the last specified.*/
ساكن colno increment_اندازه = 0;

/* The maximum distance between tab stops.  */
idx_t max_ستنه_پلنوالی;

/* Array of the explicit column numbers of the tab stops;
   after 'tab_list' is exhausted, each additional tab is replaced
   by a space.  The first column is column 0.  */
ساكن colno *tab_list = عدم;

/* The number of allocated entries in 'tab_list'.  */
ساكن idx_t n_tabs_allocated = 0;

/* The index of the first invalid element of 'tab_list',
   where the next element can be added.  */
ساكن idx_t لومړی_free_tab = 0;

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
صحيح exit_حالت = EXIT_SUCCESS;


ساكن فراغ
سېټ_max_ستنه_پلنوالی (colno پلنوالی)
{
  إذا (max_ستنه_پلنوالی < پلنوالی)
    {
      إذا (ckd_جمع_کړه (&max_ستنه_پلنوالی, پلنوالی, 0))
        error (EXIT_FAILURE, 0, _("tabs are too far apart"));
    }
}

/* Add tab stop TABVAL to the end of 'tab_list'.  */
خارجي فراغ
جمع_کړه_tab_stop (colno tabval)
{
  colno prev_ستنه = لومړی_free_tab ? tab_list[لومړی_free_tab - 1] : 0;
  colno ستنه_پلنوالی = prev_ستنه <= tabval ? tabval - prev_ستنه : 0;

  إذا (لومړی_free_tab == n_tabs_allocated)
    tab_list = xpalloc (tab_list, &n_tabs_allocated, 1, -1, حجم *tab_list);
  tab_list[لومړی_free_tab++] = tabval;

  سېټ_max_ستنه_پلنوالی (ستنه_پلنوالی);
}

ساكن bool
سېټ_extend_اندازه (colno tabval)
{
  bool ok = true;

  إذا (extend_اندازه)
    {
      error (0, 0,
             _("'/' specifier only allowed"
               " with the last value"));
      ok = false;
    }
  extend_اندازه = tabval;

  سېټ_max_ستنه_پلنوالی (extend_اندازه);

  أرجع ok;
}

ساكن bool
سېټ_increment_اندازه (colno tabval)
{
  bool ok = true;

  إذا (increment_اندازه)
    {
      error (0,0,
             _("'+' specifier only allowed"
               " with the last value"));
      ok = false;
    }
  increment_اندازه = tabval;

  سېټ_max_ستنه_پلنوالی (increment_اندازه);

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
  حرف ثابت *عدد_پیل = عدم;
  bool ok = true;

  لكل (; *stops; stops++)
    {
      إذا (*stops == ',' || isblank (منزل_uchar (*stops)))
        {
          إذا (have_tabval)
            {
              إذا (extend_tabval)
                {
                  إذا (! سېټ_extend_اندازه (tabval))
                    {
                      ok = false;
                      اكسر;
                    }
                }
              وإلا إذا (increment_tabval)
                {
                  إذا (! سېټ_increment_اندازه (tabval))
                    {
                      ok = false;
                      اكسر;
                    }
                }
              وإلا
                جمع_کړه_tab_stop (tabval);
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
              عدد_پیل = stops;
            }

          /* Detect overflow.  */
          إذا (!DECIMAL_DIGIT_ACCUMULATE (tabval, *stops - '0'))
            {
              idx_t len = strspn (عدد_پیل, "0123456789");
              حرف *bad_عدد = ximemdup0 (عدد_پیل, len);
              error (0, 0, _("tab stop is too large %s"), quote (bad_عدد));
              حرر (bad_عدد);
              ok = false;
              stops = عدد_پیل + len - 1;
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
        ok &= سېټ_extend_اندازه (tabval);
      وإلا إذا (increment_tabval)
        ok &= سېټ_increment_اندازه (tabval);
      وإلا
        جمع_کړه_tab_stop (tabval);
    }

  إذا (! ok)
    وتل (EXIT_FAILURE);
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

  إذا (increment_اندازه && extend_اندازه)
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
  validate_tab_stops (tab_list, لومړی_free_tab);

  إذا (لومړی_free_tab == 0)
    tab_اندازه = max_ستنه_پلنوالی = extend_اندازه
                                  ? extend_اندازه : increment_اندازه
                                                  ? increment_اندازه : 8;
  وإلا إذا (لومړی_free_tab == 1 && ! extend_اندازه && ! increment_اندازه)
    tab_اندازه = tab_list[0];
  وإلا
    tab_اندازه = 0;
}


/* Return number of first tab stop after COLUMN.  TAB_INDEX specifies
   many multiple tab-sizes.  Set *LAST_TAB depending on whether we are
   returning COLUMN + 1 merely because we're past the last tab.
   If the number would overflow, diagnose this and exit.  */
خارجي colno
get_بل_tab_ستنه (colno ستنه, idx_t *tab_شاخص, bool *وروستی_tab)
{
  *وروستی_tab = false;
  colno tab_واټن;

  /* single tab-size - return multiples of it */
  إذا (tab_اندازه)
    tab_واټن = tab_اندازه - ستنه % tab_اندازه;
  وإلا
    {
      /* multiple tab-sizes - iterate them until the tab position is beyond
         the current input column. */
      لكل ( ; *tab_شاخص < لومړی_free_tab ; (*tab_شاخص)++ )
        {
          colno tab = tab_list[*tab_شاخص];
          إذا (ستنه < tab)
            أرجع tab;
        }

      /* relative last tab - return multiples of it */
      إذا (extend_اندازه)
        tab_واټن = extend_اندازه - ستنه % extend_اندازه;
      وإلا إذا (increment_اندازه)
        {
          /* incremental last tab - add increment_size to the previous
             tab stop */
          colno پای_ټکی_tab = tab_list[لومړی_free_tab - 1];
          tab_واټن = increment_اندازه - ((ستنه - پای_ټکی_tab) % increment_اندازه);
        }
      وإلا
        {
          *وروستی_tab = true;
          tab_واټن = 1;
        }
    }

  colno tab_stop;
  إذا (ckd_جمع_کړه (&tab_stop, ستنه, tab_واټن))
    error (EXIT_FAILURE, 0, _("input line is too long"));
  أرجع tab_stop;
}


/* Sets new file-list */
خارجي فراغ
سېټ_file_list (حرف **list)
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
بل_file (FILE *fp)
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
          exit_حالت = EXIT_FAILURE;
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
      exit_حالت = EXIT_FAILURE;
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
emit_tab_list_info (حرف ثابت *پروګرام)
{
  /* suppress syntax check for emit_mandatory_arg_note() */
  oputs_ (پروګرام, _("\
  -t, --tabs=LIST\n\
         use comma separated list د tab positions.\n\
"));
  fputs (_("\
         The وروستی specified position can be prefixed with '/'\n\
         منزل specify a tab اندازه منزل use after the وروستی\n\
         explicitly specified tab stop.  Also a مختاړی د '+'\n\
         can be کارول_شوی منزل align پاتې_شوی tab stops relative منزل\n\
         the وروستی specified tab stop instead د the لومړی ستنه\n\
"), stdout);
}
