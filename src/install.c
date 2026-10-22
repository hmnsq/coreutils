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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <getopt.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <signal.h>
#ଅନ୍ତର୍ଭାଵ <pwd.h>
#ଅନ୍ତର୍ଭାଵ <grp.h>
#ଅନ୍ତର୍ଭାଵ <selinux/label.h>
#ଅନ୍ତର୍ଭାଵ <spawn.h>
#ଅନ୍ତର୍ଭାଵ <sys/wait.h>

#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "backupfile.h"
#ଅନ୍ତର୍ଭାଵ "cp-hash.h"
#ଅନ୍ତର୍ଭାଵ "copy.h"
#ଅନ୍ତର୍ଭାଵ "filenamecat.h"
#ଅନ୍ତର୍ଭାଵ "full-read.h"
#ଅନ୍ତର୍ଭାଵ "mkancesdirs.h"
#ଅନ୍ତର୍ଭାଵ "mkdir-p.h"
#ଅନ୍ତର୍ଭାଵ "modechange.h"
#ଅନ୍ତର୍ଭାଵ "prog-fprintf.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "savewd.h"
#ଅନ୍ତର୍ଭାଵ "selinux.h"
#ଅନ୍ତର୍ଭାଵ "stat-time.h"
#ଅନ୍ତର୍ଭାଵ "targetdir.h"
#ଅନ୍ତର୍ଭାଵ "utimens.h"
#ଅନ୍ତର୍ଭାଵ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "install"

#define AUTHORS proper_name ("David MacKenzie")

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା selinux_enabled = 0;
ସ୍ଥିର bool use_default_selinux_context = true;

#ଯଦି ! HAVE_ENDGRENT
# define endgrent() ((ଶୂନ୍ୟ) 0)
#endif

#ଯଦି ! HAVE_ENDPWENT
# define endpwent() ((ଶୂନ୍ୟ) 0)
#endif

/* The user name that will own the files, or NULL to make the owner
   the current user ID. */
ସ୍ଥିର ଅକ୍ଷର *owner_name;

/* The user ID corresponding to 'owner_name'. */
ସ୍ଥିର uid_t owner_id;

/* The group name that will own the files, or NULL to make the group
   the current group ID. */
ସ୍ଥିର ଅକ୍ଷର *group_name;

/* The group ID corresponding to 'group_name'. */
ସ୍ଥିର gid_t group_id;

#define DEFAULT_MODE (S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)

/* The file mode bits to which non-directory files will be set.  The umask has
   no effect. */
ସ୍ଥିର mode_t mode = DEFAULT_MODE;

/* Similar, but for directories.  */
ସ୍ଥିର mode_t dir_mode = DEFAULT_MODE;

/* The file mode bits that the user cares about.  This should be a
   superset of DIR_MODE and a subset of CHMOD_MODE_BITS.  This matters
   for directories, since otherwise directories may keep their S_ISUID
   or S_ISGID bits.  */
ସ୍ଥିର mode_t dir_mode_bits = CHMOD_MODE_BITS;

/* Compare files before installing (-C) */
ସ୍ଥିର bool copy_only_if_needed;

/* If true, strip executable files after copying them. */
ସ୍ଥିର bool strip_files;

/* If true, install a directory instead of a regular file. */
ସ୍ଥିର bool dir_arg;

/* Program used to strip binaries, "strip" is default */
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *strip_program = "strip";

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ଗଣନା
{
  DEBUG_OPTION = CHAR_MAX + 1,
  PRESERVE_CONTEXT_OPTION,
  STRIP_PROGRAM_OPTION
};

ସ୍ଥିର ସଂରଚନା option ଅଚଳ long_options[] =
{
  {"backup", optional_argument, ଖାଲି, 'b'},
  {"compare", no_argument, ଖାଲି, 'C'},
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {"debug", no_argument, ଖାଲି, DEBUG_OPTION},
  {"directory", no_argument, ଖାଲି, 'd'},
  {"group", required_argument, ଖାଲି, 'g'},
  {"mode", required_argument, ଖାଲି, 'm'},
  {"no-target-directory", no_argument, ଖାଲି, 'T'},
  {"owner", required_argument, ଖାଲି, 'o'},
  {"preserve-timestamps", no_argument, ଖାଲି, 'p'},
  {"preserve-context", no_argument, ଖାଲି, PRESERVE_CONTEXT_OPTION},
  {"strip", no_argument, ଖାଲି, 's'},
  {"strip-program", required_argument, ଖାଲି, STRIP_PROGRAM_OPTION},
  {"suffix", required_argument, ଖାଲି, 'S'},
  {"target-directory", required_argument, ଖାଲି, 't'},
  {"verbose", no_argument, ଖାଲି, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ଖାଲି, 0, ଖାଲି, 0}
};

