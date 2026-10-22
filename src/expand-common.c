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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "c-ctype.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "fadvise.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "expand-common.h"

/* If true, convert blanks even after nonblank characters have been
   read on the line.  */
bool convert_entire_ਸਤਰ = false;

/* If nonzero, the size of all tab stops.  If zero, use 'tab_list' instead.  */
ਸਥਿਰ colno tab_ਮਾਪ = 0;

/* If nonzero, the size of all tab stops after the last specified.  */
ਸਥਿਰ colno extend_ਮਾਪ = 0;

/* If nonzero, an increment for additional tab stops after the last specified.*/
ਸਥਿਰ colno increment_ਮਾਪ = 0;

/* The maximum distance between tab stops.  */
idx_t max_ਕਾਲਮ_ਚੌੜਾਈ;

/* Array of the explicit column numbers of the tab stops;
   after 'tab_list' is exhausted, each additional tab is replaced
   by a space.  The first column is column 0.  */
ਸਥਿਰ colno *tab_list = ਨਿਰਾ;

/* The number of allocated entries in 'tab_list'.  */
ਸਥਿਰ idx_t n_tabs_allocated = 0;

/* The index of the first invalid element of 'tab_list',
   where the next element can be added.  */
ਸਥਿਰ idx_t ਪਹਿਲਾ_free_tab = 0;

/* Null-terminated array of input filenames.  */
ਸਥਿਰ ਅੱਖਰ **file_list = ਨਿਰਾ;

/* Default for 'file_list' if no files are given on the command line.  */
ਸਥਿਰ ਅੱਖਰ *stdin_argv[] =
{
  (ਅੱਖਰ *) "-", ਨਿਰਾ
};

/* True if we have ever read standard input.  */
ਸਥਿਰ bool have_read_stdin = false;

/* The desired exit status.  */
ਪੂਰਨ exit_ਸਥਿਤੀ = EXIT_SUCCESS;


ਸਥਿਰ ਖਾਲੀ
ਸੈੱਟ_max_ਕਾਲਮ_ਚੌੜਾਈ (colno ਚੌੜਾਈ)
{
  ਜੇ (max_ਕਾਲਮ_ਚੌੜਾਈ < ਚੌੜਾਈ)
    {
      ਜੇ (ckd_ਜੋੜੋ (&max_ਕਾਲਮ_ਚੌੜਾਈ, ਚੌੜਾਈ, 0))
        error (EXIT_FAILURE, 0, _("tabs are too far apart"));
    }
}

/* Add tab stop TABVAL to the end of 'tab_list'.  */
ਬਾਹਰੀ ਖਾਲੀ
ਜੋੜੋ_tab_stop (colno tabval)
{
  colno prev_ਕਾਲਮ = ਪਹਿਲਾ_free_tab ? tab_list[ਪਹਿਲਾ_free_tab - 1] : 0;
  colno ਕਾਲਮ_ਚੌੜਾਈ = prev_ਕਾਲਮ <= tabval ? tabval - prev_ਕਾਲਮ : 0;

  ਜੇ (ਪਹਿਲਾ_free_tab == n_tabs_allocated)
    tab_list = xpalloc (tab_list, &n_tabs_allocated, 1, -1, ਆਕਾਰ *tab_list);
  tab_list[ਪਹਿਲਾ_free_tab++] = tabval;

  ਸੈੱਟ_max_ਕਾਲਮ_ਚੌੜਾਈ (ਕਾਲਮ_ਚੌੜਾਈ);
}

ਸਥਿਰ bool
ਸੈੱਟ_extend_ਮਾਪ (colno tabval)
{
  bool ok = true;

  ਜੇ (extend_ਮਾਪ)
    {
      error (0, 0,
             _("'/' specifier only allowed"
               " with the last value"));
      ok = false;
    }
  extend_ਮਾਪ = tabval;

  ਸੈੱਟ_max_ਕਾਲਮ_ਚੌੜਾਈ (extend_ਮਾਪ);

  ਵਾਪਸ ok;
}

