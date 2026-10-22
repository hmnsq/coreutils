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

#include <config.h>

/* Keep this conditional in sync with the similar conditional in
   ../m4/stat-prog.m4.  */
#ако ((STAT_STATVFS || STAT_STATVFS64)                                       \
     && (HAVE_STRUCT_STATVFS_F_BASETYPE || HAVE_STRUCT_STATVFS_F_FSTYPENAME \
         || (! HAVE_STRUCT_STATFS_F_FSTYPENAME && HAVE_STRUCT_STATVFS_F_TYPE)))
# define USE_STATVFS 1
#инако
# define USE_STATVFS 0
#endif

#include <stdio.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#ако USE_STATVFS
# include <sys/statvfs.h>
#elif HAVE_SYS_VFS_H
# include <sys/vfs.h>
#elif HAVE_SYS_MOUNT_H && HAVE_SYS_PARAM_H
/* NOTE: freebsd5.0 needs sys/param.h and sys/mount.h for statfs.
   It does have statvfs.h, but shouldn't use it, since it doesn't
   HAVE_STRUCT_STATVFS_F_BASETYPE.  So find a clean way to fix it.  */
/* NetBSD 1.5.2 needs these, for the declaration of struct statfs. */
# include <sys/param.h>
# include <sys/mount.h>
# ако HAVE_NFS_NFS_CLNT_H && HAVE_NFS_VFS_H
/* Ultrix 4.4 needs these for the declaration of struct statfs.  */
#  include <netinet/in.h>
#  include <nfs/nfs_clnt.h>
#  include <nfs/vfs.h>
# endif
#elif HAVE_OS_H /* BeOS */
# include <fs_info.h>
#endif
#include <selinux/selinux.h>
#include <getopt.h>

#include "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#include "system.h"

#include "areadlink.h"
#include "c-ctype.h"
#include "file-type.h"
#include "filemode.h"
#include "fs.h"
#include "mountlist.h"
#include "octhexdigits.h"
#include "quote.h"
#include "stat-size.h"
#include "stat-time.h"
#include "strftime.h"
#include "find-mount-point.h"
#include "xvasprintf.h"
#include "statx.h"

#ако HAVE_STATX && defined STATX_INO
# define USE_STATX 1
#инако
# define USE_STATX 0
#endif

#ако USE_STATVFS
# define STRUCT_STATXFS_F_FSID_IS_INTEGER STRUCT_STATVFS_F_FSID_IS_INTEGER
# define HAVE_STRUCT_STATXFS_F_TYPE HAVE_STRUCT_STATVFS_F_TYPE
# ако HAVE_STRUCT_STATVFS_F_NAMEMAX
#  define SB_F_NAMEMAX(S) ((S)->f_namemax)
# endif
# ако ! STAT_STATVFS && STAT_STATVFS64
#  define STRUCT_STATVFS struct statvfs64
#  define STATFS statvfs64
# инако
#  define STRUCT_STATVFS struct statvfs
#  define STATFS statvfs
# endif
# define STATFS_FRSIZE(S) ((S)->f_frsize)
#инако
# define HAVE_STRUCT_STATXFS_F_TYPE HAVE_STRUCT_STATFS_F_TYPE
# ако HAVE_STRUCT_STATFS_F_NAMELEN
#  define SB_F_NAMEMAX(S) ((S)->f_namelen)
# elif HAVE_STRUCT_STATFS_F_NAMEMAX
#  define SB_F_NAMEMAX(S) ((S)->f_namemax)
# endif
# define STATFS statfs
# ако HAVE_OS_H /* BeOS */
/* BeOS has a statvfs function, but it does not return sensible values
   for f_files, f_ffree and f_favail, and lacks f_type, f_basetype and
   f_fstypename.  Use 'struct fs_info' instead.  */
NODISCARD
static цео
statfs (char const *filename, struct fs_info *buf)
{
  dev_t device = dev_for_path (filename);
  ако (device < 0)
    {
      errno = (device == B_ENTRY_NOT_FOUND ? ENOENT
               : device == B_BAD_VALUE ? EINVAL
               : device == B_NAME_TOO_LONG ? ENAMETOOLONG
               : device == B_NO_MEMORY ? ENOMEM
               : device == B_FILE_ERROR ? EIO
               : 0);
      врати -1;
    }
  /* If successful, buf->dev will be == device.  */
  врати fs_stat_dev (device, buf);
}
#  define f_fsid dev
#  define f_blocks total_blocks
#  define f_bfree free_blocks
#  define f_bavail free_blocks
#  define f_bsize io_size
#  define f_files total_nodes
#  define f_ffree free_nodes
#  define STRUCT_STATVFS struct fs_info
#  define STRUCT_STATXFS_F_FSID_IS_INTEGER true
#  define STATFS_FRSIZE(S) ((S)->block_size)
# инако
#  define STRUCT_STATVFS struct statfs
#  define STRUCT_STATXFS_F_FSID_IS_INTEGER STRUCT_STATFS_F_FSID_IS_INTEGER
#  ако HAVE_STRUCT_STATFS_F_FRSIZE
#   define STATFS_FRSIZE(S) ((S)->f_frsize)
#  инако
#   define STATFS_FRSIZE(S) 0
#  endif
# endif
#endif

#ifdef SB_F_NAMEMAX
# define OUT_NAMEMAX out_uint
#инако
/* Depending on whether statvfs or statfs is used,
   neither f_namemax or f_namelen may be available.  */
# define SB_F_NAMEMAX(S) "?"
# define OUT_NAMEMAX out_string
#endif

#ако HAVE_STRUCT_STATVFS_F_BASETYPE
# define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME f_basetype
#инако
# ако HAVE_STRUCT_STATVFS_F_FSTYPENAME || HAVE_STRUCT_STATFS_F_FSTYPENAME
#  define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME f_fstypename
# elif HAVE_OS_H /* BeOS */
#  define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME fsh_name
# endif
#endif

#ако HAVE_GETATTRAT
# include <attr.h>
# include <sys/nvpair.h>
#endif

static char const digits[] = "0123456789";

/* Flags that are portable for use in printf, for at least one
   conversion specifier; make_format removes non-portable flags as
   needed for particular specifiers.  The glibc 2.2 extension "I" is
   listed here; it is removed by make_format because it has undefined
   behavior elsewhere and because it is incompatible with
   out_epoch_sec.  */
static char const printf_flags[] = "'-+ #0I";

/* Formats for the --terse option.  */
static char const fmt_terse_fs[] = "%Qn %i %l %t %s %S %b %f %a %c %d\n";
static char const fmt_terse_regular[] = "%Qn %s %b %f %u %g %D %i %h %t %T"
                                        " %X %Y %Z %W %o\n";
static char const fmt_terse_selinux[] = "%Qn %s %b %f %u %g %D %i %h %t %T"
                                        " %X %Y %Z %W %o %C\n";

#define PROGRAM_NAME "stat"

#define AUTHORS proper_name ("Michael Meskes")

enum
{
  PRINTF_OPTION = CHAR_MAX + 1
};

enum cached_mode
{
  cached_default,
  cached_never,
  cached_always
};

static char const *const cached_args[] =
{
  "default", "never", "always", NULL
};

static enum cached_mode const cached_modes[] =
{
  cached_default, cached_never, cached_always
};

static struct option const long_options[] =
{
  {"dereference", no_argument, NULL, 'L'},
  {"file-system", no_argument, NULL, 'f'},
  {"format", required_argument, NULL, 'c'},
  {"printf", required_argument, NULL, PRINTF_OPTION},
  {"terse", no_argument, NULL, 't'},
  {"cached", required_argument, NULL, 0},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0}
};

/* Whether to follow symbolic links;  True for --dereference (-L).  */
static bool follow_links;

