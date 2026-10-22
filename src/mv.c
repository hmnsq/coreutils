/* mv -- move or rename files
   Copyright (C) 1986-2026 Free Software Foundation, Inc.

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

/* Written by Mike Parker, David MacKenzie, and Jim Meyering */

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <getopt.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <selinux/label.h>

#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "argmatch.h"
#അന്തര്ഭാവ "assure.h"
#അന്തര്ഭാവ "backupfile.h"
#അന്തര്ഭാവ "copy.h"
#അന്തര്ഭാവ "cp-hash.h"
#അന്തര്ഭാവ "filenamecat.h"
#അന്തര്ഭാവ "remove.h"
#അന്തര്ഭാവ "renameatu.h"
#അന്തര്ഭാവ "root-dev-ino.h"
#അന്തര്ഭാവ "targetdir.h"
#അന്തര്ഭാവ "priv-set.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "mv"

#define AUTHORS \
  proper_name ("Mike Parker"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
എണ്ണൽ
{
  DEBUG_OPTION = CHAR_MAX + 1,
  EXCHANGE_OPTION,
  NO_COPY_OPTION,
  STRIP_TRAILING_SLASHES_OPTION
};

സ്ഥിതം അക്ഷരം സ്ഥിരം *സ്ഥിരം update_type_string[] =
{
  "all", "none", "none-fail", "older", ശൂന്യ
};
സ്ഥിതം എണ്ണൽ Update_type സ്ഥിരം update_type[] =
{
  UPDATE_ALL, UPDATE_NONE, UPDATE_NONE_FAIL, UPDATE_OLDER,
};
ARGMATCH_VERIFY (update_type_string, update_type);

സ്ഥിതം ഘടന option സ്ഥിരം long_options[] =
{
  {"backup", optional_argument, ശൂന്യ, 'b'},
  {"context", no_argument, ശൂന്യ, 'Z'},
  {"debug", no_argument, ശൂന്യ, DEBUG_OPTION},
  {"exchange", no_argument, ശൂന്യ, EXCHANGE_OPTION},
  {"force", no_argument, ശൂന്യ, 'f'},
  {"interactive", no_argument, ശൂന്യ, 'i'},
  {"no-clobber", no_argument, ശൂന്യ, 'n'},   /* Deprecated.  */
  {"no-copy", no_argument, ശൂന്യ, NO_COPY_OPTION},
  {"no-target-directory", no_argument, ശൂന്യ, 'T'},
  {"strip-trailing-slashes", no_argument, ശൂന്യ,
   STRIP_TRAILING_SLASHES_OPTION},
  {"suffix", required_argument, ശൂന്യ, 'S'},
  {"target-directory", required_argument, ശൂന്യ, 't'},
  {"update", optional_argument, ശൂന്യ, 'u'},
  {"verbose", no_argument, ശൂന്യ, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ശൂന്യ, 0, ശൂന്യ, 0}
};

സ്ഥിതം ശൂന്യം
rm_option_init (ഘടന rm_options *x)
{
  x->ignore_missing_files = false;
  x->remove_empty_directories = true;
  x->recursive = true;
  x->one_file_system = false;

  /* Should we prompt for removal, too?  No.  Prompting for the 'move'
     part is enough.  It implies removal.  */
  x->interactive = RMI_NEVER;
  x->stdin_tty = false;

  x->verbose = false;

  /* Since this program may well have to process additional command
     line arguments after any call to 'rm', that function must preserve
     the initial working directory, in case one of those is a
     '.'-relative name.  */
  x->require_restore_cwd = true;

  {
    സ്ഥിതം ഘടന dev_ino dev_ino_buf;
    x->വേര്_dev_ino = get_വേര്_dev_ino (&dev_ino_buf);
    എങ്കിൽ (x->വേര്_dev_ino == ശൂന്യ)
      error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
             quoteaf ("/"));
  }

  x->preserve_all_വേര് = false;
}

