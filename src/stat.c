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
#ورنہ
# define USE_STATVFS 0
#اگر_ختم

#شامل <stdio.h>
#شامل <sys/types.h>
#شامل <pwd.h>
#شامل <grp.h>
#اگر USE_STATVFS
# include <sys/statvfs.h>
#وگرنہ_اگر HAVE_SYS_VFS_H
# include <sys/vfs.h>
#وگرنہ_اگر HAVE_SYS_MOUNT_H && HAVE_SYS_PARAM_H
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
#وگرنہ_اگر HAVE_OS_H /* BeOS */
# include <fs_info.h>
#اگر_ختم
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
#ورنہ
# define USE_STATX 0
#اگر_ختم

#اگر USE_STATVFS
# define STRUCT_STATXFS_F_FSID_IS_INTEGER STRUCT_STATVFS_F_FSID_IS_INTEGER
# define HAVE_STRUCT_STATXFS_F_TYPE HAVE_STRUCT_STATVFS_F_TYPE
# اگر HAVE_STRUCT_STATVFS_F_NAMEMAX
#  define SB_F_NAMEMAX(S) ((S)->f_namemax)
# endif
# اگر ! STAT_STATVFS && STAT_STATVFS64
#  define STRUCT_STATVFS ساخت statvfs64
#  define STATFS statvfs64
# ورنہ
#  define STRUCT_STATVFS ساخت statvfs
#  define STATFS statvfs
# endif
# define STATFS_FRSIZE(S) ((S)->f_frsize)
#ورنہ
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
ساکن صحیح
statfs (حرف ثابت *filename, ساخت fs_info *buf)
{
  dev_t device = dev_for_راستہ (filename);
  اگر (device < 0)
    {
      errno = (device == B_اندراج_NOT_ملا ? ENOENT
               : device == B_BAD_قدر ? EINVAL
               : device == B_NAME_TOO_LONG ? ENAMETOOLONG
               : device == B_NO_حافظہ ? ENOMEM
               : device == B_FILE_ERROR ? EIO
               : 0);
      واپس -1;
    }
  /* If successful, buf->dev will be == device.  */
  واپس fs_stat_dev (device, buf);
}
#  define f_fsid dev
#  define f_بلاکیں کل_بلاکیں
#  define f_bfree free_بلاکیں
#  define f_bavail free_بلاکیں
#  define f_bsize io_سائز
#  define f_files کل_nodes
#  define f_ffree free_nodes
#  define STRUCT_STATVFS ساخت fs_info
#  define STRUCT_STATXFS_F_FSID_IS_INTEGER true
#  define STATFS_FRSIZE(S) ((S)->بلاک_سائز)
# ورنہ
#  define STRUCT_STATVFS ساخت statfs
#  define STRUCT_STATXFS_F_FSID_IS_INTEGER STRUCT_STATFS_F_FSID_IS_INTEGER
#  اگر HAVE_STRUCT_STATFS_F_FRSIZE
#   define STATFS_FRSIZE(S) ((S)->f_frsize)
#  ورنہ
#   define STATFS_FRSIZE(S) 0
#  endif
# endif
#اگر_ختم

#اگر_تعریف_شدہ SB_F_NAMEMAX
# define برآمد_NAMEMAX برآمد_uint
#ورنہ
/* Depending on whether statvfs or statfs is used,
   neither f_namemax or f_namelen may be available.  */
# define SB_F_NAMEMAX(S) "?"
# define برآمد_NAMEMAX برآمد_string
#اگر_ختم

#اگر HAVE_STRUCT_STATVFS_F_BASETYPE
# define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME f_basetype
#ورنہ
# اگر HAVE_STRUCT_STATVFS_F_FSTYPENAME || HAVE_STRUCT_STATFS_F_FSTYPENAME
#  define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME f_fstypename
# elif HAVE_OS_H /* BeOS */
#  define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME fsh_name
# endif
#اگر_ختم

#اگر HAVE_GETATTRAT
# include <attr.h>
# include <sys/nvpair.h>
#اگر_ختم

ساکن حرف ثابت digits[] = "0123456789";

/* Flags that are portable for use in printf, for at least one
   conversion specifier; make_format removes non-portable flags as
   needed for particular specifiers.  The glibc 2.2 extension "I" is
   listed here; it is removed by make_format because it has undefined
   behavior elsewhere and because it is incompatible with
   out_epoch_sec.  */
ساکن حرف ثابت printf_flags[] = "'-+ #0I";

/* Formats for the --terse option.  */
ساکن حرف ثابت fmt_terse_fs[] = "%Qn %i %l %t %s %S %b %f %a %c %d\n";
ساکن حرف ثابت fmt_terse_regular[] = "%Qn %s %b %f %u %g %D %i %h %t %T"
                                        " %X %Y %Z %W %o\n";
ساکن حرف ثابت fmt_terse_selinux[] = "%Qn %s %b %f %u %g %D %i %h %t %T"
                                        " %X %Y %Z %W %o %C\n";

#تعریف_کرو پروگرام_NAME "stat"

#تعریف_کرو AUTHORS proper_name ("Michael Meskes")

شمار
{
  PRINTF_OPTION = CHAR_MAX + 1
};

شمار cached_منوال
{
  cached_default,
  cached_never,
  cached_always
};

ساکن حرف ثابت *ثابت cached_دلائل[] =
{
  "default", "never", "always", تہی
};

ساکن شمار cached_منوال ثابت cached_modes[] =
{
  cached_default, cached_never, cached_always
};

ساکن ساخت option ثابت long_options[] =
{
  {"dereference", no_دلیل, تہی, 'L'},
  {"file-system", no_دلیل, تہی, 'f'},
  {"format", required_دلیل, تہی, 'c'},
  {"printf", required_دلیل, تہی, PRINTF_OPTION},
  {"terse", no_دلیل, تہی, 't'},
  {"cached", required_دلیل, تہی, 0},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0}
};

/* Whether to follow symbolic links;  True for --dereference (-L).  */
ساکن bool follow_links;

/* Whether to interpret backslash-escape sequences.
   True for --printf=FMT, not for --format=FMT (-c).  */
ساکن bool interpret_backslash_escapes;

/* The trailing delimiter string:
   "" for --printf=FMT, "\n" for --format=FMT (-c).  */
ساکن حرف ثابت *trailing_delim = "";

/* The representation of the decimal point in the current locale.  */
ساکن حرف ثابت *decimal_نقطہ;
ساکن size_t decimal_نقطہ_len;

ساکن bool
print_stat (حرف *pformat, size_t سابقہ_len, حرف mod, حرف m,
            صحیح fd, حرف ثابت *filename, خالی ثابت *ڈیٹا);

/* Return the type of the specified file system.
   Some systems have statfvs.f_basetype[FSTYPSZ] (AIX, HP-UX, and Solaris).
   Others have statvfs.f_fstypename[_VFS_NAMELEN] (NetBSD 3.0).
   Others have statfs.f_fstypename[MFSNAMELEN] (NetBSD 1.5.2).
   Still others have neither and have to get by with f_type (GNU/Linux).
   But f_type may only exist in statfs (Cygwin).  */
