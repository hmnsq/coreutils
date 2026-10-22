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
#تعریف_کرو پروگرام_NAME "shuf"

#تعریف_کرو AUTHORS proper_name ("Paul Eggert")

/* reservoir-sampling introduces CPU overhead for small inputs.
   So only enable it for inputs >= this limit.
   This limit was determined using these commands:
     $ for p in $(seq 7); do src/seq $((10**$p)) > 10p$p.in; done
     $ for p in $(seq 7); do time shuf-nores -n10 10p$p.in >/dev/null; done
     $ for p in $(seq 7); do time shuf -n10 10p$p.in >/dev/null; done  .*/
شمار { RESERVOIR_MIN_داخلہ = 8192 * 1024 };


خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("\
Usage: %s [OPTION]... [FILE]\n\
  or:  %s -e [OPTION]... [ARG]...\n\
  or:  %s -i LO-HI [OPTION]...\n\
"),
              پروگرام_name, پروگرام_name, پروگرام_name);
      fputs (_("\
Write a random permutation کا the داخلہ سطریں مقصد standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -e, --echo                treat each ARG as an داخلہ سطر\n\
"));
      oputs (_("\
  -i, --داخلہ-range=LO-HI   treat each نمبر LO through HI as an داخلہ سطر\n\
"));
      oputs (_("\
  -n, --سر-گنتی=گنتی_2    output at most گنتی_2 سطریں\n\
"));
      oputs (_("\
  -o, --output=FILE         اكتب نتیجہ مقصد FILE instead کا standard output\n\
"));
      oputs (_("\
      --random-ماخذ_2=FILE  get random بائٹیں from FILE\n\
"));
      oputs (_("\
  -r, --repeat              output سطریں can be repeated\n\
"));
      oputs (_("\
  -z, --zero-terminated     سطر delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      emit_ancillary_info (پروگرام_NAME);
    }

  باہر_نکلو (حالت_2);
}

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
شمار
{
  RANDOM_ماخذ_OPTION = CHAR_MAX + 1
};

ساکن ساخت option ثابت long_opts[] =
{
  {"echo", no_دلیل, تہی, 'e'},
  {"input-range", required_دلیل, تہی, 'i'},
  {"head-count", required_دلیل, تہی, 'n'},
  {"output", required_دلیل, تہی, 'o'},
  {"random-source", required_دلیل, تہی, RANDOM_ماخذ_OPTION},
  {"repeat", no_دلیل, تہی, 'r'},
  {"zero-terminated", no_دلیل, تہی, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0},
};

ساکن خالی
داخلہ_from_argv (حرف **عملیہ_2, صحیح n_operands, حرف eolbyte)
{
  حرف *p;
  size_t سائز = n_operands;
  صحیح i;

  برائے (i = 0; i < n_operands; i++)
    سائز += لڑی_لمبائی (عملیہ_2[i]);
  p = xmalloc (سائز);

  برائے (i = 0; i < n_operands; i++)
    {
      حرف *p1 = stpcpy (p, عملیہ_2[i]);
      عملیہ_2[i] = p;
      p = p1;
      *p++ = eolbyte;
    }

  عملیہ_2[n_operands] = p;
}

/* Return the start of the next line after LINE, which is guaranteed
   to end in EOLBYTE.  */

ساکن حرف *
اگلا_سطر (حرف *سطر, حرف eolbyte)
{
  حرف *p = rawmemchr (سطر, eolbyte);
  واپس p + 1;
}

/* Return the size of the input if possible or OFF_T_MAX if not.  */

ساکن off_t
داخلہ_سائز (خالی)
{
  off_t file_سائز;

  ساخت stat stat_buf;
  اگر (fstat (STDIN_FILENO, &stat_buf) != 0)
    واپس OFF_T_MAX;
  اگر (usable_st_سائز (&stat_buf))
    file_سائز = stat_buf.st_سائز;
  ورنہ
    واپس OFF_T_MAX;

  off_t داخلہ_انحراف = lseek (STDIN_FILENO, 0, SEEK_CUR);
  اگر (داخلہ_انحراف < 0)
    واپس OFF_T_MAX;

  file_سائز -= داخلہ_انحراف;

  واپس file_سائز;
}

