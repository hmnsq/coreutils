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

#অন্তর্ভাৱ <config.h>

#অন্তর্ভাৱ <sys/types.h>
#অন্তর্ভাৱ <getopt.h>
#অন্তর্ভাৱ "system.h"

#অন্তর্ভাৱ "fadvise.h"
#অন্তর্ভাৱ "linebuffer.h"
#অন্তর্ভাৱ "quote.h"
#অন্তর্ভাৱ "randint.h"
#অন্তর্ভাৱ "randperm.h"
#অন্তর্ভাৱ "read-file.h"
#অন্তর্ভাৱ "stdio--.h"
#অন্তর্ভাৱ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "shuf"

#define AUTHORS proper_name ("Paul Eggert")

/* reservoir-sampling introduces CPU overhead for small inputs.
   So only enable it for inputs >= this limit.
   This limit was determined using these commands:
     $ for p in $(seq 7); do src/seq $((10**$p)) > 10p$p.in; done
     $ for p in $(seq 7); do time shuf-nores -n10 10p$p.in >/dev/null; done
     $ for p in $(seq 7); do time shuf -n10 10p$p.in >/dev/null; done  .*/
গণনা { RESERVOIR_MIN_প্ৰবেশ = 8192 * 1024 };


শূন্য
usage (পূর্ণাঙ্ক status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]\n\
  or:  %s -e [OPTION]... [ARG]...\n\
  or:  %s -i LO-HI [OPTION]...\n\
