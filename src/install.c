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

#ضمن <config.h>
#ضمن <stdio.h>
#ضمن <getopt.h>
#ضمن <sys/types.h>
#ضمن <signal.h>
#ضمن <pwd.h>
#ضمن <grp.h>
#ضمن <selinux/label.h>
#ضمن <spawn.h>
#ضمن <sys/wait.h>

#ضمن "system.h"
#ضمن "backupfile.h"
#ضمن "cp-hash.h"
#ضمن "copy.h"
#ضمن "filenamecat.h"
#ضمن "full-read.h"
#ضمن "mkancesdirs.h"
#ضمن "mkdir-p.h"
#ضمن "modechange.h"
#ضمن "prog-fprintf.h"
#ضمن "quote.h"
#ضمن "savewd.h"
#ضمن "selinux.h"
#ضمن "stat-time.h"
#ضمن "targetdir.h"
#ضمن "utimens.h"
#ضمن "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "install"

#define AUTHORS proper_name ("David MacKenzie")

ساكن صحيح selinux_enabled = 0;
ساكن bool use_default_selinux_context = true;

#𐡄𐡍 ! HAVE_ENDGRENT
# define endgrent() ((فراغ) 0)
#endif

#𐡄𐡍 ! HAVE_ENDPWENT
# define endpwent() ((فراغ) 0)
#endif

/* The user name that will own the files, or NULL to make the owner
   the current user ID. */
ساكن حرف *owner_name;

/* The user ID corresponding to 'owner_name'. */
ساكن uid_t owner_id;

/* The group name that will own the files, or NULL to make the group
   the current group ID. */
ساكن حرف *group_name;

/* The group ID corresponding to 'group_name'. */
ساكن gid_t group_id;

#define DEFAULT_MODE (S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)

/* The file mode bits to which non-directory files will be set.  The umask has
   no effect. */
ساكن mode_t mode = DEFAULT_MODE;

/* Similar, but for directories.  */
ساكن mode_t dir_mode = DEFAULT_MODE;

/* The file mode bits that the user cares about.  This should be a
   superset of DIR_MODE and a subset of CHMOD_MODE_BITS.  This matters
   for directories, since otherwise directories may keep their S_ISUID
   or S_ISGID bits.  */
ساكن mode_t dir_mode_bits = CHMOD_MODE_BITS;

/* Compare files before installing (-C) */
ساكن bool copy_only_if_needed;

/* If true, strip executable files after copying them. */
ساكن bool strip_files;

/* If true, install a directory instead of a regular file. */
ساكن bool dir_arg;

/* Program used to strip binaries, "strip" is default */
ساكن حرف ثابت *strip_program = "strip";

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
تعداد
{
  DEBUG_OPTION = CHAR_MAX + 1,
  PRESERVE_CONTEXT_OPTION,
  STRIP_PROGRAM_OPTION
};

