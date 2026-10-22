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

#ضمن <config.h>             /* sets _FILE_OFFSET_BITS=64 etc. */
#ضمن <stdio.h>
#ضمن <getopt.h>
#ضمن <sys/types.h>

#ضمن "system.h"
#ضمن "quote.h"
#ضمن "stat-size.h"
#ضمن "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "truncate"

#define AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

/* (-c) If true, don't create if not already there */
ساكن bool no_create;

/* (-o) If true, --size refers to blocks not bytes */
ساكن bool block_mode;

/* (-r) Reference file to use size from */
ساكن حرف ثابت *ref_file;

ساكن بنية option ثابت longopts[] =
{
  {"no-create", no_argument, عدم, 'c'},
  {"io-blocks", no_argument, عدم, 'o'},
  {"reference", required_argument, عدم, 'r'},
  {"size", required_argument, عدم, 's'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

تعريفنوع تعداد
{ rm_abs = 0, rm_rel, rm_min, rm_max, rm_rdn, rm_rup } rel_mode_t;

فراغ
usage (صحيح status)
{
  إذا (status != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      printf (_("Usage: %s OPTION... FILE...\n"), program_name);
      fputs (_("\
Shrink or extend the size of each FILE to the specified size\n\
\n\
A FILE argument that does not exist is created.\n\
\n\
If a FILE is larger than the specified size, the extra data is lost.\n\
If a FILE is shorter, it is extended and the sparse extended part (hole)\n\
reads as zero bytes.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -c, --no-create        افعل not create any files\n\
"));
      oputs (_("\
  -o, --io-blocks        treat SIZE as العدد of IO blocks instead of bytes\n\
"));
      oputs (_("\
  -r, --reference=RFILE  base size on RFILE\n\
"));
      oputs (_("\
  -s, --size=SIZE        set or adjust the file size by SIZE bytes\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_size_note ();
      fputs (_("\n\
SIZE may also be prefixed by one of the following modifying characters:\n\
'+' extend by, '-' reduce by, '<' at most, '>' at least,\n\
'/' round down to multiple of, '%' round up to multiple of.\n"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* return true on success, false on error.  */
ساكن bool
do_ftruncate (صحيح fd, حرف ثابت *fname, off_t ssize, off_t rsize,
              rel_mode_t rel_mode)
{
  بنية stat sb;
  off_t nsize;

  إذا ((block_mode || (rel_mode && rsize < 0)) && fstat (fd, &sb) != 0)
    {
      error (0, errno, _("cannot fstat %s"), quoteaf (fname));
      إرجاع false;
    }
  إذا (block_mode)
    {
      ptrdiff_t blksize = STP_BLKSIZE (&sb);
      intmax_t ssize0 = ssize;
      إذا (ckd_mul (&ssize, ssize, blksize))
        {
          error (0, 0,
                 _("overflow in %jd * %td byte blocks for file %s"),
                 ssize0, blksize, quoteaf (fname));
          إرجاع false;
        }
    }
  إذا (rel_mode)
    {
      off_t fsize;

      إذا (0 <= rsize)
        fsize = rsize;
      وإلا
        {
          إذا (usable_st_size (&sb))
            {
              fsize = sb.st_size;
              إذا (fsize < 0)
                {
                  /* Sanity check.  Overflow is the only reason I can think
                     this would ever go negative. */
                  error (0, 0, _("%s has unusable, apparently negative size"),
                         quoteaf (fname));
                  إرجاع false;
                }
            }
          وإلا
            {
              fsize = lseek (fd, 0, SEEK_END);
              إذا (fsize < 0)
                {
                  error (0, errno, _("cannot get the size of %s"),
                         quoteaf (fname));
                  إرجاع false;
                }
            }
        }

      إذا (rel_mode == rm_min)
        nsize = MAX (fsize, ssize);
      وإلا إذا (rel_mode == rm_max)
        nsize = MIN (fsize, ssize);
      وإلا إذا (rel_mode == rm_rdn)
        /* 0..ssize-1 -> 0 */
        nsize = fsize - fsize % ssize;
      وإلا
        {
          إذا (rel_mode == rm_rup)
            {
              /* 1..ssize -> ssize */
              off_t r = fsize % ssize;
              ssize = r == 0 ? 0 : ssize - r;
            }
          إذا (ckd_add (&nsize, fsize, ssize))
            {
              error (0, 0, _("overflow extending size of file %s"),
                     quoteaf (fname));
              إرجاع false;
            }
        }
    }
  وإلا
    nsize = ssize;
  إذا (nsize < 0)
    nsize = 0;

  إذا (ftruncate (fd, nsize) != 0)
    {
      error (0, errno, _("failed to truncate %s at %jd bytes"),
             quoteaf (fname), (intmax_t) {nsize});
      إرجاع false;
    }

  إرجاع true;
}

صحيح
الرئيسية (صحيح argc, حرف **argv)
{
  bool got_size = false;
  off_t size IF_LINT ( = 0);
  off_t rsize = -1;
  rel_mode_t rel_mode = rm_abs;
  صحيح c;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  بينما ((c = getopt_long (argc, argv, "cor:s:", longopts, عدم)) != -1)
    {
      بدل (c)
        {
        حالة 'c':
          no_create = true;
          اكسر;

        حالة 'o':
          block_mode = true;
          اكسر;

        حالة 'r':
          ref_file = optarg;
          اكسر;

        حالة 's':
          /* skip any whitespace */
          بينما (isspace (to_uchar (*optarg)))
            optarg++;
          بدل (*optarg)
            {
            حالة '<':
              rel_mode = rm_max;
              optarg++;
              اكسر;
            حالة '>':
              rel_mode = rm_min;
              optarg++;
              اكسر;
            حالة '/':
              rel_mode = rm_rdn;
              optarg++;
              اكسر;
            حالة '%':
              rel_mode = rm_rup;
              optarg++;
              اكسر;
            }
          /* skip any whitespace */
          بينما (isspace (to_uchar (*optarg)))
            optarg++;
          إذا (*optarg == '+' || *optarg == '-')
            {
              إذا (rel_mode)
                {
                  error (0, 0, _("multiple relative modifiers specified"));
                  /* Note other combinations are flagged as invalid numbers */
                  usage (EXIT_FAILURE);
                }
              rel_mode = rm_rel;
            }
          /* Support dd BLOCK size suffixes + lowercase g,t,m for bsd compat.
             Note we don't support dd's b=512, c=1, w=2 or 21x512MiB formats. */
          size = xdectoimax (optarg, OFF_T_MIN, OFF_T_MAX, "EgGkKmMPQRtTYZ0",
                             _("Invalid number"), 0);
          /* Rounding to multiple of 0 is nonsensical */
          إذا ((rel_mode == rm_rup || rel_mode == rm_rdn) && size == 0)
            error (EXIT_FAILURE, 0, _("division by zero"));
          got_size = true;
          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  argv += optind;
  argc -= optind;

  /* must specify either size or reference file */
  إذا (!ref_file && !got_size)
    {
      error (0, 0, _("you must specify either %s or %s"),
             quote_n (0, "--size"), quote_n (1, "--reference"));
      usage (EXIT_FAILURE);
    }
  /* must specify a relative size with a reference file */
  إذا (ref_file && got_size && !rel_mode)
    {
      error (0, 0, _("you must specify a relative %s with %s"),
             quote_n (0, "--size"), quote_n (1, "--reference"));
      usage (EXIT_FAILURE);
    }
  /* block_mode without size is not valid */
  إذا (block_mode && !got_size)
    {
      error (0, 0, _("%s was specified but %s was not"),
             quote_n (0, "--io-blocks"), quote_n (1, "--size"));
      usage (EXIT_FAILURE);
    }
  /* must specify at least 1 file */
  إذا (argc < 1)
    {
      error (0, 0, _("missing file operand"));
      usage (EXIT_FAILURE);
    }

  إذا (ref_file)
    {
      بنية stat sb;
      off_t file_size = -1;
      إذا (stat (ref_file, &sb) != 0)
        error (EXIT_FAILURE, errno, _("cannot stat %s"), quoteaf (ref_file));
      إذا (usable_st_size (&sb))
        file_size = sb.st_size;
      وإلا
        {
          /* O_NONBLOCK so we do not hang forever on a FIFO with no writer.  */
          صحيح ref_fd = open (ref_file, O_RDONLY | O_NONBLOCK);
          إذا (0 <= ref_fd)
            {
              off_t file_end = lseek (ref_fd, 0, SEEK_END);
              صحيح saved_errno = errno;
              close (ref_fd); /* ignore failure */
              إذا (0 <= file_end)
                file_size = file_end;
              وإلا
                {
                  /* restore, in case close clobbered it. */
                  errno = saved_errno;
                }
            }
        }
      إذا (file_size < 0)
        error (EXIT_FAILURE, errno, _("cannot get the size of %s"),
               quoteaf (ref_file));
      إذا (!got_size)
        size = file_size;
      وإلا
        rsize = file_size;
    }

  صحيح oflags = O_WRONLY | (no_create ? 0 : O_CREAT) | O_NONBLOCK;
  bool errors = false;

  لكل (حرف ثابت *fname; (fname = *argv); argv++)
    {
      صحيح fd = open (fname, oflags, MODE_RW_UGO);
      إذا (fd < 0)
        {
          /* 'truncate -s0 -c no-such-file'  shouldn't gen error
             'truncate -s0 no-such-dir/file' should gen ENOENT error
             'truncate -s0 no-such-dir/' should gen EISDIR error
             'truncate -s0 .' should gen EISDIR error */
          إذا (!(no_create && errno == ENOENT))
            {
              error (0, errno, _("cannot open %s for writing"),
                     quoteaf (fname));
              errors = true;
            }
        }
      وإلا
        {
          errors |= !do_ftruncate (fd, fname, size, rsize, rel_mode);
          إذا (close (fd) != 0)
            {
              error (0, errno, _("failed to close %s"), quoteaf (fname));
              errors = true;
            }
        }
    }

  إرجاع errors ? EXIT_FAILURE : EXIT_SUCCESS;
}
