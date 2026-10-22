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
गणना { RESERVOIR_MIN_निवेश = 8192 * 1024 };


शून्य
usage (पूर्णाङ्क status)
{
  यदि (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s [OPTION]... [खाता]\n\
  or:  %s -e [OPTION]... [ARG]...\n\
  or:  %s -i LO-HI [OPTION]...\n\
"),
              program_name, program_name, program_name);
      ख_माला_दो (_("\
Write a random permutation of the निवेश पंक्तियाँ to standard output.\n\
"), मानक_निकास);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -e, --echo                treat each ARG as an निवेश पंक्ति\n\
"));
      oputs (_("\
  -i, --निवेश-range=LO-HI   treat each संख्या LO through HI as an निवेश पंक्ति\n\
"));
      oputs (_("\
  -n, --head-गिनती=गिनती_2    output at most गिनती_2 पंक्तियाँ\n\
"));
      oputs (_("\
  -o, --output=खाता         लिख result to खाता instead of standard output\n\
"));
      oputs (_("\
      --random-source=खाता  get random bytes from खाता\n\
"));
      oputs (_("\
  -r, --repeat              output पंक्तियाँ can be repeated\n\
"));
      oputs (_("\
  -z, --zero-terminated     पंक्ति delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }

  निकास (status);
}

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
गणना
{
  RANDOM_SOURCE_OPTION = अक्षर_उच्च + 1
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
निवेश_from_argv (वर्ण **operand, पूर्णाङ्क n_operands, वर्ण eolbyte)
{
  वर्ण *p;
  माप_प्रकार size = n_operands;
  पूर्णाङ्क i;

  प्रति (i = 0; i < n_operands; i++)
    size += म_माप (operand[i]);
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
next_पंक्ति (वर्ण *पंक्ति, वर्ण eolbyte)
{
  वर्ण *p = rawmemchr (पंक्ति, eolbyte);
  प्रत्यागम p + 1;
}

/* Return the size of the input if possible or OFF_T_MAX if not.  */

स्थिर off_t
निवेश_size (शून्य)
{
  off_t file_size;

  संरचना stat stat_buf;
  यदि (ख_स्थिति (STDIN_FILENO, &stat_buf) != 0)
    प्रत्यागम OFF_T_MAX;
  यदि (usable_st_size (&stat_buf))
    file_size = stat_buf.st_size;
  अन्यथा
    प्रत्यागम OFF_T_MAX;

  off_t निवेश_offset = lseek (STDIN_FILENO, 0, प्रस्तुत_से);
  यदि (निवेश_offset < 0)
    प्रत्यागम OFF_T_MAX;

  file_size -= निवेश_offset;

  प्रत्यागम file_size;
}

/* Read all lines and store up to K permuted lines in *OUT_RSRV.
   Return the number of lines read, up to a maximum of K.  */

स्थिर idx_t
read_निवेश_reservoir_sampling (खाता *in, वर्ण eolbyte, idx_t k,
                               संरचना randint_source *s,
                               संरचना linebuffer **out_rsrv)
{
  randint n_पंक्तियाँ = 0;
  idx_t n_alloc_पंक्तियाँ = 0;
  संरचना linebuffer *पंक्ति = रिक्त;
  संरचना linebuffer *rsrv = रिक्त;

  /* Fill the first K lines, directly into the reservoir.  */
  प्रति (n_पंक्तियाँ = 0; n_पंक्तियाँ < k; n_पंक्तियाँ++)
    {
      /* Enlarge reservoir if needed.  */
      यदि (n_पंक्तियाँ == n_alloc_पंक्तियाँ)
        {
          idx_t old = n_alloc_पंक्तियाँ;
          rsrv = xpalloc (rsrv, &n_alloc_पंक्तियाँ, 1, k, परिमाण *rsrv);
          स_रखो (&rsrv[n_पंक्तियाँ], 0, (n_alloc_पंक्तियाँ - old) * परिमाण *rsrv);
        }

      पंक्ति = readlinebuffer_delim (&rsrv[n_पंक्तियाँ], in, eolbyte);
      यदि (!पंक्ति)
        भङ्ग;
    }

  /* last line wasn't null - so there may be more lines to read.  */
  यदि (पंक्ति != रिक्त)
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
          randint j = randint_choose (s, n_पंक्तियाँ + 1);  /* 0 .. n_lines.  */
          पंक्ति = (j < k) ? (&rsrv[j]) : (&dummy);
        }
      यावत् (readlinebuffer_delim (पंक्ति, in, eolbyte) != रिक्त && n_पंक्तियाँ++);

      यदि (! n_पंक्तियाँ)
        error (निकास_त्रुटि, EOVERFLOW, _("too many input lines"));

      freebuffer (&dummy);
    }

  /* no more input lines, or an input error.  */
  यदि (ख_त्रुटि (in))
    error (निकास_त्रुटि, त्रुटि_सं, _("read error"));

  *out_rsrv = rsrv;
  प्रत्यागम MIN (k, n_पंक्तियाँ);
}

स्थिर पूर्णाङ्क
write_permuted_output_reservoir (माप_प्रकार n_पंक्तियाँ, संरचना linebuffer *पंक्तियाँ,
                                 माप_प्रकार ध्रुव *permutation)
{
  प्रति (माप_प्रकार i = 0; i < n_पंक्तियाँ; i++)
    {
      ध्रुव संरचना linebuffer *p = &पंक्तियाँ[permutation[i]];
      यदि (ख_डालो (p->buffer, परिमाण (वर्ण), p->length, मानक_निकास) != p->length)
        प्रत्यागम -1;
    }

  प्रत्यागम 0;
}

/* Read data from file IN.  Input lines are delimited by EOLBYTE;
   silently append a trailing EOLBYTE if the file ends in some other
   byte.  Store a pointer to the resulting array of lines into *PLINE.
   Return the number of lines read.  Report an error and exit on
   failure.  */

स्थिर माप_प्रकार
read_निवेश (खाता *in, वर्ण eolbyte, वर्ण ***pline)
{
  वर्ण *p;
  वर्ण *buf = रिक्त;
  माप_प्रकार used;
  वर्ण *lim;
  वर्ण **पंक्ति;
  माप_प्रकार n_पंक्तियाँ;

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
    error (निकास_त्रुटि, त्रुटि_सं, _("read error"));

  यदि (used && buf[used - 1] != eolbyte)
    buf[used++] = eolbyte;

  lim = buf + used;

  n_पंक्तियाँ = 0;
  प्रति (p = buf; p < lim; p = next_पंक्ति (p, eolbyte))
    n_पंक्तियाँ++;

  *pline = पंक्ति = xnmalloc (n_पंक्तियाँ + 1, परिमाण *पंक्ति);

  पंक्ति[0] = p = buf;
  प्रति (माप_प्रकार i = 1; i <= n_पंक्तियाँ; i++)
    पंक्ति[i] = p = next_पंक्ति (p, eolbyte);

  प्रत्यागम n_पंक्तियाँ;
}

/* Output N_LINES lines to stdout from LINE array,
   chosen by the indices in PERMUTATION.
   PERMUTATION and LINE must have at least N_LINES elements.
   Strings in LINE must include the line-terminator character.  */
स्थिर पूर्णाङ्क
write_permuted_पंक्तियाँ (माप_प्रकार n_पंक्तियाँ, वर्ण *ध्रुव *पंक्ति,
                      माप_प्रकार ध्रुव *permutation)
{
  प्रति (माप_प्रकार i = 0; i < n_पंक्तियाँ; i++)
    {
      वर्ण *ध्रुव *p = पंक्ति + permutation[i];
      माप_प्रकार len = p[1] - p[0];
      यदि (ख_डालो (p[0], परिमाण *p[0], len, मानक_निकास) != len)
        प्रत्यागम -1;
    }

  प्रत्यागम 0;
}

/* Print NUMBER followed by EOLBYTE to standard output.
   Return false on failure, true on success.  */
स्थिर bool
print_संख्या (अचिन्हित दीर्घ पूर्णाङ्क संख्या, वर्ण eolbyte)
{
  वर्ण buf[INT_BUFSIZE_BOUND (अचिन्हित दीर्घ पूर्णाङ्क)];
  वर्ण *p = buf + INT_STRLEN_BOUND (अचिन्हित दीर्घ पूर्णाङ्क);
  *p = eolbyte;
  कुरु
    *--p = '0' + संख्या % 10;
  यावत् ((संख्या /= 10) != 0);
  idx_t len = buf + परिमाण buf - p;
  प्रत्यागम ख_डालो (p, 1, len, मानक_निकास) == len;
}

/* Output N_LINES of numbers to stdout, from PERMUTATION array.
   PERMUTATION must have at least N_LINES elements.  */
स्थिर पूर्णाङ्क
write_permuted_numbers (माप_प्रकार n_पंक्तियाँ, माप_प्रकार lo_निवेश,
                        माप_प्रकार ध्रुव *permutation, वर्ण eolbyte)
{
  प्रति (माप_प्रकार i = 0; i < n_पंक्तियाँ; i++)
    {
      अचिन्हित दीर्घ पूर्णाङ्क n = lo_निवेश + permutation[i];
      यदि (! print_संख्या (n, eolbyte))
        प्रत्यागम -1;
    }

  प्रत्यागम 0;
}

/* Output COUNT numbers to stdout, chosen randomly from range
   LO_INPUT through HI_INPUT.  */
स्थिर पूर्णाङ्क
write_random_numbers (संरचना randint_source *s, माप_प्रकार गिनती,
                      माप_प्रकार lo_निवेश, माप_प्रकार hi_निवेश, वर्ण eolbyte)
{
  ध्रुव randint range = hi_निवेश - lo_निवेश + 1;

  प्रति (माप_प्रकार i = 0; i < गिनती; i++)
    {
      अचिन्हित दीर्घ पूर्णाङ्क j = lo_निवेश + randint_choose (s, range);
      यदि (! print_संख्या (j, eolbyte))
        प्रत्यागम -1;
    }

  प्रत्यागम 0;
}

/* Output COUNT lines to stdout from LINES array.
   LINES must have at least N_LINES elements in it.
   Strings in LINES_ must include the line-terminator character.  */
स्थिर पूर्णाङ्क
write_random_पंक्तियाँ (संरचना randint_source *s, माप_प्रकार गिनती,
                    वर्ण *ध्रुव *पंक्तियाँ, माप_प्रकार n_पंक्तियाँ)
{
  प्रति (माप_प्रकार i = 0; i < गिनती; i++)
    {
      ध्रुव randint j = randint_choose (s, n_पंक्तियाँ);
      वर्ण *ध्रुव *p = पंक्तियाँ + j;
      माप_प्रकार len = p[1] - p[0];
      यदि (ख_डालो (p[0], परिमाण *p[0], len, मानक_निकास) != len)
        प्रत्यागम -1;
    }

  प्रत्यागम 0;
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  bool echo = false;
  bool निवेश_range = false;
  माप_प्रकार lo_निवेश = SIZE_MAX;
  माप_प्रकार hi_निवेश = 0;
  idx_t head_पंक्तियाँ = MIN (IDX_MAX, SIZE_MAX);
  वर्ण ध्रुव *outfile = रिक्त;
  वर्ण *random_source = रिक्त;
  वर्ण eolbyte = '\n';
  वर्ण **निवेश_पंक्तियाँ = रिक्त;
  bool use_reservoir_sampling = false;
  bool repeat = false;

  पूर्णाङ्क optc;
  पूर्णाङ्क n_operands;
  वर्ण **operand;
  माप_प्रकार n_पंक्तियाँ;
  वर्ण **पंक्ति = रिक्त;
  संरचना linebuffer *reservoir = रिक्त;
  संरचना randint_source *randint_source;
  माप_प्रकार *permutation = रिक्त;
  पूर्णाङ्क i;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  यावत् ((optc = getopt_long (argc, argv, "ei:n:o:rz", long_opts, रिक्त))
         != -1)
    विकल्प (optc)
      {
      पक्ष 'e':
        echo = true;
        भङ्ग;

      पक्ष 'i':
        {
          यदि (निवेश_range)
            error (निकास_त्रुटि, 0, _("multiple -i options specified"));
          निवेश_range = true;

          uintmax_t u;
          वर्ण *lo_end;
          strtol_error err = xstrtoumax (optarg, &lo_end, 10, &u, रिक्त);
          यदि (err == LONGINT_OK)
            {
              lo_निवेश = u;
              यदि (lo_निवेश != u)
                err = LONGINT_OVERFLOW;
              अन्यथा यदि (*lo_end != '-')
                err = LONGINT_INVALID;
              अन्यथा
                {
                  err = xstrtoumax (lo_end + 1, रिक्त, 10, &u, "");
                  यदि (err == LONGINT_OK)
                    {
                      hi_निवेश = u;
                      यदि (hi_निवेश != u)
                        err = LONGINT_OVERFLOW;
                    }
                }
            }

          n_पंक्तियाँ = hi_निवेश - lo_निवेश + 1;

          यदि (err != LONGINT_OK || (lo_निवेश <= hi_निवेश) == (n_पंक्तियाँ == 0))
            error (निकास_त्रुटि, err == LONGINT_OVERFLOW ? EOVERFLOW : 0,
                   "%s: %s", _("invalid input range"), quote (optarg));
        }
        भङ्ग;

      पक्ष 'n':
        {
          uintmax_t argval;
          strtol_error e = xstrtoumax (optarg, रिक्त, 10, &argval, "");

          यदि (e == LONGINT_OK)
            head_पंक्तियाँ = MIN (head_पंक्तियाँ, argval);
          अन्यथा यदि (e != LONGINT_OVERFLOW)
            error (निकास_त्रुटि, 0, _("invalid line count: %s"),
                   quote (optarg));
        }
        भङ्ग;

      पक्ष 'o':
        यदि (outfile && !streq (outfile, optarg))
          error (निकास_त्रुटि, 0, _("multiple output files specified"));
        outfile = optarg;
        भङ्ग;

      पक्ष RANDOM_SOURCE_OPTION:
        यदि (random_source && !streq (random_source, optarg))
          error (निकास_त्रुटि, 0, _("multiple random sources specified"));
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
        usage (निकास_त्रुटि);
      }

  n_operands = argc - optind;
  operand = argv + optind;

  /* Check invalid usage.  */
  यदि (echo && निवेश_range)
    {
      error (0, 0, _("cannot combine -e and -i options"));
      usage (निकास_त्रुटि);
    }
  यदि (निवेश_range ? 0 < n_operands : !echo && 1 < n_operands)
    {
      error (0, 0, _("extra operand %s"), quote (operand[!निवेश_range]));
      usage (निकास_त्रुटि);
    }

  /* Prepare input.  */
  यदि (head_पंक्तियाँ == 0)
    {
      n_पंक्तियाँ = 0;
      पंक्ति = रिक्त;
    }
  अन्यथा यदि (echo)
    {
      निवेश_from_argv (operand, n_operands, eolbyte);
      n_पंक्तियाँ = n_operands;
      पंक्ति = operand;
    }
  अन्यथा यदि (निवेश_range)
    {
      IF_LINT (n_पंक्तियाँ = hi_निवेश - lo_निवेश + 1); /* Avoid GCC 10 warning.  */
      पंक्ति = रिक्त;
    }
  अन्यथा
    {
      /* If an input file is specified, re-open it as stdin.  */
      यदि (n_operands == 1
          && ! (streq (operand[0], "-")
                || ख_व_खोलो (operand[0], "r", मानक_निवेश)))
        error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (operand[0]));

      fadvise (मानक_निवेश, FADVISE_SEQUENTIAL);

      यदि (repeat || head_पंक्तियाँ == MIN (IDX_MAX, SIZE_MAX)
          || निवेश_size () <= RESERVOIR_MIN_निवेश)
        {
          n_पंक्तियाँ = read_निवेश (मानक_निवेश, eolbyte, &निवेश_पंक्तियाँ);
          पंक्ति = निवेश_पंक्तियाँ;
        }
      अन्यथा
        {
          use_reservoir_sampling = true;
          n_पंक्तियाँ = SIZE_MAX;   /* unknown number of input lines, for now.  */
        }
    }

  /* The adjusted head line count; can be less than HEAD_LINES if the
     input is small and if not repeating.  */
  idx_t ahead_पंक्तियाँ = repeat || head_पंक्तियाँ < n_पंक्तियाँ ? head_पंक्तियाँ : n_पंक्तियाँ;

  randint_source = randint_all_new (random_source,
                                    (use_reservoir_sampling || repeat
                                     ? SIZE_MAX
                                     : randperm_bound (ahead_पंक्तियाँ, n_पंक्तियाँ)));
  यदि (! randint_source)
    error (निकास_त्रुटि, त्रुटि_सं, "%s",
           quotef (random_source ? random_source : "getrandom"));

  यदि (use_reservoir_sampling)
    {
      /* Instead of reading the entire file into 'line',
         use reservoir-sampling to store just AHEAD_LINES random lines.  */
      n_पंक्तियाँ = read_निवेश_reservoir_sampling (मानक_निवेश, eolbyte, ahead_पंक्तियाँ,
                                               randint_source, &reservoir);
      ahead_पंक्तियाँ = n_पंक्तियाँ;
    }

  /* Close stdin now, rather than earlier, so that randint_all_new
     doesn't have to worry about opening something other than
     stdin.  */
  यदि (! (head_पंक्तियाँ == 0 || echo || निवेश_range || ख_बंद (मानक_निवेश) == 0))
    error (निकास_त्रुटि, त्रुटि_सं, _("read error"));

  यदि (!repeat)
    permutation = randperm_new (randint_source, ahead_पंक्तियाँ, n_पंक्तियाँ);

  यदि (outfile && ! ख_व_खोलो (outfile, "w", मानक_निकास))
    error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (outfile));

  /* Generate output according to requested method */
  यदि (repeat)
    {
      यदि (head_पंक्तियाँ == 0)
        i = 0;
      अन्यथा
        {
          यदि (n_पंक्तियाँ == 0)
            error (निकास_त्रुटि, 0, _("no lines to repeat"));
          यदि (निवेश_range)
            i = write_random_numbers (randint_source, ahead_पंक्तियाँ,
                                      lo_निवेश, hi_निवेश, eolbyte);
          अन्यथा
            i = write_random_पंक्तियाँ (randint_source, ahead_पंक्तियाँ, पंक्ति, n_पंक्तियाँ);
        }
    }
  अन्यथा
    {
      यदि (use_reservoir_sampling)
        i = write_permuted_output_reservoir (n_पंक्तियाँ, reservoir, permutation);
      अन्यथा यदि (निवेश_range)
        i = write_permuted_numbers (ahead_पंक्तियाँ, lo_निवेश,
                                    permutation, eolbyte);
      अन्यथा
        i = write_permuted_पंक्तियाँ (ahead_पंक्तियाँ, पंक्ति, permutation);
    }

  यदि (i != 0)
    write_error ();

  IF_LINT (randint_all_free (randint_source));  /* For older valgrind.  */

  main_exit (निकास_सफल);
}
