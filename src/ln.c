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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <getopt.h>

#অন্তর্ভাব "system.h"
#অন্তর্ভাব "backupfile.h"
#অন্তর্ভাব "fcntl-safer.h"
#অন্তর্ভাব "filenamecat.h"
#অন্তর্ভাব "file-set.h"
#অন্তর্ভাব "force-link.h"
#অন্তর্ভাব "hash.h"
#অন্তর্ভাব "hashcode-file.h"
#অন্তর্ভাব "priv-set.h"
#অন্তর্ভাব "relpath.h"
#অন্তর্ভাব "same.h"
#অন্তর্ভাব "unlinkdir.h"
#অন্তর্ভাব "yesno.h"
#অন্তর্ভাব "canonicalize.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "ln"

#define AUTHORS \
  proper_name ("Mike Parker"), \
  proper_name ("David MacKenzie")

/* FIXME: document */
স্থির গণনা backup_type backup_type;

/* If true, make symbolic links; otherwise, make hard links.  */
স্থির bool symbolic_link;

/* If true, make symbolic links relative  */
স্থির bool relative;

/* If true, hard links are logical rather than physical.  */
স্থির bool logical = !!LINK_FOLLOWS_SYMLINKS;

/* If true, ask the user before removing existing files.  */
স্থির bool interactive;

/* If true, remove existing files unconditionally.  */
স্থির bool remove_existing_files;

/* If true, list each file as it is moved. */
স্থির bool verbose;

/* If true, allow the superuser to *attempt* to make hard links
   to directories.  However, it appears that this option is not useful
   in practice, since even the superuser is prohibited from hard-linking
   directories on most existing systems (Solaris being an exception).  */
স্থির bool hard_dir_link;

/* If true, watch out for creating or removing hard links to directories.  */
স্থির bool beware_hard_dir_link;

/* If nonzero, and the specified destination is a symbolic link to a
   directory, treat it just as if it were a directory.  Otherwise, the
   command 'ln --force --no-dereference file symlink-to-dir' deletes
   symlink-to-dir before creating the new link.  */
স্থির bool dereference_dest_dir_symlinks = true;

/* This is a set of destination name/inode/dev triples for hard links
   created by ln.  Use this data structure to avoid data loss via a
   sequence of commands like this:
   rm -rf a b c; mkdir a b c; touch a/f b/f; ln -f a/f b/f c && rm -r a b */
স্থির Hash_table *dest_set;

/* Initial size of the dest_set hash table.  */
গণনা { DEST_INFO_INITIAL_CAPACITY = 61 };

স্থির কাঠামো option ধ্রুবক long_options[] =
{
  {"backup", optional_argument, ফাঁকা, 'b'},
  {"directory", no_argument, ফাঁকা, 'F'},
  {"no-dereference", no_argument, ফাঁকা, 'n'},
  {"no-target-directory", no_argument, ফাঁকা, 'T'},
  {"force", no_argument, ফাঁকা, 'f'},
  {"interactive", no_argument, ফাঁকা, 'i'},
  {"suffix", required_argument, ফাঁকা, 'S'},
  {"target-directory", required_argument, ফাঁকা, 't'},
  {"logical", no_argument, ফাঁকা, 'L'},
  {"physical", no_argument, ফাঁকা, 'P'},
  {"relative", no_argument, ফাঁকা, 'r'},
  {"symbolic", no_argument, ফাঁকা, 's'},
  {"verbose", no_argument, ফাঁকা, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ফাঁকা, 0, ফাঁকা, 0}
};

/* Return an errno value for a system call that returned STATUS.
   This is zero if STATUS is zero, and is errno otherwise.  */

স্থির পূর্ণসংখ্যা
errnoize (পূর্ণসংখ্যা status)
{
  ফেরত status < 0 ? errno : 0;
}

/* Return FROM represented as relative to the dir of TARGET.
   The result is malloced.  */

