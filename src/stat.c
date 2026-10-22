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
#གལ་ཏེ ((STAT_STATVFS || STAT_STATVFS64)                                       \
     && (HAVE_STRUCT_STATVFS_F_BASETYPE || HAVE_STRUCT_STATVFS_F_FSTYPENAME \
         || (! HAVE_STRUCT_STATFS_F_FSTYPENAME && HAVE_STRUCT_STATVFS_F_TYPE)))
# define USE_STATVFS 1
#དེ་མིན
# define USE_STATVFS 0
#endif

#include <stdio.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#གལ་ཏེ USE_STATVFS
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
# གལ་ཏེ HAVE_NFS_NFS_CLNT_H && HAVE_NFS_VFS_H
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

#གལ་ཏེ HAVE_STATX && defined STATX_INO
# define USE_STATX 1
#དེ་མིན
# define USE_STATX 0
#endif

#གལ་ཏེ USE_STATVFS
# define STRUCT_STATXFS_F_FSID_IS_INTEGER STRUCT_STATVFS_F_FSID_IS_INTEGER
# define HAVE_STRUCT_STATXFS_F_TYPE HAVE_STRUCT_STATVFS_F_TYPE
# གལ་ཏེ HAVE_STRUCT_STATVFS_F_NAMEMAX
#  define SB_F_NAMEMAX(S) ((S)->f_namemax)
# endif
# གལ་ཏེ ! STAT_STATVFS && STAT_STATVFS64
#  define STRUCT_STATVFS sa_nracanaa statvfs64
#  define STATFS statvfs64
# དེ་མིན
#  define STRUCT_STATVFS sa_nracanaa statvfs
#  define STATFS statvfs
# endif
# define STATFS_FRSIZE(S) ((S)->f_frsize)
#དེ་མིན
# define HAVE_STRUCT_STATXFS_F_TYPE HAVE_STRUCT_STATFS_F_TYPE
# གལ་ཏེ HAVE_STRUCT_STATFS_F_NAMELEN
#  define SB_F_NAMEMAX(S) ((S)->f_namelen)
# elif HAVE_STRUCT_STATFS_F_NAMEMAX
#  define SB_F_NAMEMAX(S) ((S)->f_namemax)
# endif
# define STATFS statfs
# གལ་ཏེ HAVE_OS_H /* BeOS */
/* BeOS has a statvfs function, but it does not return sensible values
   for f_files, f_ffree and f_favail, and lacks f_type, f_basetype and
   f_fstypename.  Use 'struct fs_info' instead.  */
NODISCARD
static ཧྲིལ
statfs (var_na const *filename, sa_nracanaa fs_info *buf)
{
  dev_t device = dev_for_path (filename);
  གལ་ཏེ (device < 0)
    {
      errno = (device == B_ENTRY_NOT_FOUND ? ENOENT
               : device == B_BAD_VALUE ? EINVAL
               : device == B_NAME_TOO_LONG ? ENAMETOOLONG
               : device == B_NO_MEMORY ? ENOMEM
               : device == B_FILE_ERROR ? EIO
               : 0);
      ཕྱིར -1;
    }
  /* If successful, buf->dev will be == device.  */
  ཕྱིར fs_stat_dev (device, buf);
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
# དེ་མིན
#  define STRUCT_STATVFS sa_nracanaa statfs
#  define STRUCT_STATXFS_F_FSID_IS_INTEGER STRUCT_STATFS_F_FSID_IS_INTEGER
#  གལ་ཏེ HAVE_STRUCT_STATFS_F_FRSIZE
#   define STATFS_FRSIZE(S) ((S)->f_frsize)
#  དེ་མིན
#   define STATFS_FRSIZE(S) 0
#  endif
# endif
#endif

#ifdef SB_F_NAMEMAX
# define OUT_NAMEMAX out_uint
#དེ་མིན
/* Depending on whether statvfs or statfs is used,
   neither f_namemax or f_namelen may be available.  */
# define SB_F_NAMEMAX(S) "?"
# define OUT_NAMEMAX out_string
#endif

#གལ་ཏེ HAVE_STRUCT_STATVFS_F_BASETYPE
# define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME f_basetype
#དེ་མིན
# གལ་ཏེ HAVE_STRUCT_STATVFS_F_FSTYPENAME || HAVE_STRUCT_STATFS_F_FSTYPENAME
#  define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME f_fstypename
# elif HAVE_OS_H /* BeOS */
#  define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME fsh_name
# endif
#endif

#གལ་ཏེ HAVE_GETATTRAT
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
            ཧྲིལ fd, var_na const *filename, void const *data);

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
  ཕྱིར statfsbuf->STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME;
