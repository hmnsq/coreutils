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

#شامل <config.h>
#شامل <stdio.h>
#شامل <getopt.h>
#شامل <sys/types.h>
#شامل <signal.h>
#شامل <pwd.h>
#شامل <grp.h>
#شامل <selinux/label.h>
#شامل <spawn.h>
#شامل <sys/wait.h>

#شامل "system.h"
#شامل "backupfile.h"
#شامل "cp-hash.h"
#شامل "copy.h"
#شامل "filenamecat.h"
#شامل "full-read.h"
#شامل "mkancesdirs.h"
#شامل "mkdir-p.h"
#شامل "modechange.h"
#شامل "prog-fprintf.h"
#شامل "quote.h"
#شامل "savewd.h"
#شامل "selinux.h"
#شامل "stat-time.h"
#شامل "targetdir.h"
#شامل "utimens.h"
#شامل "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "install"

#تعریف_کرو AUTHORS proper_name ("David MacKenzie")

ساکن صحیح selinux_enabled = 0;
ساکن bool use_default_selinux_context = true;

#اگر ! HAVE_ENDGRENT
# define endgrent() ((خالی) 0)
#اگر_ختم

#اگر ! HAVE_ENDPWENT
# define endpwent() ((خالی) 0)
#اگر_ختم

/* The user name that will own the files, or NULL to make the owner
   the current user ID. */
ساکن حرف *owner_name;

/* The user ID corresponding to 'owner_name'. */
ساکن uid_t owner_id;

/* The group name that will own the files, or NULL to make the group
   the current group ID. */
ساکن حرف *گروہ_name;

/* The group ID corresponding to 'group_name'. */
ساکن gid_t گروہ_id;

#تعریف_کرو DEFAULT_منوال (S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)

/* The file mode bits to which non-directory files will be set.  The umask has
   no effect. */
ساکن mode_t منوال_2 = DEFAULT_منوال;

/* Similar, but for directories.  */
ساکن mode_t dir_منوال = DEFAULT_منوال;

/* The file mode bits that the user cares about.  This should be a
   superset of DIR_MODE and a subset of CHMOD_MODE_BITS.  This matters
   for directories, since otherwise directories may keep their S_ISUID
   or S_ISGID bits.  */
ساکن mode_t dir_منوال_بٹ = CHMOD_منوال_بٹ;

/* Compare files before installing (-C) */
ساکن bool copy_only_if_needed;

/* If true, strip executable files after copying them. */
ساکن bool strip_files;

/* If true, install a directory instead of a regular file. */
ساکن bool dir_arg;

/* Program used to strip binaries, "strip" is default */
ساکن حرف ثابت *strip_پروگرام = "strip";

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
شمار
{
  DEBUG_OPTION = CHAR_MAX + 1,
  PRESERVE_CONTEXT_OPTION,
  STRIP_پروگرام_OPTION
};

ساکن ساخت option ثابت long_options[] =
{
  {"backup", optional_دلیل, تہی, 'b'},
  {"compare", no_دلیل, تہی, 'C'},
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {"debug", no_دلیل, تہی, DEBUG_OPTION},
  {"directory", no_دلیل, تہی, 'd'},
  {"group", required_دلیل, تہی, 'g'},
  {"mode", required_دلیل, تہی, 'm'},
  {"no-target-directory", no_دلیل, تہی, 'T'},
  {"owner", required_دلیل, تہی, 'o'},
  {"preserve-timestamps", no_دلیل, تہی, 'p'},
  {"preserve-context", no_دلیل, تہی, PRESERVE_CONTEXT_OPTION},
  {"strip", no_دلیل, تہی, 's'},
  {"strip-program", required_دلیل, تہی, STRIP_پروگرام_OPTION},
  {"suffix", required_دلیل, تہی, 'S'},
  {"target-directory", required_دلیل, تہی, 't'},
  {"verbose", no_دلیل, تہی, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0}
};

/* Compare content of opened files using file descriptors A_FD and B_FD. Return
   true if files are equal. */
ساکن bool
have_same_content (صحیح a_fd, صحیح b_fd)
{
  شمار { CMP_بلاک_سائز = 4096 };
  ساکن حرف a_buff[CMP_بلاک_سائز];
  ساکن حرف b_buff[CMP_بلاک_سائز];

  idx_t سائز;
  جب (0 < (سائز = بھرا_read (a_fd, a_buff, حجم a_buff))) {
    اگر (سائز != بھرا_read (b_fd, b_buff, حجم b_buff))
      واپس false;

    اگر (!memeq (a_buff, b_buff, سائز))
      واپس false;
  }

  واپس سائز == 0;
}

