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

#അന്തര്ഭാവ <config.h>

#അന്തര്ഭാവ <getopt.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "c-ctype.h"
#അന്തര്ഭാവ "fadvise.h"
#അന്തര്ഭാവ "hard-locale.h"
#അന്തര്ഭാവ "mbswidth.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "stat-time.h"
#അന്തര്ഭാവ "stdio--.h"
#അന്തര്ഭാവ "strftime.h"
#അന്തര്ഭാവ "xstrtol.h"
#അന്തര്ഭാവ "xstrtol-error.h"
#അന്തര്ഭാവ "xdectoint.h"

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

ഘടന COLUMN;
ഘടന COLUMN
  {
    FILE *fp;			/* Input stream for this column. */
    അക്ഷരം സ്ഥിരം *name;		/* File name. */
    എണ്ണൽ
      {
        OPEN,
        FF_FOUND,		/* used with -b option, set with \f, changed
                                   to ON_HOLD after print_header */
        ON_HOLD,		/* Hit a form feed. */
        CLOSED
      }
    status;			/* Status of the file pointer. */

    /* Func to print lines in this col. */
    bool (*print_func) (ഘടന COLUMN *);

    /* Func to print/store chars in this col. */
    ശൂന്യം (*char_func) (അക്ഷരം);

    പൂർണ്ണസംഖ്യ current_വരി;		/* Index of current place in line_vector. */
    പൂർണ്ണസംഖ്യ വരികൾ_stored;		/* Number of lines stored in buff. */
    പൂർണ്ണസംഖ്യ വരികൾ_to_print;		/* No. lines stored or space left on page. */
    പൂർണ്ണസംഖ്യ start_position;		/* Horizontal position of first char. */
    bool numbered;
    bool full_page_printed;	/* True means printed without a FF found. */

    /* p->full_page_printed  controls a special case of "FF set by hand":
       True means a full page has been printed without FF found. To avoid an
       additional empty page we have to ignore a FF immediately following in
       the next line. */
  };

തരനിർവചനം ഘടന COLUMN COLUMN;

സ്ഥിതം പൂർണ്ണസംഖ്യ char_to_clump (അക്ഷരം c);
സ്ഥിതം bool read_വരി (COLUMN *p);
സ്ഥിതം bool print_page (ശൂന്യം);
സ്ഥിതം bool print_stored (COLUMN *p);
സ്ഥിതം bool open_file (അക്ഷരം *name, COLUMN *p);
സ്ഥിതം bool skip_to_page (uintmax_t page);
സ്ഥിതം ശൂന്യം print_header (ശൂന്യം);
സ്ഥിതം ശൂന്യം pad_across_to (പൂർണ്ണസംഖ്യ position);
സ്ഥിതം ശൂന്യം add_വരി_സംഖ്യ (COLUMN *p);
സ്ഥിതം പൂർണ്ണസംഖ്യ getoptnum (അക്ഷരം സ്ഥിരം *n_str, പൂർണ്ണസംഖ്യ min, അക്ഷരം സ്ഥിരം *errfmt);
സ്ഥിതം ശൂന്യം getoptarg (അക്ഷരം *arg, അക്ഷരം switch_char, അക്ഷരം *character,
                       പൂർണ്ണസംഖ്യ *സംഖ്യ);
സ്ഥിതം ശൂന്യം print_files (പൂർണ്ണസംഖ്യ സംഖ്യ_of_files, അക്ഷരം **av);
സ്ഥിതം ശൂന്യം init_parameters (പൂർണ്ണസംഖ്യ സംഖ്യ_of_files);
സ്ഥിതം ശൂന്യം init_header (അക്ഷരം സ്ഥിരം *filename, പൂർണ്ണസംഖ്യ desc);
സ്ഥിതം bool init_fps (പൂർണ്ണസംഖ്യ സംഖ്യ_of_files, അക്ഷരം **av);
സ്ഥിതം ശൂന്യം init_funcs (ശൂന്യം);
സ്ഥിതം ശൂന്യം init_store_cols (ശൂന്യം);
സ്ഥിതം ശൂന്യം store_columns (ശൂന്യം);
സ്ഥിതം ശൂന്യം balance (പൂർണ്ണസംഖ്യ total_stored);
സ്ഥിതം ശൂന്യം store_char (അക്ഷരം c);
സ്ഥിതം ശൂന്യം pad_down (unsigned പൂർണ്ണസംഖ്യ വരികൾ);
സ്ഥിതം ശൂന്യം read_rest_of_വരി (COLUMN *p);
സ്ഥിതം ശൂന്യം skip_read (COLUMN *p, പൂർണ്ണസംഖ്യ column_സംഖ്യ);
സ്ഥിതം ശൂന്യം print_char (അക്ഷരം c);
സ്ഥിതം ശൂന്യം cleanup (ശൂന്യം);
സ്ഥിതം ശൂന്യം print_sep_string (ശൂന്യം);
സ്ഥിതം ശൂന്യം separator_string (അക്ഷരം സ്ഥിരം *optarg_S);

/* All of the columns to print.  */
സ്ഥിതം COLUMN *column_vector;

/* When printing a single file in multiple downward columns,
   we store the leftmost columns contiguously in buff.
   To print a line from buff, get the index of the first character
   from line_vector[i], and print up to line_vector[i + 1]. */
സ്ഥിതം അക്ഷരം *buff;

/* Index of the position in buff where the next character
   will be stored. */
സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ buff_current;

/* The number of characters in buff.
   Used for allocation of buff and to detect overflow of buff. */
സ്ഥിതം idx_t buff_allocated;

/* Array of indices into buff.
   Each entry is an index of the first character of a line.
   This is used when storing lines to facilitate shuffling when
   we do column balancing on the last page. */
സ്ഥിതം പൂർണ്ണസംഖ്യ *വരി_vector;

/* Array of horizontal positions.
   For each line in line_vector, end_vector[line] is the horizontal
   position we are in after printing that line.  We keep track of this
   so that we know how much we need to pad to prepare for the next
   column. */
സ്ഥിതം പൂർണ്ണസംഖ്യ *end_vector;

/* (-m) True means we're printing multiple files in parallel. */
സ്ഥിതം bool parallel_files = false;

/* (-m) True means a line starts with some empty columns (some files
   already CLOSED or ON_HOLD) which we have to align. */
സ്ഥിതം bool align_empty_cols;

/* (-m) True means we have not yet found any printable column in a line.
   align_empty_cols = true  has to be maintained. */
സ്ഥിതം bool empty_വരി;

/* (-m) False means printable column output precedes a form feed found.
   Column alignment is done only once. No additional action with that form
   feed.
   True means we found only a form feed in a column. Maybe we have to do
   some column alignment with that form feed. */
സ്ഥിതം bool FF_only;

/* (-[0-9]+) True means we're given an option explicitly specifying
   number of columns.  Used to detect when this option is used with -m
   and when translating old options to new/long options. */
സ്ഥിതം bool explicit_columns = false;

/* (-t|-T) False means we aren't printing headers and footers. */
സ്ഥിതം bool extremities = true;

/* (-t) True means we retain all FF set by hand in input files.
   False is set with -T option. */
സ്ഥിതം bool keep_FF = false;
സ്ഥിതം bool print_a_FF = false;

/* True means we need to print a header as soon as we know we've got input
   to print after it. */
സ്ഥിതം bool print_a_header;

/* (-f) True means use formfeeds instead of newlines to separate pages. */
സ്ഥിതം bool use_form_feed = false;

/* True means we have read the standard input. */
സ്ഥിതം bool have_read_stdin = false;

/* True means the -a flag has been given. */
സ്ഥിതം bool print_across_flag = false;

/* True means we're printing one file in multiple (>1) downward columns. */
സ്ഥിതം bool storing_columns = true;

/* (-b) True means balance columns on the last page as Sys V does. */
/* That's no longer an independent option. With storing_columns = true
   balance_columns = true is used too (s. function init_parameters).
   We get a consistent formulation with "FF set by hand" in input files. */
സ്ഥിതം bool balance_columns = false;

/* (-l) Number of lines on a page, including header and footer lines. */
സ്ഥിതം പൂർണ്ണസംഖ്യ വരികൾ_per_page = 66;

/* Number of lines in the header and footer can be reset to 0 using
   the -t flag. */
എണ്ണൽ { വരികൾ_per_header = 5 };
സ്ഥിതം പൂർണ്ണസംഖ്യ വരികൾ_per_body;
എണ്ണൽ { വരികൾ_per_footer = 5 };

/* (-w|-W) Width in characters of the page.  Does not include the width of
   the margin. */
സ്ഥിതം പൂർണ്ണസംഖ്യ chars_per_വരി = 72;

/* (-w|W) True means we truncate lines longer than chars_per_column. */
സ്ഥിതം bool truncate_വരികൾ = false;

/* (-J) True means we join lines without any line truncation. -J
   dominates -w option. */
സ്ഥിതം bool join_വരികൾ = false;

/* Number of characters in a column.  Based on col_sep_length and
   page width. */
സ്ഥിതം പൂർണ്ണസംഖ്യ chars_per_column;

/* (-e) True means convert tabs to spaces on input. */
സ്ഥിതം bool untabify_നിവേശം = false;

/* (-e) The input tab character. */
സ്ഥിതം അക്ഷരം നിവേശം_tab_char = '\t';

/* (-e) Tabstops are at chars_per_tab, 2*chars_per_tab, 3*chars_per_tab, ...
   where the leftmost column is 1. */
സ്ഥിതം പൂർണ്ണസംഖ്യ chars_per_നിവേശം_tab = 8;

/* (-i) True means convert spaces to tabs on output. */
സ്ഥിതം bool tabify_output = false;

/* (-i) The output tab character. */
സ്ഥിതം അക്ഷരം output_tab_char = '\t';

/* (-i) The width of the output tab. */
സ്ഥിതം പൂർണ്ണസംഖ്യ chars_per_output_tab = 8;

/* Keeps track of pending white space.  When we hit a nonspace
   character after some whitespace, we print whitespace, tabbing
   if necessary to get to output_position + spaces_not_printed. */
സ്ഥിതം പൂർണ്ണസംഖ്യ spaces_not_printed;

/* (-o) Number of spaces in the left margin (tabs used when possible). */
സ്ഥിതം പൂർണ്ണസംഖ്യ chars_per_margin = 0;