ساكن بنية option ثابت long_options[] =
{
  {"backup", optional_argument, عدم, 'b'},
  {"compare", no_argument, عدم, 'C'},
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {"debug", no_argument, عدم, DEBUG_OPTION},
  {"directory", no_argument, عدم, 'd'},
  {"group", required_argument, عدم, 'g'},
  {"mode", required_argument, عدم, 'm'},
  {"no-target-directory", no_argument, عدم, 'T'},
  {"owner", required_argument, عدم, 'o'},
  {"preserve-timestamps", no_argument, عدم, 'p'},
  {"preserve-context", no_argument, عدم, PRESERVE_CONTEXT_OPTION},
  {"strip", no_argument, عدم, 's'},
  {"strip-program", required_argument, عدم, STRIP_PROGRAM_OPTION},
  {"suffix", required_argument, عدم, 'S'},
  {"target-directory", required_argument, عدم, 't'},
  {"verbose", no_argument, عدم, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

/* Compare content of opened files using file descriptors A_FD and B_FD. Return
   true if files are equal. */
ساكن bool
have_same_content (صحيح a_fd, صحيح b_fd)
{
  تعداد { CMP_BLOCK_SIZE = 4096 };
  ساكن حرف a_buff[CMP_BLOCK_SIZE];
  ساكن حرف b_buff[CMP_BLOCK_SIZE];

  idx_t size;
  طالما (0 < (size = full_read (a_fd, a_buff, حجم a_buff))) {
    𐡄𐡍 (size != full_read (b_fd, b_buff, حجم b_buff))
      أرجع false;

    𐡄𐡍 (!memeq (a_buff, b_buff, size))
      أرجع false;
  }

  أرجع size == 0;
}

/* Return true for mode with non-permission bits. */
ساكن bool
extra_mode (mode_t input)
{
  mode_t mask = S_IRWXUGO | S_IFMT;
  أرجع !! (input & ~ mask);
}

/* Return true if copy of file SRC_NAME to file DEST_NAME aka
   DEST_DIRFD+DEST_RELNAME is necessary. */
ساكن bool
need_copy (حرف ثابت *src_name, حرف ثابت *dest_name,
           صحيح dest_dirfd, حرف ثابت *dest_relname,
           ثابت بنية cp_options *x)
{
  𐡄𐡍 (extra_mode (mode))
    أرجع true;

  /* compare files using stat */
  بنية stat src_sb;
  𐡄𐡍 (stat (src_name, &src_sb) != 0)
    أرجع true;

  بنية stat dest_sb;
  𐡄𐡍 (fstatat (dest_dirfd, dest_relname, &dest_sb, AT_SYMLINK_NOFOLLOW) != 0)
    أرجع true;

  𐡄𐡍 (!S_ISREG (src_sb.st_mode) || !S_ISREG (dest_sb.st_mode)
      || extra_mode (src_sb.st_mode) || extra_mode (dest_sb.st_mode))
    أرجع true;

  𐡄𐡍 (src_sb.st_size != dest_sb.st_size
      || (dest_sb.st_mode & CHMOD_MODE_BITS) != mode)
    أرجع true;

  𐡄𐡍 (owner_id == (uid_t) -1)
    {
      errno = 0;
      uid_t ruid = getuid ();
      𐡄𐡍 ((ruid == (uid_t) -1 && errno) || dest_sb.st_uid != ruid)
        أرجع true;
    }
  وإلا 𐡄𐡍 (dest_sb.st_uid != owner_id)
    أرجع true;

  𐡄𐡍 (group_id == (uid_t) -1)
    {
      errno = 0;
      gid_t rgid = getgid ();
      𐡄𐡍 ((rgid == (uid_t) -1 && errno) || dest_sb.st_gid != rgid)
        أرجع true;
    }
  وإلا 𐡄𐡍 (dest_sb.st_gid != group_id)
    أرجع true;

  /* compare SELinux context if preserving */
  𐡄𐡍 (selinux_enabled && x->preserve_security_context)
    {
      حرف *file_scontext_raw = عدم;
      𐡄𐡍 (getfilecon_raw (src_name, &file_scontext_raw) == -1)
        أرجع true;

      حرف *to_scontext_raw = عدم;
      𐡄𐡍 (getfilecon_raw (dest_name, &to_scontext_raw) == -1)
        {
          freecon (file_scontext_raw);
          أرجع true;
        }

      bool scontext_match = streq (file_scontext_raw, to_scontext_raw);

      freecon (file_scontext_raw);
      freecon (to_scontext_raw);
      𐡄𐡍 (!scontext_match)
        أرجع true;
    }

  /* compare files content */
  صحيح src_fd = open (src_name, O_RDONLY | O_BINARY);
  𐡄𐡍 (src_fd < 0)
    أرجع true;

  صحيح dest_fd = openat (dest_dirfd, dest_relname, O_RDONLY | O_BINARY);
  𐡄𐡍 (dest_fd < 0)
    {
      close (src_fd);
      أرجع true;
    }

  bool content_match = have_same_content (src_fd, dest_fd);

  close (src_fd);
  close (dest_fd);
  أرجع !content_match;
}

ساكن فراغ
cp_option_init (بنية cp_options *x)
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
  x->set_security_context = عدم; /* Whether to set sys default context.  */
  x->preserve_xattr = false;
  x->verbose = false;
  x->dest_info = عدم;
  x->src_info = عدم;
}

ساكن بنية selabel_handle *
get_labeling_handle (فراغ)
{
  ساكن bool initialized;
  ساكن بنية selabel_handle *hnd;
  𐡄𐡍 (!initialized)
    {
      initialized = true;
      hnd = selabel_open (SELABEL_CTX_FILE, عدم, 0);
      𐡄𐡍 (!hnd)
        error (0, errno, _("warning: security labeling handle failed"));
    }
  أرجع hnd;
}

/* Modify file context to match the specified policy.
   If an error occurs the file will remain with the default directory
   context.  Note this sets the context to that returned by selabel_lookup
   and thus discards MLS levels and user identity of the FILE.  */
ساكن فراغ
setdefaultfilecon (حرف ثابت *file)
{
  𐡄𐡍 (selinux_enabled != 1)
    {
      /* Indicate no context found. */
      أرجع;
    }

  بنية stat st;
  𐡄𐡍 (lstat (file, &st) != 0)
    أرجع;

  بنية selabel_handle *hnd = get_labeling_handle ();
  𐡄𐡍 (!hnd)
    أرجع;

  حرف *scontext_raw = عدم;
  𐡄𐡍 (selabel_lookup_raw (hnd, &scontext_raw, file, st.st_mode) != 0)
    {
      𐡄𐡍 (errno != ENOENT && ! ignorable_ctx_err (errno))
        error (0, errno, _("warning: %s: context lookup failed"),
               quotef (file));
      أرجع;
    }

  𐡄𐡍 (lsetfilecon_raw (file, scontext_raw) < 0 && errno != ENOTSUP)
    error (0, errno,
           _("warning: %s: failed to change context to %s"),
           quotef_n (0, file), quote_n (1, scontext_raw));

  freecon (scontext_raw);
}

/* Report that directory DIR was made, if OPTIONS requests this.  */
ساكن فراغ
announce_mkdir (حرف ثابت *dir, فراغ *options)
{
  بنية cp_options ثابت *x = options;
  𐡄𐡍 (x->verbose)
    prog_fprintf (stdout, _("creating directory %s"), quoteaf (dir));
}

/* Make ancestor directory DIR, whose last file name component is
   COMPONENT, with options OPTIONS.  Assume the working directory is
   COMPONENT's parent.  */
ساكن صحيح
make_ancestor (حرف ثابت *dir, حرف ثابت *component, فراغ *options)
{
  بنية cp_options ثابت *x = options;
  𐡄𐡍 (x->set_security_context
      && defaultcon (x->set_security_context, component, S_IFDIR) < 0
      && ! ignorable_ctx_err (errno))
    error (0, errno, _("failed to set default creation context for %s"),
           quoteaf (dir));

  صحيح r = mkdir (component, DEFAULT_MODE);
  𐡄𐡍 (r == 0)
    announce_mkdir (dir, options);
  أرجع r;
}

/* Process a command-line file name, for the -d option.  */
ساكن صحيح
process_dir (حرف *dir, بنية savewd *wd, فراغ *options)
{
  بنية cp_options ثابت *x = options;

  صحيح ret = (make_dir_parents (dir, wd, make_ancestor, options,
                               dir_mode, announce_mkdir,
                               dir_mode_bits, owner_id, group_id, false)
          ? EXIT_SUCCESS
          : EXIT_FAILURE);

  /* FIXME: Due to the current structure of make_dir_parents()
     we don't have the facility to call defaultcon() before the
     final component of DIR is created.  So for now, create the
     final component with the context from previous component
     and here we set the context for the final component. */
  𐡄𐡍 (ret == EXIT_SUCCESS && x->set_security_context)
    {
      𐡄𐡍 (! restorecon (x->set_security_context, last_component (dir), false)
          && ! ignorable_ctx_err (errno))
        error (0, errno, _("failed to restore context for %s"),
               quoteaf (dir));
    }

  أرجع ret;
}

تعداد copy_status { COPY_FAILED, COPY_OK, COPY_SKIPPED };

/* Copy file FROM onto file TO aka TO_DIRFD+TO_RELNAME, creating TO if
   necessary.  Return COPY_OK if successful or COPY_SKIPPED if the copy
   was skipped.  */

ساكن تعداد copy_status
copy_file (حرف ثابت *from, حرف ثابت *to,
           صحيح to_dirfd, حرف ثابت *to_relname, ثابت بنية cp_options *x)
{
  𐡄𐡍 (copy_only_if_needed && !need_copy (from, to, to_dirfd, to_relname, x))
    أرجع COPY_SKIPPED;

  /* Allow installing from non-regular files like /dev/null.
     Charles Karney reported that some Sun version of install allows that
     and that sendmail's installation process relies on the behavior.
     However, since !x->recursive, the call to "copy" will fail if FROM
     is a directory.  */

  bool copy_into_self;
  أرجع (copy (from, to, to_dirfd, to_relname, 0, x, &copy_into_self, عدم)
          ? COPY_OK : COPY_FAILED);
}

/* Set the attributes of file or directory NAME aka DIRFD+RELNAME.
   Return true if successful.  */

ساكن bool
change_attributes (حرف ثابت *name, صحيح dirfd, حرف ثابت *relname)
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

  𐡄𐡍 (! (owner_id == (uid_t) -1 && group_id == (gid_t) -1)
      && lchownat (dirfd, relname, owner_id, group_id) != 0)
    error (0, errno, _("cannot change ownership of %s"), quoteaf (name));
  وإلا 𐡄𐡍 (chmodat (dirfd, relname, mode) != 0)
    error (0, errno, _("cannot change permissions of %s"), quoteaf (name));
  وإلا
    ok = true;

  𐡄𐡍 (use_default_selinux_context)
    setdefaultfilecon (name);

  أرجع ok;
}

