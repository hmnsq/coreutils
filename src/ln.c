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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "backupfile.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "fcntl-safer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "filenamecat.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "file-set.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "force-link.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hash.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hashcode-file.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "priv-set.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "relpath.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "same.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unlinkdir.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "yesno.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "canonicalize.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "ln"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS \
  proper_name ("Mike Parker"), \
  proper_name ("David MacKenzie")

/* FIXME: document */
ਸਥਿਰ ਗਿਣਤੀ backup_type backup_type;

/* If true, make symbolic links; otherwise, make hard links.  */
ਸਥਿਰ bool symbolic_link;

/* If true, make symbolic links relative  */
ਸਥਿਰ bool relative;

/* If true, hard links are logical rather than physical.  */
ਸਥਿਰ bool logical = !!LINK_FOLLOWS_SYMLINKS;

/* If true, ask the user before removing existing files.  */
ਸਥਿਰ bool interactive;

/* If true, remove existing files unconditionally.  */
ਸਥਿਰ bool remove_existing_files;

/* If true, list each file as it is moved. */
ਸਥਿਰ bool verbose;

/* If true, allow the superuser to *attempt* to make hard links
   to directories.  However, it appears that this option is not useful
   in practice, since even the superuser is prohibited from hard-linking
   directories on most existing systems (Solaris being an exception).  */
ਸਥਿਰ bool hard_dir_link;

/* If true, watch out for creating or removing hard links to directories.  */
ਸਥਿਰ bool beware_hard_dir_link;

/* If nonzero, and the specified destination is a symbolic link to a
   directory, treat it just as if it were a directory.  Otherwise, the
   command 'ln --force --no-dereference file symlink-to-dir' deletes
   symlink-to-dir before creating the new link.  */
ਸਥਿਰ bool dereference_dest_dir_symlinks = true;

/* This is a set of destination name/inode/dev triples for hard links
   created by ln.  Use this data structure to avoid data loss via a
   sequence of commands like this:
   rm -rf a b c; mkdir a b c; touch a/f b/f; ln -f a/f b/f c && rm -r a b */
ਸਥਿਰ Hash_ਸਾਰਣੀ *dest_ਸੈੱਟ;

