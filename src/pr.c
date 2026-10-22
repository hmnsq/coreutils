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
#define PROGRAM_NAME "pr"

#define AUTHORS \
  proper_name ("Pete TerMaat"), \
  proper_name ("Roland Huebner")

/* Used with start_position in the struct COLUMN described below.
   If start_position == ANYWHERE, we aren't truncating columns and
   can begin printing a column anywhere.  Otherwise we must pad to
   the horizontal position start_position. */
#define ANYWHERE	0

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

ಸಂರಚನಾ COLUMN;
ಸಂರಚನಾ COLUMN
  {
    FILE *fp;			/* Input stream for this column. */
    ವರ್ಣ ಧ್ರುವ *name;		/* File name. */
    ಗಣನಾ
      {
        OPEN,
        FF_FOUND,		/* used with -b option, set with \f, changed
                                   to ON_HOLD after print_header */
        ON_HOLD,		/* Hit a form feed. */
        CLOSED
      }
    status;			/* Status of the file pointer. */

    /* Func to print lines in this col. */
    bool (*print_func) (ಸಂರಚನಾ COLUMN *);

    /* Func to print/store chars in this col. */
    ಶೂನ್ಯ (*char_func) (ವರ್ಣ);

    ಪೂರ್ಣಾಙ್ಕ current_ಸಾಲು;		/* Index of current place in line_vector. */
    ಪೂರ್ಣಾಙ್ಕ ಸಾಲುಗಳು_stored;		/* Number of lines stored in buff. */
    ಪೂರ್ಣಾಙ್ಕ ಸಾಲುಗಳು_to_print;		/* No. lines stored or space left on page. */
    ಪೂರ್ಣಾಙ್ಕ start_position;		/* Horizontal position of first char. */
    bool numbered;
    bool full_page_printed;	/* True means printed without a FF found. */

    /* p->full_page_printed  controls a special case of "FF set by hand":
       True means a full page has been printed without FF found. To avoid an
       additional empty page we have to ignore a FF immediately following in
       the next line. */
  };

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ COLUMN COLUMN;

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ char_to_clump (ವರ್ಣ c);
ಸ್ಥಿರ bool read_ಸಾಲು (COLUMN *p);
ಸ್ಥಿರ bool print_page (ಶೂನ್ಯ);
ಸ್ಥಿರ bool print_stored (COLUMN *p);
ಸ್ಥಿರ bool open_file (ವರ್ಣ *name, COLUMN *p);
ಸ್ಥಿರ bool skip_to_page (uintmax_t page);
ಸ್ಥಿರ ಶೂನ್ಯ print_header (ಶೂನ್ಯ);
ಸ್ಥಿರ ಶೂನ್ಯ pad_across_to (ಪೂರ್ಣಾಙ್ಕ position);
ಸ್ಥಿರ ಶೂನ್ಯ add_ಸಾಲು_ಸಂಖ್ಯೆ (COLUMN *p);
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ getoptnum (ವರ್ಣ ಧ್ರುವ *n_str, ಪೂರ್ಣಾಙ್ಕ min, ವರ್ಣ ಧ್ರುವ *errfmt);
ಸ್ಥಿರ ಶೂನ್ಯ getoptarg (ವರ್ಣ *arg, ವರ್ಣ switch_char, ವರ್ಣ *character,
                       ಪೂರ್ಣಾಙ್ಕ *ಸಂಖ್ಯೆ);
ಸ್ಥಿರ ಶೂನ್ಯ print_files (ಪೂರ್ಣಾಙ್ಕ ಸಂಖ್ಯೆ_of_files, ವರ್ಣ **av);
ಸ್ಥಿರ ಶೂನ್ಯ init_parameters (ಪೂರ್ಣಾಙ್ಕ ಸಂಖ್ಯೆ_of_files);
ಸ್ಥಿರ ಶೂನ್ಯ init_header (ವರ್ಣ ಧ್ರುವ *filename, ಪೂರ್ಣಾಙ್ಕ desc);
ಸ್ಥಿರ bool init_fps (ಪೂರ್ಣಾಙ್ಕ ಸಂಖ್ಯೆ_of_files, ವರ್ಣ **av);
ಸ್ಥಿರ ಶೂನ್ಯ init_funcs (ಶೂನ್ಯ);
ಸ್ಥಿರ ಶೂನ್ಯ init_store_cols (ಶೂನ್ಯ);
ಸ್ಥಿರ ಶೂನ್ಯ store_columns (ಶೂನ್ಯ);
ಸ್ಥಿರ ಶೂನ್ಯ balance (ಪೂರ್ಣಾಙ್ಕ total_stored);
ಸ್ಥಿರ ಶೂನ್ಯ store_char (ವರ್ಣ c);
ಸ್ಥಿರ ಶೂನ್ಯ pad_down (unsigned ಪೂರ್ಣಾಙ್ಕ ಸಾಲುಗಳು);
ಸ್ಥಿರ ಶೂನ್ಯ read_rest_of_ಸಾಲು (COLUMN *p);
ಸ್ಥಿರ ಶೂನ್ಯ skip_read (COLUMN *p, ಪೂರ್ಣಾಙ್ಕ column_ಸಂಖ್ಯೆ);
ಸ್ಥಿರ ಶೂನ್ಯ print_char (ವರ್ಣ c);
ಸ್ಥಿರ ಶೂನ್ಯ cleanup (ಶೂನ್ಯ);
ಸ್ಥಿರ ಶೂನ್ಯ print_sep_string (ಶೂನ್ಯ);
ಸ್ಥಿರ ಶೂನ್ಯ separator_string (ವರ್ಣ ಧ್ರುವ *optarg_S);

/* All of the columns to print.  */
ಸ್ಥಿರ COLUMN *column_vector;

/* When printing a single file in multiple downward columns,
   we store the leftmost columns contiguously in buff.
   To print a line from buff, get the index of the first character
   from line_vector[i], and print up to line_vector[i + 1]. */
ಸ್ಥಿರ ವರ್ಣ *buff;

/* Index of the position in buff where the next character
   will be stored. */
ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ buff_current;

/* The number of characters in buff.
   Used for allocation of buff and to detect overflow of buff. */
ಸ್ಥಿರ idx_t buff_allocated;

/* Array of indices into buff.
   Each entry is an index of the first character of a line.
   This is used when storing lines to facilitate shuffling when
   we do column balancing on the last page. */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ *ಸಾಲು_vector;

/* Array of horizontal positions.
   For each line in line_vector, end_vector[line] is the horizontal
   position we are in after printing that line.  We keep track of this
   so that we know how much we need to pad to prepare for the next
   column. */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ *end_vector;

/* (-m) True means we're printing multiple files in parallel. */
ಸ್ಥಿರ bool parallel_files = false;

/* (-m) True means a line starts with some empty columns (some files
   already CLOSED or ON_HOLD) which we have to align. */
ಸ್ಥಿರ bool align_empty_cols;

/* (-m) True means we have not yet found any printable column in a line.
   align_empty_cols = true  has to be maintained. */
ಸ್ಥಿರ bool empty_ಸಾಲು;

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
ಸ್ಥಿರ bool print_a_header;

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
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ ಸಾಲುಗಳು_per_page = 66;

/* Number of lines in the header and footer can be reset to 0 using
   the -t flag. */
ಗಣನಾ { ಸಾಲುಗಳು_per_header = 5 };
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ ಸಾಲುಗಳು_per_body;
ಗಣನಾ { ಸಾಲುಗಳು_per_footer = 5 };

/* (-w|-W) Width in characters of the page.  Does not include the width of
   the margin. */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ chars_per_ಸಾಲು = 72;

/* (-w|W) True means we truncate lines longer than chars_per_column. */
ಸ್ಥಿರ bool truncate_ಸಾಲುಗಳು = false;

/* (-J) True means we join lines without any line truncation. -J
   dominates -w option. */
ಸ್ಥಿರ bool join_ಸಾಲುಗಳು = false;

/* Number of characters in a column.  Based on col_sep_length and
   page width. */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ chars_per_column;

/* (-e) True means convert tabs to spaces on input. */
ಸ್ಥಿರ bool untabify_ಒಳಹರಿವು = false;

/* (-e) The input tab character. */
ಸ್ಥಿರ ವರ್ಣ ಒಳಹರಿವು_tab_char = '\t';

/* (-e) Tabstops are at chars_per_tab, 2*chars_per_tab, 3*chars_per_tab, ...
   where the leftmost column is 1. */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ chars_per_ಒಳಹರಿವು_tab = 8;

/* (-i) True means convert spaces to tabs on output. */
ಸ್ಥಿರ bool tabify_output = false;

/* (-i) The output tab character. */
ಸ್ಥಿರ ವರ್ಣ output_tab_char = '\t';

/* (-i) The width of the output tab. */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ chars_per_output_tab = 8;

/* Keeps track of pending white space.  When we hit a nonspace
   character after some whitespace, we print whitespace, tabbing
   if necessary to get to output_position + spaces_not_printed. */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ spaces_not_printed;

/* (-o) Number of spaces in the left margin (tabs used when possible). */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ chars_per_margin = 0;

/* Position where the next character will fall.
   Leftmost position is 0 + chars_per_margin.
   Rightmost position is chars_per_margin + chars_per_line - 1.
   This is important for converting spaces to tabs on output. */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ output_position;

/* Horizontal position relative to the current file.
   (output_position depends on where we are on the page;
   input_position depends on where we are in the file.)
   Important for converting tabs to spaces on input. */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ ಒಳಹರಿವು_position;

/* True if there were any failed opens so we can exit with nonzero
   status.  */
ಸ್ಥಿರ bool failed_opens = false;

/* The number of spaces taken up if we print a tab character with width
   c_ from position h_. */
#define TAB_WIDTH(c_, h_) ((c_) - ((h_) % (c_)))

/* The horizontal position we'll be at after printing a tab character
   of width c_ from the position h_. */
#define POS_AFTER_TAB(c_, h_) ((h_) + TAB_WIDTH (c_, h_))

/* (-NNN) Number of columns of text to print. */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ columns = 1;

/* (+NNN:MMM) Page numbers on which to begin and stop printing.
   first_page_number = 0  will be used to check input only. */
ಸ್ಥಿರ uintmax_t first_page_ಸಂಖ್ಯೆ = 0;
ಸ್ಥಿರ uintmax_t last_page_ಸಂಖ್ಯೆ = UINTMAX_MAX;

/* Number of files open (not closed, not on hold). */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ files_ready_to_read = 0;

/* Current page number.  Displayed in header. */
ಸ್ಥಿರ uintmax_t page_ಸಂಖ್ಯೆ;

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
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ ಸಾಲು_ಸಂಖ್ಯೆ;

/* (-n) True means lines should be preceded by numbers. */
ಸ್ಥಿರ bool numbered_ಸಾಲುಗಳು = false;

