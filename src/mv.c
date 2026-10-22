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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <selinux/label.h>

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "argmatch.h"
#ಅನ್ತರ್ಭಾವ "assure.h"
#ಅನ್ತರ್ಭಾವ "backupfile.h"
#ಅನ್ತರ್ಭಾವ "copy.h"
#ಅನ್ತರ್ಭಾವ "cp-hash.h"
#ಅನ್ತರ್ಭಾವ "filenamecat.h"
#ಅನ್ತರ್ಭಾವ "remove.h"
#ಅನ್ತರ್ಭಾವ "renameatu.h"
#ಅನ್ತರ್ಭಾವ "root-dev-ino.h"
#ಅನ್ತರ್ಭಾವ "targetdir.h"
#ಅನ್ತರ್ಭಾವ "priv-set.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "mv"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS \
  proper_name ("Mike Parker"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ಎಣಿಕೆ
{
  DEBUG_OPTION = CHAR_MAX + 1,
  EXCHANGE_OPTION,
  NO_COPY_OPTION,
  STRIP_TRAILING_SLASHES_OPTION
};

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸ್ಥಿರಾಂಕ update_type_string[] =
{
  "all", "none", "none-fail", "older", ಖಾಲಿ
};
ಸ್ಥಿರ ಎಣಿಕೆ Update_type ಸ್ಥಿರಾಂಕ update_type[] =
{
  UPDATE_ALL, UPDATE_NONE, UPDATE_NONE_FAIL, UPDATE_OLDER,
};
ARGMATCH_VERIFY (update_type_string, update_type);

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ long_options[] =
{
  {"backup", optional_ವಾದ, ಖಾಲಿ, 'b'},
  {"context", no_ವಾದ, ಖಾಲಿ, 'Z'},
  {"debug", no_ವಾದ, ಖಾಲಿ, DEBUG_OPTION},
  {"exchange", no_ವಾದ, ಖಾಲಿ, EXCHANGE_OPTION},
  {"force", no_ವಾದ, ಖಾಲಿ, 'f'},
  {"interactive", no_ವಾದ, ಖಾಲಿ, 'i'},
  {"no-clobber", no_ವಾದ, ಖಾಲಿ, 'n'},   /* Deprecated.  */
  {"no-copy", no_ವಾದ, ಖಾಲಿ, NO_COPY_OPTION},
  {"no-target-directory", no_ವಾದ, ಖಾಲಿ, 'T'},
  {"strip-trailing-slashes", no_ವಾದ, ಖಾಲಿ,
   STRIP_TRAILING_SLASHES_OPTION},
  {"suffix", required_ವಾದ, ಖಾಲಿ, 'S'},
  {"target-directory", required_ವಾದ, ಖಾಲಿ, 't'},
  {"update", optional_ವಾದ, ಖಾಲಿ, 'u'},
  {"verbose", no_ವಾದ, ಖಾಲಿ, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};

ಸ್ಥಿರ ಶೂನ್ಯ
rm_option_init (ರಚನೆ rm_options *x)
{
  x->ignore_missing_files = false;
  x->remove_ಖಾಲಿಗಳು_directories = true;
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
    ಸ್ಥಿರ ರಚನೆ dev_ino dev_ino_buf;
    x->ಬೇರು_dev_ino = get_ಬೇರು_dev_ino (&dev_ino_buf);
    ಆದರೆ (x->ಬೇರು_dev_ino == ಖಾಲಿ)
      error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
             quoteaf ("/"));
  }

  x->preserve_all_ಬೇರು = false;
}

ಸ್ಥಿರ ಶೂನ್ಯ
cp_option_init (ರಚನೆ cp_options *x)
{
  bool selinux_enabled = (0 < is_selinux_enabled ());

  cp_options_default (x);
  x->copy_as_regular = false;  /* FIXME: maybe make this an option */
  x->reflink_ಬಹುಲಕ = REFLINK_AUTO;
  x->dereference = DEREF_NEVER;
  x->unlink_dest_before_opening = false;
  x->unlink_dest_after_failed_open = false;
  x->hard_link = false;
  x->interactive = I_UNSPECIFIED;
  x->move_ಬಹುಲಕ = true;
  x->install_ಬಹುಲಕ = false;
  x->one_file_system = false;
  x->preserve_ownership = true;
  x->preserve_links = true;
  x->preserve_ಬಹುಲಕ = true;
  x->preserve_timestamps = true;
  x->explicit_no_preserve_ಬಹುಲಕ= false;
  x->preserve_security_context = selinux_enabled;
  x->ಗಣ_security_context = ಖಾಲಿ;
  x->reduce_diagnostics = false;
  x->ದತ್ತಾಂಶ_copy_required = true;
  x->require_preserve = false;  /* FIXME: maybe make this an option */
  x->require_preserve_context = false;
  x->preserve_xattr = true;
  x->require_preserve_xattr = true;
  x->recursive = true;
  x->sparse_ಬಹುಲಕ = SPARSE_AUTO;  /* FIXME: maybe make this an option */
  x->symbolic_link = false;
  x->ಗಣ_ಬಹುಲಕ = false;
  x->ಬಹುಲಕ_2 = 0;
  x->stdin_tty = isatty (STDIN_FILENO);

  x->open_dangling_dest_symlink = false;
  x->update = UPDATE_ALL;
  x->verbose = false;
  x->dest_info = ಖಾಲಿ;
  x->src_info = ಖಾಲಿ;
}

