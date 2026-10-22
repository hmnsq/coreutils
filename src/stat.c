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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>

/* Keep this conditional in sync with the similar conditional in
   ../m4/stat-prog.m4.  */
#ਜੇ ((STAT_STATVFS || STAT_STATVFS64)                                       \
     && (HAVE_STRUCT_STATVFS_F_BASETYPE || HAVE_STRUCT_STATVFS_F_FSTYPENAME \
         || (! HAVE_STRUCT_STATFS_F_FSTYPENAME && HAVE_STRUCT_STATVFS_F_TYPE)))
# define USE_STATVFS 1
#ਨਹੀਂਤਾਂ
# define USE_STATVFS 0
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <pwd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <grp.h>
#ਜੇ USE_STATVFS
# include <sys/statvfs.h>
#ਜਾਂਜੇ HAVE_SYS_VFS_H
# include <sys/vfs.h>
#ਜਾਂਜੇ HAVE_SYS_MOUNT_H && HAVE_SYS_PARAM_H
/* NOTE: freebsd5.0 needs sys/param.h and sys/mount.h for statfs.
   It does have statvfs.h, but shouldn't use it, since it doesn't
   HAVE_STRUCT_STATVFS_F_BASETYPE.  So find a clean way to fix it.  */
/* NetBSD 1.5.2 needs these, for the declaration of struct statfs. */
# include <sys/param.h>
# include <sys/mount.h>
# ਜੇ HAVE_NFS_NFS_CLNT_H && HAVE_NFS_VFS_H
/* Ultrix 4.4 needs these for the declaration of struct statfs.  */
#  include <netinet/in.h>
#  include <nfs/nfs_clnt.h>
#  include <nfs/vfs.h>
# endif
#ਜਾਂਜੇ HAVE_OS_H /* BeOS */
# include <fs_info.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <selinux/selinux.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "areadlink.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "c-ctype.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "file-type.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "filemode.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "mountlist.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "octhexdigits.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "stat-size.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "stat-time.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strftime.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "find-mount-point.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xvasprintf.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "statx.h"

#ਜੇ HAVE_STATX && defined STATX_INO
# define USE_STATX 1
#ਨਹੀਂਤਾਂ
# define USE_STATX 0
#ਜੇ_ਅੰਤ

#ਜੇ USE_STATVFS
# define STRUCT_STATXFS_F_FSID_IS_INTEGER STRUCT_STATVFS_F_FSID_IS_INTEGER
# define HAVE_STRUCT_STATXFS_F_TYPE HAVE_STRUCT_STATVFS_F_TYPE
# ਜੇ HAVE_STRUCT_STATVFS_F_NAMEMAX
#  define SB_F_NAMEMAX(S) ((S)->f_namemax)
# endif
# ਜੇ ! STAT_STATVFS && STAT_STATVFS64
#  define STRUCT_STATVFS ਬਣਤਰ statvfs64
#  define STATFS statvfs64
# ਨਹੀਂਤਾਂ
#  define STRUCT_STATVFS ਬਣਤਰ statvfs
#  define STATFS statvfs
# endif
# define STATFS_FRSIZE(S) ((S)->f_frsize)
#ਨਹੀਂਤਾਂ
# define HAVE_STRUCT_STATXFS_F_TYPE HAVE_STRUCT_STATFS_F_TYPE
# ਜੇ HAVE_STRUCT_STATFS_F_NAMELEN
#  define SB_F_NAMEMAX(S) ((S)->f_namelen)
# elif HAVE_STRUCT_STATFS_F_NAMEMAX
#  define SB_F_NAMEMAX(S) ((S)->f_namemax)
# endif
# define STATFS statfs
# ਜੇ HAVE_OS_H /* BeOS */
/* BeOS has a statvfs function, but it does not return sensible values
   for f_files, f_ffree and f_favail, and lacks f_type, f_basetype and
   f_fstypename.  Use 'struct fs_info' instead.  */
NODISCARD
ਸਥਿਰ ਪੂਰਨ
statfs (ਅੱਖਰ ਅਚਲ *filename, ਬਣਤਰ fs_info *buf)
{
  dev_t device = dev_for_ਰਸਤਾ (filename);
  ਜੇ (device < 0)
    {
      errno = (device == B_ਇੰਦਰਾਜ_NOT_ਮਿਲਿਆ ? ENOENT
               : device == B_BAD_ਮੁੱਲ ? EINVAL
               : device == B_NAME_TOO_LONG ? ENAMETOOLONG
               : device == B_NO_ਸਮ੍ਰਿਤੀ ? ENOMEM
               : device == B_FILE_ERROR ? EIO
               : 0);
      ਵਾਪਸ -1;
    }
  /* If successful, buf->dev will be == device.  */
  ਵਾਪਸ fs_stat_dev (device, buf);
}
#  define f_fsid dev
#  define f_ਖੰਡਾਂ ਕੁੱਲ_ਖੰਡਾਂ
#  define f_bfree free_ਖੰਡਾਂ
#  define f_bavail free_ਖੰਡਾਂ
#  define f_bsize io_ਮਾਪ
#  define f_files ਕੁੱਲ_nodes
#  define f_ffree free_nodes
#  define STRUCT_STATVFS ਬਣਤਰ fs_info
#  define STRUCT_STATXFS_F_FSID_IS_INTEGER true
#  define STATFS_FRSIZE(S) ((S)->ਖੰਡ_ਮਾਪ)
# ਨਹੀਂਤਾਂ
#  define STRUCT_STATVFS ਬਣਤਰ statfs
#  define STRUCT_STATXFS_F_FSID_IS_INTEGER STRUCT_STATFS_F_FSID_IS_INTEGER
#  ਜੇ HAVE_STRUCT_STATFS_F_FRSIZE
#   define STATFS_FRSIZE(S) ((S)->f_frsize)
#  ਨਹੀਂਤਾਂ
#   define STATFS_FRSIZE(S) 0
#  endif
# endif
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ SB_F_NAMEMAX
# define ਬਾਹਰਲਾ_NAMEMAX ਬਾਹਰਲਾ_uint
#ਨਹੀਂਤਾਂ
/* Depending on whether statvfs or statfs is used,
   neither f_namemax or f_namelen may be available.  */
# define SB_F_NAMEMAX(S) "?"
# define ਬਾਹਰਲਾ_NAMEMAX ਬਾਹਰਲਾ_string
#ਜੇ_ਅੰਤ

#ਜੇ HAVE_STRUCT_STATVFS_F_BASETYPE
# define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME f_basetype
#ਨਹੀਂਤਾਂ
# ਜੇ HAVE_STRUCT_STATVFS_F_FSTYPENAME || HAVE_STRUCT_STATFS_F_FSTYPENAME
#  define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME f_fstypename
# elif HAVE_OS_H /* BeOS */
#  define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME fsh_name
# endif
#ਜੇ_ਅੰਤ

#ਜੇ HAVE_GETATTRAT
# include <attr.h>
# include <sys/nvpair.h>
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਅੱਖਰ ਅਚਲ digits[] = "0123456789";

/* Flags that are portable for use in printf, for at least one
   conversion specifier; make_format removes non-portable flags as
   needed for particular specifiers.  The glibc 2.2 extension "I" is
   listed here; it is removed by make_format because it has undefined
   behavior elsewhere and because it is incompatible with
   out_epoch_sec.  */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ printf_flags[] = "'-+ #0I";

/* Formats for the --terse option.  */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ fmt_terse_fs[] = "%Qn %i %l %t %s %S %b %f %a %c %d\n";
ਸਥਿਰ ਅੱਖਰ ਅਚਲ fmt_terse_regular[] = "%Qn %s %b %f %u %g %D %i %h %t %T"
                                        " %X %Y %Z %W %o\n";
ਸਥਿਰ ਅੱਖਰ ਅਚਲ fmt_terse_selinux[] = "%Qn %s %b %f %u %g %D %i %h %t %T"
                                        " %X %Y %Z %W %o %C\n";

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "stat"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS proper_name ("Michael Meskes")

ਗਿਣਤੀ
{
  PRINTF_OPTION = CHAR_MAX + 1
};

ਗਿਣਤੀ cached_ਬਹੁਲਕ
{
  cached_default,
  cached_never,
  cached_always
};

ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ਅਚਲ cached_ਦਲੀਲਾਂ[] =
{
  "default", "never", "always", ਨਿਰਾ
};

ਸਥਿਰ ਗਿਣਤੀ cached_ਬਹੁਲਕ ਅਚਲ cached_modes[] =
{
  cached_default, cached_never, cached_always
};

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ long_options[] =
{
  {"dereference", no_ਦਲੀਲ, ਨਿਰਾ, 'L'},
  {"file-system", no_ਦਲੀਲ, ਨਿਰਾ, 'f'},
  {"format", required_ਦਲੀਲ, ਨਿਰਾ, 'c'},
  {"printf", required_ਦਲੀਲ, ਨਿਰਾ, PRINTF_OPTION},
  {"terse", no_ਦਲੀਲ, ਨਿਰਾ, 't'},
  {"cached", required_ਦਲੀਲ, ਨਿਰਾ, 0},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};

/* Whether to follow symbolic links;  True for --dereference (-L).  */
ਸਥਿਰ bool follow_links;

/* Whether to interpret backslash-escape sequences.
   True for --printf=FMT, not for --format=FMT (-c).  */
ਸਥਿਰ bool interpret_backslash_escapes;

/* The trailing delimiter string:
   "" for --printf=FMT, "\n" for --format=FMT (-c).  */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *trailing_delim = "";

/* The representation of the decimal point in the current locale.  */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *decimal_ਬਿੰਦੂ;
ਸਥਿਰ size_t decimal_ਬਿੰਦੂ_len;

ਸਥਿਰ bool
print_stat (ਅੱਖਰ *pformat, size_t ਅਗੇਤਰ_len, ਅੱਖਰ mod, ਅੱਖਰ m,
            ਪੂਰਨ fd, ਅੱਖਰ ਅਚਲ *filename, ਖਾਲੀ ਅਚਲ *ਅੰਕੜੇ);

/* Return the type of the specified file system.
   Some systems have statfvs.f_basetype[FSTYPSZ] (AIX, HP-UX, and Solaris).
   Others have statvfs.f_fstypename[_VFS_NAMELEN] (NetBSD 3.0).
   Others have statfs.f_fstypename[MFSNAMELEN] (NetBSD 1.5.2).
   Still others have neither and have to get by with f_type (GNU/Linux).
   But f_type may only exist in statfs (Cygwin).  */
NODISCARD
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *
human_fstype (STRUCT_STATVFS ਅਚਲ *statfsbuf)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME
  ਵਾਪਸ statfsbuf->STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME;
