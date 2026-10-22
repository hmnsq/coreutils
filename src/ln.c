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

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <getopt.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "backupfile.h"
#अन्तर्भाव "fcntl-safer.h"
#अन्तर्भाव "filenamecat.h"
#अन्तर्भाव "file-set.h"
#अन्तर्भाव "force-link.h"
#अन्तर्भाव "hash.h"
#अन्तर्भाव "hashcode-file.h"
#अन्तर्भाव "priv-set.h"
#अन्तर्भाव "relpath.h"
#अन्तर्भाव "same.h"
#अन्तर्भाव "unlinkdir.h"
#अन्तर्भाव "yesno.h"
#अन्तर्भाव "canonicalize.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#परिभाषय क्रमादेशः_NAME "ln"

#परिभाषय AUTHORS \
  proper_name ("Mike Parker"), \
  proper_name ("David MacKenzie")

/* FIXME: document */
स्थिर गणना backup_type backup_type;

/* If true, make symbolic links; otherwise, make hard links.  */
स्थिर bool symbolic_link;

/* If true, make symbolic links relative  */
स्थिर bool relative;

/* If true, hard links are logical rather than physical.  */
स्थिर bool logical = !!LINK_FOLLOWS_SYMLINKS;

/* If true, ask the user before removing existing files.  */
स्थिर bool interactive;

/* If true, remove existing files unconditionally.  */
स्थिर bool remove_existing_files;

/* If true, list each file as it is moved. */
स्थिर bool verbose;

/* If true, allow the superuser to *attempt* to make hard links
   to directories.  However, it appears that this option is not useful
   in practice, since even the superuser is prohibited from hard-linking
   directories on most existing systems (Solaris being an exception).  */
स्थिर bool hard_dir_link;

/* If true, watch out for creating or removing hard links to directories.  */
स्थिर bool beware_hard_dir_link;

/* If nonzero, and the specified destination is a symbolic link to a
   directory, treat it just as if it were a directory.  Otherwise, the
   command 'ln --force --no-dereference file symlink-to-dir' deletes
   symlink-to-dir before creating the new link.  */
स्थिर bool dereference_dest_dir_symlinks = true;

/* This is a set of destination name/inode/dev triples for hard links
   created by ln.  Use this data structure to avoid data loss via a
   sequence of commands like this:
   rm -rf a b c; mkdir a b c; touch a/f b/f; ln -f a/f b/f c && rm -r a b */
स्थिर Hash_सारणी *dest_समुच्चयः;

/* Initial size of the dest_set hash table.  */
गणना { DEST_INFO_INITIAL_क्षमता = 61 };

