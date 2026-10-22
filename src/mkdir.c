/* mkdir -- make directories
   Copyright (C) 1990-2026 Free Software Foundation, Inc.

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

/* David MacKenzie <djm@ai.mit.edu>  */

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <getopt.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <selinux/label.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "mkdir-p.h"
#अन्तर्भाव "modechange.h"
#अन्तर्भाव "prog-fprintf.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "savewd.h"
#अन्तर्भाव "selinux.h"
#अन्तर्भाव "smack.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#परिभाषय क्रमादेशः_NAME "mkdir"

#परिभाषय AUTHORS proper_name ("David MacKenzie")

स्थिर संरचना option ध्रुव longopts[] =
{
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {"mode", required_तर्कः, रिक्त, 'm'},
  {"parents", no_तर्कः, रिक्त, 'p'},
  {"verbose", no_तर्कः, रिक्त, 'v'},
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
      प्रारूप_लिख (_("Usage: %s [OPTION]... DIRECTORY...\n"), क्रमादेशः_name);
      fputs (_("\
Create the DIRECTORY(ies), यदि they कुरु not already exist.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -m, --बहुलकम्_2=बहुलकम्\n\
         समुच्चयः_2 file बहुलकम्_2 (as अन्तर् chmod), not a=rwx - umask\n\
"));
      oputs (_("\
  -p, --parents\n\
         no error यदि existing, make parent directories as needed,\n\
         with their file modes unaffected by any -m option\n\
"));
      oputs (_("\
  -v, --verbose\n\
         print a सन्देशः_2 प्रति each created directory\n\
"));
      oputs (_("\
  -Z\n\
         समुच्चयः_2 SELinux security context सम्बन्धि each created directory\n\
         गन्तव्यम् the स्वभाव type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or यदि CTX is specified then समुच्चयः_2 the\n\
         SELinux or SMACK security context गन्तव्यम् CTX\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (संस्करणम्_OPTION_DESCRIPTION);
      emit_ancillary_info (क्रमादेशः_NAME);
    }
  निर्गच्छ (स्थितिः);
}

/* Options passed to subsidiary functions.  */
संरचना mkdir_options
{
  /* Function to make an ancestor, or NULL if ancestors should not be
     made.  */
  पूर्णाङ्क (*make_ancestor_function) (वर्ण ध्रुव *, वर्ण ध्रुव *, शून्य *);

  /* Umask value for when making an ancestor.  */
  mode_t umask_ancestor;

  /* Umask value for when making the directory itself.  */
  mode_t umask_self;

  /* Mode for directory itself.  */
  mode_t बहुलकम्_2;

  /* File mode bits affected by MODE.  */
  mode_t बहुलकम्_द्वयङ्काः;

  /* Set the SELinux File Context.  */
  संरचना selabel_handle *समुच्चयः_security_context;

  /* If not null, format to use when reporting newly made directories.  */
  वर्ण ध्रुव *created_directory_format;
};

/* Report that directory DIR was made, if OPTIONS requests this.  */
स्थिर शून्य
announce_mkdir (वर्ण ध्रुव *dir, शून्य *options)
{
  संरचना mkdir_options ध्रुव *o = options;
  यदि (o->created_directory_format)
    prog_fprintf (stdout, o->created_directory_format, quoteaf (dir));
}

/* Make ancestor directory DIR, whose last component is COMPONENT,
   with options OPTIONS.  Assume the working directory is COMPONENT's
   parent.  Return 0 if successful and the resulting directory is
   readable, 1 if successful but the resulting directory is not
   readable, -1 (setting errno) otherwise.  */
स्थिर पूर्णाङ्क
make_ancestor (वर्ण ध्रुव *dir, वर्ण ध्रुव *component, शून्य *options)
{
  संरचना mkdir_options ध्रुव *o = options;

  यदि (o->समुच्चयः_security_context
      && defaultcon (o->समुच्चयः_security_context, component, S_IFDIR) < 0
      && ! ignorable_ctx_err (errno))
    error (0, errno, _("failed to set default creation context for %s"),
           quoteaf (dir));

  यदि (o->umask_ancestor != o->umask_self)
    umask (o->umask_ancestor);
  पूर्णाङ्क r = mkdir (component, S_IRWXUGO);
  यदि (o->umask_ancestor != o->umask_self)
    {
      पूर्णाङ्क mkdir_errno = errno;
      umask (o->umask_self);
      errno = mkdir_errno;
    }
  यदि (r == 0)
    {
      r = (o->umask_ancestor & S_IRUSR) != 0;
      announce_mkdir (dir, options);
    }
  प्रत्यागम r;
}

/* Process a command-line file name.  */
स्थिर पूर्णाङ्क
process_dir (वर्ण *dir, संरचना savewd *wd, शून्य *options)
{
  संरचना mkdir_options ध्रुव *o = options;

  /* If possible set context before DIR created.  */
  यदि (o->समुच्चयः_security_context)
    {
      यदि (! o->make_ancestor_function
          && defaultcon (o->समुच्चयः_security_context, dir, S_IFDIR) < 0
          && ! ignorable_ctx_err (errno))
        error (0, errno, _("failed to set default creation context for %s"),
               quoteaf (dir));
    }

  पूर्णाङ्क ret = (make_dir_parents (dir, wd, o->make_ancestor_function, options,
                               o->बहुलकम्_2, announce_mkdir,
                               o->बहुलकम्_द्वयङ्काः, (uid_t) -1, (gid_t) -1, true)
             ? EXIT_SUCCESS
             : EXIT_FAILURE);

  /* FIXME: Due to the current structure of make_dir_parents()
     we don't have the facility to call defaultcon() before the
     final component of DIR is created.  So for now, create the
     final component with the context from previous component
     and here we set the context for the final component. */
  यदि (ret == EXIT_SUCCESS && o->समुच्चयः_security_context
      && o->make_ancestor_function)
    {
      यदि (! restorecon (o->समुच्चयः_security_context, चरमः_component (dir), false)
          && ! ignorable_ctx_err (errno))
        error (0, errno, _("failed to restore context for %s"),
               quoteaf (dir));
    }

  प्रत्यागम ret;
}

पूर्णाङ्क
मुख्यम् (पूर्णाङ्क argc, वर्ण **argv)
{
  वर्ण ध्रुव *specified_बहुलकम् = रिक्त;
  पूर्णाङ्क optc;
  वर्ण ध्रुव *scontext = रिक्त;
  संरचना mkdir_options options;

  options.make_ancestor_function = रिक्त;
  options.बहुलकम्_2 = S_IRWXUGO;
  options.बहुलकम्_द्वयङ्काः = 0;
  options.created_directory_format = रिक्त;
  options.समुच्चयः_security_context = रिक्त;

  initialize_main (&argc, &argv);
  समुच्चयः_क्रमादेशः_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  यावत् ((optc = getopt_long (argc, argv, "pm:vZ", longopts, रिक्त)) != -1)
    {
      विकल्प (optc)
        {
        पक्ष 'p':
          options.make_ancestor_function = make_ancestor;
          भङ्ग;
        पक्ष 'm':
          specified_बहुलकम् = optarg;
          भङ्ग;
        पक्ष 'v': /* --verbose  */
          options.created_directory_format = _("created directory %s");
          भङ्ग;
        पक्ष 'Z':
          यदि (is_smack_enabled ())
            {
              /* We don't yet support -Z to restore context with SMACK.  */
              scontext = optarg;
            }
          अन्यथा यदि (is_selinux_enabled () > 0)
            {
              यदि (optarg)
                scontext = optarg;
              अन्यथा
                {
                  options.समुच्चयः_security_context = selabel_open (SELABEL_CTX_FILE,
                                                               रिक्त, 0);
                  यदि (! options.समुच्चयः_security_context)
                    error (0, errno, _("warning: ignoring --context"));
                }
            }
          अन्यथा यदि (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux/SMACK-enabled kernel"));
            }
          भङ्ग;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_संस्करणम्_CHAR (क्रमादेशः_NAME, AUTHORS);
        स्वभाव:
          usage (EXIT_FAILURE);
        }
    }

  यदि (optind == argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  /* FIXME: This assumes mkdir() is done in the same process.
     If that's not always the case we would need to call this
     like we do when options.set_security_context.  */
  यदि (scontext)
    {
      पूर्णाङ्क ret = 0;
      यदि (is_smack_enabled ())
        ret = smack_समुच्चयः_label_for_self (scontext);
      अन्यथा
        ret = setfscreatecon (scontext);

      यदि (ret < 0)
        error (EXIT_FAILURE, errno,
               _("failed to set default file creation context to %s"),
               quote (scontext));
    }


  यदि (options.make_ancestor_function || specified_बहुलकम्)
    {
      mode_t umask_मानम् = umask (0);
      options.umask_ancestor = umask_मानम् & ~(S_IWUSR | S_IXUSR);

      यदि (specified_बहुलकम्)
        {
          संरचना बहुलकम्_change *change = बहुलकम्_compile (specified_बहुलकम्);
          यदि (!change)
            error (EXIT_FAILURE, 0, _("invalid mode %s"),
                   quote (specified_बहुलकम्));
          options.बहुलकम्_2 = बहुलकम्_adjust (S_IRWXUGO, true, umask_मानम्, change,
                                      &options.बहुलकम्_द्वयङ्काः);
          options.umask_self = umask_मानम् & ~options.बहुलकम्_2;
          मुक्त (change);
        }
      अन्यथा
        {
          options.बहुलकम्_2 = S_IRWXUGO;
          options.umask_self = umask_मानम्;
        }

      umask (options.umask_self);
    }

  प्रत्यागम savewd_process_files (argc - optind, argv + optind,
                               process_dir, &options);
}