#ਨਹੀਂਤਾਂ
  ਚੋਣ (statfsbuf->f_type)
    {
# ਜੇ defined __linux__ || defined __ANDROID__

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

    ਹਾਲਤ S_MAGIC_AAFS: /* 0x5A3C69F0 local */
      ਵਾਪਸ "aafs";
    ਹਾਲਤ S_MAGIC_ACFS: /* 0x61636673 remote */
      ਵਾਪਸ "acfs";
    ਹਾਲਤ S_MAGIC_ADFS: /* 0xADF5 local */
      ਵਾਪਸ "adfs";
    ਹਾਲਤ S_MAGIC_AFFS: /* 0xADFF local */
      ਵਾਪਸ "affs";
    ਹਾਲਤ S_MAGIC_AFS: /* 0x5346414F remote */
      ਵਾਪਸ "afs";
    ਹਾਲਤ S_MAGIC_ANON_INODE_FS: /* 0x09041934 local */
      ਵਾਪਸ "anon-inode FS";
    ਹਾਲਤ S_MAGIC_AUFS: /* 0x61756673 remote */
      /* FIXME: change syntax or add an optional attribute like "inotify:no".
         The above is labeled as "remote" so that tail always uses polling,
         but this isn't really a remote file system type.  */
      ਵਾਪਸ "aufs";
    ਹਾਲਤ S_MAGIC_AUTOFS: /* 0x0187 local */
      ਵਾਪਸ "autofs";
    ਹਾਲਤ S_MAGIC_BALLOON_KVM: /* 0x13661366 local */
      ਵਾਪਸ "balloon-kvm-fs";
    ਹਾਲਤ S_MAGIC_BCACHEFS: /* 0xCA451A4E local */
      ਵਾਪਸ "bcachefs";
    ਹਾਲਤ S_MAGIC_BEFS: /* 0x42465331 local */
      ਵਾਪਸ "befs";
    ਹਾਲਤ S_MAGIC_BDEVFS: /* 0x62646576 local */
      ਵਾਪਸ "bdevfs";
    ਹਾਲਤ S_MAGIC_BFS: /* 0x1BADFACE local */
      ਵਾਪਸ "bfs";
    ਹਾਲਤ S_MAGIC_BINDERFS: /* 0x6C6F6F70 local */
      ਵਾਪਸ "binderfs";
    ਹਾਲਤ S_MAGIC_BPF_FS: /* 0xCAFE4A11 local */
      ਵਾਪਸ "bpf_fs";
    ਹਾਲਤ S_MAGIC_BINFMTFS: /* 0x42494E4D local */
      ਵਾਪਸ "binfmt_misc";
    ਹਾਲਤ S_MAGIC_BTRFS: /* 0x9123683E local */
      ਵਾਪਸ "btrfs";
    ਹਾਲਤ S_MAGIC_BTRFS_TEST: /* 0x73727279 local */
      ਵਾਪਸ "btrfs_test";
    ਹਾਲਤ S_MAGIC_CEPH: /* 0x00C36400 remote */
      ਵਾਪਸ "ceph";
    ਹਾਲਤ S_MAGIC_CGROUP: /* 0x0027E0EB local */
      ਵਾਪਸ "cgroupfs";
    ਹਾਲਤ S_MAGIC_CGROUP2: /* 0x63677270 local */
      ਵਾਪਸ "cgroup2fs";
    ਹਾਲਤ S_MAGIC_CIFS: /* 0xFF534D42 remote */
      ਵਾਪਸ "cifs";
    ਹਾਲਤ S_MAGIC_CODA: /* 0x73757245 remote */
      ਵਾਪਸ "coda";
    ਹਾਲਤ S_MAGIC_COH: /* 0x012FF7B7 local */
      ਵਾਪਸ "coh";
    ਹਾਲਤ S_MAGIC_CONFIGFS: /* 0x62656570 local */
      ਵਾਪਸ "configfs";
    ਹਾਲਤ S_MAGIC_CRAMFS: /* 0x28CD3D45 local */
      ਵਾਪਸ "cramfs";
    ਹਾਲਤ S_MAGIC_CRAMFS_WEND: /* 0x453DCD28 local */
      ਵਾਪਸ "cramfs-wend";
    ਹਾਲਤ S_MAGIC_DAXFS: /* 0x64646178 local */
      ਵਾਪਸ "daxfs";
    ਹਾਲਤ S_MAGIC_DEBUGFS: /* 0x64626720 local */
      ਵਾਪਸ "debugfs";
    ਹਾਲਤ S_MAGIC_DEVFS: /* 0x1373 local */
      ਵਾਪਸ "devfs";
    ਹਾਲਤ S_MAGIC_DEVMEM: /* 0x454D444D local */
      ਵਾਪਸ "devmem";
    ਹਾਲਤ S_MAGIC_DEVPTS: /* 0x1CD1 local */
      ਵਾਪਸ "devpts";
    ਹਾਲਤ S_MAGIC_DMA_BUF: /* 0x444D4142 local */
      ਵਾਪਸ "dma-buf-fs";
    ਹਾਲਤ S_MAGIC_ECRYPTFS: /* 0xF15F local */
      ਵਾਪਸ "ecryptfs";
    ਹਾਲਤ S_MAGIC_EFIVARFS: /* 0xDE5E81E4 local */
      ਵਾਪਸ "efivarfs";
    ਹਾਲਤ S_MAGIC_EFS: /* 0x00414A53 local */
      ਵਾਪਸ "efs";
    ਹਾਲਤ S_MAGIC_EROFS_V1: /* 0xE0F5E1E2 local */
      ਵਾਪਸ "erofs";
    ਹਾਲਤ S_MAGIC_EXFAT: /* 0x2011BAB0 local */
      ਵਾਪਸ "exfat";
    ਹਾਲਤ S_MAGIC_EXFS: /* 0x45584653 local */
      ਵਾਪਸ "exfs";
    ਹਾਲਤ S_MAGIC_EXOFS: /* 0x5DF5 local */
      ਵਾਪਸ "exofs";
    ਹਾਲਤ S_MAGIC_EXT: /* 0x137D local */
      ਵਾਪਸ "ext";
    ਹਾਲਤ S_MAGIC_EXT2: /* 0xEF53 local */
      ਵਾਪਸ "ext2/ext3";
    ਹਾਲਤ S_MAGIC_EXT2_OLD: /* 0xEF51 local */
      ਵਾਪਸ "ext2";
    ਹਾਲਤ S_MAGIC_F2FS: /* 0xF2F52010 local */
      ਵਾਪਸ "f2fs";
    ਹਾਲਤ S_MAGIC_FAIL_FS: /* 0x4641494C local */
      ਵਾਪਸ "failfs";
    ਹਾਲਤ S_MAGIC_FAT: /* 0x4006 local */
      ਵਾਪਸ "fat";
    ਹਾਲਤ S_MAGIC_FHGFS: /* 0x19830326 remote */
      ਵਾਪਸ "fhgfs";
    ਹਾਲਤ S_MAGIC_FUSE: /* 0x65735546 remote */
      ਵਾਪਸ "fuse";
    ਹਾਲਤ S_MAGIC_FUSECTL: /* 0x65735543 remote */
      ਵਾਪਸ "fusectl";
    ਹਾਲਤ S_MAGIC_FUTEXFS: /* 0x0BAD1DEA local */
      ਵਾਪਸ "futexfs";
    ਹਾਲਤ S_MAGIC_GFS: /* 0x01161970 remote */
      ਵਾਪਸ "gfs/gfs2";
    ਹਾਲਤ S_MAGIC_GPFS: /* 0x47504653 remote */
      ਵਾਪਸ "gpfs";
    ਹਾਲਤ S_MAGIC_GUEST_MEMFD: /* 0x474D454D remote */
      ਵਾਪਸ "guest-memfd";
    ਹਾਲਤ S_MAGIC_HFS: /* 0x4244 local */
      ਵਾਪਸ "hfs";
    ਹਾਲਤ S_MAGIC_HFS_PLUS: /* 0x482B local */
      ਵਾਪਸ "hfs+";
    ਹਾਲਤ S_MAGIC_HFS_X: /* 0x4858 local */
      ਵਾਪਸ "hfsx";
    ਹਾਲਤ S_MAGIC_HOSTFS: /* 0x00C0FFEE local */
      ਵਾਪਸ "hostfs";
    ਹਾਲਤ S_MAGIC_HPFS: /* 0xF995E849 local */
      ਵਾਪਸ "hpfs";
    ਹਾਲਤ S_MAGIC_HUGETLBFS: /* 0x958458F6 local */
      ਵਾਪਸ "hugetlbfs";
    ਹਾਲਤ S_MAGIC_MTD_INODE_FS: /* 0x11307854 local */
      ਵਾਪਸ "inodefs";
    ਹਾਲਤ S_MAGIC_IBRIX: /* 0x013111A8 remote */
      ਵਾਪਸ "ibrix";
    ਹਾਲਤ S_MAGIC_INOTIFYFS: /* 0x2BAD1DEA local */
      ਵਾਪਸ "inotifyfs";
    ਹਾਲਤ S_MAGIC_ISOFS: /* 0x9660 local */
      ਵਾਪਸ "isofs";
    ਹਾਲਤ S_MAGIC_ISOFS_R_WIN: /* 0x4004 local */
      ਵਾਪਸ "isofs";
    ਹਾਲਤ S_MAGIC_ISOFS_WIN: /* 0x4000 local */
      ਵਾਪਸ "isofs";
    ਹਾਲਤ S_MAGIC_JFFS: /* 0x07C0 local */
      ਵਾਪਸ "jffs";
    ਹਾਲਤ S_MAGIC_JFFS2: /* 0x72B6 local */
      ਵਾਪਸ "jffs2";
    ਹਾਲਤ S_MAGIC_JFS: /* 0x3153464A local */
      ਵਾਪਸ "jfs";
    ਹਾਲਤ S_MAGIC_KAFS: /* 0x6B414653 remote */
      ਵਾਪਸ "k-afs";
    ਹਾਲਤ S_MAGIC_LOGFS: /* 0xC97E8168 local */
      ਵਾਪਸ "logfs";
    ਹਾਲਤ S_MAGIC_LUSTRE: /* 0x0BD00BD0 remote */
      ਵਾਪਸ "lustre";
    ਹਾਲਤ S_MAGIC_M1FS: /* 0x5346314D local */
      ਵਾਪਸ "m1fs";
    ਹਾਲਤ S_MAGIC_MINIX: /* 0x137F local */
      ਵਾਪਸ "minix";
    ਹਾਲਤ S_MAGIC_MINIX_30: /* 0x138F local */
      ਵਾਪਸ "minix (30 char.)";
    ਹਾਲਤ S_MAGIC_MINIX_V2: /* 0x2468 local */
      ਵਾਪਸ "minix v2";
    ਹਾਲਤ S_MAGIC_MINIX_V2_30: /* 0x2478 local */
      ਵਾਪਸ "minix v2 (30 char.)";
    ਹਾਲਤ S_MAGIC_MINIX_V3: /* 0x4D5A local */
      ਵਾਪਸ "minix3";
    ਹਾਲਤ S_MAGIC_MQUEUE: /* 0x19800202 local */
      ਵਾਪਸ "mqueue";
    ਹਾਲਤ S_MAGIC_MSDOS: /* 0x4D44 local */
      ਵਾਪਸ "msdos";
    ਹਾਲਤ S_MAGIC_NCP: /* 0x564C remote */
      ਵਾਪਸ "novell";
    ਹਾਲਤ S_MAGIC_NFS: /* 0x6969 remote */
      ਵਾਪਸ "nfs";
    ਹਾਲਤ S_MAGIC_NFSD: /* 0x6E667364 remote */
      ਵਾਪਸ "nfsd";
    ਹਾਲਤ S_MAGIC_NILFS: /* 0x3434 local */
      ਵਾਪਸ "nilfs";
    ਹਾਲਤ S_MAGIC_NSFS: /* 0x6E736673 local */
      ਵਾਪਸ "nsfs";
    ਹਾਲਤ S_MAGIC_NTFS: /* 0x5346544E local */
      ਵਾਪਸ "ntfs";
    ਹਾਲਤ S_MAGIC_NULL_FS: /* 0x4E554C4C local */
      ਵਾਪਸ "nullfs";
    ਹਾਲਤ S_MAGIC_OPENPROM: /* 0x9FA1 local */
      ਵਾਪਸ "openprom";
    ਹਾਲਤ S_MAGIC_OCFS2: /* 0x7461636F remote */
      ਵਾਪਸ "ocfs2";
    ਹਾਲਤ S_MAGIC_OVERLAYFS: /* 0x794C7630 remote */
      /* This may overlay remote file systems.
         Also there have been issues reported with inotify and overlayfs,
         so mark as "remote" so that polling is used.  */
      ਵਾਪਸ "overlayfs";
    ਹਾਲਤ S_MAGIC_PANFS: /* 0xAAD7AAEA remote */
      ਵਾਪਸ "panfs";
    ਹਾਲਤ S_MAGIC_PID_FS: /* 0x50494446 local */
      ਵਾਪਸ "pidfs";
    ਹਾਲਤ S_MAGIC_PIPEFS: /* 0x50495045 remote */
      /* FIXME: change syntax or add an optional attribute like "inotify:no".
         pipefs is labeled as "remote" so that tail always polls,
         but it isn't really remote file system type.  */
      ਵਾਪਸ "pipefs";
    ਹਾਲਤ S_MAGIC_PPC_CMM: /* 0xC7571590 local */
      ਵਾਪਸ "ppc-cmm-fs";
    ਹਾਲਤ S_MAGIC_PRL_FS: /* 0x7C7C6673 remote */
      ਵਾਪਸ "prl_fs";
    ਹਾਲਤ S_MAGIC_PROC: /* 0x9FA0 local */
      ਵਾਪਸ "proc";
    ਹਾਲਤ S_MAGIC_PSTOREFS: /* 0x6165676C local */
      ਵਾਪਸ "pstorefs";
    ਹਾਲਤ S_MAGIC_QNX4: /* 0x002F local */
      ਵਾਪਸ "qnx4";
    ਹਾਲਤ S_MAGIC_QNX6: /* 0x68191122 local */
      ਵਾਪਸ "qnx6";
    ਹਾਲਤ S_MAGIC_RAMFS: /* 0x858458F6 local */
      ਵਾਪਸ "ramfs";
    ਹਾਲਤ S_MAGIC_RDTGROUP: /* 0x07655821 local */
      ਵਾਪਸ "rdt";
    ਹਾਲਤ S_MAGIC_REISERFS: /* 0x52654973 local */
      ਵਾਪਸ "reiserfs";
    ਹਾਲਤ S_MAGIC_ROMFS: /* 0x7275 local */
      ਵਾਪਸ "romfs";
    ਹਾਲਤ S_MAGIC_RPC_PIPEFS: /* 0x67596969 local */
      ਵਾਪਸ "rpc_pipefs";
    ਹਾਲਤ S_MAGIC_SDCARDFS: /* 0x5DCA2DF5 local */
      ਵਾਪਸ "sdcardfs";
    ਹਾਲਤ S_MAGIC_SECRETMEM: /* 0x5345434D local */
      ਵਾਪਸ "secretmem";
    ਹਾਲਤ S_MAGIC_SECURITYFS: /* 0x73636673 local */
      ਵਾਪਸ "securityfs";
    ਹਾਲਤ S_MAGIC_SELINUX: /* 0xF97CFF8C local */
      ਵਾਪਸ "selinux";
    ਹਾਲਤ S_MAGIC_SMACK: /* 0x43415D53 local */
      ਵਾਪਸ "smackfs";
    ਹਾਲਤ S_MAGIC_SMB: /* 0x517B remote */
      ਵਾਪਸ "smb";
    ਹਾਲਤ S_MAGIC_SMB2: /* 0xFE534D42 remote */
      ਵਾਪਸ "smb2";
    ਹਾਲਤ S_MAGIC_SNFS: /* 0xBEEFDEAD remote */
      ਵਾਪਸ "snfs";
    ਹਾਲਤ S_MAGIC_SOCKFS: /* 0x534F434B local */
      ਵਾਪਸ "sockfs";
    ਹਾਲਤ S_MAGIC_SQUASHFS: /* 0x73717368 local */
      ਵਾਪਸ "squashfs";
    ਹਾਲਤ S_MAGIC_SYSFS: /* 0x62656572 local */
      ਵਾਪਸ "sysfs";
    ਹਾਲਤ S_MAGIC_SYSV2: /* 0x012FF7B6 local */
      ਵਾਪਸ "sysv2";
    ਹਾਲਤ S_MAGIC_SYSV4: /* 0x012FF7B5 local */
      ਵਾਪਸ "sysv4";
    ਹਾਲਤ S_MAGIC_TMPFS: /* 0x01021994 local */
      ਵਾਪਸ "tmpfs";
    ਹਾਲਤ S_MAGIC_TRACEFS: /* 0x74726163 local */
      ਵਾਪਸ "tracefs";
    ਹਾਲਤ S_MAGIC_UBIFS: /* 0x24051905 local */
      ਵਾਪਸ "ubifs";
    ਹਾਲਤ S_MAGIC_UDF: /* 0x15013346 local */
      ਵਾਪਸ "udf";
    ਹਾਲਤ S_MAGIC_UFS: /* 0x00011954 local */
      ਵਾਪਸ "ufs";
    ਹਾਲਤ S_MAGIC_UFS_BYTESWAPPED: /* 0x54190100 local */
      ਵਾਪਸ "ufs";
    ਹਾਲਤ S_MAGIC_USBDEVFS: /* 0x9FA2 local */
      ਵਾਪਸ "usbdevfs";
    ਹਾਲਤ S_MAGIC_V9FS: /* 0x01021997 local */
      ਵਾਪਸ "v9fs";
    ਹਾਲਤ S_MAGIC_VBOXSF: /* 0x786F4256 remote */
      ਵਾਪਸ "vboxsf";
    ਹਾਲਤ S_MAGIC_VMHGFS: /* 0xBACBACBC remote */
      ਵਾਪਸ "vmhgfs";
    ਹਾਲਤ S_MAGIC_VXFS: /* 0xA501FCF5 remote */
      /* Veritas File System can run in single instance or clustered mode,
         so mark as remote to cater for the latter case.  */
      ਵਾਪਸ "vxfs";
    ਹਾਲਤ S_MAGIC_VZFS: /* 0x565A4653 local */
      ਵਾਪਸ "vzfs";
    ਹਾਲਤ S_MAGIC_WSLFS: /* 0x53464846 local */
      ਵਾਪਸ "wslfs";
    ਹਾਲਤ S_MAGIC_XENFS: /* 0xABBA1974 local */
      ਵਾਪਸ "xenfs";
    ਹਾਲਤ S_MAGIC_XENIX: /* 0x012FF7B4 local */
      ਵਾਪਸ "xenix";
    ਹਾਲਤ S_MAGIC_XFS: /* 0x58465342 local */
      ਵਾਪਸ "xfs";
    ਹਾਲਤ S_MAGIC_XIAFS: /* 0x012FD16D local */
      ਵਾਪਸ "xia";
    ਹਾਲਤ S_MAGIC_Z3FOLD: /* 0x0033 local */
      ਵਾਪਸ "z3fold";
    ਹਾਲਤ S_MAGIC_ZFS: /* 0x2FC12FC1 local */
      ਵਾਪਸ "zfs";
    ਹਾਲਤ S_MAGIC_ZONEFS: /* 0x5A4F4653 local */
      ਵਾਪਸ "zonefs";
    ਹਾਲਤ S_MAGIC_ZSMALLOC: /* 0x58295829 local */
      ਵਾਪਸ "zsmallocfs";


# elif __GNU__
    ਹਾਲਤ FSTYPE_UFS:
      ਵਾਪਸ "ufs";
    ਹਾਲਤ FSTYPE_NFS:
      ਵਾਪਸ "nfs";
    ਹਾਲਤ FSTYPE_GFS:
      ਵਾਪਸ "gfs";
    ਹਾਲਤ FSTYPE_LFS:
      ਵਾਪਸ "lfs";
    ਹਾਲਤ FSTYPE_SYSV:
      ਵਾਪਸ "sysv";
    ਹਾਲਤ FSTYPE_FTP:
      ਵਾਪਸ "ftp";
    ਹਾਲਤ FSTYPE_TAR:
      ਵਾਪਸ "tar";
    ਹਾਲਤ FSTYPE_AR:
      ਵਾਪਸ "ar";
    ਹਾਲਤ FSTYPE_CPIO:
      ਵਾਪਸ "cpio";
    ਹਾਲਤ FSTYPE_MSLOSS:
      ਵਾਪਸ "msloss";
    ਹਾਲਤ FSTYPE_CPM:
      ਵਾਪਸ "cpm";
    ਹਾਲਤ FSTYPE_HFS:
      ਵਾਪਸ "hfs";
    ਹਾਲਤ FSTYPE_DTFS:
      ਵਾਪਸ "dtfs";
    ਹਾਲਤ FSTYPE_GRFS:
      ਵਾਪਸ "grfs";
    ਹਾਲਤ FSTYPE_ਪਦ:
      ਵਾਪਸ "term";
    ਹਾਲਤ FSTYPE_DEV:
      ਵਾਪਸ "dev";
    ਹਾਲਤ FSTYPE_PROC:
      ਵਾਪਸ "proc";
    ਹਾਲਤ FSTYPE_IFSOCK:
      ਵਾਪਸ "ifsock";
    ਹਾਲਤ FSTYPE_AFS:
      ਵਾਪਸ "afs";
    ਹਾਲਤ FSTYPE_DFS:
      ਵਾਪਸ "dfs";
    ਹਾਲਤ FSTYPE_PROC9:
      ਵਾਪਸ "proc9";
    ਹਾਲਤ FSTYPE_SOCKET:
      ਵਾਪਸ "socket";
    ਹਾਲਤ FSTYPE_MISC:
      ਵਾਪਸ "misc";
    ਹਾਲਤ FSTYPE_EXT2FS:
      ਵਾਪਸ "ext2/ext3";
    ਹਾਲਤ FSTYPE_HTTP:
      ਵਾਪਸ "http";
    ਹਾਲਤ FSTYPE_MEMFS:
      ਵਾਪਸ "memfs";
    ਹਾਲਤ FSTYPE_ISO9660:
      ਵਾਪਸ "iso9660";
# endif
    ਮੂਲ:
      {
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਪੂਰਨ type = statfsbuf->f_type;
        ਸਥਿਰ ਅੱਖਰ buf[ਆਕਾਰ "UNKNOWN (0x%lx)" - 3
                        + (ਆਕਾਰ type * CHAR_BIT + 3) / 4];
        sprintf (buf, "UNKNOWN (0x%lx)", type);
        ਵਾਪਸ buf;
      }
    }