/* Set the timestamps of file DEST aka DIRFD+RELNAME to match those of SRC_SB.
   Return true if successful.  */

ساكن bool
change_timestamps (بنية stat ثابت *src_sb, حرف ثابت *dest,
                   صحيح dirfd, حرف ثابت *relname)
{
  بنية timespec timespec[2] = { get_stat_atime (src_sb),
                                  get_stat_mtime (src_sb) };

  𐡄𐡍 (utimensat (dirfd, relname, timespec, 0))
    {
      error (0, errno, _("cannot set timestamps for %s"), quoteaf (dest));
      أرجع false;
    }
  أرجع true;
}

/* Strip the symbol table from the file NAME.
   We could dig the magic number out of the file first to
   determine whether to strip it, but the header files and
   magic numbers vary so much from system to system that making
   it portable would be very difficult.  Not worth the effort. */

ساكن bool
strip (حرف ثابت *name)
{
  posix_spawnattr_t attr;
  posix_spawnattr_t *attrp = عدم;

  /* Try to use vfork for systems where it matters.  */
  𐡄𐡍 (posix_spawnattr_init (&attr) == 0)
    {
      𐡄𐡍 (posix_spawnattr_setflags (&attr, POSIX_SPAWN_USEVFORK) == 0)
        attrp = &attr;
      وإلا
        posix_spawnattr_destroy (&attr);
    }

  /* Construct the arguments to 'strip'.  */
  حرف *concat_name = عدم;
  حرف ثابت *safe_name = name;
  𐡄𐡍 (name && *name == '-')
    safe_name = concat_name = file_name_concat (".", name, عدم);
  حرف ثابت *ثابت argv[] = { strip_program, safe_name, عدم };

  /* Run 'strip'.  */
  pid_t pid;
  صحيح result = posix_spawnp (&pid, strip_program, عدم, attrp,
                             (حرف * ثابت *) argv, environ);

  bool ok = false;
  𐡄𐡍 (result != 0)
    {
      error (0, result,
             streq (strip_program, "strip")
               ? _("cannot run %s")
               : _("cannot run strip program %s"),
             quoteaf (strip_program));
    }
  وإلا
    {
      /* Wait for 'strip' to complete, and emit a warning message on failure  */
      صحيح status;
      𐡄𐡍 (waitpid (pid, &status, 0) < 0)
        error (0, errno, _("waiting for strip"));
      وإلا 𐡄𐡍 (! WIFEXITED (status) || WEXITSTATUS (status))
        error (0, 0, _("strip process terminated abnormally"));
      وإلا
        ok = true;
    }

  حرر (concat_name);
  𐡄𐡍 (attrp)
    posix_spawnattr_destroy (attrp);

  أرجع ok;
}

