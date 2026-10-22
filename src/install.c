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
#تعریف_کن برنامه_NAME "install"

#تعریف_کن AUTHORS proper_name ("David MacKenzie")

ساكن صحيح selinux_enabled = 0;
ساكن bool use_default_selinux_context = true;

#إذا ! HAVE_ENDGRENT
# define endgrent() ((فراغ) 0)
#پایان_اگر

#إذا ! HAVE_ENDPWENT
# define endpwent() ((فراغ) 0)
#پایان_اگر

/* The user name that will own the files, or NULL to make the owner
   the current user ID. */
ساكن حرف *owner_name;

/* The user ID corresponding to 'owner_name'. */
ساكن uid_t owner_id;

/* The group name that will own the files, or NULL to make the group
   the current group ID. */
ساكن حرف *گروه_name;

/* The group ID corresponding to 'group_name'. */
ساكن gid_t گروه_id;

#تعریف_کن DEFAULT_نما (S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)

/* The file mode bits to which non-directory files will be set.  The umask has
   no effect. */
ساكن mode_t نما_2 = DEFAULT_نما;

/* Similar, but for directories.  */
ساكن mode_t dir_نما = DEFAULT_نما;

/* The file mode bits that the user cares about.  This should be a
   superset of DIR_MODE and a subset of CHMOD_MODE_BITS.  This matters
   for directories, since otherwise directories may keep their S_ISUID
   or S_ISGID bits.  */
ساكن mode_t dir_نما_بیتها = CHMOD_نما_بیتها;

/* Compare files before installing (-C) */
ساكن bool copy_only_if_needed;

/* If true, strip executable files after copying them. */
ساكن bool strip_files;

/* If true, install a directory instead of a regular file. */
ساكن bool dir_arg;

/* Program used to strip binaries, "strip" is default */
ساكن حرف ثابت *strip_برنامه = "strip";

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
تعداد
{
  DEBUG_OPTION = CHAR_MAX + 1,
  PRESERVE_CONTEXT_OPTION,
  STRIP_برنامه_OPTION
};