/* Position where the next character will fall.
   Leftmost position is 0 + chars_per_margin.
   Rightmost position is chars_per_margin + chars_per_line - 1.
   This is important for converting spaces to tabs on output. */
സ്ഥിതം പൂർണ്ണസംഖ്യ output_position;

/* Horizontal position relative to the current file.
   (output_position depends on where we are on the page;
   input_position depends on where we are in the file.)
   Important for converting tabs to spaces on input. */
സ്ഥിതം പൂർണ്ണസംഖ്യ നിവേശം_position;

/* True if there were any failed opens so we can exit with nonzero
   status.  */
സ്ഥിതം bool failed_opens = false;

/* The number of spaces taken up if we print a tab character with width
   c_ from position h_. */
#define TAB_WIDTH(c_, h_) ((c_) - ((h_) % (c_)))

/* The horizontal position we'll be at after printing a tab character
   of width c_ from the position h_. */
#define POS_AFTER_TAB(c_, h_) ((h_) + TAB_WIDTH (c_, h_))

/* (-NNN) Number of columns of text to print. */
സ്ഥിതം പൂർണ്ണസംഖ്യ columns = 1;

/* (+NNN:MMM) Page numbers on which to begin and stop printing.
   first_page_number = 0  will be used to check input only. */
സ്ഥിതം uintmax_t first_page_സംഖ്യ = 0;
സ്ഥിതം uintmax_t last_page_സംഖ്യ = UINTMAX_MAX;

/* Number of files open (not closed, not on hold). */
സ്ഥിതം പൂർണ്ണസംഖ്യ files_ready_to_read = 0;

/* Current page number.  Displayed in header. */
സ്ഥിതം uintmax_t page_സംഖ്യ;

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
സ്ഥിതം പൂർണ്ണസംഖ്യ വരി_സംഖ്യ;

/* (-n) True means lines should be preceded by numbers. */
സ്ഥിതം bool numbered_വരികൾ = false;

/* (-n) Character which follows each line number. */
സ്ഥിതം അക്ഷരം സംഖ്യ_separator = '\t';

/* (-n) line counting starts with 1st line of input file (not with 1st
   line of 1st page printed). */
സ്ഥിതം പൂർണ്ണസംഖ്യ വരി_എണ്ണം = 1;

/* (-n) True means counting of skipped lines starts with 1st line of
   input file. False means -N option is used in addition, counting of
   skipped lines not required. */
സ്ഥിതം bool skip_എണ്ണം = true;

/* (-N) Counting starts with start_line_number = NUMBER at 1st line of
   first page printed, usually not 1st page of input file. */
സ്ഥിതം പൂർണ്ണസംഖ്യ start_വരി_അക്കം = 1;

/* (-n) Width in characters of a line number. */
സ്ഥിതം പൂർണ്ണസംഖ്യ chars_per_സംഖ്യ = 5;

/* Used when widening the first column to accommodate numbers -- only
   needed when printing files in parallel.  Includes width of both the
   number and the number_separator. */
സ്ഥിതം പൂർണ്ണസംഖ്യ സംഖ്യ_width;

/* Buffer sprintf uses to format a line number. */
സ്ഥിതം അക്ഷരം *സംഖ്യ_buff;

/* (-v) True means nonprintable characters are printed as escape sequences.
   control-g becomes \007. */
സ്ഥിതം bool use_esc_sequence = false;

/* (-c) True means nonprintable characters are printed as control prefixes.
   control-g becomes ^G. */
സ്ഥിതം bool use_cntrl_prefix = false;

/* (-d) True means output is double spaced. */
സ്ഥിതം bool double_space = false;

/* Number of files opened initially in init_files.  Should be 1
   unless we're printing multiple files in parallel. */
സ്ഥിതം പൂർണ്ണസംഖ്യ total_files = 0;

/* (-r) True means don't complain if we can't open a file. */
സ്ഥിതം bool ignore_failed_opens = false;

/* (-S) True means we separate columns with a specified string.
   -S option does not affect line truncation nor column alignment. */
സ്ഥിതം bool use_col_separator = false;

/* String used to separate columns if the -S option has been specified.
   Default without -S but together with one of the column options
   -a|COLUMN|-m is a 'space' and with the -J option a 'tab'. */
സ്ഥിതം അക്ഷരം സ്ഥിരം *col_sep_string = "";
സ്ഥിതം പൂർണ്ണസംഖ്യ col_sep_length = 0;
സ്ഥിതം അക്ഷരം *column_separator = (അക്ഷരം *) " ";
സ്ഥിതം അക്ഷരം *വരി_separator = (അക്ഷരം *) "\t";

/* Number of separator characters waiting to be printed as soon as we
   know that we have any input remaining to be printed. */
സ്ഥിതം പൂർണ്ണസംഖ്യ separators_not_printed;

/* Position we need to pad to, as soon as we know that we have input
   remaining to be printed. */
സ്ഥിതം പൂർണ്ണസംഖ്യ padding_not_printed;

/* True means we should pad the end of the page.  Remains false until we
   know we have a page to print. */
സ്ഥിതം bool pad_vertically;

/* (-h) String of characters used in place of the filename in the header. */
സ്ഥിതം അക്ഷരം *custom_header;

/* (-D) Date format for the header.  */
സ്ഥിതം അക്ഷരം സ്ഥിരം *date_format;

/* The local time zone rules, as per the TZ environment variable.  */
സ്ഥിതം timezone_t localtz;

/* Date and file name for the header.  */
സ്ഥിതം അക്ഷരം *date_text;
സ്ഥിതം അക്ഷരം സ്ഥിരം *file_text;

/* Output columns available, not counting the date and file name.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ header_width_available;

സ്ഥിതം അക്ഷരം *clump_buff;

/* True means we read the line no. lines_per_body in skip_read
   called by skip_to_page. That variable controls the coincidence of a
   "FF set by hand" and "full_page_printed", see above the definition of
   structure COLUMN. */
സ്ഥിതം bool last_വരി = false;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
എണ്ണൽ
{
  COLUMNS_OPTION = CHAR_MAX + 1,
  PAGES_OPTION
};

സ്ഥിതം അക്ഷരം സ്ഥിരം short_options[] =
  "-0123456789D:FJN:S::TW:abcde::fh:i::l:mn::o:rs::tvw:";

സ്ഥിതം ഘടന option സ്ഥിരം long_options[] =
{
  {"pages", required_argument, ശൂന്യ, PAGES_OPTION},
  {"columns", required_argument, ശൂന്യ, COLUMNS_OPTION},
  {"across", no_argument, ശൂന്യ, 'a'},
  {"show-control-chars", no_argument, ശൂന്യ, 'c'},
  {"double-space", no_argument, ശൂന്യ, 'd'},
  {"date-format", required_argument, ശൂന്യ, 'D'},
  {"expand-tabs", optional_argument, ശൂന്യ, 'e'},
  {"form-feed", no_argument, ശൂന്യ, 'f'},
  {"header", required_argument, ശൂന്യ, 'h'},
  {"output-tabs", optional_argument, ശൂന്യ, 'i'},
  {"join-lines", no_argument, ശൂന്യ, 'J'},
  {"length", required_argument, ശൂന്യ, 'l'},
  {"merge", no_argument, ശൂന്യ, 'm'},
  {"number-lines", optional_argument, ശൂന്യ, 'n'},
  {"first-line-number", required_argument, ശൂന്യ, 'N'},
  {"indent", required_argument, ശൂന്യ, 'o'},
  {"no-file-warnings", no_argument, ശൂന്യ, 'r'},
  {"separator", optional_argument, ശൂന്യ, 's'},
  {"sep-string", optional_argument, ശൂന്യ, 'S'},
  {"omit-header", no_argument, ശൂന്യ, 't'},
  {"omit-pagination", no_argument, ശൂന്യ, 'T'},
  {"show-nonprinting", no_argument, ശൂന്യ, 'v'},
  {"width", required_argument, ശൂന്യ, 'w'},
  {"page-width", required_argument, ശൂന്യ, 'W'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ശൂന്യ, 0, ശൂന്യ, 0}
};

സ്ഥിതം _Noreturn ശൂന്യം
integer_overflow (ശൂന്യം)
{
  error (EXIT_FAILURE, 0, _("integer overflow"));
}

/* Return the number of columns that have either an open file or
   stored lines. */

ATTRIBUTE_PURE
സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ
cols_ready_to_print (ശൂന്യം)
{
  COLUMN *q;
  unsigned പൂർണ്ണസംഖ്യ i;
  unsigned പൂർണ്ണസംഖ്യ n;

  n = 0;
  വേണ്ടി (q = column_vector, i = 0; i < columns; ++q, ++i)
    എങ്കിൽ (q->status == OPEN
        || q->status == FF_FOUND	/* With -b: To print a header only */
        || (storing_columns && q->വരികൾ_stored > 0 && q->വരികൾ_to_print > 0))
      ++n;
  തിരികെ n;
}

/* Estimate first_ / last_page_number
   using option +FIRST_PAGE:LAST_PAGE */

സ്ഥിതം bool
first_last_page (പൂർണ്ണസംഖ്യ oi, അക്ഷരം c, അക്ഷരം സ്ഥിരം *pages)
{
  അക്ഷരം *p;
  uintmax_t first;
  uintmax_t last = UINTMAX_MAX;
  strtol_error err = xstrtoumax (pages, &p, 10, &first, "");
  എങ്കിൽ (err != LONGINT_OK && err != LONGINT_INVALID_SUFFIX_CHAR)
    xstrtol_fatal (err, oi, c, long_options, pages);

  എങ്കിൽ (p == pages || !first)
    തിരികെ false;

  എങ്കിൽ (*p == ':')
    {
      അക്ഷരം സ്ഥിരം *p1 = p + 1;
      err = xstrtoumax (p1, &p, 10, &last, "");
      എങ്കിൽ (err != LONGINT_OK)
        xstrtol_fatal (err, oi, c, long_options, pages);
      എങ്കിൽ (p1 == p || last < first)
        തിരികെ false;
    }

  എങ്കിൽ (*p)
    തിരികെ false;

  first_page_സംഖ്യ = first;
  last_page_സംഖ്യ = last;
  തിരികെ true;
}

/* Parse column count string S, and if it's valid (1 or larger and
   within range of the type of 'columns') set the global variables
   columns and explicit_columns.  Otherwise, exit with a diagnostic.  */