स्थिर संरचना option ध्रुव long_options[] =
{
  {"backup", optional_तर्कः, रिक्त, 'b'},
  {"directory", no_तर्कः, रिक्त, 'F'},
  {"no-dereference", no_तर्कः, रिक्त, 'n'},
  {"no-target-directory", no_तर्कः, रिक्त, 'T'},
  {"force", no_तर्कः, रिक्त, 'f'},
  {"interactive", no_तर्कः, रिक्त, 'i'},
  {"suffix", required_तर्कः, रिक्त, 'S'},
  {"target-directory", required_तर्कः, रिक्त, 't'},
  {"logical", no_तर्कः, रिक्त, 'L'},
  {"physical", no_तर्कः, रिक्त, 'P'},
  {"relative", no_तर्कः, रिक्त, 'r'},
  {"symbolic", no_तर्कः, रिक्त, 's'},
  {"verbose", no_तर्कः, रिक्त, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_संस्करणम्_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

/* Return an errno value for a system call that returned STATUS.
   This is zero if STATUS is zero, and is errno otherwise.  */

स्थिर पूर्णाङ्क
errnoize (पूर्णाङ्क स्थितिः)
{
  प्रत्यागम स्थितिः < 0 ? errno : 0;
}

/* Return FROM represented as relative to the dir of TARGET.
   The result is malloced.  */

स्थिर वर्ण *
convert_abs_rel (वर्ण ध्रुव *from, वर्ण ध्रुव *target)
{
  /* Get dirname to generate paths relative to.  We don't resolve
     the full TARGET as the last component could be an existing symlink.  */
  वर्ण *targetdir = dir_name (target);

  वर्ण *realdest = canonicalize_filename_बहुलकम् (targetdir, CAN_MISSING);
  वर्ण *realfrom = canonicalize_filename_बहुलकम् (from, CAN_MISSING);

  वर्ण *relative_from = रिक्त;
  यदि (realdest && realfrom)
    {
      /* Write to a PATH_MAX buffer.  */
      relative_from = xmalloc (PATH_MAX);

      यदि (!relpath (realfrom, realdest, relative_from, PATH_MAX))
        {
          मुक्त (relative_from);
          relative_from = रिक्त;
        }
    }

  मुक्त (targetdir);
  मुक्त (realdest);
  मुक्त (realfrom);

  प्रत्यागम relative_from ? relative_from : xstrdup (from);
}

/* Link SOURCE to DESTDIR_FD + DEST_BASE atomically.  DESTDIR_FD is
   the directory containing DEST_BASE.  Return 0 if successful, a
   positive errno value on failure, and -1 if an atomic link cannot be
   done.  This handles the common case where the destination does not
   already exist and -r is not specified.  */

स्थिर पूर्णाङ्क
atomic_link (वर्ण ध्रुव *source, पूर्णाङ्क destdir_fd, वर्ण ध्रुव *dest_आधारः)
{
  प्रत्यागम (symbolic_link
          ? (relative ? -1
             : errnoize (symlinkat (source, destdir_fd, dest_आधारः)))
          : beware_hard_dir_link ? -1
          : errnoize (linkat (AT_FDCWD, source, destdir_fd, dest_आधारः,
                              logical ? AT_SYMLINK_FOLLOW : 0)));
}

/* Link SOURCE to a directory entry under DESTDIR_FD named DEST_BASE.
   DEST is the full name of the destination, useful for diagnostics.
   LINK_ERRNO is zero if the link has already been made,
   positive if attempting the link failed with errno == LINK_ERRNO,
   -1 if no attempt has been made to create the link.
   Return true if successful.  */

स्थिर bool
do_link (वर्ण ध्रुव *source, पूर्णाङ्क destdir_fd, वर्ण ध्रुव *dest_आधारः,
         वर्ण ध्रुव *dest, पूर्णाङ्क link_errno)
{
  संरचना stat source_stats;
  पूर्णाङ्क source_स्थितिः = 1;
  वर्ण *backup_आधारः = रिक्त;
  वर्ण *rel_source = रिक्त;
  पूर्णाङ्क nofollow_flag = logical ? 0 : AT_SYMLINK_NOFOLLOW;
  यदि (link_errno < 0)
    link_errno = atomic_link (source, destdir_fd, dest_आधारः);

  /* Get SOURCE_STATS if later code will need it, if only for sharper
     diagnostics.  */
  यदि ((link_errno || dest_समुच्चयः) && !symbolic_link)
    {
      source_स्थितिः = fstatat (AT_FDCWD, source, &source_stats, nofollow_flag);
      यदि (source_स्थितिः != 0)
        {
          error (0, errno, _("failed to access %s"), quoteaf (source));
          प्रत्यागम false;
        }
    }

  यदि (link_errno)
    {
      यदि (!symbolic_link && !hard_dir_link && S_ISDIR (source_stats.st_बहुलकम्))
        {
          error (0, 0, _("%s: hard link not allowed for directory"),
                 quotef (source));
          प्रत्यागम false;
        }

      यदि (relative)
        source = rel_source = convert_abs_rel (source, dest);

      bool force = (remove_existing_files || interactive
                    || backup_type != no_backups);
      यदि (force)
        {
          संरचना stat dest_stats;
          यदि (fstatat (destdir_fd, dest_आधारः, &dest_stats, AT_SYMLINK_NOFOLLOW)
              != 0)
            {
              यदि (errno != ENOENT)
                {
                  error (0, errno, _("failed to access %s"), quoteaf (dest));
                  गच्छ fail;
                }
              force = false;
            }
          अन्यथा यदि (S_ISDIR (dest_stats.st_बहुलकम्))
            {
              error (0, 0, _("%s: cannot overwrite directory"), quotef (dest));
              गच्छ fail;
            }
          अन्यथा यदि (आलोकितम्_file (dest_समुच्चयः, dest, &dest_stats))
            {
              /* The current target was created as a hard link to another
                 source file.  */
              error (0, 0,
                     _("will not overwrite just-created %s with %s"),
                     quoteaf_n (0, dest), quoteaf_n (1, source));
              गच्छ fail;
            }
          अन्यथा
            {
              /* Beware removing DEST if it is the same directory entry as
                 SOURCE, because in that case removing DEST can cause the
                 subsequent link creation either to fail (for hard links), or
                 to replace a non-symlink DEST with a self-loop (for symbolic
                 links) which loses the contents of DEST.  So, when backing
                 up, worry about creating hard links (since the backups cover
                 the symlink case); otherwise, worry about about -f.  */
              यदि (backup_type != no_backups
                  ? !symbolic_link
                  : remove_existing_files)
                {
                  /* Detect whether removing DEST would also remove SOURCE.
                     If the file has only one link then both are surely the
                     same directory entry.  Otherwise check whether they point
                     to the same name in the same directory.  */
                  यदि (source_स्थितिः != 0)
                    source_स्थितिः = stat (source, &source_stats);
                  यदि (source_स्थितिः == 0
                      && psame_inode (&source_stats, &dest_stats)
                      && (source_stats.st_nlink == 1
                          || same_nameat (AT_FDCWD, source,
                                          destdir_fd, dest_आधारः)))
                    {
                      error (0, 0, _("%s and %s are the same file"),
                             quoteaf_n (0, source), quoteaf_n (1, dest));
                      गच्छ fail;
                    }
                }

              यदि (link_errno < 0 || link_errno == EEXIST)
                {
                  यदि (interactive)
                    {
                      fprintf (stderr, _("%s: replace %s? "),
                               क्रमादेशः_name, quoteaf (dest));
                      यदि (!yesno ())
                        {
                          मुक्त (rel_source);
                          प्रत्यागम false;
                        }
                    }

                  यदि (backup_type != no_backups)
                    {
                      backup_आधारः = find_backup_file_name (destdir_fd,
                                                           dest_आधारः,
                                                           backup_type);
                      यदि (renameat (destdir_fd, dest_आधारः,
                                    destdir_fd, backup_आधारः)
                          != 0)
                        {
                          पूर्णाङ्क rename_errno = errno;
                          मुक्त (backup_आधारः);
                          backup_आधारः = रिक्त;
                          यदि (rename_errno != ENOENT)
                            {
                              error (0, rename_errno, _("cannot backup %s"),
                                     quoteaf (dest));
                              गच्छ fail;
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
           ? force_symlinkat (source, destdir_fd, dest_आधारः,
                              force, link_errno)
           : force_linkat (AT_FDCWD, source, destdir_fd, dest_आधारः,
                           logical ? AT_SYMLINK_FOLLOW : 0,
                           force, link_errno));
      /* Until now, link_errno < 0 meant the link has not been tried.
         From here on, link_errno < 0 means the link worked but
         required removing the destination first.  */
    }

  यदि (link_errno <= 0)
    {
      /* Right after creating a hard link, do this: (note dest name and
         source_stats, which are also the just-linked-destinations stats) */
      यदि (! symbolic_link)
        लेखः_file (dest_समुच्चयः, dest, &source_stats);

      यदि (verbose)
        {
          वर्ण ध्रुव *quoted_backup = "";
          वर्ण ध्रुव *backup_sep = "";
          यदि (backup_आधारः)
            {
              वर्ण *backup = backup_आधारः;
              शून्य *alloc = रिक्त;
              ptrdiff_t destdirlen = dest_आधारः - dest;
              यदि (0 < destdirlen)
                {
                  alloc = xmalloc (destdirlen + वाक्यदैर्घ्यम् (backup_आधारः) + 1);
                  backup = स्मृतिं_प्रतिलिख (alloc, dest, destdirlen);
                  वाक्यं_प्रतिलिख (backup + destdirlen, backup_आधारः);
                }
              quoted_backup = quoteaf_n (2, backup);
              backup_sep = " ~ ";
              मुक्त (alloc);
            }
          प्रारूप_लिख ("%s%s%s %c> %s\n", quoted_backup, backup_sep,
                  quoteaf_n (0, dest), symbolic_link ? '-' : '=',
                  quoteaf_n (1, source));
        }
    }
  अन्यथा
    {
      वर्ण *dest_quoted = quoteaf_n (0, dest);
      वर्ण *source_quoted = quoteaf_n (1, source);

      यदि (symbolic_link)
        {
          यदि (link_errno != ENAMETOOLONG && *source)
            error (0, link_errno, _("failed to create symbolic link %s"),
                   dest_quoted);
          अन्यथा
            error (0, link_errno, _("failed to create symbolic link %s -> %s"),
                   dest_quoted, source_quoted);
        }
      अन्यथा
        {
          यदि (link_errno == EMLINK)
            error (0, link_errno, _("failed to create hard link to %s"),
                   source_quoted);
          अन्यथा यदि (link_errno == EDQUOT || link_errno == EEXIST
                   || link_errno == ENOSPC || link_errno == EROFS)
            error (0, link_errno, _("failed to create hard link %s"),
                   dest_quoted);
          अन्यथा
            error (0, link_errno, _("failed to create hard link %s => %s"),
                   dest_quoted, source_quoted);
        }

      यदि (backup_आधारः)
        {
          यदि (renameat (destdir_fd, backup_आधारः, destdir_fd, dest_आधारः) != 0)
            error (0, errno, _("cannot un-backup %s"), quoteaf (dest));
        }
    }

  मुक्त (backup_आधारः);
  मुक्त (rel_source);
  प्रत्यागम link_errno <= 0;

fail:
  मुक्त (rel_source);
  प्रत्यागम false;
}

शून्य
usage (पूर्णाङ्क स्थितिः)
{
  यदि (स्थितिः != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
    {
      प्रारूप_लिख (_("\
Usage: %s [OPTION]... [-T] TARGET LINK_NAME\n\
  or:  %s [OPTION]... TARGET\n\
  or:  %s [OPTION]... TARGET... DIRECTORY\n\
  or:  %s [OPTION]... -t DIRECTORY TARGET...\n\
"),
              क्रमादेशः_name, क्रमादेशः_name, क्रमादेशः_name, क्रमादेशः_name);
      fputs (_("\
अन्तर्_2 the 1st form, create a link गन्तव्यम् TARGET with the name LINK_NAME.\n\
अन्तर्_2 the 2nd form, create a link गन्तव्यम् TARGET अन्तर् the current directory.\n\
अन्तर्_2 the 3rd and 4th forms, create links गन्तव्यम् each TARGET अन्तर् DIRECTORY.\n\
Create hard links by स्वभाव, symbolic links with --symbolic.\n\
By स्वभाव, each destination (name सम्बन्धि new link) should not already exist.\n\
When creating hard links, each TARGET must exist.  Symbolic links\n\
can hold arbitrary text; यदि later resolved, a relative link is\n\
interpreted अन्तर् सम्बन्धः गन्तव्यम् its parent directory.\n\
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
  -d, -F, --directory\n\
         allow the superuser गन्तव्यम् attempt गन्तव्यम् hard link directories,\n\
         यदि supported by the system\n\
"));
      oputs (_("\
  -f, --force\n\
         remove existing destination files\n\
"));
      oputs (_("\
  -i, --interactive\n\
         prompt whether गन्तव्यम् remove destinations\n\
"));
      oputs (_("\
  -L, --logical\n\
         dereference TARGETs that are symbolic links\n\
"));
      oputs (_("\
  -n, --no-dereference\n\
         treat LINK_NAME as a प्रसामान्यम् file\n\
         यदि it is a symbolic link गन्तव्यम् a directory\n\
"));
      oputs (_("\
  -P, --physical\n\
         make hard links directly गन्तव्यम् symbolic links\n\
"));
      oputs (_("\
  -r, --relative\n\
         with -s, create links relative गन्तव्यम् link location\n\
"));
      oputs (_("\
  -s, --symbolic\n\
         make symbolic links instead सम्बन्धि hard links\n\
"));
      oputs (_("\
  -S, --suffix=SUFFIX\n\
         override the usual backup suffix\n\
"));
      oputs (_("\
  -t, --target-directory=DIRECTORY\n\
         specify the DIRECTORY अन्तर् which गन्तव्यम् create the links\n\
"));
      oputs (_("\
  -T, --no-target-directory\n\
         treat LINK_NAME as a प्रसामान्यम् file always\n\
"));
      oputs (_("\
  -v, --verbose\n\
         print name सम्बन्धि each linked file\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (संस्करणम्_OPTION_DESCRIPTION);
      emit_backup_suffix_note ();
      प्रारूप_लिख (_("\
\n\
Using -s ignores -L and -P.  Otherwise, the चरमः option specified controls\n\
behavior when a TARGET is a symbolic link, defaulting गन्तव्यम् %s.\n\
"), LINK_FOLLOWS_SYMLINKS ? "-L" : "-P");
      emit_ancillary_info (क्रमादेशः_NAME);
    }
  निर्गच्छ (स्थितिः);
}

पूर्णाङ्क
मुख्यम् (पूर्णाङ्क argc, वर्ण **argv)
{
  पूर्णाङ्क c;
  bool ok;
  bool make_backups = false;
  वर्ण ध्रुव *backup_suffix = रिक्त;
  वर्ण *संस्करणम्_control_string = रिक्त;
  वर्ण ध्रुव *target_directory = रिक्त;
  पूर्णाङ्क destdir_fd;
  bool no_target_directory = false;
  पूर्णाङ्क n_files;
  वर्ण **file;
  पूर्णाङ्क link_errno = -1;

  initialize_main (&argc, &argv);
  समुच्चयः_क्रमादेशः_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdin);

  यावत् ((c = getopt_long (argc, argv, "bdfinrst:vFLPS:T",
                           long_options, रिक्त))
         != -1)
    {
      विकल्प (c)
        {
        पक्ष 'b':
          make_backups = true;
          यदि (optarg)
            संस्करणम्_control_string = optarg;
          भङ्ग;
        पक्ष 'd':
        पक्ष 'F':
          hard_dir_link = true;
          भङ्ग;
        पक्ष 'f':
          remove_existing_files = true;
          interactive = false;
          भङ्ग;
        पक्ष 'i':
          remove_existing_files = false;
          interactive = true;
          भङ्ग;
        पक्ष 'L':
          logical = true;
          भङ्ग;
        पक्ष 'n':
          dereference_dest_dir_symlinks = false;
          भङ्ग;
        पक्ष 'P':
          logical = false;
          भङ्ग;
        पक्ष 'r':
          relative = true;
          भङ्ग;
        पक्ष 's':
          symbolic_link = true;
          भङ्ग;
        पक्ष 't':
          यदि (target_directory)
            error (EXIT_FAILURE, 0, _("multiple target directories specified"));
          अन्यथा
            {
              संरचना stat st;
              यदि (stat (optarg, &st) != 0)
                error (EXIT_FAILURE, errno, _("failed to access %s"),
                       quoteaf (optarg));
              यदि (! S_ISDIR (st.st_बहुलकम्))
                error (EXIT_FAILURE, 0, _("target %s is not a directory"),
                       quoteaf (optarg));
            }
          target_directory = optarg;
          भङ्ग;
        पक्ष 'T':
          no_target_directory = true;
          भङ्ग;
        पक्ष 'v':
          verbose = true;
          भङ्ग;
        पक्ष 'S':
          make_backups = true;
          backup_suffix = optarg;
          भङ्ग;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_संस्करणम्_CHAR (क्रमादेशः_NAME, AUTHORS);
        स्वभाव:
          usage (EXIT_FAILURE);
          भङ्ग;
        }
    }

  n_files = argc - optind;
  file = argv + optind;

  यदि (n_files <= 0)
    {
      error (0, 0, _("missing file operand"));
      usage (EXIT_FAILURE);
    }

  यदि (relative && !symbolic_link)
    error (EXIT_FAILURE, 0, _("cannot do --relative without --symbolic"));

  यदि (!hard_dir_link)
    {
      priv_समुच्चयः_remove_linkdir ();
      beware_hard_dir_link = !cannot_unlink_dir ();
    }

  यदि (no_target_directory)
    {
      यदि (target_directory)
        error (EXIT_FAILURE, 0,
               _("cannot combine --target-directory "
                 "and --no-target-directory"));
      यदि (n_files != 2)
        {
          यदि (n_files < 2)
            error (0, 0,
                   _("missing destination file operand after %s"),
                   quoteaf (file[0]));
          अन्यथा
            error (0, 0, _("extra operand %s"), quoteaf (file[2]));
          usage (EXIT_FAILURE);
        }
    }
  अन्यथा यदि (n_files < 2 && !target_directory)
    {
      target_directory = ".";
      destdir_fd = AT_FDCWD;
    }
  अन्यथा
    {
      यदि (n_files == 2 && !target_directory)
        link_errno = atomic_link (file[0], AT_FDCWD, file[1]);
      यदि (link_errno < 0 || link_errno == EEXIST || link_errno == ENOTDIR
          || link_errno == EINVAL)
        {
          वर्ण ध्रुव *d
            = target_directory ? target_directory : file[n_files - 1];
          पूर्णाङ्क flags = (O_PATHSEARCH | O_DIRECTORY
                       | (dereference_dest_dir_symlinks ? 0 : O_NOFOLLOW));
          destdir_fd = openat_safer (AT_FDCWD, d, flags);
          यदि (0 <= destdir_fd)
            {
              n_files -= !target_directory;
              target_directory = d;
            }
          अन्यथा यदि (! (n_files == 2 && !target_directory))
            error (EXIT_FAILURE, errno, _("target %s"), quoteaf (d));
        }
    }

  backup_type = (make_backups
                 ? xget_संस्करणम् (_("backup type"), संस्करणम्_control_string)
                 : no_backups);
  समुच्चयः_simple_backup_suffix (backup_suffix);


  यदि (target_directory)
    {
      /* Create the data structure we'll use to record which hard links we
         create.  Used to ensure that ln detects an obscure corner case that
         might result in user data loss.  Create it only if needed.  */
      यदि (2 <= n_files
          && remove_existing_files
          /* Don't bother trying to protect symlinks, since ln clobbering
             a just-created symlink won't ever lead to real data loss.  */
          && ! symbolic_link
          /* No destination hard link can be clobbered when making
             numbered backups.  */
          && backup_type != numbered_backups)
        {
          dest_समुच्चयः = hash_initialize (DEST_INFO_INITIAL_क्षमता,
                                      रिक्त,
                                      triple_hash,
                                      triple_compare,
                                      triple_free);
          यदि (dest_समुच्चयः == रिक्त)
            xalloc_die ();
        }

      ok = true;
      प्रति (पूर्णाङ्क i = 0; i < n_files; ++i)
        {
          वर्ण *dest_आधारः;
          वर्ण *dest = file_name_concat (target_directory,
                                         चरमः_component (file[i]),
                                         &dest_आधारः);
          strip_trailing_slashes (dest_आधारः);
          ok &= do_link (file[i], destdir_fd, dest_आधारः, dest, -1);
          मुक्त (dest);
        }
    }
  अन्यथा
    ok = do_link (file[0], AT_FDCWD, file[1], file[1], link_errno);

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
