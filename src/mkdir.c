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

#ضمن <config.h>
#ضمن <stdio.h>
#ضمن <getopt.h>
#ضمن <sys/types.h>
#ضمن <selinux/label.h>

#ضمن "system.h"
#ضمن "mkdir-p.h"
#ضمن "modechange.h"
#ضمن "prog-fprintf.h"
#ضمن "quote.h"
#ضمن "savewd.h"
#ضمن "selinux.h"
#ضمن "smack.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "mkdir"

#تعریف_کرو AUTHORS proper_name ("David MacKenzie")

ساكن بنية option ثابت longopts[] =
{
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {"mode", required_دلیل, عدم, 'm'},
  {"parents", no_دلیل, عدم, 'p'},
  {"verbose", no_دلیل, عدم, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکھو (_("Usage: %s [OPTION]... DIRECTORY...\n"), پروگرام_name);
      fputs (_("\
Create the DIRECTORY(ies), إذا they افعل not already exist.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -m, --منوال_2=منوال\n\
         سیٹ_2 file منوال_2 (as وچ chmod), not a=rwx - umask\n\
"));
      oputs (_("\
  -p, --parents\n\
         no error إذا existing, make ماپا directories as needed,\n\
         with their file modes unaffected by any -m option\n\
"));
      oputs (_("\
  -v, --verbose\n\
         print a سنیہا_2 لكل each created directory\n\
"));
      oputs (_("\
  -Z\n\
         سیٹ_2 SELinux security context دا each created directory\n\
         منزل the افتراضي type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or إذا CTX is specified then سیٹ_2 the\n\
         SELinux or SMACK security context منزل CTX\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت);
}

/* Options passed to subsidiary functions.  */
بنية mkdir_options
{
  /* Function to make an ancestor, or NULL if ancestors should not be
     made.  */
  صحيح (*make_ancestor_function) (حرف ثابت *, حرف ثابت *, فراغ *);

  /* Umask value for when making an ancestor.  */
  mode_t umask_ancestor;

  /* Umask value for when making the directory itself.  */
  mode_t umask_self;

  /* Mode for directory itself.  */
  mode_t منوال_2;

  /* File mode bits affected by MODE.  */
  mode_t منوال_بٹ;

  /* Set the SELinux File Context.  */
  بنية selabel_handle *سیٹ_security_context;

  /* If not null, format to use when reporting newly made directories.  */
  حرف ثابت *created_directory_format;
};

/* Report that directory DIR was made, if OPTIONS requests this.  */
ساكن فراغ
announce_mkdir (حرف ثابت *dir, فراغ *options)
{
  بنية mkdir_options ثابت *o = options;
  إذا (o->created_directory_format)
    prog_fprintf (stdout, o->created_directory_format, quoteaf (dir));
}

/* Make ancestor directory DIR, whose last component is COMPONENT,
   with options OPTIONS.  Assume the working directory is COMPONENT's
   parent.  Return 0 if successful and the resulting directory is
   readable, 1 if successful but the resulting directory is not
   readable, -1 (setting errno) otherwise.  */
ساكن صحيح
make_ancestor (حرف ثابت *dir, حرف ثابت *component, فراغ *options)
{
  بنية mkdir_options ثابت *o = options;

  إذا (o->سیٹ_security_context
      && defaultcon (o->سیٹ_security_context, component, S_IFDIR) < 0
      && ! ignorable_ctx_err (errno))
    error (0, errno, _("failed to set default creation context for %s"),
           quoteaf (dir));

  إذا (o->umask_ancestor != o->umask_self)
    umask (o->umask_ancestor);
  صحيح r = mkdir (component, S_IRWXUGO);
  إذا (o->umask_ancestor != o->umask_self)
    {
      صحيح mkdir_errno = errno;
      umask (o->umask_self);
      errno = mkdir_errno;
    }
  إذا (r == 0)
    {
      r = (o->umask_ancestor & S_IRUSR) != 0;
      announce_mkdir (dir, options);
    }
  أرجع r;
}

/* Process a command-line file name.  */
ساكن صحيح
process_dir (حرف *dir, بنية savewd *wd, فراغ *options)
{
  بنية mkdir_options ثابت *o = options;

  /* If possible set context before DIR created.  */
  إذا (o->سیٹ_security_context)
    {
      إذا (! o->make_ancestor_function
          && defaultcon (o->سیٹ_security_context, dir, S_IFDIR) < 0
          && ! ignorable_ctx_err (errno))
        error (0, errno, _("failed to set default creation context for %s"),
               quoteaf (dir));
    }

  صحيح ret = (make_dir_parents (dir, wd, o->make_ancestor_function, options,
                               o->منوال_2, announce_mkdir,
                               o->منوال_بٹ, (uid_t) -1, (gid_t) -1, true)
             ? EXIT_SUCCESS
             : EXIT_FAILURE);

  /* FIXME: Due to the current structure of make_dir_parents()
     we don't have the facility to call defaultcon() before the
     final component of DIR is created.  So for now, create the
     final component with the context from previous component
     and here we set the context for the final component. */
  إذا (ret == EXIT_SUCCESS && o->سیٹ_security_context
      && o->make_ancestor_function)
    {
      إذا (! restorecon (o->سیٹ_security_context, آخری_component (dir), false)
          && ! ignorable_ctx_err (errno))
        error (0, errno, _("failed to restore context for %s"),
               quoteaf (dir));
    }

  أرجع ret;
}

صحيح
مکھ (صحيح argc, حرف **argv)
{
  حرف ثابت *specified_منوال = عدم;
  صحيح optc;
  حرف ثابت *scontext = عدم;
  بنية mkdir_options options;

  options.make_ancestor_function = عدم;
  options.منوال_2 = S_IRWXUGO;
  options.منوال_بٹ = 0;
  options.created_directory_format = عدم;
  options.سیٹ_security_context = عدم;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  طالما ((optc = getopt_long (argc, argv, "pm:vZ", longopts, عدم)) != -1)
    {
      بدل (optc)
        {
        حالة 'p':
          options.make_ancestor_function = make_ancestor;
          اكسر;
        حالة 'm':
          specified_منوال = optarg;
          اكسر;
        حالة 'v': /* --verbose  */
          options.created_directory_format = _("created directory %s");
          اكسر;
        حالة 'Z':
          إذا (is_smack_enabled ())
            {
              /* We don't yet support -Z to restore context with SMACK.  */
              scontext = optarg;
            }
          وإلا إذا (is_selinux_enabled () > 0)
            {
              إذا (optarg)
                scontext = optarg;
              وإلا
                {
                  options.سیٹ_security_context = selabel_open (SELABEL_CTX_FILE,
                                                               عدم, 0);
                  إذا (! options.سیٹ_security_context)
                    error (0, errno, _("warning: ignoring --context"));
                }
            }
          وإلا إذا (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux/SMACK-enabled kernel"));
            }
          اكسر;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);
        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  إذا (optind == argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  /* FIXME: This assumes mkdir() is done in the same process.
     If that's not always the case we would need to call this
     like we do when options.set_security_context.  */
  إذا (scontext)
    {
      صحيح ret = 0;
      إذا (is_smack_enabled ())
        ret = smack_سیٹ_label_for_self (scontext);
      وإلا
        ret = setfscreatecon (scontext);

      إذا (ret < 0)
        error (EXIT_FAILURE, errno,
               _("failed to set default file creation context to %s"),
               quote (scontext));
    }


  إذا (options.make_ancestor_function || specified_منوال)
    {
      mode_t umask_قدر = umask (0);
      options.umask_ancestor = umask_قدر & ~(S_IWUSR | S_IXUSR);

      إذا (specified_منوال)
        {
          بنية منوال_change *change = منوال_compile (specified_منوال);
          إذا (!change)
            error (EXIT_FAILURE, 0, _("invalid mode %s"),
                   quote (specified_منوال));
          options.منوال_2 = منوال_adjust (S_IRWXUGO, true, umask_قدر, change,
                                      &options.منوال_بٹ);
          options.umask_self = umask_قدر & ~options.منوال_2;
          حرر (change);
        }
      وإلا
        {
          options.منوال_2 = S_IRWXUGO;
          options.umask_self = umask_قدر;
        }

      umask (options.umask_self);
    }

  أرجع savewd_process_files (argc - optind, argv + optind,
                               process_dir, &options);
}