സ്ഥിതം ശൂന്യം
cp_option_init (ഘടന cp_options *x)
{
  bool selinux_enabled = (0 < is_selinux_enabled ());

  cp_options_default (x);
  x->copy_as_regular = false;  /* FIXME: maybe make this an option */
  x->reflink_mode = REFLINK_AUTO;
  x->dereference = DEREF_NEVER;
  x->unlink_dest_before_opening = false;
  x->unlink_dest_after_failed_open = false;
  x->hard_link = false;
  x->interactive = I_UNSPECIFIED;
  x->move_mode = true;
  x->install_mode = false;
  x->one_file_system = false;
  x->preserve_ownership = true;
  x->preserve_links = true;
  x->preserve_mode = true;
  x->preserve_timestamps = true;
  x->explicit_no_preserve_mode= false;
  x->preserve_security_context = selinux_enabled;
  x->set_security_context = ശൂന്യ;
  x->reduce_diagnostics = false;
  x->data_copy_required = true;
  x->require_preserve = false;  /* FIXME: maybe make this an option */
  x->require_preserve_context = false;
  x->preserve_xattr = true;
  x->require_preserve_xattr = true;
  x->recursive = true;
  x->sparse_mode = SPARSE_AUTO;  /* FIXME: maybe make this an option */
  x->symbolic_link = false;
  x->set_mode = false;
  x->mode = 0;
  x->stdin_tty = isatty (STDIN_FILENO);

  x->open_dangling_dest_symlink = false;
  x->update = UPDATE_ALL;
  x->verbose = false;
  x->dest_info = ശൂന്യ;
  x->src_info = ശൂന്യ;
}

/* Move SOURCE onto DEST aka DEST_DIRFD+DEST_RELNAME.
   Handle cross-file-system moves.
   If SOURCE is a directory, DEST must not exist.
   Return true if successful.  */

