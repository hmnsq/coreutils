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

#समावेश <config.h>

#समावेश <sys/types.h>
#समावेश <getopt.h>
#समावेश "system.h"

#समावेश "fadvise.h"
#समावेश "linebuffer.h"
#समावेश "quote.h"
#समावेश "randint.h"
#समावेश "randperm.h"
#समावेश "read-file.h"
#समावेश "stdio--.h"
#समावेश "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "shuf"

#घोषणा AUTHORS proper_name ("Paul Eggert")

/* reservoir-sampling introduces CPU overhead for small inputs.
   So only enable it for inputs >= this limit.
   This limit was determined using these commands:
     $ for p in $(seq 7); do src/seq $((10**$p)) > 10p$p.in; done
     $ for p in $(seq 7); do time shuf-nores -n10 10p$p.in >/dev/null; done
     $ for p in $(seq 7); do time shuf -n10 10p$p.in >/dev/null; done  .*/
क्रमागत { RESERVOIR_MIN_निवेश = 8192 * 1024 };


व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
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
  -n, --शीर्ष-गिनती=गिनती_2    output at most गिनती_2 पंक्तियाँ\n\
"));
      oputs (_("\
  -o, --output=खाता         ग_लिखो परिणाम to खाता instead of standard output\n\
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
क्रमागत
{
  RANDOM_SOURCE_OPTION = अक्षर_उच्च + 1
};

अटल काष्ठा option स्थिर long_opts[] =
{
  {"echo", no_argument, शून्य, 'e'},
  {"input-range", required_argument, शून्य, 'i'},
  {"head-count", required_argument, शून्य, 'n'},
  {"output", required_argument, शून्य, 'o'},
  {"random-source", required_argument, शून्य, RANDOM_SOURCE_OPTION},
  {"repeat", no_argument, शून्य, 'r'},
  {"zero-terminated", no_argument, शून्य, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {शून्य, 0, शून्य, 0},
};

अटल व्योम
निवेश_from_argv (अक्षर **operand, पूर्णांक n_operands, अक्षर eolbyte)
{
  अक्षर *p;
  माप_प्रकार आकार = n_operands;
  पूर्णांक i;

  क्रम (i = 0; i < n_operands; i++)
    आकार += म_माप (operand[i]);
  p = xmalloc (आकार);

  क्रम (i = 0; i < n_operands; i++)
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

अटल अक्षर *
अग्र_पंक्ति (अक्षर *पंक्ति, अक्षर eolbyte)
{
  अक्षर *p = rawmemchr (पंक्ति, eolbyte);
  वापस p + 1;
}

/* Return the size of the input if possible or OFF_T_MAX if not.  */

अटल off_t
निवेश_आकार (व्योम)
{
  off_t file_आकार;

  काष्ठा stat stat_buf;
  अगर (ख_स्थिति (STDIN_FILENO, &stat_buf) != 0)
    वापस OFF_T_MAX;
  अगर (usable_st_आकार (&stat_buf))
    file_आकार = stat_buf.st_आकार;
  अन्यथा
    वापस OFF_T_MAX;

  off_t निवेश_offset = lseek (STDIN_FILENO, 0, प्रस्तुत_से);
  अगर (निवेश_offset < 0)
    वापस OFF_T_MAX;

  file_आकार -= निवेश_offset;

  वापस file_आकार;
}

/* Read all lines and store up to K permuted lines in *OUT_RSRV.
   Return the number of lines read, up to a maximum of K.  */

अटल idx_t
read_निवेश_reservoir_sampling (खाता *in, अक्षर eolbyte, idx_t k,
                               काष्ठा randint_source *s,
                               काष्ठा linebuffer **out_rsrv)
{
  randint n_पंक्तियाँ = 0;
  idx_t n_alloc_पंक्तियाँ = 0;
  काष्ठा linebuffer *पंक्ति = शून्य;
  काष्ठा linebuffer *rsrv = शून्य;

  /* Fill the first K lines, directly into the reservoir.  */
  क्रम (n_पंक्तियाँ = 0; n_पंक्तियाँ < k; n_पंक्तियाँ++)
    {
      /* Enlarge reservoir if needed.  */
      अगर (n_पंक्तियाँ == n_alloc_पंक्तियाँ)
        {
          idx_t old = n_alloc_पंक्तियाँ;
          rsrv = xpalloc (rsrv, &n_alloc_पंक्तियाँ, 1, k, माप *rsrv);
          स_रखो (&rsrv[n_पंक्तियाँ], 0, (n_alloc_पंक्तियाँ - old) * माप *rsrv);
        }

      पंक्ति = readlinebuffer_delim (&rsrv[n_पंक्तियाँ], in, eolbyte);
      अगर (!पंक्ति)
        अवरोध;
    }

  /* last line wasn't null - so there may be more lines to read.  */
  अगर (पंक्ति != शून्य)
    {
      काष्ठा linebuffer dummy;
      initbuffer (&dummy);  /* space for lines not put in reservoir.  */

      /* Choose the fate of the next line, with decreasing probability (as
         n_lines increases in size).

         If the line will be used, store it directly in the reservoir.
         Otherwise, store it in dummy space.

         With 'struct linebuffer', storing into existing buffer will reduce
         re-allocations (will only re-allocate if the new line is longer than
         the currently allocated space).  */
      करो
        {
          randint j = randint_choose (s, n_पंक्तियाँ + 1);  /* 0 .. n_lines.  */
          पंक्ति = (j < k) ? (&rsrv[j]) : (&dummy);
        }
      जबतक (readlinebuffer_delim (पंक्ति, in, eolbyte) != शून्य && n_पंक्तियाँ++);

      अगर (! n_पंक्तियाँ)
        error (निकास_त्रुटि, EOVERFLOW, _("too many input lines"));

      freebuffer (&dummy);
    }

  /* no more input lines, or an input error.  */
  अगर (ख_त्रुटि (in))
    error (निकास_त्रुटि, त्रुटि_सं, _("read error"));

  *out_rsrv = rsrv;
  वापस MIN (k, n_पंक्तियाँ);
}

अटल पूर्णांक
write_permuted_output_reservoir (माप_प्रकार n_पंक्तियाँ, काष्ठा linebuffer *पंक्तियाँ,
                                 माप_प्रकार स्थिर *permutation)
{
  क्रम (माप_प्रकार i = 0; i < n_पंक्तियाँ; i++)
    {
      स्थिर काष्ठा linebuffer *p = &पंक्तियाँ[permutation[i]];
      अगर (ख_डालो (p->बफर, माप (अक्षर), p->लंबाई, मानक_निकास) != p->लंबाई)
        वापस -1;
    }

  वापस 0;
}

/* Read data from file IN.  Input lines are delimited by EOLBYTE;
   silently append a trailing EOLBYTE if the file ends in some other
   byte.  Store a pointer to the resulting array of lines into *PLINE.
   Return the number of lines read.  Report an error and exit on
   failure.  */

अटल माप_प्रकार
read_निवेश (खाता *in, अक्षर eolbyte, अक्षर ***pline)
{
  अक्षर *p;
  अक्षर *buf = शून्य;
  माप_प्रकार used;
  अक्षर *lim;
  अक्षर **पंक्ति;
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
  अगर (!(buf = fread_file (in, 0, &used)))
    error (निकास_त्रुटि, त्रुटि_सं, _("read error"));

  अगर (used && buf[used - 1] != eolbyte)
    buf[used++] = eolbyte;

  lim = buf + used;

  n_पंक्तियाँ = 0;
  क्रम (p = buf; p < lim; p = अग्र_पंक्ति (p, eolbyte))
    n_पंक्तियाँ++;

  *pline = पंक्ति = xnmalloc (n_पंक्तियाँ + 1, माप *पंक्ति);

  पंक्ति[0] = p = buf;
  क्रम (माप_प्रकार i = 1; i <= n_पंक्तियाँ; i++)
    पंक्ति[i] = p = अग्र_पंक्ति (p, eolbyte);

  वापस n_पंक्तियाँ;
}

/* Output N_LINES lines to stdout from LINE array,
   chosen by the indices in PERMUTATION.
   PERMUTATION and LINE must have at least N_LINES elements.
   Strings in LINE must include the line-terminator character.  */
अटल पूर्णांक
write_permuted_पंक्तियाँ (माप_प्रकार n_पंक्तियाँ, अक्षर *स्थिर *पंक्ति,
                      माप_प्रकार स्थिर *permutation)
{
  क्रम (माप_प्रकार i = 0; i < n_पंक्तियाँ; i++)
    {
      अक्षर *स्थिर *p = पंक्ति + permutation[i];
      माप_प्रकार len = p[1] - p[0];
      अगर (ख_डालो (p[0], माप *p[0], len, मानक_निकास) != len)
        वापस -1;
    }

  वापस 0;
}

/* Print NUMBER followed by EOLBYTE to standard output.
   Return false on failure, true on success.  */
अटल bool
print_संख्या (अचिन्हित दीर्घ पूर्णांक संख्या, अक्षर eolbyte)
{
  अक्षर buf[INT_BUFSIZE_BOUND (अचिन्हित दीर्घ पूर्णांक)];
  अक्षर *p = buf + INT_STRLEN_BOUND (अचिन्हित दीर्घ पूर्णांक);
  *p = eolbyte;
  करो
    *--p = '0' + संख्या % 10;
  जबतक ((संख्या /= 10) != 0);
  idx_t len = buf + माप buf - p;
  वापस ख_डालो (p, 1, len, मानक_निकास) == len;
}

/* Output N_LINES of numbers to stdout, from PERMUTATION array.
   PERMUTATION must have at least N_LINES elements.  */
अटल पूर्णांक
write_permuted_numbers (माप_प्रकार n_पंक्तियाँ, माप_प्रकार lo_निवेश,
                        माप_प्रकार स्थिर *permutation, अक्षर eolbyte)
{
  क्रम (माप_प्रकार i = 0; i < n_पंक्तियाँ; i++)
    {
      अचिन्हित दीर्घ पूर्णांक n = lo_निवेश + permutation[i];
      अगर (! print_संख्या (n, eolbyte))
        वापस -1;
    }

  वापस 0;
}

/* Output COUNT numbers to stdout, chosen randomly from range
   LO_INPUT through HI_INPUT.  */
अटल पूर्णांक
write_random_numbers (काष्ठा randint_source *s, माप_प्रकार गिनती,
                      माप_प्रकार lo_निवेश, माप_प्रकार hi_निवेश, अक्षर eolbyte)
{
  स्थिर randint range = hi_निवेश - lo_निवेश + 1;

  क्रम (माप_प्रकार i = 0; i < गिनती; i++)
    {
      अचिन्हित दीर्घ पूर्णांक j = lo_निवेश + randint_choose (s, range);
      अगर (! print_संख्या (j, eolbyte))
        वापस -1;
    }

  वापस 0;
}

/* Output COUNT lines to stdout from LINES array.
   LINES must have at least N_LINES elements in it.
   Strings in LINES_ must include the line-terminator character.  */
अटल पूर्णांक
write_random_पंक्तियाँ (काष्ठा randint_source *s, माप_प्रकार गिनती,
                    अक्षर *स्थिर *पंक्तियाँ, माप_प्रकार n_पंक्तियाँ)
{
  क्रम (माप_प्रकार i = 0; i < गिनती; i++)
    {
      स्थिर randint j = randint_choose (s, n_पंक्तियाँ);
      अक्षर *स्थिर *p = पंक्तियाँ + j;
      माप_प्रकार len = p[1] - p[0];
      अगर (ख_डालो (p[0], माप *p[0], len, मानक_निकास) != len)
        वापस -1;
    }

  वापस 0;
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  bool echo = false;
  bool निवेश_range = false;
  माप_प्रकार lo_निवेश = SIZE_MAX;
  माप_प्रकार hi_निवेश = 0;
  idx_t शीर्ष_पंक्तियाँ = MIN (IDX_MAX, SIZE_MAX);
  अक्षर स्थिर *outfile = शून्य;
  अक्षर *random_source = शून्य;
  अक्षर eolbyte = '\n';
  अक्षर **निवेश_पंक्तियाँ = शून्य;
  bool use_reservoir_sampling = false;
  bool repeat = false;

  पूर्णांक optc;
  पूर्णांक n_operands;
  अक्षर **operand;
  माप_प्रकार n_पंक्तियाँ;
  अक्षर **पंक्ति = शून्य;
  काष्ठा linebuffer *reservoir = शून्य;
  काष्ठा randint_source *randint_source;
  माप_प्रकार *permutation = शून्य;
  पूर्णांक i;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  जबतक ((optc = getopt_long (argc, argv, "ei:n:o:rz", long_opts, शून्य))
         != -1)
    चयन (optc)
      {
      हाल 'e':
        echo = true;
        अवरोध;

      हाल 'i':
        {
          अगर (निवेश_range)
            error (निकास_त्रुटि, 0, _("multiple -i options specified"));
          निवेश_range = true;

          uintmax_t u;
          अक्षर *lo_end;
          strtol_error err = xstrtoumax (optarg, &lo_end, 10, &u, शून्य);
          अगर (err == LONGINT_OK)
            {
              lo_निवेश = u;
              अगर (lo_निवेश != u)
                err = LONGINT_OVERFLOW;
              अन्यथा अगर (*lo_end != '-')
                err = LONGINT_INVALID;
              अन्यथा
                {
                  err = xstrtoumax (lo_end + 1, शून्य, 10, &u, "");
                  अगर (err == LONGINT_OK)
                    {
                      hi_निवेश = u;
                      अगर (hi_निवेश != u)
                        err = LONGINT_OVERFLOW;
                    }
                }
            }

          n_पंक्तियाँ = hi_निवेश - lo_निवेश + 1;

          अगर (err != LONGINT_OK || (lo_निवेश <= hi_निवेश) == (n_पंक्तियाँ == 0))
            error (निकास_त्रुटि, err == LONGINT_OVERFLOW ? EOVERFLOW : 0,
                   "%s: %s", _("invalid input range"), quote (optarg));
        }
        अवरोध;

      हाल 'n':
        {
          uintmax_t argval;
          strtol_error e = xstrtoumax (optarg, शून्य, 10, &argval, "");

          अगर (e == LONGINT_OK)
            शीर्ष_पंक्तियाँ = MIN (शीर्ष_पंक्तियाँ, argval);
          अन्यथा अगर (e != LONGINT_OVERFLOW)
            error (निकास_त्रुटि, 0, _("invalid line count: %s"),
                   quote (optarg));
        }
        अवरोध;

      हाल 'o':
        अगर (outfile && !streq (outfile, optarg))
          error (निकास_त्रुटि, 0, _("multiple output files specified"));
        outfile = optarg;
        अवरोध;

      हाल RANDOM_SOURCE_OPTION:
        अगर (random_source && !streq (random_source, optarg))
          error (निकास_त्रुटि, 0, _("multiple random sources specified"));
        random_source = optarg;
        अवरोध;

      हाल 'r':
        repeat = true;
        अवरोध;

      हाल 'z':
        eolbyte = '\0';
        अवरोध;

      case_GETOPT_HELP_CHAR;
      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
      शेष:
        usage (निकास_त्रुटि);
      }

  n_operands = argc - optind;
  operand = argv + optind;

  /* Check invalid usage.  */
  अगर (echo && निवेश_range)
    {
      error (0, 0, _("cannot combine -e and -i options"));
      usage (निकास_त्रुटि);
    }
  अगर (निवेश_range ? 0 < n_operands : !echo && 1 < n_operands)
    {
      error (0, 0, _("extra operand %s"), quote (operand[!निवेश_range]));
      usage (निकास_त्रुटि);
    }

  /* Prepare input.  */
  अगर (शीर्ष_पंक्तियाँ == 0)
    {
      n_पंक्तियाँ = 0;
      पंक्ति = शून्य;
    }
  अन्यथा अगर (echo)
    {
      निवेश_from_argv (operand, n_operands, eolbyte);
      n_पंक्तियाँ = n_operands;
      पंक्ति = operand;
    }
  अन्यथा अगर (निवेश_range)
    {
      IF_LINT (n_पंक्तियाँ = hi_निवेश - lo_निवेश + 1); /* Avoid GCC 10 warning.  */
      पंक्ति = शून्य;
    }
  अन्यथा
    {
      /* If an input file is specified, re-open it as stdin.  */
      अगर (n_operands == 1
          && ! (streq (operand[0], "-")
                || ख_व_खोलो (operand[0], "r", मानक_निवेश)))
        error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (operand[0]));

      fadvise (मानक_निवेश, FADVISE_SEQUENTIAL);

      अगर (repeat || शीर्ष_पंक्तियाँ == MIN (IDX_MAX, SIZE_MAX)
          || निवेश_आकार () <= RESERVOIR_MIN_निवेश)
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
  idx_t ahead_पंक्तियाँ = repeat || शीर्ष_पंक्तियाँ < n_पंक्तियाँ ? शीर्ष_पंक्तियाँ : n_पंक्तियाँ;

  randint_source = randint_all_new (random_source,
                                    (use_reservoir_sampling || repeat
                                     ? SIZE_MAX
                                     : randperm_bound (ahead_पंक्तियाँ, n_पंक्तियाँ)));
  अगर (! randint_source)
    error (निकास_त्रुटि, त्रुटि_सं, "%s",
           quotef (random_source ? random_source : "getrandom"));

  अगर (use_reservoir_sampling)
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
  अगर (! (शीर्ष_पंक्तियाँ == 0 || echo || निवेश_range || ख_बंद (मानक_निवेश) == 0))
    error (निकास_त्रुटि, त्रुटि_सं, _("read error"));

  अगर (!repeat)
    permutation = randperm_new (randint_source, ahead_पंक्तियाँ, n_पंक्तियाँ);

  अगर (outfile && ! ख_व_खोलो (outfile, "w", मानक_निकास))
    error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (outfile));

  /* Generate output according to requested method */
  अगर (repeat)
    {
      अगर (शीर्ष_पंक्तियाँ == 0)
        i = 0;
      अन्यथा
        {
          अगर (n_पंक्तियाँ == 0)
            error (निकास_त्रुटि, 0, _("no lines to repeat"));
          अगर (निवेश_range)
            i = write_random_numbers (randint_source, ahead_पंक्तियाँ,
                                      lo_निवेश, hi_निवेश, eolbyte);
          अन्यथा
            i = write_random_पंक्तियाँ (randint_source, ahead_पंक्तियाँ, पंक्ति, n_पंक्तियाँ);
        }
    }
  अन्यथा
    {
      अगर (use_reservoir_sampling)
        i = write_permuted_output_reservoir (n_पंक्तियाँ, reservoir, permutation);
      अन्यथा अगर (निवेश_range)
        i = write_permuted_numbers (ahead_पंक्तियाँ, lo_निवेश,
                                    permutation, eolbyte);
      अन्यथा
        i = write_permuted_पंक्तियाँ (ahead_पंक्तियाँ, पंक्ति, permutation);
    }

  अगर (i != 0)
    write_error ();

  IF_LINT (randint_all_free (randint_source));  /* For older valgrind.  */

  main_exit (निकास_सफल);
}
