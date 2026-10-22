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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <getopt.h>

#অন্তর্ভাব "system.h"

#অন্তর্ভাব "close-stream.h"
#অন্তর্ভাব "filenamecat.h"
#অন্তর্ভাব "quote.h"
#অন্তর্ভাব "tempname.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "mktemp"

#define AUTHORS \
  proper_name ("Jim Meyering"), \
  proper_name ("Eric Blake")

স্থির বর্ণ ধ্রুব *default_template = "tmp.XXXXXXXXXX";

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
গণনা
{
  SUFFIX_OPTION = CHAR_MAX + 1,
};

স্থির সংরচনা option ধ্রুব longopts[] =
{
  {"directory", no_argument, রিক্ত, 'd'},
  {"quiet", no_argument, রিক্ত, 'q'},
  {"dry-run", no_argument, রিক্ত, 'u'},
  {"suffix", required_argument, রিক্ত, SUFFIX_OPTION},
  {"tmpdir", optional_argument, রিক্ত, 'p'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {রিক্ত, 0, রিক্ত, 0}
};

শূন্য
usage (পূর্ণাঙ্ক status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
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
         কুরু not create anything; merely print a name (unsafe)\n\
"));
      oputs (_("\
  -q, --quiet\n\
         suppress diagnostics about file/dir-creation failure\n\
"));
      oputs (_("\
      --suffix=SUFF\n\
         append SUFF to TEMPLATE; SUFF must not contain a slash.\n\
         This option is implied যদি TEMPLATE does not end in X\n\
"));
      oputs (_("\
  -p DIR, --tmpdir[=DIR]\n\
         interpret TEMPLATE relative to DIR;\n\
         যদি DIR is not specified, use $TMPDIR যদি set, অন্যথা /tmp.\n\
         With this option, TEMPLATE must not be an absolute name;\n\
         unlike with -t, TEMPLATE may contain slashes,\n\
         but mktemp creates only the final component\n\
"));
      oputs (_("\
  -t\n\
         interpret TEMPLATE as a single file name component,\n\
         relative to a directory: $TMPDIR, যদি set;\n\
         অন্যথা the directory specified via -p; অন্যথা /tmp [deprecated]\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }

  exit (status);
}

স্থির size_t
গুনতি_consecutive_X_s (বর্ণ ধ্রুব *s, size_t len)
{
  size_t n = 0;
  প্রতি ( ; len && s[len - 1] == 'X'; len--)
    ++n;
  প্রত্যাগম n;
}

স্থির পূর্ণাঙ্ক
mkstemp_len (বর্ণ *tmpl, size_t suff_len, size_t x_len, bool dry_run)
{
  প্রত্যাগম gen_tempname_len (tmpl, suff_len, 0, dry_run ? GT_NOCREATE : GT_FILE,
                           x_len);
}

স্থির পূর্ণাঙ্ক
mkdtemp_len (বর্ণ *tmpl, size_t suff_len, size_t x_len, bool dry_run)
{
  প্রত্যাগম gen_tempname_len (tmpl, suff_len, 0, dry_run ? GT_NOCREATE : GT_DIR,
                           x_len);
}

/* True if we have already closed standard output.  */
স্থির bool stdout_closed;

/* Avoid closing stdout twice.  Since we conditionally call
   close_stream (stdout) in order to decide whether to clean up a
   temporary file, the exit hook needs to know whether to do all of
   close_stdout or just the stderr half.  */
স্থির শূন্য
maybe_close_stdout (শূন্য)
{
  যদি (!stdout_closed)
    close_stdout ();
  অন্যথা যদি (close_stream (stderr) != 0)
    _exit (EXIT_FAILURE);
}

পূর্ণাঙ্ক
main (পূর্ণাঙ্ক argc, বর্ণ **argv)
{
  বর্ণ ধ্রুব *dest_dir;
  বর্ণ ধ্রুব *dest_dir_arg = রিক্ত;
  bool suppress_file_err = false;
  পূর্ণাঙ্ক c;
  বর্ণ *template;
  বর্ণ *suffix = রিক্ত;
  bool use_dest_dir = false;
  bool deprecated_t_option = false;
  bool create_directory = false;
  bool dry_run = false;
  পূর্ণাঙ্ক status = EXIT_SUCCESS;
  size_t x_গুনতি;
  size_t suffix_len;
  বর্ণ *dest_name = রিক্ত;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (maybe_close_stdout);

  যাবত্ ((c = getopt_long (argc, argv, "dp:qtuV", longopts, রিক্ত)) != -1)
    {
      বিকল্প (c)
        {
        পক্ষ 'd':
          create_directory = true;
          ভঙ্গ;
        পক্ষ 'p':
          dest_dir_arg = optarg;
          use_dest_dir = true;
          ভঙ্গ;
        পক্ষ 'q':
          suppress_file_err = true;
          ভঙ্গ;
        পক্ষ 't':
          use_dest_dir = true;
          deprecated_t_option = true;
          ভঙ্গ;
        পক্ষ 'u':
          dry_run = true;
          ভঙ্গ;

        পক্ষ SUFFIX_OPTION:
          suffix = optarg;
          ভঙ্গ;

        case_GETOPT_HELP_CHAR;

        পক্ষ 'V': /* Undocumented alias, for compatibility with the original
                     mktemp program.  */
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        স্বভাব:
          usage (EXIT_FAILURE);
        }
    }

  পূর্ণাঙ্ক n_args = argc - optind;
  যদি (2 <= n_args)
    {
      error (0, 0, _("too many templates"));
      usage (EXIT_FAILURE);
    }

  যদি (n_args == 0)
    {
      use_dest_dir = true;
      template = (বর্ণ *) default_template;
    }
  অন্যথা
    {
      template = argv[optind];
    }

  যদি (suffix)
    {
      size_t len = strlen (template);
      যদি (!len || template[len - 1] != 'X')
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
  অন্যথা
    {
      template = xstrdup (template);
      suffix = strrchr (template, 'X');
      যদি (!suffix)
        suffix = strchr (template, '\0');
      অন্যথা
        suffix++;
      suffix_len = strlen (suffix);
    }

  /* At this point, template is malloc'd, and suffix points into template.  */
  যদি (suffix_len && last_component (suffix) != suffix)
    {
      error (EXIT_FAILURE, 0,
             _("invalid suffix %s, contains directory separator"),
             quote (suffix));
    }
  x_গুনতি = গুনতি_consecutive_X_s (template, suffix - template);
  যদি (x_গুনতি < 3)
    {
      /* when suffix was appended to template only diagnose the template.  */
      যদি (template == dest_name)
        template[suffix - template] = '\0';
      error (EXIT_FAILURE, 0, _("too few X's in template %s"),
             quote (template));
    }

  যদি (use_dest_dir)
    {
      যদি (deprecated_t_option)
        {
          বর্ণ *env = getenv ("TMPDIR");
          যদি (env && *env)
            dest_dir = env;
          অন্যথা যদি (dest_dir_arg && *dest_dir_arg)
            dest_dir = dest_dir_arg;
          অন্যথা
            dest_dir = "/tmp";

          যদি (last_component (template) != template)
            error (EXIT_FAILURE, 0,
                   _("invalid template, %s, contains directory separator"),
                   quote (template));
        }
      অন্যথা
        {
          যদি (dest_dir_arg && *dest_dir_arg)
            dest_dir = dest_dir_arg;
          অন্যথা
            {
              বর্ণ *env = getenv ("TMPDIR");
              dest_dir = (env && *env ? env : "/tmp");
            }
          যদি (IS_ABSOLUTE_FILE_NAME (template))
            error (EXIT_FAILURE, 0,
                   _("invalid template, %s; with --tmpdir,"
                     " it may not be absolute"),
                   quote (template));
        }

      dest_name = file_name_concat (dest_dir, template, রিক্ত);
      মুক্ত (template);
      template = dest_name;
      /* Note that suffix is now invalid.  */
    }

  /* Make a copy to be used in case of diagnostic, since failing
     mkstemp may leave the buffer in an undefined state.  */
  dest_name = xstrdup (template);

  যদি (create_directory)
    {
      পূর্ণাঙ্ক err = mkdtemp_len (dest_name, suffix_len, x_গুনতি, dry_run);
      যদি (err != 0)
        {
          যদি (!suppress_file_err)
            error (0, errno, _("failed to create directory via template %s"),
                   quote (template));
          status = EXIT_FAILURE;
        }
    }
  অন্যথা
    {
      পূর্ণাঙ্ক fd = mkstemp_len (dest_name, suffix_len, x_গুনতি, dry_run);
      যদি (fd < 0 || (!dry_run && close (fd) != 0))
        {
          যদি (!suppress_file_err)
            error (0, errno, _("failed to create file via template %s"),
                   quote (template));
          status = EXIT_FAILURE;
        }
    }

  যদি (status == EXIT_SUCCESS)
    {
      puts (dest_name);
      /* If we created a file, but then failed to output the file
         name, we should clean up the mess before failing.  */
      যদি (!dry_run && ((stdout_closed = true), close_stream (stdout) != 0))
        {
          পূর্ণাঙ্ক saved_errno = errno;
          (create_directory ? rmdir : unlink) (dest_name);
          যদি (!suppress_file_err)
            error (0, saved_errno, _("write error"));
          status = EXIT_FAILURE;
        }
    }

  main_exit (status);
}
