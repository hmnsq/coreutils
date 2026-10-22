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

#समावेश <config.h>

#समावेश <getopt.h>
#समावेश <sys/types.h>
#समावेश "system.h"
#समावेश "c-ctype.h"
#समावेश "fadvise.h"
#समावेश "hard-locale.h"
#समावेश "mbswidth.h"
#समावेश "quote.h"
#समावेश "stat-time.h"
#समावेश "stdio--.h"
#समावेश "strftime.h"
#समावेश "xstrtol.h"
#समावेश "xstrtol-error.h"
#समावेश "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "pr"

#घोषणा AUTHORS \
  proper_name ("Pete TerMaat"), \
  proper_name ("Roland Huebner")

/* Used with start_position in the struct COLUMN described below.
   If start_position == ANYWHERE, we aren't truncating columns and
   can begin printing a column anywhere.  Otherwise we must pad to
   the horizontal position start_position. */
#घोषणा ANYWHERE	0

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

काष्ठा COLUMN;
काष्ठा COLUMN
  {
    खाता *fp;			/* Input stream for this column. */
    अक्षर स्थिर *name;		/* File name. */
    क्रमागत
      {
        OPEN,
        FF_FOUND,		/* used with -b option, set with \f, changed
                                   to ON_HOLD after print_header */
        ON_HOLD,		/* Hit a form feed. */
        CLOSED
      }
    status;			/* Status of the file pointer. */

    /* Func to print lines in this col. */
    bool (*print_func) (काष्ठा COLUMN *);

    /* Func to print/store chars in this col. */
    व्योम (*char_func) (अक्षर);

    पूर्णांक current_पंक्ति;		/* Index of current place in line_vector. */
    पूर्णांक पंक्तियाँ_stored;		/* Number of lines stored in buff. */
    पूर्णांक पंक्तियाँ_to_print;		/* No. lines stored or space left on page. */
    पूर्णांक start_position;		/* Horizontal position of first char. */
    bool numbered;
    bool full_page_printed;	/* True means printed without a FF found. */

    /* p->full_page_printed  controls a special case of "FF set by hand":
       True means a full page has been printed without FF found. To avoid an
       additional empty page we have to ignore a FF immediately following in
       the next line. */
  };

प्रकार काष्ठा COLUMN COLUMN;

अटल पूर्णांक char_to_clump (अक्षर c);
अटल bool read_पंक्ति (COLUMN *p);
अटल bool print_page (व्योम);
अटल bool print_stored (COLUMN *p);
अटल bool open_file (अक्षर *name, COLUMN *p);
अटल bool skip_to_page (uintmax_t page);
अटल व्योम print_header (व्योम);
अटल व्योम pad_across_to (पूर्णांक position);
अटल व्योम add_पंक्ति_संख्या (COLUMN *p);
अटल पूर्णांक getoptnum (अक्षर स्थिर *n_str, पूर्णांक min, अक्षर स्थिर *errfmt);
अटल व्योम getoptarg (अक्षर *arg, अक्षर switch_char, अक्षर *character,
                       पूर्णांक *संख्या);
अटल व्योम print_files (पूर्णांक संख्या_of_files, अक्षर **av);
अटल व्योम init_parameters (पूर्णांक संख्या_of_files);
अटल व्योम init_header (अक्षर स्थिर *filename, पूर्णांक desc);
अटल bool init_fps (पूर्णांक संख्या_of_files, अक्षर **av);
अटल व्योम init_funcs (व्योम);
अटल व्योम init_store_cols (व्योम);
अटल व्योम store_columns (व्योम);
अटल व्योम balance (पूर्णांक योग_stored);
अटल व्योम store_char (अक्षर c);
अटल व्योम pad_down (अचिन्हित पूर्णांक पंक्तियाँ);
अटल व्योम read_rest_of_पंक्ति (COLUMN *p);
अटल व्योम skip_read (COLUMN *p, पूर्णांक column_संख्या);
अटल व्योम print_char (अक्षर c);
अटल व्योम cleanup (व्योम);
अटल व्योम print_sep_string (व्योम);
अटल व्योम separator_string (अक्षर स्थिर *optarg_S);

/* All of the columns to print.  */
अटल COLUMN *column_vector;

/* When printing a single file in multiple downward columns,
   we store the leftmost columns contiguously in buff.
   To print a line from buff, get the index of the first character
   from line_vector[i], and print up to line_vector[i + 1]. */
अटल अक्षर *buff;

/* Index of the position in buff where the next character
   will be stored. */
अटल अचिन्हित पूर्णांक buff_current;

/* The number of characters in buff.
   Used for allocation of buff and to detect overflow of buff. */
अटल idx_t buff_allocated;

/* Array of indices into buff.
   Each entry is an index of the first character of a line.
   This is used when storing lines to facilitate shuffling when
   we do column balancing on the last page. */
अटल पूर्णांक *पंक्ति_vector;

/* Array of horizontal positions.
   For each line in line_vector, end_vector[line] is the horizontal
   position we are in after printing that line.  We keep track of this
   so that we know how much we need to pad to prepare for the next
   column. */
अटल पूर्णांक *end_vector;

/* (-m) True means we're printing multiple files in parallel. */
अटल bool parallel_files = false;

/* (-m) True means a line starts with some empty columns (some files
   already CLOSED or ON_HOLD) which we have to align. */
अटल bool align_empty_cols;

/* (-m) True means we have not yet found any printable column in a line.
   align_empty_cols = true  has to be maintained. */
अटल bool empty_पंक्ति;

/* (-m) False means printable column output precedes a form feed found.
   Column alignment is done only once. No additional action with that form
   feed.
   True means we found only a form feed in a column. Maybe we have to do
   some column alignment with that form feed. */
अटल bool FF_only;

/* (-[0-9]+) True means we're given an option explicitly specifying
   number of columns.  Used to detect when this option is used with -m
   and when translating old options to new/long options. */
अटल bool explicit_columns = false;

/* (-t|-T) False means we aren't printing headers and footers. */
अटल bool extremities = true;

/* (-t) True means we retain all FF set by hand in input files.
   False is set with -T option. */
अटल bool keep_FF = false;
अटल bool print_a_FF = false;

/* True means we need to print a header as soon as we know we've got input
   to print after it. */
अटल bool print_a_header;

/* (-f) True means use formfeeds instead of newlines to separate pages. */
अटल bool use_form_feed = false;

/* True means we have read the standard input. */
अटल bool have_read_stdin = false;

/* True means the -a flag has been given. */
अटल bool print_across_flag = false;

/* True means we're printing one file in multiple (>1) downward columns. */
अटल bool storing_columns = true;

/* (-b) True means balance columns on the last page as Sys V does. */
/* That's no longer an independent option. With storing_columns = true
   balance_columns = true is used too (s. function init_parameters).
   We get a consistent formulation with "FF set by hand" in input files. */
अटल bool balance_columns = false;

/* (-l) Number of lines on a page, including header and footer lines. */
अटल पूर्णांक पंक्तियाँ_per_page = 66;

/* Number of lines in the header and footer can be reset to 0 using
   the -t flag. */
क्रमागत { पंक्तियाँ_per_header = 5 };
अटल पूर्णांक पंक्तियाँ_per_body;
क्रमागत { पंक्तियाँ_per_footer = 5 };

/* (-w|-W) Width in characters of the page.  Does not include the width of
   the margin. */
अटल पूर्णांक chars_per_पंक्ति = 72;

/* (-w|W) True means we truncate lines longer than chars_per_column. */
अटल bool truncate_पंक्तियाँ = false;

/* (-J) True means we join lines without any line truncation. -J
   dominates -w option. */
अटल bool join_पंक्तियाँ = false;

/* Number of characters in a column.  Based on col_sep_length and
   page width. */
अटल पूर्णांक chars_per_column;

/* (-e) True means convert tabs to spaces on input. */
अटल bool untabify_निवेश = false;

/* (-e) The input tab character. */
अटल अक्षर निवेश_tab_char = '\t';

/* (-e) Tabstops are at chars_per_tab, 2*chars_per_tab, 3*chars_per_tab, ...
   where the leftmost column is 1. */
अटल पूर्णांक chars_per_निवेश_tab = 8;

/* (-i) True means convert spaces to tabs on output. */
अटल bool tabify_output = false;

/* (-i) The output tab character. */
अटल अक्षर output_tab_char = '\t';

/* (-i) The width of the output tab. */
अटल पूर्णांक chars_per_output_tab = 8;

/* Keeps track of pending white space.  When we hit a nonspace
   character after some whitespace, we print whitespace, tabbing
   if necessary to get to output_position + spaces_not_printed. */
अटल पूर्णांक spaces_not_printed;

/* (-o) Number of spaces in the left margin (tabs used when possible). */
अटल पूर्णांक chars_per_margin = 0;

/* Position where the next character will fall.
   Leftmost position is 0 + chars_per_margin.
   Rightmost position is chars_per_margin + chars_per_line - 1.
   This is important for converting spaces to tabs on output. */
अटल पूर्णांक output_position;

