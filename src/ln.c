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

#ضمن <config.h>
#ضمن <stdio.h>
#ضمن <sys/types.h>
#ضمن <getopt.h>

#ضمن "system.h"
#ضمن "backupfile.h"
#ضمن "fcntl-safer.h"
#ضمن "filenamecat.h"
#ضمن "file-set.h"
#ضمن "force-link.h"
#ضمن "hash.h"
#ضمن "hashcode-file.h"
#ضمن "priv-set.h"
#ضمن "relpath.h"
#ضمن "same.h"
#ضمن "unlinkdir.h"
#ضمن "yesno.h"
#ضمن "canonicalize.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#وصف_ڪريو پروگرام_NAME "ln"

#وصف_ڪريو AUTHORS \
  proper_name ("Mike Parker"), \
  proper_name ("David MacKenzie")

/* FIXME: document */
ساكن تعداد backup_type backup_type;

/* If true, make symbolic links; otherwise, make hard links.  */
ساكن bool symbolic_link;

/* If true, make symbolic links relative  */
ساكن bool relative;

/* If true, hard links are logical rather than physical.  */
ساكن bool logical = !!LINK_FOLLOWS_SYMLINKS;

/* If true, ask the user before removing existing files.  */
ساكن bool interactive;

/* If true, remove existing files unconditionally.  */
ساكن bool remove_existing_files;

/* If true, list each file as it is moved. */
ساكن bool verbose;

/* If true, allow the superuser to *attempt* to make hard links
   to directories.  However, it appears that this option is not useful
   in practice, since even the superuser is prohibited from hard-linking
   directories on most existing systems (Solaris being an exception).  */
ساكن bool hard_dir_link;

/* If true, watch out for creating or removing hard links to directories.  */
ساكن bool beware_hard_dir_link;

/* If nonzero, and the specified destination is a symbolic link to a
   directory, treat it just as if it were a directory.  Otherwise, the
   command 'ln --force --no-dereference file symlink-to-dir' deletes
   symlink-to-dir before creating the new link.  */
ساكن bool dereference_dest_dir_symlinks = true;

/* This is a set of destination name/inode/dev triples for hard links
   created by ln.  Use this data structure to avoid data loss via a
   sequence of commands like this:
   rm -rf a b c; mkdir a b c; touch a/f b/f; ln -f a/f b/f c && rm -r a b */
ساكن Hash_جدول *dest_سيٽ;

/* Initial size of the dest_set hash table.  */
تعداد { DEST_INFO_INITIAL_صلاحيت = 61 };