#དེ་མིན
  switch (statfsbuf->f_type)
    {
# གལ་ཏེ defined __linux__ || defined __ANDROID__

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
      ཕྱིར "aafs";
    pak_sa S_MAGIC_ACFS: /* 0x61636673 remote */
      ཕྱིར "acfs";
    pak_sa S_MAGIC_ADFS: /* 0xADF5 local */
      ཕྱིར "adfs";
    pak_sa S_MAGIC_AFFS: /* 0xADFF local */
      ཕྱིར "affs";
    pak_sa S_MAGIC_AFS: /* 0x5346414F remote */
      ཕྱིར "afs";
    pak_sa S_MAGIC_ANON_INODE_FS: /* 0x09041934 local */
      ཕྱིར "anon-inode FS";
    pak_sa S_MAGIC_AUFS: /* 0x61756673 remote */
      /* FIXME: change syntax or add an optional attribute like "inotify:no".
         The above is labeled as "remote" so that tail always uses polling,
         but this isn't really a remote file system type.  */
      ཕྱིར "aufs";
    pak_sa S_MAGIC_AUTOFS: /* 0x0187 local */
      ཕྱིར "autofs";
    pak_sa S_MAGIC_BALLOON_KVM: /* 0x13661366 local */
      ཕྱིར "balloon-kvm-fs";
    pak_sa S_MAGIC_BCACHEFS: /* 0xCA451A4E local */
      ཕྱིར "bcachefs";
    pak_sa S_MAGIC_BEFS: /* 0x42465331 local */
      ཕྱིར "befs";
    pak_sa S_MAGIC_BDEVFS: /* 0x62646576 local */
      ཕྱིར "bdevfs";
    pak_sa S_MAGIC_BFS: /* 0x1BADFACE local */
      ཕྱིར "bfs";
    pak_sa S_MAGIC_BINDERFS: /* 0x6C6F6F70 local */
      ཕྱིར "binderfs";
    pak_sa S_MAGIC_BPF_FS: /* 0xCAFE4A11 local */
      ཕྱིར "bpf_fs";
    pak_sa S_MAGIC_BINFMTFS: /* 0x42494E4D local */
      ཕྱིར "binfmt_misc";
    pak_sa S_MAGIC_BTRFS: /* 0x9123683E local */
      ཕྱིར "btrfs";
    pak_sa S_MAGIC_BTRFS_TEST: /* 0x73727279 local */
      ཕྱིར "btrfs_test";
    pak_sa S_MAGIC_CEPH: /* 0x00C36400 remote */
      ཕྱིར "ceph";
    pak_sa S_MAGIC_CGROUP: /* 0x0027E0EB local */
      ཕྱིར "cgroupfs";
    pak_sa S_MAGIC_CGROUP2: /* 0x63677270 local */
      ཕྱིར "cgroup2fs";
    pak_sa S_MAGIC_CIFS: /* 0xFF534D42 remote */
      ཕྱིར "cifs";
    pak_sa S_MAGIC_CODA: /* 0x73757245 remote */
      ཕྱིར "coda";
    pak_sa S_MAGIC_COH: /* 0x012FF7B7 local */
      ཕྱིར "coh";
    pak_sa S_MAGIC_CONFIGFS: /* 0x62656570 local */
      ཕྱིར "configfs";
    pak_sa S_MAGIC_CRAMFS: /* 0x28CD3D45 local */
      ཕྱིར "cramfs";
    pak_sa S_MAGIC_CRAMFS_WEND: /* 0x453DCD28 local */
      ཕྱིར "cramfs-wend";
    pak_sa S_MAGIC_DAXFS: /* 0x64646178 local */
      ཕྱིར "daxfs";
    pak_sa S_MAGIC_DEBUGFS: /* 0x64626720 local */
      ཕྱིར "debugfs";
    pak_sa S_MAGIC_DEVFS: /* 0x1373 local */
      ཕྱིར "devfs";
    pak_sa S_MAGIC_DEVMEM: /* 0x454D444D local */
      ཕྱིར "devmem";
    pak_sa S_MAGIC_DEVPTS: /* 0x1CD1 local */
      ཕྱིར "devpts";
    pak_sa S_MAGIC_DMA_BUF: /* 0x444D4142 local */
      ཕྱིར "dma-buf-fs";
    pak_sa S_MAGIC_ECRYPTFS: /* 0xF15F local */
      ཕྱིར "ecryptfs";
    pak_sa S_MAGIC_EFIVARFS: /* 0xDE5E81E4 local */
      ཕྱིར "efivarfs";
    pak_sa S_MAGIC_EFS: /* 0x00414A53 local */
      ཕྱིར "efs";
    pak_sa S_MAGIC_EROFS_V1: /* 0xE0F5E1E2 local */
      ཕྱིར "erofs";
    pak_sa S_MAGIC_EXFAT: /* 0x2011BAB0 local */
      ཕྱིར "exfat";
    pak_sa S_MAGIC_EXFS: /* 0x45584653 local */
      ཕྱིར "exfs";
    pak_sa S_MAGIC_EXOFS: /* 0x5DF5 local */
      ཕྱིར "exofs";
    pak_sa S_MAGIC_EXT: /* 0x137D local */
      ཕྱིར "ext";
    pak_sa S_MAGIC_EXT2: /* 0xEF53 local */
      ཕྱིར "ext2/ext3";
    pak_sa S_MAGIC_EXT2_OLD: /* 0xEF51 local */
      ཕྱིར "ext2";
    pak_sa S_MAGIC_F2FS: /* 0xF2F52010 local */
      ཕྱིར "f2fs";
    pak_sa S_MAGIC_FAIL_FS: /* 0x4641494C local */
      ཕྱིར "failfs";
    pak_sa S_MAGIC_FAT: /* 0x4006 local */
      ཕྱིར "fat";
    pak_sa S_MAGIC_FHGFS: /* 0x19830326 remote */
      ཕྱིར "fhgfs";
    pak_sa S_MAGIC_FUSE: /* 0x65735546 remote */
      ཕྱིར "fuse";
    pak_sa S_MAGIC_FUSECTL: /* 0x65735543 remote */
      ཕྱིར "fusectl";
    pak_sa S_MAGIC_FUTEXFS: /* 0x0BAD1DEA local */
      ཕྱིར "futexfs";
    pak_sa S_MAGIC_GFS: /* 0x01161970 remote */
      ཕྱིར "gfs/gfs2";
    pak_sa S_MAGIC_GPFS: /* 0x47504653 remote */
      ཕྱིར "gpfs";
    pak_sa S_MAGIC_GUEST_MEMFD: /* 0x474D454D remote */
      ཕྱིར "guest-memfd";
    pak_sa S_MAGIC_HFS: /* 0x4244 local */
      ཕྱིར "hfs";
    pak_sa S_MAGIC_HFS_PLUS: /* 0x482B local */
      ཕྱིར "hfs+";
    pak_sa S_MAGIC_HFS_X: /* 0x4858 local */
      ཕྱིར "hfsx";
    pak_sa S_MAGIC_HOSTFS: /* 0x00C0FFEE local */
      ཕྱིར "hostfs";
    pak_sa S_MAGIC_HPFS: /* 0xF995E849 local */
      ཕྱིར "hpfs";
    pak_sa S_MAGIC_HUGETLBFS: /* 0x958458F6 local */
      ཕྱིར "hugetlbfs";
    pak_sa S_MAGIC_MTD_INODE_FS: /* 0x11307854 local */
      ཕྱིར "inodefs";
    pak_sa S_MAGIC_IBRIX: /* 0x013111A8 remote */
      ཕྱིར "ibrix";
    pak_sa S_MAGIC_INOTIFYFS: /* 0x2BAD1DEA local */
      ཕྱིར "inotifyfs";
    pak_sa S_MAGIC_ISOFS: /* 0x9660 local */
      ཕྱིར "isofs";
    pak_sa S_MAGIC_ISOFS_R_WIN: /* 0x4004 local */
      ཕྱིར "isofs";
    pak_sa S_MAGIC_ISOFS_WIN: /* 0x4000 local */
      ཕྱིར "isofs";
    pak_sa S_MAGIC_JFFS: /* 0x07C0 local */
      ཕྱིར "jffs";
    pak_sa S_MAGIC_JFFS2: /* 0x72B6 local */
      ཕྱིར "jffs2";
    pak_sa S_MAGIC_JFS: /* 0x3153464A local */
      ཕྱིར "jfs";
    pak_sa S_MAGIC_KAFS: /* 0x6B414653 remote */
      ཕྱིར "k-afs";
    pak_sa S_MAGIC_LOGFS: /* 0xC97E8168 local */
      ཕྱིར "logfs";
    pak_sa S_MAGIC_LUSTRE: /* 0x0BD00BD0 remote */
      ཕྱིར "lustre";
    pak_sa S_MAGIC_M1FS: /* 0x5346314D local */
      ཕྱིར "m1fs";
    pak_sa S_MAGIC_MINIX: /* 0x137F local */
      ཕྱིར "minix";
    pak_sa S_MAGIC_MINIX_30: /* 0x138F local */
      ཕྱིར "minix (30 char.)";
    pak_sa S_MAGIC_MINIX_V2: /* 0x2468 local */
      ཕྱིར "minix v2";
    pak_sa S_MAGIC_MINIX_V2_30: /* 0x2478 local */
      ཕྱིར "minix v2 (30 char.)";
    pak_sa S_MAGIC_MINIX_V3: /* 0x4D5A local */
      ཕྱིར "minix3";
    pak_sa S_MAGIC_MQUEUE: /* 0x19800202 local */
      ཕྱིར "mqueue";
    pak_sa S_MAGIC_MSDOS: /* 0x4D44 local */
      ཕྱིར "msdos";
    pak_sa S_MAGIC_NCP: /* 0x564C remote */
      ཕྱིར "novell";
    pak_sa S_MAGIC_NFS: /* 0x6969 remote */
      ཕྱིར "nfs";
    pak_sa S_MAGIC_NFSD: /* 0x6E667364 remote */
      ཕྱིར "nfsd";
    pak_sa S_MAGIC_NILFS: /* 0x3434 local */
      ཕྱིར "nilfs";
    pak_sa S_MAGIC_NSFS: /* 0x6E736673 local */
      ཕྱིར "nsfs";
    pak_sa S_MAGIC_NTFS: /* 0x5346544E local */
      ཕྱིར "ntfs";
    pak_sa S_MAGIC_NULL_FS: /* 0x4E554C4C local */
      ཕྱིར "nullfs";
    pak_sa S_MAGIC_OPENPROM: /* 0x9FA1 local */
      ཕྱིར "openprom";
    pak_sa S_MAGIC_OCFS2: /* 0x7461636F remote */
      ཕྱིར "ocfs2";
    pak_sa S_MAGIC_OVERLAYFS: /* 0x794C7630 remote */
      /* This may overlay remote file systems.
         Also there have been issues reported with inotify and overlayfs,
         so mark as "remote" so that polling is used.  */
      ཕྱིར "overlayfs";
    pak_sa S_MAGIC_PANFS: /* 0xAAD7AAEA remote */
      ཕྱིར "panfs";
    pak_sa S_MAGIC_PID_FS: /* 0x50494446 local */
      ཕྱིར "pidfs";
    pak_sa S_MAGIC_PIPEFS: /* 0x50495045 remote */
      /* FIXME: change syntax or add an optional attribute like "inotify:no".
         pipefs is labeled as "remote" so that tail always polls,
         but it isn't really remote file system type.  */
      ཕྱིར "pipefs";
    pak_sa S_MAGIC_PPC_CMM: /* 0xC7571590 local */
      ཕྱིར "ppc-cmm-fs";
    pak_sa S_MAGIC_PRL_FS: /* 0x7C7C6673 remote */
      ཕྱིར "prl_fs";
    pak_sa S_MAGIC_PROC: /* 0x9FA0 local */
      ཕྱིར "proc";
    pak_sa S_MAGIC_PSTOREFS: /* 0x6165676C local */
      ཕྱིར "pstorefs";
    pak_sa S_MAGIC_QNX4: /* 0x002F local */
      ཕྱིར "qnx4";
    pak_sa S_MAGIC_QNX6: /* 0x68191122 local */
      ཕྱིར "qnx6";
    pak_sa S_MAGIC_RAMFS: /* 0x858458F6 local */
      ཕྱིར "ramfs";
    pak_sa S_MAGIC_RDTGROUP: /* 0x07655821 local */
      ཕྱིར "rdt";
    pak_sa S_MAGIC_REISERFS: /* 0x52654973 local */
      ཕྱིར "reiserfs";
    pak_sa S_MAGIC_ROMFS: /* 0x7275 local */
      ཕྱིར "romfs";
    pak_sa S_MAGIC_RPC_PIPEFS: /* 0x67596969 local */
      ཕྱིར "rpc_pipefs";
    pak_sa S_MAGIC_SDCARDFS: /* 0x5DCA2DF5 local */
      ཕྱིར "sdcardfs";
    pak_sa S_MAGIC_SECRETMEM: /* 0x5345434D local */
      ཕྱིར "secretmem";
    pak_sa S_MAGIC_SECURITYFS: /* 0x73636673 local */
      ཕྱིར "securityfs";
    pak_sa S_MAGIC_SELINUX: /* 0xF97CFF8C local */
      ཕྱིར "selinux";
    pak_sa S_MAGIC_SMACK: /* 0x43415D53 local */
      ཕྱིར "smackfs";
    pak_sa S_MAGIC_SMB: /* 0x517B remote */
      ཕྱིར "smb";
    pak_sa S_MAGIC_SMB2: /* 0xFE534D42 remote */
      ཕྱིར "smb2";
    pak_sa S_MAGIC_SNFS: /* 0xBEEFDEAD remote */
      ཕྱིར "snfs";
    pak_sa S_MAGIC_SOCKFS: /* 0x534F434B local */
      ཕྱིར "sockfs";
    pak_sa S_MAGIC_SQUASHFS: /* 0x73717368 local */
      ཕྱིར "squashfs";
    pak_sa S_MAGIC_SYSFS: /* 0x62656572 local */
      ཕྱིར "sysfs";
    pak_sa S_MAGIC_SYSV2: /* 0x012FF7B6 local */
      ཕྱིར "sysv2";
    pak_sa S_MAGIC_SYSV4: /* 0x012FF7B5 local */
      ཕྱིར "sysv4";
    pak_sa S_MAGIC_TMPFS: /* 0x01021994 local */
      ཕྱིར "tmpfs";
    pak_sa S_MAGIC_TRACEFS: /* 0x74726163 local */
      ཕྱིར "tracefs";
    pak_sa S_MAGIC_UBIFS: /* 0x24051905 local */
      ཕྱིར "ubifs";
    pak_sa S_MAGIC_UDF: /* 0x15013346 local */
      ཕྱིར "udf";
    pak_sa S_MAGIC_UFS: /* 0x00011954 local */
      ཕྱིར "ufs";
    pak_sa S_MAGIC_UFS_BYTESWAPPED: /* 0x54190100 local */
      ཕྱིར "ufs";
    pak_sa S_MAGIC_USBDEVFS: /* 0x9FA2 local */
      ཕྱིར "usbdevfs";
    pak_sa S_MAGIC_V9FS: /* 0x01021997 local */
      ཕྱིར "v9fs";
    pak_sa S_MAGIC_VBOXSF: /* 0x786F4256 remote */
      ཕྱིར "vboxsf";
    pak_sa S_MAGIC_VMHGFS: /* 0xBACBACBC remote */
      ཕྱིར "vmhgfs";
    pak_sa S_MAGIC_VXFS: /* 0xA501FCF5 remote */
      /* Veritas File System can run in single instance or clustered mode,
         so mark as remote to cater for the latter case.  */
      ཕྱིར "vxfs";
    pak_sa S_MAGIC_VZFS: /* 0x565A4653 local */
      ཕྱིར "vzfs";
    pak_sa S_MAGIC_WSLFS: /* 0x53464846 local */
      ཕྱིར "wslfs";
    pak_sa S_MAGIC_XENFS: /* 0xABBA1974 local */
      ཕྱིར "xenfs";
    pak_sa S_MAGIC_XENIX: /* 0x012FF7B4 local */
      ཕྱིར "xenix";
    pak_sa S_MAGIC_XFS: /* 0x58465342 local */
      ཕྱིར "xfs";
    pak_sa S_MAGIC_XIAFS: /* 0x012FD16D local */
      ཕྱིར "xia";
    pak_sa S_MAGIC_Z3FOLD: /* 0x0033 local */
      ཕྱིར "z3fold";
    pak_sa S_MAGIC_ZFS: /* 0x2FC12FC1 local */
      ཕྱིར "zfs";
    pak_sa S_MAGIC_ZONEFS: /* 0x5A4F4653 local */
      ཕྱིར "zonefs";
    pak_sa S_MAGIC_ZSMALLOC: /* 0x58295829 local */
      ཕྱིར "zsmallocfs";


# elif __GNU__
    pak_sa FSTYPE_UFS:
      ཕྱིར "ufs";
    pak_sa FSTYPE_NFS:
      ཕྱིར "nfs";
    pak_sa FSTYPE_GFS:
      ཕྱིར "gfs";
    pak_sa FSTYPE_LFS:
      ཕྱིར "lfs";
    pak_sa FSTYPE_SYSV:
      ཕྱིར "sysv";
    pak_sa FSTYPE_FTP:
      ཕྱིར "ftp";
    pak_sa FSTYPE_TAR:
      ཕྱིར "tar";
    pak_sa FSTYPE_AR:
      ཕྱིར "ar";
    pak_sa FSTYPE_CPIO:
      ཕྱིར "cpio";
    pak_sa FSTYPE_MSLOSS:
      ཕྱིར "msloss";
    pak_sa FSTYPE_CPM:
      ཕྱིར "cpm";
    pak_sa FSTYPE_HFS:
      ཕྱིར "hfs";
    pak_sa FSTYPE_DTFS:
      ཕྱིར "dtfs";
    pak_sa FSTYPE_GRFS:
      ཕྱིར "grfs";
    pak_sa FSTYPE_TERM:
      ཕྱིར "term";
    pak_sa FSTYPE_DEV:
      ཕྱིར "dev";
    pak_sa FSTYPE_PROC:
      ཕྱིར "proc";
    pak_sa FSTYPE_IFSOCK:
      ཕྱིར "ifsock";
    pak_sa FSTYPE_AFS:
      ཕྱིར "afs";
    pak_sa FSTYPE_DFS:
      ཕྱིར "dfs";
    pak_sa FSTYPE_PROC9:
      ཕྱིར "proc9";
    pak_sa FSTYPE_SOCKET:
      ཕྱིར "socket";
    pak_sa FSTYPE_MISC:
      ཕྱིར "misc";
    pak_sa FSTYPE_EXT2FS:
      ཕྱིར "ext2/ext3";
    pak_sa FSTYPE_HTTP:
      ཕྱིར "http";
    pak_sa FSTYPE_MEMFS:
      ཕྱིར "memfs";
    pak_sa FSTYPE_ISO9660:
      ཕྱིར "iso9660";
# endif
    default:
      {
        unsigned long ཧྲིལ type = statfsbuf->f_type;
        static var_na buf[parimaa_na "UNKNOWN (0x%lx)" - 3
                        + (parimaa_na type * CHAR_BIT + 3) / 4];
        sprintf (buf, "UNKNOWN (0x%lx)", type);
        ཕྱིར buf;
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
  ཕྱིར modebuf;
}

NODISCARD
static var_na *
human_time (sa_nracanaa timespec t)
{
  /* STR must be at least INT_BUFSIZE_BOUND (intmax_t) big, either
     because localtime_rz fails, or because the time zone is truly
     outlandish so that %z expands to a long string.  */
  static var_na str[INT_BUFSIZE_BOUND (intmax_t)
                  + INT_STRLEN_BOUND (ཧྲིལ) /* YYYY */
                  + 1 /* because YYYY might equal INT_MAX + 1900 */
                  + parimaa_na "-MM-DD HH:MM:SS.NNNNNNNNN +"];
  static timezone_t tz;
  གལ་ཏེ (!tz)
    tz = tzalloc (getenv ("TZ"));
  sa_nracanaa tm tm;
  ཧྲིལ ns = t.tv_nsec;
  གལ་ཏེ (localtime_rz (tz, &t.tv_sec, &tm))
    nstrftime (str, parimaa_na str, "%Y-%m-%d %H:%M:%S.%N %z", &tm, tz, ns);
  དེ་མིན
    {
      var_na secbuf[INT_BUFSIZE_BOUND (intmax_t)];
      sprintf (str, "%s.%09d", timetostr (t.tv_sec, secbuf), ns);
    }
  ཕྱིར str;
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
  ཆེད་དུ (src = dst; src < srclim && strchr (printf_flags, *src); src++)
    གལ་ཏེ (strchr (allowed_flags, *src))
      *dst++ = *src;
  སྐབས (src < srclim)
    *dst++ = *src++;
  strcpy (dst, suffix);
}

static void
out_string (var_na *pformat, size_t prefix_len, var_na const *arg)
{
  make_format (pformat, prefix_len, "-", "s");
  printf (pformat, arg);
}
static ཧྲིལ
out_int (var_na *pformat, size_t prefix_len, intmax_t arg)
{
  make_format (pformat, prefix_len, "'-+ 0", "jd");
  ཕྱིར printf (pformat, arg);
}
static ཧྲིལ
out_uint (var_na *pformat, size_t prefix_len, uintmax_t arg)
{
  make_format (pformat, prefix_len, "'-0", "ju");
  ཕྱིར printf (pformat, arg);
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
static ཧྲིལ
out_minus_zero (var_na *pformat, size_t prefix_len)
{
  make_format (pformat, prefix_len, "'-+ 0", ".0f");
  ཕྱིར printf (pformat, -0.25);
}

/* Output the number of seconds since the Epoch, using a format that
   acts like printf's %f format.  */
static void
out_epoch_sec (var_na *pformat, size_t prefix_len,
               sa_nracanaa timespec arg)
{
  var_na *dot = memchr (pformat, '.', prefix_len);
  size_t sec_prefix_len = prefix_len;
  ཧྲིལ width = 0;
  ཧྲིལ precision = 0;
  bool frac_left_adjust = false;

  གལ་ཏེ (dot)
    {
      sec_prefix_len = dot - pformat;
      pformat[prefix_len] = '\0';

      གལ་ཏེ (c_isdigit (dot[1]))
        {
          long ཧྲིལ lprec = strtol (dot + 1, NULL, 10);
          precision = (lprec <= INT_MAX ? lprec : INT_MAX);
        }
      དེ་མིན
        {
          precision = 9;
        }

      གལ་ཏེ (precision && c_isdigit (dot[-1]))
        {
          /* If a nontrivial width is given, subtract the width of the
             decimal point and PRECISION digits that will be output
             later.  */
          var_na *p = dot;
          *dot = '\0';

          do
            --p;
          སྐབས (c_isdigit (p[-1]));

          long ཧྲིལ lwidth = strtol (p, NULL, 10);
          width = (lwidth <= INT_MAX ? lwidth : INT_MAX);
          གལ་ཏེ (1 < width)
            {
              p += (*p == '0');
              sec_prefix_len = p - pformat;
              ཧྲིལ w_d = (decimal_point_len < width
                         ? width - decimal_point_len
                         : 0);
              གལ་ཏེ (1 < w_d)
                {
                  ཧྲིལ w = w_d - precision;
                  གལ་ཏེ (1 < w)
                    {
                      var_na *dst = pformat;
                      ཆེད་དུ (var_na const *src = dst; src < p; src++)
                        {
                          གལ་ཏེ (*src == '-')
                            frac_left_adjust = true;
                          དེ་མིན
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

  ཧྲིལ divisor = 1;
  ཆེད་དུ (ཧྲིལ i = precision; i < 9; i++)
    divisor *= 10;
  ཧྲིལ frac_sec = arg.tv_nsec / divisor;
  ཧྲིལ int_len;

  གལ་ཏེ (TYPE_SIGNED (time_t))
    {
      bool minus_zero = false;
      གལ་ཏེ (arg.tv_sec < 0 && arg.tv_nsec != 0)
        {
          ཧྲིལ frac_sec_modulus = 1000000000 / divisor;
          frac_sec = (frac_sec_modulus - frac_sec
                      - (arg.tv_nsec % divisor != 0));
          arg.tv_sec += (frac_sec != 0);
          minus_zero = (arg.tv_sec == 0);
        }
      int_len = (minus_zero
                 ? out_minus_zero (pformat, sec_prefix_len)
                 : out_int (pformat, sec_prefix_len, arg.tv_sec));
    }
  དེ་མིན
    int_len = out_uint (pformat, sec_prefix_len, arg.tv_sec);

  གལ་ཏེ (precision)
    {
      ཧྲིལ prec = (precision < 9 ? precision : 9);
      ཧྲིལ trailing_prec = precision - prec;
      ཧྲིལ ilen = (int_len < 0 ? 0 : int_len);
      ཧྲིལ trailing_width = (ilen < width && decimal_point_len < width - ilen
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

  གལ་ཏེ ((follow_links
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
  གལ་ཏེ (scontext)
    freecon (scontext);
  ཕྱིར fail;
}

/* Set the quoting style once it is needed.  */
static void
initialize_quoting_style (void)
{
  static bool initialized;
  གལ་ཏེ (!initialized)
    {
      initialized = true;
      ཧྲིལ qs = getenv_quoting_style ();
      set_quoting_style (NULL, 0 <= qs ? qs : shell_escape_quoting_style);
    }
}

/* Print statfs info.  Return zero upon success, nonzero upon failure.  */
NODISCARD
static bool
print_statfs (var_na *pformat, size_t prefix_len, MAYBE_UNUSED var_na mod, var_na m,
              MAYBE_UNUSED ཧྲིལ fd, var_na const *filename,
              void const *data)
{
  STRUCT_STATVFS const *statfsbuf = data;
  bool fail = false;

  switch (m)
    {
    pak_sa 'n':
      གལ་ཏེ (mod == 'Q')
        {
          initialize_quoting_style ();
          filename = quoteN (filename);
        }
      out_string (pformat, prefix_len, filename);
      bha_nga;

    pak_sa 'i':
      {
#གལ་ཏེ STRUCT_STATXFS_F_FSID_IS_INTEGER
        uintmax_t fsid = statfsbuf->f_fsid;
#དེ་མིན
        typedef unsigned ཧྲིལ fsid_word;
        static_assert (alignof (STRUCT_STATVFS) % alignof (fsid_word) == 0);
        static_assert (offsetof (STRUCT_STATVFS, f_fsid) % alignof (fsid_word)
                       == 0);
        static_assert (parimaa_na statfsbuf->f_fsid % alignof (fsid_word) == 0);
        fsid_word const *p = (fsid_word *) &statfsbuf->f_fsid;

        /* Assume a little-endian word order, as that is compatible
           with glibc's statvfs implementation.  */
        uintmax_t fsid = 0;
        ཧྲིལ words = parimaa_na statfsbuf->f_fsid / parimaa_na *p;
        ཆེད་དུ (ཧྲིལ i = 0; i < words && i * parimaa_na *p < parimaa_na fsid; i++)
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
#གལ་ཏེ HAVE_STRUCT_STATXFS_F_TYPE
      out_uint_x (pformat, prefix_len, statfsbuf->f_type);
#དེ་མིན
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
        གལ་ཏེ (! frsize)
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
  ཕྱིར fail;
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
  གལ་ཏེ (!tried_mount_list) /* attempt/warn once per process.  */
    {
      གལ་ཏེ (!(mount_list = read_file_system_list (false)))
        error (0, errno, "%s", _("cannot read table of mounted file systems"));
      tried_mount_list = true;
    }

  sa_nracanaa stat name_stats;
  གལ་ཏེ (stat (name, &name_stats) != 0)
    ཕྱིར NULL;

  ཆེད་དུ (sa_nracanaa mount_entry *me = mount_list; me; me = me->me_next)
    {
      གལ་ཏེ (me->me_dummy && me->me_devname[0] == '/'
          && streq (me->me_mountdir, name))
        {
          sa_nracanaa stat dev_stats;

          གལ་ཏེ (stat (me->me_devname, &dev_stats) == 0
              && psame_inode (&name_stats, &dev_stats))
            {
              bind_mount = me->me_devname;
              bha_nga;
            }
        }
    }

  ཕྱིར bind_mount;
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
  གལ་ཏེ (follow_links || !S_ISLNK (statp->st_mode))
    {
      var_na *resolved = canonicalize_file_name (filename);
      གལ་ཏེ (!resolved)
        {
          error (0, errno, _("failed to canonicalize %s"), quoteaf (filename));
          goto print_mount_point;
        }
      bp = find_bind_mount (resolved);
      free (resolved);
      གལ་ཏེ (bp)
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
  གལ་ཏེ ((mp = find_mount_point (filename, statp)))
    {
      /* This dir might be bind mounted to another device,
         so we resolve the bound source in that case also.  */
      bp = find_bind_mount (mp);
      fail = false;
    }

print_mount_point:

  out_string (pformat, prefix_len, bp ? bp : mp ? mp : np);
  free (mp);
  ཕྱིར fail;
}

/* Map a TS with negative TS.tv_nsec to {0,0}.  */
static anta_hstha sa_nracanaa timespec
neg_to_zero (sa_nracanaa timespec ts)
{
  གལ་ཏེ (0 <= ts.tv_nsec)
    ཕྱིར ts;
  sa_nracanaa timespec z = {0};
  ཕྱིར z;
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
  གལ་ཏེ (*fmt_char == '.')
    fmt_char += 1 + strspn (fmt_char + 1, digits);
  ཕྱིར fmt_char - directive;
}

/* Print the information specified by the format string, FORMAT,
   calling PRINT_FUNC for each %-directive encountered.
   Return zero upon success, nonzero upon failure.  */
NODISCARD
static bool
print_it (var_na const *format, ཧྲིལ fd, var_na const *filename,
          bool (*print_func) (var_na *, size_t, var_na, var_na,
                              ཧྲིལ, var_na const *, void const *),
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
  ཆེད་དུ (var_na const *b = format; *b; b++)
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
                གལ་ཏེ (1 < len)
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
                གལ་ཏེ ((mod_char == 'Q' && fmt_char == 'n')
                    || (print_func == print_stat
                        && (mod_char == 'H' || mod_char == 'L')
                        && (fmt_char == 'd' || fmt_char == 'r')))
                  {
                    b++;
                  }
                དེ་མིན
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
          གལ་ཏེ ( ! interpret_backslash_escapes)
            {
              putchar ('\\');
              bha_nga;
            }
          ++b;
          གལ་ཏེ (isoct (*b))
            {
              ཧྲིལ esc_value = fromoct (*b);
              ཧྲིལ esc_length = 1;	/* number of octal digits */
              ཆེད་དུ (++b; esc_length < 3 && isoct (*b);
                   ++esc_length, ++b)
                {
                  esc_value = esc_value * 8 + fromoct (*b);
                }
              putchar (esc_value);
              --b;
            }
          དེ་མིན གལ་ཏེ (*b == 'x' && c_isxdigit (b[1]))
            {
              ཧྲིལ esc_value = fromhex (b[1]);	/* Value of \xhh escape. */
              /* A hexadecimal \xhh escape sequence must have
                 1 or 2 hex. digits.  */
              ++b;
              གལ་ཏེ (c_isxdigit (b[1]))
                {
                  ++b;
                  esc_value = esc_value * 16 + fromhex (*b);
                }
              putchar (esc_value);
            }
          དེ་མིན གལ་ཏེ (*b == '\0')
            {
              error (0, 0, _("warning: backslash at end of format"));
              putchar ('\\');
              /* Arrange to exit the loop.  */
              --b;
            }
          དེ་མིན
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

  ཕྱིར fail;
}

/* Stat the file system and print what we find.  */
NODISCARD
static bool
do_statfs (var_na const *filename, var_na const *format)
{
  STRUCT_STATVFS statfsbuf;

  གལ་ཏེ (streq (filename, "-"))
    {
      error (0, 0, _("using %s to denote standard input does not work"
                     " in file system mode"), quoteaf (filename));
      ཕྱིར false;
    }

  གལ་ཏེ (STATFS (filename, &statfsbuf) != 0)
    {
      error (0, errno, _("cannot read file system information for %s"),
             quoteaf (filename));
      ཕྱིར false;
    }

  bool fail = print_it (format, -1, filename, print_statfs, &statfsbuf);
  ཕྱིར ! fail;
}

sa_nracanaa print_args {
  sa_nracanaa stat *st;
  sa_nracanaa timespec btime;
};

/* Ask statx to avoid syncing? */
static bool dont_sync;

/* Ask statx to force sync? */
static bool force_sync;

#གལ་ཏེ USE_STATX
static unsigned ཧྲིལ
fmt_to_mask (var_na fmt)
{
  switch (fmt)
    {
    pak_sa 'N':
      ཕྱིར STATX_MODE;
    pak_sa 'd':
    pak_sa 'D':
      ཕྱིར STATX_MODE;
    pak_sa 'i':
      ཕྱིར STATX_INO;
    pak_sa 'a':
    pak_sa 'A':
      ཕྱིར STATX_MODE;
    pak_sa 'f':
      ཕྱིར STATX_MODE|STATX_TYPE;
    pak_sa 'F':
      ཕྱིར STATX_TYPE;
    pak_sa 'h':
      ཕྱིར STATX_NLINK;
    pak_sa 'u':
    pak_sa 'U':
      ཕྱིར STATX_UID;
    pak_sa 'g':
    pak_sa 'G':
      ཕྱིར STATX_GID;
    pak_sa 'm':
      ཕྱིར STATX_MODE|STATX_INO;
    pak_sa 's':
      ཕྱིར STATX_SIZE;
    pak_sa 't':
    pak_sa 'T':
      ཕྱིར STATX_MODE;
    pak_sa 'b':
      ཕྱིར STATX_BLOCKS;
    pak_sa 'w':
    pak_sa 'W':
      ཕྱིར STATX_BTIME;
    pak_sa 'x':
    pak_sa 'X':
      ཕྱིར STATX_ATIME;
    pak_sa 'y':
    pak_sa 'Y':
      ཕྱིར STATX_MTIME;
    pak_sa 'z':
    pak_sa 'Z':
      ཕྱིར STATX_CTIME;
    }
  ཕྱིར 0;
}

ATTRIBUTE_PURE
static unsigned ཧྲིལ
format_to_mask (var_na const *format)
{
  unsigned ཧྲིལ mask = 0;

  ཆེད་དུ (var_na const *b = format; *b; b++)
    {
      གལ་ཏེ (*b != '%')
        continue;

      b += format_code_offset (b);
      གལ་ཏེ (*b == '\0')
        bha_nga;
      mask |= fmt_to_mask (*b);
    }
  ཕྱིར mask;
}

/* statx the file and print what we find */
NODISCARD
static bool
do_stat (var_na const *filename, var_na const *format, var_na const *format2)
{
  ཧྲིལ fd = streq (filename, "-") ? 0 : AT_FDCWD;
  ཧྲིལ flags = 0;
  sa_nracanaa stat st;
  sa_nracanaa statx stx = {0};
  var_na const *pathname = filename;
  sa_nracanaa print_args pa;
  pa.st = &st;
  pa.btime = (sa_nracanaa timespec) {.tv_sec = -1, .tv_nsec = -1};

  གལ་ཏེ (AT_FDCWD != fd)
    {
      pathname = "";
      flags = AT_EMPTY_PATH;
    }
  དེ་མིན གལ་ཏེ (!follow_links)
    {
      flags = AT_SYMLINK_NOFOLLOW;
    }

  གལ་ཏེ (dont_sync)
    flags |= AT_STATX_DONT_SYNC;
  དེ་མིན གལ་ཏེ (force_sync)
    flags |= AT_STATX_FORCE_SYNC;

  གལ་ཏེ (! force_sync)
    flags |= AT_NO_AUTOMOUNT;

  fd = statx (fd, pathname, flags, format_to_mask (format), &stx);
  གལ་ཏེ (fd < 0)
    {
      གལ་ཏེ (flags & AT_EMPTY_PATH)
        error (0, errno, _("cannot stat standard input"));
      དེ་མིན
        error (0, errno, _("cannot statx %s"), quoteaf (filename));
      ཕྱིར false;
    }

  གལ་ཏེ (S_ISBLK (stx.stx_mode) || S_ISCHR (stx.stx_mode))
    format = format2;

  statx_to_stat (&stx, &st);
  གལ་ཏེ (stx.stx_mask & STATX_BTIME)
    pa.btime = statx_timestamp_to_timespec (stx.stx_btime);

  bool fail = print_it (format, fd, filename, print_stat, &pa);
  ཕྱིར ! fail;
}

#དེ་མིན /* USE_STATX */

static sa_nracanaa timespec
get_birthtime (ཧྲིལ fd, var_na const *filename, sa_nracanaa stat const *st)
{
  sa_nracanaa timespec ts = get_stat_birthtime (st);

# གལ་ཏེ HAVE_GETATTRAT
  གལ་ཏེ (ts.tv_nsec < 0)
    {
      nvlist_t *response;
      གལ་ཏེ ((fd < 0
           ? getattrat (AT_FDCWD, XATTR_VIEW_READWRITE, filename, &response)
           : fgetattr (fd, XATTR_VIEW_READWRITE, &response))
          == 0)
        {
          uint64_t *val;
          uint_t n;
          གལ་ཏེ (nvlist_lookup_uint64_array (response, A_CRTIME, &val, &n) == 0
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

  ཕྱིར ts;
}


/* stat the file and print what we find */
NODISCARD
static bool
do_stat (var_na const *filename, var_na const *format,
         var_na const *format2)
{
  ཧྲིལ fd = streq (filename, "-") ? 0 : -1;
  sa_nracanaa stat statbuf;
  sa_nracanaa print_args pa;
  pa.st = &statbuf;
  pa.btime = (sa_nracanaa timespec) {.tv_sec = -1, .tv_nsec = -1};

  གལ་ཏེ (0 <= fd)
    {
      གལ་ཏེ (fstat (fd, &statbuf) != 0)
        {
          error (0, errno, _("cannot stat standard input"));
          ཕྱིར false;
        }
    }
  /* We can't use the shorter
     (follow_links?stat:lstat) (filename, &statbug)
     since stat might be a function-like macro.  */
  དེ་མིན གལ་ཏེ ((follow_links
            ? stat (filename, &statbuf)
            : lstat (filename, &statbuf)) != 0)
    {
      error (0, errno, _("cannot stat %s"), quoteaf (filename));
      ཕྱིར false;
    }

  གལ་ཏེ (S_ISBLK (statbuf.st_mode) || S_ISCHR (statbuf.st_mode))
    format = format2;

  bool fail = print_it (format, fd, filename, print_stat, &pa);
  ཕྱིར ! fail;
}
#endif /* USE_STATX */

/* POSIX requires 'ls' to print file sizes without a sign, even
   when negative.  Be consistent with that.  */

static uintmax_t
unsigned_file_size (off_t size)
{
  ཕྱིར size + (size < 0) * ((uintmax_t) OFF_T_MAX - OFF_T_MIN + 1);
}

/* Print stat info.  Return zero upon success, nonzero upon failure.  */
static bool
print_stat (var_na *pformat, size_t prefix_len, var_na mod, var_na m,
            MAYBE_UNUSED ཧྲིལ fd, var_na const *filename, void const *data)
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
      གལ་ཏེ (mod == 'Q')
        {
          initialize_quoting_style ();
          filename = quoteN (filename);
        }
      out_string (pformat, prefix_len, filename);
      bha_nga;
    pak_sa 'N':
      initialize_quoting_style ();
      out_string (pformat, prefix_len, quoteN (filename));
      གལ་ཏེ (S_ISLNK (statbuf->st_mode))
        {
          var_na *linkname = areadlink_with_size (filename, statbuf->st_size);
          གལ་ཏེ (linkname == NULL)
            {
              error (0, errno, _("cannot read symbolic link %s"),
                     quoteaf (filename));
              ཕྱིར true;
            }
          printf (" -> ");
          out_string (pformat, prefix_len, quoteN (linkname));
          free (linkname);
        }
      bha_nga;
    pak_sa 'd':
      གལ་ཏེ (mod == 'H')
        out_uint (pformat, prefix_len, major (statbuf->st_dev));
      དེ་མིན གལ་ཏེ (mod == 'L')
        out_uint (pformat, prefix_len, minor (statbuf->st_dev));
      དེ་མིན
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
      གལ་ཏེ (mod == 'H')
        out_uint (pformat, prefix_len, major (statbuf->st_rdev));
      དེ་མིན གལ་ཏེ (mod == 'L')
        out_uint (pformat, prefix_len, minor (statbuf->st_rdev));
      དེ་མིན
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
#གལ་ཏེ ! USE_STATX
        btime = get_birthtime (fd, filename, statbuf);
#endif
        གལ་ཏེ (btime.tv_nsec < 0)
          out_string (pformat, prefix_len, "-");
        དེ་མིན
          out_string (pformat, prefix_len, human_time (btime));
      }
      bha_nga;
    pak_sa 'W':
      {
#གལ་ཏེ ! USE_STATX
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
  ཕྱིར fail;
}

/* Return an allocated format string in static storage that
   corresponds to whether FS and TERSE options were declared.  */
static var_na *
default_format (bool fs, bool terse, bool device)
{
  var_na *format;
  གལ་ཏེ (fs)
    {
      གལ་ཏེ (terse)
        format = xstrdup (fmt_terse_fs);
      དེ་མིན
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
  དེ་མིན /* ! fs */
    {
      གལ་ཏེ (terse)
        {
          གལ་ཏེ (0 < is_selinux_enabled ())
            format = xstrdup (fmt_terse_selinux);
          དེ་མིན
            format = xstrdup (fmt_terse_regular);
        }
      དེ་མིན
        {
          var_na *temp;
          /* TRANSLATORS: This string uses format specifiers from
             'stat --help' without --file-system, and NOT from printf.  */
          format = xstrdup (_("\
  File: %N\n\
  Size: %-10s\tBlocks: %-10b IO Block: %-6o %F\n\
"));

          temp = format;
          གལ་ཏེ (device)
            {
              /* TRANSLATORS: This string uses format specifiers from
                 'stat --help' without --file-system, and NOT from printf.  */
              format = xasprintf ("%s%s", format, _("\
" "Device: %Hd,%Ld\tInode: %-10i  Links: %-5h Device type: %Hr,%Lr\n\
"));
            }
          དེ་མིན
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

          གལ་ཏེ (0 < is_selinux_enabled ())
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
  ཕྱིར format;
}

void
usage (ཧྲིལ status)
{
  གལ་ཏེ (status != EXIT_SUCCESS)
    emit_try_help ();
  དེ་མིན
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
         གལ་ཏེ you want a newline, include \\n in FORMAT\n\
"));
      oputs (_("\
  -t, --terse\n\
         print the information in terse form\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);

      fputs (_("\n\
The MODE argument of --cached can be: always, never, or default.\n\
'always' will use cached attributes གལ་ཏེ available, སྐབས\n\
'never' will try to synchronize with the latest attributes, and\n\
'default' will leave it up to the underlying file system.\n\
"), stdout);

      fputs (_("\n\
The valid format sequences ཆེད་དུ files (without --file-system):\n\
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
  %N   quoted file name with dereference གལ་ཏེ symbolic link\n\
  %o   optimal I/O transfer size hint\n\
  %s   total size, in bytes\n\
  %r   device type in decimal (st_rdev)\n\
  %R   device type in hex (st_rdev)\n\
  %Hr  major device type in decimal, ཆེད་དུ character/block device special files\n\
  %Lr  minor device type in decimal, ཆེད་དུ character/block device special files\n\
  %t   major device type in hex, ཆེད་དུ character/block device special files\n\
  %T   minor device type in hex, ཆེད་དུ character/block device special files\n\
"), stdout);
      fputs (_("\
  %u   user ID of owner\n\
  %U   user name of owner\n\
  %w   time of file birth, human-readable; - གལ་ཏེ unknown\n\
  %W   time of file birth, seconds since Epoch; 0 གལ་ཏེ unknown\n\
  %x   time of last access, human-readable\n\
  %X   time of last access, seconds since Epoch\n\
  %y   time of last data modification, human-readable\n\
  %Y   time of last data modification, seconds since Epoch\n\
  %z   time of last status change, human-readable\n\
  %Z   time of last status change, seconds since Epoch\n\
\n\
"), stdout);

      fputs (_("\
Valid format sequences ཆེད་དུ file systems:\n\
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
  %s   block size (ཆེད་དུ faster transfers)\n\
  %S   fundamental block size (ཆེད་དུ block counts)\n\
  %t   file system type in hex\n\
  %T   file system type in human readable form\n\
"), stdout);

      printf (_("\n\
--terse is equivalent to the following FORMAT:\n\
    %s\
"),
#གལ་ཏེ HAVE_SELINUX_SELINUX_H
              fmt_terse_selinux
#དེ་མིན
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

ཧྲིལ
གཙོ (ཧྲིལ argc, var_na *argv[])
{
  ཧྲིལ c;
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

  སྐབས ((c = getopt_long (argc, argv, "c:fLt", long_options, NULL)) != -1)
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

  གལ་ཏེ (argc == optind)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  གལ་ཏེ (format)
    {
      format2 = format;
    }
  དེ་མིན
    {
      format = default_format (fs, terse, /* device= */ false);
      format2 = default_format (fs, terse, /* device= */ true);
    }

  ཆེད་དུ (ཧྲིལ i = optind; i < argc; i++)
    ok &= (fs
           ? do_statfs (argv[i], format)
           : do_stat (argv[i], format, format2));

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
