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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <getopt.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <signal.h>
#அந்தர்பாவ <pwd.h>
#அந்தர்பாவ <grp.h>
#அந்தர்பாவ <selinux/label.h>
#அந்தர்பாவ <spawn.h>
#அந்தர்பாவ <sys/wait.h>

#அந்தர்பாவ "system.h"
#அந்தர்பாவ "backupfile.h"
#அந்தர்பாவ "cp-hash.h"
#அந்தர்பாவ "copy.h"
#அந்தர்பாவ "filenamecat.h"
#அந்தர்பாவ "full-read.h"
#அந்தர்பாவ "mkancesdirs.h"
#அந்தர்பாவ "mkdir-p.h"
#அந்தர்பாவ "modechange.h"
#அந்தர்பாவ "prog-fprintf.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "savewd.h"
#அந்தர்பாவ "selinux.h"
#அந்தர்பாவ "stat-time.h"
#அந்தர்பாவ "targetdir.h"
#அந்தர்பாவ "utimens.h"
#அந்தர்பாவ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "install"

#define AUTHORS proper_name ("David MacKenzie")

நிலையான முழு selinux_enabled = 0;
நிலையான bool use_default_selinux_context = true;

#எனில் ! HAVE_ENDGRENT
# define endgrent() ((வெறுமை) 0)
#endif

#எனில் ! HAVE_ENDPWENT
# define endpwent() ((வெறுமை) 0)
#endif

/* The user name that will own the files, or NULL to make the owner
   the current user ID. */
நிலையான எழுத்து *owner_name;

/* The user ID corresponding to 'owner_name'. */
நிலையான uid_t owner_id;

/* The group name that will own the files, or NULL to make the group
   the current group ID. */
நிலையான எழுத்து *group_name;

/* The group ID corresponding to 'group_name'. */
நிலையான gid_t group_id;

#define DEFAULT_MODE (S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)

/* The file mode bits to which non-directory files will be set.  The umask has
   no effect. */
நிலையான mode_t mode = DEFAULT_MODE;

/* Similar, but for directories.  */
நிலையான mode_t dir_mode = DEFAULT_MODE;

/* The file mode bits that the user cares about.  This should be a
   superset of DIR_MODE and a subset of CHMOD_MODE_BITS.  This matters
   for directories, since otherwise directories may keep their S_ISUID
   or S_ISGID bits.  */
நிலையான mode_t dir_mode_bits = CHMOD_MODE_BITS;

/* Compare files before installing (-C) */
நிலையான bool copy_only_if_needed;

/* If true, strip executable files after copying them. */
நிலையான bool strip_files;

/* If true, install a directory instead of a regular file. */
நிலையான bool dir_arg;

/* Program used to strip binaries, "strip" is default */
நிலையான எழுத்து மாறிலி *strip_program = "strip";

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
எண்ணிக்கை
{
  DEBUG_OPTION = CHAR_MAX + 1,
  PRESERVE_CONTEXT_OPTION,
  STRIP_PROGRAM_OPTION
};

