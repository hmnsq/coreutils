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

#અન્તર્ભાવ <config.h>

/* Keep this conditional in sync with the similar conditional in
   ../m4/stat-prog.m4.  */
#જો ((STAT_STATVFS || STAT_STATVFS64)                                       \
     && (HAVE_STRUCT_STATVFS_F_BASETYPE || HAVE_STRUCT_STATVFS_F_FSTYPENAME \
         || (! HAVE_STRUCT_STATFS_F_FSTYPENAME && HAVE_STRUCT_STATVFS_F_TYPE)))
# define USE_STATVFS 1
#નહીંતર
# define USE_STATVFS 0
#endif

#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <pwd.h>
#અન્તર્ભાવ <grp.h>
#જો USE_STATVFS
# include <sys/statvfs.h>
#અથવાજો HAVE_SYS_VFS_H
# include <sys/vfs.h>
#અથવાજો HAVE_SYS_MOUNT_H && HAVE_SYS_PARAM_H
/* NOTE: freebsd5.0 needs sys/param.h and sys/mount.h for statfs.
   It does have statvfs.h, but shouldn't use it, since it doesn't
   HAVE_STRUCT_STATVFS_F_BASETYPE.  So find a clean way to fix it.  */
/* NetBSD 1.5.2 needs these, for the declaration of struct statfs. */
# include <sys/param.h>
# include <sys/mount.h>
# જો HAVE_NFS_NFS_CLNT_H && HAVE_NFS_VFS_H
/* Ultrix 4.4 needs these for the declaration of struct statfs.  */
#  include <netinet/in.h>
#  include <nfs/nfs_clnt.h>
#  include <nfs/vfs.h>
# endif
#અથવાજો HAVE_OS_H /* BeOS */
# include <fs_info.h>
#endif
#અન્તર્ભાવ <selinux/selinux.h>
#અન્તર્ભાવ <getopt.h>

#અન્તર્ભાવ "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#અન્તર્ભાવ "system.h"

#અન્તર્ભાવ "areadlink.h"
#અન્તર્ભાવ "c-ctype.h"
#અન્તર્ભાવ "file-type.h"
#અન્તર્ભાવ "filemode.h"
#અન્તર્ભાવ "fs.h"
#અન્તર્ભાવ "mountlist.h"
#અન્તર્ભાવ "octhexdigits.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "stat-size.h"
#અન્તર્ભાવ "stat-time.h"
#અન્તર્ભાવ "strftime.h"
#અન્તર્ભાવ "find-mount-point.h"
#અન્તર્ભાવ "xvasprintf.h"
#અન્તર્ભાવ "statx.h"

#જો HAVE_STATX && defined STATX_INO
# define USE_STATX 1
#નહીંતર
# define USE_STATX 0
#endif

#જો USE_STATVFS
# define STRUCT_STATXFS_F_FSID_IS_INTEGER STRUCT_STATVFS_F_FSID_IS_INTEGER
# define HAVE_STRUCT_STATXFS_F_TYPE HAVE_STRUCT_STATVFS_F_TYPE
# જો HAVE_STRUCT_STATVFS_F_NAMEMAX
#  define SB_F_NAMEMAX(S) ((S)->f_namemax)
# endif
# જો ! STAT_STATVFS && STAT_STATVFS64
#  define STRUCT_STATVFS સંરચના statvfs64
#  define STATFS statvfs64
# નહીંતર
#  define STRUCT_STATVFS સંરચના statvfs
#  define STATFS statvfs
# endif
# define STATFS_FRSIZE(S) ((S)->f_frsize)
#નહીંતર
# define HAVE_STRUCT_STATXFS_F_TYPE HAVE_STRUCT_STATFS_F_TYPE
# જો HAVE_STRUCT_STATFS_F_NAMELEN
#  define SB_F_NAMEMAX(S) ((S)->f_namelen)
# elif HAVE_STRUCT_STATFS_F_NAMEMAX
#  define SB_F_NAMEMAX(S) ((S)->f_namemax)
# endif
# define STATFS statfs
# જો HAVE_OS_H /* BeOS */
/* BeOS has a statvfs function, but it does not return sensible values
   for f_files, f_ffree and f_favail, and lacks f_type, f_basetype and
   f_fstypename.  Use 'struct fs_info' instead.  */
NODISCARD
સ્થિર પૂર્ણાંક
statfs (અક્ષર અચલ *filename, સંરચના fs_info *buf)
{
  dev_t device = dev_for_path (filename);
  જો (device < 0)
    {
      errno = (device == B_ENTRY_NOT_FOUND ? ENOENT
               : device == B_BAD_કિંમત ? EINVAL
               : device == B_NAME_TOO_LONG ? ENAMETOOLONG
               : device == B_NO_MEMORY ? ENOMEM
               : device == B_FILE_ERROR ? EIO
               : 0);
      પાછા -1;
    }
  /* If successful, buf->dev will be == device.  */
  પાછા fs_stat_dev (device, buf);
}
#  define f_fsid dev
#  define f_blocks total_blocks
#  define f_bfree free_blocks
#  define f_bavail free_blocks
#  define f_bsize io_size
#  define f_files total_nodes
#  define f_ffree free_nodes
#  define STRUCT_STATVFS સંરચના fs_info
#  define STRUCT_STATXFS_F_FSID_IS_INTEGER true
#  define STATFS_FRSIZE(S) ((S)->block_size)
# નહીંતર
#  define STRUCT_STATVFS સંરચના statfs
#  define STRUCT_STATXFS_F_FSID_IS_INTEGER STRUCT_STATFS_F_FSID_IS_INTEGER
#  જો HAVE_STRUCT_STATFS_F_FRSIZE
#   define STATFS_FRSIZE(S) ((S)->f_frsize)
#  નહીંતર
#   define STATFS_FRSIZE(S) 0
#  endif
# endif
#endif

#ifdef SB_F_NAMEMAX
# define OUT_NAMEMAX out_uint
#નહીંતર
/* Depending on whether statvfs or statfs is used,
   neither f_namemax or f_namelen may be available.  */
# define SB_F_NAMEMAX(S) "?"
# define OUT_NAMEMAX out_string
#endif

#જો HAVE_STRUCT_STATVFS_F_BASETYPE
# define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME f_basetype
#નહીંતર
# જો HAVE_STRUCT_STATVFS_F_FSTYPENAME || HAVE_STRUCT_STATFS_F_FSTYPENAME
#  define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME f_fstypename
# elif HAVE_OS_H /* BeOS */
#  define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME fsh_name
# endif
#endif

#જો HAVE_GETATTRAT
# include <attr.h>
# include <sys/nvpair.h>
#endif

સ્થિર અક્ષર અચલ digits[] = "0123456789";

/* Flags that are portable for use in printf, for at least one
   conversion specifier; make_format removes non-portable flags as
   needed for particular specifiers.  The glibc 2.2 extension "I" is
   listed here; it is removed by make_format because it has undefined
   behavior elsewhere and because it is incompatible with
   out_epoch_sec.  */
સ્થિર અક્ષર અચલ printf_flags[] = "'-+ #0I";

/* Formats for the --terse option.  */
સ્થિર અક્ષર અચલ fmt_terse_fs[] = "%Qn %i %l %t %s %S %b %f %a %c %d\n";
સ્થિર અક્ષર અચલ fmt_terse_regular[] = "%Qn %s %b %f %u %g %D %i %h %t %T"
                                        " %X %Y %Z %W %o\n";
સ્થિર અક્ષર અચલ fmt_terse_selinux[] = "%Qn %s %b %f %u %g %D %i %h %t %T"
                                        " %X %Y %Z %W %o %C\n";

#define PROGRAM_NAME "stat"

#define AUTHORS proper_name ("Michael Meskes")

ગણના
{
  PRINTF_OPTION = CHAR_MAX + 1
};

ગણના cached_mode
{
  cached_default,
  cached_never,
  cached_always
};

સ્થિર અક્ષર અચલ *અચલ cached_args[] =
{
  "default", "never", "always", ખાલી
};

સ્થિર ગણના cached_mode અચલ cached_modes[] =
{
  cached_default, cached_never, cached_always
};

સ્થિર સંરચના option અચલ long_options[] =
{
  {"dereference", no_argument, ખાલી, 'L'},
  {"file-system", no_argument, ખાલી, 'f'},
  {"format", required_argument, ખાલી, 'c'},
  {"printf", required_argument, ખાલી, PRINTF_OPTION},
  {"terse", no_argument, ખાલી, 't'},
  {"cached", required_argument, ખાલી, 0},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ખાલી, 0, ખાલી, 0}
};

/* Whether to follow symbolic links;  True for --dereference (-L).  */
સ્થિર bool follow_links;

/* Whether to interpret backslash-escape sequences.
   True for --printf=FMT, not for --format=FMT (-c).  */
સ્થિર bool interpret_backslash_escapes;

/* The trailing delimiter string:
   "" for --printf=FMT, "\n" for --format=FMT (-c).  */
સ્થિર અક્ષર અચલ *trailing_delim = "";

