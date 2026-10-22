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

#अन्तर्भाव <config.h>

#अन्तर्भाव <getopt.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव "system.h"
#अन्तर्भाव "c-ctype.h"
#अन्तर्भाव "fadvise.h"
#अन्तर्भाव "hard-locale.h"
#अन्तर्भाव "mbswidth.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "stat-time.h"
#अन्तर्भाव "stdio--.h"
#अन्तर्भाव "strftime.h"
#अन्तर्भाव "xstrtol.h"
#अन्तर्भाव "xstrtol-error.h"
#अन्तर्भाव "xdectoint.h"

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

संरचना COLUMN;
संरचना COLUMN
  {
    FILE *fp;			/* Input stream for this column. */
    वर्ण ध्रुव *name;		/* File name. */
    गणना
      {
        OPEN,
        FF_FOUND,		/* used with -b option, set with \f, changed
                                   to ON_HOLD after print_header */
        ON_HOLD,		/* Hit a form feed. */
        CLOSED
      }
    status;			/* Status of the file pointer. */

    /* Func to print lines in this col. */
    bool (*print_func) (संरचना COLUMN *);

    /* Func to print/store chars in this col. */
    शून्य (*char_func) (वर्ण);

    पूर्णाङ्क current_पङ्क्तिः;		/* Index of current place in line_vector. */
    पूर्णाङ्क पङ्क्तयः_stored;		/* Number of lines stored in buff. */
    पूर्णाङ्क पङ्क्तयः_to_print;		/* No. lines stored or space left on page. */
    पूर्णाङ्क start_position;		/* Horizontal position of first char. */
    bool numbered;
    bool full_page_printed;	/* True means printed without a FF found. */

    /* p->full_page_printed  controls a special case of "FF set by hand":
       True means a full page has been printed without FF found. To avoid an
       additional empty page we have to ignore a FF immediately following in
       the next line. */
  };

प्रकारनिर्देश संरचना COLUMN COLUMN;

स्थिर पूर्णाङ्क char_to_clump (वर्ण c);
स्थिर bool read_पङ्क्तिः (COLUMN *p);
स्थिर bool print_page (शून्य);
स्थिर bool print_stored (COLUMN *p);
स्थिर bool open_file (वर्ण *name, COLUMN *p);
स्थिर bool skip_to_page (uintmax_t page);
स्थिर शून्य print_header (शून्य);
स्थिर शून्य pad_across_to (पूर्णाङ्क position);
स्थिर शून्य add_पङ्क्तिः_सङ्ख्या (COLUMN *p);
स्थिर पूर्णाङ्क getoptnum (वर्ण ध्रुव *n_str, पूर्णाङ्क min, वर्ण ध्रुव *errfmt);
स्थिर शून्य getoptarg (वर्ण *arg, वर्ण switch_char, वर्ण *character,
                       पूर्णाङ्क *सङ्ख्या);
स्थिर शून्य print_files (पूर्णाङ्क सङ्ख्या_of_files, वर्ण **av);
स्थिर शून्य init_parameters (पूर्णाङ्क सङ्ख्या_of_files);
स्थिर शून्य init_header (वर्ण ध्रुव *filename, पूर्णाङ्क desc);
स्थिर bool init_fps (पूर्णाङ्क सङ्ख्या_of_files, वर्ण **av);
स्थिर शून्य init_funcs (शून्य);
स्थिर शून्य init_store_cols (शून्य);
स्थिर शून्य store_columns (शून्य);
स्थिर शून्य balance (पूर्णाङ्क total_stored);
स्थिर शून्य store_char (वर्ण c);
स्थिर शून्य pad_down (unsigned पूर्णाङ्क पङ्क्तयः);
स्थिर शून्य read_rest_of_पङ्क्तिः (COLUMN *p);
स्थिर शून्य skip_read (COLUMN *p, पूर्णाङ्क column_सङ्ख्या);
स्थिर शून्य print_char (वर्ण c);
स्थिर शून्य cleanup (शून्य);
स्थिर शून्य print_sep_string (शून्य);
स्थिर शून्य separator_string (वर्ण ध्रुव *optarg_S);

/* All of the columns to print.  */
स्थिर COLUMN *column_vector;

/* When printing a single file in multiple downward columns,
   we store the leftmost columns contiguously in buff.
   To print a line from buff, get the index of the first character
   from line_vector[i], and print up to line_vector[i + 1]. */
स्थिर वर्ण *buff;

/* Index of the position in buff where the next character
   will be stored. */
स्थिर unsigned पूर्णाङ्क buff_current;

/* The number of characters in buff.
   Used for allocation of buff and to detect overflow of buff. */
स्थिर idx_t buff_allocated;

/* Array of indices into buff.
   Each entry is an index of the first character of a line.
   This is used when storing lines to facilitate shuffling when
   we do column balancing on the last page. */
स्थिर पूर्णाङ्क *पङ्क्तिः_vector;

/* Array of horizontal positions.
   For each line in line_vector, end_vector[line] is the horizontal
   position we are in after printing that line.  We keep track of this
   so that we know how much we need to pad to prepare for the next
   column. */
स्थिर पूर्णाङ्क *end_vector;

/* (-m) True means we're printing multiple files in parallel. */
स्थिर bool parallel_files = false;

/* (-m) True means a line starts with some empty columns (some files
   already CLOSED or ON_HOLD) which we have to align. */
स्थिर bool align_empty_cols;

/* (-m) True means we have not yet found any printable column in a line.
   align_empty_cols = true  has to be maintained. */
स्थिर bool empty_पङ्क्तिः;

/* (-m) False means printable column output precedes a form feed found.
   Column alignment is done only once. No additional action with that form
   feed.
   True means we found only a form feed in a column. Maybe we have to do
   some column alignment with that form feed. */
स्थिर bool FF_only;

/* (-[0-9]+) True means we're given an option explicitly specifying
   number of columns.  Used to detect when this option is used with -m
   and when translating old options to new/long options. */
स्थिर bool explicit_columns = false;

/* (-t|-T) False means we aren't printing headers and footers. */
स्थिर bool extremities = true;

/* (-t) True means we retain all FF set by hand in input files.
   False is set with -T option. */
स्थिर bool keep_FF = false;
स्थिर bool print_a_FF = false;

/* True means we need to print a header as soon as we know we've got input
   to print after it. */
स्थिर bool print_a_header;

/* (-f) True means use formfeeds instead of newlines to separate pages. */
स्थिर bool use_form_feed = false;

/* True means we have read the standard input. */
स्थिर bool have_read_stdin = false;

/* True means the -a flag has been given. */
स्थिर bool print_across_flag = false;

/* True means we're printing one file in multiple (>1) downward columns. */
स्थिर bool storing_columns = true;

/* (-b) True means balance columns on the last page as Sys V does. */
/* That's no longer an independent option. With storing_columns = true
   balance_columns = true is used too (s. function init_parameters).
   We get a consistent formulation with "FF set by hand" in input files. */
स्थिर bool balance_columns = false;

/* (-l) Number of lines on a page, including header and footer lines. */
स्थिर पूर्णाङ्क पङ्क्तयः_per_page = 66;

/* Number of lines in the header and footer can be reset to 0 using
   the -t flag. */
गणना { पङ्क्तयः_per_header = 5 };
स्थिर पूर्णाङ्क पङ्क्तयः_per_body;
गणना { पङ्क्तयः_per_footer = 5 };

/* (-w|-W) Width in characters of the page.  Does not include the width of
   the margin. */
स्थिर पूर्णाङ्क chars_per_पङ्क्तिः = 72;

/* (-w|W) True means we truncate lines longer than chars_per_column. */
स्थिर bool truncate_पङ्क्तयः = false;

/* (-J) True means we join lines without any line truncation. -J
   dominates -w option. */
स्थिर bool join_पङ्क्तयः = false;

/* Number of characters in a column.  Based on col_sep_length and
   page width. */
स्थिर पूर्णाङ्क chars_per_column;

/* (-e) True means convert tabs to spaces on input. */
स्थिर bool untabify_निवेशः = false;

/* (-e) The input tab character. */
स्थिर वर्ण निवेशः_tab_char = '\t';

/* (-e) Tabstops are at chars_per_tab, 2*chars_per_tab, 3*chars_per_tab, ...
   where the leftmost column is 1. */
स्थिर पूर्णाङ्क chars_per_निवेशः_tab = 8;

/* (-i) True means convert spaces to tabs on output. */
स्थिर bool tabify_output = false;

/* (-i) The output tab character. */
स्थिर वर्ण output_tab_char = '\t';

/* (-i) The width of the output tab. */
स्थिर पूर्णाङ्क chars_per_output_tab = 8;

/* Keeps track of pending white space.  When we hit a nonspace
   character after some whitespace, we print whitespace, tabbing
   if necessary to get to output_position + spaces_not_printed. */
स्थिर पूर्णाङ्क spaces_not_printed;

/* (-o) Number of spaces in the left margin (tabs used when possible). */
स्थिर पूर्णाङ्क chars_per_margin = 0;

/* Position where the next character will fall.
   Leftmost position is 0 + chars_per_margin.
   Rightmost position is chars_per_margin + chars_per_line - 1.
   This is important for converting spaces to tabs on output. */
स्थिर पूर्णाङ्क output_position;