/* Compare content of opened files using file descriptors A_FD and B_FD. Return
   true if files are equal. */
ସ୍ଥିର bool
have_same_content (ପୂର୍ଣ୍ଣସଂଖ୍ୟା a_fd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା b_fd)
{
  ଗଣନା { CMP_BLOCK_SIZE = 4096 };
  ସ୍ଥିର ଅକ୍ଷର a_buff[CMP_BLOCK_SIZE];
  ସ୍ଥିର ଅକ୍ଷର b_buff[CMP_BLOCK_SIZE];

  idx_t size;
  ଯେତେବେଳେ (0 < (size = full_read (a_fd, a_buff, ଆକାର a_buff))) {
    ଯଦି (size != full_read (b_fd, b_buff, ଆକାର b_buff))
      ଫେରନ୍ତୁ false;

    ଯଦି (!memeq (a_buff, b_buff, size))
      ଫେରନ୍ତୁ false;
  }

  ଫେରନ୍ତୁ size == 0;
}

/* Return true for mode with non-permission bits. */
ସ୍ଥିର bool
extra_mode (mode_t ନିବେଶ)
{
  mode_t mask = S_IRWXUGO | S_IFMT;
  ଫେରନ୍ତୁ !! (ନିବେଶ & ~ mask);
}

/* Return true if copy of file SRC_NAME to file DEST_NAME aka
   DEST_DIRFD+DEST_RELNAME is necessary. */
ସ୍ଥିର bool
need_copy (ଅକ୍ଷର ଅଚଳ *src_name, ଅକ୍ଷର ଅଚଳ *dest_name,
           ପୂର୍ଣ୍ଣସଂଖ୍ୟା dest_dirfd, ଅକ୍ଷର ଅଚଳ *dest_relname,
           ଅଚଳ ସଂରଚନା cp_options *x)
{
  ଯଦି (extra_mode (mode))
    ଫେରନ୍ତୁ true;

  /* compare files using stat */
  ସଂରଚନା stat src_sb;
  ଯଦି (stat (src_name, &src_sb) != 0)
    ଫେରନ୍ତୁ true;

  ସଂରଚନା stat dest_sb;
  ଯଦି (fstatat (dest_dirfd, dest_relname, &dest_sb, AT_SYMLINK_NOFOLLOW) != 0)
    ଫେରନ୍ତୁ true;

  ଯଦି (!S_ISREG (src_sb.st_mode) || !S_ISREG (dest_sb.st_mode)
      || extra_mode (src_sb.st_mode) || extra_mode (dest_sb.st_mode))
    ଫେରନ୍ତୁ true;

  ଯଦି (src_sb.st_size != dest_sb.st_size
      || (dest_sb.st_mode & CHMOD_MODE_BITS) != mode)
    ଫେରନ୍ତୁ true;

  ଯଦି (owner_id == (uid_t) -1)
    {
      errno = 0;
      uid_t ruid = getuid ();
      ଯଦି ((ruid == (uid_t) -1 && errno) || dest_sb.st_uid != ruid)
        ଫେରନ୍ତୁ true;
    }
  ଅନ୍ୟଥା ଯଦି (dest_sb.st_uid != owner_id)
    ଫେରନ୍ତୁ true;

  ଯଦି (group_id == (uid_t) -1)
    {
      errno = 0;
      gid_t rgid = getgid ();
      ଯଦି ((rgid == (uid_t) -1 && errno) || dest_sb.st_gid != rgid)
        ଫେରନ୍ତୁ true;
    }
  ଅନ୍ୟଥା ଯଦି (dest_sb.st_gid != group_id)
    ଫେରନ୍ତୁ true;

  /* compare SELinux context if preserving */
  ଯଦି (selinux_enabled && x->preserve_security_context)
    {
      ଅକ୍ଷର *file_scontext_raw = ଖାଲି;
      ଯଦି (getfilecon_raw (src_name, &file_scontext_raw) == -1)
        ଫେରନ୍ତୁ true;

      ଅକ୍ଷର *to_scontext_raw = ଖାଲି;
      ଯଦି (getfilecon_raw (dest_name, &to_scontext_raw) == -1)
        {
          freecon (file_scontext_raw);
          ଫେରନ୍ତୁ true;
        }

      bool scontext_match = streq (file_scontext_raw, to_scontext_raw);

      freecon (file_scontext_raw);
      freecon (to_scontext_raw);
      ଯଦି (!scontext_match)
        ଫେରନ୍ତୁ true;
    }

  /* compare files content */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା src_fd = open (src_name, O_RDONLY | O_ଦ୍ୱିମିକ);
  ଯଦି (src_fd < 0)
    ଫେରନ୍ତୁ true;

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା dest_fd = openat (dest_dirfd, dest_relname, O_RDONLY | O_ଦ୍ୱିମିକ);
  ଯଦି (dest_fd < 0)
    {
      close (src_fd);
      ଫେରନ୍ତୁ true;
    }

  bool content_match = have_same_content (src_fd, dest_fd);

  close (src_fd);
  close (dest_fd);
  ଫେରନ୍ତୁ !content_match;
}

