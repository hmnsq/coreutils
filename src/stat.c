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

#అన్తర్భావ <config.h>

/* Keep this conditional in sync with the similar conditional in
   ../m4/stat-prog.m4.  */
#ఒకవేళ ((STAT_STATVFS || STAT_STATVFS64)                                       \
     && (HAVE_STRUCT_STATVFS_F_BASETYPE || HAVE_STRUCT_STATVFS_F_FSTYPENAME \
         || (! HAVE_STRUCT_STATFS_F_FSTYPENAME && HAVE_STRUCT_STATVFS_F_TYPE)))
# define USE_STATVFS 1
#లేకపోతే
# define USE_STATVFS 0
#అయితే_ముగింపు

#అన్తర్భావ <stdio.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <pwd.h>
#అన్తర్భావ <grp.h>
#ఒకవేళ USE_STATVFS
# include <sys/statvfs.h>
#లేదావేళ HAVE_SYS_VFS_H
# include <sys/vfs.h>
#లేదావేళ HAVE_SYS_MOUNT_H && HAVE_SYS_PARAM_H
/* NOTE: freebsd5.0 needs sys/param.h and sys/mount.h for statfs.
   It does have statvfs.h, but shouldn't use it, since it doesn't
   HAVE_STRUCT_STATVFS_F_BASETYPE.  So find a clean way to fix it.  */
/* NetBSD 1.5.2 needs these, for the declaration of struct statfs. */
# include <sys/param.h>
# include <sys/mount.h>
# ఒకవేళ HAVE_NFS_NFS_CLNT_H && HAVE_NFS_VFS_H
/* Ultrix 4.4 needs these for the declaration of struct statfs.  */
#  include <netinet/in.h>
#  include <nfs/nfs_clnt.h>
#  include <nfs/vfs.h>
# endif
#లేదావేళ HAVE_OS_H /* BeOS */
# include <fs_info.h>
#అయితే_ముగింపు
#అన్తర్భావ <selinux/selinux.h>
#అన్తర్భావ <getopt.h>

#అన్తర్భావ "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#అన్తర్భావ "system.h"

#అన్తర్భావ "areadlink.h"
#అన్తర్భావ "c-ctype.h"
#అన్తర్భావ "file-type.h"
#అన్తర్భావ "filemode.h"
#అన్తర్భావ "fs.h"
#అన్తర్భావ "mountlist.h"
#అన్తర్భావ "octhexdigits.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "stat-size.h"
#అన్తర్భావ "stat-time.h"
#అన్తర్భావ "strftime.h"
#అన్తర్భావ "find-mount-point.h"
#అన్తర్భావ "xvasprintf.h"
#అన్తర్భావ "statx.h"

#ఒకవేళ HAVE_STATX && defined STATX_INO
# define USE_STATX 1
#లేకపోతే
# define USE_STATX 0
#అయితే_ముగింపు

#ఒకవేళ USE_STATVFS
# define STRUCT_STATXFS_F_FSID_IS_INTEGER STRUCT_STATVFS_F_FSID_IS_INTEGER
# define HAVE_STRUCT_STATXFS_F_TYPE HAVE_STRUCT_STATVFS_F_TYPE
# ఒకవేళ HAVE_STRUCT_STATVFS_F_NAMEMAX
#  define SB_F_NAMEMAX(S) ((S)->f_namemax)
# endif
# ఒకవేళ ! STAT_STATVFS && STAT_STATVFS64
#  define STRUCT_STATVFS నిర్మాణం statvfs64
#  define STATFS statvfs64
# లేకపోతే
#  define STRUCT_STATVFS నిర్మాణం statvfs
#  define STATFS statvfs
# endif
# define STATFS_FRSIZE(S) ((S)->f_frsize)
#లేకపోతే
# define HAVE_STRUCT_STATXFS_F_TYPE HAVE_STRUCT_STATFS_F_TYPE
# ఒకవేళ HAVE_STRUCT_STATFS_F_NAMELEN
#  define SB_F_NAMEMAX(S) ((S)->f_namelen)
# elif HAVE_STRUCT_STATFS_F_NAMEMAX
#  define SB_F_NAMEMAX(S) ((S)->f_namemax)
# endif
# define STATFS statfs
# ఒకవేళ HAVE_OS_H /* BeOS */
/* BeOS has a statvfs function, but it does not return sensible values
   for f_files, f_ffree and f_favail, and lacks f_type, f_basetype and
   f_fstypename.  Use 'struct fs_info' instead.  */
NODISCARD
స్థిర పూర్ణాంకం
statfs (అక్షరం స్థిరరాశి *filename, నిర్మాణం fs_info *buf)
{
  dev_t device = dev_for_మార్గం (filename);
  ఒకవేళ (device < 0)
    {
      errno = (device == B_నమోదు_NOT_దొరికింది ? ENOENT
               : device == B_BAD_విలువ ? EINVAL
               : device == B_NAME_TOO_LONG ? ENAMETOOLONG
               : device == B_NO_జ్ఞాపకం ? ENOMEM
               : device == B_FILE_ERROR ? EIO
               : 0);
      తిరిగి -1;
    }
  /* If successful, buf->dev will be == device.  */
  తిరిగి fs_stat_dev (device, buf);
}
#  define f_fsid dev
#  define f_ఖండాలు మొత్తం_ఖండాలు
#  define f_bfree free_ఖండాలు
#  define f_bavail free_ఖండాలు
#  define f_bsize io_కొలత
#  define f_files మొత్తం_nodes
#  define f_ffree free_nodes
#  define STRUCT_STATVFS నిర్మాణం fs_info
#  define STRUCT_STATXFS_F_FSID_IS_INTEGER true
#  define STATFS_FRSIZE(S) ((S)->ఖండం_కొలత)
# లేకపోతే
#  define STRUCT_STATVFS నిర్మాణం statfs
#  define STRUCT_STATXFS_F_FSID_IS_INTEGER STRUCT_STATFS_F_FSID_IS_INTEGER
#  ఒకవేళ HAVE_STRUCT_STATFS_F_FRSIZE
#   define STATFS_FRSIZE(S) ((S)->f_frsize)
#  లేకపోతే
#   define STATFS_FRSIZE(S) 0
#  endif
# endif
#అయితే_ముగింపు

#నిర్వచితమైతే SB_F_NAMEMAX
# define బహిర్గతం_NAMEMAX బహిర్గతం_uint
#లేకపోతే
/* Depending on whether statvfs or statfs is used,
   neither f_namemax or f_namelen may be available.  */
# define SB_F_NAMEMAX(S) "?"
# define బహిర్గతం_NAMEMAX బహిర్గతం_string
#అయితే_ముగింపు

#ఒకవేళ HAVE_STRUCT_STATVFS_F_BASETYPE
# define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME f_basetype
#లేకపోతే
# ఒకవేళ HAVE_STRUCT_STATVFS_F_FSTYPENAME || HAVE_STRUCT_STATFS_F_FSTYPENAME
#  define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME f_fstypename
# elif HAVE_OS_H /* BeOS */
#  define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME fsh_name
# endif
#అయితే_ముగింపు

#ఒకవేళ HAVE_GETATTRAT
# include <attr.h>
# include <sys/nvpair.h>
#అయితే_ముగింపు

స్థిర అక్షరం స్థిరరాశి digits[] = "0123456789";

/* Flags that are portable for use in printf, for at least one
   conversion specifier; make_format removes non-portable flags as
   needed for particular specifiers.  The glibc 2.2 extension "I" is
   listed here; it is removed by make_format because it has undefined
   behavior elsewhere and because it is incompatible with
   out_epoch_sec.  */
స్థిర అక్షరం స్థిరరాశి printf_flags[] = "'-+ #0I";

/* Formats for the --terse option.  */
స్థిర అక్షరం స్థిరరాశి fmt_terse_fs[] = "%Qn %i %l %t %s %S %b %f %a %c %d\n";
స్థిర అక్షరం స్థిరరాశి fmt_terse_regular[] = "%Qn %s %b %f %u %g %D %i %h %t %T"
                                        " %X %Y %Z %W %o\n";
స్థిర అక్షరం స్థిరరాశి fmt_terse_selinux[] = "%Qn %s %b %f %u %g %D %i %h %t %T"
                                        " %X %Y %Z %W %o %C\n";

#నిర్వచించు కార్యక్రమం_NAME "stat"

#నిర్వచించు AUTHORS proper_name ("Michael Meskes")

గణన
{
  PRINTF_OPTION = CHAR_MAX + 1
};

గణన cached_బహుళకం
{
  cached_default,
  cached_never,
  cached_always
};

స్థిర అక్షరం స్థిరరాశి *స్థిరరాశి cached_వాదనలు[] =
{
  "default", "never", "always", ఖాళీ
};

స్థిర గణన cached_బహుళకం స్థిరరాశి cached_modes[] =
{
  cached_default, cached_never, cached_always
};

స్థిర నిర్మాణం option స్థిరరాశి long_options[] =
{
  {"dereference", no_వాదం, ఖాళీ, 'L'},
  {"file-system", no_వాదం, ఖాళీ, 'f'},
  {"format", required_వాదం, ఖాళీ, 'c'},
  {"printf", required_వాదం, ఖాళీ, PRINTF_OPTION},
  {"terse", no_వాదం, ఖాళీ, 't'},
  {"cached", required_వాదం, ఖాళీ, 0},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0}
};

/* Whether to follow symbolic links;  True for --dereference (-L).  */
స్థిర bool follow_links;

/* Whether to interpret backslash-escape sequences.
   True for --printf=FMT, not for --format=FMT (-c).  */
స్థిర bool interpret_backslash_escapes;

/* The trailing delimiter string:
   "" for --printf=FMT, "\n" for --format=FMT (-c).  */
స్థిర అక్షరం స్థిరరాశి *trailing_delim = "";

/* The representation of the decimal point in the current locale.  */
స్థిర అక్షరం స్థిరరాశి *decimal_బిందువు;
స్థిర size_t decimal_బిందువు_len;

స్థిర bool
print_stat (అక్షరం *pformat, size_t ఉపసర్గ_len, అక్షరం mod, అక్షరం m,
            పూర్ణాంకం fd, అక్షరం స్థిరరాశి *filename, శూన్య స్థిరరాశి *దత్తాంశం);

