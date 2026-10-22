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

#অন্তর্ভাৱ <config.h>

#অন্তর্ভাৱ <getopt.h>
#অন্তর্ভাৱ <sys/types.h>
#অন্তর্ভাৱ "system.h"
#অন্তর্ভাৱ "c-ctype.h"
#অন্তর্ভাৱ "fadvise.h"
#অন্তর্ভাৱ "hard-locale.h"
#অন্তর্ভাৱ "mbswidth.h"
#অন্তর্ভাৱ "quote.h"
#অন্তর্ভাৱ "stat-time.h"
#অন্তর্ভাৱ "stdio--.h"
#অন্তর্ভাৱ "strftime.h"
#অন্তর্ভাৱ "xstrtol.h"
#অন্তর্ভাৱ "xstrtol-error.h"
#অন্তর্ভাৱ "xdectoint.h"

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

সংরচনা COLUMN;
সংরচনা COLUMN
  {
    FILE *fp;			/* Input stream for this column. */
    ৱর্ণ ধ্রুৱ *name;		/* File name. */
    গণনা
      {
        OPEN,
        FF_FOUND,		/* used with -b option, set with \f, changed
                                   to ON_HOLD after print_header */
        ON_HOLD,		/* Hit a form feed. */
        CLOSED
      }
    status;			/* Status of the file pointer. */

    /* Func to print lines in this col. */
    bool (*print_func) (সংরচনা COLUMN *);

    /* Func to print/store chars in this col. */
    শূন্য (*char_func) (ৱর্ণ);

    পূর্ণাঙ্ক current_সাৰি;		/* Index of current place in line_vector. */
    পূর্ণাঙ্ক সাৰিগুলি_stored;		/* Number of lines stored in buff. */
    পূর্ণাঙ্ক সাৰিগুলি_to_print;		/* No. lines stored or space left on page. */
    পূর্ণাঙ্ক start_position;		/* Horizontal position of first char. */
    bool numbered;
    bool full_page_printed;	/* True means printed without a FF found. */

    /* p->full_page_printed  controls a special case of "FF set by hand":
       True means a full page has been printed without FF found. To avoid an
       additional empty page we have to ignore a FF immediately following in
       the next line. */
  };

প্রকারনির্দেশ সংরচনা COLUMN COLUMN;

স্থির পূর্ণাঙ্ক char_to_clump (ৱর্ণ c);
স্থির bool read_সাৰি (COLUMN *p);
স্থির bool print_page (শূন্য);
স্থির bool print_stored (COLUMN *p);
স্থির bool open_file (ৱর্ণ *name, COLUMN *p);
স্থির bool skip_to_page (uintmax_t page);
স্থির শূন্য print_header (শূন্য);
স্থির শূন্য pad_across_to (পূর্ণাঙ্ক position);
স্থির শূন্য add_সাৰি_সংখ্যা (COLUMN *p);
স্থির পূর্ণাঙ্ক getoptnum (ৱর্ণ ধ্রুৱ *n_str, পূর্ণাঙ্ক min, ৱর্ণ ধ্রুৱ *errfmt);
স্থির শূন্য getoptarg (ৱর্ণ *arg, ৱর্ণ switch_char, ৱর্ণ *character,
                       পূর্ণাঙ্ক *সংখ্যা);
স্থির শূন্য print_files (পূর্ণাঙ্ক সংখ্যা_of_files, ৱর্ণ **av);
স্থির শূন্য init_parameters (পূর্ণাঙ্ক সংখ্যা_of_files);
স্থির শূন্য init_header (ৱর্ণ ধ্রুৱ *filename, পূর্ণাঙ্ক desc);
স্থির bool init_fps (পূর্ণাঙ্ক সংখ্যা_of_files, ৱর্ণ **av);
স্থির শূন্য init_funcs (শূন্য);
স্থির শূন্য init_store_cols (শূন্য);
স্থির শূন্য store_columns (শূন্য);
স্থির শূন্য balance (পূর্ণাঙ্ক total_stored);
স্থির শূন্য store_char (ৱর্ণ c);
স্থির শূন্য pad_down (unsigned পূর্ণাঙ্ক সাৰিগুলি);
স্থির শূন্য read_rest_of_সাৰি (COLUMN *p);
স্থির শূন্য skip_read (COLUMN *p, পূর্ণাঙ্ক column_সংখ্যা);
স্থির শূন্য print_char (ৱর্ণ c);
স্থির শূন্য cleanup (শূন্য);
স্থির শূন্য print_sep_string (শূন্য);
স্থির শূন্য separator_string (ৱর্ণ ধ্রুৱ *optarg_S);

/* All of the columns to print.  */
স্থির COLUMN *column_vector;

/* When printing a single file in multiple downward columns,
   we store the leftmost columns contiguously in buff.
   To print a line from buff, get the index of the first character
   from line_vector[i], and print up to line_vector[i + 1]. */
স্থির ৱর্ণ *buff;

/* Index of the position in buff where the next character
   will be stored. */
স্থির unsigned পূর্ণাঙ্ক buff_current;

/* The number of characters in buff.
   Used for allocation of buff and to detect overflow of buff. */
স্থির idx_t buff_allocated;

/* Array of indices into buff.
   Each entry is an index of the first character of a line.
   This is used when storing lines to facilitate shuffling when
   we do column balancing on the last page. */
স্থির পূর্ণাঙ্ক *সাৰি_vector;

/* Array of horizontal positions.
   For each line in line_vector, end_vector[line] is the horizontal
   position we are in after printing that line.  We keep track of this
   so that we know how much we need to pad to prepare for the next
   column. */
স্থির পূর্ণাঙ্ক *end_vector;

/* (-m) True means we're printing multiple files in parallel. */
স্থির bool parallel_files = false;

/* (-m) True means a line starts with some empty columns (some files
   already CLOSED or ON_HOLD) which we have to align. */
স্থির bool align_empty_cols;

/* (-m) True means we have not yet found any printable column in a line.
   align_empty_cols = true  has to be maintained. */
স্থির bool empty_সাৰি;

/* (-m) False means printable column output precedes a form feed found.
   Column alignment is done only once. No additional action with that form
   feed.
   True means we found only a form feed in a column. Maybe we have to do
   some column alignment with that form feed. */
স্থির bool FF_only;

/* (-[0-9]+) True means we're given an option explicitly specifying
   number of columns.  Used to detect when this option is used with -m
   and when translating old options to new/long options. */
স্থির bool explicit_columns = false;

/* (-t|-T) False means we aren't printing headers and footers. */
স্থির bool extremities = true;

/* (-t) True means we retain all FF set by hand in input files.
   False is set with -T option. */
স্থির bool keep_FF = false;
স্থির bool print_a_FF = false;

/* True means we need to print a header as soon as we know we've got input
   to print after it. */
স্থির bool print_a_header;

/* (-f) True means use formfeeds instead of newlines to separate pages. */
স্থির bool use_form_feed = false;

/* True means we have read the standard input. */
স্থির bool have_read_stdin = false;

/* True means the -a flag has been given. */
স্থির bool print_across_flag = false;

/* True means we're printing one file in multiple (>1) downward columns. */
স্থির bool storing_columns = true;

/* (-b) True means balance columns on the last page as Sys V does. */
/* That's no longer an independent option. With storing_columns = true
   balance_columns = true is used too (s. function init_parameters).
   We get a consistent formulation with "FF set by hand" in input files. */
স্থির bool balance_columns = false;

/* (-l) Number of lines on a page, including header and footer lines. */
স্থির পূর্ণাঙ্ক সাৰিগুলি_per_page = 66;

/* Number of lines in the header and footer can be reset to 0 using
   the -t flag. */
গণনা { সাৰিগুলি_per_header = 5 };
স্থির পূর্ণাঙ্ক সাৰিগুলি_per_body;
গণনা { সাৰিগুলি_per_footer = 5 };

/* (-w|-W) Width in characters of the page.  Does not include the width of
   the margin. */
স্থির পূর্ণাঙ্ক chars_per_সাৰি = 72;

/* (-w|W) True means we truncate lines longer than chars_per_column. */
স্থির bool truncate_সাৰিগুলি = false;

/* (-J) True means we join lines without any line truncation. -J
   dominates -w option. */
স্থির bool join_সাৰিগুলি = false;

/* Number of characters in a column.  Based on col_sep_length and
   page width. */
স্থির পূর্ণাঙ্ক chars_per_column;

/* (-e) True means convert tabs to spaces on input. */
স্থির bool untabify_প্ৰবেশ = false;

/* (-e) The input tab character. */
স্থির ৱর্ণ প্ৰবেশ_tab_char = '\t';

/* (-e) Tabstops are at chars_per_tab, 2*chars_per_tab, 3*chars_per_tab, ...
   where the leftmost column is 1. */
স্থির পূর্ণাঙ্ক chars_per_প্ৰবেশ_tab = 8;

/* (-i) True means convert spaces to tabs on output. */
স্থির bool tabify_output = false;

/* (-i) The output tab character. */
স্থির ৱর্ণ output_tab_char = '\t';

/* (-i) The width of the output tab. */
স্থির পূর্ণাঙ্ক chars_per_output_tab = 8;

/* Keeps track of pending white space.  When we hit a nonspace
   character after some whitespace, we print whitespace, tabbing
   if necessary to get to output_position + spaces_not_printed. */
স্থির পূর্ণাঙ্ক spaces_not_printed;

/* (-o) Number of spaces in the left margin (tabs used when possible). */
স্থির পূর্ণাঙ্ক chars_per_margin = 0;

