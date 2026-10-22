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

#અન્તર્ભાવ <config.h>

#અન્તર્ભાવ <getopt.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "c-ctype.h"
#અન્તર્ભાવ "fadvise.h"
#અન્તર્ભાવ "hard-locale.h"
#અન્તર્ભાવ "mbswidth.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "stat-time.h"
#અન્તર્ભાવ "stdio--.h"
#અન્તર્ભાવ "strftime.h"
#અન્તર્ભાવ "xstrtol.h"
#અન્તર્ભાવ "xstrtol-error.h"
#અન્તર્ભાવ "xdectoint.h"

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

સંરચના COLUMN;
સંરચના COLUMN
  {
    FILE *fp;			/* Input stream for this column. */
    અક્ષર અચલ *name;		/* File name. */
    ગણના
      {
        OPEN,
        FF_FOUND,		/* used with -b option, set with \f, changed
                                   to ON_HOLD after print_header */
        ON_HOLD,		/* Hit a form feed. */
        CLOSED
      }
    status;			/* Status of the file pointer. */

    /* Func to print lines in this col. */
    bool (*print_func) (સંરચના COLUMN *);

    /* Func to print/store chars in this col. */
    શૂન્ય (*char_func) (અક્ષર);

    પૂર્ણાંક current_લીટી;		/* Index of current place in line_vector. */
    પૂર્ણાંક લીટીઓ_stored;		/* Number of lines stored in buff. */
    પૂર્ણાંક લીટીઓ_to_print;		/* No. lines stored or space left on page. */
    પૂર્ણાંક start_position;		/* Horizontal position of first char. */
    bool numbered;
    bool full_page_printed;	/* True means printed without a FF found. */

    /* p->full_page_printed  controls a special case of "FF set by hand":
       True means a full page has been printed without FF found. To avoid an
       additional empty page we have to ignore a FF immediately following in
       the next line. */
  };

પ્રકારવ્યાખ્યા સંરચના COLUMN COLUMN;

સ્થિર પૂર્ણાંક char_to_clump (અક્ષર c);
સ્થિર bool read_લીટી (COLUMN *p);
સ્થિર bool print_page (શૂન્ય);
સ્થિર bool print_stored (COLUMN *p);
સ્થિર bool open_file (અક્ષર *name, COLUMN *p);
સ્થિર bool skip_to_page (uintmax_t page);
સ્થિર શૂન્ય print_header (શૂન્ય);
સ્થિર શૂન્ય pad_across_to (પૂર્ણાંક position);
સ્થિર શૂન્ય add_લીટી_સંખ્યા (COLUMN *p);
સ્થિર પૂર્ણાંક getoptnum (અક્ષર અચલ *n_str, પૂર્ણાંક min, અક્ષર અચલ *errfmt);
સ્થિર શૂન્ય getoptarg (અક્ષર *arg, અક્ષર switch_char, અક્ષર *character,
                       પૂર્ણાંક *સંખ્યા);
સ્થિર શૂન્ય print_files (પૂર્ણાંક સંખ્યા_of_files, અક્ષર **av);
સ્થિર શૂન્ય init_parameters (પૂર્ણાંક સંખ્યા_of_files);
સ્થિર શૂન્ય init_header (અક્ષર અચલ *filename, પૂર્ણાંક desc);
સ્થિર bool init_fps (પૂર્ણાંક સંખ્યા_of_files, અક્ષર **av);
સ્થિર શૂન્ય init_funcs (શૂન્ય);
સ્થિર શૂન્ય init_store_cols (શૂન્ય);
સ્થિર શૂન્ય store_columns (શૂન્ય);
સ્થિર શૂન્ય balance (પૂર્ણાંક total_stored);
સ્થિર શૂન્ય store_char (અક્ષર c);
સ્થિર શૂન્ય pad_down (unsigned પૂર્ણાંક લીટીઓ);
સ્થિર શૂન્ય read_rest_of_લીટી (COLUMN *p);
સ્થિર શૂન્ય skip_read (COLUMN *p, પૂર્ણાંક column_સંખ્યા);
સ્થિર શૂન્ય print_char (અક્ષર c);
સ્થિર શૂન્ય cleanup (શૂન્ય);
સ્થિર શૂન્ય print_sep_string (શૂન્ય);
સ્થિર શૂન્ય separator_string (અક્ષર અચલ *optarg_S);

/* All of the columns to print.  */
સ્થિર COLUMN *column_vector;

/* When printing a single file in multiple downward columns,
   we store the leftmost columns contiguously in buff.
   To print a line from buff, get the index of the first character
   from line_vector[i], and print up to line_vector[i + 1]. */
સ્થિર અક્ષર *buff;

/* Index of the position in buff where the next character
   will be stored. */
સ્થિર unsigned પૂર્ણાંક buff_current;

/* The number of characters in buff.
   Used for allocation of buff and to detect overflow of buff. */
સ્થિર idx_t buff_allocated;

/* Array of indices into buff.
   Each entry is an index of the first character of a line.
   This is used when storing lines to facilitate shuffling when
   we do column balancing on the last page. */
સ્થિર પૂર્ણાંક *લીટી_vector;

/* Array of horizontal positions.
   For each line in line_vector, end_vector[line] is the horizontal
   position we are in after printing that line.  We keep track of this
   so that we know how much we need to pad to prepare for the next
   column. */
સ્થિર પૂર્ણાંક *end_vector;

/* (-m) True means we're printing multiple files in parallel. */
સ્થિર bool parallel_files = false;

/* (-m) True means a line starts with some empty columns (some files
   already CLOSED or ON_HOLD) which we have to align. */
સ્થિર bool align_empty_cols;

/* (-m) True means we have not yet found any printable column in a line.
   align_empty_cols = true  has to be maintained. */
સ્થિર bool empty_લીટી;

/* (-m) False means printable column output precedes a form feed found.
   Column alignment is done only once. No additional action with that form
   feed.
   True means we found only a form feed in a column. Maybe we have to do
   some column alignment with that form feed. */
સ્થિર bool FF_only;

/* (-[0-9]+) True means we're given an option explicitly specifying
   number of columns.  Used to detect when this option is used with -m
   and when translating old options to new/long options. */
સ્થિર bool explicit_columns = false;

/* (-t|-T) False means we aren't printing headers and footers. */
સ્થિર bool extremities = true;

/* (-t) True means we retain all FF set by hand in input files.
   False is set with -T option. */
સ્થિર bool keep_FF = false;
સ્થિર bool print_a_FF = false;

/* True means we need to print a header as soon as we know we've got input
   to print after it. */
સ્થિર bool print_a_header;

/* (-f) True means use formfeeds instead of newlines to separate pages. */
સ્થિર bool use_form_feed = false;

/* True means we have read the standard input. */
સ્થિર bool have_read_stdin = false;

/* True means the -a flag has been given. */
સ્થિર bool print_across_flag = false;

/* True means we're printing one file in multiple (>1) downward columns. */
સ્થિર bool storing_columns = true;

/* (-b) True means balance columns on the last page as Sys V does. */
/* That's no longer an independent option. With storing_columns = true
   balance_columns = true is used too (s. function init_parameters).
   We get a consistent formulation with "FF set by hand" in input files. */
સ્થિર bool balance_columns = false;

/* (-l) Number of lines on a page, including header and footer lines. */
સ્થિર પૂર્ણાંક લીટીઓ_per_page = 66;

/* Number of lines in the header and footer can be reset to 0 using
   the -t flag. */
ગણના { લીટીઓ_per_header = 5 };
સ્થિર પૂર્ણાંક લીટીઓ_per_body;
ગણના { લીટીઓ_per_footer = 5 };

/* (-w|-W) Width in characters of the page.  Does not include the width of
   the margin. */
સ્થિર પૂર્ણાંક chars_per_લીટી = 72;

/* (-w|W) True means we truncate lines longer than chars_per_column. */
સ્થિર bool truncate_લીટીઓ = false;

/* (-J) True means we join lines without any line truncation. -J
   dominates -w option. */
સ્થિર bool join_લીટીઓ = false;

/* Number of characters in a column.  Based on col_sep_length and
   page width. */
સ્થિર પૂર્ણાંક chars_per_column;

/* (-e) True means convert tabs to spaces on input. */
સ્થિર bool untabify_આગત = false;

/* (-e) The input tab character. */
સ્થિર અક્ષર આગત_tab_char = '\t';

/* (-e) Tabstops are at chars_per_tab, 2*chars_per_tab, 3*chars_per_tab, ...
   where the leftmost column is 1. */
સ્થિર પૂર્ણાંક chars_per_આગત_tab = 8;

/* (-i) True means convert spaces to tabs on output. */
સ્થિર bool tabify_output = false;

/* (-i) The output tab character. */
સ્થિર અક્ષર output_tab_char = '\t';

/* (-i) The width of the output tab. */
સ્થિર પૂર્ણાંક chars_per_output_tab = 8;

/* Keeps track of pending white space.  When we hit a nonspace
   character after some whitespace, we print whitespace, tabbing
   if necessary to get to output_position + spaces_not_printed. */
