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

#அந்தர்பாவ <config.h>

#அந்தர்பாவ <getopt.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ "system.h"
#அந்தர்பாவ "c-ctype.h"
#அந்தர்பாவ "fadvise.h"
#அந்தர்பாவ "hard-locale.h"
#அந்தர்பாவ "mbswidth.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "stat-time.h"
#அந்தர்பாவ "stdio--.h"
#அந்தர்பாவ "strftime.h"
#அந்தர்பாவ "xstrtol.h"
#அந்தர்பாவ "xstrtol-error.h"
#அந்தர்பாவ "xdectoint.h"

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

கட்டமைப்பு COLUMN;
கட்டமைப்பு COLUMN
  {
    FILE *fp;			/* Input stream for this column. */
    எழுத்து மாறிலி *name;		/* File name. */
    எண்ணிக்கை
      {
        OPEN,
        FF_FOUND,		/* used with -b option, set with \f, changed
                                   to ON_HOLD after print_header */
        ON_HOLD,		/* Hit a form feed. */
        CLOSED
      }
    status;			/* Status of the file pointer. */

    /* Func to print lines in this col. */
    bool (*print_func) (கட்டமைப்பு COLUMN *);

    /* Func to print/store chars in this col. */
    வெறுமை (*char_func) (எழுத்து);

    முழு current_வரி;		/* Index of current place in line_vector. */
    முழு வரிகள்_stored;		/* Number of lines stored in buff. */
    முழு வரிகள்_to_print;		/* No. lines stored or space left on page. */
    முழு start_position;		/* Horizontal position of first char. */
    bool numbered;
    bool full_page_printed;	/* True means printed without a FF found. */

    /* p->full_page_printed  controls a special case of "FF set by hand":
       True means a full page has been printed without FF found. To avoid an
       additional empty page we have to ignore a FF immediately following in
       the next line. */
  };

வகைவரை கட்டமைப்பு COLUMN COLUMN;

நிலையான முழு char_to_clump (எழுத்து c);
நிலையான bool read_வரி (COLUMN *p);
நிலையான bool print_page (வெறுமை);
நிலையான bool print_stored (COLUMN *p);
நிலையான bool open_file (எழுத்து *name, COLUMN *p);
நிலையான bool skip_to_page (uintmax_t page);
நிலையான வெறுமை print_header (வெறுமை);
நிலையான வெறுமை pad_across_to (முழு position);
நிலையான வெறுமை add_வரி_எண் (COLUMN *p);
நிலையான முழு getoptnum (எழுத்து மாறிலி *n_str, முழு min, எழுத்து மாறிலி *errfmt);
நிலையான வெறுமை getoptarg (எழுத்து *arg, எழுத்து switch_char, எழுத்து *character,
                       முழு *எண்);
நிலையான வெறுமை print_files (முழு எண்_of_files, எழுத்து **av);
நிலையான வெறுமை init_parameters (முழு எண்_of_files);
நிலையான வெறுமை init_header (எழுத்து மாறிலி *filename, முழு desc);
நிலையான bool init_fps (முழு எண்_of_files, எழுத்து **av);
நிலையான வெறுமை init_funcs (வெறுமை);
நிலையான வெறுமை init_store_cols (வெறுமை);
நிலையான வெறுமை store_columns (வெறுமை);
நிலையான வெறுமை balance (முழு total_stored);
நிலையான வெறுமை store_char (எழுத்து c);
நிலையான வெறுமை pad_down (unsigned முழு வரிகள்);
நிலையான வெறுமை read_rest_of_வரி (COLUMN *p);
நிலையான வெறுமை skip_read (COLUMN *p, முழு column_எண்);
நிலையான வெறுமை print_char (எழுத்து c);
நிலையான வெறுமை cleanup (வெறுமை);
நிலையான வெறுமை print_sep_string (வெறுமை);
நிலையான வெறுமை separator_string (எழுத்து மாறிலி *optarg_S);

/* All of the columns to print.  */
நிலையான COLUMN *column_vector;

/* When printing a single file in multiple downward columns,
   we store the leftmost columns contiguously in buff.
   To print a line from buff, get the index of the first character
   from line_vector[i], and print up to line_vector[i + 1]. */
நிலையான எழுத்து *buff;

/* Index of the position in buff where the next character
   will be stored. */
நிலையான unsigned முழு buff_current;

/* The number of characters in buff.
   Used for allocation of buff and to detect overflow of buff. */
நிலையான idx_t buff_allocated;

/* Array of indices into buff.
   Each entry is an index of the first character of a line.
   This is used when storing lines to facilitate shuffling when
   we do column balancing on the last page. */
நிலையான முழு *வரி_vector;

/* Array of horizontal positions.
   For each line in line_vector, end_vector[line] is the horizontal
   position we are in after printing that line.  We keep track of this
   so that we know how much we need to pad to prepare for the next
   column. */
நிலையான முழு *end_vector;

/* (-m) True means we're printing multiple files in parallel. */
நிலையான bool parallel_files = false;

/* (-m) True means a line starts with some empty columns (some files
   already CLOSED or ON_HOLD) which we have to align. */
நிலையான bool align_empty_cols;

/* (-m) True means we have not yet found any printable column in a line.
   align_empty_cols = true  has to be maintained. */
நிலையான bool empty_வரி;

/* (-m) False means printable column output precedes a form feed found.
   Column alignment is done only once. No additional action with that form
   feed.
   True means we found only a form feed in a column. Maybe we have to do
   some column alignment with that form feed. */
நிலையான bool FF_only;

/* (-[0-9]+) True means we're given an option explicitly specifying
   number of columns.  Used to detect when this option is used with -m
   and when translating old options to new/long options. */
நிலையான bool explicit_columns = false;

/* (-t|-T) False means we aren't printing headers and footers. */
நிலையான bool extremities = true;

/* (-t) True means we retain all FF set by hand in input files.
   False is set with -T option. */
நிலையான bool keep_FF = false;
நிலையான bool print_a_FF = false;

/* True means we need to print a header as soon as we know we've got input
   to print after it. */
நிலையான bool print_a_header;

/* (-f) True means use formfeeds instead of newlines to separate pages. */
நிலையான bool use_form_feed = false;

/* True means we have read the standard input. */
நிலையான bool have_read_stdin = false;

/* True means the -a flag has been given. */
நிலையான bool print_across_flag = false;

/* True means we're printing one file in multiple (>1) downward columns. */
நிலையான bool storing_columns = true;

/* (-b) True means balance columns on the last page as Sys V does. */
/* That's no longer an independent option. With storing_columns = true
   balance_columns = true is used too (s. function init_parameters).
   We get a consistent formulation with "FF set by hand" in input files. */
நிலையான bool balance_columns = false;

/* (-l) Number of lines on a page, including header and footer lines. */
நிலையான முழு வரிகள்_per_page = 66;

/* Number of lines in the header and footer can be reset to 0 using
   the -t flag. */
எண்ணிக்கை { வரிகள்_per_header = 5 };
நிலையான முழு வரிகள்_per_body;
எண்ணிக்கை { வரிகள்_per_footer = 5 };

/* (-w|-W) Width in characters of the page.  Does not include the width of
   the margin. */
நிலையான முழு chars_per_வரி = 72;

/* (-w|W) True means we truncate lines longer than chars_per_column. */
நிலையான bool truncate_வரிகள் = false;

/* (-J) True means we join lines without any line truncation. -J
   dominates -w option. */
நிலையான bool join_வரிகள் = false;

/* Number of characters in a column.  Based on col_sep_length and
   page width. */
நிலையான முழு chars_per_column;

/* (-e) True means convert tabs to spaces on input. */
நிலையான bool untabify_உள்ளீடு = false;

/* (-e) The input tab character. */
நிலையான எழுத்து உள்ளீடு_tab_char = '\t';

/* (-e) Tabstops are at chars_per_tab, 2*chars_per_tab, 3*chars_per_tab, ...
   where the leftmost column is 1. */
நிலையான முழு chars_per_உள்ளீடு_tab = 8;

/* (-i) True means convert spaces to tabs on output. */
நிலையான bool tabify_output = false;

/* (-i) The output tab character. */
நிலையான எழுத்து output_tab_char = '\t';

/* (-i) The width of the output tab. */
நிலையான முழு chars_per_output_tab = 8;

/* Keeps track of pending white space.  When we hit a nonspace
   character after some whitespace, we print whitespace, tabbing
   if necessary to get to output_position + spaces_not_printed. */
நிலையான முழு spaces_not_printed;

/* (-o) Number of spaces in the left margin (tabs used when possible). */
நிலையான முழு chars_per_margin = 0;

/* Position where the next character will fall.
   Leftmost position is 0 + chars_per_margin.
   Rightmost position is chars_per_margin + chars_per_line - 1.
   This is important for converting spaces to tabs on output. */
நிலையான முழு output_position;

