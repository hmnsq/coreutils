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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "c-ctype.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "fadvise.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hard-locale.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "mbswidth.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "stat-time.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "stdio--.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strftime.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xstrtol.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xstrtol-error.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "pr"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS \
  proper_name ("Pete TerMaat"), \
  proper_name ("Roland Huebner")

/* Used with start_position in the struct COLUMN described below.
   If start_position == ANYWHERE, we aren't truncating columns and
   can begin printing a column anywhere.  Otherwise we must pad to
   the horizontal position start_position. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ANYWHERE	0

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

ਬਣਤਰ ਕਾਲਮ_3;
ਬਣਤਰ ਕਾਲਮ_3
  {
    FILE *fp;			/* Input stream for this column. */
    ਅੱਖਰ ਅਚਲ *name;		/* File name. */
    ਗਿਣਤੀ
      {
        OPEN,
        FF_ਮਿਲਿਆ,		/* used with -b option, set with \f, changed
                                   to ON_HOLD after print_header */
        ON_HOLD,		/* Hit a form feed. */
        CLOSED
      }
    ਸਥਿਤੀ;			/* Status of the file pointer. */

    /* Func to print lines in this col. */
    bool (*print_func) (ਬਣਤਰ ਕਾਲਮ_3 *);

    /* Func to print/store chars in this col. */
    ਖਾਲੀ (*char_func) (ਅੱਖਰ);

    ਪੂਰਨ current_ਸਤਰ;		/* Index of current place in line_vector. */
    ਪੂਰਨ ਸਤਰਾਂ_stored;		/* Number of lines stored in buff. */
    ਪੂਰਨ ਸਤਰਾਂ_ਮੰਜ਼ਿਲ_print;		/* No. lines stored or space left on page. */
    ਪੂਰਨ ਸ਼ੁਰੂ_position;		/* Horizontal position of first char. */
    bool numbered;
    bool ਭਰਿਆ_ਪੰਨਾ_printed;	/* True means printed without a FF found. */

    /* p->full_page_printed  controls a special case of "FF set by hand":
       True means a full page has been printed without FF found. To avoid an
       additional empty page we have to ignore a FF immediately following in
       the next line. */
  };

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ਕਾਲਮ_3 ਕਾਲਮ_3;

ਸਥਿਰ ਪੂਰਨ char_ਮੰਜ਼ਿਲ_clump (ਅੱਖਰ c);
ਸਥਿਰ bool read_ਸਤਰ (ਕਾਲਮ_3 *p);
ਸਥਿਰ bool print_ਪੰਨਾ (ਖਾਲੀ);
ਸਥਿਰ bool print_stored (ਕਾਲਮ_3 *p);
ਸਥਿਰ bool open_file (ਅੱਖਰ *name, ਕਾਲਮ_3 *p);
ਸਥਿਰ bool skip_ਮੰਜ਼ਿਲ_ਪੰਨਾ (uintmax_t ਪੰਨਾ);
ਸਥਿਰ ਖਾਲੀ print_ਸਿਰਲੇਖ (ਖਾਲੀ);
ਸਥਿਰ ਖਾਲੀ pad_across_ਮੰਜ਼ਿਲ (ਪੂਰਨ position);
ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_ਸਤਰ_ਸੰਖਿਆ (ਕਾਲਮ_3 *p);
ਸਥਿਰ ਪੂਰਨ getoptnum (ਅੱਖਰ ਅਚਲ *n_str, ਪੂਰਨ min, ਅੱਖਰ ਅਚਲ *errfmt);
ਸਥਿਰ ਖਾਲੀ getoptarg (ਅੱਖਰ *arg, ਅੱਖਰ switch_char, ਅੱਖਰ *character,
                       ਪੂਰਨ *ਸੰਖਿਆ);
ਸਥਿਰ ਖਾਲੀ print_files (ਪੂਰਨ ਸੰਖਿਆ_ਦਾ_files, ਅੱਖਰ **av);
ਸਥਿਰ ਖਾਲੀ init_parameters (ਪੂਰਨ ਸੰਖਿਆ_ਦਾ_files);
ਸਥਿਰ ਖਾਲੀ init_ਸਿਰਲੇਖ (ਅੱਖਰ ਅਚਲ *filename, ਪੂਰਨ desc);
ਸਥਿਰ bool init_fps (ਪੂਰਨ ਸੰਖਿਆ_ਦਾ_files, ਅੱਖਰ **av);
ਸਥਿਰ ਖਾਲੀ init_funcs (ਖਾਲੀ);
ਸਥਿਰ ਖਾਲੀ init_store_ਖਾਨੇ_ਗਿਣਤੀ (ਖਾਲੀ);
ਸਥਿਰ ਖਾਲੀ store_columns (ਖਾਲੀ);
ਸਥਿਰ ਖਾਲੀ balance (ਪੂਰਨ ਕੁੱਲ_stored);
ਸਥਿਰ ਖਾਲੀ store_char (ਅੱਖਰ c);
ਸਥਿਰ ਖਾਲੀ pad_ਥੱਲੇ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਤਰਾਂ);
ਸਥਿਰ ਖਾਲੀ read_rest_ਦਾ_ਸਤਰ (ਕਾਲਮ_3 *p);
ਸਥਿਰ ਖਾਲੀ skip_read (ਕਾਲਮ_3 *p, ਪੂਰਨ ਕਾਲਮ_ਸੰਖਿਆ);
ਸਥਿਰ ਖਾਲੀ print_char (ਅੱਖਰ c);
ਸਥਿਰ ਖਾਲੀ cleanup (ਖਾਲੀ);
ਸਥਿਰ ਖਾਲੀ print_sep_string (ਖਾਲੀ);
ਸਥਿਰ ਖਾਲੀ separator_string (ਅੱਖਰ ਅਚਲ *optarg_S);

/* All of the columns to print.  */
ਸਥਿਰ ਕਾਲਮ_3 *ਕਾਲਮ_ਸਦਿਸ਼;

/* When printing a single file in multiple downward columns,
   we store the leftmost columns contiguously in buff.
   To print a line from buff, get the index of the first character
   from line_vector[i], and print up to line_vector[i + 1]. */
ਸਥਿਰ ਅੱਖਰ *buff;

/* Index of the position in buff where the next character
   will be stored. */
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ buff_current;

/* The number of characters in buff.
   Used for allocation of buff and to detect overflow of buff. */
ਸਥਿਰ idx_t buff_allocated;

/* Array of indices into buff.
   Each entry is an index of the first character of a line.
   This is used when storing lines to facilitate shuffling when
   we do column balancing on the last page. */
ਸਥਿਰ ਪੂਰਨ *ਸਤਰ_ਸਦਿਸ਼;

/* Array of horizontal positions.
   For each line in line_vector, end_vector[line] is the horizontal
   position we are in after printing that line.  We keep track of this
   so that we know how much we need to pad to prepare for the next
   column. */
ਸਥਿਰ ਪੂਰਨ *ਸਿਰਾ_ਸਦਿਸ਼;

/* (-m) True means we're printing multiple files in parallel. */
ਸਥਿਰ bool parallel_files = false;

/* (-m) True means a line starts with some empty columns (some files
   already CLOSED or ON_HOLD) which we have to align. */
ਸਥਿਰ bool align_ਖਾਲੀ_ਥਾਵਾਂ_ਖਾਨੇ_ਗਿਣਤੀ;

/* (-m) True means we have not yet found any printable column in a line.
   align_empty_cols = true  has to be maintained. */
ਸਥਿਰ bool ਖਾਲੀ_ਥਾਵਾਂ_ਸਤਰ;

/* (-m) False means printable column output precedes a form feed found.
   Column alignment is done only once. No additional action with that form
   feed.
   True means we found only a form feed in a column. Maybe we have to do
   some column alignment with that form feed. */
ਸਥਿਰ bool FF_only;

/* (-[0-9]+) True means we're given an option explicitly specifying
   number of columns.  Used to detect when this option is used with -m
   and when translating old options to new/long options. */
ਸਥਿਰ bool explicit_columns = false;

/* (-t|-T) False means we aren't printing headers and footers. */
ਸਥਿਰ bool extremities = true;

/* (-t) True means we retain all FF set by hand in input files.
   False is set with -T option. */
ਸਥਿਰ bool keep_FF = false;
ਸਥਿਰ bool print_a_FF = false;

/* True means we need to print a header as soon as we know we've got input
   to print after it. */
ਸਥਿਰ bool print_a_ਸਿਰਲੇਖ;

/* (-f) True means use formfeeds instead of newlines to separate pages. */
ਸਥਿਰ bool use_form_feed = false;

/* True means we have read the standard input. */
ਸਥਿਰ bool have_read_stdin = false;

/* True means the -a flag has been given. */
ਸਥਿਰ bool print_across_flag = false;

/* True means we're printing one file in multiple (>1) downward columns. */
ਸਥਿਰ bool storing_columns = true;

/* (-b) True means balance columns on the last page as Sys V does. */
/* That's no longer an independent option. With storing_columns = true
   balance_columns = true is used too (s. function init_parameters).
   We get a consistent formulation with "FF set by hand" in input files. */
ਸਥਿਰ bool balance_columns = false;

/* (-l) Number of lines on a page, including header and footer lines. */
ਸਥਿਰ ਪੂਰਨ ਸਤਰਾਂ_ਪ੍ਰਤੀ_ਪੰਨਾ = 66;

/* Number of lines in the header and footer can be reset to 0 using
   the -t flag. */
ਗਿਣਤੀ { ਸਤਰਾਂ_ਪ੍ਰਤੀ_ਸਿਰਲੇਖ = 5 };
ਸਥਿਰ ਪੂਰਨ ਸਤਰਾਂ_ਪ੍ਰਤੀ_body;
ਗਿਣਤੀ { ਸਤਰਾਂ_ਪ੍ਰਤੀ_footer = 5 };

/* (-w|-W) Width in characters of the page.  Does not include the width of
   the margin. */
ਸਥਿਰ ਪੂਰਨ chars_ਪ੍ਰਤੀ_ਸਤਰ = 72;

/* (-w|W) True means we truncate lines longer than chars_per_column. */
ਸਥਿਰ bool truncate_ਸਤਰਾਂ = false;

/* (-J) True means we join lines without any line truncation. -J
   dominates -w option. */
ਸਥਿਰ bool join_ਸਤਰਾਂ = false;

/* Number of characters in a column.  Based on col_sep_length and
   page width. */
ਸਥਿਰ ਪੂਰਨ chars_ਪ੍ਰਤੀ_ਕਾਲਮ;

/* (-e) True means convert tabs to spaces on input. */
ਸਥਿਰ bool untabify_ਨਿਵੇਸ਼ = false;

/* (-e) The input tab character. */
ਸਥਿਰ ਅੱਖਰ ਨਿਵੇਸ਼_tab_char = '\t';

/* (-e) Tabstops are at chars_per_tab, 2*chars_per_tab, 3*chars_per_tab, ...
   where the leftmost column is 1. */
ਸਥਿਰ ਪੂਰਨ chars_ਪ੍ਰਤੀ_ਨਿਵੇਸ਼_tab = 8;

/* (-i) True means convert spaces to tabs on output. */
ਸਥਿਰ bool tabify_output = false;

/* (-i) The output tab character. */
ਸਥਿਰ ਅੱਖਰ output_tab_char = '\t';

/* (-i) The width of the output tab. */
ਸਥਿਰ ਪੂਰਨ chars_ਪ੍ਰਤੀ_output_tab = 8;

/* Keeps track of pending white space.  When we hit a nonspace
   character after some whitespace, we print whitespace, tabbing
   if necessary to get to output_position + spaces_not_printed. */
ਸਥਿਰ ਪੂਰਨ spaces_not_printed;

/* (-o) Number of spaces in the left margin (tabs used when possible). */
ਸਥਿਰ ਪੂਰਨ chars_ਪ੍ਰਤੀ_margin = 0;

/* Position where the next character will fall.
   Leftmost position is 0 + chars_per_margin.
   Rightmost position is chars_per_margin + chars_per_line - 1.
   This is important for converting spaces to tabs on output. */
ਸਥਿਰ ਪੂਰਨ output_position;

/* Horizontal position relative to the current file.
   (output_position depends on where we are on the page;
   input_position depends on where we are in the file.)
   Important for converting tabs to spaces on input. */
ਸਥਿਰ ਪੂਰਨ ਨਿਵੇਸ਼_position;

/* True if there were any failed opens so we can exit with nonzero
   status.  */
ਸਥਿਰ bool failed_opens = false;

/* The number of spaces taken up if we print a tab character with width
   c_ from position h_. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TAB_ਚੌੜਾਈ(c_, h_) ((c_) - ((h_) % (c_)))

/* The horizontal position we'll be at after printing a tab character
   of width c_ from the position h_. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ POS_AFTER_TAB(c_, h_) ((h_) + TAB_ਚੌੜਾਈ (c_, h_))

/* (-NNN) Number of columns of text to print. */
ਸਥਿਰ ਪੂਰਨ columns = 1;

/* (+NNN:MMM) Page numbers on which to begin and stop printing.
   first_page_number = 0  will be used to check input only. */
ਸਥਿਰ uintmax_t ਪਹਿਲਾ_ਪੰਨਾ_ਸੰਖਿਆ = 0;
ਸਥਿਰ uintmax_t ਆਖਰੀ_ਪੰਨਾ_ਸੰਖਿਆ = UINTMAX_MAX;

/* Number of files open (not closed, not on hold). */
ਸਥਿਰ ਪੂਰਨ files_ready_ਮੰਜ਼ਿਲ_read = 0;

