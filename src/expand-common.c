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
bool convert_entire_سٽ = false;

/* If nonzero, the size of all tab stops.  If zero, use 'tab_list' instead.  */
ساكن colno tab_سائيز = 0;

/* If nonzero, the size of all tab stops after the last specified.  */
ساكن colno extend_سائيز = 0;

/* If nonzero, an increment for additional tab stops after the last specified.*/
ساكن colno increment_سائيز = 0;

/* The maximum distance between tab stops.  */
idx_t max_ڪالم_ويڪر;

/* Array of the explicit column numbers of the tab stops;
   after 'tab_list' is exhausted, each additional tab is replaced
   by a space.  The first column is column 0.  */
ساكن colno *tab_list = عدم;

/* The number of allocated entries in 'tab_list'.  */
ساكن idx_t n_tabs_allocated = 0;

/* The index of the first invalid element of 'tab_list',
   where the next element can be added.  */
ساكن idx_t پهريون_free_tab = 0;

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
سيٽ_max_ڪالم_ويڪر (colno ويڪر)
{
  إذا (max_ڪالم_ويڪر < ويڪر)
    {
      إذا (ckd_جوڙيو (&max_ڪالم_ويڪر, ويڪر, 0))
        error (EXIT_FAILURE, 0, _("tabs are too far apart"));
    }
}

/* Add tab stop TABVAL to the end of 'tab_list'.  */
خارجي فراغ
جوڙيو_tab_stop (colno tabval)
{
  colno prev_ڪالم = پهريون_free_tab ? tab_list[پهريون_free_tab - 1] : 0;
  colno ڪالم_ويڪر = prev_ڪالم <= tabval ? tabval - prev_ڪالم : 0;

  إذا (پهريون_free_tab == n_tabs_allocated)
    tab_list = xpalloc (tab_list, &n_tabs_allocated, 1, -1, حجم *tab_list);
  tab_list[پهريون_free_tab++] = tabval;

  سيٽ_max_ڪالم_ويڪر (ڪالم_ويڪر);
}

ساكن bool
سيٽ_extend_سائيز (colno tabval)
{
  bool ok = true;

  إذا (extend_سائيز)
    {
      error (0, 0,
             _("'/' specifier only allowed"
               " with the last value"));
      ok = false;
    }
  extend_سائيز = tabval;

  سيٽ_max_ڪالم_ويڪر (extend_سائيز);

  أرجع ok;
}

ساكن bool
سيٽ_increment_سائيز (colno tabval)
{
  bool ok = true;

  إذا (increment_سائيز)
    {
      error (0,0,
             _("'+' specifier only allowed"
               " with the last value"));
      ok = false;
    }
  increment_سائيز = tabval;

  سيٽ_max_ڪالم_ويڪر (increment_سائيز);

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
  حرف ثابت *انگ_شروعات = عدم;
  bool ok = true;

  لكل (; *stops; stops++)
    {
      إذا (*stops == ',' || isblank (منزل_uchar (*stops)))
        {
          إذا (have_tabval)
            {
              إذا (extend_tabval)
                {
                  إذا (! سيٽ_extend_سائيز (tabval))
                    {
                      ok = false;
                      اكسر;
                    }
                }
              وإلا إذا (increment_tabval)
                {
                  إذا (! سيٽ_increment_سائيز (tabval))
                    {
                      ok = false;
                      اكسر;
                    }
                }
              وإلا
                جوڙيو_tab_stop (tabval);
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
              انگ_شروعات = stops;
            }

          /* Detect overflow.  */
          إذا (!DECIMAL_DIGIT_ACCUMULATE (tabval, *stops - '0'))
            {
              idx_t len = strspn (انگ_شروعات, "0123456789");
              حرف *bad_انگ = ximemdup0 (انگ_شروعات, len);
              error (0, 0, _("tab stop is too large %s"), quote (bad_انگ));
              حرر (bad_انگ);
              ok = false;
              stops = انگ_شروعات + len - 1;
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
        ok &= سيٽ_extend_سائيز (tabval);
      وإلا إذا (increment_tabval)
        ok &= سيٽ_increment_سائيز (tabval);
      وإلا
        جوڙيو_tab_stop (tabval);
    }

  إذا (! ok)
    ٻاهر_نڪرو (EXIT_FAILURE);
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

  إذا (increment_سائيز && extend_سائيز)
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
  validate_tab_stops (tab_list, پهريون_free_tab);

  إذا (پهريون_free_tab == 0)
    tab_سائيز = max_ڪالم_ويڪر = extend_سائيز
                                  ? extend_سائيز : increment_سائيز
                                                  ? increment_سائيز : 8;
  وإلا إذا (پهريون_free_tab == 1 && ! extend_سائيز && ! increment_سائيز)
    tab_سائيز = tab_list[0];
  وإلا
    tab_سائيز = 0;
}


/* Return number of first tab stop after COLUMN.  TAB_INDEX specifies
   many multiple tab-sizes.  Set *LAST_TAB depending on whether we are
   returning COLUMN + 1 merely because we're past the last tab.
   If the number would overflow, diagnose this and exit.  */
خارجي colno
get_ايندڙ_tab_ڪالم (colno ڪالم, idx_t *tab_اشاريو, bool *آخري_tab)
{
  *آخري_tab = false;
  colno tab_مفاصلو;

  /* single tab-size - return multiples of it */
  إذا (tab_سائيز)
    tab_مفاصلو = tab_سائيز - ڪالم % tab_سائيز;
  وإلا
    {
      /* multiple tab-sizes - iterate them until the tab position is beyond
         the current input column. */
      لكل ( ; *tab_اشاريو < پهريون_free_tab ; (*tab_اشاريو)++ )
        {
          colno tab = tab_list[*tab_اشاريو];
          إذا (ڪالم < tab)
            أرجع tab;
        }

      /* relative last tab - return multiples of it */
      إذا (extend_سائيز)
        tab_مفاصلو = extend_سائيز - ڪالم % extend_سائيز;
      وإلا إذا (increment_سائيز)
        {
          /* incremental last tab - add increment_size to the previous
             tab stop */
          colno پڇاڙي_tab = tab_list[پهريون_free_tab - 1];
          tab_مفاصلو = increment_سائيز - ((ڪالم - پڇاڙي_tab) % increment_سائيز);
        }
      وإلا
        {
          *آخري_tab = true;
          tab_مفاصلو = 1;
        }
    }

  colno tab_stop;
  إذا (ckd_جوڙيو (&tab_stop, ڪالم, tab_مفاصلو))
    error (EXIT_FAILURE, 0, _("input line is too long"));
  أرجع tab_stop;
}


/* Sets new file-list */
خارجي فراغ
سيٽ_file_list (حرف **list)
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
ايندڙ_file (FILE *fp)
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
emit_tab_list_info (حرف ثابت *پروگرام)
{
  /* suppress syntax check for emit_mandatory_arg_note() */
  oputs_ (پروگرام, _("\
  -t, --tabs=LIST\n\
         use comma separated list جو tab positions.\n\
"));
  fputs (_("\
         The آخري specified position can be prefixed with '/'\n\
         منزل specify a tab سائيز منزل use after the آخري\n\
         explicitly specified tab stop.  Also a اڳياڙي جو '+'\n\
         can be استعمال_ٿيل منزل align بچيل tab stops relative منزل\n\
         the آخري specified tab stop instead جو the پهريون ڪالم\n\
"), stdout);
}
