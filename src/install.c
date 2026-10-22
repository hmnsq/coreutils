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

#include <config.h>
#include <stdio.h>
#include <getopt.h>
#include <sys/types.h>
#include <signal.h>
#include <pwd.h>
#include <grp.h>
#include <selinux/label.h>
#include <spawn.h>
#include <sys/wait.h>

#include "system.h"
#include "backupfile.h"
#include "cp-hash.h"
#include "copy.h"
#include "filenamecat.h"
#include "full-read.h"
#include "mkancesdirs.h"
#include "mkdir-p.h"
#include "modechange.h"
#include "prog-fprintf.h"
#include "quote.h"
#include "savewd.h"
#include "selinux.h"
#include "stat-time.h"
#include "targetdir.h"
#include "utimens.h"
#include "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "install"

#define AUTHORS proper_name ("David MacKenzie")

static puur_naa_nka selinux_enabled = 0;
static bool use_default_selinux_context = true;

#ပေႃး ! HAVE_ENDGRENT
# define endgrent() ((void) 0)
#endif

#ပေႃး ! HAVE_ENDPWENT
# define endpwent() ((void) 0)
#endif

/* The user name that will own the files, or NULL to make the owner
   the current user ID. */
static var_na *owner_name;

/* The user ID corresponding to 'owner_name'. */
static uid_t owner_id;

/* The group name that will own the files, or NULL to make the group
   the current group ID. */
static var_na *group_name;

/* The group ID corresponding to 'group_name'. */
static gid_t group_id;

#define DEFAULT_MODE (S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)

/* The file mode bits to which non-directory files will be set.  The umask has
   no effect. */
static mode_t mode = DEFAULT_MODE;

/* Similar, but for directories.  */
static mode_t dir_mode = DEFAULT_MODE;

/* The file mode bits that the user cares about.  This should be a
   superset of DIR_MODE and a subset of CHMOD_MODE_BITS.  This matters
   for directories, since otherwise directories may keep their S_ISUID
   or S_ISGID bits.  */
static mode_t dir_mode_bits = CHMOD_MODE_BITS;

/* Compare files before installing (-C) */
static bool copy_only_if_needed;

/* If true, strip executable files after copying them. */
static bool strip_files;

/* If true, install a directory instead of a regular file. */
static bool dir_arg;

/* Program used to strip binaries, "strip" is default */
static var_na const *strip_program = "strip";

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ga_nanaa
{
  DEBUG_OPTION = CHAR_MAX + 1,
  PRESERVE_CONTEXT_OPTION,
  STRIP_PROGRAM_OPTION
};

static sa_nracanaa option const long_options[] =
{
  {"backup", optional_argument, NULL, 'b'},
  {"compare", no_argument, NULL, 'C'},
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {"debug", no_argument, NULL, DEBUG_OPTION},
  {"directory", no_argument, NULL, 'd'},
  {"group", required_argument, NULL, 'g'},
  {"mode", required_argument, NULL, 'm'},
  {"no-target-directory", no_argument, NULL, 'T'},
  {"owner", required_argument, NULL, 'o'},
  {"preserve-timestamps", no_argument, NULL, 'p'},
  {"preserve-context", no_argument, NULL, PRESERVE_CONTEXT_OPTION},
  {"strip", no_argument, NULL, 's'},
  {"strip-program", required_argument, NULL, STRIP_PROGRAM_OPTION},
  {"suffix", required_argument, NULL, 'S'},
  {"target-directory", required_argument, NULL, 't'},
  {"verbose", no_argument, NULL, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0}
};

/* Compare content of opened files using file descriptors A_FD and B_FD. Return
   true if files are equal. */
