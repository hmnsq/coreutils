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

#ಅನ್ತರ್ಭಾವ <config.h>

#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "c-ctype.h"
#ಅನ್ತರ್ಭಾವ "fadvise.h"
#ಅನ್ತರ್ಭಾವ "quote.h"

#ಅನ್ತರ್ಭಾವ "expand-common.h"

/* If true, convert blanks even after nonblank characters have been
   read on the line.  */
bool convert_entire_ಸಾಲು = false;

/* If nonzero, the size of all tab stops.  If zero, use 'tab_list' instead.  */
ಸ್ಥಿರ colno tab_ಗಾತ್ರ = 0;

/* If nonzero, the size of all tab stops after the last specified.  */
ಸ್ಥಿರ colno extend_ಗಾತ್ರ = 0;

/* If nonzero, an increment for additional tab stops after the last specified.*/
ಸ್ಥಿರ colno increment_ಗಾತ್ರ = 0;

/* The maximum distance between tab stops.  */
idx_t max_ಸ್ತಂಭ_ಅಗಲ;

/* Array of the explicit column numbers of the tab stops;
   after 'tab_list' is exhausted, each additional tab is replaced
   by a space.  The first column is column 0.  */
ಸ್ಥಿರ colno *tab_list = ಖಾಲಿ;

/* The number of allocated entries in 'tab_list'.  */
ಸ್ಥಿರ idx_t n_tabs_allocated = 0;

/* The index of the first invalid element of 'tab_list',
   where the next element can be added.  */
ಸ್ಥಿರ idx_t ಮೊದಲ_free_tab = 0;

/* Null-terminated array of input filenames.  */
ಸ್ಥಿರ ಅಕ್ಷರ **file_list = ಖಾಲಿ;

/* Default for 'file_list' if no files are given on the command line.  */
ಸ್ಥಿರ ಅಕ್ಷರ *stdin_argv[] =
{
  (ಅಕ್ಷರ *) "-", ಖಾಲಿ
};

/* True if we have ever read standard input.  */
ಸ್ಥಿರ bool have_read_stdin = false;

/* The desired exit status.  */
ಪೂರ್ಣಾಂಕ exit_ಸ್ಥಿತಿ = EXIT_SUCCESS;


ಸ್ಥಿರ ಶೂನ್ಯ
ಗಣ_max_ಸ್ತಂಭ_ಅಗಲ (colno ಅಗಲ)
{
  ಆದರೆ (max_ಸ್ತಂಭ_ಅಗಲ < ಅಗಲ)
    {
      ಆದರೆ (ckd_ಕೂಡಿಸು (&max_ಸ್ತಂಭ_ಅಗಲ, ಅಗಲ, 0))
        error (EXIT_FAILURE, 0, _("tabs are too far apart"));
    }
}

/* Add tab stop TABVAL to the end of 'tab_list'.  */
ಬಾಹ್ಯ ಶೂನ್ಯ
ಕೂಡಿಸು_tab_stop (colno tabval)
{
  colno prev_ಸ್ತಂಭ = ಮೊದಲ_free_tab ? tab_list[ಮೊದಲ_free_tab - 1] : 0;
  colno ಸ್ತಂಭ_ಅಗಲ = prev_ಸ್ತಂಭ <= tabval ? tabval - prev_ಸ್ತಂಭ : 0;

  ಆದರೆ (ಮೊದಲ_free_tab == n_tabs_allocated)
    tab_list = xpalloc (tab_list, &n_tabs_allocated, 1, -1, ಗಾತ್ರ *tab_list);
  tab_list[ಮೊದಲ_free_tab++] = tabval;

  ಗಣ_max_ಸ್ತಂಭ_ಅಗಲ (ಸ್ತಂಭ_ಅಗಲ);
}