NODISCARD
ساکن حرف ثابت *
human_fstype (STRUCT_STATVFS ثابت *statfsbuf)
{
#اگر_تعریف_شدہ STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME
  واپس statfsbuf->STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME;
#ورنہ
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
      واپس "aafs";
    حالت S_MAGIC_ACFS: /* 0x61636673 remote */
      واپس "acfs";
    حالت S_MAGIC_ADFS: /* 0xADF5 local */
      واپس "adfs";
    حالت S_MAGIC_AFFS: /* 0xADFF local */
      واپس "affs";
    حالت S_MAGIC_AFS: /* 0x5346414F remote */
      واپس "afs";
    حالت S_MAGIC_ANON_INODE_FS: /* 0x09041934 local */
      واپس "anon-inode FS";
    حالت S_MAGIC_AUFS: /* 0x61756673 remote */
      /* FIXME: change syntax or add an optional attribute like "inotify:no".
         The above is labeled as "remote" so that tail always uses polling,
         but this isn't really a remote file system type.  */
      واپس "aufs";
    حالت S_MAGIC_AUTOFS: /* 0x0187 local */
      واپس "autofs";
    حالت S_MAGIC_BALLOON_KVM: /* 0x13661366 local */
      واپس "balloon-kvm-fs";
    حالت S_MAGIC_BCACHEFS: /* 0xCA451A4E local */
      واپس "bcachefs";
    حالت S_MAGIC_BEFS: /* 0x42465331 local */
      واپس "befs";
    حالت S_MAGIC_BDEVFS: /* 0x62646576 local */
      واپس "bdevfs";
    حالت S_MAGIC_BFS: /* 0x1BADFACE local */
      واپس "bfs";
    حالت S_MAGIC_BINDERFS: /* 0x6C6F6F70 local */
      واپس "binderfs";
    حالت S_MAGIC_BPF_FS: /* 0xCAFE4A11 local */
      واپس "bpf_fs";
    حالت S_MAGIC_BINFMTFS: /* 0x42494E4D local */
      واپس "binfmt_misc";
    حالت S_MAGIC_BTRFS: /* 0x9123683E local */
      واپس "btrfs";
    حالت S_MAGIC_BTRFS_TEST: /* 0x73727279 local */
      واپس "btrfs_test";
    حالت S_MAGIC_CEPH: /* 0x00C36400 remote */
      واپس "ceph";
    حالت S_MAGIC_CGROUP: /* 0x0027E0EB local */
      واپس "cgroupfs";
    حالت S_MAGIC_CGROUP2: /* 0x63677270 local */
      واپس "cgroup2fs";
    حالت S_MAGIC_CIFS: /* 0xFF534D42 remote */
      واپس "cifs";
    حالت S_MAGIC_CODA: /* 0x73757245 remote */
      واپس "coda";
    حالت S_MAGIC_COH: /* 0x012FF7B7 local */
      واپس "coh";
    حالت S_MAGIC_CONFIGFS: /* 0x62656570 local */
      واپس "configfs";
    حالت S_MAGIC_CRAMFS: /* 0x28CD3D45 local */
      واپس "cramfs";
    حالت S_MAGIC_CRAMFS_WEND: /* 0x453DCD28 local */
      واپس "cramfs-wend";
    حالت S_MAGIC_DAXFS: /* 0x64646178 local */
      واپس "daxfs";
    حالت S_MAGIC_DEBUGFS: /* 0x64626720 local */
      واپس "debugfs";
    حالت S_MAGIC_DEVFS: /* 0x1373 local */
      واپس "devfs";
    حالت S_MAGIC_DEVMEM: /* 0x454D444D local */
      واپس "devmem";
    حالت S_MAGIC_DEVPTS: /* 0x1CD1 local */
      واپس "devpts";
    حالت S_MAGIC_DMA_BUF: /* 0x444D4142 local */
      واپس "dma-buf-fs";
    حالت S_MAGIC_ECRYPTFS: /* 0xF15F local */
      واپس "ecryptfs";
    حالت S_MAGIC_EFIVARFS: /* 0xDE5E81E4 local */
      واپس "efivarfs";
    حالت S_MAGIC_EFS: /* 0x00414A53 local */
      واپس "efs";
    حالت S_MAGIC_EROFS_V1: /* 0xE0F5E1E2 local */
      واپس "erofs";
    حالت S_MAGIC_EXFAT: /* 0x2011BAB0 local */
      واپس "exfat";
    حالت S_MAGIC_EXFS: /* 0x45584653 local */
      واپس "exfs";
    حالت S_MAGIC_EXOFS: /* 0x5DF5 local */
      واپس "exofs";
    حالت S_MAGIC_EXT: /* 0x137D local */
      واپس "ext";
    حالت S_MAGIC_EXT2: /* 0xEF53 local */
      واپس "ext2/ext3";
    حالت S_MAGIC_EXT2_OLD: /* 0xEF51 local */
      واپس "ext2";
    حالت S_MAGIC_F2FS: /* 0xF2F52010 local */
      واپس "f2fs";
    حالت S_MAGIC_FAIL_FS: /* 0x4641494C local */
      واپس "failfs";
    حالت S_MAGIC_FAT: /* 0x4006 local */
      واپس "fat";
    حالت S_MAGIC_FHGFS: /* 0x19830326 remote */
      واپس "fhgfs";
    حالت S_MAGIC_FUSE: /* 0x65735546 remote */
      واپس "fuse";
    حالت S_MAGIC_FUSECTL: /* 0x65735543 remote */
      واپس "fusectl";
    حالت S_MAGIC_FUTEXFS: /* 0x0BAD1DEA local */
      واپس "futexfs";
    حالت S_MAGIC_GFS: /* 0x01161970 remote */
      واپس "gfs/gfs2";
    حالت S_MAGIC_GPFS: /* 0x47504653 remote */
      واپس "gpfs";
    حالت S_MAGIC_GUEST_MEMFD: /* 0x474D454D remote */
      واپس "guest-memfd";
    حالت S_MAGIC_HFS: /* 0x4244 local */
      واپس "hfs";
    حالت S_MAGIC_HFS_PLUS: /* 0x482B local */
      واپس "hfs+";
    حالت S_MAGIC_HFS_X: /* 0x4858 local */
      واپس "hfsx";
    حالت S_MAGIC_HOSTFS: /* 0x00C0FFEE local */
      واپس "hostfs";
    حالت S_MAGIC_HPFS: /* 0xF995E849 local */
      واپس "hpfs";
    حالت S_MAGIC_HUGETLBFS: /* 0x958458F6 local */
      واپس "hugetlbfs";
    حالت S_MAGIC_MTD_INODE_FS: /* 0x11307854 local */
      واپس "inodefs";
    حالت S_MAGIC_IBRIX: /* 0x013111A8 remote */
      واپس "ibrix";
    حالت S_MAGIC_INOTIFYFS: /* 0x2BAD1DEA local */
      واپس "inotifyfs";
    حالت S_MAGIC_ISOFS: /* 0x9660 local */
      واپس "isofs";
    حالت S_MAGIC_ISOFS_R_WIN: /* 0x4004 local */
      واپس "isofs";
    حالت S_MAGIC_ISOFS_WIN: /* 0x4000 local */
      واپس "isofs";
    حالت S_MAGIC_JFFS: /* 0x07C0 local */
      واپس "jffs";
    حالت S_MAGIC_JFFS2: /* 0x72B6 local */
      واپس "jffs2";
    حالت S_MAGIC_JFS: /* 0x3153464A local */
      واپس "jfs";
    حالت S_MAGIC_KAFS: /* 0x6B414653 remote */
      واپس "k-afs";
    حالت S_MAGIC_LOGFS: /* 0xC97E8168 local */
      واپس "logfs";
    حالت S_MAGIC_LUSTRE: /* 0x0BD00BD0 remote */
      واپس "lustre";
    حالت S_MAGIC_M1FS: /* 0x5346314D local */
      واپس "m1fs";
    حالت S_MAGIC_MINIX: /* 0x137F local */
      واپس "minix";
    حالت S_MAGIC_MINIX_30: /* 0x138F local */
      واپس "minix (30 char.)";
    حالت S_MAGIC_MINIX_V2: /* 0x2468 local */
      واپس "minix v2";
    حالت S_MAGIC_MINIX_V2_30: /* 0x2478 local */
      واپس "minix v2 (30 char.)";
    حالت S_MAGIC_MINIX_V3: /* 0x4D5A local */
      واپس "minix3";
    حالت S_MAGIC_MQUEUE: /* 0x19800202 local */
      واپس "mqueue";
    حالت S_MAGIC_MSDOS: /* 0x4D44 local */
      واپس "msdos";
    حالت S_MAGIC_NCP: /* 0x564C remote */
      واپس "novell";
    حالت S_MAGIC_NFS: /* 0x6969 remote */
      واپس "nfs";
    حالت S_MAGIC_NFSD: /* 0x6E667364 remote */
      واپس "nfsd";
    حالت S_MAGIC_NILFS: /* 0x3434 local */
      واپس "nilfs";
    حالت S_MAGIC_NSFS: /* 0x6E736673 local */
      واپس "nsfs";
    حالت S_MAGIC_NTFS: /* 0x5346544E local */
      واپس "ntfs";
    حالت S_MAGIC_NULL_FS: /* 0x4E554C4C local */
      واپس "nullfs";
    حالت S_MAGIC_OPENPROM: /* 0x9FA1 local */
      واپس "openprom";
    حالت S_MAGIC_OCFS2: /* 0x7461636F remote */
      واپس "ocfs2";
    حالت S_MAGIC_OVERLAYFS: /* 0x794C7630 remote */
      /* This may overlay remote file systems.
         Also there have been issues reported with inotify and overlayfs,
         so mark as "remote" so that polling is used.  */
      واپس "overlayfs";
    حالت S_MAGIC_PANFS: /* 0xAAD7AAEA remote */
      واپس "panfs";
    حالت S_MAGIC_PID_FS: /* 0x50494446 local */
      واپس "pidfs";
    حالت S_MAGIC_PIPEFS: /* 0x50495045 remote */
      /* FIXME: change syntax or add an optional attribute like "inotify:no".
         pipefs is labeled as "remote" so that tail always polls,
         but it isn't really remote file system type.  */
      واپس "pipefs";
    حالت S_MAGIC_PPC_CMM: /* 0xC7571590 local */
      واپس "ppc-cmm-fs";
    حالت S_MAGIC_PRL_FS: /* 0x7C7C6673 remote */
      واپس "prl_fs";
    حالت S_MAGIC_PROC: /* 0x9FA0 local */
      واپس "proc";
    حالت S_MAGIC_PSTOREFS: /* 0x6165676C local */
      واپس "pstorefs";
    حالت S_MAGIC_QNX4: /* 0x002F local */
      واپس "qnx4";
    حالت S_MAGIC_QNX6: /* 0x68191122 local */
      واپس "qnx6";
    حالت S_MAGIC_RAMFS: /* 0x858458F6 local */
      واپس "ramfs";
    حالت S_MAGIC_RDTGROUP: /* 0x07655821 local */
      واپس "rdt";
    حالت S_MAGIC_REISERFS: /* 0x52654973 local */
      واپس "reiserfs";
    حالت S_MAGIC_ROMFS: /* 0x7275 local */
      واپس "romfs";
    حالت S_MAGIC_RPC_PIPEFS: /* 0x67596969 local */
      واپس "rpc_pipefs";
    حالت S_MAGIC_SDCARDFS: /* 0x5DCA2DF5 local */
      واپس "sdcardfs";
    حالت S_MAGIC_SECRETMEM: /* 0x5345434D local */
      واپس "secretmem";
    حالت S_MAGIC_SECURITYFS: /* 0x73636673 local */
      واپس "securityfs";
    حالت S_MAGIC_SELINUX: /* 0xF97CFF8C local */
      واپس "selinux";
    حالت S_MAGIC_SMACK: /* 0x43415D53 local */
      واپس "smackfs";
    حالت S_MAGIC_SMB: /* 0x517B remote */
      واپس "smb";
    حالت S_MAGIC_SMB2: /* 0xFE534D42 remote */
      واپس "smb2";
    حالت S_MAGIC_SNFS: /* 0xBEEFDEAD remote */
      واپس "snfs";
    حالت S_MAGIC_SOCKFS: /* 0x534F434B local */
      واپس "sockfs";
    حالت S_MAGIC_SQUASHFS: /* 0x73717368 local */
      واپس "squashfs";
    حالت S_MAGIC_SYSFS: /* 0x62656572 local */
      واپس "sysfs";
    حالت S_MAGIC_SYSV2: /* 0x012FF7B6 local */
      واپس "sysv2";
    حالت S_MAGIC_SYSV4: /* 0x012FF7B5 local */
      واپس "sysv4";
    حالت S_MAGIC_TMPFS: /* 0x01021994 local */
      واپس "tmpfs";
    حالت S_MAGIC_TRACEFS: /* 0x74726163 local */
      واپس "tracefs";
    حالت S_MAGIC_UBIFS: /* 0x24051905 local */
      واپس "ubifs";
    حالت S_MAGIC_UDF: /* 0x15013346 local */
      واپس "udf";
    حالت S_MAGIC_UFS: /* 0x00011954 local */
      واپس "ufs";
    حالت S_MAGIC_UFS_BYTESWAPPED: /* 0x54190100 local */
      واپس "ufs";
    حالت S_MAGIC_USBDEVFS: /* 0x9FA2 local */
      واپس "usbdevfs";
    حالت S_MAGIC_V9FS: /* 0x01021997 local */
      واپس "v9fs";
    حالت S_MAGIC_VBOXSF: /* 0x786F4256 remote */
      واپس "vboxsf";
    حالت S_MAGIC_VMHGFS: /* 0xBACBACBC remote */
      واپس "vmhgfs";
    حالت S_MAGIC_VXFS: /* 0xA501FCF5 remote */
      /* Veritas File System can run in single instance or clustered mode,
         so mark as remote to cater for the latter case.  */
      واپس "vxfs";
    حالت S_MAGIC_VZFS: /* 0x565A4653 local */
      واپس "vzfs";
    حالت S_MAGIC_WSLFS: /* 0x53464846 local */
      واپس "wslfs";
    حالت S_MAGIC_XENFS: /* 0xABBA1974 local */
      واپس "xenfs";
    حالت S_MAGIC_XENIX: /* 0x012FF7B4 local */
      واپس "xenix";
    حالت S_MAGIC_XFS: /* 0x58465342 local */
      واپس "xfs";
    حالت S_MAGIC_XIAFS: /* 0x012FD16D local */
      واپس "xia";
    حالت S_MAGIC_Z3FOLD: /* 0x0033 local */
      واپس "z3fold";
    حالت S_MAGIC_ZFS: /* 0x2FC12FC1 local */
      واپس "zfs";
    حالت S_MAGIC_ZONEFS: /* 0x5A4F4653 local */
      واپس "zonefs";
    حالت S_MAGIC_ZSMALLOC: /* 0x58295829 local */
      واپس "zsmallocfs";


# elif __GNU__
    حالت FSTYPE_UFS:
      واپس "ufs";
    حالت FSTYPE_NFS:
      واپس "nfs";
    حالت FSTYPE_GFS:
      واپس "gfs";
    حالت FSTYPE_LFS:
      واپس "lfs";
    حالت FSTYPE_SYSV:
      واپس "sysv";
    حالت FSTYPE_FTP:
      واپس "ftp";
    حالت FSTYPE_TAR:
      واپس "tar";
    حالت FSTYPE_AR:
      واپس "ar";
    حالت FSTYPE_CPIO:
      واپس "cpio";
    حالت FSTYPE_MSLOSS:
      واپس "msloss";
    حالت FSTYPE_CPM:
      واپس "cpm";
    حالت FSTYPE_HFS:
      واپس "hfs";
    حالت FSTYPE_DTFS:
      واپس "dtfs";
    حالت FSTYPE_GRFS:
      واپس "grfs";
    حالت FSTYPE_رکن:
      واپس "term";
    حالت FSTYPE_DEV:
      واپس "dev";
    حالت FSTYPE_PROC:
      واپس "proc";
    حالت FSTYPE_IFSOCK:
      واپس "ifsock";
    حالت FSTYPE_AFS:
      واپس "afs";
    حالت FSTYPE_DFS:
      واپس "dfs";
    حالت FSTYPE_PROC9:
      واپس "proc9";
    حالت FSTYPE_SOCKET:
      واپس "socket";
    حالت FSTYPE_MISC:
      واپس "misc";
    حالت FSTYPE_EXT2FS:
      واپس "ext2/ext3";
    حالت FSTYPE_HTTP:
      واپس "http";
    حالت FSTYPE_MEMFS:
      واپس "memfs";
    حالت FSTYPE_ISO9660:
      واپس "iso9660";
# endif
    طےشدہ:
      {
        غیرعلامتی طویل صحیح type = statfsbuf->f_type;
        ساکن حرف buf[حجم "UNKNOWN (0x%lx)" - 3
                        + (حجم type * CHAR_BIT + 3) / 4];
        sprintf (buf, "UNKNOWN (0x%lx)", type);
        واپس buf;
      }
    }
