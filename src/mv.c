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

#ضمن <config.h>
#ضمن <stdio.h>
#ضمن <getopt.h>
#ضمن <sys/types.h>
#ضمن <selinux/label.h>

#ضمن "system.h"
#ضمن "argmatch.h"
#ضمن "assure.h"
#ضمن "backupfile.h"
#ضمن "copy.h"
#ضمن "cp-hash.h"
#ضمن "filenamecat.h"
#ضمن "remove.h"
#ضمن "renameatu.h"
#ضمن "root-dev-ino.h"
#ضمن "targetdir.h"
#ضمن "priv-set.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "mv"

#define AUTHORS \
  proper_name ("Mike Parker"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
تعداد
{
  DEBUG_OPTION = CHAR_MAX + 1,
  EXCHANGE_OPTION,
  NO_COPY_OPTION,
  STRIP_TRAILING_SLASHES_OPTION
};

ساكن حرف ثابت *ثابت update_type_string[] =
{
  "all", "none", "none-fail", "older", عدم
};
ساكن تعداد Update_type ثابت update_type[] =
{
  UPDATE_ALL, UPDATE_NONE, UPDATE_NONE_FAIL, UPDATE_OLDER,
};
ARGMATCH_VERIFY (update_type_string, update_type);

ساكن بنية option ثابت long_options[] =
{
  {"backup", optional_argument, عدم, 'b'},
  {"context", no_argument, عدم, 'Z'},
  {"debug", no_argument, عدم, DEBUG_OPTION},
  {"exchange", no_argument, عدم, EXCHANGE_OPTION},
  {"force", no_argument, عدم, 'f'},
  {"interactive", no_argument, عدم, 'i'},
  {"no-clobber", no_argument, عدم, 'n'},   /* Deprecated.  */
  {"no-copy", no_argument, عدم, NO_COPY_OPTION},
  {"no-target-directory", no_argument, عدم, 'T'},
  {"strip-trailing-slashes", no_argument, عدم,
   STRIP_TRAILING_SLASHES_OPTION},
  {"suffix", required_argument, عدم, 'S'},
  {"target-directory", required_argument, عدم, 't'},
  {"update", optional_argument, عدم, 'u'},
  {"verbose", no_argument, عدم, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

ساكن فراغ
rm_option_init (بنية rm_options *x)
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
    ساكن بنية dev_ino dev_ino_buf;
    x->root_dev_ino = get_root_dev_ino (&dev_ino_buf);
    אם (x->root_dev_ino == عدم)
      error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
             quoteaf ("/"));
  }

  x->preserve_all_root = false;
}

ساكن فراغ
cp_option_init (بنية cp_options *x)
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
  x->set_security_context = عدم;
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
  x->dest_info = عدم;
  x->src_info = عدم;
}

/* Move SOURCE onto DEST aka DEST_DIRFD+DEST_RELNAME.
   Handle cross-file-system moves.
   If SOURCE is a directory, DEST must not exist.
   Return true if successful.  */

ساكن bool
do_move (حرف ثابت *source, حرف ثابت *dest,
         صحيح dest_dirfd, حرف ثابت *dest_relname, ثابت بنية cp_options *x)
{
  bool copy_into_self;
  bool rename_succeeded;
  bool ok = copy (source, dest, dest_dirfd, dest_relname, 0, x,
                  &copy_into_self, &rename_succeeded);

  אם (ok)
    {
      حرف ثابت *dir_to_remove;
      אם (copy_into_self)
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

          dir_to_remove = عدم;
          ok = false;
        }
      אחרת אם (rename_succeeded)
        {
          /* No need to remove anything.  SOURCE was successfully
             renamed to DEST.  Or the user declined to rename a file.  */
          dir_to_remove = عدم;
        }
      אחרת
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

      אם (dir_to_remove != عدم)
        {
          بنية rm_options rm_options;
          rm_option_init (&rm_options);
          rm_options.verbose = x->verbose;
          حرف ثابت *dir[2] = { dir_to_remove, عدم };

          تعداد RM_status status = rm ((فراغ *) dir, &rm_options);
          affirm (VALID_STATUS (status));
          אם (status == RM_ERROR)
            ok = false;
        }
    }

  החזר ok;
}