/* Whether to interpret backslash-escape sequences.
   True for --printf=FMT, not for --format=FMT (-c).  */
static bool interpret_backslash_escapes;

/* The trailing delimiter string:
   "" for --printf=FMT, "\n" for --format=FMT (-c).  */
static char const *trailing_delim = "";

/* The representation of the decimal point in the current locale.  */
static char const *decimal_point;
static size_t decimal_point_len;

static bool
print_stat (char *pformat, size_t prefix_len, char mod, char m,
            цео fd, char const *filename, void const *data);

/* Return the type of the specified file system.
   Some systems have statfvs.f_basetype[FSTYPSZ] (AIX, HP-UX, and Solaris).
   Others have statvfs.f_fstypename[_VFS_NAMELEN] (NetBSD 3.0).
   Others have statfs.f_fstypename[MFSNAMELEN] (NetBSD 1.5.2).
   Still others have neither and have to get by with f_type (GNU/Linux).
   But f_type may only exist in statfs (Cygwin).  */
NODISCARD
static char const *
human_fstype (STRUCT_STATVFS const *statfsbuf)
{
#ifdef STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME
  врати statfsbuf->STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME;
#инако
  switch (statfsbuf->f_type)
    {
# ако defined __linux__ || defined __ANDROID__

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

    case S_MAGIC_AAFS: /* 0x5A3C69F0 local */
      врати "aafs";
    case S_MAGIC_ACFS: /* 0x61636673 remote */
      врати "acfs";
    case S_MAGIC_ADFS: /* 0xADF5 local */
      врати "adfs";
    case S_MAGIC_AFFS: /* 0xADFF local */
      врати "affs";
    case S_MAGIC_AFS: /* 0x5346414F remote */
      врати "afs";
    case S_MAGIC_ANON_INODE_FS: /* 0x09041934 local */
      врати "anon-inode FS";
    case S_MAGIC_AUFS: /* 0x61756673 remote */
      /* FIXME: change syntax or add an optional attribute like "inotify:no".
         The above is labeled as "remote" so that tail always uses polling,
         but this isn't really a remote file system type.  */
      врати "aufs";
    case S_MAGIC_AUTOFS: /* 0x0187 local */
      врати "autofs";
    case S_MAGIC_BALLOON_KVM: /* 0x13661366 local */
      врати "balloon-kvm-fs";
    case S_MAGIC_BCACHEFS: /* 0xCA451A4E local */
      врати "bcachefs";
    case S_MAGIC_BEFS: /* 0x42465331 local */
      врати "befs";
    case S_MAGIC_BDEVFS: /* 0x62646576 local */
      врати "bdevfs";
    case S_MAGIC_BFS: /* 0x1BADFACE local */
      врати "bfs";
    case S_MAGIC_BINDERFS: /* 0x6C6F6F70 local */
      врати "binderfs";
    case S_MAGIC_BPF_FS: /* 0xCAFE4A11 local */
      врати "bpf_fs";
    case S_MAGIC_BINFMTFS: /* 0x42494E4D local */
      врати "binfmt_misc";
    case S_MAGIC_BTRFS: /* 0x9123683E local */
      врати "btrfs";
    case S_MAGIC_BTRFS_TEST: /* 0x73727279 local */
      врати "btrfs_test";
    case S_MAGIC_CEPH: /* 0x00C36400 remote */
      врати "ceph";
    case S_MAGIC_CGROUP: /* 0x0027E0EB local */
      врати "cgroupfs";
    case S_MAGIC_CGROUP2: /* 0x63677270 local */
      врати "cgroup2fs";
    case S_MAGIC_CIFS: /* 0xFF534D42 remote */
      врати "cifs";
    case S_MAGIC_CODA: /* 0x73757245 remote */
      врати "coda";
    case S_MAGIC_COH: /* 0x012FF7B7 local */
      врати "coh";
    case S_MAGIC_CONFIGFS: /* 0x62656570 local */
      врати "configfs";
    case S_MAGIC_CRAMFS: /* 0x28CD3D45 local */
      врати "cramfs";
    case S_MAGIC_CRAMFS_WEND: /* 0x453DCD28 local */
      врати "cramfs-wend";
    case S_MAGIC_DAXFS: /* 0x64646178 local */
      врати "daxfs";
    case S_MAGIC_DEBUGFS: /* 0x64626720 local */
      врати "debugfs";
    case S_MAGIC_DEVFS: /* 0x1373 local */
      врати "devfs";
    case S_MAGIC_DEVMEM: /* 0x454D444D local */
      врати "devmem";
    case S_MAGIC_DEVPTS: /* 0x1CD1 local */
      врати "devpts";
    case S_MAGIC_DMA_BUF: /* 0x444D4142 local */
      врати "dma-buf-fs";
    case S_MAGIC_ECRYPTFS: /* 0xF15F local */
      врати "ecryptfs";
    case S_MAGIC_EFIVARFS: /* 0xDE5E81E4 local */
      врати "efivarfs";
    case S_MAGIC_EFS: /* 0x00414A53 local */
      врати "efs";
    case S_MAGIC_EROFS_V1: /* 0xE0F5E1E2 local */
      врати "erofs";
    case S_MAGIC_EXFAT: /* 0x2011BAB0 local */
      врати "exfat";
    case S_MAGIC_EXFS: /* 0x45584653 local */
      врати "exfs";
    case S_MAGIC_EXOFS: /* 0x5DF5 local */
      врати "exofs";
    case S_MAGIC_EXT: /* 0x137D local */
      врати "ext";
    case S_MAGIC_EXT2: /* 0xEF53 local */
      врати "ext2/ext3";
    case S_MAGIC_EXT2_OLD: /* 0xEF51 local */
      врати "ext2";
    case S_MAGIC_F2FS: /* 0xF2F52010 local */
      врати "f2fs";
    case S_MAGIC_FAIL_FS: /* 0x4641494C local */
      врати "failfs";
    case S_MAGIC_FAT: /* 0x4006 local */
      врати "fat";
    case S_MAGIC_FHGFS: /* 0x19830326 remote */
      врати "fhgfs";
    case S_MAGIC_FUSE: /* 0x65735546 remote */
      врати "fuse";
    case S_MAGIC_FUSECTL: /* 0x65735543 remote */
      врати "fusectl";
    case S_MAGIC_FUTEXFS: /* 0x0BAD1DEA local */
      врати "futexfs";
    case S_MAGIC_GFS: /* 0x01161970 remote */
      врати "gfs/gfs2";
    case S_MAGIC_GPFS: /* 0x47504653 remote */
      врати "gpfs";
    case S_MAGIC_GUEST_MEMFD: /* 0x474D454D remote */
      врати "guest-memfd";
    case S_MAGIC_HFS: /* 0x4244 local */
      врати "hfs";
    case S_MAGIC_HFS_PLUS: /* 0x482B local */
      врати "hfs+";
    case S_MAGIC_HFS_X: /* 0x4858 local */
      врати "hfsx";
    case S_MAGIC_HOSTFS: /* 0x00C0FFEE local */
      врати "hostfs";
    case S_MAGIC_HPFS: /* 0xF995E849 local */
      врати "hpfs";
    case S_MAGIC_HUGETLBFS: /* 0x958458F6 local */
      врати "hugetlbfs";
    case S_MAGIC_MTD_INODE_FS: /* 0x11307854 local */
      врати "inodefs";
    case S_MAGIC_IBRIX: /* 0x013111A8 remote */
      врати "ibrix";
    case S_MAGIC_INOTIFYFS: /* 0x2BAD1DEA local */
      врати "inotifyfs";
    case S_MAGIC_ISOFS: /* 0x9660 local */
      врати "isofs";
    case S_MAGIC_ISOFS_R_WIN: /* 0x4004 local */
      врати "isofs";
    case S_MAGIC_ISOFS_WIN: /* 0x4000 local */
      врати "isofs";
    case S_MAGIC_JFFS: /* 0x07C0 local */
      врати "jffs";
    case S_MAGIC_JFFS2: /* 0x72B6 local */
      врати "jffs2";
    case S_MAGIC_JFS: /* 0x3153464A local */
      врати "jfs";
    case S_MAGIC_KAFS: /* 0x6B414653 remote */
      врати "k-afs";
    case S_MAGIC_LOGFS: /* 0xC97E8168 local */
      врати "logfs";
    case S_MAGIC_LUSTRE: /* 0x0BD00BD0 remote */
      врати "lustre";
    case S_MAGIC_M1FS: /* 0x5346314D local */
      врати "m1fs";
    case S_MAGIC_MINIX: /* 0x137F local */
      врати "minix";
    case S_MAGIC_MINIX_30: /* 0x138F local */
      врати "minix (30 char.)";
    case S_MAGIC_MINIX_V2: /* 0x2468 local */
      врати "minix v2";
    case S_MAGIC_MINIX_V2_30: /* 0x2478 local */
      врати "minix v2 (30 char.)";
    case S_MAGIC_MINIX_V3: /* 0x4D5A local */
      врати "minix3";
    case S_MAGIC_MQUEUE: /* 0x19800202 local */
      врати "mqueue";
    case S_MAGIC_MSDOS: /* 0x4D44 local */
      врати "msdos";
    case S_MAGIC_NCP: /* 0x564C remote */
      врати "novell";
    case S_MAGIC_NFS: /* 0x6969 remote */
      врати "nfs";
    case S_MAGIC_NFSD: /* 0x6E667364 remote */
      врати "nfsd";
    case S_MAGIC_NILFS: /* 0x3434 local */
      врати "nilfs";
    case S_MAGIC_NSFS: /* 0x6E736673 local */
      врати "nsfs";
    case S_MAGIC_NTFS: /* 0x5346544E local */
      врати "ntfs";
    case S_MAGIC_NULL_FS: /* 0x4E554C4C local */
      врати "nullfs";
    case S_MAGIC_OPENPROM: /* 0x9FA1 local */
      врати "openprom";
    case S_MAGIC_OCFS2: /* 0x7461636F remote */
      врати "ocfs2";
    case S_MAGIC_OVERLAYFS: /* 0x794C7630 remote */
      /* This may overlay remote file systems.
         Also there have been issues reported with inotify and overlayfs,
         so mark as "remote" so that polling is used.  */
      врати "overlayfs";
    case S_MAGIC_PANFS: /* 0xAAD7AAEA remote */
      врати "panfs";
    case S_MAGIC_PID_FS: /* 0x50494446 local */
      врати "pidfs";
    case S_MAGIC_PIPEFS: /* 0x50495045 remote */
      /* FIXME: change syntax or add an optional attribute like "inotify:no".
         pipefs is labeled as "remote" so that tail always polls,
         but it isn't really remote file system type.  */
      врати "pipefs";
    case S_MAGIC_PPC_CMM: /* 0xC7571590 local */
      врати "ppc-cmm-fs";
    case S_MAGIC_PRL_FS: /* 0x7C7C6673 remote */
      врати "prl_fs";
    case S_MAGIC_PROC: /* 0x9FA0 local */
      врати "proc";
    case S_MAGIC_PSTOREFS: /* 0x6165676C local */
      врати "pstorefs";
    case S_MAGIC_QNX4: /* 0x002F local */
      врати "qnx4";
    case S_MAGIC_QNX6: /* 0x68191122 local */
      врати "qnx6";
    case S_MAGIC_RAMFS: /* 0x858458F6 local */
      врати "ramfs";
    case S_MAGIC_RDTGROUP: /* 0x07655821 local */
      врати "rdt";
    case S_MAGIC_REISERFS: /* 0x52654973 local */
      врати "reiserfs";
    case S_MAGIC_ROMFS: /* 0x7275 local */
      врати "romfs";
    case S_MAGIC_RPC_PIPEFS: /* 0x67596969 local */
      врати "rpc_pipefs";
    case S_MAGIC_SDCARDFS: /* 0x5DCA2DF5 local */
      врати "sdcardfs";
    case S_MAGIC_SECRETMEM: /* 0x5345434D local */
      врати "secretmem";
    case S_MAGIC_SECURITYFS: /* 0x73636673 local */
      врати "securityfs";
    case S_MAGIC_SELINUX: /* 0xF97CFF8C local */
      врати "selinux";
    case S_MAGIC_SMACK: /* 0x43415D53 local */
      врати "smackfs";
    case S_MAGIC_SMB: /* 0x517B remote */
      врати "smb";
    case S_MAGIC_SMB2: /* 0xFE534D42 remote */
      врати "smb2";
    case S_MAGIC_SNFS: /* 0xBEEFDEAD remote */
      врати "snfs";
    case S_MAGIC_SOCKFS: /* 0x534F434B local */
      врати "sockfs";
    case S_MAGIC_SQUASHFS: /* 0x73717368 local */
      врати "squashfs";
    case S_MAGIC_SYSFS: /* 0x62656572 local */
      врати "sysfs";
    case S_MAGIC_SYSV2: /* 0x012FF7B6 local */
      врати "sysv2";
    case S_MAGIC_SYSV4: /* 0x012FF7B5 local */
      врати "sysv4";
    case S_MAGIC_TMPFS: /* 0x01021994 local */
      врати "tmpfs";
    case S_MAGIC_TRACEFS: /* 0x74726163 local */
      врати "tracefs";
    case S_MAGIC_UBIFS: /* 0x24051905 local */
      врати "ubifs";
    case S_MAGIC_UDF: /* 0x15013346 local */
      врати "udf";
    case S_MAGIC_UFS: /* 0x00011954 local */
      врати "ufs";
    case S_MAGIC_UFS_BYTESWAPPED: /* 0x54190100 local */
      врати "ufs";
    case S_MAGIC_USBDEVFS: /* 0x9FA2 local */
      врати "usbdevfs";
    case S_MAGIC_V9FS: /* 0x01021997 local */
      врати "v9fs";
    case S_MAGIC_VBOXSF: /* 0x786F4256 remote */
      врати "vboxsf";
    case S_MAGIC_VMHGFS: /* 0xBACBACBC remote */
      врати "vmhgfs";
    case S_MAGIC_VXFS: /* 0xA501FCF5 remote */
      /* Veritas File System can run in single instance or clustered mode,
         so mark as remote to cater for the latter case.  */
      врати "vxfs";
    case S_MAGIC_VZFS: /* 0x565A4653 local */
      врати "vzfs";
    case S_MAGIC_WSLFS: /* 0x53464846 local */
      врати "wslfs";
    case S_MAGIC_XENFS: /* 0xABBA1974 local */
      врати "xenfs";
    case S_MAGIC_XENIX: /* 0x012FF7B4 local */
      врати "xenix";
    case S_MAGIC_XFS: /* 0x58465342 local */
      врати "xfs";
    case S_MAGIC_XIAFS: /* 0x012FD16D local */
      врати "xia";
    case S_MAGIC_Z3FOLD: /* 0x0033 local */
      врати "z3fold";
    case S_MAGIC_ZFS: /* 0x2FC12FC1 local */
      врати "zfs";
    case S_MAGIC_ZONEFS: /* 0x5A4F4653 local */
      врати "zonefs";
    case S_MAGIC_ZSMALLOC: /* 0x58295829 local */
      врати "zsmallocfs";


# elif __GNU__
    case FSTYPE_UFS:
      врати "ufs";
    case FSTYPE_NFS:
      врати "nfs";
    case FSTYPE_GFS:
      врати "gfs";
    case FSTYPE_LFS:
      врати "lfs";
    case FSTYPE_SYSV:
      врати "sysv";
    case FSTYPE_FTP:
      врати "ftp";
    case FSTYPE_TAR:
      врати "tar";
    case FSTYPE_AR:
      врати "ar";
    case FSTYPE_CPIO:
      врати "cpio";
    case FSTYPE_MSLOSS:
      врати "msloss";
    case FSTYPE_CPM:
      врати "cpm";
    case FSTYPE_HFS:
      врати "hfs";
    case FSTYPE_DTFS:
      врати "dtfs";
    case FSTYPE_GRFS:
      врати "grfs";
    case FSTYPE_TERM:
      врати "term";
    case FSTYPE_DEV:
      врати "dev";
    case FSTYPE_PROC:
      врати "proc";
    case FSTYPE_IFSOCK:
      врати "ifsock";
    case FSTYPE_AFS:
      врати "afs";
    case FSTYPE_DFS:
      врати "dfs";
    case FSTYPE_PROC9:
      врати "proc9";
    case FSTYPE_SOCKET:
      врати "socket";
    case FSTYPE_MISC:
      врати "misc";
    case FSTYPE_EXT2FS:
      врати "ext2/ext3";
    case FSTYPE_HTTP:
      врати "http";
    case FSTYPE_MEMFS:
      врати "memfs";
    case FSTYPE_ISO9660:
      врати "iso9660";
# endif
    default:
      {
        unsigned long цео type = statfsbuf->f_type;
        static char buf[sizeof "UNKNOWN (0x%lx)" - 3
                        + (sizeof type * CHAR_BIT + 3) / 4];
        sprintf (buf, "UNKNOWN (0x%lx)", type);
        врати buf;
      }
    }
#endif
}

