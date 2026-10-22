/* 'ln' program to create links between files.
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

/* Written by Mike Parker and David MacKenzie. */

#شامل <config.h>
#شامل <stdio.h>
#شامل <sys/types.h>
#شامل <getopt.h>

#شامل "system.h"
#شامل "backupfile.h"
#شامل "fcntl-safer.h"
#شامل "filenamecat.h"
#شامل "file-set.h"
#شامل "force-link.h"
#شامل "hash.h"
#شامل "hashcode-file.h"
#شامل "priv-set.h"
#شامل "relpath.h"
#شامل "same.h"
#شامل "unlinkdir.h"
#شامل "yesno.h"
#شامل "canonicalize.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "ln"

#define AUTHORS \
  proper_name ("Mike Parker"), \
  proper_name ("David MacKenzie")

/* FIXME: document */
ایستا شمارش backup_type backup_type;

/* If true, make symbolic links; otherwise, make hard links.  */
ایستا bool symbolic_link;

/* If true, make symbolic links relative  */
ایستا bool relative;

/* If true, hard links are logical rather than physical.  */
ایستا bool logical = !!LINK_FOLLOWS_SYMLINKS;

/* If true, ask the user before removing existing files.  */
ایستا bool interactive;

/* If true, remove existing files unconditionally.  */
ایستا bool remove_existing_files;

/* If true, list each file as it is moved. */
ایستا bool verbose;

/* If true, allow the superuser to *attempt* to make hard links
   to directories.  However, it appears that this option is not useful
   in practice, since even the superuser is prohibited from hard-linking
   directories on most existing systems (Solaris being an exception).  */
ایستا bool hard_dir_link;

/* If true, watch out for creating or removing hard links to directories.  */
ایستا bool beware_hard_dir_link;

/* If nonzero, and the specified destination is a symbolic link to a
   directory, treat it just as if it were a directory.  Otherwise, the
   command 'ln --force --no-dereference file symlink-to-dir' deletes
   symlink-to-dir before creating the new link.  */
ایستا bool dereference_dest_dir_symlinks = true;

/* This is a set of destination name/inode/dev triples for hard links
   created by ln.  Use this data structure to avoid data loss via a
   sequence of commands like this:
   rm -rf a b c; mkdir a b c; touch a/f b/f; ln -f a/f b/f c && rm -r a b */
ایستا Hash_table *dest_set;

/* Initial size of the dest_set hash table.  */
شمارش { DEST_INFO_INITIAL_CAPACITY = 61 };