/* The representation of the decimal point in the current locale.  */
સ્થિર અક્ષર અચલ *decimal_point;
સ્થિર size_t decimal_point_len;

સ્થિર bool
print_stat (અક્ષર *pformat, size_t prefix_len, અક્ષર mod, અક્ષર m,
            પૂર્ણાંક fd, અક્ષર અચલ *filename, શૂન્ય અચલ *data);

/* Return the type of the specified file system.
   Some systems have statfvs.f_basetype[FSTYPSZ] (AIX, HP-UX, and Solaris).
   Others have statvfs.f_fstypename[_VFS_NAMELEN] (NetBSD 3.0).
   Others have statfs.f_fstypename[MFSNAMELEN] (NetBSD 1.5.2).
   Still others have neither and have to get by with f_type (GNU/Linux).
   But f_type may only exist in statfs (Cygwin).  */
NODISCARD
સ્થિર અક્ષર અચલ *
human_fstype (STRUCT_STATVFS અચલ *statfsbuf)
{
#ifdef STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME
  પાછા statfsbuf->STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME;
#નહીંતર
  પસંદગી (statfsbuf->f_type)
    {
# જો defined __linux__ || defined __ANDROID__

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

    સ્થિતિ S_MAGIC_AAFS: /* 0x5A3C69F0 local */
      પાછા "aafs";
    સ્થિતિ S_MAGIC_ACFS: /* 0x61636673 remote */
      પાછા "acfs";
    સ્થિતિ S_MAGIC_ADFS: /* 0xADF5 local */
      પાછા "adfs";
    સ્થિતિ S_MAGIC_AFFS: /* 0xADFF local */
      પાછા "affs";
    સ્થિતિ S_MAGIC_AFS: /* 0x5346414F remote */
      પાછા "afs";
    સ્થિતિ S_MAGIC_ANON_INODE_FS: /* 0x09041934 local */
      પાછા "anon-inode FS";
    સ્થિતિ S_MAGIC_AUFS: /* 0x61756673 remote */
      /* FIXME: change syntax or add an optional attribute like "inotify:no".
         The above is labeled as "remote" so that tail always uses polling,
         but this isn't really a remote file system type.  */
      પાછા "aufs";
    સ્થિતિ S_MAGIC_AUTOFS: /* 0x0187 local */
      પાછા "autofs";
    સ્થિતિ S_MAGIC_BALLOON_KVM: /* 0x13661366 local */
      પાછા "balloon-kvm-fs";
    સ્થિતિ S_MAGIC_BCACHEFS: /* 0xCA451A4E local */
      પાછા "bcachefs";
    સ્થિતિ S_MAGIC_BEFS: /* 0x42465331 local */
      પાછા "befs";
    સ્થિતિ S_MAGIC_BDEVFS: /* 0x62646576 local */
      પાછા "bdevfs";
    સ્થિતિ S_MAGIC_BFS: /* 0x1BADFACE local */
      પાછા "bfs";
    સ્થિતિ S_MAGIC_BINDERFS: /* 0x6C6F6F70 local */
      પાછા "binderfs";
    સ્થિતિ S_MAGIC_BPF_FS: /* 0xCAFE4A11 local */
      પાછા "bpf_fs";
    સ્થિતિ S_MAGIC_BINFMTFS: /* 0x42494E4D local */
      પાછા "binfmt_misc";
    સ્થિતિ S_MAGIC_BTRFS: /* 0x9123683E local */
      પાછા "btrfs";
    સ્થિતિ S_MAGIC_BTRFS_TEST: /* 0x73727279 local */
      પાછા "btrfs_test";
    સ્થિતિ S_MAGIC_CEPH: /* 0x00C36400 remote */
      પાછા "ceph";
    સ્થિતિ S_MAGIC_CGROUP: /* 0x0027E0EB local */
      પાછા "cgroupfs";
    સ્થિતિ S_MAGIC_CGROUP2: /* 0x63677270 local */
      પાછા "cgroup2fs";
    સ્થિતિ S_MAGIC_CIFS: /* 0xFF534D42 remote */
      પાછા "cifs";
    સ્થિતિ S_MAGIC_CODA: /* 0x73757245 remote */
      પાછા "coda";
    સ્થિતિ S_MAGIC_COH: /* 0x012FF7B7 local */
      પાછા "coh";
    સ્થિતિ S_MAGIC_CONFIGFS: /* 0x62656570 local */
      પાછા "configfs";
    સ્થિતિ S_MAGIC_CRAMFS: /* 0x28CD3D45 local */
      પાછા "cramfs";
    સ્થિતિ S_MAGIC_CRAMFS_WEND: /* 0x453DCD28 local */
      પાછા "cramfs-wend";
    સ્થિતિ S_MAGIC_DAXFS: /* 0x64646178 local */
      પાછા "daxfs";
    સ્થિતિ S_MAGIC_DEBUGFS: /* 0x64626720 local */
      પાછા "debugfs";
    સ્થિતિ S_MAGIC_DEVFS: /* 0x1373 local */
      પાછા "devfs";
    સ્થિતિ S_MAGIC_DEVMEM: /* 0x454D444D local */
      પાછા "devmem";
    સ્થિતિ S_MAGIC_DEVPTS: /* 0x1CD1 local */
      પાછા "devpts";
    સ્થિતિ S_MAGIC_DMA_BUF: /* 0x444D4142 local */
      પાછા "dma-buf-fs";
    સ્થિતિ S_MAGIC_ECRYPTFS: /* 0xF15F local */
      પાછા "ecryptfs";
    સ્થિતિ S_MAGIC_EFIVARFS: /* 0xDE5E81E4 local */
      પાછા "efivarfs";
    સ્થિતિ S_MAGIC_EFS: /* 0x00414A53 local */
      પાછા "efs";
    સ્થિતિ S_MAGIC_EROFS_V1: /* 0xE0F5E1E2 local */
      પાછા "erofs";
    સ્થિતિ S_MAGIC_EXFAT: /* 0x2011BAB0 local */
      પાછા "exfat";
    સ્થિતિ S_MAGIC_EXFS: /* 0x45584653 local */
      પાછા "exfs";
    સ્થિતિ S_MAGIC_EXOFS: /* 0x5DF5 local */
      પાછા "exofs";
    સ્થિતિ S_MAGIC_EXT: /* 0x137D local */
      પાછા "ext";
    સ્થિતિ S_MAGIC_EXT2: /* 0xEF53 local */
      પાછા "ext2/ext3";
    સ્થિતિ S_MAGIC_EXT2_OLD: /* 0xEF51 local */
      પાછા "ext2";
    સ્થિતિ S_MAGIC_F2FS: /* 0xF2F52010 local */
      પાછા "f2fs";
    સ્થિતિ S_MAGIC_FAIL_FS: /* 0x4641494C local */
      પાછા "failfs";
    સ્થિતિ S_MAGIC_FAT: /* 0x4006 local */
      પાછા "fat";
    સ્થિતિ S_MAGIC_FHGFS: /* 0x19830326 remote */
      પાછા "fhgfs";
    સ્થિતિ S_MAGIC_FUSE: /* 0x65735546 remote */
      પાછા "fuse";
    સ્થિતિ S_MAGIC_FUSECTL: /* 0x65735543 remote */
      પાછા "fusectl";
    સ્થિતિ S_MAGIC_FUTEXFS: /* 0x0BAD1DEA local */
      પાછા "futexfs";
    સ્થિતિ S_MAGIC_GFS: /* 0x01161970 remote */
      પાછા "gfs/gfs2";
    સ્થિતિ S_MAGIC_GPFS: /* 0x47504653 remote */
      પાછા "gpfs";
    સ્થિતિ S_MAGIC_GUEST_MEMFD: /* 0x474D454D remote */
      પાછા "guest-memfd";
    સ્થિતિ S_MAGIC_HFS: /* 0x4244 local */
      પાછા "hfs";
    સ્થિતિ S_MAGIC_HFS_PLUS: /* 0x482B local */
      પાછા "hfs+";
    સ્થિતિ S_MAGIC_HFS_X: /* 0x4858 local */
      પાછા "hfsx";
    સ્થિતિ S_MAGIC_HOSTFS: /* 0x00C0FFEE local */
      પાછા "hostfs";
    સ્થિતિ S_MAGIC_HPFS: /* 0xF995E849 local */
      પાછા "hpfs";
    સ્થિતિ S_MAGIC_HUGETLBFS: /* 0x958458F6 local */
      પાછા "hugetlbfs";
    સ્થિતિ S_MAGIC_MTD_INODE_FS: /* 0x11307854 local */
      પાછા "inodefs";
    સ્થિતિ S_MAGIC_IBRIX: /* 0x013111A8 remote */
      પાછા "ibrix";
    સ્થિતિ S_MAGIC_INOTIFYFS: /* 0x2BAD1DEA local */
      પાછા "inotifyfs";
    સ્થિતિ S_MAGIC_ISOFS: /* 0x9660 local */
      પાછા "isofs";
    સ્થિતિ S_MAGIC_ISOFS_R_WIN: /* 0x4004 local */
      પાછા "isofs";
    સ્થિતિ S_MAGIC_ISOFS_WIN: /* 0x4000 local */
      પાછા "isofs";
    સ્થિતિ S_MAGIC_JFFS: /* 0x07C0 local */
      પાછા "jffs";
    સ્થિતિ S_MAGIC_JFFS2: /* 0x72B6 local */
      પાછા "jffs2";
    સ્થિતિ S_MAGIC_JFS: /* 0x3153464A local */
      પાછા "jfs";
    સ્થિતિ S_MAGIC_KAFS: /* 0x6B414653 remote */
      પાછા "k-afs";
    સ્થિતિ S_MAGIC_LOGFS: /* 0xC97E8168 local */
      પાછા "logfs";
    સ્થિતિ S_MAGIC_LUSTRE: /* 0x0BD00BD0 remote */
      પાછા "lustre";
    સ્થિતિ S_MAGIC_M1FS: /* 0x5346314D local */
      પાછા "m1fs";
    સ્થિતિ S_MAGIC_MINIX: /* 0x137F local */
      પાછા "minix";
    સ્થિતિ S_MAGIC_MINIX_30: /* 0x138F local */
      પાછા "minix (30 char.)";
    સ્થિતિ S_MAGIC_MINIX_V2: /* 0x2468 local */
      પાછા "minix v2";
    સ્થિતિ S_MAGIC_MINIX_V2_30: /* 0x2478 local */
      પાછા "minix v2 (30 char.)";
    સ્થિતિ S_MAGIC_MINIX_V3: /* 0x4D5A local */
      પાછા "minix3";
    સ્થિતિ S_MAGIC_MQUEUE: /* 0x19800202 local */
      પાછા "mqueue";
    સ્થિતિ S_MAGIC_MSDOS: /* 0x4D44 local */
      પાછા "msdos";
    સ્થિતિ S_MAGIC_NCP: /* 0x564C remote */
      પાછા "novell";
    સ્થિતિ S_MAGIC_NFS: /* 0x6969 remote */
      પાછા "nfs";
    સ્થિતિ S_MAGIC_NFSD: /* 0x6E667364 remote */
      પાછા "nfsd";
    સ્થિતિ S_MAGIC_NILFS: /* 0x3434 local */
      પાછા "nilfs";
    સ્થિતિ S_MAGIC_NSFS: /* 0x6E736673 local */
      પાછા "nsfs";
    સ્થિતિ S_MAGIC_NTFS: /* 0x5346544E local */
      પાછા "ntfs";
    સ્થિતિ S_MAGIC_NULL_FS: /* 0x4E554C4C local */
      પાછા "nullfs";
    સ્થિતિ S_MAGIC_OPENPROM: /* 0x9FA1 local */
      પાછા "openprom";
    સ્થિતિ S_MAGIC_OCFS2: /* 0x7461636F remote */
      પાછા "ocfs2";
    સ્થિતિ S_MAGIC_OVERLAYFS: /* 0x794C7630 remote */
      /* This may overlay remote file systems.
         Also there have been issues reported with inotify and overlayfs,
         so mark as "remote" so that polling is used.  */
      પાછા "overlayfs";
    સ્થિતિ S_MAGIC_PANFS: /* 0xAAD7AAEA remote */
      પાછા "panfs";
    સ્થિતિ S_MAGIC_PID_FS: /* 0x50494446 local */
      પાછા "pidfs";
    સ્થિતિ S_MAGIC_PIPEFS: /* 0x50495045 remote */
      /* FIXME: change syntax or add an optional attribute like "inotify:no".
         pipefs is labeled as "remote" so that tail always polls,
         but it isn't really remote file system type.  */
      પાછા "pipefs";
    સ્થિતિ S_MAGIC_PPC_CMM: /* 0xC7571590 local */
      પાછા "ppc-cmm-fs";
    સ્થિતિ S_MAGIC_PRL_FS: /* 0x7C7C6673 remote */
      પાછા "prl_fs";
    સ્થિતિ S_MAGIC_PROC: /* 0x9FA0 local */
      પાછા "proc";
    સ્થિતિ S_MAGIC_PSTOREFS: /* 0x6165676C local */
      પાછા "pstorefs";
    સ્થિતિ S_MAGIC_QNX4: /* 0x002F local */
      પાછા "qnx4";
    સ્થિતિ S_MAGIC_QNX6: /* 0x68191122 local */
      પાછા "qnx6";
    સ્થિતિ S_MAGIC_RAMFS: /* 0x858458F6 local */
      પાછા "ramfs";
    સ્થિતિ S_MAGIC_RDTGROUP: /* 0x07655821 local */
      પાછા "rdt";
    સ્થિતિ S_MAGIC_REISERFS: /* 0x52654973 local */
      પાછા "reiserfs";
    સ્થિતિ S_MAGIC_ROMFS: /* 0x7275 local */
      પાછા "romfs";
    સ્થિતિ S_MAGIC_RPC_PIPEFS: /* 0x67596969 local */
      પાછા "rpc_pipefs";
    સ્થિતિ S_MAGIC_SDCARDFS: /* 0x5DCA2DF5 local */
      પાછા "sdcardfs";
    સ્થિતિ S_MAGIC_SECRETMEM: /* 0x5345434D local */
      પાછા "secretmem";
    સ્થિતિ S_MAGIC_SECURITYFS: /* 0x73636673 local */
      પાછા "securityfs";
    સ્થિતિ S_MAGIC_SELINUX: /* 0xF97CFF8C local */
      પાછા "selinux";
    સ્થિતિ S_MAGIC_SMACK: /* 0x43415D53 local */
      પાછા "smackfs";
    સ્થિતિ S_MAGIC_SMB: /* 0x517B remote */
      પાછા "smb";
    સ્થિતિ S_MAGIC_SMB2: /* 0xFE534D42 remote */
      પાછા "smb2";
    સ્થિતિ S_MAGIC_SNFS: /* 0xBEEFDEAD remote */
      પાછા "snfs";
    સ્થિતિ S_MAGIC_SOCKFS: /* 0x534F434B local */
      પાછા "sockfs";
    સ્થિતિ S_MAGIC_SQUASHFS: /* 0x73717368 local */
      પાછા "squashfs";
    સ્થિતિ S_MAGIC_SYSFS: /* 0x62656572 local */
      પાછા "sysfs";
    સ્થિતિ S_MAGIC_SYSV2: /* 0x012FF7B6 local */
      પાછા "sysv2";
    સ્થિતિ S_MAGIC_SYSV4: /* 0x012FF7B5 local */
      પાછા "sysv4";
    સ્થિતિ S_MAGIC_TMPFS: /* 0x01021994 local */
      પાછા "tmpfs";
    સ્થિતિ S_MAGIC_TRACEFS: /* 0x74726163 local */
      પાછા "tracefs";
    સ્થિતિ S_MAGIC_UBIFS: /* 0x24051905 local */
      પાછા "ubifs";
    સ્થિતિ S_MAGIC_UDF: /* 0x15013346 local */
      પાછા "udf";
    સ્થિતિ S_MAGIC_UFS: /* 0x00011954 local */
      પાછા "ufs";
    સ્થિતિ S_MAGIC_UFS_BYTESWAPPED: /* 0x54190100 local */
      પાછા "ufs";
    સ્થિતિ S_MAGIC_USBDEVFS: /* 0x9FA2 local */
      પાછા "usbdevfs";
    સ્થિતિ S_MAGIC_V9FS: /* 0x01021997 local */
      પાછા "v9fs";
    સ્થિતિ S_MAGIC_VBOXSF: /* 0x786F4256 remote */
      પાછા "vboxsf";
    સ્થિતિ S_MAGIC_VMHGFS: /* 0xBACBACBC remote */
      પાછા "vmhgfs";
    સ્થિતિ S_MAGIC_VXFS: /* 0xA501FCF5 remote */
      /* Veritas File System can run in single instance or clustered mode,
         so mark as remote to cater for the latter case.  */
      પાછા "vxfs";
    સ્થિતિ S_MAGIC_VZFS: /* 0x565A4653 local */
      પાછા "vzfs";
    સ્થિતિ S_MAGIC_WSLFS: /* 0x53464846 local */
      પાછા "wslfs";
    સ્થિતિ S_MAGIC_XENFS: /* 0xABBA1974 local */
      પાછા "xenfs";
    સ્થિતિ S_MAGIC_XENIX: /* 0x012FF7B4 local */
      પાછા "xenix";
    સ્થિતિ S_MAGIC_XFS: /* 0x58465342 local */
      પાછા "xfs";
    સ્થિતિ S_MAGIC_XIAFS: /* 0x012FD16D local */
      પાછા "xia";
    સ્થિતિ S_MAGIC_Z3FOLD: /* 0x0033 local */
      પાછા "z3fold";
    સ્થિતિ S_MAGIC_ZFS: /* 0x2FC12FC1 local */
      પાછા "zfs";
    સ્થિતિ S_MAGIC_ZONEFS: /* 0x5A4F4653 local */
      પાછા "zonefs";
    સ્થિતિ S_MAGIC_ZSMALLOC: /* 0x58295829 local */
      પાછા "zsmallocfs";


# elif __GNU__
    સ્થિતિ FSTYPE_UFS:
      પાછા "ufs";
    સ્થિતિ FSTYPE_NFS:
      પાછા "nfs";
    સ્થિતિ FSTYPE_GFS:
      પાછા "gfs";
    સ્થિતિ FSTYPE_LFS:
      પાછા "lfs";
    સ્થિતિ FSTYPE_SYSV:
      પાછા "sysv";
    સ્થિતિ FSTYPE_FTP:
      પાછા "ftp";
    સ્થિતિ FSTYPE_TAR:
      પાછા "tar";
    સ્થિતિ FSTYPE_AR:
      પાછા "ar";
    સ્થિતિ FSTYPE_CPIO:
      પાછા "cpio";
    સ્થિતિ FSTYPE_MSLOSS:
      પાછા "msloss";
    સ્થિતિ FSTYPE_CPM:
      પાછા "cpm";
    સ્થિતિ FSTYPE_HFS:
      પાછા "hfs";
    સ્થિતિ FSTYPE_DTFS:
      પાછા "dtfs";
    સ્થિતિ FSTYPE_GRFS:
      પાછા "grfs";
    સ્થિતિ FSTYPE_TERM:
      પાછા "term";
    સ્થિતિ FSTYPE_DEV:
      પાછા "dev";
    સ્થિતિ FSTYPE_PROC:
      પાછા "proc";
    સ્થિતિ FSTYPE_IFSOCK:
      પાછા "ifsock";
    સ્થિતિ FSTYPE_AFS:
      પાછા "afs";
    સ્થિતિ FSTYPE_DFS:
      પાછા "dfs";
    સ્થિતિ FSTYPE_PROC9:
      પાછા "proc9";
    સ્થિતિ FSTYPE_SOCKET:
      પાછા "socket";
    સ્થિતિ FSTYPE_MISC:
      પાછા "misc";
    સ્થિતિ FSTYPE_EXT2FS:
      પાછા "ext2/ext3";
    સ્થિતિ FSTYPE_HTTP:
      પાછા "http";
    સ્થિતિ FSTYPE_MEMFS:
      પાછા "memfs";
    સ્થિતિ FSTYPE_ISO9660:
      પાછા "iso9660";
# endif
    મૂળભૂત:
      {
        unsigned long પૂર્ણાંક type = statfsbuf->f_type;
        સ્થિર અક્ષર buf[કદ "UNKNOWN (0x%lx)" - 3
                        + (કદ type * CHAR_BIT + 3) / 4];
        sprintf (buf, "UNKNOWN (0x%lx)", type);
        પાછા buf;
      }
    }
#endif
}

