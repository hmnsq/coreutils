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
#إذا ((STAT_STATVFS || STAT_STATVFS64)                                       \
     && (HAVE_STRUCT_STATVFS_F_BASETYPE || HAVE_STRUCT_STATVFS_F_FSTYPENAME \
         || (! HAVE_STRUCT_STATFS_F_FSTYPENAME && HAVE_STRUCT_STATVFS_F_TYPE)))
# define USE_STATVFS 1
#وإلا
# define USE_STATVFS 0
#جيڪڏهن_پورو

#ضمن <stdio.h>
#ضمن <sys/types.h>
#ضمن <pwd.h>
#ضمن <grp.h>
#إذا USE_STATVFS
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
# إذا HAVE_NFS_NFS_CLNT_H && HAVE_NFS_VFS_H
/* Ultrix 4.4 needs these for the declaration of struct statfs.  */
#  include <netinet/in.h>
#  include <nfs/nfs_clnt.h>
#  include <nfs/vfs.h>
# endif
#وإلاإذا HAVE_OS_H /* BeOS */
# include <fs_info.h>
#جيڪڏهن_پورو
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

#إذا HAVE_STATX && defined STATX_INO
# define USE_STATX 1
#وإلا
# define USE_STATX 0
#جيڪڏهن_پورو

#إذا USE_STATVFS
# define STRUCT_STATXFS_F_FSID_IS_INTEGER STRUCT_STATVFS_F_FSID_IS_INTEGER
# define HAVE_STRUCT_STATXFS_F_TYPE HAVE_STRUCT_STATVFS_F_TYPE
# إذا HAVE_STRUCT_STATVFS_F_NAMEMAX
#  define SB_F_NAMEMAX(S) ((S)->f_namemax)
# endif
# إذا ! STAT_STATVFS && STAT_STATVFS64
#  define STRUCT_STATVFS بنية statvfs64
#  define STATFS statvfs64
# وإلا
#  define STRUCT_STATVFS بنية statvfs
#  define STATFS statvfs
# endif
# define STATFS_FRSIZE(S) ((S)->f_frsize)
#وإلا
# define HAVE_STRUCT_STATXFS_F_TYPE HAVE_STRUCT_STATFS_F_TYPE
# إذا HAVE_STRUCT_STATFS_F_NAMELEN
#  define SB_F_NAMEMAX(S) ((S)->f_namelen)
# elif HAVE_STRUCT_STATFS_F_NAMEMAX
#  define SB_F_NAMEMAX(S) ((S)->f_namemax)
# endif
# define STATFS statfs
# إذا HAVE_OS_H /* BeOS */
/* BeOS has a statvfs function, but it does not return sensible values
   for f_files, f_ffree and f_favail, and lacks f_type, f_basetype and
   f_fstypename.  Use 'struct fs_info' instead.  */
NODISCARD
ساكن صحيح
statfs (حرف ثابت *filename, بنية fs_info *buf)
{
  dev_t device = dev_for_رستو (filename);
  إذا (device < 0)
    {
      errno = (device == B_داخلا_NOT_مليو ? ENOENT
               : device == B_BAD_قدر ? EINVAL
               : device == B_NAME_TOO_LONG ? ENAMETOOLONG
               : device == B_NO_يادگيري ? ENOMEM
               : device == B_FILE_ERROR ? EIO
               : 0);
      أرجع -1;
    }
  /* If successful, buf->dev will be == device.  */
  أرجع fs_stat_dev (device, buf);
}
#  define f_fsid dev
#  define f_بلاڪون ڪل_بلاڪون
#  define f_bfree free_بلاڪون
#  define f_bavail free_بلاڪون
#  define f_bsize io_سائيز
#  define f_files ڪل_nodes
#  define f_ffree free_nodes
#  define STRUCT_STATVFS بنية fs_info
#  define STRUCT_STATXFS_F_FSID_IS_INTEGER true
#  define STATFS_FRSIZE(S) ((S)->بلاڪ_سائيز)
# وإلا
#  define STRUCT_STATVFS بنية statfs
#  define STRUCT_STATXFS_F_FSID_IS_INTEGER STRUCT_STATFS_F_FSID_IS_INTEGER
#  إذا HAVE_STRUCT_STATFS_F_FRSIZE
#   define STATFS_FRSIZE(S) ((S)->f_frsize)
#  وإلا
#   define STATFS_FRSIZE(S) 0
#  endif
# endif
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل SB_F_NAMEMAX
# define ٻاهريون_NAMEMAX ٻاهريون_uint
#وإلا
/* Depending on whether statvfs or statfs is used,
   neither f_namemax or f_namelen may be available.  */
# define SB_F_NAMEMAX(S) "?"
# define ٻاهريون_NAMEMAX ٻاهريون_string
#جيڪڏهن_پورو

#إذا HAVE_STRUCT_STATVFS_F_BASETYPE
# define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME f_basetype
#وإلا
# إذا HAVE_STRUCT_STATVFS_F_FSTYPENAME || HAVE_STRUCT_STATFS_F_FSTYPENAME
#  define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME f_fstypename
# elif HAVE_OS_H /* BeOS */
#  define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME fsh_name
# endif
#جيڪڏهن_پورو

#إذا HAVE_GETATTRAT
# include <attr.h>
# include <sys/nvpair.h>
#جيڪڏهن_پورو

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

#وصف_ڪريو پروگرام_NAME "stat"

#وصف_ڪريو AUTHORS proper_name ("Michael Meskes")

تعداد
{
  PRINTF_OPTION = CHAR_MAX + 1
};

تعداد cached_گهڻائي
{
  cached_default,
  cached_never,
  cached_always
};

ساكن حرف ثابت *ثابت cached_دليلون[] =
{
  "default", "never", "always", عدم
};

ساكن تعداد cached_گهڻائي ثابت cached_modes[] =
{
  cached_default, cached_never, cached_always
};

ساكن بنية option ثابت long_options[] =
{
  {"dereference", no_دليل, عدم, 'L'},
  {"file-system", no_دليل, عدم, 'f'},
  {"format", required_دليل, عدم, 'c'},
  {"printf", required_دليل, عدم, PRINTF_OPTION},
  {"terse", no_دليل, عدم, 't'},
  {"cached", required_دليل, عدم, 0},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخو_OPTION_DECL},
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
ساكن حرف ثابت *decimal_نقطو;
ساكن size_t decimal_نقطو_len;

ساكن bool
print_stat (حرف *pformat, size_t اڳياڙي_len, حرف mod, حرف m,
            صحيح fd, حرف ثابت *filename, فراغ ثابت *معلومات);

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
#جيڪڏهن_وصف_ٿيل STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME
  أرجع statfsbuf->STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME;