/* Return the type of the specified file system.
   Some systems have statfvs.f_basetype[FSTYPSZ] (AIX, HP-UX, and Solaris).
   Others have statvfs.f_fstypename[_VFS_NAMELEN] (NetBSD 3.0).
   Others have statfs.f_fstypename[MFSNAMELEN] (NetBSD 1.5.2).
   Still others have neither and have to get by with f_type (GNU/Linux).
   But f_type may only exist in statfs (Cygwin).  */
NODISCARD
స్థిర అక్షరం స్థిరరాశి *
human_fstype (STRUCT_STATVFS స్థిరరాశి *statfsbuf)
{
#నిర్వచితమైతే STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME
  తిరిగి statfsbuf->STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME;
#లేకపోతే
  ఎంపిక (statfsbuf->f_type)
    {
# ఒకవేళ defined __linux__ || defined __ANDROID__

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

    స్థితి S_MAGIC_AAFS: /* 0x5A3C69F0 local */
      తిరిగి "aafs";
    స్థితి S_MAGIC_ACFS: /* 0x61636673 remote */
      తిరిగి "acfs";
    స్థితి S_MAGIC_ADFS: /* 0xADF5 local */
      తిరిగి "adfs";
    స్థితి S_MAGIC_AFFS: /* 0xADFF local */
      తిరిగి "affs";
    స్థితి S_MAGIC_AFS: /* 0x5346414F remote */
      తిరిగి "afs";
    స్థితి S_MAGIC_ANON_INODE_FS: /* 0x09041934 local */
      తిరిగి "anon-inode FS";
    స్థితి S_MAGIC_AUFS: /* 0x61756673 remote */
      /* FIXME: change syntax or add an optional attribute like "inotify:no".
         The above is labeled as "remote" so that tail always uses polling,
         but this isn't really a remote file system type.  */
      తిరిగి "aufs";
    స్థితి S_MAGIC_AUTOFS: /* 0x0187 local */
      తిరిగి "autofs";
    స్థితి S_MAGIC_BALLOON_KVM: /* 0x13661366 local */
      తిరిగి "balloon-kvm-fs";
    స్థితి S_MAGIC_BCACHEFS: /* 0xCA451A4E local */
      తిరిగి "bcachefs";
    స్థితి S_MAGIC_BEFS: /* 0x42465331 local */
      తిరిగి "befs";
    స్థితి S_MAGIC_BDEVFS: /* 0x62646576 local */
      తిరిగి "bdevfs";
    స్థితి S_MAGIC_BFS: /* 0x1BADFACE local */
      తిరిగి "bfs";
    స్థితి S_MAGIC_BINDERFS: /* 0x6C6F6F70 local */
      తిరిగి "binderfs";
    స్థితి S_MAGIC_BPF_FS: /* 0xCAFE4A11 local */
      తిరిగి "bpf_fs";
    స్థితి S_MAGIC_BINFMTFS: /* 0x42494E4D local */
      తిరిగి "binfmt_misc";
    స్థితి S_MAGIC_BTRFS: /* 0x9123683E local */
      తిరిగి "btrfs";
    స్థితి S_MAGIC_BTRFS_TEST: /* 0x73727279 local */
      తిరిగి "btrfs_test";
    స్థితి S_MAGIC_CEPH: /* 0x00C36400 remote */
      తిరిగి "ceph";
    స్థితి S_MAGIC_CGROUP: /* 0x0027E0EB local */
      తిరిగి "cgroupfs";
    స్థితి S_MAGIC_CGROUP2: /* 0x63677270 local */
      తిరిగి "cgroup2fs";
    స్థితి S_MAGIC_CIFS: /* 0xFF534D42 remote */
      తిరిగి "cifs";
    స్థితి S_MAGIC_CODA: /* 0x73757245 remote */
      తిరిగి "coda";
    స్థితి S_MAGIC_COH: /* 0x012FF7B7 local */
      తిరిగి "coh";
    స్థితి S_MAGIC_CONFIGFS: /* 0x62656570 local */
      తిరిగి "configfs";
    స్థితి S_MAGIC_CRAMFS: /* 0x28CD3D45 local */
      తిరిగి "cramfs";
    స్థితి S_MAGIC_CRAMFS_WEND: /* 0x453DCD28 local */
      తిరిగి "cramfs-wend";
    స్థితి S_MAGIC_DAXFS: /* 0x64646178 local */
      తిరిగి "daxfs";
    స్థితి S_MAGIC_DEBUGFS: /* 0x64626720 local */
      తిరిగి "debugfs";
    స్థితి S_MAGIC_DEVFS: /* 0x1373 local */
      తిరిగి "devfs";
    స్థితి S_MAGIC_DEVMEM: /* 0x454D444D local */
      తిరిగి "devmem";
    స్థితి S_MAGIC_DEVPTS: /* 0x1CD1 local */
      తిరిగి "devpts";
    స్థితి S_MAGIC_DMA_BUF: /* 0x444D4142 local */
      తిరిగి "dma-buf-fs";
    స్థితి S_MAGIC_ECRYPTFS: /* 0xF15F local */
      తిరిగి "ecryptfs";
    స్థితి S_MAGIC_EFIVARFS: /* 0xDE5E81E4 local */
      తిరిగి "efivarfs";
    స్థితి S_MAGIC_EFS: /* 0x00414A53 local */
      తిరిగి "efs";
    స్థితి S_MAGIC_EROFS_V1: /* 0xE0F5E1E2 local */
      తిరిగి "erofs";
    స్థితి S_MAGIC_EXFAT: /* 0x2011BAB0 local */
      తిరిగి "exfat";
    స్థితి S_MAGIC_EXFS: /* 0x45584653 local */
      తిరిగి "exfs";
    స్థితి S_MAGIC_EXOFS: /* 0x5DF5 local */
      తిరిగి "exofs";
    స్థితి S_MAGIC_EXT: /* 0x137D local */
      తిరిగి "ext";
    స్థితి S_MAGIC_EXT2: /* 0xEF53 local */
      తిరిగి "ext2/ext3";
    స్థితి S_MAGIC_EXT2_OLD: /* 0xEF51 local */
      తిరిగి "ext2";
    స్థితి S_MAGIC_F2FS: /* 0xF2F52010 local */
      తిరిగి "f2fs";
    స్థితి S_MAGIC_FAIL_FS: /* 0x4641494C local */
      తిరిగి "failfs";
    స్థితి S_MAGIC_FAT: /* 0x4006 local */
      తిరిగి "fat";
    స్థితి S_MAGIC_FHGFS: /* 0x19830326 remote */
      తిరిగి "fhgfs";
    స్థితి S_MAGIC_FUSE: /* 0x65735546 remote */
      తిరిగి "fuse";
    స్థితి S_MAGIC_FUSECTL: /* 0x65735543 remote */
      తిరిగి "fusectl";
    స్థితి S_MAGIC_FUTEXFS: /* 0x0BAD1DEA local */
      తిరిగి "futexfs";
    స్థితి S_MAGIC_GFS: /* 0x01161970 remote */
      తిరిగి "gfs/gfs2";
    స్థితి S_MAGIC_GPFS: /* 0x47504653 remote */
      తిరిగి "gpfs";
    స్థితి S_MAGIC_GUEST_MEMFD: /* 0x474D454D remote */
      తిరిగి "guest-memfd";
    స్థితి S_MAGIC_HFS: /* 0x4244 local */
      తిరిగి "hfs";
    స్థితి S_MAGIC_HFS_PLUS: /* 0x482B local */
      తిరిగి "hfs+";
    స్థితి S_MAGIC_HFS_X: /* 0x4858 local */
      తిరిగి "hfsx";
    స్థితి S_MAGIC_HOSTFS: /* 0x00C0FFEE local */
      తిరిగి "hostfs";
    స్థితి S_MAGIC_HPFS: /* 0xF995E849 local */
      తిరిగి "hpfs";
    స్థితి S_MAGIC_HUGETLBFS: /* 0x958458F6 local */
      తిరిగి "hugetlbfs";
    స్థితి S_MAGIC_MTD_INODE_FS: /* 0x11307854 local */
      తిరిగి "inodefs";
    స్థితి S_MAGIC_IBRIX: /* 0x013111A8 remote */
      తిరిగి "ibrix";
    స్థితి S_MAGIC_INOTIFYFS: /* 0x2BAD1DEA local */
      తిరిగి "inotifyfs";
    స్థితి S_MAGIC_ISOFS: /* 0x9660 local */
      తిరిగి "isofs";
    స్థితి S_MAGIC_ISOFS_R_WIN: /* 0x4004 local */
      తిరిగి "isofs";
    స్థితి S_MAGIC_ISOFS_WIN: /* 0x4000 local */
      తిరిగి "isofs";
    స్థితి S_MAGIC_JFFS: /* 0x07C0 local */
      తిరిగి "jffs";
    స్థితి S_MAGIC_JFFS2: /* 0x72B6 local */
      తిరిగి "jffs2";
    స్థితి S_MAGIC_JFS: /* 0x3153464A local */
      తిరిగి "jfs";
    స్థితి S_MAGIC_KAFS: /* 0x6B414653 remote */
      తిరిగి "k-afs";
    స్థితి S_MAGIC_LOGFS: /* 0xC97E8168 local */
      తిరిగి "logfs";
    స్థితి S_MAGIC_LUSTRE: /* 0x0BD00BD0 remote */
      తిరిగి "lustre";
    స్థితి S_MAGIC_M1FS: /* 0x5346314D local */
      తిరిగి "m1fs";
    స్థితి S_MAGIC_MINIX: /* 0x137F local */
      తిరిగి "minix";
    స్థితి S_MAGIC_MINIX_30: /* 0x138F local */
      తిరిగి "minix (30 char.)";
    స్థితి S_MAGIC_MINIX_V2: /* 0x2468 local */
      తిరిగి "minix v2";
    స్థితి S_MAGIC_MINIX_V2_30: /* 0x2478 local */
      తిరిగి "minix v2 (30 char.)";
    స్థితి S_MAGIC_MINIX_V3: /* 0x4D5A local */
      తిరిగి "minix3";
    స్థితి S_MAGIC_MQUEUE: /* 0x19800202 local */
      తిరిగి "mqueue";
    స్థితి S_MAGIC_MSDOS: /* 0x4D44 local */
      తిరిగి "msdos";
    స్థితి S_MAGIC_NCP: /* 0x564C remote */
      తిరిగి "novell";
    స్థితి S_MAGIC_NFS: /* 0x6969 remote */
      తిరిగి "nfs";
    స్థితి S_MAGIC_NFSD: /* 0x6E667364 remote */
      తిరిగి "nfsd";
    స్థితి S_MAGIC_NILFS: /* 0x3434 local */
      తిరిగి "nilfs";
    స్థితి S_MAGIC_NSFS: /* 0x6E736673 local */
      తిరిగి "nsfs";
    స్థితి S_MAGIC_NTFS: /* 0x5346544E local */
      తిరిగి "ntfs";
    స్థితి S_MAGIC_NULL_FS: /* 0x4E554C4C local */
      తిరిగి "nullfs";
    స్థితి S_MAGIC_OPENPROM: /* 0x9FA1 local */
      తిరిగి "openprom";
    స్థితి S_MAGIC_OCFS2: /* 0x7461636F remote */
      తిరిగి "ocfs2";
    స్థితి S_MAGIC_OVERLAYFS: /* 0x794C7630 remote */
      /* This may overlay remote file systems.
         Also there have been issues reported with inotify and overlayfs,
         so mark as "remote" so that polling is used.  */
      తిరిగి "overlayfs";
    స్థితి S_MAGIC_PANFS: /* 0xAAD7AAEA remote */
      తిరిగి "panfs";
    స్థితి S_MAGIC_PID_FS: /* 0x50494446 local */
      తిరిగి "pidfs";
    స్థితి S_MAGIC_PIPEFS: /* 0x50495045 remote */
      /* FIXME: change syntax or add an optional attribute like "inotify:no".
         pipefs is labeled as "remote" so that tail always polls,
         but it isn't really remote file system type.  */
      తిరిగి "pipefs";
    స్థితి S_MAGIC_PPC_CMM: /* 0xC7571590 local */
      తిరిగి "ppc-cmm-fs";
    స్థితి S_MAGIC_PRL_FS: /* 0x7C7C6673 remote */
      తిరిగి "prl_fs";
    స్థితి S_MAGIC_PROC: /* 0x9FA0 local */
      తిరిగి "proc";
    స్థితి S_MAGIC_PSTOREFS: /* 0x6165676C local */
      తిరిగి "pstorefs";
    స్థితి S_MAGIC_QNX4: /* 0x002F local */
      తిరిగి "qnx4";
    స్థితి S_MAGIC_QNX6: /* 0x68191122 local */
      తిరిగి "qnx6";
    స్థితి S_MAGIC_RAMFS: /* 0x858458F6 local */
      తిరిగి "ramfs";
    స్థితి S_MAGIC_RDTGROUP: /* 0x07655821 local */
      తిరిగి "rdt";
    స్థితి S_MAGIC_REISERFS: /* 0x52654973 local */
      తిరిగి "reiserfs";
    స్థితి S_MAGIC_ROMFS: /* 0x7275 local */
      తిరిగి "romfs";
    స్థితి S_MAGIC_RPC_PIPEFS: /* 0x67596969 local */
      తిరిగి "rpc_pipefs";
    స్థితి S_MAGIC_SDCARDFS: /* 0x5DCA2DF5 local */
      తిరిగి "sdcardfs";
    స్థితి S_MAGIC_SECRETMEM: /* 0x5345434D local */
      తిరిగి "secretmem";
    స్థితి S_MAGIC_SECURITYFS: /* 0x73636673 local */
      తిరిగి "securityfs";
    స్థితి S_MAGIC_SELINUX: /* 0xF97CFF8C local */
      తిరిగి "selinux";
    స్థితి S_MAGIC_SMACK: /* 0x43415D53 local */
      తిరిగి "smackfs";
    స్థితి S_MAGIC_SMB: /* 0x517B remote */
      తిరిగి "smb";
    స్థితి S_MAGIC_SMB2: /* 0xFE534D42 remote */
      తిరిగి "smb2";
    స్థితి S_MAGIC_SNFS: /* 0xBEEFDEAD remote */
      తిరిగి "snfs";
    స్థితి S_MAGIC_SOCKFS: /* 0x534F434B local */
      తిరిగి "sockfs";
    స్థితి S_MAGIC_SQUASHFS: /* 0x73717368 local */
      తిరిగి "squashfs";
    స్థితి S_MAGIC_SYSFS: /* 0x62656572 local */
      తిరిగి "sysfs";
    స్థితి S_MAGIC_SYSV2: /* 0x012FF7B6 local */
      తిరిగి "sysv2";
    స్థితి S_MAGIC_SYSV4: /* 0x012FF7B5 local */
      తిరిగి "sysv4";
    స్థితి S_MAGIC_TMPFS: /* 0x01021994 local */
      తిరిగి "tmpfs";
    స్థితి S_MAGIC_TRACEFS: /* 0x74726163 local */
      తిరిగి "tracefs";
    స్థితి S_MAGIC_UBIFS: /* 0x24051905 local */
      తిరిగి "ubifs";
    స్థితి S_MAGIC_UDF: /* 0x15013346 local */
      తిరిగి "udf";
    స్థితి S_MAGIC_UFS: /* 0x00011954 local */
      తిరిగి "ufs";
    స్థితి S_MAGIC_UFS_BYTESWAPPED: /* 0x54190100 local */
      తిరిగి "ufs";
    స్థితి S_MAGIC_USBDEVFS: /* 0x9FA2 local */
      తిరిగి "usbdevfs";
    స్థితి S_MAGIC_V9FS: /* 0x01021997 local */
      తిరిగి "v9fs";
    స్థితి S_MAGIC_VBOXSF: /* 0x786F4256 remote */
      తిరిగి "vboxsf";
    స్థితి S_MAGIC_VMHGFS: /* 0xBACBACBC remote */
      తిరిగి "vmhgfs";
    స్థితి S_MAGIC_VXFS: /* 0xA501FCF5 remote */
      /* Veritas File System can run in single instance or clustered mode,
         so mark as remote to cater for the latter case.  */
      తిరిగి "vxfs";
    స్థితి S_MAGIC_VZFS: /* 0x565A4653 local */
      తిరిగి "vzfs";
    స్థితి S_MAGIC_WSLFS: /* 0x53464846 local */
      తిరిగి "wslfs";
    స్థితి S_MAGIC_XENFS: /* 0xABBA1974 local */
      తిరిగి "xenfs";
    స్థితి S_MAGIC_XENIX: /* 0x012FF7B4 local */
      తిరిగి "xenix";
    స్థితి S_MAGIC_XFS: /* 0x58465342 local */
      తిరిగి "xfs";
    స్థితి S_MAGIC_XIAFS: /* 0x012FD16D local */
      తిరిగి "xia";
    స్థితి S_MAGIC_Z3FOLD: /* 0x0033 local */
      తిరిగి "z3fold";
    స్థితి S_MAGIC_ZFS: /* 0x2FC12FC1 local */
      తిరిగి "zfs";
    స్థితి S_MAGIC_ZONEFS: /* 0x5A4F4653 local */
      తిరిగి "zonefs";
    స్థితి S_MAGIC_ZSMALLOC: /* 0x58295829 local */
      తిరిగి "zsmallocfs";


# elif __GNU__
    స్థితి FSTYPE_UFS:
      తిరిగి "ufs";
    స్థితి FSTYPE_NFS:
      తిరిగి "nfs";
    స్థితి FSTYPE_GFS:
      తిరిగి "gfs";
    స్థితి FSTYPE_LFS:
      తిరిగి "lfs";
    స్థితి FSTYPE_SYSV:
      తిరిగి "sysv";
    స్థితి FSTYPE_FTP:
      తిరిగి "ftp";
    స్థితి FSTYPE_TAR:
      తిరిగి "tar";
    స్థితి FSTYPE_AR:
      తిరిగి "ar";
    స్థితి FSTYPE_CPIO:
      తిరిగి "cpio";
    స్థితి FSTYPE_MSLOSS:
      తిరిగి "msloss";
    స్థితి FSTYPE_CPM:
      తిరిగి "cpm";
    స్థితి FSTYPE_HFS:
      తిరిగి "hfs";
    స్థితి FSTYPE_DTFS:
      తిరిగి "dtfs";
    స్థితి FSTYPE_GRFS:
      తిరిగి "grfs";
    స్థితి FSTYPE_పదం:
      తిరిగి "term";
    స్థితి FSTYPE_DEV:
      తిరిగి "dev";
    స్థితి FSTYPE_PROC:
      తిరిగి "proc";
    స్థితి FSTYPE_IFSOCK:
      తిరిగి "ifsock";
    స్థితి FSTYPE_AFS:
      తిరిగి "afs";
    స్థితి FSTYPE_DFS:
      తిరిగి "dfs";
    స్థితి FSTYPE_PROC9:
      తిరిగి "proc9";
    స్థితి FSTYPE_SOCKET:
      తిరిగి "socket";
    స్థితి FSTYPE_MISC:
      తిరిగి "misc";
    స్థితి FSTYPE_EXT2FS:
      తిరిగి "ext2/ext3";
    స్థితి FSTYPE_HTTP:
      తిరిగి "http";
    స్థితి FSTYPE_MEMFS:
      తిరిగి "memfs";
    స్థితి FSTYPE_ISO9660:
      తిరిగి "iso9660";
# endif
    అప్రమేయ:
      {
        గుర్తులేని దీర్ఘ పూర్ణాంకం type = statfsbuf->f_type;
        స్థిర అక్షరం buf[పరిమాణం "UNKNOWN (0x%lx)" - 3
                        + (పరిమాణం type * CHAR_BIT + 3) / 4];
        sprintf (buf, "UNKNOWN (0x%lx)", type);
        తిరిగి buf;
      }
    }
#అయితే_ముగింపు
}

