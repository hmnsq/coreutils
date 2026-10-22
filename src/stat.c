/* stat.c -- display file or file system status
   Copyright (C) 2001-2026 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.

   Written by Michael Meskes.  */

#ಅನ್ತರ್ಭಾವ <config.h>

/* Keep this conditional in sync with the similar conditional in
   ../m4/stat-prog.m4.  */
#ಆದರೆ ((STAT_STATVFS || STAT_STATVFS64)                                       \
     && (HAVE_STRUCT_STATVFS_F_BASETYPE || HAVE_STRUCT_STATVFS_F_FSTYPENAME \
         || (! HAVE_STRUCT_STATFS_F_FSTYPENAME && HAVE_STRUCT_STATVFS_F_TYPE)))
# define USE_STATVFS 1
#ಇಲ್ಲದಿದ್ದರೆ
# define USE_STATVFS 0
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <pwd.h>
#ಅನ್ತರ್ಭಾವ <grp.h>
#ಆದರೆ USE_STATVFS
# include <sys/statvfs.h>
#ಅಥವಾಆದರೆ HAVE_SYS_VFS_H
# include <sys/vfs.h>
#ಅಥವಾಆದರೆ HAVE_SYS_MOUNT_H && HAVE_SYS_PARAM_H
/* NOTE: freebsd5.0 needs sys/param.h and sys/mount.h for statfs.
   It does have statvfs.h, but shouldn't use it, since it doesn't
   HAVE_STRUCT_STATVFS_F_BASETYPE.  So find a clean way to fix it.  */
/* NetBSD 1.5.2 needs these, for the declaration of struct statfs. */
# include <sys/param.h>
# include <sys/mount.h>
# ಆದರೆ HAVE_NFS_NFS_CLNT_H && HAVE_NFS_VFS_H
/* Ultrix 4.4 needs these for the declaration of struct statfs.  */
#  include <netinet/in.h>
#  include <nfs/nfs_clnt.h>
#  include <nfs/vfs.h>
# endif
#ಅಥವಾಆದರೆ HAVE_OS_H /* BeOS */
# include <fs_info.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <selinux/selinux.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>

#ಅನ್ತರ್ಭಾವ "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ "areadlink.h"
#ಅನ್ತರ್ಭಾವ "c-ctype.h"
#ಅನ್ತರ್ಭಾವ "file-type.h"
#ಅನ್ತರ್ಭಾವ "filemode.h"
#ಅನ್ತರ್ಭಾವ "fs.h"
#ಅನ್ತರ್ಭಾವ "mountlist.h"
#ಅನ್ತರ್ಭಾವ "octhexdigits.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "stat-size.h"
#ಅನ್ತರ್ಭಾವ "stat-time.h"
#ಅನ್ತರ್ಭಾವ "strftime.h"
#ಅನ್ತರ್ಭಾವ "find-mount-point.h"
#ಅನ್ತರ್ಭಾವ "xvasprintf.h"
#ಅನ್ತರ್ಭಾವ "statx.h"

#ಆದರೆ HAVE_STATX && defined STATX_INO
# define USE_STATX 1
#ಇಲ್ಲದಿದ್ದರೆ
# define USE_STATX 0
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ USE_STATVFS
# define STRUCT_STATXFS_F_FSID_IS_INTEGER STRUCT_STATVFS_F_FSID_IS_INTEGER
# define HAVE_STRUCT_STATXFS_F_TYPE HAVE_STRUCT_STATVFS_F_TYPE
# ಆದರೆ HAVE_STRUCT_STATVFS_F_NAMEMAX
#  define SB_F_NAMEMAX(S) ((S)->f_namemax)
# endif
# ಆದರೆ ! STAT_STATVFS && STAT_STATVFS64
#  define STRUCT_STATVFS ರಚನೆ statvfs64
#  define STATFS statvfs64
# ಇಲ್ಲದಿದ್ದರೆ
#  define STRUCT_STATVFS ರಚನೆ statvfs
#  define STATFS statvfs
# endif
# define STATFS_FRSIZE(S) ((S)->f_frsize)
#ಇಲ್ಲದಿದ್ದರೆ
# define HAVE_STRUCT_STATXFS_F_TYPE HAVE_STRUCT_STATFS_F_TYPE
# ಆದರೆ HAVE_STRUCT_STATFS_F_NAMELEN
#  define SB_F_NAMEMAX(S) ((S)->f_namelen)
# elif HAVE_STRUCT_STATFS_F_NAMEMAX
#  define SB_F_NAMEMAX(S) ((S)->f_namemax)
# endif
# define STATFS statfs
# ಆದರೆ HAVE_OS_H /* BeOS */
/* BeOS has a statvfs function, but it does not return sensible values
   for f_files, f_ffree and f_favail, and lacks f_type, f_basetype and
   f_fstypename.  Use 'struct fs_info' instead.  */
NODISCARD
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
statfs (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename, ರಚನೆ fs_info *buf)
{
  dev_t device = dev_for_ಮಾರ್ಗ (filename);
  ಆದರೆ (device < 0)
    {
      errno = (device == B_ನಮೂದು_NOT_ಸಿಕ್ಕಿತು ? ENOENT
               : device == B_BAD_ಮೌಲ್ಯ ? EINVAL
               : device == B_NAME_TOO_LONG ? ENAMETOOLONG
               : device == B_NO_ಸ್ಮರಣೆ ? ENOMEM
               : device == B_FILE_ERROR ? EIO
               : 0);
      ಹಿಂತಿರುಗಿ -1;
    }
  /* If successful, buf->dev will be == device.  */
  ಹಿಂತಿರುಗಿ fs_stat_dev (device, buf);
}
#  define f_fsid dev
#  define f_ಖಂಡಗಳು ಒಟ್ಟು_ಖಂಡಗಳು
#  define f_bfree free_ಖಂಡಗಳು
#  define f_bavail free_ಖಂಡಗಳು
#  define f_bsize io_ಗಾತ್ರ
#  define f_files ಒಟ್ಟು_nodes
#  define f_ffree free_nodes
#  define STRUCT_STATVFS ರಚನೆ fs_info
#  define STRUCT_STATXFS_F_FSID_IS_INTEGER true
#  define STATFS_FRSIZE(S) ((S)->ಖಂಡ_ಗಾತ್ರ)
# ಇಲ್ಲದಿದ್ದರೆ
#  define STRUCT_STATVFS ರಚನೆ statfs
#  define STRUCT_STATXFS_F_FSID_IS_INTEGER STRUCT_STATFS_F_FSID_IS_INTEGER
#  ಆದರೆ HAVE_STRUCT_STATFS_F_FRSIZE
#   define STATFS_FRSIZE(S) ((S)->f_frsize)
#  ಇಲ್ಲದಿದ್ದರೆ
#   define STATFS_FRSIZE(S) 0
#  endif
# endif
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ SB_F_NAMEMAX
# define ಹೊರಹೋಗು_NAMEMAX ಹೊರಹೋಗು_uint
#ಇಲ್ಲದಿದ್ದರೆ
/* Depending on whether statvfs or statfs is used,
   neither f_namemax or f_namelen may be available.  */
# define SB_F_NAMEMAX(S) "?"
# define ಹೊರಹೋಗು_NAMEMAX ಹೊರಹೋಗು_string
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ HAVE_STRUCT_STATVFS_F_BASETYPE
# define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME f_basetype
#ಇಲ್ಲದಿದ್ದರೆ
# ಆದರೆ HAVE_STRUCT_STATVFS_F_FSTYPENAME || HAVE_STRUCT_STATFS_F_FSTYPENAME
#  define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME f_fstypename
# elif HAVE_OS_H /* BeOS */
#  define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME fsh_name
# endif
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ HAVE_GETATTRAT
# include <attr.h>
# include <sys/nvpair.h>
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ digits[] = "0123456789";

/* Flags that are portable for use in printf, for at least one
   conversion specifier; make_format removes non-portable flags as
   needed for particular specifiers.  The glibc 2.2 extension "I" is
   listed here; it is removed by make_format because it has undefined
   behavior elsewhere and because it is incompatible with
   out_epoch_sec.  */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ printf_flags[] = "'-+ #0I";

/* Formats for the --terse option.  */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ fmt_terse_fs[] = "%Qn %i %l %t %s %S %b %f %a %c %d\n";
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ fmt_terse_regular[] = "%Qn %s %b %f %u %g %D %i %h %t %T"
                                        " %X %Y %Z %W %o\n";
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ fmt_terse_selinux[] = "%Qn %s %b %f %u %g %D %i %h %t %T"
                                        " %X %Y %Z %W %o %C\n";

#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "stat"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS proper_name ("Michael Meskes")

ಎಣಿಕೆ
{
  PRINTF_OPTION = CHAR_MAX + 1
};

ಎಣಿಕೆ cached_ಬಹುಲಕ
{
  cached_default,
  cached_never,
  cached_always
};

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸ್ಥಿರಾಂಕ cached_ವಾದಗಳು[] =
{
  "default", "never", "always", ಖಾಲಿ
};

ಸ್ಥಿರ ಎಣಿಕೆ cached_ಬಹುಲಕ ಸ್ಥಿರಾಂಕ cached_modes[] =
{
  cached_default, cached_never, cached_always
};

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ long_options[] =
{
  {"dereference", no_ವಾದ, ಖಾಲಿ, 'L'},
  {"file-system", no_ವಾದ, ಖಾಲಿ, 'f'},
  {"format", required_ವಾದ, ಖಾಲಿ, 'c'},
  {"printf", required_ವಾದ, ಖಾಲಿ, PRINTF_OPTION},
  {"terse", no_ವಾದ, ಖಾಲಿ, 't'},
  {"cached", required_ವಾದ, ಖಾಲಿ, 0},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};

/* Whether to follow symbolic links;  True for --dereference (-L).  */
ಸ್ಥಿರ bool follow_links;

/* Whether to interpret backslash-escape sequences.
   True for --printf=FMT, not for --format=FMT (-c).  */
ಸ್ಥಿರ bool interpret_backslash_escapes;

/* The trailing delimiter string:
   "" for --printf=FMT, "\n" for --format=FMT (-c).  */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *trailing_delim = "";

/* The representation of the decimal point in the current locale.  */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *decimal_ಬಿಂದು;
ಸ್ಥಿರ size_t decimal_ಬಿಂದು_len;

ಸ್ಥಿರ bool
print_stat (ಅಕ್ಷರ *pformat, size_t ಪೂರ್ವಪ್ರತ್ಯಯ_len, ಅಕ್ಷರ mod, ಅಕ್ಷರ m,
            ಪೂರ್ಣಾಂಕ fd, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename, ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *ದತ್ತಾಂಶ);

/* Return the type of the specified file system.
   Some systems have statfvs.f_basetype[FSTYPSZ] (AIX, HP-UX, and Solaris).
   Others have statvfs.f_fstypename[_VFS_NAMELEN] (NetBSD 3.0).
   Others have statfs.f_fstypename[MFSNAMELEN] (NetBSD 1.5.2).
   Still others have neither and have to get by with f_type (GNU/Linux).
   But f_type may only exist in statfs (Cygwin).  */
NODISCARD
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *
human_fstype (STRUCT_STATVFS ಸ್ಥಿರಾಂಕ *statfsbuf)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME
  ಹಿಂತಿರುಗಿ statfsbuf->STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME;
