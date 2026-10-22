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

#شامل <config.h>

#شامل <getopt.h>
#شامل <sys/types.h>
#شامل "system.h"
#شامل "c-ctype.h"
#شامل "fadvise.h"
#شامل "hard-locale.h"
#شامل "mbswidth.h"
#شامل "quote.h"
#شامل "stat-time.h"
#شامل "stdio--.h"
#شامل "strftime.h"
#شامل "xstrtol.h"
#شامل "xstrtol-error.h"
#شامل "xdectoint.h"

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

ساختار COLUMN;
ساختار COLUMN
  {
    FILE *fp;			/* Input stream for this column. */
    نویسه ثابت *name;		/* File name. */
    شمارش
      {
        OPEN,
        FF_FOUND,		/* used with -b option, set with \f, changed
                                   to ON_HOLD after print_header */
        ON_HOLD,		/* Hit a form feed. */
        CLOSED
      }
    status;			/* Status of the file pointer. */

    /* Func to print lines in this col. */
    bool (*print_func) (ساختار COLUMN *);

    /* Func to print/store chars in this col. */
    تهی (*char_func) (نویسه);

    صحیح current_سطر;		/* Index of current place in line_vector. */
    صحیح سطرها_stored;		/* Number of lines stored in buff. */
    صحیح سطرها_to_print;		/* No. lines stored or space left on page. */
    صحیح start_position;		/* Horizontal position of first char. */
    bool numbered;
    bool full_page_printed;	/* True means printed without a FF found. */

    /* p->full_page_printed  controls a special case of "FF set by hand":
       True means a full page has been printed without FF found. To avoid an
       additional empty page we have to ignore a FF immediately following in
       the next line. */
  };

تعریفنوع ساختار COLUMN COLUMN;

ایستا صحیح char_to_clump (نویسه c);
ایستا bool read_سطر (COLUMN *p);
ایستا bool print_page (تهی);
ایستا bool print_stored (COLUMN *p);
ایستا bool open_file (نویسه *name, COLUMN *p);
ایستا bool skip_to_page (uintmax_t page);
ایستا تهی print_header (تهی);
ایستا تهی pad_across_to (صحیح position);
ایستا تهی add_سطر_عدد (COLUMN *p);
ایستا صحیح getoptnum (نویسه ثابت *n_str, صحیح min, نویسه ثابت *errfmt);
ایستا تهی getoptarg (نویسه *arg, نویسه switch_char, نویسه *character,
                       صحیح *عدد);
ایستا تهی print_files (صحیح عدد_of_files, نویسه **av);
ایستا تهی init_parameters (صحیح عدد_of_files);
ایستا تهی init_header (نویسه ثابت *filename, صحیح desc);
ایستا bool init_fps (صحیح عدد_of_files, نویسه **av);
ایستا تهی init_funcs (تهی);
ایستا تهی init_store_cols (تهی);
ایستا تهی store_columns (تهی);
ایستا تهی balance (صحیح total_stored);
ایستا تهی store_char (نویسه c);
ایستا تهی pad_down (unsigned صحیح سطرها);
ایستا تهی read_rest_of_سطر (COLUMN *p);
ایستا تهی skip_read (COLUMN *p, صحیح column_عدد);
ایستا تهی print_char (نویسه c);
ایستا تهی cleanup (تهی);
ایستا تهی print_sep_string (تهی);
ایستا تهی separator_string (نویسه ثابت *optarg_S);

/* All of the columns to print.  */
ایستا COLUMN *column_vector;

/* When printing a single file in multiple downward columns,
   we store the leftmost columns contiguously in buff.
   To print a line from buff, get the index of the first character
   from line_vector[i], and print up to line_vector[i + 1]. */
ایستا نویسه *buff;

/* Index of the position in buff where the next character
   will be stored. */
ایستا unsigned صحیح buff_current;

/* The number of characters in buff.
   Used for allocation of buff and to detect overflow of buff. */
ایستا idx_t buff_allocated;

/* Array of indices into buff.
   Each entry is an index of the first character of a line.
   This is used when storing lines to facilitate shuffling when
   we do column balancing on the last page. */
ایستا صحیح *سطر_vector;

/* Array of horizontal positions.
   For each line in line_vector, end_vector[line] is the horizontal
   position we are in after printing that line.  We keep track of this
   so that we know how much we need to pad to prepare for the next
   column. */
ایستا صحیح *end_vector;

/* (-m) True means we're printing multiple files in parallel. */
ایستا bool parallel_files = false;

/* (-m) True means a line starts with some empty columns (some files
   already CLOSED or ON_HOLD) which we have to align. */
ایستا bool align_empty_cols;

/* (-m) True means we have not yet found any printable column in a line.
   align_empty_cols = true  has to be maintained. */
ایستا bool empty_سطر;

/* (-m) False means printable column output precedes a form feed found.
   Column alignment is done only once. No additional action with that form
   feed.
   True means we found only a form feed in a column. Maybe we have to do
   some column alignment with that form feed. */
ایستا bool FF_only;

/* (-[0-9]+) True means we're given an option explicitly specifying
   number of columns.  Used to detect when this option is used with -m
   and when translating old options to new/long options. */
ایستا bool explicit_columns = false;

/* (-t|-T) False means we aren't printing headers and footers. */
ایستا bool extremities = true;

/* (-t) True means we retain all FF set by hand in input files.
   False is set with -T option. */
ایستا bool keep_FF = false;
ایستا bool print_a_FF = false;

/* True means we need to print a header as soon as we know we've got input
   to print after it. */
ایستا bool print_a_header;

/* (-f) True means use formfeeds instead of newlines to separate pages. */
ایستا bool use_form_feed = false;

/* True means we have read the standard input. */
ایستا bool have_read_stdin = false;

/* True means the -a flag has been given. */
ایستا bool print_across_flag = false;

/* True means we're printing one file in multiple (>1) downward columns. */
ایستا bool storing_columns = true;

/* (-b) True means balance columns on the last page as Sys V does. */
/* That's no longer an independent option. With storing_columns = true
   balance_columns = true is used too (s. function init_parameters).
   We get a consistent formulation with "FF set by hand" in input files. */
ایستا bool balance_columns = false;

/* (-l) Number of lines on a page, including header and footer lines. */
ایستا صحیح سطرها_per_page = 66;

/* Number of lines in the header and footer can be reset to 0 using
   the -t flag. */
شمارش { سطرها_per_header = 5 };
ایستا صحیح سطرها_per_body;
شمارش { سطرها_per_footer = 5 };

/* (-w|-W) Width in characters of the page.  Does not include the width of
   the margin. */
ایستا صحیح chars_per_سطر = 72;

/* (-w|W) True means we truncate lines longer than chars_per_column. */
ایستا bool truncate_سطرها = false;

/* (-J) True means we join lines without any line truncation. -J
   dominates -w option. */
ایستا bool join_سطرها = false;

/* Number of characters in a column.  Based on col_sep_length and
   page width. */
ایستا صحیح chars_per_column;

/* (-e) True means convert tabs to spaces on input. */
ایستا bool untabify_درون_داد = false;

/* (-e) The input tab character. */
ایستا نویسه درون_داد_tab_char = '\t';

/* (-e) Tabstops are at chars_per_tab, 2*chars_per_tab, 3*chars_per_tab, ...
   where the leftmost column is 1. */
ایستا صحیح chars_per_درون_داد_tab = 8;

/* (-i) True means convert spaces to tabs on output. */
ایستا bool tabify_output = false;

/* (-i) The output tab character. */
ایستا نویسه output_tab_char = '\t';

/* (-i) The width of the output tab. */
ایستا صحیح chars_per_output_tab = 8;

/* Keeps track of pending white space.  When we hit a nonspace
   character after some whitespace, we print whitespace, tabbing
   if necessary to get to output_position + spaces_not_printed. */
ایستا صحیح spaces_not_printed;

/* (-o) Number of spaces in the left margin (tabs used when possible). */
ایستا صحیح chars_per_margin = 0;

/* Position where the next character will fall.
   Leftmost position is 0 + chars_per_margin.
   Rightmost position is chars_per_margin + chars_per_line - 1.
   This is important for converting spaces to tabs on output. */
ایستا صحیح output_position;

/* Horizontal position relative to the current file.
   (output_position depends on where we are on the page;
   input_position depends on where we are in the file.)
   Important for converting tabs to spaces on input. */
ایستا صحیح درون_داد_position;