#وإلا
  بدل (statfsbuf->f_type)
    {
# إذا defined __linux__ || defined __ANDROID__

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
      أرجع "aafs";
    حالة S_MAGIC_ACFS: /* 0x61636673 remote */
      أرجع "acfs";
    حالة S_MAGIC_ADFS: /* 0xADF5 local */
      أرجع "adfs";
    حالة S_MAGIC_AFFS: /* 0xADFF local */
      أرجع "affs";
    حالة S_MAGIC_AFS: /* 0x5346414F remote */
      أرجع "afs";
    حالة S_MAGIC_ANON_INODE_FS: /* 0x09041934 local */
      أرجع "anon-inode FS";
    حالة S_MAGIC_AUFS: /* 0x61756673 remote */
      /* FIXME: change syntax or add an optional attribute like "inotify:no".
         The above is labeled as "remote" so that tail always uses polling,
         but this isn't really a remote file system type.  */
      أرجع "aufs";
    حالة S_MAGIC_AUTOFS: /* 0x0187 local */
      أرجع "autofs";
    حالة S_MAGIC_BALLOON_KVM: /* 0x13661366 local */
      أرجع "balloon-kvm-fs";
    حالة S_MAGIC_BCACHEFS: /* 0xCA451A4E local */
      أرجع "bcachefs";
    حالة S_MAGIC_BEFS: /* 0x42465331 local */
      أرجع "befs";
    حالة S_MAGIC_BDEVFS: /* 0x62646576 local */
      أرجع "bdevfs";
    حالة S_MAGIC_BFS: /* 0x1BADFACE local */
      أرجع "bfs";
    حالة S_MAGIC_BINDERFS: /* 0x6C6F6F70 local */
      أرجع "binderfs";
    حالة S_MAGIC_BPF_FS: /* 0xCAFE4A11 local */
      أرجع "bpf_fs";
    حالة S_MAGIC_BINFMTFS: /* 0x42494E4D local */
      أرجع "binfmt_misc";
    حالة S_MAGIC_BTRFS: /* 0x9123683E local */
      أرجع "btrfs";
    حالة S_MAGIC_BTRFS_TEST: /* 0x73727279 local */
      أرجع "btrfs_test";
    حالة S_MAGIC_CEPH: /* 0x00C36400 remote */
      أرجع "ceph";
    حالة S_MAGIC_CGROUP: /* 0x0027E0EB local */
      أرجع "cgroupfs";
    حالة S_MAGIC_CGROUP2: /* 0x63677270 local */
      أرجع "cgroup2fs";
    حالة S_MAGIC_CIFS: /* 0xFF534D42 remote */
      أرجع "cifs";
    حالة S_MAGIC_CODA: /* 0x73757245 remote */
      أرجع "coda";
    حالة S_MAGIC_COH: /* 0x012FF7B7 local */
      أرجع "coh";
    حالة S_MAGIC_CONFIGFS: /* 0x62656570 local */
      أرجع "configfs";
    حالة S_MAGIC_CRAMFS: /* 0x28CD3D45 local */
      أرجع "cramfs";
    حالة S_MAGIC_CRAMFS_WEND: /* 0x453DCD28 local */
      أرجع "cramfs-wend";
    حالة S_MAGIC_DAXFS: /* 0x64646178 local */
      أرجع "daxfs";
    حالة S_MAGIC_DEBUGFS: /* 0x64626720 local */
      أرجع "debugfs";
    حالة S_MAGIC_DEVFS: /* 0x1373 local */
      أرجع "devfs";
    حالة S_MAGIC_DEVMEM: /* 0x454D444D local */
      أرجع "devmem";
    حالة S_MAGIC_DEVPTS: /* 0x1CD1 local */
      أرجع "devpts";
    حالة S_MAGIC_DMA_BUF: /* 0x444D4142 local */
      أرجع "dma-buf-fs";
    حالة S_MAGIC_ECRYPTFS: /* 0xF15F local */
      أرجع "ecryptfs";
    حالة S_MAGIC_EFIVARFS: /* 0xDE5E81E4 local */
      أرجع "efivarfs";
    حالة S_MAGIC_EFS: /* 0x00414A53 local */
      أرجع "efs";
    حالة S_MAGIC_EROFS_V1: /* 0xE0F5E1E2 local */
      أرجع "erofs";
    حالة S_MAGIC_EXFAT: /* 0x2011BAB0 local */
      أرجع "exfat";
    حالة S_MAGIC_EXFS: /* 0x45584653 local */
      أرجع "exfs";
    حالة S_MAGIC_EXOFS: /* 0x5DF5 local */
      أرجع "exofs";
    حالة S_MAGIC_EXT: /* 0x137D local */
      أرجع "ext";
    حالة S_MAGIC_EXT2: /* 0xEF53 local */
      أرجع "ext2/ext3";
    حالة S_MAGIC_EXT2_OLD: /* 0xEF51 local */
      أرجع "ext2";
    حالة S_MAGIC_F2FS: /* 0xF2F52010 local */
      أرجع "f2fs";
    حالة S_MAGIC_FAIL_FS: /* 0x4641494C local */
      أرجع "failfs";
    حالة S_MAGIC_FAT: /* 0x4006 local */
      أرجع "fat";
    حالة S_MAGIC_FHGFS: /* 0x19830326 remote */
      أرجع "fhgfs";
    حالة S_MAGIC_FUSE: /* 0x65735546 remote */
      أرجع "fuse";
    حالة S_MAGIC_FUSECTL: /* 0x65735543 remote */
      أرجع "fusectl";
    حالة S_MAGIC_FUTEXFS: /* 0x0BAD1DEA local */
      أرجع "futexfs";
    حالة S_MAGIC_GFS: /* 0x01161970 remote */
      أرجع "gfs/gfs2";
    حالة S_MAGIC_GPFS: /* 0x47504653 remote */
      أرجع "gpfs";
    حالة S_MAGIC_GUEST_MEMFD: /* 0x474D454D remote */
      أرجع "guest-memfd";
    حالة S_MAGIC_HFS: /* 0x4244 local */
      أرجع "hfs";
    حالة S_MAGIC_HFS_PLUS: /* 0x482B local */
      أرجع "hfs+";
    حالة S_MAGIC_HFS_X: /* 0x4858 local */
      أرجع "hfsx";
    حالة S_MAGIC_HOSTFS: /* 0x00C0FFEE local */
      أرجع "hostfs";
    حالة S_MAGIC_HPFS: /* 0xF995E849 local */
      أرجع "hpfs";
    حالة S_MAGIC_HUGETLBFS: /* 0x958458F6 local */
      أرجع "hugetlbfs";
    حالة S_MAGIC_MTD_INODE_FS: /* 0x11307854 local */
      أرجع "inodefs";
    حالة S_MAGIC_IBRIX: /* 0x013111A8 remote */
      أرجع "ibrix";
    حالة S_MAGIC_INOTIFYFS: /* 0x2BAD1DEA local */
      أرجع "inotifyfs";
    حالة S_MAGIC_ISOFS: /* 0x9660 local */
      أرجع "isofs";
    حالة S_MAGIC_ISOFS_R_WIN: /* 0x4004 local */
      أرجع "isofs";
    حالة S_MAGIC_ISOFS_WIN: /* 0x4000 local */
      أرجع "isofs";
    حالة S_MAGIC_JFFS: /* 0x07C0 local */
      أرجع "jffs";
    حالة S_MAGIC_JFFS2: /* 0x72B6 local */
      أرجع "jffs2";
    حالة S_MAGIC_JFS: /* 0x3153464A local */
      أرجع "jfs";
    حالة S_MAGIC_KAFS: /* 0x6B414653 remote */
      أرجع "k-afs";
    حالة S_MAGIC_LOGFS: /* 0xC97E8168 local */
      أرجع "logfs";
    حالة S_MAGIC_LUSTRE: /* 0x0BD00BD0 remote */
      أرجع "lustre";
    حالة S_MAGIC_M1FS: /* 0x5346314D local */
      أرجع "m1fs";
    حالة S_MAGIC_MINIX: /* 0x137F local */
      أرجع "minix";
    حالة S_MAGIC_MINIX_30: /* 0x138F local */
      أرجع "minix (30 char.)";
    حالة S_MAGIC_MINIX_V2: /* 0x2468 local */
      أرجع "minix v2";
    حالة S_MAGIC_MINIX_V2_30: /* 0x2478 local */
      أرجع "minix v2 (30 char.)";
    حالة S_MAGIC_MINIX_V3: /* 0x4D5A local */
      أرجع "minix3";
    حالة S_MAGIC_MQUEUE: /* 0x19800202 local */
      أرجع "mqueue";
    حالة S_MAGIC_MSDOS: /* 0x4D44 local */
      أرجع "msdos";
    حالة S_MAGIC_NCP: /* 0x564C remote */
      أرجع "novell";
    حالة S_MAGIC_NFS: /* 0x6969 remote */
      أرجع "nfs";
    حالة S_MAGIC_NFSD: /* 0x6E667364 remote */
      أرجع "nfsd";
    حالة S_MAGIC_NILFS: /* 0x3434 local */
      أرجع "nilfs";
    حالة S_MAGIC_NSFS: /* 0x6E736673 local */
      أرجع "nsfs";
    حالة S_MAGIC_NTFS: /* 0x5346544E local */
      أرجع "ntfs";
    حالة S_MAGIC_NULL_FS: /* 0x4E554C4C local */
      أرجع "nullfs";
    حالة S_MAGIC_OPENPROM: /* 0x9FA1 local */
      أرجع "openprom";
    حالة S_MAGIC_OCFS2: /* 0x7461636F remote */
      أرجع "ocfs2";
    حالة S_MAGIC_OVERLAYFS: /* 0x794C7630 remote */
      /* This may overlay remote file systems.
         Also there have been issues reported with inotify and overlayfs,
         so mark as "remote" so that polling is used.  */
      أرجع "overlayfs";
    حالة S_MAGIC_PANFS: /* 0xAAD7AAEA remote */
      أرجع "panfs";
    حالة S_MAGIC_PID_FS: /* 0x50494446 local */
      أرجع "pidfs";
    حالة S_MAGIC_PIPEFS: /* 0x50495045 remote */
      /* FIXME: change syntax or add an optional attribute like "inotify:no".
         pipefs is labeled as "remote" so that tail always polls,
         but it isn't really remote file system type.  */
      أرجع "pipefs";
    حالة S_MAGIC_PPC_CMM: /* 0xC7571590 local */
      أرجع "ppc-cmm-fs";
    حالة S_MAGIC_PRL_FS: /* 0x7C7C6673 remote */
      أرجع "prl_fs";
    حالة S_MAGIC_PROC: /* 0x9FA0 local */
      أرجع "proc";
    حالة S_MAGIC_PSTOREFS: /* 0x6165676C local */
      أرجع "pstorefs";
    حالة S_MAGIC_QNX4: /* 0x002F local */
      أرجع "qnx4";
    حالة S_MAGIC_QNX6: /* 0x68191122 local */
      أرجع "qnx6";
    حالة S_MAGIC_RAMFS: /* 0x858458F6 local */
      أرجع "ramfs";
    حالة S_MAGIC_RDTGROUP: /* 0x07655821 local */
      أرجع "rdt";
    حالة S_MAGIC_REISERFS: /* 0x52654973 local */
      أرجع "reiserfs";
    حالة S_MAGIC_ROMFS: /* 0x7275 local */
      أرجع "romfs";
    حالة S_MAGIC_RPC_PIPEFS: /* 0x67596969 local */
      أرجع "rpc_pipefs";
    حالة S_MAGIC_SDCARDFS: /* 0x5DCA2DF5 local */
      أرجع "sdcardfs";
    حالة S_MAGIC_SECRETMEM: /* 0x5345434D local */
      أرجع "secretmem";
    حالة S_MAGIC_SECURITYFS: /* 0x73636673 local */
      أرجع "securityfs";
    حالة S_MAGIC_SELINUX: /* 0xF97CFF8C local */
      أرجع "selinux";
    حالة S_MAGIC_SMACK: /* 0x43415D53 local */
      أرجع "smackfs";
    حالة S_MAGIC_SMB: /* 0x517B remote */
      أرجع "smb";
    حالة S_MAGIC_SMB2: /* 0xFE534D42 remote */
      أرجع "smb2";
    حالة S_MAGIC_SNFS: /* 0xBEEFDEAD remote */
      أرجع "snfs";
    حالة S_MAGIC_SOCKFS: /* 0x534F434B local */
      أرجع "sockfs";
    حالة S_MAGIC_SQUASHFS: /* 0x73717368 local */
      أرجع "squashfs";
    حالة S_MAGIC_SYSFS: /* 0x62656572 local */
      أرجع "sysfs";
    حالة S_MAGIC_SYSV2: /* 0x012FF7B6 local */
      أرجع "sysv2";
    حالة S_MAGIC_SYSV4: /* 0x012FF7B5 local */
      أرجع "sysv4";
    حالة S_MAGIC_TMPFS: /* 0x01021994 local */
      أرجع "tmpfs";
    حالة S_MAGIC_TRACEFS: /* 0x74726163 local */
      أرجع "tracefs";
    حالة S_MAGIC_UBIFS: /* 0x24051905 local */
      أرجع "ubifs";
    حالة S_MAGIC_UDF: /* 0x15013346 local */
      أرجع "udf";
    حالة S_MAGIC_UFS: /* 0x00011954 local */
      أرجع "ufs";
    حالة S_MAGIC_UFS_BYTESWAPPED: /* 0x54190100 local */
      أرجع "ufs";
    حالة S_MAGIC_USBDEVFS: /* 0x9FA2 local */
      أرجع "usbdevfs";
    حالة S_MAGIC_V9FS: /* 0x01021997 local */
      أرجع "v9fs";
    حالة S_MAGIC_VBOXSF: /* 0x786F4256 remote */
      أرجع "vboxsf";
    حالة S_MAGIC_VMHGFS: /* 0xBACBACBC remote */
      أرجع "vmhgfs";
    حالة S_MAGIC_VXFS: /* 0xA501FCF5 remote */
      /* Veritas File System can run in single instance or clustered mode,
         so mark as remote to cater for the latter case.  */
      أرجع "vxfs";
    حالة S_MAGIC_VZFS: /* 0x565A4653 local */
      أرجع "vzfs";
    حالة S_MAGIC_WSLFS: /* 0x53464846 local */
      أرجع "wslfs";
    حالة S_MAGIC_XENFS: /* 0xABBA1974 local */
      أرجع "xenfs";
    حالة S_MAGIC_XENIX: /* 0x012FF7B4 local */
      أرجع "xenix";
    حالة S_MAGIC_XFS: /* 0x58465342 local */
      أرجع "xfs";
    حالة S_MAGIC_XIAFS: /* 0x012FD16D local */
      أرجع "xia";
    حالة S_MAGIC_Z3FOLD: /* 0x0033 local */
      أرجع "z3fold";
    حالة S_MAGIC_ZFS: /* 0x2FC12FC1 local */
      أرجع "zfs";
    حالة S_MAGIC_ZONEFS: /* 0x5A4F4653 local */
      أرجع "zonefs";
    حالة S_MAGIC_ZSMALLOC: /* 0x58295829 local */
      أرجع "zsmallocfs";


# elif __GNU__
    حالة FSTYPE_UFS:
      أرجع "ufs";
    حالة FSTYPE_NFS:
      أرجع "nfs";
    حالة FSTYPE_GFS:
      أرجع "gfs";
    حالة FSTYPE_LFS:
      أرجع "lfs";
    حالة FSTYPE_SYSV:
      أرجع "sysv";
    حالة FSTYPE_FTP:
      أرجع "ftp";
    حالة FSTYPE_TAR:
      أرجع "tar";
    حالة FSTYPE_AR:
      أرجع "ar";
    حالة FSTYPE_CPIO:
      أرجع "cpio";
    حالة FSTYPE_MSLOSS:
      أرجع "msloss";
    حالة FSTYPE_CPM:
      أرجع "cpm";
    حالة FSTYPE_HFS:
      أرجع "hfs";
    حالة FSTYPE_DTFS:
      أرجع "dtfs";
    حالة FSTYPE_GRFS:
      أرجع "grfs";
    حالة FSTYPE_رڪن:
      أرجع "term";
    حالة FSTYPE_DEV:
      أرجع "dev";
    حالة FSTYPE_PROC:
      أرجع "proc";
    حالة FSTYPE_IFSOCK:
      أرجع "ifsock";
    حالة FSTYPE_AFS:
      أرجع "afs";
    حالة FSTYPE_DFS:
      أرجع "dfs";
    حالة FSTYPE_PROC9:
      أرجع "proc9";
    حالة FSTYPE_SOCKET:
      أرجع "socket";
    حالة FSTYPE_MISC:
      أرجع "misc";
    حالة FSTYPE_EXT2FS:
      أرجع "ext2/ext3";
    حالة FSTYPE_HTTP:
      أرجع "http";
    حالة FSTYPE_MEMFS:
      أرجع "memfs";
    حالة FSTYPE_ISO9660:
      أرجع "iso9660";
# endif
    افتراضي:
      {
        بنانشان ڊگهو صحيح type = statfsbuf->f_type;
        ساكن حرف buf[حجم "UNKNOWN (0x%lx)" - 3
                        + (حجم type * CHAR_BIT + 3) / 4];
        sprintf (buf, "UNKNOWN (0x%lx)", type);
        أرجع buf;
      }
    }