#ಇಲ್ಲದಿದ್ದರೆ
  ಆಯ್ಕೆ (statfsbuf->f_type)
    {
# ಆದರೆ defined __linux__ || defined __ANDROID__

      /* Compare with what's in libc:
         f=/a/libc/sysdeps/unix/sysv/linux/linux_fsinfo.h
         sed -n '/ADFS_SUPER_MAGIC/,/SYSFS_MAGIC/p' $f \
           | perl -n -e '/#define (.*?)_(?:SUPER_)MAGIC\s+0x(\S+)/' \
             -e 'and print "case S_MAGIC_$1: /\* 0x" . uc($2) . " *\/\n"' \
           | sort > sym_libc
         perl -ne '/^\s+(case S_MAGIC_.*?): \/\* 0x(\S+) \*\//' \
             -e 'and do { $v=uc$2; print "$1: /\* 0x$v *\/\n"}' stat.c \
           | sort > sym_stat
         diff -u sym_stat sym_libc
      */

      /* Also compare with the list in "man 2 statfs" using the
         fs-magic-compare make target.  */

      /* IMPORTANT NOTE: Each of the following 'case S_MAGIC_...:'
         statements must be followed by a hexadecimal constant in
         a comment.  The S_MAGIC_... name and constant are automatically
         combined to produce the #define directives in fs.h.  */

    ಸ್ಥಿತಿ S_MAGIC_AAFS: /* 0x5A3C69F0 local */
      ಹಿಂತಿರುಗಿ "aafs";
    ಸ್ಥಿತಿ S_MAGIC_ACFS: /* 0x61636673 remote */
      ಹಿಂತಿರುಗಿ "acfs";
    ಸ್ಥಿತಿ S_MAGIC_ADFS: /* 0xADF5 local */
      ಹಿಂತಿರುಗಿ "adfs";
    ಸ್ಥಿತಿ S_MAGIC_AFFS: /* 0xADFF local */
      ಹಿಂತಿರುಗಿ "affs";
    ಸ್ಥಿತಿ S_MAGIC_AFS: /* 0x5346414F remote */
      ಹಿಂತಿರುಗಿ "afs";
    ಸ್ಥಿತಿ S_MAGIC_ANON_INODE_FS: /* 0x09041934 local */
      ಹಿಂತಿರುಗಿ "anon-inode FS";
    ಸ್ಥಿತಿ S_MAGIC_AUFS: /* 0x61756673 remote */
      /* FIXME: change syntax or add an optional attribute like "inotify:no".
         The above is labeled as "remote" so that tail always uses polling,
         but this isn't really a remote file system type.  */
      ಹಿಂತಿರುಗಿ "aufs";
    ಸ್ಥಿತಿ S_MAGIC_AUTOFS: /* 0x0187 local */
      ಹಿಂತಿರುಗಿ "autofs";
    ಸ್ಥಿತಿ S_MAGIC_BALLOON_KVM: /* 0x13661366 local */
      ಹಿಂತಿರುಗಿ "balloon-kvm-fs";
    ಸ್ಥಿತಿ S_MAGIC_BCACHEFS: /* 0xCA451A4E local */
      ಹಿಂತಿರುಗಿ "bcachefs";
    ಸ್ಥಿತಿ S_MAGIC_BEFS: /* 0x42465331 local */
      ಹಿಂತಿರುಗಿ "befs";
    ಸ್ಥಿತಿ S_MAGIC_BDEVFS: /* 0x62646576 local */
      ಹಿಂತಿರುಗಿ "bdevfs";
    ಸ್ಥಿತಿ S_MAGIC_BFS: /* 0x1BADFACE local */
      ಹಿಂತಿರುಗಿ "bfs";
    ಸ್ಥಿತಿ S_MAGIC_BINDERFS: /* 0x6C6F6F70 local */
      ಹಿಂತಿರುಗಿ "binderfs";
    ಸ್ಥಿತಿ S_MAGIC_BPF_FS: /* 0xCAFE4A11 local */
      ಹಿಂತಿರುಗಿ "bpf_fs";
    ಸ್ಥಿತಿ S_MAGIC_BINFMTFS: /* 0x42494E4D local */
      ಹಿಂತಿರುಗಿ "binfmt_misc";
    ಸ್ಥಿತಿ S_MAGIC_BTRFS: /* 0x9123683E local */
      ಹಿಂತಿರುಗಿ "btrfs";
    ಸ್ಥಿತಿ S_MAGIC_BTRFS_TEST: /* 0x73727279 local */
      ಹಿಂತಿರುಗಿ "btrfs_test";
    ಸ್ಥಿತಿ S_MAGIC_CEPH: /* 0x00C36400 remote */
      ಹಿಂತಿರುಗಿ "ceph";
    ಸ್ಥಿತಿ S_MAGIC_CGROUP: /* 0x0027E0EB local */
      ಹಿಂತಿರುಗಿ "cgroupfs";
    ಸ್ಥಿತಿ S_MAGIC_CGROUP2: /* 0x63677270 local */
      ಹಿಂತಿರುಗಿ "cgroup2fs";
    ಸ್ಥಿತಿ S_MAGIC_CIFS: /* 0xFF534D42 remote */
      ಹಿಂತಿರುಗಿ "cifs";
    ಸ್ಥಿತಿ S_MAGIC_CODA: /* 0x73757245 remote */
      ಹಿಂತಿರುಗಿ "coda";
    ಸ್ಥಿತಿ S_MAGIC_COH: /* 0x012FF7B7 local */
      ಹಿಂತಿರುಗಿ "coh";
    ಸ್ಥಿತಿ S_MAGIC_CONFIGFS: /* 0x62656570 local */
      ಹಿಂತಿರುಗಿ "configfs";
    ಸ್ಥಿತಿ S_MAGIC_CRAMFS: /* 0x28CD3D45 local */
      ಹಿಂತಿರುಗಿ "cramfs";
    ಸ್ಥಿತಿ S_MAGIC_CRAMFS_WEND: /* 0x453DCD28 local */
      ಹಿಂತಿರುಗಿ "cramfs-wend";
    ಸ್ಥಿತಿ S_MAGIC_DAXFS: /* 0x64646178 local */
      ಹಿಂತಿರುಗಿ "daxfs";
    ಸ್ಥಿತಿ S_MAGIC_DEBUGFS: /* 0x64626720 local */
      ಹಿಂತಿರುಗಿ "debugfs";
    ಸ್ಥಿತಿ S_MAGIC_DEVFS: /* 0x1373 local */
      ಹಿಂತಿರುಗಿ "devfs";
    ಸ್ಥಿತಿ S_MAGIC_DEVMEM: /* 0x454D444D local */
      ಹಿಂತಿರುಗಿ "devmem";
    ಸ್ಥಿತಿ S_MAGIC_DEVPTS: /* 0x1CD1 local */
      ಹಿಂತಿರುಗಿ "devpts";
    ಸ್ಥಿತಿ S_MAGIC_DMA_BUF: /* 0x444D4142 local */
      ಹಿಂತಿರುಗಿ "dma-buf-fs";
    ಸ್ಥಿತಿ S_MAGIC_ECRYPTFS: /* 0xF15F local */
      ಹಿಂತಿರುಗಿ "ecryptfs";
    ಸ್ಥಿತಿ S_MAGIC_EFIVARFS: /* 0xDE5E81E4 local */
      ಹಿಂತಿರುಗಿ "efivarfs";
    ಸ್ಥಿತಿ S_MAGIC_EFS: /* 0x00414A53 local */
      ಹಿಂತಿರುಗಿ "efs";
    ಸ್ಥಿತಿ S_MAGIC_EROFS_V1: /* 0xE0F5E1E2 local */
      ಹಿಂತಿರುಗಿ "erofs";
    ಸ್ಥಿತಿ S_MAGIC_EXFAT: /* 0x2011BAB0 local */
      ಹಿಂತಿರುಗಿ "exfat";
    ಸ್ಥಿತಿ S_MAGIC_EXFS: /* 0x45584653 local */
      ಹಿಂತಿರುಗಿ "exfs";
    ಸ್ಥಿತಿ S_MAGIC_EXOFS: /* 0x5DF5 local */
      ಹಿಂತಿರುಗಿ "exofs";
    ಸ್ಥಿತಿ S_MAGIC_EXT: /* 0x137D local */
      ಹಿಂತಿರುಗಿ "ext";
    ಸ್ಥಿತಿ S_MAGIC_EXT2: /* 0xEF53 local */
      ಹಿಂತಿರುಗಿ "ext2/ext3";
    ಸ್ಥಿತಿ S_MAGIC_EXT2_OLD: /* 0xEF51 local */
      ಹಿಂತಿರುಗಿ "ext2";
    ಸ್ಥಿತಿ S_MAGIC_F2FS: /* 0xF2F52010 local */
      ಹಿಂತಿರುಗಿ "f2fs";
    ಸ್ಥಿತಿ S_MAGIC_FAIL_FS: /* 0x4641494C local */
      ಹಿಂತಿರುಗಿ "failfs";
    ಸ್ಥಿತಿ S_MAGIC_FAT: /* 0x4006 local */
      ಹಿಂತಿರುಗಿ "fat";
    ಸ್ಥಿತಿ S_MAGIC_FHGFS: /* 0x19830326 remote */
      ಹಿಂತಿರುಗಿ "fhgfs";
    ಸ್ಥಿತಿ S_MAGIC_FUSE: /* 0x65735546 remote */
      ಹಿಂತಿರುಗಿ "fuse";
    ಸ್ಥಿತಿ S_MAGIC_FUSECTL: /* 0x65735543 remote */
      ಹಿಂತಿರುಗಿ "fusectl";
    ಸ್ಥಿತಿ S_MAGIC_FUTEXFS: /* 0x0BAD1DEA local */
      ಹಿಂತಿರುಗಿ "futexfs";
    ಸ್ಥಿತಿ S_MAGIC_GFS: /* 0x01161970 remote */
      ಹಿಂತಿರುಗಿ "gfs/gfs2";
    ಸ್ಥಿತಿ S_MAGIC_GPFS: /* 0x47504653 remote */
      ಹಿಂತಿರುಗಿ "gpfs";
    ಸ್ಥಿತಿ S_MAGIC_GUEST_MEMFD: /* 0x474D454D remote */
      ಹಿಂತಿರುಗಿ "guest-memfd";
    ಸ್ಥಿತಿ S_MAGIC_HFS: /* 0x4244 local */
      ಹಿಂತಿರುಗಿ "hfs";
    ಸ್ಥಿತಿ S_MAGIC_HFS_PLUS: /* 0x482B local */
      ಹಿಂತಿರುಗಿ "hfs+";
    ಸ್ಥಿತಿ S_MAGIC_HFS_X: /* 0x4858 local */
      ಹಿಂತಿರುಗಿ "hfsx";
    ಸ್ಥಿತಿ S_MAGIC_HOSTFS: /* 0x00C0FFEE local */
      ಹಿಂತಿರುಗಿ "hostfs";
    ಸ್ಥಿತಿ S_MAGIC_HPFS: /* 0xF995E849 local */
      ಹಿಂತಿರುಗಿ "hpfs";
    ಸ್ಥಿತಿ S_MAGIC_HUGETLBFS: /* 0x958458F6 local */
      ಹಿಂತಿರುಗಿ "hugetlbfs";
    ಸ್ಥಿತಿ S_MAGIC_MTD_INODE_FS: /* 0x11307854 local */
      ಹಿಂತಿರುಗಿ "inodefs";
    ಸ್ಥಿತಿ S_MAGIC_IBRIX: /* 0x013111A8 remote */
      ಹಿಂತಿರುಗಿ "ibrix";
    ಸ್ಥಿತಿ S_MAGIC_INOTIFYFS: /* 0x2BAD1DEA local */
      ಹಿಂತಿರುಗಿ "inotifyfs";
    ಸ್ಥಿತಿ S_MAGIC_ISOFS: /* 0x9660 local */
      ಹಿಂತಿರುಗಿ "isofs";
    ಸ್ಥಿತಿ S_MAGIC_ISOFS_R_WIN: /* 0x4004 local */
      ಹಿಂತಿರುಗಿ "isofs";
    ಸ್ಥಿತಿ S_MAGIC_ISOFS_WIN: /* 0x4000 local */
      ಹಿಂತಿರುಗಿ "isofs";
    ಸ್ಥಿತಿ S_MAGIC_JFFS: /* 0x07C0 local */
      ಹಿಂತಿರುಗಿ "jffs";
    ಸ್ಥಿತಿ S_MAGIC_JFFS2: /* 0x72B6 local */
      ಹಿಂತಿರುಗಿ "jffs2";
    ಸ್ಥಿತಿ S_MAGIC_JFS: /* 0x3153464A local */
      ಹಿಂತಿರುಗಿ "jfs";
    ಸ್ಥಿತಿ S_MAGIC_KAFS: /* 0x6B414653 remote */
      ಹಿಂತಿರುಗಿ "k-afs";
    ಸ್ಥಿತಿ S_MAGIC_LOGFS: /* 0xC97E8168 local */
      ಹಿಂತಿರುಗಿ "logfs";
    ಸ್ಥಿತಿ S_MAGIC_LUSTRE: /* 0x0BD00BD0 remote */
      ಹಿಂತಿರುಗಿ "lustre";
    ಸ್ಥಿತಿ S_MAGIC_M1FS: /* 0x5346314D local */
      ಹಿಂತಿರುಗಿ "m1fs";
    ಸ್ಥಿತಿ S_MAGIC_MINIX: /* 0x137F local */
      ಹಿಂತಿರುಗಿ "minix";
    ಸ್ಥಿತಿ S_MAGIC_MINIX_30: /* 0x138F local */
      ಹಿಂತಿರುಗಿ "minix (30 char.)";
    ಸ್ಥಿತಿ S_MAGIC_MINIX_V2: /* 0x2468 local */
      ಹಿಂತಿರುಗಿ "minix v2";
    ಸ್ಥಿತಿ S_MAGIC_MINIX_V2_30: /* 0x2478 local */
      ಹಿಂತಿರುಗಿ "minix v2 (30 char.)";
    ಸ್ಥಿತಿ S_MAGIC_MINIX_V3: /* 0x4D5A local */
      ಹಿಂತಿರುಗಿ "minix3";
    ಸ್ಥಿತಿ S_MAGIC_MQUEUE: /* 0x19800202 local */
      ಹಿಂತಿರುಗಿ "mqueue";
    ಸ್ಥಿತಿ S_MAGIC_MSDOS: /* 0x4D44 local */
      ಹಿಂತಿರುಗಿ "msdos";
    ಸ್ಥಿತಿ S_MAGIC_NCP: /* 0x564C remote */
      ಹಿಂತಿರುಗಿ "novell";
    ಸ್ಥಿತಿ S_MAGIC_NFS: /* 0x6969 remote */
      ಹಿಂತಿರುಗಿ "nfs";
    ಸ್ಥಿತಿ S_MAGIC_NFSD: /* 0x6E667364 remote */
      ಹಿಂತಿರುಗಿ "nfsd";
    ಸ್ಥಿತಿ S_MAGIC_NILFS: /* 0x3434 local */
      ಹಿಂತಿರುಗಿ "nilfs";
    ಸ್ಥಿತಿ S_MAGIC_NSFS: /* 0x6E736673 local */
      ಹಿಂತಿರುಗಿ "nsfs";
    ಸ್ಥಿತಿ S_MAGIC_NTFS: /* 0x5346544E local */
      ಹಿಂತಿರುಗಿ "ntfs";
    ಸ್ಥಿತಿ S_MAGIC_NULL_FS: /* 0x4E554C4C local */
      ಹಿಂತಿರುಗಿ "nullfs";
    ಸ್ಥಿತಿ S_MAGIC_OPENPROM: /* 0x9FA1 local */
      ಹಿಂತಿರುಗಿ "openprom";
    ಸ್ಥಿತಿ S_MAGIC_OCFS2: /* 0x7461636F remote */
      ಹಿಂತಿರುಗಿ "ocfs2";
    ಸ್ಥಿತಿ S_MAGIC_OVERLAYFS: /* 0x794C7630 remote */
      /* This may overlay remote file systems.
         Also there have been issues reported with inotify and overlayfs,
         so mark as "remote" so that polling is used.  */
      ಹಿಂತಿರುಗಿ "overlayfs";
    ಸ್ಥಿತಿ S_MAGIC_PANFS: /* 0xAAD7AAEA remote */
      ಹಿಂತಿರುಗಿ "panfs";
    ಸ್ಥಿತಿ S_MAGIC_PID_FS: /* 0x50494446 local */
      ಹಿಂತಿರುಗಿ "pidfs";
    ಸ್ಥಿತಿ S_MAGIC_PIPEFS: /* 0x50495045 remote */
      /* FIXME: change syntax or add an optional attribute like "inotify:no".
         pipefs is labeled as "remote" so that tail always polls,
         but it isn't really remote file system type.  */
      ಹಿಂತಿರುಗಿ "pipefs";
    ಸ್ಥಿತಿ S_MAGIC_PPC_CMM: /* 0xC7571590 local */
      ಹಿಂತಿರುಗಿ "ppc-cmm-fs";
    ಸ್ಥಿತಿ S_MAGIC_PRL_FS: /* 0x7C7C6673 remote */
      ಹಿಂತಿರುಗಿ "prl_fs";
    ಸ್ಥಿತಿ S_MAGIC_PROC: /* 0x9FA0 local */
      ಹಿಂತಿರುಗಿ "proc";
    ಸ್ಥಿತಿ S_MAGIC_PSTOREFS: /* 0x6165676C local */
      ಹಿಂತಿರುಗಿ "pstorefs";
    ಸ್ಥಿತಿ S_MAGIC_QNX4: /* 0x002F local */
      ಹಿಂತಿರುಗಿ "qnx4";
    ಸ್ಥಿತಿ S_MAGIC_QNX6: /* 0x68191122 local */
      ಹಿಂತಿರುಗಿ "qnx6";
    ಸ್ಥಿತಿ S_MAGIC_RAMFS: /* 0x858458F6 local */
      ಹಿಂತಿರುಗಿ "ramfs";
    ಸ್ಥಿತಿ S_MAGIC_RDTGROUP: /* 0x07655821 local */
      ಹಿಂತಿರುಗಿ "rdt";
    ಸ್ಥಿತಿ S_MAGIC_REISERFS: /* 0x52654973 local */
      ಹಿಂತಿರುಗಿ "reiserfs";
    ಸ್ಥಿತಿ S_MAGIC_ROMFS: /* 0x7275 local */
      ಹಿಂತಿರುಗಿ "romfs";
    ಸ್ಥಿತಿ S_MAGIC_RPC_PIPEFS: /* 0x67596969 local */
      ಹಿಂತಿರುಗಿ "rpc_pipefs";
    ಸ್ಥಿತಿ S_MAGIC_SDCARDFS: /* 0x5DCA2DF5 local */
      ಹಿಂತಿರುಗಿ "sdcardfs";
    ಸ್ಥಿತಿ S_MAGIC_SECRETMEM: /* 0x5345434D local */
      ಹಿಂತಿರುಗಿ "secretmem";
    ಸ್ಥಿತಿ S_MAGIC_SECURITYFS: /* 0x73636673 local */
      ಹಿಂತಿರುಗಿ "securityfs";
    ಸ್ಥಿತಿ S_MAGIC_SELINUX: /* 0xF97CFF8C local */
      ಹಿಂತಿರುಗಿ "selinux";
    ಸ್ಥಿತಿ S_MAGIC_SMACK: /* 0x43415D53 local */
      ಹಿಂತಿರುಗಿ "smackfs";
    ಸ್ಥಿತಿ S_MAGIC_SMB: /* 0x517B remote */
      ಹಿಂತಿರುಗಿ "smb";
    ಸ್ಥಿತಿ S_MAGIC_SMB2: /* 0xFE534D42 remote */
      ಹಿಂತಿರುಗಿ "smb2";
    ಸ್ಥಿತಿ S_MAGIC_SNFS: /* 0xBEEFDEAD remote */
      ಹಿಂತಿರುಗಿ "snfs";
    ಸ್ಥಿತಿ S_MAGIC_SOCKFS: /* 0x534F434B local */
      ಹಿಂತಿರುಗಿ "sockfs";
    ಸ್ಥಿತಿ S_MAGIC_SQUASHFS: /* 0x73717368 local */
      ಹಿಂತಿರುಗಿ "squashfs";
    ಸ್ಥಿತಿ S_MAGIC_SYSFS: /* 0x62656572 local */
      ಹಿಂತಿರುಗಿ "sysfs";
    ಸ್ಥಿತಿ S_MAGIC_SYSV2: /* 0x012FF7B6 local */
      ಹಿಂತಿರುಗಿ "sysv2";
    ಸ್ಥಿತಿ S_MAGIC_SYSV4: /* 0x012FF7B5 local */
      ಹಿಂತಿರುಗಿ "sysv4";
    ಸ್ಥಿತಿ S_MAGIC_TMPFS: /* 0x01021994 local */
      ಹಿಂತಿರುಗಿ "tmpfs";
    ಸ್ಥಿತಿ S_MAGIC_TRACEFS: /* 0x74726163 local */
      ಹಿಂತಿರುಗಿ "tracefs";
    ಸ್ಥಿತಿ S_MAGIC_UBIFS: /* 0x24051905 local */
      ಹಿಂತಿರುಗಿ "ubifs";
    ಸ್ಥಿತಿ S_MAGIC_UDF: /* 0x15013346 local */
      ಹಿಂತಿರುಗಿ "udf";
    ಸ್ಥಿತಿ S_MAGIC_UFS: /* 0x00011954 local */
      ಹಿಂತಿರುಗಿ "ufs";
    ಸ್ಥಿತಿ S_MAGIC_UFS_BYTESWAPPED: /* 0x54190100 local */
      ಹಿಂತಿರುಗಿ "ufs";
    ಸ್ಥಿತಿ S_MAGIC_USBDEVFS: /* 0x9FA2 local */
      ಹಿಂತಿರುಗಿ "usbdevfs";
    ಸ್ಥಿತಿ S_MAGIC_V9FS: /* 0x01021997 local */
      ಹಿಂತಿರುಗಿ "v9fs";
    ಸ್ಥಿತಿ S_MAGIC_VBOXSF: /* 0x786F4256 remote */
      ಹಿಂತಿರುಗಿ "vboxsf";
    ಸ್ಥಿತಿ S_MAGIC_VMHGFS: /* 0xBACBACBC remote */
      ಹಿಂತಿರುಗಿ "vmhgfs";
    ಸ್ಥಿತಿ S_MAGIC_VXFS: /* 0xA501FCF5 remote */
      /* Veritas File System can run in single instance or clustered mode,
         so mark as remote to cater for the latter case.  */
      ಹಿಂತಿರುಗಿ "vxfs";
    ಸ್ಥಿತಿ S_MAGIC_VZFS: /* 0x565A4653 local */
      ಹಿಂತಿರುಗಿ "vzfs";
    ಸ್ಥಿತಿ S_MAGIC_WSLFS: /* 0x53464846 local */
      ಹಿಂತಿರುಗಿ "wslfs";
    ಸ್ಥಿತಿ S_MAGIC_XENFS: /* 0xABBA1974 local */
      ಹಿಂತಿರುಗಿ "xenfs";
    ಸ್ಥಿತಿ S_MAGIC_XENIX: /* 0x012FF7B4 local */
      ಹಿಂತಿರುಗಿ "xenix";
    ಸ್ಥಿತಿ S_MAGIC_XFS: /* 0x58465342 local */
      ಹಿಂತಿರುಗಿ "xfs";
    ಸ್ಥಿತಿ S_MAGIC_XIAFS: /* 0x012FD16D local */
      ಹಿಂತಿರುಗಿ "xia";
    ಸ್ಥಿತಿ S_MAGIC_Z3FOLD: /* 0x0033 local */
      ಹಿಂತಿರುಗಿ "z3fold";
    ಸ್ಥಿತಿ S_MAGIC_ZFS: /* 0x2FC12FC1 local */
      ಹಿಂತಿರುಗಿ "zfs";
    ಸ್ಥಿತಿ S_MAGIC_ZONEFS: /* 0x5A4F4653 local */
      ಹಿಂತಿರುಗಿ "zonefs";
    ಸ್ಥಿತಿ S_MAGIC_ZSMALLOC: /* 0x58295829 local */
      ಹಿಂತಿರುಗಿ "zsmallocfs";


# elif __GNU__
    ಸ್ಥಿತಿ FSTYPE_UFS:
      ಹಿಂತಿರುಗಿ "ufs";
    ಸ್ಥಿತಿ FSTYPE_NFS:
      ಹಿಂತಿರುಗಿ "nfs";
    ಸ್ಥಿತಿ FSTYPE_GFS:
      ಹಿಂತಿರುಗಿ "gfs";
    ಸ್ಥಿತಿ FSTYPE_LFS:
      ಹಿಂತಿರುಗಿ "lfs";
    ಸ್ಥಿತಿ FSTYPE_SYSV:
      ಹಿಂತಿರುಗಿ "sysv";
    ಸ್ಥಿತಿ FSTYPE_FTP:
      ಹಿಂತಿರುಗಿ "ftp";
    ಸ್ಥಿತಿ FSTYPE_TAR:
      ಹಿಂತಿರುಗಿ "tar";
    ಸ್ಥಿತಿ FSTYPE_AR:
      ಹಿಂತಿರುಗಿ "ar";
    ಸ್ಥಿತಿ FSTYPE_CPIO:
      ಹಿಂತಿರುಗಿ "cpio";
    ಸ್ಥಿತಿ FSTYPE_MSLOSS:
      ಹಿಂತಿರುಗಿ "msloss";
    ಸ್ಥಿತಿ FSTYPE_CPM:
      ಹಿಂತಿರುಗಿ "cpm";
    ಸ್ಥಿತಿ FSTYPE_HFS:
      ಹಿಂತಿರುಗಿ "hfs";
    ಸ್ಥಿತಿ FSTYPE_DTFS:
      ಹಿಂತಿರುಗಿ "dtfs";
    ಸ್ಥಿತಿ FSTYPE_GRFS:
      ಹಿಂತಿರುಗಿ "grfs";
    ಸ್ಥಿತಿ FSTYPE_ಪದ:
      ಹಿಂತಿರುಗಿ "term";
    ಸ್ಥಿತಿ FSTYPE_DEV:
      ಹಿಂತಿರುಗಿ "dev";
    ಸ್ಥಿತಿ FSTYPE_PROC:
      ಹಿಂತಿರುಗಿ "proc";
    ಸ್ಥಿತಿ FSTYPE_IFSOCK:
      ಹಿಂತಿರುಗಿ "ifsock";
    ಸ್ಥಿತಿ FSTYPE_AFS:
      ಹಿಂತಿರುಗಿ "afs";
    ಸ್ಥಿತಿ FSTYPE_DFS:
      ಹಿಂತಿರುಗಿ "dfs";
    ಸ್ಥಿತಿ FSTYPE_PROC9:
      ಹಿಂತಿರುಗಿ "proc9";
    ಸ್ಥಿತಿ FSTYPE_SOCKET:
      ಹಿಂತಿರುಗಿ "socket";
    ಸ್ಥಿತಿ FSTYPE_MISC:
      ಹಿಂತಿರುಗಿ "misc";
    ಸ್ಥಿತಿ FSTYPE_EXT2FS:
      ಹಿಂತಿರುಗಿ "ext2/ext3";
    ಸ್ಥಿತಿ FSTYPE_HTTP:
      ಹಿಂತಿರುಗಿ "http";
    ಸ್ಥಿತಿ FSTYPE_MEMFS:
      ಹಿಂತಿರುಗಿ "memfs";
    ಸ್ಥಿತಿ FSTYPE_ISO9660:
      ಹಿಂತಿರುಗಿ "iso9660";
# endif
    ಪೂರ್ವನಿಯೋಜಿತ:
      {
        ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ type = statfsbuf->f_type;
        ಸ್ಥಿರ ಅಕ್ಷರ buf[ಗಾತ್ರ "UNKNOWN (0x%lx)" - 3
                        + (ಗಾತ್ರ type * CHAR_BIT + 3) / 4];
        sprintf (buf, "UNKNOWN (0x%lx)", type);
        ಹಿಂತಿರುಗಿ buf;
      }
    }