/* Current page number.  Displayed in header. */
ਸਥਿਰ uintmax_t ਪੰਨਾ_ਸੰਖਿਆ;

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
ਸਥਿਰ ਪੂਰਨ ਸਤਰ_ਸੰਖਿਆ;

/* (-n) True means lines should be preceded by numbers. */
ਸਥਿਰ bool numbered_ਸਤਰਾਂ = false;

/* (-n) Character which follows each line number. */
ਸਥਿਰ ਅੱਖਰ ਸੰਖਿਆ_separator = '\t';

/* (-n) line counting starts with 1st line of input file (not with 1st
   line of 1st page printed). */
ਸਥਿਰ ਪੂਰਨ ਸਤਰ_ਗਿਣਤੀ = 1;

/* (-n) True means counting of skipped lines starts with 1st line of
   input file. False means -N option is used in addition, counting of
   skipped lines not required. */
ਸਥਿਰ bool skip_ਗਿਣਤੀ = true;

/* (-N) Counting starts with start_line_number = NUMBER at 1st line of
   first page printed, usually not 1st page of input file. */
ਸਥਿਰ ਪੂਰਨ ਸ਼ੁਰੂ_ਸਤਰ_ਅੰਕ = 1;

/* (-n) Width in characters of a line number. */
ਸਥਿਰ ਪੂਰਨ chars_ਪ੍ਰਤੀ_ਸੰਖਿਆ = 5;

/* Used when widening the first column to accommodate numbers -- only
   needed when printing files in parallel.  Includes width of both the
   number and the number_separator. */
ਸਥਿਰ ਪੂਰਨ ਸੰਖਿਆ_ਚੌੜਾਈ;

/* Buffer sprintf uses to format a line number. */
ਸਥਿਰ ਅੱਖਰ *ਸੰਖਿਆ_buff;

/* (-v) True means nonprintable characters are printed as escape sequences.
   control-g becomes \007. */
ਸਥਿਰ bool use_esc_sequence = false;

/* (-c) True means nonprintable characters are printed as control prefixes.
   control-g becomes ^G. */
ਸਥਿਰ bool use_cntrl_ਅਗੇਤਰ = false;

/* (-d) True means output is double spaced. */
ਸਥਿਰ bool double_space = false;

/* Number of files opened initially in init_files.  Should be 1
   unless we're printing multiple files in parallel. */
ਸਥਿਰ ਪੂਰਨ ਕੁੱਲ_files = 0;

/* (-r) True means don't complain if we can't open a file. */
ਸਥਿਰ bool ignore_failed_opens = false;

/* (-S) True means we separate columns with a specified string.
   -S option does not affect line truncation nor column alignment. */
ਸਥਿਰ bool use_ਖਾਨਾ_separator = false;

/* String used to separate columns if the -S option has been specified.
   Default without -S but together with one of the column options
   -a|COLUMN|-m is a 'space' and with the -J option a 'tab'. */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ਖਾਨਾ_sep_string = "";
ਸਥਿਰ ਪੂਰਨ ਖਾਨਾ_sep_ਲੰਮਾਈ = 0;
ਸਥਿਰ ਅੱਖਰ *ਕਾਲਮ_separator = (ਅੱਖਰ *) " ";
ਸਥਿਰ ਅੱਖਰ *ਸਤਰ_separator = (ਅੱਖਰ *) "\t";

/* Number of separator characters waiting to be printed as soon as we
   know that we have any input remaining to be printed. */
ਸਥਿਰ ਪੂਰਨ separators_not_printed;

/* Position we need to pad to, as soon as we know that we have input
   remaining to be printed. */
ਸਥਿਰ ਪੂਰਨ padding_not_printed;

/* True means we should pad the end of the page.  Remains false until we
   know we have a page to print. */
ਸਥਿਰ bool pad_vertically;

/* (-h) String of characters used in place of the filename in the header. */
ਸਥਿਰ ਅੱਖਰ *custom_ਸਿਰਲੇਖ;

/* (-D) Date format for the header.  */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *date_format;

/* The local time zone rules, as per the TZ environment variable.  */
ਸਥਿਰ timezone_t localtz;

/* Date and file name for the header.  */
ਸਥਿਰ ਅੱਖਰ *date_ਪਾਠ;
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *file_ਪਾਠ;

/* Output columns available, not counting the date and file name.  */
ਸਥਿਰ ਪੂਰਨ ਸਿਰਲੇਖ_ਚੌੜਾਈ_ਉਪਲਬਧ;

ਸਥਿਰ ਅੱਖਰ *clump_buff;

/* True means we read the line no. lines_per_body in skip_read
   called by skip_to_page. That variable controls the coincidence of a
   "FF set by hand" and "full_page_printed", see above the definition of
   structure COLUMN. */
ਸਥਿਰ bool ਆਖਰੀ_ਸਤਰ = false;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ਗਿਣਤੀ
{
  COLUMNS_OPTION = CHAR_MAX + 1,
  ਪੰਨੇ_OPTION
};

