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

#ضمن <config.h>

/* Keep this conditional in sync with the similar conditional in
   ../m4/stat-prog.m4.  */
#אם ((STAT_STATVFS || STAT_STATVFS64)                                       \
     && (HAVE_STRUCT_STATVFS_F_BASETYPE || HAVE_STRUCT_STATVFS_F_FSTYPENAME \
         || (! HAVE_STRUCT_STATFS_F_FSTYPENAME && HAVE_STRUCT_STATVFS_F_TYPE)))
# define USE_STATVFS 1
#אחרת
# define USE_STATVFS 0
#endif

#ضمن <stdio.h>
#ضمن <sys/types.h>
#ضمن <pwd.h>
#ضمن <grp.h>
#אם USE_STATVFS
# include <sys/statvfs.h>
#وإلاإذا HAVE_SYS_VFS_H
# include <sys/vfs.h>
#وإلاإذا HAVE_SYS_MOUNT_H && HAVE_SYS_PARAM_H
/* NOTE: freebsd5.0 needs sys/param.h and sys/mount.h for statfs.
   It does have statvfs.h, but shouldn't use it, since it doesn't
   HAVE_STRUCT_STATVFS_F_BASETYPE.  So find a clean way to fix it.  */
/* NetBSD 1.5.2 needs these, for the declaration of struct statfs. */
# include <sys/param.h>
# include <sys/mount.h>
# אם HAVE_NFS_NFS_CLNT_H && HAVE_NFS_VFS_H
/* Ultrix 4.4 needs these for the declaration of struct statfs.  */
#  include <netinet/in.h>
#  include <nfs/nfs_clnt.h>
#  include <nfs/vfs.h>
# endif
#وإلاإذا HAVE_OS_H /* BeOS */
# include <fs_info.h>
#endif
#ضمن <selinux/selinux.h>
#ضمن <getopt.h>

#ضمن "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#ضمن "system.h"

#ضمن "areadlink.h"
#ضمن "c-ctype.h"
#ضمن "file-type.h"
#ضمن "filemode.h"
#ضمن "fs.h"
#ضمن "mountlist.h"
#ضمن "octhexdigits.h"
#ضمن "quote.h"
#ضمن "stat-size.h"
#ضمن "stat-time.h"
#ضمن "strftime.h"
#ضمن "find-mount-point.h"
#ضمن "xvasprintf.h"
#ضمن "statx.h"

#אם HAVE_STATX && defined STATX_INO
# define USE_STATX 1
#אחרת
# define USE_STATX 0
#endif

#אם USE_STATVFS
# define STRUCT_STATXFS_F_FSID_IS_INTEGER STRUCT_STATVFS_F_FSID_IS_INTEGER
# define HAVE_STRUCT_STATXFS_F_TYPE HAVE_STRUCT_STATVFS_F_TYPE
# אם HAVE_STRUCT_STATVFS_F_NAMEMAX
#  define SB_F_NAMEMAX(S) ((S)->f_namemax)
# endif
# אם ! STAT_STATVFS && STAT_STATVFS64
#  define STRUCT_STATVFS بنية statvfs64
#  define STATFS statvfs64
# אחרת
#  define STRUCT_STATVFS بنية statvfs
#  define STATFS statvfs
# endif
# define STATFS_FRSIZE(S) ((S)->f_frsize)
#אחרת
# define HAVE_STRUCT_STATXFS_F_TYPE HAVE_STRUCT_STATFS_F_TYPE
# אם HAVE_STRUCT_STATFS_F_NAMELEN
#  define SB_F_NAMEMAX(S) ((S)->f_namelen)
# elif HAVE_STRUCT_STATFS_F_NAMEMAX
#  define SB_F_NAMEMAX(S) ((S)->f_namemax)
# endif
# define STATFS statfs
# אם HAVE_OS_H /* BeOS */
/* BeOS has a statvfs function, but it does not return sensible values
   for f_files, f_ffree and f_favail, and lacks f_type, f_basetype and
   f_fstypename.  Use 'struct fs_info' instead.  */
NODISCARD
ساكن صحيح
statfs (حرف ثابت *filename, بنية fs_info *buf)
{
  dev_t device = dev_for_path (filename);
  אם (device < 0)
    {
      errno = (device == B_ENTRY_NOT_FOUND ? ENOENT
               : device == B_BAD_VALUE ? EINVAL
               : device == B_NAME_TOO_LONG ? ENAMETOOLONG
               : device == B_NO_MEMORY ? ENOMEM
               : device == B_FILE_ERROR ? EIO
               : 0);
      החזר -1;
    }
  /* If successful, buf->dev will be == device.  */
  החזר fs_stat_dev (device, buf);
}
#  define f_fsid dev
#  define f_blocks total_blocks
#  define f_bfree free_blocks
#  define f_bavail free_blocks
#  define f_bsize io_size
#  define f_files total_nodes
#  define f_ffree free_nodes
#  define STRUCT_STATVFS بنية fs_info
#  define STRUCT_STATXFS_F_FSID_IS_INTEGER true
#  define STATFS_FRSIZE(S) ((S)->block_size)
# אחרת
#  define STRUCT_STATVFS بنية statfs
#  define STRUCT_STATXFS_F_FSID_IS_INTEGER STRUCT_STATFS_F_FSID_IS_INTEGER
#  אם HAVE_STRUCT_STATFS_F_FRSIZE
#   define STATFS_FRSIZE(S) ((S)->f_frsize)
#  אחרת
#   define STATFS_FRSIZE(S) 0
#  endif
# endif
#endif

#ifdef SB_F_NAMEMAX
# define OUT_NAMEMAX out_uint
#אחרת
/* Depending on whether statvfs or statfs is used,
   neither f_namemax or f_namelen may be available.  */
# define SB_F_NAMEMAX(S) "?"
# define OUT_NAMEMAX out_string
#endif

#אם HAVE_STRUCT_STATVFS_F_BASETYPE
# define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME f_basetype
#אחרת
# אם HAVE_STRUCT_STATVFS_F_FSTYPENAME || HAVE_STRUCT_STATFS_F_FSTYPENAME
#  define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME f_fstypename
# elif HAVE_OS_H /* BeOS */
#  define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME fsh_name
# endif
#endif

#אם HAVE_GETATTRAT
# include <attr.h>
# include <sys/nvpair.h>
#endif

ساكن حرف ثابت digits[] = "0123456789";

/* Flags that are portable for use in printf, for at least one
   conversion specifier; make_format removes non-portable flags as
   needed for particular specifiers.  The glibc 2.2 extension "I" is
   listed here; it is removed by make_format because it has undefined
   behavior elsewhere and because it is incompatible with
   out_epoch_sec.  */
ساكن حرف ثابت printf_flags[] = "'-+ #0I";

/* Formats for the --terse option.  */
ساكن حرف ثابت fmt_terse_fs[] = "%Qn %i %l %t %s %S %b %f %a %c %d\n";
ساكن حرف ثابت fmt_terse_regular[] = "%Qn %s %b %f %u %g %D %i %h %t %T"
                                        " %X %Y %Z %W %o\n";
ساكن حرف ثابت fmt_terse_selinux[] = "%Qn %s %b %f %u %g %D %i %h %t %T"
                                        " %X %Y %Z %W %o %C\n";

#define PROGRAM_NAME "stat"

#define AUTHORS proper_name ("Michael Meskes")

تعداد
{
  PRINTF_OPTION = CHAR_MAX + 1
};

تعداد cached_mode
{
  cached_default,
  cached_never,
  cached_always
};

ساكن حرف ثابت *ثابت cached_args[] =
{
  "default", "never", "always", عدم
};

ساكن تعداد cached_mode ثابت cached_modes[] =
{
  cached_default, cached_never, cached_always
};

