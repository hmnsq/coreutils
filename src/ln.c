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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "backupfile.h"
#ಅನ್ತರ್ಭಾವ "fcntl-safer.h"
#ಅನ್ತರ್ಭಾವ "filenamecat.h"
#ಅನ್ತರ್ಭಾವ "file-set.h"
#ಅನ್ತರ್ಭಾವ "force-link.h"
#ಅನ್ತರ್ಭಾವ "hash.h"
#ಅನ್ತರ್ಭಾವ "hashcode-file.h"
#ಅನ್ತರ್ಭಾವ "priv-set.h"
#ಅನ್ತರ್ಭಾವ "relpath.h"
#ಅನ್ತರ್ಭಾವ "same.h"
#ಅನ್ತರ್ಭಾವ "unlinkdir.h"
#ಅನ್ತರ್ಭಾವ "yesno.h"
#ಅನ್ತರ್ಭಾವ "canonicalize.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "ln"

#define AUTHORS \
  proper_name ("Mike Parker"), \
  proper_name ("David MacKenzie")

/* FIXME: document */
ಸ್ಥಿರ ಗಣನಾ backup_type backup_type;

/* If true, make symbolic links; otherwise, make hard links.  */
ಸ್ಥಿರ bool symbolic_link;

/* If true, make symbolic links relative  */
ಸ್ಥಿರ bool relative;

/* If true, hard links are logical rather than physical.  */
ಸ್ಥಿರ bool logical = !!LINK_FOLLOWS_SYMLINKS;

/* If true, ask the user before removing existing files.  */
ಸ್ಥಿರ bool interactive;

/* If true, remove existing files unconditionally.  */
ಸ್ಥಿರ bool remove_existing_files;

/* If true, list each file as it is moved. */
ಸ್ಥಿರ bool verbose;

/* If true, allow the superuser to *attempt* to make hard links
   to directories.  However, it appears that this option is not useful
   in practice, since even the superuser is prohibited from hard-linking
   directories on most existing systems (Solaris being an exception).  */
ಸ್ಥಿರ bool hard_dir_link;

/* If true, watch out for creating or removing hard links to directories.  */
ಸ್ಥಿರ bool beware_hard_dir_link;

/* If nonzero, and the specified destination is a symbolic link to a
   directory, treat it just as if it were a directory.  Otherwise, the
   command 'ln --force --no-dereference file symlink-to-dir' deletes
   symlink-to-dir before creating the new link.  */
ಸ್ಥಿರ bool dereference_dest_dir_symlinks = true;

/* This is a set of destination name/inode/dev triples for hard links
   created by ln.  Use this data structure to avoid data loss via a
   sequence of commands like this:
   rm -rf a b c; mkdir a b c; touch a/f b/f; ln -f a/f b/f c && rm -r a b */
ಸ್ಥಿರ Hash_table *dest_set;

/* Initial size of the dest_set hash table.  */
ಗಣನಾ { DEST_INFO_INITIAL_CAPACITY = 61 };

ಸ್ಥಿರ ಸಂರಚನಾ option ಧ್ರುವ long_options[] =
{
  {"backup", optional_argument, ರಿಕ್ತ, 'b'},
  {"directory", no_argument, ರಿಕ್ತ, 'F'},
  {"no-dereference", no_argument, ರಿಕ್ತ, 'n'},
  {"no-target-directory", no_argument, ರಿಕ್ತ, 'T'},
  {"force", no_argument, ರಿಕ್ತ, 'f'},
  {"interactive", no_argument, ರಿಕ್ತ, 'i'},
  {"suffix", required_argument, ರಿಕ್ತ, 'S'},
  {"target-directory", required_argument, ರಿಕ್ತ, 't'},
  {"logical", no_argument, ರಿಕ್ತ, 'L'},
  {"physical", no_argument, ರಿಕ್ತ, 'P'},
  {"relative", no_argument, ರಿಕ್ತ, 'r'},
  {"symbolic", no_argument, ರಿಕ್ತ, 's'},
  {"verbose", no_argument, ರಿಕ್ತ, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ರಿಕ್ತ, 0, ರಿಕ್ತ, 0}
};