#ಆದರೆ_ಅಂತ್ಯ
}

NODISCARD
ಸ್ಥಿರ ಅಕ್ಷರ *
human_access (ರಚನೆ stat ಸ್ಥಿರಾಂಕ *statbuf)
{
  ಸ್ಥಿರ ಅಕ್ಷರ modebuf[12];
  filemodestring (statbuf, modebuf);
  modebuf[10] = 0;
  ಹಿಂತಿರುಗಿ modebuf;
}

NODISCARD
ಸ್ಥಿರ ಅಕ್ಷರ *
human_ಸಮಯ (ರಚನೆ timespec t)
{
  /* STR must be at least INT_BUFSIZE_BOUND (intmax_t) big, either
     because localtime_rz fails, or because the time zone is truly
     outlandish so that %z expands to a long string.  */
  ಸ್ಥಿರ ಅಕ್ಷರ str[INT_BUFSIZE_BOUND (intmax_t)
                  + INT_STRLEN_BOUND (ಪೂರ್ಣಾಂಕ) /* YYYY */
                  + 1 /* because YYYY might equal INT_MAX + 1900 */
                  + ಗಾತ್ರ "-MM-DD HH:MM:SS.NNNNNNNNN +"];
  ಸ್ಥಿರ timezone_t tz;
  ಆದರೆ (!tz)
    tz = tzalloc (getenv ("TZ"));
  ರಚನೆ tm tm;
  ಪೂರ್ಣಾಂಕ ns = t.tv_nsec;
  ಆದರೆ (localtime_rz (tz, &t.tv_sec, &tm))
    nstrftime (str, ಗಾತ್ರ str, "%Y-%m-%d %H:%M:%S.%N %z", &tm, tz, ns);
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಅಕ್ಷರ secbuf[INT_BUFSIZE_BOUND (intmax_t)];
      sprintf (str, "%s.%09d", timetostr (t.tv_sec, secbuf), ns);
    }
  ಹಿಂತಿರುಗಿ str;
}

