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
#ಯದಿ ((STAT_STATVFS || STAT_STATVFS64)                                       \
     && (HAVE_STRUCT_STATVFS_F_BASETYPE || HAVE_STRUCT_STATVFS_F_FSTYPENAME \
         || (! HAVE_STRUCT_STATFS_F_FSTYPENAME && HAVE_STRUCT_STATVFS_F_TYPE)))
# define USE_STATVFS 1
#ಅನ್ಯಥಾ
# define USE_STATVFS 0
#endif

#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <pwd.h>
#ಅನ್ತರ್ಭಾವ <grp.h>
#ಯದಿ USE_STATVFS
# include <sys/statvfs.h>
#ನೋಚೇದ್ಯದಿ HAVE_SYS_VFS_H
# include <sys/vfs.h>
#ನೋಚೇದ್ಯದಿ HAVE_SYS_MOUNT_H && HAVE_SYS_PARAM_H
/* NOTE: freebsd5.0 needs sys/param.h and sys/mount.h for statfs.
   It does have statvfs.h, but shouldn't use it, since it doesn't
   HAVE_STRUCT_STATVFS_F_BASETYPE.  So find a clean way to fix it.  */
/* NetBSD 1.5.2 needs these, for the declaration of struct statfs. */
# include <sys/param.h>
# include <sys/mount.h>
# ಯದಿ HAVE_NFS_NFS_CLNT_H && HAVE_NFS_VFS_H
/* Ultrix 4.4 needs these for the declaration of struct statfs.  */
#  include <netinet/in.h>
#  include <nfs/nfs_clnt.h>
#  include <nfs/vfs.h>
# endif
#ನೋಚೇದ್ಯದಿ HAVE_OS_H /* BeOS */
# include <fs_info.h>
#endif
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

#ಯದಿ HAVE_STATX && defined STATX_INO
# define USE_STATX 1
#ಅನ್ಯಥಾ
# define USE_STATX 0
#endif

#ಯದಿ USE_STATVFS
# define STRUCT_STATXFS_F_FSID_IS_INTEGER STRUCT_STATVFS_F_FSID_IS_INTEGER
# define HAVE_STRUCT_STATXFS_F_TYPE HAVE_STRUCT_STATVFS_F_TYPE
# ಯದಿ HAVE_STRUCT_STATVFS_F_NAMEMAX
#  define SB_F_NAMEMAX(S) ((S)->f_namemax)
# endif
# ಯದಿ ! STAT_STATVFS && STAT_STATVFS64
#  define STRUCT_STATVFS ಸಂರಚನಾ statvfs64
#  define STATFS statvfs64
# ಅನ್ಯಥಾ
#  define STRUCT_STATVFS ಸಂರಚನಾ statvfs
#  define STATFS statvfs
# endif
# define STATFS_FRSIZE(S) ((S)->f_frsize)
#ಅನ್ಯಥಾ
# define HAVE_STRUCT_STATXFS_F_TYPE HAVE_STRUCT_STATFS_F_TYPE
# ಯದಿ HAVE_STRUCT_STATFS_F_NAMELEN
#  define SB_F_NAMEMAX(S) ((S)->f_namelen)
# elif HAVE_STRUCT_STATFS_F_NAMEMAX
#  define SB_F_NAMEMAX(S) ((S)->f_namemax)
# endif
# define STATFS statfs
# ಯದಿ HAVE_OS_H /* BeOS */
/* BeOS has a statvfs function, but it does not return sensible values
   for f_files, f_ffree and f_favail, and lacks f_type, f_basetype and
   f_fstypename.  Use 'struct fs_info' instead.  */
NODISCARD
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
statfs (ವರ್ಣ ಧ್ರುವ *filename, ಸಂರಚನಾ fs_info *buf)
{
  dev_t device = dev_for_path (filename);
  ಯದಿ (device < 0)
    {
      errno = (device == B_ENTRY_NOT_FOUND ? ENOENT
               : device == B_BAD_ಮೌಲ್ಯ ? EINVAL
               : device == B_NAME_TOO_LONG ? ENAMETOOLONG
               : device == B_NO_MEMORY ? ENOMEM
               : device == B_FILE_ERROR ? EIO
               : 0);
      ಪ್ರತ್ಯಾಗಮ -1;
    }
  /* If successful, buf->dev will be == device.  */
  ಪ್ರತ್ಯಾಗಮ fs_stat_dev (device, buf);
}
#  define f_fsid dev
#  define f_blocks total_blocks
#  define f_bfree free_blocks
#  define f_bavail free_blocks
#  define f_bsize io_size
#  define f_files total_nodes
#  define f_ffree free_nodes
#  define STRUCT_STATVFS ಸಂರಚನಾ fs_info
#  define STRUCT_STATXFS_F_FSID_IS_INTEGER true
#  define STATFS_FRSIZE(S) ((S)->block_size)
# ಅನ್ಯಥಾ
#  define STRUCT_STATVFS ಸಂರಚನಾ statfs
#  define STRUCT_STATXFS_F_FSID_IS_INTEGER STRUCT_STATFS_F_FSID_IS_INTEGER
#  ಯದಿ HAVE_STRUCT_STATFS_F_FRSIZE
#   define STATFS_FRSIZE(S) ((S)->f_frsize)
#  ಅನ್ಯಥಾ
#   define STATFS_FRSIZE(S) 0
#  endif
# endif
#endif

#ifdef SB_F_NAMEMAX
# define OUT_NAMEMAX out_uint
#ಅನ್ಯಥಾ
/* Depending on whether statvfs or statfs is used,
   neither f_namemax or f_namelen may be available.  */
# define SB_F_NAMEMAX(S) "?"
# define OUT_NAMEMAX out_string
#endif

#ಯದಿ HAVE_STRUCT_STATVFS_F_BASETYPE
# define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME f_basetype
#ಅನ್ಯಥಾ
# ಯದಿ HAVE_STRUCT_STATVFS_F_FSTYPENAME || HAVE_STRUCT_STATFS_F_FSTYPENAME
#  define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME f_fstypename
# elif HAVE_OS_H /* BeOS */
#  define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME fsh_name
# endif
#endif

#ಯದಿ HAVE_GETATTRAT
# include <attr.h>
# include <sys/nvpair.h>
#endif

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ digits[] = "0123456789";

/* Flags that are portable for use in printf, for at least one
   conversion specifier; make_format removes non-portable flags as
   needed for particular specifiers.  The glibc 2.2 extension "I" is
   listed here; it is removed by make_format because it has undefined
   behavior elsewhere and because it is incompatible with
   out_epoch_sec.  */
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ printf_flags[] = "'-+ #0I";

/* Formats for the --terse option.  */
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ fmt_terse_fs[] = "%Qn %i %l %t %s %S %b %f %a %c %d\n";
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ fmt_terse_regular[] = "%Qn %s %b %f %u %g %D %i %h %t %T"
                                        " %X %Y %Z %W %o\n";
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ fmt_terse_selinux[] = "%Qn %s %b %f %u %g %D %i %h %t %T"
                                        " %X %Y %Z %W %o %C\n";

#define PROGRAM_NAME "stat"

#define AUTHORS proper_name ("Michael Meskes")

ಗಣನಾ
{
  PRINTF_OPTION = CHAR_MAX + 1
};

ಗಣನಾ cached_mode
{
  cached_default,
  cached_never,
  cached_always
};

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *ಧ್ರುವ cached_args[] =
{
  "default", "never", "always", ರಿಕ್ತ
};

ಸ್ಥಿರ ಗಣನಾ cached_mode ಧ್ರುವ cached_modes[] =
{
  cached_default, cached_never, cached_always
};

ಸ್ಥಿರ ಸಂರಚನಾ option ಧ್ರುವ long_options[] =
{
  {"dereference", no_argument, ರಿಕ್ತ, 'L'},
  {"file-system", no_argument, ರಿಕ್ತ, 'f'},
  {"format", required_argument, ರಿಕ್ತ, 'c'},
  {"printf", required_argument, ರಿಕ್ತ, PRINTF_OPTION},
  {"terse", no_argument, ರಿಕ್ತ, 't'},
  {"cached", required_argument, ರಿಕ್ತ, 0},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ರಿಕ್ತ, 0, ರಿಕ್ತ, 0}
};

/* Whether to follow symbolic links;  True for --dereference (-L).  */
ಸ್ಥಿರ bool follow_links;

/* Whether to interpret backslash-escape sequences.
   True for --printf=FMT, not for --format=FMT (-c).  */
ಸ್ಥಿರ bool interpret_backslash_escapes;

/* The trailing delimiter string:
   "" for --printf=FMT, "\n" for --format=FMT (-c).  */
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *trailing_delim = "";

/* The representation of the decimal point in the current locale.  */
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *decimal_point;
ಸ್ಥಿರ size_t decimal_point_len;

ಸ್ಥಿರ bool
print_stat (ವರ್ಣ *pformat, size_t prefix_len, ವರ್ಣ mod, ವರ್ಣ m,
            ಪೂರ್ಣಾಙ್ಕ fd, ವರ್ಣ ಧ್ರುವ *filename, ಶೂನ್ಯ ಧ್ರುವ *data);

/* Return the type of the specified file system.
   Some systems have statfvs.f_basetype[FSTYPSZ] (AIX, HP-UX, and Solaris).
   Others have statvfs.f_fstypename[_VFS_NAMELEN] (NetBSD 3.0).
   Others have statfs.f_fstypename[MFSNAMELEN] (NetBSD 1.5.2).
   Still others have neither and have to get by with f_type (GNU/Linux).
   But f_type may only exist in statfs (Cygwin).  */