ସ୍ଥିର ଶୂନ୍ୟ
cp_option_init (ସଂରଚନା cp_options *x)
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
  x->set_security_context = ଖାଲି; /* Whether to set sys default context.  */
  x->preserve_xattr = false;
  x->verbose = false;
  x->dest_info = ଖାଲି;
  x->src_info = ଖାଲି;
}

ସ୍ଥିର ସଂରଚନା selabel_handle *
get_labeling_handle (ଶୂନ୍ୟ)
{
  ସ୍ଥିର bool initialized;
  ସ୍ଥିର ସଂରଚନା selabel_handle *hnd;
  ଯଦି (!initialized)
    {
      initialized = true;
      hnd = selabel_open (SELABEL_CTX_FILE, ଖାଲି, 0);
      ଯଦି (!hnd)
        error (0, errno, _("warning: security labeling handle failed"));
    }
  ଫେରନ୍ତୁ hnd;
}

/* Modify file context to match the specified policy.
   If an error occurs the file will remain with the default directory
   context.  Note this sets the context to that returned by selabel_lookup
   and thus discards MLS levels and user identity of the FILE.  */
ସ୍ଥିର ଶୂନ୍ୟ
setdefaultfilecon (ଅକ୍ଷର ଅଚଳ *file)
{
  ଯଦି (selinux_enabled != 1)
    {
      /* Indicate no context found. */
      ଫେରନ୍ତୁ;
    }

  ସଂରଚନା stat st;
  ଯଦି (lstat (file, &st) != 0)
    ଫେରନ୍ତୁ;

  ସଂରଚନା selabel_handle *hnd = get_labeling_handle ();
  ଯଦି (!hnd)
    ଫେରନ୍ତୁ;

  ଅକ୍ଷର *scontext_raw = ଖାଲି;
  ଯଦି (selabel_lookup_raw (hnd, &scontext_raw, file, st.st_mode) != 0)
    {
      ଯଦି (errno != ENOENT && ! ignorable_ctx_err (errno))
        error (0, errno, _("warning: %s: context lookup failed"),
               quotef (file));
      ଫେରନ୍ତୁ;
    }

  ଯଦି (lsetfilecon_raw (file, scontext_raw) < 0 && errno != ENOTSUP)
    error (0, errno,
           _("warning: %s: failed to change context to %s"),
           quotef_n (0, file), quote_n (1, scontext_raw));

  freecon (scontext_raw);
}

/* Report that directory DIR was made, if OPTIONS requests this.  */
ସ୍ଥିର ଶୂନ୍ୟ
announce_mkdir (ଅକ୍ଷର ଅଚଳ *dir, ଶୂନ୍ୟ *options)
{
  ସଂରଚନା cp_options ଅଚଳ *x = options;
  ଯଦି (x->verbose)
    prog_fprintf (stdout, _("creating directory %s"), quoteaf (dir));
}

