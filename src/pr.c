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

#అన్తర్భావ <config.h>

#అన్తర్భావ <getopt.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ "system.h"
#అన్తర్భావ "c-ctype.h"
#అన్తర్భావ "fadvise.h"
#అన్తర్భావ "hard-locale.h"
#అన్తర్భావ "mbswidth.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "stat-time.h"
#అన్తర్భావ "stdio--.h"
#అన్తర్భావ "strftime.h"
#అన్తర్భావ "xstrtol.h"
#అన్తర్భావ "xstrtol-error.h"
#అన్తర్భావ "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "pr"

#నిర్వచించు AUTHORS \
  proper_name ("Pete TerMaat"), \
  proper_name ("Roland Huebner")

/* Used with start_position in the struct COLUMN described below.
   If start_position == ANYWHERE, we aren't truncating columns and
   can begin printing a column anywhere.  Otherwise we must pad to
   the horizontal position start_position. */
#నిర్వచించు ANYWHERE	0

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

నిర్మాణం స్తంభం_3;
నిర్మాణం స్తంభం_3
  {
    FILE *fp;			/* Input stream for this column. */
    అక్షరం స్థిరరాశి *name;		/* File name. */
    గణన
      {
        OPEN,
        FF_దొరికింది,		/* used with -b option, set with \f, changed
                                   to ON_HOLD after print_header */
        ON_HOLD,		/* Hit a form feed. */
        CLOSED
      }
    స్థితి_2;			/* Status of the file pointer. */

    /* Func to print lines in this col. */
    bool (*print_func) (నిర్మాణం స్తంభం_3 *);

    /* Func to print/store chars in this col. */
    శూన్య (*char_func) (అక్షరం);

    పూర్ణాంకం current_పంక్తి;		/* Index of current place in line_vector. */
    పూర్ణాంకం పంక్తులు_stored;		/* Number of lines stored in buff. */
    పూర్ణాంకం పంక్తులు_గమ్యం_print;		/* No. lines stored or space left on page. */
    పూర్ణాంకం ప్రారంభం_position;		/* Horizontal position of first char. */
    bool numbered;
    bool నిండిన_పుట_printed;	/* True means printed without a FF found. */

    /* p->full_page_printed  controls a special case of "FF set by hand":
       True means a full page has been printed without FF found. To avoid an
       additional empty page we have to ignore a FF immediately following in
       the next line. */
  };

రకనిర్వచనం నిర్మాణం స్తంభం_3 స్తంభం_3;

స్థిర పూర్ణాంకం char_గమ్యం_clump (అక్షరం c);
స్థిర bool read_పంక్తి (స్తంభం_3 *p);
స్థిర bool print_పుట (శూన్య);
స్థిర bool print_stored (స్తంభం_3 *p);
స్థిర bool open_file (అక్షరం *name, స్తంభం_3 *p);
స్థిర bool skip_గమ్యం_పుట (uintmax_t పుట);
స్థిర శూన్య print_శీర్షిక (శూన్య);
స్థిర శూన్య pad_across_గమ్యం (పూర్ణాంకం position);
స్థిర శూన్య కూడు_పంక్తి_సంఖ్య (స్తంభం_3 *p);
స్థిర పూర్ణాంకం getoptnum (అక్షరం స్థిరరాశి *n_str, పూర్ణాంకం min, అక్షరం స్థిరరాశి *errfmt);
స్థిర శూన్య getoptarg (అక్షరం *arg, అక్షరం switch_char, అక్షరం *character,
                       పూర్ణాంకం *సంఖ్య);
స్థిర శూన్య print_files (పూర్ణాంకం సంఖ్య_యొక్క_files, అక్షరం **av);
స్థిర శూన్య init_parameters (పూర్ణాంకం సంఖ్య_యొక్క_files);
స్థిర శూన్య init_శీర్షిక (అక్షరం స్థిరరాశి *filename, పూర్ణాంకం desc);
స్థిర bool init_fps (పూర్ణాంకం సంఖ్య_యొక్క_files, అక్షరం **av);
స్థిర శూన్య init_funcs (శూన్య);
స్థిర శూన్య init_store_నిలువుల_సంఖ్య (శూన్య);
స్థిర శూన్య store_columns (శూన్య);
స్థిర శూన్య balance (పూర్ణాంకం మొత్తం_stored);
స్థిర శూన్య store_char (అక్షరం c);
స్థిర శూన్య pad_కిందికి (గుర్తులేని పూర్ణాంకం పంక్తులు);
స్థిర శూన్య read_rest_యొక్క_పంక్తి (స్తంభం_3 *p);
స్థిర శూన్య skip_read (స్తంభం_3 *p, పూర్ణాంకం స్తంభం_సంఖ్య);
స్థిర శూన్య print_char (అక్షరం c);
స్థిర శూన్య cleanup (శూన్య);
స్థిర శూన్య print_sep_string (శూన్య);
స్థిర శూన్య separator_string (అక్షరం స్థిరరాశి *optarg_S);

/* All of the columns to print.  */
స్థిర స్తంభం_3 *స్తంభం_సదిశ;

/* When printing a single file in multiple downward columns,
   we store the leftmost columns contiguously in buff.
   To print a line from buff, get the index of the first character
   from line_vector[i], and print up to line_vector[i + 1]. */
స్థిర అక్షరం *buff;

/* Index of the position in buff where the next character
   will be stored. */
స్థిర గుర్తులేని పూర్ణాంకం buff_current;

/* The number of characters in buff.
   Used for allocation of buff and to detect overflow of buff. */
స్థిర idx_t buff_allocated;

/* Array of indices into buff.
   Each entry is an index of the first character of a line.
   This is used when storing lines to facilitate shuffling when
   we do column balancing on the last page. */
స్థిర పూర్ణాంకం *పంక్తి_సదిశ;

/* Array of horizontal positions.
   For each line in line_vector, end_vector[line] is the horizontal
   position we are in after printing that line.  We keep track of this
   so that we know how much we need to pad to prepare for the next
   column. */
స్థిర పూర్ణాంకం *చివర_సదిశ;

/* (-m) True means we're printing multiple files in parallel. */
స్థిర bool parallel_files = false;

/* (-m) True means a line starts with some empty columns (some files
   already CLOSED or ON_HOLD) which we have to align. */
స్థిర bool align_ఖాళీలు_నిలువుల_సంఖ్య;

/* (-m) True means we have not yet found any printable column in a line.
   align_empty_cols = true  has to be maintained. */
స్థిర bool ఖాళీలు_పంక్తి;

/* (-m) False means printable column output precedes a form feed found.
   Column alignment is done only once. No additional action with that form
   feed.
   True means we found only a form feed in a column. Maybe we have to do
   some column alignment with that form feed. */
స్థిర bool FF_only;

/* (-[0-9]+) True means we're given an option explicitly specifying
   number of columns.  Used to detect when this option is used with -m
   and when translating old options to new/long options. */
స్థిర bool explicit_columns = false;

/* (-t|-T) False means we aren't printing headers and footers. */
స్థిర bool extremities = true;

/* (-t) True means we retain all FF set by hand in input files.
   False is set with -T option. */
స్థిర bool keep_FF = false;
స్థిర bool print_a_FF = false;

/* True means we need to print a header as soon as we know we've got input
   to print after it. */
స్థిర bool print_a_శీర్షిక;

/* (-f) True means use formfeeds instead of newlines to separate pages. */
స్థిర bool use_form_feed = false;

/* True means we have read the standard input. */
స్థిర bool have_read_stdin = false;

/* True means the -a flag has been given. */
స్థిర bool print_across_flag = false;

/* True means we're printing one file in multiple (>1) downward columns. */
స్థిర bool storing_columns = true;

/* (-b) True means balance columns on the last page as Sys V does. */
/* That's no longer an independent option. With storing_columns = true
   balance_columns = true is used too (s. function init_parameters).
   We get a consistent formulation with "FF set by hand" in input files. */
స్థిర bool balance_columns = false;

/* (-l) Number of lines on a page, including header and footer lines. */
స్థిర పూర్ణాంకం పంక్తులు_చొప్పున_పుట = 66;

/* Number of lines in the header and footer can be reset to 0 using
   the -t flag. */
గణన { పంక్తులు_చొప్పున_శీర్షిక = 5 };
స్థిర పూర్ణాంకం పంక్తులు_చొప్పున_body;
గణన { పంక్తులు_చొప్పున_footer = 5 };

/* (-w|-W) Width in characters of the page.  Does not include the width of
   the margin. */
స్థిర పూర్ణాంకం chars_చొప్పున_పంక్తి = 72;

/* (-w|W) True means we truncate lines longer than chars_per_column. */
స్థిర bool truncate_పంక్తులు = false;

/* (-J) True means we join lines without any line truncation. -J
   dominates -w option. */
స్థిర bool join_పంక్తులు = false;

/* Number of characters in a column.  Based on col_sep_length and
   page width. */
స్థిర పూర్ణాంకం chars_చొప్పున_స్తంభం;

/* (-e) True means convert tabs to spaces on input. */
స్థిర bool untabify_ఆదానం = false;

/* (-e) The input tab character. */
స్థిర అక్షరం ఆదానం_tab_char = '\t';

/* (-e) Tabstops are at chars_per_tab, 2*chars_per_tab, 3*chars_per_tab, ...
   where the leftmost column is 1. */
స్థిర పూర్ణాంకం chars_చొప్పున_ఆదానం_tab = 8;

/* (-i) True means convert spaces to tabs on output. */
స్థిర bool tabify_output = false;

/* (-i) The output tab character. */
స్థిర అక్షరం output_tab_char = '\t';

/* (-i) The width of the output tab. */
స్థిర పూర్ణాంకం chars_చొప్పున_output_tab = 8;

/* Keeps track of pending white space.  When we hit a nonspace
   character after some whitespace, we print whitespace, tabbing
   if necessary to get to output_position + spaces_not_printed. */
స్థిర పూర్ణాంకం spaces_not_printed;

/* (-o) Number of spaces in the left margin (tabs used when possible). */
స్థిర పూర్ణాంకం chars_చొప్పున_margin = 0;

/* Position where the next character will fall.
   Leftmost position is 0 + chars_per_margin.
   Rightmost position is chars_per_margin + chars_per_line - 1.
   This is important for converting spaces to tabs on output. */
స్థిర పూర్ణాంకం output_position;

/* Horizontal position relative to the current file.
   (output_position depends on where we are on the page;
   input_position depends on where we are in the file.)
   Important for converting tabs to spaces on input. */
స్థిర పూర్ణాంకం ఆదానం_position;

/* True if there were any failed opens so we can exit with nonzero
   status.  */
స్థిర bool failed_opens = false;

/* The number of spaces taken up if we print a tab character with width
   c_ from position h_. */
#నిర్వచించు TAB_వెడల్పు(c_, h_) ((c_) - ((h_) % (c_)))

/* The horizontal position we'll be at after printing a tab character
   of width c_ from the position h_. */
#నిర్వచించు POS_AFTER_TAB(c_, h_) ((h_) + TAB_వెడల్పు (c_, h_))

