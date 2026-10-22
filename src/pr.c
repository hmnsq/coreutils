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

#include <config.h>

#include <getopt.h>
#include <sys/types.h>
#include "system.h"
#include "c-ctype.h"
#include "fadvise.h"
#include "hard-locale.h"
#include "mbswidth.h"
#include "quote.h"
#include "stat-time.h"
#include "stdio--.h"
#include "strftime.h"
#include "xstrtol.h"
#include "xstrtol-error.h"
#include "xdectoint.h"

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

sa_nracanaa COLUMN;
sa_nracanaa COLUMN
  {
    FILE *fp;			/* Input stream for this column. */
    var_na const *name;		/* File name. */
    ga_nanaa
      {
        OPEN,
        FF_FOUND,		/* used with -b option, set with \f, changed
                                   to ON_HOLD after print_header */
        ON_HOLD,		/* Hit a form feed. */
        CLOSED
      }
    status;			/* Status of the file pointer. */

    /* Func to print lines in this col. */
    bool (*print_func) (sa_nracanaa COLUMN *);

    /* Func to print/store chars in this col. */
    void (*char_func) (var_na);

    ចំនួនគត់ current_line;		/* Index of current place in line_vector. */
    ចំនួនគត់ lines_stored;		/* Number of lines stored in buff. */
    ចំនួនគត់ lines_to_print;		/* No. lines stored or space left on page. */
    ចំនួនគត់ start_position;		/* Horizontal position of first char. */
    bool numbered;
    bool full_page_printed;	/* True means printed without a FF found. */

    /* p->full_page_printed  controls a special case of "FF set by hand":
       True means a full page has been printed without FF found. To avoid an
       additional empty page we have to ignore a FF immediately following in
       the next line. */
  };

typedef sa_nracanaa COLUMN COLUMN;

static ចំនួនគត់ char_to_clump (var_na c);
static bool read_line (COLUMN *p);
static bool print_page (void);
static bool print_stored (COLUMN *p);
static bool open_file (var_na *name, COLUMN *p);
static bool skip_to_page (uintmax_t page);
static void print_header (void);
static void pad_across_to (ចំនួនគត់ position);
static void add_line_number (COLUMN *p);
static ចំនួនគត់ getoptnum (var_na const *n_str, ចំនួនគត់ min, var_na const *errfmt);
static void getoptarg (var_na *arg, var_na switch_char, var_na *character,
                       ចំនួនគត់ *number);
static void print_files (ចំនួនគត់ number_of_files, var_na **av);
static void init_parameters (ចំនួនគត់ number_of_files);
static void init_header (var_na const *filename, ចំនួនគត់ desc);
static bool init_fps (ចំនួនគត់ number_of_files, var_na **av);
static void init_funcs (void);
static void init_store_cols (void);
static void store_columns (void);
static void balance (ចំនួនគត់ total_stored);
static void store_char (var_na c);
static void pad_down (unsigned ចំនួនគត់ lines);
static void read_rest_of_line (COLUMN *p);
static void skip_read (COLUMN *p, ចំនួនគត់ column_number);
static void print_char (var_na c);
static void cleanup (void);
static void print_sep_string (void);
static void separator_string (var_na const *optarg_S);

/* All of the columns to print.  */
static COLUMN *column_vector;

/* When printing a single file in multiple downward columns,
   we store the leftmost columns contiguously in buff.
   To print a line from buff, get the index of the first character
   from line_vector[i], and print up to line_vector[i + 1]. */
static var_na *buff;

/* Index of the position in buff where the next character
   will be stored. */
static unsigned ចំនួនគត់ buff_current;

/* The number of characters in buff.
   Used for allocation of buff and to detect overflow of buff. */
static idx_t buff_allocated;

/* Array of indices into buff.
   Each entry is an index of the first character of a line.
   This is used when storing lines to facilitate shuffling when
   we do column balancing on the last page. */
static ចំនួនគត់ *line_vector;

/* Array of horizontal positions.
   For each line in line_vector, end_vector[line] is the horizontal
   position we are in after printing that line.  We keep track of this
   so that we know how much we need to pad to prepare for the next
   column. */
static ចំនួនគត់ *end_vector;

/* (-m) True means we're printing multiple files in parallel. */
static bool parallel_files = false;

/* (-m) True means a line starts with some empty columns (some files
   already CLOSED or ON_HOLD) which we have to align. */
static bool align_empty_cols;

/* (-m) True means we have not yet found any printable column in a line.
   align_empty_cols = true  has to be maintained. */
static bool empty_line;

/* (-m) False means printable column output precedes a form feed found.
   Column alignment is done only once. No additional action with that form
   feed.
   True means we found only a form feed in a column. Maybe we have to do
   some column alignment with that form feed. */
static bool FF_only;

/* (-[0-9]+) True means we're given an option explicitly specifying
   number of columns.  Used to detect when this option is used with -m
   and when translating old options to new/long options. */
static bool explicit_columns = false;

/* (-t|-T) False means we aren't printing headers and footers. */
static bool extremities = true;

/* (-t) True means we retain all FF set by hand in input files.
   False is set with -T option. */
static bool keep_FF = false;
static bool print_a_FF = false;

/* True means we need to print a header as soon as we know we've got input
   to print after it. */
static bool print_a_header;

/* (-f) True means use formfeeds instead of newlines to separate pages. */
static bool use_form_feed = false;

/* True means we have read the standard input. */
static bool have_read_stdin = false;

/* True means the -a flag has been given. */
static bool print_across_flag = false;

/* True means we're printing one file in multiple (>1) downward columns. */
static bool storing_columns = true;

/* (-b) True means balance columns on the last page as Sys V does. */
/* That's no longer an independent option. With storing_columns = true
   balance_columns = true is used too (s. function init_parameters).
   We get a consistent formulation with "FF set by hand" in input files. */
static bool balance_columns = false;

/* (-l) Number of lines on a page, including header and footer lines. */
static ចំនួនគត់ lines_per_page = 66;

/* Number of lines in the header and footer can be reset to 0 using
   the -t flag. */
ga_nanaa { lines_per_header = 5 };
static ចំនួនគត់ lines_per_body;
ga_nanaa { lines_per_footer = 5 };

/* (-w|-W) Width in characters of the page.  Does not include the width of
   the margin. */
static ចំនួនគត់ chars_per_line = 72;

/* (-w|W) True means we truncate lines longer than chars_per_column. */
static bool truncate_lines = false;

/* (-J) True means we join lines without any line truncation. -J
   dominates -w option. */
static bool join_lines = false;

/* Number of characters in a column.  Based on col_sep_length and
   page width. */
static ចំនួនគត់ chars_per_column;

/* (-e) True means convert tabs to spaces on input. */
static bool untabify_input = false;

/* (-e) The input tab character. */
static var_na input_tab_char = '\t';

/* (-e) Tabstops are at chars_per_tab, 2*chars_per_tab, 3*chars_per_tab, ...
   where the leftmost column is 1. */
static ចំនួនគត់ chars_per_input_tab = 8;

/* (-i) True means convert spaces to tabs on output. */
static bool tabify_output = false;

/* (-i) The output tab character. */
static var_na output_tab_char = '\t';

/* (-i) The width of the output tab. */
static ចំនួនគត់ chars_per_output_tab = 8;

/* Keeps track of pending white space.  When we hit a nonspace
   character after some whitespace, we print whitespace, tabbing
   if necessary to get to output_position + spaces_not_printed. */
static ចំនួនគត់ spaces_not_printed;

/* (-o) Number of spaces in the left margin (tabs used when possible). */
static ចំនួនគត់ chars_per_margin = 0;

/* Position where the next character will fall.
   Leftmost position is 0 + chars_per_margin.
   Rightmost position is chars_per_margin + chars_per_line - 1.
   This is important for converting spaces to tabs on output. */
static ចំនួនគត់ output_position;

/* Horizontal position relative to the current file.
   (output_position depends on where we are on the page;
   input_position depends on where we are in the file.)
   Important for converting tabs to spaces on input. */
static ចំនួនគត់ input_position;

/* True if there were any failed opens so we can exit with nonzero
   status.  */
static bool failed_opens = false;

/* The number of spaces taken up if we print a tab character with width
   c_ from position h_. */
#define TAB_WIDTH(c_, h_) ((c_) - ((h_) % (c_)))

/* The horizontal position we'll be at after printing a tab character
   of width c_ from the position h_. */
#define POS_AFTER_TAB(c_, h_) ((h_) + TAB_WIDTH (c_, h_))

/* (-NNN) Number of columns of text to print. */
static ចំនួនគត់ columns = 1;

/* (+NNN:MMM) Page numbers on which to begin and stop printing.
   first_page_number = 0  will be used to check input only. */
static uintmax_t first_page_number = 0;
static uintmax_t last_page_number = UINTMAX_MAX;

/* Number of files open (not closed, not on hold). */
static ចំនួនគត់ files_ready_to_read = 0;

