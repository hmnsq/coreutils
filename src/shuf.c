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
गणना { RESERVOIR_MIN_आदान = 8192 * 1024 };


शून्य
usage (पूर्णाङ्क status)
{
  यदि (status != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]\n\
  or:  %s -e [OPTION]... [ARG]...\n\
  or:  %s -i LO-HI [OPTION]...\n\
"),
              program_name, program_name, program_name);
      fputs (_("\
Write a random permutation of the आदान ओळी to standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -e, --echo                treat each ARG as an आदान ओळ\n\
"));
      oputs (_("\
  -i, --आदान-range=LO-HI   treat each संख्या LO through HI as an आदान ओळ\n\
"));
      oputs (_("\
  -n, --head-मोजणी=मोजणी_2    output at most मोजणी_2 ओळी\n\
"));
      oputs (_("\
  -o, --output=FILE         लिख result to FILE instead of standard output\n\
"));
      oputs (_("\
      --random-source=FILE  get random bytes from FILE\n\
"));
      oputs (_("\
  -r, --repeat              output ओळी can be repeated\n\
"));
      oputs (_("\
  -z, --zero-terminated     ओळ delimiter is NUL, not newline\n\
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
आदान_from_argv (वर्ण **operand, पूर्णाङ्क n_operands, वर्ण eolbyte)
{
  वर्ण *p;
  size_t size = n_operands;
  पूर्णाङ्क i;

  प्रति (i = 0; i < n_operands; i++)
    size += strlen (operand[i]);
  p = xmalloc (size);

  प्रति (i = 0; i < n_operands; i++)
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
next_ओळ (वर्ण *ओळ, वर्ण eolbyte)
{
  वर्ण *p = rawmemchr (ओळ, eolbyte);
  प्रत्यागम p + 1;
}

/* Return the size of the input if possible or OFF_T_MAX if not.  */

स्थिर off_t
आदान_size (शून्य)
{
  off_t file_size;

  संरचना stat stat_buf;
  यदि (fstat (STDIN_FILENO, &stat_buf) != 0)
    प्रत्यागम OFF_T_MAX;
  यदि (usable_st_size (&stat_buf))
    file_size = stat_buf.st_size;
  अन्यथा
    प्रत्यागम OFF_T_MAX;

  off_t आदान_offset = lseek (STDIN_FILENO, 0, SEEK_CUR);
  यदि (आदान_offset < 0)
    प्रत्यागम OFF_T_MAX;

  file_size -= आदान_offset;

  प्रत्यागम file_size;
}

/* Read all lines and store up to K permuted lines in *OUT_RSRV.
   Return the number of lines read, up to a maximum of K.  */

स्थिर idx_t
read_आदान_reservoir_sampling (FILE *in, वर्ण eolbyte, idx_t k,
                               संरचना randint_source *s,
                               संरचना linebuffer **out_rsrv)
{
  randint n_ओळी = 0;
  idx_t n_alloc_ओळी = 0;
  संरचना linebuffer *ओळ = रिक्त;
  संरचना linebuffer *rsrv = रिक्त;

  /* Fill the first K lines, directly into the reservoir.  */
  प्रति (n_ओळी = 0; n_ओळी < k; n_ओळी++)
    {
      /* Enlarge reservoir if needed.  */
      यदि (n_ओळी == n_alloc_ओळी)
        {
          idx_t old = n_alloc_ओळी;
          rsrv = xpalloc (rsrv, &n_alloc_ओळी, 1, k, परिमाण *rsrv);
          memset (&rsrv[n_ओळी], 0, (n_alloc_ओळी - old) * परिमाण *rsrv);
        }

      ओळ = readlinebuffer_delim (&rsrv[n_ओळी], in, eolbyte);
      यदि (!ओळ)
        भङ्ग;
    }

  /* last line wasn't null - so there may be more lines to read.  */
  यदि (ओळ != रिक्त)
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
      कुरु
        {
          randint j = randint_choose (s, n_ओळी + 1);  /* 0 .. n_lines.  */
          ओळ = (j < k) ? (&rsrv[j]) : (&dummy);
        }
      यावत् (readlinebuffer_delim (ओळ, in, eolbyte) != रिक्त && n_ओळी++);

      यदि (! n_ओळी)
        error (EXIT_FAILURE, EOVERFLOW, _("too many input lines"));

      freebuffer (&dummy);
    }

  /* no more input lines, or an input error.  */
  यदि (ferror (in))
    error (EXIT_FAILURE, errno, _("read error"));

  *out_rsrv = rsrv;
  प्रत्यागम MIN (k, n_ओळी);
}

स्थिर पूर्णाङ्क
write_permuted_output_reservoir (size_t n_ओळी, संरचना linebuffer *ओळी,
                                 size_t ध्रुव *permutation)
{
  प्रति (size_t i = 0; i < n_ओळी; i++)
    {
      ध्रुव संरचना linebuffer *p = &ओळी[permutation[i]];
      यदि (fwrite (p->buffer, परिमाण (वर्ण), p->length, stdout) != p->length)
        प्रत्यागम -1;
    }

  प्रत्यागम 0;
}

/* Read data from file IN.  Input lines are delimited by EOLBYTE;
   silently append a trailing EOLBYTE if the file ends in some other
   byte.  Store a pointer to the resulting array of lines into *PLINE.
   Return the number of lines read.  Report an error and exit on
   failure.  */

स्थिर size_t
read_आदान (FILE *in, वर्ण eolbyte, वर्ण ***pline)
{
  वर्ण *p;
  वर्ण *buf = रिक्त;
  size_t used;
  वर्ण *lim;
  वर्ण **ओळ;
  size_t n_ओळी;

  /* TODO: We should limit the amount of data read here,
     to less than RESERVOIR_MIN_INPUT.  I.e., adjust fread_file() to support
     taking a byte limit.  We'd then need to ensure we handle a line spanning
     this boundary.  With that in place we could set use_reservoir_sampling
     when used==RESERVOIR_MIN_INPUT, and have read_input_reservoir_sampling()
     call a wrapper function to populate a linebuffer from the internal pline
     or if none left, stdin.  Doing that would give better performance by
     avoiding the reservoir CPU overhead when reading < RESERVOIR_MIN_INPUT
     from a pipe, and allow us to dispense with the input_size() function.  */
  यदि (!(buf = fread_file (in, 0, &used)))
    error (EXIT_FAILURE, errno, _("read error"));

  यदि (used && buf[used - 1] != eolbyte)
    buf[used++] = eolbyte;

  lim = buf + used;

  n_ओळी = 0;
  प्रति (p = buf; p < lim; p = next_ओळ (p, eolbyte))
    n_ओळी++;

  *pline = ओळ = xnmalloc (n_ओळी + 1, परिमाण *ओळ);

  ओळ[0] = p = buf;
  प्रति (size_t i = 1; i <= n_ओळी; i++)
    ओळ[i] = p = next_ओळ (p, eolbyte);

  प्रत्यागम n_ओळी;
}

/* Output N_LINES lines to stdout from LINE array,
   chosen by the indices in PERMUTATION.
   PERMUTATION and LINE must have at least N_LINES elements.
   Strings in LINE must include the line-terminator character.  */
स्थिर पूर्णाङ्क
write_permuted_ओळी (size_t n_ओळी, वर्ण *ध्रुव *ओळ,
                      size_t ध्रुव *permutation)
{
  प्रति (size_t i = 0; i < n_ओळी; i++)
    {
      वर्ण *ध्रुव *p = ओळ + permutation[i];
      size_t len = p[1] - p[0];
      यदि (fwrite (p[0], परिमाण *p[0], len, stdout) != len)
        प्रत्यागम -1;
    }

  प्रत्यागम 0;
}

/* Print NUMBER followed by EOLBYTE to standard output.
   Return false on failure, true on success.  */
स्थिर bool
print_संख्या (unsigned long पूर्णाङ्क संख्या, वर्ण eolbyte)
{
  वर्ण buf[INT_BUFSIZE_BOUND (unsigned long पूर्णाङ्क)];
  वर्ण *p = buf + INT_STRLEN_BOUND (unsigned long पूर्णाङ्क);
  *p = eolbyte;
  कुरु
    *--p = '0' + संख्या % 10;
  यावत् ((संख्या /= 10) != 0);
  idx_t len = buf + परिमाण buf - p;
  प्रत्यागम fwrite (p, 1, len, stdout) == len;
}

/* Output N_LINES of numbers to stdout, from PERMUTATION array.
   PERMUTATION must have at least N_LINES elements.  */
स्थिर पूर्णाङ्क
write_permuted_numbers (size_t n_ओळी, size_t lo_आदान,
                        size_t ध्रुव *permutation, वर्ण eolbyte)
{
  प्रति (size_t i = 0; i < n_ओळी; i++)
    {
      unsigned long पूर्णाङ्क n = lo_आदान + permutation[i];
      यदि (! print_संख्या (n, eolbyte))
        प्रत्यागम -1;
    }

  प्रत्यागम 0;
}

/* Output COUNT numbers to stdout, chosen randomly from range
   LO_INPUT through HI_INPUT.  */
स्थिर पूर्णाङ्क
write_random_numbers (संरचना randint_source *s, size_t मोजणी,
                      size_t lo_आदान, size_t hi_आदान, वर्ण eolbyte)
{
  ध्रुव randint range = hi_आदान - lo_आदान + 1;

  प्रति (size_t i = 0; i < मोजणी; i++)
    {
      unsigned long पूर्णाङ्क j = lo_आदान + randint_choose (s, range);
      यदि (! print_संख्या (j, eolbyte))
        प्रत्यागम -1;
    }

  प्रत्यागम 0;
}

/* Output COUNT lines to stdout from LINES array.
   LINES must have at least N_LINES elements in it.
   Strings in LINES_ must include the line-terminator character.  */
स्थिर पूर्णाङ्क
write_random_ओळी (संरचना randint_source *s, size_t मोजणी,
                    वर्ण *ध्रुव *ओळी, size_t n_ओळी)
{
  प्रति (size_t i = 0; i < मोजणी; i++)
    {
      ध्रुव randint j = randint_choose (s, n_ओळी);
      वर्ण *ध्रुव *p = ओळी + j;
      size_t len = p[1] - p[0];
      यदि (fwrite (p[0], परिमाण *p[0], len, stdout) != len)
        प्रत्यागम -1;
    }

  प्रत्यागम 0;
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  bool echo = false;
  bool आदान_range = false;
  size_t lo_आदान = SIZE_MAX;
  size_t hi_आदान = 0;
  idx_t head_ओळी = MIN (IDX_MAX, SIZE_MAX);
  वर्ण ध्रुव *outfile = रिक्त;
  वर्ण *random_source = रिक्त;
  वर्ण eolbyte = '\n';
  वर्ण **आदान_ओळी = रिक्त;
  bool use_reservoir_sampling = false;
  bool repeat = false;

  पूर्णाङ्क optc;
  पूर्णाङ्क n_operands;
  वर्ण **operand;
  size_t n_ओळी;
  वर्ण **ओळ = रिक्त;
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

  यावत् ((optc = getopt_long (argc, argv, "ei:n:o:rz", long_opts, रिक्त))
         != -1)
    विकल्प (optc)
      {
      पक्ष 'e':
        echo = true;
        भङ्ग;

      पक्ष 'i':
        {
          यदि (आदान_range)
            error (EXIT_FAILURE, 0, _("multiple -i options specified"));
          आदान_range = true;

          uintmax_t u;
          वर्ण *lo_end;
          strtol_error err = xstrtoumax (optarg, &lo_end, 10, &u, रिक्त);
          यदि (err == LONGINT_OK)
            {
              lo_आदान = u;
              यदि (lo_आदान != u)
                err = LONGINT_OVERFLOW;
              अन्यथा यदि (*lo_end != '-')
                err = LONGINT_INVALID;
              अन्यथा
                {
                  err = xstrtoumax (lo_end + 1, रिक्त, 10, &u, "");
                  यदि (err == LONGINT_OK)
                    {
                      hi_आदान = u;
                      यदि (hi_आदान != u)
                        err = LONGINT_OVERFLOW;
                    }
                }
            }

          n_ओळी = hi_आदान - lo_आदान + 1;

          यदि (err != LONGINT_OK || (lo_आदान <= hi_आदान) == (n_ओळी == 0))
            error (EXIT_FAILURE, err == LONGINT_OVERFLOW ? EOVERFLOW : 0,
                   "%s: %s", _("invalid input range"), quote (optarg));
        }
        भङ्ग;

      पक्ष 'n':
        {
          uintmax_t argval;
          strtol_error e = xstrtoumax (optarg, रिक्त, 10, &argval, "");

          यदि (e == LONGINT_OK)
            head_ओळी = MIN (head_ओळी, argval);
          अन्यथा यदि (e != LONGINT_OVERFLOW)
            error (EXIT_FAILURE, 0, _("invalid line count: %s"),
                   quote (optarg));
        }
        भङ्ग;

      पक्ष 'o':
        यदि (outfile && !streq (outfile, optarg))
          error (EXIT_FAILURE, 0, _("multiple output files specified"));
        outfile = optarg;
        भङ्ग;

      पक्ष RANDOM_SOURCE_OPTION:
        यदि (random_source && !streq (random_source, optarg))
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
  यदि (echo && आदान_range)
    {
      error (0, 0, _("cannot combine -e and -i options"));
      usage (EXIT_FAILURE);
    }
  यदि (आदान_range ? 0 < n_operands : !echo && 1 < n_operands)
    {
      error (0, 0, _("extra operand %s"), quote (operand[!आदान_range]));
      usage (EXIT_FAILURE);
    }

  /* Prepare input.  */
  यदि (head_ओळी == 0)
    {
      n_ओळी = 0;
      ओळ = रिक्त;
    }
  अन्यथा यदि (echo)
    {
      आदान_from_argv (operand, n_operands, eolbyte);
      n_ओळी = n_operands;
      ओळ = operand;
    }
  अन्यथा यदि (आदान_range)
    {
      IF_LINT (n_ओळी = hi_आदान - lo_आदान + 1); /* Avoid GCC 10 warning.  */
      ओळ = रिक्त;
    }
  अन्यथा
    {
      /* If an input file is specified, re-open it as stdin.  */
      यदि (n_operands == 1
          && ! (streq (operand[0], "-")
                || freopen (operand[0], "r", stdin)))
        error (EXIT_FAILURE, errno, "%s", quotef (operand[0]));

      fadvise (stdin, FADVISE_SEQUENTIAL);

      यदि (repeat || head_ओळी == MIN (IDX_MAX, SIZE_MAX)
          || आदान_size () <= RESERVOIR_MIN_आदान)
        {
          n_ओळी = read_आदान (stdin, eolbyte, &आदान_ओळी);
          ओळ = आदान_ओळी;
        }
      अन्यथा
        {
          use_reservoir_sampling = true;
          n_ओळी = SIZE_MAX;   /* unknown number of input lines, for now.  */
        }
    }

  /* The adjusted head line count; can be less than HEAD_LINES if the
     input is small and if not repeating.  */
  idx_t ahead_ओळी = repeat || head_ओळी < n_ओळी ? head_ओळी : n_ओळी;

  randint_source = randint_all_new (random_source,
                                    (use_reservoir_sampling || repeat
                                     ? SIZE_MAX
                                     : randperm_bound (ahead_ओळी, n_ओळी)));
  यदि (! randint_source)
    error (EXIT_FAILURE, errno, "%s",
           quotef (random_source ? random_source : "getrandom"));

  यदि (use_reservoir_sampling)
    {
      /* Instead of reading the entire file into 'line',
         use reservoir-sampling to store just AHEAD_LINES random lines.  */
      n_ओळी = read_आदान_reservoir_sampling (stdin, eolbyte, ahead_ओळी,
                                               randint_source, &reservoir);
      ahead_ओळी = n_ओळी;
    }

  /* Close stdin now, rather than earlier, so that randint_all_new
     doesn't have to worry about opening something other than
     stdin.  */
  यदि (! (head_ओळी == 0 || echo || आदान_range || fclose (stdin) == 0))
    error (EXIT_FAILURE, errno, _("read error"));

  यदि (!repeat)
    permutation = randperm_new (randint_source, ahead_ओळी, n_ओळी);

  यदि (outfile && ! freopen (outfile, "w", stdout))
    error (EXIT_FAILURE, errno, "%s", quotef (outfile));

  /* Generate output according to requested method */
  यदि (repeat)
    {
      यदि (head_ओळी == 0)
        i = 0;
      अन्यथा
        {
          यदि (n_ओळी == 0)
            error (EXIT_FAILURE, 0, _("no lines to repeat"));
          यदि (आदान_range)
            i = write_random_numbers (randint_source, ahead_ओळी,
                                      lo_आदान, hi_आदान, eolbyte);
          अन्यथा
            i = write_random_ओळी (randint_source, ahead_ओळी, ओळ, n_ओळी);
        }
    }
  अन्यथा
    {
      यदि (use_reservoir_sampling)
        i = write_permuted_output_reservoir (n_ओळी, reservoir, permutation);
      अन्यथा यदि (आदान_range)
        i = write_permuted_numbers (ahead_ओळी, lo_आदान,
                                    permutation, eolbyte);
      अन्यथा
        i = write_permuted_ओळी (ahead_ओळी, ओळ, permutation);
    }

  यदि (i != 0)
    write_error ();

  IF_LINT (randint_all_free (randint_source));  /* For older valgrind.  */

  main_exit (EXIT_SUCCESS);
}