/* Make ancestor directory DIR, whose last file name component is
   COMPONENT, with options OPTIONS.  Assume the working directory is
   COMPONENT's parent.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
make_ancestor (ଅକ୍ଷର ଅଚଳ *dir, ଅକ୍ଷର ଅଚଳ *component, ଶୂନ୍ୟ *options)
{
  ସଂରଚନା cp_options ଅଚଳ *x = options;
  ଯଦି (x->set_security_context
      && defaultcon (x->set_security_context, component, S_IFDIR) < 0
      && ! ignorable_ctx_err (errno))
    error (0, errno, _("failed to set default creation context for %s"),
           quoteaf (dir));

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା r = mkdir (component, DEFAULT_MODE);
  ଯଦି (r == 0)
    announce_mkdir (dir, options);
  ଫେରନ୍ତୁ r;
}

/* Process a command-line file name, for the -d option.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
process_dir (ଅକ୍ଷର *dir, ସଂରଚନା savewd *wd, ଶୂନ୍ୟ *options)
{
  ସଂରଚନା cp_options ଅଚଳ *x = options;

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = (make_dir_parents (dir, wd, make_ancestor, options,
                               dir_mode, announce_mkdir,
                               dir_mode_bits, owner_id, group_id, false)
          ? EXIT_SUCCESS
          : EXIT_FAILURE);

  /* FIXME: Due to the current structure of make_dir_parents()
     we don't have the facility to call defaultcon() before the
     final component of DIR is created.  So for now, create the
     final component with the context from previous component
     and here we set the context for the final component. */
  ଯଦି (ret == EXIT_SUCCESS && x->set_security_context)
    {
      ଯଦି (! restorecon (x->set_security_context, last_component (dir), false)
          && ! ignorable_ctx_err (errno))
        error (0, errno, _("failed to restore context for %s"),
               quoteaf (dir));
    }

  ଫେରନ୍ତୁ ret;
}

ଗଣନା copy_status { COPY_FAILED, COPY_OK, COPY_SKIPPED };

/* Copy file FROM onto file TO aka TO_DIRFD+TO_RELNAME, creating TO if
   necessary.  Return COPY_OK if successful or COPY_SKIPPED if the copy
   was skipped.  */

ସ୍ଥିର ଗଣନା copy_status
copy_file (ଅକ୍ଷର ଅଚଳ *from, ଅକ୍ଷର ଅଚଳ *to,
           ପୂର୍ଣ୍ଣସଂଖ୍ୟା to_dirfd, ଅକ୍ଷର ଅଚଳ *to_relname, ଅଚଳ ସଂରଚନା cp_options *x)
{
  ଯଦି (copy_only_if_needed && !need_copy (from, to, to_dirfd, to_relname, x))
    ଫେରନ୍ତୁ COPY_SKIPPED;

  /* Allow installing from non-regular files like /dev/null.
     Charles Karney reported that some Sun version of install allows that
     and that sendmail's installation process relies on the behavior.
     However, since !x->recursive, the call to "copy" will fail if FROM
     is a directory.  */

  bool copy_into_self;
  ଫେରନ୍ତୁ (copy (from, to, to_dirfd, to_relname, 0, x, &copy_into_self, ଖାଲି)
          ? COPY_OK : COPY_FAILED);
}

/* Set the attributes of file or directory NAME aka DIRFD+RELNAME.
   Return true if successful.  */

ସ୍ଥିର bool
change_attributes (ଅକ୍ଷର ଅଚଳ *name, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, ଅକ୍ଷର ଅଚଳ *relname)
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

  ଯଦି (! (owner_id == (uid_t) -1 && group_id == (gid_t) -1)
      && lchownat (dirfd, relname, owner_id, group_id) != 0)
    error (0, errno, _("cannot change ownership of %s"), quoteaf (name));
  ଅନ୍ୟଥା ଯଦି (chmodat (dirfd, relname, mode) != 0)
    error (0, errno, _("cannot change permissions of %s"), quoteaf (name));
  ଅନ୍ୟଥା
    ok = true;

  ଯଦି (use_default_selinux_context)
    setdefaultfilecon (name);

  ଫେରନ୍ତୁ ok;
}

/* Set the timestamps of file DEST aka DIRFD+RELNAME to match those of SRC_SB.
   Return true if successful.  */

ସ୍ଥିର bool
change_timestamps (ସଂରଚନା stat ଅଚଳ *src_sb, ଅକ୍ଷର ଅଚଳ *dest,
                   ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, ଅକ୍ଷର ଅଚଳ *relname)
{
  ସଂରଚନା timespec timespec[2] = { get_stat_atime (src_sb),
                                  get_stat_mtime (src_sb) };

  ଯଦି (utimensat (dirfd, relname, timespec, 0))
    {
      error (0, errno, _("cannot set timestamps for %s"), quoteaf (dest));
      ଫେରନ୍ତୁ false;
    }
  ଫେରନ୍ତୁ true;
}