/* Position where the next character will fall.
   Leftmost position is 0 + chars_per_margin.
   Rightmost position is chars_per_margin + chars_per_line - 1.
   This is important for converting spaces to tabs on output. */
স্থির পূর্ণাঙ্ক output_position;

/* Horizontal position relative to the current file.
   (output_position depends on where we are on the page;
   input_position depends on where we are in the file.)
   Important for converting tabs to spaces on input. */
স্থির পূর্ণাঙ্ক প্ৰবেশ_position;

/* True if there were any failed opens so we can exit with nonzero
   status.  */
স্থির bool failed_opens = false;

/* The number of spaces taken up if we print a tab character with width
   c_ from position h_. */
#define TAB_WIDTH(c_, h_) ((c_) - ((h_) % (c_)))

/* The horizontal position we'll be at after printing a tab character
   of width c_ from the position h_. */
#define POS_AFTER_TAB(c_, h_) ((h_) + TAB_WIDTH (c_, h_))

/* (-NNN) Number of columns of text to print. */
স্থির পূর্ণাঙ্ক columns = 1;

/* (+NNN:MMM) Page numbers on which to begin and stop printing.
   first_page_number = 0  will be used to check input only. */
স্থির uintmax_t first_page_সংখ্যা = 0;
স্থির uintmax_t last_page_সংখ্যা = UINTMAX_MAX;

/* Number of files open (not closed, not on hold). */
স্থির পূর্ণাঙ্ক files_ready_to_read = 0;

/* Current page number.  Displayed in header. */
স্থির uintmax_t page_সংখ্যা;

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
স্থির পূর্ণাঙ্ক সাৰি_সংখ্যা;

/* (-n) True means lines should be preceded by numbers. */
স্থির bool numbered_সাৰিগুলি = false;

/* (-n) Character which follows each line number. */
স্থির ৱর্ণ সংখ্যা_separator = '\t';

/* (-n) line counting starts with 1st line of input file (not with 1st
   line of 1st page printed). */
স্থির পূর্ণাঙ্ক সাৰি_গুনতি = 1;

/* (-n) True means counting of skipped lines starts with 1st line of
   input file. False means -N option is used in addition, counting of
   skipped lines not required. */
স্থির bool skip_গুনতি = true;

/* (-N) Counting starts with start_line_number = NUMBER at 1st line of
   first page printed, usually not 1st page of input file. */
স্থির পূর্ণাঙ্ক start_সাৰি_অঙ্ক = 1;

/* (-n) Width in characters of a line number. */
স্থির পূর্ণাঙ্ক chars_per_সংখ্যা = 5;

/* Used when widening the first column to accommodate numbers -- only
   needed when printing files in parallel.  Includes width of both the
   number and the number_separator. */
স্থির পূর্ণাঙ্ক সংখ্যা_width;

/* Buffer sprintf uses to format a line number. */
স্থির ৱর্ণ *সংখ্যা_buff;

/* (-v) True means nonprintable characters are printed as escape sequences.
   control-g becomes \007. */
স্থির bool use_esc_sequence = false;

/* (-c) True means nonprintable characters are printed as control prefixes.
   control-g becomes ^G. */
স্থির bool use_cntrl_prefix = false;

/* (-d) True means output is double spaced. */
স্থির bool double_space = false;

/* Number of files opened initially in init_files.  Should be 1
   unless we're printing multiple files in parallel. */
স্থির পূর্ণাঙ্ক total_files = 0;

/* (-r) True means don't complain if we can't open a file. */
স্থির bool ignore_failed_opens = false;

/* (-S) True means we separate columns with a specified string.
   -S option does not affect line truncation nor column alignment. */
স্থির bool use_col_separator = false;

/* String used to separate columns if the -S option has been specified.
   Default without -S but together with one of the column options
   -a|COLUMN|-m is a 'space' and with the -J option a 'tab'. */
স্থির ৱর্ণ ধ্রুৱ *col_sep_string = "";
স্থির পূর্ণাঙ্ক col_sep_length = 0;
স্থির ৱর্ণ *column_separator = (ৱর্ণ *) " ";
স্থির ৱর্ণ *সাৰি_separator = (ৱর্ণ *) "\t";

/* Number of separator characters waiting to be printed as soon as we
   know that we have any input remaining to be printed. */
স্থির পূর্ণাঙ্ক separators_not_printed;

/* Position we need to pad to, as soon as we know that we have input
   remaining to be printed. */
স্থির পূর্ণাঙ্ক padding_not_printed;

/* True means we should pad the end of the page.  Remains false until we
   know we have a page to print. */
স্থির bool pad_vertically;

/* (-h) String of characters used in place of the filename in the header. */
স্থির ৱর্ণ *custom_header;

/* (-D) Date format for the header.  */
স্থির ৱর্ণ ধ্রুৱ *date_format;

/* The local time zone rules, as per the TZ environment variable.  */
স্থির timezone_t localtz;

/* Date and file name for the header.  */
স্থির ৱর্ণ *date_text;
স্থির ৱর্ণ ধ্রুৱ *file_text;

/* Output columns available, not counting the date and file name.  */
স্থির পূর্ণাঙ্ক header_width_available;

স্থির ৱর্ণ *clump_buff;

/* True means we read the line no. lines_per_body in skip_read
   called by skip_to_page. That variable controls the coincidence of a
   "FF set by hand" and "full_page_printed", see above the definition of
   structure COLUMN. */
স্থির bool last_সাৰি = false;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
গণনা
{
  COLUMNS_OPTION = CHAR_MAX + 1,
  PAGES_OPTION
};

স্থির ৱর্ণ ধ্রুৱ short_options[] =
  "-0123456789D:FJN:S::TW:abcde::fh:i::l:mn::o:rs::tvw:";

