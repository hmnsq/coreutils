/* pr -- convert text files for printing.
   Copyright (C) 1988-2026 Free Software Foundation, Inc.

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

/*  By Pete TerMaat, with considerable refinement by Roland Huebner.  */

/* Things to watch: Sys V screws up on ...
   pr -n -3 -s: /usr/dict/words
   pr -m -o10 -n /usr/dict/words{,,,}
   pr -6 -a -n -o5 /usr/dict/words

   Ideas:

   Keep a things_to_do list of functions to call when we know we have
   something to print.  Cleaner than current series of checks.

   Improve the printing of control prefixes.

   Expand the file name in the centered header line to a full file name.


   Concept:

   If the input_tab_char differs from the default value TAB
   ('-e[CHAR[...]]' is used), any input text tab is expanded to the
   default width of 8 spaces (compare char_to_clump). - Same as SunOS
   does.

   The treatment of the number_separator (compare add_line_number):
   The default value TAB of the number_separator ('-n[SEP[...]]') doesn't
   be thought to be an input character. An optional '-e'-input has no
   effect.
   -  With single column output
      only one POSIX requirement has to be met:
   The default n-separator should be a TAB. The consequence is a
   different width between the number and the text if the output position
   of the separator changes, i.e., it depends upon the left margin used.
   That's not nice but easy-to-use together with the defaults of other
   utilities, e.g. sort or cut. - Same as SunOS does.
   -  With multicolumn output
      two conflicting POSIX requirements exist:
   First "default n-separator is TAB", second "output text columns shall
   be of equal width". Moreover POSIX specifies the number+separator a
   part of the column, together with '-COLS' and '-a -COLS'.
   (With -m output the number shall occupy each line only once. Exactly
   the same situation as single column output exists.)
      GNU pr gives priority to the 2nd requirement and observes POSIX
   column definition. The n-separator TAB is expanded to the same number
   of spaces in each column using the default value 8. Tabification is
   only performed if it is compatible with the output position.
   Consequence: The output text columns are of equal width. The layout
   of a page does not change if the left margin varies. - Looks better
   than the SunOS approach.
      SunOS pr gives priority to the 1st requirement. n-separator TAB
   width varies with each column. Only the width of text part of the
   column is fixed.
   Consequence: The output text columns don't have equal width. The
   widths and the layout of the whole page varies with the left margin.
   An overflow of the line length (without margin) over the input value
   PAGE_WIDTH may occur.

   The interference of the POSIX-compliant small letter options -w and -s:
   ("interference" means "setting a _separator_ with -s switches off the
   column structure and the default - not generally - page_width,
   acts on -w option")
       options:       text form  / separator:     equivalent new options:
       -w l   -s[x]
    --------------------------------------------------------------------
    1.  --     --     columns    / space          --
                      trunc. to page_width = 72
    2.  --    -s[:]   full lines / TAB[:]         -J  --sep-string[="<TAB>"|:]
                      no truncation
    3.  -w l   --     columns    / space          -W l
                      trunc. to page_width = l
    4.  -w l  -s[:]   columns    / no sep.[:]     -W l  --sep-string[=:]
                      trunc. to page_width = l
    --------------------------------------------------------------------


   Options:

   Including version 1.22i:
   Some SMALL LETTER options have been redefined with the object of a
   better POSIX compliance. The output of some further cases has been
   adapted to other UNIXes. A violation of downward compatibility has to
   be accepted.
   Some NEW CAPITAL LETTER options ( -J, -S, -W) has been introduced to
   turn off unexpected interference of small letter options (-s and -w
   together with the three column options).
   -N option and the second argument LAST_PAGE of +FIRST_PAGE offer more
   flexibility; The detailed handling of form feeds set in the input
   files requires -T option.

   Capital letter options dominate small letter ones.

   Some of the option-arguments cannot be specified as separate arguments
   from the preceding option letter (already stated in POSIX specification).

   Form feeds in the input cause page breaks in the output. Multiple
   form feeds produce empty pages.

   +FIRST_PAGE[:LAST_PAGE], --pages=FIRST_PAGE[:LAST_PAGE]
                begin [stop] printing with page FIRST_[LAST_]PAGE

   -COLS, --columns=COLS
                Produce output that is COLS columns wide and
                print columns down, unless -a is used. Balance number of
                lines in the columns on each page.

   -a, --across		Print columns across rather than down, used
                together with -COLS. The input
                one
                two
                three
                four
                will be printed with '-a -3' as
                one	two	three
                four

   -b		Balance columns on the last page.
                -b is no longer an independent option. It's always used
                together with -COLS (unless -a is used) to get a
                consistent formulation with "FF set by hand" in input
                files. Each formfeed found terminates the number of lines
                to be read with the actual page. The situation for
                printing columns down is equivalent to that on the last
                page. So we need a balancing.

                Keeping -b as an underground option guarantees some
                downward compatibility. Utilities using pr with -b
                (a most frequently used form) still work as usual.

   -c, --show-control-chars
                Print nonprintable characters as control prefixes.
                Control-g is printed as ^G (use hat notation) and
                octal backslash notation.

   -d, --double-space	Double space the output.

   -D FORMAT, --date-format=FORMAT  Use FORMAT for the header date.

   -e[CHAR[WIDTH]], --expand-tabs[=CHAR[WIDTH]]
                Expand tabs to spaces on input.  Optional argument CHAR
                is the input TAB character. (Default is TAB).  Optional
                argument WIDTH is the input TAB character's width.
                (Default is 8.)

   -F, -f, --form-feed	Use formfeeds instead of newlines to separate
                pages. A three line HEADER is used, no TRAILER with -F,
                without -F both HEADER and TRAILER are made of five lines.

   -h HEADER, --header=HEADER
                Replace the filename in the header with the string HEADER.
                A centered header is used.

   -i[CHAR[WIDTH]], --output-tabs[=CHAR[WIDTH]]
                Replace spaces with tabs on output.  Optional argument
                CHAR is the output TAB character. (Default is TAB).
                Optional argument WIDTH is the output TAB character's
                width. (Default is 8)

   -J, --join-lines	Merge lines of full length, turns off -W/-w
                line truncation, no column alignment, --sep-string[=STRING]
                sets separators, works with all column options
                (-COLS | -a -COLS | -m).
                -J has been introduced (together with -W and --sep-string) to
                disentangle the old (POSIX compliant) options -w, -s
                along with the 3 column options.

   -l PAGE_LENGTH, --length=PAGE_LENGTH
                Set the page length to PAGE_LENGTH lines. Default is 66,
                including 5 lines of HEADER and 5 lines of TRAILER
                without -F, but only 3 lines of HEADER and no TRAILER
                with -F (i.e the number of text lines defaults to 56 or
                63 respectively).

   -m, --merge		Print files in parallel; pad_across_to align
                columns; truncate lines and print separator strings;
                Do it also with empty columns to get a continuous line
                numbering and column marking by separators throughout
                the whole merged file.

                Empty pages in some input files produce empty columns
                [marked by separators] in the merged pages. Completely
                empty merged pages show no column separators at all.

                The layout of a merged page is ruled by the largest form
                feed distance of the single pages at that page. Shorter
                columns will be filled up with empty lines.

                Together with -J option join lines of full length and
                set separators when -S option is used.

   -n[SEP[DIGITS]], --number-lines[=SEP[DIGITS]]
                Provide DIGITS digit line numbering (default for DIGITS
                is 5). With multicolumn output the number occupies the
                first DIGITS column positions of each text column or only
                each line of -m output.
                With single column output the number precedes each line
                just as -m output.
                Optional argument SEP is the character appended to the
                line number to separate it from the text followed.
                The default separator is a TAB. In a strict sense a TAB
                is always printed with single column output only. The
                TAB-width varies with the TAB-position, e.g. with the
                left margin specified by -o option.
                With multicolumn output priority is given to "equal width
                of output columns" (a POSIX specification). The TAB-width
                is fixed to the value of the 1st column and does not
                change with different values of left margin. That means a
                fixed number of spaces is always printed in the place of
                a TAB. The tabification depends upon the output
                position.

                Default counting of the line numbers starts with 1st
                line of the input file (not the 1st line printed,
                compare the --page option and -N option).

   -N NUMBER, --first-line-number=NUMBER
                Start line counting with the number NUMBER at the 1st
                line of first page printed (mostly not the 1st line of
                the input file).

   -o MARGIN, --indent=MARGIN
                Offset each line with a margin MARGIN spaces wide.
                Total page width is the size of the margin plus the
                PAGE_WIDTH set with -W/-w option.

   -r, --no-file-warnings
                Omit warning when a file cannot be opened.

   -s[CHAR], --separator[=CHAR]
                Separate columns by a single character CHAR, default for
                CHAR is the TAB character without -w and 'no char' with -w.
                Without '-s' default separator 'space' is set.
                -s[CHAR] turns off line truncation of all 3 column options
                (-COLS|-a -COLS|-m) except -w is set. That is a POSIX
                compliant formulation. The source code translates -s into
                the new options -S and -J, also -W if required.

   -S[STRING], --sep-string[=STRING]
                Separate columns by any string STRING. The -S option
                doesn't react upon the -W/-w option (unlike -s option
                does). It defines a separator nothing else.
                Without -S: Default separator TAB is used with -J and
                'space' otherwise (same as -S" ").
                With -S "": No separator is used.
                Quotes should be used with blanks and some shell active
                characters.
                -S is problematic because in its obsolete form you
                cannot use -S "STRING", but in its standard form you
                must use -S "STRING" if STRING is empty.  Use
                --sep-string to avoid the ambiguity.

   -t, --omit-header	Do not print headers or footers but retain form
                feeds set in the input files.

   -T, --omit-pagination
                Do not print headers or footers, eliminate any pagination
                by form feeds set in the input files.

   -v, --show-nonprinting
                Print nonprintable characters as escape sequences. Use
                octal backslash notation. Control-G becomes \007.

   -w PAGE_WIDTH, --width=PAGE_WIDTH
                Set page width to PAGE_WIDTH characters for multiple
                text-column output only (default for PAGE_WIDTH is 72).
                -s[CHAR] turns off the default page width and any line
                truncation. Lines of full length will be merged,
                regardless of the column options set. A POSIX compliant
                formulation.

   -W PAGE_WIDTH, --page-width=PAGE_WIDTH
                Set the page width to PAGE_WIDTH characters. That's valid
                with and without a column option. Text lines will be
                truncated, unless -J is used. Together with one of the
                column options (-COLS| -a -COLS| -m) column alignment
                is always used.
                Default is 72 characters.
                Without -W PAGE_WIDTH
                - but with one of the column options default truncation of
                  72 characters is used (to keep downward compatibility
                  and to simplify most frequently met column tasks).
                  Column alignment and column separators are used.
                - and without any of the column options NO line truncation
                  is used (to keep downward compatibility and to meet most
                  frequent tasks). That's equivalent to  -W 72 -J .

                With/without  -W PAGE_WIDTH  the header line is always
                truncated to avoid line overflow.

                (In pr versions newer than 1.14 -S option does no longer
                affect -W option.)

*/

#ಅನ್ತರ್ಭಾವ <config.h>

#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "c-ctype.h"
#ಅನ್ತರ್ಭಾವ "fadvise.h"
#ಅನ್ತರ್ಭಾವ "hard-locale.h"
#ಅನ್ತರ್ಭಾವ "mbswidth.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "stat-time.h"
#ಅನ್ತರ್ಭಾವ "stdio--.h"
#ಅನ್ತರ್ಭಾವ "strftime.h"
#ಅನ್ತರ್ಭಾವ "xstrtol.h"
#ಅನ್ತರ್ಭಾವ "xstrtol-error.h"
#ಅನ್ತರ್ಭಾವ "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "pr"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS \
  proper_name ("Pete TerMaat"), \
  proper_name ("Roland Huebner")

/* Used with start_position in the struct COLUMN described below.
   If start_position == ANYWHERE, we aren't truncating columns and
   can begin printing a column anywhere.  Otherwise we must pad to
   the horizontal position start_position. */
#ವ್ಯಾಖ್ಯಾನಿಸು ANYWHERE	0

/* Each column has one of these structures allocated for it.
   If we're only dealing with one file, fp is the same for all
   columns.

   The general strategy is to spend time setting up these column
   structures (storing columns if necessary), after which printing
   is a matter of flitting from column to column and calling
   print_func.

   Parallel files, single files printing across in multiple
   columns, and single files printing down in multiple columns all
   fit the same printing loop.

   print_func		Function used to print lines in this column.
                        If we're storing this column it will be
                        print_stored(), Otherwise it will be read_line().

   char_func		Function used to process characters in this column.
                        If we're storing this column it will be store_char(),
                        otherwise it will be print_char().

   current_line		Index of the current entry in line_vector, which
                        contains the index of the first character of the
                        current line in buff[].

   lines_stored		Number of lines in this column which are stored in
                        buff.

   lines_to_print	If we're storing this column, lines_to_print is
                        the number of stored_lines which remain to be
                        printed.  Otherwise it is the number of lines
                        we can print without exceeding lines_per_body.

   start_position	The horizontal position we want to be in before we
                        print the first character in this column.

   numbered		True means precede this column with a line number. */

/* FIXME: There are many unchecked integer overflows in this file,
   that will cause this command to misbehave given large inputs or
   options.  Many of the "int" values below should be "size_t" or
   something else like that.  */

ರಚನೆ ಸ್ತಂಭ_3;
ರಚನೆ ಸ್ತಂಭ_3
  {
    FILE *fp;			/* Input stream for this column. */
    ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name;		/* File name. */
    ಎಣಿಕೆ
      {
        OPEN,
        FF_ಸಿಕ್ಕಿತು,		/* used with -b option, set with \f, changed
                                   to ON_HOLD after print_header */
        ON_HOLD,		/* Hit a form feed. */
        CLOSED
      }
    ಸ್ಥಿತಿ_2;			/* Status of the file pointer. */

    /* Func to print lines in this col. */
    bool (*print_func) (ರಚನೆ ಸ್ತಂಭ_3 *);

    /* Func to print/store chars in this col. */
    ಶೂನ್ಯ (*char_func) (ಅಕ್ಷರ);

    ಪೂರ್ಣಾಂಕ current_ಸಾಲು;		/* Index of current place in line_vector. */
    ಪೂರ್ಣಾಂಕ ಸಾಲುಗಳು_stored;		/* Number of lines stored in buff. */
    ಪೂರ್ಣಾಂಕ ಸಾಲುಗಳು_ಗಮ್ಯ_print;		/* No. lines stored or space left on page. */
    ಪೂರ್ಣಾಂಕ ಆರಂಭ_position;		/* Horizontal position of first char. */
    bool numbered;
    bool ತುಂಬಿದ_ಪುಟ_printed;	/* True means printed without a FF found. */

    /* p->full_page_printed  controls a special case of "FF set by hand":
       True means a full page has been printed without FF found. To avoid an
       additional empty page we have to ignore a FF immediately following in
       the next line. */
  };

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ ಸ್ತಂಭ_3 ಸ್ತಂಭ_3;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ char_ಗಮ್ಯ_clump (ಅಕ್ಷರ c);
ಸ್ಥಿರ bool read_ಸಾಲು (ಸ್ತಂಭ_3 *p);
ಸ್ಥಿರ bool print_ಪುಟ (ಶೂನ್ಯ);
ಸ್ಥಿರ bool print_stored (ಸ್ತಂಭ_3 *p);
ಸ್ಥಿರ bool open_file (ಅಕ್ಷರ *name, ಸ್ತಂಭ_3 *p);
ಸ್ಥಿರ bool skip_ಗಮ್ಯ_ಪುಟ (uintmax_t ಪುಟ);
ಸ್ಥಿರ ಶೂನ್ಯ print_ಶೀರ್ಷಿಕೆ (ಶೂನ್ಯ);
ಸ್ಥಿರ ಶೂನ್ಯ pad_across_ಗಮ್ಯ (ಪೂರ್ಣಾಂಕ position);
ಸ್ಥಿರ ಶೂನ್ಯ ಕೂಡಿಸು_ಸಾಲು_ಸಂಖ್ಯೆ (ಸ್ತಂಭ_3 *p);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ getoptnum (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *n_str, ಪೂರ್ಣಾಂಕ min, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *errfmt);
ಸ್ಥಿರ ಶೂನ್ಯ getoptarg (ಅಕ್ಷರ *arg, ಅಕ್ಷರ switch_char, ಅಕ್ಷರ *character,
                       ಪೂರ್ಣಾಂಕ *ಸಂಖ್ಯೆ);