/* Return true for mode with non-permission bits. */
ساکن bool
extra_منوال (mode_t داخلہ)
{
  mode_t نقاب = S_IRWXUGO | S_IFMT;
  واپس !! (داخلہ & ~ نقاب);
}

/* Return true if copy of file SRC_NAME to file DEST_NAME aka
   DEST_DIRFD+DEST_RELNAME is necessary. */
ساکن bool
need_copy (حرف ثابت *src_name, حرف ثابت *dest_name,
           صحیح dest_dirfd, حرف ثابت *dest_relname,
           ثابت ساخت cp_options *x)
{
  اگر (extra_منوال (منوال_2))
    واپس true;

  /* compare files using stat */
  ساخت stat src_sb;
  اگر (stat (src_name, &src_sb) != 0)
    واپس true;

  ساخت stat dest_sb;
  اگر (fstatat (dest_dirfd, dest_relname, &dest_sb, AT_SYMLINK_NOFOLLOW) != 0)
    واپس true;

  اگر (!S_ISREG (src_sb.st_منوال) || !S_ISREG (dest_sb.st_منوال)
      || extra_منوال (src_sb.st_منوال) || extra_منوال (dest_sb.st_منوال))
    واپس true;

  اگر (src_sb.st_سائز != dest_sb.st_سائز
      || (dest_sb.st_منوال & CHMOD_منوال_بٹ) != منوال_2)
    واپس true;

  اگر (owner_id == (uid_t) -1)
    {
      errno = 0;
      uid_t ruid = getuid ();
      اگر ((ruid == (uid_t) -1 && errno) || dest_sb.st_uid != ruid)
        واپس true;
    }
  ورنہ اگر (dest_sb.st_uid != owner_id)
    واپس true;

  اگر (گروہ_id == (uid_t) -1)
    {
      errno = 0;
      gid_t rgid = getgid ();
      اگر ((rgid == (uid_t) -1 && errno) || dest_sb.st_gid != rgid)
        واپس true;
    }
  ورنہ اگر (dest_sb.st_gid != گروہ_id)
    واپس true;

  /* compare SELinux context if preserving */
  اگر (selinux_enabled && x->preserve_security_context)
    {
      حرف *file_scontext_raw = تہی;
      اگر (getfilecon_raw (src_name, &file_scontext_raw) == -1)
        واپس true;

      حرف *مقصد_scontext_raw = تہی;
      اگر (getfilecon_raw (dest_name, &مقصد_scontext_raw) == -1)
        {
          freecon (file_scontext_raw);
          واپس true;
        }

      bool scontext_مطابقت = streq (file_scontext_raw, مقصد_scontext_raw);

      freecon (file_scontext_raw);
      freecon (مقصد_scontext_raw);
      اگر (!scontext_مطابقت)
        واپس true;
    }

  /* compare files content */
  صحیح src_fd = open (src_name, O_RDONLY | O_ثنائی);
  اگر (src_fd < 0)
    واپس true;

  صحیح dest_fd = openat (dest_dirfd, dest_relname, O_RDONLY | O_ثنائی);
  اگر (dest_fd < 0)
    {
      close (src_fd);
      واپس true;
    }

  bool content_مطابقت = have_same_content (src_fd, dest_fd);

  close (src_fd);
  close (dest_fd);
  واپس !content_مطابقت;
}

ساکن خالی
cp_option_init (ساخت cp_options *x)
{
  cp_options_default (x);
  x->copy_as_regular = true;
  x->reflink_منوال = REFLINK_AUTO;
  x->dereference = DEREF_ALWAYS;
  x->unlink_dest_before_opening = true;
  x->unlink_dest_after_failed_open = false;
  x->hard_link = false;
  x->interactive = I_UNSPECIFIED;
  x->move_منوال = false;
  x->install_منوال = true;
  x->one_file_system = false;
  x->preserve_ownership = false;
  x->preserve_links = false;
  x->preserve_منوال = false;
  x->preserve_timestamps = false;
  x->explicit_no_preserve_منوال = false;
  x->reduce_diagnostics=false;
  x->ڈیٹا_copy_required = true;
  x->require_preserve = false;
  x->require_preserve_xattr = false;
  x->recursive = false;
  x->sparse_منوال = SPARSE_AUTO;
  x->symbolic_link = false;
  x->backup_type = no_backups;

  /* Create destination files initially writable so we can run strip on them.
     Although GNU strip works fine on read-only files, some others
     would fail.  */
  x->سیٹ_منوال = true;
  x->منوال_2 = S_IRUSR | S_IWUSR;
  x->stdin_tty = false;

  x->open_dangling_dest_symlink = false;
  x->update = UPDATE_ALL;
  x->require_preserve_context = false;  /* Not used by install currently.  */
  x->preserve_security_context = false; /* Whether to copy context from src.  */
  x->سیٹ_security_context = تہی; /* Whether to set sys default context.  */
  x->preserve_xattr = false;
  x->verbose = false;
  x->dest_info = تہی;
  x->src_info = تہی;
}