/* (-NNN) Number of columns of text to print. */
స్థిర పూర్ణాంకం columns = 1;

/* (+NNN:MMM) Page numbers on which to begin and stop printing.
   first_page_number = 0  will be used to check input only. */
స్థిర uintmax_t మొదటి_పుట_సంఖ్య = 0;
స్థిర uintmax_t చివరి_పుట_సంఖ్య = UINTMAX_MAX;

/* Number of files open (not closed, not on hold). */
స్థిర పూర్ణాంకం files_ready_గమ్యం_read = 0;

/* Current page number.  Displayed in header. */
స్థిర uintmax_t పుట_సంఖ్య;

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
స్థిర పూర్ణాంకం పంక్తి_సంఖ్య;

/* (-n) True means lines should be preceded by numbers. */
స్థిర bool numbered_పంక్తులు = false;

/* (-n) Character which follows each line number. */
స్థిర అక్షరం సంఖ్య_separator = '\t';

/* (-n) line counting starts with 1st line of input file (not with 1st
   line of 1st page printed). */
స్థిర పూర్ణాంకం పంక్తి_లెక్క = 1;

/* (-n) True means counting of skipped lines starts with 1st line of
   input file. False means -N option is used in addition, counting of
   skipped lines not required. */
స్థిర bool skip_లెక్క = true;

/* (-N) Counting starts with start_line_number = NUMBER at 1st line of
   first page printed, usually not 1st page of input file. */
స్థిర పూర్ణాంకం ప్రారంభం_పంక్తి_అంకె = 1;

/* (-n) Width in characters of a line number. */
స్థిర పూర్ణాంకం chars_చొప్పున_సంఖ్య = 5;

/* Used when widening the first column to accommodate numbers -- only
   needed when printing files in parallel.  Includes width of both the
   number and the number_separator. */
స్థిర పూర్ణాంకం సంఖ్య_వెడల్పు;

/* Buffer sprintf uses to format a line number. */
స్థిర అక్షరం *సంఖ్య_buff;

/* (-v) True means nonprintable characters are printed as escape sequences.
   control-g becomes \007. */
స్థిర bool use_esc_sequence = false;

/* (-c) True means nonprintable characters are printed as control prefixes.
   control-g becomes ^G. */
స్థిర bool use_cntrl_ఉపసర్గ = false;

/* (-d) True means output is double spaced. */
స్థిర bool double_space = false;

/* Number of files opened initially in init_files.  Should be 1
   unless we're printing multiple files in parallel. */
స్థిర పూర్ణాంకం మొత్తం_files = 0;

/* (-r) True means don't complain if we can't open a file. */
స్థిర bool ignore_failed_opens = false;

/* (-S) True means we separate columns with a specified string.
   -S option does not affect line truncation nor column alignment. */
స్థిర bool use_నిలువు_separator = false;

/* String used to separate columns if the -S option has been specified.
   Default without -S but together with one of the column options
   -a|COLUMN|-m is a 'space' and with the -J option a 'tab'. */
స్థిర అక్షరం స్థిరరాశి *నిలువు_sep_string = "";
స్థిర పూర్ణాంకం నిలువు_sep_నిడివి = 0;
స్థిర అక్షరం *స్తంభం_separator = (అక్షరం *) " ";
స్థిర అక్షరం *పంక్తి_separator = (అక్షరం *) "\t";

/* Number of separator characters waiting to be printed as soon as we
   know that we have any input remaining to be printed. */
స్థిర పూర్ణాంకం separators_not_printed;

/* Position we need to pad to, as soon as we know that we have input
   remaining to be printed. */
స్థిర పూర్ణాంకం padding_not_printed;

/* True means we should pad the end of the page.  Remains false until we
   know we have a page to print. */
స్థిర bool pad_vertically;

/* (-h) String of characters used in place of the filename in the header. */
స్థిర అక్షరం *custom_శీర్షిక;

/* (-D) Date format for the header.  */
స్థిర అక్షరం స్థిరరాశి *date_format;

/* The local time zone rules, as per the TZ environment variable.  */
స్థిర timezone_t localtz;

/* Date and file name for the header.  */
స్థిర అక్షరం *date_పాఠ్యం;
స్థిర అక్షరం స్థిరరాశి *file_పాఠ్యం;

/* Output columns available, not counting the date and file name.  */
స్థిర పూర్ణాంకం శీర్షిక_వెడల్పు_అందుబాటు;

స్థిర అక్షరం *clump_buff;

/* True means we read the line no. lines_per_body in skip_read
   called by skip_to_page. That variable controls the coincidence of a
   "FF set by hand" and "full_page_printed", see above the definition of
   structure COLUMN. */
స్థిర bool చివరి_పంక్తి = false;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
గణన
{
  COLUMNS_OPTION = CHAR_MAX + 1,
  పుటలు_OPTION
};

స్థిర అక్షరం స్థిరరాశి short_options[] =
  "-0123456789D:FJN:S::TW:abcde::fh:i::l:mn::o:rs::tvw:";

స్థిర నిర్మాణం option స్థిరరాశి long_options[] =
{
  {"pages", required_వాదం, ఖాళీ, పుటలు_OPTION},
  {"columns", required_వాదం, ఖాళీ, COLUMNS_OPTION},
  {"across", no_వాదం, ఖాళీ, 'a'},
  {"show-control-chars", no_వాదం, ఖాళీ, 'c'},
  {"double-space", no_వాదం, ఖాళీ, 'd'},
  {"date-format", required_వాదం, ఖాళీ, 'D'},
  {"expand-tabs", optional_వాదం, ఖాళీ, 'e'},
  {"form-feed", no_వాదం, ఖాళీ, 'f'},
  {"header", required_వాదం, ఖాళీ, 'h'},
  {"output-tabs", optional_వాదం, ఖాళీ, 'i'},
  {"join-lines", no_వాదం, ఖాళీ, 'J'},
  {"length", required_వాదం, ఖాళీ, 'l'},
  {"merge", no_వాదం, ఖాళీ, 'm'},
  {"number-lines", optional_వాదం, ఖాళీ, 'n'},
  {"first-line-number", required_వాదం, ఖాళీ, 'N'},
  {"indent", required_వాదం, ఖాళీ, 'o'},
  {"no-file-warnings", no_వాదం, ఖాళీ, 'r'},
  {"separator", optional_వాదం, ఖాళీ, 's'},
  {"sep-string", optional_వాదం, ఖాళీ, 'S'},
  {"omit-header", no_వాదం, ఖాళీ, 't'},
  {"omit-pagination", no_వాదం, ఖాళీ, 'T'},
  {"show-nonprinting", no_వాదం, ఖాళీ, 'v'},
  {"width", required_వాదం, ఖాళీ, 'w'},
  {"page-width", required_వాదం, ఖాళీ, 'W'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0}
};

స్థిర _Noreturn శూన్య
integer_అధికప్రవాహం (శూన్య)
{
  error (EXIT_FAILURE, 0, _("integer overflow"));
}

/* Return the number of columns that have either an open file or
   stored lines. */

ATTRIBUTE_శుద్ధ
స్థిర గుర్తులేని పూర్ణాంకం
నిలువుల_సంఖ్య_ready_గమ్యం_print (శూన్య)
{
  స్తంభం_3 *q;
  గుర్తులేని పూర్ణాంకం i;
  గుర్తులేని పూర్ణాంకం n;

  n = 0;
  కొరకు (q = స్తంభం_సదిశ, i = 0; i < columns; ++q, ++i)
    ఒకవేళ (q->స్థితి_2 == OPEN
        || q->స్థితి_2 == FF_దొరికింది	/* With -b: To print a header only */
        || (storing_columns && q->పంక్తులు_stored > 0 && q->పంక్తులు_గమ్యం_print > 0))
      ++n;
  తిరిగి n;
}

/* Estimate first_ / last_page_number
   using option +FIRST_PAGE:LAST_PAGE */

స్థిర bool
మొదటి_చివరి_పుట (పూర్ణాంకం oi, అక్షరం c, అక్షరం స్థిరరాశి *పుటలు)
{
  అక్షరం *p;
  uintmax_t మొదటి;
  uintmax_t చివరి = UINTMAX_MAX;
  strtol_error err = xstrtoumax (పుటలు, &p, 10, &మొదటి, "");
  ఒకవేళ (err != LONGINT_OK && err != LONGINT_INVALID_SUFFIX_CHAR)
    xstrtol_fatal (err, oi, c, long_options, పుటలు);

  ఒకవేళ (p == పుటలు || !మొదటి)
    తిరిగి false;

  ఒకవేళ (*p == ':')
    {
      అక్షరం స్థిరరాశి *p1 = p + 1;
      err = xstrtoumax (p1, &p, 10, &చివరి, "");
      ఒకవేళ (err != LONGINT_OK)
        xstrtol_fatal (err, oi, c, long_options, పుటలు);
      ఒకవేళ (p1 == p || చివరి < మొదటి)
        తిరిగి false;
    }

  ఒకవేళ (*p)
    తిరిగి false;

  మొదటి_పుట_సంఖ్య = మొదటి;
  చివరి_పుట_సంఖ్య = చివరి;
  తిరిగి true;
}

/* Parse column count string S, and if it's valid (1 or larger and
   within range of the type of 'columns') set the global variables
   columns and explicit_columns.  Otherwise, exit with a diagnostic.  */

స్థిర శూన్య
parse_స్తంభం_లెక్క (అక్షరం స్థిరరాశి *s)
{
  columns = getoptnum (s, 1, _("invalid number of columns"));
  explicit_columns = true;
}

/* Estimate length of col_sep_string with option -S.  */