/* Initialize the user and group ownership of the files to install. */

ساكن فراغ
get_ids (فراغ)
{
  𐡄𐡍 (owner_name)
    {
      بنية passwd *pw = getpwnam (owner_name);
      𐡄𐡍 (pw == عدم)
        {
          uintmax_t tmp;
          𐡄𐡍 (xstrtoumax (owner_name, عدم, 0, &tmp, "") != LONGINT_OK
              || ckd_add (&owner_id, tmp, 0))
            error (EXIT_FAILURE, 0, _("invalid user %s"),
                   quoteaf (owner_name));
        }
      وإلا
        owner_id = pw->pw_uid;
      endpwent ();
    }
  وإلا
    owner_id = (uid_t) -1;

  𐡄𐡍 (group_name)
    {
      بنية group *gr = getgrnam (group_name);
      𐡄𐡍 (gr == عدم)
        {
          uintmax_t tmp;
          𐡄𐡍 (xstrtoumax (group_name, عدم, 0, &tmp, "") != LONGINT_OK
              || ckd_add (&group_id, tmp, 0))
            error (EXIT_FAILURE, 0, _("invalid group %s"),
                   quoteaf (group_name));
        }
      وإلا
        group_id = gr->gr_gid;
      endgrent ();
    }
  وإلا
    group_id = (gid_t) -1;
}