/* Move SOURCE onto DEST aka DEST_DIRFD+DEST_RELNAME.
   Handle cross-file-system moves.
   If SOURCE is a directory, DEST must not exist.
   Return true if successful.  */

ಸ್ಥಿರ bool
do_move (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಮೂಲ_2, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dest,
         ಪೂರ್ಣಾಂಕ dest_dirfd, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dest_relname, ಸ್ಥಿರಾಂಕ ರಚನೆ cp_options *x)
{
  bool copy_into_self;
  bool rename_succeeded;
  bool ok = copy (ಮೂಲ_2, dest, dest_dirfd, dest_relname, 0, x,
                  &copy_into_self, &rename_succeeded);

  ಆದರೆ (ok)
    {
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dir_ಗಮ್ಯ_remove;
      ಆದರೆ (copy_into_self)
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

          dir_ಗಮ್ಯ_remove = ಖಾಲಿ;
          ok = false;
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (rename_succeeded)
        {
          /* No need to remove anything.  SOURCE was successfully
             renamed to DEST.  Or the user declined to rename a file.  */
          dir_ಗಮ್ಯ_remove = ಖಾಲಿ;
        }
      ಇಲ್ಲದಿದ್ದರೆ
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

          dir_ಗಮ್ಯ_remove = ಮೂಲ_2;
        }

      ಆದರೆ (dir_ಗಮ್ಯ_remove != ಖಾಲಿ)
        {
          ರಚನೆ rm_options rm_options;
          rm_option_init (&rm_options);
          rm_options.verbose = x->verbose;
          ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dir[2] = { dir_ಗಮ್ಯ_remove, ಖಾಲಿ };

          ಎಣಿಕೆ RM_ಸ್ಥಿತಿ ಸ್ಥಿತಿ_2 = rm ((ಶೂನ್ಯ *) dir, &rm_options);
          affirm (VALID_ಸ್ಥಿತಿ (ಸ್ಥಿತಿ_2));
          ಆದರೆ (ಸ್ಥಿತಿ_2 == RM_ERROR)
            ok = false;
        }
    }

  ಹಿಂತಿರುಗಿ ok;
}

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("\
Usage: %s [OPTION]... [-T] ಮೂಲ DEST\n\
  or:  %s [OPTION]... ಮೂಲ... DIRECTORY\n\
  or:  %s [OPTION]... -t DIRECTORY ಮೂಲ...\n\
"),
              ಕಾರ್ಯಕ್ರಮ_name, ಕಾರ್ಯಕ್ರಮ_name, ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
Rename ಮೂಲ ಗಮ್ಯ DEST, or move ಮೂಲ(s) ಗಮ್ಯ DIRECTORY.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
      --backup[=CONTROL]\n\
         make a backup ದ each existing destination file\n\