#ਜੇ_ਅੰਤ
}

NODISCARD
ਸਥਿਰ ਅੱਖਰ *
human_access (ਬਣਤਰ stat ਅਚਲ *statbuf)
{
  ਸਥਿਰ ਅੱਖਰ modebuf[12];
  filemodestring (statbuf, modebuf);
  modebuf[10] = 0;
  ਵਾਪਸ modebuf;
}

NODISCARD
ਸਥਿਰ ਅੱਖਰ *
human_ਸਮਾਂ (ਬਣਤਰ timespec t)
{
  /* STR must be at least INT_BUFSIZE_BOUND (intmax_t) big, either
     because localtime_rz fails, or because the time zone is truly
     outlandish so that %z expands to a long string.  */
  ਸਥਿਰ ਅੱਖਰ str[INT_BUFSIZE_BOUND (intmax_t)
                  + INT_STRLEN_BOUND (ਪੂਰਨ) /* YYYY */
                  + 1 /* because YYYY might equal INT_MAX + 1900 */
                  + ਆਕਾਰ "-MM-DD HH:MM:SS.NNNNNNNNN +"];
  ਸਥਿਰ timezone_t tz;
  ਜੇ (!tz)
    tz = tzalloc (getenv ("TZ"));
  ਬਣਤਰ tm tm;
  ਪੂਰਨ ns = t.tv_nsec;
  ਜੇ (localtime_rz (tz, &t.tv_sec, &tm))
    nstrftime (str, ਆਕਾਰ str, "%Y-%m-%d %H:%M:%S.%N %z", &tm, tz, ns);
  ਨਹੀਂਤਾਂ
    {
      ਅੱਖਰ secbuf[INT_BUFSIZE_BOUND (intmax_t)];
      sprintf (str, "%s.%09d", timetostr (t.tv_sec, secbuf), ns);
    }
  ਵਾਪਸ str;
}

