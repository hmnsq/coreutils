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
#وصف_ڪريو پروگرام_NAME "install"

#وصف_ڪريو AUTHORS proper_name ("David MacKenzie")

ساكن صحيح selinux_enabled = 0;
ساكن bool use_default_selinux_context = true;

#إذا ! HAVE_ENDGRENT
# define endgrent() ((فراغ) 0)
#جيڪڏهن_پورو

#إذا ! HAVE_ENDPWENT
# define endpwent() ((فراغ) 0)
#جيڪڏهن_پورو

/* The user name that will own the files, or NULL to make the owner
   the current user ID. */
ساكن حرف *owner_name;

/* The user ID corresponding to 'owner_name'. */
ساكن uid_t owner_id;

/* The group name that will own the files, or NULL to make the group
   the current group ID. */
ساكن حرف *گروهه_name;

/* The group ID corresponding to 'group_name'. */
ساكن gid_t گروهه_id;

#وصف_ڪريو DEFAULT_گهڻائي (S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)

/* The file mode bits to which non-directory files will be set.  The umask has
   no effect. */
ساكن mode_t گهڻائي_2 = DEFAULT_گهڻائي;

/* Similar, but for directories.  */
ساكن mode_t dir_گهڻائي = DEFAULT_گهڻائي;

/* The file mode bits that the user cares about.  This should be a
   superset of DIR_MODE and a subset of CHMOD_MODE_BITS.  This matters
   for directories, since otherwise directories may keep their S_ISUID
   or S_ISGID bits.  */
ساكن mode_t dir_گهڻائي_بٽ = CHMOD_گهڻائي_بٽ;

/* Compare files before installing (-C) */
ساكن bool copy_only_if_needed;

/* If true, strip executable files after copying them. */
ساكن bool strip_files;

/* If true, install a directory instead of a regular file. */
ساكن bool dir_arg;

/* Program used to strip binaries, "strip" is default */
ساكن حرف ثابت *strip_پروگرام = "strip";

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
تعداد
{
  DEBUG_OPTION = CHAR_MAX + 1,
  PRESERVE_CONTEXT_OPTION,
  STRIP_پروگرام_OPTION
};