NODISCARD
స్థిర అక్షరం *
human_access (నిర్మాణం stat స్థిరరాశి *statbuf)
{
  స్థిర అక్షరం modebuf[12];
  filemodestring (statbuf, modebuf);
  modebuf[10] = 0;
  తిరిగి modebuf;
}

NODISCARD
స్థిర అక్షరం *
human_సమయం (నిర్మాణం timespec t)
{
  /* STR must be at least INT_BUFSIZE_BOUND (intmax_t) big, either
     because localtime_rz fails, or because the time zone is truly
     outlandish so that %z expands to a long string.  */
  స్థిర అక్షరం str[INT_BUFSIZE_BOUND (intmax_t)
                  + INT_STRLEN_BOUND (పూర్ణాంకం) /* YYYY */
                  + 1 /* because YYYY might equal INT_MAX + 1900 */
                  + పరిమాణం "-MM-DD HH:MM:SS.NNNNNNNNN +"];
  స్థిర timezone_t tz;
  ఒకవేళ (!tz)
    tz = tzalloc (getenv ("TZ"));
  నిర్మాణం tm tm;
  పూర్ణాంకం ns = t.tv_nsec;
  ఒకవేళ (localtime_rz (tz, &t.tv_sec, &tm))
    nstrftime (str, పరిమాణం str, "%Y-%m-%d %H:%M:%S.%N %z", &tm, tz, ns);
  లేకపోతే
    {
      అక్షరం secbuf[INT_BUFSIZE_BOUND (intmax_t)];
      sprintf (str, "%s.%09d", timetostr (t.tv_sec, secbuf), ns);
    }
  తిరిగి str;
}

