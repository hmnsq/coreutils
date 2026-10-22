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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <getopt.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <selinux/label.h>

#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "mkdir-p.h"
#ଅନ୍ତର୍ଭାଵ "modechange.h"
#ଅନ୍ତର୍ଭାଵ "prog-fprintf.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "savewd.h"
#ଅନ୍ତର୍ଭାଵ "selinux.h"
#ଅନ୍ତର୍ଭାଵ "smack.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "mkdir"

#define AUTHORS proper_name ("David MacKenzie")

ସ୍ଥିର ସଂରଚନା option ଅଚଳ longopts[] =
{
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {"mode", required_argument, ଖାଲି, 'm'},
  {"parents", no_argument, ଖାଲି, 'p'},
  {"verbose", no_argument, ଖାଲି, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ଖାଲି, 0, ଖାଲି, 0}
};

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
    {
      printf (_("Usage: %s [OPTION]... DIRECTORY...\n"), program_name);
      fputs (_("\
Create the DIRECTORY(ies), ଯଦି they କର not already exist.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -m, --mode=MODE\n\
         set file mode (as in chmod), not a=rwx - umask\n\
"));
      oputs (_("\
  -p, --parents\n\
         no error ଯଦି existing, make parent directories as needed,\n\
         with their file modes unaffected by any -m option\n\
"));
      oputs (_("\
  -v, --verbose\n\
         print a message ପାଇଁ each created directory\n\
"));
      oputs (_("\
  -Z\n\
         set SELinux security context of each created directory\n\
         to the ପୂର୍ବନିର୍ଦ୍ଧାରିତ type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or ଯଦି CTX is specified then set the\n\
         SELinux or SMACK security context to CTX\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Options passed to subsidiary functions.  */
ସଂରଚନା mkdir_options
{
  /* Function to make an ancestor, or NULL if ancestors should not be
     made.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*make_ancestor_function) (ଅକ୍ଷର ଅଚଳ *, ଅକ୍ଷର ଅଚଳ *, ଶୂନ୍ୟ *);

  /* Umask value for when making an ancestor.  */
  mode_t umask_ancestor;

  /* Umask value for when making the directory itself.  */
  mode_t umask_self;

  /* Mode for directory itself.  */
  mode_t mode;

  /* File mode bits affected by MODE.  */
  mode_t mode_bits;

  /* Set the SELinux File Context.  */
  ସଂରଚନା selabel_handle *set_security_context;

  /* If not null, format to use when reporting newly made directories.  */
  ଅକ୍ଷର ଅଚଳ *created_directory_format;
};

/* Report that directory DIR was made, if OPTIONS requests this.  */
ସ୍ଥିର ଶୂନ୍ୟ
announce_mkdir (ଅକ୍ଷର ଅଚଳ *dir, ଶୂନ୍ୟ *options)
{
  ସଂରଚନା mkdir_options ଅଚଳ *o = options;
  ଯଦି (o->created_directory_format)
    prog_fprintf (stdout, o->created_directory_format, quoteaf (dir));
}

/* Make ancestor directory DIR, whose last component is COMPONENT,
   with options OPTIONS.  Assume the working directory is COMPONENT's
   parent.  Return 0 if successful and the resulting directory is
   readable, 1 if successful but the resulting directory is not
   readable, -1 (setting errno) otherwise.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
make_ancestor (ଅକ୍ଷର ଅଚଳ *dir, ଅକ୍ଷର ଅଚଳ *component, ଶୂନ୍ୟ *options)
{
  ସଂରଚନା mkdir_options ଅଚଳ *o = options;

  ଯଦି (o->set_security_context
      && defaultcon (o->set_security_context, component, S_IFDIR) < 0
      && ! ignorable_ctx_err (errno))
    error (0, errno, _("failed to set default creation context for %s"),
           quoteaf (dir));

  ଯଦି (o->umask_ancestor != o->umask_self)
    umask (o->umask_ancestor);
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା r = mkdir (component, S_IRWXUGO);
  ଯଦି (o->umask_ancestor != o->umask_self)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା mkdir_errno = errno;
      umask (o->umask_self);
      errno = mkdir_errno;
    }
  ଯଦି (r == 0)
    {
      r = (o->umask_ancestor & S_IRUSR) != 0;
      announce_mkdir (dir, options);
    }
  ଫେରନ୍ତୁ r;
}

/* Process a command-line file name.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
process_dir (ଅକ୍ଷର *dir, ସଂରଚନା savewd *wd, ଶୂନ୍ୟ *options)
{
  ସଂରଚନା mkdir_options ଅଚଳ *o = options;

  /* If possible set context before DIR created.  */
  ଯଦି (o->set_security_context)
    {
      ଯଦି (! o->make_ancestor_function
          && defaultcon (o->set_security_context, dir, S_IFDIR) < 0
          && ! ignorable_ctx_err (errno))
        error (0, errno, _("failed to set default creation context for %s"),
               quoteaf (dir));
    }

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = (make_dir_parents (dir, wd, o->make_ancestor_function, options,
                               o->mode, announce_mkdir,
                               o->mode_bits, (uid_t) -1, (gid_t) -1, true)
             ? EXIT_SUCCESS
             : EXIT_FAILURE);

  /* FIXME: Due to the current structure of make_dir_parents()
     we don't have the facility to call defaultcon() before the
     final component of DIR is created.  So for now, create the
     final component with the context from previous component
     and here we set the context for the final component. */
  ଯଦି (ret == EXIT_SUCCESS && o->set_security_context
      && o->make_ancestor_function)
    {
      ଯଦି (! restorecon (o->set_security_context, last_component (dir), false)
          && ! ignorable_ctx_err (errno))
        error (0, errno, _("failed to restore context for %s"),
               quoteaf (dir));
    }

  ଫେରନ୍ତୁ ret;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  ଅକ୍ଷର ଅଚଳ *specified_mode = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା optc;
  ଅକ୍ଷର ଅଚଳ *scontext = ଖାଲି;
  ସଂରଚନା mkdir_options options;

  options.make_ancestor_function = ଖାଲି;
  options.mode = S_IRWXUGO;
  options.mode_bits = 0;
  options.created_directory_format = ଖାଲି;
  options.set_security_context = ଖାଲି;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ଯେତେବେଳେ ((optc = getopt_long (argc, argv, "pm:vZ", longopts, ଖାଲି)) != -1)
    {
      ବାଛ (optc)
        {
        ସ୍ଥିତି 'p':
          options.make_ancestor_function = make_ancestor;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'm':
          specified_mode = optarg;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'v': /* --verbose  */
          options.created_directory_format = _("created directory %s");
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'Z':
          ଯଦି (is_smack_enabled ())
            {
              /* We don't yet support -Z to restore context with SMACK.  */
              scontext = optarg;
            }
          ଅନ୍ୟଥା ଯଦି (is_selinux_enabled () > 0)
            {
              ଯଦି (optarg)
                scontext = optarg;
              ଅନ୍ୟଥା
                {
                  options.set_security_context = selabel_open (SELABEL_CTX_FILE,
                                                               ଖାଲି, 0);
                  ଯଦି (! options.set_security_context)
                    error (0, errno, _("warning: ignoring --context"));
                }
            }
          ଅନ୍ୟଥା ଯଦି (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux/SMACK-enabled kernel"));
            }
          ଭାଙ୍ଗ;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          usage (EXIT_FAILURE);
        }
    }

  ଯଦି (optind == argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  /* FIXME: This assumes mkdir() is done in the same process.
     If that's not always the case we would need to call this
     like we do when options.set_security_context.  */
  ଯଦି (scontext)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;
      ଯଦି (is_smack_enabled ())
        ret = smack_set_label_for_self (scontext);
      ଅନ୍ୟଥା
        ret = setfscreatecon (scontext);

      ଯଦି (ret < 0)
        error (EXIT_FAILURE, errno,
               _("failed to set default file creation context to %s"),
               quote (scontext));
    }


  ଯଦି (options.make_ancestor_function || specified_mode)
    {
      mode_t umask_ମୂଲ୍ୟ = umask (0);
      options.umask_ancestor = umask_ମୂଲ୍ୟ & ~(S_IWUSR | S_IXUSR);

      ଯଦି (specified_mode)
        {
          ସଂରଚନା mode_change *change = mode_compile (specified_mode);
          ଯଦି (!change)
            error (EXIT_FAILURE, 0, _("invalid mode %s"),
                   quote (specified_mode));
          options.mode = mode_adjust (S_IRWXUGO, true, umask_ମୂଲ୍ୟ, change,
                                      &options.mode_bits);
          options.umask_self = umask_ମୂଲ୍ୟ & ~options.mode;
          ମୁକ୍ତ (change);
        }
      ଅନ୍ୟଥା
        {
          options.mode = S_IRWXUGO;
          options.umask_self = umask_ମୂଲ୍ୟ;
        }

      umask (options.umask_self);
    }

  ଫେରନ୍ତୁ savewd_process_files (argc - optind, argv + optind,
                               process_dir, &options);
}
