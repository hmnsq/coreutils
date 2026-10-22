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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <selinux/label.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "argmatch.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "assure.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "backupfile.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "copy.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cp-hash.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "filenamecat.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "remove.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "renameatu.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "root-dev-ino.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "targetdir.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "priv-set.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "mv"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS \
  proper_name ("Mike Parker"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ਗਿਣਤੀ
{
  DEBUG_OPTION = CHAR_MAX + 1,
  EXCHANGE_OPTION,
  NO_COPY_OPTION,
  STRIP_TRAILING_SLASHES_OPTION
};

ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ਅਚਲ update_type_string[] =
{
  "all", "none", "none-fail", "older", ਨਿਰਾ
};
ਸਥਿਰ ਗਿਣਤੀ Update_type ਅਚਲ update_type[] =
{
  UPDATE_ALL, UPDATE_NONE, UPDATE_NONE_FAIL, UPDATE_OLDER,
};
ARGMATCH_VERIFY (update_type_string, update_type);

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ long_options[] =
{
  {"backup", optional_ਦਲੀਲ, ਨਿਰਾ, 'b'},
  {"context", no_ਦਲੀਲ, ਨਿਰਾ, 'Z'},
  {"debug", no_ਦਲੀਲ, ਨਿਰਾ, DEBUG_OPTION},
  {"exchange", no_ਦਲੀਲ, ਨਿਰਾ, EXCHANGE_OPTION},
  {"force", no_ਦਲੀਲ, ਨਿਰਾ, 'f'},
  {"interactive", no_ਦਲੀਲ, ਨਿਰਾ, 'i'},
  {"no-clobber", no_ਦਲੀਲ, ਨਿਰਾ, 'n'},   /* Deprecated.  */
  {"no-copy", no_ਦਲੀਲ, ਨਿਰਾ, NO_COPY_OPTION},
  {"no-target-directory", no_ਦਲੀਲ, ਨਿਰਾ, 'T'},
  {"strip-trailing-slashes", no_ਦਲੀਲ, ਨਿਰਾ,
   STRIP_TRAILING_SLASHES_OPTION},
  {"suffix", required_ਦਲੀਲ, ਨਿਰਾ, 'S'},
  {"target-directory", required_ਦਲੀਲ, ਨਿਰਾ, 't'},
  {"update", optional_ਦਲੀਲ, ਨਿਰਾ, 'u'},
  {"verbose", no_ਦਲੀਲ, ਨਿਰਾ, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};

ਸਥਿਰ ਖਾਲੀ
rm_option_init (ਬਣਤਰ rm_options *x)
{
  x->ignore_missing_files = false;
  x->remove_ਖਾਲੀ_ਥਾਵਾਂ_directories = true;
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
    ਸਥਿਰ ਬਣਤਰ dev_ino dev_ino_buf;
    x->ਜੜ੍ਹ_dev_ino = get_ਜੜ੍ਹ_dev_ino (&dev_ino_buf);
    ਜੇ (x->ਜੜ੍ਹ_dev_ino == ਨਿਰਾ)
      error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
             quoteaf ("/"));
  }

  x->preserve_all_ਜੜ੍ਹ = false;
}