স্থির অক্ষর *
convert_abs_rel (অক্ষর ধ্রুবক *from, অক্ষর ধ্রুবক *target)
{
  /* Get dirname to generate paths relative to.  We don't resolve
     the full TARGET as the last component could be an existing symlink.  */
  অক্ষর *targetdir = dir_name (target);

  অক্ষর *realdest = canonicalize_filename_mode (targetdir, CAN_MISSING);
  অক্ষর *realfrom = canonicalize_filename_mode (from, CAN_MISSING);

  অক্ষর *relative_from = ফাঁকা;
  যদি (realdest && realfrom)
    {
      /* Write to a PATH_MAX buffer.  */
      relative_from = xmalloc (PATH_MAX);

      যদি (!relpath (realfrom, realdest, relative_from, PATH_MAX))
        {
          মুক্ত (relative_from);
          relative_from = ফাঁকা;
        }
    }

  মুক্ত (targetdir);
  মুক্ত (realdest);
  মুক্ত (realfrom);

  ফেরত relative_from ? relative_from : xstrdup (from);
}

/* Link SOURCE to DESTDIR_FD + DEST_BASE atomically.  DESTDIR_FD is
   the directory containing DEST_BASE.  Return 0 if successful, a
   positive errno value on failure, and -1 if an atomic link cannot be
   done.  This handles the common case where the destination does not
   already exist and -r is not specified.  */

