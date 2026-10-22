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
#নোচেদ্যদি HAVE_SYS_VFS_H
# include <sys/vfs.h>
#নোচেদ্যদি HAVE_SYS_MOUNT_H && HAVE_SYS_PARAM_H
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
#নোচেদ্যদি HAVE_OS_H /* BeOS */
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
#  define STRUCT_STATVFS সংরচনা statvfs64
#  define STATFS statvfs64
# অন্যথা
#  define STRUCT_STATVFS সংরচনা statvfs
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
স্থির পূর্ণাঙ্ক
statfs (বর্ণ ধ্রুব *filename, সংরচনা fs_info *buf)
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
      প্রত্যাগম -1;
    }
  /* If successful, buf->dev will be == device.  */
  প্রত্যাগম fs_stat_dev (device, buf);
}
#  define f_fsid dev
#  define f_blocks total_blocks
#  define f_bfree free_blocks
#  define f_bavail free_blocks
#  define f_bsize io_size
#  define f_files total_nodes
#  define f_ffree free_nodes
#  define STRUCT_STATVFS সংরচনা fs_info
#  define STRUCT_STATXFS_F_FSID_IS_INTEGER true
#  define STATFS_FRSIZE(S) ((S)->block_size)
# অন্যথা
#  define STRUCT_STATVFS সংরচনা statfs
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

স্থির বর্ণ ধ্রুব digits[] = "0123456789";

/* Flags that are portable for use in printf, for at least one
   conversion specifier; make_format removes non-portable flags as
   needed for particular specifiers.  The glibc 2.2 extension "I" is
   listed here; it is removed by make_format because it has undefined
   behavior elsewhere and because it is incompatible with
   out_epoch_sec.  */
স্থির বর্ণ ধ্রুব printf_flags[] = "'-+ #0I";

/* Formats for the --terse option.  */
স্থির বর্ণ ধ্রুব fmt_terse_fs[] = "%Qn %i %l %t %s %S %b %f %a %c %d\n";
স্থির বর্ণ ধ্রুব fmt_terse_regular[] = "%Qn %s %b %f %u %g %D %i %h %t %T"
                                        " %X %Y %Z %W %o\n";
স্থির বর্ণ ধ্রুব fmt_terse_selinux[] = "%Qn %s %b %f %u %g %D %i %h %t %T"
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

স্থির বর্ণ ধ্রুব *ধ্রুব cached_args[] =
{
  "default", "never", "always", রিক্ত
};

স্থির গণনা cached_mode ধ্রুব cached_modes[] =
{
  cached_default, cached_never, cached_always
};

স্থির সংরচনা option ধ্রুব long_options[] =
{
  {"dereference", no_argument, রিক্ত, 'L'},
  {"file-system", no_argument, রিক্ত, 'f'},
  {"format", required_argument, রিক্ত, 'c'},
  {"printf", required_argument, রিক্ত, PRINTF_OPTION},
  {"terse", no_argument, রিক্ত, 't'},
  {"cached", required_argument, রিক্ত, 0},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {রিক্ত, 0, রিক্ত, 0}
};

/* Whether to follow symbolic links;  True for --dereference (-L).  */
স্থির bool follow_links;

/* Whether to interpret backslash-escape sequences.
   True for --printf=FMT, not for --format=FMT (-c).  */
স্থির bool interpret_backslash_escapes;

/* The trailing delimiter string:
   "" for --printf=FMT, "\n" for --format=FMT (-c).  */
স্থির বর্ণ ধ্রুব *trailing_delim = "";

/* The representation of the decimal point in the current locale.  */
স্থির বর্ণ ধ্রুব *decimal_point;
স্থির size_t decimal_point_len;

স্থির bool
print_stat (বর্ণ *pformat, size_t prefix_len, বর্ণ mod, বর্ণ m,
            পূর্ণাঙ্ক fd, বর্ণ ধ্রুব *filename, শূন্য ধ্রুব *data);

/* Return the type of the specified file system.
   Some systems have statfvs.f_basetype[FSTYPSZ] (AIX, HP-UX, and Solaris).
   Others have statvfs.f_fstypename[_VFS_NAMELEN] (NetBSD 3.0).
   Others have statfs.f_fstypename[MFSNAMELEN] (NetBSD 1.5.2).
   Still others have neither and have to get by with f_type (GNU/Linux).
   But f_type may only exist in statfs (Cygwin).  */