સ્થિર પૂર્ણાંક spaces_not_printed;

/* (-o) Number of spaces in the left margin (tabs used when possible). */
સ્થિર પૂર્ણાંક chars_per_margin = 0;

/* Position where the next character will fall.
   Leftmost position is 0 + chars_per_margin.
   Rightmost position is chars_per_margin + chars_per_line - 1.
   This is important for converting spaces to tabs on output. */
સ્થિર પૂર્ણાંક output_position;

/* Horizontal position relative to the current file.
   (output_position depends on where we are on the page;
   input_position depends on where we are in the file.)
   Important for converting tabs to spaces on input. */
સ્થિર પૂર્ણાંક આગત_position;

/* True if there were any failed opens so we can exit with nonzero
   status.  */
સ્થિર bool failed_opens = false;

/* The number of spaces taken up if we print a tab character with width
   c_ from position h_. */
#define TAB_WIDTH(c_, h_) ((c_) - ((h_) % (c_)))

/* The horizontal position we'll be at after printing a tab character
   of width c_ from the position h_. */
#define POS_AFTER_TAB(c_, h_) ((h_) + TAB_WIDTH (c_, h_))

/* (-NNN) Number of columns of text to print. */
સ્થિર પૂર્ણાંક columns = 1;

/* (+NNN:MMM) Page numbers on which to begin and stop printing.
   first_page_number = 0  will be used to check input only. */
સ્થિર uintmax_t first_page_સંખ્યા = 0;
સ્થિર uintmax_t last_page_સંખ્યા = UINTMAX_MAX;

/* Number of files open (not closed, not on hold). */
સ્થિર પૂર્ણાંક files_ready_to_read = 0;

/* Current page number.  Displayed in header. */
સ્થિર uintmax_t page_સંખ્યા;

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
સ્થિર પૂર્ણાંક લીટી_સંખ્યા;

/* (-n) True means lines should be preceded by numbers. */
સ્થિર bool numbered_લીટીઓ = false;

/* (-n) Character which follows each line number. */
સ્થિર અક્ષર સંખ્યા_separator = '\t';

/* (-n) line counting starts with 1st line of input file (not with 1st
   line of 1st page printed). */
સ્થિર પૂર્ણાંક લીટી_ગણતરી = 1;

/* (-n) True means counting of skipped lines starts with 1st line of
   input file. False means -N option is used in addition, counting of
   skipped lines not required. */
સ્થિર bool skip_ગણતરી = true;

/* (-N) Counting starts with start_line_number = NUMBER at 1st line of
   first page printed, usually not 1st page of input file. */
સ્થિર પૂર્ણાંક start_લીટી_અંક = 1;

/* (-n) Width in characters of a line number. */
સ્થિર પૂર્ણાંક chars_per_સંખ્યા = 5;

/* Used when widening the first column to accommodate numbers -- only
   needed when printing files in parallel.  Includes width of both the
   number and the number_separator. */
સ્થિર પૂર્ણાંક સંખ્યા_width;

/* Buffer sprintf uses to format a line number. */
સ્થિર અક્ષર *સંખ્યા_buff;

/* (-v) True means nonprintable characters are printed as escape sequences.
   control-g becomes \007. */
સ્થિર bool use_esc_sequence = false;

/* (-c) True means nonprintable characters are printed as control prefixes.
   control-g becomes ^G. */
સ્થિર bool use_cntrl_prefix = false;

/* (-d) True means output is double spaced. */
સ્થિર bool double_space = false;

/* Number of files opened initially in init_files.  Should be 1
   unless we're printing multiple files in parallel. */
સ્થિર પૂર્ણાંક total_files = 0;

/* (-r) True means don't complain if we can't open a file. */
સ્થિર bool ignore_failed_opens = false;

/* (-S) True means we separate columns with a specified string.
   -S option does not affect line truncation nor column alignment. */
સ્થિર bool use_col_separator = false;

/* String used to separate columns if the -S option has been specified.
   Default without -S but together with one of the column options
   -a|COLUMN|-m is a 'space' and with the -J option a 'tab'. */
સ્થિર અક્ષર અચલ *col_sep_string = "";
સ્થિર પૂર્ણાંક col_sep_length = 0;
સ્થિર અક્ષર *column_separator = (અક્ષર *) " ";
સ્થિર અક્ષર *લીટી_separator = (અક્ષર *) "\t";

/* Number of separator characters waiting to be printed as soon as we
   know that we have any input remaining to be printed. */
સ્થિર પૂર્ણાંક separators_not_printed;

/* Position we need to pad to, as soon as we know that we have input
   remaining to be printed. */
સ્થિર પૂર્ણાંક padding_not_printed;

/* True means we should pad the end of the page.  Remains false until we
   know we have a page to print. */
સ્થિર bool pad_vertically;

/* (-h) String of characters used in place of the filename in the header. */
સ્થિર અક્ષર *custom_header;

/* (-D) Date format for the header.  */
સ્થિર અક્ષર અચલ *date_format;

/* The local time zone rules, as per the TZ environment variable.  */
સ્થિર timezone_t localtz;

/* Date and file name for the header.  */
સ્થિર અક્ષર *date_text;
સ્થિર અક્ષર અચલ *file_text;

/* Output columns available, not counting the date and file name.  */
સ્થિર પૂર્ણાંક header_width_available;

સ્થિર અક્ષર *clump_buff;

/* True means we read the line no. lines_per_body in skip_read
   called by skip_to_page. That variable controls the coincidence of a
   "FF set by hand" and "full_page_printed", see above the definition of
   structure COLUMN. */
સ્થિર bool last_લીટી = false;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ગણના
{
  COLUMNS_OPTION = CHAR_MAX + 1,
  PAGES_OPTION
};

સ્થિર અક્ષર અચલ short_options[] =
  "-0123456789D:FJN:S::TW:abcde::fh:i::l:mn::o:rs::tvw:";

સ્થિર સંરચના option અચલ long_options[] =
{
  {"pages", required_argument, ખાલી, PAGES_OPTION},
  {"columns", required_argument, ખાલી, COLUMNS_OPTION},
  {"across", no_argument, ખાલી, 'a'},
  {"show-control-chars", no_argument, ખાલી, 'c'},
  {"double-space", no_argument, ખાલી, 'd'},
  {"date-format", required_argument, ખાલી, 'D'},
  {"expand-tabs", optional_argument, ખાલી, 'e'},
  {"form-feed", no_argument, ખાલી, 'f'},
  {"header", required_argument, ખાલી, 'h'},
  {"output-tabs", optional_argument, ખાલી, 'i'},
  {"join-lines", no_argument, ખાલી, 'J'},
  {"length", required_argument, ખાલી, 'l'},
  {"merge", no_argument, ખાલી, 'm'},
  {"number-lines", optional_argument, ખાલી, 'n'},
  {"first-line-number", required_argument, ખાલી, 'N'},
  {"indent", required_argument, ખાલી, 'o'},
  {"no-file-warnings", no_argument, ખાલી, 'r'},
  {"separator", optional_argument, ખાલી, 's'},
  {"sep-string", optional_argument, ખાલી, 'S'},
  {"omit-header", no_argument, ખાલી, 't'},
  {"omit-pagination", no_argument, ખાલી, 'T'},
  {"show-nonprinting", no_argument, ખાલી, 'v'},
  {"width", required_argument, ખાલી, 'w'},
  {"page-width", required_argument, ખાલી, 'W'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ખાલી, 0, ખાલી, 0}
};

સ્થિર _Noreturn શૂન્ય
integer_overflow (શૂન્ય)
{
  error (EXIT_FAILURE, 0, _("integer overflow"));
}

/* Return the number of columns that have either an open file or
   stored lines. */

ATTRIBUTE_PURE
સ્થિર unsigned પૂર્ણાંક
cols_ready_to_print (શૂન્ય)
{
  COLUMN *q;
  unsigned પૂર્ણાંક i;
  unsigned પૂર્ણાંક n;

  n = 0;
  માટે (q = column_vector, i = 0; i < columns; ++q, ++i)
    જો (q->status == OPEN
        || q->status == FF_FOUND	/* With -b: To print a header only */
        || (storing_columns && q->લીટીઓ_stored > 0 && q->લીટીઓ_to_print > 0))
      ++n;
  પાછા n;
}

/* Estimate first_ / last_page_number
   using option +FIRST_PAGE:LAST_PAGE */

સ્થિર bool
first_last_page (પૂર્ણાંક oi, અક્ષર c, અક્ષર અચલ *pages)
{
  અક્ષર *p;
  uintmax_t first;
  uintmax_t last = UINTMAX_MAX;
  strtol_error err = xstrtoumax (pages, &p, 10, &first, "");
  જો (err != LONGINT_OK && err != LONGINT_INVALID_SUFFIX_CHAR)
    xstrtol_fatal (err, oi, c, long_options, pages);

  જો (p == pages || !first)
    પાછા false;

  જો (*p == ':')
    {
      અક્ષર અચલ *p1 = p + 1;
      err = xstrtoumax (p1, &p, 10, &last, "");
      જો (err != LONGINT_OK)
        xstrtol_fatal (err, oi, c, long_options, pages);
      જો (p1 == p || last < first)
        પાછા false;
    }

  જો (*p)
    પાછા false;

  first_page_સંખ્યા = first;
  last_page_સંખ્યા = last;
  પાછા true;
}