#جيڪڏهن_پورو
}

NODISCARD
ساكن حرف *
human_access (بنية stat ثابت *statbuf)
{
  ساكن حرف modebuf[12];
  filemodestring (statbuf, modebuf);
  modebuf[10] = 0;
  أرجع modebuf;
}

NODISCARD
ساكن حرف *
human_وقت (بنية timespec t)
{
  /* STR must be at least INT_BUFSIZE_BOUND (intmax_t) big, either
     because localtime_rz fails, or because the time zone is truly
     outlandish so that %z expands to a long string.  */
  ساكن حرف str[INT_BUFSIZE_BOUND (intmax_t)
                  + INT_STRLEN_BOUND (صحيح) /* YYYY */
                  + 1 /* because YYYY might equal INT_MAX + 1900 */
                  + حجم "-MM-DD HH:MM:SS.NNNNNNNNN +"];
  ساكن timezone_t tz;
  إذا (!tz)
    tz = tzalloc (getenv ("TZ"));
  بنية tm tm;
  صحيح ns = t.tv_nsec;
  إذا (localtime_rz (tz, &t.tv_sec, &tm))
    nstrftime (str, حجم str, "%Y-%m-%d %H:%M:%S.%N %z", &tm, tz, ns);
  وإلا
    {
      حرف secbuf[INT_BUFSIZE_BOUND (intmax_t)];
      sprintf (str, "%s.%09d", timetostr (t.tv_sec, secbuf), ns);
    }
  أرجع str;
}

