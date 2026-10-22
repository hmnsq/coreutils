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
#परिभाषय क्रमादेशः_NAME "pr"

#परिभाषय AUTHORS \
  proper_name ("Pete TerMaat"), \
  proper_name ("Roland Huebner")

/* Used with start_position in the struct COLUMN described below.
   If start_position == ANYWHERE, we aren't truncating columns and
   can begin printing a column anywhere.  Otherwise we must pad to
   the horizontal position start_position. */
#परिभाषय ANYWHERE	0

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
        FF_प्राप्तम्,		/* used with -b option, set with \f, changed
                                   to ON_HOLD after print_header */
        ON_HOLD,		/* Hit a form feed. */
        CLOSED
      }
    स्थितिः;			/* Status of the file pointer. */

    /* Func to print lines in this col. */
    bool (*print_func) (संरचना COLUMN *);

    /* Func to print/store chars in this col. */
    शून्य (*char_func) (वर्ण);

    पूर्णाङ्क current_पङ्क्तिः;		/* Index of current place in line_vector. */
    पूर्णाङ्क पङ्क्तयः_stored;		/* Number of lines stored in buff. */
    पूर्णाङ्क पङ्क्तयः_गन्तव्यम्_print;		/* No. lines stored or space left on page. */
    पूर्णाङ्क start_position;		/* Horizontal position of first char. */
    bool numbered;
    bool पूर्णम्_page_printed;	/* True means printed without a FF found. */

    /* p->full_page_printed  controls a special case of "FF set by hand":
       True means a full page has been printed without FF found. To avoid an
       additional empty page we have to ignore a FF immediately following in
       the next line. */
  };

प्रकारनिर्देश संरचना COLUMN COLUMN;

स्थिर पूर्णाङ्क char_गन्तव्यम्_clump (वर्ण c);
स्थिर bool read_पङ्क्तिः (COLUMN *p);
स्थिर bool print_page (शून्य);
स्थिर bool print_stored (COLUMN *p);
स्थिर bool open_file (वर्ण *name, COLUMN *p);
स्थिर bool skip_गन्तव्यम्_page (uintmax_t page);
स्थिर शून्य print_शीर्षकम् (शून्य);
स्थिर शून्य pad_across_गन्तव्यम् (पूर्णाङ्क position);
स्थिर शून्य सङ्कलय_पङ्क्तिः_सङ्ख्या (COLUMN *p);
स्थिर पूर्णाङ्क getoptnum (वर्ण ध्रुव *n_str, पूर्णाङ्क min, वर्ण ध्रुव *errfmt);
स्थिर शून्य getoptarg (वर्ण *arg, वर्ण switch_char, वर्ण *character,
                       पूर्णाङ्क *सङ्ख्या);
स्थिर शून्य print_files (पूर्णाङ्क सङ्ख्या_सम्बन्धि_files, वर्ण **av);
स्थिर शून्य init_parameters (पूर्णाङ्क सङ्ख्या_सम्बन्धि_files);
स्थिर शून्य init_शीर्षकम् (वर्ण ध्रुव *filename, पूर्णाङ्क desc);
स्थिर bool init_fps (पूर्णाङ्क सङ्ख्या_सम्बन्धि_files, वर्ण **av);
स्थिर शून्य init_funcs (शून्य);
स्थिर शून्य init_store_स्तम्भसङ्ख्या (शून्य);
स्थिर शून्य store_columns (शून्य);
स्थिर शून्य balance (पूर्णाङ्क योग_stored);
स्थिर शून्य store_char (वर्ण c);
स्थिर शून्य pad_अधस्तात् (अचिह्नित पूर्णाङ्क पङ्क्तयः);
स्थिर शून्य read_rest_सम्बन्धि_पङ्क्तिः (COLUMN *p);
स्थिर शून्य skip_read (COLUMN *p, पूर्णाङ्क column_सङ्ख्या);
स्थिर शून्य print_char (वर्ण c);
स्थिर शून्य cleanup (शून्य);
स्थिर शून्य print_sep_string (शून्य);
स्थिर शून्य separator_string (वर्ण ध्रुव *optarg_S);

/* All of the columns to print.  */
स्थिर COLUMN *column_सदिशम्;

/* When printing a single file in multiple downward columns,
   we store the leftmost columns contiguously in buff.
   To print a line from buff, get the index of the first character
   from line_vector[i], and print up to line_vector[i + 1]. */
स्थिर वर्ण *buff;

/* Index of the position in buff where the next character
   will be stored. */
स्थिर अचिह्नित पूर्णाङ्क buff_current;

/* The number of characters in buff.
   Used for allocation of buff and to detect overflow of buff. */
स्थिर idx_t buff_allocated;

/* Array of indices into buff.
   Each entry is an index of the first character of a line.
   This is used when storing lines to facilitate shuffling when
   we do column balancing on the last page. */
स्थिर पूर्णाङ्क *पङ्क्तिः_सदिशम्;

/* Array of horizontal positions.
   For each line in line_vector, end_vector[line] is the horizontal
   position we are in after printing that line.  We keep track of this
   so that we know how much we need to pad to prepare for the next
   column. */
स्थिर पूर्णाङ्क *अन्तः_सदिशम्;

/* (-m) True means we're printing multiple files in parallel. */
स्थिर bool parallel_files = false;

/* (-m) True means a line starts with some empty columns (some files
   already CLOSED or ON_HOLD) which we have to align. */
स्थिर bool align_रिक्तानि_स्तम्भसङ्ख्या;

/* (-m) True means we have not yet found any printable column in a line.
   align_empty_cols = true  has to be maintained. */
स्थिर bool रिक्तानि_पङ्क्तिः;

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
स्थिर bool print_a_शीर्षकम्;

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
स्थिर पूर्णाङ्क पङ्क्तयः_प्रत्येकम्_page = 66;

/* Number of lines in the header and footer can be reset to 0 using
   the -t flag. */
गणना { पङ्क्तयः_प्रत्येकम्_शीर्षकम् = 5 };
स्थिर पूर्णाङ्क पङ्क्तयः_प्रत्येकम्_body;
गणना { पङ्क्तयः_प्रत्येकम्_footer = 5 };

/* (-w|-W) Width in characters of the page.  Does not include the width of
   the margin. */
स्थिर पूर्णाङ्क chars_प्रत्येकम्_पङ्क्तिः = 72;

/* (-w|W) True means we truncate lines longer than chars_per_column. */
स्थिर bool truncate_पङ्क्तयः = false;

/* (-J) True means we join lines without any line truncation. -J
   dominates -w option. */
स्थिर bool join_पङ्क्तयः = false;

/* Number of characters in a column.  Based on col_sep_length and
   page width. */
स्थिर पूर्णाङ्क chars_प्रत्येकम्_column;

/* (-e) True means convert tabs to spaces on input. */
स्थिर bool untabify_निवेशः = false;

/* (-e) The input tab character. */
स्थिर वर्ण निवेशः_tab_char = '\t';

/* (-e) Tabstops are at chars_per_tab, 2*chars_per_tab, 3*chars_per_tab, ...
   where the leftmost column is 1. */
स्थिर पूर्णाङ्क chars_प्रत्येकम्_निवेशः_tab = 8;

/* (-i) True means convert spaces to tabs on output. */
स्थिर bool tabify_output = false;

/* (-i) The output tab character. */
स्थिर वर्ण output_tab_char = '\t';

/* (-i) The width of the output tab. */
स्थिर पूर्णाङ्क chars_प्रत्येकम्_output_tab = 8;

/* Keeps track of pending white space.  When we hit a nonspace
   character after some whitespace, we print whitespace, tabbing
   if necessary to get to output_position + spaces_not_printed. */
स्थिर पूर्णाङ्क spaces_not_printed;

/* (-o) Number of spaces in the left margin (tabs used when possible). */
स्थिर पूर्णाङ्क chars_प्रत्येकम्_margin = 0;

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
#परिभाषय TAB_विस्तारः(c_, h_) ((c_) - ((h_) % (c_)))

/* The horizontal position we'll be at after printing a tab character
   of width c_ from the position h_. */
#परिभाषय POS_AFTER_TAB(c_, h_) ((h_) + TAB_विस्तारः (c_, h_))

/* (-NNN) Number of columns of text to print. */
स्थिर पूर्णाङ्क columns = 1;

/* (+NNN:MMM) Page numbers on which to begin and stop printing.
   first_page_number = 0  will be used to check input only. */
स्थिर uintmax_t first_page_सङ्ख्या = 0;
स्थिर uintmax_t चरमः_page_सङ्ख्या = UINTMAX_MAX;

/* Number of files open (not closed, not on hold). */
स्थिर पूर्णाङ्क files_ready_गन्तव्यम्_read = 0;

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
स्थिर पूर्णाङ्क chars_प्रत्येकम्_सङ्ख्या = 5;

/* Used when widening the first column to accommodate numbers -- only
   needed when printing files in parallel.  Includes width of both the
   number and the number_separator. */
स्थिर पूर्णाङ्क सङ्ख्या_विस्तारः;