NODISCARD
স্থির বর্ণ ধ্রুব *
human_fstype (STRUCT_STATVFS ধ্রুব *statfsbuf)
{
#ifdef STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME
  প্রত্যাগম statfsbuf->STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME;
#অন্যথা
  বিকল্প (statfsbuf->f_type)
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

    পক্ষ S_MAGIC_AAFS: /* 0x5A3C69F0 local */
      প্রত্যাগম "aafs";
    পক্ষ S_MAGIC_ACFS: /* 0x61636673 remote */
      প্রত্যাগম "acfs";
    পক্ষ S_MAGIC_ADFS: /* 0xADF5 local */
      প্রত্যাগম "adfs";
    পক্ষ S_MAGIC_AFFS: /* 0xADFF local */
      প্রত্যাগম "affs";
    পক্ষ S_MAGIC_AFS: /* 0x5346414F remote */
      প্রত্যাগম "afs";
    পক্ষ S_MAGIC_ANON_INODE_FS: /* 0x09041934 local */
      প্রত্যাগম "anon-inode FS";
    পক্ষ S_MAGIC_AUFS: /* 0x61756673 remote */
      /* FIXME: change syntax or add an optional attribute like "inotify:no".
         The above is labeled as "remote" so that tail always uses polling,
         but this isn't really a remote file system type.  */
      প্রত্যাগম "aufs";
    পক্ষ S_MAGIC_AUTOFS: /* 0x0187 local */
      প্রত্যাগম "autofs";
    পক্ষ S_MAGIC_BALLOON_KVM: /* 0x13661366 local */
      প্রত্যাগম "balloon-kvm-fs";
    পক্ষ S_MAGIC_BCACHEFS: /* 0xCA451A4E local */
      প্রত্যাগম "bcachefs";
    পক্ষ S_MAGIC_BEFS: /* 0x42465331 local */
      প্রত্যাগম "befs";
    পক্ষ S_MAGIC_BDEVFS: /* 0x62646576 local */
      প্রত্যাগম "bdevfs";
    পক্ষ S_MAGIC_BFS: /* 0x1BADFACE local */
      প্রত্যাগম "bfs";
    পক্ষ S_MAGIC_BINDERFS: /* 0x6C6F6F70 local */
      প্রত্যাগম "binderfs";
    পক্ষ S_MAGIC_BPF_FS: /* 0xCAFE4A11 local */
      প্রত্যাগম "bpf_fs";
    পক্ষ S_MAGIC_BINFMTFS: /* 0x42494E4D local */
      প্রত্যাগম "binfmt_misc";
    পক্ষ S_MAGIC_BTRFS: /* 0x9123683E local */
      প্রত্যাগম "btrfs";
    পক্ষ S_MAGIC_BTRFS_TEST: /* 0x73727279 local */
      প্রত্যাগম "btrfs_test";
    পক্ষ S_MAGIC_CEPH: /* 0x00C36400 remote */
      প্রত্যাগম "ceph";
    পক্ষ S_MAGIC_CGROUP: /* 0x0027E0EB local */
      প্রত্যাগম "cgroupfs";
    পক্ষ S_MAGIC_CGROUP2: /* 0x63677270 local */
      প্রত্যাগম "cgroup2fs";
    পক্ষ S_MAGIC_CIFS: /* 0xFF534D42 remote */
      প্রত্যাগম "cifs";
    পক্ষ S_MAGIC_CODA: /* 0x73757245 remote */
      প্রত্যাগম "coda";
    পক্ষ S_MAGIC_COH: /* 0x012FF7B7 local */
      প্রত্যাগম "coh";
    পক্ষ S_MAGIC_CONFIGFS: /* 0x62656570 local */
      প্রত্যাগম "configfs";
    পক্ষ S_MAGIC_CRAMFS: /* 0x28CD3D45 local */
      প্রত্যাগম "cramfs";
    পক্ষ S_MAGIC_CRAMFS_WEND: /* 0x453DCD28 local */
      প্রত্যাগম "cramfs-wend";
    পক্ষ S_MAGIC_DAXFS: /* 0x64646178 local */
      প্রত্যাগম "daxfs";
    পক্ষ S_MAGIC_DEBUGFS: /* 0x64626720 local */
      প্রত্যাগম "debugfs";
    পক্ষ S_MAGIC_DEVFS: /* 0x1373 local */
      প্রত্যাগম "devfs";
    পক্ষ S_MAGIC_DEVMEM: /* 0x454D444D local */
      প্রত্যাগম "devmem";
    পক্ষ S_MAGIC_DEVPTS: /* 0x1CD1 local */
      প্রত্যাগম "devpts";
    পক্ষ S_MAGIC_DMA_BUF: /* 0x444D4142 local */
      প্রত্যাগম "dma-buf-fs";
    পক্ষ S_MAGIC_ECRYPTFS: /* 0xF15F local */
      প্রত্যাগম "ecryptfs";
    পক্ষ S_MAGIC_EFIVARFS: /* 0xDE5E81E4 local */
      প্রত্যাগম "efivarfs";
    পক্ষ S_MAGIC_EFS: /* 0x00414A53 local */
      প্রত্যাগম "efs";
    পক্ষ S_MAGIC_EROFS_V1: /* 0xE0F5E1E2 local */
      প্রত্যাগম "erofs";
    পক্ষ S_MAGIC_EXFAT: /* 0x2011BAB0 local */
      প্রত্যাগম "exfat";
    পক্ষ S_MAGIC_EXFS: /* 0x45584653 local */
      প্রত্যাগম "exfs";
    পক্ষ S_MAGIC_EXOFS: /* 0x5DF5 local */
      প্রত্যাগম "exofs";
    পক্ষ S_MAGIC_EXT: /* 0x137D local */
      প্রত্যাগম "ext";
    পক্ষ S_MAGIC_EXT2: /* 0xEF53 local */
      প্রত্যাগম "ext2/ext3";
    পক্ষ S_MAGIC_EXT2_OLD: /* 0xEF51 local */
      প্রত্যাগম "ext2";
    পক্ষ S_MAGIC_F2FS: /* 0xF2F52010 local */
      প্রত্যাগম "f2fs";
    পক্ষ S_MAGIC_FAIL_FS: /* 0x4641494C local */
      প্রত্যাগম "failfs";
    পক্ষ S_MAGIC_FAT: /* 0x4006 local */
      প্রত্যাগম "fat";
    পক্ষ S_MAGIC_FHGFS: /* 0x19830326 remote */
      প্রত্যাগম "fhgfs";
    পক্ষ S_MAGIC_FUSE: /* 0x65735546 remote */
      প্রত্যাগম "fuse";
    পক্ষ S_MAGIC_FUSECTL: /* 0x65735543 remote */
      প্রত্যাগম "fusectl";
    পক্ষ S_MAGIC_FUTEXFS: /* 0x0BAD1DEA local */
      প্রত্যাগম "futexfs";
    পক্ষ S_MAGIC_GFS: /* 0x01161970 remote */
      প্রত্যাগম "gfs/gfs2";
    পক্ষ S_MAGIC_GPFS: /* 0x47504653 remote */
      প্রত্যাগম "gpfs";
    পক্ষ S_MAGIC_GUEST_MEMFD: /* 0x474D454D remote */
      প্রত্যাগম "guest-memfd";
    পক্ষ S_MAGIC_HFS: /* 0x4244 local */
      প্রত্যাগম "hfs";
    পক্ষ S_MAGIC_HFS_PLUS: /* 0x482B local */
      প্রত্যাগম "hfs+";
    পক্ষ S_MAGIC_HFS_X: /* 0x4858 local */
      প্রত্যাগম "hfsx";
    পক্ষ S_MAGIC_HOSTFS: /* 0x00C0FFEE local */
      প্রত্যাগম "hostfs";
    পক্ষ S_MAGIC_HPFS: /* 0xF995E849 local */
      প্রত্যাগম "hpfs";
    পক্ষ S_MAGIC_HUGETLBFS: /* 0x958458F6 local */
      প্রত্যাগম "hugetlbfs";
    পক্ষ S_MAGIC_MTD_INODE_FS: /* 0x11307854 local */
      প্রত্যাগম "inodefs";
    পক্ষ S_MAGIC_IBRIX: /* 0x013111A8 remote */
      প্রত্যাগম "ibrix";
    পক্ষ S_MAGIC_INOTIFYFS: /* 0x2BAD1DEA local */
      প্রত্যাগম "inotifyfs";
    পক্ষ S_MAGIC_ISOFS: /* 0x9660 local */
      প্রত্যাগম "isofs";
    পক্ষ S_MAGIC_ISOFS_R_WIN: /* 0x4004 local */
      প্রত্যাগম "isofs";
    পক্ষ S_MAGIC_ISOFS_WIN: /* 0x4000 local */
      প্রত্যাগম "isofs";
    পক্ষ S_MAGIC_JFFS: /* 0x07C0 local */
      প্রত্যাগম "jffs";
    পক্ষ S_MAGIC_JFFS2: /* 0x72B6 local */
      প্রত্যাগম "jffs2";
    পক্ষ S_MAGIC_JFS: /* 0x3153464A local */
      প্রত্যাগম "jfs";
    পক্ষ S_MAGIC_KAFS: /* 0x6B414653 remote */
      প্রত্যাগম "k-afs";
    পক্ষ S_MAGIC_LOGFS: /* 0xC97E8168 local */
      প্রত্যাগম "logfs";
    পক্ষ S_MAGIC_LUSTRE: /* 0x0BD00BD0 remote */
      প্রত্যাগম "lustre";
    পক্ষ S_MAGIC_M1FS: /* 0x5346314D local */
      প্রত্যাগম "m1fs";
    পক্ষ S_MAGIC_MINIX: /* 0x137F local */
      প্রত্যাগম "minix";
    পক্ষ S_MAGIC_MINIX_30: /* 0x138F local */
      প্রত্যাগম "minix (30 char.)";
    পক্ষ S_MAGIC_MINIX_V2: /* 0x2468 local */
      প্রত্যাগম "minix v2";
    পক্ষ S_MAGIC_MINIX_V2_30: /* 0x2478 local */
      প্রত্যাগম "minix v2 (30 char.)";
    পক্ষ S_MAGIC_MINIX_V3: /* 0x4D5A local */
      প্রত্যাগম "minix3";
    পক্ষ S_MAGIC_MQUEUE: /* 0x19800202 local */
      প্রত্যাগম "mqueue";
    পক্ষ S_MAGIC_MSDOS: /* 0x4D44 local */
      প্রত্যাগম "msdos";
    পক্ষ S_MAGIC_NCP: /* 0x564C remote */
      প্রত্যাগম "novell";
    পক্ষ S_MAGIC_NFS: /* 0x6969 remote */
      প্রত্যাগম "nfs";
    পক্ষ S_MAGIC_NFSD: /* 0x6E667364 remote */
      প্রত্যাগম "nfsd";
    পক্ষ S_MAGIC_NILFS: /* 0x3434 local */
      প্রত্যাগম "nilfs";
    পক্ষ S_MAGIC_NSFS: /* 0x6E736673 local */
      প্রত্যাগম "nsfs";
    পক্ষ S_MAGIC_NTFS: /* 0x5346544E local */
      প্রত্যাগম "ntfs";
    পক্ষ S_MAGIC_NULL_FS: /* 0x4E554C4C local */
      প্রত্যাগম "nullfs";
    পক্ষ S_MAGIC_OPENPROM: /* 0x9FA1 local */
      প্রত্যাগম "openprom";
    পক্ষ S_MAGIC_OCFS2: /* 0x7461636F remote */
      প্রত্যাগম "ocfs2";
    পক্ষ S_MAGIC_OVERLAYFS: /* 0x794C7630 remote */
      /* This may overlay remote file systems.
         Also there have been issues reported with inotify and overlayfs,
         so mark as "remote" so that polling is used.  */
      প্রত্যাগম "overlayfs";
    পক্ষ S_MAGIC_PANFS: /* 0xAAD7AAEA remote */
      প্রত্যাগম "panfs";
    পক্ষ S_MAGIC_PID_FS: /* 0x50494446 local */
      প্রত্যাগম "pidfs";
    পক্ষ S_MAGIC_PIPEFS: /* 0x50495045 remote */
      /* FIXME: change syntax or add an optional attribute like "inotify:no".
         pipefs is labeled as "remote" so that tail always polls,
         but it isn't really remote file system type.  */
      প্রত্যাগম "pipefs";
    পক্ষ S_MAGIC_PPC_CMM: /* 0xC7571590 local */
      প্রত্যাগম "ppc-cmm-fs";
    পক্ষ S_MAGIC_PRL_FS: /* 0x7C7C6673 remote */
      প্রত্যাগম "prl_fs";
    পক্ষ S_MAGIC_PROC: /* 0x9FA0 local */
      প্রত্যাগম "proc";
    পক্ষ S_MAGIC_PSTOREFS: /* 0x6165676C local */
      প্রত্যাগম "pstorefs";
    পক্ষ S_MAGIC_QNX4: /* 0x002F local */
      প্রত্যাগম "qnx4";
    পক্ষ S_MAGIC_QNX6: /* 0x68191122 local */
      প্রত্যাগম "qnx6";
    পক্ষ S_MAGIC_RAMFS: /* 0x858458F6 local */
      প্রত্যাগম "ramfs";
    পক্ষ S_MAGIC_RDTGROUP: /* 0x07655821 local */
      প্রত্যাগম "rdt";
    পক্ষ S_MAGIC_REISERFS: /* 0x52654973 local */
      প্রত্যাগম "reiserfs";
    পক্ষ S_MAGIC_ROMFS: /* 0x7275 local */
      প্রত্যাগম "romfs";
    পক্ষ S_MAGIC_RPC_PIPEFS: /* 0x67596969 local */
      প্রত্যাগম "rpc_pipefs";
    পক্ষ S_MAGIC_SDCARDFS: /* 0x5DCA2DF5 local */
      প্রত্যাগম "sdcardfs";
    পক্ষ S_MAGIC_SECRETMEM: /* 0x5345434D local */
      প্রত্যাগম "secretmem";
    পক্ষ S_MAGIC_SECURITYFS: /* 0x73636673 local */
      প্রত্যাগম "securityfs";
    পক্ষ S_MAGIC_SELINUX: /* 0xF97CFF8C local */
      প্রত্যাগম "selinux";
    পক্ষ S_MAGIC_SMACK: /* 0x43415D53 local */
      প্রত্যাগম "smackfs";
    পক্ষ S_MAGIC_SMB: /* 0x517B remote */
      প্রত্যাগম "smb";
    পক্ষ S_MAGIC_SMB2: /* 0xFE534D42 remote */
      প্রত্যাগম "smb2";
    পক্ষ S_MAGIC_SNFS: /* 0xBEEFDEAD remote */
      প্রত্যাগম "snfs";
    পক্ষ S_MAGIC_SOCKFS: /* 0x534F434B local */
      প্রত্যাগম "sockfs";
    পক্ষ S_MAGIC_SQUASHFS: /* 0x73717368 local */
      প্রত্যাগম "squashfs";
    পক্ষ S_MAGIC_SYSFS: /* 0x62656572 local */
      প্রত্যাগম "sysfs";
    পক্ষ S_MAGIC_SYSV2: /* 0x012FF7B6 local */
      প্রত্যাগম "sysv2";
    পক্ষ S_MAGIC_SYSV4: /* 0x012FF7B5 local */
      প্রত্যাগম "sysv4";
    পক্ষ S_MAGIC_TMPFS: /* 0x01021994 local */
      প্রত্যাগম "tmpfs";
    পক্ষ S_MAGIC_TRACEFS: /* 0x74726163 local */
      প্রত্যাগম "tracefs";
    পক্ষ S_MAGIC_UBIFS: /* 0x24051905 local */
      প্রত্যাগম "ubifs";
    পক্ষ S_MAGIC_UDF: /* 0x15013346 local */
      প্রত্যাগম "udf";
    পক্ষ S_MAGIC_UFS: /* 0x00011954 local */
      প্রত্যাগম "ufs";
    পক্ষ S_MAGIC_UFS_BYTESWAPPED: /* 0x54190100 local */
      প্রত্যাগম "ufs";
    পক্ষ S_MAGIC_USBDEVFS: /* 0x9FA2 local */
      প্রত্যাগম "usbdevfs";
    পক্ষ S_MAGIC_V9FS: /* 0x01021997 local */
      প্রত্যাগম "v9fs";
    পক্ষ S_MAGIC_VBOXSF: /* 0x786F4256 remote */
      প্রত্যাগম "vboxsf";
    পক্ষ S_MAGIC_VMHGFS: /* 0xBACBACBC remote */
      প্রত্যাগম "vmhgfs";
    পক্ষ S_MAGIC_VXFS: /* 0xA501FCF5 remote */
      /* Veritas File System can run in single instance or clustered mode,
         so mark as remote to cater for the latter case.  */
      প্রত্যাগম "vxfs";
    পক্ষ S_MAGIC_VZFS: /* 0x565A4653 local */
      প্রত্যাগম "vzfs";
    পক্ষ S_MAGIC_WSLFS: /* 0x53464846 local */
      প্রত্যাগম "wslfs";
    পক্ষ S_MAGIC_XENFS: /* 0xABBA1974 local */
      প্রত্যাগম "xenfs";
    পক্ষ S_MAGIC_XENIX: /* 0x012FF7B4 local */
      প্রত্যাগম "xenix";
    পক্ষ S_MAGIC_XFS: /* 0x58465342 local */
      প্রত্যাগম "xfs";
    পক্ষ S_MAGIC_XIAFS: /* 0x012FD16D local */
      প্রত্যাগম "xia";
    পক্ষ S_MAGIC_Z3FOLD: /* 0x0033 local */
      প্রত্যাগম "z3fold";
    পক্ষ S_MAGIC_ZFS: /* 0x2FC12FC1 local */
      প্রত্যাগম "zfs";
    পক্ষ S_MAGIC_ZONEFS: /* 0x5A4F4653 local */
      প্রত্যাগম "zonefs";
    পক্ষ S_MAGIC_ZSMALLOC: /* 0x58295829 local */
      প্রত্যাগম "zsmallocfs";


# elif __GNU__
    পক্ষ FSTYPE_UFS:
      প্রত্যাগম "ufs";
    পক্ষ FSTYPE_NFS:
      প্রত্যাগম "nfs";
    পক্ষ FSTYPE_GFS:
      প্রত্যাগম "gfs";
    পক্ষ FSTYPE_LFS:
      প্রত্যাগম "lfs";
    পক্ষ FSTYPE_SYSV:
      প্রত্যাগম "sysv";
    পক্ষ FSTYPE_FTP:
      প্রত্যাগম "ftp";
    পক্ষ FSTYPE_TAR:
      প্রত্যাগম "tar";
    পক্ষ FSTYPE_AR:
      প্রত্যাগম "ar";
    পক্ষ FSTYPE_CPIO:
      প্রত্যাগম "cpio";
    পক্ষ FSTYPE_MSLOSS:
      প্রত্যাগম "msloss";
    পক্ষ FSTYPE_CPM:
      প্রত্যাগম "cpm";
    পক্ষ FSTYPE_HFS:
      প্রত্যাগম "hfs";
    পক্ষ FSTYPE_DTFS:
      প্রত্যাগম "dtfs";
    পক্ষ FSTYPE_GRFS:
      প্রত্যাগম "grfs";
    পক্ষ FSTYPE_TERM:
      প্রত্যাগম "term";
    পক্ষ FSTYPE_DEV:
      প্রত্যাগম "dev";
    পক্ষ FSTYPE_PROC:
      প্রত্যাগম "proc";
    পক্ষ FSTYPE_IFSOCK:
      প্রত্যাগম "ifsock";
    পক্ষ FSTYPE_AFS:
      প্রত্যাগম "afs";
    পক্ষ FSTYPE_DFS:
      প্রত্যাগম "dfs";
    পক্ষ FSTYPE_PROC9:
      প্রত্যাগম "proc9";
    পক্ষ FSTYPE_SOCKET:
      প্রত্যাগম "socket";
    পক্ষ FSTYPE_MISC:
      প্রত্যাগম "misc";
    পক্ষ FSTYPE_EXT2FS:
      প্রত্যাগম "ext2/ext3";
    পক্ষ FSTYPE_HTTP:
      প্রত্যাগম "http";
    পক্ষ FSTYPE_MEMFS:
      প্রত্যাগম "memfs";
    পক্ষ FSTYPE_ISO9660:
      প্রত্যাগম "iso9660";
# endif
    স্বভাব:
      {
        unsigned long পূর্ণাঙ্ক type = statfsbuf->f_type;
        স্থির বর্ণ buf[পরিমাণ "UNKNOWN (0x%lx)" - 3
                        + (পরিমাণ type * CHAR_BIT + 3) / 4];
        sprintf (buf, "UNKNOWN (0x%lx)", type);
        প্রত্যাগম buf;
      }
    }