நிலையான கட்டமைப்பு option மாறிலி long_options[] =
{
  {"backup", optional_argument, வெற்று, 'b'},
  {"compare", no_argument, வெற்று, 'C'},
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {"debug", no_argument, வெற்று, DEBUG_OPTION},
  {"directory", no_argument, வெற்று, 'd'},
  {"group", required_argument, வெற்று, 'g'},
  {"mode", required_argument, வெற்று, 'm'},
  {"no-target-directory", no_argument, வெற்று, 'T'},
  {"owner", required_argument, வெற்று, 'o'},
  {"preserve-timestamps", no_argument, வெற்று, 'p'},
  {"preserve-context", no_argument, வெற்று, PRESERVE_CONTEXT_OPTION},
  {"strip", no_argument, வெற்று, 's'},
  {"strip-program", required_argument, வெற்று, STRIP_PROGRAM_OPTION},
  {"suffix", required_argument, வெற்று, 'S'},
  {"target-directory", required_argument, வெற்று, 't'},
  {"verbose", no_argument, வெற்று, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {வெற்று, 0, வெற்று, 0}
};

/* Compare content of opened files using file descriptors A_FD and B_FD. Return
   true if files are equal. */
நிலையான bool
have_same_content (முழு a_fd, முழு b_fd)
{
  எண்ணிக்கை { CMP_BLOCK_SIZE = 4096 };
  நிலையான எழுத்து a_buff[CMP_BLOCK_SIZE];
  நிலையான எழுத்து b_buff[CMP_BLOCK_SIZE];

  idx_t size;
  போது (0 < (size = full_read (a_fd, a_buff, அளவு a_buff))) {
    எனில் (size != full_read (b_fd, b_buff, அளவு b_buff))
      திரும்பு false;

    எனில் (!memeq (a_buff, b_buff, size))
      திரும்பு false;
  }

  திரும்பு size == 0;
}

/* Return true for mode with non-permission bits. */
நிலையான bool
extra_mode (mode_t உள்ளீடு)
{
  mode_t mask = S_IRWXUGO | S_IFMT;
  திரும்பு !! (உள்ளீடு & ~ mask);
}

/* Return true if copy of file SRC_NAME to file DEST_NAME aka
   DEST_DIRFD+DEST_RELNAME is necessary. */
நிலையான bool
need_copy (எழுத்து மாறிலி *src_name, எழுத்து மாறிலி *dest_name,
           முழு dest_dirfd, எழுத்து மாறிலி *dest_relname,
           மாறிலி கட்டமைப்பு cp_options *x)
{
  எனில் (extra_mode (mode))
    திரும்பு true;

  /* compare files using stat */
  கட்டமைப்பு stat src_sb;
  எனில் (stat (src_name, &src_sb) != 0)
    திரும்பு true;

  கட்டமைப்பு stat dest_sb;
  எனில் (fstatat (dest_dirfd, dest_relname, &dest_sb, AT_SYMLINK_NOFOLLOW) != 0)
    திரும்பு true;

  எனில் (!S_ISREG (src_sb.st_mode) || !S_ISREG (dest_sb.st_mode)
      || extra_mode (src_sb.st_mode) || extra_mode (dest_sb.st_mode))
    திரும்பு true;

  எனில் (src_sb.st_size != dest_sb.st_size
      || (dest_sb.st_mode & CHMOD_MODE_BITS) != mode)
    திரும்பு true;

  எனில் (owner_id == (uid_t) -1)
    {
      errno = 0;
      uid_t ruid = getuid ();
      எனில் ((ruid == (uid_t) -1 && errno) || dest_sb.st_uid != ruid)
        திரும்பு true;
    }
  இல்லையெனில் எனில் (dest_sb.st_uid != owner_id)
    திரும்பு true;

  எனில் (group_id == (uid_t) -1)
    {
      errno = 0;
      gid_t rgid = getgid ();
      எனில் ((rgid == (uid_t) -1 && errno) || dest_sb.st_gid != rgid)
        திரும்பு true;
    }
  இல்லையெனில் எனில் (dest_sb.st_gid != group_id)
    திரும்பு true;

  /* compare SELinux context if preserving */
  எனில் (selinux_enabled && x->preserve_security_context)
    {
      எழுத்து *file_scontext_raw = வெற்று;
      எனில் (getfilecon_raw (src_name, &file_scontext_raw) == -1)
        திரும்பு true;

      எழுத்து *to_scontext_raw = வெற்று;
      எனில் (getfilecon_raw (dest_name, &to_scontext_raw) == -1)
        {
          freecon (file_scontext_raw);
          திரும்பு true;
        }

      bool scontext_match = streq (file_scontext_raw, to_scontext_raw);

      freecon (file_scontext_raw);
      freecon (to_scontext_raw);
      எனில் (!scontext_match)
        திரும்பு true;
    }

  /* compare files content */
  முழு src_fd = open (src_name, O_RDONLY | O_இருமம்);
  எனில் (src_fd < 0)
    திரும்பு true;

  முழு dest_fd = openat (dest_dirfd, dest_relname, O_RDONLY | O_இருமம்);
  எனில் (dest_fd < 0)
    {
      close (src_fd);
      திரும்பு true;
    }

  bool content_match = have_same_content (src_fd, dest_fd);

  close (src_fd);
  close (dest_fd);
  திரும்பு !content_match;
}

நிலையான வெறுமை
cp_option_init (கட்டமைப்பு cp_options *x)
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
  x->set_security_context = வெற்று; /* Whether to set sys default context.  */
  x->preserve_xattr = false;
  x->verbose = false;
  x->dest_info = வெற்று;
  x->src_info = வெற்று;
}