/* Horizontal position relative to the current file.
   (output_position depends on where we are on the page;
   input_position depends on where we are in the file.)
   Important for converting tabs to spaces on input. */
நிலையான முழு உள்ளீடு_position;

/* True if there were any failed opens so we can exit with nonzero
   status.  */
நிலையான bool failed_opens = false;

/* The number of spaces taken up if we print a tab character with width
   c_ from position h_. */
#define TAB_WIDTH(c_, h_) ((c_) - ((h_) % (c_)))

/* The horizontal position we'll be at after printing a tab character
   of width c_ from the position h_. */
#define POS_AFTER_TAB(c_, h_) ((h_) + TAB_WIDTH (c_, h_))

/* (-NNN) Number of columns of text to print. */
நிலையான முழு columns = 1;

/* (+NNN:MMM) Page numbers on which to begin and stop printing.
   first_page_number = 0  will be used to check input only. */
நிலையான uintmax_t first_page_எண் = 0;
நிலையான uintmax_t last_page_எண் = UINTMAX_MAX;

/* Number of files open (not closed, not on hold). */
நிலையான முழு files_ready_to_read = 0;

/* Current page number.  Displayed in header. */
நிலையான uintmax_t page_எண்;

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
நிலையான முழு வரி_எண்;

/* (-n) True means lines should be preceded by numbers. */
நிலையான bool numbered_வரிகள் = false;

/* (-n) Character which follows each line number. */
நிலையான எழுத்து எண்_separator = '\t';

/* (-n) line counting starts with 1st line of input file (not with 1st
   line of 1st page printed). */
நிலையான முழு வரி_மொத்தம் = 1;

/* (-n) True means counting of skipped lines starts with 1st line of
   input file. False means -N option is used in addition, counting of
   skipped lines not required. */
நிலையான bool skip_மொத்தம் = true;

/* (-N) Counting starts with start_line_number = NUMBER at 1st line of
   first page printed, usually not 1st page of input file. */
நிலையான முழு start_வரி_இலக்கம் = 1;

/* (-n) Width in characters of a line number. */
நிலையான முழு chars_per_எண் = 5;

/* Used when widening the first column to accommodate numbers -- only
   needed when printing files in parallel.  Includes width of both the
   number and the number_separator. */
நிலையான முழு எண்_width;

/* Buffer sprintf uses to format a line number. */
நிலையான எழுத்து *எண்_buff;

/* (-v) True means nonprintable characters are printed as escape sequences.
   control-g becomes \007. */
நிலையான bool use_esc_sequence = false;

/* (-c) True means nonprintable characters are printed as control prefixes.
   control-g becomes ^G. */
நிலையான bool use_cntrl_prefix = false;

/* (-d) True means output is double spaced. */
நிலையான bool double_space = false;

/* Number of files opened initially in init_files.  Should be 1
   unless we're printing multiple files in parallel. */
நிலையான முழு total_files = 0;

/* (-r) True means don't complain if we can't open a file. */
நிலையான bool ignore_failed_opens = false;

/* (-S) True means we separate columns with a specified string.
   -S option does not affect line truncation nor column alignment. */
நிலையான bool use_col_separator = false;

/* String used to separate columns if the -S option has been specified.
   Default without -S but together with one of the column options
   -a|COLUMN|-m is a 'space' and with the -J option a 'tab'. */
நிலையான எழுத்து மாறிலி *col_sep_string = "";
நிலையான முழு col_sep_length = 0;
நிலையான எழுத்து *column_separator = (எழுத்து *) " ";
நிலையான எழுத்து *வரி_separator = (எழுத்து *) "\t";

/* Number of separator characters waiting to be printed as soon as we
   know that we have any input remaining to be printed. */
நிலையான முழு separators_not_printed;

/* Position we need to pad to, as soon as we know that we have input
   remaining to be printed. */
நிலையான முழு padding_not_printed;

/* True means we should pad the end of the page.  Remains false until we
   know we have a page to print. */
நிலையான bool pad_vertically;

/* (-h) String of characters used in place of the filename in the header. */
நிலையான எழுத்து *custom_header;

/* (-D) Date format for the header.  */
நிலையான எழுத்து மாறிலி *date_format;

/* The local time zone rules, as per the TZ environment variable.  */
நிலையான timezone_t localtz;

/* Date and file name for the header.  */
நிலையான எழுத்து *date_text;
நிலையான எழுத்து மாறிலி *file_text;

/* Output columns available, not counting the date and file name.  */
நிலையான முழு header_width_available;

நிலையான எழுத்து *clump_buff;

/* True means we read the line no. lines_per_body in skip_read
   called by skip_to_page. That variable controls the coincidence of a
   "FF set by hand" and "full_page_printed", see above the definition of
   structure COLUMN. */
நிலையான bool last_வரி = false;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
எண்ணிக்கை
{
  COLUMNS_OPTION = CHAR_MAX + 1,
  PAGES_OPTION
};

நிலையான எழுத்து மாறிலி short_options[] =
  "-0123456789D:FJN:S::TW:abcde::fh:i::l:mn::o:rs::tvw:";