#endif
}

NODISCARD
স্থির বর্ণ *
human_access (সংরচনা stat ধ্রুব *statbuf)
{
  স্থির বর্ণ modebuf[12];
  filemodestring (statbuf, modebuf);
  modebuf[10] = 0;
  প্রত্যাগম modebuf;
}

NODISCARD
স্থির বর্ণ *
human_time (সংরচনা timespec t)
{
  /* STR must be at least INT_BUFSIZE_BOUND (intmax_t) big, either
     because localtime_rz fails, or because the time zone is truly
     outlandish so that %z expands to a long string.  */
  স্থির বর্ণ str[INT_BUFSIZE_BOUND (intmax_t)
                  + INT_STRLEN_BOUND (পূর্ণাঙ্ক) /* YYYY */
                  + 1 /* because YYYY might equal INT_MAX + 1900 */
                  + পরিমাণ "-MM-DD HH:MM:SS.NNNNNNNNN +"];
  স্থির timezone_t tz;
  যদি (!tz)
    tz = tzalloc (getenv ("TZ"));
  সংরচনা tm tm;
  পূর্ণাঙ্ক ns = t.tv_nsec;
  যদি (localtime_rz (tz, &t.tv_sec, &tm))
    nstrftime (str, পরিমাণ str, "%Y-%m-%d %H:%M:%S.%N %z", &tm, tz, ns);
  অন্যথা
    {
      বর্ণ secbuf[INT_BUFSIZE_BOUND (intmax_t)];
      sprintf (str, "%s.%09d", timetostr (t.tv_sec, secbuf), ns);
    }
  প্রত্যাগম str;
}

