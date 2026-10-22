/* Shuffle lines of text.

   Copyright (C) 2006-2026 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.

   Written by Paul Eggert.  */

#include <config.h>

#include <sys/types.h>
#include <getopt.h>
#include "system.h"

#include "fadvise.h"
#include "linebuffer.h"
#include "quote.h"
#include "randint.h"
#include "randperm.h"
#include "read-file.h"
#include "stdio--.h"
#include "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "shuf"

#define AUTHORS proper_name ("Paul Eggert")

/* reservoir-sampling introduces CPU overhead for small inputs.
   So only enable it for inputs >= this limit.
   This limit was determined using these commands:
     $ for p in $(seq 7); do src/seq $((10**$p)) > 10p$p.in; done
     $ for p in $(seq 7); do time shuf-nores -n10 10p$p.in >/dev/null; done
     $ for p in $(seq 7); do time shuf -n10 10p$p.in >/dev/null; done  .*/
enum { RESERVOIR_MIN_INPUT = 8192 * 1024 };


void
usage (정수 status)
{
  만약 (status != EXIT_SUCCESS)
    emit_try_help ();
  아니면
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]\n\
  or:  %s -e [OPTION]... [ARG]...\n\
  or:  %s -i LO-HI [OPTION]...\n\
"),
              program_name, program_name, program_name);
      fputs (_("\
Write a random permutation of the input lines to standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -e, --echo                treat each ARG as an input line\n\
"));
      oputs (_("\
  -i, --input-range=LO-HI   treat each number LO through HI as an input line\n\
"));
      oputs (_("\
  -n, --head-count=COUNT    output at most COUNT lines\n\
"));
      oputs (_("\
  -o, --output=FILE         write result to FILE instead of standard output\n\
"));
      oputs (_("\
      --random-source=FILE  get random bytes from FILE\n\
"));
      oputs (_("\
  -r, --repeat              output lines can be repeated\n\
"));
      oputs (_("\
  -z, --zero-terminated     line delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }

  exit (status);
}

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
enum
{
  RANDOM_SOURCE_OPTION = CHAR_MAX + 1
};

static struct option const long_opts[] =
{
  {"echo", no_argument, NULL, 'e'},
  {"input-range", required_argument, NULL, 'i'},
  {"head-count", required_argument, NULL, 'n'},
  {"output", required_argument, NULL, 'o'},
  {"random-source", required_argument, NULL, RANDOM_SOURCE_OPTION},
  {"repeat", no_argument, NULL, 'r'},
  {"zero-terminated", no_argument, NULL, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0},
};

static void
input_from_argv (char **operand, 정수 n_operands, char eolbyte)
{
  char *p;
  size_t size = n_operands;
  정수 i;

  위해 (i = 0; i < n_operands; i++)
    size += strlen (operand[i]);
  p = xmalloc (size);

  위해 (i = 0; i < n_operands; i++)
    {
      char *p1 = stpcpy (p, operand[i]);
      operand[i] = p;
      p = p1;
      *p++ = eolbyte;
    }

  operand[n_operands] = p;
}

/* Return the start of the next line after LINE, which is guaranteed
   to end in EOLBYTE.  */

static char *
next_line (char *line, char eolbyte)
{
  char *p = rawmemchr (line, eolbyte);
  반환 p + 1;
}

/* Return the size of the input if possible or OFF_T_MAX if not.  */

static off_t
input_size (void)
{
  off_t file_size;

  struct stat stat_buf;
  만약 (fstat (STDIN_FILENO, &stat_buf) != 0)
    반환 OFF_T_MAX;
  만약 (usable_st_size (&stat_buf))
    file_size = stat_buf.st_size;
  아니면
    반환 OFF_T_MAX;

  off_t input_offset = lseek (STDIN_FILENO, 0, SEEK_CUR);
  만약 (input_offset < 0)
    반환 OFF_T_MAX;

  file_size -= input_offset;

  반환 file_size;
}

/* Read all lines and store up to K permuted lines in *OUT_RSRV.
   Return the number of lines read, up to a maximum of K.  */

static idx_t
read_input_reservoir_sampling (FILE *in, char eolbyte, idx_t k,
                               struct randint_source *s,
                               struct linebuffer **out_rsrv)
{
  randint n_lines = 0;
  idx_t n_alloc_lines = 0;
  struct linebuffer *line = NULL;
  struct linebuffer *rsrv = NULL;

  /* Fill the first K lines, directly into the reservoir.  */
  위해 (n_lines = 0; n_lines < k; n_lines++)
    {
      /* Enlarge reservoir if needed.  */
      만약 (n_lines == n_alloc_lines)
        {
          idx_t old = n_alloc_lines;
          rsrv = xpalloc (rsrv, &n_alloc_lines, 1, k, sizeof *rsrv);
          memset (&rsrv[n_lines], 0, (n_alloc_lines - old) * sizeof *rsrv);
        }

      line = readlinebuffer_delim (&rsrv[n_lines], in, eolbyte);
      만약 (!line)
        break;
    }

  /* last line wasn't null - so there may be more lines to read.  */
  만약 (line != NULL)
    {
      struct linebuffer dummy;
      initbuffer (&dummy);  /* space for lines not put in reservoir.  */

      /* Choose the fate of the next line, with decreasing probability (as
         n_lines increases in size).

         If the line will be used, store it directly in the reservoir.
         Otherwise, store it in dummy space.

         With 'struct linebuffer', storing into existing buffer will reduce
         re-allocations (will only re-allocate if the new line is longer than
         the currently allocated space).  */
      do
        {
          randint j = randint_choose (s, n_lines + 1);  /* 0 .. n_lines.  */
          line = (j < k) ? (&rsrv[j]) : (&dummy);
        }
      동안 (readlinebuffer_delim (line, in, eolbyte) != NULL && n_lines++);

      만약 (! n_lines)
        error (EXIT_FAILURE, EOVERFLOW, _("too many input lines"));

      freebuffer (&dummy);
    }

  /* no more input lines, or an input error.  */
  만약 (ferror (in))
    error (EXIT_FAILURE, errno, _("read error"));

  *out_rsrv = rsrv;
  반환 MIN (k, n_lines);
}

static 정수
write_permuted_output_reservoir (size_t n_lines, struct linebuffer *lines,
                                 size_t const *permutation)
{
  위해 (size_t i = 0; i < n_lines; i++)
    {
      const struct linebuffer *p = &lines[permutation[i]];
      만약 (fwrite (p->buffer, sizeof (char), p->length, stdout) != p->length)
        반환 -1;
    }

  반환 0;
}

/* Read data from file IN.  Input lines are delimited by EOLBYTE;
   silently append a trailing EOLBYTE if the file ends in some other
   byte.  Store a pointer to the resulting array of lines into *PLINE.
   Return the number of lines read.  Report an error and exit on
   failure.  */

static size_t
read_input (FILE *in, char eolbyte, char ***pline)
{
  char *p;
  char *buf = NULL;
  size_t used;
  char *lim;
  char **line;
  size_t n_lines;

  /* TODO: We should limit the amount of data read here,
     to less than RESERVOIR_MIN_INPUT.  I.e., adjust fread_file() to support
     taking a byte limit.  We'd then need to ensure we handle a line spanning
     this boundary.  With that in place we could set use_reservoir_sampling
     when used==RESERVOIR_MIN_INPUT, and have read_input_reservoir_sampling()
     call a wrapper function to populate a linebuffer from the internal pline
     or if none left, stdin.  Doing that would give better performance by
     avoiding the reservoir CPU overhead when reading < RESERVOIR_MIN_INPUT
     from a pipe, and allow us to dispense with the input_size() function.  */
  만약 (!(buf = fread_file (in, 0, &used)))
    error (EXIT_FAILURE, errno, _("read error"));

  만약 (used && buf[used - 1] != eolbyte)
    buf[used++] = eolbyte;

  lim = buf + used;

  n_lines = 0;
  위해 (p = buf; p < lim; p = next_line (p, eolbyte))
    n_lines++;

  *pline = line = xnmalloc (n_lines + 1, sizeof *line);

  line[0] = p = buf;
  위해 (size_t i = 1; i <= n_lines; i++)
    line[i] = p = next_line (p, eolbyte);

  반환 n_lines;
}

/* Output N_LINES lines to stdout from LINE array,
   chosen by the indices in PERMUTATION.
   PERMUTATION and LINE must have at least N_LINES elements.
   Strings in LINE must include the line-terminator character.  */
static 정수
write_permuted_lines (size_t n_lines, char *const *line,
                      size_t const *permutation)
{
  위해 (size_t i = 0; i < n_lines; i++)
    {
      char *const *p = line + permutation[i];
      size_t len = p[1] - p[0];
      만약 (fwrite (p[0], sizeof *p[0], len, stdout) != len)
        반환 -1;
    }

  반환 0;
}

/* Print NUMBER followed by EOLBYTE to standard output.
   Return false on failure, true on success.  */
static bool
print_number (unsigned long 정수 number, char eolbyte)
{
  char buf[INT_BUFSIZE_BOUND (unsigned long 정수)];
  char *p = buf + INT_STRLEN_BOUND (unsigned long 정수);
  *p = eolbyte;
  do
    *--p = '0' + number % 10;
  동안 ((number /= 10) != 0);
  idx_t len = buf + sizeof buf - p;
  반환 fwrite (p, 1, len, stdout) == len;
}

/* Output N_LINES of numbers to stdout, from PERMUTATION array.
   PERMUTATION must have at least N_LINES elements.  */
static 정수
write_permuted_numbers (size_t n_lines, size_t lo_input,
                        size_t const *permutation, char eolbyte)
{
  위해 (size_t i = 0; i < n_lines; i++)
    {
      unsigned long 정수 n = lo_input + permutation[i];
      만약 (! print_number (n, eolbyte))
        반환 -1;
    }

  반환 0;
}

/* Output COUNT numbers to stdout, chosen randomly from range
   LO_INPUT through HI_INPUT.  */
static 정수
write_random_numbers (struct randint_source *s, size_t count,
                      size_t lo_input, size_t hi_input, char eolbyte)
{
  const randint range = hi_input - lo_input + 1;

  위해 (size_t i = 0; i < count; i++)
    {
      unsigned long 정수 j = lo_input + randint_choose (s, range);
      만약 (! print_number (j, eolbyte))
        반환 -1;
    }

  반환 0;
}

/* Output COUNT lines to stdout from LINES array.
   LINES must have at least N_LINES elements in it.
   Strings in LINES_ must include the line-terminator character.  */
static 정수
write_random_lines (struct randint_source *s, size_t count,
                    char *const *lines, size_t n_lines)
{
  위해 (size_t i = 0; i < count; i++)
    {
      const randint j = randint_choose (s, n_lines);
      char *const *p = lines + j;
      size_t len = p[1] - p[0];
      만약 (fwrite (p[0], sizeof *p[0], len, stdout) != len)
        반환 -1;
    }

  반환 0;
}

정수
main (정수 argc, char **argv)
{
  bool echo = false;
  bool input_range = false;
  size_t lo_input = SIZE_MAX;
  size_t hi_input = 0;
  idx_t head_lines = MIN (IDX_MAX, SIZE_MAX);
  char const *outfile = NULL;
  char *random_source = NULL;
  char eolbyte = '\n';
  char **input_lines = NULL;
  bool use_reservoir_sampling = false;
  bool repeat = false;

  정수 optc;
  정수 n_operands;
  char **operand;
  size_t n_lines;
  char **line = NULL;
  struct linebuffer *reservoir = NULL;
  struct randint_source *randint_source;
  size_t *permutation = NULL;
  정수 i;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  동안 ((optc = getopt_long (argc, argv, "ei:n:o:rz", long_opts, NULL))
         != -1)
    switch (optc)
      {
      case 'e':
        echo = true;
        break;

      case 'i':
        {
          만약 (input_range)
            error (EXIT_FAILURE, 0, _("multiple -i options specified"));
          input_range = true;

          uintmax_t u;
          char *lo_end;
          strtol_error err = xstrtoumax (optarg, &lo_end, 10, &u, NULL);
          만약 (err == LONGINT_OK)
            {
              lo_input = u;
              만약 (lo_input != u)
                err = LONGINT_OVERFLOW;
              아니면 만약 (*lo_end != '-')
                err = LONGINT_INVALID;
              아니면
                {
                  err = xstrtoumax (lo_end + 1, NULL, 10, &u, "");
                  만약 (err == LONGINT_OK)
                    {
                      hi_input = u;
                      만약 (hi_input != u)
                        err = LONGINT_OVERFLOW;
                    }
                }
            }

          n_lines = hi_input - lo_input + 1;

          만약 (err != LONGINT_OK || (lo_input <= hi_input) == (n_lines == 0))
            error (EXIT_FAILURE, err == LONGINT_OVERFLOW ? EOVERFLOW : 0,
                   "%s: %s", _("invalid input range"), quote (optarg));
        }
        break;

      case 'n':
        {
          uintmax_t argval;
          strtol_error e = xstrtoumax (optarg, NULL, 10, &argval, "");

          만약 (e == LONGINT_OK)
            head_lines = MIN (head_lines, argval);
          아니면 만약 (e != LONGINT_OVERFLOW)
            error (EXIT_FAILURE, 0, _("invalid line count: %s"),
                   quote (optarg));
        }
        break;

      case 'o':
        만약 (outfile && !streq (outfile, optarg))
          error (EXIT_FAILURE, 0, _("multiple output files specified"));
        outfile = optarg;
        break;

      case RANDOM_SOURCE_OPTION:
        만약 (random_source && !streq (random_source, optarg))
          error (EXIT_FAILURE, 0, _("multiple random sources specified"));
        random_source = optarg;
        break;

      case 'r':
        repeat = true;
        break;

      case 'z':
        eolbyte = '\0';
        break;

      case_GETOPT_HELP_CHAR;
      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
      default:
        usage (EXIT_FAILURE);
      }

  n_operands = argc - optind;
  operand = argv + optind;

  /* Check invalid usage.  */
  만약 (echo && input_range)
    {
      error (0, 0, _("cannot combine -e and -i options"));
      usage (EXIT_FAILURE);
    }
  만약 (input_range ? 0 < n_operands : !echo && 1 < n_operands)
    {
      error (0, 0, _("extra operand %s"), quote (operand[!input_range]));
      usage (EXIT_FAILURE);
    }

  /* Prepare input.  */
  만약 (head_lines == 0)
    {
      n_lines = 0;
      line = NULL;
    }
  아니면 만약 (echo)
    {
      input_from_argv (operand, n_operands, eolbyte);
      n_lines = n_operands;
      line = operand;
    }
  아니면 만약 (input_range)
    {
      IF_LINT (n_lines = hi_input - lo_input + 1); /* Avoid GCC 10 warning.  */
      line = NULL;
    }
  아니면
    {
      /* If an input file is specified, re-open it as stdin.  */
      만약 (n_operands == 1
          && ! (streq (operand[0], "-")
                || freopen (operand[0], "r", stdin)))
        error (EXIT_FAILURE, errno, "%s", quotef (operand[0]));

      fadvise (stdin, FADVISE_SEQUENTIAL);

      만약 (repeat || head_lines == MIN (IDX_MAX, SIZE_MAX)
          || input_size () <= RESERVOIR_MIN_INPUT)
        {
          n_lines = read_input (stdin, eolbyte, &input_lines);
          line = input_lines;
        }
      아니면
        {
          use_reservoir_sampling = true;
          n_lines = SIZE_MAX;   /* unknown number of input lines, for now.  */
        }
    }

  /* The adjusted head line count; can be less than HEAD_LINES if the
     input is small and if not repeating.  */
  idx_t ahead_lines = repeat || head_lines < n_lines ? head_lines : n_lines;

  randint_source = randint_all_new (random_source,
                                    (use_reservoir_sampling || repeat
                                     ? SIZE_MAX
                                     : randperm_bound (ahead_lines, n_lines)));
  만약 (! randint_source)
    error (EXIT_FAILURE, errno, "%s",
           quotef (random_source ? random_source : "getrandom"));

  만약 (use_reservoir_sampling)
    {
      /* Instead of reading the entire file into 'line',
         use reservoir-sampling to store just AHEAD_LINES random lines.  */
      n_lines = read_input_reservoir_sampling (stdin, eolbyte, ahead_lines,
                                               randint_source, &reservoir);
      ahead_lines = n_lines;
    }

  /* Close stdin now, rather than earlier, so that randint_all_new
     doesn't have to worry about opening something other than
     stdin.  */
  만약 (! (head_lines == 0 || echo || input_range || fclose (stdin) == 0))
    error (EXIT_FAILURE, errno, _("read error"));

  만약 (!repeat)
    permutation = randperm_new (randint_source, ahead_lines, n_lines);

  만약 (outfile && ! freopen (outfile, "w", stdout))
    error (EXIT_FAILURE, errno, "%s", quotef (outfile));

  /* Generate output according to requested method */
  만약 (repeat)
    {
      만약 (head_lines == 0)
        i = 0;
      아니면
        {
          만약 (n_lines == 0)
            error (EXIT_FAILURE, 0, _("no lines to repeat"));
          만약 (input_range)
            i = write_random_numbers (randint_source, ahead_lines,
                                      lo_input, hi_input, eolbyte);
          아니면
            i = write_random_lines (randint_source, ahead_lines, line, n_lines);
        }
    }
  아니면
    {
      만약 (use_reservoir_sampling)
        i = write_permuted_output_reservoir (n_lines, reservoir, permutation);
      아니면 만약 (input_range)
        i = write_permuted_numbers (ahead_lines, lo_input,
                                    permutation, eolbyte);
      아니면
        i = write_permuted_lines (ahead_lines, line, permutation);
    }

  만약 (i != 0)
    write_error ();

  IF_LINT (randint_all_free (randint_source));  /* For older valgrind.  */

  main_exit (EXIT_SUCCESS);
}
