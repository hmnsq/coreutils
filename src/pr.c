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

#ଅନ୍ତର୍ଭାଵ <config.h>

#ଅନ୍ତର୍ଭାଵ <getopt.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "c-ctype.h"
#ଅନ୍ତର୍ଭାଵ "fadvise.h"
#ଅନ୍ତର୍ଭାଵ "hard-locale.h"
#ଅନ୍ତର୍ଭାଵ "mbswidth.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "stat-time.h"
#ଅନ୍ତର୍ଭାଵ "stdio--.h"
#ଅନ୍ତର୍ଭାଵ "strftime.h"
#ଅନ୍ତର୍ଭାଵ "xstrtol.h"
#ଅନ୍ତର୍ଭାଵ "xstrtol-error.h"
#ଅନ୍ତର୍ଭାଵ "xdectoint.h"

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

ସଂରଚନା COLUMN;
ସଂରଚନା COLUMN
  {
    FILE *fp;			/* Input stream for this column. */
    ଅକ୍ଷର ଅଚଳ *name;		/* File name. */
    ଗଣନା
      {
        OPEN,
        FF_FOUND,		/* used with -b option, set with \f, changed
                                   to ON_HOLD after print_header */
        ON_HOLD,		/* Hit a form feed. */
        CLOSED
      }
    status;			/* Status of the file pointer. */

    /* Func to print lines in this col. */
    bool (*print_func) (ସଂରଚନା COLUMN *);

    /* Func to print/store chars in this col. */
    ଶୂନ୍ୟ (*char_func) (ଅକ୍ଷର);

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା current_ଧାଡ଼ି;		/* Index of current place in line_vector. */
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଧାଡ଼ିଗୁଡ଼ିକ_stored;		/* Number of lines stored in buff. */
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଧାଡ଼ିଗୁଡ଼ିକ_to_print;		/* No. lines stored or space left on page. */
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା start_position;		/* Horizontal position of first char. */
    bool numbered;
    bool full_page_printed;	/* True means printed without a FF found. */

    /* p->full_page_printed  controls a special case of "FF set by hand":
       True means a full page has been printed without FF found. To avoid an
       additional empty page we have to ignore a FF immediately following in
       the next line. */
  };

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା COLUMN COLUMN;

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା char_to_clump (ଅକ୍ଷର c);
ସ୍ଥିର bool read_ଧାଡ଼ି (COLUMN *p);
ସ୍ଥିର bool print_page (ଶୂନ୍ୟ);
ସ୍ଥିର bool print_stored (COLUMN *p);
ସ୍ଥିର bool open_file (ଅକ୍ଷର *name, COLUMN *p);
ସ୍ଥିର bool skip_to_page (uintmax_t page);
ସ୍ଥିର ଶୂନ୍ୟ print_header (ଶୂନ୍ୟ);
ସ୍ଥିର ଶୂନ୍ୟ pad_across_to (ପୂର୍ଣ୍ଣସଂଖ୍ୟା position);
ସ୍ଥିର ଶୂନ୍ୟ add_ଧାଡ଼ି_ସଂଖ୍ୟା (COLUMN *p);
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା getoptnum (ଅକ୍ଷର ଅଚଳ *n_str, ପୂର୍ଣ୍ଣସଂଖ୍ୟା min, ଅକ୍ଷର ଅଚଳ *errfmt);
ସ୍ଥିର ଶୂନ୍ୟ getoptarg (ଅକ୍ଷର *arg, ଅକ୍ଷର switch_char, ଅକ୍ଷର *character,
                       ପୂର୍ଣ୍ଣସଂଖ୍ୟା *ସଂଖ୍ୟା);
ସ୍ଥିର ଶୂନ୍ୟ print_files (ପୂର୍ଣ୍ଣସଂଖ୍ୟା ସଂଖ୍ୟା_of_files, ଅକ୍ଷର **av);
ସ୍ଥିର ଶୂନ୍ୟ init_parameters (ପୂର୍ଣ୍ଣସଂଖ୍ୟା ସଂଖ୍ୟା_of_files);
ସ୍ଥିର ଶୂନ୍ୟ init_header (ଅକ୍ଷର ଅଚଳ *filename, ପୂର୍ଣ୍ଣସଂଖ୍ୟା desc);
ସ୍ଥିର bool init_fps (ପୂର୍ଣ୍ଣସଂଖ୍ୟା ସଂଖ୍ୟା_of_files, ଅକ୍ଷର **av);
ସ୍ଥିର ଶୂନ୍ୟ init_funcs (ଶୂନ୍ୟ);
ସ୍ଥିର ଶୂନ୍ୟ init_store_cols (ଶୂନ୍ୟ);
ସ୍ଥିର ଶୂନ୍ୟ store_columns (ଶୂନ୍ୟ);
ସ୍ଥିର ଶୂନ୍ୟ balance (ପୂର୍ଣ୍ଣସଂଖ୍ୟା total_stored);
ସ୍ଥିର ଶୂନ୍ୟ store_char (ଅକ୍ଷର c);
ସ୍ଥିର ଶୂନ୍ୟ pad_down (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଧାଡ଼ିଗୁଡ଼ିକ);
ସ୍ଥିର ଶୂନ୍ୟ read_rest_of_ଧାଡ଼ି (COLUMN *p);
ସ୍ଥିର ଶୂନ୍ୟ skip_read (COLUMN *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା column_ସଂଖ୍ୟା);
ସ୍ଥିର ଶୂନ୍ୟ print_char (ଅକ୍ଷର c);
ସ୍ଥିର ଶୂନ୍ୟ cleanup (ଶୂନ୍ୟ);
ସ୍ଥିର ଶୂନ୍ୟ print_sep_string (ଶୂନ୍ୟ);
ସ୍ଥିର ଶୂନ୍ୟ separator_string (ଅକ୍ଷର ଅଚଳ *optarg_S);

/* All of the columns to print.  */
ସ୍ଥିର COLUMN *column_vector;

/* When printing a single file in multiple downward columns,
   we store the leftmost columns contiguously in buff.
   To print a line from buff, get the index of the first character
   from line_vector[i], and print up to line_vector[i + 1]. */
ସ୍ଥିର ଅକ୍ଷର *buff;

/* Index of the position in buff where the next character
   will be stored. */
ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା buff_current;

/* The number of characters in buff.
   Used for allocation of buff and to detect overflow of buff. */
ସ୍ଥିର idx_t buff_allocated;

/* Array of indices into buff.
   Each entry is an index of the first character of a line.
   This is used when storing lines to facilitate shuffling when
   we do column balancing on the last page. */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା *ଧାଡ଼ି_vector;

/* Array of horizontal positions.
   For each line in line_vector, end_vector[line] is the horizontal
   position we are in after printing that line.  We keep track of this
   so that we know how much we need to pad to prepare for the next
   column. */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା *end_vector;

/* (-m) True means we're printing multiple files in parallel. */
ସ୍ଥିର bool parallel_files = false;

/* (-m) True means a line starts with some empty columns (some files
   already CLOSED or ON_HOLD) which we have to align. */
ସ୍ଥିର bool align_empty_cols;

/* (-m) True means we have not yet found any printable column in a line.
   align_empty_cols = true  has to be maintained. */
ସ୍ଥିର bool empty_ଧାଡ଼ି;

/* (-m) False means printable column output precedes a form feed found.
   Column alignment is done only once. No additional action with that form
   feed.
   True means we found only a form feed in a column. Maybe we have to do
   some column alignment with that form feed. */
ସ୍ଥିର bool FF_only;

/* (-[0-9]+) True means we're given an option explicitly specifying
   number of columns.  Used to detect when this option is used with -m
   and when translating old options to new/long options. */
ସ୍ଥିର bool explicit_columns = false;

/* (-t|-T) False means we aren't printing headers and footers. */
ସ୍ଥିର bool extremities = true;

/* (-t) True means we retain all FF set by hand in input files.
   False is set with -T option. */
ସ୍ଥିର bool keep_FF = false;
ସ୍ଥିର bool print_a_FF = false;

/* True means we need to print a header as soon as we know we've got input
   to print after it. */
ସ୍ଥିର bool print_a_header;

/* (-f) True means use formfeeds instead of newlines to separate pages. */
ସ୍ଥିର bool use_form_feed = false;

/* True means we have read the standard input. */
ସ୍ଥିର bool have_read_stdin = false;

/* True means the -a flag has been given. */
ସ୍ଥିର bool print_across_flag = false;

/* True means we're printing one file in multiple (>1) downward columns. */
ସ୍ଥିର bool storing_columns = true;

/* (-b) True means balance columns on the last page as Sys V does. */
/* That's no longer an independent option. With storing_columns = true
   balance_columns = true is used too (s. function init_parameters).
   We get a consistent formulation with "FF set by hand" in input files. */
ସ୍ଥିର bool balance_columns = false;

/* (-l) Number of lines on a page, including header and footer lines. */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଧାଡ଼ିଗୁଡ଼ିକ_per_page = 66;

/* Number of lines in the header and footer can be reset to 0 using
   the -t flag. */
ଗଣନା { ଧାଡ଼ିଗୁଡ଼ିକ_per_header = 5 };
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଧାଡ଼ିଗୁଡ଼ିକ_per_body;
ଗଣନା { ଧାଡ଼ିଗୁଡ଼ିକ_per_footer = 5 };

/* (-w|-W) Width in characters of the page.  Does not include the width of
   the margin. */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା chars_per_ଧାଡ଼ି = 72;

/* (-w|W) True means we truncate lines longer than chars_per_column. */
ସ୍ଥିର bool truncate_ଧାଡ଼ିଗୁଡ଼ିକ = false;

/* (-J) True means we join lines without any line truncation. -J
   dominates -w option. */
ସ୍ଥିର bool join_ଧାଡ଼ିଗୁଡ଼ିକ = false;

/* Number of characters in a column.  Based on col_sep_length and
   page width. */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା chars_per_column;

/* (-e) True means convert tabs to spaces on input. */
ସ୍ଥିର bool untabify_ନିବେଶ = false;

/* (-e) The input tab character. */
ସ୍ଥିର ଅକ୍ଷର ନିବେଶ_tab_char = '\t';

/* (-e) Tabstops are at chars_per_tab, 2*chars_per_tab, 3*chars_per_tab, ...
   where the leftmost column is 1. */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା chars_per_ନିବେଶ_tab = 8;

/* (-i) True means convert spaces to tabs on output. */
ସ୍ଥିର bool tabify_output = false;

/* (-i) The output tab character. */
ସ୍ଥିର ଅକ୍ଷର output_tab_char = '\t';

/* (-i) The width of the output tab. */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା chars_per_output_tab = 8;