/* Return an errno value for a system call that returned STATUS.
   This is zero if STATUS is zero, and is errno otherwise.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
errnoize (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಪ್ರತ್ಯಾಗಮ status < 0 ? errno : 0;
}

/* Return FROM represented as relative to the dir of TARGET.
   The result is malloced.  */

ಸ್ಥಿರ ವರ್ಣ *
convert_abs_rel (ವರ್ಣ ಧ್ರುವ *from, ವರ್ಣ ಧ್ರುವ *target)
{
  /* Get dirname to generate paths relative to.  We don't resolve
     the full TARGET as the last component could be an existing symlink.  */
  ವರ್ಣ *targetdir = dir_name (target);

  ವರ್ಣ *realdest = canonicalize_filename_mode (targetdir, CAN_MISSING);
  ವರ್ಣ *realfrom = canonicalize_filename_mode (from, CAN_MISSING);

  ವರ್ಣ *relative_from = ರಿಕ್ತ;
  ಯದಿ (realdest && realfrom)
    {
      /* Write to a PATH_MAX buffer.  */
      relative_from = xmalloc (PATH_MAX);

      ಯದಿ (!relpath (realfrom, realdest, relative_from, PATH_MAX))
        {
          ಮುಕ್ತ (relative_from);
          relative_from = ರಿಕ್ತ;
        }
    }

  ಮುಕ್ತ (targetdir);
  ಮುಕ್ತ (realdest);
  ಮುಕ್ತ (realfrom);

  ಪ್ರತ್ಯಾಗಮ relative_from ? relative_from : xstrdup (from);
}