/* PFORMAT points to a '%' followed by a prefix of a format, all of
   size PREFIX_LEN.  The flags allowed for this format are
   ALLOWED_FLAGS; remove other printf flags from the prefix, then
   append SUFFIX.  */
స్థిర శూన్య
make_format (అక్షరం *pformat, size_t ఉపసర్గ_len, అక్షరం స్థిరరాశి *allowed_flags,
             అక్షరం స్థిరరాశి *suffix)
{
  అక్షరం *dst = pformat + 1;
  అక్షరం స్థిరరాశి *src;
  అక్షరం స్థిరరాశి *srclim = pformat + ఉపసర్గ_len;
  కొరకు (src = dst; src < srclim && strchr (printf_flags, *src); src++)
    ఒకవేళ (strchr (allowed_flags, *src))
      *dst++ = *src;
  అయితే (src < srclim)
    *dst++ = *src++;
  strcpy (dst, suffix);
}

స్థిర శూన్య
బహిర్గతం_string (అక్షరం *pformat, size_t ఉపసర్గ_len, అక్షరం స్థిరరాశి *arg)
{
  make_format (pformat, ఉపసర్గ_len, "-", "s");
  రాయి (pformat, arg);
}
స్థిర పూర్ణాంకం
బహిర్గతం_int (అక్షరం *pformat, size_t ఉపసర్గ_len, intmax_t arg)
{
  make_format (pformat, ఉపసర్గ_len, "'-+ 0", "jd");
  తిరిగి రాయి (pformat, arg);
}
స్థిర పూర్ణాంకం
బహిర్గతం_uint (అక్షరం *pformat, size_t ఉపసర్గ_len, uintmax_t arg)
{
  make_format (pformat, ఉపసర్గ_len, "'-0", "ju");
  తిరిగి రాయి (pformat, arg);
}
స్థిర శూన్య
బహిర్గతం_uint_o (అక్షరం *pformat, size_t ఉపసర్గ_len, uintmax_t arg)
{
  make_format (pformat, ఉపసర్గ_len, "-#0", "jo");
  రాయి (pformat, arg);
}
స్థిర శూన్య
బహిర్గతం_uint_x (అక్షరం *pformat, size_t ఉపసర్గ_len, uintmax_t arg)
{
  make_format (pformat, ఉపసర్గ_len, "-#0", "jx");
  రాయి (pformat, arg);
}
స్థిర పూర్ణాంకం
బహిర్గతం_minus_zero (అక్షరం *pformat, size_t ఉపసర్గ_len)
{
  make_format (pformat, ఉపసర్గ_len, "'-+ 0", ".0f");
  తిరిగి రాయి (pformat, -0.25);
}

/* Output the number of seconds since the Epoch, using a format that
   acts like printf's %f format.  */
స్థిర శూన్య
బహిర్గతం_epoch_sec (అక్షరం *pformat, size_t ఉపసర్గ_len,
               నిర్మాణం timespec arg)
{
  అక్షరం *dot = memchr (pformat, '.', ఉపసర్గ_len);
  size_t sec_ఉపసర్గ_len = ఉపసర్గ_len;
  పూర్ణాంకం వెడల్పు = 0;
  పూర్ణాంకం precision = 0;
  bool frac_ఎడమ_adjust = false;

  ఒకవేళ (dot)
    {
      sec_ఉపసర్గ_len = dot - pformat;
      pformat[ఉపసర్గ_len] = '\0';

      ఒకవేళ (c_isdigit (dot[1]))
        {
          దీర్ఘ పూర్ణాంకం lprec = strtol (dot + 1, ఖాళీ, 10);
          precision = (lprec <= గరిష్ఠ_పూర్ణాంకం ? lprec : గరిష్ఠ_పూర్ణాంకం);
        }
      లేకపోతే
        {
          precision = 9;
        }

      ఒకవేళ (precision && c_isdigit (dot[-1]))
        {
          /* If a nontrivial width is given, subtract the width of the
             decimal point and PRECISION digits that will be output
             later.  */
          అక్షరం *p = dot;
          *dot = '\0';

          చేయి
            --p;
          అయితే (c_isdigit (p[-1]));

          దీర్ఘ పూర్ణాంకం lwidth = strtol (p, ఖాళీ, 10);
          వెడల్పు = (lwidth <= గరిష్ఠ_పూర్ణాంకం ? lwidth : గరిష్ఠ_పూర్ణాంకం);
          ఒకవేళ (1 < వెడల్పు)
            {
              p += (*p == '0');
              sec_ఉపసర్గ_len = p - pformat;
              పూర్ణాంకం w_d = (decimal_బిందువు_len < వెడల్పు
                         ? వెడల్పు - decimal_బిందువు_len
                         : 0);
              ఒకవేళ (1 < w_d)
                {
                  పూర్ణాంకం w = w_d - precision;
                  ఒకవేళ (1 < w)
                    {
                      అక్షరం *dst = pformat;
                      కొరకు (అక్షరం స్థిరరాశి *src = dst; src < p; src++)
                        {
                          ఒకవేళ (*src == '-')
                            frac_ఎడమ_adjust = true;
                          లేకపోతే
                            *dst++ = *src;
                        }
                      sec_ఉపసర్గ_len =
                        (dst - pformat
                         + (frac_ఎడమ_adjust ? 0 : sprintf (dst, "%d", w)));
                    }
                }
            }
        }
    }

  పూర్ణాంకం భాజకం = 1;
  కొరకు (పూర్ణాంకం i = precision; i < 9; i++)
    భాజకం *= 10;
  పూర్ణాంకం frac_sec = arg.tv_nsec / భాజకం;
  పూర్ణాంకం int_len;

  ఒకవేళ (TYPE_SIGNED (time_t))
    {
      bool minus_zero = false;
      ఒకవేళ (arg.tv_sec < 0 && arg.tv_nsec != 0)
        {
          పూర్ణాంకం frac_sec_modulus = 1000000000 / భాజకం;
          frac_sec = (frac_sec_modulus - frac_sec
                      - (arg.tv_nsec % భాజకం != 0));
          arg.tv_sec += (frac_sec != 0);
          minus_zero = (arg.tv_sec == 0);
        }
      int_len = (minus_zero
                 ? బహిర్గతం_minus_zero (pformat, sec_ఉపసర్గ_len)
                 : బహిర్గతం_int (pformat, sec_ఉపసర్గ_len, arg.tv_sec));
    }
  లేకపోతే
    int_len = బహిర్గతం_uint (pformat, sec_ఉపసర్గ_len, arg.tv_sec);

  ఒకవేళ (precision)
    {
      పూర్ణాంకం prec = (precision < 9 ? precision : 9);
      పూర్ణాంకం trailing_prec = precision - prec;
      పూర్ణాంకం ilen = (int_len < 0 ? 0 : int_len);
      పూర్ణాంకం trailing_వెడల్పు = (ilen < వెడల్పు && decimal_బిందువు_len < వెడల్పు - ilen
                            ? వెడల్పు - ilen - decimal_బిందువు_len - prec
                            : 0);
      రాయి ("%s%.*d%-*.*d", decimal_బిందువు, prec, frac_sec,
              trailing_వెడల్పు, trailing_prec, 0);
    }
}

/* Print the context information of FILENAME, and return true iff the
   context could not be obtained.  */
NODISCARD
స్థిర bool
బహిర్గతం_file_context (అక్షరం *pformat, size_t ఉపసర్గ_len, అక్షరం స్థిరరాశి *filename)
{
  అక్షరం *scontext;
  bool fail = false;

  ఒకవేళ ((follow_links
       ? getfilecon (filename, &scontext)
       : lgetfilecon (filename, &scontext)) < 0)
    {
      error (0, errno, _("failed to get security context of %s"),
             quoteaf (filename));
      scontext = ఖాళీ;
      fail = true;
    }
  strcpy (pformat + ఉపసర్గ_len, "s");
  రాయి (pformat, (scontext ? scontext : "?"));
  ఒకవేళ (scontext)
    freecon (scontext);
  తిరిగి fail;
}

/* Set the quoting style once it is needed.  */
స్థిర శూన్య
initialize_quoting_style (శూన్య)
{
  స్థిర bool initialized;
  ఒకవేళ (!initialized)
    {
      initialized = true;
      పూర్ణాంకం qs = getenv_quoting_style ();
      సమితి_quoting_style (ఖాళీ, 0 <= qs ? qs : shell_escape_quoting_style);
    }
}