ಸ್ಥಿರ ಶೂನ್ಯ print_files (ಪೂರ್ಣಾಂಕ ಸಂಖ್ಯೆ_ದ_files, ಅಕ್ಷರ **av);
ಸ್ಥಿರ ಶೂನ್ಯ init_parameters (ಪೂರ್ಣಾಂಕ ಸಂಖ್ಯೆ_ದ_files);
ಸ್ಥಿರ ಶೂನ್ಯ init_ಶೀರ್ಷಿಕೆ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename, ಪೂರ್ಣಾಂಕ desc);
ಸ್ಥಿರ bool init_fps (ಪೂರ್ಣಾಂಕ ಸಂಖ್ಯೆ_ದ_files, ಅಕ್ಷರ **av);
ಸ್ಥಿರ ಶೂನ್ಯ init_funcs (ಶೂನ್ಯ);
ಸ್ಥಿರ ಶೂನ್ಯ init_store_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ (ಶೂನ್ಯ);
ಸ್ಥಿರ ಶೂನ್ಯ store_columns (ಶೂನ್ಯ);
ಸ್ಥಿರ ಶೂನ್ಯ balance (ಪೂರ್ಣಾಂಕ ಒಟ್ಟು_stored);
ಸ್ಥಿರ ಶೂನ್ಯ store_char (ಅಕ್ಷರ c);
ಸ್ಥಿರ ಶೂನ್ಯ pad_ಕೆಳಕ್ಕೆ (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಸಾಲುಗಳು);
ಸ್ಥಿರ ಶೂನ್ಯ read_rest_ದ_ಸಾಲು (ಸ್ತಂಭ_3 *p);
ಸ್ಥಿರ ಶೂನ್ಯ skip_read (ಸ್ತಂಭ_3 *p, ಪೂರ್ಣಾಂಕ ಸ್ತಂಭ_ಸಂಖ್ಯೆ);
ಸ್ಥಿರ ಶೂನ್ಯ print_char (ಅಕ್ಷರ c);
ಸ್ಥಿರ ಶೂನ್ಯ cleanup (ಶೂನ್ಯ);
ಸ್ಥಿರ ಶೂನ್ಯ print_sep_string (ಶೂನ್ಯ);
ಸ್ಥಿರ ಶೂನ್ಯ separator_string (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *optarg_S);

/* All of the columns to print.  */
ಸ್ಥಿರ ಸ್ತಂಭ_3 *ಸ್ತಂಭ_ಸದಿಶ;

/* When printing a single file in multiple downward columns,
   we store the leftmost columns contiguously in buff.
   To print a line from buff, get the index of the first character
   from line_vector[i], and print up to line_vector[i + 1]. */
ಸ್ಥಿರ ಅಕ್ಷರ *buff;

/* Index of the position in buff where the next character
   will be stored. */
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ buff_current;

/* The number of characters in buff.
   Used for allocation of buff and to detect overflow of buff. */
ಸ್ಥಿರ idx_t buff_allocated;

/* Array of indices into buff.
   Each entry is an index of the first character of a line.
   This is used when storing lines to facilitate shuffling when
   we do column balancing on the last page. */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ *ಸಾಲು_ಸದಿಶ;

/* Array of horizontal positions.
   For each line in line_vector, end_vector[line] is the horizontal
   position we are in after printing that line.  We keep track of this
   so that we know how much we need to pad to prepare for the next
   column. */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ *ಕೊನೆ_ಸದಿಶ;

/* (-m) True means we're printing multiple files in parallel. */
ಸ್ಥಿರ bool parallel_files = false;

/* (-m) True means a line starts with some empty columns (some files
   already CLOSED or ON_HOLD) which we have to align. */
ಸ್ಥಿರ bool align_ಖಾಲಿಗಳು_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;

/* (-m) True means we have not yet found any printable column in a line.
   align_empty_cols = true  has to be maintained. */
ಸ್ಥಿರ bool ಖಾಲಿಗಳು_ಸಾಲು;

/* (-m) False means printable column output precedes a form feed found.
   Column alignment is done only once. No additional action with that form
   feed.
   True means we found only a form feed in a column. Maybe we have to do
   some column alignment with that form feed. */
ಸ್ಥಿರ bool FF_only;

/* (-[0-9]+) True means we're given an option explicitly specifying
   number of columns.  Used to detect when this option is used with -m
   and when translating old options to new/long options. */
ಸ್ಥಿರ bool explicit_columns = false;

/* (-t|-T) False means we aren't printing headers and footers. */
ಸ್ಥಿರ bool extremities = true;

/* (-t) True means we retain all FF set by hand in input files.
   False is set with -T option. */
ಸ್ಥಿರ bool keep_FF = false;
ಸ್ಥಿರ bool print_a_FF = false;

/* True means we need to print a header as soon as we know we've got input
   to print after it. */
ಸ್ಥಿರ bool print_a_ಶೀರ್ಷಿಕೆ;

/* (-f) True means use formfeeds instead of newlines to separate pages. */
ಸ್ಥಿರ bool use_form_feed = false;

/* True means we have read the standard input. */
ಸ್ಥಿರ bool have_read_stdin = false;

/* True means the -a flag has been given. */
ಸ್ಥಿರ bool print_across_flag = false;

/* True means we're printing one file in multiple (>1) downward columns. */
ಸ್ಥಿರ bool storing_columns = true;

/* (-b) True means balance columns on the last page as Sys V does. */
/* That's no longer an independent option. With storing_columns = true
   balance_columns = true is used too (s. function init_parameters).
   We get a consistent formulation with "FF set by hand" in input files. */
ಸ್ಥಿರ bool balance_columns = false;

/* (-l) Number of lines on a page, including header and footer lines. */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಸಾಲುಗಳು_ಪ್ರತಿ_ಪುಟ = 66;

/* Number of lines in the header and footer can be reset to 0 using
   the -t flag. */
ಎಣಿಕೆ { ಸಾಲುಗಳು_ಪ್ರತಿ_ಶೀರ್ಷಿಕೆ = 5 };
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಸಾಲುಗಳು_ಪ್ರತಿ_body;
ಎಣಿಕೆ { ಸಾಲುಗಳು_ಪ್ರತಿ_footer = 5 };

/* (-w|-W) Width in characters of the page.  Does not include the width of
   the margin. */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ chars_ಪ್ರತಿ_ಸಾಲು = 72;

/* (-w|W) True means we truncate lines longer than chars_per_column. */
ಸ್ಥಿರ bool truncate_ಸಾಲುಗಳು = false;

/* (-J) True means we join lines without any line truncation. -J
   dominates -w option. */
ಸ್ಥಿರ bool join_ಸಾಲುಗಳು = false;

/* Number of characters in a column.  Based on col_sep_length and
   page width. */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ chars_ಪ್ರತಿ_ಸ್ತಂಭ;

/* (-e) True means convert tabs to spaces on input. */
ಸ್ಥಿರ bool untabify_ಒಳಹರಿವು = false;

/* (-e) The input tab character. */
ಸ್ಥಿರ ಅಕ್ಷರ ಒಳಹರಿವು_tab_char = '\t';

/* (-e) Tabstops are at chars_per_tab, 2*chars_per_tab, 3*chars_per_tab, ...
   where the leftmost column is 1. */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ chars_ಪ್ರತಿ_ಒಳಹರಿವು_tab = 8;

/* (-i) True means convert spaces to tabs on output. */
ಸ್ಥಿರ bool tabify_output = false;

/* (-i) The output tab character. */
ಸ್ಥಿರ ಅಕ್ಷರ output_tab_char = '\t';

/* (-i) The width of the output tab. */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ chars_ಪ್ರತಿ_output_tab = 8;

/* Keeps track of pending white space.  When we hit a nonspace
   character after some whitespace, we print whitespace, tabbing
   if necessary to get to output_position + spaces_not_printed. */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ spaces_not_printed;

/* (-o) Number of spaces in the left margin (tabs used when possible). */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ chars_ಪ್ರತಿ_margin = 0;

/* Position where the next character will fall.
   Leftmost position is 0 + chars_per_margin.
   Rightmost position is chars_per_margin + chars_per_line - 1.
   This is important for converting spaces to tabs on output. */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ output_position;

/* Horizontal position relative to the current file.
   (output_position depends on where we are on the page;
   input_position depends on where we are in the file.)
   Important for converting tabs to spaces on input. */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಒಳಹರಿವು_position;

/* True if there were any failed opens so we can exit with nonzero
   status.  */
ಸ್ಥಿರ bool failed_opens = false;

/* The number of spaces taken up if we print a tab character with width
   c_ from position h_. */
#ವ್ಯಾಖ್ಯಾನಿಸು TAB_ಅಗಲ(c_, h_) ((c_) - ((h_) % (c_)))

/* The horizontal position we'll be at after printing a tab character
   of width c_ from the position h_. */
#ವ್ಯಾಖ್ಯಾನಿಸು POS_AFTER_TAB(c_, h_) ((h_) + TAB_ಅಗಲ (c_, h_))

/* (-NNN) Number of columns of text to print. */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ columns = 1;

/* (+NNN:MMM) Page numbers on which to begin and stop printing.
   first_page_number = 0  will be used to check input only. */
ಸ್ಥಿರ uintmax_t ಮೊದಲ_ಪುಟ_ಸಂಖ್ಯೆ = 0;
ಸ್ಥಿರ uintmax_t ಕೊನೆಯ_ಪುಟ_ಸಂಖ್ಯೆ = UINTMAX_MAX;

/* Number of files open (not closed, not on hold). */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ files_ready_ಗಮ್ಯ_read = 0;

/* Current page number.  Displayed in header. */
ಸ್ಥಿರ uintmax_t ಪುಟ_ಸಂಖ್ಯೆ;

/* Current line number.  Displayed when -n flag is specified.

   When printing files in parallel (-m flag), line numbering is as follows:
   1    foo     goo     moo
   2    hoo     too     zoo

   When printing files across (-a flag), ...
   1    foo     2       moo     3       goo
   4    hoo     5       too     6       zoo

   Otherwise, line numbering is as follows:
   1    foo     3       goo     5       too
   2    moo     4       hoo     6       zoo */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಸಾಲು_ಸಂಖ್ಯೆ;

/* (-n) True means lines should be preceded by numbers. */
ಸ್ಥಿರ bool numbered_ಸಾಲುಗಳು = false;

/* (-n) Character which follows each line number. */
ಸ್ಥಿರ ಅಕ್ಷರ ಸಂಖ್ಯೆ_separator = '\t';

/* (-n) line counting starts with 1st line of input file (not with 1st
   line of 1st page printed). */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಸಾಲು_ಎಣಿಕೆ = 1;

/* (-n) True means counting of skipped lines starts with 1st line of
   input file. False means -N option is used in addition, counting of
   skipped lines not required. */
ಸ್ಥಿರ bool skip_ಎಣಿಕೆ = true;

/* (-N) Counting starts with start_line_number = NUMBER at 1st line of
   first page printed, usually not 1st page of input file. */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಆರಂಭ_ಸಾಲು_ಅಂಕೆ = 1;

/* (-n) Width in characters of a line number. */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ chars_ಪ್ರತಿ_ಸಂಖ್ಯೆ = 5;

/* Used when widening the first column to accommodate numbers -- only
   needed when printing files in parallel.  Includes width of both the
   number and the number_separator. */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಸಂಖ್ಯೆ_ಅಗಲ;

/* Buffer sprintf uses to format a line number. */
ಸ್ಥಿರ ಅಕ್ಷರ *ಸಂಖ್ಯೆ_buff;

/* (-v) True means nonprintable characters are printed as escape sequences.
   control-g becomes \007. */
ಸ್ಥಿರ bool use_esc_sequence = false;

/* (-c) True means nonprintable characters are printed as control prefixes.
   control-g becomes ^G. */
ಸ್ಥಿರ bool use_cntrl_ಪೂರ್ವಪ್ರತ್ಯಯ = false;

/* (-d) True means output is double spaced. */
ಸ್ಥಿರ bool double_space = false;

/* Number of files opened initially in init_files.  Should be 1
   unless we're printing multiple files in parallel. */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಒಟ್ಟು_files = 0;

/* (-r) True means don't complain if we can't open a file. */
ಸ್ಥಿರ bool ignore_failed_opens = false;

/* (-S) True means we separate columns with a specified string.
   -S option does not affect line truncation nor column alignment. */
ಸ್ಥಿರ bool use_ಲಂಬಸಾಲು_separator = false;

/* String used to separate columns if the -S option has been specified.
   Default without -S but together with one of the column options
   -a|COLUMN|-m is a 'space' and with the -J option a 'tab'. */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಲಂಬಸಾಲು_sep_string = "";
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_sep_ಉದ್ದಳತೆ = 0;
ಸ್ಥಿರ ಅಕ್ಷರ *ಸ್ತಂಭ_separator = (ಅಕ್ಷರ *) " ";
ಸ್ಥಿರ ಅಕ್ಷರ *ಸಾಲು_separator = (ಅಕ್ಷರ *) "\t";

/* Number of separator characters waiting to be printed as soon as we
   know that we have any input remaining to be printed. */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ separators_not_printed;

/* Position we need to pad to, as soon as we know that we have input
   remaining to be printed. */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ padding_not_printed;

/* True means we should pad the end of the page.  Remains false until we
   know we have a page to print. */
ಸ್ಥಿರ bool pad_vertically;

/* (-h) String of characters used in place of the filename in the header. */
ಸ್ಥಿರ ಅಕ್ಷರ *custom_ಶೀರ್ಷಿಕೆ;

/* (-D) Date format for the header.  */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *date_format;

/* The local time zone rules, as per the TZ environment variable.  */
ಸ್ಥಿರ timezone_t localtz;

/* Date and file name for the header.  */
ಸ್ಥಿರ ಅಕ್ಷರ *date_ಪಠ್ಯ;
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file_ಪಠ್ಯ;

/* Output columns available, not counting the date and file name.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಶೀರ್ಷಿಕೆ_ಅಗಲ_ಲಭ್ಯ;

ಸ್ಥಿರ ಅಕ್ಷರ *clump_buff;

/* True means we read the line no. lines_per_body in skip_read
   called by skip_to_page. That variable controls the coincidence of a
   "FF set by hand" and "full_page_printed", see above the definition of
   structure COLUMN. */