ਸਥਿਰ ਖਾਲੀ
cp_option_init (ਬਣਤਰ cp_options *x)
{
  bool selinux_enabled = (0 < is_selinux_enabled ());

  cp_options_default (x);
  x->copy_as_regular = false;  /* FIXME: maybe make this an option */
  x->reflink_ਬਹੁਲਕ = REFLINK_AUTO;
  x->dereference = DEREF_NEVER;
  x->unlink_dest_before_opening = false;
  x->unlink_dest_after_failed_open = false;
  x->hard_link = false;
  x->interactive = I_UNSPECIFIED;
  x->move_ਬਹੁਲਕ = true;
  x->install_ਬਹੁਲਕ = false;
  x->one_file_system = false;
  x->preserve_ownership = true;
  x->preserve_links = true;
  x->preserve_ਬਹੁਲਕ = true;
  x->preserve_timestamps = true;
  x->explicit_no_preserve_ਬਹੁਲਕ= false;
  x->preserve_security_context = selinux_enabled;
  x->ਸੈੱਟ_security_context = ਨਿਰਾ;
  x->reduce_diagnostics = false;
  x->ਅੰਕੜੇ_copy_required = true;
  x->require_preserve = false;  /* FIXME: maybe make this an option */
  x->require_preserve_context = false;
  x->preserve_xattr = true;
  x->require_preserve_xattr = true;
  x->recursive = true;
  x->sparse_ਬਹੁਲਕ = SPARSE_AUTO;  /* FIXME: maybe make this an option */
  x->symbolic_link = false;
  x->ਸੈੱਟ_ਬਹੁਲਕ = false;
  x->ਬਹੁਲਕ_2 = 0;
  x->stdin_tty = isatty (STDIN_FILENO);

  x->open_dangling_dest_symlink = false;
  x->update = UPDATE_ALL;
  x->verbose = false;
  x->dest_info = ਨਿਰਾ;
  x->src_info = ਨਿਰਾ;
}

/* Move SOURCE onto DEST aka DEST_DIRFD+DEST_RELNAME.
   Handle cross-file-system moves.
   If SOURCE is a directory, DEST must not exist.
   Return true if successful.  */

ਸਥਿਰ bool
do_move (ਅੱਖਰ ਅਚਲ *ਸਰੋਤ_2, ਅੱਖਰ ਅਚਲ *dest,
         ਪੂਰਨ dest_dirfd, ਅੱਖਰ ਅਚਲ *dest_relname, ਅਚਲ ਬਣਤਰ cp_options *x)
{
  bool copy_into_self;
  bool rename_succeeded;
  bool ok = copy (ਸਰੋਤ_2, dest, dest_dirfd, dest_relname, 0, x,
                  &copy_into_self, &rename_succeeded);

  ਜੇ (ok)
    {
      ਅੱਖਰ ਅਚਲ *dir_ਮੰਜ਼ਿਲ_remove;
      ਜੇ (copy_into_self)
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

          dir_ਮੰਜ਼ਿਲ_remove = ਨਿਰਾ;
          ok = false;
        }
      ਨਹੀਂਤਾਂ ਜੇ (rename_succeeded)
        {
          /* No need to remove anything.  SOURCE was successfully
             renamed to DEST.  Or the user declined to rename a file.  */
          dir_ਮੰਜ਼ਿਲ_remove = ਨਿਰਾ;
        }
      ਨਹੀਂਤਾਂ
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

          dir_ਮੰਜ਼ਿਲ_remove = ਸਰੋਤ_2;
        }

      ਜੇ (dir_ਮੰਜ਼ਿਲ_remove != ਨਿਰਾ)
        {
          ਬਣਤਰ rm_options rm_options;
          rm_option_init (&rm_options);
          rm_options.verbose = x->verbose;
          ਅੱਖਰ ਅਚਲ *dir[2] = { dir_ਮੰਜ਼ਿਲ_remove, ਨਿਰਾ };

          ਗਿਣਤੀ RM_ਸਥਿਤੀ ਸਥਿਤੀ = rm ((ਖਾਲੀ *) dir, &rm_options);
          affirm (VALID_ਸਥਿਤੀ (ਸਥਿਤੀ));
          ਜੇ (ਸਥਿਤੀ == RM_ERROR)
            ok = false;
        }
    }

  ਵਾਪਸ ok;
}

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("\
Usage: %s [OPTION]... [-T] ਸਰੋਤ DEST\n\
  or:  %s [OPTION]... ਸਰੋਤ... DIRECTORY\n\
  or:  %s [OPTION]... -t DIRECTORY ਸਰੋਤ...\n\
"),
              ਪ੍ਰੋਗਰਾਮ_name, ਪ੍ਰੋਗਰਾਮ_name, ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