স্থির সংরচনা option ধ্রুৱ long_options[] =
{
  {"pages", required_argument, রিক্ত, PAGES_OPTION},
  {"columns", required_argument, রিক্ত, COLUMNS_OPTION},
  {"across", no_argument, রিক্ত, 'a'},
  {"show-control-chars", no_argument, রিক্ত, 'c'},
  {"double-space", no_argument, রিক্ত, 'd'},
  {"date-format", required_argument, রিক্ত, 'D'},
  {"expand-tabs", optional_argument, রিক্ত, 'e'},
  {"form-feed", no_argument, রিক্ত, 'f'},
  {"header", required_argument, রিক্ত, 'h'},
  {"output-tabs", optional_argument, রিক্ত, 'i'},
  {"join-lines", no_argument, রিক্ত, 'J'},
  {"length", required_argument, রিক্ত, 'l'},
  {"merge", no_argument, রিক্ত, 'm'},
  {"number-lines", optional_argument, রিক্ত, 'n'},
  {"first-line-number", required_argument, রিক্ত, 'N'},
  {"indent", required_argument, রিক্ত, 'o'},
  {"no-file-warnings", no_argument, রিক্ত, 'r'},
  {"separator", optional_argument, রিক্ত, 's'},
  {"sep-string", optional_argument, রিক্ত, 'S'},
  {"omit-header", no_argument, রিক্ত, 't'},
  {"omit-pagination", no_argument, রিক্ত, 'T'},
  {"show-nonprinting", no_argument, রিক্ত, 'v'},
  {"width", required_argument, রিক্ত, 'w'},
  {"page-width", required_argument, রিক্ত, 'W'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {রিক্ত, 0, রিক্ত, 0}
};

স্থির _Noreturn শূন্য
integer_overflow (শূন্য)
{
  error (EXIT_FAILURE, 0, _("integer overflow"));
}

/* Return the number of columns that have either an open file or
   stored lines. */

ATTRIBUTE_PURE
স্থির unsigned পূর্ণাঙ্ক
cols_ready_to_print (শূন্য)
{
  COLUMN *q;
  unsigned পূর্ণাঙ্ক i;
  unsigned পূর্ণাঙ্ক n;

  n = 0;
  বাবে (q = column_vector, i = 0; i < columns; ++q, ++i)
    যদি (q->status == OPEN
        || q->status == FF_FOUND	/* With -b: To print a header only */
        || (storing_columns && q->সাৰিগুলি_stored > 0 && q->সাৰিগুলি_to_print > 0))
      ++n;
  ঘূৰাই n;
}

/* Estimate first_ / last_page_number
   using option +FIRST_PAGE:LAST_PAGE */

স্থির bool
first_last_page (পূর্ণাঙ্ক oi, ৱর্ণ c, ৱর্ণ ধ্রুৱ *pages)
{
  ৱর্ণ *p;
  uintmax_t first;
  uintmax_t last = UINTMAX_MAX;
  strtol_error err = xstrtoumax (pages, &p, 10, &first, "");
  যদি (err != LONGINT_OK && err != LONGINT_INVALID_SUFFIX_CHAR)
    xstrtol_fatal (err, oi, c, long_options, pages);

  যদি (p == pages || !first)
    ঘূৰাই false;

  যদি (*p == ':')
    {
      ৱর্ণ ধ্রুৱ *p1 = p + 1;
      err = xstrtoumax (p1, &p, 10, &last, "");
      যদি (err != LONGINT_OK)
        xstrtol_fatal (err, oi, c, long_options, pages);
      যদি (p1 == p || last < first)
        ঘূৰাই false;
    }

  যদি (*p)
    ঘূৰাই false;

  first_page_সংখ্যা = first;
  last_page_সংখ্যা = last;
  ঘূৰাই true;
}

/* Parse column count string S, and if it's valid (1 or larger and
   within range of the type of 'columns') set the global variables
   columns and explicit_columns.  Otherwise, exit with a diagnostic.  */

স্থির শূন্য
parse_column_গুনতি (ৱর্ণ ধ্রুৱ *s)
{
  columns = getoptnum (s, 1, _("invalid number of columns"));
  explicit_columns = true;
}

/* Estimate length of col_sep_string with option -S.  */

স্থির শূন্য
separator_string (ৱর্ণ ধ্রুৱ *optarg_S)
{
  size_t len = strlen (optarg_S);
  যদি (INT_MAX < len)
    integer_overflow ();
  col_sep_length = len;
  col_sep_string = optarg_S;
}

পূর্ণাঙ্ক
main (পূর্ণাঙ্ক argc, ৱর্ণ **argv)
{
  unsigned পূর্ণাঙ্ক n_files;
  bool old_options = false;
  bool old_w = false;
  bool old_s = false;
  ৱর্ণ **file_names;

  /* Accumulate the digits of old-style options like -99.  */
  ৱর্ণ *column_গুনতি_string = রিক্ত;
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
                ? xnmalloc (argc - 1, পরিমাণ (ৱর্ণ *))
                : রিক্ত);

  যেতিয়ালৈ (true)
    {
      পূর্ণাঙ্ক oi = -1;
      পূর্ণাঙ্ক c = getopt_long (argc, argv, short_options, long_options, &oi);
      যদি (c == -1)
        ভঙ্গ;

      যদি (c_isdigit (c))
        {
          /* Accumulate column-count digits specified via old-style options. */
          যদি (n_digits + 1 >= n_alloc)
            column_গুনতি_string = xpalloc (column_গুনতি_string, &n_alloc, 2, -1,
                                           পরিমাণ *column_গুনতি_string);
          column_গুনতি_string[n_digits++] = c;
          column_গুনতি_string[n_digits] = '\0';
          অনুৱর্ত;
        }

      n_digits = 0;

      ৱিকল্প (c)
        {
        পক্ষ 1:			/* Non-option argument. */
          /* long option --page dominates old '+FIRST_PAGE ...'.  */
          যদি (! (first_page_সংখ্যা == 0
                 && *optarg == '+' && first_last_page (-2, '+', optarg + 1)))
            file_names[n_files++] = optarg;
          ভঙ্গ;

        পক্ষ PAGES_OPTION:	/* --pages=FIRST_PAGE[:LAST_PAGE] */
          {			/* dominates old opt +... */
            যদি (! optarg)
              error (EXIT_FAILURE, 0,
                     _("'--pages=FIRST_PAGE[:LAST_PAGE]' missing argument"));
            অন্যথা যদি (! first_last_page (oi, 0, optarg))
              error (EXIT_FAILURE, 0, _("invalid page range %s"),
                     quote (optarg));
            ভঙ্গ;
          }

        পক্ষ COLUMNS_OPTION:	/* --columns=COLS */
          {
            parse_column_গুনতি (optarg);

            /* If there was a prior column count specified via the
               short-named option syntax, e.g., -9, ensure that this
               long-name-specified value overrides it.  */
            মুক্ত (column_গুনতি_string);
            column_গুনতি_string = রিক্ত;
            n_alloc = 0;
            ভঙ্গ;
          }

        পক্ষ 'a':
          print_across_flag = true;
          storing_columns = false;
          ভঙ্গ;
        পক্ষ 'b':
          balance_columns = true;
          ভঙ্গ;
        পক্ষ 'c':
          use_cntrl_prefix = true;
          ভঙ্গ;
        পক্ষ 'd':
          double_space = true;
          ভঙ্গ;
        পক্ষ 'D':
          date_format = optarg;
          ভঙ্গ;
        পক্ষ 'e':
          যদি (optarg)
            getoptarg (optarg, 'e', &প্ৰবেশ_tab_char,
                       &chars_per_প্ৰবেশ_tab);
          /* Could check tab width > 0. */
          untabify_প্ৰবেশ = true;
          ভঙ্গ;
        পক্ষ 'f':
        পক্ষ 'F':
          use_form_feed = true;
          ভঙ্গ;
        পক্ষ 'h':
          custom_header = optarg;
          ভঙ্গ;
        পক্ষ 'i':
          যদি (optarg)
            getoptarg (optarg, 'i', &output_tab_char,
                       &chars_per_output_tab);
          /* Could check tab width > 0. */
          tabify_output = true;
          ভঙ্গ;
        পক্ষ 'J':
          join_সাৰিগুলি = true;
          ভঙ্গ;
        পক্ষ 'l':
          সাৰিগুলি_per_page
            = getoptnum (optarg, 1,
                         _("'-l PAGE_LENGTH' invalid number of lines"));
          ভঙ্গ;
        পক্ষ 'm':
          parallel_files = true;
          storing_columns = false;
          ভঙ্গ;
        পক্ষ 'n':
          numbered_সাৰিগুলি = true;
          যদি (optarg)
            getoptarg (optarg, 'n', &সংখ্যা_separator,
                       &chars_per_সংখ্যা);
          ভঙ্গ;
        পক্ষ 'N':
          skip_গুনতি = false;
          start_সাৰি_অঙ্ক
            = getoptnum (optarg, INT_MIN,
                         _("'-N NUMBER' invalid starting line number"));
          ভঙ্গ;
        পক্ষ 'o':
          chars_per_margin = getoptnum (optarg, 0,
                                        _("'-o MARGIN' invalid line offset"));
          ভঙ্গ;
        পক্ষ 'r':
          ignore_failed_opens = true;
          ভঙ্গ;
        পক্ষ 's':
          old_options = true;
          old_s = true;
          যদি (!use_col_separator && optarg)
            separator_string (optarg);
          ভঙ্গ;
        পক্ষ 'S':
          old_s = false;
          /* Reset an additional input of -s, -S dominates -s */
          col_sep_string = "";
          col_sep_length = 0;
          use_col_separator = true;
          যদি (optarg)
            separator_string (optarg);
          ভঙ্গ;
        পক্ষ 't':
          extremities = false;
          keep_FF = true;
          ভঙ্গ;
        পক্ষ 'T':
          extremities = false;
          keep_FF = false;
          ভঙ্গ;
        পক্ষ 'v':
          use_esc_sequence = true;
          ভঙ্গ;
        পক্ষ 'w':
          old_options = true;
          old_w = true;
          {
            পূর্ণাঙ্ক tmp_cpl
              = getoptnum (optarg, 1,
                           _("'-w PAGE_WIDTH' invalid number of characters"));
            যদি (! truncate_সাৰিগুলি)
              chars_per_সাৰি = tmp_cpl;
          }
          ভঙ্গ;
        পক্ষ 'W':
          old_w = false;			/* dominates -w */
          truncate_সাৰিগুলি = true;
          chars_per_সাৰি
            = getoptnum (optarg, 1,
                         _("'-W PAGE_WIDTH' invalid number of characters"));
          ভঙ্গ;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        স্ৱভাৱ:
          usage (EXIT_FAILURE);
          ভঙ্গ;
        }
    }

  যদি (column_গুনতি_string)
    {
      parse_column_গুনতি (column_গুনতি_string);
      মুক্ত (column_গুনতি_string);
    }

  যদি (! date_format)
    date_format = (getenv ("POSIXLY_CORRECT") && !hard_locale (LC_TIME)
                   ? "%b %e %H:%M %Y"
                   : "%Y-%m-%d %H:%M");

  localtz = tzalloc (getenv ("TZ"));

  /* Now we can set a reasonable initial value: */
  যদি (first_page_সংখ্যা == 0)
    first_page_সংখ্যা = 1;

  যদি (parallel_files && explicit_columns)
    error (EXIT_FAILURE, 0,
           _("cannot specify number of columns when printing in parallel"));

  যদি (parallel_files && print_across_flag)
    error (EXIT_FAILURE, 0,
           _("cannot specify both printing across and printing in parallel"));

/* Translate some old short options to new/long options.
   To meet downward compatibility with other UNIX pr utilities
   and some POSIX specifications. */

  যদি (old_options)
    {
      যদি (old_w)
        {
          যদি (parallel_files || explicit_columns)
            {
              /* activate -W */
              truncate_সাৰিগুলি = true;
              যদি (old_s)
                /* adapt HP-UX and SunOS: -s = no separator;
                   activate -S */
                use_col_separator = true;
            }
          অন্যথা
            /* old -w sets width with columns only
               activate -J */
            join_সাৰিগুলি = true;
        }
      অন্যথা যদি (!use_col_separator)
        {
          /* No -S option read */
          যদি (old_s && (parallel_files || explicit_columns))
            {
              যদি (!truncate_সাৰিগুলি)
                {
                  /* old -s (without -w and -W) annuls column alignment,
                  uses fields, activate -J */
                  join_সাৰিগুলি = true;
                  যদি (col_sep_length > 0)
                    /* activate -S */
                    use_col_separator = true;
                }
              অন্যথা
                /* with -W */
                /* adapt HP-UX and SunOS: -s = no separator;
                   activate -S */
                use_col_separator = true;
            }
        }
    }

  বাবে (; optind < argc; optind++)
    {
      file_names[n_files++] = argv[optind];
    }

  যদি (n_files == 0)
    {
      /* No file arguments specified;  read from standard input.  */
      print_files (0, রিক্ত);
    }
  অন্যথা
    {
      যদি (parallel_files)
        print_files (n_files, file_names);
      অন্যথা
        {
          বাবে (unsigned পূর্ণাঙ্ক i = 0; i < n_files; i++)
            print_files (1, &file_names[i]);
        }
    }

  cleanup ();

  যদি (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, _("standard input"));
  main_exit (failed_opens ? EXIT_FAILURE : EXIT_SUCCESS);
}