/* True if there were any failed opens so we can exit with nonzero
   status.  */
ایستا bool failed_opens = false;

/* The number of spaces taken up if we print a tab character with width
   c_ from position h_. */
#define TAB_WIDTH(c_, h_) ((c_) - ((h_) % (c_)))

/* The horizontal position we'll be at after printing a tab character
   of width c_ from the position h_. */
#define POS_AFTER_TAB(c_, h_) ((h_) + TAB_WIDTH (c_, h_))

/* (-NNN) Number of columns of text to print. */
ایستا صحیح columns = 1;

/* (+NNN:MMM) Page numbers on which to begin and stop printing.
   first_page_number = 0  will be used to check input only. */
ایستا uintmax_t first_page_عدد = 0;
ایستا uintmax_t last_page_عدد = UINTMAX_MAX;

/* Number of files open (not closed, not on hold). */
ایستا صحیح files_ready_to_read = 0;

/* Current page number.  Displayed in header. */
ایستا uintmax_t page_عدد;

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
ایستا صحیح سطر_عدد;

/* (-n) True means lines should be preceded by numbers. */
ایستا bool numbered_سطرها = false;

/* (-n) Character which follows each line number. */
ایستا نویسه عدد_separator = '\t';

/* (-n) line counting starts with 1st line of input file (not with 1st
   line of 1st page printed). */
ایستا صحیح سطر_شمار = 1;

/* (-n) True means counting of skipped lines starts with 1st line of
   input file. False means -N option is used in addition, counting of
   skipped lines not required. */
ایستا bool skip_شمار = true;

/* (-N) Counting starts with start_line_number = NUMBER at 1st line of
   first page printed, usually not 1st page of input file. */
ایستا صحیح start_سطر_رقم = 1;

/* (-n) Width in characters of a line number. */
ایستا صحیح chars_per_عدد = 5;

/* Used when widening the first column to accommodate numbers -- only
   needed when printing files in parallel.  Includes width of both the
   number and the number_separator. */
ایستا صحیح عدد_width;

/* Buffer sprintf uses to format a line number. */
ایستا نویسه *عدد_buff;

/* (-v) True means nonprintable characters are printed as escape sequences.
   control-g becomes \007. */
ایستا bool use_esc_sequence = false;

/* (-c) True means nonprintable characters are printed as control prefixes.
   control-g becomes ^G. */
ایستا bool use_cntrl_prefix = false;

/* (-d) True means output is double spaced. */
ایستا bool double_space = false;

/* Number of files opened initially in init_files.  Should be 1
   unless we're printing multiple files in parallel. */
ایستا صحیح total_files = 0;

/* (-r) True means don't complain if we can't open a file. */
ایستا bool ignore_failed_opens = false;

/* (-S) True means we separate columns with a specified string.
   -S option does not affect line truncation nor column alignment. */
ایستا bool use_col_separator = false;

/* String used to separate columns if the -S option has been specified.
   Default without -S but together with one of the column options
   -a|COLUMN|-m is a 'space' and with the -J option a 'tab'. */
ایستا نویسه ثابت *col_sep_string = "";
ایستا صحیح col_sep_length = 0;
ایستا نویسه *column_separator = (نویسه *) " ";
ایستا نویسه *سطر_separator = (نویسه *) "\t";

/* Number of separator characters waiting to be printed as soon as we
   know that we have any input remaining to be printed. */
ایستا صحیح separators_not_printed;

/* Position we need to pad to, as soon as we know that we have input
   remaining to be printed. */
ایستا صحیح padding_not_printed;

/* True means we should pad the end of the page.  Remains false until we
   know we have a page to print. */
ایستا bool pad_vertically;

/* (-h) String of characters used in place of the filename in the header. */
ایستا نویسه *custom_header;

/* (-D) Date format for the header.  */
ایستا نویسه ثابت *date_format;

/* The local time zone rules, as per the TZ environment variable.  */
ایستا timezone_t localtz;

/* Date and file name for the header.  */
ایستا نویسه *date_text;
ایستا نویسه ثابت *file_text;

/* Output columns available, not counting the date and file name.  */
ایستا صحیح header_width_available;

ایستا نویسه *clump_buff;

/* True means we read the line no. lines_per_body in skip_read
   called by skip_to_page. That variable controls the coincidence of a
   "FF set by hand" and "full_page_printed", see above the definition of
   structure COLUMN. */
ایستا bool last_سطر = false;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
شمارش
{
  COLUMNS_OPTION = CHAR_MAX + 1,
  PAGES_OPTION
};

ایستا نویسه ثابت short_options[] =
  "-0123456789D:FJN:S::TW:abcde::fh:i::l:mn::o:rs::tvw:";

