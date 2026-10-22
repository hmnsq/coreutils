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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <getopt.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <selinux/label.h>

#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "mkdir-p.h"
#અન્તર્ભાવ "modechange.h"
#અન્તર્ભાવ "prog-fprintf.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "savewd.h"
#અન્તર્ભાવ "selinux.h"
#અન્તર્ભાવ "smack.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "mkdir"

#define AUTHORS proper_name ("David MacKenzie")

સ્થિર સંરચના option અચલ longopts[] =
{
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {"mode", required_argument, ખાલી, 'm'},
  {"parents", no_argument, ખાલી, 'p'},
  {"verbose", no_argument, ખાલી, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ખાલી, 0, ખાલી, 0}
};

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
    {
      printf (_("Usage: %s [OPTION]... DIRECTORY...\n"), program_name);
      fputs (_("\
Create the DIRECTORY(ies), જો they કરો not already exist.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -m, --mode=MODE\n\
         set file mode (as in chmod), not a=rwx - umask\n\
"));
      oputs (_("\
  -p, --parents\n\
         no error જો existing, make parent directories as needed,\n\
         with their file modes unaffected by any -m option\n\
"));
      oputs (_("\
  -v, --verbose\n\
         print a message માટે each created directory\n\
"));
      oputs (_("\
  -Z\n\
         set SELinux security context of each created directory\n\
         to the મૂળભૂત type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or જો CTX is specified then set the\n\
         SELinux or SMACK security context to CTX\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Options passed to subsidiary functions.  */
સંરચના mkdir_options
{
  /* Function to make an ancestor, or NULL if ancestors should not be
     made.  */
  પૂર્ણાંક (*make_ancestor_function) (અક્ષર અચલ *, અક્ષર અચલ *, શૂન્ય *);

  /* Umask value for when making an ancestor.  */
  mode_t umask_ancestor;

  /* Umask value for when making the directory itself.  */
  mode_t umask_self;

  /* Mode for directory itself.  */
  mode_t mode;

  /* File mode bits affected by MODE.  */
  mode_t mode_bits;

  /* Set the SELinux File Context.  */
  સંરચના selabel_handle *set_security_context;

  /* If not null, format to use when reporting newly made directories.  */
  અક્ષર અચલ *created_directory_format;
};

/* Report that directory DIR was made, if OPTIONS requests this.  */
સ્થિર શૂન્ય
announce_mkdir (અક્ષર અચલ *dir, શૂન્ય *options)
{
  સંરચના mkdir_options અચલ *o = options;
  જો (o->created_directory_format)
    prog_fprintf (stdout, o->created_directory_format, quoteaf (dir));
}

/* Make ancestor directory DIR, whose last component is COMPONENT,
   with options OPTIONS.  Assume the working directory is COMPONENT's
   parent.  Return 0 if successful and the resulting directory is
   readable, 1 if successful but the resulting directory is not
   readable, -1 (setting errno) otherwise.  */
સ્થિર પૂર્ણાંક
make_ancestor (અક્ષર અચલ *dir, અક્ષર અચલ *component, શૂન્ય *options)
{
  સંરચના mkdir_options અચલ *o = options;

  જો (o->set_security_context
      && defaultcon (o->set_security_context, component, S_IFDIR) < 0
      && ! ignorable_ctx_err (errno))
    error (0, errno, _("failed to set default creation context for %s"),
           quoteaf (dir));

  જો (o->umask_ancestor != o->umask_self)
    umask (o->umask_ancestor);
  પૂર્ણાંક r = mkdir (component, S_IRWXUGO);
  જો (o->umask_ancestor != o->umask_self)
    {
      પૂર્ણાંક mkdir_errno = errno;
      umask (o->umask_self);
      errno = mkdir_errno;
    }
  જો (r == 0)
    {
      r = (o->umask_ancestor & S_IRUSR) != 0;
      announce_mkdir (dir, options);
    }
  પાછા r;
}

/* Process a command-line file name.  */
સ્થિર પૂર્ણાંક
process_dir (અક્ષર *dir, સંરચના savewd *wd, શૂન્ય *options)
{
  સંરચના mkdir_options અચલ *o = options;

  /* If possible set context before DIR created.  */
  જો (o->set_security_context)
    {
      જો (! o->make_ancestor_function
          && defaultcon (o->set_security_context, dir, S_IFDIR) < 0
          && ! ignorable_ctx_err (errno))
        error (0, errno, _("failed to set default creation context for %s"),
               quoteaf (dir));
    }

  પૂર્ણાંક ret = (make_dir_parents (dir, wd, o->make_ancestor_function, options,
                               o->mode, announce_mkdir,
                               o->mode_bits, (uid_t) -1, (gid_t) -1, true)
             ? EXIT_SUCCESS
             : EXIT_FAILURE);

  /* FIXME: Due to the current structure of make_dir_parents()
     we don't have the facility to call defaultcon() before the
     final component of DIR is created.  So for now, create the
     final component with the context from previous component
     and here we set the context for the final component. */
  જો (ret == EXIT_SUCCESS && o->set_security_context
      && o->make_ancestor_function)
    {
      જો (! restorecon (o->set_security_context, last_component (dir), false)
          && ! ignorable_ctx_err (errno))
        error (0, errno, _("failed to restore context for %s"),
               quoteaf (dir));
    }

  પાછા ret;
}

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  અક્ષર અચલ *specified_mode = ખાલી;
  પૂર્ણાંક optc;
  અક્ષર અચલ *scontext = ખાલી;
  સંરચના mkdir_options options;

  options.make_ancestor_function = ખાલી;
  options.mode = S_IRWXUGO;
  options.mode_bits = 0;
  options.created_directory_format = ખાલી;
  options.set_security_context = ખાલી;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  જ્યારે ((optc = getopt_long (argc, argv, "pm:vZ", longopts, ખાલી)) != -1)
    {
      પસંદગી (optc)
        {
        સ્થિતિ 'p':
          options.make_ancestor_function = make_ancestor;
          તોડો;
        સ્થિતિ 'm':
          specified_mode = optarg;
          તોડો;
        સ્થિતિ 'v': /* --verbose  */
          options.created_directory_format = _("created directory %s");
          તોડો;
        સ્થિતિ 'Z':
          જો (is_smack_enabled ())
            {
              /* We don't yet support -Z to restore context with SMACK.  */
              scontext = optarg;
            }
          નહીંતર જો (is_selinux_enabled () > 0)
            {
              જો (optarg)
                scontext = optarg;
              નહીંતર
                {
                  options.set_security_context = selabel_open (SELABEL_CTX_FILE,
                                                               ખાલી, 0);
                  જો (! options.set_security_context)
                    error (0, errno, _("warning: ignoring --context"));
                }
            }
          નહીંતર જો (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux/SMACK-enabled kernel"));
            }
          તોડો;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        મૂળભૂત:
          usage (EXIT_FAILURE);
        }
    }

  જો (optind == argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  /* FIXME: This assumes mkdir() is done in the same process.
     If that's not always the case we would need to call this
     like we do when options.set_security_context.  */
  જો (scontext)
    {
      પૂર્ણાંક ret = 0;
      જો (is_smack_enabled ())
        ret = smack_set_label_for_self (scontext);
      નહીંતર
        ret = setfscreatecon (scontext);

      જો (ret < 0)
        error (EXIT_FAILURE, errno,
               _("failed to set default file creation context to %s"),
               quote (scontext));
    }


  જો (options.make_ancestor_function || specified_mode)
    {
      mode_t umask_કિંમત = umask (0);
      options.umask_ancestor = umask_કિંમત & ~(S_IWUSR | S_IXUSR);

      જો (specified_mode)
        {
          સંરચના mode_change *change = mode_compile (specified_mode);
          જો (!change)
            error (EXIT_FAILURE, 0, _("invalid mode %s"),
                   quote (specified_mode));
          options.mode = mode_adjust (S_IRWXUGO, true, umask_કિંમત, change,
                                      &options.mode_bits);
          options.umask_self = umask_કિંમત & ~options.mode;
          મુક્ત (change);
        }
      નહીંતર
        {
          options.mode = S_IRWXUGO;
          options.umask_self = umask_કિંમત;
        }

      umask (options.umask_self);
    }

  પાછા savewd_process_files (argc - optind, argv + optind,
                               process_dir, &options);
}