/* PFORMAT points to a '%' followed by a prefix of a format, all of
   size PREFIX_LEN.  The flags allowed for this format are
   ALLOWED_FLAGS; remove other printf flags from the prefix, then
   append SUFFIX.  */
ساكن فراغ
make_format (حرف *pformat, size_t اڳياڙي_len, حرف ثابت *allowed_flags,
             حرف ثابت *suffix)
{
  حرف *dst = pformat + 1;
  حرف ثابت *src;
  حرف ثابت *srclim = pformat + اڳياڙي_len;
  لكل (src = dst; src < srclim && strchr (printf_flags, *src); src++)
    إذا (strchr (allowed_flags, *src))
      *dst++ = *src;
  طالما (src < srclim)
    *dst++ = *src++;
  strcpy (dst, suffix);
}

ساكن فراغ
ٻاهريون_string (حرف *pformat, size_t اڳياڙي_len, حرف ثابت *arg)
{
  make_format (pformat, اڳياڙي_len, "-", "s");
  لکو (pformat, arg);
}
ساكن صحيح
ٻاهريون_int (حرف *pformat, size_t اڳياڙي_len, intmax_t arg)
{
  make_format (pformat, اڳياڙي_len, "'-+ 0", "jd");
  أرجع لکو (pformat, arg);
}
ساكن صحيح
ٻاهريون_uint (حرف *pformat, size_t اڳياڙي_len, uintmax_t arg)
{
  make_format (pformat, اڳياڙي_len, "'-0", "ju");
  أرجع لکو (pformat, arg);
}
ساكن فراغ
ٻاهريون_uint_o (حرف *pformat, size_t اڳياڙي_len, uintmax_t arg)
{
  make_format (pformat, اڳياڙي_len, "-#0", "jo");
  لکو (pformat, arg);
}
ساكن فراغ
ٻاهريون_uint_x (حرف *pformat, size_t اڳياڙي_len, uintmax_t arg)
{
  make_format (pformat, اڳياڙي_len, "-#0", "jx");
  لکو (pformat, arg);
}
ساكن صحيح
ٻاهريون_minus_zero (حرف *pformat, size_t اڳياڙي_len)
{
  make_format (pformat, اڳياڙي_len, "'-+ 0", ".0f");
  أرجع لکو (pformat, -0.25);
}

/* Output the number of seconds since the Epoch, using a format that
   acts like printf's %f format.  */
ساكن فراغ
ٻاهريون_epoch_sec (حرف *pformat, size_t اڳياڙي_len,
               بنية timespec arg)
{
  حرف *dot = memchr (pformat, '.', اڳياڙي_len);
  size_t sec_اڳياڙي_len = اڳياڙي_len;
  صحيح ويڪر = 0;
  صحيح precision = 0;
  bool frac_کاٻو_adjust = false;

  إذا (dot)
    {
      sec_اڳياڙي_len = dot - pformat;
      pformat[اڳياڙي_len] = '\0';

      إذا (c_isdigit (dot[1]))
        {
          ڊگهو صحيح lprec = strtol (dot + 1, عدم, 10);
          precision = (lprec <= وڌ_کان_وڌ_عدد ? lprec : وڌ_کان_وڌ_عدد);
        }
      وإلا
        {
          precision = 9;
        }

      إذا (precision && c_isdigit (dot[-1]))
        {
          /* If a nontrivial width is given, subtract the width of the
             decimal point and PRECISION digits that will be output
             later.  */
          حرف *p = dot;
          *dot = '\0';

          افعل
            --p;
          طالما (c_isdigit (p[-1]));

          ڊگهو صحيح lwidth = strtol (p, عدم, 10);
          ويڪر = (lwidth <= وڌ_کان_وڌ_عدد ? lwidth : وڌ_کان_وڌ_عدد);
          إذا (1 < ويڪر)
            {
              p += (*p == '0');
              sec_اڳياڙي_len = p - pformat;
              صحيح w_d = (decimal_نقطو_len < ويڪر
                         ? ويڪر - decimal_نقطو_len
                         : 0);
              إذا (1 < w_d)
                {
                  صحيح w = w_d - precision;
                  إذا (1 < w)
                    {
                      حرف *dst = pformat;
                      لكل (حرف ثابت *src = dst; src < p; src++)
                        {
                          إذا (*src == '-')
                            frac_کاٻو_adjust = true;
                          وإلا
                            *dst++ = *src;
                        }
                      sec_اڳياڙي_len =
                        (dst - pformat
                         + (frac_کاٻو_adjust ? 0 : sprintf (dst, "%d", w)));
                    }
                }
            }
        }
    }

  صحيح ورهائيندڙ = 1;
  لكل (صحيح i = precision; i < 9; i++)
    ورهائيندڙ *= 10;
  صحيح frac_sec = arg.tv_nsec / ورهائيندڙ;
  صحيح int_len;

  إذا (TYPE_SIGNED (time_t))
    {
      bool minus_zero = false;
      إذا (arg.tv_sec < 0 && arg.tv_nsec != 0)
        {
          صحيح frac_sec_modulus = 1000000000 / ورهائيندڙ;
          frac_sec = (frac_sec_modulus - frac_sec
                      - (arg.tv_nsec % ورهائيندڙ != 0));
          arg.tv_sec += (frac_sec != 0);
          minus_zero = (arg.tv_sec == 0);
        }
      int_len = (minus_zero
                 ? ٻاهريون_minus_zero (pformat, sec_اڳياڙي_len)
                 : ٻاهريون_int (pformat, sec_اڳياڙي_len, arg.tv_sec));
    }
  وإلا
    int_len = ٻاهريون_uint (pformat, sec_اڳياڙي_len, arg.tv_sec);

  إذا (precision)
    {
      صحيح prec = (precision < 9 ? precision : 9);
      صحيح trailing_prec = precision - prec;
      صحيح ilen = (int_len < 0 ? 0 : int_len);
      صحيح trailing_ويڪر = (ilen < ويڪر && decimal_نقطو_len < ويڪر - ilen
                            ? ويڪر - ilen - decimal_نقطو_len - prec
                            : 0);
      لکو ("%s%.*d%-*.*d", decimal_نقطو, prec, frac_sec,
              trailing_ويڪر, trailing_prec, 0);
    }
}

/* Print the context information of FILENAME, and return true iff the
   context could not be obtained.  */
NODISCARD
ساكن bool
ٻاهريون_file_context (حرف *pformat, size_t اڳياڙي_len, حرف ثابت *filename)
{
  حرف *scontext;
  bool fail = false;

  إذا ((follow_links
       ? getfilecon (filename, &scontext)
       : lgetfilecon (filename, &scontext)) < 0)
    {
      error (0, errno, _("failed to get security context of %s"),
             quoteaf (filename));
      scontext = عدم;
      fail = true;
    }
  strcpy (pformat + اڳياڙي_len, "s");
  لکو (pformat, (scontext ? scontext : "?"));
  إذا (scontext)
    freecon (scontext);
  أرجع fail;
}

/* Set the quoting style once it is needed.  */
ساكن فراغ
initialize_quoting_style (فراغ)
{
  ساكن bool initialized;
  إذا (!initialized)
    {
      initialized = true;
      صحيح qs = getenv_quoting_style ();
      سيٽ_quoting_style (عدم, 0 <= qs ? qs : shell_escape_quoting_style);
    }
}

