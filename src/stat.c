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
#якщо ((STAT_STATVFS || STAT_STATVFS64)                                       \
     && (HAVE_STRUCT_STATVFS_F_BASETYPE || HAVE_STRUCT_STATVFS_F_FSTYPENAME \
         || (! HAVE_STRUCT_STATFS_F_FSTYPENAME && HAVE_STRUCT_STATVFS_F_TYPE)))
# define USE_STATVFS 1
#інакше
# define USE_STATVFS 0
#endif

#include <stdio.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#якщо USE_STATVFS
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
# якщо HAVE_NFS_NFS_CLNT_H && HAVE_NFS_VFS_H
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

#якщо HAVE_STATX && defined STATX_INO
# define USE_STATX 1
#інакше
# define USE_STATX 0
#endif

#якщо USE_STATVFS
# define STRUCT_STATXFS_F_FSID_IS_INTEGER STRUCT_STATVFS_F_FSID_IS_INTEGER
# define HAVE_STRUCT_STATXFS_F_TYPE HAVE_STRUCT_STATVFS_F_TYPE
# якщо HAVE_STRUCT_STATVFS_F_NAMEMAX
#  define SB_F_NAMEMAX(S) ((S)->f_namemax)
# endif
# якщо ! STAT_STATVFS && STAT_STATVFS64
#  define STRUCT_STATVFS struct statvfs64
#  define STATFS statvfs64
# інакше
#  define STRUCT_STATVFS struct statvfs
#  define STATFS statvfs
# endif
# define STATFS_FRSIZE(S) ((S)->f_frsize)
#інакше
# define HAVE_STRUCT_STATXFS_F_TYPE HAVE_STRUCT_STATFS_F_TYPE
# якщо HAVE_STRUCT_STATFS_F_NAMELEN
#  define SB_F_NAMEMAX(S) ((S)->f_namelen)
# elif HAVE_STRUCT_STATFS_F_NAMEMAX
#  define SB_F_NAMEMAX(S) ((S)->f_namemax)
# endif
# define STATFS statfs
# якщо HAVE_OS_H /* BeOS */
/* BeOS has a statvfs function, but it does not return sensible values
   for f_files, f_ffree and f_favail, and lacks f_type, f_basetype and
   f_fstypename.  Use 'struct fs_info' instead.  */
NODISCARD
static ціле
statfs (char const *filename, struct fs_info *buf)
{
  dev_t device = dev_for_path (filename);
  якщо (device < 0)
    {
      errno = (device == B_ENTRY_NOT_FOUND ? ENOENT
               : device == B_BAD_VALUE ? EINVAL
               : device == B_NAME_TOO_LONG ? ENAMETOOLONG
               : device == B_NO_MEMORY ? ENOMEM
               : device == B_FILE_ERROR ? EIO
               : 0);
      повернути -1;
    }
  /* If successful, buf->dev will be == device.  */
  повернути fs_stat_dev (device, buf);
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
# інакше
#  define STRUCT_STATVFS struct statfs
#  define STRUCT_STATXFS_F_FSID_IS_INTEGER STRUCT_STATFS_F_FSID_IS_INTEGER
#  якщо HAVE_STRUCT_STATFS_F_FRSIZE
#   define STATFS_FRSIZE(S) ((S)->f_frsize)
#  інакше
#   define STATFS_FRSIZE(S) 0
#  endif
# endif
#endif

#ifdef SB_F_NAMEMAX
# define OUT_NAMEMAX out_uint
#інакше
/* Depending on whether statvfs or statfs is used,
   neither f_namemax or f_namelen may be available.  */
# define SB_F_NAMEMAX(S) "?"
# define OUT_NAMEMAX out_string
#endif

#якщо HAVE_STRUCT_STATVFS_F_BASETYPE
# define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME f_basetype
#інакше
# якщо HAVE_STRUCT_STATVFS_F_FSTYPENAME || HAVE_STRUCT_STATFS_F_FSTYPENAME
#  define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME f_fstypename
# elif HAVE_OS_H /* BeOS */
#  define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME fsh_name
# endif
#endif

#якщо HAVE_GETATTRAT
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
            ціле fd, char const *filename, void const *data);

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
  повернути statfsbuf->STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME;
