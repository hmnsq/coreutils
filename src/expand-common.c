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
ساکن colno tab_سائز = 0;

/* If nonzero, the size of all tab stops after the last specified.  */
ساکن colno extend_سائز = 0;

/* If nonzero, an increment for additional tab stops after the last specified.*/
ساکن colno increment_سائز = 0;

/* The maximum distance between tab stops.  */
idx_t max_عمود_چوڑائی;

/* Array of the explicit column numbers of the tab stops;
   after 'tab_list' is exhausted, each additional tab is replaced
   by a space.  The first column is column 0.  */
ساکن colno *tab_list = تہی;

/* The number of allocated entries in 'tab_list'.  */
ساکن idx_t n_tabs_allocated = 0;

/* The index of the first invalid element of 'tab_list',
   where the next element can be added.  */
ساکن idx_t پہلا_free_tab = 0;

/* Null-terminated array of input filenames.  */
ساکن حرف **file_list = تہی;

/* Default for 'file_list' if no files are given on the command line.  */
ساکن حرف *stdin_argv[] =
{
  (حرف *) "-", تہی
};

/* True if we have ever read standard input.  */
ساکن bool have_read_stdin = false;

/* The desired exit status.  */
صحیح exit_حالت = EXIT_SUCCESS;


ساکن خالی
سیٹ_max_عمود_چوڑائی (colno چوڑائی)
{
  اگر (max_عمود_چوڑائی < چوڑائی)
    {
      اگر (ckd_جوڑو (&max_عمود_چوڑائی, چوڑائی, 0))
        error (EXIT_FAILURE, 0, _("tabs are too far apart"));
    }
}

/* Add tab stop TABVAL to the end of 'tab_list'.  */
خارجی خالی
جوڑو_tab_stop (colno tabval)
{
  colno پچھلا_عمود = پہلا_free_tab ? tab_list[پہلا_free_tab - 1] : 0;
  colno عمود_چوڑائی = پچھلا_عمود <= tabval ? tabval - پچھلا_عمود : 0;

  اگر (پہلا_free_tab == n_tabs_allocated)
    tab_list = xpalloc (tab_list, &n_tabs_allocated, 1, -1, حجم *tab_list);
  tab_list[پہلا_free_tab++] = tabval;

  سیٹ_max_عمود_چوڑائی (عمود_چوڑائی);
}

ساکن bool
سیٹ_extend_سائز (colno tabval)
{
  bool ok = true;

  اگر (extend_سائز)
    {
      error (0, 0,
             _("'/' specifier only allowed"
               " with the last value"));
      ok = false;
    }
  extend_سائز = tabval;

  سیٹ_max_عمود_چوڑائی (extend_سائز);

  واپس ok;
}

ساکن bool
سیٹ_increment_سائز (colno tabval)
{
  bool ok = true;

  اگر (increment_سائز)
    {
      error (0,0,
             _("'+' specifier only allowed"
               " with the last value"));
      ok = false;
    }
  increment_سائز = tabval;

  سیٹ_max_عمود_چوڑائی (increment_سائز);

  واپس ok;
}

/* Add the comma or blank separated list of tab stops STOPS
   to the list of tab stops.  */
خارجی خالی
parse_tab_stops (حرف ثابت *stops)
{
  bool have_tabval = false;
  colno tabval = 0;
  bool extend_tabval = false;
  bool increment_tabval = false;
  حرف ثابت *عدد_آغاز = تہی;
  bool ok = true;

  برائے (; *stops; stops++)
    {
      اگر (*stops == ',' || isblank (مقصد_uchar (*stops)))
        {
          اگر (have_tabval)
            {
              اگر (extend_tabval)
                {
                  اگر (! سیٹ_extend_سائز (tabval))
                    {
                      ok = false;
                      توڑو;
                    }
                }
              ورنہ اگر (increment_tabval)
                {
                  اگر (! سیٹ_increment_سائز (tabval))
                    {
                      ok = false;
                      توڑو;
                    }
                }
              ورنہ
                جوڑو_tab_stop (tabval);
            }
          have_tabval = false;
        }
      ورنہ اگر (*stops == '/')
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
      ورنہ اگر (*stops == '+')
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
      ورنہ اگر (c_isdigit (*stops))
        {
          اگر (!have_tabval)
            {
              tabval = 0;
              have_tabval = true;
              عدد_آغاز = stops;
            }

          /* Detect overflow.  */
          اگر (!DECIMAL_DIGIT_ACCUMULATE (tabval, *stops - '0'))
            {
              idx_t len = strspn (عدد_آغاز, "0123456789");
              حرف *bad_عدد = ximemdup0 (عدد_آغاز, len);
              error (0, 0, _("tab stop is too large %s"), quote (bad_عدد));
              آزاد (bad_عدد);
              ok = false;
              stops = عدد_آغاز + len - 1;
            }
        }
      ورنہ
        {
          error (0, 0, _("tab size contains invalid character(s): %s"),
                 quote (stops));
          ok = false;
          توڑو;
        }
    }

  اگر (ok && have_tabval)
    {
      اگر (extend_tabval)
        ok &= سیٹ_extend_سائز (tabval);
      ورنہ اگر (increment_tabval)
        ok &= سیٹ_increment_سائز (tabval);
      ورنہ
        جوڑو_tab_stop (tabval);
    }

  اگر (! ok)
    باہر_نکلو (EXIT_FAILURE);
}