Rename ਸਰੋਤ ਮੰਜ਼ਿਲ DEST, or move ਸਰੋਤ(s) ਮੰਜ਼ਿਲ DIRECTORY.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
      --backup[=CONTROL]\n\
         make a backup ਦਾ each existing destination file\n\
"));
      oputs (_("\
  -b\n\
         like --backup but does not accept an ਦਲੀਲ\n\
"));
      oputs (_("\
      --debug\n\
         explain how a file is copied.  ਸਿੱਟਾ_ਦਿੰਦਾ_2 -v\n\
"));
      oputs (_("\
      --exchange\n\
         exchange ਸਰੋਤ_2 and destination\n\
"));
      oputs (_("\
  -f, --force\n\
         ਕਰੋ not prompt before overwriting\n\
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
If you specify ਹੋਰ than one ਦਾ -i, -f, -n, only the final one takes effect.\n\
"), stdout);
      oputs (_("\
      --no-copy\n\
         ਕਰੋ not copy ਜੇ renaming fails\n\
"));
      oputs (_("\
      --strip-trailing-slashes\n\
         remove any trailing slashes from each ਸਰੋਤ ਦਲੀਲ\n\
"));
      oputs (_("\
  -S, --suffix=SUFFIX\n\
         override the usual backup suffix\n\
"));
      oputs (_("\
  -t, --ਨਿਸ਼ਾਨਾ-directory=DIRECTORY\n\
         move all ਸਰੋਤ arguments into DIRECTORY\n\
"));
      oputs (_("\
  -T, --no-ਨਿਸ਼ਾਨਾ-directory\n\
         treat DEST as a ਸਾਧਾਰਨ file\n\
"));
      oputs (_("\
      --update[=UPDATE]\n\
         control which existing files are updated;\n\
         UPDATE={all,none,none-fail,older(ਮੂਲ)}\n\
"));
      oputs (_("\
  -u\n\
         equivalent ਮੰਜ਼ਿਲ --update[=older].  See ਹੇਠਾਂ\n\
"));
      oputs (_("\
  -v, --verbose\n\
         explain what is being ਪੂਰਾ\n\
"));
      oputs (_("\
  -Z, --context\n\
         ਸੈੱਟ_2 SELinux security context ਦਾ destination file ਮੰਜ਼ਿਲ ਮੂਲ type\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      emit_update_parameters_note ();
      emit_backup_suffix_note ();
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  bool make_backups = false;
  ਅੱਖਰ ਅਚਲ *backup_suffix = ਨਿਰਾ;
  ਅੱਖਰ *ਸੰਸਕਰਣ_control_string = ਨਿਰਾ;
  ਬਣਤਰ cp_options x;
  bool remove_trailing_slashes = false;
  ਅੱਖਰ ਅਚਲ *ਨਿਸ਼ਾਨਾ_directory = ਨਿਰਾ;
  bool no_ਨਿਸ਼ਾਨਾ_directory = false;
  bool selinux_enabled = (0 < is_selinux_enabled ());

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdin);

  cp_option_init (&x);

  /* Try to disable the ability to unlink a directory.  */
  priv_ਸੈੱਟ_remove_linkdir ();

  ਪੂਰਨ c;
  ਜਦੋਂ ((c = getopt_long (argc, argv, "bfint:uvS:TZ", long_options, ਨਿਰਾ))
         != -1)
    {
      ਚੋਣ (c)
        {
        ਹਾਲਤ 'b':
          make_backups = true;
          ਜੇ (optarg)
            ਸੰਸਕਰਣ_control_string = optarg;
          ਰੋਕੋ;
        ਹਾਲਤ 'f':
          x.interactive = I_ALWAYS_YES;
          ਰੋਕੋ;
        ਹਾਲਤ 'i':
          x.interactive = I_ASK_USER;
          ਰੋਕੋ;
        ਹਾਲਤ 'n':
          x.interactive = I_ALWAYS_SKIP;
          ਰੋਕੋ;
        ਹਾਲਤ DEBUG_OPTION:
          x.debug = x.verbose = true;
          ਰੋਕੋ;
        ਹਾਲਤ EXCHANGE_OPTION:
          x.exchange = true;
          ਰੋਕੋ;
        ਹਾਲਤ NO_COPY_OPTION:
          x.no_copy = true;
          ਰੋਕੋ;
        ਹਾਲਤ STRIP_TRAILING_SLASHES_OPTION:
          remove_trailing_slashes = true;
          ਰੋਕੋ;
        ਹਾਲਤ 't':
          ਜੇ (ਨਿਸ਼ਾਨਾ_directory)
            error (EXIT_FAILURE, 0, _("multiple target directories specified"));
          ਨਿਸ਼ਾਨਾ_directory = optarg;
          ਰੋਕੋ;
        ਹਾਲਤ 'T':
          no_ਨਿਸ਼ਾਨਾ_directory = true;
          ਰੋਕੋ;
        ਹਾਲਤ 'u':
          x.update = UPDATE_OLDER;
          ਜੇ (optarg)
            x.update = XARGMATCH ("--update", optarg,
                                  update_type_string, update_type);
          ਰੋਕੋ;
        ਹਾਲਤ 'v':
          x.verbose = true;
          ਰੋਕੋ;
        ਹਾਲਤ 'S':
          make_backups = true;
          backup_suffix = optarg;
          ਰੋਕੋ;
        ਹਾਲਤ 'Z':
          /* As a performance enhancement, don't even bother trying
             to "restorecon" when not on an selinux-enabled kernel.  */
          ਜੇ (selinux_enabled)
            {
              x.preserve_security_context = false;
              x.ਸੈੱਟ_security_context = selabel_open (SELABEL_CTX_FILE,
                                                     ਨਿਰਾ, 0);
              ਜੇ (! x.ਸੈੱਟ_security_context)
                error (0, errno, _("warning: ignoring --context"));
            }
          ਰੋਕੋ;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);
        ਮੂਲ:
          usage (EXIT_FAILURE);
        }
    }

  ਪੂਰਨ n_files = argc - optind;
  ਅੱਖਰ **file = argv + optind;

  ਜੇ (n_files <= !ਨਿਸ਼ਾਨਾ_directory)
    {
      ਜੇ (n_files <= 0)
        error (0, 0, _("missing file operand"));
      ਨਹੀਂਤਾਂ
        error (0, 0, _("missing destination file operand after %s"),
               quoteaf (file[0]));
      usage (EXIT_FAILURE);
    }

  ਬਣਤਰ stat sb;
  sb.st_ਬਹੁਲਕ = 0;
  ਪੂਰਨ ਨਿਸ਼ਾਨਾ_dirfd = AT_FDCWD;
  ਜੇ (no_ਨਿਸ਼ਾਨਾ_directory)
    {
      ਜੇ (ਨਿਸ਼ਾਨਾ_directory)
        error (EXIT_FAILURE, 0,
               _("cannot combine --target-directory (-t) "
                 "and --no-target-directory (-T)"));
      ਜੇ (2 < n_files)
        {
          error (0, 0, _("extra operand %s"), quoteaf (file[2]));
          usage (EXIT_FAILURE);
        }
    }
  ਨਹੀਂਤਾਂ ਜੇ (ਨਿਸ਼ਾਨਾ_directory)
    {
      ਨਿਸ਼ਾਨਾ_dirfd = ਨਿਸ਼ਾਨਾ_directory_ਸੰਕਾਰਜ (ਨਿਸ਼ਾਨਾ_directory, &sb);
      ਜੇ (! ਨਿਸ਼ਾਨਾ_dirfd_valid (ਨਿਸ਼ਾਨਾ_dirfd))
        error (EXIT_FAILURE, errno, _("target directory %s"),
               quoteaf (ਨਿਸ਼ਾਨਾ_directory));
    }
  ਨਹੀਂਤਾਂ
    {
      ਅੱਖਰ ਅਚਲ *lastfile = file[n_files - 1];
      ਜੇ (n_files == 2 && !x.exchange)
        x.rename_errno = (renameatu (AT_FDCWD, file[0], AT_FDCWD, lastfile,
                                     RENAME_NOREPLACE)
                          ? errno : 0);
      ਜੇ (x.rename_errno != 0)
        {
          ਪੂਰਨ fd = ਨਿਸ਼ਾਨਾ_directory_ਸੰਕਾਰਜ (lastfile, &sb);
          ਜੇ (ਨਿਸ਼ਾਨਾ_dirfd_valid (fd))
            {
              x.rename_errno = -1;
              ਨਿਸ਼ਾਨਾ_dirfd = fd;
              ਨਿਸ਼ਾਨਾ_directory = lastfile;
              n_files--;
            }
          ਨਹੀਂਤਾਂ
            {
              /* The last operand LASTFILE cannot be opened as a directory.
                 If there are more than two operands, report an error.

                 Also, report an error if LASTFILE is known to be a directory
                 even though it could not be opened, which can happen if
                 opening failed with EACCES on a platform lacking O_PATH.
                 In this case use stat to test whether LASTFILE is a
                 directory, in case opening a non-directory with (O_SEARCH
                 | O_DIRECTORY) failed with EACCES not ENOTDIR.  */
              ਪੂਰਨ err = errno;
              ਜੇ (2 < n_files
                  || (O_PATHSEARCH == O_ਖੋਜ && err == EACCES
                      && (sb.st_ਬਹੁਲਕ != 0 || stat (lastfile, &sb) == 0)
                      && S_ISDIR (sb.st_ਬਹੁਲਕ)))
                error (EXIT_FAILURE, err, _("target %s"), quoteaf (lastfile));
            }
        }
    }

  /* Handle the ambiguity in the semantics of mv induced by the
     varying semantics of the rename function.  POSIX-compatible
     systems (e.g., GNU/Linux) have a rename function that honors a
     trailing slash in the source, while others (Solaris 9, FreeBSD
     7.2) have a rename function that ignores it.  */
  ਜੇ (remove_trailing_slashes)
    ਲਈ (ਪੂਰਨ i = 0; i < n_files; i++)
      strip_trailing_slashes (file[i]);

  ਜੇ (x.interactive == I_ALWAYS_SKIP)
    x.update = UPDATE_NONE;

  ਜੇ (make_backups
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
                   ? xget_ਸੰਸਕਰਣ (_("backup type"),
                                   ਸੰਸਕਰਣ_control_string)
                   : no_backups);
  ਸੈੱਟ_simple_backup_suffix (backup_suffix);

  hash_init ();

  bool ok;
  ਜੇ (ਨਿਸ਼ਾਨਾ_directory)
    {
      /* Initialize the hash table only if we'll need it.
         The problem it is used to detect can arise only if there are
         two or more files to move.  */
      ਜੇ (2 <= n_files)
        dest_info_init (&x);

      ok = true;
      ਲਈ (ਪੂਰਨ i = 0; i < n_files; ++i)
        {
          x.ਆਖਰੀ_file = i + 1 == n_files;
          ਅੱਖਰ ਅਚਲ *ਸਰੋਤ_2 = file[i];
          ਅੱਖਰ ਅਚਲ *ਸਰੋਤ_basename = ਆਖਰੀ_component (ਸਰੋਤ_2);
          ਅੱਖਰ *dest_relname;
          ਅੱਖਰ *dest = file_name_concat (ਨਿਸ਼ਾਨਾ_directory, ਸਰੋਤ_basename,
                                         &dest_relname);
          strip_trailing_slashes (dest_relname);
          ok &= do_move (ਸਰੋਤ_2, dest, ਨਿਸ਼ਾਨਾ_dirfd, dest_relname, &x);
          ਮੁਕਤ (dest);
        }
    }
  ਨਹੀਂਤਾਂ
    {
      x.ਆਖਰੀ_file = true;
      ok = do_move (file[0], file[1], AT_FDCWD, file[1], &x);
    }

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
