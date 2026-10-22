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
ga_nanaa { RESERVOIR_MIN_INPUT = 8192 * 1024 };


void
usage (ကိန်းပြည့် status)
{
  အကယ်၍ (status != EXIT_SUCCESS)
    emit_try_help ();
  မဟုတ်ပါက
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
ga_nanaa
{
  RANDOM_SOURCE_OPTION = CHAR_MAX + 1
};

static sa_nracanaa option const long_opts[] =
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
input_from_argv (var_na **operand, ကိန်းပြည့် n_operands, var_na eolbyte)
{
  var_na *p;
  size_t size = n_operands;
  ကိန်းပြည့် i;

  အတွက် (i = 0; i < n_operands; i++)
    size += strlen (operand[i]);
  p = xmalloc (size);

  အတွက် (i = 0; i < n_operands; i++)
    {
      var_na *p1 = stpcpy (p, operand[i]);
      operand[i] = p;
      p = p1;
      *p++ = eolbyte;
    }

  operand[n_operands] = p;
}

/* Return the start of the next line after LINE, which is guaranteed
   to end in EOLBYTE.  */

static var_na *
next_line (var_na *line, var_na eolbyte)
{
  var_na *p = rawmemchr (line, eolbyte);
  ပြန်ပေး p + 1;
}

/* Return the size of the input if possible or OFF_T_MAX if not.  */

static off_t
input_size (void)
{
  off_t file_size;

  sa_nracanaa stat stat_buf;
  အကယ်၍ (fstat (STDIN_FILENO, &stat_buf) != 0)
    ပြန်ပေး OFF_T_MAX;
  အကယ်၍ (usable_st_size (&stat_buf))
    file_size = stat_buf.st_size;
  မဟုတ်ပါက
    ပြန်ပေး OFF_T_MAX;

  off_t input_offset = lseek (STDIN_FILENO, 0, SEEK_CUR);
  အကယ်၍ (input_offset < 0)
    ပြန်ပေး OFF_T_MAX;

  file_size -= input_offset;

  ပြန်ပေး file_size;
}

/* Read all lines and store up to K permuted lines in *OUT_RSRV.
   Return the number of lines read, up to a maximum of K.  */

static idx_t
read_input_reservoir_sampling (FILE *in, var_na eolbyte, idx_t k,
                               sa_nracanaa randint_source *s,
                               sa_nracanaa linebuffer **out_rsrv)
{
  randint n_lines = 0;
  idx_t n_alloc_lines = 0;
  sa_nracanaa linebuffer *line = NULL;
  sa_nracanaa linebuffer *rsrv = NULL;

  /* Fill the first K lines, directly into the reservoir.  */
  အတွက် (n_lines = 0; n_lines < k; n_lines++)
    {
      /* Enlarge reservoir if needed.  */
      အကယ်၍ (n_lines == n_alloc_lines)
        {
          idx_t old = n_alloc_lines;
          rsrv = xpalloc (rsrv, &n_alloc_lines, 1, k, parimaa_na *rsrv);
          memset (&rsrv[n_lines], 0, (n_alloc_lines - old) * parimaa_na *rsrv);
        }

      line = readlinebuffer_delim (&rsrv[n_lines], in, eolbyte);
      အကယ်၍ (!line)
        bha_nga;
    }

  /* last line wasn't null - so there may be more lines to read.  */
  အကယ်၍ (line != NULL)
    {
      sa_nracanaa linebuffer dummy;
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
      နေစဉ် (readlinebuffer_delim (line, in, eolbyte) != NULL && n_lines++);

      အကယ်၍ (! n_lines)
        error (EXIT_FAILURE, EOVERFLOW, _("too many input lines"));

      freebuffer (&dummy);
    }

  /* no more input lines, or an input error.  */
  အကယ်၍ (ferror (in))
    error (EXIT_FAILURE, errno, _("read error"));

  *out_rsrv = rsrv;
  ပြန်ပေး MIN (k, n_lines);
}

static ကိန်းပြည့်
write_permuted_output_reservoir (size_t n_lines, sa_nracanaa linebuffer *lines,
                                 size_t const *permutation)
{
  အတွက် (size_t i = 0; i < n_lines; i++)
    {
      const sa_nracanaa linebuffer *p = &lines[permutation[i]];
      အကယ်၍ (fwrite (p->buffer, parimaa_na (var_na), p->length, stdout) != p->length)
        ပြန်ပေး -1;
    }

  ပြန်ပေး 0;
}

/* Read data from file IN.  Input lines are delimited by EOLBYTE;
   silently append a trailing EOLBYTE if the file ends in some other
   byte.  Store a pointer to the resulting array of lines into *PLINE.
   Return the number of lines read.  Report an error and exit on
   failure.  */

static size_t
read_input (FILE *in, var_na eolbyte, var_na ***pline)
{
  var_na *p;
  var_na *buf = NULL;
  size_t used;
  var_na *lim;
  var_na **line;
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
  အကယ်၍ (!(buf = fread_file (in, 0, &used)))
    error (EXIT_FAILURE, errno, _("read error"));

  အကယ်၍ (used && buf[used - 1] != eolbyte)
    buf[used++] = eolbyte;

  lim = buf + used;

  n_lines = 0;
  အတွက် (p = buf; p < lim; p = next_line (p, eolbyte))
    n_lines++;

  *pline = line = xnmalloc (n_lines + 1, parimaa_na *line);

  line[0] = p = buf;
  အတွက် (size_t i = 1; i <= n_lines; i++)
    line[i] = p = next_line (p, eolbyte);

  ပြန်ပေး n_lines;
}

/* Output N_LINES lines to stdout from LINE array,
   chosen by the indices in PERMUTATION.
   PERMUTATION and LINE must have at least N_LINES elements.
   Strings in LINE must include the line-terminator character.  */
static ကိန်းပြည့်
write_permuted_lines (size_t n_lines, var_na *const *line,
                      size_t const *permutation)
{
  အတွက် (size_t i = 0; i < n_lines; i++)
    {
      var_na *const *p = line + permutation[i];
      size_t len = p[1] - p[0];
      အကယ်၍ (fwrite (p[0], parimaa_na *p[0], len, stdout) != len)
        ပြန်ပေး -1;
    }

  ပြန်ပေး 0;
}

/* Print NUMBER followed by EOLBYTE to standard output.
   Return false on failure, true on success.  */
static bool
print_number (unsigned long ကိန်းပြည့် number, var_na eolbyte)
{
  var_na buf[INT_BUFSIZE_BOUND (unsigned long ကိန်းပြည့်)];
  var_na *p = buf + INT_STRLEN_BOUND (unsigned long ကိန်းပြည့်);
  *p = eolbyte;
  do
    *--p = '0' + number % 10;
  နေစဉ် ((number /= 10) != 0);
  idx_t len = buf + parimaa_na buf - p;
  ပြန်ပေး fwrite (p, 1, len, stdout) == len;
}

/* Output N_LINES of numbers to stdout, from PERMUTATION array.
   PERMUTATION must have at least N_LINES elements.  */
static ကိန်းပြည့်
write_permuted_numbers (size_t n_lines, size_t lo_input,
                        size_t const *permutation, var_na eolbyte)
{
  အတွက် (size_t i = 0; i < n_lines; i++)
    {
      unsigned long ကိန်းပြည့် n = lo_input + permutation[i];
      အကယ်၍ (! print_number (n, eolbyte))
        ပြန်ပေး -1;
    }

  ပြန်ပေး 0;
}

/* Output COUNT numbers to stdout, chosen randomly from range
   LO_INPUT through HI_INPUT.  */
static ကိန်းပြည့်
write_random_numbers (sa_nracanaa randint_source *s, size_t count,
                      size_t lo_input, size_t hi_input, var_na eolbyte)
{
  const randint range = hi_input - lo_input + 1;

  အတွက် (size_t i = 0; i < count; i++)
    {
      unsigned long ကိန်းပြည့် j = lo_input + randint_choose (s, range);
      အကယ်၍ (! print_number (j, eolbyte))
        ပြန်ပေး -1;
    }

  ပြန်ပေး 0;
}

/* Output COUNT lines to stdout from LINES array.
   LINES must have at least N_LINES elements in it.
   Strings in LINES_ must include the line-terminator character.  */
static ကိန်းပြည့်
write_random_lines (sa_nracanaa randint_source *s, size_t count,
                    var_na *const *lines, size_t n_lines)
{
  အတွက် (size_t i = 0; i < count; i++)
    {
      const randint j = randint_choose (s, n_lines);
      var_na *const *p = lines + j;
      size_t len = p[1] - p[0];
      အကယ်၍ (fwrite (p[0], parimaa_na *p[0], len, stdout) != len)
        ပြန်ပေး -1;
    }

  ပြန်ပေး 0;
}

ကိန်းပြည့်
main (ကိန်းပြည့် argc, var_na **argv)
{
  bool echo = false;
  bool input_range = false;
  size_t lo_input = SIZE_MAX;
  size_t hi_input = 0;
  idx_t head_lines = MIN (IDX_MAX, SIZE_MAX);
  var_na const *outfile = NULL;
  var_na *random_source = NULL;
  var_na eolbyte = '\n';
  var_na **input_lines = NULL;
  bool use_reservoir_sampling = false;
  bool repeat = false;

  ကိန်းပြည့် optc;
  ကိန်းပြည့် n_operands;
  var_na **operand;
  size_t n_lines;
  var_na **line = NULL;
  sa_nracanaa linebuffer *reservoir = NULL;
  sa_nracanaa randint_source *randint_source;
  size_t *permutation = NULL;
  ကိန်းပြည့် i;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  နေစဉ် ((optc = getopt_long (argc, argv, "ei:n:o:rz", long_opts, NULL))
         != -1)
    switch (optc)
      {
      pak_sa 'e':
        echo = true;
        bha_nga;

      pak_sa 'i':
        {
          အကယ်၍ (input_range)
            error (EXIT_FAILURE, 0, _("multiple -i options specified"));
          input_range = true;

          uintmax_t u;
          var_na *lo_end;
          strtol_error err = xstrtoumax (optarg, &lo_end, 10, &u, NULL);
          အကယ်၍ (err == LONGINT_OK)
            {
              lo_input = u;
              အကယ်၍ (lo_input != u)
                err = LONGINT_OVERFLOW;
              မဟုတ်ပါက အကယ်၍ (*lo_end != '-')
                err = LONGINT_INVALID;
              မဟုတ်ပါက
                {
                  err = xstrtoumax (lo_end + 1, NULL, 10, &u, "");
                  အကယ်၍ (err == LONGINT_OK)
                    {
                      hi_input = u;
                      အကယ်၍ (hi_input != u)
                        err = LONGINT_OVERFLOW;
                    }
                }
            }

          n_lines = hi_input - lo_input + 1;

          အကယ်၍ (err != LONGINT_OK || (lo_input <= hi_input) == (n_lines == 0))
            error (EXIT_FAILURE, err == LONGINT_OVERFLOW ? EOVERFLOW : 0,
                   "%s: %s", _("invalid input range"), quote (optarg));
        }
        bha_nga;

      pak_sa 'n':
        {
          uintmax_t argval;
          strtol_error e = xstrtoumax (optarg, NULL, 10, &argval, "");

          အကယ်၍ (e == LONGINT_OK)
            head_lines = MIN (head_lines, argval);
          မဟုတ်ပါက အကယ်၍ (e != LONGINT_OVERFLOW)
            error (EXIT_FAILURE, 0, _("invalid line count: %s"),
                   quote (optarg));
        }
        bha_nga;

      pak_sa 'o':
        အကယ်၍ (outfile && !streq (outfile, optarg))
          error (EXIT_FAILURE, 0, _("multiple output files specified"));
        outfile = optarg;
        bha_nga;

      pak_sa RANDOM_SOURCE_OPTION:
        အကယ်၍ (random_source && !streq (random_source, optarg))
          error (EXIT_FAILURE, 0, _("multiple random sources specified"));
        random_source = optarg;
        bha_nga;

      pak_sa 'r':
        repeat = true;
        bha_nga;

      pak_sa 'z':
        eolbyte = '\0';
        bha_nga;

      case_GETOPT_HELP_CHAR;
      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
      default:
        usage (EXIT_FAILURE);
      }

  n_operands = argc - optind;
  operand = argv + optind;

  /* Check invalid usage.  */
  အကယ်၍ (echo && input_range)
    {
      error (0, 0, _("cannot combine -e and -i options"));
      usage (EXIT_FAILURE);
    }
  အကယ်၍ (input_range ? 0 < n_operands : !echo && 1 < n_operands)
    {
      error (0, 0, _("extra operand %s"), quote (operand[!input_range]));
      usage (EXIT_FAILURE);
    }

  /* Prepare input.  */
  အကယ်၍ (head_lines == 0)
    {
      n_lines = 0;
      line = NULL;
    }
  မဟုတ်ပါက အကယ်၍ (echo)
    {
      input_from_argv (operand, n_operands, eolbyte);
      n_lines = n_operands;
      line = operand;
    }
  မဟုတ်ပါက အကယ်၍ (input_range)
    {
      IF_LINT (n_lines = hi_input - lo_input + 1); /* Avoid GCC 10 warning.  */
      line = NULL;
    }
  မဟုတ်ပါက
    {
      /* If an input file is specified, re-open it as stdin.  */
      အကယ်၍ (n_operands == 1
          && ! (streq (operand[0], "-")
                || freopen (operand[0], "r", stdin)))
        error (EXIT_FAILURE, errno, "%s", quotef (operand[0]));

      fadvise (stdin, FADVISE_SEQUENTIAL);

      အကယ်၍ (repeat || head_lines == MIN (IDX_MAX, SIZE_MAX)
          || input_size () <= RESERVOIR_MIN_INPUT)
        {
          n_lines = read_input (stdin, eolbyte, &input_lines);
          line = input_lines;
        }
      မဟုတ်ပါက
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
  အကယ်၍ (! randint_source)
    error (EXIT_FAILURE, errno, "%s",
           quotef (random_source ? random_source : "getrandom"));

  အကယ်၍ (use_reservoir_sampling)
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
  အကယ်၍ (! (head_lines == 0 || echo || input_range || fclose (stdin) == 0))
    error (EXIT_FAILURE, errno, _("read error"));

  အကယ်၍ (!repeat)
    permutation = randperm_new (randint_source, ahead_lines, n_lines);

  အကယ်၍ (outfile && ! freopen (outfile, "w", stdout))
    error (EXIT_FAILURE, errno, "%s", quotef (outfile));

  /* Generate output according to requested method */
  အကယ်၍ (repeat)
    {
      အကယ်၍ (head_lines == 0)
        i = 0;
      မဟုတ်ပါက
        {
          အကယ်၍ (n_lines == 0)
            error (EXIT_FAILURE, 0, _("no lines to repeat"));
          အကယ်၍ (input_range)
            i = write_random_numbers (randint_source, ahead_lines,
                                      lo_input, hi_input, eolbyte);
          မဟုတ်ပါက
            i = write_random_lines (randint_source, ahead_lines, line, n_lines);
        }
    }
  မဟုတ်ပါက
    {
      အကယ်၍ (use_reservoir_sampling)
        i = write_permuted_output_reservoir (n_lines, reservoir, permutation);
      မဟုတ်ပါက အကယ်၍ (input_range)
        i = write_permuted_numbers (ahead_lines, lo_input,
                                    permutation, eolbyte);
      မဟုတ်ပါက
        i = write_permuted_lines (ahead_lines, line, permutation);
    }

  အကယ်၍ (i != 0)
    write_error ();

  IF_LINT (randint_all_free (randint_source));  /* For older valgrind.  */

  main_exit (EXIT_SUCCESS);
}