/* Print statfs info.  Return zero upon success, nonzero upon failure.  */
NODISCARD
స్థిర bool
print_statfs (అక్షరం *pformat, size_t ఉపసర్గ_len, MAYBE_వాడని అక్షరం mod, అక్షరం m,
              MAYBE_వాడని పూర్ణాంకం fd, అక్షరం స్థిరరాశి *filename,
              శూన్య స్థిరరాశి *దత్తాంశం)
{
  STRUCT_STATVFS స్థిరరాశి *statfsbuf = దత్తాంశం;
  bool fail = false;

  ఎంపిక (m)
    {
    స్థితి 'n':
      ఒకవేళ (mod == 'Q')
        {
          initialize_quoting_style ();
          filename = quoteN (filename);
        }
      బహిర్గతం_string (pformat, ఉపసర్గ_len, filename);
      ఆపు;

    స్థితి 'i':
      {
#ఒకవేళ STRUCT_STATXFS_F_FSID_IS_INTEGER
        uintmax_t fsid = statfsbuf->f_fsid;
#లేకపోతే
        రకనిర్వచనం గుర్తులేని పూర్ణాంకం fsid_మాట;
        static_assert (alignof (STRUCT_STATVFS) % alignof (fsid_మాట) == 0);
        static_assert (offsetof (STRUCT_STATVFS, f_fsid) % alignof (fsid_మాట)
                       == 0);
        static_assert (పరిమాణం statfsbuf->f_fsid % alignof (fsid_మాట) == 0);
        fsid_మాట స్థిరరాశి *p = (fsid_మాట *) &statfsbuf->f_fsid;

        /* Assume a little-endian word order, as that is compatible
           with glibc's statvfs implementation.  */
        uintmax_t fsid = 0;
        పూర్ణాంకం పదాలు = పరిమాణం statfsbuf->f_fsid / పరిమాణం *p;
        కొరకు (పూర్ణాంకం i = 0; i < పదాలు && i * పరిమాణం *p < పరిమాణం fsid; i++)
          {
            uintmax_t u = p[పదాలు - 1 - i];
            fsid |= u << (i * CHAR_BIT * పరిమాణం *p);
          }
#అయితే_ముగింపు
        బహిర్గతం_uint_x (pformat, ఉపసర్గ_len, fsid);
      }
      ఆపు;

    స్థితి 'l':
      బహిర్గతం_NAMEMAX (pformat, ఉపసర్గ_len, SB_F_NAMEMAX (statfsbuf));
      ఆపు;
    స్థితి 't':
#ఒకవేళ HAVE_STRUCT_STATXFS_F_TYPE
      బహిర్గతం_uint_x (pformat, ఉపసర్గ_len, statfsbuf->f_type);
#లేకపోతే
      fputc ('?', stdout);
#అయితే_ముగింపు
      ఆపు;
    స్థితి 'T':
      బహిర్గతం_string (pformat, ఉపసర్గ_len, human_fstype (statfsbuf));
      ఆపు;
    స్థితి 'b':
      బహిర్గతం_int (pformat, ఉపసర్గ_len, statfsbuf->f_ఖండాలు);
      ఆపు;
    స్థితి 'f':
      బహిర్గతం_int (pformat, ఉపసర్గ_len, statfsbuf->f_bfree);
      ఆపు;
    స్థితి 'a':
      బహిర్గతం_int (pformat, ఉపసర్గ_len, statfsbuf->f_bavail);
      ఆపు;
    స్థితి 's':
      బహిర్గతం_uint (pformat, ఉపసర్గ_len, statfsbuf->f_bsize);
      ఆపు;
    స్థితి 'S':
      {
        uintmax_t frsize = STATFS_FRSIZE (statfsbuf);
        ఒకవేళ (! frsize)
          frsize = statfsbuf->f_bsize;
        బహిర్గతం_uint (pformat, ఉపసర్గ_len, frsize);
      }
      ఆపు;
    స్థితి 'c':
      బహిర్గతం_uint (pformat, ఉపసర్గ_len, statfsbuf->f_files);
      ఆపు;
    స్థితి 'd':
      బహిర్గతం_int (pformat, ఉపసర్గ_len, statfsbuf->f_ffree);
      ఆపు;
    అప్రమేయ:
      fputc ('?', stdout);
      ఆపు;
    }
  తిరిగి fail;
}

/* Return any bind mounted source for a path.
   The caller should not free the returned buffer.
   Return NULL if no bind mount found.  */
NODISCARD
స్థిర అక్షరం స్థిరరాశి *
కనుగొను_bind_mount (అక్షరం స్థిరరాశి * name)
{
  అక్షరం స్థిరరాశి * bind_mount = ఖాళీ;

  స్థిర నిర్మాణం mount_నమోదు *mount_list;
  స్థిర bool tried_mount_list = false;
  ఒకవేళ (!tried_mount_list) /* attempt/warn once per process.  */
    {
      ఒకవేళ (!(mount_list = read_file_system_list (false)))
        error (0, errno, "%s", _("cannot read table of mounted file systems"));
      tried_mount_list = true;
    }

  నిర్మాణం stat name_stats;
  ఒకవేళ (stat (name, &name_stats) != 0)
    తిరిగి ఖాళీ;

  కొరకు (నిర్మాణం mount_నమోదు *me = mount_list; me; me = me->me_తదుపరి)
    {
      ఒకవేళ (me->me_dummy && me->me_devname[0] == '/'
          && streq (me->me_mountdir, name))
        {
          నిర్మాణం stat dev_stats;

          ఒకవేళ (stat (me->me_devname, &dev_stats) == 0
              && psame_inode (&name_stats, &dev_stats))
            {
              bind_mount = me->me_devname;
              ఆపు;
            }
        }
    }

  తిరిగి bind_mount;
}

/* Print mount point.  Return zero upon success, nonzero upon failure.  */
NODISCARD
స్థిర bool
బహిర్గతం_mount_బిందువు (అక్షరం స్థిరరాశి *filename, అక్షరం *pformat, size_t ఉపసర్గ_len,
                 స్థిరరాశి నిర్మాణం stat *statp)
{

  అక్షరం స్థిరరాశి *np = "?", *bp = ఖాళీ;
  అక్షరం *mp = ఖాళీ;
  bool fail = true;

  /* Look for bind mounts first.  Note we output the immediate alias,
     rather than further resolving to a base device mount point.  */
  ఒకవేళ (follow_links || !S_ISLNK (statp->st_బహుళకం))
    {
      అక్షరం *resolved = canonicalize_file_name (filename);
      ఒకవేళ (!resolved)
        {
          error (0, errno, _("failed to canonicalize %s"), quoteaf (filename));
          వెళ్ళు print_mount_బిందువు;
        }
      bp = కనుగొను_bind_mount (resolved);
      విడుదల (resolved);
      ఒకవేళ (bp)
        {
          fail = false;
          వెళ్ళు print_mount_బిందువు;
        }
    }

  /* If there is no direct bind mount, then navigate
     back up the tree looking for a device change.
     Note we don't detect if any of the directory components
     are bind mounted to the same device, but that's OK
     since we've not directly queried them.  */
  ఒకవేళ ((mp = కనుగొను_mount_బిందువు (filename, statp)))
    {
      /* This dir might be bind mounted to another device,
         so we resolve the bound source in that case also.  */
      bp = కనుగొను_bind_mount (mp);
      fail = false;
    }

print_mount_బిందువు:

  బహిర్గతం_string (pformat, ఉపసర్గ_len, bp ? bp : mp ? mp : np);
  విడుదల (mp);
  తిరిగి fail;
}

/* Map a TS with negative TS.tv_nsec to {0,0}.  */
స్థిర అంతఃవరుస నిర్మాణం timespec
neg_గమ్యం_zero (నిర్మాణం timespec ts)
{
  ఒకవేళ (0 <= ts.tv_nsec)
    తిరిగి ts;
  నిర్మాణం timespec z = {0};
  తిరిగి z;
}

/* Output a single-character \ escape.  */

స్థిర శూన్య
print_esc_char (అక్షరం c)
{
  ఎంపిక (c)
    {
    స్థితి 'a':			/* Alert. */
      c ='\a';
      ఆపు;
    స్థితి 'b':			/* Backspace. */
      c ='\b';
      ఆపు;
    స్థితి 'e':			/* Escape. */
      c ='\x1B';
      ఆపు;
    స్థితి 'f':			/* Form feed. */
      c ='\f';
      ఆపు;
    స్థితి 'n':			/* New line. */
      c ='\n';
      ఆపు;
    స్థితి 'r':			/* Carriage return. */
      c ='\r';
      ఆపు;
    స్థితి 't':			/* Horizontal tab. */
      c ='\t';
      ఆపు;
    స్థితి 'v':			/* Vertical tab. */
      c ='\v';
      ఆపు;
    స్థితి '"':
    స్థితి '\\':
      ఆపు;
    అప్రమేయ:
      error (0, 0, _("warning: unrecognized escape '\\%c'"), c);
      ఆపు;
    }
  అక్షరం_రాయి (c);
}

ATTRIBUTE_శుద్ధ
స్థిర size_t
format_సంకేతం_స్థానభ్రంశం (అక్షరం స్థిరరాశి *directive)
{
  size_t len = strspn (directive + 1, printf_flags);
  అక్షరం స్థిరరాశి *fmt_char = directive + len + 1;
  fmt_char += strspn (fmt_char, digits);
  ఒకవేళ (*fmt_char == '.')
    fmt_char += 1 + strspn (fmt_char + 1, digits);
  తిరిగి fmt_char - directive;
}

/* Print the information specified by the format string, FORMAT,
   calling PRINT_FUNC for each %-directive encountered.
   Return zero upon success, nonzero upon failure.  */
