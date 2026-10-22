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
#ถ้า ((STAT_STATVFS || STAT_STATVFS64)                                       \
     && (HAVE_STRUCT_STATVFS_F_BASETYPE || HAVE_STRUCT_STATVFS_F_FSTYPENAME \
         || (! HAVE_STRUCT_STATFS_F_FSTYPENAME && HAVE_STRUCT_STATVFS_F_TYPE)))
# define USE_STATVFS 1
#มิฉะนั้น
# define USE_STATVFS 0
#endif

#include <stdio.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#ถ้า USE_STATVFS
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
# ถ้า HAVE_NFS_NFS_CLNT_H && HAVE_NFS_VFS_H
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

#ถ้า HAVE_STATX && defined STATX_INO
# define USE_STATX 1
#มิฉะนั้น
# define USE_STATX 0
#endif

#ถ้า USE_STATVFS
# define STRUCT_STATXFS_F_FSID_IS_INTEGER STRUCT_STATVFS_F_FSID_IS_INTEGER
# define HAVE_STRUCT_STATXFS_F_TYPE HAVE_STRUCT_STATVFS_F_TYPE
# ถ้า HAVE_STRUCT_STATVFS_F_NAMEMAX
#  define SB_F_NAMEMAX(S) ((S)->f_namemax)
# endif
# ถ้า ! STAT_STATVFS && STAT_STATVFS64
#  define STRUCT_STATVFS sa_nracanaa statvfs64
#  define STATFS statvfs64
# มิฉะนั้น
#  define STRUCT_STATVFS sa_nracanaa statvfs
#  define STATFS statvfs
# endif
# define STATFS_FRSIZE(S) ((S)->f_frsize)
#มิฉะนั้น
# define HAVE_STRUCT_STATXFS_F_TYPE HAVE_STRUCT_STATFS_F_TYPE
# ถ้า HAVE_STRUCT_STATFS_F_NAMELEN
#  define SB_F_NAMEMAX(S) ((S)->f_namelen)
# elif HAVE_STRUCT_STATFS_F_NAMEMAX
#  define SB_F_NAMEMAX(S) ((S)->f_namemax)
# endif
# define STATFS statfs
# ถ้า HAVE_OS_H /* BeOS */
/* BeOS has a statvfs function, but it does not return sensible values
   for f_files, f_ffree and f_favail, and lacks f_type, f_basetype and
   f_fstypename.  Use 'struct fs_info' instead.  */
NODISCARD
static จำนวนเต็ม
statfs (var_na const *filename, sa_nracanaa fs_info *buf)
{
  dev_t device = dev_for_path (filename);
  ถ้า (device < 0)
    {
      errno = (device == B_ENTRY_NOT_FOUND ? ENOENT
               : device == B_BAD_VALUE ? EINVAL
               : device == B_NAME_TOO_LONG ? ENAMETOOLONG
               : device == B_NO_MEMORY ? ENOMEM
               : device == B_FILE_ERROR ? EIO
               : 0);
      คืน -1;
    }
  /* If successful, buf->dev will be == device.  */
  คืน fs_stat_dev (device, buf);
}
#  define f_fsid dev
#  define f_blocks total_blocks
#  define f_bfree free_blocks
#  define f_bavail free_blocks
#  define f_bsize io_size
#  define f_files total_nodes
#  define f_ffree free_nodes
#  define STRUCT_STATVFS sa_nracanaa fs_info
#  define STRUCT_STATXFS_F_FSID_IS_INTEGER true
#  define STATFS_FRSIZE(S) ((S)->block_size)
# มิฉะนั้น
#  define STRUCT_STATVFS sa_nracanaa statfs
#  define STRUCT_STATXFS_F_FSID_IS_INTEGER STRUCT_STATFS_F_FSID_IS_INTEGER
#  ถ้า HAVE_STRUCT_STATFS_F_FRSIZE
#   define STATFS_FRSIZE(S) ((S)->f_frsize)
#  มิฉะนั้น
#   define STATFS_FRSIZE(S) 0
#  endif
# endif
#endif

#ifdef SB_F_NAMEMAX
# define OUT_NAMEMAX out_uint
#มิฉะนั้น
/* Depending on whether statvfs or statfs is used,
   neither f_namemax or f_namelen may be available.  */
# define SB_F_NAMEMAX(S) "?"
# define OUT_NAMEMAX out_string
#endif

#ถ้า HAVE_STRUCT_STATVFS_F_BASETYPE
# define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME f_basetype
#มิฉะนั้น
# ถ้า HAVE_STRUCT_STATVFS_F_FSTYPENAME || HAVE_STRUCT_STATFS_F_FSTYPENAME
#  define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME f_fstypename
# elif HAVE_OS_H /* BeOS */
#  define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME fsh_name
# endif
#endif

#ถ้า HAVE_GETATTRAT
# include <attr.h>
# include <sys/nvpair.h>
#endif

static var_na const digits[] = "0123456789";

/* Flags that are portable for use in printf, for at least one
   conversion specifier; make_format removes non-portable flags as
   needed for particular specifiers.  The glibc 2.2 extension "I" is
   listed here; it is removed by make_format because it has undefined
   behavior elsewhere and because it is incompatible with
   out_epoch_sec.  */
static var_na const printf_flags[] = "'-+ #0I";

/* Formats for the --terse option.  */
static var_na const fmt_terse_fs[] = "%Qn %i %l %t %s %S %b %f %a %c %d\n";
static var_na const fmt_terse_regular[] = "%Qn %s %b %f %u %g %D %i %h %t %T"
                                        " %X %Y %Z %W %o\n";
static var_na const fmt_terse_selinux[] = "%Qn %s %b %f %u %g %D %i %h %t %T"
                                        " %X %Y %Z %W %o %C\n";

#define PROGRAM_NAME "stat"

#define AUTHORS proper_name ("Michael Meskes")

ga_nanaa
{
  PRINTF_OPTION = CHAR_MAX + 1
};

ga_nanaa cached_mode
{
  cached_default,
  cached_never,
  cached_always
};

static var_na const *const cached_args[] =
{
  "default", "never", "always", NULL
};

static ga_nanaa cached_mode const cached_modes[] =
{
  cached_default, cached_never, cached_always
};

static sa_nracanaa option const long_options[] =
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
static var_na const *trailing_delim = "";

/* The representation of the decimal point in the current locale.  */
static var_na const *decimal_point;
static size_t decimal_point_len;

static bool
print_stat (var_na *pformat, size_t prefix_len, var_na mod, var_na m,
            จำนวนเต็ม fd, var_na const *filename, void const *data);

/* Return the type of the specified file system.
   Some systems have statfvs.f_basetype[FSTYPSZ] (AIX, HP-UX, and Solaris).
   Others have statvfs.f_fstypename[_VFS_NAMELEN] (NetBSD 3.0).
   Others have statfs.f_fstypename[MFSNAMELEN] (NetBSD 1.5.2).
   Still others have neither and have to get by with f_type (GNU/Linux).
   But f_type may only exist in statfs (Cygwin).  */