/* Parse column count string S, and if it's valid (1 or larger and
   within range of the type of 'columns') set the global variables
   columns and explicit_columns.  Otherwise, exit with a diagnostic.  */

સ્થિર શૂન્ય
parse_column_ગણતરી (અક્ષર અચલ *s)
{
  columns = getoptnum (s, 1, _("invalid number of columns"));
  explicit_columns = true;
}

/* Estimate length of col_sep_string with option -S.  */

સ્થિર શૂન્ય
separator_string (અક્ષર અચલ *optarg_S)
{
  size_t len = strlen (optarg_S);
  જો (INT_MAX < len)
    integer_overflow ();
  col_sep_length = len;
  col_sep_string = optarg_S;
}

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  unsigned પૂર્ણાંક n_files;
  bool old_options = false;
  bool old_w = false;
  bool old_s = false;
  અક્ષર **file_names;

  /* Accumulate the digits of old-style options like -99.  */
  અક્ષર *column_ગણતરી_string = ખાલી;
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
                ? xnmalloc (argc - 1, કદ (અક્ષર *))
                : ખાલી);

  જ્યારે (true)
    {
      પૂર્ણાંક oi = -1;
      પૂર્ણાંક c = getopt_long (argc, argv, short_options, long_options, &oi);
      જો (c == -1)
        તોડો;

      જો (c_isdigit (c))
        {
          /* Accumulate column-count digits specified via old-style options. */
          જો (n_digits + 1 >= n_alloc)
            column_ગણતરી_string = xpalloc (column_ગણતરી_string, &n_alloc, 2, -1,
                                           કદ *column_ગણતરી_string);
          column_ગણતરી_string[n_digits++] = c;
          column_ગણતરી_string[n_digits] = '\0';
          ચાલુ;
        }

      n_digits = 0;

      પસંદગી (c)
        {
        સ્થિતિ 1:			/* Non-option argument. */
          /* long option --page dominates old '+FIRST_PAGE ...'.  */
          જો (! (first_page_સંખ્યા == 0
                 && *optarg == '+' && first_last_page (-2, '+', optarg + 1)))
            file_names[n_files++] = optarg;
          તોડો;

        સ્થિતિ PAGES_OPTION:	/* --pages=FIRST_PAGE[:LAST_PAGE] */
          {			/* dominates old opt +... */
            જો (! optarg)
              error (EXIT_FAILURE, 0,
                     _("'--pages=FIRST_PAGE[:LAST_PAGE]' missing argument"));
            નહીંતર જો (! first_last_page (oi, 0, optarg))
              error (EXIT_FAILURE, 0, _("invalid page range %s"),
                     quote (optarg));
            તોડો;
          }

        સ્થિતિ COLUMNS_OPTION:	/* --columns=COLS */
          {
            parse_column_ગણતરી (optarg);

            /* If there was a prior column count specified via the
               short-named option syntax, e.g., -9, ensure that this
               long-name-specified value overrides it.  */
            મુક્ત (column_ગણતરી_string);
            column_ગણતરી_string = ખાલી;
            n_alloc = 0;
            તોડો;
          }

        સ્થિતિ 'a':
          print_across_flag = true;
          storing_columns = false;
          તોડો;
        સ્થિતિ 'b':
          balance_columns = true;
          તોડો;
        સ્થિતિ 'c':
          use_cntrl_prefix = true;
          તોડો;
        સ્થિતિ 'd':
          double_space = true;
          તોડો;
        સ્થિતિ 'D':
          date_format = optarg;
          તોડો;
        સ્થિતિ 'e':
          જો (optarg)
            getoptarg (optarg, 'e', &આગત_tab_char,
                       &chars_per_આગત_tab);
          /* Could check tab width > 0. */
          untabify_આગત = true;
          તોડો;
        સ્થિતિ 'f':
        સ્થિતિ 'F':
          use_form_feed = true;
          તોડો;
        સ્થિતિ 'h':
          custom_header = optarg;
          તોડો;
        સ્થિતિ 'i':
          જો (optarg)
            getoptarg (optarg, 'i', &output_tab_char,
                       &chars_per_output_tab);
          /* Could check tab width > 0. */
          tabify_output = true;
          તોડો;
        સ્થિતિ 'J':
          join_લીટીઓ = true;
          તોડો;
        સ્થિતિ 'l':
          લીટીઓ_per_page
            = getoptnum (optarg, 1,
                         _("'-l PAGE_LENGTH' invalid number of lines"));
          તોડો;
        સ્થિતિ 'm':
          parallel_files = true;
          storing_columns = false;
          તોડો;
        સ્થિતિ 'n':
          numbered_લીટીઓ = true;
          જો (optarg)
            getoptarg (optarg, 'n', &સંખ્યા_separator,
                       &chars_per_સંખ્યા);
          તોડો;
        સ્થિતિ 'N':
          skip_ગણતરી = false;
          start_લીટી_અંક
            = getoptnum (optarg, INT_MIN,
                         _("'-N NUMBER' invalid starting line number"));
          તોડો;
        સ્થિતિ 'o':
          chars_per_margin = getoptnum (optarg, 0,
                                        _("'-o MARGIN' invalid line offset"));
          તોડો;
        સ્થિતિ 'r':
          ignore_failed_opens = true;
          તોડો;
        સ્થિતિ 's':
          old_options = true;
          old_s = true;
          જો (!use_col_separator && optarg)
            separator_string (optarg);
          તોડો;
        સ્થિતિ 'S':
          old_s = false;
          /* Reset an additional input of -s, -S dominates -s */
          col_sep_string = "";
          col_sep_length = 0;
          use_col_separator = true;
          જો (optarg)
            separator_string (optarg);
          તોડો;
        સ્થિતિ 't':
          extremities = false;
          keep_FF = true;
          તોડો;
        સ્થિતિ 'T':
          extremities = false;
          keep_FF = false;
          તોડો;
        સ્થિતિ 'v':
          use_esc_sequence = true;
          તોડો;
        સ્થિતિ 'w':
          old_options = true;
          old_w = true;
          {
            પૂર્ણાંક tmp_cpl
              = getoptnum (optarg, 1,
                           _("'-w PAGE_WIDTH' invalid number of characters"));
            જો (! truncate_લીટીઓ)
              chars_per_લીટી = tmp_cpl;
          }
          તોડો;
        સ્થિતિ 'W':
          old_w = false;			/* dominates -w */
          truncate_લીટીઓ = true;
          chars_per_લીટી
            = getoptnum (optarg, 1,
                         _("'-W PAGE_WIDTH' invalid number of characters"));
          તોડો;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        મૂળભૂત:
          usage (EXIT_FAILURE);
          તોડો;
        }
    }

  જો (column_ગણતરી_string)
    {
      parse_column_ગણતરી (column_ગણતરી_string);
      મુક્ત (column_ગણતરી_string);
    }

  જો (! date_format)
    date_format = (getenv ("POSIXLY_CORRECT") && !hard_locale (LC_TIME)
                   ? "%b %e %H:%M %Y"
                   : "%Y-%m-%d %H:%M");

  localtz = tzalloc (getenv ("TZ"));

  /* Now we can set a reasonable initial value: */
  જો (first_page_સંખ્યા == 0)
    first_page_સંખ્યા = 1;

  જો (parallel_files && explicit_columns)
    error (EXIT_FAILURE, 0,
           _("cannot specify number of columns when printing in parallel"));

  જો (parallel_files && print_across_flag)
    error (EXIT_FAILURE, 0,
           _("cannot specify both printing across and printing in parallel"));

/* Translate some old short options to new/long options.
   To meet downward compatibility with other UNIX pr utilities
   and some POSIX specifications. */

  જો (old_options)
    {
      જો (old_w)
        {
          જો (parallel_files || explicit_columns)
            {
              /* activate -W */
              truncate_લીટીઓ = true;
              જો (old_s)
                /* adapt HP-UX and SunOS: -s = no separator;
                   activate -S */
                use_col_separator = true;
            }
          નહીંતર
            /* old -w sets width with columns only
               activate -J */
            join_લીટીઓ = true;
        }
      નહીંતર જો (!use_col_separator)
        {
          /* No -S option read */
          જો (old_s && (parallel_files || explicit_columns))
            {
              જો (!truncate_લીટીઓ)
                {
                  /* old -s (without -w and -W) annuls column alignment,
                  uses fields, activate -J */
                  join_લીટીઓ = true;
                  જો (col_sep_length > 0)
                    /* activate -S */
                    use_col_separator = true;
                }
              નહીંતર
                /* with -W */
                /* adapt HP-UX and SunOS: -s = no separator;
                   activate -S */
                use_col_separator = true;
            }
        }
    }

  માટે (; optind < argc; optind++)
    {
      file_names[n_files++] = argv[optind];
    }

  જો (n_files == 0)
    {
      /* No file arguments specified;  read from standard input.  */
      print_files (0, ખાલી);
    }
  નહીંતર
    {
      જો (parallel_files)
        print_files (n_files, file_names);
      નહીંતર
        {
          માટે (unsigned પૂર્ણાંક i = 0; i < n_files; i++)
            print_files (1, &file_names[i]);
        }
    }

  cleanup ();

  જો (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, _("standard input"));
  main_exit (failed_opens ? EXIT_FAILURE : EXIT_SUCCESS);
}

