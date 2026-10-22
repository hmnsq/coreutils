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
#تعریف_کن برنامه_NAME "shuf"

#تعریف_کن AUTHORS proper_name ("Paul Eggert")

/* reservoir-sampling introduces CPU overhead for small inputs.
   So only enable it for inputs >= this limit.
   This limit was determined using these commands:
     $ for p in $(seq 7); do src/seq $((10**$p)) > 10p$p.in; done
     $ for p in $(seq 7); do time shuf-nores -n10 10p$p.in >/dev/null; done
     $ for p in $(seq 7); do time shuf -n10 10p$p.in >/dev/null; done  .*/
تعداد { RESERVOIR_MIN_درون_داد = 8192 * 1024 };


فراغ
usage (صحيح وضعیت)
{
  إذا (وضعیت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کن (_("\
Usage: %s [OPTION]... [FILE]\n\
  or:  %s -e [OPTION]... [ARG]...\n\
  or:  %s -i LO-HI [OPTION]...\n\
"),
              برنامه_name, برنامه_name, برنامه_name);
      fputs (_("\
Write a random permutation از the درون_داد سطرها مقصد standard output.\n\
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
  -n, --سر-شمار=شمار_2    output at most شمار_2 سطرها\n\
"));
      oputs (_("\
  -o, --output=FILE         اكتب نتیجه مقصد FILE instead از standard output\n\
"));
      oputs (_("\
      --random-منبع_2=FILE  get random بایتها from FILE\n\
"));
      oputs (_("\
  -r, --repeat              output سطرها can be repeated\n\
"));
      oputs (_("\
  -z, --zero-terminated     سطر delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);
      emit_ancillary_info (برنامه_NAME);
    }

  خروج (وضعیت);
}

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
تعداد
{
  RANDOM_منبع_OPTION = CHAR_MAX + 1
};

ساكن بنية option ثابت long_opts[] =
{
  {"echo", no_آرگومان, عدم, 'e'},
  {"input-range", required_آرگومان, عدم, 'i'},
  {"head-count", required_آرگومان, عدم, 'n'},
  {"output", required_آرگومان, عدم, 'o'},
  {"random-source", required_آرگومان, عدم, RANDOM_منبع_OPTION},
  {"repeat", no_آرگومان, عدم, 'r'},
  {"zero-terminated", no_آرگومان, عدم, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخه_OPTION_DECL},
  {عدم, 0, عدم, 0},
};

ساكن فراغ
درون_داد_from_argv (حرف **عملوند_2, صحيح n_operands, حرف eolbyte)
{
  حرف *p;
  size_t اندازه = n_operands;
  صحيح i;

  لكل (i = 0; i < n_operands; i++)
    اندازه += طول_رشته (عملوند_2[i]);
  p = xmalloc (اندازه);

  لكل (i = 0; i < n_operands; i++)
    {
      حرف *p1 = stpcpy (p, عملوند_2[i]);
      عملوند_2[i] = p;
      p = p1;
      *p++ = eolbyte;
    }

  عملوند_2[n_operands] = p;
}

/* Return the start of the next line after LINE, which is guaranteed
   to end in EOLBYTE.  */

ساكن حرف *
بعدی_سطر (حرف *سطر, حرف eolbyte)
{
  حرف *p = rawmemchr (سطر, eolbyte);
  أرجع p + 1;
}

/* Return the size of the input if possible or OFF_T_MAX if not.  */

ساكن off_t
درون_داد_اندازه (فراغ)
{
  off_t file_اندازه;

  بنية stat stat_buf;
  إذا (fstat (STDIN_FILENO, &stat_buf) != 0)
    أرجع OFF_T_MAX;
  إذا (usable_st_اندازه (&stat_buf))
    file_اندازه = stat_buf.st_اندازه;
  وإلا
    أرجع OFF_T_MAX;

  off_t درون_داد_جابه_جایی = lseek (STDIN_FILENO, 0, SEEK_CUR);
  إذا (درون_داد_جابه_جایی < 0)
    أرجع OFF_T_MAX;

  file_اندازه -= درون_داد_جابه_جایی;

  أرجع file_اندازه;
}

/* Read all lines and store up to K permuted lines in *OUT_RSRV.
   Return the number of lines read, up to a maximum of K.  */

ساكن idx_t
read_درون_داد_reservoir_sampling (FILE *درون, حرف eolbyte, idx_t k,
                               بنية randint_منبع *s,
                               بنية linebuffer **بیرون_rsrv)
{
  randint n_سطرها = 0;
  idx_t n_alloc_سطرها = 0;
  بنية linebuffer *سطر = عدم;
  بنية linebuffer *rsrv = عدم;

  /* Fill the first K lines, directly into the reservoir.  */
  لكل (n_سطرها = 0; n_سطرها < k; n_سطرها++)
    {
      /* Enlarge reservoir if needed.  */
      إذا (n_سطرها == n_alloc_سطرها)
        {
          idx_t old = n_alloc_سطرها;
          rsrv = xpalloc (rsrv, &n_alloc_سطرها, 1, k, حجم *rsrv);
          memset (&rsrv[n_سطرها], 0, (n_alloc_سطرها - old) * حجم *rsrv);
        }

      سطر = readlinebuffer_delim (&rsrv[n_سطرها], درون, eolbyte);
      إذا (!سطر)
        اكسر;
    }

  /* last line wasn't null - so there may be more lines to read.  */
  إذا (سطر != عدم)
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
          randint j = randint_انتخاب (s, n_سطرها + 1);  /* 0 .. n_lines.  */
          سطر = (j < k) ? (&rsrv[j]) : (&dummy);
        }
      طالما (readlinebuffer_delim (سطر, درون, eolbyte) != عدم && n_سطرها++);

      إذا (! n_سطرها)
        error (EXIT_FAILURE, EOVERFLOW, _("too many input lines"));

      freebuffer (&dummy);
    }

  /* no more input lines, or an input error.  */
  إذا (ferror (درون))
    error (EXIT_FAILURE, errno, _("read error"));

  *بیرون_rsrv = rsrv;
  أرجع MIN (k, n_سطرها);
}

ساكن صحيح
write_permuted_output_reservoir (size_t n_سطرها, بنية linebuffer *سطرها,
                                 size_t ثابت *permutation)
{
  لكل (size_t i = 0; i < n_سطرها; i++)
    {
      ثابت بنية linebuffer *p = &سطرها[permutation[i]];
      إذا (fwrite (p->میانگیر, حجم (حرف), p->درازی, stdout) != p->درازی)
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
read_درون_داد (FILE *درون, حرف eolbyte, حرف ***pline)
{
  حرف *p;
  حرف *buf = عدم;
  size_t استفاده_شده;
  حرف *lim;
  حرف **سطر;
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
  إذا (!(buf = fread_file (درون, 0, &استفاده_شده)))
    error (EXIT_FAILURE, errno, _("read error"));

  إذا (استفاده_شده && buf[استفاده_شده - 1] != eolbyte)
    buf[استفاده_شده++] = eolbyte;

  lim = buf + استفاده_شده;

  n_سطرها = 0;
  لكل (p = buf; p < lim; p = بعدی_سطر (p, eolbyte))
    n_سطرها++;

  *pline = سطر = xnmalloc (n_سطرها + 1, حجم *سطر);

  سطر[0] = p = buf;
  لكل (size_t i = 1; i <= n_سطرها; i++)
    سطر[i] = p = بعدی_سطر (p, eolbyte);

  أرجع n_سطرها;
}

/* Output N_LINES lines to stdout from LINE array,
   chosen by the indices in PERMUTATION.
   PERMUTATION and LINE must have at least N_LINES elements.
   Strings in LINE must include the line-terminator character.  */
ساكن صحيح
write_permuted_سطرها (size_t n_سطرها, حرف *ثابت *سطر,
                      size_t ثابت *permutation)
{
  لكل (size_t i = 0; i < n_سطرها; i++)
    {
      حرف *ثابت *p = سطر + permutation[i];
      size_t len = p[1] - p[0];
      إذا (fwrite (p[0], حجم *p[0], len, stdout) != len)
        أرجع -1;
    }

  أرجع 0;
}

/* Print NUMBER followed by EOLBYTE to standard output.
   Return false on failure, true on success.  */
ساكن bool
print_عدد (بیعلامت دراز صحيح عدد, حرف eolbyte)
{
  حرف buf[INT_BUFSIZE_BOUND (بیعلامت دراز صحيح)];
  حرف *p = buf + INT_STRLEN_BOUND (بیعلامت دراز صحيح);
  *p = eolbyte;
  افعل
    *--p = '0' + عدد % 10;
  طالما ((عدد /= 10) != 0);
  idx_t len = buf + حجم buf - p;
  أرجع fwrite (p, 1, len, stdout) == len;
}

/* Output N_LINES of numbers to stdout, from PERMUTATION array.
   PERMUTATION must have at least N_LINES elements.  */
ساكن صحيح
write_permuted_numbers (size_t n_سطرها, size_t lo_درون_داد,
                        size_t ثابت *permutation, حرف eolbyte)
{
  لكل (size_t i = 0; i < n_سطرها; i++)
    {
      بیعلامت دراز صحيح n = lo_درون_داد + permutation[i];
      إذا (! print_عدد (n, eolbyte))
        أرجع -1;
    }

  أرجع 0;
}

/* Output COUNT numbers to stdout, chosen randomly from range
   LO_INPUT through HI_INPUT.  */
ساكن صحيح
write_random_numbers (بنية randint_منبع *s, size_t شمار,
                      size_t lo_درون_داد, size_t hi_درون_داد, حرف eolbyte)
{
  ثابت randint range = hi_درون_داد - lo_درون_داد + 1;

  لكل (size_t i = 0; i < شمار; i++)
    {
      بیعلامت دراز صحيح j = lo_درون_داد + randint_انتخاب (s, range);
      إذا (! print_عدد (j, eolbyte))
        أرجع -1;
    }

  أرجع 0;
}

/* Output COUNT lines to stdout from LINES array.
   LINES must have at least N_LINES elements in it.
   Strings in LINES_ must include the line-terminator character.  */
ساكن صحيح
write_random_سطرها (بنية randint_منبع *s, size_t شمار,
                    حرف *ثابت *سطرها, size_t n_سطرها)
{
  لكل (size_t i = 0; i < شمار; i++)
    {
      ثابت randint j = randint_انتخاب (s, n_سطرها);
      حرف *ثابت *p = سطرها + j;
      size_t len = p[1] - p[0];
      إذا (fwrite (p[0], حجم *p[0], len, stdout) != len)
        أرجع -1;
    }

  أرجع 0;
}

صحيح
اصلی (صحيح argc, حرف **argv)
{
  bool echo = false;
  bool درون_داد_range = false;
  size_t lo_درون_داد = SIZE_MAX;
  size_t hi_درون_داد = 0;
  idx_t سر_سطرها = MIN (IDX_MAX, SIZE_MAX);
  حرف ثابت *outfile = عدم;
  حرف *random_منبع = عدم;
  حرف eolbyte = '\n';
  حرف **درون_داد_سطرها = عدم;
  bool use_reservoir_sampling = false;
  bool repeat = false;

  صحيح optc;
  صحيح n_operands;
  حرف **عملوند_2;
  size_t n_سطرها;
  حرف **سطر = عدم;
  بنية linebuffer *reservoir = عدم;
  بنية randint_منبع *randint_منبع;
  size_t *permutation = عدم;
  صحيح i;

  initialize_main (&argc, &argv);
  مجموعه_برنامه_name (argv[0]);
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
          إذا (درون_داد_range)
            error (EXIT_FAILURE, 0, _("multiple -i options specified"));
          درون_داد_range = true;

          uintmax_t u;
          حرف *lo_انتها;
          strtol_error err = xstrtoumax (optarg, &lo_انتها, 10, &u, عدم);
          إذا (err == LONGINT_OK)
            {
              lo_درون_داد = u;
              إذا (lo_درون_داد != u)
                err = LONGINT_سرریز;
              وإلا إذا (*lo_انتها != '-')
                err = LONGINT_INVALID;
              وإلا
                {
                  err = xstrtoumax (lo_انتها + 1, عدم, 10, &u, "");
                  إذا (err == LONGINT_OK)
                    {
                      hi_درون_داد = u;
                      إذا (hi_درون_داد != u)
                        err = LONGINT_سرریز;
                    }
                }
            }

          n_سطرها = hi_درون_داد - lo_درون_داد + 1;

          إذا (err != LONGINT_OK || (lo_درون_داد <= hi_درون_داد) == (n_سطرها == 0))
            error (EXIT_FAILURE, err == LONGINT_سرریز ? EOVERFLOW : 0,
                   "%s: %s", _("invalid input range"), quote (optarg));
        }
        اكسر;

      حالة 'n':
        {
          uintmax_t argval;
          strtol_error e = xstrtoumax (optarg, عدم, 10, &argval, "");

          إذا (e == LONGINT_OK)
            سر_سطرها = MIN (سر_سطرها, argval);
          وإلا إذا (e != LONGINT_سرریز)
            error (EXIT_FAILURE, 0, _("invalid line count: %s"),
                   quote (optarg));
        }
        اكسر;

      حالة 'o':
        إذا (outfile && !streq (outfile, optarg))
          error (EXIT_FAILURE, 0, _("multiple output files specified"));
        outfile = optarg;
        اكسر;

      حالة RANDOM_منبع_OPTION:
        إذا (random_منبع && !streq (random_منبع, optarg))
          error (EXIT_FAILURE, 0, _("multiple random sources specified"));
        random_منبع = optarg;
        اكسر;

      حالة 'r':
        repeat = true;
        اكسر;

      حالة 'z':
        eolbyte = '\0';
        اكسر;

      case_GETOPT_HELP_CHAR;
      case_GETOPT_نسخه_CHAR (برنامه_NAME, AUTHORS);
      افتراضي:
        usage (EXIT_FAILURE);
      }

  n_operands = argc - optind;
  عملوند_2 = argv + optind;

  /* Check invalid usage.  */
  إذا (echo && درون_داد_range)
    {
      error (0, 0, _("cannot combine -e and -i options"));
      usage (EXIT_FAILURE);
    }
  إذا (درون_داد_range ? 0 < n_operands : !echo && 1 < n_operands)
    {
      error (0, 0, _("extra operand %s"), quote (عملوند_2[!درون_داد_range]));
      usage (EXIT_FAILURE);
    }

  /* Prepare input.  */
  إذا (سر_سطرها == 0)
    {
      n_سطرها = 0;
      سطر = عدم;
    }
  وإلا إذا (echo)
    {
      درون_داد_from_argv (عملوند_2, n_operands, eolbyte);
      n_سطرها = n_operands;
      سطر = عملوند_2;
    }
  وإلا إذا (درون_داد_range)
    {
      IF_LINT (n_سطرها = hi_درون_داد - lo_درون_داد + 1); /* Avoid GCC 10 warning.  */
      سطر = عدم;
    }
  وإلا
    {
      /* If an input file is specified, re-open it as stdin.  */
      إذا (n_operands == 1
          && ! (streq (عملوند_2[0], "-")
                || freopen (عملوند_2[0], "r", stdin)))
        error (EXIT_FAILURE, errno, "%s", quotef (عملوند_2[0]));

      fadvise (stdin, FADVISE_SEQUENTIAL);

      إذا (repeat || سر_سطرها == MIN (IDX_MAX, SIZE_MAX)
          || درون_داد_اندازه () <= RESERVOIR_MIN_درون_داد)
        {
          n_سطرها = read_درون_داد (stdin, eolbyte, &درون_داد_سطرها);
          سطر = درون_داد_سطرها;
        }
      وإلا
        {
          use_reservoir_sampling = true;
          n_سطرها = SIZE_MAX;   /* unknown number of input lines, for now.  */
        }
    }

  /* The adjusted head line count; can be less than HEAD_LINES if the
     input is small and if not repeating.  */
  idx_t ahead_سطرها = repeat || سر_سطرها < n_سطرها ? سر_سطرها : n_سطرها;

  randint_منبع = randint_all_new (random_منبع,
                                    (use_reservoir_sampling || repeat
                                     ? SIZE_MAX
                                     : randperm_bound (ahead_سطرها, n_سطرها)));
  إذا (! randint_منبع)
    error (EXIT_FAILURE, errno, "%s",
           quotef (random_منبع ? random_منبع : "getrandom"));

  إذا (use_reservoir_sampling)
    {
      /* Instead of reading the entire file into 'line',
         use reservoir-sampling to store just AHEAD_LINES random lines.  */
      n_سطرها = read_درون_داد_reservoir_sampling (stdin, eolbyte, ahead_سطرها,
                                               randint_منبع, &reservoir);
      ahead_سطرها = n_سطرها;
    }

  /* Close stdin now, rather than earlier, so that randint_all_new
     doesn't have to worry about opening something other than
     stdin.  */
  إذا (! (سر_سطرها == 0 || echo || درون_داد_range || fclose (stdin) == 0))
    error (EXIT_FAILURE, errno, _("read error"));

  إذا (!repeat)
    permutation = randperm_new (randint_منبع, ahead_سطرها, n_سطرها);

  إذا (outfile && ! freopen (outfile, "w", stdout))
    error (EXIT_FAILURE, errno, "%s", quotef (outfile));

  /* Generate output according to requested method */
  إذا (repeat)
    {
      إذا (سر_سطرها == 0)
        i = 0;
      وإلا
        {
          إذا (n_سطرها == 0)
            error (EXIT_FAILURE, 0, _("no lines to repeat"));
          إذا (درون_داد_range)
            i = write_random_numbers (randint_منبع, ahead_سطرها,
                                      lo_درون_داد, hi_درون_داد, eolbyte);
          وإلا
            i = write_random_سطرها (randint_منبع, ahead_سطرها, سطر, n_سطرها);
        }
    }
  وإلا
    {
      إذا (use_reservoir_sampling)
        i = write_permuted_output_reservoir (n_سطرها, reservoir, permutation);
      وإلا إذا (درون_داد_range)
        i = write_permuted_numbers (ahead_سطرها, lo_درون_داد,
                                    permutation, eolbyte);
      وإلا
        i = write_permuted_سطرها (ahead_سطرها, سطر, permutation);
    }

  إذا (i != 0)
    write_error ();

  IF_LINT (randint_all_free (randint_منبع));  /* For older valgrind.  */

  main_exit (EXIT_SUCCESS);
}
