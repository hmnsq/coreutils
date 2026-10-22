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

#অন্তর্ভাব <config.h>

/* Keep this conditional in sync with the similar conditional in
   ../m4/stat-prog.m4.  */
#যদি ((STAT_STATVFS || STAT_STATVFS64)                                       \
     && (HAVE_STRUCT_STATVFS_F_BASETYPE || HAVE_STRUCT_STATVFS_F_FSTYPENAME \
         || (! HAVE_STRUCT_STATFS_F_FSTYPENAME && HAVE_STRUCT_STATVFS_F_TYPE)))
# define USE_STATVFS 1
#অন্যথা
# define USE_STATVFS 0
#endif

#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <pwd.h>
#অন্তর্ভাব <grp.h>
#যদি USE_STATVFS
# include <sys/statvfs.h>
#নতুবা_যদি HAVE_SYS_VFS_H
# include <sys/vfs.h>
#নতুবা_যদি HAVE_SYS_MOUNT_H && HAVE_SYS_PARAM_H
/* NOTE: freebsd5.0 needs sys/param.h and sys/mount.h for statfs.
   It does have statvfs.h, but shouldn't use it, since it doesn't
   HAVE_STRUCT_STATVFS_F_BASETYPE.  So find a clean way to fix it.  */
/* NetBSD 1.5.2 needs these, for the declaration of struct statfs. */
# include <sys/param.h>
# include <sys/mount.h>
# যদি HAVE_NFS_NFS_CLNT_H && HAVE_NFS_VFS_H
/* Ultrix 4.4 needs these for the declaration of struct statfs.  */
#  include <netinet/in.h>
#  include <nfs/nfs_clnt.h>
#  include <nfs/vfs.h>
# endif
#নতুবা_যদি HAVE_OS_H /* BeOS */
# include <fs_info.h>
#endif
#অন্তর্ভাব <selinux/selinux.h>
#অন্তর্ভাব <getopt.h>

#অন্তর্ভাব "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#অন্তর্ভাব "system.h"

#অন্তর্ভাব "areadlink.h"
#অন্তর্ভাব "c-ctype.h"
#অন্তর্ভাব "file-type.h"
#অন্তর্ভাব "filemode.h"
#অন্তর্ভাব "fs.h"
#অন্তর্ভাব "mountlist.h"
#অন্তর্ভাব "octhexdigits.h"
#অন্তর্ভাব "quote.h"
#অন্তর্ভাব "stat-size.h"
#অন্তর্ভাব "stat-time.h"
#অন্তর্ভাব "strftime.h"
#অন্তর্ভাব "find-mount-point.h"
#অন্তর্ভাব "xvasprintf.h"
#অন্তর্ভাব "statx.h"

#যদি HAVE_STATX && defined STATX_INO
# define USE_STATX 1
#অন্যথা
# define USE_STATX 0
#endif

#যদি USE_STATVFS
# define STRUCT_STATXFS_F_FSID_IS_INTEGER STRUCT_STATVFS_F_FSID_IS_INTEGER
# define HAVE_STRUCT_STATXFS_F_TYPE HAVE_STRUCT_STATVFS_F_TYPE
# যদি HAVE_STRUCT_STATVFS_F_NAMEMAX
#  define SB_F_NAMEMAX(S) ((S)->f_namemax)
# endif
# যদি ! STAT_STATVFS && STAT_STATVFS64
#  define STRUCT_STATVFS কাঠামো statvfs64
#  define STATFS statvfs64
# অন্যথা
#  define STRUCT_STATVFS কাঠামো statvfs
#  define STATFS statvfs
# endif
# define STATFS_FRSIZE(S) ((S)->f_frsize)
#অন্যথা
# define HAVE_STRUCT_STATXFS_F_TYPE HAVE_STRUCT_STATFS_F_TYPE
# যদি HAVE_STRUCT_STATFS_F_NAMELEN
#  define SB_F_NAMEMAX(S) ((S)->f_namelen)
# elif HAVE_STRUCT_STATFS_F_NAMEMAX
#  define SB_F_NAMEMAX(S) ((S)->f_namemax)
# endif
# define STATFS statfs
# যদি HAVE_OS_H /* BeOS */
/* BeOS has a statvfs function, but it does not return sensible values
   for f_files, f_ffree and f_favail, and lacks f_type, f_basetype and
   f_fstypename.  Use 'struct fs_info' instead.  */
NODISCARD
স্থির পূর্ণসংখ্যা
statfs (অক্ষর ধ্রুবক *filename, কাঠামো fs_info *buf)
{
  dev_t device = dev_for_path (filename);
  যদি (device < 0)
    {
      errno = (device == B_ENTRY_NOT_FOUND ? ENOENT
               : device == B_BAD_মান ? EINVAL
               : device == B_NAME_TOO_LONG ? ENAMETOOLONG
               : device == B_NO_MEMORY ? ENOMEM
               : device == B_FILE_ERROR ? EIO
               : 0);
      ফেরত -1;
    }
  /* If successful, buf->dev will be == device.  */
  ফেরত fs_stat_dev (device, buf);
}
#  define f_fsid dev
#  define f_blocks যোগ_blocks
#  define f_bfree free_blocks
#  define f_bavail free_blocks
#  define f_bsize io_আকার
#  define f_files যোগ_nodes
#  define f_ffree free_nodes
#  define STRUCT_STATVFS কাঠামো fs_info
#  define STRUCT_STATXFS_F_FSID_IS_INTEGER true
#  define STATFS_FRSIZE(S) ((S)->block_আকার)
# অন্যথা
#  define STRUCT_STATVFS কাঠামো statfs
#  define STRUCT_STATXFS_F_FSID_IS_INTEGER STRUCT_STATFS_F_FSID_IS_INTEGER
#  যদি HAVE_STRUCT_STATFS_F_FRSIZE
#   define STATFS_FRSIZE(S) ((S)->f_frsize)
#  অন্যথা
#   define STATFS_FRSIZE(S) 0
#  endif
# endif
#endif

#ifdef SB_F_NAMEMAX
# define OUT_NAMEMAX out_uint
#অন্যথা
/* Depending on whether statvfs or statfs is used,
   neither f_namemax or f_namelen may be available.  */
# define SB_F_NAMEMAX(S) "?"
# define OUT_NAMEMAX out_string
#endif

#যদি HAVE_STRUCT_STATVFS_F_BASETYPE
# define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME f_basetype
#অন্যথা
# যদি HAVE_STRUCT_STATVFS_F_FSTYPENAME || HAVE_STRUCT_STATFS_F_FSTYPENAME
#  define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME f_fstypename
# elif HAVE_OS_H /* BeOS */
#  define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME fsh_name
# endif
#endif

#যদি HAVE_GETATTRAT
# include <attr.h>
# include <sys/nvpair.h>
#endif

স্থির অক্ষর ধ্রুবক digits[] = "0123456789";

/* Flags that are portable for use in printf, for at least one
   conversion specifier; make_format removes non-portable flags as
   needed for particular specifiers.  The glibc 2.2 extension "I" is
   listed here; it is removed by make_format because it has undefined
   behavior elsewhere and because it is incompatible with
   out_epoch_sec.  */
স্থির অক্ষর ধ্রুবক printf_flags[] = "'-+ #0I";

/* Formats for the --terse option.  */
স্থির অক্ষর ধ্রুবক fmt_terse_fs[] = "%Qn %i %l %t %s %S %b %f %a %c %d\n";
স্থির অক্ষর ধ্রুবক fmt_terse_regular[] = "%Qn %s %b %f %u %g %D %i %h %t %T"
                                        " %X %Y %Z %W %o\n";
স্থির অক্ষর ধ্রুবক fmt_terse_selinux[] = "%Qn %s %b %f %u %g %D %i %h %t %T"
                                        " %X %Y %Z %W %o %C\n";

#define PROGRAM_NAME "stat"

#define AUTHORS proper_name ("Michael Meskes")

গণনা
{
  PRINTF_OPTION = CHAR_MAX + 1
};

গণনা cached_mode
{
  cached_default,
  cached_never,
  cached_always
};

স্থির অক্ষর ধ্রুবক *ধ্রুবক cached_args[] =
{
  "default", "never", "always", ফাঁকা
};

স্থির গণনা cached_mode ধ্রুবক cached_modes[] =
{
  cached_default, cached_never, cached_always
};

স্থির কাঠামো option ধ্রুবক long_options[] =
{
  {"dereference", no_argument, ফাঁকা, 'L'},
  {"file-system", no_argument, ফাঁকা, 'f'},
  {"format", required_argument, ফাঁকা, 'c'},
  {"printf", required_argument, ফাঁকা, PRINTF_OPTION},
  {"terse", no_argument, ফাঁকা, 't'},
  {"cached", required_argument, ফাঁকা, 0},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ফাঁকা, 0, ফাঁকা, 0}
};

/* Whether to follow symbolic links;  True for --dereference (-L).  */
স্থির bool follow_links;

/* Whether to interpret backslash-escape sequences.
   True for --printf=FMT, not for --format=FMT (-c).  */
স্থির bool interpret_backslash_escapes;

/* The trailing delimiter string:
   "" for --printf=FMT, "\n" for --format=FMT (-c).  */
স্থির অক্ষর ধ্রুবক *trailing_delim = "";

/* The representation of the decimal point in the current locale.  */
স্থির অক্ষর ধ্রুবক *decimal_point;
স্থির size_t decimal_point_len;