ساكن بنية option ثابت long_options[] =
{
  {"backup", optional_دليل, عدم, 'b'},
  {"compare", no_دليل, عدم, 'C'},
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {"debug", no_دليل, عدم, DEBUG_OPTION},
  {"directory", no_دليل, عدم, 'd'},
  {"group", required_دليل, عدم, 'g'},
  {"mode", required_دليل, عدم, 'm'},
  {"no-target-directory", no_دليل, عدم, 'T'},
  {"owner", required_دليل, عدم, 'o'},
  {"preserve-timestamps", no_دليل, عدم, 'p'},
  {"preserve-context", no_دليل, عدم, PRESERVE_CONTEXT_OPTION},
  {"strip", no_دليل, عدم, 's'},
  {"strip-program", required_دليل, عدم, STRIP_پروگرام_OPTION},
  {"suffix", required_دليل, عدم, 'S'},
  {"target-directory", required_دليل, عدم, 't'},
  {"verbose", no_دليل, عدم, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخو_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

/* Compare content of opened files using file descriptors A_FD and B_FD. Return
   true if files are equal. */
ساكن bool
have_same_content (صحيح a_fd, صحيح b_fd)
{
  تعداد { CMP_بلاڪ_سائيز = 4096 };
  ساكن حرف a_buff[CMP_بلاڪ_سائيز];
  ساكن حرف b_buff[CMP_بلاڪ_سائيز];

  idx_t سائيز;
  طالما (0 < (سائيز = ڀريل_read (a_fd, a_buff, حجم a_buff))) {
    إذا (سائيز != ڀريل_read (b_fd, b_buff, حجم b_buff))
      أرجع false;

    إذا (!memeq (a_buff, b_buff, سائيز))
      أرجع false;
  }

  أرجع سائيز == 0;
}

/* Return true for mode with non-permission bits. */
ساكن bool
extra_گهڻائي (mode_t داخلا)
{
  mode_t نقاب = S_IRWXUGO | S_IFMT;
  أرجع !! (داخلا & ~ نقاب);
}

/* Return true if copy of file SRC_NAME to file DEST_NAME aka
   DEST_DIRFD+DEST_RELNAME is necessary. */
ساكن bool
need_copy (حرف ثابت *src_name, حرف ثابت *dest_name,
           صحيح dest_dirfd, حرف ثابت *dest_relname,
           ثابت بنية cp_options *x)
{
  إذا (extra_گهڻائي (گهڻائي_2))
    أرجع true;

  /* compare files using stat */
  بنية stat src_sb;
  إذا (stat (src_name, &src_sb) != 0)
    أرجع true;

  بنية stat dest_sb;
  إذا (fstatat (dest_dirfd, dest_relname, &dest_sb, AT_SYMLINK_NOFOLLOW) != 0)
    أرجع true;

  إذا (!S_ISREG (src_sb.st_گهڻائي) || !S_ISREG (dest_sb.st_گهڻائي)
      || extra_گهڻائي (src_sb.st_گهڻائي) || extra_گهڻائي (dest_sb.st_گهڻائي))
    أرجع true;

  إذا (src_sb.st_سائيز != dest_sb.st_سائيز
      || (dest_sb.st_گهڻائي & CHMOD_گهڻائي_بٽ) != گهڻائي_2)
    أرجع true;

  إذا (owner_id == (uid_t) -1)
    {
      errno = 0;
      uid_t ruid = getuid ();
      إذا ((ruid == (uid_t) -1 && errno) || dest_sb.st_uid != ruid)
        أرجع true;
    }
  وإلا إذا (dest_sb.st_uid != owner_id)
    أرجع true;

  إذا (گروهه_id == (uid_t) -1)
    {
      errno = 0;
      gid_t rgid = getgid ();
      إذا ((rgid == (uid_t) -1 && errno) || dest_sb.st_gid != rgid)
        أرجع true;
    }
  وإلا إذا (dest_sb.st_gid != گروهه_id)
    أرجع true;

  /* compare SELinux context if preserving */
  إذا (selinux_enabled && x->preserve_security_context)
    {
      حرف *file_scontext_raw = عدم;
      إذا (getfilecon_raw (src_name, &file_scontext_raw) == -1)
        أرجع true;

      حرف *منزل_scontext_raw = عدم;
      إذا (getfilecon_raw (dest_name, &منزل_scontext_raw) == -1)
        {
          freecon (file_scontext_raw);
          أرجع true;
        }

      bool scontext_ميلاپ = streq (file_scontext_raw, منزل_scontext_raw);

      freecon (file_scontext_raw);
      freecon (منزل_scontext_raw);
      إذا (!scontext_ميلاپ)
        أرجع true;
    }

  /* compare files content */
  صحيح src_fd = open (src_name, O_RDONLY | O_ٻٽو);
  إذا (src_fd < 0)
    أرجع true;

  صحيح dest_fd = openat (dest_dirfd, dest_relname, O_RDONLY | O_ٻٽو);
  إذا (dest_fd < 0)
    {
      close (src_fd);
      أرجع true;
    }

  bool content_ميلاپ = have_same_content (src_fd, dest_fd);

  close (src_fd);
  close (dest_fd);
  أرجع !content_ميلاپ;
}

ساكن فراغ
cp_option_init (بنية cp_options *x)
{
  cp_options_default (x);
  x->copy_as_regular = true;
  x->reflink_گهڻائي = REFLINK_AUTO;
  x->dereference = DEREF_ALWAYS;
  x->unlink_dest_before_opening = true;
  x->unlink_dest_after_failed_open = false;
  x->hard_link = false;
  x->interactive = I_UNSPECIFIED;
  x->move_گهڻائي = false;
  x->install_گهڻائي = true;
  x->one_file_system = false;
  x->preserve_ownership = false;
  x->preserve_links = false;
  x->preserve_گهڻائي = false;
  x->preserve_timestamps = false;
  x->explicit_no_preserve_گهڻائي = false;
  x->reduce_diagnostics=false;
  x->معلومات_copy_required = true;
  x->require_preserve = false;
  x->require_preserve_xattr = false;
  x->recursive = false;
  x->sparse_گهڻائي = SPARSE_AUTO;
  x->symbolic_link = false;
  x->backup_type = no_backups;

  /* Create destination files initially writable so we can run strip on them.
     Although GNU strip works fine on read-only files, some others
     would fail.  */
  x->سيٽ_گهڻائي = true;
  x->گهڻائي_2 = S_IRUSR | S_IWUSR;
  x->stdin_tty = false;

  x->open_dangling_dest_symlink = false;
  x->update = UPDATE_ALL;
  x->require_preserve_context = false;  /* Not used by install currently.  */
  x->preserve_security_context = false; /* Whether to copy context from src.  */
  x->سيٽ_security_context = عدم; /* Whether to set sys default context.  */
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
  إذا (!initialized)
    {
      initialized = true;
      hnd = selabel_open (SELABEL_CTX_FILE, عدم, 0);
      إذا (!hnd)
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
  إذا (selinux_enabled != 1)
    {
      /* Indicate no context found. */
      أرجع;
    }

  بنية stat st;
  إذا (lstat (file, &st) != 0)
    أرجع;

  بنية selabel_handle *hnd = get_labeling_handle ();
  إذا (!hnd)
    أرجع;

  حرف *scontext_raw = عدم;
  إذا (selabel_lookup_raw (hnd, &scontext_raw, file, st.st_گهڻائي) != 0)
    {
      إذا (errno != ENOENT && ! ignorable_ctx_err (errno))
        error (0, errno, _("warning: %s: context lookup failed"),
               quotef (file));
      أرجع;
    }

  إذا (lsetfilecon_raw (file, scontext_raw) < 0 && errno != ENOTSUP)
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
  إذا (x->verbose)
    prog_fprintf (stdout, _("creating directory %s"), quoteaf (dir));
}

/* Make ancestor directory DIR, whose last file name component is
   COMPONENT, with options OPTIONS.  Assume the working directory is
   COMPONENT's parent.  */
ساكن صحيح
make_ancestor (حرف ثابت *dir, حرف ثابت *component, فراغ *options)
{
  بنية cp_options ثابت *x = options;
  إذا (x->سيٽ_security_context
      && defaultcon (x->سيٽ_security_context, component, S_IFDIR) < 0
      && ! ignorable_ctx_err (errno))
    error (0, errno, _("failed to set default creation context for %s"),
           quoteaf (dir));

  صحيح r = mkdir (component, DEFAULT_گهڻائي);
  إذا (r == 0)
    announce_mkdir (dir, options);
  أرجع r;
}

/* Process a command-line file name, for the -d option.  */
ساكن صحيح
process_dir (حرف *dir, بنية savewd *wd, فراغ *options)
{
  بنية cp_options ثابت *x = options;

  صحيح ret = (make_dir_parents (dir, wd, make_ancestor, options,
                               dir_گهڻائي, announce_mkdir,
                               dir_گهڻائي_بٽ, owner_id, گروهه_id, false)
          ? EXIT_SUCCESS
          : EXIT_FAILURE);

  /* FIXME: Due to the current structure of make_dir_parents()
     we don't have the facility to call defaultcon() before the
     final component of DIR is created.  So for now, create the
     final component with the context from previous component
     and here we set the context for the final component. */
  إذا (ret == EXIT_SUCCESS && x->سيٽ_security_context)
    {
      إذا (! restorecon (x->سيٽ_security_context, آخري_component (dir), false)
          && ! ignorable_ctx_err (errno))
        error (0, errno, _("failed to restore context for %s"),
               quoteaf (dir));
    }

  أرجع ret;
}

تعداد copy_حالت { COPY_FAILED, COPY_OK, COPY_SKIPPED };

/* Copy file FROM onto file TO aka TO_DIRFD+TO_RELNAME, creating TO if
   necessary.  Return COPY_OK if successful or COPY_SKIPPED if the copy
   was skipped.  */

ساكن تعداد copy_حالت
copy_file (حرف ثابت *from, حرف ثابت *منزل,
           صحيح منزل_dirfd, حرف ثابت *منزل_relname, ثابت بنية cp_options *x)
{
  إذا (copy_only_if_needed && !need_copy (from, منزل, منزل_dirfd, منزل_relname, x))
    أرجع COPY_SKIPPED;

  /* Allow installing from non-regular files like /dev/null.
     Charles Karney reported that some Sun version of install allows that
     and that sendmail's installation process relies on the behavior.
     However, since !x->recursive, the call to "copy" will fail if FROM
     is a directory.  */

  bool copy_into_self;
  أرجع (copy (from, منزل, منزل_dirfd, منزل_relname, 0, x, &copy_into_self, عدم)
          ? COPY_OK : COPY_FAILED);
}

/* Set the attributes of file or directory NAME aka DIRFD+RELNAME.
   Return true if successful.  */

ساكن bool
change_خاصيتون (حرف ثابت *name, صحيح dirfd, حرف ثابت *relname)
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

  إذا (! (owner_id == (uid_t) -1 && گروهه_id == (gid_t) -1)
      && lchownat (dirfd, relname, owner_id, گروهه_id) != 0)
    error (0, errno, _("cannot change ownership of %s"), quoteaf (name));
  وإلا إذا (chmodat (dirfd, relname, گهڻائي_2) != 0)
    error (0, errno, _("cannot change permissions of %s"), quoteaf (name));
  وإلا
    ok = true;

  إذا (use_default_selinux_context)
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

  إذا (utimensat (dirfd, relname, timespec, 0))
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
  إذا (posix_spawnattr_init (&attr) == 0)
    {
      إذا (posix_spawnattr_setflags (&attr, POSIX_SPAWN_USEVFORK) == 0)
        attrp = &attr;
      وإلا
        posix_spawnattr_destroy (&attr);
    }

  /* Construct the arguments to 'strip'.  */
  حرف *concat_name = عدم;
  حرف ثابت *محفوظ_name = name;
  إذا (name && *name == '-')
    محفوظ_name = concat_name = file_name_concat (".", name, عدم);
  حرف ثابت *ثابت argv[] = { strip_پروگرام, محفوظ_name, عدم };

  /* Run 'strip'.  */
  pid_t pid;
  صحيح نتيجو = posix_spawnp (&pid, strip_پروگرام, عدم, attrp,
                             (حرف * ثابت *) argv, environ);

  bool ok = false;
  إذا (نتيجو != 0)
    {
      error (0, نتيجو,
             streq (strip_پروگرام, "strip")
               ? _("cannot run %s")
               : _("cannot run strip program %s"),
             quoteaf (strip_پروگرام));
    }
  وإلا
    {
      /* Wait for 'strip' to complete, and emit a warning message on failure  */
      صحيح حالت;
      إذا (waitpid (pid, &حالت, 0) < 0)
        error (0, errno, _("waiting for strip"));
      وإلا إذا (! WIFEXITED (حالت) || WEXITSTATUS (حالت))
        error (0, 0, _("strip process terminated abnormally"));
      وإلا
        ok = true;
    }

  حرر (concat_name);
  إذا (attrp)
    posix_spawnattr_destroy (attrp);

  أرجع ok;
}

/* Initialize the user and group ownership of the files to install. */

ساكن فراغ
get_ids (فراغ)
{
  إذا (owner_name)
    {
      بنية passwd *pw = getpwnam (owner_name);
      إذا (pw == عدم)
        {
          uintmax_t tmp;
          إذا (xstrtoumax (owner_name, عدم, 0, &tmp, "") != LONGINT_OK
              || ckd_جوڙيو (&owner_id, tmp, 0))
            error (EXIT_FAILURE, 0, _("invalid user %s"),
                   quoteaf (owner_name));
        }
      وإلا
        owner_id = pw->pw_uid;
      endpwent ();
    }
  وإلا
    owner_id = (uid_t) -1;

  إذا (گروهه_name)
    {
      بنية گروهه *gr = getgrnam (گروهه_name);
      إذا (gr == عدم)
        {
          uintmax_t tmp;
          إذا (xstrtoumax (گروهه_name, عدم, 0, &tmp, "") != LONGINT_OK
              || ckd_جوڙيو (&گروهه_id, tmp, 0))
            error (EXIT_FAILURE, 0, _("invalid group %s"),
                   quoteaf (گروهه_name));
        }
      وإلا
        گروهه_id = gr->gr_gid;
      endgrent ();
    }
  وإلا
    گروهه_id = (gid_t) -1;
}

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکو (_("\
Usage: %s [OPTION]... [-T] ذريعو DEST\n\
  or:  %s [OPTION]... ذريعو... DIRECTORY\n\
  or:  %s [OPTION]... -t DIRECTORY ذريعو...\n\
  or:  %s [OPTION]... -d DIRECTORY...\n\
"),
              پروگرام_name, پروگرام_name, پروگرام_name, پروگرام_name);
      fputs (_("\
\n\
This install پروگرام copies files (often just compiled) into destination\n\
locations you چونڊيو.  If you want منزل download and install a ready-منزل-use\n\
package on a GNU/Linux system, you should instead be using a package manager\n\
like yum(1) or apt-get(1).\n\
\n\
اندريون_2 the پهريون three forms, copy ذريعو منزل DEST or گڻو ذريعو(s) منزل\n\
the existing DIRECTORY, طالما setting permission modes and owner/گروهه.\n\
اندريون_2 the 4th form, create all components جو the given DIRECTORY(ies).\n\
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
  -c\n\
         (ignored)\n\
"));
      oputs (_("\
  -C, --compare\n\
         compare content جو ذريعو_2 and destination files,\n\
         and إذا no change منزل content, ownership, and permissions,\n\
         افعل not modify the destination at all\n\
"));
      oputs (_("\
  -d, --directory\n\
         treat all arguments as directory نالا;\n\
         create all components جو the specified directories\n\
"));
      oputs (_("\
  -D\n\
         create all leading components جو DEST except the آخري,\n\
         or all components جو --هدف-directory,\n\
         then copy ذريعو منزل DEST\n\
"));
      oputs (_("\
      --debug\n\
         explain how a file is copied.  نتيجو_ڏئي_2 -v\n\
"));
      oputs (_("\
  -g, --گروهه=گروهه_2\n\
         سيٽ_2 گروهه ownership, instead جو process' current گروهه\n\
"));
      oputs (_("\
  -m, --گهڻائي_2=گهڻائي\n\
         سيٽ_2 permission گهڻائي_2 (as اندريون chmod), instead جو rwxr-xr-x\n\
"));
      oputs (_("\
  -o, --owner=OWNER\n\
         سيٽ_2 ownership (super-user only)\n\
"));
      oputs (_("\
  -p, --preserve-timestamps\n\
         apply access/modification times جو ذريعو files\n\
         منزل corresponding destination files\n\
"));
      oputs (_("\
  -s, --strip\n\
         strip نشان tables\n\
"));
      oputs (_("\
      --strip-پروگرام=پروگرام_2\n\
         پروگرام استعمال_ٿيل منزل strip binaries\n\
"));
      oputs (_("\
  -S, --suffix=SUFFIX\n\
         override the usual backup suffix\n\
"));
      oputs (_("\
  -t, --هدف-directory=DIRECTORY\n\
         copy all ذريعو arguments into DIRECTORY\n\
"));
      oputs (_("\
  -T, --no-هدف-directory\n\
         treat DEST as a عام_2 file\n\
"));
      oputs (_("\
  -v, --verbose\n\
         print the name جو each created file or directory\n\
"));
      oputs (_("\
      --preserve-context\n\
         preserve SELinux security context\n\
"));
      oputs (_("\
  -Z\n\
         سيٽ_2 SELinux security context جو destination file\n\
         and each created directory, منزل افتراضي type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or إذا CTX is specified then سيٽ_2 the\n\
         SELinux or SMACK security context منزل CTX\n\
"));

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخو_OPTION_DESCRIPTION);
      emit_backup_suffix_note ();
      emit_ancillary_info (پروگرام_NAME);
    }
  ٻاهر_نڪرو (حالت);
}

/* Copy file FROM onto file TO aka TO_DIRFD+TO_RELNAME and give TO the
   appropriate attributes.  X gives the command options.
   Return true if successful.  */

ساكن bool
install_file_اندريون_file (حرف ثابت *from, حرف ثابت *منزل,
                      صحيح منزل_dirfd, حرف ثابت *منزل_relname,
                      ثابت بنية cp_options *x)
{
  بنية stat from_sb;
  إذا (x->preserve_timestamps && stat (from, &from_sb) != 0)
    {
      error (0, errno, _("cannot stat %s"), quoteaf (from));
      أرجع false;
    }
  تعداد copy_حالت copy_حالت = copy_file (from, منزل, منزل_dirfd, منزل_relname, x);
  إذا (copy_حالت == COPY_FAILED)
    أرجع false;
  إذا (copy_حالت == COPY_OK && strip_files && ! strip (منزل))
    {
      إذا (unlinkat (منزل_dirfd, منزل_relname, 0) != 0)  /* Cleanup.  */
        error (EXIT_FAILURE, errno, _("cannot unlink %s"), quoteaf (منزل));
      أرجع false;
    }
  إذا (x->preserve_timestamps && (copy_حالت == COPY_SKIPPED || strip_files
                                 || ! S_ISREG (from_sb.st_گهڻائي))
      && ! change_timestamps (&from_sb, منزل, منزل_dirfd, منزل_relname))
    أرجع false;

  أرجع copy_حالت == COPY_SKIPPED
         || change_خاصيتون (منزل, منزل_dirfd, منزل_relname);
}

/* Create any missing parent directories of TO,
   while maintaining the current Working Directory.
   Return true if successful.  */

ساكن bool
mkancesdirs_محفوظ_wd (حرف ثابت *from, حرف *منزل, بنية cp_options *x,
                     bool save_always)
{
  bool save_working_directory =
    save_always
    || ! (IS_ABSOLUTE_FILE_NAME (from) && IS_ABSOLUTE_FILE_NAME (منزل));

  بنية savewd wd;
  savewd_init (&wd);
  إذا (! save_working_directory)
    savewd_پڄاڻي (&wd);

  صحيح حالت = EXIT_SUCCESS;
  إذا (mkancesdirs (منزل, &wd, make_ancestor, x) == -1)
    {
      error (0, errno, _("cannot create directory %s"), quoteaf (منزل));
      حالت = EXIT_FAILURE;
    }

  إذا (save_working_directory)
    {
      صحيح restore_نتيجو = savewd_restore (&wd, حالت);
      صحيح restore_errno = errno;
      savewd_پڄاڻي (&wd);
      إذا (EXIT_SUCCESS < restore_نتيجو)
        أرجع false;
      إذا (restore_نتيجو < 0 && حالت == EXIT_SUCCESS)
        {
          error (0, restore_errno, _("cannot create directory %s"),
                 quoteaf (منزل));
          أرجع false;
        }
    }
  أرجع حالت == EXIT_SUCCESS;
}

/* Copy file FROM onto file TO, creating any missing parent directories of TO.
   Return true if successful.  */

ساكن bool
install_file_اندريون_file_parents (حرف ثابت *from, حرف *منزل,
                              ثابت بنية cp_options *x)
{
  أرجع (mkancesdirs_محفوظ_wd (from, منزل, (بنية cp_options *)x, false)
          && install_file_اندريون_file (from, منزل, AT_FDCWD, منزل, x));
}

/* Copy file FROM into directory TO_DIR, keeping its same name,
   and give the copy the appropriate attributes.
   Return true if successful.  */

ساكن bool
install_file_اندريون_dir (حرف ثابت *from, حرف ثابت *منزل_dir,
                     ثابت بنية cp_options *x, bool mkdir_and_install,
                     صحيح *هدف_dirfd)
{
  حرف ثابت *from_بنياد = آخري_component (from);
  حرف *منزل_relname;
  حرف *منزل = file_name_concat (منزل_dir, from_بنياد, &منزل_relname);
  bool ret = true;

  إذا (!هدف_dirfd_valid (*هدف_dirfd)
      && (ret = mkdir_and_install)
      && (ret = mkancesdirs_محفوظ_wd (from, منزل, (بنية cp_options *) x, true)))
    {
      صحيح fd = open (منزل_dir, O_PATHSEARCH | O_DIRECTORY);
      إذا (fd < 0)
        {
          error (0, errno, _("cannot open %s"), quoteaf (منزل));
          ret = false;
        }
      وإلا
        *هدف_dirfd = fd;
    }

  إذا (ret)
    {
      صحيح منزل_dirfd = *هدف_dirfd;
      إذا (!هدف_dirfd_valid (منزل_dirfd))
        {
          منزل_dirfd = AT_FDCWD;
          منزل_relname = منزل;
        }
      ret = install_file_اندريون_file (from, منزل, منزل_dirfd, منزل_relname, x);
    }

  حرر (منزل);
  أرجع ret;
}

صحيح
مکيه (صحيح argc, حرف **argv)
{
  حرف ثابت *specified_گهڻائي = عدم;
  bool make_backups = false;
  حرف ثابت *backup_suffix = عدم;
  حرف *نسخو_control_string = عدم;
  bool mkdir_and_install = false;
  بنية cp_options x;
  حرف ثابت *هدف_directory = عدم;
  bool no_هدف_directory = false;
  bool strip_پروگرام_specified = false;
  حرف ثابت *scontext = عدم;
  /* set iff kernel has extra selinux system calls */
  selinux_enabled = (0 < is_selinux_enabled ());

  initialize_main (&argc, &argv);
  سيٽ_پروگرام_name (argv[0]);
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
          إذا (optarg)
            نسخو_control_string = optarg;
          اكسر;
        حالة 'c':
          اكسر;
        حالة 'C':
          copy_only_if_needed = true;
          اكسر;
        حالة 's':
          strip_files = true;
#جيڪڏهن_وصف_ٿيل SIGCHLD
          /* System V fork+wait does not work if SIGCHLD is ignored.  */
          إشارة (SIGCHLD, SIG_DFL);
#جيڪڏهن_پورو
          اكسر;
        حالة DEBUG_OPTION:
          x.debug = x.verbose = true;
          اكسر;
        حالة STRIP_پروگرام_OPTION:
          strip_پروگرام = xstrdup (optarg);
          strip_پروگرام_specified = true;
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
          گروهه_name = optarg;
          اكسر;
        حالة 'm':
          specified_گهڻائي = optarg;
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
          إذا (هدف_directory)
            error (EXIT_FAILURE, 0,
                   _("multiple target directories specified"));
          هدف_directory = optarg;
          اكسر;
        حالة 'T':
          no_هدف_directory = true;
          اكسر;

        حالة PRESERVE_CONTEXT_OPTION:
          إذا (! selinux_enabled)
            {
              error (0, 0, _("WARNING: ignoring --preserve-context; "
                             "this kernel is not SELinux-enabled"));
              اكسر;
            }
          x.preserve_security_context = true;
          use_default_selinux_context = false;
          اكسر;
        حالة 'Z':
          إذا (selinux_enabled)
            {
              /* Disable use of the install(1) specific setdefaultfilecon().
                 Note setdefaultfilecon() is different from the newer and more
                 generic restorecon() in that the former sets the context of
                 the dest files to that returned by selabel_lookup directly,
                 thus discarding MLS level and user identity of the file.
                 TODO: consider removing setdefaultfilecon() in future.  */
              use_default_selinux_context = false;

              إذا (optarg)
                scontext = optarg;
              وإلا
                x.سيٽ_security_context = get_labeling_handle ();
            }
          وإلا إذا (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux-enabled kernel"));
            }
          اكسر;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_نسخو_CHAR (پروگرام_NAME, AUTHORS);
        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  /* Check for invalid combinations of arguments. */
  إذا (dir_arg && strip_files)
    error (EXIT_FAILURE, 0,
           _("the strip option may not be used when installing a directory"));
  إذا (dir_arg && هدف_directory)
    error (EXIT_FAILURE, 0,
           _("target directory not allowed when installing a directory"));

  x.backup_type = (make_backups
                   ? xget_نسخو (_("backup type"),
                                   نسخو_control_string)
                   : no_backups);
  سيٽ_simple_backup_suffix (backup_suffix);

  إذا (x.preserve_security_context && (x.سيٽ_security_context || scontext))
    error (EXIT_FAILURE, 0,
           _("cannot set target context and preserve it"));

  إذا (scontext && setfscreatecon (scontext) < 0)
    error (EXIT_FAILURE, errno,
           _("failed to set default file creation context to %s"),
         quote (scontext));

  صحيح n_files = argc - optind;
  حرف **file = argv + optind;

  إذا (n_files <= ! (dir_arg || هدف_directory))
    {
      إذا (n_files <= 0)
        error (0, 0, _("missing file operand"));
      وإلا
        error (0, 0, _("missing destination file operand after %s"),
               quoteaf (file[0]));
      usage (EXIT_FAILURE);
    }

  بنية stat sb;
  صحيح هدف_dirfd = AT_FDCWD;
  إذا (no_هدف_directory)
    {
      إذا (هدف_directory)
        error (EXIT_FAILURE, 0,
               _("cannot combine --target-directory (-t) "
                 "and --no-target-directory (-T)"));
      إذا (2 < n_files)
        {
          error (0, 0, _("extra operand %s"), quoteaf (file[2]));
          usage (EXIT_FAILURE);
        }
    }
  وإلا إذا (هدف_directory)
    {
      هدف_dirfd = هدف_directory_عمليو (هدف_directory, &sb);
      إذا (! (هدف_dirfd_valid (هدف_dirfd)
             || (mkdir_and_install && errno == ENOENT)))
        error (EXIT_FAILURE, errno, _("failed to access %s"),
               quoteaf (هدف_directory));
    }
  وإلا إذا (!dir_arg)
    {
      حرف ثابت *lastfile = file[n_files - 1];
      صحيح fd = هدف_directory_عمليو (lastfile, &sb);
      إذا (هدف_dirfd_valid (fd))
        {
          هدف_dirfd = fd;
          هدف_directory = lastfile;
          n_files--;
        }
      وإلا إذا (2 < n_files)
        error (EXIT_FAILURE, errno, _("target %s"), quoteaf (lastfile));
    }

  إذا (specified_گهڻائي)
    {
      بنية گهڻائي_change *change = گهڻائي_compile (specified_گهڻائي);
      إذا (!change)
        error (EXIT_FAILURE, 0, _("invalid mode %s"), quote (specified_گهڻائي));
      گهڻائي_2 = گهڻائي_adjust (0, false, 0, change, عدم);
      dir_گهڻائي = گهڻائي_adjust (0, true, 0, change, &dir_گهڻائي_بٽ);
      حرر (change);
    }

  إذا (strip_پروگرام_specified && !strip_files)
    error (0, 0, _("WARNING: ignoring --strip-program option as -s option was "
                   "not specified"));

  إذا (copy_only_if_needed && strip_files)
    {
      error (0, 0, _("options --compare (-C) and --strip are mutually "
                     "exclusive"));
      usage (EXIT_FAILURE);
    }

  إذا (copy_only_if_needed && extra_گهڻائي (گهڻائي_2))
    error (0, 0, _("the --compare (-C) option is ignored when you"
                   " specify a mode with non-permission bits"));

  get_ids ();

  صحيح exit_حالت = EXIT_SUCCESS;
  إذا (dir_arg)
    exit_حالت = savewd_process_files (n_files, file, process_dir, &x);
  وإلا
    {
      /* FIXME: it's a little gross that this initialization is
         required by copy.c::copy. */
      hash_init ();

      إذا (!هدف_directory)
        {
          إذا (! (mkdir_and_install
                 ? install_file_اندريون_file_parents (file[0], file[1], &x)
                 : install_file_اندريون_file (file[0], file[1], AT_FDCWD,
                                         file[1], &x)))
            exit_حالت = EXIT_FAILURE;
        }
      وإلا
        {
          dest_info_init (&x);
          لكل (صحيح i = 0; i < n_files; i++)
            إذا (! install_file_اندريون_dir (file[i], هدف_directory, &x,
                                       i == 0 && mkdir_and_install,
                                       &هدف_dirfd))
              exit_حالت = EXIT_FAILURE;
        }
    }

  main_exit (exit_حالت);
}