/* (-n) Character which follows each line number. */
ಸ್ಥಿರ ವರ್ಣ ಸಂಖ್ಯೆ_separator = '\t';

/* (-n) line counting starts with 1st line of input file (not with 1st
   line of 1st page printed). */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ ಸಾಲು_ಎಣಿಕೆ = 1;

/* (-n) True means counting of skipped lines starts with 1st line of
   input file. False means -N option is used in addition, counting of
   skipped lines not required. */
ಸ್ಥಿರ bool skip_ಎಣಿಕೆ = true;

/* (-N) Counting starts with start_line_number = NUMBER at 1st line of
   first page printed, usually not 1st page of input file. */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ start_ಸಾಲು_ಅಂಕೆ = 1;

/* (-n) Width in characters of a line number. */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ chars_per_ಸಂಖ್ಯೆ = 5;

/* Used when widening the first column to accommodate numbers -- only
   needed when printing files in parallel.  Includes width of both the
   number and the number_separator. */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ ಸಂಖ್ಯೆ_width;

/* Buffer sprintf uses to format a line number. */
ಸ್ಥಿರ ವರ್ಣ *ಸಂಖ್ಯೆ_buff;

/* (-v) True means nonprintable characters are printed as escape sequences.
   control-g becomes \007. */
ಸ್ಥಿರ bool use_esc_sequence = false;

/* (-c) True means nonprintable characters are printed as control prefixes.
   control-g becomes ^G. */
ಸ್ಥಿರ bool use_cntrl_prefix = false;

/* (-d) True means output is double spaced. */
ಸ್ಥಿರ bool double_space = false;

/* Number of files opened initially in init_files.  Should be 1
   unless we're printing multiple files in parallel. */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ total_files = 0;

/* (-r) True means don't complain if we can't open a file. */
ಸ್ಥಿರ bool ignore_failed_opens = false;

/* (-S) True means we separate columns with a specified string.
   -S option does not affect line truncation nor column alignment. */
ಸ್ಥಿರ bool use_col_separator = false;

/* String used to separate columns if the -S option has been specified.
   Default without -S but together with one of the column options
   -a|COLUMN|-m is a 'space' and with the -J option a 'tab'. */
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *col_sep_string = "";
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ col_sep_length = 0;
ಸ್ಥಿರ ವರ್ಣ *column_separator = (ವರ್ಣ *) " ";
ಸ್ಥಿರ ವರ್ಣ *ಸಾಲು_separator = (ವರ್ಣ *) "\t";

/* Number of separator characters waiting to be printed as soon as we
   know that we have any input remaining to be printed. */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ separators_not_printed;

/* Position we need to pad to, as soon as we know that we have input
   remaining to be printed. */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ padding_not_printed;

/* True means we should pad the end of the page.  Remains false until we
   know we have a page to print. */
ಸ್ಥಿರ bool pad_vertically;

/* (-h) String of characters used in place of the filename in the header. */
ಸ್ಥಿರ ವರ್ಣ *custom_header;

/* (-D) Date format for the header.  */
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *date_format;

/* The local time zone rules, as per the TZ environment variable.  */
ಸ್ಥಿರ timezone_t localtz;

/* Date and file name for the header.  */
ಸ್ಥಿರ ವರ್ಣ *date_text;
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *file_text;

/* Output columns available, not counting the date and file name.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ header_width_available;

ಸ್ಥಿರ ವರ್ಣ *clump_buff;

/* True means we read the line no. lines_per_body in skip_read
   called by skip_to_page. That variable controls the coincidence of a
   "FF set by hand" and "full_page_printed", see above the definition of
   structure COLUMN. */
ಸ್ಥಿರ bool last_ಸಾಲು = false;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ಗಣನಾ
{
  COLUMNS_OPTION = CHAR_MAX + 1,
  PAGES_OPTION
};

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ short_options[] =
  "-0123456789D:FJN:S::TW:abcde::fh:i::l:mn::o:rs::tvw:";

ಸ್ಥಿರ ಸಂರಚನಾ option ಧ್ರುವ long_options[] =
{
  {"pages", required_argument, ರಿಕ್ತ, PAGES_OPTION},
  {"columns", required_argument, ರಿಕ್ತ, COLUMNS_OPTION},
  {"across", no_argument, ರಿಕ್ತ, 'a'},
  {"show-control-chars", no_argument, ರಿಕ್ತ, 'c'},
  {"double-space", no_argument, ರಿಕ್ತ, 'd'},
  {"date-format", required_argument, ರಿಕ್ತ, 'D'},
  {"expand-tabs", optional_argument, ರಿಕ್ತ, 'e'},
  {"form-feed", no_argument, ರಿಕ್ತ, 'f'},
  {"header", required_argument, ರಿಕ್ತ, 'h'},
  {"output-tabs", optional_argument, ರಿಕ್ತ, 'i'},
  {"join-lines", no_argument, ರಿಕ್ತ, 'J'},
  {"length", required_argument, ರಿಕ್ತ, 'l'},
  {"merge", no_argument, ರಿಕ್ತ, 'm'},
  {"number-lines", optional_argument, ರಿಕ್ತ, 'n'},
  {"first-line-number", required_argument, ರಿಕ್ತ, 'N'},
  {"indent", required_argument, ರಿಕ್ತ, 'o'},
  {"no-file-warnings", no_argument, ರಿಕ್ತ, 'r'},
  {"separator", optional_argument, ರಿಕ್ತ, 's'},
  {"sep-string", optional_argument, ರಿಕ್ತ, 'S'},
  {"omit-header", no_argument, ರಿಕ್ತ, 't'},
  {"omit-pagination", no_argument, ರಿಕ್ತ, 'T'},
  {"show-nonprinting", no_argument, ರಿಕ್ತ, 'v'},
  {"width", required_argument, ರಿಕ್ತ, 'w'},
  {"page-width", required_argument, ರಿಕ್ತ, 'W'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ರಿಕ್ತ, 0, ರಿಕ್ತ, 0}
};

ಸ್ಥಿರ _Noreturn ಶೂನ್ಯ
integer_overflow (ಶೂನ್ಯ)
{
  error (EXIT_FAILURE, 0, _("integer overflow"));
}

/* Return the number of columns that have either an open file or
   stored lines. */

ATTRIBUTE_PURE
ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ
cols_ready_to_print (ಶೂನ್ಯ)
{
  COLUMN *q;
  unsigned ಪೂರ್ಣಾಙ್ಕ i;
  unsigned ಪೂರ್ಣಾಙ್ಕ n;

  n = 0;
  ಪ್ರತಿ (q = column_vector, i = 0; i < columns; ++q, ++i)
    ಯದಿ (q->status == OPEN
        || q->status == FF_FOUND	/* With -b: To print a header only */
        || (storing_columns && q->ಸಾಲುಗಳು_stored > 0 && q->ಸಾಲುಗಳು_to_print > 0))
      ++n;
  ಪ್ರತ್ಯಾಗಮ n;
}

/* Estimate first_ / last_page_number
   using option +FIRST_PAGE:LAST_PAGE */

ಸ್ಥಿರ bool
first_last_page (ಪೂರ್ಣಾಙ್ಕ oi, ವರ್ಣ c, ವರ್ಣ ಧ್ರುವ *pages)
{
  ವರ್ಣ *p;
  uintmax_t first;
  uintmax_t last = UINTMAX_MAX;
  strtol_error err = xstrtoumax (pages, &p, 10, &first, "");
  ಯದಿ (err != LONGINT_OK && err != LONGINT_INVALID_SUFFIX_CHAR)
    xstrtol_fatal (err, oi, c, long_options, pages);

  ಯದಿ (p == pages || !first)
    ಪ್ರತ್ಯಾಗಮ false;

  ಯದಿ (*p == ':')
    {
      ವರ್ಣ ಧ್ರುವ *p1 = p + 1;
      err = xstrtoumax (p1, &p, 10, &last, "");
      ಯದಿ (err != LONGINT_OK)
        xstrtol_fatal (err, oi, c, long_options, pages);
      ಯದಿ (p1 == p || last < first)
        ಪ್ರತ್ಯಾಗಮ false;
    }

  ಯದಿ (*p)
    ಪ್ರತ್ಯಾಗಮ false;

  first_page_ಸಂಖ್ಯೆ = first;
  last_page_ಸಂಖ್ಯೆ = last;
  ಪ್ರತ್ಯಾಗಮ true;
}

/* Parse column count string S, and if it's valid (1 or larger and
   within range of the type of 'columns') set the global variables
   columns and explicit_columns.  Otherwise, exit with a diagnostic.  */

ಸ್ಥಿರ ಶೂನ್ಯ
parse_column_ಎಣಿಕೆ (ವರ್ಣ ಧ್ರುವ *s)
{
  columns = getoptnum (s, 1, _("invalid number of columns"));
  explicit_columns = true;
}

/* Estimate length of col_sep_string with option -S.  */