/* PFORMAT points to a '%' followed by a prefix of a format, all of
   size PREFIX_LEN.  The flags allowed for this format are
   ALLOWED_FLAGS; remove other printf flags from the prefix, then
   append SUFFIX.  */
ಸ್ಥಿರ ಶೂನ್ಯ
make_format (ಅಕ್ಷರ *pformat, size_t ಪೂರ್ವಪ್ರತ್ಯಯ_len, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *allowed_flags,
             ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *suffix)
{
  ಅಕ್ಷರ *dst = pformat + 1;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *src;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *srclim = pformat + ಪೂರ್ವಪ್ರತ್ಯಯ_len;
  ಗಾಗಿ (src = dst; src < srclim && strchr (printf_flags, *src); src++)
    ಆದರೆ (strchr (allowed_flags, *src))
      *dst++ = *src;
  ಆಗ (src < srclim)
    *dst++ = *src++;
  strcpy (dst, suffix);
}

ಸ್ಥಿರ ಶೂನ್ಯ
ಹೊರಹೋಗು_string (ಅಕ್ಷರ *pformat, size_t ಪೂರ್ವಪ್ರತ್ಯಯ_len, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *arg)
{
  make_format (pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len, "-", "s");
  ಬರೆ (pformat, arg);
}
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ಹೊರಹೋಗು_int (ಅಕ್ಷರ *pformat, size_t ಪೂರ್ವಪ್ರತ್ಯಯ_len, intmax_t arg)
{
  make_format (pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len, "'-+ 0", "jd");
  ಹಿಂತಿರುಗಿ ಬರೆ (pformat, arg);
}
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ಹೊರಹೋಗು_uint (ಅಕ್ಷರ *pformat, size_t ಪೂರ್ವಪ್ರತ್ಯಯ_len, uintmax_t arg)
{
  make_format (pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len, "'-0", "ju");
  ಹಿಂತಿರುಗಿ ಬರೆ (pformat, arg);
}
ಸ್ಥಿರ ಶೂನ್ಯ
ಹೊರಹೋಗು_uint_o (ಅಕ್ಷರ *pformat, size_t ಪೂರ್ವಪ್ರತ್ಯಯ_len, uintmax_t arg)
{
  make_format (pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len, "-#0", "jo");
  ಬರೆ (pformat, arg);
}
ಸ್ಥಿರ ಶೂನ್ಯ
ಹೊರಹೋಗು_uint_x (ಅಕ್ಷರ *pformat, size_t ಪೂರ್ವಪ್ರತ್ಯಯ_len, uintmax_t arg)
{
  make_format (pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len, "-#0", "jx");
  ಬರೆ (pformat, arg);
}
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ಹೊರಹೋಗು_minus_zero (ಅಕ್ಷರ *pformat, size_t ಪೂರ್ವಪ್ರತ್ಯಯ_len)
{
  make_format (pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len, "'-+ 0", ".0f");
  ಹಿಂತಿರುಗಿ ಬರೆ (pformat, -0.25);
}

/* Output the number of seconds since the Epoch, using a format that
   acts like printf's %f format.  */
ಸ್ಥಿರ ಶೂನ್ಯ
ಹೊರಹೋಗು_epoch_sec (ಅಕ್ಷರ *pformat, size_t ಪೂರ್ವಪ್ರತ್ಯಯ_len,
               ರಚನೆ timespec arg)
{
  ಅಕ್ಷರ *dot = memchr (pformat, '.', ಪೂರ್ವಪ್ರತ್ಯಯ_len);
  size_t sec_ಪೂರ್ವಪ್ರತ್ಯಯ_len = ಪೂರ್ವಪ್ರತ್ಯಯ_len;
  ಪೂರ್ಣಾಂಕ ಅಗಲ = 0;
  ಪೂರ್ಣಾಂಕ precision = 0;
  bool frac_ಎಡ_adjust = false;

  ಆದರೆ (dot)
    {
      sec_ಪೂರ್ವಪ್ರತ್ಯಯ_len = dot - pformat;
      pformat[ಪೂರ್ವಪ್ರತ್ಯಯ_len] = '\0';

      ಆದರೆ (c_isdigit (dot[1]))
        {
          ದೀರ್ಘ ಪೂರ್ಣಾಂಕ lprec = strtol (dot + 1, ಖಾಲಿ, 10);
          precision = (lprec <= ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ ? lprec : ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ);
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          precision = 9;
        }

      ಆದರೆ (precision && c_isdigit (dot[-1]))
        {
          /* If a nontrivial width is given, subtract the width of the
             decimal point and PRECISION digits that will be output
             later.  */
          ಅಕ್ಷರ *p = dot;
          *dot = '\0';

          ಮಾಡು
            --p;
          ಆಗ (c_isdigit (p[-1]));

          ದೀರ್ಘ ಪೂರ್ಣಾಂಕ lwidth = strtol (p, ಖಾಲಿ, 10);
          ಅಗಲ = (lwidth <= ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ ? lwidth : ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ);
          ಆದರೆ (1 < ಅಗಲ)
            {
              p += (*p == '0');
              sec_ಪೂರ್ವಪ್ರತ್ಯಯ_len = p - pformat;
              ಪೂರ್ಣಾಂಕ w_d = (decimal_ಬಿಂದು_len < ಅಗಲ
                         ? ಅಗಲ - decimal_ಬಿಂದು_len
                         : 0);
              ಆದರೆ (1 < w_d)
                {
                  ಪೂರ್ಣಾಂಕ w = w_d - precision;
                  ಆದರೆ (1 < w)
                    {
                      ಅಕ್ಷರ *dst = pformat;
                      ಗಾಗಿ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *src = dst; src < p; src++)
                        {
                          ಆದರೆ (*src == '-')
                            frac_ಎಡ_adjust = true;
                          ಇಲ್ಲದಿದ್ದರೆ
                            *dst++ = *src;
                        }
                      sec_ಪೂರ್ವಪ್ರತ್ಯಯ_len =
                        (dst - pformat
                         + (frac_ಎಡ_adjust ? 0 : sprintf (dst, "%d", w)));
                    }
                }
            }
        }
    }

  ಪೂರ್ಣಾಂಕ ಭಾಜಕ = 1;
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = precision; i < 9; i++)
    ಭಾಜಕ *= 10;
  ಪೂರ್ಣಾಂಕ frac_sec = arg.tv_nsec / ಭಾಜಕ;
  ಪೂರ್ಣಾಂಕ int_len;

  ಆದರೆ (TYPE_SIGNED (time_t))
    {
      bool minus_zero = false;
      ಆದರೆ (arg.tv_sec < 0 && arg.tv_nsec != 0)
        {
          ಪೂರ್ಣಾಂಕ frac_sec_modulus = 1000000000 / ಭಾಜಕ;
          frac_sec = (frac_sec_modulus - frac_sec
                      - (arg.tv_nsec % ಭಾಜಕ != 0));
          arg.tv_sec += (frac_sec != 0);
          minus_zero = (arg.tv_sec == 0);
        }
      int_len = (minus_zero
                 ? ಹೊರಹೋಗು_minus_zero (pformat, sec_ಪೂರ್ವಪ್ರತ್ಯಯ_len)
                 : ಹೊರಹೋಗು_int (pformat, sec_ಪೂರ್ವಪ್ರತ್ಯಯ_len, arg.tv_sec));
    }
  ಇಲ್ಲದಿದ್ದರೆ
    int_len = ಹೊರಹೋಗು_uint (pformat, sec_ಪೂರ್ವಪ್ರತ್ಯಯ_len, arg.tv_sec);

  ಆದರೆ (precision)
    {
      ಪೂರ್ಣಾಂಕ prec = (precision < 9 ? precision : 9);
      ಪೂರ್ಣಾಂಕ trailing_prec = precision - prec;
      ಪೂರ್ಣಾಂಕ ilen = (int_len < 0 ? 0 : int_len);
      ಪೂರ್ಣಾಂಕ trailing_ಅಗಲ = (ilen < ಅಗಲ && decimal_ಬಿಂದು_len < ಅಗಲ - ilen
                            ? ಅಗಲ - ilen - decimal_ಬಿಂದು_len - prec
                            : 0);
      ಬರೆ ("%s%.*d%-*.*d", decimal_ಬಿಂದು, prec, frac_sec,
              trailing_ಅಗಲ, trailing_prec, 0);
    }
}

