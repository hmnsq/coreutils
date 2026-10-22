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

#अन्तर्भाव <config.h>

#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <getopt.h>
#अन्तर्भाव "system.h"

#अन्तर्भाव "fadvise.h"
#अन्तर्भाव "linebuffer.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "randint.h"
#अन्तर्भाव "randperm.h"
#अन्तर्भाव "read-file.h"
#अन्तर्भाव "stdio--.h"
#अन्तर्भाव "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "shuf"

#define AUTHORS proper_name ("Paul Eggert")

/* reservoir-sampling introduces CPU overhead for small inputs.
   So only enable it for inputs >= this limit.
   This limit was determined using these commands:
     $ for p in $(seq 7); do src/seq $((10**$p)) > 10p$p.in; done
     $ for p in $(seq 7); do time shuf-nores -n10 10p$p.in >/dev/null; done
     $ for p in $(seq 7); do time shuf -n10 10p$p.in >/dev/null; done  .*/
गणना { RESERVOIR_MIN_निवेशः = 8192 * 1024 };


शून्य
usage (पूर्णाङ्क status)
{
  सचे (status != EXIT_SUCCESS)
    emit_try_help ();
  अञ्ञथा
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]\n\
  or:  %s -e [OPTION]... [ARG]...\n\
  or:  %s -i LO-HI [OPTION]...\n\