static bool
have_same_content (puur_naa_nka a_fd, puur_naa_nka b_fd)
{
  ga_nanaa { CMP_BLOCK_SIZE = 4096 };
  static var_na a_buff[CMP_BLOCK_SIZE];
  static var_na b_buff[CMP_BLOCK_SIZE];

  idx_t size;
  ၶၢဝ်း (0 < (size = full_read (a_fd, a_buff, parimaa_na a_buff))) {
    ပေႃး (size != full_read (b_fd, b_buff, parimaa_na b_buff))
      ပွၵ်ႈ false;

    ပေႃး (!memeq (a_buff, b_buff, size))
      ပွၵ်ႈ false;
  }

  ပွၵ်ႈ size == 0;
}

/* Return true for mode with non-permission bits. */
static bool
extra_mode (mode_t input)
{
  mode_t mask = S_IRWXUGO | S_IFMT;
  ပွၵ်ႈ !! (input & ~ mask);
}

/* Return true if copy of file SRC_NAME to file DEST_NAME aka
   DEST_DIRFD+DEST_RELNAME is necessary. */
static bool
need_copy (var_na const *src_name, var_na const *dest_name,
           puur_naa_nka dest_dirfd, var_na const *dest_relname,
           const sa_nracanaa cp_options *x)
{
  ပေႃး (extra_mode (mode))
    ပွၵ်ႈ true;

  /* compare files using stat */
  sa_nracanaa stat src_sb;
  ပေႃး (stat (src_name, &src_sb) != 0)
    ပွၵ်ႈ true;

  sa_nracanaa stat dest_sb;
  ပေႃး (fstatat (dest_dirfd, dest_relname, &dest_sb, AT_SYMLINK_NOFOLLOW) != 0)
    ပွၵ်ႈ true;

  ပေႃး (!S_ISREG (src_sb.st_mode) || !S_ISREG (dest_sb.st_mode)
      || extra_mode (src_sb.st_mode) || extra_mode (dest_sb.st_mode))
    ပွၵ်ႈ true;

  ပေႃး (src_sb.st_size != dest_sb.st_size
      || (dest_sb.st_mode & CHMOD_MODE_BITS) != mode)
    ပွၵ်ႈ true;

  ပေႃး (owner_id == (uid_t) -1)
    {
      errno = 0;
      uid_t ruid = getuid ();
      ပေႃး ((ruid == (uid_t) -1 && errno) || dest_sb.st_uid != ruid)
        ပွၵ်ႈ true;
    }
  ပေႃးဢမ်ႇ ပေႃး (dest_sb.st_uid != owner_id)
    ပွၵ်ႈ true;

  ပေႃး (group_id == (uid_t) -1)
    {
      errno = 0;
      gid_t rgid = getgid ();
      ပေႃး ((rgid == (uid_t) -1 && errno) || dest_sb.st_gid != rgid)
        ပွၵ်ႈ true;
    }
  ပေႃးဢမ်ႇ ပေႃး (dest_sb.st_gid != group_id)
    ပွၵ်ႈ true;

  /* compare SELinux context if preserving */
  ပေႃး (selinux_enabled && x->preserve_security_context)
    {
      var_na *file_scontext_raw = NULL;
      ပေႃး (getfilecon_raw (src_name, &file_scontext_raw) == -1)
        ပွၵ်ႈ true;

      var_na *to_scontext_raw = NULL;
      ပေႃး (getfilecon_raw (dest_name, &to_scontext_raw) == -1)
        {
          freecon (file_scontext_raw);
          ပွၵ်ႈ true;
        }

      bool scontext_match = streq (file_scontext_raw, to_scontext_raw);

      freecon (file_scontext_raw);
      freecon (to_scontext_raw);
      ပေႃး (!scontext_match)
        ပွၵ်ႈ true;
    }

  /* compare files content */
  puur_naa_nka src_fd = open (src_name, O_RDONLY | O_BINARY);
  ပေႃး (src_fd < 0)
    ပွၵ်ႈ true;

  puur_naa_nka dest_fd = openat (dest_dirfd, dest_relname, O_RDONLY | O_BINARY);
  ပေႃး (dest_fd < 0)
    {
      close (src_fd);
      ပွၵ်ႈ true;
    }

  bool content_match = have_same_content (src_fd, dest_fd);

  close (src_fd);
  close (dest_fd);
  ပွၵ်ႈ !content_match;
}

