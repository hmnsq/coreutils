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

#অন্তর্ভাব <config.h>             /* sets _FILE_OFFSET_BITS=64 etc. */
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <getopt.h>
#অন্তর্ভাব <sys/types.h>

#অন্তর্ভাব "system.h"
#অন্তর্ভাব "quote.h"
#অন্তর্ভাব "stat-size.h"
#অন্তর্ভাব "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "truncate"

#define AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

/* (-c) If true, don't create if not already there */
স্থির bool no_create;

/* (-o) If true, --size refers to blocks not bytes */
স্থির bool block_mode;

/* (-r) Reference file to use size from */
স্থির অক্ষর ধ্রুবক *ref_file;

স্থির কাঠামো option ধ্রুবক longopts[] =
{
  {"no-create", no_argument, ফাঁকা, 'c'},
  {"io-blocks", no_argument, ফাঁকা, 'o'},
  {"reference", required_argument, ফাঁকা, 'r'},
  {"size", required_argument, ফাঁকা, 's'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ফাঁকা, 0, ফাঁকা, 0}
};

প্রকারলেখ গণনা
{ rm_abs = 0, rm_rel, rm_min, rm_max, rm_rdn, rm_rup } rel_mode_t;

শূন্য
usage (পূর্ণসংখ্যা status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
    {
      printf (_("Usage: %s OPTION... FILE...\n"), program_name);
      fputs (_("\
Shrink or extend the আকার_2 of each FILE to the specified আকার_2\n\
\n\
A FILE argument that does not exist is created.\n\
\n\
If a FILE is larger than the specified আকার_2, the extra তথ্য is lost.\n\
If a FILE is shorter, it is extended and the sparse extended part (hole)\n\
reads as zero bytes.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -c, --no-create        করো not create any files\n\
"));
      oputs (_("\
  -o, --io-blocks        treat আকার_3 as সংখ্যা of IO blocks instead of bytes\n\
"));
      oputs (_("\
  -r, --reference=RFILE  base আকার_2 on RFILE\n\
"));
      oputs (_("\
  -s, --আকার_2=আকার_3        set or adjust the file আকার_2 by আকার_3 bytes\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_আকার_note ();
      fputs (_("\n\
আকার_3 may also be prefixed by one of the following modifying characters:\n\
'+' extend by, '-' reduce by, '<' at most, '>' at least,\n\
'/' round down to multiple of, '%' round up to multiple of.\n"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* return true on success, false on error.  */
স্থির bool
do_ftruncate (পূর্ণসংখ্যা fd, অক্ষর ধ্রুবক *fname, off_t ssize, off_t rsize,
              rel_mode_t rel_mode)
{
  কাঠামো stat sb;
  off_t nsize;

  যদি ((block_mode || (rel_mode && rsize < 0)) && fstat (fd, &sb) != 0)
    {
      error (0, errno, _("cannot fstat %s"), quoteaf (fname));
      ফেরত false;
    }
  যদি (block_mode)
    {
      ptrdiff_t blksize = STP_BLKSIZE (&sb);
      intmax_t ssize0 = ssize;
      যদি (ckd_mul (&ssize, ssize, blksize))
        {
          error (0, 0,
                 _("overflow in %jd * %td byte blocks for file %s"),
                 ssize0, blksize, quoteaf (fname));
          ফেরত false;
        }
    }
  যদি (rel_mode)
    {
      off_t fsize;

      যদি (0 <= rsize)
        fsize = rsize;
      অন্যথা
        {
          যদি (usable_st_আকার (&sb))
            {
              fsize = sb.st_আকার;
              যদি (fsize < 0)
                {
                  /* Sanity check.  Overflow is the only reason I can think
                     this would ever go negative. */
                  error (0, 0, _("%s has unusable, apparently negative size"),
                         quoteaf (fname));
                  ফেরত false;
                }
            }
          অন্যথা
            {
              fsize = lseek (fd, 0, SEEK_END);
              যদি (fsize < 0)
                {
                  error (0, errno, _("cannot get the size of %s"),
                         quoteaf (fname));
                  ফেরত false;
                }
            }
        }

      যদি (rel_mode == rm_min)
        nsize = MAX (fsize, ssize);
      অন্যথা যদি (rel_mode == rm_max)
        nsize = MIN (fsize, ssize);
      অন্যথা যদি (rel_mode == rm_rdn)
        /* 0..ssize-1 -> 0 */
        nsize = fsize - fsize % ssize;
      অন্যথা
        {
          যদি (rel_mode == rm_rup)
            {
              /* 1..ssize -> ssize */
              off_t r = fsize % ssize;
              ssize = r == 0 ? 0 : ssize - r;
            }
          যদি (ckd_add (&nsize, fsize, ssize))
            {
              error (0, 0, _("overflow extending size of file %s"),
                     quoteaf (fname));
              ফেরত false;
            }
        }
    }
  অন্যথা
    nsize = ssize;
  যদি (nsize < 0)
    nsize = 0;

  যদি (ftruncate (fd, nsize) != 0)
    {
      error (0, errno, _("failed to truncate %s at %jd bytes"),
             quoteaf (fname), (intmax_t) {nsize});
      ফেরত false;
    }

  ফেরত true;
}

পূর্ণসংখ্যা
প্রধান (পূর্ণসংখ্যা argc, অক্ষর **argv)
{
  bool got_আকার = false;
  off_t আকার_2 IF_LINT ( = 0);
  off_t rsize = -1;
  rel_mode_t rel_mode = rm_abs;
  পূর্ণসংখ্যা c;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  যখন ((c = getopt_long (argc, argv, "cor:s:", longopts, ফাঁকা)) != -1)
    {
      নির্বাচন (c)
        {
        ক্ষেত্র 'c':
          no_create = true;
          বিরতি;

        ক্ষেত্র 'o':
          block_mode = true;
          বিরতি;

        ক্ষেত্র 'r':
          ref_file = optarg;
          বিরতি;

        ক্ষেত্র 's':
          /* skip any whitespace */
          যখন (isspace (to_uchar (*optarg)))
            optarg++;
          নির্বাচন (*optarg)
            {
            ক্ষেত্র '<':
              rel_mode = rm_max;
              optarg++;
              বিরতি;
            ক্ষেত্র '>':
              rel_mode = rm_min;
              optarg++;
              বিরতি;
            ক্ষেত্র '/':
              rel_mode = rm_rdn;
              optarg++;
              বিরতি;
            ক্ষেত্র '%':
              rel_mode = rm_rup;
              optarg++;
              বিরতি;
            }
          /* skip any whitespace */
          যখন (isspace (to_uchar (*optarg)))
            optarg++;
          যদি (*optarg == '+' || *optarg == '-')
            {
              যদি (rel_mode)
                {
                  error (0, 0, _("multiple relative modifiers specified"));
                  /* Note other combinations are flagged as invalid numbers */
                  usage (EXIT_FAILURE);
                }
              rel_mode = rm_rel;
            }
          /* Support dd BLOCK size suffixes + lowercase g,t,m for bsd compat.
             Note we don't support dd's b=512, c=1, w=2 or 21x512MiB formats. */
          আকার_2 = xdectoimax (optarg, OFF_T_MIN, OFF_T_MAX, "EgGkKmMPQRtTYZ0",
                             _("Invalid number"), 0);
          /* Rounding to multiple of 0 is nonsensical */
          যদি ((rel_mode == rm_rup || rel_mode == rm_rdn) && আকার_2 == 0)
            error (EXIT_FAILURE, 0, _("division by zero"));
          got_আকার = true;
          বিরতি;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        পূর্বনির্ধারিত:
          usage (EXIT_FAILURE);
        }
    }

  argv += optind;
  argc -= optind;

  /* must specify either size or reference file */
  যদি (!ref_file && !got_আকার)
    {
      error (0, 0, _("you must specify either %s or %s"),
             quote_n (0, "--size"), quote_n (1, "--reference"));
      usage (EXIT_FAILURE);
    }
  /* must specify a relative size with a reference file */
  যদি (ref_file && got_আকার && !rel_mode)
    {
      error (0, 0, _("you must specify a relative %s with %s"),
             quote_n (0, "--size"), quote_n (1, "--reference"));
      usage (EXIT_FAILURE);
    }
  /* block_mode without size is not valid */
  যদি (block_mode && !got_আকার)
    {
      error (0, 0, _("%s was specified but %s was not"),
             quote_n (0, "--io-blocks"), quote_n (1, "--size"));
      usage (EXIT_FAILURE);
    }
  /* must specify at least 1 file */
  যদি (argc < 1)
    {
      error (0, 0, _("missing file operand"));
      usage (EXIT_FAILURE);
    }

  যদি (ref_file)
    {
      কাঠামো stat sb;
      off_t file_আকার = -1;
      যদি (stat (ref_file, &sb) != 0)
        error (EXIT_FAILURE, errno, _("cannot stat %s"), quoteaf (ref_file));
      যদি (usable_st_আকার (&sb))
        file_আকার = sb.st_আকার;
      অন্যথা
        {
          /* O_NONBLOCK so we do not hang forever on a FIFO with no writer.  */
          পূর্ণসংখ্যা ref_fd = open (ref_file, O_RDONLY | O_NONBLOCK);
          যদি (0 <= ref_fd)
            {
              off_t file_end = lseek (ref_fd, 0, SEEK_END);
              পূর্ণসংখ্যা saved_errno = errno;
              close (ref_fd); /* ignore failure */
              যদি (0 <= file_end)
                file_আকার = file_end;
              অন্যথা
                {
                  /* restore, in case close clobbered it. */
                  errno = saved_errno;
                }
            }
        }
      যদি (file_আকার < 0)
        error (EXIT_FAILURE, errno, _("cannot get the size of %s"),
               quoteaf (ref_file));
      যদি (!got_আকার)
        আকার_2 = file_আকার;
      অন্যথা
        rsize = file_আকার;
    }

  পূর্ণসংখ্যা oflags = O_WRONLY | (no_create ? 0 : O_CREAT) | O_NONBLOCK;
  bool errors = false;

  জন্য (অক্ষর ধ্রুবক *fname; (fname = *argv); argv++)
    {
      পূর্ণসংখ্যা fd = open (fname, oflags, MODE_RW_UGO);
      যদি (fd < 0)
        {
          /* 'truncate -s0 -c no-such-file'  shouldn't gen error
             'truncate -s0 no-such-dir/file' should gen ENOENT error
             'truncate -s0 no-such-dir/' should gen EISDIR error
             'truncate -s0 .' should gen EISDIR error */
          যদি (!(no_create && errno == ENOENT))
            {
              error (0, errno, _("cannot open %s for writing"),
                     quoteaf (fname));
              errors = true;
            }
        }
      অন্যথা
        {
          errors |= !do_ftruncate (fd, fname, আকার_2, rsize, rel_mode);
          যদি (close (fd) != 0)
            {
              error (0, errno, _("failed to close %s"), quoteaf (fname));
              errors = true;
            }
        }
    }

  ফেরত errors ? EXIT_FAILURE : EXIT_SUCCESS;
}
