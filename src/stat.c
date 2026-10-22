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

#அந்தர்பாவ <config.h>

/* Keep this conditional in sync with the similar conditional in
   ../m4/stat-prog.m4.  */
#எனில் ((STAT_STATVFS || STAT_STATVFS64)                                       \
     && (HAVE_STRUCT_STATVFS_F_BASETYPE || HAVE_STRUCT_STATVFS_F_FSTYPENAME \
         || (! HAVE_STRUCT_STATFS_F_FSTYPENAME && HAVE_STRUCT_STATVFS_F_TYPE)))
# define USE_STATVFS 1
#இல்லையெனில்
# define USE_STATVFS 0
#endif

#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <pwd.h>
#அந்தர்பாவ <grp.h>
#எனில் USE_STATVFS
# include <sys/statvfs.h>
#அல்லதெனில் HAVE_SYS_VFS_H
# include <sys/vfs.h>
#அல்லதெனில் HAVE_SYS_MOUNT_H && HAVE_SYS_PARAM_H
/* NOTE: freebsd5.0 needs sys/param.h and sys/mount.h for statfs.
   It does have statvfs.h, but shouldn't use it, since it doesn't
   HAVE_STRUCT_STATVFS_F_BASETYPE.  So find a clean way to fix it.  */
/* NetBSD 1.5.2 needs these, for the declaration of struct statfs. */
# include <sys/param.h>
# include <sys/mount.h>
# எனில் HAVE_NFS_NFS_CLNT_H && HAVE_NFS_VFS_H
/* Ultrix 4.4 needs these for the declaration of struct statfs.  */
#  include <netinet/in.h>
#  include <nfs/nfs_clnt.h>
#  include <nfs/vfs.h>
# endif
#அல்லதெனில் HAVE_OS_H /* BeOS */
# include <fs_info.h>
#endif
#அந்தர்பாவ <selinux/selinux.h>
#அந்தர்பாவ <getopt.h>

#அந்தர்பாவ "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#அந்தர்பாவ "system.h"

#அந்தர்பாவ "areadlink.h"
#அந்தர்பாவ "c-ctype.h"
#அந்தர்பாவ "file-type.h"
#அந்தர்பாவ "filemode.h"
#அந்தர்பாவ "fs.h"
#அந்தர்பாவ "mountlist.h"
#அந்தர்பாவ "octhexdigits.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "stat-size.h"
#அந்தர்பாவ "stat-time.h"
#அந்தர்பாவ "strftime.h"
#அந்தர்பாவ "find-mount-point.h"
#அந்தர்பாவ "xvasprintf.h"
#அந்தர்பாவ "statx.h"

#எனில் HAVE_STATX && defined STATX_INO
# define USE_STATX 1
#இல்லையெனில்
# define USE_STATX 0
#endif

#எனில் USE_STATVFS
# define STRUCT_STATXFS_F_FSID_IS_INTEGER STRUCT_STATVFS_F_FSID_IS_INTEGER
# define HAVE_STRUCT_STATXFS_F_TYPE HAVE_STRUCT_STATVFS_F_TYPE
# எனில் HAVE_STRUCT_STATVFS_F_NAMEMAX
#  define SB_F_NAMEMAX(S) ((S)->f_namemax)
# endif
# எனில் ! STAT_STATVFS && STAT_STATVFS64
#  define STRUCT_STATVFS கட்டமைப்பு statvfs64
#  define STATFS statvfs64
# இல்லையெனில்
#  define STRUCT_STATVFS கட்டமைப்பு statvfs
#  define STATFS statvfs
# endif
# define STATFS_FRSIZE(S) ((S)->f_frsize)
#இல்லையெனில்
# define HAVE_STRUCT_STATXFS_F_TYPE HAVE_STRUCT_STATFS_F_TYPE
# எனில் HAVE_STRUCT_STATFS_F_NAMELEN
#  define SB_F_NAMEMAX(S) ((S)->f_namelen)
# elif HAVE_STRUCT_STATFS_F_NAMEMAX
#  define SB_F_NAMEMAX(S) ((S)->f_namemax)
# endif
# define STATFS statfs
# எனில் HAVE_OS_H /* BeOS */
/* BeOS has a statvfs function, but it does not return sensible values
   for f_files, f_ffree and f_favail, and lacks f_type, f_basetype and
   f_fstypename.  Use 'struct fs_info' instead.  */
NODISCARD
நிலையான முழு
statfs (எழுத்து மாறிலி *filename, கட்டமைப்பு fs_info *buf)
{
  dev_t device = dev_for_path (filename);
  எனில் (device < 0)
    {
      errno = (device == B_ENTRY_NOT_FOUND ? ENOENT
               : device == B_BAD_மதிப்பு ? EINVAL
               : device == B_NAME_TOO_LONG ? ENAMETOOLONG
               : device == B_NO_MEMORY ? ENOMEM
               : device == B_FILE_ERROR ? EIO
               : 0);
      திரும்பு -1;
    }
  /* If successful, buf->dev will be == device.  */
  திரும்பு fs_stat_dev (device, buf);
}
#  define f_fsid dev
#  define f_blocks total_blocks
#  define f_bfree free_blocks
#  define f_bavail free_blocks
#  define f_bsize io_size
#  define f_files total_nodes
#  define f_ffree free_nodes
#  define STRUCT_STATVFS கட்டமைப்பு fs_info
#  define STRUCT_STATXFS_F_FSID_IS_INTEGER true
#  define STATFS_FRSIZE(S) ((S)->block_size)
# இல்லையெனில்
#  define STRUCT_STATVFS கட்டமைப்பு statfs
#  define STRUCT_STATXFS_F_FSID_IS_INTEGER STRUCT_STATFS_F_FSID_IS_INTEGER
#  எனில் HAVE_STRUCT_STATFS_F_FRSIZE
#   define STATFS_FRSIZE(S) ((S)->f_frsize)
#  இல்லையெனில்
#   define STATFS_FRSIZE(S) 0
#  endif
# endif
#endif

#ifdef SB_F_NAMEMAX
# define OUT_NAMEMAX out_uint
#இல்லையெனில்
/* Depending on whether statvfs or statfs is used,
   neither f_namemax or f_namelen may be available.  */
# define SB_F_NAMEMAX(S) "?"
# define OUT_NAMEMAX out_string
#endif

#எனில் HAVE_STRUCT_STATVFS_F_BASETYPE
# define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME f_basetype
#இல்லையெனில்
# எனில் HAVE_STRUCT_STATVFS_F_FSTYPENAME || HAVE_STRUCT_STATFS_F_FSTYPENAME
#  define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME f_fstypename
# elif HAVE_OS_H /* BeOS */
#  define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME fsh_name
# endif
#endif

#எனில் HAVE_GETATTRAT
# include <attr.h>
# include <sys/nvpair.h>
#endif

நிலையான எழுத்து மாறிலி digits[] = "0123456789";

/* Flags that are portable for use in printf, for at least one
   conversion specifier; make_format removes non-portable flags as
   needed for particular specifiers.  The glibc 2.2 extension "I" is
   listed here; it is removed by make_format because it has undefined
   behavior elsewhere and because it is incompatible with
   out_epoch_sec.  */
நிலையான எழுத்து மாறிலி printf_flags[] = "'-+ #0I";

/* Formats for the --terse option.  */
நிலையான எழுத்து மாறிலி fmt_terse_fs[] = "%Qn %i %l %t %s %S %b %f %a %c %d\n";
நிலையான எழுத்து மாறிலி fmt_terse_regular[] = "%Qn %s %b %f %u %g %D %i %h %t %T"
                                        " %X %Y %Z %W %o\n";
நிலையான எழுத்து மாறிலி fmt_terse_selinux[] = "%Qn %s %b %f %u %g %D %i %h %t %T"
                                        " %X %Y %Z %W %o %C\n";

#define PROGRAM_NAME "stat"

#define AUTHORS proper_name ("Michael Meskes")

எண்ணிக்கை
{
  PRINTF_OPTION = CHAR_MAX + 1
};

எண்ணிக்கை cached_mode
{
  cached_default,
  cached_never,
  cached_always
};

நிலையான எழுத்து மாறிலி *மாறிலி cached_args[] =
{
  "default", "never", "always", வெற்று
};

நிலையான எண்ணிக்கை cached_mode மாறிலி cached_modes[] =
{
  cached_default, cached_never, cached_always
};

நிலையான கட்டமைப்பு option மாறிலி long_options[] =
{
  {"dereference", no_argument, வெற்று, 'L'},
  {"file-system", no_argument, வெற்று, 'f'},
  {"format", required_argument, வெற்று, 'c'},
  {"printf", required_argument, வெற்று, PRINTF_OPTION},
  {"terse", no_argument, வெற்று, 't'},
  {"cached", required_argument, வெற்று, 0},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {வெற்று, 0, வெற்று, 0}
};

/* Whether to follow symbolic links;  True for --dereference (-L).  */
நிலையான bool follow_links;

/* Whether to interpret backslash-escape sequences.
   True for --printf=FMT, not for --format=FMT (-c).  */