/* Strip the symbol table from the file NAME.
   We could dig the magic number out of the file first to
   determine whether to strip it, but the header files and
   magic numbers vary so much from system to system that making
   it portable would be very difficult.  Not worth the effort. */

ସ୍ଥିର bool
strip (ଅକ୍ଷର ଅଚଳ *name)
{
  posix_spawnattr_t attr;
  posix_spawnattr_t *attrp = ଖାଲି;

  /* Try to use vfork for systems where it matters.  */
  ଯଦି (posix_spawnattr_init (&attr) == 0)
    {
      ଯଦି (posix_spawnattr_setflags (&attr, POSIX_SPAWN_USEVFORK) == 0)
        attrp = &attr;
      ଅନ୍ୟଥା
        posix_spawnattr_destroy (&attr);
    }

  /* Construct the arguments to 'strip'.  */
  ଅକ୍ଷର *concat_name = ଖାଲି;
  ଅକ୍ଷର ଅଚଳ *safe_name = name;
  ଯଦି (name && *name == '-')
    safe_name = concat_name = file_name_concat (".", name, ଖାଲି);
  ଅକ୍ଷର ଅଚଳ *ଅଚଳ argv[] = { strip_program, safe_name, ଖାଲି };

  /* Run 'strip'.  */
  pid_t pid;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = posix_spawnp (&pid, strip_program, ଖାଲି, attrp,
                             (ଅକ୍ଷର * ଅଚଳ *) argv, environ);

  bool ok = false;
  ଯଦି (result != 0)
    {
      error (0, result,
             streq (strip_program, "strip")
               ? _("cannot run %s")
               : _("cannot run strip program %s"),
             quoteaf (strip_program));
    }
  ଅନ୍ୟଥା
    {
      /* Wait for 'strip' to complete, and emit a warning message on failure  */
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା status;
      ଯଦି (waitpid (pid, &status, 0) < 0)
        error (0, errno, _("waiting for strip"));
      ଅନ୍ୟଥା ଯଦି (! WIFEXITED (status) || WEXITSTATUS (status))
        error (0, 0, _("strip process terminated abnormally"));
      ଅନ୍ୟଥା
        ok = true;
    }

  ମୁକ୍ତ (concat_name);
  ଯଦି (attrp)
    posix_spawnattr_destroy (attrp);

  ଫେରନ୍ତୁ ok;
}

/* Initialize the user and group ownership of the files to install. */

ସ୍ଥିର ଶୂନ୍ୟ
get_ids (ଶୂନ୍ୟ)
{
  ଯଦି (owner_name)
    {
      ସଂରଚନା passwd *pw = getpwnam (owner_name);
      ଯଦି (pw == ଖାଲି)
        {
          uintmax_t tmp;
          ଯଦି (xstrtoumax (owner_name, ଖାଲି, 0, &tmp, "") != LONGINT_OK
              || ckd_add (&owner_id, tmp, 0))
            error (EXIT_FAILURE, 0, _("invalid user %s"),
                   quoteaf (owner_name));
        }
      ଅନ୍ୟଥା
        owner_id = pw->pw_uid;
      endpwent ();
    }
  ଅନ୍ୟଥା
    owner_id = (uid_t) -1;

  ଯଦି (group_name)
    {
      ସଂରଚନା group *gr = getgrnam (group_name);
      ଯଦି (gr == ଖାଲି)
        {
          uintmax_t tmp;
          ଯଦି (xstrtoumax (group_name, ଖାଲି, 0, &tmp, "") != LONGINT_OK
              || ckd_add (&group_id, tmp, 0))
            error (EXIT_FAILURE, 0, _("invalid group %s"),
                   quoteaf (group_name));
        }
      ଅନ୍ୟଥା
        group_id = gr->gr_gid;
      endgrent ();
    }
  ଅନ୍ୟଥା
    group_id = (gid_t) -1;
}

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
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
the existing DIRECTORY, ଯେତେବେଳେ setting permission modes and owner/group.\n\
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
         and ଯଦି no change to content, ownership, and permissions,\n\
         କର not modify the destination at all\n\
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
         strip ପ୍ରତୀକ tables\n\
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
         and each created directory, to ପୂର୍ବନିର୍ଦ୍ଧାରିତ type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or ଯଦି CTX is specified then set the\n\
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