NODISCARD
સ્થિર અક્ષર *
human_access (સંરચના stat અચલ *statbuf)
{
  સ્થિર અક્ષર modebuf[12];
  filemodestring (statbuf, modebuf);
  modebuf[10] = 0;
  પાછા modebuf;
}

NODISCARD
સ્થિર અક્ષર *
human_time (સંરચના timespec t)
{
  /* STR must be at least INT_BUFSIZE_BOUND (intmax_t) big, either
     because localtime_rz fails, or because the time zone is truly
     outlandish so that %z expands to a long string.  */
  સ્થિર અક્ષર str[INT_BUFSIZE_BOUND (intmax_t)
                  + INT_STRLEN_BOUND (પૂર્ણાંક) /* YYYY */
                  + 1 /* because YYYY might equal INT_MAX + 1900 */
                  + કદ "-MM-DD HH:MM:SS.NNNNNNNNN +"];
  સ્થિર timezone_t tz;
  જો (!tz)
    tz = tzalloc (getenv ("TZ"));
  સંરચના tm tm;
  પૂર્ણાંક ns = t.tv_nsec;
  જો (localtime_rz (tz, &t.tv_sec, &tm))
    nstrftime (str, કદ str, "%Y-%m-%d %H:%M:%S.%N %z", &tm, tz, ns);
  નહીંતર
    {
      અક્ષર secbuf[INT_BUFSIZE_BOUND (intmax_t)];
      sprintf (str, "%s.%09d", timetostr (t.tv_sec, secbuf), ns);
    }
  પાછા str;
}