স্থির bool
print_stat (অক্ষর *pformat, size_t prefix_len, অক্ষর mod, অক্ষর m,
            পূর্ণসংখ্যা fd, অক্ষর ধ্রুবক *filename, শূন্য ধ্রুবক *তথ্য);

/* Return the type of the specified file system.
   Some systems have statfvs.f_basetype[FSTYPSZ] (AIX, HP-UX, and Solaris).
   Others have statvfs.f_fstypename[_VFS_NAMELEN] (NetBSD 3.0).
   Others have statfs.f_fstypename[MFSNAMELEN] (NetBSD 1.5.2).
   Still others have neither and have to get by with f_type (GNU/Linux).
   But f_type may only exist in statfs (Cygwin).  */
NODISCARD
স্থির অক্ষর ধ্রুবক *
human_fstype (STRUCT_STATVFS ধ্রুবক *statfsbuf)
{
#ifdef STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME
  ফেরত statfsbuf->STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME;
#অন্যথা
  নির্বাচন (statfsbuf->f_type)
    {
# যদি defined __linux__ || defined __ANDROID__

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

    ক্ষেত্র S_MAGIC_AAFS: /* 0x5A3C69F0 local */
      ফেরত "aafs";
    ক্ষেত্র S_MAGIC_ACFS: /* 0x61636673 remote */
      ফেরত "acfs";
    ক্ষেত্র S_MAGIC_ADFS: /* 0xADF5 local */
      ফেরত "adfs";
    ক্ষেত্র S_MAGIC_AFFS: /* 0xADFF local */
      ফেরত "affs";
    ক্ষেত্র S_MAGIC_AFS: /* 0x5346414F remote */
      ফেরত "afs";
    ক্ষেত্র S_MAGIC_ANON_INODE_FS: /* 0x09041934 local */
      ফেরত "anon-inode FS";
    ক্ষেত্র S_MAGIC_AUFS: /* 0x61756673 remote */
      /* FIXME: change syntax or add an optional attribute like "inotify:no".
         The above is labeled as "remote" so that tail always uses polling,
         but this isn't really a remote file system type.  */
      ফেরত "aufs";
    ক্ষেত্র S_MAGIC_AUTOFS: /* 0x0187 local */
      ফেরত "autofs";
    ক্ষেত্র S_MAGIC_BALLOON_KVM: /* 0x13661366 local */
      ফেরত "balloon-kvm-fs";
    ক্ষেত্র S_MAGIC_BCACHEFS: /* 0xCA451A4E local */
      ফেরত "bcachefs";
    ক্ষেত্র S_MAGIC_BEFS: /* 0x42465331 local */
      ফেরত "befs";
    ক্ষেত্র S_MAGIC_BDEVFS: /* 0x62646576 local */
      ফেরত "bdevfs";
    ক্ষেত্র S_MAGIC_BFS: /* 0x1BADFACE local */
      ফেরত "bfs";
    ক্ষেত্র S_MAGIC_BINDERFS: /* 0x6C6F6F70 local */
      ফেরত "binderfs";
    ক্ষেত্র S_MAGIC_BPF_FS: /* 0xCAFE4A11 local */
      ফেরত "bpf_fs";
    ক্ষেত্র S_MAGIC_BINFMTFS: /* 0x42494E4D local */
      ফেরত "binfmt_misc";
    ক্ষেত্র S_MAGIC_BTRFS: /* 0x9123683E local */
      ফেরত "btrfs";
    ক্ষেত্র S_MAGIC_BTRFS_TEST: /* 0x73727279 local */
      ফেরত "btrfs_test";
    ক্ষেত্র S_MAGIC_CEPH: /* 0x00C36400 remote */
      ফেরত "ceph";
    ক্ষেত্র S_MAGIC_CGROUP: /* 0x0027E0EB local */
      ফেরত "cgroupfs";
    ক্ষেত্র S_MAGIC_CGROUP2: /* 0x63677270 local */
      ফেরত "cgroup2fs";
    ক্ষেত্র S_MAGIC_CIFS: /* 0xFF534D42 remote */
      ফেরত "cifs";
    ক্ষেত্র S_MAGIC_CODA: /* 0x73757245 remote */
      ফেরত "coda";
    ক্ষেত্র S_MAGIC_COH: /* 0x012FF7B7 local */
      ফেরত "coh";
    ক্ষেত্র S_MAGIC_CONFIGFS: /* 0x62656570 local */
      ফেরত "configfs";
    ক্ষেত্র S_MAGIC_CRAMFS: /* 0x28CD3D45 local */
      ফেরত "cramfs";
    ক্ষেত্র S_MAGIC_CRAMFS_WEND: /* 0x453DCD28 local */
      ফেরত "cramfs-wend";
    ক্ষেত্র S_MAGIC_DAXFS: /* 0x64646178 local */
      ফেরত "daxfs";
    ক্ষেত্র S_MAGIC_DEBUGFS: /* 0x64626720 local */
      ফেরত "debugfs";
    ক্ষেত্র S_MAGIC_DEVFS: /* 0x1373 local */
      ফেরত "devfs";
    ক্ষেত্র S_MAGIC_DEVMEM: /* 0x454D444D local */
      ফেরত "devmem";
    ক্ষেত্র S_MAGIC_DEVPTS: /* 0x1CD1 local */
      ফেরত "devpts";
    ক্ষেত্র S_MAGIC_DMA_BUF: /* 0x444D4142 local */
      ফেরত "dma-buf-fs";
    ক্ষেত্র S_MAGIC_ECRYPTFS: /* 0xF15F local */
      ফেরত "ecryptfs";
    ক্ষেত্র S_MAGIC_EFIVARFS: /* 0xDE5E81E4 local */
      ফেরত "efivarfs";
    ক্ষেত্র S_MAGIC_EFS: /* 0x00414A53 local */
      ফেরত "efs";
    ক্ষেত্র S_MAGIC_EROFS_V1: /* 0xE0F5E1E2 local */
      ফেরত "erofs";
    ক্ষেত্র S_MAGIC_EXFAT: /* 0x2011BAB0 local */
      ফেরত "exfat";
    ক্ষেত্র S_MAGIC_EXFS: /* 0x45584653 local */
      ফেরত "exfs";
    ক্ষেত্র S_MAGIC_EXOFS: /* 0x5DF5 local */
      ফেরত "exofs";
    ক্ষেত্র S_MAGIC_EXT: /* 0x137D local */
      ফেরত "ext";
    ক্ষেত্র S_MAGIC_EXT2: /* 0xEF53 local */
      ফেরত "ext2/ext3";
    ক্ষেত্র S_MAGIC_EXT2_OLD: /* 0xEF51 local */
      ফেরত "ext2";
    ক্ষেত্র S_MAGIC_F2FS: /* 0xF2F52010 local */
      ফেরত "f2fs";
    ক্ষেত্র S_MAGIC_FAIL_FS: /* 0x4641494C local */
      ফেরত "failfs";
    ক্ষেত্র S_MAGIC_FAT: /* 0x4006 local */
      ফেরত "fat";
    ক্ষেত্র S_MAGIC_FHGFS: /* 0x19830326 remote */
      ফেরত "fhgfs";
    ক্ষেত্র S_MAGIC_FUSE: /* 0x65735546 remote */
      ফেরত "fuse";
    ক্ষেত্র S_MAGIC_FUSECTL: /* 0x65735543 remote */
      ফেরত "fusectl";
    ক্ষেত্র S_MAGIC_FUTEXFS: /* 0x0BAD1DEA local */
      ফেরত "futexfs";
    ক্ষেত্র S_MAGIC_GFS: /* 0x01161970 remote */
      ফেরত "gfs/gfs2";
    ক্ষেত্র S_MAGIC_GPFS: /* 0x47504653 remote */
      ফেরত "gpfs";
    ক্ষেত্র S_MAGIC_GUEST_MEMFD: /* 0x474D454D remote */
      ফেরত "guest-memfd";
    ক্ষেত্র S_MAGIC_HFS: /* 0x4244 local */
      ফেরত "hfs";
    ক্ষেত্র S_MAGIC_HFS_PLUS: /* 0x482B local */
      ফেরত "hfs+";
    ক্ষেত্র S_MAGIC_HFS_X: /* 0x4858 local */
      ফেরত "hfsx";
    ক্ষেত্র S_MAGIC_HOSTFS: /* 0x00C0FFEE local */
      ফেরত "hostfs";
    ক্ষেত্র S_MAGIC_HPFS: /* 0xF995E849 local */
      ফেরত "hpfs";
    ক্ষেত্র S_MAGIC_HUGETLBFS: /* 0x958458F6 local */
      ফেরত "hugetlbfs";
    ক্ষেত্র S_MAGIC_MTD_INODE_FS: /* 0x11307854 local */
      ফেরত "inodefs";
    ক্ষেত্র S_MAGIC_IBRIX: /* 0x013111A8 remote */
      ফেরত "ibrix";
    ক্ষেত্র S_MAGIC_INOTIFYFS: /* 0x2BAD1DEA local */
      ফেরত "inotifyfs";
    ক্ষেত্র S_MAGIC_ISOFS: /* 0x9660 local */
      ফেরত "isofs";
    ক্ষেত্র S_MAGIC_ISOFS_R_WIN: /* 0x4004 local */
      ফেরত "isofs";
    ক্ষেত্র S_MAGIC_ISOFS_WIN: /* 0x4000 local */
      ফেরত "isofs";
    ক্ষেত্র S_MAGIC_JFFS: /* 0x07C0 local */
      ফেরত "jffs";
    ক্ষেত্র S_MAGIC_JFFS2: /* 0x72B6 local */
      ফেরত "jffs2";
    ক্ষেত্র S_MAGIC_JFS: /* 0x3153464A local */
      ফেরত "jfs";
    ক্ষেত্র S_MAGIC_KAFS: /* 0x6B414653 remote */
      ফেরত "k-afs";
    ক্ষেত্র S_MAGIC_LOGFS: /* 0xC97E8168 local */
      ফেরত "logfs";
    ক্ষেত্র S_MAGIC_LUSTRE: /* 0x0BD00BD0 remote */
      ফেরত "lustre";
    ক্ষেত্র S_MAGIC_M1FS: /* 0x5346314D local */
      ফেরত "m1fs";
    ক্ষেত্র S_MAGIC_MINIX: /* 0x137F local */
      ফেরত "minix";
    ক্ষেত্র S_MAGIC_MINIX_30: /* 0x138F local */
      ফেরত "minix (30 char.)";
    ক্ষেত্র S_MAGIC_MINIX_V2: /* 0x2468 local */
      ফেরত "minix v2";
    ক্ষেত্র S_MAGIC_MINIX_V2_30: /* 0x2478 local */
      ফেরত "minix v2 (30 char.)";
    ক্ষেত্র S_MAGIC_MINIX_V3: /* 0x4D5A local */
      ফেরত "minix3";
    ক্ষেত্র S_MAGIC_MQUEUE: /* 0x19800202 local */
      ফেরত "mqueue";
    ক্ষেত্র S_MAGIC_MSDOS: /* 0x4D44 local */
      ফেরত "msdos";
    ক্ষেত্র S_MAGIC_NCP: /* 0x564C remote */
      ফেরত "novell";
    ক্ষেত্র S_MAGIC_NFS: /* 0x6969 remote */
      ফেরত "nfs";
    ক্ষেত্র S_MAGIC_NFSD: /* 0x6E667364 remote */
      ফেরত "nfsd";
    ক্ষেত্র S_MAGIC_NILFS: /* 0x3434 local */
      ফেরত "nilfs";
    ক্ষেত্র S_MAGIC_NSFS: /* 0x6E736673 local */
      ফেরত "nsfs";
    ক্ষেত্র S_MAGIC_NTFS: /* 0x5346544E local */
      ফেরত "ntfs";
    ক্ষেত্র S_MAGIC_NULL_FS: /* 0x4E554C4C local */
      ফেরত "nullfs";
    ক্ষেত্র S_MAGIC_OPENPROM: /* 0x9FA1 local */
      ফেরত "openprom";
    ক্ষেত্র S_MAGIC_OCFS2: /* 0x7461636F remote */
      ফেরত "ocfs2";
    ক্ষেত্র S_MAGIC_OVERLAYFS: /* 0x794C7630 remote */
      /* This may overlay remote file systems.
         Also there have been issues reported with inotify and overlayfs,
         so mark as "remote" so that polling is used.  */
      ফেরত "overlayfs";
    ক্ষেত্র S_MAGIC_PANFS: /* 0xAAD7AAEA remote */
      ফেরত "panfs";
    ক্ষেত্র S_MAGIC_PID_FS: /* 0x50494446 local */
      ফেরত "pidfs";
    ক্ষেত্র S_MAGIC_PIPEFS: /* 0x50495045 remote */
      /* FIXME: change syntax or add an optional attribute like "inotify:no".
         pipefs is labeled as "remote" so that tail always polls,
         but it isn't really remote file system type.  */
      ফেরত "pipefs";
    ক্ষেত্র S_MAGIC_PPC_CMM: /* 0xC7571590 local */
      ফেরত "ppc-cmm-fs";
    ক্ষেত্র S_MAGIC_PRL_FS: /* 0x7C7C6673 remote */
      ফেরত "prl_fs";
    ক্ষেত্র S_MAGIC_PROC: /* 0x9FA0 local */
      ফেরত "proc";
    ক্ষেত্র S_MAGIC_PSTOREFS: /* 0x6165676C local */
      ফেরত "pstorefs";
    ক্ষেত্র S_MAGIC_QNX4: /* 0x002F local */
      ফেরত "qnx4";
    ক্ষেত্র S_MAGIC_QNX6: /* 0x68191122 local */
      ফেরত "qnx6";
    ক্ষেত্র S_MAGIC_RAMFS: /* 0x858458F6 local */
      ফেরত "ramfs";
    ক্ষেত্র S_MAGIC_RDTGROUP: /* 0x07655821 local */
      ফেরত "rdt";
    ক্ষেত্র S_MAGIC_REISERFS: /* 0x52654973 local */
      ফেরত "reiserfs";
    ক্ষেত্র S_MAGIC_ROMFS: /* 0x7275 local */
      ফেরত "romfs";
    ক্ষেত্র S_MAGIC_RPC_PIPEFS: /* 0x67596969 local */
      ফেরত "rpc_pipefs";
    ক্ষেত্র S_MAGIC_SDCARDFS: /* 0x5DCA2DF5 local */
      ফেরত "sdcardfs";
    ক্ষেত্র S_MAGIC_SECRETMEM: /* 0x5345434D local */
      ফেরত "secretmem";
    ক্ষেত্র S_MAGIC_SECURITYFS: /* 0x73636673 local */
      ফেরত "securityfs";
    ক্ষেত্র S_MAGIC_SELINUX: /* 0xF97CFF8C local */
      ফেরত "selinux";
    ক্ষেত্র S_MAGIC_SMACK: /* 0x43415D53 local */
      ফেরত "smackfs";
    ক্ষেত্র S_MAGIC_SMB: /* 0x517B remote */
      ফেরত "smb";
    ক্ষেত্র S_MAGIC_SMB2: /* 0xFE534D42 remote */
      ফেরত "smb2";
    ক্ষেত্র S_MAGIC_SNFS: /* 0xBEEFDEAD remote */
      ফেরত "snfs";
    ক্ষেত্র S_MAGIC_SOCKFS: /* 0x534F434B local */
      ফেরত "sockfs";
    ক্ষেত্র S_MAGIC_SQUASHFS: /* 0x73717368 local */
      ফেরত "squashfs";
    ক্ষেত্র S_MAGIC_SYSFS: /* 0x62656572 local */
      ফেরত "sysfs";
    ক্ষেত্র S_MAGIC_SYSV2: /* 0x012FF7B6 local */
      ফেরত "sysv2";
    ক্ষেত্র S_MAGIC_SYSV4: /* 0x012FF7B5 local */
      ফেরত "sysv4";
    ক্ষেত্র S_MAGIC_TMPFS: /* 0x01021994 local */
      ফেরত "tmpfs";
    ক্ষেত্র S_MAGIC_TRACEFS: /* 0x74726163 local */
      ফেরত "tracefs";
    ক্ষেত্র S_MAGIC_UBIFS: /* 0x24051905 local */
      ফেরত "ubifs";
    ক্ষেত্র S_MAGIC_UDF: /* 0x15013346 local */
      ফেরত "udf";
    ক্ষেত্র S_MAGIC_UFS: /* 0x00011954 local */
      ফেরত "ufs";
    ক্ষেত্র S_MAGIC_UFS_BYTESWAPPED: /* 0x54190100 local */
      ফেরত "ufs";
    ক্ষেত্র S_MAGIC_USBDEVFS: /* 0x9FA2 local */
      ফেরত "usbdevfs";
    ক্ষেত্র S_MAGIC_V9FS: /* 0x01021997 local */
      ফেরত "v9fs";
    ক্ষেত্র S_MAGIC_VBOXSF: /* 0x786F4256 remote */
      ফেরত "vboxsf";
    ক্ষেত্র S_MAGIC_VMHGFS: /* 0xBACBACBC remote */
      ফেরত "vmhgfs";
    ক্ষেত্র S_MAGIC_VXFS: /* 0xA501FCF5 remote */
      /* Veritas File System can run in single instance or clustered mode,
         so mark as remote to cater for the latter case.  */
      ফেরত "vxfs";
    ক্ষেত্র S_MAGIC_VZFS: /* 0x565A4653 local */
      ফেরত "vzfs";
    ক্ষেত্র S_MAGIC_WSLFS: /* 0x53464846 local */
      ফেরত "wslfs";
    ক্ষেত্র S_MAGIC_XENFS: /* 0xABBA1974 local */
      ফেরত "xenfs";
    ক্ষেত্র S_MAGIC_XENIX: /* 0x012FF7B4 local */
      ফেরত "xenix";
    ক্ষেত্র S_MAGIC_XFS: /* 0x58465342 local */
      ফেরত "xfs";
    ক্ষেত্র S_MAGIC_XIAFS: /* 0x012FD16D local */
      ফেরত "xia";
    ক্ষেত্র S_MAGIC_Z3FOLD: /* 0x0033 local */
      ফেরত "z3fold";
    ক্ষেত্র S_MAGIC_ZFS: /* 0x2FC12FC1 local */
      ফেরত "zfs";
    ক্ষেত্র S_MAGIC_ZONEFS: /* 0x5A4F4653 local */
      ফেরত "zonefs";
    ক্ষেত্র S_MAGIC_ZSMALLOC: /* 0x58295829 local */
      ফেরত "zsmallocfs";


# elif __GNU__
    ক্ষেত্র FSTYPE_UFS:
      ফেরত "ufs";
    ক্ষেত্র FSTYPE_NFS:
      ফেরত "nfs";
    ক্ষেত্র FSTYPE_GFS:
      ফেরত "gfs";
    ক্ষেত্র FSTYPE_LFS:
      ফেরত "lfs";
    ক্ষেত্র FSTYPE_SYSV:
      ফেরত "sysv";
    ক্ষেত্র FSTYPE_FTP:
      ফেরত "ftp";
    ক্ষেত্র FSTYPE_TAR:
      ফেরত "tar";
    ক্ষেত্র FSTYPE_AR:
      ফেরত "ar";
    ক্ষেত্র FSTYPE_CPIO:
      ফেরত "cpio";
    ক্ষেত্র FSTYPE_MSLOSS:
      ফেরত "msloss";
    ক্ষেত্র FSTYPE_CPM:
      ফেরত "cpm";
    ক্ষেত্র FSTYPE_HFS:
      ফেরত "hfs";
    ক্ষেত্র FSTYPE_DTFS:
      ফেরত "dtfs";
    ক্ষেত্র FSTYPE_GRFS:
      ফেরত "grfs";
    ক্ষেত্র FSTYPE_TERM:
      ফেরত "term";
    ক্ষেত্র FSTYPE_DEV:
      ফেরত "dev";
    ক্ষেত্র FSTYPE_PROC:
      ফেরত "proc";
    ক্ষেত্র FSTYPE_IFSOCK:
      ফেরত "ifsock";
    ক্ষেত্র FSTYPE_AFS:
      ফেরত "afs";
    ক্ষেত্র FSTYPE_DFS:
      ফেরত "dfs";
    ক্ষেত্র FSTYPE_PROC9:
      ফেরত "proc9";
    ক্ষেত্র FSTYPE_SOCKET:
      ফেরত "socket";
    ক্ষেত্র FSTYPE_MISC:
      ফেরত "misc";
    ক্ষেত্র FSTYPE_EXT2FS:
      ফেরত "ext2/ext3";
    ক্ষেত্র FSTYPE_HTTP:
      ফেরত "http";
    ক্ষেত্র FSTYPE_MEMFS:
      ফেরত "memfs";
    ক্ষেত্র FSTYPE_ISO9660:
      ফেরত "iso9660";
# endif
    পূর্বনির্ধারিত:
      {
        unsigned long পূর্ণসংখ্যা type = statfsbuf->f_type;
        স্থির অক্ষর buf[আকার "UNKNOWN (0x%lx)" - 3
                        + (আকার type * CHAR_BIT + 3) / 4];
        sprintf (buf, "UNKNOWN (0x%lx)", type);
        ফেরত buf;
      }
    }
#endif
}