/* Parse numeric arguments, ensuring MIN <= number <= INT_MAX.  */

স্থির পূর্ণাঙ্ক
getoptnum (ৱর্ণ ধ্রুৱ *n_str, পূর্ণাঙ্ক min, ৱর্ণ ধ্রুৱ *err)
{
  ঘূৰাই xnumtoimax (n_str, 10, min, INT_MAX, "", err, 0,
                     min <= 0 ? 0 : XTOINT_MIN_RANGE);
}

/* Parse options of the form -scNNN.

   Example: -nck, where 'n' is the option, c is the optional number
   separator, and k is the optional width of the field used when printing
   a number. */

স্থির শূন্য
getoptarg (ৱর্ণ *arg, ৱর্ণ switch_char, ৱর্ণ *character, পূর্ণাঙ্ক *সংখ্যা)
{
  ৱর্ণ ধ্রুৱ *অঙ্ক = arg;

  যদি (!*অঙ্ক)
    {
      error (0, 0, _("'-%c': Invalid argument: %s"), switch_char, quote (অঙ্ক));
      usage (EXIT_FAILURE);
    }

  যদি (!c_isdigit (*অঙ্ক))
    *character = *অঙ্ক++;
  যদি (*অঙ্ক)
    {
      long পূর্ণাঙ্ক tmp_long;
      strtol_error e = xstrtol (অঙ্ক, রিক্ত, 10, &tmp_long, "");
      যদি (e == LONGINT_OK)
        {
          যদি (tmp_long <= 0)
            e = LONGINT_INVALID;
          অন্যথা যদি (INT_MAX < tmp_long)
            e = LONGINT_OVERFLOW;
        }
      যদি (e != LONGINT_OK)
        {
          error (0, e & LONGINT_OVERFLOW ? EOVERFLOW : 0,
             _("'-%c' extra characters or invalid number in the argument: %s"),
                 switch_char, quote (arg));
          usage (EXIT_FAILURE);
        }
      *সংখ্যা = tmp_long;
    }
}

/* Set parameters related to formatting. */

স্থির শূন্য
init_parameters (পূর্ণাঙ্ক সংখ্যা_of_files)
{
  পূর্ণাঙ্ক chars_used_by_সংখ্যা = 0;

  সাৰিগুলি_per_body = সাৰিগুলি_per_page - সাৰিগুলি_per_header - সাৰিগুলি_per_footer;
  যদি (সাৰিগুলি_per_body <= 0)
    {
      extremities = false;
      keep_FF = true;
    }
  যদি (! extremities)
    সাৰিগুলি_per_body = সাৰিগুলি_per_page;

  যদি (double_space)
    সাৰিগুলি_per_body = MAX (1, সাৰিগুলি_per_body / 2);

  /* If input is stdin, cannot print parallel files.  BSD dumps core
     on this. */
  যদি (সংখ্যা_of_files == 0)
    parallel_files = false;

  যদি (parallel_files)
    columns = সংখ্যা_of_files;

  /* One file, multi columns down: -b option is set to get a consistent
     formulation with "FF set by hand" in input files. */
  যদি (storing_columns)
    balance_columns = true;

  /* Tabification is assumed for multiple columns. */
  যদি (columns > 1)
    {
      যদি (!use_col_separator)
        {
          /* Use default separator */
          যদি (join_সাৰিগুলি)
            col_sep_string = সাৰি_separator;
          অন্যথা
            col_sep_string = column_separator;

          col_sep_length = 1;
          use_col_separator = true;
        }
      /* It's rather pointless to define a TAB separator with column
         alignment */
      অন্যথা যদি (!join_সাৰিগুলি && col_sep_length == 1 && *col_sep_string == '\t')
        col_sep_string = column_separator;

      truncate_সাৰিগুলি = true;
      যদি (! (col_sep_length == 1 && *col_sep_string == '\t'))
        untabify_প্ৰবেশ = true;
      tabify_output = true;
    }
  অন্যথা
    storing_columns = false;

  /* -J dominates -w in any case */
  যদি (join_সাৰিগুলি)
    truncate_সাৰিগুলি = false;

  যদি (numbered_সাৰিগুলি)
    {
      পূর্ণাঙ্ক chars_per_default_tab = 8;

      সাৰি_গুনতি = start_সাৰি_অঙ্ক;

      /* To allow input tab-expansion (-e sensitive) use:
         if (number_separator == input_tab_char)
           number_width = chars_per_number
             + TAB_WIDTH (chars_per_input_tab, chars_per_number);   */

      /* Estimate chars_per_text without any margin and keep it constant. */
      যদি (সংখ্যা_separator == '\t')
        সংখ্যা_width = (chars_per_সংখ্যা
                        + TAB_WIDTH (chars_per_default_tab, chars_per_সংখ্যা));
      অন্যথা
        সংখ্যা_width = chars_per_সংখ্যা + 1;

      /* The number is part of the column width unless we are
         printing files in parallel. */
      যদি (parallel_files)
        chars_used_by_সংখ্যা = সংখ্যা_width;
    }

  পূর্ণাঙ্ক sep_chars, useful_chars;
  যদি (ckd_mul (&sep_chars, columns - 1, col_sep_length))
    sep_chars = INT_MAX;
  যদি (ckd_sub (&useful_chars, chars_per_সাৰি - chars_used_by_সংখ্যা,
               sep_chars))
    useful_chars = 0;
  chars_per_column = useful_chars / columns;

  যদি (chars_per_column < 1)
    error (EXIT_FAILURE, 0, _("page width too narrow"));

  যদি (numbered_সাৰিগুলি)
    {
      মুক্ত (সংখ্যা_buff);
      সংখ্যা_buff = xmalloc (MAX (chars_per_সংখ্যা,
                                  INT_STRLEN_BOUND (সাৰি_সংখ্যা)) + 1);
    }

  /* Pick the maximum between the tab width and the width of an
     escape sequence.
     The width of an escape sequence (4) isn't the lower limit any longer.
     We've to use 8 as the lower limit, if we use chars_per_default_tab = 8
     to expand a tab which is not an input_tab-char. */
  মুক্ত (clump_buff);
  clump_buff = xmalloc (MAX (8, chars_per_প্ৰবেশ_tab));
}

/* Open the necessary files,
   maintaining a COLUMN structure for each column.

   With multiple files, each column p has a different p->fp.
   With single files, each column p has the same p->fp.
   Return false if (number_of_files > 0) and no files can be opened,
   true otherwise.

   With each column/file p, p->full_page_printed is initialized,
   see also open_file.  */

স্থির bool
init_fps (পূর্ণাঙ্ক সংখ্যা_of_files, ৱর্ণ **av)
{
  COLUMN *p;

  total_files = 0;

  মুক্ত (column_vector);
  column_vector = xnmalloc (columns, পরিমাণ (COLUMN));

  যদি (parallel_files)
    {
      পূর্ণাঙ্ক files_বাম = সংখ্যা_of_files;
      বাবে (p = column_vector; files_বাম--; ++p, ++av)
        {
          যদি (! open_file (*av, p))
            {
              --p;
              --columns;
            }
        }
      যদি (columns == 0)
        ঘূৰাই false;
      init_header ("", -1);
    }
  অন্যথা
    {
      p = column_vector;
      যদি (সংখ্যা_of_files > 0)
        {
          যদি (! open_file (*av, p))
            ঘূৰাই false;
          init_header (*av, fileno (p->fp));
          p->সাৰিগুলি_stored = 0;
        }
      অন্যথা
        {
          p->name = _("standard input");
          p->fp = stdin;
          have_read_stdin = true;
          p->status = OPEN;
          p->full_page_printed = false;
          ++total_files;
          init_header ("", -1);
          p->সাৰিগুলি_stored = 0;
        }

      ৱর্ণ ধ্রুৱ *firstname = p->name;
      FILE *firstfp = p->fp;
      পূর্ণাঙ্ক i;
      বাবে (i = columns - 1, ++p; i; --i, ++p)
        {
          p->name = firstname;
          p->fp = firstfp;
          p->status = OPEN;
          p->full_page_printed = false;
          p->সাৰিগুলি_stored = 0;
        }
    }
  files_ready_to_read = total_files;
  ঘূৰাই true;
}

/* Determine print_func and char_func, the functions
   used by each column for printing and/or storing.

   Determine the horizontal position desired when we begin
   printing a column (p->start_position). */