സ്ഥിതം bool
do_move (അക്ഷരം സ്ഥിരം *source, അക്ഷരം സ്ഥിരം *dest,
         പൂർണ്ണസംഖ്യ dest_dirfd, അക്ഷരം സ്ഥിരം *dest_relname, സ്ഥിരം ഘടന cp_options *x)
{
  bool copy_into_self;
  bool rename_succeeded;
  bool ok = copy (source, dest, dest_dirfd, dest_relname, 0, x,
                  &copy_into_self, &rename_succeeded);

  എങ്കിൽ (ok)
    {
      അക്ഷരം സ്ഥിരം *dir_to_remove;
      എങ്കിൽ (copy_into_self)
        {
          /* In general, when copy returns with copy_into_self set, SOURCE is
             the same as, or a parent of DEST.  In this case we know it's a
             parent.  It doesn't make sense to move a directory into itself, and
             besides in some situations doing so would give highly unintuitive
             results.  Run this 'mkdir b; touch a c; mv * b' in an empty
             directory.  Here's the result of running echo $(find b -print):
             b b/a b/b b/b/a b/c.  Notice that only file 'a' was copied
             into b/b.  Handle this by giving a diagnostic, removing the
             copied-into-self directory, DEST ('b/b' in the example),
             and failing.  */

          dir_to_remove = ശൂന്യ;
          ok = false;
        }
      അല്ലെങ്കിൽ എങ്കിൽ (rename_succeeded)
        {
          /* No need to remove anything.  SOURCE was successfully
             renamed to DEST.  Or the user declined to rename a file.  */
          dir_to_remove = ശൂന്യ;
        }
      അല്ലെങ്കിൽ
        {
          /* This may mean SOURCE and DEST referred to different devices.
             It may also conceivably mean that even though they referred
             to the same device, rename wasn't implemented for that device.

             E.g., (from Joel N. Weber),
             [...] there might someday be cases where you can't rename
             but you can copy where the device name is the same, especially
             on Hurd.  Consider an ftpfs with a primitive ftp server that
             supports uploading, downloading and deleting, but not renaming.

             Also, note that comparing device numbers is not a reliable
             check for 'can-rename'.  Some systems can be set up so that
             files from many different physical devices all have the same
             st_dev field.  This is a feature of some NFS mounting
             configurations.

             We reach this point if SOURCE has been successfully copied
             to DEST.  Now we have to remove SOURCE.

             This function used to resort to copying only when rename
             failed and set errno to EXDEV.  */

          dir_to_remove = source;
        }

      എങ്കിൽ (dir_to_remove != ശൂന്യ)
        {
          ഘടന rm_options rm_options;
          rm_option_init (&rm_options);
          rm_options.verbose = x->verbose;
          അക്ഷരം സ്ഥിരം *dir[2] = { dir_to_remove, ശൂന്യ };

          എണ്ണൽ RM_status status = rm ((ശൂന്യം *) dir, &rm_options);
          affirm (VALID_STATUS (status));
          എങ്കിൽ (status == RM_ERROR)
            ok = false;
        }
    }

  തിരികെ ok;
}

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
    {
      printf (_("\
Usage: %s [OPTION]... [-T] SOURCE DEST\n\
  or:  %s [OPTION]... SOURCE... DIRECTORY\n\
  or:  %s [OPTION]... -t DIRECTORY SOURCE...\n\
"),
              program_name, program_name, program_name);
      fputs (_("\
Rename SOURCE to DEST, or move SOURCE(s) to DIRECTORY.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
      --backup[=CONTROL]\n\
         make a backup of each existing destination file\n\
"));
      oputs (_("\
  -b\n\
         like --backup but does not accept an argument\n\
"));
      oputs (_("\
      --debug\n\
         explain how a file is copied.  Implies -v\n\
"));
      oputs (_("\
      --exchange\n\
         exchange source and destination\n\
"));
      oputs (_("\
  -f, --force\n\
         ചെയ്യുക not prompt before overwriting\n\
"));
      oputs (_("\
  -i, --interactive\n\
         prompt before overwrite\n\
"));
      oputs (_("\
  -n, --no-clobber\n\
         (deprecated) silently skip existing files.  See also --update\n\
"));
      fputs (_("\
If you specify more than one of -i, -f, -n, only the final one takes effect.\n\
"), stdout);
      oputs (_("\
      --no-copy\n\
         ചെയ്യുക not copy എങ്കിൽ renaming fails\n\
"));
      oputs (_("\
      --strip-trailing-slashes\n\
         remove any trailing slashes from each SOURCE argument\n\
"));
      oputs (_("\
  -S, --suffix=SUFFIX\n\
         override the usual backup suffix\n\
"));
      oputs (_("\
  -t, --target-directory=DIRECTORY\n\
         move all SOURCE arguments into DIRECTORY\n\
"));
      oputs (_("\
  -T, --no-target-directory\n\
         treat DEST as a normal file\n\
"));
      oputs (_("\
      --update[=UPDATE]\n\
         control which existing files are updated;\n\
         UPDATE={all,none,none-fail,older(സ്ഥിരസ്ഥിതി)}\n\
"));
      oputs (_("\
  -u\n\
         equivalent to --update[=older].  See below\n\
"));
      oputs (_("\
  -v, --verbose\n\
         explain what is being done\n\
"));
      oputs (_("\
  -Z, --context\n\
         set SELinux security context of destination file to സ്ഥിരസ്ഥിതി type\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_update_parameters_note ();
      emit_backup_suffix_note ();
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  bool make_backups = false;
  അക്ഷരം സ്ഥിരം *backup_suffix = ശൂന്യ;
  അക്ഷരം *version_control_string = ശൂന്യ;
  ഘടന cp_options x;
  bool remove_trailing_slashes = false;
  അക്ഷരം സ്ഥിരം *target_directory = ശൂന്യ;
  bool no_target_directory = false;
  bool selinux_enabled = (0 < is_selinux_enabled ());

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdin);

  cp_option_init (&x);

  /* Try to disable the ability to unlink a directory.  */
  priv_set_remove_linkdir ();

  പൂർണ്ണസംഖ്യ c;
  ആകുമ്പോൾ ((c = getopt_long (argc, argv, "bfint:uvS:TZ", long_options, ശൂന്യ))
         != -1)
    {
      തിരഞ്ഞെടുപ്പ് (c)
        {
        അവസ്ഥ 'b':
          make_backups = true;
          എങ്കിൽ (optarg)
            version_control_string = optarg;
          നിർത്തുക;
        അവസ്ഥ 'f':
          x.interactive = I_ALWAYS_YES;
          നിർത്തുക;
        അവസ്ഥ 'i':
          x.interactive = I_ASK_USER;
          നിർത്തുക;
        അവസ്ഥ 'n':
          x.interactive = I_ALWAYS_SKIP;
          നിർത്തുക;
        അവസ്ഥ DEBUG_OPTION:
          x.debug = x.verbose = true;
          നിർത്തുക;
        അവസ്ഥ EXCHANGE_OPTION:
          x.exchange = true;
          നിർത്തുക;
        അവസ്ഥ NO_COPY_OPTION:
          x.no_copy = true;
          നിർത്തുക;
        അവസ്ഥ STRIP_TRAILING_SLASHES_OPTION:
          remove_trailing_slashes = true;
          നിർത്തുക;
        അവസ്ഥ 't':
          എങ്കിൽ (target_directory)
            error (EXIT_FAILURE, 0, _("multiple target directories specified"));
          target_directory = optarg;
          നിർത്തുക;
        അവസ്ഥ 'T':
          no_target_directory = true;
          നിർത്തുക;
        അവസ്ഥ 'u':
          x.update = UPDATE_OLDER;
          എങ്കിൽ (optarg)
            x.update = XARGMATCH ("--update", optarg,
                                  update_type_string, update_type);
          നിർത്തുക;
        അവസ്ഥ 'v':
          x.verbose = true;
          നിർത്തുക;
        അവസ്ഥ 'S':
          make_backups = true;
          backup_suffix = optarg;
          നിർത്തുക;
        അവസ്ഥ 'Z':
          /* As a performance enhancement, don't even bother trying
             to "restorecon" when not on an selinux-enabled kernel.  */
          എങ്കിൽ (selinux_enabled)
            {
              x.preserve_security_context = false;
              x.set_security_context = selabel_open (SELABEL_CTX_FILE,
                                                     ശൂന്യ, 0);
              എങ്കിൽ (! x.set_security_context)
                error (0, errno, _("warning: ignoring --context"));
            }
          നിർത്തുക;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        സ്ഥിരസ്ഥിതി:
          usage (EXIT_FAILURE);
        }
    }

  പൂർണ്ണസംഖ്യ n_files = argc - optind;
  അക്ഷരം **file = argv + optind;

  എങ്കിൽ (n_files <= !target_directory)
    {
      എങ്കിൽ (n_files <= 0)
        error (0, 0, _("missing file operand"));
      അല്ലെങ്കിൽ
        error (0, 0, _("missing destination file operand after %s"),
               quoteaf (file[0]));
      usage (EXIT_FAILURE);
    }

  ഘടന stat sb;
  sb.st_mode = 0;
  പൂർണ്ണസംഖ്യ target_dirfd = AT_FDCWD;
  എങ്കിൽ (no_target_directory)
    {
      എങ്കിൽ (target_directory)
        error (EXIT_FAILURE, 0,
               _("cannot combine --target-directory (-t) "
                 "and --no-target-directory (-T)"));
      എങ്കിൽ (2 < n_files)
        {
          error (0, 0, _("extra operand %s"), quoteaf (file[2]));
          usage (EXIT_FAILURE);
        }
    }
  അല്ലെങ്കിൽ എങ്കിൽ (target_directory)
    {
      target_dirfd = target_directory_operand (target_directory, &sb);
      എങ്കിൽ (! target_dirfd_valid (target_dirfd))
        error (EXIT_FAILURE, errno, _("target directory %s"),
               quoteaf (target_directory));
    }
  അല്ലെങ്കിൽ
    {
      അക്ഷരം സ്ഥിരം *lastfile = file[n_files - 1];
      എങ്കിൽ (n_files == 2 && !x.exchange)
        x.rename_errno = (renameatu (AT_FDCWD, file[0], AT_FDCWD, lastfile,
                                     RENAME_NOREPLACE)
                          ? errno : 0);
      എങ്കിൽ (x.rename_errno != 0)
        {
          പൂർണ്ണസംഖ്യ fd = target_directory_operand (lastfile, &sb);
          എങ്കിൽ (target_dirfd_valid (fd))
            {
              x.rename_errno = -1;
              target_dirfd = fd;
              target_directory = lastfile;
              n_files--;
            }
          അല്ലെങ്കിൽ
            {
              /* The last operand LASTFILE cannot be opened as a directory.
                 If there are more than two operands, report an error.

                 Also, report an error if LASTFILE is known to be a directory
                 even though it could not be opened, which can happen if
                 opening failed with EACCES on a platform lacking O_PATH.
                 In this case use stat to test whether LASTFILE is a
                 directory, in case opening a non-directory with (O_SEARCH
                 | O_DIRECTORY) failed with EACCES not ENOTDIR.  */
              പൂർണ്ണസംഖ്യ err = errno;
              എങ്കിൽ (2 < n_files
                  || (O_PATHSEARCH == O_തിരയൽ && err == EACCES
                      && (sb.st_mode != 0 || stat (lastfile, &sb) == 0)
                      && S_ISDIR (sb.st_mode)))
                error (EXIT_FAILURE, err, _("target %s"), quoteaf (lastfile));
            }
        }
    }

  /* Handle the ambiguity in the semantics of mv induced by the
     varying semantics of the rename function.  POSIX-compatible
     systems (e.g., GNU/Linux) have a rename function that honors a
     trailing slash in the source, while others (Solaris 9, FreeBSD
     7.2) have a rename function that ignores it.  */
  എങ്കിൽ (remove_trailing_slashes)
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < n_files; i++)
      strip_trailing_slashes (file[i]);

  എങ്കിൽ (x.interactive == I_ALWAYS_SKIP)
    x.update = UPDATE_NONE;

  എങ്കിൽ (make_backups
      && (x.exchange
          || x.update == UPDATE_NONE
          || x.update == UPDATE_NONE_FAIL))
    {
      error (0, 0,
             _("cannot combine --backup with "
               "--exchange, -n, or --update=none-fail"));
      usage (EXIT_FAILURE);
    }

  x.backup_type = (make_backups
                   ? xget_version (_("backup type"),
                                   version_control_string)
                   : no_backups);
  set_simple_backup_suffix (backup_suffix);

  hash_init ();

  bool ok;
  എങ്കിൽ (target_directory)
    {
      /* Initialize the hash table only if we'll need it.
         The problem it is used to detect can arise only if there are
         two or more files to move.  */
      എങ്കിൽ (2 <= n_files)
        dest_info_init (&x);

      ok = true;
      വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < n_files; ++i)
        {
          x.last_file = i + 1 == n_files;
          അക്ഷരം സ്ഥിരം *source = file[i];
          അക്ഷരം സ്ഥിരം *source_basename = last_component (source);
          അക്ഷരം *dest_relname;
          അക്ഷരം *dest = file_name_concat (target_directory, source_basename,
                                         &dest_relname);
          strip_trailing_slashes (dest_relname);
          ok &= do_move (source, dest, target_dirfd, dest_relname, &x);
          സ്വതന്ത്രം (dest);
        }
    }
  അല്ലെങ്കിൽ
    {
      x.last_file = true;
      ok = do_move (file[0], file[1], AT_FDCWD, file[1], &x);
    }

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