NODISCARD
স্থির অক্ষর *
human_access (কাঠামো stat ধ্রুবক *statbuf)
{
  স্থির অক্ষর modebuf[12];
  filemodestring (statbuf, modebuf);
  modebuf[10] = 0;
  ফেরত modebuf;
}

NODISCARD
স্থির অক্ষর *
human_time (কাঠামো timespec t)
{
  /* STR must be at least INT_BUFSIZE_BOUND (intmax_t) big, either
     because localtime_rz fails, or because the time zone is truly
     outlandish so that %z expands to a long string.  */
  স্থির অক্ষর str[INT_BUFSIZE_BOUND (intmax_t)
                  + INT_STRLEN_BOUND (পূর্ণসংখ্যা) /* YYYY */
                  + 1 /* because YYYY might equal INT_MAX + 1900 */
                  + আকার "-MM-DD HH:MM:SS.NNNNNNNNN +"];
  স্থির timezone_t tz;
  যদি (!tz)
    tz = tzalloc (getenv ("TZ"));
  কাঠামো tm tm;
  পূর্ণসংখ্যা ns = t.tv_nsec;
  যদি (localtime_rz (tz, &t.tv_sec, &tm))
    nstrftime (str, আকার str, "%Y-%m-%d %H:%M:%S.%N %z", &tm, tz, ns);
  অন্যথা
    {
      অক্ষর secbuf[INT_BUFSIZE_BOUND (intmax_t)];
      sprintf (str, "%s.%09d", timetostr (t.tv_sec, secbuf), ns);
    }
  ফেরত str;
}

