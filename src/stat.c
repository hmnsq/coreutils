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

#شامل <config.h>

/* Keep this conditional in sync with the similar conditional in
   ../m4/stat-prog.m4.  */
#اگر ((STAT_STATVFS || STAT_STATVFS64)                                       \
     && (HAVE_STRUCT_STATVFS_F_BASETYPE || HAVE_STRUCT_STATVFS_F_FSTYPENAME \
         || (! HAVE_STRUCT_STATFS_F_FSTYPENAME && HAVE_STRUCT_STATVFS_F_TYPE)))
# define USE_STATVFS 1
#وگرنه
# define USE_STATVFS 0
#endif

#شامل <stdio.h>
#شامل <sys/types.h>
#شامل <pwd.h>
#شامل <grp.h>
#اگر USE_STATVFS
# include <sys/statvfs.h>
#وگرنهاگر HAVE_SYS_VFS_H
# include <sys/vfs.h>
#وگرنهاگر HAVE_SYS_MOUNT_H && HAVE_SYS_PARAM_H
/* NOTE: freebsd5.0 needs sys/param.h and sys/mount.h for statfs.
   It does have statvfs.h, but shouldn't use it, since it doesn't
   HAVE_STRUCT_STATVFS_F_BASETYPE.  So find a clean way to fix it.  */
/* NetBSD 1.5.2 needs these, for the declaration of struct statfs. */
# include <sys/param.h>
# include <sys/mount.h>
# اگر HAVE_NFS_NFS_CLNT_H && HAVE_NFS_VFS_H
/* Ultrix 4.4 needs these for the declaration of struct statfs.  */
#  include <netinet/in.h>
#  include <nfs/nfs_clnt.h>
#  include <nfs/vfs.h>
# endif
#وگرنهاگر HAVE_OS_H /* BeOS */
# include <fs_info.h>
#endif
#شامل <selinux/selinux.h>
#شامل <getopt.h>

#شامل "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#شامل "system.h"

#شامل "areadlink.h"
#شامل "c-ctype.h"
#شامل "file-type.h"
#شامل "filemode.h"
#شامل "fs.h"
#شامل "mountlist.h"
#شامل "octhexdigits.h"
#شامل "quote.h"
#شامل "stat-size.h"
#شامل "stat-time.h"
#شامل "strftime.h"
#شامل "find-mount-point.h"
#شامل "xvasprintf.h"
#شامل "statx.h"

#اگر HAVE_STATX && defined STATX_INO
# define USE_STATX 1
#وگرنه
# define USE_STATX 0
#endif

#اگر USE_STATVFS
# define STRUCT_STATXFS_F_FSID_IS_INTEGER STRUCT_STATVFS_F_FSID_IS_INTEGER
# define HAVE_STRUCT_STATXFS_F_TYPE HAVE_STRUCT_STATVFS_F_TYPE
# اگر HAVE_STRUCT_STATVFS_F_NAMEMAX
#  define SB_F_NAMEMAX(S) ((S)->f_namemax)
# endif
# اگر ! STAT_STATVFS && STAT_STATVFS64
#  define STRUCT_STATVFS ساختار statvfs64
#  define STATFS statvfs64
# وگرنه
#  define STRUCT_STATVFS ساختار statvfs
#  define STATFS statvfs
# endif
# define STATFS_FRSIZE(S) ((S)->f_frsize)
#وگرنه
# define HAVE_STRUCT_STATXFS_F_TYPE HAVE_STRUCT_STATFS_F_TYPE
# اگر HAVE_STRUCT_STATFS_F_NAMELEN
#  define SB_F_NAMEMAX(S) ((S)->f_namelen)
# elif HAVE_STRUCT_STATFS_F_NAMEMAX
#  define SB_F_NAMEMAX(S) ((S)->f_namemax)
# endif
# define STATFS statfs
# اگر HAVE_OS_H /* BeOS */
/* BeOS has a statvfs function, but it does not return sensible values
   for f_files, f_ffree and f_favail, and lacks f_type, f_basetype and
   f_fstypename.  Use 'struct fs_info' instead.  */
NODISCARD
ایستا صحیح
statfs (نویسه ثابت *filename, ساختار fs_info *buf)
{
  dev_t device = dev_for_path (filename);
  اگر (device < 0)
    {
      errno = (device == B_ENTRY_NOT_FOUND ? ENOENT
               : device == B_BAD_مقدار ? EINVAL
               : device == B_NAME_TOO_LONG ? ENAMETOOLONG
               : device == B_NO_MEMORY ? ENOMEM
               : device == B_FILE_ERROR ? EIO
               : 0);
      بازگشت -1;
    }
  /* If successful, buf->dev will be == device.  */
  بازگشت fs_stat_dev (device, buf);
}
#  define f_fsid dev
#  define f_blocks total_blocks
#  define f_bfree free_blocks
#  define f_bavail free_blocks
#  define f_bsize io_size
#  define f_files total_nodes
#  define f_ffree free_nodes
#  define STRUCT_STATVFS ساختار fs_info
#  define STRUCT_STATXFS_F_FSID_IS_INTEGER true
#  define STATFS_FRSIZE(S) ((S)->block_size)
# وگرنه
#  define STRUCT_STATVFS ساختار statfs
#  define STRUCT_STATXFS_F_FSID_IS_INTEGER STRUCT_STATFS_F_FSID_IS_INTEGER
#  اگر HAVE_STRUCT_STATFS_F_FRSIZE
#   define STATFS_FRSIZE(S) ((S)->f_frsize)
#  وگرنه
#   define STATFS_FRSIZE(S) 0
#  endif
# endif
#endif

#ifdef SB_F_NAMEMAX
# define OUT_NAMEMAX out_uint
#وگرنه
/* Depending on whether statvfs or statfs is used,
   neither f_namemax or f_namelen may be available.  */
# define SB_F_NAMEMAX(S) "?"
# define OUT_NAMEMAX out_string
#endif

#اگر HAVE_STRUCT_STATVFS_F_BASETYPE
# define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME f_basetype
#وگرنه
# اگر HAVE_STRUCT_STATVFS_F_FSTYPENAME || HAVE_STRUCT_STATFS_F_FSTYPENAME
#  define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME f_fstypename
# elif HAVE_OS_H /* BeOS */
#  define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME fsh_name
# endif
#endif

#اگر HAVE_GETATTRAT
# include <attr.h>
# include <sys/nvpair.h>
#endif

ایستا نویسه ثابت digits[] = "0123456789";

/* Flags that are portable for use in printf, for at least one
   conversion specifier; make_format removes non-portable flags as
   needed for particular specifiers.  The glibc 2.2 extension "I" is
   listed here; it is removed by make_format because it has undefined
   behavior elsewhere and because it is incompatible with
   out_epoch_sec.  */
ایستا نویسه ثابت printf_flags[] = "'-+ #0I";

/* Formats for the --terse option.  */
ایستا نویسه ثابت fmt_terse_fs[] = "%Qn %i %l %t %s %S %b %f %a %c %d\n";
ایستا نویسه ثابت fmt_terse_regular[] = "%Qn %s %b %f %u %g %D %i %h %t %T"
                                        " %X %Y %Z %W %o\n";
ایستا نویسه ثابت fmt_terse_selinux[] = "%Qn %s %b %f %u %g %D %i %h %t %T"
                                        " %X %Y %Z %W %o %C\n";

#define PROGRAM_NAME "stat"

#define AUTHORS proper_name ("Michael Meskes")

شمارش
{
  PRINTF_OPTION = CHAR_MAX + 1
};

شمارش cached_mode
{
  cached_default,
  cached_never,
  cached_always
};

ایستا نویسه ثابت *ثابت cached_args[] =
{
  "default", "never", "always", پوچ
};

ایستا شمارش cached_mode ثابت cached_modes[] =
{
  cached_default, cached_never, cached_always
};

ایستا ساختار option ثابت long_options[] =
{
  {"dereference", no_argument, پوچ, 'L'},
  {"file-system", no_argument, پوچ, 'f'},
  {"format", required_argument, پوچ, 'c'},
  {"printf", required_argument, پوچ, PRINTF_OPTION},
  {"terse", no_argument, پوچ, 't'},
  {"cached", required_argument, پوچ, 0},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {پوچ, 0, پوچ, 0}
};

/* Whether to follow symbolic links;  True for --dereference (-L).  */
ایستا bool follow_links;

/* Whether to interpret backslash-escape sequences.
   True for --printf=FMT, not for --format=FMT (-c).  */
ایستا bool interpret_backslash_escapes;