/* Buffer sprintf uses to format a line number. */
स्थिर वर्ण *सङ्ख्या_buff;

/* (-v) True means nonprintable characters are printed as escape sequences.
   control-g becomes \007. */
स्थिर bool use_esc_sequence = false;

/* (-c) True means nonprintable characters are printed as control prefixes.
   control-g becomes ^G. */
स्थिर bool use_cntrl_उपसर्गः = false;

/* (-d) True means output is double spaced. */
स्थिर bool double_space = false;

/* Number of files opened initially in init_files.  Should be 1
   unless we're printing multiple files in parallel. */
स्थिर पूर्णाङ्क योग_files = 0;

/* (-r) True means don't complain if we can't open a file. */
स्थिर bool ignore_failed_opens = false;

/* (-S) True means we separate columns with a specified string.
   -S option does not affect line truncation nor column alignment. */
स्थिर bool use_col_separator = false;

/* String used to separate columns if the -S option has been specified.
   Default without -S but together with one of the column options
   -a|COLUMN|-m is a 'space' and with the -J option a 'tab'. */
स्थिर वर्ण ध्रुव *col_sep_string = "";
स्थिर पूर्णाङ्क col_sep_दीर्घता = 0;
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
स्थिर वर्ण *custom_शीर्षकम्;

/* (-D) Date format for the header.  */
स्थिर वर्ण ध्रुव *date_format;

/* The local time zone rules, as per the TZ environment variable.  */
स्थिर timezone_t localtz;

/* Date and file name for the header.  */
स्थिर वर्ण *date_text;
स्थिर वर्ण ध्रुव *file_text;

/* Output columns available, not counting the date and file name.  */
स्थिर पूर्णाङ्क शीर्षकम्_विस्तारः_उपलब्धम्;

स्थिर वर्ण *clump_buff;

/* True means we read the line no. lines_per_body in skip_read
   called by skip_to_page. That variable controls the coincidence of a
   "FF set by hand" and "full_page_printed", see above the definition of
   structure COLUMN. */
स्थिर bool चरमः_पङ्क्तिः = false;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
गणना
{
  COLUMNS_OPTION = CHAR_MAX + 1,
  पृष्ठानि_OPTION
};

स्थिर वर्ण ध्रुव short_options[] =
  "-0123456789D:FJN:S::TW:abcde::fh:i::l:mn::o:rs::tvw:";