NODISCARD
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *
human_fstype (STRUCT_STATVFS ಧ್ರುವ *statfsbuf)
{
#ifdef STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME
  ಪ್ರತ್ಯಾಗಮ statfsbuf->STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME;
#ಅನ್ಯಥಾ
  ವಿಕಲ್ಪ (statfsbuf->f_type)
    {
# ಯದಿ defined __linux__ || defined __ANDROID__

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

    ಪಕ್ಷ S_MAGIC_AAFS: /* 0x5A3C69F0 local */
      ಪ್ರತ್ಯಾಗಮ "aafs";
    ಪಕ್ಷ S_MAGIC_ACFS: /* 0x61636673 remote */
      ಪ್ರತ್ಯಾಗಮ "acfs";
    ಪಕ್ಷ S_MAGIC_ADFS: /* 0xADF5 local */
      ಪ್ರತ್ಯಾಗಮ "adfs";
    ಪಕ್ಷ S_MAGIC_AFFS: /* 0xADFF local */
      ಪ್ರತ್ಯಾಗಮ "affs";
    ಪಕ್ಷ S_MAGIC_AFS: /* 0x5346414F remote */
      ಪ್ರತ್ಯಾಗಮ "afs";
    ಪಕ್ಷ S_MAGIC_ANON_INODE_FS: /* 0x09041934 local */
      ಪ್ರತ್ಯಾಗಮ "anon-inode FS";
    ಪಕ್ಷ S_MAGIC_AUFS: /* 0x61756673 remote */
      /* FIXME: change syntax or add an optional attribute like "inotify:no".
         The above is labeled as "remote" so that tail always uses polling,
         but this isn't really a remote file system type.  */
      ಪ್ರತ್ಯಾಗಮ "aufs";
    ಪಕ್ಷ S_MAGIC_AUTOFS: /* 0x0187 local */
      ಪ್ರತ್ಯಾಗಮ "autofs";
    ಪಕ್ಷ S_MAGIC_BALLOON_KVM: /* 0x13661366 local */
      ಪ್ರತ್ಯಾಗಮ "balloon-kvm-fs";
    ಪಕ್ಷ S_MAGIC_BCACHEFS: /* 0xCA451A4E local */
      ಪ್ರತ್ಯಾಗಮ "bcachefs";
    ಪಕ್ಷ S_MAGIC_BEFS: /* 0x42465331 local */
      ಪ್ರತ್ಯಾಗಮ "befs";
    ಪಕ್ಷ S_MAGIC_BDEVFS: /* 0x62646576 local */
      ಪ್ರತ್ಯಾಗಮ "bdevfs";
    ಪಕ್ಷ S_MAGIC_BFS: /* 0x1BADFACE local */
      ಪ್ರತ್ಯಾಗಮ "bfs";
    ಪಕ್ಷ S_MAGIC_BINDERFS: /* 0x6C6F6F70 local */
      ಪ್ರತ್ಯಾಗಮ "binderfs";
    ಪಕ್ಷ S_MAGIC_BPF_FS: /* 0xCAFE4A11 local */
      ಪ್ರತ್ಯಾಗಮ "bpf_fs";
    ಪಕ್ಷ S_MAGIC_BINFMTFS: /* 0x42494E4D local */
      ಪ್ರತ್ಯಾಗಮ "binfmt_misc";
    ಪಕ್ಷ S_MAGIC_BTRFS: /* 0x9123683E local */
      ಪ್ರತ್ಯಾಗಮ "btrfs";
    ಪಕ್ಷ S_MAGIC_BTRFS_TEST: /* 0x73727279 local */
      ಪ್ರತ್ಯಾಗಮ "btrfs_test";
    ಪಕ್ಷ S_MAGIC_CEPH: /* 0x00C36400 remote */
      ಪ್ರತ್ಯಾಗಮ "ceph";
    ಪಕ್ಷ S_MAGIC_CGROUP: /* 0x0027E0EB local */
      ಪ್ರತ್ಯಾಗಮ "cgroupfs";
    ಪಕ್ಷ S_MAGIC_CGROUP2: /* 0x63677270 local */
      ಪ್ರತ್ಯಾಗಮ "cgroup2fs";
    ಪಕ್ಷ S_MAGIC_CIFS: /* 0xFF534D42 remote */
      ಪ್ರತ್ಯಾಗಮ "cifs";
    ಪಕ್ಷ S_MAGIC_CODA: /* 0x73757245 remote */
      ಪ್ರತ್ಯಾಗಮ "coda";
    ಪಕ್ಷ S_MAGIC_COH: /* 0x012FF7B7 local */
      ಪ್ರತ್ಯಾಗಮ "coh";
    ಪಕ್ಷ S_MAGIC_CONFIGFS: /* 0x62656570 local */
      ಪ್ರತ್ಯಾಗಮ "configfs";
    ಪಕ್ಷ S_MAGIC_CRAMFS: /* 0x28CD3D45 local */
      ಪ್ರತ್ಯಾಗಮ "cramfs";
    ಪಕ್ಷ S_MAGIC_CRAMFS_WEND: /* 0x453DCD28 local */
      ಪ್ರತ್ಯಾಗಮ "cramfs-wend";
    ಪಕ್ಷ S_MAGIC_DAXFS: /* 0x64646178 local */
      ಪ್ರತ್ಯಾಗಮ "daxfs";
    ಪಕ್ಷ S_MAGIC_DEBUGFS: /* 0x64626720 local */
      ಪ್ರತ್ಯಾಗಮ "debugfs";
    ಪಕ್ಷ S_MAGIC_DEVFS: /* 0x1373 local */
      ಪ್ರತ್ಯಾಗಮ "devfs";
    ಪಕ್ಷ S_MAGIC_DEVMEM: /* 0x454D444D local */
      ಪ್ರತ್ಯಾಗಮ "devmem";
    ಪಕ್ಷ S_MAGIC_DEVPTS: /* 0x1CD1 local */
      ಪ್ರತ್ಯಾಗಮ "devpts";
    ಪಕ್ಷ S_MAGIC_DMA_BUF: /* 0x444D4142 local */
      ಪ್ರತ್ಯಾಗಮ "dma-buf-fs";
    ಪಕ್ಷ S_MAGIC_ECRYPTFS: /* 0xF15F local */
      ಪ್ರತ್ಯಾಗಮ "ecryptfs";
    ಪಕ್ಷ S_MAGIC_EFIVARFS: /* 0xDE5E81E4 local */
      ಪ್ರತ್ಯಾಗಮ "efivarfs";
    ಪಕ್ಷ S_MAGIC_EFS: /* 0x00414A53 local */
      ಪ್ರತ್ಯಾಗಮ "efs";
    ಪಕ್ಷ S_MAGIC_EROFS_V1: /* 0xE0F5E1E2 local */
      ಪ್ರತ್ಯಾಗಮ "erofs";
    ಪಕ್ಷ S_MAGIC_EXFAT: /* 0x2011BAB0 local */
      ಪ್ರತ್ಯಾಗಮ "exfat";
    ಪಕ್ಷ S_MAGIC_EXFS: /* 0x45584653 local */
      ಪ್ರತ್ಯಾಗಮ "exfs";
    ಪಕ್ಷ S_MAGIC_EXOFS: /* 0x5DF5 local */
      ಪ್ರತ್ಯಾಗಮ "exofs";
    ಪಕ್ಷ S_MAGIC_EXT: /* 0x137D local */
      ಪ್ರತ್ಯಾಗಮ "ext";
    ಪಕ್ಷ S_MAGIC_EXT2: /* 0xEF53 local */
      ಪ್ರತ್ಯಾಗಮ "ext2/ext3";
    ಪಕ್ಷ S_MAGIC_EXT2_OLD: /* 0xEF51 local */
      ಪ್ರತ್ಯಾಗಮ "ext2";
    ಪಕ್ಷ S_MAGIC_F2FS: /* 0xF2F52010 local */
      ಪ್ರತ್ಯಾಗಮ "f2fs";
    ಪಕ್ಷ S_MAGIC_FAIL_FS: /* 0x4641494C local */
      ಪ್ರತ್ಯಾಗಮ "failfs";
    ಪಕ್ಷ S_MAGIC_FAT: /* 0x4006 local */
      ಪ್ರತ್ಯಾಗಮ "fat";
    ಪಕ್ಷ S_MAGIC_FHGFS: /* 0x19830326 remote */
      ಪ್ರತ್ಯಾಗಮ "fhgfs";
    ಪಕ್ಷ S_MAGIC_FUSE: /* 0x65735546 remote */
      ಪ್ರತ್ಯಾಗಮ "fuse";
    ಪಕ್ಷ S_MAGIC_FUSECTL: /* 0x65735543 remote */
      ಪ್ರತ್ಯಾಗಮ "fusectl";
    ಪಕ್ಷ S_MAGIC_FUTEXFS: /* 0x0BAD1DEA local */
      ಪ್ರತ್ಯಾಗಮ "futexfs";
    ಪಕ್ಷ S_MAGIC_GFS: /* 0x01161970 remote */
      ಪ್ರತ್ಯಾಗಮ "gfs/gfs2";
    ಪಕ್ಷ S_MAGIC_GPFS: /* 0x47504653 remote */
      ಪ್ರತ್ಯಾಗಮ "gpfs";
    ಪಕ್ಷ S_MAGIC_GUEST_MEMFD: /* 0x474D454D remote */
      ಪ್ರತ್ಯಾಗಮ "guest-memfd";
    ಪಕ್ಷ S_MAGIC_HFS: /* 0x4244 local */
      ಪ್ರತ್ಯಾಗಮ "hfs";
    ಪಕ್ಷ S_MAGIC_HFS_PLUS: /* 0x482B local */
      ಪ್ರತ್ಯಾಗಮ "hfs+";
    ಪಕ್ಷ S_MAGIC_HFS_X: /* 0x4858 local */
      ಪ್ರತ್ಯಾಗಮ "hfsx";
    ಪಕ್ಷ S_MAGIC_HOSTFS: /* 0x00C0FFEE local */
      ಪ್ರತ್ಯಾಗಮ "hostfs";
    ಪಕ್ಷ S_MAGIC_HPFS: /* 0xF995E849 local */
      ಪ್ರತ್ಯಾಗಮ "hpfs";
    ಪಕ್ಷ S_MAGIC_HUGETLBFS: /* 0x958458F6 local */
      ಪ್ರತ್ಯಾಗಮ "hugetlbfs";
    ಪಕ್ಷ S_MAGIC_MTD_INODE_FS: /* 0x11307854 local */
      ಪ್ರತ್ಯಾಗಮ "inodefs";
    ಪಕ್ಷ S_MAGIC_IBRIX: /* 0x013111A8 remote */
      ಪ್ರತ್ಯಾಗಮ "ibrix";
    ಪಕ್ಷ S_MAGIC_INOTIFYFS: /* 0x2BAD1DEA local */
      ಪ್ರತ್ಯಾಗಮ "inotifyfs";
    ಪಕ್ಷ S_MAGIC_ISOFS: /* 0x9660 local */
      ಪ್ರತ್ಯಾಗಮ "isofs";
    ಪಕ್ಷ S_MAGIC_ISOFS_R_WIN: /* 0x4004 local */
      ಪ್ರತ್ಯಾಗಮ "isofs";
    ಪಕ್ಷ S_MAGIC_ISOFS_WIN: /* 0x4000 local */
      ಪ್ರತ್ಯಾಗಮ "isofs";
    ಪಕ್ಷ S_MAGIC_JFFS: /* 0x07C0 local */
      ಪ್ರತ್ಯಾಗಮ "jffs";
    ಪಕ್ಷ S_MAGIC_JFFS2: /* 0x72B6 local */
      ಪ್ರತ್ಯಾಗಮ "jffs2";
    ಪಕ್ಷ S_MAGIC_JFS: /* 0x3153464A local */
      ಪ್ರತ್ಯಾಗಮ "jfs";
    ಪಕ್ಷ S_MAGIC_KAFS: /* 0x6B414653 remote */
      ಪ್ರತ್ಯಾಗಮ "k-afs";
    ಪಕ್ಷ S_MAGIC_LOGFS: /* 0xC97E8168 local */
      ಪ್ರತ್ಯಾಗಮ "logfs";
    ಪಕ್ಷ S_MAGIC_LUSTRE: /* 0x0BD00BD0 remote */
      ಪ್ರತ್ಯಾಗಮ "lustre";
    ಪಕ್ಷ S_MAGIC_M1FS: /* 0x5346314D local */
      ಪ್ರತ್ಯಾಗಮ "m1fs";
    ಪಕ್ಷ S_MAGIC_MINIX: /* 0x137F local */
      ಪ್ರತ್ಯಾಗಮ "minix";
    ಪಕ್ಷ S_MAGIC_MINIX_30: /* 0x138F local */
      ಪ್ರತ್ಯಾಗಮ "minix (30 char.)";
    ಪಕ್ಷ S_MAGIC_MINIX_V2: /* 0x2468 local */
      ಪ್ರತ್ಯಾಗಮ "minix v2";
    ಪಕ್ಷ S_MAGIC_MINIX_V2_30: /* 0x2478 local */
      ಪ್ರತ್ಯಾಗಮ "minix v2 (30 char.)";
    ಪಕ್ಷ S_MAGIC_MINIX_V3: /* 0x4D5A local */
      ಪ್ರತ್ಯಾಗಮ "minix3";
    ಪಕ್ಷ S_MAGIC_MQUEUE: /* 0x19800202 local */
      ಪ್ರತ್ಯಾಗಮ "mqueue";
    ಪಕ್ಷ S_MAGIC_MSDOS: /* 0x4D44 local */
      ಪ್ರತ್ಯಾಗಮ "msdos";
    ಪಕ್ಷ S_MAGIC_NCP: /* 0x564C remote */
      ಪ್ರತ್ಯಾಗಮ "novell";
    ಪಕ್ಷ S_MAGIC_NFS: /* 0x6969 remote */
      ಪ್ರತ್ಯಾಗಮ "nfs";
    ಪಕ್ಷ S_MAGIC_NFSD: /* 0x6E667364 remote */
      ಪ್ರತ್ಯಾಗಮ "nfsd";
    ಪಕ್ಷ S_MAGIC_NILFS: /* 0x3434 local */
      ಪ್ರತ್ಯಾಗಮ "nilfs";
    ಪಕ್ಷ S_MAGIC_NSFS: /* 0x6E736673 local */
      ಪ್ರತ್ಯಾಗಮ "nsfs";
    ಪಕ್ಷ S_MAGIC_NTFS: /* 0x5346544E local */
      ಪ್ರತ್ಯಾಗಮ "ntfs";
    ಪಕ್ಷ S_MAGIC_NULL_FS: /* 0x4E554C4C local */
      ಪ್ರತ್ಯಾಗಮ "nullfs";
    ಪಕ್ಷ S_MAGIC_OPENPROM: /* 0x9FA1 local */
      ಪ್ರತ್ಯಾಗಮ "openprom";
    ಪಕ್ಷ S_MAGIC_OCFS2: /* 0x7461636F remote */
      ಪ್ರತ್ಯಾಗಮ "ocfs2";
    ಪಕ್ಷ S_MAGIC_OVERLAYFS: /* 0x794C7630 remote */
      /* This may overlay remote file systems.
         Also there have been issues reported with inotify and overlayfs,
         so mark as "remote" so that polling is used.  */
      ಪ್ರತ್ಯಾಗಮ "overlayfs";
    ಪಕ್ಷ S_MAGIC_PANFS: /* 0xAAD7AAEA remote */
      ಪ್ರತ್ಯಾಗಮ "panfs";
    ಪಕ್ಷ S_MAGIC_PID_FS: /* 0x50494446 local */
      ಪ್ರತ್ಯಾಗಮ "pidfs";
    ಪಕ್ಷ S_MAGIC_PIPEFS: /* 0x50495045 remote */
      /* FIXME: change syntax or add an optional attribute like "inotify:no".
         pipefs is labeled as "remote" so that tail always polls,
         but it isn't really remote file system type.  */
      ಪ್ರತ್ಯಾಗಮ "pipefs";
    ಪಕ್ಷ S_MAGIC_PPC_CMM: /* 0xC7571590 local */
      ಪ್ರತ್ಯಾಗಮ "ppc-cmm-fs";
    ಪಕ್ಷ S_MAGIC_PRL_FS: /* 0x7C7C6673 remote */
      ಪ್ರತ್ಯಾಗಮ "prl_fs";
    ಪಕ್ಷ S_MAGIC_PROC: /* 0x9FA0 local */
      ಪ್ರತ್ಯಾಗಮ "proc";
    ಪಕ್ಷ S_MAGIC_PSTOREFS: /* 0x6165676C local */
      ಪ್ರತ್ಯಾಗಮ "pstorefs";
    ಪಕ್ಷ S_MAGIC_QNX4: /* 0x002F local */
      ಪ್ರತ್ಯಾಗಮ "qnx4";
    ಪಕ್ಷ S_MAGIC_QNX6: /* 0x68191122 local */
      ಪ್ರತ್ಯಾಗಮ "qnx6";
    ಪಕ್ಷ S_MAGIC_RAMFS: /* 0x858458F6 local */
      ಪ್ರತ್ಯಾಗಮ "ramfs";
    ಪಕ್ಷ S_MAGIC_RDTGROUP: /* 0x07655821 local */
      ಪ್ರತ್ಯಾಗಮ "rdt";
    ಪಕ್ಷ S_MAGIC_REISERFS: /* 0x52654973 local */
      ಪ್ರತ್ಯಾಗಮ "reiserfs";
    ಪಕ್ಷ S_MAGIC_ROMFS: /* 0x7275 local */
      ಪ್ರತ್ಯಾಗಮ "romfs";
    ಪಕ್ಷ S_MAGIC_RPC_PIPEFS: /* 0x67596969 local */
      ಪ್ರತ್ಯಾಗಮ "rpc_pipefs";
    ಪಕ್ಷ S_MAGIC_SDCARDFS: /* 0x5DCA2DF5 local */
      ಪ್ರತ್ಯಾಗಮ "sdcardfs";
    ಪಕ್ಷ S_MAGIC_SECRETMEM: /* 0x5345434D local */
      ಪ್ರತ್ಯಾಗಮ "secretmem";
    ಪಕ್ಷ S_MAGIC_SECURITYFS: /* 0x73636673 local */
      ಪ್ರತ್ಯಾಗಮ "securityfs";
    ಪಕ್ಷ S_MAGIC_SELINUX: /* 0xF97CFF8C local */
      ಪ್ರತ್ಯಾಗಮ "selinux";
    ಪಕ್ಷ S_MAGIC_SMACK: /* 0x43415D53 local */
      ಪ್ರತ್ಯಾಗಮ "smackfs";
    ಪಕ್ಷ S_MAGIC_SMB: /* 0x517B remote */
      ಪ್ರತ್ಯಾಗಮ "smb";
    ಪಕ್ಷ S_MAGIC_SMB2: /* 0xFE534D42 remote */
      ಪ್ರತ್ಯಾಗಮ "smb2";
    ಪಕ್ಷ S_MAGIC_SNFS: /* 0xBEEFDEAD remote */
      ಪ್ರತ್ಯಾಗಮ "snfs";
    ಪಕ್ಷ S_MAGIC_SOCKFS: /* 0x534F434B local */
      ಪ್ರತ್ಯಾಗಮ "sockfs";
    ಪಕ್ಷ S_MAGIC_SQUASHFS: /* 0x73717368 local */
      ಪ್ರತ್ಯಾಗಮ "squashfs";
    ಪಕ್ಷ S_MAGIC_SYSFS: /* 0x62656572 local */
      ಪ್ರತ್ಯಾಗಮ "sysfs";
    ಪಕ್ಷ S_MAGIC_SYSV2: /* 0x012FF7B6 local */
      ಪ್ರತ್ಯಾಗಮ "sysv2";
    ಪಕ್ಷ S_MAGIC_SYSV4: /* 0x012FF7B5 local */
      ಪ್ರತ್ಯಾಗಮ "sysv4";
    ಪಕ್ಷ S_MAGIC_TMPFS: /* 0x01021994 local */
      ಪ್ರತ್ಯಾಗಮ "tmpfs";
    ಪಕ್ಷ S_MAGIC_TRACEFS: /* 0x74726163 local */
      ಪ್ರತ್ಯಾಗಮ "tracefs";
    ಪಕ್ಷ S_MAGIC_UBIFS: /* 0x24051905 local */
      ಪ್ರತ್ಯಾಗಮ "ubifs";
    ಪಕ್ಷ S_MAGIC_UDF: /* 0x15013346 local */
      ಪ್ರತ್ಯಾಗಮ "udf";
    ಪಕ್ಷ S_MAGIC_UFS: /* 0x00011954 local */
      ಪ್ರತ್ಯಾಗಮ "ufs";
    ಪಕ್ಷ S_MAGIC_UFS_BYTESWAPPED: /* 0x54190100 local */
      ಪ್ರತ್ಯಾಗಮ "ufs";
    ಪಕ್ಷ S_MAGIC_USBDEVFS: /* 0x9FA2 local */
      ಪ್ರತ್ಯಾಗಮ "usbdevfs";
    ಪಕ್ಷ S_MAGIC_V9FS: /* 0x01021997 local */
      ಪ್ರತ್ಯಾಗಮ "v9fs";
    ಪಕ್ಷ S_MAGIC_VBOXSF: /* 0x786F4256 remote */
      ಪ್ರತ್ಯಾಗಮ "vboxsf";
    ಪಕ್ಷ S_MAGIC_VMHGFS: /* 0xBACBACBC remote */
      ಪ್ರತ್ಯಾಗಮ "vmhgfs";
    ಪಕ್ಷ S_MAGIC_VXFS: /* 0xA501FCF5 remote */
      /* Veritas File System can run in single instance or clustered mode,
         so mark as remote to cater for the latter case.  */
      ಪ್ರತ್ಯಾಗಮ "vxfs";
    ಪಕ್ಷ S_MAGIC_VZFS: /* 0x565A4653 local */
      ಪ್ರತ್ಯಾಗಮ "vzfs";
    ಪಕ್ಷ S_MAGIC_WSLFS: /* 0x53464846 local */
      ಪ್ರತ್ಯಾಗಮ "wslfs";
    ಪಕ್ಷ S_MAGIC_XENFS: /* 0xABBA1974 local */
      ಪ್ರತ್ಯಾಗಮ "xenfs";
    ಪಕ್ಷ S_MAGIC_XENIX: /* 0x012FF7B4 local */
      ಪ್ರತ್ಯಾಗಮ "xenix";
    ಪಕ್ಷ S_MAGIC_XFS: /* 0x58465342 local */
      ಪ್ರತ್ಯಾಗಮ "xfs";
    ಪಕ್ಷ S_MAGIC_XIAFS: /* 0x012FD16D local */
      ಪ್ರತ್ಯಾಗಮ "xia";
    ಪಕ್ಷ S_MAGIC_Z3FOLD: /* 0x0033 local */
      ಪ್ರತ್ಯಾಗಮ "z3fold";
    ಪಕ್ಷ S_MAGIC_ZFS: /* 0x2FC12FC1 local */
      ಪ್ರತ್ಯಾಗಮ "zfs";
    ಪಕ್ಷ S_MAGIC_ZONEFS: /* 0x5A4F4653 local */
      ಪ್ರತ್ಯಾಗಮ "zonefs";
    ಪಕ್ಷ S_MAGIC_ZSMALLOC: /* 0x58295829 local */
      ಪ್ರತ್ಯಾಗಮ "zsmallocfs";


# elif __GNU__
    ಪಕ್ಷ FSTYPE_UFS:
      ಪ್ರತ್ಯಾಗಮ "ufs";
    ಪಕ್ಷ FSTYPE_NFS:
      ಪ್ರತ್ಯಾಗಮ "nfs";
    ಪಕ್ಷ FSTYPE_GFS:
      ಪ್ರತ್ಯಾಗಮ "gfs";
    ಪಕ್ಷ FSTYPE_LFS:
      ಪ್ರತ್ಯಾಗಮ "lfs";
    ಪಕ್ಷ FSTYPE_SYSV:
      ಪ್ರತ್ಯಾಗಮ "sysv";
    ಪಕ್ಷ FSTYPE_FTP:
      ಪ್ರತ್ಯಾಗಮ "ftp";
    ಪಕ್ಷ FSTYPE_TAR:
      ಪ್ರತ್ಯಾಗಮ "tar";
    ಪಕ್ಷ FSTYPE_AR:
      ಪ್ರತ್ಯಾಗಮ "ar";
    ಪಕ್ಷ FSTYPE_CPIO:
      ಪ್ರತ್ಯಾಗಮ "cpio";
    ಪಕ್ಷ FSTYPE_MSLOSS:
      ಪ್ರತ್ಯಾಗಮ "msloss";
    ಪಕ್ಷ FSTYPE_CPM:
      ಪ್ರತ್ಯಾಗಮ "cpm";
    ಪಕ್ಷ FSTYPE_HFS:
      ಪ್ರತ್ಯಾಗಮ "hfs";
    ಪಕ್ಷ FSTYPE_DTFS:
      ಪ್ರತ್ಯಾಗಮ "dtfs";
    ಪಕ್ಷ FSTYPE_GRFS:
      ಪ್ರತ್ಯಾಗಮ "grfs";
    ಪಕ್ಷ FSTYPE_TERM:
      ಪ್ರತ್ಯಾಗಮ "term";
    ಪಕ್ಷ FSTYPE_DEV:
      ಪ್ರತ್ಯಾಗಮ "dev";
    ಪಕ್ಷ FSTYPE_PROC:
      ಪ್ರತ್ಯಾಗಮ "proc";
    ಪಕ್ಷ FSTYPE_IFSOCK:
      ಪ್ರತ್ಯಾಗಮ "ifsock";
    ಪಕ್ಷ FSTYPE_AFS:
      ಪ್ರತ್ಯಾಗಮ "afs";
    ಪಕ್ಷ FSTYPE_DFS:
      ಪ್ರತ್ಯಾಗಮ "dfs";
    ಪಕ್ಷ FSTYPE_PROC9:
      ಪ್ರತ್ಯಾಗಮ "proc9";
    ಪಕ್ಷ FSTYPE_SOCKET:
      ಪ್ರತ್ಯಾಗಮ "socket";
    ಪಕ್ಷ FSTYPE_MISC:
      ಪ್ರತ್ಯಾಗಮ "misc";
    ಪಕ್ಷ FSTYPE_EXT2FS:
      ಪ್ರತ್ಯಾಗಮ "ext2/ext3";
    ಪಕ್ಷ FSTYPE_HTTP:
      ಪ್ರತ್ಯಾಗಮ "http";
    ಪಕ್ಷ FSTYPE_MEMFS:
      ಪ್ರತ್ಯಾಗಮ "memfs";
    ಪಕ್ಷ FSTYPE_ISO9660:
      ಪ್ರತ್ಯಾಗಮ "iso9660";
# endif
    ಸ್ವಭಾವ:
      {
        unsigned long ಪೂರ್ಣಾಙ್ಕ type = statfsbuf->f_type;
        ಸ್ಥಿರ ವರ್ಣ buf[ಪರಿಮಾಣ "UNKNOWN (0x%lx)" - 3
                        + (ಪರಿಮಾಣ type * CHAR_BIT + 3) / 4];
        sprintf (buf, "UNKNOWN (0x%lx)", type);
        ಪ್ರತ್ಯಾಗಮ buf;
      }
    }
#endif
}