/* Keeps track of pending white space.  When we hit a nonspace
   character after some whitespace, we print whitespace, tabbing
   if necessary to get to output_position + spaces_not_printed. */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା spaces_not_printed;

/* (-o) Number of spaces in the left margin (tabs used when possible). */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା chars_per_margin = 0;

/* Position where the next character will fall.
   Leftmost position is 0 + chars_per_margin.
   Rightmost position is chars_per_margin + chars_per_line - 1.
   This is important for converting spaces to tabs on output. */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା output_position;

/* Horizontal position relative to the current file.
   (output_position depends on where we are on the page;
   input_position depends on where we are in the file.)
   Important for converting tabs to spaces on input. */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ନିବେଶ_position;

/* True if there were any failed opens so we can exit with nonzero
   status.  */
ସ୍ଥିର bool failed_opens = false;

/* The number of spaces taken up if we print a tab character with width
   c_ from position h_. */
#define TAB_WIDTH(c_, h_) ((c_) - ((h_) % (c_)))

/* The horizontal position we'll be at after printing a tab character
   of width c_ from the position h_. */
#define POS_AFTER_TAB(c_, h_) ((h_) + TAB_WIDTH (c_, h_))

/* (-NNN) Number of columns of text to print. */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା columns = 1;

/* (+NNN:MMM) Page numbers on which to begin and stop printing.
   first_page_number = 0  will be used to check input only. */
ସ୍ଥିର uintmax_t first_page_ସଂଖ୍ୟା = 0;
ସ୍ଥିର uintmax_t last_page_ସଂଖ୍ୟା = UINTMAX_MAX;

/* Number of files open (not closed, not on hold). */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା files_ready_to_read = 0;

/* Current page number.  Displayed in header. */
ସ୍ଥିର uintmax_t page_ସଂଖ୍ୟା;

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
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଧାଡ଼ି_ସଂଖ୍ୟା;

/* (-n) True means lines should be preceded by numbers. */
ସ୍ଥିର bool numbered_ଧାଡ଼ିଗୁଡ଼ିକ = false;

/* (-n) Character which follows each line number. */
ସ୍ଥିର ଅକ୍ଷର ସଂଖ୍ୟା_separator = '\t';

/* (-n) line counting starts with 1st line of input file (not with 1st
   line of 1st page printed). */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଧାଡ଼ି_ଗଣନା = 1;

/* (-n) True means counting of skipped lines starts with 1st line of
   input file. False means -N option is used in addition, counting of
   skipped lines not required. */
ସ୍ଥିର bool skip_ଗଣନା = true;

/* (-N) Counting starts with start_line_number = NUMBER at 1st line of
   first page printed, usually not 1st page of input file. */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା start_ଧାଡ଼ି_ଅଙ୍କ = 1;

/* (-n) Width in characters of a line number. */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା chars_per_ସଂଖ୍ୟା = 5;

/* Used when widening the first column to accommodate numbers -- only
   needed when printing files in parallel.  Includes width of both the
   number and the number_separator. */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ସଂଖ୍ୟା_width;

/* Buffer sprintf uses to format a line number. */
ସ୍ଥିର ଅକ୍ଷର *ସଂଖ୍ୟା_buff;

/* (-v) True means nonprintable characters are printed as escape sequences.
   control-g becomes \007. */
ସ୍ଥିର bool use_esc_sequence = false;

/* (-c) True means nonprintable characters are printed as control prefixes.
   control-g becomes ^G. */
ସ୍ଥିର bool use_cntrl_prefix = false;

/* (-d) True means output is double spaced. */
ସ୍ଥିର bool double_space = false;

/* Number of files opened initially in init_files.  Should be 1
   unless we're printing multiple files in parallel. */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା total_files = 0;

/* (-r) True means don't complain if we can't open a file. */
ସ୍ଥିର bool ignore_failed_opens = false;

/* (-S) True means we separate columns with a specified string.
   -S option does not affect line truncation nor column alignment. */
ସ୍ଥିର bool use_col_separator = false;

/* String used to separate columns if the -S option has been specified.
   Default without -S but together with one of the column options
   -a|COLUMN|-m is a 'space' and with the -J option a 'tab'. */
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *col_sep_string = "";
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା col_sep_length = 0;
ସ୍ଥିର ଅକ୍ଷର *column_separator = (ଅକ୍ଷର *) " ";
ସ୍ଥିର ଅକ୍ଷର *ଧାଡ଼ି_separator = (ଅକ୍ଷର *) "\t";

/* Number of separator characters waiting to be printed as soon as we
   know that we have any input remaining to be printed. */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା separators_not_printed;

/* Position we need to pad to, as soon as we know that we have input
   remaining to be printed. */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା padding_not_printed;

/* True means we should pad the end of the page.  Remains false until we
   know we have a page to print. */
ସ୍ଥିର bool pad_vertically;

/* (-h) String of characters used in place of the filename in the header. */
ସ୍ଥିର ଅକ୍ଷର *custom_header;

/* (-D) Date format for the header.  */
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *date_format;

/* The local time zone rules, as per the TZ environment variable.  */
ସ୍ଥିର timezone_t localtz;

/* Date and file name for the header.  */
ସ୍ଥିର ଅକ୍ଷର *date_text;
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *file_text;

/* Output columns available, not counting the date and file name.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା header_width_available;

ସ୍ଥିର ଅକ୍ଷର *clump_buff;

/* True means we read the line no. lines_per_body in skip_read
   called by skip_to_page. That variable controls the coincidence of a
   "FF set by hand" and "full_page_printed", see above the definition of
   structure COLUMN. */
ସ୍ଥିର bool last_ଧାଡ଼ି = false;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ଗଣନା
{
  COLUMNS_OPTION = CHAR_MAX + 1,
  PAGES_OPTION
};

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ short_options[] =
  "-0123456789D:FJN:S::TW:abcde::fh:i::l:mn::o:rs::tvw:";

ସ୍ଥିର ସଂରଚନା option ଅଚଳ long_options[] =
{
  {"pages", required_argument, ଖାଲି, PAGES_OPTION},
  {"columns", required_argument, ଖାଲି, COLUMNS_OPTION},
  {"across", no_argument, ଖାଲି, 'a'},
  {"show-control-chars", no_argument, ଖାଲି, 'c'},
  {"double-space", no_argument, ଖାଲି, 'd'},
  {"date-format", required_argument, ଖାଲି, 'D'},
  {"expand-tabs", optional_argument, ଖାଲି, 'e'},
  {"form-feed", no_argument, ଖାଲି, 'f'},
  {"header", required_argument, ଖାଲି, 'h'},
  {"output-tabs", optional_argument, ଖାଲି, 'i'},
  {"join-lines", no_argument, ଖାଲି, 'J'},
  {"length", required_argument, ଖାଲି, 'l'},
  {"merge", no_argument, ଖାଲି, 'm'},
  {"number-lines", optional_argument, ଖାଲି, 'n'},
  {"first-line-number", required_argument, ଖାଲି, 'N'},
  {"indent", required_argument, ଖାଲି, 'o'},
  {"no-file-warnings", no_argument, ଖାଲି, 'r'},
  {"separator", optional_argument, ଖାଲି, 's'},
  {"sep-string", optional_argument, ଖାଲି, 'S'},
  {"omit-header", no_argument, ଖାଲି, 't'},
  {"omit-pagination", no_argument, ଖାଲି, 'T'},
  {"show-nonprinting", no_argument, ଖାଲି, 'v'},
  {"width", required_argument, ଖାଲି, 'w'},
  {"page-width", required_argument, ଖାଲି, 'W'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ଖାଲି, 0, ଖାଲି, 0}
};

ସ୍ଥିର _Noreturn ଶୂନ୍ୟ
integer_overflow (ଶୂନ୍ୟ)
{
  error (EXIT_FAILURE, 0, _("integer overflow"));
}

/* Return the number of columns that have either an open file or
   stored lines. */

ATTRIBUTE_PURE
ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା
cols_ready_to_print (ଶୂନ୍ୟ)
{
  COLUMN *q;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା n;

  n = 0;
  ପାଇଁ (q = column_vector, i = 0; i < columns; ++q, ++i)
    ଯଦି (q->status == OPEN
        || q->status == FF_FOUND	/* With -b: To print a header only */
        || (storing_columns && q->ଧାଡ଼ିଗୁଡ଼ିକ_stored > 0 && q->ଧାଡ଼ିଗୁଡ଼ିକ_to_print > 0))
      ++n;
  ଫେରନ୍ତୁ n;
}

/* Estimate first_ / last_page_number
   using option +FIRST_PAGE:LAST_PAGE */

ସ୍ଥିର bool
first_last_page (ପୂର୍ଣ୍ଣସଂଖ୍ୟା oi, ଅକ୍ଷର c, ଅକ୍ଷର ଅଚଳ *pages)
{
  ଅକ୍ଷର *p;
  uintmax_t first;
  uintmax_t last = UINTMAX_MAX;
  strtol_error err = xstrtoumax (pages, &p, 10, &first, "");
  ଯଦି (err != LONGINT_OK && err != LONGINT_INVALID_SUFFIX_CHAR)
    xstrtol_fatal (err, oi, c, long_options, pages);

  ଯଦି (p == pages || !first)
    ଫେରନ୍ତୁ false;

  ଯଦି (*p == ':')
    {
      ଅକ୍ଷର ଅଚଳ *p1 = p + 1;
      err = xstrtoumax (p1, &p, 10, &last, "");
      ଯଦି (err != LONGINT_OK)
        xstrtol_fatal (err, oi, c, long_options, pages);
      ଯଦି (p1 == p || last < first)
        ଫେରନ୍ତୁ false;
    }

  ଯଦି (*p)
    ଫେରନ୍ତୁ false;

  first_page_ସଂଖ୍ୟା = first;
  last_page_ସଂଖ୍ୟା = last;
  ଫେରନ୍ତୁ true;
}

/* Parse column count string S, and if it's valid (1 or larger and
   within range of the type of 'columns') set the global variables
   columns and explicit_columns.  Otherwise, exit with a diagnostic.  */

ସ୍ଥିର ଶୂନ୍ୟ
parse_column_ଗଣନା (ଅକ୍ଷର ଅଚଳ *s)
{
  columns = getoptnum (s, 1, _("invalid number of columns"));
  explicit_columns = true;
}

/* Estimate length of col_sep_string with option -S.  */

