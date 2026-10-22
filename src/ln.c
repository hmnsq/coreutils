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

#समावेश <config.h>
#समावेश <stdio.h>
#समावेश <sys/types.h>
#समावेश <getopt.h>

#समावेश "system.h"
#समावेश "backupfile.h"
#समावेश "fcntl-safer.h"
#समावेश "filenamecat.h"
#समावेश "file-set.h"
#समावेश "force-link.h"
#समावेश "hash.h"
#समावेश "hashcode-file.h"
#समावेश "priv-set.h"
#समावेश "relpath.h"
#समावेश "same.h"
#समावेश "unlinkdir.h"
#समावेश "yesno.h"
#समावेश "canonicalize.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "ln"

#घोषणा AUTHORS \
  proper_name ("Mike Parker"), \
  proper_name ("David MacKenzie")

/* FIXME: document */
अटल क्रमागत backup_type backup_type;

/* If true, make symbolic links; otherwise, make hard links.  */
अटल bool symbolic_link;

/* If true, make symbolic links relative  */
अटल bool relative;

/* If true, hard links are logical rather than physical.  */
अटल bool logical = !!LINK_FOLLOWS_SYMLINKS;

/* If true, ask the user before removing existing files.  */
अटल bool interactive;

/* If true, remove existing files unconditionally.  */
अटल bool remove_existing_files;

/* If true, list each file as it is moved. */
अटल bool verbose;

/* If true, allow the superuser to *attempt* to make hard links
   to directories.  However, it appears that this option is not useful
   in practice, since even the superuser is prohibited from hard-linking
   directories on most existing systems (Solaris being an exception).  */
अटल bool hard_dir_link;

/* If true, watch out for creating or removing hard links to directories.  */
अटल bool beware_hard_dir_link;

/* If nonzero, and the specified destination is a symbolic link to a
   directory, treat it just as if it were a directory.  Otherwise, the
   command 'ln --force --no-dereference file symlink-to-dir' deletes
   symlink-to-dir before creating the new link.  */
अटल bool dereference_dest_dir_symlinks = true;

/* This is a set of destination name/inode/dev triples for hard links
   created by ln.  Use this data structure to avoid data loss via a
   sequence of commands like this:
   rm -rf a b c; mkdir a b c; touch a/f b/f; ln -f a/f b/f c && rm -r a b */
अटल Hash_table *dest_set;

/* Initial size of the dest_set hash table.  */
क्रमागत { DEST_INFO_INITIAL_CAPACITY = 61 };