#اگر_ختم
}

NODISCARD
ساکن حرف *
human_access (ساخت stat ثابت *statbuf)
{
  ساکن حرف modebuf[12];
  filemodestring (statbuf, modebuf);
  modebuf[10] = 0;
  واپس modebuf;
}

NODISCARD
ساکن حرف *
human_وقت (ساخت timespec t)
{
  /* STR must be at least INT_BUFSIZE_BOUND (intmax_t) big, either
     because localtime_rz fails, or because the time zone is truly
     outlandish so that %z expands to a long string.  */
  ساکن حرف str[INT_BUFSIZE_BOUND (intmax_t)
                  + INT_STRLEN_BOUND (صحیح) /* YYYY */
                  + 1 /* because YYYY might equal INT_MAX + 1900 */
                  + حجم "-MM-DD HH:MM:SS.NNNNNNNNN +"];
  ساکن timezone_t tz;
  اگر (!tz)
    tz = tzalloc (getenv ("TZ"));
  ساخت tm tm;
  صحیح ns = t.tv_nsec;
  اگر (localtime_rz (tz, &t.tv_sec, &tm))
    nstrftime (str, حجم str, "%Y-%m-%d %H:%M:%S.%N %z", &tm, tz, ns);
  ورنہ
    {
      حرف secbuf[INT_BUFSIZE_BOUND (intmax_t)];
      sprintf (str, "%s.%09d", timetostr (t.tv_sec, secbuf), ns);
    }
  واپس str;
}

