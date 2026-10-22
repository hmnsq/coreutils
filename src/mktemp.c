/* Create a temporary file or directory, safely.
   Copyright (C) 2007-2026 Free Software Foundation, Inc.

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

/* Written by Jim Meyering and Eric Blake.  */

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <getopt.h>

#ଅନ୍ତର୍ଭାଵ "system.h"

#ଅନ୍ତର୍ଭାଵ "close-stream.h"
#ଅନ୍ତର୍ଭାଵ "filenamecat.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "tempname.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "mktemp"

#define AUTHORS \
  proper_name ("Jim Meyering"), \
  proper_name ("Eric Blake")

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *default_template = "tmp.XXXXXXXXXX";

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ଗଣନା
{
  SUFFIX_OPTION = CHAR_MAX + 1,
};

ସ୍ଥିର ସଂରଚନା option ଅଚଳ longopts[] =
{
  {"directory", no_argument, ଖାଲି, 'd'},
  {"quiet", no_argument, ଖାଲି, 'q'},
  {"dry-run", no_argument, ଖାଲି, 'u'},
  {"suffix", required_argument, ଖାଲି, SUFFIX_OPTION},
  {"tmpdir", optional_argument, ଖାଲି, 'p'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ଖାଲି, 0, ଖାଲି, 0}
};

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
    {
      printf (_("Usage: %s [OPTION]... [TEMPLATE]\n"), program_name);
      fputs (_("\
Create a temporary file or directory, safely, and print its name.\n\
TEMPLATE must contain at least 3 consecutive 'X's in last component.\n\
If TEMPLATE is not specified, use tmp.XXXXXXXXXX, and --tmpdir is implied.\n\
"), stdout);
      fputs (_("\
Files are created u+rw, and directories u+rwx, minus umask restrictions.\n\
"), stdout);
      fputs ("\n", stdout);
      oputs (_("\
  -d, --directory\n\
         create a directory, not a file\n\
"));
      oputs (_("\
  -u, --dry-run\n\
         କର not create anything; merely print a name (unsafe)\n\
"));
      oputs (_("\
  -q, --quiet\n\
         suppress diagnostics about file/dir-creation failure\n\
"));
      oputs (_("\
      --suffix=SUFF\n\
         append SUFF to TEMPLATE; SUFF must not contain a slash.\n\
         This option is implied ଯଦି TEMPLATE does not end in X\n\
"));
      oputs (_("\
  -p DIR, --tmpdir[=DIR]\n\
         interpret TEMPLATE relative to DIR;\n\
         ଯଦି DIR is not specified, use $TMPDIR ଯଦି set, ଅନ୍ୟଥା /tmp.\n\
         With this option, TEMPLATE must not be an absolute name;\n\
         unlike with -t, TEMPLATE may contain slashes,\n\
         but mktemp creates only the final component\n\
"));
      oputs (_("\
  -t\n\
         interpret TEMPLATE as a single file name component,\n\
         relative to a directory: $TMPDIR, ଯଦି set;\n\
         ଅନ୍ୟଥା the directory specified via -p; ଅନ୍ୟଥା /tmp [deprecated]\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }

  exit (status);
}

ସ୍ଥିର size_t
ଗଣନା_consecutive_X_s (ଅକ୍ଷର ଅଚଳ *s, size_t len)
{
  size_t n = 0;
  ପାଇଁ ( ; len && s[len - 1] == 'X'; len--)
    ++n;
  ଫେରନ୍ତୁ n;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
mkstemp_len (ଅକ୍ଷର *tmpl, size_t suff_len, size_t x_len, bool dry_run)
{
  ଫେରନ୍ତୁ gen_tempname_len (tmpl, suff_len, 0, dry_run ? GT_NOCREATE : GT_FILE,
                           x_len);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
mkdtemp_len (ଅକ୍ଷର *tmpl, size_t suff_len, size_t x_len, bool dry_run)
{
  ଫେରନ୍ତୁ gen_tempname_len (tmpl, suff_len, 0, dry_run ? GT_NOCREATE : GT_DIR,
                           x_len);
}

/* True if we have already closed standard output.  */
ସ୍ଥିର bool stdout_closed;

/* Avoid closing stdout twice.  Since we conditionally call
   close_stream (stdout) in order to decide whether to clean up a
   temporary file, the exit hook needs to know whether to do all of
   close_stdout or just the stderr half.  */
ସ୍ଥିର ଶୂନ୍ୟ
maybe_close_stdout (ଶୂନ୍ୟ)
{
  ଯଦି (!stdout_closed)
    close_stdout ();
  ଅନ୍ୟଥା ଯଦି (close_stream (stderr) != 0)
    _exit (EXIT_FAILURE);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  ଅକ୍ଷର ଅଚଳ *dest_dir;
  ଅକ୍ଷର ଅଚଳ *dest_dir_arg = ଖାଲି;
  bool suppress_file_err = false;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା c;
  ଅକ୍ଷର *template;
  ଅକ୍ଷର *suffix = ଖାଲି;
  bool use_dest_dir = false;
  bool deprecated_t_option = false;
  bool create_directory = false;
  bool dry_run = false;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା status = EXIT_SUCCESS;
  size_t x_ଗଣନା;
  size_t suffix_len;
  ଅକ୍ଷର *dest_name = ଖାଲି;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (maybe_close_stdout);

  ଯେତେବେଳେ ((c = getopt_long (argc, argv, "dp:qtuV", longopts, ଖାଲି)) != -1)
    {
      ବାଛ (c)
        {
        ସ୍ଥିତି 'd':
          create_directory = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'p':
          dest_dir_arg = optarg;
          use_dest_dir = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'q':
          suppress_file_err = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 't':
          use_dest_dir = true;
          deprecated_t_option = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'u':
          dry_run = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି SUFFIX_OPTION:
          suffix = optarg;
          ଭାଙ୍ଗ;

        case_GETOPT_HELP_CHAR;

        ସ୍ଥିତି 'V': /* Undocumented alias, for compatibility with the original
                     mktemp program.  */
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          usage (EXIT_FAILURE);
        }
    }

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_args = argc - optind;
  ଯଦି (2 <= n_args)
    {
      error (0, 0, _("too many templates"));
      usage (EXIT_FAILURE);
    }

  ଯଦି (n_args == 0)
    {
      use_dest_dir = true;
      template = (ଅକ୍ଷର *) default_template;
    }
  ଅନ୍ୟଥା
    {
      template = argv[optind];
    }

  ଯଦି (suffix)
    {
      size_t len = strlen (template);
      ଯଦି (!len || template[len - 1] != 'X')
        {
          error (EXIT_FAILURE, 0,
                 _("with --suffix, template %s must end in X"),
                 quote (template));
        }
      suffix_len = strlen (suffix);
      dest_name = xcharalloc (len + suffix_len + 1);
      memcpy (dest_name, template, len);
      memcpy (dest_name + len, suffix, suffix_len + 1);
      template = dest_name;
      suffix = dest_name + len;
    }
  ଅନ୍ୟଥା
    {
      template = xstrdup (template);
      suffix = strrchr (template, 'X');
      ଯଦି (!suffix)
        suffix = strchr (template, '\0');
      ଅନ୍ୟଥା
        suffix++;
      suffix_len = strlen (suffix);
    }

  /* At this point, template is malloc'd, and suffix points into template.  */
  ଯଦି (suffix_len && last_component (suffix) != suffix)
    {
      error (EXIT_FAILURE, 0,
             _("invalid suffix %s, contains directory separator"),
             quote (suffix));
    }
  x_ଗଣନା = ଗଣନା_consecutive_X_s (template, suffix - template);
  ଯଦି (x_ଗଣନା < 3)
    {
      /* when suffix was appended to template only diagnose the template.  */
      ଯଦି (template == dest_name)
        template[suffix - template] = '\0';
      error (EXIT_FAILURE, 0, _("too few X's in template %s"),
             quote (template));
    }

  ଯଦି (use_dest_dir)
    {
      ଯଦି (deprecated_t_option)
        {
          ଅକ୍ଷର *env = getenv ("TMPDIR");
          ଯଦି (env && *env)
            dest_dir = env;
          ଅନ୍ୟଥା ଯଦି (dest_dir_arg && *dest_dir_arg)
            dest_dir = dest_dir_arg;
          ଅନ୍ୟଥା
            dest_dir = "/tmp";

          ଯଦି (last_component (template) != template)
            error (EXIT_FAILURE, 0,
                   _("invalid template, %s, contains directory separator"),
                   quote (template));
        }
      ଅନ୍ୟଥା
        {
          ଯଦି (dest_dir_arg && *dest_dir_arg)
            dest_dir = dest_dir_arg;
          ଅନ୍ୟଥା
            {
              ଅକ୍ଷର *env = getenv ("TMPDIR");
              dest_dir = (env && *env ? env : "/tmp");
            }
          ଯଦି (IS_ABSOLUTE_FILE_NAME (template))
            error (EXIT_FAILURE, 0,
                   _("invalid template, %s; with --tmpdir,"
                     " it may not be absolute"),
                   quote (template));
        }

      dest_name = file_name_concat (dest_dir, template, ଖାଲି);
      ମୁକ୍ତ (template);
      template = dest_name;
      /* Note that suffix is now invalid.  */
    }

  /* Make a copy to be used in case of diagnostic, since failing
     mkstemp may leave the buffer in an undefined state.  */
  dest_name = xstrdup (template);

  ଯଦି (create_directory)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା err = mkdtemp_len (dest_name, suffix_len, x_ଗଣନା, dry_run);
      ଯଦି (err != 0)
        {
          ଯଦି (!suppress_file_err)
            error (0, errno, _("failed to create directory via template %s"),
                   quote (template));
          status = EXIT_FAILURE;
        }
    }
  ଅନ୍ୟଥା
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = mkstemp_len (dest_name, suffix_len, x_ଗଣନା, dry_run);
      ଯଦି (fd < 0 || (!dry_run && close (fd) != 0))
        {
          ଯଦି (!suppress_file_err)
            error (0, errno, _("failed to create file via template %s"),
                   quote (template));
          status = EXIT_FAILURE;
        }
    }

  ଯଦି (status == EXIT_SUCCESS)
    {
      puts (dest_name);
      /* If we created a file, but then failed to output the file
         name, we should clean up the mess before failing.  */
      ଯଦି (!dry_run && ((stdout_closed = true), close_stream (stdout) != 0))
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_errno = errno;
          (create_directory ? rmdir : unlink) (dest_name);
          ଯଦି (!suppress_file_err)
            error (0, saved_errno, _("write error"));
          status = EXIT_FAILURE;
        }
    }

  main_exit (status);
}