ਸਥਿਰ bool
ਸੈੱਟ_increment_ਮਾਪ (colno tabval)
{
  bool ok = true;

  ਜੇ (increment_ਮਾਪ)
    {
      error (0,0,
             _("'+' specifier only allowed"
               " with the last value"));
      ok = false;
    }
  increment_ਮਾਪ = tabval;

  ਸੈੱਟ_max_ਕਾਲਮ_ਚੌੜਾਈ (increment_ਮਾਪ);

  ਵਾਪਸ ok;
}

/* Add the comma or blank separated list of tab stops STOPS
   to the list of tab stops.  */
ਬਾਹਰੀ ਖਾਲੀ
parse_tab_stops (ਅੱਖਰ ਅਚਲ *stops)
{
  bool have_tabval = false;
  colno tabval = 0;
  bool extend_tabval = false;
  bool increment_tabval = false;
  ਅੱਖਰ ਅਚਲ *ਅੰਕ_ਸ਼ੁਰੂ = ਨਿਰਾ;
  bool ok = true;

  ਲਈ (; *stops; stops++)
    {
      ਜੇ (*stops == ',' || isblank (ਮੰਜ਼ਿਲ_uchar (*stops)))
        {
          ਜੇ (have_tabval)
            {
              ਜੇ (extend_tabval)
                {
                  ਜੇ (! ਸੈੱਟ_extend_ਮਾਪ (tabval))
                    {
                      ok = false;
                      ਰੋਕੋ;
                    }
                }
              ਨਹੀਂਤਾਂ ਜੇ (increment_tabval)
                {
                  ਜੇ (! ਸੈੱਟ_increment_ਮਾਪ (tabval))
                    {
                      ok = false;
                      ਰੋਕੋ;
                    }
                }
              ਨਹੀਂਤਾਂ
                ਜੋੜੋ_tab_stop (tabval);
            }
          have_tabval = false;
        }
      ਨਹੀਂਤਾਂ ਜੇ (*stops == '/')
        {
          ਜੇ (have_tabval)
            {
              error (0, 0, _("'/' specifier not at start of number: %s"),
                     quote (stops));
              ok = false;
            }
          extend_tabval = true;
          increment_tabval = false;
        }
      ਨਹੀਂਤਾਂ ਜੇ (*stops == '+')
        {
          ਜੇ (have_tabval)
            {
              error (0, 0, _("'+' specifier not at start of number: %s"),
                     quote (stops));
              ok = false;
            }
          increment_tabval = true;
          extend_tabval = false;
        }
      ਨਹੀਂਤਾਂ ਜੇ (c_isdigit (*stops))
        {
          ਜੇ (!have_tabval)
            {
              tabval = 0;
              have_tabval = true;
              ਅੰਕ_ਸ਼ੁਰੂ = stops;
            }

          /* Detect overflow.  */
          ਜੇ (!DECIMAL_DIGIT_ACCUMULATE (tabval, *stops - '0'))
            {
              idx_t len = strspn (ਅੰਕ_ਸ਼ੁਰੂ, "0123456789");
              ਅੱਖਰ *bad_ਅੰਕ = ximemdup0 (ਅੰਕ_ਸ਼ੁਰੂ, len);
              error (0, 0, _("tab stop is too large %s"), quote (bad_ਅੰਕ));
              ਮੁਕਤ (bad_ਅੰਕ);
              ok = false;
              stops = ਅੰਕ_ਸ਼ੁਰੂ + len - 1;
            }
        }
      ਨਹੀਂਤਾਂ
        {
          error (0, 0, _("tab size contains invalid character(s): %s"),
                 quote (stops));
          ok = false;
          ਰੋਕੋ;
        }
    }

  ਜੇ (ok && have_tabval)
    {
      ਜੇ (extend_tabval)
        ok &= ਸੈੱਟ_extend_ਮਾਪ (tabval);
      ਨਹੀਂਤਾਂ ਜੇ (increment_tabval)
        ok &= ਸੈੱਟ_increment_ਮਾਪ (tabval);
      ਨਹੀਂਤਾਂ
        ਜੋੜੋ_tab_stop (tabval);
    }

  ਜੇ (! ok)
    ਬਾਹਰ_ਨਿਕਲੋ (EXIT_FAILURE);
}