ಸ್ಥಿರ ಶೂನ್ಯ
separator_string (ವರ್ಣ ಧ್ರುವ *optarg_S)
{
  size_t len = strlen (optarg_S);
  ಯದಿ (INT_MAX < len)
    integer_overflow ();
  col_sep_length = len;
  col_sep_string = optarg_S;
}

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  unsigned ಪೂರ್ಣಾಙ್ಕ n_files;
  bool old_options = false;
  bool old_w = false;
  bool old_s = false;
  ವರ್ಣ **file_names;

  /* Accumulate the digits of old-style options like -99.  */
  ವರ್ಣ *column_ಎಣಿಕೆ_string = ರಿಕ್ತ;
  idx_t n_digits = 0;
  idx_t n_alloc = 0;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  n_files = 0;
  file_names = (argc > 1
                ? xnmalloc (argc - 1, ಪರಿಮಾಣ (ವರ್ಣ *))
                : ರಿಕ್ತ);

  ಯಾವತ್ (true)
    {
      ಪೂರ್ಣಾಙ್ಕ oi = -1;
      ಪೂರ್ಣಾಙ್ಕ c = getopt_long (argc, argv, short_options, long_options, &oi);
      ಯದಿ (c == -1)
        ಭಙ್ಗ;

      ಯದಿ (c_isdigit (c))
        {
          /* Accumulate column-count digits specified via old-style options. */
          ಯದಿ (n_digits + 1 >= n_alloc)
            column_ಎಣಿಕೆ_string = xpalloc (column_ಎಣಿಕೆ_string, &n_alloc, 2, -1,
                                           ಪರಿಮಾಣ *column_ಎಣಿಕೆ_string);
          column_ಎಣಿಕೆ_string[n_digits++] = c;
          column_ಎಣಿಕೆ_string[n_digits] = '\0';
          ಅನುವರ್ತ;
        }

      n_digits = 0;

      ವಿಕಲ್ಪ (c)
        {
        ಪಕ್ಷ 1:			/* Non-option argument. */
          /* long option --page dominates old '+FIRST_PAGE ...'.  */
          ಯದಿ (! (first_page_ಸಂಖ್ಯೆ == 0
                 && *optarg == '+' && first_last_page (-2, '+', optarg + 1)))
            file_names[n_files++] = optarg;
          ಭಙ್ಗ;

        ಪಕ್ಷ PAGES_OPTION:	/* --pages=FIRST_PAGE[:LAST_PAGE] */
          {			/* dominates old opt +... */
            ಯದಿ (! optarg)
              error (EXIT_FAILURE, 0,
                     _("'--pages=FIRST_PAGE[:LAST_PAGE]' missing argument"));
            ಅನ್ಯಥಾ ಯದಿ (! first_last_page (oi, 0, optarg))
              error (EXIT_FAILURE, 0, _("invalid page range %s"),
                     quote (optarg));
            ಭಙ್ಗ;
          }

        ಪಕ್ಷ COLUMNS_OPTION:	/* --columns=COLS */
          {
            parse_column_ಎಣಿಕೆ (optarg);

            /* If there was a prior column count specified via the
               short-named option syntax, e.g., -9, ensure that this
               long-name-specified value overrides it.  */
            ಮುಕ್ತ (column_ಎಣಿಕೆ_string);
            column_ಎಣಿಕೆ_string = ರಿಕ್ತ;
            n_alloc = 0;
            ಭಙ್ಗ;
          }

        ಪಕ್ಷ 'a':
          print_across_flag = true;
          storing_columns = false;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'b':
          balance_columns = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'c':
          use_cntrl_prefix = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'd':
          double_space = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'D':
          date_format = optarg;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'e':
          ಯದಿ (optarg)
            getoptarg (optarg, 'e', &ಒಳಹರಿವು_tab_char,
                       &chars_per_ಒಳಹರಿವು_tab);
          /* Could check tab width > 0. */
          untabify_ಒಳಹರಿವು = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'f':
        ಪಕ್ಷ 'F':
          use_form_feed = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'h':
          custom_header = optarg;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'i':
          ಯದಿ (optarg)
            getoptarg (optarg, 'i', &output_tab_char,
                       &chars_per_output_tab);
          /* Could check tab width > 0. */
          tabify_output = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'J':
          join_ಸಾಲುಗಳು = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'l':
          ಸಾಲುಗಳು_per_page
            = getoptnum (optarg, 1,
                         _("'-l PAGE_LENGTH' invalid number of lines"));
          ಭಙ್ಗ;
        ಪಕ್ಷ 'm':
          parallel_files = true;
          storing_columns = false;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'n':
          numbered_ಸಾಲುಗಳು = true;
          ಯದಿ (optarg)
            getoptarg (optarg, 'n', &ಸಂಖ್ಯೆ_separator,
                       &chars_per_ಸಂಖ್ಯೆ);
          ಭಙ್ಗ;
        ಪಕ್ಷ 'N':
          skip_ಎಣಿಕೆ = false;
          start_ಸಾಲು_ಅಂಕೆ
            = getoptnum (optarg, INT_MIN,
                         _("'-N NUMBER' invalid starting line number"));
          ಭಙ್ಗ;
        ಪಕ್ಷ 'o':
          chars_per_margin = getoptnum (optarg, 0,
                                        _("'-o MARGIN' invalid line offset"));
          ಭಙ್ಗ;
        ಪಕ್ಷ 'r':
          ignore_failed_opens = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ 's':
          old_options = true;
          old_s = true;
          ಯದಿ (!use_col_separator && optarg)
            separator_string (optarg);
          ಭಙ್ಗ;
        ಪಕ್ಷ 'S':
          old_s = false;
          /* Reset an additional input of -s, -S dominates -s */
          col_sep_string = "";
          col_sep_length = 0;
          use_col_separator = true;
          ಯದಿ (optarg)
            separator_string (optarg);
          ಭಙ್ಗ;
        ಪಕ್ಷ 't':
          extremities = false;
          keep_FF = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'T':
          extremities = false;
          keep_FF = false;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'v':
          use_esc_sequence = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'w':
          old_options = true;
          old_w = true;
          {
            ಪೂರ್ಣಾಙ್ಕ tmp_cpl
              = getoptnum (optarg, 1,
                           _("'-w PAGE_WIDTH' invalid number of characters"));
            ಯದಿ (! truncate_ಸಾಲುಗಳು)
              chars_per_ಸಾಲು = tmp_cpl;
          }
          ಭಙ್ಗ;
        ಪಕ್ಷ 'W':
          old_w = false;			/* dominates -w */
          truncate_ಸಾಲುಗಳು = true;
          chars_per_ಸಾಲು
            = getoptnum (optarg, 1,
                         _("'-W PAGE_WIDTH' invalid number of characters"));
          ಭಙ್ಗ;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        ಸ್ವಭಾವ:
          usage (EXIT_FAILURE);
          ಭಙ್ಗ;
        }
    }

  ಯದಿ (column_ಎಣಿಕೆ_string)
    {
      parse_column_ಎಣಿಕೆ (column_ಎಣಿಕೆ_string);
      ಮುಕ್ತ (column_ಎಣಿಕೆ_string);
    }

  ಯದಿ (! date_format)
    date_format = (getenv ("POSIXLY_CORRECT") && !hard_locale (LC_TIME)
                   ? "%b %e %H:%M %Y"
                   : "%Y-%m-%d %H:%M");

  localtz = tzalloc (getenv ("TZ"));

  /* Now we can set a reasonable initial value: */
  ಯದಿ (first_page_ಸಂಖ್ಯೆ == 0)
    first_page_ಸಂಖ್ಯೆ = 1;

  ಯದಿ (parallel_files && explicit_columns)
    error (EXIT_FAILURE, 0,
           _("cannot specify number of columns when printing in parallel"));

  ಯದಿ (parallel_files && print_across_flag)
    error (EXIT_FAILURE, 0,
           _("cannot specify both printing across and printing in parallel"));

/* Translate some old short options to new/long options.
   To meet downward compatibility with other UNIX pr utilities
   and some POSIX specifications. */

  ಯದಿ (old_options)
    {
      ಯದಿ (old_w)
        {
          ಯದಿ (parallel_files || explicit_columns)
            {
              /* activate -W */
              truncate_ಸಾಲುಗಳು = true;
              ಯದಿ (old_s)
                /* adapt HP-UX and SunOS: -s = no separator;
                   activate -S */
                use_col_separator = true;
            }
          ಅನ್ಯಥಾ
            /* old -w sets width with columns only
               activate -J */
            join_ಸಾಲುಗಳು = true;
        }
      ಅನ್ಯಥಾ ಯದಿ (!use_col_separator)
        {
          /* No -S option read */
          ಯದಿ (old_s && (parallel_files || explicit_columns))
            {
              ಯದಿ (!truncate_ಸಾಲುಗಳು)
                {
                  /* old -s (without -w and -W) annuls column alignment,
                  uses fields, activate -J */
                  join_ಸಾಲುಗಳು = true;
                  ಯದಿ (col_sep_length > 0)
                    /* activate -S */
                    use_col_separator = true;
                }
              ಅನ್ಯಥಾ
                /* with -W */
                /* adapt HP-UX and SunOS: -s = no separator;
                   activate -S */
                use_col_separator = true;
            }
        }
    }

  ಪ್ರತಿ (; optind < argc; optind++)
    {
      file_names[n_files++] = argv[optind];
    }

  ಯದಿ (n_files == 0)
    {
      /* No file arguments specified;  read from standard input.  */
      print_files (0, ರಿಕ್ತ);
    }
  ಅನ್ಯಥಾ
    {
      ಯದಿ (parallel_files)
        print_files (n_files, file_names);
      ಅನ್ಯಥಾ
        {
          ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < n_files; i++)
            print_files (1, &file_names[i]);
        }
    }

  cleanup ();

  ಯದಿ (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, _("standard input"));
  main_exit (failed_opens ? EXIT_FAILURE : EXIT_SUCCESS);
}

/* Parse numeric arguments, ensuring MIN <= number <= INT_MAX.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
getoptnum (ವರ್ಣ ಧ್ರುವ *n_str, ಪೂರ್ಣಾಙ್ಕ min, ವರ್ಣ ಧ್ರುವ *err)
{
  ಪ್ರತ್ಯಾಗಮ xnumtoimax (n_str, 10, min, INT_MAX, "", err, 0,
                     min <= 0 ? 0 : XTOINT_MIN_RANGE);
}

/* Parse options of the form -scNNN.

   Example: -nck, where 'n' is the option, c is the optional number
   separator, and k is the optional width of the field used when printing
   a number. */

ಸ್ಥಿರ ಶೂನ್ಯ
getoptarg (ವರ್ಣ *arg, ವರ್ಣ switch_char, ವರ್ಣ *character, ಪೂರ್ಣಾಙ್ಕ *ಸಂಖ್ಯೆ)
{
  ವರ್ಣ ಧ್ರುವ *ಅಂಕೆ = arg;

  ಯದಿ (!*ಅಂಕೆ)
    {
      error (0, 0, _("'-%c': Invalid argument: %s"), switch_char, quote (ಅಂಕೆ));
      usage (EXIT_FAILURE);
    }

  ಯದಿ (!c_isdigit (*ಅಂಕೆ))
    *character = *ಅಂಕೆ++;
  ಯದಿ (*ಅಂಕೆ)
    {
      long ಪೂರ್ಣಾಙ್ಕ tmp_long;
      strtol_error e = xstrtol (ಅಂಕೆ, ರಿಕ್ತ, 10, &tmp_long, "");
      ಯದಿ (e == LONGINT_OK)
        {
          ಯದಿ (tmp_long <= 0)
            e = LONGINT_INVALID;
          ಅನ್ಯಥಾ ಯದಿ (INT_MAX < tmp_long)
            e = LONGINT_OVERFLOW;
        }
      ಯದಿ (e != LONGINT_OK)
        {
          error (0, e & LONGINT_OVERFLOW ? EOVERFLOW : 0,
             _("'-%c' extra characters or invalid number in the argument: %s"),
                 switch_char, quote (arg));
          usage (EXIT_FAILURE);
        }
      *ಸಂಖ್ಯೆ = tmp_long;
    }
}

/* Set parameters related to formatting. */