/* PFORMAT points to a '%' followed by a prefix of a format, all of
   size PREFIX_LEN.  The flags allowed for this format are
   ALLOWED_FLAGS; remove other printf flags from the prefix, then
   append SUFFIX.  */
স্থির শূন্য
make_format (বর্ণ *pformat, size_t prefix_len, বর্ণ ধ্রুব *allowed_flags,
             বর্ণ ধ্রুব *suffix)
{
  বর্ণ *dst = pformat + 1;
  বর্ণ ধ্রুব *src;
  বর্ণ ধ্রুব *srclim = pformat + prefix_len;
  প্রতি (src = dst; src < srclim && strchr (printf_flags, *src); src++)
    যদি (strchr (allowed_flags, *src))
      *dst++ = *src;
  যাবত্ (src < srclim)
    *dst++ = *src++;
  strcpy (dst, suffix);
}

স্থির শূন্য
out_string (বর্ণ *pformat, size_t prefix_len, বর্ণ ধ্রুব *arg)
{
  make_format (pformat, prefix_len, "-", "s");
  printf (pformat, arg);
}
স্থির পূর্ণাঙ্ক
out_int (বর্ণ *pformat, size_t prefix_len, intmax_t arg)
{
  make_format (pformat, prefix_len, "'-+ 0", "jd");
  প্রত্যাগম printf (pformat, arg);
}
স্থির পূর্ণাঙ্ক
out_uint (বর্ণ *pformat, size_t prefix_len, uintmax_t arg)
{
  make_format (pformat, prefix_len, "'-0", "ju");
  প্রত্যাগম printf (pformat, arg);
}
স্থির শূন্য
out_uint_o (বর্ণ *pformat, size_t prefix_len, uintmax_t arg)
{
  make_format (pformat, prefix_len, "-#0", "jo");
  printf (pformat, arg);
}
স্থির শূন্য
out_uint_x (বর্ণ *pformat, size_t prefix_len, uintmax_t arg)
{
  make_format (pformat, prefix_len, "-#0", "jx");
  printf (pformat, arg);
}
স্থির পূর্ণাঙ্ক
out_minus_zero (বর্ণ *pformat, size_t prefix_len)
{
  make_format (pformat, prefix_len, "'-+ 0", ".0f");
  প্রত্যাগম printf (pformat, -0.25);
}

/* Output the number of seconds since the Epoch, using a format that
   acts like printf's %f format.  */