ସ୍ଥିର ଶୂନ୍ୟ
separator_string (ଅକ୍ଷର ଅଚଳ *optarg_S)
{
  size_t len = strlen (optarg_S);
  ଯଦି (INT_MAX < len)
    integer_overflow ();
  col_sep_length = len;
  col_sep_string = optarg_S;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_files;
  bool old_options = false;
  bool old_w = false;
  bool old_s = false;
  ଅକ୍ଷର **file_names;

  /* Accumulate the digits of old-style options like -99.  */
  ଅକ୍ଷର *column_ଗଣନା_string = ଖାଲି;
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
                ? xnmalloc (argc - 1, ଆକାର (ଅକ୍ଷର *))
                : ଖାଲି);

  ଯେତେବେଳେ (true)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା oi = -1;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = getopt_long (argc, argv, short_options, long_options, &oi);
      ଯଦି (c == -1)
        ଭାଙ୍ଗ;

      ଯଦି (c_isdigit (c))
        {
          /* Accumulate column-count digits specified via old-style options. */
          ଯଦି (n_digits + 1 >= n_alloc)
            column_ଗଣନା_string = xpalloc (column_ଗଣନା_string, &n_alloc, 2, -1,
                                           ଆକାର *column_ଗଣନା_string);
          column_ଗଣନା_string[n_digits++] = c;
          column_ଗଣନା_string[n_digits] = '\0';
          ଜାରି;
        }

      n_digits = 0;

      ବାଛ (c)
        {
        ସ୍ଥିତି 1:			/* Non-option argument. */
          /* long option --page dominates old '+FIRST_PAGE ...'.  */
          ଯଦି (! (first_page_ସଂଖ୍ୟା == 0
                 && *optarg == '+' && first_last_page (-2, '+', optarg + 1)))
            file_names[n_files++] = optarg;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି PAGES_OPTION:	/* --pages=FIRST_PAGE[:LAST_PAGE] */
          {			/* dominates old opt +... */
            ଯଦି (! optarg)
              error (EXIT_FAILURE, 0,
                     _("'--pages=FIRST_PAGE[:LAST_PAGE]' missing argument"));
            ଅନ୍ୟଥା ଯଦି (! first_last_page (oi, 0, optarg))
              error (EXIT_FAILURE, 0, _("invalid page range %s"),
                     quote (optarg));
            ଭାଙ୍ଗ;
          }

        ସ୍ଥିତି COLUMNS_OPTION:	/* --columns=COLS */
          {
            parse_column_ଗଣନା (optarg);

            /* If there was a prior column count specified via the
               short-named option syntax, e.g., -9, ensure that this
               long-name-specified value overrides it.  */
            ମୁକ୍ତ (column_ଗଣନା_string);
            column_ଗଣନା_string = ଖାଲି;
            n_alloc = 0;
            ଭାଙ୍ଗ;
          }

        ସ୍ଥିତି 'a':
          print_across_flag = true;
          storing_columns = false;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'b':
          balance_columns = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'c':
          use_cntrl_prefix = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'd':
          double_space = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'D':
          date_format = optarg;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'e':
          ଯଦି (optarg)
            getoptarg (optarg, 'e', &ନିବେଶ_tab_char,
                       &chars_per_ନିବେଶ_tab);
          /* Could check tab width > 0. */
          untabify_ନିବେଶ = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'f':
        ସ୍ଥିତି 'F':
          use_form_feed = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'h':
          custom_header = optarg;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'i':
          ଯଦି (optarg)
            getoptarg (optarg, 'i', &output_tab_char,
                       &chars_per_output_tab);
          /* Could check tab width > 0. */
          tabify_output = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'J':
          join_ଧାଡ଼ିଗୁଡ଼ିକ = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'l':
          ଧାଡ଼ିଗୁଡ଼ିକ_per_page
            = getoptnum (optarg, 1,
                         _("'-l PAGE_LENGTH' invalid number of lines"));
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'm':
          parallel_files = true;
          storing_columns = false;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'n':
          numbered_ଧାଡ଼ିଗୁଡ଼ିକ = true;
          ଯଦି (optarg)
            getoptarg (optarg, 'n', &ସଂଖ୍ୟା_separator,
                       &chars_per_ସଂଖ୍ୟା);
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'N':
          skip_ଗଣନା = false;
          start_ଧାଡ଼ି_ଅଙ୍କ
            = getoptnum (optarg, INT_MIN,
                         _("'-N NUMBER' invalid starting line number"));
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'o':
          chars_per_margin = getoptnum (optarg, 0,
                                        _("'-o MARGIN' invalid line offset"));
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'r':
          ignore_failed_opens = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 's':
          old_options = true;
          old_s = true;
          ଯଦି (!use_col_separator && optarg)
            separator_string (optarg);
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'S':
          old_s = false;
          /* Reset an additional input of -s, -S dominates -s */
          col_sep_string = "";
          col_sep_length = 0;
          use_col_separator = true;
          ଯଦି (optarg)
            separator_string (optarg);
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 't':
          extremities = false;
          keep_FF = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'T':
          extremities = false;
          keep_FF = false;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'v':
          use_esc_sequence = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'w':
          old_options = true;
          old_w = true;
          {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା tmp_cpl
              = getoptnum (optarg, 1,
                           _("'-w PAGE_WIDTH' invalid number of characters"));
            ଯଦି (! truncate_ଧାଡ଼ିଗୁଡ଼ିକ)
              chars_per_ଧାଡ଼ି = tmp_cpl;
          }
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'W':
          old_w = false;			/* dominates -w */
          truncate_ଧାଡ଼ିଗୁଡ଼ିକ = true;
          chars_per_ଧାଡ଼ି
            = getoptnum (optarg, 1,
                         _("'-W PAGE_WIDTH' invalid number of characters"));
          ଭାଙ୍ଗ;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          usage (EXIT_FAILURE);
          ଭାଙ୍ଗ;
        }
    }

  ଯଦି (column_ଗଣନା_string)
    {
      parse_column_ଗଣନା (column_ଗଣନା_string);
      ମୁକ୍ତ (column_ଗଣନା_string);
    }

  ଯଦି (! date_format)
    date_format = (getenv ("POSIXLY_CORRECT") && !hard_locale (LC_TIME)
                   ? "%b %e %H:%M %Y"
                   : "%Y-%m-%d %H:%M");

  localtz = tzalloc (getenv ("TZ"));

  /* Now we can set a reasonable initial value: */
  ଯଦି (first_page_ସଂଖ୍ୟା == 0)
    first_page_ସଂଖ୍ୟା = 1;

  ଯଦି (parallel_files && explicit_columns)
    error (EXIT_FAILURE, 0,
           _("cannot specify number of columns when printing in parallel"));

  ଯଦି (parallel_files && print_across_flag)
    error (EXIT_FAILURE, 0,
           _("cannot specify both printing across and printing in parallel"));

/* Translate some old short options to new/long options.
   To meet downward compatibility with other UNIX pr utilities
   and some POSIX specifications. */

  ଯଦି (old_options)
    {
      ଯଦି (old_w)
        {
          ଯଦି (parallel_files || explicit_columns)
            {
              /* activate -W */
              truncate_ଧାଡ଼ିଗୁଡ଼ିକ = true;
              ଯଦି (old_s)
                /* adapt HP-UX and SunOS: -s = no separator;
                   activate -S */
                use_col_separator = true;
            }
          ଅନ୍ୟଥା
            /* old -w sets width with columns only
               activate -J */
            join_ଧାଡ଼ିଗୁଡ଼ିକ = true;
        }
      ଅନ୍ୟଥା ଯଦି (!use_col_separator)
        {
          /* No -S option read */
          ଯଦି (old_s && (parallel_files || explicit_columns))
            {
              ଯଦି (!truncate_ଧାଡ଼ିଗୁଡ଼ିକ)
                {
                  /* old -s (without -w and -W) annuls column alignment,
                  uses fields, activate -J */
                  join_ଧାଡ଼ିଗୁଡ଼ିକ = true;
                  ଯଦି (col_sep_length > 0)
                    /* activate -S */
                    use_col_separator = true;
                }
              ଅନ୍ୟଥା
                /* with -W */
                /* adapt HP-UX and SunOS: -s = no separator;
                   activate -S */
                use_col_separator = true;
            }
        }
    }

  ପାଇଁ (; optind < argc; optind++)
    {
      file_names[n_files++] = argv[optind];
    }

  ଯଦି (n_files == 0)
    {
      /* No file arguments specified;  read from standard input.  */
      print_files (0, ଖାଲି);
    }
  ଅନ୍ୟଥା
    {
      ଯଦି (parallel_files)
        print_files (n_files, file_names);
      ଅନ୍ୟଥା
        {
          ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < n_files; i++)
            print_files (1, &file_names[i]);
        }
    }

  cleanup ();

  ଯଦି (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, _("standard input"));
  main_exit (failed_opens ? EXIT_FAILURE : EXIT_SUCCESS);
}

/* Parse numeric arguments, ensuring MIN <= number <= INT_MAX.  */

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
getoptnum (ଅକ୍ଷର ଅଚଳ *n_str, ପୂର୍ଣ୍ଣସଂଖ୍ୟା min, ଅକ୍ଷର ଅଚଳ *err)
{
  ଫେରନ୍ତୁ xnumtoimax (n_str, 10, min, INT_MAX, "", err, 0,
                     min <= 0 ? 0 : XTOINT_MIN_RANGE);
}

/* Parse options of the form -scNNN.

   Example: -nck, where 'n' is the option, c is the optional number
   separator, and k is the optional width of the field used when printing
   a number. */

ସ୍ଥିର ଶୂନ୍ୟ
getoptarg (ଅକ୍ଷର *arg, ଅକ୍ଷର switch_char, ଅକ୍ଷର *character, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *ସଂଖ୍ୟା)
{
  ଅକ୍ଷର ଅଚଳ *ଅଙ୍କ = arg;

  ଯଦି (!*ଅଙ୍କ)
    {
      error (0, 0, _("'-%c': Invalid argument: %s"), switch_char, quote (ଅଙ୍କ));
      usage (EXIT_FAILURE);
    }

  ଯଦି (!c_isdigit (*ଅଙ୍କ))
    *character = *ଅଙ୍କ++;
  ଯଦି (*ଅଙ୍କ)
    {
      long ପୂର୍ଣ୍ଣସଂଖ୍ୟା tmp_long;
      strtol_error e = xstrtol (ଅଙ୍କ, ଖାଲି, 10, &tmp_long, "");
      ଯଦି (e == LONGINT_OK)
        {
          ଯଦି (tmp_long <= 0)
            e = LONGINT_INVALID;
          ଅନ୍ୟଥା ଯଦି (INT_MAX < tmp_long)
            e = LONGINT_OVERFLOW;
        }
      ଯଦି (e != LONGINT_OK)
        {
          error (0, e & LONGINT_OVERFLOW ? EOVERFLOW : 0,
             _("'-%c' extra characters or invalid number in the argument: %s"),
                 switch_char, quote (arg));
          usage (EXIT_FAILURE);
        }
      *ସଂଖ୍ୟା = tmp_long;
    }
}