ساکن ساخت selabel_handle *
get_labeling_handle (خالی)
{
  ساکن bool initialized;
  ساکن ساخت selabel_handle *hnd;
  اگر (!initialized)
    {
      initialized = true;
      hnd = selabel_open (SELABEL_CTX_FILE, تہی, 0);
      اگر (!hnd)
        error (0, errno, _("warning: security labeling handle failed"));
    }
  واپس hnd;
}

/* Modify file context to match the specified policy.
   If an error occurs the file will remain with the default directory
   context.  Note this sets the context to that returned by selabel_lookup
   and thus discards MLS levels and user identity of the FILE.  */
ساکن خالی
setdefaultfilecon (حرف ثابت *file)
{
  اگر (selinux_enabled != 1)
    {
      /* Indicate no context found. */
      واپس;
    }

  ساخت stat st;
  اگر (lstat (file, &st) != 0)
    واپس;

  ساخت selabel_handle *hnd = get_labeling_handle ();
  اگر (!hnd)
    واپس;

  حرف *scontext_raw = تہی;
  اگر (selabel_lookup_raw (hnd, &scontext_raw, file, st.st_منوال) != 0)
    {
      اگر (errno != ENOENT && ! ignorable_ctx_err (errno))
        error (0, errno, _("warning: %s: context lookup failed"),
               quotef (file));
      واپس;
    }

  اگر (lsetfilecon_raw (file, scontext_raw) < 0 && errno != ENOTSUP)
    error (0, errno,
           _("warning: %s: failed to change context to %s"),
           quotef_n (0, file), quote_n (1, scontext_raw));

  freecon (scontext_raw);
}

/* Report that directory DIR was made, if OPTIONS requests this.  */
ساکن خالی
announce_mkdir (حرف ثابت *dir, خالی *options)
{
  ساخت cp_options ثابت *x = options;
  اگر (x->verbose)
    prog_fprintf (stdout, _("creating directory %s"), quoteaf (dir));
}

/* Make ancestor directory DIR, whose last file name component is
   COMPONENT, with options OPTIONS.  Assume the working directory is
   COMPONENT's parent.  */
ساکن صحیح
make_ancestor (حرف ثابت *dir, حرف ثابت *component, خالی *options)
{
  ساخت cp_options ثابت *x = options;
  اگر (x->سیٹ_security_context
      && defaultcon (x->سیٹ_security_context, component, S_IFDIR) < 0
      && ! ignorable_ctx_err (errno))
    error (0, errno, _("failed to set default creation context for %s"),
           quoteaf (dir));

  صحیح r = mkdir (component, DEFAULT_منوال);
  اگر (r == 0)
    announce_mkdir (dir, options);
  واپس r;
}

/* Process a command-line file name, for the -d option.  */
ساکن صحیح
process_dir (حرف *dir, ساخت savewd *wd, خالی *options)
{
  ساخت cp_options ثابت *x = options;

  صحیح ret = (make_dir_parents (dir, wd, make_ancestor, options,
                               dir_منوال, announce_mkdir,
                               dir_منوال_بٹ, owner_id, گروہ_id, false)
          ? EXIT_SUCCESS
          : EXIT_FAILURE);

  /* FIXME: Due to the current structure of make_dir_parents()
     we don't have the facility to call defaultcon() before the
     final component of DIR is created.  So for now, create the
     final component with the context from previous component
     and here we set the context for the final component. */
  اگر (ret == EXIT_SUCCESS && x->سیٹ_security_context)
    {
      اگر (! restorecon (x->سیٹ_security_context, آخری_component (dir), false)
          && ! ignorable_ctx_err (errno))
        error (0, errno, _("failed to restore context for %s"),
               quoteaf (dir));
    }

  واپس ret;
}

شمار copy_حالت { COPY_FAILED, COPY_OK, COPY_SKIPPED };

/* Copy file FROM onto file TO aka TO_DIRFD+TO_RELNAME, creating TO if
   necessary.  Return COPY_OK if successful or COPY_SKIPPED if the copy
   was skipped.  */