/* Print the context information of FILENAME, and return true iff the
   context could not be obtained.  */
NODISCARD
ಸ್ಥಿರ bool
ಹೊರಹೋಗು_file_context (ಅಕ್ಷರ *pformat, size_t ಪೂರ್ವಪ್ರತ್ಯಯ_len, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename)
{
  ಅಕ್ಷರ *scontext;
  bool fail = false;

  ಆದರೆ ((follow_links
       ? getfilecon (filename, &scontext)
       : lgetfilecon (filename, &scontext)) < 0)
    {
      error (0, errno, _("failed to get security context of %s"),
             quoteaf (filename));
      scontext = ಖಾಲಿ;
      fail = true;
    }
  strcpy (pformat + ಪೂರ್ವಪ್ರತ್ಯಯ_len, "s");
  ಬರೆ (pformat, (scontext ? scontext : "?"));
  ಆದರೆ (scontext)
    freecon (scontext);
  ಹಿಂತಿರುಗಿ fail;
}

/* Set the quoting style once it is needed.  */
ಸ್ಥಿರ ಶೂನ್ಯ
initialize_quoting_style (ಶೂನ್ಯ)
{
  ಸ್ಥಿರ bool initialized;
  ಆದರೆ (!initialized)
    {
      initialized = true;
      ಪೂರ್ಣಾಂಕ qs = getenv_quoting_style ();
      ಗಣ_quoting_style (ಖಾಲಿ, 0 <= qs ? qs : shell_escape_quoting_style);
    }
}

/* Print statfs info.  Return zero upon success, nonzero upon failure.  */
NODISCARD
ಸ್ಥಿರ bool
print_statfs (ಅಕ್ಷರ *pformat, size_t ಪೂರ್ವಪ್ರತ್ಯಯ_len, MAYBE_ಬಳಸದ ಅಕ್ಷರ mod, ಅಕ್ಷರ m,
              MAYBE_ಬಳಸದ ಪೂರ್ಣಾಂಕ fd, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename,
              ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *ದತ್ತಾಂಶ)
{
  STRUCT_STATVFS ಸ್ಥಿರಾಂಕ *statfsbuf = ದತ್ತಾಂಶ;
  bool fail = false;

  ಆಯ್ಕೆ (m)
    {
    ಸ್ಥಿತಿ 'n':
      ಆದರೆ (mod == 'Q')
        {
          initialize_quoting_style ();
          filename = quoteN (filename);
        }
      ಹೊರಹೋಗು_string (pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len, filename);
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ 'i':
      {
#ಆದರೆ STRUCT_STATXFS_F_FSID_IS_INTEGER
        uintmax_t fsid = statfsbuf->f_fsid;
#ಇಲ್ಲದಿದ್ದರೆ
        ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ fsid_ಶಬ್ದ;
        static_assert (alignof (STRUCT_STATVFS) % alignof (fsid_ಶಬ್ದ) == 0);
        static_assert (offsetof (STRUCT_STATVFS, f_fsid) % alignof (fsid_ಶಬ್ದ)
                       == 0);
        static_assert (ಗಾತ್ರ statfsbuf->f_fsid % alignof (fsid_ಶಬ್ದ) == 0);
        fsid_ಶಬ್ದ ಸ್ಥಿರಾಂಕ *p = (fsid_ಶಬ್ದ *) &statfsbuf->f_fsid;

        /* Assume a little-endian word order, as that is compatible
           with glibc's statvfs implementation.  */
        uintmax_t fsid = 0;
        ಪೂರ್ಣಾಂಕ ಪದಗಳು = ಗಾತ್ರ statfsbuf->f_fsid / ಗಾತ್ರ *p;
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಪದಗಳು && i * ಗಾತ್ರ *p < ಗಾತ್ರ fsid; i++)
          {
            uintmax_t u = p[ಪದಗಳು - 1 - i];
            fsid |= u << (i * CHAR_BIT * ಗಾತ್ರ *p);
          }
#ಆದರೆ_ಅಂತ್ಯ
        ಹೊರಹೋಗು_uint_x (pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len, fsid);
      }
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ 'l':
      ಹೊರಹೋಗು_NAMEMAX (pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len, SB_F_NAMEMAX (statfsbuf));
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 't':
#ಆದರೆ HAVE_STRUCT_STATXFS_F_TYPE
      ಹೊರಹೋಗು_uint_x (pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len, statfsbuf->f_type);
#ಇಲ್ಲದಿದ್ದರೆ
      fputc ('?', stdout);
#ಆದರೆ_ಅಂತ್ಯ
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'T':
      ಹೊರಹೋಗು_string (pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len, human_fstype (statfsbuf));
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'b':
      ಹೊರಹೋಗು_int (pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len, statfsbuf->f_ಖಂಡಗಳು);
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'f':
      ಹೊರಹೋಗು_int (pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len, statfsbuf->f_bfree);
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'a':
      ಹೊರಹೋಗು_int (pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len, statfsbuf->f_bavail);
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 's':
      ಹೊರಹೋಗು_uint (pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len, statfsbuf->f_bsize);
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'S':
      {
        uintmax_t frsize = STATFS_FRSIZE (statfsbuf);
        ಆದರೆ (! frsize)
          frsize = statfsbuf->f_bsize;
        ಹೊರಹೋಗು_uint (pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len, frsize);
      }
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'c':
      ಹೊರಹೋಗು_uint (pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len, statfsbuf->f_files);
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'd':
      ಹೊರಹೋಗು_int (pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len, statfsbuf->f_ffree);
      ನಿಲ್ಲಿಸು;
    ಪೂರ್ವನಿಯೋಜಿತ:
      fputc ('?', stdout);
      ನಿಲ್ಲಿಸು;
    }
  ಹಿಂತಿರುಗಿ fail;
}

/* Return any bind mounted source for a path.
   The caller should not free the returned buffer.
   Return NULL if no bind mount found.  */
NODISCARD
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *
ಕಂಡುಹಿಡಿ_bind_mount (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ * name)
{
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ * bind_mount = ಖಾಲಿ;

  ಸ್ಥಿರ ರಚನೆ mount_ನಮೂದು *mount_list;
  ಸ್ಥಿರ bool tried_mount_list = false;
  ಆದರೆ (!tried_mount_list) /* attempt/warn once per process.  */
    {
      ಆದರೆ (!(mount_list = read_file_system_list (false)))
        error (0, errno, "%s", _("cannot read table of mounted file systems"));
      tried_mount_list = true;
    }

  ರಚನೆ stat name_stats;
  ಆದರೆ (stat (name, &name_stats) != 0)
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;

  ಗಾಗಿ (ರಚನೆ mount_ನಮೂದು *me = mount_list; me; me = me->me_ಮುಂದಿನ)
    {
      ಆದರೆ (me->me_dummy && me->me_devname[0] == '/'
          && streq (me->me_mountdir, name))
        {
          ರಚನೆ stat dev_stats;

          ಆದರೆ (stat (me->me_devname, &dev_stats) == 0
              && psame_inode (&name_stats, &dev_stats))
            {
              bind_mount = me->me_devname;
              ನಿಲ್ಲಿಸು;
            }
        }
    }

  ಹಿಂತಿರುಗಿ bind_mount;
}

/* Print mount point.  Return zero upon success, nonzero upon failure.  */
NODISCARD
ಸ್ಥಿರ bool
ಹೊರಹೋಗು_mount_ಬಿಂದು (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename, ಅಕ್ಷರ *pformat, size_t ಪೂರ್ವಪ್ರತ್ಯಯ_len,
                 ಸ್ಥಿರಾಂಕ ರಚನೆ stat *statp)
{

  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *np = "?", *bp = ಖಾಲಿ;
  ಅಕ್ಷರ *mp = ಖಾಲಿ;
  bool fail = true;

  /* Look for bind mounts first.  Note we output the immediate alias,
     rather than further resolving to a base device mount point.  */
  ಆದರೆ (follow_links || !S_ISLNK (statp->st_ಬಹುಲಕ))
    {
      ಅಕ್ಷರ *resolved = canonicalize_file_name (filename);
      ಆದರೆ (!resolved)
        {
          error (0, errno, _("failed to canonicalize %s"), quoteaf (filename));
          ಹೋಗು print_mount_ಬಿಂದು;
        }
      bp = ಕಂಡುಹಿಡಿ_bind_mount (resolved);
      ಬಿಡುಗಡೆ (resolved);
      ಆದರೆ (bp)
        {
          fail = false;
          ಹೋಗು print_mount_ಬಿಂದು;
        }
    }

  /* If there is no direct bind mount, then navigate
     back up the tree looking for a device change.
     Note we don't detect if any of the directory components
     are bind mounted to the same device, but that's OK
     since we've not directly queried them.  */
  ಆದರೆ ((mp = ಕಂಡುಹಿಡಿ_mount_ಬಿಂದು (filename, statp)))
    {
      /* This dir might be bind mounted to another device,
         so we resolve the bound source in that case also.  */
      bp = ಕಂಡುಹಿಡಿ_bind_mount (mp);
      fail = false;
    }

print_mount_ಬಿಂದು:

  ಹೊರಹೋಗು_string (pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len, bp ? bp : mp ? mp : np);
  ಬಿಡುಗಡೆ (mp);
  ಹಿಂತಿರುಗಿ fail;
}

/* Map a TS with negative TS.tv_nsec to {0,0}.  */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ timespec
neg_ಗಮ್ಯ_zero (ರಚನೆ timespec ts)
{
  ಆದರೆ (0 <= ts.tv_nsec)
    ಹಿಂತಿರುಗಿ ts;
  ರಚನೆ timespec z = {0};
  ಹಿಂತಿರುಗಿ z;
}

/* Output a single-character \ escape.  */

ಸ್ಥಿರ ಶೂನ್ಯ
print_esc_char (ಅಕ್ಷರ c)
{
  ಆಯ್ಕೆ (c)
    {
    ಸ್ಥಿತಿ 'a':			/* Alert. */
      c ='\a';
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'b':			/* Backspace. */
      c ='\b';
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'e':			/* Escape. */
      c ='\x1B';
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'f':			/* Form feed. */
      c ='\f';
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'n':			/* New line. */
      c ='\n';
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'r':			/* Carriage return. */
      c ='\r';
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 't':			/* Horizontal tab. */
      c ='\t';
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'v':			/* Vertical tab. */
      c ='\v';
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ '"':
    ಸ್ಥಿತಿ '\\':
      ನಿಲ್ಲಿಸು;
    ಪೂರ್ವನಿಯೋಜಿತ:
      error (0, 0, _("warning: unrecognized escape '\\%c'"), c);
      ನಿಲ್ಲಿಸು;
    }
  ಅಕ್ಷರ_ಬರೆ (c);
}

