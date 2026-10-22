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

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <getopt.h>

#అన్తర్భావ "system.h"
#అన్తర్భావ "backupfile.h"
#అన్తర్భావ "fcntl-safer.h"
#అన్తర్భావ "filenamecat.h"
#అన్తర్భావ "file-set.h"
#అన్తర్భావ "force-link.h"
#అన్తర్భావ "hash.h"
#అన్తర్భావ "hashcode-file.h"
#అన్తర్భావ "priv-set.h"
#అన్తర్భావ "relpath.h"
#అన్తర్భావ "same.h"
#అన్తర్భావ "unlinkdir.h"
#అన్తర్భావ "yesno.h"
#అన్తర్భావ "canonicalize.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "ln"

#నిర్వచించు AUTHORS \
  proper_name ("Mike Parker"), \
  proper_name ("David MacKenzie")

/* FIXME: document */
స్థిర గణన backup_type backup_type;

/* If true, make symbolic links; otherwise, make hard links.  */
స్థిర bool symbolic_link;

/* If true, make symbolic links relative  */
స్థిర bool relative;

/* If true, hard links are logical rather than physical.  */
స్థిర bool logical = !!LINK_FOLLOWS_SYMLINKS;

/* If true, ask the user before removing existing files.  */
స్థిర bool interactive;

/* If true, remove existing files unconditionally.  */
స్థిర bool remove_existing_files;

/* If true, list each file as it is moved. */
స్థిర bool verbose;

/* If true, allow the superuser to *attempt* to make hard links
   to directories.  However, it appears that this option is not useful
   in practice, since even the superuser is prohibited from hard-linking
   directories on most existing systems (Solaris being an exception).  */
స్థిర bool hard_dir_link;

/* If true, watch out for creating or removing hard links to directories.  */
స్థిర bool beware_hard_dir_link;

/* If nonzero, and the specified destination is a symbolic link to a
   directory, treat it just as if it were a directory.  Otherwise, the
   command 'ln --force --no-dereference file symlink-to-dir' deletes
   symlink-to-dir before creating the new link.  */
స్థిర bool dereference_dest_dir_symlinks = true;

/* This is a set of destination name/inode/dev triples for hard links
   created by ln.  Use this data structure to avoid data loss via a
   sequence of commands like this:
   rm -rf a b c; mkdir a b c; touch a/f b/f; ln -f a/f b/f c && rm -r a b */
స్థిర Hash_పట్టిక *dest_సమితి;

/* Initial size of the dest_set hash table.  */
గణన { DEST_INFO_INITIAL_సామర్థ్యం = 61 };

స్థిర నిర్మాణం option స్థిరరాశి long_options[] =
{
  {"backup", optional_వాదం, ఖాళీ, 'b'},
  {"directory", no_వాదం, ఖాళీ, 'F'},
  {"no-dereference", no_వాదం, ఖాళీ, 'n'},
  {"no-target-directory", no_వాదం, ఖాళీ, 'T'},
  {"force", no_వాదం, ఖాళీ, 'f'},
  {"interactive", no_వాదం, ఖాళీ, 'i'},
  {"suffix", required_వాదం, ఖాళీ, 'S'},
  {"target-directory", required_వాదం, ఖాళీ, 't'},
  {"logical", no_వాదం, ఖాళీ, 'L'},
  {"physical", no_వాదం, ఖాళీ, 'P'},
  {"relative", no_వాదం, ఖాళీ, 'r'},
  {"symbolic", no_వాదం, ఖాళీ, 's'},
  {"verbose", no_వాదం, ఖాళీ, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0}
};

/* Return an errno value for a system call that returned STATUS.
   This is zero if STATUS is zero, and is errno otherwise.  */

స్థిర పూర్ణాంకం
errnoize (పూర్ణాంకం స్థితి_2)
{
  తిరిగి స్థితి_2 < 0 ? errno : 0;
}

/* Return FROM represented as relative to the dir of TARGET.
   The result is malloced.  */