ساکن شمار copy_حالت
copy_file (حرف ثابت *from, حرف ثابت *مقصد,
           صحیح مقصد_dirfd, حرف ثابت *مقصد_relname, ثابت ساخت cp_options *x)
{
  اگر (copy_only_if_needed && !need_copy (from, مقصد, مقصد_dirfd, مقصد_relname, x))
    واپس COPY_SKIPPED;

  /* Allow installing from non-regular files like /dev/null.
     Charles Karney reported that some Sun version of install allows that
     and that sendmail's installation process relies on the behavior.
     However, since !x->recursive, the call to "copy" will fail if FROM
     is a directory.  */

  bool copy_into_self;
  واپس (copy (from, مقصد, مقصد_dirfd, مقصد_relname, 0, x, &copy_into_self, تہی)
          ? COPY_OK : COPY_FAILED);
}

/* Set the attributes of file or directory NAME aka DIRFD+RELNAME.
   Return true if successful.  */

ساکن bool
change_اوصاف (حرف ثابت *name, صحیح dirfd, حرف ثابت *relname)
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

  اگر (! (owner_id == (uid_t) -1 && گروہ_id == (gid_t) -1)
      && lchownat (dirfd, relname, owner_id, گروہ_id) != 0)
    error (0, errno, _("cannot change ownership of %s"), quoteaf (name));
  ورنہ اگر (chmodat (dirfd, relname, منوال_2) != 0)
    error (0, errno, _("cannot change permissions of %s"), quoteaf (name));
  ورنہ
    ok = true;

  اگر (use_default_selinux_context)
    setdefaultfilecon (name);

  واپس ok;
}

/* Set the timestamps of file DEST aka DIRFD+RELNAME to match those of SRC_SB.
   Return true if successful.  */

ساکن bool
change_timestamps (ساخت stat ثابت *src_sb, حرف ثابت *dest,
                   صحیح dirfd, حرف ثابت *relname)
{
  ساخت timespec timespec[2] = { get_stat_atime (src_sb),
                                  get_stat_mtime (src_sb) };

  اگر (utimensat (dirfd, relname, timespec, 0))
    {
      error (0, errno, _("cannot set timestamps for %s"), quoteaf (dest));
      واپس false;
    }
  واپس true;
}

/* Strip the symbol table from the file NAME.
   We could dig the magic number out of the file first to
   determine whether to strip it, but the header files and
   magic numbers vary so much from system to system that making
   it portable would be very difficult.  Not worth the effort. */

ساکن bool
strip (حرف ثابت *name)
{
  posix_spawnattr_t attr;
  posix_spawnattr_t *attrp = تہی;

  /* Try to use vfork for systems where it matters.  */
  اگر (posix_spawnattr_init (&attr) == 0)
    {
      اگر (posix_spawnattr_setflags (&attr, POSIX_SPAWN_USEVFORK) == 0)
        attrp = &attr;
      ورنہ
        posix_spawnattr_destroy (&attr);
    }

  /* Construct the arguments to 'strip'.  */
  حرف *concat_name = تہی;
  حرف ثابت *محفوظ_name = name;
  اگر (name && *name == '-')
    محفوظ_name = concat_name = file_name_concat (".", name, تہی);
  حرف ثابت *ثابت argv[] = { strip_پروگرام, محفوظ_name, تہی };

  /* Run 'strip'.  */
  pid_t pid;
  صحیح نتیجہ = posix_spawnp (&pid, strip_پروگرام, تہی, attrp,
                             (حرف * ثابت *) argv, environ);

  bool ok = false;
  اگر (نتیجہ != 0)
    {
      error (0, نتیجہ,
             streq (strip_پروگرام, "strip")
               ? _("cannot run %s")
               : _("cannot run strip program %s"),
             quoteaf (strip_پروگرام));
    }
  ورنہ
    {
      /* Wait for 'strip' to complete, and emit a warning message on failure  */
      صحیح حالت_2;
      اگر (waitpid (pid, &حالت_2, 0) < 0)
        error (0, errno, _("waiting for strip"));
      ورنہ اگر (! WIFEXITED (حالت_2) || WEXITSTATUS (حالت_2))
        error (0, 0, _("strip process terminated abnormally"));
      ورنہ
        ok = true;
    }

  آزاد (concat_name);
  اگر (attrp)
    posix_spawnattr_destroy (attrp);

  واپس ok;
}

/* Initialize the user and group ownership of the files to install. */

