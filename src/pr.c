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

#ضمن <config.h>

#ضمن <getopt.h>
#ضمن <sys/types.h>
#ضمن "system.h"
#ضمن "c-ctype.h"
#ضمن "fadvise.h"
#ضمن "hard-locale.h"
#ضمن "mbswidth.h"
#ضمن "quote.h"
#ضمن "stat-time.h"
#ضمن "stdio--.h"
#ضمن "strftime.h"
#ضمن "xstrtol.h"
#ضمن "xstrtol-error.h"
#ضمن "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "pr"

#تعریف_کرو AUTHORS \
  proper_name ("Pete TerMaat"), \
  proper_name ("Roland Huebner")

/* Used with start_position in the struct COLUMN described below.
   If start_position == ANYWHERE, we aren't truncating columns and
   can begin printing a column anywhere.  Otherwise we must pad to
   the horizontal position start_position. */
#تعریف_کرو ANYWHERE	0

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

بنية کالم_3;
بنية کالم_3
  {
    FILE *fp;			/* Input stream for this column. */
    حرف ثابت *name;		/* File name. */
    تعداد
      {
        OPEN,
        FF_لبھیا,		/* used with -b option, set with \f, changed
                                   to ON_HOLD after print_header */
        ON_HOLD,		/* Hit a form feed. */
        CLOSED
      }
    حالت;			/* Status of the file pointer. */

    /* Func to print lines in this col. */
    bool (*print_func) (بنية کالم_3 *);

    /* Func to print/store chars in this col. */
    فراغ (*char_func) (حرف);

    صحيح current_سطر;		/* Index of current place in line_vector. */
    صحيح سطریں_stored;		/* Number of lines stored in buff. */
    صحيح سطریں_منزل_print;		/* No. lines stored or space left on page. */
    صحيح شروع_position;		/* Horizontal position of first char. */
    bool numbered;
    bool بھریا_ورقہ_printed;	/* True means printed without a FF found. */

    /* p->full_page_printed  controls a special case of "FF set by hand":
       True means a full page has been printed without FF found. To avoid an
       additional empty page we have to ignore a FF immediately following in
       the next line. */
  };

تعريفنوع بنية کالم_3 کالم_3;

ساكن صحيح char_منزل_clump (حرف c);
ساكن bool read_سطر (کالم_3 *p);
ساكن bool print_ورقہ (فراغ);
ساكن bool print_stored (کالم_3 *p);
ساكن bool open_file (حرف *name, کالم_3 *p);
ساكن bool skip_منزل_ورقہ (uintmax_t ورقہ);
ساكن فراغ print_سرخی (فراغ);
ساكن فراغ pad_across_منزل (صحيح position);
ساكن فراغ جوڑو_سطر_نمبر (کالم_3 *p);
ساكن صحيح getoptnum (حرف ثابت *n_str, صحيح min, حرف ثابت *errfmt);
ساكن فراغ getoptarg (حرف *arg, حرف switch_char, حرف *character,
                       صحيح *نمبر);
ساكن فراغ print_files (صحيح نمبر_دا_files, حرف **av);
ساكن فراغ init_parameters (صحيح نمبر_دا_files);
ساكن فراغ init_سرخی (حرف ثابت *filename, صحيح desc);
ساكن bool init_fps (صحيح نمبر_دا_files, حرف **av);
ساكن فراغ init_funcs (فراغ);
ساكن فراغ init_store_خانے_گنتی (فراغ);
ساكن فراغ store_columns (فراغ);
ساكن فراغ balance (صحيح کل_stored);
ساكن فراغ store_char (حرف c);
ساكن فراغ pad_تھلے (بنانشانی صحيح سطریں);
ساكن فراغ read_rest_دا_سطر (کالم_3 *p);
ساكن فراغ skip_read (کالم_3 *p, صحيح کالم_نمبر);
ساكن فراغ print_char (حرف c);
ساكن فراغ cleanup (فراغ);
ساكن فراغ print_sep_string (فراغ);
ساكن فراغ separator_string (حرف ثابت *optarg_S);

/* All of the columns to print.  */
ساكن کالم_3 *کالم_سمتیہ;

/* When printing a single file in multiple downward columns,
   we store the leftmost columns contiguously in buff.
   To print a line from buff, get the index of the first character
   from line_vector[i], and print up to line_vector[i + 1]. */
ساكن حرف *buff;

/* Index of the position in buff where the next character
   will be stored. */
ساكن بنانشانی صحيح buff_current;

/* The number of characters in buff.
   Used for allocation of buff and to detect overflow of buff. */
ساكن idx_t buff_allocated;

/* Array of indices into buff.
   Each entry is an index of the first character of a line.
   This is used when storing lines to facilitate shuffling when
   we do column balancing on the last page. */
ساكن صحيح *سطر_سمتیہ;

/* Array of horizontal positions.
   For each line in line_vector, end_vector[line] is the horizontal
   position we are in after printing that line.  We keep track of this
   so that we know how much we need to pad to prepare for the next
   column. */
ساكن صحيح *سرا_سمتیہ;

/* (-m) True means we're printing multiple files in parallel. */
ساكن bool parallel_files = false;

/* (-m) True means a line starts with some empty columns (some files
   already CLOSED or ON_HOLD) which we have to align. */
ساكن bool align_خالی_تھاواں_خانے_گنتی;

/* (-m) True means we have not yet found any printable column in a line.
   align_empty_cols = true  has to be maintained. */
ساكن bool خالی_تھاواں_سطر;

/* (-m) False means printable column output precedes a form feed found.
   Column alignment is done only once. No additional action with that form
   feed.
   True means we found only a form feed in a column. Maybe we have to do
   some column alignment with that form feed. */
ساكن bool FF_only;

/* (-[0-9]+) True means we're given an option explicitly specifying
   number of columns.  Used to detect when this option is used with -m
   and when translating old options to new/long options. */
ساكن bool explicit_columns = false;

/* (-t|-T) False means we aren't printing headers and footers. */
ساكن bool extremities = true;

/* (-t) True means we retain all FF set by hand in input files.
   False is set with -T option. */
ساكن bool keep_FF = false;
ساكن bool print_a_FF = false;

/* True means we need to print a header as soon as we know we've got input
   to print after it. */
ساكن bool print_a_سرخی;

/* (-f) True means use formfeeds instead of newlines to separate pages. */
ساكن bool use_form_feed = false;

/* True means we have read the standard input. */
ساكن bool have_read_stdin = false;

/* True means the -a flag has been given. */
ساكن bool print_across_flag = false;

/* True means we're printing one file in multiple (>1) downward columns. */
ساكن bool storing_columns = true;

/* (-b) True means balance columns on the last page as Sys V does. */
/* That's no longer an independent option. With storing_columns = true
   balance_columns = true is used too (s. function init_parameters).
   We get a consistent formulation with "FF set by hand" in input files. */
ساكن bool balance_columns = false;

/* (-l) Number of lines on a page, including header and footer lines. */
ساكن صحيح سطریں_فی_ورقہ = 66;

/* Number of lines in the header and footer can be reset to 0 using
   the -t flag. */
تعداد { سطریں_فی_سرخی = 5 };
ساكن صحيح سطریں_فی_body;
تعداد { سطریں_فی_footer = 5 };

/* (-w|-W) Width in characters of the page.  Does not include the width of
   the margin. */
ساكن صحيح chars_فی_سطر = 72;

/* (-w|W) True means we truncate lines longer than chars_per_column. */
ساكن bool truncate_سطریں = false;

/* (-J) True means we join lines without any line truncation. -J
   dominates -w option. */
ساكن bool join_سطریں = false;

/* Number of characters in a column.  Based on col_sep_length and
   page width. */
ساكن صحيح chars_فی_کالم;

/* (-e) True means convert tabs to spaces on input. */
ساكن bool untabify_داخلہ = false;

/* (-e) The input tab character. */
ساكن حرف داخلہ_tab_char = '\t';

/* (-e) Tabstops are at chars_per_tab, 2*chars_per_tab, 3*chars_per_tab, ...
   where the leftmost column is 1. */
ساكن صحيح chars_فی_داخلہ_tab = 8;

/* (-i) True means convert spaces to tabs on output. */
ساكن bool tabify_output = false;

/* (-i) The output tab character. */
ساكن حرف output_tab_char = '\t';

/* (-i) The width of the output tab. */
ساكن صحيح chars_فی_output_tab = 8;

/* Keeps track of pending white space.  When we hit a nonspace
   character after some whitespace, we print whitespace, tabbing
   if necessary to get to output_position + spaces_not_printed. */
ساكن صحيح spaces_not_printed;

/* (-o) Number of spaces in the left margin (tabs used when possible). */
ساكن صحيح chars_فی_margin = 0;

/* Position where the next character will fall.
   Leftmost position is 0 + chars_per_margin.
   Rightmost position is chars_per_margin + chars_per_line - 1.
   This is important for converting spaces to tabs on output. */
ساكن صحيح output_position;

/* Horizontal position relative to the current file.
   (output_position depends on where we are on the page;
   input_position depends on where we are in the file.)
   Important for converting tabs to spaces on input. */
ساكن صحيح داخلہ_position;

/* True if there were any failed opens so we can exit with nonzero
   status.  */
ساكن bool failed_opens = false;

/* The number of spaces taken up if we print a tab character with width
   c_ from position h_. */
#تعریف_کرو TAB_چوڑائی(c_, h_) ((c_) - ((h_) % (c_)))