/* Initial size of the dest_set hash table.  */
ਗਿਣਤੀ { DEST_INFO_INITIAL_ਸਮਰੱਥਾ = 61 };

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ long_options[] =
{
  {"backup", optional_ਦਲੀਲ, ਨਿਰਾ, 'b'},
  {"directory", no_ਦਲੀਲ, ਨਿਰਾ, 'F'},
  {"no-dereference", no_ਦਲੀਲ, ਨਿਰਾ, 'n'},
  {"no-target-directory", no_ਦਲੀਲ, ਨਿਰਾ, 'T'},
  {"force", no_ਦਲੀਲ, ਨਿਰਾ, 'f'},
  {"interactive", no_ਦਲੀਲ, ਨਿਰਾ, 'i'},
  {"suffix", required_ਦਲੀਲ, ਨਿਰਾ, 'S'},
  {"target-directory", required_ਦਲੀਲ, ਨਿਰਾ, 't'},
  {"logical", no_ਦਲੀਲ, ਨਿਰਾ, 'L'},
  {"physical", no_ਦਲੀਲ, ਨਿਰਾ, 'P'},
  {"relative", no_ਦਲੀਲ, ਨਿਰਾ, 'r'},
  {"symbolic", no_ਦਲੀਲ, ਨਿਰਾ, 's'},
  {"verbose", no_ਦਲੀਲ, ਨਿਰਾ, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};

/* Return an errno value for a system call that returned STATUS.
   This is zero if STATUS is zero, and is errno otherwise.  */

ਸਥਿਰ ਪੂਰਨ
errnoize (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਵਾਪਸ ਸਥਿਤੀ < 0 ? errno : 0;
}

/* Return FROM represented as relative to the dir of TARGET.
   The result is malloced.  */

ਸਥਿਰ ਅੱਖਰ *
convert_abs_rel (ਅੱਖਰ ਅਚਲ *from, ਅੱਖਰ ਅਚਲ *ਨਿਸ਼ਾਨਾ)
{
  /* Get dirname to generate paths relative to.  We don't resolve
     the full TARGET as the last component could be an existing symlink.  */
  ਅੱਖਰ *targetdir = dir_name (ਨਿਸ਼ਾਨਾ);

  ਅੱਖਰ *realdest = canonicalize_filename_ਬਹੁਲਕ (targetdir, CAN_MISSING);
  ਅੱਖਰ *realfrom = canonicalize_filename_ਬਹੁਲਕ (from, CAN_MISSING);

  ਅੱਖਰ *relative_from = ਨਿਰਾ;
  ਜੇ (realdest && realfrom)
    {
      /* Write to a PATH_MAX buffer.  */
      relative_from = xmalloc (PATH_MAX);

      ਜੇ (!relpath (realfrom, realdest, relative_from, PATH_MAX))
        {
          ਮੁਕਤ (relative_from);
          relative_from = ਨਿਰਾ;
        }
    }

  ਮੁਕਤ (targetdir);
  ਮੁਕਤ (realdest);
  ਮੁਕਤ (realfrom);

  ਵਾਪਸ relative_from ? relative_from : xstrdup (from);
}

/* Link SOURCE to DESTDIR_FD + DEST_BASE atomically.  DESTDIR_FD is
   the directory containing DEST_BASE.  Return 0 if successful, a
   positive errno value on failure, and -1 if an atomic link cannot be
   done.  This handles the common case where the destination does not
   already exist and -r is not specified.  */

ਸਥਿਰ ਪੂਰਨ
atomic_link (ਅੱਖਰ ਅਚਲ *ਸਰੋਤ_2, ਪੂਰਨ destdir_fd, ਅੱਖਰ ਅਚਲ *dest_ਅਧਾਰ)
{
  ਵਾਪਸ (symbolic_link
          ? (relative ? -1
             : errnoize (symlinkat (ਸਰੋਤ_2, destdir_fd, dest_ਅਧਾਰ)))
          : beware_hard_dir_link ? -1
          : errnoize (linkat (AT_FDCWD, ਸਰੋਤ_2, destdir_fd, dest_ਅਧਾਰ,
                              logical ? AT_SYMLINK_FOLLOW : 0)));
}

/* Link SOURCE to a directory entry under DESTDIR_FD named DEST_BASE.
   DEST is the full name of the destination, useful for diagnostics.
   LINK_ERRNO is zero if the link has already been made,
   positive if attempting the link failed with errno == LINK_ERRNO,
   -1 if no attempt has been made to create the link.
   Return true if successful.  */

ਸਥਿਰ bool
do_link (ਅੱਖਰ ਅਚਲ *ਸਰੋਤ_2, ਪੂਰਨ destdir_fd, ਅੱਖਰ ਅਚਲ *dest_ਅਧਾਰ,
         ਅੱਖਰ ਅਚਲ *dest, ਪੂਰਨ link_errno)
{
  ਬਣਤਰ stat ਸਰੋਤ_stats;
  ਪੂਰਨ ਸਰੋਤ_ਸਥਿਤੀ = 1;
  ਅੱਖਰ *backup_ਅਧਾਰ = ਨਿਰਾ;
  ਅੱਖਰ *rel_ਸਰੋਤ = ਨਿਰਾ;
  ਪੂਰਨ nofollow_flag = logical ? 0 : AT_SYMLINK_NOFOLLOW;
  ਜੇ (link_errno < 0)
    link_errno = atomic_link (ਸਰੋਤ_2, destdir_fd, dest_ਅਧਾਰ);

  /* Get SOURCE_STATS if later code will need it, if only for sharper
     diagnostics.  */
  ਜੇ ((link_errno || dest_ਸੈੱਟ) && !symbolic_link)
    {
      ਸਰੋਤ_ਸਥਿਤੀ = fstatat (AT_FDCWD, ਸਰੋਤ_2, &ਸਰੋਤ_stats, nofollow_flag);
      ਜੇ (ਸਰੋਤ_ਸਥਿਤੀ != 0)
        {
          error (0, errno, _("failed to access %s"), quoteaf (ਸਰੋਤ_2));
          ਵਾਪਸ false;
        }
    }

  ਜੇ (link_errno)
    {
      ਜੇ (!symbolic_link && !hard_dir_link && S_ISDIR (ਸਰੋਤ_stats.st_ਬਹੁਲਕ))
        {
          error (0, 0, _("%s: hard link not allowed for directory"),
                 quotef (ਸਰੋਤ_2));
          ਵਾਪਸ false;
        }

      ਜੇ (relative)
        ਸਰੋਤ_2 = rel_ਸਰੋਤ = convert_abs_rel (ਸਰੋਤ_2, dest);

      bool force = (remove_existing_files || interactive
                    || backup_type != no_backups);
      ਜੇ (force)
        {
          ਬਣਤਰ stat dest_stats;
          ਜੇ (fstatat (destdir_fd, dest_ਅਧਾਰ, &dest_stats, AT_SYMLINK_NOFOLLOW)
              != 0)
            {
              ਜੇ (errno != ENOENT)
                {
                  error (0, errno, _("failed to access %s"), quoteaf (dest));
                  ਜਾਓ fail;
                }
              force = false;
            }
          ਨਹੀਂਤਾਂ ਜੇ (S_ISDIR (dest_stats.st_ਬਹੁਲਕ))
            {
              error (0, 0, _("%s: cannot overwrite directory"), quotef (dest));
              ਜਾਓ fail;
            }
          ਨਹੀਂਤਾਂ ਜੇ (ਵੇਖਿਆ_file (dest_ਸੈੱਟ, dest, &dest_stats))
            {
              /* The current target was created as a hard link to another
                 source file.  */
              error (0, 0,
                     _("will not overwrite just-created %s with %s"),
                     quoteaf_n (0, dest), quoteaf_n (1, ਸਰੋਤ_2));
              ਜਾਓ fail;
            }
          ਨਹੀਂਤਾਂ
            {
              /* Beware removing DEST if it is the same directory entry as
                 SOURCE, because in that case removing DEST can cause the
                 subsequent link creation either to fail (for hard links), or
                 to replace a non-symlink DEST with a self-loop (for symbolic
                 links) which loses the contents of DEST.  So, when backing
                 up, worry about creating hard links (since the backups cover
                 the symlink case); otherwise, worry about about -f.  */
              ਜੇ (backup_type != no_backups
                  ? !symbolic_link
                  : remove_existing_files)
                {
                  /* Detect whether removing DEST would also remove SOURCE.
                     If the file has only one link then both are surely the
                     same directory entry.  Otherwise check whether they point
                     to the same name in the same directory.  */
                  ਜੇ (ਸਰੋਤ_ਸਥਿਤੀ != 0)
                    ਸਰੋਤ_ਸਥਿਤੀ = stat (ਸਰੋਤ_2, &ਸਰੋਤ_stats);
                  ਜੇ (ਸਰੋਤ_ਸਥਿਤੀ == 0
                      && psame_inode (&ਸਰੋਤ_stats, &dest_stats)
                      && (ਸਰੋਤ_stats.st_nlink == 1
                          || same_nameat (AT_FDCWD, ਸਰੋਤ_2,
                                          destdir_fd, dest_ਅਧਾਰ)))
                    {
                      error (0, 0, _("%s and %s are the same file"),
                             quoteaf_n (0, ਸਰੋਤ_2), quoteaf_n (1, dest));
                      ਜਾਓ fail;
                    }
                }

              ਜੇ (link_errno < 0 || link_errno == EEXIST)
                {
                  ਜੇ (interactive)
                    {
                      fprintf (stderr, _("%s: replace %s? "),
                               ਪ੍ਰੋਗਰਾਮ_name, quoteaf (dest));
                      ਜੇ (!yesno ())
                        {
                          ਮੁਕਤ (rel_ਸਰੋਤ);
                          ਵਾਪਸ false;
                        }
                    }

                  ਜੇ (backup_type != no_backups)
                    {
                      backup_ਅਧਾਰ = ਲੱਭੋ_backup_file_name (destdir_fd,
                                                           dest_ਅਧਾਰ,
                                                           backup_type);
                      ਜੇ (renameat (destdir_fd, dest_ਅਧਾਰ,
                                    destdir_fd, backup_ਅਧਾਰ)
                          != 0)
                        {
                          ਪੂਰਨ rename_errno = errno;
                          ਮੁਕਤ (backup_ਅਧਾਰ);
                          backup_ਅਧਾਰ = ਨਿਰਾ;
                          ਜੇ (rename_errno != ENOENT)
                            {
                              error (0, rename_errno, _("cannot backup %s"),
                                     quoteaf (dest));
                              ਜਾਓ fail;
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
           ? force_symlinkat (ਸਰੋਤ_2, destdir_fd, dest_ਅਧਾਰ,
                              force, link_errno)
           : force_linkat (AT_FDCWD, ਸਰੋਤ_2, destdir_fd, dest_ਅਧਾਰ,
                           logical ? AT_SYMLINK_FOLLOW : 0,
                           force, link_errno));
      /* Until now, link_errno < 0 meant the link has not been tried.
         From here on, link_errno < 0 means the link worked but
         required removing the destination first.  */
    }

  ਜੇ (link_errno <= 0)
    {
      /* Right after creating a hard link, do this: (note dest name and
         source_stats, which are also the just-linked-destinations stats) */
      ਜੇ (! symbolic_link)
        ਰਿਕਾਰਡ_file (dest_ਸੈੱਟ, dest, &ਸਰੋਤ_stats);

      ਜੇ (verbose)
        {
          ਅੱਖਰ ਅਚਲ *quoted_backup = "";
          ਅੱਖਰ ਅਚਲ *backup_sep = "";
          ਜੇ (backup_ਅਧਾਰ)
            {
              ਅੱਖਰ *backup = backup_ਅਧਾਰ;
              ਖਾਲੀ *alloc = ਨਿਰਾ;
              ptrdiff_t destdirlen = dest_ਅਧਾਰ - dest;
              ਜੇ (0 < destdirlen)
                {
                  alloc = xmalloc (destdirlen + ਲੜੀ_ਲੰਬਾਈ (backup_ਅਧਾਰ) + 1);
                  backup = ਸਮ੍ਰਿਤੀ_ਨਕਲ (alloc, dest, destdirlen);
                  strcpy (backup + destdirlen, backup_ਅਧਾਰ);
                }
              quoted_backup = quoteaf_n (2, backup);
              backup_sep = " ~ ";
              ਮੁਕਤ (alloc);
            }
          ਲਿਖੋ ("%s%s%s %c> %s\n", quoted_backup, backup_sep,
                  quoteaf_n (0, dest), symbolic_link ? '-' : '=',
                  quoteaf_n (1, ਸਰੋਤ_2));
        }
    }
  ਨਹੀਂਤਾਂ
    {
      ਅੱਖਰ *dest_quoted = quoteaf_n (0, dest);
      ਅੱਖਰ *ਸਰੋਤ_quoted = quoteaf_n (1, ਸਰੋਤ_2);

      ਜੇ (symbolic_link)
        {
          ਜੇ (link_errno != ENAMETOOLONG && *ਸਰੋਤ_2)
            error (0, link_errno, _("failed to create symbolic link %s"),
                   dest_quoted);
          ਨਹੀਂਤਾਂ
            error (0, link_errno, _("failed to create symbolic link %s -> %s"),
                   dest_quoted, ਸਰੋਤ_quoted);
        }
      ਨਹੀਂਤਾਂ
        {
          ਜੇ (link_errno == EMLINK)
            error (0, link_errno, _("failed to create hard link to %s"),
                   ਸਰੋਤ_quoted);
          ਨਹੀਂਤਾਂ ਜੇ (link_errno == EDQUOT || link_errno == EEXIST
                   || link_errno == ENOSPC || link_errno == EROFS)
            error (0, link_errno, _("failed to create hard link %s"),
                   dest_quoted);
          ਨਹੀਂਤਾਂ
            error (0, link_errno, _("failed to create hard link %s => %s"),
                   dest_quoted, ਸਰੋਤ_quoted);
        }

      ਜੇ (backup_ਅਧਾਰ)
        {
          ਜੇ (renameat (destdir_fd, backup_ਅਧਾਰ, destdir_fd, dest_ਅਧਾਰ) != 0)
            error (0, errno, _("cannot un-backup %s"), quoteaf (dest));
        }
    }

  ਮੁਕਤ (backup_ਅਧਾਰ);
  ਮੁਕਤ (rel_ਸਰੋਤ);
  ਵਾਪਸ link_errno <= 0;

fail:
  ਮੁਕਤ (rel_ਸਰੋਤ);
  ਵਾਪਸ false;
}

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("\
Usage: %s [OPTION]... [-T] ਨਿਸ਼ਾਨਾ_2 LINK_NAME\n\
  or:  %s [OPTION]... ਨਿਸ਼ਾਨਾ_2\n\
  or:  %s [OPTION]... ਨਿਸ਼ਾਨਾ_2... DIRECTORY\n\
  or:  %s [OPTION]... -t DIRECTORY ਨਿਸ਼ਾਨਾ_2...\n\
"),
              ਪ੍ਰੋਗਰਾਮ_name, ਪ੍ਰੋਗਰਾਮ_name, ਪ੍ਰੋਗਰਾਮ_name, ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
ਵਿੱਚ_2 the 1st form, create a link ਮੰਜ਼ਿਲ ਨਿਸ਼ਾਨਾ_2 with the name LINK_NAME.\n\
ਵਿੱਚ_2 the 2nd form, create a link ਮੰਜ਼ਿਲ ਨਿਸ਼ਾਨਾ_2 ਵਿੱਚ the current directory.\n\
ਵਿੱਚ_2 the 3rd and 4th forms, create links ਮੰਜ਼ਿਲ each ਨਿਸ਼ਾਨਾ_2 ਵਿੱਚ DIRECTORY.\n\
Create hard links by ਮੂਲ, symbolic links with --symbolic.\n\
By ਮੂਲ, each destination (name ਦਾ new link) should not already exist.\n\
When creating hard links, each ਨਿਸ਼ਾਨਾ_2 must exist.  Symbolic links\n\
can hold arbitrary ਪਾਠ; ਜੇ later resolved, a relative link is\n\
interpreted ਵਿੱਚ ਸਬੰਧ ਮੰਜ਼ਿਲ its ਮਾਪਾ directory.\n\
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
  -d, -F, --directory\n\
         allow the superuser ਮੰਜ਼ਿਲ attempt ਮੰਜ਼ਿਲ hard link directories,\n\
         ਜੇ supported by the system\n\
"));
      oputs (_("\
  -f, --force\n\
         remove existing destination files\n\
"));
      oputs (_("\
  -i, --interactive\n\
         prompt whether ਮੰਜ਼ਿਲ remove destinations\n\
"));
      oputs (_("\
  -L, --logical\n\
         dereference TARGETs that are symbolic links\n\
"));
      oputs (_("\
  -n, --no-dereference\n\
         treat LINK_NAME as a ਸਾਧਾਰਨ file\n\
         ਜੇ it is a symbolic link ਮੰਜ਼ਿਲ a directory\n\
"));
      oputs (_("\
  -P, --physical\n\
         make hard links directly ਮੰਜ਼ਿਲ symbolic links\n\
"));
      oputs (_("\
  -r, --relative\n\
         with -s, create links relative ਮੰਜ਼ਿਲ link location\n\
"));
      oputs (_("\
  -s, --symbolic\n\
         make symbolic links instead ਦਾ hard links\n\
"));
      oputs (_("\
  -S, --suffix=SUFFIX\n\
         override the usual backup suffix\n\
"));
      oputs (_("\
  -t, --ਨਿਸ਼ਾਨਾ-directory=DIRECTORY\n\
         specify the DIRECTORY ਵਿੱਚ which ਮੰਜ਼ਿਲ create the links\n\
"));
      oputs (_("\
  -T, --no-ਨਿਸ਼ਾਨਾ-directory\n\
         treat LINK_NAME as a ਸਾਧਾਰਨ file always\n\
"));
      oputs (_("\
  -v, --verbose\n\
         print name ਦਾ each linked file\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      emit_backup_suffix_note ();
      ਲਿਖੋ (_("\
\n\
Using -s ignores -L and -P.  Otherwise, the ਆਖਰੀ option specified controls\n\
behavior when a ਨਿਸ਼ਾਨਾ_2 is a symbolic link, defaulting ਮੰਜ਼ਿਲ %s.\n\
"), LINK_FOLLOWS_SYMLINKS ? "-L" : "-P");
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਪੂਰਨ c;
  bool ok;
  bool make_backups = false;
  ਅੱਖਰ ਅਚਲ *backup_suffix = ਨਿਰਾ;
  ਅੱਖਰ *ਸੰਸਕਰਣ_control_string = ਨਿਰਾ;
  ਅੱਖਰ ਅਚਲ *ਨਿਸ਼ਾਨਾ_directory = ਨਿਰਾ;
  ਪੂਰਨ destdir_fd;
  bool no_ਨਿਸ਼ਾਨਾ_directory = false;
  ਪੂਰਨ n_files;
  ਅੱਖਰ **file;
  ਪੂਰਨ link_errno = -1;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdin);

  ਜਦੋਂ ((c = getopt_long (argc, argv, "bdfinrst:vFLPS:T",
                           long_options, ਨਿਰਾ))
         != -1)
    {
      ਚੋਣ (c)
        {
        ਹਾਲਤ 'b':
          make_backups = true;
          ਜੇ (optarg)
            ਸੰਸਕਰਣ_control_string = optarg;
          ਰੋਕੋ;
        ਹਾਲਤ 'd':
        ਹਾਲਤ 'F':
          hard_dir_link = true;
          ਰੋਕੋ;
        ਹਾਲਤ 'f':
          remove_existing_files = true;
          interactive = false;
          ਰੋਕੋ;
        ਹਾਲਤ 'i':
          remove_existing_files = false;
          interactive = true;
          ਰੋਕੋ;
        ਹਾਲਤ 'L':
          logical = true;
          ਰੋਕੋ;
        ਹਾਲਤ 'n':
          dereference_dest_dir_symlinks = false;
          ਰੋਕੋ;
        ਹਾਲਤ 'P':
          logical = false;
          ਰੋਕੋ;
        ਹਾਲਤ 'r':
          relative = true;
          ਰੋਕੋ;
        ਹਾਲਤ 's':
          symbolic_link = true;
          ਰੋਕੋ;
        ਹਾਲਤ 't':
          ਜੇ (ਨਿਸ਼ਾਨਾ_directory)
            error (EXIT_FAILURE, 0, _("multiple target directories specified"));
          ਨਹੀਂਤਾਂ
            {
              ਬਣਤਰ stat st;
              ਜੇ (stat (optarg, &st) != 0)
                error (EXIT_FAILURE, errno, _("failed to access %s"),
                       quoteaf (optarg));
              ਜੇ (! S_ISDIR (st.st_ਬਹੁਲਕ))
                error (EXIT_FAILURE, 0, _("target %s is not a directory"),
                       quoteaf (optarg));
            }
          ਨਿਸ਼ਾਨਾ_directory = optarg;
          ਰੋਕੋ;
        ਹਾਲਤ 'T':
          no_ਨਿਸ਼ਾਨਾ_directory = true;
          ਰੋਕੋ;
        ਹਾਲਤ 'v':
          verbose = true;
          ਰੋਕੋ;
        ਹਾਲਤ 'S':
          make_backups = true;
          backup_suffix = optarg;
          ਰੋਕੋ;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);
        ਮੂਲ:
          usage (EXIT_FAILURE);
          ਰੋਕੋ;
        }
    }

  n_files = argc - optind;
  file = argv + optind;

  ਜੇ (n_files <= 0)
    {
      error (0, 0, _("missing file operand"));
      usage (EXIT_FAILURE);
    }

  ਜੇ (relative && !symbolic_link)
    error (EXIT_FAILURE, 0, _("cannot do --relative without --symbolic"));

  ਜੇ (!hard_dir_link)
    {
      priv_ਸੈੱਟ_remove_linkdir ();
      beware_hard_dir_link = !cannot_unlink_dir ();
    }

  ਜੇ (no_ਨਿਸ਼ਾਨਾ_directory)
    {
      ਜੇ (ਨਿਸ਼ਾਨਾ_directory)
        error (EXIT_FAILURE, 0,
               _("cannot combine --target-directory "
                 "and --no-target-directory"));
      ਜੇ (n_files != 2)
        {
          ਜੇ (n_files < 2)
            error (0, 0,
                   _("missing destination file operand after %s"),
                   quoteaf (file[0]));
          ਨਹੀਂਤਾਂ
            error (0, 0, _("extra operand %s"), quoteaf (file[2]));
          usage (EXIT_FAILURE);
        }
    }
  ਨਹੀਂਤਾਂ ਜੇ (n_files < 2 && !ਨਿਸ਼ਾਨਾ_directory)
    {
      ਨਿਸ਼ਾਨਾ_directory = ".";
      destdir_fd = AT_FDCWD;
    }
  ਨਹੀਂਤਾਂ
    {
      ਜੇ (n_files == 2 && !ਨਿਸ਼ਾਨਾ_directory)
        link_errno = atomic_link (file[0], AT_FDCWD, file[1]);
      ਜੇ (link_errno < 0 || link_errno == EEXIST || link_errno == ENOTDIR
          || link_errno == EINVAL)
        {
          ਅੱਖਰ ਅਚਲ *d
            = ਨਿਸ਼ਾਨਾ_directory ? ਨਿਸ਼ਾਨਾ_directory : file[n_files - 1];
          ਪੂਰਨ flags = (O_PATHSEARCH | O_DIRECTORY
                       | (dereference_dest_dir_symlinks ? 0 : O_NOFOLLOW));
          destdir_fd = openat_safer (AT_FDCWD, d, flags);
          ਜੇ (0 <= destdir_fd)
            {
              n_files -= !ਨਿਸ਼ਾਨਾ_directory;
              ਨਿਸ਼ਾਨਾ_directory = d;
            }
          ਨਹੀਂਤਾਂ ਜੇ (! (n_files == 2 && !ਨਿਸ਼ਾਨਾ_directory))
            error (EXIT_FAILURE, errno, _("target %s"), quoteaf (d));
        }
    }

  backup_type = (make_backups
                 ? xget_ਸੰਸਕਰਣ (_("backup type"), ਸੰਸਕਰਣ_control_string)
                 : no_backups);
  ਸੈੱਟ_simple_backup_suffix (backup_suffix);


  ਜੇ (ਨਿਸ਼ਾਨਾ_directory)
    {
      /* Create the data structure we'll use to record which hard links we
         create.  Used to ensure that ln detects an obscure corner case that
         might result in user data loss.  Create it only if needed.  */
      ਜੇ (2 <= n_files
          && remove_existing_files
          /* Don't bother trying to protect symlinks, since ln clobbering
             a just-created symlink won't ever lead to real data loss.  */
          && ! symbolic_link
          /* No destination hard link can be clobbered when making
             numbered backups.  */
          && backup_type != numbered_backups)
        {
          dest_ਸੈੱਟ = hash_initialize (DEST_INFO_INITIAL_ਸਮਰੱਥਾ,
                                      ਨਿਰਾ,
                                      triple_hash,
                                      triple_compare,
                                      triple_free);
          ਜੇ (dest_ਸੈੱਟ == ਨਿਰਾ)
            xalloc_die ();
        }

      ok = true;
      ਲਈ (ਪੂਰਨ i = 0; i < n_files; ++i)
        {
          ਅੱਖਰ *dest_ਅਧਾਰ;
          ਅੱਖਰ *dest = file_name_concat (ਨਿਸ਼ਾਨਾ_directory,
                                         ਆਖਰੀ_component (file[i]),
                                         &dest_ਅਧਾਰ);
          strip_trailing_slashes (dest_ਅਧਾਰ);
          ok &= do_link (file[i], destdir_fd, dest_ਅਧਾਰ, dest, -1);
          ਮੁਕਤ (dest);
        }
    }
  ਨਹੀਂਤਾਂ
    ok = do_link (file[0], AT_FDCWD, file[1], file[1], link_errno);

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