/* PFORMAT points to a '%' followed by a prefix of a format, all of
   size PREFIX_LEN.  The flags allowed for this format are
   ALLOWED_FLAGS; remove other printf flags from the prefix, then
   append SUFFIX.  */
ساکن خالی
make_format (حرف *pformat, size_t سابقہ_len, حرف ثابت *allowed_flags,
             حرف ثابت *suffix)
{
  حرف *dst = pformat + 1;
  حرف ثابت *src;
  حرف ثابت *srclim = pformat + سابقہ_len;
  برائے (src = dst; src < srclim && strchr (printf_flags, *src); src++)
    اگر (strchr (allowed_flags, *src))
      *dst++ = *src;
  جب (src < srclim)
    *dst++ = *src++;
  strcpy (dst, suffix);
}

ساکن خالی
برآمد_string (حرف *pformat, size_t سابقہ_len, حرف ثابت *arg)
{
  make_format (pformat, سابقہ_len, "-", "s");
  لکھو (pformat, arg);
}
ساکن صحیح
برآمد_int (حرف *pformat, size_t سابقہ_len, intmax_t arg)
{
  make_format (pformat, سابقہ_len, "'-+ 0", "jd");
  واپس لکھو (pformat, arg);
}
ساکن صحیح
برآمد_uint (حرف *pformat, size_t سابقہ_len, uintmax_t arg)
{
  make_format (pformat, سابقہ_len, "'-0", "ju");
  واپس لکھو (pformat, arg);
}
ساکن خالی
برآمد_uint_o (حرف *pformat, size_t سابقہ_len, uintmax_t arg)
{
  make_format (pformat, سابقہ_len, "-#0", "jo");
  لکھو (pformat, arg);
}
ساکن خالی
برآمد_uint_x (حرف *pformat, size_t سابقہ_len, uintmax_t arg)
{
  make_format (pformat, سابقہ_len, "-#0", "jx");
  لکھو (pformat, arg);
}
ساکن صحیح
برآمد_minus_zero (حرف *pformat, size_t سابقہ_len)
{
  make_format (pformat, سابقہ_len, "'-+ 0", ".0f");
  واپس لکھو (pformat, -0.25);
}

/* Output the number of seconds since the Epoch, using a format that
   acts like printf's %f format.  */
ساکن خالی
برآمد_epoch_sec (حرف *pformat, size_t سابقہ_len,
               ساخت timespec arg)
{
  حرف *dot = memchr (pformat, '.', سابقہ_len);
  size_t sec_سابقہ_len = سابقہ_len;
  صحیح چوڑائی = 0;
  صحیح precision = 0;
  bool frac_بایاں_adjust = false;

  اگر (dot)
    {
      sec_سابقہ_len = dot - pformat;
      pformat[سابقہ_len] = '\0';

      اگر (c_isdigit (dot[1]))
        {
          طویل صحیح lprec = strtol (dot + 1, تہی, 10);
          precision = (lprec <= زیادہ_سے_زیادہ_عدد ? lprec : زیادہ_سے_زیادہ_عدد);
        }
      ورنہ
        {
          precision = 9;
        }

      اگر (precision && c_isdigit (dot[-1]))
        {
          /* If a nontrivial width is given, subtract the width of the
             decimal point and PRECISION digits that will be output
             later.  */
          حرف *p = dot;
          *dot = '\0';

          کرو
            --p;
          جب (c_isdigit (p[-1]));

          طویل صحیح lwidth = strtol (p, تہی, 10);
          چوڑائی = (lwidth <= زیادہ_سے_زیادہ_عدد ? lwidth : زیادہ_سے_زیادہ_عدد);
          اگر (1 < چوڑائی)
            {
              p += (*p == '0');
              sec_سابقہ_len = p - pformat;
              صحیح w_d = (decimal_نقطہ_len < چوڑائی
                         ? چوڑائی - decimal_نقطہ_len
                         : 0);
              اگر (1 < w_d)
                {
                  صحیح w = w_d - precision;
                  اگر (1 < w)
                    {
                      حرف *dst = pformat;
                      برائے (حرف ثابت *src = dst; src < p; src++)
                        {
                          اگر (*src == '-')
                            frac_بایاں_adjust = true;
                          ورنہ
                            *dst++ = *src;
                        }
                      sec_سابقہ_len =
                        (dst - pformat
                         + (frac_بایاں_adjust ? 0 : sprintf (dst, "%d", w)));
                    }
                }
            }
        }
    }

  صحیح مقسوم_علیہ = 1;
  برائے (صحیح i = precision; i < 9; i++)
    مقسوم_علیہ *= 10;
  صحیح frac_sec = arg.tv_nsec / مقسوم_علیہ;
  صحیح int_len;

  اگر (TYPE_SIGNED (time_t))
    {
      bool minus_zero = false;
      اگر (arg.tv_sec < 0 && arg.tv_nsec != 0)
        {
          صحیح frac_sec_modulus = 1000000000 / مقسوم_علیہ;
          frac_sec = (frac_sec_modulus - frac_sec
                      - (arg.tv_nsec % مقسوم_علیہ != 0));
          arg.tv_sec += (frac_sec != 0);
          minus_zero = (arg.tv_sec == 0);
        }
      int_len = (minus_zero
                 ? برآمد_minus_zero (pformat, sec_سابقہ_len)
                 : برآمد_int (pformat, sec_سابقہ_len, arg.tv_sec));
    }
  ورنہ
    int_len = برآمد_uint (pformat, sec_سابقہ_len, arg.tv_sec);

  اگر (precision)
    {
      صحیح prec = (precision < 9 ? precision : 9);
      صحیح trailing_prec = precision - prec;
      صحیح ilen = (int_len < 0 ? 0 : int_len);
      صحیح trailing_چوڑائی = (ilen < چوڑائی && decimal_نقطہ_len < چوڑائی - ilen
                            ? چوڑائی - ilen - decimal_نقطہ_len - prec
                            : 0);
      لکھو ("%s%.*d%-*.*d", decimal_نقطہ, prec, frac_sec,
              trailing_چوڑائی, trailing_prec, 0);
    }
}

/* Print the context information of FILENAME, and return true iff the
   context could not be obtained.  */
NODISCARD
ساکن bool
برآمد_file_context (حرف *pformat, size_t سابقہ_len, حرف ثابت *filename)
{
  حرف *scontext;
  bool fail = false;

  اگر ((follow_links
       ? getfilecon (filename, &scontext)
       : lgetfilecon (filename, &scontext)) < 0)
    {
      error (0, errno, _("failed to get security context of %s"),
             quoteaf (filename));
      scontext = تہی;
      fail = true;
    }
  strcpy (pformat + سابقہ_len, "s");
  لکھو (pformat, (scontext ? scontext : "?"));
  اگر (scontext)
    freecon (scontext);
  واپس fail;
}

/* Set the quoting style once it is needed.  */
ساکن خالی
initialize_quoting_style (خالی)
{
  ساکن bool initialized;
  اگر (!initialized)
    {
      initialized = true;
      صحیح qs = getenv_quoting_style ();
      سیٹ_quoting_style (تہی, 0 <= qs ? qs : shell_escape_quoting_style);
    }
}

