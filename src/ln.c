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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <getopt.h>

#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "backupfile.h"
#അന്തര്ഭാവ "fcntl-safer.h"
#അന്തര്ഭാവ "filenamecat.h"
#അന്തര്ഭാവ "file-set.h"
#അന്തര്ഭാവ "force-link.h"
#അന്തര്ഭാവ "hash.h"
#അന്തര്ഭാവ "hashcode-file.h"
#അന്തര്ഭാവ "priv-set.h"
#അന്തര്ഭാവ "relpath.h"
#അന്തര്ഭാവ "same.h"
#അന്തര്ഭാവ "unlinkdir.h"
#അന്തര്ഭാവ "yesno.h"
#അന്തര്ഭാവ "canonicalize.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "ln"

#define AUTHORS \
  proper_name ("Mike Parker"), \
  proper_name ("David MacKenzie")

/* FIXME: document */
സ്ഥിതം എണ്ണൽ backup_type backup_type;

/* If true, make symbolic links; otherwise, make hard links.  */
സ്ഥിതം bool symbolic_link;

/* If true, make symbolic links relative  */
സ്ഥിതം bool relative;

/* If true, hard links are logical rather than physical.  */
സ്ഥിതം bool logical = !!LINK_FOLLOWS_SYMLINKS;

/* If true, ask the user before removing existing files.  */
സ്ഥിതം bool interactive;

/* If true, remove existing files unconditionally.  */
സ്ഥിതം bool remove_existing_files;

/* If true, list each file as it is moved. */
സ്ഥിതം bool verbose;

/* If true, allow the superuser to *attempt* to make hard links
   to directories.  However, it appears that this option is not useful
   in practice, since even the superuser is prohibited from hard-linking
   directories on most existing systems (Solaris being an exception).  */
സ്ഥിതം bool hard_dir_link;

/* If true, watch out for creating or removing hard links to directories.  */
സ്ഥിതം bool beware_hard_dir_link;

/* If nonzero, and the specified destination is a symbolic link to a
   directory, treat it just as if it were a directory.  Otherwise, the
   command 'ln --force --no-dereference file symlink-to-dir' deletes
   symlink-to-dir before creating the new link.  */
സ്ഥിതം bool dereference_dest_dir_symlinks = true;

/* This is a set of destination name/inode/dev triples for hard links
   created by ln.  Use this data structure to avoid data loss via a
   sequence of commands like this:
   rm -rf a b c; mkdir a b c; touch a/f b/f; ln -f a/f b/f c && rm -r a b */
സ്ഥിതം Hash_table *dest_set;

/* Initial size of the dest_set hash table.  */
എണ്ണൽ { DEST_INFO_INITIAL_CAPACITY = 61 };

സ്ഥിതം ഘടന option സ്ഥിരം long_options[] =
{
  {"backup", optional_argument, ശൂന്യ, 'b'},
  {"directory", no_argument, ശൂന്യ, 'F'},
  {"no-dereference", no_argument, ശൂന്യ, 'n'},
  {"no-target-directory", no_argument, ശൂന്യ, 'T'},
  {"force", no_argument, ശൂന്യ, 'f'},
  {"interactive", no_argument, ശൂന്യ, 'i'},
  {"suffix", required_argument, ശൂന്യ, 'S'},
  {"target-directory", required_argument, ശൂന്യ, 't'},
  {"logical", no_argument, ശൂന്യ, 'L'},
  {"physical", no_argument, ശൂന്യ, 'P'},
  {"relative", no_argument, ശൂന്യ, 'r'},
  {"symbolic", no_argument, ശൂന്യ, 's'},
  {"verbose", no_argument, ശൂന്യ, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ശൂന്യ, 0, ശൂന്യ, 0}
};

/* Return an errno value for a system call that returned STATUS.
   This is zero if STATUS is zero, and is errno otherwise.  */

സ്ഥിതം പൂർണ്ണസംഖ്യ
errnoize (പൂർണ്ണസംഖ്യ status)
{
  തിരികെ status < 0 ? errno : 0;
}

/* Return FROM represented as relative to the dir of TARGET.
   The result is malloced.  */

