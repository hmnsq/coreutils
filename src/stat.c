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
#यदि ((STAT_STATVFS || STAT_STATVFS64)                                       \
     && (HAVE_STRUCT_STATVFS_F_BASETYPE || HAVE_STRUCT_STATVFS_F_FSTYPENAME \
         || (! HAVE_STRUCT_STATFS_F_FSTYPENAME && HAVE_STRUCT_STATVFS_F_TYPE)))
# define USE_STATVFS 1
#अन्यथा
# define USE_STATVFS 0
#endif

#अन्तर्भाव <stdio.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <pwd.h>
#अन्तर्भाव <grp.h>
#यदि USE_STATVFS
# include <sys/statvfs.h>
#अथवायदि HAVE_SYS_VFS_H
# include <sys/vfs.h>
#अथवायदि HAVE_SYS_MOUNT_H && HAVE_SYS_PARAM_H
/* NOTE: freebsd5.0 needs sys/param.h and sys/mount.h for statfs.
   It does have statvfs.h, but shouldn't use it, since it doesn't
   HAVE_STRUCT_STATVFS_F_BASETYPE.  So find a clean way to fix it.  */
/* NetBSD 1.5.2 needs these, for the declaration of struct statfs. */
# include <sys/param.h>
# include <sys/mount.h>
# यदि HAVE_NFS_NFS_CLNT_H && HAVE_NFS_VFS_H
/* Ultrix 4.4 needs these for the declaration of struct statfs.  */
#  include <netinet/in.h>
#  include <nfs/nfs_clnt.h>
#  include <nfs/vfs.h>
# endif
#अथवायदि HAVE_OS_H /* BeOS */
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

#यदि HAVE_STATX && defined STATX_INO
# define USE_STATX 1
#अन्यथा
# define USE_STATX 0
#endif

#यदि USE_STATVFS
# define STRUCT_STATXFS_F_FSID_IS_INTEGER STRUCT_STATVFS_F_FSID_IS_INTEGER
# define HAVE_STRUCT_STATXFS_F_TYPE HAVE_STRUCT_STATVFS_F_TYPE
# यदि HAVE_STRUCT_STATVFS_F_NAMEMAX
#  define SB_F_NAMEMAX(S) ((S)->f_namemax)
# endif
# यदि ! STAT_STATVFS && STAT_STATVFS64
#  define STRUCT_STATVFS संरचना statvfs64
#  define STATFS statvfs64
# अन्यथा
#  define STRUCT_STATVFS संरचना statvfs
#  define STATFS statvfs
# endif
# define STATFS_FRSIZE(S) ((S)->f_frsize)
#अन्यथा
# define HAVE_STRUCT_STATXFS_F_TYPE HAVE_STRUCT_STATFS_F_TYPE
# यदि HAVE_STRUCT_STATFS_F_NAMELEN
#  define SB_F_NAMEMAX(S) ((S)->f_namelen)
# elif HAVE_STRUCT_STATFS_F_NAMEMAX
#  define SB_F_NAMEMAX(S) ((S)->f_namemax)
# endif
# define STATFS statfs
# यदि HAVE_OS_H /* BeOS */
/* BeOS has a statvfs function, but it does not return sensible values
   for f_files, f_ffree and f_favail, and lacks f_type, f_basetype and
   f_fstypename.  Use 'struct fs_info' instead.  */