ਸਥਿਰ ਅੱਖਰ ਅਚਲ short_options[] =
  "-0123456789D:FJN:S::TW:abcde::fh:i::l:mn::o:rs::tvw:";

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ long_options[] =
{
  {"pages", required_ਦਲੀਲ, ਨਿਰਾ, ਪੰਨੇ_OPTION},
  {"columns", required_ਦਲੀਲ, ਨਿਰਾ, COLUMNS_OPTION},
  {"across", no_ਦਲੀਲ, ਨਿਰਾ, 'a'},
  {"show-control-chars", no_ਦਲੀਲ, ਨਿਰਾ, 'c'},
  {"double-space", no_ਦਲੀਲ, ਨਿਰਾ, 'd'},
  {"date-format", required_ਦਲੀਲ, ਨਿਰਾ, 'D'},
  {"expand-tabs", optional_ਦਲੀਲ, ਨਿਰਾ, 'e'},
  {"form-feed", no_ਦਲੀਲ, ਨਿਰਾ, 'f'},
  {"header", required_ਦਲੀਲ, ਨਿਰਾ, 'h'},
  {"output-tabs", optional_ਦਲੀਲ, ਨਿਰਾ, 'i'},
  {"join-lines", no_ਦਲੀਲ, ਨਿਰਾ, 'J'},
  {"length", required_ਦਲੀਲ, ਨਿਰਾ, 'l'},
  {"merge", no_ਦਲੀਲ, ਨਿਰਾ, 'm'},
  {"number-lines", optional_ਦਲੀਲ, ਨਿਰਾ, 'n'},
  {"first-line-number", required_ਦਲੀਲ, ਨਿਰਾ, 'N'},
  {"indent", required_ਦਲੀਲ, ਨਿਰਾ, 'o'},
  {"no-file-warnings", no_ਦਲੀਲ, ਨਿਰਾ, 'r'},
  {"separator", optional_ਦਲੀਲ, ਨਿਰਾ, 's'},
  {"sep-string", optional_ਦਲੀਲ, ਨਿਰਾ, 'S'},
  {"omit-header", no_ਦਲੀਲ, ਨਿਰਾ, 't'},
  {"omit-pagination", no_ਦਲੀਲ, ਨਿਰਾ, 'T'},
  {"show-nonprinting", no_ਦਲੀਲ, ਨਿਰਾ, 'v'},
  {"width", required_ਦਲੀਲ, ਨਿਰਾ, 'w'},
  {"page-width", required_ਦਲੀਲ, ਨਿਰਾ, 'W'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};

ਸਥਿਰ _Noreturn ਖਾਲੀ
integer_ਉਛਾਲ (ਖਾਲੀ)
{
  error (EXIT_FAILURE, 0, _("integer overflow"));
}

/* Return the number of columns that have either an open file or
   stored lines. */

ATTRIBUTE_ਸ਼ੁੱਧ
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
ਖਾਨੇ_ਗਿਣਤੀ_ready_ਮੰਜ਼ਿਲ_print (ਖਾਲੀ)
{
  ਕਾਲਮ_3 *q;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ n;

  n = 0;
  ਲਈ (q = ਕਾਲਮ_ਸਦਿਸ਼, i = 0; i < columns; ++q, ++i)
    ਜੇ (q->ਸਥਿਤੀ == OPEN
        || q->ਸਥਿਤੀ == FF_ਮਿਲਿਆ	/* With -b: To print a header only */
        || (storing_columns && q->ਸਤਰਾਂ_stored > 0 && q->ਸਤਰਾਂ_ਮੰਜ਼ਿਲ_print > 0))
      ++n;
  ਵਾਪਸ n;
}

/* Estimate first_ / last_page_number
   using option +FIRST_PAGE:LAST_PAGE */

ਸਥਿਰ bool
ਪਹਿਲਾ_ਆਖਰੀ_ਪੰਨਾ (ਪੂਰਨ oi, ਅੱਖਰ c, ਅੱਖਰ ਅਚਲ *ਪੰਨੇ)
{
  ਅੱਖਰ *p;
  uintmax_t ਪਹਿਲਾ;
  uintmax_t ਆਖਰੀ = UINTMAX_MAX;
  strtol_error err = xstrtoumax (ਪੰਨੇ, &p, 10, &ਪਹਿਲਾ, "");
  ਜੇ (err != LONGINT_OK && err != LONGINT_INVALID_SUFFIX_CHAR)
    xstrtol_fatal (err, oi, c, long_options, ਪੰਨੇ);

  ਜੇ (p == ਪੰਨੇ || !ਪਹਿਲਾ)
    ਵਾਪਸ false;

  ਜੇ (*p == ':')
    {
      ਅੱਖਰ ਅਚਲ *p1 = p + 1;
      err = xstrtoumax (p1, &p, 10, &ਆਖਰੀ, "");
      ਜੇ (err != LONGINT_OK)
        xstrtol_fatal (err, oi, c, long_options, ਪੰਨੇ);
      ਜੇ (p1 == p || ਆਖਰੀ < ਪਹਿਲਾ)
        ਵਾਪਸ false;
    }

  ਜੇ (*p)
    ਵਾਪਸ false;

  ਪਹਿਲਾ_ਪੰਨਾ_ਸੰਖਿਆ = ਪਹਿਲਾ;
  ਆਖਰੀ_ਪੰਨਾ_ਸੰਖਿਆ = ਆਖਰੀ;
  ਵਾਪਸ true;
}

/* Parse column count string S, and if it's valid (1 or larger and
   within range of the type of 'columns') set the global variables
   columns and explicit_columns.  Otherwise, exit with a diagnostic.  */

ਸਥਿਰ ਖਾਲੀ
parse_ਕਾਲਮ_ਗਿਣਤੀ (ਅੱਖਰ ਅਚਲ *s)
{
  columns = getoptnum (s, 1, _("invalid number of columns"));
  explicit_columns = true;
}

/* Estimate length of col_sep_string with option -S.  */

ਸਥਿਰ ਖਾਲੀ
separator_string (ਅੱਖਰ ਅਚਲ *optarg_S)
{
  size_t len = ਲੜੀ_ਲੰਬਾਈ (optarg_S);
  ਜੇ (ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ < len)
    integer_ਉਛਾਲ ();
  ਖਾਨਾ_sep_ਲੰਮਾਈ = len;
  ਖਾਨਾ_sep_string = optarg_S;
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ n_files;
  bool old_options = false;
  bool old_w = false;
  bool old_s = false;
  ਅੱਖਰ **file_ਨਾਂ;

  /* Accumulate the digits of old-style options like -99.  */
  ਅੱਖਰ *ਕਾਲਮ_ਗਿਣਤੀ_string = ਨਿਰਾ;
  idx_t n_digits = 0;
  idx_t n_alloc = 0;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  n_files = 0;
  file_ਨਾਂ = (argc > 1
                ? xnmalloc (argc - 1, ਆਕਾਰ (ਅੱਖਰ *))
                : ਨਿਰਾ);

  ਜਦੋਂ (true)
    {
      ਪੂਰਨ oi = -1;
      ਪੂਰਨ c = getopt_long (argc, argv, short_options, long_options, &oi);
      ਜੇ (c == -1)
        ਰੋਕੋ;

      ਜੇ (c_isdigit (c))
        {
          /* Accumulate column-count digits specified via old-style options. */
          ਜੇ (n_digits + 1 >= n_alloc)
            ਕਾਲਮ_ਗਿਣਤੀ_string = xpalloc (ਕਾਲਮ_ਗਿਣਤੀ_string, &n_alloc, 2, -1,
                                           ਆਕਾਰ *ਕਾਲਮ_ਗਿਣਤੀ_string);
          ਕਾਲਮ_ਗਿਣਤੀ_string[n_digits++] = c;
          ਕਾਲਮ_ਗਿਣਤੀ_string[n_digits] = '\0';
          ਜਾਰੀ;
        }

      n_digits = 0;

      ਚੋਣ (c)
        {
        ਹਾਲਤ 1:			/* Non-option argument. */
          /* long option --page dominates old '+FIRST_PAGE ...'.  */
          ਜੇ (! (ਪਹਿਲਾ_ਪੰਨਾ_ਸੰਖਿਆ == 0
                 && *optarg == '+' && ਪਹਿਲਾ_ਆਖਰੀ_ਪੰਨਾ (-2, '+', optarg + 1)))
            file_ਨਾਂ[n_files++] = optarg;
          ਰੋਕੋ;

        ਹਾਲਤ ਪੰਨੇ_OPTION:	/* --pages=FIRST_PAGE[:LAST_PAGE] */
          {			/* dominates old opt +... */
            ਜੇ (! optarg)
              error (EXIT_FAILURE, 0,
                     _("'--pages=FIRST_PAGE[:LAST_PAGE]' missing argument"));
            ਨਹੀਂਤਾਂ ਜੇ (! ਪਹਿਲਾ_ਆਖਰੀ_ਪੰਨਾ (oi, 0, optarg))
              error (EXIT_FAILURE, 0, _("invalid page range %s"),
                     quote (optarg));
            ਰੋਕੋ;
          }

        ਹਾਲਤ COLUMNS_OPTION:	/* --columns=COLS */
          {
            parse_ਕਾਲਮ_ਗਿਣਤੀ (optarg);

            /* If there was a prior column count specified via the
               short-named option syntax, e.g., -9, ensure that this
               long-name-specified value overrides it.  */
            ਮੁਕਤ (ਕਾਲਮ_ਗਿਣਤੀ_string);
            ਕਾਲਮ_ਗਿਣਤੀ_string = ਨਿਰਾ;
            n_alloc = 0;
            ਰੋਕੋ;
          }

        ਹਾਲਤ 'a':
          print_across_flag = true;
          storing_columns = false;
          ਰੋਕੋ;
        ਹਾਲਤ 'b':
          balance_columns = true;
          ਰੋਕੋ;
        ਹਾਲਤ 'c':
          use_cntrl_ਅਗੇਤਰ = true;
          ਰੋਕੋ;
        ਹਾਲਤ 'd':
          double_space = true;
          ਰੋਕੋ;
        ਹਾਲਤ 'D':
          date_format = optarg;
          ਰੋਕੋ;
        ਹਾਲਤ 'e':
          ਜੇ (optarg)
            getoptarg (optarg, 'e', &ਨਿਵੇਸ਼_tab_char,
                       &chars_ਪ੍ਰਤੀ_ਨਿਵੇਸ਼_tab);
          /* Could check tab width > 0. */
          untabify_ਨਿਵੇਸ਼ = true;
          ਰੋਕੋ;
        ਹਾਲਤ 'f':
        ਹਾਲਤ 'F':
          use_form_feed = true;
          ਰੋਕੋ;
        ਹਾਲਤ 'h':
          custom_ਸਿਰਲੇਖ = optarg;
          ਰੋਕੋ;
        ਹਾਲਤ 'i':
          ਜੇ (optarg)
            getoptarg (optarg, 'i', &output_tab_char,
                       &chars_ਪ੍ਰਤੀ_output_tab);
          /* Could check tab width > 0. */
          tabify_output = true;
          ਰੋਕੋ;
        ਹਾਲਤ 'J':
          join_ਸਤਰਾਂ = true;
          ਰੋਕੋ;
        ਹਾਲਤ 'l':
          ਸਤਰਾਂ_ਪ੍ਰਤੀ_ਪੰਨਾ
            = getoptnum (optarg, 1,
                         _("'-l PAGE_LENGTH' invalid number of lines"));
          ਰੋਕੋ;
        ਹਾਲਤ 'm':
          parallel_files = true;
          storing_columns = false;
          ਰੋਕੋ;
        ਹਾਲਤ 'n':
          numbered_ਸਤਰਾਂ = true;
          ਜੇ (optarg)
            getoptarg (optarg, 'n', &ਸੰਖਿਆ_separator,
                       &chars_ਪ੍ਰਤੀ_ਸੰਖਿਆ);
          ਰੋਕੋ;
        ਹਾਲਤ 'N':
          skip_ਗਿਣਤੀ = false;
          ਸ਼ੁਰੂ_ਸਤਰ_ਅੰਕ
            = getoptnum (optarg, INT_MIN,
                         _("'-N NUMBER' invalid starting line number"));
          ਰੋਕੋ;
        ਹਾਲਤ 'o':
          chars_ਪ੍ਰਤੀ_margin = getoptnum (optarg, 0,
                                        _("'-o MARGIN' invalid line offset"));
          ਰੋਕੋ;
        ਹਾਲਤ 'r':
          ignore_failed_opens = true;
          ਰੋਕੋ;
        ਹਾਲਤ 's':
          old_options = true;
          old_s = true;
          ਜੇ (!use_ਖਾਨਾ_separator && optarg)
            separator_string (optarg);
          ਰੋਕੋ;
        ਹਾਲਤ 'S':
          old_s = false;
          /* Reset an additional input of -s, -S dominates -s */
          ਖਾਨਾ_sep_string = "";
          ਖਾਨਾ_sep_ਲੰਮਾਈ = 0;
          use_ਖਾਨਾ_separator = true;
          ਜੇ (optarg)
            separator_string (optarg);
          ਰੋਕੋ;
        ਹਾਲਤ 't':
          extremities = false;
          keep_FF = true;
          ਰੋਕੋ;
        ਹਾਲਤ 'T':
          extremities = false;
          keep_FF = false;
          ਰੋਕੋ;
        ਹਾਲਤ 'v':
          use_esc_sequence = true;
          ਰੋਕੋ;
        ਹਾਲਤ 'w':
          old_options = true;
          old_w = true;
          {
            ਪੂਰਨ tmp_cpl
              = getoptnum (optarg, 1,
                           _("'-w PAGE_WIDTH' invalid number of characters"));
            ਜੇ (! truncate_ਸਤਰਾਂ)
              chars_ਪ੍ਰਤੀ_ਸਤਰ = tmp_cpl;
          }
          ਰੋਕੋ;
        ਹਾਲਤ 'W':
          old_w = false;			/* dominates -w */
          truncate_ਸਤਰਾਂ = true;
          chars_ਪ੍ਰਤੀ_ਸਤਰ
            = getoptnum (optarg, 1,
                         _("'-W PAGE_WIDTH' invalid number of characters"));
          ਰੋਕੋ;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);
        ਮੂਲ:
          usage (EXIT_FAILURE);
          ਰੋਕੋ;
        }
    }

  ਜੇ (ਕਾਲਮ_ਗਿਣਤੀ_string)
    {
      parse_ਕਾਲਮ_ਗਿਣਤੀ (ਕਾਲਮ_ਗਿਣਤੀ_string);
      ਮੁਕਤ (ਕਾਲਮ_ਗਿਣਤੀ_string);
    }

  ਜੇ (! date_format)
    date_format = (getenv ("POSIXLY_CORRECT") && !hard_locale (LC_ਸਮਾਂ)
                   ? "%b %e %H:%M %Y"
                   : "%Y-%m-%d %H:%M");

  localtz = tzalloc (getenv ("TZ"));

  /* Now we can set a reasonable initial value: */
  ਜੇ (ਪਹਿਲਾ_ਪੰਨਾ_ਸੰਖਿਆ == 0)
    ਪਹਿਲਾ_ਪੰਨਾ_ਸੰਖਿਆ = 1;

  ਜੇ (parallel_files && explicit_columns)
    error (EXIT_FAILURE, 0,
           _("cannot specify number of columns when printing in parallel"));

  ਜੇ (parallel_files && print_across_flag)
    error (EXIT_FAILURE, 0,
           _("cannot specify both printing across and printing in parallel"));

/* Translate some old short options to new/long options.
   To meet downward compatibility with other UNIX pr utilities
   and some POSIX specifications. */

  ਜੇ (old_options)
    {
      ਜੇ (old_w)
        {
          ਜੇ (parallel_files || explicit_columns)
            {
              /* activate -W */
              truncate_ਸਤਰਾਂ = true;
              ਜੇ (old_s)
                /* adapt HP-UX and SunOS: -s = no separator;
                   activate -S */
                use_ਖਾਨਾ_separator = true;
            }
          ਨਹੀਂਤਾਂ
            /* old -w sets width with columns only
               activate -J */
            join_ਸਤਰਾਂ = true;
        }
      ਨਹੀਂਤਾਂ ਜੇ (!use_ਖਾਨਾ_separator)
        {
          /* No -S option read */
          ਜੇ (old_s && (parallel_files || explicit_columns))
            {
              ਜੇ (!truncate_ਸਤਰਾਂ)
                {
                  /* old -s (without -w and -W) annuls column alignment,
                  uses fields, activate -J */
                  join_ਸਤਰਾਂ = true;
                  ਜੇ (ਖਾਨਾ_sep_ਲੰਮਾਈ > 0)
                    /* activate -S */
                    use_ਖਾਨਾ_separator = true;
                }
              ਨਹੀਂਤਾਂ
                /* with -W */
                /* adapt HP-UX and SunOS: -s = no separator;
                   activate -S */
                use_ਖਾਨਾ_separator = true;
            }
        }
    }

  ਲਈ (; optind < argc; optind++)
    {
      file_ਨਾਂ[n_files++] = argv[optind];
    }

  ਜੇ (n_files == 0)
    {
      /* No file arguments specified;  read from standard input.  */
      print_files (0, ਨਿਰਾ);
    }
  ਨਹੀਂਤਾਂ
    {
      ਜੇ (parallel_files)
        print_files (n_files, file_ਨਾਂ);
      ਨਹੀਂਤਾਂ
        {
          ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < n_files; i++)
            print_files (1, &file_ਨਾਂ[i]);
        }
    }

  cleanup ();

  ਜੇ (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, _("standard input"));
  main_exit (failed_opens ? EXIT_FAILURE : EXIT_SUCCESS);
}

/* Parse numeric arguments, ensuring MIN <= number <= INT_MAX.  */

ਸਥਿਰ ਪੂਰਨ
getoptnum (ਅੱਖਰ ਅਚਲ *n_str, ਪੂਰਨ min, ਅੱਖਰ ਅਚਲ *err)
{
  ਵਾਪਸ xnumtoimax (n_str, 10, min, ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ, "", err, 0,
                     min <= 0 ? 0 : XTOINT_MIN_RANGE);
}

/* Parse options of the form -scNNN.

   Example: -nck, where 'n' is the option, c is the optional number
   separator, and k is the optional width of the field used when printing
   a number. */

ਸਥਿਰ ਖਾਲੀ
getoptarg (ਅੱਖਰ *arg, ਅੱਖਰ switch_char, ਅੱਖਰ *character, ਪੂਰਨ *ਸੰਖਿਆ)
{
  ਅੱਖਰ ਅਚਲ *ਅੰਕ = arg;

  ਜੇ (!*ਅੰਕ)
    {
      error (0, 0, _("'-%c': Invalid argument: %s"), switch_char, quote (ਅੰਕ));
      usage (EXIT_FAILURE);
    }

  ਜੇ (!c_isdigit (*ਅੰਕ))
    *character = *ਅੰਕ++;
  ਜੇ (*ਅੰਕ)
    {
      ਲੰਮਾ ਪੂਰਨ tmp_long;
      strtol_error e = xstrtol (ਅੰਕ, ਨਿਰਾ, 10, &tmp_long, "");
      ਜੇ (e == LONGINT_OK)
        {
          ਜੇ (tmp_long <= 0)
            e = LONGINT_INVALID;
          ਨਹੀਂਤਾਂ ਜੇ (ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ < tmp_long)
            e = LONGINT_ਉਛਾਲ;
        }
      ਜੇ (e != LONGINT_OK)
        {
          error (0, e & LONGINT_ਉਛਾਲ ? EOVERFLOW : 0,
             _("'-%c' extra characters or invalid number in the argument: %s"),
                 switch_char, quote (arg));
          usage (EXIT_FAILURE);
        }
      *ਸੰਖਿਆ = tmp_long;
    }
}

/* Set parameters related to formatting. */