స్థిర శూన్య
separator_string (అక్షరం స్థిరరాశి *optarg_S)
{
  size_t len = అక్షరశ్రేణి_పొడవు (optarg_S);
  ఒకవేళ (గరిష్ఠ_పూర్ణాంకం < len)
    integer_అధికప్రవాహం ();
  నిలువు_sep_నిడివి = len;
  నిలువు_sep_string = optarg_S;
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  గుర్తులేని పూర్ణాంకం n_files;
  bool old_options = false;
  bool old_w = false;
  bool old_s = false;
  అక్షరం **file_పేర్లు;

  /* Accumulate the digits of old-style options like -99.  */
  అక్షరం *స్తంభం_లెక్క_string = ఖాళీ;
  idx_t n_digits = 0;
  idx_t n_alloc = 0;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  n_files = 0;
  file_పేర్లు = (argc > 1
                ? xnmalloc (argc - 1, పరిమాణం (అక్షరం *))
                : ఖాళీ);

  అయితే (true)
    {
      పూర్ణాంకం oi = -1;
      పూర్ణాంకం c = getopt_long (argc, argv, short_options, long_options, &oi);
      ఒకవేళ (c == -1)
        ఆపు;

      ఒకవేళ (c_isdigit (c))
        {
          /* Accumulate column-count digits specified via old-style options. */
          ఒకవేళ (n_digits + 1 >= n_alloc)
            స్తంభం_లెక్క_string = xpalloc (స్తంభం_లెక్క_string, &n_alloc, 2, -1,
                                           పరిమాణం *స్తంభం_లెక్క_string);
          స్తంభం_లెక్క_string[n_digits++] = c;
          స్తంభం_లెక్క_string[n_digits] = '\0';
          కొనసాగు;
        }

      n_digits = 0;

      ఎంపిక (c)
        {
        స్థితి 1:			/* Non-option argument. */
          /* long option --page dominates old '+FIRST_PAGE ...'.  */
          ఒకవేళ (! (మొదటి_పుట_సంఖ్య == 0
                 && *optarg == '+' && మొదటి_చివరి_పుట (-2, '+', optarg + 1)))
            file_పేర్లు[n_files++] = optarg;
          ఆపు;

        స్థితి పుటలు_OPTION:	/* --pages=FIRST_PAGE[:LAST_PAGE] */
          {			/* dominates old opt +... */
            ఒకవేళ (! optarg)
              error (EXIT_FAILURE, 0,
                     _("'--pages=FIRST_PAGE[:LAST_PAGE]' missing argument"));
            లేకపోతే ఒకవేళ (! మొదటి_చివరి_పుట (oi, 0, optarg))
              error (EXIT_FAILURE, 0, _("invalid page range %s"),
                     quote (optarg));
            ఆపు;
          }

        స్థితి COLUMNS_OPTION:	/* --columns=COLS */
          {
            parse_స్తంభం_లెక్క (optarg);

            /* If there was a prior column count specified via the
               short-named option syntax, e.g., -9, ensure that this
               long-name-specified value overrides it.  */
            విడుదల (స్తంభం_లెక్క_string);
            స్తంభం_లెక్క_string = ఖాళీ;
            n_alloc = 0;
            ఆపు;
          }

        స్థితి 'a':
          print_across_flag = true;
          storing_columns = false;
          ఆపు;
        స్థితి 'b':
          balance_columns = true;
          ఆపు;
        స్థితి 'c':
          use_cntrl_ఉపసర్గ = true;
          ఆపు;
        స్థితి 'd':
          double_space = true;
          ఆపు;
        స్థితి 'D':
          date_format = optarg;
          ఆపు;
        స్థితి 'e':
          ఒకవేళ (optarg)
            getoptarg (optarg, 'e', &ఆదానం_tab_char,
                       &chars_చొప్పున_ఆదానం_tab);
          /* Could check tab width > 0. */
          untabify_ఆదానం = true;
          ఆపు;
        స్థితి 'f':
        స్థితి 'F':
          use_form_feed = true;
          ఆపు;
        స్థితి 'h':
          custom_శీర్షిక = optarg;
          ఆపు;
        స్థితి 'i':
          ఒకవేళ (optarg)
            getoptarg (optarg, 'i', &output_tab_char,
                       &chars_చొప్పున_output_tab);
          /* Could check tab width > 0. */
          tabify_output = true;
          ఆపు;
        స్థితి 'J':
          join_పంక్తులు = true;
          ఆపు;
        స్థితి 'l':
          పంక్తులు_చొప్పున_పుట
            = getoptnum (optarg, 1,
                         _("'-l PAGE_LENGTH' invalid number of lines"));
          ఆపు;
        స్థితి 'm':
          parallel_files = true;
          storing_columns = false;
          ఆపు;
        స్థితి 'n':
          numbered_పంక్తులు = true;
          ఒకవేళ (optarg)
            getoptarg (optarg, 'n', &సంఖ్య_separator,
                       &chars_చొప్పున_సంఖ్య);
          ఆపు;
        స్థితి 'N':
          skip_లెక్క = false;
          ప్రారంభం_పంక్తి_అంకె
            = getoptnum (optarg, INT_MIN,
                         _("'-N NUMBER' invalid starting line number"));
          ఆపు;
        స్థితి 'o':
          chars_చొప్పున_margin = getoptnum (optarg, 0,
                                        _("'-o MARGIN' invalid line offset"));
          ఆపు;
        స్థితి 'r':
          ignore_failed_opens = true;
          ఆపు;
        స్థితి 's':
          old_options = true;
          old_s = true;
          ఒకవేళ (!use_నిలువు_separator && optarg)
            separator_string (optarg);
          ఆపు;
        స్థితి 'S':
          old_s = false;
          /* Reset an additional input of -s, -S dominates -s */
          నిలువు_sep_string = "";
          నిలువు_sep_నిడివి = 0;
          use_నిలువు_separator = true;
          ఒకవేళ (optarg)
            separator_string (optarg);
          ఆపు;
        స్థితి 't':
          extremities = false;
          keep_FF = true;
          ఆపు;
        స్థితి 'T':
          extremities = false;
          keep_FF = false;
          ఆపు;
        స్థితి 'v':
          use_esc_sequence = true;
          ఆపు;
        స్థితి 'w':
          old_options = true;
          old_w = true;
          {
            పూర్ణాంకం tmp_cpl
              = getoptnum (optarg, 1,
                           _("'-w PAGE_WIDTH' invalid number of characters"));
            ఒకవేళ (! truncate_పంక్తులు)
              chars_చొప్పున_పంక్తి = tmp_cpl;
          }
          ఆపు;
        స్థితి 'W':
          old_w = false;			/* dominates -w */
          truncate_పంక్తులు = true;
          chars_చొప్పున_పంక్తి
            = getoptnum (optarg, 1,
                         _("'-W PAGE_WIDTH' invalid number of characters"));
          ఆపు;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);
        అప్రమేయ:
          usage (EXIT_FAILURE);
          ఆపు;
        }
    }

  ఒకవేళ (స్తంభం_లెక్క_string)
    {
      parse_స్తంభం_లెక్క (స్తంభం_లెక్క_string);
      విడుదల (స్తంభం_లెక్క_string);
    }

  ఒకవేళ (! date_format)
    date_format = (getenv ("POSIXLY_CORRECT") && !hard_locale (LC_సమయం)
                   ? "%b %e %H:%M %Y"
                   : "%Y-%m-%d %H:%M");

  localtz = tzalloc (getenv ("TZ"));

  /* Now we can set a reasonable initial value: */
  ఒకవేళ (మొదటి_పుట_సంఖ్య == 0)
    మొదటి_పుట_సంఖ్య = 1;

  ఒకవేళ (parallel_files && explicit_columns)
    error (EXIT_FAILURE, 0,
           _("cannot specify number of columns when printing in parallel"));

  ఒకవేళ (parallel_files && print_across_flag)
    error (EXIT_FAILURE, 0,
           _("cannot specify both printing across and printing in parallel"));

/* Translate some old short options to new/long options.
   To meet downward compatibility with other UNIX pr utilities
   and some POSIX specifications. */

  ఒకవేళ (old_options)
    {
      ఒకవేళ (old_w)
        {
          ఒకవేళ (parallel_files || explicit_columns)
            {
              /* activate -W */
              truncate_పంక్తులు = true;
              ఒకవేళ (old_s)
                /* adapt HP-UX and SunOS: -s = no separator;
                   activate -S */
                use_నిలువు_separator = true;
            }
          లేకపోతే
            /* old -w sets width with columns only
               activate -J */
            join_పంక్తులు = true;
        }
      లేకపోతే ఒకవేళ (!use_నిలువు_separator)
        {
          /* No -S option read */
          ఒకవేళ (old_s && (parallel_files || explicit_columns))
            {
              ఒకవేళ (!truncate_పంక్తులు)
                {
                  /* old -s (without -w and -W) annuls column alignment,
                  uses fields, activate -J */
                  join_పంక్తులు = true;
                  ఒకవేళ (నిలువు_sep_నిడివి > 0)
                    /* activate -S */
                    use_నిలువు_separator = true;
                }
              లేకపోతే
                /* with -W */
                /* adapt HP-UX and SunOS: -s = no separator;
                   activate -S */
                use_నిలువు_separator = true;
            }
        }
    }

  కొరకు (; optind < argc; optind++)
    {
      file_పేర్లు[n_files++] = argv[optind];
    }

  ఒకవేళ (n_files == 0)
    {
      /* No file arguments specified;  read from standard input.  */
      print_files (0, ఖాళీ);
    }
  లేకపోతే
    {
      ఒకవేళ (parallel_files)
        print_files (n_files, file_పేర్లు);
      లేకపోతే
        {
          కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < n_files; i++)
            print_files (1, &file_పేర్లు[i]);
        }
    }

  cleanup ();

  ఒకవేళ (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, _("standard input"));
  main_exit (failed_opens ? EXIT_FAILURE : EXIT_SUCCESS);
}

/* Parse numeric arguments, ensuring MIN <= number <= INT_MAX.  */

స్థిర పూర్ణాంకం
getoptnum (అక్షరం స్థిరరాశి *n_str, పూర్ణాంకం min, అక్షరం స్థిరరాశి *err)
{
  తిరిగి xnumtoimax (n_str, 10, min, గరిష్ఠ_పూర్ణాంకం, "", err, 0,
                     min <= 0 ? 0 : XTOINT_MIN_RANGE);
}

/* Parse options of the form -scNNN.

   Example: -nck, where 'n' is the option, c is the optional number
   separator, and k is the optional width of the field used when printing
   a number. */

స్థిర శూన్య
getoptarg (అక్షరం *arg, అక్షరం switch_char, అక్షరం *character, పూర్ణాంకం *సంఖ్య)
{
  అక్షరం స్థిరరాశి *అంకె = arg;

  ఒకవేళ (!*అంకె)
    {
      error (0, 0, _("'-%c': Invalid argument: %s"), switch_char, quote (అంకె));
      usage (EXIT_FAILURE);
    }

  ఒకవేళ (!c_isdigit (*అంకె))
    *character = *అంకె++;
  ఒకవేళ (*అంకె)
    {
      దీర్ఘ పూర్ణాంకం tmp_long;
      strtol_error e = xstrtol (అంకె, ఖాళీ, 10, &tmp_long, "");
      ఒకవేళ (e == LONGINT_OK)
        {
          ఒకవేళ (tmp_long <= 0)
            e = LONGINT_INVALID;
          లేకపోతే ఒకవేళ (గరిష్ఠ_పూర్ణాంకం < tmp_long)
            e = LONGINT_అధికప్రవాహం;
        }
      ఒకవేళ (e != LONGINT_OK)
        {
          error (0, e & LONGINT_అధికప్రవాహం ? EOVERFLOW : 0,
             _("'-%c' extra characters or invalid number in the argument: %s"),
                 switch_char, quote (arg));
          usage (EXIT_FAILURE);
        }
      *సంఖ్య = tmp_long;
    }
}

/* Set parameters related to formatting. */