NODISCARD
static char *
human_access (struct stat const *statbuf)
{
  static char modebuf[12];
  filemodestring (statbuf, modebuf);
  modebuf[10] = 0;
  врати modebuf;
}

NODISCARD
static char *
human_time (struct timespec t)
{
  /* STR must be at least INT_BUFSIZE_BOUND (intmax_t) big, either
     because localtime_rz fails, or because the time zone is truly
     outlandish so that %z expands to a long string.  */
  static char str[INT_BUFSIZE_BOUND (intmax_t)
                  + INT_STRLEN_BOUND (цео) /* YYYY */
                  + 1 /* because YYYY might equal INT_MAX + 1900 */
                  + sizeof "-MM-DD HH:MM:SS.NNNNNNNNN +"];
  static timezone_t tz;
  ако (!tz)
    tz = tzalloc (getenv ("TZ"));
  struct tm tm;
  цео ns = t.tv_nsec;
  ако (localtime_rz (tz, &t.tv_sec, &tm))
    nstrftime (str, sizeof str, "%Y-%m-%d %H:%M:%S.%N %z", &tm, tz, ns);
  инако
    {
      char secbuf[INT_BUFSIZE_BOUND (intmax_t)];
      sprintf (str, "%s.%09d", timetostr (t.tv_sec, secbuf), ns);
    }
  врати str;
}

