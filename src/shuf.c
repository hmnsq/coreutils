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
#تعریف_کړه پروګرام_NAME "shuf"

#تعریف_کړه AUTHORS proper_name ("Paul Eggert")

/* reservoir-sampling introduces CPU overhead for small inputs.
   So only enable it for inputs >= this limit.
   This limit was determined using these commands:
     $ for p in $(seq 7); do src/seq $((10**$p)) > 10p$p.in; done
     $ for p in $(seq 7); do time shuf-nores -n10 10p$p.in >/dev/null; done
     $ for p in $(seq 7); do time shuf -n10 10p$p.in >/dev/null; done  .*/
تعداد { RESERVOIR_MIN_ننوت = 8192 * 1024 };


فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کړه (_("\
Usage: %s [OPTION]... [FILE]\n\
  or:  %s -e [OPTION]... [ARG]...\n\
  or:  %s -i LO-HI [OPTION]...\n\
"),
              پروګرام_name, پروګرام_name, پروګرام_name);
      fputs (_("\
Write a random permutation د the ننوت کرښې منزل standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -e, --echo                treat each ARG as an ننوت کرښه\n\
"));
      oputs (_("\
  -i, --ننوت-range=LO-HI   treat each شمېره LO through HI as an ننوت کرښه\n\
"));
      oputs (_("\
  -n, --سر-شمېر=شمېر_2    output at most شمېر_2 کرښې\n\
"));
      oputs (_("\
  -o, --output=FILE         اكتب پایله منزل FILE instead د standard output\n\
"));
      oputs (_("\
      --random-سرچینه_2=FILE  get random بایټونه from FILE\n\
"));
      oputs (_("\
  -r, --repeat              output کرښې can be repeated\n\
"));
      oputs (_("\
  -z, --zero-terminated     کرښه delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);
      emit_ancillary_info (پروګرام_NAME);
    }

  وتل (حالت);
}

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
تعداد
{
  RANDOM_سرچینه_OPTION = CHAR_MAX + 1
};

ساكن بنية option ثابت long_opts[] =
{
  {"echo", no_دلیل, عدم, 'e'},
  {"input-range", required_دلیل, عدم, 'i'},
  {"head-count", required_دلیل, عدم, 'n'},
  {"output", required_دلیل, عدم, 'o'},
  {"random-source", required_دلیل, عدم, RANDOM_سرچینه_OPTION},
  {"repeat", no_دلیل, عدم, 'r'},
  {"zero-terminated", no_دلیل, عدم, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخه_OPTION_DECL},
  {عدم, 0, عدم, 0},
};