/* Parse numeric arguments, ensuring MIN <= number <= INT_MAX.  */

સ્થિર પૂર્ણાંક
getoptnum (અક્ષર અચલ *n_str, પૂર્ણાંક min, અક્ષર અચલ *err)
{
  પાછા xnumtoimax (n_str, 10, min, INT_MAX, "", err, 0,
                     min <= 0 ? 0 : XTOINT_MIN_RANGE);
}

/* Parse options of the form -scNNN.

   Example: -nck, where 'n' is the option, c is the optional number
   separator, and k is the optional width of the field used when printing
   a number. */

સ્થિર શૂન્ય
getoptarg (અક્ષર *arg, અક્ષર switch_char, અક્ષર *character, પૂર્ણાંક *સંખ્યા)
{
  અક્ષર અચલ *અંક = arg;

  જો (!*અંક)
    {
      error (0, 0, _("'-%c': Invalid argument: %s"), switch_char, quote (અંક));
      usage (EXIT_FAILURE);
    }

  જો (!c_isdigit (*અંક))
    *character = *અંક++;
  જો (*અંક)
    {
      long પૂર્ણાંક tmp_long;
      strtol_error e = xstrtol (અંક, ખાલી, 10, &tmp_long, "");
      જો (e == LONGINT_OK)
        {
          જો (tmp_long <= 0)
            e = LONGINT_INVALID;
          નહીંતર જો (INT_MAX < tmp_long)
            e = LONGINT_OVERFLOW;
        }
      જો (e != LONGINT_OK)
        {
          error (0, e & LONGINT_OVERFLOW ? EOVERFLOW : 0,
             _("'-%c' extra characters or invalid number in the argument: %s"),
                 switch_char, quote (arg));
          usage (EXIT_FAILURE);
        }
      *સંખ્યા = tmp_long;
    }
}

/* Set parameters related to formatting. */

સ્થિર શૂન્ય
init_parameters (પૂર્ણાંક સંખ્યા_of_files)
{
  પૂર્ણાંક chars_used_by_સંખ્યા = 0;

  લીટીઓ_per_body = લીટીઓ_per_page - લીટીઓ_per_header - લીટીઓ_per_footer;
  જો (લીટીઓ_per_body <= 0)
    {
      extremities = false;
      keep_FF = true;
    }
  જો (! extremities)
    લીટીઓ_per_body = લીટીઓ_per_page;

  જો (double_space)
    લીટીઓ_per_body = MAX (1, લીટીઓ_per_body / 2);

  /* If input is stdin, cannot print parallel files.  BSD dumps core
     on this. */
  જો (સંખ્યા_of_files == 0)
    parallel_files = false;

  જો (parallel_files)
    columns = સંખ્યા_of_files;

  /* One file, multi columns down: -b option is set to get a consistent
     formulation with "FF set by hand" in input files. */
  જો (storing_columns)
    balance_columns = true;

  /* Tabification is assumed for multiple columns. */
  જો (columns > 1)
    {
      જો (!use_col_separator)
        {
          /* Use default separator */
          જો (join_લીટીઓ)
            col_sep_string = લીટી_separator;
          નહીંતર
            col_sep_string = column_separator;

          col_sep_length = 1;
          use_col_separator = true;
        }
      /* It's rather pointless to define a TAB separator with column
         alignment */
      નહીંતર જો (!join_લીટીઓ && col_sep_length == 1 && *col_sep_string == '\t')
        col_sep_string = column_separator;

      truncate_લીટીઓ = true;
      જો (! (col_sep_length == 1 && *col_sep_string == '\t'))
        untabify_આગત = true;
      tabify_output = true;
    }
  નહીંતર
    storing_columns = false;

  /* -J dominates -w in any case */
  જો (join_લીટીઓ)
    truncate_લીટીઓ = false;

  જો (numbered_લીટીઓ)
    {
      પૂર્ણાંક chars_per_default_tab = 8;

      લીટી_ગણતરી = start_લીટી_અંક;

      /* To allow input tab-expansion (-e sensitive) use:
         if (number_separator == input_tab_char)
           number_width = chars_per_number
             + TAB_WIDTH (chars_per_input_tab, chars_per_number);   */

      /* Estimate chars_per_text without any margin and keep it constant. */
      જો (સંખ્યા_separator == '\t')
        સંખ્યા_width = (chars_per_સંખ્યા
                        + TAB_WIDTH (chars_per_default_tab, chars_per_સંખ્યા));
      નહીંતર
        સંખ્યા_width = chars_per_સંખ્યા + 1;

      /* The number is part of the column width unless we are
         printing files in parallel. */
      જો (parallel_files)
        chars_used_by_સંખ્યા = સંખ્યા_width;
    }

  પૂર્ણાંક sep_chars, useful_chars;
  જો (ckd_mul (&sep_chars, columns - 1, col_sep_length))
    sep_chars = INT_MAX;
  જો (ckd_sub (&useful_chars, chars_per_લીટી - chars_used_by_સંખ્યા,
               sep_chars))
    useful_chars = 0;
  chars_per_column = useful_chars / columns;

  જો (chars_per_column < 1)
    error (EXIT_FAILURE, 0, _("page width too narrow"));

  જો (numbered_લીટીઓ)
    {
      મુક્ત (સંખ્યા_buff);
      સંખ્યા_buff = xmalloc (MAX (chars_per_સંખ્યા,
                                  INT_STRLEN_BOUND (લીટી_સંખ્યા)) + 1);
    }

  /* Pick the maximum between the tab width and the width of an
     escape sequence.
     The width of an escape sequence (4) isn't the lower limit any longer.
     We've to use 8 as the lower limit, if we use chars_per_default_tab = 8
     to expand a tab which is not an input_tab-char. */
  મુક્ત (clump_buff);
  clump_buff = xmalloc (MAX (8, chars_per_આગત_tab));
}

/* Open the necessary files,
   maintaining a COLUMN structure for each column.

   With multiple files, each column p has a different p->fp.
   With single files, each column p has the same p->fp.
   Return false if (number_of_files > 0) and no files can be opened,
   true otherwise.

   With each column/file p, p->full_page_printed is initialized,
   see also open_file.  */

સ્થિર bool
init_fps (પૂર્ણાંક સંખ્યા_of_files, અક્ષર **av)
{
  COLUMN *p;

  total_files = 0;

  મુક્ત (column_vector);
  column_vector = xnmalloc (columns, કદ (COLUMN));

  જો (parallel_files)
    {
      પૂર્ણાંક files_ડાબું = સંખ્યા_of_files;
      માટે (p = column_vector; files_ડાબું--; ++p, ++av)
        {
          જો (! open_file (*av, p))
            {
              --p;
              --columns;
            }
        }
      જો (columns == 0)
        પાછા false;
      init_header ("", -1);
    }
  નહીંતર
    {
      p = column_vector;
      જો (સંખ્યા_of_files > 0)
        {
          જો (! open_file (*av, p))
            પાછા false;
          init_header (*av, fileno (p->fp));
          p->લીટીઓ_stored = 0;
        }
      નહીંતર
        {
          p->name = _("standard input");
          p->fp = stdin;
          have_read_stdin = true;
          p->status = OPEN;
          p->full_page_printed = false;
          ++total_files;
          init_header ("", -1);
          p->લીટીઓ_stored = 0;
        }

      અક્ષર અચલ *firstname = p->name;
      FILE *firstfp = p->fp;
      પૂર્ણાંક i;
      માટે (i = columns - 1, ++p; i; --i, ++p)
        {
          p->name = firstname;
          p->fp = firstfp;
          p->status = OPEN;
          p->full_page_printed = false;
          p->લીટીઓ_stored = 0;
        }
    }
  files_ready_to_read = total_files;
  પાછા true;
}

/* Determine print_func and char_func, the functions
   used by each column for printing and/or storing.

   Determine the horizontal position desired when we begin
   printing a column (p->start_position). */