ایستا ساختار option ثابت long_options[] =
{
  {"pages", required_argument, پوچ, PAGES_OPTION},
  {"columns", required_argument, پوچ, COLUMNS_OPTION},
  {"across", no_argument, پوچ, 'a'},
  {"show-control-chars", no_argument, پوچ, 'c'},
  {"double-space", no_argument, پوچ, 'd'},
  {"date-format", required_argument, پوچ, 'D'},
  {"expand-tabs", optional_argument, پوچ, 'e'},
  {"form-feed", no_argument, پوچ, 'f'},
  {"header", required_argument, پوچ, 'h'},
  {"output-tabs", optional_argument, پوچ, 'i'},
  {"join-lines", no_argument, پوچ, 'J'},
  {"length", required_argument, پوچ, 'l'},
  {"merge", no_argument, پوچ, 'm'},
  {"number-lines", optional_argument, پوچ, 'n'},
  {"first-line-number", required_argument, پوچ, 'N'},
  {"indent", required_argument, پوچ, 'o'},
  {"no-file-warnings", no_argument, پوچ, 'r'},
  {"separator", optional_argument, پوچ, 's'},
  {"sep-string", optional_argument, پوچ, 'S'},
  {"omit-header", no_argument, پوچ, 't'},
  {"omit-pagination", no_argument, پوچ, 'T'},
  {"show-nonprinting", no_argument, پوچ, 'v'},
  {"width", required_argument, پوچ, 'w'},
  {"page-width", required_argument, پوچ, 'W'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {پوچ, 0, پوچ, 0}
};

ایستا _Noreturn تهی
integer_overflow (تهی)
{
  error (EXIT_FAILURE, 0, _("integer overflow"));
}

/* Return the number of columns that have either an open file or
   stored lines. */

ATTRIBUTE_PURE
ایستا unsigned صحیح
cols_ready_to_print (تهی)
{
  COLUMN *q;
  unsigned صحیح i;
  unsigned صحیح n;

  n = 0;
  برای (q = column_vector, i = 0; i < columns; ++q, ++i)
    اگر (q->status == OPEN
        || q->status == FF_FOUND	/* With -b: To print a header only */
        || (storing_columns && q->سطرها_stored > 0 && q->سطرها_to_print > 0))
      ++n;
  بازگشت n;
}

/* Estimate first_ / last_page_number
   using option +FIRST_PAGE:LAST_PAGE */

ایستا bool
first_last_page (صحیح oi, نویسه c, نویسه ثابت *pages)
{
  نویسه *p;
  uintmax_t first;
  uintmax_t last = UINTMAX_MAX;
  strtol_error err = xstrtoumax (pages, &p, 10, &first, "");
  اگر (err != LONGINT_OK && err != LONGINT_INVALID_SUFFIX_CHAR)
    xstrtol_fatal (err, oi, c, long_options, pages);

  اگر (p == pages || !first)
    بازگشت false;

  اگر (*p == ':')
    {
      نویسه ثابت *p1 = p + 1;
      err = xstrtoumax (p1, &p, 10, &last, "");
      اگر (err != LONGINT_OK)
        xstrtol_fatal (err, oi, c, long_options, pages);
      اگر (p1 == p || last < first)
        بازگشت false;
    }

  اگر (*p)
    بازگشت false;

  first_page_عدد = first;
  last_page_عدد = last;
  بازگشت true;
}

/* Parse column count string S, and if it's valid (1 or larger and
   within range of the type of 'columns') set the global variables
   columns and explicit_columns.  Otherwise, exit with a diagnostic.  */

ایستا تهی
parse_column_شمار (نویسه ثابت *s)
{
  columns = getoptnum (s, 1, _("invalid number of columns"));
  explicit_columns = true;
}

/* Estimate length of col_sep_string with option -S.  */

ایستا تهی
separator_string (نویسه ثابت *optarg_S)
{
  size_t len = strlen (optarg_S);
  اگر (INT_MAX < len)
    integer_overflow ();
  col_sep_length = len;
  col_sep_string = optarg_S;
}

صحیح
اصلی (صحیح argc, نویسه **argv)
{
  unsigned صحیح n_files;
  bool old_options = false;
  bool old_w = false;
  bool old_s = false;
  نویسه **file_names;

  /* Accumulate the digits of old-style options like -99.  */
  نویسه *column_شمار_string = پوچ;
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
                ? xnmalloc (argc - 1, اندازه (نویسه *))
                : پوچ);

  وقتی (true)
    {
      صحیح oi = -1;
      صحیح c = getopt_long (argc, argv, short_options, long_options, &oi);
      اگر (c == -1)
        بشکن;

      اگر (c_isdigit (c))
        {
          /* Accumulate column-count digits specified via old-style options. */
          اگر (n_digits + 1 >= n_alloc)
            column_شمار_string = xpalloc (column_شمار_string, &n_alloc, 2, -1,
                                           اندازه *column_شمار_string);
          column_شمار_string[n_digits++] = c;
          column_شمار_string[n_digits] = '\0';
          ادامه;
        }

      n_digits = 0;

      انتخاب (c)
        {
        حالت 1:			/* Non-option argument. */
          /* long option --page dominates old '+FIRST_PAGE ...'.  */
          اگر (! (first_page_عدد == 0
                 && *optarg == '+' && first_last_page (-2, '+', optarg + 1)))
            file_names[n_files++] = optarg;
          بشکن;

        حالت PAGES_OPTION:	/* --pages=FIRST_PAGE[:LAST_PAGE] */
          {			/* dominates old opt +... */
            اگر (! optarg)
              error (EXIT_FAILURE, 0,
                     _("'--pages=FIRST_PAGE[:LAST_PAGE]' missing argument"));
            وگرنه اگر (! first_last_page (oi, 0, optarg))
              error (EXIT_FAILURE, 0, _("invalid page range %s"),
                     quote (optarg));
            بشکن;
          }

        حالت COLUMNS_OPTION:	/* --columns=COLS */
          {
            parse_column_شمار (optarg);

            /* If there was a prior column count specified via the
               short-named option syntax, e.g., -9, ensure that this
               long-name-specified value overrides it.  */
            آزاد (column_شمار_string);
            column_شمار_string = پوچ;
            n_alloc = 0;
            بشکن;
          }

        حالت 'a':
          print_across_flag = true;
          storing_columns = false;
          بشکن;
        حالت 'b':
          balance_columns = true;
          بشکن;
        حالت 'c':
          use_cntrl_prefix = true;
          بشکن;
        حالت 'd':
          double_space = true;
          بشکن;
        حالت 'D':
          date_format = optarg;
          بشکن;
        حالت 'e':
          اگر (optarg)
            getoptarg (optarg, 'e', &درون_داد_tab_char,
                       &chars_per_درون_داد_tab);
          /* Could check tab width > 0. */
          untabify_درون_داد = true;
          بشکن;
        حالت 'f':
        حالت 'F':
          use_form_feed = true;
          بشکن;
        حالت 'h':
          custom_header = optarg;
          بشکن;
        حالت 'i':
          اگر (optarg)
            getoptarg (optarg, 'i', &output_tab_char,
                       &chars_per_output_tab);
          /* Could check tab width > 0. */
          tabify_output = true;
          بشکن;
        حالت 'J':
          join_سطرها = true;
          بشکن;
        حالت 'l':
          سطرها_per_page
            = getoptnum (optarg, 1,
                         _("'-l PAGE_LENGTH' invalid number of lines"));
          بشکن;
        حالت 'm':
          parallel_files = true;
          storing_columns = false;
          بشکن;
        حالت 'n':
          numbered_سطرها = true;
          اگر (optarg)
            getoptarg (optarg, 'n', &عدد_separator,
                       &chars_per_عدد);
          بشکن;
        حالت 'N':
          skip_شمار = false;
          start_سطر_رقم
            = getoptnum (optarg, INT_MIN,
                         _("'-N NUMBER' invalid starting line number"));
          بشکن;
        حالت 'o':
          chars_per_margin = getoptnum (optarg, 0,
                                        _("'-o MARGIN' invalid line offset"));
          بشکن;
        حالت 'r':
          ignore_failed_opens = true;
          بشکن;
        حالت 's':
          old_options = true;
          old_s = true;
          اگر (!use_col_separator && optarg)
            separator_string (optarg);
          بشکن;
        حالت 'S':
          old_s = false;
          /* Reset an additional input of -s, -S dominates -s */
          col_sep_string = "";
          col_sep_length = 0;
          use_col_separator = true;
          اگر (optarg)
            separator_string (optarg);
          بشکن;
        حالت 't':
          extremities = false;
          keep_FF = true;
          بشکن;
        حالت 'T':
          extremities = false;
          keep_FF = false;
          بشکن;
        حالت 'v':
          use_esc_sequence = true;
          بشکن;
        حالت 'w':
          old_options = true;
          old_w = true;
          {
            صحیح tmp_cpl
              = getoptnum (optarg, 1,
                           _("'-w PAGE_WIDTH' invalid number of characters"));
            اگر (! truncate_سطرها)
              chars_per_سطر = tmp_cpl;
          }
          بشکن;
        حالت 'W':
          old_w = false;			/* dominates -w */
          truncate_سطرها = true;
          chars_per_سطر
            = getoptnum (optarg, 1,
                         _("'-W PAGE_WIDTH' invalid number of characters"));
          بشکن;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        پیشفرض:
          usage (EXIT_FAILURE);
          بشکن;
        }
    }

  اگر (column_شمار_string)
    {
      parse_column_شمار (column_شمار_string);
      آزاد (column_شمار_string);
    }

  اگر (! date_format)
    date_format = (getenv ("POSIXLY_CORRECT") && !hard_locale (LC_TIME)
                   ? "%b %e %H:%M %Y"
                   : "%Y-%m-%d %H:%M");

  localtz = tzalloc (getenv ("TZ"));

  /* Now we can set a reasonable initial value: */
  اگر (first_page_عدد == 0)
    first_page_عدد = 1;

  اگر (parallel_files && explicit_columns)
    error (EXIT_FAILURE, 0,
           _("cannot specify number of columns when printing in parallel"));

  اگر (parallel_files && print_across_flag)
    error (EXIT_FAILURE, 0,
           _("cannot specify both printing across and printing in parallel"));

/* Translate some old short options to new/long options.
   To meet downward compatibility with other UNIX pr utilities
   and some POSIX specifications. */

  اگر (old_options)
    {
      اگر (old_w)
        {
          اگر (parallel_files || explicit_columns)
            {
              /* activate -W */
              truncate_سطرها = true;
              اگر (old_s)
                /* adapt HP-UX and SunOS: -s = no separator;
                   activate -S */
                use_col_separator = true;
            }
          وگرنه
            /* old -w sets width with columns only
               activate -J */
            join_سطرها = true;
        }
      وگرنه اگر (!use_col_separator)
        {
          /* No -S option read */
          اگر (old_s && (parallel_files || explicit_columns))
            {
              اگر (!truncate_سطرها)
                {
                  /* old -s (without -w and -W) annuls column alignment,
                  uses fields, activate -J */
                  join_سطرها = true;
                  اگر (col_sep_length > 0)
                    /* activate -S */
                    use_col_separator = true;
                }
              وگرنه
                /* with -W */
                /* adapt HP-UX and SunOS: -s = no separator;
                   activate -S */
                use_col_separator = true;
            }
        }
    }

  برای (; optind < argc; optind++)
    {
      file_names[n_files++] = argv[optind];
    }

  اگر (n_files == 0)
    {
      /* No file arguments specified;  read from standard input.  */
      print_files (0, پوچ);
    }
  وگرنه
    {
      اگر (parallel_files)
        print_files (n_files, file_names);
      وگرنه
        {
          برای (unsigned صحیح i = 0; i < n_files; i++)
            print_files (1, &file_names[i]);
        }
    }

  cleanup ();

  اگر (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, _("standard input"));
  main_exit (failed_opens ? EXIT_FAILURE : EXIT_SUCCESS);
}

/* Parse numeric arguments, ensuring MIN <= number <= INT_MAX.  */

