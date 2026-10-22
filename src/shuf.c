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
#परिभाषय क्रमादेशः_NAME "shuf"

#परिभाषय AUTHORS proper_name ("Paul Eggert")

/* reservoir-sampling introduces CPU overhead for small inputs.
   So only enable it for inputs >= this limit.
   This limit was determined using these commands:
     $ for p in $(seq 7); do src/seq $((10**$p)) > 10p$p.in; done
     $ for p in $(seq 7); do time shuf-nores -n10 10p$p.in >/dev/null; done
     $ for p in $(seq 7); do time shuf -n10 10p$p.in >/dev/null; done  .*/
गणना { RESERVOIR_MIN_निवेशः = 8192 * 1024 };


शून्य
usage (पूर्णाङ्क स्थितिः)
{
  यदि (स्थितिः != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
    {
      प्रारूप_लिख (_("\
Usage: %s [OPTION]... [FILE]\n\
  or:  %s -e [OPTION]... [ARG]...\n\
  or:  %s -i LO-HI [OPTION]...\n\
"),
              क्रमादेशः_name, क्रमादेशः_name, क्रमादेशः_name);
      fputs (_("\
Write a random permutation सम्बन्धि the निवेशः पङ्क्तयः गन्तव्यम् standard output.\n\
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
  -n, --शीर्ष-गणनम्=गणनम्_2    output at most गणनम्_2 पङ्क्तयः\n\
"));
      oputs (_("\
  -o, --output=FILE         लिख परिणाम गन्तव्यम् FILE instead सम्बन्धि standard output\n\
"));
      oputs (_("\
      --random-source=FILE  get random अष्टकानि from FILE\n\
"));
      oputs (_("\
  -r, --repeat              output पङ्क्तयः can be repeated\n\
"));
      oputs (_("\
  -z, --zero-terminated     पङ्क्तिः delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (संस्करणम्_OPTION_DESCRIPTION);
      emit_ancillary_info (क्रमादेशः_NAME);
    }

  निर्गच्छ (स्थितिः);
}

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
गणना
{
  RANDOM_SOURCE_OPTION = CHAR_MAX + 1
};

स्थिर संरचना option ध्रुव long_opts[] =
{
  {"echo", no_तर्कः, रिक्त, 'e'},
  {"input-range", required_तर्कः, रिक्त, 'i'},
  {"head-count", required_तर्कः, रिक्त, 'n'},
  {"output", required_तर्कः, रिक्त, 'o'},
  {"random-source", required_तर्कः, रिक्त, RANDOM_SOURCE_OPTION},
  {"repeat", no_तर्कः, रिक्त, 'r'},
  {"zero-terminated", no_तर्कः, रिक्त, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_संस्करणम्_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0},
};

स्थिर शून्य
निवेशः_from_argv (वर्ण **सङ्कार्यम्_2, पूर्णाङ्क n_operands, वर्ण eolbyte)
{
  वर्ण *p;
  size_t आकार = n_operands;
  पूर्णाङ्क i;

  प्रति (i = 0; i < n_operands; i++)
    आकार += वाक्यदैर्घ्यम् (सङ्कार्यम्_2[i]);
  p = xmalloc (आकार);

  प्रति (i = 0; i < n_operands; i++)
    {
      वर्ण *p1 = stpcpy (p, सङ्कार्यम्_2[i]);
      सङ्कार्यम्_2[i] = p;
      p = p1;
      *p++ = eolbyte;
    }

  सङ्कार्यम्_2[n_operands] = p;
}

/* Return the start of the next line after LINE, which is guaranteed
   to end in EOLBYTE.  */

स्थिर वर्ण *
अग्र_पङ्क्तिः (वर्ण *पङ्क्तिः, वर्ण eolbyte)
{
  वर्ण *p = rawmemchr (पङ्क्तिः, eolbyte);
  प्रत्यागम p + 1;
}

/* Return the size of the input if possible or OFF_T_MAX if not.  */

स्थिर off_t
निवेशः_आकार (शून्य)
{
  off_t file_आकार;

  संरचना stat stat_buf;
  यदि (fstat (STDIN_FILENO, &stat_buf) != 0)
    प्रत्यागम OFF_T_MAX;
  यदि (usable_st_आकार (&stat_buf))
    file_आकार = stat_buf.st_आकार;
  अन्यथा
    प्रत्यागम OFF_T_MAX;

  off_t निवेशः_विस्थापनम् = lseek (STDIN_FILENO, 0, SEEK_CUR);
  यदि (निवेशः_विस्थापनम् < 0)
    प्रत्यागम OFF_T_MAX;

  file_आकार -= निवेशः_विस्थापनम्;

  प्रत्यागम file_आकार;
}

/* Read all lines and store up to K permuted lines in *OUT_RSRV.
   Return the number of lines read, up to a maximum of K.  */

स्थिर idx_t
read_निवेशः_reservoir_sampling (FILE *अन्तर्, वर्ण eolbyte, idx_t k,
                               संरचना randint_source *s,
                               संरचना linebuffer **बहिर्गमनम्_rsrv)
{
  randint n_पङ्क्तयः = 0;
  idx_t n_alloc_पङ्क्तयः = 0;
  संरचना linebuffer *पङ्क्तिः = रिक्त;
  संरचना linebuffer *rsrv = रिक्त;

  /* Fill the first K lines, directly into the reservoir.  */
  प्रति (n_पङ्क्तयः = 0; n_पङ्क्तयः < k; n_पङ्क्तयः++)
    {
      /* Enlarge reservoir if needed.  */
      यदि (n_पङ्क्तयः == n_alloc_पङ्क्तयः)
        {
          idx_t old = n_alloc_पङ्क्तयः;
          rsrv = xpalloc (rsrv, &n_alloc_पङ्क्तयः, 1, k, परिमाण *rsrv);
          स्मृतिं_पूरय (&rsrv[n_पङ्क्तयः], 0, (n_alloc_पङ्क्तयः - old) * परिमाण *rsrv);
        }

      पङ्क्तिः = readlinebuffer_delim (&rsrv[n_पङ्क्तयः], अन्तर्, eolbyte);
      यदि (!पङ्क्तिः)
        भङ्ग;
    }

  /* last line wasn't null - so there may be more lines to read.  */
  यदि (पङ्क्तिः != रिक्त)
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
          randint j = randint_वृणु (s, n_पङ्क्तयः + 1);  /* 0 .. n_lines.  */
          पङ्क्तिः = (j < k) ? (&rsrv[j]) : (&dummy);
        }
      यावत् (readlinebuffer_delim (पङ्क्तिः, अन्तर्, eolbyte) != रिक्त && n_पङ्क्तयः++);

      यदि (! n_पङ्क्तयः)
        error (EXIT_FAILURE, EOVERFLOW, _("too many input lines"));

      freebuffer (&dummy);
    }

  /* no more input lines, or an input error.  */
  यदि (ferror (अन्तर्))
    error (EXIT_FAILURE, errno, _("read error"));

  *बहिर्गमनम्_rsrv = rsrv;
  प्रत्यागम MIN (k, n_पङ्क्तयः);
}

स्थिर पूर्णाङ्क
write_permuted_output_reservoir (size_t n_पङ्क्तयः, संरचना linebuffer *पङ्क्तयः,
                                 size_t ध्रुव *permutation)
{
  प्रति (size_t i = 0; i < n_पङ्क्तयः; i++)
    {
      ध्रुव संरचना linebuffer *p = &पङ्क्तयः[permutation[i]];
      यदि (fwrite (p->बफर, परिमाण (वर्ण), p->दीर्घता, stdout) != p->दीर्घता)
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
read_निवेशः (FILE *अन्तर्, वर्ण eolbyte, वर्ण ***pline)
{
  वर्ण *p;
  वर्ण *buf = रिक्त;
  size_t प्रयुक्तम्;
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
  यदि (!(buf = fread_file (अन्तर्, 0, &प्रयुक्तम्)))
    error (EXIT_FAILURE, errno, _("read error"));

  यदि (प्रयुक्तम् && buf[प्रयुक्तम् - 1] != eolbyte)
    buf[प्रयुक्तम्++] = eolbyte;

  lim = buf + प्रयुक्तम्;

  n_पङ्क्तयः = 0;
  प्रति (p = buf; p < lim; p = अग्र_पङ्क्तिः (p, eolbyte))
    n_पङ्क्तयः++;

  *pline = पङ्क्तिः = xnmalloc (n_पङ्क्तयः + 1, परिमाण *पङ्क्तिः);

  पङ्क्तिः[0] = p = buf;
  प्रति (size_t i = 1; i <= n_पङ्क्तयः; i++)
    पङ्क्तिः[i] = p = अग्र_पङ्क्तिः (p, eolbyte);

  प्रत्यागम n_पङ्क्तयः;
}

/* Output N_LINES lines to stdout from LINE array,
   chosen by the indices in PERMUTATION.
   PERMUTATION and LINE must have at least N_LINES elements.
   Strings in LINE must include the line-terminator character.  */
स्थिर पूर्णाङ्क
write_permuted_पङ्क्तयः (size_t n_पङ्क्तयः, वर्ण *ध्रुव *पङ्क्तिः,
                      size_t ध्रुव *permutation)
{
  प्रति (size_t i = 0; i < n_पङ्क्तयः; i++)
    {
      वर्ण *ध्रुव *p = पङ्क्तिः + permutation[i];
      size_t len = p[1] - p[0];
      यदि (fwrite (p[0], परिमाण *p[0], len, stdout) != len)
        प्रत्यागम -1;
    }

  प्रत्यागम 0;
}

/* Print NUMBER followed by EOLBYTE to standard output.
   Return false on failure, true on success.  */
स्थिर bool
print_सङ्ख्या (अचिह्नित दीर्घ पूर्णाङ्क सङ्ख्या, वर्ण eolbyte)
{
  वर्ण buf[INT_BUFSIZE_BOUND (अचिह्नित दीर्घ पूर्णाङ्क)];
  वर्ण *p = buf + INT_STRLEN_BOUND (अचिह्नित दीर्घ पूर्णाङ्क);
  *p = eolbyte;
  कुरु
    *--p = '0' + सङ्ख्या % 10;
  यावत् ((सङ्ख्या /= 10) != 0);
  idx_t len = buf + परिमाण buf - p;
  प्रत्यागम fwrite (p, 1, len, stdout) == len;
}

/* Output N_LINES of numbers to stdout, from PERMUTATION array.
   PERMUTATION must have at least N_LINES elements.  */
स्थिर पूर्णाङ्क
write_permuted_numbers (size_t n_पङ्क्तयः, size_t lo_निवेशः,
                        size_t ध्रुव *permutation, वर्ण eolbyte)
{
  प्रति (size_t i = 0; i < n_पङ्क्तयः; i++)
    {
      अचिह्नित दीर्घ पूर्णाङ्क n = lo_निवेशः + permutation[i];
      यदि (! print_सङ्ख्या (n, eolbyte))
        प्रत्यागम -1;
    }

  प्रत्यागम 0;
}

/* Output COUNT numbers to stdout, chosen randomly from range
   LO_INPUT through HI_INPUT.  */
स्थिर पूर्णाङ्क
write_random_numbers (संरचना randint_source *s, size_t गणनम्,
                      size_t lo_निवेशः, size_t hi_निवेशः, वर्ण eolbyte)
{
  ध्रुव randint range = hi_निवेशः - lo_निवेशः + 1;

  प्रति (size_t i = 0; i < गणनम्; i++)
    {
      अचिह्नित दीर्घ पूर्णाङ्क j = lo_निवेशः + randint_वृणु (s, range);
      यदि (! print_सङ्ख्या (j, eolbyte))
        प्रत्यागम -1;
    }

  प्रत्यागम 0;
}

/* Output COUNT lines to stdout from LINES array.
   LINES must have at least N_LINES elements in it.
   Strings in LINES_ must include the line-terminator character.  */
स्थिर पूर्णाङ्क
write_random_पङ्क्तयः (संरचना randint_source *s, size_t गणनम्,
                    वर्ण *ध्रुव *पङ्क्तयः, size_t n_पङ्क्तयः)
{
  प्रति (size_t i = 0; i < गणनम्; i++)
    {
      ध्रुव randint j = randint_वृणु (s, n_पङ्क्तयः);
      वर्ण *ध्रुव *p = पङ्क्तयः + j;
      size_t len = p[1] - p[0];
      यदि (fwrite (p[0], परिमाण *p[0], len, stdout) != len)
        प्रत्यागम -1;
    }

  प्रत्यागम 0;
}

पूर्णाङ्क
मुख्यम् (पूर्णाङ्क argc, वर्ण **argv)
{
  bool echo = false;
  bool निवेशः_range = false;
  size_t lo_निवेशः = SIZE_MAX;
  size_t hi_निवेशः = 0;
  idx_t शीर्ष_पङ्क्तयः = MIN (IDX_MAX, SIZE_MAX);
  वर्ण ध्रुव *outfile = रिक्त;
  वर्ण *random_source = रिक्त;
  वर्ण eolbyte = '\n';
  वर्ण **निवेशः_पङ्क्तयः = रिक्त;
  bool use_reservoir_sampling = false;
  bool repeat = false;

  पूर्णाङ्क optc;
  पूर्णाङ्क n_operands;
  वर्ण **सङ्कार्यम्_2;
  size_t n_पङ्क्तयः;
  वर्ण **पङ्क्तिः = रिक्त;
  संरचना linebuffer *reservoir = रिक्त;
  संरचना randint_source *randint_source;
  size_t *permutation = रिक्त;
  पूर्णाङ्क i;

  initialize_main (&argc, &argv);
  समुच्चयः_क्रमादेशः_name (argv[0]);
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
          यदि (निवेशः_range)
            error (EXIT_FAILURE, 0, _("multiple -i options specified"));
          निवेशः_range = true;

          uintmax_t u;
          वर्ण *lo_अन्तः;
          strtol_error err = xstrtoumax (optarg, &lo_अन्तः, 10, &u, रिक्त);
          यदि (err == LONGINT_OK)
            {
              lo_निवेशः = u;
              यदि (lo_निवेशः != u)
                err = LONGINT_अतिप्रवाहः;
              अन्यथा यदि (*lo_अन्तः != '-')
                err = LONGINT_INVALID;
              अन्यथा
                {
                  err = xstrtoumax (lo_अन्तः + 1, रिक्त, 10, &u, "");
                  यदि (err == LONGINT_OK)
                    {
                      hi_निवेशः = u;
                      यदि (hi_निवेशः != u)
                        err = LONGINT_अतिप्रवाहः;
                    }
                }
            }

          n_पङ्क्तयः = hi_निवेशः - lo_निवेशः + 1;

          यदि (err != LONGINT_OK || (lo_निवेशः <= hi_निवेशः) == (n_पङ्क्तयः == 0))
            error (EXIT_FAILURE, err == LONGINT_अतिप्रवाहः ? EOVERFLOW : 0,
                   "%s: %s", _("invalid input range"), quote (optarg));
        }
        भङ्ग;

      पक्ष 'n':
        {
          uintmax_t argval;
          strtol_error e = xstrtoumax (optarg, रिक्त, 10, &argval, "");

          यदि (e == LONGINT_OK)
            शीर्ष_पङ्क्तयः = MIN (शीर्ष_पङ्क्तयः, argval);
          अन्यथा यदि (e != LONGINT_अतिप्रवाहः)
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
      case_GETOPT_संस्करणम्_CHAR (क्रमादेशः_NAME, AUTHORS);
      स्वभाव:
        usage (EXIT_FAILURE);
      }

  n_operands = argc - optind;
  सङ्कार्यम्_2 = argv + optind;

  /* Check invalid usage.  */
  यदि (echo && निवेशः_range)
    {
      error (0, 0, _("cannot combine -e and -i options"));
      usage (EXIT_FAILURE);
    }
  यदि (निवेशः_range ? 0 < n_operands : !echo && 1 < n_operands)
    {
      error (0, 0, _("extra operand %s"), quote (सङ्कार्यम्_2[!निवेशः_range]));
      usage (EXIT_FAILURE);
    }

  /* Prepare input.  */
  यदि (शीर्ष_पङ्क्तयः == 0)
    {
      n_पङ्क्तयः = 0;
      पङ्क्तिः = रिक्त;
    }
  अन्यथा यदि (echo)
    {
      निवेशः_from_argv (सङ्कार्यम्_2, n_operands, eolbyte);
      n_पङ्क्तयः = n_operands;
      पङ्क्तिः = सङ्कार्यम्_2;
    }
  अन्यथा यदि (निवेशः_range)
    {
      IF_LINT (n_पङ्क्तयः = hi_निवेशः - lo_निवेशः + 1); /* Avoid GCC 10 warning.  */
      पङ्क्तिः = रिक्त;
    }
  अन्यथा
    {
      /* If an input file is specified, re-open it as stdin.  */
      यदि (n_operands == 1
          && ! (streq (सङ्कार्यम्_2[0], "-")
                || freopen (सङ्कार्यम्_2[0], "r", stdin)))
        error (EXIT_FAILURE, errno, "%s", quotef (सङ्कार्यम्_2[0]));

      fadvise (stdin, FADVISE_SEQUENTIAL);

      यदि (repeat || शीर्ष_पङ्क्तयः == MIN (IDX_MAX, SIZE_MAX)
          || निवेशः_आकार () <= RESERVOIR_MIN_निवेशः)
        {
          n_पङ्क्तयः = read_निवेशः (stdin, eolbyte, &निवेशः_पङ्क्तयः);
          पङ्क्तिः = निवेशः_पङ्क्तयः;
        }
      अन्यथा
        {
          use_reservoir_sampling = true;
          n_पङ्क्तयः = SIZE_MAX;   /* unknown number of input lines, for now.  */
        }
    }

  /* The adjusted head line count; can be less than HEAD_LINES if the
     input is small and if not repeating.  */
  idx_t ahead_पङ्क्तयः = repeat || शीर्ष_पङ्क्तयः < n_पङ्क्तयः ? शीर्ष_पङ्क्तयः : n_पङ्क्तयः;

  randint_source = randint_all_new (random_source,
                                    (use_reservoir_sampling || repeat
                                     ? SIZE_MAX
                                     : randperm_bound (ahead_पङ्क्तयः, n_पङ्क्तयः)));
  यदि (! randint_source)
    error (EXIT_FAILURE, errno, "%s",
           quotef (random_source ? random_source : "getrandom"));

  यदि (use_reservoir_sampling)
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
  यदि (! (शीर्ष_पङ्क्तयः == 0 || echo || निवेशः_range || fclose (stdin) == 0))
    error (EXIT_FAILURE, errno, _("read error"));

  यदि (!repeat)
    permutation = randperm_new (randint_source, ahead_पङ्क्तयः, n_पङ्क्तयः);

  यदि (outfile && ! freopen (outfile, "w", stdout))
    error (EXIT_FAILURE, errno, "%s", quotef (outfile));

  /* Generate output according to requested method */
  यदि (repeat)
    {
      यदि (शीर्ष_पङ्क्तयः == 0)
        i = 0;
      अन्यथा
        {
          यदि (n_पङ्क्तयः == 0)
            error (EXIT_FAILURE, 0, _("no lines to repeat"));
          यदि (निवेशः_range)
            i = write_random_numbers (randint_source, ahead_पङ्क्तयः,
                                      lo_निवेशः, hi_निवेशः, eolbyte);
          अन्यथा
            i = write_random_पङ्क्तयः (randint_source, ahead_पङ्क्तयः, पङ्क्तिः, n_पङ्क्तयः);
        }
    }
  अन्यथा
    {
      यदि (use_reservoir_sampling)
        i = write_permuted_output_reservoir (n_पङ्क्तयः, reservoir, permutation);
      अन्यथा यदि (निवेशः_range)
        i = write_permuted_numbers (ahead_पङ्क्तयः, lo_निवेशः,
                                    permutation, eolbyte);
      अन्यथा
        i = write_permuted_पङ्क्तयः (ahead_पङ्क्तयः, पङ्क्तिः, permutation);
    }

  यदि (i != 0)
    write_error ();

  IF_LINT (randint_all_free (randint_source));  /* For older valgrind.  */

  main_exit (EXIT_SUCCESS);
}
