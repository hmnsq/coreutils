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
#define PROGRAM_NAME "mkdir"

#define AUTHORS proper_name ("David MacKenzie")

ساكن بنية option ثابت longopts[] =
{
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {"mode", required_argument, عدم, 'm'},
  {"parents", no_argument, عدم, 'p'},
  {"verbose", no_argument, عدم, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح status)
{
  𐤀𐤌 (status != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      printf (_("Usage: %s [OPTION]... DIRECTORY...\n"), program_name);
      fputs (_("\
Create the DIRECTORY(ies), 𐤀𐤌 they 𐤐𐤏𐤋 not already exist.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -m, --mode=MODE\n\
         set file mode (as in chmod), not a=rwx - umask\n\
"));
      oputs (_("\
  -p, --parents\n\
         no error 𐤀𐤌 existing, make parent directories as needed,\n\
         with their file modes unaffected by any -m option\n\
"));
      oputs (_("\
  -v, --verbose\n\
         print a message لكل each created directory\n\
"));
      oputs (_("\
  -Z\n\
         set SELinux security context of each created directory\n\
         to the افتراضي type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or 𐤀𐤌 CTX is specified then set the\n\
         SELinux or SMACK security context to CTX\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
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
  mode_t mode;

  /* File mode bits affected by MODE.  */
  mode_t mode_bits;

  /* Set the SELinux File Context.  */
  بنية selabel_handle *set_security_context;

  /* If not null, format to use when reporting newly made directories.  */
  حرف ثابت *created_directory_format;
};

/* Report that directory DIR was made, if OPTIONS requests this.  */
ساكن فراغ
announce_mkdir (حرف ثابت *dir, فراغ *options)
{
  بنية mkdir_options ثابت *o = options;
  𐤀𐤌 (o->created_directory_format)
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

  𐤀𐤌 (o->set_security_context
      && defaultcon (o->set_security_context, component, S_IFDIR) < 0
      && ! ignorable_ctx_err (errno))
    error (0, errno, _("failed to set default creation context for %s"),
           quoteaf (dir));

  𐤀𐤌 (o->umask_ancestor != o->umask_self)
    umask (o->umask_ancestor);
  صحيح r = mkdir (component, S_IRWXUGO);
  𐤀𐤌 (o->umask_ancestor != o->umask_self)
    {
      صحيح mkdir_errno = errno;
      umask (o->umask_self);
      errno = mkdir_errno;
    }
  𐤀𐤌 (r == 0)
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
  𐤀𐤌 (o->set_security_context)
    {
      𐤀𐤌 (! o->make_ancestor_function
          && defaultcon (o->set_security_context, dir, S_IFDIR) < 0
          && ! ignorable_ctx_err (errno))
        error (0, errno, _("failed to set default creation context for %s"),
               quoteaf (dir));
    }

  صحيح ret = (make_dir_parents (dir, wd, o->make_ancestor_function, options,
                               o->mode, announce_mkdir,
                               o->mode_bits, (uid_t) -1, (gid_t) -1, true)
             ? EXIT_SUCCESS
             : EXIT_FAILURE);

  /* FIXME: Due to the current structure of make_dir_parents()
     we don't have the facility to call defaultcon() before the
     final component of DIR is created.  So for now, create the
     final component with the context from previous component
     and here we set the context for the final component. */
  𐤀𐤌 (ret == EXIT_SUCCESS && o->set_security_context
      && o->make_ancestor_function)
    {
      𐤀𐤌 (! restorecon (o->set_security_context, last_component (dir), false)
          && ! ignorable_ctx_err (errno))
        error (0, errno, _("failed to restore context for %s"),
               quoteaf (dir));
    }

  أرجع ret;
}

صحيح
main (صحيح argc, حرف **argv)
{
  حرف ثابت *specified_mode = عدم;
  صحيح optc;
  حرف ثابت *scontext = عدم;
  بنية mkdir_options options;

  options.make_ancestor_function = عدم;
  options.mode = S_IRWXUGO;
  options.mode_bits = 0;
  options.created_directory_format = عدم;
  options.set_security_context = عدم;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
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
          specified_mode = optarg;
          اكسر;
        حالة 'v': /* --verbose  */
          options.created_directory_format = _("created directory %s");
          اكسر;
        حالة 'Z':
          𐤀𐤌 (is_smack_enabled ())
            {
              /* We don't yet support -Z to restore context with SMACK.  */
              scontext = optarg;
            }
          وإلا 𐤀𐤌 (is_selinux_enabled () > 0)
            {
              𐤀𐤌 (optarg)
                scontext = optarg;
              وإلا
                {
                  options.set_security_context = selabel_open (SELABEL_CTX_FILE,
                                                               عدم, 0);
                  𐤀𐤌 (! options.set_security_context)
                    error (0, errno, _("warning: ignoring --context"));
                }
            }
          وإلا 𐤀𐤌 (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux/SMACK-enabled kernel"));
            }
          اكسر;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  𐤀𐤌 (optind == argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  /* FIXME: This assumes mkdir() is done in the same process.
     If that's not always the case we would need to call this
     like we do when options.set_security_context.  */
  𐤀𐤌 (scontext)
    {
      صحيح ret = 0;
      𐤀𐤌 (is_smack_enabled ())
        ret = smack_set_label_for_self (scontext);
      وإلا
        ret = setfscreatecon (scontext);

      𐤀𐤌 (ret < 0)
        error (EXIT_FAILURE, errno,
               _("failed to set default file creation context to %s"),
               quote (scontext));
    }


  𐤀𐤌 (options.make_ancestor_function || specified_mode)
    {
      mode_t umask_value = umask (0);
      options.umask_ancestor = umask_value & ~(S_IWUSR | S_IXUSR);

      𐤀𐤌 (specified_mode)
        {
          بنية mode_change *change = mode_compile (specified_mode);
          𐤀𐤌 (!change)
            error (EXIT_FAILURE, 0, _("invalid mode %s"),
                   quote (specified_mode));
          options.mode = mode_adjust (S_IRWXUGO, true, umask_value, change,
                                      &options.mode_bits);
          options.umask_self = umask_value & ~options.mode;
          حرر (change);
        }
      وإلا
        {
          options.mode = S_IRWXUGO;
          options.umask_self = umask_value;
        }

      umask (options.umask_self);
    }

  أرجع savewd_process_files (argc - optind, argv + optind,
                               process_dir, &options);
}
