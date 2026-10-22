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

#شامل <config.h>             /* sets _FILE_OFFSET_BITS=64 etc. */
#شامل <stdio.h>
#شامل <getopt.h>
#شامل <sys/types.h>

#شامل "system.h"
#شامل "quote.h"
#شامل "stat-size.h"
#شامل "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "truncate"

#تعریف_کرو AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

/* (-c) If true, don't create if not already there */
ساکن bool no_create;

/* (-o) If true, --size refers to blocks not bytes */
ساکن bool بلاک_منوال;

/* (-r) Reference file to use size from */
ساکن حرف ثابت *ref_file;

ساکن ساخت option ثابت longopts[] =
{
  {"no-create", no_دلیل, تہی, 'c'},
  {"io-blocks", no_دلیل, تہی, 'o'},
  {"reference", required_دلیل, تہی, 'r'},
  {"size", required_دلیل, تہی, 's'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0}
};

قسمتعریف شمار
{ rm_abs = 0, rm_rel, rm_min, rm_max, rm_rdn, rm_rup } rel_منوال_t;

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("Usage: %s OPTION... FILE...\n"), پروگرام_name);
      fputs (_("\
Shrink or extend the سائز کا each FILE مقصد the specified سائز\n\
\n\
A FILE دلیل that does not exist is created.\n\
\n\
If a FILE is larger than the specified سائز, the extra ڈیٹا is گمشدہ.\n\
If a FILE is shorter, it is extended and the sparse extended part (hole)\n\
reads as zero بائٹیں.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -c, --no-create        کرو not create any files\n\
"));
      oputs (_("\
  -o, --io-بلاکیں        treat سائز_2 as نمبر کا IO بلاکیں instead کا بائٹیں\n\
"));
      oputs (_("\
  -r, --reference=RFILE  بنیاد_2 سائز on RFILE\n\
"));
      oputs (_("\
  -s, --سائز=سائز_2        سیٹ_2 or adjust the file سائز by سائز_2 بائٹیں\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      emit_سائز_note ();
      fputs (_("\n\
سائز_2 may also be prefixed by one کا the following modifying characters:\n\
'+' extend by, '-' reduce by, '<' at most, '>' at کم_سے_کم,\n\
'/' باری_2 نیچے_کو مقصد مضرب کا, '%' باری_2 اوپر مقصد مضرب کا.\n"), stdout);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

/* return true on success, false on error.  */
ساکن bool
do_ftruncate (صحیح fd, حرف ثابت *fname, off_t ssize, off_t rsize,
              rel_منوال_t rel_منوال)
{
  ساخت stat sb;
  off_t nsize;

  اگر ((بلاک_منوال || (rel_منوال && rsize < 0)) && fstat (fd, &sb) != 0)
    {
      error (0, errno, _("cannot fstat %s"), quoteaf (fname));
      واپس false;
    }
  اگر (بلاک_منوال)
    {
      ptrdiff_t blksize = STP_BLKSIZE (&sb);
      intmax_t ssize0 = ssize;
      اگر (ckd_mul (&ssize, ssize, blksize))
        {
          error (0, 0,
                 _("overflow in %jd * %td byte blocks for file %s"),
                 ssize0, blksize, quoteaf (fname));
          واپس false;
        }
    }
  اگر (rel_منوال)
    {
      off_t fsize;

      اگر (0 <= rsize)
        fsize = rsize;
      ورنہ
        {
          اگر (usable_st_سائز (&sb))
            {
              fsize = sb.st_سائز;
              اگر (fsize < 0)
                {
                  /* Sanity check.  Overflow is the only reason I can think
                     this would ever go negative. */
                  error (0, 0, _("%s has unusable, apparently negative size"),
                         quoteaf (fname));
                  واپس false;
                }
            }
          ورنہ
            {
              fsize = lseek (fd, 0, SEEK_END);
              اگر (fsize < 0)
                {
                  error (0, errno, _("cannot get the size of %s"),
                         quoteaf (fname));
                  واپس false;
                }
            }
        }

      اگر (rel_منوال == rm_min)
        nsize = MAX (fsize, ssize);
      ورنہ اگر (rel_منوال == rm_max)
        nsize = MIN (fsize, ssize);
      ورنہ اگر (rel_منوال == rm_rdn)
        /* 0..ssize-1 -> 0 */
        nsize = fsize - fsize % ssize;
      ورنہ
        {
          اگر (rel_منوال == rm_rup)
            {
              /* 1..ssize -> ssize */
              off_t r = fsize % ssize;
              ssize = r == 0 ? 0 : ssize - r;
            }
          اگر (ckd_جوڑو (&nsize, fsize, ssize))
            {
              error (0, 0, _("overflow extending size of file %s"),
                     quoteaf (fname));
              واپس false;
            }
        }
    }
  ورنہ
    nsize = ssize;
  اگر (nsize < 0)
    nsize = 0;

  اگر (ftruncate (fd, nsize) != 0)
    {
      error (0, errno, _("failed to truncate %s at %jd bytes"),
             quoteaf (fname), (intmax_t) {nsize});
      واپس false;
    }

  واپس true;
}

صحیح
مین (صحیح argc, حرف **argv)
{
  bool got_سائز = false;
  off_t سائز IF_LINT ( = 0);
  off_t rsize = -1;
  rel_منوال_t rel_منوال = rm_abs;
  صحیح c;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  جب ((c = getopt_long (argc, argv, "cor:s:", longopts, تہی)) != -1)
    {
      انتخاب (c)
        {
        حالت 'c':
          no_create = true;
          توڑو;

        حالت 'o':
          بلاک_منوال = true;
          توڑو;

        حالت 'r':
          ref_file = optarg;
          توڑو;

        حالت 's':
          /* skip any whitespace */
          جب (خلا_ہے (مقصد_uchar (*optarg)))
            optarg++;
          انتخاب (*optarg)
            {
            حالت '<':
              rel_منوال = rm_max;
              optarg++;
              توڑو;
            حالت '>':
              rel_منوال = rm_min;
              optarg++;
              توڑو;
            حالت '/':
              rel_منوال = rm_rdn;
              optarg++;
              توڑو;
            حالت '%':
              rel_منوال = rm_rup;
              optarg++;
              توڑو;
            }
          /* skip any whitespace */
          جب (خلا_ہے (مقصد_uchar (*optarg)))
            optarg++;
          اگر (*optarg == '+' || *optarg == '-')
            {
              اگر (rel_منوال)
                {
                  error (0, 0, _("multiple relative modifiers specified"));
                  /* Note other combinations are flagged as invalid numbers */
                  usage (EXIT_FAILURE);
                }
              rel_منوال = rm_rel;
            }
          /* Support dd BLOCK size suffixes + lowercase g,t,m for bsd compat.
             Note we don't support dd's b=512, c=1, w=2 or 21x512MiB formats. */
          سائز = xdectoimax (optarg, OFF_T_MIN, OFF_T_MAX, "EgGkKmMPQRtTYZ0",
                             _("Invalid number"), 0);
          /* Rounding to multiple of 0 is nonsensical */
          اگر ((rel_منوال == rm_rup || rel_منوال == rm_rdn) && سائز == 0)
            error (EXIT_FAILURE, 0, _("division by zero"));
          got_سائز = true;
          توڑو;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

        طےشدہ:
          usage (EXIT_FAILURE);
        }
    }

  argv += optind;
  argc -= optind;

  /* must specify either size or reference file */
  اگر (!ref_file && !got_سائز)
    {
      error (0, 0, _("you must specify either %s or %s"),
             quote_n (0, "--size"), quote_n (1, "--reference"));
      usage (EXIT_FAILURE);
    }
  /* must specify a relative size with a reference file */
  اگر (ref_file && got_سائز && !rel_منوال)
    {
      error (0, 0, _("you must specify a relative %s with %s"),
             quote_n (0, "--size"), quote_n (1, "--reference"));
      usage (EXIT_FAILURE);
    }
  /* block_mode without size is not valid */
  اگر (بلاک_منوال && !got_سائز)
    {
      error (0, 0, _("%s was specified but %s was not"),
             quote_n (0, "--io-blocks"), quote_n (1, "--size"));
      usage (EXIT_FAILURE);
    }
  /* must specify at least 1 file */
  اگر (argc < 1)
    {
      error (0, 0, _("missing file operand"));
      usage (EXIT_FAILURE);
    }

  اگر (ref_file)
    {
      ساخت stat sb;
      off_t file_سائز = -1;
      اگر (stat (ref_file, &sb) != 0)
        error (EXIT_FAILURE, errno, _("cannot stat %s"), quoteaf (ref_file));
      اگر (usable_st_سائز (&sb))
        file_سائز = sb.st_سائز;
      ورنہ
        {
          /* O_NONBLOCK so we do not hang forever on a FIFO with no writer.  */
          صحیح ref_fd = open (ref_file, O_RDONLY | O_NONBLOCK);
          اگر (0 <= ref_fd)
            {
              off_t file_انتہا = lseek (ref_fd, 0, SEEK_END);
              صحیح saved_errno = errno;
              close (ref_fd); /* ignore failure */
              اگر (0 <= file_انتہا)
                file_سائز = file_انتہا;
              ورنہ
                {
                  /* restore, in case close clobbered it. */
                  errno = saved_errno;
                }
            }
        }
      اگر (file_سائز < 0)
        error (EXIT_FAILURE, errno, _("cannot get the size of %s"),
               quoteaf (ref_file));
      اگر (!got_سائز)
        سائز = file_سائز;
      ورنہ
        rsize = file_سائز;
    }

  صحیح oflags = O_WRONLY | (no_create ? 0 : O_CREAT) | O_NONBLOCK;
  bool errors = false;

  برائے (حرف ثابت *fname; (fname = *argv); argv++)
    {
      صحیح fd = open (fname, oflags, منوال_RW_UGO);
      اگر (fd < 0)
        {
          /* 'truncate -s0 -c no-such-file'  shouldn't gen error
             'truncate -s0 no-such-dir/file' should gen ENOENT error
             'truncate -s0 no-such-dir/' should gen EISDIR error
             'truncate -s0 .' should gen EISDIR error */
          اگر (!(no_create && errno == ENOENT))
            {
              error (0, errno, _("cannot open %s for writing"),
                     quoteaf (fname));
              errors = true;
            }
        }
      ورنہ
        {
          errors |= !do_ftruncate (fd, fname, سائز, rsize, rel_منوال);
          اگر (close (fd) != 0)
            {
              error (0, errno, _("failed to close %s"), quoteaf (fname));
              errors = true;
            }
        }
    }

  واپس errors ? EXIT_FAILURE : EXIT_SUCCESS;
}