/* Check that the list of tab stops TABS, with ENTRIES entries,
   contains only nonzero, ascending values.  */

ساکن خالی
validate_tab_stops (colno ثابت *tabs, idx_t entries)
{
  colno پچھلا_tab = 0;

  برائے (idx_t i = 0; i < entries; i++)
    {
      اگر (tabs[i] == 0)
        error (EXIT_FAILURE, 0, _("tab size cannot be 0"));
      اگر (tabs[i] <= پچھلا_tab)
        error (EXIT_FAILURE, 0, _("tab sizes must be ascending"));
      پچھلا_tab = tabs[i];
    }

  اگر (increment_سائز && extend_سائز)
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
خارجی خالی
finalize_tab_stops (خالی)
{
  validate_tab_stops (tab_list, پہلا_free_tab);

  اگر (پہلا_free_tab == 0)
    tab_سائز = max_عمود_چوڑائی = extend_سائز
                                  ? extend_سائز : increment_سائز
                                                  ? increment_سائز : 8;
  ورنہ اگر (پہلا_free_tab == 1 && ! extend_سائز && ! increment_سائز)
    tab_سائز = tab_list[0];
  ورنہ
    tab_سائز = 0;
}


/* Return number of first tab stop after COLUMN.  TAB_INDEX specifies
   many multiple tab-sizes.  Set *LAST_TAB depending on whether we are
   returning COLUMN + 1 merely because we're past the last tab.
   If the number would overflow, diagnose this and exit.  */
خارجی colno
get_اگلا_tab_عمود (colno عمود, idx_t *tab_اشاریہ, bool *آخری_tab)
{
  *آخری_tab = false;
  colno tab_فاصلہ;

  /* single tab-size - return multiples of it */
  اگر (tab_سائز)
    tab_فاصلہ = tab_سائز - عمود % tab_سائز;
  ورنہ
    {
      /* multiple tab-sizes - iterate them until the tab position is beyond
         the current input column. */
      برائے ( ; *tab_اشاریہ < پہلا_free_tab ; (*tab_اشاریہ)++ )
        {
          colno tab = tab_list[*tab_اشاریہ];
          اگر (عمود < tab)
            واپس tab;
        }

      /* relative last tab - return multiples of it */
      اگر (extend_سائز)
        tab_فاصلہ = extend_سائز - عمود % extend_سائز;
      ورنہ اگر (increment_سائز)
        {
          /* incremental last tab - add increment_size to the previous
             tab stop */
          colno انتہا_tab = tab_list[پہلا_free_tab - 1];
          tab_فاصلہ = increment_سائز - ((عمود - انتہا_tab) % increment_سائز);
        }
      ورنہ
        {
          *آخری_tab = true;
          tab_فاصلہ = 1;
        }
    }

  colno tab_stop;
  اگر (ckd_جوڑو (&tab_stop, عمود, tab_فاصلہ))
    error (EXIT_FAILURE, 0, _("input line is too long"));
  واپس tab_stop;
}


/* Sets new file-list */
خارجی خالی
سیٹ_file_list (حرف **list)
{
  have_read_stdin = false;

  اگر (!list)
    file_list = stdin_argv;
  ورنہ
    file_list = list;
}

/* Close the old stream pointer FP if it is non-null,
   and return a new one opened to read the next input file.
   Open a filename of '-' as the standard input.
   Return NULL if there are no more input files.  */

خارجی FILE *
اگلا_file (FILE *fp)
{
  ساکن حرف *پچھلا_file;
  حرف *file;

  اگر (fp)
    {
      صحیح err = errno;
      اگر (!ferror (fp))
        err = 0;
      اگر (streq (پچھلا_file, "-"))
        clearerr (fp);		/* Also clear EOF.  */
      ورنہ اگر (fclose (fp) != 0)
        err = errno;
      اگر (err)
        {
          error (0, err, "%s", quotef (پچھلا_file));
          exit_حالت = EXIT_FAILURE;
        }
    }

  جب ((file = *file_list++) != تہی)
    {
      اگر (streq (file, "-"))
        {
          have_read_stdin = true;
          fp = stdin;
        }
      ورنہ
        fp = fopen (file, "r");
      اگر (fp)
        {
          پچھلا_file = file;
          fadvise (fp, FADVISE_SEQUENTIAL);
          واپس fp;
        }
      error (0, errno, "%s", quotef (file));
      exit_حالت = EXIT_FAILURE;
    }
  واپس تہی;
}

/* Close standard input if we have read from it.  */
خارجی خالی
cleanup_file_list_stdin (خالی)
{
    اگر (have_read_stdin && fclose (stdin) != 0)
      error (EXIT_FAILURE, errno, "-");
}

/* Emit the --help output for --tabs=LIST option accepted by expand and
   unexpand.  */
خارجی خالی
emit_tab_list_info (حرف ثابت *پروگرام)
{
  /* suppress syntax check for emit_mandatory_arg_note() */
  oputs_ (پروگرام, _("\
  -t, --tabs=LIST\n\
         use comma separated list کا tab positions.\n\
"));
  fputs (_("\
         The آخری specified position can be prefixed with '/'\n\
         مقصد specify a tab سائز مقصد use after the آخری\n\
         explicitly specified tab stop.  Also a سابقہ کا '+'\n\
         can be مستعمل مقصد align بقایا tab stops relative مقصد\n\
         the آخری specified tab stop instead کا the پہلا عمود\n\
"), stdout);
}