/* PFORMAT points to a '%' followed by a prefix of a format, all of
   size PREFIX_LEN.  The flags allowed for this format are
   ALLOWED_FLAGS; remove other printf flags from the prefix, then
   append SUFFIX.  */
સ્થિર શૂન્ય
make_format (અક્ષર *pformat, size_t prefix_len, અક્ષર અચલ *allowed_flags,
             અક્ષર અચલ *suffix)
{
  અક્ષર *dst = pformat + 1;
  અક્ષર અચલ *src;
  અક્ષર અચલ *srclim = pformat + prefix_len;
  માટે (src = dst; src < srclim && strchr (printf_flags, *src); src++)
    જો (strchr (allowed_flags, *src))
      *dst++ = *src;
  જ્યારે (src < srclim)
    *dst++ = *src++;
  strcpy (dst, suffix);
}

સ્થિર શૂન્ય
out_string (અક્ષર *pformat, size_t prefix_len, અક્ષર અચલ *arg)
{
  make_format (pformat, prefix_len, "-", "s");
  printf (pformat, arg);
}
સ્થિર પૂર્ણાંક
out_int (અક્ષર *pformat, size_t prefix_len, intmax_t arg)
{
  make_format (pformat, prefix_len, "'-+ 0", "jd");
  પાછા printf (pformat, arg);
}
સ્થિર પૂર્ણાંક
out_uint (અક્ષર *pformat, size_t prefix_len, uintmax_t arg)
{
  make_format (pformat, prefix_len, "'-0", "ju");
  પાછા printf (pformat, arg);
}
સ્થિર શૂન્ય
out_uint_o (અક્ષર *pformat, size_t prefix_len, uintmax_t arg)
{
  make_format (pformat, prefix_len, "-#0", "jo");
  printf (pformat, arg);
}
સ્થિર શૂન્ય
out_uint_x (અક્ષર *pformat, size_t prefix_len, uintmax_t arg)
{
  make_format (pformat, prefix_len, "-#0", "jx");
  printf (pformat, arg);
}
સ્થિર પૂર્ણાંક
out_minus_zero (અક્ષર *pformat, size_t prefix_len)
{
  make_format (pformat, prefix_len, "'-+ 0", ".0f");
  પાછા printf (pformat, -0.25);
}

/* Output the number of seconds since the Epoch, using a format that
   acts like printf's %f format.  */
સ્થિર શૂન્ય
out_epoch_sec (અક્ષર *pformat, size_t prefix_len,
               સંરચના timespec arg)
{
  અક્ષર *dot = memchr (pformat, '.', prefix_len);
  size_t sec_prefix_len = prefix_len;
  પૂર્ણાંક width = 0;
  પૂર્ણાંક precision = 0;
  bool frac_ડાબું_adjust = false;

  જો (dot)
    {
      sec_prefix_len = dot - pformat;
      pformat[prefix_len] = '\0';

      જો (c_isdigit (dot[1]))
        {
          long પૂર્ણાંક lprec = strtol (dot + 1, ખાલી, 10);
          precision = (lprec <= INT_MAX ? lprec : INT_MAX);
        }
      નહીંતર
        {
          precision = 9;
        }

      જો (precision && c_isdigit (dot[-1]))
        {
          /* If a nontrivial width is given, subtract the width of the
             decimal point and PRECISION digits that will be output
             later.  */
          અક્ષર *p = dot;
          *dot = '\0';

          કરો
            --p;
          જ્યારે (c_isdigit (p[-1]));

          long પૂર્ણાંક lwidth = strtol (p, ખાલી, 10);
          width = (lwidth <= INT_MAX ? lwidth : INT_MAX);
          જો (1 < width)
            {
              p += (*p == '0');
              sec_prefix_len = p - pformat;
              પૂર્ણાંક w_d = (decimal_point_len < width
                         ? width - decimal_point_len
                         : 0);
              જો (1 < w_d)
                {
                  પૂર્ણાંક w = w_d - precision;
                  જો (1 < w)
                    {
                      અક્ષર *dst = pformat;
                      માટે (અક્ષર અચલ *src = dst; src < p; src++)
                        {
                          જો (*src == '-')
                            frac_ડાબું_adjust = true;
                          નહીંતર
                            *dst++ = *src;
                        }
                      sec_prefix_len =
                        (dst - pformat
                         + (frac_ડાબું_adjust ? 0 : sprintf (dst, "%d", w)));
                    }
                }
            }
        }
    }

  પૂર્ણાંક ભાજક = 1;
  માટે (પૂર્ણાંક i = precision; i < 9; i++)
    ભાજક *= 10;
  પૂર્ણાંક frac_sec = arg.tv_nsec / ભાજક;
  પૂર્ણાંક int_len;

  જો (TYPE_SIGNED (time_t))
    {
      bool minus_zero = false;
      જો (arg.tv_sec < 0 && arg.tv_nsec != 0)
        {
          પૂર્ણાંક frac_sec_modulus = 1000000000 / ભાજક;
          frac_sec = (frac_sec_modulus - frac_sec
                      - (arg.tv_nsec % ભાજક != 0));
          arg.tv_sec += (frac_sec != 0);
          minus_zero = (arg.tv_sec == 0);
        }
      int_len = (minus_zero
                 ? out_minus_zero (pformat, sec_prefix_len)
                 : out_int (pformat, sec_prefix_len, arg.tv_sec));
    }
  નહીંતર
    int_len = out_uint (pformat, sec_prefix_len, arg.tv_sec);

  જો (precision)
    {
      પૂર્ણાંક prec = (precision < 9 ? precision : 9);
      પૂર્ણાંક trailing_prec = precision - prec;
      પૂર્ણાંક ilen = (int_len < 0 ? 0 : int_len);
      પૂર્ણાંક trailing_width = (ilen < width && decimal_point_len < width - ilen
                            ? width - ilen - decimal_point_len - prec
                            : 0);
      printf ("%s%.*d%-*.*d", decimal_point, prec, frac_sec,
              trailing_width, trailing_prec, 0);
    }
}