స్థిర శూన్య
init_parameters (పూర్ణాంకం సంఖ్య_యొక్క_files)
{
  పూర్ణాంకం chars_వాడిన_by_సంఖ్య = 0;

  పంక్తులు_చొప్పున_body = పంక్తులు_చొప్పున_పుట - పంక్తులు_చొప్పున_శీర్షిక - పంక్తులు_చొప్పున_footer;
  ఒకవేళ (పంక్తులు_చొప్పున_body <= 0)
    {
      extremities = false;
      keep_FF = true;
    }
  ఒకవేళ (! extremities)
    పంక్తులు_చొప్పున_body = పంక్తులు_చొప్పున_పుట;

  ఒకవేళ (double_space)
    పంక్తులు_చొప్పున_body = MAX (1, పంక్తులు_చొప్పున_body / 2);

  /* If input is stdin, cannot print parallel files.  BSD dumps core
     on this. */
  ఒకవేళ (సంఖ్య_యొక్క_files == 0)
    parallel_files = false;

  ఒకవేళ (parallel_files)
    columns = సంఖ్య_యొక్క_files;

  /* One file, multi columns down: -b option is set to get a consistent
     formulation with "FF set by hand" in input files. */
  ఒకవేళ (storing_columns)
    balance_columns = true;

  /* Tabification is assumed for multiple columns. */
  ఒకవేళ (columns > 1)
    {
      ఒకవేళ (!use_నిలువు_separator)
        {
          /* Use default separator */
          ఒకవేళ (join_పంక్తులు)
            నిలువు_sep_string = పంక్తి_separator;
          లేకపోతే
            నిలువు_sep_string = స్తంభం_separator;

          నిలువు_sep_నిడివి = 1;
          use_నిలువు_separator = true;
        }
      /* It's rather pointless to define a TAB separator with column
         alignment */
      లేకపోతే ఒకవేళ (!join_పంక్తులు && నిలువు_sep_నిడివి == 1 && *నిలువు_sep_string == '\t')
        నిలువు_sep_string = స్తంభం_separator;

      truncate_పంక్తులు = true;
      ఒకవేళ (! (నిలువు_sep_నిడివి == 1 && *నిలువు_sep_string == '\t'))
        untabify_ఆదానం = true;
      tabify_output = true;
    }
  లేకపోతే
    storing_columns = false;

  /* -J dominates -w in any case */
  ఒకవేళ (join_పంక్తులు)
    truncate_పంక్తులు = false;

  ఒకవేళ (numbered_పంక్తులు)
    {
      పూర్ణాంకం chars_చొప్పున_default_tab = 8;

      పంక్తి_లెక్క = ప్రారంభం_పంక్తి_అంకె;

      /* To allow input tab-expansion (-e sensitive) use:
         if (number_separator == input_tab_char)
           number_width = chars_per_number
             + TAB_WIDTH (chars_per_input_tab, chars_per_number);   */

      /* Estimate chars_per_text without any margin and keep it constant. */
      ఒకవేళ (సంఖ్య_separator == '\t')
        సంఖ్య_వెడల్పు = (chars_చొప్పున_సంఖ్య
                        + TAB_వెడల్పు (chars_చొప్పున_default_tab, chars_చొప్పున_సంఖ్య));
      లేకపోతే
        సంఖ్య_వెడల్పు = chars_చొప్పున_సంఖ్య + 1;

      /* The number is part of the column width unless we are
         printing files in parallel. */
      ఒకవేళ (parallel_files)
        chars_వాడిన_by_సంఖ్య = సంఖ్య_వెడల్పు;
    }

  పూర్ణాంకం sep_chars, useful_chars;
  ఒకవేళ (ckd_mul (&sep_chars, columns - 1, నిలువు_sep_నిడివి))
    sep_chars = గరిష్ఠ_పూర్ణాంకం;
  ఒకవేళ (ckd_sub (&useful_chars, chars_చొప్పున_పంక్తి - chars_వాడిన_by_సంఖ్య,
               sep_chars))
    useful_chars = 0;
  chars_చొప్పున_స్తంభం = useful_chars / columns;

  ఒకవేళ (chars_చొప్పున_స్తంభం < 1)
    error (EXIT_FAILURE, 0, _("page width too narrow"));

  ఒకవేళ (numbered_పంక్తులు)
    {
      విడుదల (సంఖ్య_buff);
      సంఖ్య_buff = xmalloc (MAX (chars_చొప్పున_సంఖ్య,
                                  INT_STRLEN_BOUND (పంక్తి_సంఖ్య)) + 1);
    }

  /* Pick the maximum between the tab width and the width of an
     escape sequence.
     The width of an escape sequence (4) isn't the lower limit any longer.
     We've to use 8 as the lower limit, if we use chars_per_default_tab = 8
     to expand a tab which is not an input_tab-char. */
  విడుదల (clump_buff);
  clump_buff = xmalloc (MAX (8, chars_చొప్పున_ఆదానం_tab));
}

/* Open the necessary files,
   maintaining a COLUMN structure for each column.

   With multiple files, each column p has a different p->fp.
   With single files, each column p has the same p->fp.
   Return false if (number_of_files > 0) and no files can be opened,
   true otherwise.

   With each column/file p, p->full_page_printed is initialized,
   see also open_file.  */

స్థిర bool
init_fps (పూర్ణాంకం సంఖ్య_యొక్క_files, అక్షరం **av)
{
  స్తంభం_3 *p;

  మొత్తం_files = 0;

  విడుదల (స్తంభం_సదిశ);
  స్తంభం_సదిశ = xnmalloc (columns, పరిమాణం (స్తంభం_3));

  ఒకవేళ (parallel_files)
    {
      పూర్ణాంకం files_ఎడమ = సంఖ్య_యొక్క_files;
      కొరకు (p = స్తంభం_సదిశ; files_ఎడమ--; ++p, ++av)
        {
          ఒకవేళ (! open_file (*av, p))
            {
              --p;
              --columns;
            }
        }
      ఒకవేళ (columns == 0)
        తిరిగి false;
      init_శీర్షిక ("", -1);
    }
  లేకపోతే
    {
      p = స్తంభం_సదిశ;
      ఒకవేళ (సంఖ్య_యొక్క_files > 0)
        {
          ఒకవేళ (! open_file (*av, p))
            తిరిగి false;
          init_శీర్షిక (*av, fileno (p->fp));
          p->పంక్తులు_stored = 0;
        }
      లేకపోతే
        {
          p->name = _("standard input");
          p->fp = stdin;
          have_read_stdin = true;
          p->స్థితి_2 = OPEN;
          p->నిండిన_పుట_printed = false;
          ++మొత్తం_files;
          init_శీర్షిక ("", -1);
          p->పంక్తులు_stored = 0;
        }

      అక్షరం స్థిరరాశి *firstname = p->name;
      FILE *firstfp = p->fp;
      పూర్ణాంకం i;
      కొరకు (i = columns - 1, ++p; i; --i, ++p)
        {
          p->name = firstname;
          p->fp = firstfp;
          p->స్థితి_2 = OPEN;
          p->నిండిన_పుట_printed = false;
          p->పంక్తులు_stored = 0;
        }
    }
  files_ready_గమ్యం_read = మొత్తం_files;
  తిరిగి true;
}

/* Determine print_func and char_func, the functions
   used by each column for printing and/or storing.

   Determine the horizontal position desired when we begin
   printing a column (p->start_position). */

స్థిర శూన్య
init_funcs (శూన్య)
{
  పూర్ణాంకం i, h, h_తదుపరి;
  స్తంభం_3 *p;

  h = chars_చొప్పున_margin;

  ఒకవేళ (!truncate_పంక్తులు)
    h_తదుపరి = ANYWHERE;
  లేకపోతే
    {
      /* When numbering lines of parallel files, we enlarge the
         first column to accommodate the number.  Looks better than
         the Sys V approach. */
      ఒకవేళ (parallel_files && numbered_పంక్తులు)
        h_తదుపరి = h + chars_చొప్పున_స్తంభం + సంఖ్య_వెడల్పు;
      లేకపోతే
        h_తదుపరి = h + chars_చొప్పున_స్తంభం;
    }

  /* Enlarge p->start_position of first column to use the same form of
     padding_not_printed with all columns. */
  h = h + నిలువు_sep_నిడివి;

  /* This loop takes care of all but the rightmost column. */

  కొరకు (p = స్తంభం_సదిశ, i = 1; i < columns; ++p, ++i)
    {
      ఒకవేళ (storing_columns)	/* One file, multi columns down. */
        {
          p->char_func = store_char;
          p->print_func = print_stored;
        }
      లేకపోతే
        /* One file, multi columns across; or parallel files.  */
        {
          p->char_func = print_char;
          p->print_func = read_పంక్తి;
        }

      /* Number only the first column when printing files in
         parallel. */
      p->numbered = numbered_పంక్తులు && (!parallel_files || i == 1);
      p->ప్రారంభం_position = h;

      /* If we don't truncate lines, all start_positions are
         ANYWHERE, except the first column's start_position when
         using a margin. */

      ఒకవేళ (!truncate_పంక్తులు)
        {
          h = ANYWHERE;
          h_తదుపరి = ANYWHERE;
        }
      లేకపోతే
        {
          h = h_తదుపరి + నిలువు_sep_నిడివి;
          h_తదుపరి = h + chars_చొప్పున_స్తంభం;
        }
    }

  /* The rightmost column.

     Doesn't need to be stored unless we intend to balance
     columns on the last page. */
  ఒకవేళ (storing_columns && balance_columns)
    {
      p->char_func = store_char;
      p->print_func = print_stored;
    }
  లేకపోతే
    {
      p->char_func = print_char;
      p->print_func = read_పంక్తి;
    }

  p->numbered = numbered_పంక్తులు && (!parallel_files || i == 1);
  p->ప్రారంభం_position = h;
}

/* Open a file.  Return true if successful.

   With each file p, p->full_page_printed is initialized,
   see also init_fps. */

స్థిర bool
open_file (అక్షరం *name, స్తంభం_3 *p)
{
  ఒకవేళ (streq (name, "-"))
    {
      p->name = _("standard input");
      p->fp = stdin;
      have_read_stdin = true;
    }
  లేకపోతే
    {
      p->name = name;
      p->fp = fopen (name, "r");
    }
  ఒకవేళ (p->fp == ఖాళీ)
    {
      failed_opens = true;
      ఒకవేళ (!ignore_failed_opens)
        error (0, errno, "%s", quotef (name));
      తిరిగి false;
    }
  fadvise (p->fp, FADVISE_SEQUENTIAL);
  p->స్థితి_2 = OPEN;
  p->నిండిన_పుట_printed = false;
  ++మొత్తం_files;
  తిరిగి true;
}

/* Close the file in P.

   If we aren't dealing with multiple files in parallel, we change
   the status of all columns in the column list to reflect the close. */

స్థిర శూన్య
close_file (స్తంభం_3 *p)
{
  స్తంభం_3 *q;
  పూర్ణాంకం i;

  ఒకవేళ (p->స్థితి_2 == CLOSED)
    తిరిగి;

  పూర్ణాంకం err = errno;
  ఒకవేళ (!ferror (p->fp))
    err = 0;
  ఒకవేళ (fileno (p->fp) == STDIN_FILENO)
    clearerr (p->fp);
  లేకపోతే ఒకవేళ (fclose (p->fp) != 0 && !err)
    err = errno;
  ఒకవేళ (err)
    error (EXIT_FAILURE, err, "%s", quotef (p->name));

  ఒకవేళ (!parallel_files)
    {
      కొరకు (q = స్తంభం_సదిశ, i = columns; i; ++q, --i)
        {
          q->స్థితి_2 = CLOSED;
          ఒకవేళ (q->పంక్తులు_stored == 0)
            {
              q->పంక్తులు_గమ్యం_print = 0;
            }
        }
    }
  లేకపోతే
    {
      p->స్థితి_2 = CLOSED;
      p->పంక్తులు_గమ్యం_print = 0;
    }

  --files_ready_గమ్యం_read;
}