நிலையான கட்டமைப்பு selabel_handle *
get_labeling_handle (வெறுமை)
{
  நிலையான bool initialized;
  நிலையான கட்டமைப்பு selabel_handle *hnd;
  எனில் (!initialized)
    {
      initialized = true;
      hnd = selabel_open (SELABEL_CTX_FILE, வெற்று, 0);
      எனில் (!hnd)
        error (0, errno, _("warning: security labeling handle failed"));
    }
  திரும்பு hnd;
}

/* Modify file context to match the specified policy.
   If an error occurs the file will remain with the default directory
   context.  Note this sets the context to that returned by selabel_lookup
   and thus discards MLS levels and user identity of the FILE.  */
நிலையான வெறுமை
setdefaultfilecon (எழுத்து மாறிலி *file)
{
  எனில் (selinux_enabled != 1)
    {
      /* Indicate no context found. */
      திரும்பு;
    }

  கட்டமைப்பு stat st;
  எனில் (lstat (file, &st) != 0)
    திரும்பு;

  கட்டமைப்பு selabel_handle *hnd = get_labeling_handle ();
  எனில் (!hnd)
    திரும்பு;

  எழுத்து *scontext_raw = வெற்று;
  எனில் (selabel_lookup_raw (hnd, &scontext_raw, file, st.st_mode) != 0)
    {
      எனில் (errno != ENOENT && ! ignorable_ctx_err (errno))
        error (0, errno, _("warning: %s: context lookup failed"),
               quotef (file));
      திரும்பு;
    }

  எனில் (lsetfilecon_raw (file, scontext_raw) < 0 && errno != ENOTSUP)
    error (0, errno,
           _("warning: %s: failed to change context to %s"),
           quotef_n (0, file), quote_n (1, scontext_raw));

  freecon (scontext_raw);
}

/* Report that directory DIR was made, if OPTIONS requests this.  */
நிலையான வெறுமை
announce_mkdir (எழுத்து மாறிலி *dir, வெறுமை *options)
{
  கட்டமைப்பு cp_options மாறிலி *x = options;
  எனில் (x->verbose)
    prog_fprintf (stdout, _("creating directory %s"), quoteaf (dir));
}

/* Make ancestor directory DIR, whose last file name component is
   COMPONENT, with options OPTIONS.  Assume the working directory is
   COMPONENT's parent.  */
நிலையான முழு
make_ancestor (எழுத்து மாறிலி *dir, எழுத்து மாறிலி *component, வெறுமை *options)
{
  கட்டமைப்பு cp_options மாறிலி *x = options;
  எனில் (x->set_security_context
      && defaultcon (x->set_security_context, component, S_IFDIR) < 0
      && ! ignorable_ctx_err (errno))
    error (0, errno, _("failed to set default creation context for %s"),
           quoteaf (dir));

  முழு r = mkdir (component, DEFAULT_MODE);
  எனில் (r == 0)
    announce_mkdir (dir, options);
  திரும்பு r;
}