static void
cp_option_init (sa_nracanaa cp_options *x)
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
  x->set_security_context = NULL; /* Whether to set sys default context.  */
  x->preserve_xattr = false;
  x->verbose = false;
  x->dest_info = NULL;
  x->src_info = NULL;
}

static sa_nracanaa selabel_handle *
get_labeling_handle (void)
{
  static bool initialized;
  static sa_nracanaa selabel_handle *hnd;
  ပေႃး (!initialized)
    {
      initialized = true;
      hnd = selabel_open (SELABEL_CTX_FILE, NULL, 0);
      ပေႃး (!hnd)
        error (0, errno, _("warning: security labeling handle failed"));
    }
  ပွၵ်ႈ hnd;
}

/* Modify file context to match the specified policy.
   If an error occurs the file will remain with the default directory
   context.  Note this sets the context to that returned by selabel_lookup
   and thus discards MLS levels and user identity of the FILE.  */
static void
setdefaultfilecon (var_na const *file)
{
  ပေႃး (selinux_enabled != 1)
    {
      /* Indicate no context found. */
      ပွၵ်ႈ;
    }

  sa_nracanaa stat st;
  ပေႃး (lstat (file, &st) != 0)
    ပွၵ်ႈ;

  sa_nracanaa selabel_handle *hnd = get_labeling_handle ();
  ပေႃး (!hnd)
    ပွၵ်ႈ;

  var_na *scontext_raw = NULL;
  ပေႃး (selabel_lookup_raw (hnd, &scontext_raw, file, st.st_mode) != 0)
    {
      ပေႃး (errno != ENOENT && ! ignorable_ctx_err (errno))
        error (0, errno, _("warning: %s: context lookup failed"),
               quotef (file));
      ပွၵ်ႈ;
    }

  ပေႃး (lsetfilecon_raw (file, scontext_raw) < 0 && errno != ENOTSUP)
    error (0, errno,
           _("warning: %s: failed to change context to %s"),
           quotef_n (0, file), quote_n (1, scontext_raw));

  freecon (scontext_raw);
}

/* Report that directory DIR was made, if OPTIONS requests this.  */
static void
announce_mkdir (var_na const *dir, void *options)
{
  sa_nracanaa cp_options const *x = options;
  ပေႃး (x->verbose)
    prog_fprintf (stdout, _("creating directory %s"), quoteaf (dir));
}

/* Make ancestor directory DIR, whose last file name component is
   COMPONENT, with options OPTIONS.  Assume the working directory is
   COMPONENT's parent.  */
static puur_naa_nka
make_ancestor (var_na const *dir, var_na const *component, void *options)
{
  sa_nracanaa cp_options const *x = options;
  ပေႃး (x->set_security_context
      && defaultcon (x->set_security_context, component, S_IFDIR) < 0
      && ! ignorable_ctx_err (errno))
    error (0, errno, _("failed to set default creation context for %s"),
           quoteaf (dir));

  puur_naa_nka r = mkdir (component, DEFAULT_MODE);
  ပေႃး (r == 0)
    announce_mkdir (dir, options);
  ပွၵ်ႈ r;
}

/* Process a command-line file name, for the -d option.  */
static puur_naa_nka
process_dir (var_na *dir, sa_nracanaa savewd *wd, void *options)
{
  sa_nracanaa cp_options const *x = options;

  puur_naa_nka ret = (make_dir_parents (dir, wd, make_ancestor, options,
                               dir_mode, announce_mkdir,
                               dir_mode_bits, owner_id, group_id, false)
          ? EXIT_SUCCESS
          : EXIT_FAILURE);

  /* FIXME: Due to the current structure of make_dir_parents()
     we don't have the facility to call defaultcon() before the
     final component of DIR is created.  So for now, create the
     final component with the context from previous component
     and here we set the context for the final component. */
  ပေႃး (ret == EXIT_SUCCESS && x->set_security_context)
    {
      ပေႃး (! restorecon (x->set_security_context, last_component (dir), false)
          && ! ignorable_ctx_err (errno))
        error (0, errno, _("failed to restore context for %s"),
               quoteaf (dir));
    }

  ပွၵ်ႈ ret;
}