స్థిర అక్షరం *
convert_abs_rel (అక్షరం స్థిరరాశి *from, అక్షరం స్థిరరాశి *లక్ష్యం)
{
  /* Get dirname to generate paths relative to.  We don't resolve
     the full TARGET as the last component could be an existing symlink.  */
  అక్షరం *targetdir = dir_name (లక్ష్యం);

  అక్షరం *realdest = canonicalize_filename_బహుళకం (targetdir, CAN_MISSING);
  అక్షరం *realfrom = canonicalize_filename_బహుళకం (from, CAN_MISSING);

  అక్షరం *relative_from = ఖాళీ;
  ఒకవేళ (realdest && realfrom)
    {
      /* Write to a PATH_MAX buffer.  */
      relative_from = xmalloc (PATH_MAX);

      ఒకవేళ (!relpath (realfrom, realdest, relative_from, PATH_MAX))
        {
          విడుదల (relative_from);
          relative_from = ఖాళీ;
        }
    }

  విడుదల (targetdir);
  విడుదల (realdest);
  విడుదల (realfrom);

  తిరిగి relative_from ? relative_from : xstrdup (from);
}

/* Link SOURCE to DESTDIR_FD + DEST_BASE atomically.  DESTDIR_FD is
   the directory containing DEST_BASE.  Return 0 if successful, a
   positive errno value on failure, and -1 if an atomic link cannot be
   done.  This handles the common case where the destination does not
   already exist and -r is not specified.  */

స్థిర పూర్ణాంకం
atomic_link (అక్షరం స్థిరరాశి *ఆధారం_2, పూర్ణాంకం destdir_fd, అక్షరం స్థిరరాశి *dest_పునాది)
{
  తిరిగి (symbolic_link
          ? (relative ? -1
             : errnoize (symlinkat (ఆధారం_2, destdir_fd, dest_పునాది)))
          : beware_hard_dir_link ? -1
          : errnoize (linkat (AT_FDCWD, ఆధారం_2, destdir_fd, dest_పునాది,
                              logical ? AT_SYMLINK_FOLLOW : 0)));
}

/* Link SOURCE to a directory entry under DESTDIR_FD named DEST_BASE.
   DEST is the full name of the destination, useful for diagnostics.
   LINK_ERRNO is zero if the link has already been made,
   positive if attempting the link failed with errno == LINK_ERRNO,
   -1 if no attempt has been made to create the link.
   Return true if successful.  */

