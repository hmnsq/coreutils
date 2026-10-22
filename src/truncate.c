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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>             /* sets _FILE_OFFSET_BITS=64 etc. */
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "stat-size.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "truncate"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

/* (-c) If true, don't create if not already there */
ਸਥਿਰ bool no_create;

/* (-o) If true, --size refers to blocks not bytes */
ਸਥਿਰ bool ਖੰਡ_ਬਹੁਲਕ;

/* (-r) Reference file to use size from */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ref_file;

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ longopts[] =
{
  {"no-create", no_ਦਲੀਲ, ਨਿਰਾ, 'c'},
  {"io-blocks", no_ਦਲੀਲ, ਨਿਰਾ, 'o'},
  {"reference", required_ਦਲੀਲ, ਨਿਰਾ, 'r'},
  {"size", required_ਦਲੀਲ, ਨਿਰਾ, 's'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਗਿਣਤੀ
{ rm_abs = 0, rm_rel, rm_min, rm_max, rm_rdn, rm_rup } rel_ਬਹੁਲਕ_t;

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("Usage: %s OPTION... FILE...\n"), ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
Shrink or extend the ਮਾਪ ਦਾ each FILE ਮੰਜ਼ਿਲ the specified ਮਾਪ\n\
\n\
A FILE ਦਲੀਲ that does not exist is created.\n\
\n\
If a FILE is larger than the specified ਮਾਪ, the extra ਅੰਕੜੇ is ਗੁਆਚਾ.\n\
If a FILE is shorter, it is extended and the sparse extended part (hole)\n\
reads as zero ਬਾਈਟਾਂ.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -c, --no-create        ਕਰੋ not create any files\n\
"));
      oputs (_("\
  -o, --io-ਖੰਡਾਂ        treat ਮਾਪ_2 as ਸੰਖਿਆ ਦਾ IO ਖੰਡਾਂ instead ਦਾ ਬਾਈਟਾਂ\n\
"));
      oputs (_("\
  -r, --reference=RFILE  ਅਧਾਰ_2 ਮਾਪ on RFILE\n\
"));
      oputs (_("\
  -s, --ਮਾਪ=ਮਾਪ_2        ਸੈੱਟ_2 or adjust the file ਮਾਪ by ਮਾਪ_2 ਬਾਈਟਾਂ\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      emit_ਮਾਪ_note ();
      fputs (_("\n\
ਮਾਪ_2 may also be prefixed by one ਦਾ the following modifying characters:\n\
'+' extend by, '-' reduce by, '<' at most, '>' at ਘੱਟ_ਤੋਂ_ਘੱਟ,\n\
'/' ਗੇੜਾ_2 ਥੱਲੇ ਮੰਜ਼ਿਲ ਗੁਣਜ ਦਾ, '%' ਗੇੜਾ_2 ਉੱਪਰ ਮੰਜ਼ਿਲ ਗੁਣਜ ਦਾ.\n"), stdout);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

/* return true on success, false on error.  */
ਸਥਿਰ bool
do_ftruncate (ਪੂਰਨ fd, ਅੱਖਰ ਅਚਲ *fname, off_t ssize, off_t rsize,
              rel_ਬਹੁਲਕ_t rel_ਬਹੁਲਕ)
{
  ਬਣਤਰ stat sb;
  off_t nsize;

  ਜੇ ((ਖੰਡ_ਬਹੁਲਕ || (rel_ਬਹੁਲਕ && rsize < 0)) && fstat (fd, &sb) != 0)
    {
      error (0, errno, _("cannot fstat %s"), quoteaf (fname));
      ਵਾਪਸ false;
    }
  ਜੇ (ਖੰਡ_ਬਹੁਲਕ)
    {
      ptrdiff_t blksize = STP_BLKSIZE (&sb);
      intmax_t ssize0 = ssize;
      ਜੇ (ckd_mul (&ssize, ssize, blksize))
        {
          error (0, 0,
                 _("overflow in %jd * %td byte blocks for file %s"),
                 ssize0, blksize, quoteaf (fname));
          ਵਾਪਸ false;
        }
    }
  ਜੇ (rel_ਬਹੁਲਕ)
    {
      off_t fsize;

      ਜੇ (0 <= rsize)
        fsize = rsize;
      ਨਹੀਂਤਾਂ
        {
          ਜੇ (usable_st_ਮਾਪ (&sb))
            {
              fsize = sb.st_ਮਾਪ;
              ਜੇ (fsize < 0)
                {
                  /* Sanity check.  Overflow is the only reason I can think
                     this would ever go negative. */
                  error (0, 0, _("%s has unusable, apparently negative size"),
                         quoteaf (fname));
                  ਵਾਪਸ false;
                }
            }
          ਨਹੀਂਤਾਂ
            {
              fsize = lseek (fd, 0, SEEK_END);
              ਜੇ (fsize < 0)
                {
                  error (0, errno, _("cannot get the size of %s"),
                         quoteaf (fname));
                  ਵਾਪਸ false;
                }
            }
        }

      ਜੇ (rel_ਬਹੁਲਕ == rm_min)
        nsize = MAX (fsize, ssize);
      ਨਹੀਂਤਾਂ ਜੇ (rel_ਬਹੁਲਕ == rm_max)
        nsize = MIN (fsize, ssize);
      ਨਹੀਂਤਾਂ ਜੇ (rel_ਬਹੁਲਕ == rm_rdn)
        /* 0..ssize-1 -> 0 */
        nsize = fsize - fsize % ssize;
      ਨਹੀਂਤਾਂ
        {
          ਜੇ (rel_ਬਹੁਲਕ == rm_rup)
            {
              /* 1..ssize -> ssize */
              off_t r = fsize % ssize;
              ssize = r == 0 ? 0 : ssize - r;
            }
          ਜੇ (ckd_ਜੋੜੋ (&nsize, fsize, ssize))
            {
              error (0, 0, _("overflow extending size of file %s"),
                     quoteaf (fname));
              ਵਾਪਸ false;
            }
        }
    }
  ਨਹੀਂਤਾਂ
    nsize = ssize;
  ਜੇ (nsize < 0)
    nsize = 0;

  ਜੇ (ftruncate (fd, nsize) != 0)
    {
      error (0, errno, _("failed to truncate %s at %jd bytes"),
             quoteaf (fname), (intmax_t) {nsize});
      ਵਾਪਸ false;
    }

  ਵਾਪਸ true;
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  bool got_ਮਾਪ = false;
  off_t ਮਾਪ IF_LINT ( = 0);
  off_t rsize = -1;
  rel_ਬਹੁਲਕ_t rel_ਬਹੁਲਕ = rm_abs;
  ਪੂਰਨ c;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ਜਦੋਂ ((c = getopt_long (argc, argv, "cor:s:", longopts, ਨਿਰਾ)) != -1)
    {
      ਚੋਣ (c)
        {
        ਹਾਲਤ 'c':
          no_create = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'o':
          ਖੰਡ_ਬਹੁਲਕ = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'r':
          ref_file = optarg;
          ਰੋਕੋ;

        ਹਾਲਤ 's':
          /* skip any whitespace */
          ਜਦੋਂ (ਖਾਲੀ_ਥਾਂ_ਹੈ (ਮੰਜ਼ਿਲ_uchar (*optarg)))
            optarg++;
          ਚੋਣ (*optarg)
            {
            ਹਾਲਤ '<':
              rel_ਬਹੁਲਕ = rm_max;
              optarg++;
              ਰੋਕੋ;
            ਹਾਲਤ '>':
              rel_ਬਹੁਲਕ = rm_min;
              optarg++;
              ਰੋਕੋ;
            ਹਾਲਤ '/':
              rel_ਬਹੁਲਕ = rm_rdn;
              optarg++;
              ਰੋਕੋ;
            ਹਾਲਤ '%':
              rel_ਬਹੁਲਕ = rm_rup;
              optarg++;
              ਰੋਕੋ;
            }
          /* skip any whitespace */
          ਜਦੋਂ (ਖਾਲੀ_ਥਾਂ_ਹੈ (ਮੰਜ਼ਿਲ_uchar (*optarg)))
            optarg++;
          ਜੇ (*optarg == '+' || *optarg == '-')
            {
              ਜੇ (rel_ਬਹੁਲਕ)
                {
                  error (0, 0, _("multiple relative modifiers specified"));
                  /* Note other combinations are flagged as invalid numbers */
                  usage (EXIT_FAILURE);
                }
              rel_ਬਹੁਲਕ = rm_rel;
            }
          /* Support dd BLOCK size suffixes + lowercase g,t,m for bsd compat.
             Note we don't support dd's b=512, c=1, w=2 or 21x512MiB formats. */
          ਮਾਪ = xdectoimax (optarg, OFF_T_MIN, OFF_T_MAX, "EgGkKmMPQRtTYZ0",
                             _("Invalid number"), 0);
          /* Rounding to multiple of 0 is nonsensical */
          ਜੇ ((rel_ਬਹੁਲਕ == rm_rup || rel_ਬਹੁਲਕ == rm_rdn) && ਮਾਪ == 0)
            error (EXIT_FAILURE, 0, _("division by zero"));
          got_ਮਾਪ = true;
          ਰੋਕੋ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);

        ਮੂਲ:
          usage (EXIT_FAILURE);
        }
    }

  argv += optind;
  argc -= optind;

  /* must specify either size or reference file */
  ਜੇ (!ref_file && !got_ਮਾਪ)
    {
      error (0, 0, _("you must specify either %s or %s"),
             quote_n (0, "--size"), quote_n (1, "--reference"));
      usage (EXIT_FAILURE);
    }
  /* must specify a relative size with a reference file */
  ਜੇ (ref_file && got_ਮਾਪ && !rel_ਬਹੁਲਕ)
    {
      error (0, 0, _("you must specify a relative %s with %s"),
             quote_n (0, "--size"), quote_n (1, "--reference"));
      usage (EXIT_FAILURE);
    }
  /* block_mode without size is not valid */
  ਜੇ (ਖੰਡ_ਬਹੁਲਕ && !got_ਮਾਪ)
    {
      error (0, 0, _("%s was specified but %s was not"),
             quote_n (0, "--io-blocks"), quote_n (1, "--size"));
      usage (EXIT_FAILURE);
    }
  /* must specify at least 1 file */
  ਜੇ (argc < 1)
    {
      error (0, 0, _("missing file operand"));
      usage (EXIT_FAILURE);
    }

  ਜੇ (ref_file)
    {
      ਬਣਤਰ stat sb;
      off_t file_ਮਾਪ = -1;
      ਜੇ (stat (ref_file, &sb) != 0)
        error (EXIT_FAILURE, errno, _("cannot stat %s"), quoteaf (ref_file));
      ਜੇ (usable_st_ਮਾਪ (&sb))
        file_ਮਾਪ = sb.st_ਮਾਪ;
      ਨਹੀਂਤਾਂ
        {
          /* O_NONBLOCK so we do not hang forever on a FIFO with no writer.  */
          ਪੂਰਨ ref_fd = open (ref_file, O_RDONLY | O_NONBLOCK);
          ਜੇ (0 <= ref_fd)
            {
              off_t file_ਸਿਰਾ = lseek (ref_fd, 0, SEEK_END);
              ਪੂਰਨ saved_errno = errno;
              close (ref_fd); /* ignore failure */
              ਜੇ (0 <= file_ਸਿਰਾ)
                file_ਮਾਪ = file_ਸਿਰਾ;
              ਨਹੀਂਤਾਂ
                {
                  /* restore, in case close clobbered it. */
                  errno = saved_errno;
                }
            }
        }
      ਜੇ (file_ਮਾਪ < 0)
        error (EXIT_FAILURE, errno, _("cannot get the size of %s"),
               quoteaf (ref_file));
      ਜੇ (!got_ਮਾਪ)
        ਮਾਪ = file_ਮਾਪ;
      ਨਹੀਂਤਾਂ
        rsize = file_ਮਾਪ;
    }

  ਪੂਰਨ oflags = O_WRONLY | (no_create ? 0 : O_CREAT) | O_NONBLOCK;
  bool errors = false;

  ਲਈ (ਅੱਖਰ ਅਚਲ *fname; (fname = *argv); argv++)
    {
      ਪੂਰਨ fd = open (fname, oflags, ਬਹੁਲਕ_RW_UGO);
      ਜੇ (fd < 0)
        {
          /* 'truncate -s0 -c no-such-file'  shouldn't gen error
             'truncate -s0 no-such-dir/file' should gen ENOENT error
             'truncate -s0 no-such-dir/' should gen EISDIR error
             'truncate -s0 .' should gen EISDIR error */
          ਜੇ (!(no_create && errno == ENOENT))
            {
              error (0, errno, _("cannot open %s for writing"),
                     quoteaf (fname));
              errors = true;
            }
        }
      ਨਹੀਂਤਾਂ
        {
          errors |= !do_ftruncate (fd, fname, ਮਾਪ, rsize, rel_ਬਹੁਲਕ);
          ਜੇ (close (fd) != 0)
            {
              error (0, errno, _("failed to close %s"), quoteaf (fname));
              errors = true;
            }
        }
    }

  ਵਾਪਸ errors ? EXIT_FAILURE : EXIT_SUCCESS;
}