/* PFORMAT points to a '%' followed by a prefix of a format, all of
   size PREFIX_LEN.  The flags allowed for this format are
   ALLOWED_FLAGS; remove other printf flags from the prefix, then
   append SUFFIX.  */
static void
make_format (char *pformat, size_t prefix_len, char const *allowed_flags,
             char const *suffix)
{
  char *dst = pformat + 1;
  char const *src;
  char const *srclim = pformat + prefix_len;
  за (src = dst; src < srclim && strchr (printf_flags, *src); src++)
    ако (strchr (allowed_flags, *src))
      *dst++ = *src;
  док (src < srclim)
    *dst++ = *src++;
  strcpy (dst, suffix);
}

static void
out_string (char *pformat, size_t prefix_len, char const *arg)
{
  make_format (pformat, prefix_len, "-", "s");
  printf (pformat, arg);
}
static цео
out_int (char *pformat, size_t prefix_len, intmax_t arg)
{
  make_format (pformat, prefix_len, "'-+ 0", "jd");
  врати printf (pformat, arg);
}
static цео
out_uint (char *pformat, size_t prefix_len, uintmax_t arg)
{
  make_format (pformat, prefix_len, "'-0", "ju");
  врати printf (pformat, arg);
}
static void
out_uint_o (char *pformat, size_t prefix_len, uintmax_t arg)
{
  make_format (pformat, prefix_len, "-#0", "jo");
  printf (pformat, arg);
}
static void
out_uint_x (char *pformat, size_t prefix_len, uintmax_t arg)
{
  make_format (pformat, prefix_len, "-#0", "jx");
  printf (pformat, arg);
}
static цео
out_minus_zero (char *pformat, size_t prefix_len)
{
  make_format (pformat, prefix_len, "'-+ 0", ".0f");
  врати printf (pformat, -0.25);
}

/* Output the number of seconds since the Epoch, using a format that
   acts like printf's %f format.  */
static void
out_epoch_sec (char *pformat, size_t prefix_len,
               struct timespec arg)
{
  char *dot = memchr (pformat, '.', prefix_len);
  size_t sec_prefix_len = prefix_len;
  цео width = 0;
  цео precision = 0;
  bool frac_left_adjust = false;

  ако (dot)
    {
      sec_prefix_len = dot - pformat;
      pformat[prefix_len] = '\0';

      ако (c_isdigit (dot[1]))
        {
          long цео lprec = strtol (dot + 1, NULL, 10);
          precision = (lprec <= INT_MAX ? lprec : INT_MAX);
        }
      инако
        {
          precision = 9;
        }

      ако (precision && c_isdigit (dot[-1]))
        {
          /* If a nontrivial width is given, subtract the width of the
             decimal point and PRECISION digits that will be output
             later.  */
          char *p = dot;
          *dot = '\0';

          do
            --p;
          док (c_isdigit (p[-1]));

          long цео lwidth = strtol (p, NULL, 10);
          width = (lwidth <= INT_MAX ? lwidth : INT_MAX);
          ако (1 < width)
            {
              p += (*p == '0');
              sec_prefix_len = p - pformat;
              цео w_d = (decimal_point_len < width
                         ? width - decimal_point_len
                         : 0);
              ако (1 < w_d)
                {
                  цео w = w_d - precision;
                  ако (1 < w)
                    {
                      char *dst = pformat;
                      за (char const *src = dst; src < p; src++)
                        {
                          ако (*src == '-')
                            frac_left_adjust = true;
                          инако
                            *dst++ = *src;
                        }
                      sec_prefix_len =
                        (dst - pformat
                         + (frac_left_adjust ? 0 : sprintf (dst, "%d", w)));
                    }
                }
            }
        }
    }

  цео divisor = 1;
  за (цео i = precision; i < 9; i++)
    divisor *= 10;
  цео frac_sec = arg.tv_nsec / divisor;
  цео int_len;

  ако (TYPE_SIGNED (time_t))
    {
      bool minus_zero = false;
      ако (arg.tv_sec < 0 && arg.tv_nsec != 0)
        {
          цео frac_sec_modulus = 1000000000 / divisor;
          frac_sec = (frac_sec_modulus - frac_sec
                      - (arg.tv_nsec % divisor != 0));
          arg.tv_sec += (frac_sec != 0);
          minus_zero = (arg.tv_sec == 0);
        }
      int_len = (minus_zero
                 ? out_minus_zero (pformat, sec_prefix_len)
                 : out_int (pformat, sec_prefix_len, arg.tv_sec));
    }
  инако
    int_len = out_uint (pformat, sec_prefix_len, arg.tv_sec);