ایستا صحیح
getoptnum (نویسه ثابت *n_str, صحیح min, نویسه ثابت *err)
{
  بازگشت xnumtoimax (n_str, 10, min, INT_MAX, "", err, 0,
                     min <= 0 ? 0 : XTOINT_MIN_RANGE);
}

/* Parse options of the form -scNNN.

   Example: -nck, where 'n' is the option, c is the optional number
   separator, and k is the optional width of the field used when printing
   a number. */

ایستا تهی
getoptarg (نویسه *arg, نویسه switch_char, نویسه *character, صحیح *عدد)
{
  نویسه ثابت *رقم = arg;

  اگر (!*رقم)
    {
      error (0, 0, _("'-%c': Invalid argument: %s"), switch_char, quote (رقم));
      usage (EXIT_FAILURE);
    }

  اگر (!c_isdigit (*رقم))
    *character = *رقم++;
  اگر (*رقم)
    {
      long صحیح tmp_long;
      strtol_error e = xstrtol (رقم, پوچ, 10, &tmp_long, "");
      اگر (e == LONGINT_OK)
        {
          اگر (tmp_long <= 0)
            e = LONGINT_INVALID;
          وگرنه اگر (INT_MAX < tmp_long)
            e = LONGINT_OVERFLOW;
        }
      اگر (e != LONGINT_OK)
        {
          error (0, e & LONGINT_OVERFLOW ? EOVERFLOW : 0,
             _("'-%c' extra characters or invalid number in the argument: %s"),
                 switch_char, quote (arg));
          usage (EXIT_FAILURE);
        }
      *عدد = tmp_long;
    }
}

/* Set parameters related to formatting. */

ایستا تهی
init_parameters (صحیح عدد_of_files)
{
  صحیح chars_used_by_عدد = 0;

  سطرها_per_body = سطرها_per_page - سطرها_per_header - سطرها_per_footer;
  اگر (سطرها_per_body <= 0)
    {
      extremities = false;
      keep_FF = true;
    }
  اگر (! extremities)
    سطرها_per_body = سطرها_per_page;

  اگر (double_space)
    سطرها_per_body = MAX (1, سطرها_per_body / 2);

  /* If input is stdin, cannot print parallel files.  BSD dumps core
     on this. */
  اگر (عدد_of_files == 0)
    parallel_files = false;

  اگر (parallel_files)
    columns = عدد_of_files;

  /* One file, multi columns down: -b option is set to get a consistent
     formulation with "FF set by hand" in input files. */
  اگر (storing_columns)
    balance_columns = true;

  /* Tabification is assumed for multiple columns. */
  اگر (columns > 1)
    {
      اگر (!use_col_separator)
        {
          /* Use default separator */
          اگر (join_سطرها)
            col_sep_string = سطر_separator;
          وگرنه
            col_sep_string = column_separator;

          col_sep_length = 1;
          use_col_separator = true;
        }
      /* It's rather pointless to define a TAB separator with column
         alignment */
      وگرنه اگر (!join_سطرها && col_sep_length == 1 && *col_sep_string == '\t')
        col_sep_string = column_separator;

      truncate_سطرها = true;
      اگر (! (col_sep_length == 1 && *col_sep_string == '\t'))
        untabify_درون_داد = true;
      tabify_output = true;
    }
  وگرنه
    storing_columns = false;

  /* -J dominates -w in any case */
  اگر (join_سطرها)
    truncate_سطرها = false;

  اگر (numbered_سطرها)
    {
      صحیح chars_per_default_tab = 8;

      سطر_شمار = start_سطر_رقم;

      /* To allow input tab-expansion (-e sensitive) use:
         if (number_separator == input_tab_char)
           number_width = chars_per_number
             + TAB_WIDTH (chars_per_input_tab, chars_per_number);   */

      /* Estimate chars_per_text without any margin and keep it constant. */
      اگر (عدد_separator == '\t')
        عدد_width = (chars_per_عدد
                        + TAB_WIDTH (chars_per_default_tab, chars_per_عدد));
      وگرنه
        عدد_width = chars_per_عدد + 1;

      /* The number is part of the column width unless we are
         printing files in parallel. */
      اگر (parallel_files)
        chars_used_by_عدد = عدد_width;
    }

  صحیح sep_chars, useful_chars;
  اگر (ckd_mul (&sep_chars, columns - 1, col_sep_length))
    sep_chars = INT_MAX;
  اگر (ckd_sub (&useful_chars, chars_per_سطر - chars_used_by_عدد,
               sep_chars))
    useful_chars = 0;
  chars_per_column = useful_chars / columns;

  اگر (chars_per_column < 1)
    error (EXIT_FAILURE, 0, _("page width too narrow"));

  اگر (numbered_سطرها)
    {
      آزاد (عدد_buff);
      عدد_buff = xmalloc (MAX (chars_per_عدد,
                                  INT_STRLEN_BOUND (سطر_عدد)) + 1);
    }

  /* Pick the maximum between the tab width and the width of an
     escape sequence.
     The width of an escape sequence (4) isn't the lower limit any longer.
     We've to use 8 as the lower limit, if we use chars_per_default_tab = 8
     to expand a tab which is not an input_tab-char. */
  آزاد (clump_buff);
  clump_buff = xmalloc (MAX (8, chars_per_درون_داد_tab));
}

/* Open the necessary files,
   maintaining a COLUMN structure for each column.

   With multiple files, each column p has a different p->fp.
   With single files, each column p has the same p->fp.
   Return false if (number_of_files > 0) and no files can be opened,
   true otherwise.

   With each column/file p, p->full_page_printed is initialized,
   see also open_file.  */

ایستا bool
init_fps (صحیح عدد_of_files, نویسه **av)
{
  COLUMN *p;

  total_files = 0;

  آزاد (column_vector);
  column_vector = xnmalloc (columns, اندازه (COLUMN));

  اگر (parallel_files)
    {
      صحیح files_چپ = عدد_of_files;
      برای (p = column_vector; files_چپ--; ++p, ++av)
        {
          اگر (! open_file (*av, p))
            {
              --p;
              --columns;
            }
        }
      اگر (columns == 0)
        بازگشت false;
      init_header ("", -1);
    }
  وگرنه
    {
      p = column_vector;
      اگر (عدد_of_files > 0)
        {
          اگر (! open_file (*av, p))
            بازگشت false;
          init_header (*av, fileno (p->fp));
          p->سطرها_stored = 0;
        }
      وگرنه
        {
          p->name = _("standard input");
          p->fp = stdin;
          have_read_stdin = true;
          p->status = OPEN;
          p->full_page_printed = false;
          ++total_files;
          init_header ("", -1);
          p->سطرها_stored = 0;
        }

      نویسه ثابت *firstname = p->name;
      FILE *firstfp = p->fp;
      صحیح i;
      برای (i = columns - 1, ++p; i; --i, ++p)
        {
          p->name = firstname;
          p->fp = firstfp;
          p->status = OPEN;
          p->full_page_printed = false;
          p->سطرها_stored = 0;
        }
    }
  files_ready_to_read = total_files;
  بازگشت true;
}

/* Determine print_func and char_func, the functions
   used by each column for printing and/or storing.

   Determine the horizontal position desired when we begin
   printing a column (p->start_position). */

ایستا تهی
init_funcs (تهی)
{
  صحیح i, h, h_next;
  COLUMN *p;

  h = chars_per_margin;

  اگر (!truncate_سطرها)
    h_next = ANYWHERE;
  وگرنه
    {
      /* When numbering lines of parallel files, we enlarge the
         first column to accommodate the number.  Looks better than
         the Sys V approach. */
      اگر (parallel_files && numbered_سطرها)
        h_next = h + chars_per_column + عدد_width;
      وگرنه
        h_next = h + chars_per_column;
    }

  /* Enlarge p->start_position of first column to use the same form of
     padding_not_printed with all columns. */
  h = h + col_sep_length;

  /* This loop takes care of all but the rightmost column. */

  برای (p = column_vector, i = 1; i < columns; ++p, ++i)
    {
      اگر (storing_columns)	/* One file, multi columns down. */
        {
          p->char_func = store_char;
          p->print_func = print_stored;
        }
      وگرنه
        /* One file, multi columns across; or parallel files.  */
        {
          p->char_func = print_char;
          p->print_func = read_سطر;
        }

      /* Number only the first column when printing files in
         parallel. */
      p->numbered = numbered_سطرها && (!parallel_files || i == 1);
      p->start_position = h;

      /* If we don't truncate lines, all start_positions are
         ANYWHERE, except the first column's start_position when
         using a margin. */

      اگر (!truncate_سطرها)
        {
          h = ANYWHERE;
          h_next = ANYWHERE;
        }
      وگرنه
        {
          h = h_next + col_sep_length;
          h_next = h + chars_per_column;
        }
    }

  /* The rightmost column.

     Doesn't need to be stored unless we intend to balance
     columns on the last page. */
  اگر (storing_columns && balance_columns)
    {
      p->char_func = store_char;
      p->print_func = print_stored;
    }
  وگرنه
    {
      p->char_func = print_char;
      p->print_func = read_سطر;
    }

  p->numbered = numbered_سطرها && (!parallel_files || i == 1);
  p->start_position = h;
}