ସ୍ଥିର bool
install_file_in_file (ଅକ୍ଷର ଅଚଳ *from, ଅକ୍ଷର ଅଚଳ *to,
                      ପୂର୍ଣ୍ଣସଂଖ୍ୟା to_dirfd, ଅକ୍ଷର ଅଚଳ *to_relname,
                      ଅଚଳ ସଂରଚନା cp_options *x)
{
  ସଂରଚନା stat from_sb;
  ଯଦି (x->preserve_timestamps && stat (from, &from_sb) != 0)
    {
      error (0, errno, _("cannot stat %s"), quoteaf (from));
      ଫେରନ୍ତୁ false;
    }
  ଗଣନା copy_status copy_status = copy_file (from, to, to_dirfd, to_relname, x);
  ଯଦି (copy_status == COPY_FAILED)
    ଫେରନ୍ତୁ false;
  ଯଦି (copy_status == COPY_OK && strip_files && ! strip (to))
    {
      ଯଦି (unlinkat (to_dirfd, to_relname, 0) != 0)  /* Cleanup.  */
        error (EXIT_FAILURE, errno, _("cannot unlink %s"), quoteaf (to));
      ଫେରନ୍ତୁ false;
    }
  ଯଦି (x->preserve_timestamps && (copy_status == COPY_SKIPPED || strip_files
                                 || ! S_ISREG (from_sb.st_mode))
      && ! change_timestamps (&from_sb, to, to_dirfd, to_relname))
    ଫେରନ୍ତୁ false;

  ଫେରନ୍ତୁ copy_status == COPY_SKIPPED
         || change_attributes (to, to_dirfd, to_relname);
}

/* Create any missing parent directories of TO,
   while maintaining the current Working Directory.
   Return true if successful.  */

ସ୍ଥିର bool
mkancesdirs_safe_wd (ଅକ୍ଷର ଅଚଳ *from, ଅକ୍ଷର *to, ସଂରଚନା cp_options *x,
                     bool save_always)
{
  bool save_working_directory =
    save_always
    || ! (IS_ABSOLUTE_FILE_NAME (from) && IS_ABSOLUTE_FILE_NAME (to));

  ସଂରଚନା savewd wd;
  savewd_init (&wd);
  ଯଦି (! save_working_directory)
    savewd_finish (&wd);

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା status = EXIT_SUCCESS;
  ଯଦି (mkancesdirs (to, &wd, make_ancestor, x) == -1)
    {
      error (0, errno, _("cannot create directory %s"), quoteaf (to));
      status = EXIT_FAILURE;
    }

  ଯଦି (save_working_directory)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା restore_result = savewd_restore (&wd, status);
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା restore_errno = errno;
      savewd_finish (&wd);
      ଯଦି (EXIT_SUCCESS < restore_result)
        ଫେରନ୍ତୁ false;
      ଯଦି (restore_result < 0 && status == EXIT_SUCCESS)
        {
          error (0, restore_errno, _("cannot create directory %s"),
                 quoteaf (to));
          ଫେରନ୍ତୁ false;
        }
    }
  ଫେରନ୍ତୁ status == EXIT_SUCCESS;
}

/* Copy file FROM onto file TO, creating any missing parent directories of TO.
   Return true if successful.  */

ସ୍ଥିର bool
install_file_in_file_parents (ଅକ୍ଷର ଅଚଳ *from, ଅକ୍ଷର *to,
                              ଅଚଳ ସଂରଚନା cp_options *x)
{
  ଫେରନ୍ତୁ (mkancesdirs_safe_wd (from, to, (ସଂରଚନା cp_options *)x, false)
          && install_file_in_file (from, to, AT_FDCWD, to, x));
}

/* Copy file FROM into directory TO_DIR, keeping its same name,
   and give the copy the appropriate attributes.
   Return true if successful.  */