/* Current page number.  Displayed in header. */
static uintmax_t page_number;

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
static ចំនួនគត់ line_number;

/* (-n) True means lines should be preceded by numbers. */
static bool numbered_lines = false;

/* (-n) Character which follows each line number. */
static var_na number_separator = '\t';

/* (-n) line counting starts with 1st line of input file (not with 1st
   line of 1st page printed). */
static ចំនួនគត់ line_count = 1;

/* (-n) True means counting of skipped lines starts with 1st line of
   input file. False means -N option is used in addition, counting of
   skipped lines not required. */
static bool skip_count = true;

/* (-N) Counting starts with start_line_number = NUMBER at 1st line of
   first page printed, usually not 1st page of input file. */
static ចំនួនគត់ start_line_num = 1;

/* (-n) Width in characters of a line number. */
static ចំនួនគត់ chars_per_number = 5;

/* Used when widening the first column to accommodate numbers -- only
   needed when printing files in parallel.  Includes width of both the
   number and the number_separator. */
static ចំនួនគត់ number_width;

/* Buffer sprintf uses to format a line number. */
static var_na *number_buff;

/* (-v) True means nonprintable characters are printed as escape sequences.
   control-g becomes \007. */
static bool use_esc_sequence = false;

/* (-c) True means nonprintable characters are printed as control prefixes.
   control-g becomes ^G. */
static bool use_cntrl_prefix = false;

/* (-d) True means output is double spaced. */
static bool double_space = false;

/* Number of files opened initially in init_files.  Should be 1
   unless we're printing multiple files in parallel. */
static ចំនួនគត់ total_files = 0;

/* (-r) True means don't complain if we can't open a file. */
static bool ignore_failed_opens = false;

/* (-S) True means we separate columns with a specified string.
   -S option does not affect line truncation nor column alignment. */
static bool use_col_separator = false;

/* String used to separate columns if the -S option has been specified.
   Default without -S but together with one of the column options
   -a|COLUMN|-m is a 'space' and with the -J option a 'tab'. */
static var_na const *col_sep_string = "";
static ចំនួនគត់ col_sep_length = 0;
static var_na *column_separator = (var_na *) " ";
static var_na *line_separator = (var_na *) "\t";

/* Number of separator characters waiting to be printed as soon as we
   know that we have any input remaining to be printed. */
static ចំនួនគត់ separators_not_printed;

/* Position we need to pad to, as soon as we know that we have input
   remaining to be printed. */
static ចំនួនគត់ padding_not_printed;

/* True means we should pad the end of the page.  Remains false until we
   know we have a page to print. */
static bool pad_vertically;

/* (-h) String of characters used in place of the filename in the header. */
static var_na *custom_header;

/* (-D) Date format for the header.  */
static var_na const *date_format;

/* The local time zone rules, as per the TZ environment variable.  */
static timezone_t localtz;

/* Date and file name for the header.  */
static var_na *date_text;
static var_na const *file_text;

/* Output columns available, not counting the date and file name.  */
static ចំនួនគត់ header_width_available;

static var_na *clump_buff;

/* True means we read the line no. lines_per_body in skip_read
   called by skip_to_page. That variable controls the coincidence of a
   "FF set by hand" and "full_page_printed", see above the definition of
   structure COLUMN. */
static bool last_line = false;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ga_nanaa
{
  COLUMNS_OPTION = CHAR_MAX + 1,
  PAGES_OPTION
};

static var_na const short_options[] =
  "-0123456789D:FJN:S::TW:abcde::fh:i::l:mn::o:rs::tvw:";

static sa_nracanaa option const long_options[] =
{
  {"pages", required_argument, NULL, PAGES_OPTION},
  {"columns", required_argument, NULL, COLUMNS_OPTION},
  {"across", no_argument, NULL, 'a'},
  {"show-control-chars", no_argument, NULL, 'c'},
  {"double-space", no_argument, NULL, 'd'},
  {"date-format", required_argument, NULL, 'D'},
  {"expand-tabs", optional_argument, NULL, 'e'},
  {"form-feed", no_argument, NULL, 'f'},
  {"header", required_argument, NULL, 'h'},
  {"output-tabs", optional_argument, NULL, 'i'},
  {"join-lines", no_argument, NULL, 'J'},
  {"length", required_argument, NULL, 'l'},
  {"merge", no_argument, NULL, 'm'},
  {"number-lines", optional_argument, NULL, 'n'},
  {"first-line-number", required_argument, NULL, 'N'},
  {"indent", required_argument, NULL, 'o'},
  {"no-file-warnings", no_argument, NULL, 'r'},
  {"separator", optional_argument, NULL, 's'},
  {"sep-string", optional_argument, NULL, 'S'},
  {"omit-header", no_argument, NULL, 't'},
  {"omit-pagination", no_argument, NULL, 'T'},
  {"show-nonprinting", no_argument, NULL, 'v'},
  {"width", required_argument, NULL, 'w'},
  {"page-width", required_argument, NULL, 'W'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0}
};

static _Noreturn void
integer_overflow (void)
{
  error (EXIT_FAILURE, 0, _("integer overflow"));
}

/* Return the number of columns that have either an open file or
   stored lines. */

ATTRIBUTE_PURE
static unsigned ចំនួនគត់
cols_ready_to_print (void)
{
  COLUMN *q;
  unsigned ចំនួនគត់ i;
  unsigned ចំនួនគត់ n;

  n = 0;
  សម្រាប់ (q = column_vector, i = 0; i < columns; ++q, ++i)
    ប្រសិនបើ (q->status == OPEN
        || q->status == FF_FOUND	/* With -b: To print a header only */
        || (storing_columns && q->lines_stored > 0 && q->lines_to_print > 0))
      ++n;
  ត្រឡប់ n;
}

/* Estimate first_ / last_page_number
   using option +FIRST_PAGE:LAST_PAGE */

static bool
first_last_page (ចំនួនគត់ oi, var_na c, var_na const *pages)
{
  var_na *p;
  uintmax_t first;
  uintmax_t last = UINTMAX_MAX;
  strtol_error err = xstrtoumax (pages, &p, 10, &first, "");
  ប្រសិនបើ (err != LONGINT_OK && err != LONGINT_INVALID_SUFFIX_CHAR)
    xstrtol_fatal (err, oi, c, long_options, pages);

  ប្រសិនបើ (p == pages || !first)
    ត្រឡប់ false;

  ប្រសិនបើ (*p == ':')
    {
      var_na const *p1 = p + 1;
      err = xstrtoumax (p1, &p, 10, &last, "");
      ប្រសិនបើ (err != LONGINT_OK)
        xstrtol_fatal (err, oi, c, long_options, pages);
      ប្រសិនបើ (p1 == p || last < first)
        ត្រឡប់ false;
    }

  ប្រសិនបើ (*p)
    ត្រឡប់ false;

  first_page_number = first;
  last_page_number = last;
  ត្រឡប់ true;
}

/* Parse column count string S, and if it's valid (1 or larger and
   within range of the type of 'columns') set the global variables
   columns and explicit_columns.  Otherwise, exit with a diagnostic.  */

static void
parse_column_count (var_na const *s)
{
  columns = getoptnum (s, 1, _("invalid number of columns"));
  explicit_columns = true;
}

/* Estimate length of col_sep_string with option -S.  */

static void
separator_string (var_na const *optarg_S)
{
  size_t len = strlen (optarg_S);
  ប្រសិនបើ (INT_MAX < len)
    integer_overflow ();
  col_sep_length = len;
  col_sep_string = optarg_S;
}

