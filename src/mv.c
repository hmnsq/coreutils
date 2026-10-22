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

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <getopt.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <selinux/label.h>

#అన్తర్భావ "system.h"
#అన్తర్భావ "argmatch.h"
#అన్తర్భావ "assure.h"
#అన్తర్భావ "backupfile.h"
#అన్తర్భావ "copy.h"
#అన్తర్భావ "cp-hash.h"
#అన్తర్భావ "filenamecat.h"
#అన్తర్భావ "remove.h"
#అన్తర్భావ "renameatu.h"
#అన్తర్భావ "root-dev-ino.h"
#అన్తర్భావ "targetdir.h"
#అన్తర్భావ "priv-set.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "mv"

#నిర్వచించు AUTHORS \
  proper_name ("Mike Parker"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
గణన
{
  DEBUG_OPTION = CHAR_MAX + 1,
  EXCHANGE_OPTION,
  NO_COPY_OPTION,
  STRIP_TRAILING_SLASHES_OPTION
};

స్థిర అక్షరం స్థిరరాశి *స్థిరరాశి update_type_string[] =
{
  "all", "none", "none-fail", "older", ఖాళీ
};
స్థిర గణన Update_type స్థిరరాశి update_type[] =
{
  UPDATE_ALL, UPDATE_NONE, UPDATE_NONE_FAIL, UPDATE_OLDER,
};
ARGMATCH_VERIFY (update_type_string, update_type);

స్థిర నిర్మాణం option స్థిరరాశి long_options[] =
{
  {"backup", optional_వాదం, ఖాళీ, 'b'},
  {"context", no_వాదం, ఖాళీ, 'Z'},
  {"debug", no_వాదం, ఖాళీ, DEBUG_OPTION},
  {"exchange", no_వాదం, ఖాళీ, EXCHANGE_OPTION},
  {"force", no_వాదం, ఖాళీ, 'f'},
  {"interactive", no_వాదం, ఖాళీ, 'i'},
  {"no-clobber", no_వాదం, ఖాళీ, 'n'},   /* Deprecated.  */
  {"no-copy", no_వాదం, ఖాళీ, NO_COPY_OPTION},
  {"no-target-directory", no_వాదం, ఖాళీ, 'T'},
  {"strip-trailing-slashes", no_వాదం, ఖాళీ,
   STRIP_TRAILING_SLASHES_OPTION},
  {"suffix", required_వాదం, ఖాళీ, 'S'},
  {"target-directory", required_వాదం, ఖాళీ, 't'},
  {"update", optional_వాదం, ఖాళీ, 'u'},
  {"verbose", no_వాదం, ఖాళీ, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0}
};

స్థిర శూన్య
rm_option_init (నిర్మాణం rm_options *x)
{
  x->ignore_missing_files = false;
  x->remove_ఖాళీలు_directories = true;
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
    స్థిర నిర్మాణం dev_ino dev_ino_buf;
    x->మూలం_dev_ino = get_మూలం_dev_ino (&dev_ino_buf);
    ఒకవేళ (x->మూలం_dev_ino == ఖాళీ)
      error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
             quoteaf ("/"));
  }

  x->preserve_all_మూలం = false;
}

స్థిర శూన్య
cp_option_init (నిర్మాణం cp_options *x)
{
  bool selinux_enabled = (0 < is_selinux_enabled ());

  cp_options_default (x);
  x->copy_as_regular = false;  /* FIXME: maybe make this an option */
  x->reflink_బహుళకం = REFLINK_AUTO;
  x->dereference = DEREF_NEVER;
  x->unlink_dest_before_opening = false;
  x->unlink_dest_after_failed_open = false;
  x->hard_link = false;
  x->interactive = I_UNSPECIFIED;
  x->move_బహుళకం = true;
  x->install_బహుళకం = false;
  x->one_file_system = false;
  x->preserve_ownership = true;
  x->preserve_links = true;
  x->preserve_బహుళకం = true;
  x->preserve_timestamps = true;
  x->explicit_no_preserve_బహుళకం= false;
  x->preserve_security_context = selinux_enabled;
  x->సమితి_security_context = ఖాళీ;
  x->reduce_diagnostics = false;
  x->దత్తాంశం_copy_required = true;
  x->require_preserve = false;  /* FIXME: maybe make this an option */
  x->require_preserve_context = false;
  x->preserve_xattr = true;
  x->require_preserve_xattr = true;
  x->recursive = true;
  x->sparse_బహుళకం = SPARSE_AUTO;  /* FIXME: maybe make this an option */
  x->symbolic_link = false;
  x->సమితి_బహుళకం = false;
  x->బహుళకం_2 = 0;
  x->stdin_tty = isatty (STDIN_FILENO);

  x->open_dangling_dest_symlink = false;
  x->update = UPDATE_ALL;
  x->verbose = false;
  x->dest_info = ఖాళీ;
  x->src_info = ఖాళీ;
}