/* Link SOURCE to DESTDIR_FD + DEST_BASE atomically.  DESTDIR_FD is
   the directory containing DEST_BASE.  Return 0 if successful, a
   positive errno value on failure, and -1 if an atomic link cannot be
   done.  This handles the common case where the destination does not
   already exist and -r is not specified.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
atomic_link (ವರ್ಣ ಧ್ರುವ *source, ಪೂರ್ಣಾಙ್ಕ destdir_fd, ವರ್ಣ ಧ್ರುವ *dest_base)
{
  ಪ್ರತ್ಯಾಗಮ (symbolic_link
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

ಸ್ಥಿರ bool
do_link (ವರ್ಣ ಧ್ರುವ *source, ಪೂರ್ಣಾಙ್ಕ destdir_fd, ವರ್ಣ ಧ್ರುವ *dest_base,
         ವರ್ಣ ಧ್ರುವ *dest, ಪೂರ್ಣಾಙ್ಕ link_errno)
{
  ಸಂರಚನಾ stat source_stats;
  ಪೂರ್ಣಾಙ್ಕ source_status = 1;
  ವರ್ಣ *backup_base = ರಿಕ್ತ;
  ವರ್ಣ *rel_source = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ nofollow_flag = logical ? 0 : AT_SYMLINK_NOFOLLOW;
  ಯದಿ (link_errno < 0)
    link_errno = atomic_link (source, destdir_fd, dest_base);

  /* Get SOURCE_STATS if later code will need it, if only for sharper
     diagnostics.  */
  ಯದಿ ((link_errno || dest_set) && !symbolic_link)
    {
      source_status = fstatat (AT_FDCWD, source, &source_stats, nofollow_flag);
      ಯದಿ (source_status != 0)
        {
          error (0, errno, _("failed to access %s"), quoteaf (source));
          ಪ್ರತ್ಯಾಗಮ false;
        }
    }

  ಯದಿ (link_errno)
    {
      ಯದಿ (!symbolic_link && !hard_dir_link && S_ISDIR (source_stats.st_mode))
        {
          error (0, 0, _("%s: hard link not allowed for directory"),
                 quotef (source));
          ಪ್ರತ್ಯಾಗಮ false;
        }

      ಯದಿ (relative)
        source = rel_source = convert_abs_rel (source, dest);

      bool force = (remove_existing_files || interactive
                    || backup_type != no_backups);
      ಯದಿ (force)
        {
          ಸಂರಚನಾ stat dest_stats;
          ಯದಿ (fstatat (destdir_fd, dest_base, &dest_stats, AT_SYMLINK_NOFOLLOW)
              != 0)
            {
              ಯದಿ (errno != ENOENT)
                {
                  error (0, errno, _("failed to access %s"), quoteaf (dest));
                  ಗಚ್ಛ fail;
                }
              force = false;
            }
          ಅನ್ಯಥಾ ಯದಿ (S_ISDIR (dest_stats.st_mode))
            {
              error (0, 0, _("%s: cannot overwrite directory"), quotef (dest));
              ಗಚ್ಛ fail;
            }
          ಅನ್ಯಥಾ ಯದಿ (seen_file (dest_set, dest, &dest_stats))
            {
              /* The current target was created as a hard link to another
                 source file.  */
              error (0, 0,
                     _("will not overwrite just-created %s with %s"),
                     quoteaf_n (0, dest), quoteaf_n (1, source));
              ಗಚ್ಛ fail;
            }
          ಅನ್ಯಥಾ
            {
              /* Beware removing DEST if it is the same directory entry as
                 SOURCE, because in that case removing DEST can cause the
                 subsequent link creation either to fail (for hard links), or
                 to replace a non-symlink DEST with a self-loop (for symbolic
                 links) which loses the contents of DEST.  So, when backing
                 up, worry about creating hard links (since the backups cover
                 the symlink case); otherwise, worry about about -f.  */
              ಯದಿ (backup_type != no_backups
                  ? !symbolic_link
                  : remove_existing_files)
                {
                  /* Detect whether removing DEST would also remove SOURCE.
                     If the file has only one link then both are surely the
                     same directory entry.  Otherwise check whether they point
                     to the same name in the same directory.  */
                  ಯದಿ (source_status != 0)
                    source_status = stat (source, &source_stats);
                  ಯದಿ (source_status == 0
                      && psame_inode (&source_stats, &dest_stats)
                      && (source_stats.st_nlink == 1
                          || same_nameat (AT_FDCWD, source,
                                          destdir_fd, dest_base)))
                    {
                      error (0, 0, _("%s and %s are the same file"),
                             quoteaf_n (0, source), quoteaf_n (1, dest));
                      ಗಚ್ಛ fail;
                    }
                }

              ಯದಿ (link_errno < 0 || link_errno == EEXIST)
                {
                  ಯದಿ (interactive)
                    {
                      fprintf (stderr, _("%s: replace %s? "),
                               program_name, quoteaf (dest));
                      ಯದಿ (!yesno ())
                        {
                          ಮುಕ್ತ (rel_source);
                          ಪ್ರತ್ಯಾಗಮ false;
                        }
                    }

                  ಯದಿ (backup_type != no_backups)
                    {
                      backup_base = find_backup_file_name (destdir_fd,
                                                           dest_base,
                                                           backup_type);
                      ಯದಿ (renameat (destdir_fd, dest_base,
                                    destdir_fd, backup_base)
                          != 0)
                        {
                          ಪೂರ್ಣಾಙ್ಕ rename_errno = errno;
                          ಮುಕ್ತ (backup_base);
                          backup_base = ರಿಕ್ತ;
                          ಯದಿ (rename_errno != ENOENT)
                            {
                              error (0, rename_errno, _("cannot backup %s"),
                                     quoteaf (dest));
                              ಗಚ್ಛ fail;
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

  ಯದಿ (link_errno <= 0)
    {
      /* Right after creating a hard link, do this: (note dest name and
         source_stats, which are also the just-linked-destinations stats) */
      ಯದಿ (! symbolic_link)
        record_file (dest_set, dest, &source_stats);

      ಯದಿ (verbose)
        {
          ವರ್ಣ ಧ್ರುವ *quoted_backup = "";
          ವರ್ಣ ಧ್ರುವ *backup_sep = "";
          ಯದಿ (backup_base)
            {
              ವರ್ಣ *backup = backup_base;
              ಶೂನ್ಯ *alloc = ರಿಕ್ತ;
              ptrdiff_t destdirlen = dest_base - dest;
              ಯದಿ (0 < destdirlen)
                {
                  alloc = xmalloc (destdirlen + strlen (backup_base) + 1);
                  backup = memcpy (alloc, dest, destdirlen);
                  strcpy (backup + destdirlen, backup_base);
                }
              quoted_backup = quoteaf_n (2, backup);
              backup_sep = " ~ ";
              ಮುಕ್ತ (alloc);
            }
          printf ("%s%s%s %c> %s\n", quoted_backup, backup_sep,
                  quoteaf_n (0, dest), symbolic_link ? '-' : '=',
                  quoteaf_n (1, source));
        }
    }
  ಅನ್ಯಥಾ
    {
      ವರ್ಣ *dest_quoted = quoteaf_n (0, dest);
      ವರ್ಣ *source_quoted = quoteaf_n (1, source);

      ಯದಿ (symbolic_link)
        {
          ಯದಿ (link_errno != ENAMETOOLONG && *source)
            error (0, link_errno, _("failed to create symbolic link %s"),
                   dest_quoted);
          ಅನ್ಯಥಾ
            error (0, link_errno, _("failed to create symbolic link %s -> %s"),
                   dest_quoted, source_quoted);
        }
      ಅನ್ಯಥಾ
        {
          ಯದಿ (link_errno == EMLINK)
            error (0, link_errno, _("failed to create hard link to %s"),
                   source_quoted);
          ಅನ್ಯಥಾ ಯದಿ (link_errno == EDQUOT || link_errno == EEXIST
                   || link_errno == ENOSPC || link_errno == EROFS)
            error (0, link_errno, _("failed to create hard link %s"),
                   dest_quoted);
          ಅನ್ಯಥಾ
            error (0, link_errno, _("failed to create hard link %s => %s"),
                   dest_quoted, source_quoted);
        }

      ಯದಿ (backup_base)
        {
          ಯದಿ (renameat (destdir_fd, backup_base, destdir_fd, dest_base) != 0)
            error (0, errno, _("cannot un-backup %s"), quoteaf (dest));
        }
    }

  ಮುಕ್ತ (backup_base);
  ಮುಕ್ತ (rel_source);
  ಪ್ರತ್ಯಾಗಮ link_errno <= 0;

fail:
  ಮುಕ್ತ (rel_source);
  ಪ್ರತ್ಯಾಗಮ false;
}

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
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
Create hard links by ಸ್ವಭಾವ, symbolic links with --symbolic.\n\
By ಸ್ವಭಾವ, each destination (name of new link) should not already exist.\n\
When creating hard links, each TARGET must exist.  Symbolic links\n\
can hold arbitrary text; ಯದಿ later resolved, a relative link is\n\
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
         ಯದಿ supported by the system\n\
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
         ಯದಿ it is a symbolic link to a directory\n\
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

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  ಪೂರ್ಣಾಙ್ಕ c;
  bool ok;
  bool make_backups = false;
  ವರ್ಣ ಧ್ರುವ *backup_suffix = ರಿಕ್ತ;
  ವರ್ಣ *version_control_string = ರಿಕ್ತ;
  ವರ್ಣ ಧ್ರುವ *target_directory = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ destdir_fd;
  bool no_target_directory = false;
  ಪೂರ್ಣಾಙ್ಕ n_files;
  ವರ್ಣ **file;
  ಪೂರ್ಣಾಙ್ಕ link_errno = -1;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdin);

  ಯಾವತ್ ((c = getopt_long (argc, argv, "bdfinrst:vFLPS:T",
                           long_options, ರಿಕ್ತ))
         != -1)
    {
      ವಿಕಲ್ಪ (c)
        {
        ಪಕ್ಷ 'b':
          make_backups = true;
          ಯದಿ (optarg)
            version_control_string = optarg;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'd':
        ಪಕ್ಷ 'F':
          hard_dir_link = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'f':
          remove_existing_files = true;
          interactive = false;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'i':
          remove_existing_files = false;
          interactive = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'L':
          logical = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'n':
          dereference_dest_dir_symlinks = false;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'P':
          logical = false;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'r':
          relative = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ 's':
          symbolic_link = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ 't':
          ಯದಿ (target_directory)
            error (EXIT_FAILURE, 0, _("multiple target directories specified"));
          ಅನ್ಯಥಾ
            {
              ಸಂರಚನಾ stat st;
              ಯದಿ (stat (optarg, &st) != 0)
                error (EXIT_FAILURE, errno, _("failed to access %s"),
                       quoteaf (optarg));
              ಯದಿ (! S_ISDIR (st.st_mode))
                error (EXIT_FAILURE, 0, _("target %s is not a directory"),
                       quoteaf (optarg));
            }
          target_directory = optarg;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'T':
          no_target_directory = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'v':
          verbose = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'S':
          make_backups = true;
          backup_suffix = optarg;
          ಭಙ್ಗ;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        ಸ್ವಭಾವ:
          usage (EXIT_FAILURE);
          ಭಙ್ಗ;
        }
    }

  n_files = argc - optind;
  file = argv + optind;

  ಯದಿ (n_files <= 0)
    {
      error (0, 0, _("missing file operand"));
      usage (EXIT_FAILURE);
    }

  ಯದಿ (relative && !symbolic_link)
    error (EXIT_FAILURE, 0, _("cannot do --relative without --symbolic"));

  ಯದಿ (!hard_dir_link)
    {
      priv_set_remove_linkdir ();
      beware_hard_dir_link = !cannot_unlink_dir ();
    }

  ಯದಿ (no_target_directory)
    {
      ಯದಿ (target_directory)
        error (EXIT_FAILURE, 0,
               _("cannot combine --target-directory "
                 "and --no-target-directory"));
      ಯದಿ (n_files != 2)
        {
          ಯದಿ (n_files < 2)
            error (0, 0,
                   _("missing destination file operand after %s"),
                   quoteaf (file[0]));
          ಅನ್ಯಥಾ
            error (0, 0, _("extra operand %s"), quoteaf (file[2]));
          usage (EXIT_FAILURE);
        }
    }
  ಅನ್ಯಥಾ ಯದಿ (n_files < 2 && !target_directory)
    {
      target_directory = ".";
      destdir_fd = AT_FDCWD;
    }
  ಅನ್ಯಥಾ
    {
      ಯದಿ (n_files == 2 && !target_directory)
        link_errno = atomic_link (file[0], AT_FDCWD, file[1]);
      ಯದಿ (link_errno < 0 || link_errno == EEXIST || link_errno == ENOTDIR
          || link_errno == EINVAL)
        {
          ವರ್ಣ ಧ್ರುವ *d
            = target_directory ? target_directory : file[n_files - 1];
          ಪೂರ್ಣಾಙ್ಕ flags = (O_PATHSEARCH | O_DIRECTORY
                       | (dereference_dest_dir_symlinks ? 0 : O_NOFOLLOW));
          destdir_fd = openat_safer (AT_FDCWD, d, flags);
          ಯದಿ (0 <= destdir_fd)
            {
              n_files -= !target_directory;
              target_directory = d;
            }
          ಅನ್ಯಥಾ ಯದಿ (! (n_files == 2 && !target_directory))
            error (EXIT_FAILURE, errno, _("target %s"), quoteaf (d));
        }
    }

  backup_type = (make_backups
                 ? xget_version (_("backup type"), version_control_string)
                 : no_backups);
  set_simple_backup_suffix (backup_suffix);


  ಯದಿ (target_directory)
    {
      /* Create the data structure we'll use to record which hard links we
         create.  Used to ensure that ln detects an obscure corner case that
         might result in user data loss.  Create it only if needed.  */
      ಯದಿ (2 <= n_files
          && remove_existing_files
          /* Don't bother trying to protect symlinks, since ln clobbering
             a just-created symlink won't ever lead to real data loss.  */
          && ! symbolic_link
          /* No destination hard link can be clobbered when making
             numbered backups.  */
          && backup_type != numbered_backups)
        {
          dest_set = hash_initialize (DEST_INFO_INITIAL_CAPACITY,
                                      ರಿಕ್ತ,
                                      triple_hash,
                                      triple_compare,
                                      triple_free);
          ಯದಿ (dest_set == ರಿಕ್ತ)
            xalloc_die ();
        }

      ok = true;
      ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < n_files; ++i)
        {
          ವರ್ಣ *dest_base;
          ವರ್ಣ *dest = file_name_concat (target_directory,
                                         last_component (file[i]),
                                         &dest_base);
          strip_trailing_slashes (dest_base);
          ok &= do_link (file[i], destdir_fd, dest_base, dest, -1);
          ಮುಕ್ತ (dest);
        }
    }
  ಅನ್ಯಥಾ
    ok = do_link (file[0], AT_FDCWD, file[1], file[1], link_errno);

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