സ്ഥിതം ശൂന്യം
parse_column_എണ്ണം (അക്ഷരം സ്ഥിരം *s)
{
  columns = getoptnum (s, 1, _("invalid number of columns"));
  explicit_columns = true;
}

/* Estimate length of col_sep_string with option -S.  */

സ്ഥിതം ശൂന്യം
separator_string (അക്ഷരം സ്ഥിരം *optarg_S)
{
  size_t len = strlen (optarg_S);
  എങ്കിൽ (INT_MAX < len)
    integer_overflow ();
  col_sep_length = len;
  col_sep_string = optarg_S;
}

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  unsigned പൂർണ്ണസംഖ്യ n_files;
  bool old_options = false;
  bool old_w = false;
  bool old_s = false;
  അക്ഷരം **file_names;

  /* Accumulate the digits of old-style options like -99.  */
  അക്ഷരം *column_എണ്ണം_string = ശൂന്യ;
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
                ? xnmalloc (argc - 1, വലുപ്പം (അക്ഷരം *))
                : ശൂന്യ);

  ആകുമ്പോൾ (true)
    {
      പൂർണ്ണസംഖ്യ oi = -1;
      പൂർണ്ണസംഖ്യ c = getopt_long (argc, argv, short_options, long_options, &oi);
      എങ്കിൽ (c == -1)
        നിർത്തുക;

      എങ്കിൽ (c_isdigit (c))
        {
          /* Accumulate column-count digits specified via old-style options. */
          എങ്കിൽ (n_digits + 1 >= n_alloc)
            column_എണ്ണം_string = xpalloc (column_എണ്ണം_string, &n_alloc, 2, -1,
                                           വലുപ്പം *column_എണ്ണം_string);
          column_എണ്ണം_string[n_digits++] = c;
          column_എണ്ണം_string[n_digits] = '\0';
          തുടരുക;
        }

      n_digits = 0;

      തിരഞ്ഞെടുപ്പ് (c)
        {
        അവസ്ഥ 1:			/* Non-option argument. */
          /* long option --page dominates old '+FIRST_PAGE ...'.  */
          എങ്കിൽ (! (first_page_സംഖ്യ == 0
                 && *optarg == '+' && first_last_page (-2, '+', optarg + 1)))
            file_names[n_files++] = optarg;
          നിർത്തുക;

        അവസ്ഥ PAGES_OPTION:	/* --pages=FIRST_PAGE[:LAST_PAGE] */
          {			/* dominates old opt +... */
            എങ്കിൽ (! optarg)
              error (EXIT_FAILURE, 0,
                     _("'--pages=FIRST_PAGE[:LAST_PAGE]' missing argument"));
            അല്ലെങ്കിൽ എങ്കിൽ (! first_last_page (oi, 0, optarg))
              error (EXIT_FAILURE, 0, _("invalid page range %s"),
                     quote (optarg));
            നിർത്തുക;
          }

        അവസ്ഥ COLUMNS_OPTION:	/* --columns=COLS */
          {
            parse_column_എണ്ണം (optarg);

            /* If there was a prior column count specified via the
               short-named option syntax, e.g., -9, ensure that this
               long-name-specified value overrides it.  */
            സ്വതന്ത്രം (column_എണ്ണം_string);
            column_എണ്ണം_string = ശൂന്യ;
            n_alloc = 0;
            നിർത്തുക;
          }

        അവസ്ഥ 'a':
          print_across_flag = true;
          storing_columns = false;
          നിർത്തുക;
        അവസ്ഥ 'b':
          balance_columns = true;
          നിർത്തുക;
        അവസ്ഥ 'c':
          use_cntrl_prefix = true;
          നിർത്തുക;
        അവസ്ഥ 'd':
          double_space = true;
          നിർത്തുക;
        അവസ്ഥ 'D':
          date_format = optarg;
          നിർത്തുക;
        അവസ്ഥ 'e':
          എങ്കിൽ (optarg)
            getoptarg (optarg, 'e', &നിവേശം_tab_char,
                       &chars_per_നിവേശം_tab);
          /* Could check tab width > 0. */
          untabify_നിവേശം = true;
          നിർത്തുക;
        അവസ്ഥ 'f':
        അവസ്ഥ 'F':
          use_form_feed = true;
          നിർത്തുക;
        അവസ്ഥ 'h':
          custom_header = optarg;
          നിർത്തുക;
        അവസ്ഥ 'i':
          എങ്കിൽ (optarg)
            getoptarg (optarg, 'i', &output_tab_char,
                       &chars_per_output_tab);
          /* Could check tab width > 0. */
          tabify_output = true;
          നിർത്തുക;
        അവസ്ഥ 'J':
          join_വരികൾ = true;
          നിർത്തുക;
        അവസ്ഥ 'l':
          വരികൾ_per_page
            = getoptnum (optarg, 1,
                         _("'-l PAGE_LENGTH' invalid number of lines"));
          നിർത്തുക;
        അവസ്ഥ 'm':
          parallel_files = true;
          storing_columns = false;
          നിർത്തുക;
        അവസ്ഥ 'n':
          numbered_വരികൾ = true;
          എങ്കിൽ (optarg)
            getoptarg (optarg, 'n', &സംഖ്യ_separator,
                       &chars_per_സംഖ്യ);
          നിർത്തുക;
        അവസ്ഥ 'N':
          skip_എണ്ണം = false;
          start_വരി_അക്കം
            = getoptnum (optarg, INT_MIN,
                         _("'-N NUMBER' invalid starting line number"));
          നിർത്തുക;
        അവസ്ഥ 'o':
          chars_per_margin = getoptnum (optarg, 0,
                                        _("'-o MARGIN' invalid line offset"));
          നിർത്തുക;
        അവസ്ഥ 'r':
          ignore_failed_opens = true;
          നിർത്തുക;
        അവസ്ഥ 's':
          old_options = true;
          old_s = true;
          എങ്കിൽ (!use_col_separator && optarg)
            separator_string (optarg);
          നിർത്തുക;
        അവസ്ഥ 'S':
          old_s = false;
          /* Reset an additional input of -s, -S dominates -s */
          col_sep_string = "";
          col_sep_length = 0;
          use_col_separator = true;
          എങ്കിൽ (optarg)
            separator_string (optarg);
          നിർത്തുക;
        അവസ്ഥ 't':
          extremities = false;
          keep_FF = true;
          നിർത്തുക;
        അവസ്ഥ 'T':
          extremities = false;
          keep_FF = false;
          നിർത്തുക;
        അവസ്ഥ 'v':
          use_esc_sequence = true;
          നിർത്തുക;
        അവസ്ഥ 'w':
          old_options = true;
          old_w = true;
          {
            പൂർണ്ണസംഖ്യ tmp_cpl
              = getoptnum (optarg, 1,
                           _("'-w PAGE_WIDTH' invalid number of characters"));
            എങ്കിൽ (! truncate_വരികൾ)
              chars_per_വരി = tmp_cpl;
          }
          നിർത്തുക;
        അവസ്ഥ 'W':
          old_w = false;			/* dominates -w */
          truncate_വരികൾ = true;
          chars_per_വരി
            = getoptnum (optarg, 1,
                         _("'-W PAGE_WIDTH' invalid number of characters"));
          നിർത്തുക;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        സ്ഥിരസ്ഥിതി:
          usage (EXIT_FAILURE);
          നിർത്തുക;
        }
    }

  എങ്കിൽ (column_എണ്ണം_string)
    {
      parse_column_എണ്ണം (column_എണ്ണം_string);
      സ്വതന്ത്രം (column_എണ്ണം_string);
    }

  എങ്കിൽ (! date_format)
    date_format = (getenv ("POSIXLY_CORRECT") && !hard_locale (LC_TIME)
                   ? "%b %e %H:%M %Y"
                   : "%Y-%m-%d %H:%M");

  localtz = tzalloc (getenv ("TZ"));

  /* Now we can set a reasonable initial value: */
  എങ്കിൽ (first_page_സംഖ്യ == 0)
    first_page_സംഖ്യ = 1;

  എങ്കിൽ (parallel_files && explicit_columns)
    error (EXIT_FAILURE, 0,
           _("cannot specify number of columns when printing in parallel"));

  എങ്കിൽ (parallel_files && print_across_flag)
    error (EXIT_FAILURE, 0,
           _("cannot specify both printing across and printing in parallel"));

/* Translate some old short options to new/long options.
   To meet downward compatibility with other UNIX pr utilities
   and some POSIX specifications. */

  എങ്കിൽ (old_options)
    {
      എങ്കിൽ (old_w)
        {
          എങ്കിൽ (parallel_files || explicit_columns)
            {
              /* activate -W */
              truncate_വരികൾ = true;
              എങ്കിൽ (old_s)
                /* adapt HP-UX and SunOS: -s = no separator;
                   activate -S */
                use_col_separator = true;
            }
          അല്ലെങ്കിൽ
            /* old -w sets width with columns only
               activate -J */
            join_വരികൾ = true;
        }
      അല്ലെങ്കിൽ എങ്കിൽ (!use_col_separator)
        {
          /* No -S option read */
          എങ്കിൽ (old_s && (parallel_files || explicit_columns))
            {
              എങ്കിൽ (!truncate_വരികൾ)
                {
                  /* old -s (without -w and -W) annuls column alignment,
                  uses fields, activate -J */
                  join_വരികൾ = true;
                  എങ്കിൽ (col_sep_length > 0)
                    /* activate -S */
                    use_col_separator = true;
                }
              അല്ലെങ്കിൽ
                /* with -W */
                /* adapt HP-UX and SunOS: -s = no separator;
                   activate -S */
                use_col_separator = true;
            }
        }
    }

  വേണ്ടി (; optind < argc; optind++)
    {
      file_names[n_files++] = argv[optind];
    }

  എങ്കിൽ (n_files == 0)
    {
      /* No file arguments specified;  read from standard input.  */
      print_files (0, ശൂന്യ);
    }
  അല്ലെങ്കിൽ
    {
      എങ്കിൽ (parallel_files)
        print_files (n_files, file_names);
      അല്ലെങ്കിൽ
        {
          വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < n_files; i++)
            print_files (1, &file_names[i]);
        }
    }

  cleanup ();

  എങ്കിൽ (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, _("standard input"));
  main_exit (failed_opens ? EXIT_FAILURE : EXIT_SUCCESS);
}