ចំនួនគត់
main (ចំនួនគត់ argc, var_na **argv)
{
  unsigned ចំនួនគត់ n_files;
  bool old_options = false;
  bool old_w = false;
  bool old_s = false;
  var_na **file_names;

  /* Accumulate the digits of old-style options like -99.  */
  var_na *column_count_string = NULL;
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
                ? xnmalloc (argc - 1, parimaa_na (var_na *))
                : NULL);

  ខណៈពេល (true)
    {
      ចំនួនគត់ oi = -1;
      ចំនួនគត់ c = getopt_long (argc, argv, short_options, long_options, &oi);
      ប្រសិនបើ (c == -1)
        bha_nga;

      ប្រសិនបើ (c_isdigit (c))
        {
          /* Accumulate column-count digits specified via old-style options. */
          ប្រសិនបើ (n_digits + 1 >= n_alloc)
            column_count_string = xpalloc (column_count_string, &n_alloc, 2, -1,
                                           parimaa_na *column_count_string);
          column_count_string[n_digits++] = c;
          column_count_string[n_digits] = '\0';
          continue;
        }

      n_digits = 0;

      switch (c)
        {
        pak_sa 1:			/* Non-option argument. */
          /* long option --page dominates old '+FIRST_PAGE ...'.  */
          ប្រសិនបើ (! (first_page_number == 0
                 && *optarg == '+' && first_last_page (-2, '+', optarg + 1)))
            file_names[n_files++] = optarg;
          bha_nga;

        pak_sa PAGES_OPTION:	/* --pages=FIRST_PAGE[:LAST_PAGE] */
          {			/* dominates old opt +... */
            ប្រសិនបើ (! optarg)
              error (EXIT_FAILURE, 0,
                     _("'--pages=FIRST_PAGE[:LAST_PAGE]' missing argument"));
            បើមិនដូច្នេះ ប្រសិនបើ (! first_last_page (oi, 0, optarg))
              error (EXIT_FAILURE, 0, _("invalid page range %s"),
                     quote (optarg));
            bha_nga;
          }

        pak_sa COLUMNS_OPTION:	/* --columns=COLS */
          {
            parse_column_count (optarg);

            /* If there was a prior column count specified via the
               short-named option syntax, e.g., -9, ensure that this
               long-name-specified value overrides it.  */
            free (column_count_string);
            column_count_string = NULL;
            n_alloc = 0;
            bha_nga;
          }

        pak_sa 'a':
          print_across_flag = true;
          storing_columns = false;
          bha_nga;
        pak_sa 'b':
          balance_columns = true;
          bha_nga;
        pak_sa 'c':
          use_cntrl_prefix = true;
          bha_nga;
        pak_sa 'd':
          double_space = true;
          bha_nga;
        pak_sa 'D':
          date_format = optarg;
          bha_nga;
        pak_sa 'e':
          ប្រសិនបើ (optarg)
            getoptarg (optarg, 'e', &input_tab_char,
                       &chars_per_input_tab);
          /* Could check tab width > 0. */
          untabify_input = true;
          bha_nga;
        pak_sa 'f':
        pak_sa 'F':
          use_form_feed = true;
          bha_nga;
        pak_sa 'h':
          custom_header = optarg;
          bha_nga;
        pak_sa 'i':
          ប្រសិនបើ (optarg)
            getoptarg (optarg, 'i', &output_tab_char,
                       &chars_per_output_tab);
          /* Could check tab width > 0. */
          tabify_output = true;
          bha_nga;
        pak_sa 'J':
          join_lines = true;
          bha_nga;
        pak_sa 'l':
          lines_per_page
            = getoptnum (optarg, 1,
                         _("'-l PAGE_LENGTH' invalid number of lines"));
          bha_nga;
        pak_sa 'm':
          parallel_files = true;
          storing_columns = false;
          bha_nga;
        pak_sa 'n':
          numbered_lines = true;
          ប្រសិនបើ (optarg)
            getoptarg (optarg, 'n', &number_separator,
                       &chars_per_number);
          bha_nga;
        pak_sa 'N':
          skip_count = false;
          start_line_num
            = getoptnum (optarg, INT_MIN,
                         _("'-N NUMBER' invalid starting line number"));
          bha_nga;
        pak_sa 'o':
          chars_per_margin = getoptnum (optarg, 0,
                                        _("'-o MARGIN' invalid line offset"));
          bha_nga;
        pak_sa 'r':
          ignore_failed_opens = true;
          bha_nga;
        pak_sa 's':
          old_options = true;
          old_s = true;
          ប្រសិនបើ (!use_col_separator && optarg)
            separator_string (optarg);
          bha_nga;
        pak_sa 'S':
          old_s = false;
          /* Reset an additional input of -s, -S dominates -s */
          col_sep_string = "";
          col_sep_length = 0;
          use_col_separator = true;
          ប្រសិនបើ (optarg)
            separator_string (optarg);
          bha_nga;
        pak_sa 't':
          extremities = false;
          keep_FF = true;
          bha_nga;
        pak_sa 'T':
          extremities = false;
          keep_FF = false;
          bha_nga;
        pak_sa 'v':
          use_esc_sequence = true;
          bha_nga;
        pak_sa 'w':
          old_options = true;
          old_w = true;
          {
            ចំនួនគត់ tmp_cpl
              = getoptnum (optarg, 1,
                           _("'-w PAGE_WIDTH' invalid number of characters"));
            ប្រសិនបើ (! truncate_lines)
              chars_per_line = tmp_cpl;
          }
          bha_nga;
        pak_sa 'W':
          old_w = false;			/* dominates -w */
          truncate_lines = true;
          chars_per_line
            = getoptnum (optarg, 1,
                         _("'-W PAGE_WIDTH' invalid number of characters"));
          bha_nga;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        default:
          usage (EXIT_FAILURE);
          bha_nga;
        }
    }

  ប្រសិនបើ (column_count_string)
    {
      parse_column_count (column_count_string);
      free (column_count_string);
    }

  ប្រសិនបើ (! date_format)
    date_format = (getenv ("POSIXLY_CORRECT") && !hard_locale (LC_TIME)
                   ? "%b %e %H:%M %Y"
                   : "%Y-%m-%d %H:%M");

  localtz = tzalloc (getenv ("TZ"));

  /* Now we can set a reasonable initial value: */
  ប្រសិនបើ (first_page_number == 0)
    first_page_number = 1;

  ប្រសិនបើ (parallel_files && explicit_columns)
    error (EXIT_FAILURE, 0,
           _("cannot specify number of columns when printing in parallel"));

  ប្រសិនបើ (parallel_files && print_across_flag)
    error (EXIT_FAILURE, 0,
           _("cannot specify both printing across and printing in parallel"));

/* Translate some old short options to new/long options.
   To meet downward compatibility with other UNIX pr utilities
   and some POSIX specifications. */

  ប្រសិនបើ (old_options)
    {
      ប្រសិនបើ (old_w)
        {
          ប្រសិនបើ (parallel_files || explicit_columns)
            {
              /* activate -W */
              truncate_lines = true;
              ប្រសិនបើ (old_s)
                /* adapt HP-UX and SunOS: -s = no separator;
                   activate -S */
                use_col_separator = true;
            }
          បើមិនដូច្នេះ
            /* old -w sets width with columns only
               activate -J */
            join_lines = true;
        }
      បើមិនដូច្នេះ ប្រសិនបើ (!use_col_separator)
        {
          /* No -S option read */
          ប្រសិនបើ (old_s && (parallel_files || explicit_columns))
            {
              ប្រសិនបើ (!truncate_lines)
                {
                  /* old -s (without -w and -W) annuls column alignment,
                  uses fields, activate -J */
                  join_lines = true;
                  ប្រសិនបើ (col_sep_length > 0)
                    /* activate -S */
                    use_col_separator = true;
                }
              បើមិនដូច្នេះ
                /* with -W */
                /* adapt HP-UX and SunOS: -s = no separator;
                   activate -S */
                use_col_separator = true;
            }
        }
    }

  សម្រាប់ (; optind < argc; optind++)
    {
      file_names[n_files++] = argv[optind];
    }

  ប្រសិនបើ (n_files == 0)
    {
      /* No file arguments specified;  read from standard input.  */
      print_files (0, NULL);
    }
  បើមិនដូច្នេះ
    {
      ប្រសិនបើ (parallel_files)
        print_files (n_files, file_names);
      បើមិនដូច្នេះ
        {
          សម្រាប់ (unsigned ចំនួនគត់ i = 0; i < n_files; i++)
            print_files (1, &file_names[i]);
        }
    }

  cleanup ();

  ប្រសិនបើ (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, _("standard input"));
  main_exit (failed_opens ? EXIT_FAILURE : EXIT_SUCCESS);
}

/* Parse numeric arguments, ensuring MIN <= number <= INT_MAX.  */

static ចំនួនគត់
getoptnum (var_na const *n_str, ចំនួនគត់ min, var_na const *err)
{
  ត្រឡប់ xnumtoimax (n_str, 10, min, INT_MAX, "", err, 0,
                     min <= 0 ? 0 : XTOINT_MIN_RANGE);
}

/* Parse options of the form -scNNN.

   Example: -nck, where 'n' is the option, c is the optional number
   separator, and k is the optional width of the field used when printing
   a number. */

static void
getoptarg (var_na *arg, var_na switch_char, var_na *character, ចំនួនគត់ *number)
{
  var_na const *num = arg;

  ប្រសិនបើ (!*num)
    {
      error (0, 0, _("'-%c': Invalid argument: %s"), switch_char, quote (num));
      usage (EXIT_FAILURE);
    }

  ប្រសិនបើ (!c_isdigit (*num))
    *character = *num++;
  ប្រសិនបើ (*num)
    {
      long ចំនួនគត់ tmp_long;
      strtol_error e = xstrtol (num, NULL, 10, &tmp_long, "");
      ប្រសិនបើ (e == LONGINT_OK)
        {
          ប្រសិនបើ (tmp_long <= 0)
            e = LONGINT_INVALID;
          បើមិនដូច្នេះ ប្រសិនបើ (INT_MAX < tmp_long)
            e = LONGINT_OVERFLOW;
        }
      ប្រសិនបើ (e != LONGINT_OK)
        {
          error (0, e & LONGINT_OVERFLOW ? EOVERFLOW : 0,
             _("'-%c' extra characters or invalid number in the argument: %s"),
                 switch_char, quote (arg));
          usage (EXIT_FAILURE);
        }
      *number = tmp_long;
    }
}