/* Move SOURCE onto DEST aka DEST_DIRFD+DEST_RELNAME.
   Handle cross-file-system moves.
   If SOURCE is a directory, DEST must not exist.
   Return true if successful.  */

స్థిర bool
do_move (అక్షరం స్థిరరాశి *ఆధారం_2, అక్షరం స్థిరరాశి *dest,
         పూర్ణాంకం dest_dirfd, అక్షరం స్థిరరాశి *dest_relname, స్థిరరాశి నిర్మాణం cp_options *x)
{
  bool copy_into_self;
  bool rename_succeeded;
  bool ok = copy (ఆధారం_2, dest, dest_dirfd, dest_relname, 0, x,
                  &copy_into_self, &rename_succeeded);

  ఒకవేళ (ok)
    {
      అక్షరం స్థిరరాశి *dir_గమ్యం_remove;
      ఒకవేళ (copy_into_self)
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

          dir_గమ్యం_remove = ఖాళీ;
          ok = false;
        }
      లేకపోతే ఒకవేళ (rename_succeeded)
        {
          /* No need to remove anything.  SOURCE was successfully
             renamed to DEST.  Or the user declined to rename a file.  */
          dir_గమ్యం_remove = ఖాళీ;
        }
      లేకపోతే
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

          dir_గమ్యం_remove = ఆధారం_2;
        }

      ఒకవేళ (dir_గమ్యం_remove != ఖాళీ)
        {
          నిర్మాణం rm_options rm_options;
          rm_option_init (&rm_options);
          rm_options.verbose = x->verbose;
          అక్షరం స్థిరరాశి *dir[2] = { dir_గమ్యం_remove, ఖాళీ };

          గణన RM_స్థితి స్థితి_2 = rm ((శూన్య *) dir, &rm_options);
          affirm (VALID_స్థితి (స్థితి_2));
          ఒకవేళ (స్థితి_2 == RM_ERROR)
            ok = false;
        }
    }

  తిరిగి ok;
}

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("\
Usage: %s [OPTION]... [-T] ఆధారం DEST\n\
  or:  %s [OPTION]... ఆధారం... DIRECTORY\n\
  or:  %s [OPTION]... -t DIRECTORY ఆధారం...\n\
"),
              కార్యక్రమం_name, కార్యక్రమం_name, కార్యక్రమం_name);
      fputs (_("\
Rename ఆధారం గమ్యం DEST, or move ఆధారం(s) గమ్యం DIRECTORY.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
      --backup[=CONTROL]\n\
         make a backup యొక్క each existing destination file\n\
"));
      oputs (_("\
  -b\n\
         like --backup but does not accept an వాదం\n\
"));
      oputs (_("\
      --debug\n\
         explain how a file is copied.  సూచిస్తుంది_2 -v\n\
"));
      oputs (_("\
      --exchange\n\
         exchange ఆధారం_2 and destination\n\
"));
      oputs (_("\
  -f, --force\n\
         చేయి not prompt before overwriting\n\
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
If you specify మరిన్ని than one యొక్క -i, -f, -n, only the final one takes effect.\n\
"), stdout);
      oputs (_("\
      --no-copy\n\
         చేయి not copy ఒకవేళ renaming fails\n\
"));
      oputs (_("\
      --strip-trailing-slashes\n\
         remove any trailing slashes from each ఆధారం వాదం\n\
"));
      oputs (_("\
  -S, --suffix=SUFFIX\n\
         override the usual backup suffix\n\
"));
      oputs (_("\
  -t, --లక్ష్యం-directory=DIRECTORY\n\
         move all ఆధారం arguments into DIRECTORY\n\
"));
      oputs (_("\
  -T, --no-లక్ష్యం-directory\n\
         treat DEST as a సాధారణ file\n\
"));
      oputs (_("\
      --update[=UPDATE]\n\
         control which existing files are updated;\n\
         UPDATE={all,none,none-fail,older(అప్రమేయ)}\n\
"));
      oputs (_("\
  -u\n\
         equivalent గమ్యం --update[=older].  See క్రింద\n\
"));
      oputs (_("\
  -v, --verbose\n\
         explain what is being పూర్తి\n\
"));
      oputs (_("\
  -Z, --context\n\
         సమితి_2 SELinux security context యొక్క destination file గమ్యం అప్రమేయ type\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      emit_update_parameters_note ();
      emit_backup_suffix_note ();
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  bool make_backups = false;
  అక్షరం స్థిరరాశి *backup_suffix = ఖాళీ;
  అక్షరం *సంస్కరణ_control_string = ఖాళీ;
  నిర్మాణం cp_options x;
  bool remove_trailing_slashes = false;
  అక్షరం స్థిరరాశి *లక్ష్యం_directory = ఖాళీ;
  bool no_లక్ష్యం_directory = false;
  bool selinux_enabled = (0 < is_selinux_enabled ());

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdin);

  cp_option_init (&x);

  /* Try to disable the ability to unlink a directory.  */
  priv_సమితి_remove_linkdir ();

  పూర్ణాంకం c;
  అయితే ((c = getopt_long (argc, argv, "bfint:uvS:TZ", long_options, ఖాళీ))
         != -1)
    {
      ఎంపిక (c)
        {
        స్థితి 'b':
          make_backups = true;
          ఒకవేళ (optarg)
            సంస్కరణ_control_string = optarg;
          ఆపు;
        స్థితి 'f':
          x.interactive = I_ALWAYS_YES;
          ఆపు;
        స్థితి 'i':
          x.interactive = I_ASK_USER;
          ఆపు;
        స్థితి 'n':
          x.interactive = I_ALWAYS_SKIP;
          ఆపు;
        స్థితి DEBUG_OPTION:
          x.debug = x.verbose = true;
          ఆపు;
        స్థితి EXCHANGE_OPTION:
          x.exchange = true;
          ఆపు;
        స్థితి NO_COPY_OPTION:
          x.no_copy = true;
          ఆపు;
        స్థితి STRIP_TRAILING_SLASHES_OPTION:
          remove_trailing_slashes = true;
          ఆపు;
        స్థితి 't':
          ఒకవేళ (లక్ష్యం_directory)
            error (EXIT_FAILURE, 0, _("multiple target directories specified"));
          లక్ష్యం_directory = optarg;
          ఆపు;
        స్థితి 'T':
          no_లక్ష్యం_directory = true;
          ఆపు;
        స్థితి 'u':
          x.update = UPDATE_OLDER;
          ఒకవేళ (optarg)
            x.update = XARGMATCH ("--update", optarg,
                                  update_type_string, update_type);
          ఆపు;
        స్థితి 'v':
          x.verbose = true;
          ఆపు;
        స్థితి 'S':
          make_backups = true;
          backup_suffix = optarg;
          ఆపు;
        స్థితి 'Z':
          /* As a performance enhancement, don't even bother trying
             to "restorecon" when not on an selinux-enabled kernel.  */
          ఒకవేళ (selinux_enabled)
            {
              x.preserve_security_context = false;
              x.సమితి_security_context = selabel_open (SELABEL_CTX_FILE,
                                                     ఖాళీ, 0);
              ఒకవేళ (! x.సమితి_security_context)
                error (0, errno, _("warning: ignoring --context"));
            }
          ఆపు;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);
        అప్రమేయ:
          usage (EXIT_FAILURE);
        }
    }

  పూర్ణాంకం n_files = argc - optind;
  అక్షరం **file = argv + optind;

  ఒకవేళ (n_files <= !లక్ష్యం_directory)
    {
      ఒకవేళ (n_files <= 0)
        error (0, 0, _("missing file operand"));
      లేకపోతే
        error (0, 0, _("missing destination file operand after %s"),
               quoteaf (file[0]));
      usage (EXIT_FAILURE);
    }

  నిర్మాణం stat sb;
  sb.st_బహుళకం = 0;
  పూర్ణాంకం లక్ష్యం_dirfd = AT_FDCWD;
  ఒకవేళ (no_లక్ష్యం_directory)
    {
      ఒకవేళ (లక్ష్యం_directory)
        error (EXIT_FAILURE, 0,
               _("cannot combine --target-directory (-t) "
                 "and --no-target-directory (-T)"));
      ఒకవేళ (2 < n_files)
        {
          error (0, 0, _("extra operand %s"), quoteaf (file[2]));
          usage (EXIT_FAILURE);
        }
    }
  లేకపోతే ఒకవేళ (లక్ష్యం_directory)
    {
      లక్ష్యం_dirfd = లక్ష్యం_directory_నిర్వాహ్యం (లక్ష్యం_directory, &sb);
      ఒకవేళ (! లక్ష్యం_dirfd_valid (లక్ష్యం_dirfd))
        error (EXIT_FAILURE, errno, _("target directory %s"),
               quoteaf (లక్ష్యం_directory));
    }
  లేకపోతే
    {
      అక్షరం స్థిరరాశి *lastfile = file[n_files - 1];
      ఒకవేళ (n_files == 2 && !x.exchange)
        x.rename_errno = (renameatu (AT_FDCWD, file[0], AT_FDCWD, lastfile,
                                     RENAME_NOREPLACE)
                          ? errno : 0);
      ఒకవేళ (x.rename_errno != 0)
        {
          పూర్ణాంకం fd = లక్ష్యం_directory_నిర్వాహ్యం (lastfile, &sb);
          ఒకవేళ (లక్ష్యం_dirfd_valid (fd))
            {
              x.rename_errno = -1;
              లక్ష్యం_dirfd = fd;
              లక్ష్యం_directory = lastfile;
              n_files--;
            }
          లేకపోతే
            {
              /* The last operand LASTFILE cannot be opened as a directory.
                 If there are more than two operands, report an error.

                 Also, report an error if LASTFILE is known to be a directory
                 even though it could not be opened, which can happen if
                 opening failed with EACCES on a platform lacking O_PATH.
                 In this case use stat to test whether LASTFILE is a
                 directory, in case opening a non-directory with (O_SEARCH
                 | O_DIRECTORY) failed with EACCES not ENOTDIR.  */
              పూర్ణాంకం err = errno;
              ఒకవేళ (2 < n_files
                  || (O_PATHSEARCH == O_శోధన && err == EACCES
                      && (sb.st_బహుళకం != 0 || stat (lastfile, &sb) == 0)
                      && S_ISDIR (sb.st_బహుళకం)))
                error (EXIT_FAILURE, err, _("target %s"), quoteaf (lastfile));
            }
        }
    }

  /* Handle the ambiguity in the semantics of mv induced by the
     varying semantics of the rename function.  POSIX-compatible
     systems (e.g., GNU/Linux) have a rename function that honors a
     trailing slash in the source, while others (Solaris 9, FreeBSD
     7.2) have a rename function that ignores it.  */
  ఒకవేళ (remove_trailing_slashes)
    కొరకు (పూర్ణాంకం i = 0; i < n_files; i++)
      strip_trailing_slashes (file[i]);

  ఒకవేళ (x.interactive == I_ALWAYS_SKIP)
    x.update = UPDATE_NONE;

  ఒకవేళ (make_backups
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
                   ? xget_సంస్కరణ (_("backup type"),
                                   సంస్కరణ_control_string)
                   : no_backups);
  సమితి_simple_backup_suffix (backup_suffix);

  hash_init ();

  bool ok;
  ఒకవేళ (లక్ష్యం_directory)
    {
      /* Initialize the hash table only if we'll need it.
         The problem it is used to detect can arise only if there are
         two or more files to move.  */
      ఒకవేళ (2 <= n_files)
        dest_info_init (&x);

      ok = true;
      కొరకు (పూర్ణాంకం i = 0; i < n_files; ++i)
        {
          x.చివరి_file = i + 1 == n_files;
          అక్షరం స్థిరరాశి *ఆధారం_2 = file[i];
          అక్షరం స్థిరరాశి *ఆధారం_basename = చివరి_component (ఆధారం_2);
          అక్షరం *dest_relname;
          అక్షరం *dest = file_name_concat (లక్ష్యం_directory, ఆధారం_basename,
                                         &dest_relname);
          strip_trailing_slashes (dest_relname);
          ok &= do_move (ఆధారం_2, dest, లక్ష్యం_dirfd, dest_relname, &x);
          విడుదల (dest);
        }
    }
  లేకపోతే
    {
      x.చివరి_file = true;
      ok = do_move (file[0], file[1], AT_FDCWD, file[1], &x);
    }

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