/* Process a command-line file name, for the -d option.  */
நிலையான முழு
process_dir (எழுத்து *dir, கட்டமைப்பு savewd *wd, வெறுமை *options)
{
  கட்டமைப்பு cp_options மாறிலி *x = options;

  முழு ret = (make_dir_parents (dir, wd, make_ancestor, options,
                               dir_mode, announce_mkdir,
                               dir_mode_bits, owner_id, group_id, false)
          ? EXIT_SUCCESS
          : EXIT_FAILURE);

  /* FIXME: Due to the current structure of make_dir_parents()
     we don't have the facility to call defaultcon() before the
     final component of DIR is created.  So for now, create the
     final component with the context from previous component
     and here we set the context for the final component. */
  எனில் (ret == EXIT_SUCCESS && x->set_security_context)
    {
      எனில் (! restorecon (x->set_security_context, last_component (dir), false)
          && ! ignorable_ctx_err (errno))
        error (0, errno, _("failed to restore context for %s"),
               quoteaf (dir));
    }

  திரும்பு ret;
}

எண்ணிக்கை copy_status { COPY_FAILED, COPY_OK, COPY_SKIPPED };

/* Copy file FROM onto file TO aka TO_DIRFD+TO_RELNAME, creating TO if
   necessary.  Return COPY_OK if successful or COPY_SKIPPED if the copy
   was skipped.  */

நிலையான எண்ணிக்கை copy_status
copy_file (எழுத்து மாறிலி *from, எழுத்து மாறிலி *to,
           முழு to_dirfd, எழுத்து மாறிலி *to_relname, மாறிலி கட்டமைப்பு cp_options *x)
{
  எனில் (copy_only_if_needed && !need_copy (from, to, to_dirfd, to_relname, x))
    திரும்பு COPY_SKIPPED;

  /* Allow installing from non-regular files like /dev/null.
     Charles Karney reported that some Sun version of install allows that
     and that sendmail's installation process relies on the behavior.
     However, since !x->recursive, the call to "copy" will fail if FROM
     is a directory.  */

  bool copy_into_self;
  திரும்பு (copy (from, to, to_dirfd, to_relname, 0, x, &copy_into_self, வெற்று)
          ? COPY_OK : COPY_FAILED);
}

/* Set the attributes of file or directory NAME aka DIRFD+RELNAME.
   Return true if successful.  */

நிலையான bool
change_attributes (எழுத்து மாறிலி *name, முழு dirfd, எழுத்து மாறிலி *relname)
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

  எனில் (! (owner_id == (uid_t) -1 && group_id == (gid_t) -1)
      && lchownat (dirfd, relname, owner_id, group_id) != 0)
    error (0, errno, _("cannot change ownership of %s"), quoteaf (name));
  இல்லையெனில் எனில் (chmodat (dirfd, relname, mode) != 0)
    error (0, errno, _("cannot change permissions of %s"), quoteaf (name));
  இல்லையெனில்
    ok = true;

  எனில் (use_default_selinux_context)
    setdefaultfilecon (name);

  திரும்பு ok;
}

/* Set the timestamps of file DEST aka DIRFD+RELNAME to match those of SRC_SB.
   Return true if successful.  */

நிலையான bool
change_timestamps (கட்டமைப்பு stat மாறிலி *src_sb, எழுத்து மாறிலி *dest,
                   முழு dirfd, எழுத்து மாறிலி *relname)
{
  கட்டமைப்பு timespec timespec[2] = { get_stat_atime (src_sb),
                                  get_stat_mtime (src_sb) };

  எனில் (utimensat (dirfd, relname, timespec, 0))
    {
      error (0, errno, _("cannot set timestamps for %s"), quoteaf (dest));
      திரும்பு false;
    }
  திரும்பு true;
}

/* Strip the symbol table from the file NAME.
   We could dig the magic number out of the file first to
   determine whether to strip it, but the header files and
   magic numbers vary so much from system to system that making
   it portable would be very difficult.  Not worth the effort. */

