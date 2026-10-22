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
usage (ຈຳນວນເຕັມ status)
{
  ຖ້າ (status != EXIT_SUCCESS)
    emit_try_help ();
  ບໍ່ດັ່ງນັ້ນ
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
         This option is implied ຖ້າ TEMPLATE does not end in X\n\
"));
      oputs (_("\
  -p DIR, --tmpdir[=DIR]\n\
         interpret TEMPLATE relative to DIR;\n\
         ຖ້າ DIR is not specified, use $TMPDIR ຖ້າ set, ບໍ່ດັ່ງນັ້ນ /tmp.\n\
         With this option, TEMPLATE must not be an absolute name;\n\
         unlike with -t, TEMPLATE may contain slashes,\n\
         but mktemp creates only the final component\n\
"));
      oputs (_("\
  -t\n\
         interpret TEMPLATE as a single file name component,\n\
         relative to a directory: $TMPDIR, ຖ້າ set;\n\
         ບໍ່ດັ່ງນັ້ນ the directory specified via -p; ບໍ່ດັ່ງນັ້ນ /tmp [deprecated]\n\
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
  ສຳລັບ ( ; len && s[len - 1] == 'X'; len--)
    ++n;
  ສົ່ງຄືນ n;
}

static ຈຳນວນເຕັມ
mkstemp_len (var_na *tmpl, size_t suff_len, size_t x_len, bool dry_run)
{
  ສົ່ງຄືນ gen_tempname_len (tmpl, suff_len, 0, dry_run ? GT_NOCREATE : GT_FILE,
                           x_len);
}

static ຈຳນວນເຕັມ
mkdtemp_len (var_na *tmpl, size_t suff_len, size_t x_len, bool dry_run)
{
  ສົ່ງຄືນ gen_tempname_len (tmpl, suff_len, 0, dry_run ? GT_NOCREATE : GT_DIR,
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
  ຖ້າ (!stdout_closed)
    close_stdout ();
  ບໍ່ດັ່ງນັ້ນ ຖ້າ (close_stream (stderr) != 0)
    _exit (EXIT_FAILURE);
}

ຈຳນວນເຕັມ
main (ຈຳນວນເຕັມ argc, var_na **argv)
{
  var_na const *dest_dir;
  var_na const *dest_dir_arg = NULL;
  bool suppress_file_err = false;
  ຈຳນວນເຕັມ c;
  var_na *template;
  var_na *suffix = NULL;
  bool use_dest_dir = false;
  bool deprecated_t_option = false;
  bool create_directory = false;
  bool dry_run = false;
  ຈຳນວນເຕັມ status = EXIT_SUCCESS;
  size_t x_count;
  size_t suffix_len;
  var_na *dest_name = NULL;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (maybe_close_stdout);

  ໃນຂະນະທີ່ ((c = getopt_long (argc, argv, "dp:qtuV", longopts, NULL)) != -1)
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

  ຈຳນວນເຕັມ n_args = argc - optind;
  ຖ້າ (2 <= n_args)
    {
      error (0, 0, _("too many templates"));
      usage (EXIT_FAILURE);
    }

  ຖ້າ (n_args == 0)
    {
      use_dest_dir = true;
      template = (var_na *) default_template;
    }
  ບໍ່ດັ່ງນັ້ນ
    {
      template = argv[optind];
    }

  ຖ້າ (suffix)
    {
      size_t len = strlen (template);
      ຖ້າ (!len || template[len - 1] != 'X')
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
  ບໍ່ດັ່ງນັ້ນ
    {
      template = xstrdup (template);
      suffix = strrchr (template, 'X');
      ຖ້າ (!suffix)
        suffix = strchr (template, '\0');
      ບໍ່ດັ່ງນັ້ນ
        suffix++;
      suffix_len = strlen (suffix);
    }

  /* At this point, template is malloc'd, and suffix points into template.  */
  ຖ້າ (suffix_len && last_component (suffix) != suffix)
    {
      error (EXIT_FAILURE, 0,
             _("invalid suffix %s, contains directory separator"),
             quote (suffix));
    }
  x_count = count_consecutive_X_s (template, suffix - template);
  ຖ້າ (x_count < 3)
    {
      /* when suffix was appended to template only diagnose the template.  */
      ຖ້າ (template == dest_name)
        template[suffix - template] = '\0';
      error (EXIT_FAILURE, 0, _("too few X's in template %s"),
             quote (template));
    }

  ຖ້າ (use_dest_dir)
    {
      ຖ້າ (deprecated_t_option)
        {
          var_na *env = getenv ("TMPDIR");
          ຖ້າ (env && *env)
            dest_dir = env;
          ບໍ່ດັ່ງນັ້ນ ຖ້າ (dest_dir_arg && *dest_dir_arg)
            dest_dir = dest_dir_arg;
          ບໍ່ດັ່ງນັ້ນ
            dest_dir = "/tmp";

          ຖ້າ (last_component (template) != template)
            error (EXIT_FAILURE, 0,
                   _("invalid template, %s, contains directory separator"),
                   quote (template));
        }
      ບໍ່ດັ່ງນັ້ນ
        {
          ຖ້າ (dest_dir_arg && *dest_dir_arg)
            dest_dir = dest_dir_arg;
          ບໍ່ດັ່ງນັ້ນ
            {
              var_na *env = getenv ("TMPDIR");
              dest_dir = (env && *env ? env : "/tmp");
            }
          ຖ້າ (IS_ABSOLUTE_FILE_NAME (template))
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

  ຖ້າ (create_directory)
    {
      ຈຳນວນເຕັມ err = mkdtemp_len (dest_name, suffix_len, x_count, dry_run);
      ຖ້າ (err != 0)
        {
          ຖ້າ (!suppress_file_err)
            error (0, errno, _("failed to create directory via template %s"),
                   quote (template));
          status = EXIT_FAILURE;
        }
    }
  ບໍ່ດັ່ງນັ້ນ
    {
      ຈຳນວນເຕັມ fd = mkstemp_len (dest_name, suffix_len, x_count, dry_run);
      ຖ້າ (fd < 0 || (!dry_run && close (fd) != 0))
        {
          ຖ້າ (!suppress_file_err)
            error (0, errno, _("failed to create file via template %s"),
                   quote (template));
          status = EXIT_FAILURE;
        }
    }

  ຖ້າ (status == EXIT_SUCCESS)
    {
      puts (dest_name);
      /* If we created a file, but then failed to output the file
         name, we should clean up the mess before failing.  */
      ຖ້າ (!dry_run && ((stdout_closed = true), close_stream (stdout) != 0))
        {
          ຈຳນວນເຕັມ saved_errno = errno;
          (create_directory ? rmdir : unlink) (dest_name);
          ຖ້າ (!suppress_file_err)
            error (0, saved_errno, _("write error"));
          status = EXIT_FAILURE;
        }
    }

  main_exit (status);
}