/* The trailing delimiter string:
   "" for --printf=FMT, "\n" for --format=FMT (-c).  */
ایستا نویسه ثابت *trailing_delim = "";

/* The representation of the decimal point in the current locale.  */
ایستا نویسه ثابت *decimal_point;
ایستا size_t decimal_point_len;

ایستا bool
print_stat (نویسه *pformat, size_t prefix_len, نویسه mod, نویسه m,
            صحیح fd, نویسه ثابت *filename, تهی ثابت *data);

/* Return the type of the specified file system.
   Some systems have statfvs.f_basetype[FSTYPSZ] (AIX, HP-UX, and Solaris).
   Others have statvfs.f_fstypename[_VFS_NAMELEN] (NetBSD 3.0).
   Others have statfs.f_fstypename[MFSNAMELEN] (NetBSD 1.5.2).
   Still others have neither and have to get by with f_type (GNU/Linux).
   But f_type may only exist in statfs (Cygwin).  */
NODISCARD
ایستا نویسه ثابت *
human_fstype (STRUCT_STATVFS ثابت *statfsbuf)
{
#ifdef STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME
  بازگشت statfsbuf->STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME;
#وگرنه
  انتخاب (statfsbuf->f_type)
    {
# اگر defined __linux__ || defined __ANDROID__

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

    حالت S_MAGIC_AAFS: /* 0x5A3C69F0 local */
      بازگشت "aafs";
    حالت S_MAGIC_ACFS: /* 0x61636673 remote */
      بازگشت "acfs";
    حالت S_MAGIC_ADFS: /* 0xADF5 local */
      بازگشت "adfs";
    حالت S_MAGIC_AFFS: /* 0xADFF local */
      بازگشت "affs";
    حالت S_MAGIC_AFS: /* 0x5346414F remote */
      بازگشت "afs";
    حالت S_MAGIC_ANON_INODE_FS: /* 0x09041934 local */
      بازگشت "anon-inode FS";
    حالت S_MAGIC_AUFS: /* 0x61756673 remote */
      /* FIXME: change syntax or add an optional attribute like "inotify:no".
         The above is labeled as "remote" so that tail always uses polling,
         but this isn't really a remote file system type.  */
      بازگشت "aufs";
    حالت S_MAGIC_AUTOFS: /* 0x0187 local */
      بازگشت "autofs";
    حالت S_MAGIC_BALLOON_KVM: /* 0x13661366 local */
      بازگشت "balloon-kvm-fs";
    حالت S_MAGIC_BCACHEFS: /* 0xCA451A4E local */
      بازگشت "bcachefs";
    حالت S_MAGIC_BEFS: /* 0x42465331 local */
      بازگشت "befs";
    حالت S_MAGIC_BDEVFS: /* 0x62646576 local */
      بازگشت "bdevfs";
    حالت S_MAGIC_BFS: /* 0x1BADFACE local */
      بازگشت "bfs";
    حالت S_MAGIC_BINDERFS: /* 0x6C6F6F70 local */
      بازگشت "binderfs";
    حالت S_MAGIC_BPF_FS: /* 0xCAFE4A11 local */
      بازگشت "bpf_fs";
    حالت S_MAGIC_BINFMTFS: /* 0x42494E4D local */
      بازگشت "binfmt_misc";
    حالت S_MAGIC_BTRFS: /* 0x9123683E local */
      بازگشت "btrfs";
    حالت S_MAGIC_BTRFS_TEST: /* 0x73727279 local */
      بازگشت "btrfs_test";
    حالت S_MAGIC_CEPH: /* 0x00C36400 remote */
      بازگشت "ceph";
    حالت S_MAGIC_CGROUP: /* 0x0027E0EB local */
      بازگشت "cgroupfs";
    حالت S_MAGIC_CGROUP2: /* 0x63677270 local */
      بازگشت "cgroup2fs";
    حالت S_MAGIC_CIFS: /* 0xFF534D42 remote */
      بازگشت "cifs";
    حالت S_MAGIC_CODA: /* 0x73757245 remote */
      بازگشت "coda";
    حالت S_MAGIC_COH: /* 0x012FF7B7 local */
      بازگشت "coh";
    حالت S_MAGIC_CONFIGFS: /* 0x62656570 local */
      بازگشت "configfs";
    حالت S_MAGIC_CRAMFS: /* 0x28CD3D45 local */
      بازگشت "cramfs";
    حالت S_MAGIC_CRAMFS_WEND: /* 0x453DCD28 local */
      بازگشت "cramfs-wend";
    حالت S_MAGIC_DAXFS: /* 0x64646178 local */
      بازگشت "daxfs";
    حالت S_MAGIC_DEBUGFS: /* 0x64626720 local */
      بازگشت "debugfs";
    حالت S_MAGIC_DEVFS: /* 0x1373 local */
      بازگشت "devfs";
    حالت S_MAGIC_DEVMEM: /* 0x454D444D local */
      بازگشت "devmem";
    حالت S_MAGIC_DEVPTS: /* 0x1CD1 local */
      بازگشت "devpts";
    حالت S_MAGIC_DMA_BUF: /* 0x444D4142 local */
      بازگشت "dma-buf-fs";
    حالت S_MAGIC_ECRYPTFS: /* 0xF15F local */
      بازگشت "ecryptfs";
    حالت S_MAGIC_EFIVARFS: /* 0xDE5E81E4 local */
      بازگشت "efivarfs";
    حالت S_MAGIC_EFS: /* 0x00414A53 local */
      بازگشت "efs";
    حالت S_MAGIC_EROFS_V1: /* 0xE0F5E1E2 local */
      بازگشت "erofs";
    حالت S_MAGIC_EXFAT: /* 0x2011BAB0 local */
      بازگشت "exfat";
    حالت S_MAGIC_EXFS: /* 0x45584653 local */
      بازگشت "exfs";
    حالت S_MAGIC_EXOFS: /* 0x5DF5 local */
      بازگشت "exofs";
    حالت S_MAGIC_EXT: /* 0x137D local */
      بازگشت "ext";
    حالت S_MAGIC_EXT2: /* 0xEF53 local */
      بازگشت "ext2/ext3";
    حالت S_MAGIC_EXT2_OLD: /* 0xEF51 local */
      بازگشت "ext2";
    حالت S_MAGIC_F2FS: /* 0xF2F52010 local */
      بازگشت "f2fs";
    حالت S_MAGIC_FAIL_FS: /* 0x4641494C local */
      بازگشت "failfs";
    حالت S_MAGIC_FAT: /* 0x4006 local */
      بازگشت "fat";
    حالت S_MAGIC_FHGFS: /* 0x19830326 remote */
      بازگشت "fhgfs";
    حالت S_MAGIC_FUSE: /* 0x65735546 remote */
      بازگشت "fuse";
    حالت S_MAGIC_FUSECTL: /* 0x65735543 remote */
      بازگشت "fusectl";
    حالت S_MAGIC_FUTEXFS: /* 0x0BAD1DEA local */
      بازگشت "futexfs";
    حالت S_MAGIC_GFS: /* 0x01161970 remote */
      بازگشت "gfs/gfs2";
    حالت S_MAGIC_GPFS: /* 0x47504653 remote */
      بازگشت "gpfs";
    حالت S_MAGIC_GUEST_MEMFD: /* 0x474D454D remote */
      بازگشت "guest-memfd";
    حالت S_MAGIC_HFS: /* 0x4244 local */
      بازگشت "hfs";
    حالت S_MAGIC_HFS_PLUS: /* 0x482B local */
      بازگشت "hfs+";
    حالت S_MAGIC_HFS_X: /* 0x4858 local */
      بازگشت "hfsx";
    حالت S_MAGIC_HOSTFS: /* 0x00C0FFEE local */
      بازگشت "hostfs";
    حالت S_MAGIC_HPFS: /* 0xF995E849 local */
      بازگشت "hpfs";
    حالت S_MAGIC_HUGETLBFS: /* 0x958458F6 local */
      بازگشت "hugetlbfs";
    حالت S_MAGIC_MTD_INODE_FS: /* 0x11307854 local */
      بازگشت "inodefs";
    حالت S_MAGIC_IBRIX: /* 0x013111A8 remote */
      بازگشت "ibrix";
    حالت S_MAGIC_INOTIFYFS: /* 0x2BAD1DEA local */
      بازگشت "inotifyfs";
    حالت S_MAGIC_ISOFS: /* 0x9660 local */
      بازگشت "isofs";
    حالت S_MAGIC_ISOFS_R_WIN: /* 0x4004 local */
      بازگشت "isofs";
    حالت S_MAGIC_ISOFS_WIN: /* 0x4000 local */
      بازگشت "isofs";
    حالت S_MAGIC_JFFS: /* 0x07C0 local */
      بازگشت "jffs";
    حالت S_MAGIC_JFFS2: /* 0x72B6 local */
      بازگشت "jffs2";
    حالت S_MAGIC_JFS: /* 0x3153464A local */
      بازگشت "jfs";
    حالت S_MAGIC_KAFS: /* 0x6B414653 remote */
      بازگشت "k-afs";
    حالت S_MAGIC_LOGFS: /* 0xC97E8168 local */
      بازگشت "logfs";
    حالت S_MAGIC_LUSTRE: /* 0x0BD00BD0 remote */
      بازگشت "lustre";
    حالت S_MAGIC_M1FS: /* 0x5346314D local */
      بازگشت "m1fs";
    حالت S_MAGIC_MINIX: /* 0x137F local */
      بازگشت "minix";
    حالت S_MAGIC_MINIX_30: /* 0x138F local */
      بازگشت "minix (30 char.)";
    حالت S_MAGIC_MINIX_V2: /* 0x2468 local */
      بازگشت "minix v2";
    حالت S_MAGIC_MINIX_V2_30: /* 0x2478 local */
      بازگشت "minix v2 (30 char.)";
    حالت S_MAGIC_MINIX_V3: /* 0x4D5A local */
      بازگشت "minix3";
    حالت S_MAGIC_MQUEUE: /* 0x19800202 local */
      بازگشت "mqueue";
    حالت S_MAGIC_MSDOS: /* 0x4D44 local */
      بازگشت "msdos";
    حالت S_MAGIC_NCP: /* 0x564C remote */
      بازگشت "novell";
    حالت S_MAGIC_NFS: /* 0x6969 remote */
      بازگشت "nfs";
    حالت S_MAGIC_NFSD: /* 0x6E667364 remote */
      بازگشت "nfsd";
    حالت S_MAGIC_NILFS: /* 0x3434 local */
      بازگشت "nilfs";
    حالت S_MAGIC_NSFS: /* 0x6E736673 local */
      بازگشت "nsfs";
    حالت S_MAGIC_NTFS: /* 0x5346544E local */
      بازگشت "ntfs";
    حالت S_MAGIC_NULL_FS: /* 0x4E554C4C local */
      بازگشت "nullfs";
    حالت S_MAGIC_OPENPROM: /* 0x9FA1 local */
      بازگشت "openprom";
    حالت S_MAGIC_OCFS2: /* 0x7461636F remote */
      بازگشت "ocfs2";
    حالت S_MAGIC_OVERLAYFS: /* 0x794C7630 remote */
      /* This may overlay remote file systems.
         Also there have been issues reported with inotify and overlayfs,
         so mark as "remote" so that polling is used.  */
      بازگشت "overlayfs";
    حالت S_MAGIC_PANFS: /* 0xAAD7AAEA remote */
      بازگشت "panfs";
    حالت S_MAGIC_PID_FS: /* 0x50494446 local */
      بازگشت "pidfs";
    حالت S_MAGIC_PIPEFS: /* 0x50495045 remote */
      /* FIXME: change syntax or add an optional attribute like "inotify:no".
         pipefs is labeled as "remote" so that tail always polls,
         but it isn't really remote file system type.  */
      بازگشت "pipefs";
    حالت S_MAGIC_PPC_CMM: /* 0xC7571590 local */
      بازگشت "ppc-cmm-fs";
    حالت S_MAGIC_PRL_FS: /* 0x7C7C6673 remote */
      بازگشت "prl_fs";
    حالت S_MAGIC_PROC: /* 0x9FA0 local */
      بازگشت "proc";
    حالت S_MAGIC_PSTOREFS: /* 0x6165676C local */
      بازگشت "pstorefs";
    حالت S_MAGIC_QNX4: /* 0x002F local */
      بازگشت "qnx4";
    حالت S_MAGIC_QNX6: /* 0x68191122 local */
      بازگشت "qnx6";
    حالت S_MAGIC_RAMFS: /* 0x858458F6 local */
      بازگشت "ramfs";
    حالت S_MAGIC_RDTGROUP: /* 0x07655821 local */
      بازگشت "rdt";
    حالت S_MAGIC_REISERFS: /* 0x52654973 local */
      بازگشت "reiserfs";
    حالت S_MAGIC_ROMFS: /* 0x7275 local */
      بازگشت "romfs";
    حالت S_MAGIC_RPC_PIPEFS: /* 0x67596969 local */
      بازگشت "rpc_pipefs";
    حالت S_MAGIC_SDCARDFS: /* 0x5DCA2DF5 local */
      بازگشت "sdcardfs";
    حالت S_MAGIC_SECRETMEM: /* 0x5345434D local */
      بازگشت "secretmem";
    حالت S_MAGIC_SECURITYFS: /* 0x73636673 local */
      بازگشت "securityfs";
    حالت S_MAGIC_SELINUX: /* 0xF97CFF8C local */
      بازگشت "selinux";
    حالت S_MAGIC_SMACK: /* 0x43415D53 local */
      بازگشت "smackfs";
    حالت S_MAGIC_SMB: /* 0x517B remote */
      بازگشت "smb";
    حالت S_MAGIC_SMB2: /* 0xFE534D42 remote */
      بازگشت "smb2";
    حالت S_MAGIC_SNFS: /* 0xBEEFDEAD remote */
      بازگشت "snfs";
    حالت S_MAGIC_SOCKFS: /* 0x534F434B local */
      بازگشت "sockfs";
    حالت S_MAGIC_SQUASHFS: /* 0x73717368 local */
      بازگشت "squashfs";
    حالت S_MAGIC_SYSFS: /* 0x62656572 local */
      بازگشت "sysfs";
    حالت S_MAGIC_SYSV2: /* 0x012FF7B6 local */
      بازگشت "sysv2";
    حالت S_MAGIC_SYSV4: /* 0x012FF7B5 local */
      بازگشت "sysv4";
    حالت S_MAGIC_TMPFS: /* 0x01021994 local */
      بازگشت "tmpfs";
    حالت S_MAGIC_TRACEFS: /* 0x74726163 local */
      بازگشت "tracefs";
    حالت S_MAGIC_UBIFS: /* 0x24051905 local */
      بازگشت "ubifs";
    حالت S_MAGIC_UDF: /* 0x15013346 local */
      بازگشت "udf";
    حالت S_MAGIC_UFS: /* 0x00011954 local */
      بازگشت "ufs";
    حالت S_MAGIC_UFS_BYTESWAPPED: /* 0x54190100 local */
      بازگشت "ufs";
    حالت S_MAGIC_USBDEVFS: /* 0x9FA2 local */
      بازگشت "usbdevfs";
    حالت S_MAGIC_V9FS: /* 0x01021997 local */
      بازگشت "v9fs";
    حالت S_MAGIC_VBOXSF: /* 0x786F4256 remote */
      بازگشت "vboxsf";
    حالت S_MAGIC_VMHGFS: /* 0xBACBACBC remote */
      بازگشت "vmhgfs";
    حالت S_MAGIC_VXFS: /* 0xA501FCF5 remote */
      /* Veritas File System can run in single instance or clustered mode,
         so mark as remote to cater for the latter case.  */
      بازگشت "vxfs";
    حالت S_MAGIC_VZFS: /* 0x565A4653 local */
      بازگشت "vzfs";
    حالت S_MAGIC_WSLFS: /* 0x53464846 local */
      بازگشت "wslfs";
    حالت S_MAGIC_XENFS: /* 0xABBA1974 local */
      بازگشت "xenfs";
    حالت S_MAGIC_XENIX: /* 0x012FF7B4 local */
      بازگشت "xenix";
    حالت S_MAGIC_XFS: /* 0x58465342 local */
      بازگشت "xfs";
    حالت S_MAGIC_XIAFS: /* 0x012FD16D local */
      بازگشت "xia";
    حالت S_MAGIC_Z3FOLD: /* 0x0033 local */
      بازگشت "z3fold";
    حالت S_MAGIC_ZFS: /* 0x2FC12FC1 local */
      بازگشت "zfs";
    حالت S_MAGIC_ZONEFS: /* 0x5A4F4653 local */
      بازگشت "zonefs";
    حالت S_MAGIC_ZSMALLOC: /* 0x58295829 local */
      بازگشت "zsmallocfs";


# elif __GNU__
    حالت FSTYPE_UFS:
      بازگشت "ufs";
    حالت FSTYPE_NFS:
      بازگشت "nfs";
    حالت FSTYPE_GFS:
      بازگشت "gfs";
    حالت FSTYPE_LFS:
      بازگشت "lfs";
    حالت FSTYPE_SYSV:
      بازگشت "sysv";
    حالت FSTYPE_FTP:
      بازگشت "ftp";
    حالت FSTYPE_TAR:
      بازگشت "tar";
    حالت FSTYPE_AR:
      بازگشت "ar";
    حالت FSTYPE_CPIO:
      بازگشت "cpio";
    حالت FSTYPE_MSLOSS:
      بازگشت "msloss";
    حالت FSTYPE_CPM:
      بازگشت "cpm";
    حالت FSTYPE_HFS:
      بازگشت "hfs";
    حالت FSTYPE_DTFS:
      بازگشت "dtfs";
    حالت FSTYPE_GRFS:
      بازگشت "grfs";
    حالت FSTYPE_TERM:
      بازگشت "term";
    حالت FSTYPE_DEV:
      بازگشت "dev";
    حالت FSTYPE_PROC:
      بازگشت "proc";
    حالت FSTYPE_IFSOCK:
      بازگشت "ifsock";
    حالت FSTYPE_AFS:
      بازگشت "afs";
    حالت FSTYPE_DFS:
      بازگشت "dfs";
    حالت FSTYPE_PROC9:
      بازگشت "proc9";
    حالت FSTYPE_SOCKET:
      بازگشت "socket";
    حالت FSTYPE_MISC:
      بازگشت "misc";
    حالت FSTYPE_EXT2FS:
      بازگشت "ext2/ext3";
    حالت FSTYPE_HTTP:
      بازگشت "http";
    حالت FSTYPE_MEMFS:
      بازگشت "memfs";
    حالت FSTYPE_ISO9660:
      بازگشت "iso9660";
# endif
    پیشفرض:
      {
        unsigned long صحیح type = statfsbuf->f_type;
        ایستا نویسه buf[اندازه "UNKNOWN (0x%lx)" - 3
                        + (اندازه type * CHAR_BIT + 3) / 4];
        sprintf (buf, "UNKNOWN (0x%lx)", type);
        بازگشت buf;
      }
    }
#endif
}

