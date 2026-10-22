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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <getopt.h>

#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "backupfile.h"
#ଅନ୍ତର୍ଭାଵ "fcntl-safer.h"
#ଅନ୍ତର୍ଭାଵ "filenamecat.h"
#ଅନ୍ତର୍ଭାଵ "file-set.h"
#ଅନ୍ତର୍ଭାଵ "force-link.h"
#ଅନ୍ତର୍ଭାଵ "hash.h"
#ଅନ୍ତର୍ଭାଵ "hashcode-file.h"
#ଅନ୍ତର୍ଭାଵ "priv-set.h"
#ଅନ୍ତର୍ଭାଵ "relpath.h"
#ଅନ୍ତର୍ଭାଵ "same.h"
#ଅନ୍ତର୍ଭାଵ "unlinkdir.h"
#ଅନ୍ତର୍ଭାଵ "yesno.h"
#ଅନ୍ତର୍ଭାଵ "canonicalize.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "ln"

#define AUTHORS \
  proper_name ("Mike Parker"), \
  proper_name ("David MacKenzie")

/* FIXME: document */
ସ୍ଥିର ଗଣନା backup_type backup_type;

/* If true, make symbolic links; otherwise, make hard links.  */
ସ୍ଥିର bool symbolic_link;

/* If true, make symbolic links relative  */
ସ୍ଥିର bool relative;

/* If true, hard links are logical rather than physical.  */
ସ୍ଥିର bool logical = !!LINK_FOLLOWS_SYMLINKS;

/* If true, ask the user before removing existing files.  */
ସ୍ଥିର bool interactive;

/* If true, remove existing files unconditionally.  */
ସ୍ଥିର bool remove_existing_files;

/* If true, list each file as it is moved. */
ସ୍ଥିର bool verbose;

/* If true, allow the superuser to *attempt* to make hard links
   to directories.  However, it appears that this option is not useful
   in practice, since even the superuser is prohibited from hard-linking
   directories on most existing systems (Solaris being an exception).  */
ସ୍ଥିର bool hard_dir_link;

/* If true, watch out for creating or removing hard links to directories.  */
ସ୍ଥିର bool beware_hard_dir_link;

/* If nonzero, and the specified destination is a symbolic link to a
   directory, treat it just as if it were a directory.  Otherwise, the
   command 'ln --force --no-dereference file symlink-to-dir' deletes
   symlink-to-dir before creating the new link.  */
ସ୍ଥିର bool dereference_dest_dir_symlinks = true;

/* This is a set of destination name/inode/dev triples for hard links
   created by ln.  Use this data structure to avoid data loss via a
   sequence of commands like this:
   rm -rf a b c; mkdir a b c; touch a/f b/f; ln -f a/f b/f c && rm -r a b */
ସ୍ଥିର Hash_table *dest_set;

/* Initial size of the dest_set hash table.  */
ଗଣନା { DEST_INFO_INITIAL_CAPACITY = 61 };

ସ୍ଥିର ସଂରଚନା option ଅଚଳ long_options[] =
{
  {"backup", optional_argument, ଖାଲି, 'b'},
  {"directory", no_argument, ଖାଲି, 'F'},
  {"no-dereference", no_argument, ଖାଲି, 'n'},
  {"no-target-directory", no_argument, ଖାଲି, 'T'},
  {"force", no_argument, ଖାଲି, 'f'},
  {"interactive", no_argument, ଖାଲି, 'i'},
  {"suffix", required_argument, ଖାଲି, 'S'},
  {"target-directory", required_argument, ଖାଲି, 't'},
  {"logical", no_argument, ଖାଲି, 'L'},
  {"physical", no_argument, ଖାଲି, 'P'},
  {"relative", no_argument, ଖାଲି, 'r'},
  {"symbolic", no_argument, ଖାଲି, 's'},
  {"verbose", no_argument, ଖାଲି, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ଖାଲି, 0, ଖାଲି, 0}
};

/* Return an errno value for a system call that returned STATUS.
   This is zero if STATUS is zero, and is errno otherwise.  */

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
errnoize (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଫେରନ୍ତୁ status < 0 ? errno : 0;
}