अटल काष्ठा option स्थिर long_options[] =
{
  {"backup", optional_argument, शून्य, 'b'},
  {"directory", no_argument, शून्य, 'F'},
  {"no-dereference", no_argument, शून्य, 'n'},
  {"no-target-directory", no_argument, शून्य, 'T'},
  {"force", no_argument, शून्य, 'f'},
  {"interactive", no_argument, शून्य, 'i'},
  {"suffix", required_argument, शून्य, 'S'},
  {"target-directory", required_argument, शून्य, 't'},
  {"logical", no_argument, शून्य, 'L'},
  {"physical", no_argument, शून्य, 'P'},
  {"relative", no_argument, शून्य, 'r'},
  {"symbolic", no_argument, शून्य, 's'},
  {"verbose", no_argument, शून्य, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {शून्य, 0, शून्य, 0}
};

/* Return an errno value for a system call that returned STATUS.
   This is zero if STATUS is zero, and is errno otherwise.  */

अटल पूर्णांक
errnoize (पूर्णांक status)
{
  वापस status < 0 ? त्रुटि_सं : 0;
}

/* Return FROM represented as relative to the dir of TARGET.
   The result is malloced.  */

अटल अक्षर *
convert_abs_rel (अक्षर स्थिर *from, अक्षर स्थिर *target)
{
  /* Get dirname to generate paths relative to.  We don't resolve
     the full TARGET as the last component could be an existing symlink.  */
  अक्षर *targetdir = dir_name (target);

  अक्षर *realdest = canonicalize_filename_mode (targetdir, CAN_MISSING);
  अक्षर *realfrom = canonicalize_filename_mode (from, CAN_MISSING);

  अक्षर *relative_from = शून्य;
  अगर (realdest && realfrom)
    {
      /* Write to a PATH_MAX buffer.  */
      relative_from = xmalloc (PATH_MAX);

      अगर (!relpath (realfrom, realdest, relative_from, PATH_MAX))
        {
          मुक्त (relative_from);
          relative_from = शून्य;
        }
    }

  मुक्त (targetdir);
  मुक्त (realdest);
  मुक्त (realfrom);

  वापस relative_from ? relative_from : xstrdup (from);
}

/* Link SOURCE to DESTDIR_FD + DEST_BASE atomically.  DESTDIR_FD is
   the directory containing DEST_BASE.  Return 0 if successful, a
   positive errno value on failure, and -1 if an atomic link cannot be
   done.  This handles the common case where the destination does not
   already exist and -r is not specified.  */

अटल पूर्णांक
atomic_link (अक्षर स्थिर *source, पूर्णांक destdir_fd, अक्षर स्थिर *dest_base)
{
  वापस (symbolic_link
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

अटल bool
do_link (अक्षर स्थिर *source, पूर्णांक destdir_fd, अक्षर स्थिर *dest_base,
         अक्षर स्थिर *dest, पूर्णांक link_errno)
{
  काष्ठा stat source_stats;
  पूर्णांक source_status = 1;
  अक्षर *backup_base = शून्य;
  अक्षर *rel_source = शून्य;
  पूर्णांक nofollow_flag = logical ? 0 : AT_SYMLINK_NOFOLLOW;
  अगर (link_errno < 0)
    link_errno = atomic_link (source, destdir_fd, dest_base);

  /* Get SOURCE_STATS if later code will need it, if only for sharper
     diagnostics.  */
  अगर ((link_errno || dest_set) && !symbolic_link)
    {
      source_status = fstatat (AT_FDCWD, source, &source_stats, nofollow_flag);
      अगर (source_status != 0)
        {
          error (0, त्रुटि_सं, _("failed to access %s"), quoteaf (source));
          वापस false;
        }
    }

  अगर (link_errno)
    {
      अगर (!symbolic_link && !hard_dir_link && S_ISDIR (source_stats.st_mode))
        {
          error (0, 0, _("%s: hard link not allowed for directory"),
                 quotef (source));
          वापस false;
        }

      अगर (relative)
        source = rel_source = convert_abs_rel (source, dest);

      bool force = (remove_existing_files || interactive
                    || backup_type != no_backups);
      अगर (force)
        {
          काष्ठा stat dest_stats;
          अगर (fstatat (destdir_fd, dest_base, &dest_stats, AT_SYMLINK_NOFOLLOW)
              != 0)
            {
              अगर (त्रुटि_सं != ENOENT)
                {
                  error (0, त्रुटि_सं, _("failed to access %s"), quoteaf (dest));
                  जाओ fail;
                }
              force = false;
            }
          अन्यथा अगर (S_ISDIR (dest_stats.st_mode))
            {
              error (0, 0, _("%s: cannot overwrite directory"), quotef (dest));
              जाओ fail;
            }
          अन्यथा अगर (seen_file (dest_set, dest, &dest_stats))
            {
              /* The current target was created as a hard link to another
                 source file.  */
              error (0, 0,
                     _("will not overwrite just-created %s with %s"),
                     quoteaf_n (0, dest), quoteaf_n (1, source));
              जाओ fail;
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
              अगर (backup_type != no_backups
                  ? !symbolic_link
                  : remove_existing_files)
                {
                  /* Detect whether removing DEST would also remove SOURCE.
                     If the file has only one link then both are surely the
                     same directory entry.  Otherwise check whether they point
                     to the same name in the same directory.  */
                  अगर (source_status != 0)
                    source_status = stat (source, &source_stats);
                  अगर (source_status == 0
                      && psame_inode (&source_stats, &dest_stats)
                      && (source_stats.st_nlink == 1
                          || same_nameat (AT_FDCWD, source,
                                          destdir_fd, dest_base)))
                    {
                      error (0, 0, _("%s and %s are the same file"),
                             quoteaf_n (0, source), quoteaf_n (1, dest));
                      जाओ fail;
                    }
                }

              अगर (link_errno < 0 || link_errno == EEXIST)
                {
                  अगर (interactive)
                    {
                      ख_लिखो (मानक_त्रुटि, _("%s: replace %s? "),
                               program_name, quoteaf (dest));
                      अगर (!yesno ())
                        {
                          मुक्त (rel_source);
                          वापस false;
                        }
                    }

                  अगर (backup_type != no_backups)
                    {
                      backup_base = find_backup_file_name (destdir_fd,
                                                           dest_base,
                                                           backup_type);
                      अगर (renameat (destdir_fd, dest_base,
                                    destdir_fd, backup_base)
                          != 0)
                        {
                          पूर्णांक rename_errno = त्रुटि_सं;
                          मुक्त (backup_base);
                          backup_base = शून्य;
                          अगर (rename_errno != ENOENT)
                            {
                              error (0, rename_errno, _("cannot backup %s"),
                                     quoteaf (dest));
                              जाओ fail;
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

  अगर (link_errno <= 0)
    {
      /* Right after creating a hard link, do this: (note dest name and
         source_stats, which are also the just-linked-destinations stats) */
      अगर (! symbolic_link)
        record_file (dest_set, dest, &source_stats);

      अगर (verbose)
        {
          अक्षर स्थिर *quoted_backup = "";
          अक्षर स्थिर *backup_sep = "";
          अगर (backup_base)
            {
              अक्षर *backup = backup_base;
              व्योम *alloc = शून्य;
              सूचक_भेद_प्रकार destdirlen = dest_base - dest;
              अगर (0 < destdirlen)
                {
                  alloc = xmalloc (destdirlen + म_माप (backup_base) + 1);
                  backup = स_नकल (alloc, dest, destdirlen);
                  म_नकल (backup + destdirlen, backup_base);
                }
              quoted_backup = quoteaf_n (2, backup);
              backup_sep = " ~ ";
              मुक्त (alloc);
            }
          म_लिखो ("%s%s%s %c> %s\n", quoted_backup, backup_sep,
                  quoteaf_n (0, dest), symbolic_link ? '-' : '=',
                  quoteaf_n (1, source));
        }
    }
  अन्यथा
    {
      अक्षर *dest_quoted = quoteaf_n (0, dest);
      अक्षर *source_quoted = quoteaf_n (1, source);

      अगर (symbolic_link)
        {
          अगर (link_errno != ENAMETOOLONG && *source)
            error (0, link_errno, _("failed to create symbolic link %s"),
                   dest_quoted);
          अन्यथा
            error (0, link_errno, _("failed to create symbolic link %s -> %s"),
                   dest_quoted, source_quoted);
        }
      अन्यथा
        {
          अगर (link_errno == EMLINK)
            error (0, link_errno, _("failed to create hard link to %s"),
                   source_quoted);
          अन्यथा अगर (link_errno == EDQUOT || link_errno == EEXIST
                   || link_errno == ENOSPC || link_errno == EROFS)
            error (0, link_errno, _("failed to create hard link %s"),
                   dest_quoted);
          अन्यथा
            error (0, link_errno, _("failed to create hard link %s => %s"),
                   dest_quoted, source_quoted);
        }

      अगर (backup_base)
        {
          अगर (renameat (destdir_fd, backup_base, destdir_fd, dest_base) != 0)
            error (0, त्रुटि_सं, _("cannot un-backup %s"), quoteaf (dest));
        }
    }

  मुक्त (backup_base);
  मुक्त (rel_source);
  वापस link_errno <= 0;

fail:
  मुक्त (rel_source);
  वापस false;
}

व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s [OPTION]... [-T] TARGET LINK_NAME\n\
  or:  %s [OPTION]... TARGET\n\
  or:  %s [OPTION]... TARGET... DIRECTORY\n\
  or:  %s [OPTION]... -t DIRECTORY TARGET...\n\
"),
              program_name, program_name, program_name, program_name);
      ख_माला_दो (_("\
In the 1st form, create a link to TARGET with the name LINK_NAME.\n\
In the 2nd form, create a link to TARGET in the current directory.\n\
In the 3rd and 4th forms, create links to each TARGET in DIRECTORY.\n\
Create hard links by शेष, symbolic links with --symbolic.\n\
By शेष, each destination (name of new link) should not already exist.\n\
When creating hard links, each TARGET must exist.  Symbolic links\n\
can hold arbitrary text; अगर later resolved, a relative link is\n\
interpreted in relation to its parent directory.\n\
"), मानक_निकास);

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
         अगर supported by the प्रणाली\n\
"));
      oputs (_("\
  -f, --force\n\
         हटाओ existing destination files\n\
"));
      oputs (_("\
  -i, --interactive\n\
         prompt whether to हटाओ destinations\n\
"));
      oputs (_("\
  -L, --logical\n\
         dereference TARGETs that are symbolic links\n\
"));
      oputs (_("\
  -n, --no-dereference\n\
         treat LINK_NAME as a normal file\n\
         अगर it is a symbolic link to a directory\n\
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
      म_लिखो (_("\
\n\
Using -s ignores -L and -P.  Otherwise, the last option specified controls\n\
behavior when a TARGET is a symbolic link, defaulting to %s.\n\
"), LINK_FOLLOWS_SYMLINKS ? "-L" : "-P");
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  पूर्णांक c;
  bool ok;
  bool make_backups = false;
  अक्षर स्थिर *backup_suffix = शून्य;
  अक्षर *version_control_string = शून्य;
  अक्षर स्थिर *target_directory = शून्य;
  पूर्णांक destdir_fd;
  bool no_target_directory = false;
  पूर्णांक n_files;
  अक्षर **file;
  पूर्णांक link_errno = -1;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdin);

  जबतक ((c = getopt_long (argc, argv, "bdfinrst:vFLPS:T",
                           long_options, शून्य))
         != -1)
    {
      चयन (c)
        {
        हाल 'b':
          make_backups = true;
          अगर (optarg)
            version_control_string = optarg;
          अवरोध;
        हाल 'd':
        हाल 'F':
          hard_dir_link = true;
          अवरोध;
        हाल 'f':
          remove_existing_files = true;
          interactive = false;
          अवरोध;
        हाल 'i':
          remove_existing_files = false;
          interactive = true;
          अवरोध;
        हाल 'L':
          logical = true;
          अवरोध;
        हाल 'n':
          dereference_dest_dir_symlinks = false;
          अवरोध;
        हाल 'P':
          logical = false;
          अवरोध;
        हाल 'r':
          relative = true;
          अवरोध;
        हाल 's':
          symbolic_link = true;
          अवरोध;
        हाल 't':
          अगर (target_directory)
            error (निकास_त्रुटि, 0, _("multiple target directories specified"));
          अन्यथा
            {
              काष्ठा stat st;
              अगर (stat (optarg, &st) != 0)
                error (निकास_त्रुटि, त्रुटि_सं, _("failed to access %s"),
                       quoteaf (optarg));
              अगर (! S_ISDIR (st.st_mode))
                error (निकास_त्रुटि, 0, _("target %s is not a directory"),
                       quoteaf (optarg));
            }
          target_directory = optarg;
          अवरोध;
        हाल 'T':
          no_target_directory = true;
          अवरोध;
        हाल 'v':
          verbose = true;
          अवरोध;
        हाल 'S':
          make_backups = true;
          backup_suffix = optarg;
          अवरोध;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        शेष:
          usage (निकास_त्रुटि);
          अवरोध;
        }
    }

  n_files = argc - optind;
  file = argv + optind;

  अगर (n_files <= 0)
    {
      error (0, 0, _("missing file operand"));
      usage (निकास_त्रुटि);
    }

  अगर (relative && !symbolic_link)
    error (निकास_त्रुटि, 0, _("cannot do --relative without --symbolic"));

  अगर (!hard_dir_link)
    {
      priv_set_remove_linkdir ();
      beware_hard_dir_link = !cannot_unlink_dir ();
    }

  अगर (no_target_directory)
    {
      अगर (target_directory)
        error (निकास_त्रुटि, 0,
               _("cannot combine --target-directory "
                 "and --no-target-directory"));
      अगर (n_files != 2)
        {
          अगर (n_files < 2)
            error (0, 0,
                   _("missing destination file operand after %s"),
                   quoteaf (file[0]));
          अन्यथा
            error (0, 0, _("extra operand %s"), quoteaf (file[2]));
          usage (निकास_त्रुटि);
        }
    }
  अन्यथा अगर (n_files < 2 && !target_directory)
    {
      target_directory = ".";
      destdir_fd = AT_FDCWD;
    }
  अन्यथा
    {
      अगर (n_files == 2 && !target_directory)
        link_errno = atomic_link (file[0], AT_FDCWD, file[1]);
      अगर (link_errno < 0 || link_errno == EEXIST || link_errno == ENOTDIR
          || link_errno == EINVAL)
        {
          अक्षर स्थिर *d
            = target_directory ? target_directory : file[n_files - 1];
          पूर्णांक flags = (O_PATHSEARCH | O_DIRECTORY
                       | (dereference_dest_dir_symlinks ? 0 : O_NOFOLLOW));
          destdir_fd = openat_safer (AT_FDCWD, d, flags);
          अगर (0 <= destdir_fd)
            {
              n_files -= !target_directory;
              target_directory = d;
            }
          अन्यथा अगर (! (n_files == 2 && !target_directory))
            error (निकास_त्रुटि, त्रुटि_सं, _("target %s"), quoteaf (d));
        }
    }

  backup_type = (make_backups
                 ? xget_version (_("backup type"), version_control_string)
                 : no_backups);
  set_simple_backup_suffix (backup_suffix);


  अगर (target_directory)
    {
      /* Create the data structure we'll use to record which hard links we
         create.  Used to ensure that ln detects an obscure corner case that
         might result in user data loss.  Create it only if needed.  */
      अगर (2 <= n_files
          && remove_existing_files
          /* Don't bother trying to protect symlinks, since ln clobbering
             a just-created symlink won't ever lead to real data loss.  */
          && ! symbolic_link
          /* No destination hard link can be clobbered when making
             numbered backups.  */
          && backup_type != numbered_backups)
        {
          dest_set = hash_initialize (DEST_INFO_INITIAL_CAPACITY,
                                      शून्य,
                                      triple_hash,
                                      triple_compare,
                                      triple_free);
          अगर (dest_set == शून्य)
            xalloc_die ();
        }

      ok = true;
      क्रम (पूर्णांक i = 0; i < n_files; ++i)
        {
          अक्षर *dest_base;
          अक्षर *dest = file_name_concat (target_directory,
                                         last_component (file[i]),
                                         &dest_base);
          strip_trailing_slashes (dest_base);
          ok &= do_link (file[i], destdir_fd, dest_base, dest, -1);
          मुक्त (dest);
        }
    }
  अन्यथा
    ok = do_link (file[0], AT_FDCWD, file[1], file[1], link_errno);

  main_exit (ok ? निकास_सफल : निकास_त्रुटि);
}