NODISCARD
స్థిర bool
print_it (అక్షరం స్థిరరాశి *format, పూర్ణాంకం fd, అక్షరం స్థిరరాశి *filename,
          bool (*print_func) (అక్షరం *, size_t, అక్షరం, అక్షరం,
                              పూర్ణాంకం, అక్షరం స్థిరరాశి *, శూన్య స్థిరరాశి *),
          శూన్య స్థిరరాశి *దత్తాంశం)
{
  bool fail = false;

  /* Add 2 to accommodate our conversion of the stat '%s' format string
     to the longer printf '%llu' one.  */
  గణన
    {
      MAX_ADDITIONAL_బైట్లు =
        (MAX (పరిమాణం "jd",
              MAX (పరిమాణం "jo", MAX (పరిమాణం "ju", పరిమాణం "jx")))
         - 1)
    };
  size_t n_alloc = అక్షరశ్రేణి_పొడవు (format) + MAX_ADDITIONAL_బైట్లు + 1;
  అక్షరం *dest = xmalloc (n_alloc);
  కొరకు (అక్షరం స్థిరరాశి *b = format; *b; b++)
    {
      ఎంపిక (*b)
        {
        స్థితి '%':
          {
            size_t len = format_సంకేతం_స్థానభ్రంశం (b);
            అక్షరం fmt_char = *(b + len);
            అక్షరం mod_char = 0;
            జ్ఞాపకం_నకలు (dest, b, len);
            b += len;

            ఎంపిక (fmt_char)
              {
              స్థితి '\0':
                --b;
                FALLTHROUGH;
              స్థితి '%':
                ఒకవేళ (1 < len)
                  {
                    dest[len] = fmt_char;
                    dest[len + 1] = '\0';
                    error (EXIT_FAILURE, 0, _("%s: invalid directive"),
                           quote (dest));
                  }
                అక్షరం_రాయి ('%');
                ఆపు;
              స్థితి 'H':
              స్థితి 'L':
              స్థితి 'Q':
                mod_char = fmt_char;
                fmt_char = *(b + 1);
                ఒకవేళ ((mod_char == 'Q' && fmt_char == 'n')
                    || (print_func == print_stat
                        && (mod_char == 'H' || mod_char == 'L')
                        && (fmt_char == 'd' || fmt_char == 'r')))
                  {
                    b++;
                  }
                లేకపోతే
                  {
                    fmt_char = mod_char;
                    mod_char = 0;
                  }
                FALLTHROUGH;
              అప్రమేయ:
                fail |= print_func (dest, len, mod_char, fmt_char,
                                    fd, filename, దత్తాంశం);
                ఆపు;
              }
            ఆపు;
          }

        స్థితి '\\':
          ఒకవేళ ( ! interpret_backslash_escapes)
            {
              అక్షరం_రాయి ('\\');
              ఆపు;
            }
          ++b;
          ఒకవేళ (isoct (*b))
            {
              పూర్ణాంకం esc_విలువ = fromoct (*b);
              పూర్ణాంకం esc_నిడివి = 1;	/* number of octal digits */
              కొరకు (++b; esc_నిడివి < 3 && isoct (*b);
                   ++esc_నిడివి, ++b)
                {
                  esc_విలువ = esc_విలువ * 8 + fromoct (*b);
                }
              అక్షరం_రాయి (esc_విలువ);
              --b;
            }
          లేకపోతే ఒకవేళ (*b == 'x' && c_isxdigit (b[1]))
            {
              పూర్ణాంకం esc_విలువ = fromhex (b[1]);	/* Value of \xhh escape. */
              /* A hexadecimal \xhh escape sequence must have
                 1 or 2 hex. digits.  */
              ++b;
              ఒకవేళ (c_isxdigit (b[1]))
                {
                  ++b;
                  esc_విలువ = esc_విలువ * 16 + fromhex (*b);
                }
              అక్షరం_రాయి (esc_విలువ);
            }
          లేకపోతే ఒకవేళ (*b == '\0')
            {
              error (0, 0, _("warning: backslash at end of format"));
              అక్షరం_రాయి ('\\');
              /* Arrange to exit the loop.  */
              --b;
            }
          లేకపోతే
            {
              print_esc_char (*b);
            }
          ఆపు;

        అప్రమేయ:
          అక్షరం_రాయి (*b);
          ఆపు;
        }
    }
  విడుదల (dest);

  fputs (trailing_delim, stdout);

  తిరిగి fail;
}

/* Stat the file system and print what we find.  */
NODISCARD
స్థిర bool
do_statfs (అక్షరం స్థిరరాశి *filename, అక్షరం స్థిరరాశి *format)
{
  STRUCT_STATVFS statfsbuf;

  ఒకవేళ (streq (filename, "-"))
    {
      error (0, 0, _("using %s to denote standard input does not work"
                     " in file system mode"), quoteaf (filename));
      తిరిగి false;
    }

  ఒకవేళ (STATFS (filename, &statfsbuf) != 0)
    {
      error (0, errno, _("cannot read file system information for %s"),
             quoteaf (filename));
      తిరిగి false;
    }

  bool fail = print_it (format, -1, filename, print_statfs, &statfsbuf);
  తిరిగి ! fail;
}

నిర్మాణం print_వాదనలు {
  నిర్మాణం stat *st;
  నిర్మాణం timespec btime;
};

/* Ask statx to avoid syncing? */
స్థిర bool dont_sync;

/* Ask statx to force sync? */
స్థిర bool force_sync;

#ఒకవేళ USE_STATX
స్థిర గుర్తులేని పూర్ణాంకం
fmt_గమ్యం_ముసుగు (అక్షరం fmt)
{
  ఎంపిక (fmt)
    {
    స్థితి 'N':
      తిరిగి STATX_బహుళకం;
    స్థితి 'd':
    స్థితి 'D':
      తిరిగి STATX_బహుళకం;
    స్థితి 'i':
      తిరిగి STATX_INO;
    స్థితి 'a':
    స్థితి 'A':
      తిరిగి STATX_బహుళకం;
    స్థితి 'f':
      తిరిగి STATX_బహుళకం|STATX_TYPE;
    స్థితి 'F':
      తిరిగి STATX_TYPE;
    స్థితి 'h':
      తిరిగి STATX_NLINK;
    స్థితి 'u':
    స్థితి 'U':
      తిరిగి STATX_UID;
    స్థితి 'g':
    స్థితి 'G':
      తిరిగి STATX_GID;
    స్థితి 'm':
      తిరిగి STATX_బహుళకం|STATX_INO;
    స్థితి 's':
      తిరిగి STATX_కొలత;
    స్థితి 't':
    స్థితి 'T':
      తిరిగి STATX_బహుళకం;
    స్థితి 'b':
      తిరిగి STATX_ఖండాలు;
    స్థితి 'w':
    స్థితి 'W':
      తిరిగి STATX_BTIME;
    స్థితి 'x':
    స్థితి 'X':
      తిరిగి STATX_ATIME;
    స్థితి 'y':
    స్థితి 'Y':
      తిరిగి STATX_MTIME;
    స్థితి 'z':
    స్థితి 'Z':
      తిరిగి STATX_CTIME;
    }
  తిరిగి 0;
}

ATTRIBUTE_శుద్ధ
స్థిర గుర్తులేని పూర్ణాంకం
format_గమ్యం_ముసుగు (అక్షరం స్థిరరాశి *format)
{
  గుర్తులేని పూర్ణాంకం ముసుగు = 0;

  కొరకు (అక్షరం స్థిరరాశి *b = format; *b; b++)
    {
      ఒకవేళ (*b != '%')
        కొనసాగు;

      b += format_సంకేతం_స్థానభ్రంశం (b);
      ఒకవేళ (*b == '\0')
        ఆపు;
      ముసుగు |= fmt_గమ్యం_ముసుగు (*b);
    }
  తిరిగి ముసుగు;
}

/* statx the file and print what we find */
NODISCARD
స్థిర bool
do_stat (అక్షరం స్థిరరాశి *filename, అక్షరం స్థిరరాశి *format, అక్షరం స్థిరరాశి *format2)
{
  పూర్ణాంకం fd = streq (filename, "-") ? 0 : AT_FDCWD;
  పూర్ణాంకం flags = 0;
  నిర్మాణం stat st;
  నిర్మాణం statx stx = {0};
  అక్షరం స్థిరరాశి *pathname = filename;
  నిర్మాణం print_వాదనలు pa;
  pa.st = &st;
  pa.btime = (నిర్మాణం timespec) {.tv_sec = -1, .tv_nsec = -1};

  ఒకవేళ (AT_FDCWD != fd)
    {
      pathname = "";
      flags = AT_ఖాళీలు_మార్గం;
    }
  లేకపోతే ఒకవేళ (!follow_links)
    {
      flags = AT_SYMLINK_NOFOLLOW;
    }

  ఒకవేళ (dont_sync)
    flags |= AT_STATX_DONT_SYNC;
  లేకపోతే ఒకవేళ (force_sync)
    flags |= AT_STATX_FORCE_SYNC;

  ఒకవేళ (! force_sync)
    flags |= AT_NO_AUTOMOUNT;

  fd = statx (fd, pathname, flags, format_గమ్యం_ముసుగు (format), &stx);
  ఒకవేళ (fd < 0)
    {
      ఒకవేళ (flags & AT_ఖాళీలు_మార్గం)
        error (0, errno, _("cannot stat standard input"));
      లేకపోతే
        error (0, errno, _("cannot statx %s"), quoteaf (filename));
      తిరిగి false;
    }

  ఒకవేళ (S_ISBLK (stx.stx_బహుళకం) || S_ISCHR (stx.stx_బహుళకం))
    format = format2;

  statx_గమ్యం_stat (&stx, &st);
  ఒకవేళ (stx.stx_ముసుగు & STATX_BTIME)
    pa.btime = statx_timestamp_గమ్యం_timespec (stx.stx_btime);

  bool fail = print_it (format, fd, filename, print_stat, &pa);
  తిరిగి ! fail;
}

#లేకపోతే /* USE_STATX */

స్థిర నిర్మాణం timespec
get_birthtime (పూర్ణాంకం fd, అక్షరం స్థిరరాశి *filename, నిర్మాణం stat స్థిరరాశి *st)
{
  నిర్మాణం timespec ts = get_stat_birthtime (st);

# ఒకవేళ HAVE_GETATTRAT
  ఒకవేళ (ts.tv_nsec < 0)
    {
      nvlist_t *response;
      ఒకవేళ ((fd < 0
           ? getattrat (AT_FDCWD, XATTR_VIEW_READWRITE, filename, &response)
           : fgetattr (fd, XATTR_VIEW_READWRITE, &response))
          == 0)
        {
          uint64_t *val;
          uint_t n;
          ఒకవేళ (nvlist_lookup_uint64_array (response, A_CRTIME, &val, &n) == 0
              && 2 <= n
              && val[0] <= TYPE_గరిష్ఠ (time_t)
              && val[1] < 1000000000 * 2 /* for leap seconds */)
            {
              ts.tv_sec = val[0];
              ts.tv_nsec = val[1];
            }
          nvlist_free (response);
        }
    }
# endif

  తిరిగి ts;
}