സ്ഥിതം അക്ഷരം *
convert_abs_rel (അക്ഷരം സ്ഥിരം *from, അക്ഷരം സ്ഥിരം *target)
{
  /* Get dirname to generate paths relative to.  We don't resolve
     the full TARGET as the last component could be an existing symlink.  */
  അക്ഷരം *targetdir = dir_name (target);

  അക്ഷരം *realdest = canonicalize_filename_mode (targetdir, CAN_MISSING);
  അക്ഷരം *realfrom = canonicalize_filename_mode (from, CAN_MISSING);

  അക്ഷരം *relative_from = ശൂന്യ;
  എങ്കിൽ (realdest && realfrom)
    {
      /* Write to a PATH_MAX buffer.  */
      relative_from = xmalloc (PATH_MAX);

      എങ്കിൽ (!relpath (realfrom, realdest, relative_from, PATH_MAX))
        {
          സ്വതന്ത്രം (relative_from);
          relative_from = ശൂന്യ;
        }
    }

  സ്വതന്ത്രം (targetdir);
  സ്വതന്ത്രം (realdest);
  സ്വതന്ത്രം (realfrom);

  തിരികെ relative_from ? relative_from : xstrdup (from);
}

/* Link SOURCE to DESTDIR_FD + DEST_BASE atomically.  DESTDIR_FD is
   the directory containing DEST_BASE.  Return 0 if successful, a
   positive errno value on failure, and -1 if an atomic link cannot be
   done.  This handles the common case where the destination does not
   already exist and -r is not specified.  */