فراغ
usage (صحيح status)
{
  𐡄𐡍 (status != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
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
the existing DIRECTORY, طالما setting permission modes and owner/group.\n\
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
         and 𐡄𐡍 no change to content, ownership, and permissions,\n\
         𐡏𐡁𐡃 not modify the destination at all\n\
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
         strip symbol tables\n\
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
         and each created directory, to افتراضي type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or 𐡄𐡍 CTX is specified then set the\n\
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

ساكن bool
install_file_in_file (حرف ثابت *from, حرف ثابت *to,
                      صحيح to_dirfd, حرف ثابت *to_relname,
                      ثابت بنية cp_options *x)
{
  بنية stat from_sb;
  𐡄𐡍 (x->preserve_timestamps && stat (from, &from_sb) != 0)
    {
      error (0, errno, _("cannot stat %s"), quoteaf (from));
      أرجع false;
    }
  تعداد copy_status copy_status = copy_file (from, to, to_dirfd, to_relname, x);
  𐡄𐡍 (copy_status == COPY_FAILED)
    أرجع false;
  𐡄𐡍 (copy_status == COPY_OK && strip_files && ! strip (to))
    {
      𐡄𐡍 (unlinkat (to_dirfd, to_relname, 0) != 0)  /* Cleanup.  */
        error (EXIT_FAILURE, errno, _("cannot unlink %s"), quoteaf (to));
      أرجع false;
    }
  𐡄𐡍 (x->preserve_timestamps && (copy_status == COPY_SKIPPED || strip_files
                                 || ! S_ISREG (from_sb.st_mode))
      && ! change_timestamps (&from_sb, to, to_dirfd, to_relname))
    أرجع false;

  أرجع copy_status == COPY_SKIPPED
         || change_attributes (to, to_dirfd, to_relname);
}

/* Create any missing parent directories of TO,
   while maintaining the current Working Directory.
   Return true if successful.  */

ساكن bool
mkancesdirs_safe_wd (حرف ثابت *from, حرف *to, بنية cp_options *x,
                     bool save_always)
{
  bool save_working_directory =
    save_always
    || ! (IS_ABSOLUTE_FILE_NAME (from) && IS_ABSOLUTE_FILE_NAME (to));

  بنية savewd wd;
  savewd_init (&wd);
  𐡄𐡍 (! save_working_directory)
    savewd_finish (&wd);

  صحيح status = EXIT_SUCCESS;
  𐡄𐡍 (mkancesdirs (to, &wd, make_ancestor, x) == -1)
    {
      error (0, errno, _("cannot create directory %s"), quoteaf (to));
      status = EXIT_FAILURE;
    }

  𐡄𐡍 (save_working_directory)
    {
      صحيح restore_result = savewd_restore (&wd, status);
      صحيح restore_errno = errno;
      savewd_finish (&wd);
      𐡄𐡍 (EXIT_SUCCESS < restore_result)
        أرجع false;
      𐡄𐡍 (restore_result < 0 && status == EXIT_SUCCESS)
        {
          error (0, restore_errno, _("cannot create directory %s"),
                 quoteaf (to));
          أرجع false;
        }
    }
  أرجع status == EXIT_SUCCESS;
}

/* Copy file FROM onto file TO, creating any missing parent directories of TO.
   Return true if successful.  */

ساكن bool
install_file_in_file_parents (حرف ثابت *from, حرف *to,
                              ثابت بنية cp_options *x)
{
  أرجع (mkancesdirs_safe_wd (from, to, (بنية cp_options *)x, false)
          && install_file_in_file (from, to, AT_FDCWD, to, x));
}

/* Copy file FROM into directory TO_DIR, keeping its same name,
   and give the copy the appropriate attributes.
   Return true if successful.  */

ساكن bool
install_file_in_dir (حرف ثابت *from, حرف ثابت *to_dir,
                     ثابت بنية cp_options *x, bool mkdir_and_install,
                     صحيح *target_dirfd)
{
  حرف ثابت *from_base = last_component (from);
  حرف *to_relname;
  حرف *to = file_name_concat (to_dir, from_base, &to_relname);
  bool ret = true;

  𐡄𐡍 (!target_dirfd_valid (*target_dirfd)
      && (ret = mkdir_and_install)
      && (ret = mkancesdirs_safe_wd (from, to, (بنية cp_options *) x, true)))
    {
      صحيح fd = open (to_dir, O_PATHSEARCH | O_DIRECTORY);
      𐡄𐡍 (fd < 0)
        {
          error (0, errno, _("cannot open %s"), quoteaf (to));
          ret = false;
        }
      وإلا
        *target_dirfd = fd;
    }

  𐡄𐡍 (ret)
    {
      صحيح to_dirfd = *target_dirfd;
      𐡄𐡍 (!target_dirfd_valid (to_dirfd))
        {
          to_dirfd = AT_FDCWD;
          to_relname = to;
        }
      ret = install_file_in_file (from, to, to_dirfd, to_relname, x);
    }

  حرر (to);
  أرجع ret;
}

صحيح
main (صحيح argc, حرف **argv)
{
  حرف ثابت *specified_mode = عدم;
  bool make_backups = false;
  حرف ثابت *backup_suffix = عدم;
  حرف *version_control_string = عدم;
  bool mkdir_and_install = false;
  بنية cp_options x;
  حرف ثابت *target_directory = عدم;
  bool no_target_directory = false;
  bool strip_program_specified = false;
  حرف ثابت *scontext = عدم;
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

  صحيح optc;
  طالما ((optc = getopt_long (argc, argv, "bcCsDdg:m:o:pt:TvS:Z", long_options,
                              عدم))
         != -1)
    {
      بدل (optc)
        {
        حالة 'b':
          make_backups = true;
          𐡄𐡍 (optarg)
            version_control_string = optarg;
          اكسر;
        حالة 'c':
          اكسر;
        حالة 'C':
          copy_only_if_needed = true;
          اكسر;
        حالة 's':
          strip_files = true;
#ifdef SIGCHLD
          /* System V fork+wait does not work if SIGCHLD is ignored.  */
          إشارة (SIGCHLD, SIG_DFL);
#endif
          اكسر;
        حالة DEBUG_OPTION:
          x.debug = x.verbose = true;
          اكسر;
        حالة STRIP_PROGRAM_OPTION:
          strip_program = xstrdup (optarg);
          strip_program_specified = true;
          اكسر;
        حالة 'd':
          dir_arg = true;
          اكسر;
        حالة 'D':
          mkdir_and_install = true;
          اكسر;
        حالة 'v':
          x.verbose = true;
          اكسر;
        حالة 'g':
          group_name = optarg;
          اكسر;
        حالة 'm':
          specified_mode = optarg;
          اكسر;
        حالة 'o':
          owner_name = optarg;
          اكسر;
        حالة 'p':
          x.preserve_timestamps = true;
          اكسر;
        حالة 'S':
          make_backups = true;
          backup_suffix = optarg;
          اكسر;
        حالة 't':
          𐡄𐡍 (target_directory)
            error (EXIT_FAILURE, 0,
                   _("multiple target directories specified"));
          target_directory = optarg;
          اكسر;
        حالة 'T':
          no_target_directory = true;
          اكسر;

        حالة PRESERVE_CONTEXT_OPTION:
          𐡄𐡍 (! selinux_enabled)
            {
              error (0, 0, _("WARNING: ignoring --preserve-context; "
                             "this kernel is not SELinux-enabled"));
              اكسر;
            }
          x.preserve_security_context = true;
          use_default_selinux_context = false;
          اكسر;
        حالة 'Z':
          𐡄𐡍 (selinux_enabled)
            {
              /* Disable use of the install(1) specific setdefaultfilecon().
                 Note setdefaultfilecon() is different from the newer and more
                 generic restorecon() in that the former sets the context of
                 the dest files to that returned by selabel_lookup directly,
                 thus discarding MLS level and user identity of the file.
                 TODO: consider removing setdefaultfilecon() in future.  */
              use_default_selinux_context = false;

              𐡄𐡍 (optarg)
                scontext = optarg;
              وإلا
                x.set_security_context = get_labeling_handle ();
            }
          وإلا 𐡄𐡍 (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux-enabled kernel"));
            }
          اكسر;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  /* Check for invalid combinations of arguments. */
  𐡄𐡍 (dir_arg && strip_files)
    error (EXIT_FAILURE, 0,
           _("the strip option may not be used when installing a directory"));
  𐡄𐡍 (dir_arg && target_directory)
    error (EXIT_FAILURE, 0,
           _("target directory not allowed when installing a directory"));

  x.backup_type = (make_backups
                   ? xget_version (_("backup type"),
                                   version_control_string)
                   : no_backups);
  set_simple_backup_suffix (backup_suffix);

  𐡄𐡍 (x.preserve_security_context && (x.set_security_context || scontext))
    error (EXIT_FAILURE, 0,
           _("cannot set target context and preserve it"));

  𐡄𐡍 (scontext && setfscreatecon (scontext) < 0)
    error (EXIT_FAILURE, errno,
           _("failed to set default file creation context to %s"),
         quote (scontext));

  صحيح n_files = argc - optind;
  حرف **file = argv + optind;

  𐡄𐡍 (n_files <= ! (dir_arg || target_directory))
    {
      𐡄𐡍 (n_files <= 0)
        error (0, 0, _("missing file operand"));
      وإلا
        error (0, 0, _("missing destination file operand after %s"),
               quoteaf (file[0]));
      usage (EXIT_FAILURE);
    }

  بنية stat sb;
  صحيح target_dirfd = AT_FDCWD;
  𐡄𐡍 (no_target_directory)
    {
      𐡄𐡍 (target_directory)
        error (EXIT_FAILURE, 0,
               _("cannot combine --target-directory (-t) "
                 "and --no-target-directory (-T)"));
      𐡄𐡍 (2 < n_files)
        {
          error (0, 0, _("extra operand %s"), quoteaf (file[2]));
          usage (EXIT_FAILURE);
        }
    }
  وإلا 𐡄𐡍 (target_directory)
    {
      target_dirfd = target_directory_operand (target_directory, &sb);
      𐡄𐡍 (! (target_dirfd_valid (target_dirfd)
             || (mkdir_and_install && errno == ENOENT)))
        error (EXIT_FAILURE, errno, _("failed to access %s"),
               quoteaf (target_directory));
    }
  وإلا 𐡄𐡍 (!dir_arg)
    {
      حرف ثابت *lastfile = file[n_files - 1];
      صحيح fd = target_directory_operand (lastfile, &sb);
      𐡄𐡍 (target_dirfd_valid (fd))
        {
          target_dirfd = fd;
          target_directory = lastfile;
          n_files--;
        }
      وإلا 𐡄𐡍 (2 < n_files)
        error (EXIT_FAILURE, errno, _("target %s"), quoteaf (lastfile));
    }

  𐡄𐡍 (specified_mode)
    {
      بنية mode_change *change = mode_compile (specified_mode);
      𐡄𐡍 (!change)
        error (EXIT_FAILURE, 0, _("invalid mode %s"), quote (specified_mode));
      mode = mode_adjust (0, false, 0, change, عدم);
      dir_mode = mode_adjust (0, true, 0, change, &dir_mode_bits);
      حرر (change);
    }

  𐡄𐡍 (strip_program_specified && !strip_files)
    error (0, 0, _("WARNING: ignoring --strip-program option as -s option was "
                   "not specified"));

  𐡄𐡍 (copy_only_if_needed && strip_files)
    {
      error (0, 0, _("options --compare (-C) and --strip are mutually "
                     "exclusive"));
      usage (EXIT_FAILURE);
    }

  𐡄𐡍 (copy_only_if_needed && extra_mode (mode))
    error (0, 0, _("the --compare (-C) option is ignored when you"
                   " specify a mode with non-permission bits"));

  get_ids ();

  صحيح exit_status = EXIT_SUCCESS;
  𐡄𐡍 (dir_arg)
    exit_status = savewd_process_files (n_files, file, process_dir, &x);
  وإلا
    {
      /* FIXME: it's a little gross that this initialization is
         required by copy.c::copy. */
      hash_init ();

      𐡄𐡍 (!target_directory)
        {
          𐡄𐡍 (! (mkdir_and_install
                 ? install_file_in_file_parents (file[0], file[1], &x)
                 : install_file_in_file (file[0], file[1], AT_FDCWD,
                                         file[1], &x)))
            exit_status = EXIT_FAILURE;
        }
      وإلا
        {
          dest_info_init (&x);
          لكل (صحيح i = 0; i < n_files; i++)
            𐡄𐡍 (! install_file_in_dir (file[i], target_directory, &x,
                                       i == 0 && mkdir_and_install,
                                       &target_dirfd))
              exit_status = EXIT_FAILURE;
        }
    }

  main_exit (exit_status);
}