/* Open a file.  Return true if successful.

   With each file p, p->full_page_printed is initialized,
   see also init_fps. */

ایستا bool
open_file (نویسه *name, COLUMN *p)
{
  اگر (streq (name, "-"))
    {
      p->name = _("standard input");
      p->fp = stdin;
      have_read_stdin = true;
    }
  وگرنه
    {
      p->name = name;
      p->fp = fopen (name, "r");
    }
  اگر (p->fp == پوچ)
    {
      failed_opens = true;
      اگر (!ignore_failed_opens)
        error (0, errno, "%s", quotef (name));
      بازگشت false;
    }
  fadvise (p->fp, FADVISE_SEQUENTIAL);
  p->status = OPEN;
  p->full_page_printed = false;
  ++total_files;
  بازگشت true;
}

/* Close the file in P.

   If we aren't dealing with multiple files in parallel, we change
   the status of all columns in the column list to reflect the close. */

ایستا تهی
close_file (COLUMN *p)
{
  COLUMN *q;
  صحیح i;

  اگر (p->status == CLOSED)
    بازگشت;

  صحیح err = errno;
  اگر (!ferror (p->fp))
    err = 0;
  اگر (fileno (p->fp) == STDIN_FILENO)
    clearerr (p->fp);
  وگرنه اگر (fclose (p->fp) != 0 && !err)
    err = errno;
  اگر (err)
    error (EXIT_FAILURE, err, "%s", quotef (p->name));

  اگر (!parallel_files)
    {
      برای (q = column_vector, i = columns; i; ++q, --i)
        {
          q->status = CLOSED;
          اگر (q->سطرها_stored == 0)
            {
              q->سطرها_to_print = 0;
            }
        }
    }
  وگرنه
    {
      p->status = CLOSED;
      p->سطرها_to_print = 0;
    }

  --files_ready_to_read;
}

/* Put a file on hold until we start a new page,
   since we've hit a form feed.

   If we aren't dealing with parallel files, we must change the
   status of all columns in the column list. */

ایستا تهی
hold_file (COLUMN *p)
{
  COLUMN *q;
  صحیح i;

  اگر (!parallel_files)
    برای (q = column_vector, i = columns; i; ++q, --i)
      {
        اگر (storing_columns)
          q->status = FF_FOUND;
        وگرنه
          q->status = ON_HOLD;
      }
  وگرنه
    p->status = ON_HOLD;

  p->سطرها_to_print = 0;
  --files_ready_to_read;
}

/* Undo hold_file -- go through the column list and change any
   ON_HOLD columns to OPEN.  Used at the end of each page. */