స్థిర bool
do_link (అక్షరం స్థిరరాశి *ఆధారం_2, పూర్ణాంకం destdir_fd, అక్షరం స్థిరరాశి *dest_పునాది,
         అక్షరం స్థిరరాశి *dest, పూర్ణాంకం link_errno)
{
  నిర్మాణం stat ఆధారం_stats;
  పూర్ణాంకం ఆధారం_స్థితి = 1;
  అక్షరం *backup_పునాది = ఖాళీ;
  అక్షరం *rel_ఆధారం = ఖాళీ;
  పూర్ణాంకం nofollow_flag = logical ? 0 : AT_SYMLINK_NOFOLLOW;
  ఒకవేళ (link_errno < 0)
    link_errno = atomic_link (ఆధారం_2, destdir_fd, dest_పునాది);

  /* Get SOURCE_STATS if later code will need it, if only for sharper
     diagnostics.  */
  ఒకవేళ ((link_errno || dest_సమితి) && !symbolic_link)
    {
      ఆధారం_స్థితి = fstatat (AT_FDCWD, ఆధారం_2, &ఆధారం_stats, nofollow_flag);
      ఒకవేళ (ఆధారం_స్థితి != 0)
        {
          error (0, errno, _("failed to access %s"), quoteaf (ఆధారం_2));
          తిరిగి false;
        }
    }

  ఒకవేళ (link_errno)
    {
      ఒకవేళ (!symbolic_link && !hard_dir_link && S_ISDIR (ఆధారం_stats.st_బహుళకం))
        {
          error (0, 0, _("%s: hard link not allowed for directory"),
                 quotef (ఆధారం_2));
          తిరిగి false;
        }

      ఒకవేళ (relative)
        ఆధారం_2 = rel_ఆధారం = convert_abs_rel (ఆధారం_2, dest);

      bool force = (remove_existing_files || interactive
                    || backup_type != no_backups);
      ఒకవేళ (force)
        {
          నిర్మాణం stat dest_stats;
          ఒకవేళ (fstatat (destdir_fd, dest_పునాది, &dest_stats, AT_SYMLINK_NOFOLLOW)
              != 0)
            {
              ఒకవేళ (errno != ENOENT)
                {
                  error (0, errno, _("failed to access %s"), quoteaf (dest));
                  వెళ్ళు fail;
                }
              force = false;
            }
          లేకపోతే ఒకవేళ (S_ISDIR (dest_stats.st_బహుళకం))
            {
              error (0, 0, _("%s: cannot overwrite directory"), quotef (dest));
              వెళ్ళు fail;
            }
          లేకపోతే ఒకవేళ (చూసిన_file (dest_సమితి, dest, &dest_stats))
            {
              /* The current target was created as a hard link to another
                 source file.  */
              error (0, 0,
                     _("will not overwrite just-created %s with %s"),
                     quoteaf_n (0, dest), quoteaf_n (1, ఆధారం_2));
              వెళ్ళు fail;
            }
          లేకపోతే
            {
              /* Beware removing DEST if it is the same directory entry as
                 SOURCE, because in that case removing DEST can cause the
                 subsequent link creation either to fail (for hard links), or
                 to replace a non-symlink DEST with a self-loop (for symbolic
                 links) which loses the contents of DEST.  So, when backing
                 up, worry about creating hard links (since the backups cover
                 the symlink case); otherwise, worry about about -f.  */
              ఒకవేళ (backup_type != no_backups
                  ? !symbolic_link
                  : remove_existing_files)
                {
                  /* Detect whether removing DEST would also remove SOURCE.
                     If the file has only one link then both are surely the
                     same directory entry.  Otherwise check whether they point
                     to the same name in the same directory.  */
                  ఒకవేళ (ఆధారం_స్థితి != 0)
                    ఆధారం_స్థితి = stat (ఆధారం_2, &ఆధారం_stats);
                  ఒకవేళ (ఆధారం_స్థితి == 0
                      && psame_inode (&ఆధారం_stats, &dest_stats)
                      && (ఆధారం_stats.st_nlink == 1
                          || same_nameat (AT_FDCWD, ఆధారం_2,
                                          destdir_fd, dest_పునాది)))
                    {
                      error (0, 0, _("%s and %s are the same file"),
                             quoteaf_n (0, ఆధారం_2), quoteaf_n (1, dest));
                      వెళ్ళు fail;
                    }
                }

              ఒకవేళ (link_errno < 0 || link_errno == EEXIST)
                {
                  ఒకవేళ (interactive)
                    {
                      fprintf (stderr, _("%s: replace %s? "),
                               కార్యక్రమం_name, quoteaf (dest));
                      ఒకవేళ (!yesno ())
                        {
                          విడుదల (rel_ఆధారం);
                          తిరిగి false;
                        }
                    }

                  ఒకవేళ (backup_type != no_backups)
                    {
                      backup_పునాది = కనుగొను_backup_file_name (destdir_fd,
                                                           dest_పునాది,
                                                           backup_type);
                      ఒకవేళ (renameat (destdir_fd, dest_పునాది,
                                    destdir_fd, backup_పునాది)
                          != 0)
                        {
                          పూర్ణాంకం rename_errno = errno;
                          విడుదల (backup_పునాది);
                          backup_పునాది = ఖాళీ;
                          ఒకవేళ (rename_errno != ENOENT)
                            {
                              error (0, rename_errno, _("cannot backup %s"),
                                     quoteaf (dest));
                              వెళ్ళు fail;
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
           ? force_symlinkat (ఆధారం_2, destdir_fd, dest_పునాది,
                              force, link_errno)
           : force_linkat (AT_FDCWD, ఆధారం_2, destdir_fd, dest_పునాది,
                           logical ? AT_SYMLINK_FOLLOW : 0,
                           force, link_errno));
      /* Until now, link_errno < 0 meant the link has not been tried.
         From here on, link_errno < 0 means the link worked but
         required removing the destination first.  */
    }

  ఒకవేళ (link_errno <= 0)
    {
      /* Right after creating a hard link, do this: (note dest name and
         source_stats, which are also the just-linked-destinations stats) */
      ఒకవేళ (! symbolic_link)
        రికార్డు_file (dest_సమితి, dest, &ఆధారం_stats);

      ఒకవేళ (verbose)
        {
          అక్షరం స్థిరరాశి *quoted_backup = "";
          అక్షరం స్థిరరాశి *backup_sep = "";
          ఒకవేళ (backup_పునాది)
            {
              అక్షరం *backup = backup_పునాది;
              శూన్య *alloc = ఖాళీ;
              ptrdiff_t destdirlen = dest_పునాది - dest;
              ఒకవేళ (0 < destdirlen)
                {
                  alloc = xmalloc (destdirlen + అక్షరశ్రేణి_పొడవు (backup_పునాది) + 1);
                  backup = జ్ఞాపకం_నకలు (alloc, dest, destdirlen);
                  strcpy (backup + destdirlen, backup_పునాది);
                }
              quoted_backup = quoteaf_n (2, backup);
              backup_sep = " ~ ";
              విడుదల (alloc);
            }
          రాయి ("%s%s%s %c> %s\n", quoted_backup, backup_sep,
                  quoteaf_n (0, dest), symbolic_link ? '-' : '=',
                  quoteaf_n (1, ఆధారం_2));
        }
    }
  లేకపోతే
    {
      అక్షరం *dest_quoted = quoteaf_n (0, dest);
      అక్షరం *ఆధారం_quoted = quoteaf_n (1, ఆధారం_2);

      ఒకవేళ (symbolic_link)
        {
          ఒకవేళ (link_errno != ENAMETOOLONG && *ఆధారం_2)
            error (0, link_errno, _("failed to create symbolic link %s"),
                   dest_quoted);
          లేకపోతే
            error (0, link_errno, _("failed to create symbolic link %s -> %s"),
                   dest_quoted, ఆధారం_quoted);
        }
      లేకపోతే
        {
          ఒకవేళ (link_errno == EMLINK)
            error (0, link_errno, _("failed to create hard link to %s"),
                   ఆధారం_quoted);
          లేకపోతే ఒకవేళ (link_errno == EDQUOT || link_errno == EEXIST
                   || link_errno == ENOSPC || link_errno == EROFS)
            error (0, link_errno, _("failed to create hard link %s"),
                   dest_quoted);
          లేకపోతే
            error (0, link_errno, _("failed to create hard link %s => %s"),
                   dest_quoted, ఆధారం_quoted);
        }

      ఒకవేళ (backup_పునాది)
        {
          ఒకవేళ (renameat (destdir_fd, backup_పునాది, destdir_fd, dest_పునాది) != 0)
            error (0, errno, _("cannot un-backup %s"), quoteaf (dest));
        }
    }

  విడుదల (backup_పునాది);
  విడుదల (rel_ఆధారం);
  తిరిగి link_errno <= 0;

fail:
  విడుదల (rel_ఆధారం);
  తిరిగి false;
}

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("\
Usage: %s [OPTION]... [-T] లక్ష్యం_2 LINK_NAME\n\
  or:  %s [OPTION]... లక్ష్యం_2\n\
  or:  %s [OPTION]... లక్ష్యం_2... DIRECTORY\n\
  or:  %s [OPTION]... -t DIRECTORY లక్ష్యం_2...\n\
"),
              కార్యక్రమం_name, కార్యక్రమం_name, కార్యక్రమం_name, కార్యక్రమం_name);
      fputs (_("\
లోపల_2 the 1st form, create a link గమ్యం లక్ష్యం_2 with the name LINK_NAME.\n\
లోపల_2 the 2nd form, create a link గమ్యం లక్ష్యం_2 లోపల the current directory.\n\
లోపల_2 the 3rd and 4th forms, create links గమ్యం each లక్ష్యం_2 లోపల DIRECTORY.\n\
Create hard links by అప్రమేయ, symbolic links with --symbolic.\n\
By అప్రమేయ, each destination (name యొక్క new link) should not already exist.\n\
When creating hard links, each లక్ష్యం_2 must exist.  Symbolic links\n\
can hold arbitrary పాఠ్యం; ఒకవేళ later resolved, a relative link is\n\
interpreted లోపల సంబంధం గమ్యం its మాతృ directory.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
      --backup[=CONTROL]\n\
         make a backup యొక్క each existing destination file\n\
"));
      oputs (_("\
  -b\n\
         like --backup but does not accept an వాదం\n\
"));
      oputs (_("\
  -d, -F, --directory\n\
         allow the superuser గమ్యం attempt గమ్యం hard link directories,\n\
         ఒకవేళ supported by the system\n\
"));
      oputs (_("\
  -f, --force\n\
         remove existing destination files\n\
"));
      oputs (_("\
  -i, --interactive\n\
         prompt whether గమ్యం remove destinations\n\
"));
      oputs (_("\
  -L, --logical\n\
         dereference TARGETs that are symbolic links\n\
"));
      oputs (_("\
  -n, --no-dereference\n\
         treat LINK_NAME as a సాధారణ file\n\
         ఒకవేళ it is a symbolic link గమ్యం a directory\n\
"));
      oputs (_("\
  -P, --physical\n\
         make hard links directly గమ్యం symbolic links\n\
"));
      oputs (_("\
  -r, --relative\n\
         with -s, create links relative గమ్యం link location\n\
"));
      oputs (_("\
  -s, --symbolic\n\
         make symbolic links instead యొక్క hard links\n\
"));
      oputs (_("\
  -S, --suffix=SUFFIX\n\
         override the usual backup suffix\n\
"));
      oputs (_("\
  -t, --లక్ష్యం-directory=DIRECTORY\n\
         specify the DIRECTORY లోపల which గమ్యం create the links\n\
"));
      oputs (_("\
  -T, --no-లక్ష్యం-directory\n\
         treat LINK_NAME as a సాధారణ file always\n\
"));
      oputs (_("\
  -v, --verbose\n\
         print name యొక్క each linked file\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      emit_backup_suffix_note ();
      రాయి (_("\
\n\
Using -s ignores -L and -P.  Otherwise, the చివరి option specified controls\n\
behavior when a లక్ష్యం_2 is a symbolic link, defaulting గమ్యం %s.\n\
"), LINK_FOLLOWS_SYMLINKS ? "-L" : "-P");
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  పూర్ణాంకం c;
  bool ok;
  bool make_backups = false;
  అక్షరం స్థిరరాశి *backup_suffix = ఖాళీ;
  అక్షరం *సంస్కరణ_control_string = ఖాళీ;
  అక్షరం స్థిరరాశి *లక్ష్యం_directory = ఖాళీ;
  పూర్ణాంకం destdir_fd;
  bool no_లక్ష్యం_directory = false;
  పూర్ణాంకం n_files;
  అక్షరం **file;
  పూర్ణాంకం link_errno = -1;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdin);

  అయితే ((c = getopt_long (argc, argv, "bdfinrst:vFLPS:T",
                           long_options, ఖాళీ))
         != -1)
    {
      ఎంపిక (c)
        {
        స్థితి 'b':
          make_backups = true;
          ఒకవేళ (optarg)
            సంస్కరణ_control_string = optarg;
          ఆపు;
        స్థితి 'd':
        స్థితి 'F':
          hard_dir_link = true;
          ఆపు;
        స్థితి 'f':
          remove_existing_files = true;
          interactive = false;
          ఆపు;
        స్థితి 'i':
          remove_existing_files = false;
          interactive = true;
          ఆపు;
        స్థితి 'L':
          logical = true;
          ఆపు;
        స్థితి 'n':
          dereference_dest_dir_symlinks = false;
          ఆపు;
        స్థితి 'P':
          logical = false;
          ఆపు;
        స్థితి 'r':
          relative = true;
          ఆపు;
        స్థితి 's':
          symbolic_link = true;
          ఆపు;
        స్థితి 't':
          ఒకవేళ (లక్ష్యం_directory)
            error (EXIT_FAILURE, 0, _("multiple target directories specified"));
          లేకపోతే
            {
              నిర్మాణం stat st;
              ఒకవేళ (stat (optarg, &st) != 0)
                error (EXIT_FAILURE, errno, _("failed to access %s"),
                       quoteaf (optarg));
              ఒకవేళ (! S_ISDIR (st.st_బహుళకం))
                error (EXIT_FAILURE, 0, _("target %s is not a directory"),
                       quoteaf (optarg));
            }
          లక్ష్యం_directory = optarg;
          ఆపు;
        స్థితి 'T':
          no_లక్ష్యం_directory = true;
          ఆపు;
        స్థితి 'v':
          verbose = true;
          ఆపు;
        స్థితి 'S':
          make_backups = true;
          backup_suffix = optarg;
          ఆపు;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);
        అప్రమేయ:
          usage (EXIT_FAILURE);
          ఆపు;
        }
    }

  n_files = argc - optind;
  file = argv + optind;

  ఒకవేళ (n_files <= 0)
    {
      error (0, 0, _("missing file operand"));
      usage (EXIT_FAILURE);
    }

  ఒకవేళ (relative && !symbolic_link)
    error (EXIT_FAILURE, 0, _("cannot do --relative without --symbolic"));

  ఒకవేళ (!hard_dir_link)
    {
      priv_సమితి_remove_linkdir ();
      beware_hard_dir_link = !cannot_unlink_dir ();
    }

  ఒకవేళ (no_లక్ష్యం_directory)
    {
      ఒకవేళ (లక్ష్యం_directory)
        error (EXIT_FAILURE, 0,
               _("cannot combine --target-directory "
                 "and --no-target-directory"));
      ఒకవేళ (n_files != 2)
        {
          ఒకవేళ (n_files < 2)
            error (0, 0,
                   _("missing destination file operand after %s"),
                   quoteaf (file[0]));
          లేకపోతే
            error (0, 0, _("extra operand %s"), quoteaf (file[2]));
          usage (EXIT_FAILURE);
        }
    }
  లేకపోతే ఒకవేళ (n_files < 2 && !లక్ష్యం_directory)
    {
      లక్ష్యం_directory = ".";
      destdir_fd = AT_FDCWD;
    }
  లేకపోతే
    {
      ఒకవేళ (n_files == 2 && !లక్ష్యం_directory)
        link_errno = atomic_link (file[0], AT_FDCWD, file[1]);
      ఒకవేళ (link_errno < 0 || link_errno == EEXIST || link_errno == ENOTDIR
          || link_errno == EINVAL)
        {
          అక్షరం స్థిరరాశి *d
            = లక్ష్యం_directory ? లక్ష్యం_directory : file[n_files - 1];
          పూర్ణాంకం flags = (O_PATHSEARCH | O_DIRECTORY
                       | (dereference_dest_dir_symlinks ? 0 : O_NOFOLLOW));
          destdir_fd = openat_safer (AT_FDCWD, d, flags);
          ఒకవేళ (0 <= destdir_fd)
            {
              n_files -= !లక్ష్యం_directory;
              లక్ష్యం_directory = d;
            }
          లేకపోతే ఒకవేళ (! (n_files == 2 && !లక్ష్యం_directory))
            error (EXIT_FAILURE, errno, _("target %s"), quoteaf (d));
        }
    }

  backup_type = (make_backups
                 ? xget_సంస్కరణ (_("backup type"), సంస్కరణ_control_string)
                 : no_backups);
  సమితి_simple_backup_suffix (backup_suffix);


  ఒకవేళ (లక్ష్యం_directory)
    {
      /* Create the data structure we'll use to record which hard links we
         create.  Used to ensure that ln detects an obscure corner case that
         might result in user data loss.  Create it only if needed.  */
      ఒకవేళ (2 <= n_files
          && remove_existing_files
          /* Don't bother trying to protect symlinks, since ln clobbering
             a just-created symlink won't ever lead to real data loss.  */
          && ! symbolic_link
          /* No destination hard link can be clobbered when making
             numbered backups.  */
          && backup_type != numbered_backups)
        {
          dest_సమితి = hash_initialize (DEST_INFO_INITIAL_సామర్థ్యం,
                                      ఖాళీ,
                                      triple_hash,
                                      triple_compare,
                                      triple_free);
          ఒకవేళ (dest_సమితి == ఖాళీ)
            xalloc_die ();
        }

      ok = true;
      కొరకు (పూర్ణాంకం i = 0; i < n_files; ++i)
        {
          అక్షరం *dest_పునాది;
          అక్షరం *dest = file_name_concat (లక్ష్యం_directory,
                                         చివరి_component (file[i]),
                                         &dest_పునాది);
          strip_trailing_slashes (dest_పునాది);
          ok &= do_link (file[i], destdir_fd, dest_పునాది, dest, -1);
          విడుదల (dest);
        }
    }
  లేకపోతే
    ok = do_link (file[0], AT_FDCWD, file[1], file[1], link_errno);

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
