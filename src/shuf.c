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

#ضمن <config.h>

#ضمن <sys/types.h>
#ضمن <getopt.h>
#ضمن "system.h"

#ضمن "fadvise.h"
#ضمن "linebuffer.h"
#ضمن "quote.h"
#ضمن "randint.h"
#ضمن "randperm.h"
#ضمن "read-file.h"
#ضمن "stdio--.h"
#ضمن "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "shuf"

#define AUTHORS proper_name ("Paul Eggert")

/* reservoir-sampling introduces CPU overhead for small inputs.
   So only enable it for inputs >= this limit.
   This limit was determined using these commands:
     $ for p in $(seq 7); do src/seq $((10**$p)) > 10p$p.in; done
     $ for p in $(seq 7); do time shuf-nores -n10 10p$p.in >/dev/null; done
     $ for p in $(seq 7); do time shuf -n10 10p$p.in >/dev/null; done  .*/
تعداد { RESERVOIR_MIN_ماسوقن = 8192 * 1024 };


فراغ
usage (صحيح status)
{
  إذا (status != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]\n\
  or:  %s -e [OPTION]... [ARG]...\n\
  or:  %s -i LO-HI [OPTION]...\n\
"),
              program_name, program_name, program_name);
      fputs (_("\
Write a random permutation of the ماسوقن باريس_سموا to standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -e, --echo                treat each ARG as an ماسوقن باريس\n\
"));
      oputs (_("\
  -i, --ماسوقن-range=LO-HI   treat each نومبور LO through HI as an ماسوقن باريس\n\
"));
      oputs (_("\
  -n, --head-کيراءن=کيراءن_2    output at most کيراءن_2 باريس_سموا\n\
"));
      oputs (_("\
  -o, --output=FILE         اكتب result to FILE instead of standard output\n\
"));
      oputs (_("\
      --random-source=FILE  get random bytes from FILE\n\
"));
      oputs (_("\
  -r, --repeat              output باريس_سموا can be repeated\n\
"));
      oputs (_("\
  -z, --zero-terminated     باريس delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }

  exit (status);
}

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
تعداد
{
  RANDOM_SOURCE_OPTION = CHAR_MAX + 1
};

ساكن بنية option ثابت long_opts[] =
{
  {"echo", no_argument, عدم, 'e'},
  {"input-range", required_argument, عدم, 'i'},
  {"head-count", required_argument, عدم, 'n'},
  {"output", required_argument, عدم, 'o'},
  {"random-source", required_argument, عدم, RANDOM_SOURCE_OPTION},
  {"repeat", no_argument, عدم, 'r'},
  {"zero-terminated", no_argument, عدم, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {عدم, 0, عدم, 0},
};

ساكن فراغ
ماسوقن_from_argv (حرف **operand, صحيح n_operands, حرف eolbyte)
{
  حرف *p;
  size_t size = n_operands;
  صحيح i;

  لكل (i = 0; i < n_operands; i++)
    size += strlen (operand[i]);
  p = xmalloc (size);

  لكل (i = 0; i < n_operands; i++)
    {
      حرف *p1 = stpcpy (p, operand[i]);
      operand[i] = p;
      p = p1;
      *p++ = eolbyte;
    }

  operand[n_operands] = p;
}

/* Return the start of the next line after LINE, which is guaranteed
   to end in EOLBYTE.  */

ساكن حرف *
next_باريس (حرف *باريس, حرف eolbyte)
{
  حرف *p = rawmemchr (باريس, eolbyte);
  أرجع p + 1;
}

/* Return the size of the input if possible or OFF_T_MAX if not.  */

ساكن off_t
ماسوقن_size (فراغ)
{
  off_t file_size;

  بنية stat stat_buf;
  إذا (fstat (STDIN_FILENO, &stat_buf) != 0)
    أرجع OFF_T_MAX;
  إذا (usable_st_size (&stat_buf))
    file_size = stat_buf.st_size;
  وإلا
    أرجع OFF_T_MAX;

  off_t ماسوقن_offset = lseek (STDIN_FILENO, 0, SEEK_CUR);
  إذا (ماسوقن_offset < 0)
    أرجع OFF_T_MAX;

  file_size -= ماسوقن_offset;

  أرجع file_size;
}

/* Read all lines and store up to K permuted lines in *OUT_RSRV.
   Return the number of lines read, up to a maximum of K.  */

ساكن idx_t
read_ماسوقن_reservoir_sampling (FILE *in, حرف eolbyte, idx_t k,
                               بنية randint_source *s,
                               بنية linebuffer **out_rsrv)
{
  randint n_باريس_سموا = 0;
  idx_t n_alloc_باريس_سموا = 0;
  بنية linebuffer *باريس = عدم;
  بنية linebuffer *rsrv = عدم;

  /* Fill the first K lines, directly into the reservoir.  */
  لكل (n_باريس_سموا = 0; n_باريس_سموا < k; n_باريس_سموا++)
    {
      /* Enlarge reservoir if needed.  */
      إذا (n_باريس_سموا == n_alloc_باريس_سموا)
        {
          idx_t old = n_alloc_باريس_سموا;
          rsrv = xpalloc (rsrv, &n_alloc_باريس_سموا, 1, k, حجم *rsrv);
          memset (&rsrv[n_باريس_سموا], 0, (n_alloc_باريس_سموا - old) * حجم *rsrv);
        }

      باريس = readlinebuffer_delim (&rsrv[n_باريس_سموا], in, eolbyte);
      إذا (!باريس)
        اكسر;
    }

  /* last line wasn't null - so there may be more lines to read.  */
  إذا (باريس != عدم)
    {
      بنية linebuffer dummy;
      initbuffer (&dummy);  /* space for lines not put in reservoir.  */

      /* Choose the fate of the next line, with decreasing probability (as
         n_lines increases in size).

         If the line will be used, store it directly in the reservoir.
         Otherwise, store it in dummy space.

         With 'struct linebuffer', storing into existing buffer will reduce
         re-allocations (will only re-allocate if the new line is longer than
         the currently allocated space).  */
      افعل
        {
          randint j = randint_choose (s, n_باريس_سموا + 1);  /* 0 .. n_lines.  */
          باريس = (j < k) ? (&rsrv[j]) : (&dummy);
        }
      طالما (readlinebuffer_delim (باريس, in, eolbyte) != عدم && n_باريس_سموا++);

      إذا (! n_باريس_سموا)
        error (EXIT_FAILURE, EOVERFLOW, _("too many input lines"));

      freebuffer (&dummy);
    }

  /* no more input lines, or an input error.  */
  إذا (ferror (in))
    error (EXIT_FAILURE, errno, _("read error"));

  *out_rsrv = rsrv;
  أرجع MIN (k, n_باريس_سموا);
}

ساكن صحيح
write_permuted_output_reservoir (size_t n_باريس_سموا, بنية linebuffer *باريس_سموا,
                                 size_t ثابت *permutation)
{
  لكل (size_t i = 0; i < n_باريس_سموا; i++)
    {
      ثابت بنية linebuffer *p = &باريس_سموا[permutation[i]];
      إذا (fwrite (p->buffer, حجم (حرف), p->length, stdout) != p->length)
        أرجع -1;
    }

  أرجع 0;
}

/* Read data from file IN.  Input lines are delimited by EOLBYTE;
   silently append a trailing EOLBYTE if the file ends in some other
   byte.  Store a pointer to the resulting array of lines into *PLINE.
   Return the number of lines read.  Report an error and exit on
   failure.  */

ساكن size_t
read_ماسوقن (FILE *in, حرف eolbyte, حرف ***pline)
{
  حرف *p;
  حرف *buf = عدم;
  size_t used;
  حرف *lim;
  حرف **باريس;
  size_t n_باريس_سموا;

  /* TODO: We should limit the amount of data read here,
     to less than RESERVOIR_MIN_INPUT.  I.e., adjust fread_file() to support
     taking a byte limit.  We'd then need to ensure we handle a line spanning
     this boundary.  With that in place we could set use_reservoir_sampling
     when used==RESERVOIR_MIN_INPUT, and have read_input_reservoir_sampling()
     call a wrapper function to populate a linebuffer from the internal pline
     or if none left, stdin.  Doing that would give better performance by
     avoiding the reservoir CPU overhead when reading < RESERVOIR_MIN_INPUT
     from a pipe, and allow us to dispense with the input_size() function.  */
  إذا (!(buf = fread_file (in, 0, &used)))
    error (EXIT_FAILURE, errno, _("read error"));

  إذا (used && buf[used - 1] != eolbyte)
    buf[used++] = eolbyte;

  lim = buf + used;

  n_باريس_سموا = 0;
  لكل (p = buf; p < lim; p = next_باريس (p, eolbyte))
    n_باريس_سموا++;

  *pline = باريس = xnmalloc (n_باريس_سموا + 1, حجم *باريس);

  باريس[0] = p = buf;
  لكل (size_t i = 1; i <= n_باريس_سموا; i++)
    باريس[i] = p = next_باريس (p, eolbyte);

  أرجع n_باريس_سموا;
}

/* Output N_LINES lines to stdout from LINE array,
   chosen by the indices in PERMUTATION.
   PERMUTATION and LINE must have at least N_LINES elements.
   Strings in LINE must include the line-terminator character.  */
ساكن صحيح
write_permuted_باريس_سموا (size_t n_باريس_سموا, حرف *ثابت *باريس,
                      size_t ثابت *permutation)
{
  لكل (size_t i = 0; i < n_باريس_سموا; i++)
    {
      حرف *ثابت *p = باريس + permutation[i];
      size_t len = p[1] - p[0];
      إذا (fwrite (p[0], حجم *p[0], len, stdout) != len)
        أرجع -1;
    }

  أرجع 0;
}

/* Print NUMBER followed by EOLBYTE to standard output.
   Return false on failure, true on success.  */
ساكن bool
print_نومبور (unsigned long صحيح نومبور, حرف eolbyte)
{
  حرف buf[INT_BUFSIZE_BOUND (unsigned long صحيح)];
  حرف *p = buf + INT_STRLEN_BOUND (unsigned long صحيح);
  *p = eolbyte;
  افعل
    *--p = '0' + نومبور % 10;
  طالما ((نومبور /= 10) != 0);
  idx_t len = buf + حجم buf - p;
  أرجع fwrite (p, 1, len, stdout) == len;
}

/* Output N_LINES of numbers to stdout, from PERMUTATION array.
   PERMUTATION must have at least N_LINES elements.  */
ساكن صحيح
write_permuted_numbers (size_t n_باريس_سموا, size_t lo_ماسوقن,
                        size_t ثابت *permutation, حرف eolbyte)
{
  لكل (size_t i = 0; i < n_باريس_سموا; i++)
    {
      unsigned long صحيح n = lo_ماسوقن + permutation[i];
      إذا (! print_نومبور (n, eolbyte))
        أرجع -1;
    }

  أرجع 0;
}

/* Output COUNT numbers to stdout, chosen randomly from range
   LO_INPUT through HI_INPUT.  */
ساكن صحيح
write_random_numbers (بنية randint_source *s, size_t کيراءن,
                      size_t lo_ماسوقن, size_t hi_ماسوقن, حرف eolbyte)
{
  ثابت randint range = hi_ماسوقن - lo_ماسوقن + 1;

  لكل (size_t i = 0; i < کيراءن; i++)
    {
      unsigned long صحيح j = lo_ماسوقن + randint_choose (s, range);
      إذا (! print_نومبور (j, eolbyte))
        أرجع -1;
    }

  أرجع 0;
}

/* Output COUNT lines to stdout from LINES array.
   LINES must have at least N_LINES elements in it.
   Strings in LINES_ must include the line-terminator character.  */
ساكن صحيح
write_random_باريس_سموا (بنية randint_source *s, size_t کيراءن,
                    حرف *ثابت *باريس_سموا, size_t n_باريس_سموا)
{
  لكل (size_t i = 0; i < کيراءن; i++)
    {
      ثابت randint j = randint_choose (s, n_باريس_سموا);
      حرف *ثابت *p = باريس_سموا + j;
      size_t len = p[1] - p[0];
      إذا (fwrite (p[0], حجم *p[0], len, stdout) != len)
        أرجع -1;
    }

  أرجع 0;
}

صحيح
main (صحيح argc, حرف **argv)
{
  bool echo = false;
  bool ماسوقن_range = false;
  size_t lo_ماسوقن = SIZE_MAX;
  size_t hi_ماسوقن = 0;
  idx_t head_باريس_سموا = MIN (IDX_MAX, SIZE_MAX);
  حرف ثابت *outfile = عدم;
  حرف *random_source = عدم;
  حرف eolbyte = '\n';
  حرف **ماسوقن_باريس_سموا = عدم;
  bool use_reservoir_sampling = false;
  bool repeat = false;

  صحيح optc;
  صحيح n_operands;
  حرف **operand;
  size_t n_باريس_سموا;
  حرف **باريس = عدم;
  بنية linebuffer *reservoir = عدم;
  بنية randint_source *randint_source;
  size_t *permutation = عدم;
  صحيح i;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  طالما ((optc = getopt_long (argc, argv, "ei:n:o:rz", long_opts, عدم))
         != -1)
    بدل (optc)
      {
      حالة 'e':
        echo = true;
        اكسر;

      حالة 'i':
        {
          إذا (ماسوقن_range)
            error (EXIT_FAILURE, 0, _("multiple -i options specified"));
          ماسوقن_range = true;

          uintmax_t u;
          حرف *lo_end;
          strtol_error err = xstrtoumax (optarg, &lo_end, 10, &u, عدم);
          إذا (err == LONGINT_OK)
            {
              lo_ماسوقن = u;
              إذا (lo_ماسوقن != u)
                err = LONGINT_OVERFLOW;
              وإلا إذا (*lo_end != '-')
                err = LONGINT_INVALID;
              وإلا
                {
                  err = xstrtoumax (lo_end + 1, عدم, 10, &u, "");
                  إذا (err == LONGINT_OK)
                    {
                      hi_ماسوقن = u;
                      إذا (hi_ماسوقن != u)
                        err = LONGINT_OVERFLOW;
                    }
                }
            }

          n_باريس_سموا = hi_ماسوقن - lo_ماسوقن + 1;

          إذا (err != LONGINT_OK || (lo_ماسوقن <= hi_ماسوقن) == (n_باريس_سموا == 0))
            error (EXIT_FAILURE, err == LONGINT_OVERFLOW ? EOVERFLOW : 0,
                   "%s: %s", _("invalid input range"), quote (optarg));
        }
        اكسر;

      حالة 'n':
        {
          uintmax_t argval;
          strtol_error e = xstrtoumax (optarg, عدم, 10, &argval, "");

          إذا (e == LONGINT_OK)
            head_باريس_سموا = MIN (head_باريس_سموا, argval);
          وإلا إذا (e != LONGINT_OVERFLOW)
            error (EXIT_FAILURE, 0, _("invalid line count: %s"),
                   quote (optarg));
        }
        اكسر;

      حالة 'o':
        إذا (outfile && !streq (outfile, optarg))
          error (EXIT_FAILURE, 0, _("multiple output files specified"));
        outfile = optarg;
        اكسر;

      حالة RANDOM_SOURCE_OPTION:
        إذا (random_source && !streq (random_source, optarg))
          error (EXIT_FAILURE, 0, _("multiple random sources specified"));
        random_source = optarg;
        اكسر;

      حالة 'r':
        repeat = true;
        اكسر;

      حالة 'z':
        eolbyte = '\0';
        اكسر;

      case_GETOPT_HELP_CHAR;
      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
      افتراضي:
        usage (EXIT_FAILURE);
      }

  n_operands = argc - optind;
  operand = argv + optind;

  /* Check invalid usage.  */
  إذا (echo && ماسوقن_range)
    {
      error (0, 0, _("cannot combine -e and -i options"));
      usage (EXIT_FAILURE);
    }
  إذا (ماسوقن_range ? 0 < n_operands : !echo && 1 < n_operands)
    {
      error (0, 0, _("extra operand %s"), quote (operand[!ماسوقن_range]));
      usage (EXIT_FAILURE);
    }

  /* Prepare input.  */
  إذا (head_باريس_سموا == 0)
    {
      n_باريس_سموا = 0;
      باريس = عدم;
    }
  وإلا إذا (echo)
    {
      ماسوقن_from_argv (operand, n_operands, eolbyte);
      n_باريس_سموا = n_operands;
      باريس = operand;
    }
  وإلا إذا (ماسوقن_range)
    {
      IF_LINT (n_باريس_سموا = hi_ماسوقن - lo_ماسوقن + 1); /* Avoid GCC 10 warning.  */
      باريس = عدم;
    }
  وإلا
    {
      /* If an input file is specified, re-open it as stdin.  */
      إذا (n_operands == 1
          && ! (streq (operand[0], "-")
                || freopen (operand[0], "r", stdin)))
        error (EXIT_FAILURE, errno, "%s", quotef (operand[0]));

      fadvise (stdin, FADVISE_SEQUENTIAL);

      إذا (repeat || head_باريس_سموا == MIN (IDX_MAX, SIZE_MAX)
          || ماسوقن_size () <= RESERVOIR_MIN_ماسوقن)
        {
          n_باريس_سموا = read_ماسوقن (stdin, eolbyte, &ماسوقن_باريس_سموا);
          باريس = ماسوقن_باريس_سموا;
        }
      وإلا
        {
          use_reservoir_sampling = true;
          n_باريس_سموا = SIZE_MAX;   /* unknown number of input lines, for now.  */
        }
    }

  /* The adjusted head line count; can be less than HEAD_LINES if the
     input is small and if not repeating.  */
  idx_t ahead_باريس_سموا = repeat || head_باريس_سموا < n_باريس_سموا ? head_باريس_سموا : n_باريس_سموا;

  randint_source = randint_all_new (random_source,
                                    (use_reservoir_sampling || repeat
                                     ? SIZE_MAX
                                     : randperm_bound (ahead_باريس_سموا, n_باريس_سموا)));
  إذا (! randint_source)
    error (EXIT_FAILURE, errno, "%s",
           quotef (random_source ? random_source : "getrandom"));

  إذا (use_reservoir_sampling)
    {
      /* Instead of reading the entire file into 'line',
         use reservoir-sampling to store just AHEAD_LINES random lines.  */
      n_باريس_سموا = read_ماسوقن_reservoir_sampling (stdin, eolbyte, ahead_باريس_سموا,
                                               randint_source, &reservoir);
      ahead_باريس_سموا = n_باريس_سموا;
    }

  /* Close stdin now, rather than earlier, so that randint_all_new
     doesn't have to worry about opening something other than
     stdin.  */
  إذا (! (head_باريس_سموا == 0 || echo || ماسوقن_range || fclose (stdin) == 0))
    error (EXIT_FAILURE, errno, _("read error"));

  إذا (!repeat)
    permutation = randperm_new (randint_source, ahead_باريس_سموا, n_باريس_سموا);

  إذا (outfile && ! freopen (outfile, "w", stdout))
    error (EXIT_FAILURE, errno, "%s", quotef (outfile));

  /* Generate output according to requested method */
  إذا (repeat)
    {
      إذا (head_باريس_سموا == 0)
        i = 0;
      وإلا
        {
          إذا (n_باريس_سموا == 0)
            error (EXIT_FAILURE, 0, _("no lines to repeat"));
          إذا (ماسوقن_range)
            i = write_random_numbers (randint_source, ahead_باريس_سموا,
                                      lo_ماسوقن, hi_ماسوقن, eolbyte);
          وإلا
            i = write_random_باريس_سموا (randint_source, ahead_باريس_سموا, باريس, n_باريس_سموا);
        }
    }
  وإلا
    {
      إذا (use_reservoir_sampling)
        i = write_permuted_output_reservoir (n_باريس_سموا, reservoir, permutation);
      وإلا إذا (ماسوقن_range)
        i = write_permuted_numbers (ahead_باريس_سموا, lo_ماسوقن,
                                    permutation, eolbyte);
      وإلا
        i = write_permuted_باريس_سموا (ahead_باريس_سموا, باريس, permutation);
    }

  إذا (i != 0)
    write_error ();

  IF_LINT (randint_all_free (randint_source));  /* For older valgrind.  */

  main_exit (EXIT_SUCCESS);
}
