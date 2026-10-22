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

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <getopt.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <selinux/label.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "argmatch.h"
#अन्तर्भाव "assure.h"
#अन्तर्भाव "backupfile.h"
#अन्तर्भाव "copy.h"
#अन्तर्भाव "cp-hash.h"
#अन्तर्भाव "filenamecat.h"
#अन्तर्भाव "remove.h"
#अन्तर्भाव "renameatu.h"
#अन्तर्भाव "root-dev-ino.h"
#अन्तर्भाव "targetdir.h"
#अन्तर्भाव "priv-set.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#परिभाषय क्रमादेशः_NAME "mv"

#परिभाषय AUTHORS \
  proper_name ("Mike Parker"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
गणना
{
  DEBUG_OPTION = CHAR_MAX + 1,
  EXCHANGE_OPTION,
  NO_COPY_OPTION,
  STRIP_TRAILING_SLASHES_OPTION
};

स्थिर वर्ण ध्रुव *ध्रुव update_type_string[] =
{
  "all", "none", "none-fail", "older", रिक्त
};
स्थिर गणना Update_type ध्रुव update_type[] =
{
  UPDATE_ALL, UPDATE_NONE, UPDATE_NONE_FAIL, UPDATE_OLDER,
};
ARGMATCH_VERIFY (update_type_string, update_type);

स्थिर संरचना option ध्रुव long_options[] =
{
  {"backup", optional_तर्कः, रिक्त, 'b'},
  {"context", no_तर्कः, रिक्त, 'Z'},
  {"debug", no_तर्कः, रिक्त, DEBUG_OPTION},
  {"exchange", no_तर्कः, रिक्त, EXCHANGE_OPTION},
  {"force", no_तर्कः, रिक्त, 'f'},
  {"interactive", no_तर्कः, रिक्त, 'i'},
  {"no-clobber", no_तर्कः, रिक्त, 'n'},   /* Deprecated.  */
  {"no-copy", no_तर्कः, रिक्त, NO_COPY_OPTION},
  {"no-target-directory", no_तर्कः, रिक्त, 'T'},
  {"strip-trailing-slashes", no_तर्कः, रिक्त,
   STRIP_TRAILING_SLASHES_OPTION},
  {"suffix", required_तर्कः, रिक्त, 'S'},
  {"target-directory", required_तर्कः, रिक्त, 't'},
  {"update", optional_तर्कः, रिक्त, 'u'},
  {"verbose", no_तर्कः, रिक्त, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_संस्करणम्_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

स्थिर शून्य
rm_option_init (संरचना rm_options *x)
{
  x->ignore_missing_files = false;
  x->remove_रिक्तानि_directories = true;
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
    स्थिर संरचना dev_ino dev_ino_buf;
    x->मूलम्_dev_ino = get_मूलम्_dev_ino (&dev_ino_buf);
    यदि (x->मूलम्_dev_ino == रिक्त)
      error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
             quoteaf ("/"));
  }

  x->preserve_all_मूलम् = false;
}

स्थिर शून्य
cp_option_init (संरचना cp_options *x)
{
  bool selinux_enabled = (0 < is_selinux_enabled ());

  cp_options_default (x);
  x->copy_as_regular = false;  /* FIXME: maybe make this an option */
  x->reflink_बहुलकम् = REFLINK_AUTO;
  x->dereference = DEREF_NEVER;
  x->unlink_dest_before_opening = false;
  x->unlink_dest_after_failed_open = false;
  x->hard_link = false;
  x->interactive = I_UNSPECIFIED;
  x->move_बहुलकम् = true;
  x->install_बहुलकम् = false;
  x->one_file_system = false;
  x->preserve_ownership = true;
  x->preserve_links = true;
  x->preserve_बहुलकम् = true;
  x->preserve_timestamps = true;
  x->explicit_no_preserve_बहुलकम्= false;
  x->preserve_security_context = selinux_enabled;
  x->समुच्चयः_security_context = रिक्त;
  x->reduce_diagnostics = false;
  x->दत्त_copy_required = true;
  x->require_preserve = false;  /* FIXME: maybe make this an option */
  x->require_preserve_context = false;
  x->preserve_xattr = true;
  x->require_preserve_xattr = true;
  x->recursive = true;
  x->sparse_बहुलकम् = SPARSE_AUTO;  /* FIXME: maybe make this an option */
  x->symbolic_link = false;
  x->समुच्चयः_बहुलकम् = false;
  x->बहुलकम्_2 = 0;
  x->stdin_tty = isatty (STDIN_FILENO);

  x->open_dangling_dest_symlink = false;
  x->update = UPDATE_ALL;
  x->verbose = false;
  x->dest_info = रिक्त;
  x->src_info = रिक्त;
}

/* Move SOURCE onto DEST aka DEST_DIRFD+DEST_RELNAME.
   Handle cross-file-system moves.
   If SOURCE is a directory, DEST must not exist.
   Return true if successful.  */

स्थिर bool
do_move (वर्ण ध्रुव *source, वर्ण ध्रुव *dest,
         पूर्णाङ्क dest_dirfd, वर्ण ध्रुव *dest_relname, ध्रुव संरचना cp_options *x)
{
  bool copy_into_self;
  bool rename_succeeded;
  bool ok = copy (source, dest, dest_dirfd, dest_relname, 0, x,
                  &copy_into_self, &rename_succeeded);

  यदि (ok)
    {
      वर्ण ध्रुव *dir_गन्तव्यम्_remove;
      यदि (copy_into_self)
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

          dir_गन्तव्यम्_remove = रिक्त;
          ok = false;
        }
      अन्यथा यदि (rename_succeeded)
        {
          /* No need to remove anything.  SOURCE was successfully
             renamed to DEST.  Or the user declined to rename a file.  */
          dir_गन्तव्यम्_remove = रिक्त;
        }
      अन्यथा
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

          dir_गन्तव्यम्_remove = source;
        }

      यदि (dir_गन्तव्यम्_remove != रिक्त)
        {
          संरचना rm_options rm_options;
          rm_option_init (&rm_options);
          rm_options.verbose = x->verbose;
          वर्ण ध्रुव *dir[2] = { dir_गन्तव्यम्_remove, रिक्त };

          गणना RM_स्थितिः स्थितिः = rm ((शून्य *) dir, &rm_options);
          affirm (VALID_स्थितिः (स्थितिः));
          यदि (स्थितिः == RM_ERROR)
            ok = false;
        }
    }

  प्रत्यागम ok;
}

