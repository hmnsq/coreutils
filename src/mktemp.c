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
#define PROGRAM_NAME "mktemp"

#define AUTHORS \
  proper_name ("Jim Meyering"), \
  proper_name ("Eric Blake")

स्थिर वर्ण अचर *default_template = "tmp.XXXXXXXXXX";

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
गणना
{
  SUFFIX_OPTION = अक्षर_उच्च + 1,
};

स्थिर संरचना option अचर longopts[] =
{
  {"directory", no_argument, रिक्त, 'd'},
  {"quiet", no_argument, रिक्त, 'q'},
  {"dry-run", no_argument, रिक्त, 'u'},
  {"suffix", required_argument, रिक्त, SUFFIX_OPTION},
  {"tmpdir", optional_argument, रिक्त, 'p'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

शून्य
usage (पूर्णाङ्क status)
{
  यदि (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("Usage: %s [OPTION]... [TEMPLATE]\n"), program_name);
      ख_माला_दो (_("\
Create a temporary file or directory, safely, and print its name.\n\
TEMPLATE must contain at least 3 consecutive 'X's in last component.\n\
If TEMPLATE is not specified, use tmp.XXXXXXXXXX, and --tmpdir is implied.\n\
"), मानक_निकास);
      ख_माला_दो (_("\
Files are created u+rw, and directories u+rwx, minus umask restrictions.\n\
"), मानक_निकास);
      ख_माला_दो ("\n", मानक_निकास);
      oputs (_("\
  -d, --directory\n\
         create a directory, not a file\n\
"));
      oputs (_("\
  -u, --dry-run\n\
         गर not create anything; merely print a name (unsafe)\n\
"));
      oputs (_("\
  -q, --quiet\n\
         suppress diagnostics about file/dir-creation failure\n\
"));
      oputs (_("\
      --suffix=SUFF\n\
         append SUFF to TEMPLATE; SUFF must not contain a slash.\n\
         This option is implied यदि TEMPLATE does not end in X\n\
"));
      oputs (_("\
  -p सूची, --tmpdir[=सूची]\n\
         interpret TEMPLATE relative to सूची;\n\
         यदि सूची is not specified, use $TMPDIR यदि set, अन्यथा /tmp.\n\
         With this option, TEMPLATE must not be an absolute name;\n\
         unlike with -t, TEMPLATE may contain slashes,\n\
         but mktemp creates only the final component\n\
"));
      oputs (_("\
  -t\n\
         interpret TEMPLATE as a single file name component,\n\
         relative to a directory: $TMPDIR, यदि set;\n\
         अन्यथा the directory specified via -p; अन्यथा /tmp [deprecated]\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }

  निकास (status);
}

स्थिर माप_प्रकार
गन्ती_consecutive_X_s (वर्ण अचर *s, माप_प्रकार len)
{
  माप_प्रकार n = 0;
  लागि ( ; len && s[len - 1] == 'X'; len--)
    ++n;
  फर्काऊ n;
}

स्थिर पूर्णाङ्क
mkstemp_len (वर्ण *tmpl, माप_प्रकार suff_len, माप_प्रकार x_len, bool dry_run)
{
  फर्काऊ gen_tempname_len (tmpl, suff_len, 0, dry_run ? GT_NOCREATE : GT_FILE,
                           x_len);
}

स्थिर पूर्णाङ्क
mkdtemp_len (वर्ण *tmpl, माप_प्रकार suff_len, माप_प्रकार x_len, bool dry_run)
{
  फर्काऊ gen_tempname_len (tmpl, suff_len, 0, dry_run ? GT_NOCREATE : GT_DIR,
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
  अन्यथा यदि (close_stream (मानक_त्रुटि) != 0)
    _exit (निकास_त्रुटि);
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  वर्ण अचर *dest_dir;
  वर्ण अचर *dest_dir_arg = रिक्त;
  bool suppress_file_err = false;
  पूर्णाङ्क c;
  वर्ण *ढाँचा;
  वर्ण *suffix = रिक्त;
  bool use_dest_dir = false;
  bool deprecated_t_option = false;
  bool create_directory = false;
  bool dry_run = false;
  पूर्णाङ्क status = निकास_सफल;
  माप_प्रकार x_गन्ती;
  माप_प्रकार suffix_len;
  वर्ण *dest_name = रिक्त;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (maybe_close_stdout);

  जबसम्म ((c = getopt_long (argc, argv, "dp:qtuV", longopts, रिक्त)) != -1)
    {
      विकल्प (c)
        {
        पक्ष 'd':
          create_directory = true;
          रोक;
        पक्ष 'p':
          dest_dir_arg = optarg;
          use_dest_dir = true;
          रोक;
        पक्ष 'q':
          suppress_file_err = true;
          रोक;
        पक्ष 't':
          use_dest_dir = true;
          deprecated_t_option = true;
          रोक;
        पक्ष 'u':
          dry_run = true;
          रोक;

        पक्ष SUFFIX_OPTION:
          suffix = optarg;
          रोक;

        case_GETOPT_HELP_CHAR;

        पक्ष 'V': /* Undocumented alias, for compatibility with the original
                     mktemp program.  */
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        स्वभाव:
          usage (निकास_त्रुटि);
        }
    }

  पूर्णाङ्क n_args = argc - optind;
  यदि (2 <= n_args)
    {
      error (0, 0, _("too many templates"));
      usage (निकास_त्रुटि);
    }

  यदि (n_args == 0)
    {
      use_dest_dir = true;
      ढाँचा = (वर्ण *) default_template;
    }
  अन्यथा
    {
      ढाँचा = argv[optind];
    }

  यदि (suffix)
    {
      माप_प्रकार len = म_माप (ढाँचा);
      यदि (!len || ढाँचा[len - 1] != 'X')
        {
          error (निकास_त्रुटि, 0,
                 _("with --suffix, template %s must end in X"),
                 quote (ढाँचा));
        }
      suffix_len = म_माप (suffix);
      dest_name = xcharalloc (len + suffix_len + 1);
      स_नकल (dest_name, ढाँचा, len);
      स_नकल (dest_name + len, suffix, suffix_len + 1);
      ढाँचा = dest_name;
      suffix = dest_name + len;
    }
  अन्यथा
    {
      ढाँचा = xstrdup (ढाँचा);
      suffix = म_खोजप (ढाँचा, 'X');
      यदि (!suffix)
        suffix = म_अक्षर (ढाँचा, '\0');
      अन्यथा
        suffix++;
      suffix_len = म_माप (suffix);
    }

  /* At this point, template is malloc'd, and suffix points into template.  */
  यदि (suffix_len && last_component (suffix) != suffix)
    {
      error (निकास_त्रुटि, 0,
             _("invalid suffix %s, contains directory separator"),
             quote (suffix));
    }
  x_गन्ती = गन्ती_consecutive_X_s (ढाँचा, suffix - ढाँचा);
  यदि (x_गन्ती < 3)
    {
      /* when suffix was appended to template only diagnose the template.  */
      यदि (ढाँचा == dest_name)
        ढाँचा[suffix - ढाँचा] = '\0';
      error (निकास_त्रुटि, 0, _("too few X's in template %s"),
             quote (ढाँचा));
    }

  यदि (use_dest_dir)
    {
      यदि (deprecated_t_option)
        {
          वर्ण *env = दो_पर्या ("TMPDIR");
          यदि (env && *env)
            dest_dir = env;
          अन्यथा यदि (dest_dir_arg && *dest_dir_arg)
            dest_dir = dest_dir_arg;
          अन्यथा
            dest_dir = "/tmp";

          यदि (last_component (ढाँचा) != ढाँचा)
            error (निकास_त्रुटि, 0,
                   _("invalid template, %s, contains directory separator"),
                   quote (ढाँचा));
        }
      अन्यथा
        {
          यदि (dest_dir_arg && *dest_dir_arg)
            dest_dir = dest_dir_arg;
          अन्यथा
            {
              वर्ण *env = दो_पर्या ("TMPDIR");
              dest_dir = (env && *env ? env : "/tmp");
            }
          यदि (IS_ABSOLUTE_FILE_NAME (ढाँचा))
            error (निकास_त्रुटि, 0,
                   _("invalid template, %s; with --tmpdir,"
                     " it may not be absolute"),
                   quote (ढाँचा));
        }

      dest_name = file_name_concat (dest_dir, ढाँचा, रिक्त);
      मुक्त (ढाँचा);
      ढाँचा = dest_name;
      /* Note that suffix is now invalid.  */
    }

  /* Make a copy to be used in case of diagnostic, since failing
     mkstemp may leave the buffer in an undefined state.  */
  dest_name = xstrdup (ढाँचा);

  यदि (create_directory)
    {
      पूर्णाङ्क err = mkdtemp_len (dest_name, suffix_len, x_गन्ती, dry_run);
      यदि (err != 0)
        {
          यदि (!suppress_file_err)
            error (0, त्रुटि_सं, _("failed to create directory via template %s"),
                   quote (ढाँचा));
          status = निकास_त्रुटि;
        }
    }
  अन्यथा
    {
      पूर्णाङ्क fd = mkstemp_len (dest_name, suffix_len, x_गन्ती, dry_run);
      यदि (fd < 0 || (!dry_run && बंद (fd) != 0))
        {
          यदि (!suppress_file_err)
            error (0, त्रुटि_सं, _("failed to create file via template %s"),
                   quote (ढाँचा));
          status = निकास_त्रुटि;
        }
    }

  यदि (status == निकास_सफल)
    {
      माला_दो (dest_name);
      /* If we created a file, but then failed to output the file
         name, we should clean up the mess before failing.  */
      यदि (!dry_run && ((stdout_closed = true), close_stream (मानक_निकास) != 0))
        {
          पूर्णाङ्क saved_errno = त्रुटि_सं;
          (create_directory ? सूची_हटाओ : unlink) (dest_name);
          यदि (!suppress_file_err)
            error (0, saved_errno, _("write error"));
          status = निकास_त्रुटि;
        }
    }

  main_exit (status);
}
