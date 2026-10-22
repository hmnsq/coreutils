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

#include <config.h>
#include <stdio.h>
#include <getopt.h>
#include <sys/types.h>
#include <selinux/label.h>

#include "system.h"
#include "mkdir-p.h"
#include "modechange.h"
#include "prog-fprintf.h"
#include "quote.h"
#include "savewd.h"
#include "selinux.h"
#include "smack.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "mkdir"

#define AUTHORS proper_name ("David MacKenzie")

static sa_nracanaa option const longopts[] =
{
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {"mode", required_argument, NULL, 'm'},
  {"parents", no_argument, NULL, 'p'},
  {"verbose", no_argument, NULL, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0}
};

void
usage (පූර්ණ_සංඛ්‍යා status)
{
  නම් (status != EXIT_SUCCESS)
    emit_try_help ();
  නැතහොත්
    {
      printf (_("Usage: %s [OPTION]... DIRECTORY...\n"), program_name);
      fputs (_("\
Create the DIRECTORY(ies), නම් they do not already exist.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -m, --mode=MODE\n\
         set file mode (as in chmod), not a=rwx - umask\n\
"));
      oputs (_("\
  -p, --parents\n\
         no error නම් existing, make parent directories as needed,\n\
         with their file modes unaffected by any -m option\n\
"));
      oputs (_("\
  -v, --verbose\n\
         print a message සඳහා each created directory\n\
"));
      oputs (_("\
  -Z\n\
         set SELinux security context of each created directory\n\
         to the default type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or නම් CTX is specified then set the\n\
         SELinux or SMACK security context to CTX\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Options passed to subsidiary functions.  */
sa_nracanaa mkdir_options
{
  /* Function to make an ancestor, or NULL if ancestors should not be
     made.  */
  පූර්ණ_සංඛ්‍යා (*make_ancestor_function) (var_na const *, var_na const *, void *);

  /* Umask value for when making an ancestor.  */
  mode_t umask_ancestor;

  /* Umask value for when making the directory itself.  */
  mode_t umask_self;

  /* Mode for directory itself.  */
  mode_t mode;

  /* File mode bits affected by MODE.  */
  mode_t mode_bits;

  /* Set the SELinux File Context.  */
  sa_nracanaa selabel_handle *set_security_context;

  /* If not null, format to use when reporting newly made directories.  */
  var_na const *created_directory_format;
};

/* Report that directory DIR was made, if OPTIONS requests this.  */
static void
announce_mkdir (var_na const *dir, void *options)
{
  sa_nracanaa mkdir_options const *o = options;
  නම් (o->created_directory_format)
    prog_fprintf (stdout, o->created_directory_format, quoteaf (dir));
}

/* Make ancestor directory DIR, whose last component is COMPONENT,
   with options OPTIONS.  Assume the working directory is COMPONENT's
   parent.  Return 0 if successful and the resulting directory is
   readable, 1 if successful but the resulting directory is not
   readable, -1 (setting errno) otherwise.  */
static පූර්ණ_සංඛ්‍යා
make_ancestor (var_na const *dir, var_na const *component, void *options)
{
  sa_nracanaa mkdir_options const *o = options;

  නම් (o->set_security_context
      && defaultcon (o->set_security_context, component, S_IFDIR) < 0
      && ! ignorable_ctx_err (errno))
    error (0, errno, _("failed to set default creation context for %s"),
           quoteaf (dir));

  නම් (o->umask_ancestor != o->umask_self)
    umask (o->umask_ancestor);
  පූර්ණ_සංඛ්‍යා r = mkdir (component, S_IRWXUGO);
  නම් (o->umask_ancestor != o->umask_self)
    {
      පූර්ණ_සංඛ්‍යා mkdir_errno = errno;
      umask (o->umask_self);
      errno = mkdir_errno;
    }
  නම් (r == 0)
    {
      r = (o->umask_ancestor & S_IRUSR) != 0;
      announce_mkdir (dir, options);
    }
  ආපසු r;
}

/* Process a command-line file name.  */
static පූර්ණ_සංඛ්‍යා
process_dir (var_na *dir, sa_nracanaa savewd *wd, void *options)
{
  sa_nracanaa mkdir_options const *o = options;

  /* If possible set context before DIR created.  */
  නම් (o->set_security_context)
    {
      නම් (! o->make_ancestor_function
          && defaultcon (o->set_security_context, dir, S_IFDIR) < 0
          && ! ignorable_ctx_err (errno))
        error (0, errno, _("failed to set default creation context for %s"),
               quoteaf (dir));
    }

  පූර්ණ_සංඛ්‍යා ret = (make_dir_parents (dir, wd, o->make_ancestor_function, options,
                               o->mode, announce_mkdir,
                               o->mode_bits, (uid_t) -1, (gid_t) -1, true)
             ? EXIT_SUCCESS
             : EXIT_FAILURE);

  /* FIXME: Due to the current structure of make_dir_parents()
     we don't have the facility to call defaultcon() before the
     final component of DIR is created.  So for now, create the
     final component with the context from previous component
     and here we set the context for the final component. */
  නම් (ret == EXIT_SUCCESS && o->set_security_context
      && o->make_ancestor_function)
    {
      නම් (! restorecon (o->set_security_context, last_component (dir), false)
          && ! ignorable_ctx_err (errno))
        error (0, errno, _("failed to restore context for %s"),
               quoteaf (dir));
    }

  ආපසු ret;
}

පූර්ණ_සංඛ්‍යා
main (පූර්ණ_සංඛ්‍යා argc, var_na **argv)
{
  var_na const *specified_mode = NULL;
  පූර්ණ_සංඛ්‍යා optc;
  var_na const *scontext = NULL;
  sa_nracanaa mkdir_options options;

  options.make_ancestor_function = NULL;
  options.mode = S_IRWXUGO;
  options.mode_bits = 0;
  options.created_directory_format = NULL;
  options.set_security_context = NULL;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  අතරතුර ((optc = getopt_long (argc, argv, "pm:vZ", longopts, NULL)) != -1)
    {
      switch (optc)
        {
        pak_sa 'p':
          options.make_ancestor_function = make_ancestor;
          bha_nga;
        pak_sa 'm':
          specified_mode = optarg;
          bha_nga;
        pak_sa 'v': /* --verbose  */
          options.created_directory_format = _("created directory %s");
          bha_nga;
        pak_sa 'Z':
          නම් (is_smack_enabled ())
            {
              /* We don't yet support -Z to restore context with SMACK.  */
              scontext = optarg;
            }
          නැතහොත් නම් (is_selinux_enabled () > 0)
            {
              නම් (optarg)
                scontext = optarg;
              නැතහොත්
                {
                  options.set_security_context = selabel_open (SELABEL_CTX_FILE,
                                                               NULL, 0);
                  නම් (! options.set_security_context)
                    error (0, errno, _("warning: ignoring --context"));
                }
            }
          නැතහොත් නම් (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux/SMACK-enabled kernel"));
            }
          bha_nga;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        default:
          usage (EXIT_FAILURE);
        }
    }

  නම් (optind == argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  /* FIXME: This assumes mkdir() is done in the same process.
     If that's not always the case we would need to call this
     like we do when options.set_security_context.  */
  නම් (scontext)
    {
      පූර්ණ_සංඛ්‍යා ret = 0;
      නම් (is_smack_enabled ())
        ret = smack_set_label_for_self (scontext);
      නැතහොත්
        ret = setfscreatecon (scontext);

      නම් (ret < 0)
        error (EXIT_FAILURE, errno,
               _("failed to set default file creation context to %s"),
               quote (scontext));
    }


  නම් (options.make_ancestor_function || specified_mode)
    {
      mode_t umask_value = umask (0);
      options.umask_ancestor = umask_value & ~(S_IWUSR | S_IXUSR);

      නම් (specified_mode)
        {
          sa_nracanaa mode_change *change = mode_compile (specified_mode);
          නම් (!change)
            error (EXIT_FAILURE, 0, _("invalid mode %s"),
                   quote (specified_mode));
          options.mode = mode_adjust (S_IRWXUGO, true, umask_value, change,
                                      &options.mode_bits);
          options.umask_self = umask_value & ~options.mode;
          free (change);
        }
      නැතහොත්
        {
          options.mode = S_IRWXUGO;
          options.umask_self = umask_value;
        }

      umask (options.umask_self);
    }

  ආපසු savewd_process_files (argc - optind, argv + optind,
                               process_dir, &options);
}