/* Print statfs info.  Return zero upon success, nonzero upon failure.  */
NODISCARD
ساکن bool
print_statfs (حرف *pformat, size_t سابقہ_len, MAYBE_غیر_مستعمل حرف mod, حرف m,
              MAYBE_غیر_مستعمل صحیح fd, حرف ثابت *filename,
              خالی ثابت *ڈیٹا)
{
  STRUCT_STATVFS ثابت *statfsbuf = ڈیٹا;
  bool fail = false;

  انتخاب (m)
    {
    حالت 'n':
      اگر (mod == 'Q')
        {
          initialize_quoting_style ();
          filename = quoteN (filename);
        }
      برآمد_string (pformat, سابقہ_len, filename);
      توڑو;

    حالت 'i':
      {
#اگر STRUCT_STATXFS_F_FSID_IS_INTEGER
        uintmax_t fsid = statfsbuf->f_fsid;
#ورنہ
        قسمتعریف غیرعلامتی صحیح fsid_لفظ;
        static_assert (alignof (STRUCT_STATVFS) % alignof (fsid_لفظ) == 0);
        static_assert (offsetof (STRUCT_STATVFS, f_fsid) % alignof (fsid_لفظ)
                       == 0);
        static_assert (حجم statfsbuf->f_fsid % alignof (fsid_لفظ) == 0);
        fsid_لفظ ثابت *p = (fsid_لفظ *) &statfsbuf->f_fsid;

        /* Assume a little-endian word order, as that is compatible
           with glibc's statvfs implementation.  */
        uintmax_t fsid = 0;
        صحیح الفاظ = حجم statfsbuf->f_fsid / حجم *p;
        برائے (صحیح i = 0; i < الفاظ && i * حجم *p < حجم fsid; i++)
          {
            uintmax_t u = p[الفاظ - 1 - i];
            fsid |= u << (i * CHAR_BIT * حجم *p);
          }
#اگر_ختم
        برآمد_uint_x (pformat, سابقہ_len, fsid);
      }
      توڑو;

    حالت 'l':
      برآمد_NAMEMAX (pformat, سابقہ_len, SB_F_NAMEMAX (statfsbuf));
      توڑو;
    حالت 't':
#اگر HAVE_STRUCT_STATXFS_F_TYPE
      برآمد_uint_x (pformat, سابقہ_len, statfsbuf->f_type);
#ورنہ
      fputc ('?', stdout);
#اگر_ختم
      توڑو;
    حالت 'T':
      برآمد_string (pformat, سابقہ_len, human_fstype (statfsbuf));
      توڑو;
    حالت 'b':
      برآمد_int (pformat, سابقہ_len, statfsbuf->f_بلاکیں);
      توڑو;
    حالت 'f':
      برآمد_int (pformat, سابقہ_len, statfsbuf->f_bfree);
      توڑو;
    حالت 'a':
      برآمد_int (pformat, سابقہ_len, statfsbuf->f_bavail);
      توڑو;
    حالت 's':
      برآمد_uint (pformat, سابقہ_len, statfsbuf->f_bsize);
      توڑو;
    حالت 'S':
      {
        uintmax_t frsize = STATFS_FRSIZE (statfsbuf);
        اگر (! frsize)
          frsize = statfsbuf->f_bsize;
        برآمد_uint (pformat, سابقہ_len, frsize);
      }
      توڑو;
    حالت 'c':
      برآمد_uint (pformat, سابقہ_len, statfsbuf->f_files);
      توڑو;
    حالت 'd':
      برآمد_int (pformat, سابقہ_len, statfsbuf->f_ffree);
      توڑو;
    طےشدہ:
      fputc ('?', stdout);
      توڑو;
    }
  واپس fail;
}

/* Return any bind mounted source for a path.
   The caller should not free the returned buffer.
   Return NULL if no bind mount found.  */
NODISCARD
ساکن حرف ثابت *
ڈھونڈو_bind_mount (حرف ثابت * name)
{
  حرف ثابت * bind_mount = تہی;

  ساکن ساخت mount_اندراج *mount_list;
  ساکن bool tried_mount_list = false;
  اگر (!tried_mount_list) /* attempt/warn once per process.  */
    {
      اگر (!(mount_list = read_file_system_list (false)))
        error (0, errno, "%s", _("cannot read table of mounted file systems"));
      tried_mount_list = true;
    }

  ساخت stat name_stats;
  اگر (stat (name, &name_stats) != 0)
    واپس تہی;

  برائے (ساخت mount_اندراج *me = mount_list; me; me = me->me_اگلا)
    {
      اگر (me->me_dummy && me->me_devname[0] == '/'
          && streq (me->me_mountdir, name))
        {
          ساخت stat dev_stats;

          اگر (stat (me->me_devname, &dev_stats) == 0
              && psame_inode (&name_stats, &dev_stats))
            {
              bind_mount = me->me_devname;
              توڑو;
            }
        }
    }

  واپس bind_mount;
}

/* Print mount point.  Return zero upon success, nonzero upon failure.  */
NODISCARD
ساکن bool
برآمد_mount_نقطہ (حرف ثابت *filename, حرف *pformat, size_t سابقہ_len,
                 ثابت ساخت stat *statp)
{

  حرف ثابت *np = "?", *bp = تہی;
  حرف *mp = تہی;
  bool fail = true;

  /* Look for bind mounts first.  Note we output the immediate alias,
     rather than further resolving to a base device mount point.  */
  اگر (follow_links || !S_ISLNK (statp->st_منوال))
    {
      حرف *resolved = canonicalize_file_name (filename);
      اگر (!resolved)
        {
          error (0, errno, _("failed to canonicalize %s"), quoteaf (filename));
          جاؤ print_mount_نقطہ;
        }
      bp = ڈھونڈو_bind_mount (resolved);
      آزاد (resolved);
      اگر (bp)
        {
          fail = false;
          جاؤ print_mount_نقطہ;
        }
    }

  /* If there is no direct bind mount, then navigate
     back up the tree looking for a device change.
     Note we don't detect if any of the directory components
     are bind mounted to the same device, but that's OK
     since we've not directly queried them.  */
  اگر ((mp = ڈھونڈو_mount_نقطہ (filename, statp)))
    {
      /* This dir might be bind mounted to another device,
         so we resolve the bound source in that case also.  */
      bp = ڈھونڈو_bind_mount (mp);
      fail = false;
    }

print_mount_نقطہ:

  برآمد_string (pformat, سابقہ_len, bp ? bp : mp ? mp : np);
  آزاد (mp);
  واپس fail;
}

/* Map a TS with negative TS.tv_nsec to {0,0}.  */
ساکن اندرونی ساخت timespec
neg_مقصد_zero (ساخت timespec ts)
{
  اگر (0 <= ts.tv_nsec)
    واپس ts;
  ساخت timespec z = {0};
  واپس z;
}

/* Output a single-character \ escape.  */

ساکن خالی
print_esc_char (حرف c)
{
  انتخاب (c)
    {
    حالت 'a':			/* Alert. */
      c ='\a';
      توڑو;
    حالت 'b':			/* Backspace. */
      c ='\b';
      توڑو;
    حالت 'e':			/* Escape. */
      c ='\x1B';
      توڑو;
    حالت 'f':			/* Form feed. */
      c ='\f';
      توڑو;
    حالت 'n':			/* New line. */
      c ='\n';
      توڑو;
    حالت 'r':			/* Carriage return. */
      c ='\r';
      توڑو;
    حالت 't':			/* Horizontal tab. */
      c ='\t';
      توڑو;
    حالت 'v':			/* Vertical tab. */
      c ='\v';
      توڑو;
    حالت '"':
    حالت '\\':
      توڑو;
    طےشدہ:
      error (0, 0, _("warning: unrecognized escape '\\%c'"), c);
      توڑو;
    }
  حرف_لکھو (c);
}

ATTRIBUTE_خالص
ساکن size_t
format_رمز_انحراف (حرف ثابت *directive)
{
  size_t len = strspn (directive + 1, printf_flags);
  حرف ثابت *fmt_char = directive + len + 1;
  fmt_char += strspn (fmt_char, digits);
  اگر (*fmt_char == '.')
    fmt_char += 1 + strspn (fmt_char + 1, digits);
  واپس fmt_char - directive;
}

/* Print the information specified by the format string, FORMAT,
   calling PRINT_FUNC for each %-directive encountered.
   Return zero upon success, nonzero upon failure.  */
