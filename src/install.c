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

static 정수 selinux_enabled = 0;
static bool use_default_selinux_context = true;

#만약 ! HAVE_ENDGRENT
# define endgrent() ((void) 0)
#endif

#만약 ! HAVE_ENDPWENT
# define endpwent() ((void) 0)
#endif

/* The user name that will own the files, or NULL to make the owner
   the current user ID. */
static char *owner_name;

/* The user ID corresponding to 'owner_name'. */
static uid_t owner_id;

/* The group name that will own the files, or NULL to make the group
   the current group ID. */
static char *group_name;

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
static char const *strip_program = "strip";

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
enum
{
  DEBUG_OPTION = CHAR_MAX + 1,
  PRESERVE_CONTEXT_OPTION,
  STRIP_PROGRAM_OPTION
};

static struct option const long_options[] =
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
have_same_content (정수 a_fd, 정수 b_fd)
{
  enum { CMP_BLOCK_SIZE = 4096 };
  static char a_buff[CMP_BLOCK_SIZE];
  static char b_buff[CMP_BLOCK_SIZE];

  idx_t size;
  동안 (0 < (size = full_read (a_fd, a_buff, sizeof a_buff))) {
    만약 (size != full_read (b_fd, b_buff, sizeof b_buff))
      반환 false;

    만약 (!memeq (a_buff, b_buff, size))
      반환 false;
  }

  반환 size == 0;
}

/* Return true for mode with non-permission bits. */
static bool
extra_mode (mode_t input)
{
  mode_t mask = S_IRWXUGO | S_IFMT;
  반환 !! (input & ~ mask);
}

/* Return true if copy of file SRC_NAME to file DEST_NAME aka
   DEST_DIRFD+DEST_RELNAME is necessary. */
static bool
need_copy (char const *src_name, char const *dest_name,
           정수 dest_dirfd, char const *dest_relname,
           const struct cp_options *x)
{
  만약 (extra_mode (mode))
    반환 true;

  /* compare files using stat */
  struct stat src_sb;
  만약 (stat (src_name, &src_sb) != 0)
    반환 true;

  struct stat dest_sb;
  만약 (fstatat (dest_dirfd, dest_relname, &dest_sb, AT_SYMLINK_NOFOLLOW) != 0)
    반환 true;

  만약 (!S_ISREG (src_sb.st_mode) || !S_ISREG (dest_sb.st_mode)
      || extra_mode (src_sb.st_mode) || extra_mode (dest_sb.st_mode))
    반환 true;

  만약 (src_sb.st_size != dest_sb.st_size
      || (dest_sb.st_mode & CHMOD_MODE_BITS) != mode)
    반환 true;

  만약 (owner_id == (uid_t) -1)
    {
      errno = 0;
      uid_t ruid = getuid ();
      만약 ((ruid == (uid_t) -1 && errno) || dest_sb.st_uid != ruid)
        반환 true;
    }
  아니면 만약 (dest_sb.st_uid != owner_id)
    반환 true;

  만약 (group_id == (uid_t) -1)
    {
      errno = 0;
      gid_t rgid = getgid ();
      만약 ((rgid == (uid_t) -1 && errno) || dest_sb.st_gid != rgid)
        반환 true;
    }
  아니면 만약 (dest_sb.st_gid != group_id)
    반환 true;

  /* compare SELinux context if preserving */
  만약 (selinux_enabled && x->preserve_security_context)
    {
      char *file_scontext_raw = NULL;
      만약 (getfilecon_raw (src_name, &file_scontext_raw) == -1)
        반환 true;

      char *to_scontext_raw = NULL;
      만약 (getfilecon_raw (dest_name, &to_scontext_raw) == -1)
        {
          freecon (file_scontext_raw);
          반환 true;
        }

      bool scontext_match = streq (file_scontext_raw, to_scontext_raw);

      freecon (file_scontext_raw);
      freecon (to_scontext_raw);
      만약 (!scontext_match)
        반환 true;
    }

  /* compare files content */
  정수 src_fd = open (src_name, O_RDONLY | O_BINARY);
  만약 (src_fd < 0)
    반환 true;

  정수 dest_fd = openat (dest_dirfd, dest_relname, O_RDONLY | O_BINARY);
  만약 (dest_fd < 0)
    {
      close (src_fd);
      반환 true;
    }

  bool content_match = have_same_content (src_fd, dest_fd);

  close (src_fd);
  close (dest_fd);
  반환 !content_match;
}