#інакше
  switch (statfsbuf->f_type)
    {
# якщо defined __linux__ || defined __ANDROID__

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
      повернути "aafs";
    case S_MAGIC_ACFS: /* 0x61636673 remote */
      повернути "acfs";
    case S_MAGIC_ADFS: /* 0xADF5 local */
      повернути "adfs";
    case S_MAGIC_AFFS: /* 0xADFF local */
      повернути "affs";
    case S_MAGIC_AFS: /* 0x5346414F remote */
      повернути "afs";
    case S_MAGIC_ANON_INODE_FS: /* 0x09041934 local */
      повернути "anon-inode FS";
    case S_MAGIC_AUFS: /* 0x61756673 remote */
      /* FIXME: change syntax or add an optional attribute like "inotify:no".
         The above is labeled as "remote" so that tail always uses polling,
         but this isn't really a remote file system type.  */
      повернути "aufs";
    case S_MAGIC_AUTOFS: /* 0x0187 local */
      повернути "autofs";
    case S_MAGIC_BALLOON_KVM: /* 0x13661366 local */
      повернути "balloon-kvm-fs";
    case S_MAGIC_BCACHEFS: /* 0xCA451A4E local */
      повернути "bcachefs";
    case S_MAGIC_BEFS: /* 0x42465331 local */
      повернути "befs";
    case S_MAGIC_BDEVFS: /* 0x62646576 local */
      повернути "bdevfs";
    case S_MAGIC_BFS: /* 0x1BADFACE local */
      повернути "bfs";
    case S_MAGIC_BINDERFS: /* 0x6C6F6F70 local */
      повернути "binderfs";
    case S_MAGIC_BPF_FS: /* 0xCAFE4A11 local */
      повернути "bpf_fs";
    case S_MAGIC_BINFMTFS: /* 0x42494E4D local */
      повернути "binfmt_misc";
    case S_MAGIC_BTRFS: /* 0x9123683E local */
      повернути "btrfs";
    case S_MAGIC_BTRFS_TEST: /* 0x73727279 local */
      повернути "btrfs_test";
    case S_MAGIC_CEPH: /* 0x00C36400 remote */
      повернути "ceph";
    case S_MAGIC_CGROUP: /* 0x0027E0EB local */
      повернути "cgroupfs";
    case S_MAGIC_CGROUP2: /* 0x63677270 local */
      повернути "cgroup2fs";
    case S_MAGIC_CIFS: /* 0xFF534D42 remote */
      повернути "cifs";
    case S_MAGIC_CODA: /* 0x73757245 remote */
      повернути "coda";
    case S_MAGIC_COH: /* 0x012FF7B7 local */
      повернути "coh";
    case S_MAGIC_CONFIGFS: /* 0x62656570 local */
      повернути "configfs";
    case S_MAGIC_CRAMFS: /* 0x28CD3D45 local */
      повернути "cramfs";
    case S_MAGIC_CRAMFS_WEND: /* 0x453DCD28 local */
      повернути "cramfs-wend";
    case S_MAGIC_DAXFS: /* 0x64646178 local */
      повернути "daxfs";
    case S_MAGIC_DEBUGFS: /* 0x64626720 local */
      повернути "debugfs";
    case S_MAGIC_DEVFS: /* 0x1373 local */
      повернути "devfs";
    case S_MAGIC_DEVMEM: /* 0x454D444D local */
      повернути "devmem";
    case S_MAGIC_DEVPTS: /* 0x1CD1 local */
      повернути "devpts";
    case S_MAGIC_DMA_BUF: /* 0x444D4142 local */
      повернути "dma-buf-fs";
    case S_MAGIC_ECRYPTFS: /* 0xF15F local */
      повернути "ecryptfs";
    case S_MAGIC_EFIVARFS: /* 0xDE5E81E4 local */
      повернути "efivarfs";
    case S_MAGIC_EFS: /* 0x00414A53 local */
      повернути "efs";
    case S_MAGIC_EROFS_V1: /* 0xE0F5E1E2 local */
      повернути "erofs";
    case S_MAGIC_EXFAT: /* 0x2011BAB0 local */
      повернути "exfat";
    case S_MAGIC_EXFS: /* 0x45584653 local */
      повернути "exfs";
    case S_MAGIC_EXOFS: /* 0x5DF5 local */
      повернути "exofs";
    case S_MAGIC_EXT: /* 0x137D local */
      повернути "ext";
    case S_MAGIC_EXT2: /* 0xEF53 local */
      повернути "ext2/ext3";
    case S_MAGIC_EXT2_OLD: /* 0xEF51 local */
      повернути "ext2";
    case S_MAGIC_F2FS: /* 0xF2F52010 local */
      повернути "f2fs";
    case S_MAGIC_FAIL_FS: /* 0x4641494C local */
      повернути "failfs";
    case S_MAGIC_FAT: /* 0x4006 local */
      повернути "fat";
    case S_MAGIC_FHGFS: /* 0x19830326 remote */
      повернути "fhgfs";
    case S_MAGIC_FUSE: /* 0x65735546 remote */
      повернути "fuse";
    case S_MAGIC_FUSECTL: /* 0x65735543 remote */
      повернути "fusectl";
    case S_MAGIC_FUTEXFS: /* 0x0BAD1DEA local */
      повернути "futexfs";
    case S_MAGIC_GFS: /* 0x01161970 remote */
      повернути "gfs/gfs2";
    case S_MAGIC_GPFS: /* 0x47504653 remote */
      повернути "gpfs";
    case S_MAGIC_GUEST_MEMFD: /* 0x474D454D remote */
      повернути "guest-memfd";
    case S_MAGIC_HFS: /* 0x4244 local */
      повернути "hfs";
    case S_MAGIC_HFS_PLUS: /* 0x482B local */
      повернути "hfs+";
    case S_MAGIC_HFS_X: /* 0x4858 local */
      повернути "hfsx";
    case S_MAGIC_HOSTFS: /* 0x00C0FFEE local */
      повернути "hostfs";
    case S_MAGIC_HPFS: /* 0xF995E849 local */
      повернути "hpfs";
    case S_MAGIC_HUGETLBFS: /* 0x958458F6 local */
      повернути "hugetlbfs";
    case S_MAGIC_MTD_INODE_FS: /* 0x11307854 local */
      повернути "inodefs";
    case S_MAGIC_IBRIX: /* 0x013111A8 remote */
      повернути "ibrix";
    case S_MAGIC_INOTIFYFS: /* 0x2BAD1DEA local */
      повернути "inotifyfs";
    case S_MAGIC_ISOFS: /* 0x9660 local */
      повернути "isofs";
    case S_MAGIC_ISOFS_R_WIN: /* 0x4004 local */
      повернути "isofs";
    case S_MAGIC_ISOFS_WIN: /* 0x4000 local */
      повернути "isofs";
    case S_MAGIC_JFFS: /* 0x07C0 local */
      повернути "jffs";
    case S_MAGIC_JFFS2: /* 0x72B6 local */
      повернути "jffs2";
    case S_MAGIC_JFS: /* 0x3153464A local */
      повернути "jfs";
    case S_MAGIC_KAFS: /* 0x6B414653 remote */
      повернути "k-afs";
    case S_MAGIC_LOGFS: /* 0xC97E8168 local */
      повернути "logfs";
    case S_MAGIC_LUSTRE: /* 0x0BD00BD0 remote */
      повернути "lustre";
    case S_MAGIC_M1FS: /* 0x5346314D local */
      повернути "m1fs";
    case S_MAGIC_MINIX: /* 0x137F local */
      повернути "minix";
    case S_MAGIC_MINIX_30: /* 0x138F local */
      повернути "minix (30 char.)";
    case S_MAGIC_MINIX_V2: /* 0x2468 local */
      повернути "minix v2";
    case S_MAGIC_MINIX_V2_30: /* 0x2478 local */
      повернути "minix v2 (30 char.)";
    case S_MAGIC_MINIX_V3: /* 0x4D5A local */
      повернути "minix3";
    case S_MAGIC_MQUEUE: /* 0x19800202 local */
      повернути "mqueue";
    case S_MAGIC_MSDOS: /* 0x4D44 local */
      повернути "msdos";
    case S_MAGIC_NCP: /* 0x564C remote */
      повернути "novell";
    case S_MAGIC_NFS: /* 0x6969 remote */
      повернути "nfs";
    case S_MAGIC_NFSD: /* 0x6E667364 remote */
      повернути "nfsd";
    case S_MAGIC_NILFS: /* 0x3434 local */
      повернути "nilfs";
    case S_MAGIC_NSFS: /* 0x6E736673 local */
      повернути "nsfs";
    case S_MAGIC_NTFS: /* 0x5346544E local */
      повернути "ntfs";
    case S_MAGIC_NULL_FS: /* 0x4E554C4C local */
      повернути "nullfs";
    case S_MAGIC_OPENPROM: /* 0x9FA1 local */
      повернути "openprom";
    case S_MAGIC_OCFS2: /* 0x7461636F remote */
      повернути "ocfs2";
    case S_MAGIC_OVERLAYFS: /* 0x794C7630 remote */
      /* This may overlay remote file systems.
         Also there have been issues reported with inotify and overlayfs,
         so mark as "remote" so that polling is used.  */
      повернути "overlayfs";
    case S_MAGIC_PANFS: /* 0xAAD7AAEA remote */
      повернути "panfs";
    case S_MAGIC_PID_FS: /* 0x50494446 local */
      повернути "pidfs";
    case S_MAGIC_PIPEFS: /* 0x50495045 remote */
      /* FIXME: change syntax or add an optional attribute like "inotify:no".
         pipefs is labeled as "remote" so that tail always polls,
         but it isn't really remote file system type.  */
      повернути "pipefs";
    case S_MAGIC_PPC_CMM: /* 0xC7571590 local */
      повернути "ppc-cmm-fs";
    case S_MAGIC_PRL_FS: /* 0x7C7C6673 remote */
      повернути "prl_fs";
    case S_MAGIC_PROC: /* 0x9FA0 local */
      повернути "proc";
    case S_MAGIC_PSTOREFS: /* 0x6165676C local */
      повернути "pstorefs";
    case S_MAGIC_QNX4: /* 0x002F local */
      повернути "qnx4";
    case S_MAGIC_QNX6: /* 0x68191122 local */
      повернути "qnx6";
    case S_MAGIC_RAMFS: /* 0x858458F6 local */
      повернути "ramfs";
    case S_MAGIC_RDTGROUP: /* 0x07655821 local */
      повернути "rdt";
    case S_MAGIC_REISERFS: /* 0x52654973 local */
      повернути "reiserfs";
    case S_MAGIC_ROMFS: /* 0x7275 local */
      повернути "romfs";
    case S_MAGIC_RPC_PIPEFS: /* 0x67596969 local */
      повернути "rpc_pipefs";
    case S_MAGIC_SDCARDFS: /* 0x5DCA2DF5 local */
      повернути "sdcardfs";
    case S_MAGIC_SECRETMEM: /* 0x5345434D local */
      повернути "secretmem";
    case S_MAGIC_SECURITYFS: /* 0x73636673 local */
      повернути "securityfs";
    case S_MAGIC_SELINUX: /* 0xF97CFF8C local */
      повернути "selinux";
    case S_MAGIC_SMACK: /* 0x43415D53 local */
      повернути "smackfs";
    case S_MAGIC_SMB: /* 0x517B remote */
      повернути "smb";
    case S_MAGIC_SMB2: /* 0xFE534D42 remote */
      повернути "smb2";
    case S_MAGIC_SNFS: /* 0xBEEFDEAD remote */
      повернути "snfs";
    case S_MAGIC_SOCKFS: /* 0x534F434B local */
      повернути "sockfs";
    case S_MAGIC_SQUASHFS: /* 0x73717368 local */
      повернути "squashfs";
    case S_MAGIC_SYSFS: /* 0x62656572 local */
      повернути "sysfs";
    case S_MAGIC_SYSV2: /* 0x012FF7B6 local */
      повернути "sysv2";
    case S_MAGIC_SYSV4: /* 0x012FF7B5 local */
      повернути "sysv4";
    case S_MAGIC_TMPFS: /* 0x01021994 local */
      повернути "tmpfs";
    case S_MAGIC_TRACEFS: /* 0x74726163 local */
      повернути "tracefs";
    case S_MAGIC_UBIFS: /* 0x24051905 local */
      повернути "ubifs";
    case S_MAGIC_UDF: /* 0x15013346 local */
      повернути "udf";
    case S_MAGIC_UFS: /* 0x00011954 local */
      повернути "ufs";
    case S_MAGIC_UFS_BYTESWAPPED: /* 0x54190100 local */
      повернути "ufs";
    case S_MAGIC_USBDEVFS: /* 0x9FA2 local */
      повернути "usbdevfs";
    case S_MAGIC_V9FS: /* 0x01021997 local */
      повернути "v9fs";
    case S_MAGIC_VBOXSF: /* 0x786F4256 remote */
      повернути "vboxsf";
    case S_MAGIC_VMHGFS: /* 0xBACBACBC remote */
      повернути "vmhgfs";
    case S_MAGIC_VXFS: /* 0xA501FCF5 remote */
      /* Veritas File System can run in single instance or clustered mode,
         so mark as remote to cater for the latter case.  */
      повернути "vxfs";
    case S_MAGIC_VZFS: /* 0x565A4653 local */
      повернути "vzfs";
    case S_MAGIC_WSLFS: /* 0x53464846 local */
      повернути "wslfs";
    case S_MAGIC_XENFS: /* 0xABBA1974 local */
      повернути "xenfs";
    case S_MAGIC_XENIX: /* 0x012FF7B4 local */
      повернути "xenix";
    case S_MAGIC_XFS: /* 0x58465342 local */
      повернути "xfs";
    case S_MAGIC_XIAFS: /* 0x012FD16D local */
      повернути "xia";
    case S_MAGIC_Z3FOLD: /* 0x0033 local */
      повернути "z3fold";
    case S_MAGIC_ZFS: /* 0x2FC12FC1 local */
      повернути "zfs";
    case S_MAGIC_ZONEFS: /* 0x5A4F4653 local */
      повернути "zonefs";
    case S_MAGIC_ZSMALLOC: /* 0x58295829 local */
      повернути "zsmallocfs";


# elif __GNU__
    case FSTYPE_UFS:
      повернути "ufs";
    case FSTYPE_NFS:
      повернути "nfs";
    case FSTYPE_GFS:
      повернути "gfs";
    case FSTYPE_LFS:
      повернути "lfs";
    case FSTYPE_SYSV:
      повернути "sysv";
    case FSTYPE_FTP:
      повернути "ftp";
    case FSTYPE_TAR:
      повернути "tar";
    case FSTYPE_AR:
      повернути "ar";
    case FSTYPE_CPIO:
      повернути "cpio";
    case FSTYPE_MSLOSS:
      повернути "msloss";
    case FSTYPE_CPM:
      повернути "cpm";
    case FSTYPE_HFS:
      повернути "hfs";
    case FSTYPE_DTFS:
      повернути "dtfs";
    case FSTYPE_GRFS:
      повернути "grfs";
    case FSTYPE_TERM:
      повернути "term";
    case FSTYPE_DEV:
      повернути "dev";
    case FSTYPE_PROC:
      повернути "proc";
    case FSTYPE_IFSOCK:
      повернути "ifsock";
    case FSTYPE_AFS:
      повернути "afs";
    case FSTYPE_DFS:
      повернути "dfs";
    case FSTYPE_PROC9:
      повернути "proc9";
    case FSTYPE_SOCKET:
      повернути "socket";
    case FSTYPE_MISC:
      повернути "misc";
    case FSTYPE_EXT2FS:
      повернути "ext2/ext3";
    case FSTYPE_HTTP:
      повернути "http";
    case FSTYPE_MEMFS:
      повернути "memfs";
    case FSTYPE_ISO9660:
      повернути "iso9660";
# endif
    default:
      {
        unsigned long ціле type = statfsbuf->f_type;
        static char buf[sizeof "UNKNOWN (0x%lx)" - 3
                        + (sizeof type * CHAR_BIT + 3) / 4];
        sprintf (buf, "UNKNOWN (0x%lx)", type);
        повернути buf;
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
  повернути modebuf;
}

NODISCARD
static char *
human_time (struct timespec t)
{
  /* STR must be at least INT_BUFSIZE_BOUND (intmax_t) big, either
     because localtime_rz fails, or because the time zone is truly
     outlandish so that %z expands to a long string.  */
  static char str[INT_BUFSIZE_BOUND (intmax_t)
                  + INT_STRLEN_BOUND (ціле) /* YYYY */
                  + 1 /* because YYYY might equal INT_MAX + 1900 */
                  + sizeof "-MM-DD HH:MM:SS.NNNNNNNNN +"];
  static timezone_t tz;
  якщо (!tz)
    tz = tzalloc (getenv ("TZ"));
  struct tm tm;
  ціле ns = t.tv_nsec;
  якщо (localtime_rz (tz, &t.tv_sec, &tm))
    nstrftime (str, sizeof str, "%Y-%m-%d %H:%M:%S.%N %z", &tm, tz, ns);
  інакше
    {
      char secbuf[INT_BUFSIZE_BOUND (intmax_t)];
      sprintf (str, "%s.%09d", timetostr (t.tv_sec, secbuf), ns);
    }
  повернути str;
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
  для (src = dst; src < srclim && strchr (printf_flags, *src); src++)
    якщо (strchr (allowed_flags, *src))
      *dst++ = *src;
  поки (src < srclim)
    *dst++ = *src++;
  strcpy (dst, suffix);
}

static void
out_string (char *pformat, size_t prefix_len, char const *arg)
{
  make_format (pformat, prefix_len, "-", "s");
  printf (pformat, arg);
}
static ціле
out_int (char *pformat, size_t prefix_len, intmax_t arg)
{
  make_format (pformat, prefix_len, "'-+ 0", "jd");
  повернути printf (pformat, arg);
}
static ціле
out_uint (char *pformat, size_t prefix_len, uintmax_t arg)
{
  make_format (pformat, prefix_len, "'-0", "ju");
  повернути printf (pformat, arg);
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
static ціле
out_minus_zero (char *pformat, size_t prefix_len)
{
  make_format (pformat, prefix_len, "'-+ 0", ".0f");
  повернути printf (pformat, -0.25);
}

/* Output the number of seconds since the Epoch, using a format that
   acts like printf's %f format.  */
static void
out_epoch_sec (char *pformat, size_t prefix_len,
               struct timespec arg)
{
  char *dot = memchr (pformat, '.', prefix_len);
  size_t sec_prefix_len = prefix_len;
  ціле width = 0;
  ціле precision = 0;
  bool frac_left_adjust = false;

  якщо (dot)
    {
      sec_prefix_len = dot - pformat;
      pformat[prefix_len] = '\0';

      якщо (c_isdigit (dot[1]))
        {
          long ціле lprec = strtol (dot + 1, NULL, 10);
          precision = (lprec <= INT_MAX ? lprec : INT_MAX);
        }
      інакше
        {
          precision = 9;
        }

      якщо (precision && c_isdigit (dot[-1]))
        {
          /* If a nontrivial width is given, subtract the width of the
             decimal point and PRECISION digits that will be output
             later.  */
          char *p = dot;
          *dot = '\0';

          do
            --p;
          поки (c_isdigit (p[-1]));

          long ціле lwidth = strtol (p, NULL, 10);
          width = (lwidth <= INT_MAX ? lwidth : INT_MAX);
          якщо (1 < width)
            {
              p += (*p == '0');
              sec_prefix_len = p - pformat;
              ціле w_d = (decimal_point_len < width
                         ? width - decimal_point_len
                         : 0);
              якщо (1 < w_d)
                {
                  ціле w = w_d - precision;
                  якщо (1 < w)
                    {
                      char *dst = pformat;
                      для (char const *src = dst; src < p; src++)
                        {
                          якщо (*src == '-')
                            frac_left_adjust = true;
                          інакше
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

  ціле divisor = 1;
  для (ціле i = precision; i < 9; i++)
    divisor *= 10;
  ціле frac_sec = arg.tv_nsec / divisor;
  ціле int_len;

  якщо (TYPE_SIGNED (time_t))
    {
      bool minus_zero = false;
      якщо (arg.tv_sec < 0 && arg.tv_nsec != 0)
        {
          ціле frac_sec_modulus = 1000000000 / divisor;
          frac_sec = (frac_sec_modulus - frac_sec
                      - (arg.tv_nsec % divisor != 0));
          arg.tv_sec += (frac_sec != 0);
          minus_zero = (arg.tv_sec == 0);
        }
      int_len = (minus_zero
                 ? out_minus_zero (pformat, sec_prefix_len)
                 : out_int (pformat, sec_prefix_len, arg.tv_sec));
    }
  інакше
    int_len = out_uint (pformat, sec_prefix_len, arg.tv_sec);

  якщо (precision)
    {
      ціле prec = (precision < 9 ? precision : 9);
      ціле trailing_prec = precision - prec;
      ціле ilen = (int_len < 0 ? 0 : int_len);
      ціле trailing_width = (ilen < width && decimal_point_len < width - ilen
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

  якщо ((follow_links
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
  якщо (scontext)
    freecon (scontext);
  повернути fail;
}

/* Set the quoting style once it is needed.  */
static void
initialize_quoting_style (void)
{
  static bool initialized;
  якщо (!initialized)
    {
      initialized = true;
      ціле qs = getenv_quoting_style ();
      set_quoting_style (NULL, 0 <= qs ? qs : shell_escape_quoting_style);
    }
}

/* Print statfs info.  Return zero upon success, nonzero upon failure.  */
NODISCARD
static bool
print_statfs (char *pformat, size_t prefix_len, MAYBE_UNUSED char mod, char m,
              MAYBE_UNUSED ціле fd, char const *filename,
              void const *data)
{
  STRUCT_STATVFS const *statfsbuf = data;
  bool fail = false;

  switch (m)
    {
    case 'n':
      якщо (mod == 'Q')
        {
          initialize_quoting_style ();
          filename = quoteN (filename);
        }
      out_string (pformat, prefix_len, filename);
      break;

    case 'i':
      {
#якщо STRUCT_STATXFS_F_FSID_IS_INTEGER
        uintmax_t fsid = statfsbuf->f_fsid;
#інакше
        typedef unsigned ціле fsid_word;
        static_assert (alignof (STRUCT_STATVFS) % alignof (fsid_word) == 0);
        static_assert (offsetof (STRUCT_STATVFS, f_fsid) % alignof (fsid_word)
                       == 0);
        static_assert (sizeof statfsbuf->f_fsid % alignof (fsid_word) == 0);
        fsid_word const *p = (fsid_word *) &statfsbuf->f_fsid;

        /* Assume a little-endian word order, as that is compatible
           with glibc's statvfs implementation.  */
        uintmax_t fsid = 0;
        ціле words = sizeof statfsbuf->f_fsid / sizeof *p;
        для (ціле i = 0; i < words && i * sizeof *p < sizeof fsid; i++)
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
#якщо HAVE_STRUCT_STATXFS_F_TYPE
      out_uint_x (pformat, prefix_len, statfsbuf->f_type);
#інакше
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
        якщо (! frsize)
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
  повернути fail;
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
  якщо (!tried_mount_list) /* attempt/warn once per process.  */
    {
      якщо (!(mount_list = read_file_system_list (false)))
        error (0, errno, "%s", _("cannot read table of mounted file systems"));
      tried_mount_list = true;
    }

  struct stat name_stats;
  якщо (stat (name, &name_stats) != 0)
    повернути NULL;

  для (struct mount_entry *me = mount_list; me; me = me->me_next)
    {
      якщо (me->me_dummy && me->me_devname[0] == '/'
          && streq (me->me_mountdir, name))
        {
          struct stat dev_stats;

          якщо (stat (me->me_devname, &dev_stats) == 0
              && psame_inode (&name_stats, &dev_stats))
            {
              bind_mount = me->me_devname;
              break;
            }
        }
    }

  повернути bind_mount;
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
  якщо (follow_links || !S_ISLNK (statp->st_mode))
    {
      char *resolved = canonicalize_file_name (filename);
      якщо (!resolved)
        {
          error (0, errno, _("failed to canonicalize %s"), quoteaf (filename));
          goto print_mount_point;
        }
      bp = find_bind_mount (resolved);
      free (resolved);
      якщо (bp)
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
  якщо ((mp = find_mount_point (filename, statp)))
    {
      /* This dir might be bind mounted to another device,
         so we resolve the bound source in that case also.  */
      bp = find_bind_mount (mp);
      fail = false;
    }

print_mount_point:

  out_string (pformat, prefix_len, bp ? bp : mp ? mp : np);
  free (mp);
  повернути fail;
}

/* Map a TS with negative TS.tv_nsec to {0,0}.  */
static inline struct timespec
neg_to_zero (struct timespec ts)
{
  якщо (0 <= ts.tv_nsec)
    повернути ts;
  struct timespec z = {0};
  повернути z;
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
  якщо (*fmt_char == '.')
    fmt_char += 1 + strspn (fmt_char + 1, digits);
  повернути fmt_char - directive;
}

/* Print the information specified by the format string, FORMAT,
   calling PRINT_FUNC for each %-directive encountered.
   Return zero upon success, nonzero upon failure.  */
NODISCARD
static bool
print_it (char const *format, ціле fd, char const *filename,
          bool (*print_func) (char *, size_t, char, char,
                              ціле, char const *, void const *),
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
  для (char const *b = format; *b; b++)
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
                якщо (1 < len)
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
                якщо ((mod_char == 'Q' && fmt_char == 'n')
                    || (print_func == print_stat
                        && (mod_char == 'H' || mod_char == 'L')
                        && (fmt_char == 'd' || fmt_char == 'r')))
                  {
                    b++;
                  }
                інакше
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
          якщо ( ! interpret_backslash_escapes)
            {
              putchar ('\\');
              break;
            }
          ++b;
          якщо (isoct (*b))
            {
              ціле esc_value = fromoct (*b);
              ціле esc_length = 1;	/* number of octal digits */
              для (++b; esc_length < 3 && isoct (*b);
                   ++esc_length, ++b)
                {
                  esc_value = esc_value * 8 + fromoct (*b);
                }
              putchar (esc_value);
              --b;
            }
          інакше якщо (*b == 'x' && c_isxdigit (b[1]))
            {
              ціле esc_value = fromhex (b[1]);	/* Value of \xhh escape. */
              /* A hexadecimal \xhh escape sequence must have
                 1 or 2 hex. digits.  */
              ++b;
              якщо (c_isxdigit (b[1]))
                {
                  ++b;
                  esc_value = esc_value * 16 + fromhex (*b);
                }
              putchar (esc_value);
            }
          інакше якщо (*b == '\0')
            {
              error (0, 0, _("warning: backslash at end of format"));
              putchar ('\\');
              /* Arrange to exit the loop.  */
              --b;
            }
          інакше
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

  повернути fail;
}

/* Stat the file system and print what we find.  */
NODISCARD
static bool
do_statfs (char const *filename, char const *format)
{
  STRUCT_STATVFS statfsbuf;

  якщо (streq (filename, "-"))
    {
      error (0, 0, _("using %s to denote standard input does not work"
                     " in file system mode"), quoteaf (filename));
      повернути false;
    }

  якщо (STATFS (filename, &statfsbuf) != 0)
    {
      error (0, errno, _("cannot read file system information for %s"),
             quoteaf (filename));
      повернути false;
    }

  bool fail = print_it (format, -1, filename, print_statfs, &statfsbuf);
  повернути ! fail;
}

struct print_args {
  struct stat *st;
  struct timespec btime;
};

/* Ask statx to avoid syncing? */
static bool dont_sync;

/* Ask statx to force sync? */
static bool force_sync;

#якщо USE_STATX
static unsigned ціле
fmt_to_mask (char fmt)
{
  switch (fmt)
    {
    case 'N':
      повернути STATX_MODE;
    case 'd':
    case 'D':
      повернути STATX_MODE;
    case 'i':
      повернути STATX_INO;
    case 'a':
    case 'A':
      повернути STATX_MODE;
    case 'f':
      повернути STATX_MODE|STATX_TYPE;
    case 'F':
      повернути STATX_TYPE;
    case 'h':
      повернути STATX_NLINK;
    case 'u':
    case 'U':
      повернути STATX_UID;
    case 'g':
    case 'G':
      повернути STATX_GID;
    case 'm':
      повернути STATX_MODE|STATX_INO;
    case 's':
      повернути STATX_SIZE;
    case 't':
    case 'T':
      повернути STATX_MODE;
    case 'b':
      повернути STATX_BLOCKS;
    case 'w':
    case 'W':
      повернути STATX_BTIME;
    case 'x':
    case 'X':
      повернути STATX_ATIME;
    case 'y':
    case 'Y':
      повернути STATX_MTIME;
    case 'z':
    case 'Z':
      повернути STATX_CTIME;
    }
  повернути 0;
}

ATTRIBUTE_PURE
static unsigned ціле
format_to_mask (char const *format)
{
  unsigned ціле mask = 0;

  для (char const *b = format; *b; b++)
    {
      якщо (*b != '%')
        continue;

      b += format_code_offset (b);
      якщо (*b == '\0')
        break;
      mask |= fmt_to_mask (*b);
    }
  повернути mask;
}

/* statx the file and print what we find */
NODISCARD
static bool
do_stat (char const *filename, char const *format, char const *format2)
{
  ціле fd = streq (filename, "-") ? 0 : AT_FDCWD;
  ціле flags = 0;
  struct stat st;
  struct statx stx = {0};
  char const *pathname = filename;
  struct print_args pa;
  pa.st = &st;
  pa.btime = (struct timespec) {.tv_sec = -1, .tv_nsec = -1};

  якщо (AT_FDCWD != fd)
    {
      pathname = "";
      flags = AT_EMPTY_PATH;
    }
  інакше якщо (!follow_links)
    {
      flags = AT_SYMLINK_NOFOLLOW;
    }

  якщо (dont_sync)
    flags |= AT_STATX_DONT_SYNC;
  інакше якщо (force_sync)
    flags |= AT_STATX_FORCE_SYNC;

  якщо (! force_sync)
    flags |= AT_NO_AUTOMOUNT;

  fd = statx (fd, pathname, flags, format_to_mask (format), &stx);
  якщо (fd < 0)
    {
      якщо (flags & AT_EMPTY_PATH)
        error (0, errno, _("cannot stat standard input"));
      інакше
        error (0, errno, _("cannot statx %s"), quoteaf (filename));
      повернути false;
    }

  якщо (S_ISBLK (stx.stx_mode) || S_ISCHR (stx.stx_mode))
    format = format2;

  statx_to_stat (&stx, &st);
  якщо (stx.stx_mask & STATX_BTIME)
    pa.btime = statx_timestamp_to_timespec (stx.stx_btime);

  bool fail = print_it (format, fd, filename, print_stat, &pa);
  повернути ! fail;
}

#інакше /* USE_STATX */

static struct timespec
get_birthtime (ціле fd, char const *filename, struct stat const *st)
{
  struct timespec ts = get_stat_birthtime (st);

# якщо HAVE_GETATTRAT
  якщо (ts.tv_nsec < 0)
    {
      nvlist_t *response;
      якщо ((fd < 0
           ? getattrat (AT_FDCWD, XATTR_VIEW_READWRITE, filename, &response)
           : fgetattr (fd, XATTR_VIEW_READWRITE, &response))
          == 0)
        {
          uint64_t *val;
          uint_t n;
          якщо (nvlist_lookup_uint64_array (response, A_CRTIME, &val, &n) == 0
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

  повернути ts;
}


/* stat the file and print what we find */
NODISCARD
static bool
do_stat (char const *filename, char const *format,
         char const *format2)
{
  ціле fd = streq (filename, "-") ? 0 : -1;
  struct stat statbuf;
  struct print_args pa;
  pa.st = &statbuf;
  pa.btime = (struct timespec) {.tv_sec = -1, .tv_nsec = -1};

  якщо (0 <= fd)
    {
      якщо (fstat (fd, &statbuf) != 0)
        {
          error (0, errno, _("cannot stat standard input"));
          повернути false;
        }
    }
  /* We can't use the shorter
     (follow_links?stat:lstat) (filename, &statbug)
     since stat might be a function-like macro.  */
  інакше якщо ((follow_links
            ? stat (filename, &statbuf)
            : lstat (filename, &statbuf)) != 0)
    {
      error (0, errno, _("cannot stat %s"), quoteaf (filename));
      повернути false;
    }

  якщо (S_ISBLK (statbuf.st_mode) || S_ISCHR (statbuf.st_mode))
    format = format2;

  bool fail = print_it (format, fd, filename, print_stat, &pa);
  повернути ! fail;
}
#endif /* USE_STATX */

/* POSIX requires 'ls' to print file sizes without a sign, even
   when negative.  Be consistent with that.  */

static uintmax_t
unsigned_file_size (off_t size)
{
  повернути size + (size < 0) * ((uintmax_t) OFF_T_MAX - OFF_T_MIN + 1);
}

/* Print stat info.  Return zero upon success, nonzero upon failure.  */
static bool
print_stat (char *pformat, size_t prefix_len, char mod, char m,
            MAYBE_UNUSED ціле fd, char const *filename, void const *data)
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
      якщо (mod == 'Q')
        {
          initialize_quoting_style ();
          filename = quoteN (filename);
        }
      out_string (pformat, prefix_len, filename);
      break;
    case 'N':
      initialize_quoting_style ();
      out_string (pformat, prefix_len, quoteN (filename));
      якщо (S_ISLNK (statbuf->st_mode))
        {
          char *linkname = areadlink_with_size (filename, statbuf->st_size);
          якщо (linkname == NULL)
            {
              error (0, errno, _("cannot read symbolic link %s"),
                     quoteaf (filename));
              повернути true;
            }
          printf (" -> ");
          out_string (pformat, prefix_len, quoteN (linkname));
          free (linkname);
        }
      break;
    case 'd':
      якщо (mod == 'H')
        out_uint (pformat, prefix_len, major (statbuf->st_dev));
      інакше якщо (mod == 'L')
        out_uint (pformat, prefix_len, minor (statbuf->st_dev));
      інакше
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
      якщо (mod == 'H')
        out_uint (pformat, prefix_len, major (statbuf->st_rdev));
      інакше якщо (mod == 'L')
        out_uint (pformat, prefix_len, minor (statbuf->st_rdev));
      інакше
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
#якщо ! USE_STATX
        btime = get_birthtime (fd, filename, statbuf);
#endif
        якщо (btime.tv_nsec < 0)
          out_string (pformat, prefix_len, "-");
        інакше
          out_string (pformat, prefix_len, human_time (btime));
      }
      break;
    case 'W':
      {
#якщо ! USE_STATX
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
  повернути fail;
}

/* Return an allocated format string in static storage that
   corresponds to whether FS and TERSE options were declared.  */
static char *
default_format (bool fs, bool terse, bool device)
{
  char *format;
  якщо (fs)
    {
      якщо (terse)
        format = xstrdup (fmt_terse_fs);
      інакше
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
  інакше /* ! fs */
    {
      якщо (terse)
        {
          якщо (0 < is_selinux_enabled ())
            format = xstrdup (fmt_terse_selinux);
          інакше
            format = xstrdup (fmt_terse_regular);
        }
      інакше
        {
          char *temp;
          /* TRANSLATORS: This string uses format specifiers from
             'stat --help' without --file-system, and NOT from printf.  */
          format = xstrdup (_("\
  File: %N\n\
  Size: %-10s\tBlocks: %-10b IO Block: %-6o %F\n\
"));

          temp = format;
          якщо (device)
            {
              /* TRANSLATORS: This string uses format specifiers from
                 'stat --help' without --file-system, and NOT from printf.  */
              format = xasprintf ("%s%s", format, _("\
" "Device: %Hd,%Ld\tInode: %-10i  Links: %-5h Device type: %Hr,%Lr\n\
"));
            }
          інакше
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

          якщо (0 < is_selinux_enabled ())
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
  повернути format;
}

void
usage (ціле status)
{
  якщо (status != EXIT_SUCCESS)
    emit_try_help ();
  інакше
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
         якщо you want a newline, include \\n in FORMAT\n\
"));
      oputs (_("\
  -t, --terse\n\
         print the information in terse form\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);

      fputs (_("\n\
The MODE argument of --cached can be: always, never, or default.\n\
'always' will use cached attributes якщо available, поки\n\
'never' will try to synchronize with the latest attributes, and\n\
'default' will leave it up to the underlying file system.\n\
"), stdout);

      fputs (_("\n\
The valid format sequences для files (without --file-system):\n\
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
  %N   quoted file name with dereference якщо symbolic link\n\
  %o   optimal I/O transfer size hint\n\
  %s   total size, in bytes\n\
  %r   device type in decimal (st_rdev)\n\
  %R   device type in hex (st_rdev)\n\
  %Hr  major device type in decimal, для character/block device special files\n\
  %Lr  minor device type in decimal, для character/block device special files\n\
  %t   major device type in hex, для character/block device special files\n\
  %T   minor device type in hex, для character/block device special files\n\
"), stdout);
      fputs (_("\
  %u   user ID of owner\n\
  %U   user name of owner\n\
  %w   time of file birth, human-readable; - якщо unknown\n\
  %W   time of file birth, seconds since Epoch; 0 якщо unknown\n\
  %x   time of last access, human-readable\n\
  %X   time of last access, seconds since Epoch\n\
  %y   time of last data modification, human-readable\n\
  %Y   time of last data modification, seconds since Epoch\n\
  %z   time of last status change, human-readable\n\
  %Z   time of last status change, seconds since Epoch\n\
\n\
"), stdout);

      fputs (_("\
Valid format sequences для file systems:\n\
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
  %s   block size (для faster transfers)\n\
  %S   fundamental block size (для block counts)\n\
  %t   file system type in hex\n\
  %T   file system type in human readable form\n\
"), stdout);

      printf (_("\n\
--terse is equivalent to the following FORMAT:\n\
    %s\
"),
#якщо HAVE_SELINUX_SELINUX_H
              fmt_terse_selinux
#інакше
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

ціле
main (ціле argc, char *argv[])
{
  ціле c;
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

  поки ((c = getopt_long (argc, argv, "c:fLt", long_options, NULL)) != -1)
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

  якщо (argc == optind)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  якщо (format)
    {
      format2 = format;
    }
  інакше
    {
      format = default_format (fs, terse, /* device= */ false);
      format2 = default_format (fs, terse, /* device= */ true);
    }

  для (ціле i = optind; i < argc; i++)
    ok &= (fs
           ? do_statfs (argv[i], format)
           : do_stat (argv[i], format, format2));

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