/* The horizontal position we'll be at after printing a tab character
   of width c_ from the position h_. */
#تعریف_کرو POS_AFTER_TAB(c_, h_) ((h_) + TAB_چوڑائی (c_, h_))

/* (-NNN) Number of columns of text to print. */
ساكن صحيح columns = 1;

/* (+NNN:MMM) Page numbers on which to begin and stop printing.
   first_page_number = 0  will be used to check input only. */
ساكن uintmax_t پہلا_ورقہ_نمبر = 0;
ساكن uintmax_t آخری_ورقہ_نمبر = UINTMAX_MAX;

/* Number of files open (not closed, not on hold). */
ساكن صحيح files_ready_منزل_read = 0;

/* Current page number.  Displayed in header. */
ساكن uintmax_t ورقہ_نمبر;

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
ساكن صحيح سطر_نمبر;

/* (-n) True means lines should be preceded by numbers. */
ساكن bool numbered_سطریں = false;

/* (-n) Character which follows each line number. */
ساكن حرف نمبر_separator = '\t';

/* (-n) line counting starts with 1st line of input file (not with 1st
   line of 1st page printed). */
ساكن صحيح سطر_گنتی = 1;

/* (-n) True means counting of skipped lines starts with 1st line of
   input file. False means -N option is used in addition, counting of
   skipped lines not required. */
ساكن bool skip_گنتی = true;

/* (-N) Counting starts with start_line_number = NUMBER at 1st line of
   first page printed, usually not 1st page of input file. */
ساكن صحيح شروع_سطر_عدد = 1;

/* (-n) Width in characters of a line number. */
ساكن صحيح chars_فی_نمبر = 5;

/* Used when widening the first column to accommodate numbers -- only
   needed when printing files in parallel.  Includes width of both the
   number and the number_separator. */
ساكن صحيح نمبر_چوڑائی;

/* Buffer sprintf uses to format a line number. */
ساكن حرف *نمبر_buff;

/* (-v) True means nonprintable characters are printed as escape sequences.
   control-g becomes \007. */
ساكن bool use_esc_sequence = false;

/* (-c) True means nonprintable characters are printed as control prefixes.
   control-g becomes ^G. */
ساكن bool use_cntrl_اگیتر = false;

/* (-d) True means output is double spaced. */
ساكن bool double_space = false;

/* Number of files opened initially in init_files.  Should be 1
   unless we're printing multiple files in parallel. */
ساكن صحيح کل_files = 0;

/* (-r) True means don't complain if we can't open a file. */
ساكن bool ignore_failed_opens = false;

/* (-S) True means we separate columns with a specified string.
   -S option does not affect line truncation nor column alignment. */
ساكن bool use_خانہ_separator = false;

/* String used to separate columns if the -S option has been specified.
   Default without -S but together with one of the column options
   -a|COLUMN|-m is a 'space' and with the -J option a 'tab'. */
ساكن حرف ثابت *خانہ_sep_string = "";
ساكن صحيح خانہ_sep_لمائی = 0;
ساكن حرف *کالم_separator = (حرف *) " ";
ساكن حرف *سطر_separator = (حرف *) "\t";

/* Number of separator characters waiting to be printed as soon as we
   know that we have any input remaining to be printed. */
ساكن صحيح separators_not_printed;

/* Position we need to pad to, as soon as we know that we have input
   remaining to be printed. */
ساكن صحيح padding_not_printed;

/* True means we should pad the end of the page.  Remains false until we
   know we have a page to print. */
ساكن bool pad_vertically;

/* (-h) String of characters used in place of the filename in the header. */
ساكن حرف *custom_سرخی;

/* (-D) Date format for the header.  */
ساكن حرف ثابت *date_format;

/* The local time zone rules, as per the TZ environment variable.  */
ساكن timezone_t localtz;

/* Date and file name for the header.  */
ساكن حرف *date_متن;
ساكن حرف ثابت *file_متن;

/* Output columns available, not counting the date and file name.  */
ساكن صحيح سرخی_چوڑائی_دستیاب;

ساكن حرف *clump_buff;

/* True means we read the line no. lines_per_body in skip_read
   called by skip_to_page. That variable controls the coincidence of a
   "FF set by hand" and "full_page_printed", see above the definition of
   structure COLUMN. */
ساكن bool آخری_سطر = false;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
تعداد
{
  COLUMNS_OPTION = CHAR_MAX + 1,
  ورقے_OPTION
};

ساكن حرف ثابت short_options[] =
  "-0123456789D:FJN:S::TW:abcde::fh:i::l:mn::o:rs::tvw:";

