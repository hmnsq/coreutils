/* install - copy files and set attributes
   Copyright (C) 1989-2026 Free Software Foundation, Inc.

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

/* Written by David MacKenzie <djm@gnu.ai.mit.edu> */

#অন্তর্ভাৱ <config.h>
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <getopt.h>
#অন্তর্ভাৱ <sys/types.h>
#অন্তর্ভাৱ <signal.h>
#অন্তর্ভাৱ <pwd.h>
#অন্তর্ভাৱ <grp.h>
#অন্তর্ভাৱ <selinux/label.h>
#অন্তর্ভাৱ <spawn.h>
#অন্তর্ভাৱ <sys/wait.h>

#অন্তর্ভাৱ "system.h"
#অন্তর্ভাৱ "backupfile.h"
#অন্তর্ভাৱ "cp-hash.h"
#অন্তর্ভাৱ "copy.h"
#অন্তর্ভাৱ "filenamecat.h"
#অন্তর্ভাৱ "full-read.h"
#অন্তর্ভাৱ "mkancesdirs.h"
#অন্তর্ভাৱ "mkdir-p.h"
#অন্তর্ভাৱ "modechange.h"
#অন্তর্ভাৱ "prog-fprintf.h"
#অন্তর্ভাৱ "quote.h"
#অন্তর্ভাৱ "savewd.h"
#অন্তর্ভাৱ "selinux.h"
#অন্তর্ভাৱ "stat-time.h"
#অন্তর্ভাৱ "targetdir.h"
#অন্তর্ভাৱ "utimens.h"
#অন্তর্ভাৱ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "install"

#define AUTHORS proper_name ("David MacKenzie")

স্থির পূর্ণাঙ্ক selinux_enabled = 0;
স্থির bool use_default_selinux_context = true;

#যদি ! HAVE_ENDGRENT
# define endgrent() ((শূন্য) 0)
#endif

#যদি ! HAVE_ENDPWENT
# define endpwent() ((শূন্য) 0)
#endif

/* The user name that will own the files, or NULL to make the owner
   the current user ID. */
স্থির ৱর্ণ *owner_name;

/* The user ID corresponding to 'owner_name'. */
স্থির uid_t owner_id;

/* The group name that will own the files, or NULL to make the group
   the current group ID. */
স্থির ৱর্ণ *group_name;

/* The group ID corresponding to 'group_name'. */
স্থির gid_t group_id;

#define DEFAULT_MODE (S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)

/* The file mode bits to which non-directory files will be set.  The umask has
   no effect. */
স্থির mode_t mode = DEFAULT_MODE;

/* Similar, but for directories.  */
স্থির mode_t dir_mode = DEFAULT_MODE;

/* The file mode bits that the user cares about.  This should be a
   superset of DIR_MODE and a subset of CHMOD_MODE_BITS.  This matters
   for directories, since otherwise directories may keep their S_ISUID
   or S_ISGID bits.  */
স্থির mode_t dir_mode_bits = CHMOD_MODE_BITS;

/* Compare files before installing (-C) */
স্থির bool copy_only_if_needed;

/* If true, strip executable files after copying them. */
স্থির bool strip_files;

/* If true, install a directory instead of a regular file. */
স্থির bool dir_arg;

/* Program used to strip binaries, "strip" is default */
স্থির ৱর্ণ ধ্রুৱ *strip_program = "strip";

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
গণনা
{
  DEBUG_OPTION = CHAR_MAX + 1,
  PRESERVE_CONTEXT_OPTION,
  STRIP_PROGRAM_OPTION
};