/* Parse numeric arguments, ensuring MIN <= number <= INT_MAX.  */

സ്ഥിതം പൂർണ്ണസംഖ്യ
getoptnum (അക്ഷരം സ്ഥിരം *n_str, പൂർണ്ണസംഖ്യ min, അക്ഷരം സ്ഥിരം *err)
{
  തിരികെ xnumtoimax (n_str, 10, min, INT_MAX, "", err, 0,
                     min <= 0 ? 0 : XTOINT_MIN_RANGE);
}

/* Parse options of the form -scNNN.

   Example: -nck, where 'n' is the option, c is the optional number
   separator, and k is the optional width of the field used when printing
   a number. */

സ്ഥിതം ശൂന്യം
getoptarg (അക്ഷരം *arg, അക്ഷരം switch_char, അക്ഷരം *character, പൂർണ്ണസംഖ്യ *സംഖ്യ)
{
  അക്ഷരം സ്ഥിരം *അക്കം = arg;

  എങ്കിൽ (!*അക്കം)
    {
      error (0, 0, _("'-%c': Invalid argument: %s"), switch_char, quote (അക്കം));
      usage (EXIT_FAILURE);
    }

  എങ്കിൽ (!c_isdigit (*അക്കം))
    *character = *അക്കം++;
  എങ്കിൽ (*അക്കം)
    {
      long പൂർണ്ണസംഖ്യ tmp_long;
      strtol_error e = xstrtol (അക്കം, ശൂന്യ, 10, &tmp_long, "");
      എങ്കിൽ (e == LONGINT_OK)
        {
          എങ്കിൽ (tmp_long <= 0)
            e = LONGINT_INVALID;
          അല്ലെങ്കിൽ എങ്കിൽ (INT_MAX < tmp_long)
            e = LONGINT_OVERFLOW;
        }
      എങ്കിൽ (e != LONGINT_OK)
        {
          error (0, e & LONGINT_OVERFLOW ? EOVERFLOW : 0,
             _("'-%c' extra characters or invalid number in the argument: %s"),
                 switch_char, quote (arg));
          usage (EXIT_FAILURE);
        }
      *സംഖ്യ = tmp_long;
    }
}

/* Set parameters related to formatting. */

സ്ഥിതം ശൂന്യം
init_parameters (പൂർണ്ണസംഖ്യ സംഖ്യ_of_files)
{
  പൂർണ്ണസംഖ്യ chars_used_by_സംഖ്യ = 0;

  വരികൾ_per_body = വരികൾ_per_page - വരികൾ_per_header - വരികൾ_per_footer;
  എങ്കിൽ (വരികൾ_per_body <= 0)
    {
      extremities = false;
      keep_FF = true;
    }
  എങ്കിൽ (! extremities)
    വരികൾ_per_body = വരികൾ_per_page;

  എങ്കിൽ (double_space)
    വരികൾ_per_body = MAX (1, വരികൾ_per_body / 2);

  /* If input is stdin, cannot print parallel files.  BSD dumps core
     on this. */
  എങ്കിൽ (സംഖ്യ_of_files == 0)
    parallel_files = false;

  എങ്കിൽ (parallel_files)
    columns = സംഖ്യ_of_files;

  /* One file, multi columns down: -b option is set to get a consistent
     formulation with "FF set by hand" in input files. */
  എങ്കിൽ (storing_columns)
    balance_columns = true;

  /* Tabification is assumed for multiple columns. */
  എങ്കിൽ (columns > 1)
    {
      എങ്കിൽ (!use_col_separator)
        {
          /* Use default separator */
          എങ്കിൽ (join_വരികൾ)
            col_sep_string = വരി_separator;
          അല്ലെങ്കിൽ
            col_sep_string = column_separator;

          col_sep_length = 1;
          use_col_separator = true;
        }
      /* It's rather pointless to define a TAB separator with column
         alignment */
      അല്ലെങ്കിൽ എങ്കിൽ (!join_വരികൾ && col_sep_length == 1 && *col_sep_string == '\t')
        col_sep_string = column_separator;

      truncate_വരികൾ = true;
      എങ്കിൽ (! (col_sep_length == 1 && *col_sep_string == '\t'))
        untabify_നിവേശം = true;
      tabify_output = true;
    }
  അല്ലെങ്കിൽ
    storing_columns = false;

  /* -J dominates -w in any case */
  എങ്കിൽ (join_വരികൾ)
    truncate_വരികൾ = false;

  എങ്കിൽ (numbered_വരികൾ)
    {
      പൂർണ്ണസംഖ്യ chars_per_default_tab = 8;

      വരി_എണ്ണം = start_വരി_അക്കം;

      /* To allow input tab-expansion (-e sensitive) use:
         if (number_separator == input_tab_char)
           number_width = chars_per_number
             + TAB_WIDTH (chars_per_input_tab, chars_per_number);   */

      /* Estimate chars_per_text without any margin and keep it constant. */
      എങ്കിൽ (സംഖ്യ_separator == '\t')
        സംഖ്യ_width = (chars_per_സംഖ്യ
                        + TAB_WIDTH (chars_per_default_tab, chars_per_സംഖ്യ));
      അല്ലെങ്കിൽ
        സംഖ്യ_width = chars_per_സംഖ്യ + 1;

      /* The number is part of the column width unless we are
         printing files in parallel. */
      എങ്കിൽ (parallel_files)
        chars_used_by_സംഖ്യ = സംഖ്യ_width;
    }

  പൂർണ്ണസംഖ്യ sep_chars, useful_chars;
  എങ്കിൽ (ckd_mul (&sep_chars, columns - 1, col_sep_length))
    sep_chars = INT_MAX;
  എങ്കിൽ (ckd_sub (&useful_chars, chars_per_വരി - chars_used_by_സംഖ്യ,
               sep_chars))
    useful_chars = 0;
  chars_per_column = useful_chars / columns;

  എങ്കിൽ (chars_per_column < 1)
    error (EXIT_FAILURE, 0, _("page width too narrow"));

  എങ്കിൽ (numbered_വരികൾ)
    {
      സ്വതന്ത്രം (സംഖ്യ_buff);
      സംഖ്യ_buff = xmalloc (MAX (chars_per_സംഖ്യ,
                                  INT_STRLEN_BOUND (വരി_സംഖ്യ)) + 1);
    }

  /* Pick the maximum between the tab width and the width of an
     escape sequence.
     The width of an escape sequence (4) isn't the lower limit any longer.
     We've to use 8 as the lower limit, if we use chars_per_default_tab = 8
     to expand a tab which is not an input_tab-char. */
  സ്വതന്ത്രം (clump_buff);
  clump_buff = xmalloc (MAX (8, chars_per_നിവേശം_tab));
}

/* Open the necessary files,
   maintaining a COLUMN structure for each column.

   With multiple files, each column p has a different p->fp.
   With single files, each column p has the same p->fp.
   Return false if (number_of_files > 0) and no files can be opened,
   true otherwise.

   With each column/file p, p->full_page_printed is initialized,
   see also open_file.  */

സ്ഥിതം bool
init_fps (പൂർണ്ണസംഖ്യ സംഖ്യ_of_files, അക്ഷരം **av)
{
  COLUMN *p;

  total_files = 0;

  സ്വതന്ത്രം (column_vector);
  column_vector = xnmalloc (columns, വലുപ്പം (COLUMN));

  എങ്കിൽ (parallel_files)
    {
      പൂർണ്ണസംഖ്യ files_ഇടത് = സംഖ്യ_of_files;
      വേണ്ടി (p = column_vector; files_ഇടത്--; ++p, ++av)
        {
          എങ്കിൽ (! open_file (*av, p))
            {
              --p;
              --columns;
            }
        }
      എങ്കിൽ (columns == 0)
        തിരികെ false;
      init_header ("", -1);
    }
  അല്ലെങ്കിൽ
    {
      p = column_vector;
      എങ്കിൽ (സംഖ്യ_of_files > 0)
        {
          എങ്കിൽ (! open_file (*av, p))
            തിരികെ false;
          init_header (*av, fileno (p->fp));
          p->വരികൾ_stored = 0;
        }
      അല്ലെങ്കിൽ
        {
          p->name = _("standard input");
          p->fp = stdin;
          have_read_stdin = true;
          p->status = OPEN;
          p->full_page_printed = false;
          ++total_files;
          init_header ("", -1);
          p->വരികൾ_stored = 0;
        }

      അക്ഷരം സ്ഥിരം *firstname = p->name;
      FILE *firstfp = p->fp;
      പൂർണ്ണസംഖ്യ i;
      വേണ്ടി (i = columns - 1, ++p; i; --i, ++p)
        {
          p->name = firstname;
          p->fp = firstfp;
          p->status = OPEN;
          p->full_page_printed = false;
          p->വരികൾ_stored = 0;
        }
    }
  files_ready_to_read = total_files;
  തിരികെ true;
}

/* Determine print_func and char_func, the functions
   used by each column for printing and/or storing.

   Determine the horizontal position desired when we begin
   printing a column (p->start_position). */

സ്ഥിതം ശൂന്യം
init_funcs (ശൂന്യം)
{
  പൂർണ്ണസംഖ്യ i, h, h_next;
  COLUMN *p;

  h = chars_per_margin;

  എങ്കിൽ (!truncate_വരികൾ)
    h_next = ANYWHERE;
  അല്ലെങ്കിൽ
    {
      /* When numbering lines of parallel files, we enlarge the
         first column to accommodate the number.  Looks better than
         the Sys V approach. */
      എങ്കിൽ (parallel_files && numbered_വരികൾ)
        h_next = h + chars_per_column + സംഖ്യ_width;
      അല്ലെങ്കിൽ
        h_next = h + chars_per_column;
    }

  /* Enlarge p->start_position of first column to use the same form of
     padding_not_printed with all columns. */
  h = h + col_sep_length;

  /* This loop takes care of all but the rightmost column. */

  വേണ്ടി (p = column_vector, i = 1; i < columns; ++p, ++i)
    {
      എങ്കിൽ (storing_columns)	/* One file, multi columns down. */
        {
          p->char_func = store_char;
          p->print_func = print_stored;
        }
      അല്ലെങ്കിൽ
        /* One file, multi columns across; or parallel files.  */
        {
          p->char_func = print_char;
          p->print_func = read_വരി;
        }

      /* Number only the first column when printing files in
         parallel. */
      p->numbered = numbered_വരികൾ && (!parallel_files || i == 1);
      p->start_position = h;

      /* If we don't truncate lines, all start_positions are
         ANYWHERE, except the first column's start_position when
         using a margin. */

      എങ്കിൽ (!truncate_വരികൾ)
        {
          h = ANYWHERE;
          h_next = ANYWHERE;
        }
      അല്ലെങ്കിൽ
        {
          h = h_next + col_sep_length;
          h_next = h + chars_per_column;
        }
    }

  /* The rightmost column.

     Doesn't need to be stored unless we intend to balance
     columns on the last page. */
  എങ്കിൽ (storing_columns && balance_columns)
    {
      p->char_func = store_char;
      p->print_func = print_stored;
    }
  അല്ലെങ്കിൽ
    {
      p->char_func = print_char;
      p->print_func = read_വരി;
    }

  p->numbered = numbered_വരികൾ && (!parallel_files || i == 1);
  p->start_position = h;
}