NODISCARD
स्थिर पूर्णाङ्क
statfs (वर्ण अचर *filename, संरचना fs_info *buf)
{
  dev_t device = dev_for_path (filename);
  यदि (device < 0)
    {
      त्रुटि_सं = (device == B_ENTRY_NOT_FOUND ? ENOENT
               : device == B_BAD_मान ? EINVAL
               : device == B_NAME_TOO_LONG ? ENAMETOOLONG
               : device == B_NO_MEMORY ? ENOMEM
               : device == B_FILE_ERROR ? EIO
               : 0);
      फर्काऊ -1;
    }
  /* If successful, buf->dev will be == device.  */
  फर्काऊ fs_stat_dev (device, buf);
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
# अन्यथा
#  define STRUCT_STATVFS संरचना statfs
#  define STRUCT_STATXFS_F_FSID_IS_INTEGER STRUCT_STATFS_F_FSID_IS_INTEGER
#  यदि HAVE_STRUCT_STATFS_F_FRSIZE
#   define STATFS_FRSIZE(S) ((S)->f_frsize)
#  अन्यथा
#   define STATFS_FRSIZE(S) 0
#  endif
# endif
#endif

#ifdef SB_F_NAMEMAX
# define OUT_NAMEMAX out_uint
#अन्यथा
/* Depending on whether statvfs or statfs is used,
   neither f_namemax or f_namelen may be available.  */
# define SB_F_NAMEMAX(S) "?"
# define OUT_NAMEMAX out_string
#endif

#यदि HAVE_STRUCT_STATVFS_F_BASETYPE
# define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME f_basetype
#अन्यथा
# यदि HAVE_STRUCT_STATVFS_F_FSTYPENAME || HAVE_STRUCT_STATFS_F_FSTYPENAME
#  define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME f_fstypename
# elif HAVE_OS_H /* BeOS */
#  define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME fsh_name
# endif
#endif

#यदि HAVE_GETATTRAT
# include <attr.h>
# include <sys/nvpair.h>
#endif

स्थिर वर्ण अचर digits[] = "0123456789";

/* Flags that are portable for use in printf, for at least one
   conversion specifier; make_format removes non-portable flags as
   needed for particular specifiers.  The glibc 2.2 extension "I" is
   listed here; it is removed by make_format because it has undefined
   behavior elsewhere and because it is incompatible with
   out_epoch_sec.  */
स्थिर वर्ण अचर printf_flags[] = "'-+ #0I";

/* Formats for the --terse option.  */
स्थिर वर्ण अचर fmt_terse_fs[] = "%Qn %i %l %t %s %S %b %f %a %c %d\n";
स्थिर वर्ण अचर fmt_terse_regular[] = "%Qn %s %b %f %u %g %D %i %h %t %T"
                                        " %X %Y %Z %W %o\n";
स्थिर वर्ण अचर fmt_terse_selinux[] = "%Qn %s %b %f %u %g %D %i %h %t %T"
                                        " %X %Y %Z %W %o %C\n";

#define PROGRAM_NAME "stat"

#define AUTHORS proper_name ("Michael Meskes")

गणना
{
  PRINTF_OPTION = अक्षर_उच्च + 1
};

गणना cached_mode
{
  cached_default,
  cached_never,
  cached_always
};

स्थिर वर्ण अचर *अचर cached_args[] =
{
  "default", "never", "always", रिक्त
};

स्थिर गणना cached_mode अचर cached_modes[] =
{
  cached_default, cached_never, cached_always
};

स्थिर संरचना option अचर long_options[] =
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
स्थिर वर्ण अचर *trailing_delim = "";

/* The representation of the decimal point in the current locale.  */
स्थिर वर्ण अचर *decimal_point;
स्थिर माप_प्रकार decimal_point_len;

स्थिर bool
print_stat (वर्ण *pformat, माप_प्रकार prefix_len, वर्ण mod, वर्ण m,
            पूर्णाङ्क fd, वर्ण अचर *filename, शून्य अचर *data);

/* Return the type of the specified file system.
   Some systems have statfvs.f_basetype[FSTYPSZ] (AIX, HP-UX, and Solaris).
   Others have statvfs.f_fstypename[_VFS_NAMELEN] (NetBSD 3.0).
   Others have statfs.f_fstypename[MFSNAMELEN] (NetBSD 1.5.2).
   Still others have neither and have to get by with f_type (GNU/Linux).
   But f_type may only exist in statfs (Cygwin).  */
NODISCARD
स्थिर वर्ण अचर *
human_fstype (STRUCT_STATVFS अचर *statfsbuf)
{
#ifdef STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME
  फर्काऊ statfsbuf->STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME;
#अन्यथा
  विकल्प (statfsbuf->f_type)
    {
# यदि defined __linux__ || defined __ANDROID__

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
      फर्काऊ "aafs";
    पक्ष S_MAGIC_ACFS: /* 0x61636673 remote */
      फर्काऊ "acfs";
    पक्ष S_MAGIC_ADFS: /* 0xADF5 local */
      फर्काऊ "adfs";
    पक्ष S_MAGIC_AFFS: /* 0xADFF local */
      फर्काऊ "affs";
    पक्ष S_MAGIC_AFS: /* 0x5346414F remote */
      फर्काऊ "afs";
    पक्ष S_MAGIC_ANON_INODE_FS: /* 0x09041934 local */
      फर्काऊ "anon-inode FS";
    पक्ष S_MAGIC_AUFS: /* 0x61756673 remote */
      /* FIXME: change syntax or add an optional attribute like "inotify:no".
         The above is labeled as "remote" so that tail always uses polling,
         but this isn't really a remote file system type.  */
      फर्काऊ "aufs";
    पक्ष S_MAGIC_AUTOFS: /* 0x0187 local */
      फर्काऊ "autofs";
    पक्ष S_MAGIC_BALLOON_KVM: /* 0x13661366 local */
      फर्काऊ "balloon-kvm-fs";
    पक्ष S_MAGIC_BCACHEFS: /* 0xCA451A4E local */
      फर्काऊ "bcachefs";
    पक्ष S_MAGIC_BEFS: /* 0x42465331 local */
      फर्काऊ "befs";
    पक्ष S_MAGIC_BDEVFS: /* 0x62646576 local */
      फर्काऊ "bdevfs";
    पक्ष S_MAGIC_BFS: /* 0x1BADFACE local */
      फर्काऊ "bfs";
    पक्ष S_MAGIC_BINDERFS: /* 0x6C6F6F70 local */
      फर्काऊ "binderfs";
    पक्ष S_MAGIC_BPF_FS: /* 0xCAFE4A11 local */
      फर्काऊ "bpf_fs";
    पक्ष S_MAGIC_BINFMTFS: /* 0x42494E4D local */
      फर्काऊ "binfmt_misc";
    पक्ष S_MAGIC_BTRFS: /* 0x9123683E local */
      फर्काऊ "btrfs";
    पक्ष S_MAGIC_BTRFS_TEST: /* 0x73727279 local */
      फर्काऊ "btrfs_test";
    पक्ष S_MAGIC_CEPH: /* 0x00C36400 remote */
      फर्काऊ "ceph";
    पक्ष S_MAGIC_CGROUP: /* 0x0027E0EB local */
      फर्काऊ "cgroupfs";
    पक्ष S_MAGIC_CGROUP2: /* 0x63677270 local */
      फर्काऊ "cgroup2fs";
    पक्ष S_MAGIC_CIFS: /* 0xFF534D42 remote */
      फर्काऊ "cifs";
    पक्ष S_MAGIC_CODA: /* 0x73757245 remote */
      फर्काऊ "coda";
    पक्ष S_MAGIC_COH: /* 0x012FF7B7 local */
      फर्काऊ "coh";
    पक्ष S_MAGIC_CONFIGFS: /* 0x62656570 local */
      फर्काऊ "configfs";
    पक्ष S_MAGIC_CRAMFS: /* 0x28CD3D45 local */
      फर्काऊ "cramfs";
    पक्ष S_MAGIC_CRAMFS_WEND: /* 0x453DCD28 local */
      फर्काऊ "cramfs-wend";
    पक्ष S_MAGIC_DAXFS: /* 0x64646178 local */
      फर्काऊ "daxfs";
    पक्ष S_MAGIC_DEBUGFS: /* 0x64626720 local */
      फर्काऊ "debugfs";
    पक्ष S_MAGIC_DEVFS: /* 0x1373 local */
      फर्काऊ "devfs";
    पक्ष S_MAGIC_DEVMEM: /* 0x454D444D local */
      फर्काऊ "devmem";
    पक्ष S_MAGIC_DEVPTS: /* 0x1CD1 local */
      फर्काऊ "devpts";
    पक्ष S_MAGIC_DMA_BUF: /* 0x444D4142 local */
      फर्काऊ "dma-buf-fs";
    पक्ष S_MAGIC_ECRYPTFS: /* 0xF15F local */
      फर्काऊ "ecryptfs";
    पक्ष S_MAGIC_EFIVARFS: /* 0xDE5E81E4 local */
      फर्काऊ "efivarfs";
    पक्ष S_MAGIC_EFS: /* 0x00414A53 local */
      फर्काऊ "efs";
    पक्ष S_MAGIC_EROFS_V1: /* 0xE0F5E1E2 local */
      फर्काऊ "erofs";
    पक्ष S_MAGIC_EXFAT: /* 0x2011BAB0 local */
      फर्काऊ "exfat";
    पक्ष S_MAGIC_EXFS: /* 0x45584653 local */
      फर्काऊ "exfs";
    पक्ष S_MAGIC_EXOFS: /* 0x5DF5 local */
      फर्काऊ "exofs";
    पक्ष S_MAGIC_EXT: /* 0x137D local */
      फर्काऊ "ext";
    पक्ष S_MAGIC_EXT2: /* 0xEF53 local */
      फर्काऊ "ext2/ext3";
    पक्ष S_MAGIC_EXT2_OLD: /* 0xEF51 local */
      फर्काऊ "ext2";
    पक्ष S_MAGIC_F2FS: /* 0xF2F52010 local */
      फर्काऊ "f2fs";
    पक्ष S_MAGIC_FAIL_FS: /* 0x4641494C local */
      फर्काऊ "failfs";
    पक्ष S_MAGIC_FAT: /* 0x4006 local */
      फर्काऊ "fat";
    पक्ष S_MAGIC_FHGFS: /* 0x19830326 remote */
      फर्काऊ "fhgfs";
    पक्ष S_MAGIC_FUSE: /* 0x65735546 remote */
      फर्काऊ "fuse";
    पक्ष S_MAGIC_FUSECTL: /* 0x65735543 remote */
      फर्काऊ "fusectl";
    पक्ष S_MAGIC_FUTEXFS: /* 0x0BAD1DEA local */
      फर्काऊ "futexfs";
    पक्ष S_MAGIC_GFS: /* 0x01161970 remote */
      फर्काऊ "gfs/gfs2";
    पक्ष S_MAGIC_GPFS: /* 0x47504653 remote */
      फर्काऊ "gpfs";
    पक्ष S_MAGIC_GUEST_MEMFD: /* 0x474D454D remote */
      फर्काऊ "guest-memfd";
    पक्ष S_MAGIC_HFS: /* 0x4244 local */
      फर्काऊ "hfs";
    पक्ष S_MAGIC_HFS_PLUS: /* 0x482B local */
      फर्काऊ "hfs+";
    पक्ष S_MAGIC_HFS_X: /* 0x4858 local */
      फर्काऊ "hfsx";
    पक्ष S_MAGIC_HOSTFS: /* 0x00C0FFEE local */
      फर्काऊ "hostfs";
    पक्ष S_MAGIC_HPFS: /* 0xF995E849 local */
      फर्काऊ "hpfs";
    पक्ष S_MAGIC_HUGETLBFS: /* 0x958458F6 local */
      फर्काऊ "hugetlbfs";
    पक्ष S_MAGIC_MTD_INODE_FS: /* 0x11307854 local */
      फर्काऊ "inodefs";
    पक्ष S_MAGIC_IBRIX: /* 0x013111A8 remote */
      फर्काऊ "ibrix";
    पक्ष S_MAGIC_INOTIFYFS: /* 0x2BAD1DEA local */
      फर्काऊ "inotifyfs";
    पक्ष S_MAGIC_ISOFS: /* 0x9660 local */
      फर्काऊ "isofs";
    पक्ष S_MAGIC_ISOFS_R_WIN: /* 0x4004 local */
      फर्काऊ "isofs";
    पक्ष S_MAGIC_ISOFS_WIN: /* 0x4000 local */
      फर्काऊ "isofs";
    पक्ष S_MAGIC_JFFS: /* 0x07C0 local */
      फर्काऊ "jffs";
    पक्ष S_MAGIC_JFFS2: /* 0x72B6 local */
      फर्काऊ "jffs2";
    पक्ष S_MAGIC_JFS: /* 0x3153464A local */
      फर्काऊ "jfs";
    पक्ष S_MAGIC_KAFS: /* 0x6B414653 remote */
      फर्काऊ "k-afs";
    पक्ष S_MAGIC_LOGFS: /* 0xC97E8168 local */
      फर्काऊ "logfs";
    पक्ष S_MAGIC_LUSTRE: /* 0x0BD00BD0 remote */
      फर्काऊ "lustre";
    पक्ष S_MAGIC_M1FS: /* 0x5346314D local */
      फर्काऊ "m1fs";
    पक्ष S_MAGIC_MINIX: /* 0x137F local */
      फर्काऊ "minix";
    पक्ष S_MAGIC_MINIX_30: /* 0x138F local */
      फर्काऊ "minix (30 char.)";
    पक्ष S_MAGIC_MINIX_V2: /* 0x2468 local */
      फर्काऊ "minix v2";
    पक्ष S_MAGIC_MINIX_V2_30: /* 0x2478 local */
      फर्काऊ "minix v2 (30 char.)";
    पक्ष S_MAGIC_MINIX_V3: /* 0x4D5A local */
      फर्काऊ "minix3";
    पक्ष S_MAGIC_MQUEUE: /* 0x19800202 local */
      फर्काऊ "mqueue";
    पक्ष S_MAGIC_MSDOS: /* 0x4D44 local */
      फर्काऊ "msdos";
    पक्ष S_MAGIC_NCP: /* 0x564C remote */
      फर्काऊ "novell";
    पक्ष S_MAGIC_NFS: /* 0x6969 remote */
      फर्काऊ "nfs";
    पक्ष S_MAGIC_NFSD: /* 0x6E667364 remote */
      फर्काऊ "nfsd";
    पक्ष S_MAGIC_NILFS: /* 0x3434 local */
      फर्काऊ "nilfs";
    पक्ष S_MAGIC_NSFS: /* 0x6E736673 local */
      फर्काऊ "nsfs";
    पक्ष S_MAGIC_NTFS: /* 0x5346544E local */
      फर्काऊ "ntfs";
    पक्ष S_MAGIC_NULL_FS: /* 0x4E554C4C local */
      फर्काऊ "nullfs";
    पक्ष S_MAGIC_OPENPROM: /* 0x9FA1 local */
      फर्काऊ "openprom";
    पक्ष S_MAGIC_OCFS2: /* 0x7461636F remote */
      फर्काऊ "ocfs2";
    पक्ष S_MAGIC_OVERLAYFS: /* 0x794C7630 remote */
      /* This may overlay remote file systems.
         Also there have been issues reported with inotify and overlayfs,
         so mark as "remote" so that polling is used.  */
      फर्काऊ "overlayfs";
    पक्ष S_MAGIC_PANFS: /* 0xAAD7AAEA remote */
      फर्काऊ "panfs";
    पक्ष S_MAGIC_PID_FS: /* 0x50494446 local */
      फर्काऊ "pidfs";
    पक्ष S_MAGIC_PIPEFS: /* 0x50495045 remote */
      /* FIXME: change syntax or add an optional attribute like "inotify:no".
         pipefs is labeled as "remote" so that tail always polls,
         but it isn't really remote file system type.  */
      फर्काऊ "pipefs";
    पक्ष S_MAGIC_PPC_CMM: /* 0xC7571590 local */
      फर्काऊ "ppc-cmm-fs";
    पक्ष S_MAGIC_PRL_FS: /* 0x7C7C6673 remote */
      फर्काऊ "prl_fs";
    पक्ष S_MAGIC_PROC: /* 0x9FA0 local */
      फर्काऊ "proc";
    पक्ष S_MAGIC_PSTOREFS: /* 0x6165676C local */
      फर्काऊ "pstorefs";
    पक्ष S_MAGIC_QNX4: /* 0x002F local */
      फर्काऊ "qnx4";
    पक्ष S_MAGIC_QNX6: /* 0x68191122 local */
      फर्काऊ "qnx6";
    पक्ष S_MAGIC_RAMFS: /* 0x858458F6 local */
      फर्काऊ "ramfs";
    पक्ष S_MAGIC_RDTGROUP: /* 0x07655821 local */
      फर्काऊ "rdt";
    पक्ष S_MAGIC_REISERFS: /* 0x52654973 local */
      फर्काऊ "reiserfs";
    पक्ष S_MAGIC_ROMFS: /* 0x7275 local */
      फर्काऊ "romfs";
    पक्ष S_MAGIC_RPC_PIPEFS: /* 0x67596969 local */
      फर्काऊ "rpc_pipefs";
    पक्ष S_MAGIC_SDCARDFS: /* 0x5DCA2DF5 local */
      फर्काऊ "sdcardfs";
    पक्ष S_MAGIC_SECRETMEM: /* 0x5345434D local */
      फर्काऊ "secretmem";
    पक्ष S_MAGIC_SECURITYFS: /* 0x73636673 local */
      फर्काऊ "securityfs";
    पक्ष S_MAGIC_SELINUX: /* 0xF97CFF8C local */
      फर्काऊ "selinux";
    पक्ष S_MAGIC_SMACK: /* 0x43415D53 local */
      फर्काऊ "smackfs";
    पक्ष S_MAGIC_SMB: /* 0x517B remote */
      फर्काऊ "smb";
    पक्ष S_MAGIC_SMB2: /* 0xFE534D42 remote */
      फर्काऊ "smb2";
    पक्ष S_MAGIC_SNFS: /* 0xBEEFDEAD remote */
      फर्काऊ "snfs";
    पक्ष S_MAGIC_SOCKFS: /* 0x534F434B local */
      फर्काऊ "sockfs";
    पक्ष S_MAGIC_SQUASHFS: /* 0x73717368 local */
      फर्काऊ "squashfs";
    पक्ष S_MAGIC_SYSFS: /* 0x62656572 local */
      फर्काऊ "sysfs";
    पक्ष S_MAGIC_SYSV2: /* 0x012FF7B6 local */
      फर्काऊ "sysv2";
    पक्ष S_MAGIC_SYSV4: /* 0x012FF7B5 local */
      फर्काऊ "sysv4";
    पक्ष S_MAGIC_TMPFS: /* 0x01021994 local */
      फर्काऊ "tmpfs";
    पक्ष S_MAGIC_TRACEFS: /* 0x74726163 local */
      फर्काऊ "tracefs";
    पक्ष S_MAGIC_UBIFS: /* 0x24051905 local */
      फर्काऊ "ubifs";
    पक्ष S_MAGIC_UDF: /* 0x15013346 local */
      फर्काऊ "udf";
    पक्ष S_MAGIC_UFS: /* 0x00011954 local */
      फर्काऊ "ufs";
    पक्ष S_MAGIC_UFS_BYTESWAPPED: /* 0x54190100 local */
      फर्काऊ "ufs";
    पक्ष S_MAGIC_USBDEVFS: /* 0x9FA2 local */
      फर्काऊ "usbdevfs";
    पक्ष S_MAGIC_V9FS: /* 0x01021997 local */
      फर्काऊ "v9fs";
    पक्ष S_MAGIC_VBOXSF: /* 0x786F4256 remote */
      फर्काऊ "vboxsf";
    पक्ष S_MAGIC_VMHGFS: /* 0xBACBACBC remote */
      फर्काऊ "vmhgfs";
    पक्ष S_MAGIC_VXFS: /* 0xA501FCF5 remote */
      /* Veritas File System can run in single instance or clustered mode,
         so mark as remote to cater for the latter case.  */
      फर्काऊ "vxfs";
    पक्ष S_MAGIC_VZFS: /* 0x565A4653 local */
      फर्काऊ "vzfs";
    पक्ष S_MAGIC_WSLFS: /* 0x53464846 local */
      फर्काऊ "wslfs";
    पक्ष S_MAGIC_XENFS: /* 0xABBA1974 local */
      फर्काऊ "xenfs";
    पक्ष S_MAGIC_XENIX: /* 0x012FF7B4 local */
      फर्काऊ "xenix";
    पक्ष S_MAGIC_XFS: /* 0x58465342 local */
      फर्काऊ "xfs";
    पक्ष S_MAGIC_XIAFS: /* 0x012FD16D local */
      फर्काऊ "xia";
    पक्ष S_MAGIC_Z3FOLD: /* 0x0033 local */
      फर्काऊ "z3fold";
    पक्ष S_MAGIC_ZFS: /* 0x2FC12FC1 local */
      फर्काऊ "zfs";
    पक्ष S_MAGIC_ZONEFS: /* 0x5A4F4653 local */
      फर्काऊ "zonefs";
    पक्ष S_MAGIC_ZSMALLOC: /* 0x58295829 local */
      फर्काऊ "zsmallocfs";


# elif __GNU__
    पक्ष FSTYPE_UFS:
      फर्काऊ "ufs";
    पक्ष FSTYPE_NFS:
      फर्काऊ "nfs";
    पक्ष FSTYPE_GFS:
      फर्काऊ "gfs";
    पक्ष FSTYPE_LFS:
      फर्काऊ "lfs";
    पक्ष FSTYPE_SYSV:
      फर्काऊ "sysv";
    पक्ष FSTYPE_FTP:
      फर्काऊ "ftp";
    पक्ष FSTYPE_TAR:
      फर्काऊ "tar";
    पक्ष FSTYPE_AR:
      फर्काऊ "ar";
    पक्ष FSTYPE_CPIO:
      फर्काऊ "cpio";
    पक्ष FSTYPE_MSLOSS:
      फर्काऊ "msloss";
    पक्ष FSTYPE_CPM:
      फर्काऊ "cpm";
    पक्ष FSTYPE_HFS:
      फर्काऊ "hfs";
    पक्ष FSTYPE_DTFS:
      फर्काऊ "dtfs";
    पक्ष FSTYPE_GRFS:
      फर्काऊ "grfs";
    पक्ष FSTYPE_TERM:
      फर्काऊ "term";
    पक्ष FSTYPE_DEV:
      फर्काऊ "dev";
    पक्ष FSTYPE_PROC:
      फर्काऊ "proc";
    पक्ष FSTYPE_IFSOCK:
      फर्काऊ "ifsock";
    पक्ष FSTYPE_AFS:
      फर्काऊ "afs";
    पक्ष FSTYPE_DFS:
      फर्काऊ "dfs";
    पक्ष FSTYPE_PROC9:
      फर्काऊ "proc9";
    पक्ष FSTYPE_SOCKET:
      फर्काऊ "socket";
    पक्ष FSTYPE_MISC:
      फर्काऊ "misc";
    पक्ष FSTYPE_EXT2FS:
      फर्काऊ "ext2/ext3";
    पक्ष FSTYPE_HTTP:
      फर्काऊ "http";
    पक्ष FSTYPE_MEMFS:
      फर्काऊ "memfs";
    पक्ष FSTYPE_ISO9660:
      फर्काऊ "iso9660";
# endif
    स्वभाव:
      {
        अचिन्हित दीर्घ पूर्णाङ्क type = statfsbuf->f_type;
        स्थिर वर्ण buf[परिमाण "UNKNOWN (0x%lx)" - 3
                        + (परिमाण type * अक्षर_बिट + 3) / 4];
        प्र_लिखो (buf, "UNKNOWN (0x%lx)", type);
        फर्काऊ buf;
      }
    }
#endif
}

NODISCARD
स्थिर वर्ण *
human_access (संरचना stat अचर *statbuf)
{
  स्थिर वर्ण modebuf[12];
  filemodestring (statbuf, modebuf);
  modebuf[10] = 0;
  फर्काऊ modebuf;
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
  यदि (!tz)
    tz = tzalloc (दो_पर्या ("TZ"));
  संरचना पंचांग पंचांग;
  पूर्णाङ्क ns = t.tv_nsec;
  यदि (localtime_rz (tz, &t.tv_sec, &पंचांग))
    nstrftime (str, परिमाण str, "%Y-%m-%d %H:%M:%S.%N %z", &पंचांग, tz, ns);
  अन्यथा
    {
      वर्ण secbuf[INT_BUFSIZE_BOUND (intmax_t)];
      प्र_लिखो (str, "%s.%09d", timetostr (t.tv_sec, secbuf), ns);
    }
  फर्काऊ str;
}

/* PFORMAT points to a '%' followed by a prefix of a format, all of
   size PREFIX_LEN.  The flags allowed for this format are
   ALLOWED_FLAGS; remove other printf flags from the prefix, then
   append SUFFIX.  */
स्थिर शून्य
make_format (वर्ण *pformat, माप_प्रकार prefix_len, वर्ण अचर *allowed_flags,
             वर्ण अचर *suffix)
{
  वर्ण *dst = pformat + 1;
  वर्ण अचर *src;
  वर्ण अचर *srclim = pformat + prefix_len;
  लागि (src = dst; src < srclim && म_अक्षर (printf_flags, *src); src++)
    यदि (म_अक्षर (allowed_flags, *src))
      *dst++ = *src;
  जबसम्म (src < srclim)
    *dst++ = *src++;
  म_नकल (dst, suffix);
}

स्थिर शून्य
out_string (वर्ण *pformat, माप_प्रकार prefix_len, वर्ण अचर *arg)
{
  make_format (pformat, prefix_len, "-", "s");
  म_लिखो (pformat, arg);
}
स्थिर पूर्णाङ्क
out_int (वर्ण *pformat, माप_प्रकार prefix_len, intmax_t arg)
{
  make_format (pformat, prefix_len, "'-+ 0", "jd");
  फर्काऊ म_लिखो (pformat, arg);
}
स्थिर पूर्णाङ्क
out_uint (वर्ण *pformat, माप_प्रकार prefix_len, uintmax_t arg)
{
  make_format (pformat, prefix_len, "'-0", "ju");
  फर्काऊ म_लिखो (pformat, arg);
}
स्थिर शून्य
out_uint_o (वर्ण *pformat, माप_प्रकार prefix_len, uintmax_t arg)
{
  make_format (pformat, prefix_len, "-#0", "jo");
  म_लिखो (pformat, arg);
}
स्थिर शून्य
out_uint_x (वर्ण *pformat, माप_प्रकार prefix_len, uintmax_t arg)
{
  make_format (pformat, prefix_len, "-#0", "jx");
  म_लिखो (pformat, arg);
}
स्थिर पूर्णाङ्क
out_minus_zero (वर्ण *pformat, माप_प्रकार prefix_len)
{
  make_format (pformat, prefix_len, "'-+ 0", ".0f");
  फर्काऊ म_लिखो (pformat, -0.25);
}

/* Output the number of seconds since the Epoch, using a format that
   acts like printf's %f format.  */
स्थिर शून्य
out_epoch_sec (वर्ण *pformat, माप_प्रकार prefix_len,
               संरचना timespec arg)
{
  वर्ण *dot = स_प्रथम (pformat, '.', prefix_len);
  माप_प्रकार sec_prefix_len = prefix_len;
  पूर्णाङ्क width = 0;
  पूर्णाङ्क precision = 0;
  bool frac_बायाँ_adjust = false;

  यदि (dot)
    {
      sec_prefix_len = dot - pformat;
      pformat[prefix_len] = '\0';

      यदि (c_isdigit (dot[1]))
        {
          दीर्घ पूर्णाङ्क lprec = म_से_दीर्घ (dot + 1, रिक्त, 10);
          precision = (lprec <= पूर्णांक_उच्च ? lprec : पूर्णांक_उच्च);
        }
      अन्यथा
        {
          precision = 9;
        }

      यदि (precision && c_isdigit (dot[-1]))
        {
          /* If a nontrivial width is given, subtract the width of the
             decimal point and PRECISION digits that will be output
             later.  */
          वर्ण *p = dot;
          *dot = '\0';

          गर
            --p;
          जबसम्म (c_isdigit (p[-1]));

          दीर्घ पूर्णाङ्क lwidth = म_से_दीर्घ (p, रिक्त, 10);
          width = (lwidth <= पूर्णांक_उच्च ? lwidth : पूर्णांक_उच्च);
          यदि (1 < width)
            {
              p += (*p == '0');
              sec_prefix_len = p - pformat;
              पूर्णाङ्क w_d = (decimal_point_len < width
                         ? width - decimal_point_len
                         : 0);
              यदि (1 < w_d)
                {
                  पूर्णाङ्क w = w_d - precision;
                  यदि (1 < w)
                    {
                      वर्ण *dst = pformat;
                      लागि (वर्ण अचर *src = dst; src < p; src++)
                        {
                          यदि (*src == '-')
                            frac_बायाँ_adjust = true;
                          अन्यथा
                            *dst++ = *src;
                        }
                      sec_prefix_len =
                        (dst - pformat
                         + (frac_बायाँ_adjust ? 0 : प्र_लिखो (dst, "%d", w)));
                    }
                }
            }
        }
    }

  पूर्णाङ्क भाजक = 1;
  लागि (पूर्णाङ्क i = precision; i < 9; i++)
    भाजक *= 10;
  पूर्णाङ्क frac_sec = arg.tv_nsec / भाजक;
  पूर्णाङ्क int_len;

  यदि (TYPE_SIGNED (समय_प्रकार))
    {
      bool minus_zero = false;
      यदि (arg.tv_sec < 0 && arg.tv_nsec != 0)
        {
          पूर्णाङ्क frac_sec_modulus = 1000000000 / भाजक;
          frac_sec = (frac_sec_modulus - frac_sec
                      - (arg.tv_nsec % भाजक != 0));
          arg.tv_sec += (frac_sec != 0);
          minus_zero = (arg.tv_sec == 0);
        }
      int_len = (minus_zero
                 ? out_minus_zero (pformat, sec_prefix_len)
                 : out_int (pformat, sec_prefix_len, arg.tv_sec));
    }
  अन्यथा
    int_len = out_uint (pformat, sec_prefix_len, arg.tv_sec);

  यदि (precision)
    {
      पूर्णाङ्क prec = (precision < 9 ? precision : 9);
      पूर्णाङ्क trailing_prec = precision - prec;
      पूर्णाङ्क ilen = (int_len < 0 ? 0 : int_len);
      पूर्णाङ्क trailing_width = (ilen < width && decimal_point_len < width - ilen
                            ? width - ilen - decimal_point_len - prec
                            : 0);
      म_लिखो ("%s%.*d%-*.*d", decimal_point, prec, frac_sec,
              trailing_width, trailing_prec, 0);
    }
}

/* Print the context information of FILENAME, and return true iff the
   context could not be obtained.  */
NODISCARD
स्थिर bool
out_file_context (वर्ण *pformat, माप_प्रकार prefix_len, वर्ण अचर *filename)
{
  वर्ण *scontext;
  bool fail = false;

  यदि ((follow_links
       ? getfilecon (filename, &scontext)
       : lgetfilecon (filename, &scontext)) < 0)
    {
      error (0, त्रुटि_सं, _("failed to get security context of %s"),
             quoteaf (filename));
      scontext = रिक्त;
      fail = true;
    }
  म_नकल (pformat + prefix_len, "s");
  म_लिखो (pformat, (scontext ? scontext : "?"));
  यदि (scontext)
    freecon (scontext);
  फर्काऊ fail;
}

/* Set the quoting style once it is needed.  */
स्थिर शून्य
initialize_quoting_style (शून्य)
{
  स्थिर bool initialized;
  यदि (!initialized)
    {
      initialized = true;
      पूर्णाङ्क qs = getenv_quoting_style ();
      set_quoting_style (रिक्त, 0 <= qs ? qs : shell_escape_quoting_style);
    }
}

/* Print statfs info.  Return zero upon success, nonzero upon failure.  */
NODISCARD
स्थिर bool
print_statfs (वर्ण *pformat, माप_प्रकार prefix_len, MAYBE_UNUSED वर्ण mod, वर्ण m,
              MAYBE_UNUSED पूर्णाङ्क fd, वर्ण अचर *filename,
              शून्य अचर *data)
{
  STRUCT_STATVFS अचर *statfsbuf = data;
  bool fail = false;

  विकल्प (m)
    {
    पक्ष 'n':
      यदि (mod == 'Q')
        {
          initialize_quoting_style ();
          filename = quoteN (filename);
        }
      out_string (pformat, prefix_len, filename);
      रोक;

    पक्ष 'i':
      {
#यदि STRUCT_STATXFS_F_FSID_IS_INTEGER
        uintmax_t fsid = statfsbuf->f_fsid;
#अन्यथा
        प्रकारनिर्देश अचिन्हित पूर्णाङ्क fsid_word;
        static_assert (alignof (STRUCT_STATVFS) % alignof (fsid_word) == 0);
        static_assert (दुरत्व (STRUCT_STATVFS, f_fsid) % alignof (fsid_word)
                       == 0);
        static_assert (परिमाण statfsbuf->f_fsid % alignof (fsid_word) == 0);
        fsid_word अचर *p = (fsid_word *) &statfsbuf->f_fsid;

        /* Assume a little-endian word order, as that is compatible
           with glibc's statvfs implementation.  */
        uintmax_t fsid = 0;
        पूर्णाङ्क शब्दहरू = परिमाण statfsbuf->f_fsid / परिमाण *p;
        लागि (पूर्णाङ्क i = 0; i < शब्दहरू && i * परिमाण *p < परिमाण fsid; i++)
          {
            uintmax_t u = p[शब्दहरू - 1 - i];
            fsid |= u << (i * अक्षर_बिट * परिमाण *p);
          }
#endif
        out_uint_x (pformat, prefix_len, fsid);
      }
      रोक;

    पक्ष 'l':
      OUT_NAMEMAX (pformat, prefix_len, SB_F_NAMEMAX (statfsbuf));
      रोक;
    पक्ष 't':
#यदि HAVE_STRUCT_STATXFS_F_TYPE
      out_uint_x (pformat, prefix_len, statfsbuf->f_type);
#अन्यथा
      ख_अक्षर_दो ('?', मानक_निकास);
#endif
      रोक;
    पक्ष 'T':
      out_string (pformat, prefix_len, human_fstype (statfsbuf));
      रोक;
    पक्ष 'b':
      out_int (pformat, prefix_len, statfsbuf->f_blocks);
      रोक;
    पक्ष 'f':
      out_int (pformat, prefix_len, statfsbuf->f_bfree);
      रोक;
    पक्ष 'a':
      out_int (pformat, prefix_len, statfsbuf->f_bavail);
      रोक;
    पक्ष 's':
      out_uint (pformat, prefix_len, statfsbuf->f_bsize);
      रोक;
    पक्ष 'S':
      {
        uintmax_t frsize = STATFS_FRSIZE (statfsbuf);
        यदि (! frsize)
          frsize = statfsbuf->f_bsize;
        out_uint (pformat, prefix_len, frsize);
      }
      रोक;
    पक्ष 'c':
      out_uint (pformat, prefix_len, statfsbuf->f_files);
      रोक;
    पक्ष 'd':
      out_int (pformat, prefix_len, statfsbuf->f_ffree);
      रोक;
    स्वभाव:
      ख_अक्षर_दो ('?', मानक_निकास);
      रोक;
    }
  फर्काऊ fail;
}

/* Return any bind mounted source for a path.
   The caller should not free the returned buffer.
   Return NULL if no bind mount found.  */
NODISCARD
स्थिर वर्ण अचर *
find_bind_mount (वर्ण अचर * name)
{
  वर्ण अचर * bind_mount = रिक्त;

  स्थिर संरचना mount_entry *mount_list;
  स्थिर bool tried_mount_list = false;
  यदि (!tried_mount_list) /* attempt/warn once per process.  */
    {
      यदि (!(mount_list = read_file_system_list (false)))
        error (0, त्रुटि_सं, "%s", _("cannot read table of mounted file systems"));
      tried_mount_list = true;
    }

  संरचना stat name_stats;
  यदि (stat (name, &name_stats) != 0)
    फर्काऊ रिक्त;

  लागि (संरचना mount_entry *me = mount_list; me; me = me->me_next)
    {
      यदि (me->me_dummy && me->me_devname[0] == '/'
          && streq (me->me_mountdir, name))
        {
          संरचना stat dev_stats;

          यदि (stat (me->me_devname, &dev_stats) == 0
              && psame_inode (&name_stats, &dev_stats))
            {
              bind_mount = me->me_devname;
              रोक;
            }
        }
    }

  फर्काऊ bind_mount;
}

/* Print mount point.  Return zero upon success, nonzero upon failure.  */
NODISCARD
स्थिर bool
out_mount_point (वर्ण अचर *filename, वर्ण *pformat, माप_प्रकार prefix_len,
                 अचर संरचना stat *statp)
{

  वर्ण अचर *np = "?", *bp = रिक्त;
  वर्ण *mp = रिक्त;
  bool fail = true;

  /* Look for bind mounts first.  Note we output the immediate alias,
     rather than further resolving to a base device mount point.  */
  यदि (follow_links || !S_ISLNK (statp->st_mode))
    {
      वर्ण *resolved = canonicalize_file_name (filename);
      यदि (!resolved)
        {
          error (0, त्रुटि_सं, _("failed to canonicalize %s"), quoteaf (filename));
          गच्छ print_mount_point;
        }
      bp = find_bind_mount (resolved);
      मुक्त (resolved);
      यदि (bp)
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
  यदि ((mp = find_mount_point (filename, statp)))
    {
      /* This dir might be bind mounted to another device,
         so we resolve the bound source in that case also.  */
      bp = find_bind_mount (mp);
      fail = false;
    }

print_mount_point:

  out_string (pformat, prefix_len, bp ? bp : mp ? mp : np);
  मुक्त (mp);
  फर्काऊ fail;
}

/* Map a TS with negative TS.tv_nsec to {0,0}.  */
स्थिर अन्तःस्थ संरचना timespec
neg_to_zero (संरचना timespec ts)
{
  यदि (0 <= ts.tv_nsec)
    फर्काऊ ts;
  संरचना timespec z = {0};
  फर्काऊ z;
}

/* Output a single-character \ escape.  */

स्थिर शून्य
print_esc_char (वर्ण c)
{
  विकल्प (c)
    {
    पक्ष 'a':			/* Alert. */
      c ='\a';
      रोक;
    पक्ष 'b':			/* Backspace. */
      c ='\b';
      रोक;
    पक्ष 'e':			/* Escape. */
      c ='\x1B';
      रोक;
    पक्ष 'f':			/* Form feed. */
      c ='\f';
      रोक;
    पक्ष 'n':			/* New line. */
      c ='\n';
      रोक;
    पक्ष 'r':			/* Carriage return. */
      c ='\r';
      रोक;
    पक्ष 't':			/* Horizontal tab. */
      c ='\t';
      रोक;
    पक्ष 'v':			/* Vertical tab. */
      c ='\v';
      रोक;
    पक्ष '"':
    पक्ष '\\':
      रोक;
    स्वभाव:
      error (0, 0, _("warning: unrecognized escape '\\%c'"), c);
      रोक;
    }
  अक्षर_दो (c);
}

ATTRIBUTE_PURE
स्थिर माप_प्रकार
format_code_offset (वर्ण अचर *directive)
{
  माप_प्रकार len = म_अखोज (directive + 1, printf_flags);
  वर्ण अचर *fmt_char = directive + len + 1;
  fmt_char += म_अखोज (fmt_char, digits);
  यदि (*fmt_char == '.')
    fmt_char += 1 + म_अखोज (fmt_char + 1, digits);
  फर्काऊ fmt_char - directive;
}

/* Print the information specified by the format string, FORMAT,
   calling PRINT_FUNC for each %-directive encountered.
   Return zero upon success, nonzero upon failure.  */
NODISCARD
स्थिर bool
print_it (वर्ण अचर *format, पूर्णाङ्क fd, वर्ण अचर *filename,
          bool (*print_func) (वर्ण *, माप_प्रकार, वर्ण, वर्ण,
                              पूर्णाङ्क, वर्ण अचर *, शून्य अचर *),
          शून्य अचर *data)
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
  माप_प्रकार n_alloc = म_माप (format) + MAX_ADDITIONAL_BYTES + 1;
  वर्ण *dest = xmalloc (n_alloc);
  लागि (वर्ण अचर *b = format; *b; b++)
    {
      विकल्प (*b)
        {
        पक्ष '%':
          {
            माप_प्रकार len = format_code_offset (b);
            वर्ण fmt_char = *(b + len);
            वर्ण mod_char = 0;
            स_नकल (dest, b, len);
            b += len;

            विकल्प (fmt_char)
              {
              पक्ष '\0':
                --b;
                FALLTHROUGH;
              पक्ष '%':
                यदि (1 < len)
                  {
                    dest[len] = fmt_char;
                    dest[len + 1] = '\0';
                    error (निकास_त्रुटि, 0, _("%s: invalid directive"),
                           quote (dest));
                  }
                अक्षर_दो ('%');
                रोक;
              पक्ष 'H':
              पक्ष 'L':
              पक्ष 'Q':
                mod_char = fmt_char;
                fmt_char = *(b + 1);
                यदि ((mod_char == 'Q' && fmt_char == 'n')
                    || (print_func == print_stat
                        && (mod_char == 'H' || mod_char == 'L')
                        && (fmt_char == 'd' || fmt_char == 'r')))
                  {
                    b++;
                  }
                अन्यथा
                  {
                    fmt_char = mod_char;
                    mod_char = 0;
                  }
                FALLTHROUGH;
              स्वभाव:
                fail |= print_func (dest, len, mod_char, fmt_char,
                                    fd, filename, data);
                रोक;
              }
            रोक;
          }

        पक्ष '\\':
          यदि ( ! interpret_backslash_escapes)
            {
              अक्षर_दो ('\\');
              रोक;
            }
          ++b;
          यदि (isoct (*b))
            {
              पूर्णाङ्क esc_मान = fromoct (*b);
              पूर्णाङ्क esc_length = 1;	/* number of octal digits */
              लागि (++b; esc_length < 3 && isoct (*b);
                   ++esc_length, ++b)
                {
                  esc_मान = esc_मान * 8 + fromoct (*b);
                }
              अक्षर_दो (esc_मान);
              --b;
            }
          अन्यथा यदि (*b == 'x' && c_isxdigit (b[1]))
            {
              पूर्णाङ्क esc_मान = fromhex (b[1]);	/* Value of \xhh escape. */
              /* A hexadecimal \xhh escape sequence must have
                 1 or 2 hex. digits.  */
              ++b;
              यदि (c_isxdigit (b[1]))
                {
                  ++b;
                  esc_मान = esc_मान * 16 + fromhex (*b);
                }
              अक्षर_दो (esc_मान);
            }
          अन्यथा यदि (*b == '\0')
            {
              error (0, 0, _("warning: backslash at end of format"));
              अक्षर_दो ('\\');
              /* Arrange to exit the loop.  */
              --b;
            }
          अन्यथा
            {
              print_esc_char (*b);
            }
          रोक;

        स्वभाव:
          अक्षर_दो (*b);
          रोक;
        }
    }
  मुक्त (dest);

  ख_माला_दो (trailing_delim, मानक_निकास);

  फर्काऊ fail;
}

/* Stat the file system and print what we find.  */
NODISCARD
स्थिर bool
do_statfs (वर्ण अचर *filename, वर्ण अचर *format)
{
  STRUCT_STATVFS statfsbuf;

  यदि (streq (filename, "-"))
    {
      error (0, 0, _("using %s to denote standard input does not work"
                     " in file system mode"), quoteaf (filename));
      फर्काऊ false;
    }

  यदि (STATFS (filename, &statfsbuf) != 0)
    {
      error (0, त्रुटि_सं, _("cannot read file system information for %s"),
             quoteaf (filename));
      फर्काऊ false;
    }

  bool fail = print_it (format, -1, filename, print_statfs, &statfsbuf);
  फर्काऊ ! fail;
}

संरचना print_args {
  संरचना stat *st;
  संरचना timespec btime;
};

/* Ask statx to avoid syncing? */
स्थिर bool dont_sync;

/* Ask statx to force sync? */
स्थिर bool force_sync;

#यदि USE_STATX
स्थिर अचिन्हित पूर्णाङ्क
fmt_to_mask (वर्ण fmt)
{
  विकल्प (fmt)
    {
    पक्ष 'N':
      फर्काऊ STATX_MODE;
    पक्ष 'd':
    पक्ष 'D':
      फर्काऊ STATX_MODE;
    पक्ष 'i':
      फर्काऊ STATX_INO;
    पक्ष 'a':
    पक्ष 'A':
      फर्काऊ STATX_MODE;
    पक्ष 'f':
      फर्काऊ STATX_MODE|STATX_TYPE;
    पक्ष 'F':
      फर्काऊ STATX_TYPE;
    पक्ष 'h':
      फर्काऊ STATX_NLINK;
    पक्ष 'u':
    पक्ष 'U':
      फर्काऊ STATX_UID;
    पक्ष 'g':
    पक्ष 'G':
      फर्काऊ STATX_GID;
    पक्ष 'm':
      फर्काऊ STATX_MODE|STATX_INO;
    पक्ष 's':
      फर्काऊ STATX_SIZE;
    पक्ष 't':
    पक्ष 'T':
      फर्काऊ STATX_MODE;
    पक्ष 'b':
      फर्काऊ STATX_BLOCKS;
    पक्ष 'w':
    पक्ष 'W':
      फर्काऊ STATX_BTIME;
    पक्ष 'x':
    पक्ष 'X':
      फर्काऊ STATX_ATIME;
    पक्ष 'y':
    पक्ष 'Y':
      फर्काऊ STATX_MTIME;
    पक्ष 'z':
    पक्ष 'Z':
      फर्काऊ STATX_CTIME;
    }
  फर्काऊ 0;
}

ATTRIBUTE_PURE
स्थिर अचिन्हित पूर्णाङ्क
format_to_mask (वर्ण अचर *format)
{
  अचिन्हित पूर्णाङ्क mask = 0;

  लागि (वर्ण अचर *b = format; *b; b++)
    {
      यदि (*b != '%')
        जारी;

      b += format_code_offset (b);
      यदि (*b == '\0')
        रोक;
      mask |= fmt_to_mask (*b);
    }
  फर्काऊ mask;
}

/* statx the file and print what we find */
NODISCARD
स्थिर bool
do_stat (वर्ण अचर *filename, वर्ण अचर *format, वर्ण अचर *format2)
{
  पूर्णाङ्क fd = streq (filename, "-") ? 0 : AT_FDCWD;
  पूर्णाङ्क flags = 0;
  संरचना stat st;
  संरचना statx stx = {0};
  वर्ण अचर *pathname = filename;
  संरचना print_args pa;
  pa.st = &st;
  pa.btime = (संरचना timespec) {.tv_sec = -1, .tv_nsec = -1};

  यदि (AT_FDCWD != fd)
    {
      pathname = "";
      flags = AT_EMPTY_PATH;
    }
  अन्यथा यदि (!follow_links)
    {
      flags = AT_SYMLINK_NOFOLLOW;
    }

  यदि (dont_sync)
    flags |= AT_STATX_DONT_SYNC;
  अन्यथा यदि (force_sync)
    flags |= AT_STATX_FORCE_SYNC;

  यदि (! force_sync)
    flags |= AT_NO_AUTOMOUNT;

  fd = statx (fd, pathname, flags, format_to_mask (format), &stx);
  यदि (fd < 0)
    {
      यदि (flags & AT_EMPTY_PATH)
        error (0, त्रुटि_सं, _("cannot stat standard input"));
      अन्यथा
        error (0, त्रुटि_सं, _("cannot statx %s"), quoteaf (filename));
      फर्काऊ false;
    }

  यदि (S_ISBLK (stx.stx_mode) || S_ISCHR (stx.stx_mode))
    format = format2;

  statx_to_stat (&stx, &st);
  यदि (stx.stx_mask & STATX_BTIME)
    pa.btime = statx_timestamp_to_timespec (stx.stx_btime);

  bool fail = print_it (format, fd, filename, print_stat, &pa);
  फर्काऊ ! fail;
}

#अन्यथा /* USE_STATX */

स्थिर संरचना timespec
get_birthtime (पूर्णाङ्क fd, वर्ण अचर *filename, संरचना stat अचर *st)
{
  संरचना timespec ts = get_stat_birthtime (st);

# यदि HAVE_GETATTRAT
  यदि (ts.tv_nsec < 0)
    {
      nvlist_t *response;
      यदि ((fd < 0
           ? getattrat (AT_FDCWD, XATTR_VIEW_READWRITE, filename, &response)
           : fgetattr (fd, XATTR_VIEW_READWRITE, &response))
          == 0)
        {
          uint64_t *val;
          uint_t n;
          यदि (nvlist_lookup_uint64_array (response, A_CRTIME, &val, &n) == 0
              && 2 <= n
              && val[0] <= TYPE_MAXIMUM (समय_प्रकार)
              && val[1] < 1000000000 * 2 /* for leap seconds */)
            {
              ts.tv_sec = val[0];
              ts.tv_nsec = val[1];
            }
          nvlist_free (response);
        }
    }
# endif

  फर्काऊ ts;
}


/* stat the file and print what we find */
NODISCARD
स्थिर bool
do_stat (वर्ण अचर *filename, वर्ण अचर *format,
         वर्ण अचर *format2)
{
  पूर्णाङ्क fd = streq (filename, "-") ? 0 : -1;
  संरचना stat statbuf;
  संरचना print_args pa;
  pa.st = &statbuf;
  pa.btime = (संरचना timespec) {.tv_sec = -1, .tv_nsec = -1};

  यदि (0 <= fd)
    {
      यदि (ख_स्थिति (fd, &statbuf) != 0)
        {
          error (0, त्रुटि_सं, _("cannot stat standard input"));
          फर्काऊ false;
        }
    }
  /* We can't use the shorter
     (follow_links?stat:lstat) (filename, &statbug)
     since stat might be a function-like macro.  */
  अन्यथा यदि ((follow_links
            ? stat (filename, &statbuf)
            : lstat (filename, &statbuf)) != 0)
    {
      error (0, त्रुटि_सं, _("cannot stat %s"), quoteaf (filename));
      फर्काऊ false;
    }

  यदि (S_ISBLK (statbuf.st_mode) || S_ISCHR (statbuf.st_mode))
    format = format2;

  bool fail = print_it (format, fd, filename, print_stat, &pa);
  फर्काऊ ! fail;
}
#endif /* USE_STATX */

/* POSIX requires 'ls' to print file sizes without a sign, even
   when negative.  Be consistent with that.  */

स्थिर uintmax_t
unsigned_file_size (off_t size)
{
  फर्काऊ size + (size < 0) * ((uintmax_t) OFF_T_MAX - OFF_T_MIN + 1);
}

/* Print stat info.  Return zero upon success, nonzero upon failure.  */
स्थिर bool
print_stat (वर्ण *pformat, माप_प्रकार prefix_len, वर्ण mod, वर्ण m,
            MAYBE_UNUSED पूर्णाङ्क fd, वर्ण अचर *filename, शून्य अचर *data)
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
      यदि (mod == 'Q')
        {
          initialize_quoting_style ();
          filename = quoteN (filename);
        }
      out_string (pformat, prefix_len, filename);
      रोक;
    पक्ष 'N':
      initialize_quoting_style ();
      out_string (pformat, prefix_len, quoteN (filename));
      यदि (S_ISLNK (statbuf->st_mode))
        {
          वर्ण *linkname = areadlink_with_size (filename, statbuf->st_size);
          यदि (linkname == रिक्त)
            {
              error (0, त्रुटि_सं, _("cannot read symbolic link %s"),
                     quoteaf (filename));
              फर्काऊ true;
            }
          म_लिखो (" -> ");
          out_string (pformat, prefix_len, quoteN (linkname));
          मुक्त (linkname);
        }
      रोक;
    पक्ष 'd':
      यदि (mod == 'H')
        out_uint (pformat, prefix_len, major (statbuf->st_dev));
      अन्यथा यदि (mod == 'L')
        out_uint (pformat, prefix_len, minor (statbuf->st_dev));
      अन्यथा
        out_uint (pformat, prefix_len, statbuf->st_dev);
      रोक;
    पक्ष 'D':
      out_uint_x (pformat, prefix_len, statbuf->st_dev);
      रोक;
    पक्ष 'i':
      out_uint (pformat, prefix_len, statbuf->st_ino);
      रोक;
    पक्ष 'a':
      out_uint_o (pformat, prefix_len, statbuf->st_mode & CHMOD_MODE_BITS);
      रोक;
    पक्ष 'A':
      out_string (pformat, prefix_len, human_access (statbuf));
      रोक;
    पक्ष 'f':
      out_uint_x (pformat, prefix_len, statbuf->st_mode);
      रोक;
    पक्ष 'F':
      out_string (pformat, prefix_len, file_type (statbuf));
      रोक;
    पक्ष 'h':
      out_uint (pformat, prefix_len, statbuf->st_nlink);
      रोक;
    पक्ष 'u':
      out_uint (pformat, prefix_len, statbuf->st_uid);
      रोक;
    पक्ष 'U':
      pw_ent = getpwuid (statbuf->st_uid);
      out_string (pformat, prefix_len,
                  pw_ent ? pw_ent->pw_name : "UNKNOWN");
      रोक;
    पक्ष 'g':
      out_uint (pformat, prefix_len, statbuf->st_gid);
      रोक;
    पक्ष 'G':
      gw_ent = getgrgid (statbuf->st_gid);
      out_string (pformat, prefix_len,
                  gw_ent ? gw_ent->gr_name : "UNKNOWN");
      रोक;
    पक्ष 'm':
      fail |= out_mount_point (filename, pformat, prefix_len, statbuf);
      रोक;
    पक्ष 's':
      out_uint (pformat, prefix_len, unsigned_file_size (statbuf->st_size));
      रोक;
    पक्ष 'r':
      यदि (mod == 'H')
        out_uint (pformat, prefix_len, major (statbuf->st_rdev));
      अन्यथा यदि (mod == 'L')
        out_uint (pformat, prefix_len, minor (statbuf->st_rdev));
      अन्यथा
        out_uint (pformat, prefix_len, statbuf->st_rdev);
      रोक;
    पक्ष 'R':
      out_uint_x (pformat, prefix_len, statbuf->st_rdev);
      रोक;
    पक्ष 't':
      out_uint_x (pformat, prefix_len, major (statbuf->st_rdev));
      रोक;
    पक्ष 'T':
      out_uint_x (pformat, prefix_len, minor (statbuf->st_rdev));
      रोक;
    पक्ष 'B':
      out_uint (pformat, prefix_len, ST_NBLOCKSIZE);
      रोक;
    पक्ष 'b':
      out_uint (pformat, prefix_len, STP_NBLOCKS (statbuf));
      रोक;
    पक्ष 'o':
      out_uint (pformat, prefix_len, STP_BLKSIZE (statbuf));
      रोक;
    पक्ष 'w':
      {
#यदि ! USE_STATX
        btime = get_birthtime (fd, filename, statbuf);
#endif
        यदि (btime.tv_nsec < 0)
          out_string (pformat, prefix_len, "-");
        अन्यथा
          out_string (pformat, prefix_len, human_time (btime));
      }
      रोक;
    पक्ष 'W':
      {
#यदि ! USE_STATX
        btime = get_birthtime (fd, filename, statbuf);
#endif
        out_epoch_sec (pformat, prefix_len, neg_to_zero (btime));
      }
      रोक;
    पक्ष 'x':
      out_string (pformat, prefix_len, human_time (get_stat_atime (statbuf)));
      रोक;
    पक्ष 'X':
      out_epoch_sec (pformat, prefix_len, get_stat_atime (statbuf));
      रोक;
    पक्ष 'y':
      out_string (pformat, prefix_len, human_time (get_stat_mtime (statbuf)));
      रोक;
    पक्ष 'Y':
      out_epoch_sec (pformat, prefix_len, get_stat_mtime (statbuf));
      रोक;
    पक्ष 'z':
      out_string (pformat, prefix_len, human_time (get_stat_ctime (statbuf)));
      रोक;
    पक्ष 'Z':
      out_epoch_sec (pformat, prefix_len, get_stat_ctime (statbuf));
      रोक;
    पक्ष 'C':
      fail |= out_file_context (pformat, prefix_len, filename);
      रोक;
    स्वभाव:
      ख_अक्षर_दो ('?', मानक_निकास);
      रोक;
    }
  फर्काऊ fail;
}