/* Read all lines and store up to K permuted lines in *OUT_RSRV.
   Return the number of lines read, up to a maximum of K.  */

ساکن idx_t
read_داخلہ_reservoir_sampling (FILE *اندر, حرف eolbyte, idx_t k,
                               ساخت randint_ماخذ *s,
                               ساخت linebuffer **برآمد_rsrv)
{
  randint n_سطریں = 0;
  idx_t n_alloc_سطریں = 0;
  ساخت linebuffer *سطر = تہی;
  ساخت linebuffer *rsrv = تہی;

  /* Fill the first K lines, directly into the reservoir.  */
  برائے (n_سطریں = 0; n_سطریں < k; n_سطریں++)
    {
      /* Enlarge reservoir if needed.  */
      اگر (n_سطریں == n_alloc_سطریں)
        {
          idx_t old = n_alloc_سطریں;
          rsrv = xpalloc (rsrv, &n_alloc_سطریں, 1, k, حجم *rsrv);
          memset (&rsrv[n_سطریں], 0, (n_alloc_سطریں - old) * حجم *rsrv);
        }

      سطر = readlinebuffer_delim (&rsrv[n_سطریں], اندر, eolbyte);
      اگر (!سطر)
        توڑو;
    }

  /* last line wasn't null - so there may be more lines to read.  */
  اگر (سطر != تہی)
    {
      ساخت linebuffer dummy;
      initbuffer (&dummy);  /* space for lines not put in reservoir.  */

      /* Choose the fate of the next line, with decreasing probability (as
         n_lines increases in size).

         If the line will be used, store it directly in the reservoir.
         Otherwise, store it in dummy space.

         With 'struct linebuffer', storing into existing buffer will reduce
         re-allocations (will only re-allocate if the new line is longer than
         the currently allocated space).  */
      کرو
        {
          randint j = randint_انتخاب (s, n_سطریں + 1);  /* 0 .. n_lines.  */
          سطر = (j < k) ? (&rsrv[j]) : (&dummy);
        }
      جب (readlinebuffer_delim (سطر, اندر, eolbyte) != تہی && n_سطریں++);

      اگر (! n_سطریں)
        error (EXIT_FAILURE, EOVERFLOW, _("too many input lines"));

      freebuffer (&dummy);
    }

  /* no more input lines, or an input error.  */
  اگر (ferror (اندر))
    error (EXIT_FAILURE, errno, _("read error"));

  *برآمد_rsrv = rsrv;
  واپس MIN (k, n_سطریں);
}

ساکن صحیح
write_permuted_output_reservoir (size_t n_سطریں, ساخت linebuffer *سطریں,
                                 size_t ثابت *permutation)
{
  برائے (size_t i = 0; i < n_سطریں; i++)
    {
      ثابت ساخت linebuffer *p = &سطریں[permutation[i]];
      اگر (fwrite (p->بفر, حجم (حرف), p->لمبائی, stdout) != p->لمبائی)
        واپس -1;
    }

  واپس 0;
}

/* Read data from file IN.  Input lines are delimited by EOLBYTE;
   silently append a trailing EOLBYTE if the file ends in some other
   byte.  Store a pointer to the resulting array of lines into *PLINE.
   Return the number of lines read.  Report an error and exit on
   failure.  */

ساکن size_t
read_داخلہ (FILE *اندر, حرف eolbyte, حرف ***pline)
{
  حرف *p;
  حرف *buf = تہی;
  size_t مستعمل;
  حرف *lim;
  حرف **سطر;
  size_t n_سطریں;

  /* TODO: We should limit the amount of data read here,
     to less than RESERVOIR_MIN_INPUT.  I.e., adjust fread_file() to support
     taking a byte limit.  We'd then need to ensure we handle a line spanning
     this boundary.  With that in place we could set use_reservoir_sampling
     when used==RESERVOIR_MIN_INPUT, and have read_input_reservoir_sampling()
     call a wrapper function to populate a linebuffer from the internal pline
     or if none left, stdin.  Doing that would give better performance by
     avoiding the reservoir CPU overhead when reading < RESERVOIR_MIN_INPUT
     from a pipe, and allow us to dispense with the input_size() function.  */
  اگر (!(buf = fread_file (اندر, 0, &مستعمل)))
    error (EXIT_FAILURE, errno, _("read error"));

  اگر (مستعمل && buf[مستعمل - 1] != eolbyte)
    buf[مستعمل++] = eolbyte;

  lim = buf + مستعمل;

  n_سطریں = 0;
  برائے (p = buf; p < lim; p = اگلا_سطر (p, eolbyte))
    n_سطریں++;

  *pline = سطر = xnmalloc (n_سطریں + 1, حجم *سطر);

  سطر[0] = p = buf;
  برائے (size_t i = 1; i <= n_سطریں; i++)
    سطر[i] = p = اگلا_سطر (p, eolbyte);

  واپس n_سطریں;
}