/* Open a file.  Return true if successful.

   With each file p, p->full_page_printed is initialized,
   see also init_fps. */

സ്ഥിതം bool
open_file (അക്ഷരം *name, COLUMN *p)
{
  എങ്കിൽ (streq (name, "-"))
    {
      p->name = _("standard input");
      p->fp = stdin;
      have_read_stdin = true;
    }
  അല്ലെങ്കിൽ
    {
      p->name = name;
      p->fp = fopen (name, "r");
    }
  എങ്കിൽ (p->fp == ശൂന്യ)
    {
      failed_opens = true;
      എങ്കിൽ (!ignore_failed_opens)
        error (0, errno, "%s", quotef (name));
      തിരികെ false;
    }
  fadvise (p->fp, FADVISE_SEQUENTIAL);
  p->status = OPEN;
  p->full_page_printed = false;
  ++total_files;
  തിരികെ true;
}

/* Close the file in P.

   If we aren't dealing with multiple files in parallel, we change
   the status of all columns in the column list to reflect the close. */

സ്ഥിതം ശൂന്യം
close_file (COLUMN *p)
{
  COLUMN *q;
  പൂർണ്ണസംഖ്യ i;

  എങ്കിൽ (p->status == CLOSED)
    തിരികെ;

  പൂർണ്ണസംഖ്യ err = errno;
  എങ്കിൽ (!ferror (p->fp))
    err = 0;
  എങ്കിൽ (fileno (p->fp) == STDIN_FILENO)
    clearerr (p->fp);
  അല്ലെങ്കിൽ എങ്കിൽ (fclose (p->fp) != 0 && !err)
    err = errno;
  എങ്കിൽ (err)
    error (EXIT_FAILURE, err, "%s", quotef (p->name));

  എങ്കിൽ (!parallel_files)
    {
      വേണ്ടി (q = column_vector, i = columns; i; ++q, --i)
        {
          q->status = CLOSED;
          എങ്കിൽ (q->വരികൾ_stored == 0)
            {
              q->വരികൾ_to_print = 0;
            }
        }
    }
  അല്ലെങ്കിൽ
    {
      p->status = CLOSED;
      p->വരികൾ_to_print = 0;
    }

  --files_ready_to_read;
}

/* Put a file on hold until we start a new page,
   since we've hit a form feed.

   If we aren't dealing with parallel files, we must change the
   status of all columns in the column list. */

സ്ഥിതം ശൂന്യം
hold_file (COLUMN *p)
{
  COLUMN *q;
  പൂർണ്ണസംഖ്യ i;

  എങ്കിൽ (!parallel_files)
    വേണ്ടി (q = column_vector, i = columns; i; ++q, --i)
      {
        എങ്കിൽ (storing_columns)
          q->status = FF_FOUND;
        അല്ലെങ്കിൽ
          q->status = ON_HOLD;
      }
  അല്ലെങ്കിൽ
    p->status = ON_HOLD;

  p->വരികൾ_to_print = 0;
  --files_ready_to_read;
}

/* Undo hold_file -- go through the column list and change any
   ON_HOLD columns to OPEN.  Used at the end of each page. */