/* Return FROM represented as relative to the dir of TARGET.
   The result is malloced.  */

ସ୍ଥିର ଅକ୍ଷର *
convert_abs_rel (ଅକ୍ଷର ଅଚଳ *from, ଅକ୍ଷର ଅଚଳ *target)
{
  /* Get dirname to generate paths relative to.  We don't resolve
     the full TARGET as the last component could be an existing symlink.  */
  ଅକ୍ଷର *targetdir = dir_name (target);

  ଅକ୍ଷର *realdest = canonicalize_filename_mode (targetdir, CAN_MISSING);
  ଅକ୍ଷର *realfrom = canonicalize_filename_mode (from, CAN_MISSING);

  ଅକ୍ଷର *relative_from = ଖାଲି;
  ଯଦି (realdest && realfrom)
    {
      /* Write to a PATH_MAX buffer.  */
      relative_from = xmalloc (PATH_MAX);

      ଯଦି (!relpath (realfrom, realdest, relative_from, PATH_MAX))
        {
          ମୁକ୍ତ (relative_from);
          relative_from = ଖାଲି;
        }
    }

  ମୁକ୍ତ (targetdir);
  ମୁକ୍ତ (realdest);
  ମୁକ୍ତ (realfrom);

  ଫେରନ୍ତୁ relative_from ? relative_from : xstrdup (from);
}

