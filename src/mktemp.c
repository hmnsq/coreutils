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

#समावेश <config.h>
#समावेश <sys/types.h>
#समावेश <getopt.h>

#समावेश "system.h"

#समावेश "close-stream.h"
#समावेश "filenamecat.h"
#समावेश "quote.h"
#समावेश "tempname.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "mktemp"

#घोषणा AUTHORS \
  proper_name ("Jim Meyering"), \
  proper_name ("Eric Blake")

अटल अक्षर स्थिर *default_template = "tmp.XXXXXXXXXX";

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
क्रमागत
{
  SUFFIX_OPTION = अक्षर_उच्च + 1,
};

अटल काष्ठा option स्थिर longopts[] =
{
  {"directory", no_argument, शून्य, 'd'},
  {"quiet", no_argument, शून्य, 'q'},
  {"dry-run", no_argument, शून्य, 'u'},
  {"suffix", required_argument, शून्य, SUFFIX_OPTION},
  {"tmpdir", optional_argument, शून्य, 'p'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {शून्य, 0, शून्य, 0}
};

व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
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
         करो not create anything; merely print a name (unsafe)\n\
"));
      oputs (_("\
  -q, --quiet\n\
         suppress diagnostics about file/dir-creation failure\n\
"));
      oputs (_("\
      --suffix=SUFF\n\
         append SUFF to TEMPLATE; SUFF must not contain a slash.\n\
         This option is implied अगर TEMPLATE does not end in X\n\
"));
      oputs (_("\
  -p सूची, --tmpdir[=सूची]\n\
         interpret TEMPLATE relative to सूची;\n\
         अगर सूची is not specified, use $TMPDIR अगर set, अन्यथा /tmp.\n\
         With this option, TEMPLATE must not be an absolute name;\n\
         unlike with -t, TEMPLATE may contain slashes,\n\
         but mktemp creates only the final component\n\
"));
      oputs (_("\
  -t\n\
         interpret TEMPLATE as a single file name component,\n\
         relative to a directory: $TMPDIR, अगर set;\n\
         अन्यथा the directory specified via -p; अन्यथा /tmp [deprecated]\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }

  निकास (status);
}

अटल माप_प्रकार
गिनती_consecutive_X_s (अक्षर स्थिर *s, माप_प्रकार len)
{
  माप_प्रकार n = 0;
  क्रम ( ; len && s[len - 1] == 'X'; len--)
    ++n;
  वापस n;
}

अटल पूर्णांक
mkstemp_len (अक्षर *tmpl, माप_प्रकार suff_len, माप_प्रकार x_len, bool dry_run)
{
  वापस gen_tempname_len (tmpl, suff_len, 0, dry_run ? GT_NOCREATE : GT_FILE,
                           x_len);
}

अटल पूर्णांक
mkdtemp_len (अक्षर *tmpl, माप_प्रकार suff_len, माप_प्रकार x_len, bool dry_run)
{
  वापस gen_tempname_len (tmpl, suff_len, 0, dry_run ? GT_NOCREATE : GT_DIR,
                           x_len);
}

/* True if we have already closed standard output.  */
अटल bool stdout_closed;

/* Avoid closing stdout twice.  Since we conditionally call
   close_stream (stdout) in order to decide whether to clean up a
   temporary file, the exit hook needs to know whether to do all of
   close_stdout or just the stderr half.  */
अटल व्योम
maybe_close_stdout (व्योम)
{
  अगर (!stdout_closed)
    close_stdout ();
  अन्यथा अगर (close_stream (मानक_त्रुटि) != 0)
    _निकास (निकास_त्रुटि);
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  अक्षर स्थिर *dest_dir;
  अक्षर स्थिर *dest_dir_arg = शून्य;
  bool suppress_file_err = false;
  पूर्णांक c;
  अक्षर *ढाँचा;
  अक्षर *suffix = शून्य;
  bool use_dest_dir = false;
  bool deprecated_t_option = false;
  bool create_directory = false;
  bool dry_run = false;
  पूर्णांक status = निकास_सफल;
  माप_प्रकार x_गिनती;
  माप_प्रकार suffix_len;
  अक्षर *dest_name = शून्य;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (maybe_close_stdout);

  जबतक ((c = getopt_long (argc, argv, "dp:qtuV", longopts, शून्य)) != -1)
    {
      चयन (c)
        {
        हाल 'd':
          create_directory = true;
          अवरोध;
        हाल 'p':
          dest_dir_arg = optarg;
          use_dest_dir = true;
          अवरोध;
        हाल 'q':
          suppress_file_err = true;
          अवरोध;
        हाल 't':
          use_dest_dir = true;
          deprecated_t_option = true;
          अवरोध;
        हाल 'u':
          dry_run = true;
          अवरोध;

        हाल SUFFIX_OPTION:
          suffix = optarg;
          अवरोध;

        case_GETOPT_HELP_CHAR;

        हाल 'V': /* Undocumented alias, for compatibility with the original
                     mktemp program.  */
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        शेष:
          usage (निकास_त्रुटि);
        }
    }

  पूर्णांक n_args = argc - optind;
  अगर (2 <= n_args)
    {
      error (0, 0, _("too many templates"));
      usage (निकास_त्रुटि);
    }

  अगर (n_args == 0)
    {
      use_dest_dir = true;
      ढाँचा = (अक्षर *) default_template;
    }
  अन्यथा
    {
      ढाँचा = argv[optind];
    }

  अगर (suffix)
    {
      माप_प्रकार len = म_माप (ढाँचा);
      अगर (!len || ढाँचा[len - 1] != 'X')
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
      अगर (!suffix)
        suffix = म_अक्षर (ढाँचा, '\0');
      अन्यथा
        suffix++;
      suffix_len = म_माप (suffix);
    }

  /* At this point, template is malloc'd, and suffix points into template.  */
  अगर (suffix_len && last_component (suffix) != suffix)
    {
      error (निकास_त्रुटि, 0,
             _("invalid suffix %s, contains directory separator"),
             quote (suffix));
    }
  x_गिनती = गिनती_consecutive_X_s (ढाँचा, suffix - ढाँचा);
  अगर (x_गिनती < 3)
    {
      /* when suffix was appended to template only diagnose the template.  */
      अगर (ढाँचा == dest_name)
        ढाँचा[suffix - ढाँचा] = '\0';
      error (निकास_त्रुटि, 0, _("too few X's in template %s"),
             quote (ढाँचा));
    }

  अगर (use_dest_dir)
    {
      अगर (deprecated_t_option)
        {
          अक्षर *env = दो_पर्या ("TMPDIR");
          अगर (env && *env)
            dest_dir = env;
          अन्यथा अगर (dest_dir_arg && *dest_dir_arg)
            dest_dir = dest_dir_arg;
          अन्यथा
            dest_dir = "/tmp";

          अगर (last_component (ढाँचा) != ढाँचा)
            error (निकास_त्रुटि, 0,
                   _("invalid template, %s, contains directory separator"),
                   quote (ढाँचा));
        }
      अन्यथा
        {
          अगर (dest_dir_arg && *dest_dir_arg)
            dest_dir = dest_dir_arg;
          अन्यथा
            {
              अक्षर *env = दो_पर्या ("TMPDIR");
              dest_dir = (env && *env ? env : "/tmp");
            }
          अगर (IS_ABSOLUTE_FILE_NAME (ढाँचा))
            error (निकास_त्रुटि, 0,
                   _("invalid template, %s; with --tmpdir,"
                     " it may not be absolute"),
                   quote (ढाँचा));
        }

      dest_name = file_name_concat (dest_dir, ढाँचा, शून्य);
      मुक्त (ढाँचा);
      ढाँचा = dest_name;
      /* Note that suffix is now invalid.  */
    }

  /* Make a copy to be used in case of diagnostic, since failing
     mkstemp may leave the buffer in an undefined state.  */
  dest_name = xstrdup (ढाँचा);

  अगर (create_directory)
    {
      पूर्णांक err = mkdtemp_len (dest_name, suffix_len, x_गिनती, dry_run);
      अगर (err != 0)
        {
          अगर (!suppress_file_err)
            error (0, त्रुटि_सं, _("failed to create directory via template %s"),
                   quote (ढाँचा));
          status = निकास_त्रुटि;
        }
    }
  अन्यथा
    {
      पूर्णांक fd = mkstemp_len (dest_name, suffix_len, x_गिनती, dry_run);
      अगर (fd < 0 || (!dry_run && बंद (fd) != 0))
        {
          अगर (!suppress_file_err)
            error (0, त्रुटि_सं, _("failed to create file via template %s"),
                   quote (ढाँचा));
          status = निकास_त्रुटि;
        }
    }

  अगर (status == निकास_सफल)
    {
      माला_दो (dest_name);
      /* If we created a file, but then failed to output the file
         name, we should clean up the mess before failing.  */
      अगर (!dry_run && ((stdout_closed = true), close_stream (मानक_निकास) != 0))
        {
          पूर्णांक saved_errno = त्रुटि_सं;
          (create_directory ? सूची_हटाओ : unlink) (dest_name);
          अगर (!suppress_file_err)
            error (0, saved_errno, _("write error"));
          status = निकास_त्रुटि;
        }
    }

  main_exit (status);
}