/* Print the context information of FILENAME, and return true iff the
   context could not be obtained.  */
NODISCARD
સ્થિર bool
out_file_context (અક્ષર *pformat, size_t prefix_len, અક્ષર અચલ *filename)
{
  અક્ષર *scontext;
  bool fail = false;

  જો ((follow_links
       ? getfilecon (filename, &scontext)
       : lgetfilecon (filename, &scontext)) < 0)
    {
      error (0, errno, _("failed to get security context of %s"),
             quoteaf (filename));
      scontext = ખાલી;
      fail = true;
    }
  strcpy (pformat + prefix_len, "s");
  printf (pformat, (scontext ? scontext : "?"));
  જો (scontext)
    freecon (scontext);
  પાછા fail;
}

/* Set the quoting style once it is needed.  */
સ્થિર શૂન્ય
initialize_quoting_style (શૂન્ય)
{
  સ્થિર bool initialized;
  જો (!initialized)
    {
      initialized = true;
      પૂર્ણાંક qs = getenv_quoting_style ();
      set_quoting_style (ખાલી, 0 <= qs ? qs : shell_escape_quoting_style);
    }
}

/* Print statfs info.  Return zero upon success, nonzero upon failure.  */
NODISCARD
સ્થિર bool
print_statfs (અક્ષર *pformat, size_t prefix_len, MAYBE_UNUSED અક્ષર mod, અક્ષર m,
              MAYBE_UNUSED પૂર્ણાંક fd, અક્ષર અચલ *filename,
              શૂન્ય અચલ *data)
{
  STRUCT_STATVFS અચલ *statfsbuf = data;
  bool fail = false;

  પસંદગી (m)
    {
    સ્થિતિ 'n':
      જો (mod == 'Q')
        {
          initialize_quoting_style ();
          filename = quoteN (filename);
        }
      out_string (pformat, prefix_len, filename);
      તોડો;

    સ્થિતિ 'i':
      {
#જો STRUCT_STATXFS_F_FSID_IS_INTEGER
        uintmax_t fsid = statfsbuf->f_fsid;
#નહીંતર
        પ્રકારવ્યાખ્યા unsigned પૂર્ણાંક fsid_word;
        static_assert (alignof (STRUCT_STATVFS) % alignof (fsid_word) == 0);
        static_assert (offsetof (STRUCT_STATVFS, f_fsid) % alignof (fsid_word)
                       == 0);
        static_assert (કદ statfsbuf->f_fsid % alignof (fsid_word) == 0);
        fsid_word અચલ *p = (fsid_word *) &statfsbuf->f_fsid;

        /* Assume a little-endian word order, as that is compatible
           with glibc's statvfs implementation.  */
        uintmax_t fsid = 0;
        પૂર્ણાંક શબ્દો = કદ statfsbuf->f_fsid / કદ *p;
        માટે (પૂર્ણાંક i = 0; i < શબ્દો && i * કદ *p < કદ fsid; i++)
          {
            uintmax_t u = p[શબ્દો - 1 - i];
            fsid |= u << (i * CHAR_BIT * કદ *p);
          }
#endif
        out_uint_x (pformat, prefix_len, fsid);
      }
      તોડો;

    સ્થિતિ 'l':
      OUT_NAMEMAX (pformat, prefix_len, SB_F_NAMEMAX (statfsbuf));
      તોડો;
    સ્થિતિ 't':
#જો HAVE_STRUCT_STATXFS_F_TYPE
      out_uint_x (pformat, prefix_len, statfsbuf->f_type);
#નહીંતર
      fputc ('?', stdout);
#endif
      તોડો;
    સ્થિતિ 'T':
      out_string (pformat, prefix_len, human_fstype (statfsbuf));
      તોડો;
    સ્થિતિ 'b':
      out_int (pformat, prefix_len, statfsbuf->f_blocks);
      તોડો;
    સ્થિતિ 'f':
      out_int (pformat, prefix_len, statfsbuf->f_bfree);
      તોડો;
    સ્થિતિ 'a':
      out_int (pformat, prefix_len, statfsbuf->f_bavail);
      તોડો;
    સ્થિતિ 's':
      out_uint (pformat, prefix_len, statfsbuf->f_bsize);
      તોડો;
    સ્થિતિ 'S':
      {
        uintmax_t frsize = STATFS_FRSIZE (statfsbuf);
        જો (! frsize)
          frsize = statfsbuf->f_bsize;
        out_uint (pformat, prefix_len, frsize);
      }
      તોડો;
    સ્થિતિ 'c':
      out_uint (pformat, prefix_len, statfsbuf->f_files);
      તોડો;
    સ્થિતિ 'd':
      out_int (pformat, prefix_len, statfsbuf->f_ffree);
      તોડો;
    મૂળભૂત:
      fputc ('?', stdout);
      તોડો;
    }
  પાછા fail;
}

/* Return any bind mounted source for a path.
   The caller should not free the returned buffer.
   Return NULL if no bind mount found.  */
NODISCARD
સ્થિર અક્ષર અચલ *
find_bind_mount (અક્ષર અચલ * name)
{
  અક્ષર અચલ * bind_mount = ખાલી;

  સ્થિર સંરચના mount_entry *mount_list;
  સ્થિર bool tried_mount_list = false;
  જો (!tried_mount_list) /* attempt/warn once per process.  */
    {
      જો (!(mount_list = read_file_system_list (false)))
        error (0, errno, "%s", _("cannot read table of mounted file systems"));
      tried_mount_list = true;
    }

  સંરચના stat name_stats;
  જો (stat (name, &name_stats) != 0)
    પાછા ખાલી;

  માટે (સંરચના mount_entry *me = mount_list; me; me = me->me_next)
    {
      જો (me->me_dummy && me->me_devname[0] == '/'
          && streq (me->me_mountdir, name))
        {
          સંરચના stat dev_stats;

          જો (stat (me->me_devname, &dev_stats) == 0
              && psame_inode (&name_stats, &dev_stats))
            {
              bind_mount = me->me_devname;
              તોડો;
            }
        }
    }

  પાછા bind_mount;
}

/* Print mount point.  Return zero upon success, nonzero upon failure.  */
NODISCARD
સ્થિર bool
out_mount_point (અક્ષર અચલ *filename, અક્ષર *pformat, size_t prefix_len,
                 અચલ સંરચના stat *statp)
{

  અક્ષર અચલ *np = "?", *bp = ખાલી;
  અક્ષર *mp = ખાલી;
  bool fail = true;

  /* Look for bind mounts first.  Note we output the immediate alias,
     rather than further resolving to a base device mount point.  */
  જો (follow_links || !S_ISLNK (statp->st_mode))
    {
      અક્ષર *resolved = canonicalize_file_name (filename);
      જો (!resolved)
        {
          error (0, errno, _("failed to canonicalize %s"), quoteaf (filename));
          જાઓ print_mount_point;
        }
      bp = find_bind_mount (resolved);
      મુક્ત (resolved);
      જો (bp)
        {
          fail = false;
          જાઓ print_mount_point;
        }
    }

  /* If there is no direct bind mount, then navigate
     back up the tree looking for a device change.
     Note we don't detect if any of the directory components
     are bind mounted to the same device, but that's OK
     since we've not directly queried them.  */
  જો ((mp = find_mount_point (filename, statp)))
    {
      /* This dir might be bind mounted to another device,
         so we resolve the bound source in that case also.  */
      bp = find_bind_mount (mp);
      fail = false;
    }

print_mount_point:

  out_string (pformat, prefix_len, bp ? bp : mp ? mp : np);
  મુક્ત (mp);
  પાછા fail;
}

/* Map a TS with negative TS.tv_nsec to {0,0}.  */
સ્થિર અંતઃરેખ સંરચના timespec
neg_to_zero (સંરચના timespec ts)
{
  જો (0 <= ts.tv_nsec)
    પાછા ts;
  સંરચના timespec z = {0};
  પાછા z;
}

/* Output a single-character \ escape.  */