ATTRIBUTE_ಶುದ್ಧ
ಸ್ಥಿರ size_t
format_ಸಂಕೇತ_ಸ್ಥಾನಾಂತರ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *directive)
{
  size_t len = strspn (directive + 1, printf_flags);
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *fmt_char = directive + len + 1;
  fmt_char += strspn (fmt_char, digits);
  ಆದರೆ (*fmt_char == '.')
    fmt_char += 1 + strspn (fmt_char + 1, digits);
  ಹಿಂತಿರುಗಿ fmt_char - directive;
}

/* Print the information specified by the format string, FORMAT,
   calling PRINT_FUNC for each %-directive encountered.
   Return zero upon success, nonzero upon failure.  */
NODISCARD
ಸ್ಥಿರ bool
print_it (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *format, ಪೂರ್ಣಾಂಕ fd, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename,
          bool (*print_func) (ಅಕ್ಷರ *, size_t, ಅಕ್ಷರ, ಅಕ್ಷರ,
                              ಪೂರ್ಣಾಂಕ, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *, ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *),
          ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *ದತ್ತಾಂಶ)
{
  bool fail = false;

  /* Add 2 to accommodate our conversion of the stat '%s' format string
     to the longer printf '%llu' one.  */
  ಎಣಿಕೆ
    {
      MAX_ADDITIONAL_ಬೈಟುಗಳು =
        (MAX (ಗಾತ್ರ "jd",
              MAX (ಗಾತ್ರ "jo", MAX (ಗಾತ್ರ "ju", ಗಾತ್ರ "jx")))
         - 1)
    };
  size_t n_alloc = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (format) + MAX_ADDITIONAL_ಬೈಟುಗಳು + 1;
  ಅಕ್ಷರ *dest = xmalloc (n_alloc);
  ಗಾಗಿ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *b = format; *b; b++)
    {
      ಆಯ್ಕೆ (*b)
        {
        ಸ್ಥಿತಿ '%':
          {
            size_t len = format_ಸಂಕೇತ_ಸ್ಥಾನಾಂತರ (b);
            ಅಕ್ಷರ fmt_char = *(b + len);
            ಅಕ್ಷರ mod_char = 0;
            ಸ್ಮರಣೆ_ನಕಲು (dest, b, len);
            b += len;

            ಆಯ್ಕೆ (fmt_char)
              {
              ಸ್ಥಿತಿ '\0':
                --b;
                FALLTHROUGH;
              ಸ್ಥಿತಿ '%':
                ಆದರೆ (1 < len)
                  {
                    dest[len] = fmt_char;
                    dest[len + 1] = '\0';
                    error (EXIT_FAILURE, 0, _("%s: invalid directive"),
                           quote (dest));
                  }
                ಅಕ್ಷರ_ಬರೆ ('%');
                ನಿಲ್ಲಿಸು;
              ಸ್ಥಿತಿ 'H':
              ಸ್ಥಿತಿ 'L':
              ಸ್ಥಿತಿ 'Q':
                mod_char = fmt_char;
                fmt_char = *(b + 1);
                ಆದರೆ ((mod_char == 'Q' && fmt_char == 'n')
                    || (print_func == print_stat
                        && (mod_char == 'H' || mod_char == 'L')
                        && (fmt_char == 'd' || fmt_char == 'r')))
                  {
                    b++;
                  }
                ಇಲ್ಲದಿದ್ದರೆ
                  {
                    fmt_char = mod_char;
                    mod_char = 0;
                  }
                FALLTHROUGH;
              ಪೂರ್ವನಿಯೋಜಿತ:
                fail |= print_func (dest, len, mod_char, fmt_char,
                                    fd, filename, ದತ್ತಾಂಶ);
                ನಿಲ್ಲಿಸು;
              }
            ನಿಲ್ಲಿಸು;
          }

        ಸ್ಥಿತಿ '\\':
          ಆದರೆ ( ! interpret_backslash_escapes)
            {
              ಅಕ್ಷರ_ಬರೆ ('\\');
              ನಿಲ್ಲಿಸು;
            }
          ++b;
          ಆದರೆ (isoct (*b))
            {
              ಪೂರ್ಣಾಂಕ esc_ಮೌಲ್ಯ = fromoct (*b);
              ಪೂರ್ಣಾಂಕ esc_ಉದ್ದಳತೆ = 1;	/* number of octal digits */
              ಗಾಗಿ (++b; esc_ಉದ್ದಳತೆ < 3 && isoct (*b);
                   ++esc_ಉದ್ದಳತೆ, ++b)
                {
                  esc_ಮೌಲ್ಯ = esc_ಮೌಲ್ಯ * 8 + fromoct (*b);
                }
              ಅಕ್ಷರ_ಬರೆ (esc_ಮೌಲ್ಯ);
              --b;
            }
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (*b == 'x' && c_isxdigit (b[1]))
            {
              ಪೂರ್ಣಾಂಕ esc_ಮೌಲ್ಯ = fromhex (b[1]);	/* Value of \xhh escape. */
              /* A hexadecimal \xhh escape sequence must have
                 1 or 2 hex. digits.  */
              ++b;
              ಆದರೆ (c_isxdigit (b[1]))
                {
                  ++b;
                  esc_ಮೌಲ್ಯ = esc_ಮೌಲ್ಯ * 16 + fromhex (*b);
                }
              ಅಕ್ಷರ_ಬರೆ (esc_ಮೌಲ್ಯ);
            }
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (*b == '\0')
            {
              error (0, 0, _("warning: backslash at end of format"));
              ಅಕ್ಷರ_ಬರೆ ('\\');
              /* Arrange to exit the loop.  */
              --b;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              print_esc_char (*b);
            }
          ನಿಲ್ಲಿಸು;

        ಪೂರ್ವನಿಯೋಜಿತ:
          ಅಕ್ಷರ_ಬರೆ (*b);
          ನಿಲ್ಲಿಸು;
        }
    }
  ಬಿಡುಗಡೆ (dest);

  fputs (trailing_delim, stdout);

  ಹಿಂತಿರುಗಿ fail;
}

/* Stat the file system and print what we find.  */
NODISCARD
ಸ್ಥಿರ bool
do_statfs (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *format)
{
  STRUCT_STATVFS statfsbuf;

  ಆದರೆ (streq (filename, "-"))
    {
      error (0, 0, _("using %s to denote standard input does not work"
                     " in file system mode"), quoteaf (filename));
      ಹಿಂತಿರುಗಿ false;
    }

  ಆದರೆ (STATFS (filename, &statfsbuf) != 0)
    {
      error (0, errno, _("cannot read file system information for %s"),
             quoteaf (filename));
      ಹಿಂತಿರುಗಿ false;
    }

  bool fail = print_it (format, -1, filename, print_statfs, &statfsbuf);
  ಹಿಂತಿರುಗಿ ! fail;
}

ರಚನೆ print_ವಾದಗಳು {
  ರಚನೆ stat *st;
  ರಚನೆ timespec btime;
};

/* Ask statx to avoid syncing? */
ಸ್ಥಿರ bool dont_sync;

/* Ask statx to force sync? */
ಸ್ಥಿರ bool force_sync;

#ಆದರೆ USE_STATX
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ
fmt_ಗಮ್ಯ_ಮುಖವಾಡ (ಅಕ್ಷರ fmt)
{
  ಆಯ್ಕೆ (fmt)
    {
    ಸ್ಥಿತಿ 'N':
      ಹಿಂತಿರುಗಿ STATX_ಬಹುಲಕ;
    ಸ್ಥಿತಿ 'd':
    ಸ್ಥಿತಿ 'D':
      ಹಿಂತಿರುಗಿ STATX_ಬಹುಲಕ;
    ಸ್ಥಿತಿ 'i':
      ಹಿಂತಿರುಗಿ STATX_INO;
    ಸ್ಥಿತಿ 'a':
    ಸ್ಥಿತಿ 'A':
      ಹಿಂತಿರುಗಿ STATX_ಬಹುಲಕ;
    ಸ್ಥಿತಿ 'f':
      ಹಿಂತಿರುಗಿ STATX_ಬಹುಲಕ|STATX_TYPE;
    ಸ್ಥಿತಿ 'F':
      ಹಿಂತಿರುಗಿ STATX_TYPE;
    ಸ್ಥಿತಿ 'h':
      ಹಿಂತಿರುಗಿ STATX_NLINK;
    ಸ್ಥಿತಿ 'u':
    ಸ್ಥಿತಿ 'U':
      ಹಿಂತಿರುಗಿ STATX_UID;
    ಸ್ಥಿತಿ 'g':
    ಸ್ಥಿತಿ 'G':
      ಹಿಂತಿರುಗಿ STATX_GID;
    ಸ್ಥಿತಿ 'm':
      ಹಿಂತಿರುಗಿ STATX_ಬಹುಲಕ|STATX_INO;
    ಸ್ಥಿತಿ 's':
      ಹಿಂತಿರುಗಿ STATX_ಗಾತ್ರ;
    ಸ್ಥಿತಿ 't':
    ಸ್ಥಿತಿ 'T':
      ಹಿಂತಿರುಗಿ STATX_ಬಹುಲಕ;
    ಸ್ಥಿತಿ 'b':
      ಹಿಂತಿರುಗಿ STATX_ಖಂಡಗಳು;
    ಸ್ಥಿತಿ 'w':
    ಸ್ಥಿತಿ 'W':
      ಹಿಂತಿರುಗಿ STATX_BTIME;
    ಸ್ಥಿತಿ 'x':
    ಸ್ಥಿತಿ 'X':
      ಹಿಂತಿರುಗಿ STATX_ATIME;
    ಸ್ಥಿತಿ 'y':
    ಸ್ಥಿತಿ 'Y':
      ಹಿಂತಿರುಗಿ STATX_MTIME;
    ಸ್ಥಿತಿ 'z':
    ಸ್ಥಿತಿ 'Z':
      ಹಿಂತಿರುಗಿ STATX_CTIME;
    }
  ಹಿಂತಿರುಗಿ 0;
}

ATTRIBUTE_ಶುದ್ಧ
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ
format_ಗಮ್ಯ_ಮುಖವಾಡ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *format)
{
  ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಮುಖವಾಡ = 0;

  ಗಾಗಿ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *b = format; *b; b++)
    {
      ಆದರೆ (*b != '%')
        ಮುಂದುವರಿ;

      b += format_ಸಂಕೇತ_ಸ್ಥಾನಾಂತರ (b);
      ಆದರೆ (*b == '\0')
        ನಿಲ್ಲಿಸು;
      ಮುಖವಾಡ |= fmt_ಗಮ್ಯ_ಮುಖವಾಡ (*b);
    }
  ಹಿಂತಿರುಗಿ ಮುಖವಾಡ;
}

/* statx the file and print what we find */
NODISCARD
ಸ್ಥಿರ bool
do_stat (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *format, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *format2)
{
  ಪೂರ್ಣಾಂಕ fd = streq (filename, "-") ? 0 : AT_FDCWD;
  ಪೂರ್ಣಾಂಕ flags = 0;
  ರಚನೆ stat st;
  ರಚನೆ statx stx = {0};
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *pathname = filename;
  ರಚನೆ print_ವಾದಗಳು pa;
  pa.st = &st;
  pa.btime = (ರಚನೆ timespec) {.tv_sec = -1, .tv_nsec = -1};

  ಆದರೆ (AT_FDCWD != fd)
    {
      pathname = "";
      flags = AT_ಖಾಲಿಗಳು_ಮಾರ್ಗ;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!follow_links)
    {
      flags = AT_SYMLINK_NOFOLLOW;
    }

  ಆದರೆ (dont_sync)
    flags |= AT_STATX_DONT_SYNC;
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (force_sync)
    flags |= AT_STATX_FORCE_SYNC;

  ಆದರೆ (! force_sync)
    flags |= AT_NO_AUTOMOUNT;

  fd = statx (fd, pathname, flags, format_ಗಮ್ಯ_ಮುಖವಾಡ (format), &stx);
  ಆದರೆ (fd < 0)
    {
      ಆದರೆ (flags & AT_ಖಾಲಿಗಳು_ಮಾರ್ಗ)
        error (0, errno, _("cannot stat standard input"));
      ಇಲ್ಲದಿದ್ದರೆ
        error (0, errno, _("cannot statx %s"), quoteaf (filename));
      ಹಿಂತಿರುಗಿ false;
    }

  ಆದರೆ (S_ISBLK (stx.stx_ಬಹುಲಕ) || S_ISCHR (stx.stx_ಬಹುಲಕ))
    format = format2;

  statx_ಗಮ್ಯ_stat (&stx, &st);
  ಆದರೆ (stx.stx_ಮುಖವಾಡ & STATX_BTIME)
    pa.btime = statx_timestamp_ಗಮ್ಯ_timespec (stx.stx_btime);

  bool fail = print_it (format, fd, filename, print_stat, &pa);
  ಹಿಂತಿರುಗಿ ! fail;
}

