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
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "mktemp"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS \
  proper_name ("Jim Meyering"), \
  proper_name ("Eric Blake")

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *default_template = "tmp.XXXXXXXXXX";

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ಎಣಿಕೆ
{
  SUFFIX_OPTION = CHAR_MAX + 1,
};

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ longopts[] =
{
  {"directory", no_ವಾದ, ಖಾಲಿ, 'd'},
  {"quiet", no_ವಾದ, ಖಾಲಿ, 'q'},
  {"dry-run", no_ವಾದ, ಖಾಲಿ, 'u'},
  {"suffix", required_ವಾದ, ಖಾಲಿ, SUFFIX_OPTION},
  {"tmpdir", optional_ವಾದ, ಖಾಲಿ, 'p'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("Usage: %s [OPTION]... [TEMPLATE]\n"), ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
Create a temporary file or directory, safely, and print its name.\n\
TEMPLATE must contain at ಕನಿಷ್ಠ 3 consecutive 'X's ಒಳ ಕೊನೆಯ component.\n\
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
  -u, --dry-ಸತತ_2\n\
         ಮಾಡು not create anything; merely print a name (unsafe)\n\
"));
      oputs (_("\
  -q, --quiet\n\
         suppress diagnostics about file/dir-creation failure\n\
"));
      oputs (_("\
      --suffix=SUFF\n\
         append SUFF ಗಮ್ಯ TEMPLATE; SUFF must not contain a slash.\n\
         This option is implied ಆದರೆ TEMPLATE does not ಕೊನೆ ಒಳ X\n\
"));
      oputs (_("\
  -p DIR, --tmpdir[=DIR]\n\
         interpret TEMPLATE relative ಗಮ್ಯ DIR;\n\
         ಆದರೆ DIR is not specified, use $TMPDIR ಆದರೆ ಗಣ_2, ಇಲ್ಲದಿದ್ದರೆ /tmp.\n\
         With this option, TEMPLATE must not be an absolute name;\n\
         unlike with -t, TEMPLATE may contain slashes,\n\
         but mktemp creates only the final component\n\
"));
      oputs (_("\
  -t\n\
         interpret TEMPLATE as a single file name component,\n\
         relative ಗಮ್ಯ a directory: $TMPDIR, ಆದರೆ ಗಣ_2;\n\
         ಇಲ್ಲದಿದ್ದರೆ the directory specified ಮೂಲಕ -p; ಇಲ್ಲದಿದ್ದರೆ /tmp [deprecated]\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }

  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

ಸ್ಥಿರ size_t
ಎಣಿಕೆ_consecutive_X_s (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *s, size_t len)
{
  size_t n = 0;
  ಗಾಗಿ ( ; len && s[len - 1] == 'X'; len--)
    ++n;
  ಹಿಂತಿರುಗಿ n;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
mkstemp_len (ಅಕ್ಷರ *tmpl, size_t suff_len, size_t x_len, bool dry_ಸತತ)
{
  ಹಿಂತಿರುಗಿ gen_tempname_len (tmpl, suff_len, 0, dry_ಸತತ ? GT_NOCREATE : GT_FILE,
                           x_len);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
mkdtemp_len (ಅಕ್ಷರ *tmpl, size_t suff_len, size_t x_len, bool dry_ಸತತ)
{
  ಹಿಂತಿರುಗಿ gen_tempname_len (tmpl, suff_len, 0, dry_ಸತತ ? GT_NOCREATE : GT_DIR,
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
  ಆದರೆ (!stdout_closed)
    close_stdout ();
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (close_ಹರಿವು (stderr) != 0)
    _exit (EXIT_FAILURE);
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dest_dir;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dest_dir_arg = ಖಾಲಿ;
  bool suppress_file_err = false;
  ಪೂರ್ಣಾಂಕ c;
  ಅಕ್ಷರ *ಮಾದರಿ;
  ಅಕ್ಷರ *suffix = ಖಾಲಿ;
  bool use_dest_dir = false;
  bool deprecated_t_option = false;
  bool create_directory = false;
  bool dry_ಸತತ = false;
  ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2 = EXIT_SUCCESS;
  size_t x_ಎಣಿಕೆ;
  size_t suffix_len;
  ಅಕ್ಷರ *dest_name = ಖಾಲಿ;

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (maybe_close_stdout);

  ಆಗ ((c = getopt_long (argc, argv, "dp:qtuV", longopts, ಖಾಲಿ)) != -1)
    {
      ಆಯ್ಕೆ (c)
        {
        ಸ್ಥಿತಿ 'd':
          create_directory = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'p':
          dest_dir_arg = optarg;
          use_dest_dir = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'q':
          suppress_file_err = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 't':
          use_dest_dir = true;
          deprecated_t_option = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'u':
          dry_ಸತತ = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ SUFFIX_OPTION:
          suffix = optarg;
          ನಿಲ್ಲಿಸು;

        case_GETOPT_HELP_CHAR;

        ಸ್ಥಿತಿ 'V': /* Undocumented alias, for compatibility with the original
                     mktemp program.  */
        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);
        ಪೂರ್ವನಿಯೋಜಿತ:
          usage (EXIT_FAILURE);
        }
    }

  ಪೂರ್ಣಾಂಕ n_ವಾದಗಳು = argc - optind;
  ಆದರೆ (2 <= n_ವಾದಗಳು)
    {
      error (0, 0, _("too many templates"));
      usage (EXIT_FAILURE);
    }

  ಆದರೆ (n_ವಾದಗಳು == 0)
    {
      use_dest_dir = true;
      ಮಾದರಿ = (ಅಕ್ಷರ *) default_template;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಮಾದರಿ = argv[optind];
    }

  ಆದರೆ (suffix)
    {
      size_t len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ಮಾದರಿ);
      ಆದರೆ (!len || ಮಾದರಿ[len - 1] != 'X')
        {
          error (EXIT_FAILURE, 0,
                 _("with --suffix, template %s must end in X"),
                 quote (ಮಾದರಿ));
        }
      suffix_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (suffix);
      dest_name = xcharalloc (len + suffix_len + 1);
      ಸ್ಮರಣೆ_ನಕಲು (dest_name, ಮಾದರಿ, len);
      ಸ್ಮರಣೆ_ನಕಲು (dest_name + len, suffix, suffix_len + 1);
      ಮಾದರಿ = dest_name;
      suffix = dest_name + len;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಮಾದರಿ = xstrdup (ಮಾದರಿ);
      suffix = strrchr (ಮಾದರಿ, 'X');
      ಆದರೆ (!suffix)
        suffix = strchr (ಮಾದರಿ, '\0');
      ಇಲ್ಲದಿದ್ದರೆ
        suffix++;
      suffix_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (suffix);
    }

  /* At this point, template is malloc'd, and suffix points into template.  */
  ಆದರೆ (suffix_len && ಕೊನೆಯ_component (suffix) != suffix)
    {
      error (EXIT_FAILURE, 0,
             _("invalid suffix %s, contains directory separator"),
             quote (suffix));
    }
  x_ಎಣಿಕೆ = ಎಣಿಕೆ_consecutive_X_s (ಮಾದರಿ, suffix - ಮಾದರಿ);
  ಆದರೆ (x_ಎಣಿಕೆ < 3)
    {
      /* when suffix was appended to template only diagnose the template.  */
      ಆದರೆ (ಮಾದರಿ == dest_name)
        ಮಾದರಿ[suffix - ಮಾದರಿ] = '\0';
      error (EXIT_FAILURE, 0, _("too few X's in template %s"),
             quote (ಮಾದರಿ));
    }

  ಆದರೆ (use_dest_dir)
    {
      ಆದರೆ (deprecated_t_option)
        {
          ಅಕ್ಷರ *env = getenv ("TMPDIR");
          ಆದರೆ (env && *env)
            dest_dir = env;
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (dest_dir_arg && *dest_dir_arg)
            dest_dir = dest_dir_arg;
          ಇಲ್ಲದಿದ್ದರೆ
            dest_dir = "/tmp";

          ಆದರೆ (ಕೊನೆಯ_component (ಮಾದರಿ) != ಮಾದರಿ)
            error (EXIT_FAILURE, 0,
                   _("invalid template, %s, contains directory separator"),
                   quote (ಮಾದರಿ));
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಆದರೆ (dest_dir_arg && *dest_dir_arg)
            dest_dir = dest_dir_arg;
          ಇಲ್ಲದಿದ್ದರೆ
            {
              ಅಕ್ಷರ *env = getenv ("TMPDIR");
              dest_dir = (env && *env ? env : "/tmp");
            }
          ಆದರೆ (IS_ABSOLUTE_FILE_NAME (ಮಾದರಿ))
            error (EXIT_FAILURE, 0,
                   _("invalid template, %s; with --tmpdir,"
                     " it may not be absolute"),
                   quote (ಮಾದರಿ));
        }

      dest_name = file_name_concat (dest_dir, ಮಾದರಿ, ಖಾಲಿ);
      ಬಿಡುಗಡೆ (ಮಾದರಿ);
      ಮಾದರಿ = dest_name;
      /* Note that suffix is now invalid.  */
    }

  /* Make a copy to be used in case of diagnostic, since failing
     mkstemp may leave the buffer in an undefined state.  */
  dest_name = xstrdup (ಮಾದರಿ);

  ಆದರೆ (create_directory)
    {
      ಪೂರ್ಣಾಂಕ err = mkdtemp_len (dest_name, suffix_len, x_ಎಣಿಕೆ, dry_ಸತತ);
      ಆದರೆ (err != 0)
        {
          ಆದರೆ (!suppress_file_err)
            error (0, errno, _("failed to create directory via template %s"),
                   quote (ಮಾದರಿ));
          ಸ್ಥಿತಿ_2 = EXIT_FAILURE;
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಪೂರ್ಣಾಂಕ fd = mkstemp_len (dest_name, suffix_len, x_ಎಣಿಕೆ, dry_ಸತತ);
      ಆದರೆ (fd < 0 || (!dry_ಸತತ && close (fd) != 0))
        {
          ಆದರೆ (!suppress_file_err)
            error (0, errno, _("failed to create file via template %s"),
                   quote (ಮಾದರಿ));
          ಸ್ಥಿತಿ_2 = EXIT_FAILURE;
        }
    }

  ಆದರೆ (ಸ್ಥಿತಿ_2 == EXIT_SUCCESS)
    {
      puts (dest_name);
      /* If we created a file, but then failed to output the file
         name, we should clean up the mess before failing.  */
      ಆದರೆ (!dry_ಸತತ && ((stdout_closed = true), close_ಹರಿವು (stdout) != 0))
        {
          ಪೂರ್ಣಾಂಕ saved_errno = errno;
          (create_directory ? rmdir : unlink) (dest_name);
          ಆದರೆ (!suppress_file_err)
            error (0, saved_errno, _("write error"));
          ಸ್ಥಿತಿ_2 = EXIT_FAILURE;
        }
    }

  main_exit (ಸ್ಥಿತಿ_2);
}