நிலையான கட்டமைப்பு option மாறிலி long_options[] =
{
  {"pages", required_argument, வெற்று, PAGES_OPTION},
  {"columns", required_argument, வெற்று, COLUMNS_OPTION},
  {"across", no_argument, வெற்று, 'a'},
  {"show-control-chars", no_argument, வெற்று, 'c'},
  {"double-space", no_argument, வெற்று, 'd'},
  {"date-format", required_argument, வெற்று, 'D'},
  {"expand-tabs", optional_argument, வெற்று, 'e'},
  {"form-feed", no_argument, வெற்று, 'f'},
  {"header", required_argument, வெற்று, 'h'},
  {"output-tabs", optional_argument, வெற்று, 'i'},
  {"join-lines", no_argument, வெற்று, 'J'},
  {"length", required_argument, வெற்று, 'l'},
  {"merge", no_argument, வெற்று, 'm'},
  {"number-lines", optional_argument, வெற்று, 'n'},
  {"first-line-number", required_argument, வெற்று, 'N'},
  {"indent", required_argument, வெற்று, 'o'},
  {"no-file-warnings", no_argument, வெற்று, 'r'},
  {"separator", optional_argument, வெற்று, 's'},
  {"sep-string", optional_argument, வெற்று, 'S'},
  {"omit-header", no_argument, வெற்று, 't'},
  {"omit-pagination", no_argument, வெற்று, 'T'},
  {"show-nonprinting", no_argument, வெற்று, 'v'},
  {"width", required_argument, வெற்று, 'w'},
  {"page-width", required_argument, வெற்று, 'W'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {வெற்று, 0, வெற்று, 0}
};

நிலையான _Noreturn வெறுமை
integer_overflow (வெறுமை)
{
  error (EXIT_FAILURE, 0, _("integer overflow"));
}

/* Return the number of columns that have either an open file or
   stored lines. */

ATTRIBUTE_PURE
நிலையான unsigned முழு
cols_ready_to_print (வெறுமை)
{
  COLUMN *q;
  unsigned முழு i;
  unsigned முழு n;

  n = 0;
  க்கு (q = column_vector, i = 0; i < columns; ++q, ++i)
    எனில் (q->status == OPEN
        || q->status == FF_FOUND	/* With -b: To print a header only */
        || (storing_columns && q->வரிகள்_stored > 0 && q->வரிகள்_to_print > 0))
      ++n;
  திரும்பு n;
}

/* Estimate first_ / last_page_number
   using option +FIRST_PAGE:LAST_PAGE */

நிலையான bool
first_last_page (முழு oi, எழுத்து c, எழுத்து மாறிலி *pages)
{
  எழுத்து *p;
  uintmax_t first;
  uintmax_t last = UINTMAX_MAX;
  strtol_error err = xstrtoumax (pages, &p, 10, &first, "");
  எனில் (err != LONGINT_OK && err != LONGINT_INVALID_SUFFIX_CHAR)
    xstrtol_fatal (err, oi, c, long_options, pages);

  எனில் (p == pages || !first)
    திரும்பு false;

  எனில் (*p == ':')
    {
      எழுத்து மாறிலி *p1 = p + 1;
      err = xstrtoumax (p1, &p, 10, &last, "");
      எனில் (err != LONGINT_OK)
        xstrtol_fatal (err, oi, c, long_options, pages);
      எனில் (p1 == p || last < first)
        திரும்பு false;
    }

  எனில் (*p)
    திரும்பு false;

  first_page_எண் = first;
  last_page_எண் = last;
  திரும்பு true;
}

/* Parse column count string S, and if it's valid (1 or larger and
   within range of the type of 'columns') set the global variables
   columns and explicit_columns.  Otherwise, exit with a diagnostic.  */

நிலையான வெறுமை
parse_column_மொத்தம் (எழுத்து மாறிலி *s)
{
  columns = getoptnum (s, 1, _("invalid number of columns"));
  explicit_columns = true;
}

/* Estimate length of col_sep_string with option -S.  */

நிலையான வெறுமை
separator_string (எழுத்து மாறிலி *optarg_S)
{
  size_t len = strlen (optarg_S);
  எனில் (INT_MAX < len)
    integer_overflow ();
  col_sep_length = len;
  col_sep_string = optarg_S;
}

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  unsigned முழு n_files;
  bool old_options = false;
  bool old_w = false;
  bool old_s = false;
  எழுத்து **file_names;

  /* Accumulate the digits of old-style options like -99.  */
  எழுத்து *column_மொத்தம்_string = வெற்று;
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
                ? xnmalloc (argc - 1, அளவு (எழுத்து *))
                : வெற்று);

  போது (true)
    {
      முழு oi = -1;
      முழு c = getopt_long (argc, argv, short_options, long_options, &oi);
      எனில் (c == -1)
        நிறுத்து;

      எனில் (c_isdigit (c))
        {
          /* Accumulate column-count digits specified via old-style options. */
          எனில் (n_digits + 1 >= n_alloc)
            column_மொத்தம்_string = xpalloc (column_மொத்தம்_string, &n_alloc, 2, -1,
                                           அளவு *column_மொத்தம்_string);
          column_மொத்தம்_string[n_digits++] = c;
          column_மொத்தம்_string[n_digits] = '\0';
          தொடர்;
        }

      n_digits = 0;

      தேர்வு (c)
        {
        நிலை 1:			/* Non-option argument. */
          /* long option --page dominates old '+FIRST_PAGE ...'.  */
          எனில் (! (first_page_எண் == 0
                 && *optarg == '+' && first_last_page (-2, '+', optarg + 1)))
            file_names[n_files++] = optarg;
          நிறுத்து;

        நிலை PAGES_OPTION:	/* --pages=FIRST_PAGE[:LAST_PAGE] */
          {			/* dominates old opt +... */
            எனில் (! optarg)
              error (EXIT_FAILURE, 0,
                     _("'--pages=FIRST_PAGE[:LAST_PAGE]' missing argument"));
            இல்லையெனில் எனில் (! first_last_page (oi, 0, optarg))
              error (EXIT_FAILURE, 0, _("invalid page range %s"),
                     quote (optarg));
            நிறுத்து;
          }

        நிலை COLUMNS_OPTION:	/* --columns=COLS */
          {
            parse_column_மொத்தம் (optarg);

            /* If there was a prior column count specified via the
               short-named option syntax, e.g., -9, ensure that this
               long-name-specified value overrides it.  */
            விடுவி (column_மொத்தம்_string);
            column_மொத்தம்_string = வெற்று;
            n_alloc = 0;
            நிறுத்து;
          }

        நிலை 'a':
          print_across_flag = true;
          storing_columns = false;
          நிறுத்து;
        நிலை 'b':
          balance_columns = true;
          நிறுத்து;
        நிலை 'c':
          use_cntrl_prefix = true;
          நிறுத்து;
        நிலை 'd':
          double_space = true;
          நிறுத்து;
        நிலை 'D':
          date_format = optarg;
          நிறுத்து;
        நிலை 'e':
          எனில் (optarg)
            getoptarg (optarg, 'e', &உள்ளீடு_tab_char,
                       &chars_per_உள்ளீடு_tab);
          /* Could check tab width > 0. */
          untabify_உள்ளீடு = true;
          நிறுத்து;
        நிலை 'f':
        நிலை 'F':
          use_form_feed = true;
          நிறுத்து;
        நிலை 'h':
          custom_header = optarg;
          நிறுத்து;
        நிலை 'i':
          எனில் (optarg)
            getoptarg (optarg, 'i', &output_tab_char,
                       &chars_per_output_tab);
          /* Could check tab width > 0. */
          tabify_output = true;
          நிறுத்து;
        நிலை 'J':
          join_வரிகள் = true;
          நிறுத்து;
        நிலை 'l':
          வரிகள்_per_page
            = getoptnum (optarg, 1,
                         _("'-l PAGE_LENGTH' invalid number of lines"));
          நிறுத்து;
        நிலை 'm':
          parallel_files = true;
          storing_columns = false;
          நிறுத்து;
        நிலை 'n':
          numbered_வரிகள் = true;
          எனில் (optarg)
            getoptarg (optarg, 'n', &எண்_separator,
                       &chars_per_எண்);
          நிறுத்து;
        நிலை 'N':
          skip_மொத்தம் = false;
          start_வரி_இலக்கம்
            = getoptnum (optarg, INT_MIN,
                         _("'-N NUMBER' invalid starting line number"));
          நிறுத்து;
        நிலை 'o':
          chars_per_margin = getoptnum (optarg, 0,
                                        _("'-o MARGIN' invalid line offset"));
          நிறுத்து;
        நிலை 'r':
          ignore_failed_opens = true;
          நிறுத்து;
        நிலை 's':
          old_options = true;
          old_s = true;
          எனில் (!use_col_separator && optarg)
            separator_string (optarg);
          நிறுத்து;
        நிலை 'S':
          old_s = false;
          /* Reset an additional input of -s, -S dominates -s */
          col_sep_string = "";
          col_sep_length = 0;
          use_col_separator = true;
          எனில் (optarg)
            separator_string (optarg);
          நிறுத்து;
        நிலை 't':
          extremities = false;
          keep_FF = true;
          நிறுத்து;
        நிலை 'T':
          extremities = false;
          keep_FF = false;
          நிறுத்து;
        நிலை 'v':
          use_esc_sequence = true;
          நிறுத்து;
        நிலை 'w':
          old_options = true;
          old_w = true;
          {
            முழு tmp_cpl
              = getoptnum (optarg, 1,
                           _("'-w PAGE_WIDTH' invalid number of characters"));
            எனில் (! truncate_வரிகள்)
              chars_per_வரி = tmp_cpl;
          }
          நிறுத்து;
        நிலை 'W':
          old_w = false;			/* dominates -w */
          truncate_வரிகள் = true;
          chars_per_வரி
            = getoptnum (optarg, 1,
                         _("'-W PAGE_WIDTH' invalid number of characters"));
          நிறுத்து;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        இயல்பு:
          usage (EXIT_FAILURE);
          நிறுத்து;
        }
    }

  எனில் (column_மொத்தம்_string)
    {
      parse_column_மொத்தம் (column_மொத்தம்_string);
      விடுவி (column_மொத்தம்_string);
    }

  எனில் (! date_format)
    date_format = (getenv ("POSIXLY_CORRECT") && !hard_locale (LC_TIME)
                   ? "%b %e %H:%M %Y"
                   : "%Y-%m-%d %H:%M");

  localtz = tzalloc (getenv ("TZ"));

  /* Now we can set a reasonable initial value: */
  எனில் (first_page_எண் == 0)
    first_page_எண் = 1;

  எனில் (parallel_files && explicit_columns)
    error (EXIT_FAILURE, 0,
           _("cannot specify number of columns when printing in parallel"));

  எனில் (parallel_files && print_across_flag)
    error (EXIT_FAILURE, 0,
           _("cannot specify both printing across and printing in parallel"));

/* Translate some old short options to new/long options.
   To meet downward compatibility with other UNIX pr utilities
   and some POSIX specifications. */

  எனில் (old_options)
    {
      எனில் (old_w)
        {
          எனில் (parallel_files || explicit_columns)
            {
              /* activate -W */
              truncate_வரிகள் = true;
              எனில் (old_s)
                /* adapt HP-UX and SunOS: -s = no separator;
                   activate -S */
                use_col_separator = true;
            }
          இல்லையெனில்
            /* old -w sets width with columns only
               activate -J */
            join_வரிகள் = true;
        }
      இல்லையெனில் எனில் (!use_col_separator)
        {
          /* No -S option read */
          எனில் (old_s && (parallel_files || explicit_columns))
            {
              எனில் (!truncate_வரிகள்)
                {
                  /* old -s (without -w and -W) annuls column alignment,
                  uses fields, activate -J */
                  join_வரிகள் = true;
                  எனில் (col_sep_length > 0)
                    /* activate -S */
                    use_col_separator = true;
                }
              இல்லையெனில்
                /* with -W */
                /* adapt HP-UX and SunOS: -s = no separator;
                   activate -S */
                use_col_separator = true;
            }
        }
    }

  க்கு (; optind < argc; optind++)
    {
      file_names[n_files++] = argv[optind];
    }

  எனில் (n_files == 0)
    {
      /* No file arguments specified;  read from standard input.  */
      print_files (0, வெற்று);
    }
  இல்லையெனில்
    {
      எனில் (parallel_files)
        print_files (n_files, file_names);
      இல்லையெனில்
        {
          க்கு (unsigned முழு i = 0; i < n_files; i++)
            print_files (1, &file_names[i]);
        }
    }

  cleanup ();

  எனில் (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, _("standard input"));
  main_exit (failed_opens ? EXIT_FAILURE : EXIT_SUCCESS);
}