NODISCARD
ایستا نویسه *
human_access (ساختار stat ثابت *statbuf)
{
  ایستا نویسه modebuf[12];
  filemodestring (statbuf, modebuf);
  modebuf[10] = 0;
  بازگشت modebuf;
}

NODISCARD
ایستا نویسه *
human_time (ساختار timespec t)
{
  /* STR must be at least INT_BUFSIZE_BOUND (intmax_t) big, either
     because localtime_rz fails, or because the time zone is truly
     outlandish so that %z expands to a long string.  */
  ایستا نویسه str[INT_BUFSIZE_BOUND (intmax_t)
                  + INT_STRLEN_BOUND (صحیح) /* YYYY */
                  + 1 /* because YYYY might equal INT_MAX + 1900 */
                  + اندازه "-MM-DD HH:MM:SS.NNNNNNNNN +"];
  ایستا timezone_t tz;
  اگر (!tz)
    tz = tzalloc (getenv ("TZ"));
  ساختار tm tm;
  صحیح ns = t.tv_nsec;
  اگر (localtime_rz (tz, &t.tv_sec, &tm))
    nstrftime (str, اندازه str, "%Y-%m-%d %H:%M:%S.%N %z", &tm, tz, ns);
  وگرنه
    {
      نویسه secbuf[INT_BUFSIZE_BOUND (intmax_t)];
      sprintf (str, "%s.%09d", timetostr (t.tv_sec, secbuf), ns);
    }
  بازگشت str;
}