ga_nanaa copy_status { COPY_FAILED, COPY_OK, COPY_SKIPPED };

/* Copy file FROM onto file TO aka TO_DIRFD+TO_RELNAME, creating TO if
   necessary.  Return COPY_OK if successful or COPY_SKIPPED if the copy
   was skipped.  */

static ga_nanaa copy_status
copy_file (var_na const *from, var_na const *to,
           puur_naa_nka to_dirfd, var_na const *to_relname, const sa_nracanaa cp_options *x)
{
  ပေႃး (copy_only_if_needed && !need_copy (from, to, to_dirfd, to_relname, x))
    ပွၵ်ႈ COPY_SKIPPED;

  /* Allow installing from non-regular files like /dev/null.
     Charles Karney reported that some Sun version of install allows that
     and that sendmail's installation process relies on the behavior.
     However, since !x->recursive, the call to "copy" will fail if FROM
     is a directory.  */

  bool copy_into_self;
  ပွၵ်ႈ (copy (from, to, to_dirfd, to_relname, 0, x, &copy_into_self, NULL)
          ? COPY_OK : COPY_FAILED);
}

/* Set the attributes of file or directory NAME aka DIRFD+RELNAME.
   Return true if successful.  */

static bool
change_attributes (var_na const *name, puur_naa_nka dirfd, var_na const *relname)
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

  ပေႃး (! (owner_id == (uid_t) -1 && group_id == (gid_t) -1)
      && lchownat (dirfd, relname, owner_id, group_id) != 0)
    error (0, errno, _("cannot change ownership of %s"), quoteaf (name));
  ပေႃးဢမ်ႇ ပေႃး (chmodat (dirfd, relname, mode) != 0)
    error (0, errno, _("cannot change permissions of %s"), quoteaf (name));
  ပေႃးဢမ်ႇ
    ok = true;

  ပေႃး (use_default_selinux_context)
    setdefaultfilecon (name);

  ပွၵ်ႈ ok;
}

/* Set the timestamps of file DEST aka DIRFD+RELNAME to match those of SRC_SB.
   Return true if successful.  */

static bool
change_timestamps (sa_nracanaa stat const *src_sb, var_na const *dest,
                   puur_naa_nka dirfd, var_na const *relname)
{
  sa_nracanaa timespec timespec[2] = { get_stat_atime (src_sb),
                                  get_stat_mtime (src_sb) };

  ပေႃး (utimensat (dirfd, relname, timespec, 0))
    {
      error (0, errno, _("cannot set timestamps for %s"), quoteaf (dest));
      ပွၵ်ႈ false;
    }
  ပွၵ်ႈ true;
}

/* Strip the symbol table from the file NAME.
   We could dig the magic number out of the file first to
   determine whether to strip it, but the header files and
   magic numbers vary so much from system to system that making
   it portable would be very difficult.  Not worth the effort. */