স্থির শূন্য
init_funcs (শূন্য)
{
  পূর্ণাঙ্ক i, h, h_next;
  COLUMN *p;

  h = chars_per_margin;

  যদি (!truncate_সাৰিগুলি)
    h_next = ANYWHERE;
  অন্যথা
    {
      /* When numbering lines of parallel files, we enlarge the
         first column to accommodate the number.  Looks better than
         the Sys V approach. */
      যদি (parallel_files && numbered_সাৰিগুলি)
        h_next = h + chars_per_column + সংখ্যা_width;
      অন্যথা
        h_next = h + chars_per_column;
    }

  /* Enlarge p->start_position of first column to use the same form of
     padding_not_printed with all columns. */
  h = h + col_sep_length;

  /* This loop takes care of all but the rightmost column. */

  বাবে (p = column_vector, i = 1; i < columns; ++p, ++i)
    {
      যদি (storing_columns)	/* One file, multi columns down. */
        {
          p->char_func = store_char;
          p->print_func = print_stored;
        }
      অন্যথা
        /* One file, multi columns across; or parallel files.  */
        {
          p->char_func = print_char;
          p->print_func = read_সাৰি;
        }

      /* Number only the first column when printing files in
         parallel. */
      p->numbered = numbered_সাৰিগুলি && (!parallel_files || i == 1);
      p->start_position = h;

      /* If we don't truncate lines, all start_positions are
         ANYWHERE, except the first column's start_position when
         using a margin. */

      যদি (!truncate_সাৰিগুলি)
        {
          h = ANYWHERE;
          h_next = ANYWHERE;
        }
      অন্যথা
        {
          h = h_next + col_sep_length;
          h_next = h + chars_per_column;
        }
    }

  /* The rightmost column.

     Doesn't need to be stored unless we intend to balance
     columns on the last page. */
  যদি (storing_columns && balance_columns)
    {
      p->char_func = store_char;
      p->print_func = print_stored;
    }
  অন্যথা
    {
      p->char_func = print_char;
      p->print_func = read_সাৰি;
    }

  p->numbered = numbered_সাৰিগুলি && (!parallel_files || i == 1);
  p->start_position = h;
}

/* Open a file.  Return true if successful.

   With each file p, p->full_page_printed is initialized,
   see also init_fps. */

স্থির bool
open_file (ৱর্ণ *name, COLUMN *p)
{
  যদি (streq (name, "-"))
    {
      p->name = _("standard input");
      p->fp = stdin;
      have_read_stdin = true;
    }
  অন্যথা
    {
      p->name = name;
      p->fp = fopen (name, "r");
    }
  যদি (p->fp == রিক্ত)
    {
      failed_opens = true;
      যদি (!ignore_failed_opens)
        error (0, errno, "%s", quotef (name));
      ঘূৰাই false;
    }
  fadvise (p->fp, FADVISE_SEQUENTIAL);
  p->status = OPEN;
  p->full_page_printed = false;
  ++total_files;
  ঘূৰাই true;
}

/* Close the file in P.

   If we aren't dealing with multiple files in parallel, we change
   the status of all columns in the column list to reflect the close. */

স্থির শূন্য
close_file (COLUMN *p)
{
  COLUMN *q;
  পূর্ণাঙ্ক i;

  যদি (p->status == CLOSED)
    ঘূৰাই;

  পূর্ণাঙ্ক err = errno;
  যদি (!ferror (p->fp))
    err = 0;
  যদি (fileno (p->fp) == STDIN_FILENO)
    clearerr (p->fp);
  অন্যথা যদি (fclose (p->fp) != 0 && !err)
    err = errno;
  যদি (err)
    error (EXIT_FAILURE, err, "%s", quotef (p->name));

  যদি (!parallel_files)
    {
      বাবে (q = column_vector, i = columns; i; ++q, --i)
        {
          q->status = CLOSED;
          যদি (q->সাৰিগুলি_stored == 0)
            {
              q->সাৰিগুলি_to_print = 0;
            }
        }
    }
  অন্যথা
    {
      p->status = CLOSED;
      p->সাৰিগুলি_to_print = 0;
    }

  --files_ready_to_read;
}

/* Put a file on hold until we start a new page,
   since we've hit a form feed.

   If we aren't dealing with parallel files, we must change the
   status of all columns in the column list. */

স্থির শূন্য
hold_file (COLUMN *p)
{
  COLUMN *q;
  পূর্ণাঙ্ক i;

  যদি (!parallel_files)
    বাবে (q = column_vector, i = columns; i; ++q, --i)
      {
        যদি (storing_columns)
          q->status = FF_FOUND;
        অন্যথা
          q->status = ON_HOLD;
      }
  অন্যথা
    p->status = ON_HOLD;

  p->সাৰিগুলি_to_print = 0;
  --files_ready_to_read;
}

/* Undo hold_file -- go through the column list and change any
   ON_HOLD columns to OPEN.  Used at the end of each page. */

