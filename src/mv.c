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

#অন্তর্ভাৱ <config.h>
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <getopt.h>
#অন্তর্ভাৱ <sys/types.h>
#অন্তর্ভাৱ <selinux/label.h>

#অন্তর্ভাৱ "system.h"
#অন্তর্ভাৱ "argmatch.h"
#অন্তর্ভাৱ "assure.h"
#অন্তর্ভাৱ "backupfile.h"
#অন্তর্ভাৱ "copy.h"
#অন্তর্ভাৱ "cp-hash.h"
#অন্তর্ভাৱ "filenamecat.h"
#অন্তর্ভাৱ "remove.h"
#অন্তর্ভাৱ "renameatu.h"
#অন্তর্ভাৱ "root-dev-ino.h"
#অন্তর্ভাৱ "targetdir.h"
#অন্তর্ভাৱ "priv-set.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "mv"

#define AUTHORS \
  proper_name ("Mike Parker"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
গণনা
{
  DEBUG_OPTION = CHAR_MAX + 1,
  EXCHANGE_OPTION,
  NO_COPY_OPTION,
  STRIP_TRAILING_SLASHES_OPTION
};

স্থির ৱর্ণ ধ্রুৱ *ধ্রুৱ update_type_string[] =
{
  "all", "none", "none-fail", "older", রিক্ত
};
স্থির গণনা Update_type ধ্রুৱ update_type[] =
{
  UPDATE_ALL, UPDATE_NONE, UPDATE_NONE_FAIL, UPDATE_OLDER,
};
ARGMATCH_VERIFY (update_type_string, update_type);

স্থির সংরচনা option ধ্রুৱ long_options[] =
{
  {"backup", optional_argument, রিক্ত, 'b'},
  {"context", no_argument, রিক্ত, 'Z'},
  {"debug", no_argument, রিক্ত, DEBUG_OPTION},
  {"exchange", no_argument, রিক্ত, EXCHANGE_OPTION},
  {"force", no_argument, রিক্ত, 'f'},
  {"interactive", no_argument, রিক্ত, 'i'},
  {"no-clobber", no_argument, রিক্ত, 'n'},   /* Deprecated.  */
  {"no-copy", no_argument, রিক্ত, NO_COPY_OPTION},
  {"no-target-directory", no_argument, রিক্ত, 'T'},
  {"strip-trailing-slashes", no_argument, রিক্ত,
   STRIP_TRAILING_SLASHES_OPTION},
  {"suffix", required_argument, রিক্ত, 'S'},
  {"target-directory", required_argument, রিক্ত, 't'},
  {"update", optional_argument, রিক্ত, 'u'},
  {"verbose", no_argument, রিক্ত, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {রিক্ত, 0, রিক্ত, 0}
};

স্থির শূন্য
rm_option_init (সংরচনা rm_options *x)
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
    স্থির সংরচনা dev_ino dev_ino_buf;
    x->মূল_dev_ino = get_মূল_dev_ino (&dev_ino_buf);
    যদি (x->মূল_dev_ino == রিক্ত)
      error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
             quoteaf ("/"));
  }

  x->preserve_all_মূল = false;
}

স্থির শূন্য
cp_option_init (সংরচনা cp_options *x)
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
  x->set_security_context = রিক্ত;
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
  x->dest_info = রিক্ত;
  x->src_info = রিক্ত;
}

/* Move SOURCE onto DEST aka DEST_DIRFD+DEST_RELNAME.
   Handle cross-file-system moves.
   If SOURCE is a directory, DEST must not exist.
   Return true if successful.  */