/* PFORMAT points to a '%' followed by a prefix of a format, all of
   size PREFIX_LEN.  The flags allowed for this format are
   ALLOWED_FLAGS; remove other printf flags from the prefix, then
   append SUFFIX.  */
স্থির শূন্য
make_format (অক্ষর *pformat, size_t prefix_len, অক্ষর ধ্রুবক *allowed_flags,
             অক্ষর ধ্রুবক *suffix)
{
  অক্ষর *dst = pformat + 1;
  অক্ষর ধ্রুবক *src;
  অক্ষর ধ্রুবক *srclim = pformat + prefix_len;
  জন্য (src = dst; src < srclim && strchr (printf_flags, *src); src++)
    যদি (strchr (allowed_flags, *src))
      *dst++ = *src;
  যখন (src < srclim)
    *dst++ = *src++;
  strcpy (dst, suffix);
}

স্থির শূন্য
out_string (অক্ষর *pformat, size_t prefix_len, অক্ষর ধ্রুবক *arg)
{
  make_format (pformat, prefix_len, "-", "s");
  printf (pformat, arg);
}
স্থির পূর্ণসংখ্যা
out_int (অক্ষর *pformat, size_t prefix_len, intmax_t arg)
{
  make_format (pformat, prefix_len, "'-+ 0", "jd");
  ফেরত printf (pformat, arg);
}
স্থির পূর্ণসংখ্যা
out_uint (অক্ষর *pformat, size_t prefix_len, uintmax_t arg)
{
  make_format (pformat, prefix_len, "'-0", "ju");
  ফেরত printf (pformat, arg);
}
স্থির শূন্য
out_uint_o (অক্ষর *pformat, size_t prefix_len, uintmax_t arg)
{
  make_format (pformat, prefix_len, "-#0", "jo");
  printf (pformat, arg);
}
স্থির শূন্য
out_uint_x (অক্ষর *pformat, size_t prefix_len, uintmax_t arg)
{
  make_format (pformat, prefix_len, "-#0", "jx");
  printf (pformat, arg);
}
স্থির পূর্ণসংখ্যা
out_minus_zero (অক্ষর *pformat, size_t prefix_len)
{
  make_format (pformat, prefix_len, "'-+ 0", ".0f");
  ফেরত printf (pformat, -0.25);
}

/* Output the number of seconds since the Epoch, using a format that
   acts like printf's %f format.  */
স্থির শূন্য
out_epoch_sec (অক্ষর *pformat, size_t prefix_len,
               কাঠামো timespec arg)
{
  অক্ষর *dot = memchr (pformat, '.', prefix_len);
  size_t sec_prefix_len = prefix_len;
  পূর্ণসংখ্যা width = 0;
  পূর্ণসংখ্যা precision = 0;
  bool frac_বাম_adjust = false;

  যদি (dot)
    {
      sec_prefix_len = dot - pformat;
      pformat[prefix_len] = '\0';

      যদি (c_isdigit (dot[1]))
        {
          long পূর্ণসংখ্যা lprec = strtol (dot + 1, ফাঁকা, 10);
          precision = (lprec <= INT_MAX ? lprec : INT_MAX);
        }
      অন্যথা
        {
          precision = 9;
        }

      যদি (precision && c_isdigit (dot[-1]))
        {
          /* If a nontrivial width is given, subtract the width of the
             decimal point and PRECISION digits that will be output
             later.  */
          অক্ষর *p = dot;
          *dot = '\0';

          করো
            --p;
          যখন (c_isdigit (p[-1]));

          long পূর্ণসংখ্যা lwidth = strtol (p, ফাঁকা, 10);
          width = (lwidth <= INT_MAX ? lwidth : INT_MAX);
          যদি (1 < width)
            {
              p += (*p == '0');
              sec_prefix_len = p - pformat;
              পূর্ণসংখ্যা w_d = (decimal_point_len < width
                         ? width - decimal_point_len
                         : 0);
              যদি (1 < w_d)
                {
                  পূর্ণসংখ্যা w = w_d - precision;
                  যদি (1 < w)
                    {
                      অক্ষর *dst = pformat;
                      জন্য (অক্ষর ধ্রুবক *src = dst; src < p; src++)
                        {
                          যদি (*src == '-')
                            frac_বাম_adjust = true;
                          অন্যথা
                            *dst++ = *src;
                        }
                      sec_prefix_len =
                        (dst - pformat
                         + (frac_বাম_adjust ? 0 : sprintf (dst, "%d", w)));
                    }
                }
            }
        }
    }

  পূর্ণসংখ্যা ভাজক = 1;
  জন্য (পূর্ণসংখ্যা i = precision; i < 9; i++)
    ভাজক *= 10;
  পূর্ণসংখ্যা frac_sec = arg.tv_nsec / ভাজক;
  পূর্ণসংখ্যা int_len;

  যদি (TYPE_SIGNED (time_t))
    {
      bool minus_zero = false;
      যদি (arg.tv_sec < 0 && arg.tv_nsec != 0)
        {
          পূর্ণসংখ্যা frac_sec_modulus = 1000000000 / ভাজক;
          frac_sec = (frac_sec_modulus - frac_sec
                      - (arg.tv_nsec % ভাজক != 0));
          arg.tv_sec += (frac_sec != 0);
          minus_zero = (arg.tv_sec == 0);
        }
      int_len = (minus_zero
                 ? out_minus_zero (pformat, sec_prefix_len)
                 : out_int (pformat, sec_prefix_len, arg.tv_sec));
    }
  অন্যথা
    int_len = out_uint (pformat, sec_prefix_len, arg.tv_sec);

  যদি (precision)
    {
      পূর্ণসংখ্যা prec = (precision < 9 ? precision : 9);
      পূর্ণসংখ্যা trailing_prec = precision - prec;
      পূর্ণসংখ্যা ilen = (int_len < 0 ? 0 : int_len);
      পূর্ণসংখ্যা trailing_width = (ilen < width && decimal_point_len < width - ilen
                            ? width - ilen - decimal_point_len - prec
                            : 0);
      printf ("%s%.*d%-*.*d", decimal_point, prec, frac_sec,
              trailing_width, trailing_prec, 0);
    }
}