static void
cp_option_init (struct cp_options *x)
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

static struct selabel_handle *
get_labeling_handle (void)
{
  static bool initialized;
  static struct selabel_handle *hnd;
  만약 (!initialized)
    {
      initialized = true;
      hnd = selabel_open (SELABEL_CTX_FILE, NULL, 0);
      만약 (!hnd)
        error (0, errno, _("warning: security labeling handle failed"));
    }
  반환 hnd;
}

/* Modify file context to match the specified policy.
   If an error occurs the file will remain with the default directory
   context.  Note this sets the context to that returned by selabel_lookup
   and thus discards MLS levels and user identity of the FILE.  */
static void
setdefaultfilecon (char const *file)
{
  만약 (selinux_enabled != 1)
    {
      /* Indicate no context found. */
      반환;
    }

  struct stat st;
  만약 (lstat (file, &st) != 0)
    반환;

  struct selabel_handle *hnd = get_labeling_handle ();
  만약 (!hnd)
    반환;

  char *scontext_raw = NULL;
  만약 (selabel_lookup_raw (hnd, &scontext_raw, file, st.st_mode) != 0)
    {
      만약 (errno != ENOENT && ! ignorable_ctx_err (errno))
        error (0, errno, _("warning: %s: context lookup failed"),
               quotef (file));
      반환;
    }

  만약 (lsetfilecon_raw (file, scontext_raw) < 0 && errno != ENOTSUP)
    error (0, errno,
           _("warning: %s: failed to change context to %s"),
           quotef_n (0, file), quote_n (1, scontext_raw));

  freecon (scontext_raw);
}

/* Report that directory DIR was made, if OPTIONS requests this.  */
static void
announce_mkdir (char const *dir, void *options)
{
  struct cp_options const *x = options;
  만약 (x->verbose)
    prog_fprintf (stdout, _("creating directory %s"), quoteaf (dir));
}

/* Make ancestor directory DIR, whose last file name component is
   COMPONENT, with options OPTIONS.  Assume the working directory is
   COMPONENT's parent.  */
static 정수
make_ancestor (char const *dir, char const *component, void *options)
{
  struct cp_options const *x = options;
  만약 (x->set_security_context
      && defaultcon (x->set_security_context, component, S_IFDIR) < 0
      && ! ignorable_ctx_err (errno))
    error (0, errno, _("failed to set default creation context for %s"),
           quoteaf (dir));

  정수 r = mkdir (component, DEFAULT_MODE);
  만약 (r == 0)
    announce_mkdir (dir, options);
  반환 r;
}

/* Process a command-line file name, for the -d option.  */
static 정수
process_dir (char *dir, struct savewd *wd, void *options)
{
  struct cp_options const *x = options;

  정수 ret = (make_dir_parents (dir, wd, make_ancestor, options,
                               dir_mode, announce_mkdir,
                               dir_mode_bits, owner_id, group_id, false)
          ? EXIT_SUCCESS
          : EXIT_FAILURE);

  /* FIXME: Due to the current structure of make_dir_parents()
     we don't have the facility to call defaultcon() before the
     final component of DIR is created.  So for now, create the
     final component with the context from previous component
     and here we set the context for the final component. */
  만약 (ret == EXIT_SUCCESS && x->set_security_context)
    {
      만약 (! restorecon (x->set_security_context, last_component (dir), false)
          && ! ignorable_ctx_err (errno))
        error (0, errno, _("failed to restore context for %s"),
               quoteaf (dir));
    }

  반환 ret;
}

enum copy_status { COPY_FAILED, COPY_OK, COPY_SKIPPED };

/* Copy file FROM onto file TO aka TO_DIRFD+TO_RELNAME, creating TO if
   necessary.  Return COPY_OK if successful or COPY_SKIPPED if the copy
   was skipped.  */