સ્થિર શૂન્ય
init_funcs (શૂન્ય)
{
  પૂર્ણાંક i, h, h_next;
  COLUMN *p;

  h = chars_per_margin;

  જો (!truncate_લીટીઓ)
    h_next = ANYWHERE;
  નહીંતર
    {
      /* When numbering lines of parallel files, we enlarge the
         first column to accommodate the number.  Looks better than
         the Sys V approach. */
      જો (parallel_files && numbered_લીટીઓ)
        h_next = h + chars_per_column + સંખ્યા_width;
      નહીંતર
        h_next = h + chars_per_column;
    }

  /* Enlarge p->start_position of first column to use the same form of
     padding_not_printed with all columns. */
  h = h + col_sep_length;

  /* This loop takes care of all but the rightmost column. */

  માટે (p = column_vector, i = 1; i < columns; ++p, ++i)
    {
      જો (storing_columns)	/* One file, multi columns down. */
        {
          p->char_func = store_char;
          p->print_func = print_stored;
        }
      નહીંતર
        /* One file, multi columns across; or parallel files.  */
        {
          p->char_func = print_char;
          p->print_func = read_લીટી;
        }

      /* Number only the first column when printing files in
         parallel. */
      p->numbered = numbered_લીટીઓ && (!parallel_files || i == 1);
      p->start_position = h;

      /* If we don't truncate lines, all start_positions are
         ANYWHERE, except the first column's start_position when
         using a margin. */

      જો (!truncate_લીટીઓ)
        {
          h = ANYWHERE;
          h_next = ANYWHERE;
        }
      નહીંતર
        {
          h = h_next + col_sep_length;
          h_next = h + chars_per_column;
        }
    }

  /* The rightmost column.

     Doesn't need to be stored unless we intend to balance
     columns on the last page. */
  જો (storing_columns && balance_columns)
    {
      p->char_func = store_char;
      p->print_func = print_stored;
    }
  નહીંતર
    {
      p->char_func = print_char;
      p->print_func = read_લીટી;
    }

  p->numbered = numbered_લીટીઓ && (!parallel_files || i == 1);
  p->start_position = h;
}

/* Open a file.  Return true if successful.

   With each file p, p->full_page_printed is initialized,
   see also init_fps. */

સ્થિર bool
open_file (અક્ષર *name, COLUMN *p)
{
  જો (streq (name, "-"))
    {
      p->name = _("standard input");
      p->fp = stdin;
      have_read_stdin = true;
    }
  નહીંતર
    {
      p->name = name;
      p->fp = fopen (name, "r");
    }
  જો (p->fp == ખાલી)
    {
      failed_opens = true;
      જો (!ignore_failed_opens)
        error (0, errno, "%s", quotef (name));
      પાછા false;
    }
  fadvise (p->fp, FADVISE_SEQUENTIAL);
  p->status = OPEN;
  p->full_page_printed = false;
  ++total_files;
  પાછા true;
}

/* Close the file in P.

   If we aren't dealing with multiple files in parallel, we change
   the status of all columns in the column list to reflect the close. */

સ્થિર શૂન્ય
close_file (COLUMN *p)
{
  COLUMN *q;
  પૂર્ણાંક i;

  જો (p->status == CLOSED)
    પાછા;

  પૂર્ણાંક err = errno;
  જો (!ferror (p->fp))
    err = 0;
  જો (fileno (p->fp) == STDIN_FILENO)
    clearerr (p->fp);
  નહીંતર જો (fclose (p->fp) != 0 && !err)
    err = errno;
  જો (err)
    error (EXIT_FAILURE, err, "%s", quotef (p->name));

  જો (!parallel_files)
    {
      માટે (q = column_vector, i = columns; i; ++q, --i)
        {
          q->status = CLOSED;
          જો (q->લીટીઓ_stored == 0)
            {
              q->લીટીઓ_to_print = 0;
            }
        }
    }
  નહીંતર
    {
      p->status = CLOSED;
      p->લીટીઓ_to_print = 0;
    }

  --files_ready_to_read;
}

/* Put a file on hold until we start a new page,
   since we've hit a form feed.

   If we aren't dealing with parallel files, we must change the
   status of all columns in the column list. */

સ્થિર શૂન્ય
hold_file (COLUMN *p)
{
  COLUMN *q;
  પૂર્ણાંક i;

  જો (!parallel_files)
    માટે (q = column_vector, i = columns; i; ++q, --i)
      {
        જો (storing_columns)
          q->status = FF_FOUND;
        નહીંતર
          q->status = ON_HOLD;
      }
  નહીંતર
    p->status = ON_HOLD;

  p->લીટીઓ_to_print = 0;
  --files_ready_to_read;
}

/* Undo hold_file -- go through the column list and change any
   ON_HOLD columns to OPEN.  Used at the end of each page. */