ایستا تهی
reset_status (تهی)
{
  صحیح i = columns;
  COLUMN *p;

  برای (p = column_vector; i; --i, ++p)
    اگر (p->status == ON_HOLD)
      {
        p->status = OPEN;
        files_ready_to_read++;
      }

  اگر (storing_columns)
    {
      اگر (column_vector->status == CLOSED)
        /* We use the info to output an error message in  skip_to_page. */
        files_ready_to_read = 0;
      وگرنه
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

ایستا تهی
print_files (صحیح عدد_of_files, نویسه **av)
{
  init_parameters (عدد_of_files);
  اگر (! init_fps (عدد_of_files, av))
    بازگشت;
  اگر (storing_columns)
    init_store_cols ();

  اگر (first_page_عدد > 1)
    {
      اگر (!skip_to_page (first_page_عدد))
        بازگشت;
      وگرنه
        page_عدد = first_page_عدد;
    }
  وگرنه
    page_عدد = 1;

  init_funcs ();

  سطر_عدد = سطر_شمار;
  وقتی (print_page ())
    ;
}

/* Initialize header information.
   If DESC is non-negative, it is a file descriptor open to
   FILENAME for reading.  */

ایستا تهی
init_header (نویسه ثابت *filename, صحیح desc)
{
  نویسه *buf = پوچ;
  ساختار stat st;
  ساختار timespec t;
  صحیح ns;
  ساختار tm tm;

  /* If parallel files or standard input, use current date. */
  اگر (streq (filename, "-"))
    desc = -1;
  اگر (0 <= desc && fstat (desc, &st) == 0)
    t = get_stat_mtime (&st);
  وگرنه
    {
      ایستا ساختار timespec timespec;
      اگر (! timespec.tv_sec)
        gettime (&timespec);
      t = timespec;
    }

  ns = t.tv_nsec;
  اگر (localtime_rz (localtz, &t.tv_sec, &tm))
    {
      ptrdiff_t len = nstrftime (پوچ, MIN (PTRDIFF_MAX, SIZE_MAX),
                                 date_format, &tm, localtz, ns);
      اگر (0 <= len)
        {
          buf = ximalloc (len + 1);
          nstrftime (buf, len + 1, date_format, &tm, localtz, ns);
        }
    }
  اگر (!buf)
    {
      نویسه secbuf[INT_BUFSIZE_BOUND (intmax_t)];
      buf = xmalloc (اندازه secbuf + MAX (10, INT_BUFSIZE_BOUND (صحیح)));
      sprintf (buf, "%s.%09d", timetostr (t.tv_sec, secbuf), ns);
    }

  آزاد (date_text);
  date_text = buf;
  file_text = custom_header ? custom_header : desc < 0 ? "" : filename;
  header_width_available = (chars_per_سطر
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

ایستا تهی
init_page (تهی)
{
  صحیح j;
  COLUMN *p;

  اگر (storing_columns)
    {
      store_columns ();
      برای (j = columns - 1, p = column_vector; j; --j, ++p)
        {
          p->سطرها_to_print = p->سطرها_stored;
        }

      /* Last column. */
      اگر (balance_columns)
        {
          p->سطرها_to_print = p->سطرها_stored;
        }
      /* Since we're not balancing columns, we don't need to store
         the rightmost column.   Read it straight from the file. */
      وگرنه
        {
          اگر (p->status == OPEN)
            {
              p->سطرها_to_print = سطرها_per_body;
            }
          وگرنه
            p->سطرها_to_print = 0;
        }
    }
  وگرنه
    برای (j = columns, p = column_vector; j; --j, ++p)
      اگر (p->status == OPEN)
        {
          p->سطرها_to_print = سطرها_per_body;
        }
      وگرنه
        p->سطرها_to_print = 0;
}

/* Align empty columns and print separators.
   Empty columns will be formed by files with status ON_HOLD or CLOSED
   when printing multiple files in parallel. */

ایستا تهی
align_column (COLUMN *p)
{
  padding_not_printed = p->start_position;
  اگر (col_sep_length < padding_not_printed)
    {
      pad_across_to (padding_not_printed - col_sep_length);
      padding_not_printed = ANYWHERE;
    }

  اگر (use_col_separator)
    print_sep_string ();

  اگر (p->numbered)
    add_سطر_عدد (p);
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

ایستا bool
print_page (تهی)
{
  صحیح j;
  صحیح سطرها_چپ_on_page;
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

  اگر (cols_ready_to_print () == 0)
    بازگشت false;

  اگر (extremities)
    print_a_header = true;

  /* Don't pad unless we know a page was printed. */
  pad_vertically = false;
  pv = false;

  سطرها_چپ_on_page = سطرها_per_body;
  اگر (double_space)
    سطرها_چپ_on_page *= 2;

  وقتی (سطرها_چپ_on_page > 0 && cols_ready_to_print () > 0)
    {
      output_position = 0;
      spaces_not_printed = 0;
      separators_not_printed = 0;
      pad_vertically = false;
      align_empty_cols = false;
      empty_سطر = true;

      برای (j = 1, p = column_vector; j <= columns; ++j, ++p)
        {
          درون_داد_position = 0;
          اگر (p->سطرها_to_print > 0 || p->status == FF_FOUND)
            {
              FF_only = false;
              padding_not_printed = p->start_position;
              اگر (!(p->print_func) (p))
                read_rest_of_سطر (p);
              pv |= pad_vertically;

              --p->سطرها_to_print;
              اگر (p->سطرها_to_print <= 0)
                {
                  اگر (cols_ready_to_print () == 0)
                    بشکن;
                }

              /* File p changed its status to ON_HOLD or CLOSED */
              اگر (parallel_files && p->status != OPEN)
                {
                  اگر (empty_سطر)
                    align_empty_cols = true;
                  وگرنه اگر (p->status == CLOSED
                           || (p->status == ON_HOLD && FF_only))
                    align_column (p);
                }
            }
          وگرنه اگر (parallel_files)
            {
              /* File status ON_HOLD or CLOSED */
              اگر (empty_سطر)
                align_empty_cols = true;
              وگرنه
                align_column (p);
            }

          /* We need it also with an empty column */
          اگر (use_col_separator)
            ++separators_not_printed;
        }

      اگر (pad_vertically)
        {
          putchar ('\n');
          --سطرها_چپ_on_page;
        }

      اگر (cols_ready_to_print () == 0 && !extremities)
        بشکن;

      اگر (double_space && pv)
        {
          putchar ('\n');
          --سطرها_چپ_on_page;
        }
    }

  اگر (سطرها_چپ_on_page == 0)
    برای (j = 1, p = column_vector; j <= columns; ++j, ++p)
      اگر (p->status == OPEN)
        p->full_page_printed = true;

  pad_vertically = pv;

  اگر (pad_vertically && extremities)
    pad_down (سطرها_چپ_on_page + سطرها_per_footer);
  وگرنه اگر (keep_FF && print_a_FF)
    {
      putchar ('\f');
      print_a_FF = false;
    }

  اگر (ferror (stdout))
    write_error ();

  اگر (last_page_عدد < ++page_عدد)
    بازگشت false;		/* Stop printing with LAST_PAGE */

  reset_status ();		/* Change ON_HOLD to OPEN. */

  بازگشت true;			/* More pages to go. */
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

ایستا تهی
init_store_cols (تهی)
{
  /* Tune this.  */
  صحیح total_سطرها, total_سطرها_1, chars_per_column_1, chars_if_truncate;
  اگر (ckd_mul (&total_سطرها, سطرها_per_body, columns)
      || ckd_add (&total_سطرها_1, total_سطرها, 1)
      || ckd_add (&chars_per_column_1, chars_per_column, 1)
      || ckd_mul (&chars_if_truncate, total_سطرها, chars_per_column_1)
      || ckd_mul (&buff_allocated, chars_if_truncate, use_col_separator + 1))
    integer_overflow ();

  آزاد (سطر_vector);
  /* FIXME: here's where it was allocated.  */
  سطر_vector = xnmalloc (total_سطرها_1, اندازه *سطر_vector);

  آزاد (end_vector);
  end_vector = xnmalloc (total_سطرها, اندازه *end_vector);

  آزاد (buff);
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

ایستا تهی
store_columns (تهی)
{
  صحیح i, j;
  unsigned صحیح سطر = 0;
  unsigned صحیح buff_start;
  صحیح last_col;		/* The rightmost column which will be saved in buff */
  COLUMN *p;

  buff_current = 0;
  buff_start = 0;

  اگر (balance_columns)
    last_col = columns;
  وگرنه
    last_col = columns - 1;

  برای (i = 1, p = column_vector; i <= last_col; ++i, ++p)
    p->سطرها_stored = 0;

  برای (i = 1, p = column_vector; i <= last_col && files_ready_to_read;
       ++i, ++p)
    {
      p->current_سطر = سطر;
      برای (j = سطرها_per_body; j && files_ready_to_read; --j)

        اگر (p->status == OPEN)	/* Redundant.  Clean up. */
          {
            درون_داد_position = 0;

            اگر (!read_سطر (p))
              read_rest_of_سطر (p);

            اگر (p->status == OPEN
                || buff_start != buff_current)
              {
                ++p->سطرها_stored;
                سطر_vector[سطر] = buff_start;
                end_vector[سطر++] = درون_داد_position;
                buff_start = buff_current;
              }
          }
    }

  /* Keep track of the location of the last char in buff. */
  سطر_vector[سطر] = buff_start;

  اگر (balance_columns)
    balance (سطر);
}

ایستا تهی
balance (صحیح total_stored)
{
  COLUMN *p;
  صحیح i, سطرها;
  صحیح first_سطر = 0;

  برای (i = 1, p = column_vector; i <= columns; ++i, ++p)
    {
      سطرها = total_stored / columns;
      اگر (i <= total_stored % columns)
        ++سطرها;

      p->سطرها_stored = سطرها;
      p->current_سطر = first_سطر;

      first_سطر += سطرها;
    }
}

/* Store a character in the buffer. */

ایستا تهی
store_char (نویسه c)
{
  اگر (buff_current >= buff_allocated)
    {
      /* May be too generous. */
      buff = xpalloc (buff, &buff_allocated, 1, -1, اندازه *buff);
    }
  buff[buff_current++] = c;
}

ایستا تهی
add_سطر_عدد (COLUMN *p)
{
  صحیح i;
  نویسه *s;
  صحیح رقم_width;

  /* Cutting off the higher-order digits is more informative than
     lower-order cut off. */
  رقم_width = sprintf (عدد_buff, "%*d", chars_per_عدد, سطر_عدد);
  سطر_عدد++;
  s = عدد_buff + (رقم_width - chars_per_عدد);
  برای (i = chars_per_عدد; i > 0; i--)
    (p->char_func) (*s++);

  اگر (columns > 1)
    {
      /* Tabification is assumed for multiple columns, also for n-separators,
         but 'default n-separator = TAB' hasn't been given priority over
         equal column_width also specified by POSIX. */
      اگر (عدد_separator == '\t')
        {
          i = عدد_width - chars_per_عدد;
          وقتی (i-- > 0)
            (p->char_func) (' ');
        }
      وگرنه
        (p->char_func) (عدد_separator);
    }
  وگرنه
    /* To comply with POSIX, we avoid any expansion of default TAB
       separator with a single column output. No column_width requirement
       has to be considered. */
    {
      (p->char_func) (عدد_separator);
      اگر (عدد_separator == '\t')
        output_position = POS_AFTER_TAB (chars_per_output_tab,
                          output_position);
    }

  اگر (truncate_سطرها && !parallel_files)
    درون_داد_position += عدد_width;
}

/* Print (or store) padding until the current horizontal position
   is position. */

ایستا تهی
pad_across_to (صحیح position)
{
  صحیح h = output_position;

  اگر (tabify_output)
    spaces_not_printed = position - output_position;
  وگرنه
    {
      وقتی (++h <= position)
        putchar (' ');
      output_position = position;
    }
}

/* Pad to the bottom of the page.

   If the user has requested a formfeed, use one.
   Otherwise, use newlines. */

ایستا تهی
pad_down (unsigned صحیح سطرها)
{
  اگر (use_form_feed)
    putchar ('\f');
  وگرنه
    برای (unsigned صحیح i = سطرها; i; --i)
      putchar ('\n');
}

/* Read the rest of the line.

   Read from the current column's file until an end of line is
   hit.  Used when we've truncated a line and we no longer need
   to print or store its characters. */

ایستا تهی
read_rest_of_سطر (COLUMN *p)
{
  صحیح c;
  FILE *f = p->fp;

  وقتی ((c = getc (f)) != '\n')
    {
      اگر (c == '\f')
        {
          اگر ((c = getc (f)) != '\n')
            ungetc (c, f);
          اگر (keep_FF)
            print_a_FF = true;
          hold_file (p);
          بشکن;
        }
      وگرنه اگر (c == EOF)
        {
          close_file (p);
          بشکن;
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

ایستا تهی
skip_read (COLUMN *p, صحیح column_عدد)
{
  صحیح c;
  FILE *f = p->fp;
  صحیح i;
  bool single_ff = false;
  COLUMN *q;

  /* Read 1st character in a line or any character succeeding a FF */
  اگر ((c = getc (f)) == '\f' && p->full_page_printed)
    /* A FF-coincidence with a previous full_page_printed.
       To avoid an additional empty page, eliminate the FF */
    اگر ((c = getc (f)) == '\n')
      c = getc (f);

  p->full_page_printed = false;

  /* 1st character a FF means a single FF without any printable
     characters. Don't count it as a line with -n option. */
  اگر (c == '\f')
    single_ff = true;

  /* Preparing for a FF-coincidence: Maybe we finish that page
     without a FF found */
  اگر (last_سطر)
    p->full_page_printed = true;

  وقتی (c != '\n')
    {
      اگر (c == '\f')
        {
          /* No FF-coincidence possible,
             no catching up of a FF-coincidence with next page */
          اگر (last_سطر)
            {
              اگر (!parallel_files)
                برای (q = column_vector, i = columns; i; ++q, --i)
                  q->full_page_printed = false;
              وگرنه
                p->full_page_printed = false;
            }

          اگر ((c = getc (f)) != '\n')
            ungetc (c, f);
          hold_file (p);
          بشکن;
        }
      وگرنه اگر (c == EOF)
        {
          close_file (p);
          بشکن;
        }
      c = getc (f);
    }

  اگر (skip_شمار)
    اگر ((!parallel_files || column_عدد == 1) && !single_ff)
      ++سطر_شمار;
}

/* If we're tabifying output,

   When print_char encounters white space it keeps track
   of our desired horizontal position and delays printing
   until this function is called. */

ایستا تهی
print_white_space (تهی)
{
  صحیح h_new;
  صحیح h_old = output_position;
  صحیح goal = h_old + spaces_not_printed;

  وقتی (goal - h_old > 1
         && (h_new = POS_AFTER_TAB (chars_per_output_tab, h_old)) <= goal)
    {
      putchar (output_tab_char);
      h_old = h_new;
    }
  وقتی (++h_old <= goal)
    putchar (' ');

  output_position = goal;
  spaces_not_printed = 0;
}

/* Print column separators.

   We keep a count until we know that we'll be printing a line,
   then print_sep_string() is called. */

ایستا تهی
print_sep_string (تهی)
{
  نویسه ثابت *s = col_sep_string;
  صحیح l = col_sep_length;

  اگر (separators_not_printed <= 0)
    {
      /* We'll be starting a line with chars_per_margin, anything else? */
      اگر (spaces_not_printed > 0)
        print_white_space ();
    }
  وگرنه
    {
      برای (; separators_not_printed > 0; --separators_not_printed)
        {
          وقتی (l-- > 0)
            {
              /* 3 types of sep_strings: spaces only, spaces and chars,
              chars only */
              اگر (*s == ' ')
                {
                  /* We're tabifying output; consecutive spaces in
                  sep_string may have to be converted to tabs */
                  s++;
                  ++spaces_not_printed;
                }
              وگرنه
                {
                  اگر (spaces_not_printed > 0)
                    print_white_space ();
                  putchar (*s++);
                  ++output_position;
                }
            }
          /* sep_string ends with some spaces */
          اگر (spaces_not_printed > 0)
            print_white_space ();
        }
    }
}

/* Print (or store, depending on p->char_func) a clump of N
   characters. */

ایستا تهی
print_clump (COLUMN *p, صحیح n, نویسه *clump)
{
  وقتی (n--)
    (p->char_func) (*clump++);

  اگر (ferror (stdout))
    write_error ();
}

/* Print a character.

   Update the following comment: process-char hasn't been used any
   longer.
   If we're tabifying, all tabs have been converted to spaces by
   process_char().  Keep a count of consecutive spaces, and when
   a nonspace is encountered, call print_white_space() to print the
   required number of tabs and spaces. */

ایستا تهی
print_char (نویسه c)
{
  اگر (tabify_output)
    {
      اگر (c == ' ')
        {
          ++spaces_not_printed;
          بازگشت;
        }
      وگرنه اگر (spaces_not_printed > 0)
        print_white_space ();

      /* Nonprintables are assumed to have width 0, except '\b'. */
      اگر (! isprint (to_uchar (c)))
        {
          اگر (c == '\b')
            --output_position;
        }
      وگرنه
        ++output_position;
    }
  putchar (c);
}

/* Skip to page PAGE before printing.
   PAGE may be larger than total number of pages. */

ایستا bool
skip_to_page (uintmax_t page)
{
  برای (uintmax_t n = 1; n < page; ++n)
    {
      COLUMN *p;
      صحیح j;

      برای (صحیح i = 1; i < سطرها_per_body; ++i)
        {
          برای (j = 1, p = column_vector; j <= columns; ++j, ++p)
            اگر (p->status == OPEN)
              skip_read (p, j);
        }
      last_سطر = true;
      برای (j = 1, p = column_vector; j <= columns; ++j, ++p)
        اگر (p->status == OPEN)
          skip_read (p, j);

      اگر (storing_columns)	/* change FF_FOUND to ON_HOLD */
        برای (j = 1, p = column_vector; j <= columns; ++j, ++p)
          اگر (p->status != CLOSED)
            p->status = ON_HOLD;

      reset_status ();
      last_سطر = false;

      اگر (files_ready_to_read < 1)
        {
          /* It's very helpful, normally the total number of pages is
             not known in advance.  */
          error (0, 0,
                 _("starting page number %ju exceeds page count %ju"),
                 page, n);
          بشکن;
        }
    }
  بازگشت files_ready_to_read > 0;
}

/* Print a header.

   Formfeeds are assumed to use up two lines at the beginning of
   the page. */

ایستا تهی
print_header (تهی)
{
  نویسه page_text[256 + INT_STRLEN_BOUND (page_عدد)];
  صحیح available_width;
  صحیح lhs_spaces;
  صحیح rhs_spaces;

  output_position = 0;
  pad_across_to (chars_per_margin);
  print_white_space ();

  اگر (page_عدد == 0)
    error (EXIT_FAILURE, 0, _("page number overflow"));

  /* The translator must ensure that formatting the translation of
     "Page %ju" does not generate more than (sizeof page_text - 1)
     bytes.  */
  sprintf (page_text, _("Page %ju"), page_عدد);
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

ایستا bool
read_سطر (COLUMN *p)
{
  صحیح c;
  صحیح chars IF_LINT (= 0);
  صحیح last_درون_داد_position;
  صحیح j, k;
  COLUMN *q;

  /* read 1st character in each line or any character succeeding a FF: */
  c = getc (p->fp);

  last_درون_داد_position = درون_داد_position;

  اگر (c == '\f' && p->full_page_printed)
    اگر ((c = getc (p->fp)) == '\n')
      c = getc (p->fp);
  p->full_page_printed = false;

  انتخاب (c)
    {
    حالت '\f':
      اگر ((c = getc (p->fp)) != '\n')
        ungetc (c, p->fp);
      FF_only = true;
      اگر (print_a_header && !storing_columns)
        {
          pad_vertically = true;
          print_header ();
        }
      وگرنه اگر (keep_FF)
        print_a_FF = true;
      hold_file (p);
      بازگشت true;
    حالت EOF:
      close_file (p);
      بازگشت true;
    حالت '\n':
      بشکن;
    پیشفرض:
      chars = char_to_clump (c);
    }

  اگر (truncate_سطرها && درون_داد_position > chars_per_column)
    {
      درون_داد_position = last_درون_داد_position;
      بازگشت false;
    }

  اگر (p->char_func != store_char)
    {
      pad_vertically = true;

      اگر (print_a_header && !storing_columns)
        print_header ();

      اگر (parallel_files && align_empty_cols)
        {
          /* We have to align empty columns at the beginning of a line. */
          k = separators_not_printed;
          separators_not_printed = 0;
          برای (j = 1, q = column_vector; j <= k; ++j, ++q)
            {
              align_column (q);
              separators_not_printed += 1;
            }
          padding_not_printed = p->start_position;
          اگر (truncate_سطرها)
            spaces_not_printed = chars_per_column;
          وگرنه
            spaces_not_printed = 0;
          align_empty_cols = false;
        }

      اگر (col_sep_length < padding_not_printed)
        {
          pad_across_to (padding_not_printed - col_sep_length);
          padding_not_printed = ANYWHERE;
        }

      اگر (use_col_separator)
        print_sep_string ();
    }

  اگر (p->numbered)
    add_سطر_عدد (p);

  empty_سطر = false;
  اگر (c == '\n')
    بازگشت true;

  print_clump (p, chars, clump_buff);

  وقتی (true)
    {
      c = getc (p->fp);

      انتخاب (c)
        {
        حالت '\n':
          بازگشت true;
        حالت '\f':
          اگر ((c = getc (p->fp)) != '\n')
            ungetc (c, p->fp);
          اگر (keep_FF)
            print_a_FF = true;
          hold_file (p);
          بازگشت true;
        حالت EOF:
          close_file (p);
          بازگشت true;
        }

      last_درون_داد_position = درون_داد_position;
      chars = char_to_clump (c);
      اگر (truncate_سطرها && درون_داد_position > chars_per_column)
        {
          درون_داد_position = last_درون_داد_position;
          بازگشت false;
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

ایستا bool
print_stored (COLUMN *p)
{
  COLUMN *q;

  صحیح سطر = p->current_سطر++;
  نویسه *first = &buff[سطر_vector[سطر]];
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
  نویسه *last = &buff[سطر_vector[سطر + 1]];

  pad_vertically = true;

  اگر (print_a_header)
    print_header ();

  اگر (p->status == FF_FOUND)
    {
      صحیح i;
      برای (i = 1, q = column_vector; i <= columns; ++i, ++q)
        q->status = ON_HOLD;
      اگر (column_vector->سطرها_to_print <= 0)
        {
          اگر (!extremities)
            pad_vertically = false;
          بازگشت true;		/* print a header only */
        }
    }

  اگر (col_sep_length < padding_not_printed)
    {
      pad_across_to (padding_not_printed - col_sep_length);
      padding_not_printed = ANYWHERE;
    }

  اگر (use_col_separator)
    print_sep_string ();

  وقتی (first != last)
    print_char (*first++);

  اگر (spaces_not_printed == 0)
    {
      output_position = p->start_position + end_vector[سطر];
      اگر (p->start_position - col_sep_length == chars_per_margin)
        output_position -= col_sep_length;
    }

  بازگشت true;
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

ایستا صحیح
char_to_clump (نویسه c)
{
  unsigned نویسه uc = c;
  نویسه *s = clump_buff;
  نویسه esc_buff[4];
  صحیح width;
  صحیح chars;
  صحیح chars_per_c = 8;

  اگر (c == درون_داد_tab_char)
    chars_per_c = chars_per_درون_داد_tab;

  اگر (c == درون_داد_tab_char || c == '\t')
    {
      width = TAB_WIDTH (chars_per_c, درون_داد_position);

      اگر (untabify_درون_داد)
        {
          برای (صحیح i = width; i; --i)
            *s++ = ' ';
          chars = width;
        }
      وگرنه
        {
          *s = c;
          chars = 1;
        }

    }
  وگرنه اگر (! isprint (uc))
    {
      اگر (use_esc_sequence)
        {
          width = 4;
          chars = 4;
          *s++ = '\\';
          sprintf (esc_buff, "%03o", uc);
          برای (صحیح i = 0; i <= 2; ++i)
            *s++ = esc_buff[i];
        }
      وگرنه اگر (use_cntrl_prefix)
        {
          اگر (uc < 0200)
            {
              width = 2;
              chars = 2;
              *s++ = '^';
              *s = c ^ 0100;
            }
          وگرنه
            {
              width = 4;
              chars = 4;
              *s++ = '\\';
              sprintf (esc_buff, "%03o", uc);
              برای (صحیح i = 0; i <= 2; ++i)
                *s++ = esc_buff[i];
            }
        }
      وگرنه اگر (c == '\b')
        {
          width = -1;
          chars = 1;
          *s = c;
        }
      وگرنه
        {
          width = 0;
          chars = 1;
          *s = c;
        }
    }
  وگرنه
    {
      width = 1;
      chars = 1;
      *s = c;
    }

  /* Too many backspaces must put us in position 0 -- never negative.  */
  اگر (width < 0 && درون_داد_position == 0)
    {
      chars = 0;
      درون_داد_position = 0;
    }
  وگرنه اگر (width < 0 && درون_داد_position <= -width)
    درون_داد_position = 0;
  وگرنه
    {
      اگر (ckd_add (&درون_داد_position, درون_داد_position, width))
        integer_overflow ();
    }

  بازگشت chars;
}

/* We've just printed some files and need to clean up things before
   looking for more options and printing the next batch of files.

   Free everything we've xmalloc'ed, except 'header'. */

ایستا تهی
cleanup (تهی)
{
  آزاد (عدد_buff);
  آزاد (clump_buff);
  آزاد (column_vector);
  آزاد (سطر_vector);
  آزاد (end_vector);
  آزاد (buff);
}

/* Complain, print a usage message, and die. */

تهی
usage (صحیح status)
{
  اگر (status != EXIT_SUCCESS)
    emit_try_help ();
  وگرنه
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              program_name);

      fputs (_("\
Paginate or columnate FILE(s) برای printing.\n\
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
         Balance عدد of سطرها in the columns on each page\n\
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
  -d, --دوگانه-space\n\
         دوگانه space the output\n\
"));
      oputs (_("\
  -D, --date-format=FORMAT\n\
         use FORMAT برای the header date\n\
"));
      oputs (_("\
  -e[CHAR[WIDTH]], --expand-tabs[=CHAR[WIDTH]]\n\
         expand درون_داد CHARs (TABs) to tab WIDTH (8)\n\
"));
      oputs (_("\
  -F, -f, --form-feed\n\
         use form feeds instead of newlines to separate pages\n\
         (by a 3-سطر page header with -F or a 5-سطر header\n\
         and trailer without -F)\n\
"));
      oputs (_("\
  -h, --header=HEADER\n\
         use a centered HEADER instead of filename in page header,\n\
         -h \"\" prints a blank سطر, don't use -h\"\"\n\
"));
      oputs (_("\
  -i[CHAR[WIDTH]], --output-tabs[=CHAR[WIDTH]]\n\
         replace spaces with CHARs (TABs) to tab WIDTH (8)\n\
"));
      oputs (_("\
  -J, --join-سطرها\n\
         merge full سطرها, turns off -W سطر truncation,\n\
         no column alignment, --sep-string[=STRING] sets separators\n\
"));
      oputs (_("\
  -l, --length=PAGE_LENGTH\n\
         set the page length to PAGE_LENGTH (66) سطرها\n\
         (پیشفرض عدد of سطرها of text 56, and with -F 63).\n\
         implies -t اگر PAGE_LENGTH <= 10\n\
"));
      oputs (_("\
  -m, --merge\n\
         print all files in parallel, one in each column,\n\
         truncate سطرها, but join سطرها of full length with -J\n\
"));
      oputs (_("\
  -n[SEP[DIGITS]], --عدد-سطرها[=SEP[DIGITS]]\n\
         عدد سطرها, use DIGITS (5) digits, then SEP (TAB),\n\
         پیشفرض counting starts with 1st سطر of درون_داد file\n\
"));
      oputs (_("\
  -N, --first-سطر-عدد=عدد_2\n\
         start counting with عدد_2 at 1st سطر of first\n\
         page printed (see +FIRST_PAGE)\n\
"));
      oputs (_("\
  -o, --indent=MARGIN\n\
         offset each سطر with MARGIN (zero) spaces, انجامبده not\n\
         affect -w or -W, MARGIN will be added to PAGE_WIDTH\n\
"));
      oputs (_("\
  -r, --no-file-warnings\n\
         omit warning when a file cannot be opened\n\
"));
      oputs (_("\
  -s[CHAR], --separator[=CHAR]\n\
         separate columns by a single character, پیشفرض برای CHAR\n\
         is the <TAB> character without -w and \'no نویسه\' with -w.\n\
         -s[CHAR] turns off سطر truncation of all 3 column\n\
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
         omit page headers and trailers; implied اگر PAGE_LENGTH <= 10\n\
"));
      oputs (_("\
  -T, --omit-pagination\n\
         omit page headers and trailers,\n\
         eliminate any pagination by form feeds set in درون_داد files\n\
"));
      oputs (_("\
  -v, --show-nonprinting\n\
         use octal backslash notation\n\
"));
      oputs (_("\
  -w, --width=PAGE_WIDTH\n\
         set page width to PAGE_WIDTH (72) characters برای\n\
         multiple text-column output only, -s[نویسه] turns off (72)\n\
"));
      oputs (_("\
  -W, --page-width=PAGE_WIDTH\n\
         set page width to PAGE_WIDTH (72) characters always,\n\
         truncate سطرها, except -J option is set,\n\
         no interference with -S or -s\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}
