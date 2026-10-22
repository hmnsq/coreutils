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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>

#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ "close-stream.h"
#ಅನ್ತರ್ಭಾವ "filenamecat.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "tempname.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "mktemp"

#define AUTHORS \
  proper_name ("Jim Meyering"), \
  proper_name ("Eric Blake")

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *default_template = "tmp.XXXXXXXXXX";

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ಗಣನಾ
{
  SUFFIX_OPTION = CHAR_MAX + 1,
};

ಸ್ಥಿರ ಸಂರಚನಾ option ಧ್ರುವ longopts[] =
{
  {"directory", no_argument, ರಿಕ್ತ, 'd'},
  {"quiet", no_argument, ರಿಕ್ತ, 'q'},
  {"dry-run", no_argument, ರಿಕ್ತ, 'u'},
  {"suffix", required_argument, ರಿಕ್ತ, SUFFIX_OPTION},
  {"tmpdir", optional_argument, ರಿಕ್ತ, 'p'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ರಿಕ್ತ, 0, ರಿಕ್ತ, 0}
};

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
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
         ಕುರು not create anything; merely print a name (unsafe)\n\
"));
      oputs (_("\
  -q, --quiet\n\
         suppress diagnostics about file/dir-creation failure\n\
"));
      oputs (_("\
      --suffix=SUFF\n\
         append SUFF to TEMPLATE; SUFF must not contain a slash.\n\
         This option is implied ಯದಿ TEMPLATE does not end in X\n\
"));
      oputs (_("\
  -p DIR, --tmpdir[=DIR]\n\
         interpret TEMPLATE relative to DIR;\n\
         ಯದಿ DIR is not specified, use $TMPDIR ಯದಿ set, ಅನ್ಯಥಾ /tmp.\n\
         With this option, TEMPLATE must not be an absolute name;\n\
         unlike with -t, TEMPLATE may contain slashes,\n\
         but mktemp creates only the final component\n\
"));
      oputs (_("\
  -t\n\
         interpret TEMPLATE as a single file name component,\n\
         relative to a directory: $TMPDIR, ಯದಿ set;\n\
         ಅನ್ಯಥಾ the directory specified via -p; ಅನ್ಯಥಾ /tmp [deprecated]\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }

  exit (status);
}

ಸ್ಥಿರ size_t
ಎಣಿಕೆ_consecutive_X_s (ವರ್ಣ ಧ್ರುವ *s, size_t len)
{
  size_t n = 0;
  ಪ್ರತಿ ( ; len && s[len - 1] == 'X'; len--)
    ++n;
  ಪ್ರತ್ಯಾಗಮ n;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
mkstemp_len (ವರ್ಣ *tmpl, size_t suff_len, size_t x_len, bool dry_run)
{
  ಪ್ರತ್ಯಾಗಮ gen_tempname_len (tmpl, suff_len, 0, dry_run ? GT_NOCREATE : GT_FILE,
                           x_len);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
mkdtemp_len (ವರ್ಣ *tmpl, size_t suff_len, size_t x_len, bool dry_run)
{
  ಪ್ರತ್ಯಾಗಮ gen_tempname_len (tmpl, suff_len, 0, dry_run ? GT_NOCREATE : GT_DIR,
                           x_len);
}

/* True if we have already closed standard output.  */
ಸ್ಥಿರ bool stdout_closed;

/* Avoid closing stdout twice.  Since we conditionally call
   close_stream (stdout) in order to decide whether to clean up a
   temporary file, the exit hook needs to know whether to do all of
   close_stdout or just the stderr half.  */
ಸ್ಥಿರ ಶೂನ್ಯ
maybe_close_stdout (ಶೂನ್ಯ)
{
  ಯದಿ (!stdout_closed)
    close_stdout ();
  ಅನ್ಯಥಾ ಯದಿ (close_stream (stderr) != 0)
    _exit (EXIT_FAILURE);
}

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  ವರ್ಣ ಧ್ರುವ *dest_dir;
  ವರ್ಣ ಧ್ರುವ *dest_dir_arg = ರಿಕ್ತ;
  bool suppress_file_err = false;
  ಪೂರ್ಣಾಙ್ಕ c;
  ವರ್ಣ *template;
  ವರ್ಣ *suffix = ರಿಕ್ತ;
  bool use_dest_dir = false;
  bool deprecated_t_option = false;
  bool create_directory = false;
  bool dry_run = false;
  ಪೂರ್ಣಾಙ್ಕ status = EXIT_SUCCESS;
  size_t x_ಎಣಿಕೆ;
  size_t suffix_len;
  ವರ್ಣ *dest_name = ರಿಕ್ತ;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (maybe_close_stdout);

  ಯಾವತ್ ((c = getopt_long (argc, argv, "dp:qtuV", longopts, ರಿಕ್ತ)) != -1)
    {
      ವಿಕಲ್ಪ (c)
        {
        ಪಕ್ಷ 'd':
          create_directory = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'p':
          dest_dir_arg = optarg;
          use_dest_dir = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'q':
          suppress_file_err = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ 't':
          use_dest_dir = true;
          deprecated_t_option = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'u':
          dry_run = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ SUFFIX_OPTION:
          suffix = optarg;
          ಭಙ್ಗ;

        case_GETOPT_HELP_CHAR;

        ಪಕ್ಷ 'V': /* Undocumented alias, for compatibility with the original
                     mktemp program.  */
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        ಸ್ವಭಾವ:
          usage (EXIT_FAILURE);
        }
    }

  ಪೂರ್ಣಾಙ್ಕ n_args = argc - optind;
  ಯದಿ (2 <= n_args)
    {
      error (0, 0, _("too many templates"));
      usage (EXIT_FAILURE);
    }

  ಯದಿ (n_args == 0)
    {
      use_dest_dir = true;
      template = (ವರ್ಣ *) default_template;
    }
  ಅನ್ಯಥಾ
    {
      template = argv[optind];
    }

  ಯದಿ (suffix)
    {
      size_t len = strlen (template);
      ಯದಿ (!len || template[len - 1] != 'X')
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
  ಅನ್ಯಥಾ
    {
      template = xstrdup (template);
      suffix = strrchr (template, 'X');
      ಯದಿ (!suffix)
        suffix = strchr (template, '\0');
      ಅನ್ಯಥಾ
        suffix++;
      suffix_len = strlen (suffix);
    }

  /* At this point, template is malloc'd, and suffix points into template.  */
  ಯದಿ (suffix_len && last_component (suffix) != suffix)
    {
      error (EXIT_FAILURE, 0,
             _("invalid suffix %s, contains directory separator"),
             quote (suffix));
    }
  x_ಎಣಿಕೆ = ಎಣಿಕೆ_consecutive_X_s (template, suffix - template);
  ಯದಿ (x_ಎಣಿಕೆ < 3)
    {
      /* when suffix was appended to template only diagnose the template.  */
      ಯದಿ (template == dest_name)
        template[suffix - template] = '\0';
      error (EXIT_FAILURE, 0, _("too few X's in template %s"),
             quote (template));
    }

  ಯದಿ (use_dest_dir)
    {
      ಯದಿ (deprecated_t_option)
        {
          ವರ್ಣ *env = getenv ("TMPDIR");
          ಯದಿ (env && *env)
            dest_dir = env;
          ಅನ್ಯಥಾ ಯದಿ (dest_dir_arg && *dest_dir_arg)
            dest_dir = dest_dir_arg;
          ಅನ್ಯಥಾ
            dest_dir = "/tmp";

          ಯದಿ (last_component (template) != template)
            error (EXIT_FAILURE, 0,
                   _("invalid template, %s, contains directory separator"),
                   quote (template));
        }
      ಅನ್ಯಥಾ
        {
          ಯದಿ (dest_dir_arg && *dest_dir_arg)
            dest_dir = dest_dir_arg;
          ಅನ್ಯಥಾ
            {
              ವರ್ಣ *env = getenv ("TMPDIR");
              dest_dir = (env && *env ? env : "/tmp");
            }
          ಯದಿ (IS_ABSOLUTE_FILE_NAME (template))
            error (EXIT_FAILURE, 0,
                   _("invalid template, %s; with --tmpdir,"
                     " it may not be absolute"),
                   quote (template));
        }

      dest_name = file_name_concat (dest_dir, template, ರಿಕ್ತ);
      ಮುಕ್ತ (template);
      template = dest_name;
      /* Note that suffix is now invalid.  */
    }

  /* Make a copy to be used in case of diagnostic, since failing
     mkstemp may leave the buffer in an undefined state.  */
  dest_name = xstrdup (template);

  ಯದಿ (create_directory)
    {
      ಪೂರ್ಣಾಙ್ಕ err = mkdtemp_len (dest_name, suffix_len, x_ಎಣಿಕೆ, dry_run);
      ಯದಿ (err != 0)
        {
          ಯದಿ (!suppress_file_err)
            error (0, errno, _("failed to create directory via template %s"),
                   quote (template));
          status = EXIT_FAILURE;
        }
    }
  ಅನ್ಯಥಾ
    {
      ಪೂರ್ಣಾಙ್ಕ fd = mkstemp_len (dest_name, suffix_len, x_ಎಣಿಕೆ, dry_run);
      ಯದಿ (fd < 0 || (!dry_run && close (fd) != 0))
        {
          ಯದಿ (!suppress_file_err)
            error (0, errno, _("failed to create file via template %s"),
                   quote (template));
          status = EXIT_FAILURE;
        }
    }

  ಯದಿ (status == EXIT_SUCCESS)
    {
      puts (dest_name);
      /* If we created a file, but then failed to output the file
         name, we should clean up the mess before failing.  */
      ಯದಿ (!dry_run && ((stdout_closed = true), close_stream (stdout) != 0))
        {
          ಪೂರ್ಣಾಙ್ಕ saved_errno = errno;
          (create_directory ? rmdir : unlink) (dest_name);
          ಯದಿ (!suppress_file_err)
            error (0, saved_errno, _("write error"));
          status = EXIT_FAILURE;
        }
    }

  main_exit (status);
}