NODISCARD
ساکن bool
print_it (حرف ثابت *format, صحیح fd, حرف ثابت *filename,
          bool (*print_func) (حرف *, size_t, حرف, حرف,
                              صحیح, حرف ثابت *, خالی ثابت *),
          خالی ثابت *ڈیٹا)
{
  bool fail = false;

  /* Add 2 to accommodate our conversion of the stat '%s' format string
     to the longer printf '%llu' one.  */
  شمار
    {
      MAX_ADDITIONAL_بائٹیں =
        (MAX (حجم "jd",
              MAX (حجم "jo", MAX (حجم "ju", حجم "jx")))
         - 1)
    };
  size_t n_alloc = لڑی_لمبائی (format) + MAX_ADDITIONAL_بائٹیں + 1;
  حرف *dest = xmalloc (n_alloc);
  برائے (حرف ثابت *b = format; *b; b++)
    {
      انتخاب (*b)
        {
        حالت '%':
          {
            size_t len = format_رمز_انحراف (b);
            حرف fmt_char = *(b + len);
            حرف mod_char = 0;
            حافظہ_نقل (dest, b, len);
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
                حرف_لکھو ('%');
                توڑو;
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
                ورنہ
                  {
                    fmt_char = mod_char;
                    mod_char = 0;
                  }
                FALLTHROUGH;
              طےشدہ:
                fail |= print_func (dest, len, mod_char, fmt_char,
                                    fd, filename, ڈیٹا);
                توڑو;
              }
            توڑو;
          }

        حالت '\\':
          اگر ( ! interpret_backslash_escapes)
            {
              حرف_لکھو ('\\');
              توڑو;
            }
          ++b;
          اگر (isoct (*b))
            {
              صحیح esc_قدر = fromoct (*b);
              صحیح esc_لمبائی = 1;	/* number of octal digits */
              برائے (++b; esc_لمبائی < 3 && isoct (*b);
                   ++esc_لمبائی, ++b)
                {
                  esc_قدر = esc_قدر * 8 + fromoct (*b);
                }
              حرف_لکھو (esc_قدر);
              --b;
            }
          ورنہ اگر (*b == 'x' && c_isxdigit (b[1]))
            {
              صحیح esc_قدر = fromhex (b[1]);	/* Value of \xhh escape. */
              /* A hexadecimal \xhh escape sequence must have
                 1 or 2 hex. digits.  */
              ++b;
              اگر (c_isxdigit (b[1]))
                {
                  ++b;
                  esc_قدر = esc_قدر * 16 + fromhex (*b);
                }
              حرف_لکھو (esc_قدر);
            }
          ورنہ اگر (*b == '\0')
            {
              error (0, 0, _("warning: backslash at end of format"));
              حرف_لکھو ('\\');
              /* Arrange to exit the loop.  */
              --b;
            }
          ورنہ
            {
              print_esc_char (*b);
            }
          توڑو;

        طےشدہ:
          حرف_لکھو (*b);
          توڑو;
        }
    }
  آزاد (dest);

  fputs (trailing_delim, stdout);

  واپس fail;
}

/* Stat the file system and print what we find.  */
NODISCARD
ساکن bool
do_statfs (حرف ثابت *filename, حرف ثابت *format)
{
  STRUCT_STATVFS statfsbuf;

  اگر (streq (filename, "-"))
    {
      error (0, 0, _("using %s to denote standard input does not work"
                     " in file system mode"), quoteaf (filename));
      واپس false;
    }

  اگر (STATFS (filename, &statfsbuf) != 0)
    {
      error (0, errno, _("cannot read file system information for %s"),
             quoteaf (filename));
      واپس false;
    }

  bool fail = print_it (format, -1, filename, print_statfs, &statfsbuf);
  واپس ! fail;
}

ساخت print_دلائل {
  ساخت stat *st;
  ساخت timespec btime;
};

/* Ask statx to avoid syncing? */
ساکن bool dont_sync;

/* Ask statx to force sync? */
ساکن bool force_sync;

#اگر USE_STATX
ساکن غیرعلامتی صحیح
fmt_مقصد_نقاب (حرف fmt)
{
  انتخاب (fmt)
    {
    حالت 'N':
      واپس STATX_منوال;
    حالت 'd':
    حالت 'D':
      واپس STATX_منوال;
    حالت 'i':
      واپس STATX_INO;
    حالت 'a':
    حالت 'A':
      واپس STATX_منوال;
    حالت 'f':
      واپس STATX_منوال|STATX_TYPE;
    حالت 'F':
      واپس STATX_TYPE;
    حالت 'h':
      واپس STATX_NLINK;
    حالت 'u':
    حالت 'U':
      واپس STATX_UID;
    حالت 'g':
    حالت 'G':
      واپس STATX_GID;
    حالت 'm':
      واپس STATX_منوال|STATX_INO;
    حالت 's':
      واپس STATX_سائز;
    حالت 't':
    حالت 'T':
      واپس STATX_منوال;
    حالت 'b':
      واپس STATX_بلاکیں;
    حالت 'w':
    حالت 'W':
      واپس STATX_BTIME;
    حالت 'x':
    حالت 'X':
      واپس STATX_ATIME;
    حالت 'y':
    حالت 'Y':
      واپس STATX_MTIME;
    حالت 'z':
    حالت 'Z':
      واپس STATX_CTIME;
    }
  واپس 0;
}

ATTRIBUTE_خالص
ساکن غیرعلامتی صحیح
format_مقصد_نقاب (حرف ثابت *format)
{
  غیرعلامتی صحیح نقاب = 0;

  برائے (حرف ثابت *b = format; *b; b++)
    {
      اگر (*b != '%')
        جاری;

      b += format_رمز_انحراف (b);
      اگر (*b == '\0')
        توڑو;
      نقاب |= fmt_مقصد_نقاب (*b);
    }
  واپس نقاب;
}

/* statx the file and print what we find */
NODISCARD
ساکن bool
do_stat (حرف ثابت *filename, حرف ثابت *format, حرف ثابت *format2)
{
  صحیح fd = streq (filename, "-") ? 0 : AT_FDCWD;
  صحیح flags = 0;
  ساخت stat st;
  ساخت statx stx = {0};
  حرف ثابت *pathname = filename;
  ساخت print_دلائل pa;
  pa.st = &st;
  pa.btime = (ساخت timespec) {.tv_sec = -1, .tv_nsec = -1};

  اگر (AT_FDCWD != fd)
    {
      pathname = "";
      flags = AT_خالی_خانے_راستہ;
    }
  ورنہ اگر (!follow_links)
    {
      flags = AT_SYMLINK_NOFOLLOW;
    }

  اگر (dont_sync)
    flags |= AT_STATX_DONT_SYNC;
  ورنہ اگر (force_sync)
    flags |= AT_STATX_FORCE_SYNC;

  اگر (! force_sync)
    flags |= AT_NO_AUTOMOUNT;

  fd = statx (fd, pathname, flags, format_مقصد_نقاب (format), &stx);
  اگر (fd < 0)
    {
      اگر (flags & AT_خالی_خانے_راستہ)
        error (0, errno, _("cannot stat standard input"));
      ورنہ
        error (0, errno, _("cannot statx %s"), quoteaf (filename));
      واپس false;
    }

  اگر (S_ISBLK (stx.stx_منوال) || S_ISCHR (stx.stx_منوال))
    format = format2;

  statx_مقصد_stat (&stx, &st);
  اگر (stx.stx_نقاب & STATX_BTIME)
    pa.btime = statx_timestamp_مقصد_timespec (stx.stx_btime);

  bool fail = print_it (format, fd, filename, print_stat, &pa);
  واپس ! fail;
}

#ورنہ /* USE_STATX */

ساکن ساخت timespec
get_birthtime (صحیح fd, حرف ثابت *filename, ساخت stat ثابت *st)
{
  ساخت timespec ts = get_stat_birthtime (st);

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
              && val[0] <= TYPE_زیادہ_سے_زیادہ (time_t)
              && val[1] < 1000000000 * 2 /* for leap seconds */)
            {
              ts.tv_sec = val[0];
              ts.tv_nsec = val[1];
            }
          nvlist_free (response);
        }
    }
# endif

  واپس ts;
}


/* stat the file and print what we find */
NODISCARD
ساکن bool
do_stat (حرف ثابت *filename, حرف ثابت *format,
         حرف ثابت *format2)
{
  صحیح fd = streq (filename, "-") ? 0 : -1;
  ساخت stat statbuf;
  ساخت print_دلائل pa;
  pa.st = &statbuf;
  pa.btime = (ساخت timespec) {.tv_sec = -1, .tv_nsec = -1};

  اگر (0 <= fd)
    {
      اگر (fstat (fd, &statbuf) != 0)
        {
          error (0, errno, _("cannot stat standard input"));
          واپس false;
        }
    }
  /* We can't use the shorter
     (follow_links?stat:lstat) (filename, &statbug)
     since stat might be a function-like macro.  */
  ورنہ اگر ((follow_links
            ? stat (filename, &statbuf)
            : lstat (filename, &statbuf)) != 0)
    {
      error (0, errno, _("cannot stat %s"), quoteaf (filename));
      واپس false;
    }

  اگر (S_ISBLK (statbuf.st_منوال) || S_ISCHR (statbuf.st_منوال))
    format = format2;

  bool fail = print_it (format, fd, filename, print_stat, &pa);
  واپس ! fail;
}
#اگر_ختم /* USE_STATX */