/* Check that the list of tab stops TABS, with ENTRIES entries,
   contains only nonzero, ascending values.  */

ਸਥਿਰ ਖਾਲੀ
validate_tab_stops (colno ਅਚਲ *tabs, idx_t entries)
{
  colno prev_tab = 0;

  ਲਈ (idx_t i = 0; i < entries; i++)
    {
      ਜੇ (tabs[i] == 0)
        error (EXIT_FAILURE, 0, _("tab size cannot be 0"));
      ਜੇ (tabs[i] <= prev_tab)
        error (EXIT_FAILURE, 0, _("tab sizes must be ascending"));
      prev_tab = tabs[i];
    }

  ਜੇ (increment_ਮਾਪ && extend_ਮਾਪ)
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
ਬਾਹਰੀ ਖਾਲੀ
finalize_tab_stops (ਖਾਲੀ)
{
  validate_tab_stops (tab_list, ਪਹਿਲਾ_free_tab);

  ਜੇ (ਪਹਿਲਾ_free_tab == 0)
    tab_ਮਾਪ = max_ਕਾਲਮ_ਚੌੜਾਈ = extend_ਮਾਪ
                                  ? extend_ਮਾਪ : increment_ਮਾਪ
                                                  ? increment_ਮਾਪ : 8;
  ਨਹੀਂਤਾਂ ਜੇ (ਪਹਿਲਾ_free_tab == 1 && ! extend_ਮਾਪ && ! increment_ਮਾਪ)
    tab_ਮਾਪ = tab_list[0];
  ਨਹੀਂਤਾਂ
    tab_ਮਾਪ = 0;
}


/* Return number of first tab stop after COLUMN.  TAB_INDEX specifies
   many multiple tab-sizes.  Set *LAST_TAB depending on whether we are
   returning COLUMN + 1 merely because we're past the last tab.
   If the number would overflow, diagnose this and exit.  */
ਬਾਹਰੀ colno
get_ਅਗਲਾ_tab_ਕਾਲਮ (colno ਕਾਲਮ, idx_t *tab_ਸੂਚਕਾਂਕ, bool *ਆਖਰੀ_tab)
{
  *ਆਖਰੀ_tab = false;
  colno tab_ਦੂਰੀ;

  /* single tab-size - return multiples of it */
  ਜੇ (tab_ਮਾਪ)
    tab_ਦੂਰੀ = tab_ਮਾਪ - ਕਾਲਮ % tab_ਮਾਪ;
  ਨਹੀਂਤਾਂ
    {
      /* multiple tab-sizes - iterate them until the tab position is beyond
         the current input column. */
      ਲਈ ( ; *tab_ਸੂਚਕਾਂਕ < ਪਹਿਲਾ_free_tab ; (*tab_ਸੂਚਕਾਂਕ)++ )
        {
          colno tab = tab_list[*tab_ਸੂਚਕਾਂਕ];
          ਜੇ (ਕਾਲਮ < tab)
            ਵਾਪਸ tab;
        }

      /* relative last tab - return multiples of it */
      ਜੇ (extend_ਮਾਪ)
        tab_ਦੂਰੀ = extend_ਮਾਪ - ਕਾਲਮ % extend_ਮਾਪ;
      ਨਹੀਂਤਾਂ ਜੇ (increment_ਮਾਪ)
        {
          /* incremental last tab - add increment_size to the previous
             tab stop */
          colno ਸਿਰਾ_tab = tab_list[ਪਹਿਲਾ_free_tab - 1];
          tab_ਦੂਰੀ = increment_ਮਾਪ - ((ਕਾਲਮ - ਸਿਰਾ_tab) % increment_ਮਾਪ);
        }
      ਨਹੀਂਤਾਂ
        {
          *ਆਖਰੀ_tab = true;
          tab_ਦੂਰੀ = 1;
        }
    }

  colno tab_stop;
  ਜੇ (ckd_ਜੋੜੋ (&tab_stop, ਕਾਲਮ, tab_ਦੂਰੀ))
    error (EXIT_FAILURE, 0, _("input line is too long"));
  ਵਾਪਸ tab_stop;
}


/* Sets new file-list */
ਬਾਹਰੀ ਖਾਲੀ
ਸੈੱਟ_file_list (ਅੱਖਰ **list)
{
  have_read_stdin = false;

  ਜੇ (!list)
    file_list = stdin_argv;
  ਨਹੀਂਤਾਂ
    file_list = list;
}

/* Close the old stream pointer FP if it is non-null,
   and return a new one opened to read the next input file.
   Open a filename of '-' as the standard input.
   Return NULL if there are no more input files.  */

ਬਾਹਰੀ FILE *
ਅਗਲਾ_file (FILE *fp)
{
  ਸਥਿਰ ਅੱਖਰ *prev_file;
  ਅੱਖਰ *file;

  ਜੇ (fp)
    {
      ਪੂਰਨ err = errno;
      ਜੇ (!ferror (fp))
        err = 0;
      ਜੇ (streq (prev_file, "-"))
        clearerr (fp);		/* Also clear EOF.  */
      ਨਹੀਂਤਾਂ ਜੇ (fclose (fp) != 0)
        err = errno;
      ਜੇ (err)
        {
          error (0, err, "%s", quotef (prev_file));
          exit_ਸਥਿਤੀ = EXIT_FAILURE;
        }
    }

  ਜਦੋਂ ((file = *file_list++) != ਨਿਰਾ)
    {
      ਜੇ (streq (file, "-"))
        {
          have_read_stdin = true;
          fp = stdin;
        }
      ਨਹੀਂਤਾਂ
        fp = fopen (file, "r");
      ਜੇ (fp)
        {
          prev_file = file;
          fadvise (fp, FADVISE_SEQUENTIAL);
          ਵਾਪਸ fp;
        }
      error (0, errno, "%s", quotef (file));
      exit_ਸਥਿਤੀ = EXIT_FAILURE;
    }
  ਵਾਪਸ ਨਿਰਾ;
}

/* Close standard input if we have read from it.  */
ਬਾਹਰੀ ਖਾਲੀ
cleanup_file_list_stdin (ਖਾਲੀ)
{
    ਜੇ (have_read_stdin && fclose (stdin) != 0)
      error (EXIT_FAILURE, errno, "-");
}

/* Emit the --help output for --tabs=LIST option accepted by expand and
   unexpand.  */
ਬਾਹਰੀ ਖਾਲੀ
emit_tab_list_info (ਅੱਖਰ ਅਚਲ *ਪ੍ਰੋਗਰਾਮ)
{
  /* suppress syntax check for emit_mandatory_arg_note() */
  oputs_ (ਪ੍ਰੋਗਰਾਮ, _("\
  -t, --tabs=LIST\n\
         use comma separated list ਦਾ tab positions.\n\
"));
  fputs (_("\
         The ਆਖਰੀ specified position can be prefixed with '/'\n\
         ਮੰਜ਼ਿਲ specify a tab ਮਾਪ ਮੰਜ਼ਿਲ use after the ਆਖਰੀ\n\
         explicitly specified tab stop.  Also a ਅਗੇਤਰ ਦਾ '+'\n\
         can be ਵਰਤਿਆ ਮੰਜ਼ਿਲ align ਬਚਿਆ tab stops relative ਮੰਜ਼ਿਲ\n\
         the ਆਖਰੀ specified tab stop instead ਦਾ the ਪਹਿਲਾ ਕਾਲਮ\n\
"), stdout);
}