/* Print statfs info.  Return zero upon success, nonzero upon failure.  */
NODISCARD
ساكن bool
print_statfs (حرف *pformat, size_t اڳياڙي_len, MAYBE_اڻ_استعمال_ٿيل حرف mod, حرف m,
              MAYBE_اڻ_استعمال_ٿيل صحيح fd, حرف ثابت *filename,
              فراغ ثابت *معلومات)
{
  STRUCT_STATVFS ثابت *statfsbuf = معلومات;
  bool fail = false;

  بدل (m)
    {
    حالة 'n':
      إذا (mod == 'Q')
        {
          initialize_quoting_style ();
          filename = quoteN (filename);
        }
      ٻاهريون_string (pformat, اڳياڙي_len, filename);
      اكسر;

    حالة 'i':
      {
#إذا STRUCT_STATXFS_F_FSID_IS_INTEGER
        uintmax_t fsid = statfsbuf->f_fsid;
#وإلا
        تعريفنوع بنانشان صحيح fsid_لفظ;
        static_assert (alignof (STRUCT_STATVFS) % alignof (fsid_لفظ) == 0);
        static_assert (offsetof (STRUCT_STATVFS, f_fsid) % alignof (fsid_لفظ)
                       == 0);
        static_assert (حجم statfsbuf->f_fsid % alignof (fsid_لفظ) == 0);
        fsid_لفظ ثابت *p = (fsid_لفظ *) &statfsbuf->f_fsid;

        /* Assume a little-endian word order, as that is compatible
           with glibc's statvfs implementation.  */
        uintmax_t fsid = 0;
        صحيح لفظ_4 = حجم statfsbuf->f_fsid / حجم *p;
        لكل (صحيح i = 0; i < لفظ_4 && i * حجم *p < حجم fsid; i++)
          {
            uintmax_t u = p[لفظ_4 - 1 - i];
            fsid |= u << (i * CHAR_BIT * حجم *p);
          }
#جيڪڏهن_پورو
        ٻاهريون_uint_x (pformat, اڳياڙي_len, fsid);
      }
      اكسر;

    حالة 'l':
      ٻاهريون_NAMEMAX (pformat, اڳياڙي_len, SB_F_NAMEMAX (statfsbuf));
      اكسر;
    حالة 't':
#إذا HAVE_STRUCT_STATXFS_F_TYPE
      ٻاهريون_uint_x (pformat, اڳياڙي_len, statfsbuf->f_type);
#وإلا
      fputc ('?', stdout);
#جيڪڏهن_پورو
      اكسر;
    حالة 'T':
      ٻاهريون_string (pformat, اڳياڙي_len, human_fstype (statfsbuf));
      اكسر;
    حالة 'b':
      ٻاهريون_int (pformat, اڳياڙي_len, statfsbuf->f_بلاڪون);
      اكسر;
    حالة 'f':
      ٻاهريون_int (pformat, اڳياڙي_len, statfsbuf->f_bfree);
      اكسر;
    حالة 'a':
      ٻاهريون_int (pformat, اڳياڙي_len, statfsbuf->f_bavail);
      اكسر;
    حالة 's':
      ٻاهريون_uint (pformat, اڳياڙي_len, statfsbuf->f_bsize);
      اكسر;
    حالة 'S':
      {
        uintmax_t frsize = STATFS_FRSIZE (statfsbuf);
        إذا (! frsize)
          frsize = statfsbuf->f_bsize;
        ٻاهريون_uint (pformat, اڳياڙي_len, frsize);
      }
      اكسر;
    حالة 'c':
      ٻاهريون_uint (pformat, اڳياڙي_len, statfsbuf->f_files);
      اكسر;
    حالة 'd':
      ٻاهريون_int (pformat, اڳياڙي_len, statfsbuf->f_ffree);
      اكسر;
    افتراضي:
      fputc ('?', stdout);
      اكسر;
    }
  أرجع fail;
}

/* Return any bind mounted source for a path.
   The caller should not free the returned buffer.
   Return NULL if no bind mount found.  */
NODISCARD
ساكن حرف ثابت *
ڳوليو_bind_mount (حرف ثابت * name)
{
  حرف ثابت * bind_mount = عدم;

  ساكن بنية mount_داخلا *mount_list;
  ساكن bool tried_mount_list = false;
  إذا (!tried_mount_list) /* attempt/warn once per process.  */
    {
      إذا (!(mount_list = read_file_system_list (false)))
        error (0, errno, "%s", _("cannot read table of mounted file systems"));
      tried_mount_list = true;
    }

  بنية stat name_stats;
  إذا (stat (name, &name_stats) != 0)
    أرجع عدم;

  لكل (بنية mount_داخلا *me = mount_list; me; me = me->me_ايندڙ)
    {
      إذا (me->me_dummy && me->me_devname[0] == '/'
          && streq (me->me_mountdir, name))
        {
          بنية stat dev_stats;

          إذا (stat (me->me_devname, &dev_stats) == 0
              && psame_inode (&name_stats, &dev_stats))
            {
              bind_mount = me->me_devname;
              اكسر;
            }
        }
    }

  أرجع bind_mount;
}

/* Print mount point.  Return zero upon success, nonzero upon failure.  */
NODISCARD
ساكن bool
ٻاهريون_mount_نقطو (حرف ثابت *filename, حرف *pformat, size_t اڳياڙي_len,
                 ثابت بنية stat *statp)
{

  حرف ثابت *np = "?", *bp = عدم;
  حرف *mp = عدم;
  bool fail = true;

  /* Look for bind mounts first.  Note we output the immediate alias,
     rather than further resolving to a base device mount point.  */
  إذا (follow_links || !S_ISLNK (statp->st_گهڻائي))
    {
      حرف *resolved = canonicalize_file_name (filename);
      إذا (!resolved)
        {
          error (0, errno, _("failed to canonicalize %s"), quoteaf (filename));
          اذهب print_mount_نقطو;
        }
      bp = ڳوليو_bind_mount (resolved);
      حرر (resolved);
      إذا (bp)
        {
          fail = false;
          اذهب print_mount_نقطو;
        }
    }

  /* If there is no direct bind mount, then navigate
     back up the tree looking for a device change.
     Note we don't detect if any of the directory components
     are bind mounted to the same device, but that's OK
     since we've not directly queried them.  */
  إذا ((mp = ڳوليو_mount_نقطو (filename, statp)))
    {
      /* This dir might be bind mounted to another device,
         so we resolve the bound source in that case also.  */
      bp = ڳوليو_bind_mount (mp);
      fail = false;
    }

print_mount_نقطو:

  ٻاهريون_string (pformat, اڳياڙي_len, bp ? bp : mp ? mp : np);
  حرر (mp);
  أرجع fail;
}

/* Map a TS with negative TS.tv_nsec to {0,0}.  */
ساكن مضمن بنية timespec
neg_منزل_zero (بنية timespec ts)
{
  إذا (0 <= ts.tv_nsec)
    أرجع ts;
  بنية timespec z = {0};
  أرجع z;
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
  اکر_لکو (c);
}

ATTRIBUTE_خالص
ساكن size_t
format_ڪوڊ_وٿي (حرف ثابت *directive)
{
  size_t len = strspn (directive + 1, printf_flags);
  حرف ثابت *fmt_char = directive + len + 1;
  fmt_char += strspn (fmt_char, digits);
  إذا (*fmt_char == '.')
    fmt_char += 1 + strspn (fmt_char + 1, digits);
  أرجع fmt_char - directive;
}

/* Print the information specified by the format string, FORMAT,
   calling PRINT_FUNC for each %-directive encountered.
   Return zero upon success, nonzero upon failure.  */