/* POSIX requires 'ls' to print file sizes without a sign, even
   when negative.  Be consistent with that.  */

ساکن uintmax_t
unsigned_file_سائز (off_t سائز)
{
  واپس سائز + (سائز < 0) * ((uintmax_t) OFF_T_MAX - OFF_T_MIN + 1);
}

/* Print stat info.  Return zero upon success, nonzero upon failure.  */
ساکن bool
print_stat (حرف *pformat, size_t سابقہ_len, حرف mod, حرف m,
            MAYBE_غیر_مستعمل صحیح fd, حرف ثابت *filename, خالی ثابت *ڈیٹا)
{
  ساخت print_دلائل *parg = (ساخت print_دلائل *) ڈیٹا;
  ساخت stat *statbuf = parg->st;
  ساخت timespec btime = parg->btime;
  ساخت passwd *pw_ent;
  ساخت گروہ *gw_ent;
  bool fail = false;

  انتخاب (m)
    {
    حالت 'n':
      اگر (mod == 'Q')
        {
          initialize_quoting_style ();
          filename = quoteN (filename);
        }
      برآمد_string (pformat, سابقہ_len, filename);
      توڑو;
    حالت 'N':
      initialize_quoting_style ();
      برآمد_string (pformat, سابقہ_len, quoteN (filename));
      اگر (S_ISLNK (statbuf->st_منوال))
        {
          حرف *linkname = areadlink_with_سائز (filename, statbuf->st_سائز);
          اگر (linkname == تہی)
            {
              error (0, errno, _("cannot read symbolic link %s"),
                     quoteaf (filename));
              واپس true;
            }
          لکھو (" -> ");
          برآمد_string (pformat, سابقہ_len, quoteN (linkname));
          آزاد (linkname);
        }
      توڑو;
    حالت 'd':
      اگر (mod == 'H')
        برآمد_uint (pformat, سابقہ_len, major (statbuf->st_dev));
      ورنہ اگر (mod == 'L')
        برآمد_uint (pformat, سابقہ_len, minor (statbuf->st_dev));
      ورنہ
        برآمد_uint (pformat, سابقہ_len, statbuf->st_dev);
      توڑو;
    حالت 'D':
      برآمد_uint_x (pformat, سابقہ_len, statbuf->st_dev);
      توڑو;
    حالت 'i':
      برآمد_uint (pformat, سابقہ_len, statbuf->st_ino);
      توڑو;
    حالت 'a':
      برآمد_uint_o (pformat, سابقہ_len, statbuf->st_منوال & CHMOD_منوال_بٹ);
      توڑو;
    حالت 'A':
      برآمد_string (pformat, سابقہ_len, human_access (statbuf));
      توڑو;
    حالت 'f':
      برآمد_uint_x (pformat, سابقہ_len, statbuf->st_منوال);
      توڑو;
    حالت 'F':
      برآمد_string (pformat, سابقہ_len, file_type (statbuf));
      توڑو;
    حالت 'h':
      برآمد_uint (pformat, سابقہ_len, statbuf->st_nlink);
      توڑو;
    حالت 'u':
      برآمد_uint (pformat, سابقہ_len, statbuf->st_uid);
      توڑو;
    حالت 'U':
      pw_ent = getpwuid (statbuf->st_uid);
      برآمد_string (pformat, سابقہ_len,
                  pw_ent ? pw_ent->pw_name : "UNKNOWN");
      توڑو;
    حالت 'g':
      برآمد_uint (pformat, سابقہ_len, statbuf->st_gid);
      توڑو;
    حالت 'G':
      gw_ent = getgrgid (statbuf->st_gid);
      برآمد_string (pformat, سابقہ_len,
                  gw_ent ? gw_ent->gr_name : "UNKNOWN");
      توڑو;
    حالت 'm':
      fail |= برآمد_mount_نقطہ (filename, pformat, سابقہ_len, statbuf);
      توڑو;
    حالت 's':
      برآمد_uint (pformat, سابقہ_len, unsigned_file_سائز (statbuf->st_سائز));
      توڑو;
    حالت 'r':
      اگر (mod == 'H')
        برآمد_uint (pformat, سابقہ_len, major (statbuf->st_rdev));
      ورنہ اگر (mod == 'L')
        برآمد_uint (pformat, سابقہ_len, minor (statbuf->st_rdev));
      ورنہ
        برآمد_uint (pformat, سابقہ_len, statbuf->st_rdev);
      توڑو;
    حالت 'R':
      برآمد_uint_x (pformat, سابقہ_len, statbuf->st_rdev);
      توڑو;
    حالت 't':
      برآمد_uint_x (pformat, سابقہ_len, major (statbuf->st_rdev));
      توڑو;
    حالت 'T':
      برآمد_uint_x (pformat, سابقہ_len, minor (statbuf->st_rdev));
      توڑو;
    حالت 'B':
      برآمد_uint (pformat, سابقہ_len, ST_NBLOCKSIZE);
      توڑو;
    حالت 'b':
      برآمد_uint (pformat, سابقہ_len, STP_NBLOCKS (statbuf));
      توڑو;
    حالت 'o':
      برآمد_uint (pformat, سابقہ_len, STP_BLKSIZE (statbuf));
      توڑو;
    حالت 'w':
      {
#اگر ! USE_STATX
        btime = get_birthtime (fd, filename, statbuf);
#اگر_ختم
        اگر (btime.tv_nsec < 0)
          برآمد_string (pformat, سابقہ_len, "-");
        ورنہ
          برآمد_string (pformat, سابقہ_len, human_وقت (btime));
      }
      توڑو;
    حالت 'W':
      {
#اگر ! USE_STATX
        btime = get_birthtime (fd, filename, statbuf);
#اگر_ختم
        برآمد_epoch_sec (pformat, سابقہ_len, neg_مقصد_zero (btime));
      }
      توڑو;
    حالت 'x':
      برآمد_string (pformat, سابقہ_len, human_وقت (get_stat_atime (statbuf)));
      توڑو;
    حالت 'X':
      برآمد_epoch_sec (pformat, سابقہ_len, get_stat_atime (statbuf));
      توڑو;
    حالت 'y':
      برآمد_string (pformat, سابقہ_len, human_وقت (get_stat_mtime (statbuf)));
      توڑو;
    حالت 'Y':
      برآمد_epoch_sec (pformat, سابقہ_len, get_stat_mtime (statbuf));
      توڑو;
    حالت 'z':
      برآمد_string (pformat, سابقہ_len, human_وقت (get_stat_ctime (statbuf)));
      توڑو;
    حالت 'Z':
      برآمد_epoch_sec (pformat, سابقہ_len, get_stat_ctime (statbuf));
      توڑو;
    حالت 'C':
      fail |= برآمد_file_context (pformat, سابقہ_len, filename);
      توڑو;
    طےشدہ:
      fputc ('?', stdout);
      توڑو;
    }
  واپس fail;
}

/* Return an allocated format string in static storage that
   corresponds to whether FS and TERSE options were declared.  */
