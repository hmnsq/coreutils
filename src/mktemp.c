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

#అన్తర్భావ <config.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <getopt.h>

#అన్తర్భావ "system.h"

#అన్తర్భావ "close-stream.h"
#అన్తర్భావ "filenamecat.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "tempname.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "mktemp"

#నిర్వచించు AUTHORS \
  proper_name ("Jim Meyering"), \
  proper_name ("Eric Blake")

స్థిర అక్షరం స్థిరరాశి *default_template = "tmp.XXXXXXXXXX";

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
గణన
{
  SUFFIX_OPTION = CHAR_MAX + 1,
};

స్థిర నిర్మాణం option స్థిరరాశి longopts[] =
{
  {"directory", no_వాదం, ఖాళీ, 'd'},
  {"quiet", no_వాదం, ఖాళీ, 'q'},
  {"dry-run", no_వాదం, ఖాళీ, 'u'},
  {"suffix", required_వాదం, ఖాళీ, SUFFIX_OPTION},
  {"tmpdir", optional_వాదం, ఖాళీ, 'p'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0}
};

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("Usage: %s [OPTION]... [TEMPLATE]\n"), కార్యక్రమం_name);
      fputs (_("\
Create a temporary file or directory, safely, and print its name.\n\
TEMPLATE must contain at కనిష్ఠ 3 consecutive 'X's లోపల చివరి component.\n\
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
  -u, --dry-నిరంతరం_2\n\
         చేయి not create anything; merely print a name (unsafe)\n\
"));
      oputs (_("\
  -q, --quiet\n\
         suppress diagnostics about file/dir-creation failure\n\
"));
      oputs (_("\
      --suffix=SUFF\n\
         append SUFF గమ్యం TEMPLATE; SUFF must not contain a slash.\n\
         This option is implied ఒకవేళ TEMPLATE does not చివర లోపల X\n\
"));
      oputs (_("\
  -p DIR, --tmpdir[=DIR]\n\
         interpret TEMPLATE relative గమ్యం DIR;\n\
         ఒకవేళ DIR is not specified, use $TMPDIR ఒకవేళ సమితి_2, లేకపోతే /tmp.\n\
         With this option, TEMPLATE must not be an absolute name;\n\
         unlike with -t, TEMPLATE may contain slashes,\n\
         but mktemp creates only the final component\n\
"));
      oputs (_("\
  -t\n\
         interpret TEMPLATE as a single file name component,\n\
         relative గమ్యం a directory: $TMPDIR, ఒకవేళ సమితి_2;\n\
         లేకపోతే the directory specified ద్వారా -p; లేకపోతే /tmp [deprecated]\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }

  నిష్క్రమించు (స్థితి_2);
}

స్థిర size_t
లెక్క_consecutive_X_s (అక్షరం స్థిరరాశి *s, size_t len)
{
  size_t n = 0;
  కొరకు ( ; len && s[len - 1] == 'X'; len--)
    ++n;
  తిరిగి n;
}

స్థిర పూర్ణాంకం
mkstemp_len (అక్షరం *tmpl, size_t suff_len, size_t x_len, bool dry_నిరంతరం)
{
  తిరిగి gen_tempname_len (tmpl, suff_len, 0, dry_నిరంతరం ? GT_NOCREATE : GT_FILE,
                           x_len);
}

స్థిర పూర్ణాంకం
mkdtemp_len (అక్షరం *tmpl, size_t suff_len, size_t x_len, bool dry_నిరంతరం)
{
  తిరిగి gen_tempname_len (tmpl, suff_len, 0, dry_నిరంతరం ? GT_NOCREATE : GT_DIR,
                           x_len);
}

/* True if we have already closed standard output.  */
స్థిర bool stdout_closed;

/* Avoid closing stdout twice.  Since we conditionally call
   close_stream (stdout) in order to decide whether to clean up a
   temporary file, the exit hook needs to know whether to do all of
   close_stdout or just the stderr half.  */
స్థిర శూన్య
maybe_close_stdout (శూన్య)
{
  ఒకవేళ (!stdout_closed)
    close_stdout ();
  లేకపోతే ఒకవేళ (close_ప్రవాహం (stderr) != 0)
    _exit (EXIT_FAILURE);
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  అక్షరం స్థిరరాశి *dest_dir;
  అక్షరం స్థిరరాశి *dest_dir_arg = ఖాళీ;
  bool suppress_file_err = false;
  పూర్ణాంకం c;
  అక్షరం *మూస;
  అక్షరం *suffix = ఖాళీ;
  bool use_dest_dir = false;
  bool deprecated_t_option = false;
  bool create_directory = false;
  bool dry_నిరంతరం = false;
  పూర్ణాంకం స్థితి_2 = EXIT_SUCCESS;
  size_t x_లెక్క;
  size_t suffix_len;
  అక్షరం *dest_name = ఖాళీ;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (maybe_close_stdout);

  అయితే ((c = getopt_long (argc, argv, "dp:qtuV", longopts, ఖాళీ)) != -1)
    {
      ఎంపిక (c)
        {
        స్థితి 'd':
          create_directory = true;
          ఆపు;
        స్థితి 'p':
          dest_dir_arg = optarg;
          use_dest_dir = true;
          ఆపు;
        స్థితి 'q':
          suppress_file_err = true;
          ఆపు;
        స్థితి 't':
          use_dest_dir = true;
          deprecated_t_option = true;
          ఆపు;
        స్థితి 'u':
          dry_నిరంతరం = true;
          ఆపు;

        స్థితి SUFFIX_OPTION:
          suffix = optarg;
          ఆపు;

        case_GETOPT_HELP_CHAR;

        స్థితి 'V': /* Undocumented alias, for compatibility with the original
                     mktemp program.  */
        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);
        అప్రమేయ:
          usage (EXIT_FAILURE);
        }
    }

  పూర్ణాంకం n_వాదనలు = argc - optind;
  ఒకవేళ (2 <= n_వాదనలు)
    {
      error (0, 0, _("too many templates"));
      usage (EXIT_FAILURE);
    }

  ఒకవేళ (n_వాదనలు == 0)
    {
      use_dest_dir = true;
      మూస = (అక్షరం *) default_template;
    }
  లేకపోతే
    {
      మూస = argv[optind];
    }

  ఒకవేళ (suffix)
    {
      size_t len = అక్షరశ్రేణి_పొడవు (మూస);
      ఒకవేళ (!len || మూస[len - 1] != 'X')
        {
          error (EXIT_FAILURE, 0,
                 _("with --suffix, template %s must end in X"),
                 quote (మూస));
        }
      suffix_len = అక్షరశ్రేణి_పొడవు (suffix);
      dest_name = xcharalloc (len + suffix_len + 1);
      జ్ఞాపకం_నకలు (dest_name, మూస, len);
      జ్ఞాపకం_నకలు (dest_name + len, suffix, suffix_len + 1);
      మూస = dest_name;
      suffix = dest_name + len;
    }
  లేకపోతే
    {
      మూస = xstrdup (మూస);
      suffix = strrchr (మూస, 'X');
      ఒకవేళ (!suffix)
        suffix = strchr (మూస, '\0');
      లేకపోతే
        suffix++;
      suffix_len = అక్షరశ్రేణి_పొడవు (suffix);
    }

  /* At this point, template is malloc'd, and suffix points into template.  */
  ఒకవేళ (suffix_len && చివరి_component (suffix) != suffix)
    {
      error (EXIT_FAILURE, 0,
             _("invalid suffix %s, contains directory separator"),
             quote (suffix));
    }
  x_లెక్క = లెక్క_consecutive_X_s (మూస, suffix - మూస);
  ఒకవేళ (x_లెక్క < 3)
    {
      /* when suffix was appended to template only diagnose the template.  */
      ఒకవేళ (మూస == dest_name)
        మూస[suffix - మూస] = '\0';
      error (EXIT_FAILURE, 0, _("too few X's in template %s"),
             quote (మూస));
    }

  ఒకవేళ (use_dest_dir)
    {
      ఒకవేళ (deprecated_t_option)
        {
          అక్షరం *env = getenv ("TMPDIR");
          ఒకవేళ (env && *env)
            dest_dir = env;
          లేకపోతే ఒకవేళ (dest_dir_arg && *dest_dir_arg)
            dest_dir = dest_dir_arg;
          లేకపోతే
            dest_dir = "/tmp";

          ఒకవేళ (చివరి_component (మూస) != మూస)
            error (EXIT_FAILURE, 0,
                   _("invalid template, %s, contains directory separator"),
                   quote (మూస));
        }
      లేకపోతే
        {
          ఒకవేళ (dest_dir_arg && *dest_dir_arg)
            dest_dir = dest_dir_arg;
          లేకపోతే
            {
              అక్షరం *env = getenv ("TMPDIR");
              dest_dir = (env && *env ? env : "/tmp");
            }
          ఒకవేళ (IS_ABSOLUTE_FILE_NAME (మూస))
            error (EXIT_FAILURE, 0,
                   _("invalid template, %s; with --tmpdir,"
                     " it may not be absolute"),
                   quote (మూస));
        }

      dest_name = file_name_concat (dest_dir, మూస, ఖాళీ);
      విడుదల (మూస);
      మూస = dest_name;
      /* Note that suffix is now invalid.  */
    }

  /* Make a copy to be used in case of diagnostic, since failing
     mkstemp may leave the buffer in an undefined state.  */
  dest_name = xstrdup (మూస);

  ఒకవేళ (create_directory)
    {
      పూర్ణాంకం err = mkdtemp_len (dest_name, suffix_len, x_లెక్క, dry_నిరంతరం);
      ఒకవేళ (err != 0)
        {
          ఒకవేళ (!suppress_file_err)
            error (0, errno, _("failed to create directory via template %s"),
                   quote (మూస));
          స్థితి_2 = EXIT_FAILURE;
        }
    }
  లేకపోతే
    {
      పూర్ణాంకం fd = mkstemp_len (dest_name, suffix_len, x_లెక్క, dry_నిరంతరం);
      ఒకవేళ (fd < 0 || (!dry_నిరంతరం && close (fd) != 0))
        {
          ఒకవేళ (!suppress_file_err)
            error (0, errno, _("failed to create file via template %s"),
                   quote (మూస));
          స్థితి_2 = EXIT_FAILURE;
        }
    }

  ఒకవేళ (స్థితి_2 == EXIT_SUCCESS)
    {
      puts (dest_name);
      /* If we created a file, but then failed to output the file
         name, we should clean up the mess before failing.  */
      ఒకవేళ (!dry_నిరంతరం && ((stdout_closed = true), close_ప్రవాహం (stdout) != 0))
        {
          పూర్ణాంకం saved_errno = errno;
          (create_directory ? rmdir : unlink) (dest_name);
          ఒకవేళ (!suppress_file_err)
            error (0, saved_errno, _("write error"));
          స్థితి_2 = EXIT_FAILURE;
        }
    }

  main_exit (స్థితి_2);
}