/* PFORMAT points to a '%' followed by a prefix of a format, all of
   size PREFIX_LEN.  The flags allowed for this format are
   ALLOWED_FLAGS; remove other printf flags from the prefix, then
   append SUFFIX.  */
ਸਥਿਰ ਖਾਲੀ
make_format (ਅੱਖਰ *pformat, size_t ਅਗੇਤਰ_len, ਅੱਖਰ ਅਚਲ *allowed_flags,
             ਅੱਖਰ ਅਚਲ *suffix)
{
  ਅੱਖਰ *dst = pformat + 1;
  ਅੱਖਰ ਅਚਲ *src;
  ਅੱਖਰ ਅਚਲ *srclim = pformat + ਅਗੇਤਰ_len;
  ਲਈ (src = dst; src < srclim && strchr (printf_flags, *src); src++)
    ਜੇ (strchr (allowed_flags, *src))
      *dst++ = *src;
  ਜਦੋਂ (src < srclim)
    *dst++ = *src++;
  strcpy (dst, suffix);
}

ਸਥਿਰ ਖਾਲੀ
ਬਾਹਰਲਾ_string (ਅੱਖਰ *pformat, size_t ਅਗੇਤਰ_len, ਅੱਖਰ ਅਚਲ *arg)
{
  make_format (pformat, ਅਗੇਤਰ_len, "-", "s");
  ਲਿਖੋ (pformat, arg);
}
ਸਥਿਰ ਪੂਰਨ
ਬਾਹਰਲਾ_int (ਅੱਖਰ *pformat, size_t ਅਗੇਤਰ_len, intmax_t arg)
{
  make_format (pformat, ਅਗੇਤਰ_len, "'-+ 0", "jd");
  ਵਾਪਸ ਲਿਖੋ (pformat, arg);
}
ਸਥਿਰ ਪੂਰਨ
ਬਾਹਰਲਾ_uint (ਅੱਖਰ *pformat, size_t ਅਗੇਤਰ_len, uintmax_t arg)
{
  make_format (pformat, ਅਗੇਤਰ_len, "'-0", "ju");
  ਵਾਪਸ ਲਿਖੋ (pformat, arg);
}
ਸਥਿਰ ਖਾਲੀ
ਬਾਹਰਲਾ_uint_o (ਅੱਖਰ *pformat, size_t ਅਗੇਤਰ_len, uintmax_t arg)
{
  make_format (pformat, ਅਗੇਤਰ_len, "-#0", "jo");
  ਲਿਖੋ (pformat, arg);
}
ਸਥਿਰ ਖਾਲੀ
ਬਾਹਰਲਾ_uint_x (ਅੱਖਰ *pformat, size_t ਅਗੇਤਰ_len, uintmax_t arg)
{
  make_format (pformat, ਅਗੇਤਰ_len, "-#0", "jx");
  ਲਿਖੋ (pformat, arg);
}
ਸਥਿਰ ਪੂਰਨ
ਬਾਹਰਲਾ_minus_zero (ਅੱਖਰ *pformat, size_t ਅਗੇਤਰ_len)
{
  make_format (pformat, ਅਗੇਤਰ_len, "'-+ 0", ".0f");
  ਵਾਪਸ ਲਿਖੋ (pformat, -0.25);
}

/* Output the number of seconds since the Epoch, using a format that
   acts like printf's %f format.  */
ਸਥਿਰ ਖਾਲੀ
ਬਾਹਰਲਾ_epoch_sec (ਅੱਖਰ *pformat, size_t ਅਗੇਤਰ_len,
               ਬਣਤਰ timespec arg)
{
  ਅੱਖਰ *dot = memchr (pformat, '.', ਅਗੇਤਰ_len);
  size_t sec_ਅਗੇਤਰ_len = ਅਗੇਤਰ_len;
  ਪੂਰਨ ਚੌੜਾਈ = 0;
  ਪੂਰਨ precision = 0;
  bool frac_ਖੱਬਾ_adjust = false;

  ਜੇ (dot)
    {
      sec_ਅਗੇਤਰ_len = dot - pformat;
      pformat[ਅਗੇਤਰ_len] = '\0';

      ਜੇ (c_isdigit (dot[1]))
        {
          ਲੰਮਾ ਪੂਰਨ lprec = strtol (dot + 1, ਨਿਰਾ, 10);
          precision = (lprec <= ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ ? lprec : ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ);
        }
      ਨਹੀਂਤਾਂ
        {
          precision = 9;
        }

      ਜੇ (precision && c_isdigit (dot[-1]))
        {
          /* If a nontrivial width is given, subtract the width of the
             decimal point and PRECISION digits that will be output
             later.  */
          ਅੱਖਰ *p = dot;
          *dot = '\0';

          ਕਰੋ
            --p;
          ਜਦੋਂ (c_isdigit (p[-1]));

          ਲੰਮਾ ਪੂਰਨ lwidth = strtol (p, ਨਿਰਾ, 10);
          ਚੌੜਾਈ = (lwidth <= ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ ? lwidth : ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ);
          ਜੇ (1 < ਚੌੜਾਈ)
            {
              p += (*p == '0');
              sec_ਅਗੇਤਰ_len = p - pformat;
              ਪੂਰਨ w_d = (decimal_ਬਿੰਦੂ_len < ਚੌੜਾਈ
                         ? ਚੌੜਾਈ - decimal_ਬਿੰਦੂ_len
                         : 0);
              ਜੇ (1 < w_d)
                {
                  ਪੂਰਨ w = w_d - precision;
                  ਜੇ (1 < w)
                    {
                      ਅੱਖਰ *dst = pformat;
                      ਲਈ (ਅੱਖਰ ਅਚਲ *src = dst; src < p; src++)
                        {
                          ਜੇ (*src == '-')
                            frac_ਖੱਬਾ_adjust = true;
                          ਨਹੀਂਤਾਂ
                            *dst++ = *src;
                        }
                      sec_ਅਗੇਤਰ_len =
                        (dst - pformat
                         + (frac_ਖੱਬਾ_adjust ? 0 : sprintf (dst, "%d", w)));
                    }
                }
            }
        }
    }

  ਪੂਰਨ ਭਾਜਕ = 1;
  ਲਈ (ਪੂਰਨ i = precision; i < 9; i++)
    ਭਾਜਕ *= 10;
  ਪੂਰਨ frac_sec = arg.tv_nsec / ਭਾਜਕ;
  ਪੂਰਨ int_len;

  ਜੇ (TYPE_SIGNED (time_t))
    {
      bool minus_zero = false;
      ਜੇ (arg.tv_sec < 0 && arg.tv_nsec != 0)
        {
          ਪੂਰਨ frac_sec_modulus = 1000000000 / ਭਾਜਕ;
          frac_sec = (frac_sec_modulus - frac_sec
                      - (arg.tv_nsec % ਭਾਜਕ != 0));
          arg.tv_sec += (frac_sec != 0);
          minus_zero = (arg.tv_sec == 0);
        }
      int_len = (minus_zero
                 ? ਬਾਹਰਲਾ_minus_zero (pformat, sec_ਅਗੇਤਰ_len)
                 : ਬਾਹਰਲਾ_int (pformat, sec_ਅਗੇਤਰ_len, arg.tv_sec));
    }
  ਨਹੀਂਤਾਂ
    int_len = ਬਾਹਰਲਾ_uint (pformat, sec_ਅਗੇਤਰ_len, arg.tv_sec);

  ਜੇ (precision)
    {
      ਪੂਰਨ prec = (precision < 9 ? precision : 9);
      ਪੂਰਨ trailing_prec = precision - prec;
      ਪੂਰਨ ilen = (int_len < 0 ? 0 : int_len);
      ਪੂਰਨ trailing_ਚੌੜਾਈ = (ilen < ਚੌੜਾਈ && decimal_ਬਿੰਦੂ_len < ਚੌੜਾਈ - ilen
                            ? ਚੌੜਾਈ - ilen - decimal_ਬਿੰਦੂ_len - prec
                            : 0);
      ਲਿਖੋ ("%s%.*d%-*.*d", decimal_ਬਿੰਦੂ, prec, frac_sec,
              trailing_ਚੌੜਾਈ, trailing_prec, 0);
    }
}

/* Print the context information of FILENAME, and return true iff the
   context could not be obtained.  */
NODISCARD
ਸਥਿਰ bool
ਬਾਹਰਲਾ_file_context (ਅੱਖਰ *pformat, size_t ਅਗੇਤਰ_len, ਅੱਖਰ ਅਚਲ *filename)
{
  ਅੱਖਰ *scontext;
  bool fail = false;

  ਜੇ ((follow_links
       ? getfilecon (filename, &scontext)
       : lgetfilecon (filename, &scontext)) < 0)
    {
      error (0, errno, _("failed to get security context of %s"),
             quoteaf (filename));
      scontext = ਨਿਰਾ;
      fail = true;
    }
  strcpy (pformat + ਅਗੇਤਰ_len, "s");
  ਲਿਖੋ (pformat, (scontext ? scontext : "?"));
  ਜੇ (scontext)
    freecon (scontext);
  ਵਾਪਸ fail;
}

/* Set the quoting style once it is needed.  */
ਸਥਿਰ ਖਾਲੀ
initialize_quoting_style (ਖਾਲੀ)
{
  ਸਥਿਰ bool initialized;
  ਜੇ (!initialized)
    {
      initialized = true;
      ਪੂਰਨ qs = getenv_quoting_style ();
      ਸੈੱਟ_quoting_style (ਨਿਰਾ, 0 <= qs ? qs : shell_escape_quoting_style);
    }
}