/* Parse numeric arguments, ensuring MIN <= number <= INT_MAX.  */

நிலையான முழு
getoptnum (எழுத்து மாறிலி *n_str, முழு min, எழுத்து மாறிலி *err)
{
  திரும்பு xnumtoimax (n_str, 10, min, INT_MAX, "", err, 0,
                     min <= 0 ? 0 : XTOINT_MIN_RANGE);
}

/* Parse options of the form -scNNN.

   Example: -nck, where 'n' is the option, c is the optional number
   separator, and k is the optional width of the field used when printing
   a number. */

நிலையான வெறுமை
getoptarg (எழுத்து *arg, எழுத்து switch_char, எழுத்து *character, முழு *எண்)
{
  எழுத்து மாறிலி *இலக்கம் = arg;

  எனில் (!*இலக்கம்)
    {
      error (0, 0, _("'-%c': Invalid argument: %s"), switch_char, quote (இலக்கம்));
      usage (EXIT_FAILURE);
    }

  எனில் (!c_isdigit (*இலக்கம்))
    *character = *இலக்கம்++;
  எனில் (*இலக்கம்)
    {
      long முழு tmp_long;
      strtol_error e = xstrtol (இலக்கம், வெற்று, 10, &tmp_long, "");
      எனில் (e == LONGINT_OK)
        {
          எனில் (tmp_long <= 0)
            e = LONGINT_INVALID;
          இல்லையெனில் எனில் (INT_MAX < tmp_long)
            e = LONGINT_OVERFLOW;
        }
      எனில் (e != LONGINT_OK)
        {
          error (0, e & LONGINT_OVERFLOW ? EOVERFLOW : 0,
             _("'-%c' extra characters or invalid number in the argument: %s"),
                 switch_char, quote (arg));
          usage (EXIT_FAILURE);
        }
      *எண் = tmp_long;
    }
}

/* Set parameters related to formatting. */

நிலையான வெறுமை
init_parameters (முழு எண்_of_files)
{
  முழு chars_used_by_எண் = 0;

  வரிகள்_per_body = வரிகள்_per_page - வரிகள்_per_header - வரிகள்_per_footer;
  எனில் (வரிகள்_per_body <= 0)
    {
      extremities = false;
      keep_FF = true;
    }
  எனில் (! extremities)
    வரிகள்_per_body = வரிகள்_per_page;

  எனில் (double_space)
    வரிகள்_per_body = MAX (1, வரிகள்_per_body / 2);

  /* If input is stdin, cannot print parallel files.  BSD dumps core
     on this. */
  எனில் (எண்_of_files == 0)
    parallel_files = false;

  எனில் (parallel_files)
    columns = எண்_of_files;

  /* One file, multi columns down: -b option is set to get a consistent
     formulation with "FF set by hand" in input files. */
  எனில் (storing_columns)
    balance_columns = true;

  /* Tabification is assumed for multiple columns. */
  எனில் (columns > 1)
    {
      எனில் (!use_col_separator)
        {
          /* Use default separator */
          எனில் (join_வரிகள்)
            col_sep_string = வரி_separator;
          இல்லையெனில்
            col_sep_string = column_separator;

          col_sep_length = 1;
          use_col_separator = true;
        }
      /* It's rather pointless to define a TAB separator with column
         alignment */
      இல்லையெனில் எனில் (!join_வரிகள் && col_sep_length == 1 && *col_sep_string == '\t')
        col_sep_string = column_separator;

      truncate_வரிகள் = true;
      எனில் (! (col_sep_length == 1 && *col_sep_string == '\t'))
        untabify_உள்ளீடு = true;
      tabify_output = true;
    }
  இல்லையெனில்
    storing_columns = false;

  /* -J dominates -w in any case */
  எனில் (join_வரிகள்)
    truncate_வரிகள் = false;

  எனில் (numbered_வரிகள்)
    {
      முழு chars_per_default_tab = 8;

      வரி_மொத்தம் = start_வரி_இலக்கம்;

      /* To allow input tab-expansion (-e sensitive) use:
         if (number_separator == input_tab_char)
           number_width = chars_per_number
             + TAB_WIDTH (chars_per_input_tab, chars_per_number);   */

      /* Estimate chars_per_text without any margin and keep it constant. */
      எனில் (எண்_separator == '\t')
        எண்_width = (chars_per_எண்
                        + TAB_WIDTH (chars_per_default_tab, chars_per_எண்));
      இல்லையெனில்
        எண்_width = chars_per_எண் + 1;

      /* The number is part of the column width unless we are
         printing files in parallel. */
      எனில் (parallel_files)
        chars_used_by_எண் = எண்_width;
    }

  முழு sep_chars, useful_chars;
  எனில் (ckd_mul (&sep_chars, columns - 1, col_sep_length))
    sep_chars = INT_MAX;
  எனில் (ckd_sub (&useful_chars, chars_per_வரி - chars_used_by_எண்,
               sep_chars))
    useful_chars = 0;
  chars_per_column = useful_chars / columns;

  எனில் (chars_per_column < 1)
    error (EXIT_FAILURE, 0, _("page width too narrow"));

  எனில் (numbered_வரிகள்)
    {
      விடுவி (எண்_buff);
      எண்_buff = xmalloc (MAX (chars_per_எண்,
                                  INT_STRLEN_BOUND (வரி_எண்)) + 1);
    }

  /* Pick the maximum between the tab width and the width of an
     escape sequence.
     The width of an escape sequence (4) isn't the lower limit any longer.
     We've to use 8 as the lower limit, if we use chars_per_default_tab = 8
     to expand a tab which is not an input_tab-char. */
  விடுவி (clump_buff);
  clump_buff = xmalloc (MAX (8, chars_per_உள்ளீடு_tab));
}

/* Open the necessary files,
   maintaining a COLUMN structure for each column.

   With multiple files, each column p has a different p->fp.
   With single files, each column p has the same p->fp.
   Return false if (number_of_files > 0) and no files can be opened,
   true otherwise.

   With each column/file p, p->full_page_printed is initialized,
   see also open_file.  */

நிலையான bool
init_fps (முழு எண்_of_files, எழுத்து **av)
{
  COLUMN *p;

  total_files = 0;

  விடுவி (column_vector);
  column_vector = xnmalloc (columns, அளவு (COLUMN));

  எனில் (parallel_files)
    {
      முழு files_இடது = எண்_of_files;
      க்கு (p = column_vector; files_இடது--; ++p, ++av)
        {
          எனில் (! open_file (*av, p))
            {
              --p;
              --columns;
            }
        }
      எனில் (columns == 0)
        திரும்பு false;
      init_header ("", -1);
    }
  இல்லையெனில்
    {
      p = column_vector;
      எனில் (எண்_of_files > 0)
        {
          எனில் (! open_file (*av, p))
            திரும்பு false;
          init_header (*av, fileno (p->fp));
          p->வரிகள்_stored = 0;
        }
      இல்லையெனில்
        {
          p->name = _("standard input");
          p->fp = stdin;
          have_read_stdin = true;
          p->status = OPEN;
          p->full_page_printed = false;
          ++total_files;
          init_header ("", -1);
          p->வரிகள்_stored = 0;
        }

      எழுத்து மாறிலி *firstname = p->name;
      FILE *firstfp = p->fp;
      முழு i;
      க்கு (i = columns - 1, ++p; i; --i, ++p)
        {
          p->name = firstname;
          p->fp = firstfp;
          p->status = OPEN;
          p->full_page_printed = false;
          p->வரிகள்_stored = 0;
        }
    }
  files_ready_to_read = total_files;
  திரும்பு true;
}

/* Determine print_func and char_func, the functions
   used by each column for printing and/or storing.

   Determine the horizontal position desired when we begin
   printing a column (p->start_position). */

நிலையான வெறுமை
init_funcs (வெறுமை)
{
  முழு i, h, h_next;
  COLUMN *p;

  h = chars_per_margin;

  எனில் (!truncate_வரிகள்)
    h_next = ANYWHERE;
  இல்லையெனில்
    {
      /* When numbering lines of parallel files, we enlarge the
         first column to accommodate the number.  Looks better than
         the Sys V approach. */
      எனில் (parallel_files && numbered_வரிகள்)
        h_next = h + chars_per_column + எண்_width;
      இல்லையெனில்
        h_next = h + chars_per_column;
    }

  /* Enlarge p->start_position of first column to use the same form of
     padding_not_printed with all columns. */
  h = h + col_sep_length;

  /* This loop takes care of all but the rightmost column. */

  க்கு (p = column_vector, i = 1; i < columns; ++p, ++i)
    {
      எனில் (storing_columns)	/* One file, multi columns down. */
        {
          p->char_func = store_char;
          p->print_func = print_stored;
        }
      இல்லையெனில்
        /* One file, multi columns across; or parallel files.  */
        {
          p->char_func = print_char;
          p->print_func = read_வரி;
        }

      /* Number only the first column when printing files in
         parallel. */
      p->numbered = numbered_வரிகள் && (!parallel_files || i == 1);
      p->start_position = h;

      /* If we don't truncate lines, all start_positions are
         ANYWHERE, except the first column's start_position when
         using a margin. */

      எனில் (!truncate_வரிகள்)
        {
          h = ANYWHERE;
          h_next = ANYWHERE;
        }
      இல்லையெனில்
        {
          h = h_next + col_sep_length;
          h_next = h + chars_per_column;
        }
    }

  /* The rightmost column.

     Doesn't need to be stored unless we intend to balance
     columns on the last page. */
  எனில் (storing_columns && balance_columns)
    {
      p->char_func = store_char;
      p->print_func = print_stored;
    }
  இல்லையெனில்
    {
      p->char_func = print_char;
      p->print_func = read_வரி;
    }

  p->numbered = numbered_வரிகள் && (!parallel_files || i == 1);
  p->start_position = h;
}