/* stat the file and print what we find */
NODISCARD
స్థిర bool
do_stat (అక్షరం స్థిరరాశి *filename, అక్షరం స్థిరరాశి *format,
         అక్షరం స్థిరరాశి *format2)
{
  పూర్ణాంకం fd = streq (filename, "-") ? 0 : -1;
  నిర్మాణం stat statbuf;
  నిర్మాణం print_వాదనలు pa;
  pa.st = &statbuf;
  pa.btime = (నిర్మాణం timespec) {.tv_sec = -1, .tv_nsec = -1};

  ఒకవేళ (0 <= fd)
    {
      ఒకవేళ (fstat (fd, &statbuf) != 0)
        {
          error (0, errno, _("cannot stat standard input"));
          తిరిగి false;
        }
    }
  /* We can't use the shorter
     (follow_links?stat:lstat) (filename, &statbug)
     since stat might be a function-like macro.  */
  లేకపోతే ఒకవేళ ((follow_links
            ? stat (filename, &statbuf)
            : lstat (filename, &statbuf)) != 0)
    {
      error (0, errno, _("cannot stat %s"), quoteaf (filename));
      తిరిగి false;
    }

  ఒకవేళ (S_ISBLK (statbuf.st_బహుళకం) || S_ISCHR (statbuf.st_బహుళకం))
    format = format2;

  bool fail = print_it (format, fd, filename, print_stat, &pa);
  తిరిగి ! fail;
}
#అయితే_ముగింపు /* USE_STATX */

/* POSIX requires 'ls' to print file sizes without a sign, even
   when negative.  Be consistent with that.  */

స్థిర uintmax_t
unsigned_file_కొలత (off_t కొలత)
{
  తిరిగి కొలత + (కొలత < 0) * ((uintmax_t) OFF_T_MAX - OFF_T_MIN + 1);
}

/* Print stat info.  Return zero upon success, nonzero upon failure.  */
స్థిర bool
print_stat (అక్షరం *pformat, size_t ఉపసర్గ_len, అక్షరం mod, అక్షరం m,
            MAYBE_వాడని పూర్ణాంకం fd, అక్షరం స్థిరరాశి *filename, శూన్య స్థిరరాశి *దత్తాంశం)
{
  నిర్మాణం print_వాదనలు *parg = (నిర్మాణం print_వాదనలు *) దత్తాంశం;
  నిర్మాణం stat *statbuf = parg->st;
  నిర్మాణం timespec btime = parg->btime;
  నిర్మాణం passwd *pw_ent;
  నిర్మాణం సమూహం *gw_ent;
  bool fail = false;

  ఎంపిక (m)
    {
    స్థితి 'n':
      ఒకవేళ (mod == 'Q')
        {
          initialize_quoting_style ();
          filename = quoteN (filename);
        }
      బహిర్గతం_string (pformat, ఉపసర్గ_len, filename);
      ఆపు;
    స్థితి 'N':
      initialize_quoting_style ();
      బహిర్గతం_string (pformat, ఉపసర్గ_len, quoteN (filename));
      ఒకవేళ (S_ISLNK (statbuf->st_బహుళకం))
        {
          అక్షరం *linkname = areadlink_with_కొలత (filename, statbuf->st_కొలత);
          ఒకవేళ (linkname == ఖాళీ)
            {
              error (0, errno, _("cannot read symbolic link %s"),
                     quoteaf (filename));
              తిరిగి true;
            }
          రాయి (" -> ");
          బహిర్గతం_string (pformat, ఉపసర్గ_len, quoteN (linkname));
          విడుదల (linkname);
        }
      ఆపు;
    స్థితి 'd':
      ఒకవేళ (mod == 'H')
        బహిర్గతం_uint (pformat, ఉపసర్గ_len, major (statbuf->st_dev));
      లేకపోతే ఒకవేళ (mod == 'L')
        బహిర్గతం_uint (pformat, ఉపసర్గ_len, minor (statbuf->st_dev));
      లేకపోతే
        బహిర్గతం_uint (pformat, ఉపసర్గ_len, statbuf->st_dev);
      ఆపు;
    స్థితి 'D':
      బహిర్గతం_uint_x (pformat, ఉపసర్గ_len, statbuf->st_dev);
      ఆపు;
    స్థితి 'i':
      బహిర్గతం_uint (pformat, ఉపసర్గ_len, statbuf->st_ino);
      ఆపు;
    స్థితి 'a':
      బహిర్గతం_uint_o (pformat, ఉపసర్గ_len, statbuf->st_బహుళకం & CHMOD_బహుళకం_బిట్లు);
      ఆపు;
    స్థితి 'A':
      బహిర్గతం_string (pformat, ఉపసర్గ_len, human_access (statbuf));
      ఆపు;
    స్థితి 'f':
      బహిర్గతం_uint_x (pformat, ఉపసర్గ_len, statbuf->st_బహుళకం);
      ఆపు;
    స్థితి 'F':
      బహిర్గతం_string (pformat, ఉపసర్గ_len, file_type (statbuf));
      ఆపు;
    స్థితి 'h':
      బహిర్గతం_uint (pformat, ఉపసర్గ_len, statbuf->st_nlink);
      ఆపు;
    స్థితి 'u':
      బహిర్గతం_uint (pformat, ఉపసర్గ_len, statbuf->st_uid);
      ఆపు;
    స్థితి 'U':
      pw_ent = getpwuid (statbuf->st_uid);
      బహిర్గతం_string (pformat, ఉపసర్గ_len,
                  pw_ent ? pw_ent->pw_name : "UNKNOWN");
      ఆపు;
    స్థితి 'g':
      బహిర్గతం_uint (pformat, ఉపసర్గ_len, statbuf->st_gid);
      ఆపు;
    స్థితి 'G':
      gw_ent = getgrgid (statbuf->st_gid);
      బహిర్గతం_string (pformat, ఉపసర్గ_len,
                  gw_ent ? gw_ent->gr_name : "UNKNOWN");
      ఆపు;
    స్థితి 'm':
      fail |= బహిర్గతం_mount_బిందువు (filename, pformat, ఉపసర్గ_len, statbuf);
      ఆపు;
    స్థితి 's':
      బహిర్గతం_uint (pformat, ఉపసర్గ_len, unsigned_file_కొలత (statbuf->st_కొలత));
      ఆపు;
    స్థితి 'r':
      ఒకవేళ (mod == 'H')
        బహిర్గతం_uint (pformat, ఉపసర్గ_len, major (statbuf->st_rdev));
      లేకపోతే ఒకవేళ (mod == 'L')
        బహిర్గతం_uint (pformat, ఉపసర్గ_len, minor (statbuf->st_rdev));
      లేకపోతే
        బహిర్గతం_uint (pformat, ఉపసర్గ_len, statbuf->st_rdev);
      ఆపు;
    స్థితి 'R':
      బహిర్గతం_uint_x (pformat, ఉపసర్గ_len, statbuf->st_rdev);
      ఆపు;
    స్థితి 't':
      బహిర్గతం_uint_x (pformat, ఉపసర్గ_len, major (statbuf->st_rdev));
      ఆపు;
    స్థితి 'T':
      బహిర్గతం_uint_x (pformat, ఉపసర్గ_len, minor (statbuf->st_rdev));
      ఆపు;
    స్థితి 'B':
      బహిర్గతం_uint (pformat, ఉపసర్గ_len, ST_NBLOCKSIZE);
      ఆపు;
    స్థితి 'b':
      బహిర్గతం_uint (pformat, ఉపసర్గ_len, STP_NBLOCKS (statbuf));
      ఆపు;
    స్థితి 'o':
      బహిర్గతం_uint (pformat, ఉపసర్గ_len, STP_BLKSIZE (statbuf));
      ఆపు;
    స్థితి 'w':
      {
#ఒకవేళ ! USE_STATX
        btime = get_birthtime (fd, filename, statbuf);
#అయితే_ముగింపు
        ఒకవేళ (btime.tv_nsec < 0)
          బహిర్గతం_string (pformat, ఉపసర్గ_len, "-");
        లేకపోతే
          బహిర్గతం_string (pformat, ఉపసర్గ_len, human_సమయం (btime));
      }
      ఆపు;
    స్థితి 'W':
      {
#ఒకవేళ ! USE_STATX
        btime = get_birthtime (fd, filename, statbuf);
#అయితే_ముగింపు
        బహిర్గతం_epoch_sec (pformat, ఉపసర్గ_len, neg_గమ్యం_zero (btime));
      }
      ఆపు;
    స్థితి 'x':
      బహిర్గతం_string (pformat, ఉపసర్గ_len, human_సమయం (get_stat_atime (statbuf)));
      ఆపు;
    స్థితి 'X':
      బహిర్గతం_epoch_sec (pformat, ఉపసర్గ_len, get_stat_atime (statbuf));
      ఆపు;
    స్థితి 'y':
      బహిర్గతం_string (pformat, ఉపసర్గ_len, human_సమయం (get_stat_mtime (statbuf)));
      ఆపు;
    స్థితి 'Y':
      బహిర్గతం_epoch_sec (pformat, ఉపసర్గ_len, get_stat_mtime (statbuf));
      ఆపు;
    స్థితి 'z':
      బహిర్గతం_string (pformat, ఉపసర్గ_len, human_సమయం (get_stat_ctime (statbuf)));
      ఆపు;
    స్థితి 'Z':
      బహిర్గతం_epoch_sec (pformat, ఉపసర్గ_len, get_stat_ctime (statbuf));
      ఆపు;
    స్థితి 'C':
      fail |= బహిర్గతం_file_context (pformat, ఉపసర్గ_len, filename);
      ఆపు;
    అప్రమేయ:
      fputc ('?', stdout);
      ఆపు;
    }
  తిరిగి fail;
}

/* Return an allocated format string in static storage that
   corresponds to whether FS and TERSE options were declared.  */