/* Print the context information of FILENAME, and return true iff the
   context could not be obtained.  */
NODISCARD
স্থির bool
out_file_context (অক্ষর *pformat, size_t prefix_len, অক্ষর ধ্রুবক *filename)
{
  অক্ষর *scontext;
  bool fail = false;

  যদি ((follow_links
       ? getfilecon (filename, &scontext)
       : lgetfilecon (filename, &scontext)) < 0)
    {
      error (0, errno, _("failed to get security context of %s"),
             quoteaf (filename));
      scontext = ফাঁকা;
      fail = true;
    }
  strcpy (pformat + prefix_len, "s");
  printf (pformat, (scontext ? scontext : "?"));
  যদি (scontext)
    freecon (scontext);
  ফেরত fail;
}

/* Set the quoting style once it is needed.  */
স্থির শূন্য
initialize_quoting_style (শূন্য)
{
  স্থির bool initialized;
  যদি (!initialized)
    {
      initialized = true;
      পূর্ণসংখ্যা qs = getenv_quoting_style ();
      set_quoting_style (ফাঁকা, 0 <= qs ? qs : shell_escape_quoting_style);
    }
}

/* Print statfs info.  Return zero upon success, nonzero upon failure.  */
NODISCARD
স্থির bool
print_statfs (অক্ষর *pformat, size_t prefix_len, MAYBE_UNUSED অক্ষর mod, অক্ষর m,
              MAYBE_UNUSED পূর্ণসংখ্যা fd, অক্ষর ধ্রুবক *filename,
              শূন্য ধ্রুবক *তথ্য)
{
  STRUCT_STATVFS ধ্রুবক *statfsbuf = তথ্য;
  bool fail = false;

  নির্বাচন (m)
    {
    ক্ষেত্র 'n':
      যদি (mod == 'Q')
        {
          initialize_quoting_style ();
          filename = quoteN (filename);
        }
      out_string (pformat, prefix_len, filename);
      বিরতি;

    ক্ষেত্র 'i':
      {
#যদি STRUCT_STATXFS_F_FSID_IS_INTEGER
        uintmax_t fsid = statfsbuf->f_fsid;
#অন্যথা
        প্রকারলেখ unsigned পূর্ণসংখ্যা fsid_word;
        static_assert (alignof (STRUCT_STATVFS) % alignof (fsid_word) == 0);
        static_assert (offsetof (STRUCT_STATVFS, f_fsid) % alignof (fsid_word)
                       == 0);
        static_assert (আকার statfsbuf->f_fsid % alignof (fsid_word) == 0);
        fsid_word ধ্রুবক *p = (fsid_word *) &statfsbuf->f_fsid;

        /* Assume a little-endian word order, as that is compatible
           with glibc's statvfs implementation.  */
        uintmax_t fsid = 0;
        পূর্ণসংখ্যা শব্দগুলি = আকার statfsbuf->f_fsid / আকার *p;
        জন্য (পূর্ণসংখ্যা i = 0; i < শব্দগুলি && i * আকার *p < আকার fsid; i++)
          {
            uintmax_t u = p[শব্দগুলি - 1 - i];
            fsid |= u << (i * CHAR_BIT * আকার *p);
          }
#endif
        out_uint_x (pformat, prefix_len, fsid);
      }
      বিরতি;

    ক্ষেত্র 'l':
      OUT_NAMEMAX (pformat, prefix_len, SB_F_NAMEMAX (statfsbuf));
      বিরতি;
    ক্ষেত্র 't':
#যদি HAVE_STRUCT_STATXFS_F_TYPE
      out_uint_x (pformat, prefix_len, statfsbuf->f_type);
#অন্যথা
      fputc ('?', stdout);
#endif
      বিরতি;
    ক্ষেত্র 'T':
      out_string (pformat, prefix_len, human_fstype (statfsbuf));
      বিরতি;
    ক্ষেত্র 'b':
      out_int (pformat, prefix_len, statfsbuf->f_blocks);
      বিরতি;
    ক্ষেত্র 'f':
      out_int (pformat, prefix_len, statfsbuf->f_bfree);
      বিরতি;
    ক্ষেত্র 'a':
      out_int (pformat, prefix_len, statfsbuf->f_bavail);
      বিরতি;
    ক্ষেত্র 's':
      out_uint (pformat, prefix_len, statfsbuf->f_bsize);
      বিরতি;
    ক্ষেত্র 'S':
      {
        uintmax_t frsize = STATFS_FRSIZE (statfsbuf);
        যদি (! frsize)
          frsize = statfsbuf->f_bsize;
        out_uint (pformat, prefix_len, frsize);
      }
      বিরতি;
    ক্ষেত্র 'c':
      out_uint (pformat, prefix_len, statfsbuf->f_files);
      বিরতি;
    ক্ষেত্র 'd':
      out_int (pformat, prefix_len, statfsbuf->f_ffree);
      বিরতি;
    পূর্বনির্ধারিত:
      fputc ('?', stdout);
      বিরতি;
    }
  ফেরত fail;
}

/* Return any bind mounted source for a path.
   The caller should not free the returned buffer.
   Return NULL if no bind mount found.  */
NODISCARD
স্থির অক্ষর ধ্রুবক *
find_bind_mount (অক্ষর ধ্রুবক * name)
{
  অক্ষর ধ্রুবক * bind_mount = ফাঁকা;

  স্থির কাঠামো mount_entry *mount_list;
  স্থির bool tried_mount_list = false;
  যদি (!tried_mount_list) /* attempt/warn once per process.  */
    {
      যদি (!(mount_list = read_file_system_list (false)))
        error (0, errno, "%s", _("cannot read table of mounted file systems"));
      tried_mount_list = true;
    }

  কাঠামো stat name_stats;
  যদি (stat (name, &name_stats) != 0)
    ফেরত ফাঁকা;

  জন্য (কাঠামো mount_entry *me = mount_list; me; me = me->me_পরবর্তী)
    {
      যদি (me->me_dummy && me->me_devname[0] == '/'
          && streq (me->me_mountdir, name))
        {
          কাঠামো stat dev_stats;

          যদি (stat (me->me_devname, &dev_stats) == 0
              && psame_inode (&name_stats, &dev_stats))
            {
              bind_mount = me->me_devname;
              বিরতি;
            }
        }
    }

  ফেরত bind_mount;
}

/* Print mount point.  Return zero upon success, nonzero upon failure.  */
NODISCARD
স্থির bool
out_mount_point (অক্ষর ধ্রুবক *filename, অক্ষর *pformat, size_t prefix_len,
                 ধ্রুবক কাঠামো stat *statp)
{

  অক্ষর ধ্রুবক *np = "?", *bp = ফাঁকা;
  অক্ষর *mp = ফাঁকা;
  bool fail = true;

  /* Look for bind mounts first.  Note we output the immediate alias,
     rather than further resolving to a base device mount point.  */
  যদি (follow_links || !S_ISLNK (statp->st_mode))
    {
      অক্ষর *resolved = canonicalize_file_name (filename);
      যদি (!resolved)
        {
          error (0, errno, _("failed to canonicalize %s"), quoteaf (filename));
          যাও print_mount_point;
        }
      bp = find_bind_mount (resolved);
      মুক্ত (resolved);
      যদি (bp)
        {
          fail = false;
          যাও print_mount_point;
        }
    }

  /* If there is no direct bind mount, then navigate
     back up the tree looking for a device change.
     Note we don't detect if any of the directory components
     are bind mounted to the same device, but that's OK
     since we've not directly queried them.  */
  যদি ((mp = find_mount_point (filename, statp)))
    {
      /* This dir might be bind mounted to another device,
         so we resolve the bound source in that case also.  */
      bp = find_bind_mount (mp);
      fail = false;
    }

print_mount_point:

  out_string (pformat, prefix_len, bp ? bp : mp ? mp : np);
  মুক্ত (mp);
  ফেরত fail;
}

/* Map a TS with negative TS.tv_nsec to {0,0}.  */
স্থির অন্তঃরেখ কাঠামো timespec
neg_to_zero (কাঠামো timespec ts)
{
  যদি (0 <= ts.tv_nsec)
    ফেরত ts;
  কাঠামো timespec z = {0};
  ফেরত z;
}

/* Output a single-character \ escape.  */

স্থির শূন্য
print_esc_char (অক্ষর c)
{
  নির্বাচন (c)
    {
    ক্ষেত্র 'a':			/* Alert. */
      c ='\a';
      বিরতি;
    ক্ষেত্র 'b':			/* Backspace. */
      c ='\b';
      বিরতি;
    ক্ষেত্র 'e':			/* Escape. */
      c ='\x1B';
      বিরতি;
    ক্ষেত্র 'f':			/* Form feed. */
      c ='\f';
      বিরতি;
    ক্ষেত্র 'n':			/* New line. */
      c ='\n';
      বিরতি;
    ক্ষেত্র 'r':			/* Carriage return. */
      c ='\r';
      বিরতি;
    ক্ষেত্র 't':			/* Horizontal tab. */
      c ='\t';
      বিরতি;
    ক্ষেত্র 'v':			/* Vertical tab. */
      c ='\v';
      বিরতি;
    ক্ষেত্র '"':
    ক্ষেত্র '\\':
      বিরতি;
    পূর্বনির্ধারিত:
      error (0, 0, _("warning: unrecognized escape '\\%c'"), c);
      বিরতি;
    }
  putchar (c);
}