ಸ್ಥಿರ bool ಕೊನೆಯ_ಸಾಲು = false;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ಎಣಿಕೆ
{
  COLUMNS_OPTION = CHAR_MAX + 1,
  ಪುಟಗಳು_OPTION
};

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ short_options[] =
  "-0123456789D:FJN:S::TW:abcde::fh:i::l:mn::o:rs::tvw:";

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ long_options[] =
{
  {"pages", required_ವಾದ, ಖಾಲಿ, ಪುಟಗಳು_OPTION},
  {"columns", required_ವಾದ, ಖಾಲಿ, COLUMNS_OPTION},
  {"across", no_ವಾದ, ಖಾಲಿ, 'a'},
  {"show-control-chars", no_ವಾದ, ಖಾಲಿ, 'c'},
  {"double-space", no_ವಾದ, ಖಾಲಿ, 'd'},
  {"date-format", required_ವಾದ, ಖಾಲಿ, 'D'},
  {"expand-tabs", optional_ವಾದ, ಖಾಲಿ, 'e'},
  {"form-feed", no_ವಾದ, ಖಾಲಿ, 'f'},
  {"header", required_ವಾದ, ಖಾಲಿ, 'h'},
  {"output-tabs", optional_ವಾದ, ಖಾಲಿ, 'i'},
  {"join-lines", no_ವಾದ, ಖಾಲಿ, 'J'},
  {"length", required_ವಾದ, ಖಾಲಿ, 'l'},
  {"merge", no_ವಾದ, ಖಾಲಿ, 'm'},
  {"number-lines", optional_ವಾದ, ಖಾಲಿ, 'n'},
  {"first-line-number", required_ವಾದ, ಖಾಲಿ, 'N'},
  {"indent", required_ವಾದ, ಖಾಲಿ, 'o'},
  {"no-file-warnings", no_ವಾದ, ಖಾಲಿ, 'r'},
  {"separator", optional_ವಾದ, ಖಾಲಿ, 's'},
  {"sep-string", optional_ವಾದ, ಖಾಲಿ, 'S'},
  {"omit-header", no_ವಾದ, ಖಾಲಿ, 't'},
  {"omit-pagination", no_ವಾದ, ಖಾಲಿ, 'T'},
  {"show-nonprinting", no_ವಾದ, ಖಾಲಿ, 'v'},
  {"width", required_ವಾದ, ಖಾಲಿ, 'w'},
  {"page-width", required_ವಾದ, ಖಾಲಿ, 'W'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};

ಸ್ಥಿರ _Noreturn ಶೂನ್ಯ
integer_ಅತಿಪ್ರವಾಹ (ಶೂನ್ಯ)
{
  error (EXIT_FAILURE, 0, _("integer overflow"));
}

/* Return the number of columns that have either an open file or
   stored lines. */

ATTRIBUTE_ಶುದ್ಧ
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ
ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_ready_ಗಮ್ಯ_print (ಶೂನ್ಯ)
{
  ಸ್ತಂಭ_3 *q;
  ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ i;
  ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ n;

  n = 0;
  ಗಾಗಿ (q = ಸ್ತಂಭ_ಸದಿಶ, i = 0; i < columns; ++q, ++i)
    ಆದರೆ (q->ಸ್ಥಿತಿ_2 == OPEN
        || q->ಸ್ಥಿತಿ_2 == FF_ಸಿಕ್ಕಿತು	/* With -b: To print a header only */
        || (storing_columns && q->ಸಾಲುಗಳು_stored > 0 && q->ಸಾಲುಗಳು_ಗಮ್ಯ_print > 0))
      ++n;
  ಹಿಂತಿರುಗಿ n;
}

/* Estimate first_ / last_page_number
   using option +FIRST_PAGE:LAST_PAGE */

ಸ್ಥಿರ bool
ಮೊದಲ_ಕೊನೆಯ_ಪುಟ (ಪೂರ್ಣಾಂಕ oi, ಅಕ್ಷರ c, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಪುಟಗಳು)
{
  ಅಕ್ಷರ *p;
  uintmax_t ಮೊದಲ;
  uintmax_t ಕೊನೆಯ = UINTMAX_MAX;
  strtol_error err = xstrtoumax (ಪುಟಗಳು, &p, 10, &ಮೊದಲ, "");
  ಆದರೆ (err != LONGINT_OK && err != LONGINT_INVALID_SUFFIX_CHAR)
    xstrtol_fatal (err, oi, c, long_options, ಪುಟಗಳು);

  ಆದರೆ (p == ಪುಟಗಳು || !ಮೊದಲ)
    ಹಿಂತಿರುಗಿ false;

  ಆದರೆ (*p == ':')
    {
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *p1 = p + 1;
      err = xstrtoumax (p1, &p, 10, &ಕೊನೆಯ, "");
      ಆದರೆ (err != LONGINT_OK)
        xstrtol_fatal (err, oi, c, long_options, ಪುಟಗಳು);
      ಆದರೆ (p1 == p || ಕೊನೆಯ < ಮೊದಲ)
        ಹಿಂತಿರುಗಿ false;
    }

  ಆದರೆ (*p)
    ಹಿಂತಿರುಗಿ false;

  ಮೊದಲ_ಪುಟ_ಸಂಖ್ಯೆ = ಮೊದಲ;
  ಕೊನೆಯ_ಪುಟ_ಸಂಖ್ಯೆ = ಕೊನೆಯ;
  ಹಿಂತಿರುಗಿ true;
}

/* Parse column count string S, and if it's valid (1 or larger and
   within range of the type of 'columns') set the global variables
   columns and explicit_columns.  Otherwise, exit with a diagnostic.  */

ಸ್ಥಿರ ಶೂನ್ಯ
parse_ಸ್ತಂಭ_ಎಣಿಕೆ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *s)
{
  columns = getoptnum (s, 1, _("invalid number of columns"));
  explicit_columns = true;
}

/* Estimate length of col_sep_string with option -S.  */

ಸ್ಥಿರ ಶೂನ್ಯ
separator_string (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *optarg_S)
{
  size_t len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (optarg_S);
  ಆದರೆ (ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ < len)
    integer_ಅತಿಪ್ರವಾಹ ();
  ಲಂಬಸಾಲು_sep_ಉದ್ದಳತೆ = len;
  ಲಂಬಸಾಲು_sep_string = optarg_S;
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ n_files;
  bool old_options = false;
  bool old_w = false;
  bool old_s = false;
  ಅಕ್ಷರ **file_ಹೆಸರುಗಳು;

  /* Accumulate the digits of old-style options like -99.  */
  ಅಕ್ಷರ *ಸ್ತಂಭ_ಎಣಿಕೆ_string = ಖಾಲಿ;
  idx_t n_digits = 0;
  idx_t n_alloc = 0;

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  n_files = 0;
  file_ಹೆಸರುಗಳು = (argc > 1
                ? xnmalloc (argc - 1, ಗಾತ್ರ (ಅಕ್ಷರ *))
                : ಖಾಲಿ);

  ಆಗ (true)
    {
      ಪೂರ್ಣಾಂಕ oi = -1;
      ಪೂರ್ಣಾಂಕ c = getopt_long (argc, argv, short_options, long_options, &oi);
      ಆದರೆ (c == -1)
        ನಿಲ್ಲಿಸು;

      ಆದರೆ (c_isdigit (c))
        {
          /* Accumulate column-count digits specified via old-style options. */
          ಆದರೆ (n_digits + 1 >= n_alloc)
            ಸ್ತಂಭ_ಎಣಿಕೆ_string = xpalloc (ಸ್ತಂಭ_ಎಣಿಕೆ_string, &n_alloc, 2, -1,
                                           ಗಾತ್ರ *ಸ್ತಂಭ_ಎಣಿಕೆ_string);
          ಸ್ತಂಭ_ಎಣಿಕೆ_string[n_digits++] = c;
          ಸ್ತಂಭ_ಎಣಿಕೆ_string[n_digits] = '\0';
          ಮುಂದುವರಿ;
        }

      n_digits = 0;

      ಆಯ್ಕೆ (c)
        {
        ಸ್ಥಿತಿ 1:			/* Non-option argument. */
          /* long option --page dominates old '+FIRST_PAGE ...'.  */
          ಆದರೆ (! (ಮೊದಲ_ಪುಟ_ಸಂಖ್ಯೆ == 0
                 && *optarg == '+' && ಮೊದಲ_ಕೊನೆಯ_ಪುಟ (-2, '+', optarg + 1)))
            file_ಹೆಸರುಗಳು[n_files++] = optarg;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ ಪುಟಗಳು_OPTION:	/* --pages=FIRST_PAGE[:LAST_PAGE] */
          {			/* dominates old opt +... */
            ಆದರೆ (! optarg)
              error (EXIT_FAILURE, 0,
                     _("'--pages=FIRST_PAGE[:LAST_PAGE]' missing argument"));
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (! ಮೊದಲ_ಕೊನೆಯ_ಪುಟ (oi, 0, optarg))
              error (EXIT_FAILURE, 0, _("invalid page range %s"),
                     quote (optarg));
            ನಿಲ್ಲಿಸು;
          }

        ಸ್ಥಿತಿ COLUMNS_OPTION:	/* --columns=COLS */
          {
            parse_ಸ್ತಂಭ_ಎಣಿಕೆ (optarg);

            /* If there was a prior column count specified via the
               short-named option syntax, e.g., -9, ensure that this
               long-name-specified value overrides it.  */
            ಬಿಡುಗಡೆ (ಸ್ತಂಭ_ಎಣಿಕೆ_string);
            ಸ್ತಂಭ_ಎಣಿಕೆ_string = ಖಾಲಿ;
            n_alloc = 0;
            ನಿಲ್ಲಿಸು;
          }

        ಸ್ಥಿತಿ 'a':
          print_across_flag = true;
          storing_columns = false;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'b':
          balance_columns = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'c':
          use_cntrl_ಪೂರ್ವಪ್ರತ್ಯಯ = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'd':
          double_space = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'D':
          date_format = optarg;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'e':
          ಆದರೆ (optarg)
            getoptarg (optarg, 'e', &ಒಳಹರಿವು_tab_char,
                       &chars_ಪ್ರತಿ_ಒಳಹರಿವು_tab);
          /* Could check tab width > 0. */
          untabify_ಒಳಹರಿವು = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'f':
        ಸ್ಥಿತಿ 'F':
          use_form_feed = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'h':
          custom_ಶೀರ್ಷಿಕೆ = optarg;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'i':
          ಆದರೆ (optarg)
            getoptarg (optarg, 'i', &output_tab_char,
                       &chars_ಪ್ರತಿ_output_tab);
          /* Could check tab width > 0. */
          tabify_output = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'J':
          join_ಸಾಲುಗಳು = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'l':
          ಸಾಲುಗಳು_ಪ್ರತಿ_ಪುಟ
            = getoptnum (optarg, 1,
                         _("'-l PAGE_LENGTH' invalid number of lines"));
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'm':
          parallel_files = true;
          storing_columns = false;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'n':
          numbered_ಸಾಲುಗಳು = true;
          ಆದರೆ (optarg)
            getoptarg (optarg, 'n', &ಸಂಖ್ಯೆ_separator,
                       &chars_ಪ್ರತಿ_ಸಂಖ್ಯೆ);
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'N':
          skip_ಎಣಿಕೆ = false;
          ಆರಂಭ_ಸಾಲು_ಅಂಕೆ
            = getoptnum (optarg, INT_MIN,
                         _("'-N NUMBER' invalid starting line number"));
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'o':
          chars_ಪ್ರತಿ_margin = getoptnum (optarg, 0,
                                        _("'-o MARGIN' invalid line offset"));
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'r':
          ignore_failed_opens = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 's':
          old_options = true;
          old_s = true;
          ಆದರೆ (!use_ಲಂಬಸಾಲು_separator && optarg)
            separator_string (optarg);
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'S':
          old_s = false;
          /* Reset an additional input of -s, -S dominates -s */
          ಲಂಬಸಾಲು_sep_string = "";
          ಲಂಬಸಾಲು_sep_ಉದ್ದಳತೆ = 0;
          use_ಲಂಬಸಾಲು_separator = true;
          ಆದರೆ (optarg)
            separator_string (optarg);
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 't':
          extremities = false;
          keep_FF = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'T':
          extremities = false;
          keep_FF = false;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'v':
          use_esc_sequence = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'w':
          old_options = true;
          old_w = true;
          {
            ಪೂರ್ಣಾಂಕ tmp_cpl
              = getoptnum (optarg, 1,
                           _("'-w PAGE_WIDTH' invalid number of characters"));
            ಆದರೆ (! truncate_ಸಾಲುಗಳು)
              chars_ಪ್ರತಿ_ಸಾಲು = tmp_cpl;
          }
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'W':
          old_w = false;			/* dominates -w */
          truncate_ಸಾಲುಗಳು = true;
          chars_ಪ್ರತಿ_ಸಾಲು
            = getoptnum (optarg, 1,
                         _("'-W PAGE_WIDTH' invalid number of characters"));
          ನಿಲ್ಲಿಸು;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);
        ಪೂರ್ವನಿಯೋಜಿತ:
          usage (EXIT_FAILURE);
          ನಿಲ್ಲಿಸು;
        }
    }

  ಆದರೆ (ಸ್ತಂಭ_ಎಣಿಕೆ_string)
    {
      parse_ಸ್ತಂಭ_ಎಣಿಕೆ (ಸ್ತಂಭ_ಎಣಿಕೆ_string);
      ಬಿಡುಗಡೆ (ಸ್ತಂಭ_ಎಣಿಕೆ_string);
    }

  ಆದರೆ (! date_format)
    date_format = (getenv ("POSIXLY_CORRECT") && !hard_locale (LC_ಸಮಯ)
                   ? "%b %e %H:%M %Y"
                   : "%Y-%m-%d %H:%M");

  localtz = tzalloc (getenv ("TZ"));

  /* Now we can set a reasonable initial value: */
  ಆದರೆ (ಮೊದಲ_ಪುಟ_ಸಂಖ್ಯೆ == 0)
    ಮೊದಲ_ಪುಟ_ಸಂಖ್ಯೆ = 1;

  ಆದರೆ (parallel_files && explicit_columns)
    error (EXIT_FAILURE, 0,
           _("cannot specify number of columns when printing in parallel"));

  ಆದರೆ (parallel_files && print_across_flag)
    error (EXIT_FAILURE, 0,
           _("cannot specify both printing across and printing in parallel"));