நிலையான bool
strip (எழுத்து மாறிலி *name)
{
  posix_spawnattr_t attr;
  posix_spawnattr_t *attrp = வெற்று;

  /* Try to use vfork for systems where it matters.  */
  எனில் (posix_spawnattr_init (&attr) == 0)
    {
      எனில் (posix_spawnattr_setflags (&attr, POSIX_SPAWN_USEVFORK) == 0)
        attrp = &attr;
      இல்லையெனில்
        posix_spawnattr_destroy (&attr);
    }

  /* Construct the arguments to 'strip'.  */
  எழுத்து *concat_name = வெற்று;
  எழுத்து மாறிலி *safe_name = name;
  எனில் (name && *name == '-')
    safe_name = concat_name = file_name_concat (".", name, வெற்று);
  எழுத்து மாறிலி *மாறிலி argv[] = { strip_program, safe_name, வெற்று };

  /* Run 'strip'.  */
  pid_t pid;
  முழு result = posix_spawnp (&pid, strip_program, வெற்று, attrp,
                             (எழுத்து * மாறிலி *) argv, environ);

  bool ok = false;
  எனில் (result != 0)
    {
      error (0, result,
             streq (strip_program, "strip")
               ? _("cannot run %s")
               : _("cannot run strip program %s"),
             quoteaf (strip_program));
    }
  இல்லையெனில்
    {
      /* Wait for 'strip' to complete, and emit a warning message on failure  */
      முழு status;
      எனில் (waitpid (pid, &status, 0) < 0)
        error (0, errno, _("waiting for strip"));
      இல்லையெனில் எனில் (! WIFEXITED (status) || WEXITSTATUS (status))
        error (0, 0, _("strip process terminated abnormally"));
      இல்லையெனில்
        ok = true;
    }

  விடுவி (concat_name);
  எனில் (attrp)
    posix_spawnattr_destroy (attrp);

  திரும்பு ok;
}

/* Initialize the user and group ownership of the files to install. */

நிலையான வெறுமை
get_ids (வெறுமை)
{
  எனில் (owner_name)
    {
      கட்டமைப்பு passwd *pw = getpwnam (owner_name);
      எனில் (pw == வெற்று)
        {
          uintmax_t tmp;
          எனில் (xstrtoumax (owner_name, வெற்று, 0, &tmp, "") != LONGINT_OK
              || ckd_add (&owner_id, tmp, 0))
            error (EXIT_FAILURE, 0, _("invalid user %s"),
                   quoteaf (owner_name));
        }
      இல்லையெனில்
        owner_id = pw->pw_uid;
      endpwent ();
    }
  இல்லையெனில்
    owner_id = (uid_t) -1;

  எனில் (group_name)
    {
      கட்டமைப்பு group *gr = getgrnam (group_name);
      எனில் (gr == வெற்று)
        {
          uintmax_t tmp;
          எனில் (xstrtoumax (group_name, வெற்று, 0, &tmp, "") != LONGINT_OK
              || ckd_add (&group_id, tmp, 0))
            error (EXIT_FAILURE, 0, _("invalid group %s"),
                   quoteaf (group_name));
        }
      இல்லையெனில்
        group_id = gr->gr_gid;
      endgrent ();
    }
  இல்லையெனில்
    group_id = (gid_t) -1;
}

வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
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
the existing DIRECTORY, போது setting permission modes and owner/group.\n\
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
         and எனில் no change to content, ownership, and permissions,\n\
         செய் not modify the destination at all\n\
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
         strip குறியீடு tables\n\
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
         and each created directory, to இயல்பு type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or எனில் CTX is specified then set the\n\
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

நிலையான bool
install_file_in_file (எழுத்து மாறிலி *from, எழுத்து மாறிலி *to,
                      முழு to_dirfd, எழுத்து மாறிலி *to_relname,
                      மாறிலி கட்டமைப்பு cp_options *x)
{
  கட்டமைப்பு stat from_sb;
  எனில் (x->preserve_timestamps && stat (from, &from_sb) != 0)
    {
      error (0, errno, _("cannot stat %s"), quoteaf (from));
      திரும்பு false;
    }
  எண்ணிக்கை copy_status copy_status = copy_file (from, to, to_dirfd, to_relname, x);
  எனில் (copy_status == COPY_FAILED)
    திரும்பு false;
  எனில் (copy_status == COPY_OK && strip_files && ! strip (to))
    {
      எனில் (unlinkat (to_dirfd, to_relname, 0) != 0)  /* Cleanup.  */
        error (EXIT_FAILURE, errno, _("cannot unlink %s"), quoteaf (to));
      திரும்பு false;
    }
  எனில் (x->preserve_timestamps && (copy_status == COPY_SKIPPED || strip_files
                                 || ! S_ISREG (from_sb.st_mode))
      && ! change_timestamps (&from_sb, to, to_dirfd, to_relname))
    திரும்பு false;

  திரும்பு copy_status == COPY_SKIPPED
         || change_attributes (to, to_dirfd, to_relname);
}

