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
enum { RESERVOIR_MIN_पासोक = 8192 * 1024 };


void
usage (int status)
{
  if (status != EXIT_SUCCESS)
    emit_try_help ();
  else
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]\n\
  or:  %s -e [OPTION]... [ARG]...\n\
  or:  %s -i LO-HI [OPTION]...\n\
"),
              program_name, program_name, program_name);
      fputs (_("\
Write a random permutation of the पासोक मगा_लिन्या to standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -e, --echo                treat each ARG as an पासोक लिन्या\n\
"));
      oputs (_("\
  -i, --पासोक-range=LO-HI   treat each नुमेरो_2 LO through HI as an पासोक लिन्या\n\
"));
      oputs (_("\
  -n, --head-बिलांग=बिलांग_2    output at most बिलांग_2 मगा_लिन्या\n\
"));
      oputs (_("\
  -o, --output=FILE         write result to FILE instead of standard output\n\
"));
      oputs (_("\
      --random-source=FILE  get random bytes from FILE\n\
"));
      oputs (_("\
  -r, --repeat              output मगा_लिन्या can be repeated\n\
"));
      oputs (_("\
  -z, --zero-terminated     लिन्या delimiter is NUL, not newline\n\
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
पासोक_from_argv (char **operand, int n_operands, char eolbyte)
{
  char *p;
  size_t size = n_operands;
  int i;

  for (i = 0; i < n_operands; i++)
    size += strlen (operand[i]);
  p = xmalloc (size);

  for (i = 0; i < n_operands; i++)
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
next_लिन्या (char *लिन्या, char eolbyte)
{
  char *p = rawmemchr (लिन्या, eolbyte);
  return p + 1;
}

/* Return the size of the input if possible or OFF_T_MAX if not.  */

static off_t
पासोक_size (void)
{
  off_t file_size;

  struct stat stat_buf;
  if (fstat (STDIN_FILENO, &stat_buf) != 0)
    return OFF_T_MAX;
  if (usable_st_size (&stat_buf))
    file_size = stat_buf.st_size;
  else
    return OFF_T_MAX;

  off_t पासोक_offset = lseek (STDIN_FILENO, 0, SEEK_CUR);
  if (पासोक_offset < 0)
    return OFF_T_MAX;

  file_size -= पासोक_offset;

  return file_size;
}

/* Read all lines and store up to K permuted lines in *OUT_RSRV.
   Return the number of lines read, up to a maximum of K.  */

static idx_t
read_पासोक_reservoir_sampling (FILE *in, char eolbyte, idx_t k,
                               struct randint_source *s,
                               struct linebuffer **out_rsrv)
{
  randint n_मगा_लिन्या = 0;
  idx_t n_alloc_मगा_लिन्या = 0;
  struct linebuffer *लिन्या = NULL;
  struct linebuffer *rsrv = NULL;

  /* Fill the first K lines, directly into the reservoir.  */
  for (n_मगा_लिन्या = 0; n_मगा_लिन्या < k; n_मगा_लिन्या++)
    {
      /* Enlarge reservoir if needed.  */
      if (n_मगा_लिन्या == n_alloc_मगा_लिन्या)
        {
          idx_t old = n_alloc_मगा_लिन्या;
          rsrv = xpalloc (rsrv, &n_alloc_मगा_लिन्या, 1, k, sizeof *rsrv);
          memset (&rsrv[n_मगा_लिन्या], 0, (n_alloc_मगा_लिन्या - old) * sizeof *rsrv);
        }

      लिन्या = readlinebuffer_delim (&rsrv[n_मगा_लिन्या], in, eolbyte);
      if (!लिन्या)
        break;
    }

  /* last line wasn't null - so there may be more lines to read.  */
  if (लिन्या != NULL)
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
          randint j = randint_choose (s, n_मगा_लिन्या + 1);  /* 0 .. n_lines.  */
          लिन्या = (j < k) ? (&rsrv[j]) : (&dummy);
        }
      while (readlinebuffer_delim (लिन्या, in, eolbyte) != NULL && n_मगा_लिन्या++);

      if (! n_मगा_लिन्या)
        error (EXIT_FAILURE, EOVERFLOW, _("too many input lines"));

      freebuffer (&dummy);
    }

  /* no more input lines, or an input error.  */
  if (ferror (in))
    error (EXIT_FAILURE, errno, _("read error"));

  *out_rsrv = rsrv;
  return MIN (k, n_मगा_लिन्या);
}

static int
write_permuted_output_reservoir (size_t n_मगा_लिन्या, struct linebuffer *मगा_लिन्या,
                                 size_t const *permutation)
{
  for (size_t i = 0; i < n_मगा_लिन्या; i++)
    {
      const struct linebuffer *p = &मगा_लिन्या[permutation[i]];
      if (fwrite (p->buffer, sizeof (char), p->length, stdout) != p->length)
        return -1;
    }

  return 0;
}

/* Read data from file IN.  Input lines are delimited by EOLBYTE;
   silently append a trailing EOLBYTE if the file ends in some other
   byte.  Store a pointer to the resulting array of lines into *PLINE.
   Return the number of lines read.  Report an error and exit on
   failure.  */

static size_t
read_पासोक (FILE *in, char eolbyte, char ***pline)
{
  char *p;
  char *buf = NULL;
  size_t used;
  char *lim;
  char **लिन्या;
  size_t n_मगा_लिन्या;

  /* TODO: We should limit the amount of data read here,
     to less than RESERVOIR_MIN_INPUT.  I.e., adjust fread_file() to support
     taking a byte limit.  We'd then need to ensure we handle a line spanning
     this boundary.  With that in place we could set use_reservoir_sampling
     when used==RESERVOIR_MIN_INPUT, and have read_input_reservoir_sampling()
     call a wrapper function to populate a linebuffer from the internal pline
     or if none left, stdin.  Doing that would give better performance by
     avoiding the reservoir CPU overhead when reading < RESERVOIR_MIN_INPUT
     from a pipe, and allow us to dispense with the input_size() function.  */
  if (!(buf = fread_file (in, 0, &used)))
    error (EXIT_FAILURE, errno, _("read error"));

  if (used && buf[used - 1] != eolbyte)
    buf[used++] = eolbyte;

  lim = buf + used;

  n_मगा_लिन्या = 0;
  for (p = buf; p < lim; p = next_लिन्या (p, eolbyte))
    n_मगा_लिन्या++;

  *pline = लिन्या = xnmalloc (n_मगा_लिन्या + 1, sizeof *लिन्या);

  लिन्या[0] = p = buf;
  for (size_t i = 1; i <= n_मगा_लिन्या; i++)
    लिन्या[i] = p = next_लिन्या (p, eolbyte);

  return n_मगा_लिन्या;
}

/* Output N_LINES lines to stdout from LINE array,
   chosen by the indices in PERMUTATION.
   PERMUTATION and LINE must have at least N_LINES elements.
   Strings in LINE must include the line-terminator character.  */
static int
write_permuted_मगा_लिन्या (size_t n_मगा_लिन्या, char *const *लिन्या,
                      size_t const *permutation)
{
  for (size_t i = 0; i < n_मगा_लिन्या; i++)
    {
      char *const *p = लिन्या + permutation[i];
      size_t len = p[1] - p[0];
      if (fwrite (p[0], sizeof *p[0], len, stdout) != len)
        return -1;
    }

  return 0;
}

/* Print NUMBER followed by EOLBYTE to standard output.
   Return false on failure, true on success.  */
static bool
इलिम्बाग_नुमेरो (unsigned long int नुमेरो_2, char eolbyte)
{
  char buf[INT_BUFSIZE_BOUND (unsigned long int)];
  char *p = buf + INT_STRLEN_BOUND (unsigned long int);
  *p = eolbyte;
  do
    *--p = '0' + नुमेरो_2 % 10;
  while ((नुमेरो_2 /= 10) != 0);
  idx_t len = buf + sizeof buf - p;
  return fwrite (p, 1, len, stdout) == len;
}

/* Output N_LINES of numbers to stdout, from PERMUTATION array.
   PERMUTATION must have at least N_LINES elements.  */
static int
write_permuted_numbers (size_t n_मगा_लिन्या, size_t lo_पासोक,
                        size_t const *permutation, char eolbyte)
{
  for (size_t i = 0; i < n_मगा_लिन्या; i++)
    {
      unsigned long int n = lo_पासोक + permutation[i];
      if (! इलिम्बाग_नुमेरो (n, eolbyte))
        return -1;
    }

  return 0;
}

/* Output COUNT numbers to stdout, chosen randomly from range
   LO_INPUT through HI_INPUT.  */
static int
write_random_numbers (struct randint_source *s, size_t बिलांग,
                      size_t lo_पासोक, size_t hi_पासोक, char eolbyte)
{
  const randint range = hi_पासोक - lo_पासोक + 1;

  for (size_t i = 0; i < बिलांग; i++)
    {
      unsigned long int j = lo_पासोक + randint_choose (s, range);
      if (! इलिम्बाग_नुमेरो (j, eolbyte))
        return -1;
    }

  return 0;
}

/* Output COUNT lines to stdout from LINES array.
   LINES must have at least N_LINES elements in it.
   Strings in LINES_ must include the line-terminator character.  */
static int
write_random_मगा_लिन्या (struct randint_source *s, size_t बिलांग,
                    char *const *मगा_लिन्या, size_t n_मगा_लिन्या)
{
  for (size_t i = 0; i < बिलांग; i++)
    {
      const randint j = randint_choose (s, n_मगा_लिन्या);
      char *const *p = मगा_लिन्या + j;
      size_t len = p[1] - p[0];
      if (fwrite (p[0], sizeof *p[0], len, stdout) != len)
        return -1;
    }

  return 0;
}

int
main (int argc, char **argv)
{
  bool echo = false;
  bool पासोक_range = false;
  size_t lo_पासोक = SIZE_MAX;
  size_t hi_पासोक = 0;
  idx_t head_मगा_लिन्या = MIN (IDX_MAX, SIZE_MAX);
  char const *outfile = NULL;
  char *random_source = NULL;
  char eolbyte = '\n';
  char **पासोक_मगा_लिन्या = NULL;
  bool use_reservoir_sampling = false;
  bool repeat = false;

  int optc;
  int n_operands;
  char **operand;
  size_t n_मगा_लिन्या;
  char **लिन्या = NULL;
  struct linebuffer *reservoir = NULL;
  struct randint_source *randint_source;
  size_t *permutation = NULL;
  int i;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  while ((optc = getopt_long (argc, argv, "ei:n:o:rz", long_opts, NULL))
         != -1)
    switch (optc)
      {
      case 'e':
        echo = true;
        break;

      case 'i':
        {
          if (पासोक_range)
            error (EXIT_FAILURE, 0, _("multiple -i options specified"));
          पासोक_range = true;

          uintmax_t u;
          char *lo_end;
          strtol_error err = xstrtoumax (optarg, &lo_end, 10, &u, NULL);
          if (err == LONGINT_OK)
            {
              lo_पासोक = u;
              if (lo_पासोक != u)
                err = LONGINT_OVERFLOW;
              else if (*lo_end != '-')
                err = LONGINT_INVALID;
              else
                {
                  err = xstrtoumax (lo_end + 1, NULL, 10, &u, "");
                  if (err == LONGINT_OK)
                    {
                      hi_पासोक = u;
                      if (hi_पासोक != u)
                        err = LONGINT_OVERFLOW;
                    }
                }
            }

          n_मगा_लिन्या = hi_पासोक - lo_पासोक + 1;

          if (err != LONGINT_OK || (lo_पासोक <= hi_पासोक) == (n_मगा_लिन्या == 0))
            error (EXIT_FAILURE, err == LONGINT_OVERFLOW ? EOVERFLOW : 0,
                   "%s: %s", _("invalid input range"), quote (optarg));
        }
        break;

      case 'n':
        {
          uintmax_t argval;
          strtol_error e = xstrtoumax (optarg, NULL, 10, &argval, "");

          if (e == LONGINT_OK)
            head_मगा_लिन्या = MIN (head_मगा_लिन्या, argval);
          else if (e != LONGINT_OVERFLOW)
            error (EXIT_FAILURE, 0, _("invalid line count: %s"),
                   quote (optarg));
        }
        break;

      case 'o':
        if (outfile && !streq (outfile, optarg))
          error (EXIT_FAILURE, 0, _("multiple output files specified"));
        outfile = optarg;
        break;

      case RANDOM_SOURCE_OPTION:
        if (random_source && !streq (random_source, optarg))
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
  if (echo && पासोक_range)
    {
      error (0, 0, _("cannot combine -e and -i options"));
      usage (EXIT_FAILURE);
    }
  if (पासोक_range ? 0 < n_operands : !echo && 1 < n_operands)
    {
      error (0, 0, _("extra operand %s"), quote (operand[!पासोक_range]));
      usage (EXIT_FAILURE);
    }

  /* Prepare input.  */
  if (head_मगा_लिन्या == 0)
    {
      n_मगा_लिन्या = 0;
      लिन्या = NULL;
    }
  else if (echo)
    {
      पासोक_from_argv (operand, n_operands, eolbyte);
      n_मगा_लिन्या = n_operands;
      लिन्या = operand;
    }
  else if (पासोक_range)
    {
      IF_LINT (n_मगा_लिन्या = hi_पासोक - lo_पासोक + 1); /* Avoid GCC 10 warning.  */
      लिन्या = NULL;
    }
  else
    {
      /* If an input file is specified, re-open it as stdin.  */
      if (n_operands == 1
          && ! (streq (operand[0], "-")
                || freopen (operand[0], "r", stdin)))
        error (EXIT_FAILURE, errno, "%s", quotef (operand[0]));

      fadvise (stdin, FADVISE_SEQUENTIAL);

      if (repeat || head_मगा_लिन्या == MIN (IDX_MAX, SIZE_MAX)
          || पासोक_size () <= RESERVOIR_MIN_पासोक)
        {
          n_मगा_लिन्या = read_पासोक (stdin, eolbyte, &पासोक_मगा_लिन्या);
          लिन्या = पासोक_मगा_लिन्या;
        }
      else
        {
          use_reservoir_sampling = true;
          n_मगा_लिन्या = SIZE_MAX;   /* unknown number of input lines, for now.  */
        }
    }

  /* The adjusted head line count; can be less than HEAD_LINES if the
     input is small and if not repeating.  */
  idx_t ahead_मगा_लिन्या = repeat || head_मगा_लिन्या < n_मगा_लिन्या ? head_मगा_लिन्या : n_मगा_लिन्या;

  randint_source = randint_all_new (random_source,
                                    (use_reservoir_sampling || repeat
                                     ? SIZE_MAX
                                     : randperm_bound (ahead_मगा_लिन्या, n_मगा_लिन्या)));
  if (! randint_source)
    error (EXIT_FAILURE, errno, "%s",
           quotef (random_source ? random_source : "getrandom"));

  if (use_reservoir_sampling)
    {
      /* Instead of reading the entire file into 'line',
         use reservoir-sampling to store just AHEAD_LINES random lines.  */
      n_मगा_लिन्या = read_पासोक_reservoir_sampling (stdin, eolbyte, ahead_मगा_लिन्या,
                                               randint_source, &reservoir);
      ahead_मगा_लिन्या = n_मगा_लिन्या;
    }

  /* Close stdin now, rather than earlier, so that randint_all_new
     doesn't have to worry about opening something other than
     stdin.  */
  if (! (head_मगा_लिन्या == 0 || echo || पासोक_range || fclose (stdin) == 0))
    error (EXIT_FAILURE, errno, _("read error"));

  if (!repeat)
    permutation = randperm_new (randint_source, ahead_मगा_लिन्या, n_मगा_लिन्या);

  if (outfile && ! freopen (outfile, "w", stdout))
    error (EXIT_FAILURE, errno, "%s", quotef (outfile));

  /* Generate output according to requested method */
  if (repeat)
    {
      if (head_मगा_लिन्या == 0)
        i = 0;
      else
        {
          if (n_मगा_लिन्या == 0)
            error (EXIT_FAILURE, 0, _("no lines to repeat"));
          if (पासोक_range)
            i = write_random_numbers (randint_source, ahead_मगा_लिन्या,
                                      lo_पासोक, hi_पासोक, eolbyte);
          else
            i = write_random_मगा_लिन्या (randint_source, ahead_मगा_लिन्या, लिन्या, n_मगा_लिन्या);
        }
    }
  else
    {
      if (use_reservoir_sampling)
        i = write_permuted_output_reservoir (n_मगा_लिन्या, reservoir, permutation);
      else if (पासोक_range)
        i = write_permuted_numbers (ahead_मगा_लिन्या, lo_पासोक,
                                    permutation, eolbyte);
      else
        i = write_permuted_मगा_लिन्या (ahead_मगा_लिन्या, लिन्या, permutation);
    }

  if (i != 0)
    write_error ();

  IF_LINT (randint_all_free (randint_source));  /* For older valgrind.  */

  main_exit (EXIT_SUCCESS);
}
