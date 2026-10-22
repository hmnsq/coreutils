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
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "mkdir"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS proper_name ("David MacKenzie")

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ longopts[] =
{
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {"mode", required_ವಾದ, ಖಾಲಿ, 'm'},
  {"parents", no_ವಾದ, ಖಾಲಿ, 'p'},
  {"verbose", no_ವಾದ, ಖಾಲಿ, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("Usage: %s [OPTION]... DIRECTORY...\n"), ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
Create the DIRECTORY(ies), ಆದರೆ they ಮಾಡು not already exist.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -m, --ಬಹುಲಕ_2=ಬಹುಲಕ\n\
         ಗಣ_2 file ಬಹುಲಕ_2 (as ಒಳ chmod), not a=rwx - umask\n\
"));
      oputs (_("\
  -p, --parents\n\
         no error ಆದರೆ existing, make ಪೋಷಕ directories as needed,\n\
         with their file modes unaffected by any -m option\n\
"));
      oputs (_("\
  -v, --verbose\n\
         print a ಸಂದೇಶ_2 ಗಾಗಿ each created directory\n\
"));
      oputs (_("\
  -Z\n\
         ಗಣ_2 SELinux security context ದ each created directory\n\
         ಗಮ್ಯ the ಪೂರ್ವನಿಯೋಜಿತ type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or ಆದರೆ CTX is specified then ಗಣ_2 the\n\
         SELinux or SMACK security context ಗಮ್ಯ CTX\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

/* Options passed to subsidiary functions.  */
ರಚನೆ mkdir_options
{
  /* Function to make an ancestor, or NULL if ancestors should not be
     made.  */
  ಪೂರ್ಣಾಂಕ (*make_ancestor_function) (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *, ಶೂನ್ಯ *);

  /* Umask value for when making an ancestor.  */
  mode_t umask_ancestor;

  /* Umask value for when making the directory itself.  */
  mode_t umask_self;

  /* Mode for directory itself.  */
  mode_t ಬಹುಲಕ_2;

  /* File mode bits affected by MODE.  */
  mode_t ಬಹುಲಕ_ಬಿಟ್ಟುಗಳು;

  /* Set the SELinux File Context.  */
  ರಚನೆ selabel_handle *ಗಣ_security_context;

  /* If not null, format to use when reporting newly made directories.  */
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *created_directory_format;
};

/* Report that directory DIR was made, if OPTIONS requests this.  */
ಸ್ಥಿರ ಶೂನ್ಯ
announce_mkdir (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dir, ಶೂನ್ಯ *options)
{
  ರಚನೆ mkdir_options ಸ್ಥಿರಾಂಕ *o = options;
  ಆದರೆ (o->created_directory_format)
    prog_fprintf (stdout, o->created_directory_format, quoteaf (dir));
}

/* Make ancestor directory DIR, whose last component is COMPONENT,
   with options OPTIONS.  Assume the working directory is COMPONENT's
   parent.  Return 0 if successful and the resulting directory is
   readable, 1 if successful but the resulting directory is not
   readable, -1 (setting errno) otherwise.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
make_ancestor (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dir, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *component, ಶೂನ್ಯ *options)
{
  ರಚನೆ mkdir_options ಸ್ಥಿರಾಂಕ *o = options;

  ಆದರೆ (o->ಗಣ_security_context
      && defaultcon (o->ಗಣ_security_context, component, S_IFDIR) < 0
      && ! ignorable_ctx_err (errno))
    error (0, errno, _("failed to set default creation context for %s"),
           quoteaf (dir));

  ಆದರೆ (o->umask_ancestor != o->umask_self)
    umask (o->umask_ancestor);
  ಪೂರ್ಣಾಂಕ r = mkdir (component, S_IRWXUGO);
  ಆದರೆ (o->umask_ancestor != o->umask_self)
    {
      ಪೂರ್ಣಾಂಕ mkdir_errno = errno;
      umask (o->umask_self);
      errno = mkdir_errno;
    }
  ಆದರೆ (r == 0)
    {
      r = (o->umask_ancestor & S_IRUSR) != 0;
      announce_mkdir (dir, options);
    }
  ಹಿಂತಿರುಗಿ r;
}

/* Process a command-line file name.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
process_dir (ಅಕ್ಷರ *dir, ರಚನೆ savewd *wd, ಶೂನ್ಯ *options)
{
  ರಚನೆ mkdir_options ಸ್ಥಿರಾಂಕ *o = options;

  /* If possible set context before DIR created.  */
  ಆದರೆ (o->ಗಣ_security_context)
    {
      ಆದರೆ (! o->make_ancestor_function
          && defaultcon (o->ಗಣ_security_context, dir, S_IFDIR) < 0
          && ! ignorable_ctx_err (errno))
        error (0, errno, _("failed to set default creation context for %s"),
               quoteaf (dir));
    }

  ಪೂರ್ಣಾಂಕ ret = (make_dir_parents (dir, wd, o->make_ancestor_function, options,
                               o->ಬಹುಲಕ_2, announce_mkdir,
                               o->ಬಹುಲಕ_ಬಿಟ್ಟುಗಳು, (uid_t) -1, (gid_t) -1, true)
             ? EXIT_SUCCESS
             : EXIT_FAILURE);

  /* FIXME: Due to the current structure of make_dir_parents()
     we don't have the facility to call defaultcon() before the
     final component of DIR is created.  So for now, create the
     final component with the context from previous component
     and here we set the context for the final component. */
  ಆದರೆ (ret == EXIT_SUCCESS && o->ಗಣ_security_context
      && o->make_ancestor_function)
    {
      ಆದರೆ (! restorecon (o->ಗಣ_security_context, ಕೊನೆಯ_component (dir), false)
          && ! ignorable_ctx_err (errno))
        error (0, errno, _("failed to restore context for %s"),
               quoteaf (dir));
    }

  ಹಿಂತಿರುಗಿ ret;
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *specified_ಬಹುಲಕ = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ optc;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *scontext = ಖಾಲಿ;
  ರಚನೆ mkdir_options options;

  options.make_ancestor_function = ಖಾಲಿ;
  options.ಬಹುಲಕ_2 = S_IRWXUGO;
  options.ಬಹುಲಕ_ಬಿಟ್ಟುಗಳು = 0;
  options.created_directory_format = ಖಾಲಿ;
  options.ಗಣ_security_context = ಖಾಲಿ;

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಆಗ ((optc = getopt_long (argc, argv, "pm:vZ", longopts, ಖಾಲಿ)) != -1)
    {
      ಆಯ್ಕೆ (optc)
        {
        ಸ್ಥಿತಿ 'p':
          options.make_ancestor_function = make_ancestor;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'm':
          specified_ಬಹುಲಕ = optarg;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'v': /* --verbose  */
          options.created_directory_format = _("created directory %s");
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'Z':
          ಆದರೆ (is_smack_enabled ())
            {
              /* We don't yet support -Z to restore context with SMACK.  */
              scontext = optarg;
            }
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (is_selinux_enabled () > 0)
            {
              ಆದರೆ (optarg)
                scontext = optarg;
              ಇಲ್ಲದಿದ್ದರೆ
                {
                  options.ಗಣ_security_context = selabel_open (SELABEL_CTX_FILE,
                                                               ಖಾಲಿ, 0);
                  ಆದರೆ (! options.ಗಣ_security_context)
                    error (0, errno, _("warning: ignoring --context"));
                }
            }
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux/SMACK-enabled kernel"));
            }
          ನಿಲ್ಲಿಸು;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);
        ಪೂರ್ವನಿಯೋಜಿತ:
          usage (EXIT_FAILURE);
        }
    }

  ಆದರೆ (optind == argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  /* FIXME: This assumes mkdir() is done in the same process.
     If that's not always the case we would need to call this
     like we do when options.set_security_context.  */
  ಆದರೆ (scontext)
    {
      ಪೂರ್ಣಾಂಕ ret = 0;
      ಆದರೆ (is_smack_enabled ())
        ret = smack_ಗಣ_label_for_self (scontext);
      ಇಲ್ಲದಿದ್ದರೆ
        ret = setfscreatecon (scontext);

      ಆದರೆ (ret < 0)
        error (EXIT_FAILURE, errno,
               _("failed to set default file creation context to %s"),
               quote (scontext));
    }


  ಆದರೆ (options.make_ancestor_function || specified_ಬಹುಲಕ)
    {
      mode_t umask_ಮೌಲ್ಯ = umask (0);
      options.umask_ancestor = umask_ಮೌಲ್ಯ & ~(S_IWUSR | S_IXUSR);

      ಆದರೆ (specified_ಬಹುಲಕ)
        {
          ರಚನೆ ಬಹುಲಕ_change *change = ಬಹುಲಕ_compile (specified_ಬಹುಲಕ);
          ಆದರೆ (!change)
            error (EXIT_FAILURE, 0, _("invalid mode %s"),
                   quote (specified_ಬಹುಲಕ));
          options.ಬಹುಲಕ_2 = ಬಹುಲಕ_adjust (S_IRWXUGO, true, umask_ಮೌಲ್ಯ, change,
                                      &options.ಬಹುಲಕ_ಬಿಟ್ಟುಗಳು);
          options.umask_self = umask_ಮೌಲ್ಯ & ~options.ಬಹುಲಕ_2;
          ಬಿಡುಗಡೆ (change);
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          options.ಬಹುಲಕ_2 = S_IRWXUGO;
          options.umask_self = umask_ಮೌಲ್ಯ;
        }

      umask (options.umask_self);
    }

  ಹಿಂತಿರುಗಿ savewd_process_files (argc - optind, argv + optind,
                               process_dir, &options);
}