સ્થિર શૂન્ય
print_esc_char (અક્ષર c)
{
  પસંદગી (c)
    {
    સ્થિતિ 'a':			/* Alert. */
      c ='\a';
      તોડો;
    સ્થિતિ 'b':			/* Backspace. */
      c ='\b';
      તોડો;
    સ્થિતિ 'e':			/* Escape. */
      c ='\x1B';
      તોડો;
    સ્થિતિ 'f':			/* Form feed. */
      c ='\f';
      તોડો;
    સ્થિતિ 'n':			/* New line. */
      c ='\n';
      તોડો;
    સ્થિતિ 'r':			/* Carriage return. */
      c ='\r';
      તોડો;
    સ્થિતિ 't':			/* Horizontal tab. */
      c ='\t';
      તોડો;
    સ્થિતિ 'v':			/* Vertical tab. */
      c ='\v';
      તોડો;
    સ્થિતિ '"':
    સ્થિતિ '\\':
      તોડો;
    મૂળભૂત:
      error (0, 0, _("warning: unrecognized escape '\\%c'"), c);
      તોડો;
    }
  putchar (c);
}

ATTRIBUTE_PURE
સ્થિર size_t
format_code_offset (અક્ષર અચલ *directive)
{
  size_t len = strspn (directive + 1, printf_flags);
  અક્ષર અચલ *fmt_char = directive + len + 1;
  fmt_char += strspn (fmt_char, digits);
  જો (*fmt_char == '.')
    fmt_char += 1 + strspn (fmt_char + 1, digits);
  પાછા fmt_char - directive;
}

/* Print the information specified by the format string, FORMAT,
   calling PRINT_FUNC for each %-directive encountered.
   Return zero upon success, nonzero upon failure.  */
NODISCARD
સ્થિર bool
print_it (અક્ષર અચલ *format, પૂર્ણાંક fd, અક્ષર અચલ *filename,
          bool (*print_func) (અક્ષર *, size_t, અક્ષર, અક્ષર,
                              પૂર્ણાંક, અક્ષર અચલ *, શૂન્ય અચલ *),
          શૂન્ય અચલ *data)
{
  bool fail = false;

  /* Add 2 to accommodate our conversion of the stat '%s' format string
     to the longer printf '%llu' one.  */
  ગણના
    {
      MAX_ADDITIONAL_BYTES =
        (MAX (કદ "jd",
              MAX (કદ "jo", MAX (કદ "ju", કદ "jx")))
         - 1)
    };
  size_t n_alloc = strlen (format) + MAX_ADDITIONAL_BYTES + 1;
  અક્ષર *dest = xmalloc (n_alloc);
  માટે (અક્ષર અચલ *b = format; *b; b++)
    {
      પસંદગી (*b)
        {
        સ્થિતિ '%':
          {
            size_t len = format_code_offset (b);
            અક્ષર fmt_char = *(b + len);
            અક્ષર mod_char = 0;
            memcpy (dest, b, len);
            b += len;

            પસંદગી (fmt_char)
              {
              સ્થિતિ '\0':
                --b;
                FALLTHROUGH;
              સ્થિતિ '%':
                જો (1 < len)
                  {
                    dest[len] = fmt_char;
                    dest[len + 1] = '\0';
                    error (EXIT_FAILURE, 0, _("%s: invalid directive"),
                           quote (dest));
                  }
                putchar ('%');
                તોડો;
              સ્થિતિ 'H':
              સ્થિતિ 'L':
              સ્થિતિ 'Q':
                mod_char = fmt_char;
                fmt_char = *(b + 1);
                જો ((mod_char == 'Q' && fmt_char == 'n')
                    || (print_func == print_stat
                        && (mod_char == 'H' || mod_char == 'L')
                        && (fmt_char == 'd' || fmt_char == 'r')))
                  {
                    b++;
                  }
                નહીંતર
                  {
                    fmt_char = mod_char;
                    mod_char = 0;
                  }
                FALLTHROUGH;
              મૂળભૂત:
                fail |= print_func (dest, len, mod_char, fmt_char,
                                    fd, filename, data);
                તોડો;
              }
            તોડો;
          }

        સ્થિતિ '\\':
          જો ( ! interpret_backslash_escapes)
            {
              putchar ('\\');
              તોડો;
            }
          ++b;
          જો (isoct (*b))
            {
              પૂર્ણાંક esc_કિંમત = fromoct (*b);
              પૂર્ણાંક esc_length = 1;	/* number of octal digits */
              માટે (++b; esc_length < 3 && isoct (*b);
                   ++esc_length, ++b)
                {
                  esc_કિંમત = esc_કિંમત * 8 + fromoct (*b);
                }
              putchar (esc_કિંમત);
              --b;
            }
          નહીંતર જો (*b == 'x' && c_isxdigit (b[1]))
            {
              પૂર્ણાંક esc_કિંમત = fromhex (b[1]);	/* Value of \xhh escape. */
              /* A hexadecimal \xhh escape sequence must have
                 1 or 2 hex. digits.  */
              ++b;
              જો (c_isxdigit (b[1]))
                {
                  ++b;
                  esc_કિંમત = esc_કિંમત * 16 + fromhex (*b);
                }
              putchar (esc_કિંમત);
            }
          નહીંતર જો (*b == '\0')
            {
              error (0, 0, _("warning: backslash at end of format"));
              putchar ('\\');
              /* Arrange to exit the loop.  */
              --b;
            }
          નહીંતર
            {
              print_esc_char (*b);
            }
          તોડો;

        મૂળભૂત:
          putchar (*b);
          તોડો;
        }
    }
  મુક્ત (dest);

  fputs (trailing_delim, stdout);

  પાછા fail;
}

/* Stat the file system and print what we find.  */
NODISCARD
સ્થિર bool
do_statfs (અક્ષર અચલ *filename, અક્ષર અચલ *format)
{
  STRUCT_STATVFS statfsbuf;

  જો (streq (filename, "-"))
    {
      error (0, 0, _("using %s to denote standard input does not work"
                     " in file system mode"), quoteaf (filename));
      પાછા false;
    }

  જો (STATFS (filename, &statfsbuf) != 0)
    {
      error (0, errno, _("cannot read file system information for %s"),
             quoteaf (filename));
      પાછા false;
    }

  bool fail = print_it (format, -1, filename, print_statfs, &statfsbuf);
  પાછા ! fail;
}

સંરચના print_args {
  સંરચના stat *st;
  સંરચના timespec btime;
};

/* Ask statx to avoid syncing? */
સ્થિર bool dont_sync;

/* Ask statx to force sync? */
સ્થિર bool force_sync;

#જો USE_STATX
સ્થિર unsigned પૂર્ણાંક
fmt_to_mask (અક્ષર fmt)
{
  પસંદગી (fmt)
    {
    સ્થિતિ 'N':
      પાછા STATX_MODE;
    સ્થિતિ 'd':
    સ્થિતિ 'D':
      પાછા STATX_MODE;
    સ્થિતિ 'i':
      પાછા STATX_INO;
    સ્થિતિ 'a':
    સ્થિતિ 'A':
      પાછા STATX_MODE;
    સ્થિતિ 'f':
      પાછા STATX_MODE|STATX_TYPE;
    સ્થિતિ 'F':
      પાછા STATX_TYPE;
    સ્થિતિ 'h':
      પાછા STATX_NLINK;
    સ્થિતિ 'u':
    સ્થિતિ 'U':
      પાછા STATX_UID;
    સ્થિતિ 'g':
    સ્થિતિ 'G':
      પાછા STATX_GID;
    સ્થિતિ 'm':
      પાછા STATX_MODE|STATX_INO;
    સ્થિતિ 's':
      પાછા STATX_SIZE;
    સ્થિતિ 't':
    સ્થિતિ 'T':
      પાછા STATX_MODE;
    સ્થિતિ 'b':
      પાછા STATX_BLOCKS;
    સ્થિતિ 'w':
    સ્થિતિ 'W':
      પાછા STATX_BTIME;
    સ્થિતિ 'x':
    સ્થિતિ 'X':
      પાછા STATX_ATIME;
    સ્થિતિ 'y':
    સ્થિતિ 'Y':
      પાછા STATX_MTIME;
    સ્થિતિ 'z':
    સ્થિતિ 'Z':
      પાછા STATX_CTIME;
    }
  પાછા 0;
}

ATTRIBUTE_PURE
સ્થિર unsigned પૂર્ણાંક
format_to_mask (અક્ષર અચલ *format)
{
  unsigned પૂર્ણાંક mask = 0;

  માટે (અક્ષર અચલ *b = format; *b; b++)
    {
      જો (*b != '%')
        ચાલુ;

      b += format_code_offset (b);
      જો (*b == '\0')
        તોડો;
      mask |= fmt_to_mask (*b);
    }
  પાછા mask;
}