/* Set parameters related to formatting. */

ସ୍ଥିର ଶୂନ୍ୟ
init_parameters (ପୂର୍ଣ୍ଣସଂଖ୍ୟା ସଂଖ୍ୟା_of_files)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା chars_used_by_ସଂଖ୍ୟା = 0;

  ଧାଡ଼ିଗୁଡ଼ିକ_per_body = ଧାଡ଼ିଗୁଡ଼ିକ_per_page - ଧାଡ଼ିଗୁଡ଼ିକ_per_header - ଧାଡ଼ିଗୁଡ଼ିକ_per_footer;
  ଯଦି (ଧାଡ଼ିଗୁଡ଼ିକ_per_body <= 0)
    {
      extremities = false;
      keep_FF = true;
    }
  ଯଦି (! extremities)
    ଧାଡ଼ିଗୁଡ଼ିକ_per_body = ଧାଡ଼ିଗୁଡ଼ିକ_per_page;

  ଯଦି (double_space)
    ଧାଡ଼ିଗୁଡ଼ିକ_per_body = MAX (1, ଧାଡ଼ିଗୁଡ଼ିକ_per_body / 2);

  /* If input is stdin, cannot print parallel files.  BSD dumps core
     on this. */
  ଯଦି (ସଂଖ୍ୟା_of_files == 0)
    parallel_files = false;

  ଯଦି (parallel_files)
    columns = ସଂଖ୍ୟା_of_files;

  /* One file, multi columns down: -b option is set to get a consistent
     formulation with "FF set by hand" in input files. */
  ଯଦି (storing_columns)
    balance_columns = true;

  /* Tabification is assumed for multiple columns. */
  ଯଦି (columns > 1)
    {
      ଯଦି (!use_col_separator)
        {
          /* Use default separator */
          ଯଦି (join_ଧାଡ଼ିଗୁଡ଼ିକ)
            col_sep_string = ଧାଡ଼ି_separator;
          ଅନ୍ୟଥା
            col_sep_string = column_separator;

          col_sep_length = 1;
          use_col_separator = true;
        }
      /* It's rather pointless to define a TAB separator with column
         alignment */
      ଅନ୍ୟଥା ଯଦି (!join_ଧାଡ଼ିଗୁଡ଼ିକ && col_sep_length == 1 && *col_sep_string == '\t')
        col_sep_string = column_separator;

      truncate_ଧାଡ଼ିଗୁଡ଼ିକ = true;
      ଯଦି (! (col_sep_length == 1 && *col_sep_string == '\t'))
        untabify_ନିବେଶ = true;
      tabify_output = true;
    }
  ଅନ୍ୟଥା
    storing_columns = false;

  /* -J dominates -w in any case */
  ଯଦି (join_ଧାଡ଼ିଗୁଡ଼ିକ)
    truncate_ଧାଡ଼ିଗୁଡ଼ିକ = false;

  ଯଦି (numbered_ଧାଡ଼ିଗୁଡ଼ିକ)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା chars_per_default_tab = 8;

      ଧାଡ଼ି_ଗଣନା = start_ଧାଡ଼ି_ଅଙ୍କ;

      /* To allow input tab-expansion (-e sensitive) use:
         if (number_separator == input_tab_char)
           number_width = chars_per_number
             + TAB_WIDTH (chars_per_input_tab, chars_per_number);   */

      /* Estimate chars_per_text without any margin and keep it constant. */
      ଯଦି (ସଂଖ୍ୟା_separator == '\t')
        ସଂଖ୍ୟା_width = (chars_per_ସଂଖ୍ୟା
                        + TAB_WIDTH (chars_per_default_tab, chars_per_ସଂଖ୍ୟା));
      ଅନ୍ୟଥା
        ସଂଖ୍ୟା_width = chars_per_ସଂଖ୍ୟା + 1;

      /* The number is part of the column width unless we are
         printing files in parallel. */
      ଯଦି (parallel_files)
        chars_used_by_ସଂଖ୍ୟା = ସଂଖ୍ୟା_width;
    }

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା sep_chars, useful_chars;
  ଯଦି (ckd_mul (&sep_chars, columns - 1, col_sep_length))
    sep_chars = INT_MAX;
  ଯଦି (ckd_sub (&useful_chars, chars_per_ଧାଡ଼ି - chars_used_by_ସଂଖ୍ୟା,
               sep_chars))
    useful_chars = 0;
  chars_per_column = useful_chars / columns;

  ଯଦି (chars_per_column < 1)
    error (EXIT_FAILURE, 0, _("page width too narrow"));

  ଯଦି (numbered_ଧାଡ଼ିଗୁଡ଼ିକ)
    {
      ମୁକ୍ତ (ସଂଖ୍ୟା_buff);
      ସଂଖ୍ୟା_buff = xmalloc (MAX (chars_per_ସଂଖ୍ୟା,
                                  INT_STRLEN_BOUND (ଧାଡ଼ି_ସଂଖ୍ୟା)) + 1);
    }

  /* Pick the maximum between the tab width and the width of an
     escape sequence.
     The width of an escape sequence (4) isn't the lower limit any longer.
     We've to use 8 as the lower limit, if we use chars_per_default_tab = 8
     to expand a tab which is not an input_tab-char. */
  ମୁକ୍ତ (clump_buff);
  clump_buff = xmalloc (MAX (8, chars_per_ନିବେଶ_tab));
}

/* Open the necessary files,
   maintaining a COLUMN structure for each column.

   With multiple files, each column p has a different p->fp.
   With single files, each column p has the same p->fp.
   Return false if (number_of_files > 0) and no files can be opened,
   true otherwise.

   With each column/file p, p->full_page_printed is initialized,
   see also open_file.  */

ସ୍ଥିର bool
init_fps (ପୂର୍ଣ୍ଣସଂଖ୍ୟା ସଂଖ୍ୟା_of_files, ଅକ୍ଷର **av)
{
  COLUMN *p;

  total_files = 0;

  ମୁକ୍ତ (column_vector);
  column_vector = xnmalloc (columns, ଆକାର (COLUMN));

  ଯଦି (parallel_files)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା files_ବାମ = ସଂଖ୍ୟା_of_files;
      ପାଇଁ (p = column_vector; files_ବାମ--; ++p, ++av)
        {
          ଯଦି (! open_file (*av, p))
            {
              --p;
              --columns;
            }
        }
      ଯଦି (columns == 0)
        ଫେରନ୍ତୁ false;
      init_header ("", -1);
    }
  ଅନ୍ୟଥା
    {
      p = column_vector;
      ଯଦି (ସଂଖ୍ୟା_of_files > 0)
        {
          ଯଦି (! open_file (*av, p))
            ଫେରନ୍ତୁ false;
          init_header (*av, fileno (p->fp));
          p->ଧାଡ଼ିଗୁଡ଼ିକ_stored = 0;
        }
      ଅନ୍ୟଥା
        {
          p->name = _("standard input");
          p->fp = stdin;
          have_read_stdin = true;
          p->status = OPEN;
          p->full_page_printed = false;
          ++total_files;
          init_header ("", -1);
          p->ଧାଡ଼ିଗୁଡ଼ିକ_stored = 0;
        }

      ଅକ୍ଷର ଅଚଳ *firstname = p->name;
      FILE *firstfp = p->fp;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
      ପାଇଁ (i = columns - 1, ++p; i; --i, ++p)
        {
          p->name = firstname;
          p->fp = firstfp;
          p->status = OPEN;
          p->full_page_printed = false;
          p->ଧାଡ଼ିଗୁଡ଼ିକ_stored = 0;
        }
    }
  files_ready_to_read = total_files;
  ଫେରନ୍ତୁ true;
}

/* Determine print_func and char_func, the functions
   used by each column for printing and/or storing.

   Determine the horizontal position desired when we begin
   printing a column (p->start_position). */

ସ୍ଥିର ଶୂନ୍ୟ
init_funcs (ଶୂନ୍ୟ)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, h, h_next;
  COLUMN *p;

  h = chars_per_margin;

  ଯଦି (!truncate_ଧାଡ଼ିଗୁଡ଼ିକ)
    h_next = ANYWHERE;
  ଅନ୍ୟଥା
    {
      /* When numbering lines of parallel files, we enlarge the
         first column to accommodate the number.  Looks better than
         the Sys V approach. */
      ଯଦି (parallel_files && numbered_ଧାଡ଼ିଗୁଡ଼ିକ)
        h_next = h + chars_per_column + ସଂଖ୍ୟା_width;
      ଅନ୍ୟଥା
        h_next = h + chars_per_column;
    }

  /* Enlarge p->start_position of first column to use the same form of
     padding_not_printed with all columns. */
  h = h + col_sep_length;

  /* This loop takes care of all but the rightmost column. */

  ପାଇଁ (p = column_vector, i = 1; i < columns; ++p, ++i)
    {
      ଯଦି (storing_columns)	/* One file, multi columns down. */
        {
          p->char_func = store_char;
          p->print_func = print_stored;
        }
      ଅନ୍ୟଥା
        /* One file, multi columns across; or parallel files.  */
        {
          p->char_func = print_char;
          p->print_func = read_ଧାଡ଼ି;
        }

      /* Number only the first column when printing files in
         parallel. */
      p->numbered = numbered_ଧାଡ଼ିଗୁଡ଼ିକ && (!parallel_files || i == 1);
      p->start_position = h;

      /* If we don't truncate lines, all start_positions are
         ANYWHERE, except the first column's start_position when
         using a margin. */

      ଯଦି (!truncate_ଧାଡ଼ିଗୁଡ଼ିକ)
        {
          h = ANYWHERE;
          h_next = ANYWHERE;
        }
      ଅନ୍ୟଥା
        {
          h = h_next + col_sep_length;
          h_next = h + chars_per_column;
        }
    }

  /* The rightmost column.

     Doesn't need to be stored unless we intend to balance
     columns on the last page. */
  ଯଦି (storing_columns && balance_columns)
    {
      p->char_func = store_char;
      p->print_func = print_stored;
    }
  ଅନ୍ୟଥା
    {
      p->char_func = print_char;
      p->print_func = read_ଧାଡ଼ି;
    }

  p->numbered = numbered_ଧାଡ଼ିଗୁଡ଼ିକ && (!parallel_files || i == 1);
  p->start_position = h;
}

/* Open a file.  Return true if successful.

   With each file p, p->full_page_printed is initialized,
   see also init_fps. */