স্থির পূর্ণসংখ্যা
atomic_link (অক্ষর ধ্রুবক *source, পূর্ণসংখ্যা destdir_fd, অক্ষর ধ্রুবক *dest_base)
{
  ফেরত (symbolic_link
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

স্থির bool
do_link (অক্ষর ধ্রুবক *source, পূর্ণসংখ্যা destdir_fd, অক্ষর ধ্রুবক *dest_base,
         অক্ষর ধ্রুবক *dest, পূর্ণসংখ্যা link_errno)
{
  কাঠামো stat source_stats;
  পূর্ণসংখ্যা source_status = 1;
  অক্ষর *backup_base = ফাঁকা;
  অক্ষর *rel_source = ফাঁকা;
  পূর্ণসংখ্যা nofollow_flag = logical ? 0 : AT_SYMLINK_NOFOLLOW;
  যদি (link_errno < 0)
    link_errno = atomic_link (source, destdir_fd, dest_base);

  /* Get SOURCE_STATS if later code will need it, if only for sharper
     diagnostics.  */
  যদি ((link_errno || dest_set) && !symbolic_link)
    {
      source_status = fstatat (AT_FDCWD, source, &source_stats, nofollow_flag);
      যদি (source_status != 0)
        {
          error (0, errno, _("failed to access %s"), quoteaf (source));
          ফেরত false;
        }
    }

  যদি (link_errno)
    {
      যদি (!symbolic_link && !hard_dir_link && S_ISDIR (source_stats.st_mode))
        {
          error (0, 0, _("%s: hard link not allowed for directory"),
                 quotef (source));
          ফেরত false;
        }

      যদি (relative)
        source = rel_source = convert_abs_rel (source, dest);

      bool force = (remove_existing_files || interactive
                    || backup_type != no_backups);
      যদি (force)
        {
          কাঠামো stat dest_stats;
          যদি (fstatat (destdir_fd, dest_base, &dest_stats, AT_SYMLINK_NOFOLLOW)
              != 0)
            {
              যদি (errno != ENOENT)
                {
                  error (0, errno, _("failed to access %s"), quoteaf (dest));
                  যাও fail;
                }
              force = false;
            }
          অন্যথা যদি (S_ISDIR (dest_stats.st_mode))
            {
              error (0, 0, _("%s: cannot overwrite directory"), quotef (dest));
              যাও fail;
            }
          অন্যথা যদি (seen_file (dest_set, dest, &dest_stats))
            {
              /* The current target was created as a hard link to another
                 source file.  */
              error (0, 0,
                     _("will not overwrite just-created %s with %s"),
                     quoteaf_n (0, dest), quoteaf_n (1, source));
              যাও fail;
            }
          অন্যথা
            {
              /* Beware removing DEST if it is the same directory entry as
                 SOURCE, because in that case removing DEST can cause the
                 subsequent link creation either to fail (for hard links), or
                 to replace a non-symlink DEST with a self-loop (for symbolic
                 links) which loses the contents of DEST.  So, when backing
                 up, worry about creating hard links (since the backups cover
                 the symlink case); otherwise, worry about about -f.  */
              যদি (backup_type != no_backups
                  ? !symbolic_link
                  : remove_existing_files)
                {
                  /* Detect whether removing DEST would also remove SOURCE.
                     If the file has only one link then both are surely the
                     same directory entry.  Otherwise check whether they point
                     to the same name in the same directory.  */
                  যদি (source_status != 0)
                    source_status = stat (source, &source_stats);
                  যদি (source_status == 0
                      && psame_inode (&source_stats, &dest_stats)
                      && (source_stats.st_nlink == 1
                          || same_nameat (AT_FDCWD, source,
                                          destdir_fd, dest_base)))
                    {
                      error (0, 0, _("%s and %s are the same file"),
                             quoteaf_n (0, source), quoteaf_n (1, dest));
                      যাও fail;
                    }
                }

              যদি (link_errno < 0 || link_errno == EEXIST)
                {
                  যদি (interactive)
                    {
                      fprintf (stderr, _("%s: replace %s? "),
                               program_name, quoteaf (dest));
                      যদি (!yesno ())
                        {
                          মুক্ত (rel_source);
                          ফেরত false;
                        }
                    }

                  যদি (backup_type != no_backups)
                    {
                      backup_base = find_backup_file_name (destdir_fd,
                                                           dest_base,
                                                           backup_type);
                      যদি (renameat (destdir_fd, dest_base,
                                    destdir_fd, backup_base)
                          != 0)
                        {
                          পূর্ণসংখ্যা rename_errno = errno;
                          মুক্ত (backup_base);
                          backup_base = ফাঁকা;
                          যদি (rename_errno != ENOENT)
                            {
                              error (0, rename_errno, _("cannot backup %s"),
                                     quoteaf (dest));
                              যাও fail;
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

  যদি (link_errno <= 0)
    {
      /* Right after creating a hard link, do this: (note dest name and
         source_stats, which are also the just-linked-destinations stats) */
      যদি (! symbolic_link)
        record_file (dest_set, dest, &source_stats);

      যদি (verbose)
        {
          অক্ষর ধ্রুবক *quoted_backup = "";
          অক্ষর ধ্রুবক *backup_sep = "";
          যদি (backup_base)
            {
              অক্ষর *backup = backup_base;
              শূন্য *alloc = ফাঁকা;
              ptrdiff_t destdirlen = dest_base - dest;
              যদি (0 < destdirlen)
                {
                  alloc = xmalloc (destdirlen + strlen (backup_base) + 1);
                  backup = memcpy (alloc, dest, destdirlen);
                  strcpy (backup + destdirlen, backup_base);
                }
              quoted_backup = quoteaf_n (2, backup);
              backup_sep = " ~ ";
              মুক্ত (alloc);
            }
          printf ("%s%s%s %c> %s\n", quoted_backup, backup_sep,
                  quoteaf_n (0, dest), symbolic_link ? '-' : '=',
                  quoteaf_n (1, source));
        }
    }
  অন্যথা
    {
      অক্ষর *dest_quoted = quoteaf_n (0, dest);
      অক্ষর *source_quoted = quoteaf_n (1, source);

      যদি (symbolic_link)
        {
          যদি (link_errno != ENAMETOOLONG && *source)
            error (0, link_errno, _("failed to create symbolic link %s"),
                   dest_quoted);
          অন্যথা
            error (0, link_errno, _("failed to create symbolic link %s -> %s"),
                   dest_quoted, source_quoted);
        }
      অন্যথা
        {
          যদি (link_errno == EMLINK)
            error (0, link_errno, _("failed to create hard link to %s"),
                   source_quoted);
          অন্যথা যদি (link_errno == EDQUOT || link_errno == EEXIST
                   || link_errno == ENOSPC || link_errno == EROFS)
            error (0, link_errno, _("failed to create hard link %s"),
                   dest_quoted);
          অন্যথা
            error (0, link_errno, _("failed to create hard link %s => %s"),
                   dest_quoted, source_quoted);
        }

      যদি (backup_base)
        {
          যদি (renameat (destdir_fd, backup_base, destdir_fd, dest_base) != 0)
            error (0, errno, _("cannot un-backup %s"), quoteaf (dest));
        }
    }

  মুক্ত (backup_base);
  মুক্ত (rel_source);
  ফেরত link_errno <= 0;

fail:
  মুক্ত (rel_source);
  ফেরত false;
}

শূন্য
usage (পূর্ণসংখ্যা status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
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
Create hard links by পূর্বনির্ধারিত, symbolic links with --symbolic.\n\
By পূর্বনির্ধারিত, each destination (name of new link) should not already exist.\n\
When creating hard links, each TARGET must exist.  Symbolic links\n\
can hold arbitrary text; যদি later resolved, a relative link is\n\
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
         যদি supported by the system\n\
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
         যদি it is a symbolic link to a directory\n\
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

পূর্ণসংখ্যা
প্রধান (পূর্ণসংখ্যা argc, অক্ষর **argv)
{
  পূর্ণসংখ্যা c;
  bool ok;
  bool make_backups = false;
  অক্ষর ধ্রুবক *backup_suffix = ফাঁকা;
  অক্ষর *version_control_string = ফাঁকা;
  অক্ষর ধ্রুবক *target_directory = ফাঁকা;
  পূর্ণসংখ্যা destdir_fd;
  bool no_target_directory = false;
  পূর্ণসংখ্যা n_files;
  অক্ষর **file;
  পূর্ণসংখ্যা link_errno = -1;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdin);

  যখন ((c = getopt_long (argc, argv, "bdfinrst:vFLPS:T",
                           long_options, ফাঁকা))
         != -1)
    {
      নির্বাচন (c)
        {
        ক্ষেত্র 'b':
          make_backups = true;
          যদি (optarg)
            version_control_string = optarg;
          বিরতি;
        ক্ষেত্র 'd':
        ক্ষেত্র 'F':
          hard_dir_link = true;
          বিরতি;
        ক্ষেত্র 'f':
          remove_existing_files = true;
          interactive = false;
          বিরতি;
        ক্ষেত্র 'i':
          remove_existing_files = false;
          interactive = true;
          বিরতি;
        ক্ষেত্র 'L':
          logical = true;
          বিরতি;
        ক্ষেত্র 'n':
          dereference_dest_dir_symlinks = false;
          বিরতি;
        ক্ষেত্র 'P':
          logical = false;
          বিরতি;
        ক্ষেত্র 'r':
          relative = true;
          বিরতি;
        ক্ষেত্র 's':
          symbolic_link = true;
          বিরতি;
        ক্ষেত্র 't':
          যদি (target_directory)
            error (EXIT_FAILURE, 0, _("multiple target directories specified"));
          অন্যথা
            {
              কাঠামো stat st;
              যদি (stat (optarg, &st) != 0)
                error (EXIT_FAILURE, errno, _("failed to access %s"),
                       quoteaf (optarg));
              যদি (! S_ISDIR (st.st_mode))
                error (EXIT_FAILURE, 0, _("target %s is not a directory"),
                       quoteaf (optarg));
            }
          target_directory = optarg;
          বিরতি;
        ক্ষেত্র 'T':
          no_target_directory = true;
          বিরতি;
        ক্ষেত্র 'v':
          verbose = true;
          বিরতি;
        ক্ষেত্র 'S':
          make_backups = true;
          backup_suffix = optarg;
          বিরতি;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        পূর্বনির্ধারিত:
          usage (EXIT_FAILURE);
          বিরতি;
        }
    }

  n_files = argc - optind;
  file = argv + optind;

  যদি (n_files <= 0)
    {
      error (0, 0, _("missing file operand"));
      usage (EXIT_FAILURE);
    }

  যদি (relative && !symbolic_link)
    error (EXIT_FAILURE, 0, _("cannot do --relative without --symbolic"));

  যদি (!hard_dir_link)
    {
      priv_set_remove_linkdir ();
      beware_hard_dir_link = !cannot_unlink_dir ();
    }

  যদি (no_target_directory)
    {
      যদি (target_directory)
        error (EXIT_FAILURE, 0,
               _("cannot combine --target-directory "
                 "and --no-target-directory"));
      যদি (n_files != 2)
        {
          যদি (n_files < 2)
            error (0, 0,
                   _("missing destination file operand after %s"),
                   quoteaf (file[0]));
          অন্যথা
            error (0, 0, _("extra operand %s"), quoteaf (file[2]));
          usage (EXIT_FAILURE);
        }
    }
  অন্যথা যদি (n_files < 2 && !target_directory)
    {
      target_directory = ".";
      destdir_fd = AT_FDCWD;
    }
  অন্যথা
    {
      যদি (n_files == 2 && !target_directory)
        link_errno = atomic_link (file[0], AT_FDCWD, file[1]);
      যদি (link_errno < 0 || link_errno == EEXIST || link_errno == ENOTDIR
          || link_errno == EINVAL)
        {
          অক্ষর ধ্রুবক *d
            = target_directory ? target_directory : file[n_files - 1];
          পূর্ণসংখ্যা flags = (O_PATHSEARCH | O_DIRECTORY
                       | (dereference_dest_dir_symlinks ? 0 : O_NOFOLLOW));
          destdir_fd = openat_safer (AT_FDCWD, d, flags);
          যদি (0 <= destdir_fd)
            {
              n_files -= !target_directory;
              target_directory = d;
            }
          অন্যথা যদি (! (n_files == 2 && !target_directory))
            error (EXIT_FAILURE, errno, _("target %s"), quoteaf (d));
        }
    }

  backup_type = (make_backups
                 ? xget_version (_("backup type"), version_control_string)
                 : no_backups);
  set_simple_backup_suffix (backup_suffix);


  যদি (target_directory)
    {
      /* Create the data structure we'll use to record which hard links we
         create.  Used to ensure that ln detects an obscure corner case that
         might result in user data loss.  Create it only if needed.  */
      যদি (2 <= n_files
          && remove_existing_files
          /* Don't bother trying to protect symlinks, since ln clobbering
             a just-created symlink won't ever lead to real data loss.  */
          && ! symbolic_link
          /* No destination hard link can be clobbered when making
             numbered backups.  */
          && backup_type != numbered_backups)
        {
          dest_set = hash_initialize (DEST_INFO_INITIAL_CAPACITY,
                                      ফাঁকা,
                                      triple_hash,
                                      triple_compare,
                                      triple_free);
          যদি (dest_set == ফাঁকা)
            xalloc_die ();
        }

      ok = true;
      জন্য (পূর্ণসংখ্যা i = 0; i < n_files; ++i)
        {
          অক্ষর *dest_base;
          অক্ষর *dest = file_name_concat (target_directory,
                                         last_component (file[i]),
                                         &dest_base);
          strip_trailing_slashes (dest_base);
          ok &= do_link (file[i], destdir_fd, dest_base, dest, -1);
          মুক্ত (dest);
        }
    }
  অন্যথা
    ok = do_link (file[0], AT_FDCWD, file[1], file[1], link_errno);

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