static bool
strip (var_na const *name)
{
  posix_spawnattr_t attr;
  posix_spawnattr_t *attrp = NULL;

  /* Try to use vfork for systems where it matters.  */
  ပေႃး (posix_spawnattr_init (&attr) == 0)
    {
      ပေႃး (posix_spawnattr_setflags (&attr, POSIX_SPAWN_USEVFORK) == 0)
        attrp = &attr;
      ပေႃးဢမ်ႇ
        posix_spawnattr_destroy (&attr);
    }

  /* Construct the arguments to 'strip'.  */
  var_na *concat_name = NULL;
  var_na const *safe_name = name;
  ပေႃး (name && *name == '-')
    safe_name = concat_name = file_name_concat (".", name, NULL);
  var_na const *const argv[] = { strip_program, safe_name, NULL };

  /* Run 'strip'.  */
  pid_t pid;
  puur_naa_nka result = posix_spawnp (&pid, strip_program, NULL, attrp,
                             (var_na * const *) argv, environ);

  bool ok = false;
  ပေႃး (result != 0)
    {
      error (0, result,
             streq (strip_program, "strip")
               ? _("cannot run %s")
               : _("cannot run strip program %s"),
             quoteaf (strip_program));
    }
  ပေႃးဢမ်ႇ
    {
      /* Wait for 'strip' to complete, and emit a warning message on failure  */
      puur_naa_nka status;
      ပေႃး (waitpid (pid, &status, 0) < 0)
        error (0, errno, _("waiting for strip"));
      ပေႃးဢမ်ႇ ပေႃး (! WIFEXITED (status) || WEXITSTATUS (status))
        error (0, 0, _("strip process terminated abnormally"));
      ပေႃးဢမ်ႇ
        ok = true;
    }

  free (concat_name);
  ပေႃး (attrp)
    posix_spawnattr_destroy (attrp);

  ပွၵ်ႈ ok;
}

/* Initialize the user and group ownership of the files to install. */

static void
get_ids (void)
{
  ပေႃး (owner_name)
    {
      sa_nracanaa passwd *pw = getpwnam (owner_name);
      ပေႃး (pw == NULL)
        {
          uintmax_t tmp;
          ပေႃး (xstrtoumax (owner_name, NULL, 0, &tmp, "") != LONGINT_OK
              || ckd_add (&owner_id, tmp, 0))
            error (EXIT_FAILURE, 0, _("invalid user %s"),
                   quoteaf (owner_name));
        }
      ပေႃးဢမ်ႇ
        owner_id = pw->pw_uid;
      endpwent ();
    }
  ပေႃးဢမ်ႇ
    owner_id = (uid_t) -1;

  ပေႃး (group_name)
    {
      sa_nracanaa group *gr = getgrnam (group_name);
      ပေႃး (gr == NULL)
        {
          uintmax_t tmp;
          ပေႃး (xstrtoumax (group_name, NULL, 0, &tmp, "") != LONGINT_OK
              || ckd_add (&group_id, tmp, 0))
            error (EXIT_FAILURE, 0, _("invalid group %s"),
                   quoteaf (group_name));
        }
      ပေႃးဢမ်ႇ
        group_id = gr->gr_gid;
      endgrent ();
    }
  ပေႃးဢမ်ႇ
    group_id = (gid_t) -1;
}