/* PFORMAT points to a '%' followed by a prefix of a format, all of
   size PREFIX_LEN.  The flags allowed for this format are
   ALLOWED_FLAGS; remove other printf flags from the prefix, then
   append SUFFIX.  */
ایستا تهی
make_format (نویسه *pformat, size_t prefix_len, نویسه ثابت *allowed_flags,
             نویسه ثابت *suffix)
{
  نویسه *dst = pformat + 1;
  نویسه ثابت *src;
  نویسه ثابت *srclim = pformat + prefix_len;
  برای (src = dst; src < srclim && strchr (printf_flags, *src); src++)
    اگر (strchr (allowed_flags, *src))
      *dst++ = *src;
  وقتی (src < srclim)
    *dst++ = *src++;
  strcpy (dst, suffix);
}

ایستا تهی
out_string (نویسه *pformat, size_t prefix_len, نویسه ثابت *arg)
{
  make_format (pformat, prefix_len, "-", "s");
  printf (pformat, arg);
}
ایستا صحیح
out_int (نویسه *pformat, size_t prefix_len, intmax_t arg)
{
  make_format (pformat, prefix_len, "'-+ 0", "jd");
  بازگشت printf (pformat, arg);
}
ایستا صحیح
out_uint (نویسه *pformat, size_t prefix_len, uintmax_t arg)
{
  make_format (pformat, prefix_len, "'-0", "ju");
  بازگشت printf (pformat, arg);
}
ایستا تهی
out_uint_o (نویسه *pformat, size_t prefix_len, uintmax_t arg)
{
  make_format (pformat, prefix_len, "-#0", "jo");
  printf (pformat, arg);
}
ایستا تهی
out_uint_x (نویسه *pformat, size_t prefix_len, uintmax_t arg)
{
  make_format (pformat, prefix_len, "-#0", "jx");
  printf (pformat, arg);
}
ایستا صحیح
out_minus_zero (نویسه *pformat, size_t prefix_len)
{
  make_format (pformat, prefix_len, "'-+ 0", ".0f");
  بازگشت printf (pformat, -0.25);
}

/* Output the number of seconds since the Epoch, using a format that
   acts like printf's %f format.  */
ایستا تهی
out_epoch_sec (نویسه *pformat, size_t prefix_len,
               ساختار timespec arg)
{
  نویسه *dot = memchr (pformat, '.', prefix_len);
  size_t sec_prefix_len = prefix_len;
  صحیح width = 0;
  صحیح precision = 0;
  bool frac_چپ_adjust = false;

  اگر (dot)
    {
      sec_prefix_len = dot - pformat;
      pformat[prefix_len] = '\0';

      اگر (c_isdigit (dot[1]))
        {
          long صحیح lprec = strtol (dot + 1, پوچ, 10);
          precision = (lprec <= INT_MAX ? lprec : INT_MAX);
        }
      وگرنه
        {
          precision = 9;
        }

      اگر (precision && c_isdigit (dot[-1]))
        {
          /* If a nontrivial width is given, subtract the width of the
             decimal point and PRECISION digits that will be output
             later.  */
          نویسه *p = dot;
          *dot = '\0';

          انجامبده
            --p;
          وقتی (c_isdigit (p[-1]));

          long صحیح lwidth = strtol (p, پوچ, 10);
          width = (lwidth <= INT_MAX ? lwidth : INT_MAX);
          اگر (1 < width)
            {
              p += (*p == '0');
              sec_prefix_len = p - pformat;
              صحیح w_d = (decimal_point_len < width
                         ? width - decimal_point_len
                         : 0);
              اگر (1 < w_d)
                {
                  صحیح w = w_d - precision;
                  اگر (1 < w)
                    {
                      نویسه *dst = pformat;
                      برای (نویسه ثابت *src = dst; src < p; src++)
                        {
                          اگر (*src == '-')
                            frac_چپ_adjust = true;
                          وگرنه
                            *dst++ = *src;
                        }
                      sec_prefix_len =
                        (dst - pformat
                         + (frac_چپ_adjust ? 0 : sprintf (dst, "%d", w)));
                    }
                }
            }
        }
    }

  صحیح مقسوم_علیه = 1;
  برای (صحیح i = precision; i < 9; i++)
    مقسوم_علیه *= 10;
  صحیح frac_sec = arg.tv_nsec / مقسوم_علیه;
  صحیح int_len;

  اگر (TYPE_SIGNED (time_t))
    {
      bool minus_zero = false;
      اگر (arg.tv_sec < 0 && arg.tv_nsec != 0)
        {
          صحیح frac_sec_modulus = 1000000000 / مقسوم_علیه;
          frac_sec = (frac_sec_modulus - frac_sec
                      - (arg.tv_nsec % مقسوم_علیه != 0));
          arg.tv_sec += (frac_sec != 0);
          minus_zero = (arg.tv_sec == 0);
        }
      int_len = (minus_zero
                 ? out_minus_zero (pformat, sec_prefix_len)
                 : out_int (pformat, sec_prefix_len, arg.tv_sec));
    }
  وگرنه
    int_len = out_uint (pformat, sec_prefix_len, arg.tv_sec);

  اگر (precision)
    {
      صحیح prec = (precision < 9 ? precision : 9);
      صحیح trailing_prec = precision - prec;
      صحیح ilen = (int_len < 0 ? 0 : int_len);
      صحیح trailing_width = (ilen < width && decimal_point_len < width - ilen
                            ? width - ilen - decimal_point_len - prec
                            : 0);
      printf ("%s%.*d%-*.*d", decimal_point, prec, frac_sec,
              trailing_width, trailing_prec, 0);
    }
}