நிலையான bool interpret_backslash_escapes;

/* The trailing delimiter string:
   "" for --printf=FMT, "\n" for --format=FMT (-c).  */
நிலையான எழுத்து மாறிலி *trailing_delim = "";

/* The representation of the decimal point in the current locale.  */
நிலையான எழுத்து மாறிலி *decimal_point;
நிலையான size_t decimal_point_len;

நிலையான bool
print_stat (எழுத்து *pformat, size_t prefix_len, எழுத்து mod, எழுத்து m,
            முழு fd, எழுத்து மாறிலி *filename, வெறுமை மாறிலி *data);

/* Return the type of the specified file system.
   Some systems have statfvs.f_basetype[FSTYPSZ] (AIX, HP-UX, and Solaris).
   Others have statvfs.f_fstypename[_VFS_NAMELEN] (NetBSD 3.0).
   Others have statfs.f_fstypename[MFSNAMELEN] (NetBSD 1.5.2).
   Still others have neither and have to get by with f_type (GNU/Linux).
   But f_type may only exist in statfs (Cygwin).  */
NODISCARD
நிலையான எழுத்து மாறிலி *
human_fstype (STRUCT_STATVFS மாறிலி *statfsbuf)
{
#ifdef STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME
  திரும்பு statfsbuf->STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME;
#இல்லையெனில்
  தேர்வு (statfsbuf->f_type)
    {
# எனில் defined __linux__ || defined __ANDROID__

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

    நிலை S_MAGIC_AAFS: /* 0x5A3C69F0 local */
      திரும்பு "aafs";
    நிலை S_MAGIC_ACFS: /* 0x61636673 remote */
      திரும்பு "acfs";
    நிலை S_MAGIC_ADFS: /* 0xADF5 local */
      திரும்பு "adfs";
    நிலை S_MAGIC_AFFS: /* 0xADFF local */
      திரும்பு "affs";
    நிலை S_MAGIC_AFS: /* 0x5346414F remote */
      திரும்பு "afs";
    நிலை S_MAGIC_ANON_INODE_FS: /* 0x09041934 local */
      திரும்பு "anon-inode FS";
    நிலை S_MAGIC_AUFS: /* 0x61756673 remote */
      /* FIXME: change syntax or add an optional attribute like "inotify:no".
         The above is labeled as "remote" so that tail always uses polling,
         but this isn't really a remote file system type.  */
      திரும்பு "aufs";
    நிலை S_MAGIC_AUTOFS: /* 0x0187 local */
      திரும்பு "autofs";
    நிலை S_MAGIC_BALLOON_KVM: /* 0x13661366 local */
      திரும்பு "balloon-kvm-fs";
    நிலை S_MAGIC_BCACHEFS: /* 0xCA451A4E local */
      திரும்பு "bcachefs";
    நிலை S_MAGIC_BEFS: /* 0x42465331 local */
      திரும்பு "befs";
    நிலை S_MAGIC_BDEVFS: /* 0x62646576 local */
      திரும்பு "bdevfs";
    நிலை S_MAGIC_BFS: /* 0x1BADFACE local */
      திரும்பு "bfs";
    நிலை S_MAGIC_BINDERFS: /* 0x6C6F6F70 local */
      திரும்பு "binderfs";
    நிலை S_MAGIC_BPF_FS: /* 0xCAFE4A11 local */
      திரும்பு "bpf_fs";
    நிலை S_MAGIC_BINFMTFS: /* 0x42494E4D local */
      திரும்பு "binfmt_misc";
    நிலை S_MAGIC_BTRFS: /* 0x9123683E local */
      திரும்பு "btrfs";
    நிலை S_MAGIC_BTRFS_TEST: /* 0x73727279 local */
      திரும்பு "btrfs_test";
    நிலை S_MAGIC_CEPH: /* 0x00C36400 remote */
      திரும்பு "ceph";
    நிலை S_MAGIC_CGROUP: /* 0x0027E0EB local */
      திரும்பு "cgroupfs";
    நிலை S_MAGIC_CGROUP2: /* 0x63677270 local */
      திரும்பு "cgroup2fs";
    நிலை S_MAGIC_CIFS: /* 0xFF534D42 remote */
      திரும்பு "cifs";
    நிலை S_MAGIC_CODA: /* 0x73757245 remote */
      திரும்பு "coda";
    நிலை S_MAGIC_COH: /* 0x012FF7B7 local */
      திரும்பு "coh";
    நிலை S_MAGIC_CONFIGFS: /* 0x62656570 local */
      திரும்பு "configfs";
    நிலை S_MAGIC_CRAMFS: /* 0x28CD3D45 local */
      திரும்பு "cramfs";
    நிலை S_MAGIC_CRAMFS_WEND: /* 0x453DCD28 local */
      திரும்பு "cramfs-wend";
    நிலை S_MAGIC_DAXFS: /* 0x64646178 local */
      திரும்பு "daxfs";
    நிலை S_MAGIC_DEBUGFS: /* 0x64626720 local */
      திரும்பு "debugfs";
    நிலை S_MAGIC_DEVFS: /* 0x1373 local */
      திரும்பு "devfs";
    நிலை S_MAGIC_DEVMEM: /* 0x454D444D local */
      திரும்பு "devmem";
    நிலை S_MAGIC_DEVPTS: /* 0x1CD1 local */
      திரும்பு "devpts";
    நிலை S_MAGIC_DMA_BUF: /* 0x444D4142 local */
      திரும்பு "dma-buf-fs";
    நிலை S_MAGIC_ECRYPTFS: /* 0xF15F local */
      திரும்பு "ecryptfs";
    நிலை S_MAGIC_EFIVARFS: /* 0xDE5E81E4 local */
      திரும்பு "efivarfs";
    நிலை S_MAGIC_EFS: /* 0x00414A53 local */
      திரும்பு "efs";
    நிலை S_MAGIC_EROFS_V1: /* 0xE0F5E1E2 local */
      திரும்பு "erofs";
    நிலை S_MAGIC_EXFAT: /* 0x2011BAB0 local */
      திரும்பு "exfat";
    நிலை S_MAGIC_EXFS: /* 0x45584653 local */
      திரும்பு "exfs";
    நிலை S_MAGIC_EXOFS: /* 0x5DF5 local */
      திரும்பு "exofs";
    நிலை S_MAGIC_EXT: /* 0x137D local */
      திரும்பு "ext";
    நிலை S_MAGIC_EXT2: /* 0xEF53 local */
      திரும்பு "ext2/ext3";
    நிலை S_MAGIC_EXT2_OLD: /* 0xEF51 local */
      திரும்பு "ext2";
    நிலை S_MAGIC_F2FS: /* 0xF2F52010 local */
      திரும்பு "f2fs";
    நிலை S_MAGIC_FAIL_FS: /* 0x4641494C local */
      திரும்பு "failfs";
    நிலை S_MAGIC_FAT: /* 0x4006 local */
      திரும்பு "fat";
    நிலை S_MAGIC_FHGFS: /* 0x19830326 remote */
      திரும்பு "fhgfs";
    நிலை S_MAGIC_FUSE: /* 0x65735546 remote */
      திரும்பு "fuse";
    நிலை S_MAGIC_FUSECTL: /* 0x65735543 remote */
      திரும்பு "fusectl";
    நிலை S_MAGIC_FUTEXFS: /* 0x0BAD1DEA local */
      திரும்பு "futexfs";
    நிலை S_MAGIC_GFS: /* 0x01161970 remote */
      திரும்பு "gfs/gfs2";
    நிலை S_MAGIC_GPFS: /* 0x47504653 remote */
      திரும்பு "gpfs";
    நிலை S_MAGIC_GUEST_MEMFD: /* 0x474D454D remote */
      திரும்பு "guest-memfd";
    நிலை S_MAGIC_HFS: /* 0x4244 local */
      திரும்பு "hfs";
    நிலை S_MAGIC_HFS_PLUS: /* 0x482B local */
      திரும்பு "hfs+";
    நிலை S_MAGIC_HFS_X: /* 0x4858 local */
      திரும்பு "hfsx";
    நிலை S_MAGIC_HOSTFS: /* 0x00C0FFEE local */
      திரும்பு "hostfs";
    நிலை S_MAGIC_HPFS: /* 0xF995E849 local */
      திரும்பு "hpfs";
    நிலை S_MAGIC_HUGETLBFS: /* 0x958458F6 local */
      திரும்பு "hugetlbfs";
    நிலை S_MAGIC_MTD_INODE_FS: /* 0x11307854 local */
      திரும்பு "inodefs";
    நிலை S_MAGIC_IBRIX: /* 0x013111A8 remote */
      திரும்பு "ibrix";
    நிலை S_MAGIC_INOTIFYFS: /* 0x2BAD1DEA local */
      திரும்பு "inotifyfs";
    நிலை S_MAGIC_ISOFS: /* 0x9660 local */
      திரும்பு "isofs";
    நிலை S_MAGIC_ISOFS_R_WIN: /* 0x4004 local */
      திரும்பு "isofs";
    நிலை S_MAGIC_ISOFS_WIN: /* 0x4000 local */
      திரும்பு "isofs";
    நிலை S_MAGIC_JFFS: /* 0x07C0 local */
      திரும்பு "jffs";
    நிலை S_MAGIC_JFFS2: /* 0x72B6 local */
      திரும்பு "jffs2";
    நிலை S_MAGIC_JFS: /* 0x3153464A local */
      திரும்பு "jfs";
    நிலை S_MAGIC_KAFS: /* 0x6B414653 remote */
      திரும்பு "k-afs";
    நிலை S_MAGIC_LOGFS: /* 0xC97E8168 local */
      திரும்பு "logfs";
    நிலை S_MAGIC_LUSTRE: /* 0x0BD00BD0 remote */
      திரும்பு "lustre";
    நிலை S_MAGIC_M1FS: /* 0x5346314D local */
      திரும்பு "m1fs";
    நிலை S_MAGIC_MINIX: /* 0x137F local */
      திரும்பு "minix";
    நிலை S_MAGIC_MINIX_30: /* 0x138F local */
      திரும்பு "minix (30 char.)";
    நிலை S_MAGIC_MINIX_V2: /* 0x2468 local */
      திரும்பு "minix v2";
    நிலை S_MAGIC_MINIX_V2_30: /* 0x2478 local */
      திரும்பு "minix v2 (30 char.)";
    நிலை S_MAGIC_MINIX_V3: /* 0x4D5A local */
      திரும்பு "minix3";
    நிலை S_MAGIC_MQUEUE: /* 0x19800202 local */
      திரும்பு "mqueue";
    நிலை S_MAGIC_MSDOS: /* 0x4D44 local */
      திரும்பு "msdos";
    நிலை S_MAGIC_NCP: /* 0x564C remote */
      திரும்பு "novell";
    நிலை S_MAGIC_NFS: /* 0x6969 remote */
      திரும்பு "nfs";
    நிலை S_MAGIC_NFSD: /* 0x6E667364 remote */
      திரும்பு "nfsd";
    நிலை S_MAGIC_NILFS: /* 0x3434 local */
      திரும்பு "nilfs";
    நிலை S_MAGIC_NSFS: /* 0x6E736673 local */
      திரும்பு "nsfs";
    நிலை S_MAGIC_NTFS: /* 0x5346544E local */
      திரும்பு "ntfs";
    நிலை S_MAGIC_NULL_FS: /* 0x4E554C4C local */
      திரும்பு "nullfs";
    நிலை S_MAGIC_OPENPROM: /* 0x9FA1 local */
      திரும்பு "openprom";
    நிலை S_MAGIC_OCFS2: /* 0x7461636F remote */
      திரும்பு "ocfs2";
    நிலை S_MAGIC_OVERLAYFS: /* 0x794C7630 remote */
      /* This may overlay remote file systems.
         Also there have been issues reported with inotify and overlayfs,
         so mark as "remote" so that polling is used.  */
      திரும்பு "overlayfs";
    நிலை S_MAGIC_PANFS: /* 0xAAD7AAEA remote */
      திரும்பு "panfs";
    நிலை S_MAGIC_PID_FS: /* 0x50494446 local */
      திரும்பு "pidfs";
    நிலை S_MAGIC_PIPEFS: /* 0x50495045 remote */
      /* FIXME: change syntax or add an optional attribute like "inotify:no".
         pipefs is labeled as "remote" so that tail always polls,
         but it isn't really remote file system type.  */
      திரும்பு "pipefs";
    நிலை S_MAGIC_PPC_CMM: /* 0xC7571590 local */
      திரும்பு "ppc-cmm-fs";
    நிலை S_MAGIC_PRL_FS: /* 0x7C7C6673 remote */
      திரும்பு "prl_fs";
    நிலை S_MAGIC_PROC: /* 0x9FA0 local */
      திரும்பு "proc";
    நிலை S_MAGIC_PSTOREFS: /* 0x6165676C local */
      திரும்பு "pstorefs";
    நிலை S_MAGIC_QNX4: /* 0x002F local */
      திரும்பு "qnx4";
    நிலை S_MAGIC_QNX6: /* 0x68191122 local */
      திரும்பு "qnx6";
    நிலை S_MAGIC_RAMFS: /* 0x858458F6 local */
      திரும்பு "ramfs";
    நிலை S_MAGIC_RDTGROUP: /* 0x07655821 local */
      திரும்பு "rdt";
    நிலை S_MAGIC_REISERFS: /* 0x52654973 local */
      திரும்பு "reiserfs";
    நிலை S_MAGIC_ROMFS: /* 0x7275 local */
      திரும்பு "romfs";
    நிலை S_MAGIC_RPC_PIPEFS: /* 0x67596969 local */
      திரும்பு "rpc_pipefs";
    நிலை S_MAGIC_SDCARDFS: /* 0x5DCA2DF5 local */
      திரும்பு "sdcardfs";
    நிலை S_MAGIC_SECRETMEM: /* 0x5345434D local */
      திரும்பு "secretmem";
    நிலை S_MAGIC_SECURITYFS: /* 0x73636673 local */
      திரும்பு "securityfs";
    நிலை S_MAGIC_SELINUX: /* 0xF97CFF8C local */
      திரும்பு "selinux";
    நிலை S_MAGIC_SMACK: /* 0x43415D53 local */
      திரும்பு "smackfs";
    நிலை S_MAGIC_SMB: /* 0x517B remote */
      திரும்பு "smb";
    நிலை S_MAGIC_SMB2: /* 0xFE534D42 remote */
      திரும்பு "smb2";
    நிலை S_MAGIC_SNFS: /* 0xBEEFDEAD remote */
      திரும்பு "snfs";
    நிலை S_MAGIC_SOCKFS: /* 0x534F434B local */
      திரும்பு "sockfs";
    நிலை S_MAGIC_SQUASHFS: /* 0x73717368 local */
      திரும்பு "squashfs";
    நிலை S_MAGIC_SYSFS: /* 0x62656572 local */
      திரும்பு "sysfs";
    நிலை S_MAGIC_SYSV2: /* 0x012FF7B6 local */
      திரும்பு "sysv2";
    நிலை S_MAGIC_SYSV4: /* 0x012FF7B5 local */
      திரும்பு "sysv4";
    நிலை S_MAGIC_TMPFS: /* 0x01021994 local */
      திரும்பு "tmpfs";
    நிலை S_MAGIC_TRACEFS: /* 0x74726163 local */
      திரும்பு "tracefs";
    நிலை S_MAGIC_UBIFS: /* 0x24051905 local */
      திரும்பு "ubifs";
    நிலை S_MAGIC_UDF: /* 0x15013346 local */
      திரும்பு "udf";
    நிலை S_MAGIC_UFS: /* 0x00011954 local */
      திரும்பு "ufs";
    நிலை S_MAGIC_UFS_BYTESWAPPED: /* 0x54190100 local */
      திரும்பு "ufs";
    நிலை S_MAGIC_USBDEVFS: /* 0x9FA2 local */
      திரும்பு "usbdevfs";
    நிலை S_MAGIC_V9FS: /* 0x01021997 local */
      திரும்பு "v9fs";
    நிலை S_MAGIC_VBOXSF: /* 0x786F4256 remote */
      திரும்பு "vboxsf";
    நிலை S_MAGIC_VMHGFS: /* 0xBACBACBC remote */
      திரும்பு "vmhgfs";
    நிலை S_MAGIC_VXFS: /* 0xA501FCF5 remote */
      /* Veritas File System can run in single instance or clustered mode,
         so mark as remote to cater for the latter case.  */
      திரும்பு "vxfs";
    நிலை S_MAGIC_VZFS: /* 0x565A4653 local */
      திரும்பு "vzfs";
    நிலை S_MAGIC_WSLFS: /* 0x53464846 local */
      திரும்பு "wslfs";
    நிலை S_MAGIC_XENFS: /* 0xABBA1974 local */
      திரும்பு "xenfs";
    நிலை S_MAGIC_XENIX: /* 0x012FF7B4 local */
      திரும்பு "xenix";
    நிலை S_MAGIC_XFS: /* 0x58465342 local */
      திரும்பு "xfs";
    நிலை S_MAGIC_XIAFS: /* 0x012FD16D local */
      திரும்பு "xia";
    நிலை S_MAGIC_Z3FOLD: /* 0x0033 local */
      திரும்பு "z3fold";
    நிலை S_MAGIC_ZFS: /* 0x2FC12FC1 local */
      திரும்பு "zfs";
    நிலை S_MAGIC_ZONEFS: /* 0x5A4F4653 local */
      திரும்பு "zonefs";
    நிலை S_MAGIC_ZSMALLOC: /* 0x58295829 local */
      திரும்பு "zsmallocfs";


# elif __GNU__
    நிலை FSTYPE_UFS:
      திரும்பு "ufs";
    நிலை FSTYPE_NFS:
      திரும்பு "nfs";
    நிலை FSTYPE_GFS:
      திரும்பு "gfs";
    நிலை FSTYPE_LFS:
      திரும்பு "lfs";
    நிலை FSTYPE_SYSV:
      திரும்பு "sysv";
    நிலை FSTYPE_FTP:
      திரும்பு "ftp";
    நிலை FSTYPE_TAR:
      திரும்பு "tar";
    நிலை FSTYPE_AR:
      திரும்பு "ar";
    நிலை FSTYPE_CPIO:
      திரும்பு "cpio";
    நிலை FSTYPE_MSLOSS:
      திரும்பு "msloss";
    நிலை FSTYPE_CPM:
      திரும்பு "cpm";
    நிலை FSTYPE_HFS:
      திரும்பு "hfs";
    நிலை FSTYPE_DTFS:
      திரும்பு "dtfs";
    நிலை FSTYPE_GRFS:
      திரும்பு "grfs";
    நிலை FSTYPE_TERM:
      திரும்பு "term";
    நிலை FSTYPE_DEV:
      திரும்பு "dev";
    நிலை FSTYPE_PROC:
      திரும்பு "proc";
    நிலை FSTYPE_IFSOCK:
      திரும்பு "ifsock";
    நிலை FSTYPE_AFS:
      திரும்பு "afs";
    நிலை FSTYPE_DFS:
      திரும்பு "dfs";
    நிலை FSTYPE_PROC9:
      திரும்பு "proc9";
    நிலை FSTYPE_SOCKET:
      திரும்பு "socket";
    நிலை FSTYPE_MISC:
      திரும்பு "misc";
    நிலை FSTYPE_EXT2FS:
      திரும்பு "ext2/ext3";
    நிலை FSTYPE_HTTP:
      திரும்பு "http";
    நிலை FSTYPE_MEMFS:
      திரும்பு "memfs";
    நிலை FSTYPE_ISO9660:
      திரும்பு "iso9660";
# endif
    இயல்பு:
      {
        unsigned long முழு type = statfsbuf->f_type;
        நிலையான எழுத்து buf[அளவு "UNKNOWN (0x%lx)" - 3
                        + (அளவு type * CHAR_BIT + 3) / 4];
        sprintf (buf, "UNKNOWN (0x%lx)", type);
        திரும்பு buf;
      }
    }
#endif
}

