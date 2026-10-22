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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <getopt.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <selinux/label.h>

#அந்தர்பாவ "system.h"
#அந்தர்பாவ "mkdir-p.h"
#அந்தர்பாவ "modechange.h"
#அந்தர்பாவ "prog-fprintf.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "savewd.h"
#அந்தர்பாவ "selinux.h"
#அந்தர்பாவ "smack.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "mkdir"

#define AUTHORS proper_name ("David MacKenzie")

நிலையான கட்டமைப்பு option மாறிலி longopts[] =
{
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {"mode", required_argument, வெற்று, 'm'},
  {"parents", no_argument, வெற்று, 'p'},
  {"verbose", no_argument, வெற்று, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {வெற்று, 0, வெற்று, 0}
};

வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
    {
      printf (_("Usage: %s [OPTION]... DIRECTORY...\n"), program_name);
      fputs (_("\
Create the DIRECTORY(ies), எனில் they செய் not already exist.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -m, --mode=MODE\n\
         set file mode (as in chmod), not a=rwx - umask\n\
"));
      oputs (_("\
  -p, --parents\n\
         no error எனில் existing, make parent directories as needed,\n\
         with their file modes unaffected by any -m option\n\
"));
      oputs (_("\
  -v, --verbose\n\
         print a message க்கு each created directory\n\
"));
      oputs (_("\
  -Z\n\
         set SELinux security context of each created directory\n\
         to the இயல்பு type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or எனில் CTX is specified then set the\n\
         SELinux or SMACK security context to CTX\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Options passed to subsidiary functions.  */
கட்டமைப்பு mkdir_options
{
  /* Function to make an ancestor, or NULL if ancestors should not be
     made.  */
  முழு (*make_ancestor_function) (எழுத்து மாறிலி *, எழுத்து மாறிலி *, வெறுமை *);

  /* Umask value for when making an ancestor.  */
  mode_t umask_ancestor;

  /* Umask value for when making the directory itself.  */
  mode_t umask_self;

  /* Mode for directory itself.  */
  mode_t mode;

  /* File mode bits affected by MODE.  */
  mode_t mode_bits;

  /* Set the SELinux File Context.  */
  கட்டமைப்பு selabel_handle *set_security_context;

  /* If not null, format to use when reporting newly made directories.  */
  எழுத்து மாறிலி *created_directory_format;
};

/* Report that directory DIR was made, if OPTIONS requests this.  */
நிலையான வெறுமை
announce_mkdir (எழுத்து மாறிலி *dir, வெறுமை *options)
{
  கட்டமைப்பு mkdir_options மாறிலி *o = options;
  எனில் (o->created_directory_format)
    prog_fprintf (stdout, o->created_directory_format, quoteaf (dir));
}

/* Make ancestor directory DIR, whose last component is COMPONENT,
   with options OPTIONS.  Assume the working directory is COMPONENT's
   parent.  Return 0 if successful and the resulting directory is
   readable, 1 if successful but the resulting directory is not
   readable, -1 (setting errno) otherwise.  */
நிலையான முழு
make_ancestor (எழுத்து மாறிலி *dir, எழுத்து மாறிலி *component, வெறுமை *options)
{
  கட்டமைப்பு mkdir_options மாறிலி *o = options;

  எனில் (o->set_security_context
      && defaultcon (o->set_security_context, component, S_IFDIR) < 0
      && ! ignorable_ctx_err (errno))
    error (0, errno, _("failed to set default creation context for %s"),
           quoteaf (dir));

  எனில் (o->umask_ancestor != o->umask_self)
    umask (o->umask_ancestor);
  முழு r = mkdir (component, S_IRWXUGO);
  எனில் (o->umask_ancestor != o->umask_self)
    {
      முழு mkdir_errno = errno;
      umask (o->umask_self);
      errno = mkdir_errno;
    }
  எனில் (r == 0)
    {
      r = (o->umask_ancestor & S_IRUSR) != 0;
      announce_mkdir (dir, options);
    }
  திரும்பு r;
}

/* Process a command-line file name.  */
நிலையான முழு
process_dir (எழுத்து *dir, கட்டமைப்பு savewd *wd, வெறுமை *options)
{
  கட்டமைப்பு mkdir_options மாறிலி *o = options;

  /* If possible set context before DIR created.  */
  எனில் (o->set_security_context)
    {
      எனில் (! o->make_ancestor_function
          && defaultcon (o->set_security_context, dir, S_IFDIR) < 0
          && ! ignorable_ctx_err (errno))
        error (0, errno, _("failed to set default creation context for %s"),
               quoteaf (dir));
    }

  முழு ret = (make_dir_parents (dir, wd, o->make_ancestor_function, options,
                               o->mode, announce_mkdir,
                               o->mode_bits, (uid_t) -1, (gid_t) -1, true)
             ? EXIT_SUCCESS
             : EXIT_FAILURE);

  /* FIXME: Due to the current structure of make_dir_parents()
     we don't have the facility to call defaultcon() before the
     final component of DIR is created.  So for now, create the
     final component with the context from previous component
     and here we set the context for the final component. */
  எனில் (ret == EXIT_SUCCESS && o->set_security_context
      && o->make_ancestor_function)
    {
      எனில் (! restorecon (o->set_security_context, last_component (dir), false)
          && ! ignorable_ctx_err (errno))
        error (0, errno, _("failed to restore context for %s"),
               quoteaf (dir));
    }

  திரும்பு ret;
}

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  எழுத்து மாறிலி *specified_mode = வெற்று;
  முழு optc;
  எழுத்து மாறிலி *scontext = வெற்று;
  கட்டமைப்பு mkdir_options options;

  options.make_ancestor_function = வெற்று;
  options.mode = S_IRWXUGO;
  options.mode_bits = 0;
  options.created_directory_format = வெற்று;
  options.set_security_context = வெற்று;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  போது ((optc = getopt_long (argc, argv, "pm:vZ", longopts, வெற்று)) != -1)
    {
      தேர்வு (optc)
        {
        நிலை 'p':
          options.make_ancestor_function = make_ancestor;
          நிறுத்து;
        நிலை 'm':
          specified_mode = optarg;
          நிறுத்து;
        நிலை 'v': /* --verbose  */
          options.created_directory_format = _("created directory %s");
          நிறுத்து;
        நிலை 'Z':
          எனில் (is_smack_enabled ())
            {
              /* We don't yet support -Z to restore context with SMACK.  */
              scontext = optarg;
            }
          இல்லையெனில் எனில் (is_selinux_enabled () > 0)
            {
              எனில் (optarg)
                scontext = optarg;
              இல்லையெனில்
                {
                  options.set_security_context = selabel_open (SELABEL_CTX_FILE,
                                                               வெற்று, 0);
                  எனில் (! options.set_security_context)
                    error (0, errno, _("warning: ignoring --context"));
                }
            }
          இல்லையெனில் எனில் (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux/SMACK-enabled kernel"));
            }
          நிறுத்து;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        இயல்பு:
          usage (EXIT_FAILURE);
        }
    }

  எனில் (optind == argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  /* FIXME: This assumes mkdir() is done in the same process.
     If that's not always the case we would need to call this
     like we do when options.set_security_context.  */
  எனில் (scontext)
    {
      முழு ret = 0;
      எனில் (is_smack_enabled ())
        ret = smack_set_label_for_self (scontext);
      இல்லையெனில்
        ret = setfscreatecon (scontext);

      எனில் (ret < 0)
        error (EXIT_FAILURE, errno,
               _("failed to set default file creation context to %s"),
               quote (scontext));
    }


  எனில் (options.make_ancestor_function || specified_mode)
    {
      mode_t umask_மதிப்பு = umask (0);
      options.umask_ancestor = umask_மதிப்பு & ~(S_IWUSR | S_IXUSR);

      எனில் (specified_mode)
        {
          கட்டமைப்பு mode_change *change = mode_compile (specified_mode);
          எனில் (!change)
            error (EXIT_FAILURE, 0, _("invalid mode %s"),
                   quote (specified_mode));
          options.mode = mode_adjust (S_IRWXUGO, true, umask_மதிப்பு, change,
                                      &options.mode_bits);
          options.umask_self = umask_மதிப்பு & ~options.mode;
          விடுவி (change);
        }
      இல்லையெனில்
        {
          options.mode = S_IRWXUGO;
          options.umask_self = umask_மதிப்பு;
        }

      umask (options.umask_self);
    }

  திரும்பு savewd_process_files (argc - optind, argv + optind,
                               process_dir, &options);
}
