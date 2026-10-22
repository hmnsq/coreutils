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

#अन्तर्भाव <config.h>

/* Keep this conditional in sync with the similar conditional in
   ../m4/stat-prog.m4.  */
#जइ ((STAT_STATVFS || STAT_STATVFS64)                                       \
     && (HAVE_STRUCT_STATVFS_F_BASETYPE || HAVE_STRUCT_STATVFS_F_FSTYPENAME \
         || (! HAVE_STRUCT_STATFS_F_FSTYPENAME && HAVE_STRUCT_STATVFS_F_TYPE)))
# define USE_STATVFS 1
#अण्णहा
# define USE_STATVFS 0
#endif

#अन्तर्भाव <stdio.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <pwd.h>
#अन्तर्भाव <grp.h>
#जइ USE_STATVFS
# include <sys/statvfs.h>
#नोचेद्यदि HAVE_SYS_VFS_H
# include <sys/vfs.h>
#नोचेद्यदि HAVE_SYS_MOUNT_H && HAVE_SYS_PARAM_H
/* NOTE: freebsd5.0 needs sys/param.h and sys/mount.h for statfs.
   It does have statvfs.h, but shouldn't use it, since it doesn't
   HAVE_STRUCT_STATVFS_F_BASETYPE.  So find a clean way to fix it.  */
/* NetBSD 1.5.2 needs these, for the declaration of struct statfs. */
# include <sys/param.h>
# include <sys/mount.h>
# जइ HAVE_NFS_NFS_CLNT_H && HAVE_NFS_VFS_H
/* Ultrix 4.4 needs these for the declaration of struct statfs.  */
#  include <netinet/in.h>
#  include <nfs/nfs_clnt.h>
#  include <nfs/vfs.h>
# endif
#नोचेद्यदि HAVE_OS_H /* BeOS */
# include <fs_info.h>
#endif
#अन्तर्भाव <selinux/selinux.h>
#अन्तर्भाव <getopt.h>

#अन्तर्भाव "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#अन्तर्भाव "system.h"

#अन्तर्भाव "areadlink.h"
#अन्तर्भाव "c-ctype.h"
#अन्तर्भाव "file-type.h"
#अन्तर्भाव "filemode.h"
#अन्तर्भाव "fs.h"
#अन्तर्भाव "mountlist.h"
#अन्तर्भाव "octhexdigits.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "stat-size.h"
#अन्तर्भाव "stat-time.h"
#अन्तर्भाव "strftime.h"
#अन्तर्भाव "find-mount-point.h"
#अन्तर्भाव "xvasprintf.h"
#अन्तर्भाव "statx.h"

#जइ HAVE_STATX && defined STATX_INO
# define USE_STATX 1
#अण्णहा
# define USE_STATX 0
#endif

#जइ USE_STATVFS
# define STRUCT_STATXFS_F_FSID_IS_INTEGER STRUCT_STATVFS_F_FSID_IS_INTEGER
# define HAVE_STRUCT_STATXFS_F_TYPE HAVE_STRUCT_STATVFS_F_TYPE
# जइ HAVE_STRUCT_STATVFS_F_NAMEMAX
#  define SB_F_NAMEMAX(S) ((S)->f_namemax)
# endif
# जइ ! STAT_STATVFS && STAT_STATVFS64
#  define STRUCT_STATVFS संरचना statvfs64
#  define STATFS statvfs64
# अण्णहा
#  define STRUCT_STATVFS संरचना statvfs
#  define STATFS statvfs
# endif
# define STATFS_FRSIZE(S) ((S)->f_frsize)
#अण्णहा
# define HAVE_STRUCT_STATXFS_F_TYPE HAVE_STRUCT_STATFS_F_TYPE
# जइ HAVE_STRUCT_STATFS_F_NAMELEN
#  define SB_F_NAMEMAX(S) ((S)->f_namelen)
# elif HAVE_STRUCT_STATFS_F_NAMEMAX
#  define SB_F_NAMEMAX(S) ((S)->f_namemax)
# endif
# define STATFS statfs
# जइ HAVE_OS_H /* BeOS */
/* BeOS has a statvfs function, but it does not return sensible values
   for f_files, f_ffree and f_favail, and lacks f_type, f_basetype and
   f_fstypename.  Use 'struct fs_info' instead.  */
NODISCARD
स्थिर पूर्णाङ्क
statfs (वर्ण ध्रुव *filename, संरचना fs_info *buf)
{
  dev_t device = dev_for_path (filename);
  जइ (device < 0)
    {
      errno = (device == B_ENTRY_NOT_FOUND ? ENOENT
               : device == B_BAD_मानम् ? EINVAL
               : device == B_NAME_TOO_LONG ? ENAMETOOLONG
               : device == B_NO_MEMORY ? ENOMEM
               : device == B_FILE_ERROR ? EIO
               : 0);
      प्रत्यागम -1;
    }
  /* If successful, buf->dev will be == device.  */
  प्रत्यागम fs_stat_dev (device, buf);
}
#  define f_fsid dev
#  define f_blocks total_blocks
#  define f_bfree free_blocks
#  define f_bavail free_blocks
#  define f_bsize io_size
#  define f_files total_nodes
#  define f_ffree free_nodes
#  define STRUCT_STATVFS संरचना fs_info
#  define STRUCT_STATXFS_F_FSID_IS_INTEGER true
#  define STATFS_FRSIZE(S) ((S)->block_size)
# अण्णहा
#  define STRUCT_STATVFS संरचना statfs
#  define STRUCT_STATXFS_F_FSID_IS_INTEGER STRUCT_STATFS_F_FSID_IS_INTEGER
#  जइ HAVE_STRUCT_STATFS_F_FRSIZE
#   define STATFS_FRSIZE(S) ((S)->f_frsize)
#  अण्णहा
#   define STATFS_FRSIZE(S) 0
#  endif
# endif
#endif

#ifdef SB_F_NAMEMAX
# define OUT_NAMEMAX out_uint
#अण्णहा
/* Depending on whether statvfs or statfs is used,
   neither f_namemax or f_namelen may be available.  */
# define SB_F_NAMEMAX(S) "?"
# define OUT_NAMEMAX out_string
#endif

#जइ HAVE_STRUCT_STATVFS_F_BASETYPE
# define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME f_basetype
#अण्णहा
# जइ HAVE_STRUCT_STATVFS_F_FSTYPENAME || HAVE_STRUCT_STATFS_F_FSTYPENAME
#  define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME f_fstypename
# elif HAVE_OS_H /* BeOS */
#  define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME fsh_name
# endif
#endif

#जइ HAVE_GETATTRAT
# include <attr.h>
# include <sys/nvpair.h>
#endif

स्थिर वर्ण ध्रुव digits[] = "0123456789";

/* Flags that are portable for use in printf, for at least one
   conversion specifier; make_format removes non-portable flags as
   needed for particular specifiers.  The glibc 2.2 extension "I" is
   listed here; it is removed by make_format because it has undefined
   behavior elsewhere and because it is incompatible with
   out_epoch_sec.  */
स्थिर वर्ण ध्रुव printf_flags[] = "'-+ #0I";

/* Formats for the --terse option.  */
स्थिर वर्ण ध्रुव fmt_terse_fs[] = "%Qn %i %l %t %s %S %b %f %a %c %d\n";
स्थिर वर्ण ध्रुव fmt_terse_regular[] = "%Qn %s %b %f %u %g %D %i %h %t %T"
                                        " %X %Y %Z %W %o\n";
स्थिर वर्ण ध्रुव fmt_terse_selinux[] = "%Qn %s %b %f %u %g %D %i %h %t %T"
                                        " %X %Y %Z %W %o %C\n";

#define PROGRAM_NAME "stat"

#define AUTHORS proper_name ("Michael Meskes")

गणना
{
  PRINTF_OPTION = CHAR_MAX + 1
};

गणना cached_mode
{
  cached_default,
  cached_never,
  cached_always
};

स्थिर वर्ण ध्रुव *ध्रुव cached_args[] =
{
  "default", "never", "always", रिक्त
};

स्थिर गणना cached_mode ध्रुव cached_modes[] =
{
  cached_default, cached_never, cached_always
};