/* Horizontal position relative to the current file.
   (output_position depends on where we are on the page;
   input_position depends on where we are in the file.)
   Important for converting tabs to spaces on input. */
स्थिर पूर्णाङ्क निवेशः_position;

/* True if there were any failed opens so we can exit with nonzero
   status.  */
स्थिर bool failed_opens = false;

/* The number of spaces taken up if we print a tab character with width
   c_ from position h_. */
#define TAB_WIDTH(c_, h_) ((c_) - ((h_) % (c_)))

/* The horizontal position we'll be at after printing a tab character
   of width c_ from the position h_. */
#define POS_AFTER_TAB(c_, h_) ((h_) + TAB_WIDTH (c_, h_))

/* (-NNN) Number of columns of text to print. */
स्थिर पूर्णाङ्क columns = 1;

/* (+NNN:MMM) Page numbers on which to begin and stop printing.
   first_page_number = 0  will be used to check input only. */
स्थिर uintmax_t first_page_सङ्ख्या = 0;
स्थिर uintmax_t last_page_सङ्ख्या = UINTMAX_MAX;

/* Number of files open (not closed, not on hold). */
स्थिर पूर्णाङ्क files_ready_to_read = 0;

/* Current page number.  Displayed in header. */
स्थिर uintmax_t page_सङ्ख्या;

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
स्थिर पूर्णाङ्क पङ्क्तिः_सङ्ख्या;

/* (-n) True means lines should be preceded by numbers. */
स्थिर bool numbered_पङ्क्तयः = false;

/* (-n) Character which follows each line number. */
स्थिर वर्ण सङ्ख्या_separator = '\t';

/* (-n) line counting starts with 1st line of input file (not with 1st
   line of 1st page printed). */
स्थिर पूर्णाङ्क पङ्क्तिः_गणनम् = 1;

/* (-n) True means counting of skipped lines starts with 1st line of
   input file. False means -N option is used in addition, counting of
   skipped lines not required. */
स्थिर bool skip_गणनम् = true;

/* (-N) Counting starts with start_line_number = NUMBER at 1st line of
   first page printed, usually not 1st page of input file. */
स्थिर पूर्णाङ्क start_पङ्क्तिः_अङ्कः = 1;

/* (-n) Width in characters of a line number. */
स्थिर पूर्णाङ्क chars_per_सङ्ख्या = 5;

/* Used when widening the first column to accommodate numbers -- only
   needed when printing files in parallel.  Includes width of both the
   number and the number_separator. */
स्थिर पूर्णाङ्क सङ्ख्या_width;

/* Buffer sprintf uses to format a line number. */
स्थिर वर्ण *सङ्ख्या_buff;

/* (-v) True means nonprintable characters are printed as escape sequences.
   control-g becomes \007. */
स्थिर bool use_esc_sequence = false;

/* (-c) True means nonprintable characters are printed as control prefixes.
   control-g becomes ^G. */
स्थिर bool use_cntrl_prefix = false;

/* (-d) True means output is double spaced. */
स्थिर bool double_space = false;

/* Number of files opened initially in init_files.  Should be 1
   unless we're printing multiple files in parallel. */
स्थिर पूर्णाङ्क total_files = 0;

/* (-r) True means don't complain if we can't open a file. */
स्थिर bool ignore_failed_opens = false;

/* (-S) True means we separate columns with a specified string.
   -S option does not affect line truncation nor column alignment. */
स्थिर bool use_col_separator = false;

/* String used to separate columns if the -S option has been specified.
   Default without -S but together with one of the column options
   -a|COLUMN|-m is a 'space' and with the -J option a 'tab'. */
स्थिर वर्ण ध्रुव *col_sep_string = "";
स्थिर पूर्णाङ्क col_sep_length = 0;
स्थिर वर्ण *column_separator = (वर्ण *) " ";
स्थिर वर्ण *पङ्क्तिः_separator = (वर्ण *) "\t";

/* Number of separator characters waiting to be printed as soon as we
   know that we have any input remaining to be printed. */
स्थिर पूर्णाङ्क separators_not_printed;

/* Position we need to pad to, as soon as we know that we have input
   remaining to be printed. */
स्थिर पूर्णाङ्क padding_not_printed;

/* True means we should pad the end of the page.  Remains false until we
   know we have a page to print. */
स्थिर bool pad_vertically;

/* (-h) String of characters used in place of the filename in the header. */
स्थिर वर्ण *custom_header;

/* (-D) Date format for the header.  */
स्थिर वर्ण ध्रुव *date_format;

/* The local time zone rules, as per the TZ environment variable.  */
स्थिर timezone_t localtz;

/* Date and file name for the header.  */
स्थिर वर्ण *date_text;
स्थिर वर्ण ध्रुव *file_text;

/* Output columns available, not counting the date and file name.  */
स्थिर पूर्णाङ्क header_width_available;

स्थिर वर्ण *clump_buff;

/* True means we read the line no. lines_per_body in skip_read
   called by skip_to_page. That variable controls the coincidence of a
   "FF set by hand" and "full_page_printed", see above the definition of
   structure COLUMN. */
स्थिर bool last_पङ्क्तिः = false;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
गणना
{
  COLUMNS_OPTION = CHAR_MAX + 1,
  PAGES_OPTION
};

स्थिर वर्ण ध्रुव short_options[] =
  "-0123456789D:FJN:S::TW:abcde::fh:i::l:mn::o:rs::tvw:";