NODISCARD
ಸ್ಥಿರ ವರ್ಣ *
human_access (ಸಂರಚನಾ stat ಧ್ರುವ *statbuf)
{
  ಸ್ಥಿರ ವರ್ಣ modebuf[12];
  filemodestring (statbuf, modebuf);
  modebuf[10] = 0;
  ಪ್ರತ್ಯಾಗಮ modebuf;
}

NODISCARD
ಸ್ಥಿರ ವರ್ಣ *
human_time (ಸಂರಚನಾ timespec t)
{
  /* STR must be at least INT_BUFSIZE_BOUND (intmax_t) big, either
     because localtime_rz fails, or because the time zone is truly
     outlandish so that %z expands to a long string.  */
  ಸ್ಥಿರ ವರ್ಣ str[INT_BUFSIZE_BOUND (intmax_t)
                  + INT_STRLEN_BOUND (ಪೂರ್ಣಾಙ್ಕ) /* YYYY */
                  + 1 /* because YYYY might equal INT_MAX + 1900 */
                  + ಪರಿಮಾಣ "-MM-DD HH:MM:SS.NNNNNNNNN +"];
  ಸ್ಥಿರ timezone_t tz;
  ಯದಿ (!tz)
    tz = tzalloc (getenv ("TZ"));
  ಸಂರಚನಾ tm tm;
  ಪೂರ್ಣಾಙ್ಕ ns = t.tv_nsec;
  ಯದಿ (localtime_rz (tz, &t.tv_sec, &tm))
    nstrftime (str, ಪರಿಮಾಣ str, "%Y-%m-%d %H:%M:%S.%N %z", &tm, tz, ns);
  ಅನ್ಯಥಾ
    {
      ವರ್ಣ secbuf[INT_BUFSIZE_BOUND (intmax_t)];
      sprintf (str, "%s.%09d", timetostr (t.tv_sec, secbuf), ns);
    }
  ಪ್ರತ್ಯಾಗಮ str;
}