NODISCARD
நிலையான எழுத்து *
human_access (கட்டமைப்பு stat மாறிலி *statbuf)
{
  நிலையான எழுத்து modebuf[12];
  filemodestring (statbuf, modebuf);
  modebuf[10] = 0;
  திரும்பு modebuf;
}

NODISCARD
நிலையான எழுத்து *
human_time (கட்டமைப்பு timespec t)
{
  /* STR must be at least INT_BUFSIZE_BOUND (intmax_t) big, either
     because localtime_rz fails, or because the time zone is truly
     outlandish so that %z expands to a long string.  */
  நிலையான எழுத்து str[INT_BUFSIZE_BOUND (intmax_t)
                  + INT_STRLEN_BOUND (முழு) /* YYYY */
                  + 1 /* because YYYY might equal INT_MAX + 1900 */
                  + அளவு "-MM-DD HH:MM:SS.NNNNNNNNN +"];
  நிலையான timezone_t tz;
  எனில் (!tz)
    tz = tzalloc (getenv ("TZ"));
  கட்டமைப்பு tm tm;
  முழு ns = t.tv_nsec;
  எனில் (localtime_rz (tz, &t.tv_sec, &tm))
    nstrftime (str, அளவு str, "%Y-%m-%d %H:%M:%S.%N %z", &tm, tz, ns);
  இல்லையெனில்
    {
      எழுத்து secbuf[INT_BUFSIZE_BOUND (intmax_t)];
      sprintf (str, "%s.%09d", timetostr (t.tv_sec, secbuf), ns);
    }
  திரும்பு str;
}