ସ୍ଥିର bool
install_file_in_dir (ଅକ୍ଷର ଅଚଳ *from, ଅକ୍ଷର ଅଚଳ *to_dir,
                     ଅଚଳ ସଂରଚନା cp_options *x, bool mkdir_and_install,
                     ପୂର୍ଣ୍ଣସଂଖ୍ୟା *target_dirfd)
{
  ଅକ୍ଷର ଅଚଳ *from_base = last_component (from);
  ଅକ୍ଷର *to_relname;
  ଅକ୍ଷର *to = file_name_concat (to_dir, from_base, &to_relname);
  bool ret = true;

  ଯଦି (!target_dirfd_valid (*target_dirfd)
      && (ret = mkdir_and_install)
      && (ret = mkancesdirs_safe_wd (from, to, (ସଂରଚନା cp_options *) x, true)))
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = open (to_dir, O_PATHSEARCH | O_DIRECTORY);
      ଯଦି (fd < 0)
        {
          error (0, errno, _("cannot open %s"), quoteaf (to));
          ret = false;
        }
      ଅନ୍ୟଥା
        *target_dirfd = fd;
    }

  ଯଦି (ret)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା to_dirfd = *target_dirfd;
      ଯଦି (!target_dirfd_valid (to_dirfd))
        {
          to_dirfd = AT_FDCWD;
          to_relname = to;
        }
      ret = install_file_in_file (from, to, to_dirfd, to_relname, x);
    }

  ମୁକ୍ତ (to);
  ଫେରନ୍ତୁ ret;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  ଅକ୍ଷର ଅଚଳ *specified_mode = ଖାଲି;
  bool make_backups = false;
  ଅକ୍ଷର ଅଚଳ *backup_suffix = ଖାଲି;
  ଅକ୍ଷର *version_control_string = ଖାଲି;
  bool mkdir_and_install = false;
  ସଂରଚନା cp_options x;
  ଅକ୍ଷର ଅଚଳ *target_directory = ଖାଲି;
  bool no_target_directory = false;
  bool strip_program_specified = false;
  ଅକ୍ଷର ଅଚଳ *scontext = ଖାଲି;
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

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା optc;
  ଯେତେବେଳେ ((optc = getopt_long (argc, argv, "bcCsDdg:m:o:pt:TvS:Z", long_options,
                              ଖାଲି))
         != -1)
    {
      ବାଛ (optc)
        {
        ସ୍ଥିତି 'b':
          make_backups = true;
          ଯଦି (optarg)
            version_control_string = optarg;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'c':
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'C':
          copy_only_if_needed = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 's':
          strip_files = true;
#ifdef SIGCHLD
          /* System V fork+wait does not work if SIGCHLD is ignored.  */
          ସଙ୍କେତ (SIGCHLD, SIG_DFL);
#endif
          ଭାଙ୍ଗ;
        ସ୍ଥିତି DEBUG_OPTION:
          x.debug = x.verbose = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି STRIP_PROGRAM_OPTION:
          strip_program = xstrdup (optarg);
          strip_program_specified = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'd':
          dir_arg = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'D':
          mkdir_and_install = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'v':
          x.verbose = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'g':
          group_name = optarg;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'm':
          specified_mode = optarg;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'o':
          owner_name = optarg;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'p':
          x.preserve_timestamps = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'S':
          make_backups = true;
          backup_suffix = optarg;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 't':
          ଯଦି (target_directory)
            error (EXIT_FAILURE, 0,
                   _("multiple target directories specified"));
          target_directory = optarg;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'T':
          no_target_directory = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି PRESERVE_CONTEXT_OPTION:
          ଯଦି (! selinux_enabled)
            {
              error (0, 0, _("WARNING: ignoring --preserve-context; "
                             "this kernel is not SELinux-enabled"));
              ଭାଙ୍ଗ;
            }
          x.preserve_security_context = true;
          use_default_selinux_context = false;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'Z':
          ଯଦି (selinux_enabled)
            {
              /* Disable use of the install(1) specific setdefaultfilecon().
                 Note setdefaultfilecon() is different from the newer and more
                 generic restorecon() in that the former sets the context of
                 the dest files to that returned by selabel_lookup directly,
                 thus discarding MLS level and user identity of the file.
                 TODO: consider removing setdefaultfilecon() in future.  */
              use_default_selinux_context = false;

              ଯଦି (optarg)
                scontext = optarg;
              ଅନ୍ୟଥା
                x.set_security_context = get_labeling_handle ();
            }
          ଅନ୍ୟଥା ଯଦି (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux-enabled kernel"));
            }
          ଭାଙ୍ଗ;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          usage (EXIT_FAILURE);
        }
    }

  /* Check for invalid combinations of arguments. */
  ଯଦି (dir_arg && strip_files)
    error (EXIT_FAILURE, 0,
           _("the strip option may not be used when installing a directory"));
  ଯଦି (dir_arg && target_directory)
    error (EXIT_FAILURE, 0,
           _("target directory not allowed when installing a directory"));

  x.backup_type = (make_backups
                   ? xget_version (_("backup type"),
                                   version_control_string)
                   : no_backups);
  set_simple_backup_suffix (backup_suffix);

  ଯଦି (x.preserve_security_context && (x.set_security_context || scontext))
    error (EXIT_FAILURE, 0,
           _("cannot set target context and preserve it"));

  ଯଦି (scontext && setfscreatecon (scontext) < 0)
    error (EXIT_FAILURE, errno,
           _("failed to set default file creation context to %s"),
         quote (scontext));

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_files = argc - optind;
  ଅକ୍ଷର **file = argv + optind;

  ଯଦି (n_files <= ! (dir_arg || target_directory))
    {
      ଯଦି (n_files <= 0)
        error (0, 0, _("missing file operand"));
      ଅନ୍ୟଥା
        error (0, 0, _("missing destination file operand after %s"),
               quoteaf (file[0]));
      usage (EXIT_FAILURE);
    }

  ସଂରଚନା stat sb;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା target_dirfd = AT_FDCWD;
  ଯଦି (no_target_directory)
    {
      ଯଦି (target_directory)
        error (EXIT_FAILURE, 0,
               _("cannot combine --target-directory (-t) "
                 "and --no-target-directory (-T)"));
      ଯଦି (2 < n_files)
        {
          error (0, 0, _("extra operand %s"), quoteaf (file[2]));
          usage (EXIT_FAILURE);
        }
    }
  ଅନ୍ୟଥା ଯଦି (target_directory)
    {
      target_dirfd = target_directory_operand (target_directory, &sb);
      ଯଦି (! (target_dirfd_valid (target_dirfd)
             || (mkdir_and_install && errno == ENOENT)))
        error (EXIT_FAILURE, errno, _("failed to access %s"),
               quoteaf (target_directory));
    }
  ଅନ୍ୟଥା ଯଦି (!dir_arg)
    {
      ଅକ୍ଷର ଅଚଳ *lastfile = file[n_files - 1];
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = target_directory_operand (lastfile, &sb);
      ଯଦି (target_dirfd_valid (fd))
        {
          target_dirfd = fd;
          target_directory = lastfile;
          n_files--;
        }
      ଅନ୍ୟଥା ଯଦି (2 < n_files)
        error (EXIT_FAILURE, errno, _("target %s"), quoteaf (lastfile));
    }

  ଯଦି (specified_mode)
    {
      ସଂରଚନା mode_change *change = mode_compile (specified_mode);
      ଯଦି (!change)
        error (EXIT_FAILURE, 0, _("invalid mode %s"), quote (specified_mode));
      mode = mode_adjust (0, false, 0, change, ଖାଲି);
      dir_mode = mode_adjust (0, true, 0, change, &dir_mode_bits);
      ମୁକ୍ତ (change);
    }

  ଯଦି (strip_program_specified && !strip_files)
    error (0, 0, _("WARNING: ignoring --strip-program option as -s option was "
                   "not specified"));

  ଯଦି (copy_only_if_needed && strip_files)
    {
      error (0, 0, _("options --compare (-C) and --strip are mutually "
                     "exclusive"));
      usage (EXIT_FAILURE);
    }

  ଯଦି (copy_only_if_needed && extra_mode (mode))
    error (0, 0, _("the --compare (-C) option is ignored when you"
                   " specify a mode with non-permission bits"));

  get_ids ();

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା exit_status = EXIT_SUCCESS;
  ଯଦି (dir_arg)
    exit_status = savewd_process_files (n_files, file, process_dir, &x);
  ଅନ୍ୟଥା
    {
      /* FIXME: it's a little gross that this initialization is
         required by copy.c::copy. */
      hash_init ();

      ଯଦି (!target_directory)
        {
          ଯଦି (! (mkdir_and_install
                 ? install_file_in_file_parents (file[0], file[1], &x)
                 : install_file_in_file (file[0], file[1], AT_FDCWD,
                                         file[1], &x)))
            exit_status = EXIT_FAILURE;
        }
      ଅନ୍ୟଥା
        {
          dest_info_init (&x);
          ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < n_files; i++)
            ଯଦି (! install_file_in_dir (file[i], target_directory, &x,
                                       i == 0 && mkdir_and_install,
                                       &target_dirfd))
              exit_status = EXIT_FAILURE;
        }
    }

  main_exit (exit_status);
}