NODISCARD
ساكن bool
print_it (حرف ثابت *format, صحيح fd, حرف ثابت *filename,
          bool (*print_func) (حرف *, size_t, حرف, حرف,
                              صحيح, حرف ثابت *, فراغ ثابت *),
          فراغ ثابت *معلومات)
{
  bool fail = false;

  /* Add 2 to accommodate our conversion of the stat '%s' format string
     to the longer printf '%llu' one.  */
  تعداد
    {
      MAX_ADDITIONAL_بائيٽون =
        (MAX (حجم "jd",
              MAX (حجم "jo", MAX (حجم "ju", حجم "jx")))
         - 1)
    };
  size_t n_alloc = لڙي_ڊيگهه (format) + MAX_ADDITIONAL_بائيٽون + 1;
  حرف *dest = xmalloc (n_alloc);
  لكل (حرف ثابت *b = format; *b; b++)
    {
      بدل (*b)
        {
        حالة '%':
          {
            size_t len = format_ڪوڊ_وٿي (b);
            حرف fmt_char = *(b + len);
            حرف mod_char = 0;
            يادگيري_نقل (dest, b, len);
            b += len;

            بدل (fmt_char)
              {
              حالة '\0':
                --b;
                FALLTHROUGH;
              حالة '%':
                إذا (1 < len)
                  {
                    dest[len] = fmt_char;
                    dest[len + 1] = '\0';
                    error (EXIT_FAILURE, 0, _("%s: invalid directive"),
                           quote (dest));
                  }
                اکر_لکو ('%');
                اكسر;
              حالة 'H':
              حالة 'L':
              حالة 'Q':
                mod_char = fmt_char;
                fmt_char = *(b + 1);
                إذا ((mod_char == 'Q' && fmt_char == 'n')
                    || (print_func == print_stat
                        && (mod_char == 'H' || mod_char == 'L')
                        && (fmt_char == 'd' || fmt_char == 'r')))
                  {
                    b++;
                  }
                وإلا
                  {
                    fmt_char = mod_char;
                    mod_char = 0;
                  }
                FALLTHROUGH;
              افتراضي:
                fail |= print_func (dest, len, mod_char, fmt_char,
                                    fd, filename, معلومات);
                اكسر;
              }
            اكسر;
          }

        حالة '\\':
          إذا ( ! interpret_backslash_escapes)
            {
              اکر_لکو ('\\');
              اكسر;
            }
          ++b;
          إذا (isoct (*b))
            {
              صحيح esc_قدر = fromoct (*b);
              صحيح esc_ڊيگهائي = 1;	/* number of octal digits */
              لكل (++b; esc_ڊيگهائي < 3 && isoct (*b);
                   ++esc_ڊيگهائي, ++b)
                {
                  esc_قدر = esc_قدر * 8 + fromoct (*b);
                }
              اکر_لکو (esc_قدر);
              --b;
            }
          وإلا إذا (*b == 'x' && c_isxdigit (b[1]))
            {
              صحيح esc_قدر = fromhex (b[1]);	/* Value of \xhh escape. */
              /* A hexadecimal \xhh escape sequence must have
                 1 or 2 hex. digits.  */
              ++b;
              إذا (c_isxdigit (b[1]))
                {
                  ++b;
                  esc_قدر = esc_قدر * 16 + fromhex (*b);
                }
              اکر_لکو (esc_قدر);
            }
          وإلا إذا (*b == '\0')
            {
              error (0, 0, _("warning: backslash at end of format"));
              اکر_لکو ('\\');
              /* Arrange to exit the loop.  */
              --b;
            }
          وإلا
            {
              print_esc_char (*b);
            }
          اكسر;

        افتراضي:
          اکر_لکو (*b);
          اكسر;
        }
    }
  حرر (dest);

  fputs (trailing_delim, stdout);

  أرجع fail;
}

/* Stat the file system and print what we find.  */
NODISCARD
ساكن bool
do_statfs (حرف ثابت *filename, حرف ثابت *format)
{
  STRUCT_STATVFS statfsbuf;

  إذا (streq (filename, "-"))
    {
      error (0, 0, _("using %s to denote standard input does not work"
                     " in file system mode"), quoteaf (filename));
      أرجع false;
    }

  إذا (STATFS (filename, &statfsbuf) != 0)
    {
      error (0, errno, _("cannot read file system information for %s"),
             quoteaf (filename));
      أرجع false;
    }

  bool fail = print_it (format, -1, filename, print_statfs, &statfsbuf);
  أرجع ! fail;
}

بنية print_دليلون {
  بنية stat *st;
  بنية timespec btime;
};

/* Ask statx to avoid syncing? */
ساكن bool dont_sync;

/* Ask statx to force sync? */
ساكن bool force_sync;

#إذا USE_STATX
ساكن بنانشان صحيح
fmt_منزل_نقاب (حرف fmt)
{
  بدل (fmt)
    {
    حالة 'N':
      أرجع STATX_گهڻائي;
    حالة 'd':
    حالة 'D':
      أرجع STATX_گهڻائي;
    حالة 'i':
      أرجع STATX_INO;
    حالة 'a':
    حالة 'A':
      أرجع STATX_گهڻائي;
    حالة 'f':
      أرجع STATX_گهڻائي|STATX_TYPE;
    حالة 'F':
      أرجع STATX_TYPE;
    حالة 'h':
      أرجع STATX_NLINK;
    حالة 'u':
    حالة 'U':
      أرجع STATX_UID;
    حالة 'g':
    حالة 'G':
      أرجع STATX_GID;
    حالة 'm':
      أرجع STATX_گهڻائي|STATX_INO;
    حالة 's':
      أرجع STATX_سائيز;
    حالة 't':
    حالة 'T':
      أرجع STATX_گهڻائي;
    حالة 'b':
      أرجع STATX_بلاڪون;
    حالة 'w':
    حالة 'W':
      أرجع STATX_BTIME;
    حالة 'x':
    حالة 'X':
      أرجع STATX_ATIME;
    حالة 'y':
    حالة 'Y':
      أرجع STATX_MTIME;
    حالة 'z':
    حالة 'Z':
      أرجع STATX_CTIME;
    }
  أرجع 0;
}

ATTRIBUTE_خالص
ساكن بنانشان صحيح
format_منزل_نقاب (حرف ثابت *format)
{
  بنانشان صحيح نقاب = 0;

  لكل (حرف ثابت *b = format; *b; b++)
    {
      إذا (*b != '%')
        استمر;

      b += format_ڪوڊ_وٿي (b);
      إذا (*b == '\0')
        اكسر;
      نقاب |= fmt_منزل_نقاب (*b);
    }
  أرجع نقاب;
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
  بنية print_دليلون pa;
  pa.st = &st;
  pa.btime = (بنية timespec) {.tv_sec = -1, .tv_nsec = -1};

  إذا (AT_FDCWD != fd)
    {
      pathname = "";
      flags = AT_خالي_جايون_رستو;
    }
  وإلا إذا (!follow_links)
    {
      flags = AT_SYMLINK_NOFOLLOW;
    }

  إذا (dont_sync)
    flags |= AT_STATX_DONT_SYNC;
  وإلا إذا (force_sync)
    flags |= AT_STATX_FORCE_SYNC;

  إذا (! force_sync)
    flags |= AT_NO_AUTOMOUNT;

  fd = statx (fd, pathname, flags, format_منزل_نقاب (format), &stx);
  إذا (fd < 0)
    {
      إذا (flags & AT_خالي_جايون_رستو)
        error (0, errno, _("cannot stat standard input"));
      وإلا
        error (0, errno, _("cannot statx %s"), quoteaf (filename));
      أرجع false;
    }

  إذا (S_ISBLK (stx.stx_گهڻائي) || S_ISCHR (stx.stx_گهڻائي))
    format = format2;

  statx_منزل_stat (&stx, &st);
  إذا (stx.stx_نقاب & STATX_BTIME)
    pa.btime = statx_timestamp_منزل_timespec (stx.stx_btime);

  bool fail = print_it (format, fd, filename, print_stat, &pa);
  أرجع ! fail;
}

#وإلا /* USE_STATX */

ساكن بنية timespec
get_birthtime (صحيح fd, حرف ثابت *filename, بنية stat ثابت *st)
{
  بنية timespec ts = get_stat_birthtime (st);

# إذا HAVE_GETATTRAT
  إذا (ts.tv_nsec < 0)
    {
      nvlist_t *response;
      إذا ((fd < 0
           ? getattrat (AT_FDCWD, XATTR_VIEW_READWRITE, filename, &response)
           : fgetattr (fd, XATTR_VIEW_READWRITE, &response))
          == 0)
        {
          uint64_t *val;
          uint_t n;
          إذا (nvlist_lookup_uint64_array (response, A_CRTIME, &val, &n) == 0
              && 2 <= n
              && val[0] <= TYPE_وڌ_کان_وڌ (time_t)
              && val[1] < 1000000000 * 2 /* for leap seconds */)
            {
              ts.tv_sec = val[0];
              ts.tv_nsec = val[1];
            }
          nvlist_free (response);
        }
    }
# endif

  أرجع ts;
}


/* stat the file and print what we find */
NODISCARD
ساكن bool
do_stat (حرف ثابت *filename, حرف ثابت *format,
         حرف ثابت *format2)
{
  صحيح fd = streq (filename, "-") ? 0 : -1;
  بنية stat statbuf;
  بنية print_دليلون pa;
  pa.st = &statbuf;
  pa.btime = (بنية timespec) {.tv_sec = -1, .tv_nsec = -1};

  إذا (0 <= fd)
    {
      إذا (fstat (fd, &statbuf) != 0)
        {
          error (0, errno, _("cannot stat standard input"));
          أرجع false;
        }
    }
  /* We can't use the shorter
     (follow_links?stat:lstat) (filename, &statbug)
     since stat might be a function-like macro.  */
  وإلا إذا ((follow_links
            ? stat (filename, &statbuf)
            : lstat (filename, &statbuf)) != 0)
    {
      error (0, errno, _("cannot stat %s"), quoteaf (filename));
      أرجع false;
    }

  إذا (S_ISBLK (statbuf.st_گهڻائي) || S_ISCHR (statbuf.st_گهڻائي))
    format = format2;

  bool fail = print_it (format, fd, filename, print_stat, &pa);
  أرجع ! fail;
}
#جيڪڏهن_پورو /* USE_STATX */