ساكن بنية option ثابت long_options[] =
{
  {"backup", optional_آرگومان, عدم, 'b'},
  {"compare", no_آرگومان, عدم, 'C'},
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {"debug", no_آرگومان, عدم, DEBUG_OPTION},
  {"directory", no_آرگومان, عدم, 'd'},
  {"group", required_آرگومان, عدم, 'g'},
  {"mode", required_آرگومان, عدم, 'm'},
  {"no-target-directory", no_آرگومان, عدم, 'T'},
  {"owner", required_آرگومان, عدم, 'o'},
  {"preserve-timestamps", no_آرگومان, عدم, 'p'},
  {"preserve-context", no_آرگومان, عدم, PRESERVE_CONTEXT_OPTION},
  {"strip", no_آرگومان, عدم, 's'},
  {"strip-program", required_آرگومان, عدم, STRIP_برنامه_OPTION},
  {"suffix", required_آرگومان, عدم, 'S'},
  {"target-directory", required_آرگومان, عدم, 't'},
  {"verbose", no_آرگومان, عدم, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخه_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

/* Compare content of opened files using file descriptors A_FD and B_FD. Return
   true if files are equal. */
ساكن bool
have_same_content (صحيح a_fd, صحيح b_fd)
{
  تعداد { CMP_بلوک_اندازه = 4096 };
  ساكن حرف a_buff[CMP_بلوک_اندازه];
  ساكن حرف b_buff[CMP_بلوک_اندازه];

  idx_t اندازه;
  طالما (0 < (اندازه = پر_read (a_fd, a_buff, حجم a_buff))) {
    إذا (اندازه != پر_read (b_fd, b_buff, حجم b_buff))
      أرجع false;

    إذا (!memeq (a_buff, b_buff, اندازه))
      أرجع false;
  }

  أرجع اندازه == 0;
}

/* Return true for mode with non-permission bits. */
ساكن bool
extra_نما (mode_t درون_داد)
{
  mode_t ماسک = S_IRWXUGO | S_IFMT;
  أرجع !! (درون_داد & ~ ماسک);
}

/* Return true if copy of file SRC_NAME to file DEST_NAME aka
   DEST_DIRFD+DEST_RELNAME is necessary. */
ساكن bool
need_copy (حرف ثابت *src_name, حرف ثابت *dest_name,
           صحيح dest_dirfd, حرف ثابت *dest_relname,
           ثابت بنية cp_options *x)
{
  إذا (extra_نما (نما_2))
    أرجع true;

  /* compare files using stat */
  بنية stat src_sb;
  إذا (stat (src_name, &src_sb) != 0)
    أرجع true;

  بنية stat dest_sb;
  إذا (fstatat (dest_dirfd, dest_relname, &dest_sb, AT_SYMLINK_NOFOLLOW) != 0)
    أرجع true;

  إذا (!S_ISREG (src_sb.st_نما) || !S_ISREG (dest_sb.st_نما)
      || extra_نما (src_sb.st_نما) || extra_نما (dest_sb.st_نما))
    أرجع true;

  إذا (src_sb.st_اندازه != dest_sb.st_اندازه
      || (dest_sb.st_نما & CHMOD_نما_بیتها) != نما_2)
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

  إذا (گروه_id == (uid_t) -1)
    {
      errno = 0;
      gid_t rgid = getgid ();
      إذا ((rgid == (uid_t) -1 && errno) || dest_sb.st_gid != rgid)
        أرجع true;
    }
  وإلا إذا (dest_sb.st_gid != گروه_id)
    أرجع true;

  /* compare SELinux context if preserving */
  إذا (selinux_enabled && x->preserve_security_context)
    {
      حرف *file_scontext_raw = عدم;
      إذا (getfilecon_raw (src_name, &file_scontext_raw) == -1)
        أرجع true;

      حرف *مقصد_scontext_raw = عدم;
      إذا (getfilecon_raw (dest_name, &مقصد_scontext_raw) == -1)
        {
          freecon (file_scontext_raw);
          أرجع true;
        }

      bool scontext_تطابق = streq (file_scontext_raw, مقصد_scontext_raw);

      freecon (file_scontext_raw);
      freecon (مقصد_scontext_raw);
      إذا (!scontext_تطابق)
        أرجع true;
    }

  /* compare files content */
  صحيح src_fd = open (src_name, O_RDONLY | O_دودویی);
  إذا (src_fd < 0)
    أرجع true;

  صحيح dest_fd = openat (dest_dirfd, dest_relname, O_RDONLY | O_دودویی);
  إذا (dest_fd < 0)
    {
      close (src_fd);
      أرجع true;
    }

  bool content_تطابق = have_same_content (src_fd, dest_fd);

  close (src_fd);
  close (dest_fd);
  أرجع !content_تطابق;
}

ساكن فراغ
cp_option_init (بنية cp_options *x)
{
  cp_options_default (x);
  x->copy_as_regular = true;
  x->reflink_نما = REFLINK_AUTO;
  x->dereference = DEREF_ALWAYS;
  x->unlink_dest_before_opening = true;
  x->unlink_dest_after_failed_open = false;
  x->hard_link = false;
  x->interactive = I_UNSPECIFIED;
  x->move_نما = false;
  x->install_نما = true;
  x->one_file_system = false;
  x->preserve_ownership = false;
  x->preserve_links = false;
  x->preserve_نما = false;
  x->preserve_timestamps = false;
  x->explicit_no_preserve_نما = false;
  x->reduce_diagnostics=false;
  x->داده_copy_required = true;
  x->require_preserve = false;
  x->require_preserve_xattr = false;
  x->recursive = false;
  x->sparse_نما = SPARSE_AUTO;
  x->symbolic_link = false;
  x->backup_type = no_backups;

  /* Create destination files initially writable so we can run strip on them.
     Although GNU strip works fine on read-only files, some others
     would fail.  */
  x->مجموعه_نما = true;
  x->نما_2 = S_IRUSR | S_IWUSR;
  x->stdin_tty = false;

  x->open_dangling_dest_symlink = false;
  x->update = UPDATE_ALL;
  x->require_preserve_context = false;  /* Not used by install currently.  */
  x->preserve_security_context = false; /* Whether to copy context from src.  */
  x->مجموعه_security_context = عدم; /* Whether to set sys default context.  */
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
  إذا (selabel_lookup_raw (hnd, &scontext_raw, file, st.st_نما) != 0)
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
  إذا (x->مجموعه_security_context
      && defaultcon (x->مجموعه_security_context, component, S_IFDIR) < 0
      && ! ignorable_ctx_err (errno))
    error (0, errno, _("failed to set default creation context for %s"),
           quoteaf (dir));

  صحيح r = mkdir (component, DEFAULT_نما);
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
                               dir_نما, announce_mkdir,
                               dir_نما_بیتها, owner_id, گروه_id, false)
          ? EXIT_SUCCESS
          : EXIT_FAILURE);

  /* FIXME: Due to the current structure of make_dir_parents()
     we don't have the facility to call defaultcon() before the
     final component of DIR is created.  So for now, create the
     final component with the context from previous component
     and here we set the context for the final component. */
  إذا (ret == EXIT_SUCCESS && x->مجموعه_security_context)
    {
      إذا (! restorecon (x->مجموعه_security_context, آخرین_component (dir), false)
          && ! ignorable_ctx_err (errno))
        error (0, errno, _("failed to restore context for %s"),
               quoteaf (dir));
    }

  أرجع ret;
}