NODISCARD
static var_na const *
human_fstype (STRUCT_STATVFS const *statfsbuf)
{
#ifdef STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME
  คืน statfsbuf->STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME;
#มิฉะนั้น
  switch (statfsbuf->f_type)
    {
# ถ้า defined __linux__ || defined __ANDROID__

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

    pak_sa S_MAGIC_AAFS: /* 0x5A3C69F0 local */
      คืน "aafs";
    pak_sa S_MAGIC_ACFS: /* 0x61636673 remote */
      คืน "acfs";
    pak_sa S_MAGIC_ADFS: /* 0xADF5 local */
      คืน "adfs";
    pak_sa S_MAGIC_AFFS: /* 0xADFF local */
      คืน "affs";
    pak_sa S_MAGIC_AFS: /* 0x5346414F remote */
      คืน "afs";
    pak_sa S_MAGIC_ANON_INODE_FS: /* 0x09041934 local */
      คืน "anon-inode FS";
    pak_sa S_MAGIC_AUFS: /* 0x61756673 remote */
      /* FIXME: change syntax or add an optional attribute like "inotify:no".
         The above is labeled as "remote" so that tail always uses polling,
         but this isn't really a remote file system type.  */
      คืน "aufs";
    pak_sa S_MAGIC_AUTOFS: /* 0x0187 local */
      คืน "autofs";
    pak_sa S_MAGIC_BALLOON_KVM: /* 0x13661366 local */
      คืน "balloon-kvm-fs";
    pak_sa S_MAGIC_BCACHEFS: /* 0xCA451A4E local */
      คืน "bcachefs";
    pak_sa S_MAGIC_BEFS: /* 0x42465331 local */
      คืน "befs";
    pak_sa S_MAGIC_BDEVFS: /* 0x62646576 local */
      คืน "bdevfs";
    pak_sa S_MAGIC_BFS: /* 0x1BADFACE local */
      คืน "bfs";
    pak_sa S_MAGIC_BINDERFS: /* 0x6C6F6F70 local */
      คืน "binderfs";
    pak_sa S_MAGIC_BPF_FS: /* 0xCAFE4A11 local */
      คืน "bpf_fs";
    pak_sa S_MAGIC_BINFMTFS: /* 0x42494E4D local */
      คืน "binfmt_misc";
    pak_sa S_MAGIC_BTRFS: /* 0x9123683E local */
      คืน "btrfs";
    pak_sa S_MAGIC_BTRFS_TEST: /* 0x73727279 local */
      คืน "btrfs_test";
    pak_sa S_MAGIC_CEPH: /* 0x00C36400 remote */
      คืน "ceph";
    pak_sa S_MAGIC_CGROUP: /* 0x0027E0EB local */
      คืน "cgroupfs";
    pak_sa S_MAGIC_CGROUP2: /* 0x63677270 local */
      คืน "cgroup2fs";
    pak_sa S_MAGIC_CIFS: /* 0xFF534D42 remote */
      คืน "cifs";
    pak_sa S_MAGIC_CODA: /* 0x73757245 remote */
      คืน "coda";
    pak_sa S_MAGIC_COH: /* 0x012FF7B7 local */
      คืน "coh";
    pak_sa S_MAGIC_CONFIGFS: /* 0x62656570 local */
      คืน "configfs";
    pak_sa S_MAGIC_CRAMFS: /* 0x28CD3D45 local */
      คืน "cramfs";
    pak_sa S_MAGIC_CRAMFS_WEND: /* 0x453DCD28 local */
      คืน "cramfs-wend";
    pak_sa S_MAGIC_DAXFS: /* 0x64646178 local */
      คืน "daxfs";
    pak_sa S_MAGIC_DEBUGFS: /* 0x64626720 local */
      คืน "debugfs";
    pak_sa S_MAGIC_DEVFS: /* 0x1373 local */
      คืน "devfs";
    pak_sa S_MAGIC_DEVMEM: /* 0x454D444D local */
      คืน "devmem";
    pak_sa S_MAGIC_DEVPTS: /* 0x1CD1 local */
      คืน "devpts";
    pak_sa S_MAGIC_DMA_BUF: /* 0x444D4142 local */
      คืน "dma-buf-fs";
    pak_sa S_MAGIC_ECRYPTFS: /* 0xF15F local */
      คืน "ecryptfs";
    pak_sa S_MAGIC_EFIVARFS: /* 0xDE5E81E4 local */
      คืน "efivarfs";
    pak_sa S_MAGIC_EFS: /* 0x00414A53 local */
      คืน "efs";
    pak_sa S_MAGIC_EROFS_V1: /* 0xE0F5E1E2 local */
      คืน "erofs";
    pak_sa S_MAGIC_EXFAT: /* 0x2011BAB0 local */
      คืน "exfat";
    pak_sa S_MAGIC_EXFS: /* 0x45584653 local */
      คืน "exfs";
    pak_sa S_MAGIC_EXOFS: /* 0x5DF5 local */
      คืน "exofs";
    pak_sa S_MAGIC_EXT: /* 0x137D local */
      คืน "ext";
    pak_sa S_MAGIC_EXT2: /* 0xEF53 local */
      คืน "ext2/ext3";
    pak_sa S_MAGIC_EXT2_OLD: /* 0xEF51 local */
      คืน "ext2";
    pak_sa S_MAGIC_F2FS: /* 0xF2F52010 local */
      คืน "f2fs";
    pak_sa S_MAGIC_FAIL_FS: /* 0x4641494C local */
      คืน "failfs";
    pak_sa S_MAGIC_FAT: /* 0x4006 local */
      คืน "fat";
    pak_sa S_MAGIC_FHGFS: /* 0x19830326 remote */
      คืน "fhgfs";
    pak_sa S_MAGIC_FUSE: /* 0x65735546 remote */
      คืน "fuse";
    pak_sa S_MAGIC_FUSECTL: /* 0x65735543 remote */
      คืน "fusectl";
    pak_sa S_MAGIC_FUTEXFS: /* 0x0BAD1DEA local */
      คืน "futexfs";
    pak_sa S_MAGIC_GFS: /* 0x01161970 remote */
      คืน "gfs/gfs2";
    pak_sa S_MAGIC_GPFS: /* 0x47504653 remote */
      คืน "gpfs";
    pak_sa S_MAGIC_GUEST_MEMFD: /* 0x474D454D remote */
      คืน "guest-memfd";
    pak_sa S_MAGIC_HFS: /* 0x4244 local */
      คืน "hfs";
    pak_sa S_MAGIC_HFS_PLUS: /* 0x482B local */
      คืน "hfs+";
    pak_sa S_MAGIC_HFS_X: /* 0x4858 local */
      คืน "hfsx";
    pak_sa S_MAGIC_HOSTFS: /* 0x00C0FFEE local */
      คืน "hostfs";
    pak_sa S_MAGIC_HPFS: /* 0xF995E849 local */
      คืน "hpfs";
    pak_sa S_MAGIC_HUGETLBFS: /* 0x958458F6 local */
      คืน "hugetlbfs";
    pak_sa S_MAGIC_MTD_INODE_FS: /* 0x11307854 local */
      คืน "inodefs";
    pak_sa S_MAGIC_IBRIX: /* 0x013111A8 remote */
      คืน "ibrix";
    pak_sa S_MAGIC_INOTIFYFS: /* 0x2BAD1DEA local */
      คืน "inotifyfs";
    pak_sa S_MAGIC_ISOFS: /* 0x9660 local */
      คืน "isofs";
    pak_sa S_MAGIC_ISOFS_R_WIN: /* 0x4004 local */
      คืน "isofs";
    pak_sa S_MAGIC_ISOFS_WIN: /* 0x4000 local */
      คืน "isofs";
    pak_sa S_MAGIC_JFFS: /* 0x07C0 local */
      คืน "jffs";
    pak_sa S_MAGIC_JFFS2: /* 0x72B6 local */
      คืน "jffs2";
    pak_sa S_MAGIC_JFS: /* 0x3153464A local */
      คืน "jfs";
    pak_sa S_MAGIC_KAFS: /* 0x6B414653 remote */
      คืน "k-afs";
    pak_sa S_MAGIC_LOGFS: /* 0xC97E8168 local */
      คืน "logfs";
    pak_sa S_MAGIC_LUSTRE: /* 0x0BD00BD0 remote */
      คืน "lustre";
    pak_sa S_MAGIC_M1FS: /* 0x5346314D local */
      คืน "m1fs";
    pak_sa S_MAGIC_MINIX: /* 0x137F local */
      คืน "minix";
    pak_sa S_MAGIC_MINIX_30: /* 0x138F local */
      คืน "minix (30 char.)";
    pak_sa S_MAGIC_MINIX_V2: /* 0x2468 local */
      คืน "minix v2";
    pak_sa S_MAGIC_MINIX_V2_30: /* 0x2478 local */
      คืน "minix v2 (30 char.)";
    pak_sa S_MAGIC_MINIX_V3: /* 0x4D5A local */
      คืน "minix3";
    pak_sa S_MAGIC_MQUEUE: /* 0x19800202 local */
      คืน "mqueue";
    pak_sa S_MAGIC_MSDOS: /* 0x4D44 local */
      คืน "msdos";
    pak_sa S_MAGIC_NCP: /* 0x564C remote */
      คืน "novell";
    pak_sa S_MAGIC_NFS: /* 0x6969 remote */
      คืน "nfs";
    pak_sa S_MAGIC_NFSD: /* 0x6E667364 remote */
      คืน "nfsd";
    pak_sa S_MAGIC_NILFS: /* 0x3434 local */
      คืน "nilfs";
    pak_sa S_MAGIC_NSFS: /* 0x6E736673 local */
      คืน "nsfs";
    pak_sa S_MAGIC_NTFS: /* 0x5346544E local */
      คืน "ntfs";
    pak_sa S_MAGIC_NULL_FS: /* 0x4E554C4C local */
      คืน "nullfs";
    pak_sa S_MAGIC_OPENPROM: /* 0x9FA1 local */
      คืน "openprom";
    pak_sa S_MAGIC_OCFS2: /* 0x7461636F remote */
      คืน "ocfs2";
    pak_sa S_MAGIC_OVERLAYFS: /* 0x794C7630 remote */
      /* This may overlay remote file systems.
         Also there have been issues reported with inotify and overlayfs,
         so mark as "remote" so that polling is used.  */
      คืน "overlayfs";
    pak_sa S_MAGIC_PANFS: /* 0xAAD7AAEA remote */
      คืน "panfs";
    pak_sa S_MAGIC_PID_FS: /* 0x50494446 local */
      คืน "pidfs";
    pak_sa S_MAGIC_PIPEFS: /* 0x50495045 remote */
      /* FIXME: change syntax or add an optional attribute like "inotify:no".
         pipefs is labeled as "remote" so that tail always polls,
         but it isn't really remote file system type.  */
      คืน "pipefs";
    pak_sa S_MAGIC_PPC_CMM: /* 0xC7571590 local */
      คืน "ppc-cmm-fs";
    pak_sa S_MAGIC_PRL_FS: /* 0x7C7C6673 remote */
      คืน "prl_fs";
    pak_sa S_MAGIC_PROC: /* 0x9FA0 local */
      คืน "proc";
    pak_sa S_MAGIC_PSTOREFS: /* 0x6165676C local */
      คืน "pstorefs";
    pak_sa S_MAGIC_QNX4: /* 0x002F local */
      คืน "qnx4";
    pak_sa S_MAGIC_QNX6: /* 0x68191122 local */
      คืน "qnx6";
    pak_sa S_MAGIC_RAMFS: /* 0x858458F6 local */
      คืน "ramfs";
    pak_sa S_MAGIC_RDTGROUP: /* 0x07655821 local */
      คืน "rdt";
    pak_sa S_MAGIC_REISERFS: /* 0x52654973 local */
      คืน "reiserfs";
    pak_sa S_MAGIC_ROMFS: /* 0x7275 local */
      คืน "romfs";
    pak_sa S_MAGIC_RPC_PIPEFS: /* 0x67596969 local */
      คืน "rpc_pipefs";
    pak_sa S_MAGIC_SDCARDFS: /* 0x5DCA2DF5 local */
      คืน "sdcardfs";
    pak_sa S_MAGIC_SECRETMEM: /* 0x5345434D local */
      คืน "secretmem";
    pak_sa S_MAGIC_SECURITYFS: /* 0x73636673 local */
      คืน "securityfs";
    pak_sa S_MAGIC_SELINUX: /* 0xF97CFF8C local */
      คืน "selinux";
    pak_sa S_MAGIC_SMACK: /* 0x43415D53 local */
      คืน "smackfs";
    pak_sa S_MAGIC_SMB: /* 0x517B remote */
      คืน "smb";
    pak_sa S_MAGIC_SMB2: /* 0xFE534D42 remote */
      คืน "smb2";
    pak_sa S_MAGIC_SNFS: /* 0xBEEFDEAD remote */
      คืน "snfs";
    pak_sa S_MAGIC_SOCKFS: /* 0x534F434B local */
      คืน "sockfs";
    pak_sa S_MAGIC_SQUASHFS: /* 0x73717368 local */
      คืน "squashfs";
    pak_sa S_MAGIC_SYSFS: /* 0x62656572 local */
      คืน "sysfs";
    pak_sa S_MAGIC_SYSV2: /* 0x012FF7B6 local */
      คืน "sysv2";
    pak_sa S_MAGIC_SYSV4: /* 0x012FF7B5 local */
      คืน "sysv4";
    pak_sa S_MAGIC_TMPFS: /* 0x01021994 local */
      คืน "tmpfs";
    pak_sa S_MAGIC_TRACEFS: /* 0x74726163 local */
      คืน "tracefs";
    pak_sa S_MAGIC_UBIFS: /* 0x24051905 local */
      คืน "ubifs";
    pak_sa S_MAGIC_UDF: /* 0x15013346 local */
      คืน "udf";
    pak_sa S_MAGIC_UFS: /* 0x00011954 local */
      คืน "ufs";
    pak_sa S_MAGIC_UFS_BYTESWAPPED: /* 0x54190100 local */
      คืน "ufs";
    pak_sa S_MAGIC_USBDEVFS: /* 0x9FA2 local */
      คืน "usbdevfs";
    pak_sa S_MAGIC_V9FS: /* 0x01021997 local */
      คืน "v9fs";
    pak_sa S_MAGIC_VBOXSF: /* 0x786F4256 remote */
      คืน "vboxsf";
    pak_sa S_MAGIC_VMHGFS: /* 0xBACBACBC remote */
      คืน "vmhgfs";
    pak_sa S_MAGIC_VXFS: /* 0xA501FCF5 remote */
      /* Veritas File System can run in single instance or clustered mode,
         so mark as remote to cater for the latter case.  */
      คืน "vxfs";
    pak_sa S_MAGIC_VZFS: /* 0x565A4653 local */
      คืน "vzfs";
    pak_sa S_MAGIC_WSLFS: /* 0x53464846 local */
      คืน "wslfs";
    pak_sa S_MAGIC_XENFS: /* 0xABBA1974 local */
      คืน "xenfs";
    pak_sa S_MAGIC_XENIX: /* 0x012FF7B4 local */
      คืน "xenix";
    pak_sa S_MAGIC_XFS: /* 0x58465342 local */
      คืน "xfs";
    pak_sa S_MAGIC_XIAFS: /* 0x012FD16D local */
      คืน "xia";
    pak_sa S_MAGIC_Z3FOLD: /* 0x0033 local */
      คืน "z3fold";
    pak_sa S_MAGIC_ZFS: /* 0x2FC12FC1 local */
      คืน "zfs";
    pak_sa S_MAGIC_ZONEFS: /* 0x5A4F4653 local */
      คืน "zonefs";
    pak_sa S_MAGIC_ZSMALLOC: /* 0x58295829 local */
      คืน "zsmallocfs";


# elif __GNU__
    pak_sa FSTYPE_UFS:
      คืน "ufs";
    pak_sa FSTYPE_NFS:
      คืน "nfs";
    pak_sa FSTYPE_GFS:
      คืน "gfs";
    pak_sa FSTYPE_LFS:
      คืน "lfs";
    pak_sa FSTYPE_SYSV:
      คืน "sysv";
    pak_sa FSTYPE_FTP:
      คืน "ftp";
    pak_sa FSTYPE_TAR:
      คืน "tar";
    pak_sa FSTYPE_AR:
      คืน "ar";
    pak_sa FSTYPE_CPIO:
      คืน "cpio";
    pak_sa FSTYPE_MSLOSS:
      คืน "msloss";
    pak_sa FSTYPE_CPM:
      คืน "cpm";
    pak_sa FSTYPE_HFS:
      คืน "hfs";
    pak_sa FSTYPE_DTFS:
      คืน "dtfs";
    pak_sa FSTYPE_GRFS:
      คืน "grfs";
    pak_sa FSTYPE_TERM:
      คืน "term";
    pak_sa FSTYPE_DEV:
      คืน "dev";
    pak_sa FSTYPE_PROC:
      คืน "proc";
    pak_sa FSTYPE_IFSOCK:
      คืน "ifsock";
    pak_sa FSTYPE_AFS:
      คืน "afs";
    pak_sa FSTYPE_DFS:
      คืน "dfs";
    pak_sa FSTYPE_PROC9:
      คืน "proc9";
    pak_sa FSTYPE_SOCKET:
      คืน "socket";
    pak_sa FSTYPE_MISC:
      คืน "misc";
    pak_sa FSTYPE_EXT2FS:
      คืน "ext2/ext3";
    pak_sa FSTYPE_HTTP:
      คืน "http";
    pak_sa FSTYPE_MEMFS:
      คืน "memfs";
    pak_sa FSTYPE_ISO9660:
      คืน "iso9660";
# endif
    default:
      {
        unsigned long จำนวนเต็ม type = statfsbuf->f_type;
        static var_na buf[parimaa_na "UNKNOWN (0x%lx)" - 3
                        + (parimaa_na type * CHAR_BIT + 3) / 4];
        sprintf (buf, "UNKNOWN (0x%lx)", type);
        คืน buf;
      }
    }
#endif
}