ایستا ساختار option ثابت long_options[] =
{
  {"backup", optional_argument, پوچ, 'b'},
  {"directory", no_argument, پوچ, 'F'},
  {"no-dereference", no_argument, پوچ, 'n'},
  {"no-target-directory", no_argument, پوچ, 'T'},
  {"force", no_argument, پوچ, 'f'},
  {"interactive", no_argument, پوچ, 'i'},
  {"suffix", required_argument, پوچ, 'S'},
  {"target-directory", required_argument, پوچ, 't'},
  {"logical", no_argument, پوچ, 'L'},
  {"physical", no_argument, پوچ, 'P'},
  {"relative", no_argument, پوچ, 'r'},
  {"symbolic", no_argument, پوچ, 's'},
  {"verbose", no_argument, پوچ, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {پوچ, 0, پوچ, 0}
};

/* Return an errno value for a system call that returned STATUS.
   This is zero if STATUS is zero, and is errno otherwise.  */

ایستا صحیح
errnoize (صحیح status)
{
  بازگشت status < 0 ? errno : 0;
}

/* Return FROM represented as relative to the dir of TARGET.
   The result is malloced.  */

ایستا نویسه *
convert_abs_rel (نویسه ثابت *from, نویسه ثابت *target)
{
  /* Get dirname to generate paths relative to.  We don't resolve
     the full TARGET as the last component could be an existing symlink.  */
  نویسه *targetdir = dir_name (target);

  نویسه *realdest = canonicalize_filename_mode (targetdir, CAN_MISSING);
  نویسه *realfrom = canonicalize_filename_mode (from, CAN_MISSING);

  نویسه *relative_from = پوچ;
  اگر (realdest && realfrom)
    {
      /* Write to a PATH_MAX buffer.  */
      relative_from = xmalloc (PATH_MAX);

      اگر (!relpath (realfrom, realdest, relative_from, PATH_MAX))
        {
          آزاد (relative_from);
          relative_from = پوچ;
        }
    }

  آزاد (targetdir);
  آزاد (realdest);
  آزاد (realfrom);

  بازگشت relative_from ? relative_from : xstrdup (from);
}

/* Link SOURCE to DESTDIR_FD + DEST_BASE atomically.  DESTDIR_FD is
   the directory containing DEST_BASE.  Return 0 if successful, a
   positive errno value on failure, and -1 if an atomic link cannot be
   done.  This handles the common case where the destination does not
   already exist and -r is not specified.  */

ایستا صحیح
atomic_link (نویسه ثابت *source, صحیح destdir_fd, نویسه ثابت *dest_base)
{
  بازگشت (symbolic_link
          ? (relative ? -1
             : errnoize (symlinkat (source, destdir_fd, dest_base)))
          : beware_hard_dir_link ? -1
          : errnoize (linkat (AT_FDCWD, source, destdir_fd, dest_base,
                              logical ? AT_SYMLINK_FOLLOW : 0)));
}

/* Link SOURCE to a directory entry under DESTDIR_FD named DEST_BASE.
   DEST is the full name of the destination, useful for diagnostics.
   LINK_ERRNO is zero if the link has already been made,
   positive if attempting the link failed with errno == LINK_ERRNO,
   -1 if no attempt has been made to create the link.
   Return true if successful.  */

ایستا bool
do_link (نویسه ثابت *source, صحیح destdir_fd, نویسه ثابت *dest_base,
         نویسه ثابت *dest, صحیح link_errno)
{
  ساختار stat source_stats;
  صحیح source_status = 1;
  نویسه *backup_base = پوچ;
  نویسه *rel_source = پوچ;
  صحیح nofollow_flag = logical ? 0 : AT_SYMLINK_NOFOLLOW;
  اگر (link_errno < 0)
    link_errno = atomic_link (source, destdir_fd, dest_base);

  /* Get SOURCE_STATS if later code will need it, if only for sharper
     diagnostics.  */
  اگر ((link_errno || dest_set) && !symbolic_link)
    {
      source_status = fstatat (AT_FDCWD, source, &source_stats, nofollow_flag);
      اگر (source_status != 0)
        {
          error (0, errno, _("failed to access %s"), quoteaf (source));
          بازگشت false;
        }
    }

  اگر (link_errno)
    {
      اگر (!symbolic_link && !hard_dir_link && S_ISDIR (source_stats.st_mode))
        {
          error (0, 0, _("%s: hard link not allowed for directory"),
                 quotef (source));
          بازگشت false;
        }

      اگر (relative)
        source = rel_source = convert_abs_rel (source, dest);

      bool force = (remove_existing_files || interactive
                    || backup_type != no_backups);
      اگر (force)
        {
          ساختار stat dest_stats;
          اگر (fstatat (destdir_fd, dest_base, &dest_stats, AT_SYMLINK_NOFOLLOW)
              != 0)
            {
              اگر (errno != ENOENT)
                {
                  error (0, errno, _("failed to access %s"), quoteaf (dest));
                  برو fail;
                }
              force = false;
            }
          وگرنه اگر (S_ISDIR (dest_stats.st_mode))
            {
              error (0, 0, _("%s: cannot overwrite directory"), quotef (dest));
              برو fail;
            }
          وگرنه اگر (seen_file (dest_set, dest, &dest_stats))
            {
              /* The current target was created as a hard link to another
                 source file.  */
              error (0, 0,
                     _("will not overwrite just-created %s with %s"),
                     quoteaf_n (0, dest), quoteaf_n (1, source));
              برو fail;
            }
          وگرنه
            {
              /* Beware removing DEST if it is the same directory entry as
                 SOURCE, because in that case removing DEST can cause the
                 subsequent link creation either to fail (for hard links), or
                 to replace a non-symlink DEST with a self-loop (for symbolic
                 links) which loses the contents of DEST.  So, when backing
                 up, worry about creating hard links (since the backups cover
                 the symlink case); otherwise, worry about about -f.  */
              اگر (backup_type != no_backups
                  ? !symbolic_link
                  : remove_existing_files)
                {
                  /* Detect whether removing DEST would also remove SOURCE.
                     If the file has only one link then both are surely the
                     same directory entry.  Otherwise check whether they point
                     to the same name in the same directory.  */
                  اگر (source_status != 0)
                    source_status = stat (source, &source_stats);
                  اگر (source_status == 0
                      && psame_inode (&source_stats, &dest_stats)
                      && (source_stats.st_nlink == 1
                          || same_nameat (AT_FDCWD, source,
                                          destdir_fd, dest_base)))
                    {
                      error (0, 0, _("%s and %s are the same file"),
                             quoteaf_n (0, source), quoteaf_n (1, dest));
                      برو fail;
                    }
                }

              اگر (link_errno < 0 || link_errno == EEXIST)
                {
                  اگر (interactive)
                    {
                      fprintf (stderr, _("%s: replace %s? "),
                               program_name, quoteaf (dest));
                      اگر (!yesno ())
                        {
                          آزاد (rel_source);
                          بازگشت false;
                        }
                    }

                  اگر (backup_type != no_backups)
                    {
                      backup_base = find_backup_file_name (destdir_fd,
                                                           dest_base,
                                                           backup_type);
                      اگر (renameat (destdir_fd, dest_base,
                                    destdir_fd, backup_base)
                          != 0)
                        {
                          صحیح rename_errno = errno;
                          آزاد (backup_base);
                          backup_base = پوچ;
                          اگر (rename_errno != ENOENT)
                            {
                              error (0, rename_errno, _("cannot backup %s"),
                                     quoteaf (dest));
                              برو fail;
                            }
                          force = false;
                        }
                    }
                }
            }
        }

      /* If the attempt to create a link fails and we are removing or
         backing up destinations, unlink the destination and try again.

         On the surface, POSIX states that 'ln -f A B' unlinks B before trying
         to link A to B.  But strictly following this has the counterintuitive
         effect of losing the contents of B if A does not exist.  Fortunately,
         POSIX 2008 clarified that an application is free to fail early if it
         can prove that continuing onward cannot succeed, so we can try to
         link A to B before blindly unlinking B, thus sometimes attempting to
         link a second time during a successful 'ln -f A B'.

         Try to unlink DEST even if we may have backed it up successfully.
         In some unusual cases (when DEST and the backup are hard-links
         that refer to the same file), rename succeeds and DEST remains.
         If we didn't remove DEST in that case, the subsequent symlink or
         link call would fail.  */
      link_errno
        = (symbolic_link
           ? force_symlinkat (source, destdir_fd, dest_base,
                              force, link_errno)
           : force_linkat (AT_FDCWD, source, destdir_fd, dest_base,
                           logical ? AT_SYMLINK_FOLLOW : 0,
                           force, link_errno));
      /* Until now, link_errno < 0 meant the link has not been tried.
         From here on, link_errno < 0 means the link worked but
         required removing the destination first.  */
    }

  اگر (link_errno <= 0)
    {
      /* Right after creating a hard link, do this: (note dest name and
         source_stats, which are also the just-linked-destinations stats) */
      اگر (! symbolic_link)
        record_file (dest_set, dest, &source_stats);

      اگر (verbose)
        {
          نویسه ثابت *quoted_backup = "";
          نویسه ثابت *backup_sep = "";
          اگر (backup_base)
            {
              نویسه *backup = backup_base;
              تهی *alloc = پوچ;
              ptrdiff_t destdirlen = dest_base - dest;
              اگر (0 < destdirlen)
                {
                  alloc = xmalloc (destdirlen + strlen (backup_base) + 1);
                  backup = memcpy (alloc, dest, destdirlen);
                  strcpy (backup + destdirlen, backup_base);
                }
              quoted_backup = quoteaf_n (2, backup);
              backup_sep = " ~ ";
              آزاد (alloc);
            }
          printf ("%s%s%s %c> %s\n", quoted_backup, backup_sep,
                  quoteaf_n (0, dest), symbolic_link ? '-' : '=',
                  quoteaf_n (1, source));
        }
    }
  وگرنه
    {
      نویسه *dest_quoted = quoteaf_n (0, dest);
      نویسه *source_quoted = quoteaf_n (1, source);

      اگر (symbolic_link)
        {
          اگر (link_errno != ENAMETOOLONG && *source)
            error (0, link_errno, _("failed to create symbolic link %s"),
                   dest_quoted);
          وگرنه
            error (0, link_errno, _("failed to create symbolic link %s -> %s"),
                   dest_quoted, source_quoted);
        }
      وگرنه
        {
          اگر (link_errno == EMLINK)
            error (0, link_errno, _("failed to create hard link to %s"),
                   source_quoted);
          وگرنه اگر (link_errno == EDQUOT || link_errno == EEXIST
                   || link_errno == ENOSPC || link_errno == EROFS)
            error (0, link_errno, _("failed to create hard link %s"),
                   dest_quoted);
          وگرنه
            error (0, link_errno, _("failed to create hard link %s => %s"),
                   dest_quoted, source_quoted);
        }

      اگر (backup_base)
        {
          اگر (renameat (destdir_fd, backup_base, destdir_fd, dest_base) != 0)
            error (0, errno, _("cannot un-backup %s"), quoteaf (dest));
        }
    }

  آزاد (backup_base);
  آزاد (rel_source);
  بازگشت link_errno <= 0;

fail:
  آزاد (rel_source);
  بازگشت false;
}

تهی
usage (صحیح status)
{
  اگر (status != EXIT_SUCCESS)
    emit_try_help ();
  وگرنه
    {
      printf (_("\
Usage: %s [OPTION]... [-T] TARGET LINK_NAME\n\
  or:  %s [OPTION]... TARGET\n\
  or:  %s [OPTION]... TARGET... DIRECTORY\n\
  or:  %s [OPTION]... -t DIRECTORY TARGET...\n\
"),
              program_name, program_name, program_name, program_name);
      fputs (_("\
In the 1st form, create a link to TARGET with the name LINK_NAME.\n\
In the 2nd form, create a link to TARGET in the current directory.\n\
In the 3rd and 4th forms, create links to each TARGET in DIRECTORY.\n\
Create hard links by پیشفرض, symbolic links with --symbolic.\n\
By پیشفرض, each destination (name of new link) should not already exist.\n\
When creating hard links, each TARGET must exist.  Symbolic links\n\
can hold arbitrary text; اگر later resolved, a relative link is\n\
interpreted in relation to its parent directory.\n\
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
  -d, -F, --directory\n\
         allow the superuser to attempt to hard link directories,\n\
         اگر supported by the system\n\
"));
      oputs (_("\
  -f, --force\n\
         remove existing destination files\n\
"));
      oputs (_("\
  -i, --interactive\n\
         prompt whether to remove destinations\n\
"));
      oputs (_("\
  -L, --logical\n\
         dereference TARGETs that are symbolic links\n\
"));
      oputs (_("\
  -n, --no-dereference\n\
         treat LINK_NAME as a normal file\n\
         اگر it is a symbolic link to a directory\n\
"));
      oputs (_("\
  -P, --physical\n\
         make hard links directly to symbolic links\n\
"));
      oputs (_("\
  -r, --relative\n\
         with -s, create links relative to link location\n\
"));
      oputs (_("\
  -s, --symbolic\n\
         make symbolic links instead of hard links\n\
"));
      oputs (_("\
  -S, --suffix=SUFFIX\n\
         override the usual backup suffix\n\
"));
      oputs (_("\
  -t, --target-directory=DIRECTORY\n\
         specify the DIRECTORY in which to create the links\n\
"));
      oputs (_("\
  -T, --no-target-directory\n\
         treat LINK_NAME as a normal file always\n\
"));
      oputs (_("\
  -v, --verbose\n\
         print name of each linked file\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_backup_suffix_note ();
      printf (_("\
\n\
Using -s ignores -L and -P.  Otherwise, the last option specified controls\n\
behavior when a TARGET is a symbolic link, defaulting to %s.\n\
"), LINK_FOLLOWS_SYMLINKS ? "-L" : "-P");
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

صحیح
اصلی (صحیح argc, نویسه **argv)
{
  صحیح c;
  bool ok;
  bool make_backups = false;
  نویسه ثابت *backup_suffix = پوچ;
  نویسه *version_control_string = پوچ;
  نویسه ثابت *target_directory = پوچ;
  صحیح destdir_fd;
  bool no_target_directory = false;
  صحیح n_files;
  نویسه **file;
  صحیح link_errno = -1;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdin);

  وقتی ((c = getopt_long (argc, argv, "bdfinrst:vFLPS:T",
                           long_options, پوچ))
         != -1)
    {
      انتخاب (c)
        {
        حالت 'b':
          make_backups = true;
          اگر (optarg)
            version_control_string = optarg;
          بشکن;
        حالت 'd':
        حالت 'F':
          hard_dir_link = true;
          بشکن;
        حالت 'f':
          remove_existing_files = true;
          interactive = false;
          بشکن;
        حالت 'i':
          remove_existing_files = false;
          interactive = true;
          بشکن;
        حالت 'L':
          logical = true;
          بشکن;
        حالت 'n':
          dereference_dest_dir_symlinks = false;
          بشکن;
        حالت 'P':
          logical = false;
          بشکن;
        حالت 'r':
          relative = true;
          بشکن;
        حالت 's':
          symbolic_link = true;
          بشکن;
        حالت 't':
          اگر (target_directory)
            error (EXIT_FAILURE, 0, _("multiple target directories specified"));
          وگرنه
            {
              ساختار stat st;
              اگر (stat (optarg, &st) != 0)
                error (EXIT_FAILURE, errno, _("failed to access %s"),
                       quoteaf (optarg));
              اگر (! S_ISDIR (st.st_mode))
                error (EXIT_FAILURE, 0, _("target %s is not a directory"),
                       quoteaf (optarg));
            }
          target_directory = optarg;
          بشکن;
        حالت 'T':
          no_target_directory = true;
          بشکن;
        حالت 'v':
          verbose = true;
          بشکن;
        حالت 'S':
          make_backups = true;
          backup_suffix = optarg;
          بشکن;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        پیشفرض:
          usage (EXIT_FAILURE);
          بشکن;
        }
    }

  n_files = argc - optind;
  file = argv + optind;

  اگر (n_files <= 0)
    {
      error (0, 0, _("missing file operand"));
      usage (EXIT_FAILURE);
    }

  اگر (relative && !symbolic_link)
    error (EXIT_FAILURE, 0, _("cannot do --relative without --symbolic"));

  اگر (!hard_dir_link)
    {
      priv_set_remove_linkdir ();
      beware_hard_dir_link = !cannot_unlink_dir ();
    }

  اگر (no_target_directory)
    {
      اگر (target_directory)
        error (EXIT_FAILURE, 0,
               _("cannot combine --target-directory "
                 "and --no-target-directory"));
      اگر (n_files != 2)
        {
          اگر (n_files < 2)
            error (0, 0,
                   _("missing destination file operand after %s"),
                   quoteaf (file[0]));
          وگرنه
            error (0, 0, _("extra operand %s"), quoteaf (file[2]));
          usage (EXIT_FAILURE);
        }
    }
  وگرنه اگر (n_files < 2 && !target_directory)
    {
      target_directory = ".";
      destdir_fd = AT_FDCWD;
    }
  وگرنه
    {
      اگر (n_files == 2 && !target_directory)
        link_errno = atomic_link (file[0], AT_FDCWD, file[1]);
      اگر (link_errno < 0 || link_errno == EEXIST || link_errno == ENOTDIR
          || link_errno == EINVAL)
        {
          نویسه ثابت *d
            = target_directory ? target_directory : file[n_files - 1];
          صحیح flags = (O_PATHSEARCH | O_DIRECTORY
                       | (dereference_dest_dir_symlinks ? 0 : O_NOFOLLOW));
          destdir_fd = openat_safer (AT_FDCWD, d, flags);
          اگر (0 <= destdir_fd)
            {
              n_files -= !target_directory;
              target_directory = d;
            }
          وگرنه اگر (! (n_files == 2 && !target_directory))
            error (EXIT_FAILURE, errno, _("target %s"), quoteaf (d));
        }
    }

  backup_type = (make_backups
                 ? xget_version (_("backup type"), version_control_string)
                 : no_backups);
  set_simple_backup_suffix (backup_suffix);


  اگر (target_directory)
    {
      /* Create the data structure we'll use to record which hard links we
         create.  Used to ensure that ln detects an obscure corner case that
         might result in user data loss.  Create it only if needed.  */
      اگر (2 <= n_files
          && remove_existing_files
          /* Don't bother trying to protect symlinks, since ln clobbering
             a just-created symlink won't ever lead to real data loss.  */
          && ! symbolic_link
          /* No destination hard link can be clobbered when making
             numbered backups.  */
          && backup_type != numbered_backups)
        {
          dest_set = hash_initialize (DEST_INFO_INITIAL_CAPACITY,
                                      پوچ,
                                      triple_hash,
                                      triple_compare,
                                      triple_free);
          اگر (dest_set == پوچ)
            xalloc_die ();
        }

      ok = true;
      برای (صحیح i = 0; i < n_files; ++i)
        {
          نویسه *dest_base;
          نویسه *dest = file_name_concat (target_directory,
                                         last_component (file[i]),
                                         &dest_base);
          strip_trailing_slashes (dest_base);
          ok &= do_link (file[i], destdir_fd, dest_base, dest, -1);
          آزاد (dest);
        }
    }
  وگرنه
    ok = do_link (file[0], AT_FDCWD, file[1], file[1], link_errno);

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