સ્થિર શૂન્ય
reset_status (શૂન્ય)
{
  પૂર્ણાંક i = columns;
  COLUMN *p;

  માટે (p = column_vector; i; --i, ++p)
    જો (p->status == ON_HOLD)
      {
        p->status = OPEN;
        files_ready_to_read++;
      }

  જો (storing_columns)
    {
      જો (column_vector->status == CLOSED)
        /* We use the info to output an error message in  skip_to_page. */
        files_ready_to_read = 0;
      નહીંતર
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

સ્થિર શૂન્ય
print_files (પૂર્ણાંક સંખ્યા_of_files, અક્ષર **av)
{
  init_parameters (સંખ્યા_of_files);
  જો (! init_fps (સંખ્યા_of_files, av))
    પાછા;
  જો (storing_columns)
    init_store_cols ();

  જો (first_page_સંખ્યા > 1)
    {
      જો (!skip_to_page (first_page_સંખ્યા))
        પાછા;
      નહીંતર
        page_સંખ્યા = first_page_સંખ્યા;
    }
  નહીંતર
    page_સંખ્યા = 1;

  init_funcs ();

  લીટી_સંખ્યા = લીટી_ગણતરી;
  જ્યારે (print_page ())
    ;
}

/* Initialize header information.
   If DESC is non-negative, it is a file descriptor open to
   FILENAME for reading.  */

સ્થિર શૂન્ય
init_header (અક્ષર અચલ *filename, પૂર્ણાંક desc)
{
  અક્ષર *buf = ખાલી;
  સંરચના stat st;
  સંરચના timespec t;
  પૂર્ણાંક ns;
  સંરચના tm tm;

  /* If parallel files or standard input, use current date. */
  જો (streq (filename, "-"))
    desc = -1;
  જો (0 <= desc && fstat (desc, &st) == 0)
    t = get_stat_mtime (&st);
  નહીંતર
    {
      સ્થિર સંરચના timespec timespec;
      જો (! timespec.tv_sec)
        gettime (&timespec);
      t = timespec;
    }

  ns = t.tv_nsec;
  જો (localtime_rz (localtz, &t.tv_sec, &tm))
    {
      ptrdiff_t len = nstrftime (ખાલી, MIN (PTRDIFF_MAX, SIZE_MAX),
                                 date_format, &tm, localtz, ns);
      જો (0 <= len)
        {
          buf = ximalloc (len + 1);
          nstrftime (buf, len + 1, date_format, &tm, localtz, ns);
        }
    }
  જો (!buf)
    {
      અક્ષર secbuf[INT_BUFSIZE_BOUND (intmax_t)];
      buf = xmalloc (કદ secbuf + MAX (10, INT_BUFSIZE_BOUND (પૂર્ણાંક)));
      sprintf (buf, "%s.%09d", timetostr (t.tv_sec, secbuf), ns);
    }

  મુક્ત (date_text);
  date_text = buf;
  file_text = custom_header ? custom_header : desc < 0 ? "" : filename;
  header_width_available = (chars_per_લીટી
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

સ્થિર શૂન્ય
init_page (શૂન્ય)
{
  પૂર્ણાંક j;
  COLUMN *p;

  જો (storing_columns)
    {
      store_columns ();
      માટે (j = columns - 1, p = column_vector; j; --j, ++p)
        {
          p->લીટીઓ_to_print = p->લીટીઓ_stored;
        }

      /* Last column. */
      જો (balance_columns)
        {
          p->લીટીઓ_to_print = p->લીટીઓ_stored;
        }
      /* Since we're not balancing columns, we don't need to store
         the rightmost column.   Read it straight from the file. */
      નહીંતર
        {
          જો (p->status == OPEN)
            {
              p->લીટીઓ_to_print = લીટીઓ_per_body;
            }
          નહીંતર
            p->લીટીઓ_to_print = 0;
        }
    }
  નહીંતર
    માટે (j = columns, p = column_vector; j; --j, ++p)
      જો (p->status == OPEN)
        {
          p->લીટીઓ_to_print = લીટીઓ_per_body;
        }
      નહીંતર
        p->લીટીઓ_to_print = 0;
}

/* Align empty columns and print separators.
   Empty columns will be formed by files with status ON_HOLD or CLOSED
   when printing multiple files in parallel. */

સ્થિર શૂન્ય
align_column (COLUMN *p)
{
  padding_not_printed = p->start_position;
  જો (col_sep_length < padding_not_printed)
    {
      pad_across_to (padding_not_printed - col_sep_length);
      padding_not_printed = ANYWHERE;
    }

  જો (use_col_separator)
    print_sep_string ();

  જો (p->numbered)
    add_લીટી_સંખ્યા (p);
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

સ્થિર bool
print_page (શૂન્ય)
{
  પૂર્ણાંક j;
  પૂર્ણાંક લીટીઓ_ડાબું_on_page;
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

  જો (cols_ready_to_print () == 0)
    પાછા false;

  જો (extremities)
    print_a_header = true;

  /* Don't pad unless we know a page was printed. */
  pad_vertically = false;
  pv = false;

  લીટીઓ_ડાબું_on_page = લીટીઓ_per_body;
  જો (double_space)
    લીટીઓ_ડાબું_on_page *= 2;

  જ્યારે (લીટીઓ_ડાબું_on_page > 0 && cols_ready_to_print () > 0)
    {
      output_position = 0;
      spaces_not_printed = 0;
      separators_not_printed = 0;
      pad_vertically = false;
      align_empty_cols = false;
      empty_લીટી = true;

      માટે (j = 1, p = column_vector; j <= columns; ++j, ++p)
        {
          આગત_position = 0;
          જો (p->લીટીઓ_to_print > 0 || p->status == FF_FOUND)
            {
              FF_only = false;
              padding_not_printed = p->start_position;
              જો (!(p->print_func) (p))
                read_rest_of_લીટી (p);
              pv |= pad_vertically;

              --p->લીટીઓ_to_print;
              જો (p->લીટીઓ_to_print <= 0)
                {
                  જો (cols_ready_to_print () == 0)
                    તોડો;
                }

              /* File p changed its status to ON_HOLD or CLOSED */
              જો (parallel_files && p->status != OPEN)
                {
                  જો (empty_લીટી)
                    align_empty_cols = true;
                  નહીંતર જો (p->status == CLOSED
                           || (p->status == ON_HOLD && FF_only))
                    align_column (p);
                }
            }
          નહીંતર જો (parallel_files)
            {
              /* File status ON_HOLD or CLOSED */
              જો (empty_લીટી)
                align_empty_cols = true;
              નહીંતર
                align_column (p);
            }

          /* We need it also with an empty column */
          જો (use_col_separator)
            ++separators_not_printed;
        }

      જો (pad_vertically)
        {
          putchar ('\n');
          --લીટીઓ_ડાબું_on_page;
        }

      જો (cols_ready_to_print () == 0 && !extremities)
        તોડો;

      જો (double_space && pv)
        {
          putchar ('\n');
          --લીટીઓ_ડાબું_on_page;
        }
    }

  જો (લીટીઓ_ડાબું_on_page == 0)
    માટે (j = 1, p = column_vector; j <= columns; ++j, ++p)
      જો (p->status == OPEN)
        p->full_page_printed = true;

  pad_vertically = pv;

  જો (pad_vertically && extremities)
    pad_down (લીટીઓ_ડાબું_on_page + લીટીઓ_per_footer);
  નહીંતર જો (keep_FF && print_a_FF)
    {
      putchar ('\f');
      print_a_FF = false;
    }

  જો (ferror (stdout))
    write_error ();

  જો (last_page_સંખ્યા < ++page_સંખ્યા)
    પાછા false;		/* Stop printing with LAST_PAGE */

  reset_status ();		/* Change ON_HOLD to OPEN. */

  પાછા true;			/* More pages to go. */
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

સ્થિર શૂન્ય
init_store_cols (શૂન્ય)
{
  /* Tune this.  */
  પૂર્ણાંક total_લીટીઓ, total_લીટીઓ_1, chars_per_column_1, chars_if_truncate;
  જો (ckd_mul (&total_લીટીઓ, લીટીઓ_per_body, columns)
      || ckd_add (&total_લીટીઓ_1, total_લીટીઓ, 1)
      || ckd_add (&chars_per_column_1, chars_per_column, 1)
      || ckd_mul (&chars_if_truncate, total_લીટીઓ, chars_per_column_1)
      || ckd_mul (&buff_allocated, chars_if_truncate, use_col_separator + 1))
    integer_overflow ();

  મુક્ત (લીટી_vector);
  /* FIXME: here's where it was allocated.  */
  લીટી_vector = xnmalloc (total_લીટીઓ_1, કદ *લીટી_vector);

  મુક્ત (end_vector);
  end_vector = xnmalloc (total_લીટીઓ, કદ *end_vector);

  મુક્ત (buff);
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

સ્થિર શૂન્ય
store_columns (શૂન્ય)
{
  પૂર્ણાંક i, j;
  unsigned પૂર્ણાંક લીટી = 0;
  unsigned પૂર્ણાંક buff_start;
  પૂર્ણાંક last_col;		/* The rightmost column which will be saved in buff */
  COLUMN *p;

  buff_current = 0;
  buff_start = 0;

  જો (balance_columns)
    last_col = columns;
  નહીંતર
    last_col = columns - 1;

  માટે (i = 1, p = column_vector; i <= last_col; ++i, ++p)
    p->લીટીઓ_stored = 0;

  માટે (i = 1, p = column_vector; i <= last_col && files_ready_to_read;
       ++i, ++p)
    {
      p->current_લીટી = લીટી;
      માટે (j = લીટીઓ_per_body; j && files_ready_to_read; --j)

        જો (p->status == OPEN)	/* Redundant.  Clean up. */
          {
            આગત_position = 0;

            જો (!read_લીટી (p))
              read_rest_of_લીટી (p);

            જો (p->status == OPEN
                || buff_start != buff_current)
              {
                ++p->લીટીઓ_stored;
                લીટી_vector[લીટી] = buff_start;
                end_vector[લીટી++] = આગત_position;
                buff_start = buff_current;
              }
          }
    }

  /* Keep track of the location of the last char in buff. */
  લીટી_vector[લીટી] = buff_start;

  જો (balance_columns)
    balance (લીટી);
}

સ્થિર શૂન્ય
balance (પૂર્ણાંક total_stored)
{
  COLUMN *p;
  પૂર્ણાંક i, લીટીઓ;
  પૂર્ણાંક first_લીટી = 0;

  માટે (i = 1, p = column_vector; i <= columns; ++i, ++p)
    {
      લીટીઓ = total_stored / columns;
      જો (i <= total_stored % columns)
        ++લીટીઓ;

      p->લીટીઓ_stored = લીટીઓ;
      p->current_લીટી = first_લીટી;

      first_લીટી += લીટીઓ;
    }
}

/* Store a character in the buffer. */

સ્થિર શૂન્ય
store_char (અક્ષર c)
{
  જો (buff_current >= buff_allocated)
    {
      /* May be too generous. */
      buff = xpalloc (buff, &buff_allocated, 1, -1, કદ *buff);
    }
  buff[buff_current++] = c;
}

સ્થિર શૂન્ય
add_લીટી_સંખ્યા (COLUMN *p)
{
  પૂર્ણાંક i;
  અક્ષર *s;
  પૂર્ણાંક અંક_width;

  /* Cutting off the higher-order digits is more informative than
     lower-order cut off. */
  અંક_width = sprintf (સંખ્યા_buff, "%*d", chars_per_સંખ્યા, લીટી_સંખ્યા);
  લીટી_સંખ્યા++;
  s = સંખ્યા_buff + (અંક_width - chars_per_સંખ્યા);
  માટે (i = chars_per_સંખ્યા; i > 0; i--)
    (p->char_func) (*s++);

  જો (columns > 1)
    {
      /* Tabification is assumed for multiple columns, also for n-separators,
         but 'default n-separator = TAB' hasn't been given priority over
         equal column_width also specified by POSIX. */
      જો (સંખ્યા_separator == '\t')
        {
          i = સંખ્યા_width - chars_per_સંખ્યા;
          જ્યારે (i-- > 0)
            (p->char_func) (' ');
        }
      નહીંતર
        (p->char_func) (સંખ્યા_separator);
    }
  નહીંતર
    /* To comply with POSIX, we avoid any expansion of default TAB
       separator with a single column output. No column_width requirement
       has to be considered. */
    {
      (p->char_func) (સંખ્યા_separator);
      જો (સંખ્યા_separator == '\t')
        output_position = POS_AFTER_TAB (chars_per_output_tab,
                          output_position);
    }

  જો (truncate_લીટીઓ && !parallel_files)
    આગત_position += સંખ્યા_width;
}

/* Print (or store) padding until the current horizontal position
   is position. */

સ્થિર શૂન્ય
pad_across_to (પૂર્ણાંક position)
{
  પૂર્ણાંક h = output_position;

  જો (tabify_output)
    spaces_not_printed = position - output_position;
  નહીંતર
    {
      જ્યારે (++h <= position)
        putchar (' ');
      output_position = position;
    }
}

/* Pad to the bottom of the page.

   If the user has requested a formfeed, use one.
   Otherwise, use newlines. */

સ્થિર શૂન્ય
pad_down (unsigned પૂર્ણાંક લીટીઓ)
{
  જો (use_form_feed)
    putchar ('\f');
  નહીંતર
    માટે (unsigned પૂર્ણાંક i = લીટીઓ; i; --i)
      putchar ('\n');
}

/* Read the rest of the line.

   Read from the current column's file until an end of line is
   hit.  Used when we've truncated a line and we no longer need
   to print or store its characters. */

સ્થિર શૂન્ય
read_rest_of_લીટી (COLUMN *p)
{
  પૂર્ણાંક c;
  FILE *f = p->fp;

  જ્યારે ((c = getc (f)) != '\n')
    {
      જો (c == '\f')
        {
          જો ((c = getc (f)) != '\n')
            ungetc (c, f);
          જો (keep_FF)
            print_a_FF = true;
          hold_file (p);
          તોડો;
        }
      નહીંતર જો (c == EOF)
        {
          close_file (p);
          તોડો;
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

સ્થિર શૂન્ય
skip_read (COLUMN *p, પૂર્ણાંક column_સંખ્યા)
{
  પૂર્ણાંક c;
  FILE *f = p->fp;
  પૂર્ણાંક i;
  bool single_ff = false;
  COLUMN *q;

  /* Read 1st character in a line or any character succeeding a FF */
  જો ((c = getc (f)) == '\f' && p->full_page_printed)
    /* A FF-coincidence with a previous full_page_printed.
       To avoid an additional empty page, eliminate the FF */
    જો ((c = getc (f)) == '\n')
      c = getc (f);

  p->full_page_printed = false;

  /* 1st character a FF means a single FF without any printable
     characters. Don't count it as a line with -n option. */
  જો (c == '\f')
    single_ff = true;

  /* Preparing for a FF-coincidence: Maybe we finish that page
     without a FF found */
  જો (last_લીટી)
    p->full_page_printed = true;

  જ્યારે (c != '\n')
    {
      જો (c == '\f')
        {
          /* No FF-coincidence possible,
             no catching up of a FF-coincidence with next page */
          જો (last_લીટી)
            {
              જો (!parallel_files)
                માટે (q = column_vector, i = columns; i; ++q, --i)
                  q->full_page_printed = false;
              નહીંતર
                p->full_page_printed = false;
            }

          જો ((c = getc (f)) != '\n')
            ungetc (c, f);
          hold_file (p);
          તોડો;
        }
      નહીંતર જો (c == EOF)
        {
          close_file (p);
          તોડો;
        }
      c = getc (f);
    }

  જો (skip_ગણતરી)
    જો ((!parallel_files || column_સંખ્યા == 1) && !single_ff)
      ++લીટી_ગણતરી;
}

/* If we're tabifying output,

   When print_char encounters white space it keeps track
   of our desired horizontal position and delays printing
   until this function is called. */

સ્થિર શૂન્ય
print_white_space (શૂન્ય)
{
  પૂર્ણાંક h_new;
  પૂર્ણાંક h_old = output_position;
  પૂર્ણાંક goal = h_old + spaces_not_printed;

  જ્યારે (goal - h_old > 1
         && (h_new = POS_AFTER_TAB (chars_per_output_tab, h_old)) <= goal)
    {
      putchar (output_tab_char);
      h_old = h_new;
    }
  જ્યારે (++h_old <= goal)
    putchar (' ');

  output_position = goal;
  spaces_not_printed = 0;
}

/* Print column separators.

   We keep a count until we know that we'll be printing a line,
   then print_sep_string() is called. */

સ્થિર શૂન્ય
print_sep_string (શૂન્ય)
{
  અક્ષર અચલ *s = col_sep_string;
  પૂર્ણાંક l = col_sep_length;

  જો (separators_not_printed <= 0)
    {
      /* We'll be starting a line with chars_per_margin, anything else? */
      જો (spaces_not_printed > 0)
        print_white_space ();
    }
  નહીંતર
    {
      માટે (; separators_not_printed > 0; --separators_not_printed)
        {
          જ્યારે (l-- > 0)
            {
              /* 3 types of sep_strings: spaces only, spaces and chars,
              chars only */
              જો (*s == ' ')
                {
                  /* We're tabifying output; consecutive spaces in
                  sep_string may have to be converted to tabs */
                  s++;
                  ++spaces_not_printed;
                }
              નહીંતર
                {
                  જો (spaces_not_printed > 0)
                    print_white_space ();
                  putchar (*s++);
                  ++output_position;
                }
            }
          /* sep_string ends with some spaces */
          જો (spaces_not_printed > 0)
            print_white_space ();
        }
    }
}

/* Print (or store, depending on p->char_func) a clump of N
   characters. */

સ્થિર શૂન્ય
print_clump (COLUMN *p, પૂર્ણાંક n, અક્ષર *clump)
{
  જ્યારે (n--)
    (p->char_func) (*clump++);

  જો (ferror (stdout))
    write_error ();
}

/* Print a character.

   Update the following comment: process-char hasn't been used any
   longer.
   If we're tabifying, all tabs have been converted to spaces by
   process_char().  Keep a count of consecutive spaces, and when
   a nonspace is encountered, call print_white_space() to print the
   required number of tabs and spaces. */

સ્થિર શૂન્ય
print_char (અક્ષર c)
{
  જો (tabify_output)
    {
      જો (c == ' ')
        {
          ++spaces_not_printed;
          પાછા;
        }
      નહીંતર જો (spaces_not_printed > 0)
        print_white_space ();

      /* Nonprintables are assumed to have width 0, except '\b'. */
      જો (! isprint (to_uchar (c)))
        {
          જો (c == '\b')
            --output_position;
        }
      નહીંતર
        ++output_position;
    }
  putchar (c);
}

/* Skip to page PAGE before printing.
   PAGE may be larger than total number of pages. */

સ્થિર bool
skip_to_page (uintmax_t page)
{
  માટે (uintmax_t n = 1; n < page; ++n)
    {
      COLUMN *p;
      પૂર્ણાંક j;

      માટે (પૂર્ણાંક i = 1; i < લીટીઓ_per_body; ++i)
        {
          માટે (j = 1, p = column_vector; j <= columns; ++j, ++p)
            જો (p->status == OPEN)
              skip_read (p, j);
        }
      last_લીટી = true;
      માટે (j = 1, p = column_vector; j <= columns; ++j, ++p)
        જો (p->status == OPEN)
          skip_read (p, j);

      જો (storing_columns)	/* change FF_FOUND to ON_HOLD */
        માટે (j = 1, p = column_vector; j <= columns; ++j, ++p)
          જો (p->status != CLOSED)
            p->status = ON_HOLD;

      reset_status ();
      last_લીટી = false;

      જો (files_ready_to_read < 1)
        {
          /* It's very helpful, normally the total number of pages is
             not known in advance.  */
          error (0, 0,
                 _("starting page number %ju exceeds page count %ju"),
                 page, n);
          તોડો;
        }
    }
  પાછા files_ready_to_read > 0;
}

/* Print a header.

   Formfeeds are assumed to use up two lines at the beginning of
   the page. */

સ્થિર શૂન્ય
print_header (શૂન્ય)
{
  અક્ષર page_text[256 + INT_STRLEN_BOUND (page_સંખ્યા)];
  પૂર્ણાંક available_width;
  પૂર્ણાંક lhs_spaces;
  પૂર્ણાંક rhs_spaces;

  output_position = 0;
  pad_across_to (chars_per_margin);
  print_white_space ();

  જો (page_સંખ્યા == 0)
    error (EXIT_FAILURE, 0, _("page number overflow"));

  /* The translator must ensure that formatting the translation of
     "Page %ju" does not generate more than (sizeof page_text - 1)
     bytes.  */
  sprintf (page_text, _("Page %ju"), page_સંખ્યા);
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

સ્થિર bool
read_લીટી (COLUMN *p)
{
  પૂર્ણાંક c;
  પૂર્ણાંક chars IF_LINT (= 0);
  પૂર્ણાંક last_આગત_position;
  પૂર્ણાંક j, k;
  COLUMN *q;

  /* read 1st character in each line or any character succeeding a FF: */
  c = getc (p->fp);

  last_આગત_position = આગત_position;

  જો (c == '\f' && p->full_page_printed)
    જો ((c = getc (p->fp)) == '\n')
      c = getc (p->fp);
  p->full_page_printed = false;

  પસંદગી (c)
    {
    સ્થિતિ '\f':
      જો ((c = getc (p->fp)) != '\n')
        ungetc (c, p->fp);
      FF_only = true;
      જો (print_a_header && !storing_columns)
        {
          pad_vertically = true;
          print_header ();
        }
      નહીંતર જો (keep_FF)
        print_a_FF = true;
      hold_file (p);
      પાછા true;
    સ્થિતિ EOF:
      close_file (p);
      પાછા true;
    સ્થિતિ '\n':
      તોડો;
    મૂળભૂત:
      chars = char_to_clump (c);
    }

  જો (truncate_લીટીઓ && આગત_position > chars_per_column)
    {
      આગત_position = last_આગત_position;
      પાછા false;
    }

  જો (p->char_func != store_char)
    {
      pad_vertically = true;

      જો (print_a_header && !storing_columns)
        print_header ();

      જો (parallel_files && align_empty_cols)
        {
          /* We have to align empty columns at the beginning of a line. */
          k = separators_not_printed;
          separators_not_printed = 0;
          માટે (j = 1, q = column_vector; j <= k; ++j, ++q)
            {
              align_column (q);
              separators_not_printed += 1;
            }
          padding_not_printed = p->start_position;
          જો (truncate_લીટીઓ)
            spaces_not_printed = chars_per_column;
          નહીંતર
            spaces_not_printed = 0;
          align_empty_cols = false;
        }

      જો (col_sep_length < padding_not_printed)
        {
          pad_across_to (padding_not_printed - col_sep_length);
          padding_not_printed = ANYWHERE;
        }

      જો (use_col_separator)
        print_sep_string ();
    }

  જો (p->numbered)
    add_લીટી_સંખ્યા (p);

  empty_લીટી = false;
  જો (c == '\n')
    પાછા true;

  print_clump (p, chars, clump_buff);

  જ્યારે (true)
    {
      c = getc (p->fp);

      પસંદગી (c)
        {
        સ્થિતિ '\n':
          પાછા true;
        સ્થિતિ '\f':
          જો ((c = getc (p->fp)) != '\n')
            ungetc (c, p->fp);
          જો (keep_FF)
            print_a_FF = true;
          hold_file (p);
          પાછા true;
        સ્થિતિ EOF:
          close_file (p);
          પાછા true;
        }

      last_આગત_position = આગત_position;
      chars = char_to_clump (c);
      જો (truncate_લીટીઓ && આગત_position > chars_per_column)
        {
          આગત_position = last_આગત_position;
          પાછા false;
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

સ્થિર bool
print_stored (COLUMN *p)
{
  COLUMN *q;

  પૂર્ણાંક લીટી = p->current_લીટી++;
  અક્ષર *first = &buff[લીટી_vector[લીટી]];
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
  અક્ષર *last = &buff[લીટી_vector[લીટી + 1]];

  pad_vertically = true;

  જો (print_a_header)
    print_header ();

  જો (p->status == FF_FOUND)
    {
      પૂર્ણાંક i;
      માટે (i = 1, q = column_vector; i <= columns; ++i, ++q)
        q->status = ON_HOLD;
      જો (column_vector->લીટીઓ_to_print <= 0)
        {
          જો (!extremities)
            pad_vertically = false;
          પાછા true;		/* print a header only */
        }
    }

  જો (col_sep_length < padding_not_printed)
    {
      pad_across_to (padding_not_printed - col_sep_length);
      padding_not_printed = ANYWHERE;
    }

  જો (use_col_separator)
    print_sep_string ();

  જ્યારે (first != last)
    print_char (*first++);

  જો (spaces_not_printed == 0)
    {
      output_position = p->start_position + end_vector[લીટી];
      જો (p->start_position - col_sep_length == chars_per_margin)
        output_position -= col_sep_length;
    }

  પાછા true;
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

સ્થિર પૂર્ણાંક
char_to_clump (અક્ષર c)
{
  unsigned અક્ષર uc = c;
  અક્ષર *s = clump_buff;
  અક્ષર esc_buff[4];
  પૂર્ણાંક width;
  પૂર્ણાંક chars;
  પૂર્ણાંક chars_per_c = 8;

  જો (c == આગત_tab_char)
    chars_per_c = chars_per_આગત_tab;

  જો (c == આગત_tab_char || c == '\t')
    {
      width = TAB_WIDTH (chars_per_c, આગત_position);

      જો (untabify_આગત)
        {
          માટે (પૂર્ણાંક i = width; i; --i)
            *s++ = ' ';
          chars = width;
        }
      નહીંતર
        {
          *s = c;
          chars = 1;
        }

    }
  નહીંતર જો (! isprint (uc))
    {
      જો (use_esc_sequence)
        {
          width = 4;
          chars = 4;
          *s++ = '\\';
          sprintf (esc_buff, "%03o", uc);
          માટે (પૂર્ણાંક i = 0; i <= 2; ++i)
            *s++ = esc_buff[i];
        }
      નહીંતર જો (use_cntrl_prefix)
        {
          જો (uc < 0200)
            {
              width = 2;
              chars = 2;
              *s++ = '^';
              *s = c ^ 0100;
            }
          નહીંતર
            {
              width = 4;
              chars = 4;
              *s++ = '\\';
              sprintf (esc_buff, "%03o", uc);
              માટે (પૂર્ણાંક i = 0; i <= 2; ++i)
                *s++ = esc_buff[i];
            }
        }
      નહીંતર જો (c == '\b')
        {
          width = -1;
          chars = 1;
          *s = c;
        }
      નહીંતર
        {
          width = 0;
          chars = 1;
          *s = c;
        }
    }
  નહીંતર
    {
      width = 1;
      chars = 1;
      *s = c;
    }

  /* Too many backspaces must put us in position 0 -- never negative.  */
  જો (width < 0 && આગત_position == 0)
    {
      chars = 0;
      આગત_position = 0;
    }
  નહીંતર જો (width < 0 && આગત_position <= -width)
    આગત_position = 0;
  નહીંતર
    {
      જો (ckd_add (&આગત_position, આગત_position, width))
        integer_overflow ();
    }

  પાછા chars;
}

/* We've just printed some files and need to clean up things before
   looking for more options and printing the next batch of files.

   Free everything we've xmalloc'ed, except 'header'. */

સ્થિર શૂન્ય
cleanup (શૂન્ય)
{
  મુક્ત (સંખ્યા_buff);
  મુક્ત (clump_buff);
  મુક્ત (column_vector);
  મુક્ત (લીટી_vector);
  મુક્ત (end_vector);
  મુક્ત (buff);
}

/* Complain, print a usage message, and die. */

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              program_name);

      fputs (_("\
Paginate or columnate FILE(s) માટે printing.\n\
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
         Balance સંખ્યા of લીટીઓ in the columns on each page\n\
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
  -d, --દ્વિગુણ-space\n\
         દ્વિગુણ space the output\n\
"));
      oputs (_("\
  -D, --date-format=FORMAT\n\
         use FORMAT માટે the header date\n\
"));
      oputs (_("\
  -e[CHAR[WIDTH]], --expand-tabs[=CHAR[WIDTH]]\n\
         expand આગત CHARs (TABs) to tab WIDTH (8)\n\
"));
      oputs (_("\
  -F, -f, --form-feed\n\
         use form feeds instead of newlines to separate pages\n\
         (by a 3-લીટી page header with -F or a 5-લીટી header\n\
         and trailer without -F)\n\
"));
      oputs (_("\
  -h, --header=HEADER\n\
         use a centered HEADER instead of filename in page header,\n\
         -h \"\" prints a blank લીટી, don't use -h\"\"\n\
"));
      oputs (_("\
  -i[CHAR[WIDTH]], --output-tabs[=CHAR[WIDTH]]\n\
         replace spaces with CHARs (TABs) to tab WIDTH (8)\n\
"));
      oputs (_("\
  -J, --join-લીટીઓ\n\
         merge full લીટીઓ, turns off -W લીટી truncation,\n\
         no column alignment, --sep-string[=STRING] sets separators\n\
"));
      oputs (_("\
  -l, --length=PAGE_LENGTH\n\
         set the page length to PAGE_LENGTH (66) લીટીઓ\n\
         (મૂળભૂત સંખ્યા of લીટીઓ of text 56, and with -F 63).\n\
         implies -t જો PAGE_LENGTH <= 10\n\
"));
      oputs (_("\
  -m, --merge\n\
         print all files in parallel, one in each column,\n\
         truncate લીટીઓ, but join લીટીઓ of full length with -J\n\
"));
      oputs (_("\
  -n[SEP[DIGITS]], --સંખ્યા-લીટીઓ[=SEP[DIGITS]]\n\
         સંખ્યા લીટીઓ, use DIGITS (5) digits, then SEP (TAB),\n\
         મૂળભૂત counting starts with 1st લીટી of આગત file\n\
"));
      oputs (_("\
  -N, --first-લીટી-સંખ્યા=સંખ્યા_2\n\
         start counting with સંખ્યા_2 at 1st લીટી of first\n\
         page printed (see +FIRST_PAGE)\n\
"));
      oputs (_("\
  -o, --indent=MARGIN\n\
         offset each લીટી with MARGIN (zero) spaces, કરો not\n\
         affect -w or -W, MARGIN will be added to PAGE_WIDTH\n\
"));
      oputs (_("\
  -r, --no-file-warnings\n\
         omit warning when a file cannot be opened\n\
"));
      oputs (_("\
  -s[CHAR], --separator[=CHAR]\n\
         separate columns by a single character, મૂળભૂત માટે CHAR\n\
         is the <TAB> character without -w and \'no અક્ષર\' with -w.\n\
         -s[CHAR] turns off લીટી truncation of all 3 column\n\
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
         omit page headers and trailers; implied જો PAGE_LENGTH <= 10\n\
"));
      oputs (_("\
  -T, --omit-pagination\n\
         omit page headers and trailers,\n\
         eliminate any pagination by form feeds set in આગત files\n\
"));
      oputs (_("\
  -v, --show-nonprinting\n\
         use octal backslash notation\n\
"));
      oputs (_("\
  -w, --width=PAGE_WIDTH\n\
         set page width to PAGE_WIDTH (72) characters માટે\n\
         multiple text-column output only, -s[અક્ષર] turns off (72)\n\
"));
      oputs (_("\
  -W, --page-width=PAGE_WIDTH\n\
         set page width to PAGE_WIDTH (72) characters always,\n\
         truncate લીટીઓ, except -J option is set,\n\
         no interference with -S or -s\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}