ساكن بنية option ثابت long_options[] =
{
  {"backup", optional_دليل, عدم, 'b'},
  {"directory", no_دليل, عدم, 'F'},
  {"no-dereference", no_دليل, عدم, 'n'},
  {"no-target-directory", no_دليل, عدم, 'T'},
  {"force", no_دليل, عدم, 'f'},
  {"interactive", no_دليل, عدم, 'i'},
  {"suffix", required_دليل, عدم, 'S'},
  {"target-directory", required_دليل, عدم, 't'},
  {"logical", no_دليل, عدم, 'L'},
  {"physical", no_دليل, عدم, 'P'},
  {"relative", no_دليل, عدم, 'r'},
  {"symbolic", no_دليل, عدم, 's'},
  {"verbose", no_دليل, عدم, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخو_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

/* Return an errno value for a system call that returned STATUS.
   This is zero if STATUS is zero, and is errno otherwise.  */

ساكن صحيح
errnoize (صحيح حالت)
{
  أرجع حالت < 0 ? errno : 0;
}

/* Return FROM represented as relative to the dir of TARGET.
   The result is malloced.  */

ساكن حرف *
convert_abs_rel (حرف ثابت *from, حرف ثابت *هدف)
{
  /* Get dirname to generate paths relative to.  We don't resolve
     the full TARGET as the last component could be an existing symlink.  */
  حرف *targetdir = dir_name (هدف);

  حرف *realdest = canonicalize_filename_گهڻائي (targetdir, CAN_MISSING);
  حرف *realfrom = canonicalize_filename_گهڻائي (from, CAN_MISSING);

  حرف *relative_from = عدم;
  إذا (realdest && realfrom)
    {
      /* Write to a PATH_MAX buffer.  */
      relative_from = xmalloc (PATH_MAX);

      إذا (!relpath (realfrom, realdest, relative_from, PATH_MAX))
        {
          حرر (relative_from);
          relative_from = عدم;
        }
    }

  حرر (targetdir);
  حرر (realdest);
  حرر (realfrom);

  أرجع relative_from ? relative_from : xstrdup (from);
}

/* Link SOURCE to DESTDIR_FD + DEST_BASE atomically.  DESTDIR_FD is
   the directory containing DEST_BASE.  Return 0 if successful, a
   positive errno value on failure, and -1 if an atomic link cannot be
   done.  This handles the common case where the destination does not
   already exist and -r is not specified.  */

ساكن صحيح
atomic_link (حرف ثابت *ذريعو_2, صحيح destdir_fd, حرف ثابت *dest_بنياد)
{
  أرجع (symbolic_link
          ? (relative ? -1
             : errnoize (symlinkat (ذريعو_2, destdir_fd, dest_بنياد)))
          : beware_hard_dir_link ? -1
          : errnoize (linkat (AT_FDCWD, ذريعو_2, destdir_fd, dest_بنياد,
                              logical ? AT_SYMLINK_FOLLOW : 0)));
}

/* Link SOURCE to a directory entry under DESTDIR_FD named DEST_BASE.
   DEST is the full name of the destination, useful for diagnostics.
   LINK_ERRNO is zero if the link has already been made,
   positive if attempting the link failed with errno == LINK_ERRNO,
   -1 if no attempt has been made to create the link.
   Return true if successful.  */

ساكن bool
do_link (حرف ثابت *ذريعو_2, صحيح destdir_fd, حرف ثابت *dest_بنياد,
         حرف ثابت *dest, صحيح link_errno)
{
  بنية stat ذريعو_stats;
  صحيح ذريعو_حالت = 1;
  حرف *backup_بنياد = عدم;
  حرف *rel_ذريعو = عدم;
  صحيح nofollow_flag = logical ? 0 : AT_SYMLINK_NOFOLLOW;
  إذا (link_errno < 0)
    link_errno = atomic_link (ذريعو_2, destdir_fd, dest_بنياد);

  /* Get SOURCE_STATS if later code will need it, if only for sharper
     diagnostics.  */
  إذا ((link_errno || dest_سيٽ) && !symbolic_link)
    {
      ذريعو_حالت = fstatat (AT_FDCWD, ذريعو_2, &ذريعو_stats, nofollow_flag);
      إذا (ذريعو_حالت != 0)
        {
          error (0, errno, _("failed to access %s"), quoteaf (ذريعو_2));
          أرجع false;
        }
    }

  إذا (link_errno)
    {
      إذا (!symbolic_link && !hard_dir_link && S_ISDIR (ذريعو_stats.st_گهڻائي))
        {
          error (0, 0, _("%s: hard link not allowed for directory"),
                 quotef (ذريعو_2));
          أرجع false;
        }

      إذا (relative)
        ذريعو_2 = rel_ذريعو = convert_abs_rel (ذريعو_2, dest);

      bool force = (remove_existing_files || interactive
                    || backup_type != no_backups);
      إذا (force)
        {
          بنية stat dest_stats;
          إذا (fstatat (destdir_fd, dest_بنياد, &dest_stats, AT_SYMLINK_NOFOLLOW)
              != 0)
            {
              إذا (errno != ENOENT)
                {
                  error (0, errno, _("failed to access %s"), quoteaf (dest));
                  اذهب fail;
                }
              force = false;
            }
          وإلا إذا (S_ISDIR (dest_stats.st_گهڻائي))
            {
              error (0, 0, _("%s: cannot overwrite directory"), quotef (dest));
              اذهب fail;
            }
          وإلا إذا (ڏٺل_file (dest_سيٽ, dest, &dest_stats))
            {
              /* The current target was created as a hard link to another
                 source file.  */
              error (0, 0,
                     _("will not overwrite just-created %s with %s"),
                     quoteaf_n (0, dest), quoteaf_n (1, ذريعو_2));
              اذهب fail;
            }
          وإلا
            {
              /* Beware removing DEST if it is the same directory entry as
                 SOURCE, because in that case removing DEST can cause the
                 subsequent link creation either to fail (for hard links), or
                 to replace a non-symlink DEST with a self-loop (for symbolic
                 links) which loses the contents of DEST.  So, when backing
                 up, worry about creating hard links (since the backups cover
                 the symlink case); otherwise, worry about about -f.  */
              إذا (backup_type != no_backups
                  ? !symbolic_link
                  : remove_existing_files)
                {
                  /* Detect whether removing DEST would also remove SOURCE.
                     If the file has only one link then both are surely the
                     same directory entry.  Otherwise check whether they point
                     to the same name in the same directory.  */
                  إذا (ذريعو_حالت != 0)
                    ذريعو_حالت = stat (ذريعو_2, &ذريعو_stats);
                  إذا (ذريعو_حالت == 0
                      && psame_inode (&ذريعو_stats, &dest_stats)
                      && (ذريعو_stats.st_nlink == 1
                          || same_nameat (AT_FDCWD, ذريعو_2,
                                          destdir_fd, dest_بنياد)))
                    {
                      error (0, 0, _("%s and %s are the same file"),
                             quoteaf_n (0, ذريعو_2), quoteaf_n (1, dest));
                      اذهب fail;
                    }
                }

              إذا (link_errno < 0 || link_errno == EEXIST)
                {
                  إذا (interactive)
                    {
                      fprintf (stderr, _("%s: replace %s? "),
                               پروگرام_name, quoteaf (dest));
                      إذا (!yesno ())
                        {
                          حرر (rel_ذريعو);
                          أرجع false;
                        }
                    }

                  إذا (backup_type != no_backups)
                    {
                      backup_بنياد = ڳوليو_backup_file_name (destdir_fd,
                                                           dest_بنياد,
                                                           backup_type);
                      إذا (renameat (destdir_fd, dest_بنياد,
                                    destdir_fd, backup_بنياد)
                          != 0)
                        {
                          صحيح rename_errno = errno;
                          حرر (backup_بنياد);
                          backup_بنياد = عدم;
                          إذا (rename_errno != ENOENT)
                            {
                              error (0, rename_errno, _("cannot backup %s"),
                                     quoteaf (dest));
                              اذهب fail;
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
           ? force_symlinkat (ذريعو_2, destdir_fd, dest_بنياد,
                              force, link_errno)
           : force_linkat (AT_FDCWD, ذريعو_2, destdir_fd, dest_بنياد,
                           logical ? AT_SYMLINK_FOLLOW : 0,
                           force, link_errno));
      /* Until now, link_errno < 0 meant the link has not been tried.
         From here on, link_errno < 0 means the link worked but
         required removing the destination first.  */
    }

  إذا (link_errno <= 0)
    {
      /* Right after creating a hard link, do this: (note dest name and
         source_stats, which are also the just-linked-destinations stats) */
      إذا (! symbolic_link)
        رڪارڊ_file (dest_سيٽ, dest, &ذريعو_stats);

      إذا (verbose)
        {
          حرف ثابت *quoted_backup = "";
          حرف ثابت *backup_sep = "";
          إذا (backup_بنياد)
            {
              حرف *backup = backup_بنياد;
              فراغ *alloc = عدم;
              ptrdiff_t destdirlen = dest_بنياد - dest;
              إذا (0 < destdirlen)
                {
                  alloc = xmalloc (destdirlen + لڙي_ڊيگهه (backup_بنياد) + 1);
                  backup = يادگيري_نقل (alloc, dest, destdirlen);
                  strcpy (backup + destdirlen, backup_بنياد);
                }
              quoted_backup = quoteaf_n (2, backup);
              backup_sep = " ~ ";
              حرر (alloc);
            }
          لکو ("%s%s%s %c> %s\n", quoted_backup, backup_sep,
                  quoteaf_n (0, dest), symbolic_link ? '-' : '=',
                  quoteaf_n (1, ذريعو_2));
        }
    }
  وإلا
    {
      حرف *dest_quoted = quoteaf_n (0, dest);
      حرف *ذريعو_quoted = quoteaf_n (1, ذريعو_2);

      إذا (symbolic_link)
        {
          إذا (link_errno != ENAMETOOLONG && *ذريعو_2)
            error (0, link_errno, _("failed to create symbolic link %s"),
                   dest_quoted);
          وإلا
            error (0, link_errno, _("failed to create symbolic link %s -> %s"),
                   dest_quoted, ذريعو_quoted);
        }
      وإلا
        {
          إذا (link_errno == EMLINK)
            error (0, link_errno, _("failed to create hard link to %s"),
                   ذريعو_quoted);
          وإلا إذا (link_errno == EDQUOT || link_errno == EEXIST
                   || link_errno == ENOSPC || link_errno == EROFS)
            error (0, link_errno, _("failed to create hard link %s"),
                   dest_quoted);
          وإلا
            error (0, link_errno, _("failed to create hard link %s => %s"),
                   dest_quoted, ذريعو_quoted);
        }

      إذا (backup_بنياد)
        {
          إذا (renameat (destdir_fd, backup_بنياد, destdir_fd, dest_بنياد) != 0)
            error (0, errno, _("cannot un-backup %s"), quoteaf (dest));
        }
    }

  حرر (backup_بنياد);
  حرر (rel_ذريعو);
  أرجع link_errno <= 0;

fail:
  حرر (rel_ذريعو);
  أرجع false;
}

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکو (_("\
Usage: %s [OPTION]... [-T] هدف_2 LINK_NAME\n\
  or:  %s [OPTION]... هدف_2\n\
  or:  %s [OPTION]... هدف_2... DIRECTORY\n\
  or:  %s [OPTION]... -t DIRECTORY هدف_2...\n\
"),
              پروگرام_name, پروگرام_name, پروگرام_name, پروگرام_name);
      fputs (_("\
اندريون_2 the 1st form, create a link منزل هدف_2 with the name LINK_NAME.\n\
اندريون_2 the 2nd form, create a link منزل هدف_2 اندريون the current directory.\n\
اندريون_2 the 3rd and 4th forms, create links منزل each هدف_2 اندريون DIRECTORY.\n\
Create hard links by افتراضي, symbolic links with --symbolic.\n\
By افتراضي, each destination (name جو new link) should not already exist.\n\
When creating hard links, each هدف_2 must exist.  Symbolic links\n\
can hold arbitrary متن; إذا later resolved, a relative link is\n\
interpreted اندريون لاڳاپو منزل its والد directory.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
      --backup[=CONTROL]\n\
         make a backup جو each existing destination file\n\
"));
      oputs (_("\
  -b\n\
         like --backup but does not accept an دليل\n\
"));
      oputs (_("\
  -d, -F, --directory\n\
         allow the superuser منزل attempt منزل hard link directories,\n\
         إذا supported by the system\n\
"));
      oputs (_("\
  -f, --force\n\
         remove existing destination files\n\
"));
      oputs (_("\
  -i, --interactive\n\
         prompt whether منزل remove destinations\n\
"));
      oputs (_("\
  -L, --logical\n\
         dereference TARGETs that are symbolic links\n\
"));
      oputs (_("\
  -n, --no-dereference\n\
         treat LINK_NAME as a عام_2 file\n\
         إذا it is a symbolic link منزل a directory\n\
"));
      oputs (_("\
  -P, --physical\n\
         make hard links directly منزل symbolic links\n\
"));
      oputs (_("\
  -r, --relative\n\
         with -s, create links relative منزل link location\n\
"));
      oputs (_("\
  -s, --symbolic\n\
         make symbolic links instead جو hard links\n\
"));
      oputs (_("\
  -S, --suffix=SUFFIX\n\
         override the usual backup suffix\n\
"));
      oputs (_("\
  -t, --هدف-directory=DIRECTORY\n\
         specify the DIRECTORY اندريون which منزل create the links\n\
"));
      oputs (_("\
  -T, --no-هدف-directory\n\
         treat LINK_NAME as a عام_2 file always\n\
"));
      oputs (_("\
  -v, --verbose\n\
         print name جو each linked file\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخو_OPTION_DESCRIPTION);
      emit_backup_suffix_note ();
      لکو (_("\
\n\
Using -s ignores -L and -P.  Otherwise, the آخري option specified controls\n\
behavior when a هدف_2 is a symbolic link, defaulting منزل %s.\n\
"), LINK_FOLLOWS_SYMLINKS ? "-L" : "-P");
      emit_ancillary_info (پروگرام_NAME);
    }
  ٻاهر_نڪرو (حالت);
}

صحيح
مکيه (صحيح argc, حرف **argv)
{
  صحيح c;
  bool ok;
  bool make_backups = false;
  حرف ثابت *backup_suffix = عدم;
  حرف *نسخو_control_string = عدم;
  حرف ثابت *هدف_directory = عدم;
  صحيح destdir_fd;
  bool no_هدف_directory = false;
  صحيح n_files;
  حرف **file;
  صحيح link_errno = -1;

  initialize_main (&argc, &argv);
  سيٽ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdin);

  طالما ((c = getopt_long (argc, argv, "bdfinrst:vFLPS:T",
                           long_options, عدم))
         != -1)
    {
      بدل (c)
        {
        حالة 'b':
          make_backups = true;
          إذا (optarg)
            نسخو_control_string = optarg;
          اكسر;
        حالة 'd':
        حالة 'F':
          hard_dir_link = true;
          اكسر;
        حالة 'f':
          remove_existing_files = true;
          interactive = false;
          اكسر;
        حالة 'i':
          remove_existing_files = false;
          interactive = true;
          اكسر;
        حالة 'L':
          logical = true;
          اكسر;
        حالة 'n':
          dereference_dest_dir_symlinks = false;
          اكسر;
        حالة 'P':
          logical = false;
          اكسر;
        حالة 'r':
          relative = true;
          اكسر;
        حالة 's':
          symbolic_link = true;
          اكسر;
        حالة 't':
          إذا (هدف_directory)
            error (EXIT_FAILURE, 0, _("multiple target directories specified"));
          وإلا
            {
              بنية stat st;
              إذا (stat (optarg, &st) != 0)
                error (EXIT_FAILURE, errno, _("failed to access %s"),
                       quoteaf (optarg));
              إذا (! S_ISDIR (st.st_گهڻائي))
                error (EXIT_FAILURE, 0, _("target %s is not a directory"),
                       quoteaf (optarg));
            }
          هدف_directory = optarg;
          اكسر;
        حالة 'T':
          no_هدف_directory = true;
          اكسر;
        حالة 'v':
          verbose = true;
          اكسر;
        حالة 'S':
          make_backups = true;
          backup_suffix = optarg;
          اكسر;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_نسخو_CHAR (پروگرام_NAME, AUTHORS);
        افتراضي:
          usage (EXIT_FAILURE);
          اكسر;
        }
    }

  n_files = argc - optind;
  file = argv + optind;

  إذا (n_files <= 0)
    {
      error (0, 0, _("missing file operand"));
      usage (EXIT_FAILURE);
    }

  إذا (relative && !symbolic_link)
    error (EXIT_FAILURE, 0, _("cannot do --relative without --symbolic"));

  إذا (!hard_dir_link)
    {
      priv_سيٽ_remove_linkdir ();
      beware_hard_dir_link = !cannot_unlink_dir ();
    }

  إذا (no_هدف_directory)
    {
      إذا (هدف_directory)
        error (EXIT_FAILURE, 0,
               _("cannot combine --target-directory "
                 "and --no-target-directory"));
      إذا (n_files != 2)
        {
          إذا (n_files < 2)
            error (0, 0,
                   _("missing destination file operand after %s"),
                   quoteaf (file[0]));
          وإلا
            error (0, 0, _("extra operand %s"), quoteaf (file[2]));
          usage (EXIT_FAILURE);
        }
    }
  وإلا إذا (n_files < 2 && !هدف_directory)
    {
      هدف_directory = ".";
      destdir_fd = AT_FDCWD;
    }
  وإلا
    {
      إذا (n_files == 2 && !هدف_directory)
        link_errno = atomic_link (file[0], AT_FDCWD, file[1]);
      إذا (link_errno < 0 || link_errno == EEXIST || link_errno == ENOTDIR
          || link_errno == EINVAL)
        {
          حرف ثابت *d
            = هدف_directory ? هدف_directory : file[n_files - 1];
          صحيح flags = (O_PATHSEARCH | O_DIRECTORY
                       | (dereference_dest_dir_symlinks ? 0 : O_NOFOLLOW));
          destdir_fd = openat_safer (AT_FDCWD, d, flags);
          إذا (0 <= destdir_fd)
            {
              n_files -= !هدف_directory;
              هدف_directory = d;
            }
          وإلا إذا (! (n_files == 2 && !هدف_directory))
            error (EXIT_FAILURE, errno, _("target %s"), quoteaf (d));
        }
    }

  backup_type = (make_backups
                 ? xget_نسخو (_("backup type"), نسخو_control_string)
                 : no_backups);
  سيٽ_simple_backup_suffix (backup_suffix);


  إذا (هدف_directory)
    {
      /* Create the data structure we'll use to record which hard links we
         create.  Used to ensure that ln detects an obscure corner case that
         might result in user data loss.  Create it only if needed.  */
      إذا (2 <= n_files
          && remove_existing_files
          /* Don't bother trying to protect symlinks, since ln clobbering
             a just-created symlink won't ever lead to real data loss.  */
          && ! symbolic_link
          /* No destination hard link can be clobbered when making
             numbered backups.  */
          && backup_type != numbered_backups)
        {
          dest_سيٽ = hash_initialize (DEST_INFO_INITIAL_صلاحيت,
                                      عدم,
                                      triple_hash,
                                      triple_compare,
                                      triple_free);
          إذا (dest_سيٽ == عدم)
            xalloc_die ();
        }

      ok = true;
      لكل (صحيح i = 0; i < n_files; ++i)
        {
          حرف *dest_بنياد;
          حرف *dest = file_name_concat (هدف_directory,
                                         آخري_component (file[i]),
                                         &dest_بنياد);
          strip_trailing_slashes (dest_بنياد);
          ok &= do_link (file[i], destdir_fd, dest_بنياد, dest, -1);
          حرر (dest);
        }
    }
  وإلا
    ok = do_link (file[0], AT_FDCWD, file[1], file[1], link_errno);

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