/* statx the file and print what we find */
NODISCARD
સ્થિર bool
do_stat (અક્ષર અચલ *filename, અક્ષર અચલ *format, અક્ષર અચલ *format2)
{
  પૂર્ણાંક fd = streq (filename, "-") ? 0 : AT_FDCWD;
  પૂર્ણાંક flags = 0;
  સંરચના stat st;
  સંરચના statx stx = {0};
  અક્ષર અચલ *pathname = filename;
  સંરચના print_args pa;
  pa.st = &st;
  pa.btime = (સંરચના timespec) {.tv_sec = -1, .tv_nsec = -1};

  જો (AT_FDCWD != fd)
    {
      pathname = "";
      flags = AT_EMPTY_PATH;
    }
  નહીંતર જો (!follow_links)
    {
      flags = AT_SYMLINK_NOFOLLOW;
    }

  જો (dont_sync)
    flags |= AT_STATX_DONT_SYNC;
  નહીંતર જો (force_sync)
    flags |= AT_STATX_FORCE_SYNC;

  જો (! force_sync)
    flags |= AT_NO_AUTOMOUNT;

  fd = statx (fd, pathname, flags, format_to_mask (format), &stx);
  જો (fd < 0)
    {
      જો (flags & AT_EMPTY_PATH)
        error (0, errno, _("cannot stat standard input"));
      નહીંતર
        error (0, errno, _("cannot statx %s"), quoteaf (filename));
      પાછા false;
    }

  જો (S_ISBLK (stx.stx_mode) || S_ISCHR (stx.stx_mode))
    format = format2;

  statx_to_stat (&stx, &st);
  જો (stx.stx_mask & STATX_BTIME)
    pa.btime = statx_timestamp_to_timespec (stx.stx_btime);

  bool fail = print_it (format, fd, filename, print_stat, &pa);
  પાછા ! fail;
}

#નહીંતર /* USE_STATX */