/* Print statfs info.  Return zero upon success, nonzero upon failure.  */
NODISCARD
ਸਥਿਰ bool
print_statfs (ਅੱਖਰ *pformat, size_t ਅਗੇਤਰ_len, MAYBE_ਅਣਵਰਤਿਆ ਅੱਖਰ mod, ਅੱਖਰ m,
              MAYBE_ਅਣਵਰਤਿਆ ਪੂਰਨ fd, ਅੱਖਰ ਅਚਲ *filename,
              ਖਾਲੀ ਅਚਲ *ਅੰਕੜੇ)
{
  STRUCT_STATVFS ਅਚਲ *statfsbuf = ਅੰਕੜੇ;
  bool fail = false;

  ਚੋਣ (m)
    {
    ਹਾਲਤ 'n':
      ਜੇ (mod == 'Q')
        {
          initialize_quoting_style ();
          filename = quoteN (filename);
        }
      ਬਾਹਰਲਾ_string (pformat, ਅਗੇਤਰ_len, filename);
      ਰੋਕੋ;

    ਹਾਲਤ 'i':
      {
#ਜੇ STRUCT_STATXFS_F_FSID_IS_INTEGER
        uintmax_t fsid = statfsbuf->f_fsid;
#ਨਹੀਂਤਾਂ
        ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ fsid_ਸ਼ਬਦ;
        static_assert (alignof (STRUCT_STATVFS) % alignof (fsid_ਸ਼ਬਦ) == 0);
        static_assert (offsetof (STRUCT_STATVFS, f_fsid) % alignof (fsid_ਸ਼ਬਦ)
                       == 0);
        static_assert (ਆਕਾਰ statfsbuf->f_fsid % alignof (fsid_ਸ਼ਬਦ) == 0);
        fsid_ਸ਼ਬਦ ਅਚਲ *p = (fsid_ਸ਼ਬਦ *) &statfsbuf->f_fsid;

        /* Assume a little-endian word order, as that is compatible
           with glibc's statvfs implementation.  */
        uintmax_t fsid = 0;
        ਪੂਰਨ ਸ਼ਬਦ_4 = ਆਕਾਰ statfsbuf->f_fsid / ਆਕਾਰ *p;
        ਲਈ (ਪੂਰਨ i = 0; i < ਸ਼ਬਦ_4 && i * ਆਕਾਰ *p < ਆਕਾਰ fsid; i++)
          {
            uintmax_t u = p[ਸ਼ਬਦ_4 - 1 - i];
            fsid |= u << (i * CHAR_BIT * ਆਕਾਰ *p);
          }
#ਜੇ_ਅੰਤ
        ਬਾਹਰਲਾ_uint_x (pformat, ਅਗੇਤਰ_len, fsid);
      }
      ਰੋਕੋ;

    ਹਾਲਤ 'l':
      ਬਾਹਰਲਾ_NAMEMAX (pformat, ਅਗੇਤਰ_len, SB_F_NAMEMAX (statfsbuf));
      ਰੋਕੋ;
    ਹਾਲਤ 't':
#ਜੇ HAVE_STRUCT_STATXFS_F_TYPE
      ਬਾਹਰਲਾ_uint_x (pformat, ਅਗੇਤਰ_len, statfsbuf->f_type);
#ਨਹੀਂਤਾਂ
      fputc ('?', stdout);
#ਜੇ_ਅੰਤ
      ਰੋਕੋ;
    ਹਾਲਤ 'T':
      ਬਾਹਰਲਾ_string (pformat, ਅਗੇਤਰ_len, human_fstype (statfsbuf));
      ਰੋਕੋ;
    ਹਾਲਤ 'b':
      ਬਾਹਰਲਾ_int (pformat, ਅਗੇਤਰ_len, statfsbuf->f_ਖੰਡਾਂ);
      ਰੋਕੋ;
    ਹਾਲਤ 'f':
      ਬਾਹਰਲਾ_int (pformat, ਅਗੇਤਰ_len, statfsbuf->f_bfree);
      ਰੋਕੋ;
    ਹਾਲਤ 'a':
      ਬਾਹਰਲਾ_int (pformat, ਅਗੇਤਰ_len, statfsbuf->f_bavail);
      ਰੋਕੋ;
    ਹਾਲਤ 's':
      ਬਾਹਰਲਾ_uint (pformat, ਅਗੇਤਰ_len, statfsbuf->f_bsize);
      ਰੋਕੋ;
    ਹਾਲਤ 'S':
      {
        uintmax_t frsize = STATFS_FRSIZE (statfsbuf);
        ਜੇ (! frsize)
          frsize = statfsbuf->f_bsize;
        ਬਾਹਰਲਾ_uint (pformat, ਅਗੇਤਰ_len, frsize);
      }
      ਰੋਕੋ;
    ਹਾਲਤ 'c':
      ਬਾਹਰਲਾ_uint (pformat, ਅਗੇਤਰ_len, statfsbuf->f_files);
      ਰੋਕੋ;
    ਹਾਲਤ 'd':
      ਬਾਹਰਲਾ_int (pformat, ਅਗੇਤਰ_len, statfsbuf->f_ffree);
      ਰੋਕੋ;
    ਮੂਲ:
      fputc ('?', stdout);
      ਰੋਕੋ;
    }
  ਵਾਪਸ fail;
}

/* Return any bind mounted source for a path.
   The caller should not free the returned buffer.
   Return NULL if no bind mount found.  */
NODISCARD
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *
ਲੱਭੋ_bind_mount (ਅੱਖਰ ਅਚਲ * name)
{
  ਅੱਖਰ ਅਚਲ * bind_mount = ਨਿਰਾ;

  ਸਥਿਰ ਬਣਤਰ mount_ਇੰਦਰਾਜ *mount_list;
  ਸਥਿਰ bool tried_mount_list = false;
  ਜੇ (!tried_mount_list) /* attempt/warn once per process.  */
    {
      ਜੇ (!(mount_list = read_file_system_list (false)))
        error (0, errno, "%s", _("cannot read table of mounted file systems"));
      tried_mount_list = true;
    }

  ਬਣਤਰ stat name_stats;
  ਜੇ (stat (name, &name_stats) != 0)
    ਵਾਪਸ ਨਿਰਾ;

  ਲਈ (ਬਣਤਰ mount_ਇੰਦਰਾਜ *me = mount_list; me; me = me->me_ਅਗਲਾ)
    {
      ਜੇ (me->me_dummy && me->me_devname[0] == '/'
          && streq (me->me_mountdir, name))
        {
          ਬਣਤਰ stat dev_stats;

          ਜੇ (stat (me->me_devname, &dev_stats) == 0
              && psame_inode (&name_stats, &dev_stats))
            {
              bind_mount = me->me_devname;
              ਰੋਕੋ;
            }
        }
    }

  ਵਾਪਸ bind_mount;
}

/* Print mount point.  Return zero upon success, nonzero upon failure.  */
NODISCARD
ਸਥਿਰ bool
ਬਾਹਰਲਾ_mount_ਬਿੰਦੂ (ਅੱਖਰ ਅਚਲ *filename, ਅੱਖਰ *pformat, size_t ਅਗੇਤਰ_len,
                 ਅਚਲ ਬਣਤਰ stat *statp)
{

  ਅੱਖਰ ਅਚਲ *np = "?", *bp = ਨਿਰਾ;
  ਅੱਖਰ *mp = ਨਿਰਾ;
  bool fail = true;

  /* Look for bind mounts first.  Note we output the immediate alias,
     rather than further resolving to a base device mount point.  */
  ਜੇ (follow_links || !S_ISLNK (statp->st_ਬਹੁਲਕ))
    {
      ਅੱਖਰ *resolved = canonicalize_file_name (filename);
      ਜੇ (!resolved)
        {
          error (0, errno, _("failed to canonicalize %s"), quoteaf (filename));
          ਜਾਓ print_mount_ਬਿੰਦੂ;
        }
      bp = ਲੱਭੋ_bind_mount (resolved);
      ਮੁਕਤ (resolved);
      ਜੇ (bp)
        {
          fail = false;
          ਜਾਓ print_mount_ਬਿੰਦੂ;
        }
    }

  /* If there is no direct bind mount, then navigate
     back up the tree looking for a device change.
     Note we don't detect if any of the directory components
     are bind mounted to the same device, but that's OK
     since we've not directly queried them.  */
  ਜੇ ((mp = ਲੱਭੋ_mount_ਬਿੰਦੂ (filename, statp)))
    {
      /* This dir might be bind mounted to another device,
         so we resolve the bound source in that case also.  */
      bp = ਲੱਭੋ_bind_mount (mp);
      fail = false;
    }

print_mount_ਬਿੰਦੂ:

  ਬਾਹਰਲਾ_string (pformat, ਅਗੇਤਰ_len, bp ? bp : mp ? mp : np);
  ਮੁਕਤ (mp);
  ਵਾਪਸ fail;
}

/* Map a TS with negative TS.tv_nsec to {0,0}.  */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ timespec
neg_ਮੰਜ਼ਿਲ_zero (ਬਣਤਰ timespec ts)
{
  ਜੇ (0 <= ts.tv_nsec)
    ਵਾਪਸ ts;
  ਬਣਤਰ timespec z = {0};
  ਵਾਪਸ z;
}

/* Output a single-character \ escape.  */

ਸਥਿਰ ਖਾਲੀ
print_esc_char (ਅੱਖਰ c)
{
  ਚੋਣ (c)
    {
    ਹਾਲਤ 'a':			/* Alert. */
      c ='\a';
      ਰੋਕੋ;
    ਹਾਲਤ 'b':			/* Backspace. */
      c ='\b';
      ਰੋਕੋ;
    ਹਾਲਤ 'e':			/* Escape. */
      c ='\x1B';
      ਰੋਕੋ;
    ਹਾਲਤ 'f':			/* Form feed. */
      c ='\f';
      ਰੋਕੋ;
    ਹਾਲਤ 'n':			/* New line. */
      c ='\n';
      ਰੋਕੋ;
    ਹਾਲਤ 'r':			/* Carriage return. */
      c ='\r';
      ਰੋਕੋ;
    ਹਾਲਤ 't':			/* Horizontal tab. */
      c ='\t';
      ਰੋਕੋ;
    ਹਾਲਤ 'v':			/* Vertical tab. */
      c ='\v';
      ਰੋਕੋ;
    ਹਾਲਤ '"':
    ਹਾਲਤ '\\':
      ਰੋਕੋ;
    ਮੂਲ:
      error (0, 0, _("warning: unrecognized escape '\\%c'"), c);
      ਰੋਕੋ;
    }
  ਅੱਖਰ_ਲਿਖੋ (c);
}

ATTRIBUTE_ਸ਼ੁੱਧ
ਸਥਿਰ size_t
format_ਕੋਡ_ਵਿੱਥ (ਅੱਖਰ ਅਚਲ *directive)
{
  size_t len = strspn (directive + 1, printf_flags);
  ਅੱਖਰ ਅਚਲ *fmt_char = directive + len + 1;
  fmt_char += strspn (fmt_char, digits);
  ਜੇ (*fmt_char == '.')
    fmt_char += 1 + strspn (fmt_char + 1, digits);
  ਵਾਪਸ fmt_char - directive;
}

/* Print the information specified by the format string, FORMAT,
   calling PRINT_FUNC for each %-directive encountered.
   Return zero upon success, nonzero upon failure.  */
NODISCARD
ਸਥਿਰ bool
print_it (ਅੱਖਰ ਅਚਲ *format, ਪੂਰਨ fd, ਅੱਖਰ ਅਚਲ *filename,
          bool (*print_func) (ਅੱਖਰ *, size_t, ਅੱਖਰ, ਅੱਖਰ,
                              ਪੂਰਨ, ਅੱਖਰ ਅਚਲ *, ਖਾਲੀ ਅਚਲ *),
          ਖਾਲੀ ਅਚਲ *ਅੰਕੜੇ)
{
  bool fail = false;

  /* Add 2 to accommodate our conversion of the stat '%s' format string
     to the longer printf '%llu' one.  */
  ਗਿਣਤੀ
    {
      MAX_ADDITIONAL_ਬਾਈਟਾਂ =
        (MAX (ਆਕਾਰ "jd",
              MAX (ਆਕਾਰ "jo", MAX (ਆਕਾਰ "ju", ਆਕਾਰ "jx")))
         - 1)
    };
  size_t n_alloc = ਲੜੀ_ਲੰਬਾਈ (format) + MAX_ADDITIONAL_ਬਾਈਟਾਂ + 1;
  ਅੱਖਰ *dest = xmalloc (n_alloc);
  ਲਈ (ਅੱਖਰ ਅਚਲ *b = format; *b; b++)
    {
      ਚੋਣ (*b)
        {
        ਹਾਲਤ '%':
          {
            size_t len = format_ਕੋਡ_ਵਿੱਥ (b);
            ਅੱਖਰ fmt_char = *(b + len);
            ਅੱਖਰ mod_char = 0;
            ਸਮ੍ਰਿਤੀ_ਨਕਲ (dest, b, len);
            b += len;

            ਚੋਣ (fmt_char)
              {
              ਹਾਲਤ '\0':
                --b;
                FALLTHROUGH;
              ਹਾਲਤ '%':
                ਜੇ (1 < len)
                  {
                    dest[len] = fmt_char;
                    dest[len + 1] = '\0';
                    error (EXIT_FAILURE, 0, _("%s: invalid directive"),
                           quote (dest));
                  }
                ਅੱਖਰ_ਲਿਖੋ ('%');
                ਰੋਕੋ;
              ਹਾਲਤ 'H':
              ਹਾਲਤ 'L':
              ਹਾਲਤ 'Q':
                mod_char = fmt_char;
                fmt_char = *(b + 1);
                ਜੇ ((mod_char == 'Q' && fmt_char == 'n')
                    || (print_func == print_stat
                        && (mod_char == 'H' || mod_char == 'L')
                        && (fmt_char == 'd' || fmt_char == 'r')))
                  {
                    b++;
                  }
                ਨਹੀਂਤਾਂ
                  {
                    fmt_char = mod_char;
                    mod_char = 0;
                  }
                FALLTHROUGH;
              ਮੂਲ:
                fail |= print_func (dest, len, mod_char, fmt_char,
                                    fd, filename, ਅੰਕੜੇ);
                ਰੋਕੋ;
              }
            ਰੋਕੋ;
          }

        ਹਾਲਤ '\\':
          ਜੇ ( ! interpret_backslash_escapes)
            {
              ਅੱਖਰ_ਲਿਖੋ ('\\');
              ਰੋਕੋ;
            }
          ++b;
          ਜੇ (isoct (*b))
            {
              ਪੂਰਨ esc_ਮੁੱਲ = fromoct (*b);
              ਪੂਰਨ esc_ਲੰਮਾਈ = 1;	/* number of octal digits */
              ਲਈ (++b; esc_ਲੰਮਾਈ < 3 && isoct (*b);
                   ++esc_ਲੰਮਾਈ, ++b)
                {
                  esc_ਮੁੱਲ = esc_ਮੁੱਲ * 8 + fromoct (*b);
                }
              ਅੱਖਰ_ਲਿਖੋ (esc_ਮੁੱਲ);
              --b;
            }
          ਨਹੀਂਤਾਂ ਜੇ (*b == 'x' && c_isxdigit (b[1]))
            {
              ਪੂਰਨ esc_ਮੁੱਲ = fromhex (b[1]);	/* Value of \xhh escape. */
              /* A hexadecimal \xhh escape sequence must have
                 1 or 2 hex. digits.  */
              ++b;
              ਜੇ (c_isxdigit (b[1]))
                {
                  ++b;
                  esc_ਮੁੱਲ = esc_ਮੁੱਲ * 16 + fromhex (*b);
                }
              ਅੱਖਰ_ਲਿਖੋ (esc_ਮੁੱਲ);
            }
          ਨਹੀਂਤਾਂ ਜੇ (*b == '\0')
            {
              error (0, 0, _("warning: backslash at end of format"));
              ਅੱਖਰ_ਲਿਖੋ ('\\');
              /* Arrange to exit the loop.  */
              --b;
            }
          ਨਹੀਂਤਾਂ
            {
              print_esc_char (*b);
            }
          ਰੋਕੋ;

        ਮੂਲ:
          ਅੱਖਰ_ਲਿਖੋ (*b);
          ਰੋਕੋ;
        }
    }
  ਮੁਕਤ (dest);

  fputs (trailing_delim, stdout);

  ਵਾਪਸ fail;
}