/* Horizontal position relative to the current file.
   (output_position depends on where we are on the page;
   input_position depends on where we are in the file.)
   Important for converting tabs to spaces on input. */
अटल पूर्णांक निवेश_position;

/* True if there were any failed opens so we can exit with nonzero
   status.  */
अटल bool failed_opens = false;

/* The number of spaces taken up if we print a tab character with width
   c_ from position h_. */
#घोषणा TAB_WIDTH(c_, h_) ((c_) - ((h_) % (c_)))

/* The horizontal position we'll be at after printing a tab character
   of width c_ from the position h_. */
#घोषणा POS_AFTER_TAB(c_, h_) ((h_) + TAB_WIDTH (c_, h_))

/* (-NNN) Number of columns of text to print. */
अटल पूर्णांक columns = 1;

/* (+NNN:MMM) Page numbers on which to begin and stop printing.
   first_page_number = 0  will be used to check input only. */
अटल uintmax_t first_page_संख्या = 0;
अटल uintmax_t last_page_संख्या = UINTMAX_MAX;

/* Number of files open (not closed, not on hold). */
अटल पूर्णांक files_ready_to_read = 0;

/* Current page number.  Displayed in header. */
अटल uintmax_t page_संख्या;

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
अटल पूर्णांक पंक्ति_संख्या;

/* (-n) True means lines should be preceded by numbers. */
अटल bool numbered_पंक्तियाँ = false;

/* (-n) Character which follows each line number. */
अटल अक्षर संख्या_separator = '\t';

/* (-n) line counting starts with 1st line of input file (not with 1st
   line of 1st page printed). */
अटल पूर्णांक पंक्ति_गिनती = 1;

/* (-n) True means counting of skipped lines starts with 1st line of
   input file. False means -N option is used in addition, counting of
   skipped lines not required. */
अटल bool skip_गिनती = true;

/* (-N) Counting starts with start_line_number = NUMBER at 1st line of
   first page printed, usually not 1st page of input file. */
अटल पूर्णांक start_पंक्ति_अंक = 1;

/* (-n) Width in characters of a line number. */
अटल पूर्णांक chars_per_संख्या = 5;

/* Used when widening the first column to accommodate numbers -- only
   needed when printing files in parallel.  Includes width of both the
   number and the number_separator. */
अटल पूर्णांक संख्या_width;

/* Buffer sprintf uses to format a line number. */
अटल अक्षर *संख्या_buff;

/* (-v) True means nonprintable characters are printed as escape sequences.
   control-g becomes \007. */
अटल bool use_esc_sequence = false;

/* (-c) True means nonprintable characters are printed as control prefixes.
   control-g becomes ^G. */
अटल bool use_cntrl_prefix = false;

/* (-d) True means output is double spaced. */
अटल bool double_space = false;

/* Number of files opened initially in init_files.  Should be 1
   unless we're printing multiple files in parallel. */
अटल पूर्णांक योग_files = 0;

/* (-r) True means don't complain if we can't open a file. */
अटल bool ignore_failed_opens = false;

/* (-S) True means we separate columns with a specified string.
   -S option does not affect line truncation nor column alignment. */
अटल bool use_col_separator = false;

/* String used to separate columns if the -S option has been specified.
   Default without -S but together with one of the column options
   -a|COLUMN|-m is a 'space' and with the -J option a 'tab'. */
अटल अक्षर स्थिर *col_sep_string = "";
अटल पूर्णांक col_sep_लंबाई = 0;
अटल अक्षर *column_separator = (अक्षर *) " ";
अटल अक्षर *पंक्ति_separator = (अक्षर *) "\t";

/* Number of separator characters waiting to be printed as soon as we
   know that we have any input remaining to be printed. */
अटल पूर्णांक separators_not_printed;

/* Position we need to pad to, as soon as we know that we have input
   remaining to be printed. */
अटल पूर्णांक padding_not_printed;

/* True means we should pad the end of the page.  Remains false until we
   know we have a page to print. */
अटल bool pad_vertically;

/* (-h) String of characters used in place of the filename in the header. */
अटल अक्षर *custom_header;

/* (-D) Date format for the header.  */
अटल अक्षर स्थिर *date_format;

/* The local time zone rules, as per the TZ environment variable.  */
अटल timezone_t localtz;

/* Date and file name for the header.  */
अटल अक्षर *date_text;
अटल अक्षर स्थिर *file_text;

/* Output columns available, not counting the date and file name.  */
अटल पूर्णांक header_width_available;

अटल अक्षर *clump_buff;

/* True means we read the line no. lines_per_body in skip_read
   called by skip_to_page. That variable controls the coincidence of a
   "FF set by hand" and "full_page_printed", see above the definition of
   structure COLUMN. */
अटल bool last_पंक्ति = false;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
क्रमागत
{
  COLUMNS_OPTION = अक्षर_उच्च + 1,
  PAGES_OPTION
};

अटल अक्षर स्थिर short_options[] =
  "-0123456789D:FJN:S::TW:abcde::fh:i::l:mn::o:rs::tvw:";