ସ୍ଥିର bool
open_file (ଅକ୍ଷର *name, COLUMN *p)
{
  ଯଦି (streq (name, "-"))
    {
      p->name = _("standard input");
      p->fp = stdin;
      have_read_stdin = true;
    }
  ଅନ୍ୟଥା
    {
      p->name = name;
      p->fp = fopen (name, "r");
    }
  ଯଦି (p->fp == ଖାଲି)
    {
      failed_opens = true;
      ଯଦି (!ignore_failed_opens)
        error (0, errno, "%s", quotef (name));
      ଫେରନ୍ତୁ false;
    }
  fadvise (p->fp, FADVISE_SEQUENTIAL);
  p->status = OPEN;
  p->full_page_printed = false;
  ++total_files;
  ଫେରନ୍ତୁ true;
}

/* Close the file in P.

   If we aren't dealing with multiple files in parallel, we change
   the status of all columns in the column list to reflect the close. */

ସ୍ଥିର ଶୂନ୍ୟ
close_file (COLUMN *p)
{
  COLUMN *q;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

  ଯଦି (p->status == CLOSED)
    ଫେରନ୍ତୁ;

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା err = errno;
  ଯଦି (!ferror (p->fp))
    err = 0;
  ଯଦି (fileno (p->fp) == STDIN_FILENO)
    clearerr (p->fp);
  ଅନ୍ୟଥା ଯଦି (fclose (p->fp) != 0 && !err)
    err = errno;
  ଯଦି (err)
    error (EXIT_FAILURE, err, "%s", quotef (p->name));

  ଯଦି (!parallel_files)
    {
      ପାଇଁ (q = column_vector, i = columns; i; ++q, --i)
        {
          q->status = CLOSED;
          ଯଦି (q->ଧାଡ଼ିଗୁଡ଼ିକ_stored == 0)
            {
              q->ଧାଡ଼ିଗୁଡ଼ିକ_to_print = 0;
            }
        }
    }
  ଅନ୍ୟଥା
    {
      p->status = CLOSED;
      p->ଧାଡ଼ିଗୁଡ଼ିକ_to_print = 0;
    }

  --files_ready_to_read;
}

/* Put a file on hold until we start a new page,
   since we've hit a form feed.

   If we aren't dealing with parallel files, we must change the
   status of all columns in the column list. */

ସ୍ଥିର ଶୂନ୍ୟ
hold_file (COLUMN *p)
{
  COLUMN *q;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

  ଯଦି (!parallel_files)
    ପାଇଁ (q = column_vector, i = columns; i; ++q, --i)
      {
        ଯଦି (storing_columns)
          q->status = FF_FOUND;
        ଅନ୍ୟଥା
          q->status = ON_HOLD;
      }
  ଅନ୍ୟଥା
    p->status = ON_HOLD;

  p->ଧାଡ଼ିଗୁଡ଼ିକ_to_print = 0;
  --files_ready_to_read;
}

/* Undo hold_file -- go through the column list and change any
   ON_HOLD columns to OPEN.  Used at the end of each page. */

