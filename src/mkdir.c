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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <getopt.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <selinux/label.h>

#অন্তর্ভাব "system.h"
#অন্তর্ভাব "mkdir-p.h"
#অন্তর্ভাব "modechange.h"
#অন্তর্ভাব "prog-fprintf.h"
#অন্তর্ভাব "quote.h"
#অন্তর্ভাব "savewd.h"
#অন্তর্ভাব "selinux.h"
#অন্তর্ভাব "smack.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "mkdir"

#define AUTHORS proper_name ("David MacKenzie")

স্থির কাঠামো option ধ্রুবক longopts[] =
{
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {"mode", required_argument, ফাঁকা, 'm'},
  {"parents", no_argument, ফাঁকা, 'p'},
  {"verbose", no_argument, ফাঁকা, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ফাঁকা, 0, ফাঁকা, 0}
};

শূন্য
usage (পূর্ণসংখ্যা status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
    {
      printf (_("Usage: %s [OPTION]... DIRECTORY...\n"), program_name);
      fputs (_("\
Create the DIRECTORY(ies), যদি they করো not already exist.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -m, --mode=MODE\n\
         set file mode (as in chmod), not a=rwx - umask\n\
"));
      oputs (_("\
  -p, --parents\n\
         no error যদি existing, make parent directories as needed,\n\
         with their file modes unaffected by any -m option\n\
"));
      oputs (_("\
  -v, --verbose\n\
         print a message জন্য each created directory\n\
"));
      oputs (_("\
  -Z\n\
         set SELinux security context of each created directory\n\
         to the পূর্বনির্ধারিত type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or যদি CTX is specified then set the\n\
         SELinux or SMACK security context to CTX\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Options passed to subsidiary functions.  */
কাঠামো mkdir_options
{
  /* Function to make an ancestor, or NULL if ancestors should not be
     made.  */
  পূর্ণসংখ্যা (*make_ancestor_function) (অক্ষর ধ্রুবক *, অক্ষর ধ্রুবক *, শূন্য *);

  /* Umask value for when making an ancestor.  */
  mode_t umask_ancestor;

  /* Umask value for when making the directory itself.  */
  mode_t umask_self;

  /* Mode for directory itself.  */
  mode_t mode;

  /* File mode bits affected by MODE.  */
  mode_t mode_bits;

  /* Set the SELinux File Context.  */
  কাঠামো selabel_handle *set_security_context;

  /* If not null, format to use when reporting newly made directories.  */
  অক্ষর ধ্রুবক *created_directory_format;
};

/* Report that directory DIR was made, if OPTIONS requests this.  */
স্থির শূন্য
announce_mkdir (অক্ষর ধ্রুবক *dir, শূন্য *options)
{
  কাঠামো mkdir_options ধ্রুবক *o = options;
  যদি (o->created_directory_format)
    prog_fprintf (stdout, o->created_directory_format, quoteaf (dir));
}

/* Make ancestor directory DIR, whose last component is COMPONENT,
   with options OPTIONS.  Assume the working directory is COMPONENT's
   parent.  Return 0 if successful and the resulting directory is
   readable, 1 if successful but the resulting directory is not
   readable, -1 (setting errno) otherwise.  */
স্থির পূর্ণসংখ্যা
make_ancestor (অক্ষর ধ্রুবক *dir, অক্ষর ধ্রুবক *component, শূন্য *options)
{
  কাঠামো mkdir_options ধ্রুবক *o = options;

  যদি (o->set_security_context
      && defaultcon (o->set_security_context, component, S_IFDIR) < 0
      && ! ignorable_ctx_err (errno))
    error (0, errno, _("failed to set default creation context for %s"),
           quoteaf (dir));

  যদি (o->umask_ancestor != o->umask_self)
    umask (o->umask_ancestor);
  পূর্ণসংখ্যা r = mkdir (component, S_IRWXUGO);
  যদি (o->umask_ancestor != o->umask_self)
    {
      পূর্ণসংখ্যা mkdir_errno = errno;
      umask (o->umask_self);
      errno = mkdir_errno;
    }
  যদি (r == 0)
    {
      r = (o->umask_ancestor & S_IRUSR) != 0;
      announce_mkdir (dir, options);
    }
  ফেরত r;
}

/* Process a command-line file name.  */
স্থির পূর্ণসংখ্যা
process_dir (অক্ষর *dir, কাঠামো savewd *wd, শূন্য *options)
{
  কাঠামো mkdir_options ধ্রুবক *o = options;

  /* If possible set context before DIR created.  */
  যদি (o->set_security_context)
    {
      যদি (! o->make_ancestor_function
          && defaultcon (o->set_security_context, dir, S_IFDIR) < 0
          && ! ignorable_ctx_err (errno))
        error (0, errno, _("failed to set default creation context for %s"),
               quoteaf (dir));
    }

  পূর্ণসংখ্যা ret = (make_dir_parents (dir, wd, o->make_ancestor_function, options,
                               o->mode, announce_mkdir,
                               o->mode_bits, (uid_t) -1, (gid_t) -1, true)
             ? EXIT_SUCCESS
             : EXIT_FAILURE);

  /* FIXME: Due to the current structure of make_dir_parents()
     we don't have the facility to call defaultcon() before the
     final component of DIR is created.  So for now, create the
     final component with the context from previous component
     and here we set the context for the final component. */
  যদি (ret == EXIT_SUCCESS && o->set_security_context
      && o->make_ancestor_function)
    {
      যদি (! restorecon (o->set_security_context, last_component (dir), false)
          && ! ignorable_ctx_err (errno))
        error (0, errno, _("failed to restore context for %s"),
               quoteaf (dir));
    }

  ফেরত ret;
}

পূর্ণসংখ্যা
প্রধান (পূর্ণসংখ্যা argc, অক্ষর **argv)
{
  অক্ষর ধ্রুবক *specified_mode = ফাঁকা;
  পূর্ণসংখ্যা optc;
  অক্ষর ধ্রুবক *scontext = ফাঁকা;
  কাঠামো mkdir_options options;

  options.make_ancestor_function = ফাঁকা;
  options.mode = S_IRWXUGO;
  options.mode_bits = 0;
  options.created_directory_format = ফাঁকা;
  options.set_security_context = ফাঁকা;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  যখন ((optc = getopt_long (argc, argv, "pm:vZ", longopts, ফাঁকা)) != -1)
    {
      নির্বাচন (optc)
        {
        ক্ষেত্র 'p':
          options.make_ancestor_function = make_ancestor;
          বিরতি;
        ক্ষেত্র 'm':
          specified_mode = optarg;
          বিরতি;
        ক্ষেত্র 'v': /* --verbose  */
          options.created_directory_format = _("created directory %s");
          বিরতি;
        ক্ষেত্র 'Z':
          যদি (is_smack_enabled ())
            {
              /* We don't yet support -Z to restore context with SMACK.  */
              scontext = optarg;
            }
          অন্যথা যদি (is_selinux_enabled () > 0)
            {
              যদি (optarg)
                scontext = optarg;
              অন্যথা
                {
                  options.set_security_context = selabel_open (SELABEL_CTX_FILE,
                                                               ফাঁকা, 0);
                  যদি (! options.set_security_context)
                    error (0, errno, _("warning: ignoring --context"));
                }
            }
          অন্যথা যদি (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux/SMACK-enabled kernel"));
            }
          বিরতি;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        পূর্বনির্ধারিত:
          usage (EXIT_FAILURE);
        }
    }

  যদি (optind == argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  /* FIXME: This assumes mkdir() is done in the same process.
     If that's not always the case we would need to call this
     like we do when options.set_security_context.  */
  যদি (scontext)
    {
      পূর্ণসংখ্যা ret = 0;
      যদি (is_smack_enabled ())
        ret = smack_set_label_for_self (scontext);
      অন্যথা
        ret = setfscreatecon (scontext);

      যদি (ret < 0)
        error (EXIT_FAILURE, errno,
               _("failed to set default file creation context to %s"),
               quote (scontext));
    }


  যদি (options.make_ancestor_function || specified_mode)
    {
      mode_t umask_মান = umask (0);
      options.umask_ancestor = umask_মান & ~(S_IWUSR | S_IXUSR);

      যদি (specified_mode)
        {
          কাঠামো mode_change *change = mode_compile (specified_mode);
          যদি (!change)
            error (EXIT_FAILURE, 0, _("invalid mode %s"),
                   quote (specified_mode));
          options.mode = mode_adjust (S_IRWXUGO, true, umask_মান, change,
                                      &options.mode_bits);
          options.umask_self = umask_মান & ~options.mode;
          মুক্ত (change);
        }
      অন্যথা
        {
          options.mode = S_IRWXUGO;
          options.umask_self = umask_মান;
        }

      umask (options.umask_self);
    }

  ফেরত savewd_process_files (argc - optind, argv + optind,
                               process_dir, &options);
}