/* PFORMAT points to a '%' followed by a prefix of a format, all of
   size PREFIX_LEN.  The flags allowed for this format are
   ALLOWED_FLAGS; remove other printf flags from the prefix, then
   append SUFFIX.  */
நிலையான வெறுமை
make_format (எழுத்து *pformat, size_t prefix_len, எழுத்து மாறிலி *allowed_flags,
             எழுத்து மாறிலி *suffix)
{
  எழுத்து *dst = pformat + 1;
  எழுத்து மாறிலி *src;
  எழுத்து மாறிலி *srclim = pformat + prefix_len;
  க்கு (src = dst; src < srclim && strchr (printf_flags, *src); src++)
    எனில் (strchr (allowed_flags, *src))
      *dst++ = *src;
  போது (src < srclim)
    *dst++ = *src++;
  strcpy (dst, suffix);
}

நிலையான வெறுமை
out_string (எழுத்து *pformat, size_t prefix_len, எழுத்து மாறிலி *arg)
{
  make_format (pformat, prefix_len, "-", "s");
  printf (pformat, arg);
}
நிலையான முழு
out_int (எழுத்து *pformat, size_t prefix_len, intmax_t arg)
{
  make_format (pformat, prefix_len, "'-+ 0", "jd");
  திரும்பு printf (pformat, arg);
}
நிலையான முழு
out_uint (எழுத்து *pformat, size_t prefix_len, uintmax_t arg)
{
  make_format (pformat, prefix_len, "'-0", "ju");
  திரும்பு printf (pformat, arg);
}
நிலையான வெறுமை
out_uint_o (எழுத்து *pformat, size_t prefix_len, uintmax_t arg)
{
  make_format (pformat, prefix_len, "-#0", "jo");
  printf (pformat, arg);
}
நிலையான வெறுமை
out_uint_x (எழுத்து *pformat, size_t prefix_len, uintmax_t arg)
{
  make_format (pformat, prefix_len, "-#0", "jx");
  printf (pformat, arg);
}
நிலையான முழு
out_minus_zero (எழுத்து *pformat, size_t prefix_len)
{
  make_format (pformat, prefix_len, "'-+ 0", ".0f");
  திரும்பு printf (pformat, -0.25);
}

/* Output the number of seconds since the Epoch, using a format that
   acts like printf's %f format.  */
நிலையான வெறுமை
out_epoch_sec (எழுத்து *pformat, size_t prefix_len,
               கட்டமைப்பு timespec arg)
{
  எழுத்து *dot = memchr (pformat, '.', prefix_len);
  size_t sec_prefix_len = prefix_len;
  முழு width = 0;
  முழு precision = 0;
  bool frac_இடது_adjust = false;

  எனில் (dot)
    {
      sec_prefix_len = dot - pformat;
      pformat[prefix_len] = '\0';

      எனில் (c_isdigit (dot[1]))
        {
          long முழு lprec = strtol (dot + 1, வெற்று, 10);
          precision = (lprec <= INT_MAX ? lprec : INT_MAX);
        }
      இல்லையெனில்
        {
          precision = 9;
        }

      எனில் (precision && c_isdigit (dot[-1]))
        {
          /* If a nontrivial width is given, subtract the width of the
             decimal point and PRECISION digits that will be output
             later.  */
          எழுத்து *p = dot;
          *dot = '\0';

          செய்
            --p;
          போது (c_isdigit (p[-1]));

          long முழு lwidth = strtol (p, வெற்று, 10);
          width = (lwidth <= INT_MAX ? lwidth : INT_MAX);
          எனில் (1 < width)
            {
              p += (*p == '0');
              sec_prefix_len = p - pformat;
              முழு w_d = (decimal_point_len < width
                         ? width - decimal_point_len
                         : 0);
              எனில் (1 < w_d)
                {
                  முழு w = w_d - precision;
                  எனில் (1 < w)
                    {
                      எழுத்து *dst = pformat;
                      க்கு (எழுத்து மாறிலி *src = dst; src < p; src++)
                        {
                          எனில் (*src == '-')
                            frac_இடது_adjust = true;
                          இல்லையெனில்
                            *dst++ = *src;
                        }
                      sec_prefix_len =
                        (dst - pformat
                         + (frac_இடது_adjust ? 0 : sprintf (dst, "%d", w)));
                    }
                }
            }
        }
    }

  முழு வகுஎண் = 1;
  க்கு (முழு i = precision; i < 9; i++)
    வகுஎண் *= 10;
  முழு frac_sec = arg.tv_nsec / வகுஎண்;
  முழு int_len;

  எனில் (TYPE_SIGNED (time_t))
    {
      bool minus_zero = false;
      எனில் (arg.tv_sec < 0 && arg.tv_nsec != 0)
        {
          முழு frac_sec_modulus = 1000000000 / வகுஎண்;
          frac_sec = (frac_sec_modulus - frac_sec
                      - (arg.tv_nsec % வகுஎண் != 0));
          arg.tv_sec += (frac_sec != 0);
          minus_zero = (arg.tv_sec == 0);
        }
      int_len = (minus_zero
                 ? out_minus_zero (pformat, sec_prefix_len)
                 : out_int (pformat, sec_prefix_len, arg.tv_sec));
    }
  இல்லையெனில்
    int_len = out_uint (pformat, sec_prefix_len, arg.tv_sec);

  எனில் (precision)
    {
      முழு prec = (precision < 9 ? precision : 9);
      முழு trailing_prec = precision - prec;
      முழு ilen = (int_len < 0 ? 0 : int_len);
      முழு trailing_width = (ilen < width && decimal_point_len < width - ilen
                            ? width - ilen - decimal_point_len - prec
                            : 0);
      printf ("%s%.*d%-*.*d", decimal_point, prec, frac_sec,
              trailing_width, trailing_prec, 0);
    }
}