/* Output N_LINES lines to stdout from LINE array,
   chosen by the indices in PERMUTATION.
   PERMUTATION and LINE must have at least N_LINES elements.
   Strings in LINE must include the line-terminator character.  */
ساکن صحیح
write_permuted_سطریں (size_t n_سطریں, حرف *ثابت *سطر,
                      size_t ثابت *permutation)
{
  برائے (size_t i = 0; i < n_سطریں; i++)
    {
      حرف *ثابت *p = سطر + permutation[i];
      size_t len = p[1] - p[0];
      اگر (fwrite (p[0], حجم *p[0], len, stdout) != len)
        واپس -1;
    }

  واپس 0;
}

/* Print NUMBER followed by EOLBYTE to standard output.
   Return false on failure, true on success.  */
ساکن bool
print_نمبر (غیرعلامتی طویل صحیح نمبر, حرف eolbyte)
{
  حرف buf[INT_BUFSIZE_BOUND (غیرعلامتی طویل صحیح)];
  حرف *p = buf + INT_STRLEN_BOUND (غیرعلامتی طویل صحیح);
  *p = eolbyte;
  کرو
    *--p = '0' + نمبر % 10;
  جب ((نمبر /= 10) != 0);
  idx_t len = buf + حجم buf - p;
  واپس fwrite (p, 1, len, stdout) == len;
}

/* Output N_LINES of numbers to stdout, from PERMUTATION array.
   PERMUTATION must have at least N_LINES elements.  */
ساکن صحیح
write_permuted_numbers (size_t n_سطریں, size_t lo_داخلہ,
                        size_t ثابت *permutation, حرف eolbyte)
{
  برائے (size_t i = 0; i < n_سطریں; i++)
    {
      غیرعلامتی طویل صحیح n = lo_داخلہ + permutation[i];
      اگر (! print_نمبر (n, eolbyte))
        واپس -1;
    }

  واپس 0;
}

/* Output COUNT numbers to stdout, chosen randomly from range
   LO_INPUT through HI_INPUT.  */
ساکن صحیح
write_random_numbers (ساخت randint_ماخذ *s, size_t گنتی,
                      size_t lo_داخلہ, size_t hi_داخلہ, حرف eolbyte)
{
  ثابت randint range = hi_داخلہ - lo_داخلہ + 1;

  برائے (size_t i = 0; i < گنتی; i++)
    {
      غیرعلامتی طویل صحیح j = lo_داخلہ + randint_انتخاب (s, range);
      اگر (! print_نمبر (j, eolbyte))
        واپس -1;
    }

  واپس 0;
}

/* Output COUNT lines to stdout from LINES array.
   LINES must have at least N_LINES elements in it.
   Strings in LINES_ must include the line-terminator character.  */
ساکن صحیح
write_random_سطریں (ساخت randint_ماخذ *s, size_t گنتی,
                    حرف *ثابت *سطریں, size_t n_سطریں)
{
  برائے (size_t i = 0; i < گنتی; i++)
    {
      ثابت randint j = randint_انتخاب (s, n_سطریں);
      حرف *ثابت *p = سطریں + j;
      size_t len = p[1] - p[0];
      اگر (fwrite (p[0], حجم *p[0], len, stdout) != len)
        واپس -1;
    }

  واپس 0;
}

