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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <selinux/label.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "mkdir-p.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "modechange.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "prog-fprintf.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "savewd.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "selinux.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "smack.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "mkdir"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS proper_name ("David MacKenzie")

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ longopts[] =
{
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {"mode", required_ਦਲੀਲ, ਨਿਰਾ, 'm'},
  {"parents", no_ਦਲੀਲ, ਨਿਰਾ, 'p'},
  {"verbose", no_ਦਲੀਲ, ਨਿਰਾ, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("Usage: %s [OPTION]... DIRECTORY...\n"), ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
Create the DIRECTORY(ies), ਜੇ they ਕਰੋ not already exist.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -m, --ਬਹੁਲਕ_2=ਬਹੁਲਕ\n\
         ਸੈੱਟ_2 file ਬਹੁਲਕ_2 (as ਵਿੱਚ chmod), not a=rwx - umask\n\
"));
      oputs (_("\
  -p, --parents\n\
         no error ਜੇ existing, make ਮਾਪਾ directories as needed,\n\
         with their file modes unaffected by any -m option\n\
"));
      oputs (_("\
  -v, --verbose\n\
         print a ਸੁਨੇਹਾ_2 ਲਈ each created directory\n\
"));
      oputs (_("\
  -Z\n\
         ਸੈੱਟ_2 SELinux security context ਦਾ each created directory\n\
         ਮੰਜ਼ਿਲ the ਮੂਲ type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or ਜੇ CTX is specified then ਸੈੱਟ_2 the\n\
         SELinux or SMACK security context ਮੰਜ਼ਿਲ CTX\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

/* Options passed to subsidiary functions.  */
ਬਣਤਰ mkdir_options
{
  /* Function to make an ancestor, or NULL if ancestors should not be
     made.  */
  ਪੂਰਨ (*make_ancestor_function) (ਅੱਖਰ ਅਚਲ *, ਅੱਖਰ ਅਚਲ *, ਖਾਲੀ *);

  /* Umask value for when making an ancestor.  */
  mode_t umask_ancestor;

  /* Umask value for when making the directory itself.  */
  mode_t umask_self;

  /* Mode for directory itself.  */
  mode_t ਬਹੁਲਕ_2;

  /* File mode bits affected by MODE.  */
  mode_t ਬਹੁਲਕ_ਬਿੱਟ;

  /* Set the SELinux File Context.  */
  ਬਣਤਰ selabel_handle *ਸੈੱਟ_security_context;

  /* If not null, format to use when reporting newly made directories.  */
  ਅੱਖਰ ਅਚਲ *created_directory_format;
};

/* Report that directory DIR was made, if OPTIONS requests this.  */
ਸਥਿਰ ਖਾਲੀ
announce_mkdir (ਅੱਖਰ ਅਚਲ *dir, ਖਾਲੀ *options)
{
  ਬਣਤਰ mkdir_options ਅਚਲ *o = options;
  ਜੇ (o->created_directory_format)
    prog_fprintf (stdout, o->created_directory_format, quoteaf (dir));
}

/* Make ancestor directory DIR, whose last component is COMPONENT,
   with options OPTIONS.  Assume the working directory is COMPONENT's
   parent.  Return 0 if successful and the resulting directory is
   readable, 1 if successful but the resulting directory is not
   readable, -1 (setting errno) otherwise.  */
ਸਥਿਰ ਪੂਰਨ
make_ancestor (ਅੱਖਰ ਅਚਲ *dir, ਅੱਖਰ ਅਚਲ *component, ਖਾਲੀ *options)
{
  ਬਣਤਰ mkdir_options ਅਚਲ *o = options;

  ਜੇ (o->ਸੈੱਟ_security_context
      && defaultcon (o->ਸੈੱਟ_security_context, component, S_IFDIR) < 0
      && ! ignorable_ctx_err (errno))
    error (0, errno, _("failed to set default creation context for %s"),
           quoteaf (dir));

  ਜੇ (o->umask_ancestor != o->umask_self)
    umask (o->umask_ancestor);
  ਪੂਰਨ r = mkdir (component, S_IRWXUGO);
  ਜੇ (o->umask_ancestor != o->umask_self)
    {
      ਪੂਰਨ mkdir_errno = errno;
      umask (o->umask_self);
      errno = mkdir_errno;
    }
  ਜੇ (r == 0)
    {
      r = (o->umask_ancestor & S_IRUSR) != 0;
      announce_mkdir (dir, options);
    }
  ਵਾਪਸ r;
}

/* Process a command-line file name.  */
ਸਥਿਰ ਪੂਰਨ
process_dir (ਅੱਖਰ *dir, ਬਣਤਰ savewd *wd, ਖਾਲੀ *options)
{
  ਬਣਤਰ mkdir_options ਅਚਲ *o = options;

  /* If possible set context before DIR created.  */
  ਜੇ (o->ਸੈੱਟ_security_context)
    {
      ਜੇ (! o->make_ancestor_function
          && defaultcon (o->ਸੈੱਟ_security_context, dir, S_IFDIR) < 0
          && ! ignorable_ctx_err (errno))
        error (0, errno, _("failed to set default creation context for %s"),
               quoteaf (dir));
    }

  ਪੂਰਨ ret = (make_dir_parents (dir, wd, o->make_ancestor_function, options,
                               o->ਬਹੁਲਕ_2, announce_mkdir,
                               o->ਬਹੁਲਕ_ਬਿੱਟ, (uid_t) -1, (gid_t) -1, true)
             ? EXIT_SUCCESS
             : EXIT_FAILURE);

  /* FIXME: Due to the current structure of make_dir_parents()
     we don't have the facility to call defaultcon() before the
     final component of DIR is created.  So for now, create the
     final component with the context from previous component
     and here we set the context for the final component. */
  ਜੇ (ret == EXIT_SUCCESS && o->ਸੈੱਟ_security_context
      && o->make_ancestor_function)
    {
      ਜੇ (! restorecon (o->ਸੈੱਟ_security_context, ਆਖਰੀ_component (dir), false)
          && ! ignorable_ctx_err (errno))
        error (0, errno, _("failed to restore context for %s"),
               quoteaf (dir));
    }

  ਵਾਪਸ ret;
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਅੱਖਰ ਅਚਲ *specified_ਬਹੁਲਕ = ਨਿਰਾ;
  ਪੂਰਨ optc;
  ਅੱਖਰ ਅਚਲ *scontext = ਨਿਰਾ;
  ਬਣਤਰ mkdir_options options;

  options.make_ancestor_function = ਨਿਰਾ;
  options.ਬਹੁਲਕ_2 = S_IRWXUGO;
  options.ਬਹੁਲਕ_ਬਿੱਟ = 0;
  options.created_directory_format = ਨਿਰਾ;
  options.ਸੈੱਟ_security_context = ਨਿਰਾ;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ਜਦੋਂ ((optc = getopt_long (argc, argv, "pm:vZ", longopts, ਨਿਰਾ)) != -1)
    {
      ਚੋਣ (optc)
        {
        ਹਾਲਤ 'p':
          options.make_ancestor_function = make_ancestor;
          ਰੋਕੋ;
        ਹਾਲਤ 'm':
          specified_ਬਹੁਲਕ = optarg;
          ਰੋਕੋ;
        ਹਾਲਤ 'v': /* --verbose  */
          options.created_directory_format = _("created directory %s");
          ਰੋਕੋ;
        ਹਾਲਤ 'Z':
          ਜੇ (is_smack_enabled ())
            {
              /* We don't yet support -Z to restore context with SMACK.  */
              scontext = optarg;
            }
          ਨਹੀਂਤਾਂ ਜੇ (is_selinux_enabled () > 0)
            {
              ਜੇ (optarg)
                scontext = optarg;
              ਨਹੀਂਤਾਂ
                {
                  options.ਸੈੱਟ_security_context = selabel_open (SELABEL_CTX_FILE,
                                                               ਨਿਰਾ, 0);
                  ਜੇ (! options.ਸੈੱਟ_security_context)
                    error (0, errno, _("warning: ignoring --context"));
                }
            }
          ਨਹੀਂਤਾਂ ਜੇ (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux/SMACK-enabled kernel"));
            }
          ਰੋਕੋ;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);
        ਮੂਲ:
          usage (EXIT_FAILURE);
        }
    }

  ਜੇ (optind == argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  /* FIXME: This assumes mkdir() is done in the same process.
     If that's not always the case we would need to call this
     like we do when options.set_security_context.  */
  ਜੇ (scontext)
    {
      ਪੂਰਨ ret = 0;
      ਜੇ (is_smack_enabled ())
        ret = smack_ਸੈੱਟ_label_for_self (scontext);
      ਨਹੀਂਤਾਂ
        ret = setfscreatecon (scontext);

      ਜੇ (ret < 0)
        error (EXIT_FAILURE, errno,
               _("failed to set default file creation context to %s"),
               quote (scontext));
    }


  ਜੇ (options.make_ancestor_function || specified_ਬਹੁਲਕ)
    {
      mode_t umask_ਮੁੱਲ = umask (0);
      options.umask_ancestor = umask_ਮੁੱਲ & ~(S_IWUSR | S_IXUSR);

      ਜੇ (specified_ਬਹੁਲਕ)
        {
          ਬਣਤਰ ਬਹੁਲਕ_change *change = ਬਹੁਲਕ_compile (specified_ਬਹੁਲਕ);
          ਜੇ (!change)
            error (EXIT_FAILURE, 0, _("invalid mode %s"),
                   quote (specified_ਬਹੁਲਕ));
          options.ਬਹੁਲਕ_2 = ਬਹੁਲਕ_adjust (S_IRWXUGO, true, umask_ਮੁੱਲ, change,
                                      &options.ਬਹੁਲਕ_ਬਿੱਟ);
          options.umask_self = umask_ਮੁੱਲ & ~options.ਬਹੁਲਕ_2;
          ਮੁਕਤ (change);
        }
      ਨਹੀਂਤਾਂ
        {
          options.ਬਹੁਲਕ_2 = S_IRWXUGO;
          options.umask_self = umask_ਮੁੱਲ;
        }

      umask (options.umask_self);
    }

  ਵਾਪਸ savewd_process_files (argc - optind, argv + optind,
                               process_dir, &options);
}