فراغ
usage (صحيح status)
{
  אם (status != EXIT_SUCCESS)
    emit_try_help ();
  אחרת
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
         עשה not prompt before overwriting\n\
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
         עשה not copy אם renaming fails\n\
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
         UPDATE={all,none,none-fail,older(افتراضي)}\n\
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
         set SELinux security context of destination file to افتراضي type\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_update_parameters_note ();
      emit_backup_suffix_note ();
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

صحيح
main (صحيح argc, حرف **argv)
{
  bool make_backups = false;
  حرف ثابت *backup_suffix = عدم;
  حرف *version_control_string = عدم;
  بنية cp_options x;
  bool remove_trailing_slashes = false;
  حرف ثابت *target_directory = عدم;
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

  صحيح c;
  כלעוד ((c = getopt_long (argc, argv, "bfint:uvS:TZ", long_options, عدم))
         != -1)
    {
      بدل (c)
        {
        حالة 'b':
          make_backups = true;
          אם (optarg)
            version_control_string = optarg;
          اكسر;
        حالة 'f':
          x.interactive = I_ALWAYS_YES;
          اكسر;
        حالة 'i':
          x.interactive = I_ASK_USER;
          اكسر;
        حالة 'n':
          x.interactive = I_ALWAYS_SKIP;
          اكسر;
        حالة DEBUG_OPTION:
          x.debug = x.verbose = true;
          اكسر;
        حالة EXCHANGE_OPTION:
          x.exchange = true;
          اكسر;
        حالة NO_COPY_OPTION:
          x.no_copy = true;
          اكسر;
        حالة STRIP_TRAILING_SLASHES_OPTION:
          remove_trailing_slashes = true;
          اكسر;
        حالة 't':
          אם (target_directory)
            error (EXIT_FAILURE, 0, _("multiple target directories specified"));
          target_directory = optarg;
          اكسر;
        حالة 'T':
          no_target_directory = true;
          اكسر;
        حالة 'u':
          x.update = UPDATE_OLDER;
          אם (optarg)
            x.update = XARGMATCH ("--update", optarg,
                                  update_type_string, update_type);
          اكسر;
        حالة 'v':
          x.verbose = true;
          اكسر;
        حالة 'S':
          make_backups = true;
          backup_suffix = optarg;
          اكسر;
        حالة 'Z':
          /* As a performance enhancement, don't even bother trying
             to "restorecon" when not on an selinux-enabled kernel.  */
          אם (selinux_enabled)
            {
              x.preserve_security_context = false;
              x.set_security_context = selabel_open (SELABEL_CTX_FILE,
                                                     عدم, 0);
              אם (! x.set_security_context)
                error (0, errno, _("warning: ignoring --context"));
            }
          اكسر;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  صحيح n_files = argc - optind;
  حرف **file = argv + optind;

  אם (n_files <= !target_directory)
    {
      אם (n_files <= 0)
        error (0, 0, _("missing file operand"));
      אחרת
        error (0, 0, _("missing destination file operand after %s"),
               quoteaf (file[0]));
      usage (EXIT_FAILURE);
    }

  بنية stat sb;
  sb.st_mode = 0;
  صحيح target_dirfd = AT_FDCWD;
  אם (no_target_directory)
    {
      אם (target_directory)
        error (EXIT_FAILURE, 0,
               _("cannot combine --target-directory (-t) "
                 "and --no-target-directory (-T)"));
      אם (2 < n_files)
        {
          error (0, 0, _("extra operand %s"), quoteaf (file[2]));
          usage (EXIT_FAILURE);
        }
    }
  אחרת אם (target_directory)
    {
      target_dirfd = target_directory_operand (target_directory, &sb);
      אם (! target_dirfd_valid (target_dirfd))
        error (EXIT_FAILURE, errno, _("target directory %s"),
               quoteaf (target_directory));
    }
  אחרת
    {
      حرف ثابت *lastfile = file[n_files - 1];
      אם (n_files == 2 && !x.exchange)
        x.rename_errno = (renameatu (AT_FDCWD, file[0], AT_FDCWD, lastfile,
                                     RENAME_NOREPLACE)
                          ? errno : 0);
      אם (x.rename_errno != 0)
        {
          صحيح fd = target_directory_operand (lastfile, &sb);
          אם (target_dirfd_valid (fd))
            {
              x.rename_errno = -1;
              target_dirfd = fd;
              target_directory = lastfile;
              n_files--;
            }
          אחרת
            {
              /* The last operand LASTFILE cannot be opened as a directory.
                 If there are more than two operands, report an error.

                 Also, report an error if LASTFILE is known to be a directory
                 even though it could not be opened, which can happen if
                 opening failed with EACCES on a platform lacking O_PATH.
                 In this case use stat to test whether LASTFILE is a
                 directory, in case opening a non-directory with (O_SEARCH
                 | O_DIRECTORY) failed with EACCES not ENOTDIR.  */
              صحيح err = errno;
              אם (2 < n_files
                  || (O_PATHSEARCH == O_SEARCH && err == EACCES
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
  אם (remove_trailing_slashes)
    עבור (صحيح i = 0; i < n_files; i++)
      strip_trailing_slashes (file[i]);

  אם (x.interactive == I_ALWAYS_SKIP)
    x.update = UPDATE_NONE;

  אם (make_backups
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
  אם (target_directory)
    {
      /* Initialize the hash table only if we'll need it.
         The problem it is used to detect can arise only if there are
         two or more files to move.  */
      אם (2 <= n_files)
        dest_info_init (&x);

      ok = true;
      עבור (صحيح i = 0; i < n_files; ++i)
        {
          x.last_file = i + 1 == n_files;
          حرف ثابت *source = file[i];
          حرف ثابت *source_basename = last_component (source);
          حرف *dest_relname;
          حرف *dest = file_name_concat (target_directory, source_basename,
                                         &dest_relname);
          strip_trailing_slashes (dest_relname);
          ok &= do_move (source, dest, target_dirfd, dest_relname, &x);
          حرر (dest);
        }
    }
  אחרת
    {
      x.last_file = true;
      ok = do_move (file[0], file[1], AT_FDCWD, file[1], &x);
    }

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
