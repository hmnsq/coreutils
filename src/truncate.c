/* truncate -- truncate or extend the length of files.
   Copyright (C) 2008-2026 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* Written by Pádraig Brady

   This is backwards compatible with the FreeBSD utility, but is more
   flexible wrt the size specifications and the use of long options,
   to better fit the "GNU" environment.  */

#अन्तर्भाव <config.h>             /* sets _FILE_OFFSET_BITS=64 etc. */
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <getopt.h>
#अन्तर्भाव <sys/types.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "stat-size.h"
#अन्तर्भाव "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#परिभाषय क्रमादेशः_NAME "truncate"

#परिभाषय AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

/* (-c) If true, don't create if not already there */
स्थिर bool no_create;

/* (-o) If true, --size refers to blocks not bytes */
स्थिर bool खण्डम्_बहुलकम्;

/* (-r) Reference file to use size from */
स्थिर वर्ण ध्रुव *ref_file;

स्थिर संरचना option ध्रुव longopts[] =
{
  {"no-create", no_तर्कः, रिक्त, 'c'},
  {"io-blocks", no_तर्कः, रिक्त, 'o'},
  {"reference", required_तर्कः, रिक्त, 'r'},
  {"size", required_तर्कः, रिक्त, 's'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_संस्करणम्_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

प्रकारनिर्देश गणना
{ rm_abs = 0, rm_rel, rm_min, rm_max, rm_rdn, rm_rup } rel_बहुलकम्_t;

शून्य
usage (पूर्णाङ्क स्थितिः)
{
  यदि (स्थितिः != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
    {
      प्रारूप_लिख (_("Usage: %s OPTION... FILE...\n"), क्रमादेशः_name);
      fputs (_("\
Shrink or extend the आकार सम्बन्धि each FILE गन्तव्यम् the specified आकार\n\
\n\
A FILE तर्कः that does not exist is created.\n\
\n\
If a FILE is larger than the specified आकार, the extra दत्त is नष्टम्.\n\
If a FILE is shorter, it is extended and the sparse extended part (hole)\n\
reads as zero अष्टकानि.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -c, --no-create        कुरु not create any files\n\
"));
      oputs (_("\
  -o, --io-खण्डानि        treat आकार_2 as सङ्ख्या सम्बन्धि IO खण्डानि instead सम्बन्धि अष्टकानि\n\
"));
      oputs (_("\
  -r, --reference=RFILE  आधारः_2 आकार on RFILE\n\
"));
      oputs (_("\
  -s, --आकार=आकार_2        समुच्चयः_2 or adjust the file आकार by आकार_2 अष्टकानि\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (संस्करणम्_OPTION_DESCRIPTION);
      emit_आकार_note ();
      fputs (_("\n\
आकार_2 may also be prefixed by one सम्बन्धि the following modifying characters:\n\
'+' extend by, '-' reduce by, '<' at most, '>' at न्यूनतमम्,\n\
'/' आवर्तनम्_2 अधस्तात् गन्तव्यम् गुणजः सम्बन्धि, '%' आवर्तनम्_2 ऊर्ध्वम् गन्तव्यम् गुणजः सम्बन्धि.\n"), stdout);
      emit_ancillary_info (क्रमादेशः_NAME);
    }
  निर्गच्छ (स्थितिः);
}

/* return true on success, false on error.  */
स्थिर bool
do_ftruncate (पूर्णाङ्क fd, वर्ण ध्रुव *fname, off_t ssize, off_t rsize,
              rel_बहुलकम्_t rel_बहुलकम्)
{
  संरचना stat sb;
  off_t nsize;

  यदि ((खण्डम्_बहुलकम् || (rel_बहुलकम् && rsize < 0)) && fstat (fd, &sb) != 0)
    {
      error (0, errno, _("cannot fstat %s"), quoteaf (fname));
      प्रत्यागम false;
    }
  यदि (खण्डम्_बहुलकम्)
    {
      ptrdiff_t blksize = STP_BLKSIZE (&sb);
      intmax_t ssize0 = ssize;
      यदि (ckd_mul (&ssize, ssize, blksize))
        {
          error (0, 0,
                 _("overflow in %jd * %td byte blocks for file %s"),
                 ssize0, blksize, quoteaf (fname));
          प्रत्यागम false;
        }
    }
  यदि (rel_बहुलकम्)
    {
      off_t fsize;

      यदि (0 <= rsize)
        fsize = rsize;
      अन्यथा
        {
          यदि (usable_st_आकार (&sb))
            {
              fsize = sb.st_आकार;
              यदि (fsize < 0)
                {
                  /* Sanity check.  Overflow is the only reason I can think
                     this would ever go negative. */
                  error (0, 0, _("%s has unusable, apparently negative size"),
                         quoteaf (fname));
                  प्रत्यागम false;
                }
            }
          अन्यथा
            {
              fsize = lseek (fd, 0, SEEK_END);
              यदि (fsize < 0)
                {
                  error (0, errno, _("cannot get the size of %s"),
                         quoteaf (fname));
                  प्रत्यागम false;
                }
            }
        }

      यदि (rel_बहुलकम् == rm_min)
        nsize = MAX (fsize, ssize);
      अन्यथा यदि (rel_बहुलकम् == rm_max)
        nsize = MIN (fsize, ssize);
      अन्यथा यदि (rel_बहुलकम् == rm_rdn)
        /* 0..ssize-1 -> 0 */
        nsize = fsize - fsize % ssize;
      अन्यथा
        {
          यदि (rel_बहुलकम् == rm_rup)
            {
              /* 1..ssize -> ssize */
              off_t r = fsize % ssize;
              ssize = r == 0 ? 0 : ssize - r;
            }
          यदि (ckd_सङ्कलय (&nsize, fsize, ssize))
            {
              error (0, 0, _("overflow extending size of file %s"),
                     quoteaf (fname));
              प्रत्यागम false;
            }
        }
    }
  अन्यथा
    nsize = ssize;
  यदि (nsize < 0)
    nsize = 0;

  यदि (ftruncate (fd, nsize) != 0)
    {
      error (0, errno, _("failed to truncate %s at %jd bytes"),
             quoteaf (fname), (intmax_t) {nsize});
      प्रत्यागम false;
    }

  प्रत्यागम true;
}

पूर्णाङ्क
मुख्यम् (पूर्णाङ्क argc, वर्ण **argv)
{
  bool got_आकार = false;
  off_t आकार IF_LINT ( = 0);
  off_t rsize = -1;
  rel_बहुलकम्_t rel_बहुलकम् = rm_abs;
  पूर्णाङ्क c;

  initialize_main (&argc, &argv);
  समुच्चयः_क्रमादेशः_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  यावत् ((c = getopt_long (argc, argv, "cor:s:", longopts, रिक्त)) != -1)
    {
      विकल्प (c)
        {
        पक्ष 'c':
          no_create = true;
          भङ्ग;

        पक्ष 'o':
          खण्डम्_बहुलकम् = true;
          भङ्ग;

        पक्ष 'r':
          ref_file = optarg;
          भङ्ग;

        पक्ष 's':
          /* skip any whitespace */
          यावत् (रिक्तम्_वा (गन्तव्यम्_uchar (*optarg)))
            optarg++;
          विकल्प (*optarg)
            {
            पक्ष '<':
              rel_बहुलकम् = rm_max;
              optarg++;
              भङ्ग;
            पक्ष '>':
              rel_बहुलकम् = rm_min;
              optarg++;
              भङ्ग;
            पक्ष '/':
              rel_बहुलकम् = rm_rdn;
              optarg++;
              भङ्ग;
            पक्ष '%':
              rel_बहुलकम् = rm_rup;
              optarg++;
              भङ्ग;
            }
          /* skip any whitespace */
          यावत् (रिक्तम्_वा (गन्तव्यम्_uchar (*optarg)))
            optarg++;
          यदि (*optarg == '+' || *optarg == '-')
            {
              यदि (rel_बहुलकम्)
                {
                  error (0, 0, _("multiple relative modifiers specified"));
                  /* Note other combinations are flagged as invalid numbers */
                  usage (EXIT_FAILURE);
                }
              rel_बहुलकम् = rm_rel;
            }
          /* Support dd BLOCK size suffixes + lowercase g,t,m for bsd compat.
             Note we don't support dd's b=512, c=1, w=2 or 21x512MiB formats. */
          आकार = xdectoimax (optarg, OFF_T_MIN, OFF_T_MAX, "EgGkKmMPQRtTYZ0",
                             _("Invalid number"), 0);
          /* Rounding to multiple of 0 is nonsensical */
          यदि ((rel_बहुलकम् == rm_rup || rel_बहुलकम् == rm_rdn) && आकार == 0)
            error (EXIT_FAILURE, 0, _("division by zero"));
          got_आकार = true;
          भङ्ग;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_संस्करणम्_CHAR (क्रमादेशः_NAME, AUTHORS);

        स्वभाव:
          usage (EXIT_FAILURE);
        }
    }

  argv += optind;
  argc -= optind;

  /* must specify either size or reference file */
  यदि (!ref_file && !got_आकार)
    {
      error (0, 0, _("you must specify either %s or %s"),
             quote_n (0, "--size"), quote_n (1, "--reference"));
      usage (EXIT_FAILURE);
    }
  /* must specify a relative size with a reference file */
  यदि (ref_file && got_आकार && !rel_बहुलकम्)
    {
      error (0, 0, _("you must specify a relative %s with %s"),
             quote_n (0, "--size"), quote_n (1, "--reference"));
      usage (EXIT_FAILURE);
    }
  /* block_mode without size is not valid */
  यदि (खण्डम्_बहुलकम् && !got_आकार)
    {
      error (0, 0, _("%s was specified but %s was not"),
             quote_n (0, "--io-blocks"), quote_n (1, "--size"));
      usage (EXIT_FAILURE);
    }
  /* must specify at least 1 file */
  यदि (argc < 1)
    {
      error (0, 0, _("missing file operand"));
      usage (EXIT_FAILURE);
    }

  यदि (ref_file)
    {
      संरचना stat sb;
      off_t file_आकार = -1;
      यदि (stat (ref_file, &sb) != 0)
        error (EXIT_FAILURE, errno, _("cannot stat %s"), quoteaf (ref_file));
      यदि (usable_st_आकार (&sb))
        file_आकार = sb.st_आकार;
      अन्यथा
        {
          /* O_NONBLOCK so we do not hang forever on a FIFO with no writer.  */
          पूर्णाङ्क ref_fd = उद्घाटय (ref_file, O_RDONLY | O_NONBLOCK);
          यदि (0 <= ref_fd)
            {
              off_t file_अन्तः = lseek (ref_fd, 0, SEEK_END);
              पूर्णाङ्क saved_errno = errno;
              close (ref_fd); /* ignore failure */
              यदि (0 <= file_अन्तः)
                file_आकार = file_अन्तः;
              अन्यथा
                {
                  /* restore, in case close clobbered it. */
                  errno = saved_errno;
                }
            }
        }
      यदि (file_आकार < 0)
        error (EXIT_FAILURE, errno, _("cannot get the size of %s"),
               quoteaf (ref_file));
      यदि (!got_आकार)
        आकार = file_आकार;
      अन्यथा
        rsize = file_आकार;
    }

  पूर्णाङ्क oflags = O_WRONLY | (no_create ? 0 : O_CREAT) | O_NONBLOCK;
  bool errors = false;

  प्रति (वर्ण ध्रुव *fname; (fname = *argv); argv++)
    {
      पूर्णाङ्क fd = उद्घाटय (fname, oflags, बहुलकम्_RW_UGO);
      यदि (fd < 0)
        {
          /* 'truncate -s0 -c no-such-file'  shouldn't gen error
             'truncate -s0 no-such-dir/file' should gen ENOENT error
             'truncate -s0 no-such-dir/' should gen EISDIR error
             'truncate -s0 .' should gen EISDIR error */
          यदि (!(no_create && errno == ENOENT))
            {
              error (0, errno, _("cannot open %s for writing"),
                     quoteaf (fname));
              errors = true;
            }
        }
      अन्यथा
        {
          errors |= !do_ftruncate (fd, fname, आकार, rsize, rel_बहुलकम्);
          यदि (close (fd) != 0)
            {
              error (0, errno, _("failed to close %s"), quoteaf (fname));
              errors = true;
            }
        }
    }

  प्रत्यागम errors ? EXIT_FAILURE : EXIT_SUCCESS;
}