স্থির সংরচনা option ধ্রুৱ long_options[] =
{
  {"backup", optional_argument, রিক্ত, 'b'},
  {"compare", no_argument, রিক্ত, 'C'},
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {"debug", no_argument, রিক্ত, DEBUG_OPTION},
  {"directory", no_argument, রিক্ত, 'd'},
  {"group", required_argument, রিক্ত, 'g'},
  {"mode", required_argument, রিক্ত, 'm'},
  {"no-target-directory", no_argument, রিক্ত, 'T'},
  {"owner", required_argument, রিক্ত, 'o'},
  {"preserve-timestamps", no_argument, রিক্ত, 'p'},
  {"preserve-context", no_argument, রিক্ত, PRESERVE_CONTEXT_OPTION},
  {"strip", no_argument, রিক্ত, 's'},
  {"strip-program", required_argument, রিক্ত, STRIP_PROGRAM_OPTION},
  {"suffix", required_argument, রিক্ত, 'S'},
  {"target-directory", required_argument, রিক্ত, 't'},
  {"verbose", no_argument, রিক্ত, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {রিক্ত, 0, রিক্ত, 0}
};

/* Compare content of opened files using file descriptors A_FD and B_FD. Return
   true if files are equal. */
স্থির bool
have_same_content (পূর্ণাঙ্ক a_fd, পূর্ণাঙ্ক b_fd)
{
  গণনা { CMP_BLOCK_SIZE = 4096 };
  স্থির ৱর্ণ a_buff[CMP_BLOCK_SIZE];
  স্থির ৱর্ণ b_buff[CMP_BLOCK_SIZE];

  idx_t size;
  যেতিয়ালৈ (0 < (size = full_read (a_fd, a_buff, পরিমাণ a_buff))) {
    যদি (size != full_read (b_fd, b_buff, পরিমাণ b_buff))
      ঘূৰাই false;

    যদি (!memeq (a_buff, b_buff, size))
      ঘূৰাই false;
  }

  ঘূৰাই size == 0;
}

/* Return true for mode with non-permission bits. */
স্থির bool
extra_mode (mode_t প্ৰবেশ)
{
  mode_t mask = S_IRWXUGO | S_IFMT;
  ঘূৰাই !! (প্ৰবেশ & ~ mask);
}

/* Return true if copy of file SRC_NAME to file DEST_NAME aka
   DEST_DIRFD+DEST_RELNAME is necessary. */
স্থির bool
need_copy (ৱর্ণ ধ্রুৱ *src_name, ৱর্ণ ধ্রুৱ *dest_name,
           পূর্ণাঙ্ক dest_dirfd, ৱর্ণ ধ্রুৱ *dest_relname,
           ধ্রুৱ সংরচনা cp_options *x)
{
  যদি (extra_mode (mode))
    ঘূৰাই true;

  /* compare files using stat */
  সংরচনা stat src_sb;
  যদি (stat (src_name, &src_sb) != 0)
    ঘূৰাই true;

  সংরচনা stat dest_sb;
  যদি (fstatat (dest_dirfd, dest_relname, &dest_sb, AT_SYMLINK_NOFOLLOW) != 0)
    ঘূৰাই true;

  যদি (!S_ISREG (src_sb.st_mode) || !S_ISREG (dest_sb.st_mode)
      || extra_mode (src_sb.st_mode) || extra_mode (dest_sb.st_mode))
    ঘূৰাই true;

  যদি (src_sb.st_size != dest_sb.st_size
      || (dest_sb.st_mode & CHMOD_MODE_BITS) != mode)
    ঘূৰাই true;

  যদি (owner_id == (uid_t) -1)
    {
      errno = 0;
      uid_t ruid = getuid ();
      যদি ((ruid == (uid_t) -1 && errno) || dest_sb.st_uid != ruid)
        ঘূৰাই true;
    }
  অন্যথা যদি (dest_sb.st_uid != owner_id)
    ঘূৰাই true;

  যদি (group_id == (uid_t) -1)
    {
      errno = 0;
      gid_t rgid = getgid ();
      যদি ((rgid == (uid_t) -1 && errno) || dest_sb.st_gid != rgid)
        ঘূৰাই true;
    }
  অন্যথা যদি (dest_sb.st_gid != group_id)
    ঘূৰাই true;

  /* compare SELinux context if preserving */
  যদি (selinux_enabled && x->preserve_security_context)
    {
      ৱর্ণ *file_scontext_raw = রিক্ত;
      যদি (getfilecon_raw (src_name, &file_scontext_raw) == -1)
        ঘূৰাই true;

      ৱর্ণ *to_scontext_raw = রিক্ত;
      যদি (getfilecon_raw (dest_name, &to_scontext_raw) == -1)
        {
          freecon (file_scontext_raw);
          ঘূৰাই true;
        }

      bool scontext_match = streq (file_scontext_raw, to_scontext_raw);

      freecon (file_scontext_raw);
      freecon (to_scontext_raw);
      যদি (!scontext_match)
        ঘূৰাই true;
    }

  /* compare files content */
  পূর্ণাঙ্ক src_fd = open (src_name, O_RDONLY | O_দ্বিমিক);
  যদি (src_fd < 0)
    ঘূৰাই true;

  পূর্ণাঙ্ক dest_fd = openat (dest_dirfd, dest_relname, O_RDONLY | O_দ্বিমিক);
  যদি (dest_fd < 0)
    {
      close (src_fd);
      ঘূৰাই true;
    }

  bool content_match = have_same_content (src_fd, dest_fd);

  close (src_fd);
  close (dest_fd);
  ঘূৰাই !content_match;
}

স্থির শূন্য
cp_option_init (সংরচনা cp_options *x)
{
  cp_options_default (x);
  x->copy_as_regular = true;
  x->reflink_mode = REFLINK_AUTO;
  x->dereference = DEREF_ALWAYS;
  x->unlink_dest_before_opening = true;
  x->unlink_dest_after_failed_open = false;
  x->hard_link = false;
  x->interactive = I_UNSPECIFIED;
  x->move_mode = false;
  x->install_mode = true;
  x->one_file_system = false;
  x->preserve_ownership = false;
  x->preserve_links = false;
  x->preserve_mode = false;
  x->preserve_timestamps = false;
  x->explicit_no_preserve_mode = false;
  x->reduce_diagnostics=false;
  x->data_copy_required = true;
  x->require_preserve = false;
  x->require_preserve_xattr = false;
  x->recursive = false;
  x->sparse_mode = SPARSE_AUTO;
  x->symbolic_link = false;
  x->backup_type = no_backups;

  /* Create destination files initially writable so we can run strip on them.
     Although GNU strip works fine on read-only files, some others
     would fail.  */
  x->set_mode = true;
  x->mode = S_IRUSR | S_IWUSR;
  x->stdin_tty = false;

  x->open_dangling_dest_symlink = false;
  x->update = UPDATE_ALL;
  x->require_preserve_context = false;  /* Not used by install currently.  */
  x->preserve_security_context = false; /* Whether to copy context from src.  */
  x->set_security_context = রিক্ত; /* Whether to set sys default context.  */
  x->preserve_xattr = false;
  x->verbose = false;
  x->dest_info = রিক্ত;
  x->src_info = রিক্ত;
}

স্থির সংরচনা selabel_handle *
get_labeling_handle (শূন্য)
{
  স্থির bool initialized;
  স্থির সংরচনা selabel_handle *hnd;
  যদি (!initialized)
    {
      initialized = true;
      hnd = selabel_open (SELABEL_CTX_FILE, রিক্ত, 0);
      যদি (!hnd)
        error (0, errno, _("warning: security labeling handle failed"));
    }
  ঘূৰাই hnd;
}

/* Modify file context to match the specified policy.
   If an error occurs the file will remain with the default directory
   context.  Note this sets the context to that returned by selabel_lookup
   and thus discards MLS levels and user identity of the FILE.  */
স্থির শূন্য
setdefaultfilecon (ৱর্ণ ধ্রুৱ *file)
{
  যদি (selinux_enabled != 1)
    {
      /* Indicate no context found. */
      ঘূৰাই;
    }

  সংরচনা stat st;
  যদি (lstat (file, &st) != 0)
    ঘূৰাই;

  সংরচনা selabel_handle *hnd = get_labeling_handle ();
  যদি (!hnd)
    ঘূৰাই;

  ৱর্ণ *scontext_raw = রিক্ত;
  যদি (selabel_lookup_raw (hnd, &scontext_raw, file, st.st_mode) != 0)
    {
      যদি (errno != ENOENT && ! ignorable_ctx_err (errno))
        error (0, errno, _("warning: %s: context lookup failed"),
               quotef (file));
      ঘূৰাই;
    }

  যদি (lsetfilecon_raw (file, scontext_raw) < 0 && errno != ENOTSUP)
    error (0, errno,
           _("warning: %s: failed to change context to %s"),
           quotef_n (0, file), quote_n (1, scontext_raw));

  freecon (scontext_raw);
}

/* Report that directory DIR was made, if OPTIONS requests this.  */
স্থির শূন্য
announce_mkdir (ৱর্ণ ধ্রুৱ *dir, শূন্য *options)
{
  সংরচনা cp_options ধ্রুৱ *x = options;
  যদি (x->verbose)
    prog_fprintf (stdout, _("creating directory %s"), quoteaf (dir));
}

/* Make ancestor directory DIR, whose last file name component is
   COMPONENT, with options OPTIONS.  Assume the working directory is
   COMPONENT's parent.  */
স্থির পূর্ণাঙ্ক
make_ancestor (ৱর্ণ ধ্রুৱ *dir, ৱর্ণ ধ্রুৱ *component, শূন্য *options)
{
  সংরচনা cp_options ধ্রুৱ *x = options;
  যদি (x->set_security_context
      && defaultcon (x->set_security_context, component, S_IFDIR) < 0
      && ! ignorable_ctx_err (errno))
    error (0, errno, _("failed to set default creation context for %s"),
           quoteaf (dir));

  পূর্ণাঙ্ক r = mkdir (component, DEFAULT_MODE);
  যদি (r == 0)
    announce_mkdir (dir, options);
  ঘূৰাই r;
}

/* Process a command-line file name, for the -d option.  */
স্থির পূর্ণাঙ্ক
process_dir (ৱর্ণ *dir, সংরচনা savewd *wd, শূন্য *options)
{
  সংরচনা cp_options ধ্রুৱ *x = options;

  পূর্ণাঙ্ক ret = (make_dir_parents (dir, wd, make_ancestor, options,
                               dir_mode, announce_mkdir,
                               dir_mode_bits, owner_id, group_id, false)
          ? EXIT_SUCCESS
          : EXIT_FAILURE);

  /* FIXME: Due to the current structure of make_dir_parents()
     we don't have the facility to call defaultcon() before the
     final component of DIR is created.  So for now, create the
     final component with the context from previous component
     and here we set the context for the final component. */
  যদি (ret == EXIT_SUCCESS && x->set_security_context)
    {
      যদি (! restorecon (x->set_security_context, last_component (dir), false)
          && ! ignorable_ctx_err (errno))
        error (0, errno, _("failed to restore context for %s"),
               quoteaf (dir));
    }

  ঘূৰাই ret;
}

গণনা copy_status { COPY_FAILED, COPY_OK, COPY_SKIPPED };

/* Copy file FROM onto file TO aka TO_DIRFD+TO_RELNAME, creating TO if
   necessary.  Return COPY_OK if successful or COPY_SKIPPED if the copy
   was skipped.  */

স্থির গণনা copy_status
copy_file (ৱর্ণ ধ্রুৱ *from, ৱর্ণ ধ্রুৱ *to,
           পূর্ণাঙ্ক to_dirfd, ৱর্ণ ধ্রুৱ *to_relname, ধ্রুৱ সংরচনা cp_options *x)
{
  যদি (copy_only_if_needed && !need_copy (from, to, to_dirfd, to_relname, x))
    ঘূৰাই COPY_SKIPPED;

  /* Allow installing from non-regular files like /dev/null.
     Charles Karney reported that some Sun version of install allows that
     and that sendmail's installation process relies on the behavior.
     However, since !x->recursive, the call to "copy" will fail if FROM
     is a directory.  */

  bool copy_into_self;
  ঘূৰাই (copy (from, to, to_dirfd, to_relname, 0, x, &copy_into_self, রিক্ত)
          ? COPY_OK : COPY_FAILED);
}

/* Set the attributes of file or directory NAME aka DIRFD+RELNAME.
   Return true if successful.  */

স্থির bool
change_attributes (ৱর্ণ ধ্রুৱ *name, পূর্ণাঙ্ক dirfd, ৱর্ণ ধ্রুৱ *relname)
{
  bool ok = false;
  /* chown must precede chmod because on some systems,
     chown clears the set[ug]id bits for non-superusers,
     resulting in incorrect permissions.
     On System V, users can give away files with chown and then not
     be able to chmod them.  So don't give files away.

     We don't normally ignore errors from chown because the idea of
     the install command is that the file is supposed to end up with
     precisely the attributes that the user specified (or defaulted).
     If the file doesn't end up with the group they asked for, they'll
     want to know.  */

  যদি (! (owner_id == (uid_t) -1 && group_id == (gid_t) -1)
      && lchownat (dirfd, relname, owner_id, group_id) != 0)
    error (0, errno, _("cannot change ownership of %s"), quoteaf (name));
  অন্যথা যদি (chmodat (dirfd, relname, mode) != 0)
    error (0, errno, _("cannot change permissions of %s"), quoteaf (name));
  অন্যথা
    ok = true;

  যদি (use_default_selinux_context)
    setdefaultfilecon (name);

  ঘূৰাই ok;
}

/* Set the timestamps of file DEST aka DIRFD+RELNAME to match those of SRC_SB.
   Return true if successful.  */

স্থির bool
change_timestamps (সংরচনা stat ধ্রুৱ *src_sb, ৱর্ণ ধ্রুৱ *dest,
                   পূর্ণাঙ্ক dirfd, ৱর্ণ ধ্রুৱ *relname)
{
  সংরচনা timespec timespec[2] = { get_stat_atime (src_sb),
                                  get_stat_mtime (src_sb) };

  যদি (utimensat (dirfd, relname, timespec, 0))
    {
      error (0, errno, _("cannot set timestamps for %s"), quoteaf (dest));
      ঘূৰাই false;
    }
  ঘূৰাই true;
}

/* Strip the symbol table from the file NAME.
   We could dig the magic number out of the file first to
   determine whether to strip it, but the header files and
   magic numbers vary so much from system to system that making
   it portable would be very difficult.  Not worth the effort. */

স্থির bool
strip (ৱর্ণ ধ্রুৱ *name)
{
  posix_spawnattr_t attr;
  posix_spawnattr_t *attrp = রিক্ত;

  /* Try to use vfork for systems where it matters.  */
  যদি (posix_spawnattr_init (&attr) == 0)
    {
      যদি (posix_spawnattr_setflags (&attr, POSIX_SPAWN_USEVFORK) == 0)
        attrp = &attr;
      অন্যথা
        posix_spawnattr_destroy (&attr);
    }

  /* Construct the arguments to 'strip'.  */
  ৱর্ণ *concat_name = রিক্ত;
  ৱর্ণ ধ্রুৱ *safe_name = name;
  যদি (name && *name == '-')
    safe_name = concat_name = file_name_concat (".", name, রিক্ত);
  ৱর্ণ ধ্রুৱ *ধ্রুৱ argv[] = { strip_program, safe_name, রিক্ত };

  /* Run 'strip'.  */
  pid_t pid;
  পূর্ণাঙ্ক result = posix_spawnp (&pid, strip_program, রিক্ত, attrp,
                             (ৱর্ণ * ধ্রুৱ *) argv, environ);

  bool ok = false;
  যদি (result != 0)
    {
      error (0, result,
             streq (strip_program, "strip")
               ? _("cannot run %s")
               : _("cannot run strip program %s"),
             quoteaf (strip_program));
    }
  অন্যথা
    {
      /* Wait for 'strip' to complete, and emit a warning message on failure  */
      পূর্ণাঙ্ক status;
      যদি (waitpid (pid, &status, 0) < 0)
        error (0, errno, _("waiting for strip"));
      অন্যথা যদি (! WIFEXITED (status) || WEXITSTATUS (status))
        error (0, 0, _("strip process terminated abnormally"));
      অন্যথা
        ok = true;
    }

  মুক্ত (concat_name);
  যদি (attrp)
    posix_spawnattr_destroy (attrp);

  ঘূৰাই ok;
}

/* Initialize the user and group ownership of the files to install. */

স্থির শূন্য
get_ids (শূন্য)
{
  যদি (owner_name)
    {
      সংরচনা passwd *pw = getpwnam (owner_name);
      যদি (pw == রিক্ত)
        {
          uintmax_t tmp;
          যদি (xstrtoumax (owner_name, রিক্ত, 0, &tmp, "") != LONGINT_OK
              || ckd_add (&owner_id, tmp, 0))
            error (EXIT_FAILURE, 0, _("invalid user %s"),
                   quoteaf (owner_name));
        }
      অন্যথা
        owner_id = pw->pw_uid;
      endpwent ();
    }
  অন্যথা
    owner_id = (uid_t) -1;

  যদি (group_name)
    {
      সংরচনা group *gr = getgrnam (group_name);
      যদি (gr == রিক্ত)
        {
          uintmax_t tmp;
          যদি (xstrtoumax (group_name, রিক্ত, 0, &tmp, "") != LONGINT_OK
              || ckd_add (&group_id, tmp, 0))
            error (EXIT_FAILURE, 0, _("invalid group %s"),
                   quoteaf (group_name));
        }
      অন্যথা
        group_id = gr->gr_gid;
      endgrent ();
    }
  অন্যথা
    group_id = (gid_t) -1;
}

শূন্য
usage (পূর্ণাঙ্ক status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
    {
      printf (_("\
Usage: %s [OPTION]... [-T] SOURCE DEST\n\
  or:  %s [OPTION]... SOURCE... DIRECTORY\n\
  or:  %s [OPTION]... -t DIRECTORY SOURCE...\n\
  or:  %s [OPTION]... -d DIRECTORY...\n\
"),
              program_name, program_name, program_name, program_name);
      fputs (_("\
\n\
This install program copies files (often just compiled) into destination\n\
locations you choose.  If you want to download and install a ready-to-use\n\
package on a GNU/Linux system, you should instead be using a package manager\n\
like yum(1) or apt-get(1).\n\
\n\
In the first three forms, copy SOURCE to DEST or multiple SOURCE(s) to\n\
the existing DIRECTORY, যেতিয়ালৈ setting permission modes and owner/group.\n\
In the 4th form, create all components of the given DIRECTORY(ies).\n\
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
  -c\n\
         (ignored)\n\
"));
      oputs (_("\
  -C, --compare\n\
         compare content of source and destination files,\n\
         and যদি no change to content, ownership, and permissions,\n\
         কৰা not modify the destination at all\n\
"));
      oputs (_("\
  -d, --directory\n\
         treat all arguments as directory names;\n\
         create all components of the specified directories\n\
"));
      oputs (_("\
  -D\n\
         create all leading components of DEST except the last,\n\
         or all components of --target-directory,\n\
         then copy SOURCE to DEST\n\
"));
      oputs (_("\
      --debug\n\
         explain how a file is copied.  Implies -v\n\
"));
      oputs (_("\
  -g, --group=GROUP\n\
         set group ownership, instead of process' current group\n\
"));
      oputs (_("\
  -m, --mode=MODE\n\
         set permission mode (as in chmod), instead of rwxr-xr-x\n\
"));
      oputs (_("\
  -o, --owner=OWNER\n\
         set ownership (super-user only)\n\
"));
      oputs (_("\
  -p, --preserve-timestamps\n\
         apply access/modification times of SOURCE files\n\
         to corresponding destination files\n\
"));
      oputs (_("\
  -s, --strip\n\
         strip প্ৰতীক tables\n\
"));
      oputs (_("\
      --strip-program=PROGRAM\n\
         program used to strip binaries\n\
"));
      oputs (_("\
  -S, --suffix=SUFFIX\n\
         override the usual backup suffix\n\
"));
      oputs (_("\
  -t, --target-directory=DIRECTORY\n\
         copy all SOURCE arguments into DIRECTORY\n\
"));
      oputs (_("\
  -T, --no-target-directory\n\
         treat DEST as a normal file\n\
"));
      oputs (_("\
  -v, --verbose\n\
         print the name of each created file or directory\n\
"));
      oputs (_("\
      --preserve-context\n\
         preserve SELinux security context\n\
"));
      oputs (_("\
  -Z\n\
         set SELinux security context of destination file\n\
         and each created directory, to স্ৱভাৱ type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or যদি CTX is specified then set the\n\
         SELinux or SMACK security context to CTX\n\
"));

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_backup_suffix_note ();
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Copy file FROM onto file TO aka TO_DIRFD+TO_RELNAME and give TO the
   appropriate attributes.  X gives the command options.
   Return true if successful.  */

স্থির bool
install_file_in_file (ৱর্ণ ধ্রুৱ *from, ৱর্ণ ধ্রুৱ *to,
                      পূর্ণাঙ্ক to_dirfd, ৱর্ণ ধ্রুৱ *to_relname,
                      ধ্রুৱ সংরচনা cp_options *x)
{
  সংরচনা stat from_sb;
  যদি (x->preserve_timestamps && stat (from, &from_sb) != 0)
    {
      error (0, errno, _("cannot stat %s"), quoteaf (from));
      ঘূৰাই false;
    }
  গণনা copy_status copy_status = copy_file (from, to, to_dirfd, to_relname, x);
  যদি (copy_status == COPY_FAILED)
    ঘূৰাই false;
  যদি (copy_status == COPY_OK && strip_files && ! strip (to))
    {
      যদি (unlinkat (to_dirfd, to_relname, 0) != 0)  /* Cleanup.  */
        error (EXIT_FAILURE, errno, _("cannot unlink %s"), quoteaf (to));
      ঘূৰাই false;
    }
  যদি (x->preserve_timestamps && (copy_status == COPY_SKIPPED || strip_files
                                 || ! S_ISREG (from_sb.st_mode))
      && ! change_timestamps (&from_sb, to, to_dirfd, to_relname))
    ঘূৰাই false;

  ঘূৰাই copy_status == COPY_SKIPPED
         || change_attributes (to, to_dirfd, to_relname);
}

/* Create any missing parent directories of TO,
   while maintaining the current Working Directory.
   Return true if successful.  */

স্থির bool
mkancesdirs_safe_wd (ৱর্ণ ধ্রুৱ *from, ৱর্ণ *to, সংরচনা cp_options *x,
                     bool save_always)
{
  bool save_working_directory =
    save_always
    || ! (IS_ABSOLUTE_FILE_NAME (from) && IS_ABSOLUTE_FILE_NAME (to));

  সংরচনা savewd wd;
  savewd_init (&wd);
  যদি (! save_working_directory)
    savewd_finish (&wd);

  পূর্ণাঙ্ক status = EXIT_SUCCESS;
  যদি (mkancesdirs (to, &wd, make_ancestor, x) == -1)
    {
      error (0, errno, _("cannot create directory %s"), quoteaf (to));
      status = EXIT_FAILURE;
    }

  যদি (save_working_directory)
    {
      পূর্ণাঙ্ক restore_result = savewd_restore (&wd, status);
      পূর্ণাঙ্ক restore_errno = errno;
      savewd_finish (&wd);
      যদি (EXIT_SUCCESS < restore_result)
        ঘূৰাই false;
      যদি (restore_result < 0 && status == EXIT_SUCCESS)
        {
          error (0, restore_errno, _("cannot create directory %s"),
                 quoteaf (to));
          ঘূৰাই false;
        }
    }
  ঘূৰাই status == EXIT_SUCCESS;
}

/* Copy file FROM onto file TO, creating any missing parent directories of TO.
   Return true if successful.  */

স্থির bool
install_file_in_file_parents (ৱর্ণ ধ্রুৱ *from, ৱর্ণ *to,
                              ধ্রুৱ সংরচনা cp_options *x)
{
  ঘূৰাই (mkancesdirs_safe_wd (from, to, (সংরচনা cp_options *)x, false)
          && install_file_in_file (from, to, AT_FDCWD, to, x));
}

/* Copy file FROM into directory TO_DIR, keeping its same name,
   and give the copy the appropriate attributes.
   Return true if successful.  */

স্থির bool
install_file_in_dir (ৱর্ণ ধ্রুৱ *from, ৱর্ণ ধ্রুৱ *to_dir,
                     ধ্রুৱ সংরচনা cp_options *x, bool mkdir_and_install,
                     পূর্ণাঙ্ক *target_dirfd)
{
  ৱর্ণ ধ্রুৱ *from_base = last_component (from);
  ৱর্ণ *to_relname;
  ৱর্ণ *to = file_name_concat (to_dir, from_base, &to_relname);
  bool ret = true;

  যদি (!target_dirfd_valid (*target_dirfd)
      && (ret = mkdir_and_install)
      && (ret = mkancesdirs_safe_wd (from, to, (সংরচনা cp_options *) x, true)))
    {
      পূর্ণাঙ্ক fd = open (to_dir, O_PATHSEARCH | O_DIRECTORY);
      যদি (fd < 0)
        {
          error (0, errno, _("cannot open %s"), quoteaf (to));
          ret = false;
        }
      অন্যথা
        *target_dirfd = fd;
    }

  যদি (ret)
    {
      পূর্ণাঙ্ক to_dirfd = *target_dirfd;
      যদি (!target_dirfd_valid (to_dirfd))
        {
          to_dirfd = AT_FDCWD;
          to_relname = to;
        }
      ret = install_file_in_file (from, to, to_dirfd, to_relname, x);
    }

  মুক্ত (to);
  ঘূৰাই ret;
}

পূর্ণাঙ্ক
main (পূর্ণাঙ্ক argc, ৱর্ণ **argv)
{
  ৱর্ণ ধ্রুৱ *specified_mode = রিক্ত;
  bool make_backups = false;
  ৱর্ণ ধ্রুৱ *backup_suffix = রিক্ত;
  ৱর্ণ *version_control_string = রিক্ত;
  bool mkdir_and_install = false;
  সংরচনা cp_options x;
  ৱর্ণ ধ্রুৱ *target_directory = রিক্ত;
  bool no_target_directory = false;
  bool strip_program_specified = false;
  ৱর্ণ ধ্রুৱ *scontext = রিক্ত;
  /* set iff kernel has extra selinux system calls */
  selinux_enabled = (0 < is_selinux_enabled ());

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdin);

  cp_option_init (&x);

  umask (0);

  পূর্ণাঙ্ক optc;
  যেতিয়ালৈ ((optc = getopt_long (argc, argv, "bcCsDdg:m:o:pt:TvS:Z", long_options,
                              রিক্ত))
         != -1)
    {
      ৱিকল্প (optc)
        {
        পক্ষ 'b':
          make_backups = true;
          যদি (optarg)
            version_control_string = optarg;
          ভঙ্গ;
        পক্ষ 'c':
          ভঙ্গ;
        পক্ষ 'C':
          copy_only_if_needed = true;
          ভঙ্গ;
        পক্ষ 's':
          strip_files = true;
#ifdef SIGCHLD
          /* System V fork+wait does not work if SIGCHLD is ignored.  */
          সঙ্কেত (SIGCHLD, SIG_DFL);
#endif
          ভঙ্গ;
        পক্ষ DEBUG_OPTION:
          x.debug = x.verbose = true;
          ভঙ্গ;
        পক্ষ STRIP_PROGRAM_OPTION:
          strip_program = xstrdup (optarg);
          strip_program_specified = true;
          ভঙ্গ;
        পক্ষ 'd':
          dir_arg = true;
          ভঙ্গ;
        পক্ষ 'D':
          mkdir_and_install = true;
          ভঙ্গ;
        পক্ষ 'v':
          x.verbose = true;
          ভঙ্গ;
        পক্ষ 'g':
          group_name = optarg;
          ভঙ্গ;
        পক্ষ 'm':
          specified_mode = optarg;
          ভঙ্গ;
        পক্ষ 'o':
          owner_name = optarg;
          ভঙ্গ;
        পক্ষ 'p':
          x.preserve_timestamps = true;
          ভঙ্গ;
        পক্ষ 'S':
          make_backups = true;
          backup_suffix = optarg;
          ভঙ্গ;
        পক্ষ 't':
          যদি (target_directory)
            error (EXIT_FAILURE, 0,
                   _("multiple target directories specified"));
          target_directory = optarg;
          ভঙ্গ;
        পক্ষ 'T':
          no_target_directory = true;
          ভঙ্গ;

        পক্ষ PRESERVE_CONTEXT_OPTION:
          যদি (! selinux_enabled)
            {
              error (0, 0, _("WARNING: ignoring --preserve-context; "
                             "this kernel is not SELinux-enabled"));
              ভঙ্গ;
            }
          x.preserve_security_context = true;
          use_default_selinux_context = false;
          ভঙ্গ;
        পক্ষ 'Z':
          যদি (selinux_enabled)
            {
              /* Disable use of the install(1) specific setdefaultfilecon().
                 Note setdefaultfilecon() is different from the newer and more
                 generic restorecon() in that the former sets the context of
                 the dest files to that returned by selabel_lookup directly,
                 thus discarding MLS level and user identity of the file.
                 TODO: consider removing setdefaultfilecon() in future.  */
              use_default_selinux_context = false;

              যদি (optarg)
                scontext = optarg;
              অন্যথা
                x.set_security_context = get_labeling_handle ();
            }
          অন্যথা যদি (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux-enabled kernel"));
            }
          ভঙ্গ;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        স্ৱভাৱ:
          usage (EXIT_FAILURE);
        }
    }

  /* Check for invalid combinations of arguments. */
  যদি (dir_arg && strip_files)
    error (EXIT_FAILURE, 0,
           _("the strip option may not be used when installing a directory"));
  যদি (dir_arg && target_directory)
    error (EXIT_FAILURE, 0,
           _("target directory not allowed when installing a directory"));

  x.backup_type = (make_backups
                   ? xget_version (_("backup type"),
                                   version_control_string)
                   : no_backups);
  set_simple_backup_suffix (backup_suffix);

  যদি (x.preserve_security_context && (x.set_security_context || scontext))
    error (EXIT_FAILURE, 0,
           _("cannot set target context and preserve it"));

  যদি (scontext && setfscreatecon (scontext) < 0)
    error (EXIT_FAILURE, errno,
           _("failed to set default file creation context to %s"),
         quote (scontext));

  পূর্ণাঙ্ক n_files = argc - optind;
  ৱর্ণ **file = argv + optind;

  যদি (n_files <= ! (dir_arg || target_directory))
    {
      যদি (n_files <= 0)
        error (0, 0, _("missing file operand"));
      অন্যথা
        error (0, 0, _("missing destination file operand after %s"),
               quoteaf (file[0]));
      usage (EXIT_FAILURE);
    }

  সংরচনা stat sb;
  পূর্ণাঙ্ক target_dirfd = AT_FDCWD;
  যদি (no_target_directory)
    {
      যদি (target_directory)
        error (EXIT_FAILURE, 0,
               _("cannot combine --target-directory (-t) "
                 "and --no-target-directory (-T)"));
      যদি (2 < n_files)
        {
          error (0, 0, _("extra operand %s"), quoteaf (file[2]));
          usage (EXIT_FAILURE);
        }
    }
  অন্যথা যদি (target_directory)
    {
      target_dirfd = target_directory_operand (target_directory, &sb);
      যদি (! (target_dirfd_valid (target_dirfd)
             || (mkdir_and_install && errno == ENOENT)))
        error (EXIT_FAILURE, errno, _("failed to access %s"),
               quoteaf (target_directory));
    }
  অন্যথা যদি (!dir_arg)
    {
      ৱর্ণ ধ্রুৱ *lastfile = file[n_files - 1];
      পূর্ণাঙ্ক fd = target_directory_operand (lastfile, &sb);
      যদি (target_dirfd_valid (fd))
        {
          target_dirfd = fd;
          target_directory = lastfile;
          n_files--;
        }
      অন্যথা যদি (2 < n_files)
        error (EXIT_FAILURE, errno, _("target %s"), quoteaf (lastfile));
    }

  যদি (specified_mode)
    {
      সংরচনা mode_change *change = mode_compile (specified_mode);
      যদি (!change)
        error (EXIT_FAILURE, 0, _("invalid mode %s"), quote (specified_mode));
      mode = mode_adjust (0, false, 0, change, রিক্ত);
      dir_mode = mode_adjust (0, true, 0, change, &dir_mode_bits);
      মুক্ত (change);
    }

  যদি (strip_program_specified && !strip_files)
    error (0, 0, _("WARNING: ignoring --strip-program option as -s option was "
                   "not specified"));

  যদি (copy_only_if_needed && strip_files)
    {
      error (0, 0, _("options --compare (-C) and --strip are mutually "
                     "exclusive"));
      usage (EXIT_FAILURE);
    }

  যদি (copy_only_if_needed && extra_mode (mode))
    error (0, 0, _("the --compare (-C) option is ignored when you"
                   " specify a mode with non-permission bits"));

  get_ids ();

  পূর্ণাঙ্ক exit_status = EXIT_SUCCESS;
  যদি (dir_arg)
    exit_status = savewd_process_files (n_files, file, process_dir, &x);
  অন্যথা
    {
      /* FIXME: it's a little gross that this initialization is
         required by copy.c::copy. */
      hash_init ();

      যদি (!target_directory)
        {
          যদি (! (mkdir_and_install
                 ? install_file_in_file_parents (file[0], file[1], &x)
                 : install_file_in_file (file[0], file[1], AT_FDCWD,
                                         file[1], &x)))
            exit_status = EXIT_FAILURE;
        }
      অন্যথা
        {
          dest_info_init (&x);
          বাবে (পূর্ণাঙ্ক i = 0; i < n_files; i++)
            যদি (! install_file_in_dir (file[i], target_directory, &x,
                                       i == 0 && mkdir_and_install,
                                       &target_dirfd))
              exit_status = EXIT_FAILURE;
        }
    }

  main_exit (exit_status);
}