ସ୍ଥିର ଶୂନ୍ୟ
reset_status (ଶୂନ୍ୟ)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = columns;
  COLUMN *p;

  ପାଇଁ (p = column_vector; i; --i, ++p)
    ଯଦି (p->status == ON_HOLD)
      {
        p->status = OPEN;
        files_ready_to_read++;
      }

  ଯଦି (storing_columns)
    {
      ଯଦି (column_vector->status == CLOSED)
        /* We use the info to output an error message in  skip_to_page. */
        files_ready_to_read = 0;
      ଅନ୍ୟଥା
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

ସ୍ଥିର ଶୂନ୍ୟ
print_files (ପୂର୍ଣ୍ଣସଂଖ୍ୟା ସଂଖ୍ୟା_of_files, ଅକ୍ଷର **av)
{
  init_parameters (ସଂଖ୍ୟା_of_files);
  ଯଦି (! init_fps (ସଂଖ୍ୟା_of_files, av))
    ଫେରନ୍ତୁ;
  ଯଦି (storing_columns)
    init_store_cols ();

  ଯଦି (first_page_ସଂଖ୍ୟା > 1)
    {
      ଯଦି (!skip_to_page (first_page_ସଂଖ୍ୟା))
        ଫେରନ୍ତୁ;
      ଅନ୍ୟଥା
        page_ସଂଖ୍ୟା = first_page_ସଂଖ୍ୟା;
    }
  ଅନ୍ୟଥା
    page_ସଂଖ୍ୟା = 1;

  init_funcs ();

  ଧାଡ଼ି_ସଂଖ୍ୟା = ଧାଡ଼ି_ଗଣନା;
  ଯେତେବେଳେ (print_page ())
    ;
}

/* Initialize header information.
   If DESC is non-negative, it is a file descriptor open to
   FILENAME for reading.  */

ସ୍ଥିର ଶୂନ୍ୟ
init_header (ଅକ୍ଷର ଅଚଳ *filename, ପୂର୍ଣ୍ଣସଂଖ୍ୟା desc)
{
  ଅକ୍ଷର *buf = ଖାଲି;
  ସଂରଚନା stat st;
  ସଂରଚନା timespec t;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ns;
  ସଂରଚନା tm tm;

  /* If parallel files or standard input, use current date. */
  ଯଦି (streq (filename, "-"))
    desc = -1;
  ଯଦି (0 <= desc && fstat (desc, &st) == 0)
    t = get_stat_mtime (&st);
  ଅନ୍ୟଥା
    {
      ସ୍ଥିର ସଂରଚନା timespec timespec;
      ଯଦି (! timespec.tv_sec)
        gettime (&timespec);
      t = timespec;
    }

  ns = t.tv_nsec;
  ଯଦି (localtime_rz (localtz, &t.tv_sec, &tm))
    {
      ptrdiff_t len = nstrftime (ଖାଲି, MIN (PTRDIFF_MAX, SIZE_MAX),
                                 date_format, &tm, localtz, ns);
      ଯଦି (0 <= len)
        {
          buf = ximalloc (len + 1);
          nstrftime (buf, len + 1, date_format, &tm, localtz, ns);
        }
    }
  ଯଦି (!buf)
    {
      ଅକ୍ଷର secbuf[INT_BUFSIZE_BOUND (intmax_t)];
      buf = xmalloc (ଆକାର secbuf + MAX (10, INT_BUFSIZE_BOUND (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)));
      sprintf (buf, "%s.%09d", timetostr (t.tv_sec, secbuf), ns);
    }

  ମୁକ୍ତ (date_text);
  date_text = buf;
  file_text = custom_header ? custom_header : desc < 0 ? "" : filename;
  header_width_available = (chars_per_ଧାଡ଼ି
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

ସ୍ଥିର ଶୂନ୍ୟ
init_page (ଶୂନ୍ୟ)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା j;
  COLUMN *p;

  ଯଦି (storing_columns)
    {
      store_columns ();
      ପାଇଁ (j = columns - 1, p = column_vector; j; --j, ++p)
        {
          p->ଧାଡ଼ିଗୁଡ଼ିକ_to_print = p->ଧାଡ଼ିଗୁଡ଼ିକ_stored;
        }

      /* Last column. */
      ଯଦି (balance_columns)
        {
          p->ଧାଡ଼ିଗୁଡ଼ିକ_to_print = p->ଧାଡ଼ିଗୁଡ଼ିକ_stored;
        }
      /* Since we're not balancing columns, we don't need to store
         the rightmost column.   Read it straight from the file. */
      ଅନ୍ୟଥା
        {
          ଯଦି (p->status == OPEN)
            {
              p->ଧାଡ଼ିଗୁଡ଼ିକ_to_print = ଧାଡ଼ିଗୁଡ଼ିକ_per_body;
            }
          ଅନ୍ୟଥା
            p->ଧାଡ଼ିଗୁଡ଼ିକ_to_print = 0;
        }
    }
  ଅନ୍ୟଥା
    ପାଇଁ (j = columns, p = column_vector; j; --j, ++p)
      ଯଦି (p->status == OPEN)
        {
          p->ଧାଡ଼ିଗୁଡ଼ିକ_to_print = ଧାଡ଼ିଗୁଡ଼ିକ_per_body;
        }
      ଅନ୍ୟଥା
        p->ଧାଡ଼ିଗୁଡ଼ିକ_to_print = 0;
}

/* Align empty columns and print separators.
   Empty columns will be formed by files with status ON_HOLD or CLOSED
   when printing multiple files in parallel. */

ସ୍ଥିର ଶୂନ୍ୟ
align_column (COLUMN *p)
{
  padding_not_printed = p->start_position;
  ଯଦି (col_sep_length < padding_not_printed)
    {
      pad_across_to (padding_not_printed - col_sep_length);
      padding_not_printed = ANYWHERE;
    }

  ଯଦି (use_col_separator)
    print_sep_string ();

  ଯଦି (p->numbered)
    add_ଧାଡ଼ି_ସଂଖ୍ୟା (p);
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

ସ୍ଥିର bool
print_page (ଶୂନ୍ୟ)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା j;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଧାଡ଼ିଗୁଡ଼ିକ_ବାମ_on_page;
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

  ଯଦି (cols_ready_to_print () == 0)
    ଫେରନ୍ତୁ false;

  ଯଦି (extremities)
    print_a_header = true;

  /* Don't pad unless we know a page was printed. */
  pad_vertically = false;
  pv = false;

  ଧାଡ଼ିଗୁଡ଼ିକ_ବାମ_on_page = ଧାଡ଼ିଗୁଡ଼ିକ_per_body;
  ଯଦି (double_space)
    ଧାଡ଼ିଗୁଡ଼ିକ_ବାମ_on_page *= 2;

  ଯେତେବେଳେ (ଧାଡ଼ିଗୁଡ଼ିକ_ବାମ_on_page > 0 && cols_ready_to_print () > 0)
    {
      output_position = 0;
      spaces_not_printed = 0;
      separators_not_printed = 0;
      pad_vertically = false;
      align_empty_cols = false;
      empty_ଧାଡ଼ି = true;

      ପାଇଁ (j = 1, p = column_vector; j <= columns; ++j, ++p)
        {
          ନିବେଶ_position = 0;
          ଯଦି (p->ଧାଡ଼ିଗୁଡ଼ିକ_to_print > 0 || p->status == FF_FOUND)
            {
              FF_only = false;
              padding_not_printed = p->start_position;
              ଯଦି (!(p->print_func) (p))
                read_rest_of_ଧାଡ଼ି (p);
              pv |= pad_vertically;

              --p->ଧାଡ଼ିଗୁଡ଼ିକ_to_print;
              ଯଦି (p->ଧାଡ଼ିଗୁଡ଼ିକ_to_print <= 0)
                {
                  ଯଦି (cols_ready_to_print () == 0)
                    ଭାଙ୍ଗ;
                }

              /* File p changed its status to ON_HOLD or CLOSED */
              ଯଦି (parallel_files && p->status != OPEN)
                {
                  ଯଦି (empty_ଧାଡ଼ି)
                    align_empty_cols = true;
                  ଅନ୍ୟଥା ଯଦି (p->status == CLOSED
                           || (p->status == ON_HOLD && FF_only))
                    align_column (p);
                }
            }
          ଅନ୍ୟଥା ଯଦି (parallel_files)
            {
              /* File status ON_HOLD or CLOSED */
              ଯଦି (empty_ଧାଡ଼ି)
                align_empty_cols = true;
              ଅନ୍ୟଥା
                align_column (p);
            }

          /* We need it also with an empty column */
          ଯଦି (use_col_separator)
            ++separators_not_printed;
        }

      ଯଦି (pad_vertically)
        {
          putchar ('\n');
          --ଧାଡ଼ିଗୁଡ଼ିକ_ବାମ_on_page;
        }

      ଯଦି (cols_ready_to_print () == 0 && !extremities)
        ଭାଙ୍ଗ;

      ଯଦି (double_space && pv)
        {
          putchar ('\n');
          --ଧାଡ଼ିଗୁଡ଼ିକ_ବାମ_on_page;
        }
    }

  ଯଦି (ଧାଡ଼ିଗୁଡ଼ିକ_ବାମ_on_page == 0)
    ପାଇଁ (j = 1, p = column_vector; j <= columns; ++j, ++p)
      ଯଦି (p->status == OPEN)
        p->full_page_printed = true;

  pad_vertically = pv;

  ଯଦି (pad_vertically && extremities)
    pad_down (ଧାଡ଼ିଗୁଡ଼ିକ_ବାମ_on_page + ଧାଡ଼ିଗୁଡ଼ିକ_per_footer);
  ଅନ୍ୟଥା ଯଦି (keep_FF && print_a_FF)
    {
      putchar ('\f');
      print_a_FF = false;
    }

  ଯଦି (ferror (stdout))
    write_error ();

  ଯଦି (last_page_ସଂଖ୍ୟା < ++page_ସଂଖ୍ୟା)
    ଫେରନ୍ତୁ false;		/* Stop printing with LAST_PAGE */

  reset_status ();		/* Change ON_HOLD to OPEN. */

  ଫେରନ୍ତୁ true;			/* More pages to go. */
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

ସ୍ଥିର ଶୂନ୍ୟ
init_store_cols (ଶୂନ୍ୟ)
{
  /* Tune this.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା total_ଧାଡ଼ିଗୁଡ଼ିକ, total_ଧାଡ଼ିଗୁଡ଼ିକ_1, chars_per_column_1, chars_if_truncate;
  ଯଦି (ckd_mul (&total_ଧାଡ଼ିଗୁଡ଼ିକ, ଧାଡ଼ିଗୁଡ଼ିକ_per_body, columns)
      || ckd_add (&total_ଧାଡ଼ିଗୁଡ଼ିକ_1, total_ଧାଡ଼ିଗୁଡ଼ିକ, 1)
      || ckd_add (&chars_per_column_1, chars_per_column, 1)
      || ckd_mul (&chars_if_truncate, total_ଧାଡ଼ିଗୁଡ଼ିକ, chars_per_column_1)
      || ckd_mul (&buff_allocated, chars_if_truncate, use_col_separator + 1))
    integer_overflow ();

  ମୁକ୍ତ (ଧାଡ଼ି_vector);
  /* FIXME: here's where it was allocated.  */
  ଧାଡ଼ି_vector = xnmalloc (total_ଧାଡ଼ିଗୁଡ଼ିକ_1, ଆକାର *ଧାଡ଼ି_vector);

  ମୁକ୍ତ (end_vector);
  end_vector = xnmalloc (total_ଧାଡ଼ିଗୁଡ଼ିକ, ଆକାର *end_vector);

  ମୁକ୍ତ (buff);
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

ସ୍ଥିର ଶୂନ୍ୟ
store_columns (ଶୂନ୍ୟ)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଧାଡ଼ି = 0;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା buff_start;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା last_col;		/* The rightmost column which will be saved in buff */
  COLUMN *p;

  buff_current = 0;
  buff_start = 0;

  ଯଦି (balance_columns)
    last_col = columns;
  ଅନ୍ୟଥା
    last_col = columns - 1;

  ପାଇଁ (i = 1, p = column_vector; i <= last_col; ++i, ++p)
    p->ଧାଡ଼ିଗୁଡ଼ିକ_stored = 0;

  ପାଇଁ (i = 1, p = column_vector; i <= last_col && files_ready_to_read;
       ++i, ++p)
    {
      p->current_ଧାଡ଼ି = ଧାଡ଼ି;
      ପାଇଁ (j = ଧାଡ଼ିଗୁଡ଼ିକ_per_body; j && files_ready_to_read; --j)

        ଯଦି (p->status == OPEN)	/* Redundant.  Clean up. */
          {
            ନିବେଶ_position = 0;

            ଯଦି (!read_ଧାଡ଼ି (p))
              read_rest_of_ଧାଡ଼ି (p);

            ଯଦି (p->status == OPEN
                || buff_start != buff_current)
              {
                ++p->ଧାଡ଼ିଗୁଡ଼ିକ_stored;
                ଧାଡ଼ି_vector[ଧାଡ଼ି] = buff_start;
                end_vector[ଧାଡ଼ି++] = ନିବେଶ_position;
                buff_start = buff_current;
              }
          }
    }

  /* Keep track of the location of the last char in buff. */
  ଧାଡ଼ି_vector[ଧାଡ଼ି] = buff_start;

  ଯଦି (balance_columns)
    balance (ଧାଡ଼ି);
}

ସ୍ଥିର ଶୂନ୍ୟ
balance (ପୂର୍ଣ୍ଣସଂଖ୍ୟା total_stored)
{
  COLUMN *p;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, ଧାଡ଼ିଗୁଡ଼ିକ;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା first_ଧାଡ଼ି = 0;

  ପାଇଁ (i = 1, p = column_vector; i <= columns; ++i, ++p)
    {
      ଧାଡ଼ିଗୁଡ଼ିକ = total_stored / columns;
      ଯଦି (i <= total_stored % columns)
        ++ଧାଡ଼ିଗୁଡ଼ିକ;

      p->ଧାଡ଼ିଗୁଡ଼ିକ_stored = ଧାଡ଼ିଗୁଡ଼ିକ;
      p->current_ଧାଡ଼ି = first_ଧାଡ଼ି;

      first_ଧାଡ଼ି += ଧାଡ଼ିଗୁଡ଼ିକ;
    }
}

/* Store a character in the buffer. */

ସ୍ଥିର ଶୂନ୍ୟ
store_char (ଅକ୍ଷର c)
{
  ଯଦି (buff_current >= buff_allocated)
    {
      /* May be too generous. */
      buff = xpalloc (buff, &buff_allocated, 1, -1, ଆକାର *buff);
    }
  buff[buff_current++] = c;
}

ସ୍ଥିର ଶୂନ୍ୟ
add_ଧାଡ଼ି_ସଂଖ୍ୟା (COLUMN *p)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
  ଅକ୍ଷର *s;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_width;

  /* Cutting off the higher-order digits is more informative than
     lower-order cut off. */
  ଅଙ୍କ_width = sprintf (ସଂଖ୍ୟା_buff, "%*d", chars_per_ସଂଖ୍ୟା, ଧାଡ଼ି_ସଂଖ୍ୟା);
  ଧାଡ଼ି_ସଂଖ୍ୟା++;
  s = ସଂଖ୍ୟା_buff + (ଅଙ୍କ_width - chars_per_ସଂଖ୍ୟା);
  ପାଇଁ (i = chars_per_ସଂଖ୍ୟା; i > 0; i--)
    (p->char_func) (*s++);

  ଯଦି (columns > 1)
    {
      /* Tabification is assumed for multiple columns, also for n-separators,
         but 'default n-separator = TAB' hasn't been given priority over
         equal column_width also specified by POSIX. */
      ଯଦି (ସଂଖ୍ୟା_separator == '\t')
        {
          i = ସଂଖ୍ୟା_width - chars_per_ସଂଖ୍ୟା;
          ଯେତେବେଳେ (i-- > 0)
            (p->char_func) (' ');
        }
      ଅନ୍ୟଥା
        (p->char_func) (ସଂଖ୍ୟା_separator);
    }
  ଅନ୍ୟଥା
    /* To comply with POSIX, we avoid any expansion of default TAB
       separator with a single column output. No column_width requirement
       has to be considered. */
    {
      (p->char_func) (ସଂଖ୍ୟା_separator);
      ଯଦି (ସଂଖ୍ୟା_separator == '\t')
        output_position = POS_AFTER_TAB (chars_per_output_tab,
                          output_position);
    }

  ଯଦି (truncate_ଧାଡ଼ିଗୁଡ଼ିକ && !parallel_files)
    ନିବେଶ_position += ସଂଖ୍ୟା_width;
}

/* Print (or store) padding until the current horizontal position
   is position. */

ସ୍ଥିର ଶୂନ୍ୟ
pad_across_to (ପୂର୍ଣ୍ଣସଂଖ୍ୟା position)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା h = output_position;

  ଯଦି (tabify_output)
    spaces_not_printed = position - output_position;
  ଅନ୍ୟଥା
    {
      ଯେତେବେଳେ (++h <= position)
        putchar (' ');
      output_position = position;
    }
}

/* Pad to the bottom of the page.

   If the user has requested a formfeed, use one.
   Otherwise, use newlines. */

ସ୍ଥିର ଶୂନ୍ୟ
pad_down (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଧାଡ଼ିଗୁଡ଼ିକ)
{
  ଯଦି (use_form_feed)
    putchar ('\f');
  ଅନ୍ୟଥା
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = ଧାଡ଼ିଗୁଡ଼ିକ; i; --i)
      putchar ('\n');
}

/* Read the rest of the line.

   Read from the current column's file until an end of line is
   hit.  Used when we've truncated a line and we no longer need
   to print or store its characters. */