/* Set parameters related to formatting. */

static void
init_parameters (ចំនួនគត់ number_of_files)
{
  ចំនួនគត់ chars_used_by_number = 0;

  lines_per_body = lines_per_page - lines_per_header - lines_per_footer;
  ប្រសិនបើ (lines_per_body <= 0)
    {
      extremities = false;
      keep_FF = true;
    }
  ប្រសិនបើ (! extremities)
    lines_per_body = lines_per_page;

  ប្រសិនបើ (double_space)
    lines_per_body = MAX (1, lines_per_body / 2);

  /* If input is stdin, cannot print parallel files.  BSD dumps core
     on this. */
  ប្រសិនបើ (number_of_files == 0)
    parallel_files = false;

  ប្រសិនបើ (parallel_files)
    columns = number_of_files;

  /* One file, multi columns down: -b option is set to get a consistent
     formulation with "FF set by hand" in input files. */
  ប្រសិនបើ (storing_columns)
    balance_columns = true;

  /* Tabification is assumed for multiple columns. */
  ប្រសិនបើ (columns > 1)
    {
      ប្រសិនបើ (!use_col_separator)
        {
          /* Use default separator */
          ប្រសិនបើ (join_lines)
            col_sep_string = line_separator;
          បើមិនដូច្នេះ
            col_sep_string = column_separator;

          col_sep_length = 1;
          use_col_separator = true;
        }
      /* It's rather pointless to define a TAB separator with column
         alignment */
      បើមិនដូច្នេះ ប្រសិនបើ (!join_lines && col_sep_length == 1 && *col_sep_string == '\t')
        col_sep_string = column_separator;

      truncate_lines = true;
      ប្រសិនបើ (! (col_sep_length == 1 && *col_sep_string == '\t'))
        untabify_input = true;
      tabify_output = true;
    }
  បើមិនដូច្នេះ
    storing_columns = false;

  /* -J dominates -w in any case */
  ប្រសិនបើ (join_lines)
    truncate_lines = false;

  ប្រសិនបើ (numbered_lines)
    {
      ចំនួនគត់ chars_per_default_tab = 8;

      line_count = start_line_num;

      /* To allow input tab-expansion (-e sensitive) use:
         if (number_separator == input_tab_char)
           number_width = chars_per_number
             + TAB_WIDTH (chars_per_input_tab, chars_per_number);   */

      /* Estimate chars_per_text without any margin and keep it constant. */
      ប្រសិនបើ (number_separator == '\t')
        number_width = (chars_per_number
                        + TAB_WIDTH (chars_per_default_tab, chars_per_number));
      បើមិនដូច្នេះ
        number_width = chars_per_number + 1;

      /* The number is part of the column width unless we are
         printing files in parallel. */
      ប្រសិនបើ (parallel_files)
        chars_used_by_number = number_width;
    }

  ចំនួនគត់ sep_chars, useful_chars;
  ប្រសិនបើ (ckd_mul (&sep_chars, columns - 1, col_sep_length))
    sep_chars = INT_MAX;
  ប្រសិនបើ (ckd_sub (&useful_chars, chars_per_line - chars_used_by_number,
               sep_chars))
    useful_chars = 0;
  chars_per_column = useful_chars / columns;

  ប្រសិនបើ (chars_per_column < 1)
    error (EXIT_FAILURE, 0, _("page width too narrow"));

  ប្រសិនបើ (numbered_lines)
    {
      free (number_buff);
      number_buff = xmalloc (MAX (chars_per_number,
                                  INT_STRLEN_BOUND (line_number)) + 1);
    }

  /* Pick the maximum between the tab width and the width of an
     escape sequence.
     The width of an escape sequence (4) isn't the lower limit any longer.
     We've to use 8 as the lower limit, if we use chars_per_default_tab = 8
     to expand a tab which is not an input_tab-char. */
  free (clump_buff);
  clump_buff = xmalloc (MAX (8, chars_per_input_tab));
}

/* Open the necessary files,
   maintaining a COLUMN structure for each column.

   With multiple files, each column p has a different p->fp.
   With single files, each column p has the same p->fp.
   Return false if (number_of_files > 0) and no files can be opened,
   true otherwise.

   With each column/file p, p->full_page_printed is initialized,
   see also open_file.  */

static bool
init_fps (ចំនួនគត់ number_of_files, var_na **av)
{
  COLUMN *p;

  total_files = 0;

  free (column_vector);
  column_vector = xnmalloc (columns, parimaa_na (COLUMN));

  ប្រសិនបើ (parallel_files)
    {
      ចំនួនគត់ files_left = number_of_files;
      សម្រាប់ (p = column_vector; files_left--; ++p, ++av)
        {
          ប្រសិនបើ (! open_file (*av, p))
            {
              --p;
              --columns;
            }
        }
      ប្រសិនបើ (columns == 0)
        ត្រឡប់ false;
      init_header ("", -1);
    }
  បើមិនដូច្នេះ
    {
      p = column_vector;
      ប្រសិនបើ (number_of_files > 0)
        {
          ប្រសិនបើ (! open_file (*av, p))
            ត្រឡប់ false;
          init_header (*av, fileno (p->fp));
          p->lines_stored = 0;
        }
      បើមិនដូច្នេះ
        {
          p->name = _("standard input");
          p->fp = stdin;
          have_read_stdin = true;
          p->status = OPEN;
          p->full_page_printed = false;
          ++total_files;
          init_header ("", -1);
          p->lines_stored = 0;
        }

      var_na const *firstname = p->name;
      FILE *firstfp = p->fp;
      ចំនួនគត់ i;
      សម្រាប់ (i = columns - 1, ++p; i; --i, ++p)
        {
          p->name = firstname;
          p->fp = firstfp;
          p->status = OPEN;
          p->full_page_printed = false;
          p->lines_stored = 0;
        }
    }
  files_ready_to_read = total_files;
  ត្រឡប់ true;
}

/* Determine print_func and char_func, the functions
   used by each column for printing and/or storing.

   Determine the horizontal position desired when we begin
   printing a column (p->start_position). */

static void
init_funcs (void)
{
  ចំនួនគត់ i, h, h_next;
  COLUMN *p;

  h = chars_per_margin;

  ប្រសិនបើ (!truncate_lines)
    h_next = ANYWHERE;
  បើមិនដូច្នេះ
    {
      /* When numbering lines of parallel files, we enlarge the
         first column to accommodate the number.  Looks better than
         the Sys V approach. */
      ប្រសិនបើ (parallel_files && numbered_lines)
        h_next = h + chars_per_column + number_width;
      បើមិនដូច្នេះ
        h_next = h + chars_per_column;
    }

  /* Enlarge p->start_position of first column to use the same form of
     padding_not_printed with all columns. */
  h = h + col_sep_length;

  /* This loop takes care of all but the rightmost column. */

  សម្រាប់ (p = column_vector, i = 1; i < columns; ++p, ++i)
    {
      ប្រសិនបើ (storing_columns)	/* One file, multi columns down. */
        {
          p->char_func = store_char;
          p->print_func = print_stored;
        }
      បើមិនដូច្នេះ
        /* One file, multi columns across; or parallel files.  */
        {
          p->char_func = print_char;
          p->print_func = read_line;
        }

      /* Number only the first column when printing files in
         parallel. */
      p->numbered = numbered_lines && (!parallel_files || i == 1);
      p->start_position = h;

      /* If we don't truncate lines, all start_positions are
         ANYWHERE, except the first column's start_position when
         using a margin. */

      ប្រសិនបើ (!truncate_lines)
        {
          h = ANYWHERE;
          h_next = ANYWHERE;
        }
      បើមិនដូច្នេះ
        {
          h = h_next + col_sep_length;
          h_next = h + chars_per_column;
        }
    }

  /* The rightmost column.

     Doesn't need to be stored unless we intend to balance
     columns on the last page. */
  ប្រសិនបើ (storing_columns && balance_columns)
    {
      p->char_func = store_char;
      p->print_func = print_stored;
    }
  បើមិនដូច្នេះ
    {
      p->char_func = print_char;
      p->print_func = read_line;
    }

  p->numbered = numbered_lines && (!parallel_files || i == 1);
  p->start_position = h;
}

/* Open a file.  Return true if successful.

   With each file p, p->full_page_printed is initialized,
   see also init_fps. */

