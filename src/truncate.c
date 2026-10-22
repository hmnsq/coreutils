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

#ಅನ್ತರ್ಭಾವ <config.h>             /* sets _FILE_OFFSET_BITS=64 etc. */
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "stat-size.h"
#ಅನ್ತರ್ಭಾವ "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "truncate"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

/* (-c) If true, don't create if not already there */
ಸ್ಥಿರ bool no_create;

/* (-o) If true, --size refers to blocks not bytes */
ಸ್ಥಿರ bool ಖಂಡ_ಬಹುಲಕ;

/* (-r) Reference file to use size from */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ref_file;

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ longopts[] =
{
  {"no-create", no_ವಾದ, ಖಾಲಿ, 'c'},
  {"io-blocks", no_ವಾದ, ಖಾಲಿ, 'o'},
  {"reference", required_ವಾದ, ಖಾಲಿ, 'r'},
  {"size", required_ವಾದ, ಖಾಲಿ, 's'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಎಣಿಕೆ
{ rm_abs = 0, rm_rel, rm_min, rm_max, rm_rdn, rm_rup } rel_ಬಹುಲಕ_t;

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("Usage: %s OPTION... FILE...\n"), ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
Shrink or extend the ಗಾತ್ರ_2 ದ each FILE ಗಮ್ಯ the specified ಗಾತ್ರ_2\n\
\n\
A FILE ವಾದ that does not exist is created.\n\
\n\
If a FILE is larger than the specified ಗಾತ್ರ_2, the extra ದತ್ತಾಂಶ is ಕಳೆದುಹೋದ.\n\
If a FILE is shorter, it is extended and the sparse extended part (hole)\n\
reads as zero ಬೈಟುಗಳು.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -c, --no-create        ಮಾಡು not create any files\n\
"));
      oputs (_("\
  -o, --io-ಖಂಡಗಳು        treat ಗಾತ್ರ_3 as ಸಂಖ್ಯೆ ದ IO ಖಂಡಗಳು instead ದ ಬೈಟುಗಳು\n\
"));
      oputs (_("\
  -r, --reference=RFILE  ತಳ_2 ಗಾತ್ರ_2 on RFILE\n\
"));
      oputs (_("\
  -s, --ಗಾತ್ರ_2=ಗಾತ್ರ_3        ಗಣ_2 or adjust the file ಗಾತ್ರ_2 by ಗಾತ್ರ_3 ಬೈಟುಗಳು\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      emit_ಗಾತ್ರ_note ();
      fputs (_("\n\
ಗಾತ್ರ_3 may also be prefixed by one ದ the following modifying characters:\n\
'+' extend by, '-' reduce by, '<' at most, '>' at ಕನಿಷ್ಠ,\n\
'/' ಸುತ್ತು_2 ಕೆಳಕ್ಕೆ ಗಮ್ಯ ಅಪವರ್ತ್ಯ ದ, '%' ಸುತ್ತು_2 ಮೇಲೆ ಗಮ್ಯ ಅಪವರ್ತ್ಯ ದ.\n"), stdout);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

/* return true on success, false on error.  */
ಸ್ಥಿರ bool
do_ftruncate (ಪೂರ್ಣಾಂಕ fd, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *fname, off_t ssize, off_t rsize,
              rel_ಬಹುಲಕ_t rel_ಬಹುಲಕ)
{
  ರಚನೆ stat sb;
  off_t nsize;

  ಆದರೆ ((ಖಂಡ_ಬಹುಲಕ || (rel_ಬಹುಲಕ && rsize < 0)) && fstat (fd, &sb) != 0)
    {
      error (0, errno, _("cannot fstat %s"), quoteaf (fname));
      ಹಿಂತಿರುಗಿ false;
    }
  ಆದರೆ (ಖಂಡ_ಬಹುಲಕ)
    {
      ptrdiff_t blksize = STP_BLKSIZE (&sb);
      intmax_t ssize0 = ssize;
      ಆದರೆ (ckd_mul (&ssize, ssize, blksize))
        {
          error (0, 0,
                 _("overflow in %jd * %td byte blocks for file %s"),
                 ssize0, blksize, quoteaf (fname));
          ಹಿಂತಿರುಗಿ false;
        }
    }
  ಆದರೆ (rel_ಬಹುಲಕ)
    {
      off_t fsize;

      ಆದರೆ (0 <= rsize)
        fsize = rsize;
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಆದರೆ (usable_st_ಗಾತ್ರ (&sb))
            {
              fsize = sb.st_ಗಾತ್ರ;
              ಆದರೆ (fsize < 0)
                {
                  /* Sanity check.  Overflow is the only reason I can think
                     this would ever go negative. */
                  error (0, 0, _("%s has unusable, apparently negative size"),
                         quoteaf (fname));
                  ಹಿಂತಿರುಗಿ false;
                }
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              fsize = lseek (fd, 0, SEEK_END);
              ಆದರೆ (fsize < 0)
                {
                  error (0, errno, _("cannot get the size of %s"),
                         quoteaf (fname));
                  ಹಿಂತಿರುಗಿ false;
                }
            }
        }

      ಆದರೆ (rel_ಬಹುಲಕ == rm_min)
        nsize = MAX (fsize, ssize);
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (rel_ಬಹುಲಕ == rm_max)
        nsize = MIN (fsize, ssize);
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (rel_ಬಹುಲಕ == rm_rdn)
        /* 0..ssize-1 -> 0 */
        nsize = fsize - fsize % ssize;
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಆದರೆ (rel_ಬಹುಲಕ == rm_rup)
            {
              /* 1..ssize -> ssize */
              off_t r = fsize % ssize;
              ssize = r == 0 ? 0 : ssize - r;
            }
          ಆದರೆ (ckd_ಕೂಡಿಸು (&nsize, fsize, ssize))
            {
              error (0, 0, _("overflow extending size of file %s"),
                     quoteaf (fname));
              ಹಿಂತಿರುಗಿ false;
            }
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ
    nsize = ssize;
  ಆದರೆ (nsize < 0)
    nsize = 0;

  ಆದರೆ (ftruncate (fd, nsize) != 0)
    {
      error (0, errno, _("failed to truncate %s at %jd bytes"),
             quoteaf (fname), (intmax_t) {nsize});
      ಹಿಂತಿರುಗಿ false;
    }

  ಹಿಂತಿರುಗಿ true;
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  bool got_ಗಾತ್ರ = false;
  off_t ಗಾತ್ರ_2 IF_LINT ( = 0);
  off_t rsize = -1;
  rel_ಬಹುಲಕ_t rel_ಬಹುಲಕ = rm_abs;
  ಪೂರ್ಣಾಂಕ c;

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಆಗ ((c = getopt_long (argc, argv, "cor:s:", longopts, ಖಾಲಿ)) != -1)
    {
      ಆಯ್ಕೆ (c)
        {
        ಸ್ಥಿತಿ 'c':
          no_create = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'o':
          ಖಂಡ_ಬಹುಲಕ = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'r':
          ref_file = optarg;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 's':
          /* skip any whitespace */
          ಆಗ (ಖಾಲಿಯೇ (ಗಮ್ಯ_uchar (*optarg)))
            optarg++;
          ಆಯ್ಕೆ (*optarg)
            {
            ಸ್ಥಿತಿ '<':
              rel_ಬಹುಲಕ = rm_max;
              optarg++;
              ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ '>':
              rel_ಬಹುಲಕ = rm_min;
              optarg++;
              ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ '/':
              rel_ಬಹುಲಕ = rm_rdn;
              optarg++;
              ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ '%':
              rel_ಬಹುಲಕ = rm_rup;
              optarg++;
              ನಿಲ್ಲಿಸು;
            }
          /* skip any whitespace */
          ಆಗ (ಖಾಲಿಯೇ (ಗಮ್ಯ_uchar (*optarg)))
            optarg++;
          ಆದರೆ (*optarg == '+' || *optarg == '-')
            {
              ಆದರೆ (rel_ಬಹುಲಕ)
                {
                  error (0, 0, _("multiple relative modifiers specified"));
                  /* Note other combinations are flagged as invalid numbers */
                  usage (EXIT_FAILURE);
                }
              rel_ಬಹುಲಕ = rm_rel;
            }
          /* Support dd BLOCK size suffixes + lowercase g,t,m for bsd compat.
             Note we don't support dd's b=512, c=1, w=2 or 21x512MiB formats. */
          ಗಾತ್ರ_2 = xdectoimax (optarg, OFF_T_MIN, OFF_T_MAX, "EgGkKmMPQRtTYZ0",
                             _("Invalid number"), 0);
          /* Rounding to multiple of 0 is nonsensical */
          ಆದರೆ ((rel_ಬಹುಲಕ == rm_rup || rel_ಬಹುಲಕ == rm_rdn) && ಗಾತ್ರ_2 == 0)
            error (EXIT_FAILURE, 0, _("division by zero"));
          got_ಗಾತ್ರ = true;
          ನಿಲ್ಲಿಸು;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);

        ಪೂರ್ವನಿಯೋಜಿತ:
          usage (EXIT_FAILURE);
        }
    }

  argv += optind;
  argc -= optind;

  /* must specify either size or reference file */
  ಆದರೆ (!ref_file && !got_ಗಾತ್ರ)
    {
      error (0, 0, _("you must specify either %s or %s"),
             quote_n (0, "--size"), quote_n (1, "--reference"));
      usage (EXIT_FAILURE);
    }
  /* must specify a relative size with a reference file */
  ಆದರೆ (ref_file && got_ಗಾತ್ರ && !rel_ಬಹುಲಕ)
    {
      error (0, 0, _("you must specify a relative %s with %s"),
             quote_n (0, "--size"), quote_n (1, "--reference"));
      usage (EXIT_FAILURE);
    }
  /* block_mode without size is not valid */
  ಆದರೆ (ಖಂಡ_ಬಹುಲಕ && !got_ಗಾತ್ರ)
    {
      error (0, 0, _("%s was specified but %s was not"),
             quote_n (0, "--io-blocks"), quote_n (1, "--size"));
      usage (EXIT_FAILURE);
    }
  /* must specify at least 1 file */
  ಆದರೆ (argc < 1)
    {
      error (0, 0, _("missing file operand"));
      usage (EXIT_FAILURE);
    }

  ಆದರೆ (ref_file)
    {
      ರಚನೆ stat sb;
      off_t file_ಗಾತ್ರ = -1;
      ಆದರೆ (stat (ref_file, &sb) != 0)
        error (EXIT_FAILURE, errno, _("cannot stat %s"), quoteaf (ref_file));
      ಆದರೆ (usable_st_ಗಾತ್ರ (&sb))
        file_ಗಾತ್ರ = sb.st_ಗಾತ್ರ;
      ಇಲ್ಲದಿದ್ದರೆ
        {
          /* O_NONBLOCK so we do not hang forever on a FIFO with no writer.  */
          ಪೂರ್ಣಾಂಕ ref_fd = open (ref_file, O_RDONLY | O_NONBLOCK);
          ಆದರೆ (0 <= ref_fd)
            {
              off_t file_ಕೊನೆ = lseek (ref_fd, 0, SEEK_END);
              ಪೂರ್ಣಾಂಕ saved_errno = errno;
              close (ref_fd); /* ignore failure */
              ಆದರೆ (0 <= file_ಕೊನೆ)
                file_ಗಾತ್ರ = file_ಕೊನೆ;
              ಇಲ್ಲದಿದ್ದರೆ
                {
                  /* restore, in case close clobbered it. */
                  errno = saved_errno;
                }
            }
        }
      ಆದರೆ (file_ಗಾತ್ರ < 0)
        error (EXIT_FAILURE, errno, _("cannot get the size of %s"),
               quoteaf (ref_file));
      ಆದರೆ (!got_ಗಾತ್ರ)
        ಗಾತ್ರ_2 = file_ಗಾತ್ರ;
      ಇಲ್ಲದಿದ್ದರೆ
        rsize = file_ಗಾತ್ರ;
    }

  ಪೂರ್ಣಾಂಕ oflags = O_WRONLY | (no_create ? 0 : O_CREAT) | O_NONBLOCK;
  bool errors = false;

  ಗಾಗಿ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *fname; (fname = *argv); argv++)
    {
      ಪೂರ್ಣಾಂಕ fd = open (fname, oflags, ಬಹುಲಕ_RW_UGO);
      ಆದರೆ (fd < 0)
        {
          /* 'truncate -s0 -c no-such-file'  shouldn't gen error
             'truncate -s0 no-such-dir/file' should gen ENOENT error
             'truncate -s0 no-such-dir/' should gen EISDIR error
             'truncate -s0 .' should gen EISDIR error */
          ಆದರೆ (!(no_create && errno == ENOENT))
            {
              error (0, errno, _("cannot open %s for writing"),
                     quoteaf (fname));
              errors = true;
            }
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          errors |= !do_ftruncate (fd, fname, ಗಾತ್ರ_2, rsize, rel_ಬಹುಲಕ);
          ಆದರೆ (close (fd) != 0)
            {
              error (0, errno, _("failed to close %s"), quoteaf (fname));
              errors = true;
            }
        }
    }

  ಹಿಂತಿರುಗಿ errors ? EXIT_FAILURE : EXIT_SUCCESS;
}