સ્થિર સંરચના timespec
get_birthtime (પૂર્ણાંક fd, અક્ષર અચલ *filename, સંરચના stat અચલ *st)
{
  સંરચના timespec ts = get_stat_birthtime (st);

# જો HAVE_GETATTRAT
  જો (ts.tv_nsec < 0)
    {
      nvlist_t *response;
      જો ((fd < 0
           ? getattrat (AT_FDCWD, XATTR_VIEW_READWRITE, filename, &response)
           : fgetattr (fd, XATTR_VIEW_READWRITE, &response))
          == 0)
        {
          uint64_t *val;
          uint_t n;
          જો (nvlist_lookup_uint64_array (response, A_CRTIME, &val, &n) == 0
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

  પાછા ts;
}


/* stat the file and print what we find */
NODISCARD
સ્થિર bool
do_stat (અક્ષર અચલ *filename, અક્ષર અચલ *format,
         અક્ષર અચલ *format2)
{
  પૂર્ણાંક fd = streq (filename, "-") ? 0 : -1;
  સંરચના stat statbuf;
  સંરચના print_args pa;
  pa.st = &statbuf;
  pa.btime = (સંરચના timespec) {.tv_sec = -1, .tv_nsec = -1};

  જો (0 <= fd)
    {
      જો (fstat (fd, &statbuf) != 0)
        {
          error (0, errno, _("cannot stat standard input"));
          પાછા false;
        }
    }
  /* We can't use the shorter
     (follow_links?stat:lstat) (filename, &statbug)
     since stat might be a function-like macro.  */
  નહીંતર જો ((follow_links
            ? stat (filename, &statbuf)
            : lstat (filename, &statbuf)) != 0)
    {
      error (0, errno, _("cannot stat %s"), quoteaf (filename));
      પાછા false;
    }

  જો (S_ISBLK (statbuf.st_mode) || S_ISCHR (statbuf.st_mode))
    format = format2;

  bool fail = print_it (format, fd, filename, print_stat, &pa);
  પાછા ! fail;
}
#endif /* USE_STATX */

/* POSIX requires 'ls' to print file sizes without a sign, even
   when negative.  Be consistent with that.  */

સ્થિર uintmax_t
unsigned_file_size (off_t size)
{
  પાછા size + (size < 0) * ((uintmax_t) OFF_T_MAX - OFF_T_MIN + 1);
}

/* Print stat info.  Return zero upon success, nonzero upon failure.  */
સ્થિર bool
print_stat (અક્ષર *pformat, size_t prefix_len, અક્ષર mod, અક્ષર m,
            MAYBE_UNUSED પૂર્ણાંક fd, અક્ષર અચલ *filename, શૂન્ય અચલ *data)
{
  સંરચના print_args *parg = (સંરચના print_args *) data;
  સંરચના stat *statbuf = parg->st;
  સંરચના timespec btime = parg->btime;
  સંરચના passwd *pw_ent;
  સંરચના group *gw_ent;
  bool fail = false;

  પસંદગી (m)
    {
    સ્થિતિ 'n':
      જો (mod == 'Q')
        {
          initialize_quoting_style ();
          filename = quoteN (filename);
        }
      out_string (pformat, prefix_len, filename);
      તોડો;
    સ્થિતિ 'N':
      initialize_quoting_style ();
      out_string (pformat, prefix_len, quoteN (filename));
      જો (S_ISLNK (statbuf->st_mode))
        {
          અક્ષર *linkname = areadlink_with_size (filename, statbuf->st_size);
          જો (linkname == ખાલી)
            {
              error (0, errno, _("cannot read symbolic link %s"),
                     quoteaf (filename));
              પાછા true;
            }
          printf (" -> ");
          out_string (pformat, prefix_len, quoteN (linkname));
          મુક્ત (linkname);
        }
      તોડો;
    સ્થિતિ 'd':
      જો (mod == 'H')
        out_uint (pformat, prefix_len, major (statbuf->st_dev));
      નહીંતર જો (mod == 'L')
        out_uint (pformat, prefix_len, minor (statbuf->st_dev));
      નહીંતર
        out_uint (pformat, prefix_len, statbuf->st_dev);
      તોડો;
    સ્થિતિ 'D':
      out_uint_x (pformat, prefix_len, statbuf->st_dev);
      તોડો;
    સ્થિતિ 'i':
      out_uint (pformat, prefix_len, statbuf->st_ino);
      તોડો;
    સ્થિતિ 'a':
      out_uint_o (pformat, prefix_len, statbuf->st_mode & CHMOD_MODE_BITS);
      તોડો;
    સ્થિતિ 'A':
      out_string (pformat, prefix_len, human_access (statbuf));
      તોડો;
    સ્થિતિ 'f':
      out_uint_x (pformat, prefix_len, statbuf->st_mode);
      તોડો;
    સ્થિતિ 'F':
      out_string (pformat, prefix_len, file_type (statbuf));
      તોડો;
    સ્થિતિ 'h':
      out_uint (pformat, prefix_len, statbuf->st_nlink);
      તોડો;
    સ્થિતિ 'u':
      out_uint (pformat, prefix_len, statbuf->st_uid);
      તોડો;
    સ્થિતિ 'U':
      pw_ent = getpwuid (statbuf->st_uid);
      out_string (pformat, prefix_len,
                  pw_ent ? pw_ent->pw_name : "UNKNOWN");
      તોડો;
    સ્થિતિ 'g':
      out_uint (pformat, prefix_len, statbuf->st_gid);
      તોડો;
    સ્થિતિ 'G':
      gw_ent = getgrgid (statbuf->st_gid);
      out_string (pformat, prefix_len,
                  gw_ent ? gw_ent->gr_name : "UNKNOWN");
      તોડો;
    સ્થિતિ 'm':
      fail |= out_mount_point (filename, pformat, prefix_len, statbuf);
      તોડો;
    સ્થિતિ 's':
      out_uint (pformat, prefix_len, unsigned_file_size (statbuf->st_size));
      તોડો;
    સ્થિતિ 'r':
      જો (mod == 'H')
        out_uint (pformat, prefix_len, major (statbuf->st_rdev));
      નહીંતર જો (mod == 'L')
        out_uint (pformat, prefix_len, minor (statbuf->st_rdev));
      નહીંતર
        out_uint (pformat, prefix_len, statbuf->st_rdev);
      તોડો;
    સ્થિતિ 'R':
      out_uint_x (pformat, prefix_len, statbuf->st_rdev);
      તોડો;
    સ્થિતિ 't':
      out_uint_x (pformat, prefix_len, major (statbuf->st_rdev));
      તોડો;
    સ્થિતિ 'T':
      out_uint_x (pformat, prefix_len, minor (statbuf->st_rdev));
      તોડો;
    સ્થિતિ 'B':
      out_uint (pformat, prefix_len, ST_NBLOCKSIZE);
      તોડો;
    સ્થિતિ 'b':
      out_uint (pformat, prefix_len, STP_NBLOCKS (statbuf));
      તોડો;
    સ્થિતિ 'o':
      out_uint (pformat, prefix_len, STP_BLKSIZE (statbuf));
      તોડો;
    સ્થિતિ 'w':
      {
#જો ! USE_STATX
        btime = get_birthtime (fd, filename, statbuf);
#endif
        જો (btime.tv_nsec < 0)
          out_string (pformat, prefix_len, "-");
        નહીંતર
          out_string (pformat, prefix_len, human_time (btime));
      }
      તોડો;
    સ્થિતિ 'W':
      {
#જો ! USE_STATX
        btime = get_birthtime (fd, filename, statbuf);
#endif
        out_epoch_sec (pformat, prefix_len, neg_to_zero (btime));
      }
      તોડો;
    સ્થિતિ 'x':
      out_string (pformat, prefix_len, human_time (get_stat_atime (statbuf)));
      તોડો;
    સ્થિતિ 'X':
      out_epoch_sec (pformat, prefix_len, get_stat_atime (statbuf));
      તોડો;
    સ્થિતિ 'y':
      out_string (pformat, prefix_len, human_time (get_stat_mtime (statbuf)));
      તોડો;
    સ્થિતિ 'Y':
      out_epoch_sec (pformat, prefix_len, get_stat_mtime (statbuf));
      તોડો;
    સ્થિતિ 'z':
      out_string (pformat, prefix_len, human_time (get_stat_ctime (statbuf)));
      તોડો;
    સ્થિતિ 'Z':
      out_epoch_sec (pformat, prefix_len, get_stat_ctime (statbuf));
      તોડો;
    સ્થિતિ 'C':
      fail |= out_file_context (pformat, prefix_len, filename);
      તોડો;
    મૂળભૂત:
      fputc ('?', stdout);
      તોડો;
    }
  પાછા fail;
}

/* Return an allocated format string in static storage that
   corresponds to whether FS and TERSE options were declared.  */
સ્થિર અક્ષર *
default_format (bool fs, bool terse, bool device)
{
  અક્ષર *format;
  જો (fs)
    {
      જો (terse)
        format = xstrdup (fmt_terse_fs);
      નહીંતર
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
  નહીંતર /* ! fs */
    {
      જો (terse)
        {
          જો (0 < is_selinux_enabled ())
            format = xstrdup (fmt_terse_selinux);
          નહીંતર
            format = xstrdup (fmt_terse_regular);
        }
      નહીંતર
        {
          અક્ષર *temp;
          /* TRANSLATORS: This string uses format specifiers from
             'stat --help' without --file-system, and NOT from printf.  */
          format = xstrdup (_("\
  File: %N\n\
  Size: %-10s\tBlocks: %-10b IO Block: %-6o %F\n\
"));

          temp = format;
          જો (device)
            {
              /* TRANSLATORS: This string uses format specifiers from
                 'stat --help' without --file-system, and NOT from printf.  */
              format = xasprintf ("%s%s", format, _("\
" "Device: %Hd,%Ld\tInode: %-10i  Links: %-5h Device type: %Hr,%Lr\n\
"));
            }
          નહીંતર
            {
              /* TRANSLATORS: This string uses format specifiers from
                 'stat --help' without --file-system, and NOT from printf.  */
              format = xasprintf ("%s%s", format, _("\
" "Device: %Hd,%Ld\tInode: %-10i  Links: %h\n\
"));
            }
          મુક્ત (temp);

          temp = format;
          /* TRANSLATORS: This string uses format specifiers from
             'stat --help' without --file-system, and NOT from printf.  */
          format = xasprintf ("%s%s", format, _("\
" "Access: (%04a/%10.10A)  Uid: (%5u/%8U)   Gid: (%5g/%8G)\n\
"));
          મુક્ત (temp);

          જો (0 < is_selinux_enabled ())
            {
              temp = format;
              /* TRANSLATORS: This string uses format specifiers from
                 'stat --help' without --file-system, and NOT from printf.  */
              format = xasprintf ("%s%s", format, _("Context: %C\n"));
              મુક્ત (temp);
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
          મુક્ત (temp);
        }
    }
  પાછા format;
}

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
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
         use the specified FORMAT instead of the મૂળભૂત;\n\
         output a newline after each use of FORMAT\n\
"));
      oputs (_("\
      --printf=FORMAT\n\
         like --format, but interpret backslash escapes,\n\
         and કરો not output a mandatory trailing newline;\n\
         જો you want a newline, include \\n in FORMAT\n\
"));
      oputs (_("\
  -t, --terse\n\
         print the information in terse form\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);

      fputs (_("\n\
The MODE argument of --cached can be: always, never, or મૂળભૂત.\n\
'always' will use cached attributes જો available, જ્યારે\n\
'never' will try to synchronize with the latest attributes, and\n\
'default' will leave it up to the underlying file system.\n\
"), stdout);

      fputs (_("\n\
The valid format sequences માટે files (without --file-system):\n\
\n\
  %a   permission bits in octal (see '#' and '0' printf flags)\n\
  %A   permission bits and file type in human readable form\n\
  %b   સંખ્યા of blocks allocated (see %B)\n\
  %B   the size in bytes of each block reported by %b\n\
  %C   SELinux security context string\n\
"), stdout);
      fputs (_("\
  %d   device સંખ્યા in decimal (st_dev)\n\
  %D   device સંખ્યા in hex (st_dev)\n\
  %Hd  major device સંખ્યા in decimal\n\
  %Ld  minor device સંખ્યા in decimal\n\
  %f   raw mode in hex\n\
  %F   file type\n\
  %g   group ID of owner\n\
  %G   group name of owner\n\
"), stdout);
      fputs (_("\
  %h   સંખ્યા of hard links\n\
  %i   inode સંખ્યા\n\
  %m   mount point\n\
  %n   file name\n\
  %Qn  quoted file name\n\
  %N   quoted file name with dereference જો symbolic link\n\
  %o   optimal I/O transfer size hint\n\
  %s   total size, in bytes\n\
  %r   device type in decimal (st_rdev)\n\
  %R   device type in hex (st_rdev)\n\
  %Hr  major device type in decimal, માટે character/block device special files\n\
  %Lr  minor device type in decimal, માટે character/block device special files\n\
  %t   major device type in hex, માટે character/block device special files\n\
  %T   minor device type in hex, માટે character/block device special files\n\
"), stdout);
      fputs (_("\
  %u   user ID of owner\n\
  %U   user name of owner\n\
  %w   time of file birth, human-readable; - જો unknown\n\
  %W   time of file birth, seconds since Epoch; 0 જો unknown\n\
  %x   time of last access, human-readable\n\
  %X   time of last access, seconds since Epoch\n\
  %y   time of last data modification, human-readable\n\
  %Y   time of last data modification, seconds since Epoch\n\
  %z   time of last status change, human-readable\n\
  %Z   time of last status change, seconds since Epoch\n\
\n\
"), stdout);

      fputs (_("\
Valid format sequences માટે file systems:\n\
\n\
  %a   મુક્ત blocks available to non-superuser\n\
  %b   total data blocks in file system\n\
  %c   total file nodes in file system\n\
  %d   મુક્ત file nodes in file system\n\
  %f   મુક્ત blocks in file system\n\
"), stdout);
      fputs (_("\
  %i   file system ID in hex\n\
  %l   maximum length of filenames\n\
  %n   file name\n\
  %Qn  quoted file name\n\
  %s   block size (માટે faster transfers)\n\
  %S   fundamental block size (માટે block counts)\n\
  %t   file system type in hex\n\
  %T   file system type in human readable form\n\
"), stdout);

      printf (_("\n\
--terse is equivalent to the following FORMAT:\n\
    %s\
"),
#જો HAVE_SELINUX_SELINUX_H
              fmt_terse_selinux
#નહીંતર
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

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર *argv[])
{
  પૂર્ણાંક c;
  bool fs = false;
  bool terse = false;
  અક્ષર *format = ખાલી;
  અક્ષર *format2;
  bool ok = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  સંરચના lconv અચલ *locale = localeconv ();
  decimal_point = (locale->decimal_point[0] ? locale->decimal_point : ".");
  decimal_point_len = strlen (decimal_point);

  atexit (close_stdout);

  જ્યારે ((c = getopt_long (argc, argv, "c:fLt", long_options, ખાલી)) != -1)
    {
      પસંદગી (c)
        {
        સ્થિતિ PRINTF_OPTION:
          format = optarg;
          interpret_backslash_escapes = true;
          trailing_delim = "";
          તોડો;

        સ્થિતિ 'c':
          format = optarg;
          interpret_backslash_escapes = false;
          trailing_delim = "\n";
          તોડો;

        સ્થિતિ 'L':
          follow_links = true;
          તોડો;

        સ્થિતિ 'f':
          fs = true;
          તોડો;

        સ્થિતિ 't':
          terse = true;
          તોડો;

        સ્થિતિ 0:
          પસંદગી (XARGMATCH ("--cached", optarg, cached_args, cached_modes))
            {
              સ્થિતિ cached_never:
                force_sync = true;
                dont_sync = false;
                તોડો;
              સ્થિતિ cached_always:
                force_sync = false;
                dont_sync = true;
                તોડો;
              સ્થિતિ cached_default:
                force_sync = false;
                dont_sync = false;
            }
          તોડો;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        મૂળભૂત:
          usage (EXIT_FAILURE);
        }
    }

  જો (argc == optind)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  જો (format)
    {
      format2 = format;
    }
  નહીંતર
    {
      format = default_format (fs, terse, /* device= */ false);
      format2 = default_format (fs, terse, /* device= */ true);
    }

  માટે (પૂર્ણાંક i = optind; i < argc; i++)
    ok &= (fs
           ? do_statfs (argv[i], format)
           : do_stat (argv[i], format, format2));

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