ATTRIBUTE_PURE
স্থির size_t
format_code_offset (অক্ষর ধ্রুবক *directive)
{
  size_t len = strspn (directive + 1, printf_flags);
  অক্ষর ধ্রুবক *fmt_char = directive + len + 1;
  fmt_char += strspn (fmt_char, digits);
  যদি (*fmt_char == '.')
    fmt_char += 1 + strspn (fmt_char + 1, digits);
  ফেরত fmt_char - directive;
}

/* Print the information specified by the format string, FORMAT,
   calling PRINT_FUNC for each %-directive encountered.
   Return zero upon success, nonzero upon failure.  */
NODISCARD
স্থির bool
print_it (অক্ষর ধ্রুবক *format, পূর্ণসংখ্যা fd, অক্ষর ধ্রুবক *filename,
          bool (*print_func) (অক্ষর *, size_t, অক্ষর, অক্ষর,
                              পূর্ণসংখ্যা, অক্ষর ধ্রুবক *, শূন্য ধ্রুবক *),
          শূন্য ধ্রুবক *তথ্য)
{
  bool fail = false;

  /* Add 2 to accommodate our conversion of the stat '%s' format string
     to the longer printf '%llu' one.  */
  গণনা
    {
      MAX_ADDITIONAL_BYTES =
        (MAX (আকার "jd",
              MAX (আকার "jo", MAX (আকার "ju", আকার "jx")))
         - 1)
    };
  size_t n_alloc = strlen (format) + MAX_ADDITIONAL_BYTES + 1;
  অক্ষর *dest = xmalloc (n_alloc);
  জন্য (অক্ষর ধ্রুবক *b = format; *b; b++)
    {
      নির্বাচন (*b)
        {
        ক্ষেত্র '%':
          {
            size_t len = format_code_offset (b);
            অক্ষর fmt_char = *(b + len);
            অক্ষর mod_char = 0;
            memcpy (dest, b, len);
            b += len;

            নির্বাচন (fmt_char)
              {
              ক্ষেত্র '\0':
                --b;
                FALLTHROUGH;
              ক্ষেত্র '%':
                যদি (1 < len)
                  {
                    dest[len] = fmt_char;
                    dest[len + 1] = '\0';
                    error (EXIT_FAILURE, 0, _("%s: invalid directive"),
                           quote (dest));
                  }
                putchar ('%');
                বিরতি;
              ক্ষেত্র 'H':
              ক্ষেত্র 'L':
              ক্ষেত্র 'Q':
                mod_char = fmt_char;
                fmt_char = *(b + 1);
                যদি ((mod_char == 'Q' && fmt_char == 'n')
                    || (print_func == print_stat
                        && (mod_char == 'H' || mod_char == 'L')
                        && (fmt_char == 'd' || fmt_char == 'r')))
                  {
                    b++;
                  }
                অন্যথা
                  {
                    fmt_char = mod_char;
                    mod_char = 0;
                  }
                FALLTHROUGH;
              পূর্বনির্ধারিত:
                fail |= print_func (dest, len, mod_char, fmt_char,
                                    fd, filename, তথ্য);
                বিরতি;
              }
            বিরতি;
          }

        ক্ষেত্র '\\':
          যদি ( ! interpret_backslash_escapes)
            {
              putchar ('\\');
              বিরতি;
            }
          ++b;
          যদি (isoct (*b))
            {
              পূর্ণসংখ্যা esc_মান = fromoct (*b);
              পূর্ণসংখ্যা esc_দৈর্ঘ্য = 1;	/* number of octal digits */
              জন্য (++b; esc_দৈর্ঘ্য < 3 && isoct (*b);
                   ++esc_দৈর্ঘ্য, ++b)
                {
                  esc_মান = esc_মান * 8 + fromoct (*b);
                }
              putchar (esc_মান);
              --b;
            }
          অন্যথা যদি (*b == 'x' && c_isxdigit (b[1]))
            {
              পূর্ণসংখ্যা esc_মান = fromhex (b[1]);	/* Value of \xhh escape. */
              /* A hexadecimal \xhh escape sequence must have
                 1 or 2 hex. digits.  */
              ++b;
              যদি (c_isxdigit (b[1]))
                {
                  ++b;
                  esc_মান = esc_মান * 16 + fromhex (*b);
                }
              putchar (esc_মান);
            }
          অন্যথা যদি (*b == '\0')
            {
              error (0, 0, _("warning: backslash at end of format"));
              putchar ('\\');
              /* Arrange to exit the loop.  */
              --b;
            }
          অন্যথা
            {
              print_esc_char (*b);
            }
          বিরতি;

        পূর্বনির্ধারিত:
          putchar (*b);
          বিরতি;
        }
    }
  মুক্ত (dest);

  fputs (trailing_delim, stdout);

  ফেরত fail;
}

/* Stat the file system and print what we find.  */
NODISCARD
স্থির bool
do_statfs (অক্ষর ধ্রুবক *filename, অক্ষর ধ্রুবক *format)
{
  STRUCT_STATVFS statfsbuf;

  যদি (streq (filename, "-"))
    {
      error (0, 0, _("using %s to denote standard input does not work"
                     " in file system mode"), quoteaf (filename));
      ফেরত false;
    }

  যদি (STATFS (filename, &statfsbuf) != 0)
    {
      error (0, errno, _("cannot read file system information for %s"),
             quoteaf (filename));
      ফেরত false;
    }

  bool fail = print_it (format, -1, filename, print_statfs, &statfsbuf);
  ফেরত ! fail;
}

কাঠামো print_args {
  কাঠামো stat *st;
  কাঠামো timespec btime;
};

/* Ask statx to avoid syncing? */
স্থির bool dont_sync;

/* Ask statx to force sync? */
স্থির bool force_sync;

#যদি USE_STATX
স্থির unsigned পূর্ণসংখ্যা
fmt_to_mask (অক্ষর fmt)
{
  নির্বাচন (fmt)
    {
    ক্ষেত্র 'N':
      ফেরত STATX_MODE;
    ক্ষেত্র 'd':
    ক্ষেত্র 'D':
      ফেরত STATX_MODE;
    ক্ষেত্র 'i':
      ফেরত STATX_INO;
    ক্ষেত্র 'a':
    ক্ষেত্র 'A':
      ফেরত STATX_MODE;
    ক্ষেত্র 'f':
      ফেরত STATX_MODE|STATX_TYPE;
    ক্ষেত্র 'F':
      ফেরত STATX_TYPE;
    ক্ষেত্র 'h':
      ফেরত STATX_NLINK;
    ক্ষেত্র 'u':
    ক্ষেত্র 'U':
      ফেরত STATX_UID;
    ক্ষেত্র 'g':
    ক্ষেত্র 'G':
      ফেরত STATX_GID;
    ক্ষেত্র 'm':
      ফেরত STATX_MODE|STATX_INO;
    ক্ষেত্র 's':
      ফেরত STATX_আকার;
    ক্ষেত্র 't':
    ক্ষেত্র 'T':
      ফেরত STATX_MODE;
    ক্ষেত্র 'b':
      ফেরত STATX_BLOCKS;
    ক্ষেত্র 'w':
    ক্ষেত্র 'W':
      ফেরত STATX_BTIME;
    ক্ষেত্র 'x':
    ক্ষেত্র 'X':
      ফেরত STATX_ATIME;
    ক্ষেত্র 'y':
    ক্ষেত্র 'Y':
      ফেরত STATX_MTIME;
    ক্ষেত্র 'z':
    ক্ষেত্র 'Z':
      ফেরত STATX_CTIME;
    }
  ফেরত 0;
}

ATTRIBUTE_PURE
স্থির unsigned পূর্ণসংখ্যা
format_to_mask (অক্ষর ধ্রুবক *format)
{
  unsigned পূর্ণসংখ্যা mask = 0;

  জন্য (অক্ষর ধ্রুবক *b = format; *b; b++)
    {
      যদি (*b != '%')
        চালু;

      b += format_code_offset (b);
      যদি (*b == '\0')
        বিরতি;
      mask |= fmt_to_mask (*b);
    }
  ফেরত mask;
}

/* statx the file and print what we find */
NODISCARD
স্থির bool
do_stat (অক্ষর ধ্রুবক *filename, অক্ষর ধ্রুবক *format, অক্ষর ধ্রুবক *format2)
{
  পূর্ণসংখ্যা fd = streq (filename, "-") ? 0 : AT_FDCWD;
  পূর্ণসংখ্যা flags = 0;
  কাঠামো stat st;
  কাঠামো statx stx = {0};
  অক্ষর ধ্রুবক *pathname = filename;
  কাঠামো print_args pa;
  pa.st = &st;
  pa.btime = (কাঠামো timespec) {.tv_sec = -1, .tv_nsec = -1};

  যদি (AT_FDCWD != fd)
    {
      pathname = "";
      flags = AT_EMPTY_PATH;
    }
  অন্যথা যদি (!follow_links)
    {
      flags = AT_SYMLINK_NOFOLLOW;
    }

  যদি (dont_sync)
    flags |= AT_STATX_DONT_SYNC;
  অন্যথা যদি (force_sync)
    flags |= AT_STATX_FORCE_SYNC;

  যদি (! force_sync)
    flags |= AT_NO_AUTOMOUNT;

  fd = statx (fd, pathname, flags, format_to_mask (format), &stx);
  যদি (fd < 0)
    {
      যদি (flags & AT_EMPTY_PATH)
        error (0, errno, _("cannot stat standard input"));
      অন্যথা
        error (0, errno, _("cannot statx %s"), quoteaf (filename));
      ফেরত false;
    }

  যদি (S_ISBLK (stx.stx_mode) || S_ISCHR (stx.stx_mode))
    format = format2;

  statx_to_stat (&stx, &st);
  যদি (stx.stx_mask & STATX_BTIME)
    pa.btime = statx_timestamp_to_timespec (stx.stx_btime);

  bool fail = print_it (format, fd, filename, print_stat, &pa);
  ফেরত ! fail;
}