ସ୍ଥିର ଶୂନ୍ୟ
read_rest_of_ଧାଡ଼ି (COLUMN *p)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା c;
  FILE *f = p->fp;

  ଯେତେବେଳେ ((c = getc (f)) != '\n')
    {
      ଯଦି (c == '\f')
        {
          ଯଦି ((c = getc (f)) != '\n')
            ungetc (c, f);
          ଯଦି (keep_FF)
            print_a_FF = true;
          hold_file (p);
          ଭାଙ୍ଗ;
        }
      ଅନ୍ୟଥା ଯଦି (c == EOF)
        {
          close_file (p);
          ଭାଙ୍ଗ;
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

ସ୍ଥିର ଶୂନ୍ୟ
skip_read (COLUMN *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା column_ସଂଖ୍ୟା)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା c;
  FILE *f = p->fp;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
  bool single_ff = false;
  COLUMN *q;

  /* Read 1st character in a line or any character succeeding a FF */
  ଯଦି ((c = getc (f)) == '\f' && p->full_page_printed)
    /* A FF-coincidence with a previous full_page_printed.
       To avoid an additional empty page, eliminate the FF */
    ଯଦି ((c = getc (f)) == '\n')
      c = getc (f);

  p->full_page_printed = false;

  /* 1st character a FF means a single FF without any printable
     characters. Don't count it as a line with -n option. */
  ଯଦି (c == '\f')
    single_ff = true;

  /* Preparing for a FF-coincidence: Maybe we finish that page
     without a FF found */
  ଯଦି (last_ଧାଡ଼ି)
    p->full_page_printed = true;

  ଯେତେବେଳେ (c != '\n')
    {
      ଯଦି (c == '\f')
        {
          /* No FF-coincidence possible,
             no catching up of a FF-coincidence with next page */
          ଯଦି (last_ଧାଡ଼ି)
            {
              ଯଦି (!parallel_files)
                ପାଇଁ (q = column_vector, i = columns; i; ++q, --i)
                  q->full_page_printed = false;
              ଅନ୍ୟଥା
                p->full_page_printed = false;
            }

          ଯଦି ((c = getc (f)) != '\n')
            ungetc (c, f);
          hold_file (p);
          ଭାଙ୍ଗ;
        }
      ଅନ୍ୟଥା ଯଦି (c == EOF)
        {
          close_file (p);
          ଭାଙ୍ଗ;
        }
      c = getc (f);
    }

  ଯଦି (skip_ଗଣନା)
    ଯଦି ((!parallel_files || column_ସଂଖ୍ୟା == 1) && !single_ff)
      ++ଧାଡ଼ି_ଗଣନା;
}

/* If we're tabifying output,

   When print_char encounters white space it keeps track
   of our desired horizontal position and delays printing
   until this function is called. */

ସ୍ଥିର ଶୂନ୍ୟ
print_white_space (ଶୂନ୍ୟ)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା h_new;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା h_old = output_position;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା goal = h_old + spaces_not_printed;

  ଯେତେବେଳେ (goal - h_old > 1
         && (h_new = POS_AFTER_TAB (chars_per_output_tab, h_old)) <= goal)
    {
      putchar (output_tab_char);
      h_old = h_new;
    }
  ଯେତେବେଳେ (++h_old <= goal)
    putchar (' ');

  output_position = goal;
  spaces_not_printed = 0;
}

/* Print column separators.

   We keep a count until we know that we'll be printing a line,
   then print_sep_string() is called. */

ସ୍ଥିର ଶୂନ୍ୟ
print_sep_string (ଶୂନ୍ୟ)
{
  ଅକ୍ଷର ଅଚଳ *s = col_sep_string;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା l = col_sep_length;

  ଯଦି (separators_not_printed <= 0)
    {
      /* We'll be starting a line with chars_per_margin, anything else? */
      ଯଦି (spaces_not_printed > 0)
        print_white_space ();
    }
  ଅନ୍ୟଥା
    {
      ପାଇଁ (; separators_not_printed > 0; --separators_not_printed)
        {
          ଯେତେବେଳେ (l-- > 0)
            {
              /* 3 types of sep_strings: spaces only, spaces and chars,
              chars only */
              ଯଦି (*s == ' ')
                {
                  /* We're tabifying output; consecutive spaces in
                  sep_string may have to be converted to tabs */
                  s++;
                  ++spaces_not_printed;
                }
              ଅନ୍ୟଥା
                {
                  ଯଦି (spaces_not_printed > 0)
                    print_white_space ();
                  putchar (*s++);
                  ++output_position;
                }
            }
          /* sep_string ends with some spaces */
          ଯଦି (spaces_not_printed > 0)
            print_white_space ();
        }
    }
}

/* Print (or store, depending on p->char_func) a clump of N
   characters. */

ସ୍ଥିର ଶୂନ୍ୟ
print_clump (COLUMN *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଅକ୍ଷର *clump)
{
  ଯେତେବେଳେ (n--)
    (p->char_func) (*clump++);

  ଯଦି (ferror (stdout))
    write_error ();
}

/* Print a character.

   Update the following comment: process-char hasn't been used any
   longer.
   If we're tabifying, all tabs have been converted to spaces by
   process_char().  Keep a count of consecutive spaces, and when
   a nonspace is encountered, call print_white_space() to print the
   required number of tabs and spaces. */

ସ୍ଥିର ଶୂନ୍ୟ
print_char (ଅକ୍ଷର c)
{
  ଯଦି (tabify_output)
    {
      ଯଦି (c == ' ')
        {
          ++spaces_not_printed;
          ଫେରନ୍ତୁ;
        }
      ଅନ୍ୟଥା ଯଦି (spaces_not_printed > 0)
        print_white_space ();

      /* Nonprintables are assumed to have width 0, except '\b'. */
      ଯଦି (! isprint (to_uchar (c)))
        {
          ଯଦି (c == '\b')
            --output_position;
        }
      ଅନ୍ୟଥା
        ++output_position;
    }
  putchar (c);
}

/* Skip to page PAGE before printing.
   PAGE may be larger than total number of pages. */

ସ୍ଥିର bool
skip_to_page (uintmax_t page)
{
  ପାଇଁ (uintmax_t n = 1; n < page; ++n)
    {
      COLUMN *p;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା j;

      ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 1; i < ଧାଡ଼ିଗୁଡ଼ିକ_per_body; ++i)
        {
          ପାଇଁ (j = 1, p = column_vector; j <= columns; ++j, ++p)
            ଯଦି (p->status == OPEN)
              skip_read (p, j);
        }
      last_ଧାଡ଼ି = true;
      ପାଇଁ (j = 1, p = column_vector; j <= columns; ++j, ++p)
        ଯଦି (p->status == OPEN)
          skip_read (p, j);

      ଯଦି (storing_columns)	/* change FF_FOUND to ON_HOLD */
        ପାଇଁ (j = 1, p = column_vector; j <= columns; ++j, ++p)
          ଯଦି (p->status != CLOSED)
            p->status = ON_HOLD;

      reset_status ();
      last_ଧାଡ଼ି = false;

      ଯଦି (files_ready_to_read < 1)
        {
          /* It's very helpful, normally the total number of pages is
             not known in advance.  */
          error (0, 0,
                 _("starting page number %ju exceeds page count %ju"),
                 page, n);
          ଭାଙ୍ଗ;
        }
    }
  ଫେରନ୍ତୁ files_ready_to_read > 0;
}

/* Print a header.

   Formfeeds are assumed to use up two lines at the beginning of
   the page. */