صحیح
مین (صحیح argc, حرف **argv)
{
  bool echo = false;
  bool داخلہ_range = false;
  size_t lo_داخلہ = SIZE_MAX;
  size_t hi_داخلہ = 0;
  idx_t سر_سطریں = MIN (IDX_MAX, SIZE_MAX);
  حرف ثابت *outfile = تہی;
  حرف *random_ماخذ = تہی;
  حرف eolbyte = '\n';
  حرف **داخلہ_سطریں = تہی;
  bool use_reservoir_sampling = false;
  bool repeat = false;

  صحیح optc;
  صحیح n_operands;
  حرف **عملیہ_2;
  size_t n_سطریں;
  حرف **سطر = تہی;
  ساخت linebuffer *reservoir = تہی;
  ساخت randint_ماخذ *randint_ماخذ;
  size_t *permutation = تہی;
  صحیح i;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  جب ((optc = getopt_long (argc, argv, "ei:n:o:rz", long_opts, تہی))
         != -1)
    انتخاب (optc)
      {
      حالت 'e':
        echo = true;
        توڑو;

      حالت 'i':
        {
          اگر (داخلہ_range)
            error (EXIT_FAILURE, 0, _("multiple -i options specified"));
          داخلہ_range = true;

          uintmax_t u;
          حرف *lo_انتہا;
          strtol_error err = xstrtoumax (optarg, &lo_انتہا, 10, &u, تہی);
          اگر (err == LONGINT_OK)
            {
              lo_داخلہ = u;
              اگر (lo_داخلہ != u)
                err = LONGINT_حد_سے_تجاوز;
              ورنہ اگر (*lo_انتہا != '-')
                err = LONGINT_INVALID;
              ورنہ
                {
                  err = xstrtoumax (lo_انتہا + 1, تہی, 10, &u, "");
                  اگر (err == LONGINT_OK)
                    {
                      hi_داخلہ = u;
                      اگر (hi_داخلہ != u)
                        err = LONGINT_حد_سے_تجاوز;
                    }
                }
            }

          n_سطریں = hi_داخلہ - lo_داخلہ + 1;

          اگر (err != LONGINT_OK || (lo_داخلہ <= hi_داخلہ) == (n_سطریں == 0))
            error (EXIT_FAILURE, err == LONGINT_حد_سے_تجاوز ? EOVERFLOW : 0,
                   "%s: %s", _("invalid input range"), quote (optarg));
        }
        توڑو;

      حالت 'n':
        {
          uintmax_t argval;
          strtol_error e = xstrtoumax (optarg, تہی, 10, &argval, "");

          اگر (e == LONGINT_OK)
            سر_سطریں = MIN (سر_سطریں, argval);
          ورنہ اگر (e != LONGINT_حد_سے_تجاوز)
            error (EXIT_FAILURE, 0, _("invalid line count: %s"),
                   quote (optarg));
        }
        توڑو;

      حالت 'o':
        اگر (outfile && !streq (outfile, optarg))
          error (EXIT_FAILURE, 0, _("multiple output files specified"));
        outfile = optarg;
        توڑو;

      حالت RANDOM_ماخذ_OPTION:
        اگر (random_ماخذ && !streq (random_ماخذ, optarg))
          error (EXIT_FAILURE, 0, _("multiple random sources specified"));
        random_ماخذ = optarg;
        توڑو;

      حالت 'r':
        repeat = true;
        توڑو;

      حالت 'z':
        eolbyte = '\0';
        توڑو;

      case_GETOPT_HELP_CHAR;
      case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);
      طےشدہ:
        usage (EXIT_FAILURE);
      }

  n_operands = argc - optind;
  عملیہ_2 = argv + optind;

  /* Check invalid usage.  */
  اگر (echo && داخلہ_range)
    {
      error (0, 0, _("cannot combine -e and -i options"));
      usage (EXIT_FAILURE);
    }
  اگر (داخلہ_range ? 0 < n_operands : !echo && 1 < n_operands)
    {
      error (0, 0, _("extra operand %s"), quote (عملیہ_2[!داخلہ_range]));
      usage (EXIT_FAILURE);
    }

  /* Prepare input.  */
  اگر (سر_سطریں == 0)
    {
      n_سطریں = 0;
      سطر = تہی;
    }
  ورنہ اگر (echo)
    {
      داخلہ_from_argv (عملیہ_2, n_operands, eolbyte);
      n_سطریں = n_operands;
      سطر = عملیہ_2;
    }
  ورنہ اگر (داخلہ_range)
    {
      IF_LINT (n_سطریں = hi_داخلہ - lo_داخلہ + 1); /* Avoid GCC 10 warning.  */
      سطر = تہی;
    }
  ورنہ
    {
      /* If an input file is specified, re-open it as stdin.  */
      اگر (n_operands == 1
          && ! (streq (عملیہ_2[0], "-")
                || freopen (عملیہ_2[0], "r", stdin)))
        error (EXIT_FAILURE, errno, "%s", quotef (عملیہ_2[0]));

      fadvise (stdin, FADVISE_SEQUENTIAL);

      اگر (repeat || سر_سطریں == MIN (IDX_MAX, SIZE_MAX)
          || داخلہ_سائز () <= RESERVOIR_MIN_داخلہ)
        {
          n_سطریں = read_داخلہ (stdin, eolbyte, &داخلہ_سطریں);
          سطر = داخلہ_سطریں;
        }
      ورنہ
        {
          use_reservoir_sampling = true;
          n_سطریں = SIZE_MAX;   /* unknown number of input lines, for now.  */
        }
    }

  /* The adjusted head line count; can be less than HEAD_LINES if the
     input is small and if not repeating.  */
  idx_t ahead_سطریں = repeat || سر_سطریں < n_سطریں ? سر_سطریں : n_سطریں;

  randint_ماخذ = randint_all_new (random_ماخذ,
                                    (use_reservoir_sampling || repeat
                                     ? SIZE_MAX
                                     : randperm_bound (ahead_سطریں, n_سطریں)));
  اگر (! randint_ماخذ)
    error (EXIT_FAILURE, errno, "%s",
           quotef (random_ماخذ ? random_ماخذ : "getrandom"));

  اگر (use_reservoir_sampling)
    {
      /* Instead of reading the entire file into 'line',
         use reservoir-sampling to store just AHEAD_LINES random lines.  */
      n_سطریں = read_داخلہ_reservoir_sampling (stdin, eolbyte, ahead_سطریں,
                                               randint_ماخذ, &reservoir);
      ahead_سطریں = n_سطریں;
    }

  /* Close stdin now, rather than earlier, so that randint_all_new
     doesn't have to worry about opening something other than
     stdin.  */
  اگر (! (سر_سطریں == 0 || echo || داخلہ_range || fclose (stdin) == 0))
    error (EXIT_FAILURE, errno, _("read error"));

  اگر (!repeat)
    permutation = randperm_new (randint_ماخذ, ahead_سطریں, n_سطریں);

  اگر (outfile && ! freopen (outfile, "w", stdout))
    error (EXIT_FAILURE, errno, "%s", quotef (outfile));

  /* Generate output according to requested method */
  اگر (repeat)
    {
      اگر (سر_سطریں == 0)
        i = 0;
      ورنہ
        {
          اگر (n_سطریں == 0)
            error (EXIT_FAILURE, 0, _("no lines to repeat"));
          اگر (داخلہ_range)
            i = write_random_numbers (randint_ماخذ, ahead_سطریں,
                                      lo_داخلہ, hi_داخلہ, eolbyte);
          ورنہ
            i = write_random_سطریں (randint_ماخذ, ahead_سطریں, سطر, n_سطریں);
        }
    }
  ورنہ
    {
      اگر (use_reservoir_sampling)
        i = write_permuted_output_reservoir (n_سطریں, reservoir, permutation);
      ورنہ اگر (داخلہ_range)
        i = write_permuted_numbers (ahead_سطریں, lo_داخلہ,
                                    permutation, eolbyte);
      ورنہ
        i = write_permuted_سطریں (ahead_سطریں, سطر, permutation);
    }

  اگر (i != 0)
    write_error ();

  IF_LINT (randint_all_free (randint_ماخذ));  /* For older valgrind.  */

  main_exit (EXIT_SUCCESS);
}