/* PFORMAT points to a '%' followed by a prefix of a format, all of
   size PREFIX_LEN.  The flags allowed for this format are
   ALLOWED_FLAGS; remove other printf flags from the prefix, then
   append SUFFIX.  */
ಸ್ಥಿರ ಶೂನ್ಯ
make_format (ವರ್ಣ *pformat, size_t prefix_len, ವರ್ಣ ಧ್ರುವ *allowed_flags,
             ವರ್ಣ ಧ್ರುವ *suffix)
{
  ವರ್ಣ *dst = pformat + 1;
  ವರ್ಣ ಧ್ರುವ *src;
  ವರ್ಣ ಧ್ರುವ *srclim = pformat + prefix_len;
  ಪ್ರತಿ (src = dst; src < srclim && strchr (printf_flags, *src); src++)
    ಯದಿ (strchr (allowed_flags, *src))
      *dst++ = *src;
  ಯಾವತ್ (src < srclim)
    *dst++ = *src++;
  strcpy (dst, suffix);
}

ಸ್ಥಿರ ಶೂನ್ಯ
out_string (ವರ್ಣ *pformat, size_t prefix_len, ವರ್ಣ ಧ್ರುವ *arg)
{
  make_format (pformat, prefix_len, "-", "s");
  printf (pformat, arg);
}
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
out_int (ವರ್ಣ *pformat, size_t prefix_len, intmax_t arg)
{
  make_format (pformat, prefix_len, "'-+ 0", "jd");
  ಪ್ರತ್ಯಾಗಮ printf (pformat, arg);
}
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
out_uint (ವರ್ಣ *pformat, size_t prefix_len, uintmax_t arg)
{
  make_format (pformat, prefix_len, "'-0", "ju");
  ಪ್ರತ್ಯಾಗಮ printf (pformat, arg);
}
ಸ್ಥಿರ ಶೂನ್ಯ
out_uint_o (ವರ್ಣ *pformat, size_t prefix_len, uintmax_t arg)
{
  make_format (pformat, prefix_len, "-#0", "jo");
  printf (pformat, arg);
}
ಸ್ಥಿರ ಶೂನ್ಯ
out_uint_x (ವರ್ಣ *pformat, size_t prefix_len, uintmax_t arg)
{
  make_format (pformat, prefix_len, "-#0", "jx");
  printf (pformat, arg);
}
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
out_minus_zero (ವರ್ಣ *pformat, size_t prefix_len)
{
  make_format (pformat, prefix_len, "'-+ 0", ".0f");
  ಪ್ರತ್ಯಾಗಮ printf (pformat, -0.25);
}

/* Output the number of seconds since the Epoch, using a format that
   acts like printf's %f format.  */
ಸ್ಥಿರ ಶೂನ್ಯ
out_epoch_sec (ವರ್ಣ *pformat, size_t prefix_len,
               ಸಂರಚನಾ timespec arg)
{
  ವರ್ಣ *dot = memchr (pformat, '.', prefix_len);
  size_t sec_prefix_len = prefix_len;
  ಪೂರ್ಣಾಙ್ಕ width = 0;
  ಪೂರ್ಣಾಙ್ಕ precision = 0;
  bool frac_ಎಡ_adjust = false;

  ಯದಿ (dot)
    {
      sec_prefix_len = dot - pformat;
      pformat[prefix_len] = '\0';

      ಯದಿ (c_isdigit (dot[1]))
        {
          long ಪೂರ್ಣಾಙ್ಕ lprec = strtol (dot + 1, ರಿಕ್ತ, 10);
          precision = (lprec <= INT_MAX ? lprec : INT_MAX);
        }
      ಅನ್ಯಥಾ
        {
          precision = 9;
        }

      ಯದಿ (precision && c_isdigit (dot[-1]))
        {
          /* If a nontrivial width is given, subtract the width of the
             decimal point and PRECISION digits that will be output
             later.  */
          ವರ್ಣ *p = dot;
          *dot = '\0';

          ಕುರು
            --p;
          ಯಾವತ್ (c_isdigit (p[-1]));

          long ಪೂರ್ಣಾಙ್ಕ lwidth = strtol (p, ರಿಕ್ತ, 10);
          width = (lwidth <= INT_MAX ? lwidth : INT_MAX);
          ಯದಿ (1 < width)
            {
              p += (*p == '0');
              sec_prefix_len = p - pformat;
              ಪೂರ್ಣಾಙ್ಕ w_d = (decimal_point_len < width
                         ? width - decimal_point_len
                         : 0);
              ಯದಿ (1 < w_d)
                {
                  ಪೂರ್ಣಾಙ್ಕ w = w_d - precision;
                  ಯದಿ (1 < w)
                    {
                      ವರ್ಣ *dst = pformat;
                      ಪ್ರತಿ (ವರ್ಣ ಧ್ರುವ *src = dst; src < p; src++)
                        {
                          ಯದಿ (*src == '-')
                            frac_ಎಡ_adjust = true;
                          ಅನ್ಯಥಾ
                            *dst++ = *src;
                        }
                      sec_prefix_len =
                        (dst - pformat
                         + (frac_ಎಡ_adjust ? 0 : sprintf (dst, "%d", w)));
                    }
                }
            }
        }
    }

  ಪೂರ್ಣಾಙ್ಕ ಭಾಜಕ = 1;
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = precision; i < 9; i++)
    ಭಾಜಕ *= 10;
  ಪೂರ್ಣಾಙ್ಕ frac_sec = arg.tv_nsec / ಭಾಜಕ;
  ಪೂರ್ಣಾಙ್ಕ int_len;

  ಯದಿ (TYPE_SIGNED (time_t))
    {
      bool minus_zero = false;
      ಯದಿ (arg.tv_sec < 0 && arg.tv_nsec != 0)
        {
          ಪೂರ್ಣಾಙ್ಕ frac_sec_modulus = 1000000000 / ಭಾಜಕ;
          frac_sec = (frac_sec_modulus - frac_sec
                      - (arg.tv_nsec % ಭಾಜಕ != 0));
          arg.tv_sec += (frac_sec != 0);
          minus_zero = (arg.tv_sec == 0);
        }
      int_len = (minus_zero
                 ? out_minus_zero (pformat, sec_prefix_len)
                 : out_int (pformat, sec_prefix_len, arg.tv_sec));
    }
  ಅನ್ಯಥಾ
    int_len = out_uint (pformat, sec_prefix_len, arg.tv_sec);

  ಯದಿ (precision)
    {
      ಪೂರ್ಣಾಙ್ಕ prec = (precision < 9 ? precision : 9);
      ಪೂರ್ಣಾಙ್ಕ trailing_prec = precision - prec;
      ಪೂರ್ಣಾಙ್ಕ ilen = (int_len < 0 ? 0 : int_len);
      ಪೂರ್ಣಾಙ್ಕ trailing_width = (ilen < width && decimal_point_len < width - ilen
                            ? width - ilen - decimal_point_len - prec
                            : 0);
      printf ("%s%.*d%-*.*d", decimal_point, prec, frac_sec,
              trailing_width, trailing_prec, 0);
    }
}