/* POSIX requires 'ls' to print file sizes without a sign, even
   when negative.  Be consistent with that.  */

ساكن uintmax_t
unsigned_file_سائيز (off_t سائيز)
{
  أرجع سائيز + (سائيز < 0) * ((uintmax_t) OFF_T_MAX - OFF_T_MIN + 1);
}

/* Print stat info.  Return zero upon success, nonzero upon failure.  */
ساكن bool
print_stat (حرف *pformat, size_t اڳياڙي_len, حرف mod, حرف m,
            MAYBE_اڻ_استعمال_ٿيل صحيح fd, حرف ثابت *filename, فراغ ثابت *معلومات)
{
  بنية print_دليلون *parg = (بنية print_دليلون *) معلومات;
  بنية stat *statbuf = parg->st;
  بنية timespec btime = parg->btime;
  بنية passwd *pw_ent;
  بنية گروهه *gw_ent;
  bool fail = false;

  بدل (m)
    {
    حالة 'n':
      إذا (mod == 'Q')
        {
          initialize_quoting_style ();
          filename = quoteN (filename);
        }
      ٻاهريون_string (pformat, اڳياڙي_len, filename);
      اكسر;
    حالة 'N':
      initialize_quoting_style ();
      ٻاهريون_string (pformat, اڳياڙي_len, quoteN (filename));
      إذا (S_ISLNK (statbuf->st_گهڻائي))
        {
          حرف *linkname = areadlink_with_سائيز (filename, statbuf->st_سائيز);
          إذا (linkname == عدم)
            {
              error (0, errno, _("cannot read symbolic link %s"),
                     quoteaf (filename));
              أرجع true;
            }
          لکو (" -> ");
          ٻاهريون_string (pformat, اڳياڙي_len, quoteN (linkname));
          حرر (linkname);
        }
      اكسر;
    حالة 'd':
      إذا (mod == 'H')
        ٻاهريون_uint (pformat, اڳياڙي_len, major (statbuf->st_dev));
      وإلا إذا (mod == 'L')
        ٻاهريون_uint (pformat, اڳياڙي_len, minor (statbuf->st_dev));
      وإلا
        ٻاهريون_uint (pformat, اڳياڙي_len, statbuf->st_dev);
      اكسر;
    حالة 'D':
      ٻاهريون_uint_x (pformat, اڳياڙي_len, statbuf->st_dev);
      اكسر;
    حالة 'i':
      ٻاهريون_uint (pformat, اڳياڙي_len, statbuf->st_ino);
      اكسر;
    حالة 'a':
      ٻاهريون_uint_o (pformat, اڳياڙي_len, statbuf->st_گهڻائي & CHMOD_گهڻائي_بٽ);
      اكسر;
    حالة 'A':
      ٻاهريون_string (pformat, اڳياڙي_len, human_access (statbuf));
      اكسر;
    حالة 'f':
      ٻاهريون_uint_x (pformat, اڳياڙي_len, statbuf->st_گهڻائي);
      اكسر;
    حالة 'F':
      ٻاهريون_string (pformat, اڳياڙي_len, file_type (statbuf));
      اكسر;
    حالة 'h':
      ٻاهريون_uint (pformat, اڳياڙي_len, statbuf->st_nlink);
      اكسر;
    حالة 'u':
      ٻاهريون_uint (pformat, اڳياڙي_len, statbuf->st_uid);
      اكسر;
    حالة 'U':
      pw_ent = getpwuid (statbuf->st_uid);
      ٻاهريون_string (pformat, اڳياڙي_len,
                  pw_ent ? pw_ent->pw_name : "UNKNOWN");
      اكسر;
    حالة 'g':
      ٻاهريون_uint (pformat, اڳياڙي_len, statbuf->st_gid);
      اكسر;
    حالة 'G':
      gw_ent = getgrgid (statbuf->st_gid);
      ٻاهريون_string (pformat, اڳياڙي_len,
                  gw_ent ? gw_ent->gr_name : "UNKNOWN");
      اكسر;
    حالة 'm':
      fail |= ٻاهريون_mount_نقطو (filename, pformat, اڳياڙي_len, statbuf);
      اكسر;
    حالة 's':
      ٻاهريون_uint (pformat, اڳياڙي_len, unsigned_file_سائيز (statbuf->st_سائيز));
      اكسر;
    حالة 'r':
      إذا (mod == 'H')
        ٻاهريون_uint (pformat, اڳياڙي_len, major (statbuf->st_rdev));
      وإلا إذا (mod == 'L')
        ٻاهريون_uint (pformat, اڳياڙي_len, minor (statbuf->st_rdev));
      وإلا
        ٻاهريون_uint (pformat, اڳياڙي_len, statbuf->st_rdev);
      اكسر;
    حالة 'R':
      ٻاهريون_uint_x (pformat, اڳياڙي_len, statbuf->st_rdev);
      اكسر;
    حالة 't':
      ٻاهريون_uint_x (pformat, اڳياڙي_len, major (statbuf->st_rdev));
      اكسر;
    حالة 'T':
      ٻاهريون_uint_x (pformat, اڳياڙي_len, minor (statbuf->st_rdev));
      اكسر;
    حالة 'B':
      ٻاهريون_uint (pformat, اڳياڙي_len, ST_NBLOCKSIZE);
      اكسر;
    حالة 'b':
      ٻاهريون_uint (pformat, اڳياڙي_len, STP_NBLOCKS (statbuf));
      اكسر;
    حالة 'o':
      ٻاهريون_uint (pformat, اڳياڙي_len, STP_BLKSIZE (statbuf));
      اكسر;
    حالة 'w':
      {
#إذا ! USE_STATX
        btime = get_birthtime (fd, filename, statbuf);
#جيڪڏهن_پورو
        إذا (btime.tv_nsec < 0)
          ٻاهريون_string (pformat, اڳياڙي_len, "-");
        وإلا
          ٻاهريون_string (pformat, اڳياڙي_len, human_وقت (btime));
      }
      اكسر;
    حالة 'W':
      {
#إذا ! USE_STATX
        btime = get_birthtime (fd, filename, statbuf);
#جيڪڏهن_پورو
        ٻاهريون_epoch_sec (pformat, اڳياڙي_len, neg_منزل_zero (btime));
      }
      اكسر;
    حالة 'x':
      ٻاهريون_string (pformat, اڳياڙي_len, human_وقت (get_stat_atime (statbuf)));
      اكسر;
    حالة 'X':
      ٻاهريون_epoch_sec (pformat, اڳياڙي_len, get_stat_atime (statbuf));
      اكسر;
    حالة 'y':
      ٻاهريون_string (pformat, اڳياڙي_len, human_وقت (get_stat_mtime (statbuf)));
      اكسر;
    حالة 'Y':
      ٻاهريون_epoch_sec (pformat, اڳياڙي_len, get_stat_mtime (statbuf));
      اكسر;
    حالة 'z':
      ٻاهريون_string (pformat, اڳياڙي_len, human_وقت (get_stat_ctime (statbuf)));
      اكسر;
    حالة 'Z':
      ٻاهريون_epoch_sec (pformat, اڳياڙي_len, get_stat_ctime (statbuf));
      اكسر;
    حالة 'C':
      fail |= ٻاهريون_file_context (pformat, اڳياڙي_len, filename);
      اكسر;
    افتراضي:
      fputc ('?', stdout);
      اكسر;
    }
  أرجع fail;
}

/* Return an allocated format string in static storage that
   corresponds to whether FS and TERSE options were declared.  */