/* Print the context information of FILENAME, and return true iff the
   context could not be obtained.  */
NODISCARD
நிலையான bool
out_file_context (எழுத்து *pformat, size_t prefix_len, எழுத்து மாறிலி *filename)
{
  எழுத்து *scontext;
  bool fail = false;

  எனில் ((follow_links
       ? getfilecon (filename, &scontext)
       : lgetfilecon (filename, &scontext)) < 0)
    {
      error (0, errno, _("failed to get security context of %s"),
             quoteaf (filename));
      scontext = வெற்று;
      fail = true;
    }
  strcpy (pformat + prefix_len, "s");
  printf (pformat, (scontext ? scontext : "?"));
  எனில் (scontext)
    freecon (scontext);
  திரும்பு fail;
}

/* Set the quoting style once it is needed.  */
நிலையான வெறுமை
initialize_quoting_style (வெறுமை)
{
  நிலையான bool initialized;
  எனில் (!initialized)
    {
      initialized = true;
      முழு qs = getenv_quoting_style ();
      set_quoting_style (வெற்று, 0 <= qs ? qs : shell_escape_quoting_style);
    }
}

/* Print statfs info.  Return zero upon success, nonzero upon failure.  */
NODISCARD
நிலையான bool
print_statfs (எழுத்து *pformat, size_t prefix_len, MAYBE_UNUSED எழுத்து mod, எழுத்து m,
              MAYBE_UNUSED முழு fd, எழுத்து மாறிலி *filename,
              வெறுமை மாறிலி *data)
{
  STRUCT_STATVFS மாறிலி *statfsbuf = data;
  bool fail = false;

  தேர்வு (m)
    {
    நிலை 'n':
      எனில் (mod == 'Q')
        {
          initialize_quoting_style ();
          filename = quoteN (filename);
        }
      out_string (pformat, prefix_len, filename);
      நிறுத்து;

    நிலை 'i':
      {
#எனில் STRUCT_STATXFS_F_FSID_IS_INTEGER
        uintmax_t fsid = statfsbuf->f_fsid;
#இல்லையெனில்
        வகைவரை unsigned முழு fsid_word;
        static_assert (alignof (STRUCT_STATVFS) % alignof (fsid_word) == 0);
        static_assert (offsetof (STRUCT_STATVFS, f_fsid) % alignof (fsid_word)
                       == 0);
        static_assert (அளவு statfsbuf->f_fsid % alignof (fsid_word) == 0);
        fsid_word மாறிலி *p = (fsid_word *) &statfsbuf->f_fsid;

        /* Assume a little-endian word order, as that is compatible
           with glibc's statvfs implementation.  */
        uintmax_t fsid = 0;
        முழு சொற்கள் = அளவு statfsbuf->f_fsid / அளவு *p;
        க்கு (முழு i = 0; i < சொற்கள் && i * அளவு *p < அளவு fsid; i++)
          {
            uintmax_t u = p[சொற்கள் - 1 - i];
            fsid |= u << (i * CHAR_BIT * அளவு *p);
          }
#endif
        out_uint_x (pformat, prefix_len, fsid);
      }
      நிறுத்து;

    நிலை 'l':
      OUT_NAMEMAX (pformat, prefix_len, SB_F_NAMEMAX (statfsbuf));
      நிறுத்து;
    நிலை 't':
#எனில் HAVE_STRUCT_STATXFS_F_TYPE
      out_uint_x (pformat, prefix_len, statfsbuf->f_type);
#இல்லையெனில்
      fputc ('?', stdout);
#endif
      நிறுத்து;
    நிலை 'T':
      out_string (pformat, prefix_len, human_fstype (statfsbuf));
      நிறுத்து;
    நிலை 'b':
      out_int (pformat, prefix_len, statfsbuf->f_blocks);
      நிறுத்து;
    நிலை 'f':
      out_int (pformat, prefix_len, statfsbuf->f_bfree);
      நிறுத்து;
    நிலை 'a':
      out_int (pformat, prefix_len, statfsbuf->f_bavail);
      நிறுத்து;
    நிலை 's':
      out_uint (pformat, prefix_len, statfsbuf->f_bsize);
      நிறுத்து;
    நிலை 'S':
      {
        uintmax_t frsize = STATFS_FRSIZE (statfsbuf);
        எனில் (! frsize)
          frsize = statfsbuf->f_bsize;
        out_uint (pformat, prefix_len, frsize);
      }
      நிறுத்து;
    நிலை 'c':
      out_uint (pformat, prefix_len, statfsbuf->f_files);
      நிறுத்து;
    நிலை 'd':
      out_int (pformat, prefix_len, statfsbuf->f_ffree);
      நிறுத்து;
    இயல்பு:
      fputc ('?', stdout);
      நிறுத்து;
    }
  திரும்பு fail;
}

/* Return any bind mounted source for a path.
   The caller should not free the returned buffer.
   Return NULL if no bind mount found.  */
NODISCARD
நிலையான எழுத்து மாறிலி *
find_bind_mount (எழுத்து மாறிலி * name)
{
  எழுத்து மாறிலி * bind_mount = வெற்று;

  நிலையான கட்டமைப்பு mount_entry *mount_list;
  நிலையான bool tried_mount_list = false;
  எனில் (!tried_mount_list) /* attempt/warn once per process.  */
    {
      எனில் (!(mount_list = read_file_system_list (false)))
        error (0, errno, "%s", _("cannot read table of mounted file systems"));
      tried_mount_list = true;
    }

  கட்டமைப்பு stat name_stats;
  எனில் (stat (name, &name_stats) != 0)
    திரும்பு வெற்று;

  க்கு (கட்டமைப்பு mount_entry *me = mount_list; me; me = me->me_next)
    {
      எனில் (me->me_dummy && me->me_devname[0] == '/'
          && streq (me->me_mountdir, name))
        {
          கட்டமைப்பு stat dev_stats;

          எனில் (stat (me->me_devname, &dev_stats) == 0
              && psame_inode (&name_stats, &dev_stats))
            {
              bind_mount = me->me_devname;
              நிறுத்து;
            }
        }
    }

  திரும்பு bind_mount;
}

/* Print mount point.  Return zero upon success, nonzero upon failure.  */
NODISCARD
நிலையான bool
out_mount_point (எழுத்து மாறிலி *filename, எழுத்து *pformat, size_t prefix_len,
                 மாறிலி கட்டமைப்பு stat *statp)
{

  எழுத்து மாறிலி *np = "?", *bp = வெற்று;
  எழுத்து *mp = வெற்று;
  bool fail = true;

  /* Look for bind mounts first.  Note we output the immediate alias,
     rather than further resolving to a base device mount point.  */
  எனில் (follow_links || !S_ISLNK (statp->st_mode))
    {
      எழுத்து *resolved = canonicalize_file_name (filename);
      எனில் (!resolved)
        {
          error (0, errno, _("failed to canonicalize %s"), quoteaf (filename));
          செல் print_mount_point;
        }
      bp = find_bind_mount (resolved);
      விடுவி (resolved);
      எனில் (bp)
        {
          fail = false;
          செல் print_mount_point;
        }
    }

  /* If there is no direct bind mount, then navigate
     back up the tree looking for a device change.
     Note we don't detect if any of the directory components
     are bind mounted to the same device, but that's OK
     since we've not directly queried them.  */
  எனில் ((mp = find_mount_point (filename, statp)))
    {
      /* This dir might be bind mounted to another device,
         so we resolve the bound source in that case also.  */
      bp = find_bind_mount (mp);
      fail = false;
    }

print_mount_point:

  out_string (pformat, prefix_len, bp ? bp : mp ? mp : np);
  விடுவி (mp);
  திரும்பு fail;
}

/* Map a TS with negative TS.tv_nsec to {0,0}.  */
நிலையான உள்வரி கட்டமைப்பு timespec
neg_to_zero (கட்டமைப்பு timespec ts)
{
  எனில் (0 <= ts.tv_nsec)
    திரும்பு ts;
  கட்டமைப்பு timespec z = {0};
  திரும்பு z;
}

/* Output a single-character \ escape.  */

