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

#شامل <config.h>
#شامل <stdio.h>
#شامل <getopt.h>
#شامل <sys/types.h>
#شامل <selinux/label.h>

#شامل "system.h"
#شامل "mkdir-p.h"
#شامل "modechange.h"
#شامل "prog-fprintf.h"
#شامل "quote.h"
#شامل "savewd.h"
#شامل "selinux.h"
#شامل "smack.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "mkdir"

#define AUTHORS proper_name ("David MacKenzie")

ایستا ساختار option ثابت longopts[] =
{
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {"mode", required_argument, پوچ, 'm'},
  {"parents", no_argument, پوچ, 'p'},
  {"verbose", no_argument, پوچ, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {پوچ, 0, پوچ, 0}
};

تهی
usage (صحیح status)
{
  اگر (status != EXIT_SUCCESS)
    emit_try_help ();
  وگرنه
    {
      printf (_("Usage: %s [OPTION]... DIRECTORY...\n"), program_name);
      fputs (_("\
Create the DIRECTORY(ies), اگر they انجامبده not already exist.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -m, --mode=MODE\n\
         set file mode (as in chmod), not a=rwx - umask\n\
"));
      oputs (_("\
  -p, --parents\n\
         no error اگر existing, make parent directories as needed,\n\
         with their file modes unaffected by any -m option\n\
"));
      oputs (_("\
  -v, --verbose\n\
         print a message برای each created directory\n\
"));
      oputs (_("\
  -Z\n\
         set SELinux security context of each created directory\n\
         to the پیشفرض type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or اگر CTX is specified then set the\n\
         SELinux or SMACK security context to CTX\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Options passed to subsidiary functions.  */
ساختار mkdir_options
{
  /* Function to make an ancestor, or NULL if ancestors should not be
     made.  */
  صحیح (*make_ancestor_function) (نویسه ثابت *, نویسه ثابت *, تهی *);

  /* Umask value for when making an ancestor.  */
  mode_t umask_ancestor;

  /* Umask value for when making the directory itself.  */
  mode_t umask_self;

  /* Mode for directory itself.  */
  mode_t mode;

  /* File mode bits affected by MODE.  */
  mode_t mode_bits;

  /* Set the SELinux File Context.  */
  ساختار selabel_handle *set_security_context;

  /* If not null, format to use when reporting newly made directories.  */
  نویسه ثابت *created_directory_format;
};

/* Report that directory DIR was made, if OPTIONS requests this.  */
ایستا تهی
announce_mkdir (نویسه ثابت *dir, تهی *options)
{
  ساختار mkdir_options ثابت *o = options;
  اگر (o->created_directory_format)
    prog_fprintf (stdout, o->created_directory_format, quoteaf (dir));
}

/* Make ancestor directory DIR, whose last component is COMPONENT,
   with options OPTIONS.  Assume the working directory is COMPONENT's
   parent.  Return 0 if successful and the resulting directory is
   readable, 1 if successful but the resulting directory is not
   readable, -1 (setting errno) otherwise.  */
ایستا صحیح
make_ancestor (نویسه ثابت *dir, نویسه ثابت *component, تهی *options)
{
  ساختار mkdir_options ثابت *o = options;

  اگر (o->set_security_context
      && defaultcon (o->set_security_context, component, S_IFDIR) < 0
      && ! ignorable_ctx_err (errno))
    error (0, errno, _("failed to set default creation context for %s"),
           quoteaf (dir));

  اگر (o->umask_ancestor != o->umask_self)
    umask (o->umask_ancestor);
  صحیح r = mkdir (component, S_IRWXUGO);
  اگر (o->umask_ancestor != o->umask_self)
    {
      صحیح mkdir_errno = errno;
      umask (o->umask_self);
      errno = mkdir_errno;
    }
  اگر (r == 0)
    {
      r = (o->umask_ancestor & S_IRUSR) != 0;
      announce_mkdir (dir, options);
    }
  بازگشت r;
}

/* Process a command-line file name.  */
ایستا صحیح
process_dir (نویسه *dir, ساختار savewd *wd, تهی *options)
{
  ساختار mkdir_options ثابت *o = options;

  /* If possible set context before DIR created.  */
  اگر (o->set_security_context)
    {
      اگر (! o->make_ancestor_function
          && defaultcon (o->set_security_context, dir, S_IFDIR) < 0
          && ! ignorable_ctx_err (errno))
        error (0, errno, _("failed to set default creation context for %s"),
               quoteaf (dir));
    }

  صحیح ret = (make_dir_parents (dir, wd, o->make_ancestor_function, options,
                               o->mode, announce_mkdir,
                               o->mode_bits, (uid_t) -1, (gid_t) -1, true)
             ? EXIT_SUCCESS
             : EXIT_FAILURE);

  /* FIXME: Due to the current structure of make_dir_parents()
     we don't have the facility to call defaultcon() before the
     final component of DIR is created.  So for now, create the
     final component with the context from previous component
     and here we set the context for the final component. */
  اگر (ret == EXIT_SUCCESS && o->set_security_context
      && o->make_ancestor_function)
    {
      اگر (! restorecon (o->set_security_context, last_component (dir), false)
          && ! ignorable_ctx_err (errno))
        error (0, errno, _("failed to restore context for %s"),
               quoteaf (dir));
    }

  بازگشت ret;
}

صحیح
اصلی (صحیح argc, نویسه **argv)
{
  نویسه ثابت *specified_mode = پوچ;
  صحیح optc;
  نویسه ثابت *scontext = پوچ;
  ساختار mkdir_options options;

  options.make_ancestor_function = پوچ;
  options.mode = S_IRWXUGO;
  options.mode_bits = 0;
  options.created_directory_format = پوچ;
  options.set_security_context = پوچ;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  وقتی ((optc = getopt_long (argc, argv, "pm:vZ", longopts, پوچ)) != -1)
    {
      انتخاب (optc)
        {
        حالت 'p':
          options.make_ancestor_function = make_ancestor;
          بشکن;
        حالت 'm':
          specified_mode = optarg;
          بشکن;
        حالت 'v': /* --verbose  */
          options.created_directory_format = _("created directory %s");
          بشکن;
        حالت 'Z':
          اگر (is_smack_enabled ())
            {
              /* We don't yet support -Z to restore context with SMACK.  */
              scontext = optarg;
            }
          وگرنه اگر (is_selinux_enabled () > 0)
            {
              اگر (optarg)
                scontext = optarg;
              وگرنه
                {
                  options.set_security_context = selabel_open (SELABEL_CTX_FILE,
                                                               پوچ, 0);
                  اگر (! options.set_security_context)
                    error (0, errno, _("warning: ignoring --context"));
                }
            }
          وگرنه اگر (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux/SMACK-enabled kernel"));
            }
          بشکن;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        پیشفرض:
          usage (EXIT_FAILURE);
        }
    }

  اگر (optind == argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  /* FIXME: This assumes mkdir() is done in the same process.
     If that's not always the case we would need to call this
     like we do when options.set_security_context.  */
  اگر (scontext)
    {
      صحیح ret = 0;
      اگر (is_smack_enabled ())
        ret = smack_set_label_for_self (scontext);
      وگرنه
        ret = setfscreatecon (scontext);

      اگر (ret < 0)
        error (EXIT_FAILURE, errno,
               _("failed to set default file creation context to %s"),
               quote (scontext));
    }


  اگر (options.make_ancestor_function || specified_mode)
    {
      mode_t umask_مقدار = umask (0);
      options.umask_ancestor = umask_مقدار & ~(S_IWUSR | S_IXUSR);

      اگر (specified_mode)
        {
          ساختار mode_change *change = mode_compile (specified_mode);
          اگر (!change)
            error (EXIT_FAILURE, 0, _("invalid mode %s"),
                   quote (specified_mode));
          options.mode = mode_adjust (S_IRWXUGO, true, umask_مقدار, change,
                                      &options.mode_bits);
          options.umask_self = umask_مقدار & ~options.mode;
          آزاد (change);
        }
      وگرنه
        {
          options.mode = S_IRWXUGO;
          options.umask_self = umask_مقدار;
        }

      umask (options.umask_self);
    }

  بازگشت savewd_process_files (argc - optind, argv + optind,
                               process_dir, &options);
}
