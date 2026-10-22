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

#ضمن <config.h>
#ضمن <sys/types.h>
#ضمن <getopt.h>

#ضمن "system.h"

#ضمن "close-stream.h"
#ضمن "filenamecat.h"
#ضمن "quote.h"
#ضمن "tempname.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "mktemp"

#define AUTHORS \
  proper_name ("Jim Meyering"), \
  proper_name ("Eric Blake")

ساكن حرف ثابت *default_template = "tmp.XXXXXXXXXX";

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
تعداد
{
  SUFFIX_OPTION = CHAR_MAX + 1,
};

ساكن بنية option ثابت longopts[] =
{
  {"directory", no_argument, عدم, 'd'},
  {"quiet", no_argument, عدم, 'q'},
  {"dry-run", no_argument, عدم, 'u'},
  {"suffix", required_argument, عدم, SUFFIX_OPTION},
  {"tmpdir", optional_argument, عدم, 'p'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح status)
{
  إذا (status != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
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
         افعل not create anything; merely print a name (unsafe)\n\
"));
      oputs (_("\
  -q, --quiet\n\
         suppress diagnostics about file/dir-creation failure\n\
"));
      oputs (_("\
      --suffix=SUFF\n\
         append SUFF to TEMPLATE; SUFF must not contain a slash.\n\
         This option is implied إذا TEMPLATE does not end in X\n\
"));
      oputs (_("\
  -p DIR, --tmpdir[=DIR]\n\
         interpret TEMPLATE relative to DIR;\n\
         إذا DIR is not specified, use $TMPDIR إذا set, وإلا /tmp.\n\
         With this option, TEMPLATE must not be an absolute name;\n\
         unlike with -t, TEMPLATE may contain slashes,\n\
         but mktemp creates only the final component\n\
"));
      oputs (_("\
  -t\n\
         interpret TEMPLATE as a single file name component,\n\
         relative to a directory: $TMPDIR, إذا set;\n\
         وإلا the directory specified via -p; وإلا /tmp [deprecated]\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }

  exit (status);
}

ساكن size_t
کيراءن_consecutive_X_s (حرف ثابت *s, size_t len)
{
  size_t n = 0;
  لكل ( ; len && s[len - 1] == 'X'; len--)
    ++n;
  أرجع n;
}

ساكن صحيح
mkstemp_len (حرف *tmpl, size_t suff_len, size_t x_len, bool dry_run)
{
  أرجع gen_tempname_len (tmpl, suff_len, 0, dry_run ? GT_NOCREATE : GT_FILE,
                           x_len);
}

ساكن صحيح
mkdtemp_len (حرف *tmpl, size_t suff_len, size_t x_len, bool dry_run)
{
  أرجع gen_tempname_len (tmpl, suff_len, 0, dry_run ? GT_NOCREATE : GT_DIR,
                           x_len);
}

/* True if we have already closed standard output.  */
ساكن bool stdout_closed;

/* Avoid closing stdout twice.  Since we conditionally call
   close_stream (stdout) in order to decide whether to clean up a
   temporary file, the exit hook needs to know whether to do all of
   close_stdout or just the stderr half.  */
ساكن فراغ
maybe_close_stdout (فراغ)
{
  إذا (!stdout_closed)
    close_stdout ();
  وإلا إذا (close_stream (stderr) != 0)
    _exit (EXIT_FAILURE);
}

صحيح
main (صحيح argc, حرف **argv)
{
  حرف ثابت *dest_dir;
  حرف ثابت *dest_dir_arg = عدم;
  bool suppress_file_err = false;
  صحيح c;
  حرف *template;
  حرف *suffix = عدم;
  bool use_dest_dir = false;
  bool deprecated_t_option = false;
  bool create_directory = false;
  bool dry_run = false;
  صحيح status = EXIT_SUCCESS;
  size_t x_کيراءن;
  size_t suffix_len;
  حرف *dest_name = عدم;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (maybe_close_stdout);

  طالما ((c = getopt_long (argc, argv, "dp:qtuV", longopts, عدم)) != -1)
    {
      بدل (c)
        {
        حالة 'd':
          create_directory = true;
          اكسر;
        حالة 'p':
          dest_dir_arg = optarg;
          use_dest_dir = true;
          اكسر;
        حالة 'q':
          suppress_file_err = true;
          اكسر;
        حالة 't':
          use_dest_dir = true;
          deprecated_t_option = true;
          اكسر;
        حالة 'u':
          dry_run = true;
          اكسر;

        حالة SUFFIX_OPTION:
          suffix = optarg;
          اكسر;

        case_GETOPT_HELP_CHAR;

        حالة 'V': /* Undocumented alias, for compatibility with the original
                     mktemp program.  */
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  صحيح n_args = argc - optind;
  إذا (2 <= n_args)
    {
      error (0, 0, _("too many templates"));
      usage (EXIT_FAILURE);
    }

  إذا (n_args == 0)
    {
      use_dest_dir = true;
      template = (حرف *) default_template;
    }
  وإلا
    {
      template = argv[optind];
    }

  إذا (suffix)
    {
      size_t len = strlen (template);
      إذا (!len || template[len - 1] != 'X')
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
  وإلا
    {
      template = xstrdup (template);
      suffix = strrchr (template, 'X');
      إذا (!suffix)
        suffix = strchr (template, '\0');
      وإلا
        suffix++;
      suffix_len = strlen (suffix);
    }

  /* At this point, template is malloc'd, and suffix points into template.  */
  إذا (suffix_len && last_component (suffix) != suffix)
    {
      error (EXIT_FAILURE, 0,
             _("invalid suffix %s, contains directory separator"),
             quote (suffix));
    }
  x_کيراءن = کيراءن_consecutive_X_s (template, suffix - template);
  إذا (x_کيراءن < 3)
    {
      /* when suffix was appended to template only diagnose the template.  */
      إذا (template == dest_name)
        template[suffix - template] = '\0';
      error (EXIT_FAILURE, 0, _("too few X's in template %s"),
             quote (template));
    }

  إذا (use_dest_dir)
    {
      إذا (deprecated_t_option)
        {
          حرف *env = getenv ("TMPDIR");
          إذا (env && *env)
            dest_dir = env;
          وإلا إذا (dest_dir_arg && *dest_dir_arg)
            dest_dir = dest_dir_arg;
          وإلا
            dest_dir = "/tmp";

          إذا (last_component (template) != template)
            error (EXIT_FAILURE, 0,
                   _("invalid template, %s, contains directory separator"),
                   quote (template));
        }
      وإلا
        {
          إذا (dest_dir_arg && *dest_dir_arg)
            dest_dir = dest_dir_arg;
          وإلا
            {
              حرف *env = getenv ("TMPDIR");
              dest_dir = (env && *env ? env : "/tmp");
            }
          إذا (IS_ABSOLUTE_FILE_NAME (template))
            error (EXIT_FAILURE, 0,
                   _("invalid template, %s; with --tmpdir,"
                     " it may not be absolute"),
                   quote (template));
        }

      dest_name = file_name_concat (dest_dir, template, عدم);
      حرر (template);
      template = dest_name;
      /* Note that suffix is now invalid.  */
    }

  /* Make a copy to be used in case of diagnostic, since failing
     mkstemp may leave the buffer in an undefined state.  */
  dest_name = xstrdup (template);

  إذا (create_directory)
    {
      صحيح err = mkdtemp_len (dest_name, suffix_len, x_کيراءن, dry_run);
      إذا (err != 0)
        {
          إذا (!suppress_file_err)
            error (0, errno, _("failed to create directory via template %s"),
                   quote (template));
          status = EXIT_FAILURE;
        }
    }
  وإلا
    {
      صحيح fd = mkstemp_len (dest_name, suffix_len, x_کيراءن, dry_run);
      إذا (fd < 0 || (!dry_run && close (fd) != 0))
        {
          إذا (!suppress_file_err)
            error (0, errno, _("failed to create file via template %s"),
                   quote (template));
          status = EXIT_FAILURE;
        }
    }

  إذا (status == EXIT_SUCCESS)
    {
      puts (dest_name);
      /* If we created a file, but then failed to output the file
         name, we should clean up the mess before failing.  */
      إذا (!dry_run && ((stdout_closed = true), close_stream (stdout) != 0))
        {
          صحيح saved_errno = errno;
          (create_directory ? rmdir : unlink) (dest_name);
          إذا (!suppress_file_err)
            error (0, saved_errno, _("write error"));
          status = EXIT_FAILURE;
        }
    }

  main_exit (status);
}