স্থির bool
do_move (ৱর্ণ ধ্রুৱ *source, ৱর্ণ ধ্রুৱ *dest,
         পূর্ণাঙ্ক dest_dirfd, ৱর্ণ ধ্রুৱ *dest_relname, ধ্রুৱ সংরচনা cp_options *x)
{
  bool copy_into_self;
  bool rename_succeeded;
  bool ok = copy (source, dest, dest_dirfd, dest_relname, 0, x,
                  &copy_into_self, &rename_succeeded);

  যদি (ok)
    {
      ৱর্ণ ধ্রুৱ *dir_to_remove;
      যদি (copy_into_self)
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

          dir_to_remove = রিক্ত;
          ok = false;
        }
      অন্যথা যদি (rename_succeeded)
        {
          /* No need to remove anything.  SOURCE was successfully
             renamed to DEST.  Or the user declined to rename a file.  */
          dir_to_remove = রিক্ত;
        }
      অন্যথা
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

      যদি (dir_to_remove != রিক্ত)
        {
          সংরচনা rm_options rm_options;
          rm_option_init (&rm_options);
          rm_options.verbose = x->verbose;
          ৱর্ণ ধ্রুৱ *dir[2] = { dir_to_remove, রিক্ত };

          গণনা RM_status status = rm ((শূন্য *) dir, &rm_options);
          affirm (VALID_STATUS (status));
          যদি (status == RM_ERROR)
            ok = false;
        }
    }

  ঘূৰাই ok;
}