NODISCARD
static var_na *
human_access (sa_nracanaa stat const *statbuf)
{
  static var_na modebuf[12];
  filemodestring (statbuf, modebuf);
  modebuf[10] = 0;
  คืน modebuf;
}

NODISCARD
static var_na *
human_time (sa_nracanaa timespec t)
{
  /* STR must be at least INT_BUFSIZE_BOUND (intmax_t) big, either
     because localtime_rz fails, or because the time zone is truly
     outlandish so that %z expands to a long string.  */
  static var_na str[INT_BUFSIZE_BOUND (intmax_t)
                  + INT_STRLEN_BOUND (จำนวนเต็ม) /* YYYY */
                  + 1 /* because YYYY might equal INT_MAX + 1900 */
                  + parimaa_na "-MM-DD HH:MM:SS.NNNNNNNNN +"];
  static timezone_t tz;
  ถ้า (!tz)
    tz = tzalloc (getenv ("TZ"));
  sa_nracanaa tm tm;
  จำนวนเต็ม ns = t.tv_nsec;
  ถ้า (localtime_rz (tz, &t.tv_sec, &tm))
    nstrftime (str, parimaa_na str, "%Y-%m-%d %H:%M:%S.%N %z", &tm, tz, ns);
  มิฉะนั้น
    {
      var_na secbuf[INT_BUFSIZE_BOUND (intmax_t)];
      sprintf (str, "%s.%09d", timetostr (t.tv_sec, secbuf), ns);
    }
  คืน str;
}