ساكن بنية option ثابت long_options[] =
{
  {"dereference", no_argument, عدم, 'L'},
  {"file-system", no_argument, عدم, 'f'},
  {"format", required_argument, عدم, 'c'},
  {"printf", required_argument, عدم, PRINTF_OPTION},
  {"terse", no_argument, عدم, 't'},
  {"cached", required_argument, عدم, 0},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

/* Whether to follow symbolic links;  True for --dereference (-L).  */
ساكن bool follow_links;

/* Whether to interpret backslash-escape sequences.
   True for --printf=FMT, not for --format=FMT (-c).  */
ساكن bool interpret_backslash_escapes;

/* The trailing delimiter string:
   "" for --printf=FMT, "\n" for --format=FMT (-c).  */
ساكن حرف ثابت *trailing_delim = "";

/* The representation of the decimal point in the current locale.  */
ساكن حرف ثابت *decimal_point;
ساكن size_t decimal_point_len;

ساكن bool
print_stat (حرف *pformat, size_t prefix_len, حرف mod, حرف m,
            صحيح fd, حرف ثابت *filename, فراغ ثابت *data);

/* Return the type of the specified file system.
   Some systems have statfvs.f_basetype[FSTYPSZ] (AIX, HP-UX, and Solaris).
   Others have statvfs.f_fstypename[_VFS_NAMELEN] (NetBSD 3.0).
   Others have statfs.f_fstypename[MFSNAMELEN] (NetBSD 1.5.2).
   Still others have neither and have to get by with f_type (GNU/Linux).
   But f_type may only exist in statfs (Cygwin).  */
NODISCARD
ساكن حرف ثابت *
human_fstype (STRUCT_STATVFS ثابت *statfsbuf)
{
#ifdef STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME
  החזר statfsbuf->STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME;
#אחרת
  بدل (statfsbuf->f_type)
    {
# אם defined __linux__ || defined __ANDROID__

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

    حالة S_MAGIC_AAFS: /* 0x5A3C69F0 local */
      החזר "aafs";
    حالة S_MAGIC_ACFS: /* 0x61636673 remote */
      החזר "acfs";
    حالة S_MAGIC_ADFS: /* 0xADF5 local */
      החזר "adfs";
    حالة S_MAGIC_AFFS: /* 0xADFF local */
      החזר "affs";
    حالة S_MAGIC_AFS: /* 0x5346414F remote */
      החזר "afs";
    حالة S_MAGIC_ANON_INODE_FS: /* 0x09041934 local */
      החזר "anon-inode FS";
    حالة S_MAGIC_AUFS: /* 0x61756673 remote */
      /* FIXME: change syntax or add an optional attribute like "inotify:no".
         The above is labeled as "remote" so that tail always uses polling,
         but this isn't really a remote file system type.  */
      החזר "aufs";
    حالة S_MAGIC_AUTOFS: /* 0x0187 local */
      החזר "autofs";
    حالة S_MAGIC_BALLOON_KVM: /* 0x13661366 local */
      החזר "balloon-kvm-fs";
    حالة S_MAGIC_BCACHEFS: /* 0xCA451A4E local */
      החזר "bcachefs";
    حالة S_MAGIC_BEFS: /* 0x42465331 local */
      החזר "befs";
    حالة S_MAGIC_BDEVFS: /* 0x62646576 local */
      החזר "bdevfs";
    حالة S_MAGIC_BFS: /* 0x1BADFACE local */
      החזר "bfs";
    حالة S_MAGIC_BINDERFS: /* 0x6C6F6F70 local */
      החזר "binderfs";
    حالة S_MAGIC_BPF_FS: /* 0xCAFE4A11 local */
      החזר "bpf_fs";
    حالة S_MAGIC_BINFMTFS: /* 0x42494E4D local */
      החזר "binfmt_misc";
    حالة S_MAGIC_BTRFS: /* 0x9123683E local */
      החזר "btrfs";
    حالة S_MAGIC_BTRFS_TEST: /* 0x73727279 local */
      החזר "btrfs_test";
    حالة S_MAGIC_CEPH: /* 0x00C36400 remote */
      החזר "ceph";
    حالة S_MAGIC_CGROUP: /* 0x0027E0EB local */
      החזר "cgroupfs";
    حالة S_MAGIC_CGROUP2: /* 0x63677270 local */
      החזר "cgroup2fs";
    حالة S_MAGIC_CIFS: /* 0xFF534D42 remote */
      החזר "cifs";
    حالة S_MAGIC_CODA: /* 0x73757245 remote */
      החזר "coda";
    حالة S_MAGIC_COH: /* 0x012FF7B7 local */
      החזר "coh";
    حالة S_MAGIC_CONFIGFS: /* 0x62656570 local */
      החזר "configfs";
    حالة S_MAGIC_CRAMFS: /* 0x28CD3D45 local */
      החזר "cramfs";
    حالة S_MAGIC_CRAMFS_WEND: /* 0x453DCD28 local */
      החזר "cramfs-wend";
    حالة S_MAGIC_DAXFS: /* 0x64646178 local */
      החזר "daxfs";
    حالة S_MAGIC_DEBUGFS: /* 0x64626720 local */
      החזר "debugfs";
    حالة S_MAGIC_DEVFS: /* 0x1373 local */
      החזר "devfs";
    حالة S_MAGIC_DEVMEM: /* 0x454D444D local */
      החזר "devmem";
    حالة S_MAGIC_DEVPTS: /* 0x1CD1 local */
      החזר "devpts";
    حالة S_MAGIC_DMA_BUF: /* 0x444D4142 local */
      החזר "dma-buf-fs";
    حالة S_MAGIC_ECRYPTFS: /* 0xF15F local */
      החזר "ecryptfs";
    حالة S_MAGIC_EFIVARFS: /* 0xDE5E81E4 local */
      החזר "efivarfs";
    حالة S_MAGIC_EFS: /* 0x00414A53 local */
      החזר "efs";
    حالة S_MAGIC_EROFS_V1: /* 0xE0F5E1E2 local */
      החזר "erofs";
    حالة S_MAGIC_EXFAT: /* 0x2011BAB0 local */
      החזר "exfat";
    حالة S_MAGIC_EXFS: /* 0x45584653 local */
      החזר "exfs";
    حالة S_MAGIC_EXOFS: /* 0x5DF5 local */
      החזר "exofs";
    حالة S_MAGIC_EXT: /* 0x137D local */
      החזר "ext";
    حالة S_MAGIC_EXT2: /* 0xEF53 local */
      החזר "ext2/ext3";
    حالة S_MAGIC_EXT2_OLD: /* 0xEF51 local */
      החזר "ext2";
    حالة S_MAGIC_F2FS: /* 0xF2F52010 local */
      החזר "f2fs";
    حالة S_MAGIC_FAIL_FS: /* 0x4641494C local */
      החזר "failfs";
    حالة S_MAGIC_FAT: /* 0x4006 local */
      החזר "fat";
    حالة S_MAGIC_FHGFS: /* 0x19830326 remote */
      החזר "fhgfs";
    حالة S_MAGIC_FUSE: /* 0x65735546 remote */
      החזר "fuse";
    حالة S_MAGIC_FUSECTL: /* 0x65735543 remote */
      החזר "fusectl";
    حالة S_MAGIC_FUTEXFS: /* 0x0BAD1DEA local */
      החזר "futexfs";
    حالة S_MAGIC_GFS: /* 0x01161970 remote */
      החזר "gfs/gfs2";
    حالة S_MAGIC_GPFS: /* 0x47504653 remote */
      החזר "gpfs";
    حالة S_MAGIC_GUEST_MEMFD: /* 0x474D454D remote */
      החזר "guest-memfd";
    حالة S_MAGIC_HFS: /* 0x4244 local */
      החזר "hfs";
    حالة S_MAGIC_HFS_PLUS: /* 0x482B local */
      החזר "hfs+";
    حالة S_MAGIC_HFS_X: /* 0x4858 local */
      החזר "hfsx";
    حالة S_MAGIC_HOSTFS: /* 0x00C0FFEE local */
      החזר "hostfs";
    حالة S_MAGIC_HPFS: /* 0xF995E849 local */
      החזר "hpfs";
    حالة S_MAGIC_HUGETLBFS: /* 0x958458F6 local */
      החזר "hugetlbfs";
    حالة S_MAGIC_MTD_INODE_FS: /* 0x11307854 local */
      החזר "inodefs";
    حالة S_MAGIC_IBRIX: /* 0x013111A8 remote */
      החזר "ibrix";
    حالة S_MAGIC_INOTIFYFS: /* 0x2BAD1DEA local */
      החזר "inotifyfs";
    حالة S_MAGIC_ISOFS: /* 0x9660 local */
      החזר "isofs";
    حالة S_MAGIC_ISOFS_R_WIN: /* 0x4004 local */
      החזר "isofs";
    حالة S_MAGIC_ISOFS_WIN: /* 0x4000 local */
      החזר "isofs";
    حالة S_MAGIC_JFFS: /* 0x07C0 local */
      החזר "jffs";
    حالة S_MAGIC_JFFS2: /* 0x72B6 local */
      החזר "jffs2";
    حالة S_MAGIC_JFS: /* 0x3153464A local */
      החזר "jfs";
    حالة S_MAGIC_KAFS: /* 0x6B414653 remote */
      החזר "k-afs";
    حالة S_MAGIC_LOGFS: /* 0xC97E8168 local */
      החזר "logfs";
    حالة S_MAGIC_LUSTRE: /* 0x0BD00BD0 remote */
      החזר "lustre";
    حالة S_MAGIC_M1FS: /* 0x5346314D local */
      החזר "m1fs";
    حالة S_MAGIC_MINIX: /* 0x137F local */
      החזר "minix";
    حالة S_MAGIC_MINIX_30: /* 0x138F local */
      החזר "minix (30 char.)";
    حالة S_MAGIC_MINIX_V2: /* 0x2468 local */
      החזר "minix v2";
    حالة S_MAGIC_MINIX_V2_30: /* 0x2478 local */
      החזר "minix v2 (30 char.)";
    حالة S_MAGIC_MINIX_V3: /* 0x4D5A local */
      החזר "minix3";
    حالة S_MAGIC_MQUEUE: /* 0x19800202 local */
      החזר "mqueue";
    حالة S_MAGIC_MSDOS: /* 0x4D44 local */
      החזר "msdos";
    حالة S_MAGIC_NCP: /* 0x564C remote */
      החזר "novell";
    حالة S_MAGIC_NFS: /* 0x6969 remote */
      החזר "nfs";
    حالة S_MAGIC_NFSD: /* 0x6E667364 remote */
      החזר "nfsd";
    حالة S_MAGIC_NILFS: /* 0x3434 local */
      החזר "nilfs";
    حالة S_MAGIC_NSFS: /* 0x6E736673 local */
      החזר "nsfs";
    حالة S_MAGIC_NTFS: /* 0x5346544E local */
      החזר "ntfs";
    حالة S_MAGIC_NULL_FS: /* 0x4E554C4C local */
      החזר "nullfs";
    حالة S_MAGIC_OPENPROM: /* 0x9FA1 local */
      החזר "openprom";
    حالة S_MAGIC_OCFS2: /* 0x7461636F remote */
      החזר "ocfs2";
    حالة S_MAGIC_OVERLAYFS: /* 0x794C7630 remote */
      /* This may overlay remote file systems.
         Also there have been issues reported with inotify and overlayfs,
         so mark as "remote" so that polling is used.  */
      החזר "overlayfs";
    حالة S_MAGIC_PANFS: /* 0xAAD7AAEA remote */
      החזר "panfs";
    حالة S_MAGIC_PID_FS: /* 0x50494446 local */
      החזר "pidfs";
    حالة S_MAGIC_PIPEFS: /* 0x50495045 remote */
      /* FIXME: change syntax or add an optional attribute like "inotify:no".
         pipefs is labeled as "remote" so that tail always polls,
         but it isn't really remote file system type.  */
      החזר "pipefs";
    حالة S_MAGIC_PPC_CMM: /* 0xC7571590 local */
      החזר "ppc-cmm-fs";
    حالة S_MAGIC_PRL_FS: /* 0x7C7C6673 remote */
      החזר "prl_fs";
    حالة S_MAGIC_PROC: /* 0x9FA0 local */
      החזר "proc";
    حالة S_MAGIC_PSTOREFS: /* 0x6165676C local */
      החזר "pstorefs";
    حالة S_MAGIC_QNX4: /* 0x002F local */
      החזר "qnx4";
    حالة S_MAGIC_QNX6: /* 0x68191122 local */
      החזר "qnx6";
    حالة S_MAGIC_RAMFS: /* 0x858458F6 local */
      החזר "ramfs";
    حالة S_MAGIC_RDTGROUP: /* 0x07655821 local */
      החזר "rdt";
    حالة S_MAGIC_REISERFS: /* 0x52654973 local */
      החזר "reiserfs";
    حالة S_MAGIC_ROMFS: /* 0x7275 local */
      החזר "romfs";
    حالة S_MAGIC_RPC_PIPEFS: /* 0x67596969 local */
      החזר "rpc_pipefs";
    حالة S_MAGIC_SDCARDFS: /* 0x5DCA2DF5 local */
      החזר "sdcardfs";
    حالة S_MAGIC_SECRETMEM: /* 0x5345434D local */
      החזר "secretmem";
    حالة S_MAGIC_SECURITYFS: /* 0x73636673 local */
      החזר "securityfs";
    حالة S_MAGIC_SELINUX: /* 0xF97CFF8C local */
      החזר "selinux";
    حالة S_MAGIC_SMACK: /* 0x43415D53 local */
      החזר "smackfs";
    حالة S_MAGIC_SMB: /* 0x517B remote */
      החזר "smb";
    حالة S_MAGIC_SMB2: /* 0xFE534D42 remote */
      החזר "smb2";
    حالة S_MAGIC_SNFS: /* 0xBEEFDEAD remote */
      החזר "snfs";
    حالة S_MAGIC_SOCKFS: /* 0x534F434B local */
      החזר "sockfs";
    حالة S_MAGIC_SQUASHFS: /* 0x73717368 local */
      החזר "squashfs";
    حالة S_MAGIC_SYSFS: /* 0x62656572 local */
      החזר "sysfs";
    حالة S_MAGIC_SYSV2: /* 0x012FF7B6 local */
      החזר "sysv2";
    حالة S_MAGIC_SYSV4: /* 0x012FF7B5 local */
      החזר "sysv4";
    حالة S_MAGIC_TMPFS: /* 0x01021994 local */
      החזר "tmpfs";
    حالة S_MAGIC_TRACEFS: /* 0x74726163 local */
      החזר "tracefs";
    حالة S_MAGIC_UBIFS: /* 0x24051905 local */
      החזר "ubifs";
    حالة S_MAGIC_UDF: /* 0x15013346 local */
      החזר "udf";
    حالة S_MAGIC_UFS: /* 0x00011954 local */
      החזר "ufs";
    حالة S_MAGIC_UFS_BYTESWAPPED: /* 0x54190100 local */
      החזר "ufs";
    حالة S_MAGIC_USBDEVFS: /* 0x9FA2 local */
      החזר "usbdevfs";
    حالة S_MAGIC_V9FS: /* 0x01021997 local */
      החזר "v9fs";
    حالة S_MAGIC_VBOXSF: /* 0x786F4256 remote */
      החזר "vboxsf";
    حالة S_MAGIC_VMHGFS: /* 0xBACBACBC remote */
      החזר "vmhgfs";
    حالة S_MAGIC_VXFS: /* 0xA501FCF5 remote */
      /* Veritas File System can run in single instance or clustered mode,
         so mark as remote to cater for the latter case.  */
      החזר "vxfs";
    حالة S_MAGIC_VZFS: /* 0x565A4653 local */
      החזר "vzfs";
    حالة S_MAGIC_WSLFS: /* 0x53464846 local */
      החזר "wslfs";
    حالة S_MAGIC_XENFS: /* 0xABBA1974 local */
      החזר "xenfs";
    حالة S_MAGIC_XENIX: /* 0x012FF7B4 local */
      החזר "xenix";
    حالة S_MAGIC_XFS: /* 0x58465342 local */
      החזר "xfs";
    حالة S_MAGIC_XIAFS: /* 0x012FD16D local */
      החזר "xia";
    حالة S_MAGIC_Z3FOLD: /* 0x0033 local */
      החזר "z3fold";
    حالة S_MAGIC_ZFS: /* 0x2FC12FC1 local */
      החזר "zfs";
    حالة S_MAGIC_ZONEFS: /* 0x5A4F4653 local */
      החזר "zonefs";
    حالة S_MAGIC_ZSMALLOC: /* 0x58295829 local */
      החזר "zsmallocfs";


# elif __GNU__
    حالة FSTYPE_UFS:
      החזר "ufs";
    حالة FSTYPE_NFS:
      החזר "nfs";
    حالة FSTYPE_GFS:
      החזר "gfs";
    حالة FSTYPE_LFS:
      החזר "lfs";
    حالة FSTYPE_SYSV:
      החזר "sysv";
    حالة FSTYPE_FTP:
      החזר "ftp";
    حالة FSTYPE_TAR:
      החזר "tar";
    حالة FSTYPE_AR:
      החזר "ar";
    حالة FSTYPE_CPIO:
      החזר "cpio";
    حالة FSTYPE_MSLOSS:
      החזר "msloss";
    حالة FSTYPE_CPM:
      החזר "cpm";
    حالة FSTYPE_HFS:
      החזר "hfs";
    حالة FSTYPE_DTFS:
      החזר "dtfs";
    حالة FSTYPE_GRFS:
      החזר "grfs";
    حالة FSTYPE_TERM:
      החזר "term";
    حالة FSTYPE_DEV:
      החזר "dev";
    حالة FSTYPE_PROC:
      החזר "proc";
    حالة FSTYPE_IFSOCK:
      החזר "ifsock";
    حالة FSTYPE_AFS:
      החזר "afs";
    حالة FSTYPE_DFS:
      החזר "dfs";
    حالة FSTYPE_PROC9:
      החזר "proc9";
    حالة FSTYPE_SOCKET:
      החזר "socket";
    حالة FSTYPE_MISC:
      החזר "misc";
    حالة FSTYPE_EXT2FS:
      החזר "ext2/ext3";
    حالة FSTYPE_HTTP:
      החזר "http";
    حالة FSTYPE_MEMFS:
      החזר "memfs";
    حالة FSTYPE_ISO9660:
      החזר "iso9660";
# endif
    افتراضي:
      {
        unsigned long صحيح type = statfsbuf->f_type;
        ساكن حرف buf[حجم "UNKNOWN (0x%lx)" - 3
                        + (حجم type * CHAR_BIT + 3) / 4];
        sprintf (buf, "UNKNOWN (0x%lx)", type);
        החזר buf;
      }
    }
#endif
}

NODISCARD
ساكن حرف *
human_access (بنية stat ثابت *statbuf)
{
  ساكن حرف modebuf[12];
  filemodestring (statbuf, modebuf);
  modebuf[10] = 0;
  החזר modebuf;
}

NODISCARD
ساكن حرف *
human_time (بنية timespec t)
{
  /* STR must be at least INT_BUFSIZE_BOUND (intmax_t) big, either
     because localtime_rz fails, or because the time zone is truly
     outlandish so that %z expands to a long string.  */
  ساكن حرف str[INT_BUFSIZE_BOUND (intmax_t)
                  + INT_STRLEN_BOUND (صحيح) /* YYYY */
                  + 1 /* because YYYY might equal INT_MAX + 1900 */
                  + حجم "-MM-DD HH:MM:SS.NNNNNNNNN +"];
  ساكن timezone_t tz;
  אם (!tz)
    tz = tzalloc (getenv ("TZ"));
  بنية tm tm;
  صحيح ns = t.tv_nsec;
  אם (localtime_rz (tz, &t.tv_sec, &tm))
    nstrftime (str, حجم str, "%Y-%m-%d %H:%M:%S.%N %z", &tm, tz, ns);
  אחרת
    {
      حرف secbuf[INT_BUFSIZE_BOUND (intmax_t)];
      sprintf (str, "%s.%09d", timetostr (t.tv_sec, secbuf), ns);
    }
  החזר str;
}

/* PFORMAT points to a '%' followed by a prefix of a format, all of
   size PREFIX_LEN.  The flags allowed for this format are
   ALLOWED_FLAGS; remove other printf flags from the prefix, then
   append SUFFIX.  */
ساكن فراغ
make_format (حرف *pformat, size_t prefix_len, حرف ثابت *allowed_flags,
             حرف ثابت *suffix)
{
  حرف *dst = pformat + 1;
  حرف ثابت *src;
  حرف ثابت *srclim = pformat + prefix_len;
  עבור (src = dst; src < srclim && strchr (printf_flags, *src); src++)
    אם (strchr (allowed_flags, *src))
      *dst++ = *src;
  כלעוד (src < srclim)
    *dst++ = *src++;
  strcpy (dst, suffix);
}

ساكن فراغ
out_string (حرف *pformat, size_t prefix_len, حرف ثابت *arg)
{
  make_format (pformat, prefix_len, "-", "s");
  printf (pformat, arg);
}
ساكن صحيح
out_int (حرف *pformat, size_t prefix_len, intmax_t arg)
{
  make_format (pformat, prefix_len, "'-+ 0", "jd");
  החזר printf (pformat, arg);
}
ساكن صحيح
out_uint (حرف *pformat, size_t prefix_len, uintmax_t arg)
{
  make_format (pformat, prefix_len, "'-0", "ju");
  החזר printf (pformat, arg);
}
ساكن فراغ
out_uint_o (حرف *pformat, size_t prefix_len, uintmax_t arg)
{
  make_format (pformat, prefix_len, "-#0", "jo");
  printf (pformat, arg);
}
ساكن فراغ
out_uint_x (حرف *pformat, size_t prefix_len, uintmax_t arg)
{
  make_format (pformat, prefix_len, "-#0", "jx");
  printf (pformat, arg);
}
ساكن صحيح
out_minus_zero (حرف *pformat, size_t prefix_len)
{
  make_format (pformat, prefix_len, "'-+ 0", ".0f");
  החזר printf (pformat, -0.25);
}

/* Output the number of seconds since the Epoch, using a format that
   acts like printf's %f format.  */
ساكن فراغ
out_epoch_sec (حرف *pformat, size_t prefix_len,
               بنية timespec arg)
{
  حرف *dot = memchr (pformat, '.', prefix_len);
  size_t sec_prefix_len = prefix_len;
  صحيح width = 0;
  صحيح precision = 0;
  bool frac_left_adjust = false;

  אם (dot)
    {
      sec_prefix_len = dot - pformat;
      pformat[prefix_len] = '\0';

      אם (c_isdigit (dot[1]))
        {
          long صحيح lprec = strtol (dot + 1, عدم, 10);
          precision = (lprec <= INT_MAX ? lprec : INT_MAX);
        }
      אחרת
        {
          precision = 9;
        }

      אם (precision && c_isdigit (dot[-1]))
        {
          /* If a nontrivial width is given, subtract the width of the
             decimal point and PRECISION digits that will be output
             later.  */
          حرف *p = dot;
          *dot = '\0';

          עשה
            --p;
          כלעוד (c_isdigit (p[-1]));

          long صحيح lwidth = strtol (p, عدم, 10);
          width = (lwidth <= INT_MAX ? lwidth : INT_MAX);
          אם (1 < width)
            {
              p += (*p == '0');
              sec_prefix_len = p - pformat;
              صحيح w_d = (decimal_point_len < width
                         ? width - decimal_point_len
                         : 0);
              אם (1 < w_d)
                {
                  صحيح w = w_d - precision;
                  אם (1 < w)
                    {
                      حرف *dst = pformat;
                      עבור (حرف ثابت *src = dst; src < p; src++)
                        {
                          אם (*src == '-')
                            frac_left_adjust = true;
                          אחרת
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

  صحيح divisor = 1;
  עבור (صحيح i = precision; i < 9; i++)
    divisor *= 10;
  صحيح frac_sec = arg.tv_nsec / divisor;
  صحيح int_len;

  אם (TYPE_SIGNED (time_t))
    {
      bool minus_zero = false;
      אם (arg.tv_sec < 0 && arg.tv_nsec != 0)
        {
          صحيح frac_sec_modulus = 1000000000 / divisor;
          frac_sec = (frac_sec_modulus - frac_sec
                      - (arg.tv_nsec % divisor != 0));
          arg.tv_sec += (frac_sec != 0);
          minus_zero = (arg.tv_sec == 0);
        }
      int_len = (minus_zero
                 ? out_minus_zero (pformat, sec_prefix_len)
                 : out_int (pformat, sec_prefix_len, arg.tv_sec));
    }
  אחרת
    int_len = out_uint (pformat, sec_prefix_len, arg.tv_sec);

  אם (precision)
    {
      صحيح prec = (precision < 9 ? precision : 9);
      صحيح trailing_prec = precision - prec;
      صحيح ilen = (int_len < 0 ? 0 : int_len);
      صحيح trailing_width = (ilen < width && decimal_point_len < width - ilen
                            ? width - ilen - decimal_point_len - prec
                            : 0);
      printf ("%s%.*d%-*.*d", decimal_point, prec, frac_sec,
              trailing_width, trailing_prec, 0);
    }
}

/* Print the context information of FILENAME, and return true iff the
   context could not be obtained.  */
NODISCARD
ساكن bool
out_file_context (حرف *pformat, size_t prefix_len, حرف ثابت *filename)
{
  حرف *scontext;
  bool fail = false;

  אם ((follow_links
       ? getfilecon (filename, &scontext)
       : lgetfilecon (filename, &scontext)) < 0)
    {
      error (0, errno, _("failed to get security context of %s"),
             quoteaf (filename));
      scontext = عدم;
      fail = true;
    }
  strcpy (pformat + prefix_len, "s");
  printf (pformat, (scontext ? scontext : "?"));
  אם (scontext)
    freecon (scontext);
  החזר fail;
}

/* Set the quoting style once it is needed.  */
ساكن فراغ
initialize_quoting_style (فراغ)
{
  ساكن bool initialized;
  אם (!initialized)
    {
      initialized = true;
      صحيح qs = getenv_quoting_style ();
      set_quoting_style (عدم, 0 <= qs ? qs : shell_escape_quoting_style);
    }
}

/* Print statfs info.  Return zero upon success, nonzero upon failure.  */
NODISCARD
ساكن bool
print_statfs (حرف *pformat, size_t prefix_len, MAYBE_UNUSED حرف mod, حرف m,
              MAYBE_UNUSED صحيح fd, حرف ثابت *filename,
              فراغ ثابت *data)
{
  STRUCT_STATVFS ثابت *statfsbuf = data;
  bool fail = false;

  بدل (m)
    {
    حالة 'n':
      אם (mod == 'Q')
        {
          initialize_quoting_style ();
          filename = quoteN (filename);
        }
      out_string (pformat, prefix_len, filename);
      اكسر;

    حالة 'i':
      {
#אם STRUCT_STATXFS_F_FSID_IS_INTEGER
        uintmax_t fsid = statfsbuf->f_fsid;
#אחרת
        تعريفنوع unsigned صحيح fsid_word;
        static_assert (alignof (STRUCT_STATVFS) % alignof (fsid_word) == 0);
        static_assert (offsetof (STRUCT_STATVFS, f_fsid) % alignof (fsid_word)
                       == 0);
        static_assert (حجم statfsbuf->f_fsid % alignof (fsid_word) == 0);
        fsid_word ثابت *p = (fsid_word *) &statfsbuf->f_fsid;

        /* Assume a little-endian word order, as that is compatible
           with glibc's statvfs implementation.  */
        uintmax_t fsid = 0;
        صحيح words = حجم statfsbuf->f_fsid / حجم *p;
        עבור (صحيح i = 0; i < words && i * حجم *p < حجم fsid; i++)
          {
            uintmax_t u = p[words - 1 - i];
            fsid |= u << (i * CHAR_BIT * حجم *p);
          }
#endif
        out_uint_x (pformat, prefix_len, fsid);
      }
      اكسر;

    حالة 'l':
      OUT_NAMEMAX (pformat, prefix_len, SB_F_NAMEMAX (statfsbuf));
      اكسر;
    حالة 't':
#אם HAVE_STRUCT_STATXFS_F_TYPE
      out_uint_x (pformat, prefix_len, statfsbuf->f_type);
#אחרת
      fputc ('?', stdout);
#endif
      اكسر;
    حالة 'T':
      out_string (pformat, prefix_len, human_fstype (statfsbuf));
      اكسر;
    حالة 'b':
      out_int (pformat, prefix_len, statfsbuf->f_blocks);
      اكسر;
    حالة 'f':
      out_int (pformat, prefix_len, statfsbuf->f_bfree);
      اكسر;
    حالة 'a':
      out_int (pformat, prefix_len, statfsbuf->f_bavail);
      اكسر;
    حالة 's':
      out_uint (pformat, prefix_len, statfsbuf->f_bsize);
      اكسر;
    حالة 'S':
      {
        uintmax_t frsize = STATFS_FRSIZE (statfsbuf);
        אם (! frsize)
          frsize = statfsbuf->f_bsize;
        out_uint (pformat, prefix_len, frsize);
      }
      اكسر;
    حالة 'c':
      out_uint (pformat, prefix_len, statfsbuf->f_files);
      اكسر;
    حالة 'd':
      out_int (pformat, prefix_len, statfsbuf->f_ffree);
      اكسر;
    افتراضي:
      fputc ('?', stdout);
      اكسر;
    }
  החזר fail;
}

/* Return any bind mounted source for a path.
   The caller should not free the returned buffer.
   Return NULL if no bind mount found.  */
NODISCARD
ساكن حرف ثابت *
find_bind_mount (حرف ثابت * name)
{
  حرف ثابت * bind_mount = عدم;

  ساكن بنية mount_entry *mount_list;
  ساكن bool tried_mount_list = false;
  אם (!tried_mount_list) /* attempt/warn once per process.  */
    {
      אם (!(mount_list = read_file_system_list (false)))
        error (0, errno, "%s", _("cannot read table of mounted file systems"));
      tried_mount_list = true;
    }

  بنية stat name_stats;
  אם (stat (name, &name_stats) != 0)
    החזר عدم;

  עבור (بنية mount_entry *me = mount_list; me; me = me->me_next)
    {
      אם (me->me_dummy && me->me_devname[0] == '/'
          && streq (me->me_mountdir, name))
        {
          بنية stat dev_stats;

          אם (stat (me->me_devname, &dev_stats) == 0
              && psame_inode (&name_stats, &dev_stats))
            {
              bind_mount = me->me_devname;
              اكسر;
            }
        }
    }

  החזר bind_mount;
}

/* Print mount point.  Return zero upon success, nonzero upon failure.  */
NODISCARD
ساكن bool
out_mount_point (حرف ثابت *filename, حرف *pformat, size_t prefix_len,
                 ثابت بنية stat *statp)
{

  حرف ثابت *np = "?", *bp = عدم;
  حرف *mp = عدم;
  bool fail = true;

  /* Look for bind mounts first.  Note we output the immediate alias,
     rather than further resolving to a base device mount point.  */
  אם (follow_links || !S_ISLNK (statp->st_mode))
    {
      حرف *resolved = canonicalize_file_name (filename);
      אם (!resolved)
        {
          error (0, errno, _("failed to canonicalize %s"), quoteaf (filename));
          اذهب print_mount_point;
        }
      bp = find_bind_mount (resolved);
      حرر (resolved);
      אם (bp)
        {
          fail = false;
          اذهب print_mount_point;
        }
    }

  /* If there is no direct bind mount, then navigate
     back up the tree looking for a device change.
     Note we don't detect if any of the directory components
     are bind mounted to the same device, but that's OK
     since we've not directly queried them.  */
  אם ((mp = find_mount_point (filename, statp)))
    {
      /* This dir might be bind mounted to another device,
         so we resolve the bound source in that case also.  */
      bp = find_bind_mount (mp);
      fail = false;
    }

print_mount_point:

  out_string (pformat, prefix_len, bp ? bp : mp ? mp : np);
  حرر (mp);
  החזר fail;
}

/* Map a TS with negative TS.tv_nsec to {0,0}.  */
ساكن مضمن بنية timespec
neg_to_zero (بنية timespec ts)
{
  אם (0 <= ts.tv_nsec)
    החזר ts;
  بنية timespec z = {0};
  החזר z;
}

/* Output a single-character \ escape.  */

ساكن فراغ
print_esc_char (حرف c)
{
  بدل (c)
    {
    حالة 'a':			/* Alert. */
      c ='\a';
      اكسر;
    حالة 'b':			/* Backspace. */
      c ='\b';
      اكسر;
    حالة 'e':			/* Escape. */
      c ='\x1B';
      اكسر;
    حالة 'f':			/* Form feed. */
      c ='\f';
      اكسر;
    حالة 'n':			/* New line. */
      c ='\n';
      اكسر;
    حالة 'r':			/* Carriage return. */
      c ='\r';
      اكسر;
    حالة 't':			/* Horizontal tab. */
      c ='\t';
      اكسر;
    حالة 'v':			/* Vertical tab. */
      c ='\v';
      اكسر;
    حالة '"':
    حالة '\\':
      اكسر;
    افتراضي:
      error (0, 0, _("warning: unrecognized escape '\\%c'"), c);
      اكسر;
    }
  putchar (c);
}

ATTRIBUTE_PURE
ساكن size_t
format_code_offset (حرف ثابت *directive)
{
  size_t len = strspn (directive + 1, printf_flags);
  حرف ثابت *fmt_char = directive + len + 1;
  fmt_char += strspn (fmt_char, digits);
  אם (*fmt_char == '.')
    fmt_char += 1 + strspn (fmt_char + 1, digits);
  החזר fmt_char - directive;
}

/* Print the information specified by the format string, FORMAT,
   calling PRINT_FUNC for each %-directive encountered.
   Return zero upon success, nonzero upon failure.  */
NODISCARD
ساكن bool
print_it (حرف ثابت *format, صحيح fd, حرف ثابت *filename,
          bool (*print_func) (حرف *, size_t, حرف, حرف,
                              صحيح, حرف ثابت *, فراغ ثابت *),
          فراغ ثابت *data)
{
  bool fail = false;

  /* Add 2 to accommodate our conversion of the stat '%s' format string
     to the longer printf '%llu' one.  */
  تعداد
    {
      MAX_ADDITIONAL_BYTES =
        (MAX (حجم "jd",
              MAX (حجم "jo", MAX (حجم "ju", حجم "jx")))
         - 1)
    };
  size_t n_alloc = strlen (format) + MAX_ADDITIONAL_BYTES + 1;
  حرف *dest = xmalloc (n_alloc);
  עבור (حرف ثابت *b = format; *b; b++)
    {
      بدل (*b)
        {
        حالة '%':
          {
            size_t len = format_code_offset (b);
            حرف fmt_char = *(b + len);
            حرف mod_char = 0;
            memcpy (dest, b, len);
            b += len;

            بدل (fmt_char)
              {
              حالة '\0':
                --b;
                FALLTHROUGH;
              حالة '%':
                אם (1 < len)
                  {
                    dest[len] = fmt_char;
                    dest[len + 1] = '\0';
                    error (EXIT_FAILURE, 0, _("%s: invalid directive"),
                           quote (dest));
                  }
                putchar ('%');
                اكسر;
              حالة 'H':
              حالة 'L':
              حالة 'Q':
                mod_char = fmt_char;
                fmt_char = *(b + 1);
                אם ((mod_char == 'Q' && fmt_char == 'n')
                    || (print_func == print_stat
                        && (mod_char == 'H' || mod_char == 'L')
                        && (fmt_char == 'd' || fmt_char == 'r')))
                  {
                    b++;
                  }
                אחרת
                  {
                    fmt_char = mod_char;
                    mod_char = 0;
                  }
                FALLTHROUGH;
              افتراضي:
                fail |= print_func (dest, len, mod_char, fmt_char,
                                    fd, filename, data);
                اكسر;
              }
            اكسر;
          }

        حالة '\\':
          אם ( ! interpret_backslash_escapes)
            {
              putchar ('\\');
              اكسر;
            }
          ++b;
          אם (isoct (*b))
            {
              صحيح esc_value = fromoct (*b);
              صحيح esc_length = 1;	/* number of octal digits */
              עבור (++b; esc_length < 3 && isoct (*b);
                   ++esc_length, ++b)
                {
                  esc_value = esc_value * 8 + fromoct (*b);
                }
              putchar (esc_value);
              --b;
            }
          אחרת אם (*b == 'x' && c_isxdigit (b[1]))
            {
              صحيح esc_value = fromhex (b[1]);	/* Value of \xhh escape. */
              /* A hexadecimal \xhh escape sequence must have
                 1 or 2 hex. digits.  */
              ++b;
              אם (c_isxdigit (b[1]))
                {
                  ++b;
                  esc_value = esc_value * 16 + fromhex (*b);
                }
              putchar (esc_value);
            }
          אחרת אם (*b == '\0')
            {
              error (0, 0, _("warning: backslash at end of format"));
              putchar ('\\');
              /* Arrange to exit the loop.  */
              --b;
            }
          אחרת
            {
              print_esc_char (*b);
            }
          اكسر;

        افتراضي:
          putchar (*b);
          اكسر;
        }
    }
  حرر (dest);

  fputs (trailing_delim, stdout);

  החזר fail;
}

/* Stat the file system and print what we find.  */
NODISCARD
ساكن bool
do_statfs (حرف ثابت *filename, حرف ثابت *format)
{
  STRUCT_STATVFS statfsbuf;

  אם (streq (filename, "-"))
    {
      error (0, 0, _("using %s to denote standard input does not work"
                     " in file system mode"), quoteaf (filename));
      החזר false;
    }

  אם (STATFS (filename, &statfsbuf) != 0)
    {
      error (0, errno, _("cannot read file system information for %s"),
             quoteaf (filename));
      החזר false;
    }

  bool fail = print_it (format, -1, filename, print_statfs, &statfsbuf);
  החזר ! fail;
}

بنية print_args {
  بنية stat *st;
  بنية timespec btime;
};

/* Ask statx to avoid syncing? */
ساكن bool dont_sync;

/* Ask statx to force sync? */
ساكن bool force_sync;

#אם USE_STATX
ساكن unsigned صحيح
fmt_to_mask (حرف fmt)
{
  بدل (fmt)
    {
    حالة 'N':
      החזר STATX_MODE;
    حالة 'd':
    حالة 'D':
      החזר STATX_MODE;
    حالة 'i':
      החזר STATX_INO;
    حالة 'a':
    حالة 'A':
      החזר STATX_MODE;
    حالة 'f':
      החזר STATX_MODE|STATX_TYPE;
    حالة 'F':
      החזר STATX_TYPE;
    حالة 'h':
      החזר STATX_NLINK;
    حالة 'u':
    حالة 'U':
      החזר STATX_UID;
    حالة 'g':
    حالة 'G':
      החזר STATX_GID;
    حالة 'm':
      החזר STATX_MODE|STATX_INO;
    حالة 's':
      החזר STATX_SIZE;
    حالة 't':
    حالة 'T':
      החזר STATX_MODE;
    حالة 'b':
      החזר STATX_BLOCKS;
    حالة 'w':
    حالة 'W':
      החזר STATX_BTIME;
    حالة 'x':
    حالة 'X':
      החזר STATX_ATIME;
    حالة 'y':
    حالة 'Y':
      החזר STATX_MTIME;
    حالة 'z':
    حالة 'Z':
      החזר STATX_CTIME;
    }
  החזר 0;
}

ATTRIBUTE_PURE
ساكن unsigned صحيح
format_to_mask (حرف ثابت *format)
{
  unsigned صحيح mask = 0;

  עבור (حرف ثابت *b = format; *b; b++)
    {
      אם (*b != '%')
        استمر;

      b += format_code_offset (b);
      אם (*b == '\0')
        اكسر;
      mask |= fmt_to_mask (*b);
    }
  החזר mask;
}

/* statx the file and print what we find */
NODISCARD
ساكن bool
do_stat (حرف ثابت *filename, حرف ثابت *format, حرف ثابت *format2)
{
  صحيح fd = streq (filename, "-") ? 0 : AT_FDCWD;
  صحيح flags = 0;
  بنية stat st;
  بنية statx stx = {0};
  حرف ثابت *pathname = filename;
  بنية print_args pa;
  pa.st = &st;
  pa.btime = (بنية timespec) {.tv_sec = -1, .tv_nsec = -1};

  אם (AT_FDCWD != fd)
    {
      pathname = "";
      flags = AT_EMPTY_PATH;
    }
  אחרת אם (!follow_links)
    {
      flags = AT_SYMLINK_NOFOLLOW;
    }

  אם (dont_sync)
    flags |= AT_STATX_DONT_SYNC;
  אחרת אם (force_sync)
    flags |= AT_STATX_FORCE_SYNC;

  אם (! force_sync)
    flags |= AT_NO_AUTOMOUNT;

  fd = statx (fd, pathname, flags, format_to_mask (format), &stx);
  אם (fd < 0)
    {
      אם (flags & AT_EMPTY_PATH)
        error (0, errno, _("cannot stat standard input"));
      אחרת
        error (0, errno, _("cannot statx %s"), quoteaf (filename));
      החזר false;
    }

  אם (S_ISBLK (stx.stx_mode) || S_ISCHR (stx.stx_mode))
    format = format2;

  statx_to_stat (&stx, &st);
  אם (stx.stx_mask & STATX_BTIME)
    pa.btime = statx_timestamp_to_timespec (stx.stx_btime);

  bool fail = print_it (format, fd, filename, print_stat, &pa);
  החזר ! fail;
}

#אחרת /* USE_STATX */

ساكن بنية timespec
get_birthtime (صحيح fd, حرف ثابت *filename, بنية stat ثابت *st)
{
  بنية timespec ts = get_stat_birthtime (st);

# אם HAVE_GETATTRAT
  אם (ts.tv_nsec < 0)
    {
      nvlist_t *response;
      אם ((fd < 0
           ? getattrat (AT_FDCWD, XATTR_VIEW_READWRITE, filename, &response)
           : fgetattr (fd, XATTR_VIEW_READWRITE, &response))
          == 0)
        {
          uint64_t *val;
          uint_t n;
          אם (nvlist_lookup_uint64_array (response, A_CRTIME, &val, &n) == 0
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

  החזר ts;
}


/* stat the file and print what we find */
NODISCARD
ساكن bool
do_stat (حرف ثابت *filename, حرف ثابت *format,
         حرف ثابت *format2)
{
  صحيح fd = streq (filename, "-") ? 0 : -1;
  بنية stat statbuf;
  بنية print_args pa;
  pa.st = &statbuf;
  pa.btime = (بنية timespec) {.tv_sec = -1, .tv_nsec = -1};

  אם (0 <= fd)
    {
      אם (fstat (fd, &statbuf) != 0)
        {
          error (0, errno, _("cannot stat standard input"));
          החזר false;
        }
    }
  /* We can't use the shorter
     (follow_links?stat:lstat) (filename, &statbug)
     since stat might be a function-like macro.  */
  אחרת אם ((follow_links
            ? stat (filename, &statbuf)
            : lstat (filename, &statbuf)) != 0)
    {
      error (0, errno, _("cannot stat %s"), quoteaf (filename));
      החזר false;
    }

  אם (S_ISBLK (statbuf.st_mode) || S_ISCHR (statbuf.st_mode))
    format = format2;

  bool fail = print_it (format, fd, filename, print_stat, &pa);
  החזר ! fail;
}
#endif /* USE_STATX */

/* POSIX requires 'ls' to print file sizes without a sign, even
   when negative.  Be consistent with that.  */

ساكن uintmax_t
unsigned_file_size (off_t size)
{
  החזר size + (size < 0) * ((uintmax_t) OFF_T_MAX - OFF_T_MIN + 1);
}

/* Print stat info.  Return zero upon success, nonzero upon failure.  */
ساكن bool
print_stat (حرف *pformat, size_t prefix_len, حرف mod, حرف m,
            MAYBE_UNUSED صحيح fd, حرف ثابت *filename, فراغ ثابت *data)
{
  بنية print_args *parg = (بنية print_args *) data;
  بنية stat *statbuf = parg->st;
  بنية timespec btime = parg->btime;
  بنية passwd *pw_ent;
  بنية group *gw_ent;
  bool fail = false;

  بدل (m)
    {
    حالة 'n':
      אם (mod == 'Q')
        {
          initialize_quoting_style ();
          filename = quoteN (filename);
        }
      out_string (pformat, prefix_len, filename);
      اكسر;
    حالة 'N':
      initialize_quoting_style ();
      out_string (pformat, prefix_len, quoteN (filename));
      אם (S_ISLNK (statbuf->st_mode))
        {
          حرف *linkname = areadlink_with_size (filename, statbuf->st_size);
          אם (linkname == عدم)
            {
              error (0, errno, _("cannot read symbolic link %s"),
                     quoteaf (filename));
              החזר true;
            }
          printf (" -> ");
          out_string (pformat, prefix_len, quoteN (linkname));
          حرر (linkname);
        }
      اكسر;
    حالة 'd':
      אם (mod == 'H')
        out_uint (pformat, prefix_len, major (statbuf->st_dev));
      אחרת אם (mod == 'L')
        out_uint (pformat, prefix_len, minor (statbuf->st_dev));
      אחרת
        out_uint (pformat, prefix_len, statbuf->st_dev);
      اكسر;
    حالة 'D':
      out_uint_x (pformat, prefix_len, statbuf->st_dev);
      اكسر;
    حالة 'i':
      out_uint (pformat, prefix_len, statbuf->st_ino);
      اكسر;
    حالة 'a':
      out_uint_o (pformat, prefix_len, statbuf->st_mode & CHMOD_MODE_BITS);
      اكسر;
    حالة 'A':
      out_string (pformat, prefix_len, human_access (statbuf));
      اكسر;
    حالة 'f':
      out_uint_x (pformat, prefix_len, statbuf->st_mode);
      اكسر;
    حالة 'F':
      out_string (pformat, prefix_len, file_type (statbuf));
      اكسر;
    حالة 'h':
      out_uint (pformat, prefix_len, statbuf->st_nlink);
      اكسر;
    حالة 'u':
      out_uint (pformat, prefix_len, statbuf->st_uid);
      اكسر;
    حالة 'U':
      pw_ent = getpwuid (statbuf->st_uid);
      out_string (pformat, prefix_len,
                  pw_ent ? pw_ent->pw_name : "UNKNOWN");
      اكسر;
    حالة 'g':
      out_uint (pformat, prefix_len, statbuf->st_gid);
      اكسر;
    حالة 'G':
      gw_ent = getgrgid (statbuf->st_gid);
      out_string (pformat, prefix_len,
                  gw_ent ? gw_ent->gr_name : "UNKNOWN");
      اكسر;
    حالة 'm':
      fail |= out_mount_point (filename, pformat, prefix_len, statbuf);
      اكسر;
    حالة 's':
      out_uint (pformat, prefix_len, unsigned_file_size (statbuf->st_size));
      اكسر;
    حالة 'r':
      אם (mod == 'H')
        out_uint (pformat, prefix_len, major (statbuf->st_rdev));
      אחרת אם (mod == 'L')
        out_uint (pformat, prefix_len, minor (statbuf->st_rdev));
      אחרת
        out_uint (pformat, prefix_len, statbuf->st_rdev);
      اكسر;
    حالة 'R':
      out_uint_x (pformat, prefix_len, statbuf->st_rdev);
      اكسر;
    حالة 't':
      out_uint_x (pformat, prefix_len, major (statbuf->st_rdev));
      اكسر;
    حالة 'T':
      out_uint_x (pformat, prefix_len, minor (statbuf->st_rdev));
      اكسر;
    حالة 'B':
      out_uint (pformat, prefix_len, ST_NBLOCKSIZE);
      اكسر;
    حالة 'b':
      out_uint (pformat, prefix_len, STP_NBLOCKS (statbuf));
      اكسر;
    حالة 'o':
      out_uint (pformat, prefix_len, STP_BLKSIZE (statbuf));
      اكسر;
    حالة 'w':
      {
#אם ! USE_STATX
        btime = get_birthtime (fd, filename, statbuf);
#endif
        אם (btime.tv_nsec < 0)
          out_string (pformat, prefix_len, "-");
        אחרת
          out_string (pformat, prefix_len, human_time (btime));
      }
      اكسر;
    حالة 'W':
      {
#אם ! USE_STATX
        btime = get_birthtime (fd, filename, statbuf);
#endif
        out_epoch_sec (pformat, prefix_len, neg_to_zero (btime));
      }
      اكسر;
    حالة 'x':
      out_string (pformat, prefix_len, human_time (get_stat_atime (statbuf)));
      اكسر;
    حالة 'X':
      out_epoch_sec (pformat, prefix_len, get_stat_atime (statbuf));
      اكسر;
    حالة 'y':
      out_string (pformat, prefix_len, human_time (get_stat_mtime (statbuf)));
      اكسر;
    حالة 'Y':
      out_epoch_sec (pformat, prefix_len, get_stat_mtime (statbuf));
      اكسر;
    حالة 'z':
      out_string (pformat, prefix_len, human_time (get_stat_ctime (statbuf)));
      اكسر;
    حالة 'Z':
      out_epoch_sec (pformat, prefix_len, get_stat_ctime (statbuf));
      اكسر;
    حالة 'C':
      fail |= out_file_context (pformat, prefix_len, filename);
      اكسر;
    افتراضي:
      fputc ('?', stdout);
      اكسر;
    }
  החזר fail;
}

/* Return an allocated format string in static storage that
   corresponds to whether FS and TERSE options were declared.  */
ساكن حرف *
default_format (bool fs, bool terse, bool device)
{
  حرف *format;
  אם (fs)
    {
      אם (terse)
        format = xstrdup (fmt_terse_fs);
      אחרת
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
  אחרת /* ! fs */
    {
      אם (terse)
        {
          אם (0 < is_selinux_enabled ())
            format = xstrdup (fmt_terse_selinux);
          אחרת
            format = xstrdup (fmt_terse_regular);
        }
      אחרת
        {
          حرف *temp;
          /* TRANSLATORS: This string uses format specifiers from
             'stat --help' without --file-system, and NOT from printf.  */
          format = xstrdup (_("\
  File: %N\n\
  Size: %-10s\tBlocks: %-10b IO Block: %-6o %F\n\
"));

          temp = format;
          אם (device)
            {
              /* TRANSLATORS: This string uses format specifiers from
                 'stat --help' without --file-system, and NOT from printf.  */
              format = xasprintf ("%s%s", format, _("\
" "Device: %Hd,%Ld\tInode: %-10i  Links: %-5h Device type: %Hr,%Lr\n\
"));
            }
          אחרת
            {
              /* TRANSLATORS: This string uses format specifiers from
                 'stat --help' without --file-system, and NOT from printf.  */
              format = xasprintf ("%s%s", format, _("\
" "Device: %Hd,%Ld\tInode: %-10i  Links: %h\n\
"));
            }
          حرر (temp);

          temp = format;
          /* TRANSLATORS: This string uses format specifiers from
             'stat --help' without --file-system, and NOT from printf.  */
          format = xasprintf ("%s%s", format, _("\
" "Access: (%04a/%10.10A)  Uid: (%5u/%8U)   Gid: (%5g/%8G)\n\
"));
          حرر (temp);

          אם (0 < is_selinux_enabled ())
            {
              temp = format;
              /* TRANSLATORS: This string uses format specifiers from
                 'stat --help' without --file-system, and NOT from printf.  */
              format = xasprintf ("%s%s", format, _("Context: %C\n"));
              حرر (temp);
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
          حرر (temp);
        }
    }
  החזר format;
}

فراغ
usage (صحيح status)
{
  אם (status != EXIT_SUCCESS)
    emit_try_help ();
  אחרת
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
         use the specified FORMAT instead of the افتراضي;\n\
         output a newline after each use of FORMAT\n\
"));
      oputs (_("\
      --printf=FORMAT\n\
         like --format, but interpret backslash escapes,\n\
         and עשה not output a mandatory trailing newline;\n\
         אם you want a newline, include \\n in FORMAT\n\
"));
      oputs (_("\
  -t, --terse\n\
         print the information in terse form\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);

      fputs (_("\n\
The MODE argument of --cached can be: always, never, or افتراضي.\n\
'always' will use cached attributes אם available, כלעוד\n\
'never' will try to synchronize with the latest attributes, and\n\
'default' will leave it up to the underlying file system.\n\
"), stdout);

      fputs (_("\n\
The valid format sequences עבור files (without --file-system):\n\
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
  %N   quoted file name with dereference אם symbolic link\n\
  %o   optimal I/O transfer size hint\n\
  %s   total size, in bytes\n\
  %r   device type in decimal (st_rdev)\n\
  %R   device type in hex (st_rdev)\n\
  %Hr  major device type in decimal, עבור character/block device special files\n\
  %Lr  minor device type in decimal, עבור character/block device special files\n\
  %t   major device type in hex, עבור character/block device special files\n\
  %T   minor device type in hex, עבור character/block device special files\n\
"), stdout);
      fputs (_("\
  %u   user ID of owner\n\
  %U   user name of owner\n\
  %w   time of file birth, human-readable; - אם unknown\n\
  %W   time of file birth, seconds since Epoch; 0 אם unknown\n\
  %x   time of last access, human-readable\n\
  %X   time of last access, seconds since Epoch\n\
  %y   time of last data modification, human-readable\n\
  %Y   time of last data modification, seconds since Epoch\n\
  %z   time of last status change, human-readable\n\
  %Z   time of last status change, seconds since Epoch\n\
\n\
"), stdout);

      fputs (_("\
Valid format sequences עבור file systems:\n\
\n\
  %a   حرر blocks available to non-superuser\n\
  %b   total data blocks in file system\n\
  %c   total file nodes in file system\n\
  %d   حرر file nodes in file system\n\
  %f   حرر blocks in file system\n\
"), stdout);
      fputs (_("\
  %i   file system ID in hex\n\
  %l   maximum length of filenames\n\
  %n   file name\n\
  %Qn  quoted file name\n\
  %s   block size (עבור faster transfers)\n\
  %S   fundamental block size (עבור block counts)\n\
  %t   file system type in hex\n\
  %T   file system type in human readable form\n\
"), stdout);

      printf (_("\n\
--terse is equivalent to the following FORMAT:\n\
    %s\
"),
#אם HAVE_SELINUX_SELINUX_H
              fmt_terse_selinux
#אחרת
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

صحيح
main (صحيح argc, حرف *argv[])
{
  صحيح c;
  bool fs = false;
  bool terse = false;
  حرف *format = عدم;
  حرف *format2;
  bool ok = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  بنية lconv ثابت *locale = localeconv ();
  decimal_point = (locale->decimal_point[0] ? locale->decimal_point : ".");
  decimal_point_len = strlen (decimal_point);

  atexit (close_stdout);

  כלעוד ((c = getopt_long (argc, argv, "c:fLt", long_options, عدم)) != -1)
    {
      بدل (c)
        {
        حالة PRINTF_OPTION:
          format = optarg;
          interpret_backslash_escapes = true;
          trailing_delim = "";
          اكسر;

        حالة 'c':
          format = optarg;
          interpret_backslash_escapes = false;
          trailing_delim = "\n";
          اكسر;

        حالة 'L':
          follow_links = true;
          اكسر;

        حالة 'f':
          fs = true;
          اكسر;

        حالة 't':
          terse = true;
          اكسر;

        حالة 0:
          بدل (XARGMATCH ("--cached", optarg, cached_args, cached_modes))
            {
              حالة cached_never:
                force_sync = true;
                dont_sync = false;
                اكسر;
              حالة cached_always:
                force_sync = false;
                dont_sync = true;
                اكسر;
              حالة cached_default:
                force_sync = false;
                dont_sync = false;
            }
          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  אם (argc == optind)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  אם (format)
    {
      format2 = format;
    }
  אחרת
    {
      format = default_format (fs, terse, /* device= */ false);
      format2 = default_format (fs, terse, /* device= */ true);
    }

  עבור (صحيح i = optind; i < argc; i++)
    ok &= (fs
           ? do_statfs (argv[i], format)
           : do_stat (argv[i], format, format2));

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