स्थिर संरचना option ध्रुव long_options[] =
{
  {"dereference", no_argument, रिक्त, 'L'},
  {"file-system", no_argument, रिक्त, 'f'},
  {"format", required_argument, रिक्त, 'c'},
  {"printf", required_argument, रिक्त, PRINTF_OPTION},
  {"terse", no_argument, रिक्त, 't'},
  {"cached", required_argument, रिक्त, 0},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

/* Whether to follow symbolic links;  True for --dereference (-L).  */
स्थिर bool follow_links;

/* Whether to interpret backslash-escape sequences.
   True for --printf=FMT, not for --format=FMT (-c).  */
स्थिर bool interpret_backslash_escapes;

/* The trailing delimiter string:
   "" for --printf=FMT, "\n" for --format=FMT (-c).  */
स्थिर वर्ण ध्रुव *trailing_delim = "";

/* The representation of the decimal point in the current locale.  */
स्थिर वर्ण ध्रुव *decimal_point;
स्थिर size_t decimal_point_len;

स्थिर bool
print_stat (वर्ण *pformat, size_t prefix_len, वर्ण mod, वर्ण m,
            पूर्णाङ्क fd, वर्ण ध्रुव *filename, शून्य ध्रुव *data);

/* Return the type of the specified file system.
   Some systems have statfvs.f_basetype[FSTYPSZ] (AIX, HP-UX, and Solaris).
   Others have statvfs.f_fstypename[_VFS_NAMELEN] (NetBSD 3.0).
   Others have statfs.f_fstypename[MFSNAMELEN] (NetBSD 1.5.2).
   Still others have neither and have to get by with f_type (GNU/Linux).
   But f_type may only exist in statfs (Cygwin).  */
NODISCARD
स्थिर वर्ण ध्रुव *
human_fstype (STRUCT_STATVFS ध्रुव *statfsbuf)
{
#ifdef STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME
  प्रत्यागम statfsbuf->STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME;
#अण्णहा
  विकल्प (statfsbuf->f_type)
    {
# जइ defined __linux__ || defined __ANDROID__

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

    पक्ष S_MAGIC_AAFS: /* 0x5A3C69F0 local */
      प्रत्यागम "aafs";
    पक्ष S_MAGIC_ACFS: /* 0x61636673 remote */
      प्रत्यागम "acfs";
    पक्ष S_MAGIC_ADFS: /* 0xADF5 local */
      प्रत्यागम "adfs";
    पक्ष S_MAGIC_AFFS: /* 0xADFF local */
      प्रत्यागम "affs";
    पक्ष S_MAGIC_AFS: /* 0x5346414F remote */
      प्रत्यागम "afs";
    पक्ष S_MAGIC_ANON_INODE_FS: /* 0x09041934 local */
      प्रत्यागम "anon-inode FS";
    पक्ष S_MAGIC_AUFS: /* 0x61756673 remote */
      /* FIXME: change syntax or add an optional attribute like "inotify:no".
         The above is labeled as "remote" so that tail always uses polling,
         but this isn't really a remote file system type.  */
      प्रत्यागम "aufs";
    पक्ष S_MAGIC_AUTOFS: /* 0x0187 local */
      प्रत्यागम "autofs";
    पक्ष S_MAGIC_BALLOON_KVM: /* 0x13661366 local */
      प्रत्यागम "balloon-kvm-fs";
    पक्ष S_MAGIC_BCACHEFS: /* 0xCA451A4E local */
      प्रत्यागम "bcachefs";
    पक्ष S_MAGIC_BEFS: /* 0x42465331 local */
      प्रत्यागम "befs";
    पक्ष S_MAGIC_BDEVFS: /* 0x62646576 local */
      प्रत्यागम "bdevfs";
    पक्ष S_MAGIC_BFS: /* 0x1BADFACE local */
      प्रत्यागम "bfs";
    पक्ष S_MAGIC_BINDERFS: /* 0x6C6F6F70 local */
      प्रत्यागम "binderfs";
    पक्ष S_MAGIC_BPF_FS: /* 0xCAFE4A11 local */
      प्रत्यागम "bpf_fs";
    पक्ष S_MAGIC_BINFMTFS: /* 0x42494E4D local */
      प्रत्यागम "binfmt_misc";
    पक्ष S_MAGIC_BTRFS: /* 0x9123683E local */
      प्रत्यागम "btrfs";
    पक्ष S_MAGIC_BTRFS_TEST: /* 0x73727279 local */
      प्रत्यागम "btrfs_test";
    पक्ष S_MAGIC_CEPH: /* 0x00C36400 remote */
      प्रत्यागम "ceph";
    पक्ष S_MAGIC_CGROUP: /* 0x0027E0EB local */
      प्रत्यागम "cgroupfs";
    पक्ष S_MAGIC_CGROUP2: /* 0x63677270 local */
      प्रत्यागम "cgroup2fs";
    पक्ष S_MAGIC_CIFS: /* 0xFF534D42 remote */
      प्रत्यागम "cifs";
    पक्ष S_MAGIC_CODA: /* 0x73757245 remote */
      प्रत्यागम "coda";
    पक्ष S_MAGIC_COH: /* 0x012FF7B7 local */
      प्रत्यागम "coh";
    पक्ष S_MAGIC_CONFIGFS: /* 0x62656570 local */
      प्रत्यागम "configfs";
    पक्ष S_MAGIC_CRAMFS: /* 0x28CD3D45 local */
      प्रत्यागम "cramfs";
    पक्ष S_MAGIC_CRAMFS_WEND: /* 0x453DCD28 local */
      प्रत्यागम "cramfs-wend";
    पक्ष S_MAGIC_DAXFS: /* 0x64646178 local */
      प्रत्यागम "daxfs";
    पक्ष S_MAGIC_DEBUGFS: /* 0x64626720 local */
      प्रत्यागम "debugfs";
    पक्ष S_MAGIC_DEVFS: /* 0x1373 local */
      प्रत्यागम "devfs";
    पक्ष S_MAGIC_DEVMEM: /* 0x454D444D local */
      प्रत्यागम "devmem";
    पक्ष S_MAGIC_DEVPTS: /* 0x1CD1 local */
      प्रत्यागम "devpts";
    पक्ष S_MAGIC_DMA_BUF: /* 0x444D4142 local */
      प्रत्यागम "dma-buf-fs";
    पक्ष S_MAGIC_ECRYPTFS: /* 0xF15F local */
      प्रत्यागम "ecryptfs";
    पक्ष S_MAGIC_EFIVARFS: /* 0xDE5E81E4 local */
      प्रत्यागम "efivarfs";
    पक्ष S_MAGIC_EFS: /* 0x00414A53 local */
      प्रत्यागम "efs";
    पक्ष S_MAGIC_EROFS_V1: /* 0xE0F5E1E2 local */
      प्रत्यागम "erofs";
    पक्ष S_MAGIC_EXFAT: /* 0x2011BAB0 local */
      प्रत्यागम "exfat";
    पक्ष S_MAGIC_EXFS: /* 0x45584653 local */
      प्रत्यागम "exfs";
    पक्ष S_MAGIC_EXOFS: /* 0x5DF5 local */
      प्रत्यागम "exofs";
    पक्ष S_MAGIC_EXT: /* 0x137D local */
      प्रत्यागम "ext";
    पक्ष S_MAGIC_EXT2: /* 0xEF53 local */
      प्रत्यागम "ext2/ext3";
    पक्ष S_MAGIC_EXT2_OLD: /* 0xEF51 local */
      प्रत्यागम "ext2";
    पक्ष S_MAGIC_F2FS: /* 0xF2F52010 local */
      प्रत्यागम "f2fs";
    पक्ष S_MAGIC_FAIL_FS: /* 0x4641494C local */
      प्रत्यागम "failfs";
    पक्ष S_MAGIC_FAT: /* 0x4006 local */
      प्रत्यागम "fat";
    पक्ष S_MAGIC_FHGFS: /* 0x19830326 remote */
      प्रत्यागम "fhgfs";
    पक्ष S_MAGIC_FUSE: /* 0x65735546 remote */
      प्रत्यागम "fuse";
    पक्ष S_MAGIC_FUSECTL: /* 0x65735543 remote */
      प्रत्यागम "fusectl";
    पक्ष S_MAGIC_FUTEXFS: /* 0x0BAD1DEA local */
      प्रत्यागम "futexfs";
    पक्ष S_MAGIC_GFS: /* 0x01161970 remote */
      प्रत्यागम "gfs/gfs2";
    पक्ष S_MAGIC_GPFS: /* 0x47504653 remote */
      प्रत्यागम "gpfs";
    पक्ष S_MAGIC_GUEST_MEMFD: /* 0x474D454D remote */
      प्रत्यागम "guest-memfd";
    पक्ष S_MAGIC_HFS: /* 0x4244 local */
      प्रत्यागम "hfs";
    पक्ष S_MAGIC_HFS_PLUS: /* 0x482B local */
      प्रत्यागम "hfs+";
    पक्ष S_MAGIC_HFS_X: /* 0x4858 local */
      प्रत्यागम "hfsx";
    पक्ष S_MAGIC_HOSTFS: /* 0x00C0FFEE local */
      प्रत्यागम "hostfs";
    पक्ष S_MAGIC_HPFS: /* 0xF995E849 local */
      प्रत्यागम "hpfs";
    पक्ष S_MAGIC_HUGETLBFS: /* 0x958458F6 local */
      प्रत्यागम "hugetlbfs";
    पक्ष S_MAGIC_MTD_INODE_FS: /* 0x11307854 local */
      प्रत्यागम "inodefs";
    पक्ष S_MAGIC_IBRIX: /* 0x013111A8 remote */
      प्रत्यागम "ibrix";
    पक्ष S_MAGIC_INOTIFYFS: /* 0x2BAD1DEA local */
      प्रत्यागम "inotifyfs";
    पक्ष S_MAGIC_ISOFS: /* 0x9660 local */
      प्रत्यागम "isofs";
    पक्ष S_MAGIC_ISOFS_R_WIN: /* 0x4004 local */
      प्रत्यागम "isofs";
    पक्ष S_MAGIC_ISOFS_WIN: /* 0x4000 local */
      प्रत्यागम "isofs";
    पक्ष S_MAGIC_JFFS: /* 0x07C0 local */
      प्रत्यागम "jffs";
    पक्ष S_MAGIC_JFFS2: /* 0x72B6 local */
      प्रत्यागम "jffs2";
    पक्ष S_MAGIC_JFS: /* 0x3153464A local */
      प्रत्यागम "jfs";
    पक्ष S_MAGIC_KAFS: /* 0x6B414653 remote */
      प्रत्यागम "k-afs";
    पक्ष S_MAGIC_LOGFS: /* 0xC97E8168 local */
      प्रत्यागम "logfs";
    पक्ष S_MAGIC_LUSTRE: /* 0x0BD00BD0 remote */
      प्रत्यागम "lustre";
    पक्ष S_MAGIC_M1FS: /* 0x5346314D local */
      प्रत्यागम "m1fs";
    पक्ष S_MAGIC_MINIX: /* 0x137F local */
      प्रत्यागम "minix";
    पक्ष S_MAGIC_MINIX_30: /* 0x138F local */
      प्रत्यागम "minix (30 char.)";
    पक्ष S_MAGIC_MINIX_V2: /* 0x2468 local */
      प्रत्यागम "minix v2";
    पक्ष S_MAGIC_MINIX_V2_30: /* 0x2478 local */
      प्रत्यागम "minix v2 (30 char.)";
    पक्ष S_MAGIC_MINIX_V3: /* 0x4D5A local */
      प्रत्यागम "minix3";
    पक्ष S_MAGIC_MQUEUE: /* 0x19800202 local */
      प्रत्यागम "mqueue";
    पक्ष S_MAGIC_MSDOS: /* 0x4D44 local */
      प्रत्यागम "msdos";
    पक्ष S_MAGIC_NCP: /* 0x564C remote */
      प्रत्यागम "novell";
    पक्ष S_MAGIC_NFS: /* 0x6969 remote */
      प्रत्यागम "nfs";
    पक्ष S_MAGIC_NFSD: /* 0x6E667364 remote */
      प्रत्यागम "nfsd";
    पक्ष S_MAGIC_NILFS: /* 0x3434 local */
      प्रत्यागम "nilfs";
    पक्ष S_MAGIC_NSFS: /* 0x6E736673 local */
      प्रत्यागम "nsfs";
    पक्ष S_MAGIC_NTFS: /* 0x5346544E local */
      प्रत्यागम "ntfs";
    पक्ष S_MAGIC_NULL_FS: /* 0x4E554C4C local */
      प्रत्यागम "nullfs";
    पक्ष S_MAGIC_OPENPROM: /* 0x9FA1 local */
      प्रत्यागम "openprom";
    पक्ष S_MAGIC_OCFS2: /* 0x7461636F remote */
      प्रत्यागम "ocfs2";
    पक्ष S_MAGIC_OVERLAYFS: /* 0x794C7630 remote */
      /* This may overlay remote file systems.
         Also there have been issues reported with inotify and overlayfs,
         so mark as "remote" so that polling is used.  */
      प्रत्यागम "overlayfs";
    पक्ष S_MAGIC_PANFS: /* 0xAAD7AAEA remote */
      प्रत्यागम "panfs";
    पक्ष S_MAGIC_PID_FS: /* 0x50494446 local */
      प्रत्यागम "pidfs";
    पक्ष S_MAGIC_PIPEFS: /* 0x50495045 remote */
      /* FIXME: change syntax or add an optional attribute like "inotify:no".
         pipefs is labeled as "remote" so that tail always polls,
         but it isn't really remote file system type.  */
      प्रत्यागम "pipefs";
    पक्ष S_MAGIC_PPC_CMM: /* 0xC7571590 local */
      प्रत्यागम "ppc-cmm-fs";
    पक्ष S_MAGIC_PRL_FS: /* 0x7C7C6673 remote */
      प्रत्यागम "prl_fs";
    पक्ष S_MAGIC_PROC: /* 0x9FA0 local */
      प्रत्यागम "proc";
    पक्ष S_MAGIC_PSTOREFS: /* 0x6165676C local */
      प्रत्यागम "pstorefs";
    पक्ष S_MAGIC_QNX4: /* 0x002F local */
      प्रत्यागम "qnx4";
    पक्ष S_MAGIC_QNX6: /* 0x68191122 local */
      प्रत्यागम "qnx6";
    पक्ष S_MAGIC_RAMFS: /* 0x858458F6 local */
      प्रत्यागम "ramfs";
    पक्ष S_MAGIC_RDTGROUP: /* 0x07655821 local */
      प्रत्यागम "rdt";
    पक्ष S_MAGIC_REISERFS: /* 0x52654973 local */
      प्रत्यागम "reiserfs";
    पक्ष S_MAGIC_ROMFS: /* 0x7275 local */
      प्रत्यागम "romfs";
    पक्ष S_MAGIC_RPC_PIPEFS: /* 0x67596969 local */
      प्रत्यागम "rpc_pipefs";
    पक्ष S_MAGIC_SDCARDFS: /* 0x5DCA2DF5 local */
      प्रत्यागम "sdcardfs";
    पक्ष S_MAGIC_SECRETMEM: /* 0x5345434D local */
      प्रत्यागम "secretmem";
    पक्ष S_MAGIC_SECURITYFS: /* 0x73636673 local */
      प्रत्यागम "securityfs";
    पक्ष S_MAGIC_SELINUX: /* 0xF97CFF8C local */
      प्रत्यागम "selinux";
    पक्ष S_MAGIC_SMACK: /* 0x43415D53 local */
      प्रत्यागम "smackfs";
    पक्ष S_MAGIC_SMB: /* 0x517B remote */
      प्रत्यागम "smb";
    पक्ष S_MAGIC_SMB2: /* 0xFE534D42 remote */
      प्रत्यागम "smb2";
    पक्ष S_MAGIC_SNFS: /* 0xBEEFDEAD remote */
      प्रत्यागम "snfs";
    पक्ष S_MAGIC_SOCKFS: /* 0x534F434B local */
      प्रत्यागम "sockfs";
    पक्ष S_MAGIC_SQUASHFS: /* 0x73717368 local */
      प्रत्यागम "squashfs";
    पक्ष S_MAGIC_SYSFS: /* 0x62656572 local */
      प्रत्यागम "sysfs";
    पक्ष S_MAGIC_SYSV2: /* 0x012FF7B6 local */
      प्रत्यागम "sysv2";
    पक्ष S_MAGIC_SYSV4: /* 0x012FF7B5 local */
      प्रत्यागम "sysv4";
    पक्ष S_MAGIC_TMPFS: /* 0x01021994 local */
      प्रत्यागम "tmpfs";
    पक्ष S_MAGIC_TRACEFS: /* 0x74726163 local */
      प्रत्यागम "tracefs";
    पक्ष S_MAGIC_UBIFS: /* 0x24051905 local */
      प्रत्यागम "ubifs";
    पक्ष S_MAGIC_UDF: /* 0x15013346 local */
      प्रत्यागम "udf";
    पक्ष S_MAGIC_UFS: /* 0x00011954 local */
      प्रत्यागम "ufs";
    पक्ष S_MAGIC_UFS_BYTESWAPPED: /* 0x54190100 local */
      प्रत्यागम "ufs";
    पक्ष S_MAGIC_USBDEVFS: /* 0x9FA2 local */
      प्रत्यागम "usbdevfs";
    पक्ष S_MAGIC_V9FS: /* 0x01021997 local */
      प्रत्यागम "v9fs";
    पक्ष S_MAGIC_VBOXSF: /* 0x786F4256 remote */
      प्रत्यागम "vboxsf";
    पक्ष S_MAGIC_VMHGFS: /* 0xBACBACBC remote */
      प्रत्यागम "vmhgfs";
    पक्ष S_MAGIC_VXFS: /* 0xA501FCF5 remote */
      /* Veritas File System can run in single instance or clustered mode,
         so mark as remote to cater for the latter case.  */
      प्रत्यागम "vxfs";
    पक्ष S_MAGIC_VZFS: /* 0x565A4653 local */
      प्रत्यागम "vzfs";
    पक्ष S_MAGIC_WSLFS: /* 0x53464846 local */
      प्रत्यागम "wslfs";
    पक्ष S_MAGIC_XENFS: /* 0xABBA1974 local */
      प्रत्यागम "xenfs";
    पक्ष S_MAGIC_XENIX: /* 0x012FF7B4 local */
      प्रत्यागम "xenix";
    पक्ष S_MAGIC_XFS: /* 0x58465342 local */
      प्रत्यागम "xfs";
    पक्ष S_MAGIC_XIAFS: /* 0x012FD16D local */
      प्रत्यागम "xia";
    पक्ष S_MAGIC_Z3FOLD: /* 0x0033 local */
      प्रत्यागम "z3fold";
    पक्ष S_MAGIC_ZFS: /* 0x2FC12FC1 local */
      प्रत्यागम "zfs";
    पक्ष S_MAGIC_ZONEFS: /* 0x5A4F4653 local */
      प्रत्यागम "zonefs";
    पक्ष S_MAGIC_ZSMALLOC: /* 0x58295829 local */
      प्रत्यागम "zsmallocfs";


# elif __GNU__
    पक्ष FSTYPE_UFS:
      प्रत्यागम "ufs";
    पक्ष FSTYPE_NFS:
      प्रत्यागम "nfs";
    पक्ष FSTYPE_GFS:
      प्रत्यागम "gfs";
    पक्ष FSTYPE_LFS:
      प्रत्यागम "lfs";
    पक्ष FSTYPE_SYSV:
      प्रत्यागम "sysv";
    पक्ष FSTYPE_FTP:
      प्रत्यागम "ftp";
    पक्ष FSTYPE_TAR:
      प्रत्यागम "tar";
    पक्ष FSTYPE_AR:
      प्रत्यागम "ar";
    पक्ष FSTYPE_CPIO:
      प्रत्यागम "cpio";
    पक्ष FSTYPE_MSLOSS:
      प्रत्यागम "msloss";
    पक्ष FSTYPE_CPM:
      प्रत्यागम "cpm";
    पक्ष FSTYPE_HFS:
      प्रत्यागम "hfs";
    पक्ष FSTYPE_DTFS:
      प्रत्यागम "dtfs";
    पक्ष FSTYPE_GRFS:
      प्रत्यागम "grfs";
    पक्ष FSTYPE_TERM:
      प्रत्यागम "term";
    पक्ष FSTYPE_DEV:
      प्रत्यागम "dev";
    पक्ष FSTYPE_PROC:
      प्रत्यागम "proc";
    पक्ष FSTYPE_IFSOCK:
      प्रत्यागम "ifsock";
    पक्ष FSTYPE_AFS:
      प्रत्यागम "afs";
    पक्ष FSTYPE_DFS:
      प्रत्यागम "dfs";
    पक्ष FSTYPE_PROC9:
      प्रत्यागम "proc9";
    पक्ष FSTYPE_SOCKET:
      प्रत्यागम "socket";
    पक्ष FSTYPE_MISC:
      प्रत्यागम "misc";
    पक्ष FSTYPE_EXT2FS:
      प्रत्यागम "ext2/ext3";
    पक्ष FSTYPE_HTTP:
      प्रत्यागम "http";
    पक्ष FSTYPE_MEMFS:
      प्रत्यागम "memfs";
    पक्ष FSTYPE_ISO9660:
      प्रत्यागम "iso9660";
# endif
    स्वभाव:
      {
        unsigned long पूर्णाङ्क type = statfsbuf->f_type;
        स्थिर वर्ण buf[परिमाण "UNKNOWN (0x%lx)" - 3
                        + (परिमाण type * CHAR_BIT + 3) / 4];
        sprintf (buf, "UNKNOWN (0x%lx)", type);
        प्रत्यागम buf;
      }
    }
#endif
}

NODISCARD
स्थिर वर्ण *
human_access (संरचना stat ध्रुव *statbuf)
{
  स्थिर वर्ण modebuf[12];
  filemodestring (statbuf, modebuf);
  modebuf[10] = 0;
  प्रत्यागम modebuf;
}

NODISCARD
स्थिर वर्ण *
human_time (संरचना timespec t)
{
  /* STR must be at least INT_BUFSIZE_BOUND (intmax_t) big, either
     because localtime_rz fails, or because the time zone is truly
     outlandish so that %z expands to a long string.  */
  स्थिर वर्ण str[INT_BUFSIZE_BOUND (intmax_t)
                  + INT_STRLEN_BOUND (पूर्णाङ्क) /* YYYY */
                  + 1 /* because YYYY might equal INT_MAX + 1900 */
                  + परिमाण "-MM-DD HH:MM:SS.NNNNNNNNN +"];
  स्थिर timezone_t tz;
  जइ (!tz)
    tz = tzalloc (getenv ("TZ"));
  संरचना tm tm;
  पूर्णाङ्क ns = t.tv_nsec;
  जइ (localtime_rz (tz, &t.tv_sec, &tm))
    nstrftime (str, परिमाण str, "%Y-%m-%d %H:%M:%S.%N %z", &tm, tz, ns);
  अण्णहा
    {
      वर्ण secbuf[INT_BUFSIZE_BOUND (intmax_t)];
      sprintf (str, "%s.%09d", timetostr (t.tv_sec, secbuf), ns);
    }
  प्रत्यागम str;
}

/* PFORMAT points to a '%' followed by a prefix of a format, all of
   size PREFIX_LEN.  The flags allowed for this format are
   ALLOWED_FLAGS; remove other printf flags from the prefix, then
   append SUFFIX.  */
स्थिर शून्य
make_format (वर्ण *pformat, size_t prefix_len, वर्ण ध्रुव *allowed_flags,
             वर्ण ध्रुव *suffix)
{
  वर्ण *dst = pformat + 1;
  वर्ण ध्रुव *src;
  वर्ण ध्रुव *srclim = pformat + prefix_len;
  प्रति (src = dst; src < srclim && strchr (printf_flags, *src); src++)
    जइ (strchr (allowed_flags, *src))
      *dst++ = *src;
  जाव (src < srclim)
    *dst++ = *src++;
  strcpy (dst, suffix);
}

स्थिर शून्य
out_string (वर्ण *pformat, size_t prefix_len, वर्ण ध्रुव *arg)
{
  make_format (pformat, prefix_len, "-", "s");
  printf (pformat, arg);
}
स्थिर पूर्णाङ्क
out_int (वर्ण *pformat, size_t prefix_len, intmax_t arg)
{
  make_format (pformat, prefix_len, "'-+ 0", "jd");
  प्रत्यागम printf (pformat, arg);
}
स्थिर पूर्णाङ्क
out_uint (वर्ण *pformat, size_t prefix_len, uintmax_t arg)
{
  make_format (pformat, prefix_len, "'-0", "ju");
  प्रत्यागम printf (pformat, arg);
}
स्थिर शून्य
out_uint_o (वर्ण *pformat, size_t prefix_len, uintmax_t arg)
{
  make_format (pformat, prefix_len, "-#0", "jo");
  printf (pformat, arg);
}
स्थिर शून्य
out_uint_x (वर्ण *pformat, size_t prefix_len, uintmax_t arg)
{
  make_format (pformat, prefix_len, "-#0", "jx");
  printf (pformat, arg);
}
स्थिर पूर्णाङ्क
out_minus_zero (वर्ण *pformat, size_t prefix_len)
{
  make_format (pformat, prefix_len, "'-+ 0", ".0f");
  प्रत्यागम printf (pformat, -0.25);
}

/* Output the number of seconds since the Epoch, using a format that
   acts like printf's %f format.  */
स्थिर शून्य
out_epoch_sec (वर्ण *pformat, size_t prefix_len,
               संरचना timespec arg)
{
  वर्ण *dot = memchr (pformat, '.', prefix_len);
  size_t sec_prefix_len = prefix_len;
  पूर्णाङ्क width = 0;
  पूर्णाङ्क precision = 0;
  bool frac_वामः_adjust = false;

  जइ (dot)
    {
      sec_prefix_len = dot - pformat;
      pformat[prefix_len] = '\0';

      जइ (c_isdigit (dot[1]))
        {
          long पूर्णाङ्क lprec = strtol (dot + 1, रिक्त, 10);
          precision = (lprec <= INT_MAX ? lprec : INT_MAX);
        }
      अण्णहा
        {
          precision = 9;
        }

      जइ (precision && c_isdigit (dot[-1]))
        {
          /* If a nontrivial width is given, subtract the width of the
             decimal point and PRECISION digits that will be output
             later.  */
          वर्ण *p = dot;
          *dot = '\0';

          करेहि
            --p;
          जाव (c_isdigit (p[-1]));

          long पूर्णाङ्क lwidth = strtol (p, रिक्त, 10);
          width = (lwidth <= INT_MAX ? lwidth : INT_MAX);
          जइ (1 < width)
            {
              p += (*p == '0');
              sec_prefix_len = p - pformat;
              पूर्णाङ्क w_d = (decimal_point_len < width
                         ? width - decimal_point_len
                         : 0);
              जइ (1 < w_d)
                {
                  पूर्णाङ्क w = w_d - precision;
                  जइ (1 < w)
                    {
                      वर्ण *dst = pformat;
                      प्रति (वर्ण ध्रुव *src = dst; src < p; src++)
                        {
                          जइ (*src == '-')
                            frac_वामः_adjust = true;
                          अण्णहा
                            *dst++ = *src;
                        }
                      sec_prefix_len =
                        (dst - pformat
                         + (frac_वामः_adjust ? 0 : sprintf (dst, "%d", w)));
                    }
                }
            }
        }
    }

  पूर्णाङ्क भाजकः = 1;
  प्रति (पूर्णाङ्क i = precision; i < 9; i++)
    भाजकः *= 10;
  पूर्णाङ्क frac_sec = arg.tv_nsec / भाजकः;
  पूर्णाङ्क int_len;

  जइ (TYPE_SIGNED (time_t))
    {
      bool minus_zero = false;
      जइ (arg.tv_sec < 0 && arg.tv_nsec != 0)
        {
          पूर्णाङ्क frac_sec_modulus = 1000000000 / भाजकः;
          frac_sec = (frac_sec_modulus - frac_sec
                      - (arg.tv_nsec % भाजकः != 0));
          arg.tv_sec += (frac_sec != 0);
          minus_zero = (arg.tv_sec == 0);
        }
      int_len = (minus_zero
                 ? out_minus_zero (pformat, sec_prefix_len)
                 : out_int (pformat, sec_prefix_len, arg.tv_sec));
    }
  अण्णहा
    int_len = out_uint (pformat, sec_prefix_len, arg.tv_sec);

  जइ (precision)
    {
      पूर्णाङ्क prec = (precision < 9 ? precision : 9);
      पूर्णाङ्क trailing_prec = precision - prec;
      पूर्णाङ्क ilen = (int_len < 0 ? 0 : int_len);
      पूर्णाङ्क trailing_width = (ilen < width && decimal_point_len < width - ilen
                            ? width - ilen - decimal_point_len - prec
                            : 0);
      printf ("%s%.*d%-*.*d", decimal_point, prec, frac_sec,
              trailing_width, trailing_prec, 0);
    }
}

/* Print the context information of FILENAME, and return true iff the
   context could not be obtained.  */
NODISCARD
स्थिर bool
out_file_context (वर्ण *pformat, size_t prefix_len, वर्ण ध्रुव *filename)
{
  वर्ण *scontext;
  bool fail = false;

  जइ ((follow_links
       ? getfilecon (filename, &scontext)
       : lgetfilecon (filename, &scontext)) < 0)
    {
      error (0, errno, _("failed to get security context of %s"),
             quoteaf (filename));
      scontext = रिक्त;
      fail = true;
    }
  strcpy (pformat + prefix_len, "s");
  printf (pformat, (scontext ? scontext : "?"));
  जइ (scontext)
    freecon (scontext);
  प्रत्यागम fail;
}

/* Set the quoting style once it is needed.  */
स्थिर शून्य
initialize_quoting_style (शून्य)
{
  स्थिर bool initialized;
  जइ (!initialized)
    {
      initialized = true;
      पूर्णाङ्क qs = getenv_quoting_style ();
      set_quoting_style (रिक्त, 0 <= qs ? qs : shell_escape_quoting_style);
    }
}

/* Print statfs info.  Return zero upon success, nonzero upon failure.  */
NODISCARD
स्थिर bool
print_statfs (वर्ण *pformat, size_t prefix_len, MAYBE_UNUSED वर्ण mod, वर्ण m,
              MAYBE_UNUSED पूर्णाङ्क fd, वर्ण ध्रुव *filename,
              शून्य ध्रुव *data)
{
  STRUCT_STATVFS ध्रुव *statfsbuf = data;
  bool fail = false;

  विकल्प (m)
    {
    पक्ष 'n':
      जइ (mod == 'Q')
        {
          initialize_quoting_style ();
          filename = quoteN (filename);
        }
      out_string (pformat, prefix_len, filename);
      भङ्ग;

    पक्ष 'i':
      {
#जइ STRUCT_STATXFS_F_FSID_IS_INTEGER
        uintmax_t fsid = statfsbuf->f_fsid;
#अण्णहा
        प्रकारनिर्देश unsigned पूर्णाङ्क fsid_word;
        static_assert (alignof (STRUCT_STATVFS) % alignof (fsid_word) == 0);
        static_assert (offsetof (STRUCT_STATVFS, f_fsid) % alignof (fsid_word)
                       == 0);
        static_assert (परिमाण statfsbuf->f_fsid % alignof (fsid_word) == 0);
        fsid_word ध्रुव *p = (fsid_word *) &statfsbuf->f_fsid;

        /* Assume a little-endian word order, as that is compatible
           with glibc's statvfs implementation.  */
        uintmax_t fsid = 0;
        पूर्णाङ्क शब्दाः = परिमाण statfsbuf->f_fsid / परिमाण *p;
        प्रति (पूर्णाङ्क i = 0; i < शब्दाः && i * परिमाण *p < परिमाण fsid; i++)
          {
            uintmax_t u = p[शब्दाः - 1 - i];
            fsid |= u << (i * CHAR_BIT * परिमाण *p);
          }
#endif
        out_uint_x (pformat, prefix_len, fsid);
      }
      भङ्ग;

    पक्ष 'l':
      OUT_NAMEMAX (pformat, prefix_len, SB_F_NAMEMAX (statfsbuf));
      भङ्ग;
    पक्ष 't':
#जइ HAVE_STRUCT_STATXFS_F_TYPE
      out_uint_x (pformat, prefix_len, statfsbuf->f_type);
#अण्णहा
      fputc ('?', stdout);
#endif
      भङ्ग;
    पक्ष 'T':
      out_string (pformat, prefix_len, human_fstype (statfsbuf));
      भङ्ग;
    पक्ष 'b':
      out_int (pformat, prefix_len, statfsbuf->f_blocks);
      भङ्ग;
    पक्ष 'f':
      out_int (pformat, prefix_len, statfsbuf->f_bfree);
      भङ्ग;
    पक्ष 'a':
      out_int (pformat, prefix_len, statfsbuf->f_bavail);
      भङ्ग;
    पक्ष 's':
      out_uint (pformat, prefix_len, statfsbuf->f_bsize);
      भङ्ग;
    पक्ष 'S':
      {
        uintmax_t frsize = STATFS_FRSIZE (statfsbuf);
        जइ (! frsize)
          frsize = statfsbuf->f_bsize;
        out_uint (pformat, prefix_len, frsize);
      }
      भङ्ग;
    पक्ष 'c':
      out_uint (pformat, prefix_len, statfsbuf->f_files);
      भङ्ग;
    पक्ष 'd':
      out_int (pformat, prefix_len, statfsbuf->f_ffree);
      भङ्ग;
    स्वभाव:
      fputc ('?', stdout);
      भङ्ग;
    }
  प्रत्यागम fail;
}

/* Return any bind mounted source for a path.
   The caller should not free the returned buffer.
   Return NULL if no bind mount found.  */
NODISCARD
स्थिर वर्ण ध्रुव *
find_bind_mount (वर्ण ध्रुव * name)
{
  वर्ण ध्रुव * bind_mount = रिक्त;

  स्थिर संरचना mount_entry *mount_list;
  स्थिर bool tried_mount_list = false;
  जइ (!tried_mount_list) /* attempt/warn once per process.  */
    {
      जइ (!(mount_list = read_file_system_list (false)))
        error (0, errno, "%s", _("cannot read table of mounted file systems"));
      tried_mount_list = true;
    }

  संरचना stat name_stats;
  जइ (stat (name, &name_stats) != 0)
    प्रत्यागम रिक्त;

  प्रति (संरचना mount_entry *me = mount_list; me; me = me->me_next)
    {
      जइ (me->me_dummy && me->me_devname[0] == '/'
          && streq (me->me_mountdir, name))
        {
          संरचना stat dev_stats;

          जइ (stat (me->me_devname, &dev_stats) == 0
              && psame_inode (&name_stats, &dev_stats))
            {
              bind_mount = me->me_devname;
              भङ्ग;
            }
        }
    }

  प्रत्यागम bind_mount;
}

/* Print mount point.  Return zero upon success, nonzero upon failure.  */
NODISCARD
स्थिर bool
out_mount_point (वर्ण ध्रुव *filename, वर्ण *pformat, size_t prefix_len,
                 ध्रुव संरचना stat *statp)
{

  वर्ण ध्रुव *np = "?", *bp = रिक्त;
  वर्ण *mp = रिक्त;
  bool fail = true;

  /* Look for bind mounts first.  Note we output the immediate alias,
     rather than further resolving to a base device mount point.  */
  जइ (follow_links || !S_ISLNK (statp->st_mode))
    {
      वर्ण *resolved = canonicalize_file_name (filename);
      जइ (!resolved)
        {
          error (0, errno, _("failed to canonicalize %s"), quoteaf (filename));
          गच्छ print_mount_point;
        }
      bp = find_bind_mount (resolved);
      मुक्त (resolved);
      जइ (bp)
        {
          fail = false;
          गच्छ print_mount_point;
        }
    }

  /* If there is no direct bind mount, then navigate
     back up the tree looking for a device change.
     Note we don't detect if any of the directory components
     are bind mounted to the same device, but that's OK
     since we've not directly queried them.  */
  जइ ((mp = find_mount_point (filename, statp)))
    {
      /* This dir might be bind mounted to another device,
         so we resolve the bound source in that case also.  */
      bp = find_bind_mount (mp);
      fail = false;
    }

print_mount_point:

  out_string (pformat, prefix_len, bp ? bp : mp ? mp : np);
  मुक्त (mp);
  प्रत्यागम fail;
}

/* Map a TS with negative TS.tv_nsec to {0,0}.  */
स्थिर अन्तःस्थ संरचना timespec
neg_to_zero (संरचना timespec ts)
{
  जइ (0 <= ts.tv_nsec)
    प्रत्यागम ts;
  संरचना timespec z = {0};
  प्रत्यागम z;
}

/* Output a single-character \ escape.  */

स्थिर शून्य
print_esc_char (वर्ण c)
{
  विकल्प (c)
    {
    पक्ष 'a':			/* Alert. */
      c ='\a';
      भङ्ग;
    पक्ष 'b':			/* Backspace. */
      c ='\b';
      भङ्ग;
    पक्ष 'e':			/* Escape. */
      c ='\x1B';
      भङ्ग;
    पक्ष 'f':			/* Form feed. */
      c ='\f';
      भङ्ग;
    पक्ष 'n':			/* New line. */
      c ='\n';
      भङ्ग;
    पक्ष 'r':			/* Carriage return. */
      c ='\r';
      भङ्ग;
    पक्ष 't':			/* Horizontal tab. */
      c ='\t';
      भङ्ग;
    पक्ष 'v':			/* Vertical tab. */
      c ='\v';
      भङ्ग;
    पक्ष '"':
    पक्ष '\\':
      भङ्ग;
    स्वभाव:
      error (0, 0, _("warning: unrecognized escape '\\%c'"), c);
      भङ्ग;
    }
  putchar (c);
}

ATTRIBUTE_PURE
स्थिर size_t
format_code_offset (वर्ण ध्रुव *directive)
{
  size_t len = strspn (directive + 1, printf_flags);
  वर्ण ध्रुव *fmt_char = directive + len + 1;
  fmt_char += strspn (fmt_char, digits);
  जइ (*fmt_char == '.')
    fmt_char += 1 + strspn (fmt_char + 1, digits);
  प्रत्यागम fmt_char - directive;
}

/* Print the information specified by the format string, FORMAT,
   calling PRINT_FUNC for each %-directive encountered.
   Return zero upon success, nonzero upon failure.  */
NODISCARD
स्थिर bool
print_it (वर्ण ध्रुव *format, पूर्णाङ्क fd, वर्ण ध्रुव *filename,
          bool (*print_func) (वर्ण *, size_t, वर्ण, वर्ण,
                              पूर्णाङ्क, वर्ण ध्रुव *, शून्य ध्रुव *),
          शून्य ध्रुव *data)
{
  bool fail = false;

  /* Add 2 to accommodate our conversion of the stat '%s' format string
     to the longer printf '%llu' one.  */
  गणना
    {
      MAX_ADDITIONAL_BYTES =
        (MAX (परिमाण "jd",
              MAX (परिमाण "jo", MAX (परिमाण "ju", परिमाण "jx")))
         - 1)
    };
  size_t n_alloc = strlen (format) + MAX_ADDITIONAL_BYTES + 1;
  वर्ण *dest = xmalloc (n_alloc);
  प्रति (वर्ण ध्रुव *b = format; *b; b++)
    {
      विकल्प (*b)
        {
        पक्ष '%':
          {
            size_t len = format_code_offset (b);
            वर्ण fmt_char = *(b + len);
            वर्ण mod_char = 0;
            memcpy (dest, b, len);
            b += len;

            विकल्प (fmt_char)
              {
              पक्ष '\0':
                --b;
                FALLTHROUGH;
              पक्ष '%':
                जइ (1 < len)
                  {
                    dest[len] = fmt_char;
                    dest[len + 1] = '\0';
                    error (EXIT_FAILURE, 0, _("%s: invalid directive"),
                           quote (dest));
                  }
                putchar ('%');
                भङ्ग;
              पक्ष 'H':
              पक्ष 'L':
              पक्ष 'Q':
                mod_char = fmt_char;
                fmt_char = *(b + 1);
                जइ ((mod_char == 'Q' && fmt_char == 'n')
                    || (print_func == print_stat
                        && (mod_char == 'H' || mod_char == 'L')
                        && (fmt_char == 'd' || fmt_char == 'r')))
                  {
                    b++;
                  }
                अण्णहा
                  {
                    fmt_char = mod_char;
                    mod_char = 0;
                  }
                FALLTHROUGH;
              स्वभाव:
                fail |= print_func (dest, len, mod_char, fmt_char,
                                    fd, filename, data);
                भङ्ग;
              }
            भङ्ग;
          }

        पक्ष '\\':
          जइ ( ! interpret_backslash_escapes)
            {
              putchar ('\\');
              भङ्ग;
            }
          ++b;
          जइ (isoct (*b))
            {
              पूर्णाङ्क esc_मानम् = fromoct (*b);
              पूर्णाङ्क esc_length = 1;	/* number of octal digits */
              प्रति (++b; esc_length < 3 && isoct (*b);
                   ++esc_length, ++b)
                {
                  esc_मानम् = esc_मानम् * 8 + fromoct (*b);
                }
              putchar (esc_मानम्);
              --b;
            }
          अण्णहा जइ (*b == 'x' && c_isxdigit (b[1]))
            {
              पूर्णाङ्क esc_मानम् = fromhex (b[1]);	/* Value of \xhh escape. */
              /* A hexadecimal \xhh escape sequence must have
                 1 or 2 hex. digits.  */
              ++b;
              जइ (c_isxdigit (b[1]))
                {
                  ++b;
                  esc_मानम् = esc_मानम् * 16 + fromhex (*b);
                }
              putchar (esc_मानम्);
            }
          अण्णहा जइ (*b == '\0')
            {
              error (0, 0, _("warning: backslash at end of format"));
              putchar ('\\');
              /* Arrange to exit the loop.  */
              --b;
            }
          अण्णहा
            {
              print_esc_char (*b);
            }
          भङ्ग;

        स्वभाव:
          putchar (*b);
          भङ्ग;
        }
    }
  मुक्त (dest);

  fputs (trailing_delim, stdout);

  प्रत्यागम fail;
}

/* Stat the file system and print what we find.  */
NODISCARD
स्थिर bool
do_statfs (वर्ण ध्रुव *filename, वर्ण ध्रुव *format)
{
  STRUCT_STATVFS statfsbuf;

  जइ (streq (filename, "-"))
    {
      error (0, 0, _("using %s to denote standard input does not work"
                     " in file system mode"), quoteaf (filename));
      प्रत्यागम false;
    }

  जइ (STATFS (filename, &statfsbuf) != 0)
    {
      error (0, errno, _("cannot read file system information for %s"),
             quoteaf (filename));
      प्रत्यागम false;
    }

  bool fail = print_it (format, -1, filename, print_statfs, &statfsbuf);
  प्रत्यागम ! fail;
}

संरचना print_args {
  संरचना stat *st;
  संरचना timespec btime;
};

/* Ask statx to avoid syncing? */
स्थिर bool dont_sync;

/* Ask statx to force sync? */
स्थिर bool force_sync;

#जइ USE_STATX
स्थिर unsigned पूर्णाङ्क
fmt_to_mask (वर्ण fmt)
{
  विकल्प (fmt)
    {
    पक्ष 'N':
      प्रत्यागम STATX_MODE;
    पक्ष 'd':
    पक्ष 'D':
      प्रत्यागम STATX_MODE;
    पक्ष 'i':
      प्रत्यागम STATX_INO;
    पक्ष 'a':
    पक्ष 'A':
      प्रत्यागम STATX_MODE;
    पक्ष 'f':
      प्रत्यागम STATX_MODE|STATX_TYPE;
    पक्ष 'F':
      प्रत्यागम STATX_TYPE;
    पक्ष 'h':
      प्रत्यागम STATX_NLINK;
    पक्ष 'u':
    पक्ष 'U':
      प्रत्यागम STATX_UID;
    पक्ष 'g':
    पक्ष 'G':
      प्रत्यागम STATX_GID;
    पक्ष 'm':
      प्रत्यागम STATX_MODE|STATX_INO;
    पक्ष 's':
      प्रत्यागम STATX_SIZE;
    पक्ष 't':
    पक्ष 'T':
      प्रत्यागम STATX_MODE;
    पक्ष 'b':
      प्रत्यागम STATX_BLOCKS;
    पक्ष 'w':
    पक्ष 'W':
      प्रत्यागम STATX_BTIME;
    पक्ष 'x':
    पक्ष 'X':
      प्रत्यागम STATX_ATIME;
    पक्ष 'y':
    पक्ष 'Y':
      प्रत्यागम STATX_MTIME;
    पक्ष 'z':
    पक्ष 'Z':
      प्रत्यागम STATX_CTIME;
    }
  प्रत्यागम 0;
}

ATTRIBUTE_PURE
स्थिर unsigned पूर्णाङ्क
format_to_mask (वर्ण ध्रुव *format)
{
  unsigned पूर्णाङ्क mask = 0;

  प्रति (वर्ण ध्रुव *b = format; *b; b++)
    {
      जइ (*b != '%')
        अनुवर्त;

      b += format_code_offset (b);
      जइ (*b == '\0')
        भङ्ग;
      mask |= fmt_to_mask (*b);
    }
  प्रत्यागम mask;
}

/* statx the file and print what we find */
NODISCARD
स्थिर bool
do_stat (वर्ण ध्रुव *filename, वर्ण ध्रुव *format, वर्ण ध्रुव *format2)
{
  पूर्णाङ्क fd = streq (filename, "-") ? 0 : AT_FDCWD;
  पूर्णाङ्क flags = 0;
  संरचना stat st;
  संरचना statx stx = {0};
  वर्ण ध्रुव *pathname = filename;
  संरचना print_args pa;
  pa.st = &st;
  pa.btime = (संरचना timespec) {.tv_sec = -1, .tv_nsec = -1};

  जइ (AT_FDCWD != fd)
    {
      pathname = "";
      flags = AT_EMPTY_PATH;
    }
  अण्णहा जइ (!follow_links)
    {
      flags = AT_SYMLINK_NOFOLLOW;
    }

  जइ (dont_sync)
    flags |= AT_STATX_DONT_SYNC;
  अण्णहा जइ (force_sync)
    flags |= AT_STATX_FORCE_SYNC;

  जइ (! force_sync)
    flags |= AT_NO_AUTOMOUNT;

  fd = statx (fd, pathname, flags, format_to_mask (format), &stx);
  जइ (fd < 0)
    {
      जइ (flags & AT_EMPTY_PATH)
        error (0, errno, _("cannot stat standard input"));
      अण्णहा
        error (0, errno, _("cannot statx %s"), quoteaf (filename));
      प्रत्यागम false;
    }

  जइ (S_ISBLK (stx.stx_mode) || S_ISCHR (stx.stx_mode))
    format = format2;

  statx_to_stat (&stx, &st);
  जइ (stx.stx_mask & STATX_BTIME)
    pa.btime = statx_timestamp_to_timespec (stx.stx_btime);

  bool fail = print_it (format, fd, filename, print_stat, &pa);
  प्रत्यागम ! fail;
}

#अण्णहा /* USE_STATX */

स्थिर संरचना timespec
get_birthtime (पूर्णाङ्क fd, वर्ण ध्रुव *filename, संरचना stat ध्रुव *st)
{
  संरचना timespec ts = get_stat_birthtime (st);

# जइ HAVE_GETATTRAT
  जइ (ts.tv_nsec < 0)
    {
      nvlist_t *response;
      जइ ((fd < 0
           ? getattrat (AT_FDCWD, XATTR_VIEW_READWRITE, filename, &response)
           : fgetattr (fd, XATTR_VIEW_READWRITE, &response))
          == 0)
        {
          uint64_t *val;
          uint_t n;
          जइ (nvlist_lookup_uint64_array (response, A_CRTIME, &val, &n) == 0
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

  प्रत्यागम ts;
}


/* stat the file and print what we find */
NODISCARD
स्थिर bool
do_stat (वर्ण ध्रुव *filename, वर्ण ध्रुव *format,
         वर्ण ध्रुव *format2)
{
  पूर्णाङ्क fd = streq (filename, "-") ? 0 : -1;
  संरचना stat statbuf;
  संरचना print_args pa;
  pa.st = &statbuf;
  pa.btime = (संरचना timespec) {.tv_sec = -1, .tv_nsec = -1};

  जइ (0 <= fd)
    {
      जइ (fstat (fd, &statbuf) != 0)
        {
          error (0, errno, _("cannot stat standard input"));
          प्रत्यागम false;
        }
    }
  /* We can't use the shorter
     (follow_links?stat:lstat) (filename, &statbug)
     since stat might be a function-like macro.  */
  अण्णहा जइ ((follow_links
            ? stat (filename, &statbuf)
            : lstat (filename, &statbuf)) != 0)
    {
      error (0, errno, _("cannot stat %s"), quoteaf (filename));
      प्रत्यागम false;
    }

  जइ (S_ISBLK (statbuf.st_mode) || S_ISCHR (statbuf.st_mode))
    format = format2;

  bool fail = print_it (format, fd, filename, print_stat, &pa);
  प्रत्यागम ! fail;
}
#endif /* USE_STATX */

/* POSIX requires 'ls' to print file sizes without a sign, even
   when negative.  Be consistent with that.  */

स्थिर uintmax_t
unsigned_file_size (off_t size)
{
  प्रत्यागम size + (size < 0) * ((uintmax_t) OFF_T_MAX - OFF_T_MIN + 1);
}

/* Print stat info.  Return zero upon success, nonzero upon failure.  */
स्थिर bool
print_stat (वर्ण *pformat, size_t prefix_len, वर्ण mod, वर्ण m,
            MAYBE_UNUSED पूर्णाङ्क fd, वर्ण ध्रुव *filename, शून्य ध्रुव *data)
{
  संरचना print_args *parg = (संरचना print_args *) data;
  संरचना stat *statbuf = parg->st;
  संरचना timespec btime = parg->btime;
  संरचना passwd *pw_ent;
  संरचना group *gw_ent;
  bool fail = false;

  विकल्प (m)
    {
    पक्ष 'n':
      जइ (mod == 'Q')
        {
          initialize_quoting_style ();
          filename = quoteN (filename);
        }
      out_string (pformat, prefix_len, filename);
      भङ्ग;
    पक्ष 'N':
      initialize_quoting_style ();
      out_string (pformat, prefix_len, quoteN (filename));
      जइ (S_ISLNK (statbuf->st_mode))
        {
          वर्ण *linkname = areadlink_with_size (filename, statbuf->st_size);
          जइ (linkname == रिक्त)
            {
              error (0, errno, _("cannot read symbolic link %s"),
                     quoteaf (filename));
              प्रत्यागम true;
            }
          printf (" -> ");
          out_string (pformat, prefix_len, quoteN (linkname));
          मुक्त (linkname);
        }
      भङ्ग;
    पक्ष 'd':
      जइ (mod == 'H')
        out_uint (pformat, prefix_len, major (statbuf->st_dev));
      अण्णहा जइ (mod == 'L')
        out_uint (pformat, prefix_len, minor (statbuf->st_dev));
      अण्णहा
        out_uint (pformat, prefix_len, statbuf->st_dev);
      भङ्ग;
    पक्ष 'D':
      out_uint_x (pformat, prefix_len, statbuf->st_dev);
      भङ्ग;
    पक्ष 'i':
      out_uint (pformat, prefix_len, statbuf->st_ino);
      भङ्ग;
    पक्ष 'a':
      out_uint_o (pformat, prefix_len, statbuf->st_mode & CHMOD_MODE_BITS);
      भङ्ग;
    पक्ष 'A':
      out_string (pformat, prefix_len, human_access (statbuf));
      भङ्ग;
    पक्ष 'f':
      out_uint_x (pformat, prefix_len, statbuf->st_mode);
      भङ्ग;
    पक्ष 'F':
      out_string (pformat, prefix_len, file_type (statbuf));
      भङ्ग;
    पक्ष 'h':
      out_uint (pformat, prefix_len, statbuf->st_nlink);
      भङ्ग;
    पक्ष 'u':
      out_uint (pformat, prefix_len, statbuf->st_uid);
      भङ्ग;
    पक्ष 'U':
      pw_ent = getpwuid (statbuf->st_uid);
      out_string (pformat, prefix_len,
                  pw_ent ? pw_ent->pw_name : "UNKNOWN");
      भङ्ग;
    पक्ष 'g':
      out_uint (pformat, prefix_len, statbuf->st_gid);
      भङ्ग;
    पक्ष 'G':
      gw_ent = getgrgid (statbuf->st_gid);
      out_string (pformat, prefix_len,
                  gw_ent ? gw_ent->gr_name : "UNKNOWN");
      भङ्ग;
    पक्ष 'm':
      fail |= out_mount_point (filename, pformat, prefix_len, statbuf);
      भङ्ग;
    पक्ष 's':
      out_uint (pformat, prefix_len, unsigned_file_size (statbuf->st_size));
      भङ्ग;
    पक्ष 'r':
      जइ (mod == 'H')
        out_uint (pformat, prefix_len, major (statbuf->st_rdev));
      अण्णहा जइ (mod == 'L')
        out_uint (pformat, prefix_len, minor (statbuf->st_rdev));
      अण्णहा
        out_uint (pformat, prefix_len, statbuf->st_rdev);
      भङ्ग;
    पक्ष 'R':
      out_uint_x (pformat, prefix_len, statbuf->st_rdev);
      भङ्ग;
    पक्ष 't':
      out_uint_x (pformat, prefix_len, major (statbuf->st_rdev));
      भङ्ग;
    पक्ष 'T':
      out_uint_x (pformat, prefix_len, minor (statbuf->st_rdev));
      भङ्ग;
    पक्ष 'B':
      out_uint (pformat, prefix_len, ST_NBLOCKSIZE);
      भङ्ग;
    पक्ष 'b':
      out_uint (pformat, prefix_len, STP_NBLOCKS (statbuf));
      भङ्ग;
    पक्ष 'o':
      out_uint (pformat, prefix_len, STP_BLKSIZE (statbuf));
      भङ्ग;
    पक्ष 'w':
      {
#जइ ! USE_STATX
        btime = get_birthtime (fd, filename, statbuf);
#endif
        जइ (btime.tv_nsec < 0)
          out_string (pformat, prefix_len, "-");
        अण्णहा
          out_string (pformat, prefix_len, human_time (btime));
      }
      भङ्ग;
    पक्ष 'W':
      {
#जइ ! USE_STATX
        btime = get_birthtime (fd, filename, statbuf);
#endif
        out_epoch_sec (pformat, prefix_len, neg_to_zero (btime));
      }
      भङ्ग;
    पक्ष 'x':
      out_string (pformat, prefix_len, human_time (get_stat_atime (statbuf)));
      भङ्ग;
    पक्ष 'X':
      out_epoch_sec (pformat, prefix_len, get_stat_atime (statbuf));
      भङ्ग;
    पक्ष 'y':
      out_string (pformat, prefix_len, human_time (get_stat_mtime (statbuf)));
      भङ्ग;
    पक्ष 'Y':
      out_epoch_sec (pformat, prefix_len, get_stat_mtime (statbuf));
      भङ्ग;
    पक्ष 'z':
      out_string (pformat, prefix_len, human_time (get_stat_ctime (statbuf)));
      भङ्ग;
    पक्ष 'Z':
      out_epoch_sec (pformat, prefix_len, get_stat_ctime (statbuf));
      भङ्ग;
    पक्ष 'C':
      fail |= out_file_context (pformat, prefix_len, filename);
      भङ्ग;
    स्वभाव:
      fputc ('?', stdout);
      भङ्ग;
    }
  प्रत्यागम fail;
}

/* Return an allocated format string in static storage that
   corresponds to whether FS and TERSE options were declared.  */
स्थिर वर्ण *
default_format (bool fs, bool terse, bool device)
{
  वर्ण *format;
  जइ (fs)
    {
      जइ (terse)
        format = xstrdup (fmt_terse_fs);
      अण्णहा
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
  अण्णहा /* ! fs */
    {
      जइ (terse)
        {
          जइ (0 < is_selinux_enabled ())
            format = xstrdup (fmt_terse_selinux);
          अण्णहा
            format = xstrdup (fmt_terse_regular);
        }
      अण्णहा
        {
          वर्ण *temp;
          /* TRANSLATORS: This string uses format specifiers from
             'stat --help' without --file-system, and NOT from printf.  */
          format = xstrdup (_("\
  File: %N\n\
  Size: %-10s\tBlocks: %-10b IO Block: %-6o %F\n\
"));

          temp = format;
          जइ (device)
            {
              /* TRANSLATORS: This string uses format specifiers from
                 'stat --help' without --file-system, and NOT from printf.  */
              format = xasprintf ("%s%s", format, _("\
" "Device: %Hd,%Ld\tInode: %-10i  Links: %-5h Device type: %Hr,%Lr\n\
"));
            }
          अण्णहा
            {
              /* TRANSLATORS: This string uses format specifiers from
                 'stat --help' without --file-system, and NOT from printf.  */
              format = xasprintf ("%s%s", format, _("\
" "Device: %Hd,%Ld\tInode: %-10i  Links: %h\n\
"));
            }
          मुक्त (temp);

          temp = format;
          /* TRANSLATORS: This string uses format specifiers from
             'stat --help' without --file-system, and NOT from printf.  */
          format = xasprintf ("%s%s", format, _("\
" "Access: (%04a/%10.10A)  Uid: (%5u/%8U)   Gid: (%5g/%8G)\n\
"));
          मुक्त (temp);

          जइ (0 < is_selinux_enabled ())
            {
              temp = format;
              /* TRANSLATORS: This string uses format specifiers from
                 'stat --help' without --file-system, and NOT from printf.  */
              format = xasprintf ("%s%s", format, _("Context: %C\n"));
              मुक्त (temp);
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
          मुक्त (temp);
        }
    }
  प्रत्यागम format;
}

शून्य
usage (पूर्णाङ्क status)
{
  जइ (status != EXIT_SUCCESS)
    emit_try_help ();
  अण्णहा
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
         use the specified FORMAT instead of the स्वभाव;\n\
         output a newline after each use of FORMAT\n\
"));
      oputs (_("\
      --printf=FORMAT\n\
         like --format, but interpret backslash escapes,\n\
         and करेहि not output a mandatory trailing newline;\n\
         जइ you want a newline, include \\n in FORMAT\n\
"));
      oputs (_("\
  -t, --terse\n\
         print the information in terse form\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);

      fputs (_("\n\
The MODE argument of --cached can be: always, never, or स्वभाव.\n\
'always' will use cached attributes जइ available, जाव\n\
'never' will try to synchronize with the latest attributes, and\n\
'default' will leave it up to the underlying file system.\n\
"), stdout);

      fputs (_("\n\
The valid format sequences प्रति files (without --file-system):\n\
\n\
  %a   permission bits in octal (see '#' and '0' printf flags)\n\
  %A   permission bits and file type in human readable form\n\
  %b   सङ्ख्या of blocks allocated (see %B)\n\
  %B   the size in bytes of each block reported by %b\n\
  %C   SELinux security context string\n\
"), stdout);
      fputs (_("\
  %d   device सङ्ख्या in decimal (st_dev)\n\
  %D   device सङ्ख्या in hex (st_dev)\n\
  %Hd  major device सङ्ख्या in decimal\n\
  %Ld  minor device सङ्ख्या in decimal\n\
  %f   raw mode in hex\n\
  %F   file type\n\
  %g   group ID of owner\n\
  %G   group name of owner\n\
"), stdout);
      fputs (_("\
  %h   सङ्ख्या of hard links\n\
  %i   inode सङ्ख्या\n\
  %m   mount point\n\
  %n   file name\n\
  %Qn  quoted file name\n\
  %N   quoted file name with dereference जइ symbolic link\n\
  %o   optimal I/O transfer size hint\n\
  %s   total size, in bytes\n\
  %r   device type in decimal (st_rdev)\n\
  %R   device type in hex (st_rdev)\n\
  %Hr  major device type in decimal, प्रति character/block device special files\n\
  %Lr  minor device type in decimal, प्रति character/block device special files\n\
  %t   major device type in hex, प्रति character/block device special files\n\
  %T   minor device type in hex, प्रति character/block device special files\n\
"), stdout);
      fputs (_("\
  %u   user ID of owner\n\
  %U   user name of owner\n\
  %w   time of file birth, human-readable; - जइ unknown\n\
  %W   time of file birth, seconds since Epoch; 0 जइ unknown\n\
  %x   time of last access, human-readable\n\
  %X   time of last access, seconds since Epoch\n\
  %y   time of last data modification, human-readable\n\
  %Y   time of last data modification, seconds since Epoch\n\
  %z   time of last status change, human-readable\n\
  %Z   time of last status change, seconds since Epoch\n\
\n\
"), stdout);

      fputs (_("\
Valid format sequences प्रति file systems:\n\
\n\
  %a   मुक्त blocks available to non-superuser\n\
  %b   total data blocks in file system\n\
  %c   total file nodes in file system\n\
  %d   मुक्त file nodes in file system\n\
  %f   मुक्त blocks in file system\n\
"), stdout);
      fputs (_("\
  %i   file system ID in hex\n\
  %l   maximum length of filenames\n\
  %n   file name\n\
  %Qn  quoted file name\n\
  %s   block size (प्रति faster transfers)\n\
  %S   fundamental block size (प्रति block counts)\n\
  %t   file system type in hex\n\
  %T   file system type in human readable form\n\
"), stdout);

      printf (_("\n\
--terse is equivalent to the following FORMAT:\n\
    %s\
"),
#जइ HAVE_SELINUX_SELINUX_H
              fmt_terse_selinux
#अण्णहा
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

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण *argv[])
{
  पूर्णाङ्क c;
  bool fs = false;
  bool terse = false;
  वर्ण *format = रिक्त;
  वर्ण *format2;
  bool ok = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  संरचना lconv ध्रुव *locale = localeconv ();
  decimal_point = (locale->decimal_point[0] ? locale->decimal_point : ".");
  decimal_point_len = strlen (decimal_point);

  atexit (close_stdout);

  जाव ((c = getopt_long (argc, argv, "c:fLt", long_options, रिक्त)) != -1)
    {
      विकल्प (c)
        {
        पक्ष PRINTF_OPTION:
          format = optarg;
          interpret_backslash_escapes = true;
          trailing_delim = "";
          भङ्ग;

        पक्ष 'c':
          format = optarg;
          interpret_backslash_escapes = false;
          trailing_delim = "\n";
          भङ्ग;

        पक्ष 'L':
          follow_links = true;
          भङ्ग;

        पक्ष 'f':
          fs = true;
          भङ्ग;

        पक्ष 't':
          terse = true;
          भङ्ग;

        पक्ष 0:
          विकल्प (XARGMATCH ("--cached", optarg, cached_args, cached_modes))
            {
              पक्ष cached_never:
                force_sync = true;
                dont_sync = false;
                भङ्ग;
              पक्ष cached_always:
                force_sync = false;
                dont_sync = true;
                भङ्ग;
              पक्ष cached_default:
                force_sync = false;
                dont_sync = false;
            }
          भङ्ग;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        स्वभाव:
          usage (EXIT_FAILURE);
        }
    }

  जइ (argc == optind)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  जइ (format)
    {
      format2 = format;
    }
  अण्णहा
    {
      format = default_format (fs, terse, /* device= */ false);
      format2 = default_format (fs, terse, /* device= */ true);
    }

  प्रति (पूर्णाङ्क i = optind; i < argc; i++)
    ok &= (fs
           ? do_statfs (argv[i], format)
           : do_stat (argv[i], format, format2));

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