/* Print the context information of FILENAME, and return true iff the
   context could not be obtained.  */
NODISCARD
ایستا bool
out_file_context (نویسه *pformat, size_t prefix_len, نویسه ثابت *filename)
{
  نویسه *scontext;
  bool fail = false;

  اگر ((follow_links
       ? getfilecon (filename, &scontext)
       : lgetfilecon (filename, &scontext)) < 0)
    {
      error (0, errno, _("failed to get security context of %s"),
             quoteaf (filename));
      scontext = پوچ;
      fail = true;
    }
  strcpy (pformat + prefix_len, "s");
  printf (pformat, (scontext ? scontext : "?"));
  اگر (scontext)
    freecon (scontext);
  بازگشت fail;
}

/* Set the quoting style once it is needed.  */
ایستا تهی
initialize_quoting_style (تهی)
{
  ایستا bool initialized;
  اگر (!initialized)
    {
      initialized = true;
      صحیح qs = getenv_quoting_style ();
      set_quoting_style (پوچ, 0 <= qs ? qs : shell_escape_quoting_style);
    }
}

/* Print statfs info.  Return zero upon success, nonzero upon failure.  */
NODISCARD
ایستا bool
print_statfs (نویسه *pformat, size_t prefix_len, MAYBE_UNUSED نویسه mod, نویسه m,
              MAYBE_UNUSED صحیح fd, نویسه ثابت *filename,
              تهی ثابت *data)
{
  STRUCT_STATVFS ثابت *statfsbuf = data;
  bool fail = false;

  انتخاب (m)
    {
    حالت 'n':
      اگر (mod == 'Q')
        {
          initialize_quoting_style ();
          filename = quoteN (filename);
        }
      out_string (pformat, prefix_len, filename);
      بشکن;

    حالت 'i':
      {
#اگر STRUCT_STATXFS_F_FSID_IS_INTEGER
        uintmax_t fsid = statfsbuf->f_fsid;
#وگرنه
        تعریفنوع unsigned صحیح fsid_word;
        static_assert (alignof (STRUCT_STATVFS) % alignof (fsid_word) == 0);
        static_assert (offsetof (STRUCT_STATVFS, f_fsid) % alignof (fsid_word)
                       == 0);
        static_assert (اندازه statfsbuf->f_fsid % alignof (fsid_word) == 0);
        fsid_word ثابت *p = (fsid_word *) &statfsbuf->f_fsid;

        /* Assume a little-endian word order, as that is compatible
           with glibc's statvfs implementation.  */
        uintmax_t fsid = 0;
        صحیح واژگان = اندازه statfsbuf->f_fsid / اندازه *p;
        برای (صحیح i = 0; i < واژگان && i * اندازه *p < اندازه fsid; i++)
          {
            uintmax_t u = p[واژگان - 1 - i];
            fsid |= u << (i * CHAR_BIT * اندازه *p);
          }
#endif
        out_uint_x (pformat, prefix_len, fsid);
      }
      بشکن;

    حالت 'l':
      OUT_NAMEMAX (pformat, prefix_len, SB_F_NAMEMAX (statfsbuf));
      بشکن;
    حالت 't':
#اگر HAVE_STRUCT_STATXFS_F_TYPE
      out_uint_x (pformat, prefix_len, statfsbuf->f_type);
#وگرنه
      fputc ('?', stdout);
#endif
      بشکن;
    حالت 'T':
      out_string (pformat, prefix_len, human_fstype (statfsbuf));
      بشکن;
    حالت 'b':
      out_int (pformat, prefix_len, statfsbuf->f_blocks);
      بشکن;
    حالت 'f':
      out_int (pformat, prefix_len, statfsbuf->f_bfree);
      بشکن;
    حالت 'a':
      out_int (pformat, prefix_len, statfsbuf->f_bavail);
      بشکن;
    حالت 's':
      out_uint (pformat, prefix_len, statfsbuf->f_bsize);
      بشکن;
    حالت 'S':
      {
        uintmax_t frsize = STATFS_FRSIZE (statfsbuf);
        اگر (! frsize)
          frsize = statfsbuf->f_bsize;
        out_uint (pformat, prefix_len, frsize);
      }
      بشکن;
    حالت 'c':
      out_uint (pformat, prefix_len, statfsbuf->f_files);
      بشکن;
    حالت 'd':
      out_int (pformat, prefix_len, statfsbuf->f_ffree);
      بشکن;
    پیشفرض:
      fputc ('?', stdout);
      بشکن;
    }
  بازگشت fail;
}

/* Return any bind mounted source for a path.
   The caller should not free the returned buffer.
   Return NULL if no bind mount found.  */
NODISCARD
ایستا نویسه ثابت *
find_bind_mount (نویسه ثابت * name)
{
  نویسه ثابت * bind_mount = پوچ;

  ایستا ساختار mount_entry *mount_list;
  ایستا bool tried_mount_list = false;
  اگر (!tried_mount_list) /* attempt/warn once per process.  */
    {
      اگر (!(mount_list = read_file_system_list (false)))
        error (0, errno, "%s", _("cannot read table of mounted file systems"));
      tried_mount_list = true;
    }

  ساختار stat name_stats;
  اگر (stat (name, &name_stats) != 0)
    بازگشت پوچ;

  برای (ساختار mount_entry *me = mount_list; me; me = me->me_next)
    {
      اگر (me->me_dummy && me->me_devname[0] == '/'
          && streq (me->me_mountdir, name))
        {
          ساختار stat dev_stats;

          اگر (stat (me->me_devname, &dev_stats) == 0
              && psame_inode (&name_stats, &dev_stats))
            {
              bind_mount = me->me_devname;
              بشکن;
            }
        }
    }

  بازگشت bind_mount;
}

/* Print mount point.  Return zero upon success, nonzero upon failure.  */
NODISCARD
ایستا bool
out_mount_point (نویسه ثابت *filename, نویسه *pformat, size_t prefix_len,
                 ثابت ساختار stat *statp)
{

  نویسه ثابت *np = "?", *bp = پوچ;
  نویسه *mp = پوچ;
  bool fail = true;

  /* Look for bind mounts first.  Note we output the immediate alias,
     rather than further resolving to a base device mount point.  */
  اگر (follow_links || !S_ISLNK (statp->st_mode))
    {
      نویسه *resolved = canonicalize_file_name (filename);
      اگر (!resolved)
        {
          error (0, errno, _("failed to canonicalize %s"), quoteaf (filename));
          برو print_mount_point;
        }
      bp = find_bind_mount (resolved);
      آزاد (resolved);
      اگر (bp)
        {
          fail = false;
          برو print_mount_point;
        }
    }

  /* If there is no direct bind mount, then navigate
     back up the tree looking for a device change.
     Note we don't detect if any of the directory components
     are bind mounted to the same device, but that's OK
     since we've not directly queried them.  */
  اگر ((mp = find_mount_point (filename, statp)))
    {
      /* This dir might be bind mounted to another device,
         so we resolve the bound source in that case also.  */
      bp = find_bind_mount (mp);
      fail = false;
    }

print_mount_point:

  out_string (pformat, prefix_len, bp ? bp : mp ? mp : np);
  آزاد (mp);
  بازگشت fail;
}

/* Map a TS with negative TS.tv_nsec to {0,0}.  */
ایستا درونخطی ساختار timespec
neg_to_zero (ساختار timespec ts)
{
  اگر (0 <= ts.tv_nsec)
    بازگشت ts;
  ساختار timespec z = {0};
  بازگشت z;
}

/* Output a single-character \ escape.  */