  ако (precision)
    {
      цео prec = (precision < 9 ? precision : 9);
      цео trailing_prec = precision - prec;
      цео ilen = (int_len < 0 ? 0 : int_len);
      цео trailing_width = (ilen < width && decimal_point_len < width - ilen
                            ? width - ilen - decimal_point_len - prec
                            : 0);
      printf ("%s%.*d%-*.*d", decimal_point, prec, frac_sec,
              trailing_width, trailing_prec, 0);
    }
}

/* Print the context information of FILENAME, and return true iff the
   context could not be obtained.  */
NODISCARD
static bool
out_file_context (char *pformat, size_t prefix_len, char const *filename)
{
  char *scontext;
  bool fail = false;

  ако ((follow_links
       ? getfilecon (filename, &scontext)
       : lgetfilecon (filename, &scontext)) < 0)
    {
      error (0, errno, _("failed to get security context of %s"),
             quoteaf (filename));
      scontext = NULL;
      fail = true;
    }
  strcpy (pformat + prefix_len, "s");
  printf (pformat, (scontext ? scontext : "?"));
  ако (scontext)
    freecon (scontext);
  врати fail;
}

/* Set the quoting style once it is needed.  */
static void
initialize_quoting_style (void)
{
  static bool initialized;
  ако (!initialized)
    {
      initialized = true;
      цео qs = getenv_quoting_style ();
      set_quoting_style (NULL, 0 <= qs ? qs : shell_escape_quoting_style);
    }
}

/* Print statfs info.  Return zero upon success, nonzero upon failure.  */
NODISCARD
static bool
print_statfs (char *pformat, size_t prefix_len, MAYBE_UNUSED char mod, char m,
              MAYBE_UNUSED цео fd, char const *filename,
              void const *data)
{
  STRUCT_STATVFS const *statfsbuf = data;
  bool fail = false;

  switch (m)
    {
    case 'n':
      ако (mod == 'Q')
        {
          initialize_quoting_style ();
          filename = quoteN (filename);
        }
      out_string (pformat, prefix_len, filename);
      break;

    case 'i':
      {
#ако STRUCT_STATXFS_F_FSID_IS_INTEGER
        uintmax_t fsid = statfsbuf->f_fsid;
#инако
        typedef unsigned цео fsid_word;
        static_assert (alignof (STRUCT_STATVFS) % alignof (fsid_word) == 0);
        static_assert (offsetof (STRUCT_STATVFS, f_fsid) % alignof (fsid_word)
                       == 0);
        static_assert (sizeof statfsbuf->f_fsid % alignof (fsid_word) == 0);
        fsid_word const *p = (fsid_word *) &statfsbuf->f_fsid;

        /* Assume a little-endian word order, as that is compatible
           with glibc's statvfs implementation.  */
        uintmax_t fsid = 0;
        цео words = sizeof statfsbuf->f_fsid / sizeof *p;
        за (цео i = 0; i < words && i * sizeof *p < sizeof fsid; i++)
          {
            uintmax_t u = p[words - 1 - i];
            fsid |= u << (i * CHAR_BIT * sizeof *p);
          }
#endif
        out_uint_x (pformat, prefix_len, fsid);
      }
      break;

    case 'l':
      OUT_NAMEMAX (pformat, prefix_len, SB_F_NAMEMAX (statfsbuf));
      break;
    case 't':
#ако HAVE_STRUCT_STATXFS_F_TYPE
      out_uint_x (pformat, prefix_len, statfsbuf->f_type);
#инако
      fputc ('?', stdout);
#endif
      break;
    case 'T':
      out_string (pformat, prefix_len, human_fstype (statfsbuf));
      break;
    case 'b':
      out_int (pformat, prefix_len, statfsbuf->f_blocks);
      break;
    case 'f':
      out_int (pformat, prefix_len, statfsbuf->f_bfree);
      break;
    case 'a':
      out_int (pformat, prefix_len, statfsbuf->f_bavail);
      break;
    case 's':
      out_uint (pformat, prefix_len, statfsbuf->f_bsize);
      break;
    case 'S':
      {
        uintmax_t frsize = STATFS_FRSIZE (statfsbuf);
        ако (! frsize)
          frsize = statfsbuf->f_bsize;
        out_uint (pformat, prefix_len, frsize);
      }
      break;
    case 'c':
      out_uint (pformat, prefix_len, statfsbuf->f_files);
      break;
    case 'd':
      out_int (pformat, prefix_len, statfsbuf->f_ffree);
      break;
    default:
      fputc ('?', stdout);
      break;
    }
  врати fail;
}

/* Return any bind mounted source for a path.
   The caller should not free the returned buffer.
   Return NULL if no bind mount found.  */
NODISCARD
static char const *
find_bind_mount (char const * name)
{
  char const * bind_mount = NULL;

  static struct mount_entry *mount_list;
  static bool tried_mount_list = false;
  ако (!tried_mount_list) /* attempt/warn once per process.  */
    {
      ако (!(mount_list = read_file_system_list (false)))
        error (0, errno, "%s", _("cannot read table of mounted file systems"));
      tried_mount_list = true;
    }

  struct stat name_stats;
  ако (stat (name, &name_stats) != 0)
    врати NULL;

  за (struct mount_entry *me = mount_list; me; me = me->me_next)
    {
      ако (me->me_dummy && me->me_devname[0] == '/'
          && streq (me->me_mountdir, name))
        {
          struct stat dev_stats;

          ако (stat (me->me_devname, &dev_stats) == 0
              && psame_inode (&name_stats, &dev_stats))
            {
              bind_mount = me->me_devname;
              break;
            }
        }
    }

  врати bind_mount;
}

/* Print mount point.  Return zero upon success, nonzero upon failure.  */
NODISCARD
static bool
out_mount_point (char const *filename, char *pformat, size_t prefix_len,
                 const struct stat *statp)
{

  char const *np = "?", *bp = NULL;
  char *mp = NULL;
  bool fail = true;

  /* Look for bind mounts first.  Note we output the immediate alias,
     rather than further resolving to a base device mount point.  */
  ако (follow_links || !S_ISLNK (statp->st_mode))
    {
      char *resolved = canonicalize_file_name (filename);
      ако (!resolved)
        {
          error (0, errno, _("failed to canonicalize %s"), quoteaf (filename));
          goto print_mount_point;
        }
      bp = find_bind_mount (resolved);
      free (resolved);
      ако (bp)
        {
          fail = false;
          goto print_mount_point;
        }
    }

  /* If there is no direct bind mount, then navigate
     back up the tree looking for a device change.
     Note we don't detect if any of the directory components
     are bind mounted to the same device, but that's OK
     since we've not directly queried them.  */
  ако ((mp = find_mount_point (filename, statp)))
    {
      /* This dir might be bind mounted to another device,
         so we resolve the bound source in that case also.  */
      bp = find_bind_mount (mp);
      fail = false;
    }

print_mount_point:

  out_string (pformat, prefix_len, bp ? bp : mp ? mp : np);
  free (mp);
  врати fail;
}

/* Map a TS with negative TS.tv_nsec to {0,0}.  */
static inline struct timespec
neg_to_zero (struct timespec ts)
{
  ако (0 <= ts.tv_nsec)
    врати ts;
  struct timespec z = {0};
  врати z;
}

/* Output a single-character \ escape.  */

