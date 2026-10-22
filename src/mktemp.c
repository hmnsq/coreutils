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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <getopt.h>

#അന്തര്ഭാവ "system.h"

#അന്തര്ഭാവ "close-stream.h"
#അന്തര്ഭാവ "filenamecat.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "tempname.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "mktemp"

#define AUTHORS \
  proper_name ("Jim Meyering"), \
  proper_name ("Eric Blake")

സ്ഥിതം അക്ഷരം സ്ഥിരം *default_template = "tmp.XXXXXXXXXX";

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
എണ്ണൽ
{
  SUFFIX_OPTION = CHAR_MAX + 1,
};

സ്ഥിതം ഘടന option സ്ഥിരം longopts[] =
{
  {"directory", no_argument, ശൂന്യ, 'd'},
  {"quiet", no_argument, ശൂന്യ, 'q'},
  {"dry-run", no_argument, ശൂന്യ, 'u'},
  {"suffix", required_argument, ശൂന്യ, SUFFIX_OPTION},
  {"tmpdir", optional_argument, ശൂന്യ, 'p'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ശൂന്യ, 0, ശൂന്യ, 0}
};

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
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
         ചെയ്യുക not create anything; merely print a name (unsafe)\n\
"));
      oputs (_("\
  -q, --quiet\n\
         suppress diagnostics about file/dir-creation failure\n\
"));
      oputs (_("\
      --suffix=SUFF\n\
         append SUFF to TEMPLATE; SUFF must not contain a slash.\n\
         This option is implied എങ്കിൽ TEMPLATE does not end in X\n\
"));
      oputs (_("\
  -p DIR, --tmpdir[=DIR]\n\
         interpret TEMPLATE relative to DIR;\n\
         എങ്കിൽ DIR is not specified, use $TMPDIR എങ്കിൽ set, അല്ലെങ്കിൽ /tmp.\n\
         With this option, TEMPLATE must not be an absolute name;\n\
         unlike with -t, TEMPLATE may contain slashes,\n\
         but mktemp creates only the final component\n\
"));
      oputs (_("\
  -t\n\
         interpret TEMPLATE as a single file name component,\n\
         relative to a directory: $TMPDIR, എങ്കിൽ set;\n\
         അല്ലെങ്കിൽ the directory specified via -p; അല്ലെങ്കിൽ /tmp [deprecated]\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }

  exit (status);
}

സ്ഥിതം size_t
എണ്ണം_consecutive_X_s (അക്ഷരം സ്ഥിരം *s, size_t len)
{
  size_t n = 0;
  വേണ്ടി ( ; len && s[len - 1] == 'X'; len--)
    ++n;
  തിരികെ n;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
mkstemp_len (അക്ഷരം *tmpl, size_t suff_len, size_t x_len, bool dry_run)
{
  തിരികെ gen_tempname_len (tmpl, suff_len, 0, dry_run ? GT_NOCREATE : GT_FILE,
                           x_len);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
mkdtemp_len (അക്ഷരം *tmpl, size_t suff_len, size_t x_len, bool dry_run)
{
  തിരികെ gen_tempname_len (tmpl, suff_len, 0, dry_run ? GT_NOCREATE : GT_DIR,
                           x_len);
}

/* True if we have already closed standard output.  */
സ്ഥിതം bool stdout_closed;

/* Avoid closing stdout twice.  Since we conditionally call
   close_stream (stdout) in order to decide whether to clean up a
   temporary file, the exit hook needs to know whether to do all of
   close_stdout or just the stderr half.  */
സ്ഥിതം ശൂന്യം
maybe_close_stdout (ശൂന്യം)
{
  എങ്കിൽ (!stdout_closed)
    close_stdout ();
  അല്ലെങ്കിൽ എങ്കിൽ (close_stream (stderr) != 0)
    _exit (EXIT_FAILURE);
}

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  അക്ഷരം സ്ഥിരം *dest_dir;
  അക്ഷരം സ്ഥിരം *dest_dir_arg = ശൂന്യ;
  bool suppress_file_err = false;
  പൂർണ്ണസംഖ്യ c;
  അക്ഷരം *template;
  അക്ഷരം *suffix = ശൂന്യ;
  bool use_dest_dir = false;
  bool deprecated_t_option = false;
  bool create_directory = false;
  bool dry_run = false;
  പൂർണ്ണസംഖ്യ status = EXIT_SUCCESS;
  size_t x_എണ്ണം;
  size_t suffix_len;
  അക്ഷരം *dest_name = ശൂന്യ;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (maybe_close_stdout);

  ആകുമ്പോൾ ((c = getopt_long (argc, argv, "dp:qtuV", longopts, ശൂന്യ)) != -1)
    {
      തിരഞ്ഞെടുപ്പ് (c)
        {
        അവസ്ഥ 'd':
          create_directory = true;
          നിർത്തുക;
        അവസ്ഥ 'p':
          dest_dir_arg = optarg;
          use_dest_dir = true;
          നിർത്തുക;
        അവസ്ഥ 'q':
          suppress_file_err = true;
          നിർത്തുക;
        അവസ്ഥ 't':
          use_dest_dir = true;
          deprecated_t_option = true;
          നിർത്തുക;
        അവസ്ഥ 'u':
          dry_run = true;
          നിർത്തുക;

        അവസ്ഥ SUFFIX_OPTION:
          suffix = optarg;
          നിർത്തുക;

        case_GETOPT_HELP_CHAR;

        അവസ്ഥ 'V': /* Undocumented alias, for compatibility with the original
                     mktemp program.  */
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        സ്ഥിരസ്ഥിതി:
          usage (EXIT_FAILURE);
        }
    }

  പൂർണ്ണസംഖ്യ n_args = argc - optind;
  എങ്കിൽ (2 <= n_args)
    {
      error (0, 0, _("too many templates"));
      usage (EXIT_FAILURE);
    }

  എങ്കിൽ (n_args == 0)
    {
      use_dest_dir = true;
      template = (അക്ഷരം *) default_template;
    }
  അല്ലെങ്കിൽ
    {
      template = argv[optind];
    }

  എങ്കിൽ (suffix)
    {
      size_t len = strlen (template);
      എങ്കിൽ (!len || template[len - 1] != 'X')
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
  അല്ലെങ്കിൽ
    {
      template = xstrdup (template);
      suffix = strrchr (template, 'X');
      എങ്കിൽ (!suffix)
        suffix = strchr (template, '\0');
      അല്ലെങ്കിൽ
        suffix++;
      suffix_len = strlen (suffix);
    }

  /* At this point, template is malloc'd, and suffix points into template.  */
  എങ്കിൽ (suffix_len && last_component (suffix) != suffix)
    {
      error (EXIT_FAILURE, 0,
             _("invalid suffix %s, contains directory separator"),
             quote (suffix));
    }
  x_എണ്ണം = എണ്ണം_consecutive_X_s (template, suffix - template);
  എങ്കിൽ (x_എണ്ണം < 3)
    {
      /* when suffix was appended to template only diagnose the template.  */
      എങ്കിൽ (template == dest_name)
        template[suffix - template] = '\0';
      error (EXIT_FAILURE, 0, _("too few X's in template %s"),
             quote (template));
    }

  എങ്കിൽ (use_dest_dir)
    {
      എങ്കിൽ (deprecated_t_option)
        {
          അക്ഷരം *env = getenv ("TMPDIR");
          എങ്കിൽ (env && *env)
            dest_dir = env;
          അല്ലെങ്കിൽ എങ്കിൽ (dest_dir_arg && *dest_dir_arg)
            dest_dir = dest_dir_arg;
          അല്ലെങ്കിൽ
            dest_dir = "/tmp";

          എങ്കിൽ (last_component (template) != template)
            error (EXIT_FAILURE, 0,
                   _("invalid template, %s, contains directory separator"),
                   quote (template));
        }
      അല്ലെങ്കിൽ
        {
          എങ്കിൽ (dest_dir_arg && *dest_dir_arg)
            dest_dir = dest_dir_arg;
          അല്ലെങ്കിൽ
            {
              അക്ഷരം *env = getenv ("TMPDIR");
              dest_dir = (env && *env ? env : "/tmp");
            }
          എങ്കിൽ (IS_ABSOLUTE_FILE_NAME (template))
            error (EXIT_FAILURE, 0,
                   _("invalid template, %s; with --tmpdir,"
                     " it may not be absolute"),
                   quote (template));
        }

      dest_name = file_name_concat (dest_dir, template, ശൂന്യ);
      സ്വതന്ത്രം (template);
      template = dest_name;
      /* Note that suffix is now invalid.  */
    }

  /* Make a copy to be used in case of diagnostic, since failing
     mkstemp may leave the buffer in an undefined state.  */
  dest_name = xstrdup (template);

  എങ്കിൽ (create_directory)
    {
      പൂർണ്ണസംഖ്യ err = mkdtemp_len (dest_name, suffix_len, x_എണ്ണം, dry_run);
      എങ്കിൽ (err != 0)
        {
          എങ്കിൽ (!suppress_file_err)
            error (0, errno, _("failed to create directory via template %s"),
                   quote (template));
          status = EXIT_FAILURE;
        }
    }
  അല്ലെങ്കിൽ
    {
      പൂർണ്ണസംഖ്യ fd = mkstemp_len (dest_name, suffix_len, x_എണ്ണം, dry_run);
      എങ്കിൽ (fd < 0 || (!dry_run && close (fd) != 0))
        {
          എങ്കിൽ (!suppress_file_err)
            error (0, errno, _("failed to create file via template %s"),
                   quote (template));
          status = EXIT_FAILURE;
        }
    }

  എങ്കിൽ (status == EXIT_SUCCESS)
    {
      puts (dest_name);
      /* If we created a file, but then failed to output the file
         name, we should clean up the mess before failing.  */
      എങ്കിൽ (!dry_run && ((stdout_closed = true), close_stream (stdout) != 0))
        {
          പൂർണ്ണസംഖ്യ saved_errno = errno;
          (create_directory ? rmdir : unlink) (dest_name);
          എങ്കിൽ (!suppress_file_err)
            error (0, saved_errno, _("write error"));
          status = EXIT_FAILURE;
        }
    }

  main_exit (status);
}