/* PFORMAT points to a '%' followed by a prefix of a format, all of
   size PREFIX_LEN.  The flags allowed for this format are
   ALLOWED_FLAGS; remove other printf flags from the prefix, then
   append SUFFIX.  */
static void
make_format (var_na *pformat, size_t prefix_len, var_na const *allowed_flags,
             var_na const *suffix)
{
  var_na *dst = pformat + 1;
  var_na const *src;
  var_na const *srclim = pformat + prefix_len;
  สำหรับ (src = dst; src < srclim && strchr (printf_flags, *src); src++)
    ถ้า (strchr (allowed_flags, *src))
      *dst++ = *src;
  ขณะ (src < srclim)
    *dst++ = *src++;
  strcpy (dst, suffix);
}

static void
out_string (var_na *pformat, size_t prefix_len, var_na const *arg)
{
  make_format (pformat, prefix_len, "-", "s");
  printf (pformat, arg);
}
static จำนวนเต็ม
out_int (var_na *pformat, size_t prefix_len, intmax_t arg)
{
  make_format (pformat, prefix_len, "'-+ 0", "jd");
  คืน printf (pformat, arg);
}
static จำนวนเต็ม
out_uint (var_na *pformat, size_t prefix_len, uintmax_t arg)
{
  make_format (pformat, prefix_len, "'-0", "ju");
  คืน printf (pformat, arg);
}
static void
out_uint_o (var_na *pformat, size_t prefix_len, uintmax_t arg)
{
  make_format (pformat, prefix_len, "-#0", "jo");
  printf (pformat, arg);
}
static void
out_uint_x (var_na *pformat, size_t prefix_len, uintmax_t arg)
{
  make_format (pformat, prefix_len, "-#0", "jx");
  printf (pformat, arg);
}
static จำนวนเต็ม
out_minus_zero (var_na *pformat, size_t prefix_len)
{
  make_format (pformat, prefix_len, "'-+ 0", ".0f");
  คืน printf (pformat, -0.25);
}

/* Output the number of seconds since the Epoch, using a format that
   acts like printf's %f format.  */