/* Print the context information of FILENAME, and return true iff the
   context could not be obtained.  */
NODISCARD
ಸ್ಥಿರ bool
out_file_context (ವರ್ಣ *pformat, size_t prefix_len, ವರ್ಣ ಧ್ರುವ *filename)
{
  ವರ್ಣ *scontext;
  bool fail = false;

  ಯದಿ ((follow_links
       ? getfilecon (filename, &scontext)
       : lgetfilecon (filename, &scontext)) < 0)
    {
      error (0, errno, _("failed to get security context of %s"),
             quoteaf (filename));
      scontext = ರಿಕ್ತ;
      fail = true;
    }
  strcpy (pformat + prefix_len, "s");
  printf (pformat, (scontext ? scontext : "?"));
  ಯದಿ (scontext)
    freecon (scontext);
  ಪ್ರತ್ಯಾಗಮ fail;
}

/* Set the quoting style once it is needed.  */
ಸ್ಥಿರ ಶೂನ್ಯ
initialize_quoting_style (ಶೂನ್ಯ)
{
  ಸ್ಥಿರ bool initialized;
  ಯದಿ (!initialized)
    {
      initialized = true;
      ಪೂರ್ಣಾಙ್ಕ qs = getenv_quoting_style ();
      set_quoting_style (ರಿಕ್ತ, 0 <= qs ? qs : shell_escape_quoting_style);
    }
}

/* Print statfs info.  Return zero upon success, nonzero upon failure.  */
NODISCARD
ಸ್ಥಿರ bool
print_statfs (ವರ್ಣ *pformat, size_t prefix_len, MAYBE_UNUSED ವರ್ಣ mod, ವರ್ಣ m,
              MAYBE_UNUSED ಪೂರ್ಣಾಙ್ಕ fd, ವರ್ಣ ಧ್ರುವ *filename,
              ಶೂನ್ಯ ಧ್ರುವ *data)
{
  STRUCT_STATVFS ಧ್ರುವ *statfsbuf = data;
  bool fail = false;

  ವಿಕಲ್ಪ (m)
    {
    ಪಕ್ಷ 'n':
      ಯದಿ (mod == 'Q')
        {
          initialize_quoting_style ();
          filename = quoteN (filename);
        }
      out_string (pformat, prefix_len, filename);
      ಭಙ್ಗ;

    ಪಕ್ಷ 'i':
      {
#ಯದಿ STRUCT_STATXFS_F_FSID_IS_INTEGER
        uintmax_t fsid = statfsbuf->f_fsid;
#ಅನ್ಯಥಾ
        ಪ್ರಕಾರನಿರ್ದೇಶ unsigned ಪೂರ್ಣಾಙ್ಕ fsid_word;
        static_assert (alignof (STRUCT_STATVFS) % alignof (fsid_word) == 0);
        static_assert (offsetof (STRUCT_STATVFS, f_fsid) % alignof (fsid_word)
                       == 0);
        static_assert (ಪರಿಮಾಣ statfsbuf->f_fsid % alignof (fsid_word) == 0);
        fsid_word ಧ್ರುವ *p = (fsid_word *) &statfsbuf->f_fsid;

        /* Assume a little-endian word order, as that is compatible
           with glibc's statvfs implementation.  */
        uintmax_t fsid = 0;
        ಪೂರ್ಣಾಙ್ಕ ಪದಗಳು = ಪರಿಮಾಣ statfsbuf->f_fsid / ಪರಿಮಾಣ *p;
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಪದಗಳು && i * ಪರಿಮಾಣ *p < ಪರಿಮಾಣ fsid; i++)
          {
            uintmax_t u = p[ಪದಗಳು - 1 - i];
            fsid |= u << (i * CHAR_BIT * ಪರಿಮಾಣ *p);
          }
#endif
        out_uint_x (pformat, prefix_len, fsid);
      }
      ಭಙ್ಗ;

    ಪಕ್ಷ 'l':
      OUT_NAMEMAX (pformat, prefix_len, SB_F_NAMEMAX (statfsbuf));
      ಭಙ್ಗ;
    ಪಕ್ಷ 't':
#ಯದಿ HAVE_STRUCT_STATXFS_F_TYPE
      out_uint_x (pformat, prefix_len, statfsbuf->f_type);
#ಅನ್ಯಥಾ
      fputc ('?', stdout);
#endif
      ಭಙ್ಗ;
    ಪಕ್ಷ 'T':
      out_string (pformat, prefix_len, human_fstype (statfsbuf));
      ಭಙ್ಗ;
    ಪಕ್ಷ 'b':
      out_int (pformat, prefix_len, statfsbuf->f_blocks);
      ಭಙ್ಗ;
    ಪಕ್ಷ 'f':
      out_int (pformat, prefix_len, statfsbuf->f_bfree);
      ಭಙ್ಗ;
    ಪಕ್ಷ 'a':
      out_int (pformat, prefix_len, statfsbuf->f_bavail);
      ಭಙ್ಗ;
    ಪಕ್ಷ 's':
      out_uint (pformat, prefix_len, statfsbuf->f_bsize);
      ಭಙ್ಗ;
    ಪಕ್ಷ 'S':
      {
        uintmax_t frsize = STATFS_FRSIZE (statfsbuf);
        ಯದಿ (! frsize)
          frsize = statfsbuf->f_bsize;
        out_uint (pformat, prefix_len, frsize);
      }
      ಭಙ್ಗ;
    ಪಕ್ಷ 'c':
      out_uint (pformat, prefix_len, statfsbuf->f_files);
      ಭಙ್ಗ;
    ಪಕ್ಷ 'd':
      out_int (pformat, prefix_len, statfsbuf->f_ffree);
      ಭಙ್ಗ;
    ಸ್ವಭಾವ:
      fputc ('?', stdout);
      ಭಙ್ಗ;
    }
  ಪ್ರತ್ಯಾಗಮ fail;
}

/* Return any bind mounted source for a path.
   The caller should not free the returned buffer.
   Return NULL if no bind mount found.  */
NODISCARD
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *
find_bind_mount (ವರ್ಣ ಧ್ರುವ * name)
{
  ವರ್ಣ ಧ್ರುವ * bind_mount = ರಿಕ್ತ;

  ಸ್ಥಿರ ಸಂರಚನಾ mount_entry *mount_list;
  ಸ್ಥಿರ bool tried_mount_list = false;
  ಯದಿ (!tried_mount_list) /* attempt/warn once per process.  */
    {
      ಯದಿ (!(mount_list = read_file_system_list (false)))
        error (0, errno, "%s", _("cannot read table of mounted file systems"));
      tried_mount_list = true;
    }

  ಸಂರಚನಾ stat name_stats;
  ಯದಿ (stat (name, &name_stats) != 0)
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

  ಪ್ರತಿ (ಸಂರಚನಾ mount_entry *me = mount_list; me; me = me->me_next)
    {
      ಯದಿ (me->me_dummy && me->me_devname[0] == '/'
          && streq (me->me_mountdir, name))
        {
          ಸಂರಚನಾ stat dev_stats;

          ಯದಿ (stat (me->me_devname, &dev_stats) == 0
              && psame_inode (&name_stats, &dev_stats))
            {
              bind_mount = me->me_devname;
              ಭಙ್ಗ;
            }
        }
    }

  ಪ್ರತ್ಯಾಗಮ bind_mount;
}

/* Print mount point.  Return zero upon success, nonzero upon failure.  */
NODISCARD
ಸ್ಥಿರ bool
out_mount_point (ವರ್ಣ ಧ್ರುವ *filename, ವರ್ಣ *pformat, size_t prefix_len,
                 ಧ್ರುವ ಸಂರಚನಾ stat *statp)
{

  ವರ್ಣ ಧ್ರುವ *np = "?", *bp = ರಿಕ್ತ;
  ವರ್ಣ *mp = ರಿಕ್ತ;
  bool fail = true;

  /* Look for bind mounts first.  Note we output the immediate alias,
     rather than further resolving to a base device mount point.  */
  ಯದಿ (follow_links || !S_ISLNK (statp->st_mode))
    {
      ವರ್ಣ *resolved = canonicalize_file_name (filename);
      ಯದಿ (!resolved)
        {
          error (0, errno, _("failed to canonicalize %s"), quoteaf (filename));
          ಗಚ್ಛ print_mount_point;
        }
      bp = find_bind_mount (resolved);
      ಮುಕ್ತ (resolved);
      ಯದಿ (bp)
        {
          fail = false;
          ಗಚ್ಛ print_mount_point;
        }
    }

  /* If there is no direct bind mount, then navigate
     back up the tree looking for a device change.
     Note we don't detect if any of the directory components
     are bind mounted to the same device, but that's OK
     since we've not directly queried them.  */
  ಯದಿ ((mp = find_mount_point (filename, statp)))
    {
      /* This dir might be bind mounted to another device,
         so we resolve the bound source in that case also.  */
      bp = find_bind_mount (mp);
      fail = false;
    }

print_mount_point:

  out_string (pformat, prefix_len, bp ? bp : mp ? mp : np);
  ಮುಕ್ತ (mp);
  ಪ್ರತ್ಯಾಗಮ fail;
}

/* Map a TS with negative TS.tv_nsec to {0,0}.  */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ timespec
neg_to_zero (ಸಂರಚನಾ timespec ts)
{
  ಯದಿ (0 <= ts.tv_nsec)
    ಪ್ರತ್ಯಾಗಮ ts;
  ಸಂರಚನಾ timespec z = {0};
  ಪ್ರತ್ಯಾಗಮ z;
}

/* Output a single-character \ escape.  */

ಸ್ಥಿರ ಶೂನ್ಯ
print_esc_char (ವರ್ಣ c)
{
  ವಿಕಲ್ಪ (c)
    {
    ಪಕ್ಷ 'a':			/* Alert. */
      c ='\a';
      ಭಙ್ಗ;
    ಪಕ್ಷ 'b':			/* Backspace. */
      c ='\b';
      ಭಙ್ಗ;
    ಪಕ್ಷ 'e':			/* Escape. */
      c ='\x1B';
      ಭಙ್ಗ;
    ಪಕ್ಷ 'f':			/* Form feed. */
      c ='\f';
      ಭಙ್ಗ;
    ಪಕ್ಷ 'n':			/* New line. */
      c ='\n';
      ಭಙ್ಗ;
    ಪಕ್ಷ 'r':			/* Carriage return. */
      c ='\r';
      ಭಙ್ಗ;
    ಪಕ್ಷ 't':			/* Horizontal tab. */
      c ='\t';
      ಭಙ್ಗ;
    ಪಕ್ಷ 'v':			/* Vertical tab. */
      c ='\v';
      ಭಙ್ಗ;
    ಪಕ್ಷ '"':
    ಪಕ್ಷ '\\':
      ಭಙ್ಗ;
    ಸ್ವಭಾವ:
      error (0, 0, _("warning: unrecognized escape '\\%c'"), c);
      ಭಙ್ಗ;
    }
  putchar (c);
}