స్థిర అక్షరం *
default_format (bool fs, bool terse, bool device)
{
  అక్షరం *format;
  ఒకవేళ (fs)
    {
      ఒకవేళ (terse)
        format = xstrdup (fmt_terse_fs);
      లేకపోతే
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
  లేకపోతే /* ! fs */
    {
      ఒకవేళ (terse)
        {
          ఒకవేళ (0 < is_selinux_enabled ())
            format = xstrdup (fmt_terse_selinux);
          లేకపోతే
            format = xstrdup (fmt_terse_regular);
        }
      లేకపోతే
        {
          అక్షరం *తాత్కాలిక;
          /* TRANSLATORS: This string uses format specifiers from
             'stat --help' without --file-system, and NOT from printf.  */
          format = xstrdup (_("\
  File: %N\n\
  కొలత_3: %-10s\t_ఖండాలు: %-10b IO ఖండం_3: %-6o %F\n\
"));

          తాత్కాలిక = format;
          ఒకవేళ (device)
            {
              /* TRANSLATORS: This string uses format specifiers from
                 'stat --help' without --file-system, and NOT from printf.  */
              format = xasprintf ("%s%s", format, _("\
" "Device: %Hd,%Ld\tInode: %-10i  Links: %-5h Device type: %Hr,%Lr\n\
"));
            }
          లేకపోతే
            {
              /* TRANSLATORS: This string uses format specifiers from
                 'stat --help' without --file-system, and NOT from printf.  */
              format = xasprintf ("%s%s", format, _("\
" "Device: %Hd,%Ld\tInode: %-10i  Links: %h\n\
"));
            }
          విడుదల (తాత్కాలిక);

          తాత్కాలిక = format;
          /* TRANSLATORS: This string uses format specifiers from
             'stat --help' without --file-system, and NOT from printf.  */
          format = xasprintf ("%s%s", format, _("\
" "Access: (%04a/%10.10A)  Uid: (%5u/%8U)   Gid: (%5g/%8G)\n\
"));
          విడుదల (తాత్కాలిక);

          ఒకవేళ (0 < is_selinux_enabled ())
            {
              తాత్కాలిక = format;
              /* TRANSLATORS: This string uses format specifiers from
                 'stat --help' without --file-system, and NOT from printf.  */
              format = xasprintf ("%s%s", format, _("Context: %C\n"));
              విడుదల (తాత్కాలిక);
            }

          తాత్కాలిక = format;
          format = xasprintf ("%s%s", format,
                              /* TRANSLATORS: This string uses format specifiers
                                 from 'stat --help' without --file-system, and
                                 NOT from printf.  */
                              _("Access: %x\n"
                                "Modify: %y\n"
                                "Change: %z\n"
                                " Birth: %w\n"));
          విడుదల (తాత్కాలిక);
        }
    }
  తిరిగి format;
}

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("Usage: %s [OPTION]... FILE...\n"), కార్యక్రమం_name);
      fputs (_("\
Display file or file system స్థితి_2.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -L, --dereference\n\
         follow links\n\
"));
      oputs (_("\
  -f, --file-system\n\
         display file system స్థితి_2 instead యొక్క file స్థితి_2\n\
"));
      oputs (_("\
      --cached=బహుళకం\n\
         specify how గమ్యం use cached లక్షణాలు;\n\
         useful on remote file systems. See బహుళకం క్రింద\n\
"));
      oputs (_("\
  -c, --format=FORMAT\n\
         use the specified FORMAT instead యొక్క the అప్రమేయ;\n\
         output a newline after each use యొక్క FORMAT\n\
"));
      oputs (_("\
      --రాయి=FORMAT\n\
         like --format, but interpret backslash escapes,\n\
         and చేయి not output a mandatory trailing newline;\n\
         ఒకవేళ you want a newline, include \\n లోపల FORMAT\n\
"));
      oputs (_("\
  -t, --terse\n\
         print the information లోపల terse form\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);

      fputs (_("\n\
The బహుళకం వాదం యొక్క --cached can be: always, never, or అప్రమేయ.\n\
'always' will use cached లక్షణాలు ఒకవేళ అందుబాటు, అయితే\n\
'never' will try గమ్యం synchronize with the latest లక్షణాలు, and\n\
'default' will leave it పైకి గమ్యం the underlying file system.\n\
"), stdout);

      fputs (_("\n\
The valid format sequences కొరకు files (without --file-system):\n\
\n\
  %a   permission బిట్లు_2 లోపల octal (see '#' and '0' రాయి flags)\n\
  %A   permission బిట్లు_2 and file type లోపల human readable form\n\
  %b   సంఖ్య యొక్క ఖండాలు allocated (see %B)\n\
  %B   the కొలత లోపల బైట్లు యొక్క each ఖండం reported by %b\n\
  %C   SELinux security context string\n\
"), stdout);
      fputs (_("\
  %d   device సంఖ్య లోపల decimal (st_dev)\n\
  %D   device సంఖ్య లోపల hex (st_dev)\n\
  %Hd  major device సంఖ్య లోపల decimal\n\
  %Ld  minor device సంఖ్య లోపల decimal\n\
  %f   raw బహుళకం_2 లోపల hex\n\
  %F   file type\n\
  %g   సమూహం ID యొక్క owner\n\
  %G   సమూహం name యొక్క owner\n\
"), stdout);
      fputs (_("\
  %h   సంఖ్య యొక్క hard links\n\
  %i   inode సంఖ్య\n\
  %m   mount బిందువు\n\
  %n   file name\n\
  %Qn  quoted file name\n\
  %N   quoted file name with dereference ఒకవేళ symbolic link\n\
  %o   optimal I/O transfer కొలత hint\n\
  %s   మొత్తం కొలత, లోపల బైట్లు\n\
  %r   device type లోపల decimal (st_rdev)\n\
  %R   device type లోపల hex (st_rdev)\n\
  %Hr  major device type లోపల decimal, కొరకు character/ఖండం device special files\n\
  %Lr  minor device type లోపల decimal, కొరకు character/ఖండం device special files\n\
  %t   major device type లోపల hex, కొరకు character/ఖండం device special files\n\
  %T   minor device type లోపల hex, కొరకు character/ఖండం device special files\n\
"), stdout);
      fputs (_("\
  %u   user ID యొక్క owner\n\
  %U   user name యొక్క owner\n\
  %w   సమయం యొక్క file birth, human-readable; - ఒకవేళ unknown\n\
  %W   సమయం యొక్క file birth, seconds since Epoch; 0 ఒకవేళ unknown\n\
  %x   సమయం యొక్క చివరి access, human-readable\n\
  %X   సమయం యొక్క చివరి access, seconds since Epoch\n\
  %y   సమయం యొక్క చివరి దత్తాంశం modification, human-readable\n\
  %Y   సమయం యొక్క చివరి దత్తాంశం modification, seconds since Epoch\n\
  %z   సమయం యొక్క చివరి స్థితి_2 change, human-readable\n\
  %Z   సమయం యొక్క చివరి స్థితి_2 change, seconds since Epoch\n\
\n\
"), stdout);

      fputs (_("\
Valid format sequences కొరకు file systems:\n\
\n\
  %a   విడుదల ఖండాలు అందుబాటు గమ్యం non-superuser\n\
  %b   మొత్తం దత్తాంశం ఖండాలు లోపల file system\n\
  %c   మొత్తం file nodes లోపల file system\n\
  %d   విడుదల file nodes లోపల file system\n\
  %f   విడుదల ఖండాలు లోపల file system\n\
"), stdout);
      fputs (_("\
  %i   file system ID లోపల hex\n\
  %l   గరిష్ఠ నిడివి యొక్క filenames\n\
  %n   file name\n\
  %Qn  quoted file name\n\
  %s   ఖండం కొలత (కొరకు faster transfers)\n\
  %S   fundamental ఖండం కొలత (కొరకు ఖండం counts)\n\
  %t   file system type లోపల hex\n\
  %T   file system type లోపల human readable form\n\
"), stdout);

      రాయి (_("\n\
--terse is equivalent గమ్యం the following FORMAT:\n\
    %s\
"),
#ఒకవేళ HAVE_SELINUX_SELINUX_H
              fmt_terse_selinux
#లేకపోతే
              fmt_terse_regular
#అయితే_ముగింపు
              );

        రాయి (_("\
--terse --file-system is equivalent గమ్యం the following FORMAT:\n\
    %s\
"), fmt_terse_fs);

      రాయి (USAGE_BUILTIN_WARNING, కార్యక్రమం_NAME);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం *argv[])
{
  పూర్ణాంకం c;
  bool fs = false;
  bool terse = false;
  అక్షరం *format = ఖాళీ;
  అక్షరం *format2;
  bool ok = true;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  నిర్మాణం lconv స్థిరరాశి *locale = localeconv ();
  decimal_బిందువు = (locale->decimal_బిందువు[0] ? locale->decimal_బిందువు : ".");
  decimal_బిందువు_len = అక్షరశ్రేణి_పొడవు (decimal_బిందువు);

  atexit (close_stdout);

  అయితే ((c = getopt_long (argc, argv, "c:fLt", long_options, ఖాళీ)) != -1)
    {
      ఎంపిక (c)
        {
        స్థితి PRINTF_OPTION:
          format = optarg;
          interpret_backslash_escapes = true;
          trailing_delim = "";
          ఆపు;

        స్థితి 'c':
          format = optarg;
          interpret_backslash_escapes = false;
          trailing_delim = "\n";
          ఆపు;

        స్థితి 'L':
          follow_links = true;
          ఆపు;

        స్థితి 'f':
          fs = true;
          ఆపు;

        స్థితి 't':
          terse = true;
          ఆపు;

        స్థితి 0:
          ఎంపిక (XARGMATCH ("--cached", optarg, cached_వాదనలు, cached_modes))
            {
              స్థితి cached_never:
                force_sync = true;
                dont_sync = false;
                ఆపు;
              స్థితి cached_always:
                force_sync = false;
                dont_sync = true;
                ఆపు;
              స్థితి cached_default:
                force_sync = false;
                dont_sync = false;
            }
          ఆపు;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);

        అప్రమేయ:
          usage (EXIT_FAILURE);
        }
    }

  ఒకవేళ (argc == optind)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  ఒకవేళ (format)
    {
      format2 = format;
    }
  లేకపోతే
    {
      format = default_format (fs, terse, /* device= */ false);
      format2 = default_format (fs, terse, /* device= */ true);
    }

  కొరకు (పూర్ణాంకం i = optind; i < argc; i++)
    ok &= (fs
           ? do_statfs (argv[i], format)
           : do_stat (argv[i], format, format2));

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
