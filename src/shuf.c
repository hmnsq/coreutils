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

#شامل <config.h>

#شامل <sys/types.h>
#شامل <getopt.h>
#شامل "system.h"

#شامل "fadvise.h"
#شامل "linebuffer.h"
#شامل "quote.h"
#شامل "randint.h"
#شامل "randperm.h"
#شامل "read-file.h"
#شامل "stdio--.h"
#شامل "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "shuf"

#define AUTHORS proper_name ("Paul Eggert")

/* reservoir-sampling introduces CPU overhead for small inputs.
   So only enable it for inputs >= this limit.
   This limit was determined using these commands:
     $ for p in $(seq 7); do src/seq $((10**$p)) > 10p$p.in; done
     $ for p in $(seq 7); do time shuf-nores -n10 10p$p.in >/dev/null; done
     $ for p in $(seq 7); do time shuf -n10 10p$p.in >/dev/null; done  .*/
شمارش { RESERVOIR_MIN_درون_داد = 8192 * 1024 };


تهی
usage (صحیح status)
{
  اگر (status != EXIT_SUCCESS)
    emit_try_help ();
  وگرنه
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]\n\
  or:  %s -e [OPTION]... [ARG]...\n\
  or:  %s -i LO-HI [OPTION]...\n\
"),
              program_name, program_name, program_name);
      fputs (_("\
Write a random permutation of the درون_داد سطرها to standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -e, --echo                treat each ARG as an درون_داد سطر\n\
"));
      oputs (_("\
  -i, --درون_داد-range=LO-HI   treat each عدد LO through HI as an درون_داد سطر\n\
"));
      oputs (_("\
  -n, --head-شمار=شمار_2    output at most شمار_2 سطرها\n\
"));
      oputs (_("\
  -o, --output=FILE         اكتب result to FILE instead of standard output\n\
"));
      oputs (_("\
      --random-source=FILE  get random bytes from FILE\n\
"));
      oputs (_("\
  -r, --repeat              output سطرها can be repeated\n\
"));
      oputs (_("\
  -z, --zero-terminated     سطر delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }

  exit (status);
}

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
شمارش
{
  RANDOM_SOURCE_OPTION = CHAR_MAX + 1
};

ایستا ساختار option ثابت long_opts[] =
{
  {"echo", no_argument, پوچ, 'e'},
  {"input-range", required_argument, پوچ, 'i'},
  {"head-count", required_argument, پوچ, 'n'},
  {"output", required_argument, پوچ, 'o'},
  {"random-source", required_argument, پوچ, RANDOM_SOURCE_OPTION},
  {"repeat", no_argument, پوچ, 'r'},
  {"zero-terminated", no_argument, پوچ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {پوچ, 0, پوچ, 0},
};

ایستا تهی
درون_داد_from_argv (نویسه **operand, صحیح n_operands, نویسه eolbyte)
{
  نویسه *p;
  size_t size = n_operands;
  صحیح i;

  برای (i = 0; i < n_operands; i++)
    size += strlen (operand[i]);
  p = xmalloc (size);

  برای (i = 0; i < n_operands; i++)
    {
      نویسه *p1 = stpcpy (p, operand[i]);
      operand[i] = p;
      p = p1;
      *p++ = eolbyte;
    }

  operand[n_operands] = p;
}

/* Return the start of the next line after LINE, which is guaranteed
   to end in EOLBYTE.  */

ایستا نویسه *
next_سطر (نویسه *سطر, نویسه eolbyte)
{
  نویسه *p = rawmemchr (سطر, eolbyte);
  بازگشت p + 1;
}

/* Return the size of the input if possible or OFF_T_MAX if not.  */

ایستا off_t
درون_داد_size (تهی)
{
  off_t file_size;

  ساختار stat stat_buf;
  اگر (fstat (STDIN_FILENO, &stat_buf) != 0)
    بازگشت OFF_T_MAX;
  اگر (usable_st_size (&stat_buf))
    file_size = stat_buf.st_size;
  وگرنه
    بازگشت OFF_T_MAX;

  off_t درون_داد_offset = lseek (STDIN_FILENO, 0, SEEK_CUR);
  اگر (درون_داد_offset < 0)
    بازگشت OFF_T_MAX;

  file_size -= درون_داد_offset;

  بازگشت file_size;
}

/* Read all lines and store up to K permuted lines in *OUT_RSRV.
   Return the number of lines read, up to a maximum of K.  */

ایستا idx_t
read_درون_داد_reservoir_sampling (FILE *in, نویسه eolbyte, idx_t k,
                               ساختار randint_source *s,
                               ساختار linebuffer **out_rsrv)
{
  randint n_سطرها = 0;
  idx_t n_alloc_سطرها = 0;
  ساختار linebuffer *سطر = پوچ;
  ساختار linebuffer *rsrv = پوچ;

  /* Fill the first K lines, directly into the reservoir.  */
  برای (n_سطرها = 0; n_سطرها < k; n_سطرها++)
    {
      /* Enlarge reservoir if needed.  */
      اگر (n_سطرها == n_alloc_سطرها)
        {
          idx_t old = n_alloc_سطرها;
          rsrv = xpalloc (rsrv, &n_alloc_سطرها, 1, k, اندازه *rsrv);
          memset (&rsrv[n_سطرها], 0, (n_alloc_سطرها - old) * اندازه *rsrv);
        }

      سطر = readlinebuffer_delim (&rsrv[n_سطرها], in, eolbyte);
      اگر (!سطر)
        بشکن;
    }

  /* last line wasn't null - so there may be more lines to read.  */
  اگر (سطر != پوچ)
    {
      ساختار linebuffer dummy;
      initbuffer (&dummy);  /* space for lines not put in reservoir.  */

      /* Choose the fate of the next line, with decreasing probability (as
         n_lines increases in size).

         If the line will be used, store it directly in the reservoir.
         Otherwise, store it in dummy space.

         With 'struct linebuffer', storing into existing buffer will reduce
         re-allocations (will only re-allocate if the new line is longer than
         the currently allocated space).  */
      انجامبده
        {
          randint j = randint_choose (s, n_سطرها + 1);  /* 0 .. n_lines.  */
          سطر = (j < k) ? (&rsrv[j]) : (&dummy);
        }
      وقتی (readlinebuffer_delim (سطر, in, eolbyte) != پوچ && n_سطرها++);

      اگر (! n_سطرها)
        error (EXIT_FAILURE, EOVERFLOW, _("too many input lines"));

      freebuffer (&dummy);
    }

  /* no more input lines, or an input error.  */
  اگر (ferror (in))
    error (EXIT_FAILURE, errno, _("read error"));

  *out_rsrv = rsrv;
  بازگشت MIN (k, n_سطرها);
}

ایستا صحیح
write_permuted_output_reservoir (size_t n_سطرها, ساختار linebuffer *سطرها,
                                 size_t ثابت *permutation)
{
  برای (size_t i = 0; i < n_سطرها; i++)
    {
      ثابت ساختار linebuffer *p = &سطرها[permutation[i]];
      اگر (fwrite (p->buffer, اندازه (نویسه), p->length, stdout) != p->length)
        بازگشت -1;
    }

  بازگشت 0;
}

/* Read data from file IN.  Input lines are delimited by EOLBYTE;
   silently append a trailing EOLBYTE if the file ends in some other
   byte.  Store a pointer to the resulting array of lines into *PLINE.
   Return the number of lines read.  Report an error and exit on
   failure.  */

ایستا size_t
read_درون_داد (FILE *in, نویسه eolbyte, نویسه ***pline)
{
  نویسه *p;
  نویسه *buf = پوچ;
  size_t used;
  نویسه *lim;
  نویسه **سطر;
  size_t n_سطرها;

  /* TODO: We should limit the amount of data read here,
     to less than RESERVOIR_MIN_INPUT.  I.e., adjust fread_file() to support
     taking a byte limit.  We'd then need to ensure we handle a line spanning
     this boundary.  With that in place we could set use_reservoir_sampling
     when used==RESERVOIR_MIN_INPUT, and have read_input_reservoir_sampling()
     call a wrapper function to populate a linebuffer from the internal pline
     or if none left, stdin.  Doing that would give better performance by
     avoiding the reservoir CPU overhead when reading < RESERVOIR_MIN_INPUT
     from a pipe, and allow us to dispense with the input_size() function.  */
  اگر (!(buf = fread_file (in, 0, &used)))
    error (EXIT_FAILURE, errno, _("read error"));

  اگر (used && buf[used - 1] != eolbyte)
    buf[used++] = eolbyte;

  lim = buf + used;

  n_سطرها = 0;
  برای (p = buf; p < lim; p = next_سطر (p, eolbyte))
    n_سطرها++;

  *pline = سطر = xnmalloc (n_سطرها + 1, اندازه *سطر);

  سطر[0] = p = buf;
  برای (size_t i = 1; i <= n_سطرها; i++)
    سطر[i] = p = next_سطر (p, eolbyte);

  بازگشت n_سطرها;
}

/* Output N_LINES lines to stdout from LINE array,
   chosen by the indices in PERMUTATION.
   PERMUTATION and LINE must have at least N_LINES elements.
   Strings in LINE must include the line-terminator character.  */
ایستا صحیح
write_permuted_سطرها (size_t n_سطرها, نویسه *ثابت *سطر,
                      size_t ثابت *permutation)
{
  برای (size_t i = 0; i < n_سطرها; i++)
    {
      نویسه *ثابت *p = سطر + permutation[i];
      size_t len = p[1] - p[0];
      اگر (fwrite (p[0], اندازه *p[0], len, stdout) != len)
        بازگشت -1;
    }

  بازگشت 0;
}

/* Print NUMBER followed by EOLBYTE to standard output.
   Return false on failure, true on success.  */
ایستا bool
print_عدد (unsigned long صحیح عدد, نویسه eolbyte)
{
  نویسه buf[INT_BUFSIZE_BOUND (unsigned long صحیح)];
  نویسه *p = buf + INT_STRLEN_BOUND (unsigned long صحیح);
  *p = eolbyte;
  انجامبده
    *--p = '0' + عدد % 10;
  وقتی ((عدد /= 10) != 0);
  idx_t len = buf + اندازه buf - p;
  بازگشت fwrite (p, 1, len, stdout) == len;
}

/* Output N_LINES of numbers to stdout, from PERMUTATION array.
   PERMUTATION must have at least N_LINES elements.  */
ایستا صحیح
write_permuted_numbers (size_t n_سطرها, size_t lo_درون_داد,
                        size_t ثابت *permutation, نویسه eolbyte)
{
  برای (size_t i = 0; i < n_سطرها; i++)
    {
      unsigned long صحیح n = lo_درون_داد + permutation[i];
      اگر (! print_عدد (n, eolbyte))
        بازگشت -1;
    }

  بازگشت 0;
}

/* Output COUNT numbers to stdout, chosen randomly from range
   LO_INPUT through HI_INPUT.  */
ایستا صحیح
write_random_numbers (ساختار randint_source *s, size_t شمار,
                      size_t lo_درون_داد, size_t hi_درون_داد, نویسه eolbyte)
{
  ثابت randint range = hi_درون_داد - lo_درون_داد + 1;

  برای (size_t i = 0; i < شمار; i++)
    {
      unsigned long صحیح j = lo_درون_داد + randint_choose (s, range);
      اگر (! print_عدد (j, eolbyte))
        بازگشت -1;
    }

  بازگشت 0;
}

/* Output COUNT lines to stdout from LINES array.
   LINES must have at least N_LINES elements in it.
   Strings in LINES_ must include the line-terminator character.  */
ایستا صحیح
write_random_سطرها (ساختار randint_source *s, size_t شمار,
                    نویسه *ثابت *سطرها, size_t n_سطرها)
{
  برای (size_t i = 0; i < شمار; i++)
    {
      ثابت randint j = randint_choose (s, n_سطرها);
      نویسه *ثابت *p = سطرها + j;
      size_t len = p[1] - p[0];
      اگر (fwrite (p[0], اندازه *p[0], len, stdout) != len)
        بازگشت -1;
    }

  بازگشت 0;
}

صحیح
اصلی (صحیح argc, نویسه **argv)
{
  bool echo = false;
  bool درون_داد_range = false;
  size_t lo_درون_داد = SIZE_MAX;
  size_t hi_درون_داد = 0;
  idx_t head_سطرها = MIN (IDX_MAX, SIZE_MAX);
  نویسه ثابت *outfile = پوچ;
  نویسه *random_source = پوچ;
  نویسه eolbyte = '\n';
  نویسه **درون_داد_سطرها = پوچ;
  bool use_reservoir_sampling = false;
  bool repeat = false;

  صحیح optc;
  صحیح n_operands;
  نویسه **operand;
  size_t n_سطرها;
  نویسه **سطر = پوچ;
  ساختار linebuffer *reservoir = پوچ;
  ساختار randint_source *randint_source;
  size_t *permutation = پوچ;
  صحیح i;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  وقتی ((optc = getopt_long (argc, argv, "ei:n:o:rz", long_opts, پوچ))
         != -1)
    انتخاب (optc)
      {
      حالت 'e':
        echo = true;
        بشکن;

      حالت 'i':
        {
          اگر (درون_داد_range)
            error (EXIT_FAILURE, 0, _("multiple -i options specified"));
          درون_داد_range = true;

          uintmax_t u;
          نویسه *lo_end;
          strtol_error err = xstrtoumax (optarg, &lo_end, 10, &u, پوچ);
          اگر (err == LONGINT_OK)
            {
              lo_درون_داد = u;
              اگر (lo_درون_داد != u)
                err = LONGINT_OVERFLOW;
              وگرنه اگر (*lo_end != '-')
                err = LONGINT_INVALID;
              وگرنه
                {
                  err = xstrtoumax (lo_end + 1, پوچ, 10, &u, "");
                  اگر (err == LONGINT_OK)
                    {
                      hi_درون_داد = u;
                      اگر (hi_درون_داد != u)
                        err = LONGINT_OVERFLOW;
                    }
                }
            }

          n_سطرها = hi_درون_داد - lo_درون_داد + 1;

          اگر (err != LONGINT_OK || (lo_درون_داد <= hi_درون_داد) == (n_سطرها == 0))
            error (EXIT_FAILURE, err == LONGINT_OVERFLOW ? EOVERFLOW : 0,
                   "%s: %s", _("invalid input range"), quote (optarg));
        }
        بشکن;

      حالت 'n':
        {
          uintmax_t argval;
          strtol_error e = xstrtoumax (optarg, پوچ, 10, &argval, "");

          اگر (e == LONGINT_OK)
            head_سطرها = MIN (head_سطرها, argval);
          وگرنه اگر (e != LONGINT_OVERFLOW)
            error (EXIT_FAILURE, 0, _("invalid line count: %s"),
                   quote (optarg));
        }
        بشکن;

      حالت 'o':
        اگر (outfile && !streq (outfile, optarg))
          error (EXIT_FAILURE, 0, _("multiple output files specified"));
        outfile = optarg;
        بشکن;

      حالت RANDOM_SOURCE_OPTION:
        اگر (random_source && !streq (random_source, optarg))
          error (EXIT_FAILURE, 0, _("multiple random sources specified"));
        random_source = optarg;
        بشکن;

      حالت 'r':
        repeat = true;
        بشکن;

      حالت 'z':
        eolbyte = '\0';
        بشکن;

      case_GETOPT_HELP_CHAR;
      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
      پیشفرض:
        usage (EXIT_FAILURE);
      }

  n_operands = argc - optind;
  operand = argv + optind;

  /* Check invalid usage.  */
  اگر (echo && درون_داد_range)
    {
      error (0, 0, _("cannot combine -e and -i options"));
      usage (EXIT_FAILURE);
    }
  اگر (درون_داد_range ? 0 < n_operands : !echo && 1 < n_operands)
    {
      error (0, 0, _("extra operand %s"), quote (operand[!درون_داد_range]));
      usage (EXIT_FAILURE);
    }

  /* Prepare input.  */
  اگر (head_سطرها == 0)
    {
      n_سطرها = 0;
      سطر = پوچ;
    }
  وگرنه اگر (echo)
    {
      درون_داد_from_argv (operand, n_operands, eolbyte);
      n_سطرها = n_operands;
      سطر = operand;
    }
  وگرنه اگر (درون_داد_range)
    {
      IF_LINT (n_سطرها = hi_درون_داد - lo_درون_داد + 1); /* Avoid GCC 10 warning.  */
      سطر = پوچ;
    }
  وگرنه
    {
      /* If an input file is specified, re-open it as stdin.  */
      اگر (n_operands == 1
          && ! (streq (operand[0], "-")
                || freopen (operand[0], "r", stdin)))
        error (EXIT_FAILURE, errno, "%s", quotef (operand[0]));

      fadvise (stdin, FADVISE_SEQUENTIAL);

      اگر (repeat || head_سطرها == MIN (IDX_MAX, SIZE_MAX)
          || درون_داد_size () <= RESERVOIR_MIN_درون_داد)
        {
          n_سطرها = read_درون_داد (stdin, eolbyte, &درون_داد_سطرها);
          سطر = درون_داد_سطرها;
        }
      وگرنه
        {
          use_reservoir_sampling = true;
          n_سطرها = SIZE_MAX;   /* unknown number of input lines, for now.  */
        }
    }

  /* The adjusted head line count; can be less than HEAD_LINES if the
     input is small and if not repeating.  */
  idx_t ahead_سطرها = repeat || head_سطرها < n_سطرها ? head_سطرها : n_سطرها;

  randint_source = randint_all_new (random_source,
                                    (use_reservoir_sampling || repeat
                                     ? SIZE_MAX
                                     : randperm_bound (ahead_سطرها, n_سطرها)));
  اگر (! randint_source)
    error (EXIT_FAILURE, errno, "%s",
           quotef (random_source ? random_source : "getrandom"));

  اگر (use_reservoir_sampling)
    {
      /* Instead of reading the entire file into 'line',
         use reservoir-sampling to store just AHEAD_LINES random lines.  */
      n_سطرها = read_درون_داد_reservoir_sampling (stdin, eolbyte, ahead_سطرها,
                                               randint_source, &reservoir);
      ahead_سطرها = n_سطرها;
    }

  /* Close stdin now, rather than earlier, so that randint_all_new
     doesn't have to worry about opening something other than
     stdin.  */
  اگر (! (head_سطرها == 0 || echo || درون_داد_range || fclose (stdin) == 0))
    error (EXIT_FAILURE, errno, _("read error"));

  اگر (!repeat)
    permutation = randperm_new (randint_source, ahead_سطرها, n_سطرها);

  اگر (outfile && ! freopen (outfile, "w", stdout))
    error (EXIT_FAILURE, errno, "%s", quotef (outfile));

  /* Generate output according to requested method */
  اگر (repeat)
    {
      اگر (head_سطرها == 0)
        i = 0;
      وگرنه
        {
          اگر (n_سطرها == 0)
            error (EXIT_FAILURE, 0, _("no lines to repeat"));
          اگر (درون_داد_range)
            i = write_random_numbers (randint_source, ahead_سطرها,
                                      lo_درون_داد, hi_درون_داد, eolbyte);
          وگرنه
            i = write_random_سطرها (randint_source, ahead_سطرها, سطر, n_سطرها);
        }
    }
  وگرنه
    {
      اگر (use_reservoir_sampling)
        i = write_permuted_output_reservoir (n_سطرها, reservoir, permutation);
      وگرنه اگر (درون_داد_range)
        i = write_permuted_numbers (ahead_سطرها, lo_درون_داد,
                                    permutation, eolbyte);
      وگرنه
        i = write_permuted_سطرها (ahead_سطرها, سطر, permutation);
    }

  اگر (i != 0)
    write_error ();

  IF_LINT (randint_all_free (randint_source));  /* For older valgrind.  */

  main_exit (EXIT_SUCCESS);
}
