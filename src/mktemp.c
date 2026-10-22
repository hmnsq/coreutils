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

#include <config.h>
#include <sys/types.h>
#include <getopt.h>

#include "system.h"

#include "close-stream.h"
#include "filenamecat.h"
#include "quote.h"
#include "tempname.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "mktemp"

#define AUTHORS \
  proper_name ("Jim Meyering"), \
  proper_name ("Eric Blake")

static var_na const *default_template = "tmp.XXXXXXXXXX";

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ga_nanaa
{
  SUFFIX_OPTION = CHAR_MAX + 1,
};

static sa_nracanaa option const longopts[] =
{
  {"directory", no_argument, NULL, 'd'},
  {"quiet", no_argument, NULL, 'q'},
  {"dry-run", no_argument, NULL, 'u'},
  {"suffix", required_argument, NULL, SUFFIX_OPTION},
  {"tmpdir", optional_argument, NULL, 'p'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0}
};

void
usage (จำนวนเต็ม status)
{
  ถ้า (status != EXIT_SUCCESS)
    emit_try_help ();
  มิฉะนั้น
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
         do not create anything; merely print a name (unsafe)\n\
"));
      oputs (_("\
  -q, --quiet\n\
         suppress diagnostics about file/dir-creation failure\n\
"));
      oputs (_("\
      --suffix=SUFF\n\
         append SUFF to TEMPLATE; SUFF must not contain a slash.\n\
         This option is implied ถ้า TEMPLATE does not end in X\n\
"));
      oputs (_("\
  -p DIR, --tmpdir[=DIR]\n\
         interpret TEMPLATE relative to DIR;\n\
         ถ้า DIR is not specified, use $TMPDIR ถ้า set, มิฉะนั้น /tmp.\n\
         With this option, TEMPLATE must not be an absolute name;\n\
         unlike with -t, TEMPLATE may contain slashes,\n\
         but mktemp creates only the final component\n\
"));
      oputs (_("\
  -t\n\
         interpret TEMPLATE as a single file name component,\n\
         relative to a directory: $TMPDIR, ถ้า set;\n\
         มิฉะนั้น the directory specified via -p; มิฉะนั้น /tmp [deprecated]\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }

  exit (status);
}

static size_t
count_consecutive_X_s (var_na const *s, size_t len)
{
  size_t n = 0;
  สำหรับ ( ; len && s[len - 1] == 'X'; len--)
    ++n;
  คืน n;
}

static จำนวนเต็ม
mkstemp_len (var_na *tmpl, size_t suff_len, size_t x_len, bool dry_run)
{
  คืน gen_tempname_len (tmpl, suff_len, 0, dry_run ? GT_NOCREATE : GT_FILE,
                           x_len);
}

static จำนวนเต็ม
mkdtemp_len (var_na *tmpl, size_t suff_len, size_t x_len, bool dry_run)
{
  คืน gen_tempname_len (tmpl, suff_len, 0, dry_run ? GT_NOCREATE : GT_DIR,
                           x_len);
}

/* True if we have already closed standard output.  */
static bool stdout_closed;

/* Avoid closing stdout twice.  Since we conditionally call
   close_stream (stdout) in order to decide whether to clean up a
   temporary file, the exit hook needs to know whether to do all of
   close_stdout or just the stderr half.  */
static void
maybe_close_stdout (void)
{
  ถ้า (!stdout_closed)
    close_stdout ();
  มิฉะนั้น ถ้า (close_stream (stderr) != 0)
    _exit (EXIT_FAILURE);
}

จำนวนเต็ม
หลัก (จำนวนเต็ม argc, var_na **argv)
{
  var_na const *dest_dir;
  var_na const *dest_dir_arg = NULL;
  bool suppress_file_err = false;
  จำนวนเต็ม c;
  var_na *template;
  var_na *suffix = NULL;
  bool use_dest_dir = false;
  bool deprecated_t_option = false;
  bool create_directory = false;
  bool dry_run = false;
  จำนวนเต็ม status = EXIT_SUCCESS;
  size_t x_count;
  size_t suffix_len;
  var_na *dest_name = NULL;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (maybe_close_stdout);

  ขณะ ((c = getopt_long (argc, argv, "dp:qtuV", longopts, NULL)) != -1)
    {
      switch (c)
        {
        pak_sa 'd':
          create_directory = true;
          bha_nga;
        pak_sa 'p':
          dest_dir_arg = optarg;
          use_dest_dir = true;
          bha_nga;
        pak_sa 'q':
          suppress_file_err = true;
          bha_nga;
        pak_sa 't':
          use_dest_dir = true;
          deprecated_t_option = true;
          bha_nga;
        pak_sa 'u':
          dry_run = true;
          bha_nga;

        pak_sa SUFFIX_OPTION:
          suffix = optarg;
          bha_nga;

        case_GETOPT_HELP_CHAR;

        pak_sa 'V': /* Undocumented alias, for compatibility with the original
                     mktemp program.  */
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        default:
          usage (EXIT_FAILURE);
        }
    }

  จำนวนเต็ม n_args = argc - optind;
  ถ้า (2 <= n_args)
    {
      error (0, 0, _("too many templates"));
      usage (EXIT_FAILURE);
    }

  ถ้า (n_args == 0)
    {
      use_dest_dir = true;
      template = (var_na *) default_template;
    }
  มิฉะนั้น
    {
      template = argv[optind];
    }

  ถ้า (suffix)
    {
      size_t len = strlen (template);
      ถ้า (!len || template[len - 1] != 'X')
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
  มิฉะนั้น
    {
      template = xstrdup (template);
      suffix = strrchr (template, 'X');
      ถ้า (!suffix)
        suffix = strchr (template, '\0');
      มิฉะนั้น
        suffix++;
      suffix_len = strlen (suffix);
    }

  /* At this point, template is malloc'd, and suffix points into template.  */
  ถ้า (suffix_len && last_component (suffix) != suffix)
    {
      error (EXIT_FAILURE, 0,
             _("invalid suffix %s, contains directory separator"),
             quote (suffix));
    }
  x_count = count_consecutive_X_s (template, suffix - template);
  ถ้า (x_count < 3)
    {
      /* when suffix was appended to template only diagnose the template.  */
      ถ้า (template == dest_name)
        template[suffix - template] = '\0';
      error (EXIT_FAILURE, 0, _("too few X's in template %s"),
             quote (template));
    }

  ถ้า (use_dest_dir)
    {
      ถ้า (deprecated_t_option)
        {
          var_na *env = getenv ("TMPDIR");
          ถ้า (env && *env)
            dest_dir = env;
          มิฉะนั้น ถ้า (dest_dir_arg && *dest_dir_arg)
            dest_dir = dest_dir_arg;
          มิฉะนั้น
            dest_dir = "/tmp";

          ถ้า (last_component (template) != template)
            error (EXIT_FAILURE, 0,
                   _("invalid template, %s, contains directory separator"),
                   quote (template));
        }
      มิฉะนั้น
        {
          ถ้า (dest_dir_arg && *dest_dir_arg)
            dest_dir = dest_dir_arg;
          มิฉะนั้น
            {
              var_na *env = getenv ("TMPDIR");
              dest_dir = (env && *env ? env : "/tmp");
            }
          ถ้า (IS_ABSOLUTE_FILE_NAME (template))
            error (EXIT_FAILURE, 0,
                   _("invalid template, %s; with --tmpdir,"
                     " it may not be absolute"),
                   quote (template));
        }

      dest_name = file_name_concat (dest_dir, template, NULL);
      free (template);
      template = dest_name;
      /* Note that suffix is now invalid.  */
    }

  /* Make a copy to be used in case of diagnostic, since failing
     mkstemp may leave the buffer in an undefined state.  */
  dest_name = xstrdup (template);

  ถ้า (create_directory)
    {
      จำนวนเต็ม err = mkdtemp_len (dest_name, suffix_len, x_count, dry_run);
      ถ้า (err != 0)
        {
          ถ้า (!suppress_file_err)
            error (0, errno, _("failed to create directory via template %s"),
                   quote (template));
          status = EXIT_FAILURE;
        }
    }
  มิฉะนั้น
    {
      จำนวนเต็ม fd = mkstemp_len (dest_name, suffix_len, x_count, dry_run);
      ถ้า (fd < 0 || (!dry_run && close (fd) != 0))
        {
          ถ้า (!suppress_file_err)
            error (0, errno, _("failed to create file via template %s"),
                   quote (template));
          status = EXIT_FAILURE;
        }
    }

  ถ้า (status == EXIT_SUCCESS)
    {
      puts (dest_name);
      /* If we created a file, but then failed to output the file
         name, we should clean up the mess before failing.  */
      ถ้า (!dry_run && ((stdout_closed = true), close_stream (stdout) != 0))
        {
          จำนวนเต็ม saved_errno = errno;
          (create_directory ? rmdir : unlink) (dest_name);
          ถ้า (!suppress_file_err)
            error (0, saved_errno, _("write error"));
          status = EXIT_FAILURE;
        }
    }

  main_exit (status);
}