/* Create any missing parent directories of TO,
   while maintaining the current Working Directory.
   Return true if successful.  */

நிலையான bool
mkancesdirs_safe_wd (எழுத்து மாறிலி *from, எழுத்து *to, கட்டமைப்பு cp_options *x,
                     bool save_always)
{
  bool save_working_directory =
    save_always
    || ! (IS_ABSOLUTE_FILE_NAME (from) && IS_ABSOLUTE_FILE_NAME (to));

  கட்டமைப்பு savewd wd;
  savewd_init (&wd);
  எனில் (! save_working_directory)
    savewd_finish (&wd);

  முழு status = EXIT_SUCCESS;
  எனில் (mkancesdirs (to, &wd, make_ancestor, x) == -1)
    {
      error (0, errno, _("cannot create directory %s"), quoteaf (to));
      status = EXIT_FAILURE;
    }

  எனில் (save_working_directory)
    {
      முழு restore_result = savewd_restore (&wd, status);
      முழு restore_errno = errno;
      savewd_finish (&wd);
      எனில் (EXIT_SUCCESS < restore_result)
        திரும்பு false;
      எனில் (restore_result < 0 && status == EXIT_SUCCESS)
        {
          error (0, restore_errno, _("cannot create directory %s"),
                 quoteaf (to));
          திரும்பு false;
        }
    }
  திரும்பு status == EXIT_SUCCESS;
}

/* Copy file FROM onto file TO, creating any missing parent directories of TO.
   Return true if successful.  */

நிலையான bool
install_file_in_file_parents (எழுத்து மாறிலி *from, எழுத்து *to,
                              மாறிலி கட்டமைப்பு cp_options *x)
{
  திரும்பு (mkancesdirs_safe_wd (from, to, (கட்டமைப்பு cp_options *)x, false)
          && install_file_in_file (from, to, AT_FDCWD, to, x));
}

/* Copy file FROM into directory TO_DIR, keeping its same name,
   and give the copy the appropriate attributes.
   Return true if successful.  */