"),
              program_name, program_name, program_name);
      fputs (_("\
Write a random permutation of the প্ৰবেশ সাৰিগুলি to standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -e, --echo                treat each ARG as an প্ৰবেশ সাৰি\n\
"));
      oputs (_("\
  -i, --প্ৰবেশ-range=LO-HI   treat each সংখ্যা LO through HI as an প্ৰবেশ সাৰি\n\
"));
      oputs (_("\
  -n, --head-গুনতি=গুনতি_2    output at most গুনতি_2 সাৰিগুলি\n\
"));
      oputs (_("\
  -o, --output=FILE         লিখ result to FILE instead of standard output\n\
"));
      oputs (_("\
      --random-source=FILE  get random bytes from FILE\n\
"));
      oputs (_("\
  -r, --repeat              output সাৰিগুলি can be repeated\n\
"));
      oputs (_("\
  -z, --zero-terminated     সাৰি delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }

  exit (status);
}

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
গণনা
{
  RANDOM_SOURCE_OPTION = CHAR_MAX + 1
};

স্থির সংরচনা option ধ্রুৱ long_opts[] =
{
  {"echo", no_argument, রিক্ত, 'e'},
  {"input-range", required_argument, রিক্ত, 'i'},
  {"head-count", required_argument, রিক্ত, 'n'},
  {"output", required_argument, রিক্ত, 'o'},
  {"random-source", required_argument, রিক্ত, RANDOM_SOURCE_OPTION},
  {"repeat", no_argument, রিক্ত, 'r'},
  {"zero-terminated", no_argument, রিক্ত, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {রিক্ত, 0, রিক্ত, 0},
};

স্থির শূন্য
প্ৰবেশ_from_argv (ৱর্ণ **operand, পূর্ণাঙ্ক n_operands, ৱর্ণ eolbyte)
{
  ৱর্ণ *p;
  size_t size = n_operands;
  পূর্ণাঙ্ক i;

  বাবে (i = 0; i < n_operands; i++)
    size += strlen (operand[i]);
  p = xmalloc (size);

  বাবে (i = 0; i < n_operands; i++)
    {
      ৱর্ণ *p1 = stpcpy (p, operand[i]);
      operand[i] = p;
      p = p1;
      *p++ = eolbyte;
    }

  operand[n_operands] = p;
}

/* Return the start of the next line after LINE, which is guaranteed
   to end in EOLBYTE.  */

স্থির ৱর্ণ *
next_সাৰি (ৱর্ণ *সাৰি, ৱর্ণ eolbyte)
{
  ৱর্ণ *p = rawmemchr (সাৰি, eolbyte);
  ঘূৰাই p + 1;
}

/* Return the size of the input if possible or OFF_T_MAX if not.  */

স্থির off_t
প্ৰবেশ_size (শূন্য)
{
  off_t file_size;

  সংরচনা stat stat_buf;
  যদি (fstat (STDIN_FILENO, &stat_buf) != 0)
    ঘূৰাই OFF_T_MAX;
  যদি (usable_st_size (&stat_buf))
    file_size = stat_buf.st_size;
  অন্যথা
    ঘূৰাই OFF_T_MAX;

  off_t প্ৰবেশ_offset = lseek (STDIN_FILENO, 0, SEEK_CUR);
  যদি (প্ৰবেশ_offset < 0)
    ঘূৰাই OFF_T_MAX;

  file_size -= প্ৰবেশ_offset;

  ঘূৰাই file_size;
}

/* Read all lines and store up to K permuted lines in *OUT_RSRV.
   Return the number of lines read, up to a maximum of K.  */

স্থির idx_t
read_প্ৰবেশ_reservoir_sampling (FILE *in, ৱর্ণ eolbyte, idx_t k,
                               সংরচনা randint_source *s,
                               সংরচনা linebuffer **out_rsrv)
{
  randint n_সাৰিগুলি = 0;
  idx_t n_alloc_সাৰিগুলি = 0;
  সংরচনা linebuffer *সাৰি = রিক্ত;
  সংরচনা linebuffer *rsrv = রিক্ত;

  /* Fill the first K lines, directly into the reservoir.  */
  বাবে (n_সাৰিগুলি = 0; n_সাৰিগুলি < k; n_সাৰিগুলি++)
    {
      /* Enlarge reservoir if needed.  */
      যদি (n_সাৰিগুলি == n_alloc_সাৰিগুলি)
        {
          idx_t old = n_alloc_সাৰিগুলি;
          rsrv = xpalloc (rsrv, &n_alloc_সাৰিগুলি, 1, k, পরিমাণ *rsrv);
          memset (&rsrv[n_সাৰিগুলি], 0, (n_alloc_সাৰিগুলি - old) * পরিমাণ *rsrv);
        }

      সাৰি = readlinebuffer_delim (&rsrv[n_সাৰিগুলি], in, eolbyte);
      যদি (!সাৰি)
        ভঙ্গ;
    }

  /* last line wasn't null - so there may be more lines to read.  */
  যদি (সাৰি != রিক্ত)
    {
      সংরচনা linebuffer dummy;
      initbuffer (&dummy);  /* space for lines not put in reservoir.  */

      /* Choose the fate of the next line, with decreasing probability (as
         n_lines increases in size).

         If the line will be used, store it directly in the reservoir.
         Otherwise, store it in dummy space.

         With 'struct linebuffer', storing into existing buffer will reduce
         re-allocations (will only re-allocate if the new line is longer than
         the currently allocated space).  */
      কৰা
        {
          randint j = randint_choose (s, n_সাৰিগুলি + 1);  /* 0 .. n_lines.  */
          সাৰি = (j < k) ? (&rsrv[j]) : (&dummy);
        }
      যেতিয়ালৈ (readlinebuffer_delim (সাৰি, in, eolbyte) != রিক্ত && n_সাৰিগুলি++);

      যদি (! n_সাৰিগুলি)
        error (EXIT_FAILURE, EOVERFLOW, _("too many input lines"));

      freebuffer (&dummy);
    }

  /* no more input lines, or an input error.  */
  যদি (ferror (in))
    error (EXIT_FAILURE, errno, _("read error"));

  *out_rsrv = rsrv;
  ঘূৰাই MIN (k, n_সাৰিগুলি);
}

স্থির পূর্ণাঙ্ক
write_permuted_output_reservoir (size_t n_সাৰিগুলি, সংরচনা linebuffer *সাৰিগুলি,
                                 size_t ধ্রুৱ *permutation)
{
  বাবে (size_t i = 0; i < n_সাৰিগুলি; i++)
    {
      ধ্রুৱ সংরচনা linebuffer *p = &সাৰিগুলি[permutation[i]];
      যদি (fwrite (p->buffer, পরিমাণ (ৱর্ণ), p->length, stdout) != p->length)
        ঘূৰাই -1;
    }

  ঘূৰাই 0;
}

/* Read data from file IN.  Input lines are delimited by EOLBYTE;
   silently append a trailing EOLBYTE if the file ends in some other
   byte.  Store a pointer to the resulting array of lines into *PLINE.
   Return the number of lines read.  Report an error and exit on
   failure.  */

স্থির size_t
read_প্ৰবেশ (FILE *in, ৱর্ণ eolbyte, ৱর্ণ ***pline)
{
  ৱর্ণ *p;
  ৱর্ণ *buf = রিক্ত;
  size_t used;
  ৱর্ণ *lim;
  ৱর্ণ **সাৰি;
  size_t n_সাৰিগুলি;

  /* TODO: We should limit the amount of data read here,
     to less than RESERVOIR_MIN_INPUT.  I.e., adjust fread_file() to support
     taking a byte limit.  We'd then need to ensure we handle a line spanning
     this boundary.  With that in place we could set use_reservoir_sampling
     when used==RESERVOIR_MIN_INPUT, and have read_input_reservoir_sampling()
     call a wrapper function to populate a linebuffer from the internal pline
     or if none left, stdin.  Doing that would give better performance by
     avoiding the reservoir CPU overhead when reading < RESERVOIR_MIN_INPUT
     from a pipe, and allow us to dispense with the input_size() function.  */
  যদি (!(buf = fread_file (in, 0, &used)))
    error (EXIT_FAILURE, errno, _("read error"));

  যদি (used && buf[used - 1] != eolbyte)
    buf[used++] = eolbyte;

  lim = buf + used;

  n_সাৰিগুলি = 0;
  বাবে (p = buf; p < lim; p = next_সাৰি (p, eolbyte))
    n_সাৰিগুলি++;

  *pline = সাৰি = xnmalloc (n_সাৰিগুলি + 1, পরিমাণ *সাৰি);

  সাৰি[0] = p = buf;
  বাবে (size_t i = 1; i <= n_সাৰিগুলি; i++)
    সাৰি[i] = p = next_সাৰি (p, eolbyte);

  ঘূৰাই n_সাৰিগুলি;
}

/* Output N_LINES lines to stdout from LINE array,
   chosen by the indices in PERMUTATION.
   PERMUTATION and LINE must have at least N_LINES elements.
   Strings in LINE must include the line-terminator character.  */
স্থির পূর্ণাঙ্ক
write_permuted_সাৰিগুলি (size_t n_সাৰিগুলি, ৱর্ণ *ধ্রুৱ *সাৰি,
                      size_t ধ্রুৱ *permutation)
{
  বাবে (size_t i = 0; i < n_সাৰিগুলি; i++)
    {
      ৱর্ণ *ধ্রুৱ *p = সাৰি + permutation[i];
      size_t len = p[1] - p[0];
      যদি (fwrite (p[0], পরিমাণ *p[0], len, stdout) != len)
        ঘূৰাই -1;
    }

  ঘূৰাই 0;
}

/* Print NUMBER followed by EOLBYTE to standard output.
   Return false on failure, true on success.  */
স্থির bool
print_সংখ্যা (unsigned long পূর্ণাঙ্ক সংখ্যা, ৱর্ণ eolbyte)
{
  ৱর্ণ buf[INT_BUFSIZE_BOUND (unsigned long পূর্ণাঙ্ক)];
  ৱর্ণ *p = buf + INT_STRLEN_BOUND (unsigned long পূর্ণাঙ্ক);
  *p = eolbyte;
  কৰা
    *--p = '0' + সংখ্যা % 10;
  যেতিয়ালৈ ((সংখ্যা /= 10) != 0);
  idx_t len = buf + পরিমাণ buf - p;
  ঘূৰাই fwrite (p, 1, len, stdout) == len;
}

/* Output N_LINES of numbers to stdout, from PERMUTATION array.
   PERMUTATION must have at least N_LINES elements.  */
স্থির পূর্ণাঙ্ক
write_permuted_numbers (size_t n_সাৰিগুলি, size_t lo_প্ৰবেশ,
                        size_t ধ্রুৱ *permutation, ৱর্ণ eolbyte)
{
  বাবে (size_t i = 0; i < n_সাৰিগুলি; i++)
    {
      unsigned long পূর্ণাঙ্ক n = lo_প্ৰবেশ + permutation[i];
      যদি (! print_সংখ্যা (n, eolbyte))
        ঘূৰাই -1;
    }

  ঘূৰাই 0;
}

/* Output COUNT numbers to stdout, chosen randomly from range
   LO_INPUT through HI_INPUT.  */
স্থির পূর্ণাঙ্ক
write_random_numbers (সংরচনা randint_source *s, size_t গুনতি,
                      size_t lo_প্ৰবেশ, size_t hi_প্ৰবেশ, ৱর্ণ eolbyte)
{
  ধ্রুৱ randint range = hi_প্ৰবেশ - lo_প্ৰবেশ + 1;

  বাবে (size_t i = 0; i < গুনতি; i++)
    {
      unsigned long পূর্ণাঙ্ক j = lo_প্ৰবেশ + randint_choose (s, range);
      যদি (! print_সংখ্যা (j, eolbyte))
        ঘূৰাই -1;
    }

  ঘূৰাই 0;
}

/* Output COUNT lines to stdout from LINES array.
   LINES must have at least N_LINES elements in it.
   Strings in LINES_ must include the line-terminator character.  */
স্থির পূর্ণাঙ্ক
write_random_সাৰিগুলি (সংরচনা randint_source *s, size_t গুনতি,
                    ৱর্ণ *ধ্রুৱ *সাৰিগুলি, size_t n_সাৰিগুলি)
{
  বাবে (size_t i = 0; i < গুনতি; i++)
    {
      ধ্রুৱ randint j = randint_choose (s, n_সাৰিগুলি);
      ৱর্ণ *ধ্রুৱ *p = সাৰিগুলি + j;
      size_t len = p[1] - p[0];
      যদি (fwrite (p[0], পরিমাণ *p[0], len, stdout) != len)
        ঘূৰাই -1;
    }

  ঘূৰাই 0;
}

পূর্ণাঙ্ক
main (পূর্ণাঙ্ক argc, ৱর্ণ **argv)
{
  bool echo = false;
  bool প্ৰবেশ_range = false;
  size_t lo_প্ৰবেশ = SIZE_MAX;
  size_t hi_প্ৰবেশ = 0;
  idx_t head_সাৰিগুলি = MIN (IDX_MAX, SIZE_MAX);
  ৱর্ণ ধ্রুৱ *outfile = রিক্ত;
  ৱর্ণ *random_source = রিক্ত;
  ৱর্ণ eolbyte = '\n';
  ৱর্ণ **প্ৰবেশ_সাৰিগুলি = রিক্ত;
  bool use_reservoir_sampling = false;
  bool repeat = false;

  পূর্ণাঙ্ক optc;
  পূর্ণাঙ্ক n_operands;
  ৱর্ণ **operand;
  size_t n_সাৰিগুলি;
  ৱর্ণ **সাৰি = রিক্ত;
  সংরচনা linebuffer *reservoir = রিক্ত;
  সংরচনা randint_source *randint_source;
  size_t *permutation = রিক্ত;
  পূর্ণাঙ্ক i;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  যেতিয়ালৈ ((optc = getopt_long (argc, argv, "ei:n:o:rz", long_opts, রিক্ত))
         != -1)
    ৱিকল্প (optc)
      {
      পক্ষ 'e':
        echo = true;
        ভঙ্গ;

      পক্ষ 'i':
        {
          যদি (প্ৰবেশ_range)
            error (EXIT_FAILURE, 0, _("multiple -i options specified"));
          প্ৰবেশ_range = true;

          uintmax_t u;
          ৱর্ণ *lo_end;
          strtol_error err = xstrtoumax (optarg, &lo_end, 10, &u, রিক্ত);
          যদি (err == LONGINT_OK)
            {
              lo_প্ৰবেশ = u;
              যদি (lo_প্ৰবেশ != u)
                err = LONGINT_OVERFLOW;
              অন্যথা যদি (*lo_end != '-')
                err = LONGINT_INVALID;
              অন্যথা
                {
                  err = xstrtoumax (lo_end + 1, রিক্ত, 10, &u, "");
                  যদি (err == LONGINT_OK)
                    {
                      hi_প্ৰবেশ = u;
                      যদি (hi_প্ৰবেশ != u)
                        err = LONGINT_OVERFLOW;
                    }
                }
            }

          n_সাৰিগুলি = hi_প্ৰবেশ - lo_প্ৰবেশ + 1;

          যদি (err != LONGINT_OK || (lo_প্ৰবেশ <= hi_প্ৰবেশ) == (n_সাৰিগুলি == 0))
            error (EXIT_FAILURE, err == LONGINT_OVERFLOW ? EOVERFLOW : 0,
                   "%s: %s", _("invalid input range"), quote (optarg));
        }
        ভঙ্গ;

      পক্ষ 'n':
        {
          uintmax_t argval;
          strtol_error e = xstrtoumax (optarg, রিক্ত, 10, &argval, "");

          যদি (e == LONGINT_OK)
            head_সাৰিগুলি = MIN (head_সাৰিগুলি, argval);
          অন্যথা যদি (e != LONGINT_OVERFLOW)
            error (EXIT_FAILURE, 0, _("invalid line count: %s"),
                   quote (optarg));
        }
        ভঙ্গ;

      পক্ষ 'o':
        যদি (outfile && !streq (outfile, optarg))
          error (EXIT_FAILURE, 0, _("multiple output files specified"));
        outfile = optarg;
        ভঙ্গ;

      পক্ষ RANDOM_SOURCE_OPTION:
        যদি (random_source && !streq (random_source, optarg))
          error (EXIT_FAILURE, 0, _("multiple random sources specified"));
        random_source = optarg;
        ভঙ্গ;

      পক্ষ 'r':
        repeat = true;
        ভঙ্গ;

      পক্ষ 'z':
        eolbyte = '\0';
        ভঙ্গ;

      case_GETOPT_HELP_CHAR;
      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
      স্ৱভাৱ:
        usage (EXIT_FAILURE);
      }

  n_operands = argc - optind;
  operand = argv + optind;

  /* Check invalid usage.  */
  যদি (echo && প্ৰবেশ_range)
    {
      error (0, 0, _("cannot combine -e and -i options"));
      usage (EXIT_FAILURE);
    }
  যদি (প্ৰবেশ_range ? 0 < n_operands : !echo && 1 < n_operands)
    {
      error (0, 0, _("extra operand %s"), quote (operand[!প্ৰবেশ_range]));
      usage (EXIT_FAILURE);
    }

  /* Prepare input.  */
  যদি (head_সাৰিগুলি == 0)
    {
      n_সাৰিগুলি = 0;
      সাৰি = রিক্ত;
    }
  অন্যথা যদি (echo)
    {
      প্ৰবেশ_from_argv (operand, n_operands, eolbyte);
      n_সাৰিগুলি = n_operands;
      সাৰি = operand;
    }
  অন্যথা যদি (প্ৰবেশ_range)
    {
      IF_LINT (n_সাৰিগুলি = hi_প্ৰবেশ - lo_প্ৰবেশ + 1); /* Avoid GCC 10 warning.  */
      সাৰি = রিক্ত;
    }
  অন্যথা
    {
      /* If an input file is specified, re-open it as stdin.  */
      যদি (n_operands == 1
          && ! (streq (operand[0], "-")
                || freopen (operand[0], "r", stdin)))
        error (EXIT_FAILURE, errno, "%s", quotef (operand[0]));

      fadvise (stdin, FADVISE_SEQUENTIAL);

      যদি (repeat || head_সাৰিগুলি == MIN (IDX_MAX, SIZE_MAX)
          || প্ৰবেশ_size () <= RESERVOIR_MIN_প্ৰবেশ)
        {
          n_সাৰিগুলি = read_প্ৰবেশ (stdin, eolbyte, &প্ৰবেশ_সাৰিগুলি);
          সাৰি = প্ৰবেশ_সাৰিগুলি;
        }
      অন্যথা
        {
          use_reservoir_sampling = true;
          n_সাৰিগুলি = SIZE_MAX;   /* unknown number of input lines, for now.  */
        }
    }

  /* The adjusted head line count; can be less than HEAD_LINES if the
     input is small and if not repeating.  */
  idx_t ahead_সাৰিগুলি = repeat || head_সাৰিগুলি < n_সাৰিগুলি ? head_সাৰিগুলি : n_সাৰিগুলি;

  randint_source = randint_all_new (random_source,
                                    (use_reservoir_sampling || repeat
                                     ? SIZE_MAX
                                     : randperm_bound (ahead_সাৰিগুলি, n_সাৰিগুলি)));
  যদি (! randint_source)
    error (EXIT_FAILURE, errno, "%s",
           quotef (random_source ? random_source : "getrandom"));

  যদি (use_reservoir_sampling)
    {
      /* Instead of reading the entire file into 'line',
         use reservoir-sampling to store just AHEAD_LINES random lines.  */
      n_সাৰিগুলি = read_প্ৰবেশ_reservoir_sampling (stdin, eolbyte, ahead_সাৰিগুলি,
                                               randint_source, &reservoir);
      ahead_সাৰিগুলি = n_সাৰিগুলি;
    }

  /* Close stdin now, rather than earlier, so that randint_all_new
     doesn't have to worry about opening something other than
     stdin.  */
  যদি (! (head_সাৰিগুলি == 0 || echo || প্ৰবেশ_range || fclose (stdin) == 0))
    error (EXIT_FAILURE, errno, _("read error"));

  যদি (!repeat)
    permutation = randperm_new (randint_source, ahead_সাৰিগুলি, n_সাৰিগুলি);

  যদি (outfile && ! freopen (outfile, "w", stdout))
    error (EXIT_FAILURE, errno, "%s", quotef (outfile));

  /* Generate output according to requested method */
  যদি (repeat)
    {
      যদি (head_সাৰিগুলি == 0)
        i = 0;
      অন্যথা
        {
          যদি (n_সাৰিগুলি == 0)
            error (EXIT_FAILURE, 0, _("no lines to repeat"));
          যদি (প্ৰবেশ_range)
            i = write_random_numbers (randint_source, ahead_সাৰিগুলি,
                                      lo_প্ৰবেশ, hi_প্ৰবেশ, eolbyte);
          অন্যথা
            i = write_random_সাৰিগুলি (randint_source, ahead_সাৰিগুলি, সাৰি, n_সাৰিগুলি);
        }
    }
  অন্যথা
    {
      যদি (use_reservoir_sampling)
        i = write_permuted_output_reservoir (n_সাৰিগুলি, reservoir, permutation);
      অন্যথা যদি (প্ৰবেশ_range)
        i = write_permuted_numbers (ahead_সাৰিগুলি, lo_প্ৰবেশ,
                                    permutation, eolbyte);
      অন্যথা
        i = write_permuted_সাৰিগুলি (ahead_সাৰিগুলি, সাৰি, permutation);
    }

  যদি (i != 0)
    write_error ();

  IF_LINT (randint_all_free (randint_source));  /* For older valgrind.  */

  main_exit (EXIT_SUCCESS);
}