static enum copy_status
copy_file (char const *from, char const *to,
           정수 to_dirfd, char const *to_relname, const struct cp_options *x)
{
  만약 (copy_only_if_needed && !need_copy (from, to, to_dirfd, to_relname, x))
    반환 COPY_SKIPPED;

  /* Allow installing from non-regular files like /dev/null.
     Charles Karney reported that some Sun version of install allows that
     and that sendmail's installation process relies on the behavior.
     However, since !x->recursive, the call to "copy" will fail if FROM
     is a directory.  */

  bool copy_into_self;
  반환 (copy (from, to, to_dirfd, to_relname, 0, x, &copy_into_self, NULL)
          ? COPY_OK : COPY_FAILED);
}

/* Set the attributes of file or directory NAME aka DIRFD+RELNAME.
   Return true if successful.  */

static bool
change_attributes (char const *name, 정수 dirfd, char const *relname)
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

  만약 (! (owner_id == (uid_t) -1 && group_id == (gid_t) -1)
      && lchownat (dirfd, relname, owner_id, group_id) != 0)
    error (0, errno, _("cannot change ownership of %s"), quoteaf (name));
  아니면 만약 (chmodat (dirfd, relname, mode) != 0)
    error (0, errno, _("cannot change permissions of %s"), quoteaf (name));
  아니면
    ok = true;

  만약 (use_default_selinux_context)
    setdefaultfilecon (name);

  반환 ok;
}

/* Set the timestamps of file DEST aka DIRFD+RELNAME to match those of SRC_SB.
   Return true if successful.  */

static bool
change_timestamps (struct stat const *src_sb, char const *dest,
                   정수 dirfd, char const *relname)
{
  struct timespec timespec[2] = { get_stat_atime (src_sb),
                                  get_stat_mtime (src_sb) };

  만약 (utimensat (dirfd, relname, timespec, 0))
    {
      error (0, errno, _("cannot set timestamps for %s"), quoteaf (dest));
      반환 false;
    }
  반환 true;
}

/* Strip the symbol table from the file NAME.
   We could dig the magic number out of the file first to
   determine whether to strip it, but the header files and
   magic numbers vary so much from system to system that making
   it portable would be very difficult.  Not worth the effort. */

static bool
strip (char const *name)
{
  posix_spawnattr_t attr;
  posix_spawnattr_t *attrp = NULL;

  /* Try to use vfork for systems where it matters.  */
  만약 (posix_spawnattr_init (&attr) == 0)
    {
      만약 (posix_spawnattr_setflags (&attr, POSIX_SPAWN_USEVFORK) == 0)
        attrp = &attr;
      아니면
        posix_spawnattr_destroy (&attr);
    }

  /* Construct the arguments to 'strip'.  */
  char *concat_name = NULL;
  char const *safe_name = name;
  만약 (name && *name == '-')
    safe_name = concat_name = file_name_concat (".", name, NULL);
  char const *const argv[] = { strip_program, safe_name, NULL };

  /* Run 'strip'.  */
  pid_t pid;
  정수 result = posix_spawnp (&pid, strip_program, NULL, attrp,
                             (char * const *) argv, environ);

  bool ok = false;
  만약 (result != 0)
    {
      error (0, result,
             streq (strip_program, "strip")
               ? _("cannot run %s")
               : _("cannot run strip program %s"),
             quoteaf (strip_program));
    }
  아니면
    {
      /* Wait for 'strip' to complete, and emit a warning message on failure  */
      정수 status;
      만약 (waitpid (pid, &status, 0) < 0)
        error (0, errno, _("waiting for strip"));
      아니면 만약 (! WIFEXITED (status) || WEXITSTATUS (status))
        error (0, 0, _("strip process terminated abnormally"));
      아니면
        ok = true;
    }

  free (concat_name);
  만약 (attrp)
    posix_spawnattr_destroy (attrp);

  반환 ok;
}

/* Initialize the user and group ownership of the files to install. */

