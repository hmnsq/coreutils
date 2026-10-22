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
#وصف_ڪريو پروگرام_NAME "shuf"

#وصف_ڪريو AUTHORS proper_name ("Paul Eggert")

/* reservoir-sampling introduces CPU overhead for small inputs.
   So only enable it for inputs >= this limit.
   This limit was determined using these commands:
     $ for p in $(seq 7); do src/seq $((10**$p)) > 10p$p.in; done
     $ for p in $(seq 7); do time shuf-nores -n10 10p$p.in >/dev/null; done
     $ for p in $(seq 7); do time shuf -n10 10p$p.in >/dev/null; done  .*/
تعداد { RESERVOIR_MIN_داخلا = 8192 * 1024 };


فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکو (_("\
Usage: %s [OPTION]... [FILE]\n\
  or:  %s -e [OPTION]... [ARG]...\n\
  or:  %s -i LO-HI [OPTION]...\n\
"),
              پروگرام_name, پروگرام_name, پروگرام_name);
      fputs (_("\
Write a random permutation جو the داخلا سٽون منزل standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -e, --echo                treat each ARG as an داخلا سٽ\n\
"));
      oputs (_("\
  -i, --داخلا-range=LO-HI   treat each عدد LO through HI as an داخلا سٽ\n\
"));
      oputs (_("\
  -n, --سر-ڳڻپ=ڳڻپ_2    output at most ڳڻپ_2 سٽون\n\
"));
      oputs (_("\
  -o, --output=FILE         اكتب نتيجو منزل FILE instead جو standard output\n\
"));
      oputs (_("\
      --random-ذريعو_2=FILE  get random بائيٽون from FILE\n\
"));
      oputs (_("\
  -r, --repeat              output سٽون can be repeated\n\
"));
      oputs (_("\
  -z, --zero-terminated     سٽ delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخو_OPTION_DESCRIPTION);
      emit_ancillary_info (پروگرام_NAME);
    }

  ٻاهر_نڪرو (حالت);
}

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
تعداد
{
  RANDOM_ذريعو_OPTION = CHAR_MAX + 1
};

ساكن بنية option ثابت long_opts[] =
{
  {"echo", no_دليل, عدم, 'e'},
  {"input-range", required_دليل, عدم, 'i'},
  {"head-count", required_دليل, عدم, 'n'},
  {"output", required_دليل, عدم, 'o'},
  {"random-source", required_دليل, عدم, RANDOM_ذريعو_OPTION},
  {"repeat", no_دليل, عدم, 'r'},
  {"zero-terminated", no_دليل, عدم, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخو_OPTION_DECL},
  {عدم, 0, عدم, 0},
};

ساكن فراغ
داخلا_from_argv (حرف **عمليو_2, صحيح n_operands, حرف eolbyte)
{
  حرف *p;
  size_t سائيز = n_operands;
  صحيح i;

  لكل (i = 0; i < n_operands; i++)
    سائيز += لڙي_ڊيگهه (عمليو_2[i]);
  p = xmalloc (سائيز);

  لكل (i = 0; i < n_operands; i++)
    {
      حرف *p1 = stpcpy (p, عمليو_2[i]);
      عمليو_2[i] = p;
      p = p1;
      *p++ = eolbyte;
    }

  عمليو_2[n_operands] = p;
}

/* Return the start of the next line after LINE, which is guaranteed
   to end in EOLBYTE.  */

ساكن حرف *
ايندڙ_سٽ (حرف *سٽ, حرف eolbyte)
{
  حرف *p = rawmemchr (سٽ, eolbyte);
  أرجع p + 1;
}

/* Return the size of the input if possible or OFF_T_MAX if not.  */