static void
print_esc_char (char c)
{
  switch (c)
    {
    case 'a':			/* Alert. */
      c ='\a';
      break;
    case 'b':			/* Backspace. */
      c ='\b';
      break;
    case 'e':			/* Escape. */
      c ='\x1B';
      break;
    case 'f':			/* Form feed. */
      c ='\f';
      break;
    case 'n':			/* New line. */
      c ='\n';
      break;
    case 'r':			/* Carriage return. */
      c ='\r';
      break;
    case 't':			/* Horizontal tab. */
      c ='\t';
      break;
    case 'v':			/* Vertical tab. */
      c ='\v';
      break;
    case '"':
    case '\\':
      break;
    default:
      error (0, 0, _("warning: unrecognized escape '\\%c'"), c);
      break;
    }
  putchar (c);
}

ATTRIBUTE_PURE
static size_t
format_code_offset (char const *directive)
{
  size_t len = strspn (directive + 1, printf_flags);
  char const *fmt_char = directive + len + 1;
  fmt_char += strspn (fmt_char, digits);
  ако (*fmt_char == '.')
    fmt_char += 1 + strspn (fmt_char + 1, digits);
  врати fmt_char - directive;
}

/* Print the information specified by the format string, FORMAT,
   calling PRINT_FUNC for each %-directive encountered.
   Return zero upon success, nonzero upon failure.  */
NODISCARD
static bool
print_it (char const *format, цео fd, char const *filename,
          bool (*print_func) (char *, size_t, char, char,
                              цео, char const *, void const *),
          void const *data)
{
  bool fail = false;

  /* Add 2 to accommodate our conversion of the stat '%s' format string
     to the longer printf '%llu' one.  */
  enum
    {
      MAX_ADDITIONAL_BYTES =
        (MAX (sizeof "jd",
              MAX (sizeof "jo", MAX (sizeof "ju", sizeof "jx")))
         - 1)
    };
  size_t n_alloc = strlen (format) + MAX_ADDITIONAL_BYTES + 1;
  char *dest = xmalloc (n_alloc);
  за (char const *b = format; *b; b++)
    {
      switch (*b)
        {
        case '%':
          {
            size_t len = format_code_offset (b);
            char fmt_char = *(b + len);
            char mod_char = 0;
            memcpy (dest, b, len);
            b += len;

            switch (fmt_char)
              {
              case '\0':
                --b;
                FALLTHROUGH;
              case '%':
                ако (1 < len)
                  {
                    dest[len] = fmt_char;
                    dest[len + 1] = '\0';
                    error (EXIT_FAILURE, 0, _("%s: invalid directive"),
                           quote (dest));
                  }
                putchar ('%');
                break;
              case 'H':
              case 'L':
              case 'Q':
                mod_char = fmt_char;
                fmt_char = *(b + 1);
                ако ((mod_char == 'Q' && fmt_char == 'n')
                    || (print_func == print_stat
                        && (mod_char == 'H' || mod_char == 'L')
                        && (fmt_char == 'd' || fmt_char == 'r')))
                  {
                    b++;
                  }
                инако
                  {
                    fmt_char = mod_char;
                    mod_char = 0;
                  }
                FALLTHROUGH;
              default:
                fail |= print_func (dest, len, mod_char, fmt_char,
                                    fd, filename, data);
                break;
              }
            break;
          }

        case '\\':
          ако ( ! interpret_backslash_escapes)
            {
              putchar ('\\');
              break;
            }
          ++b;
          ако (isoct (*b))
            {
              цео esc_value = fromoct (*b);
              цео esc_length = 1;	/* number of octal digits */
              за (++b; esc_length < 3 && isoct (*b);
                   ++esc_length, ++b)
                {
                  esc_value = esc_value * 8 + fromoct (*b);
                }
              putchar (esc_value);
              --b;
            }
          инако ако (*b == 'x' && c_isxdigit (b[1]))
            {
              цео esc_value = fromhex (b[1]);	/* Value of \xhh escape. */
              /* A hexadecimal \xhh escape sequence must have
                 1 or 2 hex. digits.  */
              ++b;
              ако (c_isxdigit (b[1]))
                {
                  ++b;
                  esc_value = esc_value * 16 + fromhex (*b);
                }
              putchar (esc_value);
            }
          инако ако (*b == '\0')
            {
              error (0, 0, _("warning: backslash at end of format"));
              putchar ('\\');
              /* Arrange to exit the loop.  */
              --b;
            }
          инако
            {
              print_esc_char (*b);
            }
          break;

        default:
          putchar (*b);
          break;
        }
    }
  free (dest);

  fputs (trailing_delim, stdout);

  врати fail;
}

/* Stat the file system and print what we find.  */
NODISCARD
static bool
do_statfs (char const *filename, char const *format)
{
  STRUCT_STATVFS statfsbuf;

  ако (streq (filename, "-"))
    {
      error (0, 0, _("using %s to denote standard input does not work"
                     " in file system mode"), quoteaf (filename));
      врати false;
    }

  ако (STATFS (filename, &statfsbuf) != 0)
    {
      error (0, errno, _("cannot read file system information for %s"),
             quoteaf (filename));
      врати false;
    }

  bool fail = print_it (format, -1, filename, print_statfs, &statfsbuf);
  врати ! fail;
}

struct print_args {
  struct stat *st;
  struct timespec btime;
};

/* Ask statx to avoid syncing? */
static bool dont_sync;

/* Ask statx to force sync? */
static bool force_sync;

#ако USE_STATX
static unsigned цео
fmt_to_mask (char fmt)
{
  switch (fmt)
    {
    case 'N':
      врати STATX_MODE;
    case 'd':
    case 'D':
      врати STATX_MODE;
    case 'i':
      врати STATX_INO;
    case 'a':
    case 'A':
      врати STATX_MODE;
    case 'f':
      врати STATX_MODE|STATX_TYPE;
    case 'F':
      врати STATX_TYPE;
    case 'h':
      врати STATX_NLINK;
    case 'u':
    case 'U':
      врати STATX_UID;
    case 'g':
    case 'G':
      врати STATX_GID;
    case 'm':
      врати STATX_MODE|STATX_INO;
    case 's':
      врати STATX_SIZE;
    case 't':
    case 'T':
      врати STATX_MODE;
    case 'b':
      врати STATX_BLOCKS;
    case 'w':
    case 'W':
      врати STATX_BTIME;
    case 'x':
    case 'X':
      врати STATX_ATIME;
    case 'y':
    case 'Y':
      врати STATX_MTIME;
    case 'z':
    case 'Z':
      врати STATX_CTIME;
    }
  врати 0;
}

ATTRIBUTE_PURE
static unsigned цео
format_to_mask (char const *format)
{
  unsigned цео mask = 0;

  за (char const *b = format; *b; b++)
    {
      ако (*b != '%')
        continue;

      b += format_code_offset (b);
      ако (*b == '\0')
        break;
      mask |= fmt_to_mask (*b);
    }
  врати mask;
}

/* statx the file and print what we find */
NODISCARD
static bool
do_stat (char const *filename, char const *format, char const *format2)
{
  цео fd = streq (filename, "-") ? 0 : AT_FDCWD;
  цео flags = 0;
  struct stat st;
  struct statx stx = {0};
  char const *pathname = filename;
  struct print_args pa;
  pa.st = &st;
  pa.btime = (struct timespec) {.tv_sec = -1, .tv_nsec = -1};

  ако (AT_FDCWD != fd)
    {
      pathname = "";
      flags = AT_EMPTY_PATH;
    }
  инако ако (!follow_links)
    {
      flags = AT_SYMLINK_NOFOLLOW;
    }

  ако (dont_sync)
    flags |= AT_STATX_DONT_SYNC;
  инако ако (force_sync)
    flags |= AT_STATX_FORCE_SYNC;

  ако (! force_sync)
    flags |= AT_NO_AUTOMOUNT;

  fd = statx (fd, pathname, flags, format_to_mask (format), &stx);
  ако (fd < 0)
    {
      ако (flags & AT_EMPTY_PATH)
        error (0, errno, _("cannot stat standard input"));
      инако
        error (0, errno, _("cannot statx %s"), quoteaf (filename));
      врати false;
    }

  ако (S_ISBLK (stx.stx_mode) || S_ISCHR (stx.stx_mode))
    format = format2;

  statx_to_stat (&stx, &st);
  ако (stx.stx_mask & STATX_BTIME)
    pa.btime = statx_timestamp_to_timespec (stx.stx_btime);

  bool fail = print_it (format, fd, filename, print_stat, &pa);
  врати ! fail;
}

