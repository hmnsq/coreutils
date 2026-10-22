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

#അന്തര്ഭാവ <config.h>

#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <getopt.h>
#അന്തര്ഭാവ "system.h"

#അന്തര്ഭാവ "fadvise.h"
#അന്തര്ഭാവ "linebuffer.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "randint.h"
#അന്തര്ഭാവ "randperm.h"
#അന്തര്ഭാവ "read-file.h"
#അന്തര്ഭാവ "stdio--.h"
#അന്തര്ഭാവ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "shuf"

#define AUTHORS proper_name ("Paul Eggert")

/* reservoir-sampling introduces CPU overhead for small inputs.
   So only enable it for inputs >= this limit.
   This limit was determined using these commands:
     $ for p in $(seq 7); do src/seq $((10**$p)) > 10p$p.in; done
     $ for p in $(seq 7); do time shuf-nores -n10 10p$p.in >/dev/null; done
     $ for p in $(seq 7); do time shuf -n10 10p$p.in >/dev/null; done  .*/
എണ്ണൽ { RESERVOIR_MIN_നിവേശം = 8192 * 1024 };


ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]\n\
  or:  %s -e [OPTION]... [ARG]...\n\
  or:  %s -i LO-HI [OPTION]...\n\
"),
              program_name, program_name, program_name);
      fputs (_("\
Write a random permutation of the നിവേശം വരികൾ to standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -e, --echo                treat each ARG as an നിവേശം വരി\n\
"));
      oputs (_("\
  -i, --നിവേശം-range=LO-HI   treat each സംഖ്യ LO through HI as an നിവേശം വരി\n\
"));
      oputs (_("\
  -n, --head-എണ്ണം=എണ്ണം_2    output at most എണ്ണം_2 വരികൾ\n\
"));
      oputs (_("\
  -o, --output=FILE         ലിഖ result to FILE instead of standard output\n\
"));
      oputs (_("\
      --random-source=FILE  get random bytes from FILE\n\
"));
      oputs (_("\
  -r, --repeat              output വരികൾ can be repeated\n\
"));
      oputs (_("\
  -z, --zero-terminated     വരി delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }

  exit (status);
}

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
എണ്ണൽ
{
  RANDOM_SOURCE_OPTION = CHAR_MAX + 1
};

സ്ഥിതം ഘടന option സ്ഥിരം long_opts[] =
{
  {"echo", no_argument, ശൂന്യ, 'e'},
  {"input-range", required_argument, ശൂന്യ, 'i'},
  {"head-count", required_argument, ശൂന്യ, 'n'},
  {"output", required_argument, ശൂന്യ, 'o'},
  {"random-source", required_argument, ശൂന്യ, RANDOM_SOURCE_OPTION},
  {"repeat", no_argument, ശൂന്യ, 'r'},
  {"zero-terminated", no_argument, ശൂന്യ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ശൂന്യ, 0, ശൂന്യ, 0},
};

സ്ഥിതം ശൂന്യം
നിവേശം_from_argv (അക്ഷരം **operand, പൂർണ്ണസംഖ്യ n_operands, അക്ഷരം eolbyte)
{
  അക്ഷരം *p;
  size_t size = n_operands;
  പൂർണ്ണസംഖ്യ i;

  വേണ്ടി (i = 0; i < n_operands; i++)
    size += strlen (operand[i]);
  p = xmalloc (size);

  വേണ്ടി (i = 0; i < n_operands; i++)
    {
      അക്ഷരം *p1 = stpcpy (p, operand[i]);
      operand[i] = p;
      p = p1;
      *p++ = eolbyte;
    }

  operand[n_operands] = p;
}

/* Return the start of the next line after LINE, which is guaranteed
   to end in EOLBYTE.  */

സ്ഥിതം അക്ഷരം *
next_വരി (അക്ഷരം *വരി, അക്ഷരം eolbyte)
{
  അക്ഷരം *p = rawmemchr (വരി, eolbyte);
  തിരികെ p + 1;
}

/* Return the size of the input if possible or OFF_T_MAX if not.  */

സ്ഥിതം off_t
നിവേശം_size (ശൂന്യം)
{
  off_t file_size;

  ഘടന stat stat_buf;
  എങ്കിൽ (fstat (STDIN_FILENO, &stat_buf) != 0)
    തിരികെ OFF_T_MAX;
  എങ്കിൽ (usable_st_size (&stat_buf))
    file_size = stat_buf.st_size;
  അല്ലെങ്കിൽ
    തിരികെ OFF_T_MAX;

  off_t നിവേശം_offset = lseek (STDIN_FILENO, 0, SEEK_CUR);
  എങ്കിൽ (നിവേശം_offset < 0)
    തിരികെ OFF_T_MAX;

  file_size -= നിവേശം_offset;

  തിരികെ file_size;
}

/* Read all lines and store up to K permuted lines in *OUT_RSRV.
   Return the number of lines read, up to a maximum of K.  */

സ്ഥിതം idx_t
read_നിവേശം_reservoir_sampling (FILE *in, അക്ഷരം eolbyte, idx_t k,
                               ഘടന randint_source *s,
                               ഘടന linebuffer **out_rsrv)
{
  randint n_വരികൾ = 0;
  idx_t n_alloc_വരികൾ = 0;
  ഘടന linebuffer *വരി = ശൂന്യ;
  ഘടന linebuffer *rsrv = ശൂന്യ;

  /* Fill the first K lines, directly into the reservoir.  */
  വേണ്ടി (n_വരികൾ = 0; n_വരികൾ < k; n_വരികൾ++)
    {
      /* Enlarge reservoir if needed.  */
      എങ്കിൽ (n_വരികൾ == n_alloc_വരികൾ)
        {
          idx_t old = n_alloc_വരികൾ;
          rsrv = xpalloc (rsrv, &n_alloc_വരികൾ, 1, k, വലുപ്പം *rsrv);
          memset (&rsrv[n_വരികൾ], 0, (n_alloc_വരികൾ - old) * വലുപ്പം *rsrv);
        }

      വരി = readlinebuffer_delim (&rsrv[n_വരികൾ], in, eolbyte);
      എങ്കിൽ (!വരി)
        നിർത്തുക;
    }

  /* last line wasn't null - so there may be more lines to read.  */
  എങ്കിൽ (വരി != ശൂന്യ)
    {
      ഘടന linebuffer dummy;
      initbuffer (&dummy);  /* space for lines not put in reservoir.  */

      /* Choose the fate of the next line, with decreasing probability (as
         n_lines increases in size).

         If the line will be used, store it directly in the reservoir.
         Otherwise, store it in dummy space.

         With 'struct linebuffer', storing into existing buffer will reduce
         re-allocations (will only re-allocate if the new line is longer than
         the currently allocated space).  */
      ചെയ്യുക
        {
          randint j = randint_choose (s, n_വരികൾ + 1);  /* 0 .. n_lines.  */
          വരി = (j < k) ? (&rsrv[j]) : (&dummy);
        }
      ആകുമ്പോൾ (readlinebuffer_delim (വരി, in, eolbyte) != ശൂന്യ && n_വരികൾ++);

      എങ്കിൽ (! n_വരികൾ)
        error (EXIT_FAILURE, EOVERFLOW, _("too many input lines"));

      freebuffer (&dummy);
    }

  /* no more input lines, or an input error.  */
  എങ്കിൽ (ferror (in))
    error (EXIT_FAILURE, errno, _("read error"));

  *out_rsrv = rsrv;
  തിരികെ MIN (k, n_വരികൾ);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
write_permuted_output_reservoir (size_t n_വരികൾ, ഘടന linebuffer *വരികൾ,
                                 size_t സ്ഥിരം *permutation)
{
  വേണ്ടി (size_t i = 0; i < n_വരികൾ; i++)
    {
      സ്ഥിരം ഘടന linebuffer *p = &വരികൾ[permutation[i]];
      എങ്കിൽ (fwrite (p->buffer, വലുപ്പം (അക്ഷരം), p->length, stdout) != p->length)
        തിരികെ -1;
    }

  തിരികെ 0;
}

/* Read data from file IN.  Input lines are delimited by EOLBYTE;
   silently append a trailing EOLBYTE if the file ends in some other
   byte.  Store a pointer to the resulting array of lines into *PLINE.
   Return the number of lines read.  Report an error and exit on
   failure.  */

സ്ഥിതം size_t
read_നിവേശം (FILE *in, അക്ഷരം eolbyte, അക്ഷരം ***pline)
{
  അക്ഷരം *p;
  അക്ഷരം *buf = ശൂന്യ;
  size_t used;
  അക്ഷരം *lim;
  അക്ഷരം **വരി;
  size_t n_വരികൾ;

  /* TODO: We should limit the amount of data read here,
     to less than RESERVOIR_MIN_INPUT.  I.e., adjust fread_file() to support
     taking a byte limit.  We'd then need to ensure we handle a line spanning
     this boundary.  With that in place we could set use_reservoir_sampling
     when used==RESERVOIR_MIN_INPUT, and have read_input_reservoir_sampling()
     call a wrapper function to populate a linebuffer from the internal pline
     or if none left, stdin.  Doing that would give better performance by
     avoiding the reservoir CPU overhead when reading < RESERVOIR_MIN_INPUT
     from a pipe, and allow us to dispense with the input_size() function.  */
  എങ്കിൽ (!(buf = fread_file (in, 0, &used)))
    error (EXIT_FAILURE, errno, _("read error"));

  എങ്കിൽ (used && buf[used - 1] != eolbyte)
    buf[used++] = eolbyte;

  lim = buf + used;

  n_വരികൾ = 0;
  വേണ്ടി (p = buf; p < lim; p = next_വരി (p, eolbyte))
    n_വരികൾ++;

  *pline = വരി = xnmalloc (n_വരികൾ + 1, വലുപ്പം *വരി);

  വരി[0] = p = buf;
  വേണ്ടി (size_t i = 1; i <= n_വരികൾ; i++)
    വരി[i] = p = next_വരി (p, eolbyte);

  തിരികെ n_വരികൾ;
}

/* Output N_LINES lines to stdout from LINE array,
   chosen by the indices in PERMUTATION.
   PERMUTATION and LINE must have at least N_LINES elements.
   Strings in LINE must include the line-terminator character.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
write_permuted_വരികൾ (size_t n_വരികൾ, അക്ഷരം *സ്ഥിരം *വരി,
                      size_t സ്ഥിരം *permutation)
{
  വേണ്ടി (size_t i = 0; i < n_വരികൾ; i++)
    {
      അക്ഷരം *സ്ഥിരം *p = വരി + permutation[i];
      size_t len = p[1] - p[0];
      എങ്കിൽ (fwrite (p[0], വലുപ്പം *p[0], len, stdout) != len)
        തിരികെ -1;
    }

  തിരികെ 0;
}

/* Print NUMBER followed by EOLBYTE to standard output.
   Return false on failure, true on success.  */
സ്ഥിതം bool
print_സംഖ്യ (unsigned long പൂർണ്ണസംഖ്യ സംഖ്യ, അക്ഷരം eolbyte)
{
  അക്ഷരം buf[INT_BUFSIZE_BOUND (unsigned long പൂർണ്ണസംഖ്യ)];
  അക്ഷരം *p = buf + INT_STRLEN_BOUND (unsigned long പൂർണ്ണസംഖ്യ);
  *p = eolbyte;
  ചെയ്യുക
    *--p = '0' + സംഖ്യ % 10;
  ആകുമ്പോൾ ((സംഖ്യ /= 10) != 0);
  idx_t len = buf + വലുപ്പം buf - p;
  തിരികെ fwrite (p, 1, len, stdout) == len;
}

/* Output N_LINES of numbers to stdout, from PERMUTATION array.
   PERMUTATION must have at least N_LINES elements.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
write_permuted_numbers (size_t n_വരികൾ, size_t lo_നിവേശം,
                        size_t സ്ഥിരം *permutation, അക്ഷരം eolbyte)
{
  വേണ്ടി (size_t i = 0; i < n_വരികൾ; i++)
    {
      unsigned long പൂർണ്ണസംഖ്യ n = lo_നിവേശം + permutation[i];
      എങ്കിൽ (! print_സംഖ്യ (n, eolbyte))
        തിരികെ -1;
    }

  തിരികെ 0;
}

/* Output COUNT numbers to stdout, chosen randomly from range
   LO_INPUT through HI_INPUT.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
write_random_numbers (ഘടന randint_source *s, size_t എണ്ണം,
                      size_t lo_നിവേശം, size_t hi_നിവേശം, അക്ഷരം eolbyte)
{
  സ്ഥിരം randint range = hi_നിവേശം - lo_നിവേശം + 1;

  വേണ്ടി (size_t i = 0; i < എണ്ണം; i++)
    {
      unsigned long പൂർണ്ണസംഖ്യ j = lo_നിവേശം + randint_choose (s, range);
      എങ്കിൽ (! print_സംഖ്യ (j, eolbyte))
        തിരികെ -1;
    }

  തിരികെ 0;
}

/* Output COUNT lines to stdout from LINES array.
   LINES must have at least N_LINES elements in it.
   Strings in LINES_ must include the line-terminator character.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
write_random_വരികൾ (ഘടന randint_source *s, size_t എണ്ണം,
                    അക്ഷരം *സ്ഥിരം *വരികൾ, size_t n_വരികൾ)
{
  വേണ്ടി (size_t i = 0; i < എണ്ണം; i++)
    {
      സ്ഥിരം randint j = randint_choose (s, n_വരികൾ);
      അക്ഷരം *സ്ഥിരം *p = വരികൾ + j;
      size_t len = p[1] - p[0];
      എങ്കിൽ (fwrite (p[0], വലുപ്പം *p[0], len, stdout) != len)
        തിരികെ -1;
    }

  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  bool echo = false;
  bool നിവേശം_range = false;
  size_t lo_നിവേശം = SIZE_MAX;
  size_t hi_നിവേശം = 0;
  idx_t head_വരികൾ = MIN (IDX_MAX, SIZE_MAX);
  അക്ഷരം സ്ഥിരം *outfile = ശൂന്യ;
  അക്ഷരം *random_source = ശൂന്യ;
  അക്ഷരം eolbyte = '\n';
  അക്ഷരം **നിവേശം_വരികൾ = ശൂന്യ;
  bool use_reservoir_sampling = false;
  bool repeat = false;

  പൂർണ്ണസംഖ്യ optc;
  പൂർണ്ണസംഖ്യ n_operands;
  അക്ഷരം **operand;
  size_t n_വരികൾ;
  അക്ഷരം **വരി = ശൂന്യ;
  ഘടന linebuffer *reservoir = ശൂന്യ;
  ഘടന randint_source *randint_source;
  size_t *permutation = ശൂന്യ;
  പൂർണ്ണസംഖ്യ i;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ആകുമ്പോൾ ((optc = getopt_long (argc, argv, "ei:n:o:rz", long_opts, ശൂന്യ))
         != -1)
    തിരഞ്ഞെടുപ്പ് (optc)
      {
      അവസ്ഥ 'e':
        echo = true;
        നിർത്തുക;

      അവസ്ഥ 'i':
        {
          എങ്കിൽ (നിവേശം_range)
            error (EXIT_FAILURE, 0, _("multiple -i options specified"));
          നിവേശം_range = true;

          uintmax_t u;
          അക്ഷരം *lo_end;
          strtol_error err = xstrtoumax (optarg, &lo_end, 10, &u, ശൂന്യ);
          എങ്കിൽ (err == LONGINT_OK)
            {
              lo_നിവേശം = u;
              എങ്കിൽ (lo_നിവേശം != u)
                err = LONGINT_OVERFLOW;
              അല്ലെങ്കിൽ എങ്കിൽ (*lo_end != '-')
                err = LONGINT_INVALID;
              അല്ലെങ്കിൽ
                {
                  err = xstrtoumax (lo_end + 1, ശൂന്യ, 10, &u, "");
                  എങ്കിൽ (err == LONGINT_OK)
                    {
                      hi_നിവേശം = u;
                      എങ്കിൽ (hi_നിവേശം != u)
                        err = LONGINT_OVERFLOW;
                    }
                }
            }

          n_വരികൾ = hi_നിവേശം - lo_നിവേശം + 1;

          എങ്കിൽ (err != LONGINT_OK || (lo_നിവേശം <= hi_നിവേശം) == (n_വരികൾ == 0))
            error (EXIT_FAILURE, err == LONGINT_OVERFLOW ? EOVERFLOW : 0,
                   "%s: %s", _("invalid input range"), quote (optarg));
        }
        നിർത്തുക;

      അവസ്ഥ 'n':
        {
          uintmax_t argval;
          strtol_error e = xstrtoumax (optarg, ശൂന്യ, 10, &argval, "");

          എങ്കിൽ (e == LONGINT_OK)
            head_വരികൾ = MIN (head_വരികൾ, argval);
          അല്ലെങ്കിൽ എങ്കിൽ (e != LONGINT_OVERFLOW)
            error (EXIT_FAILURE, 0, _("invalid line count: %s"),
                   quote (optarg));
        }
        നിർത്തുക;

      അവസ്ഥ 'o':
        എങ്കിൽ (outfile && !streq (outfile, optarg))
          error (EXIT_FAILURE, 0, _("multiple output files specified"));
        outfile = optarg;
        നിർത്തുക;

      അവസ്ഥ RANDOM_SOURCE_OPTION:
        എങ്കിൽ (random_source && !streq (random_source, optarg))
          error (EXIT_FAILURE, 0, _("multiple random sources specified"));
        random_source = optarg;
        നിർത്തുക;

      അവസ്ഥ 'r':
        repeat = true;
        നിർത്തുക;

      അവസ്ഥ 'z':
        eolbyte = '\0';
        നിർത്തുക;

      case_GETOPT_HELP_CHAR;
      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
      സ്ഥിരസ്ഥിതി:
        usage (EXIT_FAILURE);
      }

  n_operands = argc - optind;
  operand = argv + optind;

  /* Check invalid usage.  */
  എങ്കിൽ (echo && നിവേശം_range)
    {
      error (0, 0, _("cannot combine -e and -i options"));
      usage (EXIT_FAILURE);
    }
  എങ്കിൽ (നിവേശം_range ? 0 < n_operands : !echo && 1 < n_operands)
    {
      error (0, 0, _("extra operand %s"), quote (operand[!നിവേശം_range]));
      usage (EXIT_FAILURE);
    }

  /* Prepare input.  */
  എങ്കിൽ (head_വരികൾ == 0)
    {
      n_വരികൾ = 0;
      വരി = ശൂന്യ;
    }
  അല്ലെങ്കിൽ എങ്കിൽ (echo)
    {
      നിവേശം_from_argv (operand, n_operands, eolbyte);
      n_വരികൾ = n_operands;
      വരി = operand;
    }
  അല്ലെങ്കിൽ എങ്കിൽ (നിവേശം_range)
    {
      IF_LINT (n_വരികൾ = hi_നിവേശം - lo_നിവേശം + 1); /* Avoid GCC 10 warning.  */
      വരി = ശൂന്യ;
    }
  അല്ലെങ്കിൽ
    {
      /* If an input file is specified, re-open it as stdin.  */
      എങ്കിൽ (n_operands == 1
          && ! (streq (operand[0], "-")
                || freopen (operand[0], "r", stdin)))
        error (EXIT_FAILURE, errno, "%s", quotef (operand[0]));

      fadvise (stdin, FADVISE_SEQUENTIAL);

      എങ്കിൽ (repeat || head_വരികൾ == MIN (IDX_MAX, SIZE_MAX)
          || നിവേശം_size () <= RESERVOIR_MIN_നിവേശം)
        {
          n_വരികൾ = read_നിവേശം (stdin, eolbyte, &നിവേശം_വരികൾ);
          വരി = നിവേശം_വരികൾ;
        }
      അല്ലെങ്കിൽ
        {
          use_reservoir_sampling = true;
          n_വരികൾ = SIZE_MAX;   /* unknown number of input lines, for now.  */
        }
    }

  /* The adjusted head line count; can be less than HEAD_LINES if the
     input is small and if not repeating.  */
  idx_t ahead_വരികൾ = repeat || head_വരികൾ < n_വരികൾ ? head_വരികൾ : n_വരികൾ;

  randint_source = randint_all_new (random_source,
                                    (use_reservoir_sampling || repeat
                                     ? SIZE_MAX
                                     : randperm_bound (ahead_വരികൾ, n_വരികൾ)));
  എങ്കിൽ (! randint_source)
    error (EXIT_FAILURE, errno, "%s",
           quotef (random_source ? random_source : "getrandom"));

  എങ്കിൽ (use_reservoir_sampling)
    {
      /* Instead of reading the entire file into 'line',
         use reservoir-sampling to store just AHEAD_LINES random lines.  */
      n_വരികൾ = read_നിവേശം_reservoir_sampling (stdin, eolbyte, ahead_വരികൾ,
                                               randint_source, &reservoir);
      ahead_വരികൾ = n_വരികൾ;
    }

  /* Close stdin now, rather than earlier, so that randint_all_new
     doesn't have to worry about opening something other than
     stdin.  */
  എങ്കിൽ (! (head_വരികൾ == 0 || echo || നിവേശം_range || fclose (stdin) == 0))
    error (EXIT_FAILURE, errno, _("read error"));

  എങ്കിൽ (!repeat)
    permutation = randperm_new (randint_source, ahead_വരികൾ, n_വരികൾ);

  എങ്കിൽ (outfile && ! freopen (outfile, "w", stdout))
    error (EXIT_FAILURE, errno, "%s", quotef (outfile));

  /* Generate output according to requested method */
  എങ്കിൽ (repeat)
    {
      എങ്കിൽ (head_വരികൾ == 0)
        i = 0;
      അല്ലെങ്കിൽ
        {
          എങ്കിൽ (n_വരികൾ == 0)
            error (EXIT_FAILURE, 0, _("no lines to repeat"));
          എങ്കിൽ (നിവേശം_range)
            i = write_random_numbers (randint_source, ahead_വരികൾ,
                                      lo_നിവേശം, hi_നിവേശം, eolbyte);
          അല്ലെങ്കിൽ
            i = write_random_വരികൾ (randint_source, ahead_വരികൾ, വരി, n_വരികൾ);
        }
    }
  അല്ലെങ്കിൽ
    {
      എങ്കിൽ (use_reservoir_sampling)
        i = write_permuted_output_reservoir (n_വരികൾ, reservoir, permutation);
      അല്ലെങ്കിൽ എങ്കിൽ (നിവേശം_range)
        i = write_permuted_numbers (ahead_വരികൾ, lo_നിവേശം,
                                    permutation, eolbyte);
      അല്ലെങ്കിൽ
        i = write_permuted_വരികൾ (ahead_വരികൾ, വരി, permutation);
    }

  എങ്കിൽ (i != 0)
    write_error ();

  IF_LINT (randint_all_free (randint_source));  /* For older valgrind.  */

  main_exit (EXIT_SUCCESS);
}