ਸਥਿਰ ਖਾਲੀ
init_parameters (ਪੂਰਨ ਸੰਖਿਆ_ਦਾ_files)
{
  ਪੂਰਨ chars_ਵਰਤਿਆ_by_ਸੰਖਿਆ = 0;

  ਸਤਰਾਂ_ਪ੍ਰਤੀ_body = ਸਤਰਾਂ_ਪ੍ਰਤੀ_ਪੰਨਾ - ਸਤਰਾਂ_ਪ੍ਰਤੀ_ਸਿਰਲੇਖ - ਸਤਰਾਂ_ਪ੍ਰਤੀ_footer;
  ਜੇ (ਸਤਰਾਂ_ਪ੍ਰਤੀ_body <= 0)
    {
      extremities = false;
      keep_FF = true;
    }
  ਜੇ (! extremities)
    ਸਤਰਾਂ_ਪ੍ਰਤੀ_body = ਸਤਰਾਂ_ਪ੍ਰਤੀ_ਪੰਨਾ;

  ਜੇ (double_space)
    ਸਤਰਾਂ_ਪ੍ਰਤੀ_body = MAX (1, ਸਤਰਾਂ_ਪ੍ਰਤੀ_body / 2);

  /* If input is stdin, cannot print parallel files.  BSD dumps core
     on this. */
  ਜੇ (ਸੰਖਿਆ_ਦਾ_files == 0)
    parallel_files = false;

  ਜੇ (parallel_files)
    columns = ਸੰਖਿਆ_ਦਾ_files;

  /* One file, multi columns down: -b option is set to get a consistent
     formulation with "FF set by hand" in input files. */
  ਜੇ (storing_columns)
    balance_columns = true;

  /* Tabification is assumed for multiple columns. */
  ਜੇ (columns > 1)
    {
      ਜੇ (!use_ਖਾਨਾ_separator)
        {
          /* Use default separator */
          ਜੇ (join_ਸਤਰਾਂ)
            ਖਾਨਾ_sep_string = ਸਤਰ_separator;
          ਨਹੀਂਤਾਂ
            ਖਾਨਾ_sep_string = ਕਾਲਮ_separator;

          ਖਾਨਾ_sep_ਲੰਮਾਈ = 1;
          use_ਖਾਨਾ_separator = true;
        }
      /* It's rather pointless to define a TAB separator with column
         alignment */
      ਨਹੀਂਤਾਂ ਜੇ (!join_ਸਤਰਾਂ && ਖਾਨਾ_sep_ਲੰਮਾਈ == 1 && *ਖਾਨਾ_sep_string == '\t')
        ਖਾਨਾ_sep_string = ਕਾਲਮ_separator;

      truncate_ਸਤਰਾਂ = true;
      ਜੇ (! (ਖਾਨਾ_sep_ਲੰਮਾਈ == 1 && *ਖਾਨਾ_sep_string == '\t'))
        untabify_ਨਿਵੇਸ਼ = true;
      tabify_output = true;
    }
  ਨਹੀਂਤਾਂ
    storing_columns = false;

  /* -J dominates -w in any case */
  ਜੇ (join_ਸਤਰਾਂ)
    truncate_ਸਤਰਾਂ = false;

  ਜੇ (numbered_ਸਤਰਾਂ)
    {
      ਪੂਰਨ chars_ਪ੍ਰਤੀ_default_tab = 8;

      ਸਤਰ_ਗਿਣਤੀ = ਸ਼ੁਰੂ_ਸਤਰ_ਅੰਕ;

      /* To allow input tab-expansion (-e sensitive) use:
         if (number_separator == input_tab_char)
           number_width = chars_per_number
             + TAB_WIDTH (chars_per_input_tab, chars_per_number);   */

      /* Estimate chars_per_text without any margin and keep it constant. */
      ਜੇ (ਸੰਖਿਆ_separator == '\t')
        ਸੰਖਿਆ_ਚੌੜਾਈ = (chars_ਪ੍ਰਤੀ_ਸੰਖਿਆ
                        + TAB_ਚੌੜਾਈ (chars_ਪ੍ਰਤੀ_default_tab, chars_ਪ੍ਰਤੀ_ਸੰਖਿਆ));
      ਨਹੀਂਤਾਂ
        ਸੰਖਿਆ_ਚੌੜਾਈ = chars_ਪ੍ਰਤੀ_ਸੰਖਿਆ + 1;

      /* The number is part of the column width unless we are
         printing files in parallel. */
      ਜੇ (parallel_files)
        chars_ਵਰਤਿਆ_by_ਸੰਖਿਆ = ਸੰਖਿਆ_ਚੌੜਾਈ;
    }

  ਪੂਰਨ sep_chars, useful_chars;
  ਜੇ (ckd_mul (&sep_chars, columns - 1, ਖਾਨਾ_sep_ਲੰਮਾਈ))
    sep_chars = ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ;
  ਜੇ (ckd_sub (&useful_chars, chars_ਪ੍ਰਤੀ_ਸਤਰ - chars_ਵਰਤਿਆ_by_ਸੰਖਿਆ,
               sep_chars))
    useful_chars = 0;
  chars_ਪ੍ਰਤੀ_ਕਾਲਮ = useful_chars / columns;

  ਜੇ (chars_ਪ੍ਰਤੀ_ਕਾਲਮ < 1)
    error (EXIT_FAILURE, 0, _("page width too narrow"));

  ਜੇ (numbered_ਸਤਰਾਂ)
    {
      ਮੁਕਤ (ਸੰਖਿਆ_buff);
      ਸੰਖਿਆ_buff = xmalloc (MAX (chars_ਪ੍ਰਤੀ_ਸੰਖਿਆ,
                                  INT_STRLEN_BOUND (ਸਤਰ_ਸੰਖਿਆ)) + 1);
    }

  /* Pick the maximum between the tab width and the width of an
     escape sequence.
     The width of an escape sequence (4) isn't the lower limit any longer.
     We've to use 8 as the lower limit, if we use chars_per_default_tab = 8
     to expand a tab which is not an input_tab-char. */
  ਮੁਕਤ (clump_buff);
  clump_buff = xmalloc (MAX (8, chars_ਪ੍ਰਤੀ_ਨਿਵੇਸ਼_tab));
}

/* Open the necessary files,
   maintaining a COLUMN structure for each column.

   With multiple files, each column p has a different p->fp.
   With single files, each column p has the same p->fp.
   Return false if (number_of_files > 0) and no files can be opened,
   true otherwise.

   With each column/file p, p->full_page_printed is initialized,
   see also open_file.  */

ਸਥਿਰ bool
init_fps (ਪੂਰਨ ਸੰਖਿਆ_ਦਾ_files, ਅੱਖਰ **av)
{
  ਕਾਲਮ_3 *p;

  ਕੁੱਲ_files = 0;

  ਮੁਕਤ (ਕਾਲਮ_ਸਦਿਸ਼);
  ਕਾਲਮ_ਸਦਿਸ਼ = xnmalloc (columns, ਆਕਾਰ (ਕਾਲਮ_3));

  ਜੇ (parallel_files)
    {
      ਪੂਰਨ files_ਖੱਬਾ = ਸੰਖਿਆ_ਦਾ_files;
      ਲਈ (p = ਕਾਲਮ_ਸਦਿਸ਼; files_ਖੱਬਾ--; ++p, ++av)
        {
          ਜੇ (! open_file (*av, p))
            {
              --p;
              --columns;
            }
        }
      ਜੇ (columns == 0)
        ਵਾਪਸ false;
      init_ਸਿਰਲੇਖ ("", -1);
    }
  ਨਹੀਂਤਾਂ
    {
      p = ਕਾਲਮ_ਸਦਿਸ਼;
      ਜੇ (ਸੰਖਿਆ_ਦਾ_files > 0)
        {
          ਜੇ (! open_file (*av, p))
            ਵਾਪਸ false;
          init_ਸਿਰਲੇਖ (*av, fileno (p->fp));
          p->ਸਤਰਾਂ_stored = 0;
        }
      ਨਹੀਂਤਾਂ
        {
          p->name = _("standard input");
          p->fp = stdin;
          have_read_stdin = true;
          p->ਸਥਿਤੀ = OPEN;
          p->ਭਰਿਆ_ਪੰਨਾ_printed = false;
          ++ਕੁੱਲ_files;
          init_ਸਿਰਲੇਖ ("", -1);
          p->ਸਤਰਾਂ_stored = 0;
        }

      ਅੱਖਰ ਅਚਲ *firstname = p->name;
      FILE *firstfp = p->fp;
      ਪੂਰਨ i;
      ਲਈ (i = columns - 1, ++p; i; --i, ++p)
        {
          p->name = firstname;
          p->fp = firstfp;
          p->ਸਥਿਤੀ = OPEN;
          p->ਭਰਿਆ_ਪੰਨਾ_printed = false;
          p->ਸਤਰਾਂ_stored = 0;
        }
    }
  files_ready_ਮੰਜ਼ਿਲ_read = ਕੁੱਲ_files;
  ਵਾਪਸ true;
}

/* Determine print_func and char_func, the functions
   used by each column for printing and/or storing.

   Determine the horizontal position desired when we begin
   printing a column (p->start_position). */

ਸਥਿਰ ਖਾਲੀ
init_funcs (ਖਾਲੀ)
{
  ਪੂਰਨ i, h, h_ਅਗਲਾ;
  ਕਾਲਮ_3 *p;

  h = chars_ਪ੍ਰਤੀ_margin;

  ਜੇ (!truncate_ਸਤਰਾਂ)
    h_ਅਗਲਾ = ANYWHERE;
  ਨਹੀਂਤਾਂ
    {
      /* When numbering lines of parallel files, we enlarge the
         first column to accommodate the number.  Looks better than
         the Sys V approach. */
      ਜੇ (parallel_files && numbered_ਸਤਰਾਂ)
        h_ਅਗਲਾ = h + chars_ਪ੍ਰਤੀ_ਕਾਲਮ + ਸੰਖਿਆ_ਚੌੜਾਈ;
      ਨਹੀਂਤਾਂ
        h_ਅਗਲਾ = h + chars_ਪ੍ਰਤੀ_ਕਾਲਮ;
    }

  /* Enlarge p->start_position of first column to use the same form of
     padding_not_printed with all columns. */
  h = h + ਖਾਨਾ_sep_ਲੰਮਾਈ;

  /* This loop takes care of all but the rightmost column. */

  ਲਈ (p = ਕਾਲਮ_ਸਦਿਸ਼, i = 1; i < columns; ++p, ++i)
    {
      ਜੇ (storing_columns)	/* One file, multi columns down. */
        {
          p->char_func = store_char;
          p->print_func = print_stored;
        }
      ਨਹੀਂਤਾਂ
        /* One file, multi columns across; or parallel files.  */
        {
          p->char_func = print_char;
          p->print_func = read_ਸਤਰ;
        }

      /* Number only the first column when printing files in
         parallel. */
      p->numbered = numbered_ਸਤਰਾਂ && (!parallel_files || i == 1);
      p->ਸ਼ੁਰੂ_position = h;

      /* If we don't truncate lines, all start_positions are
         ANYWHERE, except the first column's start_position when
         using a margin. */

      ਜੇ (!truncate_ਸਤਰਾਂ)
        {
          h = ANYWHERE;
          h_ਅਗਲਾ = ANYWHERE;
        }
      ਨਹੀਂਤਾਂ
        {
          h = h_ਅਗਲਾ + ਖਾਨਾ_sep_ਲੰਮਾਈ;
          h_ਅਗਲਾ = h + chars_ਪ੍ਰਤੀ_ਕਾਲਮ;
        }
    }

  /* The rightmost column.

     Doesn't need to be stored unless we intend to balance
     columns on the last page. */
  ਜੇ (storing_columns && balance_columns)
    {
      p->char_func = store_char;
      p->print_func = print_stored;
    }
  ਨਹੀਂਤਾਂ
    {
      p->char_func = print_char;
      p->print_func = read_ਸਤਰ;
    }

  p->numbered = numbered_ਸਤਰਾਂ && (!parallel_files || i == 1);
  p->ਸ਼ੁਰੂ_position = h;
}

/* Open a file.  Return true if successful.

   With each file p, p->full_page_printed is initialized,
   see also init_fps. */

ਸਥਿਰ bool
open_file (ਅੱਖਰ *name, ਕਾਲਮ_3 *p)
{
  ਜੇ (streq (name, "-"))
    {
      p->name = _("standard input");
      p->fp = stdin;
      have_read_stdin = true;
    }
  ਨਹੀਂਤਾਂ
    {
      p->name = name;
      p->fp = fopen (name, "r");
    }
  ਜੇ (p->fp == ਨਿਰਾ)
    {
      failed_opens = true;
      ਜੇ (!ignore_failed_opens)
        error (0, errno, "%s", quotef (name));
      ਵਾਪਸ false;
    }
  fadvise (p->fp, FADVISE_SEQUENTIAL);
  p->ਸਥਿਤੀ = OPEN;
  p->ਭਰਿਆ_ਪੰਨਾ_printed = false;
  ++ਕੁੱਲ_files;
  ਵਾਪਸ true;
}

/* Close the file in P.

   If we aren't dealing with multiple files in parallel, we change
   the status of all columns in the column list to reflect the close. */

ਸਥਿਰ ਖਾਲੀ
close_file (ਕਾਲਮ_3 *p)
{
  ਕਾਲਮ_3 *q;
  ਪੂਰਨ i;

  ਜੇ (p->ਸਥਿਤੀ == CLOSED)
    ਵਾਪਸ;

  ਪੂਰਨ err = errno;
  ਜੇ (!ferror (p->fp))
    err = 0;
  ਜੇ (fileno (p->fp) == STDIN_FILENO)
    clearerr (p->fp);
  ਨਹੀਂਤਾਂ ਜੇ (fclose (p->fp) != 0 && !err)
    err = errno;
  ਜੇ (err)
    error (EXIT_FAILURE, err, "%s", quotef (p->name));

  ਜੇ (!parallel_files)
    {
      ਲਈ (q = ਕਾਲਮ_ਸਦਿਸ਼, i = columns; i; ++q, --i)
        {
          q->ਸਥਿਤੀ = CLOSED;
          ਜੇ (q->ਸਤਰਾਂ_stored == 0)
            {
              q->ਸਤਰਾਂ_ਮੰਜ਼ਿਲ_print = 0;
            }
        }
    }
  ਨਹੀਂਤਾਂ
    {
      p->ਸਥਿਤੀ = CLOSED;
      p->ਸਤਰਾਂ_ਮੰਜ਼ਿਲ_print = 0;
    }

  --files_ready_ਮੰਜ਼ਿਲ_read;
}

/* Put a file on hold until we start a new page,
   since we've hit a form feed.

   If we aren't dealing with parallel files, we must change the
   status of all columns in the column list. */

ਸਥਿਰ ਖਾਲੀ
hold_file (ਕਾਲਮ_3 *p)
{
  ਕਾਲਮ_3 *q;
  ਪੂਰਨ i;

  ਜੇ (!parallel_files)
    ਲਈ (q = ਕਾਲਮ_ਸਦਿਸ਼, i = columns; i; ++q, --i)
      {
        ਜੇ (storing_columns)
          q->ਸਥਿਤੀ = FF_ਮਿਲਿਆ;
        ਨਹੀਂਤਾਂ
          q->ਸਥਿਤੀ = ON_HOLD;
      }
  ਨਹੀਂਤਾਂ
    p->ਸਥਿਤੀ = ON_HOLD;

  p->ਸਤਰਾਂ_ਮੰਜ਼ਿਲ_print = 0;
  --files_ready_ਮੰਜ਼ਿਲ_read;
}

/* Undo hold_file -- go through the column list and change any
   ON_HOLD columns to OPEN.  Used at the end of each page. */

