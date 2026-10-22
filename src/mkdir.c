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
#تعریف_کرو پروگرام_NAME "mkdir"

#تعریف_کرو AUTHORS proper_name ("David MacKenzie")

ساکن ساخت option ثابت longopts[] =
{
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {"mode", required_دلیل, تہی, 'm'},
  {"parents", no_دلیل, تہی, 'p'},
  {"verbose", no_دلیل, تہی, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0}
};

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("Usage: %s [OPTION]... DIRECTORY...\n"), پروگرام_name);
      fputs (_("\
Create the DIRECTORY(ies), اگر they کرو not already exist.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -m, --منوال_2=منوال\n\
         سیٹ_2 file منوال_2 (as اندر chmod), not a=rwx - umask\n\
"));
      oputs (_("\
  -p, --parents\n\
         no error اگر existing, make والد directories as needed,\n\
         with their file modes unaffected by any -m option\n\
"));
      oputs (_("\
  -v, --verbose\n\
         print a پیغام_2 برائے each created directory\n\
"));
      oputs (_("\
  -Z\n\
         سیٹ_2 SELinux security context کا each created directory\n\
         مقصد the طےشدہ type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or اگر CTX is specified then سیٹ_2 the\n\
         SELinux or SMACK security context مقصد CTX\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

/* Options passed to subsidiary functions.  */
ساخت mkdir_options
{
  /* Function to make an ancestor, or NULL if ancestors should not be
     made.  */
  صحیح (*make_ancestor_function) (حرف ثابت *, حرف ثابت *, خالی *);

  /* Umask value for when making an ancestor.  */
  mode_t umask_ancestor;

  /* Umask value for when making the directory itself.  */
  mode_t umask_self;

  /* Mode for directory itself.  */
  mode_t منوال_2;

  /* File mode bits affected by MODE.  */
  mode_t منوال_بٹ;

  /* Set the SELinux File Context.  */
  ساخت selabel_handle *سیٹ_security_context;

  /* If not null, format to use when reporting newly made directories.  */
  حرف ثابت *created_directory_format;
};

/* Report that directory DIR was made, if OPTIONS requests this.  */
ساکن خالی
announce_mkdir (حرف ثابت *dir, خالی *options)
{
  ساخت mkdir_options ثابت *o = options;
  اگر (o->created_directory_format)
    prog_fprintf (stdout, o->created_directory_format, quoteaf (dir));
}

/* Make ancestor directory DIR, whose last component is COMPONENT,
   with options OPTIONS.  Assume the working directory is COMPONENT's
   parent.  Return 0 if successful and the resulting directory is
   readable, 1 if successful but the resulting directory is not
   readable, -1 (setting errno) otherwise.  */
ساکن صحیح
make_ancestor (حرف ثابت *dir, حرف ثابت *component, خالی *options)
{
  ساخت mkdir_options ثابت *o = options;

  اگر (o->سیٹ_security_context
      && defaultcon (o->سیٹ_security_context, component, S_IFDIR) < 0
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
  واپس r;
}

/* Process a command-line file name.  */
ساکن صحیح
process_dir (حرف *dir, ساخت savewd *wd, خالی *options)
{
  ساخت mkdir_options ثابت *o = options;

  /* If possible set context before DIR created.  */
  اگر (o->سیٹ_security_context)
    {
      اگر (! o->make_ancestor_function
          && defaultcon (o->سیٹ_security_context, dir, S_IFDIR) < 0
          && ! ignorable_ctx_err (errno))
        error (0, errno, _("failed to set default creation context for %s"),
               quoteaf (dir));
    }

  صحیح ret = (make_dir_parents (dir, wd, o->make_ancestor_function, options,
                               o->منوال_2, announce_mkdir,
                               o->منوال_بٹ, (uid_t) -1, (gid_t) -1, true)
             ? EXIT_SUCCESS
             : EXIT_FAILURE);

  /* FIXME: Due to the current structure of make_dir_parents()
     we don't have the facility to call defaultcon() before the
     final component of DIR is created.  So for now, create the
     final component with the context from previous component
     and here we set the context for the final component. */
  اگر (ret == EXIT_SUCCESS && o->سیٹ_security_context
      && o->make_ancestor_function)
    {
      اگر (! restorecon (o->سیٹ_security_context, آخری_component (dir), false)
          && ! ignorable_ctx_err (errno))
        error (0, errno, _("failed to restore context for %s"),
               quoteaf (dir));
    }

  واپس ret;
}

صحیح
مین (صحیح argc, حرف **argv)
{
  حرف ثابت *specified_منوال = تہی;
  صحیح optc;
  حرف ثابت *scontext = تہی;
  ساخت mkdir_options options;

  options.make_ancestor_function = تہی;
  options.منوال_2 = S_IRWXUGO;
  options.منوال_بٹ = 0;
  options.created_directory_format = تہی;
  options.سیٹ_security_context = تہی;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  جب ((optc = getopt_long (argc, argv, "pm:vZ", longopts, تہی)) != -1)
    {
      انتخاب (optc)
        {
        حالت 'p':
          options.make_ancestor_function = make_ancestor;
          توڑو;
        حالت 'm':
          specified_منوال = optarg;
          توڑو;
        حالت 'v': /* --verbose  */
          options.created_directory_format = _("created directory %s");
          توڑو;
        حالت 'Z':
          اگر (is_smack_enabled ())
            {
              /* We don't yet support -Z to restore context with SMACK.  */
              scontext = optarg;
            }
          ورنہ اگر (is_selinux_enabled () > 0)
            {
              اگر (optarg)
                scontext = optarg;
              ورنہ
                {
                  options.سیٹ_security_context = selabel_open (SELABEL_CTX_FILE,
                                                               تہی, 0);
                  اگر (! options.سیٹ_security_context)
                    error (0, errno, _("warning: ignoring --context"));
                }
            }
          ورنہ اگر (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux/SMACK-enabled kernel"));
            }
          توڑو;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);
        طےشدہ:
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
        ret = smack_سیٹ_label_for_self (scontext);
      ورنہ
        ret = setfscreatecon (scontext);

      اگر (ret < 0)
        error (EXIT_FAILURE, errno,
               _("failed to set default file creation context to %s"),
               quote (scontext));
    }


  اگر (options.make_ancestor_function || specified_منوال)
    {
      mode_t umask_قدر = umask (0);
      options.umask_ancestor = umask_قدر & ~(S_IWUSR | S_IXUSR);

      اگر (specified_منوال)
        {
          ساخت منوال_change *change = منوال_compile (specified_منوال);
          اگر (!change)
            error (EXIT_FAILURE, 0, _("invalid mode %s"),
                   quote (specified_منوال));
          options.منوال_2 = منوال_adjust (S_IRWXUGO, true, umask_قدر, change,
                                      &options.منوال_بٹ);
          options.umask_self = umask_قدر & ~options.منوال_2;
          آزاد (change);
        }
      ورنہ
        {
          options.منوال_2 = S_IRWXUGO;
          options.umask_self = umask_قدر;
        }

      umask (options.umask_self);
    }

  واپس savewd_process_files (argc - optind, argv + optind,
                               process_dir, &options);
}