ساكن off_t
داخلا_سائيز (فراغ)
{
  off_t file_سائيز;

  بنية stat stat_buf;
  إذا (fstat (STDIN_FILENO, &stat_buf) != 0)
    أرجع OFF_T_MAX;
  إذا (usable_st_سائيز (&stat_buf))
    file_سائيز = stat_buf.st_سائيز;
  وإلا
    أرجع OFF_T_MAX;

  off_t داخلا_وٿي = lseek (STDIN_FILENO, 0, SEEK_CUR);
  إذا (داخلا_وٿي < 0)
    أرجع OFF_T_MAX;

  file_سائيز -= داخلا_وٿي;

  أرجع file_سائيز;
}

/* Read all lines and store up to K permuted lines in *OUT_RSRV.
   Return the number of lines read, up to a maximum of K.  */

ساكن idx_t
read_داخلا_reservoir_sampling (FILE *اندريون, حرف eolbyte, idx_t k,
                               بنية randint_ذريعو *s,
                               بنية linebuffer **ٻاهريون_rsrv)
{
  randint n_سٽون = 0;
  idx_t n_alloc_سٽون = 0;
  بنية linebuffer *سٽ = عدم;
  بنية linebuffer *rsrv = عدم;

  /* Fill the first K lines, directly into the reservoir.  */
  لكل (n_سٽون = 0; n_سٽون < k; n_سٽون++)
    {
      /* Enlarge reservoir if needed.  */
      إذا (n_سٽون == n_alloc_سٽون)
        {
          idx_t old = n_alloc_سٽون;
          rsrv = xpalloc (rsrv, &n_alloc_سٽون, 1, k, حجم *rsrv);
          memset (&rsrv[n_سٽون], 0, (n_alloc_سٽون - old) * حجم *rsrv);
        }

      سٽ = readlinebuffer_delim (&rsrv[n_سٽون], اندريون, eolbyte);
      إذا (!سٽ)
        اكسر;
    }

  /* last line wasn't null - so there may be more lines to read.  */
  إذا (سٽ != عدم)
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
          randint j = randint_چونڊيو (s, n_سٽون + 1);  /* 0 .. n_lines.  */
          سٽ = (j < k) ? (&rsrv[j]) : (&dummy);
        }
      طالما (readlinebuffer_delim (سٽ, اندريون, eolbyte) != عدم && n_سٽون++);

      إذا (! n_سٽون)
        error (EXIT_FAILURE, EOVERFLOW, _("too many input lines"));

      freebuffer (&dummy);
    }

  /* no more input lines, or an input error.  */
  إذا (ferror (اندريون))
    error (EXIT_FAILURE, errno, _("read error"));

  *ٻاهريون_rsrv = rsrv;
  أرجع MIN (k, n_سٽون);
}