நிலையான வெறுமை
print_esc_char (எழுத்து c)
{
  தேர்வு (c)
    {
    நிலை 'a':			/* Alert. */
      c ='\a';
      நிறுத்து;
    நிலை 'b':			/* Backspace. */
      c ='\b';
      நிறுத்து;
    நிலை 'e':			/* Escape. */
      c ='\x1B';
      நிறுத்து;
    நிலை 'f':			/* Form feed. */
      c ='\f';
      நிறுத்து;
    நிலை 'n':			/* New line. */
      c ='\n';
      நிறுத்து;
    நிலை 'r':			/* Carriage return. */
      c ='\r';
      நிறுத்து;
    நிலை 't':			/* Horizontal tab. */
      c ='\t';
      நிறுத்து;
    நிலை 'v':			/* Vertical tab. */
      c ='\v';
      நிறுத்து;
    நிலை '"':
    நிலை '\\':
      நிறுத்து;
    இயல்பு:
      error (0, 0, _("warning: unrecognized escape '\\%c'"), c);
      நிறுத்து;
    }
  putchar (c);
}

ATTRIBUTE_PURE
நிலையான size_t
format_code_offset (எழுத்து மாறிலி *directive)
{
  size_t len = strspn (directive + 1, printf_flags);
  எழுத்து மாறிலி *fmt_char = directive + len + 1;
  fmt_char += strspn (fmt_char, digits);
  எனில் (*fmt_char == '.')
    fmt_char += 1 + strspn (fmt_char + 1, digits);
  திரும்பு fmt_char - directive;
}

/* Print the information specified by the format string, FORMAT,
   calling PRINT_FUNC for each %-directive encountered.
   Return zero upon success, nonzero upon failure.  */
NODISCARD
நிலையான bool
print_it (எழுத்து மாறிலி *format, முழு fd, எழுத்து மாறிலி *filename,
          bool (*print_func) (எழுத்து *, size_t, எழுத்து, எழுத்து,
                              முழு, எழுத்து மாறிலி *, வெறுமை மாறிலி *),
          வெறுமை மாறிலி *data)
{
  bool fail = false;

  /* Add 2 to accommodate our conversion of the stat '%s' format string
     to the longer printf '%llu' one.  */
  எண்ணிக்கை
    {
      MAX_ADDITIONAL_BYTES =
        (MAX (அளவு "jd",
              MAX (அளவு "jo", MAX (அளவு "ju", அளவு "jx")))
         - 1)
    };
  size_t n_alloc = strlen (format) + MAX_ADDITIONAL_BYTES + 1;
  எழுத்து *dest = xmalloc (n_alloc);
  க்கு (எழுத்து மாறிலி *b = format; *b; b++)
    {
      தேர்வு (*b)
        {
        நிலை '%':
          {
            size_t len = format_code_offset (b);
            எழுத்து fmt_char = *(b + len);
            எழுத்து mod_char = 0;
            memcpy (dest, b, len);
            b += len;

            தேர்வு (fmt_char)
              {
              நிலை '\0':
                --b;
                FALLTHROUGH;
              நிலை '%':
                எனில் (1 < len)
                  {
                    dest[len] = fmt_char;
                    dest[len + 1] = '\0';
                    error (EXIT_FAILURE, 0, _("%s: invalid directive"),
                           quote (dest));
                  }
                putchar ('%');
                நிறுத்து;
              நிலை 'H':
              நிலை 'L':
              நிலை 'Q':
                mod_char = fmt_char;
                fmt_char = *(b + 1);
                எனில் ((mod_char == 'Q' && fmt_char == 'n')
                    || (print_func == print_stat
                        && (mod_char == 'H' || mod_char == 'L')
                        && (fmt_char == 'd' || fmt_char == 'r')))
                  {
                    b++;
                  }
                இல்லையெனில்
                  {
                    fmt_char = mod_char;
                    mod_char = 0;
                  }
                FALLTHROUGH;
              இயல்பு:
                fail |= print_func (dest, len, mod_char, fmt_char,
                                    fd, filename, data);
                நிறுத்து;
              }
            நிறுத்து;
          }

        நிலை '\\':
          எனில் ( ! interpret_backslash_escapes)
            {
              putchar ('\\');
              நிறுத்து;
            }
          ++b;
          எனில் (isoct (*b))
            {
              முழு esc_மதிப்பு = fromoct (*b);
              முழு esc_length = 1;	/* number of octal digits */
              க்கு (++b; esc_length < 3 && isoct (*b);
                   ++esc_length, ++b)
                {
                  esc_மதிப்பு = esc_மதிப்பு * 8 + fromoct (*b);
                }
              putchar (esc_மதிப்பு);
              --b;
            }
          இல்லையெனில் எனில் (*b == 'x' && c_isxdigit (b[1]))
            {
              முழு esc_மதிப்பு = fromhex (b[1]);	/* Value of \xhh escape. */
              /* A hexadecimal \xhh escape sequence must have
                 1 or 2 hex. digits.  */
              ++b;
              எனில் (c_isxdigit (b[1]))
                {
                  ++b;
                  esc_மதிப்பு = esc_மதிப்பு * 16 + fromhex (*b);
                }
              putchar (esc_மதிப்பு);
            }
          இல்லையெனில் எனில் (*b == '\0')
            {
              error (0, 0, _("warning: backslash at end of format"));
              putchar ('\\');
              /* Arrange to exit the loop.  */
              --b;
            }
          இல்லையெனில்
            {
              print_esc_char (*b);
            }
          நிறுத்து;

        இயல்பு:
          putchar (*b);
          நிறுத்து;
        }
    }
  விடுவி (dest);

  fputs (trailing_delim, stdout);

  திரும்பு fail;
}

/* Stat the file system and print what we find.  */
NODISCARD
நிலையான bool
do_statfs (எழுத்து மாறிலி *filename, எழுத்து மாறிலி *format)
{
  STRUCT_STATVFS statfsbuf;

  எனில் (streq (filename, "-"))
    {
      error (0, 0, _("using %s to denote standard input does not work"
                     " in file system mode"), quoteaf (filename));
      திரும்பு false;
    }

  எனில் (STATFS (filename, &statfsbuf) != 0)
    {
      error (0, errno, _("cannot read file system information for %s"),
             quoteaf (filename));
      திரும்பு false;
    }

  bool fail = print_it (format, -1, filename, print_statfs, &statfsbuf);
  திரும்பு ! fail;
}

கட்டமைப்பு print_args {
  கட்டமைப்பு stat *st;
  கட்டமைப்பு timespec btime;
};

/* Ask statx to avoid syncing? */
நிலையான bool dont_sync;

/* Ask statx to force sync? */
நிலையான bool force_sync;

#எனில் USE_STATX
நிலையான unsigned முழு
fmt_to_mask (எழுத்து fmt)
{
  தேர்வு (fmt)
    {
    நிலை 'N':
      திரும்பு STATX_MODE;
    நிலை 'd':
    நிலை 'D':
      திரும்பு STATX_MODE;
    நிலை 'i':
      திரும்பு STATX_INO;
    நிலை 'a':
    நிலை 'A':
      திரும்பு STATX_MODE;
    நிலை 'f':
      திரும்பு STATX_MODE|STATX_TYPE;
    நிலை 'F':
      திரும்பு STATX_TYPE;
    நிலை 'h':
      திரும்பு STATX_NLINK;
    நிலை 'u':
    நிலை 'U':
      திரும்பு STATX_UID;
    நிலை 'g':
    நிலை 'G':
      திரும்பு STATX_GID;
    நிலை 'm':
      திரும்பு STATX_MODE|STATX_INO;
    நிலை 's':
      திரும்பு STATX_SIZE;
    நிலை 't':
    நிலை 'T':
      திரும்பு STATX_MODE;
    நிலை 'b':
      திரும்பு STATX_BLOCKS;
    நிலை 'w':
    நிலை 'W':
      திரும்பு STATX_BTIME;
    நிலை 'x':
    நிலை 'X':
      திரும்பு STATX_ATIME;
    நிலை 'y':
    நிலை 'Y':
      திரும்பு STATX_MTIME;
    நிலை 'z':
    நிலை 'Z':
      திரும்பு STATX_CTIME;
    }
  திரும்பு 0;
}

ATTRIBUTE_PURE
நிலையான unsigned முழு
format_to_mask (எழுத்து மாறிலி *format)
{
  unsigned முழு mask = 0;

  க்கு (எழுத்து மாறிலி *b = format; *b; b++)
    {
      எனில் (*b != '%')
        தொடர்;

      b += format_code_offset (b);
      எனில் (*b == '\0')
        நிறுத்து;
      mask |= fmt_to_mask (*b);
    }
  திரும்பு mask;
}