/* Open a file.  Return true if successful.

   With each file p, p->full_page_printed is initialized,
   see also init_fps. */

நிலையான bool
open_file (எழுத்து *name, COLUMN *p)
{
  எனில் (streq (name, "-"))
    {
      p->name = _("standard input");
      p->fp = stdin;
      have_read_stdin = true;
    }
  இல்லையெனில்
    {
      p->name = name;
      p->fp = fopen (name, "r");
    }
  எனில் (p->fp == வெற்று)
    {
      failed_opens = true;
      எனில் (!ignore_failed_opens)
        error (0, errno, "%s", quotef (name));
      திரும்பு false;
    }
  fadvise (p->fp, FADVISE_SEQUENTIAL);
  p->status = OPEN;
  p->full_page_printed = false;
  ++total_files;
  திரும்பு true;
}

/* Close the file in P.

   If we aren't dealing with multiple files in parallel, we change
   the status of all columns in the column list to reflect the close. */

நிலையான வெறுமை
close_file (COLUMN *p)
{
  COLUMN *q;
  முழு i;

  எனில் (p->status == CLOSED)
    திரும்பு;

  முழு err = errno;
  எனில் (!ferror (p->fp))
    err = 0;
  எனில் (fileno (p->fp) == STDIN_FILENO)
    clearerr (p->fp);
  இல்லையெனில் எனில் (fclose (p->fp) != 0 && !err)
    err = errno;
  எனில் (err)
    error (EXIT_FAILURE, err, "%s", quotef (p->name));

  எனில் (!parallel_files)
    {
      க்கு (q = column_vector, i = columns; i; ++q, --i)
        {
          q->status = CLOSED;
          எனில் (q->வரிகள்_stored == 0)
            {
              q->வரிகள்_to_print = 0;
            }
        }
    }
  இல்லையெனில்
    {
      p->status = CLOSED;
      p->வரிகள்_to_print = 0;
    }

  --files_ready_to_read;
}

/* Put a file on hold until we start a new page,
   since we've hit a form feed.

   If we aren't dealing with parallel files, we must change the
   status of all columns in the column list. */

நிலையான வெறுமை
hold_file (COLUMN *p)
{
  COLUMN *q;
  முழு i;

  எனில் (!parallel_files)
    க்கு (q = column_vector, i = columns; i; ++q, --i)
      {
        எனில் (storing_columns)
          q->status = FF_FOUND;
        இல்லையெனில்
          q->status = ON_HOLD;
      }
  இல்லையெனில்
    p->status = ON_HOLD;

  p->வரிகள்_to_print = 0;
  --files_ready_to_read;
}

/* Undo hold_file -- go through the column list and change any
   ON_HOLD columns to OPEN.  Used at the end of each page. */