static bool
open_file (var_na *name, COLUMN *p)
{
  ប្រសិនបើ (streq (name, "-"))
    {
      p->name = _("standard input");
      p->fp = stdin;
      have_read_stdin = true;
    }
  បើមិនដូច្នេះ
    {
      p->name = name;
      p->fp = fopen (name, "r");
    }
  ប្រសិនបើ (p->fp == NULL)
    {
      failed_opens = true;
      ប្រសិនបើ (!ignore_failed_opens)
        error (0, errno, "%s", quotef (name));
      ត្រឡប់ false;
    }
  fadvise (p->fp, FADVISE_SEQUENTIAL);
  p->status = OPEN;
  p->full_page_printed = false;
  ++total_files;
  ត្រឡប់ true;
}

/* Close the file in P.

   If we aren't dealing with multiple files in parallel, we change
   the status of all columns in the column list to reflect the close. */

static void
close_file (COLUMN *p)
{
  COLUMN *q;
  ចំនួនគត់ i;

  ប្រសិនបើ (p->status == CLOSED)
    ត្រឡប់;

  ចំនួនគត់ err = errno;
  ប្រសិនបើ (!ferror (p->fp))
    err = 0;
  ប្រសិនបើ (fileno (p->fp) == STDIN_FILENO)
    clearerr (p->fp);
  បើមិនដូច្នេះ ប្រសិនបើ (fclose (p->fp) != 0 && !err)
    err = errno;
  ប្រសិនបើ (err)
    error (EXIT_FAILURE, err, "%s", quotef (p->name));

  ប្រសិនបើ (!parallel_files)
    {
      សម្រាប់ (q = column_vector, i = columns; i; ++q, --i)
        {
          q->status = CLOSED;
          ប្រសិនបើ (q->lines_stored == 0)
            {
              q->lines_to_print = 0;
            }
        }
    }
  បើមិនដូច្នេះ
    {
      p->status = CLOSED;
      p->lines_to_print = 0;
    }

  --files_ready_to_read;
}

/* Put a file on hold until we start a new page,
   since we've hit a form feed.

   If we aren't dealing with parallel files, we must change the
   status of all columns in the column list. */

static void
hold_file (COLUMN *p)
{
  COLUMN *q;
  ចំនួនគត់ i;

  ប្រសិនបើ (!parallel_files)
    សម្រាប់ (q = column_vector, i = columns; i; ++q, --i)
      {
        ប្រសិនបើ (storing_columns)
          q->status = FF_FOUND;
        បើមិនដូច្នេះ
          q->status = ON_HOLD;
      }
  បើមិនដូច្នេះ
    p->status = ON_HOLD;

  p->lines_to_print = 0;
  --files_ready_to_read;
}

/* Undo hold_file -- go through the column list and change any
   ON_HOLD columns to OPEN.  Used at the end of each page. */

