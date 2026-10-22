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

#समावेश <config.h>             /* sets _FILE_OFFSET_BITS=64 etc. */
#समावेश <stdio.h>
#समावेश <getopt.h>
#समावेश <sys/types.h>

#समावेश "system.h"
#समावेश "quote.h"
#समावेश "stat-size.h"
#समावेश "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "truncate"

#घोषणा AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

/* (-c) If true, don't create if not already there */
अटल bool no_create;

/* (-o) If true, --size refers to blocks not bytes */
अटल bool block_mode;

/* (-r) Reference file to use size from */
अटल अक्षर स्थिर *ref_file;

अटल काष्ठा option स्थिर longopts[] =
{
  {"no-create", no_argument, शून्य, 'c'},
  {"io-blocks", no_argument, शून्य, 'o'},
  {"reference", required_argument, शून्य, 'r'},
  {"size", required_argument, शून्य, 's'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {शून्य, 0, शून्य, 0}
};

प्रकार क्रमागत
{ rm_abs = 0, rm_rel, rm_min, rm_max, rm_rdn, rm_rup } rel_mode_t;

व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("Usage: %s OPTION... FILE...\n"), program_name);
      ख_माला_दो (_("\
Shrink or extend the आकार of each खाता to the specified आकार\n\
\n\
A खाता argument that does not exist is created.\n\
\n\
If a खाता is larger than the specified आकार, the extra तथ्य is lost.\n\
If a खाता is shorter, it is extended and the sparse extended part (hole)\n\
reads as zero bytes.\n\
"), मानक_निकास);

      emit_mandatory_arg_note ();

      oputs (_("\
  -c, --no-create        करो not create any files\n\
"));
      oputs (_("\
  -o, --io-blocks        treat आकार_2 as संख्या of IO blocks instead of bytes\n\
"));
      oputs (_("\
  -r, --reference=RFILE  base आकार on RFILE\n\
"));
      oputs (_("\
  -s, --आकार=आकार_2        set or adjust the file आकार by आकार_2 bytes\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_आकार_note ();
      ख_माला_दो (_("\n\
आकार_2 may also be prefixed by one of the following modifying characters:\n\
'+' extend by, '-' reduce by, '<' at most, '>' at least,\n\
'/' round down to multiple of, '%' round up to multiple of.\n"), मानक_निकास);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

/* return true on success, false on error.  */
अटल bool
do_ftruncate (पूर्णांक fd, अक्षर स्थिर *fname, off_t ssize, off_t rsize,
              rel_mode_t rel_mode)
{
  काष्ठा stat sb;
  off_t nsize;

  अगर ((block_mode || (rel_mode && rsize < 0)) && ख_स्थिति (fd, &sb) != 0)
    {
      error (0, त्रुटि_सं, _("cannot fstat %s"), quoteaf (fname));
      वापस false;
    }
  अगर (block_mode)
    {
      सूचक_भेद_प्रकार blksize = STP_BLKSIZE (&sb);
      intmax_t ssize0 = ssize;
      अगर (ckd_mul (&ssize, ssize, blksize))
        {
          error (0, 0,
                 _("overflow in %jd * %td byte blocks for file %s"),
                 ssize0, blksize, quoteaf (fname));
          वापस false;
        }
    }
  अगर (rel_mode)
    {
      off_t fsize;

      अगर (0 <= rsize)
        fsize = rsize;
      अन्यथा
        {
          अगर (usable_st_आकार (&sb))
            {
              fsize = sb.st_आकार;
              अगर (fsize < 0)
                {
                  /* Sanity check.  Overflow is the only reason I can think
                     this would ever go negative. */
                  error (0, 0, _("%s has unusable, apparently negative size"),
                         quoteaf (fname));
                  वापस false;
                }
            }
          अन्यथा
            {
              fsize = lseek (fd, 0, अंत_से);
              अगर (fsize < 0)
                {
                  error (0, त्रुटि_सं, _("cannot get the size of %s"),
                         quoteaf (fname));
                  वापस false;
                }
            }
        }

      अगर (rel_mode == rm_min)
        nsize = MAX (fsize, ssize);
      अन्यथा अगर (rel_mode == rm_max)
        nsize = MIN (fsize, ssize);
      अन्यथा अगर (rel_mode == rm_rdn)
        /* 0..ssize-1 -> 0 */
        nsize = fsize - fsize % ssize;
      अन्यथा
        {
          अगर (rel_mode == rm_rup)
            {
              /* 1..ssize -> ssize */
              off_t r = fsize % ssize;
              ssize = r == 0 ? 0 : ssize - r;
            }
          अगर (ckd_add (&nsize, fsize, ssize))
            {
              error (0, 0, _("overflow extending size of file %s"),
                     quoteaf (fname));
              वापस false;
            }
        }
    }
  अन्यथा
    nsize = ssize;
  अगर (nsize < 0)
    nsize = 0;

  अगर (ftruncate (fd, nsize) != 0)
    {
      error (0, त्रुटि_सं, _("failed to truncate %s at %jd bytes"),
             quoteaf (fname), (intmax_t) {nsize});
      वापस false;
    }

  वापस true;
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  bool got_आकार = false;
  off_t आकार IF_LINT ( = 0);
  off_t rsize = -1;
  rel_mode_t rel_mode = rm_abs;
  पूर्णांक c;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  जबतक ((c = getopt_long (argc, argv, "cor:s:", longopts, शून्य)) != -1)
    {
      चयन (c)
        {
        हाल 'c':
          no_create = true;
          अवरोध;

        हाल 'o':
          block_mode = true;
          अवरोध;

        हाल 'r':
          ref_file = optarg;
          अवरोध;

        हाल 's':
          /* skip any whitespace */
          जबतक (है_खाली (to_uchar (*optarg)))
            optarg++;
          चयन (*optarg)
            {
            हाल '<':
              rel_mode = rm_max;
              optarg++;
              अवरोध;
            हाल '>':
              rel_mode = rm_min;
              optarg++;
              अवरोध;
            हाल '/':
              rel_mode = rm_rdn;
              optarg++;
              अवरोध;
            हाल '%':
              rel_mode = rm_rup;
              optarg++;
              अवरोध;
            }
          /* skip any whitespace */
          जबतक (है_खाली (to_uchar (*optarg)))
            optarg++;
          अगर (*optarg == '+' || *optarg == '-')
            {
              अगर (rel_mode)
                {
                  error (0, 0, _("multiple relative modifiers specified"));
                  /* Note other combinations are flagged as invalid numbers */
                  usage (निकास_त्रुटि);
                }
              rel_mode = rm_rel;
            }
          /* Support dd BLOCK size suffixes + lowercase g,t,m for bsd compat.
             Note we don't support dd's b=512, c=1, w=2 or 21x512MiB formats. */
          आकार = xdectoimax (optarg, OFF_T_MIN, OFF_T_MAX, "EgGkKmMPQRtTYZ0",
                             _("Invalid number"), 0);
          /* Rounding to multiple of 0 is nonsensical */
          अगर ((rel_mode == rm_rup || rel_mode == rm_rdn) && आकार == 0)
            error (निकास_त्रुटि, 0, _("division by zero"));
          got_आकार = true;
          अवरोध;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        शेष:
          usage (निकास_त्रुटि);
        }
    }

  argv += optind;
  argc -= optind;

  /* must specify either size or reference file */
  अगर (!ref_file && !got_आकार)
    {
      error (0, 0, _("you must specify either %s or %s"),
             quote_n (0, "--size"), quote_n (1, "--reference"));
      usage (निकास_त्रुटि);
    }
  /* must specify a relative size with a reference file */
  अगर (ref_file && got_आकार && !rel_mode)
    {
      error (0, 0, _("you must specify a relative %s with %s"),
             quote_n (0, "--size"), quote_n (1, "--reference"));
      usage (निकास_त्रुटि);
    }
  /* block_mode without size is not valid */
  अगर (block_mode && !got_आकार)
    {
      error (0, 0, _("%s was specified but %s was not"),
             quote_n (0, "--io-blocks"), quote_n (1, "--size"));
      usage (निकास_त्रुटि);
    }
  /* must specify at least 1 file */
  अगर (argc < 1)
    {
      error (0, 0, _("missing file operand"));
      usage (निकास_त्रुटि);
    }

  अगर (ref_file)
    {
      काष्ठा stat sb;
      off_t file_आकार = -1;
      अगर (stat (ref_file, &sb) != 0)
        error (निकास_त्रुटि, त्रुटि_सं, _("cannot stat %s"), quoteaf (ref_file));
      अगर (usable_st_आकार (&sb))
        file_आकार = sb.st_आकार;
      अन्यथा
        {
          /* O_NONBLOCK so we do not hang forever on a FIFO with no writer.  */
          पूर्णांक ref_fd = खोलो (ref_file, O_RDONLY | O_NONBLOCK);
          अगर (0 <= ref_fd)
            {
              off_t file_end = lseek (ref_fd, 0, अंत_से);
              पूर्णांक saved_errno = त्रुटि_सं;
              बंद (ref_fd); /* ignore failure */
              अगर (0 <= file_end)
                file_आकार = file_end;
              अन्यथा
                {
                  /* restore, in case close clobbered it. */
                  त्रुटि_सं = saved_errno;
                }
            }
        }
      अगर (file_आकार < 0)
        error (निकास_त्रुटि, त्रुटि_सं, _("cannot get the size of %s"),
               quoteaf (ref_file));
      अगर (!got_आकार)
        आकार = file_आकार;
      अन्यथा
        rsize = file_आकार;
    }

  पूर्णांक oflags = O_WRONLY | (no_create ? 0 : O_CREAT) | O_NONBLOCK;
  bool errors = false;

  क्रम (अक्षर स्थिर *fname; (fname = *argv); argv++)
    {
      पूर्णांक fd = खोलो (fname, oflags, MODE_RW_UGO);
      अगर (fd < 0)
        {
          /* 'truncate -s0 -c no-such-file'  shouldn't gen error
             'truncate -s0 no-such-dir/file' should gen ENOENT error
             'truncate -s0 no-such-dir/' should gen EISDIR error
             'truncate -s0 .' should gen EISDIR error */
          अगर (!(no_create && त्रुटि_सं == ENOENT))
            {
              error (0, त्रुटि_सं, _("cannot open %s for writing"),
                     quoteaf (fname));
              errors = true;
            }
        }
      अन्यथा
        {
          errors |= !do_ftruncate (fd, fname, आकार, rsize, rel_mode);
          अगर (बंद (fd) != 0)
            {
              error (0, त्रुटि_सं, _("failed to close %s"), quoteaf (fname));
              errors = true;
            }
        }
    }

  वापस errors ? निकास_त्रुटि : निकास_सफल;
}