স্থির শূন্য
reset_status (শূন্য)
{
  পূর্ণাঙ্ক i = columns;
  COLUMN *p;

  বাবে (p = column_vector; i; --i, ++p)
    যদি (p->status == ON_HOLD)
      {
        p->status = OPEN;
        files_ready_to_read++;
      }

  যদি (storing_columns)
    {
      যদি (column_vector->status == CLOSED)
        /* We use the info to output an error message in  skip_to_page. */
        files_ready_to_read = 0;
      অন্যথা
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

স্থির শূন্য
print_files (পূর্ণাঙ্ক সংখ্যা_of_files, ৱর্ণ **av)
{
  init_parameters (সংখ্যা_of_files);
  যদি (! init_fps (সংখ্যা_of_files, av))
    ঘূৰাই;
  যদি (storing_columns)
    init_store_cols ();

  যদি (first_page_সংখ্যা > 1)
    {
      যদি (!skip_to_page (first_page_সংখ্যা))
        ঘূৰাই;
      অন্যথা
        page_সংখ্যা = first_page_সংখ্যা;
    }
  অন্যথা
    page_সংখ্যা = 1;

  init_funcs ();

  সাৰি_সংখ্যা = সাৰি_গুনতি;
  যেতিয়ালৈ (print_page ())
    ;
}

/* Initialize header information.
   If DESC is non-negative, it is a file descriptor open to
   FILENAME for reading.  */

স্থির শূন্য
init_header (ৱর্ণ ধ্রুৱ *filename, পূর্ণাঙ্ক desc)
{
  ৱর্ণ *buf = রিক্ত;
  সংরচনা stat st;
  সংরচনা timespec t;
  পূর্ণাঙ্ক ns;
  সংরচনা tm tm;

  /* If parallel files or standard input, use current date. */
  যদি (streq (filename, "-"))
    desc = -1;
  যদি (0 <= desc && fstat (desc, &st) == 0)
    t = get_stat_mtime (&st);
  অন্যথা
    {
      স্থির সংরচনা timespec timespec;
      যদি (! timespec.tv_sec)
        gettime (&timespec);
      t = timespec;
    }

  ns = t.tv_nsec;
  যদি (localtime_rz (localtz, &t.tv_sec, &tm))
    {
      ptrdiff_t len = nstrftime (রিক্ত, MIN (PTRDIFF_MAX, SIZE_MAX),
                                 date_format, &tm, localtz, ns);
      যদি (0 <= len)
        {
          buf = ximalloc (len + 1);
          nstrftime (buf, len + 1, date_format, &tm, localtz, ns);
        }
    }
  যদি (!buf)
    {
      ৱর্ণ secbuf[INT_BUFSIZE_BOUND (intmax_t)];
      buf = xmalloc (পরিমাণ secbuf + MAX (10, INT_BUFSIZE_BOUND (পূর্ণাঙ্ক)));
      sprintf (buf, "%s.%09d", timetostr (t.tv_sec, secbuf), ns);
    }

  মুক্ত (date_text);
  date_text = buf;
  file_text = custom_header ? custom_header : desc < 0 ? "" : filename;
  header_width_available = (chars_per_সাৰি
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

স্থির শূন্য
init_page (শূন্য)
{
  পূর্ণাঙ্ক j;
  COLUMN *p;

  যদি (storing_columns)
    {
      store_columns ();
      বাবে (j = columns - 1, p = column_vector; j; --j, ++p)
        {
          p->সাৰিগুলি_to_print = p->সাৰিগুলি_stored;
        }

      /* Last column. */
      যদি (balance_columns)
        {
          p->সাৰিগুলি_to_print = p->সাৰিগুলি_stored;
        }
      /* Since we're not balancing columns, we don't need to store
         the rightmost column.   Read it straight from the file. */
      অন্যথা
        {
          যদি (p->status == OPEN)
            {
              p->সাৰিগুলি_to_print = সাৰিগুলি_per_body;
            }
          অন্যথা
            p->সাৰিগুলি_to_print = 0;
        }
    }
  অন্যথা
    বাবে (j = columns, p = column_vector; j; --j, ++p)
      যদি (p->status == OPEN)
        {
          p->সাৰিগুলি_to_print = সাৰিগুলি_per_body;
        }
      অন্যথা
        p->সাৰিগুলি_to_print = 0;
}

/* Align empty columns and print separators.
   Empty columns will be formed by files with status ON_HOLD or CLOSED
   when printing multiple files in parallel. */

স্থির শূন্য
align_column (COLUMN *p)
{
  padding_not_printed = p->start_position;
  যদি (col_sep_length < padding_not_printed)
    {
      pad_across_to (padding_not_printed - col_sep_length);
      padding_not_printed = ANYWHERE;
    }

  যদি (use_col_separator)
    print_sep_string ();

  যদি (p->numbered)
    add_সাৰি_সংখ্যা (p);
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

স্থির bool
print_page (শূন্য)
{
  পূর্ণাঙ্ক j;
  পূর্ণাঙ্ক সাৰিগুলি_বাম_on_page;
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

  যদি (cols_ready_to_print () == 0)
    ঘূৰাই false;

  যদি (extremities)
    print_a_header = true;

  /* Don't pad unless we know a page was printed. */
  pad_vertically = false;
  pv = false;

  সাৰিগুলি_বাম_on_page = সাৰিগুলি_per_body;
  যদি (double_space)
    সাৰিগুলি_বাম_on_page *= 2;

  যেতিয়ালৈ (সাৰিগুলি_বাম_on_page > 0 && cols_ready_to_print () > 0)
    {
      output_position = 0;
      spaces_not_printed = 0;
      separators_not_printed = 0;
      pad_vertically = false;
      align_empty_cols = false;
      empty_সাৰি = true;

      বাবে (j = 1, p = column_vector; j <= columns; ++j, ++p)
        {
          প্ৰবেশ_position = 0;
          যদি (p->সাৰিগুলি_to_print > 0 || p->status == FF_FOUND)
            {
              FF_only = false;
              padding_not_printed = p->start_position;
              যদি (!(p->print_func) (p))
                read_rest_of_সাৰি (p);
              pv |= pad_vertically;

              --p->সাৰিগুলি_to_print;
              যদি (p->সাৰিগুলি_to_print <= 0)
                {
                  যদি (cols_ready_to_print () == 0)
                    ভঙ্গ;
                }

              /* File p changed its status to ON_HOLD or CLOSED */
              যদি (parallel_files && p->status != OPEN)
                {
                  যদি (empty_সাৰি)
                    align_empty_cols = true;
                  অন্যথা যদি (p->status == CLOSED
                           || (p->status == ON_HOLD && FF_only))
                    align_column (p);
                }
            }
          অন্যথা যদি (parallel_files)
            {
              /* File status ON_HOLD or CLOSED */
              যদি (empty_সাৰি)
                align_empty_cols = true;
              অন্যথা
                align_column (p);
            }

          /* We need it also with an empty column */
          যদি (use_col_separator)
            ++separators_not_printed;
        }

      যদি (pad_vertically)
        {
          putchar ('\n');
          --সাৰিগুলি_বাম_on_page;
        }

      যদি (cols_ready_to_print () == 0 && !extremities)
        ভঙ্গ;

      যদি (double_space && pv)
        {
          putchar ('\n');
          --সাৰিগুলি_বাম_on_page;
        }
    }

  যদি (সাৰিগুলি_বাম_on_page == 0)
    বাবে (j = 1, p = column_vector; j <= columns; ++j, ++p)
      যদি (p->status == OPEN)
        p->full_page_printed = true;

  pad_vertically = pv;

  যদি (pad_vertically && extremities)
    pad_down (সাৰিগুলি_বাম_on_page + সাৰিগুলি_per_footer);
  অন্যথা যদি (keep_FF && print_a_FF)
    {
      putchar ('\f');
      print_a_FF = false;
    }

  যদি (ferror (stdout))
    write_error ();

  যদি (last_page_সংখ্যা < ++page_সংখ্যা)
    ঘূৰাই false;		/* Stop printing with LAST_PAGE */

  reset_status ();		/* Change ON_HOLD to OPEN. */

  ঘূৰাই true;			/* More pages to go. */
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

স্থির শূন্য
init_store_cols (শূন্য)
{
  /* Tune this.  */
  পূর্ণাঙ্ক total_সাৰিগুলি, total_সাৰিগুলি_1, chars_per_column_1, chars_if_truncate;
  যদি (ckd_mul (&total_সাৰিগুলি, সাৰিগুলি_per_body, columns)
      || ckd_add (&total_সাৰিগুলি_1, total_সাৰিগুলি, 1)
      || ckd_add (&chars_per_column_1, chars_per_column, 1)
      || ckd_mul (&chars_if_truncate, total_সাৰিগুলি, chars_per_column_1)
      || ckd_mul (&buff_allocated, chars_if_truncate, use_col_separator + 1))
    integer_overflow ();

  মুক্ত (সাৰি_vector);
  /* FIXME: here's where it was allocated.  */
  সাৰি_vector = xnmalloc (total_সাৰিগুলি_1, পরিমাণ *সাৰি_vector);

  মুক্ত (end_vector);
  end_vector = xnmalloc (total_সাৰিগুলি, পরিমাণ *end_vector);

  মুক্ত (buff);
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

স্থির শূন্য
store_columns (শূন্য)
{
  পূর্ণাঙ্ক i, j;
  unsigned পূর্ণাঙ্ক সাৰি = 0;
  unsigned পূর্ণাঙ্ক buff_start;
  পূর্ণাঙ্ক last_col;		/* The rightmost column which will be saved in buff */
  COLUMN *p;

  buff_current = 0;
  buff_start = 0;

  যদি (balance_columns)
    last_col = columns;
  অন্যথা
    last_col = columns - 1;

  বাবে (i = 1, p = column_vector; i <= last_col; ++i, ++p)
    p->সাৰিগুলি_stored = 0;

  বাবে (i = 1, p = column_vector; i <= last_col && files_ready_to_read;
       ++i, ++p)
    {
      p->current_সাৰি = সাৰি;
      বাবে (j = সাৰিগুলি_per_body; j && files_ready_to_read; --j)

        যদি (p->status == OPEN)	/* Redundant.  Clean up. */
          {
            প্ৰবেশ_position = 0;

            যদি (!read_সাৰি (p))
              read_rest_of_সাৰি (p);

            যদি (p->status == OPEN
                || buff_start != buff_current)
              {
                ++p->সাৰিগুলি_stored;
                সাৰি_vector[সাৰি] = buff_start;
                end_vector[সাৰি++] = প্ৰবেশ_position;
                buff_start = buff_current;
              }
          }
    }

  /* Keep track of the location of the last char in buff. */
  সাৰি_vector[সাৰি] = buff_start;

  যদি (balance_columns)
    balance (সাৰি);
}

স্থির শূন্য
balance (পূর্ণাঙ্ক total_stored)
{
  COLUMN *p;
  পূর্ণাঙ্ক i, সাৰিগুলি;
  পূর্ণাঙ্ক first_সাৰি = 0;

  বাবে (i = 1, p = column_vector; i <= columns; ++i, ++p)
    {
      সাৰিগুলি = total_stored / columns;
      যদি (i <= total_stored % columns)
        ++সাৰিগুলি;

      p->সাৰিগুলি_stored = সাৰিগুলি;
      p->current_সাৰি = first_সাৰি;

      first_সাৰি += সাৰিগুলি;
    }
}

/* Store a character in the buffer. */

স্থির শূন্য
store_char (ৱর্ণ c)
{
  যদি (buff_current >= buff_allocated)
    {
      /* May be too generous. */
      buff = xpalloc (buff, &buff_allocated, 1, -1, পরিমাণ *buff);
    }
  buff[buff_current++] = c;
}

স্থির শূন্য
add_সাৰি_সংখ্যা (COLUMN *p)
{
  পূর্ণাঙ্ক i;
  ৱর্ণ *s;
  পূর্ণাঙ্ক অঙ্ক_width;

  /* Cutting off the higher-order digits is more informative than
     lower-order cut off. */
  অঙ্ক_width = sprintf (সংখ্যা_buff, "%*d", chars_per_সংখ্যা, সাৰি_সংখ্যা);
  সাৰি_সংখ্যা++;
  s = সংখ্যা_buff + (অঙ্ক_width - chars_per_সংখ্যা);
  বাবে (i = chars_per_সংখ্যা; i > 0; i--)
    (p->char_func) (*s++);

  যদি (columns > 1)
    {
      /* Tabification is assumed for multiple columns, also for n-separators,
         but 'default n-separator = TAB' hasn't been given priority over
         equal column_width also specified by POSIX. */
      যদি (সংখ্যা_separator == '\t')
        {
          i = সংখ্যা_width - chars_per_সংখ্যা;
          যেতিয়ালৈ (i-- > 0)
            (p->char_func) (' ');
        }
      অন্যথা
        (p->char_func) (সংখ্যা_separator);
    }
  অন্যথা
    /* To comply with POSIX, we avoid any expansion of default TAB
       separator with a single column output. No column_width requirement
       has to be considered. */
    {
      (p->char_func) (সংখ্যা_separator);
      যদি (সংখ্যা_separator == '\t')
        output_position = POS_AFTER_TAB (chars_per_output_tab,
                          output_position);
    }

  যদি (truncate_সাৰিগুলি && !parallel_files)
    প্ৰবেশ_position += সংখ্যা_width;
}

/* Print (or store) padding until the current horizontal position
   is position. */

স্থির শূন্য
pad_across_to (পূর্ণাঙ্ক position)
{
  পূর্ণাঙ্ক h = output_position;

  যদি (tabify_output)
    spaces_not_printed = position - output_position;
  অন্যথা
    {
      যেতিয়ালৈ (++h <= position)
        putchar (' ');
      output_position = position;
    }
}

/* Pad to the bottom of the page.

   If the user has requested a formfeed, use one.
   Otherwise, use newlines. */

স্থির শূন্য
pad_down (unsigned পূর্ণাঙ্ক সাৰিগুলি)
{
  যদি (use_form_feed)
    putchar ('\f');
  অন্যথা
    বাবে (unsigned পূর্ণাঙ্ক i = সাৰিগুলি; i; --i)
      putchar ('\n');
}

/* Read the rest of the line.

   Read from the current column's file until an end of line is
   hit.  Used when we've truncated a line and we no longer need
   to print or store its characters. */

স্থির শূন্য
read_rest_of_সাৰি (COLUMN *p)
{
  পূর্ণাঙ্ক c;
  FILE *f = p->fp;

  যেতিয়ালৈ ((c = getc (f)) != '\n')
    {
      যদি (c == '\f')
        {
          যদি ((c = getc (f)) != '\n')
            ungetc (c, f);
          যদি (keep_FF)
            print_a_FF = true;
          hold_file (p);
          ভঙ্গ;
        }
      অন্যথা যদি (c == EOF)
        {
          close_file (p);
          ভঙ্গ;
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

স্থির শূন্য
skip_read (COLUMN *p, পূর্ণাঙ্ক column_সংখ্যা)
{
  পূর্ণাঙ্ক c;
  FILE *f = p->fp;
  পূর্ণাঙ্ক i;
  bool single_ff = false;
  COLUMN *q;

  /* Read 1st character in a line or any character succeeding a FF */
  যদি ((c = getc (f)) == '\f' && p->full_page_printed)
    /* A FF-coincidence with a previous full_page_printed.
       To avoid an additional empty page, eliminate the FF */
    যদি ((c = getc (f)) == '\n')
      c = getc (f);

  p->full_page_printed = false;

  /* 1st character a FF means a single FF without any printable
     characters. Don't count it as a line with -n option. */
  যদি (c == '\f')
    single_ff = true;

  /* Preparing for a FF-coincidence: Maybe we finish that page
     without a FF found */
  যদি (last_সাৰি)
    p->full_page_printed = true;

  যেতিয়ালৈ (c != '\n')
    {
      যদি (c == '\f')
        {
          /* No FF-coincidence possible,
             no catching up of a FF-coincidence with next page */
          যদি (last_সাৰি)
            {
              যদি (!parallel_files)
                বাবে (q = column_vector, i = columns; i; ++q, --i)
                  q->full_page_printed = false;
              অন্যথা
                p->full_page_printed = false;
            }

          যদি ((c = getc (f)) != '\n')
            ungetc (c, f);
          hold_file (p);
          ভঙ্গ;
        }
      অন্যথা যদি (c == EOF)
        {
          close_file (p);
          ভঙ্গ;
        }
      c = getc (f);
    }

  যদি (skip_গুনতি)
    যদি ((!parallel_files || column_সংখ্যা == 1) && !single_ff)
      ++সাৰি_গুনতি;
}

/* If we're tabifying output,

   When print_char encounters white space it keeps track
   of our desired horizontal position and delays printing
   until this function is called. */

স্থির শূন্য
print_white_space (শূন্য)
{
  পূর্ণাঙ্ক h_new;
  পূর্ণাঙ্ক h_old = output_position;
  পূর্ণাঙ্ক goal = h_old + spaces_not_printed;

  যেতিয়ালৈ (goal - h_old > 1
         && (h_new = POS_AFTER_TAB (chars_per_output_tab, h_old)) <= goal)
    {
      putchar (output_tab_char);
      h_old = h_new;
    }
  যেতিয়ালৈ (++h_old <= goal)
    putchar (' ');

  output_position = goal;
  spaces_not_printed = 0;
}

/* Print column separators.

   We keep a count until we know that we'll be printing a line,
   then print_sep_string() is called. */

স্থির শূন্য
print_sep_string (শূন্য)
{
  ৱর্ণ ধ্রুৱ *s = col_sep_string;
  পূর্ণাঙ্ক l = col_sep_length;

  যদি (separators_not_printed <= 0)
    {
      /* We'll be starting a line with chars_per_margin, anything else? */
      যদি (spaces_not_printed > 0)
        print_white_space ();
    }
  অন্যথা
    {
      বাবে (; separators_not_printed > 0; --separators_not_printed)
        {
          যেতিয়ালৈ (l-- > 0)
            {
              /* 3 types of sep_strings: spaces only, spaces and chars,
              chars only */
              যদি (*s == ' ')
                {
                  /* We're tabifying output; consecutive spaces in
                  sep_string may have to be converted to tabs */
                  s++;
                  ++spaces_not_printed;
                }
              অন্যথা
                {
                  যদি (spaces_not_printed > 0)
                    print_white_space ();
                  putchar (*s++);
                  ++output_position;
                }
            }
          /* sep_string ends with some spaces */
          যদি (spaces_not_printed > 0)
            print_white_space ();
        }
    }
}

/* Print (or store, depending on p->char_func) a clump of N
   characters. */

স্থির শূন্য
print_clump (COLUMN *p, পূর্ণাঙ্ক n, ৱর্ণ *clump)
{
  যেতিয়ালৈ (n--)
    (p->char_func) (*clump++);

  যদি (ferror (stdout))
    write_error ();
}

/* Print a character.

   Update the following comment: process-char hasn't been used any
   longer.
   If we're tabifying, all tabs have been converted to spaces by
   process_char().  Keep a count of consecutive spaces, and when
   a nonspace is encountered, call print_white_space() to print the
   required number of tabs and spaces. */

স্থির শূন্য
print_char (ৱর্ণ c)
{
  যদি (tabify_output)
    {
      যদি (c == ' ')
        {
          ++spaces_not_printed;
          ঘূৰাই;
        }
      অন্যথা যদি (spaces_not_printed > 0)
        print_white_space ();

      /* Nonprintables are assumed to have width 0, except '\b'. */
      যদি (! isprint (to_uchar (c)))
        {
          যদি (c == '\b')
            --output_position;
        }
      অন্যথা
        ++output_position;
    }
  putchar (c);
}

/* Skip to page PAGE before printing.
   PAGE may be larger than total number of pages. */

স্থির bool
skip_to_page (uintmax_t page)
{
  বাবে (uintmax_t n = 1; n < page; ++n)
    {
      COLUMN *p;
      পূর্ণাঙ্ক j;

      বাবে (পূর্ণাঙ্ক i = 1; i < সাৰিগুলি_per_body; ++i)
        {
          বাবে (j = 1, p = column_vector; j <= columns; ++j, ++p)
            যদি (p->status == OPEN)
              skip_read (p, j);
        }
      last_সাৰি = true;
      বাবে (j = 1, p = column_vector; j <= columns; ++j, ++p)
        যদি (p->status == OPEN)
          skip_read (p, j);

      যদি (storing_columns)	/* change FF_FOUND to ON_HOLD */
        বাবে (j = 1, p = column_vector; j <= columns; ++j, ++p)
          যদি (p->status != CLOSED)
            p->status = ON_HOLD;

      reset_status ();
      last_সাৰি = false;

      যদি (files_ready_to_read < 1)
        {
          /* It's very helpful, normally the total number of pages is
             not known in advance.  */
          error (0, 0,
                 _("starting page number %ju exceeds page count %ju"),
                 page, n);
          ভঙ্গ;
        }
    }
  ঘূৰাই files_ready_to_read > 0;
}

/* Print a header.

   Formfeeds are assumed to use up two lines at the beginning of
   the page. */

স্থির শূন্য
print_header (শূন্য)
{
  ৱর্ণ page_text[256 + INT_STRLEN_BOUND (page_সংখ্যা)];
  পূর্ণাঙ্ক available_width;
  পূর্ণাঙ্ক lhs_spaces;
  পূর্ণাঙ্ক rhs_spaces;

  output_position = 0;
  pad_across_to (chars_per_margin);
  print_white_space ();

  যদি (page_সংখ্যা == 0)
    error (EXIT_FAILURE, 0, _("page number overflow"));

  /* The translator must ensure that formatting the translation of
     "Page %ju" does not generate more than (sizeof page_text - 1)
     bytes.  */
  sprintf (page_text, _("Page %ju"), page_সংখ্যা);
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

স্থির bool
read_সাৰি (COLUMN *p)
{
  পূর্ণাঙ্ক c;
  পূর্ণাঙ্ক chars IF_LINT (= 0);
  পূর্ণাঙ্ক last_প্ৰবেশ_position;
  পূর্ণাঙ্ক j, k;
  COLUMN *q;

  /* read 1st character in each line or any character succeeding a FF: */
  c = getc (p->fp);

  last_প্ৰবেশ_position = প্ৰবেশ_position;

  যদি (c == '\f' && p->full_page_printed)
    যদি ((c = getc (p->fp)) == '\n')
      c = getc (p->fp);
  p->full_page_printed = false;

  ৱিকল্প (c)
    {
    পক্ষ '\f':
      যদি ((c = getc (p->fp)) != '\n')
        ungetc (c, p->fp);
      FF_only = true;
      যদি (print_a_header && !storing_columns)
        {
          pad_vertically = true;
          print_header ();
        }
      অন্যথা যদি (keep_FF)
        print_a_FF = true;
      hold_file (p);
      ঘূৰাই true;
    পক্ষ EOF:
      close_file (p);
      ঘূৰাই true;
    পক্ষ '\n':
      ভঙ্গ;
    স্ৱভাৱ:
      chars = char_to_clump (c);
    }

  যদি (truncate_সাৰিগুলি && প্ৰবেশ_position > chars_per_column)
    {
      প্ৰবেশ_position = last_প্ৰবেশ_position;
      ঘূৰাই false;
    }

  যদি (p->char_func != store_char)
    {
      pad_vertically = true;

      যদি (print_a_header && !storing_columns)
        print_header ();

      যদি (parallel_files && align_empty_cols)
        {
          /* We have to align empty columns at the beginning of a line. */
          k = separators_not_printed;
          separators_not_printed = 0;
          বাবে (j = 1, q = column_vector; j <= k; ++j, ++q)
            {
              align_column (q);
              separators_not_printed += 1;
            }
          padding_not_printed = p->start_position;
          যদি (truncate_সাৰিগুলি)
            spaces_not_printed = chars_per_column;
          অন্যথা
            spaces_not_printed = 0;
          align_empty_cols = false;
        }

      যদি (col_sep_length < padding_not_printed)
        {
          pad_across_to (padding_not_printed - col_sep_length);
          padding_not_printed = ANYWHERE;
        }

      যদি (use_col_separator)
        print_sep_string ();
    }

  যদি (p->numbered)
    add_সাৰি_সংখ্যা (p);

  empty_সাৰি = false;
  যদি (c == '\n')
    ঘূৰাই true;

  print_clump (p, chars, clump_buff);

  যেতিয়ালৈ (true)
    {
      c = getc (p->fp);

      ৱিকল্প (c)
        {
        পক্ষ '\n':
          ঘূৰাই true;
        পক্ষ '\f':
          যদি ((c = getc (p->fp)) != '\n')
            ungetc (c, p->fp);
          যদি (keep_FF)
            print_a_FF = true;
          hold_file (p);
          ঘূৰাই true;
        পক্ষ EOF:
          close_file (p);
          ঘূৰাই true;
        }

      last_প্ৰবেশ_position = প্ৰবেশ_position;
      chars = char_to_clump (c);
      যদি (truncate_সাৰিগুলি && প্ৰবেশ_position > chars_per_column)
        {
          প্ৰবেশ_position = last_প্ৰবেশ_position;
          ঘূৰাই false;
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

স্থির bool
print_stored (COLUMN *p)
{
  COLUMN *q;

  পূর্ণাঙ্ক সাৰি = p->current_সাৰি++;
  ৱর্ণ *first = &buff[সাৰি_vector[সাৰি]];
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
  ৱর্ণ *last = &buff[সাৰি_vector[সাৰি + 1]];

  pad_vertically = true;

  যদি (print_a_header)
    print_header ();

  যদি (p->status == FF_FOUND)
    {
      পূর্ণাঙ্ক i;
      বাবে (i = 1, q = column_vector; i <= columns; ++i, ++q)
        q->status = ON_HOLD;
      যদি (column_vector->সাৰিগুলি_to_print <= 0)
        {
          যদি (!extremities)
            pad_vertically = false;
          ঘূৰাই true;		/* print a header only */
        }
    }

  যদি (col_sep_length < padding_not_printed)
    {
      pad_across_to (padding_not_printed - col_sep_length);
      padding_not_printed = ANYWHERE;
    }

  যদি (use_col_separator)
    print_sep_string ();

  যেতিয়ালৈ (first != last)
    print_char (*first++);

  যদি (spaces_not_printed == 0)
    {
      output_position = p->start_position + end_vector[সাৰি];
      যদি (p->start_position - col_sep_length == chars_per_margin)
        output_position -= col_sep_length;
    }

  ঘূৰাই true;
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

স্থির পূর্ণাঙ্ক
char_to_clump (ৱর্ণ c)
{
  unsigned ৱর্ণ uc = c;
  ৱর্ণ *s = clump_buff;
  ৱর্ণ esc_buff[4];
  পূর্ণাঙ্ক width;
  পূর্ণাঙ্ক chars;
  পূর্ণাঙ্ক chars_per_c = 8;

  যদি (c == প্ৰবেশ_tab_char)
    chars_per_c = chars_per_প্ৰবেশ_tab;

  যদি (c == প্ৰবেশ_tab_char || c == '\t')
    {
      width = TAB_WIDTH (chars_per_c, প্ৰবেশ_position);

      যদি (untabify_প্ৰবেশ)
        {
          বাবে (পূর্ণাঙ্ক i = width; i; --i)
            *s++ = ' ';
          chars = width;
        }
      অন্যথা
        {
          *s = c;
          chars = 1;
        }

    }
  অন্যথা যদি (! isprint (uc))
    {
      যদি (use_esc_sequence)
        {
          width = 4;
          chars = 4;
          *s++ = '\\';
          sprintf (esc_buff, "%03o", uc);
          বাবে (পূর্ণাঙ্ক i = 0; i <= 2; ++i)
            *s++ = esc_buff[i];
        }
      অন্যথা যদি (use_cntrl_prefix)
        {
          যদি (uc < 0200)
            {
              width = 2;
              chars = 2;
              *s++ = '^';
              *s = c ^ 0100;
            }
          অন্যথা
            {
              width = 4;
              chars = 4;
              *s++ = '\\';
              sprintf (esc_buff, "%03o", uc);
              বাবে (পূর্ণাঙ্ক i = 0; i <= 2; ++i)
                *s++ = esc_buff[i];
            }
        }
      অন্যথা যদি (c == '\b')
        {
          width = -1;
          chars = 1;
          *s = c;
        }
      অন্যথা
        {
          width = 0;
          chars = 1;
          *s = c;
        }
    }
  অন্যথা
    {
      width = 1;
      chars = 1;
      *s = c;
    }

  /* Too many backspaces must put us in position 0 -- never negative.  */
  যদি (width < 0 && প্ৰবেশ_position == 0)
    {
      chars = 0;
      প্ৰবেশ_position = 0;
    }
  অন্যথা যদি (width < 0 && প্ৰবেশ_position <= -width)
    প্ৰবেশ_position = 0;
  অন্যথা
    {
      যদি (ckd_add (&প্ৰবেশ_position, প্ৰবেশ_position, width))
        integer_overflow ();
    }

  ঘূৰাই chars;
}

/* We've just printed some files and need to clean up things before
   looking for more options and printing the next batch of files.

   Free everything we've xmalloc'ed, except 'header'. */

স্থির শূন্য
cleanup (শূন্য)
{
  মুক্ত (সংখ্যা_buff);
  মুক্ত (clump_buff);
  মুক্ত (column_vector);
  মুক্ত (সাৰি_vector);
  মুক্ত (end_vector);
  মুক্ত (buff);
}

/* Complain, print a usage message, and die. */

শূন্য
usage (পূর্ণাঙ্ক status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              program_name);

      fputs (_("\
Paginate or columnate FILE(s) বাবে printing.\n\
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
         Balance সংখ্যা of সাৰিগুলি in the columns on each page\n\
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
  -d, --দ্ৱিগুণ-space\n\
         দ্ৱিগুণ space the output\n\
"));
      oputs (_("\
  -D, --date-format=FORMAT\n\
         use FORMAT বাবে the header date\n\
"));
      oputs (_("\
  -e[CHAR[WIDTH]], --expand-tabs[=CHAR[WIDTH]]\n\
         expand প্ৰবেশ CHARs (TABs) to tab WIDTH (8)\n\
"));
      oputs (_("\
  -F, -f, --form-feed\n\
         use form feeds instead of newlines to separate pages\n\
         (by a 3-সাৰি page header with -F or a 5-সাৰি header\n\
         and trailer without -F)\n\
"));
      oputs (_("\
  -h, --header=HEADER\n\
         use a centered HEADER instead of filename in page header,\n\
         -h \"\" prints a blank সাৰি, don't use -h\"\"\n\
"));
      oputs (_("\
  -i[CHAR[WIDTH]], --output-tabs[=CHAR[WIDTH]]\n\
         replace spaces with CHARs (TABs) to tab WIDTH (8)\n\
"));
      oputs (_("\
  -J, --join-সাৰিগুলি\n\
         merge full সাৰিগুলি, turns off -W সাৰি truncation,\n\
         no column alignment, --sep-string[=STRING] sets separators\n\
"));
      oputs (_("\
  -l, --length=PAGE_LENGTH\n\
         set the page length to PAGE_LENGTH (66) সাৰিগুলি\n\
         (স্ৱভাৱ সংখ্যা of সাৰিগুলি of text 56, and with -F 63).\n\
         implies -t যদি PAGE_LENGTH <= 10\n\
"));
      oputs (_("\
  -m, --merge\n\
         print all files in parallel, one in each column,\n\
         truncate সাৰিগুলি, but join সাৰিগুলি of full length with -J\n\
"));
      oputs (_("\
  -n[SEP[DIGITS]], --সংখ্যা-সাৰিগুলি[=SEP[DIGITS]]\n\
         সংখ্যা সাৰিগুলি, use DIGITS (5) digits, then SEP (TAB),\n\
         স্ৱভাৱ counting starts with 1st সাৰি of প্ৰবেশ file\n\
"));
      oputs (_("\
  -N, --first-সাৰি-সংখ্যা=সংখ্যা_2\n\
         start counting with সংখ্যা_2 at 1st সাৰি of first\n\
         page printed (see +FIRST_PAGE)\n\
"));
      oputs (_("\
  -o, --indent=MARGIN\n\
         offset each সাৰি with MARGIN (zero) spaces, কৰা not\n\
         affect -w or -W, MARGIN will be added to PAGE_WIDTH\n\
"));
      oputs (_("\
  -r, --no-file-warnings\n\
         omit warning when a file cannot be opened\n\
"));
      oputs (_("\
  -s[CHAR], --separator[=CHAR]\n\
         separate columns by a single character, স্ৱভাৱ বাবে CHAR\n\
         is the <TAB> character without -w and \'no ৱর্ণ\' with -w.\n\
         -s[CHAR] turns off সাৰি truncation of all 3 column\n\
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
         omit page headers and trailers; implied যদি PAGE_LENGTH <= 10\n\
"));
      oputs (_("\
  -T, --omit-pagination\n\
         omit page headers and trailers,\n\
         eliminate any pagination by form feeds set in প্ৰবেশ files\n\
"));
      oputs (_("\
  -v, --show-nonprinting\n\
         use octal backslash notation\n\
"));
      oputs (_("\
  -w, --width=PAGE_WIDTH\n\
         set page width to PAGE_WIDTH (72) characters বাবে\n\
         multiple text-column output only, -s[ৱর্ণ] turns off (72)\n\
"));
      oputs (_("\
  -W, --page-width=PAGE_WIDTH\n\
         set page width to PAGE_WIDTH (72) characters always,\n\
         truncate সাৰিগুলি, except -J option is set,\n\
         no interference with -S or -s\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}