/* Stat the file system and print what we find.  */
NODISCARD
ਸਥਿਰ bool
do_statfs (ਅੱਖਰ ਅਚਲ *filename, ਅੱਖਰ ਅਚਲ *format)
{
  STRUCT_STATVFS statfsbuf;

  ਜੇ (streq (filename, "-"))
    {
      error (0, 0, _("using %s to denote standard input does not work"
                     " in file system mode"), quoteaf (filename));
      ਵਾਪਸ false;
    }

  ਜੇ (STATFS (filename, &statfsbuf) != 0)
    {
      error (0, errno, _("cannot read file system information for %s"),
             quoteaf (filename));
      ਵਾਪਸ false;
    }

  bool fail = print_it (format, -1, filename, print_statfs, &statfsbuf);
  ਵਾਪਸ ! fail;
}

ਬਣਤਰ print_ਦਲੀਲਾਂ {
  ਬਣਤਰ stat *st;
  ਬਣਤਰ timespec btime;
};

/* Ask statx to avoid syncing? */
ਸਥਿਰ bool dont_sync;

/* Ask statx to force sync? */
ਸਥਿਰ bool force_sync;

#ਜੇ USE_STATX
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
fmt_ਮੰਜ਼ਿਲ_ਨਕਾਬ (ਅੱਖਰ fmt)
{
  ਚੋਣ (fmt)
    {
    ਹਾਲਤ 'N':
      ਵਾਪਸ STATX_ਬਹੁਲਕ;
    ਹਾਲਤ 'd':
    ਹਾਲਤ 'D':
      ਵਾਪਸ STATX_ਬਹੁਲਕ;
    ਹਾਲਤ 'i':
      ਵਾਪਸ STATX_INO;
    ਹਾਲਤ 'a':
    ਹਾਲਤ 'A':
      ਵਾਪਸ STATX_ਬਹੁਲਕ;
    ਹਾਲਤ 'f':
      ਵਾਪਸ STATX_ਬਹੁਲਕ|STATX_TYPE;
    ਹਾਲਤ 'F':
      ਵਾਪਸ STATX_TYPE;
    ਹਾਲਤ 'h':
      ਵਾਪਸ STATX_NLINK;
    ਹਾਲਤ 'u':
    ਹਾਲਤ 'U':
      ਵਾਪਸ STATX_UID;
    ਹਾਲਤ 'g':
    ਹਾਲਤ 'G':
      ਵਾਪਸ STATX_GID;
    ਹਾਲਤ 'm':
      ਵਾਪਸ STATX_ਬਹੁਲਕ|STATX_INO;
    ਹਾਲਤ 's':
      ਵਾਪਸ STATX_ਮਾਪ;
    ਹਾਲਤ 't':
    ਹਾਲਤ 'T':
      ਵਾਪਸ STATX_ਬਹੁਲਕ;
    ਹਾਲਤ 'b':
      ਵਾਪਸ STATX_ਖੰਡਾਂ;
    ਹਾਲਤ 'w':
    ਹਾਲਤ 'W':
      ਵਾਪਸ STATX_BTIME;
    ਹਾਲਤ 'x':
    ਹਾਲਤ 'X':
      ਵਾਪਸ STATX_ATIME;
    ਹਾਲਤ 'y':
    ਹਾਲਤ 'Y':
      ਵਾਪਸ STATX_MTIME;
    ਹਾਲਤ 'z':
    ਹਾਲਤ 'Z':
      ਵਾਪਸ STATX_CTIME;
    }
  ਵਾਪਸ 0;
}

ATTRIBUTE_ਸ਼ੁੱਧ
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
format_ਮੰਜ਼ਿਲ_ਨਕਾਬ (ਅੱਖਰ ਅਚਲ *format)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਨਕਾਬ = 0;

  ਲਈ (ਅੱਖਰ ਅਚਲ *b = format; *b; b++)
    {
      ਜੇ (*b != '%')
        ਜਾਰੀ;

      b += format_ਕੋਡ_ਵਿੱਥ (b);
      ਜੇ (*b == '\0')
        ਰੋਕੋ;
      ਨਕਾਬ |= fmt_ਮੰਜ਼ਿਲ_ਨਕਾਬ (*b);
    }
  ਵਾਪਸ ਨਕਾਬ;
}

/* statx the file and print what we find */
NODISCARD
ਸਥਿਰ bool
do_stat (ਅੱਖਰ ਅਚਲ *filename, ਅੱਖਰ ਅਚਲ *format, ਅੱਖਰ ਅਚਲ *format2)
{
  ਪੂਰਨ fd = streq (filename, "-") ? 0 : AT_FDCWD;
  ਪੂਰਨ flags = 0;
  ਬਣਤਰ stat st;
  ਬਣਤਰ statx stx = {0};
  ਅੱਖਰ ਅਚਲ *pathname = filename;
  ਬਣਤਰ print_ਦਲੀਲਾਂ pa;
  pa.st = &st;
  pa.btime = (ਬਣਤਰ timespec) {.tv_sec = -1, .tv_nsec = -1};

  ਜੇ (AT_FDCWD != fd)
    {
      pathname = "";
      flags = AT_ਖਾਲੀ_ਥਾਵਾਂ_ਰਸਤਾ;
    }
  ਨਹੀਂਤਾਂ ਜੇ (!follow_links)
    {
      flags = AT_SYMLINK_NOFOLLOW;
    }

  ਜੇ (dont_sync)
    flags |= AT_STATX_DONT_SYNC;
  ਨਹੀਂਤਾਂ ਜੇ (force_sync)
    flags |= AT_STATX_FORCE_SYNC;

  ਜੇ (! force_sync)
    flags |= AT_NO_AUTOMOUNT;

  fd = statx (fd, pathname, flags, format_ਮੰਜ਼ਿਲ_ਨਕਾਬ (format), &stx);
  ਜੇ (fd < 0)
    {
      ਜੇ (flags & AT_ਖਾਲੀ_ਥਾਵਾਂ_ਰਸਤਾ)
        error (0, errno, _("cannot stat standard input"));
      ਨਹੀਂਤਾਂ
        error (0, errno, _("cannot statx %s"), quoteaf (filename));
      ਵਾਪਸ false;
    }

  ਜੇ (S_ISBLK (stx.stx_ਬਹੁਲਕ) || S_ISCHR (stx.stx_ਬਹੁਲਕ))
    format = format2;

  statx_ਮੰਜ਼ਿਲ_stat (&stx, &st);
  ਜੇ (stx.stx_ਨਕਾਬ & STATX_BTIME)
    pa.btime = statx_timestamp_ਮੰਜ਼ਿਲ_timespec (stx.stx_btime);

  bool fail = print_it (format, fd, filename, print_stat, &pa);
  ਵਾਪਸ ! fail;
}

#ਨਹੀਂਤਾਂ /* USE_STATX */

ਸਥਿਰ ਬਣਤਰ timespec
get_birthtime (ਪੂਰਨ fd, ਅੱਖਰ ਅਚਲ *filename, ਬਣਤਰ stat ਅਚਲ *st)
{
  ਬਣਤਰ timespec ts = get_stat_birthtime (st);

# ਜੇ HAVE_GETATTRAT
  ਜੇ (ts.tv_nsec < 0)
    {
      nvlist_t *response;
      ਜੇ ((fd < 0
           ? getattrat (AT_FDCWD, XATTR_VIEW_READWRITE, filename, &response)
           : fgetattr (fd, XATTR_VIEW_READWRITE, &response))
          == 0)
        {
          uint64_t *val;
          uint_t n;
          ਜੇ (nvlist_lookup_uint64_array (response, A_CRTIME, &val, &n) == 0
              && 2 <= n
              && val[0] <= TYPE_ਵੱਧ_ਤੋਂ_ਵੱਧ (time_t)
              && val[1] < 1000000000 * 2 /* for leap seconds */)
            {
              ts.tv_sec = val[0];
              ts.tv_nsec = val[1];
            }
          nvlist_free (response);
        }
    }
# endif

  ਵਾਪਸ ts;
}


/* stat the file and print what we find */
NODISCARD
ਸਥਿਰ bool
do_stat (ਅੱਖਰ ਅਚਲ *filename, ਅੱਖਰ ਅਚਲ *format,
         ਅੱਖਰ ਅਚਲ *format2)
{
  ਪੂਰਨ fd = streq (filename, "-") ? 0 : -1;
  ਬਣਤਰ stat statbuf;
  ਬਣਤਰ print_ਦਲੀਲਾਂ pa;
  pa.st = &statbuf;
  pa.btime = (ਬਣਤਰ timespec) {.tv_sec = -1, .tv_nsec = -1};

  ਜੇ (0 <= fd)
    {
      ਜੇ (fstat (fd, &statbuf) != 0)
        {
          error (0, errno, _("cannot stat standard input"));
          ਵਾਪਸ false;
        }
    }
  /* We can't use the shorter
     (follow_links?stat:lstat) (filename, &statbug)
     since stat might be a function-like macro.  */
  ਨਹੀਂਤਾਂ ਜੇ ((follow_links
            ? stat (filename, &statbuf)
            : lstat (filename, &statbuf)) != 0)
    {
      error (0, errno, _("cannot stat %s"), quoteaf (filename));
      ਵਾਪਸ false;
    }

  ਜੇ (S_ISBLK (statbuf.st_ਬਹੁਲਕ) || S_ISCHR (statbuf.st_ਬਹੁਲਕ))
    format = format2;

  bool fail = print_it (format, fd, filename, print_stat, &pa);
  ਵਾਪਸ ! fail;
}
#ਜੇ_ਅੰਤ /* USE_STATX */