ਸਥਿਰ ਖਾਲੀ
reset_ਸਥਿਤੀ (ਖਾਲੀ)
{
  ਪੂਰਨ i = columns;
  ਕਾਲਮ_3 *p;

  ਲਈ (p = ਕਾਲਮ_ਸਦਿਸ਼; i; --i, ++p)
    ਜੇ (p->ਸਥਿਤੀ == ON_HOLD)
      {
        p->ਸਥਿਤੀ = OPEN;
        files_ready_ਮੰਜ਼ਿਲ_read++;
      }

  ਜੇ (storing_columns)
    {
      ਜੇ (ਕਾਲਮ_ਸਦਿਸ਼->ਸਥਿਤੀ == CLOSED)
        /* We use the info to output an error message in  skip_to_page. */
        files_ready_ਮੰਜ਼ਿਲ_read = 0;
      ਨਹੀਂਤਾਂ
        files_ready_ਮੰਜ਼ਿਲ_read = 1;
    }
}

/* Print a single file, or multiple files in parallel.

   Set up the list of columns, opening the necessary files.
   Allocate space for storing columns, if necessary.
   Skip to first_page_number, if user has asked to skip leading pages.
   Determine which functions are appropriate to store/print lines
   in each column.
   Print the file(s). */

ਸਥਿਰ ਖਾਲੀ
print_files (ਪੂਰਨ ਸੰਖਿਆ_ਦਾ_files, ਅੱਖਰ **av)
{
  init_parameters (ਸੰਖਿਆ_ਦਾ_files);
  ਜੇ (! init_fps (ਸੰਖਿਆ_ਦਾ_files, av))
    ਵਾਪਸ;
  ਜੇ (storing_columns)
    init_store_ਖਾਨੇ_ਗਿਣਤੀ ();

  ਜੇ (ਪਹਿਲਾ_ਪੰਨਾ_ਸੰਖਿਆ > 1)
    {
      ਜੇ (!skip_ਮੰਜ਼ਿਲ_ਪੰਨਾ (ਪਹਿਲਾ_ਪੰਨਾ_ਸੰਖਿਆ))
        ਵਾਪਸ;
      ਨਹੀਂਤਾਂ
        ਪੰਨਾ_ਸੰਖਿਆ = ਪਹਿਲਾ_ਪੰਨਾ_ਸੰਖਿਆ;
    }
  ਨਹੀਂਤਾਂ
    ਪੰਨਾ_ਸੰਖਿਆ = 1;

  init_funcs ();

  ਸਤਰ_ਸੰਖਿਆ = ਸਤਰ_ਗਿਣਤੀ;
  ਜਦੋਂ (print_ਪੰਨਾ ())
    ;
}

/* Initialize header information.
   If DESC is non-negative, it is a file descriptor open to
   FILENAME for reading.  */

ਸਥਿਰ ਖਾਲੀ
init_ਸਿਰਲੇਖ (ਅੱਖਰ ਅਚਲ *filename, ਪੂਰਨ desc)
{
  ਅੱਖਰ *buf = ਨਿਰਾ;
  ਬਣਤਰ stat st;
  ਬਣਤਰ timespec t;
  ਪੂਰਨ ns;
  ਬਣਤਰ tm tm;

  /* If parallel files or standard input, use current date. */
  ਜੇ (streq (filename, "-"))
    desc = -1;
  ਜੇ (0 <= desc && fstat (desc, &st) == 0)
    t = get_stat_mtime (&st);
  ਨਹੀਂਤਾਂ
    {
      ਸਥਿਰ ਬਣਤਰ timespec timespec;
      ਜੇ (! timespec.tv_sec)
        gettime (&timespec);
      t = timespec;
    }

  ns = t.tv_nsec;
  ਜੇ (localtime_rz (localtz, &t.tv_sec, &tm))
    {
      ptrdiff_t len = nstrftime (ਨਿਰਾ, MIN (PTRDIFF_MAX, SIZE_MAX),
                                 date_format, &tm, localtz, ns);
      ਜੇ (0 <= len)
        {
          buf = ximalloc (len + 1);
          nstrftime (buf, len + 1, date_format, &tm, localtz, ns);
        }
    }
  ਜੇ (!buf)
    {
      ਅੱਖਰ secbuf[INT_BUFSIZE_BOUND (intmax_t)];
      buf = xmalloc (ਆਕਾਰ secbuf + MAX (10, INT_BUFSIZE_BOUND (ਪੂਰਨ)));
      sprintf (buf, "%s.%09d", timetostr (t.tv_sec, secbuf), ns);
    }

  ਮੁਕਤ (date_ਪਾਠ);
  date_ਪਾਠ = buf;
  file_ਪਾਠ = custom_ਸਿਰਲੇਖ ? custom_ਸਿਰਲੇਖ : desc < 0 ? "" : filename;
  ਸਿਰਲੇਖ_ਚੌੜਾਈ_ਉਪਲਬਧ = (chars_ਪ੍ਰਤੀ_ਸਤਰ
                            - mbswidth (date_ਪਾਠ, 0)
                            - mbswidth (file_ਪਾਠ, 0));
}

/* Set things up for printing a page

   Scan through the columns ...
   Determine which are ready to print
   (i.e., which have lines stored or open files)
   Set p->lines_to_print appropriately
   (to p->lines_stored if we're storing, or lines_per_body
   if we're reading straight from the file)
   Keep track of this total so we know when to stop printing */

ਸਥਿਰ ਖਾਲੀ
init_ਪੰਨਾ (ਖਾਲੀ)
{
  ਪੂਰਨ j;
  ਕਾਲਮ_3 *p;

  ਜੇ (storing_columns)
    {
      store_columns ();
      ਲਈ (j = columns - 1, p = ਕਾਲਮ_ਸਦਿਸ਼; j; --j, ++p)
        {
          p->ਸਤਰਾਂ_ਮੰਜ਼ਿਲ_print = p->ਸਤਰਾਂ_stored;
        }

      /* Last column. */
      ਜੇ (balance_columns)
        {
          p->ਸਤਰਾਂ_ਮੰਜ਼ਿਲ_print = p->ਸਤਰਾਂ_stored;
        }
      /* Since we're not balancing columns, we don't need to store
         the rightmost column.   Read it straight from the file. */
      ਨਹੀਂਤਾਂ
        {
          ਜੇ (p->ਸਥਿਤੀ == OPEN)
            {
              p->ਸਤਰਾਂ_ਮੰਜ਼ਿਲ_print = ਸਤਰਾਂ_ਪ੍ਰਤੀ_body;
            }
          ਨਹੀਂਤਾਂ
            p->ਸਤਰਾਂ_ਮੰਜ਼ਿਲ_print = 0;
        }
    }
  ਨਹੀਂਤਾਂ
    ਲਈ (j = columns, p = ਕਾਲਮ_ਸਦਿਸ਼; j; --j, ++p)
      ਜੇ (p->ਸਥਿਤੀ == OPEN)
        {
          p->ਸਤਰਾਂ_ਮੰਜ਼ਿਲ_print = ਸਤਰਾਂ_ਪ੍ਰਤੀ_body;
        }
      ਨਹੀਂਤਾਂ
        p->ਸਤਰਾਂ_ਮੰਜ਼ਿਲ_print = 0;
}

/* Align empty columns and print separators.
   Empty columns will be formed by files with status ON_HOLD or CLOSED
   when printing multiple files in parallel. */