ساكن صحيح
write_permuted_output_reservoir (size_t n_سٽون, بنية linebuffer *سٽون,
                                 size_t ثابت *permutation)
{
  لكل (size_t i = 0; i < n_سٽون; i++)
    {
      ثابت بنية linebuffer *p = &سٽون[permutation[i]];
      إذا (fwrite (p->بفر, حجم (حرف), p->ڊيگهائي, stdout) != p->ڊيگهائي)
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
read_داخلا (FILE *اندريون, حرف eolbyte, حرف ***pline)
{
  حرف *p;
  حرف *buf = عدم;
  size_t استعمال_ٿيل;
  حرف *lim;
  حرف **سٽ;
  size_t n_سٽون;

  /* TODO: We should limit the amount of data read here,
     to less than RESERVOIR_MIN_INPUT.  I.e., adjust fread_file() to support
     taking a byte limit.  We'd then need to ensure we handle a line spanning
     this boundary.  With that in place we could set use_reservoir_sampling
     when used==RESERVOIR_MIN_INPUT, and have read_input_reservoir_sampling()
     call a wrapper function to populate a linebuffer from the internal pline
     or if none left, stdin.  Doing that would give better performance by
     avoiding the reservoir CPU overhead when reading < RESERVOIR_MIN_INPUT
     from a pipe, and allow us to dispense with the input_size() function.  */
  إذا (!(buf = fread_file (اندريون, 0, &استعمال_ٿيل)))
    error (EXIT_FAILURE, errno, _("read error"));

  إذا (استعمال_ٿيل && buf[استعمال_ٿيل - 1] != eolbyte)
    buf[استعمال_ٿيل++] = eolbyte;

  lim = buf + استعمال_ٿيل;

  n_سٽون = 0;
  لكل (p = buf; p < lim; p = ايندڙ_سٽ (p, eolbyte))
    n_سٽون++;

  *pline = سٽ = xnmalloc (n_سٽون + 1, حجم *سٽ);

  سٽ[0] = p = buf;
  لكل (size_t i = 1; i <= n_سٽون; i++)
    سٽ[i] = p = ايندڙ_سٽ (p, eolbyte);

  أرجع n_سٽون;
}

/* Output N_LINES lines to stdout from LINE array,
   chosen by the indices in PERMUTATION.
   PERMUTATION and LINE must have at least N_LINES elements.
   Strings in LINE must include the line-terminator character.  */
ساكن صحيح
write_permuted_سٽون (size_t n_سٽون, حرف *ثابت *سٽ,
                      size_t ثابت *permutation)
{
  لكل (size_t i = 0; i < n_سٽون; i++)
    {
      حرف *ثابت *p = سٽ + permutation[i];
      size_t len = p[1] - p[0];
      إذا (fwrite (p[0], حجم *p[0], len, stdout) != len)
        أرجع -1;
    }

  أرجع 0;
}

/* Print NUMBER followed by EOLBYTE to standard output.
   Return false on failure, true on success.  */
ساكن bool
print_عدد (بنانشان ڊگهو صحيح عدد, حرف eolbyte)
{
  حرف buf[INT_BUFSIZE_BOUND (بنانشان ڊگهو صحيح)];
  حرف *p = buf + INT_STRLEN_BOUND (بنانشان ڊگهو صحيح);
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
write_permuted_numbers (size_t n_سٽون, size_t lo_داخلا,
                        size_t ثابت *permutation, حرف eolbyte)
{
  لكل (size_t i = 0; i < n_سٽون; i++)
    {
      بنانشان ڊگهو صحيح n = lo_داخلا + permutation[i];
      إذا (! print_عدد (n, eolbyte))
        أرجع -1;
    }

  أرجع 0;
}

/* Output COUNT numbers to stdout, chosen randomly from range
   LO_INPUT through HI_INPUT.  */
ساكن صحيح
write_random_numbers (بنية randint_ذريعو *s, size_t ڳڻپ,
                      size_t lo_داخلا, size_t hi_داخلا, حرف eolbyte)
{
  ثابت randint range = hi_داخلا - lo_داخلا + 1;

  لكل (size_t i = 0; i < ڳڻپ; i++)
    {
      بنانشان ڊگهو صحيح j = lo_داخلا + randint_چونڊيو (s, range);
      إذا (! print_عدد (j, eolbyte))
        أرجع -1;
    }

  أرجع 0;
}

/* Output COUNT lines to stdout from LINES array.
   LINES must have at least N_LINES elements in it.
   Strings in LINES_ must include the line-terminator character.  */
ساكن صحيح
write_random_سٽون (بنية randint_ذريعو *s, size_t ڳڻپ,
                    حرف *ثابت *سٽون, size_t n_سٽون)
{
  لكل (size_t i = 0; i < ڳڻپ; i++)
    {
      ثابت randint j = randint_چونڊيو (s, n_سٽون);
      حرف *ثابت *p = سٽون + j;
      size_t len = p[1] - p[0];
      إذا (fwrite (p[0], حجم *p[0], len, stdout) != len)
        أرجع -1;
    }

  أرجع 0;
}

صحيح
مکيه (صحيح argc, حرف **argv)
{
  bool echo = false;
  bool داخلا_range = false;
  size_t lo_داخلا = SIZE_MAX;
  size_t hi_داخلا = 0;
  idx_t سر_سٽون = MIN (IDX_MAX, SIZE_MAX);
  حرف ثابت *outfile = عدم;
  حرف *random_ذريعو = عدم;
  حرف eolbyte = '\n';
  حرف **داخلا_سٽون = عدم;
  bool use_reservoir_sampling = false;
  bool repeat = false;

  صحيح optc;
  صحيح n_operands;
  حرف **عمليو_2;
  size_t n_سٽون;
  حرف **سٽ = عدم;
  بنية linebuffer *reservoir = عدم;
  بنية randint_ذريعو *randint_ذريعو;
  size_t *permutation = عدم;
  صحيح i;

  initialize_main (&argc, &argv);
  سيٽ_پروگرام_name (argv[0]);
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
          إذا (داخلا_range)
            error (EXIT_FAILURE, 0, _("multiple -i options specified"));
          داخلا_range = true;

          uintmax_t u;
          حرف *lo_پڇاڙي;
          strtol_error err = xstrtoumax (optarg, &lo_پڇاڙي, 10, &u, عدم);
          إذا (err == LONGINT_OK)
            {
              lo_داخلا = u;
              إذا (lo_داخلا != u)
                err = LONGINT_حد_کان_مٿي;
              وإلا إذا (*lo_پڇاڙي != '-')
                err = LONGINT_INVALID;
              وإلا
                {
                  err = xstrtoumax (lo_پڇاڙي + 1, عدم, 10, &u, "");
                  إذا (err == LONGINT_OK)
                    {
                      hi_داخلا = u;
                      إذا (hi_داخلا != u)
                        err = LONGINT_حد_کان_مٿي;
                    }
                }
            }

          n_سٽون = hi_داخلا - lo_داخلا + 1;

          إذا (err != LONGINT_OK || (lo_داخلا <= hi_داخلا) == (n_سٽون == 0))
            error (EXIT_FAILURE, err == LONGINT_حد_کان_مٿي ? EOVERFLOW : 0,
                   "%s: %s", _("invalid input range"), quote (optarg));
        }
        اكسر;

      حالة 'n':
        {
          uintmax_t argval;
          strtol_error e = xstrtoumax (optarg, عدم, 10, &argval, "");

          إذا (e == LONGINT_OK)
            سر_سٽون = MIN (سر_سٽون, argval);
          وإلا إذا (e != LONGINT_حد_کان_مٿي)
            error (EXIT_FAILURE, 0, _("invalid line count: %s"),
                   quote (optarg));
        }
        اكسر;

      حالة 'o':
        إذا (outfile && !streq (outfile, optarg))
          error (EXIT_FAILURE, 0, _("multiple output files specified"));
        outfile = optarg;
        اكسر;

      حالة RANDOM_ذريعو_OPTION:
        إذا (random_ذريعو && !streq (random_ذريعو, optarg))
          error (EXIT_FAILURE, 0, _("multiple random sources specified"));
        random_ذريعو = optarg;
        اكسر;

      حالة 'r':
        repeat = true;
        اكسر;

      حالة 'z':
        eolbyte = '\0';
        اكسر;

      case_GETOPT_HELP_CHAR;
      case_GETOPT_نسخو_CHAR (پروگرام_NAME, AUTHORS);
      افتراضي:
        usage (EXIT_FAILURE);
      }

  n_operands = argc - optind;
  عمليو_2 = argv + optind;

  /* Check invalid usage.  */
  إذا (echo && داخلا_range)
    {
      error (0, 0, _("cannot combine -e and -i options"));
      usage (EXIT_FAILURE);
    }
  إذا (داخلا_range ? 0 < n_operands : !echo && 1 < n_operands)
    {
      error (0, 0, _("extra operand %s"), quote (عمليو_2[!داخلا_range]));
      usage (EXIT_FAILURE);
    }

  /* Prepare input.  */
  إذا (سر_سٽون == 0)
    {
      n_سٽون = 0;
      سٽ = عدم;
    }
  وإلا إذا (echo)
    {
      داخلا_from_argv (عمليو_2, n_operands, eolbyte);
      n_سٽون = n_operands;
      سٽ = عمليو_2;
    }
  وإلا إذا (داخلا_range)
    {
      IF_LINT (n_سٽون = hi_داخلا - lo_داخلا + 1); /* Avoid GCC 10 warning.  */
      سٽ = عدم;
    }
  وإلا
    {
      /* If an input file is specified, re-open it as stdin.  */
      إذا (n_operands == 1
          && ! (streq (عمليو_2[0], "-")
                || freopen (عمليو_2[0], "r", stdin)))
        error (EXIT_FAILURE, errno, "%s", quotef (عمليو_2[0]));

      fadvise (stdin, FADVISE_SEQUENTIAL);

      إذا (repeat || سر_سٽون == MIN (IDX_MAX, SIZE_MAX)
          || داخلا_سائيز () <= RESERVOIR_MIN_داخلا)
        {
          n_سٽون = read_داخلا (stdin, eolbyte, &داخلا_سٽون);
          سٽ = داخلا_سٽون;
        }
      وإلا
        {
          use_reservoir_sampling = true;
          n_سٽون = SIZE_MAX;   /* unknown number of input lines, for now.  */
        }
    }

  /* The adjusted head line count; can be less than HEAD_LINES if the
     input is small and if not repeating.  */
  idx_t ahead_سٽون = repeat || سر_سٽون < n_سٽون ? سر_سٽون : n_سٽون;

  randint_ذريعو = randint_all_new (random_ذريعو,
                                    (use_reservoir_sampling || repeat
                                     ? SIZE_MAX
                                     : randperm_bound (ahead_سٽون, n_سٽون)));
  إذا (! randint_ذريعو)
    error (EXIT_FAILURE, errno, "%s",
           quotef (random_ذريعو ? random_ذريعو : "getrandom"));

  إذا (use_reservoir_sampling)
    {
      /* Instead of reading the entire file into 'line',
         use reservoir-sampling to store just AHEAD_LINES random lines.  */
      n_سٽون = read_داخلا_reservoir_sampling (stdin, eolbyte, ahead_سٽون,
                                               randint_ذريعو, &reservoir);
      ahead_سٽون = n_سٽون;
    }

  /* Close stdin now, rather than earlier, so that randint_all_new
     doesn't have to worry about opening something other than
     stdin.  */
  إذا (! (سر_سٽون == 0 || echo || داخلا_range || fclose (stdin) == 0))
    error (EXIT_FAILURE, errno, _("read error"));

  إذا (!repeat)
    permutation = randperm_new (randint_ذريعو, ahead_سٽون, n_سٽون);

  إذا (outfile && ! freopen (outfile, "w", stdout))
    error (EXIT_FAILURE, errno, "%s", quotef (outfile));

  /* Generate output according to requested method */
  إذا (repeat)
    {
      إذا (سر_سٽون == 0)
        i = 0;
      وإلا
        {
          إذا (n_سٽون == 0)
            error (EXIT_FAILURE, 0, _("no lines to repeat"));
          إذا (داخلا_range)
            i = write_random_numbers (randint_ذريعو, ahead_سٽون,
                                      lo_داخلا, hi_داخلا, eolbyte);
          وإلا
            i = write_random_سٽون (randint_ذريعو, ahead_سٽون, سٽ, n_سٽون);
        }
    }
  وإلا
    {
      إذا (use_reservoir_sampling)
        i = write_permuted_output_reservoir (n_سٽون, reservoir, permutation);
      وإلا إذا (داخلا_range)
        i = write_permuted_numbers (ahead_سٽون, lo_داخلا,
                                    permutation, eolbyte);
      وإلا
        i = write_permuted_سٽون (ahead_سٽون, سٽ, permutation);
    }

  إذا (i != 0)
    write_error ();

  IF_LINT (randint_all_free (randint_ذريعو));  /* For older valgrind.  */

  main_exit (EXIT_SUCCESS);
}