تعداد copy_وضعیت { COPY_FAILED, COPY_OK, COPY_SKIPPED };

/* Copy file FROM onto file TO aka TO_DIRFD+TO_RELNAME, creating TO if
   necessary.  Return COPY_OK if successful or COPY_SKIPPED if the copy
   was skipped.  */

ساكن تعداد copy_وضعیت
copy_file (حرف ثابت *from, حرف ثابت *مقصد,
           صحيح مقصد_dirfd, حرف ثابت *مقصد_relname, ثابت بنية cp_options *x)
{
  إذا (copy_only_if_needed && !need_copy (from, مقصد, مقصد_dirfd, مقصد_relname, x))
    أرجع COPY_SKIPPED;

  /* Allow installing from non-regular files like /dev/null.
     Charles Karney reported that some Sun version of install allows that
     and that sendmail's installation process relies on the behavior.
     However, since !x->recursive, the call to "copy" will fail if FROM
     is a directory.  */

  bool copy_into_self;
  أرجع (copy (from, مقصد, مقصد_dirfd, مقصد_relname, 0, x, &copy_into_self, عدم)
          ? COPY_OK : COPY_FAILED);
}

/* Set the attributes of file or directory NAME aka DIRFD+RELNAME.
   Return true if successful.  */

ساكن bool
change_صفات (حرف ثابت *name, صحيح dirfd, حرف ثابت *relname)
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

  إذا (! (owner_id == (uid_t) -1 && گروه_id == (gid_t) -1)
      && lchownat (dirfd, relname, owner_id, گروه_id) != 0)
    error (0, errno, _("cannot change ownership of %s"), quoteaf (name));
  وإلا إذا (chmodat (dirfd, relname, نما_2) != 0)
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
  حرف ثابت *امن_name = name;
  إذا (name && *name == '-')
    امن_name = concat_name = file_name_concat (".", name, عدم);
  حرف ثابت *ثابت argv[] = { strip_برنامه, امن_name, عدم };

  /* Run 'strip'.  */
  pid_t pid;
  صحيح نتیجه = posix_spawnp (&pid, strip_برنامه, عدم, attrp,
                             (حرف * ثابت *) argv, environ);

  bool ok = false;
  إذا (نتیجه != 0)
    {
      error (0, نتیجه,
             streq (strip_برنامه, "strip")
               ? _("cannot run %s")
               : _("cannot run strip program %s"),
             quoteaf (strip_برنامه));
    }
  وإلا
    {
      /* Wait for 'strip' to complete, and emit a warning message on failure  */
      صحيح وضعیت;
      إذا (waitpid (pid, &وضعیت, 0) < 0)
        error (0, errno, _("waiting for strip"));
      وإلا إذا (! WIFEXITED (وضعیت) || WEXITSTATUS (وضعیت))
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
              || ckd_جمع_کن (&owner_id, tmp, 0))
            error (EXIT_FAILURE, 0, _("invalid user %s"),
                   quoteaf (owner_name));
        }
      وإلا
        owner_id = pw->pw_uid;
      endpwent ();
    }
  وإلا
    owner_id = (uid_t) -1;

  إذا (گروه_name)
    {
      بنية گروه *gr = getgrnam (گروه_name);
      إذا (gr == عدم)
        {
          uintmax_t tmp;
          إذا (xstrtoumax (گروه_name, عدم, 0, &tmp, "") != LONGINT_OK
              || ckd_جمع_کن (&گروه_id, tmp, 0))
            error (EXIT_FAILURE, 0, _("invalid group %s"),
                   quoteaf (گروه_name));
        }
      وإلا
        گروه_id = gr->gr_gid;
      endgrent ();
    }
  وإلا
    گروه_id = (gid_t) -1;
}

