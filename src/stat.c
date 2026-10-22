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
#सचे ((STAT_STATVFS || STAT_STATVFS64)                                       \
     && (HAVE_STRUCT_STATVFS_F_BASETYPE || HAVE_STRUCT_STATVFS_F_FSTYPENAME \
         || (! HAVE_STRUCT_STATFS_F_FSTYPENAME && HAVE_STRUCT_STATVFS_F_TYPE)))
# define USE_STATVFS 1
#अञ्ञथा
# define USE_STATVFS 0
#endif

#अन्तर्भाव <stdio.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <pwd.h>
#अन्तर्भाव <grp.h>
#सचे USE_STATVFS
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
# सचे HAVE_NFS_NFS_CLNT_H && HAVE_NFS_VFS_H
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

#सचे HAVE_STATX && defined STATX_INO
# define USE_STATX 1
#अञ्ञथा
# define USE_STATX 0
#endif

#सचे USE_STATVFS
# define STRUCT_STATXFS_F_FSID_IS_INTEGER STRUCT_STATVFS_F_FSID_IS_INTEGER
# define HAVE_STRUCT_STATXFS_F_TYPE HAVE_STRUCT_STATVFS_F_TYPE
# सचे HAVE_STRUCT_STATVFS_F_NAMEMAX
#  define SB_F_NAMEMAX(S) ((S)->f_namemax)
# endif
# सचे ! STAT_STATVFS && STAT_STATVFS64
#  define STRUCT_STATVFS संरचना statvfs64
#  define STATFS statvfs64
# अञ्ञथा
#  define STRUCT_STATVFS संरचना statvfs
#  define STATFS statvfs
# endif
# define STATFS_FRSIZE(S) ((S)->f_frsize)
#अञ्ञथा
# define HAVE_STRUCT_STATXFS_F_TYPE HAVE_STRUCT_STATFS_F_TYPE
# सचे HAVE_STRUCT_STATFS_F_NAMELEN
#  define SB_F_NAMEMAX(S) ((S)->f_namelen)
# elif HAVE_STRUCT_STATFS_F_NAMEMAX
#  define SB_F_NAMEMAX(S) ((S)->f_namemax)
# endif
# define STATFS statfs
# सचे HAVE_OS_H /* BeOS */
/* BeOS has a statvfs function, but it does not return sensible values
   for f_files, f_ffree and f_favail, and lacks f_type, f_basetype and
   f_fstypename.  Use 'struct fs_info' instead.  */
