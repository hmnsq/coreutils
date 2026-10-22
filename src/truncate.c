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

#అన్తర్భావ <config.h>             /* sets _FILE_OFFSET_BITS=64 etc. */
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <getopt.h>
#అన్తర్భావ <sys/types.h>

#అన్తర్భావ "system.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "stat-size.h"
#అన్తర్భావ "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "truncate"

#నిర్వచించు AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

/* (-c) If true, don't create if not already there */
స్థిర bool no_create;

/* (-o) If true, --size refers to blocks not bytes */
స్థిర bool ఖండం_బహుళకం;

/* (-r) Reference file to use size from */
స్థిర అక్షరం స్థిరరాశి *ref_file;

స్థిర నిర్మాణం option స్థిరరాశి longopts[] =
{
  {"no-create", no_వాదం, ఖాళీ, 'c'},
  {"io-blocks", no_వాదం, ఖాళీ, 'o'},
  {"reference", required_వాదం, ఖాళీ, 'r'},
  {"size", required_వాదం, ఖాళీ, 's'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0}
};

రకనిర్వచనం గణన
{ rm_abs = 0, rm_rel, rm_min, rm_max, rm_rdn, rm_rup } rel_బహుళకం_t;

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("Usage: %s OPTION... FILE...\n"), కార్యక్రమం_name);
      fputs (_("\
Shrink or extend the కొలత యొక్క each FILE గమ్యం the specified కొలత\n\
\n\
A FILE వాదం that does not exist is created.\n\
\n\
If a FILE is larger than the specified కొలత, the extra దత్తాంశం is కోల్పోయిన.\n\
If a FILE is shorter, it is extended and the sparse extended part (hole)\n\
reads as zero బైట్లు.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -c, --no-create        చేయి not create any files\n\
"));
      oputs (_("\
  -o, --io-ఖండాలు        treat కొలత_2 as సంఖ్య యొక్క IO ఖండాలు instead యొక్క బైట్లు\n\
"));
      oputs (_("\
  -r, --reference=RFILE  పునాది_2 కొలత on RFILE\n\
"));
      oputs (_("\
  -s, --కొలత=కొలత_2        సమితి_2 or adjust the file కొలత by కొలత_2 బైట్లు\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      emit_కొలత_note ();
      fputs (_("\n\
కొలత_2 may also be prefixed by one యొక్క the following modifying characters:\n\
'+' extend by, '-' reduce by, '<' at most, '>' at కనిష్ఠ,\n\
'/' విడత_2 కిందికి గమ్యం గుణిజం యొక్క, '%' విడత_2 పైకి గమ్యం గుణిజం యొక్క.\n"), stdout);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

/* return true on success, false on error.  */
స్థిర bool
do_ftruncate (పూర్ణాంకం fd, అక్షరం స్థిరరాశి *fname, off_t ssize, off_t rsize,
              rel_బహుళకం_t rel_బహుళకం)
{
  నిర్మాణం stat sb;
  off_t nsize;

  ఒకవేళ ((ఖండం_బహుళకం || (rel_బహుళకం && rsize < 0)) && fstat (fd, &sb) != 0)
    {
      error (0, errno, _("cannot fstat %s"), quoteaf (fname));
      తిరిగి false;
    }
  ఒకవేళ (ఖండం_బహుళకం)
    {
      ptrdiff_t blksize = STP_BLKSIZE (&sb);
      intmax_t ssize0 = ssize;
      ఒకవేళ (ckd_mul (&ssize, ssize, blksize))
        {
          error (0, 0,
                 _("overflow in %jd * %td byte blocks for file %s"),
                 ssize0, blksize, quoteaf (fname));
          తిరిగి false;
        }
    }
  ఒకవేళ (rel_బహుళకం)
    {
      off_t fsize;

      ఒకవేళ (0 <= rsize)
        fsize = rsize;
      లేకపోతే
        {
          ఒకవేళ (usable_st_కొలత (&sb))
            {
              fsize = sb.st_కొలత;
              ఒకవేళ (fsize < 0)
                {
                  /* Sanity check.  Overflow is the only reason I can think
                     this would ever go negative. */
                  error (0, 0, _("%s has unusable, apparently negative size"),
                         quoteaf (fname));
                  తిరిగి false;
                }
            }
          లేకపోతే
            {
              fsize = lseek (fd, 0, SEEK_END);
              ఒకవేళ (fsize < 0)
                {
                  error (0, errno, _("cannot get the size of %s"),
                         quoteaf (fname));
                  తిరిగి false;
                }
            }
        }

      ఒకవేళ (rel_బహుళకం == rm_min)
        nsize = MAX (fsize, ssize);
      లేకపోతే ఒకవేళ (rel_బహుళకం == rm_max)
        nsize = MIN (fsize, ssize);
      లేకపోతే ఒకవేళ (rel_బహుళకం == rm_rdn)
        /* 0..ssize-1 -> 0 */
        nsize = fsize - fsize % ssize;
      లేకపోతే
        {
          ఒకవేళ (rel_బహుళకం == rm_rup)
            {
              /* 1..ssize -> ssize */
              off_t r = fsize % ssize;
              ssize = r == 0 ? 0 : ssize - r;
            }
          ఒకవేళ (ckd_కూడు (&nsize, fsize, ssize))
            {
              error (0, 0, _("overflow extending size of file %s"),
                     quoteaf (fname));
              తిరిగి false;
            }
        }
    }
  లేకపోతే
    nsize = ssize;
  ఒకవేళ (nsize < 0)
    nsize = 0;

  ఒకవేళ (ftruncate (fd, nsize) != 0)
    {
      error (0, errno, _("failed to truncate %s at %jd bytes"),
             quoteaf (fname), (intmax_t) {nsize});
      తిరిగి false;
    }

  తిరిగి true;
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  bool got_కొలత = false;
  off_t కొలత IF_LINT ( = 0);
  off_t rsize = -1;
  rel_బహుళకం_t rel_బహుళకం = rm_abs;
  పూర్ణాంకం c;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  అయితే ((c = getopt_long (argc, argv, "cor:s:", longopts, ఖాళీ)) != -1)
    {
      ఎంపిక (c)
        {
        స్థితి 'c':
          no_create = true;
          ఆపు;

        స్థితి 'o':
          ఖండం_బహుళకం = true;
          ఆపు;

        స్థితి 'r':
          ref_file = optarg;
          ఆపు;

        స్థితి 's':
          /* skip any whitespace */
          అయితే (ఖాళీనా (గమ్యం_uchar (*optarg)))
            optarg++;
          ఎంపిక (*optarg)
            {
            స్థితి '<':
              rel_బహుళకం = rm_max;
              optarg++;
              ఆపు;
            స్థితి '>':
              rel_బహుళకం = rm_min;
              optarg++;
              ఆపు;
            స్థితి '/':
              rel_బహుళకం = rm_rdn;
              optarg++;
              ఆపు;
            స్థితి '%':
              rel_బహుళకం = rm_rup;
              optarg++;
              ఆపు;
            }
          /* skip any whitespace */
          అయితే (ఖాళీనా (గమ్యం_uchar (*optarg)))
            optarg++;
          ఒకవేళ (*optarg == '+' || *optarg == '-')
            {
              ఒకవేళ (rel_బహుళకం)
                {
                  error (0, 0, _("multiple relative modifiers specified"));
                  /* Note other combinations are flagged as invalid numbers */
                  usage (EXIT_FAILURE);
                }
              rel_బహుళకం = rm_rel;
            }
          /* Support dd BLOCK size suffixes + lowercase g,t,m for bsd compat.
             Note we don't support dd's b=512, c=1, w=2 or 21x512MiB formats. */
          కొలత = xdectoimax (optarg, OFF_T_MIN, OFF_T_MAX, "EgGkKmMPQRtTYZ0",
                             _("Invalid number"), 0);
          /* Rounding to multiple of 0 is nonsensical */
          ఒకవేళ ((rel_బహుళకం == rm_rup || rel_బహుళకం == rm_rdn) && కొలత == 0)
            error (EXIT_FAILURE, 0, _("division by zero"));
          got_కొలత = true;
          ఆపు;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);

        అప్రమేయ:
          usage (EXIT_FAILURE);
        }
    }

  argv += optind;
  argc -= optind;

  /* must specify either size or reference file */
  ఒకవేళ (!ref_file && !got_కొలత)
    {
      error (0, 0, _("you must specify either %s or %s"),
             quote_n (0, "--size"), quote_n (1, "--reference"));
      usage (EXIT_FAILURE);
    }
  /* must specify a relative size with a reference file */
  ఒకవేళ (ref_file && got_కొలత && !rel_బహుళకం)
    {
      error (0, 0, _("you must specify a relative %s with %s"),
             quote_n (0, "--size"), quote_n (1, "--reference"));
      usage (EXIT_FAILURE);
    }
  /* block_mode without size is not valid */
  ఒకవేళ (ఖండం_బహుళకం && !got_కొలత)
    {
      error (0, 0, _("%s was specified but %s was not"),
             quote_n (0, "--io-blocks"), quote_n (1, "--size"));
      usage (EXIT_FAILURE);
    }
  /* must specify at least 1 file */
  ఒకవేళ (argc < 1)
    {
      error (0, 0, _("missing file operand"));
      usage (EXIT_FAILURE);
    }

  ఒకవేళ (ref_file)
    {
      నిర్మాణం stat sb;
      off_t file_కొలత = -1;
      ఒకవేళ (stat (ref_file, &sb) != 0)
        error (EXIT_FAILURE, errno, _("cannot stat %s"), quoteaf (ref_file));
      ఒకవేళ (usable_st_కొలత (&sb))
        file_కొలత = sb.st_కొలత;
      లేకపోతే
        {
          /* O_NONBLOCK so we do not hang forever on a FIFO with no writer.  */
          పూర్ణాంకం ref_fd = open (ref_file, O_RDONLY | O_NONBLOCK);
          ఒకవేళ (0 <= ref_fd)
            {
              off_t file_చివర = lseek (ref_fd, 0, SEEK_END);
              పూర్ణాంకం saved_errno = errno;
              close (ref_fd); /* ignore failure */
              ఒకవేళ (0 <= file_చివర)
                file_కొలత = file_చివర;
              లేకపోతే
                {
                  /* restore, in case close clobbered it. */
                  errno = saved_errno;
                }
            }
        }
      ఒకవేళ (file_కొలత < 0)
        error (EXIT_FAILURE, errno, _("cannot get the size of %s"),
               quoteaf (ref_file));
      ఒకవేళ (!got_కొలత)
        కొలత = file_కొలత;
      లేకపోతే
        rsize = file_కొలత;
    }

  పూర్ణాంకం oflags = O_WRONLY | (no_create ? 0 : O_CREAT) | O_NONBLOCK;
  bool errors = false;

  కొరకు (అక్షరం స్థిరరాశి *fname; (fname = *argv); argv++)
    {
      పూర్ణాంకం fd = open (fname, oflags, బహుళకం_RW_UGO);
      ఒకవేళ (fd < 0)
        {
          /* 'truncate -s0 -c no-such-file'  shouldn't gen error
             'truncate -s0 no-such-dir/file' should gen ENOENT error
             'truncate -s0 no-such-dir/' should gen EISDIR error
             'truncate -s0 .' should gen EISDIR error */
          ఒకవేళ (!(no_create && errno == ENOENT))
            {
              error (0, errno, _("cannot open %s for writing"),
                     quoteaf (fname));
              errors = true;
            }
        }
      లేకపోతే
        {
          errors |= !do_ftruncate (fd, fname, కొలత, rsize, rel_బహుళకం);
          ఒకవేళ (close (fd) != 0)
            {
              error (0, errno, _("failed to close %s"), quoteaf (fname));
              errors = true;
            }
        }
    }

  తిరిగి errors ? EXIT_FAILURE : EXIT_SUCCESS;
}
