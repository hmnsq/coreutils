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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <getopt.h>

#અન્તર્ભાવ "system.h"

#અન્તર્ભાવ "close-stream.h"
#અન્તર્ભાવ "filenamecat.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "tempname.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "mktemp"

#define AUTHORS \
  proper_name ("Jim Meyering"), \
  proper_name ("Eric Blake")

સ્થિર અક્ષર અચલ *default_template = "tmp.XXXXXXXXXX";

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ગણના
{
  SUFFIX_OPTION = CHAR_MAX + 1,
};

સ્થિર સંરચના option અચલ longopts[] =
{
  {"directory", no_argument, ખાલી, 'd'},
  {"quiet", no_argument, ખાલી, 'q'},
  {"dry-run", no_argument, ખાલી, 'u'},
  {"suffix", required_argument, ખાલી, SUFFIX_OPTION},
  {"tmpdir", optional_argument, ખાલી, 'p'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ખાલી, 0, ખાલી, 0}
};

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
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
         કરો not create anything; merely print a name (unsafe)\n\
"));
      oputs (_("\
  -q, --quiet\n\
         suppress diagnostics about file/dir-creation failure\n\
"));
      oputs (_("\
      --suffix=SUFF\n\
         append SUFF to TEMPLATE; SUFF must not contain a slash.\n\
         This option is implied જો TEMPLATE does not end in X\n\
"));
      oputs (_("\
  -p DIR, --tmpdir[=DIR]\n\
         interpret TEMPLATE relative to DIR;\n\
         જો DIR is not specified, use $TMPDIR જો set, નહીંતર /tmp.\n\
         With this option, TEMPLATE must not be an absolute name;\n\
         unlike with -t, TEMPLATE may contain slashes,\n\
         but mktemp creates only the final component\n\
"));
      oputs (_("\
  -t\n\
         interpret TEMPLATE as a single file name component,\n\
         relative to a directory: $TMPDIR, જો set;\n\
         નહીંતર the directory specified via -p; નહીંતર /tmp [deprecated]\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }

  exit (status);
}

સ્થિર size_t
ગણતરી_consecutive_X_s (અક્ષર અચલ *s, size_t len)
{
  size_t n = 0;
  માટે ( ; len && s[len - 1] == 'X'; len--)
    ++n;
  પાછા n;
}

સ્થિર પૂર્ણાંક
mkstemp_len (અક્ષર *tmpl, size_t suff_len, size_t x_len, bool dry_run)
{
  પાછા gen_tempname_len (tmpl, suff_len, 0, dry_run ? GT_NOCREATE : GT_FILE,
                           x_len);
}

સ્થિર પૂર્ણાંક
mkdtemp_len (અક્ષર *tmpl, size_t suff_len, size_t x_len, bool dry_run)
{
  પાછા gen_tempname_len (tmpl, suff_len, 0, dry_run ? GT_NOCREATE : GT_DIR,
                           x_len);
}

/* True if we have already closed standard output.  */
સ્થિર bool stdout_closed;

/* Avoid closing stdout twice.  Since we conditionally call
   close_stream (stdout) in order to decide whether to clean up a
   temporary file, the exit hook needs to know whether to do all of
   close_stdout or just the stderr half.  */
સ્થિર શૂન્ય
maybe_close_stdout (શૂન્ય)
{
  જો (!stdout_closed)
    close_stdout ();
  નહીંતર જો (close_stream (stderr) != 0)
    _exit (EXIT_FAILURE);
}

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  અક્ષર અચલ *dest_dir;
  અક્ષર અચલ *dest_dir_arg = ખાલી;
  bool suppress_file_err = false;
  પૂર્ણાંક c;
  અક્ષર *template;
  અક્ષર *suffix = ખાલી;
  bool use_dest_dir = false;
  bool deprecated_t_option = false;
  bool create_directory = false;
  bool dry_run = false;
  પૂર્ણાંક status = EXIT_SUCCESS;
  size_t x_ગણતરી;
  size_t suffix_len;
  અક્ષર *dest_name = ખાલી;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (maybe_close_stdout);

  જ્યારે ((c = getopt_long (argc, argv, "dp:qtuV", longopts, ખાલી)) != -1)
    {
      પસંદગી (c)
        {
        સ્થિતિ 'd':
          create_directory = true;
          તોડો;
        સ્થિતિ 'p':
          dest_dir_arg = optarg;
          use_dest_dir = true;
          તોડો;
        સ્થિતિ 'q':
          suppress_file_err = true;
          તોડો;
        સ્થિતિ 't':
          use_dest_dir = true;
          deprecated_t_option = true;
          તોડો;
        સ્થિતિ 'u':
          dry_run = true;
          તોડો;

        સ્થિતિ SUFFIX_OPTION:
          suffix = optarg;
          તોડો;

        case_GETOPT_HELP_CHAR;

        સ્થિતિ 'V': /* Undocumented alias, for compatibility with the original
                     mktemp program.  */
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        મૂળભૂત:
          usage (EXIT_FAILURE);
        }
    }

  પૂર્ણાંક n_args = argc - optind;
  જો (2 <= n_args)
    {
      error (0, 0, _("too many templates"));
      usage (EXIT_FAILURE);
    }

  જો (n_args == 0)
    {
      use_dest_dir = true;
      template = (અક્ષર *) default_template;
    }
  નહીંતર
    {
      template = argv[optind];
    }

  જો (suffix)
    {
      size_t len = strlen (template);
      જો (!len || template[len - 1] != 'X')
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
  નહીંતર
    {
      template = xstrdup (template);
      suffix = strrchr (template, 'X');
      જો (!suffix)
        suffix = strchr (template, '\0');
      નહીંતર
        suffix++;
      suffix_len = strlen (suffix);
    }

  /* At this point, template is malloc'd, and suffix points into template.  */
  જો (suffix_len && last_component (suffix) != suffix)
    {
      error (EXIT_FAILURE, 0,
             _("invalid suffix %s, contains directory separator"),
             quote (suffix));
    }
  x_ગણતરી = ગણતરી_consecutive_X_s (template, suffix - template);
  જો (x_ગણતરી < 3)
    {
      /* when suffix was appended to template only diagnose the template.  */
      જો (template == dest_name)
        template[suffix - template] = '\0';
      error (EXIT_FAILURE, 0, _("too few X's in template %s"),
             quote (template));
    }

  જો (use_dest_dir)
    {
      જો (deprecated_t_option)
        {
          અક્ષર *env = getenv ("TMPDIR");
          જો (env && *env)
            dest_dir = env;
          નહીંતર જો (dest_dir_arg && *dest_dir_arg)
            dest_dir = dest_dir_arg;
          નહીંતર
            dest_dir = "/tmp";

          જો (last_component (template) != template)
            error (EXIT_FAILURE, 0,
                   _("invalid template, %s, contains directory separator"),
                   quote (template));
        }
      નહીંતર
        {
          જો (dest_dir_arg && *dest_dir_arg)
            dest_dir = dest_dir_arg;
          નહીંતર
            {
              અક્ષર *env = getenv ("TMPDIR");
              dest_dir = (env && *env ? env : "/tmp");
            }
          જો (IS_ABSOLUTE_FILE_NAME (template))
            error (EXIT_FAILURE, 0,
                   _("invalid template, %s; with --tmpdir,"
                     " it may not be absolute"),
                   quote (template));
        }

      dest_name = file_name_concat (dest_dir, template, ખાલી);
      મુક્ત (template);
      template = dest_name;
      /* Note that suffix is now invalid.  */
    }

  /* Make a copy to be used in case of diagnostic, since failing
     mkstemp may leave the buffer in an undefined state.  */
  dest_name = xstrdup (template);

  જો (create_directory)
    {
      પૂર્ણાંક err = mkdtemp_len (dest_name, suffix_len, x_ગણતરી, dry_run);
      જો (err != 0)
        {
          જો (!suppress_file_err)
            error (0, errno, _("failed to create directory via template %s"),
                   quote (template));
          status = EXIT_FAILURE;
        }
    }
  નહીંતર
    {
      પૂર્ણાંક fd = mkstemp_len (dest_name, suffix_len, x_ગણતરી, dry_run);
      જો (fd < 0 || (!dry_run && close (fd) != 0))
        {
          જો (!suppress_file_err)
            error (0, errno, _("failed to create file via template %s"),
                   quote (template));
          status = EXIT_FAILURE;
        }
    }

  જો (status == EXIT_SUCCESS)
    {
      puts (dest_name);
      /* If we created a file, but then failed to output the file
         name, we should clean up the mess before failing.  */
      જો (!dry_run && ((stdout_closed = true), close_stream (stdout) != 0))
        {
          પૂર્ણાંક saved_errno = errno;
          (create_directory ? rmdir : unlink) (dest_name);
          જો (!suppress_file_err)
            error (0, saved_errno, _("write error"));
          status = EXIT_FAILURE;
        }
    }

  main_exit (status);
}