/* POSIX requires 'ls' to print file sizes without a sign, even
   when negative.  Be consistent with that.  */

ਸਥਿਰ uintmax_t
unsigned_file_ਮਾਪ (off_t ਮਾਪ)
{
  ਵਾਪਸ ਮਾਪ + (ਮਾਪ < 0) * ((uintmax_t) OFF_T_MAX - OFF_T_MIN + 1);
}

/* Print stat info.  Return zero upon success, nonzero upon failure.  */
ਸਥਿਰ bool
print_stat (ਅੱਖਰ *pformat, size_t ਅਗੇਤਰ_len, ਅੱਖਰ mod, ਅੱਖਰ m,
            MAYBE_ਅਣਵਰਤਿਆ ਪੂਰਨ fd, ਅੱਖਰ ਅਚਲ *filename, ਖਾਲੀ ਅਚਲ *ਅੰਕੜੇ)
{
  ਬਣਤਰ print_ਦਲੀਲਾਂ *parg = (ਬਣਤਰ print_ਦਲੀਲਾਂ *) ਅੰਕੜੇ;
  ਬਣਤਰ stat *statbuf = parg->st;
  ਬਣਤਰ timespec btime = parg->btime;
  ਬਣਤਰ passwd *pw_ent;
  ਬਣਤਰ ਸਮੂਹ *gw_ent;
  bool fail = false;

  ਚੋਣ (m)
    {
    ਹਾਲਤ 'n':
      ਜੇ (mod == 'Q')
        {
          initialize_quoting_style ();
          filename = quoteN (filename);
        }
      ਬਾਹਰਲਾ_string (pformat, ਅਗੇਤਰ_len, filename);
      ਰੋਕੋ;
    ਹਾਲਤ 'N':
      initialize_quoting_style ();
      ਬਾਹਰਲਾ_string (pformat, ਅਗੇਤਰ_len, quoteN (filename));
      ਜੇ (S_ISLNK (statbuf->st_ਬਹੁਲਕ))
        {
          ਅੱਖਰ *linkname = areadlink_with_ਮਾਪ (filename, statbuf->st_ਮਾਪ);
          ਜੇ (linkname == ਨਿਰਾ)
            {
              error (0, errno, _("cannot read symbolic link %s"),
                     quoteaf (filename));
              ਵਾਪਸ true;
            }
          ਲਿਖੋ (" -> ");
          ਬਾਹਰਲਾ_string (pformat, ਅਗੇਤਰ_len, quoteN (linkname));
          ਮੁਕਤ (linkname);
        }
      ਰੋਕੋ;
    ਹਾਲਤ 'd':
      ਜੇ (mod == 'H')
        ਬਾਹਰਲਾ_uint (pformat, ਅਗੇਤਰ_len, major (statbuf->st_dev));
      ਨਹੀਂਤਾਂ ਜੇ (mod == 'L')
        ਬਾਹਰਲਾ_uint (pformat, ਅਗੇਤਰ_len, minor (statbuf->st_dev));
      ਨਹੀਂਤਾਂ
        ਬਾਹਰਲਾ_uint (pformat, ਅਗੇਤਰ_len, statbuf->st_dev);
      ਰੋਕੋ;
    ਹਾਲਤ 'D':
      ਬਾਹਰਲਾ_uint_x (pformat, ਅਗੇਤਰ_len, statbuf->st_dev);
      ਰੋਕੋ;
    ਹਾਲਤ 'i':
      ਬਾਹਰਲਾ_uint (pformat, ਅਗੇਤਰ_len, statbuf->st_ino);
      ਰੋਕੋ;
    ਹਾਲਤ 'a':
      ਬਾਹਰਲਾ_uint_o (pformat, ਅਗੇਤਰ_len, statbuf->st_ਬਹੁਲਕ & CHMOD_ਬਹੁਲਕ_ਬਿੱਟ);
      ਰੋਕੋ;
    ਹਾਲਤ 'A':
      ਬਾਹਰਲਾ_string (pformat, ਅਗੇਤਰ_len, human_access (statbuf));
      ਰੋਕੋ;
    ਹਾਲਤ 'f':
      ਬਾਹਰਲਾ_uint_x (pformat, ਅਗੇਤਰ_len, statbuf->st_ਬਹੁਲਕ);
      ਰੋਕੋ;
    ਹਾਲਤ 'F':
      ਬਾਹਰਲਾ_string (pformat, ਅਗੇਤਰ_len, file_type (statbuf));
      ਰੋਕੋ;
    ਹਾਲਤ 'h':
      ਬਾਹਰਲਾ_uint (pformat, ਅਗੇਤਰ_len, statbuf->st_nlink);
      ਰੋਕੋ;
    ਹਾਲਤ 'u':
      ਬਾਹਰਲਾ_uint (pformat, ਅਗੇਤਰ_len, statbuf->st_uid);
      ਰੋਕੋ;
    ਹਾਲਤ 'U':
      pw_ent = getpwuid (statbuf->st_uid);
      ਬਾਹਰਲਾ_string (pformat, ਅਗੇਤਰ_len,
                  pw_ent ? pw_ent->pw_name : "UNKNOWN");
      ਰੋਕੋ;
    ਹਾਲਤ 'g':
      ਬਾਹਰਲਾ_uint (pformat, ਅਗੇਤਰ_len, statbuf->st_gid);
      ਰੋਕੋ;
    ਹਾਲਤ 'G':
      gw_ent = getgrgid (statbuf->st_gid);
      ਬਾਹਰਲਾ_string (pformat, ਅਗੇਤਰ_len,
                  gw_ent ? gw_ent->gr_name : "UNKNOWN");
      ਰੋਕੋ;
    ਹਾਲਤ 'm':
      fail |= ਬਾਹਰਲਾ_mount_ਬਿੰਦੂ (filename, pformat, ਅਗੇਤਰ_len, statbuf);
      ਰੋਕੋ;
    ਹਾਲਤ 's':
      ਬਾਹਰਲਾ_uint (pformat, ਅਗੇਤਰ_len, unsigned_file_ਮਾਪ (statbuf->st_ਮਾਪ));
      ਰੋਕੋ;
    ਹਾਲਤ 'r':
      ਜੇ (mod == 'H')
        ਬਾਹਰਲਾ_uint (pformat, ਅਗੇਤਰ_len, major (statbuf->st_rdev));
      ਨਹੀਂਤਾਂ ਜੇ (mod == 'L')
        ਬਾਹਰਲਾ_uint (pformat, ਅਗੇਤਰ_len, minor (statbuf->st_rdev));
      ਨਹੀਂਤਾਂ
        ਬਾਹਰਲਾ_uint (pformat, ਅਗੇਤਰ_len, statbuf->st_rdev);
      ਰੋਕੋ;
    ਹਾਲਤ 'R':
      ਬਾਹਰਲਾ_uint_x (pformat, ਅਗੇਤਰ_len, statbuf->st_rdev);
      ਰੋਕੋ;
    ਹਾਲਤ 't':
      ਬਾਹਰਲਾ_uint_x (pformat, ਅਗੇਤਰ_len, major (statbuf->st_rdev));
      ਰੋਕੋ;
    ਹਾਲਤ 'T':
      ਬਾਹਰਲਾ_uint_x (pformat, ਅਗੇਤਰ_len, minor (statbuf->st_rdev));
      ਰੋਕੋ;
    ਹਾਲਤ 'B':
      ਬਾਹਰਲਾ_uint (pformat, ਅਗੇਤਰ_len, ST_NBLOCKSIZE);
      ਰੋਕੋ;
    ਹਾਲਤ 'b':
      ਬਾਹਰਲਾ_uint (pformat, ਅਗੇਤਰ_len, STP_NBLOCKS (statbuf));
      ਰੋਕੋ;
    ਹਾਲਤ 'o':
      ਬਾਹਰਲਾ_uint (pformat, ਅਗੇਤਰ_len, STP_BLKSIZE (statbuf));
      ਰੋਕੋ;
    ਹਾਲਤ 'w':
      {
#ਜੇ ! USE_STATX
        btime = get_birthtime (fd, filename, statbuf);
#ਜੇ_ਅੰਤ
        ਜੇ (btime.tv_nsec < 0)
          ਬਾਹਰਲਾ_string (pformat, ਅਗੇਤਰ_len, "-");
        ਨਹੀਂਤਾਂ
          ਬਾਹਰਲਾ_string (pformat, ਅਗੇਤਰ_len, human_ਸਮਾਂ (btime));
      }
      ਰੋਕੋ;
    ਹਾਲਤ 'W':
      {
#ਜੇ ! USE_STATX
        btime = get_birthtime (fd, filename, statbuf);
#ਜੇ_ਅੰਤ
        ਬਾਹਰਲਾ_epoch_sec (pformat, ਅਗੇਤਰ_len, neg_ਮੰਜ਼ਿਲ_zero (btime));
      }
      ਰੋਕੋ;
    ਹਾਲਤ 'x':
      ਬਾਹਰਲਾ_string (pformat, ਅਗੇਤਰ_len, human_ਸਮਾਂ (get_stat_atime (statbuf)));
      ਰੋਕੋ;
    ਹਾਲਤ 'X':
      ਬਾਹਰਲਾ_epoch_sec (pformat, ਅਗੇਤਰ_len, get_stat_atime (statbuf));
      ਰੋਕੋ;
    ਹਾਲਤ 'y':
      ਬਾਹਰਲਾ_string (pformat, ਅਗੇਤਰ_len, human_ਸਮਾਂ (get_stat_mtime (statbuf)));
      ਰੋਕੋ;
    ਹਾਲਤ 'Y':
      ਬਾਹਰਲਾ_epoch_sec (pformat, ਅਗੇਤਰ_len, get_stat_mtime (statbuf));
      ਰੋਕੋ;
    ਹਾਲਤ 'z':
      ਬਾਹਰਲਾ_string (pformat, ਅਗੇਤਰ_len, human_ਸਮਾਂ (get_stat_ctime (statbuf)));
      ਰੋਕੋ;
    ਹਾਲਤ 'Z':
      ਬਾਹਰਲਾ_epoch_sec (pformat, ਅਗੇਤਰ_len, get_stat_ctime (statbuf));
      ਰੋਕੋ;
    ਹਾਲਤ 'C':
      fail |= ਬਾਹਰਲਾ_file_context (pformat, ਅਗੇਤਰ_len, filename);
      ਰੋਕੋ;
    ਮੂਲ:
      fputc ('?', stdout);
      ਰੋਕੋ;
    }
  ਵਾਪਸ fail;
}

/* Return an allocated format string in static storage that
   corresponds to whether FS and TERSE options were declared.  */
