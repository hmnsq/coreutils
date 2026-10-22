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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <selinux/label.h>

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "mkdir-p.h"
#ಅನ್ತರ್ಭಾವ "modechange.h"
#ಅನ್ತರ್ಭಾವ "prog-fprintf.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "savewd.h"
#ಅನ್ತರ್ಭಾವ "selinux.h"
#ಅನ್ತರ್ಭಾವ "smack.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "mkdir"

#define AUTHORS proper_name ("David MacKenzie")

ಸ್ಥಿರ ಸಂರಚನಾ option ಧ್ರುವ longopts[] =
{
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {"mode", required_argument, ರಿಕ್ತ, 'm'},
  {"parents", no_argument, ರಿಕ್ತ, 'p'},
  {"verbose", no_argument, ರಿಕ್ತ, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ರಿಕ್ತ, 0, ರಿಕ್ತ, 0}
};

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
    {
      printf (_("Usage: %s [OPTION]... DIRECTORY...\n"), program_name);
      fputs (_("\
Create the DIRECTORY(ies), ಯದಿ they ಕುರು not already exist.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -m, --mode=MODE\n\
         set file mode (as in chmod), not a=rwx - umask\n\
"));
      oputs (_("\
  -p, --parents\n\
         no error ಯದಿ existing, make parent directories as needed,\n\
         with their file modes unaffected by any -m option\n\
"));
      oputs (_("\
  -v, --verbose\n\
         print a message ಪ್ರತಿ each created directory\n\
"));
      oputs (_("\
  -Z\n\
         set SELinux security context of each created directory\n\
         to the ಸ್ವಭಾವ type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or ಯದಿ CTX is specified then set the\n\
         SELinux or SMACK security context to CTX\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Options passed to subsidiary functions.  */
ಸಂರಚನಾ mkdir_options
{
  /* Function to make an ancestor, or NULL if ancestors should not be
     made.  */
  ಪೂರ್ಣಾಙ್ಕ (*make_ancestor_function) (ವರ್ಣ ಧ್ರುವ *, ವರ್ಣ ಧ್ರುವ *, ಶೂನ್ಯ *);

  /* Umask value for when making an ancestor.  */
  mode_t umask_ancestor;

  /* Umask value for when making the directory itself.  */
  mode_t umask_self;

  /* Mode for directory itself.  */
  mode_t mode;

  /* File mode bits affected by MODE.  */
  mode_t mode_bits;

  /* Set the SELinux File Context.  */
  ಸಂರಚನಾ selabel_handle *set_security_context;

  /* If not null, format to use when reporting newly made directories.  */
  ವರ್ಣ ಧ್ರುವ *created_directory_format;
};

/* Report that directory DIR was made, if OPTIONS requests this.  */
ಸ್ಥಿರ ಶೂನ್ಯ
announce_mkdir (ವರ್ಣ ಧ್ರುವ *dir, ಶೂನ್ಯ *options)
{
  ಸಂರಚನಾ mkdir_options ಧ್ರುವ *o = options;
  ಯದಿ (o->created_directory_format)
    prog_fprintf (stdout, o->created_directory_format, quoteaf (dir));
}

/* Make ancestor directory DIR, whose last component is COMPONENT,
   with options OPTIONS.  Assume the working directory is COMPONENT's
   parent.  Return 0 if successful and the resulting directory is
   readable, 1 if successful but the resulting directory is not
   readable, -1 (setting errno) otherwise.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
make_ancestor (ವರ್ಣ ಧ್ರುವ *dir, ವರ್ಣ ಧ್ರುವ *component, ಶೂನ್ಯ *options)
{
  ಸಂರಚನಾ mkdir_options ಧ್ರುವ *o = options;

  ಯದಿ (o->set_security_context
      && defaultcon (o->set_security_context, component, S_IFDIR) < 0
      && ! ignorable_ctx_err (errno))
    error (0, errno, _("failed to set default creation context for %s"),
           quoteaf (dir));

  ಯದಿ (o->umask_ancestor != o->umask_self)
    umask (o->umask_ancestor);
  ಪೂರ್ಣಾಙ್ಕ r = mkdir (component, S_IRWXUGO);
  ಯದಿ (o->umask_ancestor != o->umask_self)
    {
      ಪೂರ್ಣಾಙ್ಕ mkdir_errno = errno;
      umask (o->umask_self);
      errno = mkdir_errno;
    }
  ಯದಿ (r == 0)
    {
      r = (o->umask_ancestor & S_IRUSR) != 0;
      announce_mkdir (dir, options);
    }
  ಪ್ರತ್ಯಾಗಮ r;
}

/* Process a command-line file name.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
process_dir (ವರ್ಣ *dir, ಸಂರಚನಾ savewd *wd, ಶೂನ್ಯ *options)
{
  ಸಂರಚನಾ mkdir_options ಧ್ರುವ *o = options;

  /* If possible set context before DIR created.  */
  ಯದಿ (o->set_security_context)
    {
      ಯದಿ (! o->make_ancestor_function
          && defaultcon (o->set_security_context, dir, S_IFDIR) < 0
          && ! ignorable_ctx_err (errno))
        error (0, errno, _("failed to set default creation context for %s"),
               quoteaf (dir));
    }

  ಪೂರ್ಣಾಙ್ಕ ret = (make_dir_parents (dir, wd, o->make_ancestor_function, options,
                               o->mode, announce_mkdir,
                               o->mode_bits, (uid_t) -1, (gid_t) -1, true)
             ? EXIT_SUCCESS
             : EXIT_FAILURE);

  /* FIXME: Due to the current structure of make_dir_parents()
     we don't have the facility to call defaultcon() before the
     final component of DIR is created.  So for now, create the
     final component with the context from previous component
     and here we set the context for the final component. */
  ಯದಿ (ret == EXIT_SUCCESS && o->set_security_context
      && o->make_ancestor_function)
    {
      ಯದಿ (! restorecon (o->set_security_context, last_component (dir), false)
          && ! ignorable_ctx_err (errno))
        error (0, errno, _("failed to restore context for %s"),
               quoteaf (dir));
    }

  ಪ್ರತ್ಯಾಗಮ ret;
}

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  ವರ್ಣ ಧ್ರುವ *specified_mode = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ optc;
  ವರ್ಣ ಧ್ರುವ *scontext = ರಿಕ್ತ;
  ಸಂರಚನಾ mkdir_options options;

  options.make_ancestor_function = ರಿಕ್ತ;
  options.mode = S_IRWXUGO;
  options.mode_bits = 0;
  options.created_directory_format = ರಿಕ್ತ;
  options.set_security_context = ರಿಕ್ತ;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಯಾವತ್ ((optc = getopt_long (argc, argv, "pm:vZ", longopts, ರಿಕ್ತ)) != -1)
    {
      ವಿಕಲ್ಪ (optc)
        {
        ಪಕ್ಷ 'p':
          options.make_ancestor_function = make_ancestor;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'm':
          specified_mode = optarg;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'v': /* --verbose  */
          options.created_directory_format = _("created directory %s");
          ಭಙ್ಗ;
        ಪಕ್ಷ 'Z':
          ಯದಿ (is_smack_enabled ())
            {
              /* We don't yet support -Z to restore context with SMACK.  */
              scontext = optarg;
            }
          ಅನ್ಯಥಾ ಯದಿ (is_selinux_enabled () > 0)
            {
              ಯದಿ (optarg)
                scontext = optarg;
              ಅನ್ಯಥಾ
                {
                  options.set_security_context = selabel_open (SELABEL_CTX_FILE,
                                                               ರಿಕ್ತ, 0);
                  ಯದಿ (! options.set_security_context)
                    error (0, errno, _("warning: ignoring --context"));
                }
            }
          ಅನ್ಯಥಾ ಯದಿ (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux/SMACK-enabled kernel"));
            }
          ಭಙ್ಗ;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        ಸ್ವಭಾವ:
          usage (EXIT_FAILURE);
        }
    }

  ಯದಿ (optind == argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  /* FIXME: This assumes mkdir() is done in the same process.
     If that's not always the case we would need to call this
     like we do when options.set_security_context.  */
  ಯದಿ (scontext)
    {
      ಪೂರ್ಣಾಙ್ಕ ret = 0;
      ಯದಿ (is_smack_enabled ())
        ret = smack_set_label_for_self (scontext);
      ಅನ್ಯಥಾ
        ret = setfscreatecon (scontext);

      ಯದಿ (ret < 0)
        error (EXIT_FAILURE, errno,
               _("failed to set default file creation context to %s"),
               quote (scontext));
    }


  ಯದಿ (options.make_ancestor_function || specified_mode)
    {
      mode_t umask_ಮೌಲ್ಯ = umask (0);
      options.umask_ancestor = umask_ಮೌಲ್ಯ & ~(S_IWUSR | S_IXUSR);

      ಯದಿ (specified_mode)
        {
          ಸಂರಚನಾ mode_change *change = mode_compile (specified_mode);
          ಯದಿ (!change)
            error (EXIT_FAILURE, 0, _("invalid mode %s"),
                   quote (specified_mode));
          options.mode = mode_adjust (S_IRWXUGO, true, umask_ಮೌಲ್ಯ, change,
                                      &options.mode_bits);
          options.umask_self = umask_ಮೌಲ್ಯ & ~options.mode;
          ಮುಕ್ತ (change);
        }
      ಅನ್ಯಥಾ
        {
          options.mode = S_IRWXUGO;
          options.umask_self = umask_ಮೌಲ್ಯ;
        }

      umask (options.umask_self);
    }

  ಪ್ರತ್ಯಾಗಮ savewd_process_files (argc - optind, argv + optind,
                               process_dir, &options);
}