#инако /* USE_STATX */

static struct timespec
get_birthtime (цео fd, char const *filename, struct stat const *st)
{
  struct timespec ts = get_stat_birthtime (st);

# ако HAVE_GETATTRAT
  ако (ts.tv_nsec < 0)
    {
      nvlist_t *response;
      ако ((fd < 0
           ? getattrat (AT_FDCWD, XATTR_VIEW_READWRITE, filename, &response)
           : fgetattr (fd, XATTR_VIEW_READWRITE, &response))
          == 0)
        {
          uint64_t *val;
          uint_t n;
          ако (nvlist_lookup_uint64_array (response, A_CRTIME, &val, &n) == 0
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

  врати ts;
}


/* stat the file and print what we find */
NODISCARD
static bool
do_stat (char const *filename, char const *format,
         char const *format2)
{
  цео fd = streq (filename, "-") ? 0 : -1;
  struct stat statbuf;
  struct print_args pa;
  pa.st = &statbuf;
  pa.btime = (struct timespec) {.tv_sec = -1, .tv_nsec = -1};

  ако (0 <= fd)
    {
      ако (fstat (fd, &statbuf) != 0)
        {
          error (0, errno, _("cannot stat standard input"));
          врати false;
        }
    }
  /* We can't use the shorter
     (follow_links?stat:lstat) (filename, &statbug)
     since stat might be a function-like macro.  */
  инако ако ((follow_links
            ? stat (filename, &statbuf)
            : lstat (filename, &statbuf)) != 0)
    {
      error (0, errno, _("cannot stat %s"), quoteaf (filename));
      врати false;
    }

  ако (S_ISBLK (statbuf.st_mode) || S_ISCHR (statbuf.st_mode))
    format = format2;

  bool fail = print_it (format, fd, filename, print_stat, &pa);
  врати ! fail;
}
#endif /* USE_STATX */

/* POSIX requires 'ls' to print file sizes without a sign, even
   when negative.  Be consistent with that.  */

static uintmax_t
unsigned_file_size (off_t size)
{
  врати size + (size < 0) * ((uintmax_t) OFF_T_MAX - OFF_T_MIN + 1);
}

/* Print stat info.  Return zero upon success, nonzero upon failure.  */
static bool
print_stat (char *pformat, size_t prefix_len, char mod, char m,
            MAYBE_UNUSED цео fd, char const *filename, void const *data)
{
  struct print_args *parg = (struct print_args *) data;
  struct stat *statbuf = parg->st;
  struct timespec btime = parg->btime;
  struct passwd *pw_ent;
  struct group *gw_ent;
  bool fail = false;

  switch (m)
    {
    case 'n':
      ако (mod == 'Q')
        {
          initialize_quoting_style ();
          filename = quoteN (filename);
        }
      out_string (pformat, prefix_len, filename);
      break;
    case 'N':
      initialize_quoting_style ();
      out_string (pformat, prefix_len, quoteN (filename));
      ако (S_ISLNK (statbuf->st_mode))
        {
          char *linkname = areadlink_with_size (filename, statbuf->st_size);
          ако (linkname == NULL)
            {
              error (0, errno, _("cannot read symbolic link %s"),
                     quoteaf (filename));
              врати true;
            }
          printf (" -> ");
          out_string (pformat, prefix_len, quoteN (linkname));
          free (linkname);
        }
      break;
    case 'd':
      ако (mod == 'H')
        out_uint (pformat, prefix_len, major (statbuf->st_dev));
      инако ако (mod == 'L')
        out_uint (pformat, prefix_len, minor (statbuf->st_dev));
      инако
        out_uint (pformat, prefix_len, statbuf->st_dev);
      break;
    case 'D':
      out_uint_x (pformat, prefix_len, statbuf->st_dev);
      break;
    case 'i':
      out_uint (pformat, prefix_len, statbuf->st_ino);
      break;
    case 'a':
      out_uint_o (pformat, prefix_len, statbuf->st_mode & CHMOD_MODE_BITS);
      break;
    case 'A':
      out_string (pformat, prefix_len, human_access (statbuf));
      break;
    case 'f':
      out_uint_x (pformat, prefix_len, statbuf->st_mode);
      break;
    case 'F':
      out_string (pformat, prefix_len, file_type (statbuf));
      break;
    case 'h':
      out_uint (pformat, prefix_len, statbuf->st_nlink);
      break;
    case 'u':
      out_uint (pformat, prefix_len, statbuf->st_uid);
      break;
    case 'U':
      pw_ent = getpwuid (statbuf->st_uid);
      out_string (pformat, prefix_len,
                  pw_ent ? pw_ent->pw_name : "UNKNOWN");
      break;
    case 'g':
      out_uint (pformat, prefix_len, statbuf->st_gid);
      break;
    case 'G':
      gw_ent = getgrgid (statbuf->st_gid);
      out_string (pformat, prefix_len,
                  gw_ent ? gw_ent->gr_name : "UNKNOWN");
      break;
    case 'm':
      fail |= out_mount_point (filename, pformat, prefix_len, statbuf);
      break;
    case 's':
      out_uint (pformat, prefix_len, unsigned_file_size (statbuf->st_size));
      break;
    case 'r':
      ако (mod == 'H')
        out_uint (pformat, prefix_len, major (statbuf->st_rdev));
      инако ако (mod == 'L')
        out_uint (pformat, prefix_len, minor (statbuf->st_rdev));
      инако
        out_uint (pformat, prefix_len, statbuf->st_rdev);
      break;
    case 'R':
      out_uint_x (pformat, prefix_len, statbuf->st_rdev);
      break;
    case 't':
      out_uint_x (pformat, prefix_len, major (statbuf->st_rdev));
      break;
    case 'T':
      out_uint_x (pformat, prefix_len, minor (statbuf->st_rdev));
      break;
    case 'B':
      out_uint (pformat, prefix_len, ST_NBLOCKSIZE);
      break;
    case 'b':
      out_uint (pformat, prefix_len, STP_NBLOCKS (statbuf));
      break;
    case 'o':
      out_uint (pformat, prefix_len, STP_BLKSIZE (statbuf));
      break;
    case 'w':
      {
#ако ! USE_STATX
        btime = get_birthtime (fd, filename, statbuf);
#endif
        ако (btime.tv_nsec < 0)
          out_string (pformat, prefix_len, "-");
        инако
          out_string (pformat, prefix_len, human_time (btime));
      }
      break;
    case 'W':
      {
#ако ! USE_STATX
        btime = get_birthtime (fd, filename, statbuf);
#endif
        out_epoch_sec (pformat, prefix_len, neg_to_zero (btime));
      }
      break;
    case 'x':
      out_string (pformat, prefix_len, human_time (get_stat_atime (statbuf)));
      break;
    case 'X':
      out_epoch_sec (pformat, prefix_len, get_stat_atime (statbuf));
      break;
    case 'y':
      out_string (pformat, prefix_len, human_time (get_stat_mtime (statbuf)));
      break;
    case 'Y':
      out_epoch_sec (pformat, prefix_len, get_stat_mtime (statbuf));
      break;
    case 'z':
      out_string (pformat, prefix_len, human_time (get_stat_ctime (statbuf)));
      break;
    case 'Z':
      out_epoch_sec (pformat, prefix_len, get_stat_ctime (statbuf));
      break;
    case 'C':
      fail |= out_file_context (pformat, prefix_len, filename);
      break;
    default:
      fputc ('?', stdout);
      break;
    }
  врати fail;
}

/* Return an allocated format string in static storage that
   corresponds to whether FS and TERSE options were declared.  */
static char *
default_format (bool fs, bool terse, bool device)
{
  char *format;
  ако (fs)
    {
      ако (terse)
        format = xstrdup (fmt_terse_fs);
      инако
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
  инако /* ! fs */
    {
      ако (terse)
        {
          ако (0 < is_selinux_enabled ())
            format = xstrdup (fmt_terse_selinux);
          инако
            format = xstrdup (fmt_terse_regular);
        }
      инако
        {
          char *temp;
          /* TRANSLATORS: This string uses format specifiers from
             'stat --help' without --file-system, and NOT from printf.  */
          format = xstrdup (_("\
  File: %N\n\
  Size: %-10s\tBlocks: %-10b IO Block: %-6o %F\n\
"));

          temp = format;
          ако (device)
            {
              /* TRANSLATORS: This string uses format specifiers from
                 'stat --help' without --file-system, and NOT from printf.  */
              format = xasprintf ("%s%s", format, _("\
" "Device: %Hd,%Ld\tInode: %-10i  Links: %-5h Device type: %Hr,%Lr\n\
"));
            }
          инако
            {
              /* TRANSLATORS: This string uses format specifiers from
                 'stat --help' without --file-system, and NOT from printf.  */
              format = xasprintf ("%s%s", format, _("\
" "Device: %Hd,%Ld\tInode: %-10i  Links: %h\n\
"));
            }
          free (temp);

          temp = format;
          /* TRANSLATORS: This string uses format specifiers from
             'stat --help' without --file-system, and NOT from printf.  */
          format = xasprintf ("%s%s", format, _("\
" "Access: (%04a/%10.10A)  Uid: (%5u/%8U)   Gid: (%5g/%8G)\n\
"));
          free (temp);

          ако (0 < is_selinux_enabled ())
            {
              temp = format;
              /* TRANSLATORS: This string uses format specifiers from
                 'stat --help' without --file-system, and NOT from printf.  */
              format = xasprintf ("%s%s", format, _("Context: %C\n"));
              free (temp);
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
          free (temp);
        }
    }
  врати format;
}

void
usage (цео status)
{
  ако (status != EXIT_SUCCESS)
    emit_try_help ();
  инако
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
         use the specified FORMAT instead of the default;\n\
         output a newline after each use of FORMAT\n\
"));
      oputs (_("\
      --printf=FORMAT\n\
         like --format, but interpret backslash escapes,\n\
         and do not output a mandatory trailing newline;\n\
         ако you want a newline, include \\n in FORMAT\n\
"));
      oputs (_("\
  -t, --terse\n\
         print the information in terse form\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);

      fputs (_("\n\
The MODE argument of --cached can be: always, never, or default.\n\
'always' will use cached attributes ако available, док\n\
'never' will try to synchronize with the latest attributes, and\n\
'default' will leave it up to the underlying file system.\n\
"), stdout);

      fputs (_("\n\
The valid format sequences за files (without --file-system):\n\
\n\
  %a   permission bits in octal (see '#' and '0' printf flags)\n\
  %A   permission bits and file type in human readable form\n\
  %b   number of blocks allocated (see %B)\n\
  %B   the size in bytes of each block reported by %b\n\
  %C   SELinux security context string\n\
"), stdout);
      fputs (_("\
  %d   device number in decimal (st_dev)\n\
  %D   device number in hex (st_dev)\n\
  %Hd  major device number in decimal\n\
  %Ld  minor device number in decimal\n\
  %f   raw mode in hex\n\
  %F   file type\n\
  %g   group ID of owner\n\
  %G   group name of owner\n\
"), stdout);
      fputs (_("\
  %h   number of hard links\n\
  %i   inode number\n\
  %m   mount point\n\
  %n   file name\n\
  %Qn  quoted file name\n\
  %N   quoted file name with dereference ако symbolic link\n\
  %o   optimal I/O transfer size hint\n\
  %s   total size, in bytes\n\
  %r   device type in decimal (st_rdev)\n\
  %R   device type in hex (st_rdev)\n\
  %Hr  major device type in decimal, за character/block device special files\n\
  %Lr  minor device type in decimal, за character/block device special files\n\
  %t   major device type in hex, за character/block device special files\n\
  %T   minor device type in hex, за character/block device special files\n\
"), stdout);
      fputs (_("\
  %u   user ID of owner\n\
  %U   user name of owner\n\
  %w   time of file birth, human-readable; - ако unknown\n\
  %W   time of file birth, seconds since Epoch; 0 ако unknown\n\
  %x   time of last access, human-readable\n\
  %X   time of last access, seconds since Epoch\n\
  %y   time of last data modification, human-readable\n\
  %Y   time of last data modification, seconds since Epoch\n\
  %z   time of last status change, human-readable\n\
  %Z   time of last status change, seconds since Epoch\n\
\n\
"), stdout);

      fputs (_("\
Valid format sequences за file systems:\n\
\n\
  %a   free blocks available to non-superuser\n\
  %b   total data blocks in file system\n\
  %c   total file nodes in file system\n\
  %d   free file nodes in file system\n\
  %f   free blocks in file system\n\
"), stdout);
      fputs (_("\
  %i   file system ID in hex\n\
  %l   maximum length of filenames\n\
  %n   file name\n\
  %Qn  quoted file name\n\
  %s   block size (за faster transfers)\n\
  %S   fundamental block size (за block counts)\n\
  %t   file system type in hex\n\
  %T   file system type in human readable form\n\
"), stdout);

      printf (_("\n\
--terse is equivalent to the following FORMAT:\n\
    %s\
"),
#ако HAVE_SELINUX_SELINUX_H
              fmt_terse_selinux
#инако
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

цео
main (цео argc, char *argv[])
{
  цео c;
  bool fs = false;
  bool terse = false;
  char *format = NULL;
  char *format2;
  bool ok = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  struct lconv const *locale = localeconv ();
  decimal_point = (locale->decimal_point[0] ? locale->decimal_point : ".");
  decimal_point_len = strlen (decimal_point);

  atexit (close_stdout);

  док ((c = getopt_long (argc, argv, "c:fLt", long_options, NULL)) != -1)
    {
      switch (c)
        {
        case PRINTF_OPTION:
          format = optarg;
          interpret_backslash_escapes = true;
          trailing_delim = "";
          break;

        case 'c':
          format = optarg;
          interpret_backslash_escapes = false;
          trailing_delim = "\n";
          break;

        case 'L':
          follow_links = true;
          break;

        case 'f':
          fs = true;
          break;

        case 't':
          terse = true;
          break;

        case 0:
          switch (XARGMATCH ("--cached", optarg, cached_args, cached_modes))
            {
              case cached_never:
                force_sync = true;
                dont_sync = false;
                break;
              case cached_always:
                force_sync = false;
                dont_sync = true;
                break;
              case cached_default:
                force_sync = false;
                dont_sync = false;
            }
          break;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        default:
          usage (EXIT_FAILURE);
        }
    }

  ако (argc == optind)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  ако (format)
    {
      format2 = format;
    }
  инако
    {
      format = default_format (fs, terse, /* device= */ false);
      format2 = default_format (fs, terse, /* device= */ true);
    }

  за (цео i = optind; i < argc; i++)
    ok &= (fs
           ? do_statfs (argv[i], format)
           : do_stat (argv[i], format, format2));

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