ਸਥਿਰ ਅੱਖਰ *
default_format (bool fs, bool terse, bool device)
{
  ਅੱਖਰ *format;
  ਜੇ (fs)
    {
      ਜੇ (terse)
        format = xstrdup (fmt_terse_fs);
      ਨਹੀਂਤਾਂ
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
  ਨਹੀਂਤਾਂ /* ! fs */
    {
      ਜੇ (terse)
        {
          ਜੇ (0 < is_selinux_enabled ())
            format = xstrdup (fmt_terse_selinux);
          ਨਹੀਂਤਾਂ
            format = xstrdup (fmt_terse_regular);
        }
      ਨਹੀਂਤਾਂ
        {
          ਅੱਖਰ *ਆਰਜ਼ੀ;
          /* TRANSLATORS: This string uses format specifiers from
             'stat --help' without --file-system, and NOT from printf.  */
          format = xstrdup (_("\
  File: %N\n\
  ਮਾਪ_3: %-10s\t_ਖੰਡਾਂ: %-10b IO ਖੰਡ_3: %-6o %F\n\
"));

          ਆਰਜ਼ੀ = format;
          ਜੇ (device)
            {
              /* TRANSLATORS: This string uses format specifiers from
                 'stat --help' without --file-system, and NOT from printf.  */
              format = xasprintf ("%s%s", format, _("\
" "Device: %Hd,%Ld\tInode: %-10i  Links: %-5h Device type: %Hr,%Lr\n\
"));
            }
          ਨਹੀਂਤਾਂ
            {
              /* TRANSLATORS: This string uses format specifiers from
                 'stat --help' without --file-system, and NOT from printf.  */
              format = xasprintf ("%s%s", format, _("\
" "Device: %Hd,%Ld\tInode: %-10i  Links: %h\n\
"));
            }
          ਮੁਕਤ (ਆਰਜ਼ੀ);

          ਆਰਜ਼ੀ = format;
          /* TRANSLATORS: This string uses format specifiers from
             'stat --help' without --file-system, and NOT from printf.  */
          format = xasprintf ("%s%s", format, _("\
" "Access: (%04a/%10.10A)  Uid: (%5u/%8U)   Gid: (%5g/%8G)\n\
"));
          ਮੁਕਤ (ਆਰਜ਼ੀ);

          ਜੇ (0 < is_selinux_enabled ())
            {
              ਆਰਜ਼ੀ = format;
              /* TRANSLATORS: This string uses format specifiers from
                 'stat --help' without --file-system, and NOT from printf.  */
              format = xasprintf ("%s%s", format, _("Context: %C\n"));
              ਮੁਕਤ (ਆਰਜ਼ੀ);
            }

          ਆਰਜ਼ੀ = format;
          format = xasprintf ("%s%s", format,
                              /* TRANSLATORS: This string uses format specifiers
                                 from 'stat --help' without --file-system, and
                                 NOT from printf.  */
                              _("Access: %x\n"
                                "Modify: %y\n"
                                "Change: %z\n"
                                " Birth: %w\n"));
          ਮੁਕਤ (ਆਰਜ਼ੀ);
        }
    }
  ਵਾਪਸ format;
}

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("Usage: %s [OPTION]... FILE...\n"), ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
Display file or file system ਸਥਿਤੀ.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -L, --dereference\n\
         follow links\n\
"));
      oputs (_("\
  -f, --file-system\n\
         display file system ਸਥਿਤੀ instead ਦਾ file ਸਥਿਤੀ\n\
"));
      oputs (_("\
      --cached=ਬਹੁਲਕ\n\
         specify how ਮੰਜ਼ਿਲ use cached ਗੁਣ;\n\
         useful on remote file systems. See ਬਹੁਲਕ ਹੇਠਾਂ\n\
"));
      oputs (_("\
  -c, --format=FORMAT\n\
         use the specified FORMAT instead ਦਾ the ਮੂਲ;\n\
         output a newline after each use ਦਾ FORMAT\n\
"));
      oputs (_("\
      --ਲਿਖੋ=FORMAT\n\
         like --format, but interpret backslash escapes,\n\
         and ਕਰੋ not output a mandatory trailing newline;\n\
         ਜੇ you want a newline, include \\n ਵਿੱਚ FORMAT\n\
"));
      oputs (_("\
  -t, --terse\n\
         print the information ਵਿੱਚ terse form\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);

      fputs (_("\n\
The ਬਹੁਲਕ ਦਲੀਲ ਦਾ --cached can be: always, never, or ਮੂਲ.\n\
'always' will use cached ਗੁਣ ਜੇ ਉਪਲਬਧ, ਜਦੋਂ\n\
'never' will try ਮੰਜ਼ਿਲ synchronize with the latest ਗੁਣ, and\n\
'default' will leave it ਉੱਪਰ ਮੰਜ਼ਿਲ the underlying file system.\n\
"), stdout);

      fputs (_("\n\
The valid format sequences ਲਈ files (without --file-system):\n\
\n\
  %a   permission ਬਿੱਟ_2 ਵਿੱਚ octal (see '#' and '0' ਲਿਖੋ flags)\n\
  %A   permission ਬਿੱਟ_2 and file type ਵਿੱਚ human readable form\n\
  %b   ਸੰਖਿਆ ਦਾ ਖੰਡਾਂ allocated (see %B)\n\
  %B   the ਮਾਪ ਵਿੱਚ ਬਾਈਟਾਂ ਦਾ each ਖੰਡ reported by %b\n\
  %C   SELinux security context string\n\
"), stdout);
      fputs (_("\
  %d   device ਸੰਖਿਆ ਵਿੱਚ decimal (st_dev)\n\
  %D   device ਸੰਖਿਆ ਵਿੱਚ hex (st_dev)\n\
  %Hd  major device ਸੰਖਿਆ ਵਿੱਚ decimal\n\
  %Ld  minor device ਸੰਖਿਆ ਵਿੱਚ decimal\n\
  %f   raw ਬਹੁਲਕ_2 ਵਿੱਚ hex\n\
  %F   file type\n\
  %g   ਸਮੂਹ ID ਦਾ owner\n\
  %G   ਸਮੂਹ name ਦਾ owner\n\
"), stdout);
      fputs (_("\
  %h   ਸੰਖਿਆ ਦਾ hard links\n\
  %i   inode ਸੰਖਿਆ\n\
  %m   mount ਬਿੰਦੂ\n\
  %n   file name\n\
  %Qn  quoted file name\n\
  %N   quoted file name with dereference ਜੇ symbolic link\n\
  %o   optimal I/O transfer ਮਾਪ hint\n\
  %s   ਕੁੱਲ ਮਾਪ, ਵਿੱਚ ਬਾਈਟਾਂ\n\
  %r   device type ਵਿੱਚ decimal (st_rdev)\n\
  %R   device type ਵਿੱਚ hex (st_rdev)\n\
  %Hr  major device type ਵਿੱਚ decimal, ਲਈ character/ਖੰਡ device special files\n\
  %Lr  minor device type ਵਿੱਚ decimal, ਲਈ character/ਖੰਡ device special files\n\
  %t   major device type ਵਿੱਚ hex, ਲਈ character/ਖੰਡ device special files\n\
  %T   minor device type ਵਿੱਚ hex, ਲਈ character/ਖੰਡ device special files\n\
"), stdout);
      fputs (_("\
  %u   user ID ਦਾ owner\n\
  %U   user name ਦਾ owner\n\
  %w   ਸਮਾਂ ਦਾ file birth, human-readable; - ਜੇ unknown\n\
  %W   ਸਮਾਂ ਦਾ file birth, seconds since Epoch; 0 ਜੇ unknown\n\
  %x   ਸਮਾਂ ਦਾ ਆਖਰੀ access, human-readable\n\
  %X   ਸਮਾਂ ਦਾ ਆਖਰੀ access, seconds since Epoch\n\
  %y   ਸਮਾਂ ਦਾ ਆਖਰੀ ਅੰਕੜੇ modification, human-readable\n\
  %Y   ਸਮਾਂ ਦਾ ਆਖਰੀ ਅੰਕੜੇ modification, seconds since Epoch\n\
  %z   ਸਮਾਂ ਦਾ ਆਖਰੀ ਸਥਿਤੀ change, human-readable\n\
  %Z   ਸਮਾਂ ਦਾ ਆਖਰੀ ਸਥਿਤੀ change, seconds since Epoch\n\
\n\
"), stdout);

      fputs (_("\
Valid format sequences ਲਈ file systems:\n\
\n\
  %a   ਮੁਕਤ ਖੰਡਾਂ ਉਪਲਬਧ ਮੰਜ਼ਿਲ non-superuser\n\
  %b   ਕੁੱਲ ਅੰਕੜੇ ਖੰਡਾਂ ਵਿੱਚ file system\n\
  %c   ਕੁੱਲ file nodes ਵਿੱਚ file system\n\
  %d   ਮੁਕਤ file nodes ਵਿੱਚ file system\n\
  %f   ਮੁਕਤ ਖੰਡਾਂ ਵਿੱਚ file system\n\
"), stdout);
      fputs (_("\
  %i   file system ID ਵਿੱਚ hex\n\
  %l   ਵੱਧ_ਤੋਂ_ਵੱਧ ਲੰਮਾਈ ਦਾ filenames\n\
  %n   file name\n\
  %Qn  quoted file name\n\
  %s   ਖੰਡ ਮਾਪ (ਲਈ faster transfers)\n\
  %S   fundamental ਖੰਡ ਮਾਪ (ਲਈ ਖੰਡ counts)\n\
  %t   file system type ਵਿੱਚ hex\n\
  %T   file system type ਵਿੱਚ human readable form\n\
"), stdout);

      ਲਿਖੋ (_("\n\
--terse is equivalent ਮੰਜ਼ਿਲ the following FORMAT:\n\
    %s\
"),
#ਜੇ HAVE_SELINUX_SELINUX_H
              fmt_terse_selinux
#ਨਹੀਂਤਾਂ
              fmt_terse_regular
#ਜੇ_ਅੰਤ
              );

        ਲਿਖੋ (_("\
--terse --file-system is equivalent ਮੰਜ਼ਿਲ the following FORMAT:\n\
    %s\
"), fmt_terse_fs);

      ਲਿਖੋ (USAGE_BUILTIN_WARNING, ਪ੍ਰੋਗਰਾਮ_NAME);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ *argv[])
{
  ਪੂਰਨ c;
  bool fs = false;
  bool terse = false;
  ਅੱਖਰ *format = ਨਿਰਾ;
  ਅੱਖਰ *format2;
  bool ok = true;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  ਬਣਤਰ lconv ਅਚਲ *locale = localeconv ();
  decimal_ਬਿੰਦੂ = (locale->decimal_ਬਿੰਦੂ[0] ? locale->decimal_ਬਿੰਦੂ : ".");
  decimal_ਬਿੰਦੂ_len = ਲੜੀ_ਲੰਬਾਈ (decimal_ਬਿੰਦੂ);

  atexit (close_stdout);

  ਜਦੋਂ ((c = getopt_long (argc, argv, "c:fLt", long_options, ਨਿਰਾ)) != -1)
    {
      ਚੋਣ (c)
        {
        ਹਾਲਤ PRINTF_OPTION:
          format = optarg;
          interpret_backslash_escapes = true;
          trailing_delim = "";
          ਰੋਕੋ;

        ਹਾਲਤ 'c':
          format = optarg;
          interpret_backslash_escapes = false;
          trailing_delim = "\n";
          ਰੋਕੋ;

        ਹਾਲਤ 'L':
          follow_links = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'f':
          fs = true;
          ਰੋਕੋ;

        ਹਾਲਤ 't':
          terse = true;
          ਰੋਕੋ;

        ਹਾਲਤ 0:
          ਚੋਣ (XARGMATCH ("--cached", optarg, cached_ਦਲੀਲਾਂ, cached_modes))
            {
              ਹਾਲਤ cached_never:
                force_sync = true;
                dont_sync = false;
                ਰੋਕੋ;
              ਹਾਲਤ cached_always:
                force_sync = false;
                dont_sync = true;
                ਰੋਕੋ;
              ਹਾਲਤ cached_default:
                force_sync = false;
                dont_sync = false;
            }
          ਰੋਕੋ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);

        ਮੂਲ:
          usage (EXIT_FAILURE);
        }
    }

  ਜੇ (argc == optind)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  ਜੇ (format)
    {
      format2 = format;
    }
  ਨਹੀਂਤਾਂ
    {
      format = default_format (fs, terse, /* device= */ false);
      format2 = default_format (fs, terse, /* device= */ true);
    }

  ਲਈ (ਪੂਰਨ i = optind; i < argc; i++)
    ok &= (fs
           ? do_statfs (argv[i], format)
           : do_stat (argv[i], format, format2));

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
