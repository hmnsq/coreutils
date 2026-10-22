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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "close-stream.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "filenamecat.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tempname.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "mktemp"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS \
  proper_name ("Jim Meyering"), \
  proper_name ("Eric Blake")

ਸਥਿਰ ਅੱਖਰ ਅਚਲ *default_template = "tmp.XXXXXXXXXX";

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ਗਿਣਤੀ
{
  SUFFIX_OPTION = CHAR_MAX + 1,
};

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ longopts[] =
{
  {"directory", no_ਦਲੀਲ, ਨਿਰਾ, 'd'},
  {"quiet", no_ਦਲੀਲ, ਨਿਰਾ, 'q'},
  {"dry-run", no_ਦਲੀਲ, ਨਿਰਾ, 'u'},
  {"suffix", required_ਦਲੀਲ, ਨਿਰਾ, SUFFIX_OPTION},
  {"tmpdir", optional_ਦਲੀਲ, ਨਿਰਾ, 'p'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("Usage: %s [OPTION]... [TEMPLATE]\n"), ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
Create a temporary file or directory, safely, and print its name.\n\
TEMPLATE must contain at ਘੱਟ_ਤੋਂ_ਘੱਟ 3 consecutive 'X's ਵਿੱਚ ਆਖਰੀ component.\n\
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
  -u, --dry-ਲੜੀਵਾਰ_2\n\
         ਕਰੋ not create anything; merely print a name (unsafe)\n\
"));
      oputs (_("\
  -q, --quiet\n\
         suppress diagnostics about file/dir-creation failure\n\
"));
      oputs (_("\
      --suffix=SUFF\n\
         append SUFF ਮੰਜ਼ਿਲ TEMPLATE; SUFF must not contain a slash.\n\
         This option is implied ਜੇ TEMPLATE does not ਸਿਰਾ ਵਿੱਚ X\n\
"));
      oputs (_("\
  -p DIR, --tmpdir[=DIR]\n\
         interpret TEMPLATE relative ਮੰਜ਼ਿਲ DIR;\n\
         ਜੇ DIR is not specified, use $TMPDIR ਜੇ ਸੈੱਟ_2, ਨਹੀਂਤਾਂ /tmp.\n\
         With this option, TEMPLATE must not be an absolute name;\n\
         unlike with -t, TEMPLATE may contain slashes,\n\
         but mktemp creates only the final component\n\
"));
      oputs (_("\
  -t\n\
         interpret TEMPLATE as a single file name component,\n\
         relative ਮੰਜ਼ਿਲ a directory: $TMPDIR, ਜੇ ਸੈੱਟ_2;\n\
         ਨਹੀਂਤਾਂ the directory specified ਰਾਹੀਂ -p; ਨਹੀਂਤਾਂ /tmp [deprecated]\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }

  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

ਸਥਿਰ size_t
ਗਿਣਤੀ_consecutive_X_s (ਅੱਖਰ ਅਚਲ *s, size_t len)
{
  size_t n = 0;
  ਲਈ ( ; len && s[len - 1] == 'X'; len--)
    ++n;
  ਵਾਪਸ n;
}

ਸਥਿਰ ਪੂਰਨ
mkstemp_len (ਅੱਖਰ *tmpl, size_t suff_len, size_t x_len, bool dry_ਲੜੀਵਾਰ)
{
  ਵਾਪਸ gen_tempname_len (tmpl, suff_len, 0, dry_ਲੜੀਵਾਰ ? GT_NOCREATE : GT_FILE,
                           x_len);
}

ਸਥਿਰ ਪੂਰਨ
mkdtemp_len (ਅੱਖਰ *tmpl, size_t suff_len, size_t x_len, bool dry_ਲੜੀਵਾਰ)
{
  ਵਾਪਸ gen_tempname_len (tmpl, suff_len, 0, dry_ਲੜੀਵਾਰ ? GT_NOCREATE : GT_DIR,
                           x_len);
}

/* True if we have already closed standard output.  */
ਸਥਿਰ bool stdout_closed;

/* Avoid closing stdout twice.  Since we conditionally call
   close_stream (stdout) in order to decide whether to clean up a
   temporary file, the exit hook needs to know whether to do all of
   close_stdout or just the stderr half.  */
ਸਥਿਰ ਖਾਲੀ
maybe_close_stdout (ਖਾਲੀ)
{
  ਜੇ (!stdout_closed)
    close_stdout ();
  ਨਹੀਂਤਾਂ ਜੇ (close_ਧਾਰਾ (stderr) != 0)
    _exit (EXIT_FAILURE);
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਅੱਖਰ ਅਚਲ *dest_dir;
  ਅੱਖਰ ਅਚਲ *dest_dir_arg = ਨਿਰਾ;
  bool suppress_file_err = false;
  ਪੂਰਨ c;
  ਅੱਖਰ *ਸਾਂਚਾ;
  ਅੱਖਰ *suffix = ਨਿਰਾ;
  bool use_dest_dir = false;
  bool deprecated_t_option = false;
  bool create_directory = false;
  bool dry_ਲੜੀਵਾਰ = false;
  ਪੂਰਨ ਸਥਿਤੀ = EXIT_SUCCESS;
  size_t x_ਗਿਣਤੀ;
  size_t suffix_len;
  ਅੱਖਰ *dest_name = ਨਿਰਾ;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (maybe_close_stdout);

  ਜਦੋਂ ((c = getopt_long (argc, argv, "dp:qtuV", longopts, ਨਿਰਾ)) != -1)
    {
      ਚੋਣ (c)
        {
        ਹਾਲਤ 'd':
          create_directory = true;
          ਰੋਕੋ;
        ਹਾਲਤ 'p':
          dest_dir_arg = optarg;
          use_dest_dir = true;
          ਰੋਕੋ;
        ਹਾਲਤ 'q':
          suppress_file_err = true;
          ਰੋਕੋ;
        ਹਾਲਤ 't':
          use_dest_dir = true;
          deprecated_t_option = true;
          ਰੋਕੋ;
        ਹਾਲਤ 'u':
          dry_ਲੜੀਵਾਰ = true;
          ਰੋਕੋ;

        ਹਾਲਤ SUFFIX_OPTION:
          suffix = optarg;
          ਰੋਕੋ;

        case_GETOPT_HELP_CHAR;

        ਹਾਲਤ 'V': /* Undocumented alias, for compatibility with the original
                     mktemp program.  */
        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);
        ਮੂਲ:
          usage (EXIT_FAILURE);
        }
    }

  ਪੂਰਨ n_ਦਲੀਲਾਂ = argc - optind;
  ਜੇ (2 <= n_ਦਲੀਲਾਂ)
    {
      error (0, 0, _("too many templates"));
      usage (EXIT_FAILURE);
    }

  ਜੇ (n_ਦਲੀਲਾਂ == 0)
    {
      use_dest_dir = true;
      ਸਾਂਚਾ = (ਅੱਖਰ *) default_template;
    }
  ਨਹੀਂਤਾਂ
    {
      ਸਾਂਚਾ = argv[optind];
    }

  ਜੇ (suffix)
    {
      size_t len = ਲੜੀ_ਲੰਬਾਈ (ਸਾਂਚਾ);
      ਜੇ (!len || ਸਾਂਚਾ[len - 1] != 'X')
        {
          error (EXIT_FAILURE, 0,
                 _("with --suffix, template %s must end in X"),
                 quote (ਸਾਂਚਾ));
        }
      suffix_len = ਲੜੀ_ਲੰਬਾਈ (suffix);
      dest_name = xcharalloc (len + suffix_len + 1);
      ਸਮ੍ਰਿਤੀ_ਨਕਲ (dest_name, ਸਾਂਚਾ, len);
      ਸਮ੍ਰਿਤੀ_ਨਕਲ (dest_name + len, suffix, suffix_len + 1);
      ਸਾਂਚਾ = dest_name;
      suffix = dest_name + len;
    }
  ਨਹੀਂਤਾਂ
    {
      ਸਾਂਚਾ = xstrdup (ਸਾਂਚਾ);
      suffix = strrchr (ਸਾਂਚਾ, 'X');
      ਜੇ (!suffix)
        suffix = strchr (ਸਾਂਚਾ, '\0');
      ਨਹੀਂਤਾਂ
        suffix++;
      suffix_len = ਲੜੀ_ਲੰਬਾਈ (suffix);
    }

  /* At this point, template is malloc'd, and suffix points into template.  */
  ਜੇ (suffix_len && ਆਖਰੀ_component (suffix) != suffix)
    {
      error (EXIT_FAILURE, 0,
             _("invalid suffix %s, contains directory separator"),
             quote (suffix));
    }
  x_ਗਿਣਤੀ = ਗਿਣਤੀ_consecutive_X_s (ਸਾਂਚਾ, suffix - ਸਾਂਚਾ);
  ਜੇ (x_ਗਿਣਤੀ < 3)
    {
      /* when suffix was appended to template only diagnose the template.  */
      ਜੇ (ਸਾਂਚਾ == dest_name)
        ਸਾਂਚਾ[suffix - ਸਾਂਚਾ] = '\0';
      error (EXIT_FAILURE, 0, _("too few X's in template %s"),
             quote (ਸਾਂਚਾ));
    }

  ਜੇ (use_dest_dir)
    {
      ਜੇ (deprecated_t_option)
        {
          ਅੱਖਰ *env = getenv ("TMPDIR");
          ਜੇ (env && *env)
            dest_dir = env;
          ਨਹੀਂਤਾਂ ਜੇ (dest_dir_arg && *dest_dir_arg)
            dest_dir = dest_dir_arg;
          ਨਹੀਂਤਾਂ
            dest_dir = "/tmp";

          ਜੇ (ਆਖਰੀ_component (ਸਾਂਚਾ) != ਸਾਂਚਾ)
            error (EXIT_FAILURE, 0,
                   _("invalid template, %s, contains directory separator"),
                   quote (ਸਾਂਚਾ));
        }
      ਨਹੀਂਤਾਂ
        {
          ਜੇ (dest_dir_arg && *dest_dir_arg)
            dest_dir = dest_dir_arg;
          ਨਹੀਂਤਾਂ
            {
              ਅੱਖਰ *env = getenv ("TMPDIR");
              dest_dir = (env && *env ? env : "/tmp");
            }
          ਜੇ (IS_ABSOLUTE_FILE_NAME (ਸਾਂਚਾ))
            error (EXIT_FAILURE, 0,
                   _("invalid template, %s; with --tmpdir,"
                     " it may not be absolute"),
                   quote (ਸਾਂਚਾ));
        }

      dest_name = file_name_concat (dest_dir, ਸਾਂਚਾ, ਨਿਰਾ);
      ਮੁਕਤ (ਸਾਂਚਾ);
      ਸਾਂਚਾ = dest_name;
      /* Note that suffix is now invalid.  */
    }

  /* Make a copy to be used in case of diagnostic, since failing
     mkstemp may leave the buffer in an undefined state.  */
  dest_name = xstrdup (ਸਾਂਚਾ);

  ਜੇ (create_directory)
    {
      ਪੂਰਨ err = mkdtemp_len (dest_name, suffix_len, x_ਗਿਣਤੀ, dry_ਲੜੀਵਾਰ);
      ਜੇ (err != 0)
        {
          ਜੇ (!suppress_file_err)
            error (0, errno, _("failed to create directory via template %s"),
                   quote (ਸਾਂਚਾ));
          ਸਥਿਤੀ = EXIT_FAILURE;
        }
    }
  ਨਹੀਂਤਾਂ
    {
      ਪੂਰਨ fd = mkstemp_len (dest_name, suffix_len, x_ਗਿਣਤੀ, dry_ਲੜੀਵਾਰ);
      ਜੇ (fd < 0 || (!dry_ਲੜੀਵਾਰ && close (fd) != 0))
        {
          ਜੇ (!suppress_file_err)
            error (0, errno, _("failed to create file via template %s"),
                   quote (ਸਾਂਚਾ));
          ਸਥਿਤੀ = EXIT_FAILURE;
        }
    }

  ਜੇ (ਸਥਿਤੀ == EXIT_SUCCESS)
    {
      puts (dest_name);
      /* If we created a file, but then failed to output the file
         name, we should clean up the mess before failing.  */
      ਜੇ (!dry_ਲੜੀਵਾਰ && ((stdout_closed = true), close_ਧਾਰਾ (stdout) != 0))
        {
          ਪੂਰਨ saved_errno = errno;
          (create_directory ? rmdir : unlink) (dest_name);
          ਜੇ (!suppress_file_err)
            error (0, saved_errno, _("write error"));
          ਸਥਿਤੀ = EXIT_FAILURE;
        }
    }

  main_exit (ਸਥਿਤੀ);
}