/* Translate some old short options to new/long options.
   To meet downward compatibility with other UNIX pr utilities
   and some POSIX specifications. */

  ಆದರೆ (old_options)
    {
      ಆದರೆ (old_w)
        {
          ಆದರೆ (parallel_files || explicit_columns)
            {
              /* activate -W */
              truncate_ಸಾಲುಗಳು = true;
              ಆದರೆ (old_s)
                /* adapt HP-UX and SunOS: -s = no separator;
                   activate -S */
                use_ಲಂಬಸಾಲು_separator = true;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            /* old -w sets width with columns only
               activate -J */
            join_ಸಾಲುಗಳು = true;
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!use_ಲಂಬಸಾಲು_separator)
        {
          /* No -S option read */
          ಆದರೆ (old_s && (parallel_files || explicit_columns))
            {
              ಆದರೆ (!truncate_ಸಾಲುಗಳು)
                {
                  /* old -s (without -w and -W) annuls column alignment,
                  uses fields, activate -J */
                  join_ಸಾಲುಗಳು = true;
                  ಆದರೆ (ಲಂಬಸಾಲು_sep_ಉದ್ದಳತೆ > 0)
                    /* activate -S */
                    use_ಲಂಬಸಾಲು_separator = true;
                }
              ಇಲ್ಲದಿದ್ದರೆ
                /* with -W */
                /* adapt HP-UX and SunOS: -s = no separator;
                   activate -S */
                use_ಲಂಬಸಾಲು_separator = true;
            }
        }
    }

  ಗಾಗಿ (; optind < argc; optind++)
    {
      file_ಹೆಸರುಗಳು[n_files++] = argv[optind];
    }

  ಆದರೆ (n_files == 0)
    {
      /* No file arguments specified;  read from standard input.  */
      print_files (0, ಖಾಲಿ);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಆದರೆ (parallel_files)
        print_files (n_files, file_ಹೆಸರುಗಳು);
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಗಾಗಿ (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ i = 0; i < n_files; i++)
            print_files (1, &file_ಹೆಸರುಗಳು[i]);
        }
    }

  cleanup ();

  ಆದರೆ (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, _("standard input"));
  main_exit (failed_opens ? EXIT_FAILURE : EXIT_SUCCESS);
}

/* Parse numeric arguments, ensuring MIN <= number <= INT_MAX.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
getoptnum (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *n_str, ಪೂರ್ಣಾಂಕ min, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *err)
{
  ಹಿಂತಿರುಗಿ xnumtoimax (n_str, 10, min, ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ, "", err, 0,
                     min <= 0 ? 0 : XTOINT_MIN_RANGE);
}

/* Parse options of the form -scNNN.

   Example: -nck, where 'n' is the option, c is the optional number
   separator, and k is the optional width of the field used when printing
   a number. */

ಸ್ಥಿರ ಶೂನ್ಯ
getoptarg (ಅಕ್ಷರ *arg, ಅಕ್ಷರ switch_char, ಅಕ್ಷರ *character, ಪೂರ್ಣಾಂಕ *ಸಂಖ್ಯೆ)
{
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಅಂಕೆ = arg;

  ಆದರೆ (!*ಅಂಕೆ)
    {
      error (0, 0, _("'-%c': Invalid argument: %s"), switch_char, quote (ಅಂಕೆ));
      usage (EXIT_FAILURE);
    }

  ಆದರೆ (!c_isdigit (*ಅಂಕೆ))
    *character = *ಅಂಕೆ++;
  ಆದರೆ (*ಅಂಕೆ)
    {
      ದೀರ್ಘ ಪೂರ್ಣಾಂಕ tmp_long;
      strtol_error e = xstrtol (ಅಂಕೆ, ಖಾಲಿ, 10, &tmp_long, "");
      ಆದರೆ (e == LONGINT_OK)
        {
          ಆದರೆ (tmp_long <= 0)
            e = LONGINT_INVALID;
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ < tmp_long)
            e = LONGINT_ಅತಿಪ್ರವಾಹ;
        }
      ಆದರೆ (e != LONGINT_OK)
        {
          error (0, e & LONGINT_ಅತಿಪ್ರವಾಹ ? EOVERFLOW : 0,
             _("'-%c' extra characters or invalid number in the argument: %s"),
                 switch_char, quote (arg));
          usage (EXIT_FAILURE);
        }
      *ಸಂಖ್ಯೆ = tmp_long;
    }
}

/* Set parameters related to formatting. */

ಸ್ಥಿರ ಶೂನ್ಯ
init_parameters (ಪೂರ್ಣಾಂಕ ಸಂಖ್ಯೆ_ದ_files)
{
  ಪೂರ್ಣಾಂಕ chars_ಬಳಸಿದ_by_ಸಂಖ್ಯೆ = 0;

  ಸಾಲುಗಳು_ಪ್ರತಿ_body = ಸಾಲುಗಳು_ಪ್ರತಿ_ಪುಟ - ಸಾಲುಗಳು_ಪ್ರತಿ_ಶೀರ್ಷಿಕೆ - ಸಾಲುಗಳು_ಪ್ರತಿ_footer;
  ಆದರೆ (ಸಾಲುಗಳು_ಪ್ರತಿ_body <= 0)
    {
      extremities = false;
      keep_FF = true;
    }
  ಆದರೆ (! extremities)
    ಸಾಲುಗಳು_ಪ್ರತಿ_body = ಸಾಲುಗಳು_ಪ್ರತಿ_ಪುಟ;

  ಆದರೆ (double_space)
    ಸಾಲುಗಳು_ಪ್ರತಿ_body = MAX (1, ಸಾಲುಗಳು_ಪ್ರತಿ_body / 2);

  /* If input is stdin, cannot print parallel files.  BSD dumps core
     on this. */
  ಆದರೆ (ಸಂಖ್ಯೆ_ದ_files == 0)
    parallel_files = false;

  ಆದರೆ (parallel_files)
    columns = ಸಂಖ್ಯೆ_ದ_files;

  /* One file, multi columns down: -b option is set to get a consistent
     formulation with "FF set by hand" in input files. */
  ಆದರೆ (storing_columns)
    balance_columns = true;

  /* Tabification is assumed for multiple columns. */
  ಆದರೆ (columns > 1)
    {
      ಆದರೆ (!use_ಲಂಬಸಾಲು_separator)
        {
          /* Use default separator */
          ಆದರೆ (join_ಸಾಲುಗಳು)
            ಲಂಬಸಾಲು_sep_string = ಸಾಲು_separator;
          ಇಲ್ಲದಿದ್ದರೆ
            ಲಂಬಸಾಲು_sep_string = ಸ್ತಂಭ_separator;

          ಲಂಬಸಾಲು_sep_ಉದ್ದಳತೆ = 1;
          use_ಲಂಬಸಾಲು_separator = true;
        }
      /* It's rather pointless to define a TAB separator with column
         alignment */
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!join_ಸಾಲುಗಳು && ಲಂಬಸಾಲು_sep_ಉದ್ದಳತೆ == 1 && *ಲಂಬಸಾಲು_sep_string == '\t')
        ಲಂಬಸಾಲು_sep_string = ಸ್ತಂಭ_separator;

      truncate_ಸಾಲುಗಳು = true;
      ಆದರೆ (! (ಲಂಬಸಾಲು_sep_ಉದ್ದಳತೆ == 1 && *ಲಂಬಸಾಲು_sep_string == '\t'))
        untabify_ಒಳಹರಿವು = true;
      tabify_output = true;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    storing_columns = false;

  /* -J dominates -w in any case */
  ಆದರೆ (join_ಸಾಲುಗಳು)
    truncate_ಸಾಲುಗಳು = false;

  ಆದರೆ (numbered_ಸಾಲುಗಳು)
    {
      ಪೂರ್ಣಾಂಕ chars_ಪ್ರತಿ_default_tab = 8;

      ಸಾಲು_ಎಣಿಕೆ = ಆರಂಭ_ಸಾಲು_ಅಂಕೆ;

      /* To allow input tab-expansion (-e sensitive) use:
         if (number_separator == input_tab_char)
           number_width = chars_per_number
             + TAB_WIDTH (chars_per_input_tab, chars_per_number);   */

      /* Estimate chars_per_text without any margin and keep it constant. */
      ಆದರೆ (ಸಂಖ್ಯೆ_separator == '\t')
        ಸಂಖ್ಯೆ_ಅಗಲ = (chars_ಪ್ರತಿ_ಸಂಖ್ಯೆ
                        + TAB_ಅಗಲ (chars_ಪ್ರತಿ_default_tab, chars_ಪ್ರತಿ_ಸಂಖ್ಯೆ));
      ಇಲ್ಲದಿದ್ದರೆ
        ಸಂಖ್ಯೆ_ಅಗಲ = chars_ಪ್ರತಿ_ಸಂಖ್ಯೆ + 1;

      /* The number is part of the column width unless we are
         printing files in parallel. */
      ಆದರೆ (parallel_files)
        chars_ಬಳಸಿದ_by_ಸಂಖ್ಯೆ = ಸಂಖ್ಯೆ_ಅಗಲ;
    }

  ಪೂರ್ಣಾಂಕ sep_chars, useful_chars;
  ಆದರೆ (ckd_mul (&sep_chars, columns - 1, ಲಂಬಸಾಲು_sep_ಉದ್ದಳತೆ))
    sep_chars = ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ;
  ಆದರೆ (ckd_sub (&useful_chars, chars_ಪ್ರತಿ_ಸಾಲು - chars_ಬಳಸಿದ_by_ಸಂಖ್ಯೆ,
               sep_chars))
    useful_chars = 0;
  chars_ಪ್ರತಿ_ಸ್ತಂಭ = useful_chars / columns;

  ಆದರೆ (chars_ಪ್ರತಿ_ಸ್ತಂಭ < 1)
    error (EXIT_FAILURE, 0, _("page width too narrow"));

  ಆದರೆ (numbered_ಸಾಲುಗಳು)
    {
      ಬಿಡುಗಡೆ (ಸಂಖ್ಯೆ_buff);
      ಸಂಖ್ಯೆ_buff = xmalloc (MAX (chars_ಪ್ರತಿ_ಸಂಖ್ಯೆ,
                                  INT_STRLEN_BOUND (ಸಾಲು_ಸಂಖ್ಯೆ)) + 1);
    }

  /* Pick the maximum between the tab width and the width of an
     escape sequence.
     The width of an escape sequence (4) isn't the lower limit any longer.
     We've to use 8 as the lower limit, if we use chars_per_default_tab = 8
     to expand a tab which is not an input_tab-char. */
  ಬಿಡುಗಡೆ (clump_buff);
  clump_buff = xmalloc (MAX (8, chars_ಪ್ರತಿ_ಒಳಹರಿವು_tab));
}

/* Open the necessary files,
   maintaining a COLUMN structure for each column.

   With multiple files, each column p has a different p->fp.
   With single files, each column p has the same p->fp.
   Return false if (number_of_files > 0) and no files can be opened,
   true otherwise.

   With each column/file p, p->full_page_printed is initialized,
   see also open_file.  */

ಸ್ಥಿರ bool
init_fps (ಪೂರ್ಣಾಂಕ ಸಂಖ್ಯೆ_ದ_files, ಅಕ್ಷರ **av)
{
  ಸ್ತಂಭ_3 *p;

  ಒಟ್ಟು_files = 0;

  ಬಿಡುಗಡೆ (ಸ್ತಂಭ_ಸದಿಶ);
  ಸ್ತಂಭ_ಸದಿಶ = xnmalloc (columns, ಗಾತ್ರ (ಸ್ತಂಭ_3));

  ಆದರೆ (parallel_files)
    {
      ಪೂರ್ಣಾಂಕ files_ಎಡ = ಸಂಖ್ಯೆ_ದ_files;
      ಗಾಗಿ (p = ಸ್ತಂಭ_ಸದಿಶ; files_ಎಡ--; ++p, ++av)
        {
          ಆದರೆ (! open_file (*av, p))
            {
              --p;
              --columns;
            }
        }
      ಆದರೆ (columns == 0)
        ಹಿಂತಿರುಗಿ false;
      init_ಶೀರ್ಷಿಕೆ ("", -1);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      p = ಸ್ತಂಭ_ಸದಿಶ;
      ಆದರೆ (ಸಂಖ್ಯೆ_ದ_files > 0)
        {
          ಆದರೆ (! open_file (*av, p))
            ಹಿಂತಿರುಗಿ false;
          init_ಶೀರ್ಷಿಕೆ (*av, fileno (p->fp));
          p->ಸಾಲುಗಳು_stored = 0;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          p->name = _("standard input");
          p->fp = stdin;
          have_read_stdin = true;
          p->ಸ್ಥಿತಿ_2 = OPEN;
          p->ತುಂಬಿದ_ಪುಟ_printed = false;
          ++ಒಟ್ಟು_files;
          init_ಶೀರ್ಷಿಕೆ ("", -1);
          p->ಸಾಲುಗಳು_stored = 0;
        }

      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *firstname = p->name;
      FILE *firstfp = p->fp;
      ಪೂರ್ಣಾಂಕ i;
      ಗಾಗಿ (i = columns - 1, ++p; i; --i, ++p)
        {
          p->name = firstname;
          p->fp = firstfp;
          p->ಸ್ಥಿತಿ_2 = OPEN;
          p->ತುಂಬಿದ_ಪುಟ_printed = false;
          p->ಸಾಲುಗಳು_stored = 0;
        }
    }
  files_ready_ಗಮ್ಯ_read = ಒಟ್ಟು_files;
  ಹಿಂತಿರುಗಿ true;
}

/* Determine print_func and char_func, the functions
   used by each column for printing and/or storing.

   Determine the horizontal position desired when we begin
   printing a column (p->start_position). */

