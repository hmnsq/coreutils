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

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <getopt.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <selinux/label.h>

#అన్తర్భావ "system.h"
#అన్తర్భావ "mkdir-p.h"
#అన్తర్భావ "modechange.h"
#అన్తర్భావ "prog-fprintf.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "savewd.h"
#అన్తర్భావ "selinux.h"
#అన్తర్భావ "smack.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "mkdir"

#నిర్వచించు AUTHORS proper_name ("David MacKenzie")

స్థిర నిర్మాణం option స్థిరరాశి longopts[] =
{
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {"mode", required_వాదం, ఖాళీ, 'm'},
  {"parents", no_వాదం, ఖాళీ, 'p'},
  {"verbose", no_వాదం, ఖాళీ, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0}
};

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("Usage: %s [OPTION]... DIRECTORY...\n"), కార్యక్రమం_name);
      fputs (_("\
Create the DIRECTORY(ies), ఒకవేళ they చేయి not already exist.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -m, --బహుళకం_2=బహుళకం\n\
         సమితి_2 file బహుళకం_2 (as లోపల chmod), not a=rwx - umask\n\
"));
      oputs (_("\
  -p, --parents\n\
         no error ఒకవేళ existing, make మాతృ directories as needed,\n\
         with their file modes unaffected by any -m option\n\
"));
      oputs (_("\
  -v, --verbose\n\
         print a సందేశం_2 కొరకు each created directory\n\
"));
      oputs (_("\
  -Z\n\
         సమితి_2 SELinux security context యొక్క each created directory\n\
         గమ్యం the అప్రమేయ type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or ఒకవేళ CTX is specified then సమితి_2 the\n\
         SELinux or SMACK security context గమ్యం CTX\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

/* Options passed to subsidiary functions.  */
నిర్మాణం mkdir_options
{
  /* Function to make an ancestor, or NULL if ancestors should not be
     made.  */
  పూర్ణాంకం (*make_ancestor_function) (అక్షరం స్థిరరాశి *, అక్షరం స్థిరరాశి *, శూన్య *);

  /* Umask value for when making an ancestor.  */
  mode_t umask_ancestor;

  /* Umask value for when making the directory itself.  */
  mode_t umask_self;

  /* Mode for directory itself.  */
  mode_t బహుళకం_2;

  /* File mode bits affected by MODE.  */
  mode_t బహుళకం_బిట్లు;

  /* Set the SELinux File Context.  */
  నిర్మాణం selabel_handle *సమితి_security_context;

  /* If not null, format to use when reporting newly made directories.  */
  అక్షరం స్థిరరాశి *created_directory_format;
};

/* Report that directory DIR was made, if OPTIONS requests this.  */
స్థిర శూన్య
announce_mkdir (అక్షరం స్థిరరాశి *dir, శూన్య *options)
{
  నిర్మాణం mkdir_options స్థిరరాశి *o = options;
  ఒకవేళ (o->created_directory_format)
    prog_fprintf (stdout, o->created_directory_format, quoteaf (dir));
}

/* Make ancestor directory DIR, whose last component is COMPONENT,
   with options OPTIONS.  Assume the working directory is COMPONENT's
   parent.  Return 0 if successful and the resulting directory is
   readable, 1 if successful but the resulting directory is not
   readable, -1 (setting errno) otherwise.  */
స్థిర పూర్ణాంకం
make_ancestor (అక్షరం స్థిరరాశి *dir, అక్షరం స్థిరరాశి *component, శూన్య *options)
{
  నిర్మాణం mkdir_options స్థిరరాశి *o = options;

  ఒకవేళ (o->సమితి_security_context
      && defaultcon (o->సమితి_security_context, component, S_IFDIR) < 0
      && ! ignorable_ctx_err (errno))
    error (0, errno, _("failed to set default creation context for %s"),
           quoteaf (dir));

  ఒకవేళ (o->umask_ancestor != o->umask_self)
    umask (o->umask_ancestor);
  పూర్ణాంకం r = mkdir (component, S_IRWXUGO);
  ఒకవేళ (o->umask_ancestor != o->umask_self)
    {
      పూర్ణాంకం mkdir_errno = errno;
      umask (o->umask_self);
      errno = mkdir_errno;
    }
  ఒకవేళ (r == 0)
    {
      r = (o->umask_ancestor & S_IRUSR) != 0;
      announce_mkdir (dir, options);
    }
  తిరిగి r;
}

/* Process a command-line file name.  */
స్థిర పూర్ణాంకం
process_dir (అక్షరం *dir, నిర్మాణం savewd *wd, శూన్య *options)
{
  నిర్మాణం mkdir_options స్థిరరాశి *o = options;

  /* If possible set context before DIR created.  */
  ఒకవేళ (o->సమితి_security_context)
    {
      ఒకవేళ (! o->make_ancestor_function
          && defaultcon (o->సమితి_security_context, dir, S_IFDIR) < 0
          && ! ignorable_ctx_err (errno))
        error (0, errno, _("failed to set default creation context for %s"),
               quoteaf (dir));
    }

  పూర్ణాంకం ret = (make_dir_parents (dir, wd, o->make_ancestor_function, options,
                               o->బహుళకం_2, announce_mkdir,
                               o->బహుళకం_బిట్లు, (uid_t) -1, (gid_t) -1, true)
             ? EXIT_SUCCESS
             : EXIT_FAILURE);

  /* FIXME: Due to the current structure of make_dir_parents()
     we don't have the facility to call defaultcon() before the
     final component of DIR is created.  So for now, create the
     final component with the context from previous component
     and here we set the context for the final component. */
  ఒకవేళ (ret == EXIT_SUCCESS && o->సమితి_security_context
      && o->make_ancestor_function)
    {
      ఒకవేళ (! restorecon (o->సమితి_security_context, చివరి_component (dir), false)
          && ! ignorable_ctx_err (errno))
        error (0, errno, _("failed to restore context for %s"),
               quoteaf (dir));
    }

  తిరిగి ret;
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  అక్షరం స్థిరరాశి *specified_బహుళకం = ఖాళీ;
  పూర్ణాంకం optc;
  అక్షరం స్థిరరాశి *scontext = ఖాళీ;
  నిర్మాణం mkdir_options options;

  options.make_ancestor_function = ఖాళీ;
  options.బహుళకం_2 = S_IRWXUGO;
  options.బహుళకం_బిట్లు = 0;
  options.created_directory_format = ఖాళీ;
  options.సమితి_security_context = ఖాళీ;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  అయితే ((optc = getopt_long (argc, argv, "pm:vZ", longopts, ఖాళీ)) != -1)
    {
      ఎంపిక (optc)
        {
        స్థితి 'p':
          options.make_ancestor_function = make_ancestor;
          ఆపు;
        స్థితి 'm':
          specified_బహుళకం = optarg;
          ఆపు;
        స్థితి 'v': /* --verbose  */
          options.created_directory_format = _("created directory %s");
          ఆపు;
        స్థితి 'Z':
          ఒకవేళ (is_smack_enabled ())
            {
              /* We don't yet support -Z to restore context with SMACK.  */
              scontext = optarg;
            }
          లేకపోతే ఒకవేళ (is_selinux_enabled () > 0)
            {
              ఒకవేళ (optarg)
                scontext = optarg;
              లేకపోతే
                {
                  options.సమితి_security_context = selabel_open (SELABEL_CTX_FILE,
                                                               ఖాళీ, 0);
                  ఒకవేళ (! options.సమితి_security_context)
                    error (0, errno, _("warning: ignoring --context"));
                }
            }
          లేకపోతే ఒకవేళ (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux/SMACK-enabled kernel"));
            }
          ఆపు;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);
        అప్రమేయ:
          usage (EXIT_FAILURE);
        }
    }

  ఒకవేళ (optind == argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  /* FIXME: This assumes mkdir() is done in the same process.
     If that's not always the case we would need to call this
     like we do when options.set_security_context.  */
  ఒకవేళ (scontext)
    {
      పూర్ణాంకం ret = 0;
      ఒకవేళ (is_smack_enabled ())
        ret = smack_సమితి_label_for_self (scontext);
      లేకపోతే
        ret = setfscreatecon (scontext);

      ఒకవేళ (ret < 0)
        error (EXIT_FAILURE, errno,
               _("failed to set default file creation context to %s"),
               quote (scontext));
    }


  ఒకవేళ (options.make_ancestor_function || specified_బహుళకం)
    {
      mode_t umask_విలువ = umask (0);
      options.umask_ancestor = umask_విలువ & ~(S_IWUSR | S_IXUSR);

      ఒకవేళ (specified_బహుళకం)
        {
          నిర్మాణం బహుళకం_change *change = బహుళకం_compile (specified_బహుళకం);
          ఒకవేళ (!change)
            error (EXIT_FAILURE, 0, _("invalid mode %s"),
                   quote (specified_బహుళకం));
          options.బహుళకం_2 = బహుళకం_adjust (S_IRWXUGO, true, umask_విలువ, change,
                                      &options.బహుళకం_బిట్లు);
          options.umask_self = umask_విలువ & ~options.బహుళకం_2;
          విడుదల (change);
        }
      లేకపోతే
        {
          options.బహుళకం_2 = S_IRWXUGO;
          options.umask_self = umask_విలువ;
        }

      umask (options.umask_self);
    }

  తిరిగి savewd_process_files (argc - optind, argv + optind,
                               process_dir, &options);
}