நிலையான வெறுமை
reset_status (வெறுமை)
{
  முழு i = columns;
  COLUMN *p;

  க்கு (p = column_vector; i; --i, ++p)
    எனில் (p->status == ON_HOLD)
      {
        p->status = OPEN;
        files_ready_to_read++;
      }

  எனில் (storing_columns)
    {
      எனில் (column_vector->status == CLOSED)
        /* We use the info to output an error message in  skip_to_page. */
        files_ready_to_read = 0;
      இல்லையெனில்
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

நிலையான வெறுமை
print_files (முழு எண்_of_files, எழுத்து **av)
{
  init_parameters (எண்_of_files);
  எனில் (! init_fps (எண்_of_files, av))
    திரும்பு;
  எனில் (storing_columns)
    init_store_cols ();

  எனில் (first_page_எண் > 1)
    {
      எனில் (!skip_to_page (first_page_எண்))
        திரும்பு;
      இல்லையெனில்
        page_எண் = first_page_எண்;
    }
  இல்லையெனில்
    page_எண் = 1;

  init_funcs ();

  வரி_எண் = வரி_மொத்தம்;
  போது (print_page ())
    ;
}

/* Initialize header information.
   If DESC is non-negative, it is a file descriptor open to
   FILENAME for reading.  */

நிலையான வெறுமை
init_header (எழுத்து மாறிலி *filename, முழு desc)
{
  எழுத்து *buf = வெற்று;
  கட்டமைப்பு stat st;
  கட்டமைப்பு timespec t;
  முழு ns;
  கட்டமைப்பு tm tm;

  /* If parallel files or standard input, use current date. */
  எனில் (streq (filename, "-"))
    desc = -1;
  எனில் (0 <= desc && fstat (desc, &st) == 0)
    t = get_stat_mtime (&st);
  இல்லையெனில்
    {
      நிலையான கட்டமைப்பு timespec timespec;
      எனில் (! timespec.tv_sec)
        gettime (&timespec);
      t = timespec;
    }

  ns = t.tv_nsec;
  எனில் (localtime_rz (localtz, &t.tv_sec, &tm))
    {
      ptrdiff_t len = nstrftime (வெற்று, MIN (PTRDIFF_MAX, SIZE_MAX),
                                 date_format, &tm, localtz, ns);
      எனில் (0 <= len)
        {
          buf = ximalloc (len + 1);
          nstrftime (buf, len + 1, date_format, &tm, localtz, ns);
        }
    }
  எனில் (!buf)
    {
      எழுத்து secbuf[INT_BUFSIZE_BOUND (intmax_t)];
      buf = xmalloc (அளவு secbuf + MAX (10, INT_BUFSIZE_BOUND (முழு)));
      sprintf (buf, "%s.%09d", timetostr (t.tv_sec, secbuf), ns);
    }

  விடுவி (date_text);
  date_text = buf;
  file_text = custom_header ? custom_header : desc < 0 ? "" : filename;
  header_width_available = (chars_per_வரி
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

நிலையான வெறுமை
init_page (வெறுமை)
{
  முழு j;
  COLUMN *p;

  எனில் (storing_columns)
    {
      store_columns ();
      க்கு (j = columns - 1, p = column_vector; j; --j, ++p)
        {
          p->வரிகள்_to_print = p->வரிகள்_stored;
        }

      /* Last column. */
      எனில் (balance_columns)
        {
          p->வரிகள்_to_print = p->வரிகள்_stored;
        }
      /* Since we're not balancing columns, we don't need to store
         the rightmost column.   Read it straight from the file. */
      இல்லையெனில்
        {
          எனில் (p->status == OPEN)
            {
              p->வரிகள்_to_print = வரிகள்_per_body;
            }
          இல்லையெனில்
            p->வரிகள்_to_print = 0;
        }
    }
  இல்லையெனில்
    க்கு (j = columns, p = column_vector; j; --j, ++p)
      எனில் (p->status == OPEN)
        {
          p->வரிகள்_to_print = வரிகள்_per_body;
        }
      இல்லையெனில்
        p->வரிகள்_to_print = 0;
}

/* Align empty columns and print separators.
   Empty columns will be formed by files with status ON_HOLD or CLOSED
   when printing multiple files in parallel. */

நிலையான வெறுமை
align_column (COLUMN *p)
{
  padding_not_printed = p->start_position;
  எனில் (col_sep_length < padding_not_printed)
    {
      pad_across_to (padding_not_printed - col_sep_length);
      padding_not_printed = ANYWHERE;
    }

  எனில் (use_col_separator)
    print_sep_string ();

  எனில் (p->numbered)
    add_வரி_எண் (p);
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

நிலையான bool
print_page (வெறுமை)
{
  முழு j;
  முழு வரிகள்_இடது_on_page;
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

  எனில் (cols_ready_to_print () == 0)
    திரும்பு false;

  எனில் (extremities)
    print_a_header = true;

  /* Don't pad unless we know a page was printed. */
  pad_vertically = false;
  pv = false;

  வரிகள்_இடது_on_page = வரிகள்_per_body;
  எனில் (double_space)
    வரிகள்_இடது_on_page *= 2;

  போது (வரிகள்_இடது_on_page > 0 && cols_ready_to_print () > 0)
    {
      output_position = 0;
      spaces_not_printed = 0;
      separators_not_printed = 0;
      pad_vertically = false;
      align_empty_cols = false;
      empty_வரி = true;

      க்கு (j = 1, p = column_vector; j <= columns; ++j, ++p)
        {
          உள்ளீடு_position = 0;
          எனில் (p->வரிகள்_to_print > 0 || p->status == FF_FOUND)
            {
              FF_only = false;
              padding_not_printed = p->start_position;
              எனில் (!(p->print_func) (p))
                read_rest_of_வரி (p);
              pv |= pad_vertically;

              --p->வரிகள்_to_print;
              எனில் (p->வரிகள்_to_print <= 0)
                {
                  எனில் (cols_ready_to_print () == 0)
                    நிறுத்து;
                }

              /* File p changed its status to ON_HOLD or CLOSED */
              எனில் (parallel_files && p->status != OPEN)
                {
                  எனில் (empty_வரி)
                    align_empty_cols = true;
                  இல்லையெனில் எனில் (p->status == CLOSED
                           || (p->status == ON_HOLD && FF_only))
                    align_column (p);
                }
            }
          இல்லையெனில் எனில் (parallel_files)
            {
              /* File status ON_HOLD or CLOSED */
              எனில் (empty_வரி)
                align_empty_cols = true;
              இல்லையெனில்
                align_column (p);
            }

          /* We need it also with an empty column */
          எனில் (use_col_separator)
            ++separators_not_printed;
        }

      எனில் (pad_vertically)
        {
          putchar ('\n');
          --வரிகள்_இடது_on_page;
        }

      எனில் (cols_ready_to_print () == 0 && !extremities)
        நிறுத்து;

      எனில் (double_space && pv)
        {
          putchar ('\n');
          --வரிகள்_இடது_on_page;
        }
    }

  எனில் (வரிகள்_இடது_on_page == 0)
    க்கு (j = 1, p = column_vector; j <= columns; ++j, ++p)
      எனில் (p->status == OPEN)
        p->full_page_printed = true;

  pad_vertically = pv;

  எனில் (pad_vertically && extremities)
    pad_down (வரிகள்_இடது_on_page + வரிகள்_per_footer);
  இல்லையெனில் எனில் (keep_FF && print_a_FF)
    {
      putchar ('\f');
      print_a_FF = false;
    }

  எனில் (ferror (stdout))
    write_error ();

  எனில் (last_page_எண் < ++page_எண்)
    திரும்பு false;		/* Stop printing with LAST_PAGE */

  reset_status ();		/* Change ON_HOLD to OPEN. */

  திரும்பு true;			/* More pages to go. */
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

நிலையான வெறுமை
init_store_cols (வெறுமை)
{
  /* Tune this.  */
  முழு total_வரிகள், total_வரிகள்_1, chars_per_column_1, chars_if_truncate;
  எனில் (ckd_mul (&total_வரிகள், வரிகள்_per_body, columns)
      || ckd_add (&total_வரிகள்_1, total_வரிகள், 1)
      || ckd_add (&chars_per_column_1, chars_per_column, 1)
      || ckd_mul (&chars_if_truncate, total_வரிகள், chars_per_column_1)
      || ckd_mul (&buff_allocated, chars_if_truncate, use_col_separator + 1))
    integer_overflow ();

  விடுவி (வரி_vector);
  /* FIXME: here's where it was allocated.  */
  வரி_vector = xnmalloc (total_வரிகள்_1, அளவு *வரி_vector);

  விடுவி (end_vector);
  end_vector = xnmalloc (total_வரிகள், அளவு *end_vector);

  விடுவி (buff);
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

நிலையான வெறுமை
store_columns (வெறுமை)
{
  முழு i, j;
  unsigned முழு வரி = 0;
  unsigned முழு buff_start;
  முழு last_col;		/* The rightmost column which will be saved in buff */
  COLUMN *p;

  buff_current = 0;
  buff_start = 0;

  எனில் (balance_columns)
    last_col = columns;
  இல்லையெனில்
    last_col = columns - 1;

  க்கு (i = 1, p = column_vector; i <= last_col; ++i, ++p)
    p->வரிகள்_stored = 0;

  க்கு (i = 1, p = column_vector; i <= last_col && files_ready_to_read;
       ++i, ++p)
    {
      p->current_வரி = வரி;
      க்கு (j = வரிகள்_per_body; j && files_ready_to_read; --j)

        எனில் (p->status == OPEN)	/* Redundant.  Clean up. */
          {
            உள்ளீடு_position = 0;

            எனில் (!read_வரி (p))
              read_rest_of_வரி (p);

            எனில் (p->status == OPEN
                || buff_start != buff_current)
              {
                ++p->வரிகள்_stored;
                வரி_vector[வரி] = buff_start;
                end_vector[வரி++] = உள்ளீடு_position;
                buff_start = buff_current;
              }
          }
    }

  /* Keep track of the location of the last char in buff. */
  வரி_vector[வரி] = buff_start;

  எனில் (balance_columns)
    balance (வரி);
}

நிலையான வெறுமை
balance (முழு total_stored)
{
  COLUMN *p;
  முழு i, வரிகள்;
  முழு first_வரி = 0;

  க்கு (i = 1, p = column_vector; i <= columns; ++i, ++p)
    {
      வரிகள் = total_stored / columns;
      எனில் (i <= total_stored % columns)
        ++வரிகள்;

      p->வரிகள்_stored = வரிகள்;
      p->current_வரி = first_வரி;

      first_வரி += வரிகள்;
    }
}

/* Store a character in the buffer. */

நிலையான வெறுமை
store_char (எழுத்து c)
{
  எனில் (buff_current >= buff_allocated)
    {
      /* May be too generous. */
      buff = xpalloc (buff, &buff_allocated, 1, -1, அளவு *buff);
    }
  buff[buff_current++] = c;
}

நிலையான வெறுமை
add_வரி_எண் (COLUMN *p)
{
  முழு i;
  எழுத்து *s;
  முழு இலக்கம்_width;

  /* Cutting off the higher-order digits is more informative than
     lower-order cut off. */
  இலக்கம்_width = sprintf (எண்_buff, "%*d", chars_per_எண், வரி_எண்);
  வரி_எண்++;
  s = எண்_buff + (இலக்கம்_width - chars_per_எண்);
  க்கு (i = chars_per_எண்; i > 0; i--)
    (p->char_func) (*s++);

  எனில் (columns > 1)
    {
      /* Tabification is assumed for multiple columns, also for n-separators,
         but 'default n-separator = TAB' hasn't been given priority over
         equal column_width also specified by POSIX. */
      எனில் (எண்_separator == '\t')
        {
          i = எண்_width - chars_per_எண்;
          போது (i-- > 0)
            (p->char_func) (' ');
        }
      இல்லையெனில்
        (p->char_func) (எண்_separator);
    }
  இல்லையெனில்
    /* To comply with POSIX, we avoid any expansion of default TAB
       separator with a single column output. No column_width requirement
       has to be considered. */
    {
      (p->char_func) (எண்_separator);
      எனில் (எண்_separator == '\t')
        output_position = POS_AFTER_TAB (chars_per_output_tab,
                          output_position);
    }

  எனில் (truncate_வரிகள் && !parallel_files)
    உள்ளீடு_position += எண்_width;
}

/* Print (or store) padding until the current horizontal position
   is position. */

நிலையான வெறுமை
pad_across_to (முழு position)
{
  முழு h = output_position;

  எனில் (tabify_output)
    spaces_not_printed = position - output_position;
  இல்லையெனில்
    {
      போது (++h <= position)
        putchar (' ');
      output_position = position;
    }
}

/* Pad to the bottom of the page.

   If the user has requested a formfeed, use one.
   Otherwise, use newlines. */

நிலையான வெறுமை
pad_down (unsigned முழு வரிகள்)
{
  எனில் (use_form_feed)
    putchar ('\f');
  இல்லையெனில்
    க்கு (unsigned முழு i = வரிகள்; i; --i)
      putchar ('\n');
}

/* Read the rest of the line.

   Read from the current column's file until an end of line is
   hit.  Used when we've truncated a line and we no longer need
   to print or store its characters. */

நிலையான வெறுமை
read_rest_of_வரி (COLUMN *p)
{
  முழு c;
  FILE *f = p->fp;

  போது ((c = getc (f)) != '\n')
    {
      எனில் (c == '\f')
        {
          எனில் ((c = getc (f)) != '\n')
            ungetc (c, f);
          எனில் (keep_FF)
            print_a_FF = true;
          hold_file (p);
          நிறுத்து;
        }
      இல்லையெனில் எனில் (c == EOF)
        {
          close_file (p);
          நிறுத்து;
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

நிலையான வெறுமை
skip_read (COLUMN *p, முழு column_எண்)
{
  முழு c;
  FILE *f = p->fp;
  முழு i;
  bool single_ff = false;
  COLUMN *q;

  /* Read 1st character in a line or any character succeeding a FF */
  எனில் ((c = getc (f)) == '\f' && p->full_page_printed)
    /* A FF-coincidence with a previous full_page_printed.
       To avoid an additional empty page, eliminate the FF */
    எனில் ((c = getc (f)) == '\n')
      c = getc (f);

  p->full_page_printed = false;

  /* 1st character a FF means a single FF without any printable
     characters. Don't count it as a line with -n option. */
  எனில் (c == '\f')
    single_ff = true;

  /* Preparing for a FF-coincidence: Maybe we finish that page
     without a FF found */
  எனில் (last_வரி)
    p->full_page_printed = true;

  போது (c != '\n')
    {
      எனில் (c == '\f')
        {
          /* No FF-coincidence possible,
             no catching up of a FF-coincidence with next page */
          எனில் (last_வரி)
            {
              எனில் (!parallel_files)
                க்கு (q = column_vector, i = columns; i; ++q, --i)
                  q->full_page_printed = false;
              இல்லையெனில்
                p->full_page_printed = false;
            }

          எனில் ((c = getc (f)) != '\n')
            ungetc (c, f);
          hold_file (p);
          நிறுத்து;
        }
      இல்லையெனில் எனில் (c == EOF)
        {
          close_file (p);
          நிறுத்து;
        }
      c = getc (f);
    }

  எனில் (skip_மொத்தம்)
    எனில் ((!parallel_files || column_எண் == 1) && !single_ff)
      ++வரி_மொத்தம்;
}

/* If we're tabifying output,

   When print_char encounters white space it keeps track
   of our desired horizontal position and delays printing
   until this function is called. */

நிலையான வெறுமை
print_white_space (வெறுமை)
{
  முழு h_new;
  முழு h_old = output_position;
  முழு goal = h_old + spaces_not_printed;

  போது (goal - h_old > 1
         && (h_new = POS_AFTER_TAB (chars_per_output_tab, h_old)) <= goal)
    {
      putchar (output_tab_char);
      h_old = h_new;
    }
  போது (++h_old <= goal)
    putchar (' ');

  output_position = goal;
  spaces_not_printed = 0;
}

/* Print column separators.

   We keep a count until we know that we'll be printing a line,
   then print_sep_string() is called. */

நிலையான வெறுமை
print_sep_string (வெறுமை)
{
  எழுத்து மாறிலி *s = col_sep_string;
  முழு l = col_sep_length;

  எனில் (separators_not_printed <= 0)
    {
      /* We'll be starting a line with chars_per_margin, anything else? */
      எனில் (spaces_not_printed > 0)
        print_white_space ();
    }
  இல்லையெனில்
    {
      க்கு (; separators_not_printed > 0; --separators_not_printed)
        {
          போது (l-- > 0)
            {
              /* 3 types of sep_strings: spaces only, spaces and chars,
              chars only */
              எனில் (*s == ' ')
                {
                  /* We're tabifying output; consecutive spaces in
                  sep_string may have to be converted to tabs */
                  s++;
                  ++spaces_not_printed;
                }
              இல்லையெனில்
                {
                  எனில் (spaces_not_printed > 0)
                    print_white_space ();
                  putchar (*s++);
                  ++output_position;
                }
            }
          /* sep_string ends with some spaces */
          எனில் (spaces_not_printed > 0)
            print_white_space ();
        }
    }
}

/* Print (or store, depending on p->char_func) a clump of N
   characters. */

நிலையான வெறுமை
print_clump (COLUMN *p, முழு n, எழுத்து *clump)
{
  போது (n--)
    (p->char_func) (*clump++);

  எனில் (ferror (stdout))
    write_error ();
}

/* Print a character.

   Update the following comment: process-char hasn't been used any
   longer.
   If we're tabifying, all tabs have been converted to spaces by
   process_char().  Keep a count of consecutive spaces, and when
   a nonspace is encountered, call print_white_space() to print the
   required number of tabs and spaces. */

நிலையான வெறுமை
print_char (எழுத்து c)
{
  எனில் (tabify_output)
    {
      எனில் (c == ' ')
        {
          ++spaces_not_printed;
          திரும்பு;
        }
      இல்லையெனில் எனில் (spaces_not_printed > 0)
        print_white_space ();

      /* Nonprintables are assumed to have width 0, except '\b'. */
      எனில் (! isprint (to_uchar (c)))
        {
          எனில் (c == '\b')
            --output_position;
        }
      இல்லையெனில்
        ++output_position;
    }
  putchar (c);
}

/* Skip to page PAGE before printing.
   PAGE may be larger than total number of pages. */

நிலையான bool
skip_to_page (uintmax_t page)
{
  க்கு (uintmax_t n = 1; n < page; ++n)
    {
      COLUMN *p;
      முழு j;

      க்கு (முழு i = 1; i < வரிகள்_per_body; ++i)
        {
          க்கு (j = 1, p = column_vector; j <= columns; ++j, ++p)
            எனில் (p->status == OPEN)
              skip_read (p, j);
        }
      last_வரி = true;
      க்கு (j = 1, p = column_vector; j <= columns; ++j, ++p)
        எனில் (p->status == OPEN)
          skip_read (p, j);

      எனில் (storing_columns)	/* change FF_FOUND to ON_HOLD */
        க்கு (j = 1, p = column_vector; j <= columns; ++j, ++p)
          எனில் (p->status != CLOSED)
            p->status = ON_HOLD;

      reset_status ();
      last_வரி = false;

      எனில் (files_ready_to_read < 1)
        {
          /* It's very helpful, normally the total number of pages is
             not known in advance.  */
          error (0, 0,
                 _("starting page number %ju exceeds page count %ju"),
                 page, n);
          நிறுத்து;
        }
    }
  திரும்பு files_ready_to_read > 0;
}

/* Print a header.

   Formfeeds are assumed to use up two lines at the beginning of
   the page. */

நிலையான வெறுமை
print_header (வெறுமை)
{
  எழுத்து page_text[256 + INT_STRLEN_BOUND (page_எண்)];
  முழு available_width;
  முழு lhs_spaces;
  முழு rhs_spaces;

  output_position = 0;
  pad_across_to (chars_per_margin);
  print_white_space ();

  எனில் (page_எண் == 0)
    error (EXIT_FAILURE, 0, _("page number overflow"));

  /* The translator must ensure that formatting the translation of
     "Page %ju" does not generate more than (sizeof page_text - 1)
     bytes.  */
  sprintf (page_text, _("Page %ju"), page_எண்);
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

நிலையான bool
read_வரி (COLUMN *p)
{
  முழு c;
  முழு chars IF_LINT (= 0);
  முழு last_உள்ளீடு_position;
  முழு j, k;
  COLUMN *q;

  /* read 1st character in each line or any character succeeding a FF: */
  c = getc (p->fp);

  last_உள்ளீடு_position = உள்ளீடு_position;

  எனில் (c == '\f' && p->full_page_printed)
    எனில் ((c = getc (p->fp)) == '\n')
      c = getc (p->fp);
  p->full_page_printed = false;

  தேர்வு (c)
    {
    நிலை '\f':
      எனில் ((c = getc (p->fp)) != '\n')
        ungetc (c, p->fp);
      FF_only = true;
      எனில் (print_a_header && !storing_columns)
        {
          pad_vertically = true;
          print_header ();
        }
      இல்லையெனில் எனில் (keep_FF)
        print_a_FF = true;
      hold_file (p);
      திரும்பு true;
    நிலை EOF:
      close_file (p);
      திரும்பு true;
    நிலை '\n':
      நிறுத்து;
    இயல்பு:
      chars = char_to_clump (c);
    }

  எனில் (truncate_வரிகள் && உள்ளீடு_position > chars_per_column)
    {
      உள்ளீடு_position = last_உள்ளீடு_position;
      திரும்பு false;
    }

  எனில் (p->char_func != store_char)
    {
      pad_vertically = true;

      எனில் (print_a_header && !storing_columns)
        print_header ();

      எனில் (parallel_files && align_empty_cols)
        {
          /* We have to align empty columns at the beginning of a line. */
          k = separators_not_printed;
          separators_not_printed = 0;
          க்கு (j = 1, q = column_vector; j <= k; ++j, ++q)
            {
              align_column (q);
              separators_not_printed += 1;
            }
          padding_not_printed = p->start_position;
          எனில் (truncate_வரிகள்)
            spaces_not_printed = chars_per_column;
          இல்லையெனில்
            spaces_not_printed = 0;
          align_empty_cols = false;
        }

      எனில் (col_sep_length < padding_not_printed)
        {
          pad_across_to (padding_not_printed - col_sep_length);
          padding_not_printed = ANYWHERE;
        }

      எனில் (use_col_separator)
        print_sep_string ();
    }

  எனில் (p->numbered)
    add_வரி_எண் (p);

  empty_வரி = false;
  எனில் (c == '\n')
    திரும்பு true;

  print_clump (p, chars, clump_buff);

  போது (true)
    {
      c = getc (p->fp);

      தேர்வு (c)
        {
        நிலை '\n':
          திரும்பு true;
        நிலை '\f':
          எனில் ((c = getc (p->fp)) != '\n')
            ungetc (c, p->fp);
          எனில் (keep_FF)
            print_a_FF = true;
          hold_file (p);
          திரும்பு true;
        நிலை EOF:
          close_file (p);
          திரும்பு true;
        }

      last_உள்ளீடு_position = உள்ளீடு_position;
      chars = char_to_clump (c);
      எனில் (truncate_வரிகள் && உள்ளீடு_position > chars_per_column)
        {
          உள்ளீடு_position = last_உள்ளீடு_position;
          திரும்பு false;
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

நிலையான bool
print_stored (COLUMN *p)
{
  COLUMN *q;

  முழு வரி = p->current_வரி++;
  எழுத்து *first = &buff[வரி_vector[வரி]];
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
  எழுத்து *last = &buff[வரி_vector[வரி + 1]];

  pad_vertically = true;

  எனில் (print_a_header)
    print_header ();

  எனில் (p->status == FF_FOUND)
    {
      முழு i;
      க்கு (i = 1, q = column_vector; i <= columns; ++i, ++q)
        q->status = ON_HOLD;
      எனில் (column_vector->வரிகள்_to_print <= 0)
        {
          எனில் (!extremities)
            pad_vertically = false;
          திரும்பு true;		/* print a header only */
        }
    }

  எனில் (col_sep_length < padding_not_printed)
    {
      pad_across_to (padding_not_printed - col_sep_length);
      padding_not_printed = ANYWHERE;
    }

  எனில் (use_col_separator)
    print_sep_string ();

  போது (first != last)
    print_char (*first++);

  எனில் (spaces_not_printed == 0)
    {
      output_position = p->start_position + end_vector[வரி];
      எனில் (p->start_position - col_sep_length == chars_per_margin)
        output_position -= col_sep_length;
    }

  திரும்பு true;
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

நிலையான முழு
char_to_clump (எழுத்து c)
{
  unsigned எழுத்து uc = c;
  எழுத்து *s = clump_buff;
  எழுத்து esc_buff[4];
  முழு width;
  முழு chars;
  முழு chars_per_c = 8;

  எனில் (c == உள்ளீடு_tab_char)
    chars_per_c = chars_per_உள்ளீடு_tab;

  எனில் (c == உள்ளீடு_tab_char || c == '\t')
    {
      width = TAB_WIDTH (chars_per_c, உள்ளீடு_position);

      எனில் (untabify_உள்ளீடு)
        {
          க்கு (முழு i = width; i; --i)
            *s++ = ' ';
          chars = width;
        }
      இல்லையெனில்
        {
          *s = c;
          chars = 1;
        }

    }
  இல்லையெனில் எனில் (! isprint (uc))
    {
      எனில் (use_esc_sequence)
        {
          width = 4;
          chars = 4;
          *s++ = '\\';
          sprintf (esc_buff, "%03o", uc);
          க்கு (முழு i = 0; i <= 2; ++i)
            *s++ = esc_buff[i];
        }
      இல்லையெனில் எனில் (use_cntrl_prefix)
        {
          எனில் (uc < 0200)
            {
              width = 2;
              chars = 2;
              *s++ = '^';
              *s = c ^ 0100;
            }
          இல்லையெனில்
            {
              width = 4;
              chars = 4;
              *s++ = '\\';
              sprintf (esc_buff, "%03o", uc);
              க்கு (முழு i = 0; i <= 2; ++i)
                *s++ = esc_buff[i];
            }
        }
      இல்லையெனில் எனில் (c == '\b')
        {
          width = -1;
          chars = 1;
          *s = c;
        }
      இல்லையெனில்
        {
          width = 0;
          chars = 1;
          *s = c;
        }
    }
  இல்லையெனில்
    {
      width = 1;
      chars = 1;
      *s = c;
    }

  /* Too many backspaces must put us in position 0 -- never negative.  */
  எனில் (width < 0 && உள்ளீடு_position == 0)
    {
      chars = 0;
      உள்ளீடு_position = 0;
    }
  இல்லையெனில் எனில் (width < 0 && உள்ளீடு_position <= -width)
    உள்ளீடு_position = 0;
  இல்லையெனில்
    {
      எனில் (ckd_add (&உள்ளீடு_position, உள்ளீடு_position, width))
        integer_overflow ();
    }

  திரும்பு chars;
}

/* We've just printed some files and need to clean up things before
   looking for more options and printing the next batch of files.

   Free everything we've xmalloc'ed, except 'header'. */

நிலையான வெறுமை
cleanup (வெறுமை)
{
  விடுவி (எண்_buff);
  விடுவி (clump_buff);
  விடுவி (column_vector);
  விடுவி (வரி_vector);
  விடுவி (end_vector);
  விடுவி (buff);
}

/* Complain, print a usage message, and die. */

வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              program_name);

      fputs (_("\
Paginate or columnate FILE(s) க்கு printing.\n\
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
         Balance எண் of வரிகள் in the columns on each page\n\
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
  -d, --இரட்டை-space\n\
         இரட்டை space the output\n\
"));
      oputs (_("\
  -D, --date-format=FORMAT\n\
         use FORMAT க்கு the header date\n\
"));
      oputs (_("\
  -e[CHAR[WIDTH]], --expand-tabs[=CHAR[WIDTH]]\n\
         expand உள்ளீடு CHARs (TABs) to tab WIDTH (8)\n\
"));
      oputs (_("\
  -F, -f, --form-feed\n\
         use form feeds instead of newlines to separate pages\n\
         (by a 3-வரி page header with -F or a 5-வரி header\n\
         and trailer without -F)\n\
"));
      oputs (_("\
  -h, --header=HEADER\n\
         use a centered HEADER instead of filename in page header,\n\
         -h \"\" prints a blank வரி, don't use -h\"\"\n\
"));
      oputs (_("\
  -i[CHAR[WIDTH]], --output-tabs[=CHAR[WIDTH]]\n\
         replace spaces with CHARs (TABs) to tab WIDTH (8)\n\
"));
      oputs (_("\
  -J, --join-வரிகள்\n\
         merge full வரிகள், turns off -W வரி truncation,\n\
         no column alignment, --sep-string[=STRING] sets separators\n\
"));
      oputs (_("\
  -l, --length=PAGE_LENGTH\n\
         set the page length to PAGE_LENGTH (66) வரிகள்\n\
         (இயல்பு எண் of வரிகள் of text 56, and with -F 63).\n\
         implies -t எனில் PAGE_LENGTH <= 10\n\
"));
      oputs (_("\
  -m, --merge\n\
         print all files in parallel, one in each column,\n\
         truncate வரிகள், but join வரிகள் of full length with -J\n\
"));
      oputs (_("\
  -n[SEP[DIGITS]], --எண்-வரிகள்[=SEP[DIGITS]]\n\
         எண் வரிகள், use DIGITS (5) digits, then SEP (TAB),\n\
         இயல்பு counting starts with 1st வரி of உள்ளீடு file\n\
"));
      oputs (_("\
  -N, --first-வரி-எண்=எண்_2\n\
         start counting with எண்_2 at 1st வரி of first\n\
         page printed (see +FIRST_PAGE)\n\
"));
      oputs (_("\
  -o, --indent=MARGIN\n\
         offset each வரி with MARGIN (zero) spaces, செய் not\n\
         affect -w or -W, MARGIN will be added to PAGE_WIDTH\n\
"));
      oputs (_("\
  -r, --no-file-warnings\n\
         omit warning when a file cannot be opened\n\
"));
      oputs (_("\
  -s[CHAR], --separator[=CHAR]\n\
         separate columns by a single character, இயல்பு க்கு CHAR\n\
         is the <TAB> character without -w and \'no எழுத்து\' with -w.\n\
         -s[CHAR] turns off வரி truncation of all 3 column\n\
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
         omit page headers and trailers; implied எனில் PAGE_LENGTH <= 10\n\
"));
      oputs (_("\
  -T, --omit-pagination\n\
         omit page headers and trailers,\n\
         eliminate any pagination by form feeds set in உள்ளீடு files\n\
"));
      oputs (_("\
  -v, --show-nonprinting\n\
         use octal backslash notation\n\
"));
      oputs (_("\
  -w, --width=PAGE_WIDTH\n\
         set page width to PAGE_WIDTH (72) characters க்கு\n\
         multiple text-column output only, -s[எழுத்து] turns off (72)\n\
"));
      oputs (_("\
  -W, --page-width=PAGE_WIDTH\n\
         set page width to PAGE_WIDTH (72) characters always,\n\
         truncate வரிகள், except -J option is set,\n\
         no interference with -S or -s\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}