स्थिर संरचना option ध्रुव long_options[] =
{
  {"pages", required_argument, रिक्त, PAGES_OPTION},
  {"columns", required_argument, रिक्त, COLUMNS_OPTION},
  {"across", no_argument, रिक्त, 'a'},
  {"show-control-chars", no_argument, रिक्त, 'c'},
  {"double-space", no_argument, रिक्त, 'd'},
  {"date-format", required_argument, रिक्त, 'D'},
  {"expand-tabs", optional_argument, रिक्त, 'e'},
  {"form-feed", no_argument, रिक्त, 'f'},
  {"header", required_argument, रिक्त, 'h'},
  {"output-tabs", optional_argument, रिक्त, 'i'},
  {"join-lines", no_argument, रिक्त, 'J'},
  {"length", required_argument, रिक्त, 'l'},
  {"merge", no_argument, रिक्त, 'm'},
  {"number-lines", optional_argument, रिक्त, 'n'},
  {"first-line-number", required_argument, रिक्त, 'N'},
  {"indent", required_argument, रिक्त, 'o'},
  {"no-file-warnings", no_argument, रिक्त, 'r'},
  {"separator", optional_argument, रिक्त, 's'},
  {"sep-string", optional_argument, रिक्त, 'S'},
  {"omit-header", no_argument, रिक्त, 't'},
  {"omit-pagination", no_argument, रिक्त, 'T'},
  {"show-nonprinting", no_argument, रिक्त, 'v'},
  {"width", required_argument, रिक्त, 'w'},
  {"page-width", required_argument, रिक्त, 'W'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

स्थिर _Noreturn शून्य
integer_overflow (शून्य)
{
  error (EXIT_FAILURE, 0, _("integer overflow"));
}

/* Return the number of columns that have either an open file or
   stored lines. */

ATTRIBUTE_PURE
स्थिर unsigned पूर्णाङ्क
cols_ready_to_print (शून्य)
{
  COLUMN *q;
  unsigned पूर्णाङ्क i;
  unsigned पूर्णाङ्क n;

  n = 0;
  प्रति (q = column_vector, i = 0; i < columns; ++q, ++i)
    जइ (q->status == OPEN
        || q->status == FF_FOUND	/* With -b: To print a header only */
        || (storing_columns && q->पङ्क्तयः_stored > 0 && q->पङ्क्तयः_to_print > 0))
      ++n;
  प्रत्यागम n;
}

/* Estimate first_ / last_page_number
   using option +FIRST_PAGE:LAST_PAGE */

स्थिर bool
first_last_page (पूर्णाङ्क oi, वर्ण c, वर्ण ध्रुव *pages)
{
  वर्ण *p;
  uintmax_t first;
  uintmax_t last = UINTMAX_MAX;
  strtol_error err = xstrtoumax (pages, &p, 10, &first, "");
  जइ (err != LONGINT_OK && err != LONGINT_INVALID_SUFFIX_CHAR)
    xstrtol_fatal (err, oi, c, long_options, pages);

  जइ (p == pages || !first)
    प्रत्यागम false;

  जइ (*p == ':')
    {
      वर्ण ध्रुव *p1 = p + 1;
      err = xstrtoumax (p1, &p, 10, &last, "");
      जइ (err != LONGINT_OK)
        xstrtol_fatal (err, oi, c, long_options, pages);
      जइ (p1 == p || last < first)
        प्रत्यागम false;
    }

  जइ (*p)
    प्रत्यागम false;

  first_page_सङ्ख्या = first;
  last_page_सङ्ख्या = last;
  प्रत्यागम true;
}

/* Parse column count string S, and if it's valid (1 or larger and
   within range of the type of 'columns') set the global variables
   columns and explicit_columns.  Otherwise, exit with a diagnostic.  */

स्थिर शून्य
parse_column_गणनम् (वर्ण ध्रुव *s)
{
  columns = getoptnum (s, 1, _("invalid number of columns"));
  explicit_columns = true;
}

/* Estimate length of col_sep_string with option -S.  */

स्थिर शून्य
separator_string (वर्ण ध्रुव *optarg_S)
{
  size_t len = strlen (optarg_S);
  जइ (INT_MAX < len)
    integer_overflow ();
  col_sep_length = len;
  col_sep_string = optarg_S;
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  unsigned पूर्णाङ्क n_files;
  bool old_options = false;
  bool old_w = false;
  bool old_s = false;
  वर्ण **file_names;

  /* Accumulate the digits of old-style options like -99.  */
  वर्ण *column_गणनम्_string = रिक्त;
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
                ? xnmalloc (argc - 1, परिमाण (वर्ण *))
                : रिक्त);

  जाव (true)
    {
      पूर्णाङ्क oi = -1;
      पूर्णाङ्क c = getopt_long (argc, argv, short_options, long_options, &oi);
      जइ (c == -1)
        भङ्ग;

      जइ (c_isdigit (c))
        {
          /* Accumulate column-count digits specified via old-style options. */
          जइ (n_digits + 1 >= n_alloc)
            column_गणनम्_string = xpalloc (column_गणनम्_string, &n_alloc, 2, -1,
                                           परिमाण *column_गणनम्_string);
          column_गणनम्_string[n_digits++] = c;
          column_गणनम्_string[n_digits] = '\0';
          अनुवर्त;
        }

      n_digits = 0;

      विकल्प (c)
        {
        पक्ष 1:			/* Non-option argument. */
          /* long option --page dominates old '+FIRST_PAGE ...'.  */
          जइ (! (first_page_सङ्ख्या == 0
                 && *optarg == '+' && first_last_page (-2, '+', optarg + 1)))
            file_names[n_files++] = optarg;
          भङ्ग;

        पक्ष PAGES_OPTION:	/* --pages=FIRST_PAGE[:LAST_PAGE] */
          {			/* dominates old opt +... */
            जइ (! optarg)
              error (EXIT_FAILURE, 0,
                     _("'--pages=FIRST_PAGE[:LAST_PAGE]' missing argument"));
            अण्णहा जइ (! first_last_page (oi, 0, optarg))
              error (EXIT_FAILURE, 0, _("invalid page range %s"),
                     quote (optarg));
            भङ्ग;
          }

        पक्ष COLUMNS_OPTION:	/* --columns=COLS */
          {
            parse_column_गणनम् (optarg);

            /* If there was a prior column count specified via the
               short-named option syntax, e.g., -9, ensure that this
               long-name-specified value overrides it.  */
            मुक्त (column_गणनम्_string);
            column_गणनम्_string = रिक्त;
            n_alloc = 0;
            भङ्ग;
          }

        पक्ष 'a':
          print_across_flag = true;
          storing_columns = false;
          भङ्ग;
        पक्ष 'b':
          balance_columns = true;
          भङ्ग;
        पक्ष 'c':
          use_cntrl_prefix = true;
          भङ्ग;
        पक्ष 'd':
          double_space = true;
          भङ्ग;
        पक्ष 'D':
          date_format = optarg;
          भङ्ग;
        पक्ष 'e':
          जइ (optarg)
            getoptarg (optarg, 'e', &निवेशः_tab_char,
                       &chars_per_निवेशः_tab);
          /* Could check tab width > 0. */
          untabify_निवेशः = true;
          भङ्ग;
        पक्ष 'f':
        पक्ष 'F':
          use_form_feed = true;
          भङ्ग;
        पक्ष 'h':
          custom_header = optarg;
          भङ्ग;
        पक्ष 'i':
          जइ (optarg)
            getoptarg (optarg, 'i', &output_tab_char,
                       &chars_per_output_tab);
          /* Could check tab width > 0. */
          tabify_output = true;
          भङ्ग;
        पक्ष 'J':
          join_पङ्क्तयः = true;
          भङ्ग;
        पक्ष 'l':
          पङ्क्तयः_per_page
            = getoptnum (optarg, 1,
                         _("'-l PAGE_LENGTH' invalid number of lines"));
          भङ्ग;
        पक्ष 'm':
          parallel_files = true;
          storing_columns = false;
          भङ्ग;
        पक्ष 'n':
          numbered_पङ्क्तयः = true;
          जइ (optarg)
            getoptarg (optarg, 'n', &सङ्ख्या_separator,
                       &chars_per_सङ्ख्या);
          भङ्ग;
        पक्ष 'N':
          skip_गणनम् = false;
          start_पङ्क्तिः_अङ्कः
            = getoptnum (optarg, INT_MIN,
                         _("'-N NUMBER' invalid starting line number"));
          भङ्ग;
        पक्ष 'o':
          chars_per_margin = getoptnum (optarg, 0,
                                        _("'-o MARGIN' invalid line offset"));
          भङ्ग;
        पक्ष 'r':
          ignore_failed_opens = true;
          भङ्ग;
        पक्ष 's':
          old_options = true;
          old_s = true;
          जइ (!use_col_separator && optarg)
            separator_string (optarg);
          भङ्ग;
        पक्ष 'S':
          old_s = false;
          /* Reset an additional input of -s, -S dominates -s */
          col_sep_string = "";
          col_sep_length = 0;
          use_col_separator = true;
          जइ (optarg)
            separator_string (optarg);
          भङ्ग;
        पक्ष 't':
          extremities = false;
          keep_FF = true;
          भङ्ग;
        पक्ष 'T':
          extremities = false;
          keep_FF = false;
          भङ्ग;
        पक्ष 'v':
          use_esc_sequence = true;
          भङ्ग;
        पक्ष 'w':
          old_options = true;
          old_w = true;
          {
            पूर्णाङ्क tmp_cpl
              = getoptnum (optarg, 1,
                           _("'-w PAGE_WIDTH' invalid number of characters"));
            जइ (! truncate_पङ्क्तयः)
              chars_per_पङ्क्तिः = tmp_cpl;
          }
          भङ्ग;
        पक्ष 'W':
          old_w = false;			/* dominates -w */
          truncate_पङ्क्तयः = true;
          chars_per_पङ्क्तिः
            = getoptnum (optarg, 1,
                         _("'-W PAGE_WIDTH' invalid number of characters"));
          भङ्ग;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        स्वभाव:
          usage (EXIT_FAILURE);
          भङ्ग;
        }
    }

  जइ (column_गणनम्_string)
    {
      parse_column_गणनम् (column_गणनम्_string);
      मुक्त (column_गणनम्_string);
    }

  जइ (! date_format)
    date_format = (getenv ("POSIXLY_CORRECT") && !hard_locale (LC_TIME)
                   ? "%b %e %H:%M %Y"
                   : "%Y-%m-%d %H:%M");

  localtz = tzalloc (getenv ("TZ"));

  /* Now we can set a reasonable initial value: */
  जइ (first_page_सङ्ख्या == 0)
    first_page_सङ्ख्या = 1;

  जइ (parallel_files && explicit_columns)
    error (EXIT_FAILURE, 0,
           _("cannot specify number of columns when printing in parallel"));

  जइ (parallel_files && print_across_flag)
    error (EXIT_FAILURE, 0,
           _("cannot specify both printing across and printing in parallel"));

/* Translate some old short options to new/long options.
   To meet downward compatibility with other UNIX pr utilities
   and some POSIX specifications. */

  जइ (old_options)
    {
      जइ (old_w)
        {
          जइ (parallel_files || explicit_columns)
            {
              /* activate -W */
              truncate_पङ्क्तयः = true;
              जइ (old_s)
                /* adapt HP-UX and SunOS: -s = no separator;
                   activate -S */
                use_col_separator = true;
            }
          अण्णहा
            /* old -w sets width with columns only
               activate -J */
            join_पङ्क्तयः = true;
        }
      अण्णहा जइ (!use_col_separator)
        {
          /* No -S option read */
          जइ (old_s && (parallel_files || explicit_columns))
            {
              जइ (!truncate_पङ्क्तयः)
                {
                  /* old -s (without -w and -W) annuls column alignment,
                  uses fields, activate -J */
                  join_पङ्क्तयः = true;
                  जइ (col_sep_length > 0)
                    /* activate -S */
                    use_col_separator = true;
                }
              अण्णहा
                /* with -W */
                /* adapt HP-UX and SunOS: -s = no separator;
                   activate -S */
                use_col_separator = true;
            }
        }
    }

  प्रति (; optind < argc; optind++)
    {
      file_names[n_files++] = argv[optind];
    }

  जइ (n_files == 0)
    {
      /* No file arguments specified;  read from standard input.  */
      print_files (0, रिक्त);
    }
  अण्णहा
    {
      जइ (parallel_files)
        print_files (n_files, file_names);
      अण्णहा
        {
          प्रति (unsigned पूर्णाङ्क i = 0; i < n_files; i++)
            print_files (1, &file_names[i]);
        }
    }

  cleanup ();

  जइ (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, _("standard input"));
  main_exit (failed_opens ? EXIT_FAILURE : EXIT_SUCCESS);
}

/* Parse numeric arguments, ensuring MIN <= number <= INT_MAX.  */

स्थिर पूर्णाङ्क
getoptnum (वर्ण ध्रुव *n_str, पूर्णाङ्क min, वर्ण ध्रुव *err)
{
  प्रत्यागम xnumtoimax (n_str, 10, min, INT_MAX, "", err, 0,
                     min <= 0 ? 0 : XTOINT_MIN_RANGE);
}

/* Parse options of the form -scNNN.

   Example: -nck, where 'n' is the option, c is the optional number
   separator, and k is the optional width of the field used when printing
   a number. */

स्थिर शून्य
getoptarg (वर्ण *arg, वर्ण switch_char, वर्ण *character, पूर्णाङ्क *सङ्ख्या)
{
  वर्ण ध्रुव *अङ्कः = arg;

  जइ (!*अङ्कः)
    {
      error (0, 0, _("'-%c': Invalid argument: %s"), switch_char, quote (अङ्कः));
      usage (EXIT_FAILURE);
    }

  जइ (!c_isdigit (*अङ्कः))
    *character = *अङ्कः++;
  जइ (*अङ्कः)
    {
      long पूर्णाङ्क tmp_long;
      strtol_error e = xstrtol (अङ्कः, रिक्त, 10, &tmp_long, "");
      जइ (e == LONGINT_OK)
        {
          जइ (tmp_long <= 0)
            e = LONGINT_INVALID;
          अण्णहा जइ (INT_MAX < tmp_long)
            e = LONGINT_OVERFLOW;
        }
      जइ (e != LONGINT_OK)
        {
          error (0, e & LONGINT_OVERFLOW ? EOVERFLOW : 0,
             _("'-%c' extra characters or invalid number in the argument: %s"),
                 switch_char, quote (arg));
          usage (EXIT_FAILURE);
        }
      *सङ्ख्या = tmp_long;
    }
}

/* Set parameters related to formatting. */

स्थिर शून्य
init_parameters (पूर्णाङ्क सङ्ख्या_of_files)
{
  पूर्णाङ्क chars_used_by_सङ्ख्या = 0;

  पङ्क्तयः_per_body = पङ्क्तयः_per_page - पङ्क्तयः_per_header - पङ्क्तयः_per_footer;
  जइ (पङ्क्तयः_per_body <= 0)
    {
      extremities = false;
      keep_FF = true;
    }
  जइ (! extremities)
    पङ्क्तयः_per_body = पङ्क्तयः_per_page;

  जइ (double_space)
    पङ्क्तयः_per_body = MAX (1, पङ्क्तयः_per_body / 2);

  /* If input is stdin, cannot print parallel files.  BSD dumps core
     on this. */
  जइ (सङ्ख्या_of_files == 0)
    parallel_files = false;

  जइ (parallel_files)
    columns = सङ्ख्या_of_files;

  /* One file, multi columns down: -b option is set to get a consistent
     formulation with "FF set by hand" in input files. */
  जइ (storing_columns)
    balance_columns = true;

  /* Tabification is assumed for multiple columns. */
  जइ (columns > 1)
    {
      जइ (!use_col_separator)
        {
          /* Use default separator */
          जइ (join_पङ्क्तयः)
            col_sep_string = पङ्क्तिः_separator;
          अण्णहा
            col_sep_string = column_separator;

          col_sep_length = 1;
          use_col_separator = true;
        }
      /* It's rather pointless to define a TAB separator with column
         alignment */
      अण्णहा जइ (!join_पङ्क्तयः && col_sep_length == 1 && *col_sep_string == '\t')
        col_sep_string = column_separator;

      truncate_पङ्क्तयः = true;
      जइ (! (col_sep_length == 1 && *col_sep_string == '\t'))
        untabify_निवेशः = true;
      tabify_output = true;
    }
  अण्णहा
    storing_columns = false;

  /* -J dominates -w in any case */
  जइ (join_पङ्क्तयः)
    truncate_पङ्क्तयः = false;

  जइ (numbered_पङ्क्तयः)
    {
      पूर्णाङ्क chars_per_default_tab = 8;

      पङ्क्तिः_गणनम् = start_पङ्क्तिः_अङ्कः;

      /* To allow input tab-expansion (-e sensitive) use:
         if (number_separator == input_tab_char)
           number_width = chars_per_number
             + TAB_WIDTH (chars_per_input_tab, chars_per_number);   */

      /* Estimate chars_per_text without any margin and keep it constant. */
      जइ (सङ्ख्या_separator == '\t')
        सङ्ख्या_width = (chars_per_सङ्ख्या
                        + TAB_WIDTH (chars_per_default_tab, chars_per_सङ्ख्या));
      अण्णहा
        सङ्ख्या_width = chars_per_सङ्ख्या + 1;

      /* The number is part of the column width unless we are
         printing files in parallel. */
      जइ (parallel_files)
        chars_used_by_सङ्ख्या = सङ्ख्या_width;
    }

  पूर्णाङ्क sep_chars, useful_chars;
  जइ (ckd_mul (&sep_chars, columns - 1, col_sep_length))
    sep_chars = INT_MAX;
  जइ (ckd_sub (&useful_chars, chars_per_पङ्क्तिः - chars_used_by_सङ्ख्या,
               sep_chars))
    useful_chars = 0;
  chars_per_column = useful_chars / columns;

  जइ (chars_per_column < 1)
    error (EXIT_FAILURE, 0, _("page width too narrow"));

  जइ (numbered_पङ्क्तयः)
    {
      मुक्त (सङ्ख्या_buff);
      सङ्ख्या_buff = xmalloc (MAX (chars_per_सङ्ख्या,
                                  INT_STRLEN_BOUND (पङ्क्तिः_सङ्ख्या)) + 1);
    }

  /* Pick the maximum between the tab width and the width of an
     escape sequence.
     The width of an escape sequence (4) isn't the lower limit any longer.
     We've to use 8 as the lower limit, if we use chars_per_default_tab = 8
     to expand a tab which is not an input_tab-char. */
  मुक्त (clump_buff);
  clump_buff = xmalloc (MAX (8, chars_per_निवेशः_tab));
}

/* Open the necessary files,
   maintaining a COLUMN structure for each column.

   With multiple files, each column p has a different p->fp.
   With single files, each column p has the same p->fp.
   Return false if (number_of_files > 0) and no files can be opened,
   true otherwise.

   With each column/file p, p->full_page_printed is initialized,
   see also open_file.  */

स्थिर bool
init_fps (पूर्णाङ्क सङ्ख्या_of_files, वर्ण **av)
{
  COLUMN *p;

  total_files = 0;

  मुक्त (column_vector);
  column_vector = xnmalloc (columns, परिमाण (COLUMN));

  जइ (parallel_files)
    {
      पूर्णाङ्क files_वामः = सङ्ख्या_of_files;
      प्रति (p = column_vector; files_वामः--; ++p, ++av)
        {
          जइ (! open_file (*av, p))
            {
              --p;
              --columns;
            }
        }
      जइ (columns == 0)
        प्रत्यागम false;
      init_header ("", -1);
    }
  अण्णहा
    {
      p = column_vector;
      जइ (सङ्ख्या_of_files > 0)
        {
          जइ (! open_file (*av, p))
            प्रत्यागम false;
          init_header (*av, fileno (p->fp));
          p->पङ्क्तयः_stored = 0;
        }
      अण्णहा
        {
          p->name = _("standard input");
          p->fp = stdin;
          have_read_stdin = true;
          p->status = OPEN;
          p->full_page_printed = false;
          ++total_files;
          init_header ("", -1);
          p->पङ्क्तयः_stored = 0;
        }

      वर्ण ध्रुव *firstname = p->name;
      FILE *firstfp = p->fp;
      पूर्णाङ्क i;
      प्रति (i = columns - 1, ++p; i; --i, ++p)
        {
          p->name = firstname;
          p->fp = firstfp;
          p->status = OPEN;
          p->full_page_printed = false;
          p->पङ्क्तयः_stored = 0;
        }
    }
  files_ready_to_read = total_files;
  प्रत्यागम true;
}

/* Determine print_func and char_func, the functions
   used by each column for printing and/or storing.

   Determine the horizontal position desired when we begin
   printing a column (p->start_position). */

स्थिर शून्य
init_funcs (शून्य)
{
  पूर्णाङ्क i, h, h_next;
  COLUMN *p;

  h = chars_per_margin;

  जइ (!truncate_पङ्क्तयः)
    h_next = ANYWHERE;
  अण्णहा
    {
      /* When numbering lines of parallel files, we enlarge the
         first column to accommodate the number.  Looks better than
         the Sys V approach. */
      जइ (parallel_files && numbered_पङ्क्तयः)
        h_next = h + chars_per_column + सङ्ख्या_width;
      अण्णहा
        h_next = h + chars_per_column;
    }

  /* Enlarge p->start_position of first column to use the same form of
     padding_not_printed with all columns. */
  h = h + col_sep_length;

  /* This loop takes care of all but the rightmost column. */

  प्रति (p = column_vector, i = 1; i < columns; ++p, ++i)
    {
      जइ (storing_columns)	/* One file, multi columns down. */
        {
          p->char_func = store_char;
          p->print_func = print_stored;
        }
      अण्णहा
        /* One file, multi columns across; or parallel files.  */
        {
          p->char_func = print_char;
          p->print_func = read_पङ्क्तिः;
        }

      /* Number only the first column when printing files in
         parallel. */
      p->numbered = numbered_पङ्क्तयः && (!parallel_files || i == 1);
      p->start_position = h;

      /* If we don't truncate lines, all start_positions are
         ANYWHERE, except the first column's start_position when
         using a margin. */

      जइ (!truncate_पङ्क्तयः)
        {
          h = ANYWHERE;
          h_next = ANYWHERE;
        }
      अण्णहा
        {
          h = h_next + col_sep_length;
          h_next = h + chars_per_column;
        }
    }

  /* The rightmost column.

     Doesn't need to be stored unless we intend to balance
     columns on the last page. */
  जइ (storing_columns && balance_columns)
    {
      p->char_func = store_char;
      p->print_func = print_stored;
    }
  अण्णहा
    {
      p->char_func = print_char;
      p->print_func = read_पङ्क्तिः;
    }

  p->numbered = numbered_पङ्क्तयः && (!parallel_files || i == 1);
  p->start_position = h;
}

/* Open a file.  Return true if successful.

   With each file p, p->full_page_printed is initialized,
   see also init_fps. */

स्थिर bool
open_file (वर्ण *name, COLUMN *p)
{
  जइ (streq (name, "-"))
    {
      p->name = _("standard input");
      p->fp = stdin;
      have_read_stdin = true;
    }
  अण्णहा
    {
      p->name = name;
      p->fp = fopen (name, "r");
    }
  जइ (p->fp == रिक्त)
    {
      failed_opens = true;
      जइ (!ignore_failed_opens)
        error (0, errno, "%s", quotef (name));
      प्रत्यागम false;
    }
  fadvise (p->fp, FADVISE_SEQUENTIAL);
  p->status = OPEN;
  p->full_page_printed = false;
  ++total_files;
  प्रत्यागम true;
}

/* Close the file in P.

   If we aren't dealing with multiple files in parallel, we change
   the status of all columns in the column list to reflect the close. */

स्थिर शून्य
close_file (COLUMN *p)
{
  COLUMN *q;
  पूर्णाङ्क i;

  जइ (p->status == CLOSED)
    प्रत्यागम;

  पूर्णाङ्क err = errno;
  जइ (!ferror (p->fp))
    err = 0;
  जइ (fileno (p->fp) == STDIN_FILENO)
    clearerr (p->fp);
  अण्णहा जइ (fclose (p->fp) != 0 && !err)
    err = errno;
  जइ (err)
    error (EXIT_FAILURE, err, "%s", quotef (p->name));

  जइ (!parallel_files)
    {
      प्रति (q = column_vector, i = columns; i; ++q, --i)
        {
          q->status = CLOSED;
          जइ (q->पङ्क्तयः_stored == 0)
            {
              q->पङ्क्तयः_to_print = 0;
            }
        }
    }
  अण्णहा
    {
      p->status = CLOSED;
      p->पङ्क्तयः_to_print = 0;
    }

  --files_ready_to_read;
}

/* Put a file on hold until we start a new page,
   since we've hit a form feed.

   If we aren't dealing with parallel files, we must change the
   status of all columns in the column list. */

स्थिर शून्य
hold_file (COLUMN *p)
{
  COLUMN *q;
  पूर्णाङ्क i;

  जइ (!parallel_files)
    प्रति (q = column_vector, i = columns; i; ++q, --i)
      {
        जइ (storing_columns)
          q->status = FF_FOUND;
        अण्णहा
          q->status = ON_HOLD;
      }
  अण्णहा
    p->status = ON_HOLD;

  p->पङ्क्तयः_to_print = 0;
  --files_ready_to_read;
}

/* Undo hold_file -- go through the column list and change any
   ON_HOLD columns to OPEN.  Used at the end of each page. */

स्थिर शून्य
reset_status (शून्य)
{
  पूर्णाङ्क i = columns;
  COLUMN *p;

  प्रति (p = column_vector; i; --i, ++p)
    जइ (p->status == ON_HOLD)
      {
        p->status = OPEN;
        files_ready_to_read++;
      }

  जइ (storing_columns)
    {
      जइ (column_vector->status == CLOSED)
        /* We use the info to output an error message in  skip_to_page. */
        files_ready_to_read = 0;
      अण्णहा
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

स्थिर शून्य
print_files (पूर्णाङ्क सङ्ख्या_of_files, वर्ण **av)
{
  init_parameters (सङ्ख्या_of_files);
  जइ (! init_fps (सङ्ख्या_of_files, av))
    प्रत्यागम;
  जइ (storing_columns)
    init_store_cols ();

  जइ (first_page_सङ्ख्या > 1)
    {
      जइ (!skip_to_page (first_page_सङ्ख्या))
        प्रत्यागम;
      अण्णहा
        page_सङ्ख्या = first_page_सङ्ख्या;
    }
  अण्णहा
    page_सङ्ख्या = 1;

  init_funcs ();

  पङ्क्तिः_सङ्ख्या = पङ्क्तिः_गणनम्;
  जाव (print_page ())
    ;
}

/* Initialize header information.
   If DESC is non-negative, it is a file descriptor open to
   FILENAME for reading.  */

स्थिर शून्य
init_header (वर्ण ध्रुव *filename, पूर्णाङ्क desc)
{
  वर्ण *buf = रिक्त;
  संरचना stat st;
  संरचना timespec t;
  पूर्णाङ्क ns;
  संरचना tm tm;

  /* If parallel files or standard input, use current date. */
  जइ (streq (filename, "-"))
    desc = -1;
  जइ (0 <= desc && fstat (desc, &st) == 0)
    t = get_stat_mtime (&st);
  अण्णहा
    {
      स्थिर संरचना timespec timespec;
      जइ (! timespec.tv_sec)
        gettime (&timespec);
      t = timespec;
    }

  ns = t.tv_nsec;
  जइ (localtime_rz (localtz, &t.tv_sec, &tm))
    {
      ptrdiff_t len = nstrftime (रिक्त, MIN (PTRDIFF_MAX, SIZE_MAX),
                                 date_format, &tm, localtz, ns);
      जइ (0 <= len)
        {
          buf = ximalloc (len + 1);
          nstrftime (buf, len + 1, date_format, &tm, localtz, ns);
        }
    }
  जइ (!buf)
    {
      वर्ण secbuf[INT_BUFSIZE_BOUND (intmax_t)];
      buf = xmalloc (परिमाण secbuf + MAX (10, INT_BUFSIZE_BOUND (पूर्णाङ्क)));
      sprintf (buf, "%s.%09d", timetostr (t.tv_sec, secbuf), ns);
    }

  मुक्त (date_text);
  date_text = buf;
  file_text = custom_header ? custom_header : desc < 0 ? "" : filename;
  header_width_available = (chars_per_पङ्क्तिः
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

स्थिर शून्य
init_page (शून्य)
{
  पूर्णाङ्क j;
  COLUMN *p;

  जइ (storing_columns)
    {
      store_columns ();
      प्रति (j = columns - 1, p = column_vector; j; --j, ++p)
        {
          p->पङ्क्तयः_to_print = p->पङ्क्तयः_stored;
        }

      /* Last column. */
      जइ (balance_columns)
        {
          p->पङ्क्तयः_to_print = p->पङ्क्तयः_stored;
        }
      /* Since we're not balancing columns, we don't need to store
         the rightmost column.   Read it straight from the file. */
      अण्णहा
        {
          जइ (p->status == OPEN)
            {
              p->पङ्क्तयः_to_print = पङ्क्तयः_per_body;
            }
          अण्णहा
            p->पङ्क्तयः_to_print = 0;
        }
    }
  अण्णहा
    प्रति (j = columns, p = column_vector; j; --j, ++p)
      जइ (p->status == OPEN)
        {
          p->पङ्क्तयः_to_print = पङ्क्तयः_per_body;
        }
      अण्णहा
        p->पङ्क्तयः_to_print = 0;
}

/* Align empty columns and print separators.
   Empty columns will be formed by files with status ON_HOLD or CLOSED
   when printing multiple files in parallel. */

स्थिर शून्य
align_column (COLUMN *p)
{
  padding_not_printed = p->start_position;
  जइ (col_sep_length < padding_not_printed)
    {
      pad_across_to (padding_not_printed - col_sep_length);
      padding_not_printed = ANYWHERE;
    }

  जइ (use_col_separator)
    print_sep_string ();

  जइ (p->numbered)
    add_पङ्क्तिः_सङ्ख्या (p);
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

स्थिर bool
print_page (शून्य)
{
  पूर्णाङ्क j;
  पूर्णाङ्क पङ्क्तयः_वामः_on_page;
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

  जइ (cols_ready_to_print () == 0)
    प्रत्यागम false;

  जइ (extremities)
    print_a_header = true;

  /* Don't pad unless we know a page was printed. */
  pad_vertically = false;
  pv = false;

  पङ्क्तयः_वामः_on_page = पङ्क्तयः_per_body;
  जइ (double_space)
    पङ्क्तयः_वामः_on_page *= 2;

  जाव (पङ्क्तयः_वामः_on_page > 0 && cols_ready_to_print () > 0)
    {
      output_position = 0;
      spaces_not_printed = 0;
      separators_not_printed = 0;
      pad_vertically = false;
      align_empty_cols = false;
      empty_पङ्क्तिः = true;

      प्रति (j = 1, p = column_vector; j <= columns; ++j, ++p)
        {
          निवेशः_position = 0;
          जइ (p->पङ्क्तयः_to_print > 0 || p->status == FF_FOUND)
            {
              FF_only = false;
              padding_not_printed = p->start_position;
              जइ (!(p->print_func) (p))
                read_rest_of_पङ्क्तिः (p);
              pv |= pad_vertically;

              --p->पङ्क्तयः_to_print;
              जइ (p->पङ्क्तयः_to_print <= 0)
                {
                  जइ (cols_ready_to_print () == 0)
                    भङ्ग;
                }

              /* File p changed its status to ON_HOLD or CLOSED */
              जइ (parallel_files && p->status != OPEN)
                {
                  जइ (empty_पङ्क्तिः)
                    align_empty_cols = true;
                  अण्णहा जइ (p->status == CLOSED
                           || (p->status == ON_HOLD && FF_only))
                    align_column (p);
                }
            }
          अण्णहा जइ (parallel_files)
            {
              /* File status ON_HOLD or CLOSED */
              जइ (empty_पङ्क्तिः)
                align_empty_cols = true;
              अण्णहा
                align_column (p);
            }

          /* We need it also with an empty column */
          जइ (use_col_separator)
            ++separators_not_printed;
        }

      जइ (pad_vertically)
        {
          putchar ('\n');
          --पङ्क्तयः_वामः_on_page;
        }

      जइ (cols_ready_to_print () == 0 && !extremities)
        भङ्ग;

      जइ (double_space && pv)
        {
          putchar ('\n');
          --पङ्क्तयः_वामः_on_page;
        }
    }

  जइ (पङ्क्तयः_वामः_on_page == 0)
    प्रति (j = 1, p = column_vector; j <= columns; ++j, ++p)
      जइ (p->status == OPEN)
        p->full_page_printed = true;

  pad_vertically = pv;

  जइ (pad_vertically && extremities)
    pad_down (पङ्क्तयः_वामः_on_page + पङ्क्तयः_per_footer);
  अण्णहा जइ (keep_FF && print_a_FF)
    {
      putchar ('\f');
      print_a_FF = false;
    }

  जइ (ferror (stdout))
    write_error ();

  जइ (last_page_सङ्ख्या < ++page_सङ्ख्या)
    प्रत्यागम false;		/* Stop printing with LAST_PAGE */

  reset_status ();		/* Change ON_HOLD to OPEN. */

  प्रत्यागम true;			/* More pages to go. */
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

स्थिर शून्य
init_store_cols (शून्य)
{
  /* Tune this.  */
  पूर्णाङ्क total_पङ्क्तयः, total_पङ्क्तयः_1, chars_per_column_1, chars_if_truncate;
  जइ (ckd_mul (&total_पङ्क्तयः, पङ्क्तयः_per_body, columns)
      || ckd_add (&total_पङ्क्तयः_1, total_पङ्क्तयः, 1)
      || ckd_add (&chars_per_column_1, chars_per_column, 1)
      || ckd_mul (&chars_if_truncate, total_पङ्क्तयः, chars_per_column_1)
      || ckd_mul (&buff_allocated, chars_if_truncate, use_col_separator + 1))
    integer_overflow ();

  मुक्त (पङ्क्तिः_vector);
  /* FIXME: here's where it was allocated.  */
  पङ्क्तिः_vector = xnmalloc (total_पङ्क्तयः_1, परिमाण *पङ्क्तिः_vector);

  मुक्त (end_vector);
  end_vector = xnmalloc (total_पङ्क्तयः, परिमाण *end_vector);

  मुक्त (buff);
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

स्थिर शून्य
store_columns (शून्य)
{
  पूर्णाङ्क i, j;
  unsigned पूर्णाङ्क पङ्क्तिः = 0;
  unsigned पूर्णाङ्क buff_start;
  पूर्णाङ्क last_col;		/* The rightmost column which will be saved in buff */
  COLUMN *p;

  buff_current = 0;
  buff_start = 0;

  जइ (balance_columns)
    last_col = columns;
  अण्णहा
    last_col = columns - 1;

  प्रति (i = 1, p = column_vector; i <= last_col; ++i, ++p)
    p->पङ्क्तयः_stored = 0;

  प्रति (i = 1, p = column_vector; i <= last_col && files_ready_to_read;
       ++i, ++p)
    {
      p->current_पङ्क्तिः = पङ्क्तिः;
      प्रति (j = पङ्क्तयः_per_body; j && files_ready_to_read; --j)

        जइ (p->status == OPEN)	/* Redundant.  Clean up. */
          {
            निवेशः_position = 0;

            जइ (!read_पङ्क्तिः (p))
              read_rest_of_पङ्क्तिः (p);

            जइ (p->status == OPEN
                || buff_start != buff_current)
              {
                ++p->पङ्क्तयः_stored;
                पङ्क्तिः_vector[पङ्क्तिः] = buff_start;
                end_vector[पङ्क्तिः++] = निवेशः_position;
                buff_start = buff_current;
              }
          }
    }

  /* Keep track of the location of the last char in buff. */
  पङ्क्तिः_vector[पङ्क्तिः] = buff_start;

  जइ (balance_columns)
    balance (पङ्क्तिः);
}

स्थिर शून्य
balance (पूर्णाङ्क total_stored)
{
  COLUMN *p;
  पूर्णाङ्क i, पङ्क्तयः;
  पूर्णाङ्क first_पङ्क्तिः = 0;

  प्रति (i = 1, p = column_vector; i <= columns; ++i, ++p)
    {
      पङ्क्तयः = total_stored / columns;
      जइ (i <= total_stored % columns)
        ++पङ्क्तयः;

      p->पङ्क्तयः_stored = पङ्क्तयः;
      p->current_पङ्क्तिः = first_पङ्क्तिः;

      first_पङ्क्तिः += पङ्क्तयः;
    }
}

/* Store a character in the buffer. */

स्थिर शून्य
store_char (वर्ण c)
{
  जइ (buff_current >= buff_allocated)
    {
      /* May be too generous. */
      buff = xpalloc (buff, &buff_allocated, 1, -1, परिमाण *buff);
    }
  buff[buff_current++] = c;
}

स्थिर शून्य
add_पङ्क्तिः_सङ्ख्या (COLUMN *p)
{
  पूर्णाङ्क i;
  वर्ण *s;
  पूर्णाङ्क अङ्कः_width;

  /* Cutting off the higher-order digits is more informative than
     lower-order cut off. */
  अङ्कः_width = sprintf (सङ्ख्या_buff, "%*d", chars_per_सङ्ख्या, पङ्क्तिः_सङ्ख्या);
  पङ्क्तिः_सङ्ख्या++;
  s = सङ्ख्या_buff + (अङ्कः_width - chars_per_सङ्ख्या);
  प्रति (i = chars_per_सङ्ख्या; i > 0; i--)
    (p->char_func) (*s++);

  जइ (columns > 1)
    {
      /* Tabification is assumed for multiple columns, also for n-separators,
         but 'default n-separator = TAB' hasn't been given priority over
         equal column_width also specified by POSIX. */
      जइ (सङ्ख्या_separator == '\t')
        {
          i = सङ्ख्या_width - chars_per_सङ्ख्या;
          जाव (i-- > 0)
            (p->char_func) (' ');
        }
      अण्णहा
        (p->char_func) (सङ्ख्या_separator);
    }
  अण्णहा
    /* To comply with POSIX, we avoid any expansion of default TAB
       separator with a single column output. No column_width requirement
       has to be considered. */
    {
      (p->char_func) (सङ्ख्या_separator);
      जइ (सङ्ख्या_separator == '\t')
        output_position = POS_AFTER_TAB (chars_per_output_tab,
                          output_position);
    }

  जइ (truncate_पङ्क्तयः && !parallel_files)
    निवेशः_position += सङ्ख्या_width;
}

/* Print (or store) padding until the current horizontal position
   is position. */

स्थिर शून्य
pad_across_to (पूर्णाङ्क position)
{
  पूर्णाङ्क h = output_position;

  जइ (tabify_output)
    spaces_not_printed = position - output_position;
  अण्णहा
    {
      जाव (++h <= position)
        putchar (' ');
      output_position = position;
    }
}

/* Pad to the bottom of the page.

   If the user has requested a formfeed, use one.
   Otherwise, use newlines. */

स्थिर शून्य
pad_down (unsigned पूर्णाङ्क पङ्क्तयः)
{
  जइ (use_form_feed)
    putchar ('\f');
  अण्णहा
    प्रति (unsigned पूर्णाङ्क i = पङ्क्तयः; i; --i)
      putchar ('\n');
}

/* Read the rest of the line.

   Read from the current column's file until an end of line is
   hit.  Used when we've truncated a line and we no longer need
   to print or store its characters. */

स्थिर शून्य
read_rest_of_पङ्क्तिः (COLUMN *p)
{
  पूर्णाङ्क c;
  FILE *f = p->fp;

  जाव ((c = getc (f)) != '\n')
    {
      जइ (c == '\f')
        {
          जइ ((c = getc (f)) != '\n')
            ungetc (c, f);
          जइ (keep_FF)
            print_a_FF = true;
          hold_file (p);
          भङ्ग;
        }
      अण्णहा जइ (c == EOF)
        {
          close_file (p);
          भङ्ग;
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

स्थिर शून्य
skip_read (COLUMN *p, पूर्णाङ्क column_सङ्ख्या)
{
  पूर्णाङ्क c;
  FILE *f = p->fp;
  पूर्णाङ्क i;
  bool single_ff = false;
  COLUMN *q;

  /* Read 1st character in a line or any character succeeding a FF */
  जइ ((c = getc (f)) == '\f' && p->full_page_printed)
    /* A FF-coincidence with a previous full_page_printed.
       To avoid an additional empty page, eliminate the FF */
    जइ ((c = getc (f)) == '\n')
      c = getc (f);

  p->full_page_printed = false;

  /* 1st character a FF means a single FF without any printable
     characters. Don't count it as a line with -n option. */
  जइ (c == '\f')
    single_ff = true;

  /* Preparing for a FF-coincidence: Maybe we finish that page
     without a FF found */
  जइ (last_पङ्क्तिः)
    p->full_page_printed = true;

  जाव (c != '\n')
    {
      जइ (c == '\f')
        {
          /* No FF-coincidence possible,
             no catching up of a FF-coincidence with next page */
          जइ (last_पङ्क्तिः)
            {
              जइ (!parallel_files)
                प्रति (q = column_vector, i = columns; i; ++q, --i)
                  q->full_page_printed = false;
              अण्णहा
                p->full_page_printed = false;
            }

          जइ ((c = getc (f)) != '\n')
            ungetc (c, f);
          hold_file (p);
          भङ्ग;
        }
      अण्णहा जइ (c == EOF)
        {
          close_file (p);
          भङ्ग;
        }
      c = getc (f);
    }

  जइ (skip_गणनम्)
    जइ ((!parallel_files || column_सङ्ख्या == 1) && !single_ff)
      ++पङ्क्तिः_गणनम्;
}

/* If we're tabifying output,

   When print_char encounters white space it keeps track
   of our desired horizontal position and delays printing
   until this function is called. */

स्थिर शून्य
print_white_space (शून्य)
{
  पूर्णाङ्क h_new;
  पूर्णाङ्क h_old = output_position;
  पूर्णाङ्क goal = h_old + spaces_not_printed;

  जाव (goal - h_old > 1
         && (h_new = POS_AFTER_TAB (chars_per_output_tab, h_old)) <= goal)
    {
      putchar (output_tab_char);
      h_old = h_new;
    }
  जाव (++h_old <= goal)
    putchar (' ');

  output_position = goal;
  spaces_not_printed = 0;
}

/* Print column separators.

   We keep a count until we know that we'll be printing a line,
   then print_sep_string() is called. */

स्थिर शून्य
print_sep_string (शून्य)
{
  वर्ण ध्रुव *s = col_sep_string;
  पूर्णाङ्क l = col_sep_length;

  जइ (separators_not_printed <= 0)
    {
      /* We'll be starting a line with chars_per_margin, anything else? */
      जइ (spaces_not_printed > 0)
        print_white_space ();
    }
  अण्णहा
    {
      प्रति (; separators_not_printed > 0; --separators_not_printed)
        {
          जाव (l-- > 0)
            {
              /* 3 types of sep_strings: spaces only, spaces and chars,
              chars only */
              जइ (*s == ' ')
                {
                  /* We're tabifying output; consecutive spaces in
                  sep_string may have to be converted to tabs */
                  s++;
                  ++spaces_not_printed;
                }
              अण्णहा
                {
                  जइ (spaces_not_printed > 0)
                    print_white_space ();
                  putchar (*s++);
                  ++output_position;
                }
            }
          /* sep_string ends with some spaces */
          जइ (spaces_not_printed > 0)
            print_white_space ();
        }
    }
}

/* Print (or store, depending on p->char_func) a clump of N
   characters. */

स्थिर शून्य
print_clump (COLUMN *p, पूर्णाङ्क n, वर्ण *clump)
{
  जाव (n--)
    (p->char_func) (*clump++);

  जइ (ferror (stdout))
    write_error ();
}

/* Print a character.

   Update the following comment: process-char hasn't been used any
   longer.
   If we're tabifying, all tabs have been converted to spaces by
   process_char().  Keep a count of consecutive spaces, and when
   a nonspace is encountered, call print_white_space() to print the
   required number of tabs and spaces. */

स्थिर शून्य
print_char (वर्ण c)
{
  जइ (tabify_output)
    {
      जइ (c == ' ')
        {
          ++spaces_not_printed;
          प्रत्यागम;
        }
      अण्णहा जइ (spaces_not_printed > 0)
        print_white_space ();

      /* Nonprintables are assumed to have width 0, except '\b'. */
      जइ (! isprint (to_uchar (c)))
        {
          जइ (c == '\b')
            --output_position;
        }
      अण्णहा
        ++output_position;
    }
  putchar (c);
}

/* Skip to page PAGE before printing.
   PAGE may be larger than total number of pages. */

स्थिर bool
skip_to_page (uintmax_t page)
{
  प्रति (uintmax_t n = 1; n < page; ++n)
    {
      COLUMN *p;
      पूर्णाङ्क j;

      प्रति (पूर्णाङ्क i = 1; i < पङ्क्तयः_per_body; ++i)
        {
          प्रति (j = 1, p = column_vector; j <= columns; ++j, ++p)
            जइ (p->status == OPEN)
              skip_read (p, j);
        }
      last_पङ्क्तिः = true;
      प्रति (j = 1, p = column_vector; j <= columns; ++j, ++p)
        जइ (p->status == OPEN)
          skip_read (p, j);

      जइ (storing_columns)	/* change FF_FOUND to ON_HOLD */
        प्रति (j = 1, p = column_vector; j <= columns; ++j, ++p)
          जइ (p->status != CLOSED)
            p->status = ON_HOLD;

      reset_status ();
      last_पङ्क्तिः = false;

      जइ (files_ready_to_read < 1)
        {
          /* It's very helpful, normally the total number of pages is
             not known in advance.  */
          error (0, 0,
                 _("starting page number %ju exceeds page count %ju"),
                 page, n);
          भङ्ग;
        }
    }
  प्रत्यागम files_ready_to_read > 0;
}

/* Print a header.

   Formfeeds are assumed to use up two lines at the beginning of
   the page. */

स्थिर शून्य
print_header (शून्य)
{
  वर्ण page_text[256 + INT_STRLEN_BOUND (page_सङ्ख्या)];
  पूर्णाङ्क available_width;
  पूर्णाङ्क lhs_spaces;
  पूर्णाङ्क rhs_spaces;

  output_position = 0;
  pad_across_to (chars_per_margin);
  print_white_space ();

  जइ (page_सङ्ख्या == 0)
    error (EXIT_FAILURE, 0, _("page number overflow"));

  /* The translator must ensure that formatting the translation of
     "Page %ju" does not generate more than (sizeof page_text - 1)
     bytes.  */
  sprintf (page_text, _("Page %ju"), page_सङ्ख्या);
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

स्थिर bool
read_पङ्क्तिः (COLUMN *p)
{
  पूर्णाङ्क c;
  पूर्णाङ्क chars IF_LINT (= 0);
  पूर्णाङ्क last_निवेशः_position;
  पूर्णाङ्क j, k;
  COLUMN *q;

  /* read 1st character in each line or any character succeeding a FF: */
  c = getc (p->fp);

  last_निवेशः_position = निवेशः_position;

  जइ (c == '\f' && p->full_page_printed)
    जइ ((c = getc (p->fp)) == '\n')
      c = getc (p->fp);
  p->full_page_printed = false;

  विकल्प (c)
    {
    पक्ष '\f':
      जइ ((c = getc (p->fp)) != '\n')
        ungetc (c, p->fp);
      FF_only = true;
      जइ (print_a_header && !storing_columns)
        {
          pad_vertically = true;
          print_header ();
        }
      अण्णहा जइ (keep_FF)
        print_a_FF = true;
      hold_file (p);
      प्रत्यागम true;
    पक्ष EOF:
      close_file (p);
      प्रत्यागम true;
    पक्ष '\n':
      भङ्ग;
    स्वभाव:
      chars = char_to_clump (c);
    }

  जइ (truncate_पङ्क्तयः && निवेशः_position > chars_per_column)
    {
      निवेशः_position = last_निवेशः_position;
      प्रत्यागम false;
    }

  जइ (p->char_func != store_char)
    {
      pad_vertically = true;

      जइ (print_a_header && !storing_columns)
        print_header ();

      जइ (parallel_files && align_empty_cols)
        {
          /* We have to align empty columns at the beginning of a line. */
          k = separators_not_printed;
          separators_not_printed = 0;
          प्रति (j = 1, q = column_vector; j <= k; ++j, ++q)
            {
              align_column (q);
              separators_not_printed += 1;
            }
          padding_not_printed = p->start_position;
          जइ (truncate_पङ्क्तयः)
            spaces_not_printed = chars_per_column;
          अण्णहा
            spaces_not_printed = 0;
          align_empty_cols = false;
        }

      जइ (col_sep_length < padding_not_printed)
        {
          pad_across_to (padding_not_printed - col_sep_length);
          padding_not_printed = ANYWHERE;
        }

      जइ (use_col_separator)
        print_sep_string ();
    }

  जइ (p->numbered)
    add_पङ्क्तिः_सङ्ख्या (p);

  empty_पङ्क्तिः = false;
  जइ (c == '\n')
    प्रत्यागम true;

  print_clump (p, chars, clump_buff);

  जाव (true)
    {
      c = getc (p->fp);

      विकल्प (c)
        {
        पक्ष '\n':
          प्रत्यागम true;
        पक्ष '\f':
          जइ ((c = getc (p->fp)) != '\n')
            ungetc (c, p->fp);
          जइ (keep_FF)
            print_a_FF = true;
          hold_file (p);
          प्रत्यागम true;
        पक्ष EOF:
          close_file (p);
          प्रत्यागम true;
        }

      last_निवेशः_position = निवेशः_position;
      chars = char_to_clump (c);
      जइ (truncate_पङ्क्तयः && निवेशः_position > chars_per_column)
        {
          निवेशः_position = last_निवेशः_position;
          प्रत्यागम false;
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

स्थिर bool
print_stored (COLUMN *p)
{
  COLUMN *q;

  पूर्णाङ्क पङ्क्तिः = p->current_पङ्क्तिः++;
  वर्ण *first = &buff[पङ्क्तिः_vector[पङ्क्तिः]];
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
  वर्ण *last = &buff[पङ्क्तिः_vector[पङ्क्तिः + 1]];

  pad_vertically = true;

  जइ (print_a_header)
    print_header ();

  जइ (p->status == FF_FOUND)
    {
      पूर्णाङ्क i;
      प्रति (i = 1, q = column_vector; i <= columns; ++i, ++q)
        q->status = ON_HOLD;
      जइ (column_vector->पङ्क्तयः_to_print <= 0)
        {
          जइ (!extremities)
            pad_vertically = false;
          प्रत्यागम true;		/* print a header only */
        }
    }

  जइ (col_sep_length < padding_not_printed)
    {
      pad_across_to (padding_not_printed - col_sep_length);
      padding_not_printed = ANYWHERE;
    }

  जइ (use_col_separator)
    print_sep_string ();

  जाव (first != last)
    print_char (*first++);

  जइ (spaces_not_printed == 0)
    {
      output_position = p->start_position + end_vector[पङ्क्तिः];
      जइ (p->start_position - col_sep_length == chars_per_margin)
        output_position -= col_sep_length;
    }

  प्रत्यागम true;
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

स्थिर पूर्णाङ्क
char_to_clump (वर्ण c)
{
  unsigned वर्ण uc = c;
  वर्ण *s = clump_buff;
  वर्ण esc_buff[4];
  पूर्णाङ्क width;
  पूर्णाङ्क chars;
  पूर्णाङ्क chars_per_c = 8;

  जइ (c == निवेशः_tab_char)
    chars_per_c = chars_per_निवेशः_tab;

  जइ (c == निवेशः_tab_char || c == '\t')
    {
      width = TAB_WIDTH (chars_per_c, निवेशः_position);

      जइ (untabify_निवेशः)
        {
          प्रति (पूर्णाङ्क i = width; i; --i)
            *s++ = ' ';
          chars = width;
        }
      अण्णहा
        {
          *s = c;
          chars = 1;
        }

    }
  अण्णहा जइ (! isprint (uc))
    {
      जइ (use_esc_sequence)
        {
          width = 4;
          chars = 4;
          *s++ = '\\';
          sprintf (esc_buff, "%03o", uc);
          प्रति (पूर्णाङ्क i = 0; i <= 2; ++i)
            *s++ = esc_buff[i];
        }
      अण्णहा जइ (use_cntrl_prefix)
        {
          जइ (uc < 0200)
            {
              width = 2;
              chars = 2;
              *s++ = '^';
              *s = c ^ 0100;
            }
          अण्णहा
            {
              width = 4;
              chars = 4;
              *s++ = '\\';
              sprintf (esc_buff, "%03o", uc);
              प्रति (पूर्णाङ्क i = 0; i <= 2; ++i)
                *s++ = esc_buff[i];
            }
        }
      अण्णहा जइ (c == '\b')
        {
          width = -1;
          chars = 1;
          *s = c;
        }
      अण्णहा
        {
          width = 0;
          chars = 1;
          *s = c;
        }
    }
  अण्णहा
    {
      width = 1;
      chars = 1;
      *s = c;
    }

  /* Too many backspaces must put us in position 0 -- never negative.  */
  जइ (width < 0 && निवेशः_position == 0)
    {
      chars = 0;
      निवेशः_position = 0;
    }
  अण्णहा जइ (width < 0 && निवेशः_position <= -width)
    निवेशः_position = 0;
  अण्णहा
    {
      जइ (ckd_add (&निवेशः_position, निवेशः_position, width))
        integer_overflow ();
    }

  प्रत्यागम chars;
}

/* We've just printed some files and need to clean up things before
   looking for more options and printing the next batch of files.

   Free everything we've xmalloc'ed, except 'header'. */

स्थिर शून्य
cleanup (शून्य)
{
  मुक्त (सङ्ख्या_buff);
  मुक्त (clump_buff);
  मुक्त (column_vector);
  मुक्त (पङ्क्तिः_vector);
  मुक्त (end_vector);
  मुक्त (buff);
}

/* Complain, print a usage message, and die. */

शून्य
usage (पूर्णाङ्क status)
{
  जइ (status != EXIT_SUCCESS)
    emit_try_help ();
  अण्णहा
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              program_name);

      fputs (_("\
Paginate or columnate FILE(s) प्रति printing.\n\
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
         Balance सङ्ख्या of पङ्क्तयः in the columns on each page\n\
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
  -d, --द्विगुण-space\n\
         द्विगुण space the output\n\
"));
      oputs (_("\
  -D, --date-format=FORMAT\n\
         use FORMAT प्रति the header date\n\
"));
      oputs (_("\
  -e[CHAR[WIDTH]], --expand-tabs[=CHAR[WIDTH]]\n\
         expand निवेशः CHARs (TABs) to tab WIDTH (8)\n\
"));
      oputs (_("\
  -F, -f, --form-feed\n\
         use form feeds instead of newlines to separate pages\n\
         (by a 3-पङ्क्तिः page header with -F or a 5-पङ्क्तिः header\n\
         and trailer without -F)\n\
"));
      oputs (_("\
  -h, --header=HEADER\n\
         use a centered HEADER instead of filename in page header,\n\
         -h \"\" prints a blank पङ्क्तिः, don't use -h\"\"\n\
"));
      oputs (_("\
  -i[CHAR[WIDTH]], --output-tabs[=CHAR[WIDTH]]\n\
         replace spaces with CHARs (TABs) to tab WIDTH (8)\n\
"));
      oputs (_("\
  -J, --join-पङ्क्तयः\n\
         merge full पङ्क्तयः, turns off -W पङ्क्तिः truncation,\n\
         no column alignment, --sep-string[=STRING] sets separators\n\
"));
      oputs (_("\
  -l, --length=PAGE_LENGTH\n\
         set the page length to PAGE_LENGTH (66) पङ्क्तयः\n\
         (स्वभाव सङ्ख्या of पङ्क्तयः of text 56, and with -F 63).\n\
         implies -t जइ PAGE_LENGTH <= 10\n\
"));
      oputs (_("\
  -m, --merge\n\
         print all files in parallel, one in each column,\n\
         truncate पङ्क्तयः, but join पङ्क्तयः of full length with -J\n\
"));
      oputs (_("\
  -n[SEP[DIGITS]], --सङ्ख्या-पङ्क्तयः[=SEP[DIGITS]]\n\
         सङ्ख्या पङ्क्तयः, use DIGITS (5) digits, then SEP (TAB),\n\
         स्वभाव counting starts with 1st पङ्क्तिः of निवेशः file\n\
"));
      oputs (_("\
  -N, --first-पङ्क्तिः-सङ्ख्या=सङ्ख्या_2\n\
         start counting with सङ्ख्या_2 at 1st पङ्क्तिः of first\n\
         page printed (see +FIRST_PAGE)\n\
"));
      oputs (_("\
  -o, --indent=MARGIN\n\
         offset each पङ्क्तिः with MARGIN (zero) spaces, करेहि not\n\
         affect -w or -W, MARGIN will be added to PAGE_WIDTH\n\
"));
      oputs (_("\
  -r, --no-file-warnings\n\
         omit warning when a file cannot be opened\n\
"));
      oputs (_("\
  -s[CHAR], --separator[=CHAR]\n\
         separate columns by a single character, स्वभाव प्रति CHAR\n\
         is the <TAB> character without -w and \'no वर्ण\' with -w.\n\
         -s[CHAR] turns off पङ्क्तिः truncation of all 3 column\n\
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
         omit page headers and trailers; implied जइ PAGE_LENGTH <= 10\n\
"));
      oputs (_("\
  -T, --omit-pagination\n\
         omit page headers and trailers,\n\
         eliminate any pagination by form feeds set in निवेशः files\n\
"));
      oputs (_("\
  -v, --show-nonprinting\n\
         use octal backslash notation\n\
"));
      oputs (_("\
  -w, --width=PAGE_WIDTH\n\
         set page width to PAGE_WIDTH (72) characters प्रति\n\
         multiple text-column output only, -s[वर्ण] turns off (72)\n\
"));
      oputs (_("\
  -W, --page-width=PAGE_WIDTH\n\
         set page width to PAGE_WIDTH (72) characters always,\n\
         truncate पङ्क्तयः, except -J option is set,\n\
         no interference with -S or -s\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}