ساكن فراغ
ننوت_from_argv (حرف **عملوند_2, صحيح n_operands, حرف eolbyte)
{
  حرف *p;
  size_t اندازه = n_operands;
  صحيح i;

  لكل (i = 0; i < n_operands; i++)
    اندازه += مزي_اوږدوالی (عملوند_2[i]);
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
بل_کرښه (حرف *کرښه, حرف eolbyte)
{
  حرف *p = rawmemchr (کرښه, eolbyte);
  أرجع p + 1;
}

/* Return the size of the input if possible or OFF_T_MAX if not.  */

ساكن off_t
ننوت_اندازه (فراغ)
{
  off_t file_اندازه;

  بنية stat stat_buf;
  إذا (fstat (STDIN_FILENO, &stat_buf) != 0)
    أرجع OFF_T_MAX;
  إذا (usable_st_اندازه (&stat_buf))
    file_اندازه = stat_buf.st_اندازه;
  وإلا
    أرجع OFF_T_MAX;

  off_t ننوت_دننه_واټن = lseek (STDIN_FILENO, 0, SEEK_CUR);
  إذا (ننوت_دننه_واټن < 0)
    أرجع OFF_T_MAX;

  file_اندازه -= ننوت_دننه_واټن;

  أرجع file_اندازه;
}

/* Read all lines and store up to K permuted lines in *OUT_RSRV.
   Return the number of lines read, up to a maximum of K.  */

ساكن idx_t
read_ننوت_reservoir_sampling (FILE *دننه, حرف eolbyte, idx_t k,
                               بنية randint_سرچینه *s,
                               بنية linebuffer **بهر_rsrv)
{
  randint n_کرښې = 0;
  idx_t n_alloc_کرښې = 0;
  بنية linebuffer *کرښه = عدم;
  بنية linebuffer *rsrv = عدم;

  /* Fill the first K lines, directly into the reservoir.  */
  لكل (n_کرښې = 0; n_کرښې < k; n_کرښې++)
    {
      /* Enlarge reservoir if needed.  */
      إذا (n_کرښې == n_alloc_کرښې)
        {
          idx_t old = n_alloc_کرښې;
          rsrv = xpalloc (rsrv, &n_alloc_کرښې, 1, k, حجم *rsrv);
          memset (&rsrv[n_کرښې], 0, (n_alloc_کرښې - old) * حجم *rsrv);
        }

      کرښه = readlinebuffer_delim (&rsrv[n_کرښې], دننه, eolbyte);
      إذا (!کرښه)
        اكسر;
    }

  /* last line wasn't null - so there may be more lines to read.  */
  إذا (کرښه != عدم)
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
          randint j = randint_ټاکنه (s, n_کرښې + 1);  /* 0 .. n_lines.  */
          کرښه = (j < k) ? (&rsrv[j]) : (&dummy);
        }
      طالما (readlinebuffer_delim (کرښه, دننه, eolbyte) != عدم && n_کرښې++);

      إذا (! n_کرښې)
        error (EXIT_FAILURE, EOVERFLOW, _("too many input lines"));

      freebuffer (&dummy);
    }

  /* no more input lines, or an input error.  */
  إذا (ferror (دننه))
    error (EXIT_FAILURE, errno, _("read error"));

  *بهر_rsrv = rsrv;
  أرجع MIN (k, n_کرښې);
}

