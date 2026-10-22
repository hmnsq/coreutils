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
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "ln"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS \
  proper_name ("Mike Parker"), \
  proper_name ("David MacKenzie")

/* FIXME: document */
ಸ್ಥಿರ ಎಣಿಕೆ backup_type backup_type;

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
ಸ್ಥಿರ Hash_ಕೋಷ್ಟಕ *dest_ಗಣ;

/* Initial size of the dest_set hash table.  */
ಎಣಿಕೆ { DEST_INFO_INITIAL_ಸಾಮರ್ಥ್ಯ = 61 };

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ long_options[] =
{
  {"backup", optional_ವಾದ, ಖಾಲಿ, 'b'},
  {"directory", no_ವಾದ, ಖಾಲಿ, 'F'},
  {"no-dereference", no_ವಾದ, ಖಾಲಿ, 'n'},
  {"no-target-directory", no_ವಾದ, ಖಾಲಿ, 'T'},
  {"force", no_ವಾದ, ಖಾಲಿ, 'f'},
  {"interactive", no_ವಾದ, ಖಾಲಿ, 'i'},
  {"suffix", required_ವಾದ, ಖಾಲಿ, 'S'},
  {"target-directory", required_ವಾದ, ಖಾಲಿ, 't'},
  {"logical", no_ವಾದ, ಖಾಲಿ, 'L'},
  {"physical", no_ವಾದ, ಖಾಲಿ, 'P'},
  {"relative", no_ವಾದ, ಖಾಲಿ, 'r'},
  {"symbolic", no_ವಾದ, ಖಾಲಿ, 's'},
  {"verbose", no_ವಾದ, ಖಾಲಿ, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};

/* Return an errno value for a system call that returned STATUS.
   This is zero if STATUS is zero, and is errno otherwise.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
errnoize (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2 < 0 ? errno : 0;
}

/* Return FROM represented as relative to the dir of TARGET.
   The result is malloced.  */

ಸ್ಥಿರ ಅಕ್ಷರ *
convert_abs_rel (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *from, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಗುರಿ)
{
  /* Get dirname to generate paths relative to.  We don't resolve
     the full TARGET as the last component could be an existing symlink.  */
  ಅಕ್ಷರ *targetdir = dir_name (ಗುರಿ);

  ಅಕ್ಷರ *realdest = canonicalize_filename_ಬಹುಲಕ (targetdir, CAN_MISSING);
  ಅಕ್ಷರ *realfrom = canonicalize_filename_ಬಹುಲಕ (from, CAN_MISSING);

  ಅಕ್ಷರ *relative_from = ಖಾಲಿ;
  ಆದರೆ (realdest && realfrom)
    {
      /* Write to a PATH_MAX buffer.  */
      relative_from = xmalloc (PATH_MAX);

      ಆದರೆ (!relpath (realfrom, realdest, relative_from, PATH_MAX))
        {
          ಬಿಡುಗಡೆ (relative_from);
          relative_from = ಖಾಲಿ;
        }
    }

  ಬಿಡುಗಡೆ (targetdir);
  ಬಿಡುಗಡೆ (realdest);
  ಬಿಡುಗಡೆ (realfrom);

  ಹಿಂತಿರುಗಿ relative_from ? relative_from : xstrdup (from);
}

/* Link SOURCE to DESTDIR_FD + DEST_BASE atomically.  DESTDIR_FD is
   the directory containing DEST_BASE.  Return 0 if successful, a
   positive errno value on failure, and -1 if an atomic link cannot be
   done.  This handles the common case where the destination does not
   already exist and -r is not specified.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
atomic_link (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಮೂಲ_2, ಪೂರ್ಣಾಂಕ destdir_fd, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dest_ತಳ)
{
  ಹಿಂತಿರುಗಿ (symbolic_link
          ? (relative ? -1
             : errnoize (symlinkat (ಮೂಲ_2, destdir_fd, dest_ತಳ)))
          : beware_hard_dir_link ? -1
          : errnoize (linkat (AT_FDCWD, ಮೂಲ_2, destdir_fd, dest_ತಳ,
                              logical ? AT_SYMLINK_FOLLOW : 0)));
}

/* Link SOURCE to a directory entry under DESTDIR_FD named DEST_BASE.
   DEST is the full name of the destination, useful for diagnostics.
   LINK_ERRNO is zero if the link has already been made,
   positive if attempting the link failed with errno == LINK_ERRNO,
   -1 if no attempt has been made to create the link.
   Return true if successful.  */

ಸ್ಥಿರ bool
do_link (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಮೂಲ_2, ಪೂರ್ಣಾಂಕ destdir_fd, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dest_ತಳ,
         ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dest, ಪೂರ್ಣಾಂಕ link_errno)
{
  ರಚನೆ stat ಮೂಲ_stats;
  ಪೂರ್ಣಾಂಕ ಮೂಲ_ಸ್ಥಿತಿ = 1;
  ಅಕ್ಷರ *backup_ತಳ = ಖಾಲಿ;
  ಅಕ್ಷರ *rel_ಮೂಲ = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ nofollow_flag = logical ? 0 : AT_SYMLINK_NOFOLLOW;
  ಆದರೆ (link_errno < 0)
    link_errno = atomic_link (ಮೂಲ_2, destdir_fd, dest_ತಳ);

  /* Get SOURCE_STATS if later code will need it, if only for sharper
     diagnostics.  */
  ಆದರೆ ((link_errno || dest_ಗಣ) && !symbolic_link)
    {
      ಮೂಲ_ಸ್ಥಿತಿ = fstatat (AT_FDCWD, ಮೂಲ_2, &ಮೂಲ_stats, nofollow_flag);
      ಆದರೆ (ಮೂಲ_ಸ್ಥಿತಿ != 0)
        {
          error (0, errno, _("failed to access %s"), quoteaf (ಮೂಲ_2));
          ಹಿಂತಿರುಗಿ false;
        }
    }

  ಆದರೆ (link_errno)
    {
      ಆದರೆ (!symbolic_link && !hard_dir_link && S_ISDIR (ಮೂಲ_stats.st_ಬಹುಲಕ))
        {
          error (0, 0, _("%s: hard link not allowed for directory"),
                 quotef (ಮೂಲ_2));
          ಹಿಂತಿರುಗಿ false;
        }

      ಆದರೆ (relative)
        ಮೂಲ_2 = rel_ಮೂಲ = convert_abs_rel (ಮೂಲ_2, dest);

      bool force = (remove_existing_files || interactive
                    || backup_type != no_backups);
      ಆದರೆ (force)
        {
          ರಚನೆ stat dest_stats;
          ಆದರೆ (fstatat (destdir_fd, dest_ತಳ, &dest_stats, AT_SYMLINK_NOFOLLOW)
              != 0)
            {
              ಆದರೆ (errno != ENOENT)
                {
                  error (0, errno, _("failed to access %s"), quoteaf (dest));
                  ಹೋಗು fail;
                }
              force = false;
            }
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (S_ISDIR (dest_stats.st_ಬಹುಲಕ))
            {
              error (0, 0, _("%s: cannot overwrite directory"), quotef (dest));
              ಹೋಗು fail;
            }
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ನೋಡಿದ_file (dest_ಗಣ, dest, &dest_stats))
            {
              /* The current target was created as a hard link to another
                 source file.  */
              error (0, 0,
                     _("will not overwrite just-created %s with %s"),
                     quoteaf_n (0, dest), quoteaf_n (1, ಮೂಲ_2));
              ಹೋಗು fail;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              /* Beware removing DEST if it is the same directory entry as
                 SOURCE, because in that case removing DEST can cause the
                 subsequent link creation either to fail (for hard links), or
                 to replace a non-symlink DEST with a self-loop (for symbolic
                 links) which loses the contents of DEST.  So, when backing
                 up, worry about creating hard links (since the backups cover
                 the symlink case); otherwise, worry about about -f.  */
              ಆದರೆ (backup_type != no_backups
                  ? !symbolic_link
                  : remove_existing_files)
                {
                  /* Detect whether removing DEST would also remove SOURCE.
                     If the file has only one link then both are surely the
                     same directory entry.  Otherwise check whether they point
                     to the same name in the same directory.  */
                  ಆದರೆ (ಮೂಲ_ಸ್ಥಿತಿ != 0)
                    ಮೂಲ_ಸ್ಥಿತಿ = stat (ಮೂಲ_2, &ಮೂಲ_stats);
                  ಆದರೆ (ಮೂಲ_ಸ್ಥಿತಿ == 0
                      && psame_inode (&ಮೂಲ_stats, &dest_stats)
                      && (ಮೂಲ_stats.st_nlink == 1
                          || same_nameat (AT_FDCWD, ಮೂಲ_2,
                                          destdir_fd, dest_ತಳ)))
                    {
                      error (0, 0, _("%s and %s are the same file"),
                             quoteaf_n (0, ಮೂಲ_2), quoteaf_n (1, dest));
                      ಹೋಗು fail;
                    }
                }

              ಆದರೆ (link_errno < 0 || link_errno == EEXIST)
                {
                  ಆದರೆ (interactive)
                    {
                      fprintf (stderr, _("%s: replace %s? "),
                               ಕಾರ್ಯಕ್ರಮ_name, quoteaf (dest));
                      ಆದರೆ (!yesno ())
                        {
                          ಬಿಡುಗಡೆ (rel_ಮೂಲ);
                          ಹಿಂತಿರುಗಿ false;
                        }
                    }

                  ಆದರೆ (backup_type != no_backups)
                    {
                      backup_ತಳ = ಕಂಡುಹಿಡಿ_backup_file_name (destdir_fd,
                                                           dest_ತಳ,
                                                           backup_type);
                      ಆದರೆ (renameat (destdir_fd, dest_ತಳ,
                                    destdir_fd, backup_ತಳ)
                          != 0)
                        {
                          ಪೂರ್ಣಾಂಕ rename_errno = errno;
                          ಬಿಡುಗಡೆ (backup_ತಳ);
                          backup_ತಳ = ಖಾಲಿ;
                          ಆದರೆ (rename_errno != ENOENT)
                            {
                              error (0, rename_errno, _("cannot backup %s"),
                                     quoteaf (dest));
                              ಹೋಗು fail;
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
           ? force_symlinkat (ಮೂಲ_2, destdir_fd, dest_ತಳ,
                              force, link_errno)
           : force_linkat (AT_FDCWD, ಮೂಲ_2, destdir_fd, dest_ತಳ,
                           logical ? AT_SYMLINK_FOLLOW : 0,
                           force, link_errno));
      /* Until now, link_errno < 0 meant the link has not been tried.
         From here on, link_errno < 0 means the link worked but
         required removing the destination first.  */
    }

  ಆದರೆ (link_errno <= 0)
    {
      /* Right after creating a hard link, do this: (note dest name and
         source_stats, which are also the just-linked-destinations stats) */
      ಆದರೆ (! symbolic_link)
        ದಾಖಲೆ_file (dest_ಗಣ, dest, &ಮೂಲ_stats);

      ಆದರೆ (verbose)
        {
          ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *quoted_backup = "";
          ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *backup_sep = "";
          ಆದರೆ (backup_ತಳ)
            {
              ಅಕ್ಷರ *backup = backup_ತಳ;
              ಶೂನ್ಯ *alloc = ಖಾಲಿ;
              ptrdiff_t destdirlen = dest_ತಳ - dest;
              ಆದರೆ (0 < destdirlen)
                {
                  alloc = xmalloc (destdirlen + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (backup_ತಳ) + 1);
                  backup = ಸ್ಮರಣೆ_ನಕಲು (alloc, dest, destdirlen);
                  strcpy (backup + destdirlen, backup_ತಳ);
                }
              quoted_backup = quoteaf_n (2, backup);
              backup_sep = " ~ ";
              ಬಿಡುಗಡೆ (alloc);
            }
          ಬರೆ ("%s%s%s %c> %s\n", quoted_backup, backup_sep,
                  quoteaf_n (0, dest), symbolic_link ? '-' : '=',
                  quoteaf_n (1, ಮೂಲ_2));
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಅಕ್ಷರ *dest_quoted = quoteaf_n (0, dest);
      ಅಕ್ಷರ *ಮೂಲ_quoted = quoteaf_n (1, ಮೂಲ_2);

      ಆದರೆ (symbolic_link)
        {
          ಆದರೆ (link_errno != ENAMETOOLONG && *ಮೂಲ_2)
            error (0, link_errno, _("failed to create symbolic link %s"),
                   dest_quoted);
          ಇಲ್ಲದಿದ್ದರೆ
            error (0, link_errno, _("failed to create symbolic link %s -> %s"),
                   dest_quoted, ಮೂಲ_quoted);
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಆದರೆ (link_errno == EMLINK)
            error (0, link_errno, _("failed to create hard link to %s"),
                   ಮೂಲ_quoted);
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (link_errno == EDQUOT || link_errno == EEXIST
                   || link_errno == ENOSPC || link_errno == EROFS)
            error (0, link_errno, _("failed to create hard link %s"),
                   dest_quoted);
          ಇಲ್ಲದಿದ್ದರೆ
            error (0, link_errno, _("failed to create hard link %s => %s"),
                   dest_quoted, ಮೂಲ_quoted);
        }

      ಆದರೆ (backup_ತಳ)
        {
          ಆದರೆ (renameat (destdir_fd, backup_ತಳ, destdir_fd, dest_ತಳ) != 0)
            error (0, errno, _("cannot un-backup %s"), quoteaf (dest));
        }
    }

  ಬಿಡುಗಡೆ (backup_ತಳ);
  ಬಿಡುಗಡೆ (rel_ಮೂಲ);
  ಹಿಂತಿರುಗಿ link_errno <= 0;

fail:
  ಬಿಡುಗಡೆ (rel_ಮೂಲ);
  ಹಿಂತಿರುಗಿ false;
}

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("\
Usage: %s [OPTION]... [-T] ಗುರಿ_2 LINK_NAME\n\
  or:  %s [OPTION]... ಗುರಿ_2\n\
  or:  %s [OPTION]... ಗುರಿ_2... DIRECTORY\n\
  or:  %s [OPTION]... -t DIRECTORY ಗುರಿ_2...\n\
"),
              ಕಾರ್ಯಕ್ರಮ_name, ಕಾರ್ಯಕ್ರಮ_name, ಕಾರ್ಯಕ್ರಮ_name, ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
ಒಳ_2 the 1st form, create a link ಗಮ್ಯ ಗುರಿ_2 with the name LINK_NAME.\n\
ಒಳ_2 the 2nd form, create a link ಗಮ್ಯ ಗುರಿ_2 ಒಳ the current directory.\n\
ಒಳ_2 the 3rd and 4th forms, create links ಗಮ್ಯ each ಗುರಿ_2 ಒಳ DIRECTORY.\n\
Create hard links by ಪೂರ್ವನಿಯೋಜಿತ, symbolic links with --symbolic.\n\
By ಪೂರ್ವನಿಯೋಜಿತ, each destination (name ದ new link) should not already exist.\n\
When creating hard links, each ಗುರಿ_2 must exist.  Symbolic links\n\
can hold arbitrary ಪಠ್ಯ; ಆದರೆ later resolved, a relative link is\n\
interpreted ಒಳ ಸಂಬಂಧ ಗಮ್ಯ its ಪೋಷಕ directory.\n\
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
  -d, -F, --directory\n\
         allow the superuser ಗಮ್ಯ attempt ಗಮ್ಯ hard link directories,\n\
         ಆದರೆ supported by the system\n\
"));
      oputs (_("\
  -f, --force\n\
         remove existing destination files\n\
"));
      oputs (_("\
  -i, --interactive\n\
         prompt whether ಗಮ್ಯ remove destinations\n\
"));
      oputs (_("\
  -L, --logical\n\
         dereference TARGETs that are symbolic links\n\
"));
      oputs (_("\
  -n, --no-dereference\n\
         treat LINK_NAME as a ಸಾಮಾನ್ಯ file\n\
         ಆದರೆ it is a symbolic link ಗಮ್ಯ a directory\n\
"));
      oputs (_("\
  -P, --physical\n\
         make hard links directly ಗಮ್ಯ symbolic links\n\
"));
      oputs (_("\
  -r, --relative\n\
         with -s, create links relative ಗಮ್ಯ link location\n\
"));
      oputs (_("\
  -s, --symbolic\n\
         make symbolic links instead ದ hard links\n\
"));
      oputs (_("\
  -S, --suffix=SUFFIX\n\
         override the usual backup suffix\n\
"));
      oputs (_("\
  -t, --ಗುರಿ-directory=DIRECTORY\n\
         specify the DIRECTORY ಒಳ which ಗಮ್ಯ create the links\n\
"));
      oputs (_("\
  -T, --no-ಗುರಿ-directory\n\
         treat LINK_NAME as a ಸಾಮಾನ್ಯ file always\n\
"));
      oputs (_("\
  -v, --verbose\n\
         print name ದ each linked file\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      emit_backup_suffix_note ();
      ಬರೆ (_("\
\n\
Using -s ignores -L and -P.  Otherwise, the ಕೊನೆಯ option specified controls\n\
behavior when a ಗುರಿ_2 is a symbolic link, defaulting ಗಮ್ಯ %s.\n\
"), LINK_FOLLOWS_SYMLINKS ? "-L" : "-P");
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  ಪೂರ್ಣಾಂಕ c;
  bool ok;
  bool make_backups = false;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *backup_suffix = ಖಾಲಿ;
  ಅಕ್ಷರ *ಆವೃತ್ತಿ_control_string = ಖಾಲಿ;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಗುರಿ_directory = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ destdir_fd;
  bool no_ಗುರಿ_directory = false;
  ಪೂರ್ಣಾಂಕ n_files;
  ಅಕ್ಷರ **file;
  ಪೂರ್ಣಾಂಕ link_errno = -1;

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdin);

  ಆಗ ((c = getopt_long (argc, argv, "bdfinrst:vFLPS:T",
                           long_options, ಖಾಲಿ))
         != -1)
    {
      ಆಯ್ಕೆ (c)
        {
        ಸ್ಥಿತಿ 'b':
          make_backups = true;
          ಆದರೆ (optarg)
            ಆವೃತ್ತಿ_control_string = optarg;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'd':
        ಸ್ಥಿತಿ 'F':
          hard_dir_link = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'f':
          remove_existing_files = true;
          interactive = false;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'i':
          remove_existing_files = false;
          interactive = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'L':
          logical = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'n':
          dereference_dest_dir_symlinks = false;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'P':
          logical = false;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'r':
          relative = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 's':
          symbolic_link = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 't':
          ಆದರೆ (ಗುರಿ_directory)
            error (EXIT_FAILURE, 0, _("multiple target directories specified"));
          ಇಲ್ಲದಿದ್ದರೆ
            {
              ರಚನೆ stat st;
              ಆದರೆ (stat (optarg, &st) != 0)
                error (EXIT_FAILURE, errno, _("failed to access %s"),
                       quoteaf (optarg));
              ಆದರೆ (! S_ISDIR (st.st_ಬಹುಲಕ))
                error (EXIT_FAILURE, 0, _("target %s is not a directory"),
                       quoteaf (optarg));
            }
          ಗುರಿ_directory = optarg;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'T':
          no_ಗುರಿ_directory = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'v':
          verbose = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'S':
          make_backups = true;
          backup_suffix = optarg;
          ನಿಲ್ಲಿಸು;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);
        ಪೂರ್ವನಿಯೋಜಿತ:
          usage (EXIT_FAILURE);
          ನಿಲ್ಲಿಸು;
        }
    }

  n_files = argc - optind;
  file = argv + optind;

  ಆದರೆ (n_files <= 0)
    {
      error (0, 0, _("missing file operand"));
      usage (EXIT_FAILURE);
    }

  ಆದರೆ (relative && !symbolic_link)
    error (EXIT_FAILURE, 0, _("cannot do --relative without --symbolic"));

  ಆದರೆ (!hard_dir_link)
    {
      priv_ಗಣ_remove_linkdir ();
      beware_hard_dir_link = !cannot_unlink_dir ();
    }

  ಆದರೆ (no_ಗುರಿ_directory)
    {
      ಆದರೆ (ಗುರಿ_directory)
        error (EXIT_FAILURE, 0,
               _("cannot combine --target-directory "
                 "and --no-target-directory"));
      ಆದರೆ (n_files != 2)
        {
          ಆದರೆ (n_files < 2)
            error (0, 0,
                   _("missing destination file operand after %s"),
                   quoteaf (file[0]));
          ಇಲ್ಲದಿದ್ದರೆ
            error (0, 0, _("extra operand %s"), quoteaf (file[2]));
          usage (EXIT_FAILURE);
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (n_files < 2 && !ಗುರಿ_directory)
    {
      ಗುರಿ_directory = ".";
      destdir_fd = AT_FDCWD;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಆದರೆ (n_files == 2 && !ಗುರಿ_directory)
        link_errno = atomic_link (file[0], AT_FDCWD, file[1]);
      ಆದರೆ (link_errno < 0 || link_errno == EEXIST || link_errno == ENOTDIR
          || link_errno == EINVAL)
        {
          ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *d
            = ಗುರಿ_directory ? ಗುರಿ_directory : file[n_files - 1];
          ಪೂರ್ಣಾಂಕ flags = (O_PATHSEARCH | O_DIRECTORY
                       | (dereference_dest_dir_symlinks ? 0 : O_NOFOLLOW));
          destdir_fd = openat_safer (AT_FDCWD, d, flags);
          ಆದರೆ (0 <= destdir_fd)
            {
              n_files -= !ಗುರಿ_directory;
              ಗುರಿ_directory = d;
            }
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (! (n_files == 2 && !ಗುರಿ_directory))
            error (EXIT_FAILURE, errno, _("target %s"), quoteaf (d));
        }
    }

  backup_type = (make_backups
                 ? xget_ಆವೃತ್ತಿ (_("backup type"), ಆವೃತ್ತಿ_control_string)
                 : no_backups);
  ಗಣ_simple_backup_suffix (backup_suffix);


  ಆದರೆ (ಗುರಿ_directory)
    {
      /* Create the data structure we'll use to record which hard links we
         create.  Used to ensure that ln detects an obscure corner case that
         might result in user data loss.  Create it only if needed.  */
      ಆದರೆ (2 <= n_files
          && remove_existing_files
          /* Don't bother trying to protect symlinks, since ln clobbering
             a just-created symlink won't ever lead to real data loss.  */
          && ! symbolic_link
          /* No destination hard link can be clobbered when making
             numbered backups.  */
          && backup_type != numbered_backups)
        {
          dest_ಗಣ = hash_initialize (DEST_INFO_INITIAL_ಸಾಮರ್ಥ್ಯ,
                                      ಖಾಲಿ,
                                      triple_hash,
                                      triple_compare,
                                      triple_free);
          ಆದರೆ (dest_ಗಣ == ಖಾಲಿ)
            xalloc_die ();
        }

      ok = true;
      ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < n_files; ++i)
        {
          ಅಕ್ಷರ *dest_ತಳ;
          ಅಕ್ಷರ *dest = file_name_concat (ಗುರಿ_directory,
                                         ಕೊನೆಯ_component (file[i]),
                                         &dest_ತಳ);
          strip_trailing_slashes (dest_ತಳ);
          ok &= do_link (file[i], destdir_fd, dest_ತಳ, dest, -1);
          ಬಿಡುಗಡೆ (dest);
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ
    ok = do_link (file[0], AT_FDCWD, file[1], file[1], link_errno);

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