शून्य
usage (पूर्णाङ्क स्थितिः)
{
  यदि (स्थितिः != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
    {
      प्रारूप_लिख (_("\
Usage: %s [OPTION]... [-T] SOURCE DEST\n\
  or:  %s [OPTION]... SOURCE... DIRECTORY\n\
  or:  %s [OPTION]... -t DIRECTORY SOURCE...\n\
"),
              क्रमादेशः_name, क्रमादेशः_name, क्रमादेशः_name);
      fputs (_("\
Rename SOURCE गन्तव्यम् DEST, or move SOURCE(s) गन्तव्यम् DIRECTORY.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
      --backup[=CONTROL]\n\
         make a backup सम्बन्धि each existing destination file\n\
"));
      oputs (_("\
  -b\n\
         like --backup but does not accept an तर्कः\n\
"));
      oputs (_("\
      --debug\n\
         explain how a file is copied.  उपपादयति_2 -v\n\
"));
      oputs (_("\
      --exchange\n\
         exchange source and destination\n\
"));
      oputs (_("\
  -f, --force\n\
         कुरु not prompt before overwriting\n\
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
If you specify अधिकम् than one सम्बन्धि -i, -f, -n, only the final one takes effect.\n\
"), stdout);
      oputs (_("\
      --no-copy\n\
         कुरु not copy यदि renaming fails\n\
"));
      oputs (_("\
      --strip-trailing-slashes\n\
         remove any trailing slashes from each SOURCE तर्कः\n\
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
         treat DEST as a प्रसामान्यम् file\n\
"));
      oputs (_("\
      --update[=UPDATE]\n\
         control which existing files are updated;\n\
         UPDATE={all,none,none-fail,older(स्वभाव)}\n\
"));
      oputs (_("\
  -u\n\
         equivalent गन्तव्यम् --update[=older].  See अधः\n\
"));
      oputs (_("\
  -v, --verbose\n\
         explain what is being समाप्तम्\n\
"));
      oputs (_("\
  -Z, --context\n\
         समुच्चयः_2 SELinux security context सम्बन्धि destination file गन्तव्यम् स्वभाव type\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (संस्करणम्_OPTION_DESCRIPTION);
      emit_update_parameters_note ();
      emit_backup_suffix_note ();
      emit_ancillary_info (क्रमादेशः_NAME);
    }
  निर्गच्छ (स्थितिः);
}

पूर्णाङ्क
मुख्यम् (पूर्णाङ्क argc, वर्ण **argv)
{
  bool make_backups = false;
  वर्ण ध्रुव *backup_suffix = रिक्त;
  वर्ण *संस्करणम्_control_string = रिक्त;
  संरचना cp_options x;
  bool remove_trailing_slashes = false;
  वर्ण ध्रुव *target_directory = रिक्त;
  bool no_target_directory = false;
  bool selinux_enabled = (0 < is_selinux_enabled ());

  initialize_main (&argc, &argv);
  समुच्चयः_क्रमादेशः_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdin);

  cp_option_init (&x);

  /* Try to disable the ability to unlink a directory.  */
  priv_समुच्चयः_remove_linkdir ();

  पूर्णाङ्क c;
  यावत् ((c = getopt_long (argc, argv, "bfint:uvS:TZ", long_options, रिक्त))
         != -1)
    {
      विकल्प (c)
        {
        पक्ष 'b':
          make_backups = true;
          यदि (optarg)
            संस्करणम्_control_string = optarg;
          भङ्ग;
        पक्ष 'f':
          x.interactive = I_ALWAYS_YES;
          भङ्ग;
        पक्ष 'i':
          x.interactive = I_ASK_USER;
          भङ्ग;
        पक्ष 'n':
          x.interactive = I_ALWAYS_SKIP;
          भङ्ग;
        पक्ष DEBUG_OPTION:
          x.debug = x.verbose = true;
          भङ्ग;
        पक्ष EXCHANGE_OPTION:
          x.exchange = true;
          भङ्ग;
        पक्ष NO_COPY_OPTION:
          x.no_copy = true;
          भङ्ग;
        पक्ष STRIP_TRAILING_SLASHES_OPTION:
          remove_trailing_slashes = true;
          भङ्ग;
        पक्ष 't':
          यदि (target_directory)
            error (EXIT_FAILURE, 0, _("multiple target directories specified"));
          target_directory = optarg;
          भङ्ग;
        पक्ष 'T':
          no_target_directory = true;
          भङ्ग;
        पक्ष 'u':
          x.update = UPDATE_OLDER;
          यदि (optarg)
            x.update = XARGMATCH ("--update", optarg,
                                  update_type_string, update_type);
          भङ्ग;
        पक्ष 'v':
          x.verbose = true;
          भङ्ग;
        पक्ष 'S':
          make_backups = true;
          backup_suffix = optarg;
          भङ्ग;
        पक्ष 'Z':
          /* As a performance enhancement, don't even bother trying
             to "restorecon" when not on an selinux-enabled kernel.  */
          यदि (selinux_enabled)
            {
              x.preserve_security_context = false;
              x.समुच्चयः_security_context = selabel_open (SELABEL_CTX_FILE,
                                                     रिक्त, 0);
              यदि (! x.समुच्चयः_security_context)
                error (0, errno, _("warning: ignoring --context"));
            }
          भङ्ग;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_संस्करणम्_CHAR (क्रमादेशः_NAME, AUTHORS);
        स्वभाव:
          usage (EXIT_FAILURE);
        }
    }

  पूर्णाङ्क n_files = argc - optind;
  वर्ण **file = argv + optind;

  यदि (n_files <= !target_directory)
    {
      यदि (n_files <= 0)
        error (0, 0, _("missing file operand"));
      अन्यथा
        error (0, 0, _("missing destination file operand after %s"),
               quoteaf (file[0]));
      usage (EXIT_FAILURE);
    }

  संरचना stat sb;
  sb.st_बहुलकम् = 0;
  पूर्णाङ्क target_dirfd = AT_FDCWD;
  यदि (no_target_directory)
    {
      यदि (target_directory)
        error (EXIT_FAILURE, 0,
               _("cannot combine --target-directory (-t) "
                 "and --no-target-directory (-T)"));
      यदि (2 < n_files)
        {
          error (0, 0, _("extra operand %s"), quoteaf (file[2]));
          usage (EXIT_FAILURE);
        }
    }
  अन्यथा यदि (target_directory)
    {
      target_dirfd = target_directory_सङ्कार्यम् (target_directory, &sb);
      यदि (! target_dirfd_valid (target_dirfd))
        error (EXIT_FAILURE, errno, _("target directory %s"),
               quoteaf (target_directory));
    }
  अन्यथा
    {
      वर्ण ध्रुव *lastfile = file[n_files - 1];
      यदि (n_files == 2 && !x.exchange)
        x.rename_errno = (renameatu (AT_FDCWD, file[0], AT_FDCWD, lastfile,
                                     RENAME_NOREPLACE)
                          ? errno : 0);
      यदि (x.rename_errno != 0)
        {
          पूर्णाङ्क fd = target_directory_सङ्कार्यम् (lastfile, &sb);
          यदि (target_dirfd_valid (fd))
            {
              x.rename_errno = -1;
              target_dirfd = fd;
              target_directory = lastfile;
              n_files--;
            }
          अन्यथा
            {
              /* The last operand LASTFILE cannot be opened as a directory.
                 If there are more than two operands, report an error.

                 Also, report an error if LASTFILE is known to be a directory
                 even though it could not be opened, which can happen if
                 opening failed with EACCES on a platform lacking O_PATH.
                 In this case use stat to test whether LASTFILE is a
                 directory, in case opening a non-directory with (O_SEARCH
                 | O_DIRECTORY) failed with EACCES not ENOTDIR.  */
              पूर्णाङ्क err = errno;
              यदि (2 < n_files
                  || (O_PATHSEARCH == O_अन्वेषणम् && err == EACCES
                      && (sb.st_बहुलकम् != 0 || stat (lastfile, &sb) == 0)
                      && S_ISDIR (sb.st_बहुलकम्)))
                error (EXIT_FAILURE, err, _("target %s"), quoteaf (lastfile));
            }
        }
    }

  /* Handle the ambiguity in the semantics of mv induced by the
     varying semantics of the rename function.  POSIX-compatible
     systems (e.g., GNU/Linux) have a rename function that honors a
     trailing slash in the source, while others (Solaris 9, FreeBSD
     7.2) have a rename function that ignores it.  */
  यदि (remove_trailing_slashes)
    प्रति (पूर्णाङ्क i = 0; i < n_files; i++)
      strip_trailing_slashes (file[i]);

  यदि (x.interactive == I_ALWAYS_SKIP)
    x.update = UPDATE_NONE;

  यदि (make_backups
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
                   ? xget_संस्करणम् (_("backup type"),
                                   संस्करणम्_control_string)
                   : no_backups);
  समुच्चयः_simple_backup_suffix (backup_suffix);

  hash_init ();

  bool ok;
  यदि (target_directory)
    {
      /* Initialize the hash table only if we'll need it.
         The problem it is used to detect can arise only if there are
         two or more files to move.  */
      यदि (2 <= n_files)
        dest_info_init (&x);

      ok = true;
      प्रति (पूर्णाङ्क i = 0; i < n_files; ++i)
        {
          x.चरमः_file = i + 1 == n_files;
          वर्ण ध्रुव *source = file[i];
          वर्ण ध्रुव *source_basename = चरमः_component (source);
          वर्ण *dest_relname;
          वर्ण *dest = file_name_concat (target_directory, source_basename,
                                         &dest_relname);
          strip_trailing_slashes (dest_relname);
          ok &= do_move (source, dest, target_dirfd, dest_relname, &x);
          मुक्त (dest);
        }
    }
  अन्यथा
    {
      x.चरमः_file = true;
      ok = do_move (file[0], file[1], AT_FDCWD, file[1], &x);
    }

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
