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

#અન્તર્ભાવ <config.h>             /* sets _FILE_OFFSET_BITS=64 etc. */
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <getopt.h>
#અન્તર્ભાવ <sys/types.h>

#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "stat-size.h"
#અન્તર્ભાવ "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "truncate"

#define AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

/* (-c) If true, don't create if not already there */
સ્થિર bool no_create;

/* (-o) If true, --size refers to blocks not bytes */
સ્થિર bool block_mode;

/* (-r) Reference file to use size from */
સ્થિર અક્ષર અચલ *ref_file;

સ્થિર સંરચના option અચલ longopts[] =
{
  {"no-create", no_argument, ખાલી, 'c'},
  {"io-blocks", no_argument, ખાલી, 'o'},
  {"reference", required_argument, ખાલી, 'r'},
  {"size", required_argument, ખાલી, 's'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ખાલી, 0, ખાલી, 0}
};

પ્રકારવ્યાખ્યા ગણના
{ rm_abs = 0, rm_rel, rm_min, rm_max, rm_rdn, rm_rup } rel_mode_t;

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
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
  -c, --no-create        કરો not create any files\n\
"));
      oputs (_("\
  -o, --io-blocks        treat SIZE as સંખ્યા of IO blocks instead of bytes\n\
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
સ્થિર bool
do_ftruncate (પૂર્ણાંક fd, અક્ષર અચલ *fname, off_t ssize, off_t rsize,
              rel_mode_t rel_mode)
{
  સંરચના stat sb;
  off_t nsize;

  જો ((block_mode || (rel_mode && rsize < 0)) && fstat (fd, &sb) != 0)
    {
      error (0, errno, _("cannot fstat %s"), quoteaf (fname));
      પાછા false;
    }
  જો (block_mode)
    {
      ptrdiff_t blksize = STP_BLKSIZE (&sb);
      intmax_t ssize0 = ssize;
      જો (ckd_mul (&ssize, ssize, blksize))
        {
          error (0, 0,
                 _("overflow in %jd * %td byte blocks for file %s"),
                 ssize0, blksize, quoteaf (fname));
          પાછા false;
        }
    }
  જો (rel_mode)
    {
      off_t fsize;

      જો (0 <= rsize)
        fsize = rsize;
      નહીંતર
        {
          જો (usable_st_size (&sb))
            {
              fsize = sb.st_size;
              જો (fsize < 0)
                {
                  /* Sanity check.  Overflow is the only reason I can think
                     this would ever go negative. */
                  error (0, 0, _("%s has unusable, apparently negative size"),
                         quoteaf (fname));
                  પાછા false;
                }
            }
          નહીંતર
            {
              fsize = lseek (fd, 0, SEEK_END);
              જો (fsize < 0)
                {
                  error (0, errno, _("cannot get the size of %s"),
                         quoteaf (fname));
                  પાછા false;
                }
            }
        }

      જો (rel_mode == rm_min)
        nsize = MAX (fsize, ssize);
      નહીંતર જો (rel_mode == rm_max)
        nsize = MIN (fsize, ssize);
      નહીંતર જો (rel_mode == rm_rdn)
        /* 0..ssize-1 -> 0 */
        nsize = fsize - fsize % ssize;
      નહીંતર
        {
          જો (rel_mode == rm_rup)
            {
              /* 1..ssize -> ssize */
              off_t r = fsize % ssize;
              ssize = r == 0 ? 0 : ssize - r;
            }
          જો (ckd_add (&nsize, fsize, ssize))
            {
              error (0, 0, _("overflow extending size of file %s"),
                     quoteaf (fname));
              પાછા false;
            }
        }
    }
  નહીંતર
    nsize = ssize;
  જો (nsize < 0)
    nsize = 0;

  જો (ftruncate (fd, nsize) != 0)
    {
      error (0, errno, _("failed to truncate %s at %jd bytes"),
             quoteaf (fname), (intmax_t) {nsize});
      પાછા false;
    }

  પાછા true;
}

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  bool got_size = false;
  off_t size IF_LINT ( = 0);
  off_t rsize = -1;
  rel_mode_t rel_mode = rm_abs;
  પૂર્ણાંક c;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  જ્યારે ((c = getopt_long (argc, argv, "cor:s:", longopts, ખાલી)) != -1)
    {
      પસંદગી (c)
        {
        સ્થિતિ 'c':
          no_create = true;
          તોડો;

        સ્થિતિ 'o':
          block_mode = true;
          તોડો;

        સ્થિતિ 'r':
          ref_file = optarg;
          તોડો;

        સ્થિતિ 's':
          /* skip any whitespace */
          જ્યારે (isspace (to_uchar (*optarg)))
            optarg++;
          પસંદગી (*optarg)
            {
            સ્થિતિ '<':
              rel_mode = rm_max;
              optarg++;
              તોડો;
            સ્થિતિ '>':
              rel_mode = rm_min;
              optarg++;
              તોડો;
            સ્થિતિ '/':
              rel_mode = rm_rdn;
              optarg++;
              તોડો;
            સ્થિતિ '%':
              rel_mode = rm_rup;
              optarg++;
              તોડો;
            }
          /* skip any whitespace */
          જ્યારે (isspace (to_uchar (*optarg)))
            optarg++;
          જો (*optarg == '+' || *optarg == '-')
            {
              જો (rel_mode)
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
          જો ((rel_mode == rm_rup || rel_mode == rm_rdn) && size == 0)
            error (EXIT_FAILURE, 0, _("division by zero"));
          got_size = true;
          તોડો;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        મૂળભૂત:
          usage (EXIT_FAILURE);
        }
    }

  argv += optind;
  argc -= optind;

  /* must specify either size or reference file */
  જો (!ref_file && !got_size)
    {
      error (0, 0, _("you must specify either %s or %s"),
             quote_n (0, "--size"), quote_n (1, "--reference"));
      usage (EXIT_FAILURE);
    }
  /* must specify a relative size with a reference file */
  જો (ref_file && got_size && !rel_mode)
    {
      error (0, 0, _("you must specify a relative %s with %s"),
             quote_n (0, "--size"), quote_n (1, "--reference"));
      usage (EXIT_FAILURE);
    }
  /* block_mode without size is not valid */
  જો (block_mode && !got_size)
    {
      error (0, 0, _("%s was specified but %s was not"),
             quote_n (0, "--io-blocks"), quote_n (1, "--size"));
      usage (EXIT_FAILURE);
    }
  /* must specify at least 1 file */
  જો (argc < 1)
    {
      error (0, 0, _("missing file operand"));
      usage (EXIT_FAILURE);
    }

  જો (ref_file)
    {
      સંરચના stat sb;
      off_t file_size = -1;
      જો (stat (ref_file, &sb) != 0)
        error (EXIT_FAILURE, errno, _("cannot stat %s"), quoteaf (ref_file));
      જો (usable_st_size (&sb))
        file_size = sb.st_size;
      નહીંતર
        {
          /* O_NONBLOCK so we do not hang forever on a FIFO with no writer.  */
          પૂર્ણાંક ref_fd = open (ref_file, O_RDONLY | O_NONBLOCK);
          જો (0 <= ref_fd)
            {
              off_t file_end = lseek (ref_fd, 0, SEEK_END);
              પૂર્ણાંક saved_errno = errno;
              close (ref_fd); /* ignore failure */
              જો (0 <= file_end)
                file_size = file_end;
              નહીંતર
                {
                  /* restore, in case close clobbered it. */
                  errno = saved_errno;
                }
            }
        }
      જો (file_size < 0)
        error (EXIT_FAILURE, errno, _("cannot get the size of %s"),
               quoteaf (ref_file));
      જો (!got_size)
        size = file_size;
      નહીંતર
        rsize = file_size;
    }

  પૂર્ણાંક oflags = O_WRONLY | (no_create ? 0 : O_CREAT) | O_NONBLOCK;
  bool errors = false;

  માટે (અક્ષર અચલ *fname; (fname = *argv); argv++)
    {
      પૂર્ણાંક fd = open (fname, oflags, MODE_RW_UGO);
      જો (fd < 0)
        {
          /* 'truncate -s0 -c no-such-file'  shouldn't gen error
             'truncate -s0 no-such-dir/file' should gen ENOENT error
             'truncate -s0 no-such-dir/' should gen EISDIR error
             'truncate -s0 .' should gen EISDIR error */
          જો (!(no_create && errno == ENOENT))
            {
              error (0, errno, _("cannot open %s for writing"),
                     quoteaf (fname));
              errors = true;
            }
        }
      નહીંતર
        {
          errors |= !do_ftruncate (fd, fname, size, rsize, rel_mode);
          જો (close (fd) != 0)
            {
              error (0, errno, _("failed to close %s"), quoteaf (fname));
              errors = true;
            }
        }
    }

  પાછા errors ? EXIT_FAILURE : EXIT_SUCCESS;
}
