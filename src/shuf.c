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
usage (पूर्णांक status)
{
  जर (status != निकास_सफल)
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
Write a random permutation of the आदान ओळी to standard output.\n\
"), मानक_निकास);

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
  -o, --output=खाता         लिख result to खाता instead of standard output\n\
"));
      oputs (_("\
      --random-source=खाता  get random bytes from खाता\n\
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

  निकास (status);
}

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
गणना
{
  RANDOM_SOURCE_OPTION = अक्षर_उच्च + 1
};

स्थिर संरचना option स्थिरांक long_opts[] =
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
आदान_from_argv (अक्षर **operand, पूर्णांक n_operands, अक्षर eolbyte)
{
  अक्षर *p;
  माप_प्रकार size = n_operands;
  पूर्णांक i;

  साठी (i = 0; i < n_operands; i++)
    size += म_माप (operand[i]);
  p = xmalloc (size);

  साठी (i = 0; i < n_operands; i++)
    {
      अक्षर *p1 = stpcpy (p, operand[i]);
      operand[i] = p;
      p = p1;
      *p++ = eolbyte;
    }

  operand[n_operands] = p;
}

/* Return the start of the next line after LINE, which is guaranteed
   to end in EOLBYTE.  */

स्थिर अक्षर *
next_ओळ (अक्षर *ओळ, अक्षर eolbyte)
{
  अक्षर *p = rawmemchr (ओळ, eolbyte);
  परत p + 1;
}

/* Return the size of the input if possible or OFF_T_MAX if not.  */

स्थिर off_t
आदान_size (शून्य)
{
  off_t file_size;

  संरचना stat stat_buf;
  जर (ख_स्थिति (STDIN_FILENO, &stat_buf) != 0)
    परत OFF_T_MAX;
  जर (usable_st_size (&stat_buf))
    file_size = stat_buf.st_size;
  अन्यथा
    परत OFF_T_MAX;

  off_t आदान_offset = lseek (STDIN_FILENO, 0, प्रस्तुत_से);
  जर (आदान_offset < 0)
    परत OFF_T_MAX;

  file_size -= आदान_offset;

  परत file_size;
}

/* Read all lines and store up to K permuted lines in *OUT_RSRV.
   Return the number of lines read, up to a maximum of K.  */

स्थिर idx_t
read_आदान_reservoir_sampling (खाता *in, अक्षर eolbyte, idx_t k,
                               संरचना randint_source *s,
                               संरचना linebuffer **out_rsrv)
{
  randint n_ओळी = 0;
  idx_t n_alloc_ओळी = 0;
  संरचना linebuffer *ओळ = रिक्त;
  संरचना linebuffer *rsrv = रिक्त;

  /* Fill the first K lines, directly into the reservoir.  */
  साठी (n_ओळी = 0; n_ओळी < k; n_ओळी++)
    {
      /* Enlarge reservoir if needed.  */
      जर (n_ओळी == n_alloc_ओळी)
        {
          idx_t old = n_alloc_ओळी;
          rsrv = xpalloc (rsrv, &n_alloc_ओळी, 1, k, आकार *rsrv);
          स_रखो (&rsrv[n_ओळी], 0, (n_alloc_ओळी - old) * आकार *rsrv);
        }

      ओळ = readlinebuffer_delim (&rsrv[n_ओळी], in, eolbyte);
      जर (!ओळ)
        थांबा;
    }

  /* last line wasn't null - so there may be more lines to read.  */
  जर (ओळ != रिक्त)
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
      करा
        {
          randint j = randint_choose (s, n_ओळी + 1);  /* 0 .. n_lines.  */
          ओळ = (j < k) ? (&rsrv[j]) : (&dummy);
        }
      पर्यंत (readlinebuffer_delim (ओळ, in, eolbyte) != रिक्त && n_ओळी++);

      जर (! n_ओळी)
        error (निकास_त्रुटि, EOVERFLOW, _("too many input lines"));

      freebuffer (&dummy);
    }

  /* no more input lines, or an input error.  */
  जर (ख_त्रुटि (in))
    error (निकास_त्रुटि, त्रुटि_सं, _("read error"));

  *out_rsrv = rsrv;
  परत MIN (k, n_ओळी);
}

स्थिर पूर्णांक
write_permuted_output_reservoir (माप_प्रकार n_ओळी, संरचना linebuffer *ओळी,
                                 माप_प्रकार स्थिरांक *permutation)
{
  साठी (माप_प्रकार i = 0; i < n_ओळी; i++)
    {
      स्थिरांक संरचना linebuffer *p = &ओळी[permutation[i]];
      जर (ख_डालो (p->buffer, आकार (अक्षर), p->length, मानक_निकास) != p->length)
        परत -1;
    }

  परत 0;
}

/* Read data from file IN.  Input lines are delimited by EOLBYTE;
   silently append a trailing EOLBYTE if the file ends in some other
   byte.  Store a pointer to the resulting array of lines into *PLINE.
   Return the number of lines read.  Report an error and exit on
   failure.  */

स्थिर माप_प्रकार
read_आदान (खाता *in, अक्षर eolbyte, अक्षर ***pline)
{
  अक्षर *p;
  अक्षर *buf = रिक्त;
  माप_प्रकार used;
  अक्षर *lim;
  अक्षर **ओळ;
  माप_प्रकार n_ओळी;

  /* TODO: We should limit the amount of data read here,
     to less than RESERVOIR_MIN_INPUT.  I.e., adjust fread_file() to support
     taking a byte limit.  We'd then need to ensure we handle a line spanning
     this boundary.  With that in place we could set use_reservoir_sampling
     when used==RESERVOIR_MIN_INPUT, and have read_input_reservoir_sampling()
     call a wrapper function to populate a linebuffer from the internal pline
     or if none left, stdin.  Doing that would give better performance by
     avoiding the reservoir CPU overhead when reading < RESERVOIR_MIN_INPUT
     from a pipe, and allow us to dispense with the input_size() function.  */
  जर (!(buf = fread_file (in, 0, &used)))
    error (निकास_त्रुटि, त्रुटि_सं, _("read error"));

  जर (used && buf[used - 1] != eolbyte)
    buf[used++] = eolbyte;

  lim = buf + used;

  n_ओळी = 0;
  साठी (p = buf; p < lim; p = next_ओळ (p, eolbyte))
    n_ओळी++;

  *pline = ओळ = xnmalloc (n_ओळी + 1, आकार *ओळ);

  ओळ[0] = p = buf;
  साठी (माप_प्रकार i = 1; i <= n_ओळी; i++)
    ओळ[i] = p = next_ओळ (p, eolbyte);

  परत n_ओळी;
}

/* Output N_LINES lines to stdout from LINE array,
   chosen by the indices in PERMUTATION.
   PERMUTATION and LINE must have at least N_LINES elements.
   Strings in LINE must include the line-terminator character.  */
स्थिर पूर्णांक
write_permuted_ओळी (माप_प्रकार n_ओळी, अक्षर *स्थिरांक *ओळ,
                      माप_प्रकार स्थिरांक *permutation)
{
  साठी (माप_प्रकार i = 0; i < n_ओळी; i++)
    {
      अक्षर *स्थिरांक *p = ओळ + permutation[i];
      माप_प्रकार len = p[1] - p[0];
      जर (ख_डालो (p[0], आकार *p[0], len, मानक_निकास) != len)
        परत -1;
    }

  परत 0;
}

/* Print NUMBER followed by EOLBYTE to standard output.
   Return false on failure, true on success.  */
स्थिर bool
print_संख्या (अचिन्हित दीर्घ पूर्णांक संख्या, अक्षर eolbyte)
{
  अक्षर buf[INT_BUFSIZE_BOUND (अचिन्हित दीर्घ पूर्णांक)];
  अक्षर *p = buf + INT_STRLEN_BOUND (अचिन्हित दीर्घ पूर्णांक);
  *p = eolbyte;
  करा
    *--p = '0' + संख्या % 10;
  पर्यंत ((संख्या /= 10) != 0);
  idx_t len = buf + आकार buf - p;
  परत ख_डालो (p, 1, len, मानक_निकास) == len;
}

/* Output N_LINES of numbers to stdout, from PERMUTATION array.
   PERMUTATION must have at least N_LINES elements.  */
स्थिर पूर्णांक
write_permuted_numbers (माप_प्रकार n_ओळी, माप_प्रकार lo_आदान,
                        माप_प्रकार स्थिरांक *permutation, अक्षर eolbyte)
{
  साठी (माप_प्रकार i = 0; i < n_ओळी; i++)
    {
      अचिन्हित दीर्घ पूर्णांक n = lo_आदान + permutation[i];
      जर (! print_संख्या (n, eolbyte))
        परत -1;
    }

  परत 0;
}

/* Output COUNT numbers to stdout, chosen randomly from range
   LO_INPUT through HI_INPUT.  */
स्थिर पूर्णांक
write_random_numbers (संरचना randint_source *s, माप_प्रकार मोजणी,
                      माप_प्रकार lo_आदान, माप_प्रकार hi_आदान, अक्षर eolbyte)
{
  स्थिरांक randint range = hi_आदान - lo_आदान + 1;

  साठी (माप_प्रकार i = 0; i < मोजणी; i++)
    {
      अचिन्हित दीर्घ पूर्णांक j = lo_आदान + randint_choose (s, range);
      जर (! print_संख्या (j, eolbyte))
        परत -1;
    }

  परत 0;
}

/* Output COUNT lines to stdout from LINES array.
   LINES must have at least N_LINES elements in it.
   Strings in LINES_ must include the line-terminator character.  */
स्थिर पूर्णांक
write_random_ओळी (संरचना randint_source *s, माप_प्रकार मोजणी,
                    अक्षर *स्थिरांक *ओळी, माप_प्रकार n_ओळी)
{
  साठी (माप_प्रकार i = 0; i < मोजणी; i++)
    {
      स्थिरांक randint j = randint_choose (s, n_ओळी);
      अक्षर *स्थिरांक *p = ओळी + j;
      माप_प्रकार len = p[1] - p[0];
      जर (ख_डालो (p[0], आकार *p[0], len, मानक_निकास) != len)
        परत -1;
    }

  परत 0;
}

पूर्णांक
main (पूर्णांक argc, अक्षर **argv)
{
  bool echo = false;
  bool आदान_range = false;
  माप_प्रकार lo_आदान = SIZE_MAX;
  माप_प्रकार hi_आदान = 0;
  idx_t head_ओळी = MIN (IDX_MAX, SIZE_MAX);
  अक्षर स्थिरांक *outfile = रिक्त;
  अक्षर *random_source = रिक्त;
  अक्षर eolbyte = '\n';
  अक्षर **आदान_ओळी = रिक्त;
  bool use_reservoir_sampling = false;
  bool repeat = false;

  पूर्णांक optc;
  पूर्णांक n_operands;
  अक्षर **operand;
  माप_प्रकार n_ओळी;
  अक्षर **ओळ = रिक्त;
  संरचना linebuffer *reservoir = रिक्त;
  संरचना randint_source *randint_source;
  माप_प्रकार *permutation = रिक्त;
  पूर्णांक i;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  पर्यंत ((optc = getopt_long (argc, argv, "ei:n:o:rz", long_opts, रिक्त))
         != -1)
    निवड (optc)
      {
      स्थिती 'e':
        echo = true;
        थांबा;

      स्थिती 'i':
        {
          जर (आदान_range)
            error (निकास_त्रुटि, 0, _("multiple -i options specified"));
          आदान_range = true;

          uintmax_t u;
          अक्षर *lo_end;
          strtol_error err = xstrtoumax (optarg, &lo_end, 10, &u, रिक्त);
          जर (err == LONGINT_OK)
            {
              lo_आदान = u;
              जर (lo_आदान != u)
                err = LONGINT_OVERFLOW;
              अन्यथा जर (*lo_end != '-')
                err = LONGINT_INVALID;
              अन्यथा
                {
                  err = xstrtoumax (lo_end + 1, रिक्त, 10, &u, "");
                  जर (err == LONGINT_OK)
                    {
                      hi_आदान = u;
                      जर (hi_आदान != u)
                        err = LONGINT_OVERFLOW;
                    }
                }
            }

          n_ओळी = hi_आदान - lo_आदान + 1;

          जर (err != LONGINT_OK || (lo_आदान <= hi_आदान) == (n_ओळी == 0))
            error (निकास_त्रुटि, err == LONGINT_OVERFLOW ? EOVERFLOW : 0,
                   "%s: %s", _("invalid input range"), quote (optarg));
        }
        थांबा;

      स्थिती 'n':
        {
          uintmax_t argval;
          strtol_error e = xstrtoumax (optarg, रिक्त, 10, &argval, "");

          जर (e == LONGINT_OK)
            head_ओळी = MIN (head_ओळी, argval);
          अन्यथा जर (e != LONGINT_OVERFLOW)
            error (निकास_त्रुटि, 0, _("invalid line count: %s"),
                   quote (optarg));
        }
        थांबा;

      स्थिती 'o':
        जर (outfile && !streq (outfile, optarg))
          error (निकास_त्रुटि, 0, _("multiple output files specified"));
        outfile = optarg;
        थांबा;

      स्थिती RANDOM_SOURCE_OPTION:
        जर (random_source && !streq (random_source, optarg))
          error (निकास_त्रुटि, 0, _("multiple random sources specified"));
        random_source = optarg;
        थांबा;

      स्थिती 'r':
        repeat = true;
        थांबा;

      स्थिती 'z':
        eolbyte = '\0';
        थांबा;

      case_GETOPT_HELP_CHAR;
      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
      पूर्वनिर्धारित:
        usage (निकास_त्रुटि);
      }

  n_operands = argc - optind;
  operand = argv + optind;

  /* Check invalid usage.  */
  जर (echo && आदान_range)
    {
      error (0, 0, _("cannot combine -e and -i options"));
      usage (निकास_त्रुटि);
    }
  जर (आदान_range ? 0 < n_operands : !echo && 1 < n_operands)
    {
      error (0, 0, _("extra operand %s"), quote (operand[!आदान_range]));
      usage (निकास_त्रुटि);
    }

  /* Prepare input.  */
  जर (head_ओळी == 0)
    {
      n_ओळी = 0;
      ओळ = रिक्त;
    }
  अन्यथा जर (echo)
    {
      आदान_from_argv (operand, n_operands, eolbyte);
      n_ओळी = n_operands;
      ओळ = operand;
    }
  अन्यथा जर (आदान_range)
    {
      IF_LINT (n_ओळी = hi_आदान - lo_आदान + 1); /* Avoid GCC 10 warning.  */
      ओळ = रिक्त;
    }
  अन्यथा
    {
      /* If an input file is specified, re-open it as stdin.  */
      जर (n_operands == 1
          && ! (streq (operand[0], "-")
                || ख_व_खोलो (operand[0], "r", मानक_निवेश)))
        error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (operand[0]));

      fadvise (मानक_निवेश, FADVISE_SEQUENTIAL);

      जर (repeat || head_ओळी == MIN (IDX_MAX, SIZE_MAX)
          || आदान_size () <= RESERVOIR_MIN_आदान)
        {
          n_ओळी = read_आदान (मानक_निवेश, eolbyte, &आदान_ओळी);
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
  जर (! randint_source)
    error (निकास_त्रुटि, त्रुटि_सं, "%s",
           quotef (random_source ? random_source : "getrandom"));

  जर (use_reservoir_sampling)
    {
      /* Instead of reading the entire file into 'line',
         use reservoir-sampling to store just AHEAD_LINES random lines.  */
      n_ओळी = read_आदान_reservoir_sampling (मानक_निवेश, eolbyte, ahead_ओळी,
                                               randint_source, &reservoir);
      ahead_ओळी = n_ओळी;
    }

  /* Close stdin now, rather than earlier, so that randint_all_new
     doesn't have to worry about opening something other than
     stdin.  */
  जर (! (head_ओळी == 0 || echo || आदान_range || ख_बंद (मानक_निवेश) == 0))
    error (निकास_त्रुटि, त्रुटि_सं, _("read error"));

  जर (!repeat)
    permutation = randperm_new (randint_source, ahead_ओळी, n_ओळी);

  जर (outfile && ! ख_व_खोलो (outfile, "w", मानक_निकास))
    error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (outfile));

  /* Generate output according to requested method */
  जर (repeat)
    {
      जर (head_ओळी == 0)
        i = 0;
      अन्यथा
        {
          जर (n_ओळी == 0)
            error (निकास_त्रुटि, 0, _("no lines to repeat"));
          जर (आदान_range)
            i = write_random_numbers (randint_source, ahead_ओळी,
                                      lo_आदान, hi_आदान, eolbyte);
          अन्यथा
            i = write_random_ओळी (randint_source, ahead_ओळी, ओळ, n_ओळी);
        }
    }
  अन्यथा
    {
      जर (use_reservoir_sampling)
        i = write_permuted_output_reservoir (n_ओळी, reservoir, permutation);
      अन्यथा जर (आदान_range)
        i = write_permuted_numbers (ahead_ओळी, lo_आदान,
                                    permutation, eolbyte);
      अन्यथा
        i = write_permuted_ओळी (ahead_ओळी, ओळ, permutation);
    }

  जर (i != 0)
    write_error ();

  IF_LINT (randint_all_free (randint_source));  /* For older valgrind.  */

  main_exit (निकास_सफल);
}