/* Return an allocated format string in static storage that
   corresponds to whether FS and TERSE options were declared.  */
स्थिर वर्ण *
default_format (bool fs, bool terse, bool device)
{
  वर्ण *format;
  यदि (fs)
    {
      यदि (terse)
        format = xstrdup (fmt_terse_fs);
      अन्यथा
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
  अन्यथा /* ! fs */
    {
      यदि (terse)
        {
          यदि (0 < is_selinux_enabled ())
            format = xstrdup (fmt_terse_selinux);
          अन्यथा
            format = xstrdup (fmt_terse_regular);
        }
      अन्यथा
        {
          वर्ण *temp;
          /* TRANSLATORS: This string uses format specifiers from
             'stat --help' without --file-system, and NOT from printf.  */
          format = xstrdup (_("\
  File: %N\n\
  Size: %-10s\tBlocks: %-10b IO Block: %-6o %F\n\
"));

          temp = format;
          यदि (device)
            {
              /* TRANSLATORS: This string uses format specifiers from
                 'stat --help' without --file-system, and NOT from printf.  */
              format = xasprintf ("%s%s", format, _("\
" "Device: %Hd,%Ld\tInode: %-10i  Links: %-5h Device type: %Hr,%Lr\n\
"));
            }
          अन्यथा
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

          यदि (0 < is_selinux_enabled ())
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
  फर्काऊ format;
}

शून्य
usage (पूर्णाङ्क status)
{
  यदि (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("Usage: %s [OPTION]... FILE...\n"), program_name);
      ख_माला_दो (_("\
Display file or file प्रणाली status.\n\
"), मानक_निकास);

      emit_mandatory_arg_note ();

      oputs (_("\
  -L, --dereference\n\
         follow links\n\
"));
      oputs (_("\
  -f, --file-प्रणाली\n\
         display file प्रणाली status instead of file status\n\
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
      --म_लिखो=FORMAT\n\
         like --format, but interpret backslash escapes,\n\
         and गर not output a mandatory trailing newline;\n\
         यदि you want a newline, include \\n in FORMAT\n\
"));
      oputs (_("\
  -t, --terse\n\
         print the information in terse form\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);

      ख_माला_दो (_("\n\
The MODE argument of --cached can be: always, never, or स्वभाव.\n\
'always' will use cached attributes यदि available, जबसम्म\n\
'never' will try to synchronize with the latest attributes, and\n\
'default' will leave it up to the underlying file प्रणाली.\n\
"), मानक_निकास);

      ख_माला_दो (_("\n\
The valid format sequences लागि files (without --file-प्रणाली):\n\
\n\
  %a   permission bits in octal (see '#' and '0' म_लिखो flags)\n\
  %A   permission bits and file type in human readable form\n\
  %b   सङ्ख्या of blocks allocated (see %B)\n\
  %B   the size in bytes of each block reported by %b\n\
  %C   SELinux security context string\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  %d   device सङ्ख्या in decimal (st_dev)\n\
  %D   device सङ्ख्या in hex (st_dev)\n\
  %Hd  major device सङ्ख्या in decimal\n\
  %Ld  minor device सङ्ख्या in decimal\n\
  %f   raw mode in hex\n\
  %F   file type\n\
  %g   group ID of owner\n\
  %G   group name of owner\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  %h   सङ्ख्या of hard links\n\
  %i   inode सङ्ख्या\n\
  %m   mount point\n\
  %n   file name\n\
  %Qn  quoted file name\n\
  %N   quoted file name with dereference यदि symbolic link\n\
  %o   optimal I/O transfer size hint\n\
  %s   total size, in bytes\n\
  %r   device type in decimal (st_rdev)\n\
  %R   device type in hex (st_rdev)\n\
  %Hr  major device type in decimal, लागि character/block device special files\n\
  %Lr  minor device type in decimal, लागि character/block device special files\n\
  %t   major device type in hex, लागि character/block device special files\n\
  %T   minor device type in hex, लागि character/block device special files\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  %u   user ID of owner\n\
  %U   user name of owner\n\
  %w   समय of file birth, human-readable; - यदि unknown\n\
  %W   समय of file birth, seconds since Epoch; 0 यदि unknown\n\
  %x   समय of last access, human-readable\n\
  %X   समय of last access, seconds since Epoch\n\
  %y   समय of last data modification, human-readable\n\
  %Y   समय of last data modification, seconds since Epoch\n\
  %z   समय of last status change, human-readable\n\
  %Z   समय of last status change, seconds since Epoch\n\
\n\
"), मानक_निकास);

      ख_माला_दो (_("\
Valid format sequences लागि file systems:\n\
\n\
  %a   मुक्त blocks available to non-superuser\n\
  %b   total data blocks in file प्रणाली\n\
  %c   total file nodes in file प्रणाली\n\
  %d   मुक्त file nodes in file प्रणाली\n\
  %f   मुक्त blocks in file प्रणाली\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  %i   file प्रणाली ID in hex\n\
  %l   maximum length of filenames\n\
  %n   file name\n\
  %Qn  quoted file name\n\
  %s   block size (लागि faster transfers)\n\
  %S   fundamental block size (लागि block counts)\n\
  %t   file प्रणाली type in hex\n\
  %T   file प्रणाली type in human readable form\n\
"), मानक_निकास);

      म_लिखो (_("\n\
--terse is equivalent to the following FORMAT:\n\
    %s\
"),
#यदि HAVE_SELINUX_SELINUX_H
              fmt_terse_selinux
#अन्यथा
              fmt_terse_regular
#endif
              );

        म_लिखो (_("\
--terse --file-प्रणाली is equivalent to the following FORMAT:\n\
    %s\
"), fmt_terse_fs);

      म_लिखो (USAGE_BUILTIN_WARNING, PROGRAM_NAME);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
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
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  संरचना स्था_बदल अचर *locale = क्षेत्र_बदलो ();
  decimal_point = (locale->decimal_point[0] ? locale->decimal_point : ".");
  decimal_point_len = म_माप (decimal_point);

  निकास_पर (close_stdout);

  जबसम्म ((c = getopt_long (argc, argv, "c:fLt", long_options, रिक्त)) != -1)
    {
      विकल्प (c)
        {
        पक्ष PRINTF_OPTION:
          format = optarg;
          interpret_backslash_escapes = true;
          trailing_delim = "";
          रोक;

        पक्ष 'c':
          format = optarg;
          interpret_backslash_escapes = false;
          trailing_delim = "\n";
          रोक;

        पक्ष 'L':
          follow_links = true;
          रोक;

        पक्ष 'f':
          fs = true;
          रोक;

        पक्ष 't':
          terse = true;
          रोक;

        पक्ष 0:
          विकल्प (XARGMATCH ("--cached", optarg, cached_args, cached_modes))
            {
              पक्ष cached_never:
                force_sync = true;
                dont_sync = false;
                रोक;
              पक्ष cached_always:
                force_sync = false;
                dont_sync = true;
                रोक;
              पक्ष cached_default:
                force_sync = false;
                dont_sync = false;
            }
          रोक;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        स्वभाव:
          usage (निकास_त्रुटि);
        }
    }

  यदि (argc == optind)
    {
      error (0, 0, _("missing operand"));
      usage (निकास_त्रुटि);
    }

  यदि (format)
    {
      format2 = format;
    }
  अन्यथा
    {
      format = default_format (fs, terse, /* device= */ false);
      format2 = default_format (fs, terse, /* device= */ true);
    }

  लागि (पूर्णाङ्क i = optind; i < argc; i++)
    ok &= (fs
           ? do_statfs (argv[i], format)
           : do_stat (argv[i], format, format2));

  main_exit (ok ? निकास_सफल : निकास_त्रुटि);
}
