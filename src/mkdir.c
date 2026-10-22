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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <getopt.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <selinux/label.h>

#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "mkdir-p.h"
#അന്തര്ഭാവ "modechange.h"
#അന്തര്ഭാവ "prog-fprintf.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "savewd.h"
#അന്തര്ഭാവ "selinux.h"
#അന്തര്ഭാവ "smack.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "mkdir"

#define AUTHORS proper_name ("David MacKenzie")

സ്ഥിതം ഘടന option സ്ഥിരം longopts[] =
{
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {"mode", required_argument, ശൂന്യ, 'm'},
  {"parents", no_argument, ശൂന്യ, 'p'},
  {"verbose", no_argument, ശൂന്യ, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ശൂന്യ, 0, ശൂന്യ, 0}
};

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
    {
      printf (_("Usage: %s [OPTION]... DIRECTORY...\n"), program_name);
      fputs (_("\
Create the DIRECTORY(ies), എങ്കിൽ they ചെയ്യുക not already exist.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -m, --mode=MODE\n\
         set file mode (as in chmod), not a=rwx - umask\n\
"));
      oputs (_("\
  -p, --parents\n\
         no error എങ്കിൽ existing, make parent directories as needed,\n\
         with their file modes unaffected by any -m option\n\
"));
      oputs (_("\
  -v, --verbose\n\
         print a message വേണ്ടി each created directory\n\
"));
      oputs (_("\
  -Z\n\
         set SELinux security context of each created directory\n\
         to the സ്ഥിരസ്ഥിതി type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or എങ്കിൽ CTX is specified then set the\n\
         SELinux or SMACK security context to CTX\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Options passed to subsidiary functions.  */
ഘടന mkdir_options
{
  /* Function to make an ancestor, or NULL if ancestors should not be
     made.  */
  പൂർണ്ണസംഖ്യ (*make_ancestor_function) (അക്ഷരം സ്ഥിരം *, അക്ഷരം സ്ഥിരം *, ശൂന്യം *);

  /* Umask value for when making an ancestor.  */
  mode_t umask_ancestor;

  /* Umask value for when making the directory itself.  */
  mode_t umask_self;

  /* Mode for directory itself.  */
  mode_t mode;

  /* File mode bits affected by MODE.  */
  mode_t mode_bits;

  /* Set the SELinux File Context.  */
  ഘടന selabel_handle *set_security_context;

  /* If not null, format to use when reporting newly made directories.  */
  അക്ഷരം സ്ഥിരം *created_directory_format;
};

/* Report that directory DIR was made, if OPTIONS requests this.  */
സ്ഥിതം ശൂന്യം
announce_mkdir (അക്ഷരം സ്ഥിരം *dir, ശൂന്യം *options)
{
  ഘടന mkdir_options സ്ഥിരം *o = options;
  എങ്കിൽ (o->created_directory_format)
    prog_fprintf (stdout, o->created_directory_format, quoteaf (dir));
}

/* Make ancestor directory DIR, whose last component is COMPONENT,
   with options OPTIONS.  Assume the working directory is COMPONENT's
   parent.  Return 0 if successful and the resulting directory is
   readable, 1 if successful but the resulting directory is not
   readable, -1 (setting errno) otherwise.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
make_ancestor (അക്ഷരം സ്ഥിരം *dir, അക്ഷരം സ്ഥിരം *component, ശൂന്യം *options)
{
  ഘടന mkdir_options സ്ഥിരം *o = options;

  എങ്കിൽ (o->set_security_context
      && defaultcon (o->set_security_context, component, S_IFDIR) < 0
      && ! ignorable_ctx_err (errno))
    error (0, errno, _("failed to set default creation context for %s"),
           quoteaf (dir));

  എങ്കിൽ (o->umask_ancestor != o->umask_self)
    umask (o->umask_ancestor);
  പൂർണ്ണസംഖ്യ r = mkdir (component, S_IRWXUGO);
  എങ്കിൽ (o->umask_ancestor != o->umask_self)
    {
      പൂർണ്ണസംഖ്യ mkdir_errno = errno;
      umask (o->umask_self);
      errno = mkdir_errno;
    }
  എങ്കിൽ (r == 0)
    {
      r = (o->umask_ancestor & S_IRUSR) != 0;
      announce_mkdir (dir, options);
    }
  തിരികെ r;
}

/* Process a command-line file name.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
process_dir (അക്ഷരം *dir, ഘടന savewd *wd, ശൂന്യം *options)
{
  ഘടന mkdir_options സ്ഥിരം *o = options;

  /* If possible set context before DIR created.  */
  എങ്കിൽ (o->set_security_context)
    {
      എങ്കിൽ (! o->make_ancestor_function
          && defaultcon (o->set_security_context, dir, S_IFDIR) < 0
          && ! ignorable_ctx_err (errno))
        error (0, errno, _("failed to set default creation context for %s"),
               quoteaf (dir));
    }

  പൂർണ്ണസംഖ്യ ret = (make_dir_parents (dir, wd, o->make_ancestor_function, options,
                               o->mode, announce_mkdir,
                               o->mode_bits, (uid_t) -1, (gid_t) -1, true)
             ? EXIT_SUCCESS
             : EXIT_FAILURE);

  /* FIXME: Due to the current structure of make_dir_parents()
     we don't have the facility to call defaultcon() before the
     final component of DIR is created.  So for now, create the
     final component with the context from previous component
     and here we set the context for the final component. */
  എങ്കിൽ (ret == EXIT_SUCCESS && o->set_security_context
      && o->make_ancestor_function)
    {
      എങ്കിൽ (! restorecon (o->set_security_context, last_component (dir), false)
          && ! ignorable_ctx_err (errno))
        error (0, errno, _("failed to restore context for %s"),
               quoteaf (dir));
    }

  തിരികെ ret;
}

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  അക്ഷരം സ്ഥിരം *specified_mode = ശൂന്യ;
  പൂർണ്ണസംഖ്യ optc;
  അക്ഷരം സ്ഥിരം *scontext = ശൂന്യ;
  ഘടന mkdir_options options;

  options.make_ancestor_function = ശൂന്യ;
  options.mode = S_IRWXUGO;
  options.mode_bits = 0;
  options.created_directory_format = ശൂന്യ;
  options.set_security_context = ശൂന്യ;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ആകുമ്പോൾ ((optc = getopt_long (argc, argv, "pm:vZ", longopts, ശൂന്യ)) != -1)
    {
      തിരഞ്ഞെടുപ്പ് (optc)
        {
        അവസ്ഥ 'p':
          options.make_ancestor_function = make_ancestor;
          നിർത്തുക;
        അവസ്ഥ 'm':
          specified_mode = optarg;
          നിർത്തുക;
        അവസ്ഥ 'v': /* --verbose  */
          options.created_directory_format = _("created directory %s");
          നിർത്തുക;
        അവസ്ഥ 'Z':
          എങ്കിൽ (is_smack_enabled ())
            {
              /* We don't yet support -Z to restore context with SMACK.  */
              scontext = optarg;
            }
          അല്ലെങ്കിൽ എങ്കിൽ (is_selinux_enabled () > 0)
            {
              എങ്കിൽ (optarg)
                scontext = optarg;
              അല്ലെങ്കിൽ
                {
                  options.set_security_context = selabel_open (SELABEL_CTX_FILE,
                                                               ശൂന്യ, 0);
                  എങ്കിൽ (! options.set_security_context)
                    error (0, errno, _("warning: ignoring --context"));
                }
            }
          അല്ലെങ്കിൽ എങ്കിൽ (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux/SMACK-enabled kernel"));
            }
          നിർത്തുക;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        സ്ഥിരസ്ഥിതി:
          usage (EXIT_FAILURE);
        }
    }

  എങ്കിൽ (optind == argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  /* FIXME: This assumes mkdir() is done in the same process.
     If that's not always the case we would need to call this
     like we do when options.set_security_context.  */
  എങ്കിൽ (scontext)
    {
      പൂർണ്ണസംഖ്യ ret = 0;
      എങ്കിൽ (is_smack_enabled ())
        ret = smack_set_label_for_self (scontext);
      അല്ലെങ്കിൽ
        ret = setfscreatecon (scontext);

      എങ്കിൽ (ret < 0)
        error (EXIT_FAILURE, errno,
               _("failed to set default file creation context to %s"),
               quote (scontext));
    }


  എങ്കിൽ (options.make_ancestor_function || specified_mode)
    {
      mode_t umask_മൂല്യം = umask (0);
      options.umask_ancestor = umask_മൂല്യം & ~(S_IWUSR | S_IXUSR);

      എങ്കിൽ (specified_mode)
        {
          ഘടന mode_change *change = mode_compile (specified_mode);
          എങ്കിൽ (!change)
            error (EXIT_FAILURE, 0, _("invalid mode %s"),
                   quote (specified_mode));
          options.mode = mode_adjust (S_IRWXUGO, true, umask_മൂല്യം, change,
                                      &options.mode_bits);
          options.umask_self = umask_മൂല്യം & ~options.mode;
          സ്വതന്ത്രം (change);
        }
      അല്ലെങ്കിൽ
        {
          options.mode = S_IRWXUGO;
          options.umask_self = umask_മൂല്യം;
        }

      umask (options.umask_self);
    }

  തിരികെ savewd_process_files (argc - optind, argv + optind,
                               process_dir, &options);
}