ایستا تهی
print_esc_char (نویسه c)
{
  انتخاب (c)
    {
    حالت 'a':			/* Alert. */
      c ='\a';
      بشکن;
    حالت 'b':			/* Backspace. */
      c ='\b';
      بشکن;
    حالت 'e':			/* Escape. */
      c ='\x1B';
      بشکن;
    حالت 'f':			/* Form feed. */
      c ='\f';
      بشکن;
    حالت 'n':			/* New line. */
      c ='\n';
      بشکن;
    حالت 'r':			/* Carriage return. */
      c ='\r';
      بشکن;
    حالت 't':			/* Horizontal tab. */
      c ='\t';
      بشکن;
    حالت 'v':			/* Vertical tab. */
      c ='\v';
      بشکن;
    حالت '"':
    حالت '\\':
      بشکن;
    پیشفرض:
      error (0, 0, _("warning: unrecognized escape '\\%c'"), c);
      بشکن;
    }
  putchar (c);
}

ATTRIBUTE_PURE
ایستا size_t
format_code_offset (نویسه ثابت *directive)
{
  size_t len = strspn (directive + 1, printf_flags);
  نویسه ثابت *fmt_char = directive + len + 1;
  fmt_char += strspn (fmt_char, digits);
  اگر (*fmt_char == '.')
    fmt_char += 1 + strspn (fmt_char + 1, digits);
  بازگشت fmt_char - directive;
}

/* Print the information specified by the format string, FORMAT,
   calling PRINT_FUNC for each %-directive encountered.
   Return zero upon success, nonzero upon failure.  */
NODISCARD
ایستا bool
print_it (نویسه ثابت *format, صحیح fd, نویسه ثابت *filename,
          bool (*print_func) (نویسه *, size_t, نویسه, نویسه,
                              صحیح, نویسه ثابت *, تهی ثابت *),
          تهی ثابت *data)
{
  bool fail = false;

  /* Add 2 to accommodate our conversion of the stat '%s' format string
     to the longer printf '%llu' one.  */
  شمارش
    {
      MAX_ADDITIONAL_BYTES =
        (MAX (اندازه "jd",
              MAX (اندازه "jo", MAX (اندازه "ju", اندازه "jx")))
         - 1)
    };
  size_t n_alloc = strlen (format) + MAX_ADDITIONAL_BYTES + 1;
  نویسه *dest = xmalloc (n_alloc);
  برای (نویسه ثابت *b = format; *b; b++)
    {
      انتخاب (*b)
        {
        حالت '%':
          {
            size_t len = format_code_offset (b);
            نویسه fmt_char = *(b + len);
            نویسه mod_char = 0;
            memcpy (dest, b, len);
            b += len;

            انتخاب (fmt_char)
              {
              حالت '\0':
                --b;
                FALLTHROUGH;
              حالت '%':
                اگر (1 < len)
                  {
                    dest[len] = fmt_char;
                    dest[len + 1] = '\0';
                    error (EXIT_FAILURE, 0, _("%s: invalid directive"),
                           quote (dest));
                  }
                putchar ('%');
                بشکن;
              حالت 'H':
              حالت 'L':
              حالت 'Q':
                mod_char = fmt_char;
                fmt_char = *(b + 1);
                اگر ((mod_char == 'Q' && fmt_char == 'n')
                    || (print_func == print_stat
                        && (mod_char == 'H' || mod_char == 'L')
                        && (fmt_char == 'd' || fmt_char == 'r')))
                  {
                    b++;
                  }
                وگرنه
                  {
                    fmt_char = mod_char;
                    mod_char = 0;
                  }
                FALLTHROUGH;
              پیشفرض:
                fail |= print_func (dest, len, mod_char, fmt_char,
                                    fd, filename, data);
                بشکن;
              }
            بشکن;
          }

        حالت '\\':
          اگر ( ! interpret_backslash_escapes)
            {
              putchar ('\\');
              بشکن;
            }
          ++b;
          اگر (isoct (*b))
            {
              صحیح esc_مقدار = fromoct (*b);
              صحیح esc_length = 1;	/* number of octal digits */
              برای (++b; esc_length < 3 && isoct (*b);
                   ++esc_length, ++b)
                {
                  esc_مقدار = esc_مقدار * 8 + fromoct (*b);
                }
              putchar (esc_مقدار);
              --b;
            }
          وگرنه اگر (*b == 'x' && c_isxdigit (b[1]))
            {
              صحیح esc_مقدار = fromhex (b[1]);	/* Value of \xhh escape. */
              /* A hexadecimal \xhh escape sequence must have
                 1 or 2 hex. digits.  */
              ++b;
              اگر (c_isxdigit (b[1]))
                {
                  ++b;
                  esc_مقدار = esc_مقدار * 16 + fromhex (*b);
                }
              putchar (esc_مقدار);
            }
          وگرنه اگر (*b == '\0')
            {
              error (0, 0, _("warning: backslash at end of format"));
              putchar ('\\');
              /* Arrange to exit the loop.  */
              --b;
            }
          وگرنه
            {
              print_esc_char (*b);
            }
          بشکن;

        پیشفرض:
          putchar (*b);
          بشکن;
        }
    }
  آزاد (dest);

  fputs (trailing_delim, stdout);

  بازگشت fail;
}

/* Stat the file system and print what we find.  */
NODISCARD
ایستا bool
do_statfs (نویسه ثابت *filename, نویسه ثابت *format)
{
  STRUCT_STATVFS statfsbuf;

  اگر (streq (filename, "-"))
    {
      error (0, 0, _("using %s to denote standard input does not work"
                     " in file system mode"), quoteaf (filename));
      بازگشت false;
    }

  اگر (STATFS (filename, &statfsbuf) != 0)
    {
      error (0, errno, _("cannot read file system information for %s"),
             quoteaf (filename));
      بازگشت false;
    }

  bool fail = print_it (format, -1, filename, print_statfs, &statfsbuf);
  بازگشت ! fail;
}

ساختار print_args {
  ساختار stat *st;
  ساختار timespec btime;
};

/* Ask statx to avoid syncing? */
ایستا bool dont_sync;

/* Ask statx to force sync? */
ایستا bool force_sync;

#اگر USE_STATX
ایستا unsigned صحیح
fmt_to_mask (نویسه fmt)
{
  انتخاب (fmt)
    {
    حالت 'N':
      بازگشت STATX_MODE;
    حالت 'd':
    حالت 'D':
      بازگشت STATX_MODE;
    حالت 'i':
      بازگشت STATX_INO;
    حالت 'a':
    حالت 'A':
      بازگشت STATX_MODE;
    حالت 'f':
      بازگشت STATX_MODE|STATX_TYPE;
    حالت 'F':
      بازگشت STATX_TYPE;
    حالت 'h':
      بازگشت STATX_NLINK;
    حالت 'u':
    حالت 'U':
      بازگشت STATX_UID;
    حالت 'g':
    حالت 'G':
      بازگشت STATX_GID;
    حالت 'm':
      بازگشت STATX_MODE|STATX_INO;
    حالت 's':
      بازگشت STATX_SIZE;
    حالت 't':
    حالت 'T':
      بازگشت STATX_MODE;
    حالت 'b':
      بازگشت STATX_BLOCKS;
    حالت 'w':
    حالت 'W':
      بازگشت STATX_BTIME;
    حالت 'x':
    حالت 'X':
      بازگشت STATX_ATIME;
    حالت 'y':
    حالت 'Y':
      بازگشت STATX_MTIME;
    حالت 'z':
    حالت 'Z':
      بازگشت STATX_CTIME;
    }
  بازگشت 0;
}

ATTRIBUTE_PURE
ایستا unsigned صحیح
format_to_mask (نویسه ثابت *format)
{
  unsigned صحیح mask = 0;

  برای (نویسه ثابت *b = format; *b; b++)
    {
      اگر (*b != '%')
        ادامه;

      b += format_code_offset (b);
      اگر (*b == '\0')
        بشکن;
      mask |= fmt_to_mask (*b);
    }
  بازگشت mask;
}