/* statx the file and print what we find */
NODISCARD
நிலையான bool
do_stat (எழுத்து மாறிலி *filename, எழுத்து மாறிலி *format, எழுத்து மாறிலி *format2)
{
  முழு fd = streq (filename, "-") ? 0 : AT_FDCWD;
  முழு flags = 0;
  கட்டமைப்பு stat st;
  கட்டமைப்பு statx stx = {0};
  எழுத்து மாறிலி *pathname = filename;
  கட்டமைப்பு print_args pa;
  pa.st = &st;
  pa.btime = (கட்டமைப்பு timespec) {.tv_sec = -1, .tv_nsec = -1};

  எனில் (AT_FDCWD != fd)
    {
      pathname = "";
      flags = AT_EMPTY_PATH;
    }
  இல்லையெனில் எனில் (!follow_links)
    {
      flags = AT_SYMLINK_NOFOLLOW;
    }

  எனில் (dont_sync)
    flags |= AT_STATX_DONT_SYNC;
  இல்லையெனில் எனில் (force_sync)
    flags |= AT_STATX_FORCE_SYNC;

  எனில் (! force_sync)
    flags |= AT_NO_AUTOMOUNT;

  fd = statx (fd, pathname, flags, format_to_mask (format), &stx);
  எனில் (fd < 0)
    {
      எனில் (flags & AT_EMPTY_PATH)
        error (0, errno, _("cannot stat standard input"));
      இல்லையெனில்
        error (0, errno, _("cannot statx %s"), quoteaf (filename));
      திரும்பு false;
    }

  எனில் (S_ISBLK (stx.stx_mode) || S_ISCHR (stx.stx_mode))
    format = format2;

  statx_to_stat (&stx, &st);
  எனில் (stx.stx_mask & STATX_BTIME)
    pa.btime = statx_timestamp_to_timespec (stx.stx_btime);

  bool fail = print_it (format, fd, filename, print_stat, &pa);
  திரும்பு ! fail;
}

#இல்லையெனில் /* USE_STATX */