ATTRIBUTE_PURE
ಸ್ಥಿರ size_t
format_code_offset (ವರ್ಣ ಧ್ರುವ *directive)
{
  size_t len = strspn (directive + 1, printf_flags);
  ವರ್ಣ ಧ್ರುವ *fmt_char = directive + len + 1;
  fmt_char += strspn (fmt_char, digits);
  ಯದಿ (*fmt_char == '.')
    fmt_char += 1 + strspn (fmt_char + 1, digits);
  ಪ್ರತ್ಯಾಗಮ fmt_char - directive;
}

/* Print the information specified by the format string, FORMAT,
   calling PRINT_FUNC for each %-directive encountered.
   Return zero upon success, nonzero upon failure.  */
NODISCARD
ಸ್ಥಿರ bool
print_it (ವರ್ಣ ಧ್ರುವ *format, ಪೂರ್ಣಾಙ್ಕ fd, ವರ್ಣ ಧ್ರುವ *filename,
          bool (*print_func) (ವರ್ಣ *, size_t, ವರ್ಣ, ವರ್ಣ,
                              ಪೂರ್ಣಾಙ್ಕ, ವರ್ಣ ಧ್ರುವ *, ಶೂನ್ಯ ಧ್ರುವ *),
          ಶೂನ್ಯ ಧ್ರುವ *data)
{
  bool fail = false;

  /* Add 2 to accommodate our conversion of the stat '%s' format string
     to the longer printf '%llu' one.  */
  ಗಣನಾ
    {
      MAX_ADDITIONAL_BYTES =
        (MAX (ಪರಿಮಾಣ "jd",
              MAX (ಪರಿಮಾಣ "jo", MAX (ಪರಿಮಾಣ "ju", ಪರಿಮಾಣ "jx")))
         - 1)
    };
  size_t n_alloc = strlen (format) + MAX_ADDITIONAL_BYTES + 1;
  ವರ್ಣ *dest = xmalloc (n_alloc);
  ಪ್ರತಿ (ವರ್ಣ ಧ್ರುವ *b = format; *b; b++)
    {
      ವಿಕಲ್ಪ (*b)
        {
        ಪಕ್ಷ '%':
          {
            size_t len = format_code_offset (b);
            ವರ್ಣ fmt_char = *(b + len);
            ವರ್ಣ mod_char = 0;
            memcpy (dest, b, len);
            b += len;

            ವಿಕಲ್ಪ (fmt_char)
              {
              ಪಕ್ಷ '\0':
                --b;
                FALLTHROUGH;
              ಪಕ್ಷ '%':
                ಯದಿ (1 < len)
                  {
                    dest[len] = fmt_char;
                    dest[len + 1] = '\0';
                    error (EXIT_FAILURE, 0, _("%s: invalid directive"),
                           quote (dest));
                  }
                putchar ('%');
                ಭಙ್ಗ;
              ಪಕ್ಷ 'H':
              ಪಕ್ಷ 'L':
              ಪಕ್ಷ 'Q':
                mod_char = fmt_char;
                fmt_char = *(b + 1);
                ಯದಿ ((mod_char == 'Q' && fmt_char == 'n')
                    || (print_func == print_stat
                        && (mod_char == 'H' || mod_char == 'L')
                        && (fmt_char == 'd' || fmt_char == 'r')))
                  {
                    b++;
                  }
                ಅನ್ಯಥಾ
                  {
                    fmt_char = mod_char;
                    mod_char = 0;
                  }
                FALLTHROUGH;
              ಸ್ವಭಾವ:
                fail |= print_func (dest, len, mod_char, fmt_char,
                                    fd, filename, data);
                ಭಙ್ಗ;
              }
            ಭಙ್ಗ;
          }

        ಪಕ್ಷ '\\':
          ಯದಿ ( ! interpret_backslash_escapes)
            {
              putchar ('\\');
              ಭಙ್ಗ;
            }
          ++b;
          ಯದಿ (isoct (*b))
            {
              ಪೂರ್ಣಾಙ್ಕ esc_ಮೌಲ್ಯ = fromoct (*b);
              ಪೂರ್ಣಾಙ್ಕ esc_length = 1;	/* number of octal digits */
              ಪ್ರತಿ (++b; esc_length < 3 && isoct (*b);
                   ++esc_length, ++b)
                {
                  esc_ಮೌಲ್ಯ = esc_ಮೌಲ್ಯ * 8 + fromoct (*b);
                }
              putchar (esc_ಮೌಲ್ಯ);
              --b;
            }
          ಅನ್ಯಥಾ ಯದಿ (*b == 'x' && c_isxdigit (b[1]))
            {
              ಪೂರ್ಣಾಙ್ಕ esc_ಮೌಲ್ಯ = fromhex (b[1]);	/* Value of \xhh escape. */
              /* A hexadecimal \xhh escape sequence must have
                 1 or 2 hex. digits.  */
              ++b;
              ಯದಿ (c_isxdigit (b[1]))
                {
                  ++b;
                  esc_ಮೌಲ್ಯ = esc_ಮೌಲ್ಯ * 16 + fromhex (*b);
                }
              putchar (esc_ಮೌಲ್ಯ);
            }
          ಅನ್ಯಥಾ ಯದಿ (*b == '\0')
            {
              error (0, 0, _("warning: backslash at end of format"));
              putchar ('\\');
              /* Arrange to exit the loop.  */
              --b;
            }
          ಅನ್ಯಥಾ
            {
              print_esc_char (*b);
            }
          ಭಙ್ಗ;

        ಸ್ವಭಾವ:
          putchar (*b);
          ಭಙ್ಗ;
        }
    }
  ಮುಕ್ತ (dest);

  fputs (trailing_delim, stdout);

  ಪ್ರತ್ಯಾಗಮ fail;
}

/* Stat the file system and print what we find.  */
NODISCARD
ಸ್ಥಿರ bool
do_statfs (ವರ್ಣ ಧ್ರುವ *filename, ವರ್ಣ ಧ್ರುವ *format)
{
  STRUCT_STATVFS statfsbuf;

  ಯದಿ (streq (filename, "-"))
    {
      error (0, 0, _("using %s to denote standard input does not work"
                     " in file system mode"), quoteaf (filename));
      ಪ್ರತ್ಯಾಗಮ false;
    }

  ಯದಿ (STATFS (filename, &statfsbuf) != 0)
    {
      error (0, errno, _("cannot read file system information for %s"),
             quoteaf (filename));
      ಪ್ರತ್ಯಾಗಮ false;
    }

  bool fail = print_it (format, -1, filename, print_statfs, &statfsbuf);
  ಪ್ರತ್ಯಾಗಮ ! fail;
}

ಸಂರಚನಾ print_args {
  ಸಂರಚನಾ stat *st;
  ಸಂರಚನಾ timespec btime;
};

/* Ask statx to avoid syncing? */
ಸ್ಥಿರ bool dont_sync;

/* Ask statx to force sync? */
ಸ್ಥಿರ bool force_sync;

#ಯದಿ USE_STATX
ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ
fmt_to_mask (ವರ್ಣ fmt)
{
  ವಿಕಲ್ಪ (fmt)
    {
    ಪಕ್ಷ 'N':
      ಪ್ರತ್ಯಾಗಮ STATX_MODE;
    ಪಕ್ಷ 'd':
    ಪಕ್ಷ 'D':
      ಪ್ರತ್ಯಾಗಮ STATX_MODE;
    ಪಕ್ಷ 'i':
      ಪ್ರತ್ಯಾಗಮ STATX_INO;
    ಪಕ್ಷ 'a':
    ಪಕ್ಷ 'A':
      ಪ್ರತ್ಯಾಗಮ STATX_MODE;
    ಪಕ್ಷ 'f':
      ಪ್ರತ್ಯಾಗಮ STATX_MODE|STATX_TYPE;
    ಪಕ್ಷ 'F':
      ಪ್ರತ್ಯಾಗಮ STATX_TYPE;
    ಪಕ್ಷ 'h':
      ಪ್ರತ್ಯಾಗಮ STATX_NLINK;
    ಪಕ್ಷ 'u':
    ಪಕ್ಷ 'U':
      ಪ್ರತ್ಯಾಗಮ STATX_UID;
    ಪಕ್ಷ 'g':
    ಪಕ್ಷ 'G':
      ಪ್ರತ್ಯಾಗಮ STATX_GID;
    ಪಕ್ಷ 'm':
      ಪ್ರತ್ಯಾಗಮ STATX_MODE|STATX_INO;
    ಪಕ್ಷ 's':
      ಪ್ರತ್ಯಾಗಮ STATX_SIZE;
    ಪಕ್ಷ 't':
    ಪಕ್ಷ 'T':
      ಪ್ರತ್ಯಾಗಮ STATX_MODE;
    ಪಕ್ಷ 'b':
      ಪ್ರತ್ಯಾಗಮ STATX_BLOCKS;
    ಪಕ್ಷ 'w':
    ಪಕ್ಷ 'W':
      ಪ್ರತ್ಯಾಗಮ STATX_BTIME;
    ಪಕ್ಷ 'x':
    ಪಕ್ಷ 'X':
      ಪ್ರತ್ಯಾಗಮ STATX_ATIME;
    ಪಕ್ಷ 'y':
    ಪಕ್ಷ 'Y':
      ಪ್ರತ್ಯಾಗಮ STATX_MTIME;
    ಪಕ್ಷ 'z':
    ಪಕ್ಷ 'Z':
      ಪ್ರತ್ಯಾಗಮ STATX_CTIME;
    }
  ಪ್ರತ್ಯಾಗಮ 0;
}

ATTRIBUTE_PURE
ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ
format_to_mask (ವರ್ಣ ಧ್ರುವ *format)
{
  unsigned ಪೂರ್ಣಾಙ್ಕ mask = 0;

  ಪ್ರತಿ (ವರ್ಣ ಧ್ರುವ *b = format; *b; b++)
    {
      ಯದಿ (*b != '%')
        ಅನುವರ್ತ;

      b += format_code_offset (b);
      ಯದಿ (*b == '\0')
        ಭಙ್ಗ;
      mask |= fmt_to_mask (*b);
    }
  ಪ್ರತ್ಯಾಗಮ mask;
}