ಸ್ಥಿರ ಶೂನ್ಯ
init_funcs (ಶೂನ್ಯ)
{
  ಪೂರ್ಣಾಂಕ i, h, h_ಮುಂದಿನ;
  ಸ್ತಂಭ_3 *p;

  h = chars_ಪ್ರತಿ_margin;

  ಆದರೆ (!truncate_ಸಾಲುಗಳು)
    h_ಮುಂದಿನ = ANYWHERE;
  ಇಲ್ಲದಿದ್ದರೆ
    {
      /* When numbering lines of parallel files, we enlarge the
         first column to accommodate the number.  Looks better than
         the Sys V approach. */
      ಆದರೆ (parallel_files && numbered_ಸಾಲುಗಳು)
        h_ಮುಂದಿನ = h + chars_ಪ್ರತಿ_ಸ್ತಂಭ + ಸಂಖ್ಯೆ_ಅಗಲ;
      ಇಲ್ಲದಿದ್ದರೆ
        h_ಮುಂದಿನ = h + chars_ಪ್ರತಿ_ಸ್ತಂಭ;
    }

  /* Enlarge p->start_position of first column to use the same form of
     padding_not_printed with all columns. */
  h = h + ಲಂಬಸಾಲು_sep_ಉದ್ದಳತೆ;

  /* This loop takes care of all but the rightmost column. */

  ಗಾಗಿ (p = ಸ್ತಂಭ_ಸದಿಶ, i = 1; i < columns; ++p, ++i)
    {
      ಆದರೆ (storing_columns)	/* One file, multi columns down. */
        {
          p->char_func = store_char;
          p->print_func = print_stored;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        /* One file, multi columns across; or parallel files.  */
        {
          p->char_func = print_char;
          p->print_func = read_ಸಾಲು;
        }

      /* Number only the first column when printing files in
         parallel. */
      p->numbered = numbered_ಸಾಲುಗಳು && (!parallel_files || i == 1);
      p->ಆರಂಭ_position = h;

      /* If we don't truncate lines, all start_positions are
         ANYWHERE, except the first column's start_position when
         using a margin. */

      ಆದರೆ (!truncate_ಸಾಲುಗಳು)
        {
          h = ANYWHERE;
          h_ಮುಂದಿನ = ANYWHERE;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          h = h_ಮುಂದಿನ + ಲಂಬಸಾಲು_sep_ಉದ್ದಳತೆ;
          h_ಮುಂದಿನ = h + chars_ಪ್ರತಿ_ಸ್ತಂಭ;
        }
    }

  /* The rightmost column.

     Doesn't need to be stored unless we intend to balance
     columns on the last page. */
  ಆದರೆ (storing_columns && balance_columns)
    {
      p->char_func = store_char;
      p->print_func = print_stored;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      p->char_func = print_char;
      p->print_func = read_ಸಾಲು;
    }

  p->numbered = numbered_ಸಾಲುಗಳು && (!parallel_files || i == 1);
  p->ಆರಂಭ_position = h;
}

/* Open a file.  Return true if successful.

   With each file p, p->full_page_printed is initialized,
   see also init_fps. */

ಸ್ಥಿರ bool
open_file (ಅಕ್ಷರ *name, ಸ್ತಂಭ_3 *p)
{
  ಆದರೆ (streq (name, "-"))
    {
      p->name = _("standard input");
      p->fp = stdin;
      have_read_stdin = true;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      p->name = name;
      p->fp = fopen (name, "r");
    }
  ಆದರೆ (p->fp == ಖಾಲಿ)
    {
      failed_opens = true;
      ಆದರೆ (!ignore_failed_opens)
        error (0, errno, "%s", quotef (name));
      ಹಿಂತಿರುಗಿ false;
    }
  fadvise (p->fp, FADVISE_SEQUENTIAL);
  p->ಸ್ಥಿತಿ_2 = OPEN;
  p->ತುಂಬಿದ_ಪುಟ_printed = false;
  ++ಒಟ್ಟು_files;
  ಹಿಂತಿರುಗಿ true;
}

/* Close the file in P.

   If we aren't dealing with multiple files in parallel, we change
   the status of all columns in the column list to reflect the close. */

ಸ್ಥಿರ ಶೂನ್ಯ
close_file (ಸ್ತಂಭ_3 *p)
{
  ಸ್ತಂಭ_3 *q;
  ಪೂರ್ಣಾಂಕ i;

  ಆದರೆ (p->ಸ್ಥಿತಿ_2 == CLOSED)
    ಹಿಂತಿರುಗಿ;

  ಪೂರ್ಣಾಂಕ err = errno;
  ಆದರೆ (!ferror (p->fp))
    err = 0;
  ಆದರೆ (fileno (p->fp) == STDIN_FILENO)
    clearerr (p->fp);
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (fclose (p->fp) != 0 && !err)
    err = errno;
  ಆದರೆ (err)
    error (EXIT_FAILURE, err, "%s", quotef (p->name));

  ಆದರೆ (!parallel_files)
    {
      ಗಾಗಿ (q = ಸ್ತಂಭ_ಸದಿಶ, i = columns; i; ++q, --i)
        {
          q->ಸ್ಥಿತಿ_2 = CLOSED;
          ಆದರೆ (q->ಸಾಲುಗಳು_stored == 0)
            {
              q->ಸಾಲುಗಳು_ಗಮ್ಯ_print = 0;
            }
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      p->ಸ್ಥಿತಿ_2 = CLOSED;
      p->ಸಾಲುಗಳು_ಗಮ್ಯ_print = 0;
    }

  --files_ready_ಗಮ್ಯ_read;
}

/* Put a file on hold until we start a new page,
   since we've hit a form feed.

   If we aren't dealing with parallel files, we must change the
   status of all columns in the column list. */

ಸ್ಥಿರ ಶೂನ್ಯ
hold_file (ಸ್ತಂಭ_3 *p)
{
  ಸ್ತಂಭ_3 *q;
  ಪೂರ್ಣಾಂಕ i;

  ಆದರೆ (!parallel_files)
    ಗಾಗಿ (q = ಸ್ತಂಭ_ಸದಿಶ, i = columns; i; ++q, --i)
      {
        ಆದರೆ (storing_columns)
          q->ಸ್ಥಿತಿ_2 = FF_ಸಿಕ್ಕಿತು;
        ಇಲ್ಲದಿದ್ದರೆ
          q->ಸ್ಥಿತಿ_2 = ON_HOLD;
      }
  ಇಲ್ಲದಿದ್ದರೆ
    p->ಸ್ಥಿತಿ_2 = ON_HOLD;

  p->ಸಾಲುಗಳು_ಗಮ್ಯ_print = 0;
  --files_ready_ಗಮ್ಯ_read;
}

/* Undo hold_file -- go through the column list and change any
   ON_HOLD columns to OPEN.  Used at the end of each page. */

ಸ್ಥಿರ ಶೂನ್ಯ
reset_ಸ್ಥಿತಿ (ಶೂನ್ಯ)
{
  ಪೂರ್ಣಾಂಕ i = columns;
  ಸ್ತಂಭ_3 *p;

  ಗಾಗಿ (p = ಸ್ತಂಭ_ಸದಿಶ; i; --i, ++p)
    ಆದರೆ (p->ಸ್ಥಿತಿ_2 == ON_HOLD)
      {
        p->ಸ್ಥಿತಿ_2 = OPEN;
        files_ready_ಗಮ್ಯ_read++;
      }

  ಆದರೆ (storing_columns)
    {
      ಆದರೆ (ಸ್ತಂಭ_ಸದಿಶ->ಸ್ಥಿತಿ_2 == CLOSED)
        /* We use the info to output an error message in  skip_to_page. */
        files_ready_ಗಮ್ಯ_read = 0;
      ಇಲ್ಲದಿದ್ದರೆ
        files_ready_ಗಮ್ಯ_read = 1;
    }
}

/* Print a single file, or multiple files in parallel.

   Set up the list of columns, opening the necessary files.
   Allocate space for storing columns, if necessary.
   Skip to first_page_number, if user has asked to skip leading pages.
   Determine which functions are appropriate to store/print lines
   in each column.
   Print the file(s). */

ಸ್ಥಿರ ಶೂನ್ಯ
print_files (ಪೂರ್ಣಾಂಕ ಸಂಖ್ಯೆ_ದ_files, ಅಕ್ಷರ **av)
{
  init_parameters (ಸಂಖ್ಯೆ_ದ_files);
  ಆದರೆ (! init_fps (ಸಂಖ್ಯೆ_ದ_files, av))
    ಹಿಂತಿರುಗಿ;
  ಆದರೆ (storing_columns)
    init_store_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ ();

  ಆದರೆ (ಮೊದಲ_ಪುಟ_ಸಂಖ್ಯೆ > 1)
    {
      ಆದರೆ (!skip_ಗಮ್ಯ_ಪುಟ (ಮೊದಲ_ಪುಟ_ಸಂಖ್ಯೆ))
        ಹಿಂತಿರುಗಿ;
      ಇಲ್ಲದಿದ್ದರೆ
        ಪುಟ_ಸಂಖ್ಯೆ = ಮೊದಲ_ಪುಟ_ಸಂಖ್ಯೆ;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    ಪುಟ_ಸಂಖ್ಯೆ = 1;

  init_funcs ();

  ಸಾಲು_ಸಂಖ್ಯೆ = ಸಾಲು_ಎಣಿಕೆ;
  ಆಗ (print_ಪುಟ ())
    ;
}

/* Initialize header information.
   If DESC is non-negative, it is a file descriptor open to
   FILENAME for reading.  */

ಸ್ಥಿರ ಶೂನ್ಯ
init_ಶೀರ್ಷಿಕೆ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename, ಪೂರ್ಣಾಂಕ desc)
{
  ಅಕ್ಷರ *buf = ಖಾಲಿ;
  ರಚನೆ stat st;
  ರಚನೆ timespec t;
  ಪೂರ್ಣಾಂಕ ns;
  ರಚನೆ tm tm;

  /* If parallel files or standard input, use current date. */
  ಆದರೆ (streq (filename, "-"))
    desc = -1;
  ಆದರೆ (0 <= desc && fstat (desc, &st) == 0)
    t = get_stat_mtime (&st);
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಸ್ಥಿರ ರಚನೆ timespec timespec;
      ಆದರೆ (! timespec.tv_sec)
        gettime (&timespec);
      t = timespec;
    }

  ns = t.tv_nsec;
  ಆದರೆ (localtime_rz (localtz, &t.tv_sec, &tm))
    {
      ptrdiff_t len = nstrftime (ಖಾಲಿ, MIN (PTRDIFF_MAX, SIZE_MAX),
                                 date_format, &tm, localtz, ns);
      ಆದರೆ (0 <= len)
        {
          buf = ximalloc (len + 1);
          nstrftime (buf, len + 1, date_format, &tm, localtz, ns);
        }
    }
  ಆದರೆ (!buf)
    {
      ಅಕ್ಷರ secbuf[INT_BUFSIZE_BOUND (intmax_t)];
      buf = xmalloc (ಗಾತ್ರ secbuf + MAX (10, INT_BUFSIZE_BOUND (ಪೂರ್ಣಾಂಕ)));
      sprintf (buf, "%s.%09d", timetostr (t.tv_sec, secbuf), ns);
    }

  ಬಿಡುಗಡೆ (date_ಪಠ್ಯ);
  date_ಪಠ್ಯ = buf;
  file_ಪಠ್ಯ = custom_ಶೀರ್ಷಿಕೆ ? custom_ಶೀರ್ಷಿಕೆ : desc < 0 ? "" : filename;
  ಶೀರ್ಷಿಕೆ_ಅಗಲ_ಲಭ್ಯ = (chars_ಪ್ರತಿ_ಸಾಲು
                            - mbswidth (date_ಪಠ್ಯ, 0)
                            - mbswidth (file_ಪಠ್ಯ, 0));
}

/* Set things up for printing a page

   Scan through the columns ...
   Determine which are ready to print
   (i.e., which have lines stored or open files)
   Set p->lines_to_print appropriately
   (to p->lines_stored if we're storing, or lines_per_body
   if we're reading straight from the file)
   Keep track of this total so we know when to stop printing */

ಸ್ಥಿರ ಶೂನ್ಯ
init_ಪುಟ (ಶೂನ್ಯ)
{
  ಪೂರ್ಣಾಂಕ j;
  ಸ್ತಂಭ_3 *p;

  ಆದರೆ (storing_columns)
    {
      store_columns ();
      ಗಾಗಿ (j = columns - 1, p = ಸ್ತಂಭ_ಸದಿಶ; j; --j, ++p)
        {
          p->ಸಾಲುಗಳು_ಗಮ್ಯ_print = p->ಸಾಲುಗಳು_stored;
        }

      /* Last column. */
      ಆದರೆ (balance_columns)
        {
          p->ಸಾಲುಗಳು_ಗಮ್ಯ_print = p->ಸಾಲುಗಳು_stored;
        }
      /* Since we're not balancing columns, we don't need to store
         the rightmost column.   Read it straight from the file. */
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಆದರೆ (p->ಸ್ಥಿತಿ_2 == OPEN)
            {
              p->ಸಾಲುಗಳು_ಗಮ್ಯ_print = ಸಾಲುಗಳು_ಪ್ರತಿ_body;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            p->ಸಾಲುಗಳು_ಗಮ್ಯ_print = 0;
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ
    ಗಾಗಿ (j = columns, p = ಸ್ತಂಭ_ಸದಿಶ; j; --j, ++p)
      ಆದರೆ (p->ಸ್ಥಿತಿ_2 == OPEN)
        {
          p->ಸಾಲುಗಳು_ಗಮ್ಯ_print = ಸಾಲುಗಳು_ಪ್ರತಿ_body;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        p->ಸಾಲುಗಳು_ಗಮ್ಯ_print = 0;
}

/* Align empty columns and print separators.
   Empty columns will be formed by files with status ON_HOLD or CLOSED
   when printing multiple files in parallel. */

ಸ್ಥಿರ ಶೂನ್ಯ
align_ಸ್ತಂಭ (ಸ್ತಂಭ_3 *p)
{
  padding_not_printed = p->ಆರಂಭ_position;
  ಆದರೆ (ಲಂಬಸಾಲು_sep_ಉದ್ದಳತೆ < padding_not_printed)
    {
      pad_across_ಗಮ್ಯ (padding_not_printed - ಲಂಬಸಾಲು_sep_ಉದ್ದಳತೆ);
      padding_not_printed = ANYWHERE;
    }

  ಆದರೆ (use_ಲಂಬಸಾಲು_separator)
    print_sep_string ();

  ಆದರೆ (p->numbered)
    ಕೂಡಿಸು_ಸಾಲು_ಸಂಖ್ಯೆ (p);
}

/* Print one page.

   As long as there are lines left on the page and columns ready to print,
   Scan across the column list
   if the column has stored lines or the file is open
   pad to the appropriate spot
   print the column
   pad the remainder of the page with \n or \f as requested
   reset the status of all files -- any files which where on hold because
   of formfeeds are now put back into the lineup. */

ಸ್ಥಿರ bool
print_ಪುಟ (ಶೂನ್ಯ)
{
  ಪೂರ್ಣಾಂಕ j;
  ಪೂರ್ಣಾಂಕ ಸಾಲುಗಳು_ಎಡ_on_ಪುಟ;
  ಸ್ತಂಭ_3 *p;

  /* Used as an accumulator (with | operator) of successive values of
     pad_vertically.  The trick is to set pad_vertically
     to false before each run through the inner loop, then after that
     loop, it tells us whether a line was actually printed (whether a
     newline needs to be output -- or two for double spacing).  But those
     values have to be accumulated (in pv) so we can invoke pad_down
     properly after the outer loop completes. */
  bool pv;

  init_ಪುಟ ();

  ಆದರೆ (ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_ready_ಗಮ್ಯ_print () == 0)
    ಹಿಂತಿರುಗಿ false;

  ಆದರೆ (extremities)
    print_a_ಶೀರ್ಷಿಕೆ = true;

  /* Don't pad unless we know a page was printed. */
  pad_vertically = false;
  pv = false;

  ಸಾಲುಗಳು_ಎಡ_on_ಪುಟ = ಸಾಲುಗಳು_ಪ್ರತಿ_body;
  ಆದರೆ (double_space)
    ಸಾಲುಗಳು_ಎಡ_on_ಪುಟ *= 2;

  ಆಗ (ಸಾಲುಗಳು_ಎಡ_on_ಪುಟ > 0 && ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_ready_ಗಮ್ಯ_print () > 0)
    {
      output_position = 0;
      spaces_not_printed = 0;
      separators_not_printed = 0;
      pad_vertically = false;
      align_ಖಾಲಿಗಳು_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = false;
      ಖಾಲಿಗಳು_ಸಾಲು = true;

      ಗಾಗಿ (j = 1, p = ಸ್ತಂಭ_ಸದಿಶ; j <= columns; ++j, ++p)
        {
          ಒಳಹರಿವು_position = 0;
          ಆದರೆ (p->ಸಾಲುಗಳು_ಗಮ್ಯ_print > 0 || p->ಸ್ಥಿತಿ_2 == FF_ಸಿಕ್ಕಿತು)
            {
              FF_only = false;
              padding_not_printed = p->ಆರಂಭ_position;
              ಆದರೆ (!(p->print_func) (p))
                read_rest_ದ_ಸಾಲು (p);
              pv |= pad_vertically;

              --p->ಸಾಲುಗಳು_ಗಮ್ಯ_print;
              ಆದರೆ (p->ಸಾಲುಗಳು_ಗಮ್ಯ_print <= 0)
                {
                  ಆದರೆ (ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_ready_ಗಮ್ಯ_print () == 0)
                    ನಿಲ್ಲಿಸು;
                }

              /* File p changed its status to ON_HOLD or CLOSED */
              ಆದರೆ (parallel_files && p->ಸ್ಥಿತಿ_2 != OPEN)
                {
                  ಆದರೆ (ಖಾಲಿಗಳು_ಸಾಲು)
                    align_ಖಾಲಿಗಳು_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = true;
                  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (p->ಸ್ಥಿತಿ_2 == CLOSED
                           || (p->ಸ್ಥಿತಿ_2 == ON_HOLD && FF_only))
                    align_ಸ್ತಂಭ (p);
                }
            }
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (parallel_files)
            {
              /* File status ON_HOLD or CLOSED */
              ಆದರೆ (ಖಾಲಿಗಳು_ಸಾಲು)
                align_ಖಾಲಿಗಳು_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = true;
              ಇಲ್ಲದಿದ್ದರೆ
                align_ಸ್ತಂಭ (p);
            }

          /* We need it also with an empty column */
          ಆದರೆ (use_ಲಂಬಸಾಲು_separator)
            ++separators_not_printed;
        }

      ಆದರೆ (pad_vertically)
        {
          ಅಕ್ಷರ_ಬರೆ ('\n');
          --ಸಾಲುಗಳು_ಎಡ_on_ಪುಟ;
        }

      ಆದರೆ (ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_ready_ಗಮ್ಯ_print () == 0 && !extremities)
        ನಿಲ್ಲಿಸು;

      ಆದರೆ (double_space && pv)
        {
          ಅಕ್ಷರ_ಬರೆ ('\n');
          --ಸಾಲುಗಳು_ಎಡ_on_ಪುಟ;
        }
    }

  ಆದರೆ (ಸಾಲುಗಳು_ಎಡ_on_ಪುಟ == 0)
    ಗಾಗಿ (j = 1, p = ಸ್ತಂಭ_ಸದಿಶ; j <= columns; ++j, ++p)
      ಆದರೆ (p->ಸ್ಥಿತಿ_2 == OPEN)
        p->ತುಂಬಿದ_ಪುಟ_printed = true;

  pad_vertically = pv;

  ಆದರೆ (pad_vertically && extremities)
    pad_ಕೆಳಕ್ಕೆ (ಸಾಲುಗಳು_ಎಡ_on_ಪುಟ + ಸಾಲುಗಳು_ಪ್ರತಿ_footer);
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (keep_FF && print_a_FF)
    {
      ಅಕ್ಷರ_ಬರೆ ('\f');
      print_a_FF = false;
    }

  ಆದರೆ (ferror (stdout))
    write_error ();

  ಆದರೆ (ಕೊನೆಯ_ಪುಟ_ಸಂಖ್ಯೆ < ++ಪುಟ_ಸಂಖ್ಯೆ)
    ಹಿಂತಿರುಗಿ false;		/* Stop printing with LAST_PAGE */

  reset_ಸ್ಥಿತಿ ();		/* Change ON_HOLD to OPEN. */

  ಹಿಂತಿರುಗಿ true;			/* More pages to go. */
}

/* Allocate space for storing columns.

   This is necessary when printing multiple columns from a single file.
   Lines are stored consecutively in buff, separated by '\0'.

   The following doesn't apply any longer - any tuning possible?
   (We can't use a fixed offset since with the '-s' flag lines aren't
   truncated.)

   We maintain a list (line_vector) of pointers to the beginnings
   of lines in buff.  We allocate one more than the number of lines
   because the last entry tells us the index of the last character,
   which we need to know in order to print the last line in buff. */

ಸ್ಥಿರ ಶೂನ್ಯ
init_store_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ (ಶೂನ್ಯ)
{
  /* Tune this.  */
  ಪೂರ್ಣಾಂಕ ಒಟ್ಟು_ಸಾಲುಗಳು, ಒಟ್ಟು_ಸಾಲುಗಳು_1, chars_ಪ್ರತಿ_ಸ್ತಂಭ_1, chars_if_truncate;
  ಆದರೆ (ckd_mul (&ಒಟ್ಟು_ಸಾಲುಗಳು, ಸಾಲುಗಳು_ಪ್ರತಿ_body, columns)
      || ckd_ಕೂಡಿಸು (&ಒಟ್ಟು_ಸಾಲುಗಳು_1, ಒಟ್ಟು_ಸಾಲುಗಳು, 1)
      || ckd_ಕೂಡಿಸು (&chars_ಪ್ರತಿ_ಸ್ತಂಭ_1, chars_ಪ್ರತಿ_ಸ್ತಂಭ, 1)
      || ckd_mul (&chars_if_truncate, ಒಟ್ಟು_ಸಾಲುಗಳು, chars_ಪ್ರತಿ_ಸ್ತಂಭ_1)
      || ckd_mul (&buff_allocated, chars_if_truncate, use_ಲಂಬಸಾಲು_separator + 1))
    integer_ಅತಿಪ್ರವಾಹ ();

  ಬಿಡುಗಡೆ (ಸಾಲು_ಸದಿಶ);
  /* FIXME: here's where it was allocated.  */
  ಸಾಲು_ಸದಿಶ = xnmalloc (ಒಟ್ಟು_ಸಾಲುಗಳು_1, ಗಾತ್ರ *ಸಾಲು_ಸದಿಶ);

  ಬಿಡುಗಡೆ (ಕೊನೆ_ಸದಿಶ);
  ಕೊನೆ_ಸದಿಶ = xnmalloc (ಒಟ್ಟು_ಸಾಲುಗಳು, ಗಾತ್ರ *ಕೊನೆ_ಸದಿಶ);

  ಬಿಡುಗಡೆ (buff);
  buff = ximalloc (buff_allocated);
}

/* Store all but the rightmost column.
   (Used when printing a single file in multiple downward columns)

   For each column
   set p->current_line to be the index in line_vector of the
   first line in the column
   For each line in the column
   store the line in buff
   add to line_vector the index of the line's first char
   buff_start is the index in buff of the first character in the
   current line. */

ಸ್ಥಿರ ಶೂನ್ಯ
store_columns (ಶೂನ್ಯ)
{
  ಪೂರ್ಣಾಂಕ i, j;
  ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಸಾಲು = 0;
  ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ buff_ಆರಂಭ;
  ಪೂರ್ಣಾಂಕ ಕೊನೆಯ_ಲಂಬಸಾಲು;		/* The rightmost column which will be saved in buff */
  ಸ್ತಂಭ_3 *p;

  buff_current = 0;
  buff_ಆರಂಭ = 0;

  ಆದರೆ (balance_columns)
    ಕೊನೆಯ_ಲಂಬಸಾಲು = columns;
  ಇಲ್ಲದಿದ್ದರೆ
    ಕೊನೆಯ_ಲಂಬಸಾಲು = columns - 1;

  ಗಾಗಿ (i = 1, p = ಸ್ತಂಭ_ಸದಿಶ; i <= ಕೊನೆಯ_ಲಂಬಸಾಲು; ++i, ++p)
    p->ಸಾಲುಗಳು_stored = 0;

  ಗಾಗಿ (i = 1, p = ಸ್ತಂಭ_ಸದಿಶ; i <= ಕೊನೆಯ_ಲಂಬಸಾಲು && files_ready_ಗಮ್ಯ_read;
       ++i, ++p)
    {
      p->current_ಸಾಲು = ಸಾಲು;
      ಗಾಗಿ (j = ಸಾಲುಗಳು_ಪ್ರತಿ_body; j && files_ready_ಗಮ್ಯ_read; --j)

        ಆದರೆ (p->ಸ್ಥಿತಿ_2 == OPEN)	/* Redundant.  Clean up. */
          {
            ಒಳಹರಿವು_position = 0;

            ಆದರೆ (!read_ಸಾಲು (p))
              read_rest_ದ_ಸಾಲು (p);

            ಆದರೆ (p->ಸ್ಥಿತಿ_2 == OPEN
                || buff_ಆರಂಭ != buff_current)
              {
                ++p->ಸಾಲುಗಳು_stored;
                ಸಾಲು_ಸದಿಶ[ಸಾಲು] = buff_ಆರಂಭ;
                ಕೊನೆ_ಸದಿಶ[ಸಾಲು++] = ಒಳಹರಿವು_position;
                buff_ಆರಂಭ = buff_current;
              }
          }
    }

  /* Keep track of the location of the last char in buff. */
  ಸಾಲು_ಸದಿಶ[ಸಾಲು] = buff_ಆರಂಭ;

  ಆದರೆ (balance_columns)
    balance (ಸಾಲು);
}

ಸ್ಥಿರ ಶೂನ್ಯ
balance (ಪೂರ್ಣಾಂಕ ಒಟ್ಟು_stored)
{
  ಸ್ತಂಭ_3 *p;
  ಪೂರ್ಣಾಂಕ i, ಸಾಲುಗಳು;
  ಪೂರ್ಣಾಂಕ ಮೊದಲ_ಸಾಲು = 0;

  ಗಾಗಿ (i = 1, p = ಸ್ತಂಭ_ಸದಿಶ; i <= columns; ++i, ++p)
    {
      ಸಾಲುಗಳು = ಒಟ್ಟು_stored / columns;
      ಆದರೆ (i <= ಒಟ್ಟು_stored % columns)
        ++ಸಾಲುಗಳು;

      p->ಸಾಲುಗಳು_stored = ಸಾಲುಗಳು;
      p->current_ಸಾಲು = ಮೊದಲ_ಸಾಲು;

      ಮೊದಲ_ಸಾಲು += ಸಾಲುಗಳು;
    }
}

/* Store a character in the buffer. */

ಸ್ಥಿರ ಶೂನ್ಯ
store_char (ಅಕ್ಷರ c)
{
  ಆದರೆ (buff_current >= buff_allocated)
    {
      /* May be too generous. */
      buff = xpalloc (buff, &buff_allocated, 1, -1, ಗಾತ್ರ *buff);
    }
  buff[buff_current++] = c;
}

ಸ್ಥಿರ ಶೂನ್ಯ
ಕೂಡಿಸು_ಸಾಲು_ಸಂಖ್ಯೆ (ಸ್ತಂಭ_3 *p)
{
  ಪೂರ್ಣಾಂಕ i;
  ಅಕ್ಷರ *s;
  ಪೂರ್ಣಾಂಕ ಅಂಕೆ_ಅಗಲ;

  /* Cutting off the higher-order digits is more informative than
     lower-order cut off. */
  ಅಂಕೆ_ಅಗಲ = sprintf (ಸಂಖ್ಯೆ_buff, "%*d", chars_ಪ್ರತಿ_ಸಂಖ್ಯೆ, ಸಾಲು_ಸಂಖ್ಯೆ);
  ಸಾಲು_ಸಂಖ್ಯೆ++;
  s = ಸಂಖ್ಯೆ_buff + (ಅಂಕೆ_ಅಗಲ - chars_ಪ್ರತಿ_ಸಂಖ್ಯೆ);
  ಗಾಗಿ (i = chars_ಪ್ರತಿ_ಸಂಖ್ಯೆ; i > 0; i--)
    (p->char_func) (*s++);

  ಆದರೆ (columns > 1)
    {
      /* Tabification is assumed for multiple columns, also for n-separators,
         but 'default n-separator = TAB' hasn't been given priority over
         equal column_width also specified by POSIX. */
      ಆದರೆ (ಸಂಖ್ಯೆ_separator == '\t')
        {
          i = ಸಂಖ್ಯೆ_ಅಗಲ - chars_ಪ್ರತಿ_ಸಂಖ್ಯೆ;
          ಆಗ (i-- > 0)
            (p->char_func) (' ');
        }
      ಇಲ್ಲದಿದ್ದರೆ
        (p->char_func) (ಸಂಖ್ಯೆ_separator);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    /* To comply with POSIX, we avoid any expansion of default TAB
       separator with a single column output. No column_width requirement
       has to be considered. */
    {
      (p->char_func) (ಸಂಖ್ಯೆ_separator);
      ಆದರೆ (ಸಂಖ್ಯೆ_separator == '\t')
        output_position = POS_AFTER_TAB (chars_ಪ್ರತಿ_output_tab,
                          output_position);
    }

  ಆದರೆ (truncate_ಸಾಲುಗಳು && !parallel_files)
    ಒಳಹರಿವು_position += ಸಂಖ್ಯೆ_ಅಗಲ;
}

/* Print (or store) padding until the current horizontal position
   is position. */

ಸ್ಥಿರ ಶೂನ್ಯ
pad_across_ಗಮ್ಯ (ಪೂರ್ಣಾಂಕ position)
{
  ಪೂರ್ಣಾಂಕ h = output_position;

  ಆದರೆ (tabify_output)
    spaces_not_printed = position - output_position;
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಆಗ (++h <= position)
        ಅಕ್ಷರ_ಬರೆ (' ');
      output_position = position;
    }
}

/* Pad to the bottom of the page.

   If the user has requested a formfeed, use one.
   Otherwise, use newlines. */

ಸ್ಥಿರ ಶೂನ್ಯ
pad_ಕೆಳಕ್ಕೆ (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಸಾಲುಗಳು)
{
  ಆದರೆ (use_form_feed)
    ಅಕ್ಷರ_ಬರೆ ('\f');
  ಇಲ್ಲದಿದ್ದರೆ
    ಗಾಗಿ (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ i = ಸಾಲುಗಳು; i; --i)
      ಅಕ್ಷರ_ಬರೆ ('\n');
}

/* Read the rest of the line.

   Read from the current column's file until an end of line is
   hit.  Used when we've truncated a line and we no longer need
   to print or store its characters. */

ಸ್ಥಿರ ಶೂನ್ಯ
read_rest_ದ_ಸಾಲು (ಸ್ತಂಭ_3 *p)
{
  ಪೂರ್ಣಾಂಕ c;
  FILE *f = p->fp;

  ಆಗ ((c = getc (f)) != '\n')
    {
      ಆದರೆ (c == '\f')
        {
          ಆದರೆ ((c = getc (f)) != '\n')
            ungetc (c, f);
          ಆದರೆ (keep_FF)
            print_a_FF = true;
          hold_file (p);
          ನಿಲ್ಲಿಸು;
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (c == EOF)
        {
          close_file (p);
          ನಿಲ್ಲಿಸು;
        }
    }
}

/* Read a line with skip_to_page.

   Read from the current column's file until an end of line is
   hit.  Used when we read full lines to skip pages.
   With skip_to_page we have to check for FF-coincidence which is done
   in function read_line otherwise.
   Count lines of skipped pages to find the line number of 1st page
   printed relative to 1st line of input file (start_line_num). */

ಸ್ಥಿರ ಶೂನ್ಯ
skip_read (ಸ್ತಂಭ_3 *p, ಪೂರ್ಣಾಂಕ ಸ್ತಂಭ_ಸಂಖ್ಯೆ)
{
  ಪೂರ್ಣಾಂಕ c;
  FILE *f = p->fp;
  ಪೂರ್ಣಾಂಕ i;
  bool single_ff = false;
  ಸ್ತಂಭ_3 *q;

  /* Read 1st character in a line or any character succeeding a FF */
  ಆದರೆ ((c = getc (f)) == '\f' && p->ತುಂಬಿದ_ಪುಟ_printed)
    /* A FF-coincidence with a previous full_page_printed.
       To avoid an additional empty page, eliminate the FF */
    ಆದರೆ ((c = getc (f)) == '\n')
      c = getc (f);

  p->ತುಂಬಿದ_ಪುಟ_printed = false;

  /* 1st character a FF means a single FF without any printable
     characters. Don't count it as a line with -n option. */
  ಆದರೆ (c == '\f')
    single_ff = true;

  /* Preparing for a FF-coincidence: Maybe we finish that page
     without a FF found */
  ಆದರೆ (ಕೊನೆಯ_ಸಾಲು)
    p->ತುಂಬಿದ_ಪುಟ_printed = true;

  ಆಗ (c != '\n')
    {
      ಆದರೆ (c == '\f')
        {
          /* No FF-coincidence possible,
             no catching up of a FF-coincidence with next page */
          ಆದರೆ (ಕೊನೆಯ_ಸಾಲು)
            {
              ಆದರೆ (!parallel_files)
                ಗಾಗಿ (q = ಸ್ತಂಭ_ಸದಿಶ, i = columns; i; ++q, --i)
                  q->ತುಂಬಿದ_ಪುಟ_printed = false;
              ಇಲ್ಲದಿದ್ದರೆ
                p->ತುಂಬಿದ_ಪುಟ_printed = false;
            }

          ಆದರೆ ((c = getc (f)) != '\n')
            ungetc (c, f);
          hold_file (p);
          ನಿಲ್ಲಿಸು;
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (c == EOF)
        {
          close_file (p);
          ನಿಲ್ಲಿಸು;
        }
      c = getc (f);
    }

  ಆದರೆ (skip_ಎಣಿಕೆ)
    ಆದರೆ ((!parallel_files || ಸ್ತಂಭ_ಸಂಖ್ಯೆ == 1) && !single_ff)
      ++ಸಾಲು_ಎಣಿಕೆ;
}

/* If we're tabifying output,

   When print_char encounters white space it keeps track
   of our desired horizontal position and delays printing
   until this function is called. */

ಸ್ಥಿರ ಶೂನ್ಯ
print_white_space (ಶೂನ್ಯ)
{
  ಪೂರ್ಣಾಂಕ h_new;
  ಪೂರ್ಣಾಂಕ h_old = output_position;
  ಪೂರ್ಣಾಂಕ goal = h_old + spaces_not_printed;

  ಆಗ (goal - h_old > 1
         && (h_new = POS_AFTER_TAB (chars_ಪ್ರತಿ_output_tab, h_old)) <= goal)
    {
      ಅಕ್ಷರ_ಬರೆ (output_tab_char);
      h_old = h_new;
    }
  ಆಗ (++h_old <= goal)
    ಅಕ್ಷರ_ಬರೆ (' ');

  output_position = goal;
  spaces_not_printed = 0;
}

/* Print column separators.

   We keep a count until we know that we'll be printing a line,
   then print_sep_string() is called. */

ಸ್ಥಿರ ಶೂನ್ಯ
print_sep_string (ಶೂನ್ಯ)
{
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *s = ಲಂಬಸಾಲು_sep_string;
  ಪೂರ್ಣಾಂಕ l = ಲಂಬಸಾಲು_sep_ಉದ್ದಳತೆ;

  ಆದರೆ (separators_not_printed <= 0)
    {
      /* We'll be starting a line with chars_per_margin, anything else? */
      ಆದರೆ (spaces_not_printed > 0)
        print_white_space ();
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಗಾಗಿ (; separators_not_printed > 0; --separators_not_printed)
        {
          ಆಗ (l-- > 0)
            {
              /* 3 types of sep_strings: spaces only, spaces and chars,
              chars only */
              ಆದರೆ (*s == ' ')
                {
                  /* We're tabifying output; consecutive spaces in
                  sep_string may have to be converted to tabs */
                  s++;
                  ++spaces_not_printed;
                }
              ಇಲ್ಲದಿದ್ದರೆ
                {
                  ಆದರೆ (spaces_not_printed > 0)
                    print_white_space ();
                  ಅಕ್ಷರ_ಬರೆ (*s++);
                  ++output_position;
                }
            }
          /* sep_string ends with some spaces */
          ಆದರೆ (spaces_not_printed > 0)
            print_white_space ();
        }
    }
}

/* Print (or store, depending on p->char_func) a clump of N
   characters. */

ಸ್ಥಿರ ಶೂನ್ಯ
print_clump (ಸ್ತಂಭ_3 *p, ಪೂರ್ಣಾಂಕ n, ಅಕ್ಷರ *clump)
{
  ಆಗ (n--)
    (p->char_func) (*clump++);

  ಆದರೆ (ferror (stdout))
    write_error ();
}

/* Print a character.

   Update the following comment: process-char hasn't been used any
   longer.
   If we're tabifying, all tabs have been converted to spaces by
   process_char().  Keep a count of consecutive spaces, and when
   a nonspace is encountered, call print_white_space() to print the
   required number of tabs and spaces. */

ಸ್ಥಿರ ಶೂನ್ಯ
print_char (ಅಕ್ಷರ c)
{
  ಆದರೆ (tabify_output)
    {
      ಆದರೆ (c == ' ')
        {
          ++spaces_not_printed;
          ಹಿಂತಿರುಗಿ;
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (spaces_not_printed > 0)
        print_white_space ();

      /* Nonprintables are assumed to have width 0, except '\b'. */
      ಆದರೆ (! isprint (ಗಮ್ಯ_uchar (c)))
        {
          ಆದರೆ (c == '\b')
            --output_position;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        ++output_position;
    }
  ಅಕ್ಷರ_ಬರೆ (c);
}

/* Skip to page PAGE before printing.
   PAGE may be larger than total number of pages. */

ಸ್ಥಿರ bool
skip_ಗಮ್ಯ_ಪುಟ (uintmax_t ಪುಟ)
{
  ಗಾಗಿ (uintmax_t n = 1; n < ಪುಟ; ++n)
    {
      ಸ್ತಂಭ_3 *p;
      ಪೂರ್ಣಾಂಕ j;

      ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 1; i < ಸಾಲುಗಳು_ಪ್ರತಿ_body; ++i)
        {
          ಗಾಗಿ (j = 1, p = ಸ್ತಂಭ_ಸದಿಶ; j <= columns; ++j, ++p)
            ಆದರೆ (p->ಸ್ಥಿತಿ_2 == OPEN)
              skip_read (p, j);
        }
      ಕೊನೆಯ_ಸಾಲು = true;
      ಗಾಗಿ (j = 1, p = ಸ್ತಂಭ_ಸದಿಶ; j <= columns; ++j, ++p)
        ಆದರೆ (p->ಸ್ಥಿತಿ_2 == OPEN)
          skip_read (p, j);

      ಆದರೆ (storing_columns)	/* change FF_FOUND to ON_HOLD */
        ಗಾಗಿ (j = 1, p = ಸ್ತಂಭ_ಸದಿಶ; j <= columns; ++j, ++p)
          ಆದರೆ (p->ಸ್ಥಿತಿ_2 != CLOSED)
            p->ಸ್ಥಿತಿ_2 = ON_HOLD;

      reset_ಸ್ಥಿತಿ ();
      ಕೊನೆಯ_ಸಾಲು = false;

      ಆದರೆ (files_ready_ಗಮ್ಯ_read < 1)
        {
          /* It's very helpful, normally the total number of pages is
             not known in advance.  */
          error (0, 0,
                 _("starting page number %ju exceeds page count %ju"),
                 ಪುಟ, n);
          ನಿಲ್ಲಿಸು;
        }
    }
  ಹಿಂತಿರುಗಿ files_ready_ಗಮ್ಯ_read > 0;
}

/* Print a header.

   Formfeeds are assumed to use up two lines at the beginning of
   the page. */

ಸ್ಥಿರ ಶೂನ್ಯ
print_ಶೀರ್ಷಿಕೆ (ಶೂನ್ಯ)
{
  ಅಕ್ಷರ ಪುಟ_ಪಠ್ಯ[256 + INT_STRLEN_BOUND (ಪುಟ_ಸಂಖ್ಯೆ)];
  ಪೂರ್ಣಾಂಕ ಲಭ್ಯ_ಅಗಲ;
  ಪೂರ್ಣಾಂಕ lhs_spaces;
  ಪೂರ್ಣಾಂಕ rhs_spaces;

  output_position = 0;
  pad_across_ಗಮ್ಯ (chars_ಪ್ರತಿ_margin);
  print_white_space ();

  ಆದರೆ (ಪುಟ_ಸಂಖ್ಯೆ == 0)
    error (EXIT_FAILURE, 0, _("page number overflow"));

  /* The translator must ensure that formatting the translation of
     "Page %ju" does not generate more than (sizeof page_text - 1)
     bytes.  */
  sprintf (ಪುಟ_ಪಠ್ಯ, _("Page %ju"), ಪುಟ_ಸಂಖ್ಯೆ);
  ಲಭ್ಯ_ಅಗಲ = ಶೀರ್ಷಿಕೆ_ಅಗಲ_ಲಭ್ಯ - mbswidth (ಪುಟ_ಪಠ್ಯ, 0);
  ಲಭ್ಯ_ಅಗಲ = MAX (0, ಲಭ್ಯ_ಅಗಲ);
  lhs_spaces = ಲಭ್ಯ_ಅಗಲ >> 1;
  rhs_spaces = ಲಭ್ಯ_ಅಗಲ - lhs_spaces;

  ಬರೆ ("\n\n%*s%s%*s%s%*s%s\n\n\n",
          chars_ಪ್ರತಿ_margin, "",
          date_ಪಠ್ಯ, lhs_spaces, " ",
          file_ಪಠ್ಯ, rhs_spaces, " ", ಪುಟ_ಪಠ್ಯ);

  print_a_ಶೀರ್ಷಿಕೆ = false;
  output_position = 0;
}

/* Print (or store, if p->char_func is store_char()) a line.

   Read a character to determine whether we have a line or not.
   (We may hit EOF, \n, or \f)

   Once we know we have a line,
   set pad_vertically = true, meaning it's safe
   to pad down at the end of the page, since we do have a page.
   print a header if needed.
   pad across to padding_not_printed if needed.
   print any separators which need to be printed.
   print a line number if it needs to be printed.

   Print the clump which corresponds to the first character.

   Enter a loop and keep printing until an end of line condition
   exists, or until we exceed chars_per_column.

   Return false if we exceed chars_per_column before reading
   an end of line character, true otherwise. */

ಸ್ಥಿರ bool
read_ಸಾಲು (ಸ್ತಂಭ_3 *p)
{
  ಪೂರ್ಣಾಂಕ c;
  ಪೂರ್ಣಾಂಕ chars IF_LINT (= 0);
  ಪೂರ್ಣಾಂಕ ಕೊನೆಯ_ಒಳಹರಿವು_position;
  ಪೂರ್ಣಾಂಕ j, k;
  ಸ್ತಂಭ_3 *q;

  /* read 1st character in each line or any character succeeding a FF: */
  c = getc (p->fp);

  ಕೊನೆಯ_ಒಳಹರಿವು_position = ಒಳಹರಿವು_position;

  ಆದರೆ (c == '\f' && p->ತುಂಬಿದ_ಪುಟ_printed)
    ಆದರೆ ((c = getc (p->fp)) == '\n')
      c = getc (p->fp);
  p->ತುಂಬಿದ_ಪುಟ_printed = false;

  ಆಯ್ಕೆ (c)
    {
    ಸ್ಥಿತಿ '\f':
      ಆದರೆ ((c = getc (p->fp)) != '\n')
        ungetc (c, p->fp);
      FF_only = true;
      ಆದರೆ (print_a_ಶೀರ್ಷಿಕೆ && !storing_columns)
        {
          pad_vertically = true;
          print_ಶೀರ್ಷಿಕೆ ();
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (keep_FF)
        print_a_FF = true;
      hold_file (p);
      ಹಿಂತಿರುಗಿ true;
    ಸ್ಥಿತಿ EOF:
      close_file (p);
      ಹಿಂತಿರುಗಿ true;
    ಸ್ಥಿತಿ '\n':
      ನಿಲ್ಲಿಸು;
    ಪೂರ್ವನಿಯೋಜಿತ:
      chars = char_ಗಮ್ಯ_clump (c);
    }

  ಆದರೆ (truncate_ಸಾಲುಗಳು && ಒಳಹರಿವು_position > chars_ಪ್ರತಿ_ಸ್ತಂಭ)
    {
      ಒಳಹರಿವು_position = ಕೊನೆಯ_ಒಳಹರಿವು_position;
      ಹಿಂತಿರುಗಿ false;
    }

  ಆದರೆ (p->char_func != store_char)
    {
      pad_vertically = true;

      ಆದರೆ (print_a_ಶೀರ್ಷಿಕೆ && !storing_columns)
        print_ಶೀರ್ಷಿಕೆ ();

      ಆದರೆ (parallel_files && align_ಖಾಲಿಗಳು_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ)
        {
          /* We have to align empty columns at the beginning of a line. */
          k = separators_not_printed;
          separators_not_printed = 0;
          ಗಾಗಿ (j = 1, q = ಸ್ತಂಭ_ಸದಿಶ; j <= k; ++j, ++q)
            {
              align_ಸ್ತಂಭ (q);
              separators_not_printed += 1;
            }
          padding_not_printed = p->ಆರಂಭ_position;
          ಆದರೆ (truncate_ಸಾಲುಗಳು)
            spaces_not_printed = chars_ಪ್ರತಿ_ಸ್ತಂಭ;
          ಇಲ್ಲದಿದ್ದರೆ
            spaces_not_printed = 0;
          align_ಖಾಲಿಗಳು_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = false;
        }

      ಆದರೆ (ಲಂಬಸಾಲು_sep_ಉದ್ದಳತೆ < padding_not_printed)
        {
          pad_across_ಗಮ್ಯ (padding_not_printed - ಲಂಬಸಾಲು_sep_ಉದ್ದಳತೆ);
          padding_not_printed = ANYWHERE;
        }

      ಆದರೆ (use_ಲಂಬಸಾಲು_separator)
        print_sep_string ();
    }

  ಆದರೆ (p->numbered)
    ಕೂಡಿಸು_ಸಾಲು_ಸಂಖ್ಯೆ (p);

  ಖಾಲಿಗಳು_ಸಾಲು = false;
  ಆದರೆ (c == '\n')
    ಹಿಂತಿರುಗಿ true;

  print_clump (p, chars, clump_buff);

  ಆಗ (true)
    {
      c = getc (p->fp);

      ಆಯ್ಕೆ (c)
        {
        ಸ್ಥಿತಿ '\n':
          ಹಿಂತಿರುಗಿ true;
        ಸ್ಥಿತಿ '\f':
          ಆದರೆ ((c = getc (p->fp)) != '\n')
            ungetc (c, p->fp);
          ಆದರೆ (keep_FF)
            print_a_FF = true;
          hold_file (p);
          ಹಿಂತಿರುಗಿ true;
        ಸ್ಥಿತಿ EOF:
          close_file (p);
          ಹಿಂತಿರುಗಿ true;
        }

      ಕೊನೆಯ_ಒಳಹರಿವು_position = ಒಳಹರಿವು_position;
      chars = char_ಗಮ್ಯ_clump (c);
      ಆದರೆ (truncate_ಸಾಲುಗಳು && ಒಳಹರಿವು_position > chars_ಪ್ರತಿ_ಸ್ತಂಭ)
        {
          ಒಳಹರಿವು_position = ಕೊನೆಯ_ಒಳಹರಿವು_position;
          ಹಿಂತಿರುಗಿ false;
        }

      print_clump (p, chars, clump_buff);
    }
}

/* Print a line from buff.

   If this function has been called, we know we have "something to
   print". But it remains to be seen whether we have a real text page
   or an empty page (a single form feed) with/without a header only.
   Therefore first we set pad_vertically to true and print a header
   if necessary.
   If FF_FOUND and we are using -t|-T option we omit any newline by
   setting pad_vertically to false (see print_page).
   Otherwise we pad across if necessary, print separators if necessary
   and text of COLUMN *p.

   Return true, meaning there is no need to call read_rest_of_line. */

ಸ್ಥಿರ bool
print_stored (ಸ್ತಂಭ_3 *p)
{
  ಸ್ತಂಭ_3 *q;

  ಪೂರ್ಣಾಂಕ ಸಾಲು = p->current_ಸಾಲು++;
  ಅಕ್ಷರ *ಮೊದಲ = &buff[ಸಾಲು_ಸದಿಶ[ಸಾಲು]];
  /* FIXME
     UMR: Uninitialized memory read:
     * This is occurring while in:
     print_stored   [pr.c:2239]
     * Reading 4 bytes from 0x5148c in the heap.
     * Address 0x5148c is 4 bytes into a malloc'd block at 0x51488 of 676 bytes
     * This block was allocated from:
     malloc         [rtlib.o]
     xmalloc        [xmalloc.c:94]
     init_store_cols [pr.c:1648]
     */
  ಅಕ್ಷರ *ಕೊನೆಯ = &buff[ಸಾಲು_ಸದಿಶ[ಸಾಲು + 1]];

  pad_vertically = true;

  ಆದರೆ (print_a_ಶೀರ್ಷಿಕೆ)
    print_ಶೀರ್ಷಿಕೆ ();

  ಆದರೆ (p->ಸ್ಥಿತಿ_2 == FF_ಸಿಕ್ಕಿತು)
    {
      ಪೂರ್ಣಾಂಕ i;
      ಗಾಗಿ (i = 1, q = ಸ್ತಂಭ_ಸದಿಶ; i <= columns; ++i, ++q)
        q->ಸ್ಥಿತಿ_2 = ON_HOLD;
      ಆದರೆ (ಸ್ತಂಭ_ಸದಿಶ->ಸಾಲುಗಳು_ಗಮ್ಯ_print <= 0)
        {
          ಆದರೆ (!extremities)
            pad_vertically = false;
          ಹಿಂತಿರುಗಿ true;		/* print a header only */
        }
    }

  ಆದರೆ (ಲಂಬಸಾಲು_sep_ಉದ್ದಳತೆ < padding_not_printed)
    {
      pad_across_ಗಮ್ಯ (padding_not_printed - ಲಂಬಸಾಲು_sep_ಉದ್ದಳತೆ);
      padding_not_printed = ANYWHERE;
    }

  ಆದರೆ (use_ಲಂಬಸಾಲು_separator)
    print_sep_string ();

  ಆಗ (ಮೊದಲ != ಕೊನೆಯ)
    print_char (*ಮೊದಲ++);

  ಆದರೆ (spaces_not_printed == 0)
    {
      output_position = p->ಆರಂಭ_position + ಕೊನೆ_ಸದಿಶ[ಸಾಲು];
      ಆದರೆ (p->ಆರಂಭ_position - ಲಂಬಸಾಲು_sep_ಉದ್ದಳತೆ == chars_ಪ್ರತಿ_margin)
        output_position -= ಲಂಬಸಾಲು_sep_ಉದ್ದಳತೆ;
    }

  ಹಿಂತಿರುಗಿ true;
}

/* Convert a character to the proper format and return the number of
   characters in the resulting clump.  Increment input_position by
   the width of the clump.

   Tabs are converted to clumps of spaces.
   Nonprintable characters may be converted to clumps of escape
   sequences or control prefixes.

   Note: the width of a clump is not necessarily equal to the number of
   characters in clump_buff.  (e.g., the width of '\b' is -1, while the
   number of characters is 1.) */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
char_ಗಮ್ಯ_clump (ಅಕ್ಷರ c)
{
  ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ uc = c;
  ಅಕ್ಷರ *s = clump_buff;
  ಅಕ್ಷರ esc_buff[4];
  ಪೂರ್ಣಾಂಕ ಅಗಲ;
  ಪೂರ್ಣಾಂಕ chars;
  ಪೂರ್ಣಾಂಕ chars_ಪ್ರತಿ_c = 8;

  ಆದರೆ (c == ಒಳಹರಿವು_tab_char)
    chars_ಪ್ರತಿ_c = chars_ಪ್ರತಿ_ಒಳಹರಿವು_tab;

  ಆದರೆ (c == ಒಳಹರಿವು_tab_char || c == '\t')
    {
      ಅಗಲ = TAB_ಅಗಲ (chars_ಪ್ರತಿ_c, ಒಳಹರಿವು_position);

      ಆದರೆ (untabify_ಒಳಹರಿವು)
        {
          ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = ಅಗಲ; i; --i)
            *s++ = ' ';
          chars = ಅಗಲ;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          *s = c;
          chars = 1;
        }

    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (! isprint (uc))
    {
      ಆದರೆ (use_esc_sequence)
        {
          ಅಗಲ = 4;
          chars = 4;
          *s++ = '\\';
          sprintf (esc_buff, "%03o", uc);
          ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i <= 2; ++i)
            *s++ = esc_buff[i];
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (use_cntrl_ಪೂರ್ವಪ್ರತ್ಯಯ)
        {
          ಆದರೆ (uc < 0200)
            {
              ಅಗಲ = 2;
              chars = 2;
              *s++ = '^';
              *s = c ^ 0100;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              ಅಗಲ = 4;
              chars = 4;
              *s++ = '\\';
              sprintf (esc_buff, "%03o", uc);
              ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i <= 2; ++i)
                *s++ = esc_buff[i];
            }
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (c == '\b')
        {
          ಅಗಲ = -1;
          chars = 1;
          *s = c;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಅಗಲ = 0;
          chars = 1;
          *s = c;
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಅಗಲ = 1;
      chars = 1;
      *s = c;
    }

  /* Too many backspaces must put us in position 0 -- never negative.  */
  ಆದರೆ (ಅಗಲ < 0 && ಒಳಹರಿವು_position == 0)
    {
      chars = 0;
      ಒಳಹರಿವು_position = 0;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಅಗಲ < 0 && ಒಳಹರಿವು_position <= -ಅಗಲ)
    ಒಳಹರಿವು_position = 0;
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಆದರೆ (ckd_ಕೂಡಿಸು (&ಒಳಹರಿವು_position, ಒಳಹರಿವು_position, ಅಗಲ))
        integer_ಅತಿಪ್ರವಾಹ ();
    }

  ಹಿಂತಿರುಗಿ chars;
}

/* We've just printed some files and need to clean up things before
   looking for more options and printing the next batch of files.

   Free everything we've xmalloc'ed, except 'header'. */

ಸ್ಥಿರ ಶೂನ್ಯ
cleanup (ಶೂನ್ಯ)
{
  ಬಿಡುಗಡೆ (ಸಂಖ್ಯೆ_buff);
  ಬಿಡುಗಡೆ (clump_buff);
  ಬಿಡುಗಡೆ (ಸ್ತಂಭ_ಸದಿಶ);
  ಬಿಡುಗಡೆ (ಸಾಲು_ಸದಿಶ);
  ಬಿಡುಗಡೆ (ಕೊನೆ_ಸದಿಶ);
  ಬಿಡುಗಡೆ (buff);
}

/* Complain, print a usage message, and die. */

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              ಕಾರ್ಯಕ್ರಮ_name);

      fputs (_("\
Paginate or columnate FILE(s) ಗಾಗಿ printing.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\n\
  +ಮೊದಲ_ಪುಟ[:ಕೊನೆಯ_ಪುಟ], --ಪುಟಗಳು=ಮೊದಲ_ಪುಟ[:ಕೊನೆಯ_ಪುಟ]\n\
         begin [stop] printing with ಪುಟ ಮೊದಲ_2[ಕೊನೆಯ_2]ಪುಟ_2\n\
"));
      oputs (_("\
  -ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, --columns=ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ\n\
         output ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ columns and print columns ಕೆಳಕ್ಕೆ, unless -a is ಬಳಸಿದ.\n\
         Balance ಸಂಖ್ಯೆ ದ ಸಾಲುಗಳು ಒಳ the columns on each ಪುಟ\n\
"));
      oputs (_("\
  -a, --across\n\
         print columns across rather than ಕೆಳಕ್ಕೆ, ಬಳಸಿದ together with -ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ\n\
"));
      oputs (_("\
  -c, --show-control-chars\n\
         use hat notation (^G) and octal backslash notation\n\
"));
      oputs (_("\
  -d, --ದ್ವಿಗುಣ-space\n\
         ದ್ವಿಗುಣ space the output\n\
"));
      oputs (_("\
  -D, --date-format=FORMAT\n\
         use FORMAT ಗಾಗಿ the ಶೀರ್ಷಿಕೆ date\n\
"));
      oputs (_("\
  -e[CHAR[ಅಗಲ_2]], --expand-tabs[=CHAR[ಅಗಲ_2]]\n\
         expand ಒಳಹರಿವು CHARs (TABs) ಗಮ್ಯ tab ಅಗಲ_2 (8)\n\
"));
      oputs (_("\
  -F, -f, --form-feed\n\
         use form feeds instead ದ newlines ಗಮ್ಯ separate ಪುಟಗಳು\n\
         (by a 3-ಸಾಲು ಪುಟ ಶೀರ್ಷಿಕೆ with -F or a 5-ಸಾಲು ಶೀರ್ಷಿಕೆ\n\
         and trailer without -F)\n\
"));
      oputs (_("\
  -h, --ಶೀರ್ಷಿಕೆ=ಶೀರ್ಷಿಕೆ_3\n\
         use a centered ಶೀರ್ಷಿಕೆ_3 instead ದ filename ಒಳ ಪುಟ ಶೀರ್ಷಿಕೆ,\n\
         -h \"\" prints a blank ಸಾಲು, don't use -h\"\"\n\
"));
      oputs (_("\
  -i[CHAR[ಅಗಲ_2]], --output-tabs[=CHAR[ಅಗಲ_2]]\n\
         replace spaces with CHARs (TABs) ಗಮ್ಯ tab ಅಗಲ_2 (8)\n\
"));
      oputs (_("\
  -J, --join-ಸಾಲುಗಳು\n\
         ವಿಲೀನ ತುಂಬಿದ ಸಾಲುಗಳು, turns off -W ಸಾಲು truncation,\n\
         no ಸ್ತಂಭ alignment, --sep-string[=STRING] sets separators\n\
"));
      oputs (_("\
  -l, --ಉದ್ದಳತೆ=ಪುಟ_ಉದ್ದಳತೆ\n\
         ಗಣ_2 the ಪುಟ ಉದ್ದಳತೆ ಗಮ್ಯ ಪುಟ_ಉದ್ದಳತೆ (66) ಸಾಲುಗಳು\n\
         (ಪೂರ್ವನಿಯೋಜಿತ ಸಂಖ್ಯೆ ದ ಸಾಲುಗಳು ದ ಪಠ್ಯ 56, and with -F 63).\n\
         ಸೂಚಿಸುತ್ತದೆ -t ಆದರೆ ಪುಟ_ಉದ್ದಳತೆ <= 10\n\
"));
      oputs (_("\
  -m, --ವಿಲೀನ\n\
         print all files ಒಳ parallel, one ಒಳ each ಸ್ತಂಭ,\n\
         truncate ಸಾಲುಗಳು, but join ಸಾಲುಗಳು ದ ತುಂಬಿದ ಉದ್ದಳತೆ with -J\n\
"));
      oputs (_("\
  -n[SEP[DIGITS]], --ಸಂಖ್ಯೆ-ಸಾಲುಗಳು[=SEP[DIGITS]]\n\
         ಸಂಖ್ಯೆ ಸಾಲುಗಳು, use DIGITS (5) digits, then SEP (TAB),\n\
         ಪೂರ್ವನಿಯೋಜಿತ counting starts with 1st ಸಾಲು ದ ಒಳಹರಿವು file\n\
"));
      oputs (_("\
  -N, --ಮೊದಲ-ಸಾಲು-ಸಂಖ್ಯೆ=ಸಂಖ್ಯೆ_2\n\
         ಆರಂಭ counting with ಸಂಖ್ಯೆ_2 at 1st ಸಾಲು ದ ಮೊದಲ\n\
         ಪುಟ printed (see +ಮೊದಲ_ಪುಟ)\n\
"));
      oputs (_("\
  -o, --indent=MARGIN\n\
         ಸ್ಥಾನಾಂತರ each ಸಾಲು with MARGIN (zero) spaces, ಮಾಡು not\n\
         affect -w or -W, MARGIN will be added ಗಮ್ಯ ಪುಟ_ಅಗಲ\n\
"));
      oputs (_("\
  -r, --no-file-warnings\n\
         omit warning when a file cannot be opened\n\
"));
      oputs (_("\
  -s[CHAR], --separator[=CHAR]\n\
         separate columns by a single character, ಪೂರ್ವನಿಯೋಜಿತ ಗಾಗಿ CHAR\n\
         is the <TAB> character without -w and \'no ಅಕ್ಷರ\' with -w.\n\
         -s[CHAR] turns off ಸಾಲು truncation ದ all 3 ಸ್ತಂಭ\n\
         options (-ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ|-a -ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ|-m) except -w is ಗಣ_2\n\
"));
      oputs (_("\
  -S[STRING], --sep-string[=STRING]\n\
         separate columns by STRING,\n\
         without -S: Default separator <TAB> with -J and <space>\n\
         otherwise (same as -S\" \"), no effect on ಸ್ತಂಭ options\n\
"));
      oputs (_("\
  -t, --omit-ಶೀರ್ಷಿಕೆ\n\
         omit ಪುಟ ಶೀರ್ಷಿಕೆಗಳು and trailers; implied ಆದರೆ ಪುಟ_ಉದ್ದಳತೆ <= 10\n\
"));
      oputs (_("\
  -T, --omit-pagination\n\
         omit ಪುಟ ಶೀರ್ಷಿಕೆಗಳು and trailers,\n\
         eliminate any pagination by form feeds ಗಣ_2 ಒಳ ಒಳಹರಿವು files\n\
"));
      oputs (_("\
  -v, --show-nonprinting\n\
         use octal backslash notation\n\
"));
      oputs (_("\
  -w, --ಅಗಲ=ಪುಟ_ಅಗಲ\n\
         ಗಣ_2 ಪುಟ ಅಗಲ ಗಮ್ಯ ಪುಟ_ಅಗಲ (72) characters ಗಾಗಿ\n\
         ಅಪವರ್ತ್ಯ ಪಠ್ಯ-ಸ್ತಂಭ output only, -s[ಅಕ್ಷರ] turns off (72)\n\
"));
      oputs (_("\
  -W, --ಪುಟ-ಅಗಲ=ಪುಟ_ಅಗಲ\n\
         ಗಣ_2 ಪುಟ ಅಗಲ ಗಮ್ಯ ಪುಟ_ಅಗಲ (72) characters always,\n\
         truncate ಸಾಲುಗಳು, except -J option is ಗಣ_2,\n\
         no interference with -S or -s\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}