ساکن خالی
get_ids (خالی)
{
  اگر (owner_name)
    {
      ساخت passwd *pw = getpwnam (owner_name);
      اگر (pw == تہی)
        {
          uintmax_t tmp;
          اگر (xstrtoumax (owner_name, تہی, 0, &tmp, "") != LONGINT_OK
              || ckd_جوڑو (&owner_id, tmp, 0))
            error (EXIT_FAILURE, 0, _("invalid user %s"),
                   quoteaf (owner_name));
        }
      ورنہ
        owner_id = pw->pw_uid;
      endpwent ();
    }
  ورنہ
    owner_id = (uid_t) -1;

  اگر (گروہ_name)
    {
      ساخت گروہ *gr = getgrnam (گروہ_name);
      اگر (gr == تہی)
        {
          uintmax_t tmp;
          اگر (xstrtoumax (گروہ_name, تہی, 0, &tmp, "") != LONGINT_OK
              || ckd_جوڑو (&گروہ_id, tmp, 0))
            error (EXIT_FAILURE, 0, _("invalid group %s"),
                   quoteaf (گروہ_name));
        }
      ورنہ
        گروہ_id = gr->gr_gid;
      endgrent ();
    }
  ورنہ
    گروہ_id = (gid_t) -1;
}

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("\
Usage: %s [OPTION]... [-T] ماخذ DEST\n\
  or:  %s [OPTION]... ماخذ... DIRECTORY\n\
  or:  %s [OPTION]... -t DIRECTORY ماخذ...\n\
  or:  %s [OPTION]... -d DIRECTORY...\n\
"),
              پروگرام_name, پروگرام_name, پروگرام_name, پروگرام_name);
      fputs (_("\
\n\
This install پروگرام copies files (often just compiled) into destination\n\
locations you انتخاب_2.  If you want مقصد download and install a ready-مقصد-use\n\
package on a GNU/Linux system, you should instead be using a package manager\n\
like yum(1) or apt-get(1).\n\
\n\
اندر_2 the پہلا three forms, copy ماخذ مقصد DEST or مضرب ماخذ(s) مقصد\n\
the existing DIRECTORY, جب setting permission modes and owner/گروہ.\n\
اندر_2 the 4th form, create all components کا the given DIRECTORY(ies).\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
      --backup[=CONTROL]\n\
         make a backup کا each existing destination file\n\
"));
      oputs (_("\
  -b\n\
         like --backup but does not accept an دلیل\n\
"));
      oputs (_("\
  -c\n\
         (ignored)\n\
"));
      oputs (_("\
  -C, --compare\n\
         compare content کا ماخذ_2 and destination files,\n\
         and اگر no change مقصد content, ownership, and permissions,\n\
         کرو not modify the destination at all\n\
"));
      oputs (_("\
  -d, --directory\n\
         treat all arguments as directory نام;\n\
         create all components کا the specified directories\n\
"));
      oputs (_("\
  -D\n\
         create all leading components کا DEST except the آخری,\n\
         or all components کا --ہدف-directory,\n\
         then copy ماخذ مقصد DEST\n\
"));
      oputs (_("\
      --debug\n\
         explain how a file is copied.  مستلزم_2 -v\n\
"));
      oputs (_("\
  -g, --گروہ=گروہ_2\n\
         سیٹ_2 گروہ ownership, instead کا process' current گروہ\n\
"));
      oputs (_("\
  -m, --منوال_2=منوال\n\
         سیٹ_2 permission منوال_2 (as اندر chmod), instead کا rwxr-xr-x\n\
"));
      oputs (_("\
  -o, --owner=OWNER\n\
         سیٹ_2 ownership (super-user only)\n\
"));
      oputs (_("\
  -p, --preserve-timestamps\n\
         apply access/modification times کا ماخذ files\n\
         مقصد corresponding destination files\n\
"));
      oputs (_("\
  -s, --strip\n\
         strip علامت tables\n\
"));
      oputs (_("\
      --strip-پروگرام=پروگرام_2\n\
         پروگرام مستعمل مقصد strip binaries\n\
"));
      oputs (_("\
  -S, --suffix=SUFFIX\n\
         override the usual backup suffix\n\
"));
      oputs (_("\
  -t, --ہدف-directory=DIRECTORY\n\
         copy all ماخذ arguments into DIRECTORY\n\
"));
      oputs (_("\
  -T, --no-ہدف-directory\n\
         treat DEST as a معمول file\n\
"));
      oputs (_("\
  -v, --verbose\n\
         print the name کا each created file or directory\n\
"));
      oputs (_("\
      --preserve-context\n\
         preserve SELinux security context\n\
"));
      oputs (_("\
  -Z\n\
         سیٹ_2 SELinux security context کا destination file\n\
         and each created directory, مقصد طےشدہ type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or اگر CTX is specified then سیٹ_2 the\n\
         SELinux or SMACK security context مقصد CTX\n\
"));

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      emit_backup_suffix_note ();
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

/* Copy file FROM onto file TO aka TO_DIRFD+TO_RELNAME and give TO the
   appropriate attributes.  X gives the command options.
   Return true if successful.  */

ساکن bool
install_file_اندر_file (حرف ثابت *from, حرف ثابت *مقصد,
                      صحیح مقصد_dirfd, حرف ثابت *مقصد_relname,
                      ثابت ساخت cp_options *x)
{
  ساخت stat from_sb;
  اگر (x->preserve_timestamps && stat (from, &from_sb) != 0)
    {
      error (0, errno, _("cannot stat %s"), quoteaf (from));
      واپس false;
    }
  شمار copy_حالت copy_حالت = copy_file (from, مقصد, مقصد_dirfd, مقصد_relname, x);
  اگر (copy_حالت == COPY_FAILED)
    واپس false;
  اگر (copy_حالت == COPY_OK && strip_files && ! strip (مقصد))
    {
      اگر (unlinkat (مقصد_dirfd, مقصد_relname, 0) != 0)  /* Cleanup.  */
        error (EXIT_FAILURE, errno, _("cannot unlink %s"), quoteaf (مقصد));
      واپس false;
    }
  اگر (x->preserve_timestamps && (copy_حالت == COPY_SKIPPED || strip_files
                                 || ! S_ISREG (from_sb.st_منوال))
      && ! change_timestamps (&from_sb, مقصد, مقصد_dirfd, مقصد_relname))
    واپس false;

  واپس copy_حالت == COPY_SKIPPED
         || change_اوصاف (مقصد, مقصد_dirfd, مقصد_relname);
}

/* Create any missing parent directories of TO,
   while maintaining the current Working Directory.
   Return true if successful.  */

ساکن bool
mkancesdirs_محفوظ_wd (حرف ثابت *from, حرف *مقصد, ساخت cp_options *x,
                     bool save_always)
{
  bool save_working_directory =
    save_always
    || ! (IS_ABSOLUTE_FILE_NAME (from) && IS_ABSOLUTE_FILE_NAME (مقصد));

  ساخت savewd wd;
  savewd_init (&wd);
  اگر (! save_working_directory)
    savewd_اختتام (&wd);

  صحیح حالت_2 = EXIT_SUCCESS;
  اگر (mkancesdirs (مقصد, &wd, make_ancestor, x) == -1)
    {
      error (0, errno, _("cannot create directory %s"), quoteaf (مقصد));
      حالت_2 = EXIT_FAILURE;
    }

  اگر (save_working_directory)
    {
      صحیح restore_نتیجہ = savewd_restore (&wd, حالت_2);
      صحیح restore_errno = errno;
      savewd_اختتام (&wd);
      اگر (EXIT_SUCCESS < restore_نتیجہ)
        واپس false;
      اگر (restore_نتیجہ < 0 && حالت_2 == EXIT_SUCCESS)
        {
          error (0, restore_errno, _("cannot create directory %s"),
                 quoteaf (مقصد));
          واپس false;
        }
    }
  واپس حالت_2 == EXIT_SUCCESS;
}

/* Copy file FROM onto file TO, creating any missing parent directories of TO.
   Return true if successful.  */

ساکن bool
install_file_اندر_file_parents (حرف ثابت *from, حرف *مقصد,
                              ثابت ساخت cp_options *x)
{
  واپس (mkancesdirs_محفوظ_wd (from, مقصد, (ساخت cp_options *)x, false)
          && install_file_اندر_file (from, مقصد, AT_FDCWD, مقصد, x));
}

/* Copy file FROM into directory TO_DIR, keeping its same name,
   and give the copy the appropriate attributes.
   Return true if successful.  */

ساکن bool
install_file_اندر_dir (حرف ثابت *from, حرف ثابت *مقصد_dir,
                     ثابت ساخت cp_options *x, bool mkdir_and_install,
                     صحیح *ہدف_dirfd)
{
  حرف ثابت *from_بنیاد = آخری_component (from);
  حرف *مقصد_relname;
  حرف *مقصد = file_name_concat (مقصد_dir, from_بنیاد, &مقصد_relname);
  bool ret = true;

  اگر (!ہدف_dirfd_valid (*ہدف_dirfd)
      && (ret = mkdir_and_install)
      && (ret = mkancesdirs_محفوظ_wd (from, مقصد, (ساخت cp_options *) x, true)))
    {
      صحیح fd = open (مقصد_dir, O_PATHSEARCH | O_DIRECTORY);
      اگر (fd < 0)
        {
          error (0, errno, _("cannot open %s"), quoteaf (مقصد));
          ret = false;
        }
      ورنہ
        *ہدف_dirfd = fd;
    }

  اگر (ret)
    {
      صحیح مقصد_dirfd = *ہدف_dirfd;
      اگر (!ہدف_dirfd_valid (مقصد_dirfd))
        {
          مقصد_dirfd = AT_FDCWD;
          مقصد_relname = مقصد;
        }
      ret = install_file_اندر_file (from, مقصد, مقصد_dirfd, مقصد_relname, x);
    }

  آزاد (مقصد);
  واپس ret;
}

صحیح
مین (صحیح argc, حرف **argv)
{
  حرف ثابت *specified_منوال = تہی;
  bool make_backups = false;
  حرف ثابت *backup_suffix = تہی;
  حرف *نسخہ_control_string = تہی;
  bool mkdir_and_install = false;
  ساخت cp_options x;
  حرف ثابت *ہدف_directory = تہی;
  bool no_ہدف_directory = false;
  bool strip_پروگرام_specified = false;
  حرف ثابت *scontext = تہی;
  /* set iff kernel has extra selinux system calls */
  selinux_enabled = (0 < is_selinux_enabled ());

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdin);

  cp_option_init (&x);

  umask (0);

  صحیح optc;
  جب ((optc = getopt_long (argc, argv, "bcCsDdg:m:o:pt:TvS:Z", long_options,
                              تہی))
         != -1)
    {
      انتخاب (optc)
        {
        حالت 'b':
          make_backups = true;
          اگر (optarg)
            نسخہ_control_string = optarg;
          توڑو;
        حالت 'c':
          توڑو;
        حالت 'C':
          copy_only_if_needed = true;
          توڑو;
        حالت 's':
          strip_files = true;
#اگر_تعریف_شدہ SIGCHLD
          /* System V fork+wait does not work if SIGCHLD is ignored.  */
          اشارہ (SIGCHLD, SIG_DFL);
#اگر_ختم
          توڑو;
        حالت DEBUG_OPTION:
          x.debug = x.verbose = true;
          توڑو;
        حالت STRIP_پروگرام_OPTION:
          strip_پروگرام = xstrdup (optarg);
          strip_پروگرام_specified = true;
          توڑو;
        حالت 'd':
          dir_arg = true;
          توڑو;
        حالت 'D':
          mkdir_and_install = true;
          توڑو;
        حالت 'v':
          x.verbose = true;
          توڑو;
        حالت 'g':
          گروہ_name = optarg;
          توڑو;
        حالت 'm':
          specified_منوال = optarg;
          توڑو;
        حالت 'o':
          owner_name = optarg;
          توڑو;
        حالت 'p':
          x.preserve_timestamps = true;
          توڑو;
        حالت 'S':
          make_backups = true;
          backup_suffix = optarg;
          توڑو;
        حالت 't':
          اگر (ہدف_directory)
            error (EXIT_FAILURE, 0,
                   _("multiple target directories specified"));
          ہدف_directory = optarg;
          توڑو;
        حالت 'T':
          no_ہدف_directory = true;
          توڑو;

        حالت PRESERVE_CONTEXT_OPTION:
          اگر (! selinux_enabled)
            {
              error (0, 0, _("WARNING: ignoring --preserve-context; "
                             "this kernel is not SELinux-enabled"));
              توڑو;
            }
          x.preserve_security_context = true;
          use_default_selinux_context = false;
          توڑو;
        حالت 'Z':
          اگر (selinux_enabled)
            {
              /* Disable use of the install(1) specific setdefaultfilecon().
                 Note setdefaultfilecon() is different from the newer and more
                 generic restorecon() in that the former sets the context of
                 the dest files to that returned by selabel_lookup directly,
                 thus discarding MLS level and user identity of the file.
                 TODO: consider removing setdefaultfilecon() in future.  */
              use_default_selinux_context = false;

              اگر (optarg)
                scontext = optarg;
              ورنہ
                x.سیٹ_security_context = get_labeling_handle ();
            }
          ورنہ اگر (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux-enabled kernel"));
            }
          توڑو;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);
        طےشدہ:
          usage (EXIT_FAILURE);
        }
    }

  /* Check for invalid combinations of arguments. */
  اگر (dir_arg && strip_files)
    error (EXIT_FAILURE, 0,
           _("the strip option may not be used when installing a directory"));
  اگر (dir_arg && ہدف_directory)
    error (EXIT_FAILURE, 0,
           _("target directory not allowed when installing a directory"));

  x.backup_type = (make_backups
                   ? xget_نسخہ (_("backup type"),
                                   نسخہ_control_string)
                   : no_backups);
  سیٹ_simple_backup_suffix (backup_suffix);

  اگر (x.preserve_security_context && (x.سیٹ_security_context || scontext))
    error (EXIT_FAILURE, 0,
           _("cannot set target context and preserve it"));

  اگر (scontext && setfscreatecon (scontext) < 0)
    error (EXIT_FAILURE, errno,
           _("failed to set default file creation context to %s"),
         quote (scontext));

  صحیح n_files = argc - optind;
  حرف **file = argv + optind;

  اگر (n_files <= ! (dir_arg || ہدف_directory))
    {
      اگر (n_files <= 0)
        error (0, 0, _("missing file operand"));
      ورنہ
        error (0, 0, _("missing destination file operand after %s"),
               quoteaf (file[0]));
      usage (EXIT_FAILURE);
    }

  ساخت stat sb;
  صحیح ہدف_dirfd = AT_FDCWD;
  اگر (no_ہدف_directory)
    {
      اگر (ہدف_directory)
        error (EXIT_FAILURE, 0,
               _("cannot combine --target-directory (-t) "
                 "and --no-target-directory (-T)"));
      اگر (2 < n_files)
        {
          error (0, 0, _("extra operand %s"), quoteaf (file[2]));
          usage (EXIT_FAILURE);
        }
    }
  ورنہ اگر (ہدف_directory)
    {
      ہدف_dirfd = ہدف_directory_عملیہ (ہدف_directory, &sb);
      اگر (! (ہدف_dirfd_valid (ہدف_dirfd)
             || (mkdir_and_install && errno == ENOENT)))
        error (EXIT_FAILURE, errno, _("failed to access %s"),
               quoteaf (ہدف_directory));
    }
  ورنہ اگر (!dir_arg)
    {
      حرف ثابت *lastfile = file[n_files - 1];
      صحیح fd = ہدف_directory_عملیہ (lastfile, &sb);
      اگر (ہدف_dirfd_valid (fd))
        {
          ہدف_dirfd = fd;
          ہدف_directory = lastfile;
          n_files--;
        }
      ورنہ اگر (2 < n_files)
        error (EXIT_FAILURE, errno, _("target %s"), quoteaf (lastfile));
    }

  اگر (specified_منوال)
    {
      ساخت منوال_change *change = منوال_compile (specified_منوال);
      اگر (!change)
        error (EXIT_FAILURE, 0, _("invalid mode %s"), quote (specified_منوال));
      منوال_2 = منوال_adjust (0, false, 0, change, تہی);
      dir_منوال = منوال_adjust (0, true, 0, change, &dir_منوال_بٹ);
      آزاد (change);
    }

  اگر (strip_پروگرام_specified && !strip_files)
    error (0, 0, _("WARNING: ignoring --strip-program option as -s option was "
                   "not specified"));

  اگر (copy_only_if_needed && strip_files)
    {
      error (0, 0, _("options --compare (-C) and --strip are mutually "
                     "exclusive"));
      usage (EXIT_FAILURE);
    }

  اگر (copy_only_if_needed && extra_منوال (منوال_2))
    error (0, 0, _("the --compare (-C) option is ignored when you"
                   " specify a mode with non-permission bits"));

  get_ids ();

  صحیح exit_حالت = EXIT_SUCCESS;
  اگر (dir_arg)
    exit_حالت = savewd_process_files (n_files, file, process_dir, &x);
  ورنہ
    {
      /* FIXME: it's a little gross that this initialization is
         required by copy.c::copy. */
      hash_init ();

      اگر (!ہدف_directory)
        {
          اگر (! (mkdir_and_install
                 ? install_file_اندر_file_parents (file[0], file[1], &x)
                 : install_file_اندر_file (file[0], file[1], AT_FDCWD,
                                         file[1], &x)))
            exit_حالت = EXIT_FAILURE;
        }
      ورنہ
        {
          dest_info_init (&x);
          برائے (صحیح i = 0; i < n_files; i++)
            اگر (! install_file_اندر_dir (file[i], ہدف_directory, &x,
                                       i == 0 && mkdir_and_install,
                                       &ہدف_dirfd))
              exit_حالت = EXIT_FAILURE;
        }
    }

  main_exit (exit_حالت);
}