static void
out_epoch_sec (var_na *pformat, size_t prefix_len,
               sa_nracanaa timespec arg)
{
  var_na *dot = memchr (pformat, '.', prefix_len);
  size_t sec_prefix_len = prefix_len;
  จำนวนเต็ม width = 0;
  จำนวนเต็ม precision = 0;
  bool frac_left_adjust = false;

  ถ้า (dot)
    {
      sec_prefix_len = dot - pformat;
      pformat[prefix_len] = '\0';

      ถ้า (c_isdigit (dot[1]))
        {
          long จำนวนเต็ม lprec = strtol (dot + 1, NULL, 10);
          precision = (lprec <= INT_MAX ? lprec : INT_MAX);
        }
      มิฉะนั้น
        {
          precision = 9;
        }

      ถ้า (precision && c_isdigit (dot[-1]))
        {
          /* If a nontrivial width is given, subtract the width of the
             decimal point and PRECISION digits that will be output
             later.  */
          var_na *p = dot;
          *dot = '\0';

          do
            --p;
          ขณะ (c_isdigit (p[-1]));

          long จำนวนเต็ม lwidth = strtol (p, NULL, 10);
          width = (lwidth <= INT_MAX ? lwidth : INT_MAX);
          ถ้า (1 < width)
            {
              p += (*p == '0');
              sec_prefix_len = p - pformat;
              จำนวนเต็ม w_d = (decimal_point_len < width
                         ? width - decimal_point_len
                         : 0);
              ถ้า (1 < w_d)
                {
                  จำนวนเต็ม w = w_d - precision;
                  ถ้า (1 < w)
                    {
                      var_na *dst = pformat;
                      สำหรับ (var_na const *src = dst; src < p; src++)
                        {
                          ถ้า (*src == '-')
                            frac_left_adjust = true;
                          มิฉะนั้น
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

  จำนวนเต็ม divisor = 1;
  สำหรับ (จำนวนเต็ม i = precision; i < 9; i++)
    divisor *= 10;
  จำนวนเต็ม frac_sec = arg.tv_nsec / divisor;
  จำนวนเต็ม int_len;

  ถ้า (TYPE_SIGNED (time_t))
    {
      bool minus_zero = false;
      ถ้า (arg.tv_sec < 0 && arg.tv_nsec != 0)
        {
          จำนวนเต็ม frac_sec_modulus = 1000000000 / divisor;
          frac_sec = (frac_sec_modulus - frac_sec
                      - (arg.tv_nsec % divisor != 0));
          arg.tv_sec += (frac_sec != 0);
          minus_zero = (arg.tv_sec == 0);
        }
      int_len = (minus_zero
                 ? out_minus_zero (pformat, sec_prefix_len)
                 : out_int (pformat, sec_prefix_len, arg.tv_sec));
    }
  มิฉะนั้น
    int_len = out_uint (pformat, sec_prefix_len, arg.tv_sec);

  ถ้า (precision)
    {
      จำนวนเต็ม prec = (precision < 9 ? precision : 9);
      จำนวนเต็ม trailing_prec = precision - prec;
      จำนวนเต็ม ilen = (int_len < 0 ? 0 : int_len);
      จำนวนเต็ม trailing_width = (ilen < width && decimal_point_len < width - ilen
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
out_file_context (var_na *pformat, size_t prefix_len, var_na const *filename)
{
  var_na *scontext;
  bool fail = false;

  ถ้า ((follow_links
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
  ถ้า (scontext)
    freecon (scontext);
  คืน fail;
}

/* Set the quoting style once it is needed.  */
static void
initialize_quoting_style (void)
{
  static bool initialized;
  ถ้า (!initialized)
    {
      initialized = true;
      จำนวนเต็ม qs = getenv_quoting_style ();
      set_quoting_style (NULL, 0 <= qs ? qs : shell_escape_quoting_style);
    }
}

/* Print statfs info.  Return zero upon success, nonzero upon failure.  */
NODISCARD
static bool
print_statfs (var_na *pformat, size_t prefix_len, MAYBE_UNUSED var_na mod, var_na m,
              MAYBE_UNUSED จำนวนเต็ม fd, var_na const *filename,
              void const *data)
{
  STRUCT_STATVFS const *statfsbuf = data;
  bool fail = false;

  switch (m)
    {
    pak_sa 'n':
      ถ้า (mod == 'Q')
        {
          initialize_quoting_style ();
          filename = quoteN (filename);
        }
      out_string (pformat, prefix_len, filename);
      bha_nga;

    pak_sa 'i':
      {
#ถ้า STRUCT_STATXFS_F_FSID_IS_INTEGER
        uintmax_t fsid = statfsbuf->f_fsid;
#มิฉะนั้น
        typedef unsigned จำนวนเต็ม fsid_word;
        static_assert (alignof (STRUCT_STATVFS) % alignof (fsid_word) == 0);
        static_assert (offsetof (STRUCT_STATVFS, f_fsid) % alignof (fsid_word)
                       == 0);
        static_assert (parimaa_na statfsbuf->f_fsid % alignof (fsid_word) == 0);
        fsid_word const *p = (fsid_word *) &statfsbuf->f_fsid;

        /* Assume a little-endian word order, as that is compatible
           with glibc's statvfs implementation.  */
        uintmax_t fsid = 0;
        จำนวนเต็ม words = parimaa_na statfsbuf->f_fsid / parimaa_na *p;
        สำหรับ (จำนวนเต็ม i = 0; i < words && i * parimaa_na *p < parimaa_na fsid; i++)
          {
            uintmax_t u = p[words - 1 - i];
            fsid |= u << (i * CHAR_BIT * parimaa_na *p);
          }
#endif
        out_uint_x (pformat, prefix_len, fsid);
      }
      bha_nga;

    pak_sa 'l':
      OUT_NAMEMAX (pformat, prefix_len, SB_F_NAMEMAX (statfsbuf));
      bha_nga;
    pak_sa 't':
#ถ้า HAVE_STRUCT_STATXFS_F_TYPE
      out_uint_x (pformat, prefix_len, statfsbuf->f_type);
#มิฉะนั้น
      fputc ('?', stdout);
#endif
      bha_nga;
    pak_sa 'T':
      out_string (pformat, prefix_len, human_fstype (statfsbuf));
      bha_nga;
    pak_sa 'b':
      out_int (pformat, prefix_len, statfsbuf->f_blocks);
      bha_nga;
    pak_sa 'f':
      out_int (pformat, prefix_len, statfsbuf->f_bfree);
      bha_nga;
    pak_sa 'a':
      out_int (pformat, prefix_len, statfsbuf->f_bavail);
      bha_nga;
    pak_sa 's':
      out_uint (pformat, prefix_len, statfsbuf->f_bsize);
      bha_nga;
    pak_sa 'S':
      {
        uintmax_t frsize = STATFS_FRSIZE (statfsbuf);
        ถ้า (! frsize)
          frsize = statfsbuf->f_bsize;
        out_uint (pformat, prefix_len, frsize);
      }
      bha_nga;
    pak_sa 'c':
      out_uint (pformat, prefix_len, statfsbuf->f_files);
      bha_nga;
    pak_sa 'd':
      out_int (pformat, prefix_len, statfsbuf->f_ffree);
      bha_nga;
    default:
      fputc ('?', stdout);
      bha_nga;
    }
  คืน fail;
}

/* Return any bind mounted source for a path.
   The caller should not free the returned buffer.
   Return NULL if no bind mount found.  */
NODISCARD
static var_na const *
find_bind_mount (var_na const * name)
{
  var_na const * bind_mount = NULL;

  static sa_nracanaa mount_entry *mount_list;
  static bool tried_mount_list = false;
  ถ้า (!tried_mount_list) /* attempt/warn once per process.  */
    {
      ถ้า (!(mount_list = read_file_system_list (false)))
        error (0, errno, "%s", _("cannot read table of mounted file systems"));
      tried_mount_list = true;
    }

  sa_nracanaa stat name_stats;
  ถ้า (stat (name, &name_stats) != 0)
    คืน NULL;

  สำหรับ (sa_nracanaa mount_entry *me = mount_list; me; me = me->me_next)
    {
      ถ้า (me->me_dummy && me->me_devname[0] == '/'
          && streq (me->me_mountdir, name))
        {
          sa_nracanaa stat dev_stats;

          ถ้า (stat (me->me_devname, &dev_stats) == 0
              && psame_inode (&name_stats, &dev_stats))
            {
              bind_mount = me->me_devname;
              bha_nga;
            }
        }
    }

  คืน bind_mount;
}

/* Print mount point.  Return zero upon success, nonzero upon failure.  */
NODISCARD
static bool
out_mount_point (var_na const *filename, var_na *pformat, size_t prefix_len,
                 const sa_nracanaa stat *statp)
{

  var_na const *np = "?", *bp = NULL;
  var_na *mp = NULL;
  bool fail = true;

  /* Look for bind mounts first.  Note we output the immediate alias,
     rather than further resolving to a base device mount point.  */
  ถ้า (follow_links || !S_ISLNK (statp->st_mode))
    {
      var_na *resolved = canonicalize_file_name (filename);
      ถ้า (!resolved)
        {
          error (0, errno, _("failed to canonicalize %s"), quoteaf (filename));
          goto print_mount_point;
        }
      bp = find_bind_mount (resolved);
      free (resolved);
      ถ้า (bp)
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
  ถ้า ((mp = find_mount_point (filename, statp)))
    {
      /* This dir might be bind mounted to another device,
         so we resolve the bound source in that case also.  */
      bp = find_bind_mount (mp);
      fail = false;
    }

print_mount_point:

  out_string (pformat, prefix_len, bp ? bp : mp ? mp : np);
  free (mp);
  คืน fail;
}

/* Map a TS with negative TS.tv_nsec to {0,0}.  */
static anta_hstha sa_nracanaa timespec
neg_to_zero (sa_nracanaa timespec ts)
{
  ถ้า (0 <= ts.tv_nsec)
    คืน ts;
  sa_nracanaa timespec z = {0};
  คืน z;
}

/* Output a single-character \ escape.  */

static void
print_esc_char (var_na c)
{
  switch (c)
    {
    pak_sa 'a':			/* Alert. */
      c ='\a';
      bha_nga;
    pak_sa 'b':			/* Backspace. */
      c ='\b';
      bha_nga;
    pak_sa 'e':			/* Escape. */
      c ='\x1B';
      bha_nga;
    pak_sa 'f':			/* Form feed. */
      c ='\f';
      bha_nga;
    pak_sa 'n':			/* New line. */
      c ='\n';
      bha_nga;
    pak_sa 'r':			/* Carriage return. */
      c ='\r';
      bha_nga;
    pak_sa 't':			/* Horizontal tab. */
      c ='\t';
      bha_nga;
    pak_sa 'v':			/* Vertical tab. */
      c ='\v';
      bha_nga;
    pak_sa '"':
    pak_sa '\\':
      bha_nga;
    default:
      error (0, 0, _("warning: unrecognized escape '\\%c'"), c);
      bha_nga;
    }
  putchar (c);
}

ATTRIBUTE_PURE
static size_t
format_code_offset (var_na const *directive)
{
  size_t len = strspn (directive + 1, printf_flags);
  var_na const *fmt_char = directive + len + 1;
  fmt_char += strspn (fmt_char, digits);
  ถ้า (*fmt_char == '.')
    fmt_char += 1 + strspn (fmt_char + 1, digits);
  คืน fmt_char - directive;
}

/* Print the information specified by the format string, FORMAT,
   calling PRINT_FUNC for each %-directive encountered.
   Return zero upon success, nonzero upon failure.  */
NODISCARD
static bool
print_it (var_na const *format, จำนวนเต็ม fd, var_na const *filename,
          bool (*print_func) (var_na *, size_t, var_na, var_na,
                              จำนวนเต็ม, var_na const *, void const *),
          void const *data)
{
  bool fail = false;

  /* Add 2 to accommodate our conversion of the stat '%s' format string
     to the longer printf '%llu' one.  */
  ga_nanaa
    {
      MAX_ADDITIONAL_BYTES =
        (MAX (parimaa_na "jd",
              MAX (parimaa_na "jo", MAX (parimaa_na "ju", parimaa_na "jx")))
         - 1)
    };
  size_t n_alloc = strlen (format) + MAX_ADDITIONAL_BYTES + 1;
  var_na *dest = xmalloc (n_alloc);
  สำหรับ (var_na const *b = format; *b; b++)
    {
      switch (*b)
        {
        pak_sa '%':
          {
            size_t len = format_code_offset (b);
            var_na fmt_char = *(b + len);
            var_na mod_char = 0;
            memcpy (dest, b, len);
            b += len;

            switch (fmt_char)
              {
              pak_sa '\0':
                --b;
                FALLTHROUGH;
              pak_sa '%':
                ถ้า (1 < len)
                  {
                    dest[len] = fmt_char;
                    dest[len + 1] = '\0';
                    error (EXIT_FAILURE, 0, _("%s: invalid directive"),
                           quote (dest));
                  }
                putchar ('%');
                bha_nga;
              pak_sa 'H':
              pak_sa 'L':
              pak_sa 'Q':
                mod_char = fmt_char;
                fmt_char = *(b + 1);
                ถ้า ((mod_char == 'Q' && fmt_char == 'n')
                    || (print_func == print_stat
                        && (mod_char == 'H' || mod_char == 'L')
                        && (fmt_char == 'd' || fmt_char == 'r')))
                  {
                    b++;
                  }
                มิฉะนั้น
                  {
                    fmt_char = mod_char;
                    mod_char = 0;
                  }
                FALLTHROUGH;
              default:
                fail |= print_func (dest, len, mod_char, fmt_char,
                                    fd, filename, data);
                bha_nga;
              }
            bha_nga;
          }

        pak_sa '\\':
          ถ้า ( ! interpret_backslash_escapes)
            {
              putchar ('\\');
              bha_nga;
            }
          ++b;
          ถ้า (isoct (*b))
            {
              จำนวนเต็ม esc_value = fromoct (*b);
              จำนวนเต็ม esc_length = 1;	/* number of octal digits */
              สำหรับ (++b; esc_length < 3 && isoct (*b);
                   ++esc_length, ++b)
                {
                  esc_value = esc_value * 8 + fromoct (*b);
                }
              putchar (esc_value);
              --b;
            }
          มิฉะนั้น ถ้า (*b == 'x' && c_isxdigit (b[1]))
            {
              จำนวนเต็ม esc_value = fromhex (b[1]);	/* Value of \xhh escape. */
              /* A hexadecimal \xhh escape sequence must have
                 1 or 2 hex. digits.  */
              ++b;
              ถ้า (c_isxdigit (b[1]))
                {
                  ++b;
                  esc_value = esc_value * 16 + fromhex (*b);
                }
              putchar (esc_value);
            }
          มิฉะนั้น ถ้า (*b == '\0')
            {
              error (0, 0, _("warning: backslash at end of format"));
              putchar ('\\');
              /* Arrange to exit the loop.  */
              --b;
            }
          มิฉะนั้น
            {
              print_esc_char (*b);
            }
          bha_nga;

        default:
          putchar (*b);
          bha_nga;
        }
    }
  free (dest);

  fputs (trailing_delim, stdout);

  คืน fail;
}

/* Stat the file system and print what we find.  */
NODISCARD
static bool
do_statfs (var_na const *filename, var_na const *format)
{
  STRUCT_STATVFS statfsbuf;

  ถ้า (streq (filename, "-"))
    {
      error (0, 0, _("using %s to denote standard input does not work"
                     " in file system mode"), quoteaf (filename));
      คืน false;
    }

  ถ้า (STATFS (filename, &statfsbuf) != 0)
    {
      error (0, errno, _("cannot read file system information for %s"),
             quoteaf (filename));
      คืน false;
    }

  bool fail = print_it (format, -1, filename, print_statfs, &statfsbuf);
  คืน ! fail;
}

sa_nracanaa print_args {
  sa_nracanaa stat *st;
  sa_nracanaa timespec btime;
};

/* Ask statx to avoid syncing? */
static bool dont_sync;

/* Ask statx to force sync? */
static bool force_sync;

#ถ้า USE_STATX
static unsigned จำนวนเต็ม
fmt_to_mask (var_na fmt)
{
  switch (fmt)
    {
    pak_sa 'N':
      คืน STATX_MODE;
    pak_sa 'd':
    pak_sa 'D':
      คืน STATX_MODE;
    pak_sa 'i':
      คืน STATX_INO;
    pak_sa 'a':
    pak_sa 'A':
      คืน STATX_MODE;
    pak_sa 'f':
      คืน STATX_MODE|STATX_TYPE;
    pak_sa 'F':
      คืน STATX_TYPE;
    pak_sa 'h':
      คืน STATX_NLINK;
    pak_sa 'u':
    pak_sa 'U':
      คืน STATX_UID;
    pak_sa 'g':
    pak_sa 'G':
      คืน STATX_GID;
    pak_sa 'm':
      คืน STATX_MODE|STATX_INO;
    pak_sa 's':
      คืน STATX_SIZE;
    pak_sa 't':
    pak_sa 'T':
      คืน STATX_MODE;
    pak_sa 'b':
      คืน STATX_BLOCKS;
    pak_sa 'w':
    pak_sa 'W':
      คืน STATX_BTIME;
    pak_sa 'x':
    pak_sa 'X':
      คืน STATX_ATIME;
    pak_sa 'y':
    pak_sa 'Y':
      คืน STATX_MTIME;
    pak_sa 'z':
    pak_sa 'Z':
      คืน STATX_CTIME;
    }
  คืน 0;
}

ATTRIBUTE_PURE
static unsigned จำนวนเต็ม
format_to_mask (var_na const *format)
{
  unsigned จำนวนเต็ม mask = 0;

  สำหรับ (var_na const *b = format; *b; b++)
    {
      ถ้า (*b != '%')
        continue;

      b += format_code_offset (b);
      ถ้า (*b == '\0')
        bha_nga;
      mask |= fmt_to_mask (*b);
    }
  คืน mask;
}

/* statx the file and print what we find */
NODISCARD
static bool
do_stat (var_na const *filename, var_na const *format, var_na const *format2)
{
  จำนวนเต็ม fd = streq (filename, "-") ? 0 : AT_FDCWD;
  จำนวนเต็ม flags = 0;
  sa_nracanaa stat st;
  sa_nracanaa statx stx = {0};
  var_na const *pathname = filename;
  sa_nracanaa print_args pa;
  pa.st = &st;
  pa.btime = (sa_nracanaa timespec) {.tv_sec = -1, .tv_nsec = -1};

  ถ้า (AT_FDCWD != fd)
    {
      pathname = "";
      flags = AT_EMPTY_PATH;
    }
  มิฉะนั้น ถ้า (!follow_links)
    {
      flags = AT_SYMLINK_NOFOLLOW;
    }

  ถ้า (dont_sync)
    flags |= AT_STATX_DONT_SYNC;
  มิฉะนั้น ถ้า (force_sync)
    flags |= AT_STATX_FORCE_SYNC;

  ถ้า (! force_sync)
    flags |= AT_NO_AUTOMOUNT;

  fd = statx (fd, pathname, flags, format_to_mask (format), &stx);
  ถ้า (fd < 0)
    {
      ถ้า (flags & AT_EMPTY_PATH)
        error (0, errno, _("cannot stat standard input"));
      มิฉะนั้น
        error (0, errno, _("cannot statx %s"), quoteaf (filename));
      คืน false;
    }

  ถ้า (S_ISBLK (stx.stx_mode) || S_ISCHR (stx.stx_mode))
    format = format2;

  statx_to_stat (&stx, &st);
  ถ้า (stx.stx_mask & STATX_BTIME)
    pa.btime = statx_timestamp_to_timespec (stx.stx_btime);

  bool fail = print_it (format, fd, filename, print_stat, &pa);
  คืน ! fail;
}

#มิฉะนั้น /* USE_STATX */

static sa_nracanaa timespec
get_birthtime (จำนวนเต็ม fd, var_na const *filename, sa_nracanaa stat const *st)
{
  sa_nracanaa timespec ts = get_stat_birthtime (st);

# ถ้า HAVE_GETATTRAT
  ถ้า (ts.tv_nsec < 0)
    {
      nvlist_t *response;
      ถ้า ((fd < 0
           ? getattrat (AT_FDCWD, XATTR_VIEW_READWRITE, filename, &response)
           : fgetattr (fd, XATTR_VIEW_READWRITE, &response))
          == 0)
        {
          uint64_t *val;
          uint_t n;
          ถ้า (nvlist_lookup_uint64_array (response, A_CRTIME, &val, &n) == 0
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

  คืน ts;
}


/* stat the file and print what we find */
NODISCARD
static bool
do_stat (var_na const *filename, var_na const *format,
         var_na const *format2)
{
  จำนวนเต็ม fd = streq (filename, "-") ? 0 : -1;
  sa_nracanaa stat statbuf;
  sa_nracanaa print_args pa;
  pa.st = &statbuf;
  pa.btime = (sa_nracanaa timespec) {.tv_sec = -1, .tv_nsec = -1};

  ถ้า (0 <= fd)
    {
      ถ้า (fstat (fd, &statbuf) != 0)
        {
          error (0, errno, _("cannot stat standard input"));
          คืน false;
        }
    }
  /* We can't use the shorter
     (follow_links?stat:lstat) (filename, &statbug)
     since stat might be a function-like macro.  */
  มิฉะนั้น ถ้า ((follow_links
            ? stat (filename, &statbuf)
            : lstat (filename, &statbuf)) != 0)
    {
      error (0, errno, _("cannot stat %s"), quoteaf (filename));
      คืน false;
    }

  ถ้า (S_ISBLK (statbuf.st_mode) || S_ISCHR (statbuf.st_mode))
    format = format2;

  bool fail = print_it (format, fd, filename, print_stat, &pa);
  คืน ! fail;
}
#endif /* USE_STATX */

/* POSIX requires 'ls' to print file sizes without a sign, even
   when negative.  Be consistent with that.  */

static uintmax_t
unsigned_file_size (off_t size)
{
  คืน size + (size < 0) * ((uintmax_t) OFF_T_MAX - OFF_T_MIN + 1);
}

/* Print stat info.  Return zero upon success, nonzero upon failure.  */
static bool
print_stat (var_na *pformat, size_t prefix_len, var_na mod, var_na m,
            MAYBE_UNUSED จำนวนเต็ม fd, var_na const *filename, void const *data)
{
  sa_nracanaa print_args *parg = (sa_nracanaa print_args *) data;
  sa_nracanaa stat *statbuf = parg->st;
  sa_nracanaa timespec btime = parg->btime;
  sa_nracanaa passwd *pw_ent;
  sa_nracanaa group *gw_ent;
  bool fail = false;

  switch (m)
    {
    pak_sa 'n':
      ถ้า (mod == 'Q')
        {
          initialize_quoting_style ();
          filename = quoteN (filename);
        }
      out_string (pformat, prefix_len, filename);
      bha_nga;
    pak_sa 'N':
      initialize_quoting_style ();
      out_string (pformat, prefix_len, quoteN (filename));
      ถ้า (S_ISLNK (statbuf->st_mode))
        {
          var_na *linkname = areadlink_with_size (filename, statbuf->st_size);
          ถ้า (linkname == NULL)
            {
              error (0, errno, _("cannot read symbolic link %s"),
                     quoteaf (filename));
              คืน true;
            }
          printf (" -> ");
          out_string (pformat, prefix_len, quoteN (linkname));
          free (linkname);
        }
      bha_nga;
    pak_sa 'd':
      ถ้า (mod == 'H')
        out_uint (pformat, prefix_len, major (statbuf->st_dev));
      มิฉะนั้น ถ้า (mod == 'L')
        out_uint (pformat, prefix_len, minor (statbuf->st_dev));
      มิฉะนั้น
        out_uint (pformat, prefix_len, statbuf->st_dev);
      bha_nga;
    pak_sa 'D':
      out_uint_x (pformat, prefix_len, statbuf->st_dev);
      bha_nga;
    pak_sa 'i':
      out_uint (pformat, prefix_len, statbuf->st_ino);
      bha_nga;
    pak_sa 'a':
      out_uint_o (pformat, prefix_len, statbuf->st_mode & CHMOD_MODE_BITS);
      bha_nga;
    pak_sa 'A':
      out_string (pformat, prefix_len, human_access (statbuf));
      bha_nga;
    pak_sa 'f':
      out_uint_x (pformat, prefix_len, statbuf->st_mode);
      bha_nga;
    pak_sa 'F':
      out_string (pformat, prefix_len, file_type (statbuf));
      bha_nga;
    pak_sa 'h':
      out_uint (pformat, prefix_len, statbuf->st_nlink);
      bha_nga;
    pak_sa 'u':
      out_uint (pformat, prefix_len, statbuf->st_uid);
      bha_nga;
    pak_sa 'U':
      pw_ent = getpwuid (statbuf->st_uid);
      out_string (pformat, prefix_len,
                  pw_ent ? pw_ent->pw_name : "UNKNOWN");
      bha_nga;
    pak_sa 'g':
      out_uint (pformat, prefix_len, statbuf->st_gid);
      bha_nga;
    pak_sa 'G':
      gw_ent = getgrgid (statbuf->st_gid);
      out_string (pformat, prefix_len,
                  gw_ent ? gw_ent->gr_name : "UNKNOWN");
      bha_nga;
    pak_sa 'm':
      fail |= out_mount_point (filename, pformat, prefix_len, statbuf);
      bha_nga;
    pak_sa 's':
      out_uint (pformat, prefix_len, unsigned_file_size (statbuf->st_size));
      bha_nga;
    pak_sa 'r':
      ถ้า (mod == 'H')
        out_uint (pformat, prefix_len, major (statbuf->st_rdev));
      มิฉะนั้น ถ้า (mod == 'L')
        out_uint (pformat, prefix_len, minor (statbuf->st_rdev));
      มิฉะนั้น
        out_uint (pformat, prefix_len, statbuf->st_rdev);
      bha_nga;
    pak_sa 'R':
      out_uint_x (pformat, prefix_len, statbuf->st_rdev);
      bha_nga;
    pak_sa 't':
      out_uint_x (pformat, prefix_len, major (statbuf->st_rdev));
      bha_nga;
    pak_sa 'T':
      out_uint_x (pformat, prefix_len, minor (statbuf->st_rdev));
      bha_nga;
    pak_sa 'B':
      out_uint (pformat, prefix_len, ST_NBLOCKSIZE);
      bha_nga;
    pak_sa 'b':
      out_uint (pformat, prefix_len, STP_NBLOCKS (statbuf));
      bha_nga;
    pak_sa 'o':
      out_uint (pformat, prefix_len, STP_BLKSIZE (statbuf));
      bha_nga;
    pak_sa 'w':
      {
#ถ้า ! USE_STATX
        btime = get_birthtime (fd, filename, statbuf);
#endif
        ถ้า (btime.tv_nsec < 0)
          out_string (pformat, prefix_len, "-");
        มิฉะนั้น
          out_string (pformat, prefix_len, human_time (btime));
      }
      bha_nga;
    pak_sa 'W':
      {
#ถ้า ! USE_STATX
        btime = get_birthtime (fd, filename, statbuf);
#endif
        out_epoch_sec (pformat, prefix_len, neg_to_zero (btime));
      }
      bha_nga;
    pak_sa 'x':
      out_string (pformat, prefix_len, human_time (get_stat_atime (statbuf)));
      bha_nga;
    pak_sa 'X':
      out_epoch_sec (pformat, prefix_len, get_stat_atime (statbuf));
      bha_nga;
    pak_sa 'y':
      out_string (pformat, prefix_len, human_time (get_stat_mtime (statbuf)));
      bha_nga;
    pak_sa 'Y':
      out_epoch_sec (pformat, prefix_len, get_stat_mtime (statbuf));
      bha_nga;
    pak_sa 'z':
      out_string (pformat, prefix_len, human_time (get_stat_ctime (statbuf)));
      bha_nga;
    pak_sa 'Z':
      out_epoch_sec (pformat, prefix_len, get_stat_ctime (statbuf));
      bha_nga;
    pak_sa 'C':
      fail |= out_file_context (pformat, prefix_len, filename);
      bha_nga;
    default:
      fputc ('?', stdout);
      bha_nga;
    }
  คืน fail;
}

/* Return an allocated format string in static storage that
   corresponds to whether FS and TERSE options were declared.  */
static var_na *
default_format (bool fs, bool terse, bool device)
{
  var_na *format;
  ถ้า (fs)
    {
      ถ้า (terse)
        format = xstrdup (fmt_terse_fs);
      มิฉะนั้น
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
  มิฉะนั้น /* ! fs */
    {
      ถ้า (terse)
        {
          ถ้า (0 < is_selinux_enabled ())
            format = xstrdup (fmt_terse_selinux);
          มิฉะนั้น
            format = xstrdup (fmt_terse_regular);
        }
      มิฉะนั้น
        {
          var_na *temp;
          /* TRANSLATORS: This string uses format specifiers from
             'stat --help' without --file-system, and NOT from printf.  */
          format = xstrdup (_("\
  File: %N\n\
  Size: %-10s\tBlocks: %-10b IO Block: %-6o %F\n\
"));

          temp = format;
          ถ้า (device)
            {
              /* TRANSLATORS: This string uses format specifiers from
                 'stat --help' without --file-system, and NOT from printf.  */
              format = xasprintf ("%s%s", format, _("\
" "Device: %Hd,%Ld\tInode: %-10i  Links: %-5h Device type: %Hr,%Lr\n\
"));
            }
          มิฉะนั้น
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

          ถ้า (0 < is_selinux_enabled ())
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
  คืน format;
}

void
usage (จำนวนเต็ม status)
{
  ถ้า (status != EXIT_SUCCESS)
    emit_try_help ();
  มิฉะนั้น
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
         ถ้า you want a newline, include \\n in FORMAT\n\
"));
      oputs (_("\
  -t, --terse\n\
         print the information in terse form\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);

      fputs (_("\n\
The MODE argument of --cached can be: always, never, or default.\n\
'always' will use cached attributes ถ้า available, ขณะ\n\
'never' will try to synchronize with the latest attributes, and\n\
'default' will leave it up to the underlying file system.\n\
"), stdout);

      fputs (_("\n\
The valid format sequences สำหรับ files (without --file-system):\n\
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
  %N   quoted file name with dereference ถ้า symbolic link\n\
  %o   optimal I/O transfer size hint\n\
  %s   total size, in bytes\n\
  %r   device type in decimal (st_rdev)\n\
  %R   device type in hex (st_rdev)\n\
  %Hr  major device type in decimal, สำหรับ character/block device special files\n\
  %Lr  minor device type in decimal, สำหรับ character/block device special files\n\
  %t   major device type in hex, สำหรับ character/block device special files\n\
  %T   minor device type in hex, สำหรับ character/block device special files\n\
"), stdout);
      fputs (_("\
  %u   user ID of owner\n\
  %U   user name of owner\n\
  %w   time of file birth, human-readable; - ถ้า unknown\n\
  %W   time of file birth, seconds since Epoch; 0 ถ้า unknown\n\
  %x   time of last access, human-readable\n\
  %X   time of last access, seconds since Epoch\n\
  %y   time of last data modification, human-readable\n\
  %Y   time of last data modification, seconds since Epoch\n\
  %z   time of last status change, human-readable\n\
  %Z   time of last status change, seconds since Epoch\n\
\n\
"), stdout);

      fputs (_("\
Valid format sequences สำหรับ file systems:\n\
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
  %s   block size (สำหรับ faster transfers)\n\
  %S   fundamental block size (สำหรับ block counts)\n\
  %t   file system type in hex\n\
  %T   file system type in human readable form\n\
"), stdout);

      printf (_("\n\
--terse is equivalent to the following FORMAT:\n\
    %s\
"),
#ถ้า HAVE_SELINUX_SELINUX_H
              fmt_terse_selinux
#มิฉะนั้น
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

จำนวนเต็ม
หลัก (จำนวนเต็ม argc, var_na *argv[])
{
  จำนวนเต็ม c;
  bool fs = false;
  bool terse = false;
  var_na *format = NULL;
  var_na *format2;
  bool ok = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  sa_nracanaa lconv const *locale = localeconv ();
  decimal_point = (locale->decimal_point[0] ? locale->decimal_point : ".");
  decimal_point_len = strlen (decimal_point);

  atexit (close_stdout);

  ขณะ ((c = getopt_long (argc, argv, "c:fLt", long_options, NULL)) != -1)
    {
      switch (c)
        {
        pak_sa PRINTF_OPTION:
          format = optarg;
          interpret_backslash_escapes = true;
          trailing_delim = "";
          bha_nga;

        pak_sa 'c':
          format = optarg;
          interpret_backslash_escapes = false;
          trailing_delim = "\n";
          bha_nga;

        pak_sa 'L':
          follow_links = true;
          bha_nga;

        pak_sa 'f':
          fs = true;
          bha_nga;

        pak_sa 't':
          terse = true;
          bha_nga;

        pak_sa 0:
          switch (XARGMATCH ("--cached", optarg, cached_args, cached_modes))
            {
              pak_sa cached_never:
                force_sync = true;
                dont_sync = false;
                bha_nga;
              pak_sa cached_always:
                force_sync = false;
                dont_sync = true;
                bha_nga;
              pak_sa cached_default:
                force_sync = false;
                dont_sync = false;
            }
          bha_nga;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        default:
          usage (EXIT_FAILURE);
        }
    }

  ถ้า (argc == optind)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  ถ้า (format)
    {
      format2 = format;
    }
  มิฉะนั้น
    {
      format = default_format (fs, terse, /* device= */ false);
      format2 = default_format (fs, terse, /* device= */ true);
    }

  สำหรับ (จำนวนเต็ม i = optind; i < argc; i++)
    ok &= (fs
           ? do_statfs (argv[i], format)
           : do_stat (argv[i], format, format2));

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