/* statx the file and print what we find */
NODISCARD
ایستا bool
do_stat (نویسه ثابت *filename, نویسه ثابت *format, نویسه ثابت *format2)
{
  صحیح fd = streq (filename, "-") ? 0 : AT_FDCWD;
  صحیح flags = 0;
  ساختار stat st;
  ساختار statx stx = {0};
  نویسه ثابت *pathname = filename;
  ساختار print_args pa;
  pa.st = &st;
  pa.btime = (ساختار timespec) {.tv_sec = -1, .tv_nsec = -1};

  اگر (AT_FDCWD != fd)
    {
      pathname = "";
      flags = AT_EMPTY_PATH;
    }
  وگرنه اگر (!follow_links)
    {
      flags = AT_SYMLINK_NOFOLLOW;
    }

  اگر (dont_sync)
    flags |= AT_STATX_DONT_SYNC;
  وگرنه اگر (force_sync)
    flags |= AT_STATX_FORCE_SYNC;

  اگر (! force_sync)
    flags |= AT_NO_AUTOMOUNT;

  fd = statx (fd, pathname, flags, format_to_mask (format), &stx);
  اگر (fd < 0)
    {
      اگر (flags & AT_EMPTY_PATH)
        error (0, errno, _("cannot stat standard input"));
      وگرنه
        error (0, errno, _("cannot statx %s"), quoteaf (filename));
      بازگشت false;
    }

  اگر (S_ISBLK (stx.stx_mode) || S_ISCHR (stx.stx_mode))
    format = format2;

  statx_to_stat (&stx, &st);
  اگر (stx.stx_mask & STATX_BTIME)
    pa.btime = statx_timestamp_to_timespec (stx.stx_btime);

  bool fail = print_it (format, fd, filename, print_stat, &pa);
  بازگشت ! fail;
}

#وگرنه /* USE_STATX */