#অন্যথা /* USE_STATX */

স্থির কাঠামো timespec
get_birthtime (পূর্ণসংখ্যা fd, অক্ষর ধ্রুবক *filename, কাঠামো stat ধ্রুবক *st)
{
  কাঠামো timespec ts = get_stat_birthtime (st);

# যদি HAVE_GETATTRAT
  যদি (ts.tv_nsec < 0)
    {
      nvlist_t *response;
      যদি ((fd < 0
           ? getattrat (AT_FDCWD, XATTR_VIEW_READWRITE, filename, &response)
           : fgetattr (fd, XATTR_VIEW_READWRITE, &response))
          == 0)
        {
          uint64_t *val;
          uint_t n;
          যদি (nvlist_lookup_uint64_array (response, A_CRTIME, &val, &n) == 0
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

  ফেরত ts;
}


/* stat the file and print what we find */
NODISCARD
স্থির bool
do_stat (অক্ষর ধ্রুবক *filename, অক্ষর ধ্রুবক *format,
         অক্ষর ধ্রুবক *format2)
{
  পূর্ণসংখ্যা fd = streq (filename, "-") ? 0 : -1;
  কাঠামো stat statbuf;
  কাঠামো print_args pa;
  pa.st = &statbuf;
  pa.btime = (কাঠামো timespec) {.tv_sec = -1, .tv_nsec = -1};

  যদি (0 <= fd)
    {
      যদি (fstat (fd, &statbuf) != 0)
        {
          error (0, errno, _("cannot stat standard input"));
          ফেরত false;
        }
    }
  /* We can't use the shorter
     (follow_links?stat:lstat) (filename, &statbug)
     since stat might be a function-like macro.  */
  অন্যথা যদি ((follow_links
            ? stat (filename, &statbuf)
            : lstat (filename, &statbuf)) != 0)
    {
      error (0, errno, _("cannot stat %s"), quoteaf (filename));
      ফেরত false;
    }

  যদি (S_ISBLK (statbuf.st_mode) || S_ISCHR (statbuf.st_mode))
    format = format2;

  bool fail = print_it (format, fd, filename, print_stat, &pa);
  ফেরত ! fail;
}
#endif /* USE_STATX */

/* POSIX requires 'ls' to print file sizes without a sign, even
   when negative.  Be consistent with that.  */

স্থির uintmax_t
unsigned_file_আকার (off_t আকার_2)
{
  ফেরত আকার_2 + (আকার_2 < 0) * ((uintmax_t) OFF_T_MAX - OFF_T_MIN + 1);
}

/* Print stat info.  Return zero upon success, nonzero upon failure.  */
স্থির bool
print_stat (অক্ষর *pformat, size_t prefix_len, অক্ষর mod, অক্ষর m,
            MAYBE_UNUSED পূর্ণসংখ্যা fd, অক্ষর ধ্রুবক *filename, শূন্য ধ্রুবক *তথ্য)
{
  কাঠামো print_args *parg = (কাঠামো print_args *) তথ্য;
  কাঠামো stat *statbuf = parg->st;
  কাঠামো timespec btime = parg->btime;
  কাঠামো passwd *pw_ent;
  কাঠামো group *gw_ent;
  bool fail = false;

  নির্বাচন (m)
    {
    ক্ষেত্র 'n':
      যদি (mod == 'Q')
        {
          initialize_quoting_style ();
          filename = quoteN (filename);
        }
      out_string (pformat, prefix_len, filename);
      বিরতি;
    ক্ষেত্র 'N':
      initialize_quoting_style ();
      out_string (pformat, prefix_len, quoteN (filename));
      যদি (S_ISLNK (statbuf->st_mode))
        {
          অক্ষর *linkname = areadlink_with_আকার (filename, statbuf->st_আকার);
          যদি (linkname == ফাঁকা)
            {
              error (0, errno, _("cannot read symbolic link %s"),
                     quoteaf (filename));
              ফেরত true;
            }
          printf (" -> ");
          out_string (pformat, prefix_len, quoteN (linkname));
          মুক্ত (linkname);
        }
      বিরতি;
    ক্ষেত্র 'd':
      যদি (mod == 'H')
        out_uint (pformat, prefix_len, major (statbuf->st_dev));
      অন্যথা যদি (mod == 'L')
        out_uint (pformat, prefix_len, minor (statbuf->st_dev));
      অন্যথা
        out_uint (pformat, prefix_len, statbuf->st_dev);
      বিরতি;
    ক্ষেত্র 'D':
      out_uint_x (pformat, prefix_len, statbuf->st_dev);
      বিরতি;
    ক্ষেত্র 'i':
      out_uint (pformat, prefix_len, statbuf->st_ino);
      বিরতি;
    ক্ষেত্র 'a':
      out_uint_o (pformat, prefix_len, statbuf->st_mode & CHMOD_MODE_BITS);
      বিরতি;
    ক্ষেত্র 'A':
      out_string (pformat, prefix_len, human_access (statbuf));
      বিরতি;
    ক্ষেত্র 'f':
      out_uint_x (pformat, prefix_len, statbuf->st_mode);
      বিরতি;
    ক্ষেত্র 'F':
      out_string (pformat, prefix_len, file_type (statbuf));
      বিরতি;
    ক্ষেত্র 'h':
      out_uint (pformat, prefix_len, statbuf->st_nlink);
      বিরতি;
    ক্ষেত্র 'u':
      out_uint (pformat, prefix_len, statbuf->st_uid);
      বিরতি;
    ক্ষেত্র 'U':
      pw_ent = getpwuid (statbuf->st_uid);
      out_string (pformat, prefix_len,
                  pw_ent ? pw_ent->pw_name : "UNKNOWN");
      বিরতি;
    ক্ষেত্র 'g':
      out_uint (pformat, prefix_len, statbuf->st_gid);
      বিরতি;
    ক্ষেত্র 'G':
      gw_ent = getgrgid (statbuf->st_gid);
      out_string (pformat, prefix_len,
                  gw_ent ? gw_ent->gr_name : "UNKNOWN");
      বিরতি;
    ক্ষেত্র 'm':
      fail |= out_mount_point (filename, pformat, prefix_len, statbuf);
      বিরতি;
    ক্ষেত্র 's':
      out_uint (pformat, prefix_len, unsigned_file_আকার (statbuf->st_আকার));
      বিরতি;
    ক্ষেত্র 'r':
      যদি (mod == 'H')
        out_uint (pformat, prefix_len, major (statbuf->st_rdev));
      অন্যথা যদি (mod == 'L')
        out_uint (pformat, prefix_len, minor (statbuf->st_rdev));
      অন্যথা
        out_uint (pformat, prefix_len, statbuf->st_rdev);
      বিরতি;
    ক্ষেত্র 'R':
      out_uint_x (pformat, prefix_len, statbuf->st_rdev);
      বিরতি;
    ক্ষেত্র 't':
      out_uint_x (pformat, prefix_len, major (statbuf->st_rdev));
      বিরতি;
    ক্ষেত্র 'T':
      out_uint_x (pformat, prefix_len, minor (statbuf->st_rdev));
      বিরতি;
    ক্ষেত্র 'B':
      out_uint (pformat, prefix_len, ST_NBLOCKSIZE);
      বিরতি;
    ক্ষেত্র 'b':
      out_uint (pformat, prefix_len, STP_NBLOCKS (statbuf));
      বিরতি;
    ক্ষেত্র 'o':
      out_uint (pformat, prefix_len, STP_BLKSIZE (statbuf));
      বিরতি;
    ক্ষেত্র 'w':
      {
#যদি ! USE_STATX
        btime = get_birthtime (fd, filename, statbuf);
#endif
        যদি (btime.tv_nsec < 0)
          out_string (pformat, prefix_len, "-");
        অন্যথা
          out_string (pformat, prefix_len, human_time (btime));
      }
      বিরতি;
    ক্ষেত্র 'W':
      {
#যদি ! USE_STATX
        btime = get_birthtime (fd, filename, statbuf);
#endif
        out_epoch_sec (pformat, prefix_len, neg_to_zero (btime));
      }
      বিরতি;
    ক্ষেত্র 'x':
      out_string (pformat, prefix_len, human_time (get_stat_atime (statbuf)));
      বিরতি;
    ক্ষেত্র 'X':
      out_epoch_sec (pformat, prefix_len, get_stat_atime (statbuf));
      বিরতি;
    ক্ষেত্র 'y':
      out_string (pformat, prefix_len, human_time (get_stat_mtime (statbuf)));
      বিরতি;
    ক্ষেত্র 'Y':
      out_epoch_sec (pformat, prefix_len, get_stat_mtime (statbuf));
      বিরতি;
    ক্ষেত্র 'z':
      out_string (pformat, prefix_len, human_time (get_stat_ctime (statbuf)));
      বিরতি;
    ক্ষেত্র 'Z':
      out_epoch_sec (pformat, prefix_len, get_stat_ctime (statbuf));
      বিরতি;
    ক্ষেত্র 'C':
      fail |= out_file_context (pformat, prefix_len, filename);
      বিরতি;
    পূর্বনির্ধারিত:
      fputc ('?', stdout);
      বিরতি;
    }
  ফেরত fail;
}

/* Return an allocated format string in static storage that
   corresponds to whether FS and TERSE options were declared.  */
স্থির অক্ষর *
default_format (bool fs, bool terse, bool device)
{
  অক্ষর *format;
  যদি (fs)
    {
      যদি (terse)
        format = xstrdup (fmt_terse_fs);
      অন্যথা
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
  অন্যথা /* ! fs */
    {
      যদি (terse)
        {
          যদি (0 < is_selinux_enabled ())
            format = xstrdup (fmt_terse_selinux);
          অন্যথা
            format = xstrdup (fmt_terse_regular);
        }
      অন্যথা
        {
          অক্ষর *অস্থায়ী;
          /* TRANSLATORS: This string uses format specifiers from
             'stat --help' without --file-system, and NOT from printf.  */
          format = xstrdup (_("\
  File: %N\n\
  আকার_4: %-10s\tBlocks: %-10b IO Block: %-6o %F\n\
"));

          অস্থায়ী = format;
          যদি (device)
            {
              /* TRANSLATORS: This string uses format specifiers from
                 'stat --help' without --file-system, and NOT from printf.  */
              format = xasprintf ("%s%s", format, _("\
" "Device: %Hd,%Ld\tInode: %-10i  Links: %-5h Device type: %Hr,%Lr\n\
"));
            }
          অন্যথা
            {
              /* TRANSLATORS: This string uses format specifiers from
                 'stat --help' without --file-system, and NOT from printf.  */
              format = xasprintf ("%s%s", format, _("\
" "Device: %Hd,%Ld\tInode: %-10i  Links: %h\n\
"));
            }
          মুক্ত (অস্থায়ী);

          অস্থায়ী = format;
          /* TRANSLATORS: This string uses format specifiers from
             'stat --help' without --file-system, and NOT from printf.  */
          format = xasprintf ("%s%s", format, _("\
" "Access: (%04a/%10.10A)  Uid: (%5u/%8U)   Gid: (%5g/%8G)\n\
"));
          মুক্ত (অস্থায়ী);

          যদি (0 < is_selinux_enabled ())
            {
              অস্থায়ী = format;
              /* TRANSLATORS: This string uses format specifiers from
                 'stat --help' without --file-system, and NOT from printf.  */
              format = xasprintf ("%s%s", format, _("Context: %C\n"));
              মুক্ত (অস্থায়ী);
            }

          অস্থায়ী = format;
          format = xasprintf ("%s%s", format,
                              /* TRANSLATORS: This string uses format specifiers
                                 from 'stat --help' without --file-system, and
                                 NOT from printf.  */
                              _("Access: %x\n"
                                "Modify: %y\n"
                                "Change: %z\n"
                                " Birth: %w\n"));
          মুক্ত (অস্থায়ী);
        }
    }
  ফেরত format;
}

শূন্য
usage (পূর্ণসংখ্যা status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
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
         use the specified FORMAT instead of the পূর্বনির্ধারিত;\n\
         output a newline after each use of FORMAT\n\
"));
      oputs (_("\
      --printf=FORMAT\n\
         like --format, but interpret backslash escapes,\n\
         and করো not output a mandatory trailing newline;\n\
         যদি you want a newline, include \\n in FORMAT\n\
"));
      oputs (_("\
  -t, --terse\n\
         print the information in terse form\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);

      fputs (_("\n\
The MODE argument of --cached can be: always, never, or পূর্বনির্ধারিত.\n\
'always' will use cached attributes যদি available, যখন\n\
'never' will try to synchronize with the latest attributes, and\n\
'default' will leave it up to the underlying file system.\n\
"), stdout);

      fputs (_("\n\
The valid format sequences জন্য files (without --file-system):\n\
\n\
  %a   permission bits in octal (see '#' and '0' printf flags)\n\
  %A   permission bits and file type in human readable form\n\
  %b   সংখ্যা of blocks allocated (see %B)\n\
  %B   the আকার_2 in bytes of each block reported by %b\n\
  %C   SELinux security context string\n\
"), stdout);
      fputs (_("\
  %d   device সংখ্যা in decimal (st_dev)\n\
  %D   device সংখ্যা in hex (st_dev)\n\
  %Hd  major device সংখ্যা in decimal\n\
  %Ld  minor device সংখ্যা in decimal\n\
  %f   raw mode in hex\n\
  %F   file type\n\
  %g   group ID of owner\n\
  %G   group name of owner\n\
"), stdout);
      fputs (_("\
  %h   সংখ্যা of hard links\n\
  %i   inode সংখ্যা\n\
  %m   mount point\n\
  %n   file name\n\
  %Qn  quoted file name\n\
  %N   quoted file name with dereference যদি symbolic link\n\
  %o   optimal I/O transfer আকার_2 hint\n\
  %s   যোগ_2 আকার_2, in bytes\n\
  %r   device type in decimal (st_rdev)\n\
  %R   device type in hex (st_rdev)\n\
  %Hr  major device type in decimal, জন্য character/block device special files\n\
  %Lr  minor device type in decimal, জন্য character/block device special files\n\
  %t   major device type in hex, জন্য character/block device special files\n\
  %T   minor device type in hex, জন্য character/block device special files\n\
"), stdout);
      fputs (_("\
  %u   user ID of owner\n\
  %U   user name of owner\n\
  %w   time of file birth, human-readable; - যদি unknown\n\
  %W   time of file birth, seconds since Epoch; 0 যদি unknown\n\
  %x   time of last access, human-readable\n\
  %X   time of last access, seconds since Epoch\n\
  %y   time of last তথ্য modification, human-readable\n\
  %Y   time of last তথ্য modification, seconds since Epoch\n\
  %z   time of last status change, human-readable\n\
  %Z   time of last status change, seconds since Epoch\n\
\n\
"), stdout);

      fputs (_("\
Valid format sequences জন্য file systems:\n\
\n\
  %a   মুক্ত blocks available to non-superuser\n\
  %b   যোগ_2 তথ্য blocks in file system\n\
  %c   যোগ_2 file nodes in file system\n\
  %d   মুক্ত file nodes in file system\n\
  %f   মুক্ত blocks in file system\n\
"), stdout);
      fputs (_("\
  %i   file system ID in hex\n\
  %l   maximum দৈর্ঘ্য of filenames\n\
  %n   file name\n\
  %Qn  quoted file name\n\
  %s   block আকার_2 (জন্য faster transfers)\n\
  %S   fundamental block আকার_2 (জন্য block counts)\n\
  %t   file system type in hex\n\
  %T   file system type in human readable form\n\
"), stdout);

      printf (_("\n\
--terse is equivalent to the following FORMAT:\n\
    %s\
"),
#যদি HAVE_SELINUX_SELINUX_H
              fmt_terse_selinux
#অন্যথা
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

পূর্ণসংখ্যা
প্রধান (পূর্ণসংখ্যা argc, অক্ষর *argv[])
{
  পূর্ণসংখ্যা c;
  bool fs = false;
  bool terse = false;
  অক্ষর *format = ফাঁকা;
  অক্ষর *format2;
  bool ok = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  কাঠামো lconv ধ্রুবক *locale = localeconv ();
  decimal_point = (locale->decimal_point[0] ? locale->decimal_point : ".");
  decimal_point_len = strlen (decimal_point);

  atexit (close_stdout);

  যখন ((c = getopt_long (argc, argv, "c:fLt", long_options, ফাঁকা)) != -1)
    {
      নির্বাচন (c)
        {
        ক্ষেত্র PRINTF_OPTION:
          format = optarg;
          interpret_backslash_escapes = true;
          trailing_delim = "";
          বিরতি;

        ক্ষেত্র 'c':
          format = optarg;
          interpret_backslash_escapes = false;
          trailing_delim = "\n";
          বিরতি;

        ক্ষেত্র 'L':
          follow_links = true;
          বিরতি;

        ক্ষেত্র 'f':
          fs = true;
          বিরতি;

        ক্ষেত্র 't':
          terse = true;
          বিরতি;

        ক্ষেত্র 0:
          নির্বাচন (XARGMATCH ("--cached", optarg, cached_args, cached_modes))
            {
              ক্ষেত্র cached_never:
                force_sync = true;
                dont_sync = false;
                বিরতি;
              ক্ষেত্র cached_always:
                force_sync = false;
                dont_sync = true;
                বিরতি;
              ক্ষেত্র cached_default:
                force_sync = false;
                dont_sync = false;
            }
          বিরতি;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        পূর্বনির্ধারিত:
          usage (EXIT_FAILURE);
        }
    }

  যদি (argc == optind)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  যদি (format)
    {
      format2 = format;
    }
  অন্যথা
    {
      format = default_format (fs, terse, /* device= */ false);
      format2 = default_format (fs, terse, /* device= */ true);
    }

  জন্য (পূর্ণসংখ্যা i = optind; i < argc; i++)
    ok &= (fs
           ? do_statfs (argv[i], format)
           : do_stat (argv[i], format, format2));

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