"),
              program_name, program_name, program_name);
      fputs (_("\
Write a random permutation of the निवेशः पङ्क्तयः to standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -e, --echo                treat each ARG as an निवेशः पङ्क्तिः\n\
"));
      oputs (_("\
  -i, --निवेशः-range=LO-HI   treat each सङ्ख्या LO through HI as an निवेशः पङ्क्तिः\n\
"));
      oputs (_("\
  -n, --head-गणनम्=गणनम्_2    output at most गणनम्_2 पङ्क्तयः\n\
"));
      oputs (_("\
  -o, --output=FILE         लिख result to FILE instead of standard output\n\
"));
      oputs (_("\
      --random-source=FILE  get random bytes from FILE\n\
"));
      oputs (_("\
  -r, --repeat              output पङ्क्तयः can be repeated\n\
"));
      oputs (_("\
  -z, --zero-terminated     पङ्क्तिः delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }

  exit (status);
}

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
गणना
{
  RANDOM_SOURCE_OPTION = CHAR_MAX + 1
};

स्थिर संरचना option ध्रुव long_opts[] =
{
  {"echo", no_argument, रिक्त, 'e'},
  {"input-range", required_argument, रिक्त, 'i'},
  {"head-count", required_argument, रिक्त, 'n'},
  {"output", required_argument, रिक्त, 'o'},
  {"random-source", required_argument, रिक्त, RANDOM_SOURCE_OPTION},
  {"repeat", no_argument, रिक्त, 'r'},
  {"zero-terminated", no_argument, रिक्त, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0},
};

स्थिर शून्य
निवेशः_from_argv (वर्ण **operand, पूर्णाङ्क n_operands, वर्ण eolbyte)
{
  वर्ण *p;
  size_t size = n_operands;
  पूर्णाङ्क i;

  अत्थाय (i = 0; i < n_operands; i++)
    size += strlen (operand[i]);
  p = xmalloc (size);

  अत्थाय (i = 0; i < n_operands; i++)
    {
      वर्ण *p1 = stpcpy (p, operand[i]);
      operand[i] = p;
      p = p1;
      *p++ = eolbyte;
    }

  operand[n_operands] = p;
}

/* Return the start of the next line after LINE, which is guaranteed
   to end in EOLBYTE.  */

स्थिर वर्ण *
next_पङ्क्तिः (वर्ण *पङ्क्तिः, वर्ण eolbyte)
{
  वर्ण *p = rawmemchr (पङ्क्तिः, eolbyte);
  पटिनिवत्त p + 1;
}

/* Return the size of the input if possible or OFF_T_MAX if not.  */

स्थिर off_t
निवेशः_size (शून्य)
{
  off_t file_size;

  संरचना stat stat_buf;
  सचे (fstat (STDIN_FILENO, &stat_buf) != 0)
    पटिनिवत्त OFF_T_MAX;
  सचे (usable_st_size (&stat_buf))
    file_size = stat_buf.st_size;
  अञ्ञथा
    पटिनिवत्त OFF_T_MAX;

  off_t निवेशः_offset = lseek (STDIN_FILENO, 0, SEEK_CUR);
  सचे (निवेशः_offset < 0)
    पटिनिवत्त OFF_T_MAX;

  file_size -= निवेशः_offset;

  पटिनिवत्त file_size;
}

/* Read all lines and store up to K permuted lines in *OUT_RSRV.
   Return the number of lines read, up to a maximum of K.  */

स्थिर idx_t
read_निवेशः_reservoir_sampling (FILE *in, वर्ण eolbyte, idx_t k,
                               संरचना randint_source *s,
                               संरचना linebuffer **out_rsrv)
{
  randint n_पङ्क्तयः = 0;
  idx_t n_alloc_पङ्क्तयः = 0;
  संरचना linebuffer *पङ्क्तिः = रिक्त;
  संरचना linebuffer *rsrv = रिक्त;

  /* Fill the first K lines, directly into the reservoir.  */
  अत्थाय (n_पङ्क्तयः = 0; n_पङ्क्तयः < k; n_पङ्क्तयः++)
    {
      /* Enlarge reservoir if needed.  */
      सचे (n_पङ्क्तयः == n_alloc_पङ्क्तयः)
        {
          idx_t old = n_alloc_पङ्क्तयः;
          rsrv = xpalloc (rsrv, &n_alloc_पङ्क्तयः, 1, k, परिमाण *rsrv);
          memset (&rsrv[n_पङ्क्तयः], 0, (n_alloc_पङ्क्तयः - old) * परिमाण *rsrv);
        }

      पङ्क्तिः = readlinebuffer_delim (&rsrv[n_पङ्क्तयः], in, eolbyte);
      सचे (!पङ्क्तिः)
        भङ्ग;
    }

  /* last line wasn't null - so there may be more lines to read.  */
  सचे (पङ्क्तिः != रिक्त)
    {
      संरचना linebuffer dummy;
      initbuffer (&dummy);  /* space for lines not put in reservoir.  */

      /* Choose the fate of the next line, with decreasing probability (as
         n_lines increases in size).

         If the line will be used, store it directly in the reservoir.
         Otherwise, store it in dummy space.

         With 'struct linebuffer', storing into existing buffer will reduce
         re-allocations (will only re-allocate if the new line is longer than
         the currently allocated space).  */
      करोहि
        {
          randint j = randint_choose (s, n_पङ्क्तयः + 1);  /* 0 .. n_lines.  */
          पङ्क्तिः = (j < k) ? (&rsrv[j]) : (&dummy);
        }
      याव (readlinebuffer_delim (पङ्क्तिः, in, eolbyte) != रिक्त && n_पङ्क्तयः++);

      सचे (! n_पङ्क्तयः)
        error (EXIT_FAILURE, EOVERFLOW, _("too many input lines"));

      freebuffer (&dummy);
    }

  /* no more input lines, or an input error.  */
  सचे (ferror (in))
    error (EXIT_FAILURE, errno, _("read error"));

  *out_rsrv = rsrv;
  पटिनिवत्त MIN (k, n_पङ्क्तयः);
}

स्थिर पूर्णाङ्क
write_permuted_output_reservoir (size_t n_पङ्क्तयः, संरचना linebuffer *पङ्क्तयः,
                                 size_t ध्रुव *permutation)
{
  अत्थाय (size_t i = 0; i < n_पङ्क्तयः; i++)
    {
      ध्रुव संरचना linebuffer *p = &पङ्क्तयः[permutation[i]];
      सचे (fwrite (p->buffer, परिमाण (वर्ण), p->length, stdout) != p->length)
        पटिनिवत्त -1;
    }

  पटिनिवत्त 0;
}

/* Read data from file IN.  Input lines are delimited by EOLBYTE;
   silently append a trailing EOLBYTE if the file ends in some other
   byte.  Store a pointer to the resulting array of lines into *PLINE.
   Return the number of lines read.  Report an error and exit on
   failure.  */

स्थिर size_t
read_निवेशः (FILE *in, वर्ण eolbyte, वर्ण ***pline)
{
  वर्ण *p;
  वर्ण *buf = रिक्त;
  size_t used;
  वर्ण *lim;
  वर्ण **पङ्क्तिः;
  size_t n_पङ्क्तयः;

  /* TODO: We should limit the amount of data read here,
     to less than RESERVOIR_MIN_INPUT.  I.e., adjust fread_file() to support
     taking a byte limit.  We'd then need to ensure we handle a line spanning
     this boundary.  With that in place we could set use_reservoir_sampling
     when used==RESERVOIR_MIN_INPUT, and have read_input_reservoir_sampling()
     call a wrapper function to populate a linebuffer from the internal pline
     or if none left, stdin.  Doing that would give better performance by
     avoiding the reservoir CPU overhead when reading < RESERVOIR_MIN_INPUT
     from a pipe, and allow us to dispense with the input_size() function.  */
  सचे (!(buf = fread_file (in, 0, &used)))
    error (EXIT_FAILURE, errno, _("read error"));

  सचे (used && buf[used - 1] != eolbyte)
    buf[used++] = eolbyte;

  lim = buf + used;

  n_पङ्क्तयः = 0;
  अत्थाय (p = buf; p < lim; p = next_पङ्क्तिः (p, eolbyte))
    n_पङ्क्तयः++;

  *pline = पङ्क्तिः = xnmalloc (n_पङ्क्तयः + 1, परिमाण *पङ्क्तिः);

  पङ्क्तिः[0] = p = buf;
  अत्थाय (size_t i = 1; i <= n_पङ्क्तयः; i++)
    पङ्क्तिः[i] = p = next_पङ्क्तिः (p, eolbyte);

  पटिनिवत्त n_पङ्क्तयः;
}

/* Output N_LINES lines to stdout from LINE array,
   chosen by the indices in PERMUTATION.
   PERMUTATION and LINE must have at least N_LINES elements.
   Strings in LINE must include the line-terminator character.  */
स्थिर पूर्णाङ्क
write_permuted_पङ्क्तयः (size_t n_पङ्क्तयः, वर्ण *ध्रुव *पङ्क्तिः,
                      size_t ध्रुव *permutation)
{
  अत्थाय (size_t i = 0; i < n_पङ्क्तयः; i++)
    {
      वर्ण *ध्रुव *p = पङ्क्तिः + permutation[i];
      size_t len = p[1] - p[0];
      सचे (fwrite (p[0], परिमाण *p[0], len, stdout) != len)
        पटिनिवत्त -1;
    }

  पटिनिवत्त 0;
}

/* Print NUMBER followed by EOLBYTE to standard output.
   Return false on failure, true on success.  */
स्थिर bool
print_सङ्ख्या (unsigned long पूर्णाङ्क सङ्ख्या, वर्ण eolbyte)
{
  वर्ण buf[INT_BUFSIZE_BOUND (unsigned long पूर्णाङ्क)];
  वर्ण *p = buf + INT_STRLEN_BOUND (unsigned long पूर्णाङ्क);
  *p = eolbyte;
  करोहि
    *--p = '0' + सङ्ख्या % 10;
  याव ((सङ्ख्या /= 10) != 0);
  idx_t len = buf + परिमाण buf - p;
  पटिनिवत्त fwrite (p, 1, len, stdout) == len;
}

/* Output N_LINES of numbers to stdout, from PERMUTATION array.
   PERMUTATION must have at least N_LINES elements.  */
स्थिर पूर्णाङ्क
write_permuted_numbers (size_t n_पङ्क्तयः, size_t lo_निवेशः,
                        size_t ध्रुव *permutation, वर्ण eolbyte)
{
  अत्थाय (size_t i = 0; i < n_पङ्क्तयः; i++)
    {
      unsigned long पूर्णाङ्क n = lo_निवेशः + permutation[i];
      सचे (! print_सङ्ख्या (n, eolbyte))
        पटिनिवत्त -1;
    }

  पटिनिवत्त 0;
}

/* Output COUNT numbers to stdout, chosen randomly from range
   LO_INPUT through HI_INPUT.  */
स्थिर पूर्णाङ्क
write_random_numbers (संरचना randint_source *s, size_t गणनम्,
                      size_t lo_निवेशः, size_t hi_निवेशः, वर्ण eolbyte)
{
  ध्रुव randint range = hi_निवेशः - lo_निवेशः + 1;

  अत्थाय (size_t i = 0; i < गणनम्; i++)
    {
      unsigned long पूर्णाङ्क j = lo_निवेशः + randint_choose (s, range);
      सचे (! print_सङ्ख्या (j, eolbyte))
        पटिनिवत्त -1;
    }

  पटिनिवत्त 0;
}

/* Output COUNT lines to stdout from LINES array.
   LINES must have at least N_LINES elements in it.
   Strings in LINES_ must include the line-terminator character.  */
स्थिर पूर्णाङ्क
write_random_पङ्क्तयः (संरचना randint_source *s, size_t गणनम्,
                    वर्ण *ध्रुव *पङ्क्तयः, size_t n_पङ्क्तयः)
{
  अत्थाय (size_t i = 0; i < गणनम्; i++)
    {
      ध्रुव randint j = randint_choose (s, n_पङ्क्तयः);
      वर्ण *ध्रुव *p = पङ्क्तयः + j;
      size_t len = p[1] - p[0];
      सचे (fwrite (p[0], परिमाण *p[0], len, stdout) != len)
        पटिनिवत्त -1;
    }

  पटिनिवत्त 0;
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  bool echo = false;
  bool निवेशः_range = false;
  size_t lo_निवेशः = SIZE_MAX;
  size_t hi_निवेशः = 0;
  idx_t head_पङ्क्तयः = MIN (IDX_MAX, SIZE_MAX);
  वर्ण ध्रुव *outfile = रिक्त;
  वर्ण *random_source = रिक्त;
  वर्ण eolbyte = '\n';
  वर्ण **निवेशः_पङ्क्तयः = रिक्त;
  bool use_reservoir_sampling = false;
  bool repeat = false;

  पूर्णाङ्क optc;
  पूर्णाङ्क n_operands;
  वर्ण **operand;
  size_t n_पङ्क्तयः;
  वर्ण **पङ्क्तिः = रिक्त;
  संरचना linebuffer *reservoir = रिक्त;
  संरचना randint_source *randint_source;
  size_t *permutation = रिक्त;
  पूर्णाङ्क i;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  याव ((optc = getopt_long (argc, argv, "ei:n:o:rz", long_opts, रिक्त))
         != -1)
    विकल्प (optc)
      {
      पक्ष 'e':
        echo = true;
        भङ्ग;

      पक्ष 'i':
        {
          सचे (निवेशः_range)
            error (EXIT_FAILURE, 0, _("multiple -i options specified"));
          निवेशः_range = true;

          uintmax_t u;
          वर्ण *lo_end;
          strtol_error err = xstrtoumax (optarg, &lo_end, 10, &u, रिक्त);
          सचे (err == LONGINT_OK)
            {
              lo_निवेशः = u;
              सचे (lo_निवेशः != u)
                err = LONGINT_OVERFLOW;
              अञ्ञथा सचे (*lo_end != '-')
                err = LONGINT_INVALID;
              अञ्ञथा
                {
                  err = xstrtoumax (lo_end + 1, रिक्त, 10, &u, "");
                  सचे (err == LONGINT_OK)
                    {
                      hi_निवेशः = u;
                      सचे (hi_निवेशः != u)
                        err = LONGINT_OVERFLOW;
                    }
                }
            }

          n_पङ्क्तयः = hi_निवेशः - lo_निवेशः + 1;

          सचे (err != LONGINT_OK || (lo_निवेशः <= hi_निवेशः) == (n_पङ्क्तयः == 0))
            error (EXIT_FAILURE, err == LONGINT_OVERFLOW ? EOVERFLOW : 0,
                   "%s: %s", _("invalid input range"), quote (optarg));
        }
        भङ्ग;

      पक्ष 'n':
        {
          uintmax_t argval;
          strtol_error e = xstrtoumax (optarg, रिक्त, 10, &argval, "");

          सचे (e == LONGINT_OK)
            head_पङ्क्तयः = MIN (head_पङ्क्तयः, argval);
          अञ्ञथा सचे (e != LONGINT_OVERFLOW)
            error (EXIT_FAILURE, 0, _("invalid line count: %s"),
                   quote (optarg));
        }
        भङ्ग;

      पक्ष 'o':
        सचे (outfile && !streq (outfile, optarg))
          error (EXIT_FAILURE, 0, _("multiple output files specified"));
        outfile = optarg;
        भङ्ग;

      पक्ष RANDOM_SOURCE_OPTION:
        सचे (random_source && !streq (random_source, optarg))
          error (EXIT_FAILURE, 0, _("multiple random sources specified"));
        random_source = optarg;
        भङ्ग;

      पक्ष 'r':
        repeat = true;
        भङ्ग;

      पक्ष 'z':
        eolbyte = '\0';
        भङ्ग;

      case_GETOPT_HELP_CHAR;
      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
      स्वभाव:
        usage (EXIT_FAILURE);
      }

  n_operands = argc - optind;
  operand = argv + optind;

  /* Check invalid usage.  */
  सचे (echo && निवेशः_range)
    {
      error (0, 0, _("cannot combine -e and -i options"));
      usage (EXIT_FAILURE);
    }
  सचे (निवेशः_range ? 0 < n_operands : !echo && 1 < n_operands)
    {
      error (0, 0, _("extra operand %s"), quote (operand[!निवेशः_range]));
      usage (EXIT_FAILURE);
    }

  /* Prepare input.  */
  सचे (head_पङ्क्तयः == 0)
    {
      n_पङ्क्तयः = 0;
      पङ्क्तिः = रिक्त;
    }
  अञ्ञथा सचे (echo)
    {
      निवेशः_from_argv (operand, n_operands, eolbyte);
      n_पङ्क्तयः = n_operands;
      पङ्क्तिः = operand;
    }
  अञ्ञथा सचे (निवेशः_range)
    {
      IF_LINT (n_पङ्क्तयः = hi_निवेशः - lo_निवेशः + 1); /* Avoid GCC 10 warning.  */
      पङ्क्तिः = रिक्त;
    }
  अञ्ञथा
    {
      /* If an input file is specified, re-open it as stdin.  */
      सचे (n_operands == 1
          && ! (streq (operand[0], "-")
                || freopen (operand[0], "r", stdin)))
        error (EXIT_FAILURE, errno, "%s", quotef (operand[0]));

      fadvise (stdin, FADVISE_SEQUENTIAL);

      सचे (repeat || head_पङ्क्तयः == MIN (IDX_MAX, SIZE_MAX)
          || निवेशः_size () <= RESERVOIR_MIN_निवेशः)
        {
          n_पङ्क्तयः = read_निवेशः (stdin, eolbyte, &निवेशः_पङ्क्तयः);
          पङ्क्तिः = निवेशः_पङ्क्तयः;
        }
      अञ्ञथा
        {
          use_reservoir_sampling = true;
          n_पङ्क्तयः = SIZE_MAX;   /* unknown number of input lines, for now.  */
        }
    }

  /* The adjusted head line count; can be less than HEAD_LINES if the
     input is small and if not repeating.  */
  idx_t ahead_पङ्क्तयः = repeat || head_पङ्क्तयः < n_पङ्क्तयः ? head_पङ्क्तयः : n_पङ्क्तयः;

  randint_source = randint_all_new (random_source,
                                    (use_reservoir_sampling || repeat
                                     ? SIZE_MAX
                                     : randperm_bound (ahead_पङ्क्तयः, n_पङ्क्तयः)));
  सचे (! randint_source)
    error (EXIT_FAILURE, errno, "%s",
           quotef (random_source ? random_source : "getrandom"));

  सचे (use_reservoir_sampling)
    {
      /* Instead of reading the entire file into 'line',
         use reservoir-sampling to store just AHEAD_LINES random lines.  */
      n_पङ्क्तयः = read_निवेशः_reservoir_sampling (stdin, eolbyte, ahead_पङ्क्तयः,
                                               randint_source, &reservoir);
      ahead_पङ्क्तयः = n_पङ्क्तयः;
    }

  /* Close stdin now, rather than earlier, so that randint_all_new
     doesn't have to worry about opening something other than
     stdin.  */
  सचे (! (head_पङ्क्तयः == 0 || echo || निवेशः_range || fclose (stdin) == 0))
    error (EXIT_FAILURE, errno, _("read error"));

  सचे (!repeat)
    permutation = randperm_new (randint_source, ahead_पङ्क्तयः, n_पङ्क्तयः);

  सचे (outfile && ! freopen (outfile, "w", stdout))
    error (EXIT_FAILURE, errno, "%s", quotef (outfile));

  /* Generate output according to requested method */
  सचे (repeat)
    {
      सचे (head_पङ्क्तयः == 0)
        i = 0;
      अञ्ञथा
        {
          सचे (n_पङ्क्तयः == 0)
            error (EXIT_FAILURE, 0, _("no lines to repeat"));
          सचे (निवेशः_range)
            i = write_random_numbers (randint_source, ahead_पङ्क्तयः,
                                      lo_निवेशः, hi_निवेशः, eolbyte);
          अञ्ञथा
            i = write_random_पङ्क्तयः (randint_source, ahead_पङ्क्तयः, पङ्क्तिः, n_पङ्क्तयः);
        }
    }
  अञ्ञथा
    {
      सचे (use_reservoir_sampling)
        i = write_permuted_output_reservoir (n_पङ्क्तयः, reservoir, permutation);
      अञ्ञथा सचे (निवेशः_range)
        i = write_permuted_numbers (ahead_पङ्क्तयः, lo_निवेशः,
                                    permutation, eolbyte);
      अञ्ञथा
        i = write_permuted_पङ्क्तयः (ahead_पङ्क्तयः, पङ्क्तिः, permutation);
    }

  सचे (i != 0)
    write_error ();

  IF_LINT (randint_all_free (randint_source));  /* For older valgrind.  */

  main_exit (EXIT_SUCCESS);
}