স্থির শূন্য
out_epoch_sec (বর্ণ *pformat, size_t prefix_len,
               সংরচনা timespec arg)
{
  বর্ণ *dot = memchr (pformat, '.', prefix_len);
  size_t sec_prefix_len = prefix_len;
  পূর্ণাঙ্ক width = 0;
  পূর্ণাঙ্ক precision = 0;
  bool frac_বাম_adjust = false;

  যদি (dot)
    {
      sec_prefix_len = dot - pformat;
      pformat[prefix_len] = '\0';

      যদি (c_isdigit (dot[1]))
        {
          long পূর্ণাঙ্ক lprec = strtol (dot + 1, রিক্ত, 10);
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
          বর্ণ *p = dot;
          *dot = '\0';

          কুরু
            --p;
          যাবত্ (c_isdigit (p[-1]));

          long পূর্ণাঙ্ক lwidth = strtol (p, রিক্ত, 10);
          width = (lwidth <= INT_MAX ? lwidth : INT_MAX);
          যদি (1 < width)
            {
              p += (*p == '0');
              sec_prefix_len = p - pformat;
              পূর্ণাঙ্ক w_d = (decimal_point_len < width
                         ? width - decimal_point_len
                         : 0);
              যদি (1 < w_d)
                {
                  পূর্ণাঙ্ক w = w_d - precision;
                  যদি (1 < w)
                    {
                      বর্ণ *dst = pformat;
                      প্রতি (বর্ণ ধ্রুব *src = dst; src < p; src++)
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

  পূর্ণাঙ্ক ভাজক = 1;
  প্রতি (পূর্ণাঙ্ক i = precision; i < 9; i++)
    ভাজক *= 10;
  পূর্ণাঙ্ক frac_sec = arg.tv_nsec / ভাজক;
  পূর্ণাঙ্ক int_len;

  যদি (TYPE_SIGNED (time_t))
    {
      bool minus_zero = false;
      যদি (arg.tv_sec < 0 && arg.tv_nsec != 0)
        {
          পূর্ণাঙ্ক frac_sec_modulus = 1000000000 / ভাজক;
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
      পূর্ণাঙ্ক prec = (precision < 9 ? precision : 9);
      পূর্ণাঙ্ক trailing_prec = precision - prec;
      পূর্ণাঙ্ক ilen = (int_len < 0 ? 0 : int_len);
      পূর্ণাঙ্ক trailing_width = (ilen < width && decimal_point_len < width - ilen
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
out_file_context (বর্ণ *pformat, size_t prefix_len, বর্ণ ধ্রুব *filename)
{
  বর্ণ *scontext;
  bool fail = false;

  যদি ((follow_links
       ? getfilecon (filename, &scontext)
       : lgetfilecon (filename, &scontext)) < 0)
    {
      error (0, errno, _("failed to get security context of %s"),
             quoteaf (filename));
      scontext = রিক্ত;
      fail = true;
    }
  strcpy (pformat + prefix_len, "s");
  printf (pformat, (scontext ? scontext : "?"));
  যদি (scontext)
    freecon (scontext);
  প্রত্যাগম fail;
}

/* Set the quoting style once it is needed.  */
স্থির শূন্য
initialize_quoting_style (শূন্য)
{
  স্থির bool initialized;
  যদি (!initialized)
    {
      initialized = true;
      পূর্ণাঙ্ক qs = getenv_quoting_style ();
      set_quoting_style (রিক্ত, 0 <= qs ? qs : shell_escape_quoting_style);
    }
}

/* Print statfs info.  Return zero upon success, nonzero upon failure.  */
NODISCARD
স্থির bool
print_statfs (বর্ণ *pformat, size_t prefix_len, MAYBE_UNUSED বর্ণ mod, বর্ণ m,
              MAYBE_UNUSED পূর্ণাঙ্ক fd, বর্ণ ধ্রুব *filename,
              শূন্য ধ্রুব *data)
{
  STRUCT_STATVFS ধ্রুব *statfsbuf = data;
  bool fail = false;

  বিকল্প (m)
    {
    পক্ষ 'n':
      যদি (mod == 'Q')
        {
          initialize_quoting_style ();
          filename = quoteN (filename);
        }
      out_string (pformat, prefix_len, filename);
      ভঙ্গ;

    পক্ষ 'i':
      {
#যদি STRUCT_STATXFS_F_FSID_IS_INTEGER
        uintmax_t fsid = statfsbuf->f_fsid;
#অন্যথা
        প্রকারনির্দেশ unsigned পূর্ণাঙ্ক fsid_word;
        static_assert (alignof (STRUCT_STATVFS) % alignof (fsid_word) == 0);
        static_assert (offsetof (STRUCT_STATVFS, f_fsid) % alignof (fsid_word)
                       == 0);
        static_assert (পরিমাণ statfsbuf->f_fsid % alignof (fsid_word) == 0);
        fsid_word ধ্রুব *p = (fsid_word *) &statfsbuf->f_fsid;

        /* Assume a little-endian word order, as that is compatible
           with glibc's statvfs implementation.  */
        uintmax_t fsid = 0;
        পূর্ণাঙ্ক শব্দগুলি = পরিমাণ statfsbuf->f_fsid / পরিমাণ *p;
        প্রতি (পূর্ণাঙ্ক i = 0; i < শব্দগুলি && i * পরিমাণ *p < পরিমাণ fsid; i++)
          {
            uintmax_t u = p[শব্দগুলি - 1 - i];
            fsid |= u << (i * CHAR_BIT * পরিমাণ *p);
          }
#endif
        out_uint_x (pformat, prefix_len, fsid);
      }
      ভঙ্গ;

    পক্ষ 'l':
      OUT_NAMEMAX (pformat, prefix_len, SB_F_NAMEMAX (statfsbuf));
      ভঙ্গ;
    পক্ষ 't':
#যদি HAVE_STRUCT_STATXFS_F_TYPE
      out_uint_x (pformat, prefix_len, statfsbuf->f_type);
#অন্যথা
      fputc ('?', stdout);
#endif
      ভঙ্গ;
    পক্ষ 'T':
      out_string (pformat, prefix_len, human_fstype (statfsbuf));
      ভঙ্গ;
    পক্ষ 'b':
      out_int (pformat, prefix_len, statfsbuf->f_blocks);
      ভঙ্গ;
    পক্ষ 'f':
      out_int (pformat, prefix_len, statfsbuf->f_bfree);
      ভঙ্গ;
    পক্ষ 'a':
      out_int (pformat, prefix_len, statfsbuf->f_bavail);
      ভঙ্গ;
    পক্ষ 's':
      out_uint (pformat, prefix_len, statfsbuf->f_bsize);
      ভঙ্গ;
    পক্ষ 'S':
      {
        uintmax_t frsize = STATFS_FRSIZE (statfsbuf);
        যদি (! frsize)
          frsize = statfsbuf->f_bsize;
        out_uint (pformat, prefix_len, frsize);
      }
      ভঙ্গ;
    পক্ষ 'c':
      out_uint (pformat, prefix_len, statfsbuf->f_files);
      ভঙ্গ;
    পক্ষ 'd':
      out_int (pformat, prefix_len, statfsbuf->f_ffree);
      ভঙ্গ;
    স্বভাব:
      fputc ('?', stdout);
      ভঙ্গ;
    }
  প্রত্যাগম fail;
}

/* Return any bind mounted source for a path.
   The caller should not free the returned buffer.
   Return NULL if no bind mount found.  */
NODISCARD
স্থির বর্ণ ধ্রুব *
find_bind_mount (বর্ণ ধ্রুব * name)
{
  বর্ণ ধ্রুব * bind_mount = রিক্ত;

  স্থির সংরচনা mount_entry *mount_list;
  স্থির bool tried_mount_list = false;
  যদি (!tried_mount_list) /* attempt/warn once per process.  */
    {
      যদি (!(mount_list = read_file_system_list (false)))
        error (0, errno, "%s", _("cannot read table of mounted file systems"));
      tried_mount_list = true;
    }

  সংরচনা stat name_stats;
  যদি (stat (name, &name_stats) != 0)
    প্রত্যাগম রিক্ত;

  প্রতি (সংরচনা mount_entry *me = mount_list; me; me = me->me_next)
    {
      যদি (me->me_dummy && me->me_devname[0] == '/'
          && streq (me->me_mountdir, name))
        {
          সংরচনা stat dev_stats;

          যদি (stat (me->me_devname, &dev_stats) == 0
              && psame_inode (&name_stats, &dev_stats))
            {
              bind_mount = me->me_devname;
              ভঙ্গ;
            }
        }
    }

  প্রত্যাগম bind_mount;
}

/* Print mount point.  Return zero upon success, nonzero upon failure.  */
NODISCARD
স্থির bool
out_mount_point (বর্ণ ধ্রুব *filename, বর্ণ *pformat, size_t prefix_len,
                 ধ্রুব সংরচনা stat *statp)
{

  বর্ণ ধ্রুব *np = "?", *bp = রিক্ত;
  বর্ণ *mp = রিক্ত;
  bool fail = true;

  /* Look for bind mounts first.  Note we output the immediate alias,
     rather than further resolving to a base device mount point.  */
  যদি (follow_links || !S_ISLNK (statp->st_mode))
    {
      বর্ণ *resolved = canonicalize_file_name (filename);
      যদি (!resolved)
        {
          error (0, errno, _("failed to canonicalize %s"), quoteaf (filename));
          গচ্ছ print_mount_point;
        }
      bp = find_bind_mount (resolved);
      মুক্ত (resolved);
      যদি (bp)
        {
          fail = false;
          গচ্ছ print_mount_point;
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
  প্রত্যাগম fail;
}

/* Map a TS with negative TS.tv_nsec to {0,0}.  */
স্থির অন্তঃস্থ সংরচনা timespec
neg_to_zero (সংরচনা timespec ts)
{
  যদি (0 <= ts.tv_nsec)
    প্রত্যাগম ts;
  সংরচনা timespec z = {0};
  প্রত্যাগম z;
}

/* Output a single-character \ escape.  */

স্থির শূন্য
print_esc_char (বর্ণ c)
{
  বিকল্প (c)
    {
    পক্ষ 'a':			/* Alert. */
      c ='\a';
      ভঙ্গ;
    পক্ষ 'b':			/* Backspace. */
      c ='\b';
      ভঙ্গ;
    পক্ষ 'e':			/* Escape. */
      c ='\x1B';
      ভঙ্গ;
    পক্ষ 'f':			/* Form feed. */
      c ='\f';
      ভঙ্গ;
    পক্ষ 'n':			/* New line. */
      c ='\n';
      ভঙ্গ;
    পক্ষ 'r':			/* Carriage return. */
      c ='\r';
      ভঙ্গ;
    পক্ষ 't':			/* Horizontal tab. */
      c ='\t';
      ভঙ্গ;
    পক্ষ 'v':			/* Vertical tab. */
      c ='\v';
      ভঙ্গ;
    পক্ষ '"':
    পক্ষ '\\':
      ভঙ্গ;
    স্বভাব:
      error (0, 0, _("warning: unrecognized escape '\\%c'"), c);
      ভঙ্গ;
    }
  putchar (c);
}

ATTRIBUTE_PURE
স্থির size_t
format_code_offset (বর্ণ ধ্রুব *directive)
{
  size_t len = strspn (directive + 1, printf_flags);
  বর্ণ ধ্রুব *fmt_char = directive + len + 1;
  fmt_char += strspn (fmt_char, digits);
  যদি (*fmt_char == '.')
    fmt_char += 1 + strspn (fmt_char + 1, digits);
  প্রত্যাগম fmt_char - directive;
}

/* Print the information specified by the format string, FORMAT,
   calling PRINT_FUNC for each %-directive encountered.
   Return zero upon success, nonzero upon failure.  */
NODISCARD
স্থির bool
print_it (বর্ণ ধ্রুব *format, পূর্ণাঙ্ক fd, বর্ণ ধ্রুব *filename,
          bool (*print_func) (বর্ণ *, size_t, বর্ণ, বর্ণ,
                              পূর্ণাঙ্ক, বর্ণ ধ্রুব *, শূন্য ধ্রুব *),
          শূন্য ধ্রুব *data)
{
  bool fail = false;

  /* Add 2 to accommodate our conversion of the stat '%s' format string
     to the longer printf '%llu' one.  */
  গণনা
    {
      MAX_ADDITIONAL_BYTES =
        (MAX (পরিমাণ "jd",
              MAX (পরিমাণ "jo", MAX (পরিমাণ "ju", পরিমাণ "jx")))
         - 1)
    };
  size_t n_alloc = strlen (format) + MAX_ADDITIONAL_BYTES + 1;
  বর্ণ *dest = xmalloc (n_alloc);
  প্রতি (বর্ণ ধ্রুব *b = format; *b; b++)
    {
      বিকল্প (*b)
        {
        পক্ষ '%':
          {
            size_t len = format_code_offset (b);
            বর্ণ fmt_char = *(b + len);
            বর্ণ mod_char = 0;
            memcpy (dest, b, len);
            b += len;

            বিকল্প (fmt_char)
              {
              পক্ষ '\0':
                --b;
                FALLTHROUGH;
              পক্ষ '%':
                যদি (1 < len)
                  {
                    dest[len] = fmt_char;
                    dest[len + 1] = '\0';
                    error (EXIT_FAILURE, 0, _("%s: invalid directive"),
                           quote (dest));
                  }
                putchar ('%');
                ভঙ্গ;
              পক্ষ 'H':
              পক্ষ 'L':
              পক্ষ 'Q':
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
              স্বভাব:
                fail |= print_func (dest, len, mod_char, fmt_char,
                                    fd, filename, data);
                ভঙ্গ;
              }
            ভঙ্গ;
          }

        পক্ষ '\\':
          যদি ( ! interpret_backslash_escapes)
            {
              putchar ('\\');
              ভঙ্গ;
            }
          ++b;
          যদি (isoct (*b))
            {
              পূর্ণাঙ্ক esc_মান = fromoct (*b);
              পূর্ণাঙ্ক esc_length = 1;	/* number of octal digits */
              প্রতি (++b; esc_length < 3 && isoct (*b);
                   ++esc_length, ++b)
                {
                  esc_মান = esc_মান * 8 + fromoct (*b);
                }
              putchar (esc_মান);
              --b;
            }
          অন্যথা যদি (*b == 'x' && c_isxdigit (b[1]))
            {
              পূর্ণাঙ্ক esc_মান = fromhex (b[1]);	/* Value of \xhh escape. */
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
          ভঙ্গ;

        স্বভাব:
          putchar (*b);
          ভঙ্গ;
        }
    }
  মুক্ত (dest);

  fputs (trailing_delim, stdout);

  প্রত্যাগম fail;
}

/* Stat the file system and print what we find.  */
NODISCARD
স্থির bool
do_statfs (বর্ণ ধ্রুব *filename, বর্ণ ধ্রুব *format)
{
  STRUCT_STATVFS statfsbuf;

  যদি (streq (filename, "-"))
    {
      error (0, 0, _("using %s to denote standard input does not work"
                     " in file system mode"), quoteaf (filename));
      প্রত্যাগম false;
    }

  যদি (STATFS (filename, &statfsbuf) != 0)
    {
      error (0, errno, _("cannot read file system information for %s"),
             quoteaf (filename));
      প্রত্যাগম false;
    }

  bool fail = print_it (format, -1, filename, print_statfs, &statfsbuf);
  প্রত্যাগম ! fail;
}

সংরচনা print_args {
  সংরচনা stat *st;
  সংরচনা timespec btime;
};

/* Ask statx to avoid syncing? */
স্থির bool dont_sync;

/* Ask statx to force sync? */
স্থির bool force_sync;

#যদি USE_STATX
স্থির unsigned পূর্ণাঙ্ক
fmt_to_mask (বর্ণ fmt)
{
  বিকল্প (fmt)
    {
    পক্ষ 'N':
      প্রত্যাগম STATX_MODE;
    পক্ষ 'd':
    পক্ষ 'D':
      প্রত্যাগম STATX_MODE;
    পক্ষ 'i':
      প্রত্যাগম STATX_INO;
    পক্ষ 'a':
    পক্ষ 'A':
      প্রত্যাগম STATX_MODE;
    পক্ষ 'f':
      প্রত্যাগম STATX_MODE|STATX_TYPE;
    পক্ষ 'F':
      প্রত্যাগম STATX_TYPE;
    পক্ষ 'h':
      প্রত্যাগম STATX_NLINK;
    পক্ষ 'u':
    পক্ষ 'U':
      প্রত্যাগম STATX_UID;
    পক্ষ 'g':
    পক্ষ 'G':
      প্রত্যাগম STATX_GID;
    পক্ষ 'm':
      প্রত্যাগম STATX_MODE|STATX_INO;
    পক্ষ 's':
      প্রত্যাগম STATX_SIZE;
    পক্ষ 't':
    পক্ষ 'T':
      প্রত্যাগম STATX_MODE;
    পক্ষ 'b':
      প্রত্যাগম STATX_BLOCKS;
    পক্ষ 'w':
    পক্ষ 'W':
      প্রত্যাগম STATX_BTIME;
    পক্ষ 'x':
    পক্ষ 'X':
      প্রত্যাগম STATX_ATIME;
    পক্ষ 'y':
    পক্ষ 'Y':
      প্রত্যাগম STATX_MTIME;
    পক্ষ 'z':
    পক্ষ 'Z':
      প্রত্যাগম STATX_CTIME;
    }
  প্রত্যাগম 0;
}

ATTRIBUTE_PURE
স্থির unsigned পূর্ণাঙ্ক
format_to_mask (বর্ণ ধ্রুব *format)
{
  unsigned পূর্ণাঙ্ক mask = 0;

  প্রতি (বর্ণ ধ্রুব *b = format; *b; b++)
    {
      যদি (*b != '%')
        অনুবর্ত;

      b += format_code_offset (b);
      যদি (*b == '\0')
        ভঙ্গ;
      mask |= fmt_to_mask (*b);
    }
  প্রত্যাগম mask;
}

/* statx the file and print what we find */
NODISCARD
স্থির bool
do_stat (বর্ণ ধ্রুব *filename, বর্ণ ধ্রুব *format, বর্ণ ধ্রুব *format2)
{
  পূর্ণাঙ্ক fd = streq (filename, "-") ? 0 : AT_FDCWD;
  পূর্ণাঙ্ক flags = 0;
  সংরচনা stat st;
  সংরচনা statx stx = {0};
  বর্ণ ধ্রুব *pathname = filename;
  সংরচনা print_args pa;
  pa.st = &st;
  pa.btime = (সংরচনা timespec) {.tv_sec = -1, .tv_nsec = -1};

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
      প্রত্যাগম false;
    }

  যদি (S_ISBLK (stx.stx_mode) || S_ISCHR (stx.stx_mode))
    format = format2;

  statx_to_stat (&stx, &st);
  যদি (stx.stx_mask & STATX_BTIME)
    pa.btime = statx_timestamp_to_timespec (stx.stx_btime);

  bool fail = print_it (format, fd, filename, print_stat, &pa);
  প্রত্যাগম ! fail;
}

#অন্যথা /* USE_STATX */

স্থির সংরচনা timespec
get_birthtime (পূর্ণাঙ্ক fd, বর্ণ ধ্রুব *filename, সংরচনা stat ধ্রুব *st)
{
  সংরচনা timespec ts = get_stat_birthtime (st);

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

  প্রত্যাগম ts;
}


/* stat the file and print what we find */
NODISCARD
স্থির bool
do_stat (বর্ণ ধ্রুব *filename, বর্ণ ধ্রুব *format,
         বর্ণ ধ্রুব *format2)
{
  পূর্ণাঙ্ক fd = streq (filename, "-") ? 0 : -1;
  সংরচনা stat statbuf;
  সংরচনা print_args pa;
  pa.st = &statbuf;
  pa.btime = (সংরচনা timespec) {.tv_sec = -1, .tv_nsec = -1};

  যদি (0 <= fd)
    {
      যদি (fstat (fd, &statbuf) != 0)
        {
          error (0, errno, _("cannot stat standard input"));
          প্রত্যাগম false;
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
      প্রত্যাগম false;
    }

  যদি (S_ISBLK (statbuf.st_mode) || S_ISCHR (statbuf.st_mode))
    format = format2;

  bool fail = print_it (format, fd, filename, print_stat, &pa);
  প্রত্যাগম ! fail;
}
#endif /* USE_STATX */

/* POSIX requires 'ls' to print file sizes without a sign, even
   when negative.  Be consistent with that.  */

স্থির uintmax_t
unsigned_file_size (off_t size)
{
  প্রত্যাগম size + (size < 0) * ((uintmax_t) OFF_T_MAX - OFF_T_MIN + 1);
}

/* Print stat info.  Return zero upon success, nonzero upon failure.  */
স্থির bool
print_stat (বর্ণ *pformat, size_t prefix_len, বর্ণ mod, বর্ণ m,
            MAYBE_UNUSED পূর্ণাঙ্ক fd, বর্ণ ধ্রুব *filename, শূন্য ধ্রুব *data)
{
  সংরচনা print_args *parg = (সংরচনা print_args *) data;
  সংরচনা stat *statbuf = parg->st;
  সংরচনা timespec btime = parg->btime;
  সংরচনা passwd *pw_ent;
  সংরচনা group *gw_ent;
  bool fail = false;

  বিকল্প (m)
    {
    পক্ষ 'n':
      যদি (mod == 'Q')
        {
          initialize_quoting_style ();
          filename = quoteN (filename);
        }
      out_string (pformat, prefix_len, filename);
      ভঙ্গ;
    পক্ষ 'N':
      initialize_quoting_style ();
      out_string (pformat, prefix_len, quoteN (filename));
      যদি (S_ISLNK (statbuf->st_mode))
        {
          বর্ণ *linkname = areadlink_with_size (filename, statbuf->st_size);
          যদি (linkname == রিক্ত)
            {
              error (0, errno, _("cannot read symbolic link %s"),
                     quoteaf (filename));
              প্রত্যাগম true;
            }
          printf (" -> ");
          out_string (pformat, prefix_len, quoteN (linkname));
          মুক্ত (linkname);
        }
      ভঙ্গ;
    পক্ষ 'd':
      যদি (mod == 'H')
        out_uint (pformat, prefix_len, major (statbuf->st_dev));
      অন্যথা যদি (mod == 'L')
        out_uint (pformat, prefix_len, minor (statbuf->st_dev));
      অন্যথা
        out_uint (pformat, prefix_len, statbuf->st_dev);
      ভঙ্গ;
    পক্ষ 'D':
      out_uint_x (pformat, prefix_len, statbuf->st_dev);
      ভঙ্গ;
    পক্ষ 'i':
      out_uint (pformat, prefix_len, statbuf->st_ino);
      ভঙ্গ;
    পক্ষ 'a':
      out_uint_o (pformat, prefix_len, statbuf->st_mode & CHMOD_MODE_BITS);
      ভঙ্গ;
    পক্ষ 'A':
      out_string (pformat, prefix_len, human_access (statbuf));
      ভঙ্গ;
    পক্ষ 'f':
      out_uint_x (pformat, prefix_len, statbuf->st_mode);
      ভঙ্গ;
    পক্ষ 'F':
      out_string (pformat, prefix_len, file_type (statbuf));
      ভঙ্গ;
    পক্ষ 'h':
      out_uint (pformat, prefix_len, statbuf->st_nlink);
      ভঙ্গ;
    পক্ষ 'u':
      out_uint (pformat, prefix_len, statbuf->st_uid);
      ভঙ্গ;
    পক্ষ 'U':
      pw_ent = getpwuid (statbuf->st_uid);
      out_string (pformat, prefix_len,
                  pw_ent ? pw_ent->pw_name : "UNKNOWN");
      ভঙ্গ;
    পক্ষ 'g':
      out_uint (pformat, prefix_len, statbuf->st_gid);
      ভঙ্গ;
    পক্ষ 'G':
      gw_ent = getgrgid (statbuf->st_gid);
      out_string (pformat, prefix_len,
                  gw_ent ? gw_ent->gr_name : "UNKNOWN");
      ভঙ্গ;
    পক্ষ 'm':
      fail |= out_mount_point (filename, pformat, prefix_len, statbuf);
      ভঙ্গ;
    পক্ষ 's':
      out_uint (pformat, prefix_len, unsigned_file_size (statbuf->st_size));
      ভঙ্গ;
    পক্ষ 'r':
      যদি (mod == 'H')
        out_uint (pformat, prefix_len, major (statbuf->st_rdev));
      অন্যথা যদি (mod == 'L')
        out_uint (pformat, prefix_len, minor (statbuf->st_rdev));
      অন্যথা
        out_uint (pformat, prefix_len, statbuf->st_rdev);
      ভঙ্গ;
    পক্ষ 'R':
      out_uint_x (pformat, prefix_len, statbuf->st_rdev);
      ভঙ্গ;
    পক্ষ 't':
      out_uint_x (pformat, prefix_len, major (statbuf->st_rdev));
      ভঙ্গ;
    পক্ষ 'T':
      out_uint_x (pformat, prefix_len, minor (statbuf->st_rdev));
      ভঙ্গ;
    পক্ষ 'B':
      out_uint (pformat, prefix_len, ST_NBLOCKSIZE);
      ভঙ্গ;
    পক্ষ 'b':
      out_uint (pformat, prefix_len, STP_NBLOCKS (statbuf));
      ভঙ্গ;
    পক্ষ 'o':
      out_uint (pformat, prefix_len, STP_BLKSIZE (statbuf));
      ভঙ্গ;
    পক্ষ 'w':
      {
#যদি ! USE_STATX
        btime = get_birthtime (fd, filename, statbuf);
#endif
        যদি (btime.tv_nsec < 0)
          out_string (pformat, prefix_len, "-");
        অন্যথা
          out_string (pformat, prefix_len, human_time (btime));
      }
      ভঙ্গ;
    পক্ষ 'W':
      {
#যদি ! USE_STATX
        btime = get_birthtime (fd, filename, statbuf);
#endif
        out_epoch_sec (pformat, prefix_len, neg_to_zero (btime));
      }
      ভঙ্গ;
    পক্ষ 'x':
      out_string (pformat, prefix_len, human_time (get_stat_atime (statbuf)));
      ভঙ্গ;
    পক্ষ 'X':
      out_epoch_sec (pformat, prefix_len, get_stat_atime (statbuf));
      ভঙ্গ;
    পক্ষ 'y':
      out_string (pformat, prefix_len, human_time (get_stat_mtime (statbuf)));
      ভঙ্গ;
    পক্ষ 'Y':
      out_epoch_sec (pformat, prefix_len, get_stat_mtime (statbuf));
      ভঙ্গ;
    পক্ষ 'z':
      out_string (pformat, prefix_len, human_time (get_stat_ctime (statbuf)));
      ভঙ্গ;
    পক্ষ 'Z':
      out_epoch_sec (pformat, prefix_len, get_stat_ctime (statbuf));
      ভঙ্গ;
    পক্ষ 'C':
      fail |= out_file_context (pformat, prefix_len, filename);
      ভঙ্গ;
    স্বভাব:
      fputc ('?', stdout);
      ভঙ্গ;
    }
  প্রত্যাগম fail;
}

/* Return an allocated format string in static storage that
   corresponds to whether FS and TERSE options were declared.  */
স্থির বর্ণ *
default_format (bool fs, bool terse, bool device)
{
  বর্ণ *format;
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
          বর্ণ *temp;
          /* TRANSLATORS: This string uses format specifiers from
             'stat --help' without --file-system, and NOT from printf.  */
          format = xstrdup (_("\
  File: %N\n\
  Size: %-10s\tBlocks: %-10b IO Block: %-6o %F\n\
"));

          temp = format;
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
          মুক্ত (temp);

          temp = format;
          /* TRANSLATORS: This string uses format specifiers from
             'stat --help' without --file-system, and NOT from printf.  */
          format = xasprintf ("%s%s", format, _("\
" "Access: (%04a/%10.10A)  Uid: (%5u/%8U)   Gid: (%5g/%8G)\n\
"));
          মুক্ত (temp);

          যদি (0 < is_selinux_enabled ())
            {
              temp = format;
              /* TRANSLATORS: This string uses format specifiers from
                 'stat --help' without --file-system, and NOT from printf.  */
              format = xasprintf ("%s%s", format, _("Context: %C\n"));
              মুক্ত (temp);
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
          মুক্ত (temp);
        }
    }
  প্রত্যাগম format;
}

শূন্য
usage (পূর্ণাঙ্ক status)
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
         use the specified FORMAT instead of the স্বভাব;\n\
         output a newline after each use of FORMAT\n\
"));
      oputs (_("\
      --printf=FORMAT\n\
         like --format, but interpret backslash escapes,\n\
         and কুরু not output a mandatory trailing newline;\n\
         যদি you want a newline, include \\n in FORMAT\n\
"));
      oputs (_("\
  -t, --terse\n\
         print the information in terse form\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);

      fputs (_("\n\
The MODE argument of --cached can be: always, never, or স্বভাব.\n\
'always' will use cached attributes যদি available, যাবত্\n\
'never' will try to synchronize with the latest attributes, and\n\
'default' will leave it up to the underlying file system.\n\
"), stdout);

      fputs (_("\n\
The valid format sequences প্রতি files (without --file-system):\n\
\n\
  %a   permission bits in octal (see '#' and '0' printf flags)\n\
  %A   permission bits and file type in human readable form\n\
  %b   সংখ্যা of blocks allocated (see %B)\n\
  %B   the size in bytes of each block reported by %b\n\
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
  %o   optimal I/O transfer size hint\n\
  %s   total size, in bytes\n\
  %r   device type in decimal (st_rdev)\n\
  %R   device type in hex (st_rdev)\n\
  %Hr  major device type in decimal, প্রতি character/block device special files\n\
  %Lr  minor device type in decimal, প্রতি character/block device special files\n\
  %t   major device type in hex, প্রতি character/block device special files\n\
  %T   minor device type in hex, প্রতি character/block device special files\n\
"), stdout);
      fputs (_("\
  %u   user ID of owner\n\
  %U   user name of owner\n\
  %w   time of file birth, human-readable; - যদি unknown\n\
  %W   time of file birth, seconds since Epoch; 0 যদি unknown\n\
  %x   time of last access, human-readable\n\
  %X   time of last access, seconds since Epoch\n\
  %y   time of last data modification, human-readable\n\
  %Y   time of last data modification, seconds since Epoch\n\
  %z   time of last status change, human-readable\n\
  %Z   time of last status change, seconds since Epoch\n\
\n\
"), stdout);

      fputs (_("\
Valid format sequences প্রতি file systems:\n\
\n\
  %a   মুক্ত blocks available to non-superuser\n\
  %b   total data blocks in file system\n\
  %c   total file nodes in file system\n\
  %d   মুক্ত file nodes in file system\n\
  %f   মুক্ত blocks in file system\n\
"), stdout);
      fputs (_("\
  %i   file system ID in hex\n\
  %l   maximum length of filenames\n\
  %n   file name\n\
  %Qn  quoted file name\n\
  %s   block size (প্রতি faster transfers)\n\
  %S   fundamental block size (প্রতি block counts)\n\
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

পূর্ণাঙ্ক
main (পূর্ণাঙ্ক argc, বর্ণ *argv[])
{
  পূর্ণাঙ্ক c;
  bool fs = false;
  bool terse = false;
  বর্ণ *format = রিক্ত;
  বর্ণ *format2;
  bool ok = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  সংরচনা lconv ধ্রুব *locale = localeconv ();
  decimal_point = (locale->decimal_point[0] ? locale->decimal_point : ".");
  decimal_point_len = strlen (decimal_point);

  atexit (close_stdout);

  যাবত্ ((c = getopt_long (argc, argv, "c:fLt", long_options, রিক্ত)) != -1)
    {
      বিকল্প (c)
        {
        পক্ষ PRINTF_OPTION:
          format = optarg;
          interpret_backslash_escapes = true;
          trailing_delim = "";
          ভঙ্গ;

        পক্ষ 'c':
          format = optarg;
          interpret_backslash_escapes = false;
          trailing_delim = "\n";
          ভঙ্গ;

        পক্ষ 'L':
          follow_links = true;
          ভঙ্গ;

        পক্ষ 'f':
          fs = true;
          ভঙ্গ;

        পক্ষ 't':
          terse = true;
          ভঙ্গ;

        পক্ষ 0:
          বিকল্প (XARGMATCH ("--cached", optarg, cached_args, cached_modes))
            {
              পক্ষ cached_never:
                force_sync = true;
                dont_sync = false;
                ভঙ্গ;
              পক্ষ cached_always:
                force_sync = false;
                dont_sync = true;
                ভঙ্গ;
              পক্ষ cached_default:
                force_sync = false;
                dont_sync = false;
            }
          ভঙ্গ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        স্বভাব:
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

  প্রতি (পূর্ণাঙ্ক i = optind; i < argc; i++)
    ok &= (fs
           ? do_statfs (argv[i], format)
           : do_stat (argv[i], format, format2));

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