#ಇಲ್ಲದಿದ್ದರೆ /* USE_STATX */

ಸ್ಥಿರ ರಚನೆ timespec
get_birthtime (ಪೂರ್ಣಾಂಕ fd, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename, ರಚನೆ stat ಸ್ಥಿರಾಂಕ *st)
{
  ರಚನೆ timespec ts = get_stat_birthtime (st);

# ಆದರೆ HAVE_GETATTRAT
  ಆದರೆ (ts.tv_nsec < 0)
    {
      nvlist_t *response;
      ಆದರೆ ((fd < 0
           ? getattrat (AT_FDCWD, XATTR_VIEW_READWRITE, filename, &response)
           : fgetattr (fd, XATTR_VIEW_READWRITE, &response))
          == 0)
        {
          uint64_t *val;
          uint_t n;
          ಆದರೆ (nvlist_lookup_uint64_array (response, A_CRTIME, &val, &n) == 0
              && 2 <= n
              && val[0] <= TYPE_ಗರಿಷ್ಠ (time_t)
              && val[1] < 1000000000 * 2 /* for leap seconds */)
            {
              ts.tv_sec = val[0];
              ts.tv_nsec = val[1];
            }
          nvlist_free (response);
        }
    }
# endif

  ಹಿಂತಿರುಗಿ ts;
}


/* stat the file and print what we find */
NODISCARD
ಸ್ಥಿರ bool
do_stat (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *format,
         ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *format2)
{
  ಪೂರ್ಣಾಂಕ fd = streq (filename, "-") ? 0 : -1;
  ರಚನೆ stat statbuf;
  ರಚನೆ print_ವಾದಗಳು pa;
  pa.st = &statbuf;
  pa.btime = (ರಚನೆ timespec) {.tv_sec = -1, .tv_nsec = -1};

  ಆದರೆ (0 <= fd)
    {
      ಆದರೆ (fstat (fd, &statbuf) != 0)
        {
          error (0, errno, _("cannot stat standard input"));
          ಹಿಂತಿರುಗಿ false;
        }
    }
  /* We can't use the shorter
     (follow_links?stat:lstat) (filename, &statbug)
     since stat might be a function-like macro.  */
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ((follow_links
            ? stat (filename, &statbuf)
            : lstat (filename, &statbuf)) != 0)
    {
      error (0, errno, _("cannot stat %s"), quoteaf (filename));
      ಹಿಂತಿರುಗಿ false;
    }

  ಆದರೆ (S_ISBLK (statbuf.st_ಬಹುಲಕ) || S_ISCHR (statbuf.st_ಬಹುಲಕ))
    format = format2;

  bool fail = print_it (format, fd, filename, print_stat, &pa);
  ಹಿಂತಿರುಗಿ ! fail;
}
#ಆದರೆ_ಅಂತ್ಯ /* USE_STATX */

/* POSIX requires 'ls' to print file sizes without a sign, even
   when negative.  Be consistent with that.  */

ಸ್ಥಿರ uintmax_t
unsigned_file_ಗಾತ್ರ (off_t ಗಾತ್ರ_2)
{
  ಹಿಂತಿರುಗಿ ಗಾತ್ರ_2 + (ಗಾತ್ರ_2 < 0) * ((uintmax_t) OFF_T_MAX - OFF_T_MIN + 1);
}

/* Print stat info.  Return zero upon success, nonzero upon failure.  */
ಸ್ಥಿರ bool
print_stat (ಅಕ್ಷರ *pformat, size_t ಪೂರ್ವಪ್ರತ್ಯಯ_len, ಅಕ್ಷರ mod, ಅಕ್ಷರ m,
            MAYBE_ಬಳಸದ ಪೂರ್ಣಾಂಕ fd, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename, ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *ದತ್ತಾಂಶ)
{
  ರಚನೆ print_ವಾದಗಳು *parg = (ರಚನೆ print_ವಾದಗಳು *) ದತ್ತಾಂಶ;
  ರಚನೆ stat *statbuf = parg->st;
  ರಚನೆ timespec btime = parg->btime;
  ರಚನೆ passwd *pw_ent;
  ರಚನೆ ಗುಂಪು *gw_ent;
  bool fail = false;

  ಆಯ್ಕೆ (m)
    {
    ಸ್ಥಿತಿ 'n':
      ಆದರೆ (mod == 'Q')
        {
          initialize_quoting_style ();
          filename = quoteN (filename);
        }
      ಹೊರಹೋಗು_string (pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len, filename);
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'N':
      initialize_quoting_style ();
      ಹೊರಹೋಗು_string (pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len, quoteN (filename));
      ಆದರೆ (S_ISLNK (statbuf->st_ಬಹುಲಕ))
        {
          ಅಕ್ಷರ *linkname = areadlink_with_ಗಾತ್ರ (filename, statbuf->st_ಗಾತ್ರ);
          ಆದರೆ (linkname == ಖಾಲಿ)
            {
              error (0, errno, _("cannot read symbolic link %s"),
                     quoteaf (filename));
              ಹಿಂತಿರುಗಿ true;
            }
          ಬರೆ (" -> ");
          ಹೊರಹೋಗು_string (pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len, quoteN (linkname));
          ಬಿಡುಗಡೆ (linkname);
        }
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'd':
      ಆದರೆ (mod == 'H')
        ಹೊರಹೋಗು_uint (pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len, major (statbuf->st_dev));
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (mod == 'L')
        ಹೊರಹೋಗು_uint (pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len, minor (statbuf->st_dev));
      ಇಲ್ಲದಿದ್ದರೆ
        ಹೊರಹೋಗು_uint (pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len, statbuf->st_dev);
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'D':
      ಹೊರಹೋಗು_uint_x (pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len, statbuf->st_dev);
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'i':
      ಹೊರಹೋಗು_uint (pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len, statbuf->st_ino);
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'a':
      ಹೊರಹೋಗು_uint_o (pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len, statbuf->st_ಬಹುಲಕ & CHMOD_ಬಹುಲಕ_ಬಿಟ್ಟುಗಳು);
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'A':
      ಹೊರಹೋಗು_string (pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len, human_access (statbuf));
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'f':
      ಹೊರಹೋಗು_uint_x (pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len, statbuf->st_ಬಹುಲಕ);
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'F':
      ಹೊರಹೋಗು_string (pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len, file_type (statbuf));
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'h':
      ಹೊರಹೋಗು_uint (pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len, statbuf->st_nlink);
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'u':
      ಹೊರಹೋಗು_uint (pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len, statbuf->st_uid);
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'U':
      pw_ent = getpwuid (statbuf->st_uid);
      ಹೊರಹೋಗು_string (pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len,
                  pw_ent ? pw_ent->pw_name : "UNKNOWN");
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'g':
      ಹೊರಹೋಗು_uint (pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len, statbuf->st_gid);
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'G':
      gw_ent = getgrgid (statbuf->st_gid);
      ಹೊರಹೋಗು_string (pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len,
                  gw_ent ? gw_ent->gr_name : "UNKNOWN");
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'm':
      fail |= ಹೊರಹೋಗು_mount_ಬಿಂದು (filename, pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len, statbuf);
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 's':
      ಹೊರಹೋಗು_uint (pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len, unsigned_file_ಗಾತ್ರ (statbuf->st_ಗಾತ್ರ));
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'r':
      ಆದರೆ (mod == 'H')
        ಹೊರಹೋಗು_uint (pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len, major (statbuf->st_rdev));
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (mod == 'L')
        ಹೊರಹೋಗು_uint (pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len, minor (statbuf->st_rdev));
      ಇಲ್ಲದಿದ್ದರೆ
        ಹೊರಹೋಗು_uint (pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len, statbuf->st_rdev);
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'R':
      ಹೊರಹೋಗು_uint_x (pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len, statbuf->st_rdev);
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 't':
      ಹೊರಹೋಗು_uint_x (pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len, major (statbuf->st_rdev));
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'T':
      ಹೊರಹೋಗು_uint_x (pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len, minor (statbuf->st_rdev));
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'B':
      ಹೊರಹೋಗು_uint (pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len, ST_NBLOCKSIZE);
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'b':
      ಹೊರಹೋಗು_uint (pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len, STP_NBLOCKS (statbuf));
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'o':
      ಹೊರಹೋಗು_uint (pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len, STP_BLKSIZE (statbuf));
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'w':
      {
#ಆದರೆ ! USE_STATX
        btime = get_birthtime (fd, filename, statbuf);
#ಆದರೆ_ಅಂತ್ಯ
        ಆದರೆ (btime.tv_nsec < 0)
          ಹೊರಹೋಗು_string (pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len, "-");
        ಇಲ್ಲದಿದ್ದರೆ
          ಹೊರಹೋಗು_string (pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len, human_ಸಮಯ (btime));
      }
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'W':
      {
#ಆದರೆ ! USE_STATX
        btime = get_birthtime (fd, filename, statbuf);
#ಆದರೆ_ಅಂತ್ಯ
        ಹೊರಹೋಗು_epoch_sec (pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len, neg_ಗಮ್ಯ_zero (btime));
      }
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'x':
      ಹೊರಹೋಗು_string (pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len, human_ಸಮಯ (get_stat_atime (statbuf)));
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'X':
      ಹೊರಹೋಗು_epoch_sec (pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len, get_stat_atime (statbuf));
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'y':
      ಹೊರಹೋಗು_string (pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len, human_ಸಮಯ (get_stat_mtime (statbuf)));
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'Y':
      ಹೊರಹೋಗು_epoch_sec (pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len, get_stat_mtime (statbuf));
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'z':
      ಹೊರಹೋಗು_string (pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len, human_ಸಮಯ (get_stat_ctime (statbuf)));
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'Z':
      ಹೊರಹೋಗು_epoch_sec (pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len, get_stat_ctime (statbuf));
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'C':
      fail |= ಹೊರಹೋಗು_file_context (pformat, ಪೂರ್ವಪ್ರತ್ಯಯ_len, filename);
      ನಿಲ್ಲಿಸು;
    ಪೂರ್ವನಿಯೋಜಿತ:
      fputc ('?', stdout);
      ನಿಲ್ಲಿಸು;
    }
  ಹಿಂತಿರುಗಿ fail;
}

/* Return an allocated format string in static storage that
   corresponds to whether FS and TERSE options were declared.  */