நிலையான bool
install_file_in_dir (எழுத்து மாறிலி *from, எழுத்து மாறிலி *to_dir,
                     மாறிலி கட்டமைப்பு cp_options *x, bool mkdir_and_install,
                     முழு *target_dirfd)
{
  எழுத்து மாறிலி *from_base = last_component (from);
  எழுத்து *to_relname;
  எழுத்து *to = file_name_concat (to_dir, from_base, &to_relname);
  bool ret = true;

  எனில் (!target_dirfd_valid (*target_dirfd)
      && (ret = mkdir_and_install)
      && (ret = mkancesdirs_safe_wd (from, to, (கட்டமைப்பு cp_options *) x, true)))
    {
      முழு fd = open (to_dir, O_PATHSEARCH | O_DIRECTORY);
      எனில் (fd < 0)
        {
          error (0, errno, _("cannot open %s"), quoteaf (to));
          ret = false;
        }
      இல்லையெனில்
        *target_dirfd = fd;
    }

  எனில் (ret)
    {
      முழு to_dirfd = *target_dirfd;
      எனில் (!target_dirfd_valid (to_dirfd))
        {
          to_dirfd = AT_FDCWD;
          to_relname = to;
        }
      ret = install_file_in_file (from, to, to_dirfd, to_relname, x);
    }

  விடுவி (to);
  திரும்பு ret;
}

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  எழுத்து மாறிலி *specified_mode = வெற்று;
  bool make_backups = false;
  எழுத்து மாறிலி *backup_suffix = வெற்று;
  எழுத்து *version_control_string = வெற்று;
  bool mkdir_and_install = false;
  கட்டமைப்பு cp_options x;
  எழுத்து மாறிலி *target_directory = வெற்று;
  bool no_target_directory = false;
  bool strip_program_specified = false;
  எழுத்து மாறிலி *scontext = வெற்று;
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

  முழு optc;
  போது ((optc = getopt_long (argc, argv, "bcCsDdg:m:o:pt:TvS:Z", long_options,
                              வெற்று))
         != -1)
    {
      தேர்வு (optc)
        {
        நிலை 'b':
          make_backups = true;
          எனில் (optarg)
            version_control_string = optarg;
          நிறுத்து;
        நிலை 'c':
          நிறுத்து;
        நிலை 'C':
          copy_only_if_needed = true;
          நிறுத்து;
        நிலை 's':
          strip_files = true;
#ifdef SIGCHLD
          /* System V fork+wait does not work if SIGCHLD is ignored.  */
          சமிக்ஞை (SIGCHLD, SIG_DFL);
#endif
          நிறுத்து;
        நிலை DEBUG_OPTION:
          x.debug = x.verbose = true;
          நிறுத்து;
        நிலை STRIP_PROGRAM_OPTION:
          strip_program = xstrdup (optarg);
          strip_program_specified = true;
          நிறுத்து;
        நிலை 'd':
          dir_arg = true;
          நிறுத்து;
        நிலை 'D':
          mkdir_and_install = true;
          நிறுத்து;
        நிலை 'v':
          x.verbose = true;
          நிறுத்து;
        நிலை 'g':
          group_name = optarg;
          நிறுத்து;
        நிலை 'm':
          specified_mode = optarg;
          நிறுத்து;
        நிலை 'o':
          owner_name = optarg;
          நிறுத்து;
        நிலை 'p':
          x.preserve_timestamps = true;
          நிறுத்து;
        நிலை 'S':
          make_backups = true;
          backup_suffix = optarg;
          நிறுத்து;
        நிலை 't':
          எனில் (target_directory)
            error (EXIT_FAILURE, 0,
                   _("multiple target directories specified"));
          target_directory = optarg;
          நிறுத்து;
        நிலை 'T':
          no_target_directory = true;
          நிறுத்து;

        நிலை PRESERVE_CONTEXT_OPTION:
          எனில் (! selinux_enabled)
            {
              error (0, 0, _("WARNING: ignoring --preserve-context; "
                             "this kernel is not SELinux-enabled"));
              நிறுத்து;
            }
          x.preserve_security_context = true;
          use_default_selinux_context = false;
          நிறுத்து;
        நிலை 'Z':
          எனில் (selinux_enabled)
            {
              /* Disable use of the install(1) specific setdefaultfilecon().
                 Note setdefaultfilecon() is different from the newer and more
                 generic restorecon() in that the former sets the context of
                 the dest files to that returned by selabel_lookup directly,
                 thus discarding MLS level and user identity of the file.
                 TODO: consider removing setdefaultfilecon() in future.  */
              use_default_selinux_context = false;

              எனில் (optarg)
                scontext = optarg;
              இல்லையெனில்
                x.set_security_context = get_labeling_handle ();
            }
          இல்லையெனில் எனில் (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux-enabled kernel"));
            }
          நிறுத்து;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        இயல்பு:
          usage (EXIT_FAILURE);
        }
    }

  /* Check for invalid combinations of arguments. */
  எனில் (dir_arg && strip_files)
    error (EXIT_FAILURE, 0,
           _("the strip option may not be used when installing a directory"));
  எனில் (dir_arg && target_directory)
    error (EXIT_FAILURE, 0,
           _("target directory not allowed when installing a directory"));

  x.backup_type = (make_backups
                   ? xget_version (_("backup type"),
                                   version_control_string)
                   : no_backups);
  set_simple_backup_suffix (backup_suffix);

  எனில் (x.preserve_security_context && (x.set_security_context || scontext))
    error (EXIT_FAILURE, 0,
           _("cannot set target context and preserve it"));

  எனில் (scontext && setfscreatecon (scontext) < 0)
    error (EXIT_FAILURE, errno,
           _("failed to set default file creation context to %s"),
         quote (scontext));

  முழு n_files = argc - optind;
  எழுத்து **file = argv + optind;

  எனில் (n_files <= ! (dir_arg || target_directory))
    {
      எனில் (n_files <= 0)
        error (0, 0, _("missing file operand"));
      இல்லையெனில்
        error (0, 0, _("missing destination file operand after %s"),
               quoteaf (file[0]));
      usage (EXIT_FAILURE);
    }

  கட்டமைப்பு stat sb;
  முழு target_dirfd = AT_FDCWD;
  எனில் (no_target_directory)
    {
      எனில் (target_directory)
        error (EXIT_FAILURE, 0,
               _("cannot combine --target-directory (-t) "
                 "and --no-target-directory (-T)"));
      எனில் (2 < n_files)
        {
          error (0, 0, _("extra operand %s"), quoteaf (file[2]));
          usage (EXIT_FAILURE);
        }
    }
  இல்லையெனில் எனில் (target_directory)
    {
      target_dirfd = target_directory_operand (target_directory, &sb);
      எனில் (! (target_dirfd_valid (target_dirfd)
             || (mkdir_and_install && errno == ENOENT)))
        error (EXIT_FAILURE, errno, _("failed to access %s"),
               quoteaf (target_directory));
    }
  இல்லையெனில் எனில் (!dir_arg)
    {
      எழுத்து மாறிலி *lastfile = file[n_files - 1];
      முழு fd = target_directory_operand (lastfile, &sb);
      எனில் (target_dirfd_valid (fd))
        {
          target_dirfd = fd;
          target_directory = lastfile;
          n_files--;
        }
      இல்லையெனில் எனில் (2 < n_files)
        error (EXIT_FAILURE, errno, _("target %s"), quoteaf (lastfile));
    }

  எனில் (specified_mode)
    {
      கட்டமைப்பு mode_change *change = mode_compile (specified_mode);
      எனில் (!change)
        error (EXIT_FAILURE, 0, _("invalid mode %s"), quote (specified_mode));
      mode = mode_adjust (0, false, 0, change, வெற்று);
      dir_mode = mode_adjust (0, true, 0, change, &dir_mode_bits);
      விடுவி (change);
    }

  எனில் (strip_program_specified && !strip_files)
    error (0, 0, _("WARNING: ignoring --strip-program option as -s option was "
                   "not specified"));

  எனில் (copy_only_if_needed && strip_files)
    {
      error (0, 0, _("options --compare (-C) and --strip are mutually "
                     "exclusive"));
      usage (EXIT_FAILURE);
    }

  எனில் (copy_only_if_needed && extra_mode (mode))
    error (0, 0, _("the --compare (-C) option is ignored when you"
                   " specify a mode with non-permission bits"));

  get_ids ();

  முழு exit_status = EXIT_SUCCESS;
  எனில் (dir_arg)
    exit_status = savewd_process_files (n_files, file, process_dir, &x);
  இல்லையெனில்
    {
      /* FIXME: it's a little gross that this initialization is
         required by copy.c::copy. */
      hash_init ();

      எனில் (!target_directory)
        {
          எனில் (! (mkdir_and_install
                 ? install_file_in_file_parents (file[0], file[1], &x)
                 : install_file_in_file (file[0], file[1], AT_FDCWD,
                                         file[1], &x)))
            exit_status = EXIT_FAILURE;
        }
      இல்லையெனில்
        {
          dest_info_init (&x);
          க்கு (முழு i = 0; i < n_files; i++)
            எனில் (! install_file_in_dir (file[i], target_directory, &x,
                                       i == 0 && mkdir_and_install,
                                       &target_dirfd))
              exit_status = EXIT_FAILURE;
        }
    }

  main_exit (exit_status);
}