ایستا ساختار timespec
get_birthtime (صحیح fd, نویسه ثابت *filename, ساختار stat ثابت *st)
{
  ساختار timespec ts = get_stat_birthtime (st);

# اگر HAVE_GETATTRAT
  اگر (ts.tv_nsec < 0)
    {
      nvlist_t *response;
      اگر ((fd < 0
           ? getattrat (AT_FDCWD, XATTR_VIEW_READWRITE, filename, &response)
           : fgetattr (fd, XATTR_VIEW_READWRITE, &response))
          == 0)
        {
          uint64_t *val;
          uint_t n;
          اگر (nvlist_lookup_uint64_array (response, A_CRTIME, &val, &n) == 0
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

  بازگشت ts;
}


/* stat the file and print what we find */
NODISCARD
ایستا bool
do_stat (نویسه ثابت *filename, نویسه ثابت *format,
         نویسه ثابت *format2)
{
  صحیح fd = streq (filename, "-") ? 0 : -1;
  ساختار stat statbuf;
  ساختار print_args pa;
  pa.st = &statbuf;
  pa.btime = (ساختار timespec) {.tv_sec = -1, .tv_nsec = -1};

  اگر (0 <= fd)
    {
      اگر (fstat (fd, &statbuf) != 0)
        {
          error (0, errno, _("cannot stat standard input"));
          بازگشت false;
        }
    }
  /* We can't use the shorter
     (follow_links?stat:lstat) (filename, &statbug)
     since stat might be a function-like macro.  */
  وگرنه اگر ((follow_links
            ? stat (filename, &statbuf)
            : lstat (filename, &statbuf)) != 0)
    {
      error (0, errno, _("cannot stat %s"), quoteaf (filename));
      بازگشت false;
    }

  اگر (S_ISBLK (statbuf.st_mode) || S_ISCHR (statbuf.st_mode))
    format = format2;

  bool fail = print_it (format, fd, filename, print_stat, &pa);
  بازگشت ! fail;
}
#endif /* USE_STATX */

/* POSIX requires 'ls' to print file sizes without a sign, even
   when negative.  Be consistent with that.  */

ایستا uintmax_t
unsigned_file_size (off_t size)
{
  بازگشت size + (size < 0) * ((uintmax_t) OFF_T_MAX - OFF_T_MIN + 1);
}

/* Print stat info.  Return zero upon success, nonzero upon failure.  */
ایستا bool
print_stat (نویسه *pformat, size_t prefix_len, نویسه mod, نویسه m,
            MAYBE_UNUSED صحیح fd, نویسه ثابت *filename, تهی ثابت *data)
{
  ساختار print_args *parg = (ساختار print_args *) data;
  ساختار stat *statbuf = parg->st;
  ساختار timespec btime = parg->btime;
  ساختار passwd *pw_ent;
  ساختار group *gw_ent;
  bool fail = false;

  انتخاب (m)
    {
    حالت 'n':
      اگر (mod == 'Q')
        {
          initialize_quoting_style ();
          filename = quoteN (filename);
        }
      out_string (pformat, prefix_len, filename);
      بشکن;
    حالت 'N':
      initialize_quoting_style ();
      out_string (pformat, prefix_len, quoteN (filename));
      اگر (S_ISLNK (statbuf->st_mode))
        {
          نویسه *linkname = areadlink_with_size (filename, statbuf->st_size);
          اگر (linkname == پوچ)
            {
              error (0, errno, _("cannot read symbolic link %s"),
                     quoteaf (filename));
              بازگشت true;
            }
          printf (" -> ");
          out_string (pformat, prefix_len, quoteN (linkname));
          آزاد (linkname);
        }
      بشکن;
    حالت 'd':
      اگر (mod == 'H')
        out_uint (pformat, prefix_len, major (statbuf->st_dev));
      وگرنه اگر (mod == 'L')
        out_uint (pformat, prefix_len, minor (statbuf->st_dev));
      وگرنه
        out_uint (pformat, prefix_len, statbuf->st_dev);
      بشکن;
    حالت 'D':
      out_uint_x (pformat, prefix_len, statbuf->st_dev);
      بشکن;
    حالت 'i':
      out_uint (pformat, prefix_len, statbuf->st_ino);
      بشکن;
    حالت 'a':
      out_uint_o (pformat, prefix_len, statbuf->st_mode & CHMOD_MODE_BITS);
      بشکن;
    حالت 'A':
      out_string (pformat, prefix_len, human_access (statbuf));
      بشکن;
    حالت 'f':
      out_uint_x (pformat, prefix_len, statbuf->st_mode);
      بشکن;
    حالت 'F':
      out_string (pformat, prefix_len, file_type (statbuf));
      بشکن;
    حالت 'h':
      out_uint (pformat, prefix_len, statbuf->st_nlink);
      بشکن;
    حالت 'u':
      out_uint (pformat, prefix_len, statbuf->st_uid);
      بشکن;
    حالت 'U':
      pw_ent = getpwuid (statbuf->st_uid);
      out_string (pformat, prefix_len,
                  pw_ent ? pw_ent->pw_name : "UNKNOWN");
      بشکن;
    حالت 'g':
      out_uint (pformat, prefix_len, statbuf->st_gid);
      بشکن;
    حالت 'G':
      gw_ent = getgrgid (statbuf->st_gid);
      out_string (pformat, prefix_len,
                  gw_ent ? gw_ent->gr_name : "UNKNOWN");
      بشکن;
    حالت 'm':
      fail |= out_mount_point (filename, pformat, prefix_len, statbuf);
      بشکن;
    حالت 's':
      out_uint (pformat, prefix_len, unsigned_file_size (statbuf->st_size));
      بشکن;
    حالت 'r':
      اگر (mod == 'H')
        out_uint (pformat, prefix_len, major (statbuf->st_rdev));
      وگرنه اگر (mod == 'L')
        out_uint (pformat, prefix_len, minor (statbuf->st_rdev));
      وگرنه
        out_uint (pformat, prefix_len, statbuf->st_rdev);
      بشکن;
    حالت 'R':
      out_uint_x (pformat, prefix_len, statbuf->st_rdev);
      بشکن;
    حالت 't':
      out_uint_x (pformat, prefix_len, major (statbuf->st_rdev));
      بشکن;
    حالت 'T':
      out_uint_x (pformat, prefix_len, minor (statbuf->st_rdev));
      بشکن;
    حالت 'B':
      out_uint (pformat, prefix_len, ST_NBLOCKSIZE);
      بشکن;
    حالت 'b':
      out_uint (pformat, prefix_len, STP_NBLOCKS (statbuf));
      بشکن;
    حالت 'o':
      out_uint (pformat, prefix_len, STP_BLKSIZE (statbuf));
      بشکن;
    حالت 'w':
      {
#اگر ! USE_STATX
        btime = get_birthtime (fd, filename, statbuf);
#endif
        اگر (btime.tv_nsec < 0)
          out_string (pformat, prefix_len, "-");
        وگرنه
          out_string (pformat, prefix_len, human_time (btime));
      }
      بشکن;
    حالت 'W':
      {
#اگر ! USE_STATX
        btime = get_birthtime (fd, filename, statbuf);
#endif
        out_epoch_sec (pformat, prefix_len, neg_to_zero (btime));
      }
      بشکن;
    حالت 'x':
      out_string (pformat, prefix_len, human_time (get_stat_atime (statbuf)));
      بشکن;
    حالت 'X':
      out_epoch_sec (pformat, prefix_len, get_stat_atime (statbuf));
      بشکن;
    حالت 'y':
      out_string (pformat, prefix_len, human_time (get_stat_mtime (statbuf)));
      بشکن;
    حالت 'Y':
      out_epoch_sec (pformat, prefix_len, get_stat_mtime (statbuf));
      بشکن;
    حالت 'z':
      out_string (pformat, prefix_len, human_time (get_stat_ctime (statbuf)));
      بشکن;
    حالت 'Z':
      out_epoch_sec (pformat, prefix_len, get_stat_ctime (statbuf));
      بشکن;
    حالت 'C':
      fail |= out_file_context (pformat, prefix_len, filename);
      بشکن;
    پیشفرض:
      fputc ('?', stdout);
      بشکن;
    }
  بازگشت fail;
}

/* Return an allocated format string in static storage that
   corresponds to whether FS and TERSE options were declared.  */
ایستا نویسه *
default_format (bool fs, bool terse, bool device)
{
  نویسه *format;
  اگر (fs)
    {
      اگر (terse)
        format = xstrdup (fmt_terse_fs);
      وگرنه
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
  وگرنه /* ! fs */
    {
      اگر (terse)
        {
          اگر (0 < is_selinux_enabled ())
            format = xstrdup (fmt_terse_selinux);
          وگرنه
            format = xstrdup (fmt_terse_regular);
        }
      وگرنه
        {
          نویسه *temp;
          /* TRANSLATORS: This string uses format specifiers from
             'stat --help' without --file-system, and NOT from printf.  */
          format = xstrdup (_("\
  File: %N\n\
  Size: %-10s\tBlocks: %-10b IO Block: %-6o %F\n\
"));

          temp = format;
          اگر (device)
            {
              /* TRANSLATORS: This string uses format specifiers from
                 'stat --help' without --file-system, and NOT from printf.  */
              format = xasprintf ("%s%s", format, _("\
" "Device: %Hd,%Ld\tInode: %-10i  Links: %-5h Device type: %Hr,%Lr\n\
"));
            }
          وگرنه
            {
              /* TRANSLATORS: This string uses format specifiers from
                 'stat --help' without --file-system, and NOT from printf.  */
              format = xasprintf ("%s%s", format, _("\
" "Device: %Hd,%Ld\tInode: %-10i  Links: %h\n\
"));
            }
          آزاد (temp);

          temp = format;
          /* TRANSLATORS: This string uses format specifiers from
             'stat --help' without --file-system, and NOT from printf.  */
          format = xasprintf ("%s%s", format, _("\
" "Access: (%04a/%10.10A)  Uid: (%5u/%8U)   Gid: (%5g/%8G)\n\
"));
          آزاد (temp);

          اگر (0 < is_selinux_enabled ())
            {
              temp = format;
              /* TRANSLATORS: This string uses format specifiers from
                 'stat --help' without --file-system, and NOT from printf.  */
              format = xasprintf ("%s%s", format, _("Context: %C\n"));
              آزاد (temp);
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
          آزاد (temp);
        }
    }
  بازگشت format;
}

تهی
usage (صحیح status)
{
  اگر (status != EXIT_SUCCESS)
    emit_try_help ();
  وگرنه
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
         use the specified FORMAT instead of the پیشفرض;\n\
         output a newline after each use of FORMAT\n\
"));
      oputs (_("\
      --printf=FORMAT\n\
         like --format, but interpret backslash escapes,\n\
         and انجامبده not output a mandatory trailing newline;\n\
         اگر you want a newline, include \\n in FORMAT\n\
"));
      oputs (_("\
  -t, --terse\n\
         print the information in terse form\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);

      fputs (_("\n\
The MODE argument of --cached can be: always, never, or پیشفرض.\n\
'always' will use cached attributes اگر available, وقتی\n\
'never' will try to synchronize with the latest attributes, and\n\
'default' will leave it up to the underlying file system.\n\
"), stdout);

      fputs (_("\n\
The valid format sequences برای files (without --file-system):\n\
\n\
  %a   permission bits in octal (see '#' and '0' printf flags)\n\
  %A   permission bits and file type in human readable form\n\
  %b   عدد of blocks allocated (see %B)\n\
  %B   the size in bytes of each block reported by %b\n\
  %C   SELinux security context string\n\
"), stdout);
      fputs (_("\
  %d   device عدد in decimal (st_dev)\n\
  %D   device عدد in hex (st_dev)\n\
  %Hd  major device عدد in decimal\n\
  %Ld  minor device عدد in decimal\n\
  %f   raw mode in hex\n\
  %F   file type\n\
  %g   group ID of owner\n\
  %G   group name of owner\n\
"), stdout);
      fputs (_("\
  %h   عدد of hard links\n\
  %i   inode عدد\n\
  %m   mount point\n\
  %n   file name\n\
  %Qn  quoted file name\n\
  %N   quoted file name with dereference اگر symbolic link\n\
  %o   optimal I/O transfer size hint\n\
  %s   total size, in bytes\n\
  %r   device type in decimal (st_rdev)\n\
  %R   device type in hex (st_rdev)\n\
  %Hr  major device type in decimal, برای character/block device special files\n\
  %Lr  minor device type in decimal, برای character/block device special files\n\
  %t   major device type in hex, برای character/block device special files\n\
  %T   minor device type in hex, برای character/block device special files\n\
"), stdout);
      fputs (_("\
  %u   user ID of owner\n\
  %U   user name of owner\n\
  %w   time of file birth, human-readable; - اگر unknown\n\
  %W   time of file birth, seconds since Epoch; 0 اگر unknown\n\
  %x   time of last access, human-readable\n\
  %X   time of last access, seconds since Epoch\n\
  %y   time of last data modification, human-readable\n\
  %Y   time of last data modification, seconds since Epoch\n\
  %z   time of last status change, human-readable\n\
  %Z   time of last status change, seconds since Epoch\n\
\n\
"), stdout);

      fputs (_("\
Valid format sequences برای file systems:\n\
\n\
  %a   آزاد blocks available to non-superuser\n\
  %b   total data blocks in file system\n\
  %c   total file nodes in file system\n\
  %d   آزاد file nodes in file system\n\
  %f   آزاد blocks in file system\n\
"), stdout);
      fputs (_("\
  %i   file system ID in hex\n\
  %l   maximum length of filenames\n\
  %n   file name\n\
  %Qn  quoted file name\n\
  %s   block size (برای faster transfers)\n\
  %S   fundamental block size (برای block counts)\n\
  %t   file system type in hex\n\
  %T   file system type in human readable form\n\
"), stdout);

      printf (_("\n\
--terse is equivalent to the following FORMAT:\n\
    %s\
"),
#اگر HAVE_SELINUX_SELINUX_H
              fmt_terse_selinux
#وگرنه
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

صحیح
اصلی (صحیح argc, نویسه *argv[])
{
  صحیح c;
  bool fs = false;
  bool terse = false;
  نویسه *format = پوچ;
  نویسه *format2;
  bool ok = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  ساختار lconv ثابت *locale = localeconv ();
  decimal_point = (locale->decimal_point[0] ? locale->decimal_point : ".");
  decimal_point_len = strlen (decimal_point);

  atexit (close_stdout);

  وقتی ((c = getopt_long (argc, argv, "c:fLt", long_options, پوچ)) != -1)
    {
      انتخاب (c)
        {
        حالت PRINTF_OPTION:
          format = optarg;
          interpret_backslash_escapes = true;
          trailing_delim = "";
          بشکن;

        حالت 'c':
          format = optarg;
          interpret_backslash_escapes = false;
          trailing_delim = "\n";
          بشکن;

        حالت 'L':
          follow_links = true;
          بشکن;

        حالت 'f':
          fs = true;
          بشکن;

        حالت 't':
          terse = true;
          بشکن;

        حالت 0:
          انتخاب (XARGMATCH ("--cached", optarg, cached_args, cached_modes))
            {
              حالت cached_never:
                force_sync = true;
                dont_sync = false;
                بشکن;
              حالت cached_always:
                force_sync = false;
                dont_sync = true;
                بشکن;
              حالت cached_default:
                force_sync = false;
                dont_sync = false;
            }
          بشکن;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        پیشفرض:
          usage (EXIT_FAILURE);
        }
    }

  اگر (argc == optind)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  اگر (format)
    {
      format2 = format;
    }
  وگرنه
    {
      format = default_format (fs, terse, /* device= */ false);
      format2 = default_format (fs, terse, /* device= */ true);
    }

  برای (صحیح i = optind; i < argc; i++)
    ok &= (fs
           ? do_statfs (argv[i], format)
           : do_stat (argv[i], format, format2));

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