ساكن صحيح
write_permuted_output_reservoir (size_t n_کرښې, بنية linebuffer *کرښې,
                                 size_t ثابت *permutation)
{
  لكل (size_t i = 0; i < n_کرښې; i++)
    {
      ثابت بنية linebuffer *p = &کرښې[permutation[i]];
      إذا (fwrite (p->بفر, حجم (حرف), p->اوږدتیا, stdout) != p->اوږدتیا)
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
read_ننوت (FILE *دننه, حرف eolbyte, حرف ***pline)
{
  حرف *p;
  حرف *buf = عدم;
  size_t کارول_شوی;
  حرف *lim;
  حرف **کرښه;
  size_t n_کرښې;

  /* TODO: We should limit the amount of data read here,
     to less than RESERVOIR_MIN_INPUT.  I.e., adjust fread_file() to support
     taking a byte limit.  We'd then need to ensure we handle a line spanning
     this boundary.  With that in place we could set use_reservoir_sampling
     when used==RESERVOIR_MIN_INPUT, and have read_input_reservoir_sampling()
     call a wrapper function to populate a linebuffer from the internal pline
     or if none left, stdin.  Doing that would give better performance by
     avoiding the reservoir CPU overhead when reading < RESERVOIR_MIN_INPUT
     from a pipe, and allow us to dispense with the input_size() function.  */
  إذا (!(buf = fread_file (دننه, 0, &کارول_شوی)))
    error (EXIT_FAILURE, errno, _("read error"));

  إذا (کارول_شوی && buf[کارول_شوی - 1] != eolbyte)
    buf[کارول_شوی++] = eolbyte;

  lim = buf + کارول_شوی;

  n_کرښې = 0;
  لكل (p = buf; p < lim; p = بل_کرښه (p, eolbyte))
    n_کرښې++;

  *pline = کرښه = xnmalloc (n_کرښې + 1, حجم *کرښه);

  کرښه[0] = p = buf;
  لكل (size_t i = 1; i <= n_کرښې; i++)
    کرښه[i] = p = بل_کرښه (p, eolbyte);

  أرجع n_کرښې;
}

/* Output N_LINES lines to stdout from LINE array,
   chosen by the indices in PERMUTATION.
   PERMUTATION and LINE must have at least N_LINES elements.
   Strings in LINE must include the line-terminator character.  */
ساكن صحيح
write_permuted_کرښې (size_t n_کرښې, حرف *ثابت *کرښه,
                      size_t ثابت *permutation)
{
  لكل (size_t i = 0; i < n_کرښې; i++)
    {
      حرف *ثابت *p = کرښه + permutation[i];
      size_t len = p[1] - p[0];
      إذا (fwrite (p[0], حجم *p[0], len, stdout) != len)
        أرجع -1;
    }

  أرجع 0;
}

/* Print NUMBER followed by EOLBYTE to standard output.
   Return false on failure, true on success.  */
ساكن bool
print_شمېره (بینښه اوږد صحيح شمېره, حرف eolbyte)
{
  حرف buf[INT_BUFSIZE_BOUND (بینښه اوږد صحيح)];
  حرف *p = buf + INT_STRLEN_BOUND (بینښه اوږد صحيح);
  *p = eolbyte;
  افعل
    *--p = '0' + شمېره % 10;
  طالما ((شمېره /= 10) != 0);
  idx_t len = buf + حجم buf - p;
  أرجع fwrite (p, 1, len, stdout) == len;
}

/* Output N_LINES of numbers to stdout, from PERMUTATION array.
   PERMUTATION must have at least N_LINES elements.  */
ساكن صحيح
write_permuted_numbers (size_t n_کرښې, size_t lo_ننوت,
                        size_t ثابت *permutation, حرف eolbyte)
{
  لكل (size_t i = 0; i < n_کرښې; i++)
    {
      بینښه اوږد صحيح n = lo_ننوت + permutation[i];
      إذا (! print_شمېره (n, eolbyte))
        أرجع -1;
    }

  أرجع 0;
}

/* Output COUNT numbers to stdout, chosen randomly from range
   LO_INPUT through HI_INPUT.  */
ساكن صحيح
write_random_numbers (بنية randint_سرچینه *s, size_t شمېر,
                      size_t lo_ننوت, size_t hi_ننوت, حرف eolbyte)
{
  ثابت randint range = hi_ننوت - lo_ننوت + 1;

  لكل (size_t i = 0; i < شمېر; i++)
    {
      بینښه اوږد صحيح j = lo_ننوت + randint_ټاکنه (s, range);
      إذا (! print_شمېره (j, eolbyte))
        أرجع -1;
    }

  أرجع 0;
}

/* Output COUNT lines to stdout from LINES array.
   LINES must have at least N_LINES elements in it.
   Strings in LINES_ must include the line-terminator character.  */
ساكن صحيح
write_random_کرښې (بنية randint_سرچینه *s, size_t شمېر,
                    حرف *ثابت *کرښې, size_t n_کرښې)
{
  لكل (size_t i = 0; i < شمېر; i++)
    {
      ثابت randint j = randint_ټاکنه (s, n_کرښې);
      حرف *ثابت *p = کرښې + j;
      size_t len = p[1] - p[0];
      إذا (fwrite (p[0], حجم *p[0], len, stdout) != len)
        أرجع -1;
    }

  أرجع 0;
}

صحيح
اصلي (صحيح argc, حرف **argv)
{
  bool echo = false;
  bool ننوت_range = false;
  size_t lo_ننوت = SIZE_MAX;
  size_t hi_ننوت = 0;
  idx_t سر_کرښې = MIN (IDX_MAX, SIZE_MAX);
  حرف ثابت *outfile = عدم;
  حرف *random_سرچینه = عدم;
  حرف eolbyte = '\n';
  حرف **ننوت_کرښې = عدم;
  bool use_reservoir_sampling = false;
  bool repeat = false;

  صحيح optc;
  صحيح n_operands;
  حرف **عملوند_2;
  size_t n_کرښې;
  حرف **کرښه = عدم;
  بنية linebuffer *reservoir = عدم;
  بنية randint_سرچینه *randint_سرچینه;
  size_t *permutation = عدم;
  صحيح i;

  initialize_main (&argc, &argv);
  سېټ_پروګرام_name (argv[0]);
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
          إذا (ننوت_range)
            error (EXIT_FAILURE, 0, _("multiple -i options specified"));
          ننوت_range = true;

          uintmax_t u;
          حرف *lo_پای_ټکی;
          strtol_error err = xstrtoumax (optarg, &lo_پای_ټکی, 10, &u, عدم);
          إذا (err == LONGINT_OK)
            {
              lo_ننوت = u;
              إذا (lo_ننوت != u)
                err = LONGINT_ډکېدنه;
              وإلا إذا (*lo_پای_ټکی != '-')
                err = LONGINT_INVALID;
              وإلا
                {
                  err = xstrtoumax (lo_پای_ټکی + 1, عدم, 10, &u, "");
                  إذا (err == LONGINT_OK)
                    {
                      hi_ننوت = u;
                      إذا (hi_ننوت != u)
                        err = LONGINT_ډکېدنه;
                    }
                }
            }

          n_کرښې = hi_ننوت - lo_ننوت + 1;

          إذا (err != LONGINT_OK || (lo_ننوت <= hi_ننوت) == (n_کرښې == 0))
            error (EXIT_FAILURE, err == LONGINT_ډکېدنه ? EOVERFLOW : 0,
                   "%s: %s", _("invalid input range"), quote (optarg));
        }
        اكسر;

      حالة 'n':
        {
          uintmax_t argval;
          strtol_error e = xstrtoumax (optarg, عدم, 10, &argval, "");

          إذا (e == LONGINT_OK)
            سر_کرښې = MIN (سر_کرښې, argval);
          وإلا إذا (e != LONGINT_ډکېدنه)
            error (EXIT_FAILURE, 0, _("invalid line count: %s"),
                   quote (optarg));
        }
        اكسر;

      حالة 'o':
        إذا (outfile && !streq (outfile, optarg))
          error (EXIT_FAILURE, 0, _("multiple output files specified"));
        outfile = optarg;
        اكسر;

      حالة RANDOM_سرچینه_OPTION:
        إذا (random_سرچینه && !streq (random_سرچینه, optarg))
          error (EXIT_FAILURE, 0, _("multiple random sources specified"));
        random_سرچینه = optarg;
        اكسر;

      حالة 'r':
        repeat = true;
        اكسر;

      حالة 'z':
        eolbyte = '\0';
        اكسر;

      case_GETOPT_HELP_CHAR;
      case_GETOPT_نسخه_CHAR (پروګرام_NAME, AUTHORS);
      افتراضي:
        usage (EXIT_FAILURE);
      }

  n_operands = argc - optind;
  عملوند_2 = argv + optind;

  /* Check invalid usage.  */
  إذا (echo && ننوت_range)
    {
      error (0, 0, _("cannot combine -e and -i options"));
      usage (EXIT_FAILURE);
    }
  إذا (ننوت_range ? 0 < n_operands : !echo && 1 < n_operands)
    {
      error (0, 0, _("extra operand %s"), quote (عملوند_2[!ننوت_range]));
      usage (EXIT_FAILURE);
    }

  /* Prepare input.  */
  إذا (سر_کرښې == 0)
    {
      n_کرښې = 0;
      کرښه = عدم;
    }
  وإلا إذا (echo)
    {
      ننوت_from_argv (عملوند_2, n_operands, eolbyte);
      n_کرښې = n_operands;
      کرښه = عملوند_2;
    }
  وإلا إذا (ننوت_range)
    {
      IF_LINT (n_کرښې = hi_ننوت - lo_ننوت + 1); /* Avoid GCC 10 warning.  */
      کرښه = عدم;
    }
  وإلا
    {
      /* If an input file is specified, re-open it as stdin.  */
      إذا (n_operands == 1
          && ! (streq (عملوند_2[0], "-")
                || freopen (عملوند_2[0], "r", stdin)))
        error (EXIT_FAILURE, errno, "%s", quotef (عملوند_2[0]));

      fadvise (stdin, FADVISE_SEQUENTIAL);

      إذا (repeat || سر_کرښې == MIN (IDX_MAX, SIZE_MAX)
          || ننوت_اندازه () <= RESERVOIR_MIN_ننوت)
        {
          n_کرښې = read_ننوت (stdin, eolbyte, &ننوت_کرښې);
          کرښه = ننوت_کرښې;
        }
      وإلا
        {
          use_reservoir_sampling = true;
          n_کرښې = SIZE_MAX;   /* unknown number of input lines, for now.  */
        }
    }

  /* The adjusted head line count; can be less than HEAD_LINES if the
     input is small and if not repeating.  */
  idx_t ahead_کرښې = repeat || سر_کرښې < n_کرښې ? سر_کرښې : n_کرښې;

  randint_سرچینه = randint_all_new (random_سرچینه,
                                    (use_reservoir_sampling || repeat
                                     ? SIZE_MAX
                                     : randperm_bound (ahead_کرښې, n_کرښې)));
  إذا (! randint_سرچینه)
    error (EXIT_FAILURE, errno, "%s",
           quotef (random_سرچینه ? random_سرچینه : "getrandom"));

  إذا (use_reservoir_sampling)
    {
      /* Instead of reading the entire file into 'line',
         use reservoir-sampling to store just AHEAD_LINES random lines.  */
      n_کرښې = read_ننوت_reservoir_sampling (stdin, eolbyte, ahead_کرښې,
                                               randint_سرچینه, &reservoir);
      ahead_کرښې = n_کرښې;
    }

  /* Close stdin now, rather than earlier, so that randint_all_new
     doesn't have to worry about opening something other than
     stdin.  */
  إذا (! (سر_کرښې == 0 || echo || ننوت_range || fclose (stdin) == 0))
    error (EXIT_FAILURE, errno, _("read error"));

  إذا (!repeat)
    permutation = randperm_new (randint_سرچینه, ahead_کرښې, n_کرښې);

  إذا (outfile && ! freopen (outfile, "w", stdout))
    error (EXIT_FAILURE, errno, "%s", quotef (outfile));

  /* Generate output according to requested method */
  إذا (repeat)
    {
      إذا (سر_کرښې == 0)
        i = 0;
      وإلا
        {
          إذا (n_کرښې == 0)
            error (EXIT_FAILURE, 0, _("no lines to repeat"));
          إذا (ننوت_range)
            i = write_random_numbers (randint_سرچینه, ahead_کرښې,
                                      lo_ننوت, hi_ننوت, eolbyte);
          وإلا
            i = write_random_کرښې (randint_سرچینه, ahead_کرښې, کرښه, n_کرښې);
        }
    }
  وإلا
    {
      إذا (use_reservoir_sampling)
        i = write_permuted_output_reservoir (n_کرښې, reservoir, permutation);
      وإلا إذا (ننوت_range)
        i = write_permuted_numbers (ahead_کرښې, lo_ننوت,
                                    permutation, eolbyte);
      وإلا
        i = write_permuted_کرښې (ahead_کرښې, کرښه, permutation);
    }

  إذا (i != 0)
    write_error ();

  IF_LINT (randint_all_free (randint_سرچینه));  /* For older valgrind.  */

  main_exit (EXIT_SUCCESS);
}