ساكن حرف *
default_format (bool fs, bool terse, bool device)
{
  حرف *format;
  إذا (fs)
    {
      إذا (terse)
        format = xstrdup (fmt_terse_fs);
      وإلا
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
  وإلا /* ! fs */
    {
      إذا (terse)
        {
          إذا (0 < is_selinux_enabled ())
            format = xstrdup (fmt_terse_selinux);
          وإلا
            format = xstrdup (fmt_terse_regular);
        }
      وإلا
        {
          حرف *عارضي;
          /* TRANSLATORS: This string uses format specifiers from
             'stat --help' without --file-system, and NOT from printf.  */
          format = xstrdup (_("\
  File: %N\n\
  سائيز_3: %-10s\t_بلاڪون: %-10b IO بلاڪ_3: %-6o %F\n\
"));

          عارضي = format;
          إذا (device)
            {
              /* TRANSLATORS: This string uses format specifiers from
                 'stat --help' without --file-system, and NOT from printf.  */
              format = xasprintf ("%s%s", format, _("\
" "Device: %Hd,%Ld\tInode: %-10i  Links: %-5h Device type: %Hr,%Lr\n\
"));
            }
          وإلا
            {
              /* TRANSLATORS: This string uses format specifiers from
                 'stat --help' without --file-system, and NOT from printf.  */
              format = xasprintf ("%s%s", format, _("\
" "Device: %Hd,%Ld\tInode: %-10i  Links: %h\n\
"));
            }
          حرر (عارضي);

          عارضي = format;
          /* TRANSLATORS: This string uses format specifiers from
             'stat --help' without --file-system, and NOT from printf.  */
          format = xasprintf ("%s%s", format, _("\
" "Access: (%04a/%10.10A)  Uid: (%5u/%8U)   Gid: (%5g/%8G)\n\
"));
          حرر (عارضي);

          إذا (0 < is_selinux_enabled ())
            {
              عارضي = format;
              /* TRANSLATORS: This string uses format specifiers from
                 'stat --help' without --file-system, and NOT from printf.  */
              format = xasprintf ("%s%s", format, _("Context: %C\n"));
              حرر (عارضي);
            }

          عارضي = format;
          format = xasprintf ("%s%s", format,
                              /* TRANSLATORS: This string uses format specifiers
                                 from 'stat --help' without --file-system, and
                                 NOT from printf.  */
                              _("Access: %x\n"
                                "Modify: %y\n"
                                "Change: %z\n"
                                " Birth: %w\n"));
          حرر (عارضي);
        }
    }
  أرجع format;
}

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکو (_("Usage: %s [OPTION]... FILE...\n"), پروگرام_name);
      fputs (_("\
Display file or file system حالت.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -L, --dereference\n\
         follow links\n\
"));
      oputs (_("\
  -f, --file-system\n\
         display file system حالت instead جو file حالت\n\
"));
      oputs (_("\
      --cached=گهڻائي\n\
         specify how منزل use cached خاصيتون;\n\
         useful on remote file systems. See گهڻائي هيٺ\n\
"));
      oputs (_("\
  -c, --format=FORMAT\n\
         use the specified FORMAT instead جو the افتراضي;\n\
         output a newline after each use جو FORMAT\n\
"));
      oputs (_("\
      --لکو=FORMAT\n\
         like --format, but interpret backslash escapes,\n\
         and افعل not output a mandatory trailing newline;\n\
         إذا you want a newline, include \\n اندريون FORMAT\n\
"));
      oputs (_("\
  -t, --terse\n\
         print the information اندريون terse form\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخو_OPTION_DESCRIPTION);

      fputs (_("\n\
The گهڻائي دليل جو --cached can be: always, never, or افتراضي.\n\
'always' will use cached خاصيتون إذا موجود, طالما\n\
'never' will try منزل synchronize with the latest خاصيتون, and\n\
'default' will leave it مٿي منزل the underlying file system.\n\
"), stdout);

      fputs (_("\n\
The valid format sequences لكل files (without --file-system):\n\
\n\
  %a   permission بٽ_2 اندريون octal (see '#' and '0' لکو flags)\n\
  %A   permission بٽ_2 and file type اندريون human readable form\n\
  %b   عدد جو بلاڪون allocated (see %B)\n\
  %B   the سائيز اندريون بائيٽون جو each بلاڪ reported by %b\n\
  %C   SELinux security context string\n\
"), stdout);
      fputs (_("\
  %d   device عدد اندريون decimal (st_dev)\n\
  %D   device عدد اندريون hex (st_dev)\n\
  %Hd  major device عدد اندريون decimal\n\
  %Ld  minor device عدد اندريون decimal\n\
  %f   raw گهڻائي_2 اندريون hex\n\
  %F   file type\n\
  %g   گروهه ID جو owner\n\
  %G   گروهه name جو owner\n\
"), stdout);
      fputs (_("\
  %h   عدد جو hard links\n\
  %i   inode عدد\n\
  %m   mount نقطو\n\
  %n   file name\n\
  %Qn  quoted file name\n\
  %N   quoted file name with dereference إذا symbolic link\n\
  %o   optimal I/O transfer سائيز hint\n\
  %s   ڪل سائيز, اندريون بائيٽون\n\
  %r   device type اندريون decimal (st_rdev)\n\
  %R   device type اندريون hex (st_rdev)\n\
  %Hr  major device type اندريون decimal, لكل character/بلاڪ device special files\n\
  %Lr  minor device type اندريون decimal, لكل character/بلاڪ device special files\n\
  %t   major device type اندريون hex, لكل character/بلاڪ device special files\n\
  %T   minor device type اندريون hex, لكل character/بلاڪ device special files\n\
"), stdout);
      fputs (_("\
  %u   user ID جو owner\n\
  %U   user name جو owner\n\
  %w   وقت جو file birth, human-readable; - إذا unknown\n\
  %W   وقت جو file birth, seconds since Epoch; 0 إذا unknown\n\
  %x   وقت جو آخري access, human-readable\n\
  %X   وقت جو آخري access, seconds since Epoch\n\
  %y   وقت جو آخري معلومات modification, human-readable\n\
  %Y   وقت جو آخري معلومات modification, seconds since Epoch\n\
  %z   وقت جو آخري حالت change, human-readable\n\
  %Z   وقت جو آخري حالت change, seconds since Epoch\n\
\n\
"), stdout);

      fputs (_("\
Valid format sequences لكل file systems:\n\
\n\
  %a   حرر بلاڪون موجود منزل non-superuser\n\
  %b   ڪل معلومات بلاڪون اندريون file system\n\
  %c   ڪل file nodes اندريون file system\n\
  %d   حرر file nodes اندريون file system\n\
  %f   حرر بلاڪون اندريون file system\n\
"), stdout);
      fputs (_("\
  %i   file system ID اندريون hex\n\
  %l   وڌ_کان_وڌ ڊيگهائي جو filenames\n\
  %n   file name\n\
  %Qn  quoted file name\n\
  %s   بلاڪ سائيز (لكل faster transfers)\n\
  %S   fundamental بلاڪ سائيز (لكل بلاڪ counts)\n\
  %t   file system type اندريون hex\n\
  %T   file system type اندريون human readable form\n\
"), stdout);

      لکو (_("\n\
--terse is equivalent منزل the following FORMAT:\n\
    %s\
"),
#إذا HAVE_SELINUX_SELINUX_H
              fmt_terse_selinux
#وإلا
              fmt_terse_regular
#جيڪڏهن_پورو
              );

        لکو (_("\
--terse --file-system is equivalent منزل the following FORMAT:\n\
    %s\
"), fmt_terse_fs);

      لکو (USAGE_BUILTIN_WARNING, پروگرام_NAME);
      emit_ancillary_info (پروگرام_NAME);
    }
  ٻاهر_نڪرو (حالت);
}

صحيح
مکيه (صحيح argc, حرف *argv[])
{
  صحيح c;
  bool fs = false;
  bool terse = false;
  حرف *format = عدم;
  حرف *format2;
  bool ok = true;

  initialize_main (&argc, &argv);
  سيٽ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  بنية lconv ثابت *locale = localeconv ();
  decimal_نقطو = (locale->decimal_نقطو[0] ? locale->decimal_نقطو : ".");
  decimal_نقطو_len = لڙي_ڊيگهه (decimal_نقطو);

  atexit (close_stdout);

  طالما ((c = getopt_long (argc, argv, "c:fLt", long_options, عدم)) != -1)
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
          بدل (XARGMATCH ("--cached", optarg, cached_دليلون, cached_modes))
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

        case_GETOPT_نسخو_CHAR (پروگرام_NAME, AUTHORS);

        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  إذا (argc == optind)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  إذا (format)
    {
      format2 = format;
    }
  وإلا
    {
      format = default_format (fs, terse, /* device= */ false);
      format2 = default_format (fs, terse, /* device= */ true);
    }

  لكل (صحيح i = optind; i < argc; i++)
    ok &= (fs
           ? do_statfs (argv[i], format)
           : do_stat (argv[i], format, format2));

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