/* Put a file on hold until we start a new page,
   since we've hit a form feed.

   If we aren't dealing with parallel files, we must change the
   status of all columns in the column list. */

స్థిర శూన్య
hold_file (స్తంభం_3 *p)
{
  స్తంభం_3 *q;
  పూర్ణాంకం i;

  ఒకవేళ (!parallel_files)
    కొరకు (q = స్తంభం_సదిశ, i = columns; i; ++q, --i)
      {
        ఒకవేళ (storing_columns)
          q->స్థితి_2 = FF_దొరికింది;
        లేకపోతే
          q->స్థితి_2 = ON_HOLD;
      }
  లేకపోతే
    p->స్థితి_2 = ON_HOLD;

  p->పంక్తులు_గమ్యం_print = 0;
  --files_ready_గమ్యం_read;
}

/* Undo hold_file -- go through the column list and change any
   ON_HOLD columns to OPEN.  Used at the end of each page. */

స్థిర శూన్య
reset_స్థితి (శూన్య)
{
  పూర్ణాంకం i = columns;
  స్తంభం_3 *p;

  కొరకు (p = స్తంభం_సదిశ; i; --i, ++p)
    ఒకవేళ (p->స్థితి_2 == ON_HOLD)
      {
        p->స్థితి_2 = OPEN;
        files_ready_గమ్యం_read++;
      }

  ఒకవేళ (storing_columns)
    {
      ఒకవేళ (స్తంభం_సదిశ->స్థితి_2 == CLOSED)
        /* We use the info to output an error message in  skip_to_page. */
        files_ready_గమ్యం_read = 0;
      లేకపోతే
        files_ready_గమ్యం_read = 1;
    }
}

/* Print a single file, or multiple files in parallel.

   Set up the list of columns, opening the necessary files.
   Allocate space for storing columns, if necessary.
   Skip to first_page_number, if user has asked to skip leading pages.
   Determine which functions are appropriate to store/print lines
   in each column.
   Print the file(s). */

స్థిర శూన్య
print_files (పూర్ణాంకం సంఖ్య_యొక్క_files, అక్షరం **av)
{
  init_parameters (సంఖ్య_యొక్క_files);
  ఒకవేళ (! init_fps (సంఖ్య_యొక్క_files, av))
    తిరిగి;
  ఒకవేళ (storing_columns)
    init_store_నిలువుల_సంఖ్య ();

  ఒకవేళ (మొదటి_పుట_సంఖ్య > 1)
    {
      ఒకవేళ (!skip_గమ్యం_పుట (మొదటి_పుట_సంఖ్య))
        తిరిగి;
      లేకపోతే
        పుట_సంఖ్య = మొదటి_పుట_సంఖ్య;
    }
  లేకపోతే
    పుట_సంఖ్య = 1;

  init_funcs ();

  పంక్తి_సంఖ్య = పంక్తి_లెక్క;
  అయితే (print_పుట ())
    ;
}

/* Initialize header information.
   If DESC is non-negative, it is a file descriptor open to
   FILENAME for reading.  */

స్థిర శూన్య
init_శీర్షిక (అక్షరం స్థిరరాశి *filename, పూర్ణాంకం desc)
{
  అక్షరం *buf = ఖాళీ;
  నిర్మాణం stat st;
  నిర్మాణం timespec t;
  పూర్ణాంకం ns;
  నిర్మాణం tm tm;

  /* If parallel files or standard input, use current date. */
  ఒకవేళ (streq (filename, "-"))
    desc = -1;
  ఒకవేళ (0 <= desc && fstat (desc, &st) == 0)
    t = get_stat_mtime (&st);
  లేకపోతే
    {
      స్థిర నిర్మాణం timespec timespec;
      ఒకవేళ (! timespec.tv_sec)
        gettime (&timespec);
      t = timespec;
    }

  ns = t.tv_nsec;
  ఒకవేళ (localtime_rz (localtz, &t.tv_sec, &tm))
    {
      ptrdiff_t len = nstrftime (ఖాళీ, MIN (PTRDIFF_MAX, SIZE_MAX),
                                 date_format, &tm, localtz, ns);
      ఒకవేళ (0 <= len)
        {
          buf = ximalloc (len + 1);
          nstrftime (buf, len + 1, date_format, &tm, localtz, ns);
        }
    }
  ఒకవేళ (!buf)
    {
      అక్షరం secbuf[INT_BUFSIZE_BOUND (intmax_t)];
      buf = xmalloc (పరిమాణం secbuf + MAX (10, INT_BUFSIZE_BOUND (పూర్ణాంకం)));
      sprintf (buf, "%s.%09d", timetostr (t.tv_sec, secbuf), ns);
    }

  విడుదల (date_పాఠ్యం);
  date_పాఠ్యం = buf;
  file_పాఠ్యం = custom_శీర్షిక ? custom_శీర్షిక : desc < 0 ? "" : filename;
  శీర్షిక_వెడల్పు_అందుబాటు = (chars_చొప్పున_పంక్తి
                            - mbswidth (date_పాఠ్యం, 0)
                            - mbswidth (file_పాఠ్యం, 0));
}

/* Set things up for printing a page

   Scan through the columns ...
   Determine which are ready to print
   (i.e., which have lines stored or open files)
   Set p->lines_to_print appropriately
   (to p->lines_stored if we're storing, or lines_per_body
   if we're reading straight from the file)
   Keep track of this total so we know when to stop printing */

స్థిర శూన్య
init_పుట (శూన్య)
{
  పూర్ణాంకం j;
  స్తంభం_3 *p;

  ఒకవేళ (storing_columns)
    {
      store_columns ();
      కొరకు (j = columns - 1, p = స్తంభం_సదిశ; j; --j, ++p)
        {
          p->పంక్తులు_గమ్యం_print = p->పంక్తులు_stored;
        }

      /* Last column. */
      ఒకవేళ (balance_columns)
        {
          p->పంక్తులు_గమ్యం_print = p->పంక్తులు_stored;
        }
      /* Since we're not balancing columns, we don't need to store
         the rightmost column.   Read it straight from the file. */
      లేకపోతే
        {
          ఒకవేళ (p->స్థితి_2 == OPEN)
            {
              p->పంక్తులు_గమ్యం_print = పంక్తులు_చొప్పున_body;
            }
          లేకపోతే
            p->పంక్తులు_గమ్యం_print = 0;
        }
    }
  లేకపోతే
    కొరకు (j = columns, p = స్తంభం_సదిశ; j; --j, ++p)
      ఒకవేళ (p->స్థితి_2 == OPEN)
        {
          p->పంక్తులు_గమ్యం_print = పంక్తులు_చొప్పున_body;
        }
      లేకపోతే
        p->పంక్తులు_గమ్యం_print = 0;
}

/* Align empty columns and print separators.
   Empty columns will be formed by files with status ON_HOLD or CLOSED
   when printing multiple files in parallel. */