NODISCARD
स्थिर पूर्णाङ्क
statfs (वर्ण ध्रुव *filename, संरचना fs_info *buf)
{
  dev_t device = dev_for_path (filename);
  सचे (device < 0)
    {
      errno = (device == B_ENTRY_NOT_FOUND ? ENOENT
               : device == B_BAD_मानम् ? EINVAL
               : device == B_NAME_TOO_LONG ? ENAMETOOLONG
               : device == B_NO_MEMORY ? ENOMEM
               : device == B_FILE_ERROR ? EIO
               : 0);
      पटिनिवत्त -1;
    }
  /* If successful, buf->dev will be == device.  */
  पटिनिवत्त fs_stat_dev (device, buf);
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
# अञ्ञथा
#  define STRUCT_STATVFS संरचना statfs
#  define STRUCT_STATXFS_F_FSID_IS_INTEGER STRUCT_STATFS_F_FSID_IS_INTEGER
#  सचे HAVE_STRUCT_STATFS_F_FRSIZE
#   define STATFS_FRSIZE(S) ((S)->f_frsize)
#  अञ्ञथा
#   define STATFS_FRSIZE(S) 0
#  endif
# endif
#endif

#ifdef SB_F_NAMEMAX
# define OUT_NAMEMAX out_uint
#अञ्ञथा
/* Depending on whether statvfs or statfs is used,
   neither f_namemax or f_namelen may be available.  */
# define SB_F_NAMEMAX(S) "?"
# define OUT_NAMEMAX out_string
#endif

#सचे HAVE_STRUCT_STATVFS_F_BASETYPE
# define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME f_basetype
#अञ्ञथा
# सचे HAVE_STRUCT_STATVFS_F_FSTYPENAME || HAVE_STRUCT_STATFS_F_FSTYPENAME
#  define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME f_fstypename
# elif HAVE_OS_H /* BeOS */
#  define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME fsh_name
# endif
#endif

#सचे HAVE_GETATTRAT
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
  पटिनिवत्त statfsbuf->STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME;
#अञ्ञथा
  विकल्प (statfsbuf->f_type)
    {
# सचे defined __linux__ || defined __ANDROID__

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
      पटिनिवत्त "aafs";
    पक्ष S_MAGIC_ACFS: /* 0x61636673 remote */
      पटिनिवत्त "acfs";
    पक्ष S_MAGIC_ADFS: /* 0xADF5 local */
      पटिनिवत्त "adfs";
    पक्ष S_MAGIC_AFFS: /* 0xADFF local */
      पटिनिवत्त "affs";
    पक्ष S_MAGIC_AFS: /* 0x5346414F remote */
      पटिनिवत्त "afs";
    पक्ष S_MAGIC_ANON_INODE_FS: /* 0x09041934 local */
      पटिनिवत्त "anon-inode FS";
    पक्ष S_MAGIC_AUFS: /* 0x61756673 remote */
      /* FIXME: change syntax or add an optional attribute like "inotify:no".
         The above is labeled as "remote" so that tail always uses polling,
         but this isn't really a remote file system type.  */
      पटिनिवत्त "aufs";
    पक्ष S_MAGIC_AUTOFS: /* 0x0187 local */
      पटिनिवत्त "autofs";
    पक्ष S_MAGIC_BALLOON_KVM: /* 0x13661366 local */
      पटिनिवत्त "balloon-kvm-fs";
    पक्ष S_MAGIC_BCACHEFS: /* 0xCA451A4E local */
      पटिनिवत्त "bcachefs";
    पक्ष S_MAGIC_BEFS: /* 0x42465331 local */
      पटिनिवत्त "befs";
    पक्ष S_MAGIC_BDEVFS: /* 0x62646576 local */
      पटिनिवत्त "bdevfs";
    पक्ष S_MAGIC_BFS: /* 0x1BADFACE local */
      पटिनिवत्त "bfs";
    पक्ष S_MAGIC_BINDERFS: /* 0x6C6F6F70 local */
      पटिनिवत्त "binderfs";
    पक्ष S_MAGIC_BPF_FS: /* 0xCAFE4A11 local */
      पटिनिवत्त "bpf_fs";
    पक्ष S_MAGIC_BINFMTFS: /* 0x42494E4D local */
      पटिनिवत्त "binfmt_misc";
    पक्ष S_MAGIC_BTRFS: /* 0x9123683E local */
      पटिनिवत्त "btrfs";
    पक्ष S_MAGIC_BTRFS_TEST: /* 0x73727279 local */
      पटिनिवत्त "btrfs_test";
    पक्ष S_MAGIC_CEPH: /* 0x00C36400 remote */
      पटिनिवत्त "ceph";
    पक्ष S_MAGIC_CGROUP: /* 0x0027E0EB local */
      पटिनिवत्त "cgroupfs";
    पक्ष S_MAGIC_CGROUP2: /* 0x63677270 local */
      पटिनिवत्त "cgroup2fs";
    पक्ष S_MAGIC_CIFS: /* 0xFF534D42 remote */
      पटिनिवत्त "cifs";
    पक्ष S_MAGIC_CODA: /* 0x73757245 remote */
      पटिनिवत्त "coda";
    पक्ष S_MAGIC_COH: /* 0x012FF7B7 local */
      पटिनिवत्त "coh";
    पक्ष S_MAGIC_CONFIGFS: /* 0x62656570 local */
      पटिनिवत्त "configfs";
    पक्ष S_MAGIC_CRAMFS: /* 0x28CD3D45 local */
      पटिनिवत्त "cramfs";
    पक्ष S_MAGIC_CRAMFS_WEND: /* 0x453DCD28 local */
      पटिनिवत्त "cramfs-wend";
    पक्ष S_MAGIC_DAXFS: /* 0x64646178 local */
      पटिनिवत्त "daxfs";
    पक्ष S_MAGIC_DEBUGFS: /* 0x64626720 local */
      पटिनिवत्त "debugfs";
    पक्ष S_MAGIC_DEVFS: /* 0x1373 local */
      पटिनिवत्त "devfs";
    पक्ष S_MAGIC_DEVMEM: /* 0x454D444D local */
      पटिनिवत्त "devmem";
    पक्ष S_MAGIC_DEVPTS: /* 0x1CD1 local */
      पटिनिवत्त "devpts";
    पक्ष S_MAGIC_DMA_BUF: /* 0x444D4142 local */
      पटिनिवत्त "dma-buf-fs";
    पक्ष S_MAGIC_ECRYPTFS: /* 0xF15F local */
      पटिनिवत्त "ecryptfs";
    पक्ष S_MAGIC_EFIVARFS: /* 0xDE5E81E4 local */
      पटिनिवत्त "efivarfs";
    पक्ष S_MAGIC_EFS: /* 0x00414A53 local */
      पटिनिवत्त "efs";
    पक्ष S_MAGIC_EROFS_V1: /* 0xE0F5E1E2 local */
      पटिनिवत्त "erofs";
    पक्ष S_MAGIC_EXFAT: /* 0x2011BAB0 local */
      पटिनिवत्त "exfat";
    पक्ष S_MAGIC_EXFS: /* 0x45584653 local */
      पटिनिवत्त "exfs";
    पक्ष S_MAGIC_EXOFS: /* 0x5DF5 local */
      पटिनिवत्त "exofs";
    पक्ष S_MAGIC_EXT: /* 0x137D local */
      पटिनिवत्त "ext";
    पक्ष S_MAGIC_EXT2: /* 0xEF53 local */
      पटिनिवत्त "ext2/ext3";
    पक्ष S_MAGIC_EXT2_OLD: /* 0xEF51 local */
      पटिनिवत्त "ext2";
    पक्ष S_MAGIC_F2FS: /* 0xF2F52010 local */
      पटिनिवत्त "f2fs";
    पक्ष S_MAGIC_FAIL_FS: /* 0x4641494C local */
      पटिनिवत्त "failfs";
    पक्ष S_MAGIC_FAT: /* 0x4006 local */
      पटिनिवत्त "fat";
    पक्ष S_MAGIC_FHGFS: /* 0x19830326 remote */
      पटिनिवत्त "fhgfs";
    पक्ष S_MAGIC_FUSE: /* 0x65735546 remote */
      पटिनिवत्त "fuse";
    पक्ष S_MAGIC_FUSECTL: /* 0x65735543 remote */
      पटिनिवत्त "fusectl";
    पक्ष S_MAGIC_FUTEXFS: /* 0x0BAD1DEA local */
      पटिनिवत्त "futexfs";
    पक्ष S_MAGIC_GFS: /* 0x01161970 remote */
      पटिनिवत्त "gfs/gfs2";
    पक्ष S_MAGIC_GPFS: /* 0x47504653 remote */
      पटिनिवत्त "gpfs";
    पक्ष S_MAGIC_GUEST_MEMFD: /* 0x474D454D remote */
      पटिनिवत्त "guest-memfd";
    पक्ष S_MAGIC_HFS: /* 0x4244 local */
      पटिनिवत्त "hfs";
    पक्ष S_MAGIC_HFS_PLUS: /* 0x482B local */
      पटिनिवत्त "hfs+";
    पक्ष S_MAGIC_HFS_X: /* 0x4858 local */
      पटिनिवत्त "hfsx";
    पक्ष S_MAGIC_HOSTFS: /* 0x00C0FFEE local */
      पटिनिवत्त "hostfs";
    पक्ष S_MAGIC_HPFS: /* 0xF995E849 local */
      पटिनिवत्त "hpfs";
    पक्ष S_MAGIC_HUGETLBFS: /* 0x958458F6 local */
      पटिनिवत्त "hugetlbfs";
    पक्ष S_MAGIC_MTD_INODE_FS: /* 0x11307854 local */
      पटिनिवत्त "inodefs";
    पक्ष S_MAGIC_IBRIX: /* 0x013111A8 remote */
      पटिनिवत्त "ibrix";
    पक्ष S_MAGIC_INOTIFYFS: /* 0x2BAD1DEA local */
      पटिनिवत्त "inotifyfs";
    पक्ष S_MAGIC_ISOFS: /* 0x9660 local */
      पटिनिवत्त "isofs";
    पक्ष S_MAGIC_ISOFS_R_WIN: /* 0x4004 local */
      पटिनिवत्त "isofs";
    पक्ष S_MAGIC_ISOFS_WIN: /* 0x4000 local */
      पटिनिवत्त "isofs";
    पक्ष S_MAGIC_JFFS: /* 0x07C0 local */
      पटिनिवत्त "jffs";
    पक्ष S_MAGIC_JFFS2: /* 0x72B6 local */
      पटिनिवत्त "jffs2";
    पक्ष S_MAGIC_JFS: /* 0x3153464A local */
      पटिनिवत्त "jfs";
    पक्ष S_MAGIC_KAFS: /* 0x6B414653 remote */
      पटिनिवत्त "k-afs";
    पक्ष S_MAGIC_LOGFS: /* 0xC97E8168 local */
      पटिनिवत्त "logfs";
    पक्ष S_MAGIC_LUSTRE: /* 0x0BD00BD0 remote */
      पटिनिवत्त "lustre";
    पक्ष S_MAGIC_M1FS: /* 0x5346314D local */
      पटिनिवत्त "m1fs";
    पक्ष S_MAGIC_MINIX: /* 0x137F local */
      पटिनिवत्त "minix";
    पक्ष S_MAGIC_MINIX_30: /* 0x138F local */
      पटिनिवत्त "minix (30 char.)";
    पक्ष S_MAGIC_MINIX_V2: /* 0x2468 local */
      पटिनिवत्त "minix v2";
    पक्ष S_MAGIC_MINIX_V2_30: /* 0x2478 local */
      पटिनिवत्त "minix v2 (30 char.)";
    पक्ष S_MAGIC_MINIX_V3: /* 0x4D5A local */
      पटिनिवत्त "minix3";
    पक्ष S_MAGIC_MQUEUE: /* 0x19800202 local */
      पटिनिवत्त "mqueue";
    पक्ष S_MAGIC_MSDOS: /* 0x4D44 local */
      पटिनिवत्त "msdos";
    पक्ष S_MAGIC_NCP: /* 0x564C remote */
      पटिनिवत्त "novell";
    पक्ष S_MAGIC_NFS: /* 0x6969 remote */
      पटिनिवत्त "nfs";
    पक्ष S_MAGIC_NFSD: /* 0x6E667364 remote */
      पटिनिवत्त "nfsd";
    पक्ष S_MAGIC_NILFS: /* 0x3434 local */
      पटिनिवत्त "nilfs";
    पक्ष S_MAGIC_NSFS: /* 0x6E736673 local */
      पटिनिवत्त "nsfs";
    पक्ष S_MAGIC_NTFS: /* 0x5346544E local */
      पटिनिवत्त "ntfs";
    पक्ष S_MAGIC_NULL_FS: /* 0x4E554C4C local */
      पटिनिवत्त "nullfs";
    पक्ष S_MAGIC_OPENPROM: /* 0x9FA1 local */
      पटिनिवत्त "openprom";
    पक्ष S_MAGIC_OCFS2: /* 0x7461636F remote */
      पटिनिवत्त "ocfs2";
    पक्ष S_MAGIC_OVERLAYFS: /* 0x794C7630 remote */
      /* This may overlay remote file systems.
         Also there have been issues reported with inotify and overlayfs,
         so mark as "remote" so that polling is used.  */
      पटिनिवत्त "overlayfs";
    पक्ष S_MAGIC_PANFS: /* 0xAAD7AAEA remote */
      पटिनिवत्त "panfs";
    पक्ष S_MAGIC_PID_FS: /* 0x50494446 local */
      पटिनिवत्त "pidfs";
    पक्ष S_MAGIC_PIPEFS: /* 0x50495045 remote */
      /* FIXME: change syntax or add an optional attribute like "inotify:no".
         pipefs is labeled as "remote" so that tail always polls,
         but it isn't really remote file system type.  */
      पटिनिवत्त "pipefs";
    पक्ष S_MAGIC_PPC_CMM: /* 0xC7571590 local */
      पटिनिवत्त "ppc-cmm-fs";
    पक्ष S_MAGIC_PRL_FS: /* 0x7C7C6673 remote */
      पटिनिवत्त "prl_fs";
    पक्ष S_MAGIC_PROC: /* 0x9FA0 local */
      पटिनिवत्त "proc";
    पक्ष S_MAGIC_PSTOREFS: /* 0x6165676C local */
      पटिनिवत्त "pstorefs";
    पक्ष S_MAGIC_QNX4: /* 0x002F local */
      पटिनिवत्त "qnx4";
    पक्ष S_MAGIC_QNX6: /* 0x68191122 local */
      पटिनिवत्त "qnx6";
    पक्ष S_MAGIC_RAMFS: /* 0x858458F6 local */
      पटिनिवत्त "ramfs";
    पक्ष S_MAGIC_RDTGROUP: /* 0x07655821 local */
      पटिनिवत्त "rdt";
    पक्ष S_MAGIC_REISERFS: /* 0x52654973 local */
      पटिनिवत्त "reiserfs";
    पक्ष S_MAGIC_ROMFS: /* 0x7275 local */
      पटिनिवत्त "romfs";
    पक्ष S_MAGIC_RPC_PIPEFS: /* 0x67596969 local */
      पटिनिवत्त "rpc_pipefs";
    पक्ष S_MAGIC_SDCARDFS: /* 0x5DCA2DF5 local */
      पटिनिवत्त "sdcardfs";
    पक्ष S_MAGIC_SECRETMEM: /* 0x5345434D local */
      पटिनिवत्त "secretmem";
    पक्ष S_MAGIC_SECURITYFS: /* 0x73636673 local */
      पटिनिवत्त "securityfs";
    पक्ष S_MAGIC_SELINUX: /* 0xF97CFF8C local */
      पटिनिवत्त "selinux";
    पक्ष S_MAGIC_SMACK: /* 0x43415D53 local */
      पटिनिवत्त "smackfs";
    पक्ष S_MAGIC_SMB: /* 0x517B remote */
      पटिनिवत्त "smb";
    पक्ष S_MAGIC_SMB2: /* 0xFE534D42 remote */
      पटिनिवत्त "smb2";
    पक्ष S_MAGIC_SNFS: /* 0xBEEFDEAD remote */
      पटिनिवत्त "snfs";
    पक्ष S_MAGIC_SOCKFS: /* 0x534F434B local */
      पटिनिवत्त "sockfs";
    पक्ष S_MAGIC_SQUASHFS: /* 0x73717368 local */
      पटिनिवत्त "squashfs";
    पक्ष S_MAGIC_SYSFS: /* 0x62656572 local */
      पटिनिवत्त "sysfs";
    पक्ष S_MAGIC_SYSV2: /* 0x012FF7B6 local */
      पटिनिवत्त "sysv2";
    पक्ष S_MAGIC_SYSV4: /* 0x012FF7B5 local */
      पटिनिवत्त "sysv4";
    पक्ष S_MAGIC_TMPFS: /* 0x01021994 local */
      पटिनिवत्त "tmpfs";
    पक्ष S_MAGIC_TRACEFS: /* 0x74726163 local */
      पटिनिवत्त "tracefs";
    पक्ष S_MAGIC_UBIFS: /* 0x24051905 local */
      पटिनिवत्त "ubifs";
    पक्ष S_MAGIC_UDF: /* 0x15013346 local */
      पटिनिवत्त "udf";
    पक्ष S_MAGIC_UFS: /* 0x00011954 local */
      पटिनिवत्त "ufs";
    पक्ष S_MAGIC_UFS_BYTESWAPPED: /* 0x54190100 local */
      पटिनिवत्त "ufs";
    पक्ष S_MAGIC_USBDEVFS: /* 0x9FA2 local */
      पटिनिवत्त "usbdevfs";
    पक्ष S_MAGIC_V9FS: /* 0x01021997 local */
      पटिनिवत्त "v9fs";
    पक्ष S_MAGIC_VBOXSF: /* 0x786F4256 remote */
      पटिनिवत्त "vboxsf";
    पक्ष S_MAGIC_VMHGFS: /* 0xBACBACBC remote */
      पटिनिवत्त "vmhgfs";
    पक्ष S_MAGIC_VXFS: /* 0xA501FCF5 remote */
      /* Veritas File System can run in single instance or clustered mode,
         so mark as remote to cater for the latter case.  */
      पटिनिवत्त "vxfs";
    पक्ष S_MAGIC_VZFS: /* 0x565A4653 local */
      पटिनिवत्त "vzfs";
    पक्ष S_MAGIC_WSLFS: /* 0x53464846 local */
      पटिनिवत्त "wslfs";
    पक्ष S_MAGIC_XENFS: /* 0xABBA1974 local */
      पटिनिवत्त "xenfs";
    पक्ष S_MAGIC_XENIX: /* 0x012FF7B4 local */
      पटिनिवत्त "xenix";
    पक्ष S_MAGIC_XFS: /* 0x58465342 local */
      पटिनिवत्त "xfs";
    पक्ष S_MAGIC_XIAFS: /* 0x012FD16D local */
      पटिनिवत्त "xia";
    पक्ष S_MAGIC_Z3FOLD: /* 0x0033 local */
      पटिनिवत्त "z3fold";
    पक्ष S_MAGIC_ZFS: /* 0x2FC12FC1 local */
      पटिनिवत्त "zfs";
    पक्ष S_MAGIC_ZONEFS: /* 0x5A4F4653 local */
      पटिनिवत्त "zonefs";
    पक्ष S_MAGIC_ZSMALLOC: /* 0x58295829 local */
      पटिनिवत्त "zsmallocfs";


# elif __GNU__
    पक्ष FSTYPE_UFS:
      पटिनिवत्त "ufs";
    पक्ष FSTYPE_NFS:
      पटिनिवत्त "nfs";
    पक्ष FSTYPE_GFS:
      पटिनिवत्त "gfs";
    पक्ष FSTYPE_LFS:
      पटिनिवत्त "lfs";
    पक्ष FSTYPE_SYSV:
      पटिनिवत्त "sysv";
    पक्ष FSTYPE_FTP:
      पटिनिवत्त "ftp";
    पक्ष FSTYPE_TAR:
      पटिनिवत्त "tar";
    पक्ष FSTYPE_AR:
      पटिनिवत्त "ar";
    पक्ष FSTYPE_CPIO:
      पटिनिवत्त "cpio";
    पक्ष FSTYPE_MSLOSS:
      पटिनिवत्त "msloss";
    पक्ष FSTYPE_CPM:
      पटिनिवत्त "cpm";
    पक्ष FSTYPE_HFS:
      पटिनिवत्त "hfs";
    पक्ष FSTYPE_DTFS:
      पटिनिवत्त "dtfs";
    पक्ष FSTYPE_GRFS:
      पटिनिवत्त "grfs";
    पक्ष FSTYPE_TERM:
      पटिनिवत्त "term";
    पक्ष FSTYPE_DEV:
      पटिनिवत्त "dev";
    पक्ष FSTYPE_PROC:
      पटिनिवत्त "proc";
    पक्ष FSTYPE_IFSOCK:
      पटिनिवत्त "ifsock";
    पक्ष FSTYPE_AFS:
      पटिनिवत्त "afs";
    पक्ष FSTYPE_DFS:
      पटिनिवत्त "dfs";
    पक्ष FSTYPE_PROC9:
      पटिनिवत्त "proc9";
    पक्ष FSTYPE_SOCKET:
      पटिनिवत्त "socket";
    पक्ष FSTYPE_MISC:
      पटिनिवत्त "misc";
    पक्ष FSTYPE_EXT2FS:
      पटिनिवत्त "ext2/ext3";
    पक्ष FSTYPE_HTTP:
      पटिनिवत्त "http";
    पक्ष FSTYPE_MEMFS:
      पटिनिवत्त "memfs";
    पक्ष FSTYPE_ISO9660:
      पटिनिवत्त "iso9660";
# endif
    स्वभाव:
      {
        unsigned long पूर्णाङ्क type = statfsbuf->f_type;
        स्थिर वर्ण buf[परिमाण "UNKNOWN (0x%lx)" - 3
                        + (परिमाण type * CHAR_BIT + 3) / 4];
        sprintf (buf, "UNKNOWN (0x%lx)", type);
        पटिनिवत्त buf;
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
  पटिनिवत्त modebuf;
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
  सचे (!tz)
    tz = tzalloc (getenv ("TZ"));
  संरचना tm tm;
  पूर्णाङ्क ns = t.tv_nsec;
  सचे (localtime_rz (tz, &t.tv_sec, &tm))
    nstrftime (str, परिमाण str, "%Y-%m-%d %H:%M:%S.%N %z", &tm, tz, ns);
  अञ्ञथा
    {
      वर्ण secbuf[INT_BUFSIZE_BOUND (intmax_t)];
      sprintf (str, "%s.%09d", timetostr (t.tv_sec, secbuf), ns);
    }
  पटिनिवत्त str;
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
  अत्थाय (src = dst; src < srclim && strchr (printf_flags, *src); src++)
    सचे (strchr (allowed_flags, *src))
      *dst++ = *src;
  याव (src < srclim)
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
  पटिनिवत्त printf (pformat, arg);
}
स्थिर पूर्णाङ्क
out_uint (वर्ण *pformat, size_t prefix_len, uintmax_t arg)
{
  make_format (pformat, prefix_len, "'-0", "ju");
  पटिनिवत्त printf (pformat, arg);
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
  पटिनिवत्त printf (pformat, -0.25);
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

  सचे (dot)
    {
      sec_prefix_len = dot - pformat;
      pformat[prefix_len] = '\0';

      सचे (c_isdigit (dot[1]))
        {
          long पूर्णाङ्क lprec = strtol (dot + 1, रिक्त, 10);
          precision = (lprec <= INT_MAX ? lprec : INT_MAX);
        }
      अञ्ञथा
        {
          precision = 9;
        }

      सचे (precision && c_isdigit (dot[-1]))
        {
          /* If a nontrivial width is given, subtract the width of the
             decimal point and PRECISION digits that will be output
             later.  */
          वर्ण *p = dot;
          *dot = '\0';

          करोहि
            --p;
          याव (c_isdigit (p[-1]));

          long पूर्णाङ्क lwidth = strtol (p, रिक्त, 10);
          width = (lwidth <= INT_MAX ? lwidth : INT_MAX);
          सचे (1 < width)
            {
              p += (*p == '0');
              sec_prefix_len = p - pformat;
              पूर्णाङ्क w_d = (decimal_point_len < width
                         ? width - decimal_point_len
                         : 0);
              सचे (1 < w_d)
                {
                  पूर्णाङ्क w = w_d - precision;
                  सचे (1 < w)
                    {
                      वर्ण *dst = pformat;
                      अत्थाय (वर्ण ध्रुव *src = dst; src < p; src++)
                        {
                          सचे (*src == '-')
                            frac_वामः_adjust = true;
                          अञ्ञथा
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
  अत्थाय (पूर्णाङ्क i = precision; i < 9; i++)
    भाजकः *= 10;
  पूर्णाङ्क frac_sec = arg.tv_nsec / भाजकः;
  पूर्णाङ्क int_len;

  सचे (TYPE_SIGNED (time_t))
    {
      bool minus_zero = false;
      सचे (arg.tv_sec < 0 && arg.tv_nsec != 0)
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
  अञ्ञथा
    int_len = out_uint (pformat, sec_prefix_len, arg.tv_sec);

  सचे (precision)
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

  सचे ((follow_links
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
  सचे (scontext)
    freecon (scontext);
  पटिनिवत्त fail;
}

/* Set the quoting style once it is needed.  */
स्थिर शून्य
initialize_quoting_style (शून्य)
{
  स्थिर bool initialized;
  सचे (!initialized)
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
      सचे (mod == 'Q')
        {
          initialize_quoting_style ();
          filename = quoteN (filename);
        }
      out_string (pformat, prefix_len, filename);
      भङ्ग;

    पक्ष 'i':
      {
#सचे STRUCT_STATXFS_F_FSID_IS_INTEGER
        uintmax_t fsid = statfsbuf->f_fsid;
#अञ्ञथा
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
        अत्थाय (पूर्णाङ्क i = 0; i < शब्दाः && i * परिमाण *p < परिमाण fsid; i++)
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
#सचे HAVE_STRUCT_STATXFS_F_TYPE
      out_uint_x (pformat, prefix_len, statfsbuf->f_type);
#अञ्ञथा
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
        सचे (! frsize)
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
  पटिनिवत्त fail;
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
  सचे (!tried_mount_list) /* attempt/warn once per process.  */
    {
      सचे (!(mount_list = read_file_system_list (false)))
        error (0, errno, "%s", _("cannot read table of mounted file systems"));
      tried_mount_list = true;
    }

  संरचना stat name_stats;
  सचे (stat (name, &name_stats) != 0)
    पटिनिवत्त रिक्त;

  अत्थाय (संरचना mount_entry *me = mount_list; me; me = me->me_next)
    {
      सचे (me->me_dummy && me->me_devname[0] == '/'
          && streq (me->me_mountdir, name))
        {
          संरचना stat dev_stats;

          सचे (stat (me->me_devname, &dev_stats) == 0
              && psame_inode (&name_stats, &dev_stats))
            {
              bind_mount = me->me_devname;
              भङ्ग;
            }
        }
    }

  पटिनिवत्त bind_mount;
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
  सचे (follow_links || !S_ISLNK (statp->st_mode))
    {
      वर्ण *resolved = canonicalize_file_name (filename);
      सचे (!resolved)
        {
          error (0, errno, _("failed to canonicalize %s"), quoteaf (filename));
          गच्छ print_mount_point;
        }
      bp = find_bind_mount (resolved);
      मुक्त (resolved);
      सचे (bp)
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
  सचे ((mp = find_mount_point (filename, statp)))
    {
      /* This dir might be bind mounted to another device,
         so we resolve the bound source in that case also.  */
      bp = find_bind_mount (mp);
      fail = false;
    }

print_mount_point:

  out_string (pformat, prefix_len, bp ? bp : mp ? mp : np);
  मुक्त (mp);
  पटिनिवत्त fail;
}

/* Map a TS with negative TS.tv_nsec to {0,0}.  */
स्थिर अन्तःस्थ संरचना timespec
neg_to_zero (संरचना timespec ts)
{
  सचे (0 <= ts.tv_nsec)
    पटिनिवत्त ts;
  संरचना timespec z = {0};
  पटिनिवत्त z;
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
  सचे (*fmt_char == '.')
    fmt_char += 1 + strspn (fmt_char + 1, digits);
  पटिनिवत्त fmt_char - directive;
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
  अत्थाय (वर्ण ध्रुव *b = format; *b; b++)
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
                सचे (1 < len)
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
                सचे ((mod_char == 'Q' && fmt_char == 'n')
                    || (print_func == print_stat
                        && (mod_char == 'H' || mod_char == 'L')
                        && (fmt_char == 'd' || fmt_char == 'r')))
                  {
                    b++;
                  }
                अञ्ञथा
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
          सचे ( ! interpret_backslash_escapes)
            {
              putchar ('\\');
              भङ्ग;
            }
          ++b;
          सचे (isoct (*b))
            {
              पूर्णाङ्क esc_मानम् = fromoct (*b);
              पूर्णाङ्क esc_length = 1;	/* number of octal digits */
              अत्थाय (++b; esc_length < 3 && isoct (*b);
                   ++esc_length, ++b)
                {
                  esc_मानम् = esc_मानम् * 8 + fromoct (*b);
                }
              putchar (esc_मानम्);
              --b;
            }
          अञ्ञथा सचे (*b == 'x' && c_isxdigit (b[1]))
            {
              पूर्णाङ्क esc_मानम् = fromhex (b[1]);	/* Value of \xhh escape. */
              /* A hexadecimal \xhh escape sequence must have
                 1 or 2 hex. digits.  */
              ++b;
              सचे (c_isxdigit (b[1]))
                {
                  ++b;
                  esc_मानम् = esc_मानम् * 16 + fromhex (*b);
                }
              putchar (esc_मानम्);
            }
          अञ्ञथा सचे (*b == '\0')
            {
              error (0, 0, _("warning: backslash at end of format"));
              putchar ('\\');
              /* Arrange to exit the loop.  */
              --b;
            }
          अञ्ञथा
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

  पटिनिवत्त fail;
}

/* Stat the file system and print what we find.  */
NODISCARD
स्थिर bool
do_statfs (वर्ण ध्रुव *filename, वर्ण ध्रुव *format)
{
  STRUCT_STATVFS statfsbuf;

  सचे (streq (filename, "-"))
    {
      error (0, 0, _("using %s to denote standard input does not work"
                     " in file system mode"), quoteaf (filename));
      पटिनिवत्त false;
    }

  सचे (STATFS (filename, &statfsbuf) != 0)
    {
      error (0, errno, _("cannot read file system information for %s"),
             quoteaf (filename));
      पटिनिवत्त false;
    }

  bool fail = print_it (format, -1, filename, print_statfs, &statfsbuf);
  पटिनिवत्त ! fail;
}

संरचना print_args {
  संरचना stat *st;
  संरचना timespec btime;
};

/* Ask statx to avoid syncing? */
स्थिर bool dont_sync;

/* Ask statx to force sync? */
स्थिर bool force_sync;

#सचे USE_STATX
स्थिर unsigned पूर्णाङ्क
fmt_to_mask (वर्ण fmt)
{
  विकल्प (fmt)
    {
    पक्ष 'N':
      पटिनिवत्त STATX_MODE;
    पक्ष 'd':
    पक्ष 'D':
      पटिनिवत्त STATX_MODE;
    पक्ष 'i':
      पटिनिवत्त STATX_INO;
    पक्ष 'a':
    पक्ष 'A':
      पटिनिवत्त STATX_MODE;
    पक्ष 'f':
      पटिनिवत्त STATX_MODE|STATX_TYPE;
    पक्ष 'F':
      पटिनिवत्त STATX_TYPE;
    पक्ष 'h':
      पटिनिवत्त STATX_NLINK;
    पक्ष 'u':
    पक्ष 'U':
      पटिनिवत्त STATX_UID;
    पक्ष 'g':
    पक्ष 'G':
      पटिनिवत्त STATX_GID;
    पक्ष 'm':
      पटिनिवत्त STATX_MODE|STATX_INO;
    पक्ष 's':
      पटिनिवत्त STATX_SIZE;
    पक्ष 't':
    पक्ष 'T':
      पटिनिवत्त STATX_MODE;
    पक्ष 'b':
      पटिनिवत्त STATX_BLOCKS;
    पक्ष 'w':
    पक्ष 'W':
      पटिनिवत्त STATX_BTIME;
    पक्ष 'x':
    पक्ष 'X':
      पटिनिवत्त STATX_ATIME;
    पक्ष 'y':
    पक्ष 'Y':
      पटिनिवत्त STATX_MTIME;
    पक्ष 'z':
    पक्ष 'Z':
      पटिनिवत्त STATX_CTIME;
    }
  पटिनिवत्त 0;
}

ATTRIBUTE_PURE
स्थिर unsigned पूर्णाङ्क
format_to_mask (वर्ण ध्रुव *format)
{
  unsigned पूर्णाङ्क mask = 0;

  अत्थाय (वर्ण ध्रुव *b = format; *b; b++)
    {
      सचे (*b != '%')
        अनुवर्त;

      b += format_code_offset (b);
      सचे (*b == '\0')
        भङ्ग;
      mask |= fmt_to_mask (*b);
    }
  पटिनिवत्त mask;
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

  सचे (AT_FDCWD != fd)
    {
      pathname = "";
      flags = AT_EMPTY_PATH;
    }
  अञ्ञथा सचे (!follow_links)
    {
      flags = AT_SYMLINK_NOFOLLOW;
    }

  सचे (dont_sync)
    flags |= AT_STATX_DONT_SYNC;
  अञ्ञथा सचे (force_sync)
    flags |= AT_STATX_FORCE_SYNC;

  सचे (! force_sync)
    flags |= AT_NO_AUTOMOUNT;

  fd = statx (fd, pathname, flags, format_to_mask (format), &stx);
  सचे (fd < 0)
    {
      सचे (flags & AT_EMPTY_PATH)
        error (0, errno, _("cannot stat standard input"));
      अञ्ञथा
        error (0, errno, _("cannot statx %s"), quoteaf (filename));
      पटिनिवत्त false;
    }

  सचे (S_ISBLK (stx.stx_mode) || S_ISCHR (stx.stx_mode))
    format = format2;

  statx_to_stat (&stx, &st);
  सचे (stx.stx_mask & STATX_BTIME)
    pa.btime = statx_timestamp_to_timespec (stx.stx_btime);

  bool fail = print_it (format, fd, filename, print_stat, &pa);
  पटिनिवत्त ! fail;
}

#अञ्ञथा /* USE_STATX */

स्थिर संरचना timespec
get_birthtime (पूर्णाङ्क fd, वर्ण ध्रुव *filename, संरचना stat ध्रुव *st)
{
  संरचना timespec ts = get_stat_birthtime (st);

# सचे HAVE_GETATTRAT
  सचे (ts.tv_nsec < 0)
    {
      nvlist_t *response;
      सचे ((fd < 0
           ? getattrat (AT_FDCWD, XATTR_VIEW_READWRITE, filename, &response)
           : fgetattr (fd, XATTR_VIEW_READWRITE, &response))
          == 0)
        {
          uint64_t *val;
          uint_t n;
          सचे (nvlist_lookup_uint64_array (response, A_CRTIME, &val, &n) == 0
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

  पटिनिवत्त ts;
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

  सचे (0 <= fd)
    {
      सचे (fstat (fd, &statbuf) != 0)
        {
          error (0, errno, _("cannot stat standard input"));
          पटिनिवत्त false;
        }
    }
  /* We can't use the shorter
     (follow_links?stat:lstat) (filename, &statbug)
     since stat might be a function-like macro.  */
  अञ्ञथा सचे ((follow_links
            ? stat (filename, &statbuf)
            : lstat (filename, &statbuf)) != 0)
    {
      error (0, errno, _("cannot stat %s"), quoteaf (filename));
      पटिनिवत्त false;
    }

  सचे (S_ISBLK (statbuf.st_mode) || S_ISCHR (statbuf.st_mode))
    format = format2;

  bool fail = print_it (format, fd, filename, print_stat, &pa);
  पटिनिवत्त ! fail;
}
#endif /* USE_STATX */

/* POSIX requires 'ls' to print file sizes without a sign, even
   when negative.  Be consistent with that.  */

स्थिर uintmax_t
unsigned_file_size (off_t size)
{
  पटिनिवत्त size + (size < 0) * ((uintmax_t) OFF_T_MAX - OFF_T_MIN + 1);
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
      सचे (mod == 'Q')
        {
          initialize_quoting_style ();
          filename = quoteN (filename);
        }
      out_string (pformat, prefix_len, filename);
      भङ्ग;
    पक्ष 'N':
      initialize_quoting_style ();
      out_string (pformat, prefix_len, quoteN (filename));
      सचे (S_ISLNK (statbuf->st_mode))
        {
          वर्ण *linkname = areadlink_with_size (filename, statbuf->st_size);
          सचे (linkname == रिक्त)
            {
              error (0, errno, _("cannot read symbolic link %s"),
                     quoteaf (filename));
              पटिनिवत्त true;
            }
          printf (" -> ");
          out_string (pformat, prefix_len, quoteN (linkname));
          मुक्त (linkname);
        }
      भङ्ग;
    पक्ष 'd':
      सचे (mod == 'H')
        out_uint (pformat, prefix_len, major (statbuf->st_dev));
      अञ्ञथा सचे (mod == 'L')
        out_uint (pformat, prefix_len, minor (statbuf->st_dev));
      अञ्ञथा
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
      सचे (mod == 'H')
        out_uint (pformat, prefix_len, major (statbuf->st_rdev));
      अञ्ञथा सचे (mod == 'L')
        out_uint (pformat, prefix_len, minor (statbuf->st_rdev));
      अञ्ञथा
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
#सचे ! USE_STATX
        btime = get_birthtime (fd, filename, statbuf);
#endif
        सचे (btime.tv_nsec < 0)
          out_string (pformat, prefix_len, "-");
        अञ्ञथा
          out_string (pformat, prefix_len, human_time (btime));
      }
      भङ्ग;
    पक्ष 'W':
      {
#सचे ! USE_STATX
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
  पटिनिवत्त fail;
}

/* Return an allocated format string in static storage that
   corresponds to whether FS and TERSE options were declared.  */
स्थिर वर्ण *
default_format (bool fs, bool terse, bool device)
{
  वर्ण *format;
  सचे (fs)
    {
      सचे (terse)
        format = xstrdup (fmt_terse_fs);
      अञ्ञथा
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
  अञ्ञथा /* ! fs */
    {
      सचे (terse)
        {
          सचे (0 < is_selinux_enabled ())
            format = xstrdup (fmt_terse_selinux);
          अञ्ञथा
            format = xstrdup (fmt_terse_regular);
        }
      अञ्ञथा
        {
          वर्ण *temp;
          /* TRANSLATORS: This string uses format specifiers from
             'stat --help' without --file-system, and NOT from printf.  */
          format = xstrdup (_("\
  File: %N\n\
  Size: %-10s\tBlocks: %-10b IO Block: %-6o %F\n\
"));

          temp = format;
          सचे (device)
            {
              /* TRANSLATORS: This string uses format specifiers from
                 'stat --help' without --file-system, and NOT from printf.  */
              format = xasprintf ("%s%s", format, _("\
" "Device: %Hd,%Ld\tInode: %-10i  Links: %-5h Device type: %Hr,%Lr\n\
"));
            }
          अञ्ञथा
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

          सचे (0 < is_selinux_enabled ())
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
  पटिनिवत्त format;
}

शून्य
usage (पूर्णाङ्क status)
{
  सचे (status != EXIT_SUCCESS)
    emit_try_help ();
  अञ्ञथा
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
         and करोहि not output a mandatory trailing newline;\n\
         सचे you want a newline, include \\n in FORMAT\n\
"));
      oputs (_("\
  -t, --terse\n\
         print the information in terse form\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);

      fputs (_("\n\
The MODE argument of --cached can be: always, never, or स्वभाव.\n\
'always' will use cached attributes सचे available, याव\n\
'never' will try to synchronize with the latest attributes, and\n\
'default' will leave it up to the underlying file system.\n\
"), stdout);

      fputs (_("\n\
The valid format sequences अत्थाय files (without --file-system):\n\
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
  %N   quoted file name with dereference सचे symbolic link\n\
  %o   optimal I/O transfer size hint\n\
  %s   total size, in bytes\n\
  %r   device type in decimal (st_rdev)\n\
  %R   device type in hex (st_rdev)\n\
  %Hr  major device type in decimal, अत्थाय character/block device special files\n\
  %Lr  minor device type in decimal, अत्थाय character/block device special files\n\
  %t   major device type in hex, अत्थाय character/block device special files\n\
  %T   minor device type in hex, अत्थाय character/block device special files\n\
"), stdout);
      fputs (_("\
  %u   user ID of owner\n\
  %U   user name of owner\n\
  %w   time of file birth, human-readable; - सचे unknown\n\
  %W   time of file birth, seconds since Epoch; 0 सचे unknown\n\
  %x   time of last access, human-readable\n\
  %X   time of last access, seconds since Epoch\n\
  %y   time of last data modification, human-readable\n\
  %Y   time of last data modification, seconds since Epoch\n\
  %z   time of last status change, human-readable\n\
  %Z   time of last status change, seconds since Epoch\n\
\n\
"), stdout);

      fputs (_("\
Valid format sequences अत्थाय file systems:\n\
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
  %s   block size (अत्थाय faster transfers)\n\
  %S   fundamental block size (अत्थाय block counts)\n\
  %t   file system type in hex\n\
  %T   file system type in human readable form\n\
"), stdout);

      printf (_("\n\
--terse is equivalent to the following FORMAT:\n\
    %s\
"),
#सचे HAVE_SELINUX_SELINUX_H
              fmt_terse_selinux
#अञ्ञथा
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

  याव ((c = getopt_long (argc, argv, "c:fLt", long_options, रिक्त)) != -1)
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

  सचे (argc == optind)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  सचे (format)
    {
      format2 = format;
    }
  अञ्ञथा
    {
      format = default_format (fs, terse, /* device= */ false);
      format2 = default_format (fs, terse, /* device= */ true);
    }

  अत्थाय (पूर्णाङ्क i = optind; i < argc; i++)
    ok &= (fs
           ? do_statfs (argv[i], format)
           : do_stat (argv[i], format, format2));

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