സ്ഥിതം പൂർണ്ണസംഖ്യ
atomic_link (അക്ഷരം സ്ഥിരം *source, പൂർണ്ണസംഖ്യ destdir_fd, അക്ഷരം സ്ഥിരം *dest_base)
{
  തിരികെ (symbolic_link
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

സ്ഥിതം bool
do_link (അക്ഷരം സ്ഥിരം *source, പൂർണ്ണസംഖ്യ destdir_fd, അക്ഷരം സ്ഥിരം *dest_base,
         അക്ഷരം സ്ഥിരം *dest, പൂർണ്ണസംഖ്യ link_errno)
{
  ഘടന stat source_stats;
  പൂർണ്ണസംഖ്യ source_status = 1;
  അക്ഷരം *backup_base = ശൂന്യ;
  അക്ഷരം *rel_source = ശൂന്യ;
  പൂർണ്ണസംഖ്യ nofollow_flag = logical ? 0 : AT_SYMLINK_NOFOLLOW;
  എങ്കിൽ (link_errno < 0)
    link_errno = atomic_link (source, destdir_fd, dest_base);

  /* Get SOURCE_STATS if later code will need it, if only for sharper
     diagnostics.  */
  എങ്കിൽ ((link_errno || dest_set) && !symbolic_link)
    {
      source_status = fstatat (AT_FDCWD, source, &source_stats, nofollow_flag);
      എങ്കിൽ (source_status != 0)
        {
          error (0, errno, _("failed to access %s"), quoteaf (source));
          തിരികെ false;
        }
    }

  എങ്കിൽ (link_errno)
    {
      എങ്കിൽ (!symbolic_link && !hard_dir_link && S_ISDIR (source_stats.st_mode))
        {
          error (0, 0, _("%s: hard link not allowed for directory"),
                 quotef (source));
          തിരികെ false;
        }

      എങ്കിൽ (relative)
        source = rel_source = convert_abs_rel (source, dest);

      bool force = (remove_existing_files || interactive
                    || backup_type != no_backups);
      എങ്കിൽ (force)
        {
          ഘടന stat dest_stats;
          എങ്കിൽ (fstatat (destdir_fd, dest_base, &dest_stats, AT_SYMLINK_NOFOLLOW)
              != 0)
            {
              എങ്കിൽ (errno != ENOENT)
                {
                  error (0, errno, _("failed to access %s"), quoteaf (dest));
                  പോകുക fail;
                }
              force = false;
            }
          അല്ലെങ്കിൽ എങ്കിൽ (S_ISDIR (dest_stats.st_mode))
            {
              error (0, 0, _("%s: cannot overwrite directory"), quotef (dest));
              പോകുക fail;
            }
          അല്ലെങ്കിൽ എങ്കിൽ (seen_file (dest_set, dest, &dest_stats))
            {
              /* The current target was created as a hard link to another
                 source file.  */
              error (0, 0,
                     _("will not overwrite just-created %s with %s"),
                     quoteaf_n (0, dest), quoteaf_n (1, source));
              പോകുക fail;
            }
          അല്ലെങ്കിൽ
            {
              /* Beware removing DEST if it is the same directory entry as
                 SOURCE, because in that case removing DEST can cause the
                 subsequent link creation either to fail (for hard links), or
                 to replace a non-symlink DEST with a self-loop (for symbolic
                 links) which loses the contents of DEST.  So, when backing
                 up, worry about creating hard links (since the backups cover
                 the symlink case); otherwise, worry about about -f.  */
              എങ്കിൽ (backup_type != no_backups
                  ? !symbolic_link
                  : remove_existing_files)
                {
                  /* Detect whether removing DEST would also remove SOURCE.
                     If the file has only one link then both are surely the
                     same directory entry.  Otherwise check whether they point
                     to the same name in the same directory.  */
                  എങ്കിൽ (source_status != 0)
                    source_status = stat (source, &source_stats);
                  എങ്കിൽ (source_status == 0
                      && psame_inode (&source_stats, &dest_stats)
                      && (source_stats.st_nlink == 1
                          || same_nameat (AT_FDCWD, source,
                                          destdir_fd, dest_base)))
                    {
                      error (0, 0, _("%s and %s are the same file"),
                             quoteaf_n (0, source), quoteaf_n (1, dest));
                      പോകുക fail;
                    }
                }

              എങ്കിൽ (link_errno < 0 || link_errno == EEXIST)
                {
                  എങ്കിൽ (interactive)
                    {
                      fprintf (stderr, _("%s: replace %s? "),
                               program_name, quoteaf (dest));
                      എങ്കിൽ (!yesno ())
                        {
                          സ്വതന്ത്രം (rel_source);
                          തിരികെ false;
                        }
                    }

                  എങ്കിൽ (backup_type != no_backups)
                    {
                      backup_base = find_backup_file_name (destdir_fd,
                                                           dest_base,
                                                           backup_type);
                      എങ്കിൽ (renameat (destdir_fd, dest_base,
                                    destdir_fd, backup_base)
                          != 0)
                        {
                          പൂർണ്ണസംഖ്യ rename_errno = errno;
                          സ്വതന്ത്രം (backup_base);
                          backup_base = ശൂന്യ;
                          എങ്കിൽ (rename_errno != ENOENT)
                            {
                              error (0, rename_errno, _("cannot backup %s"),
                                     quoteaf (dest));
                              പോകുക fail;
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

  എങ്കിൽ (link_errno <= 0)
    {
      /* Right after creating a hard link, do this: (note dest name and
         source_stats, which are also the just-linked-destinations stats) */
      എങ്കിൽ (! symbolic_link)
        record_file (dest_set, dest, &source_stats);

      എങ്കിൽ (verbose)
        {
          അക്ഷരം സ്ഥിരം *quoted_backup = "";
          അക്ഷരം സ്ഥിരം *backup_sep = "";
          എങ്കിൽ (backup_base)
            {
              അക്ഷരം *backup = backup_base;
              ശൂന്യം *alloc = ശൂന്യ;
              ptrdiff_t destdirlen = dest_base - dest;
              എങ്കിൽ (0 < destdirlen)
                {
                  alloc = xmalloc (destdirlen + strlen (backup_base) + 1);
                  backup = memcpy (alloc, dest, destdirlen);
                  strcpy (backup + destdirlen, backup_base);
                }
              quoted_backup = quoteaf_n (2, backup);
              backup_sep = " ~ ";
              സ്വതന്ത്രം (alloc);
            }
          printf ("%s%s%s %c> %s\n", quoted_backup, backup_sep,
                  quoteaf_n (0, dest), symbolic_link ? '-' : '=',
                  quoteaf_n (1, source));
        }
    }
  അല്ലെങ്കിൽ
    {
      അക്ഷരം *dest_quoted = quoteaf_n (0, dest);
      അക്ഷരം *source_quoted = quoteaf_n (1, source);

      എങ്കിൽ (symbolic_link)
        {
          എങ്കിൽ (link_errno != ENAMETOOLONG && *source)
            error (0, link_errno, _("failed to create symbolic link %s"),
                   dest_quoted);
          അല്ലെങ്കിൽ
            error (0, link_errno, _("failed to create symbolic link %s -> %s"),
                   dest_quoted, source_quoted);
        }
      അല്ലെങ്കിൽ
        {
          എങ്കിൽ (link_errno == EMLINK)
            error (0, link_errno, _("failed to create hard link to %s"),
                   source_quoted);
          അല്ലെങ്കിൽ എങ്കിൽ (link_errno == EDQUOT || link_errno == EEXIST
                   || link_errno == ENOSPC || link_errno == EROFS)
            error (0, link_errno, _("failed to create hard link %s"),
                   dest_quoted);
          അല്ലെങ്കിൽ
            error (0, link_errno, _("failed to create hard link %s => %s"),
                   dest_quoted, source_quoted);
        }

      എങ്കിൽ (backup_base)
        {
          എങ്കിൽ (renameat (destdir_fd, backup_base, destdir_fd, dest_base) != 0)
            error (0, errno, _("cannot un-backup %s"), quoteaf (dest));
        }
    }

  സ്വതന്ത്രം (backup_base);
  സ്വതന്ത്രം (rel_source);
  തിരികെ link_errno <= 0;

fail:
  സ്വതന്ത്രം (rel_source);
  തിരികെ false;
}

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
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
Create hard links by സ്ഥിരസ്ഥിതി, symbolic links with --symbolic.\n\
By സ്ഥിരസ്ഥിതി, each destination (name of new link) should not already exist.\n\
When creating hard links, each TARGET must exist.  Symbolic links\n\
can hold arbitrary text; എങ്കിൽ later resolved, a relative link is\n\
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
         എങ്കിൽ supported by the system\n\
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
         എങ്കിൽ it is a symbolic link to a directory\n\
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

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  പൂർണ്ണസംഖ്യ c;
  bool ok;
  bool make_backups = false;
  അക്ഷരം സ്ഥിരം *backup_suffix = ശൂന്യ;
  അക്ഷരം *version_control_string = ശൂന്യ;
  അക്ഷരം സ്ഥിരം *target_directory = ശൂന്യ;
  പൂർണ്ണസംഖ്യ destdir_fd;
  bool no_target_directory = false;
  പൂർണ്ണസംഖ്യ n_files;
  അക്ഷരം **file;
  പൂർണ്ണസംഖ്യ link_errno = -1;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdin);

  ആകുമ്പോൾ ((c = getopt_long (argc, argv, "bdfinrst:vFLPS:T",
                           long_options, ശൂന്യ))
         != -1)
    {
      തിരഞ്ഞെടുപ്പ് (c)
        {
        അവസ്ഥ 'b':
          make_backups = true;
          എങ്കിൽ (optarg)
            version_control_string = optarg;
          നിർത്തുക;
        അവസ്ഥ 'd':
        അവസ്ഥ 'F':
          hard_dir_link = true;
          നിർത്തുക;
        അവസ്ഥ 'f':
          remove_existing_files = true;
          interactive = false;
          നിർത്തുക;
        അവസ്ഥ 'i':
          remove_existing_files = false;
          interactive = true;
          നിർത്തുക;
        അവസ്ഥ 'L':
          logical = true;
          നിർത്തുക;
        അവസ്ഥ 'n':
          dereference_dest_dir_symlinks = false;
          നിർത്തുക;
        അവസ്ഥ 'P':
          logical = false;
          നിർത്തുക;
        അവസ്ഥ 'r':
          relative = true;
          നിർത്തുക;
        അവസ്ഥ 's':
          symbolic_link = true;
          നിർത്തുക;
        അവസ്ഥ 't':
          എങ്കിൽ (target_directory)
            error (EXIT_FAILURE, 0, _("multiple target directories specified"));
          അല്ലെങ്കിൽ
            {
              ഘടന stat st;
              എങ്കിൽ (stat (optarg, &st) != 0)
                error (EXIT_FAILURE, errno, _("failed to access %s"),
                       quoteaf (optarg));
              എങ്കിൽ (! S_ISDIR (st.st_mode))
                error (EXIT_FAILURE, 0, _("target %s is not a directory"),
                       quoteaf (optarg));
            }
          target_directory = optarg;
          നിർത്തുക;
        അവസ്ഥ 'T':
          no_target_directory = true;
          നിർത്തുക;
        അവസ്ഥ 'v':
          verbose = true;
          നിർത്തുക;
        അവസ്ഥ 'S':
          make_backups = true;
          backup_suffix = optarg;
          നിർത്തുക;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        സ്ഥിരസ്ഥിതി:
          usage (EXIT_FAILURE);
          നിർത്തുക;
        }
    }

  n_files = argc - optind;
  file = argv + optind;

  എങ്കിൽ (n_files <= 0)
    {
      error (0, 0, _("missing file operand"));
      usage (EXIT_FAILURE);
    }

  എങ്കിൽ (relative && !symbolic_link)
    error (EXIT_FAILURE, 0, _("cannot do --relative without --symbolic"));

  എങ്കിൽ (!hard_dir_link)
    {
      priv_set_remove_linkdir ();
      beware_hard_dir_link = !cannot_unlink_dir ();
    }

  എങ്കിൽ (no_target_directory)
    {
      എങ്കിൽ (target_directory)
        error (EXIT_FAILURE, 0,
               _("cannot combine --target-directory "
                 "and --no-target-directory"));
      എങ്കിൽ (n_files != 2)
        {
          എങ്കിൽ (n_files < 2)
            error (0, 0,
                   _("missing destination file operand after %s"),
                   quoteaf (file[0]));
          അല്ലെങ്കിൽ
            error (0, 0, _("extra operand %s"), quoteaf (file[2]));
          usage (EXIT_FAILURE);
        }
    }
  അല്ലെങ്കിൽ എങ്കിൽ (n_files < 2 && !target_directory)
    {
      target_directory = ".";
      destdir_fd = AT_FDCWD;
    }
  അല്ലെങ്കിൽ
    {
      എങ്കിൽ (n_files == 2 && !target_directory)
        link_errno = atomic_link (file[0], AT_FDCWD, file[1]);
      എങ്കിൽ (link_errno < 0 || link_errno == EEXIST || link_errno == ENOTDIR
          || link_errno == EINVAL)
        {
          അക്ഷരം സ്ഥിരം *d
            = target_directory ? target_directory : file[n_files - 1];
          പൂർണ്ണസംഖ്യ flags = (O_PATHSEARCH | O_DIRECTORY
                       | (dereference_dest_dir_symlinks ? 0 : O_NOFOLLOW));
          destdir_fd = openat_safer (AT_FDCWD, d, flags);
          എങ്കിൽ (0 <= destdir_fd)
            {
              n_files -= !target_directory;
              target_directory = d;
            }
          അല്ലെങ്കിൽ എങ്കിൽ (! (n_files == 2 && !target_directory))
            error (EXIT_FAILURE, errno, _("target %s"), quoteaf (d));
        }
    }

  backup_type = (make_backups
                 ? xget_version (_("backup type"), version_control_string)
                 : no_backups);
  set_simple_backup_suffix (backup_suffix);


  എങ്കിൽ (target_directory)
    {
      /* Create the data structure we'll use to record which hard links we
         create.  Used to ensure that ln detects an obscure corner case that
         might result in user data loss.  Create it only if needed.  */
      എങ്കിൽ (2 <= n_files
          && remove_existing_files
          /* Don't bother trying to protect symlinks, since ln clobbering
             a just-created symlink won't ever lead to real data loss.  */
          && ! symbolic_link
          /* No destination hard link can be clobbered when making
             numbered backups.  */
          && backup_type != numbered_backups)
        {
          dest_set = hash_initialize (DEST_INFO_INITIAL_CAPACITY,
                                      ശൂന്യ,
                                      triple_hash,
                                      triple_compare,
                                      triple_free);
          എങ്കിൽ (dest_set == ശൂന്യ)
            xalloc_die ();
        }

      ok = true;
      വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < n_files; ++i)
        {
          അക്ഷരം *dest_base;
          അക്ഷരം *dest = file_name_concat (target_directory,
                                         last_component (file[i]),
                                         &dest_base);
          strip_trailing_slashes (dest_base);
          ok &= do_link (file[i], destdir_fd, dest_base, dest, -1);
          സ്വതന്ത്രം (dest);
        }
    }
  അല്ലെങ്കിൽ
    ok = do_link (file[0], AT_FDCWD, file[1], file[1], link_errno);

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