ಸ್ಥಿರ ಅಕ್ಷರ *
default_format (bool fs, bool terse, bool device)
{
  ಅಕ್ಷರ *format;
  ಆದರೆ (fs)
    {
      ಆದರೆ (terse)
        format = xstrdup (fmt_terse_fs);
      ಇಲ್ಲದಿದ್ದರೆ
        {
          /* TRANSLATORS: This string uses format specifiers from
             'stat --help' with --file-system, and NOT from printf.  */
          format = xstrdup (_("  File: %Qn\n"
                              "    ID: %-8i Namelen: %-7l Type: %T\n"
                              "Block size: %-10s Fundamental block size: %S\n"
                              "Blocks: Total: %-10b Free: %-10f Available: %a\n"
                              "Inodes: Total: %-10c Free: %d\n"));
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ /* ! fs */
    {
      ಆದರೆ (terse)
        {
          ಆದರೆ (0 < is_selinux_enabled ())
            format = xstrdup (fmt_terse_selinux);
          ಇಲ್ಲದಿದ್ದರೆ
            format = xstrdup (fmt_terse_regular);
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಅಕ್ಷರ *ತಾತ್ಕಾಲಿಕ;
          /* TRANSLATORS: This string uses format specifiers from
             'stat --help' without --file-system, and NOT from printf.  */
          format = xstrdup (_("\
  File: %N\n\
  ಗಾತ್ರ_4: %-10s\t_ಖಂಡಗಳು: %-10b IO ಖಂಡ_3: %-6o %F\n\
"));

          ತಾತ್ಕಾಲಿಕ = format;
          ಆದರೆ (device)
            {
              /* TRANSLATORS: This string uses format specifiers from
                 'stat --help' without --file-system, and NOT from printf.  */
              format = xasprintf ("%s%s", format, _("\
" "Device: %Hd,%Ld\tInode: %-10i  Links: %-5h Device type: %Hr,%Lr\n\
"));
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              /* TRANSLATORS: This string uses format specifiers from
                 'stat --help' without --file-system, and NOT from printf.  */
              format = xasprintf ("%s%s", format, _("\
" "Device: %Hd,%Ld\tInode: %-10i  Links: %h\n\
"));
            }
          ಬಿಡುಗಡೆ (ತಾತ್ಕಾಲಿಕ);

          ತಾತ್ಕಾಲಿಕ = format;
          /* TRANSLATORS: This string uses format specifiers from
             'stat --help' without --file-system, and NOT from printf.  */
          format = xasprintf ("%s%s", format, _("\
" "Access: (%04a/%10.10A)  Uid: (%5u/%8U)   Gid: (%5g/%8G)\n\
"));
          ಬಿಡುಗಡೆ (ತಾತ್ಕಾಲಿಕ);

          ಆದರೆ (0 < is_selinux_enabled ())
            {
              ತಾತ್ಕಾಲಿಕ = format;
              /* TRANSLATORS: This string uses format specifiers from
                 'stat --help' without --file-system, and NOT from printf.  */
              format = xasprintf ("%s%s", format, _("Context: %C\n"));
              ಬಿಡುಗಡೆ (ತಾತ್ಕಾಲಿಕ);
            }

          ತಾತ್ಕಾಲಿಕ = format;
          format = xasprintf ("%s%s", format,
                              /* TRANSLATORS: This string uses format specifiers
                                 from 'stat --help' without --file-system, and
                                 NOT from printf.  */
                              _("Access: %x\n"
                                "Modify: %y\n"
                                "Change: %z\n"
                                " Birth: %w\n"));
          ಬಿಡುಗಡೆ (ತಾತ್ಕಾಲಿಕ);
        }
    }
  ಹಿಂತಿರುಗಿ format;
}

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("Usage: %s [OPTION]... FILE...\n"), ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
Display file or file system ಸ್ಥಿತಿ_2.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -L, --dereference\n\
         follow links\n\
"));
      oputs (_("\
  -f, --file-system\n\
         display file system ಸ್ಥಿತಿ_2 instead ದ file ಸ್ಥಿತಿ_2\n\
"));
      oputs (_("\
      --cached=ಬಹುಲಕ\n\
         specify how ಗಮ್ಯ use cached ಗುಣಲಕ್ಷಣಗಳು;\n\
         useful on remote file systems. See ಬಹುಲಕ ಕೆಳಗೆ\n\
"));
      oputs (_("\
  -c, --format=FORMAT\n\
         use the specified FORMAT instead ದ the ಪೂರ್ವನಿಯೋಜಿತ;\n\
         output a newline after each use ದ FORMAT\n\
"));
      oputs (_("\
      --ಬರೆ=FORMAT\n\
         like --format, but interpret backslash escapes,\n\
         and ಮಾಡು not output a mandatory trailing newline;\n\
         ಆದರೆ you want a newline, include \\n ಒಳ FORMAT\n\
"));
      oputs (_("\
  -t, --terse\n\
         print the information ಒಳ terse form\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);

      fputs (_("\n\
The ಬಹುಲಕ ವಾದ ದ --cached can be: always, never, or ಪೂರ್ವನಿಯೋಜಿತ.\n\
'always' will use cached ಗುಣಲಕ್ಷಣಗಳು ಆದರೆ ಲಭ್ಯ, ಆಗ\n\
'never' will try ಗಮ್ಯ synchronize with the latest ಗುಣಲಕ್ಷಣಗಳು, and\n\
'default' will leave it ಮೇಲೆ ಗಮ್ಯ the underlying file system.\n\
"), stdout);

      fputs (_("\n\
The valid format sequences ಗಾಗಿ files (without --file-system):\n\
\n\
  %a   permission ಬಿಟ್ಟುಗಳು_2 ಒಳ octal (see '#' and '0' ಬರೆ flags)\n\
  %A   permission ಬಿಟ್ಟುಗಳು_2 and file type ಒಳ human readable form\n\
  %b   ಸಂಖ್ಯೆ ದ ಖಂಡಗಳು allocated (see %B)\n\
  %B   the ಗಾತ್ರ_2 ಒಳ ಬೈಟುಗಳು ದ each ಖಂಡ reported by %b\n\
  %C   SELinux security context string\n\
"), stdout);
      fputs (_("\
  %d   device ಸಂಖ್ಯೆ ಒಳ decimal (st_dev)\n\
  %D   device ಸಂಖ್ಯೆ ಒಳ hex (st_dev)\n\
  %Hd  major device ಸಂಖ್ಯೆ ಒಳ decimal\n\
  %Ld  minor device ಸಂಖ್ಯೆ ಒಳ decimal\n\
  %f   raw ಬಹುಲಕ_2 ಒಳ hex\n\
  %F   file type\n\
  %g   ಗುಂಪು ID ದ owner\n\
  %G   ಗುಂಪು name ದ owner\n\
"), stdout);
      fputs (_("\
  %h   ಸಂಖ್ಯೆ ದ hard links\n\
  %i   inode ಸಂಖ್ಯೆ\n\
  %m   mount ಬಿಂದು\n\
  %n   file name\n\
  %Qn  quoted file name\n\
  %N   quoted file name with dereference ಆದರೆ symbolic link\n\
  %o   optimal I/O transfer ಗಾತ್ರ_2 hint\n\
  %s   ಒಟ್ಟು ಗಾತ್ರ_2, ಒಳ ಬೈಟುಗಳು\n\
  %r   device type ಒಳ decimal (st_rdev)\n\
  %R   device type ಒಳ hex (st_rdev)\n\
  %Hr  major device type ಒಳ decimal, ಗಾಗಿ character/ಖಂಡ device special files\n\
  %Lr  minor device type ಒಳ decimal, ಗಾಗಿ character/ಖಂಡ device special files\n\
  %t   major device type ಒಳ hex, ಗಾಗಿ character/ಖಂಡ device special files\n\
  %T   minor device type ಒಳ hex, ಗಾಗಿ character/ಖಂಡ device special files\n\
"), stdout);
      fputs (_("\
  %u   user ID ದ owner\n\
  %U   user name ದ owner\n\
  %w   ಸಮಯ ದ file birth, human-readable; - ಆದರೆ unknown\n\
  %W   ಸಮಯ ದ file birth, seconds since Epoch; 0 ಆದರೆ unknown\n\
  %x   ಸಮಯ ದ ಕೊನೆಯ access, human-readable\n\
  %X   ಸಮಯ ದ ಕೊನೆಯ access, seconds since Epoch\n\
  %y   ಸಮಯ ದ ಕೊನೆಯ ದತ್ತಾಂಶ modification, human-readable\n\
  %Y   ಸಮಯ ದ ಕೊನೆಯ ದತ್ತಾಂಶ modification, seconds since Epoch\n\
  %z   ಸಮಯ ದ ಕೊನೆಯ ಸ್ಥಿತಿ_2 change, human-readable\n\
  %Z   ಸಮಯ ದ ಕೊನೆಯ ಸ್ಥಿತಿ_2 change, seconds since Epoch\n\
\n\
"), stdout);

      fputs (_("\
Valid format sequences ಗಾಗಿ file systems:\n\
\n\
  %a   ಬಿಡುಗಡೆ ಖಂಡಗಳು ಲಭ್ಯ ಗಮ್ಯ non-superuser\n\
  %b   ಒಟ್ಟು ದತ್ತಾಂಶ ಖಂಡಗಳು ಒಳ file system\n\
  %c   ಒಟ್ಟು file nodes ಒಳ file system\n\
  %d   ಬಿಡುಗಡೆ file nodes ಒಳ file system\n\
  %f   ಬಿಡುಗಡೆ ಖಂಡಗಳು ಒಳ file system\n\
"), stdout);
      fputs (_("\
  %i   file system ID ಒಳ hex\n\
  %l   ಗರಿಷ್ಠ ಉದ್ದಳತೆ ದ filenames\n\
  %n   file name\n\
  %Qn  quoted file name\n\
  %s   ಖಂಡ ಗಾತ್ರ_2 (ಗಾಗಿ faster transfers)\n\
  %S   fundamental ಖಂಡ ಗಾತ್ರ_2 (ಗಾಗಿ ಖಂಡ counts)\n\
  %t   file system type ಒಳ hex\n\
  %T   file system type ಒಳ human readable form\n\
"), stdout);

      ಬರೆ (_("\n\
--terse is equivalent ಗಮ್ಯ the following FORMAT:\n\
    %s\
"),
#ಆದರೆ HAVE_SELINUX_SELINUX_H
              fmt_terse_selinux
#ಇಲ್ಲದಿದ್ದರೆ
              fmt_terse_regular
#ಆದರೆ_ಅಂತ್ಯ
              );

        ಬರೆ (_("\
--terse --file-system is equivalent ಗಮ್ಯ the following FORMAT:\n\
    %s\
"), fmt_terse_fs);

      ಬರೆ (USAGE_BUILTIN_WARNING, ಕಾರ್ಯಕ್ರಮ_NAME);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ *argv[])
{
  ಪೂರ್ಣಾಂಕ c;
  bool fs = false;
  bool terse = false;
  ಅಕ್ಷರ *format = ಖಾಲಿ;
  ಅಕ್ಷರ *format2;
  bool ok = true;

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  ರಚನೆ lconv ಸ್ಥಿರಾಂಕ *locale = localeconv ();
  decimal_ಬಿಂದು = (locale->decimal_ಬಿಂದು[0] ? locale->decimal_ಬಿಂದು : ".");
  decimal_ಬಿಂದು_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (decimal_ಬಿಂದು);

  atexit (close_stdout);

  ಆಗ ((c = getopt_long (argc, argv, "c:fLt", long_options, ಖಾಲಿ)) != -1)
    {
      ಆಯ್ಕೆ (c)
        {
        ಸ್ಥಿತಿ PRINTF_OPTION:
          format = optarg;
          interpret_backslash_escapes = true;
          trailing_delim = "";
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'c':
          format = optarg;
          interpret_backslash_escapes = false;
          trailing_delim = "\n";
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'L':
          follow_links = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'f':
          fs = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 't':
          terse = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 0:
          ಆಯ್ಕೆ (XARGMATCH ("--cached", optarg, cached_ವಾದಗಳು, cached_modes))
            {
              ಸ್ಥಿತಿ cached_never:
                force_sync = true;
                dont_sync = false;
                ನಿಲ್ಲಿಸು;
              ಸ್ಥಿತಿ cached_always:
                force_sync = false;
                dont_sync = true;
                ನಿಲ್ಲಿಸು;
              ಸ್ಥಿತಿ cached_default:
                force_sync = false;
                dont_sync = false;
            }
          ನಿಲ್ಲಿಸು;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);

        ಪೂರ್ವನಿಯೋಜಿತ:
          usage (EXIT_FAILURE);
        }
    }

  ಆದರೆ (argc == optind)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  ಆದರೆ (format)
    {
      format2 = format;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      format = default_format (fs, terse, /* device= */ false);
      format2 = default_format (fs, terse, /* device= */ true);
    }

  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = optind; i < argc; i++)
    ok &= (fs
           ? do_statfs (argv[i], format)
           : do_stat (argv[i], format, format2));

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