ಸ್ಥಿರ ಶೂನ್ಯ
init_parameters (ಪೂರ್ಣಾಙ್ಕ ಸಂಖ್ಯೆ_of_files)
{
  ಪೂರ್ಣಾಙ್ಕ chars_used_by_ಸಂಖ್ಯೆ = 0;

  ಸಾಲುಗಳು_per_body = ಸಾಲುಗಳು_per_page - ಸಾಲುಗಳು_per_header - ಸಾಲುಗಳು_per_footer;
  ಯದಿ (ಸಾಲುಗಳು_per_body <= 0)
    {
      extremities = false;
      keep_FF = true;
    }
  ಯದಿ (! extremities)
    ಸಾಲುಗಳು_per_body = ಸಾಲುಗಳು_per_page;

  ಯದಿ (double_space)
    ಸಾಲುಗಳು_per_body = MAX (1, ಸಾಲುಗಳು_per_body / 2);

  /* If input is stdin, cannot print parallel files.  BSD dumps core
     on this. */
  ಯದಿ (ಸಂಖ್ಯೆ_of_files == 0)
    parallel_files = false;

  ಯದಿ (parallel_files)
    columns = ಸಂಖ್ಯೆ_of_files;

  /* One file, multi columns down: -b option is set to get a consistent
     formulation with "FF set by hand" in input files. */
  ಯದಿ (storing_columns)
    balance_columns = true;

  /* Tabification is assumed for multiple columns. */
  ಯದಿ (columns > 1)
    {
      ಯದಿ (!use_col_separator)
        {
          /* Use default separator */
          ಯದಿ (join_ಸಾಲುಗಳು)
            col_sep_string = ಸಾಲು_separator;
          ಅನ್ಯಥಾ
            col_sep_string = column_separator;

          col_sep_length = 1;
          use_col_separator = true;
        }
      /* It's rather pointless to define a TAB separator with column
         alignment */
      ಅನ್ಯಥಾ ಯದಿ (!join_ಸಾಲುಗಳು && col_sep_length == 1 && *col_sep_string == '\t')
        col_sep_string = column_separator;

      truncate_ಸಾಲುಗಳು = true;
      ಯದಿ (! (col_sep_length == 1 && *col_sep_string == '\t'))
        untabify_ಒಳಹರಿವು = true;
      tabify_output = true;
    }
  ಅನ್ಯಥಾ
    storing_columns = false;

  /* -J dominates -w in any case */
  ಯದಿ (join_ಸಾಲುಗಳು)
    truncate_ಸಾಲುಗಳು = false;

  ಯದಿ (numbered_ಸಾಲುಗಳು)
    {
      ಪೂರ್ಣಾಙ್ಕ chars_per_default_tab = 8;

      ಸಾಲು_ಎಣಿಕೆ = start_ಸಾಲು_ಅಂಕೆ;

      /* To allow input tab-expansion (-e sensitive) use:
         if (number_separator == input_tab_char)
           number_width = chars_per_number
             + TAB_WIDTH (chars_per_input_tab, chars_per_number);   */

      /* Estimate chars_per_text without any margin and keep it constant. */
      ಯದಿ (ಸಂಖ್ಯೆ_separator == '\t')
        ಸಂಖ್ಯೆ_width = (chars_per_ಸಂಖ್ಯೆ
                        + TAB_WIDTH (chars_per_default_tab, chars_per_ಸಂಖ್ಯೆ));
      ಅನ್ಯಥಾ
        ಸಂಖ್ಯೆ_width = chars_per_ಸಂಖ್ಯೆ + 1;

      /* The number is part of the column width unless we are
         printing files in parallel. */
      ಯದಿ (parallel_files)
        chars_used_by_ಸಂಖ್ಯೆ = ಸಂಖ್ಯೆ_width;
    }

  ಪೂರ್ಣಾಙ್ಕ sep_chars, useful_chars;
  ಯದಿ (ckd_mul (&sep_chars, columns - 1, col_sep_length))
    sep_chars = INT_MAX;
  ಯದಿ (ckd_sub (&useful_chars, chars_per_ಸಾಲು - chars_used_by_ಸಂಖ್ಯೆ,
               sep_chars))
    useful_chars = 0;
  chars_per_column = useful_chars / columns;

  ಯದಿ (chars_per_column < 1)
    error (EXIT_FAILURE, 0, _("page width too narrow"));

  ಯದಿ (numbered_ಸಾಲುಗಳು)
    {
      ಮುಕ್ತ (ಸಂಖ್ಯೆ_buff);
      ಸಂಖ್ಯೆ_buff = xmalloc (MAX (chars_per_ಸಂಖ್ಯೆ,
                                  INT_STRLEN_BOUND (ಸಾಲು_ಸಂಖ್ಯೆ)) + 1);
    }

  /* Pick the maximum between the tab width and the width of an
     escape sequence.
     The width of an escape sequence (4) isn't the lower limit any longer.
     We've to use 8 as the lower limit, if we use chars_per_default_tab = 8
     to expand a tab which is not an input_tab-char. */
  ಮುಕ್ತ (clump_buff);
  clump_buff = xmalloc (MAX (8, chars_per_ಒಳಹರಿವು_tab));
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
init_fps (ಪೂರ್ಣಾಙ್ಕ ಸಂಖ್ಯೆ_of_files, ವರ್ಣ **av)
{
  COLUMN *p;

  total_files = 0;

  ಮುಕ್ತ (column_vector);
  column_vector = xnmalloc (columns, ಪರಿಮಾಣ (COLUMN));

  ಯದಿ (parallel_files)
    {
      ಪೂರ್ಣಾಙ್ಕ files_ಎಡ = ಸಂಖ್ಯೆ_of_files;
      ಪ್ರತಿ (p = column_vector; files_ಎಡ--; ++p, ++av)
        {
          ಯದಿ (! open_file (*av, p))
            {
              --p;
              --columns;
            }
        }
      ಯದಿ (columns == 0)
        ಪ್ರತ್ಯಾಗಮ false;
      init_header ("", -1);
    }
  ಅನ್ಯಥಾ
    {
      p = column_vector;
      ಯದಿ (ಸಂಖ್ಯೆ_of_files > 0)
        {
          ಯದಿ (! open_file (*av, p))
            ಪ್ರತ್ಯಾಗಮ false;
          init_header (*av, fileno (p->fp));
          p->ಸಾಲುಗಳು_stored = 0;
        }
      ಅನ್ಯಥಾ
        {
          p->name = _("standard input");
          p->fp = stdin;
          have_read_stdin = true;
          p->status = OPEN;
          p->full_page_printed = false;
          ++total_files;
          init_header ("", -1);
          p->ಸಾಲುಗಳು_stored = 0;
        }

      ವರ್ಣ ಧ್ರುವ *firstname = p->name;
      FILE *firstfp = p->fp;
      ಪೂರ್ಣಾಙ್ಕ i;
      ಪ್ರತಿ (i = columns - 1, ++p; i; --i, ++p)
        {
          p->name = firstname;
          p->fp = firstfp;
          p->status = OPEN;
          p->full_page_printed = false;
          p->ಸಾಲುಗಳು_stored = 0;
        }
    }
  files_ready_to_read = total_files;
  ಪ್ರತ್ಯಾಗಮ true;
}

/* Determine print_func and char_func, the functions
   used by each column for printing and/or storing.

   Determine the horizontal position desired when we begin
   printing a column (p->start_position). */

ಸ್ಥಿರ ಶೂನ್ಯ
init_funcs (ಶೂನ್ಯ)
{
  ಪೂರ್ಣಾಙ್ಕ i, h, h_next;
  COLUMN *p;

  h = chars_per_margin;

  ಯದಿ (!truncate_ಸಾಲುಗಳು)
    h_next = ANYWHERE;
  ಅನ್ಯಥಾ
    {
      /* When numbering lines of parallel files, we enlarge the
         first column to accommodate the number.  Looks better than
         the Sys V approach. */
      ಯದಿ (parallel_files && numbered_ಸಾಲುಗಳು)
        h_next = h + chars_per_column + ಸಂಖ್ಯೆ_width;
      ಅನ್ಯಥಾ
        h_next = h + chars_per_column;
    }

  /* Enlarge p->start_position of first column to use the same form of
     padding_not_printed with all columns. */
  h = h + col_sep_length;

  /* This loop takes care of all but the rightmost column. */

  ಪ್ರತಿ (p = column_vector, i = 1; i < columns; ++p, ++i)
    {
      ಯದಿ (storing_columns)	/* One file, multi columns down. */
        {
          p->char_func = store_char;
          p->print_func = print_stored;
        }
      ಅನ್ಯಥಾ
        /* One file, multi columns across; or parallel files.  */
        {
          p->char_func = print_char;
          p->print_func = read_ಸಾಲು;
        }

      /* Number only the first column when printing files in
         parallel. */
      p->numbered = numbered_ಸಾಲುಗಳು && (!parallel_files || i == 1);
      p->start_position = h;

      /* If we don't truncate lines, all start_positions are
         ANYWHERE, except the first column's start_position when
         using a margin. */

      ಯದಿ (!truncate_ಸಾಲುಗಳು)
        {
          h = ANYWHERE;
          h_next = ANYWHERE;
        }
      ಅನ್ಯಥಾ
        {
          h = h_next + col_sep_length;
          h_next = h + chars_per_column;
        }
    }

  /* The rightmost column.

     Doesn't need to be stored unless we intend to balance
     columns on the last page. */
  ಯದಿ (storing_columns && balance_columns)
    {
      p->char_func = store_char;
      p->print_func = print_stored;
    }
  ಅನ್ಯಥಾ
    {
      p->char_func = print_char;
      p->print_func = read_ಸಾಲು;
    }

  p->numbered = numbered_ಸಾಲುಗಳು && (!parallel_files || i == 1);
  p->start_position = h;
}

/* Open a file.  Return true if successful.

   With each file p, p->full_page_printed is initialized,
   see also init_fps. */

ಸ್ಥಿರ bool
open_file (ವರ್ಣ *name, COLUMN *p)
{
  ಯದಿ (streq (name, "-"))
    {
      p->name = _("standard input");
      p->fp = stdin;
      have_read_stdin = true;
    }
  ಅನ್ಯಥಾ
    {
      p->name = name;
      p->fp = fopen (name, "r");
    }
  ಯದಿ (p->fp == ರಿಕ್ತ)
    {
      failed_opens = true;
      ಯದಿ (!ignore_failed_opens)
        error (0, errno, "%s", quotef (name));
      ಪ್ರತ್ಯಾಗಮ false;
    }
  fadvise (p->fp, FADVISE_SEQUENTIAL);
  p->status = OPEN;
  p->full_page_printed = false;
  ++total_files;
  ಪ್ರತ್ಯಾಗಮ true;
}

/* Close the file in P.

   If we aren't dealing with multiple files in parallel, we change
   the status of all columns in the column list to reflect the close. */

ಸ್ಥಿರ ಶೂನ್ಯ
close_file (COLUMN *p)
{
  COLUMN *q;
  ಪೂರ್ಣಾಙ್ಕ i;

  ಯದಿ (p->status == CLOSED)
    ಪ್ರತ್ಯಾಗಮ;

  ಪೂರ್ಣಾಙ್ಕ err = errno;
  ಯದಿ (!ferror (p->fp))
    err = 0;
  ಯದಿ (fileno (p->fp) == STDIN_FILENO)
    clearerr (p->fp);
  ಅನ್ಯಥಾ ಯದಿ (fclose (p->fp) != 0 && !err)
    err = errno;
  ಯದಿ (err)
    error (EXIT_FAILURE, err, "%s", quotef (p->name));

  ಯದಿ (!parallel_files)
    {
      ಪ್ರತಿ (q = column_vector, i = columns; i; ++q, --i)
        {
          q->status = CLOSED;
          ಯದಿ (q->ಸಾಲುಗಳು_stored == 0)
            {
              q->ಸಾಲುಗಳು_to_print = 0;
            }
        }
    }
  ಅನ್ಯಥಾ
    {
      p->status = CLOSED;
      p->ಸಾಲುಗಳು_to_print = 0;
    }

  --files_ready_to_read;
}

/* Put a file on hold until we start a new page,
   since we've hit a form feed.

   If we aren't dealing with parallel files, we must change the
   status of all columns in the column list. */

ಸ್ಥಿರ ಶೂನ್ಯ
hold_file (COLUMN *p)
{
  COLUMN *q;
  ಪೂರ್ಣಾಙ್ಕ i;

  ಯದಿ (!parallel_files)
    ಪ್ರತಿ (q = column_vector, i = columns; i; ++q, --i)
      {
        ಯದಿ (storing_columns)
          q->status = FF_FOUND;
        ಅನ್ಯಥಾ
          q->status = ON_HOLD;
      }
  ಅನ್ಯಥಾ
    p->status = ON_HOLD;

  p->ಸಾಲುಗಳು_to_print = 0;
  --files_ready_to_read;
}

/* Undo hold_file -- go through the column list and change any
   ON_HOLD columns to OPEN.  Used at the end of each page. */

ಸ್ಥಿರ ಶೂನ್ಯ
reset_status (ಶೂನ್ಯ)
{
  ಪೂರ್ಣಾಙ್ಕ i = columns;
  COLUMN *p;

  ಪ್ರತಿ (p = column_vector; i; --i, ++p)
    ಯದಿ (p->status == ON_HOLD)
      {
        p->status = OPEN;
        files_ready_to_read++;
      }

  ಯದಿ (storing_columns)
    {
      ಯದಿ (column_vector->status == CLOSED)
        /* We use the info to output an error message in  skip_to_page. */
        files_ready_to_read = 0;
      ಅನ್ಯಥಾ
        files_ready_to_read = 1;
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
print_files (ಪೂರ್ಣಾಙ್ಕ ಸಂಖ್ಯೆ_of_files, ವರ್ಣ **av)
{
  init_parameters (ಸಂಖ್ಯೆ_of_files);
  ಯದಿ (! init_fps (ಸಂಖ್ಯೆ_of_files, av))
    ಪ್ರತ್ಯಾಗಮ;
  ಯದಿ (storing_columns)
    init_store_cols ();

  ಯದಿ (first_page_ಸಂಖ್ಯೆ > 1)
    {
      ಯದಿ (!skip_to_page (first_page_ಸಂಖ್ಯೆ))
        ಪ್ರತ್ಯಾಗಮ;
      ಅನ್ಯಥಾ
        page_ಸಂಖ್ಯೆ = first_page_ಸಂಖ್ಯೆ;
    }
  ಅನ್ಯಥಾ
    page_ಸಂಖ್ಯೆ = 1;

  init_funcs ();

  ಸಾಲು_ಸಂಖ್ಯೆ = ಸಾಲು_ಎಣಿಕೆ;
  ಯಾವತ್ (print_page ())
    ;
}

/* Initialize header information.
   If DESC is non-negative, it is a file descriptor open to
   FILENAME for reading.  */

ಸ್ಥಿರ ಶೂನ್ಯ
init_header (ವರ್ಣ ಧ್ರುವ *filename, ಪೂರ್ಣಾಙ್ಕ desc)
{
  ವರ್ಣ *buf = ರಿಕ್ತ;
  ಸಂರಚನಾ stat st;
  ಸಂರಚನಾ timespec t;
  ಪೂರ್ಣಾಙ್ಕ ns;
  ಸಂರಚನಾ tm tm;

  /* If parallel files or standard input, use current date. */
  ಯದಿ (streq (filename, "-"))
    desc = -1;
  ಯದಿ (0 <= desc && fstat (desc, &st) == 0)
    t = get_stat_mtime (&st);
  ಅನ್ಯಥಾ
    {
      ಸ್ಥಿರ ಸಂರಚನಾ timespec timespec;
      ಯದಿ (! timespec.tv_sec)
        gettime (&timespec);
      t = timespec;
    }

  ns = t.tv_nsec;
  ಯದಿ (localtime_rz (localtz, &t.tv_sec, &tm))
    {
      ptrdiff_t len = nstrftime (ರಿಕ್ತ, MIN (PTRDIFF_MAX, SIZE_MAX),
                                 date_format, &tm, localtz, ns);
      ಯದಿ (0 <= len)
        {
          buf = ximalloc (len + 1);
          nstrftime (buf, len + 1, date_format, &tm, localtz, ns);
        }
    }
  ಯದಿ (!buf)
    {
      ವರ್ಣ secbuf[INT_BUFSIZE_BOUND (intmax_t)];
      buf = xmalloc (ಪರಿಮಾಣ secbuf + MAX (10, INT_BUFSIZE_BOUND (ಪೂರ್ಣಾಙ್ಕ)));
      sprintf (buf, "%s.%09d", timetostr (t.tv_sec, secbuf), ns);
    }

  ಮುಕ್ತ (date_text);
  date_text = buf;
  file_text = custom_header ? custom_header : desc < 0 ? "" : filename;
  header_width_available = (chars_per_ಸಾಲು
                            - mbswidth (date_text, 0)
                            - mbswidth (file_text, 0));
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
init_page (ಶೂನ್ಯ)
{
  ಪೂರ್ಣಾಙ್ಕ j;
  COLUMN *p;

  ಯದಿ (storing_columns)
    {
      store_columns ();
      ಪ್ರತಿ (j = columns - 1, p = column_vector; j; --j, ++p)
        {
          p->ಸಾಲುಗಳು_to_print = p->ಸಾಲುಗಳು_stored;
        }

      /* Last column. */
      ಯದಿ (balance_columns)
        {
          p->ಸಾಲುಗಳು_to_print = p->ಸಾಲುಗಳು_stored;
        }
      /* Since we're not balancing columns, we don't need to store
         the rightmost column.   Read it straight from the file. */
      ಅನ್ಯಥಾ
        {
          ಯದಿ (p->status == OPEN)
            {
              p->ಸಾಲುಗಳು_to_print = ಸಾಲುಗಳು_per_body;
            }
          ಅನ್ಯಥಾ
            p->ಸಾಲುಗಳು_to_print = 0;
        }
    }
  ಅನ್ಯಥಾ
    ಪ್ರತಿ (j = columns, p = column_vector; j; --j, ++p)
      ಯದಿ (p->status == OPEN)
        {
          p->ಸಾಲುಗಳು_to_print = ಸಾಲುಗಳು_per_body;
        }
      ಅನ್ಯಥಾ
        p->ಸಾಲುಗಳು_to_print = 0;
}

/* Align empty columns and print separators.
   Empty columns will be formed by files with status ON_HOLD or CLOSED
   when printing multiple files in parallel. */

ಸ್ಥಿರ ಶೂನ್ಯ
align_column (COLUMN *p)
{
  padding_not_printed = p->start_position;
  ಯದಿ (col_sep_length < padding_not_printed)
    {
      pad_across_to (padding_not_printed - col_sep_length);
      padding_not_printed = ANYWHERE;
    }

  ಯದಿ (use_col_separator)
    print_sep_string ();

  ಯದಿ (p->numbered)
    add_ಸಾಲು_ಸಂಖ್ಯೆ (p);
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
print_page (ಶೂನ್ಯ)
{
  ಪೂರ್ಣಾಙ್ಕ j;
  ಪೂರ್ಣಾಙ್ಕ ಸಾಲುಗಳು_ಎಡ_on_page;
  COLUMN *p;

  /* Used as an accumulator (with | operator) of successive values of
     pad_vertically.  The trick is to set pad_vertically
     to false before each run through the inner loop, then after that
     loop, it tells us whether a line was actually printed (whether a
     newline needs to be output -- or two for double spacing).  But those
     values have to be accumulated (in pv) so we can invoke pad_down
     properly after the outer loop completes. */
  bool pv;

  init_page ();

  ಯದಿ (cols_ready_to_print () == 0)
    ಪ್ರತ್ಯಾಗಮ false;

  ಯದಿ (extremities)
    print_a_header = true;

  /* Don't pad unless we know a page was printed. */
  pad_vertically = false;
  pv = false;

  ಸಾಲುಗಳು_ಎಡ_on_page = ಸಾಲುಗಳು_per_body;
  ಯದಿ (double_space)
    ಸಾಲುಗಳು_ಎಡ_on_page *= 2;

  ಯಾವತ್ (ಸಾಲುಗಳು_ಎಡ_on_page > 0 && cols_ready_to_print () > 0)
    {
      output_position = 0;
      spaces_not_printed = 0;
      separators_not_printed = 0;
      pad_vertically = false;
      align_empty_cols = false;
      empty_ಸಾಲು = true;

      ಪ್ರತಿ (j = 1, p = column_vector; j <= columns; ++j, ++p)
        {
          ಒಳಹರಿವು_position = 0;
          ಯದಿ (p->ಸಾಲುಗಳು_to_print > 0 || p->status == FF_FOUND)
            {
              FF_only = false;
              padding_not_printed = p->start_position;
              ಯದಿ (!(p->print_func) (p))
                read_rest_of_ಸಾಲು (p);
              pv |= pad_vertically;

              --p->ಸಾಲುಗಳು_to_print;
              ಯದಿ (p->ಸಾಲುಗಳು_to_print <= 0)
                {
                  ಯದಿ (cols_ready_to_print () == 0)
                    ಭಙ್ಗ;
                }

              /* File p changed its status to ON_HOLD or CLOSED */
              ಯದಿ (parallel_files && p->status != OPEN)
                {
                  ಯದಿ (empty_ಸಾಲು)
                    align_empty_cols = true;
                  ಅನ್ಯಥಾ ಯದಿ (p->status == CLOSED
                           || (p->status == ON_HOLD && FF_only))
                    align_column (p);
                }
            }
          ಅನ್ಯಥಾ ಯದಿ (parallel_files)
            {
              /* File status ON_HOLD or CLOSED */
              ಯದಿ (empty_ಸಾಲು)
                align_empty_cols = true;
              ಅನ್ಯಥಾ
                align_column (p);
            }

          /* We need it also with an empty column */
          ಯದಿ (use_col_separator)
            ++separators_not_printed;
        }

      ಯದಿ (pad_vertically)
        {
          putchar ('\n');
          --ಸಾಲುಗಳು_ಎಡ_on_page;
        }

      ಯದಿ (cols_ready_to_print () == 0 && !extremities)
        ಭಙ್ಗ;

      ಯದಿ (double_space && pv)
        {
          putchar ('\n');
          --ಸಾಲುಗಳು_ಎಡ_on_page;
        }
    }

  ಯದಿ (ಸಾಲುಗಳು_ಎಡ_on_page == 0)
    ಪ್ರತಿ (j = 1, p = column_vector; j <= columns; ++j, ++p)
      ಯದಿ (p->status == OPEN)
        p->full_page_printed = true;

  pad_vertically = pv;

  ಯದಿ (pad_vertically && extremities)
    pad_down (ಸಾಲುಗಳು_ಎಡ_on_page + ಸಾಲುಗಳು_per_footer);
  ಅನ್ಯಥಾ ಯದಿ (keep_FF && print_a_FF)
    {
      putchar ('\f');
      print_a_FF = false;
    }

  ಯದಿ (ferror (stdout))
    write_error ();

  ಯದಿ (last_page_ಸಂಖ್ಯೆ < ++page_ಸಂಖ್ಯೆ)
    ಪ್ರತ್ಯಾಗಮ false;		/* Stop printing with LAST_PAGE */

  reset_status ();		/* Change ON_HOLD to OPEN. */

  ಪ್ರತ್ಯಾಗಮ true;			/* More pages to go. */
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
init_store_cols (ಶೂನ್ಯ)
{
  /* Tune this.  */
  ಪೂರ್ಣಾಙ್ಕ total_ಸಾಲುಗಳು, total_ಸಾಲುಗಳು_1, chars_per_column_1, chars_if_truncate;
  ಯದಿ (ckd_mul (&total_ಸಾಲುಗಳು, ಸಾಲುಗಳು_per_body, columns)
      || ckd_add (&total_ಸಾಲುಗಳು_1, total_ಸಾಲುಗಳು, 1)
      || ckd_add (&chars_per_column_1, chars_per_column, 1)
      || ckd_mul (&chars_if_truncate, total_ಸಾಲುಗಳು, chars_per_column_1)
      || ckd_mul (&buff_allocated, chars_if_truncate, use_col_separator + 1))
    integer_overflow ();

  ಮುಕ್ತ (ಸಾಲು_vector);
  /* FIXME: here's where it was allocated.  */
  ಸಾಲು_vector = xnmalloc (total_ಸಾಲುಗಳು_1, ಪರಿಮಾಣ *ಸಾಲು_vector);

  ಮುಕ್ತ (end_vector);
  end_vector = xnmalloc (total_ಸಾಲುಗಳು, ಪರಿಮಾಣ *end_vector);

  ಮುಕ್ತ (buff);
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
  ಪೂರ್ಣಾಙ್ಕ i, j;
  unsigned ಪೂರ್ಣಾಙ್ಕ ಸಾಲು = 0;
  unsigned ಪೂರ್ಣಾಙ್ಕ buff_start;
  ಪೂರ್ಣಾಙ್ಕ last_col;		/* The rightmost column which will be saved in buff */
  COLUMN *p;

  buff_current = 0;
  buff_start = 0;

  ಯದಿ (balance_columns)
    last_col = columns;
  ಅನ್ಯಥಾ
    last_col = columns - 1;

  ಪ್ರತಿ (i = 1, p = column_vector; i <= last_col; ++i, ++p)
    p->ಸಾಲುಗಳು_stored = 0;

  ಪ್ರತಿ (i = 1, p = column_vector; i <= last_col && files_ready_to_read;
       ++i, ++p)
    {
      p->current_ಸಾಲು = ಸಾಲು;
      ಪ್ರತಿ (j = ಸಾಲುಗಳು_per_body; j && files_ready_to_read; --j)

        ಯದಿ (p->status == OPEN)	/* Redundant.  Clean up. */
          {
            ಒಳಹರಿವು_position = 0;

            ಯದಿ (!read_ಸಾಲು (p))
              read_rest_of_ಸಾಲು (p);

            ಯದಿ (p->status == OPEN
                || buff_start != buff_current)
              {
                ++p->ಸಾಲುಗಳು_stored;
                ಸಾಲು_vector[ಸಾಲು] = buff_start;
                end_vector[ಸಾಲು++] = ಒಳಹರಿವು_position;
                buff_start = buff_current;
              }
          }
    }

  /* Keep track of the location of the last char in buff. */
  ಸಾಲು_vector[ಸಾಲು] = buff_start;

  ಯದಿ (balance_columns)
    balance (ಸಾಲು);
}

ಸ್ಥಿರ ಶೂನ್ಯ
balance (ಪೂರ್ಣಾಙ್ಕ total_stored)
{
  COLUMN *p;
  ಪೂರ್ಣಾಙ್ಕ i, ಸಾಲುಗಳು;
  ಪೂರ್ಣಾಙ್ಕ first_ಸಾಲು = 0;

  ಪ್ರತಿ (i = 1, p = column_vector; i <= columns; ++i, ++p)
    {
      ಸಾಲುಗಳು = total_stored / columns;
      ಯದಿ (i <= total_stored % columns)
        ++ಸಾಲುಗಳು;

      p->ಸಾಲುಗಳು_stored = ಸಾಲುಗಳು;
      p->current_ಸಾಲು = first_ಸಾಲು;

      first_ಸಾಲು += ಸಾಲುಗಳು;
    }
}

/* Store a character in the buffer. */

ಸ್ಥಿರ ಶೂನ್ಯ
store_char (ವರ್ಣ c)
{
  ಯದಿ (buff_current >= buff_allocated)
    {
      /* May be too generous. */
      buff = xpalloc (buff, &buff_allocated, 1, -1, ಪರಿಮಾಣ *buff);
    }
  buff[buff_current++] = c;
}

ಸ್ಥಿರ ಶೂನ್ಯ
add_ಸಾಲು_ಸಂಖ್ಯೆ (COLUMN *p)
{
  ಪೂರ್ಣಾಙ್ಕ i;
  ವರ್ಣ *s;
  ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_width;

  /* Cutting off the higher-order digits is more informative than
     lower-order cut off. */
  ಅಂಕೆ_width = sprintf (ಸಂಖ್ಯೆ_buff, "%*d", chars_per_ಸಂಖ್ಯೆ, ಸಾಲು_ಸಂಖ್ಯೆ);
  ಸಾಲು_ಸಂಖ್ಯೆ++;
  s = ಸಂಖ್ಯೆ_buff + (ಅಂಕೆ_width - chars_per_ಸಂಖ್ಯೆ);
  ಪ್ರತಿ (i = chars_per_ಸಂಖ್ಯೆ; i > 0; i--)
    (p->char_func) (*s++);

  ಯದಿ (columns > 1)
    {
      /* Tabification is assumed for multiple columns, also for n-separators,
         but 'default n-separator = TAB' hasn't been given priority over
         equal column_width also specified by POSIX. */
      ಯದಿ (ಸಂಖ್ಯೆ_separator == '\t')
        {
          i = ಸಂಖ್ಯೆ_width - chars_per_ಸಂಖ್ಯೆ;
          ಯಾವತ್ (i-- > 0)
            (p->char_func) (' ');
        }
      ಅನ್ಯಥಾ
        (p->char_func) (ಸಂಖ್ಯೆ_separator);
    }
  ಅನ್ಯಥಾ
    /* To comply with POSIX, we avoid any expansion of default TAB
       separator with a single column output. No column_width requirement
       has to be considered. */
    {
      (p->char_func) (ಸಂಖ್ಯೆ_separator);
      ಯದಿ (ಸಂಖ್ಯೆ_separator == '\t')
        output_position = POS_AFTER_TAB (chars_per_output_tab,
                          output_position);
    }

  ಯದಿ (truncate_ಸಾಲುಗಳು && !parallel_files)
    ಒಳಹರಿವು_position += ಸಂಖ್ಯೆ_width;
}

/* Print (or store) padding until the current horizontal position
   is position. */

ಸ್ಥಿರ ಶೂನ್ಯ
pad_across_to (ಪೂರ್ಣಾಙ್ಕ position)
{
  ಪೂರ್ಣಾಙ್ಕ h = output_position;

  ಯದಿ (tabify_output)
    spaces_not_printed = position - output_position;
  ಅನ್ಯಥಾ
    {
      ಯಾವತ್ (++h <= position)
        putchar (' ');
      output_position = position;
    }
}

/* Pad to the bottom of the page.

   If the user has requested a formfeed, use one.
   Otherwise, use newlines. */

ಸ್ಥಿರ ಶೂನ್ಯ
pad_down (unsigned ಪೂರ್ಣಾಙ್ಕ ಸಾಲುಗಳು)
{
  ಯದಿ (use_form_feed)
    putchar ('\f');
  ಅನ್ಯಥಾ
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = ಸಾಲುಗಳು; i; --i)
      putchar ('\n');
}

/* Read the rest of the line.

   Read from the current column's file until an end of line is
   hit.  Used when we've truncated a line and we no longer need
   to print or store its characters. */

ಸ್ಥಿರ ಶೂನ್ಯ
read_rest_of_ಸಾಲು (COLUMN *p)
{
  ಪೂರ್ಣಾಙ್ಕ c;
  FILE *f = p->fp;

  ಯಾವತ್ ((c = getc (f)) != '\n')
    {
      ಯದಿ (c == '\f')
        {
          ಯದಿ ((c = getc (f)) != '\n')
            ungetc (c, f);
          ಯದಿ (keep_FF)
            print_a_FF = true;
          hold_file (p);
          ಭಙ್ಗ;
        }
      ಅನ್ಯಥಾ ಯದಿ (c == EOF)
        {
          close_file (p);
          ಭಙ್ಗ;
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
skip_read (COLUMN *p, ಪೂರ್ಣಾಙ್ಕ column_ಸಂಖ್ಯೆ)
{
  ಪೂರ್ಣಾಙ್ಕ c;
  FILE *f = p->fp;
  ಪೂರ್ಣಾಙ್ಕ i;
  bool single_ff = false;
  COLUMN *q;

  /* Read 1st character in a line or any character succeeding a FF */
  ಯದಿ ((c = getc (f)) == '\f' && p->full_page_printed)
    /* A FF-coincidence with a previous full_page_printed.
       To avoid an additional empty page, eliminate the FF */
    ಯದಿ ((c = getc (f)) == '\n')
      c = getc (f);

  p->full_page_printed = false;

  /* 1st character a FF means a single FF without any printable
     characters. Don't count it as a line with -n option. */
  ಯದಿ (c == '\f')
    single_ff = true;

  /* Preparing for a FF-coincidence: Maybe we finish that page
     without a FF found */
  ಯದಿ (last_ಸಾಲು)
    p->full_page_printed = true;

  ಯಾವತ್ (c != '\n')
    {
      ಯದಿ (c == '\f')
        {
          /* No FF-coincidence possible,
             no catching up of a FF-coincidence with next page */
          ಯದಿ (last_ಸಾಲು)
            {
              ಯದಿ (!parallel_files)
                ಪ್ರತಿ (q = column_vector, i = columns; i; ++q, --i)
                  q->full_page_printed = false;
              ಅನ್ಯಥಾ
                p->full_page_printed = false;
            }

          ಯದಿ ((c = getc (f)) != '\n')
            ungetc (c, f);
          hold_file (p);
          ಭಙ್ಗ;
        }
      ಅನ್ಯಥಾ ಯದಿ (c == EOF)
        {
          close_file (p);
          ಭಙ್ಗ;
        }
      c = getc (f);
    }

  ಯದಿ (skip_ಎಣಿಕೆ)
    ಯದಿ ((!parallel_files || column_ಸಂಖ್ಯೆ == 1) && !single_ff)
      ++ಸಾಲು_ಎಣಿಕೆ;
}

/* If we're tabifying output,

   When print_char encounters white space it keeps track
   of our desired horizontal position and delays printing
   until this function is called. */

ಸ್ಥಿರ ಶೂನ್ಯ
print_white_space (ಶೂನ್ಯ)
{
  ಪೂರ್ಣಾಙ್ಕ h_new;
  ಪೂರ್ಣಾಙ್ಕ h_old = output_position;
  ಪೂರ್ಣಾಙ್ಕ goal = h_old + spaces_not_printed;

  ಯಾವತ್ (goal - h_old > 1
         && (h_new = POS_AFTER_TAB (chars_per_output_tab, h_old)) <= goal)
    {
      putchar (output_tab_char);
      h_old = h_new;
    }
  ಯಾವತ್ (++h_old <= goal)
    putchar (' ');

  output_position = goal;
  spaces_not_printed = 0;
}

/* Print column separators.

   We keep a count until we know that we'll be printing a line,
   then print_sep_string() is called. */

ಸ್ಥಿರ ಶೂನ್ಯ
print_sep_string (ಶೂನ್ಯ)
{
  ವರ್ಣ ಧ್ರುವ *s = col_sep_string;
  ಪೂರ್ಣಾಙ್ಕ l = col_sep_length;

  ಯದಿ (separators_not_printed <= 0)
    {
      /* We'll be starting a line with chars_per_margin, anything else? */
      ಯದಿ (spaces_not_printed > 0)
        print_white_space ();
    }
  ಅನ್ಯಥಾ
    {
      ಪ್ರತಿ (; separators_not_printed > 0; --separators_not_printed)
        {
          ಯಾವತ್ (l-- > 0)
            {
              /* 3 types of sep_strings: spaces only, spaces and chars,
              chars only */
              ಯದಿ (*s == ' ')
                {
                  /* We're tabifying output; consecutive spaces in
                  sep_string may have to be converted to tabs */
                  s++;
                  ++spaces_not_printed;
                }
              ಅನ್ಯಥಾ
                {
                  ಯದಿ (spaces_not_printed > 0)
                    print_white_space ();
                  putchar (*s++);
                  ++output_position;
                }
            }
          /* sep_string ends with some spaces */
          ಯದಿ (spaces_not_printed > 0)
            print_white_space ();
        }
    }
}

/* Print (or store, depending on p->char_func) a clump of N
   characters. */

ಸ್ಥಿರ ಶೂನ್ಯ
print_clump (COLUMN *p, ಪೂರ್ಣಾಙ್ಕ n, ವರ್ಣ *clump)
{
  ಯಾವತ್ (n--)
    (p->char_func) (*clump++);

  ಯದಿ (ferror (stdout))
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
print_char (ವರ್ಣ c)
{
  ಯದಿ (tabify_output)
    {
      ಯದಿ (c == ' ')
        {
          ++spaces_not_printed;
          ಪ್ರತ್ಯಾಗಮ;
        }
      ಅನ್ಯಥಾ ಯದಿ (spaces_not_printed > 0)
        print_white_space ();

      /* Nonprintables are assumed to have width 0, except '\b'. */
      ಯದಿ (! isprint (to_uchar (c)))
        {
          ಯದಿ (c == '\b')
            --output_position;
        }
      ಅನ್ಯಥಾ
        ++output_position;
    }
  putchar (c);
}

/* Skip to page PAGE before printing.
   PAGE may be larger than total number of pages. */

ಸ್ಥಿರ bool
skip_to_page (uintmax_t page)
{
  ಪ್ರತಿ (uintmax_t n = 1; n < page; ++n)
    {
      COLUMN *p;
      ಪೂರ್ಣಾಙ್ಕ j;

      ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 1; i < ಸಾಲುಗಳು_per_body; ++i)
        {
          ಪ್ರತಿ (j = 1, p = column_vector; j <= columns; ++j, ++p)
            ಯದಿ (p->status == OPEN)
              skip_read (p, j);
        }
      last_ಸಾಲು = true;
      ಪ್ರತಿ (j = 1, p = column_vector; j <= columns; ++j, ++p)
        ಯದಿ (p->status == OPEN)
          skip_read (p, j);

      ಯದಿ (storing_columns)	/* change FF_FOUND to ON_HOLD */
        ಪ್ರತಿ (j = 1, p = column_vector; j <= columns; ++j, ++p)
          ಯದಿ (p->status != CLOSED)
            p->status = ON_HOLD;

      reset_status ();
      last_ಸಾಲು = false;

      ಯದಿ (files_ready_to_read < 1)
        {
          /* It's very helpful, normally the total number of pages is
             not known in advance.  */
          error (0, 0,
                 _("starting page number %ju exceeds page count %ju"),
                 page, n);
          ಭಙ್ಗ;
        }
    }
  ಪ್ರತ್ಯಾಗಮ files_ready_to_read > 0;
}

/* Print a header.

   Formfeeds are assumed to use up two lines at the beginning of
   the page. */

ಸ್ಥಿರ ಶೂನ್ಯ
print_header (ಶೂನ್ಯ)
{
  ವರ್ಣ page_text[256 + INT_STRLEN_BOUND (page_ಸಂಖ್ಯೆ)];
  ಪೂರ್ಣಾಙ್ಕ available_width;
  ಪೂರ್ಣಾಙ್ಕ lhs_spaces;
  ಪೂರ್ಣಾಙ್ಕ rhs_spaces;

  output_position = 0;
  pad_across_to (chars_per_margin);
  print_white_space ();

  ಯದಿ (page_ಸಂಖ್ಯೆ == 0)
    error (EXIT_FAILURE, 0, _("page number overflow"));

  /* The translator must ensure that formatting the translation of
     "Page %ju" does not generate more than (sizeof page_text - 1)
     bytes.  */
  sprintf (page_text, _("Page %ju"), page_ಸಂಖ್ಯೆ);
  available_width = header_width_available - mbswidth (page_text, 0);
  available_width = MAX (0, available_width);
  lhs_spaces = available_width >> 1;
  rhs_spaces = available_width - lhs_spaces;

  printf ("\n\n%*s%s%*s%s%*s%s\n\n\n",
          chars_per_margin, "",
          date_text, lhs_spaces, " ",
          file_text, rhs_spaces, " ", page_text);

  print_a_header = false;
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
read_ಸಾಲು (COLUMN *p)
{
  ಪೂರ್ಣಾಙ್ಕ c;
  ಪೂರ್ಣಾಙ್ಕ chars IF_LINT (= 0);
  ಪೂರ್ಣಾಙ್ಕ last_ಒಳಹರಿವು_position;
  ಪೂರ್ಣಾಙ್ಕ j, k;
  COLUMN *q;

  /* read 1st character in each line or any character succeeding a FF: */
  c = getc (p->fp);

  last_ಒಳಹರಿವು_position = ಒಳಹರಿವು_position;

  ಯದಿ (c == '\f' && p->full_page_printed)
    ಯದಿ ((c = getc (p->fp)) == '\n')
      c = getc (p->fp);
  p->full_page_printed = false;

  ವಿಕಲ್ಪ (c)
    {
    ಪಕ್ಷ '\f':
      ಯದಿ ((c = getc (p->fp)) != '\n')
        ungetc (c, p->fp);
      FF_only = true;
      ಯದಿ (print_a_header && !storing_columns)
        {
          pad_vertically = true;
          print_header ();
        }
      ಅನ್ಯಥಾ ಯದಿ (keep_FF)
        print_a_FF = true;
      hold_file (p);
      ಪ್ರತ್ಯಾಗಮ true;
    ಪಕ್ಷ EOF:
      close_file (p);
      ಪ್ರತ್ಯಾಗಮ true;
    ಪಕ್ಷ '\n':
      ಭಙ್ಗ;
    ಸ್ವಭಾವ:
      chars = char_to_clump (c);
    }

  ಯದಿ (truncate_ಸಾಲುಗಳು && ಒಳಹರಿವು_position > chars_per_column)
    {
      ಒಳಹರಿವು_position = last_ಒಳಹರಿವು_position;
      ಪ್ರತ್ಯಾಗಮ false;
    }

  ಯದಿ (p->char_func != store_char)
    {
      pad_vertically = true;

      ಯದಿ (print_a_header && !storing_columns)
        print_header ();

      ಯದಿ (parallel_files && align_empty_cols)
        {
          /* We have to align empty columns at the beginning of a line. */
          k = separators_not_printed;
          separators_not_printed = 0;
          ಪ್ರತಿ (j = 1, q = column_vector; j <= k; ++j, ++q)
            {
              align_column (q);
              separators_not_printed += 1;
            }
          padding_not_printed = p->start_position;
          ಯದಿ (truncate_ಸಾಲುಗಳು)
            spaces_not_printed = chars_per_column;
          ಅನ್ಯಥಾ
            spaces_not_printed = 0;
          align_empty_cols = false;
        }

      ಯದಿ (col_sep_length < padding_not_printed)
        {
          pad_across_to (padding_not_printed - col_sep_length);
          padding_not_printed = ANYWHERE;
        }

      ಯದಿ (use_col_separator)
        print_sep_string ();
    }

  ಯದಿ (p->numbered)
    add_ಸಾಲು_ಸಂಖ್ಯೆ (p);

  empty_ಸಾಲು = false;
  ಯದಿ (c == '\n')
    ಪ್ರತ್ಯಾಗಮ true;

  print_clump (p, chars, clump_buff);

  ಯಾವತ್ (true)
    {
      c = getc (p->fp);

      ವಿಕಲ್ಪ (c)
        {
        ಪಕ್ಷ '\n':
          ಪ್ರತ್ಯಾಗಮ true;
        ಪಕ್ಷ '\f':
          ಯದಿ ((c = getc (p->fp)) != '\n')
            ungetc (c, p->fp);
          ಯದಿ (keep_FF)
            print_a_FF = true;
          hold_file (p);
          ಪ್ರತ್ಯಾಗಮ true;
        ಪಕ್ಷ EOF:
          close_file (p);
          ಪ್ರತ್ಯಾಗಮ true;
        }

      last_ಒಳಹರಿವು_position = ಒಳಹರಿವು_position;
      chars = char_to_clump (c);
      ಯದಿ (truncate_ಸಾಲುಗಳು && ಒಳಹರಿವು_position > chars_per_column)
        {
          ಒಳಹರಿವು_position = last_ಒಳಹರಿವು_position;
          ಪ್ರತ್ಯಾಗಮ false;
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
print_stored (COLUMN *p)
{
  COLUMN *q;

  ಪೂರ್ಣಾಙ್ಕ ಸಾಲು = p->current_ಸಾಲು++;
  ವರ್ಣ *first = &buff[ಸಾಲು_vector[ಸಾಲು]];
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
  ವರ್ಣ *last = &buff[ಸಾಲು_vector[ಸಾಲು + 1]];

  pad_vertically = true;

  ಯದಿ (print_a_header)
    print_header ();

  ಯದಿ (p->status == FF_FOUND)
    {
      ಪೂರ್ಣಾಙ್ಕ i;
      ಪ್ರತಿ (i = 1, q = column_vector; i <= columns; ++i, ++q)
        q->status = ON_HOLD;
      ಯದಿ (column_vector->ಸಾಲುಗಳು_to_print <= 0)
        {
          ಯದಿ (!extremities)
            pad_vertically = false;
          ಪ್ರತ್ಯಾಗಮ true;		/* print a header only */
        }
    }

  ಯದಿ (col_sep_length < padding_not_printed)
    {
      pad_across_to (padding_not_printed - col_sep_length);
      padding_not_printed = ANYWHERE;
    }

  ಯದಿ (use_col_separator)
    print_sep_string ();

  ಯಾವತ್ (first != last)
    print_char (*first++);

  ಯದಿ (spaces_not_printed == 0)
    {
      output_position = p->start_position + end_vector[ಸಾಲು];
      ಯದಿ (p->start_position - col_sep_length == chars_per_margin)
        output_position -= col_sep_length;
    }

  ಪ್ರತ್ಯಾಗಮ true;
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

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
char_to_clump (ವರ್ಣ c)
{
  unsigned ವರ್ಣ uc = c;
  ವರ್ಣ *s = clump_buff;
  ವರ್ಣ esc_buff[4];
  ಪೂರ್ಣಾಙ್ಕ width;
  ಪೂರ್ಣಾಙ್ಕ chars;
  ಪೂರ್ಣಾಙ್ಕ chars_per_c = 8;

  ಯದಿ (c == ಒಳಹರಿವು_tab_char)
    chars_per_c = chars_per_ಒಳಹರಿವು_tab;

  ಯದಿ (c == ಒಳಹರಿವು_tab_char || c == '\t')
    {
      width = TAB_WIDTH (chars_per_c, ಒಳಹರಿವು_position);

      ಯದಿ (untabify_ಒಳಹರಿವು)
        {
          ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = width; i; --i)
            *s++ = ' ';
          chars = width;
        }
      ಅನ್ಯಥಾ
        {
          *s = c;
          chars = 1;
        }

    }
  ಅನ್ಯಥಾ ಯದಿ (! isprint (uc))
    {
      ಯದಿ (use_esc_sequence)
        {
          width = 4;
          chars = 4;
          *s++ = '\\';
          sprintf (esc_buff, "%03o", uc);
          ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i <= 2; ++i)
            *s++ = esc_buff[i];
        }
      ಅನ್ಯಥಾ ಯದಿ (use_cntrl_prefix)
        {
          ಯದಿ (uc < 0200)
            {
              width = 2;
              chars = 2;
              *s++ = '^';
              *s = c ^ 0100;
            }
          ಅನ್ಯಥಾ
            {
              width = 4;
              chars = 4;
              *s++ = '\\';
              sprintf (esc_buff, "%03o", uc);
              ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i <= 2; ++i)
                *s++ = esc_buff[i];
            }
        }
      ಅನ್ಯಥಾ ಯದಿ (c == '\b')
        {
          width = -1;
          chars = 1;
          *s = c;
        }
      ಅನ್ಯಥಾ
        {
          width = 0;
          chars = 1;
          *s = c;
        }
    }
  ಅನ್ಯಥಾ
    {
      width = 1;
      chars = 1;
      *s = c;
    }

  /* Too many backspaces must put us in position 0 -- never negative.  */
  ಯದಿ (width < 0 && ಒಳಹರಿವು_position == 0)
    {
      chars = 0;
      ಒಳಹರಿವು_position = 0;
    }
  ಅನ್ಯಥಾ ಯದಿ (width < 0 && ಒಳಹರಿವು_position <= -width)
    ಒಳಹರಿವು_position = 0;
  ಅನ್ಯಥಾ
    {
      ಯದಿ (ckd_add (&ಒಳಹರಿವು_position, ಒಳಹರಿವು_position, width))
        integer_overflow ();
    }

  ಪ್ರತ್ಯಾಗಮ chars;
}

/* We've just printed some files and need to clean up things before
   looking for more options and printing the next batch of files.

   Free everything we've xmalloc'ed, except 'header'. */

ಸ್ಥಿರ ಶೂನ್ಯ
cleanup (ಶೂನ್ಯ)
{
  ಮುಕ್ತ (ಸಂಖ್ಯೆ_buff);
  ಮುಕ್ತ (clump_buff);
  ಮುಕ್ತ (column_vector);
  ಮುಕ್ತ (ಸಾಲು_vector);
  ಮುಕ್ತ (end_vector);
  ಮುಕ್ತ (buff);
}

/* Complain, print a usage message, and die. */

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              program_name);

      fputs (_("\
Paginate or columnate FILE(s) ಪ್ರತಿ printing.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\n\
  +FIRST_PAGE[:LAST_PAGE], --pages=FIRST_PAGE[:LAST_PAGE]\n\
         begin [stop] printing with page FIRST_[LAST_]PAGE\n\
"));
      oputs (_("\
  -COLS, --columns=COLS\n\
         output COLS columns and print columns down, unless -a is used.\n\
         Balance ಸಂಖ್ಯೆ of ಸಾಲುಗಳು in the columns on each page\n\
"));
      oputs (_("\
  -a, --across\n\
         print columns across rather than down, used together with -COLS\n\
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
         use FORMAT ಪ್ರತಿ the header date\n\
"));
      oputs (_("\
  -e[CHAR[WIDTH]], --expand-tabs[=CHAR[WIDTH]]\n\
         expand ಒಳಹರಿವು CHARs (TABs) to tab WIDTH (8)\n\
"));
      oputs (_("\
  -F, -f, --form-feed\n\
         use form feeds instead of newlines to separate pages\n\
         (by a 3-ಸಾಲು page header with -F or a 5-ಸಾಲು header\n\
         and trailer without -F)\n\
"));
      oputs (_("\
  -h, --header=HEADER\n\
         use a centered HEADER instead of filename in page header,\n\
         -h \"\" prints a blank ಸಾಲು, don't use -h\"\"\n\
"));
      oputs (_("\
  -i[CHAR[WIDTH]], --output-tabs[=CHAR[WIDTH]]\n\
         replace spaces with CHARs (TABs) to tab WIDTH (8)\n\
"));
      oputs (_("\
  -J, --join-ಸಾಲುಗಳು\n\
         merge full ಸಾಲುಗಳು, turns off -W ಸಾಲು truncation,\n\
         no column alignment, --sep-string[=STRING] sets separators\n\
"));
      oputs (_("\
  -l, --length=PAGE_LENGTH\n\
         set the page length to PAGE_LENGTH (66) ಸಾಲುಗಳು\n\
         (ಸ್ವಭಾವ ಸಂಖ್ಯೆ of ಸಾಲುಗಳು of text 56, and with -F 63).\n\
         implies -t ಯದಿ PAGE_LENGTH <= 10\n\
"));
      oputs (_("\
  -m, --merge\n\
         print all files in parallel, one in each column,\n\
         truncate ಸಾಲುಗಳು, but join ಸಾಲುಗಳು of full length with -J\n\
"));
      oputs (_("\
  -n[SEP[DIGITS]], --ಸಂಖ್ಯೆ-ಸಾಲುಗಳು[=SEP[DIGITS]]\n\
         ಸಂಖ್ಯೆ ಸಾಲುಗಳು, use DIGITS (5) digits, then SEP (TAB),\n\
         ಸ್ವಭಾವ counting starts with 1st ಸಾಲು of ಒಳಹರಿವು file\n\
"));
      oputs (_("\
  -N, --first-ಸಾಲು-ಸಂಖ್ಯೆ=ಸಂಖ್ಯೆ_2\n\
         start counting with ಸಂಖ್ಯೆ_2 at 1st ಸಾಲು of first\n\
         page printed (see +FIRST_PAGE)\n\
"));
      oputs (_("\
  -o, --indent=MARGIN\n\
         offset each ಸಾಲು with MARGIN (zero) spaces, ಕುರು not\n\
         affect -w or -W, MARGIN will be added to PAGE_WIDTH\n\
"));
      oputs (_("\
  -r, --no-file-warnings\n\
         omit warning when a file cannot be opened\n\
"));
      oputs (_("\
  -s[CHAR], --separator[=CHAR]\n\
         separate columns by a single character, ಸ್ವಭಾವ ಪ್ರತಿ CHAR\n\
         is the <TAB> character without -w and \'no ವರ್ಣ\' with -w.\n\
         -s[CHAR] turns off ಸಾಲು truncation of all 3 column\n\
         options (-COLS|-a -COLS|-m) except -w is set\n\
"));
      oputs (_("\
  -S[STRING], --sep-string[=STRING]\n\
         separate columns by STRING,\n\
         without -S: Default separator <TAB> with -J and <space>\n\
         otherwise (same as -S\" \"), no effect on column options\n\
"));
      oputs (_("\
  -t, --omit-header\n\
         omit page headers and trailers; implied ಯದಿ PAGE_LENGTH <= 10\n\
"));
      oputs (_("\
  -T, --omit-pagination\n\
         omit page headers and trailers,\n\
         eliminate any pagination by form feeds set in ಒಳಹರಿವು files\n\
"));
      oputs (_("\
  -v, --show-nonprinting\n\
         use octal backslash notation\n\
"));
      oputs (_("\
  -w, --width=PAGE_WIDTH\n\
         set page width to PAGE_WIDTH (72) characters ಪ್ರತಿ\n\
         multiple text-column output only, -s[ವರ್ಣ] turns off (72)\n\
"));
      oputs (_("\
  -W, --page-width=PAGE_WIDTH\n\
         set page width to PAGE_WIDTH (72) characters always,\n\
         truncate ಸಾಲುಗಳು, except -J option is set,\n\
         no interference with -S or -s\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}