فراغ
usage (صحيح وضعیت)
{
  إذا (وضعیت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کن (_("\
Usage: %s [OPTION]... [-T] منبع DEST\n\
  or:  %s [OPTION]... منبع... DIRECTORY\n\
  or:  %s [OPTION]... -t DIRECTORY منبع...\n\
  or:  %s [OPTION]... -d DIRECTORY...\n\
"),
              برنامه_name, برنامه_name, برنامه_name, برنامه_name);
      fputs (_("\
\n\
This install برنامه copies files (often just compiled) into destination\n\
locations you انتخاب.  If you want مقصد download and install a ready-مقصد-use\n\
package on a GNU/Linux system, you should instead be using a package manager\n\
like yum(1) or apt-get(1).\n\
\n\
درون_2 the اولی three forms, copy منبع مقصد DEST or مضرب منبع(s) مقصد\n\
the existing DIRECTORY, طالما setting permission modes and owner/گروه.\n\
درون_2 the 4th form, create all components از the given DIRECTORY(ies).\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
      --backup[=CONTROL]\n\
         make a backup از each existing destination file\n\
"));
      oputs (_("\
  -b\n\
         like --backup but does not accept an آرگومان\n\
"));
      oputs (_("\
  -c\n\
         (ignored)\n\
"));
      oputs (_("\
  -C, --compare\n\
         compare content از منبع_2 and destination files,\n\
         and إذا no change مقصد content, ownership, and permissions,\n\
         افعل not modify the destination at all\n\
"));
      oputs (_("\
  -d, --directory\n\
         treat all arguments as directory نامها;\n\
         create all components از the specified directories\n\
"));
      oputs (_("\
  -D\n\
         create all leading components از DEST except the آخرین,\n\
         or all components از --هدف-directory,\n\
         then copy منبع مقصد DEST\n\
"));
      oputs (_("\
      --debug\n\
         explain how a file is copied.  نتیجه_میدهد_2 -v\n\
"));
      oputs (_("\
  -g, --گروه=گروه_2\n\
         مجموعه_2 گروه ownership, instead از process' current گروه\n\
"));
      oputs (_("\
  -m, --نما_2=نما\n\
         مجموعه_2 permission نما_2 (as درون chmod), instead از rwxr-xr-x\n\
"));
      oputs (_("\
  -o, --owner=OWNER\n\
         مجموعه_2 ownership (super-user only)\n\
"));
      oputs (_("\
  -p, --preserve-timestamps\n\
         apply access/modification times از منبع files\n\
         مقصد corresponding destination files\n\
"));
      oputs (_("\
  -s, --strip\n\
         strip نماد tables\n\
"));
      oputs (_("\
      --strip-برنامه=برنامه_2\n\
         برنامه استفاده_شده مقصد strip binaries\n\
"));
      oputs (_("\
  -S, --suffix=SUFFIX\n\
         override the usual backup suffix\n\
"));
      oputs (_("\
  -t, --هدف-directory=DIRECTORY\n\
         copy all منبع arguments into DIRECTORY\n\
"));
      oputs (_("\
  -T, --no-هدف-directory\n\
         treat DEST as a نرمال file\n\
"));
      oputs (_("\
  -v, --verbose\n\
         print the name از each created file or directory\n\
"));
      oputs (_("\
      --preserve-context\n\
         preserve SELinux security context\n\
"));
      oputs (_("\
  -Z\n\
         مجموعه_2 SELinux security context از destination file\n\
         and each created directory, مقصد افتراضي type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or إذا CTX is specified then مجموعه_2 the\n\
         SELinux or SMACK security context مقصد CTX\n\
"));

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);
      emit_backup_suffix_note ();
      emit_ancillary_info (برنامه_NAME);
    }
  خروج (وضعیت);
}

/* Copy file FROM onto file TO aka TO_DIRFD+TO_RELNAME and give TO the
   appropriate attributes.  X gives the command options.
   Return true if successful.  */

ساكن bool
install_file_درون_file (حرف ثابت *from, حرف ثابت *مقصد,
                      صحيح مقصد_dirfd, حرف ثابت *مقصد_relname,
                      ثابت بنية cp_options *x)
{
  بنية stat from_sb;
  إذا (x->preserve_timestamps && stat (from, &from_sb) != 0)
    {
      error (0, errno, _("cannot stat %s"), quoteaf (from));
      أرجع false;
    }
  تعداد copy_وضعیت copy_وضعیت = copy_file (from, مقصد, مقصد_dirfd, مقصد_relname, x);
  إذا (copy_وضعیت == COPY_FAILED)
    أرجع false;
  إذا (copy_وضعیت == COPY_OK && strip_files && ! strip (مقصد))
    {
      إذا (unlinkat (مقصد_dirfd, مقصد_relname, 0) != 0)  /* Cleanup.  */
        error (EXIT_FAILURE, errno, _("cannot unlink %s"), quoteaf (مقصد));
      أرجع false;
    }
  إذا (x->preserve_timestamps && (copy_وضعیت == COPY_SKIPPED || strip_files
                                 || ! S_ISREG (from_sb.st_نما))
      && ! change_timestamps (&from_sb, مقصد, مقصد_dirfd, مقصد_relname))
    أرجع false;

  أرجع copy_وضعیت == COPY_SKIPPED
         || change_صفات (مقصد, مقصد_dirfd, مقصد_relname);
}

/* Create any missing parent directories of TO,
   while maintaining the current Working Directory.
   Return true if successful.  */

ساكن bool
mkancesdirs_امن_wd (حرف ثابت *from, حرف *مقصد, بنية cp_options *x,
                     bool save_always)
{
  bool save_working_directory =
    save_always
    || ! (IS_ABSOLUTE_FILE_NAME (from) && IS_ABSOLUTE_FILE_NAME (مقصد));

  بنية savewd wd;
  savewd_init (&wd);
  إذا (! save_working_directory)
    savewd_پایان (&wd);

  صحيح وضعیت = EXIT_SUCCESS;
  إذا (mkancesdirs (مقصد, &wd, make_ancestor, x) == -1)
    {
      error (0, errno, _("cannot create directory %s"), quoteaf (مقصد));
      وضعیت = EXIT_FAILURE;
    }

  إذا (save_working_directory)
    {
      صحيح restore_نتیجه = savewd_restore (&wd, وضعیت);
      صحيح restore_errno = errno;
      savewd_پایان (&wd);
      إذا (EXIT_SUCCESS < restore_نتیجه)
        أرجع false;
      إذا (restore_نتیجه < 0 && وضعیت == EXIT_SUCCESS)
        {
          error (0, restore_errno, _("cannot create directory %s"),
                 quoteaf (مقصد));
          أرجع false;
        }
    }
  أرجع وضعیت == EXIT_SUCCESS;
}

/* Copy file FROM onto file TO, creating any missing parent directories of TO.
   Return true if successful.  */

ساكن bool
install_file_درون_file_parents (حرف ثابت *from, حرف *مقصد,
                              ثابت بنية cp_options *x)
{
  أرجع (mkancesdirs_امن_wd (from, مقصد, (بنية cp_options *)x, false)
          && install_file_درون_file (from, مقصد, AT_FDCWD, مقصد, x));
}

/* Copy file FROM into directory TO_DIR, keeping its same name,
   and give the copy the appropriate attributes.
   Return true if successful.  */

ساكن bool
install_file_درون_dir (حرف ثابت *from, حرف ثابت *مقصد_dir,
                     ثابت بنية cp_options *x, bool mkdir_and_install,
                     صحيح *هدف_dirfd)
{
  حرف ثابت *from_پایه = آخرین_component (from);
  حرف *مقصد_relname;
  حرف *مقصد = file_name_concat (مقصد_dir, from_پایه, &مقصد_relname);
  bool ret = true;

  إذا (!هدف_dirfd_valid (*هدف_dirfd)
      && (ret = mkdir_and_install)
      && (ret = mkancesdirs_امن_wd (from, مقصد, (بنية cp_options *) x, true)))
    {
      صحيح fd = open (مقصد_dir, O_PATHSEARCH | O_DIRECTORY);
      إذا (fd < 0)
        {
          error (0, errno, _("cannot open %s"), quoteaf (مقصد));
          ret = false;
        }
      وإلا
        *هدف_dirfd = fd;
    }

  إذا (ret)
    {
      صحيح مقصد_dirfd = *هدف_dirfd;
      إذا (!هدف_dirfd_valid (مقصد_dirfd))
        {
          مقصد_dirfd = AT_FDCWD;
          مقصد_relname = مقصد;
        }
      ret = install_file_درون_file (from, مقصد, مقصد_dirfd, مقصد_relname, x);
    }

  حرر (مقصد);
  أرجع ret;
}

صحيح
اصلی (صحيح argc, حرف **argv)
{
  حرف ثابت *specified_نما = عدم;
  bool make_backups = false;
  حرف ثابت *backup_suffix = عدم;
  حرف *نسخه_control_string = عدم;
  bool mkdir_and_install = false;
  بنية cp_options x;
  حرف ثابت *هدف_directory = عدم;
  bool no_هدف_directory = false;
  bool strip_برنامه_specified = false;
  حرف ثابت *scontext = عدم;
  /* set iff kernel has extra selinux system calls */
  selinux_enabled = (0 < is_selinux_enabled ());

  initialize_main (&argc, &argv);
  مجموعه_برنامه_name (argv[0]);
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
            نسخه_control_string = optarg;
          اكسر;
        حالة 'c':
          اكسر;
        حالة 'C':
          copy_only_if_needed = true;
          اكسر;
        حالة 's':
          strip_files = true;
#اگر_تعریف_شده SIGCHLD
          /* System V fork+wait does not work if SIGCHLD is ignored.  */
          إشارة (SIGCHLD, SIG_DFL);
#پایان_اگر
          اكسر;
        حالة DEBUG_OPTION:
          x.debug = x.verbose = true;
          اكسر;
        حالة STRIP_برنامه_OPTION:
          strip_برنامه = xstrdup (optarg);
          strip_برنامه_specified = true;
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
          گروه_name = optarg;
          اكسر;
        حالة 'm':
          specified_نما = optarg;
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
                x.مجموعه_security_context = get_labeling_handle ();
            }
          وإلا إذا (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux-enabled kernel"));
            }
          اكسر;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_نسخه_CHAR (برنامه_NAME, AUTHORS);
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
                   ? xget_نسخه (_("backup type"),
                                   نسخه_control_string)
                   : no_backups);
  مجموعه_simple_backup_suffix (backup_suffix);

  إذا (x.preserve_security_context && (x.مجموعه_security_context || scontext))
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
      هدف_dirfd = هدف_directory_عملوند (هدف_directory, &sb);
      إذا (! (هدف_dirfd_valid (هدف_dirfd)
             || (mkdir_and_install && errno == ENOENT)))
        error (EXIT_FAILURE, errno, _("failed to access %s"),
               quoteaf (هدف_directory));
    }
  وإلا إذا (!dir_arg)
    {
      حرف ثابت *lastfile = file[n_files - 1];
      صحيح fd = هدف_directory_عملوند (lastfile, &sb);
      إذا (هدف_dirfd_valid (fd))
        {
          هدف_dirfd = fd;
          هدف_directory = lastfile;
          n_files--;
        }
      وإلا إذا (2 < n_files)
        error (EXIT_FAILURE, errno, _("target %s"), quoteaf (lastfile));
    }

  إذا (specified_نما)
    {
      بنية نما_change *change = نما_compile (specified_نما);
      إذا (!change)
        error (EXIT_FAILURE, 0, _("invalid mode %s"), quote (specified_نما));
      نما_2 = نما_adjust (0, false, 0, change, عدم);
      dir_نما = نما_adjust (0, true, 0, change, &dir_نما_بیتها);
      حرر (change);
    }

  إذا (strip_برنامه_specified && !strip_files)
    error (0, 0, _("WARNING: ignoring --strip-program option as -s option was "
                   "not specified"));

  إذا (copy_only_if_needed && strip_files)
    {
      error (0, 0, _("options --compare (-C) and --strip are mutually "
                     "exclusive"));
      usage (EXIT_FAILURE);
    }

  إذا (copy_only_if_needed && extra_نما (نما_2))
    error (0, 0, _("the --compare (-C) option is ignored when you"
                   " specify a mode with non-permission bits"));

  get_ids ();

  صحيح exit_وضعیت = EXIT_SUCCESS;
  إذا (dir_arg)
    exit_وضعیت = savewd_process_files (n_files, file, process_dir, &x);
  وإلا
    {
      /* FIXME: it's a little gross that this initialization is
         required by copy.c::copy. */
      hash_init ();

      إذا (!هدف_directory)
        {
          إذا (! (mkdir_and_install
                 ? install_file_درون_file_parents (file[0], file[1], &x)
                 : install_file_درون_file (file[0], file[1], AT_FDCWD,
                                         file[1], &x)))
            exit_وضعیت = EXIT_FAILURE;
        }
      وإلا
        {
          dest_info_init (&x);
          لكل (صحيح i = 0; i < n_files; i++)
            إذا (! install_file_درون_dir (file[i], هدف_directory, &x,
                                       i == 0 && mkdir_and_install,
                                       &هدف_dirfd))
              exit_وضعیت = EXIT_FAILURE;
        }
    }

  main_exit (exit_وضعیت);
}