static void
reset_status (void)
{
  ចំនួនគត់ i = columns;
  COLUMN *p;

  សម្រាប់ (p = column_vector; i; --i, ++p)
    ប្រសិនបើ (p->status == ON_HOLD)
      {
        p->status = OPEN;
        files_ready_to_read++;
      }

  ប្រសិនបើ (storing_columns)
    {
      ប្រសិនបើ (column_vector->status == CLOSED)
        /* We use the info to output an error message in  skip_to_page. */
        files_ready_to_read = 0;
      បើមិនដូច្នេះ
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

static void
print_files (ចំនួនគត់ number_of_files, var_na **av)
{
  init_parameters (number_of_files);
  ប្រសិនបើ (! init_fps (number_of_files, av))
    ត្រឡប់;
  ប្រសិនបើ (storing_columns)
    init_store_cols ();

  ប្រសិនបើ (first_page_number > 1)
    {
      ប្រសិនបើ (!skip_to_page (first_page_number))
        ត្រឡប់;
      បើមិនដូច្នេះ
        page_number = first_page_number;
    }
  បើមិនដូច្នេះ
    page_number = 1;

  init_funcs ();

  line_number = line_count;
  ខណៈពេល (print_page ())
    ;
}

/* Initialize header information.
   If DESC is non-negative, it is a file descriptor open to
   FILENAME for reading.  */

static void
init_header (var_na const *filename, ចំនួនគត់ desc)
{
  var_na *buf = NULL;
  sa_nracanaa stat st;
  sa_nracanaa timespec t;
  ចំនួនគត់ ns;
  sa_nracanaa tm tm;

  /* If parallel files or standard input, use current date. */
  ប្រសិនបើ (streq (filename, "-"))
    desc = -1;
  ប្រសិនបើ (0 <= desc && fstat (desc, &st) == 0)
    t = get_stat_mtime (&st);
  បើមិនដូច្នេះ
    {
      static sa_nracanaa timespec timespec;
      ប្រសិនបើ (! timespec.tv_sec)
        gettime (&timespec);
      t = timespec;
    }

  ns = t.tv_nsec;
  ប្រសិនបើ (localtime_rz (localtz, &t.tv_sec, &tm))
    {
      ptrdiff_t len = nstrftime (NULL, MIN (PTRDIFF_MAX, SIZE_MAX),
                                 date_format, &tm, localtz, ns);
      ប្រសិនបើ (0 <= len)
        {
          buf = ximalloc (len + 1);
          nstrftime (buf, len + 1, date_format, &tm, localtz, ns);
        }
    }
  ប្រសិនបើ (!buf)
    {
      var_na secbuf[INT_BUFSIZE_BOUND (intmax_t)];
      buf = xmalloc (parimaa_na secbuf + MAX (10, INT_BUFSIZE_BOUND (ចំនួនគត់)));
      sprintf (buf, "%s.%09d", timetostr (t.tv_sec, secbuf), ns);
    }

  free (date_text);
  date_text = buf;
  file_text = custom_header ? custom_header : desc < 0 ? "" : filename;
  header_width_available = (chars_per_line
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

static void
init_page (void)
{
  ចំនួនគត់ j;
  COLUMN *p;

  ប្រសិនបើ (storing_columns)
    {
      store_columns ();
      សម្រាប់ (j = columns - 1, p = column_vector; j; --j, ++p)
        {
          p->lines_to_print = p->lines_stored;
        }

      /* Last column. */
      ប្រសិនបើ (balance_columns)
        {
          p->lines_to_print = p->lines_stored;
        }
      /* Since we're not balancing columns, we don't need to store
         the rightmost column.   Read it straight from the file. */
      បើមិនដូច្នេះ
        {
          ប្រសិនបើ (p->status == OPEN)
            {
              p->lines_to_print = lines_per_body;
            }
          បើមិនដូច្នេះ
            p->lines_to_print = 0;
        }
    }
  បើមិនដូច្នេះ
    សម្រាប់ (j = columns, p = column_vector; j; --j, ++p)
      ប្រសិនបើ (p->status == OPEN)
        {
          p->lines_to_print = lines_per_body;
        }
      បើមិនដូច្នេះ
        p->lines_to_print = 0;
}

/* Align empty columns and print separators.
   Empty columns will be formed by files with status ON_HOLD or CLOSED
   when printing multiple files in parallel. */

static void
align_column (COLUMN *p)
{
  padding_not_printed = p->start_position;
  ប្រសិនបើ (col_sep_length < padding_not_printed)
    {
      pad_across_to (padding_not_printed - col_sep_length);
      padding_not_printed = ANYWHERE;
    }

  ប្រសិនបើ (use_col_separator)
    print_sep_string ();

  ប្រសិនបើ (p->numbered)
    add_line_number (p);
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

static bool
print_page (void)
{
  ចំនួនគត់ j;
  ចំនួនគត់ lines_left_on_page;
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

  ប្រសិនបើ (cols_ready_to_print () == 0)
    ត្រឡប់ false;

  ប្រសិនបើ (extremities)
    print_a_header = true;

  /* Don't pad unless we know a page was printed. */
  pad_vertically = false;
  pv = false;

  lines_left_on_page = lines_per_body;
  ប្រសិនបើ (double_space)
    lines_left_on_page *= 2;

  ខណៈពេល (lines_left_on_page > 0 && cols_ready_to_print () > 0)
    {
      output_position = 0;
      spaces_not_printed = 0;
      separators_not_printed = 0;
      pad_vertically = false;
      align_empty_cols = false;
      empty_line = true;

      សម្រាប់ (j = 1, p = column_vector; j <= columns; ++j, ++p)
        {
          input_position = 0;
          ប្រសិនបើ (p->lines_to_print > 0 || p->status == FF_FOUND)
            {
              FF_only = false;
              padding_not_printed = p->start_position;
              ប្រសិនបើ (!(p->print_func) (p))
                read_rest_of_line (p);
              pv |= pad_vertically;

              --p->lines_to_print;
              ប្រសិនបើ (p->lines_to_print <= 0)
                {
                  ប្រសិនបើ (cols_ready_to_print () == 0)
                    bha_nga;
                }

              /* File p changed its status to ON_HOLD or CLOSED */
              ប្រសិនបើ (parallel_files && p->status != OPEN)
                {
                  ប្រសិនបើ (empty_line)
                    align_empty_cols = true;
                  បើមិនដូច្នេះ ប្រសិនបើ (p->status == CLOSED
                           || (p->status == ON_HOLD && FF_only))
                    align_column (p);
                }
            }
          បើមិនដូច្នេះ ប្រសិនបើ (parallel_files)
            {
              /* File status ON_HOLD or CLOSED */
              ប្រសិនបើ (empty_line)
                align_empty_cols = true;
              បើមិនដូច្នេះ
                align_column (p);
            }

          /* We need it also with an empty column */
          ប្រសិនបើ (use_col_separator)
            ++separators_not_printed;
        }

      ប្រសិនបើ (pad_vertically)
        {
          putchar ('\n');
          --lines_left_on_page;
        }

      ប្រសិនបើ (cols_ready_to_print () == 0 && !extremities)
        bha_nga;

      ប្រសិនបើ (double_space && pv)
        {
          putchar ('\n');
          --lines_left_on_page;
        }
    }

  ប្រសិនបើ (lines_left_on_page == 0)
    សម្រាប់ (j = 1, p = column_vector; j <= columns; ++j, ++p)
      ប្រសិនបើ (p->status == OPEN)
        p->full_page_printed = true;

  pad_vertically = pv;

  ប្រសិនបើ (pad_vertically && extremities)
    pad_down (lines_left_on_page + lines_per_footer);
  បើមិនដូច្នេះ ប្រសិនបើ (keep_FF && print_a_FF)
    {
      putchar ('\f');
      print_a_FF = false;
    }

  ប្រសិនបើ (ferror (stdout))
    write_error ();

  ប្រសិនបើ (last_page_number < ++page_number)
    ត្រឡប់ false;		/* Stop printing with LAST_PAGE */

  reset_status ();		/* Change ON_HOLD to OPEN. */

  ត្រឡប់ true;			/* More pages to go. */
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

static void
init_store_cols (void)
{
  /* Tune this.  */
  ចំនួនគត់ total_lines, total_lines_1, chars_per_column_1, chars_if_truncate;
  ប្រសិនបើ (ckd_mul (&total_lines, lines_per_body, columns)
      || ckd_add (&total_lines_1, total_lines, 1)
      || ckd_add (&chars_per_column_1, chars_per_column, 1)
      || ckd_mul (&chars_if_truncate, total_lines, chars_per_column_1)
      || ckd_mul (&buff_allocated, chars_if_truncate, use_col_separator + 1))
    integer_overflow ();

  free (line_vector);
  /* FIXME: here's where it was allocated.  */
  line_vector = xnmalloc (total_lines_1, parimaa_na *line_vector);

  free (end_vector);
  end_vector = xnmalloc (total_lines, parimaa_na *end_vector);

  free (buff);
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

static void
store_columns (void)
{
  ចំនួនគត់ i, j;
  unsigned ចំនួនគត់ line = 0;
  unsigned ចំនួនគត់ buff_start;
  ចំនួនគត់ last_col;		/* The rightmost column which will be saved in buff */
  COLUMN *p;

  buff_current = 0;
  buff_start = 0;

  ប្រសិនបើ (balance_columns)
    last_col = columns;
  បើមិនដូច្នេះ
    last_col = columns - 1;

  សម្រាប់ (i = 1, p = column_vector; i <= last_col; ++i, ++p)
    p->lines_stored = 0;

  សម្រាប់ (i = 1, p = column_vector; i <= last_col && files_ready_to_read;
       ++i, ++p)
    {
      p->current_line = line;
      សម្រាប់ (j = lines_per_body; j && files_ready_to_read; --j)

        ប្រសិនបើ (p->status == OPEN)	/* Redundant.  Clean up. */
          {
            input_position = 0;

            ប្រសិនបើ (!read_line (p))
              read_rest_of_line (p);

            ប្រសិនបើ (p->status == OPEN
                || buff_start != buff_current)
              {
                ++p->lines_stored;
                line_vector[line] = buff_start;
                end_vector[line++] = input_position;
                buff_start = buff_current;
              }
          }
    }

  /* Keep track of the location of the last char in buff. */
  line_vector[line] = buff_start;

  ប្រសិនបើ (balance_columns)
    balance (line);
}

static void
balance (ចំនួនគត់ total_stored)
{
  COLUMN *p;
  ចំនួនគត់ i, lines;
  ចំនួនគត់ first_line = 0;

  សម្រាប់ (i = 1, p = column_vector; i <= columns; ++i, ++p)
    {
      lines = total_stored / columns;
      ប្រសិនបើ (i <= total_stored % columns)
        ++lines;

      p->lines_stored = lines;
      p->current_line = first_line;

      first_line += lines;
    }
}

/* Store a character in the buffer. */

static void
store_char (var_na c)
{
  ប្រសិនបើ (buff_current >= buff_allocated)
    {
      /* May be too generous. */
      buff = xpalloc (buff, &buff_allocated, 1, -1, parimaa_na *buff);
    }
  buff[buff_current++] = c;
}

static void
add_line_number (COLUMN *p)
{
  ចំនួនគត់ i;
  var_na *s;
  ចំនួនគត់ num_width;

  /* Cutting off the higher-order digits is more informative than
     lower-order cut off. */
  num_width = sprintf (number_buff, "%*d", chars_per_number, line_number);
  line_number++;
  s = number_buff + (num_width - chars_per_number);
  សម្រាប់ (i = chars_per_number; i > 0; i--)
    (p->char_func) (*s++);

  ប្រសិនបើ (columns > 1)
    {
      /* Tabification is assumed for multiple columns, also for n-separators,
         but 'default n-separator = TAB' hasn't been given priority over
         equal column_width also specified by POSIX. */
      ប្រសិនបើ (number_separator == '\t')
        {
          i = number_width - chars_per_number;
          ខណៈពេល (i-- > 0)
            (p->char_func) (' ');
        }
      បើមិនដូច្នេះ
        (p->char_func) (number_separator);
    }
  បើមិនដូច្នេះ
    /* To comply with POSIX, we avoid any expansion of default TAB
       separator with a single column output. No column_width requirement
       has to be considered. */
    {
      (p->char_func) (number_separator);
      ប្រសិនបើ (number_separator == '\t')
        output_position = POS_AFTER_TAB (chars_per_output_tab,
                          output_position);
    }

  ប្រសិនបើ (truncate_lines && !parallel_files)
    input_position += number_width;
}

/* Print (or store) padding until the current horizontal position
   is position. */

static void
pad_across_to (ចំនួនគត់ position)
{
  ចំនួនគត់ h = output_position;

  ប្រសិនបើ (tabify_output)
    spaces_not_printed = position - output_position;
  បើមិនដូច្នេះ
    {
      ខណៈពេល (++h <= position)
        putchar (' ');
      output_position = position;
    }
}

/* Pad to the bottom of the page.

   If the user has requested a formfeed, use one.
   Otherwise, use newlines. */

static void
pad_down (unsigned ចំនួនគត់ lines)
{
  ប្រសិនបើ (use_form_feed)
    putchar ('\f');
  បើមិនដូច្នេះ
    សម្រាប់ (unsigned ចំនួនគត់ i = lines; i; --i)
      putchar ('\n');
}

/* Read the rest of the line.

   Read from the current column's file until an end of line is
   hit.  Used when we've truncated a line and we no longer need
   to print or store its characters. */

static void
read_rest_of_line (COLUMN *p)
{
  ចំនួនគត់ c;
  FILE *f = p->fp;

  ខណៈពេល ((c = getc (f)) != '\n')
    {
      ប្រសិនបើ (c == '\f')
        {
          ប្រសិនបើ ((c = getc (f)) != '\n')
            ungetc (c, f);
          ប្រសិនបើ (keep_FF)
            print_a_FF = true;
          hold_file (p);
          bha_nga;
        }
      បើមិនដូច្នេះ ប្រសិនបើ (c == EOF)
        {
          close_file (p);
          bha_nga;
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

static void
skip_read (COLUMN *p, ចំនួនគត់ column_number)
{
  ចំនួនគត់ c;
  FILE *f = p->fp;
  ចំនួនគត់ i;
  bool single_ff = false;
  COLUMN *q;

  /* Read 1st character in a line or any character succeeding a FF */
  ប្រសិនបើ ((c = getc (f)) == '\f' && p->full_page_printed)
    /* A FF-coincidence with a previous full_page_printed.
       To avoid an additional empty page, eliminate the FF */
    ប្រសិនបើ ((c = getc (f)) == '\n')
      c = getc (f);

  p->full_page_printed = false;

  /* 1st character a FF means a single FF without any printable
     characters. Don't count it as a line with -n option. */
  ប្រសិនបើ (c == '\f')
    single_ff = true;

  /* Preparing for a FF-coincidence: Maybe we finish that page
     without a FF found */
  ប្រសិនបើ (last_line)
    p->full_page_printed = true;

  ខណៈពេល (c != '\n')
    {
      ប្រសិនបើ (c == '\f')
        {
          /* No FF-coincidence possible,
             no catching up of a FF-coincidence with next page */
          ប្រសិនបើ (last_line)
            {
              ប្រសិនបើ (!parallel_files)
                សម្រាប់ (q = column_vector, i = columns; i; ++q, --i)
                  q->full_page_printed = false;
              បើមិនដូច្នេះ
                p->full_page_printed = false;
            }

          ប្រសិនបើ ((c = getc (f)) != '\n')
            ungetc (c, f);
          hold_file (p);
          bha_nga;
        }
      បើមិនដូច្នេះ ប្រសិនបើ (c == EOF)
        {
          close_file (p);
          bha_nga;
        }
      c = getc (f);
    }

  ប្រសិនបើ (skip_count)
    ប្រសិនបើ ((!parallel_files || column_number == 1) && !single_ff)
      ++line_count;
}

/* If we're tabifying output,

   When print_char encounters white space it keeps track
   of our desired horizontal position and delays printing
   until this function is called. */

static void
print_white_space (void)
{
  ចំនួនគត់ h_new;
  ចំនួនគត់ h_old = output_position;
  ចំនួនគត់ goal = h_old + spaces_not_printed;

  ខណៈពេល (goal - h_old > 1
         && (h_new = POS_AFTER_TAB (chars_per_output_tab, h_old)) <= goal)
    {
      putchar (output_tab_char);
      h_old = h_new;
    }
  ខណៈពេល (++h_old <= goal)
    putchar (' ');

  output_position = goal;
  spaces_not_printed = 0;
}

/* Print column separators.

   We keep a count until we know that we'll be printing a line,
   then print_sep_string() is called. */

static void
print_sep_string (void)
{
  var_na const *s = col_sep_string;
  ចំនួនគត់ l = col_sep_length;

  ប្រសិនបើ (separators_not_printed <= 0)
    {
      /* We'll be starting a line with chars_per_margin, anything else? */
      ប្រសិនបើ (spaces_not_printed > 0)
        print_white_space ();
    }
  បើមិនដូច្នេះ
    {
      សម្រាប់ (; separators_not_printed > 0; --separators_not_printed)
        {
          ខណៈពេល (l-- > 0)
            {
              /* 3 types of sep_strings: spaces only, spaces and chars,
              chars only */
              ប្រសិនបើ (*s == ' ')
                {
                  /* We're tabifying output; consecutive spaces in
                  sep_string may have to be converted to tabs */
                  s++;
                  ++spaces_not_printed;
                }
              បើមិនដូច្នេះ
                {
                  ប្រសិនបើ (spaces_not_printed > 0)
                    print_white_space ();
                  putchar (*s++);
                  ++output_position;
                }
            }
          /* sep_string ends with some spaces */
          ប្រសិនបើ (spaces_not_printed > 0)
            print_white_space ();
        }
    }
}

/* Print (or store, depending on p->char_func) a clump of N
   characters. */

static void
print_clump (COLUMN *p, ចំនួនគត់ n, var_na *clump)
{
  ខណៈពេល (n--)
    (p->char_func) (*clump++);

  ប្រសិនបើ (ferror (stdout))
    write_error ();
}

/* Print a character.

   Update the following comment: process-char hasn't been used any
   longer.
   If we're tabifying, all tabs have been converted to spaces by
   process_char().  Keep a count of consecutive spaces, and when
   a nonspace is encountered, call print_white_space() to print the
   required number of tabs and spaces. */

static void
print_char (var_na c)
{
  ប្រសិនបើ (tabify_output)
    {
      ប្រសិនបើ (c == ' ')
        {
          ++spaces_not_printed;
          ត្រឡប់;
        }
      បើមិនដូច្នេះ ប្រសិនបើ (spaces_not_printed > 0)
        print_white_space ();

      /* Nonprintables are assumed to have width 0, except '\b'. */
      ប្រសិនបើ (! isprint (to_uchar (c)))
        {
          ប្រសិនបើ (c == '\b')
            --output_position;
        }
      បើមិនដូច្នេះ
        ++output_position;
    }
  putchar (c);
}

/* Skip to page PAGE before printing.
   PAGE may be larger than total number of pages. */

static bool
skip_to_page (uintmax_t page)
{
  សម្រាប់ (uintmax_t n = 1; n < page; ++n)
    {
      COLUMN *p;
      ចំនួនគត់ j;

      សម្រាប់ (ចំនួនគត់ i = 1; i < lines_per_body; ++i)
        {
          សម្រាប់ (j = 1, p = column_vector; j <= columns; ++j, ++p)
            ប្រសិនបើ (p->status == OPEN)
              skip_read (p, j);
        }
      last_line = true;
      សម្រាប់ (j = 1, p = column_vector; j <= columns; ++j, ++p)
        ប្រសិនបើ (p->status == OPEN)
          skip_read (p, j);

      ប្រសិនបើ (storing_columns)	/* change FF_FOUND to ON_HOLD */
        សម្រាប់ (j = 1, p = column_vector; j <= columns; ++j, ++p)
          ប្រសិនបើ (p->status != CLOSED)
            p->status = ON_HOLD;

      reset_status ();
      last_line = false;

      ប្រសិនបើ (files_ready_to_read < 1)
        {
          /* It's very helpful, normally the total number of pages is
             not known in advance.  */
          error (0, 0,
                 _("starting page number %ju exceeds page count %ju"),
                 page, n);
          bha_nga;
        }
    }
  ត្រឡប់ files_ready_to_read > 0;
}

/* Print a header.

   Formfeeds are assumed to use up two lines at the beginning of
   the page. */

static void
print_header (void)
{
  var_na page_text[256 + INT_STRLEN_BOUND (page_number)];
  ចំនួនគត់ available_width;
  ចំនួនគត់ lhs_spaces;
  ចំនួនគត់ rhs_spaces;

  output_position = 0;
  pad_across_to (chars_per_margin);
  print_white_space ();

  ប្រសិនបើ (page_number == 0)
    error (EXIT_FAILURE, 0, _("page number overflow"));

  /* The translator must ensure that formatting the translation of
     "Page %ju" does not generate more than (sizeof page_text - 1)
     bytes.  */
  sprintf (page_text, _("Page %ju"), page_number);
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

static bool
read_line (COLUMN *p)
{
  ចំនួនគត់ c;
  ចំនួនគត់ chars IF_LINT (= 0);
  ចំនួនគត់ last_input_position;
  ចំនួនគត់ j, k;
  COLUMN *q;

  /* read 1st character in each line or any character succeeding a FF: */
  c = getc (p->fp);

  last_input_position = input_position;

  ប្រសិនបើ (c == '\f' && p->full_page_printed)
    ប្រសិនបើ ((c = getc (p->fp)) == '\n')
      c = getc (p->fp);
  p->full_page_printed = false;

  switch (c)
    {
    pak_sa '\f':
      ប្រសិនបើ ((c = getc (p->fp)) != '\n')
        ungetc (c, p->fp);
      FF_only = true;
      ប្រសិនបើ (print_a_header && !storing_columns)
        {
          pad_vertically = true;
          print_header ();
        }
      បើមិនដូច្នេះ ប្រសិនបើ (keep_FF)
        print_a_FF = true;
      hold_file (p);
      ត្រឡប់ true;
    pak_sa EOF:
      close_file (p);
      ត្រឡប់ true;
    pak_sa '\n':
      bha_nga;
    default:
      chars = char_to_clump (c);
    }

  ប្រសិនបើ (truncate_lines && input_position > chars_per_column)
    {
      input_position = last_input_position;
      ត្រឡប់ false;
    }

  ប្រសិនបើ (p->char_func != store_char)
    {
      pad_vertically = true;

      ប្រសិនបើ (print_a_header && !storing_columns)
        print_header ();

      ប្រសិនបើ (parallel_files && align_empty_cols)
        {
          /* We have to align empty columns at the beginning of a line. */
          k = separators_not_printed;
          separators_not_printed = 0;
          សម្រាប់ (j = 1, q = column_vector; j <= k; ++j, ++q)
            {
              align_column (q);
              separators_not_printed += 1;
            }
          padding_not_printed = p->start_position;
          ប្រសិនបើ (truncate_lines)
            spaces_not_printed = chars_per_column;
          បើមិនដូច្នេះ
            spaces_not_printed = 0;
          align_empty_cols = false;
        }

      ប្រសិនបើ (col_sep_length < padding_not_printed)
        {
          pad_across_to (padding_not_printed - col_sep_length);
          padding_not_printed = ANYWHERE;
        }

      ប្រសិនបើ (use_col_separator)
        print_sep_string ();
    }

  ប្រសិនបើ (p->numbered)
    add_line_number (p);

  empty_line = false;
  ប្រសិនបើ (c == '\n')
    ត្រឡប់ true;

  print_clump (p, chars, clump_buff);

  ខណៈពេល (true)
    {
      c = getc (p->fp);

      switch (c)
        {
        pak_sa '\n':
          ត្រឡប់ true;
        pak_sa '\f':
          ប្រសិនបើ ((c = getc (p->fp)) != '\n')
            ungetc (c, p->fp);
          ប្រសិនបើ (keep_FF)
            print_a_FF = true;
          hold_file (p);
          ត្រឡប់ true;
        pak_sa EOF:
          close_file (p);
          ត្រឡប់ true;
        }

      last_input_position = input_position;
      chars = char_to_clump (c);
      ប្រសិនបើ (truncate_lines && input_position > chars_per_column)
        {
          input_position = last_input_position;
          ត្រឡប់ false;
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

static bool
print_stored (COLUMN *p)
{
  COLUMN *q;

  ចំនួនគត់ line = p->current_line++;
  var_na *first = &buff[line_vector[line]];
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
  var_na *last = &buff[line_vector[line + 1]];

  pad_vertically = true;

  ប្រសិនបើ (print_a_header)
    print_header ();

  ប្រសិនបើ (p->status == FF_FOUND)
    {
      ចំនួនគត់ i;
      សម្រាប់ (i = 1, q = column_vector; i <= columns; ++i, ++q)
        q->status = ON_HOLD;
      ប្រសិនបើ (column_vector->lines_to_print <= 0)
        {
          ប្រសិនបើ (!extremities)
            pad_vertically = false;
          ត្រឡប់ true;		/* print a header only */
        }
    }

  ប្រសិនបើ (col_sep_length < padding_not_printed)
    {
      pad_across_to (padding_not_printed - col_sep_length);
      padding_not_printed = ANYWHERE;
    }

  ប្រសិនបើ (use_col_separator)
    print_sep_string ();

  ខណៈពេល (first != last)
    print_char (*first++);

  ប្រសិនបើ (spaces_not_printed == 0)
    {
      output_position = p->start_position + end_vector[line];
      ប្រសិនបើ (p->start_position - col_sep_length == chars_per_margin)
        output_position -= col_sep_length;
    }

  ត្រឡប់ true;
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

static ចំនួនគត់
char_to_clump (var_na c)
{
  unsigned var_na uc = c;
  var_na *s = clump_buff;
  var_na esc_buff[4];
  ចំនួនគត់ width;
  ចំនួនគត់ chars;
  ចំនួនគត់ chars_per_c = 8;

  ប្រសិនបើ (c == input_tab_char)
    chars_per_c = chars_per_input_tab;

  ប្រសិនបើ (c == input_tab_char || c == '\t')
    {
      width = TAB_WIDTH (chars_per_c, input_position);

      ប្រសិនបើ (untabify_input)
        {
          សម្រាប់ (ចំនួនគត់ i = width; i; --i)
            *s++ = ' ';
          chars = width;
        }
      បើមិនដូច្នេះ
        {
          *s = c;
          chars = 1;
        }

    }
  បើមិនដូច្នេះ ប្រសិនបើ (! isprint (uc))
    {
      ប្រសិនបើ (use_esc_sequence)
        {
          width = 4;
          chars = 4;
          *s++ = '\\';
          sprintf (esc_buff, "%03o", uc);
          សម្រាប់ (ចំនួនគត់ i = 0; i <= 2; ++i)
            *s++ = esc_buff[i];
        }
      បើមិនដូច្នេះ ប្រសិនបើ (use_cntrl_prefix)
        {
          ប្រសិនបើ (uc < 0200)
            {
              width = 2;
              chars = 2;
              *s++ = '^';
              *s = c ^ 0100;
            }
          បើមិនដូច្នេះ
            {
              width = 4;
              chars = 4;
              *s++ = '\\';
              sprintf (esc_buff, "%03o", uc);
              សម្រាប់ (ចំនួនគត់ i = 0; i <= 2; ++i)
                *s++ = esc_buff[i];
            }
        }
      បើមិនដូច្នេះ ប្រសិនបើ (c == '\b')
        {
          width = -1;
          chars = 1;
          *s = c;
        }
      បើមិនដូច្នេះ
        {
          width = 0;
          chars = 1;
          *s = c;
        }
    }
  បើមិនដូច្នេះ
    {
      width = 1;
      chars = 1;
      *s = c;
    }

  /* Too many backspaces must put us in position 0 -- never negative.  */
  ប្រសិនបើ (width < 0 && input_position == 0)
    {
      chars = 0;
      input_position = 0;
    }
  បើមិនដូច្នេះ ប្រសិនបើ (width < 0 && input_position <= -width)
    input_position = 0;
  បើមិនដូច្នេះ
    {
      ប្រសិនបើ (ckd_add (&input_position, input_position, width))
        integer_overflow ();
    }

  ត្រឡប់ chars;
}

/* We've just printed some files and need to clean up things before
   looking for more options and printing the next batch of files.

   Free everything we've xmalloc'ed, except 'header'. */

static void
cleanup (void)
{
  free (number_buff);
  free (clump_buff);
  free (column_vector);
  free (line_vector);
  free (end_vector);
  free (buff);
}

/* Complain, print a usage message, and die. */

void
usage (ចំនួនគត់ status)
{
  ប្រសិនបើ (status != EXIT_SUCCESS)
    emit_try_help ();
  បើមិនដូច្នេះ
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              program_name);

      fputs (_("\
Paginate or columnate FILE(s) សម្រាប់ printing.\n\
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
         Balance number of lines in the columns on each page\n\
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
  -d, --dviguu_na-space\n\
         dviguu_na space the output\n\
"));
      oputs (_("\
  -D, --date-format=FORMAT\n\
         use FORMAT សម្រាប់ the header date\n\
"));
      oputs (_("\
  -e[CHAR[WIDTH]], --expand-tabs[=CHAR[WIDTH]]\n\
         expand input CHARs (TABs) to tab WIDTH (8)\n\
"));
      oputs (_("\
  -F, -f, --form-feed\n\
         use form feeds instead of newlines to separate pages\n\
         (by a 3-line page header with -F or a 5-line header\n\
         and trailer without -F)\n\
"));
      oputs (_("\
  -h, --header=HEADER\n\
         use a centered HEADER instead of filename in page header,\n\
         -h \"\" prints a blank line, don't use -h\"\"\n\
"));
      oputs (_("\
  -i[CHAR[WIDTH]], --output-tabs[=CHAR[WIDTH]]\n\
         replace spaces with CHARs (TABs) to tab WIDTH (8)\n\
"));
      oputs (_("\
  -J, --join-lines\n\
         merge full lines, turns off -W line truncation,\n\
         no column alignment, --sep-string[=STRING] sets separators\n\
"));
      oputs (_("\
  -l, --length=PAGE_LENGTH\n\
         set the page length to PAGE_LENGTH (66) lines\n\
         (default number of lines of text 56, and with -F 63).\n\
         implies -t ប្រសិនបើ PAGE_LENGTH <= 10\n\
"));
      oputs (_("\
  -m, --merge\n\
         print all files in parallel, one in each column,\n\
         truncate lines, but join lines of full length with -J\n\
"));
      oputs (_("\
  -n[SEP[DIGITS]], --number-lines[=SEP[DIGITS]]\n\
         number lines, use DIGITS (5) digits, then SEP (TAB),\n\
         default counting starts with 1st line of input file\n\
"));
      oputs (_("\
  -N, --first-line-number=NUMBER\n\
         start counting with NUMBER at 1st line of first\n\
         page printed (see +FIRST_PAGE)\n\
"));
      oputs (_("\
  -o, --indent=MARGIN\n\
         offset each line with MARGIN (zero) spaces, do not\n\
         affect -w or -W, MARGIN will be added to PAGE_WIDTH\n\
"));
      oputs (_("\
  -r, --no-file-warnings\n\
         omit warning when a file cannot be opened\n\
"));
      oputs (_("\
  -s[CHAR], --separator[=CHAR]\n\
         separate columns by a single character, default សម្រាប់ CHAR\n\
         is the <TAB> character without -w and \'no var_na\' with -w.\n\
         -s[CHAR] turns off line truncation of all 3 column\n\
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
         omit page headers and trailers; implied ប្រសិនបើ PAGE_LENGTH <= 10\n\
"));
      oputs (_("\
  -T, --omit-pagination\n\
         omit page headers and trailers,\n\
         eliminate any pagination by form feeds set in input files\n\
"));
      oputs (_("\
  -v, --show-nonprinting\n\
         use octal backslash notation\n\
"));
      oputs (_("\
  -w, --width=PAGE_WIDTH\n\
         set page width to PAGE_WIDTH (72) characters សម្រាប់\n\
         multiple text-column output only, -s[var_na] turns off (72)\n\
"));
      oputs (_("\
  -W, --page-width=PAGE_WIDTH\n\
         set page width to PAGE_WIDTH (72) characters always,\n\
         truncate lines, except -J option is set,\n\
         no interference with -S or -s\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}