स्थिर संरचना option ध्रुव long_options[] =
{
  {"pages", required_तर्कः, रिक्त, पृष्ठानि_OPTION},
  {"columns", required_तर्कः, रिक्त, COLUMNS_OPTION},
  {"across", no_तर्कः, रिक्त, 'a'},
  {"show-control-chars", no_तर्कः, रिक्त, 'c'},
  {"double-space", no_तर्कः, रिक्त, 'd'},
  {"date-format", required_तर्कः, रिक्त, 'D'},
  {"expand-tabs", optional_तर्कः, रिक्त, 'e'},
  {"form-feed", no_तर्कः, रिक्त, 'f'},
  {"header", required_तर्कः, रिक्त, 'h'},
  {"output-tabs", optional_तर्कः, रिक्त, 'i'},
  {"join-lines", no_तर्कः, रिक्त, 'J'},
  {"length", required_तर्कः, रिक्त, 'l'},
  {"merge", no_तर्कः, रिक्त, 'm'},
  {"number-lines", optional_तर्कः, रिक्त, 'n'},
  {"first-line-number", required_तर्कः, रिक्त, 'N'},
  {"indent", required_तर्कः, रिक्त, 'o'},
  {"no-file-warnings", no_तर्कः, रिक्त, 'r'},
  {"separator", optional_तर्कः, रिक्त, 's'},
  {"sep-string", optional_तर्कः, रिक्त, 'S'},
  {"omit-header", no_तर्कः, रिक्त, 't'},
  {"omit-pagination", no_तर्कः, रिक्त, 'T'},
  {"show-nonprinting", no_तर्कः, रिक्त, 'v'},
  {"width", required_तर्कः, रिक्त, 'w'},
  {"page-width", required_तर्कः, रिक्त, 'W'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_संस्करणम्_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

स्थिर _Noreturn शून्य
integer_अतिप्रवाहः (शून्य)
{
  error (EXIT_FAILURE, 0, _("integer overflow"));
}

/* Return the number of columns that have either an open file or
   stored lines. */

ATTRIBUTE_विशुद्धम्
स्थिर अचिह्नित पूर्णाङ्क
स्तम्भसङ्ख्या_ready_गन्तव्यम्_print (शून्य)
{
  COLUMN *q;
  अचिह्नित पूर्णाङ्क i;
  अचिह्नित पूर्णाङ्क n;

  n = 0;
  प्रति (q = column_सदिशम्, i = 0; i < columns; ++q, ++i)
    यदि (q->स्थितिः == OPEN
        || q->स्थितिः == FF_प्राप्तम्	/* With -b: To print a header only */
        || (storing_columns && q->पङ्क्तयः_stored > 0 && q->पङ्क्तयः_गन्तव्यम्_print > 0))
      ++n;
  प्रत्यागम n;
}

/* Estimate first_ / last_page_number
   using option +FIRST_PAGE:LAST_PAGE */

स्थिर bool
first_चरमः_page (पूर्णाङ्क oi, वर्ण c, वर्ण ध्रुव *पृष्ठानि)
{
  वर्ण *p;
  uintmax_t first;
  uintmax_t चरमः = UINTMAX_MAX;
  strtol_error err = xstrtoumax (पृष्ठानि, &p, 10, &first, "");
  यदि (err != LONGINT_OK && err != LONGINT_INVALID_SUFFIX_CHAR)
    xstrtol_fatal (err, oi, c, long_options, पृष्ठानि);

  यदि (p == पृष्ठानि || !first)
    प्रत्यागम false;

  यदि (*p == ':')
    {
      वर्ण ध्रुव *p1 = p + 1;
      err = xstrtoumax (p1, &p, 10, &चरमः, "");
      यदि (err != LONGINT_OK)
        xstrtol_fatal (err, oi, c, long_options, पृष्ठानि);
      यदि (p1 == p || चरमः < first)
        प्रत्यागम false;
    }

  यदि (*p)
    प्रत्यागम false;

  first_page_सङ्ख्या = first;
  चरमः_page_सङ्ख्या = चरमः;
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
  size_t len = वाक्यदैर्घ्यम् (optarg_S);
  यदि (अधिकतम_पूर्णाङ्कः < len)
    integer_अतिप्रवाहः ();
  col_sep_दीर्घता = len;
  col_sep_string = optarg_S;
}

पूर्णाङ्क
मुख्यम् (पूर्णाङ्क argc, वर्ण **argv)
{
  अचिह्नित पूर्णाङ्क n_files;
  bool old_options = false;
  bool old_w = false;
  bool old_s = false;
  वर्ण **file_नामानि;

  /* Accumulate the digits of old-style options like -99.  */
  वर्ण *column_गणनम्_string = रिक्त;
  idx_t n_digits = 0;
  idx_t n_alloc = 0;

  initialize_main (&argc, &argv);
  समुच्चयः_क्रमादेशः_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  n_files = 0;
  file_नामानि = (argc > 1
                ? xnmalloc (argc - 1, परिमाण (वर्ण *))
                : रिक्त);

  यावत् (true)
    {
      पूर्णाङ्क oi = -1;
      पूर्णाङ्क c = getopt_long (argc, argv, short_options, long_options, &oi);
      यदि (c == -1)
        भङ्ग;

      यदि (c_isdigit (c))
        {
          /* Accumulate column-count digits specified via old-style options. */
          यदि (n_digits + 1 >= n_alloc)
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
          यदि (! (first_page_सङ्ख्या == 0
                 && *optarg == '+' && first_चरमः_page (-2, '+', optarg + 1)))
            file_नामानि[n_files++] = optarg;
          भङ्ग;

        पक्ष पृष्ठानि_OPTION:	/* --pages=FIRST_PAGE[:LAST_PAGE] */
          {			/* dominates old opt +... */
            यदि (! optarg)
              error (EXIT_FAILURE, 0,
                     _("'--pages=FIRST_PAGE[:LAST_PAGE]' missing argument"));
            अन्यथा यदि (! first_चरमः_page (oi, 0, optarg))
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
          use_cntrl_उपसर्गः = true;
          भङ्ग;
        पक्ष 'd':
          double_space = true;
          भङ्ग;
        पक्ष 'D':
          date_format = optarg;
          भङ्ग;
        पक्ष 'e':
          यदि (optarg)
            getoptarg (optarg, 'e', &निवेशः_tab_char,
                       &chars_प्रत्येकम्_निवेशः_tab);
          /* Could check tab width > 0. */
          untabify_निवेशः = true;
          भङ्ग;
        पक्ष 'f':
        पक्ष 'F':
          use_form_feed = true;
          भङ्ग;
        पक्ष 'h':
          custom_शीर्षकम् = optarg;
          भङ्ग;
        पक्ष 'i':
          यदि (optarg)
            getoptarg (optarg, 'i', &output_tab_char,
                       &chars_प्रत्येकम्_output_tab);
          /* Could check tab width > 0. */
          tabify_output = true;
          भङ्ग;
        पक्ष 'J':
          join_पङ्क्तयः = true;
          भङ्ग;
        पक्ष 'l':
          पङ्क्तयः_प्रत्येकम्_page
            = getoptnum (optarg, 1,
                         _("'-l PAGE_LENGTH' invalid number of lines"));
          भङ्ग;
        पक्ष 'm':
          parallel_files = true;
          storing_columns = false;
          भङ्ग;
        पक्ष 'n':
          numbered_पङ्क्तयः = true;
          यदि (optarg)
            getoptarg (optarg, 'n', &सङ्ख्या_separator,
                       &chars_प्रत्येकम्_सङ्ख्या);
          भङ्ग;
        पक्ष 'N':
          skip_गणनम् = false;
          start_पङ्क्तिः_अङ्कः
            = getoptnum (optarg, INT_MIN,
                         _("'-N NUMBER' invalid starting line number"));
          भङ्ग;
        पक्ष 'o':
          chars_प्रत्येकम्_margin = getoptnum (optarg, 0,
                                        _("'-o MARGIN' invalid line offset"));
          भङ्ग;
        पक्ष 'r':
          ignore_failed_opens = true;
          भङ्ग;
        पक्ष 's':
          old_options = true;
          old_s = true;
          यदि (!use_col_separator && optarg)
            separator_string (optarg);
          भङ्ग;
        पक्ष 'S':
          old_s = false;
          /* Reset an additional input of -s, -S dominates -s */
          col_sep_string = "";
          col_sep_दीर्घता = 0;
          use_col_separator = true;
          यदि (optarg)
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
            यदि (! truncate_पङ्क्तयः)
              chars_प्रत्येकम्_पङ्क्तिः = tmp_cpl;
          }
          भङ्ग;
        पक्ष 'W':
          old_w = false;			/* dominates -w */
          truncate_पङ्क्तयः = true;
          chars_प्रत्येकम्_पङ्क्तिः
            = getoptnum (optarg, 1,
                         _("'-W PAGE_WIDTH' invalid number of characters"));
          भङ्ग;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_संस्करणम्_CHAR (क्रमादेशः_NAME, AUTHORS);
        स्वभाव:
          usage (EXIT_FAILURE);
          भङ्ग;
        }
    }

  यदि (column_गणनम्_string)
    {
      parse_column_गणनम् (column_गणनम्_string);
      मुक्त (column_गणनम्_string);
    }

  यदि (! date_format)
    date_format = (getenv ("POSIXLY_CORRECT") && !hard_locale (LC_TIME)
                   ? "%b %e %H:%M %Y"
                   : "%Y-%m-%d %H:%M");

  localtz = tzalloc (getenv ("TZ"));

  /* Now we can set a reasonable initial value: */
  यदि (first_page_सङ्ख्या == 0)
    first_page_सङ्ख्या = 1;

  यदि (parallel_files && explicit_columns)
    error (EXIT_FAILURE, 0,
           _("cannot specify number of columns when printing in parallel"));

  यदि (parallel_files && print_across_flag)
    error (EXIT_FAILURE, 0,
           _("cannot specify both printing across and printing in parallel"));

/* Translate some old short options to new/long options.
   To meet downward compatibility with other UNIX pr utilities
   and some POSIX specifications. */

  यदि (old_options)
    {
      यदि (old_w)
        {
          यदि (parallel_files || explicit_columns)
            {
              /* activate -W */
              truncate_पङ्क्तयः = true;
              यदि (old_s)
                /* adapt HP-UX and SunOS: -s = no separator;
                   activate -S */
                use_col_separator = true;
            }
          अन्यथा
            /* old -w sets width with columns only
               activate -J */
            join_पङ्क्तयः = true;
        }
      अन्यथा यदि (!use_col_separator)
        {
          /* No -S option read */
          यदि (old_s && (parallel_files || explicit_columns))
            {
              यदि (!truncate_पङ्क्तयः)
                {
                  /* old -s (without -w and -W) annuls column alignment,
                  uses fields, activate -J */
                  join_पङ्क्तयः = true;
                  यदि (col_sep_दीर्घता > 0)
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

  प्रति (; optind < argc; optind++)
    {
      file_नामानि[n_files++] = argv[optind];
    }

  यदि (n_files == 0)
    {
      /* No file arguments specified;  read from standard input.  */
      print_files (0, रिक्त);
    }
  अन्यथा
    {
      यदि (parallel_files)
        print_files (n_files, file_नामानि);
      अन्यथा
        {
          प्रति (अचिह्नित पूर्णाङ्क i = 0; i < n_files; i++)
            print_files (1, &file_नामानि[i]);
        }
    }

  cleanup ();

  यदि (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, _("standard input"));
  main_exit (failed_opens ? EXIT_FAILURE : EXIT_SUCCESS);
}

/* Parse numeric arguments, ensuring MIN <= number <= INT_MAX.  */

स्थिर पूर्णाङ्क
getoptnum (वर्ण ध्रुव *n_str, पूर्णाङ्क min, वर्ण ध्रुव *err)
{
  प्रत्यागम xnumtoimax (n_str, 10, min, अधिकतम_पूर्णाङ्कः, "", err, 0,
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

  यदि (!*अङ्कः)
    {
      error (0, 0, _("'-%c': Invalid argument: %s"), switch_char, quote (अङ्कः));
      usage (EXIT_FAILURE);
    }

  यदि (!c_isdigit (*अङ्कः))
    *character = *अङ्कः++;
  यदि (*अङ्कः)
    {
      दीर्घ पूर्णाङ्क tmp_long;
      strtol_error e = xstrtol (अङ्कः, रिक्त, 10, &tmp_long, "");
      यदि (e == LONGINT_OK)
        {
          यदि (tmp_long <= 0)
            e = LONGINT_INVALID;
          अन्यथा यदि (अधिकतम_पूर्णाङ्कः < tmp_long)
            e = LONGINT_अतिप्रवाहः;
        }
      यदि (e != LONGINT_OK)
        {
          error (0, e & LONGINT_अतिप्रवाहः ? EOVERFLOW : 0,
             _("'-%c' extra characters or invalid number in the argument: %s"),
                 switch_char, quote (arg));
          usage (EXIT_FAILURE);
        }
      *सङ्ख्या = tmp_long;
    }
}

/* Set parameters related to formatting. */

स्थिर शून्य
init_parameters (पूर्णाङ्क सङ्ख्या_सम्बन्धि_files)
{
  पूर्णाङ्क chars_प्रयुक्तम्_by_सङ्ख्या = 0;

  पङ्क्तयः_प्रत्येकम्_body = पङ्क्तयः_प्रत्येकम्_page - पङ्क्तयः_प्रत्येकम्_शीर्षकम् - पङ्क्तयः_प्रत्येकम्_footer;
  यदि (पङ्क्तयः_प्रत्येकम्_body <= 0)
    {
      extremities = false;
      keep_FF = true;
    }
  यदि (! extremities)
    पङ्क्तयः_प्रत्येकम्_body = पङ्क्तयः_प्रत्येकम्_page;

  यदि (double_space)
    पङ्क्तयः_प्रत्येकम्_body = MAX (1, पङ्क्तयः_प्रत्येकम्_body / 2);

  /* If input is stdin, cannot print parallel files.  BSD dumps core
     on this. */
  यदि (सङ्ख्या_सम्बन्धि_files == 0)
    parallel_files = false;

  यदि (parallel_files)
    columns = सङ्ख्या_सम्बन्धि_files;

  /* One file, multi columns down: -b option is set to get a consistent
     formulation with "FF set by hand" in input files. */
  यदि (storing_columns)
    balance_columns = true;

  /* Tabification is assumed for multiple columns. */
  यदि (columns > 1)
    {
      यदि (!use_col_separator)
        {
          /* Use default separator */
          यदि (join_पङ्क्तयः)
            col_sep_string = पङ्क्तिः_separator;
          अन्यथा
            col_sep_string = column_separator;

          col_sep_दीर्घता = 1;
          use_col_separator = true;
        }
      /* It's rather pointless to define a TAB separator with column
         alignment */
      अन्यथा यदि (!join_पङ्क्तयः && col_sep_दीर्घता == 1 && *col_sep_string == '\t')
        col_sep_string = column_separator;

      truncate_पङ्क्तयः = true;
      यदि (! (col_sep_दीर्घता == 1 && *col_sep_string == '\t'))
        untabify_निवेशः = true;
      tabify_output = true;
    }
  अन्यथा
    storing_columns = false;

  /* -J dominates -w in any case */
  यदि (join_पङ्क्तयः)
    truncate_पङ्क्तयः = false;

  यदि (numbered_पङ्क्तयः)
    {
      पूर्णाङ्क chars_प्रत्येकम्_default_tab = 8;

      पङ्क्तिः_गणनम् = start_पङ्क्तिः_अङ्कः;

      /* To allow input tab-expansion (-e sensitive) use:
         if (number_separator == input_tab_char)
           number_width = chars_per_number
             + TAB_WIDTH (chars_per_input_tab, chars_per_number);   */

      /* Estimate chars_per_text without any margin and keep it constant. */
      यदि (सङ्ख्या_separator == '\t')
        सङ्ख्या_विस्तारः = (chars_प्रत्येकम्_सङ्ख्या
                        + TAB_विस्तारः (chars_प्रत्येकम्_default_tab, chars_प्रत्येकम्_सङ्ख्या));
      अन्यथा
        सङ्ख्या_विस्तारः = chars_प्रत्येकम्_सङ्ख्या + 1;

      /* The number is part of the column width unless we are
         printing files in parallel. */
      यदि (parallel_files)
        chars_प्रयुक्तम्_by_सङ्ख्या = सङ्ख्या_विस्तारः;
    }

  पूर्णाङ्क sep_chars, useful_chars;
  यदि (ckd_mul (&sep_chars, columns - 1, col_sep_दीर्घता))
    sep_chars = अधिकतम_पूर्णाङ्कः;
  यदि (ckd_sub (&useful_chars, chars_प्रत्येकम्_पङ्क्तिः - chars_प्रयुक्तम्_by_सङ्ख्या,
               sep_chars))
    useful_chars = 0;
  chars_प्रत्येकम्_column = useful_chars / columns;

  यदि (chars_प्रत्येकम्_column < 1)
    error (EXIT_FAILURE, 0, _("page width too narrow"));

  यदि (numbered_पङ्क्तयः)
    {
      मुक्त (सङ्ख्या_buff);
      सङ्ख्या_buff = xmalloc (MAX (chars_प्रत्येकम्_सङ्ख्या,
                                  INT_STRLEN_BOUND (पङ्क्तिः_सङ्ख्या)) + 1);
    }

  /* Pick the maximum between the tab width and the width of an
     escape sequence.
     The width of an escape sequence (4) isn't the lower limit any longer.
     We've to use 8 as the lower limit, if we use chars_per_default_tab = 8
     to expand a tab which is not an input_tab-char. */
  मुक्त (clump_buff);
  clump_buff = xmalloc (MAX (8, chars_प्रत्येकम्_निवेशः_tab));
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
init_fps (पूर्णाङ्क सङ्ख्या_सम्बन्धि_files, वर्ण **av)
{
  COLUMN *p;

  योग_files = 0;

  मुक्त (column_सदिशम्);
  column_सदिशम् = xnmalloc (columns, परिमाण (COLUMN));

  यदि (parallel_files)
    {
      पूर्णाङ्क files_वामः = सङ्ख्या_सम्बन्धि_files;
      प्रति (p = column_सदिशम्; files_वामः--; ++p, ++av)
        {
          यदि (! open_file (*av, p))
            {
              --p;
              --columns;
            }
        }
      यदि (columns == 0)
        प्रत्यागम false;
      init_शीर्षकम् ("", -1);
    }
  अन्यथा
    {
      p = column_सदिशम्;
      यदि (सङ्ख्या_सम्बन्धि_files > 0)
        {
          यदि (! open_file (*av, p))
            प्रत्यागम false;
          init_शीर्षकम् (*av, fileno (p->fp));
          p->पङ्क्तयः_stored = 0;
        }
      अन्यथा
        {
          p->name = _("standard input");
          p->fp = stdin;
          have_read_stdin = true;
          p->स्थितिः = OPEN;
          p->पूर्णम्_page_printed = false;
          ++योग_files;
          init_शीर्षकम् ("", -1);
          p->पङ्क्तयः_stored = 0;
        }

      वर्ण ध्रुव *firstname = p->name;
      FILE *firstfp = p->fp;
      पूर्णाङ्क i;
      प्रति (i = columns - 1, ++p; i; --i, ++p)
        {
          p->name = firstname;
          p->fp = firstfp;
          p->स्थितिः = OPEN;
          p->पूर्णम्_page_printed = false;
          p->पङ्क्तयः_stored = 0;
        }
    }
  files_ready_गन्तव्यम्_read = योग_files;
  प्रत्यागम true;
}

/* Determine print_func and char_func, the functions
   used by each column for printing and/or storing.

   Determine the horizontal position desired when we begin
   printing a column (p->start_position). */

स्थिर शून्य
init_funcs (शून्य)
{
  पूर्णाङ्क i, h, h_अग्र;
  COLUMN *p;

  h = chars_प्रत्येकम्_margin;

  यदि (!truncate_पङ्क्तयः)
    h_अग्र = ANYWHERE;
  अन्यथा
    {
      /* When numbering lines of parallel files, we enlarge the
         first column to accommodate the number.  Looks better than
         the Sys V approach. */
      यदि (parallel_files && numbered_पङ्क्तयः)
        h_अग्र = h + chars_प्रत्येकम्_column + सङ्ख्या_विस्तारः;
      अन्यथा
        h_अग्र = h + chars_प्रत्येकम्_column;
    }

  /* Enlarge p->start_position of first column to use the same form of
     padding_not_printed with all columns. */
  h = h + col_sep_दीर्घता;

  /* This loop takes care of all but the rightmost column. */

  प्रति (p = column_सदिशम्, i = 1; i < columns; ++p, ++i)
    {
      यदि (storing_columns)	/* One file, multi columns down. */
        {
          p->char_func = store_char;
          p->print_func = print_stored;
        }
      अन्यथा
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

      यदि (!truncate_पङ्क्तयः)
        {
          h = ANYWHERE;
          h_अग्र = ANYWHERE;
        }
      अन्यथा
        {
          h = h_अग्र + col_sep_दीर्घता;
          h_अग्र = h + chars_प्रत्येकम्_column;
        }
    }

  /* The rightmost column.

     Doesn't need to be stored unless we intend to balance
     columns on the last page. */
  यदि (storing_columns && balance_columns)
    {
      p->char_func = store_char;
      p->print_func = print_stored;
    }
  अन्यथा
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
  यदि (streq (name, "-"))
    {
      p->name = _("standard input");
      p->fp = stdin;
      have_read_stdin = true;
    }
  अन्यथा
    {
      p->name = name;
      p->fp = fopen (name, "r");
    }
  यदि (p->fp == रिक्त)
    {
      failed_opens = true;
      यदि (!ignore_failed_opens)
        error (0, errno, "%s", quotef (name));
      प्रत्यागम false;
    }
  fadvise (p->fp, FADVISE_SEQUENTIAL);
  p->स्थितिः = OPEN;
  p->पूर्णम्_page_printed = false;
  ++योग_files;
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

  यदि (p->स्थितिः == CLOSED)
    प्रत्यागम;

  पूर्णाङ्क err = errno;
  यदि (!ferror (p->fp))
    err = 0;
  यदि (fileno (p->fp) == STDIN_FILENO)
    clearerr (p->fp);
  अन्यथा यदि (fclose (p->fp) != 0 && !err)
    err = errno;
  यदि (err)
    error (EXIT_FAILURE, err, "%s", quotef (p->name));

  यदि (!parallel_files)
    {
      प्रति (q = column_सदिशम्, i = columns; i; ++q, --i)
        {
          q->स्थितिः = CLOSED;
          यदि (q->पङ्क्तयः_stored == 0)
            {
              q->पङ्क्तयः_गन्तव्यम्_print = 0;
            }
        }
    }
  अन्यथा
    {
      p->स्थितिः = CLOSED;
      p->पङ्क्तयः_गन्तव्यम्_print = 0;
    }

  --files_ready_गन्तव्यम्_read;
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

  यदि (!parallel_files)
    प्रति (q = column_सदिशम्, i = columns; i; ++q, --i)
      {
        यदि (storing_columns)
          q->स्थितिः = FF_प्राप्तम्;
        अन्यथा
          q->स्थितिः = ON_HOLD;
      }
  अन्यथा
    p->स्थितिः = ON_HOLD;

  p->पङ्क्तयः_गन्तव्यम्_print = 0;
  --files_ready_गन्तव्यम्_read;
}

/* Undo hold_file -- go through the column list and change any
   ON_HOLD columns to OPEN.  Used at the end of each page. */

स्थिर शून्य
reset_स्थितिः (शून्य)
{
  पूर्णाङ्क i = columns;
  COLUMN *p;

  प्रति (p = column_सदिशम्; i; --i, ++p)
    यदि (p->स्थितिः == ON_HOLD)
      {
        p->स्थितिः = OPEN;
        files_ready_गन्तव्यम्_read++;
      }

  यदि (storing_columns)
    {
      यदि (column_सदिशम्->स्थितिः == CLOSED)
        /* We use the info to output an error message in  skip_to_page. */
        files_ready_गन्तव्यम्_read = 0;
      अन्यथा
        files_ready_गन्तव्यम्_read = 1;
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
print_files (पूर्णाङ्क सङ्ख्या_सम्बन्धि_files, वर्ण **av)
{
  init_parameters (सङ्ख्या_सम्बन्धि_files);
  यदि (! init_fps (सङ्ख्या_सम्बन्धि_files, av))
    प्रत्यागम;
  यदि (storing_columns)
    init_store_स्तम्भसङ्ख्या ();

  यदि (first_page_सङ्ख्या > 1)
    {
      यदि (!skip_गन्तव्यम्_page (first_page_सङ्ख्या))
        प्रत्यागम;
      अन्यथा
        page_सङ्ख्या = first_page_सङ्ख्या;
    }
  अन्यथा
    page_सङ्ख्या = 1;

  init_funcs ();

  पङ्क्तिः_सङ्ख्या = पङ्क्तिः_गणनम्;
  यावत् (print_page ())
    ;
}

/* Initialize header information.
   If DESC is non-negative, it is a file descriptor open to
   FILENAME for reading.  */

स्थिर शून्य
init_शीर्षकम् (वर्ण ध्रुव *filename, पूर्णाङ्क desc)
{
  वर्ण *buf = रिक्त;
  संरचना stat st;
  संरचना timespec t;
  पूर्णाङ्क ns;
  संरचना tm tm;

  /* If parallel files or standard input, use current date. */
  यदि (streq (filename, "-"))
    desc = -1;
  यदि (0 <= desc && fstat (desc, &st) == 0)
    t = get_stat_mtime (&st);
  अन्यथा
    {
      स्थिर संरचना timespec timespec;
      यदि (! timespec.tv_sec)
        gettime (&timespec);
      t = timespec;
    }

  ns = t.tv_nsec;
  यदि (localtime_rz (localtz, &t.tv_sec, &tm))
    {
      ptrdiff_t len = nstrftime (रिक्त, MIN (PTRDIFF_MAX, SIZE_MAX),
                                 date_format, &tm, localtz, ns);
      यदि (0 <= len)
        {
          buf = ximalloc (len + 1);
          nstrftime (buf, len + 1, date_format, &tm, localtz, ns);
        }
    }
  यदि (!buf)
    {
      वर्ण secbuf[INT_BUFSIZE_BOUND (intmax_t)];
      buf = xmalloc (परिमाण secbuf + MAX (10, INT_BUFSIZE_BOUND (पूर्णाङ्क)));
      sprintf (buf, "%s.%09d", timetostr (t.tv_sec, secbuf), ns);
    }

  मुक्त (date_text);
  date_text = buf;
  file_text = custom_शीर्षकम् ? custom_शीर्षकम् : desc < 0 ? "" : filename;
  शीर्षकम्_विस्तारः_उपलब्धम् = (chars_प्रत्येकम्_पङ्क्तिः
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

  यदि (storing_columns)
    {
      store_columns ();
      प्रति (j = columns - 1, p = column_सदिशम्; j; --j, ++p)
        {
          p->पङ्क्तयः_गन्तव्यम्_print = p->पङ्क्तयः_stored;
        }

      /* Last column. */
      यदि (balance_columns)
        {
          p->पङ्क्तयः_गन्तव्यम्_print = p->पङ्क्तयः_stored;
        }
      /* Since we're not balancing columns, we don't need to store
         the rightmost column.   Read it straight from the file. */
      अन्यथा
        {
          यदि (p->स्थितिः == OPEN)
            {
              p->पङ्क्तयः_गन्तव्यम्_print = पङ्क्तयः_प्रत्येकम्_body;
            }
          अन्यथा
            p->पङ्क्तयः_गन्तव्यम्_print = 0;
        }
    }
  अन्यथा
    प्रति (j = columns, p = column_सदिशम्; j; --j, ++p)
      यदि (p->स्थितिः == OPEN)
        {
          p->पङ्क्तयः_गन्तव्यम्_print = पङ्क्तयः_प्रत्येकम्_body;
        }
      अन्यथा
        p->पङ्क्तयः_गन्तव्यम्_print = 0;
}

/* Align empty columns and print separators.
   Empty columns will be formed by files with status ON_HOLD or CLOSED
   when printing multiple files in parallel. */

स्थिर शून्य
align_column (COLUMN *p)
{
  padding_not_printed = p->start_position;
  यदि (col_sep_दीर्घता < padding_not_printed)
    {
      pad_across_गन्तव्यम् (padding_not_printed - col_sep_दीर्घता);
      padding_not_printed = ANYWHERE;
    }

  यदि (use_col_separator)
    print_sep_string ();

  यदि (p->numbered)
    सङ्कलय_पङ्क्तिः_सङ्ख्या (p);
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

  यदि (स्तम्भसङ्ख्या_ready_गन्तव्यम्_print () == 0)
    प्रत्यागम false;

  यदि (extremities)
    print_a_शीर्षकम् = true;

  /* Don't pad unless we know a page was printed. */
  pad_vertically = false;
  pv = false;

  पङ्क्तयः_वामः_on_page = पङ्क्तयः_प्रत्येकम्_body;
  यदि (double_space)
    पङ्क्तयः_वामः_on_page *= 2;

  यावत् (पङ्क्तयः_वामः_on_page > 0 && स्तम्भसङ्ख्या_ready_गन्तव्यम्_print () > 0)
    {
      output_position = 0;
      spaces_not_printed = 0;
      separators_not_printed = 0;
      pad_vertically = false;
      align_रिक्तानि_स्तम्भसङ्ख्या = false;
      रिक्तानि_पङ्क्तिः = true;

      प्रति (j = 1, p = column_सदिशम्; j <= columns; ++j, ++p)
        {
          निवेशः_position = 0;
          यदि (p->पङ्क्तयः_गन्तव्यम्_print > 0 || p->स्थितिः == FF_प्राप्तम्)
            {
              FF_only = false;
              padding_not_printed = p->start_position;
              यदि (!(p->print_func) (p))
                read_rest_सम्बन्धि_पङ्क्तिः (p);
              pv |= pad_vertically;

              --p->पङ्क्तयः_गन्तव्यम्_print;
              यदि (p->पङ्क्तयः_गन्तव्यम्_print <= 0)
                {
                  यदि (स्तम्भसङ्ख्या_ready_गन्तव्यम्_print () == 0)
                    भङ्ग;
                }

              /* File p changed its status to ON_HOLD or CLOSED */
              यदि (parallel_files && p->स्थितिः != OPEN)
                {
                  यदि (रिक्तानि_पङ्क्तिः)
                    align_रिक्तानि_स्तम्भसङ्ख्या = true;
                  अन्यथा यदि (p->स्थितिः == CLOSED
                           || (p->स्थितिः == ON_HOLD && FF_only))
                    align_column (p);
                }
            }
          अन्यथा यदि (parallel_files)
            {
              /* File status ON_HOLD or CLOSED */
              यदि (रिक्तानि_पङ्क्तिः)
                align_रिक्तानि_स्तम्भसङ्ख्या = true;
              अन्यथा
                align_column (p);
            }

          /* We need it also with an empty column */
          यदि (use_col_separator)
            ++separators_not_printed;
        }

      यदि (pad_vertically)
        {
          अक्षरं_लिख ('\n');
          --पङ्क्तयः_वामः_on_page;
        }

      यदि (स्तम्भसङ्ख्या_ready_गन्तव्यम्_print () == 0 && !extremities)
        भङ्ग;

      यदि (double_space && pv)
        {
          अक्षरं_लिख ('\n');
          --पङ्क्तयः_वामः_on_page;
        }
    }

  यदि (पङ्क्तयः_वामः_on_page == 0)
    प्रति (j = 1, p = column_सदिशम्; j <= columns; ++j, ++p)
      यदि (p->स्थितिः == OPEN)
        p->पूर्णम्_page_printed = true;

  pad_vertically = pv;

  यदि (pad_vertically && extremities)
    pad_अधस्तात् (पङ्क्तयः_वामः_on_page + पङ्क्तयः_प्रत्येकम्_footer);
  अन्यथा यदि (keep_FF && print_a_FF)
    {
      अक्षरं_लिख ('\f');
      print_a_FF = false;
    }

  यदि (ferror (stdout))
    write_error ();

  यदि (चरमः_page_सङ्ख्या < ++page_सङ्ख्या)
    प्रत्यागम false;		/* Stop printing with LAST_PAGE */

  reset_स्थितिः ();		/* Change ON_HOLD to OPEN. */

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
init_store_स्तम्भसङ्ख्या (शून्य)
{
  /* Tune this.  */
  पूर्णाङ्क योग_पङ्क्तयः, योग_पङ्क्तयः_1, chars_प्रत्येकम्_column_1, chars_if_truncate;
  यदि (ckd_mul (&योग_पङ्क्तयः, पङ्क्तयः_प्रत्येकम्_body, columns)
      || ckd_सङ्कलय (&योग_पङ्क्तयः_1, योग_पङ्क्तयः, 1)
      || ckd_सङ्कलय (&chars_प्रत्येकम्_column_1, chars_प्रत्येकम्_column, 1)
      || ckd_mul (&chars_if_truncate, योग_पङ्क्तयः, chars_प्रत्येकम्_column_1)
      || ckd_mul (&buff_allocated, chars_if_truncate, use_col_separator + 1))
    integer_अतिप्रवाहः ();

  मुक्त (पङ्क्तिः_सदिशम्);
  /* FIXME: here's where it was allocated.  */
  पङ्क्तिः_सदिशम् = xnmalloc (योग_पङ्क्तयः_1, परिमाण *पङ्क्तिः_सदिशम्);

  मुक्त (अन्तः_सदिशम्);
  अन्तः_सदिशम् = xnmalloc (योग_पङ्क्तयः, परिमाण *अन्तः_सदिशम्);

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
  अचिह्नित पूर्णाङ्क पङ्क्तिः = 0;
  अचिह्नित पूर्णाङ्क buff_start;
  पूर्णाङ्क चरमः_col;		/* The rightmost column which will be saved in buff */
  COLUMN *p;

  buff_current = 0;
  buff_start = 0;

  यदि (balance_columns)
    चरमः_col = columns;
  अन्यथा
    चरमः_col = columns - 1;

  प्रति (i = 1, p = column_सदिशम्; i <= चरमः_col; ++i, ++p)
    p->पङ्क्तयः_stored = 0;

  प्रति (i = 1, p = column_सदिशम्; i <= चरमः_col && files_ready_गन्तव्यम्_read;
       ++i, ++p)
    {
      p->current_पङ्क्तिः = पङ्क्तिः;
      प्रति (j = पङ्क्तयः_प्रत्येकम्_body; j && files_ready_गन्तव्यम्_read; --j)

        यदि (p->स्थितिः == OPEN)	/* Redundant.  Clean up. */
          {
            निवेशः_position = 0;

            यदि (!read_पङ्क्तिः (p))
              read_rest_सम्बन्धि_पङ्क्तिः (p);

            यदि (p->स्थितिः == OPEN
                || buff_start != buff_current)
              {
                ++p->पङ्क्तयः_stored;
                पङ्क्तिः_सदिशम्[पङ्क्तिः] = buff_start;
                अन्तः_सदिशम्[पङ्क्तिः++] = निवेशः_position;
                buff_start = buff_current;
              }
          }
    }

  /* Keep track of the location of the last char in buff. */
  पङ्क्तिः_सदिशम्[पङ्क्तिः] = buff_start;

  यदि (balance_columns)
    balance (पङ्क्तिः);
}

स्थिर शून्य
balance (पूर्णाङ्क योग_stored)
{
  COLUMN *p;
  पूर्णाङ्क i, पङ्क्तयः;
  पूर्णाङ्क first_पङ्क्तिः = 0;

  प्रति (i = 1, p = column_सदिशम्; i <= columns; ++i, ++p)
    {
      पङ्क्तयः = योग_stored / columns;
      यदि (i <= योग_stored % columns)
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
  यदि (buff_current >= buff_allocated)
    {
      /* May be too generous. */
      buff = xpalloc (buff, &buff_allocated, 1, -1, परिमाण *buff);
    }
  buff[buff_current++] = c;
}

स्थिर शून्य
सङ्कलय_पङ्क्तिः_सङ्ख्या (COLUMN *p)
{
  पूर्णाङ्क i;
  वर्ण *s;
  पूर्णाङ्क अङ्कः_विस्तारः;

  /* Cutting off the higher-order digits is more informative than
     lower-order cut off. */
  अङ्कः_विस्तारः = sprintf (सङ्ख्या_buff, "%*d", chars_प्रत्येकम्_सङ्ख्या, पङ्क्तिः_सङ्ख्या);
  पङ्क्तिः_सङ्ख्या++;
  s = सङ्ख्या_buff + (अङ्कः_विस्तारः - chars_प्रत्येकम्_सङ्ख्या);
  प्रति (i = chars_प्रत्येकम्_सङ्ख्या; i > 0; i--)
    (p->char_func) (*s++);

  यदि (columns > 1)
    {
      /* Tabification is assumed for multiple columns, also for n-separators,
         but 'default n-separator = TAB' hasn't been given priority over
         equal column_width also specified by POSIX. */
      यदि (सङ्ख्या_separator == '\t')
        {
          i = सङ्ख्या_विस्तारः - chars_प्रत्येकम्_सङ्ख्या;
          यावत् (i-- > 0)
            (p->char_func) (' ');
        }
      अन्यथा
        (p->char_func) (सङ्ख्या_separator);
    }
  अन्यथा
    /* To comply with POSIX, we avoid any expansion of default TAB
       separator with a single column output. No column_width requirement
       has to be considered. */
    {
      (p->char_func) (सङ्ख्या_separator);
      यदि (सङ्ख्या_separator == '\t')
        output_position = POS_AFTER_TAB (chars_प्रत्येकम्_output_tab,
                          output_position);
    }

  यदि (truncate_पङ्क्तयः && !parallel_files)
    निवेशः_position += सङ्ख्या_विस्तारः;
}

/* Print (or store) padding until the current horizontal position
   is position. */

स्थिर शून्य
pad_across_गन्तव्यम् (पूर्णाङ्क position)
{
  पूर्णाङ्क h = output_position;

  यदि (tabify_output)
    spaces_not_printed = position - output_position;
  अन्यथा
    {
      यावत् (++h <= position)
        अक्षरं_लिख (' ');
      output_position = position;
    }
}

/* Pad to the bottom of the page.

   If the user has requested a formfeed, use one.
   Otherwise, use newlines. */

स्थिर शून्य
pad_अधस्तात् (अचिह्नित पूर्णाङ्क पङ्क्तयः)
{
  यदि (use_form_feed)
    अक्षरं_लिख ('\f');
  अन्यथा
    प्रति (अचिह्नित पूर्णाङ्क i = पङ्क्तयः; i; --i)
      अक्षरं_लिख ('\n');
}

/* Read the rest of the line.

   Read from the current column's file until an end of line is
   hit.  Used when we've truncated a line and we no longer need
   to print or store its characters. */

स्थिर शून्य
read_rest_सम्बन्धि_पङ्क्तिः (COLUMN *p)
{
  पूर्णाङ्क c;
  FILE *f = p->fp;

  यावत् ((c = getc (f)) != '\n')
    {
      यदि (c == '\f')
        {
          यदि ((c = getc (f)) != '\n')
            ungetc (c, f);
          यदि (keep_FF)
            print_a_FF = true;
          hold_file (p);
          भङ्ग;
        }
      अन्यथा यदि (c == EOF)
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
  यदि ((c = getc (f)) == '\f' && p->पूर्णम्_page_printed)
    /* A FF-coincidence with a previous full_page_printed.
       To avoid an additional empty page, eliminate the FF */
    यदि ((c = getc (f)) == '\n')
      c = getc (f);

  p->पूर्णम्_page_printed = false;

  /* 1st character a FF means a single FF without any printable
     characters. Don't count it as a line with -n option. */
  यदि (c == '\f')
    single_ff = true;

  /* Preparing for a FF-coincidence: Maybe we finish that page
     without a FF found */
  यदि (चरमः_पङ्क्तिः)
    p->पूर्णम्_page_printed = true;

  यावत् (c != '\n')
    {
      यदि (c == '\f')
        {
          /* No FF-coincidence possible,
             no catching up of a FF-coincidence with next page */
          यदि (चरमः_पङ्क्तिः)
            {
              यदि (!parallel_files)
                प्रति (q = column_सदिशम्, i = columns; i; ++q, --i)
                  q->पूर्णम्_page_printed = false;
              अन्यथा
                p->पूर्णम्_page_printed = false;
            }

          यदि ((c = getc (f)) != '\n')
            ungetc (c, f);
          hold_file (p);
          भङ्ग;
        }
      अन्यथा यदि (c == EOF)
        {
          close_file (p);
          भङ्ग;
        }
      c = getc (f);
    }

  यदि (skip_गणनम्)
    यदि ((!parallel_files || column_सङ्ख्या == 1) && !single_ff)
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

  यावत् (goal - h_old > 1
         && (h_new = POS_AFTER_TAB (chars_प्रत्येकम्_output_tab, h_old)) <= goal)
    {
      अक्षरं_लिख (output_tab_char);
      h_old = h_new;
    }
  यावत् (++h_old <= goal)
    अक्षरं_लिख (' ');

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
  पूर्णाङ्क l = col_sep_दीर्घता;

  यदि (separators_not_printed <= 0)
    {
      /* We'll be starting a line with chars_per_margin, anything else? */
      यदि (spaces_not_printed > 0)
        print_white_space ();
    }
  अन्यथा
    {
      प्रति (; separators_not_printed > 0; --separators_not_printed)
        {
          यावत् (l-- > 0)
            {
              /* 3 types of sep_strings: spaces only, spaces and chars,
              chars only */
              यदि (*s == ' ')
                {
                  /* We're tabifying output; consecutive spaces in
                  sep_string may have to be converted to tabs */
                  s++;
                  ++spaces_not_printed;
                }
              अन्यथा
                {
                  यदि (spaces_not_printed > 0)
                    print_white_space ();
                  अक्षरं_लिख (*s++);
                  ++output_position;
                }
            }
          /* sep_string ends with some spaces */
          यदि (spaces_not_printed > 0)
            print_white_space ();
        }
    }
}

/* Print (or store, depending on p->char_func) a clump of N
   characters. */

स्थिर शून्य
print_clump (COLUMN *p, पूर्णाङ्क n, वर्ण *clump)
{
  यावत् (n--)
    (p->char_func) (*clump++);

  यदि (ferror (stdout))
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
  यदि (tabify_output)
    {
      यदि (c == ' ')
        {
          ++spaces_not_printed;
          प्रत्यागम;
        }
      अन्यथा यदि (spaces_not_printed > 0)
        print_white_space ();

      /* Nonprintables are assumed to have width 0, except '\b'. */
      यदि (! isprint (गन्तव्यम्_uchar (c)))
        {
          यदि (c == '\b')
            --output_position;
        }
      अन्यथा
        ++output_position;
    }
  अक्षरं_लिख (c);
}

/* Skip to page PAGE before printing.
   PAGE may be larger than total number of pages. */

स्थिर bool
skip_गन्तव्यम्_page (uintmax_t page)
{
  प्रति (uintmax_t n = 1; n < page; ++n)
    {
      COLUMN *p;
      पूर्णाङ्क j;

      प्रति (पूर्णाङ्क i = 1; i < पङ्क्तयः_प्रत्येकम्_body; ++i)
        {
          प्रति (j = 1, p = column_सदिशम्; j <= columns; ++j, ++p)
            यदि (p->स्थितिः == OPEN)
              skip_read (p, j);
        }
      चरमः_पङ्क्तिः = true;
      प्रति (j = 1, p = column_सदिशम्; j <= columns; ++j, ++p)
        यदि (p->स्थितिः == OPEN)
          skip_read (p, j);

      यदि (storing_columns)	/* change FF_FOUND to ON_HOLD */
        प्रति (j = 1, p = column_सदिशम्; j <= columns; ++j, ++p)
          यदि (p->स्थितिः != CLOSED)
            p->स्थितिः = ON_HOLD;

      reset_स्थितिः ();
      चरमः_पङ्क्तिः = false;

      यदि (files_ready_गन्तव्यम्_read < 1)
        {
          /* It's very helpful, normally the total number of pages is
             not known in advance.  */
          error (0, 0,
                 _("starting page number %ju exceeds page count %ju"),
                 page, n);
          भङ्ग;
        }
    }
  प्रत्यागम files_ready_गन्तव्यम्_read > 0;
}

/* Print a header.

   Formfeeds are assumed to use up two lines at the beginning of
   the page. */

स्थिर शून्य
print_शीर्षकम् (शून्य)
{
  वर्ण page_text[256 + INT_STRLEN_BOUND (page_सङ्ख्या)];
  पूर्णाङ्क उपलब्धम्_विस्तारः;
  पूर्णाङ्क lhs_spaces;
  पूर्णाङ्क rhs_spaces;

  output_position = 0;
  pad_across_गन्तव्यम् (chars_प्रत्येकम्_margin);
  print_white_space ();

  यदि (page_सङ्ख्या == 0)
    error (EXIT_FAILURE, 0, _("page number overflow"));

  /* The translator must ensure that formatting the translation of
     "Page %ju" does not generate more than (sizeof page_text - 1)
     bytes.  */
  sprintf (page_text, _("Page %ju"), page_सङ्ख्या);
  उपलब्धम्_विस्तारः = शीर्षकम्_विस्तारः_उपलब्धम् - mbswidth (page_text, 0);
  उपलब्धम्_विस्तारः = MAX (0, उपलब्धम्_विस्तारः);
  lhs_spaces = उपलब्धम्_विस्तारः >> 1;
  rhs_spaces = उपलब्धम्_विस्तारः - lhs_spaces;

  प्रारूप_लिख ("\n\n%*s%s%*s%s%*s%s\n\n\n",
          chars_प्रत्येकम्_margin, "",
          date_text, lhs_spaces, " ",
          file_text, rhs_spaces, " ", page_text);

  print_a_शीर्षकम् = false;
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
  पूर्णाङ्क चरमः_निवेशः_position;
  पूर्णाङ्क j, k;
  COLUMN *q;

  /* read 1st character in each line or any character succeeding a FF: */
  c = getc (p->fp);

  चरमः_निवेशः_position = निवेशः_position;

  यदि (c == '\f' && p->पूर्णम्_page_printed)
    यदि ((c = getc (p->fp)) == '\n')
      c = getc (p->fp);
  p->पूर्णम्_page_printed = false;

  विकल्प (c)
    {
    पक्ष '\f':
      यदि ((c = getc (p->fp)) != '\n')
        ungetc (c, p->fp);
      FF_only = true;
      यदि (print_a_शीर्षकम् && !storing_columns)
        {
          pad_vertically = true;
          print_शीर्षकम् ();
        }
      अन्यथा यदि (keep_FF)
        print_a_FF = true;
      hold_file (p);
      प्रत्यागम true;
    पक्ष EOF:
      close_file (p);
      प्रत्यागम true;
    पक्ष '\n':
      भङ्ग;
    स्वभाव:
      chars = char_गन्तव्यम्_clump (c);
    }

  यदि (truncate_पङ्क्तयः && निवेशः_position > chars_प्रत्येकम्_column)
    {
      निवेशः_position = चरमः_निवेशः_position;
      प्रत्यागम false;
    }

  यदि (p->char_func != store_char)
    {
      pad_vertically = true;

      यदि (print_a_शीर्षकम् && !storing_columns)
        print_शीर्षकम् ();

      यदि (parallel_files && align_रिक्तानि_स्तम्भसङ्ख्या)
        {
          /* We have to align empty columns at the beginning of a line. */
          k = separators_not_printed;
          separators_not_printed = 0;
          प्रति (j = 1, q = column_सदिशम्; j <= k; ++j, ++q)
            {
              align_column (q);
              separators_not_printed += 1;
            }
          padding_not_printed = p->start_position;
          यदि (truncate_पङ्क्तयः)
            spaces_not_printed = chars_प्रत्येकम्_column;
          अन्यथा
            spaces_not_printed = 0;
          align_रिक्तानि_स्तम्भसङ्ख्या = false;
        }

      यदि (col_sep_दीर्घता < padding_not_printed)
        {
          pad_across_गन्तव्यम् (padding_not_printed - col_sep_दीर्घता);
          padding_not_printed = ANYWHERE;
        }

      यदि (use_col_separator)
        print_sep_string ();
    }

  यदि (p->numbered)
    सङ्कलय_पङ्क्तिः_सङ्ख्या (p);

  रिक्तानि_पङ्क्तिः = false;
  यदि (c == '\n')
    प्रत्यागम true;

  print_clump (p, chars, clump_buff);

  यावत् (true)
    {
      c = getc (p->fp);

      विकल्प (c)
        {
        पक्ष '\n':
          प्रत्यागम true;
        पक्ष '\f':
          यदि ((c = getc (p->fp)) != '\n')
            ungetc (c, p->fp);
          यदि (keep_FF)
            print_a_FF = true;
          hold_file (p);
          प्रत्यागम true;
        पक्ष EOF:
          close_file (p);
          प्रत्यागम true;
        }

      चरमः_निवेशः_position = निवेशः_position;
      chars = char_गन्तव्यम्_clump (c);
      यदि (truncate_पङ्क्तयः && निवेशः_position > chars_प्रत्येकम्_column)
        {
          निवेशः_position = चरमः_निवेशः_position;
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
  वर्ण *first = &buff[पङ्क्तिः_सदिशम्[पङ्क्तिः]];
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
  वर्ण *चरमः = &buff[पङ्क्तिः_सदिशम्[पङ्क्तिः + 1]];

  pad_vertically = true;

  यदि (print_a_शीर्षकम्)
    print_शीर्षकम् ();

  यदि (p->स्थितिः == FF_प्राप्तम्)
    {
      पूर्णाङ्क i;
      प्रति (i = 1, q = column_सदिशम्; i <= columns; ++i, ++q)
        q->स्थितिः = ON_HOLD;
      यदि (column_सदिशम्->पङ्क्तयः_गन्तव्यम्_print <= 0)
        {
          यदि (!extremities)
            pad_vertically = false;
          प्रत्यागम true;		/* print a header only */
        }
    }

  यदि (col_sep_दीर्घता < padding_not_printed)
    {
      pad_across_गन्तव्यम् (padding_not_printed - col_sep_दीर्घता);
      padding_not_printed = ANYWHERE;
    }

  यदि (use_col_separator)
    print_sep_string ();

  यावत् (first != चरमः)
    print_char (*first++);

  यदि (spaces_not_printed == 0)
    {
      output_position = p->start_position + अन्तः_सदिशम्[पङ्क्तिः];
      यदि (p->start_position - col_sep_दीर्घता == chars_प्रत्येकम्_margin)
        output_position -= col_sep_दीर्घता;
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
char_गन्तव्यम्_clump (वर्ण c)
{
  अचिह्नित वर्ण uc = c;
  वर्ण *s = clump_buff;
  वर्ण esc_buff[4];
  पूर्णाङ्क विस्तारः;
  पूर्णाङ्क chars;
  पूर्णाङ्क chars_प्रत्येकम्_c = 8;

  यदि (c == निवेशः_tab_char)
    chars_प्रत्येकम्_c = chars_प्रत्येकम्_निवेशः_tab;

  यदि (c == निवेशः_tab_char || c == '\t')
    {
      विस्तारः = TAB_विस्तारः (chars_प्रत्येकम्_c, निवेशः_position);

      यदि (untabify_निवेशः)
        {
          प्रति (पूर्णाङ्क i = विस्तारः; i; --i)
            *s++ = ' ';
          chars = विस्तारः;
        }
      अन्यथा
        {
          *s = c;
          chars = 1;
        }

    }
  अन्यथा यदि (! isprint (uc))
    {
      यदि (use_esc_sequence)
        {
          विस्तारः = 4;
          chars = 4;
          *s++ = '\\';
          sprintf (esc_buff, "%03o", uc);
          प्रति (पूर्णाङ्क i = 0; i <= 2; ++i)
            *s++ = esc_buff[i];
        }
      अन्यथा यदि (use_cntrl_उपसर्गः)
        {
          यदि (uc < 0200)
            {
              विस्तारः = 2;
              chars = 2;
              *s++ = '^';
              *s = c ^ 0100;
            }
          अन्यथा
            {
              विस्तारः = 4;
              chars = 4;
              *s++ = '\\';
              sprintf (esc_buff, "%03o", uc);
              प्रति (पूर्णाङ्क i = 0; i <= 2; ++i)
                *s++ = esc_buff[i];
            }
        }
      अन्यथा यदि (c == '\b')
        {
          विस्तारः = -1;
          chars = 1;
          *s = c;
        }
      अन्यथा
        {
          विस्तारः = 0;
          chars = 1;
          *s = c;
        }
    }
  अन्यथा
    {
      विस्तारः = 1;
      chars = 1;
      *s = c;
    }

  /* Too many backspaces must put us in position 0 -- never negative.  */
  यदि (विस्तारः < 0 && निवेशः_position == 0)
    {
      chars = 0;
      निवेशः_position = 0;
    }
  अन्यथा यदि (विस्तारः < 0 && निवेशः_position <= -विस्तारः)
    निवेशः_position = 0;
  अन्यथा
    {
      यदि (ckd_सङ्कलय (&निवेशः_position, निवेशः_position, विस्तारः))
        integer_अतिप्रवाहः ();
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
  मुक्त (column_सदिशम्);
  मुक्त (पङ्क्तिः_सदिशम्);
  मुक्त (अन्तः_सदिशम्);
  मुक्त (buff);
}

/* Complain, print a usage message, and die. */

शून्य
usage (पूर्णाङ्क स्थितिः)
{
  यदि (स्थितिः != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
    {
      प्रारूप_लिख (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              क्रमादेशः_name);

      fputs (_("\
Paginate or columnate FILE(s) प्रति printing.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\n\
  +FIRST_PAGE[:चरमः_PAGE], --पृष्ठानि=FIRST_PAGE[:चरमः_PAGE]\n\
         begin [stop] printing with page FIRST_[चरमः_2]PAGE\n\
"));
      oputs (_("\
  -स्तम्भसङ्ख्या, --columns=स्तम्भसङ्ख्या\n\
         output स्तम्भसङ्ख्या columns and print columns अधस्तात्, unless -a is प्रयुक्तम्.\n\
         Balance सङ्ख्या सम्बन्धि पङ्क्तयः अन्तर् the columns on each page\n\
"));
      oputs (_("\
  -a, --across\n\
         print columns across rather than अधस्तात्, प्रयुक्तम् together with -स्तम्भसङ्ख्या\n\
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
         use FORMAT प्रति the शीर्षकम् date\n\
"));
      oputs (_("\
  -e[CHAR[विस्तारः_2]], --expand-tabs[=CHAR[विस्तारः_2]]\n\
         expand निवेशः CHARs (TABs) गन्तव्यम् tab विस्तारः_2 (8)\n\
"));
      oputs (_("\
  -F, -f, --form-feed\n\
         use form feeds instead सम्बन्धि newlines गन्तव्यम् separate पृष्ठानि\n\
         (by a 3-पङ्क्तिः page शीर्षकम् with -F or a 5-पङ्क्तिः शीर्षकम्\n\
         and trailer without -F)\n\
"));
      oputs (_("\
  -h, --शीर्षकम्=शीर्षकम्_3\n\
         use a centered शीर्षकम्_3 instead सम्बन्धि filename अन्तर् page शीर्षकम्,\n\
         -h \"\" prints a blank पङ्क्तिः, don't use -h\"\"\n\
"));
      oputs (_("\
  -i[CHAR[विस्तारः_2]], --output-tabs[=CHAR[विस्तारः_2]]\n\
         replace spaces with CHARs (TABs) गन्तव्यम् tab विस्तारः_2 (8)\n\
"));
      oputs (_("\
  -J, --join-पङ्क्तयः\n\
         merge पूर्णम् पङ्क्तयः, turns off -W पङ्क्तिः truncation,\n\
         no column alignment, --sep-string[=STRING] sets separators\n\
"));
      oputs (_("\
  -l, --दीर्घता=PAGE_दीर्घता\n\
         समुच्चयः_2 the page दीर्घता गन्तव्यम् PAGE_दीर्घता (66) पङ्क्तयः\n\
         (स्वभाव सङ्ख्या सम्बन्धि पङ्क्तयः सम्बन्धि text 56, and with -F 63).\n\
         उपपादयति -t यदि PAGE_दीर्घता <= 10\n\
"));
      oputs (_("\
  -m, --merge\n\
         print all files अन्तर् parallel, one अन्तर् each column,\n\
         truncate पङ्क्तयः, but join पङ्क्तयः सम्बन्धि पूर्णम् दीर्घता with -J\n\
"));
      oputs (_("\
  -n[SEP[DIGITS]], --सङ्ख्या-पङ्क्तयः[=SEP[DIGITS]]\n\
         सङ्ख्या पङ्क्तयः, use DIGITS (5) digits, then SEP (TAB),\n\
         स्वभाव counting starts with 1st पङ्क्तिः सम्बन्धि निवेशः file\n\
"));
      oputs (_("\
  -N, --first-पङ्क्तिः-सङ्ख्या=सङ्ख्या_2\n\
         start counting with सङ्ख्या_2 at 1st पङ्क्तिः सम्बन्धि first\n\
         page printed (see +FIRST_PAGE)\n\
"));
      oputs (_("\
  -o, --indent=MARGIN\n\
         विस्थापनम् each पङ्क्तिः with MARGIN (zero) spaces, कुरु not\n\
         affect -w or -W, MARGIN will be added गन्तव्यम् PAGE_विस्तारः\n\
"));
      oputs (_("\
  -r, --no-file-warnings\n\
         omit warning when a file cannot be opened\n\
"));
      oputs (_("\
  -s[CHAR], --separator[=CHAR]\n\
         separate columns by a single character, स्वभाव प्रति CHAR\n\
         is the <TAB> character without -w and \'no वर्ण\' with -w.\n\
         -s[CHAR] turns off पङ्क्तिः truncation सम्बन्धि all 3 column\n\
         options (-स्तम्भसङ्ख्या|-a -स्तम्भसङ्ख्या|-m) except -w is समुच्चयः_2\n\
"));
      oputs (_("\
  -S[STRING], --sep-string[=STRING]\n\
         separate columns by STRING,\n\
         without -S: Default separator <TAB> with -J and <space>\n\
         otherwise (same as -S\" \"), no effect on column options\n\
"));
      oputs (_("\
  -t, --omit-शीर्षकम्\n\
         omit page शीर्षकाणि and trailers; implied यदि PAGE_दीर्घता <= 10\n\
"));
      oputs (_("\
  -T, --omit-pagination\n\
         omit page शीर्षकाणि and trailers,\n\
         eliminate any pagination by form feeds समुच्चयः_2 अन्तर् निवेशः files\n\
"));
      oputs (_("\
  -v, --show-nonprinting\n\
         use octal backslash notation\n\
"));
      oputs (_("\
  -w, --विस्तारः=PAGE_विस्तारः\n\
         समुच्चयः_2 page विस्तारः गन्तव्यम् PAGE_विस्तारः (72) characters प्रति\n\
         गुणजः text-column output only, -s[वर्ण] turns off (72)\n\
"));
      oputs (_("\
  -W, --page-विस्तारः=PAGE_विस्तारः\n\
         समुच्चयः_2 page विस्तारः गन्तव्यम् PAGE_विस्तारः (72) characters always,\n\
         truncate पङ्क्तयः, except -J option is समुच्चयः_2,\n\
         no interference with -S or -s\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (संस्करणम्_OPTION_DESCRIPTION);
      emit_ancillary_info (क्रमादेशः_NAME);
    }
  निर्गच्छ (स्थितिः);
}