শূন্য
usage (পূর্ণাঙ্ক status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
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
         কৰা not prompt before overwriting\n\
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
         কৰা not copy যদি renaming fails\n\
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
         UPDATE={all,none,none-fail,older(স্ৱভাৱ)}\n\
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
         set SELinux security context of destination file to স্ৱভাৱ type\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_update_parameters_note ();
      emit_backup_suffix_note ();
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

পূর্ণাঙ্ক
main (পূর্ণাঙ্ক argc, ৱর্ণ **argv)
{
  bool make_backups = false;
  ৱর্ণ ধ্রুৱ *backup_suffix = রিক্ত;
  ৱর্ণ *version_control_string = রিক্ত;
  সংরচনা cp_options x;
  bool remove_trailing_slashes = false;
  ৱর্ণ ধ্রুৱ *target_directory = রিক্ত;
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

  পূর্ণাঙ্ক c;
  যেতিয়ালৈ ((c = getopt_long (argc, argv, "bfint:uvS:TZ", long_options, রিক্ত))
         != -1)
    {
      ৱিকল্প (c)
        {
        পক্ষ 'b':
          make_backups = true;
          যদি (optarg)
            version_control_string = optarg;
          ভঙ্গ;
        পক্ষ 'f':
          x.interactive = I_ALWAYS_YES;
          ভঙ্গ;
        পক্ষ 'i':
          x.interactive = I_ASK_USER;
          ভঙ্গ;
        পক্ষ 'n':
          x.interactive = I_ALWAYS_SKIP;
          ভঙ্গ;
        পক্ষ DEBUG_OPTION:
          x.debug = x.verbose = true;
          ভঙ্গ;
        পক্ষ EXCHANGE_OPTION:
          x.exchange = true;
          ভঙ্গ;
        পক্ষ NO_COPY_OPTION:
          x.no_copy = true;
          ভঙ্গ;
        পক্ষ STRIP_TRAILING_SLASHES_OPTION:
          remove_trailing_slashes = true;
          ভঙ্গ;
        পক্ষ 't':
          যদি (target_directory)
            error (EXIT_FAILURE, 0, _("multiple target directories specified"));
          target_directory = optarg;
          ভঙ্গ;
        পক্ষ 'T':
          no_target_directory = true;
          ভঙ্গ;
        পক্ষ 'u':
          x.update = UPDATE_OLDER;
          যদি (optarg)
            x.update = XARGMATCH ("--update", optarg,
                                  update_type_string, update_type);
          ভঙ্গ;
        পক্ষ 'v':
          x.verbose = true;
          ভঙ্গ;
        পক্ষ 'S':
          make_backups = true;
          backup_suffix = optarg;
          ভঙ্গ;
        পক্ষ 'Z':
          /* As a performance enhancement, don't even bother trying
             to "restorecon" when not on an selinux-enabled kernel.  */
          যদি (selinux_enabled)
            {
              x.preserve_security_context = false;
              x.set_security_context = selabel_open (SELABEL_CTX_FILE,
                                                     রিক্ত, 0);
              যদি (! x.set_security_context)
                error (0, errno, _("warning: ignoring --context"));
            }
          ভঙ্গ;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        স্ৱভাৱ:
          usage (EXIT_FAILURE);
        }
    }

  পূর্ণাঙ্ক n_files = argc - optind;
  ৱর্ণ **file = argv + optind;

  যদি (n_files <= !target_directory)
    {
      যদি (n_files <= 0)
        error (0, 0, _("missing file operand"));
      অন্যথা
        error (0, 0, _("missing destination file operand after %s"),
               quoteaf (file[0]));
      usage (EXIT_FAILURE);
    }

  সংরচনা stat sb;
  sb.st_mode = 0;
  পূর্ণাঙ্ক target_dirfd = AT_FDCWD;
  যদি (no_target_directory)
    {
      যদি (target_directory)
        error (EXIT_FAILURE, 0,
               _("cannot combine --target-directory (-t) "
                 "and --no-target-directory (-T)"));
      যদি (2 < n_files)
        {
          error (0, 0, _("extra operand %s"), quoteaf (file[2]));
          usage (EXIT_FAILURE);
        }
    }
  অন্যথা যদি (target_directory)
    {
      target_dirfd = target_directory_operand (target_directory, &sb);
      যদি (! target_dirfd_valid (target_dirfd))
        error (EXIT_FAILURE, errno, _("target directory %s"),
               quoteaf (target_directory));
    }
  অন্যথা
    {
      ৱর্ণ ধ্রুৱ *lastfile = file[n_files - 1];
      যদি (n_files == 2 && !x.exchange)
        x.rename_errno = (renameatu (AT_FDCWD, file[0], AT_FDCWD, lastfile,
                                     RENAME_NOREPLACE)
                          ? errno : 0);
      যদি (x.rename_errno != 0)
        {
          পূর্ণাঙ্ক fd = target_directory_operand (lastfile, &sb);
          যদি (target_dirfd_valid (fd))
            {
              x.rename_errno = -1;
              target_dirfd = fd;
              target_directory = lastfile;
              n_files--;
            }
          অন্যথা
            {
              /* The last operand LASTFILE cannot be opened as a directory.
                 If there are more than two operands, report an error.

                 Also, report an error if LASTFILE is known to be a directory
                 even though it could not be opened, which can happen if
                 opening failed with EACCES on a platform lacking O_PATH.
                 In this case use stat to test whether LASTFILE is a
                 directory, in case opening a non-directory with (O_SEARCH
                 | O_DIRECTORY) failed with EACCES not ENOTDIR.  */
              পূর্ণাঙ্ক err = errno;
              যদি (2 < n_files
                  || (O_PATHSEARCH == O_অনুসন্ধান && err == EACCES
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
  যদি (remove_trailing_slashes)
    বাবে (পূর্ণাঙ্ক i = 0; i < n_files; i++)
      strip_trailing_slashes (file[i]);

  যদি (x.interactive == I_ALWAYS_SKIP)
    x.update = UPDATE_NONE;

  যদি (make_backups
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
  যদি (target_directory)
    {
      /* Initialize the hash table only if we'll need it.
         The problem it is used to detect can arise only if there are
         two or more files to move.  */
      যদি (2 <= n_files)
        dest_info_init (&x);

      ok = true;
      বাবে (পূর্ণাঙ্ক i = 0; i < n_files; ++i)
        {
          x.last_file = i + 1 == n_files;
          ৱর্ণ ধ্রুৱ *source = file[i];
          ৱর্ণ ধ্রুৱ *source_basename = last_component (source);
          ৱর্ণ *dest_relname;
          ৱর্ণ *dest = file_name_concat (target_directory, source_basename,
                                         &dest_relname);
          strip_trailing_slashes (dest_relname);
          ok &= do_move (source, dest, target_dirfd, dest_relname, &x);
          মুক্ত (dest);
        }
    }
  অন্যথা
    {
      x.last_file = true;
      ok = do_move (file[0], file[1], AT_FDCWD, file[1], &x);
    }

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