/* statx the file and print what we find */
NODISCARD
ಸ್ಥಿರ bool
do_stat (ವರ್ಣ ಧ್ರುವ *filename, ವರ್ಣ ಧ್ರುವ *format, ವರ್ಣ ಧ್ರುವ *format2)
{
  ಪೂರ್ಣಾಙ್ಕ fd = streq (filename, "-") ? 0 : AT_FDCWD;
  ಪೂರ್ಣಾಙ್ಕ flags = 0;
  ಸಂರಚನಾ stat st;
  ಸಂರಚನಾ statx stx = {0};
  ವರ್ಣ ಧ್ರುವ *pathname = filename;
  ಸಂರಚನಾ print_args pa;
  pa.st = &st;
  pa.btime = (ಸಂರಚನಾ timespec) {.tv_sec = -1, .tv_nsec = -1};

  ಯದಿ (AT_FDCWD != fd)
    {
      pathname = "";
      flags = AT_EMPTY_PATH;
    }
  ಅನ್ಯಥಾ ಯದಿ (!follow_links)
    {
      flags = AT_SYMLINK_NOFOLLOW;
    }

  ಯದಿ (dont_sync)
    flags |= AT_STATX_DONT_SYNC;
  ಅನ್ಯಥಾ ಯದಿ (force_sync)
    flags |= AT_STATX_FORCE_SYNC;

  ಯದಿ (! force_sync)
    flags |= AT_NO_AUTOMOUNT;

  fd = statx (fd, pathname, flags, format_to_mask (format), &stx);
  ಯದಿ (fd < 0)
    {
      ಯದಿ (flags & AT_EMPTY_PATH)
        error (0, errno, _("cannot stat standard input"));
      ಅನ್ಯಥಾ
        error (0, errno, _("cannot statx %s"), quoteaf (filename));
      ಪ್ರತ್ಯಾಗಮ false;
    }

  ಯದಿ (S_ISBLK (stx.stx_mode) || S_ISCHR (stx.stx_mode))
    format = format2;

  statx_to_stat (&stx, &st);
  ಯದಿ (stx.stx_mask & STATX_BTIME)
    pa.btime = statx_timestamp_to_timespec (stx.stx_btime);

  bool fail = print_it (format, fd, filename, print_stat, &pa);
  ಪ್ರತ್ಯಾಗಮ ! fail;
}

#ಅನ್ಯಥಾ /* USE_STATX */

ಸ್ಥಿರ ಸಂರಚನಾ timespec
get_birthtime (ಪೂರ್ಣಾಙ್ಕ fd, ವರ್ಣ ಧ್ರುವ *filename, ಸಂರಚನಾ stat ಧ್ರುವ *st)
{
  ಸಂರಚನಾ timespec ts = get_stat_birthtime (st);

# ಯದಿ HAVE_GETATTRAT
  ಯದಿ (ts.tv_nsec < 0)
    {
      nvlist_t *response;
      ಯದಿ ((fd < 0
           ? getattrat (AT_FDCWD, XATTR_VIEW_READWRITE, filename, &response)
           : fgetattr (fd, XATTR_VIEW_READWRITE, &response))
          == 0)
        {
          uint64_t *val;
          uint_t n;
          ಯದಿ (nvlist_lookup_uint64_array (response, A_CRTIME, &val, &n) == 0
              && 2 <= n
              && val[0] <= TYPE_MAXIMUM (time_t)
              && val[1] < 1000000000 * 2 /* for leap seconds */)
            {
              ts.tv_sec = val[0];
              ts.tv_nsec = val[1];
            }
          nvlist_free (response);
        }
    }
# endif

  ಪ್ರತ್ಯಾಗಮ ts;
}


/* stat the file and print what we find */
NODISCARD
ಸ್ಥಿರ bool
do_stat (ವರ್ಣ ಧ್ರುವ *filename, ವರ್ಣ ಧ್ರುವ *format,
         ವರ್ಣ ಧ್ರುವ *format2)
{
  ಪೂರ್ಣಾಙ್ಕ fd = streq (filename, "-") ? 0 : -1;
  ಸಂರಚನಾ stat statbuf;
  ಸಂರಚನಾ print_args pa;
  pa.st = &statbuf;
  pa.btime = (ಸಂರಚನಾ timespec) {.tv_sec = -1, .tv_nsec = -1};

  ಯದಿ (0 <= fd)
    {
      ಯದಿ (fstat (fd, &statbuf) != 0)
        {
          error (0, errno, _("cannot stat standard input"));
          ಪ್ರತ್ಯಾಗಮ false;
        }
    }
  /* We can't use the shorter
     (follow_links?stat:lstat) (filename, &statbug)
     since stat might be a function-like macro.  */
  ಅನ್ಯಥಾ ಯದಿ ((follow_links
            ? stat (filename, &statbuf)
            : lstat (filename, &statbuf)) != 0)
    {
      error (0, errno, _("cannot stat %s"), quoteaf (filename));
      ಪ್ರತ್ಯಾಗಮ false;
    }

  ಯದಿ (S_ISBLK (statbuf.st_mode) || S_ISCHR (statbuf.st_mode))
    format = format2;

  bool fail = print_it (format, fd, filename, print_stat, &pa);
  ಪ್ರತ್ಯಾಗಮ ! fail;
}
#endif /* USE_STATX */

/* POSIX requires 'ls' to print file sizes without a sign, even
   when negative.  Be consistent with that.  */

ಸ್ಥಿರ uintmax_t
unsigned_file_size (off_t size)
{
  ಪ್ರತ್ಯಾಗಮ size + (size < 0) * ((uintmax_t) OFF_T_MAX - OFF_T_MIN + 1);
}

/* Print stat info.  Return zero upon success, nonzero upon failure.  */
ಸ್ಥಿರ bool
print_stat (ವರ್ಣ *pformat, size_t prefix_len, ವರ್ಣ mod, ವರ್ಣ m,
            MAYBE_UNUSED ಪೂರ್ಣಾಙ್ಕ fd, ವರ್ಣ ಧ್ರುವ *filename, ಶೂನ್ಯ ಧ್ರುವ *data)
{
  ಸಂರಚನಾ print_args *parg = (ಸಂರಚನಾ print_args *) data;
  ಸಂರಚನಾ stat *statbuf = parg->st;
  ಸಂರಚನಾ timespec btime = parg->btime;
  ಸಂರಚನಾ passwd *pw_ent;
  ಸಂರಚನಾ group *gw_ent;
  bool fail = false;

  ವಿಕಲ್ಪ (m)
    {
    ಪಕ್ಷ 'n':
      ಯದಿ (mod == 'Q')
        {
          initialize_quoting_style ();
          filename = quoteN (filename);
        }
      out_string (pformat, prefix_len, filename);
      ಭಙ್ಗ;
    ಪಕ್ಷ 'N':
      initialize_quoting_style ();
      out_string (pformat, prefix_len, quoteN (filename));
      ಯದಿ (S_ISLNK (statbuf->st_mode))
        {
          ವರ್ಣ *linkname = areadlink_with_size (filename, statbuf->st_size);
          ಯದಿ (linkname == ರಿಕ್ತ)
            {
              error (0, errno, _("cannot read symbolic link %s"),
                     quoteaf (filename));
              ಪ್ರತ್ಯಾಗಮ true;
            }
          printf (" -> ");
          out_string (pformat, prefix_len, quoteN (linkname));
          ಮುಕ್ತ (linkname);
        }
      ಭಙ್ಗ;
    ಪಕ್ಷ 'd':
      ಯದಿ (mod == 'H')
        out_uint (pformat, prefix_len, major (statbuf->st_dev));
      ಅನ್ಯಥಾ ಯದಿ (mod == 'L')
        out_uint (pformat, prefix_len, minor (statbuf->st_dev));
      ಅನ್ಯಥಾ
        out_uint (pformat, prefix_len, statbuf->st_dev);
      ಭಙ್ಗ;
    ಪಕ್ಷ 'D':
      out_uint_x (pformat, prefix_len, statbuf->st_dev);
      ಭಙ್ಗ;
    ಪಕ್ಷ 'i':
      out_uint (pformat, prefix_len, statbuf->st_ino);
      ಭಙ್ಗ;
    ಪಕ್ಷ 'a':
      out_uint_o (pformat, prefix_len, statbuf->st_mode & CHMOD_MODE_BITS);
      ಭಙ್ಗ;
    ಪಕ್ಷ 'A':
      out_string (pformat, prefix_len, human_access (statbuf));
      ಭಙ್ಗ;
    ಪಕ್ಷ 'f':
      out_uint_x (pformat, prefix_len, statbuf->st_mode);
      ಭಙ್ಗ;
    ಪಕ್ಷ 'F':
      out_string (pformat, prefix_len, file_type (statbuf));
      ಭಙ್ಗ;
    ಪಕ್ಷ 'h':
      out_uint (pformat, prefix_len, statbuf->st_nlink);
      ಭಙ್ಗ;
    ಪಕ್ಷ 'u':
      out_uint (pformat, prefix_len, statbuf->st_uid);
      ಭಙ್ಗ;
    ಪಕ್ಷ 'U':
      pw_ent = getpwuid (statbuf->st_uid);
      out_string (pformat, prefix_len,
                  pw_ent ? pw_ent->pw_name : "UNKNOWN");
      ಭಙ್ಗ;
    ಪಕ್ಷ 'g':
      out_uint (pformat, prefix_len, statbuf->st_gid);
      ಭಙ್ಗ;
    ಪಕ್ಷ 'G':
      gw_ent = getgrgid (statbuf->st_gid);
      out_string (pformat, prefix_len,
                  gw_ent ? gw_ent->gr_name : "UNKNOWN");
      ಭಙ್ಗ;
    ಪಕ್ಷ 'm':
      fail |= out_mount_point (filename, pformat, prefix_len, statbuf);
      ಭಙ್ಗ;
    ಪಕ್ಷ 's':
      out_uint (pformat, prefix_len, unsigned_file_size (statbuf->st_size));
      ಭಙ್ಗ;
    ಪಕ್ಷ 'r':
      ಯದಿ (mod == 'H')
        out_uint (pformat, prefix_len, major (statbuf->st_rdev));
      ಅನ್ಯಥಾ ಯದಿ (mod == 'L')
        out_uint (pformat, prefix_len, minor (statbuf->st_rdev));
      ಅನ್ಯಥಾ
        out_uint (pformat, prefix_len, statbuf->st_rdev);
      ಭಙ್ಗ;
    ಪಕ್ಷ 'R':
      out_uint_x (pformat, prefix_len, statbuf->st_rdev);
      ಭಙ್ಗ;
    ಪಕ್ಷ 't':
      out_uint_x (pformat, prefix_len, major (statbuf->st_rdev));
      ಭಙ್ಗ;
    ಪಕ್ಷ 'T':
      out_uint_x (pformat, prefix_len, minor (statbuf->st_rdev));
      ಭಙ್ಗ;
    ಪಕ್ಷ 'B':
      out_uint (pformat, prefix_len, ST_NBLOCKSIZE);
      ಭಙ್ಗ;
    ಪಕ್ಷ 'b':
      out_uint (pformat, prefix_len, STP_NBLOCKS (statbuf));
      ಭಙ್ಗ;
    ಪಕ್ಷ 'o':
      out_uint (pformat, prefix_len, STP_BLKSIZE (statbuf));
      ಭಙ್ಗ;
    ಪಕ್ಷ 'w':
      {
#ಯದಿ ! USE_STATX
        btime = get_birthtime (fd, filename, statbuf);
#endif
        ಯದಿ (btime.tv_nsec < 0)
          out_string (pformat, prefix_len, "-");
        ಅನ್ಯಥಾ
          out_string (pformat, prefix_len, human_time (btime));
      }
      ಭಙ್ಗ;
    ಪಕ್ಷ 'W':
      {
#ಯದಿ ! USE_STATX
        btime = get_birthtime (fd, filename, statbuf);
#endif
        out_epoch_sec (pformat, prefix_len, neg_to_zero (btime));
      }
      ಭಙ್ಗ;
    ಪಕ್ಷ 'x':
      out_string (pformat, prefix_len, human_time (get_stat_atime (statbuf)));
      ಭಙ್ಗ;
    ಪಕ್ಷ 'X':
      out_epoch_sec (pformat, prefix_len, get_stat_atime (statbuf));
      ಭಙ್ಗ;
    ಪಕ್ಷ 'y':
      out_string (pformat, prefix_len, human_time (get_stat_mtime (statbuf)));
      ಭಙ್ಗ;
    ಪಕ್ಷ 'Y':
      out_epoch_sec (pformat, prefix_len, get_stat_mtime (statbuf));
      ಭಙ್ಗ;
    ಪಕ್ಷ 'z':
      out_string (pformat, prefix_len, human_time (get_stat_ctime (statbuf)));
      ಭಙ್ಗ;
    ಪಕ್ಷ 'Z':
      out_epoch_sec (pformat, prefix_len, get_stat_ctime (statbuf));
      ಭಙ್ಗ;
    ಪಕ್ಷ 'C':
      fail |= out_file_context (pformat, prefix_len, filename);
      ಭಙ್ಗ;
    ಸ್ವಭಾವ:
      fputc ('?', stdout);
      ಭಙ್ಗ;
    }
  ಪ್ರತ್ಯಾಗಮ fail;
}