സ്ഥിതം ശൂന്യം
reset_status (ശൂന്യം)
{
  പൂർണ്ണസംഖ്യ i = columns;
  COLUMN *p;

  വേണ്ടി (p = column_vector; i; --i, ++p)
    എങ്കിൽ (p->status == ON_HOLD)
      {
        p->status = OPEN;
        files_ready_to_read++;
      }

  എങ്കിൽ (storing_columns)
    {
      എങ്കിൽ (column_vector->status == CLOSED)
        /* We use the info to output an error message in  skip_to_page. */
        files_ready_to_read = 0;
      അല്ലെങ്കിൽ
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

സ്ഥിതം ശൂന്യം
print_files (പൂർണ്ണസംഖ്യ സംഖ്യ_of_files, അക്ഷരം **av)
{
  init_parameters (സംഖ്യ_of_files);
  എങ്കിൽ (! init_fps (സംഖ്യ_of_files, av))
    തിരികെ;
  എങ്കിൽ (storing_columns)
    init_store_cols ();

  എങ്കിൽ (first_page_സംഖ്യ > 1)
    {
      എങ്കിൽ (!skip_to_page (first_page_സംഖ്യ))
        തിരികെ;
      അല്ലെങ്കിൽ
        page_സംഖ്യ = first_page_സംഖ്യ;
    }
  അല്ലെങ്കിൽ
    page_സംഖ്യ = 1;

  init_funcs ();

  വരി_സംഖ്യ = വരി_എണ്ണം;
  ആകുമ്പോൾ (print_page ())
    ;
}

/* Initialize header information.
   If DESC is non-negative, it is a file descriptor open to
   FILENAME for reading.  */

സ്ഥിതം ശൂന്യം
init_header (അക്ഷരം സ്ഥിരം *filename, പൂർണ്ണസംഖ്യ desc)
{
  അക്ഷരം *buf = ശൂന്യ;
  ഘടന stat st;
  ഘടന timespec t;
  പൂർണ്ണസംഖ്യ ns;
  ഘടന tm tm;

  /* If parallel files or standard input, use current date. */
  എങ്കിൽ (streq (filename, "-"))
    desc = -1;
  എങ്കിൽ (0 <= desc && fstat (desc, &st) == 0)
    t = get_stat_mtime (&st);
  അല്ലെങ്കിൽ
    {
      സ്ഥിതം ഘടന timespec timespec;
      എങ്കിൽ (! timespec.tv_sec)
        gettime (&timespec);
      t = timespec;
    }

  ns = t.tv_nsec;
  എങ്കിൽ (localtime_rz (localtz, &t.tv_sec, &tm))
    {
      ptrdiff_t len = nstrftime (ശൂന്യ, MIN (PTRDIFF_MAX, SIZE_MAX),
                                 date_format, &tm, localtz, ns);
      എങ്കിൽ (0 <= len)
        {
          buf = ximalloc (len + 1);
          nstrftime (buf, len + 1, date_format, &tm, localtz, ns);
        }
    }
  എങ്കിൽ (!buf)
    {
      അക്ഷരം secbuf[INT_BUFSIZE_BOUND (intmax_t)];
      buf = xmalloc (വലുപ്പം secbuf + MAX (10, INT_BUFSIZE_BOUND (പൂർണ്ണസംഖ്യ)));
      sprintf (buf, "%s.%09d", timetostr (t.tv_sec, secbuf), ns);
    }

  സ്വതന്ത്രം (date_text);
  date_text = buf;
  file_text = custom_header ? custom_header : desc < 0 ? "" : filename;
  header_width_available = (chars_per_വരി
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

സ്ഥിതം ശൂന്യം
init_page (ശൂന്യം)
{
  പൂർണ്ണസംഖ്യ j;
  COLUMN *p;

  എങ്കിൽ (storing_columns)
    {
      store_columns ();
      വേണ്ടി (j = columns - 1, p = column_vector; j; --j, ++p)
        {
          p->വരികൾ_to_print = p->വരികൾ_stored;
        }

      /* Last column. */
      എങ്കിൽ (balance_columns)
        {
          p->വരികൾ_to_print = p->വരികൾ_stored;
        }
      /* Since we're not balancing columns, we don't need to store
         the rightmost column.   Read it straight from the file. */
      അല്ലെങ്കിൽ
        {
          എങ്കിൽ (p->status == OPEN)
            {
              p->വരികൾ_to_print = വരികൾ_per_body;
            }
          അല്ലെങ്കിൽ
            p->വരികൾ_to_print = 0;
        }
    }
  അല്ലെങ്കിൽ
    വേണ്ടി (j = columns, p = column_vector; j; --j, ++p)
      എങ്കിൽ (p->status == OPEN)
        {
          p->വരികൾ_to_print = വരികൾ_per_body;
        }
      അല്ലെങ്കിൽ
        p->വരികൾ_to_print = 0;
}

/* Align empty columns and print separators.
   Empty columns will be formed by files with status ON_HOLD or CLOSED
   when printing multiple files in parallel. */

സ്ഥിതം ശൂന്യം
align_column (COLUMN *p)
{
  padding_not_printed = p->start_position;
  എങ്കിൽ (col_sep_length < padding_not_printed)
    {
      pad_across_to (padding_not_printed - col_sep_length);
      padding_not_printed = ANYWHERE;
    }

  എങ്കിൽ (use_col_separator)
    print_sep_string ();

  എങ്കിൽ (p->numbered)
    add_വരി_സംഖ്യ (p);
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

സ്ഥിതം bool
print_page (ശൂന്യം)
{
  പൂർണ്ണസംഖ്യ j;
  പൂർണ്ണസംഖ്യ വരികൾ_ഇടത്_on_page;
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

  എങ്കിൽ (cols_ready_to_print () == 0)
    തിരികെ false;

  എങ്കിൽ (extremities)
    print_a_header = true;

  /* Don't pad unless we know a page was printed. */
  pad_vertically = false;
  pv = false;

  വരികൾ_ഇടത്_on_page = വരികൾ_per_body;
  എങ്കിൽ (double_space)
    വരികൾ_ഇടത്_on_page *= 2;

  ആകുമ്പോൾ (വരികൾ_ഇടത്_on_page > 0 && cols_ready_to_print () > 0)
    {
      output_position = 0;
      spaces_not_printed = 0;
      separators_not_printed = 0;
      pad_vertically = false;
      align_empty_cols = false;
      empty_വരി = true;

      വേണ്ടി (j = 1, p = column_vector; j <= columns; ++j, ++p)
        {
          നിവേശം_position = 0;
          എങ്കിൽ (p->വരികൾ_to_print > 0 || p->status == FF_FOUND)
            {
              FF_only = false;
              padding_not_printed = p->start_position;
              എങ്കിൽ (!(p->print_func) (p))
                read_rest_of_വരി (p);
              pv |= pad_vertically;

              --p->വരികൾ_to_print;
              എങ്കിൽ (p->വരികൾ_to_print <= 0)
                {
                  എങ്കിൽ (cols_ready_to_print () == 0)
                    നിർത്തുക;
                }

              /* File p changed its status to ON_HOLD or CLOSED */
              എങ്കിൽ (parallel_files && p->status != OPEN)
                {
                  എങ്കിൽ (empty_വരി)
                    align_empty_cols = true;
                  അല്ലെങ്കിൽ എങ്കിൽ (p->status == CLOSED
                           || (p->status == ON_HOLD && FF_only))
                    align_column (p);
                }
            }
          അല്ലെങ്കിൽ എങ്കിൽ (parallel_files)
            {
              /* File status ON_HOLD or CLOSED */
              എങ്കിൽ (empty_വരി)
                align_empty_cols = true;
              അല്ലെങ്കിൽ
                align_column (p);
            }

          /* We need it also with an empty column */
          എങ്കിൽ (use_col_separator)
            ++separators_not_printed;
        }

      എങ്കിൽ (pad_vertically)
        {
          putchar ('\n');
          --വരികൾ_ഇടത്_on_page;
        }

      എങ്കിൽ (cols_ready_to_print () == 0 && !extremities)
        നിർത്തുക;

      എങ്കിൽ (double_space && pv)
        {
          putchar ('\n');
          --വരികൾ_ഇടത്_on_page;
        }
    }

  എങ്കിൽ (വരികൾ_ഇടത്_on_page == 0)
    വേണ്ടി (j = 1, p = column_vector; j <= columns; ++j, ++p)
      എങ്കിൽ (p->status == OPEN)
        p->full_page_printed = true;

  pad_vertically = pv;

  എങ്കിൽ (pad_vertically && extremities)
    pad_down (വരികൾ_ഇടത്_on_page + വരികൾ_per_footer);
  അല്ലെങ്കിൽ എങ്കിൽ (keep_FF && print_a_FF)
    {
      putchar ('\f');
      print_a_FF = false;
    }

  എങ്കിൽ (ferror (stdout))
    write_error ();

  എങ്കിൽ (last_page_സംഖ്യ < ++page_സംഖ്യ)
    തിരികെ false;		/* Stop printing with LAST_PAGE */

  reset_status ();		/* Change ON_HOLD to OPEN. */

  തിരികെ true;			/* More pages to go. */
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

സ്ഥിതം ശൂന്യം
init_store_cols (ശൂന്യം)
{
  /* Tune this.  */
  പൂർണ്ണസംഖ്യ total_വരികൾ, total_വരികൾ_1, chars_per_column_1, chars_if_truncate;
  എങ്കിൽ (ckd_mul (&total_വരികൾ, വരികൾ_per_body, columns)
      || ckd_add (&total_വരികൾ_1, total_വരികൾ, 1)
      || ckd_add (&chars_per_column_1, chars_per_column, 1)
      || ckd_mul (&chars_if_truncate, total_വരികൾ, chars_per_column_1)
      || ckd_mul (&buff_allocated, chars_if_truncate, use_col_separator + 1))
    integer_overflow ();

  സ്വതന്ത്രം (വരി_vector);
  /* FIXME: here's where it was allocated.  */
  വരി_vector = xnmalloc (total_വരികൾ_1, വലുപ്പം *വരി_vector);

  സ്വതന്ത്രം (end_vector);
  end_vector = xnmalloc (total_വരികൾ, വലുപ്പം *end_vector);

  സ്വതന്ത്രം (buff);
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

സ്ഥിതം ശൂന്യം
store_columns (ശൂന്യം)
{
  പൂർണ്ണസംഖ്യ i, j;
  unsigned പൂർണ്ണസംഖ്യ വരി = 0;
  unsigned പൂർണ്ണസംഖ്യ buff_start;
  പൂർണ്ണസംഖ്യ last_col;		/* The rightmost column which will be saved in buff */
  COLUMN *p;

  buff_current = 0;
  buff_start = 0;

  എങ്കിൽ (balance_columns)
    last_col = columns;
  അല്ലെങ്കിൽ
    last_col = columns - 1;

  വേണ്ടി (i = 1, p = column_vector; i <= last_col; ++i, ++p)
    p->വരികൾ_stored = 0;

  വേണ്ടി (i = 1, p = column_vector; i <= last_col && files_ready_to_read;
       ++i, ++p)
    {
      p->current_വരി = വരി;
      വേണ്ടി (j = വരികൾ_per_body; j && files_ready_to_read; --j)

        എങ്കിൽ (p->status == OPEN)	/* Redundant.  Clean up. */
          {
            നിവേശം_position = 0;

            എങ്കിൽ (!read_വരി (p))
              read_rest_of_വരി (p);

            എങ്കിൽ (p->status == OPEN
                || buff_start != buff_current)
              {
                ++p->വരികൾ_stored;
                വരി_vector[വരി] = buff_start;
                end_vector[വരി++] = നിവേശം_position;
                buff_start = buff_current;
              }
          }
    }

  /* Keep track of the location of the last char in buff. */
  വരി_vector[വരി] = buff_start;

  എങ്കിൽ (balance_columns)
    balance (വരി);
}

സ്ഥിതം ശൂന്യം
balance (പൂർണ്ണസംഖ്യ total_stored)
{
  COLUMN *p;
  പൂർണ്ണസംഖ്യ i, വരികൾ;
  പൂർണ്ണസംഖ്യ first_വരി = 0;

  വേണ്ടി (i = 1, p = column_vector; i <= columns; ++i, ++p)
    {
      വരികൾ = total_stored / columns;
      എങ്കിൽ (i <= total_stored % columns)
        ++വരികൾ;

      p->വരികൾ_stored = വരികൾ;
      p->current_വരി = first_വരി;

      first_വരി += വരികൾ;
    }
}

/* Store a character in the buffer. */

സ്ഥിതം ശൂന്യം
store_char (അക്ഷരം c)
{
  എങ്കിൽ (buff_current >= buff_allocated)
    {
      /* May be too generous. */
      buff = xpalloc (buff, &buff_allocated, 1, -1, വലുപ്പം *buff);
    }
  buff[buff_current++] = c;
}

സ്ഥിതം ശൂന്യം
add_വരി_സംഖ്യ (COLUMN *p)
{
  പൂർണ്ണസംഖ്യ i;
  അക്ഷരം *s;
  പൂർണ്ണസംഖ്യ അക്കം_width;

  /* Cutting off the higher-order digits is more informative than
     lower-order cut off. */
  അക്കം_width = sprintf (സംഖ്യ_buff, "%*d", chars_per_സംഖ്യ, വരി_സംഖ്യ);
  വരി_സംഖ്യ++;
  s = സംഖ്യ_buff + (അക്കം_width - chars_per_സംഖ്യ);
  വേണ്ടി (i = chars_per_സംഖ്യ; i > 0; i--)
    (p->char_func) (*s++);

  എങ്കിൽ (columns > 1)
    {
      /* Tabification is assumed for multiple columns, also for n-separators,
         but 'default n-separator = TAB' hasn't been given priority over
         equal column_width also specified by POSIX. */
      എങ്കിൽ (സംഖ്യ_separator == '\t')
        {
          i = സംഖ്യ_width - chars_per_സംഖ്യ;
          ആകുമ്പോൾ (i-- > 0)
            (p->char_func) (' ');
        }
      അല്ലെങ്കിൽ
        (p->char_func) (സംഖ്യ_separator);
    }
  അല്ലെങ്കിൽ
    /* To comply with POSIX, we avoid any expansion of default TAB
       separator with a single column output. No column_width requirement
       has to be considered. */
    {
      (p->char_func) (സംഖ്യ_separator);
      എങ്കിൽ (സംഖ്യ_separator == '\t')
        output_position = POS_AFTER_TAB (chars_per_output_tab,
                          output_position);
    }

  എങ്കിൽ (truncate_വരികൾ && !parallel_files)
    നിവേശം_position += സംഖ്യ_width;
}

/* Print (or store) padding until the current horizontal position
   is position. */

സ്ഥിതം ശൂന്യം
pad_across_to (പൂർണ്ണസംഖ്യ position)
{
  പൂർണ്ണസംഖ്യ h = output_position;

  എങ്കിൽ (tabify_output)
    spaces_not_printed = position - output_position;
  അല്ലെങ്കിൽ
    {
      ആകുമ്പോൾ (++h <= position)
        putchar (' ');
      output_position = position;
    }
}

/* Pad to the bottom of the page.

   If the user has requested a formfeed, use one.
   Otherwise, use newlines. */

സ്ഥിതം ശൂന്യം
pad_down (unsigned പൂർണ്ണസംഖ്യ വരികൾ)
{
  എങ്കിൽ (use_form_feed)
    putchar ('\f');
  അല്ലെങ്കിൽ
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = വരികൾ; i; --i)
      putchar ('\n');
}

/* Read the rest of the line.

   Read from the current column's file until an end of line is
   hit.  Used when we've truncated a line and we no longer need
   to print or store its characters. */

സ്ഥിതം ശൂന്യം
read_rest_of_വരി (COLUMN *p)
{
  പൂർണ്ണസംഖ്യ c;
  FILE *f = p->fp;

  ആകുമ്പോൾ ((c = getc (f)) != '\n')
    {
      എങ്കിൽ (c == '\f')
        {
          എങ്കിൽ ((c = getc (f)) != '\n')
            ungetc (c, f);
          എങ്കിൽ (keep_FF)
            print_a_FF = true;
          hold_file (p);
          നിർത്തുക;
        }
      അല്ലെങ്കിൽ എങ്കിൽ (c == EOF)
        {
          close_file (p);
          നിർത്തുക;
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

സ്ഥിതം ശൂന്യം
skip_read (COLUMN *p, പൂർണ്ണസംഖ്യ column_സംഖ്യ)
{
  പൂർണ്ണസംഖ്യ c;
  FILE *f = p->fp;
  പൂർണ്ണസംഖ്യ i;
  bool single_ff = false;
  COLUMN *q;

  /* Read 1st character in a line or any character succeeding a FF */
  എങ്കിൽ ((c = getc (f)) == '\f' && p->full_page_printed)
    /* A FF-coincidence with a previous full_page_printed.
       To avoid an additional empty page, eliminate the FF */
    എങ്കിൽ ((c = getc (f)) == '\n')
      c = getc (f);

  p->full_page_printed = false;

  /* 1st character a FF means a single FF without any printable
     characters. Don't count it as a line with -n option. */
  എങ്കിൽ (c == '\f')
    single_ff = true;

  /* Preparing for a FF-coincidence: Maybe we finish that page
     without a FF found */
  എങ്കിൽ (last_വരി)
    p->full_page_printed = true;

  ആകുമ്പോൾ (c != '\n')
    {
      എങ്കിൽ (c == '\f')
        {
          /* No FF-coincidence possible,
             no catching up of a FF-coincidence with next page */
          എങ്കിൽ (last_വരി)
            {
              എങ്കിൽ (!parallel_files)
                വേണ്ടി (q = column_vector, i = columns; i; ++q, --i)
                  q->full_page_printed = false;
              അല്ലെങ്കിൽ
                p->full_page_printed = false;
            }

          എങ്കിൽ ((c = getc (f)) != '\n')
            ungetc (c, f);
          hold_file (p);
          നിർത്തുക;
        }
      അല്ലെങ്കിൽ എങ്കിൽ (c == EOF)
        {
          close_file (p);
          നിർത്തുക;
        }
      c = getc (f);
    }

  എങ്കിൽ (skip_എണ്ണം)
    എങ്കിൽ ((!parallel_files || column_സംഖ്യ == 1) && !single_ff)
      ++വരി_എണ്ണം;
}

/* If we're tabifying output,

   When print_char encounters white space it keeps track
   of our desired horizontal position and delays printing
   until this function is called. */

സ്ഥിതം ശൂന്യം
print_white_space (ശൂന്യം)
{
  പൂർണ്ണസംഖ്യ h_new;
  പൂർണ്ണസംഖ്യ h_old = output_position;
  പൂർണ്ണസംഖ്യ goal = h_old + spaces_not_printed;

  ആകുമ്പോൾ (goal - h_old > 1
         && (h_new = POS_AFTER_TAB (chars_per_output_tab, h_old)) <= goal)
    {
      putchar (output_tab_char);
      h_old = h_new;
    }
  ആകുമ്പോൾ (++h_old <= goal)
    putchar (' ');

  output_position = goal;
  spaces_not_printed = 0;
}

/* Print column separators.

   We keep a count until we know that we'll be printing a line,
   then print_sep_string() is called. */

സ്ഥിതം ശൂന്യം
print_sep_string (ശൂന്യം)
{
  അക്ഷരം സ്ഥിരം *s = col_sep_string;
  പൂർണ്ണസംഖ്യ l = col_sep_length;

  എങ്കിൽ (separators_not_printed <= 0)
    {
      /* We'll be starting a line with chars_per_margin, anything else? */
      എങ്കിൽ (spaces_not_printed > 0)
        print_white_space ();
    }
  അല്ലെങ്കിൽ
    {
      വേണ്ടി (; separators_not_printed > 0; --separators_not_printed)
        {
          ആകുമ്പോൾ (l-- > 0)
            {
              /* 3 types of sep_strings: spaces only, spaces and chars,
              chars only */
              എങ്കിൽ (*s == ' ')
                {
                  /* We're tabifying output; consecutive spaces in
                  sep_string may have to be converted to tabs */
                  s++;
                  ++spaces_not_printed;
                }
              അല്ലെങ്കിൽ
                {
                  എങ്കിൽ (spaces_not_printed > 0)
                    print_white_space ();
                  putchar (*s++);
                  ++output_position;
                }
            }
          /* sep_string ends with some spaces */
          എങ്കിൽ (spaces_not_printed > 0)
            print_white_space ();
        }
    }
}

/* Print (or store, depending on p->char_func) a clump of N
   characters. */

സ്ഥിതം ശൂന്യം
print_clump (COLUMN *p, പൂർണ്ണസംഖ്യ n, അക്ഷരം *clump)
{
  ആകുമ്പോൾ (n--)
    (p->char_func) (*clump++);

  എങ്കിൽ (ferror (stdout))
    write_error ();
}

/* Print a character.

   Update the following comment: process-char hasn't been used any
   longer.
   If we're tabifying, all tabs have been converted to spaces by
   process_char().  Keep a count of consecutive spaces, and when
   a nonspace is encountered, call print_white_space() to print the
   required number of tabs and spaces. */

സ്ഥിതം ശൂന്യം
print_char (അക്ഷരം c)
{
  എങ്കിൽ (tabify_output)
    {
      എങ്കിൽ (c == ' ')
        {
          ++spaces_not_printed;
          തിരികെ;
        }
      അല്ലെങ്കിൽ എങ്കിൽ (spaces_not_printed > 0)
        print_white_space ();

      /* Nonprintables are assumed to have width 0, except '\b'. */
      എങ്കിൽ (! isprint (to_uchar (c)))
        {
          എങ്കിൽ (c == '\b')
            --output_position;
        }
      അല്ലെങ്കിൽ
        ++output_position;
    }
  putchar (c);
}

/* Skip to page PAGE before printing.
   PAGE may be larger than total number of pages. */

സ്ഥിതം bool
skip_to_page (uintmax_t page)
{
  വേണ്ടി (uintmax_t n = 1; n < page; ++n)
    {
      COLUMN *p;
      പൂർണ്ണസംഖ്യ j;

      വേണ്ടി (പൂർണ്ണസംഖ്യ i = 1; i < വരികൾ_per_body; ++i)
        {
          വേണ്ടി (j = 1, p = column_vector; j <= columns; ++j, ++p)
            എങ്കിൽ (p->status == OPEN)
              skip_read (p, j);
        }
      last_വരി = true;
      വേണ്ടി (j = 1, p = column_vector; j <= columns; ++j, ++p)
        എങ്കിൽ (p->status == OPEN)
          skip_read (p, j);

      എങ്കിൽ (storing_columns)	/* change FF_FOUND to ON_HOLD */
        വേണ്ടി (j = 1, p = column_vector; j <= columns; ++j, ++p)
          എങ്കിൽ (p->status != CLOSED)
            p->status = ON_HOLD;

      reset_status ();
      last_വരി = false;

      എങ്കിൽ (files_ready_to_read < 1)
        {
          /* It's very helpful, normally the total number of pages is
             not known in advance.  */
          error (0, 0,
                 _("starting page number %ju exceeds page count %ju"),
                 page, n);
          നിർത്തുക;
        }
    }
  തിരികെ files_ready_to_read > 0;
}

/* Print a header.

   Formfeeds are assumed to use up two lines at the beginning of
   the page. */

സ്ഥിതം ശൂന്യം
print_header (ശൂന്യം)
{
  അക്ഷരം page_text[256 + INT_STRLEN_BOUND (page_സംഖ്യ)];
  പൂർണ്ണസംഖ്യ available_width;
  പൂർണ്ണസംഖ്യ lhs_spaces;
  പൂർണ്ണസംഖ്യ rhs_spaces;

  output_position = 0;
  pad_across_to (chars_per_margin);
  print_white_space ();

  എങ്കിൽ (page_സംഖ്യ == 0)
    error (EXIT_FAILURE, 0, _("page number overflow"));

  /* The translator must ensure that formatting the translation of
     "Page %ju" does not generate more than (sizeof page_text - 1)
     bytes.  */
  sprintf (page_text, _("Page %ju"), page_സംഖ്യ);
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

സ്ഥിതം bool
read_വരി (COLUMN *p)
{
  പൂർണ്ണസംഖ്യ c;
  പൂർണ്ണസംഖ്യ chars IF_LINT (= 0);
  പൂർണ്ണസംഖ്യ last_നിവേശം_position;
  പൂർണ്ണസംഖ്യ j, k;
  COLUMN *q;

  /* read 1st character in each line or any character succeeding a FF: */
  c = getc (p->fp);

  last_നിവേശം_position = നിവേശം_position;

  എങ്കിൽ (c == '\f' && p->full_page_printed)
    എങ്കിൽ ((c = getc (p->fp)) == '\n')
      c = getc (p->fp);
  p->full_page_printed = false;

  തിരഞ്ഞെടുപ്പ് (c)
    {
    അവസ്ഥ '\f':
      എങ്കിൽ ((c = getc (p->fp)) != '\n')
        ungetc (c, p->fp);
      FF_only = true;
      എങ്കിൽ (print_a_header && !storing_columns)
        {
          pad_vertically = true;
          print_header ();
        }
      അല്ലെങ്കിൽ എങ്കിൽ (keep_FF)
        print_a_FF = true;
      hold_file (p);
      തിരികെ true;
    അവസ്ഥ EOF:
      close_file (p);
      തിരികെ true;
    അവസ്ഥ '\n':
      നിർത്തുക;
    സ്ഥിരസ്ഥിതി:
      chars = char_to_clump (c);
    }

  എങ്കിൽ (truncate_വരികൾ && നിവേശം_position > chars_per_column)
    {
      നിവേശം_position = last_നിവേശം_position;
      തിരികെ false;
    }

  എങ്കിൽ (p->char_func != store_char)
    {
      pad_vertically = true;

      എങ്കിൽ (print_a_header && !storing_columns)
        print_header ();

      എങ്കിൽ (parallel_files && align_empty_cols)
        {
          /* We have to align empty columns at the beginning of a line. */
          k = separators_not_printed;
          separators_not_printed = 0;
          വേണ്ടി (j = 1, q = column_vector; j <= k; ++j, ++q)
            {
              align_column (q);
              separators_not_printed += 1;
            }
          padding_not_printed = p->start_position;
          എങ്കിൽ (truncate_വരികൾ)
            spaces_not_printed = chars_per_column;
          അല്ലെങ്കിൽ
            spaces_not_printed = 0;
          align_empty_cols = false;
        }

      എങ്കിൽ (col_sep_length < padding_not_printed)
        {
          pad_across_to (padding_not_printed - col_sep_length);
          padding_not_printed = ANYWHERE;
        }

      എങ്കിൽ (use_col_separator)
        print_sep_string ();
    }

  എങ്കിൽ (p->numbered)
    add_വരി_സംഖ്യ (p);

  empty_വരി = false;
  എങ്കിൽ (c == '\n')
    തിരികെ true;

  print_clump (p, chars, clump_buff);

  ആകുമ്പോൾ (true)
    {
      c = getc (p->fp);

      തിരഞ്ഞെടുപ്പ് (c)
        {
        അവസ്ഥ '\n':
          തിരികെ true;
        അവസ്ഥ '\f':
          എങ്കിൽ ((c = getc (p->fp)) != '\n')
            ungetc (c, p->fp);
          എങ്കിൽ (keep_FF)
            print_a_FF = true;
          hold_file (p);
          തിരികെ true;
        അവസ്ഥ EOF:
          close_file (p);
          തിരികെ true;
        }

      last_നിവേശം_position = നിവേശം_position;
      chars = char_to_clump (c);
      എങ്കിൽ (truncate_വരികൾ && നിവേശം_position > chars_per_column)
        {
          നിവേശം_position = last_നിവേശം_position;
          തിരികെ false;
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

സ്ഥിതം bool
print_stored (COLUMN *p)
{
  COLUMN *q;

  പൂർണ്ണസംഖ്യ വരി = p->current_വരി++;
  അക്ഷരം *first = &buff[വരി_vector[വരി]];
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
  അക്ഷരം *last = &buff[വരി_vector[വരി + 1]];

  pad_vertically = true;

  എങ്കിൽ (print_a_header)
    print_header ();

  എങ്കിൽ (p->status == FF_FOUND)
    {
      പൂർണ്ണസംഖ്യ i;
      വേണ്ടി (i = 1, q = column_vector; i <= columns; ++i, ++q)
        q->status = ON_HOLD;
      എങ്കിൽ (column_vector->വരികൾ_to_print <= 0)
        {
          എങ്കിൽ (!extremities)
            pad_vertically = false;
          തിരികെ true;		/* print a header only */
        }
    }

  എങ്കിൽ (col_sep_length < padding_not_printed)
    {
      pad_across_to (padding_not_printed - col_sep_length);
      padding_not_printed = ANYWHERE;
    }

  എങ്കിൽ (use_col_separator)
    print_sep_string ();

  ആകുമ്പോൾ (first != last)
    print_char (*first++);

  എങ്കിൽ (spaces_not_printed == 0)
    {
      output_position = p->start_position + end_vector[വരി];
      എങ്കിൽ (p->start_position - col_sep_length == chars_per_margin)
        output_position -= col_sep_length;
    }

  തിരികെ true;
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

സ്ഥിതം പൂർണ്ണസംഖ്യ
char_to_clump (അക്ഷരം c)
{
  unsigned അക്ഷരം uc = c;
  അക്ഷരം *s = clump_buff;
  അക്ഷരം esc_buff[4];
  പൂർണ്ണസംഖ്യ width;
  പൂർണ്ണസംഖ്യ chars;
  പൂർണ്ണസംഖ്യ chars_per_c = 8;

  എങ്കിൽ (c == നിവേശം_tab_char)
    chars_per_c = chars_per_നിവേശം_tab;

  എങ്കിൽ (c == നിവേശം_tab_char || c == '\t')
    {
      width = TAB_WIDTH (chars_per_c, നിവേശം_position);

      എങ്കിൽ (untabify_നിവേശം)
        {
          വേണ്ടി (പൂർണ്ണസംഖ്യ i = width; i; --i)
            *s++ = ' ';
          chars = width;
        }
      അല്ലെങ്കിൽ
        {
          *s = c;
          chars = 1;
        }

    }
  അല്ലെങ്കിൽ എങ്കിൽ (! isprint (uc))
    {
      എങ്കിൽ (use_esc_sequence)
        {
          width = 4;
          chars = 4;
          *s++ = '\\';
          sprintf (esc_buff, "%03o", uc);
          വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i <= 2; ++i)
            *s++ = esc_buff[i];
        }
      അല്ലെങ്കിൽ എങ്കിൽ (use_cntrl_prefix)
        {
          എങ്കിൽ (uc < 0200)
            {
              width = 2;
              chars = 2;
              *s++ = '^';
              *s = c ^ 0100;
            }
          അല്ലെങ്കിൽ
            {
              width = 4;
              chars = 4;
              *s++ = '\\';
              sprintf (esc_buff, "%03o", uc);
              വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i <= 2; ++i)
                *s++ = esc_buff[i];
            }
        }
      അല്ലെങ്കിൽ എങ്കിൽ (c == '\b')
        {
          width = -1;
          chars = 1;
          *s = c;
        }
      അല്ലെങ്കിൽ
        {
          width = 0;
          chars = 1;
          *s = c;
        }
    }
  അല്ലെങ്കിൽ
    {
      width = 1;
      chars = 1;
      *s = c;
    }

  /* Too many backspaces must put us in position 0 -- never negative.  */
  എങ്കിൽ (width < 0 && നിവേശം_position == 0)
    {
      chars = 0;
      നിവേശം_position = 0;
    }
  അല്ലെങ്കിൽ എങ്കിൽ (width < 0 && നിവേശം_position <= -width)
    നിവേശം_position = 0;
  അല്ലെങ്കിൽ
    {
      എങ്കിൽ (ckd_add (&നിവേശം_position, നിവേശം_position, width))
        integer_overflow ();
    }

  തിരികെ chars;
}

/* We've just printed some files and need to clean up things before
   looking for more options and printing the next batch of files.

   Free everything we've xmalloc'ed, except 'header'. */

സ്ഥിതം ശൂന്യം
cleanup (ശൂന്യം)
{
  സ്വതന്ത്രം (സംഖ്യ_buff);
  സ്വതന്ത്രം (clump_buff);
  സ്വതന്ത്രം (column_vector);
  സ്വതന്ത്രം (വരി_vector);
  സ്വതന്ത്രം (end_vector);
  സ്വതന്ത്രം (buff);
}

/* Complain, print a usage message, and die. */

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              program_name);

      fputs (_("\
Paginate or columnate FILE(s) വേണ്ടി printing.\n\
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
         Balance സംഖ്യ of വരികൾ in the columns on each page\n\
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
  -d, --ഇരട്ട-space\n\
         ഇരട്ട space the output\n\
"));
      oputs (_("\
  -D, --date-format=FORMAT\n\
         use FORMAT വേണ്ടി the header date\n\
"));
      oputs (_("\
  -e[CHAR[WIDTH]], --expand-tabs[=CHAR[WIDTH]]\n\
         expand നിവേശം CHARs (TABs) to tab WIDTH (8)\n\
"));
      oputs (_("\
  -F, -f, --form-feed\n\
         use form feeds instead of newlines to separate pages\n\
         (by a 3-വരി page header with -F or a 5-വരി header\n\
         and trailer without -F)\n\
"));
      oputs (_("\
  -h, --header=HEADER\n\
         use a centered HEADER instead of filename in page header,\n\
         -h \"\" prints a blank വരി, don't use -h\"\"\n\
"));
      oputs (_("\
  -i[CHAR[WIDTH]], --output-tabs[=CHAR[WIDTH]]\n\
         replace spaces with CHARs (TABs) to tab WIDTH (8)\n\
"));
      oputs (_("\
  -J, --join-വരികൾ\n\
         merge full വരികൾ, turns off -W വരി truncation,\n\
         no column alignment, --sep-string[=STRING] sets separators\n\
"));
      oputs (_("\
  -l, --length=PAGE_LENGTH\n\
         set the page length to PAGE_LENGTH (66) വരികൾ\n\
         (സ്ഥിരസ്ഥിതി സംഖ്യ of വരികൾ of text 56, and with -F 63).\n\
         implies -t എങ്കിൽ PAGE_LENGTH <= 10\n\
"));
      oputs (_("\
  -m, --merge\n\
         print all files in parallel, one in each column,\n\
         truncate വരികൾ, but join വരികൾ of full length with -J\n\
"));
      oputs (_("\
  -n[SEP[DIGITS]], --സംഖ്യ-വരികൾ[=SEP[DIGITS]]\n\
         സംഖ്യ വരികൾ, use DIGITS (5) digits, then SEP (TAB),\n\
         സ്ഥിരസ്ഥിതി counting starts with 1st വരി of നിവേശം file\n\
"));
      oputs (_("\
  -N, --first-വരി-സംഖ്യ=സംഖ്യ_2\n\
         start counting with സംഖ്യ_2 at 1st വരി of first\n\
         page printed (see +FIRST_PAGE)\n\
"));
      oputs (_("\
  -o, --indent=MARGIN\n\
         offset each വരി with MARGIN (zero) spaces, ചെയ്യുക not\n\
         affect -w or -W, MARGIN will be added to PAGE_WIDTH\n\
"));
      oputs (_("\
  -r, --no-file-warnings\n\
         omit warning when a file cannot be opened\n\
"));
      oputs (_("\
  -s[CHAR], --separator[=CHAR]\n\
         separate columns by a single character, സ്ഥിരസ്ഥിതി വേണ്ടി CHAR\n\
         is the <TAB> character without -w and \'no അക്ഷരം\' with -w.\n\
         -s[CHAR] turns off വരി truncation of all 3 column\n\
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
         omit page headers and trailers; implied എങ്കിൽ PAGE_LENGTH <= 10\n\
"));
      oputs (_("\
  -T, --omit-pagination\n\
         omit page headers and trailers,\n\
         eliminate any pagination by form feeds set in നിവേശം files\n\
"));
      oputs (_("\
  -v, --show-nonprinting\n\
         use octal backslash notation\n\
"));
      oputs (_("\
  -w, --width=PAGE_WIDTH\n\
         set page width to PAGE_WIDTH (72) characters വേണ്ടി\n\
         multiple text-column output only, -s[അക്ഷരം] turns off (72)\n\
"));
      oputs (_("\
  -W, --page-width=PAGE_WIDTH\n\
         set page width to PAGE_WIDTH (72) characters always,\n\
         truncate വരികൾ, except -J option is set,\n\
         no interference with -S or -s\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}