"));
      oputs (_("\
  -b\n\
         like --backup but does not accept an ವಾದ\n\
"));
      oputs (_("\
      --debug\n\
         explain how a file is copied.  ಸೂಚಿಸುತ್ತದೆ_2 -v\n\
"));
      oputs (_("\
      --exchange\n\
         exchange ಮೂಲ_2 and destination\n\
"));
      oputs (_("\
  -f, --force\n\
         ಮಾಡು not prompt before overwriting\n\
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
If you specify ಇನ್ನಷ್ಟು than one ದ -i, -f, -n, only the final one takes effect.\n\
"), stdout);
      oputs (_("\
      --no-copy\n\
         ಮಾಡು not copy ಆದರೆ renaming fails\n\
"));
      oputs (_("\
      --strip-trailing-slashes\n\
         remove any trailing slashes from each ಮೂಲ ವಾದ\n\
"));
      oputs (_("\
  -S, --suffix=SUFFIX\n\
         override the usual backup suffix\n\
"));
      oputs (_("\
  -t, --ಗುರಿ-directory=DIRECTORY\n\
         move all ಮೂಲ arguments into DIRECTORY\n\
"));
      oputs (_("\
  -T, --no-ಗುರಿ-directory\n\
         treat DEST as a ಸಾಮಾನ್ಯ file\n\
"));
      oputs (_("\
      --update[=UPDATE]\n\
         control which existing files are updated;\n\
         UPDATE={all,none,none-fail,older(ಪೂರ್ವನಿಯೋಜಿತ)}\n\
"));
      oputs (_("\
  -u\n\
         equivalent ಗಮ್ಯ --update[=older].  See ಕೆಳಗೆ\n\
"));
      oputs (_("\
  -v, --verbose\n\
         explain what is being ಮುಗಿದ\n\
"));
      oputs (_("\
  -Z, --context\n\
         ಗಣ_2 SELinux security context ದ destination file ಗಮ್ಯ ಪೂರ್ವನಿಯೋಜಿತ type\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      emit_update_parameters_note ();
      emit_backup_suffix_note ();
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  bool make_backups = false;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *backup_suffix = ಖಾಲಿ;
  ಅಕ್ಷರ *ಆವೃತ್ತಿ_control_string = ಖಾಲಿ;
  ರಚನೆ cp_options x;
  bool remove_trailing_slashes = false;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಗುರಿ_directory = ಖಾಲಿ;
  bool no_ಗುರಿ_directory = false;
  bool selinux_enabled = (0 < is_selinux_enabled ());

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdin);

  cp_option_init (&x);

  /* Try to disable the ability to unlink a directory.  */
  priv_ಗಣ_remove_linkdir ();

  ಪೂರ್ಣಾಂಕ c;
  ಆಗ ((c = getopt_long (argc, argv, "bfint:uvS:TZ", long_options, ಖಾಲಿ))
         != -1)
    {
      ಆಯ್ಕೆ (c)
        {
        ಸ್ಥಿತಿ 'b':
          make_backups = true;
          ಆದರೆ (optarg)
            ಆವೃತ್ತಿ_control_string = optarg;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'f':
          x.interactive = I_ALWAYS_YES;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'i':
          x.interactive = I_ASK_USER;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'n':
          x.interactive = I_ALWAYS_SKIP;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ DEBUG_OPTION:
          x.debug = x.verbose = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ EXCHANGE_OPTION:
          x.exchange = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ NO_COPY_OPTION:
          x.no_copy = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ STRIP_TRAILING_SLASHES_OPTION:
          remove_trailing_slashes = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 't':
          ಆದರೆ (ಗುರಿ_directory)
            error (EXIT_FAILURE, 0, _("multiple target directories specified"));
          ಗುರಿ_directory = optarg;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'T':
          no_ಗುರಿ_directory = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'u':
          x.update = UPDATE_OLDER;
          ಆದರೆ (optarg)
            x.update = XARGMATCH ("--update", optarg,
                                  update_type_string, update_type);
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'v':
          x.verbose = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'S':
          make_backups = true;
          backup_suffix = optarg;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'Z':
          /* As a performance enhancement, don't even bother trying
             to "restorecon" when not on an selinux-enabled kernel.  */
          ಆದರೆ (selinux_enabled)
            {
              x.preserve_security_context = false;
              x.ಗಣ_security_context = selabel_open (SELABEL_CTX_FILE,
                                                     ಖಾಲಿ, 0);
              ಆದರೆ (! x.ಗಣ_security_context)
                error (0, errno, _("warning: ignoring --context"));
            }
          ನಿಲ್ಲಿಸು;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);
        ಪೂರ್ವನಿಯೋಜಿತ:
          usage (EXIT_FAILURE);
        }
    }

  ಪೂರ್ಣಾಂಕ n_files = argc - optind;
  ಅಕ್ಷರ **file = argv + optind;

  ಆದರೆ (n_files <= !ಗುರಿ_directory)
    {
      ಆದರೆ (n_files <= 0)
        error (0, 0, _("missing file operand"));
      ಇಲ್ಲದಿದ್ದರೆ
        error (0, 0, _("missing destination file operand after %s"),
               quoteaf (file[0]));
      usage (EXIT_FAILURE);
    }

  ರಚನೆ stat sb;
  sb.st_ಬಹುಲಕ = 0;
  ಪೂರ್ಣಾಂಕ ಗುರಿ_dirfd = AT_FDCWD;
  ಆದರೆ (no_ಗುರಿ_directory)
    {
      ಆದರೆ (ಗುರಿ_directory)
        error (EXIT_FAILURE, 0,
               _("cannot combine --target-directory (-t) "
                 "and --no-target-directory (-T)"));
      ಆದರೆ (2 < n_files)
        {
          error (0, 0, _("extra operand %s"), quoteaf (file[2]));
          usage (EXIT_FAILURE);
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಗುರಿ_directory)
    {
      ಗುರಿ_dirfd = ಗುರಿ_directory_ಕಾರ್ಯಾಂಶ (ಗುರಿ_directory, &sb);
      ಆದರೆ (! ಗುರಿ_dirfd_valid (ಗುರಿ_dirfd))
        error (EXIT_FAILURE, errno, _("target directory %s"),
               quoteaf (ಗುರಿ_directory));
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *lastfile = file[n_files - 1];
      ಆದರೆ (n_files == 2 && !x.exchange)
        x.rename_errno = (renameatu (AT_FDCWD, file[0], AT_FDCWD, lastfile,
                                     RENAME_NOREPLACE)
                          ? errno : 0);
      ಆದರೆ (x.rename_errno != 0)
        {
          ಪೂರ್ಣಾಂಕ fd = ಗುರಿ_directory_ಕಾರ್ಯಾಂಶ (lastfile, &sb);
          ಆದರೆ (ಗುರಿ_dirfd_valid (fd))
            {
              x.rename_errno = -1;
              ಗುರಿ_dirfd = fd;
              ಗುರಿ_directory = lastfile;
              n_files--;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              /* The last operand LASTFILE cannot be opened as a directory.
                 If there are more than two operands, report an error.

                 Also, report an error if LASTFILE is known to be a directory
                 even though it could not be opened, which can happen if
                 opening failed with EACCES on a platform lacking O_PATH.
                 In this case use stat to test whether LASTFILE is a
                 directory, in case opening a non-directory with (O_SEARCH
                 | O_DIRECTORY) failed with EACCES not ENOTDIR.  */
              ಪೂರ್ಣಾಂಕ err = errno;
              ಆದರೆ (2 < n_files
                  || (O_PATHSEARCH == O_ಹುಡುಕು && err == EACCES
                      && (sb.st_ಬಹುಲಕ != 0 || stat (lastfile, &sb) == 0)
                      && S_ISDIR (sb.st_ಬಹುಲಕ)))
                error (EXIT_FAILURE, err, _("target %s"), quoteaf (lastfile));
            }
        }
    }

  /* Handle the ambiguity in the semantics of mv induced by the
     varying semantics of the rename function.  POSIX-compatible
     systems (e.g., GNU/Linux) have a rename function that honors a
     trailing slash in the source, while others (Solaris 9, FreeBSD
     7.2) have a rename function that ignores it.  */
  ಆದರೆ (remove_trailing_slashes)
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < n_files; i++)
      strip_trailing_slashes (file[i]);

  ಆದರೆ (x.interactive == I_ALWAYS_SKIP)
    x.update = UPDATE_NONE;

  ಆದರೆ (make_backups
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
                   ? xget_ಆವೃತ್ತಿ (_("backup type"),
                                   ಆವೃತ್ತಿ_control_string)
                   : no_backups);
  ಗಣ_simple_backup_suffix (backup_suffix);

  hash_init ();

  bool ok;
  ಆದರೆ (ಗುರಿ_directory)
    {
      /* Initialize the hash table only if we'll need it.
         The problem it is used to detect can arise only if there are
         two or more files to move.  */
      ಆದರೆ (2 <= n_files)
        dest_info_init (&x);

      ok = true;
      ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < n_files; ++i)
        {
          x.ಕೊನೆಯ_file = i + 1 == n_files;
          ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಮೂಲ_2 = file[i];
          ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಮೂಲ_basename = ಕೊನೆಯ_component (ಮೂಲ_2);
          ಅಕ್ಷರ *dest_relname;
          ಅಕ್ಷರ *dest = file_name_concat (ಗುರಿ_directory, ಮೂಲ_basename,
                                         &dest_relname);
          strip_trailing_slashes (dest_relname);
          ok &= do_move (ಮೂಲ_2, dest, ಗುರಿ_dirfd, dest_relname, &x);
          ಬಿಡುಗಡೆ (dest);
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      x.ಕೊನೆಯ_file = true;
      ok = do_move (file[0], file[1], AT_FDCWD, file[1], &x);
    }

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