static void
get_ids (void)
{
  만약 (owner_name)
    {
      struct passwd *pw = getpwnam (owner_name);
      만약 (pw == NULL)
        {
          uintmax_t tmp;
          만약 (xstrtoumax (owner_name, NULL, 0, &tmp, "") != LONGINT_OK
              || ckd_add (&owner_id, tmp, 0))
            error (EXIT_FAILURE, 0, _("invalid user %s"),
                   quoteaf (owner_name));
        }
      아니면
        owner_id = pw->pw_uid;
      endpwent ();
    }
  아니면
    owner_id = (uid_t) -1;

  만약 (group_name)
    {
      struct group *gr = getgrnam (group_name);
      만약 (gr == NULL)
        {
          uintmax_t tmp;
          만약 (xstrtoumax (group_name, NULL, 0, &tmp, "") != LONGINT_OK
              || ckd_add (&group_id, tmp, 0))
            error (EXIT_FAILURE, 0, _("invalid group %s"),
                   quoteaf (group_name));
        }
      아니면
        group_id = gr->gr_gid;
      endgrent ();
    }
  아니면
    group_id = (gid_t) -1;
}

void
usage (정수 status)
{
  만약 (status != EXIT_SUCCESS)
    emit_try_help ();
  아니면
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
the existing DIRECTORY, 동안 setting permission modes and owner/group.\n\
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
         and 만약 no change to content, ownership, and permissions,\n\
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
         like -Z, or 만약 CTX is specified then set the\n\
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
install_file_in_file (char const *from, char const *to,
                      정수 to_dirfd, char const *to_relname,
                      const struct cp_options *x)
{
  struct stat from_sb;
  만약 (x->preserve_timestamps && stat (from, &from_sb) != 0)
    {
      error (0, errno, _("cannot stat %s"), quoteaf (from));
      반환 false;
    }
  enum copy_status copy_status = copy_file (from, to, to_dirfd, to_relname, x);
  만약 (copy_status == COPY_FAILED)
    반환 false;
  만약 (copy_status == COPY_OK && strip_files && ! strip (to))
    {
      만약 (unlinkat (to_dirfd, to_relname, 0) != 0)  /* Cleanup.  */
        error (EXIT_FAILURE, errno, _("cannot unlink %s"), quoteaf (to));
      반환 false;
    }
  만약 (x->preserve_timestamps && (copy_status == COPY_SKIPPED || strip_files
                                 || ! S_ISREG (from_sb.st_mode))
      && ! change_timestamps (&from_sb, to, to_dirfd, to_relname))
    반환 false;

  반환 copy_status == COPY_SKIPPED
         || change_attributes (to, to_dirfd, to_relname);
}

/* Create any missing parent directories of TO,
   while maintaining the current Working Directory.
   Return true if successful.  */

static bool
mkancesdirs_safe_wd (char const *from, char *to, struct cp_options *x,
                     bool save_always)
{
  bool save_working_directory =
    save_always
    || ! (IS_ABSOLUTE_FILE_NAME (from) && IS_ABSOLUTE_FILE_NAME (to));

  struct savewd wd;
  savewd_init (&wd);
  만약 (! save_working_directory)
    savewd_finish (&wd);

  정수 status = EXIT_SUCCESS;
  만약 (mkancesdirs (to, &wd, make_ancestor, x) == -1)
    {
      error (0, errno, _("cannot create directory %s"), quoteaf (to));
      status = EXIT_FAILURE;
    }

  만약 (save_working_directory)
    {
      정수 restore_result = savewd_restore (&wd, status);
      정수 restore_errno = errno;
      savewd_finish (&wd);
      만약 (EXIT_SUCCESS < restore_result)
        반환 false;
      만약 (restore_result < 0 && status == EXIT_SUCCESS)
        {
          error (0, restore_errno, _("cannot create directory %s"),
                 quoteaf (to));
          반환 false;
        }
    }
  반환 status == EXIT_SUCCESS;
}

/* Copy file FROM onto file TO, creating any missing parent directories of TO.
   Return true if successful.  */

static bool
install_file_in_file_parents (char const *from, char *to,
                              const struct cp_options *x)
{
  반환 (mkancesdirs_safe_wd (from, to, (struct cp_options *)x, false)
          && install_file_in_file (from, to, AT_FDCWD, to, x));
}

/* Copy file FROM into directory TO_DIR, keeping its same name,
   and give the copy the appropriate attributes.
   Return true if successful.  */

static bool
install_file_in_dir (char const *from, char const *to_dir,
                     const struct cp_options *x, bool mkdir_and_install,
                     정수 *target_dirfd)
{
  char const *from_base = last_component (from);
  char *to_relname;
  char *to = file_name_concat (to_dir, from_base, &to_relname);
  bool ret = true;

  만약 (!target_dirfd_valid (*target_dirfd)
      && (ret = mkdir_and_install)
      && (ret = mkancesdirs_safe_wd (from, to, (struct cp_options *) x, true)))
    {
      정수 fd = open (to_dir, O_PATHSEARCH | O_DIRECTORY);
      만약 (fd < 0)
        {
          error (0, errno, _("cannot open %s"), quoteaf (to));
          ret = false;
        }
      아니면
        *target_dirfd = fd;
    }

  만약 (ret)
    {
      정수 to_dirfd = *target_dirfd;
      만약 (!target_dirfd_valid (to_dirfd))
        {
          to_dirfd = AT_FDCWD;
          to_relname = to;
        }
      ret = install_file_in_file (from, to, to_dirfd, to_relname, x);
    }

  free (to);
  반환 ret;
}

정수
main (정수 argc, char **argv)
{
  char const *specified_mode = NULL;
  bool make_backups = false;
  char const *backup_suffix = NULL;
  char *version_control_string = NULL;
  bool mkdir_and_install = false;
  struct cp_options x;
  char const *target_directory = NULL;
  bool no_target_directory = false;
  bool strip_program_specified = false;
  char const *scontext = NULL;
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

  정수 optc;
  동안 ((optc = getopt_long (argc, argv, "bcCsDdg:m:o:pt:TvS:Z", long_options,
                              NULL))
         != -1)
    {
      switch (optc)
        {
        case 'b':
          make_backups = true;
          만약 (optarg)
            version_control_string = optarg;
          break;
        case 'c':
          break;
        case 'C':
          copy_only_if_needed = true;
          break;
        case 's':
          strip_files = true;
#ifdef SIGCHLD
          /* System V fork+wait does not work if SIGCHLD is ignored.  */
          signal (SIGCHLD, SIG_DFL);
#endif
          break;
        case DEBUG_OPTION:
          x.debug = x.verbose = true;
          break;
        case STRIP_PROGRAM_OPTION:
          strip_program = xstrdup (optarg);
          strip_program_specified = true;
          break;
        case 'd':
          dir_arg = true;
          break;
        case 'D':
          mkdir_and_install = true;
          break;
        case 'v':
          x.verbose = true;
          break;
        case 'g':
          group_name = optarg;
          break;
        case 'm':
          specified_mode = optarg;
          break;
        case 'o':
          owner_name = optarg;
          break;
        case 'p':
          x.preserve_timestamps = true;
          break;
        case 'S':
          make_backups = true;
          backup_suffix = optarg;
          break;
        case 't':
          만약 (target_directory)
            error (EXIT_FAILURE, 0,
                   _("multiple target directories specified"));
          target_directory = optarg;
          break;
        case 'T':
          no_target_directory = true;
          break;

        case PRESERVE_CONTEXT_OPTION:
          만약 (! selinux_enabled)
            {
              error (0, 0, _("WARNING: ignoring --preserve-context; "
                             "this kernel is not SELinux-enabled"));
              break;
            }
          x.preserve_security_context = true;
          use_default_selinux_context = false;
          break;
        case 'Z':
          만약 (selinux_enabled)
            {
              /* Disable use of the install(1) specific setdefaultfilecon().
                 Note setdefaultfilecon() is different from the newer and more
                 generic restorecon() in that the former sets the context of
                 the dest files to that returned by selabel_lookup directly,
                 thus discarding MLS level and user identity of the file.
                 TODO: consider removing setdefaultfilecon() in future.  */
              use_default_selinux_context = false;

              만약 (optarg)
                scontext = optarg;
              아니면
                x.set_security_context = get_labeling_handle ();
            }
          아니면 만약 (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux-enabled kernel"));
            }
          break;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        default:
          usage (EXIT_FAILURE);
        }
    }

  /* Check for invalid combinations of arguments. */
  만약 (dir_arg && strip_files)
    error (EXIT_FAILURE, 0,
           _("the strip option may not be used when installing a directory"));
  만약 (dir_arg && target_directory)
    error (EXIT_FAILURE, 0,
           _("target directory not allowed when installing a directory"));

  x.backup_type = (make_backups
                   ? xget_version (_("backup type"),
                                   version_control_string)
                   : no_backups);
  set_simple_backup_suffix (backup_suffix);

  만약 (x.preserve_security_context && (x.set_security_context || scontext))
    error (EXIT_FAILURE, 0,
           _("cannot set target context and preserve it"));

  만약 (scontext && setfscreatecon (scontext) < 0)
    error (EXIT_FAILURE, errno,
           _("failed to set default file creation context to %s"),
         quote (scontext));

  정수 n_files = argc - optind;
  char **file = argv + optind;

  만약 (n_files <= ! (dir_arg || target_directory))
    {
      만약 (n_files <= 0)
        error (0, 0, _("missing file operand"));
      아니면
        error (0, 0, _("missing destination file operand after %s"),
               quoteaf (file[0]));
      usage (EXIT_FAILURE);
    }

  struct stat sb;
  정수 target_dirfd = AT_FDCWD;
  만약 (no_target_directory)
    {
      만약 (target_directory)
        error (EXIT_FAILURE, 0,
               _("cannot combine --target-directory (-t) "
                 "and --no-target-directory (-T)"));
      만약 (2 < n_files)
        {
          error (0, 0, _("extra operand %s"), quoteaf (file[2]));
          usage (EXIT_FAILURE);
        }
    }
  아니면 만약 (target_directory)
    {
      target_dirfd = target_directory_operand (target_directory, &sb);
      만약 (! (target_dirfd_valid (target_dirfd)
             || (mkdir_and_install && errno == ENOENT)))
        error (EXIT_FAILURE, errno, _("failed to access %s"),
               quoteaf (target_directory));
    }
  아니면 만약 (!dir_arg)
    {
      char const *lastfile = file[n_files - 1];
      정수 fd = target_directory_operand (lastfile, &sb);
      만약 (target_dirfd_valid (fd))
        {
          target_dirfd = fd;
          target_directory = lastfile;
          n_files--;
        }
      아니면 만약 (2 < n_files)
        error (EXIT_FAILURE, errno, _("target %s"), quoteaf (lastfile));
    }

  만약 (specified_mode)
    {
      struct mode_change *change = mode_compile (specified_mode);
      만약 (!change)
        error (EXIT_FAILURE, 0, _("invalid mode %s"), quote (specified_mode));
      mode = mode_adjust (0, false, 0, change, NULL);
      dir_mode = mode_adjust (0, true, 0, change, &dir_mode_bits);
      free (change);
    }

  만약 (strip_program_specified && !strip_files)
    error (0, 0, _("WARNING: ignoring --strip-program option as -s option was "
                   "not specified"));

  만약 (copy_only_if_needed && strip_files)
    {
      error (0, 0, _("options --compare (-C) and --strip are mutually "
                     "exclusive"));
      usage (EXIT_FAILURE);
    }

  만약 (copy_only_if_needed && extra_mode (mode))
    error (0, 0, _("the --compare (-C) option is ignored when you"
                   " specify a mode with non-permission bits"));

  get_ids ();

  정수 exit_status = EXIT_SUCCESS;
  만약 (dir_arg)
    exit_status = savewd_process_files (n_files, file, process_dir, &x);
  아니면
    {
      /* FIXME: it's a little gross that this initialization is
         required by copy.c::copy. */
      hash_init ();

      만약 (!target_directory)
        {
          만약 (! (mkdir_and_install
                 ? install_file_in_file_parents (file[0], file[1], &x)
                 : install_file_in_file (file[0], file[1], AT_FDCWD,
                                         file[1], &x)))
            exit_status = EXIT_FAILURE;
        }
      아니면
        {
          dest_info_init (&x);
          위해 (정수 i = 0; i < n_files; i++)
            만약 (! install_file_in_dir (file[i], target_directory, &x,
                                       i == 0 && mkdir_and_install,
                                       &target_dirfd))
              exit_status = EXIT_FAILURE;
        }
    }

  main_exit (exit_status);
}