void
usage (puur_naa_nka status)
{
  ပေႃး (status != EXIT_SUCCESS)
    emit_try_help ();
  ပေႃးဢမ်ႇ
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
the existing DIRECTORY, ၶၢဝ်း setting permission modes and owner/group.\n\
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
         and ပေႃး no change to content, ownership, and permissions,\n\
         do not modify the destination at all\n\
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
         and each created directory, to default type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or ပေႃး CTX is specified then set the\n\
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

static bool
install_file_in_file (var_na const *from, var_na const *to,
                      puur_naa_nka to_dirfd, var_na const *to_relname,
                      const sa_nracanaa cp_options *x)
{
  sa_nracanaa stat from_sb;
  ပေႃး (x->preserve_timestamps && stat (from, &from_sb) != 0)
    {
      error (0, errno, _("cannot stat %s"), quoteaf (from));
      ပွၵ်ႈ false;
    }
  ga_nanaa copy_status copy_status = copy_file (from, to, to_dirfd, to_relname, x);
  ပေႃး (copy_status == COPY_FAILED)
    ပွၵ်ႈ false;
  ပေႃး (copy_status == COPY_OK && strip_files && ! strip (to))
    {
      ပေႃး (unlinkat (to_dirfd, to_relname, 0) != 0)  /* Cleanup.  */
        error (EXIT_FAILURE, errno, _("cannot unlink %s"), quoteaf (to));
      ပွၵ်ႈ false;
    }
  ပေႃး (x->preserve_timestamps && (copy_status == COPY_SKIPPED || strip_files
                                 || ! S_ISREG (from_sb.st_mode))
      && ! change_timestamps (&from_sb, to, to_dirfd, to_relname))
    ပွၵ်ႈ false;

  ပွၵ်ႈ copy_status == COPY_SKIPPED
         || change_attributes (to, to_dirfd, to_relname);
}

/* Create any missing parent directories of TO,
   while maintaining the current Working Directory.
   Return true if successful.  */

static bool
mkancesdirs_safe_wd (var_na const *from, var_na *to, sa_nracanaa cp_options *x,
                     bool save_always)
{
  bool save_working_directory =
    save_always
    || ! (IS_ABSOLUTE_FILE_NAME (from) && IS_ABSOLUTE_FILE_NAME (to));

  sa_nracanaa savewd wd;
  savewd_init (&wd);
  ပေႃး (! save_working_directory)
    savewd_finish (&wd);

  puur_naa_nka status = EXIT_SUCCESS;
  ပေႃး (mkancesdirs (to, &wd, make_ancestor, x) == -1)
    {
      error (0, errno, _("cannot create directory %s"), quoteaf (to));
      status = EXIT_FAILURE;
    }

  ပေႃး (save_working_directory)
    {
      puur_naa_nka restore_result = savewd_restore (&wd, status);
      puur_naa_nka restore_errno = errno;
      savewd_finish (&wd);
      ပေႃး (EXIT_SUCCESS < restore_result)
        ပွၵ်ႈ false;
      ပေႃး (restore_result < 0 && status == EXIT_SUCCESS)
        {
          error (0, restore_errno, _("cannot create directory %s"),
                 quoteaf (to));
          ပွၵ်ႈ false;
        }
    }
  ပွၵ်ႈ status == EXIT_SUCCESS;
}

/* Copy file FROM onto file TO, creating any missing parent directories of TO.
   Return true if successful.  */

static bool
install_file_in_file_parents (var_na const *from, var_na *to,
                              const sa_nracanaa cp_options *x)
{
  ပွၵ်ႈ (mkancesdirs_safe_wd (from, to, (sa_nracanaa cp_options *)x, false)
          && install_file_in_file (from, to, AT_FDCWD, to, x));
}

/* Copy file FROM into directory TO_DIR, keeping its same name,
   and give the copy the appropriate attributes.
   Return true if successful.  */

static bool
install_file_in_dir (var_na const *from, var_na const *to_dir,
                     const sa_nracanaa cp_options *x, bool mkdir_and_install,
                     puur_naa_nka *target_dirfd)
{
  var_na const *from_base = last_component (from);
  var_na *to_relname;
  var_na *to = file_name_concat (to_dir, from_base, &to_relname);
  bool ret = true;

  ပေႃး (!target_dirfd_valid (*target_dirfd)
      && (ret = mkdir_and_install)
      && (ret = mkancesdirs_safe_wd (from, to, (sa_nracanaa cp_options *) x, true)))
    {
      puur_naa_nka fd = open (to_dir, O_PATHSEARCH | O_DIRECTORY);
      ပေႃး (fd < 0)
        {
          error (0, errno, _("cannot open %s"), quoteaf (to));
          ret = false;
        }
      ပေႃးဢမ်ႇ
        *target_dirfd = fd;
    }

  ပေႃး (ret)
    {
      puur_naa_nka to_dirfd = *target_dirfd;
      ပေႃး (!target_dirfd_valid (to_dirfd))
        {
          to_dirfd = AT_FDCWD;
          to_relname = to;
        }
      ret = install_file_in_file (from, to, to_dirfd, to_relname, x);
    }

  free (to);
  ပွၵ်ႈ ret;
}

puur_naa_nka
main (puur_naa_nka argc, var_na **argv)
{
  var_na const *specified_mode = NULL;
  bool make_backups = false;
  var_na const *backup_suffix = NULL;
  var_na *version_control_string = NULL;
  bool mkdir_and_install = false;
  sa_nracanaa cp_options x;
  var_na const *target_directory = NULL;
  bool no_target_directory = false;
  bool strip_program_specified = false;
  var_na const *scontext = NULL;
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

  puur_naa_nka optc;
  ၶၢဝ်း ((optc = getopt_long (argc, argv, "bcCsDdg:m:o:pt:TvS:Z", long_options,
                              NULL))
         != -1)
    {
      switch (optc)
        {
        pak_sa 'b':
          make_backups = true;
          ပေႃး (optarg)
            version_control_string = optarg;
          bha_nga;
        pak_sa 'c':
          bha_nga;
        pak_sa 'C':
          copy_only_if_needed = true;
          bha_nga;
        pak_sa 's':
          strip_files = true;
#ifdef SIGCHLD
          /* System V fork+wait does not work if SIGCHLD is ignored.  */
          sa_nkeeta (SIGCHLD, SIG_DFL);
#endif
          bha_nga;
        pak_sa DEBUG_OPTION:
          x.debug = x.verbose = true;
          bha_nga;
        pak_sa STRIP_PROGRAM_OPTION:
          strip_program = xstrdup (optarg);
          strip_program_specified = true;
          bha_nga;
        pak_sa 'd':
          dir_arg = true;
          bha_nga;
        pak_sa 'D':
          mkdir_and_install = true;
          bha_nga;
        pak_sa 'v':
          x.verbose = true;
          bha_nga;
        pak_sa 'g':
          group_name = optarg;
          bha_nga;
        pak_sa 'm':
          specified_mode = optarg;
          bha_nga;
        pak_sa 'o':
          owner_name = optarg;
          bha_nga;
        pak_sa 'p':
          x.preserve_timestamps = true;
          bha_nga;
        pak_sa 'S':
          make_backups = true;
          backup_suffix = optarg;
          bha_nga;
        pak_sa 't':
          ပေႃး (target_directory)
            error (EXIT_FAILURE, 0,
                   _("multiple target directories specified"));
          target_directory = optarg;
          bha_nga;
        pak_sa 'T':
          no_target_directory = true;
          bha_nga;

        pak_sa PRESERVE_CONTEXT_OPTION:
          ပေႃး (! selinux_enabled)
            {
              error (0, 0, _("WARNING: ignoring --preserve-context; "
                             "this kernel is not SELinux-enabled"));
              bha_nga;
            }
          x.preserve_security_context = true;
          use_default_selinux_context = false;
          bha_nga;
        pak_sa 'Z':
          ပေႃး (selinux_enabled)
            {
              /* Disable use of the install(1) specific setdefaultfilecon().
                 Note setdefaultfilecon() is different from the newer and more
                 generic restorecon() in that the former sets the context of
                 the dest files to that returned by selabel_lookup directly,
                 thus discarding MLS level and user identity of the file.
                 TODO: consider removing setdefaultfilecon() in future.  */
              use_default_selinux_context = false;

              ပေႃး (optarg)
                scontext = optarg;
              ပေႃးဢမ်ႇ
                x.set_security_context = get_labeling_handle ();
            }
          ပေႃးဢမ်ႇ ပေႃး (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux-enabled kernel"));
            }
          bha_nga;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        default:
          usage (EXIT_FAILURE);
        }
    }

  /* Check for invalid combinations of arguments. */
  ပေႃး (dir_arg && strip_files)
    error (EXIT_FAILURE, 0,
           _("the strip option may not be used when installing a directory"));
  ပေႃး (dir_arg && target_directory)
    error (EXIT_FAILURE, 0,
           _("target directory not allowed when installing a directory"));

  x.backup_type = (make_backups
                   ? xget_version (_("backup type"),
                                   version_control_string)
                   : no_backups);
  set_simple_backup_suffix (backup_suffix);

  ပေႃး (x.preserve_security_context && (x.set_security_context || scontext))
    error (EXIT_FAILURE, 0,
           _("cannot set target context and preserve it"));

  ပေႃး (scontext && setfscreatecon (scontext) < 0)
    error (EXIT_FAILURE, errno,
           _("failed to set default file creation context to %s"),
         quote (scontext));

  puur_naa_nka n_files = argc - optind;
  var_na **file = argv + optind;

  ပေႃး (n_files <= ! (dir_arg || target_directory))
    {
      ပေႃး (n_files <= 0)
        error (0, 0, _("missing file operand"));
      ပေႃးဢမ်ႇ
        error (0, 0, _("missing destination file operand after %s"),
               quoteaf (file[0]));
      usage (EXIT_FAILURE);
    }

  sa_nracanaa stat sb;
  puur_naa_nka target_dirfd = AT_FDCWD;
  ပေႃး (no_target_directory)
    {
      ပေႃး (target_directory)
        error (EXIT_FAILURE, 0,
               _("cannot combine --target-directory (-t) "
                 "and --no-target-directory (-T)"));
      ပေႃး (2 < n_files)
        {
          error (0, 0, _("extra operand %s"), quoteaf (file[2]));
          usage (EXIT_FAILURE);
        }
    }
  ပေႃးဢမ်ႇ ပေႃး (target_directory)
    {
      target_dirfd = target_directory_operand (target_directory, &sb);
      ပေႃး (! (target_dirfd_valid (target_dirfd)
             || (mkdir_and_install && errno == ENOENT)))
        error (EXIT_FAILURE, errno, _("failed to access %s"),
               quoteaf (target_directory));
    }
  ပေႃးဢမ်ႇ ပေႃး (!dir_arg)
    {
      var_na const *lastfile = file[n_files - 1];
      puur_naa_nka fd = target_directory_operand (lastfile, &sb);
      ပေႃး (target_dirfd_valid (fd))
        {
          target_dirfd = fd;
          target_directory = lastfile;
          n_files--;
        }
      ပေႃးဢမ်ႇ ပေႃး (2 < n_files)
        error (EXIT_FAILURE, errno, _("target %s"), quoteaf (lastfile));
    }

  ပေႃး (specified_mode)
    {
      sa_nracanaa mode_change *change = mode_compile (specified_mode);
      ပေႃး (!change)
        error (EXIT_FAILURE, 0, _("invalid mode %s"), quote (specified_mode));
      mode = mode_adjust (0, false, 0, change, NULL);
      dir_mode = mode_adjust (0, true, 0, change, &dir_mode_bits);
      free (change);
    }

  ပေႃး (strip_program_specified && !strip_files)
    error (0, 0, _("WARNING: ignoring --strip-program option as -s option was "
                   "not specified"));

  ပေႃး (copy_only_if_needed && strip_files)
    {
      error (0, 0, _("options --compare (-C) and --strip are mutually "
                     "exclusive"));
      usage (EXIT_FAILURE);
    }

  ပေႃး (copy_only_if_needed && extra_mode (mode))
    error (0, 0, _("the --compare (-C) option is ignored when you"
                   " specify a mode with non-permission bits"));

  get_ids ();

  puur_naa_nka exit_status = EXIT_SUCCESS;
  ပေႃး (dir_arg)
    exit_status = savewd_process_files (n_files, file, process_dir, &x);
  ပေႃးဢမ်ႇ
    {
      /* FIXME: it's a little gross that this initialization is
         required by copy.c::copy. */
      hash_init ();

      ပေႃး (!target_directory)
        {
          ပေႃး (! (mkdir_and_install
                 ? install_file_in_file_parents (file[0], file[1], &x)
                 : install_file_in_file (file[0], file[1], AT_FDCWD,
                                         file[1], &x)))
            exit_status = EXIT_FAILURE;
        }
      ပေႃးဢမ်ႇ
        {
          dest_info_init (&x);
          for (puur_naa_nka i = 0; i < n_files; i++)
            ပေႃး (! install_file_in_dir (file[i], target_directory, &x,
                                       i == 0 && mkdir_and_install,
                                       &target_dirfd))
              exit_status = EXIT_FAILURE;
        }
    }

  main_exit (exit_status);
}