/* Link SOURCE to DESTDIR_FD + DEST_BASE atomically.  DESTDIR_FD is
   the directory containing DEST_BASE.  Return 0 if successful, a
   positive errno value on failure, and -1 if an atomic link cannot be
   done.  This handles the common case where the destination does not
   already exist and -r is not specified.  */

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
atomic_link (ଅକ୍ଷର ଅଚଳ *source, ପୂର୍ଣ୍ଣସଂଖ୍ୟା destdir_fd, ଅକ୍ଷର ଅଚଳ *dest_base)
{
  ଫେରନ୍ତୁ (symbolic_link
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

ସ୍ଥିର bool
do_link (ଅକ୍ଷର ଅଚଳ *source, ପୂର୍ଣ୍ଣସଂଖ୍ୟା destdir_fd, ଅକ୍ଷର ଅଚଳ *dest_base,
         ଅକ୍ଷର ଅଚଳ *dest, ପୂର୍ଣ୍ଣସଂଖ୍ୟା link_errno)
{
  ସଂରଚନା stat source_stats;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା source_status = 1;
  ଅକ୍ଷର *backup_base = ଖାଲି;
  ଅକ୍ଷର *rel_source = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା nofollow_flag = logical ? 0 : AT_SYMLINK_NOFOLLOW;
  ଯଦି (link_errno < 0)
    link_errno = atomic_link (source, destdir_fd, dest_base);

  /* Get SOURCE_STATS if later code will need it, if only for sharper
     diagnostics.  */
  ଯଦି ((link_errno || dest_set) && !symbolic_link)
    {
      source_status = fstatat (AT_FDCWD, source, &source_stats, nofollow_flag);
      ଯଦି (source_status != 0)
        {
          error (0, errno, _("failed to access %s"), quoteaf (source));
          ଫେରନ୍ତୁ false;
        }
    }

  ଯଦି (link_errno)
    {
      ଯଦି (!symbolic_link && !hard_dir_link && S_ISDIR (source_stats.st_mode))
        {
          error (0, 0, _("%s: hard link not allowed for directory"),
                 quotef (source));
          ଫେରନ୍ତୁ false;
        }

      ଯଦି (relative)
        source = rel_source = convert_abs_rel (source, dest);

      bool force = (remove_existing_files || interactive
                    || backup_type != no_backups);
      ଯଦି (force)
        {
          ସଂରଚନା stat dest_stats;
          ଯଦି (fstatat (destdir_fd, dest_base, &dest_stats, AT_SYMLINK_NOFOLLOW)
              != 0)
            {
              ଯଦି (errno != ENOENT)
                {
                  error (0, errno, _("failed to access %s"), quoteaf (dest));
                  ଯାଅ fail;
                }
              force = false;
            }
          ଅନ୍ୟଥା ଯଦି (S_ISDIR (dest_stats.st_mode))
            {
              error (0, 0, _("%s: cannot overwrite directory"), quotef (dest));
              ଯାଅ fail;
            }
          ଅନ୍ୟଥା ଯଦି (seen_file (dest_set, dest, &dest_stats))
            {
              /* The current target was created as a hard link to another
                 source file.  */
              error (0, 0,
                     _("will not overwrite just-created %s with %s"),
                     quoteaf_n (0, dest), quoteaf_n (1, source));
              ଯାଅ fail;
            }
          ଅନ୍ୟଥା
            {
              /* Beware removing DEST if it is the same directory entry as
                 SOURCE, because in that case removing DEST can cause the
                 subsequent link creation either to fail (for hard links), or
                 to replace a non-symlink DEST with a self-loop (for symbolic
                 links) which loses the contents of DEST.  So, when backing
                 up, worry about creating hard links (since the backups cover
                 the symlink case); otherwise, worry about about -f.  */
              ଯଦି (backup_type != no_backups
                  ? !symbolic_link
                  : remove_existing_files)
                {
                  /* Detect whether removing DEST would also remove SOURCE.
                     If the file has only one link then both are surely the
                     same directory entry.  Otherwise check whether they point
                     to the same name in the same directory.  */
                  ଯଦି (source_status != 0)
                    source_status = stat (source, &source_stats);
                  ଯଦି (source_status == 0
                      && psame_inode (&source_stats, &dest_stats)
                      && (source_stats.st_nlink == 1
                          || same_nameat (AT_FDCWD, source,
                                          destdir_fd, dest_base)))
                    {
                      error (0, 0, _("%s and %s are the same file"),
                             quoteaf_n (0, source), quoteaf_n (1, dest));
                      ଯାଅ fail;
                    }
                }

              ଯଦି (link_errno < 0 || link_errno == EEXIST)
                {
                  ଯଦି (interactive)
                    {
                      fprintf (stderr, _("%s: replace %s? "),
                               program_name, quoteaf (dest));
                      ଯଦି (!yesno ())
                        {
                          ମୁକ୍ତ (rel_source);
                          ଫେରନ୍ତୁ false;
                        }
                    }

                  ଯଦି (backup_type != no_backups)
                    {
                      backup_base = find_backup_file_name (destdir_fd,
                                                           dest_base,
                                                           backup_type);
                      ଯଦି (renameat (destdir_fd, dest_base,
                                    destdir_fd, backup_base)
                          != 0)
                        {
                          ପୂର୍ଣ୍ଣସଂଖ୍ୟା rename_errno = errno;
                          ମୁକ୍ତ (backup_base);
                          backup_base = ଖାଲି;
                          ଯଦି (rename_errno != ENOENT)
                            {
                              error (0, rename_errno, _("cannot backup %s"),
                                     quoteaf (dest));
                              ଯାଅ fail;
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

  ଯଦି (link_errno <= 0)
    {
      /* Right after creating a hard link, do this: (note dest name and
         source_stats, which are also the just-linked-destinations stats) */
      ଯଦି (! symbolic_link)
        record_file (dest_set, dest, &source_stats);

      ଯଦି (verbose)
        {
          ଅକ୍ଷର ଅଚଳ *quoted_backup = "";
          ଅକ୍ଷର ଅଚଳ *backup_sep = "";
          ଯଦି (backup_base)
            {
              ଅକ୍ଷର *backup = backup_base;
              ଶୂନ୍ୟ *alloc = ଖାଲି;
              ptrdiff_t destdirlen = dest_base - dest;
              ଯଦି (0 < destdirlen)
                {
                  alloc = xmalloc (destdirlen + strlen (backup_base) + 1);
                  backup = memcpy (alloc, dest, destdirlen);
                  strcpy (backup + destdirlen, backup_base);
                }
              quoted_backup = quoteaf_n (2, backup);
              backup_sep = " ~ ";
              ମୁକ୍ତ (alloc);
            }
          printf ("%s%s%s %c> %s\n", quoted_backup, backup_sep,
                  quoteaf_n (0, dest), symbolic_link ? '-' : '=',
                  quoteaf_n (1, source));
        }
    }
  ଅନ୍ୟଥା
    {
      ଅକ୍ଷର *dest_quoted = quoteaf_n (0, dest);
      ଅକ୍ଷର *source_quoted = quoteaf_n (1, source);

      ଯଦି (symbolic_link)
        {
          ଯଦି (link_errno != ENAMETOOLONG && *source)
            error (0, link_errno, _("failed to create symbolic link %s"),
                   dest_quoted);
          ଅନ୍ୟଥା
            error (0, link_errno, _("failed to create symbolic link %s -> %s"),
                   dest_quoted, source_quoted);
        }
      ଅନ୍ୟଥା
        {
          ଯଦି (link_errno == EMLINK)
            error (0, link_errno, _("failed to create hard link to %s"),
                   source_quoted);
          ଅନ୍ୟଥା ଯଦି (link_errno == EDQUOT || link_errno == EEXIST
                   || link_errno == ENOSPC || link_errno == EROFS)
            error (0, link_errno, _("failed to create hard link %s"),
                   dest_quoted);
          ଅନ୍ୟଥା
            error (0, link_errno, _("failed to create hard link %s => %s"),
                   dest_quoted, source_quoted);
        }

      ଯଦି (backup_base)
        {
          ଯଦି (renameat (destdir_fd, backup_base, destdir_fd, dest_base) != 0)
            error (0, errno, _("cannot un-backup %s"), quoteaf (dest));
        }
    }

  ମୁକ୍ତ (backup_base);
  ମୁକ୍ତ (rel_source);
  ଫେରନ୍ତୁ link_errno <= 0;

fail:
  ମୁକ୍ତ (rel_source);
  ଫେରନ୍ତୁ false;
}

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
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
Create hard links by ପୂର୍ବନିର୍ଦ୍ଧାରିତ, symbolic links with --symbolic.\n\
By ପୂର୍ବନିର୍ଦ୍ଧାରିତ, each destination (name of new link) should not already exist.\n\
When creating hard links, each TARGET must exist.  Symbolic links\n\
can hold arbitrary text; ଯଦି later resolved, a relative link is\n\
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
         ଯଦି supported by the system\n\
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
         ଯଦି it is a symbolic link to a directory\n\
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

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା c;
  bool ok;
  bool make_backups = false;
  ଅକ୍ଷର ଅଚଳ *backup_suffix = ଖାଲି;
  ଅକ୍ଷର *version_control_string = ଖାଲି;
  ଅକ୍ଷର ଅଚଳ *target_directory = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା destdir_fd;
  bool no_target_directory = false;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_files;
  ଅକ୍ଷର **file;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା link_errno = -1;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdin);

  ଯେତେବେଳେ ((c = getopt_long (argc, argv, "bdfinrst:vFLPS:T",
                           long_options, ଖାଲି))
         != -1)
    {
      ବାଛ (c)
        {
        ସ୍ଥିତି 'b':
          make_backups = true;
          ଯଦି (optarg)
            version_control_string = optarg;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'd':
        ସ୍ଥିତି 'F':
          hard_dir_link = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'f':
          remove_existing_files = true;
          interactive = false;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'i':
          remove_existing_files = false;
          interactive = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'L':
          logical = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'n':
          dereference_dest_dir_symlinks = false;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'P':
          logical = false;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'r':
          relative = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 's':
          symbolic_link = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 't':
          ଯଦି (target_directory)
            error (EXIT_FAILURE, 0, _("multiple target directories specified"));
          ଅନ୍ୟଥା
            {
              ସଂରଚନା stat st;
              ଯଦି (stat (optarg, &st) != 0)
                error (EXIT_FAILURE, errno, _("failed to access %s"),
                       quoteaf (optarg));
              ଯଦି (! S_ISDIR (st.st_mode))
                error (EXIT_FAILURE, 0, _("target %s is not a directory"),
                       quoteaf (optarg));
            }
          target_directory = optarg;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'T':
          no_target_directory = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'v':
          verbose = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'S':
          make_backups = true;
          backup_suffix = optarg;
          ଭାଙ୍ଗ;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          usage (EXIT_FAILURE);
          ଭାଙ୍ଗ;
        }
    }

  n_files = argc - optind;
  file = argv + optind;

  ଯଦି (n_files <= 0)
    {
      error (0, 0, _("missing file operand"));
      usage (EXIT_FAILURE);
    }

  ଯଦି (relative && !symbolic_link)
    error (EXIT_FAILURE, 0, _("cannot do --relative without --symbolic"));

  ଯଦି (!hard_dir_link)
    {
      priv_set_remove_linkdir ();
      beware_hard_dir_link = !cannot_unlink_dir ();
    }

  ଯଦି (no_target_directory)
    {
      ଯଦି (target_directory)
        error (EXIT_FAILURE, 0,
               _("cannot combine --target-directory "
                 "and --no-target-directory"));
      ଯଦି (n_files != 2)
        {
          ଯଦି (n_files < 2)
            error (0, 0,
                   _("missing destination file operand after %s"),
                   quoteaf (file[0]));
          ଅନ୍ୟଥା
            error (0, 0, _("extra operand %s"), quoteaf (file[2]));
          usage (EXIT_FAILURE);
        }
    }
  ଅନ୍ୟଥା ଯଦି (n_files < 2 && !target_directory)
    {
      target_directory = ".";
      destdir_fd = AT_FDCWD;
    }
  ଅନ୍ୟଥା
    {
      ଯଦି (n_files == 2 && !target_directory)
        link_errno = atomic_link (file[0], AT_FDCWD, file[1]);
      ଯଦି (link_errno < 0 || link_errno == EEXIST || link_errno == ENOTDIR
          || link_errno == EINVAL)
        {
          ଅକ୍ଷର ଅଚଳ *d
            = target_directory ? target_directory : file[n_files - 1];
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags = (O_PATHSEARCH | O_DIRECTORY
                       | (dereference_dest_dir_symlinks ? 0 : O_NOFOLLOW));
          destdir_fd = openat_safer (AT_FDCWD, d, flags);
          ଯଦି (0 <= destdir_fd)
            {
              n_files -= !target_directory;
              target_directory = d;
            }
          ଅନ୍ୟଥା ଯଦି (! (n_files == 2 && !target_directory))
            error (EXIT_FAILURE, errno, _("target %s"), quoteaf (d));
        }
    }

  backup_type = (make_backups
                 ? xget_version (_("backup type"), version_control_string)
                 : no_backups);
  set_simple_backup_suffix (backup_suffix);


  ଯଦି (target_directory)
    {
      /* Create the data structure we'll use to record which hard links we
         create.  Used to ensure that ln detects an obscure corner case that
         might result in user data loss.  Create it only if needed.  */
      ଯଦି (2 <= n_files
          && remove_existing_files
          /* Don't bother trying to protect symlinks, since ln clobbering
             a just-created symlink won't ever lead to real data loss.  */
          && ! symbolic_link
          /* No destination hard link can be clobbered when making
             numbered backups.  */
          && backup_type != numbered_backups)
        {
          dest_set = hash_initialize (DEST_INFO_INITIAL_CAPACITY,
                                      ଖାଲି,
                                      triple_hash,
                                      triple_compare,
                                      triple_free);
          ଯଦି (dest_set == ଖାଲି)
            xalloc_die ();
        }

      ok = true;
      ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < n_files; ++i)
        {
          ଅକ୍ଷର *dest_base;
          ଅକ୍ଷର *dest = file_name_concat (target_directory,
                                         last_component (file[i]),
                                         &dest_base);
          strip_trailing_slashes (dest_base);
          ok &= do_link (file[i], destdir_fd, dest_base, dest, -1);
          ମୁକ୍ତ (dest);
        }
    }
  ଅନ୍ୟଥା
    ok = do_link (file[0], AT_FDCWD, file[1], file[1], link_errno);

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