நிலையான கட்டமைப்பு timespec
get_birthtime (முழு fd, எழுத்து மாறிலி *filename, கட்டமைப்பு stat மாறிலி *st)
{
  கட்டமைப்பு timespec ts = get_stat_birthtime (st);

# எனில் HAVE_GETATTRAT
  எனில் (ts.tv_nsec < 0)
    {
      nvlist_t *response;
      எனில் ((fd < 0
           ? getattrat (AT_FDCWD, XATTR_VIEW_READWRITE, filename, &response)
           : fgetattr (fd, XATTR_VIEW_READWRITE, &response))
          == 0)
        {
          uint64_t *val;
          uint_t n;
          எனில் (nvlist_lookup_uint64_array (response, A_CRTIME, &val, &n) == 0
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

  திரும்பு ts;
}


/* stat the file and print what we find */
NODISCARD
நிலையான bool
do_stat (எழுத்து மாறிலி *filename, எழுத்து மாறிலி *format,
         எழுத்து மாறிலி *format2)
{
  முழு fd = streq (filename, "-") ? 0 : -1;
  கட்டமைப்பு stat statbuf;
  கட்டமைப்பு print_args pa;
  pa.st = &statbuf;
  pa.btime = (கட்டமைப்பு timespec) {.tv_sec = -1, .tv_nsec = -1};

  எனில் (0 <= fd)
    {
      எனில் (fstat (fd, &statbuf) != 0)
        {
          error (0, errno, _("cannot stat standard input"));
          திரும்பு false;
        }
    }
  /* We can't use the shorter
     (follow_links?stat:lstat) (filename, &statbug)
     since stat might be a function-like macro.  */
  இல்லையெனில் எனில் ((follow_links
            ? stat (filename, &statbuf)
            : lstat (filename, &statbuf)) != 0)
    {
      error (0, errno, _("cannot stat %s"), quoteaf (filename));
      திரும்பு false;
    }

  எனில் (S_ISBLK (statbuf.st_mode) || S_ISCHR (statbuf.st_mode))
    format = format2;

  bool fail = print_it (format, fd, filename, print_stat, &pa);
  திரும்பு ! fail;
}
#endif /* USE_STATX */

/* POSIX requires 'ls' to print file sizes without a sign, even
   when negative.  Be consistent with that.  */

நிலையான uintmax_t
unsigned_file_size (off_t size)
{
  திரும்பு size + (size < 0) * ((uintmax_t) OFF_T_MAX - OFF_T_MIN + 1);
}

/* Print stat info.  Return zero upon success, nonzero upon failure.  */
நிலையான bool
print_stat (எழுத்து *pformat, size_t prefix_len, எழுத்து mod, எழுத்து m,
            MAYBE_UNUSED முழு fd, எழுத்து மாறிலி *filename, வெறுமை மாறிலி *data)
{
  கட்டமைப்பு print_args *parg = (கட்டமைப்பு print_args *) data;
  கட்டமைப்பு stat *statbuf = parg->st;
  கட்டமைப்பு timespec btime = parg->btime;
  கட்டமைப்பு passwd *pw_ent;
  கட்டமைப்பு group *gw_ent;
  bool fail = false;

  தேர்வு (m)
    {
    நிலை 'n':
      எனில் (mod == 'Q')
        {
          initialize_quoting_style ();
          filename = quoteN (filename);
        }
      out_string (pformat, prefix_len, filename);
      நிறுத்து;
    நிலை 'N':
      initialize_quoting_style ();
      out_string (pformat, prefix_len, quoteN (filename));
      எனில் (S_ISLNK (statbuf->st_mode))
        {
          எழுத்து *linkname = areadlink_with_size (filename, statbuf->st_size);
          எனில் (linkname == வெற்று)
            {
              error (0, errno, _("cannot read symbolic link %s"),
                     quoteaf (filename));
              திரும்பு true;
            }
          printf (" -> ");
          out_string (pformat, prefix_len, quoteN (linkname));
          விடுவி (linkname);
        }
      நிறுத்து;
    நிலை 'd':
      எனில் (mod == 'H')
        out_uint (pformat, prefix_len, major (statbuf->st_dev));
      இல்லையெனில் எனில் (mod == 'L')
        out_uint (pformat, prefix_len, minor (statbuf->st_dev));
      இல்லையெனில்
        out_uint (pformat, prefix_len, statbuf->st_dev);
      நிறுத்து;
    நிலை 'D':
      out_uint_x (pformat, prefix_len, statbuf->st_dev);
      நிறுத்து;
    நிலை 'i':
      out_uint (pformat, prefix_len, statbuf->st_ino);
      நிறுத்து;
    நிலை 'a':
      out_uint_o (pformat, prefix_len, statbuf->st_mode & CHMOD_MODE_BITS);
      நிறுத்து;
    நிலை 'A':
      out_string (pformat, prefix_len, human_access (statbuf));
      நிறுத்து;
    நிலை 'f':
      out_uint_x (pformat, prefix_len, statbuf->st_mode);
      நிறுத்து;
    நிலை 'F':
      out_string (pformat, prefix_len, file_type (statbuf));
      நிறுத்து;
    நிலை 'h':
      out_uint (pformat, prefix_len, statbuf->st_nlink);
      நிறுத்து;
    நிலை 'u':
      out_uint (pformat, prefix_len, statbuf->st_uid);
      நிறுத்து;
    நிலை 'U':
      pw_ent = getpwuid (statbuf->st_uid);
      out_string (pformat, prefix_len,
                  pw_ent ? pw_ent->pw_name : "UNKNOWN");
      நிறுத்து;
    நிலை 'g':
      out_uint (pformat, prefix_len, statbuf->st_gid);
      நிறுத்து;
    நிலை 'G':
      gw_ent = getgrgid (statbuf->st_gid);
      out_string (pformat, prefix_len,
                  gw_ent ? gw_ent->gr_name : "UNKNOWN");
      நிறுத்து;
    நிலை 'm':
      fail |= out_mount_point (filename, pformat, prefix_len, statbuf);
      நிறுத்து;
    நிலை 's':
      out_uint (pformat, prefix_len, unsigned_file_size (statbuf->st_size));
      நிறுத்து;
    நிலை 'r':
      எனில் (mod == 'H')
        out_uint (pformat, prefix_len, major (statbuf->st_rdev));
      இல்லையெனில் எனில் (mod == 'L')
        out_uint (pformat, prefix_len, minor (statbuf->st_rdev));
      இல்லையெனில்
        out_uint (pformat, prefix_len, statbuf->st_rdev);
      நிறுத்து;
    நிலை 'R':
      out_uint_x (pformat, prefix_len, statbuf->st_rdev);
      நிறுத்து;
    நிலை 't':
      out_uint_x (pformat, prefix_len, major (statbuf->st_rdev));
      நிறுத்து;
    நிலை 'T':
      out_uint_x (pformat, prefix_len, minor (statbuf->st_rdev));
      நிறுத்து;
    நிலை 'B':
      out_uint (pformat, prefix_len, ST_NBLOCKSIZE);
      நிறுத்து;
    நிலை 'b':
      out_uint (pformat, prefix_len, STP_NBLOCKS (statbuf));
      நிறுத்து;
    நிலை 'o':
      out_uint (pformat, prefix_len, STP_BLKSIZE (statbuf));
      நிறுத்து;
    நிலை 'w':
      {
#எனில் ! USE_STATX
        btime = get_birthtime (fd, filename, statbuf);
#endif
        எனில் (btime.tv_nsec < 0)
          out_string (pformat, prefix_len, "-");
        இல்லையெனில்
          out_string (pformat, prefix_len, human_time (btime));
      }
      நிறுத்து;
    நிலை 'W':
      {
#எனில் ! USE_STATX
        btime = get_birthtime (fd, filename, statbuf);
#endif
        out_epoch_sec (pformat, prefix_len, neg_to_zero (btime));
      }
      நிறுத்து;
    நிலை 'x':
      out_string (pformat, prefix_len, human_time (get_stat_atime (statbuf)));
      நிறுத்து;
    நிலை 'X':
      out_epoch_sec (pformat, prefix_len, get_stat_atime (statbuf));
      நிறுத்து;
    நிலை 'y':
      out_string (pformat, prefix_len, human_time (get_stat_mtime (statbuf)));
      நிறுத்து;
    நிலை 'Y':
      out_epoch_sec (pformat, prefix_len, get_stat_mtime (statbuf));
      நிறுத்து;
    நிலை 'z':
      out_string (pformat, prefix_len, human_time (get_stat_ctime (statbuf)));
      நிறுத்து;
    நிலை 'Z':
      out_epoch_sec (pformat, prefix_len, get_stat_ctime (statbuf));
      நிறுத்து;
    நிலை 'C':
      fail |= out_file_context (pformat, prefix_len, filename);
      நிறுத்து;
    இயல்பு:
      fputc ('?', stdout);
      நிறுத்து;
    }
  திரும்பு fail;
}

/* Return an allocated format string in static storage that
   corresponds to whether FS and TERSE options were declared.  */
நிலையான எழுத்து *
default_format (bool fs, bool terse, bool device)
{
  எழுத்து *format;
  எனில் (fs)
    {
      எனில் (terse)
        format = xstrdup (fmt_terse_fs);
      இல்லையெனில்
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
  இல்லையெனில் /* ! fs */
    {
      எனில் (terse)
        {
          எனில் (0 < is_selinux_enabled ())
            format = xstrdup (fmt_terse_selinux);
          இல்லையெனில்
            format = xstrdup (fmt_terse_regular);
        }
      இல்லையெனில்
        {
          எழுத்து *temp;
          /* TRANSLATORS: This string uses format specifiers from
             'stat --help' without --file-system, and NOT from printf.  */
          format = xstrdup (_("\
  File: %N\n\
  Size: %-10s\tBlocks: %-10b IO Block: %-6o %F\n\
"));

          temp = format;
          எனில் (device)
            {
              /* TRANSLATORS: This string uses format specifiers from
                 'stat --help' without --file-system, and NOT from printf.  */
              format = xasprintf ("%s%s", format, _("\
" "Device: %Hd,%Ld\tInode: %-10i  Links: %-5h Device type: %Hr,%Lr\n\
"));
            }
          இல்லையெனில்
            {
              /* TRANSLATORS: This string uses format specifiers from
                 'stat --help' without --file-system, and NOT from printf.  */
              format = xasprintf ("%s%s", format, _("\
" "Device: %Hd,%Ld\tInode: %-10i  Links: %h\n\
"));
            }
          விடுவி (temp);

          temp = format;
          /* TRANSLATORS: This string uses format specifiers from
             'stat --help' without --file-system, and NOT from printf.  */
          format = xasprintf ("%s%s", format, _("\
" "Access: (%04a/%10.10A)  Uid: (%5u/%8U)   Gid: (%5g/%8G)\n\
"));
          விடுவி (temp);

          எனில் (0 < is_selinux_enabled ())
            {
              temp = format;
              /* TRANSLATORS: This string uses format specifiers from
                 'stat --help' without --file-system, and NOT from printf.  */
              format = xasprintf ("%s%s", format, _("Context: %C\n"));
              விடுவி (temp);
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
          விடுவி (temp);
        }
    }
  திரும்பு format;
}

வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
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
         use the specified FORMAT instead of the இயல்பு;\n\
         output a newline after each use of FORMAT\n\
"));
      oputs (_("\
      --printf=FORMAT\n\
         like --format, but interpret backslash escapes,\n\
         and செய் not output a mandatory trailing newline;\n\
         எனில் you want a newline, include \\n in FORMAT\n\
"));
      oputs (_("\
  -t, --terse\n\
         print the information in terse form\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);

      fputs (_("\n\
The MODE argument of --cached can be: always, never, or இயல்பு.\n\
'always' will use cached attributes எனில் available, போது\n\
'never' will try to synchronize with the latest attributes, and\n\
'default' will leave it up to the underlying file system.\n\
"), stdout);

      fputs (_("\n\
The valid format sequences க்கு files (without --file-system):\n\
\n\
  %a   permission bits in octal (see '#' and '0' printf flags)\n\
  %A   permission bits and file type in human readable form\n\
  %b   எண் of blocks allocated (see %B)\n\
  %B   the size in bytes of each block reported by %b\n\
  %C   SELinux security context string\n\
"), stdout);
      fputs (_("\
  %d   device எண் in decimal (st_dev)\n\
  %D   device எண் in hex (st_dev)\n\
  %Hd  major device எண் in decimal\n\
  %Ld  minor device எண் in decimal\n\
  %f   raw mode in hex\n\
  %F   file type\n\
  %g   group ID of owner\n\
  %G   group name of owner\n\
"), stdout);
      fputs (_("\
  %h   எண் of hard links\n\
  %i   inode எண்\n\
  %m   mount point\n\
  %n   file name\n\
  %Qn  quoted file name\n\
  %N   quoted file name with dereference எனில் symbolic link\n\
  %o   optimal I/O transfer size hint\n\
  %s   total size, in bytes\n\
  %r   device type in decimal (st_rdev)\n\
  %R   device type in hex (st_rdev)\n\
  %Hr  major device type in decimal, க்கு character/block device special files\n\
  %Lr  minor device type in decimal, க்கு character/block device special files\n\
  %t   major device type in hex, க்கு character/block device special files\n\
  %T   minor device type in hex, க்கு character/block device special files\n\
"), stdout);
      fputs (_("\
  %u   user ID of owner\n\
  %U   user name of owner\n\
  %w   time of file birth, human-readable; - எனில் unknown\n\
  %W   time of file birth, seconds since Epoch; 0 எனில் unknown\n\
  %x   time of last access, human-readable\n\
  %X   time of last access, seconds since Epoch\n\
  %y   time of last data modification, human-readable\n\
  %Y   time of last data modification, seconds since Epoch\n\
  %z   time of last status change, human-readable\n\
  %Z   time of last status change, seconds since Epoch\n\
\n\
"), stdout);

      fputs (_("\
Valid format sequences க்கு file systems:\n\
\n\
  %a   விடுவி blocks available to non-superuser\n\
  %b   total data blocks in file system\n\
  %c   total file nodes in file system\n\
  %d   விடுவி file nodes in file system\n\
  %f   விடுவி blocks in file system\n\
"), stdout);
      fputs (_("\
  %i   file system ID in hex\n\
  %l   maximum length of filenames\n\
  %n   file name\n\
  %Qn  quoted file name\n\
  %s   block size (க்கு faster transfers)\n\
  %S   fundamental block size (க்கு block counts)\n\
  %t   file system type in hex\n\
  %T   file system type in human readable form\n\
"), stdout);

      printf (_("\n\
--terse is equivalent to the following FORMAT:\n\
    %s\
"),
#எனில் HAVE_SELINUX_SELINUX_H
              fmt_terse_selinux
#இல்லையெனில்
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

முழு
முதன்மை (முழு argc, எழுத்து *argv[])
{
  முழு c;
  bool fs = false;
  bool terse = false;
  எழுத்து *format = வெற்று;
  எழுத்து *format2;
  bool ok = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  கட்டமைப்பு lconv மாறிலி *locale = localeconv ();
  decimal_point = (locale->decimal_point[0] ? locale->decimal_point : ".");
  decimal_point_len = strlen (decimal_point);

  atexit (close_stdout);

  போது ((c = getopt_long (argc, argv, "c:fLt", long_options, வெற்று)) != -1)
    {
      தேர்வு (c)
        {
        நிலை PRINTF_OPTION:
          format = optarg;
          interpret_backslash_escapes = true;
          trailing_delim = "";
          நிறுத்து;

        நிலை 'c':
          format = optarg;
          interpret_backslash_escapes = false;
          trailing_delim = "\n";
          நிறுத்து;

        நிலை 'L':
          follow_links = true;
          நிறுத்து;

        நிலை 'f':
          fs = true;
          நிறுத்து;

        நிலை 't':
          terse = true;
          நிறுத்து;

        நிலை 0:
          தேர்வு (XARGMATCH ("--cached", optarg, cached_args, cached_modes))
            {
              நிலை cached_never:
                force_sync = true;
                dont_sync = false;
                நிறுத்து;
              நிலை cached_always:
                force_sync = false;
                dont_sync = true;
                நிறுத்து;
              நிலை cached_default:
                force_sync = false;
                dont_sync = false;
            }
          நிறுத்து;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        இயல்பு:
          usage (EXIT_FAILURE);
        }
    }

  எனில் (argc == optind)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  எனில் (format)
    {
      format2 = format;
    }
  இல்லையெனில்
    {
      format = default_format (fs, terse, /* device= */ false);
      format2 = default_format (fs, terse, /* device= */ true);
    }

  க்கு (முழு i = optind; i < argc; i++)
    ok &= (fs
           ? do_statfs (argv[i], format)
           : do_stat (argv[i], format, format2));

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