/* Return an allocated format string in static storage that
   corresponds to whether FS and TERSE options were declared.  */
ಸ್ಥಿರ ವರ್ಣ *
default_format (bool fs, bool terse, bool device)
{
  ವರ್ಣ *format;
  ಯದಿ (fs)
    {
      ಯದಿ (terse)
        format = xstrdup (fmt_terse_fs);
      ಅನ್ಯಥಾ
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
  ಅನ್ಯಥಾ /* ! fs */
    {
      ಯದಿ (terse)
        {
          ಯದಿ (0 < is_selinux_enabled ())
            format = xstrdup (fmt_terse_selinux);
          ಅನ್ಯಥಾ
            format = xstrdup (fmt_terse_regular);
        }
      ಅನ್ಯಥಾ
        {
          ವರ್ಣ *temp;
          /* TRANSLATORS: This string uses format specifiers from
             'stat --help' without --file-system, and NOT from printf.  */
          format = xstrdup (_("\
  File: %N\n\
  Size: %-10s\tBlocks: %-10b IO Block: %-6o %F\n\
"));

          temp = format;
          ಯದಿ (device)
            {
              /* TRANSLATORS: This string uses format specifiers from
                 'stat --help' without --file-system, and NOT from printf.  */
              format = xasprintf ("%s%s", format, _("\
" "Device: %Hd,%Ld\tInode: %-10i  Links: %-5h Device type: %Hr,%Lr\n\
"));
            }
          ಅನ್ಯಥಾ
            {
              /* TRANSLATORS: This string uses format specifiers from
                 'stat --help' without --file-system, and NOT from printf.  */
              format = xasprintf ("%s%s", format, _("\
" "Device: %Hd,%Ld\tInode: %-10i  Links: %h\n\
"));
            }
          ಮುಕ್ತ (temp);

          temp = format;
          /* TRANSLATORS: This string uses format specifiers from
             'stat --help' without --file-system, and NOT from printf.  */
          format = xasprintf ("%s%s", format, _("\
" "Access: (%04a/%10.10A)  Uid: (%5u/%8U)   Gid: (%5g/%8G)\n\
"));
          ಮುಕ್ತ (temp);

          ಯದಿ (0 < is_selinux_enabled ())
            {
              temp = format;
              /* TRANSLATORS: This string uses format specifiers from
                 'stat --help' without --file-system, and NOT from printf.  */
              format = xasprintf ("%s%s", format, _("Context: %C\n"));
              ಮುಕ್ತ (temp);
            }

          temp = format;
          format = xasprintf ("%s%s", format,
                              /* TRANSLATORS: This string uses format specifiers
                                 from 'stat --help' without --file-system, and
                                 NOT from printf.  */
                              _("Access: %x\n"
                                "Modify: %y\n"
                                "Change: %z\n"
                                " Birth: %w\n"));
          ಮುಕ್ತ (temp);
        }
    }
  ಪ್ರತ್ಯಾಗಮ format;
}

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
    {
      printf (_("Usage: %s [OPTION]... FILE...\n"), program_name);
      fputs (_("\
Display file or file system status.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -L, --dereference\n\
         follow links\n\
"));
      oputs (_("\
  -f, --file-system\n\
         display file system status instead of file status\n\
"));
      oputs (_("\
      --cached=MODE\n\
         specify how to use cached attributes;\n\
         useful on remote file systems. See MODE below\n\
"));
      oputs (_("\
  -c, --format=FORMAT\n\
         use the specified FORMAT instead of the ಸ್ವಭಾವ;\n\
         output a newline after each use of FORMAT\n\
"));
      oputs (_("\
      --printf=FORMAT\n\
         like --format, but interpret backslash escapes,\n\
         and ಕುರು not output a mandatory trailing newline;\n\
         ಯದಿ you want a newline, include \\n in FORMAT\n\
"));
      oputs (_("\
  -t, --terse\n\
         print the information in terse form\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);

      fputs (_("\n\
The MODE argument of --cached can be: always, never, or ಸ್ವಭಾವ.\n\
'always' will use cached attributes ಯದಿ available, ಯಾವತ್\n\
'never' will try to synchronize with the latest attributes, and\n\
'default' will leave it up to the underlying file system.\n\
"), stdout);

      fputs (_("\n\
The valid format sequences ಪ್ರತಿ files (without --file-system):\n\
\n\
  %a   permission bits in octal (see '#' and '0' printf flags)\n\
  %A   permission bits and file type in human readable form\n\
  %b   ಸಂಖ್ಯೆ of blocks allocated (see %B)\n\
  %B   the size in bytes of each block reported by %b\n\
  %C   SELinux security context string\n\
"), stdout);
      fputs (_("\
  %d   device ಸಂಖ್ಯೆ in decimal (st_dev)\n\
  %D   device ಸಂಖ್ಯೆ in hex (st_dev)\n\
  %Hd  major device ಸಂಖ್ಯೆ in decimal\n\
  %Ld  minor device ಸಂಖ್ಯೆ in decimal\n\
  %f   raw mode in hex\n\
  %F   file type\n\
  %g   group ID of owner\n\
  %G   group name of owner\n\
"), stdout);
      fputs (_("\
  %h   ಸಂಖ್ಯೆ of hard links\n\
  %i   inode ಸಂಖ್ಯೆ\n\
  %m   mount point\n\
  %n   file name\n\
  %Qn  quoted file name\n\
  %N   quoted file name with dereference ಯದಿ symbolic link\n\
  %o   optimal I/O transfer size hint\n\
  %s   total size, in bytes\n\
  %r   device type in decimal (st_rdev)\n\
  %R   device type in hex (st_rdev)\n\
  %Hr  major device type in decimal, ಪ್ರತಿ character/block device special files\n\
  %Lr  minor device type in decimal, ಪ್ರತಿ character/block device special files\n\
  %t   major device type in hex, ಪ್ರತಿ character/block device special files\n\
  %T   minor device type in hex, ಪ್ರತಿ character/block device special files\n\
"), stdout);
      fputs (_("\
  %u   user ID of owner\n\
  %U   user name of owner\n\
  %w   time of file birth, human-readable; - ಯದಿ unknown\n\
  %W   time of file birth, seconds since Epoch; 0 ಯದಿ unknown\n\
  %x   time of last access, human-readable\n\
  %X   time of last access, seconds since Epoch\n\
  %y   time of last data modification, human-readable\n\
  %Y   time of last data modification, seconds since Epoch\n\
  %z   time of last status change, human-readable\n\
  %Z   time of last status change, seconds since Epoch\n\
\n\
"), stdout);

      fputs (_("\
Valid format sequences ಪ್ರತಿ file systems:\n\
\n\
  %a   ಮುಕ್ತ blocks available to non-superuser\n\
  %b   total data blocks in file system\n\
  %c   total file nodes in file system\n\
  %d   ಮುಕ್ತ file nodes in file system\n\
  %f   ಮುಕ್ತ blocks in file system\n\
"), stdout);
      fputs (_("\
  %i   file system ID in hex\n\
  %l   maximum length of filenames\n\
  %n   file name\n\
  %Qn  quoted file name\n\
  %s   block size (ಪ್ರತಿ faster transfers)\n\
  %S   fundamental block size (ಪ್ರತಿ block counts)\n\
  %t   file system type in hex\n\
  %T   file system type in human readable form\n\
"), stdout);

      printf (_("\n\
--terse is equivalent to the following FORMAT:\n\
    %s\
"),
#ಯದಿ HAVE_SELINUX_SELINUX_H
              fmt_terse_selinux
#ಅನ್ಯಥಾ
              fmt_terse_regular
#endif
              );

        printf (_("\
--terse --file-system is equivalent to the following FORMAT:\n\
    %s\
"), fmt_terse_fs);

      printf (USAGE_BUILTIN_WARNING, PROGRAM_NAME);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ *argv[])
{
  ಪೂರ್ಣಾಙ್ಕ c;
  bool fs = false;
  bool terse = false;
  ವರ್ಣ *format = ರಿಕ್ತ;
  ವರ್ಣ *format2;
  bool ok = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  ಸಂರಚನಾ lconv ಧ್ರುವ *locale = localeconv ();
  decimal_point = (locale->decimal_point[0] ? locale->decimal_point : ".");
  decimal_point_len = strlen (decimal_point);

  atexit (close_stdout);

  ಯಾವತ್ ((c = getopt_long (argc, argv, "c:fLt", long_options, ರಿಕ್ತ)) != -1)
    {
      ವಿಕಲ್ಪ (c)
        {
        ಪಕ್ಷ PRINTF_OPTION:
          format = optarg;
          interpret_backslash_escapes = true;
          trailing_delim = "";
          ಭಙ್ಗ;

        ಪಕ್ಷ 'c':
          format = optarg;
          interpret_backslash_escapes = false;
          trailing_delim = "\n";
          ಭಙ್ಗ;

        ಪಕ್ಷ 'L':
          follow_links = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'f':
          fs = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 't':
          terse = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 0:
          ವಿಕಲ್ಪ (XARGMATCH ("--cached", optarg, cached_args, cached_modes))
            {
              ಪಕ್ಷ cached_never:
                force_sync = true;
                dont_sync = false;
                ಭಙ್ಗ;
              ಪಕ್ಷ cached_always:
                force_sync = false;
                dont_sync = true;
                ಭಙ್ಗ;
              ಪಕ್ಷ cached_default:
                force_sync = false;
                dont_sync = false;
            }
          ಭಙ್ಗ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        ಸ್ವಭಾವ:
          usage (EXIT_FAILURE);
        }
    }

  ಯದಿ (argc == optind)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  ಯದಿ (format)
    {
      format2 = format;
    }
  ಅನ್ಯಥಾ
    {
      format = default_format (fs, terse, /* device= */ false);
      format2 = default_format (fs, terse, /* device= */ true);
    }

  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = optind; i < argc; i++)
    ok &= (fs
           ? do_statfs (argv[i], format)
           : do_stat (argv[i], format, format2));

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