ساکن حرف *
default_format (bool fs, bool terse, bool device)
{
  حرف *format;
  اگر (fs)
    {
      اگر (terse)
        format = xstrdup (fmt_terse_fs);
      ورنہ
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
  ورنہ /* ! fs */
    {
      اگر (terse)
        {
          اگر (0 < is_selinux_enabled ())
            format = xstrdup (fmt_terse_selinux);
          ورنہ
            format = xstrdup (fmt_terse_regular);
        }
      ورنہ
        {
          حرف *عارضی;
          /* TRANSLATORS: This string uses format specifiers from
             'stat --help' without --file-system, and NOT from printf.  */
          format = xstrdup (_("\
  File: %N\n\
  سائز_3: %-10s\t_بلاکیں: %-10b IO بلاک_3: %-6o %F\n\
"));

          عارضی = format;
          اگر (device)
            {
              /* TRANSLATORS: This string uses format specifiers from
                 'stat --help' without --file-system, and NOT from printf.  */
              format = xasprintf ("%s%s", format, _("\
" "Device: %Hd,%Ld\tInode: %-10i  Links: %-5h Device type: %Hr,%Lr\n\
"));
            }
          ورنہ
            {
              /* TRANSLATORS: This string uses format specifiers from
                 'stat --help' without --file-system, and NOT from printf.  */
              format = xasprintf ("%s%s", format, _("\
" "Device: %Hd,%Ld\tInode: %-10i  Links: %h\n\
"));
            }
          آزاد (عارضی);

          عارضی = format;
          /* TRANSLATORS: This string uses format specifiers from
             'stat --help' without --file-system, and NOT from printf.  */
          format = xasprintf ("%s%s", format, _("\
" "Access: (%04a/%10.10A)  Uid: (%5u/%8U)   Gid: (%5g/%8G)\n\
"));
          آزاد (عارضی);

          اگر (0 < is_selinux_enabled ())
            {
              عارضی = format;
              /* TRANSLATORS: This string uses format specifiers from
                 'stat --help' without --file-system, and NOT from printf.  */
              format = xasprintf ("%s%s", format, _("Context: %C\n"));
              آزاد (عارضی);
            }

          عارضی = format;
          format = xasprintf ("%s%s", format,
                              /* TRANSLATORS: This string uses format specifiers
                                 from 'stat --help' without --file-system, and
                                 NOT from printf.  */
                              _("Access: %x\n"
                                "Modify: %y\n"
                                "Change: %z\n"
                                " Birth: %w\n"));
          آزاد (عارضی);
        }
    }
  واپس format;
}

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("Usage: %s [OPTION]... FILE...\n"), پروگرام_name);
      fputs (_("\
Display file or file system حالت_2.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -L, --dereference\n\
         follow links\n\
"));
      oputs (_("\
  -f, --file-system\n\
         display file system حالت_2 instead کا file حالت_2\n\
"));
      oputs (_("\
      --cached=منوال\n\
         specify how مقصد use cached اوصاف;\n\
         useful on remote file systems. See منوال نیچے\n\
"));
      oputs (_("\
  -c, --format=FORMAT\n\
         use the specified FORMAT instead کا the طےشدہ;\n\
         output a newline after each use کا FORMAT\n\
"));
      oputs (_("\
      --لکھو=FORMAT\n\
         like --format, but interpret backslash escapes,\n\
         and کرو not output a mandatory trailing newline;\n\
         اگر you want a newline, include \\n اندر FORMAT\n\
"));
      oputs (_("\
  -t, --terse\n\
         print the information اندر terse form\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);

      fputs (_("\n\
The منوال دلیل کا --cached can be: always, never, or طےشدہ.\n\
'always' will use cached اوصاف اگر دستیاب, جب\n\
'never' will try مقصد synchronize with the latest اوصاف, and\n\
'default' will leave it اوپر مقصد the underlying file system.\n\
"), stdout);

      fputs (_("\n\
The valid format sequences برائے files (without --file-system):\n\
\n\
  %a   permission بٹ_2 اندر octal (see '#' and '0' لکھو flags)\n\
  %A   permission بٹ_2 and file type اندر human readable form\n\
  %b   نمبر کا بلاکیں allocated (see %B)\n\
  %B   the سائز اندر بائٹیں کا each بلاک reported by %b\n\
  %C   SELinux security context string\n\
"), stdout);
      fputs (_("\
  %d   device نمبر اندر decimal (st_dev)\n\
  %D   device نمبر اندر hex (st_dev)\n\
  %Hd  major device نمبر اندر decimal\n\
  %Ld  minor device نمبر اندر decimal\n\
  %f   raw منوال_2 اندر hex\n\
  %F   file type\n\
  %g   گروہ ID کا owner\n\
  %G   گروہ name کا owner\n\
"), stdout);
      fputs (_("\
  %h   نمبر کا hard links\n\
  %i   inode نمبر\n\
  %m   mount نقطہ\n\
  %n   file name\n\
  %Qn  quoted file name\n\
  %N   quoted file name with dereference اگر symbolic link\n\
  %o   optimal I/O transfer سائز hint\n\
  %s   کل سائز, اندر بائٹیں\n\
  %r   device type اندر decimal (st_rdev)\n\
  %R   device type اندر hex (st_rdev)\n\
  %Hr  major device type اندر decimal, برائے character/بلاک device special files\n\
  %Lr  minor device type اندر decimal, برائے character/بلاک device special files\n\
  %t   major device type اندر hex, برائے character/بلاک device special files\n\
  %T   minor device type اندر hex, برائے character/بلاک device special files\n\
"), stdout);
      fputs (_("\
  %u   user ID کا owner\n\
  %U   user name کا owner\n\
  %w   وقت کا file birth, human-readable; - اگر unknown\n\
  %W   وقت کا file birth, seconds since Epoch; 0 اگر unknown\n\
  %x   وقت کا آخری access, human-readable\n\
  %X   وقت کا آخری access, seconds since Epoch\n\
  %y   وقت کا آخری ڈیٹا modification, human-readable\n\
  %Y   وقت کا آخری ڈیٹا modification, seconds since Epoch\n\
  %z   وقت کا آخری حالت_2 change, human-readable\n\
  %Z   وقت کا آخری حالت_2 change, seconds since Epoch\n\
\n\
"), stdout);

      fputs (_("\
Valid format sequences برائے file systems:\n\
\n\
  %a   آزاد بلاکیں دستیاب مقصد non-superuser\n\
  %b   کل ڈیٹا بلاکیں اندر file system\n\
  %c   کل file nodes اندر file system\n\
  %d   آزاد file nodes اندر file system\n\
  %f   آزاد بلاکیں اندر file system\n\
"), stdout);
      fputs (_("\
  %i   file system ID اندر hex\n\
  %l   زیادہ_سے_زیادہ لمبائی کا filenames\n\
  %n   file name\n\
  %Qn  quoted file name\n\
  %s   بلاک سائز (برائے faster transfers)\n\
  %S   fundamental بلاک سائز (برائے بلاک counts)\n\
  %t   file system type اندر hex\n\
  %T   file system type اندر human readable form\n\
"), stdout);

      لکھو (_("\n\
--terse is equivalent مقصد the following FORMAT:\n\
    %s\
"),
#اگر HAVE_SELINUX_SELINUX_H
              fmt_terse_selinux
#ورنہ
              fmt_terse_regular
#اگر_ختم
              );

        لکھو (_("\
--terse --file-system is equivalent مقصد the following FORMAT:\n\
    %s\
"), fmt_terse_fs);

      لکھو (USAGE_BUILTIN_WARNING, پروگرام_NAME);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

صحیح
مین (صحیح argc, حرف *argv[])
{
  صحیح c;
  bool fs = false;
  bool terse = false;
  حرف *format = تہی;
  حرف *format2;
  bool ok = true;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  ساخت lconv ثابت *locale = localeconv ();
  decimal_نقطہ = (locale->decimal_نقطہ[0] ? locale->decimal_نقطہ : ".");
  decimal_نقطہ_len = لڑی_لمبائی (decimal_نقطہ);

  atexit (close_stdout);

  جب ((c = getopt_long (argc, argv, "c:fLt", long_options, تہی)) != -1)
    {
      انتخاب (c)
        {
        حالت PRINTF_OPTION:
          format = optarg;
          interpret_backslash_escapes = true;
          trailing_delim = "";
          توڑو;

        حالت 'c':
          format = optarg;
          interpret_backslash_escapes = false;
          trailing_delim = "\n";
          توڑو;

        حالت 'L':
          follow_links = true;
          توڑو;

        حالت 'f':
          fs = true;
          توڑو;

        حالت 't':
          terse = true;
          توڑو;

        حالت 0:
          انتخاب (XARGMATCH ("--cached", optarg, cached_دلائل, cached_modes))
            {
              حالت cached_never:
                force_sync = true;
                dont_sync = false;
                توڑو;
              حالت cached_always:
                force_sync = false;
                dont_sync = true;
                توڑو;
              حالت cached_default:
                force_sync = false;
                dont_sync = false;
            }
          توڑو;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

        طےشدہ:
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
  ورنہ
    {
      format = default_format (fs, terse, /* device= */ false);
      format2 = default_format (fs, terse, /* device= */ true);
    }

  برائے (صحیح i = optind; i < argc; i++)
    ok &= (fs
           ? do_statfs (argv[i], format)
           : do_stat (argv[i], format, format2));

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