ساكن بنية option ثابت long_options[] =
{
  {"pages", required_دلیل, عدم, ورقے_OPTION},
  {"columns", required_دلیل, عدم, COLUMNS_OPTION},
  {"across", no_دلیل, عدم, 'a'},
  {"show-control-chars", no_دلیل, عدم, 'c'},
  {"double-space", no_دلیل, عدم, 'd'},
  {"date-format", required_دلیل, عدم, 'D'},
  {"expand-tabs", optional_دلیل, عدم, 'e'},
  {"form-feed", no_دلیل, عدم, 'f'},
  {"header", required_دلیل, عدم, 'h'},
  {"output-tabs", optional_دلیل, عدم, 'i'},
  {"join-lines", no_دلیل, عدم, 'J'},
  {"length", required_دلیل, عدم, 'l'},
  {"merge", no_دلیل, عدم, 'm'},
  {"number-lines", optional_دلیل, عدم, 'n'},
  {"first-line-number", required_دلیل, عدم, 'N'},
  {"indent", required_دلیل, عدم, 'o'},
  {"no-file-warnings", no_دلیل, عدم, 'r'},
  {"separator", optional_دلیل, عدم, 's'},
  {"sep-string", optional_دلیل, عدم, 'S'},
  {"omit-header", no_دلیل, عدم, 't'},
  {"omit-pagination", no_دلیل, عدم, 'T'},
  {"show-nonprinting", no_دلیل, عدم, 'v'},
  {"width", required_دلیل, عدم, 'w'},
  {"page-width", required_دلیل, عدم, 'W'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

ساكن _Noreturn فراغ
integer_حد_توں_ودھ (فراغ)
{
  error (EXIT_FAILURE, 0, _("integer overflow"));
}

/* Return the number of columns that have either an open file or
   stored lines. */

ATTRIBUTE_خالص
ساكن بنانشانی صحيح
خانے_گنتی_ready_منزل_print (فراغ)
{
  کالم_3 *q;
  بنانشانی صحيح i;
  بنانشانی صحيح n;

  n = 0;
  لكل (q = کالم_سمتیہ, i = 0; i < columns; ++q, ++i)
    إذا (q->حالت == OPEN
        || q->حالت == FF_لبھیا	/* With -b: To print a header only */
        || (storing_columns && q->سطریں_stored > 0 && q->سطریں_منزل_print > 0))
      ++n;
  أرجع n;
}

/* Estimate first_ / last_page_number
   using option +FIRST_PAGE:LAST_PAGE */

ساكن bool
پہلا_آخری_ورقہ (صحيح oi, حرف c, حرف ثابت *ورقے)
{
  حرف *p;
  uintmax_t پہلا;
  uintmax_t آخری = UINTMAX_MAX;
  strtol_error err = xstrtoumax (ورقے, &p, 10, &پہلا, "");
  إذا (err != LONGINT_OK && err != LONGINT_INVALID_SUFFIX_CHAR)
    xstrtol_fatal (err, oi, c, long_options, ورقے);

  إذا (p == ورقے || !پہلا)
    أرجع false;

  إذا (*p == ':')
    {
      حرف ثابت *p1 = p + 1;
      err = xstrtoumax (p1, &p, 10, &آخری, "");
      إذا (err != LONGINT_OK)
        xstrtol_fatal (err, oi, c, long_options, ورقے);
      إذا (p1 == p || آخری < پہلا)
        أرجع false;
    }

  إذا (*p)
    أرجع false;

  پہلا_ورقہ_نمبر = پہلا;
  آخری_ورقہ_نمبر = آخری;
  أرجع true;
}

/* Parse column count string S, and if it's valid (1 or larger and
   within range of the type of 'columns') set the global variables
   columns and explicit_columns.  Otherwise, exit with a diagnostic.  */

ساكن فراغ
parse_کالم_گنتی (حرف ثابت *s)
{
  columns = getoptnum (s, 1, _("invalid number of columns"));
  explicit_columns = true;
}

/* Estimate length of col_sep_string with option -S.  */

ساكن فراغ
separator_string (حرف ثابت *optarg_S)
{
  size_t len = لڑی_لمبائی (optarg_S);
  إذا (ودھ_توں_ودھ_عدد < len)
    integer_حد_توں_ودھ ();
  خانہ_sep_لمائی = len;
  خانہ_sep_string = optarg_S;
}

صحيح
مکھ (صحيح argc, حرف **argv)
{
  بنانشانی صحيح n_files;
  bool old_options = false;
  bool old_w = false;
  bool old_s = false;
  حرف **file_ناں;

  /* Accumulate the digits of old-style options like -99.  */
  حرف *کالم_گنتی_string = عدم;
  idx_t n_digits = 0;
  idx_t n_alloc = 0;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  n_files = 0;
  file_ناں = (argc > 1
                ? xnmalloc (argc - 1, حجم (حرف *))
                : عدم);

  طالما (true)
    {
      صحيح oi = -1;
      صحيح c = getopt_long (argc, argv, short_options, long_options, &oi);
      إذا (c == -1)
        اكسر;

      إذا (c_isdigit (c))
        {
          /* Accumulate column-count digits specified via old-style options. */
          إذا (n_digits + 1 >= n_alloc)
            کالم_گنتی_string = xpalloc (کالم_گنتی_string, &n_alloc, 2, -1,
                                           حجم *کالم_گنتی_string);
          کالم_گنتی_string[n_digits++] = c;
          کالم_گنتی_string[n_digits] = '\0';
          استمر;
        }

      n_digits = 0;

      بدل (c)
        {
        حالة 1:			/* Non-option argument. */
          /* long option --page dominates old '+FIRST_PAGE ...'.  */
          إذا (! (پہلا_ورقہ_نمبر == 0
                 && *optarg == '+' && پہلا_آخری_ورقہ (-2, '+', optarg + 1)))
            file_ناں[n_files++] = optarg;
          اكسر;

        حالة ورقے_OPTION:	/* --pages=FIRST_PAGE[:LAST_PAGE] */
          {			/* dominates old opt +... */
            إذا (! optarg)
              error (EXIT_FAILURE, 0,
                     _("'--pages=FIRST_PAGE[:LAST_PAGE]' missing argument"));
            وإلا إذا (! پہلا_آخری_ورقہ (oi, 0, optarg))
              error (EXIT_FAILURE, 0, _("invalid page range %s"),
                     quote (optarg));
            اكسر;
          }

        حالة COLUMNS_OPTION:	/* --columns=COLS */
          {
            parse_کالم_گنتی (optarg);

            /* If there was a prior column count specified via the
               short-named option syntax, e.g., -9, ensure that this
               long-name-specified value overrides it.  */
            حرر (کالم_گنتی_string);
            کالم_گنتی_string = عدم;
            n_alloc = 0;
            اكسر;
          }

        حالة 'a':
          print_across_flag = true;
          storing_columns = false;
          اكسر;
        حالة 'b':
          balance_columns = true;
          اكسر;
        حالة 'c':
          use_cntrl_اگیتر = true;
          اكسر;
        حالة 'd':
          double_space = true;
          اكسر;
        حالة 'D':
          date_format = optarg;
          اكسر;
        حالة 'e':
          إذا (optarg)
            getoptarg (optarg, 'e', &داخلہ_tab_char,
                       &chars_فی_داخلہ_tab);
          /* Could check tab width > 0. */
          untabify_داخلہ = true;
          اكسر;
        حالة 'f':
        حالة 'F':
          use_form_feed = true;
          اكسر;
        حالة 'h':
          custom_سرخی = optarg;
          اكسر;
        حالة 'i':
          إذا (optarg)
            getoptarg (optarg, 'i', &output_tab_char,
                       &chars_فی_output_tab);
          /* Could check tab width > 0. */
          tabify_output = true;
          اكسر;
        حالة 'J':
          join_سطریں = true;
          اكسر;
        حالة 'l':
          سطریں_فی_ورقہ
            = getoptnum (optarg, 1,
                         _("'-l PAGE_LENGTH' invalid number of lines"));
          اكسر;
        حالة 'm':
          parallel_files = true;
          storing_columns = false;
          اكسر;
        حالة 'n':
          numbered_سطریں = true;
          إذا (optarg)
            getoptarg (optarg, 'n', &نمبر_separator,
                       &chars_فی_نمبر);
          اكسر;
        حالة 'N':
          skip_گنتی = false;
          شروع_سطر_عدد
            = getoptnum (optarg, INT_MIN,
                         _("'-N NUMBER' invalid starting line number"));
          اكسر;
        حالة 'o':
          chars_فی_margin = getoptnum (optarg, 0,
                                        _("'-o MARGIN' invalid line offset"));
          اكسر;
        حالة 'r':
          ignore_failed_opens = true;
          اكسر;
        حالة 's':
          old_options = true;
          old_s = true;
          إذا (!use_خانہ_separator && optarg)
            separator_string (optarg);
          اكسر;
        حالة 'S':
          old_s = false;
          /* Reset an additional input of -s, -S dominates -s */
          خانہ_sep_string = "";
          خانہ_sep_لمائی = 0;
          use_خانہ_separator = true;
          إذا (optarg)
            separator_string (optarg);
          اكسر;
        حالة 't':
          extremities = false;
          keep_FF = true;
          اكسر;
        حالة 'T':
          extremities = false;
          keep_FF = false;
          اكسر;
        حالة 'v':
          use_esc_sequence = true;
          اكسر;
        حالة 'w':
          old_options = true;
          old_w = true;
          {
            صحيح tmp_cpl
              = getoptnum (optarg, 1,
                           _("'-w PAGE_WIDTH' invalid number of characters"));
            إذا (! truncate_سطریں)
              chars_فی_سطر = tmp_cpl;
          }
          اكسر;
        حالة 'W':
          old_w = false;			/* dominates -w */
          truncate_سطریں = true;
          chars_فی_سطر
            = getoptnum (optarg, 1,
                         _("'-W PAGE_WIDTH' invalid number of characters"));
          اكسر;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);
        افتراضي:
          usage (EXIT_FAILURE);
          اكسر;
        }
    }

  إذا (کالم_گنتی_string)
    {
      parse_کالم_گنتی (کالم_گنتی_string);
      حرر (کالم_گنتی_string);
    }

  إذا (! date_format)
    date_format = (getenv ("POSIXLY_CORRECT") && !hard_locale (LC_ویلا)
                   ? "%b %e %H:%M %Y"
                   : "%Y-%m-%d %H:%M");

  localtz = tzalloc (getenv ("TZ"));

  /* Now we can set a reasonable initial value: */
  إذا (پہلا_ورقہ_نمبر == 0)
    پہلا_ورقہ_نمبر = 1;

  إذا (parallel_files && explicit_columns)
    error (EXIT_FAILURE, 0,
           _("cannot specify number of columns when printing in parallel"));

  إذا (parallel_files && print_across_flag)
    error (EXIT_FAILURE, 0,
           _("cannot specify both printing across and printing in parallel"));

/* Translate some old short options to new/long options.
   To meet downward compatibility with other UNIX pr utilities
   and some POSIX specifications. */

  إذا (old_options)
    {
      إذا (old_w)
        {
          إذا (parallel_files || explicit_columns)
            {
              /* activate -W */
              truncate_سطریں = true;
              إذا (old_s)
                /* adapt HP-UX and SunOS: -s = no separator;
                   activate -S */
                use_خانہ_separator = true;
            }
          وإلا
            /* old -w sets width with columns only
               activate -J */
            join_سطریں = true;
        }
      وإلا إذا (!use_خانہ_separator)
        {
          /* No -S option read */
          إذا (old_s && (parallel_files || explicit_columns))
            {
              إذا (!truncate_سطریں)
                {
                  /* old -s (without -w and -W) annuls column alignment,
                  uses fields, activate -J */
                  join_سطریں = true;
                  إذا (خانہ_sep_لمائی > 0)
                    /* activate -S */
                    use_خانہ_separator = true;
                }
              وإلا
                /* with -W */
                /* adapt HP-UX and SunOS: -s = no separator;
                   activate -S */
                use_خانہ_separator = true;
            }
        }
    }

  لكل (; optind < argc; optind++)
    {
      file_ناں[n_files++] = argv[optind];
    }

  إذا (n_files == 0)
    {
      /* No file arguments specified;  read from standard input.  */
      print_files (0, عدم);
    }
  وإلا
    {
      إذا (parallel_files)
        print_files (n_files, file_ناں);
      وإلا
        {
          لكل (بنانشانی صحيح i = 0; i < n_files; i++)
            print_files (1, &file_ناں[i]);
        }
    }

  cleanup ();

  إذا (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, _("standard input"));
  main_exit (failed_opens ? EXIT_FAILURE : EXIT_SUCCESS);
}

/* Parse numeric arguments, ensuring MIN <= number <= INT_MAX.  */

ساكن صحيح
getoptnum (حرف ثابت *n_str, صحيح min, حرف ثابت *err)
{
  أرجع xnumtoimax (n_str, 10, min, ودھ_توں_ودھ_عدد, "", err, 0,
                     min <= 0 ? 0 : XTOINT_MIN_RANGE);
}

/* Parse options of the form -scNNN.

   Example: -nck, where 'n' is the option, c is the optional number
   separator, and k is the optional width of the field used when printing
   a number. */

ساكن فراغ
getoptarg (حرف *arg, حرف switch_char, حرف *character, صحيح *نمبر)
{
  حرف ثابت *عدد = arg;

  إذا (!*عدد)
    {
      error (0, 0, _("'-%c': Invalid argument: %s"), switch_char, quote (عدد));
      usage (EXIT_FAILURE);
    }

  إذا (!c_isdigit (*عدد))
    *character = *عدد++;
  إذا (*عدد)
    {
      لما صحيح tmp_long;
      strtol_error e = xstrtol (عدد, عدم, 10, &tmp_long, "");
      إذا (e == LONGINT_OK)
        {
          إذا (tmp_long <= 0)
            e = LONGINT_INVALID;
          وإلا إذا (ودھ_توں_ودھ_عدد < tmp_long)
            e = LONGINT_حد_توں_ودھ;
        }
      إذا (e != LONGINT_OK)
        {
          error (0, e & LONGINT_حد_توں_ودھ ? EOVERFLOW : 0,
             _("'-%c' extra characters or invalid number in the argument: %s"),
                 switch_char, quote (arg));
          usage (EXIT_FAILURE);
        }
      *نمبر = tmp_long;
    }
}

/* Set parameters related to formatting. */

ساكن فراغ
init_parameters (صحيح نمبر_دا_files)
{
  صحيح chars_ورتیا_by_نمبر = 0;

  سطریں_فی_body = سطریں_فی_ورقہ - سطریں_فی_سرخی - سطریں_فی_footer;
  إذا (سطریں_فی_body <= 0)
    {
      extremities = false;
      keep_FF = true;
    }
  إذا (! extremities)
    سطریں_فی_body = سطریں_فی_ورقہ;

  إذا (double_space)
    سطریں_فی_body = MAX (1, سطریں_فی_body / 2);

  /* If input is stdin, cannot print parallel files.  BSD dumps core
     on this. */
  إذا (نمبر_دا_files == 0)
    parallel_files = false;

  إذا (parallel_files)
    columns = نمبر_دا_files;

  /* One file, multi columns down: -b option is set to get a consistent
     formulation with "FF set by hand" in input files. */
  إذا (storing_columns)
    balance_columns = true;

  /* Tabification is assumed for multiple columns. */
  إذا (columns > 1)
    {
      إذا (!use_خانہ_separator)
        {
          /* Use default separator */
          إذا (join_سطریں)
            خانہ_sep_string = سطر_separator;
          وإلا
            خانہ_sep_string = کالم_separator;

          خانہ_sep_لمائی = 1;
          use_خانہ_separator = true;
        }
      /* It's rather pointless to define a TAB separator with column
         alignment */
      وإلا إذا (!join_سطریں && خانہ_sep_لمائی == 1 && *خانہ_sep_string == '\t')
        خانہ_sep_string = کالم_separator;

      truncate_سطریں = true;
      إذا (! (خانہ_sep_لمائی == 1 && *خانہ_sep_string == '\t'))
        untabify_داخلہ = true;
      tabify_output = true;
    }
  وإلا
    storing_columns = false;

  /* -J dominates -w in any case */
  إذا (join_سطریں)
    truncate_سطریں = false;

  إذا (numbered_سطریں)
    {
      صحيح chars_فی_default_tab = 8;

      سطر_گنتی = شروع_سطر_عدد;

      /* To allow input tab-expansion (-e sensitive) use:
         if (number_separator == input_tab_char)
           number_width = chars_per_number
             + TAB_WIDTH (chars_per_input_tab, chars_per_number);   */

      /* Estimate chars_per_text without any margin and keep it constant. */
      إذا (نمبر_separator == '\t')
        نمبر_چوڑائی = (chars_فی_نمبر
                        + TAB_چوڑائی (chars_فی_default_tab, chars_فی_نمبر));
      وإلا
        نمبر_چوڑائی = chars_فی_نمبر + 1;

      /* The number is part of the column width unless we are
         printing files in parallel. */
      إذا (parallel_files)
        chars_ورتیا_by_نمبر = نمبر_چوڑائی;
    }

  صحيح sep_chars, useful_chars;
  إذا (ckd_mul (&sep_chars, columns - 1, خانہ_sep_لمائی))
    sep_chars = ودھ_توں_ودھ_عدد;
  إذا (ckd_sub (&useful_chars, chars_فی_سطر - chars_ورتیا_by_نمبر,
               sep_chars))
    useful_chars = 0;
  chars_فی_کالم = useful_chars / columns;

  إذا (chars_فی_کالم < 1)
    error (EXIT_FAILURE, 0, _("page width too narrow"));

  إذا (numbered_سطریں)
    {
      حرر (نمبر_buff);
      نمبر_buff = xmalloc (MAX (chars_فی_نمبر,
                                  INT_STRLEN_BOUND (سطر_نمبر)) + 1);
    }

  /* Pick the maximum between the tab width and the width of an
     escape sequence.
     The width of an escape sequence (4) isn't the lower limit any longer.
     We've to use 8 as the lower limit, if we use chars_per_default_tab = 8
     to expand a tab which is not an input_tab-char. */
  حرر (clump_buff);
  clump_buff = xmalloc (MAX (8, chars_فی_داخلہ_tab));
}

/* Open the necessary files,
   maintaining a COLUMN structure for each column.

   With multiple files, each column p has a different p->fp.
   With single files, each column p has the same p->fp.
   Return false if (number_of_files > 0) and no files can be opened,
   true otherwise.

   With each column/file p, p->full_page_printed is initialized,
   see also open_file.  */

ساكن bool
init_fps (صحيح نمبر_دا_files, حرف **av)
{
  کالم_3 *p;

  کل_files = 0;

  حرر (کالم_سمتیہ);
  کالم_سمتیہ = xnmalloc (columns, حجم (کالم_3));

  إذا (parallel_files)
    {
      صحيح files_بایاں = نمبر_دا_files;
      لكل (p = کالم_سمتیہ; files_بایاں--; ++p, ++av)
        {
          إذا (! open_file (*av, p))
            {
              --p;
              --columns;
            }
        }
      إذا (columns == 0)
        أرجع false;
      init_سرخی ("", -1);
    }
  وإلا
    {
      p = کالم_سمتیہ;
      إذا (نمبر_دا_files > 0)
        {
          إذا (! open_file (*av, p))
            أرجع false;
          init_سرخی (*av, fileno (p->fp));
          p->سطریں_stored = 0;
        }
      وإلا
        {
          p->name = _("standard input");
          p->fp = stdin;
          have_read_stdin = true;
          p->حالت = OPEN;
          p->بھریا_ورقہ_printed = false;
          ++کل_files;
          init_سرخی ("", -1);
          p->سطریں_stored = 0;
        }

      حرف ثابت *firstname = p->name;
      FILE *firstfp = p->fp;
      صحيح i;
      لكل (i = columns - 1, ++p; i; --i, ++p)
        {
          p->name = firstname;
          p->fp = firstfp;
          p->حالت = OPEN;
          p->بھریا_ورقہ_printed = false;
          p->سطریں_stored = 0;
        }
    }
  files_ready_منزل_read = کل_files;
  أرجع true;
}

/* Determine print_func and char_func, the functions
   used by each column for printing and/or storing.

   Determine the horizontal position desired when we begin
   printing a column (p->start_position). */

ساكن فراغ
init_funcs (فراغ)
{
  صحيح i, h, h_اگلا;
  کالم_3 *p;

  h = chars_فی_margin;

  إذا (!truncate_سطریں)
    h_اگلا = ANYWHERE;
  وإلا
    {
      /* When numbering lines of parallel files, we enlarge the
         first column to accommodate the number.  Looks better than
         the Sys V approach. */
      إذا (parallel_files && numbered_سطریں)
        h_اگلا = h + chars_فی_کالم + نمبر_چوڑائی;
      وإلا
        h_اگلا = h + chars_فی_کالم;
    }

  /* Enlarge p->start_position of first column to use the same form of
     padding_not_printed with all columns. */
  h = h + خانہ_sep_لمائی;

  /* This loop takes care of all but the rightmost column. */

  لكل (p = کالم_سمتیہ, i = 1; i < columns; ++p, ++i)
    {
      إذا (storing_columns)	/* One file, multi columns down. */
        {
          p->char_func = store_char;
          p->print_func = print_stored;
        }
      وإلا
        /* One file, multi columns across; or parallel files.  */
        {
          p->char_func = print_char;
          p->print_func = read_سطر;
        }

      /* Number only the first column when printing files in
         parallel. */
      p->numbered = numbered_سطریں && (!parallel_files || i == 1);
      p->شروع_position = h;

      /* If we don't truncate lines, all start_positions are
         ANYWHERE, except the first column's start_position when
         using a margin. */

      إذا (!truncate_سطریں)
        {
          h = ANYWHERE;
          h_اگلا = ANYWHERE;
        }
      وإلا
        {
          h = h_اگلا + خانہ_sep_لمائی;
          h_اگلا = h + chars_فی_کالم;
        }
    }

  /* The rightmost column.

     Doesn't need to be stored unless we intend to balance
     columns on the last page. */
  إذا (storing_columns && balance_columns)
    {
      p->char_func = store_char;
      p->print_func = print_stored;
    }
  وإلا
    {
      p->char_func = print_char;
      p->print_func = read_سطر;
    }

  p->numbered = numbered_سطریں && (!parallel_files || i == 1);
  p->شروع_position = h;
}

/* Open a file.  Return true if successful.

   With each file p, p->full_page_printed is initialized,
   see also init_fps. */

ساكن bool
open_file (حرف *name, کالم_3 *p)
{
  إذا (streq (name, "-"))
    {
      p->name = _("standard input");
      p->fp = stdin;
      have_read_stdin = true;
    }
  وإلا
    {
      p->name = name;
      p->fp = fopen (name, "r");
    }
  إذا (p->fp == عدم)
    {
      failed_opens = true;
      إذا (!ignore_failed_opens)
        error (0, errno, "%s", quotef (name));
      أرجع false;
    }
  fadvise (p->fp, FADVISE_SEQUENTIAL);
  p->حالت = OPEN;
  p->بھریا_ورقہ_printed = false;
  ++کل_files;
  أرجع true;
}

/* Close the file in P.

   If we aren't dealing with multiple files in parallel, we change
   the status of all columns in the column list to reflect the close. */

ساكن فراغ
close_file (کالم_3 *p)
{
  کالم_3 *q;
  صحيح i;

  إذا (p->حالت == CLOSED)
    أرجع;

  صحيح err = errno;
  إذا (!ferror (p->fp))
    err = 0;
  إذا (fileno (p->fp) == STDIN_FILENO)
    clearerr (p->fp);
  وإلا إذا (fclose (p->fp) != 0 && !err)
    err = errno;
  إذا (err)
    error (EXIT_FAILURE, err, "%s", quotef (p->name));

  إذا (!parallel_files)
    {
      لكل (q = کالم_سمتیہ, i = columns; i; ++q, --i)
        {
          q->حالت = CLOSED;
          إذا (q->سطریں_stored == 0)
            {
              q->سطریں_منزل_print = 0;
            }
        }
    }
  وإلا
    {
      p->حالت = CLOSED;
      p->سطریں_منزل_print = 0;
    }

  --files_ready_منزل_read;
}

/* Put a file on hold until we start a new page,
   since we've hit a form feed.

   If we aren't dealing with parallel files, we must change the
   status of all columns in the column list. */

ساكن فراغ
hold_file (کالم_3 *p)
{
  کالم_3 *q;
  صحيح i;

  إذا (!parallel_files)
    لكل (q = کالم_سمتیہ, i = columns; i; ++q, --i)
      {
        إذا (storing_columns)
          q->حالت = FF_لبھیا;
        وإلا
          q->حالت = ON_HOLD;
      }
  وإلا
    p->حالت = ON_HOLD;

  p->سطریں_منزل_print = 0;
  --files_ready_منزل_read;
}

/* Undo hold_file -- go through the column list and change any
   ON_HOLD columns to OPEN.  Used at the end of each page. */

ساكن فراغ
reset_حالت (فراغ)
{
  صحيح i = columns;
  کالم_3 *p;

  لكل (p = کالم_سمتیہ; i; --i, ++p)
    إذا (p->حالت == ON_HOLD)
      {
        p->حالت = OPEN;
        files_ready_منزل_read++;
      }

  إذا (storing_columns)
    {
      إذا (کالم_سمتیہ->حالت == CLOSED)
        /* We use the info to output an error message in  skip_to_page. */
        files_ready_منزل_read = 0;
      وإلا
        files_ready_منزل_read = 1;
    }
}

/* Print a single file, or multiple files in parallel.

   Set up the list of columns, opening the necessary files.
   Allocate space for storing columns, if necessary.
   Skip to first_page_number, if user has asked to skip leading pages.
   Determine which functions are appropriate to store/print lines
   in each column.
   Print the file(s). */

ساكن فراغ
print_files (صحيح نمبر_دا_files, حرف **av)
{
  init_parameters (نمبر_دا_files);
  إذا (! init_fps (نمبر_دا_files, av))
    أرجع;
  إذا (storing_columns)
    init_store_خانے_گنتی ();

  إذا (پہلا_ورقہ_نمبر > 1)
    {
      إذا (!skip_منزل_ورقہ (پہلا_ورقہ_نمبر))
        أرجع;
      وإلا
        ورقہ_نمبر = پہلا_ورقہ_نمبر;
    }
  وإلا
    ورقہ_نمبر = 1;

  init_funcs ();

  سطر_نمبر = سطر_گنتی;
  طالما (print_ورقہ ())
    ;
}

/* Initialize header information.
   If DESC is non-negative, it is a file descriptor open to
   FILENAME for reading.  */

ساكن فراغ
init_سرخی (حرف ثابت *filename, صحيح desc)
{
  حرف *buf = عدم;
  بنية stat st;
  بنية timespec t;
  صحيح ns;
  بنية tm tm;

  /* If parallel files or standard input, use current date. */
  إذا (streq (filename, "-"))
    desc = -1;
  إذا (0 <= desc && fstat (desc, &st) == 0)
    t = get_stat_mtime (&st);
  وإلا
    {
      ساكن بنية timespec timespec;
      إذا (! timespec.tv_sec)
        gettime (&timespec);
      t = timespec;
    }

  ns = t.tv_nsec;
  إذا (localtime_rz (localtz, &t.tv_sec, &tm))
    {
      ptrdiff_t len = nstrftime (عدم, MIN (PTRDIFF_MAX, SIZE_MAX),
                                 date_format, &tm, localtz, ns);
      إذا (0 <= len)
        {
          buf = ximalloc (len + 1);
          nstrftime (buf, len + 1, date_format, &tm, localtz, ns);
        }
    }
  إذا (!buf)
    {
      حرف secbuf[INT_BUFSIZE_BOUND (intmax_t)];
      buf = xmalloc (حجم secbuf + MAX (10, INT_BUFSIZE_BOUND (صحيح)));
      sprintf (buf, "%s.%09d", timetostr (t.tv_sec, secbuf), ns);
    }

  حرر (date_متن);
  date_متن = buf;
  file_متن = custom_سرخی ? custom_سرخی : desc < 0 ? "" : filename;
  سرخی_چوڑائی_دستیاب = (chars_فی_سطر
                            - mbswidth (date_متن, 0)
                            - mbswidth (file_متن, 0));
}

/* Set things up for printing a page

   Scan through the columns ...
   Determine which are ready to print
   (i.e., which have lines stored or open files)
   Set p->lines_to_print appropriately
   (to p->lines_stored if we're storing, or lines_per_body
   if we're reading straight from the file)
   Keep track of this total so we know when to stop printing */

ساكن فراغ
init_ورقہ (فراغ)
{
  صحيح j;
  کالم_3 *p;

  إذا (storing_columns)
    {
      store_columns ();
      لكل (j = columns - 1, p = کالم_سمتیہ; j; --j, ++p)
        {
          p->سطریں_منزل_print = p->سطریں_stored;
        }

      /* Last column. */
      إذا (balance_columns)
        {
          p->سطریں_منزل_print = p->سطریں_stored;
        }
      /* Since we're not balancing columns, we don't need to store
         the rightmost column.   Read it straight from the file. */
      وإلا
        {
          إذا (p->حالت == OPEN)
            {
              p->سطریں_منزل_print = سطریں_فی_body;
            }
          وإلا
            p->سطریں_منزل_print = 0;
        }
    }
  وإلا
    لكل (j = columns, p = کالم_سمتیہ; j; --j, ++p)
      إذا (p->حالت == OPEN)
        {
          p->سطریں_منزل_print = سطریں_فی_body;
        }
      وإلا
        p->سطریں_منزل_print = 0;
}

/* Align empty columns and print separators.
   Empty columns will be formed by files with status ON_HOLD or CLOSED
   when printing multiple files in parallel. */

ساكن فراغ
align_کالم (کالم_3 *p)
{
  padding_not_printed = p->شروع_position;
  إذا (خانہ_sep_لمائی < padding_not_printed)
    {
      pad_across_منزل (padding_not_printed - خانہ_sep_لمائی);
      padding_not_printed = ANYWHERE;
    }

  إذا (use_خانہ_separator)
    print_sep_string ();

  إذا (p->numbered)
    جوڑو_سطر_نمبر (p);
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

ساكن bool
print_ورقہ (فراغ)
{
  صحيح j;
  صحيح سطریں_بایاں_on_ورقہ;
  کالم_3 *p;

  /* Used as an accumulator (with | operator) of successive values of
     pad_vertically.  The trick is to set pad_vertically
     to false before each run through the inner loop, then after that
     loop, it tells us whether a line was actually printed (whether a
     newline needs to be output -- or two for double spacing).  But those
     values have to be accumulated (in pv) so we can invoke pad_down
     properly after the outer loop completes. */
  bool pv;

  init_ورقہ ();

  إذا (خانے_گنتی_ready_منزل_print () == 0)
    أرجع false;

  إذا (extremities)
    print_a_سرخی = true;

  /* Don't pad unless we know a page was printed. */
  pad_vertically = false;
  pv = false;

  سطریں_بایاں_on_ورقہ = سطریں_فی_body;
  إذا (double_space)
    سطریں_بایاں_on_ورقہ *= 2;

  طالما (سطریں_بایاں_on_ورقہ > 0 && خانے_گنتی_ready_منزل_print () > 0)
    {
      output_position = 0;
      spaces_not_printed = 0;
      separators_not_printed = 0;
      pad_vertically = false;
      align_خالی_تھاواں_خانے_گنتی = false;
      خالی_تھاواں_سطر = true;

      لكل (j = 1, p = کالم_سمتیہ; j <= columns; ++j, ++p)
        {
          داخلہ_position = 0;
          إذا (p->سطریں_منزل_print > 0 || p->حالت == FF_لبھیا)
            {
              FF_only = false;
              padding_not_printed = p->شروع_position;
              إذا (!(p->print_func) (p))
                read_rest_دا_سطر (p);
              pv |= pad_vertically;

              --p->سطریں_منزل_print;
              إذا (p->سطریں_منزل_print <= 0)
                {
                  إذا (خانے_گنتی_ready_منزل_print () == 0)
                    اكسر;
                }

              /* File p changed its status to ON_HOLD or CLOSED */
              إذا (parallel_files && p->حالت != OPEN)
                {
                  إذا (خالی_تھاواں_سطر)
                    align_خالی_تھاواں_خانے_گنتی = true;
                  وإلا إذا (p->حالت == CLOSED
                           || (p->حالت == ON_HOLD && FF_only))
                    align_کالم (p);
                }
            }
          وإلا إذا (parallel_files)
            {
              /* File status ON_HOLD or CLOSED */
              إذا (خالی_تھاواں_سطر)
                align_خالی_تھاواں_خانے_گنتی = true;
              وإلا
                align_کالم (p);
            }

          /* We need it also with an empty column */
          إذا (use_خانہ_separator)
            ++separators_not_printed;
        }

      إذا (pad_vertically)
        {
          حرف_لکھو ('\n');
          --سطریں_بایاں_on_ورقہ;
        }

      إذا (خانے_گنتی_ready_منزل_print () == 0 && !extremities)
        اكسر;

      إذا (double_space && pv)
        {
          حرف_لکھو ('\n');
          --سطریں_بایاں_on_ورقہ;
        }
    }

  إذا (سطریں_بایاں_on_ورقہ == 0)
    لكل (j = 1, p = کالم_سمتیہ; j <= columns; ++j, ++p)
      إذا (p->حالت == OPEN)
        p->بھریا_ورقہ_printed = true;

  pad_vertically = pv;

  إذا (pad_vertically && extremities)
    pad_تھلے (سطریں_بایاں_on_ورقہ + سطریں_فی_footer);
  وإلا إذا (keep_FF && print_a_FF)
    {
      حرف_لکھو ('\f');
      print_a_FF = false;
    }

  إذا (ferror (stdout))
    write_error ();

  إذا (آخری_ورقہ_نمبر < ++ورقہ_نمبر)
    أرجع false;		/* Stop printing with LAST_PAGE */

  reset_حالت ();		/* Change ON_HOLD to OPEN. */

  أرجع true;			/* More pages to go. */
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

ساكن فراغ
init_store_خانے_گنتی (فراغ)
{
  /* Tune this.  */
  صحيح کل_سطریں, کل_سطریں_1, chars_فی_کالم_1, chars_if_truncate;
  إذا (ckd_mul (&کل_سطریں, سطریں_فی_body, columns)
      || ckd_جوڑو (&کل_سطریں_1, کل_سطریں, 1)
      || ckd_جوڑو (&chars_فی_کالم_1, chars_فی_کالم, 1)
      || ckd_mul (&chars_if_truncate, کل_سطریں, chars_فی_کالم_1)
      || ckd_mul (&buff_allocated, chars_if_truncate, use_خانہ_separator + 1))
    integer_حد_توں_ودھ ();

  حرر (سطر_سمتیہ);
  /* FIXME: here's where it was allocated.  */
  سطر_سمتیہ = xnmalloc (کل_سطریں_1, حجم *سطر_سمتیہ);

  حرر (سرا_سمتیہ);
  سرا_سمتیہ = xnmalloc (کل_سطریں, حجم *سرا_سمتیہ);

  حرر (buff);
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

ساكن فراغ
store_columns (فراغ)
{
  صحيح i, j;
  بنانشانی صحيح سطر = 0;
  بنانشانی صحيح buff_شروع;
  صحيح آخری_خانہ;		/* The rightmost column which will be saved in buff */
  کالم_3 *p;

  buff_current = 0;
  buff_شروع = 0;

  إذا (balance_columns)
    آخری_خانہ = columns;
  وإلا
    آخری_خانہ = columns - 1;

  لكل (i = 1, p = کالم_سمتیہ; i <= آخری_خانہ; ++i, ++p)
    p->سطریں_stored = 0;

  لكل (i = 1, p = کالم_سمتیہ; i <= آخری_خانہ && files_ready_منزل_read;
       ++i, ++p)
    {
      p->current_سطر = سطر;
      لكل (j = سطریں_فی_body; j && files_ready_منزل_read; --j)

        إذا (p->حالت == OPEN)	/* Redundant.  Clean up. */
          {
            داخلہ_position = 0;

            إذا (!read_سطر (p))
              read_rest_دا_سطر (p);

            إذا (p->حالت == OPEN
                || buff_شروع != buff_current)
              {
                ++p->سطریں_stored;
                سطر_سمتیہ[سطر] = buff_شروع;
                سرا_سمتیہ[سطر++] = داخلہ_position;
                buff_شروع = buff_current;
              }
          }
    }

  /* Keep track of the location of the last char in buff. */
  سطر_سمتیہ[سطر] = buff_شروع;

  إذا (balance_columns)
    balance (سطر);
}

ساكن فراغ
balance (صحيح کل_stored)
{
  کالم_3 *p;
  صحيح i, سطریں;
  صحيح پہلا_سطر = 0;

  لكل (i = 1, p = کالم_سمتیہ; i <= columns; ++i, ++p)
    {
      سطریں = کل_stored / columns;
      إذا (i <= کل_stored % columns)
        ++سطریں;

      p->سطریں_stored = سطریں;
      p->current_سطر = پہلا_سطر;

      پہلا_سطر += سطریں;
    }
}

/* Store a character in the buffer. */

ساكن فراغ
store_char (حرف c)
{
  إذا (buff_current >= buff_allocated)
    {
      /* May be too generous. */
      buff = xpalloc (buff, &buff_allocated, 1, -1, حجم *buff);
    }
  buff[buff_current++] = c;
}

ساكن فراغ
جوڑو_سطر_نمبر (کالم_3 *p)
{
  صحيح i;
  حرف *s;
  صحيح عدد_چوڑائی;

  /* Cutting off the higher-order digits is more informative than
     lower-order cut off. */
  عدد_چوڑائی = sprintf (نمبر_buff, "%*d", chars_فی_نمبر, سطر_نمبر);
  سطر_نمبر++;
  s = نمبر_buff + (عدد_چوڑائی - chars_فی_نمبر);
  لكل (i = chars_فی_نمبر; i > 0; i--)
    (p->char_func) (*s++);

  إذا (columns > 1)
    {
      /* Tabification is assumed for multiple columns, also for n-separators,
         but 'default n-separator = TAB' hasn't been given priority over
         equal column_width also specified by POSIX. */
      إذا (نمبر_separator == '\t')
        {
          i = نمبر_چوڑائی - chars_فی_نمبر;
          طالما (i-- > 0)
            (p->char_func) (' ');
        }
      وإلا
        (p->char_func) (نمبر_separator);
    }
  وإلا
    /* To comply with POSIX, we avoid any expansion of default TAB
       separator with a single column output. No column_width requirement
       has to be considered. */
    {
      (p->char_func) (نمبر_separator);
      إذا (نمبر_separator == '\t')
        output_position = POS_AFTER_TAB (chars_فی_output_tab,
                          output_position);
    }

  إذا (truncate_سطریں && !parallel_files)
    داخلہ_position += نمبر_چوڑائی;
}

/* Print (or store) padding until the current horizontal position
   is position. */

ساكن فراغ
pad_across_منزل (صحيح position)
{
  صحيح h = output_position;

  إذا (tabify_output)
    spaces_not_printed = position - output_position;
  وإلا
    {
      طالما (++h <= position)
        حرف_لکھو (' ');
      output_position = position;
    }
}

/* Pad to the bottom of the page.

   If the user has requested a formfeed, use one.
   Otherwise, use newlines. */

ساكن فراغ
pad_تھلے (بنانشانی صحيح سطریں)
{
  إذا (use_form_feed)
    حرف_لکھو ('\f');
  وإلا
    لكل (بنانشانی صحيح i = سطریں; i; --i)
      حرف_لکھو ('\n');
}

/* Read the rest of the line.

   Read from the current column's file until an end of line is
   hit.  Used when we've truncated a line and we no longer need
   to print or store its characters. */

ساكن فراغ
read_rest_دا_سطر (کالم_3 *p)
{
  صحيح c;
  FILE *f = p->fp;

  طالما ((c = getc (f)) != '\n')
    {
      إذا (c == '\f')
        {
          إذا ((c = getc (f)) != '\n')
            ungetc (c, f);
          إذا (keep_FF)
            print_a_FF = true;
          hold_file (p);
          اكسر;
        }
      وإلا إذا (c == EOF)
        {
          close_file (p);
          اكسر;
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

ساكن فراغ
skip_read (کالم_3 *p, صحيح کالم_نمبر)
{
  صحيح c;
  FILE *f = p->fp;
  صحيح i;
  bool single_ff = false;
  کالم_3 *q;

  /* Read 1st character in a line or any character succeeding a FF */
  إذا ((c = getc (f)) == '\f' && p->بھریا_ورقہ_printed)
    /* A FF-coincidence with a previous full_page_printed.
       To avoid an additional empty page, eliminate the FF */
    إذا ((c = getc (f)) == '\n')
      c = getc (f);

  p->بھریا_ورقہ_printed = false;

  /* 1st character a FF means a single FF without any printable
     characters. Don't count it as a line with -n option. */
  إذا (c == '\f')
    single_ff = true;

  /* Preparing for a FF-coincidence: Maybe we finish that page
     without a FF found */
  إذا (آخری_سطر)
    p->بھریا_ورقہ_printed = true;

  طالما (c != '\n')
    {
      إذا (c == '\f')
        {
          /* No FF-coincidence possible,
             no catching up of a FF-coincidence with next page */
          إذا (آخری_سطر)
            {
              إذا (!parallel_files)
                لكل (q = کالم_سمتیہ, i = columns; i; ++q, --i)
                  q->بھریا_ورقہ_printed = false;
              وإلا
                p->بھریا_ورقہ_printed = false;
            }

          إذا ((c = getc (f)) != '\n')
            ungetc (c, f);
          hold_file (p);
          اكسر;
        }
      وإلا إذا (c == EOF)
        {
          close_file (p);
          اكسر;
        }
      c = getc (f);
    }

  إذا (skip_گنتی)
    إذا ((!parallel_files || کالم_نمبر == 1) && !single_ff)
      ++سطر_گنتی;
}

/* If we're tabifying output,

   When print_char encounters white space it keeps track
   of our desired horizontal position and delays printing
   until this function is called. */

ساكن فراغ
print_white_space (فراغ)
{
  صحيح h_new;
  صحيح h_old = output_position;
  صحيح goal = h_old + spaces_not_printed;

  طالما (goal - h_old > 1
         && (h_new = POS_AFTER_TAB (chars_فی_output_tab, h_old)) <= goal)
    {
      حرف_لکھو (output_tab_char);
      h_old = h_new;
    }
  طالما (++h_old <= goal)
    حرف_لکھو (' ');

  output_position = goal;
  spaces_not_printed = 0;
}

/* Print column separators.

   We keep a count until we know that we'll be printing a line,
   then print_sep_string() is called. */

ساكن فراغ
print_sep_string (فراغ)
{
  حرف ثابت *s = خانہ_sep_string;
  صحيح l = خانہ_sep_لمائی;

  إذا (separators_not_printed <= 0)
    {
      /* We'll be starting a line with chars_per_margin, anything else? */
      إذا (spaces_not_printed > 0)
        print_white_space ();
    }
  وإلا
    {
      لكل (; separators_not_printed > 0; --separators_not_printed)
        {
          طالما (l-- > 0)
            {
              /* 3 types of sep_strings: spaces only, spaces and chars,
              chars only */
              إذا (*s == ' ')
                {
                  /* We're tabifying output; consecutive spaces in
                  sep_string may have to be converted to tabs */
                  s++;
                  ++spaces_not_printed;
                }
              وإلا
                {
                  إذا (spaces_not_printed > 0)
                    print_white_space ();
                  حرف_لکھو (*s++);
                  ++output_position;
                }
            }
          /* sep_string ends with some spaces */
          إذا (spaces_not_printed > 0)
            print_white_space ();
        }
    }
}

/* Print (or store, depending on p->char_func) a clump of N
   characters. */

ساكن فراغ
print_clump (کالم_3 *p, صحيح n, حرف *clump)
{
  طالما (n--)
    (p->char_func) (*clump++);

  إذا (ferror (stdout))
    write_error ();
}

/* Print a character.

   Update the following comment: process-char hasn't been used any
   longer.
   If we're tabifying, all tabs have been converted to spaces by
   process_char().  Keep a count of consecutive spaces, and when
   a nonspace is encountered, call print_white_space() to print the
   required number of tabs and spaces. */

ساكن فراغ
print_char (حرف c)
{
  إذا (tabify_output)
    {
      إذا (c == ' ')
        {
          ++spaces_not_printed;
          أرجع;
        }
      وإلا إذا (spaces_not_printed > 0)
        print_white_space ();

      /* Nonprintables are assumed to have width 0, except '\b'. */
      إذا (! isprint (منزل_uchar (c)))
        {
          إذا (c == '\b')
            --output_position;
        }
      وإلا
        ++output_position;
    }
  حرف_لکھو (c);
}

/* Skip to page PAGE before printing.
   PAGE may be larger than total number of pages. */

ساكن bool
skip_منزل_ورقہ (uintmax_t ورقہ)
{
  لكل (uintmax_t n = 1; n < ورقہ; ++n)
    {
      کالم_3 *p;
      صحيح j;

      لكل (صحيح i = 1; i < سطریں_فی_body; ++i)
        {
          لكل (j = 1, p = کالم_سمتیہ; j <= columns; ++j, ++p)
            إذا (p->حالت == OPEN)
              skip_read (p, j);
        }
      آخری_سطر = true;
      لكل (j = 1, p = کالم_سمتیہ; j <= columns; ++j, ++p)
        إذا (p->حالت == OPEN)
          skip_read (p, j);

      إذا (storing_columns)	/* change FF_FOUND to ON_HOLD */
        لكل (j = 1, p = کالم_سمتیہ; j <= columns; ++j, ++p)
          إذا (p->حالت != CLOSED)
            p->حالت = ON_HOLD;

      reset_حالت ();
      آخری_سطر = false;

      إذا (files_ready_منزل_read < 1)
        {
          /* It's very helpful, normally the total number of pages is
             not known in advance.  */
          error (0, 0,
                 _("starting page number %ju exceeds page count %ju"),
                 ورقہ, n);
          اكسر;
        }
    }
  أرجع files_ready_منزل_read > 0;
}

/* Print a header.

   Formfeeds are assumed to use up two lines at the beginning of
   the page. */

ساكن فراغ
print_سرخی (فراغ)
{
  حرف ورقہ_متن[256 + INT_STRLEN_BOUND (ورقہ_نمبر)];
  صحيح دستیاب_چوڑائی;
  صحيح lhs_spaces;
  صحيح rhs_spaces;

  output_position = 0;
  pad_across_منزل (chars_فی_margin);
  print_white_space ();

  إذا (ورقہ_نمبر == 0)
    error (EXIT_FAILURE, 0, _("page number overflow"));

  /* The translator must ensure that formatting the translation of
     "Page %ju" does not generate more than (sizeof page_text - 1)
     bytes.  */
  sprintf (ورقہ_متن, _("Page %ju"), ورقہ_نمبر);
  دستیاب_چوڑائی = سرخی_چوڑائی_دستیاب - mbswidth (ورقہ_متن, 0);
  دستیاب_چوڑائی = MAX (0, دستیاب_چوڑائی);
  lhs_spaces = دستیاب_چوڑائی >> 1;
  rhs_spaces = دستیاب_چوڑائی - lhs_spaces;

  لکھو ("\n\n%*s%s%*s%s%*s%s\n\n\n",
          chars_فی_margin, "",
          date_متن, lhs_spaces, " ",
          file_متن, rhs_spaces, " ", ورقہ_متن);

  print_a_سرخی = false;
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

ساكن bool
read_سطر (کالم_3 *p)
{
  صحيح c;
  صحيح chars IF_LINT (= 0);
  صحيح آخری_داخلہ_position;
  صحيح j, k;
  کالم_3 *q;

  /* read 1st character in each line or any character succeeding a FF: */
  c = getc (p->fp);

  آخری_داخلہ_position = داخلہ_position;

  إذا (c == '\f' && p->بھریا_ورقہ_printed)
    إذا ((c = getc (p->fp)) == '\n')
      c = getc (p->fp);
  p->بھریا_ورقہ_printed = false;

  بدل (c)
    {
    حالة '\f':
      إذا ((c = getc (p->fp)) != '\n')
        ungetc (c, p->fp);
      FF_only = true;
      إذا (print_a_سرخی && !storing_columns)
        {
          pad_vertically = true;
          print_سرخی ();
        }
      وإلا إذا (keep_FF)
        print_a_FF = true;
      hold_file (p);
      أرجع true;
    حالة EOF:
      close_file (p);
      أرجع true;
    حالة '\n':
      اكسر;
    افتراضي:
      chars = char_منزل_clump (c);
    }

  إذا (truncate_سطریں && داخلہ_position > chars_فی_کالم)
    {
      داخلہ_position = آخری_داخلہ_position;
      أرجع false;
    }

  إذا (p->char_func != store_char)
    {
      pad_vertically = true;

      إذا (print_a_سرخی && !storing_columns)
        print_سرخی ();

      إذا (parallel_files && align_خالی_تھاواں_خانے_گنتی)
        {
          /* We have to align empty columns at the beginning of a line. */
          k = separators_not_printed;
          separators_not_printed = 0;
          لكل (j = 1, q = کالم_سمتیہ; j <= k; ++j, ++q)
            {
              align_کالم (q);
              separators_not_printed += 1;
            }
          padding_not_printed = p->شروع_position;
          إذا (truncate_سطریں)
            spaces_not_printed = chars_فی_کالم;
          وإلا
            spaces_not_printed = 0;
          align_خالی_تھاواں_خانے_گنتی = false;
        }

      إذا (خانہ_sep_لمائی < padding_not_printed)
        {
          pad_across_منزل (padding_not_printed - خانہ_sep_لمائی);
          padding_not_printed = ANYWHERE;
        }

      إذا (use_خانہ_separator)
        print_sep_string ();
    }

  إذا (p->numbered)
    جوڑو_سطر_نمبر (p);

  خالی_تھاواں_سطر = false;
  إذا (c == '\n')
    أرجع true;

  print_clump (p, chars, clump_buff);

  طالما (true)
    {
      c = getc (p->fp);

      بدل (c)
        {
        حالة '\n':
          أرجع true;
        حالة '\f':
          إذا ((c = getc (p->fp)) != '\n')
            ungetc (c, p->fp);
          إذا (keep_FF)
            print_a_FF = true;
          hold_file (p);
          أرجع true;
        حالة EOF:
          close_file (p);
          أرجع true;
        }

      آخری_داخلہ_position = داخلہ_position;
      chars = char_منزل_clump (c);
      إذا (truncate_سطریں && داخلہ_position > chars_فی_کالم)
        {
          داخلہ_position = آخری_داخلہ_position;
          أرجع false;
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

ساكن bool
print_stored (کالم_3 *p)
{
  کالم_3 *q;

  صحيح سطر = p->current_سطر++;
  حرف *پہلا = &buff[سطر_سمتیہ[سطر]];
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
  حرف *آخری = &buff[سطر_سمتیہ[سطر + 1]];

  pad_vertically = true;

  إذا (print_a_سرخی)
    print_سرخی ();

  إذا (p->حالت == FF_لبھیا)
    {
      صحيح i;
      لكل (i = 1, q = کالم_سمتیہ; i <= columns; ++i, ++q)
        q->حالت = ON_HOLD;
      إذا (کالم_سمتیہ->سطریں_منزل_print <= 0)
        {
          إذا (!extremities)
            pad_vertically = false;
          أرجع true;		/* print a header only */
        }
    }

  إذا (خانہ_sep_لمائی < padding_not_printed)
    {
      pad_across_منزل (padding_not_printed - خانہ_sep_لمائی);
      padding_not_printed = ANYWHERE;
    }

  إذا (use_خانہ_separator)
    print_sep_string ();

  طالما (پہلا != آخری)
    print_char (*پہلا++);

  إذا (spaces_not_printed == 0)
    {
      output_position = p->شروع_position + سرا_سمتیہ[سطر];
      إذا (p->شروع_position - خانہ_sep_لمائی == chars_فی_margin)
        output_position -= خانہ_sep_لمائی;
    }

  أرجع true;
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

ساكن صحيح
char_منزل_clump (حرف c)
{
  بنانشانی حرف uc = c;
  حرف *s = clump_buff;
  حرف esc_buff[4];
  صحيح چوڑائی;
  صحيح chars;
  صحيح chars_فی_c = 8;

  إذا (c == داخلہ_tab_char)
    chars_فی_c = chars_فی_داخلہ_tab;

  إذا (c == داخلہ_tab_char || c == '\t')
    {
      چوڑائی = TAB_چوڑائی (chars_فی_c, داخلہ_position);

      إذا (untabify_داخلہ)
        {
          لكل (صحيح i = چوڑائی; i; --i)
            *s++ = ' ';
          chars = چوڑائی;
        }
      وإلا
        {
          *s = c;
          chars = 1;
        }

    }
  وإلا إذا (! isprint (uc))
    {
      إذا (use_esc_sequence)
        {
          چوڑائی = 4;
          chars = 4;
          *s++ = '\\';
          sprintf (esc_buff, "%03o", uc);
          لكل (صحيح i = 0; i <= 2; ++i)
            *s++ = esc_buff[i];
        }
      وإلا إذا (use_cntrl_اگیتر)
        {
          إذا (uc < 0200)
            {
              چوڑائی = 2;
              chars = 2;
              *s++ = '^';
              *s = c ^ 0100;
            }
          وإلا
            {
              چوڑائی = 4;
              chars = 4;
              *s++ = '\\';
              sprintf (esc_buff, "%03o", uc);
              لكل (صحيح i = 0; i <= 2; ++i)
                *s++ = esc_buff[i];
            }
        }
      وإلا إذا (c == '\b')
        {
          چوڑائی = -1;
          chars = 1;
          *s = c;
        }
      وإلا
        {
          چوڑائی = 0;
          chars = 1;
          *s = c;
        }
    }
  وإلا
    {
      چوڑائی = 1;
      chars = 1;
      *s = c;
    }

  /* Too many backspaces must put us in position 0 -- never negative.  */
  إذا (چوڑائی < 0 && داخلہ_position == 0)
    {
      chars = 0;
      داخلہ_position = 0;
    }
  وإلا إذا (چوڑائی < 0 && داخلہ_position <= -چوڑائی)
    داخلہ_position = 0;
  وإلا
    {
      إذا (ckd_جوڑو (&داخلہ_position, داخلہ_position, چوڑائی))
        integer_حد_توں_ودھ ();
    }

  أرجع chars;
}

/* We've just printed some files and need to clean up things before
   looking for more options and printing the next batch of files.

   Free everything we've xmalloc'ed, except 'header'. */

ساكن فراغ
cleanup (فراغ)
{
  حرر (نمبر_buff);
  حرر (clump_buff);
  حرر (کالم_سمتیہ);
  حرر (سطر_سمتیہ);
  حرر (سرا_سمتیہ);
  حرر (buff);
}

/* Complain, print a usage message, and die. */

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکھو (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              پروگرام_name);

      fputs (_("\
Paginate or columnate FILE(s) لكل printing.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\n\
  +پہلا_ورقہ[:آخری_ورقہ], --ورقے=پہلا_ورقہ[:آخری_ورقہ]\n\
         begin [stop] printing with ورقہ پہلا_2[آخری_2]ورقہ_2\n\
"));
      oputs (_("\
  -خانے_گنتی, --columns=خانے_گنتی\n\
         output خانے_گنتی columns and print columns تھلے, unless -a is ورتیا.\n\
         Balance نمبر دا سطریں وچ the columns on each ورقہ\n\
"));
      oputs (_("\
  -a, --across\n\
         print columns across rather than تھلے, ورتیا together with -خانے_گنتی\n\
"));
      oputs (_("\
  -c, --show-control-chars\n\
         use hat notation (^G) and octal backslash notation\n\
"));
      oputs (_("\
  -d, --مزدوج-space\n\
         مزدوج space the output\n\
"));
      oputs (_("\
  -D, --date-format=FORMAT\n\
         use FORMAT لكل the سرخی date\n\
"));
      oputs (_("\
  -e[CHAR[چوڑائی_2]], --expand-tabs[=CHAR[چوڑائی_2]]\n\
         expand داخلہ CHARs (TABs) منزل tab چوڑائی_2 (8)\n\
"));
      oputs (_("\
  -F, -f, --form-feed\n\
         use form feeds instead دا newlines منزل separate ورقے\n\
         (by a 3-سطر ورقہ سرخی with -F or a 5-سطر سرخی\n\
         and trailer without -F)\n\
"));
      oputs (_("\
  -h, --سرخی=سرخی_3\n\
         use a centered سرخی_3 instead دا filename وچ ورقہ سرخی,\n\
         -h \"\" prints a blank سطر, don't use -h\"\"\n\
"));
      oputs (_("\
  -i[CHAR[چوڑائی_2]], --output-tabs[=CHAR[چوڑائی_2]]\n\
         replace spaces with CHARs (TABs) منزل tab چوڑائی_2 (8)\n\
"));
      oputs (_("\
  -J, --join-سطریں\n\
         ملاؤ بھریا سطریں, turns off -W سطر truncation,\n\
         no کالم alignment, --sep-string[=STRING] sets separators\n\
"));
      oputs (_("\
  -l, --لمائی=ورقہ_لمائی\n\
         سیٹ_2 the ورقہ لمائی منزل ورقہ_لمائی (66) سطریں\n\
         (افتراضي نمبر دا سطریں دا متن 56, and with -F 63).\n\
         سٹا_دیندا -t إذا ورقہ_لمائی <= 10\n\
"));
      oputs (_("\
  -m, --ملاؤ\n\
         print all files وچ parallel, one وچ each کالم,\n\
         truncate سطریں, but join سطریں دا بھریا لمائی with -J\n\
"));
      oputs (_("\
  -n[SEP[DIGITS]], --نمبر-سطریں[=SEP[DIGITS]]\n\
         نمبر سطریں, use DIGITS (5) digits, then SEP (TAB),\n\
         افتراضي counting starts with 1st سطر دا داخلہ file\n\
"));
      oputs (_("\
  -N, --پہلا-سطر-نمبر=نمبر_2\n\
         شروع counting with نمبر_2 at 1st سطر دا پہلا\n\
         ورقہ printed (see +پہلا_ورقہ)\n\
"));
      oputs (_("\
  -o, --indent=MARGIN\n\
         وتھ each سطر with MARGIN (zero) spaces, افعل not\n\
         affect -w or -W, MARGIN will be added منزل ورقہ_چوڑائی\n\
"));
      oputs (_("\
  -r, --no-file-warnings\n\
         omit warning when a file cannot be opened\n\
"));
      oputs (_("\
  -s[CHAR], --separator[=CHAR]\n\
         separate columns by a single character, افتراضي لكل CHAR\n\
         is the <TAB> character without -w and \'no حرف\' with -w.\n\
         -s[CHAR] turns off سطر truncation دا all 3 کالم\n\
         options (-خانے_گنتی|-a -خانے_گنتی|-m) except -w is سیٹ_2\n\
"));
      oputs (_("\
  -S[STRING], --sep-string[=STRING]\n\
         separate columns by STRING,\n\
         without -S: Default separator <TAB> with -J and <space>\n\
         otherwise (same as -S\" \"), no effect on کالم options\n\
"));
      oputs (_("\
  -t, --omit-سرخی\n\
         omit ورقہ سرخیاں and trailers; implied إذا ورقہ_لمائی <= 10\n\
"));
      oputs (_("\
  -T, --omit-pagination\n\
         omit ورقہ سرخیاں and trailers,\n\
         eliminate any pagination by form feeds سیٹ_2 وچ داخلہ files\n\
"));
      oputs (_("\
  -v, --show-nonprinting\n\
         use octal backslash notation\n\
"));
      oputs (_("\
  -w, --چوڑائی=ورقہ_چوڑائی\n\
         سیٹ_2 ورقہ چوڑائی منزل ورقہ_چوڑائی (72) characters لكل\n\
         مضرب متن-کالم output only, -s[حرف] turns off (72)\n\
"));
      oputs (_("\
  -W, --ورقہ-چوڑائی=ورقہ_چوڑائی\n\
         سیٹ_2 ورقہ چوڑائی منزل ورقہ_چوڑائی (72) characters always,\n\
         truncate سطریں, except -J option is سیٹ_2,\n\
         no interference with -S or -s\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت);
}