अटल काष्ठा option स्थिर long_options[] =
{
  {"pages", required_argument, शून्य, PAGES_OPTION},
  {"columns", required_argument, शून्य, COLUMNS_OPTION},
  {"across", no_argument, शून्य, 'a'},
  {"show-control-chars", no_argument, शून्य, 'c'},
  {"double-space", no_argument, शून्य, 'd'},
  {"date-format", required_argument, शून्य, 'D'},
  {"expand-tabs", optional_argument, शून्य, 'e'},
  {"form-feed", no_argument, शून्य, 'f'},
  {"header", required_argument, शून्य, 'h'},
  {"output-tabs", optional_argument, शून्य, 'i'},
  {"join-lines", no_argument, शून्य, 'J'},
  {"length", required_argument, शून्य, 'l'},
  {"merge", no_argument, शून्य, 'm'},
  {"number-lines", optional_argument, शून्य, 'n'},
  {"first-line-number", required_argument, शून्य, 'N'},
  {"indent", required_argument, शून्य, 'o'},
  {"no-file-warnings", no_argument, शून्य, 'r'},
  {"separator", optional_argument, शून्य, 's'},
  {"sep-string", optional_argument, शून्य, 'S'},
  {"omit-header", no_argument, शून्य, 't'},
  {"omit-pagination", no_argument, शून्य, 'T'},
  {"show-nonprinting", no_argument, शून्य, 'v'},
  {"width", required_argument, शून्य, 'w'},
  {"page-width", required_argument, शून्य, 'W'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {शून्य, 0, शून्य, 0}
};

अटल _Noreturn व्योम
integer_overflow (व्योम)
{
  error (निकास_त्रुटि, 0, _("integer overflow"));
}

/* Return the number of columns that have either an open file or
   stored lines. */

ATTRIBUTE_PURE
अटल अचिन्हित पूर्णांक
cols_ready_to_print (व्योम)
{
  COLUMN *q;
  अचिन्हित पूर्णांक i;
  अचिन्हित पूर्णांक n;

  n = 0;
  क्रम (q = column_vector, i = 0; i < columns; ++q, ++i)
    अगर (q->status == OPEN
        || q->status == FF_FOUND	/* With -b: To print a header only */
        || (storing_columns && q->पंक्तियाँ_stored > 0 && q->पंक्तियाँ_to_print > 0))
      ++n;
  वापस n;
}

/* Estimate first_ / last_page_number
   using option +FIRST_PAGE:LAST_PAGE */

अटल bool
first_last_page (पूर्णांक oi, अक्षर c, अक्षर स्थिर *pages)
{
  अक्षर *p;
  uintmax_t first;
  uintmax_t last = UINTMAX_MAX;
  strtol_error err = xstrtoumax (pages, &p, 10, &first, "");
  अगर (err != LONGINT_OK && err != LONGINT_INVALID_SUFFIX_CHAR)
    xstrtol_fatal (err, oi, c, long_options, pages);

  अगर (p == pages || !first)
    वापस false;

  अगर (*p == ':')
    {
      अक्षर स्थिर *p1 = p + 1;
      err = xstrtoumax (p1, &p, 10, &last, "");
      अगर (err != LONGINT_OK)
        xstrtol_fatal (err, oi, c, long_options, pages);
      अगर (p1 == p || last < first)
        वापस false;
    }

  अगर (*p)
    वापस false;

  first_page_संख्या = first;
  last_page_संख्या = last;
  वापस true;
}

/* Parse column count string S, and if it's valid (1 or larger and
   within range of the type of 'columns') set the global variables
   columns and explicit_columns.  Otherwise, exit with a diagnostic.  */

अटल व्योम
parse_column_गिनती (अक्षर स्थिर *s)
{
  columns = getoptnum (s, 1, _("invalid number of columns"));
  explicit_columns = true;
}

/* Estimate length of col_sep_string with option -S.  */

अटल व्योम
separator_string (अक्षर स्थिर *optarg_S)
{
  माप_प्रकार len = म_माप (optarg_S);
  अगर (पूर्णांक_उच्च < len)
    integer_overflow ();
  col_sep_लंबाई = len;
  col_sep_string = optarg_S;
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  अचिन्हित पूर्णांक n_files;
  bool old_options = false;
  bool old_w = false;
  bool old_s = false;
  अक्षर **file_names;

  /* Accumulate the digits of old-style options like -99.  */
  अक्षर *column_गिनती_string = शून्य;
  idx_t n_digits = 0;
  idx_t n_alloc = 0;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  n_files = 0;
  file_names = (argc > 1
                ? xnmalloc (argc - 1, माप (अक्षर *))
                : शून्य);

  जबतक (true)
    {
      पूर्णांक oi = -1;
      पूर्णांक c = getopt_long (argc, argv, short_options, long_options, &oi);
      अगर (c == -1)
        अवरोध;

      अगर (c_isdigit (c))
        {
          /* Accumulate column-count digits specified via old-style options. */
          अगर (n_digits + 1 >= n_alloc)
            column_गिनती_string = xpalloc (column_गिनती_string, &n_alloc, 2, -1,
                                           माप *column_गिनती_string);
          column_गिनती_string[n_digits++] = c;
          column_गिनती_string[n_digits] = '\0';
          जारी;
        }

      n_digits = 0;

      चयन (c)
        {
        हाल 1:			/* Non-option argument. */
          /* long option --page dominates old '+FIRST_PAGE ...'.  */
          अगर (! (first_page_संख्या == 0
                 && *optarg == '+' && first_last_page (-2, '+', optarg + 1)))
            file_names[n_files++] = optarg;
          अवरोध;

        हाल PAGES_OPTION:	/* --pages=FIRST_PAGE[:LAST_PAGE] */
          {			/* dominates old opt +... */
            अगर (! optarg)
              error (निकास_त्रुटि, 0,
                     _("'--pages=FIRST_PAGE[:LAST_PAGE]' missing argument"));
            अन्यथा अगर (! first_last_page (oi, 0, optarg))
              error (निकास_त्रुटि, 0, _("invalid page range %s"),
                     quote (optarg));
            अवरोध;
          }

        हाल COLUMNS_OPTION:	/* --columns=COLS */
          {
            parse_column_गिनती (optarg);

            /* If there was a prior column count specified via the
               short-named option syntax, e.g., -9, ensure that this
               long-name-specified value overrides it.  */
            मुक्त (column_गिनती_string);
            column_गिनती_string = शून्य;
            n_alloc = 0;
            अवरोध;
          }

        हाल 'a':
          print_across_flag = true;
          storing_columns = false;
          अवरोध;
        हाल 'b':
          balance_columns = true;
          अवरोध;
        हाल 'c':
          use_cntrl_prefix = true;
          अवरोध;
        हाल 'd':
          double_space = true;
          अवरोध;
        हाल 'D':
          date_format = optarg;
          अवरोध;
        हाल 'e':
          अगर (optarg)
            getoptarg (optarg, 'e', &निवेश_tab_char,
                       &chars_per_निवेश_tab);
          /* Could check tab width > 0. */
          untabify_निवेश = true;
          अवरोध;
        हाल 'f':
        हाल 'F':
          use_form_feed = true;
          अवरोध;
        हाल 'h':
          custom_header = optarg;
          अवरोध;
        हाल 'i':
          अगर (optarg)
            getoptarg (optarg, 'i', &output_tab_char,
                       &chars_per_output_tab);
          /* Could check tab width > 0. */
          tabify_output = true;
          अवरोध;
        हाल 'J':
          join_पंक्तियाँ = true;
          अवरोध;
        हाल 'l':
          पंक्तियाँ_per_page
            = getoptnum (optarg, 1,
                         _("'-l PAGE_LENGTH' invalid number of lines"));
          अवरोध;
        हाल 'm':
          parallel_files = true;
          storing_columns = false;
          अवरोध;
        हाल 'n':
          numbered_पंक्तियाँ = true;
          अगर (optarg)
            getoptarg (optarg, 'n', &संख्या_separator,
                       &chars_per_संख्या);
          अवरोध;
        हाल 'N':
          skip_गिनती = false;
          start_पंक्ति_अंक
            = getoptnum (optarg, पूर्णांक_न्यून,
                         _("'-N NUMBER' invalid starting line number"));
          अवरोध;
        हाल 'o':
          chars_per_margin = getoptnum (optarg, 0,
                                        _("'-o MARGIN' invalid line offset"));
          अवरोध;
        हाल 'r':
          ignore_failed_opens = true;
          अवरोध;
        हाल 's':
          old_options = true;
          old_s = true;
          अगर (!use_col_separator && optarg)
            separator_string (optarg);
          अवरोध;
        हाल 'S':
          old_s = false;
          /* Reset an additional input of -s, -S dominates -s */
          col_sep_string = "";
          col_sep_लंबाई = 0;
          use_col_separator = true;
          अगर (optarg)
            separator_string (optarg);
          अवरोध;
        हाल 't':
          extremities = false;
          keep_FF = true;
          अवरोध;
        हाल 'T':
          extremities = false;
          keep_FF = false;
          अवरोध;
        हाल 'v':
          use_esc_sequence = true;
          अवरोध;
        हाल 'w':
          old_options = true;
          old_w = true;
          {
            पूर्णांक tmp_cpl
              = getoptnum (optarg, 1,
                           _("'-w PAGE_WIDTH' invalid number of characters"));
            अगर (! truncate_पंक्तियाँ)
              chars_per_पंक्ति = tmp_cpl;
          }
          अवरोध;
        हाल 'W':
          old_w = false;			/* dominates -w */
          truncate_पंक्तियाँ = true;
          chars_per_पंक्ति
            = getoptnum (optarg, 1,
                         _("'-W PAGE_WIDTH' invalid number of characters"));
          अवरोध;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        शेष:
          usage (निकास_त्रुटि);
          अवरोध;
        }
    }

  अगर (column_गिनती_string)
    {
      parse_column_गिनती (column_गिनती_string);
      मुक्त (column_गिनती_string);
    }

  अगर (! date_format)
    date_format = (दो_पर्या ("POSIXLY_CORRECT") && !hard_locale (LC_TIME)
                   ? "%b %e %H:%M %Y"
                   : "%Y-%m-%d %H:%M");

  localtz = tzalloc (दो_पर्या ("TZ"));

  /* Now we can set a reasonable initial value: */
  अगर (first_page_संख्या == 0)
    first_page_संख्या = 1;

  अगर (parallel_files && explicit_columns)
    error (निकास_त्रुटि, 0,
           _("cannot specify number of columns when printing in parallel"));

  अगर (parallel_files && print_across_flag)
    error (निकास_त्रुटि, 0,
           _("cannot specify both printing across and printing in parallel"));

/* Translate some old short options to new/long options.
   To meet downward compatibility with other UNIX pr utilities
   and some POSIX specifications. */

  अगर (old_options)
    {
      अगर (old_w)
        {
          अगर (parallel_files || explicit_columns)
            {
              /* activate -W */
              truncate_पंक्तियाँ = true;
              अगर (old_s)
                /* adapt HP-UX and SunOS: -s = no separator;
                   activate -S */
                use_col_separator = true;
            }
          अन्यथा
            /* old -w sets width with columns only
               activate -J */
            join_पंक्तियाँ = true;
        }
      अन्यथा अगर (!use_col_separator)
        {
          /* No -S option read */
          अगर (old_s && (parallel_files || explicit_columns))
            {
              अगर (!truncate_पंक्तियाँ)
                {
                  /* old -s (without -w and -W) annuls column alignment,
                  uses fields, activate -J */
                  join_पंक्तियाँ = true;
                  अगर (col_sep_लंबाई > 0)
                    /* activate -S */
                    use_col_separator = true;
                }
              अन्यथा
                /* with -W */
                /* adapt HP-UX and SunOS: -s = no separator;
                   activate -S */
                use_col_separator = true;
            }
        }
    }

  क्रम (; optind < argc; optind++)
    {
      file_names[n_files++] = argv[optind];
    }

  अगर (n_files == 0)
    {
      /* No file arguments specified;  read from standard input.  */
      print_files (0, शून्य);
    }
  अन्यथा
    {
      अगर (parallel_files)
        print_files (n_files, file_names);
      अन्यथा
        {
          क्रम (अचिन्हित पूर्णांक i = 0; i < n_files; i++)
            print_files (1, &file_names[i]);
        }
    }

  cleanup ();

  अगर (have_read_stdin && ख_बंद (मानक_निवेश) == खातापूर्ण)
    error (निकास_त्रुटि, त्रुटि_सं, _("standard input"));
  main_exit (failed_opens ? निकास_त्रुटि : निकास_सफल);
}

/* Parse numeric arguments, ensuring MIN <= number <= INT_MAX.  */

अटल पूर्णांक
getoptnum (अक्षर स्थिर *n_str, पूर्णांक min, अक्षर स्थिर *err)
{
  वापस xnumtoimax (n_str, 10, min, पूर्णांक_उच्च, "", err, 0,
                     min <= 0 ? 0 : XTOINT_MIN_RANGE);
}

/* Parse options of the form -scNNN.

   Example: -nck, where 'n' is the option, c is the optional number
   separator, and k is the optional width of the field used when printing
   a number. */

अटल व्योम
getoptarg (अक्षर *arg, अक्षर switch_char, अक्षर *character, पूर्णांक *संख्या)
{
  अक्षर स्थिर *अंक = arg;

  अगर (!*अंक)
    {
      error (0, 0, _("'-%c': Invalid argument: %s"), switch_char, quote (अंक));
      usage (निकास_त्रुटि);
    }

  अगर (!c_isdigit (*अंक))
    *character = *अंक++;
  अगर (*अंक)
    {
      दीर्घ पूर्णांक tmp_long;
      strtol_error e = xstrtol (अंक, शून्य, 10, &tmp_long, "");
      अगर (e == LONGINT_OK)
        {
          अगर (tmp_long <= 0)
            e = LONGINT_INVALID;
          अन्यथा अगर (पूर्णांक_उच्च < tmp_long)
            e = LONGINT_OVERFLOW;
        }
      अगर (e != LONGINT_OK)
        {
          error (0, e & LONGINT_OVERFLOW ? EOVERFLOW : 0,
             _("'-%c' extra characters or invalid number in the argument: %s"),
                 switch_char, quote (arg));
          usage (निकास_त्रुटि);
        }
      *संख्या = tmp_long;
    }
}

/* Set parameters related to formatting. */

अटल व्योम
init_parameters (पूर्णांक संख्या_of_files)
{
  पूर्णांक chars_used_by_संख्या = 0;

  पंक्तियाँ_per_body = पंक्तियाँ_per_page - पंक्तियाँ_per_header - पंक्तियाँ_per_footer;
  अगर (पंक्तियाँ_per_body <= 0)
    {
      extremities = false;
      keep_FF = true;
    }
  अगर (! extremities)
    पंक्तियाँ_per_body = पंक्तियाँ_per_page;

  अगर (double_space)
    पंक्तियाँ_per_body = MAX (1, पंक्तियाँ_per_body / 2);

  /* If input is stdin, cannot print parallel files.  BSD dumps core
     on this. */
  अगर (संख्या_of_files == 0)
    parallel_files = false;

  अगर (parallel_files)
    columns = संख्या_of_files;

  /* One file, multi columns down: -b option is set to get a consistent
     formulation with "FF set by hand" in input files. */
  अगर (storing_columns)
    balance_columns = true;

  /* Tabification is assumed for multiple columns. */
  अगर (columns > 1)
    {
      अगर (!use_col_separator)
        {
          /* Use default separator */
          अगर (join_पंक्तियाँ)
            col_sep_string = पंक्ति_separator;
          अन्यथा
            col_sep_string = column_separator;

          col_sep_लंबाई = 1;
          use_col_separator = true;
        }
      /* It's rather pointless to define a TAB separator with column
         alignment */
      अन्यथा अगर (!join_पंक्तियाँ && col_sep_लंबाई == 1 && *col_sep_string == '\t')
        col_sep_string = column_separator;

      truncate_पंक्तियाँ = true;
      अगर (! (col_sep_लंबाई == 1 && *col_sep_string == '\t'))
        untabify_निवेश = true;
      tabify_output = true;
    }
  अन्यथा
    storing_columns = false;

  /* -J dominates -w in any case */
  अगर (join_पंक्तियाँ)
    truncate_पंक्तियाँ = false;

  अगर (numbered_पंक्तियाँ)
    {
      पूर्णांक chars_per_default_tab = 8;

      पंक्ति_गिनती = start_पंक्ति_अंक;

      /* To allow input tab-expansion (-e sensitive) use:
         if (number_separator == input_tab_char)
           number_width = chars_per_number
             + TAB_WIDTH (chars_per_input_tab, chars_per_number);   */

      /* Estimate chars_per_text without any margin and keep it constant. */
      अगर (संख्या_separator == '\t')
        संख्या_width = (chars_per_संख्या
                        + TAB_WIDTH (chars_per_default_tab, chars_per_संख्या));
      अन्यथा
        संख्या_width = chars_per_संख्या + 1;

      /* The number is part of the column width unless we are
         printing files in parallel. */
      अगर (parallel_files)
        chars_used_by_संख्या = संख्या_width;
    }

  पूर्णांक sep_chars, useful_chars;
  अगर (ckd_mul (&sep_chars, columns - 1, col_sep_लंबाई))
    sep_chars = पूर्णांक_उच्च;
  अगर (ckd_sub (&useful_chars, chars_per_पंक्ति - chars_used_by_संख्या,
               sep_chars))
    useful_chars = 0;
  chars_per_column = useful_chars / columns;

  अगर (chars_per_column < 1)
    error (निकास_त्रुटि, 0, _("page width too narrow"));

  अगर (numbered_पंक्तियाँ)
    {
      मुक्त (संख्या_buff);
      संख्या_buff = xmalloc (MAX (chars_per_संख्या,
                                  INT_STRLEN_BOUND (पंक्ति_संख्या)) + 1);
    }

  /* Pick the maximum between the tab width and the width of an
     escape sequence.
     The width of an escape sequence (4) isn't the lower limit any longer.
     We've to use 8 as the lower limit, if we use chars_per_default_tab = 8
     to expand a tab which is not an input_tab-char. */
  मुक्त (clump_buff);
  clump_buff = xmalloc (MAX (8, chars_per_निवेश_tab));
}

/* Open the necessary files,
   maintaining a COLUMN structure for each column.

   With multiple files, each column p has a different p->fp.
   With single files, each column p has the same p->fp.
   Return false if (number_of_files > 0) and no files can be opened,
   true otherwise.

   With each column/file p, p->full_page_printed is initialized,
   see also open_file.  */

अटल bool
init_fps (पूर्णांक संख्या_of_files, अक्षर **av)
{
  COLUMN *p;

  योग_files = 0;

  मुक्त (column_vector);
  column_vector = xnmalloc (columns, माप (COLUMN));

  अगर (parallel_files)
    {
      पूर्णांक files_बायाँ = संख्या_of_files;
      क्रम (p = column_vector; files_बायाँ--; ++p, ++av)
        {
          अगर (! open_file (*av, p))
            {
              --p;
              --columns;
            }
        }
      अगर (columns == 0)
        वापस false;
      init_header ("", -1);
    }
  अन्यथा
    {
      p = column_vector;
      अगर (संख्या_of_files > 0)
        {
          अगर (! open_file (*av, p))
            वापस false;
          init_header (*av, fileno (p->fp));
          p->पंक्तियाँ_stored = 0;
        }
      अन्यथा
        {
          p->name = _("standard input");
          p->fp = मानक_निवेश;
          have_read_stdin = true;
          p->status = OPEN;
          p->full_page_printed = false;
          ++योग_files;
          init_header ("", -1);
          p->पंक्तियाँ_stored = 0;
        }

      अक्षर स्थिर *firstname = p->name;
      खाता *firstfp = p->fp;
      पूर्णांक i;
      क्रम (i = columns - 1, ++p; i; --i, ++p)
        {
          p->name = firstname;
          p->fp = firstfp;
          p->status = OPEN;
          p->full_page_printed = false;
          p->पंक्तियाँ_stored = 0;
        }
    }
  files_ready_to_read = योग_files;
  वापस true;
}

/* Determine print_func and char_func, the functions
   used by each column for printing and/or storing.

   Determine the horizontal position desired when we begin
   printing a column (p->start_position). */

अटल व्योम
init_funcs (व्योम)
{
  पूर्णांक i, h, h_अग्र;
  COLUMN *p;

  h = chars_per_margin;

  अगर (!truncate_पंक्तियाँ)
    h_अग्र = ANYWHERE;
  अन्यथा
    {
      /* When numbering lines of parallel files, we enlarge the
         first column to accommodate the number.  Looks better than
         the Sys V approach. */
      अगर (parallel_files && numbered_पंक्तियाँ)
        h_अग्र = h + chars_per_column + संख्या_width;
      अन्यथा
        h_अग्र = h + chars_per_column;
    }

  /* Enlarge p->start_position of first column to use the same form of
     padding_not_printed with all columns. */
  h = h + col_sep_लंबाई;

  /* This loop takes care of all but the rightmost column. */

  क्रम (p = column_vector, i = 1; i < columns; ++p, ++i)
    {
      अगर (storing_columns)	/* One file, multi columns down. */
        {
          p->char_func = store_char;
          p->print_func = print_stored;
        }
      अन्यथा
        /* One file, multi columns across; or parallel files.  */
        {
          p->char_func = print_char;
          p->print_func = read_पंक्ति;
        }

      /* Number only the first column when printing files in
         parallel. */
      p->numbered = numbered_पंक्तियाँ && (!parallel_files || i == 1);
      p->start_position = h;

      /* If we don't truncate lines, all start_positions are
         ANYWHERE, except the first column's start_position when
         using a margin. */

      अगर (!truncate_पंक्तियाँ)
        {
          h = ANYWHERE;
          h_अग्र = ANYWHERE;
        }
      अन्यथा
        {
          h = h_अग्र + col_sep_लंबाई;
          h_अग्र = h + chars_per_column;
        }
    }

  /* The rightmost column.

     Doesn't need to be stored unless we intend to balance
     columns on the last page. */
  अगर (storing_columns && balance_columns)
    {
      p->char_func = store_char;
      p->print_func = print_stored;
    }
  अन्यथा
    {
      p->char_func = print_char;
      p->print_func = read_पंक्ति;
    }

  p->numbered = numbered_पंक्तियाँ && (!parallel_files || i == 1);
  p->start_position = h;
}

/* Open a file.  Return true if successful.

   With each file p, p->full_page_printed is initialized,
   see also init_fps. */

अटल bool
open_file (अक्षर *name, COLUMN *p)
{
  अगर (streq (name, "-"))
    {
      p->name = _("standard input");
      p->fp = मानक_निवेश;
      have_read_stdin = true;
    }
  अन्यथा
    {
      p->name = name;
      p->fp = ख_खोलो (name, "r");
    }
  अगर (p->fp == शून्य)
    {
      failed_opens = true;
      अगर (!ignore_failed_opens)
        error (0, त्रुटि_सं, "%s", quotef (name));
      वापस false;
    }
  fadvise (p->fp, FADVISE_SEQUENTIAL);
  p->status = OPEN;
  p->full_page_printed = false;
  ++योग_files;
  वापस true;
}

/* Close the file in P.

   If we aren't dealing with multiple files in parallel, we change
   the status of all columns in the column list to reflect the close. */

अटल व्योम
close_file (COLUMN *p)
{
  COLUMN *q;
  पूर्णांक i;

  अगर (p->status == CLOSED)
    वापस;

  पूर्णांक err = त्रुटि_सं;
  अगर (!ख_त्रुटि (p->fp))
    err = 0;
  अगर (fileno (p->fp) == STDIN_FILENO)
    साफ_त्रुटि (p->fp);
  अन्यथा अगर (ख_बंद (p->fp) != 0 && !err)
    err = त्रुटि_सं;
  अगर (err)
    error (निकास_त्रुटि, err, "%s", quotef (p->name));

  अगर (!parallel_files)
    {
      क्रम (q = column_vector, i = columns; i; ++q, --i)
        {
          q->status = CLOSED;
          अगर (q->पंक्तियाँ_stored == 0)
            {
              q->पंक्तियाँ_to_print = 0;
            }
        }
    }
  अन्यथा
    {
      p->status = CLOSED;
      p->पंक्तियाँ_to_print = 0;
    }

  --files_ready_to_read;
}

/* Put a file on hold until we start a new page,
   since we've hit a form feed.

   If we aren't dealing with parallel files, we must change the
   status of all columns in the column list. */

अटल व्योम
hold_file (COLUMN *p)
{
  COLUMN *q;
  पूर्णांक i;

  अगर (!parallel_files)
    क्रम (q = column_vector, i = columns; i; ++q, --i)
      {
        अगर (storing_columns)
          q->status = FF_FOUND;
        अन्यथा
          q->status = ON_HOLD;
      }
  अन्यथा
    p->status = ON_HOLD;

  p->पंक्तियाँ_to_print = 0;
  --files_ready_to_read;
}

/* Undo hold_file -- go through the column list and change any
   ON_HOLD columns to OPEN.  Used at the end of each page. */

अटल व्योम
reset_status (व्योम)
{
  पूर्णांक i = columns;
  COLUMN *p;

  क्रम (p = column_vector; i; --i, ++p)
    अगर (p->status == ON_HOLD)
      {
        p->status = OPEN;
        files_ready_to_read++;
      }

  अगर (storing_columns)
    {
      अगर (column_vector->status == CLOSED)
        /* We use the info to output an error message in  skip_to_page. */
        files_ready_to_read = 0;
      अन्यथा
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

अटल व्योम
print_files (पूर्णांक संख्या_of_files, अक्षर **av)
{
  init_parameters (संख्या_of_files);
  अगर (! init_fps (संख्या_of_files, av))
    वापस;
  अगर (storing_columns)
    init_store_cols ();

  अगर (first_page_संख्या > 1)
    {
      अगर (!skip_to_page (first_page_संख्या))
        वापस;
      अन्यथा
        page_संख्या = first_page_संख्या;
    }
  अन्यथा
    page_संख्या = 1;

  init_funcs ();

  पंक्ति_संख्या = पंक्ति_गिनती;
  जबतक (print_page ())
    ;
}

/* Initialize header information.
   If DESC is non-negative, it is a file descriptor open to
   FILENAME for reading.  */

अटल व्योम
init_header (अक्षर स्थिर *filename, पूर्णांक desc)
{
  अक्षर *buf = शून्य;
  काष्ठा stat st;
  काष्ठा timespec t;
  पूर्णांक ns;
  काष्ठा पंचांग पंचांग;

  /* If parallel files or standard input, use current date. */
  अगर (streq (filename, "-"))
    desc = -1;
  अगर (0 <= desc && ख_स्थिति (desc, &st) == 0)
    t = get_stat_mtime (&st);
  अन्यथा
    {
      अटल काष्ठा timespec timespec;
      अगर (! timespec.tv_sec)
        समय_लो (&timespec);
      t = timespec;
    }

  ns = t.tv_nsec;
  अगर (localtime_rz (localtz, &t.tv_sec, &पंचांग))
    {
      सूचक_भेद_प्रकार len = nstrftime (शून्य, MIN (PTRDIFF_MAX, SIZE_MAX),
                                 date_format, &पंचांग, localtz, ns);
      अगर (0 <= len)
        {
          buf = ximalloc (len + 1);
          nstrftime (buf, len + 1, date_format, &पंचांग, localtz, ns);
        }
    }
  अगर (!buf)
    {
      अक्षर secbuf[INT_BUFSIZE_BOUND (intmax_t)];
      buf = xmalloc (माप secbuf + MAX (10, INT_BUFSIZE_BOUND (पूर्णांक)));
      प्र_लिखो (buf, "%s.%09d", timetostr (t.tv_sec, secbuf), ns);
    }

  मुक्त (date_text);
  date_text = buf;
  file_text = custom_header ? custom_header : desc < 0 ? "" : filename;
  header_width_available = (chars_per_पंक्ति
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

अटल व्योम
init_page (व्योम)
{
  पूर्णांक j;
  COLUMN *p;

  अगर (storing_columns)
    {
      store_columns ();
      क्रम (j = columns - 1, p = column_vector; j; --j, ++p)
        {
          p->पंक्तियाँ_to_print = p->पंक्तियाँ_stored;
        }

      /* Last column. */
      अगर (balance_columns)
        {
          p->पंक्तियाँ_to_print = p->पंक्तियाँ_stored;
        }
      /* Since we're not balancing columns, we don't need to store
         the rightmost column.   Read it straight from the file. */
      अन्यथा
        {
          अगर (p->status == OPEN)
            {
              p->पंक्तियाँ_to_print = पंक्तियाँ_per_body;
            }
          अन्यथा
            p->पंक्तियाँ_to_print = 0;
        }
    }
  अन्यथा
    क्रम (j = columns, p = column_vector; j; --j, ++p)
      अगर (p->status == OPEN)
        {
          p->पंक्तियाँ_to_print = पंक्तियाँ_per_body;
        }
      अन्यथा
        p->पंक्तियाँ_to_print = 0;
}

/* Align empty columns and print separators.
   Empty columns will be formed by files with status ON_HOLD or CLOSED
   when printing multiple files in parallel. */

अटल व्योम
align_column (COLUMN *p)
{
  padding_not_printed = p->start_position;
  अगर (col_sep_लंबाई < padding_not_printed)
    {
      pad_across_to (padding_not_printed - col_sep_लंबाई);
      padding_not_printed = ANYWHERE;
    }

  अगर (use_col_separator)
    print_sep_string ();

  अगर (p->numbered)
    add_पंक्ति_संख्या (p);
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

अटल bool
print_page (व्योम)
{
  पूर्णांक j;
  पूर्णांक पंक्तियाँ_बायाँ_on_page;
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

  अगर (cols_ready_to_print () == 0)
    वापस false;

  अगर (extremities)
    print_a_header = true;

  /* Don't pad unless we know a page was printed. */
  pad_vertically = false;
  pv = false;

  पंक्तियाँ_बायाँ_on_page = पंक्तियाँ_per_body;
  अगर (double_space)
    पंक्तियाँ_बायाँ_on_page *= 2;

  जबतक (पंक्तियाँ_बायाँ_on_page > 0 && cols_ready_to_print () > 0)
    {
      output_position = 0;
      spaces_not_printed = 0;
      separators_not_printed = 0;
      pad_vertically = false;
      align_empty_cols = false;
      empty_पंक्ति = true;

      क्रम (j = 1, p = column_vector; j <= columns; ++j, ++p)
        {
          निवेश_position = 0;
          अगर (p->पंक्तियाँ_to_print > 0 || p->status == FF_FOUND)
            {
              FF_only = false;
              padding_not_printed = p->start_position;
              अगर (!(p->print_func) (p))
                read_rest_of_पंक्ति (p);
              pv |= pad_vertically;

              --p->पंक्तियाँ_to_print;
              अगर (p->पंक्तियाँ_to_print <= 0)
                {
                  अगर (cols_ready_to_print () == 0)
                    अवरोध;
                }

              /* File p changed its status to ON_HOLD or CLOSED */
              अगर (parallel_files && p->status != OPEN)
                {
                  अगर (empty_पंक्ति)
                    align_empty_cols = true;
                  अन्यथा अगर (p->status == CLOSED
                           || (p->status == ON_HOLD && FF_only))
                    align_column (p);
                }
            }
          अन्यथा अगर (parallel_files)
            {
              /* File status ON_HOLD or CLOSED */
              अगर (empty_पंक्ति)
                align_empty_cols = true;
              अन्यथा
                align_column (p);
            }

          /* We need it also with an empty column */
          अगर (use_col_separator)
            ++separators_not_printed;
        }

      अगर (pad_vertically)
        {
          अक्षर_दो ('\n');
          --पंक्तियाँ_बायाँ_on_page;
        }

      अगर (cols_ready_to_print () == 0 && !extremities)
        अवरोध;

      अगर (double_space && pv)
        {
          अक्षर_दो ('\n');
          --पंक्तियाँ_बायाँ_on_page;
        }
    }

  अगर (पंक्तियाँ_बायाँ_on_page == 0)
    क्रम (j = 1, p = column_vector; j <= columns; ++j, ++p)
      अगर (p->status == OPEN)
        p->full_page_printed = true;

  pad_vertically = pv;

  अगर (pad_vertically && extremities)
    pad_down (पंक्तियाँ_बायाँ_on_page + पंक्तियाँ_per_footer);
  अन्यथा अगर (keep_FF && print_a_FF)
    {
      अक्षर_दो ('\f');
      print_a_FF = false;
    }

  अगर (ख_त्रुटि (मानक_निकास))
    write_error ();

  अगर (last_page_संख्या < ++page_संख्या)
    वापस false;		/* Stop printing with LAST_PAGE */

  reset_status ();		/* Change ON_HOLD to OPEN. */

  वापस true;			/* More pages to go. */
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

अटल व्योम
init_store_cols (व्योम)
{
  /* Tune this.  */
  पूर्णांक योग_पंक्तियाँ, योग_पंक्तियाँ_1, chars_per_column_1, chars_if_truncate;
  अगर (ckd_mul (&योग_पंक्तियाँ, पंक्तियाँ_per_body, columns)
      || ckd_add (&योग_पंक्तियाँ_1, योग_पंक्तियाँ, 1)
      || ckd_add (&chars_per_column_1, chars_per_column, 1)
      || ckd_mul (&chars_if_truncate, योग_पंक्तियाँ, chars_per_column_1)
      || ckd_mul (&buff_allocated, chars_if_truncate, use_col_separator + 1))
    integer_overflow ();

  मुक्त (पंक्ति_vector);
  /* FIXME: here's where it was allocated.  */
  पंक्ति_vector = xnmalloc (योग_पंक्तियाँ_1, माप *पंक्ति_vector);

  मुक्त (end_vector);
  end_vector = xnmalloc (योग_पंक्तियाँ, माप *end_vector);

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

अटल व्योम
store_columns (व्योम)
{
  पूर्णांक i, j;
  अचिन्हित पूर्णांक पंक्ति = 0;
  अचिन्हित पूर्णांक buff_start;
  पूर्णांक last_col;		/* The rightmost column which will be saved in buff */
  COLUMN *p;

  buff_current = 0;
  buff_start = 0;

  अगर (balance_columns)
    last_col = columns;
  अन्यथा
    last_col = columns - 1;

  क्रम (i = 1, p = column_vector; i <= last_col; ++i, ++p)
    p->पंक्तियाँ_stored = 0;

  क्रम (i = 1, p = column_vector; i <= last_col && files_ready_to_read;
       ++i, ++p)
    {
      p->current_पंक्ति = पंक्ति;
      क्रम (j = पंक्तियाँ_per_body; j && files_ready_to_read; --j)

        अगर (p->status == OPEN)	/* Redundant.  Clean up. */
          {
            निवेश_position = 0;

            अगर (!read_पंक्ति (p))
              read_rest_of_पंक्ति (p);

            अगर (p->status == OPEN
                || buff_start != buff_current)
              {
                ++p->पंक्तियाँ_stored;
                पंक्ति_vector[पंक्ति] = buff_start;
                end_vector[पंक्ति++] = निवेश_position;
                buff_start = buff_current;
              }
          }
    }

  /* Keep track of the location of the last char in buff. */
  पंक्ति_vector[पंक्ति] = buff_start;

  अगर (balance_columns)
    balance (पंक्ति);
}

अटल व्योम
balance (पूर्णांक योग_stored)
{
  COLUMN *p;
  पूर्णांक i, पंक्तियाँ;
  पूर्णांक first_पंक्ति = 0;

  क्रम (i = 1, p = column_vector; i <= columns; ++i, ++p)
    {
      पंक्तियाँ = योग_stored / columns;
      अगर (i <= योग_stored % columns)
        ++पंक्तियाँ;

      p->पंक्तियाँ_stored = पंक्तियाँ;
      p->current_पंक्ति = first_पंक्ति;

      first_पंक्ति += पंक्तियाँ;
    }
}

/* Store a character in the buffer. */

अटल व्योम
store_char (अक्षर c)
{
  अगर (buff_current >= buff_allocated)
    {
      /* May be too generous. */
      buff = xpalloc (buff, &buff_allocated, 1, -1, माप *buff);
    }
  buff[buff_current++] = c;
}

अटल व्योम
add_पंक्ति_संख्या (COLUMN *p)
{
  पूर्णांक i;
  अक्षर *s;
  पूर्णांक अंक_width;

  /* Cutting off the higher-order digits is more informative than
     lower-order cut off. */
  अंक_width = प्र_लिखो (संख्या_buff, "%*d", chars_per_संख्या, पंक्ति_संख्या);
  पंक्ति_संख्या++;
  s = संख्या_buff + (अंक_width - chars_per_संख्या);
  क्रम (i = chars_per_संख्या; i > 0; i--)
    (p->char_func) (*s++);

  अगर (columns > 1)
    {
      /* Tabification is assumed for multiple columns, also for n-separators,
         but 'default n-separator = TAB' hasn't been given priority over
         equal column_width also specified by POSIX. */
      अगर (संख्या_separator == '\t')
        {
          i = संख्या_width - chars_per_संख्या;
          जबतक (i-- > 0)
            (p->char_func) (' ');
        }
      अन्यथा
        (p->char_func) (संख्या_separator);
    }
  अन्यथा
    /* To comply with POSIX, we avoid any expansion of default TAB
       separator with a single column output. No column_width requirement
       has to be considered. */
    {
      (p->char_func) (संख्या_separator);
      अगर (संख्या_separator == '\t')
        output_position = POS_AFTER_TAB (chars_per_output_tab,
                          output_position);
    }

  अगर (truncate_पंक्तियाँ && !parallel_files)
    निवेश_position += संख्या_width;
}

/* Print (or store) padding until the current horizontal position
   is position. */

अटल व्योम
pad_across_to (पूर्णांक position)
{
  पूर्णांक h = output_position;

  अगर (tabify_output)
    spaces_not_printed = position - output_position;
  अन्यथा
    {
      जबतक (++h <= position)
        अक्षर_दो (' ');
      output_position = position;
    }
}

/* Pad to the bottom of the page.

   If the user has requested a formfeed, use one.
   Otherwise, use newlines. */

अटल व्योम
pad_down (अचिन्हित पूर्णांक पंक्तियाँ)
{
  अगर (use_form_feed)
    अक्षर_दो ('\f');
  अन्यथा
    क्रम (अचिन्हित पूर्णांक i = पंक्तियाँ; i; --i)
      अक्षर_दो ('\n');
}

/* Read the rest of the line.

   Read from the current column's file until an end of line is
   hit.  Used when we've truncated a line and we no longer need
   to print or store its characters. */

अटल व्योम
read_rest_of_पंक्ति (COLUMN *p)
{
  पूर्णांक c;
  खाता *f = p->fp;

  जबतक ((c = अ_लो (f)) != '\n')
    {
      अगर (c == '\f')
        {
          अगर ((c = अ_लो (f)) != '\n')
            अक्षर_वापस (c, f);
          अगर (keep_FF)
            print_a_FF = true;
          hold_file (p);
          अवरोध;
        }
      अन्यथा अगर (c == खातापूर्ण)
        {
          close_file (p);
          अवरोध;
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

अटल व्योम
skip_read (COLUMN *p, पूर्णांक column_संख्या)
{
  पूर्णांक c;
  खाता *f = p->fp;
  पूर्णांक i;
  bool single_ff = false;
  COLUMN *q;

  /* Read 1st character in a line or any character succeeding a FF */
  अगर ((c = अ_लो (f)) == '\f' && p->full_page_printed)
    /* A FF-coincidence with a previous full_page_printed.
       To avoid an additional empty page, eliminate the FF */
    अगर ((c = अ_लो (f)) == '\n')
      c = अ_लो (f);

  p->full_page_printed = false;

  /* 1st character a FF means a single FF without any printable
     characters. Don't count it as a line with -n option. */
  अगर (c == '\f')
    single_ff = true;

  /* Preparing for a FF-coincidence: Maybe we finish that page
     without a FF found */
  अगर (last_पंक्ति)
    p->full_page_printed = true;

  जबतक (c != '\n')
    {
      अगर (c == '\f')
        {
          /* No FF-coincidence possible,
             no catching up of a FF-coincidence with next page */
          अगर (last_पंक्ति)
            {
              अगर (!parallel_files)
                क्रम (q = column_vector, i = columns; i; ++q, --i)
                  q->full_page_printed = false;
              अन्यथा
                p->full_page_printed = false;
            }

          अगर ((c = अ_लो (f)) != '\n')
            अक्षर_वापस (c, f);
          hold_file (p);
          अवरोध;
        }
      अन्यथा अगर (c == खातापूर्ण)
        {
          close_file (p);
          अवरोध;
        }
      c = अ_लो (f);
    }

  अगर (skip_गिनती)
    अगर ((!parallel_files || column_संख्या == 1) && !single_ff)
      ++पंक्ति_गिनती;
}

/* If we're tabifying output,

   When print_char encounters white space it keeps track
   of our desired horizontal position and delays printing
   until this function is called. */

अटल व्योम
print_white_space (व्योम)
{
  पूर्णांक h_new;
  पूर्णांक h_old = output_position;
  पूर्णांक goal = h_old + spaces_not_printed;

  जबतक (goal - h_old > 1
         && (h_new = POS_AFTER_TAB (chars_per_output_tab, h_old)) <= goal)
    {
      अक्षर_दो (output_tab_char);
      h_old = h_new;
    }
  जबतक (++h_old <= goal)
    अक्षर_दो (' ');

  output_position = goal;
  spaces_not_printed = 0;
}

/* Print column separators.

   We keep a count until we know that we'll be printing a line,
   then print_sep_string() is called. */

अटल व्योम
print_sep_string (व्योम)
{
  अक्षर स्थिर *s = col_sep_string;
  पूर्णांक l = col_sep_लंबाई;

  अगर (separators_not_printed <= 0)
    {
      /* We'll be starting a line with chars_per_margin, anything else? */
      अगर (spaces_not_printed > 0)
        print_white_space ();
    }
  अन्यथा
    {
      क्रम (; separators_not_printed > 0; --separators_not_printed)
        {
          जबतक (l-- > 0)
            {
              /* 3 types of sep_strings: spaces only, spaces and chars,
              chars only */
              अगर (*s == ' ')
                {
                  /* We're tabifying output; consecutive spaces in
                  sep_string may have to be converted to tabs */
                  s++;
                  ++spaces_not_printed;
                }
              अन्यथा
                {
                  अगर (spaces_not_printed > 0)
                    print_white_space ();
                  अक्षर_दो (*s++);
                  ++output_position;
                }
            }
          /* sep_string ends with some spaces */
          अगर (spaces_not_printed > 0)
            print_white_space ();
        }
    }
}

/* Print (or store, depending on p->char_func) a clump of N
   characters. */

अटल व्योम
print_clump (COLUMN *p, पूर्णांक n, अक्षर *clump)
{
  जबतक (n--)
    (p->char_func) (*clump++);

  अगर (ख_त्रुटि (मानक_निकास))
    write_error ();
}

/* Print a character.

   Update the following comment: process-char hasn't been used any
   longer.
   If we're tabifying, all tabs have been converted to spaces by
   process_char().  Keep a count of consecutive spaces, and when
   a nonspace is encountered, call print_white_space() to print the
   required number of tabs and spaces. */

अटल व्योम
print_char (अक्षर c)
{
  अगर (tabify_output)
    {
      अगर (c == ' ')
        {
          ++spaces_not_printed;
          वापस;
        }
      अन्यथा अगर (spaces_not_printed > 0)
        print_white_space ();

      /* Nonprintables are assumed to have width 0, except '\b'. */
      अगर (! है_छाप (to_uchar (c)))
        {
          अगर (c == '\b')
            --output_position;
        }
      अन्यथा
        ++output_position;
    }
  अक्षर_दो (c);
}

/* Skip to page PAGE before printing.
   PAGE may be larger than total number of pages. */

अटल bool
skip_to_page (uintmax_t page)
{
  क्रम (uintmax_t n = 1; n < page; ++n)
    {
      COLUMN *p;
      पूर्णांक j;

      क्रम (पूर्णांक i = 1; i < पंक्तियाँ_per_body; ++i)
        {
          क्रम (j = 1, p = column_vector; j <= columns; ++j, ++p)
            अगर (p->status == OPEN)
              skip_read (p, j);
        }
      last_पंक्ति = true;
      क्रम (j = 1, p = column_vector; j <= columns; ++j, ++p)
        अगर (p->status == OPEN)
          skip_read (p, j);

      अगर (storing_columns)	/* change FF_FOUND to ON_HOLD */
        क्रम (j = 1, p = column_vector; j <= columns; ++j, ++p)
          अगर (p->status != CLOSED)
            p->status = ON_HOLD;

      reset_status ();
      last_पंक्ति = false;

      अगर (files_ready_to_read < 1)
        {
          /* It's very helpful, normally the total number of pages is
             not known in advance.  */
          error (0, 0,
                 _("starting page number %ju exceeds page count %ju"),
                 page, n);
          अवरोध;
        }
    }
  वापस files_ready_to_read > 0;
}

/* Print a header.

   Formfeeds are assumed to use up two lines at the beginning of
   the page. */

अटल व्योम
print_header (व्योम)
{
  अक्षर page_text[256 + INT_STRLEN_BOUND (page_संख्या)];
  पूर्णांक available_width;
  पूर्णांक lhs_spaces;
  पूर्णांक rhs_spaces;

  output_position = 0;
  pad_across_to (chars_per_margin);
  print_white_space ();

  अगर (page_संख्या == 0)
    error (निकास_त्रुटि, 0, _("page number overflow"));

  /* The translator must ensure that formatting the translation of
     "Page %ju" does not generate more than (sizeof page_text - 1)
     bytes.  */
  प्र_लिखो (page_text, _("Page %ju"), page_संख्या);
  available_width = header_width_available - mbswidth (page_text, 0);
  available_width = MAX (0, available_width);
  lhs_spaces = available_width >> 1;
  rhs_spaces = available_width - lhs_spaces;

  म_लिखो ("\n\n%*s%s%*s%s%*s%s\n\n\n",
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

अटल bool
read_पंक्ति (COLUMN *p)
{
  पूर्णांक c;
  पूर्णांक chars IF_LINT (= 0);
  पूर्णांक last_निवेश_position;
  पूर्णांक j, k;
  COLUMN *q;

  /* read 1st character in each line or any character succeeding a FF: */
  c = अ_लो (p->fp);

  last_निवेश_position = निवेश_position;

  अगर (c == '\f' && p->full_page_printed)
    अगर ((c = अ_लो (p->fp)) == '\n')
      c = अ_लो (p->fp);
  p->full_page_printed = false;

  चयन (c)
    {
    हाल '\f':
      अगर ((c = अ_लो (p->fp)) != '\n')
        अक्षर_वापस (c, p->fp);
      FF_only = true;
      अगर (print_a_header && !storing_columns)
        {
          pad_vertically = true;
          print_header ();
        }
      अन्यथा अगर (keep_FF)
        print_a_FF = true;
      hold_file (p);
      वापस true;
    हाल खातापूर्ण:
      close_file (p);
      वापस true;
    हाल '\n':
      अवरोध;
    शेष:
      chars = char_to_clump (c);
    }

  अगर (truncate_पंक्तियाँ && निवेश_position > chars_per_column)
    {
      निवेश_position = last_निवेश_position;
      वापस false;
    }

  अगर (p->char_func != store_char)
    {
      pad_vertically = true;

      अगर (print_a_header && !storing_columns)
        print_header ();

      अगर (parallel_files && align_empty_cols)
        {
          /* We have to align empty columns at the beginning of a line. */
          k = separators_not_printed;
          separators_not_printed = 0;
          क्रम (j = 1, q = column_vector; j <= k; ++j, ++q)
            {
              align_column (q);
              separators_not_printed += 1;
            }
          padding_not_printed = p->start_position;
          अगर (truncate_पंक्तियाँ)
            spaces_not_printed = chars_per_column;
          अन्यथा
            spaces_not_printed = 0;
          align_empty_cols = false;
        }

      अगर (col_sep_लंबाई < padding_not_printed)
        {
          pad_across_to (padding_not_printed - col_sep_लंबाई);
          padding_not_printed = ANYWHERE;
        }

      अगर (use_col_separator)
        print_sep_string ();
    }

  अगर (p->numbered)
    add_पंक्ति_संख्या (p);

  empty_पंक्ति = false;
  अगर (c == '\n')
    वापस true;

  print_clump (p, chars, clump_buff);

  जबतक (true)
    {
      c = अ_लो (p->fp);

      चयन (c)
        {
        हाल '\n':
          वापस true;
        हाल '\f':
          अगर ((c = अ_लो (p->fp)) != '\n')
            अक्षर_वापस (c, p->fp);
          अगर (keep_FF)
            print_a_FF = true;
          hold_file (p);
          वापस true;
        हाल खातापूर्ण:
          close_file (p);
          वापस true;
        }

      last_निवेश_position = निवेश_position;
      chars = char_to_clump (c);
      अगर (truncate_पंक्तियाँ && निवेश_position > chars_per_column)
        {
          निवेश_position = last_निवेश_position;
          वापस false;
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

अटल bool
print_stored (COLUMN *p)
{
  COLUMN *q;

  पूर्णांक पंक्ति = p->current_पंक्ति++;
  अक्षर *first = &buff[पंक्ति_vector[पंक्ति]];
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
  अक्षर *last = &buff[पंक्ति_vector[पंक्ति + 1]];

  pad_vertically = true;

  अगर (print_a_header)
    print_header ();

  अगर (p->status == FF_FOUND)
    {
      पूर्णांक i;
      क्रम (i = 1, q = column_vector; i <= columns; ++i, ++q)
        q->status = ON_HOLD;
      अगर (column_vector->पंक्तियाँ_to_print <= 0)
        {
          अगर (!extremities)
            pad_vertically = false;
          वापस true;		/* print a header only */
        }
    }

  अगर (col_sep_लंबाई < padding_not_printed)
    {
      pad_across_to (padding_not_printed - col_sep_लंबाई);
      padding_not_printed = ANYWHERE;
    }

  अगर (use_col_separator)
    print_sep_string ();

  जबतक (first != last)
    print_char (*first++);

  अगर (spaces_not_printed == 0)
    {
      output_position = p->start_position + end_vector[पंक्ति];
      अगर (p->start_position - col_sep_लंबाई == chars_per_margin)
        output_position -= col_sep_लंबाई;
    }

  वापस true;
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

अटल पूर्णांक
char_to_clump (अक्षर c)
{
  अचिन्हित अक्षर uc = c;
  अक्षर *s = clump_buff;
  अक्षर esc_buff[4];
  पूर्णांक width;
  पूर्णांक chars;
  पूर्णांक chars_per_c = 8;

  अगर (c == निवेश_tab_char)
    chars_per_c = chars_per_निवेश_tab;

  अगर (c == निवेश_tab_char || c == '\t')
    {
      width = TAB_WIDTH (chars_per_c, निवेश_position);

      अगर (untabify_निवेश)
        {
          क्रम (पूर्णांक i = width; i; --i)
            *s++ = ' ';
          chars = width;
        }
      अन्यथा
        {
          *s = c;
          chars = 1;
        }

    }
  अन्यथा अगर (! है_छाप (uc))
    {
      अगर (use_esc_sequence)
        {
          width = 4;
          chars = 4;
          *s++ = '\\';
          प्र_लिखो (esc_buff, "%03o", uc);
          क्रम (पूर्णांक i = 0; i <= 2; ++i)
            *s++ = esc_buff[i];
        }
      अन्यथा अगर (use_cntrl_prefix)
        {
          अगर (uc < 0200)
            {
              width = 2;
              chars = 2;
              *s++ = '^';
              *s = c ^ 0100;
            }
          अन्यथा
            {
              width = 4;
              chars = 4;
              *s++ = '\\';
              प्र_लिखो (esc_buff, "%03o", uc);
              क्रम (पूर्णांक i = 0; i <= 2; ++i)
                *s++ = esc_buff[i];
            }
        }
      अन्यथा अगर (c == '\b')
        {
          width = -1;
          chars = 1;
          *s = c;
        }
      अन्यथा
        {
          width = 0;
          chars = 1;
          *s = c;
        }
    }
  अन्यथा
    {
      width = 1;
      chars = 1;
      *s = c;
    }

  /* Too many backspaces must put us in position 0 -- never negative.  */
  अगर (width < 0 && निवेश_position == 0)
    {
      chars = 0;
      निवेश_position = 0;
    }
  अन्यथा अगर (width < 0 && निवेश_position <= -width)
    निवेश_position = 0;
  अन्यथा
    {
      अगर (ckd_add (&निवेश_position, निवेश_position, width))
        integer_overflow ();
    }

  वापस chars;
}

/* We've just printed some files and need to clean up things before
   looking for more options and printing the next batch of files.

   Free everything we've xmalloc'ed, except 'header'. */

अटल व्योम
cleanup (व्योम)
{
  मुक्त (संख्या_buff);
  मुक्त (clump_buff);
  मुक्त (column_vector);
  मुक्त (पंक्ति_vector);
  मुक्त (end_vector);
  मुक्त (buff);
}

/* Complain, print a usage message, and die. */

व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s [OPTION]... [खाता]...\n\
"),
              program_name);

      ख_माला_दो (_("\
Paginate or columnate खाता(s) क्रम printing.\n\
"), मानक_निकास);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\n\
  +FIRST_PAGE[:LAST_PAGE], --pages=FIRST_PAGE[:LAST_PAGE]\n\
         begin [stop] printing with page FIRST_[LAST_]PAGE\n\
"));
      oputs (_("\
  -COLS, --columns=COLS\n\
         output COLS columns and print columns down, unless -a is used.\n\
         Balance संख्या of पंक्तियाँ in the columns on each page\n\
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
  -d, --द्विगुन-space\n\
         द्विगुन space the output\n\
"));
      oputs (_("\
  -D, --date-format=FORMAT\n\
         use FORMAT क्रम the header date\n\
"));
      oputs (_("\
  -e[CHAR[WIDTH]], --expand-tabs[=CHAR[WIDTH]]\n\
         expand निवेश CHARs (TABs) to tab WIDTH (8)\n\
"));
      oputs (_("\
  -F, -f, --form-feed\n\
         use form feeds instead of newlines to separate pages\n\
         (by a 3-पंक्ति page header with -F or a 5-पंक्ति header\n\
         and trailer without -F)\n\
"));
      oputs (_("\
  -h, --header=HEADER\n\
         use a centered HEADER instead of filename in page header,\n\
         -h \"\" prints a blank पंक्ति, don't use -h\"\"\n\
"));
      oputs (_("\
  -i[CHAR[WIDTH]], --output-tabs[=CHAR[WIDTH]]\n\
         replace spaces with CHARs (TABs) to tab WIDTH (8)\n\
"));
      oputs (_("\
  -J, --join-पंक्तियाँ\n\
         merge full पंक्तियाँ, turns off -W पंक्ति truncation,\n\
         no column alignment, --sep-string[=STRING] sets separators\n\
"));
      oputs (_("\
  -l, --लंबाई=PAGE_लंबाई\n\
         set the page लंबाई to PAGE_लंबाई (66) पंक्तियाँ\n\
         (शेष संख्या of पंक्तियाँ of text 56, and with -F 63).\n\
         implies -t अगर PAGE_लंबाई <= 10\n\
"));
      oputs (_("\
  -m, --merge\n\
         print all files in parallel, one in each column,\n\
         truncate पंक्तियाँ, but join पंक्तियाँ of full लंबाई with -J\n\
"));
      oputs (_("\
  -n[SEP[DIGITS]], --संख्या-पंक्तियाँ[=SEP[DIGITS]]\n\
         संख्या पंक्तियाँ, use DIGITS (5) digits, then SEP (TAB),\n\
         शेष counting starts with 1st पंक्ति of निवेश file\n\
"));
      oputs (_("\
  -N, --first-पंक्ति-संख्या=संख्या_2\n\
         start counting with संख्या_2 at 1st पंक्ति of first\n\
         page printed (see +FIRST_PAGE)\n\
"));
      oputs (_("\
  -o, --indent=MARGIN\n\
         offset each पंक्ति with MARGIN (zero) spaces, करो not\n\
         affect -w or -W, MARGIN will be added to PAGE_WIDTH\n\
"));
      oputs (_("\
  -r, --no-file-warnings\n\
         omit warning when a file cannot be opened\n\
"));
      oputs (_("\
  -s[CHAR], --separator[=CHAR]\n\
         separate columns by a single character, शेष क्रम CHAR\n\
         is the <TAB> character without -w and \'no अक्षर\' with -w.\n\
         -s[CHAR] turns off पंक्ति truncation of all 3 column\n\
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
         omit page headers and trailers; implied अगर PAGE_लंबाई <= 10\n\
"));
      oputs (_("\
  -T, --omit-pagination\n\
         omit page headers and trailers,\n\
         eliminate any pagination by form feeds set in निवेश files\n\
"));
      oputs (_("\
  -v, --show-nonprinting\n\
         use octal backslash notation\n\
"));
      oputs (_("\
  -w, --width=PAGE_WIDTH\n\
         set page width to PAGE_WIDTH (72) characters क्रम\n\
         multiple text-column output only, -s[अक्षर] turns off (72)\n\
"));
      oputs (_("\
  -W, --page-width=PAGE_WIDTH\n\
         set page width to PAGE_WIDTH (72) characters always,\n\
         truncate पंक्तियाँ, except -J option is set,\n\
         no interference with -S or -s\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}