ಸ್ಥಿರ bool
ಗಣ_extend_ಗಾತ್ರ (colno tabval)
{
  bool ok = true;

  ಆದರೆ (extend_ಗಾತ್ರ)
    {
      error (0, 0,
             _("'/' specifier only allowed"
               " with the last value"));
      ok = false;
    }
  extend_ಗಾತ್ರ = tabval;

  ಗಣ_max_ಸ್ತಂಭ_ಅಗಲ (extend_ಗಾತ್ರ);

  ಹಿಂತಿರುಗಿ ok;
}

ಸ್ಥಿರ bool
ಗಣ_increment_ಗಾತ್ರ (colno tabval)
{
  bool ok = true;

  ಆದರೆ (increment_ಗಾತ್ರ)
    {
      error (0,0,
             _("'+' specifier only allowed"
               " with the last value"));
      ok = false;
    }
  increment_ಗಾತ್ರ = tabval;

  ಗಣ_max_ಸ್ತಂಭ_ಅಗಲ (increment_ಗಾತ್ರ);

  ಹಿಂತಿರುಗಿ ok;
}

/* Add the comma or blank separated list of tab stops STOPS
   to the list of tab stops.  */
ಬಾಹ್ಯ ಶೂನ್ಯ
parse_tab_stops (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *stops)
{
  bool have_tabval = false;
  colno tabval = 0;
  bool extend_tabval = false;
  bool increment_tabval = false;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಅಂಕೆ_ಆರಂಭ = ಖಾಲಿ;
  bool ok = true;

  ಗಾಗಿ (; *stops; stops++)
    {
      ಆದರೆ (*stops == ',' || isblank (ಗಮ್ಯ_uchar (*stops)))
        {
          ಆದರೆ (have_tabval)
            {
              ಆದರೆ (extend_tabval)
                {
                  ಆದರೆ (! ಗಣ_extend_ಗಾತ್ರ (tabval))
                    {
                      ok = false;
                      ನಿಲ್ಲಿಸು;
                    }
                }
              ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (increment_tabval)
                {
                  ಆದರೆ (! ಗಣ_increment_ಗಾತ್ರ (tabval))
                    {
                      ok = false;
                      ನಿಲ್ಲಿಸು;
                    }
                }
              ಇಲ್ಲದಿದ್ದರೆ
                ಕೂಡಿಸು_tab_stop (tabval);
            }
          have_tabval = false;
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (*stops == '/')
        {
          ಆದರೆ (have_tabval)
            {
              error (0, 0, _("'/' specifier not at start of number: %s"),
                     quote (stops));
              ok = false;
            }
          extend_tabval = true;
          increment_tabval = false;
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (*stops == '+')
        {
          ಆದರೆ (have_tabval)
            {
              error (0, 0, _("'+' specifier not at start of number: %s"),
                     quote (stops));
              ok = false;
            }
          increment_tabval = true;
          extend_tabval = false;
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (c_isdigit (*stops))
        {
          ಆದರೆ (!have_tabval)
            {
              tabval = 0;
              have_tabval = true;
              ಅಂಕೆ_ಆರಂಭ = stops;
            }

          /* Detect overflow.  */
          ಆದರೆ (!DECIMAL_DIGIT_ACCUMULATE (tabval, *stops - '0'))
            {
              idx_t len = strspn (ಅಂಕೆ_ಆರಂಭ, "0123456789");
              ಅಕ್ಷರ *bad_ಅಂಕೆ = ximemdup0 (ಅಂಕೆ_ಆರಂಭ, len);
              error (0, 0, _("tab stop is too large %s"), quote (bad_ಅಂಕೆ));
              ಬಿಡುಗಡೆ (bad_ಅಂಕೆ);
              ok = false;
              stops = ಅಂಕೆ_ಆರಂಭ + len - 1;
            }
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          error (0, 0, _("tab size contains invalid character(s): %s"),
                 quote (stops));
          ok = false;
          ನಿಲ್ಲಿಸು;
        }
    }

  ಆದರೆ (ok && have_tabval)
    {
      ಆದರೆ (extend_tabval)
        ok &= ಗಣ_extend_ಗಾತ್ರ (tabval);
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (increment_tabval)
        ok &= ಗಣ_increment_ಗಾತ್ರ (tabval);
      ಇಲ್ಲದಿದ್ದರೆ
        ಕೂಡಿಸು_tab_stop (tabval);
    }

  ಆದರೆ (! ok)
    ನಿರ್ಗಮಿಸು (EXIT_FAILURE);
}

/* Check that the list of tab stops TABS, with ENTRIES entries,
   contains only nonzero, ascending values.  */

ಸ್ಥಿರ ಶೂನ್ಯ
validate_tab_stops (colno ಸ್ಥಿರಾಂಕ *tabs, idx_t entries)
{
  colno prev_tab = 0;

  ಗಾಗಿ (idx_t i = 0; i < entries; i++)
    {
      ಆದರೆ (tabs[i] == 0)
        error (EXIT_FAILURE, 0, _("tab size cannot be 0"));
      ಆದರೆ (tabs[i] <= prev_tab)
        error (EXIT_FAILURE, 0, _("tab sizes must be ascending"));
      prev_tab = tabs[i];
    }

  ಆದರೆ (increment_ಗಾತ್ರ && extend_ಗಾತ್ರ)
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
ಬಾಹ್ಯ ಶೂನ್ಯ
finalize_tab_stops (ಶೂನ್ಯ)
{
  validate_tab_stops (tab_list, ಮೊದಲ_free_tab);

  ಆದರೆ (ಮೊದಲ_free_tab == 0)
    tab_ಗಾತ್ರ = max_ಸ್ತಂಭ_ಅಗಲ = extend_ಗಾತ್ರ
                                  ? extend_ಗಾತ್ರ : increment_ಗಾತ್ರ
                                                  ? increment_ಗಾತ್ರ : 8;
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಮೊದಲ_free_tab == 1 && ! extend_ಗಾತ್ರ && ! increment_ಗಾತ್ರ)
    tab_ಗಾತ್ರ = tab_list[0];
  ಇಲ್ಲದಿದ್ದರೆ
    tab_ಗಾತ್ರ = 0;
}


/* Return number of first tab stop after COLUMN.  TAB_INDEX specifies
   many multiple tab-sizes.  Set *LAST_TAB depending on whether we are
   returning COLUMN + 1 merely because we're past the last tab.
   If the number would overflow, diagnose this and exit.  */
ಬಾಹ್ಯ colno
get_ಮುಂದಿನ_tab_ಸ್ತಂಭ (colno ಸ್ತಂಭ, idx_t *tab_ಸೂಚ್ಯಂಕ, bool *ಕೊನೆಯ_tab)
{
  *ಕೊನೆಯ_tab = false;
  colno tab_ದೂರ;

  /* single tab-size - return multiples of it */
  ಆದರೆ (tab_ಗಾತ್ರ)
    tab_ದೂರ = tab_ಗಾತ್ರ - ಸ್ತಂಭ % tab_ಗಾತ್ರ;
  ಇಲ್ಲದಿದ್ದರೆ
    {
      /* multiple tab-sizes - iterate them until the tab position is beyond
         the current input column. */
      ಗಾಗಿ ( ; *tab_ಸೂಚ್ಯಂಕ < ಮೊದಲ_free_tab ; (*tab_ಸೂಚ್ಯಂಕ)++ )
        {
          colno tab = tab_list[*tab_ಸೂಚ್ಯಂಕ];
          ಆದರೆ (ಸ್ತಂಭ < tab)
            ಹಿಂತಿರುಗಿ tab;
        }

      /* relative last tab - return multiples of it */
      ಆದರೆ (extend_ಗಾತ್ರ)
        tab_ದೂರ = extend_ಗಾತ್ರ - ಸ್ತಂಭ % extend_ಗಾತ್ರ;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (increment_ಗಾತ್ರ)
        {
          /* incremental last tab - add increment_size to the previous
             tab stop */
          colno ಕೊನೆ_tab = tab_list[ಮೊದಲ_free_tab - 1];
          tab_ದೂರ = increment_ಗಾತ್ರ - ((ಸ್ತಂಭ - ಕೊನೆ_tab) % increment_ಗಾತ್ರ);
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          *ಕೊನೆಯ_tab = true;
          tab_ದೂರ = 1;
        }
    }

  colno tab_stop;
  ಆದರೆ (ckd_ಕೂಡಿಸು (&tab_stop, ಸ್ತಂಭ, tab_ದೂರ))
    error (EXIT_FAILURE, 0, _("input line is too long"));
  ಹಿಂತಿರುಗಿ tab_stop;
}


/* Sets new file-list */
ಬಾಹ್ಯ ಶೂನ್ಯ
ಗಣ_file_list (ಅಕ್ಷರ **list)
{
  have_read_stdin = false;

  ಆದರೆ (!list)
    file_list = stdin_argv;
  ಇಲ್ಲದಿದ್ದರೆ
    file_list = list;
}

/* Close the old stream pointer FP if it is non-null,
   and return a new one opened to read the next input file.
   Open a filename of '-' as the standard input.
   Return NULL if there are no more input files.  */

ಬಾಹ್ಯ FILE *
ಮುಂದಿನ_file (FILE *fp)
{
  ಸ್ಥಿರ ಅಕ್ಷರ *prev_file;
  ಅಕ್ಷರ *file;

  ಆದರೆ (fp)
    {
      ಪೂರ್ಣಾಂಕ err = errno;
      ಆದರೆ (!ferror (fp))
        err = 0;
      ಆದರೆ (streq (prev_file, "-"))
        clearerr (fp);		/* Also clear EOF.  */
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (fclose (fp) != 0)
        err = errno;
      ಆದರೆ (err)
        {
          error (0, err, "%s", quotef (prev_file));
          exit_ಸ್ಥಿತಿ = EXIT_FAILURE;
        }
    }

  ಆಗ ((file = *file_list++) != ಖಾಲಿ)
    {
      ಆದರೆ (streq (file, "-"))
        {
          have_read_stdin = true;
          fp = stdin;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        fp = fopen (file, "r");
      ಆದರೆ (fp)
        {
          prev_file = file;
          fadvise (fp, FADVISE_SEQUENTIAL);
          ಹಿಂತಿರುಗಿ fp;
        }
      error (0, errno, "%s", quotef (file));
      exit_ಸ್ಥಿತಿ = EXIT_FAILURE;
    }
  ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

/* Close standard input if we have read from it.  */
ಬಾಹ್ಯ ಶೂನ್ಯ
cleanup_file_list_stdin (ಶೂನ್ಯ)
{
    ಆದರೆ (have_read_stdin && fclose (stdin) != 0)
      error (EXIT_FAILURE, errno, "-");
}

/* Emit the --help output for --tabs=LIST option accepted by expand and
   unexpand.  */
ಬಾಹ್ಯ ಶೂನ್ಯ
emit_tab_list_info (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಕಾರ್ಯಕ್ರಮ)
{
  /* suppress syntax check for emit_mandatory_arg_note() */
  oputs_ (ಕಾರ್ಯಕ್ರಮ, _("\
  -t, --tabs=LIST\n\
         use comma separated list ದ tab positions.\n\
"));
  fputs (_("\
         The ಕೊನೆಯ specified position can be prefixed with '/'\n\
         ಗಮ್ಯ specify a tab ಗಾತ್ರ_2 ಗಮ್ಯ use after the ಕೊನೆಯ\n\
         explicitly specified tab stop.  Also a ಪೂರ್ವಪ್ರತ್ಯಯ ದ '+'\n\
         can be ಬಳಸಿದ ಗಮ್ಯ align ಉಳಿದ tab stops relative ಗಮ್ಯ\n\
         the ಕೊನೆಯ specified tab stop instead ದ the ಮೊದಲ ಸ್ತಂಭ\n\
"), stdout);
}
