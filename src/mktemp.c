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

#अन्तर्भाव <config.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <getopt.h>

#अन्तर्भाव "system.h"

#अन्तर्भाव "close-stream.h"
#अन्तर्भाव "filenamecat.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "tempname.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#परिभाषय क्रमादेशः_NAME "mktemp"

#परिभाषय AUTHORS \
  proper_name ("Jim Meyering"), \
  proper_name ("Eric Blake")

स्थिर वर्ण ध्रुव *default_template = "tmp.XXXXXXXXXX";

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
गणना
{
  SUFFIX_OPTION = CHAR_MAX + 1,
};

स्थिर संरचना option ध्रुव longopts[] =
{
  {"directory", no_तर्कः, रिक्त, 'd'},
  {"quiet", no_तर्कः, रिक्त, 'q'},
  {"dry-run", no_तर्कः, रिक्त, 'u'},
  {"suffix", required_तर्कः, रिक्त, SUFFIX_OPTION},
  {"tmpdir", optional_तर्कः, रिक्त, 'p'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_संस्करणम्_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

शून्य
usage (पूर्णाङ्क स्थितिः)
{
  यदि (स्थितिः != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
    {
      प्रारूप_लिख (_("Usage: %s [OPTION]... [TEMPLATE]\n"), क्रमादेशः_name);
      fputs (_("\
Create a temporary file or directory, safely, and print its name.\n\
TEMPLATE must contain at न्यूनतमम् 3 consecutive 'X's अन्तर् चरमः component.\n\
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
  -u, --dry-सततम्_2\n\
         कुरु not create anything; merely print a name (unsafe)\n\
"));
      oputs (_("\
  -q, --quiet\n\
         suppress diagnostics about file/dir-creation failure\n\
"));
      oputs (_("\
      --suffix=SUFF\n\
         append SUFF गन्तव्यम् TEMPLATE; SUFF must not contain a slash.\n\
         This option is implied यदि TEMPLATE does not अन्तः अन्तर् X\n\
"));
      oputs (_("\
  -p DIR, --tmpdir[=DIR]\n\
         interpret TEMPLATE relative गन्तव्यम् DIR;\n\
         यदि DIR is not specified, use $TMPDIR यदि समुच्चयः_2, अन्यथा /tmp.\n\
         With this option, TEMPLATE must not be an absolute name;\n\
         unlike with -t, TEMPLATE may contain slashes,\n\
         but mktemp creates only the final component\n\
"));
      oputs (_("\
  -t\n\
         interpret TEMPLATE as a single file name component,\n\
         relative गन्तव्यम् a directory: $TMPDIR, यदि समुच्चयः_2;\n\
         अन्यथा the directory specified द्वारा -p; अन्यथा /tmp [deprecated]\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (संस्करणम्_OPTION_DESCRIPTION);
      emit_ancillary_info (क्रमादेशः_NAME);
    }

  निर्गच्छ (स्थितिः);
}

स्थिर size_t
गणनम्_consecutive_X_s (वर्ण ध्रुव *s, size_t len)
{
  size_t n = 0;
  प्रति ( ; len && s[len - 1] == 'X'; len--)
    ++n;
  प्रत्यागम n;
}

स्थिर पूर्णाङ्क
mkstemp_len (वर्ण *tmpl, size_t suff_len, size_t x_len, bool dry_सततम्)
{
  प्रत्यागम gen_tempname_len (tmpl, suff_len, 0, dry_सततम् ? GT_NOCREATE : GT_FILE,
                           x_len);
}

स्थिर पूर्णाङ्क
mkdtemp_len (वर्ण *tmpl, size_t suff_len, size_t x_len, bool dry_सततम्)
{
  प्रत्यागम gen_tempname_len (tmpl, suff_len, 0, dry_सततम् ? GT_NOCREATE : GT_DIR,
                           x_len);
}

/* True if we have already closed standard output.  */
स्थिर bool stdout_closed;

/* Avoid closing stdout twice.  Since we conditionally call
   close_stream (stdout) in order to decide whether to clean up a
   temporary file, the exit hook needs to know whether to do all of
   close_stdout or just the stderr half.  */
स्थिर शून्य
maybe_close_stdout (शून्य)
{
  यदि (!stdout_closed)
    close_stdout ();
  अन्यथा यदि (close_प्रवाहः (stderr) != 0)
    _exit (EXIT_FAILURE);
}

पूर्णाङ्क
मुख्यम् (पूर्णाङ्क argc, वर्ण **argv)
{
  वर्ण ध्रुव *dest_dir;
  वर्ण ध्रुव *dest_dir_arg = रिक्त;
  bool suppress_file_err = false;
  पूर्णाङ्क c;
  वर्ण *प्रतिरूपम्;
  वर्ण *suffix = रिक्त;
  bool use_dest_dir = false;
  bool deprecated_t_option = false;
  bool create_directory = false;
  bool dry_सततम् = false;
  पूर्णाङ्क स्थितिः = EXIT_SUCCESS;
  size_t x_गणनम्;
  size_t suffix_len;
  वर्ण *dest_name = रिक्त;

  initialize_main (&argc, &argv);
  समुच्चयः_क्रमादेशः_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (maybe_close_stdout);

  यावत् ((c = getopt_long (argc, argv, "dp:qtuV", longopts, रिक्त)) != -1)
    {
      विकल्प (c)
        {
        पक्ष 'd':
          create_directory = true;
          भङ्ग;
        पक्ष 'p':
          dest_dir_arg = optarg;
          use_dest_dir = true;
          भङ्ग;
        पक्ष 'q':
          suppress_file_err = true;
          भङ्ग;
        पक्ष 't':
          use_dest_dir = true;
          deprecated_t_option = true;
          भङ्ग;
        पक्ष 'u':
          dry_सततम् = true;
          भङ्ग;

        पक्ष SUFFIX_OPTION:
          suffix = optarg;
          भङ्ग;

        case_GETOPT_HELP_CHAR;

        पक्ष 'V': /* Undocumented alias, for compatibility with the original
                     mktemp program.  */
        case_GETOPT_संस्करणम्_CHAR (क्रमादेशः_NAME, AUTHORS);
        स्वभाव:
          usage (EXIT_FAILURE);
        }
    }

  पूर्णाङ्क n_तर्काः = argc - optind;
  यदि (2 <= n_तर्काः)
    {
      error (0, 0, _("too many templates"));
      usage (EXIT_FAILURE);
    }

  यदि (n_तर्काः == 0)
    {
      use_dest_dir = true;
      प्रतिरूपम् = (वर्ण *) default_template;
    }
  अन्यथा
    {
      प्रतिरूपम् = argv[optind];
    }

  यदि (suffix)
    {
      size_t len = वाक्यदैर्घ्यम् (प्रतिरूपम्);
      यदि (!len || प्रतिरूपम्[len - 1] != 'X')
        {
          error (EXIT_FAILURE, 0,
                 _("with --suffix, template %s must end in X"),
                 quote (प्रतिरूपम्));
        }
      suffix_len = वाक्यदैर्घ्यम् (suffix);
      dest_name = xcharalloc (len + suffix_len + 1);
      स्मृतिं_प्रतिलिख (dest_name, प्रतिरूपम्, len);
      स्मृतिं_प्रतिलिख (dest_name + len, suffix, suffix_len + 1);
      प्रतिरूपम् = dest_name;
      suffix = dest_name + len;
    }
  अन्यथा
    {
      प्रतिरूपम् = xstrdup (प्रतिरूपम्);
      suffix = strrchr (प्रतिरूपम्, 'X');
      यदि (!suffix)
        suffix = strchr (प्रतिरूपम्, '\0');
      अन्यथा
        suffix++;
      suffix_len = वाक्यदैर्घ्यम् (suffix);
    }

  /* At this point, template is malloc'd, and suffix points into template.  */
  यदि (suffix_len && चरमः_component (suffix) != suffix)
    {
      error (EXIT_FAILURE, 0,
             _("invalid suffix %s, contains directory separator"),
             quote (suffix));
    }
  x_गणनम् = गणनम्_consecutive_X_s (प्रतिरूपम्, suffix - प्रतिरूपम्);
  यदि (x_गणनम् < 3)
    {
      /* when suffix was appended to template only diagnose the template.  */
      यदि (प्रतिरूपम् == dest_name)
        प्रतिरूपम्[suffix - प्रतिरूपम्] = '\0';
      error (EXIT_FAILURE, 0, _("too few X's in template %s"),
             quote (प्रतिरूपम्));
    }

  यदि (use_dest_dir)
    {
      यदि (deprecated_t_option)
        {
          वर्ण *env = getenv ("TMPDIR");
          यदि (env && *env)
            dest_dir = env;
          अन्यथा यदि (dest_dir_arg && *dest_dir_arg)
            dest_dir = dest_dir_arg;
          अन्यथा
            dest_dir = "/tmp";

          यदि (चरमः_component (प्रतिरूपम्) != प्रतिरूपम्)
            error (EXIT_FAILURE, 0,
                   _("invalid template, %s, contains directory separator"),
                   quote (प्रतिरूपम्));
        }
      अन्यथा
        {
          यदि (dest_dir_arg && *dest_dir_arg)
            dest_dir = dest_dir_arg;
          अन्यथा
            {
              वर्ण *env = getenv ("TMPDIR");
              dest_dir = (env && *env ? env : "/tmp");
            }
          यदि (IS_ABSOLUTE_FILE_NAME (प्रतिरूपम्))
            error (EXIT_FAILURE, 0,
                   _("invalid template, %s; with --tmpdir,"
                     " it may not be absolute"),
                   quote (प्रतिरूपम्));
        }

      dest_name = file_name_concat (dest_dir, प्रतिरूपम्, रिक्त);
      मुक्त (प्रतिरूपम्);
      प्रतिरूपम् = dest_name;
      /* Note that suffix is now invalid.  */
    }

  /* Make a copy to be used in case of diagnostic, since failing
     mkstemp may leave the buffer in an undefined state.  */
  dest_name = xstrdup (प्रतिरूपम्);

  यदि (create_directory)
    {
      पूर्णाङ्क err = mkdtemp_len (dest_name, suffix_len, x_गणनम्, dry_सततम्);
      यदि (err != 0)
        {
          यदि (!suppress_file_err)
            error (0, errno, _("failed to create directory via template %s"),
                   quote (प्रतिरूपम्));
          स्थितिः = EXIT_FAILURE;
        }
    }
  अन्यथा
    {
      पूर्णाङ्क fd = mkstemp_len (dest_name, suffix_len, x_गणनम्, dry_सततम्);
      यदि (fd < 0 || (!dry_सततम् && close (fd) != 0))
        {
          यदि (!suppress_file_err)
            error (0, errno, _("failed to create file via template %s"),
                   quote (प्रतिरूपम्));
          स्थितिः = EXIT_FAILURE;
        }
    }

  यदि (स्थितिः == EXIT_SUCCESS)
    {
      पङ्क्तिं_लिख (dest_name);
      /* If we created a file, but then failed to output the file
         name, we should clean up the mess before failing.  */
      यदि (!dry_सततम् && ((stdout_closed = true), close_प्रवाहः (stdout) != 0))
        {
          पूर्णाङ्क saved_errno = errno;
          (create_directory ? rmdir : unlink) (dest_name);
          यदि (!suppress_file_err)
            error (0, saved_errno, _("write error"));
          स्थितिः = EXIT_FAILURE;
        }
    }

  main_exit (स्थितिः);
}