ਸਥਿਰ ਖਾਲੀ
align_ਕਾਲਮ (ਕਾਲਮ_3 *p)
{
  padding_not_printed = p->ਸ਼ੁਰੂ_position;
  ਜੇ (ਖਾਨਾ_sep_ਲੰਮਾਈ < padding_not_printed)
    {
      pad_across_ਮੰਜ਼ਿਲ (padding_not_printed - ਖਾਨਾ_sep_ਲੰਮਾਈ);
      padding_not_printed = ANYWHERE;
    }

  ਜੇ (use_ਖਾਨਾ_separator)
    print_sep_string ();

  ਜੇ (p->numbered)
    ਜੋੜੋ_ਸਤਰ_ਸੰਖਿਆ (p);
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

ਸਥਿਰ bool
print_ਪੰਨਾ (ਖਾਲੀ)
{
  ਪੂਰਨ j;
  ਪੂਰਨ ਸਤਰਾਂ_ਖੱਬਾ_on_ਪੰਨਾ;
  ਕਾਲਮ_3 *p;

  /* Used as an accumulator (with | operator) of successive values of
     pad_vertically.  The trick is to set pad_vertically
     to false before each run through the inner loop, then after that
     loop, it tells us whether a line was actually printed (whether a
     newline needs to be output -- or two for double spacing).  But those
     values have to be accumulated (in pv) so we can invoke pad_down
     properly after the outer loop completes. */
  bool pv;

  init_ਪੰਨਾ ();

  ਜੇ (ਖਾਨੇ_ਗਿਣਤੀ_ready_ਮੰਜ਼ਿਲ_print () == 0)
    ਵਾਪਸ false;

  ਜੇ (extremities)
    print_a_ਸਿਰਲੇਖ = true;

  /* Don't pad unless we know a page was printed. */
  pad_vertically = false;
  pv = false;

  ਸਤਰਾਂ_ਖੱਬਾ_on_ਪੰਨਾ = ਸਤਰਾਂ_ਪ੍ਰਤੀ_body;
  ਜੇ (double_space)
    ਸਤਰਾਂ_ਖੱਬਾ_on_ਪੰਨਾ *= 2;

  ਜਦੋਂ (ਸਤਰਾਂ_ਖੱਬਾ_on_ਪੰਨਾ > 0 && ਖਾਨੇ_ਗਿਣਤੀ_ready_ਮੰਜ਼ਿਲ_print () > 0)
    {
      output_position = 0;
      spaces_not_printed = 0;
      separators_not_printed = 0;
      pad_vertically = false;
      align_ਖਾਲੀ_ਥਾਵਾਂ_ਖਾਨੇ_ਗਿਣਤੀ = false;
      ਖਾਲੀ_ਥਾਵਾਂ_ਸਤਰ = true;

      ਲਈ (j = 1, p = ਕਾਲਮ_ਸਦਿਸ਼; j <= columns; ++j, ++p)
        {
          ਨਿਵੇਸ਼_position = 0;
          ਜੇ (p->ਸਤਰਾਂ_ਮੰਜ਼ਿਲ_print > 0 || p->ਸਥਿਤੀ == FF_ਮਿਲਿਆ)
            {
              FF_only = false;
              padding_not_printed = p->ਸ਼ੁਰੂ_position;
              ਜੇ (!(p->print_func) (p))
                read_rest_ਦਾ_ਸਤਰ (p);
              pv |= pad_vertically;

              --p->ਸਤਰਾਂ_ਮੰਜ਼ਿਲ_print;
              ਜੇ (p->ਸਤਰਾਂ_ਮੰਜ਼ਿਲ_print <= 0)
                {
                  ਜੇ (ਖਾਨੇ_ਗਿਣਤੀ_ready_ਮੰਜ਼ਿਲ_print () == 0)
                    ਰੋਕੋ;
                }

              /* File p changed its status to ON_HOLD or CLOSED */
              ਜੇ (parallel_files && p->ਸਥਿਤੀ != OPEN)
                {
                  ਜੇ (ਖਾਲੀ_ਥਾਵਾਂ_ਸਤਰ)
                    align_ਖਾਲੀ_ਥਾਵਾਂ_ਖਾਨੇ_ਗਿਣਤੀ = true;
                  ਨਹੀਂਤਾਂ ਜੇ (p->ਸਥਿਤੀ == CLOSED
                           || (p->ਸਥਿਤੀ == ON_HOLD && FF_only))
                    align_ਕਾਲਮ (p);
                }
            }
          ਨਹੀਂਤਾਂ ਜੇ (parallel_files)
            {
              /* File status ON_HOLD or CLOSED */
              ਜੇ (ਖਾਲੀ_ਥਾਵਾਂ_ਸਤਰ)
                align_ਖਾਲੀ_ਥਾਵਾਂ_ਖਾਨੇ_ਗਿਣਤੀ = true;
              ਨਹੀਂਤਾਂ
                align_ਕਾਲਮ (p);
            }

          /* We need it also with an empty column */
          ਜੇ (use_ਖਾਨਾ_separator)
            ++separators_not_printed;
        }

      ਜੇ (pad_vertically)
        {
          ਅੱਖਰ_ਲਿਖੋ ('\n');
          --ਸਤਰਾਂ_ਖੱਬਾ_on_ਪੰਨਾ;
        }

      ਜੇ (ਖਾਨੇ_ਗਿਣਤੀ_ready_ਮੰਜ਼ਿਲ_print () == 0 && !extremities)
        ਰੋਕੋ;

      ਜੇ (double_space && pv)
        {
          ਅੱਖਰ_ਲਿਖੋ ('\n');
          --ਸਤਰਾਂ_ਖੱਬਾ_on_ਪੰਨਾ;
        }
    }

  ਜੇ (ਸਤਰਾਂ_ਖੱਬਾ_on_ਪੰਨਾ == 0)
    ਲਈ (j = 1, p = ਕਾਲਮ_ਸਦਿਸ਼; j <= columns; ++j, ++p)
      ਜੇ (p->ਸਥਿਤੀ == OPEN)
        p->ਭਰਿਆ_ਪੰਨਾ_printed = true;

  pad_vertically = pv;

  ਜੇ (pad_vertically && extremities)
    pad_ਥੱਲੇ (ਸਤਰਾਂ_ਖੱਬਾ_on_ਪੰਨਾ + ਸਤਰਾਂ_ਪ੍ਰਤੀ_footer);
  ਨਹੀਂਤਾਂ ਜੇ (keep_FF && print_a_FF)
    {
      ਅੱਖਰ_ਲਿਖੋ ('\f');
      print_a_FF = false;
    }

  ਜੇ (ferror (stdout))
    write_error ();

  ਜੇ (ਆਖਰੀ_ਪੰਨਾ_ਸੰਖਿਆ < ++ਪੰਨਾ_ਸੰਖਿਆ)
    ਵਾਪਸ false;		/* Stop printing with LAST_PAGE */

  reset_ਸਥਿਤੀ ();		/* Change ON_HOLD to OPEN. */

  ਵਾਪਸ true;			/* More pages to go. */
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

ਸਥਿਰ ਖਾਲੀ
init_store_ਖਾਨੇ_ਗਿਣਤੀ (ਖਾਲੀ)
{
  /* Tune this.  */
  ਪੂਰਨ ਕੁੱਲ_ਸਤਰਾਂ, ਕੁੱਲ_ਸਤਰਾਂ_1, chars_ਪ੍ਰਤੀ_ਕਾਲਮ_1, chars_if_truncate;
  ਜੇ (ckd_mul (&ਕੁੱਲ_ਸਤਰਾਂ, ਸਤਰਾਂ_ਪ੍ਰਤੀ_body, columns)
      || ckd_ਜੋੜੋ (&ਕੁੱਲ_ਸਤਰਾਂ_1, ਕੁੱਲ_ਸਤਰਾਂ, 1)
      || ckd_ਜੋੜੋ (&chars_ਪ੍ਰਤੀ_ਕਾਲਮ_1, chars_ਪ੍ਰਤੀ_ਕਾਲਮ, 1)
      || ckd_mul (&chars_if_truncate, ਕੁੱਲ_ਸਤਰਾਂ, chars_ਪ੍ਰਤੀ_ਕਾਲਮ_1)
      || ckd_mul (&buff_allocated, chars_if_truncate, use_ਖਾਨਾ_separator + 1))
    integer_ਉਛਾਲ ();

  ਮੁਕਤ (ਸਤਰ_ਸਦਿਸ਼);
  /* FIXME: here's where it was allocated.  */
  ਸਤਰ_ਸਦਿਸ਼ = xnmalloc (ਕੁੱਲ_ਸਤਰਾਂ_1, ਆਕਾਰ *ਸਤਰ_ਸਦਿਸ਼);

  ਮੁਕਤ (ਸਿਰਾ_ਸਦਿਸ਼);
  ਸਿਰਾ_ਸਦਿਸ਼ = xnmalloc (ਕੁੱਲ_ਸਤਰਾਂ, ਆਕਾਰ *ਸਿਰਾ_ਸਦਿਸ਼);

  ਮੁਕਤ (buff);
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

ਸਥਿਰ ਖਾਲੀ
store_columns (ਖਾਲੀ)
{
  ਪੂਰਨ i, j;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਤਰ = 0;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ buff_ਸ਼ੁਰੂ;
  ਪੂਰਨ ਆਖਰੀ_ਖਾਨਾ;		/* The rightmost column which will be saved in buff */
  ਕਾਲਮ_3 *p;

  buff_current = 0;
  buff_ਸ਼ੁਰੂ = 0;

  ਜੇ (balance_columns)
    ਆਖਰੀ_ਖਾਨਾ = columns;
  ਨਹੀਂਤਾਂ
    ਆਖਰੀ_ਖਾਨਾ = columns - 1;

  ਲਈ (i = 1, p = ਕਾਲਮ_ਸਦਿਸ਼; i <= ਆਖਰੀ_ਖਾਨਾ; ++i, ++p)
    p->ਸਤਰਾਂ_stored = 0;

  ਲਈ (i = 1, p = ਕਾਲਮ_ਸਦਿਸ਼; i <= ਆਖਰੀ_ਖਾਨਾ && files_ready_ਮੰਜ਼ਿਲ_read;
       ++i, ++p)
    {
      p->current_ਸਤਰ = ਸਤਰ;
      ਲਈ (j = ਸਤਰਾਂ_ਪ੍ਰਤੀ_body; j && files_ready_ਮੰਜ਼ਿਲ_read; --j)

        ਜੇ (p->ਸਥਿਤੀ == OPEN)	/* Redundant.  Clean up. */
          {
            ਨਿਵੇਸ਼_position = 0;

            ਜੇ (!read_ਸਤਰ (p))
              read_rest_ਦਾ_ਸਤਰ (p);

            ਜੇ (p->ਸਥਿਤੀ == OPEN
                || buff_ਸ਼ੁਰੂ != buff_current)
              {
                ++p->ਸਤਰਾਂ_stored;
                ਸਤਰ_ਸਦਿਸ਼[ਸਤਰ] = buff_ਸ਼ੁਰੂ;
                ਸਿਰਾ_ਸਦਿਸ਼[ਸਤਰ++] = ਨਿਵੇਸ਼_position;
                buff_ਸ਼ੁਰੂ = buff_current;
              }
          }
    }

  /* Keep track of the location of the last char in buff. */
  ਸਤਰ_ਸਦਿਸ਼[ਸਤਰ] = buff_ਸ਼ੁਰੂ;

  ਜੇ (balance_columns)
    balance (ਸਤਰ);
}

ਸਥਿਰ ਖਾਲੀ
balance (ਪੂਰਨ ਕੁੱਲ_stored)
{
  ਕਾਲਮ_3 *p;
  ਪੂਰਨ i, ਸਤਰਾਂ;
  ਪੂਰਨ ਪਹਿਲਾ_ਸਤਰ = 0;

  ਲਈ (i = 1, p = ਕਾਲਮ_ਸਦਿਸ਼; i <= columns; ++i, ++p)
    {
      ਸਤਰਾਂ = ਕੁੱਲ_stored / columns;
      ਜੇ (i <= ਕੁੱਲ_stored % columns)
        ++ਸਤਰਾਂ;

      p->ਸਤਰਾਂ_stored = ਸਤਰਾਂ;
      p->current_ਸਤਰ = ਪਹਿਲਾ_ਸਤਰ;

      ਪਹਿਲਾ_ਸਤਰ += ਸਤਰਾਂ;
    }
}

/* Store a character in the buffer. */

ਸਥਿਰ ਖਾਲੀ
store_char (ਅੱਖਰ c)
{
  ਜੇ (buff_current >= buff_allocated)
    {
      /* May be too generous. */
      buff = xpalloc (buff, &buff_allocated, 1, -1, ਆਕਾਰ *buff);
    }
  buff[buff_current++] = c;
}

ਸਥਿਰ ਖਾਲੀ
ਜੋੜੋ_ਸਤਰ_ਸੰਖਿਆ (ਕਾਲਮ_3 *p)
{
  ਪੂਰਨ i;
  ਅੱਖਰ *s;
  ਪੂਰਨ ਅੰਕ_ਚੌੜਾਈ;

  /* Cutting off the higher-order digits is more informative than
     lower-order cut off. */
  ਅੰਕ_ਚੌੜਾਈ = sprintf (ਸੰਖਿਆ_buff, "%*d", chars_ਪ੍ਰਤੀ_ਸੰਖਿਆ, ਸਤਰ_ਸੰਖਿਆ);
  ਸਤਰ_ਸੰਖਿਆ++;
  s = ਸੰਖਿਆ_buff + (ਅੰਕ_ਚੌੜਾਈ - chars_ਪ੍ਰਤੀ_ਸੰਖਿਆ);
  ਲਈ (i = chars_ਪ੍ਰਤੀ_ਸੰਖਿਆ; i > 0; i--)
    (p->char_func) (*s++);

  ਜੇ (columns > 1)
    {
      /* Tabification is assumed for multiple columns, also for n-separators,
         but 'default n-separator = TAB' hasn't been given priority over
         equal column_width also specified by POSIX. */
      ਜੇ (ਸੰਖਿਆ_separator == '\t')
        {
          i = ਸੰਖਿਆ_ਚੌੜਾਈ - chars_ਪ੍ਰਤੀ_ਸੰਖਿਆ;
          ਜਦੋਂ (i-- > 0)
            (p->char_func) (' ');
        }
      ਨਹੀਂਤਾਂ
        (p->char_func) (ਸੰਖਿਆ_separator);
    }
  ਨਹੀਂਤਾਂ
    /* To comply with POSIX, we avoid any expansion of default TAB
       separator with a single column output. No column_width requirement
       has to be considered. */
    {
      (p->char_func) (ਸੰਖਿਆ_separator);
      ਜੇ (ਸੰਖਿਆ_separator == '\t')
        output_position = POS_AFTER_TAB (chars_ਪ੍ਰਤੀ_output_tab,
                          output_position);
    }

  ਜੇ (truncate_ਸਤਰਾਂ && !parallel_files)
    ਨਿਵੇਸ਼_position += ਸੰਖਿਆ_ਚੌੜਾਈ;
}

/* Print (or store) padding until the current horizontal position
   is position. */

ਸਥਿਰ ਖਾਲੀ
pad_across_ਮੰਜ਼ਿਲ (ਪੂਰਨ position)
{
  ਪੂਰਨ h = output_position;

  ਜੇ (tabify_output)
    spaces_not_printed = position - output_position;
  ਨਹੀਂਤਾਂ
    {
      ਜਦੋਂ (++h <= position)
        ਅੱਖਰ_ਲਿਖੋ (' ');
      output_position = position;
    }
}

/* Pad to the bottom of the page.

   If the user has requested a formfeed, use one.
   Otherwise, use newlines. */

ਸਥਿਰ ਖਾਲੀ
pad_ਥੱਲੇ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਤਰਾਂ)
{
  ਜੇ (use_form_feed)
    ਅੱਖਰ_ਲਿਖੋ ('\f');
  ਨਹੀਂਤਾਂ
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਸਤਰਾਂ; i; --i)
      ਅੱਖਰ_ਲਿਖੋ ('\n');
}

/* Read the rest of the line.

   Read from the current column's file until an end of line is
   hit.  Used when we've truncated a line and we no longer need
   to print or store its characters. */

ਸਥਿਰ ਖਾਲੀ
read_rest_ਦਾ_ਸਤਰ (ਕਾਲਮ_3 *p)
{
  ਪੂਰਨ c;
  FILE *f = p->fp;

  ਜਦੋਂ ((c = getc (f)) != '\n')
    {
      ਜੇ (c == '\f')
        {
          ਜੇ ((c = getc (f)) != '\n')
            ungetc (c, f);
          ਜੇ (keep_FF)
            print_a_FF = true;
          hold_file (p);
          ਰੋਕੋ;
        }
      ਨਹੀਂਤਾਂ ਜੇ (c == EOF)
        {
          close_file (p);
          ਰੋਕੋ;
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

ਸਥਿਰ ਖਾਲੀ
skip_read (ਕਾਲਮ_3 *p, ਪੂਰਨ ਕਾਲਮ_ਸੰਖਿਆ)
{
  ਪੂਰਨ c;
  FILE *f = p->fp;
  ਪੂਰਨ i;
  bool single_ff = false;
  ਕਾਲਮ_3 *q;

  /* Read 1st character in a line or any character succeeding a FF */
  ਜੇ ((c = getc (f)) == '\f' && p->ਭਰਿਆ_ਪੰਨਾ_printed)
    /* A FF-coincidence with a previous full_page_printed.
       To avoid an additional empty page, eliminate the FF */
    ਜੇ ((c = getc (f)) == '\n')
      c = getc (f);

  p->ਭਰਿਆ_ਪੰਨਾ_printed = false;

  /* 1st character a FF means a single FF without any printable
     characters. Don't count it as a line with -n option. */
  ਜੇ (c == '\f')
    single_ff = true;

  /* Preparing for a FF-coincidence: Maybe we finish that page
     without a FF found */
  ਜੇ (ਆਖਰੀ_ਸਤਰ)
    p->ਭਰਿਆ_ਪੰਨਾ_printed = true;

  ਜਦੋਂ (c != '\n')
    {
      ਜੇ (c == '\f')
        {
          /* No FF-coincidence possible,
             no catching up of a FF-coincidence with next page */
          ਜੇ (ਆਖਰੀ_ਸਤਰ)
            {
              ਜੇ (!parallel_files)
                ਲਈ (q = ਕਾਲਮ_ਸਦਿਸ਼, i = columns; i; ++q, --i)
                  q->ਭਰਿਆ_ਪੰਨਾ_printed = false;
              ਨਹੀਂਤਾਂ
                p->ਭਰਿਆ_ਪੰਨਾ_printed = false;
            }

          ਜੇ ((c = getc (f)) != '\n')
            ungetc (c, f);
          hold_file (p);
          ਰੋਕੋ;
        }
      ਨਹੀਂਤਾਂ ਜੇ (c == EOF)
        {
          close_file (p);
          ਰੋਕੋ;
        }
      c = getc (f);
    }

  ਜੇ (skip_ਗਿਣਤੀ)
    ਜੇ ((!parallel_files || ਕਾਲਮ_ਸੰਖਿਆ == 1) && !single_ff)
      ++ਸਤਰ_ਗਿਣਤੀ;
}

/* If we're tabifying output,

   When print_char encounters white space it keeps track
   of our desired horizontal position and delays printing
   until this function is called. */

ਸਥਿਰ ਖਾਲੀ
print_white_space (ਖਾਲੀ)
{
  ਪੂਰਨ h_new;
  ਪੂਰਨ h_old = output_position;
  ਪੂਰਨ goal = h_old + spaces_not_printed;

  ਜਦੋਂ (goal - h_old > 1
         && (h_new = POS_AFTER_TAB (chars_ਪ੍ਰਤੀ_output_tab, h_old)) <= goal)
    {
      ਅੱਖਰ_ਲਿਖੋ (output_tab_char);
      h_old = h_new;
    }
  ਜਦੋਂ (++h_old <= goal)
    ਅੱਖਰ_ਲਿਖੋ (' ');

  output_position = goal;
  spaces_not_printed = 0;
}

/* Print column separators.

   We keep a count until we know that we'll be printing a line,
   then print_sep_string() is called. */

ਸਥਿਰ ਖਾਲੀ
print_sep_string (ਖਾਲੀ)
{
  ਅੱਖਰ ਅਚਲ *s = ਖਾਨਾ_sep_string;
  ਪੂਰਨ l = ਖਾਨਾ_sep_ਲੰਮਾਈ;

  ਜੇ (separators_not_printed <= 0)
    {
      /* We'll be starting a line with chars_per_margin, anything else? */
      ਜੇ (spaces_not_printed > 0)
        print_white_space ();
    }
  ਨਹੀਂਤਾਂ
    {
      ਲਈ (; separators_not_printed > 0; --separators_not_printed)
        {
          ਜਦੋਂ (l-- > 0)
            {
              /* 3 types of sep_strings: spaces only, spaces and chars,
              chars only */
              ਜੇ (*s == ' ')
                {
                  /* We're tabifying output; consecutive spaces in
                  sep_string may have to be converted to tabs */
                  s++;
                  ++spaces_not_printed;
                }
              ਨਹੀਂਤਾਂ
                {
                  ਜੇ (spaces_not_printed > 0)
                    print_white_space ();
                  ਅੱਖਰ_ਲਿਖੋ (*s++);
                  ++output_position;
                }
            }
          /* sep_string ends with some spaces */
          ਜੇ (spaces_not_printed > 0)
            print_white_space ();
        }
    }
}

/* Print (or store, depending on p->char_func) a clump of N
   characters. */

ਸਥਿਰ ਖਾਲੀ
print_clump (ਕਾਲਮ_3 *p, ਪੂਰਨ n, ਅੱਖਰ *clump)
{
  ਜਦੋਂ (n--)
    (p->char_func) (*clump++);

  ਜੇ (ferror (stdout))
    write_error ();
}

/* Print a character.

   Update the following comment: process-char hasn't been used any
   longer.
   If we're tabifying, all tabs have been converted to spaces by
   process_char().  Keep a count of consecutive spaces, and when
   a nonspace is encountered, call print_white_space() to print the
   required number of tabs and spaces. */

ਸਥਿਰ ਖਾਲੀ
print_char (ਅੱਖਰ c)
{
  ਜੇ (tabify_output)
    {
      ਜੇ (c == ' ')
        {
          ++spaces_not_printed;
          ਵਾਪਸ;
        }
      ਨਹੀਂਤਾਂ ਜੇ (spaces_not_printed > 0)
        print_white_space ();

      /* Nonprintables are assumed to have width 0, except '\b'. */
      ਜੇ (! isprint (ਮੰਜ਼ਿਲ_uchar (c)))
        {
          ਜੇ (c == '\b')
            --output_position;
        }
      ਨਹੀਂਤਾਂ
        ++output_position;
    }
  ਅੱਖਰ_ਲਿਖੋ (c);
}

/* Skip to page PAGE before printing.
   PAGE may be larger than total number of pages. */

ਸਥਿਰ bool
skip_ਮੰਜ਼ਿਲ_ਪੰਨਾ (uintmax_t ਪੰਨਾ)
{
  ਲਈ (uintmax_t n = 1; n < ਪੰਨਾ; ++n)
    {
      ਕਾਲਮ_3 *p;
      ਪੂਰਨ j;

      ਲਈ (ਪੂਰਨ i = 1; i < ਸਤਰਾਂ_ਪ੍ਰਤੀ_body; ++i)
        {
          ਲਈ (j = 1, p = ਕਾਲਮ_ਸਦਿਸ਼; j <= columns; ++j, ++p)
            ਜੇ (p->ਸਥਿਤੀ == OPEN)
              skip_read (p, j);
        }
      ਆਖਰੀ_ਸਤਰ = true;
      ਲਈ (j = 1, p = ਕਾਲਮ_ਸਦਿਸ਼; j <= columns; ++j, ++p)
        ਜੇ (p->ਸਥਿਤੀ == OPEN)
          skip_read (p, j);

      ਜੇ (storing_columns)	/* change FF_FOUND to ON_HOLD */
        ਲਈ (j = 1, p = ਕਾਲਮ_ਸਦਿਸ਼; j <= columns; ++j, ++p)
          ਜੇ (p->ਸਥਿਤੀ != CLOSED)
            p->ਸਥਿਤੀ = ON_HOLD;

      reset_ਸਥਿਤੀ ();
      ਆਖਰੀ_ਸਤਰ = false;

      ਜੇ (files_ready_ਮੰਜ਼ਿਲ_read < 1)
        {
          /* It's very helpful, normally the total number of pages is
             not known in advance.  */
          error (0, 0,
                 _("starting page number %ju exceeds page count %ju"),
                 ਪੰਨਾ, n);
          ਰੋਕੋ;
        }
    }
  ਵਾਪਸ files_ready_ਮੰਜ਼ਿਲ_read > 0;
}

/* Print a header.

   Formfeeds are assumed to use up two lines at the beginning of
   the page. */

ਸਥਿਰ ਖਾਲੀ
print_ਸਿਰਲੇਖ (ਖਾਲੀ)
{
  ਅੱਖਰ ਪੰਨਾ_ਪਾਠ[256 + INT_STRLEN_BOUND (ਪੰਨਾ_ਸੰਖਿਆ)];
  ਪੂਰਨ ਉਪਲਬਧ_ਚੌੜਾਈ;
  ਪੂਰਨ lhs_spaces;
  ਪੂਰਨ rhs_spaces;

  output_position = 0;
  pad_across_ਮੰਜ਼ਿਲ (chars_ਪ੍ਰਤੀ_margin);
  print_white_space ();

  ਜੇ (ਪੰਨਾ_ਸੰਖਿਆ == 0)
    error (EXIT_FAILURE, 0, _("page number overflow"));

  /* The translator must ensure that formatting the translation of
     "Page %ju" does not generate more than (sizeof page_text - 1)
     bytes.  */
  sprintf (ਪੰਨਾ_ਪਾਠ, _("Page %ju"), ਪੰਨਾ_ਸੰਖਿਆ);
  ਉਪਲਬਧ_ਚੌੜਾਈ = ਸਿਰਲੇਖ_ਚੌੜਾਈ_ਉਪਲਬਧ - mbswidth (ਪੰਨਾ_ਪਾਠ, 0);
  ਉਪਲਬਧ_ਚੌੜਾਈ = MAX (0, ਉਪਲਬਧ_ਚੌੜਾਈ);
  lhs_spaces = ਉਪਲਬਧ_ਚੌੜਾਈ >> 1;
  rhs_spaces = ਉਪਲਬਧ_ਚੌੜਾਈ - lhs_spaces;

  ਲਿਖੋ ("\n\n%*s%s%*s%s%*s%s\n\n\n",
          chars_ਪ੍ਰਤੀ_margin, "",
          date_ਪਾਠ, lhs_spaces, " ",
          file_ਪਾਠ, rhs_spaces, " ", ਪੰਨਾ_ਪਾਠ);

  print_a_ਸਿਰਲੇਖ = false;
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

ਸਥਿਰ bool
read_ਸਤਰ (ਕਾਲਮ_3 *p)
{
  ਪੂਰਨ c;
  ਪੂਰਨ chars IF_LINT (= 0);
  ਪੂਰਨ ਆਖਰੀ_ਨਿਵੇਸ਼_position;
  ਪੂਰਨ j, k;
  ਕਾਲਮ_3 *q;

  /* read 1st character in each line or any character succeeding a FF: */
  c = getc (p->fp);

  ਆਖਰੀ_ਨਿਵੇਸ਼_position = ਨਿਵੇਸ਼_position;

  ਜੇ (c == '\f' && p->ਭਰਿਆ_ਪੰਨਾ_printed)
    ਜੇ ((c = getc (p->fp)) == '\n')
      c = getc (p->fp);
  p->ਭਰਿਆ_ਪੰਨਾ_printed = false;

  ਚੋਣ (c)
    {
    ਹਾਲਤ '\f':
      ਜੇ ((c = getc (p->fp)) != '\n')
        ungetc (c, p->fp);
      FF_only = true;
      ਜੇ (print_a_ਸਿਰਲੇਖ && !storing_columns)
        {
          pad_vertically = true;
          print_ਸਿਰਲੇਖ ();
        }
      ਨਹੀਂਤਾਂ ਜੇ (keep_FF)
        print_a_FF = true;
      hold_file (p);
      ਵਾਪਸ true;
    ਹਾਲਤ EOF:
      close_file (p);
      ਵਾਪਸ true;
    ਹਾਲਤ '\n':
      ਰੋਕੋ;
    ਮੂਲ:
      chars = char_ਮੰਜ਼ਿਲ_clump (c);
    }

  ਜੇ (truncate_ਸਤਰਾਂ && ਨਿਵੇਸ਼_position > chars_ਪ੍ਰਤੀ_ਕਾਲਮ)
    {
      ਨਿਵੇਸ਼_position = ਆਖਰੀ_ਨਿਵੇਸ਼_position;
      ਵਾਪਸ false;
    }

  ਜੇ (p->char_func != store_char)
    {
      pad_vertically = true;

      ਜੇ (print_a_ਸਿਰਲੇਖ && !storing_columns)
        print_ਸਿਰਲੇਖ ();

      ਜੇ (parallel_files && align_ਖਾਲੀ_ਥਾਵਾਂ_ਖਾਨੇ_ਗਿਣਤੀ)
        {
          /* We have to align empty columns at the beginning of a line. */
          k = separators_not_printed;
          separators_not_printed = 0;
          ਲਈ (j = 1, q = ਕਾਲਮ_ਸਦਿਸ਼; j <= k; ++j, ++q)
            {
              align_ਕਾਲਮ (q);
              separators_not_printed += 1;
            }
          padding_not_printed = p->ਸ਼ੁਰੂ_position;
          ਜੇ (truncate_ਸਤਰਾਂ)
            spaces_not_printed = chars_ਪ੍ਰਤੀ_ਕਾਲਮ;
          ਨਹੀਂਤਾਂ
            spaces_not_printed = 0;
          align_ਖਾਲੀ_ਥਾਵਾਂ_ਖਾਨੇ_ਗਿਣਤੀ = false;
        }

      ਜੇ (ਖਾਨਾ_sep_ਲੰਮਾਈ < padding_not_printed)
        {
          pad_across_ਮੰਜ਼ਿਲ (padding_not_printed - ਖਾਨਾ_sep_ਲੰਮਾਈ);
          padding_not_printed = ANYWHERE;
        }

      ਜੇ (use_ਖਾਨਾ_separator)
        print_sep_string ();
    }

  ਜੇ (p->numbered)
    ਜੋੜੋ_ਸਤਰ_ਸੰਖਿਆ (p);

  ਖਾਲੀ_ਥਾਵਾਂ_ਸਤਰ = false;
  ਜੇ (c == '\n')
    ਵਾਪਸ true;

  print_clump (p, chars, clump_buff);

  ਜਦੋਂ (true)
    {
      c = getc (p->fp);

      ਚੋਣ (c)
        {
        ਹਾਲਤ '\n':
          ਵਾਪਸ true;
        ਹਾਲਤ '\f':
          ਜੇ ((c = getc (p->fp)) != '\n')
            ungetc (c, p->fp);
          ਜੇ (keep_FF)
            print_a_FF = true;
          hold_file (p);
          ਵਾਪਸ true;
        ਹਾਲਤ EOF:
          close_file (p);
          ਵਾਪਸ true;
        }

      ਆਖਰੀ_ਨਿਵੇਸ਼_position = ਨਿਵੇਸ਼_position;
      chars = char_ਮੰਜ਼ਿਲ_clump (c);
      ਜੇ (truncate_ਸਤਰਾਂ && ਨਿਵੇਸ਼_position > chars_ਪ੍ਰਤੀ_ਕਾਲਮ)
        {
          ਨਿਵੇਸ਼_position = ਆਖਰੀ_ਨਿਵੇਸ਼_position;
          ਵਾਪਸ false;
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

ਸਥਿਰ bool
print_stored (ਕਾਲਮ_3 *p)
{
  ਕਾਲਮ_3 *q;

  ਪੂਰਨ ਸਤਰ = p->current_ਸਤਰ++;
  ਅੱਖਰ *ਪਹਿਲਾ = &buff[ਸਤਰ_ਸਦਿਸ਼[ਸਤਰ]];
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
  ਅੱਖਰ *ਆਖਰੀ = &buff[ਸਤਰ_ਸਦਿਸ਼[ਸਤਰ + 1]];

  pad_vertically = true;

  ਜੇ (print_a_ਸਿਰਲੇਖ)
    print_ਸਿਰਲੇਖ ();

  ਜੇ (p->ਸਥਿਤੀ == FF_ਮਿਲਿਆ)
    {
      ਪੂਰਨ i;
      ਲਈ (i = 1, q = ਕਾਲਮ_ਸਦਿਸ਼; i <= columns; ++i, ++q)
        q->ਸਥਿਤੀ = ON_HOLD;
      ਜੇ (ਕਾਲਮ_ਸਦਿਸ਼->ਸਤਰਾਂ_ਮੰਜ਼ਿਲ_print <= 0)
        {
          ਜੇ (!extremities)
            pad_vertically = false;
          ਵਾਪਸ true;		/* print a header only */
        }
    }

  ਜੇ (ਖਾਨਾ_sep_ਲੰਮਾਈ < padding_not_printed)
    {
      pad_across_ਮੰਜ਼ਿਲ (padding_not_printed - ਖਾਨਾ_sep_ਲੰਮਾਈ);
      padding_not_printed = ANYWHERE;
    }

  ਜੇ (use_ਖਾਨਾ_separator)
    print_sep_string ();

  ਜਦੋਂ (ਪਹਿਲਾ != ਆਖਰੀ)
    print_char (*ਪਹਿਲਾ++);

  ਜੇ (spaces_not_printed == 0)
    {
      output_position = p->ਸ਼ੁਰੂ_position + ਸਿਰਾ_ਸਦਿਸ਼[ਸਤਰ];
      ਜੇ (p->ਸ਼ੁਰੂ_position - ਖਾਨਾ_sep_ਲੰਮਾਈ == chars_ਪ੍ਰਤੀ_margin)
        output_position -= ਖਾਨਾ_sep_ਲੰਮਾਈ;
    }

  ਵਾਪਸ true;
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

ਸਥਿਰ ਪੂਰਨ
char_ਮੰਜ਼ਿਲ_clump (ਅੱਖਰ c)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ uc = c;
  ਅੱਖਰ *s = clump_buff;
  ਅੱਖਰ esc_buff[4];
  ਪੂਰਨ ਚੌੜਾਈ;
  ਪੂਰਨ chars;
  ਪੂਰਨ chars_ਪ੍ਰਤੀ_c = 8;

  ਜੇ (c == ਨਿਵੇਸ਼_tab_char)
    chars_ਪ੍ਰਤੀ_c = chars_ਪ੍ਰਤੀ_ਨਿਵੇਸ਼_tab;

  ਜੇ (c == ਨਿਵੇਸ਼_tab_char || c == '\t')
    {
      ਚੌੜਾਈ = TAB_ਚੌੜਾਈ (chars_ਪ੍ਰਤੀ_c, ਨਿਵੇਸ਼_position);

      ਜੇ (untabify_ਨਿਵੇਸ਼)
        {
          ਲਈ (ਪੂਰਨ i = ਚੌੜਾਈ; i; --i)
            *s++ = ' ';
          chars = ਚੌੜਾਈ;
        }
      ਨਹੀਂਤਾਂ
        {
          *s = c;
          chars = 1;
        }

    }
  ਨਹੀਂਤਾਂ ਜੇ (! isprint (uc))
    {
      ਜੇ (use_esc_sequence)
        {
          ਚੌੜਾਈ = 4;
          chars = 4;
          *s++ = '\\';
          sprintf (esc_buff, "%03o", uc);
          ਲਈ (ਪੂਰਨ i = 0; i <= 2; ++i)
            *s++ = esc_buff[i];
        }
      ਨਹੀਂਤਾਂ ਜੇ (use_cntrl_ਅਗੇਤਰ)
        {
          ਜੇ (uc < 0200)
            {
              ਚੌੜਾਈ = 2;
              chars = 2;
              *s++ = '^';
              *s = c ^ 0100;
            }
          ਨਹੀਂਤਾਂ
            {
              ਚੌੜਾਈ = 4;
              chars = 4;
              *s++ = '\\';
              sprintf (esc_buff, "%03o", uc);
              ਲਈ (ਪੂਰਨ i = 0; i <= 2; ++i)
                *s++ = esc_buff[i];
            }
        }
      ਨਹੀਂਤਾਂ ਜੇ (c == '\b')
        {
          ਚੌੜਾਈ = -1;
          chars = 1;
          *s = c;
        }
      ਨਹੀਂਤਾਂ
        {
          ਚੌੜਾਈ = 0;
          chars = 1;
          *s = c;
        }
    }
  ਨਹੀਂਤਾਂ
    {
      ਚੌੜਾਈ = 1;
      chars = 1;
      *s = c;
    }

  /* Too many backspaces must put us in position 0 -- never negative.  */
  ਜੇ (ਚੌੜਾਈ < 0 && ਨਿਵੇਸ਼_position == 0)
    {
      chars = 0;
      ਨਿਵੇਸ਼_position = 0;
    }
  ਨਹੀਂਤਾਂ ਜੇ (ਚੌੜਾਈ < 0 && ਨਿਵੇਸ਼_position <= -ਚੌੜਾਈ)
    ਨਿਵੇਸ਼_position = 0;
  ਨਹੀਂਤਾਂ
    {
      ਜੇ (ckd_ਜੋੜੋ (&ਨਿਵੇਸ਼_position, ਨਿਵੇਸ਼_position, ਚੌੜਾਈ))
        integer_ਉਛਾਲ ();
    }

  ਵਾਪਸ chars;
}

/* We've just printed some files and need to clean up things before
   looking for more options and printing the next batch of files.

   Free everything we've xmalloc'ed, except 'header'. */

ਸਥਿਰ ਖਾਲੀ
cleanup (ਖਾਲੀ)
{
  ਮੁਕਤ (ਸੰਖਿਆ_buff);
  ਮੁਕਤ (clump_buff);
  ਮੁਕਤ (ਕਾਲਮ_ਸਦਿਸ਼);
  ਮੁਕਤ (ਸਤਰ_ਸਦਿਸ਼);
  ਮੁਕਤ (ਸਿਰਾ_ਸਦਿਸ਼);
  ਮੁਕਤ (buff);
}

/* Complain, print a usage message, and die. */

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              ਪ੍ਰੋਗਰਾਮ_name);

      fputs (_("\
Paginate or columnate FILE(s) ਲਈ printing.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\n\
  +ਪਹਿਲਾ_ਪੰਨਾ[:ਆਖਰੀ_ਪੰਨਾ], --ਪੰਨੇ=ਪਹਿਲਾ_ਪੰਨਾ[:ਆਖਰੀ_ਪੰਨਾ]\n\
         begin [stop] printing with ਪੰਨਾ ਪਹਿਲਾ_2[ਆਖਰੀ_2]ਪੰਨਾ_2\n\
"));
      oputs (_("\
  -ਖਾਨੇ_ਗਿਣਤੀ, --columns=ਖਾਨੇ_ਗਿਣਤੀ\n\
         output ਖਾਨੇ_ਗਿਣਤੀ columns and print columns ਥੱਲੇ, unless -a is ਵਰਤਿਆ.\n\
         Balance ਸੰਖਿਆ ਦਾ ਸਤਰਾਂ ਵਿੱਚ the columns on each ਪੰਨਾ\n\
"));
      oputs (_("\
  -a, --across\n\
         print columns across rather than ਥੱਲੇ, ਵਰਤਿਆ together with -ਖਾਨੇ_ਗਿਣਤੀ\n\
"));
      oputs (_("\
  -c, --show-control-chars\n\
         use hat notation (^G) and octal backslash notation\n\
"));
      oputs (_("\
  -d, --ਦੁੱਗਣਾ-space\n\
         ਦੁੱਗਣਾ space the output\n\
"));
      oputs (_("\
  -D, --date-format=FORMAT\n\
         use FORMAT ਲਈ the ਸਿਰਲੇਖ date\n\
"));
      oputs (_("\
  -e[CHAR[ਚੌੜਾਈ_2]], --expand-tabs[=CHAR[ਚੌੜਾਈ_2]]\n\
         expand ਨਿਵੇਸ਼ CHARs (TABs) ਮੰਜ਼ਿਲ tab ਚੌੜਾਈ_2 (8)\n\
"));
      oputs (_("\
  -F, -f, --form-feed\n\
         use form feeds instead ਦਾ newlines ਮੰਜ਼ਿਲ separate ਪੰਨੇ\n\
         (by a 3-ਸਤਰ ਪੰਨਾ ਸਿਰਲੇਖ with -F or a 5-ਸਤਰ ਸਿਰਲੇਖ\n\
         and trailer without -F)\n\
"));
      oputs (_("\
  -h, --ਸਿਰਲੇਖ=ਸਿਰਲੇਖ_3\n\
         use a centered ਸਿਰਲੇਖ_3 instead ਦਾ filename ਵਿੱਚ ਪੰਨਾ ਸਿਰਲੇਖ,\n\
         -h \"\" prints a blank ਸਤਰ, don't use -h\"\"\n\
"));
      oputs (_("\
  -i[CHAR[ਚੌੜਾਈ_2]], --output-tabs[=CHAR[ਚੌੜਾਈ_2]]\n\
         replace spaces with CHARs (TABs) ਮੰਜ਼ਿਲ tab ਚੌੜਾਈ_2 (8)\n\
"));
      oputs (_("\
  -J, --join-ਸਤਰਾਂ\n\
         ਮਿਲਾਓ ਭਰਿਆ ਸਤਰਾਂ, turns off -W ਸਤਰ truncation,\n\
         no ਕਾਲਮ alignment, --sep-string[=STRING] sets separators\n\
"));
      oputs (_("\
  -l, --ਲੰਮਾਈ=ਪੰਨਾ_ਲੰਮਾਈ\n\
         ਸੈੱਟ_2 the ਪੰਨਾ ਲੰਮਾਈ ਮੰਜ਼ਿਲ ਪੰਨਾ_ਲੰਮਾਈ (66) ਸਤਰਾਂ\n\
         (ਮੂਲ ਸੰਖਿਆ ਦਾ ਸਤਰਾਂ ਦਾ ਪਾਠ 56, and with -F 63).\n\
         ਸਿੱਟਾ_ਦਿੰਦਾ -t ਜੇ ਪੰਨਾ_ਲੰਮਾਈ <= 10\n\
"));
      oputs (_("\
  -m, --ਮਿਲਾਓ\n\
         print all files ਵਿੱਚ parallel, one ਵਿੱਚ each ਕਾਲਮ,\n\
         truncate ਸਤਰਾਂ, but join ਸਤਰਾਂ ਦਾ ਭਰਿਆ ਲੰਮਾਈ with -J\n\
"));
      oputs (_("\
  -n[SEP[DIGITS]], --ਸੰਖਿਆ-ਸਤਰਾਂ[=SEP[DIGITS]]\n\
         ਸੰਖਿਆ ਸਤਰਾਂ, use DIGITS (5) digits, then SEP (TAB),\n\
         ਮੂਲ counting starts with 1st ਸਤਰ ਦਾ ਨਿਵੇਸ਼ file\n\
"));
      oputs (_("\
  -N, --ਪਹਿਲਾ-ਸਤਰ-ਸੰਖਿਆ=ਸੰਖਿਆ_2\n\
         ਸ਼ੁਰੂ counting with ਸੰਖਿਆ_2 at 1st ਸਤਰ ਦਾ ਪਹਿਲਾ\n\
         ਪੰਨਾ printed (see +ਪਹਿਲਾ_ਪੰਨਾ)\n\
"));
      oputs (_("\
  -o, --indent=MARGIN\n\
         ਵਿੱਥ each ਸਤਰ with MARGIN (zero) spaces, ਕਰੋ not\n\
         affect -w or -W, MARGIN will be added ਮੰਜ਼ਿਲ ਪੰਨਾ_ਚੌੜਾਈ\n\
"));
      oputs (_("\
  -r, --no-file-warnings\n\
         omit warning when a file cannot be opened\n\
"));
      oputs (_("\
  -s[CHAR], --separator[=CHAR]\n\
         separate columns by a single character, ਮੂਲ ਲਈ CHAR\n\
         is the <TAB> character without -w and \'no ਅੱਖਰ\' with -w.\n\
         -s[CHAR] turns off ਸਤਰ truncation ਦਾ all 3 ਕਾਲਮ\n\
         options (-ਖਾਨੇ_ਗਿਣਤੀ|-a -ਖਾਨੇ_ਗਿਣਤੀ|-m) except -w is ਸੈੱਟ_2\n\
"));
      oputs (_("\
  -S[STRING], --sep-string[=STRING]\n\
         separate columns by STRING,\n\
         without -S: Default separator <TAB> with -J and <space>\n\
         otherwise (same as -S\" \"), no effect on ਕਾਲਮ options\n\
"));
      oputs (_("\
  -t, --omit-ਸਿਰਲੇਖ\n\
         omit ਪੰਨਾ ਸਿਰਲੇਖਾਂ and trailers; implied ਜੇ ਪੰਨਾ_ਲੰਮਾਈ <= 10\n\
"));
      oputs (_("\
  -T, --omit-pagination\n\
         omit ਪੰਨਾ ਸਿਰਲੇਖਾਂ and trailers,\n\
         eliminate any pagination by form feeds ਸੈੱਟ_2 ਵਿੱਚ ਨਿਵੇਸ਼ files\n\
"));
      oputs (_("\
  -v, --show-nonprinting\n\
         use octal backslash notation\n\
"));
      oputs (_("\
  -w, --ਚੌੜਾਈ=ਪੰਨਾ_ਚੌੜਾਈ\n\
         ਸੈੱਟ_2 ਪੰਨਾ ਚੌੜਾਈ ਮੰਜ਼ਿਲ ਪੰਨਾ_ਚੌੜਾਈ (72) characters ਲਈ\n\
         ਗੁਣਜ ਪਾਠ-ਕਾਲਮ output only, -s[ਅੱਖਰ] turns off (72)\n\
"));
      oputs (_("\
  -W, --ਪੰਨਾ-ਚੌੜਾਈ=ਪੰਨਾ_ਚੌੜਾਈ\n\
         ਸੈੱਟ_2 ਪੰਨਾ ਚੌੜਾਈ ਮੰਜ਼ਿਲ ਪੰਨਾ_ਚੌੜਾਈ (72) characters always,\n\
         truncate ਸਤਰਾਂ, except -J option is ਸੈੱਟ_2,\n\
         no interference with -S or -s\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}