ସ୍ଥିର ଶୂନ୍ୟ
print_header (ଶୂନ୍ୟ)
{
  ଅକ୍ଷର page_text[256 + INT_STRLEN_BOUND (page_ସଂଖ୍ୟା)];
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା available_width;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା lhs_spaces;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା rhs_spaces;

  output_position = 0;
  pad_across_to (chars_per_margin);
  print_white_space ();

  ଯଦି (page_ସଂଖ୍ୟା == 0)
    error (EXIT_FAILURE, 0, _("page number overflow"));

  /* The translator must ensure that formatting the translation of
     "Page %ju" does not generate more than (sizeof page_text - 1)
     bytes.  */
  sprintf (page_text, _("Page %ju"), page_ସଂଖ୍ୟା);
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

ସ୍ଥିର bool
read_ଧାଡ଼ି (COLUMN *p)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା c;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା chars IF_LINT (= 0);
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା last_ନିବେଶ_position;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା j, k;
  COLUMN *q;

  /* read 1st character in each line or any character succeeding a FF: */
  c = getc (p->fp);

  last_ନିବେଶ_position = ନିବେଶ_position;

  ଯଦି (c == '\f' && p->full_page_printed)
    ଯଦି ((c = getc (p->fp)) == '\n')
      c = getc (p->fp);
  p->full_page_printed = false;

  ବାଛ (c)
    {
    ସ୍ଥିତି '\f':
      ଯଦି ((c = getc (p->fp)) != '\n')
        ungetc (c, p->fp);
      FF_only = true;
      ଯଦି (print_a_header && !storing_columns)
        {
          pad_vertically = true;
          print_header ();
        }
      ଅନ୍ୟଥା ଯଦି (keep_FF)
        print_a_FF = true;
      hold_file (p);
      ଫେରନ୍ତୁ true;
    ସ୍ଥିତି EOF:
      close_file (p);
      ଫେରନ୍ତୁ true;
    ସ୍ଥିତି '\n':
      ଭାଙ୍ଗ;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      chars = char_to_clump (c);
    }

  ଯଦି (truncate_ଧାଡ଼ିଗୁଡ଼ିକ && ନିବେଶ_position > chars_per_column)
    {
      ନିବେଶ_position = last_ନିବେଶ_position;
      ଫେରନ୍ତୁ false;
    }

  ଯଦି (p->char_func != store_char)
    {
      pad_vertically = true;

      ଯଦି (print_a_header && !storing_columns)
        print_header ();

      ଯଦି (parallel_files && align_empty_cols)
        {
          /* We have to align empty columns at the beginning of a line. */
          k = separators_not_printed;
          separators_not_printed = 0;
          ପାଇଁ (j = 1, q = column_vector; j <= k; ++j, ++q)
            {
              align_column (q);
              separators_not_printed += 1;
            }
          padding_not_printed = p->start_position;
          ଯଦି (truncate_ଧାଡ଼ିଗୁଡ଼ିକ)
            spaces_not_printed = chars_per_column;
          ଅନ୍ୟଥା
            spaces_not_printed = 0;
          align_empty_cols = false;
        }

      ଯଦି (col_sep_length < padding_not_printed)
        {
          pad_across_to (padding_not_printed - col_sep_length);
          padding_not_printed = ANYWHERE;
        }

      ଯଦି (use_col_separator)
        print_sep_string ();
    }

  ଯଦି (p->numbered)
    add_ଧାଡ଼ି_ସଂଖ୍ୟା (p);

  empty_ଧାଡ଼ି = false;
  ଯଦି (c == '\n')
    ଫେରନ୍ତୁ true;

  print_clump (p, chars, clump_buff);

  ଯେତେବେଳେ (true)
    {
      c = getc (p->fp);

      ବାଛ (c)
        {
        ସ୍ଥିତି '\n':
          ଫେରନ୍ତୁ true;
        ସ୍ଥିତି '\f':
          ଯଦି ((c = getc (p->fp)) != '\n')
            ungetc (c, p->fp);
          ଯଦି (keep_FF)
            print_a_FF = true;
          hold_file (p);
          ଫେରନ୍ତୁ true;
        ସ୍ଥିତି EOF:
          close_file (p);
          ଫେରନ୍ତୁ true;
        }

      last_ନିବେଶ_position = ନିବେଶ_position;
      chars = char_to_clump (c);
      ଯଦି (truncate_ଧାଡ଼ିଗୁଡ଼ିକ && ନିବେଶ_position > chars_per_column)
        {
          ନିବେଶ_position = last_ନିବେଶ_position;
          ଫେରନ୍ତୁ false;
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

ସ୍ଥିର bool
print_stored (COLUMN *p)
{
  COLUMN *q;

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଧାଡ଼ି = p->current_ଧାଡ଼ି++;
  ଅକ୍ଷର *first = &buff[ଧାଡ଼ି_vector[ଧାଡ଼ି]];
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
  ଅକ୍ଷର *last = &buff[ଧାଡ଼ି_vector[ଧାଡ଼ି + 1]];

  pad_vertically = true;

  ଯଦି (print_a_header)
    print_header ();

  ଯଦି (p->status == FF_FOUND)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
      ପାଇଁ (i = 1, q = column_vector; i <= columns; ++i, ++q)
        q->status = ON_HOLD;
      ଯଦି (column_vector->ଧାଡ଼ିଗୁଡ଼ିକ_to_print <= 0)
        {
          ଯଦି (!extremities)
            pad_vertically = false;
          ଫେରନ୍ତୁ true;		/* print a header only */
        }
    }

  ଯଦି (col_sep_length < padding_not_printed)
    {
      pad_across_to (padding_not_printed - col_sep_length);
      padding_not_printed = ANYWHERE;
    }

  ଯଦି (use_col_separator)
    print_sep_string ();

  ଯେତେବେଳେ (first != last)
    print_char (*first++);

  ଯଦି (spaces_not_printed == 0)
    {
      output_position = p->start_position + end_vector[ଧାଡ଼ି];
      ଯଦି (p->start_position - col_sep_length == chars_per_margin)
        output_position -= col_sep_length;
    }

  ଫେରନ୍ତୁ true;
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

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
char_to_clump (ଅକ୍ଷର c)
{
  unsigned ଅକ୍ଷର uc = c;
  ଅକ୍ଷର *s = clump_buff;
  ଅକ୍ଷର esc_buff[4];
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା width;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା chars;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା chars_per_c = 8;

  ଯଦି (c == ନିବେଶ_tab_char)
    chars_per_c = chars_per_ନିବେଶ_tab;

  ଯଦି (c == ନିବେଶ_tab_char || c == '\t')
    {
      width = TAB_WIDTH (chars_per_c, ନିବେଶ_position);

      ଯଦି (untabify_ନିବେଶ)
        {
          ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = width; i; --i)
            *s++ = ' ';
          chars = width;
        }
      ଅନ୍ୟଥା
        {
          *s = c;
          chars = 1;
        }

    }
  ଅନ୍ୟଥା ଯଦି (! isprint (uc))
    {
      ଯଦି (use_esc_sequence)
        {
          width = 4;
          chars = 4;
          *s++ = '\\';
          sprintf (esc_buff, "%03o", uc);
          ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i <= 2; ++i)
            *s++ = esc_buff[i];
        }
      ଅନ୍ୟଥା ଯଦି (use_cntrl_prefix)
        {
          ଯଦି (uc < 0200)
            {
              width = 2;
              chars = 2;
              *s++ = '^';
              *s = c ^ 0100;
            }
          ଅନ୍ୟଥା
            {
              width = 4;
              chars = 4;
              *s++ = '\\';
              sprintf (esc_buff, "%03o", uc);
              ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i <= 2; ++i)
                *s++ = esc_buff[i];
            }
        }
      ଅନ୍ୟଥା ଯଦି (c == '\b')
        {
          width = -1;
          chars = 1;
          *s = c;
        }
      ଅନ୍ୟଥା
        {
          width = 0;
          chars = 1;
          *s = c;
        }
    }
  ଅନ୍ୟଥା
    {
      width = 1;
      chars = 1;
      *s = c;
    }

  /* Too many backspaces must put us in position 0 -- never negative.  */
  ଯଦି (width < 0 && ନିବେଶ_position == 0)
    {
      chars = 0;
      ନିବେଶ_position = 0;
    }
  ଅନ୍ୟଥା ଯଦି (width < 0 && ନିବେଶ_position <= -width)
    ନିବେଶ_position = 0;
  ଅନ୍ୟଥା
    {
      ଯଦି (ckd_add (&ନିବେଶ_position, ନିବେଶ_position, width))
        integer_overflow ();
    }

  ଫେରନ୍ତୁ chars;
}

/* We've just printed some files and need to clean up things before
   looking for more options and printing the next batch of files.

   Free everything we've xmalloc'ed, except 'header'. */

ସ୍ଥିର ଶୂନ୍ୟ
cleanup (ଶୂନ୍ୟ)
{
  ମୁକ୍ତ (ସଂଖ୍ୟା_buff);
  ମୁକ୍ତ (clump_buff);
  ମୁକ୍ତ (column_vector);
  ମୁକ୍ତ (ଧାଡ଼ି_vector);
  ମୁକ୍ତ (end_vector);
  ମୁକ୍ତ (buff);
}

/* Complain, print a usage message, and die. */

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              program_name);

      fputs (_("\
Paginate or columnate FILE(s) ପାଇଁ printing.\n\
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
         Balance ସଂଖ୍ୟା of ଧାଡ଼ିଗୁଡ଼ିକ in the columns on each page\n\
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
  -d, --ଦ୍ୱିଗୁଣ-space\n\
         ଦ୍ୱିଗୁଣ space the output\n\
"));
      oputs (_("\
  -D, --date-format=FORMAT\n\
         use FORMAT ପାଇଁ the header date\n\
"));
      oputs (_("\
  -e[CHAR[WIDTH]], --expand-tabs[=CHAR[WIDTH]]\n\
         expand ନିବେଶ CHARs (TABs) to tab WIDTH (8)\n\
"));
      oputs (_("\
  -F, -f, --form-feed\n\
         use form feeds instead of newlines to separate pages\n\
         (by a 3-ଧାଡ଼ି page header with -F or a 5-ଧାଡ଼ି header\n\
         and trailer without -F)\n\
"));
      oputs (_("\
  -h, --header=HEADER\n\
         use a centered HEADER instead of filename in page header,\n\
         -h \"\" prints a blank ଧାଡ଼ି, don't use -h\"\"\n\
"));
      oputs (_("\
  -i[CHAR[WIDTH]], --output-tabs[=CHAR[WIDTH]]\n\
         replace spaces with CHARs (TABs) to tab WIDTH (8)\n\
"));
      oputs (_("\
  -J, --join-ଧାଡ଼ିଗୁଡ଼ିକ\n\
         merge full ଧାଡ଼ିଗୁଡ଼ିକ, turns off -W ଧାଡ଼ି truncation,\n\
         no column alignment, --sep-string[=STRING] sets separators\n\
"));
      oputs (_("\
  -l, --length=PAGE_LENGTH\n\
         set the page length to PAGE_LENGTH (66) ଧାଡ଼ିଗୁଡ଼ିକ\n\
         (ପୂର୍ବନିର୍ଦ୍ଧାରିତ ସଂଖ୍ୟା of ଧାଡ଼ିଗୁଡ଼ିକ of text 56, and with -F 63).\n\
         implies -t ଯଦି PAGE_LENGTH <= 10\n\
"));
      oputs (_("\
  -m, --merge\n\
         print all files in parallel, one in each column,\n\
         truncate ଧାଡ଼ିଗୁଡ଼ିକ, but join ଧାଡ଼ିଗୁଡ଼ିକ of full length with -J\n\
"));
      oputs (_("\
  -n[SEP[DIGITS]], --ସଂଖ୍ୟା-ଧାଡ଼ିଗୁଡ଼ିକ[=SEP[DIGITS]]\n\
         ସଂଖ୍ୟା ଧାଡ଼ିଗୁଡ଼ିକ, use DIGITS (5) digits, then SEP (TAB),\n\
         ପୂର୍ବନିର୍ଦ୍ଧାରିତ counting starts with 1st ଧାଡ଼ି of ନିବେଶ file\n\
"));
      oputs (_("\
  -N, --first-ଧାଡ଼ି-ସଂଖ୍ୟା=ସଂଖ୍ୟା_2\n\
         start counting with ସଂଖ୍ୟା_2 at 1st ଧାଡ଼ି of first\n\
         page printed (see +FIRST_PAGE)\n\
"));
      oputs (_("\
  -o, --indent=MARGIN\n\
         offset each ଧାଡ଼ି with MARGIN (zero) spaces, କର not\n\
         affect -w or -W, MARGIN will be added to PAGE_WIDTH\n\
"));
      oputs (_("\
  -r, --no-file-warnings\n\
         omit warning when a file cannot be opened\n\
"));
      oputs (_("\
  -s[CHAR], --separator[=CHAR]\n\
         separate columns by a single character, ପୂର୍ବନିର୍ଦ୍ଧାରିତ ପାଇଁ CHAR\n\
         is the <TAB> character without -w and \'no ଅକ୍ଷର\' with -w.\n\
         -s[CHAR] turns off ଧାଡ଼ି truncation of all 3 column\n\
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
         omit page headers and trailers; implied ଯଦି PAGE_LENGTH <= 10\n\
"));
      oputs (_("\
  -T, --omit-pagination\n\
         omit page headers and trailers,\n\
         eliminate any pagination by form feeds set in ନିବେଶ files\n\
"));
      oputs (_("\
  -v, --show-nonprinting\n\
         use octal backslash notation\n\
"));
      oputs (_("\
  -w, --width=PAGE_WIDTH\n\
         set page width to PAGE_WIDTH (72) characters ପାଇଁ\n\
         multiple text-column output only, -s[ଅକ୍ଷର] turns off (72)\n\
"));
      oputs (_("\
  -W, --page-width=PAGE_WIDTH\n\
         set page width to PAGE_WIDTH (72) characters always,\n\
         truncate ଧାଡ଼ିଗୁଡ଼ିକ, except -J option is set,\n\
         no interference with -S or -s\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}
