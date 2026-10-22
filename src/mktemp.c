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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <getopt.h>

#அந்தர்பாவ "system.h"

#அந்தர்பாவ "close-stream.h"
#அந்தர்பாவ "filenamecat.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "tempname.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "mktemp"

#define AUTHORS \
  proper_name ("Jim Meyering"), \
  proper_name ("Eric Blake")

நிலையான எழுத்து மாறிலி *default_template = "tmp.XXXXXXXXXX";

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
எண்ணிக்கை
{
  SUFFIX_OPTION = CHAR_MAX + 1,
};

நிலையான கட்டமைப்பு option மாறிலி longopts[] =
{
  {"directory", no_argument, வெற்று, 'd'},
  {"quiet", no_argument, வெற்று, 'q'},
  {"dry-run", no_argument, வெற்று, 'u'},
  {"suffix", required_argument, வெற்று, SUFFIX_OPTION},
  {"tmpdir", optional_argument, வெற்று, 'p'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {வெற்று, 0, வெற்று, 0}
};

வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
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
         செய் not create anything; merely print a name (unsafe)\n\
"));
      oputs (_("\
  -q, --quiet\n\
         suppress diagnostics about file/dir-creation failure\n\
"));
      oputs (_("\
      --suffix=SUFF\n\
         append SUFF to TEMPLATE; SUFF must not contain a slash.\n\
         This option is implied எனில் TEMPLATE does not end in X\n\
"));
      oputs (_("\
  -p DIR, --tmpdir[=DIR]\n\
         interpret TEMPLATE relative to DIR;\n\
         எனில் DIR is not specified, use $TMPDIR எனில் set, இல்லையெனில் /tmp.\n\
         With this option, TEMPLATE must not be an absolute name;\n\
         unlike with -t, TEMPLATE may contain slashes,\n\
         but mktemp creates only the final component\n\
"));
      oputs (_("\
  -t\n\
         interpret TEMPLATE as a single file name component,\n\
         relative to a directory: $TMPDIR, எனில் set;\n\
         இல்லையெனில் the directory specified via -p; இல்லையெனில் /tmp [deprecated]\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }

  exit (status);
}

நிலையான size_t
மொத்தம்_consecutive_X_s (எழுத்து மாறிலி *s, size_t len)
{
  size_t n = 0;
  க்கு ( ; len && s[len - 1] == 'X'; len--)
    ++n;
  திரும்பு n;
}

நிலையான முழு
mkstemp_len (எழுத்து *tmpl, size_t suff_len, size_t x_len, bool dry_run)
{
  திரும்பு gen_tempname_len (tmpl, suff_len, 0, dry_run ? GT_NOCREATE : GT_FILE,
                           x_len);
}

நிலையான முழு
mkdtemp_len (எழுத்து *tmpl, size_t suff_len, size_t x_len, bool dry_run)
{
  திரும்பு gen_tempname_len (tmpl, suff_len, 0, dry_run ? GT_NOCREATE : GT_DIR,
                           x_len);
}

/* True if we have already closed standard output.  */
நிலையான bool stdout_closed;

/* Avoid closing stdout twice.  Since we conditionally call
   close_stream (stdout) in order to decide whether to clean up a
   temporary file, the exit hook needs to know whether to do all of
   close_stdout or just the stderr half.  */
நிலையான வெறுமை
maybe_close_stdout (வெறுமை)
{
  எனில் (!stdout_closed)
    close_stdout ();
  இல்லையெனில் எனில் (close_stream (stderr) != 0)
    _exit (EXIT_FAILURE);
}

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  எழுத்து மாறிலி *dest_dir;
  எழுத்து மாறிலி *dest_dir_arg = வெற்று;
  bool suppress_file_err = false;
  முழு c;
  எழுத்து *template;
  எழுத்து *suffix = வெற்று;
  bool use_dest_dir = false;
  bool deprecated_t_option = false;
  bool create_directory = false;
  bool dry_run = false;
  முழு status = EXIT_SUCCESS;
  size_t x_மொத்தம்;
  size_t suffix_len;
  எழுத்து *dest_name = வெற்று;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (maybe_close_stdout);

  போது ((c = getopt_long (argc, argv, "dp:qtuV", longopts, வெற்று)) != -1)
    {
      தேர்வு (c)
        {
        நிலை 'd':
          create_directory = true;
          நிறுத்து;
        நிலை 'p':
          dest_dir_arg = optarg;
          use_dest_dir = true;
          நிறுத்து;
        நிலை 'q':
          suppress_file_err = true;
          நிறுத்து;
        நிலை 't':
          use_dest_dir = true;
          deprecated_t_option = true;
          நிறுத்து;
        நிலை 'u':
          dry_run = true;
          நிறுத்து;

        நிலை SUFFIX_OPTION:
          suffix = optarg;
          நிறுத்து;

        case_GETOPT_HELP_CHAR;

        நிலை 'V': /* Undocumented alias, for compatibility with the original
                     mktemp program.  */
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        இயல்பு:
          usage (EXIT_FAILURE);
        }
    }

  முழு n_args = argc - optind;
  எனில் (2 <= n_args)
    {
      error (0, 0, _("too many templates"));
      usage (EXIT_FAILURE);
    }

  எனில் (n_args == 0)
    {
      use_dest_dir = true;
      template = (எழுத்து *) default_template;
    }
  இல்லையெனில்
    {
      template = argv[optind];
    }

  எனில் (suffix)
    {
      size_t len = strlen (template);
      எனில் (!len || template[len - 1] != 'X')
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
  இல்லையெனில்
    {
      template = xstrdup (template);
      suffix = strrchr (template, 'X');
      எனில் (!suffix)
        suffix = strchr (template, '\0');
      இல்லையெனில்
        suffix++;
      suffix_len = strlen (suffix);
    }

  /* At this point, template is malloc'd, and suffix points into template.  */
  எனில் (suffix_len && last_component (suffix) != suffix)
    {
      error (EXIT_FAILURE, 0,
             _("invalid suffix %s, contains directory separator"),
             quote (suffix));
    }
  x_மொத்தம் = மொத்தம்_consecutive_X_s (template, suffix - template);
  எனில் (x_மொத்தம் < 3)
    {
      /* when suffix was appended to template only diagnose the template.  */
      எனில் (template == dest_name)
        template[suffix - template] = '\0';
      error (EXIT_FAILURE, 0, _("too few X's in template %s"),
             quote (template));
    }

  எனில் (use_dest_dir)
    {
      எனில் (deprecated_t_option)
        {
          எழுத்து *env = getenv ("TMPDIR");
          எனில் (env && *env)
            dest_dir = env;
          இல்லையெனில் எனில் (dest_dir_arg && *dest_dir_arg)
            dest_dir = dest_dir_arg;
          இல்லையெனில்
            dest_dir = "/tmp";

          எனில் (last_component (template) != template)
            error (EXIT_FAILURE, 0,
                   _("invalid template, %s, contains directory separator"),
                   quote (template));
        }
      இல்லையெனில்
        {
          எனில் (dest_dir_arg && *dest_dir_arg)
            dest_dir = dest_dir_arg;
          இல்லையெனில்
            {
              எழுத்து *env = getenv ("TMPDIR");
              dest_dir = (env && *env ? env : "/tmp");
            }
          எனில் (IS_ABSOLUTE_FILE_NAME (template))
            error (EXIT_FAILURE, 0,
                   _("invalid template, %s; with --tmpdir,"
                     " it may not be absolute"),
                   quote (template));
        }

      dest_name = file_name_concat (dest_dir, template, வெற்று);
      விடுவி (template);
      template = dest_name;
      /* Note that suffix is now invalid.  */
    }

  /* Make a copy to be used in case of diagnostic, since failing
     mkstemp may leave the buffer in an undefined state.  */
  dest_name = xstrdup (template);

  எனில் (create_directory)
    {
      முழு err = mkdtemp_len (dest_name, suffix_len, x_மொத்தம், dry_run);
      எனில் (err != 0)
        {
          எனில் (!suppress_file_err)
            error (0, errno, _("failed to create directory via template %s"),
                   quote (template));
          status = EXIT_FAILURE;
        }
    }
  இல்லையெனில்
    {
      முழு fd = mkstemp_len (dest_name, suffix_len, x_மொத்தம், dry_run);
      எனில் (fd < 0 || (!dry_run && close (fd) != 0))
        {
          எனில் (!suppress_file_err)
            error (0, errno, _("failed to create file via template %s"),
                   quote (template));
          status = EXIT_FAILURE;
        }
    }

  எனில் (status == EXIT_SUCCESS)
    {
      puts (dest_name);
      /* If we created a file, but then failed to output the file
         name, we should clean up the mess before failing.  */
      எனில் (!dry_run && ((stdout_closed = true), close_stream (stdout) != 0))
        {
          முழு saved_errno = errno;
          (create_directory ? rmdir : unlink) (dest_name);
          எனில் (!suppress_file_err)
            error (0, saved_errno, _("write error"));
          status = EXIT_FAILURE;
        }
    }

  main_exit (status);
}