స్థిర శూన్య
align_స్తంభం (స్తంభం_3 *p)
{
  padding_not_printed = p->ప్రారంభం_position;
  ఒకవేళ (నిలువు_sep_నిడివి < padding_not_printed)
    {
      pad_across_గమ్యం (padding_not_printed - నిలువు_sep_నిడివి);
      padding_not_printed = ANYWHERE;
    }

  ఒకవేళ (use_నిలువు_separator)
    print_sep_string ();

  ఒకవేళ (p->numbered)
    కూడు_పంక్తి_సంఖ్య (p);
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

స్థిర bool
print_పుట (శూన్య)
{
  పూర్ణాంకం j;
  పూర్ణాంకం పంక్తులు_ఎడమ_on_పుట;
  స్తంభం_3 *p;

  /* Used as an accumulator (with | operator) of successive values of
     pad_vertically.  The trick is to set pad_vertically
     to false before each run through the inner loop, then after that
     loop, it tells us whether a line was actually printed (whether a
     newline needs to be output -- or two for double spacing).  But those
     values have to be accumulated (in pv) so we can invoke pad_down
     properly after the outer loop completes. */
  bool pv;

  init_పుట ();

  ఒకవేళ (నిలువుల_సంఖ్య_ready_గమ్యం_print () == 0)
    తిరిగి false;

  ఒకవేళ (extremities)
    print_a_శీర్షిక = true;

  /* Don't pad unless we know a page was printed. */
  pad_vertically = false;
  pv = false;

  పంక్తులు_ఎడమ_on_పుట = పంక్తులు_చొప్పున_body;
  ఒకవేళ (double_space)
    పంక్తులు_ఎడమ_on_పుట *= 2;

  అయితే (పంక్తులు_ఎడమ_on_పుట > 0 && నిలువుల_సంఖ్య_ready_గమ్యం_print () > 0)
    {
      output_position = 0;
      spaces_not_printed = 0;
      separators_not_printed = 0;
      pad_vertically = false;
      align_ఖాళీలు_నిలువుల_సంఖ్య = false;
      ఖాళీలు_పంక్తి = true;

      కొరకు (j = 1, p = స్తంభం_సదిశ; j <= columns; ++j, ++p)
        {
          ఆదానం_position = 0;
          ఒకవేళ (p->పంక్తులు_గమ్యం_print > 0 || p->స్థితి_2 == FF_దొరికింది)
            {
              FF_only = false;
              padding_not_printed = p->ప్రారంభం_position;
              ఒకవేళ (!(p->print_func) (p))
                read_rest_యొక్క_పంక్తి (p);
              pv |= pad_vertically;

              --p->పంక్తులు_గమ్యం_print;
              ఒకవేళ (p->పంక్తులు_గమ్యం_print <= 0)
                {
                  ఒకవేళ (నిలువుల_సంఖ్య_ready_గమ్యం_print () == 0)
                    ఆపు;
                }

              /* File p changed its status to ON_HOLD or CLOSED */
              ఒకవేళ (parallel_files && p->స్థితి_2 != OPEN)
                {
                  ఒకవేళ (ఖాళీలు_పంక్తి)
                    align_ఖాళీలు_నిలువుల_సంఖ్య = true;
                  లేకపోతే ఒకవేళ (p->స్థితి_2 == CLOSED
                           || (p->స్థితి_2 == ON_HOLD && FF_only))
                    align_స్తంభం (p);
                }
            }
          లేకపోతే ఒకవేళ (parallel_files)
            {
              /* File status ON_HOLD or CLOSED */
              ఒకవేళ (ఖాళీలు_పంక్తి)
                align_ఖాళీలు_నిలువుల_సంఖ్య = true;
              లేకపోతే
                align_స్తంభం (p);
            }

          /* We need it also with an empty column */
          ఒకవేళ (use_నిలువు_separator)
            ++separators_not_printed;
        }

      ఒకవేళ (pad_vertically)
        {
          అక్షరం_రాయి ('\n');
          --పంక్తులు_ఎడమ_on_పుట;
        }

      ఒకవేళ (నిలువుల_సంఖ్య_ready_గమ్యం_print () == 0 && !extremities)
        ఆపు;

      ఒకవేళ (double_space && pv)
        {
          అక్షరం_రాయి ('\n');
          --పంక్తులు_ఎడమ_on_పుట;
        }
    }

  ఒకవేళ (పంక్తులు_ఎడమ_on_పుట == 0)
    కొరకు (j = 1, p = స్తంభం_సదిశ; j <= columns; ++j, ++p)
      ఒకవేళ (p->స్థితి_2 == OPEN)
        p->నిండిన_పుట_printed = true;

  pad_vertically = pv;

  ఒకవేళ (pad_vertically && extremities)
    pad_కిందికి (పంక్తులు_ఎడమ_on_పుట + పంక్తులు_చొప్పున_footer);
  లేకపోతే ఒకవేళ (keep_FF && print_a_FF)
    {
      అక్షరం_రాయి ('\f');
      print_a_FF = false;
    }

  ఒకవేళ (ferror (stdout))
    write_error ();

  ఒకవేళ (చివరి_పుట_సంఖ్య < ++పుట_సంఖ్య)
    తిరిగి false;		/* Stop printing with LAST_PAGE */

  reset_స్థితి ();		/* Change ON_HOLD to OPEN. */

  తిరిగి true;			/* More pages to go. */
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

స్థిర శూన్య
init_store_నిలువుల_సంఖ్య (శూన్య)
{
  /* Tune this.  */
  పూర్ణాంకం మొత్తం_పంక్తులు, మొత్తం_పంక్తులు_1, chars_చొప్పున_స్తంభం_1, chars_if_truncate;
  ఒకవేళ (ckd_mul (&మొత్తం_పంక్తులు, పంక్తులు_చొప్పున_body, columns)
      || ckd_కూడు (&మొత్తం_పంక్తులు_1, మొత్తం_పంక్తులు, 1)
      || ckd_కూడు (&chars_చొప్పున_స్తంభం_1, chars_చొప్పున_స్తంభం, 1)
      || ckd_mul (&chars_if_truncate, మొత్తం_పంక్తులు, chars_చొప్పున_స్తంభం_1)
      || ckd_mul (&buff_allocated, chars_if_truncate, use_నిలువు_separator + 1))
    integer_అధికప్రవాహం ();

  విడుదల (పంక్తి_సదిశ);
  /* FIXME: here's where it was allocated.  */
  పంక్తి_సదిశ = xnmalloc (మొత్తం_పంక్తులు_1, పరిమాణం *పంక్తి_సదిశ);

  విడుదల (చివర_సదిశ);
  చివర_సదిశ = xnmalloc (మొత్తం_పంక్తులు, పరిమాణం *చివర_సదిశ);

  విడుదల (buff);
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

స్థిర శూన్య
store_columns (శూన్య)
{
  పూర్ణాంకం i, j;
  గుర్తులేని పూర్ణాంకం పంక్తి = 0;
  గుర్తులేని పూర్ణాంకం buff_ప్రారంభం;
  పూర్ణాంకం చివరి_నిలువు;		/* The rightmost column which will be saved in buff */
  స్తంభం_3 *p;

  buff_current = 0;
  buff_ప్రారంభం = 0;

  ఒకవేళ (balance_columns)
    చివరి_నిలువు = columns;
  లేకపోతే
    చివరి_నిలువు = columns - 1;

  కొరకు (i = 1, p = స్తంభం_సదిశ; i <= చివరి_నిలువు; ++i, ++p)
    p->పంక్తులు_stored = 0;

  కొరకు (i = 1, p = స్తంభం_సదిశ; i <= చివరి_నిలువు && files_ready_గమ్యం_read;
       ++i, ++p)
    {
      p->current_పంక్తి = పంక్తి;
      కొరకు (j = పంక్తులు_చొప్పున_body; j && files_ready_గమ్యం_read; --j)

        ఒకవేళ (p->స్థితి_2 == OPEN)	/* Redundant.  Clean up. */
          {
            ఆదానం_position = 0;

            ఒకవేళ (!read_పంక్తి (p))
              read_rest_యొక్క_పంక్తి (p);

            ఒకవేళ (p->స్థితి_2 == OPEN
                || buff_ప్రారంభం != buff_current)
              {
                ++p->పంక్తులు_stored;
                పంక్తి_సదిశ[పంక్తి] = buff_ప్రారంభం;
                చివర_సదిశ[పంక్తి++] = ఆదానం_position;
                buff_ప్రారంభం = buff_current;
              }
          }
    }

  /* Keep track of the location of the last char in buff. */
  పంక్తి_సదిశ[పంక్తి] = buff_ప్రారంభం;

  ఒకవేళ (balance_columns)
    balance (పంక్తి);
}

స్థిర శూన్య
balance (పూర్ణాంకం మొత్తం_stored)
{
  స్తంభం_3 *p;
  పూర్ణాంకం i, పంక్తులు;
  పూర్ణాంకం మొదటి_పంక్తి = 0;

  కొరకు (i = 1, p = స్తంభం_సదిశ; i <= columns; ++i, ++p)
    {
      పంక్తులు = మొత్తం_stored / columns;
      ఒకవేళ (i <= మొత్తం_stored % columns)
        ++పంక్తులు;

      p->పంక్తులు_stored = పంక్తులు;
      p->current_పంక్తి = మొదటి_పంక్తి;

      మొదటి_పంక్తి += పంక్తులు;
    }
}

/* Store a character in the buffer. */

స్థిర శూన్య
store_char (అక్షరం c)
{
  ఒకవేళ (buff_current >= buff_allocated)
    {
      /* May be too generous. */
      buff = xpalloc (buff, &buff_allocated, 1, -1, పరిమాణం *buff);
    }
  buff[buff_current++] = c;
}

స్థిర శూన్య
కూడు_పంక్తి_సంఖ్య (స్తంభం_3 *p)
{
  పూర్ణాంకం i;
  అక్షరం *s;
  పూర్ణాంకం అంకె_వెడల్పు;

  /* Cutting off the higher-order digits is more informative than
     lower-order cut off. */
  అంకె_వెడల్పు = sprintf (సంఖ్య_buff, "%*d", chars_చొప్పున_సంఖ్య, పంక్తి_సంఖ్య);
  పంక్తి_సంఖ్య++;
  s = సంఖ్య_buff + (అంకె_వెడల్పు - chars_చొప్పున_సంఖ్య);
  కొరకు (i = chars_చొప్పున_సంఖ్య; i > 0; i--)
    (p->char_func) (*s++);

  ఒకవేళ (columns > 1)
    {
      /* Tabification is assumed for multiple columns, also for n-separators,
         but 'default n-separator = TAB' hasn't been given priority over
         equal column_width also specified by POSIX. */
      ఒకవేళ (సంఖ్య_separator == '\t')
        {
          i = సంఖ్య_వెడల్పు - chars_చొప్పున_సంఖ్య;
          అయితే (i-- > 0)
            (p->char_func) (' ');
        }
      లేకపోతే
        (p->char_func) (సంఖ్య_separator);
    }
  లేకపోతే
    /* To comply with POSIX, we avoid any expansion of default TAB
       separator with a single column output. No column_width requirement
       has to be considered. */
    {
      (p->char_func) (సంఖ్య_separator);
      ఒకవేళ (సంఖ్య_separator == '\t')
        output_position = POS_AFTER_TAB (chars_చొప్పున_output_tab,
                          output_position);
    }

  ఒకవేళ (truncate_పంక్తులు && !parallel_files)
    ఆదానం_position += సంఖ్య_వెడల్పు;
}

/* Print (or store) padding until the current horizontal position
   is position. */

స్థిర శూన్య
pad_across_గమ్యం (పూర్ణాంకం position)
{
  పూర్ణాంకం h = output_position;

  ఒకవేళ (tabify_output)
    spaces_not_printed = position - output_position;
  లేకపోతే
    {
      అయితే (++h <= position)
        అక్షరం_రాయి (' ');
      output_position = position;
    }
}

/* Pad to the bottom of the page.

   If the user has requested a formfeed, use one.
   Otherwise, use newlines. */

స్థిర శూన్య
pad_కిందికి (గుర్తులేని పూర్ణాంకం పంక్తులు)
{
  ఒకవేళ (use_form_feed)
    అక్షరం_రాయి ('\f');
  లేకపోతే
    కొరకు (గుర్తులేని పూర్ణాంకం i = పంక్తులు; i; --i)
      అక్షరం_రాయి ('\n');
}

/* Read the rest of the line.

   Read from the current column's file until an end of line is
   hit.  Used when we've truncated a line and we no longer need
   to print or store its characters. */

స్థిర శూన్య
read_rest_యొక్క_పంక్తి (స్తంభం_3 *p)
{
  పూర్ణాంకం c;
  FILE *f = p->fp;

  అయితే ((c = getc (f)) != '\n')
    {
      ఒకవేళ (c == '\f')
        {
          ఒకవేళ ((c = getc (f)) != '\n')
            ungetc (c, f);
          ఒకవేళ (keep_FF)
            print_a_FF = true;
          hold_file (p);
          ఆపు;
        }
      లేకపోతే ఒకవేళ (c == EOF)
        {
          close_file (p);
          ఆపు;
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

స్థిర శూన్య
skip_read (స్తంభం_3 *p, పూర్ణాంకం స్తంభం_సంఖ్య)
{
  పూర్ణాంకం c;
  FILE *f = p->fp;
  పూర్ణాంకం i;
  bool single_ff = false;
  స్తంభం_3 *q;

  /* Read 1st character in a line or any character succeeding a FF */
  ఒకవేళ ((c = getc (f)) == '\f' && p->నిండిన_పుట_printed)
    /* A FF-coincidence with a previous full_page_printed.
       To avoid an additional empty page, eliminate the FF */
    ఒకవేళ ((c = getc (f)) == '\n')
      c = getc (f);

  p->నిండిన_పుట_printed = false;

  /* 1st character a FF means a single FF without any printable
     characters. Don't count it as a line with -n option. */
  ఒకవేళ (c == '\f')
    single_ff = true;

  /* Preparing for a FF-coincidence: Maybe we finish that page
     without a FF found */
  ఒకవేళ (చివరి_పంక్తి)
    p->నిండిన_పుట_printed = true;

  అయితే (c != '\n')
    {
      ఒకవేళ (c == '\f')
        {
          /* No FF-coincidence possible,
             no catching up of a FF-coincidence with next page */
          ఒకవేళ (చివరి_పంక్తి)
            {
              ఒకవేళ (!parallel_files)
                కొరకు (q = స్తంభం_సదిశ, i = columns; i; ++q, --i)
                  q->నిండిన_పుట_printed = false;
              లేకపోతే
                p->నిండిన_పుట_printed = false;
            }

          ఒకవేళ ((c = getc (f)) != '\n')
            ungetc (c, f);
          hold_file (p);
          ఆపు;
        }
      లేకపోతే ఒకవేళ (c == EOF)
        {
          close_file (p);
          ఆపు;
        }
      c = getc (f);
    }

  ఒకవేళ (skip_లెక్క)
    ఒకవేళ ((!parallel_files || స్తంభం_సంఖ్య == 1) && !single_ff)
      ++పంక్తి_లెక్క;
}

/* If we're tabifying output,

   When print_char encounters white space it keeps track
   of our desired horizontal position and delays printing
   until this function is called. */

స్థిర శూన్య
print_white_space (శూన్య)
{
  పూర్ణాంకం h_new;
  పూర్ణాంకం h_old = output_position;
  పూర్ణాంకం goal = h_old + spaces_not_printed;

  అయితే (goal - h_old > 1
         && (h_new = POS_AFTER_TAB (chars_చొప్పున_output_tab, h_old)) <= goal)
    {
      అక్షరం_రాయి (output_tab_char);
      h_old = h_new;
    }
  అయితే (++h_old <= goal)
    అక్షరం_రాయి (' ');

  output_position = goal;
  spaces_not_printed = 0;
}

/* Print column separators.

   We keep a count until we know that we'll be printing a line,
   then print_sep_string() is called. */

స్థిర శూన్య
print_sep_string (శూన్య)
{
  అక్షరం స్థిరరాశి *s = నిలువు_sep_string;
  పూర్ణాంకం l = నిలువు_sep_నిడివి;

  ఒకవేళ (separators_not_printed <= 0)
    {
      /* We'll be starting a line with chars_per_margin, anything else? */
      ఒకవేళ (spaces_not_printed > 0)
        print_white_space ();
    }
  లేకపోతే
    {
      కొరకు (; separators_not_printed > 0; --separators_not_printed)
        {
          అయితే (l-- > 0)
            {
              /* 3 types of sep_strings: spaces only, spaces and chars,
              chars only */
              ఒకవేళ (*s == ' ')
                {
                  /* We're tabifying output; consecutive spaces in
                  sep_string may have to be converted to tabs */
                  s++;
                  ++spaces_not_printed;
                }
              లేకపోతే
                {
                  ఒకవేళ (spaces_not_printed > 0)
                    print_white_space ();
                  అక్షరం_రాయి (*s++);
                  ++output_position;
                }
            }
          /* sep_string ends with some spaces */
          ఒకవేళ (spaces_not_printed > 0)
            print_white_space ();
        }
    }
}

/* Print (or store, depending on p->char_func) a clump of N
   characters. */

స్థిర శూన్య
print_clump (స్తంభం_3 *p, పూర్ణాంకం n, అక్షరం *clump)
{
  అయితే (n--)
    (p->char_func) (*clump++);

  ఒకవేళ (ferror (stdout))
    write_error ();
}

/* Print a character.

   Update the following comment: process-char hasn't been used any
   longer.
   If we're tabifying, all tabs have been converted to spaces by
   process_char().  Keep a count of consecutive spaces, and when
   a nonspace is encountered, call print_white_space() to print the
   required number of tabs and spaces. */

స్థిర శూన్య
print_char (అక్షరం c)
{
  ఒకవేళ (tabify_output)
    {
      ఒకవేళ (c == ' ')
        {
          ++spaces_not_printed;
          తిరిగి;
        }
      లేకపోతే ఒకవేళ (spaces_not_printed > 0)
        print_white_space ();

      /* Nonprintables are assumed to have width 0, except '\b'. */
      ఒకవేళ (! isprint (గమ్యం_uchar (c)))
        {
          ఒకవేళ (c == '\b')
            --output_position;
        }
      లేకపోతే
        ++output_position;
    }
  అక్షరం_రాయి (c);
}

/* Skip to page PAGE before printing.
   PAGE may be larger than total number of pages. */

స్థిర bool
skip_గమ్యం_పుట (uintmax_t పుట)
{
  కొరకు (uintmax_t n = 1; n < పుట; ++n)
    {
      స్తంభం_3 *p;
      పూర్ణాంకం j;

      కొరకు (పూర్ణాంకం i = 1; i < పంక్తులు_చొప్పున_body; ++i)
        {
          కొరకు (j = 1, p = స్తంభం_సదిశ; j <= columns; ++j, ++p)
            ఒకవేళ (p->స్థితి_2 == OPEN)
              skip_read (p, j);
        }
      చివరి_పంక్తి = true;
      కొరకు (j = 1, p = స్తంభం_సదిశ; j <= columns; ++j, ++p)
        ఒకవేళ (p->స్థితి_2 == OPEN)
          skip_read (p, j);

      ఒకవేళ (storing_columns)	/* change FF_FOUND to ON_HOLD */
        కొరకు (j = 1, p = స్తంభం_సదిశ; j <= columns; ++j, ++p)
          ఒకవేళ (p->స్థితి_2 != CLOSED)
            p->స్థితి_2 = ON_HOLD;

      reset_స్థితి ();
      చివరి_పంక్తి = false;

      ఒకవేళ (files_ready_గమ్యం_read < 1)
        {
          /* It's very helpful, normally the total number of pages is
             not known in advance.  */
          error (0, 0,
                 _("starting page number %ju exceeds page count %ju"),
                 పుట, n);
          ఆపు;
        }
    }
  తిరిగి files_ready_గమ్యం_read > 0;
}

/* Print a header.

   Formfeeds are assumed to use up two lines at the beginning of
   the page. */

స్థిర శూన్య
print_శీర్షిక (శూన్య)
{
  అక్షరం పుట_పాఠ్యం[256 + INT_STRLEN_BOUND (పుట_సంఖ్య)];
  పూర్ణాంకం అందుబాటు_వెడల్పు;
  పూర్ణాంకం lhs_spaces;
  పూర్ణాంకం rhs_spaces;

  output_position = 0;
  pad_across_గమ్యం (chars_చొప్పున_margin);
  print_white_space ();

  ఒకవేళ (పుట_సంఖ్య == 0)
    error (EXIT_FAILURE, 0, _("page number overflow"));

  /* The translator must ensure that formatting the translation of
     "Page %ju" does not generate more than (sizeof page_text - 1)
     bytes.  */
  sprintf (పుట_పాఠ్యం, _("Page %ju"), పుట_సంఖ్య);
  అందుబాటు_వెడల్పు = శీర్షిక_వెడల్పు_అందుబాటు - mbswidth (పుట_పాఠ్యం, 0);
  అందుబాటు_వెడల్పు = MAX (0, అందుబాటు_వెడల్పు);
  lhs_spaces = అందుబాటు_వెడల్పు >> 1;
  rhs_spaces = అందుబాటు_వెడల్పు - lhs_spaces;

  రాయి ("\n\n%*s%s%*s%s%*s%s\n\n\n",
          chars_చొప్పున_margin, "",
          date_పాఠ్యం, lhs_spaces, " ",
          file_పాఠ్యం, rhs_spaces, " ", పుట_పాఠ్యం);

  print_a_శీర్షిక = false;
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

స్థిర bool
read_పంక్తి (స్తంభం_3 *p)
{
  పూర్ణాంకం c;
  పూర్ణాంకం chars IF_LINT (= 0);
  పూర్ణాంకం చివరి_ఆదానం_position;
  పూర్ణాంకం j, k;
  స్తంభం_3 *q;

  /* read 1st character in each line or any character succeeding a FF: */
  c = getc (p->fp);

  చివరి_ఆదానం_position = ఆదానం_position;

  ఒకవేళ (c == '\f' && p->నిండిన_పుట_printed)
    ఒకవేళ ((c = getc (p->fp)) == '\n')
      c = getc (p->fp);
  p->నిండిన_పుట_printed = false;

  ఎంపిక (c)
    {
    స్థితి '\f':
      ఒకవేళ ((c = getc (p->fp)) != '\n')
        ungetc (c, p->fp);
      FF_only = true;
      ఒకవేళ (print_a_శీర్షిక && !storing_columns)
        {
          pad_vertically = true;
          print_శీర్షిక ();
        }
      లేకపోతే ఒకవేళ (keep_FF)
        print_a_FF = true;
      hold_file (p);
      తిరిగి true;
    స్థితి EOF:
      close_file (p);
      తిరిగి true;
    స్థితి '\n':
      ఆపు;
    అప్రమేయ:
      chars = char_గమ్యం_clump (c);
    }

  ఒకవేళ (truncate_పంక్తులు && ఆదానం_position > chars_చొప్పున_స్తంభం)
    {
      ఆదానం_position = చివరి_ఆదానం_position;
      తిరిగి false;
    }

  ఒకవేళ (p->char_func != store_char)
    {
      pad_vertically = true;

      ఒకవేళ (print_a_శీర్షిక && !storing_columns)
        print_శీర్షిక ();

      ఒకవేళ (parallel_files && align_ఖాళీలు_నిలువుల_సంఖ్య)
        {
          /* We have to align empty columns at the beginning of a line. */
          k = separators_not_printed;
          separators_not_printed = 0;
          కొరకు (j = 1, q = స్తంభం_సదిశ; j <= k; ++j, ++q)
            {
              align_స్తంభం (q);
              separators_not_printed += 1;
            }
          padding_not_printed = p->ప్రారంభం_position;
          ఒకవేళ (truncate_పంక్తులు)
            spaces_not_printed = chars_చొప్పున_స్తంభం;
          లేకపోతే
            spaces_not_printed = 0;
          align_ఖాళీలు_నిలువుల_సంఖ్య = false;
        }

      ఒకవేళ (నిలువు_sep_నిడివి < padding_not_printed)
        {
          pad_across_గమ్యం (padding_not_printed - నిలువు_sep_నిడివి);
          padding_not_printed = ANYWHERE;
        }

      ఒకవేళ (use_నిలువు_separator)
        print_sep_string ();
    }

  ఒకవేళ (p->numbered)
    కూడు_పంక్తి_సంఖ్య (p);

  ఖాళీలు_పంక్తి = false;
  ఒకవేళ (c == '\n')
    తిరిగి true;

  print_clump (p, chars, clump_buff);

  అయితే (true)
    {
      c = getc (p->fp);

      ఎంపిక (c)
        {
        స్థితి '\n':
          తిరిగి true;
        స్థితి '\f':
          ఒకవేళ ((c = getc (p->fp)) != '\n')
            ungetc (c, p->fp);
          ఒకవేళ (keep_FF)
            print_a_FF = true;
          hold_file (p);
          తిరిగి true;
        స్థితి EOF:
          close_file (p);
          తిరిగి true;
        }

      చివరి_ఆదానం_position = ఆదానం_position;
      chars = char_గమ్యం_clump (c);
      ఒకవేళ (truncate_పంక్తులు && ఆదానం_position > chars_చొప్పున_స్తంభం)
        {
          ఆదానం_position = చివరి_ఆదానం_position;
          తిరిగి false;
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

స్థిర bool
print_stored (స్తంభం_3 *p)
{
  స్తంభం_3 *q;

  పూర్ణాంకం పంక్తి = p->current_పంక్తి++;
  అక్షరం *మొదటి = &buff[పంక్తి_సదిశ[పంక్తి]];
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
  అక్షరం *చివరి = &buff[పంక్తి_సదిశ[పంక్తి + 1]];

  pad_vertically = true;

  ఒకవేళ (print_a_శీర్షిక)
    print_శీర్షిక ();

  ఒకవేళ (p->స్థితి_2 == FF_దొరికింది)
    {
      పూర్ణాంకం i;
      కొరకు (i = 1, q = స్తంభం_సదిశ; i <= columns; ++i, ++q)
        q->స్థితి_2 = ON_HOLD;
      ఒకవేళ (స్తంభం_సదిశ->పంక్తులు_గమ్యం_print <= 0)
        {
          ఒకవేళ (!extremities)
            pad_vertically = false;
          తిరిగి true;		/* print a header only */
        }
    }

  ఒకవేళ (నిలువు_sep_నిడివి < padding_not_printed)
    {
      pad_across_గమ్యం (padding_not_printed - నిలువు_sep_నిడివి);
      padding_not_printed = ANYWHERE;
    }

  ఒకవేళ (use_నిలువు_separator)
    print_sep_string ();

  అయితే (మొదటి != చివరి)
    print_char (*మొదటి++);

  ఒకవేళ (spaces_not_printed == 0)
    {
      output_position = p->ప్రారంభం_position + చివర_సదిశ[పంక్తి];
      ఒకవేళ (p->ప్రారంభం_position - నిలువు_sep_నిడివి == chars_చొప్పున_margin)
        output_position -= నిలువు_sep_నిడివి;
    }

  తిరిగి true;
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

స్థిర పూర్ణాంకం
char_గమ్యం_clump (అక్షరం c)
{
  గుర్తులేని అక్షరం uc = c;
  అక్షరం *s = clump_buff;
  అక్షరం esc_buff[4];
  పూర్ణాంకం వెడల్పు;
  పూర్ణాంకం chars;
  పూర్ణాంకం chars_చొప్పున_c = 8;

  ఒకవేళ (c == ఆదానం_tab_char)
    chars_చొప్పున_c = chars_చొప్పున_ఆదానం_tab;

  ఒకవేళ (c == ఆదానం_tab_char || c == '\t')
    {
      వెడల్పు = TAB_వెడల్పు (chars_చొప్పున_c, ఆదానం_position);

      ఒకవేళ (untabify_ఆదానం)
        {
          కొరకు (పూర్ణాంకం i = వెడల్పు; i; --i)
            *s++ = ' ';
          chars = వెడల్పు;
        }
      లేకపోతే
        {
          *s = c;
          chars = 1;
        }

    }
  లేకపోతే ఒకవేళ (! isprint (uc))
    {
      ఒకవేళ (use_esc_sequence)
        {
          వెడల్పు = 4;
          chars = 4;
          *s++ = '\\';
          sprintf (esc_buff, "%03o", uc);
          కొరకు (పూర్ణాంకం i = 0; i <= 2; ++i)
            *s++ = esc_buff[i];
        }
      లేకపోతే ఒకవేళ (use_cntrl_ఉపసర్గ)
        {
          ఒకవేళ (uc < 0200)
            {
              వెడల్పు = 2;
              chars = 2;
              *s++ = '^';
              *s = c ^ 0100;
            }
          లేకపోతే
            {
              వెడల్పు = 4;
              chars = 4;
              *s++ = '\\';
              sprintf (esc_buff, "%03o", uc);
              కొరకు (పూర్ణాంకం i = 0; i <= 2; ++i)
                *s++ = esc_buff[i];
            }
        }
      లేకపోతే ఒకవేళ (c == '\b')
        {
          వెడల్పు = -1;
          chars = 1;
          *s = c;
        }
      లేకపోతే
        {
          వెడల్పు = 0;
          chars = 1;
          *s = c;
        }
    }
  లేకపోతే
    {
      వెడల్పు = 1;
      chars = 1;
      *s = c;
    }

  /* Too many backspaces must put us in position 0 -- never negative.  */
  ఒకవేళ (వెడల్పు < 0 && ఆదానం_position == 0)
    {
      chars = 0;
      ఆదానం_position = 0;
    }
  లేకపోతే ఒకవేళ (వెడల్పు < 0 && ఆదానం_position <= -వెడల్పు)
    ఆదానం_position = 0;
  లేకపోతే
    {
      ఒకవేళ (ckd_కూడు (&ఆదానం_position, ఆదానం_position, వెడల్పు))
        integer_అధికప్రవాహం ();
    }

  తిరిగి chars;
}

/* We've just printed some files and need to clean up things before
   looking for more options and printing the next batch of files.

   Free everything we've xmalloc'ed, except 'header'. */

స్థిర శూన్య
cleanup (శూన్య)
{
  విడుదల (సంఖ్య_buff);
  విడుదల (clump_buff);
  విడుదల (స్తంభం_సదిశ);
  విడుదల (పంక్తి_సదిశ);
  విడుదల (చివర_సదిశ);
  విడుదల (buff);
}

/* Complain, print a usage message, and die. */

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              కార్యక్రమం_name);

      fputs (_("\
Paginate or columnate FILE(s) కొరకు printing.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\n\
  +మొదటి_పుట[:చివరి_పుట], --పుటలు=మొదటి_పుట[:చివరి_పుట]\n\
         begin [stop] printing with పుట మొదటి_2[చివరి_2]పుట_2\n\
"));
      oputs (_("\
  -నిలువుల_సంఖ్య, --columns=నిలువుల_సంఖ్య\n\
         output నిలువుల_సంఖ్య columns and print columns కిందికి, unless -a is వాడిన.\n\
         Balance సంఖ్య యొక్క పంక్తులు లోపల the columns on each పుట\n\
"));
      oputs (_("\
  -a, --across\n\
         print columns across rather than కిందికి, వాడిన together with -నిలువుల_సంఖ్య\n\
"));
      oputs (_("\
  -c, --show-control-chars\n\
         use hat notation (^G) and octal backslash notation\n\
"));
      oputs (_("\
  -d, --ద్విగుణ-space\n\
         ద్విగుణ space the output\n\
"));
      oputs (_("\
  -D, --date-format=FORMAT\n\
         use FORMAT కొరకు the శీర్షిక date\n\
"));
      oputs (_("\
  -e[CHAR[వెడల్పు_2]], --expand-tabs[=CHAR[వెడల్పు_2]]\n\
         expand ఆదానం CHARs (TABs) గమ్యం tab వెడల్పు_2 (8)\n\
"));
      oputs (_("\
  -F, -f, --form-feed\n\
         use form feeds instead యొక్క newlines గమ్యం separate పుటలు\n\
         (by a 3-పంక్తి పుట శీర్షిక with -F or a 5-పంక్తి శీర్షిక\n\
         and trailer without -F)\n\
"));
      oputs (_("\
  -h, --శీర్షిక=శీర్షిక_3\n\
         use a centered శీర్షిక_3 instead యొక్క filename లోపల పుట శీర్షిక,\n\
         -h \"\" prints a blank పంక్తి, don't use -h\"\"\n\
"));
      oputs (_("\
  -i[CHAR[వెడల్పు_2]], --output-tabs[=CHAR[వెడల్పు_2]]\n\
         replace spaces with CHARs (TABs) గమ్యం tab వెడల్పు_2 (8)\n\
"));
      oputs (_("\
  -J, --join-పంక్తులు\n\
         విలీనం నిండిన పంక్తులు, turns off -W పంక్తి truncation,\n\
         no స్తంభం alignment, --sep-string[=STRING] sets separators\n\
"));
      oputs (_("\
  -l, --నిడివి=పుట_నిడివి\n\
         సమితి_2 the పుట నిడివి గమ్యం పుట_నిడివి (66) పంక్తులు\n\
         (అప్రమేయ సంఖ్య యొక్క పంక్తులు యొక్క పాఠ్యం 56, and with -F 63).\n\
         సూచిస్తుంది -t ఒకవేళ పుట_నిడివి <= 10\n\
"));
      oputs (_("\
  -m, --విలీనం\n\
         print all files లోపల parallel, one లోపల each స్తంభం,\n\
         truncate పంక్తులు, but join పంక్తులు యొక్క నిండిన నిడివి with -J\n\
"));
      oputs (_("\
  -n[SEP[DIGITS]], --సంఖ్య-పంక్తులు[=SEP[DIGITS]]\n\
         సంఖ్య పంక్తులు, use DIGITS (5) digits, then SEP (TAB),\n\
         అప్రమేయ counting starts with 1st పంక్తి యొక్క ఆదానం file\n\
"));
      oputs (_("\
  -N, --మొదటి-పంక్తి-సంఖ్య=సంఖ్య_2\n\
         ప్రారంభం counting with సంఖ్య_2 at 1st పంక్తి యొక్క మొదటి\n\
         పుట printed (see +మొదటి_పుట)\n\
"));
      oputs (_("\
  -o, --indent=MARGIN\n\
         స్థానభ్రంశం each పంక్తి with MARGIN (zero) spaces, చేయి not\n\
         affect -w or -W, MARGIN will be added గమ్యం పుట_వెడల్పు\n\
"));
      oputs (_("\
  -r, --no-file-warnings\n\
         omit warning when a file cannot be opened\n\
"));
      oputs (_("\
  -s[CHAR], --separator[=CHAR]\n\
         separate columns by a single character, అప్రమేయ కొరకు CHAR\n\
         is the <TAB> character without -w and \'no అక్షరం\' with -w.\n\
         -s[CHAR] turns off పంక్తి truncation యొక్క all 3 స్తంభం\n\
         options (-నిలువుల_సంఖ్య|-a -నిలువుల_సంఖ్య|-m) except -w is సమితి_2\n\
"));
      oputs (_("\
  -S[STRING], --sep-string[=STRING]\n\
         separate columns by STRING,\n\
         without -S: Default separator <TAB> with -J and <space>\n\
         otherwise (same as -S\" \"), no effect on స్తంభం options\n\
"));
      oputs (_("\
  -t, --omit-శీర్షిక\n\
         omit పుట శీర్షికలు and trailers; implied ఒకవేళ పుట_నిడివి <= 10\n\
"));
      oputs (_("\
  -T, --omit-pagination\n\
         omit పుట శీర్షికలు and trailers,\n\
         eliminate any pagination by form feeds సమితి_2 లోపల ఆదానం files\n\
"));
      oputs (_("\
  -v, --show-nonprinting\n\
         use octal backslash notation\n\
"));
      oputs (_("\
  -w, --వెడల్పు=పుట_వెడల్పు\n\
         సమితి_2 పుట వెడల్పు గమ్యం పుట_వెడల్పు (72) characters కొరకు\n\
         గుణిజం పాఠ్యం-స్తంభం output only, -s[అక్షరం] turns off (72)\n\
"));
      oputs (_("\
  -W, --పుట-వెడల్పు=పుట_వెడల్పు\n\
         సమితి_2 పుట వెడల్పు గమ్యం పుట_వెడల్పు (72) characters always,\n\
         truncate పంక్తులు, except -J option is సమితి_2,\n\
         no interference with -S or -s\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}
