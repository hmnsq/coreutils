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

#അന്തര്ഭാവ <config.h>

/* Keep this conditional in sync with the similar conditional in
   ../m4/stat-prog.m4.  */
#എങ്കിൽ ((STAT_STATVFS || STAT_STATVFS64)                                       \
     && (HAVE_STRUCT_STATVFS_F_BASETYPE || HAVE_STRUCT_STATVFS_F_FSTYPENAME \
         || (! HAVE_STRUCT_STATFS_F_FSTYPENAME && HAVE_STRUCT_STATVFS_F_TYPE)))
# define USE_STATVFS 1
#അല്ലെങ്കിൽ
# define USE_STATVFS 0
#endif

#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <pwd.h>
#അന്തര്ഭാവ <grp.h>
#എങ്കിൽ USE_STATVFS
# include <sys/statvfs.h>
#അല്ലെങ്കിലെങ്കിൽ HAVE_SYS_VFS_H
# include <sys/vfs.h>
#അല്ലെങ്കിലെങ്കിൽ HAVE_SYS_MOUNT_H && HAVE_SYS_PARAM_H
/* NOTE: freebsd5.0 needs sys/param.h and sys/mount.h for statfs.
   It does have statvfs.h, but shouldn't use it, since it doesn't
   HAVE_STRUCT_STATVFS_F_BASETYPE.  So find a clean way to fix it.  */
/* NetBSD 1.5.2 needs these, for the declaration of struct statfs. */
# include <sys/param.h>
# include <sys/mount.h>
# എങ്കിൽ HAVE_NFS_NFS_CLNT_H && HAVE_NFS_VFS_H
/* Ultrix 4.4 needs these for the declaration of struct statfs.  */
#  include <netinet/in.h>
#  include <nfs/nfs_clnt.h>
#  include <nfs/vfs.h>
# endif
#അല്ലെങ്കിലെങ്കിൽ HAVE_OS_H /* BeOS */
# include <fs_info.h>
#endif
#അന്തര്ഭാവ <selinux/selinux.h>
#അന്തര്ഭാവ <getopt.h>

#അന്തര്ഭാവ "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#അന്തര്ഭാവ "system.h"

#അന്തര്ഭാവ "areadlink.h"
#അന്തര്ഭാവ "c-ctype.h"
#അന്തര്ഭാവ "file-type.h"
#അന്തര്ഭാവ "filemode.h"
#അന്തര്ഭാവ "fs.h"
#അന്തര്ഭാവ "mountlist.h"
#അന്തര്ഭാവ "octhexdigits.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "stat-size.h"
#അന്തര്ഭാവ "stat-time.h"
#അന്തര്ഭാവ "strftime.h"
#അന്തര്ഭാവ "find-mount-point.h"
#അന്തര്ഭാവ "xvasprintf.h"
#അന്തര്ഭാവ "statx.h"

#എങ്കിൽ HAVE_STATX && defined STATX_INO
# define USE_STATX 1
#അല്ലെങ്കിൽ
# define USE_STATX 0
#endif

#എങ്കിൽ USE_STATVFS
# define STRUCT_STATXFS_F_FSID_IS_INTEGER STRUCT_STATVFS_F_FSID_IS_INTEGER
# define HAVE_STRUCT_STATXFS_F_TYPE HAVE_STRUCT_STATVFS_F_TYPE
# എങ്കിൽ HAVE_STRUCT_STATVFS_F_NAMEMAX
#  define SB_F_NAMEMAX(S) ((S)->f_namemax)
# endif
# എങ്കിൽ ! STAT_STATVFS && STAT_STATVFS64
#  define STRUCT_STATVFS ഘടന statvfs64
#  define STATFS statvfs64
# അല്ലെങ്കിൽ
#  define STRUCT_STATVFS ഘടന statvfs
#  define STATFS statvfs
# endif
# define STATFS_FRSIZE(S) ((S)->f_frsize)
#അല്ലെങ്കിൽ
# define HAVE_STRUCT_STATXFS_F_TYPE HAVE_STRUCT_STATFS_F_TYPE
# എങ്കിൽ HAVE_STRUCT_STATFS_F_NAMELEN
#  define SB_F_NAMEMAX(S) ((S)->f_namelen)
# elif HAVE_STRUCT_STATFS_F_NAMEMAX
#  define SB_F_NAMEMAX(S) ((S)->f_namemax)
# endif
# define STATFS statfs
# എങ്കിൽ HAVE_OS_H /* BeOS */
/* BeOS has a statvfs function, but it does not return sensible values
   for f_files, f_ffree and f_favail, and lacks f_type, f_basetype and
   f_fstypename.  Use 'struct fs_info' instead.  */
NODISCARD
സ്ഥിതം പൂർണ്ണസംഖ്യ
statfs (അക്ഷരം സ്ഥിരം *filename, ഘടന fs_info *buf)
{
  dev_t device = dev_for_path (filename);
  എങ്കിൽ (device < 0)
    {
      errno = (device == B_ENTRY_NOT_FOUND ? ENOENT
               : device == B_BAD_മൂല്യം ? EINVAL
               : device == B_NAME_TOO_LONG ? ENAMETOOLONG
               : device == B_NO_MEMORY ? ENOMEM
               : device == B_FILE_ERROR ? EIO
               : 0);
      തിരികെ -1;
    }
  /* If successful, buf->dev will be == device.  */
  തിരികെ fs_stat_dev (device, buf);
}
#  define f_fsid dev
#  define f_blocks total_blocks
#  define f_bfree free_blocks
#  define f_bavail free_blocks
#  define f_bsize io_size
#  define f_files total_nodes
#  define f_ffree free_nodes
#  define STRUCT_STATVFS ഘടന fs_info
#  define STRUCT_STATXFS_F_FSID_IS_INTEGER true
#  define STATFS_FRSIZE(S) ((S)->block_size)
# അല്ലെങ്കിൽ
#  define STRUCT_STATVFS ഘടന statfs
#  define STRUCT_STATXFS_F_FSID_IS_INTEGER STRUCT_STATFS_F_FSID_IS_INTEGER
#  എങ്കിൽ HAVE_STRUCT_STATFS_F_FRSIZE
#   define STATFS_FRSIZE(S) ((S)->f_frsize)
#  അല്ലെങ്കിൽ
#   define STATFS_FRSIZE(S) 0
#  endif
# endif
#endif

#ifdef SB_F_NAMEMAX
# define OUT_NAMEMAX out_uint
#അല്ലെങ്കിൽ
/* Depending on whether statvfs or statfs is used,
   neither f_namemax or f_namelen may be available.  */
# define SB_F_NAMEMAX(S) "?"
# define OUT_NAMEMAX out_string
#endif

#എങ്കിൽ HAVE_STRUCT_STATVFS_F_BASETYPE
# define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME f_basetype
#അല്ലെങ്കിൽ
# എങ്കിൽ HAVE_STRUCT_STATVFS_F_FSTYPENAME || HAVE_STRUCT_STATFS_F_FSTYPENAME
#  define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME f_fstypename
# elif HAVE_OS_H /* BeOS */
#  define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME fsh_name
# endif
#endif

#എങ്കിൽ HAVE_GETATTRAT
# include <attr.h>
# include <sys/nvpair.h>
#endif

സ്ഥിതം അക്ഷരം സ്ഥിരം digits[] = "0123456789";

/* Flags that are portable for use in printf, for at least one
   conversion specifier; make_format removes non-portable flags as
   needed for particular specifiers.  The glibc 2.2 extension "I" is
   listed here; it is removed by make_format because it has undefined
   behavior elsewhere and because it is incompatible with
   out_epoch_sec.  */
സ്ഥിതം അക്ഷരം സ്ഥിരം printf_flags[] = "'-+ #0I";

/* Formats for the --terse option.  */
സ്ഥിതം അക്ഷരം സ്ഥിരം fmt_terse_fs[] = "%Qn %i %l %t %s %S %b %f %a %c %d\n";
സ്ഥിതം അക്ഷരം സ്ഥിരം fmt_terse_regular[] = "%Qn %s %b %f %u %g %D %i %h %t %T"
                                        " %X %Y %Z %W %o\n";
സ്ഥിതം അക്ഷരം സ്ഥിരം fmt_terse_selinux[] = "%Qn %s %b %f %u %g %D %i %h %t %T"
                                        " %X %Y %Z %W %o %C\n";

#define PROGRAM_NAME "stat"

#define AUTHORS proper_name ("Michael Meskes")

എണ്ണൽ
{
  PRINTF_OPTION = CHAR_MAX + 1
};

എണ്ണൽ cached_mode
{
  cached_default,
  cached_never,
  cached_always
};

സ്ഥിതം അക്ഷരം സ്ഥിരം *സ്ഥിരം cached_args[] =
{
  "default", "never", "always", ശൂന്യ
};

സ്ഥിതം എണ്ണൽ cached_mode സ്ഥിരം cached_modes[] =
{
  cached_default, cached_never, cached_always
};

സ്ഥിതം ഘടന option സ്ഥിരം long_options[] =
{
  {"dereference", no_argument, ശൂന്യ, 'L'},
  {"file-system", no_argument, ശൂന്യ, 'f'},
  {"format", required_argument, ശൂന്യ, 'c'},
  {"printf", required_argument, ശൂന്യ, PRINTF_OPTION},
  {"terse", no_argument, ശൂന്യ, 't'},
  {"cached", required_argument, ശൂന്യ, 0},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ശൂന്യ, 0, ശൂന്യ, 0}
};

/* Whether to follow symbolic links;  True for --dereference (-L).  */
സ്ഥിതം bool follow_links;

/* Whether to interpret backslash-escape sequences.
   True for --printf=FMT, not for --format=FMT (-c).  */
സ്ഥിതം bool interpret_backslash_escapes;

/* The trailing delimiter string:
   "" for --printf=FMT, "\n" for --format=FMT (-c).  */
സ്ഥിതം അക്ഷരം സ്ഥിരം *trailing_delim = "";

/* The representation of the decimal point in the current locale.  */
സ്ഥിതം അക്ഷരം സ്ഥിരം *decimal_point;
സ്ഥിതം size_t decimal_point_len;

സ്ഥിതം bool
print_stat (അക്ഷരം *pformat, size_t prefix_len, അക്ഷരം mod, അക്ഷരം m,
            പൂർണ്ണസംഖ്യ fd, അക്ഷരം സ്ഥിരം *filename, ശൂന്യം സ്ഥിരം *data);

/* Return the type of the specified file system.
   Some systems have statfvs.f_basetype[FSTYPSZ] (AIX, HP-UX, and Solaris).
   Others have statvfs.f_fstypename[_VFS_NAMELEN] (NetBSD 3.0).
   Others have statfs.f_fstypename[MFSNAMELEN] (NetBSD 1.5.2).
   Still others have neither and have to get by with f_type (GNU/Linux).
   But f_type may only exist in statfs (Cygwin).  */
NODISCARD
സ്ഥിതം അക്ഷരം സ്ഥിരം *
human_fstype (STRUCT_STATVFS സ്ഥിരം *statfsbuf)
{
#ifdef STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME
  തിരികെ statfsbuf->STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME;
#അല്ലെങ്കിൽ
  തിരഞ്ഞെടുപ്പ് (statfsbuf->f_type)
    {
# എങ്കിൽ defined __linux__ || defined __ANDROID__

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

    അവസ്ഥ S_MAGIC_AAFS: /* 0x5A3C69F0 local */
      തിരികെ "aafs";
    അവസ്ഥ S_MAGIC_ACFS: /* 0x61636673 remote */
      തിരികെ "acfs";
    അവസ്ഥ S_MAGIC_ADFS: /* 0xADF5 local */
      തിരികെ "adfs";
    അവസ്ഥ S_MAGIC_AFFS: /* 0xADFF local */
      തിരികെ "affs";
    അവസ്ഥ S_MAGIC_AFS: /* 0x5346414F remote */
      തിരികെ "afs";
    അവസ്ഥ S_MAGIC_ANON_INODE_FS: /* 0x09041934 local */
      തിരികെ "anon-inode FS";
    അവസ്ഥ S_MAGIC_AUFS: /* 0x61756673 remote */
      /* FIXME: change syntax or add an optional attribute like "inotify:no".
         The above is labeled as "remote" so that tail always uses polling,
         but this isn't really a remote file system type.  */
      തിരികെ "aufs";
    അവസ്ഥ S_MAGIC_AUTOFS: /* 0x0187 local */
      തിരികെ "autofs";
    അവസ്ഥ S_MAGIC_BALLOON_KVM: /* 0x13661366 local */
      തിരികെ "balloon-kvm-fs";
    അവസ്ഥ S_MAGIC_BCACHEFS: /* 0xCA451A4E local */
      തിരികെ "bcachefs";
    അവസ്ഥ S_MAGIC_BEFS: /* 0x42465331 local */
      തിരികെ "befs";
    അവസ്ഥ S_MAGIC_BDEVFS: /* 0x62646576 local */
      തിരികെ "bdevfs";
    അവസ്ഥ S_MAGIC_BFS: /* 0x1BADFACE local */
      തിരികെ "bfs";
    അവസ്ഥ S_MAGIC_BINDERFS: /* 0x6C6F6F70 local */
      തിരികെ "binderfs";
    അവസ്ഥ S_MAGIC_BPF_FS: /* 0xCAFE4A11 local */
      തിരികെ "bpf_fs";
    അവസ്ഥ S_MAGIC_BINFMTFS: /* 0x42494E4D local */
      തിരികെ "binfmt_misc";
    അവസ്ഥ S_MAGIC_BTRFS: /* 0x9123683E local */
      തിരികെ "btrfs";
    അവസ്ഥ S_MAGIC_BTRFS_TEST: /* 0x73727279 local */
      തിരികെ "btrfs_test";
    അവസ്ഥ S_MAGIC_CEPH: /* 0x00C36400 remote */
      തിരികെ "ceph";
    അവസ്ഥ S_MAGIC_CGROUP: /* 0x0027E0EB local */
      തിരികെ "cgroupfs";
    അവസ്ഥ S_MAGIC_CGROUP2: /* 0x63677270 local */
      തിരികെ "cgroup2fs";
    അവസ്ഥ S_MAGIC_CIFS: /* 0xFF534D42 remote */
      തിരികെ "cifs";
    അവസ്ഥ S_MAGIC_CODA: /* 0x73757245 remote */
      തിരികെ "coda";
    അവസ്ഥ S_MAGIC_COH: /* 0x012FF7B7 local */
      തിരികെ "coh";
    അവസ്ഥ S_MAGIC_CONFIGFS: /* 0x62656570 local */
      തിരികെ "configfs";
    അവസ്ഥ S_MAGIC_CRAMFS: /* 0x28CD3D45 local */
      തിരികെ "cramfs";
    അവസ്ഥ S_MAGIC_CRAMFS_WEND: /* 0x453DCD28 local */
      തിരികെ "cramfs-wend";
    അവസ്ഥ S_MAGIC_DAXFS: /* 0x64646178 local */
      തിരികെ "daxfs";
    അവസ്ഥ S_MAGIC_DEBUGFS: /* 0x64626720 local */
      തിരികെ "debugfs";
    അവസ്ഥ S_MAGIC_DEVFS: /* 0x1373 local */
      തിരികെ "devfs";
    അവസ്ഥ S_MAGIC_DEVMEM: /* 0x454D444D local */
      തിരികെ "devmem";
    അവസ്ഥ S_MAGIC_DEVPTS: /* 0x1CD1 local */
      തിരികെ "devpts";
    അവസ്ഥ S_MAGIC_DMA_BUF: /* 0x444D4142 local */
      തിരികെ "dma-buf-fs";
    അവസ്ഥ S_MAGIC_ECRYPTFS: /* 0xF15F local */
      തിരികെ "ecryptfs";
    അവസ്ഥ S_MAGIC_EFIVARFS: /* 0xDE5E81E4 local */
      തിരികെ "efivarfs";
    അവസ്ഥ S_MAGIC_EFS: /* 0x00414A53 local */
      തിരികെ "efs";
    അവസ്ഥ S_MAGIC_EROFS_V1: /* 0xE0F5E1E2 local */
      തിരികെ "erofs";
    അവസ്ഥ S_MAGIC_EXFAT: /* 0x2011BAB0 local */
      തിരികെ "exfat";
    അവസ്ഥ S_MAGIC_EXFS: /* 0x45584653 local */
      തിരികെ "exfs";
    അവസ്ഥ S_MAGIC_EXOFS: /* 0x5DF5 local */
      തിരികെ "exofs";
    അവസ്ഥ S_MAGIC_EXT: /* 0x137D local */
      തിരികെ "ext";
    അവസ്ഥ S_MAGIC_EXT2: /* 0xEF53 local */
      തിരികെ "ext2/ext3";
    അവസ്ഥ S_MAGIC_EXT2_OLD: /* 0xEF51 local */
      തിരികെ "ext2";
    അവസ്ഥ S_MAGIC_F2FS: /* 0xF2F52010 local */
      തിരികെ "f2fs";
    അവസ്ഥ S_MAGIC_FAIL_FS: /* 0x4641494C local */
      തിരികെ "failfs";
    അവസ്ഥ S_MAGIC_FAT: /* 0x4006 local */
      തിരികെ "fat";
    അവസ്ഥ S_MAGIC_FHGFS: /* 0x19830326 remote */
      തിരികെ "fhgfs";
    അവസ്ഥ S_MAGIC_FUSE: /* 0x65735546 remote */
      തിരികെ "fuse";
    അവസ്ഥ S_MAGIC_FUSECTL: /* 0x65735543 remote */
      തിരികെ "fusectl";
    അവസ്ഥ S_MAGIC_FUTEXFS: /* 0x0BAD1DEA local */
      തിരികെ "futexfs";
    അവസ്ഥ S_MAGIC_GFS: /* 0x01161970 remote */
      തിരികെ "gfs/gfs2";
    അവസ്ഥ S_MAGIC_GPFS: /* 0x47504653 remote */
      തിരികെ "gpfs";
    അവസ്ഥ S_MAGIC_GUEST_MEMFD: /* 0x474D454D remote */
      തിരികെ "guest-memfd";
    അവസ്ഥ S_MAGIC_HFS: /* 0x4244 local */
      തിരികെ "hfs";
    അവസ്ഥ S_MAGIC_HFS_PLUS: /* 0x482B local */
      തിരികെ "hfs+";
    അവസ്ഥ S_MAGIC_HFS_X: /* 0x4858 local */
      തിരികെ "hfsx";
    അവസ്ഥ S_MAGIC_HOSTFS: /* 0x00C0FFEE local */
      തിരികെ "hostfs";
    അവസ്ഥ S_MAGIC_HPFS: /* 0xF995E849 local */
      തിരികെ "hpfs";
    അവസ്ഥ S_MAGIC_HUGETLBFS: /* 0x958458F6 local */
      തിരികെ "hugetlbfs";
    അവസ്ഥ S_MAGIC_MTD_INODE_FS: /* 0x11307854 local */
      തിരികെ "inodefs";
    അവസ്ഥ S_MAGIC_IBRIX: /* 0x013111A8 remote */
      തിരികെ "ibrix";
    അവസ്ഥ S_MAGIC_INOTIFYFS: /* 0x2BAD1DEA local */
      തിരികെ "inotifyfs";
    അവസ്ഥ S_MAGIC_ISOFS: /* 0x9660 local */
      തിരികെ "isofs";
    അവസ്ഥ S_MAGIC_ISOFS_R_WIN: /* 0x4004 local */
      തിരികെ "isofs";
    അവസ്ഥ S_MAGIC_ISOFS_WIN: /* 0x4000 local */
      തിരികെ "isofs";
    അവസ്ഥ S_MAGIC_JFFS: /* 0x07C0 local */
      തിരികെ "jffs";
    അവസ്ഥ S_MAGIC_JFFS2: /* 0x72B6 local */
      തിരികെ "jffs2";
    അവസ്ഥ S_MAGIC_JFS: /* 0x3153464A local */
      തിരികെ "jfs";
    അവസ്ഥ S_MAGIC_KAFS: /* 0x6B414653 remote */
      തിരികെ "k-afs";
    അവസ്ഥ S_MAGIC_LOGFS: /* 0xC97E8168 local */
      തിരികെ "logfs";
    അവസ്ഥ S_MAGIC_LUSTRE: /* 0x0BD00BD0 remote */
      തിരികെ "lustre";
    അവസ്ഥ S_MAGIC_M1FS: /* 0x5346314D local */
      തിരികെ "m1fs";
    അവസ്ഥ S_MAGIC_MINIX: /* 0x137F local */
      തിരികെ "minix";
    അവസ്ഥ S_MAGIC_MINIX_30: /* 0x138F local */
      തിരികെ "minix (30 char.)";
    അവസ്ഥ S_MAGIC_MINIX_V2: /* 0x2468 local */
      തിരികെ "minix v2";
    അവസ്ഥ S_MAGIC_MINIX_V2_30: /* 0x2478 local */
      തിരികെ "minix v2 (30 char.)";
    അവസ്ഥ S_MAGIC_MINIX_V3: /* 0x4D5A local */
      തിരികെ "minix3";
    അവസ്ഥ S_MAGIC_MQUEUE: /* 0x19800202 local */
      തിരികെ "mqueue";
    അവസ്ഥ S_MAGIC_MSDOS: /* 0x4D44 local */
      തിരികെ "msdos";
    അവസ്ഥ S_MAGIC_NCP: /* 0x564C remote */
      തിരികെ "novell";
    അവസ്ഥ S_MAGIC_NFS: /* 0x6969 remote */
      തിരികെ "nfs";
    അവസ്ഥ S_MAGIC_NFSD: /* 0x6E667364 remote */
      തിരികെ "nfsd";
    അവസ്ഥ S_MAGIC_NILFS: /* 0x3434 local */
      തിരികെ "nilfs";
    അവസ്ഥ S_MAGIC_NSFS: /* 0x6E736673 local */
      തിരികെ "nsfs";
    അവസ്ഥ S_MAGIC_NTFS: /* 0x5346544E local */
      തിരികെ "ntfs";
    അവസ്ഥ S_MAGIC_NULL_FS: /* 0x4E554C4C local */
      തിരികെ "nullfs";
    അവസ്ഥ S_MAGIC_OPENPROM: /* 0x9FA1 local */
      തിരികെ "openprom";
    അവസ്ഥ S_MAGIC_OCFS2: /* 0x7461636F remote */
      തിരികെ "ocfs2";
    അവസ്ഥ S_MAGIC_OVERLAYFS: /* 0x794C7630 remote */
      /* This may overlay remote file systems.
         Also there have been issues reported with inotify and overlayfs,
         so mark as "remote" so that polling is used.  */
      തിരികെ "overlayfs";
    അവസ്ഥ S_MAGIC_PANFS: /* 0xAAD7AAEA remote */
      തിരികെ "panfs";
    അവസ്ഥ S_MAGIC_PID_FS: /* 0x50494446 local */
      തിരികെ "pidfs";
    അവസ്ഥ S_MAGIC_PIPEFS: /* 0x50495045 remote */
      /* FIXME: change syntax or add an optional attribute like "inotify:no".
         pipefs is labeled as "remote" so that tail always polls,
         but it isn't really remote file system type.  */
      തിരികെ "pipefs";
    അവസ്ഥ S_MAGIC_PPC_CMM: /* 0xC7571590 local */
      തിരികെ "ppc-cmm-fs";
    അവസ്ഥ S_MAGIC_PRL_FS: /* 0x7C7C6673 remote */
      തിരികെ "prl_fs";
    അവസ്ഥ S_MAGIC_PROC: /* 0x9FA0 local */
      തിരികെ "proc";
    അവസ്ഥ S_MAGIC_PSTOREFS: /* 0x6165676C local */
      തിരികെ "pstorefs";
    അവസ്ഥ S_MAGIC_QNX4: /* 0x002F local */
      തിരികെ "qnx4";
    അവസ്ഥ S_MAGIC_QNX6: /* 0x68191122 local */
      തിരികെ "qnx6";
    അവസ്ഥ S_MAGIC_RAMFS: /* 0x858458F6 local */
      തിരികെ "ramfs";
    അവസ്ഥ S_MAGIC_RDTGROUP: /* 0x07655821 local */
      തിരികെ "rdt";
    അവസ്ഥ S_MAGIC_REISERFS: /* 0x52654973 local */
      തിരികെ "reiserfs";
    അവസ്ഥ S_MAGIC_ROMFS: /* 0x7275 local */
      തിരികെ "romfs";
    അവസ്ഥ S_MAGIC_RPC_PIPEFS: /* 0x67596969 local */
      തിരികെ "rpc_pipefs";
    അവസ്ഥ S_MAGIC_SDCARDFS: /* 0x5DCA2DF5 local */
      തിരികെ "sdcardfs";
    അവസ്ഥ S_MAGIC_SECRETMEM: /* 0x5345434D local */
      തിരികെ "secretmem";
    അവസ്ഥ S_MAGIC_SECURITYFS: /* 0x73636673 local */
      തിരികെ "securityfs";
    അവസ്ഥ S_MAGIC_SELINUX: /* 0xF97CFF8C local */
      തിരികെ "selinux";
    അവസ്ഥ S_MAGIC_SMACK: /* 0x43415D53 local */
      തിരികെ "smackfs";
    അവസ്ഥ S_MAGIC_SMB: /* 0x517B remote */
      തിരികെ "smb";
    അവസ്ഥ S_MAGIC_SMB2: /* 0xFE534D42 remote */
      തിരികെ "smb2";
    അവസ്ഥ S_MAGIC_SNFS: /* 0xBEEFDEAD remote */
      തിരികെ "snfs";
    അവസ്ഥ S_MAGIC_SOCKFS: /* 0x534F434B local */
      തിരികെ "sockfs";
    അവസ്ഥ S_MAGIC_SQUASHFS: /* 0x73717368 local */
      തിരികെ "squashfs";
    അവസ്ഥ S_MAGIC_SYSFS: /* 0x62656572 local */
      തിരികെ "sysfs";
    അവസ്ഥ S_MAGIC_SYSV2: /* 0x012FF7B6 local */
      തിരികെ "sysv2";
    അവസ്ഥ S_MAGIC_SYSV4: /* 0x012FF7B5 local */
      തിരികെ "sysv4";
    അവസ്ഥ S_MAGIC_TMPFS: /* 0x01021994 local */
      തിരികെ "tmpfs";
    അവസ്ഥ S_MAGIC_TRACEFS: /* 0x74726163 local */
      തിരികെ "tracefs";
    അവസ്ഥ S_MAGIC_UBIFS: /* 0x24051905 local */
      തിരികെ "ubifs";
    അവസ്ഥ S_MAGIC_UDF: /* 0x15013346 local */
      തിരികെ "udf";
    അവസ്ഥ S_MAGIC_UFS: /* 0x00011954 local */
      തിരികെ "ufs";
    അവസ്ഥ S_MAGIC_UFS_BYTESWAPPED: /* 0x54190100 local */
      തിരികെ "ufs";
    അവസ്ഥ S_MAGIC_USBDEVFS: /* 0x9FA2 local */
      തിരികെ "usbdevfs";
    അവസ്ഥ S_MAGIC_V9FS: /* 0x01021997 local */
      തിരികെ "v9fs";
    അവസ്ഥ S_MAGIC_VBOXSF: /* 0x786F4256 remote */
      തിരികെ "vboxsf";
    അവസ്ഥ S_MAGIC_VMHGFS: /* 0xBACBACBC remote */
      തിരികെ "vmhgfs";
    അവസ്ഥ S_MAGIC_VXFS: /* 0xA501FCF5 remote */
      /* Veritas File System can run in single instance or clustered mode,
         so mark as remote to cater for the latter case.  */
      തിരികെ "vxfs";
    അവസ്ഥ S_MAGIC_VZFS: /* 0x565A4653 local */
      തിരികെ "vzfs";
    അവസ്ഥ S_MAGIC_WSLFS: /* 0x53464846 local */
      തിരികെ "wslfs";
    അവസ്ഥ S_MAGIC_XENFS: /* 0xABBA1974 local */
      തിരികെ "xenfs";
    അവസ്ഥ S_MAGIC_XENIX: /* 0x012FF7B4 local */
      തിരികെ "xenix";
    അവസ്ഥ S_MAGIC_XFS: /* 0x58465342 local */
      തിരികെ "xfs";
    അവസ്ഥ S_MAGIC_XIAFS: /* 0x012FD16D local */
      തിരികെ "xia";
    അവസ്ഥ S_MAGIC_Z3FOLD: /* 0x0033 local */
      തിരികെ "z3fold";
    അവസ്ഥ S_MAGIC_ZFS: /* 0x2FC12FC1 local */
      തിരികെ "zfs";
    അവസ്ഥ S_MAGIC_ZONEFS: /* 0x5A4F4653 local */
      തിരികെ "zonefs";
    അവസ്ഥ S_MAGIC_ZSMALLOC: /* 0x58295829 local */
      തിരികെ "zsmallocfs";


# elif __GNU__
    അവസ്ഥ FSTYPE_UFS:
      തിരികെ "ufs";
    അവസ്ഥ FSTYPE_NFS:
      തിരികെ "nfs";
    അവസ്ഥ FSTYPE_GFS:
      തിരികെ "gfs";
    അവസ്ഥ FSTYPE_LFS:
      തിരികെ "lfs";
    അവസ്ഥ FSTYPE_SYSV:
      തിരികെ "sysv";
    അവസ്ഥ FSTYPE_FTP:
      തിരികെ "ftp";
    അവസ്ഥ FSTYPE_TAR:
      തിരികെ "tar";
    അവസ്ഥ FSTYPE_AR:
      തിരികെ "ar";
    അവസ്ഥ FSTYPE_CPIO:
      തിരികെ "cpio";
    അവസ്ഥ FSTYPE_MSLOSS:
      തിരികെ "msloss";
    അവസ്ഥ FSTYPE_CPM:
      തിരികെ "cpm";
    അവസ്ഥ FSTYPE_HFS:
      തിരികെ "hfs";
    അവസ്ഥ FSTYPE_DTFS:
      തിരികെ "dtfs";
    അവസ്ഥ FSTYPE_GRFS:
      തിരികെ "grfs";
    അവസ്ഥ FSTYPE_TERM:
      തിരികെ "term";
    അവസ്ഥ FSTYPE_DEV:
      തിരികെ "dev";
    അവസ്ഥ FSTYPE_PROC:
      തിരികെ "proc";
    അവസ്ഥ FSTYPE_IFSOCK:
      തിരികെ "ifsock";
    അവസ്ഥ FSTYPE_AFS:
      തിരികെ "afs";
    അവസ്ഥ FSTYPE_DFS:
      തിരികെ "dfs";
    അവസ്ഥ FSTYPE_PROC9:
      തിരികെ "proc9";
    അവസ്ഥ FSTYPE_SOCKET:
      തിരികെ "socket";
    അവസ്ഥ FSTYPE_MISC:
      തിരികെ "misc";
    അവസ്ഥ FSTYPE_EXT2FS:
      തിരികെ "ext2/ext3";
    അവസ്ഥ FSTYPE_HTTP:
      തിരികെ "http";
    അവസ്ഥ FSTYPE_MEMFS:
      തിരികെ "memfs";
    അവസ്ഥ FSTYPE_ISO9660:
      തിരികെ "iso9660";
# endif
    സ്ഥിരസ്ഥിതി:
      {
        unsigned long പൂർണ്ണസംഖ്യ type = statfsbuf->f_type;
        സ്ഥിതം അക്ഷരം buf[വലുപ്പം "UNKNOWN (0x%lx)" - 3
                        + (വലുപ്പം type * CHAR_BIT + 3) / 4];
        sprintf (buf, "UNKNOWN (0x%lx)", type);
        തിരികെ buf;
      }
    }
#endif
}

NODISCARD
സ്ഥിതം അക്ഷരം *
human_access (ഘടന stat സ്ഥിരം *statbuf)
{
  സ്ഥിതം അക്ഷരം modebuf[12];
  filemodestring (statbuf, modebuf);
  modebuf[10] = 0;
  തിരികെ modebuf;
}

NODISCARD
സ്ഥിതം അക്ഷരം *
human_time (ഘടന timespec t)
{
  /* STR must be at least INT_BUFSIZE_BOUND (intmax_t) big, either
     because localtime_rz fails, or because the time zone is truly
     outlandish so that %z expands to a long string.  */
  സ്ഥിതം അക്ഷരം str[INT_BUFSIZE_BOUND (intmax_t)
                  + INT_STRLEN_BOUND (പൂർണ്ണസംഖ്യ) /* YYYY */
                  + 1 /* because YYYY might equal INT_MAX + 1900 */
                  + വലുപ്പം "-MM-DD HH:MM:SS.NNNNNNNNN +"];
  സ്ഥിതം timezone_t tz;
  എങ്കിൽ (!tz)
    tz = tzalloc (getenv ("TZ"));
  ഘടന tm tm;
  പൂർണ്ണസംഖ്യ ns = t.tv_nsec;
  എങ്കിൽ (localtime_rz (tz, &t.tv_sec, &tm))
    nstrftime (str, വലുപ്പം str, "%Y-%m-%d %H:%M:%S.%N %z", &tm, tz, ns);
  അല്ലെങ്കിൽ
    {
      അക്ഷരം secbuf[INT_BUFSIZE_BOUND (intmax_t)];
      sprintf (str, "%s.%09d", timetostr (t.tv_sec, secbuf), ns);
    }
  തിരികെ str;
}

/* PFORMAT points to a '%' followed by a prefix of a format, all of
   size PREFIX_LEN.  The flags allowed for this format are
   ALLOWED_FLAGS; remove other printf flags from the prefix, then
   append SUFFIX.  */
സ്ഥിതം ശൂന്യം
make_format (അക്ഷരം *pformat, size_t prefix_len, അക്ഷരം സ്ഥിരം *allowed_flags,
             അക്ഷരം സ്ഥിരം *suffix)
{
  അക്ഷരം *dst = pformat + 1;
  അക്ഷരം സ്ഥിരം *src;
  അക്ഷരം സ്ഥിരം *srclim = pformat + prefix_len;
  വേണ്ടി (src = dst; src < srclim && strchr (printf_flags, *src); src++)
    എങ്കിൽ (strchr (allowed_flags, *src))
      *dst++ = *src;
  ആകുമ്പോൾ (src < srclim)
    *dst++ = *src++;
  strcpy (dst, suffix);
}

സ്ഥിതം ശൂന്യം
out_string (അക്ഷരം *pformat, size_t prefix_len, അക്ഷരം സ്ഥിരം *arg)
{
  make_format (pformat, prefix_len, "-", "s");
  printf (pformat, arg);
}
സ്ഥിതം പൂർണ്ണസംഖ്യ
out_int (അക്ഷരം *pformat, size_t prefix_len, intmax_t arg)
{
  make_format (pformat, prefix_len, "'-+ 0", "jd");
  തിരികെ printf (pformat, arg);
}
സ്ഥിതം പൂർണ്ണസംഖ്യ
out_uint (അക്ഷരം *pformat, size_t prefix_len, uintmax_t arg)
{
  make_format (pformat, prefix_len, "'-0", "ju");
  തിരികെ printf (pformat, arg);
}
സ്ഥിതം ശൂന്യം
out_uint_o (അക്ഷരം *pformat, size_t prefix_len, uintmax_t arg)
{
  make_format (pformat, prefix_len, "-#0", "jo");
  printf (pformat, arg);
}
സ്ഥിതം ശൂന്യം
out_uint_x (അക്ഷരം *pformat, size_t prefix_len, uintmax_t arg)
{
  make_format (pformat, prefix_len, "-#0", "jx");
  printf (pformat, arg);
}
സ്ഥിതം പൂർണ്ണസംഖ്യ
out_minus_zero (അക്ഷരം *pformat, size_t prefix_len)
{
  make_format (pformat, prefix_len, "'-+ 0", ".0f");
  തിരികെ printf (pformat, -0.25);
}

/* Output the number of seconds since the Epoch, using a format that
   acts like printf's %f format.  */
സ്ഥിതം ശൂന്യം
out_epoch_sec (അക്ഷരം *pformat, size_t prefix_len,
               ഘടന timespec arg)
{
  അക്ഷരം *dot = memchr (pformat, '.', prefix_len);
  size_t sec_prefix_len = prefix_len;
  പൂർണ്ണസംഖ്യ width = 0;
  പൂർണ്ണസംഖ്യ precision = 0;
  bool frac_ഇടത്_adjust = false;

  എങ്കിൽ (dot)
    {
      sec_prefix_len = dot - pformat;
      pformat[prefix_len] = '\0';

      എങ്കിൽ (c_isdigit (dot[1]))
        {
          long പൂർണ്ണസംഖ്യ lprec = strtol (dot + 1, ശൂന്യ, 10);
          precision = (lprec <= INT_MAX ? lprec : INT_MAX);
        }
      അല്ലെങ്കിൽ
        {
          precision = 9;
        }

      എങ്കിൽ (precision && c_isdigit (dot[-1]))
        {
          /* If a nontrivial width is given, subtract the width of the
             decimal point and PRECISION digits that will be output
             later.  */
          അക്ഷരം *p = dot;
          *dot = '\0';

          ചെയ്യുക
            --p;
          ആകുമ്പോൾ (c_isdigit (p[-1]));

          long പൂർണ്ണസംഖ്യ lwidth = strtol (p, ശൂന്യ, 10);
          width = (lwidth <= INT_MAX ? lwidth : INT_MAX);
          എങ്കിൽ (1 < width)
            {
              p += (*p == '0');
              sec_prefix_len = p - pformat;
              പൂർണ്ണസംഖ്യ w_d = (decimal_point_len < width
                         ? width - decimal_point_len
                         : 0);
              എങ്കിൽ (1 < w_d)
                {
                  പൂർണ്ണസംഖ്യ w = w_d - precision;
                  എങ്കിൽ (1 < w)
                    {
                      അക്ഷരം *dst = pformat;
                      വേണ്ടി (അക്ഷരം സ്ഥിരം *src = dst; src < p; src++)
                        {
                          എങ്കിൽ (*src == '-')
                            frac_ഇടത്_adjust = true;
                          അല്ലെങ്കിൽ
                            *dst++ = *src;
                        }
                      sec_prefix_len =
                        (dst - pformat
                         + (frac_ഇടത്_adjust ? 0 : sprintf (dst, "%d", w)));
                    }
                }
            }
        }
    }

  പൂർണ്ണസംഖ്യ ഹാരകം = 1;
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = precision; i < 9; i++)
    ഹാരകം *= 10;
  പൂർണ്ണസംഖ്യ frac_sec = arg.tv_nsec / ഹാരകം;
  പൂർണ്ണസംഖ്യ int_len;

  എങ്കിൽ (TYPE_SIGNED (time_t))
    {
      bool minus_zero = false;
      എങ്കിൽ (arg.tv_sec < 0 && arg.tv_nsec != 0)
        {
          പൂർണ്ണസംഖ്യ frac_sec_modulus = 1000000000 / ഹാരകം;
          frac_sec = (frac_sec_modulus - frac_sec
                      - (arg.tv_nsec % ഹാരകം != 0));
          arg.tv_sec += (frac_sec != 0);
          minus_zero = (arg.tv_sec == 0);
        }
      int_len = (minus_zero
                 ? out_minus_zero (pformat, sec_prefix_len)
                 : out_int (pformat, sec_prefix_len, arg.tv_sec));
    }
  അല്ലെങ്കിൽ
    int_len = out_uint (pformat, sec_prefix_len, arg.tv_sec);

  എങ്കിൽ (precision)
    {
      പൂർണ്ണസംഖ്യ prec = (precision < 9 ? precision : 9);
      പൂർണ്ണസംഖ്യ trailing_prec = precision - prec;
      പൂർണ്ണസംഖ്യ ilen = (int_len < 0 ? 0 : int_len);
      പൂർണ്ണസംഖ്യ trailing_width = (ilen < width && decimal_point_len < width - ilen
                            ? width - ilen - decimal_point_len - prec
                            : 0);
      printf ("%s%.*d%-*.*d", decimal_point, prec, frac_sec,
              trailing_width, trailing_prec, 0);
    }
}

/* Print the context information of FILENAME, and return true iff the
   context could not be obtained.  */
NODISCARD
സ്ഥിതം bool
out_file_context (അക്ഷരം *pformat, size_t prefix_len, അക്ഷരം സ്ഥിരം *filename)
{
  അക്ഷരം *scontext;
  bool fail = false;

  എങ്കിൽ ((follow_links
       ? getfilecon (filename, &scontext)
       : lgetfilecon (filename, &scontext)) < 0)
    {
      error (0, errno, _("failed to get security context of %s"),
             quoteaf (filename));
      scontext = ശൂന്യ;
      fail = true;
    }
  strcpy (pformat + prefix_len, "s");
  printf (pformat, (scontext ? scontext : "?"));
  എങ്കിൽ (scontext)
    freecon (scontext);
  തിരികെ fail;
}

/* Set the quoting style once it is needed.  */
സ്ഥിതം ശൂന്യം
initialize_quoting_style (ശൂന്യം)
{
  സ്ഥിതം bool initialized;
  എങ്കിൽ (!initialized)
    {
      initialized = true;
      പൂർണ്ണസംഖ്യ qs = getenv_quoting_style ();
      set_quoting_style (ശൂന്യ, 0 <= qs ? qs : shell_escape_quoting_style);
    }
}

/* Print statfs info.  Return zero upon success, nonzero upon failure.  */
NODISCARD
സ്ഥിതം bool
print_statfs (അക്ഷരം *pformat, size_t prefix_len, MAYBE_UNUSED അക്ഷരം mod, അക്ഷരം m,
              MAYBE_UNUSED പൂർണ്ണസംഖ്യ fd, അക്ഷരം സ്ഥിരം *filename,
              ശൂന്യം സ്ഥിരം *data)
{
  STRUCT_STATVFS സ്ഥിരം *statfsbuf = data;
  bool fail = false;

  തിരഞ്ഞെടുപ്പ് (m)
    {
    അവസ്ഥ 'n':
      എങ്കിൽ (mod == 'Q')
        {
          initialize_quoting_style ();
          filename = quoteN (filename);
        }
      out_string (pformat, prefix_len, filename);
      നിർത്തുക;

    അവസ്ഥ 'i':
      {
#എങ്കിൽ STRUCT_STATXFS_F_FSID_IS_INTEGER
        uintmax_t fsid = statfsbuf->f_fsid;
#അല്ലെങ്കിൽ
        തരനിർവചനം unsigned പൂർണ്ണസംഖ്യ fsid_word;
        static_assert (alignof (STRUCT_STATVFS) % alignof (fsid_word) == 0);
        static_assert (offsetof (STRUCT_STATVFS, f_fsid) % alignof (fsid_word)
                       == 0);
        static_assert (വലുപ്പം statfsbuf->f_fsid % alignof (fsid_word) == 0);
        fsid_word സ്ഥിരം *p = (fsid_word *) &statfsbuf->f_fsid;

        /* Assume a little-endian word order, as that is compatible
           with glibc's statvfs implementation.  */
        uintmax_t fsid = 0;
        പൂർണ്ണസംഖ്യ വാക്കുകൾ = വലുപ്പം statfsbuf->f_fsid / വലുപ്പം *p;
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < വാക്കുകൾ && i * വലുപ്പം *p < വലുപ്പം fsid; i++)
          {
            uintmax_t u = p[വാക്കുകൾ - 1 - i];
            fsid |= u << (i * CHAR_BIT * വലുപ്പം *p);
          }
#endif
        out_uint_x (pformat, prefix_len, fsid);
      }
      നിർത്തുക;

    അവസ്ഥ 'l':
      OUT_NAMEMAX (pformat, prefix_len, SB_F_NAMEMAX (statfsbuf));
      നിർത്തുക;
    അവസ്ഥ 't':
#എങ്കിൽ HAVE_STRUCT_STATXFS_F_TYPE
      out_uint_x (pformat, prefix_len, statfsbuf->f_type);
#അല്ലെങ്കിൽ
      fputc ('?', stdout);
#endif
      നിർത്തുക;
    അവസ്ഥ 'T':
      out_string (pformat, prefix_len, human_fstype (statfsbuf));
      നിർത്തുക;
    അവസ്ഥ 'b':
      out_int (pformat, prefix_len, statfsbuf->f_blocks);
      നിർത്തുക;
    അവസ്ഥ 'f':
      out_int (pformat, prefix_len, statfsbuf->f_bfree);
      നിർത്തുക;
    അവസ്ഥ 'a':
      out_int (pformat, prefix_len, statfsbuf->f_bavail);
      നിർത്തുക;
    അവസ്ഥ 's':
      out_uint (pformat, prefix_len, statfsbuf->f_bsize);
      നിർത്തുക;
    അവസ്ഥ 'S':
      {
        uintmax_t frsize = STATFS_FRSIZE (statfsbuf);
        എങ്കിൽ (! frsize)
          frsize = statfsbuf->f_bsize;
        out_uint (pformat, prefix_len, frsize);
      }
      നിർത്തുക;
    അവസ്ഥ 'c':
      out_uint (pformat, prefix_len, statfsbuf->f_files);
      നിർത്തുക;
    അവസ്ഥ 'd':
      out_int (pformat, prefix_len, statfsbuf->f_ffree);
      നിർത്തുക;
    സ്ഥിരസ്ഥിതി:
      fputc ('?', stdout);
      നിർത്തുക;
    }
  തിരികെ fail;
}

/* Return any bind mounted source for a path.
   The caller should not free the returned buffer.
   Return NULL if no bind mount found.  */
NODISCARD
സ്ഥിതം അക്ഷരം സ്ഥിരം *
find_bind_mount (അക്ഷരം സ്ഥിരം * name)
{
  അക്ഷരം സ്ഥിരം * bind_mount = ശൂന്യ;

  സ്ഥിതം ഘടന mount_entry *mount_list;
  സ്ഥിതം bool tried_mount_list = false;
  എങ്കിൽ (!tried_mount_list) /* attempt/warn once per process.  */
    {
      എങ്കിൽ (!(mount_list = read_file_system_list (false)))
        error (0, errno, "%s", _("cannot read table of mounted file systems"));
      tried_mount_list = true;
    }

  ഘടന stat name_stats;
  എങ്കിൽ (stat (name, &name_stats) != 0)
    തിരികെ ശൂന്യ;

  വേണ്ടി (ഘടന mount_entry *me = mount_list; me; me = me->me_next)
    {
      എങ്കിൽ (me->me_dummy && me->me_devname[0] == '/'
          && streq (me->me_mountdir, name))
        {
          ഘടന stat dev_stats;

          എങ്കിൽ (stat (me->me_devname, &dev_stats) == 0
              && psame_inode (&name_stats, &dev_stats))
            {
              bind_mount = me->me_devname;
              നിർത്തുക;
            }
        }
    }

  തിരികെ bind_mount;
}

/* Print mount point.  Return zero upon success, nonzero upon failure.  */
NODISCARD
സ്ഥിതം bool
out_mount_point (അക്ഷരം സ്ഥിരം *filename, അക്ഷരം *pformat, size_t prefix_len,
                 സ്ഥിരം ഘടന stat *statp)
{

  അക്ഷരം സ്ഥിരം *np = "?", *bp = ശൂന്യ;
  അക്ഷരം *mp = ശൂന്യ;
  bool fail = true;

  /* Look for bind mounts first.  Note we output the immediate alias,
     rather than further resolving to a base device mount point.  */
  എങ്കിൽ (follow_links || !S_ISLNK (statp->st_mode))
    {
      അക്ഷരം *resolved = canonicalize_file_name (filename);
      എങ്കിൽ (!resolved)
        {
          error (0, errno, _("failed to canonicalize %s"), quoteaf (filename));
          പോകുക print_mount_point;
        }
      bp = find_bind_mount (resolved);
      സ്വതന്ത്രം (resolved);
      എങ്കിൽ (bp)
        {
          fail = false;
          പോകുക print_mount_point;
        }
    }

  /* If there is no direct bind mount, then navigate
     back up the tree looking for a device change.
     Note we don't detect if any of the directory components
     are bind mounted to the same device, but that's OK
     since we've not directly queried them.  */
  എങ്കിൽ ((mp = find_mount_point (filename, statp)))
    {
      /* This dir might be bind mounted to another device,
         so we resolve the bound source in that case also.  */
      bp = find_bind_mount (mp);
      fail = false;
    }

print_mount_point:

  out_string (pformat, prefix_len, bp ? bp : mp ? mp : np);
  സ്വതന്ത്രം (mp);
  തിരികെ fail;
}

/* Map a TS with negative TS.tv_nsec to {0,0}.  */
സ്ഥിതം വരിയിൽ ഘടന timespec
neg_to_zero (ഘടന timespec ts)
{
  എങ്കിൽ (0 <= ts.tv_nsec)
    തിരികെ ts;
  ഘടന timespec z = {0};
  തിരികെ z;
}

/* Output a single-character \ escape.  */

സ്ഥിതം ശൂന്യം
print_esc_char (അക്ഷരം c)
{
  തിരഞ്ഞെടുപ്പ് (c)
    {
    അവസ്ഥ 'a':			/* Alert. */
      c ='\a';
      നിർത്തുക;
    അവസ്ഥ 'b':			/* Backspace. */
      c ='\b';
      നിർത്തുക;
    അവസ്ഥ 'e':			/* Escape. */
      c ='\x1B';
      നിർത്തുക;
    അവസ്ഥ 'f':			/* Form feed. */
      c ='\f';
      നിർത്തുക;
    അവസ്ഥ 'n':			/* New line. */
      c ='\n';
      നിർത്തുക;
    അവസ്ഥ 'r':			/* Carriage return. */
      c ='\r';
      നിർത്തുക;
    അവസ്ഥ 't':			/* Horizontal tab. */
      c ='\t';
      നിർത്തുക;
    അവസ്ഥ 'v':			/* Vertical tab. */
      c ='\v';
      നിർത്തുക;
    അവസ്ഥ '"':
    അവസ്ഥ '\\':
      നിർത്തുക;
    സ്ഥിരസ്ഥിതി:
      error (0, 0, _("warning: unrecognized escape '\\%c'"), c);
      നിർത്തുക;
    }
  putchar (c);
}

ATTRIBUTE_PURE
സ്ഥിതം size_t
format_code_offset (അക്ഷരം സ്ഥിരം *directive)
{
  size_t len = strspn (directive + 1, printf_flags);
  അക്ഷരം സ്ഥിരം *fmt_char = directive + len + 1;
  fmt_char += strspn (fmt_char, digits);
  എങ്കിൽ (*fmt_char == '.')
    fmt_char += 1 + strspn (fmt_char + 1, digits);
  തിരികെ fmt_char - directive;
}

/* Print the information specified by the format string, FORMAT,
   calling PRINT_FUNC for each %-directive encountered.
   Return zero upon success, nonzero upon failure.  */
NODISCARD
സ്ഥിതം bool
print_it (അക്ഷരം സ്ഥിരം *format, പൂർണ്ണസംഖ്യ fd, അക്ഷരം സ്ഥിരം *filename,
          bool (*print_func) (അക്ഷരം *, size_t, അക്ഷരം, അക്ഷരം,
                              പൂർണ്ണസംഖ്യ, അക്ഷരം സ്ഥിരം *, ശൂന്യം സ്ഥിരം *),
          ശൂന്യം സ്ഥിരം *data)
{
  bool fail = false;

  /* Add 2 to accommodate our conversion of the stat '%s' format string
     to the longer printf '%llu' one.  */
  എണ്ണൽ
    {
      MAX_ADDITIONAL_BYTES =
        (MAX (വലുപ്പം "jd",
              MAX (വലുപ്പം "jo", MAX (വലുപ്പം "ju", വലുപ്പം "jx")))
         - 1)
    };
  size_t n_alloc = strlen (format) + MAX_ADDITIONAL_BYTES + 1;
  അക്ഷരം *dest = xmalloc (n_alloc);
  വേണ്ടി (അക്ഷരം സ്ഥിരം *b = format; *b; b++)
    {
      തിരഞ്ഞെടുപ്പ് (*b)
        {
        അവസ്ഥ '%':
          {
            size_t len = format_code_offset (b);
            അക്ഷരം fmt_char = *(b + len);
            അക്ഷരം mod_char = 0;
            memcpy (dest, b, len);
            b += len;

            തിരഞ്ഞെടുപ്പ് (fmt_char)
              {
              അവസ്ഥ '\0':
                --b;
                FALLTHROUGH;
              അവസ്ഥ '%':
                എങ്കിൽ (1 < len)
                  {
                    dest[len] = fmt_char;
                    dest[len + 1] = '\0';
                    error (EXIT_FAILURE, 0, _("%s: invalid directive"),
                           quote (dest));
                  }
                putchar ('%');
                നിർത്തുക;
              അവസ്ഥ 'H':
              അവസ്ഥ 'L':
              അവസ്ഥ 'Q':
                mod_char = fmt_char;
                fmt_char = *(b + 1);
                എങ്കിൽ ((mod_char == 'Q' && fmt_char == 'n')
                    || (print_func == print_stat
                        && (mod_char == 'H' || mod_char == 'L')
                        && (fmt_char == 'd' || fmt_char == 'r')))
                  {
                    b++;
                  }
                അല്ലെങ്കിൽ
                  {
                    fmt_char = mod_char;
                    mod_char = 0;
                  }
                FALLTHROUGH;
              സ്ഥിരസ്ഥിതി:
                fail |= print_func (dest, len, mod_char, fmt_char,
                                    fd, filename, data);
                നിർത്തുക;
              }
            നിർത്തുക;
          }

        അവസ്ഥ '\\':
          എങ്കിൽ ( ! interpret_backslash_escapes)
            {
              putchar ('\\');
              നിർത്തുക;
            }
          ++b;
          എങ്കിൽ (isoct (*b))
            {
              പൂർണ്ണസംഖ്യ esc_മൂല്യം = fromoct (*b);
              പൂർണ്ണസംഖ്യ esc_length = 1;	/* number of octal digits */
              വേണ്ടി (++b; esc_length < 3 && isoct (*b);
                   ++esc_length, ++b)
                {
                  esc_മൂല്യം = esc_മൂല്യം * 8 + fromoct (*b);
                }
              putchar (esc_മൂല്യം);
              --b;
            }
          അല്ലെങ്കിൽ എങ്കിൽ (*b == 'x' && c_isxdigit (b[1]))
            {
              പൂർണ്ണസംഖ്യ esc_മൂല്യം = fromhex (b[1]);	/* Value of \xhh escape. */
              /* A hexadecimal \xhh escape sequence must have
                 1 or 2 hex. digits.  */
              ++b;
              എങ്കിൽ (c_isxdigit (b[1]))
                {
                  ++b;
                  esc_മൂല്യം = esc_മൂല്യം * 16 + fromhex (*b);
                }
              putchar (esc_മൂല്യം);
            }
          അല്ലെങ്കിൽ എങ്കിൽ (*b == '\0')
            {
              error (0, 0, _("warning: backslash at end of format"));
              putchar ('\\');
              /* Arrange to exit the loop.  */
              --b;
            }
          അല്ലെങ്കിൽ
            {
              print_esc_char (*b);
            }
          നിർത്തുക;

        സ്ഥിരസ്ഥിതി:
          putchar (*b);
          നിർത്തുക;
        }
    }
  സ്വതന്ത്രം (dest);

  fputs (trailing_delim, stdout);

  തിരികെ fail;
}

/* Stat the file system and print what we find.  */
NODISCARD
സ്ഥിതം bool
do_statfs (അക്ഷരം സ്ഥിരം *filename, അക്ഷരം സ്ഥിരം *format)
{
  STRUCT_STATVFS statfsbuf;

  എങ്കിൽ (streq (filename, "-"))
    {
      error (0, 0, _("using %s to denote standard input does not work"
                     " in file system mode"), quoteaf (filename));
      തിരികെ false;
    }

  എങ്കിൽ (STATFS (filename, &statfsbuf) != 0)
    {
      error (0, errno, _("cannot read file system information for %s"),
             quoteaf (filename));
      തിരികെ false;
    }

  bool fail = print_it (format, -1, filename, print_statfs, &statfsbuf);
  തിരികെ ! fail;
}

ഘടന print_args {
  ഘടന stat *st;
  ഘടന timespec btime;
};

/* Ask statx to avoid syncing? */
സ്ഥിതം bool dont_sync;

/* Ask statx to force sync? */
സ്ഥിതം bool force_sync;

#എങ്കിൽ USE_STATX
സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ
fmt_to_mask (അക്ഷരം fmt)
{
  തിരഞ്ഞെടുപ്പ് (fmt)
    {
    അവസ്ഥ 'N':
      തിരികെ STATX_MODE;
    അവസ്ഥ 'd':
    അവസ്ഥ 'D':
      തിരികെ STATX_MODE;
    അവസ്ഥ 'i':
      തിരികെ STATX_INO;
    അവസ്ഥ 'a':
    അവസ്ഥ 'A':
      തിരികെ STATX_MODE;
    അവസ്ഥ 'f':
      തിരികെ STATX_MODE|STATX_TYPE;
    അവസ്ഥ 'F':
      തിരികെ STATX_TYPE;
    അവസ്ഥ 'h':
      തിരികെ STATX_NLINK;
    അവസ്ഥ 'u':
    അവസ്ഥ 'U':
      തിരികെ STATX_UID;
    അവസ്ഥ 'g':
    അവസ്ഥ 'G':
      തിരികെ STATX_GID;
    അവസ്ഥ 'm':
      തിരികെ STATX_MODE|STATX_INO;
    അവസ്ഥ 's':
      തിരികെ STATX_SIZE;
    അവസ്ഥ 't':
    അവസ്ഥ 'T':
      തിരികെ STATX_MODE;
    അവസ്ഥ 'b':
      തിരികെ STATX_BLOCKS;
    അവസ്ഥ 'w':
    അവസ്ഥ 'W':
      തിരികെ STATX_BTIME;
    അവസ്ഥ 'x':
    അവസ്ഥ 'X':
      തിരികെ STATX_ATIME;
    അവസ്ഥ 'y':
    അവസ്ഥ 'Y':
      തിരികെ STATX_MTIME;
    അവസ്ഥ 'z':
    അവസ്ഥ 'Z':
      തിരികെ STATX_CTIME;
    }
  തിരികെ 0;
}

ATTRIBUTE_PURE
സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ
format_to_mask (അക്ഷരം സ്ഥിരം *format)
{
  unsigned പൂർണ്ണസംഖ്യ mask = 0;

  വേണ്ടി (അക്ഷരം സ്ഥിരം *b = format; *b; b++)
    {
      എങ്കിൽ (*b != '%')
        തുടരുക;

      b += format_code_offset (b);
      എങ്കിൽ (*b == '\0')
        നിർത്തുക;
      mask |= fmt_to_mask (*b);
    }
  തിരികെ mask;
}

/* statx the file and print what we find */
NODISCARD
സ്ഥിതം bool
do_stat (അക്ഷരം സ്ഥിരം *filename, അക്ഷരം സ്ഥിരം *format, അക്ഷരം സ്ഥിരം *format2)
{
  പൂർണ്ണസംഖ്യ fd = streq (filename, "-") ? 0 : AT_FDCWD;
  പൂർണ്ണസംഖ്യ flags = 0;
  ഘടന stat st;
  ഘടന statx stx = {0};
  അക്ഷരം സ്ഥിരം *pathname = filename;
  ഘടന print_args pa;
  pa.st = &st;
  pa.btime = (ഘടന timespec) {.tv_sec = -1, .tv_nsec = -1};

  എങ്കിൽ (AT_FDCWD != fd)
    {
      pathname = "";
      flags = AT_EMPTY_PATH;
    }
  അല്ലെങ്കിൽ എങ്കിൽ (!follow_links)
    {
      flags = AT_SYMLINK_NOFOLLOW;
    }

  എങ്കിൽ (dont_sync)
    flags |= AT_STATX_DONT_SYNC;
  അല്ലെങ്കിൽ എങ്കിൽ (force_sync)
    flags |= AT_STATX_FORCE_SYNC;

  എങ്കിൽ (! force_sync)
    flags |= AT_NO_AUTOMOUNT;

  fd = statx (fd, pathname, flags, format_to_mask (format), &stx);
  എങ്കിൽ (fd < 0)
    {
      എങ്കിൽ (flags & AT_EMPTY_PATH)
        error (0, errno, _("cannot stat standard input"));
      അല്ലെങ്കിൽ
        error (0, errno, _("cannot statx %s"), quoteaf (filename));
      തിരികെ false;
    }

  എങ്കിൽ (S_ISBLK (stx.stx_mode) || S_ISCHR (stx.stx_mode))
    format = format2;

  statx_to_stat (&stx, &st);
  എങ്കിൽ (stx.stx_mask & STATX_BTIME)
    pa.btime = statx_timestamp_to_timespec (stx.stx_btime);

  bool fail = print_it (format, fd, filename, print_stat, &pa);
  തിരികെ ! fail;
}

#അല്ലെങ്കിൽ /* USE_STATX */

സ്ഥിതം ഘടന timespec
get_birthtime (പൂർണ്ണസംഖ്യ fd, അക്ഷരം സ്ഥിരം *filename, ഘടന stat സ്ഥിരം *st)
{
  ഘടന timespec ts = get_stat_birthtime (st);

# എങ്കിൽ HAVE_GETATTRAT
  എങ്കിൽ (ts.tv_nsec < 0)
    {
      nvlist_t *response;
      എങ്കിൽ ((fd < 0
           ? getattrat (AT_FDCWD, XATTR_VIEW_READWRITE, filename, &response)
           : fgetattr (fd, XATTR_VIEW_READWRITE, &response))
          == 0)
        {
          uint64_t *val;
          uint_t n;
          എങ്കിൽ (nvlist_lookup_uint64_array (response, A_CRTIME, &val, &n) == 0
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

  തിരികെ ts;
}


/* stat the file and print what we find */
NODISCARD
സ്ഥിതം bool
do_stat (അക്ഷരം സ്ഥിരം *filename, അക്ഷരം സ്ഥിരം *format,
         അക്ഷരം സ്ഥിരം *format2)
{
  പൂർണ്ണസംഖ്യ fd = streq (filename, "-") ? 0 : -1;
  ഘടന stat statbuf;
  ഘടന print_args pa;
  pa.st = &statbuf;
  pa.btime = (ഘടന timespec) {.tv_sec = -1, .tv_nsec = -1};

  എങ്കിൽ (0 <= fd)
    {
      എങ്കിൽ (fstat (fd, &statbuf) != 0)
        {
          error (0, errno, _("cannot stat standard input"));
          തിരികെ false;
        }
    }
  /* We can't use the shorter
     (follow_links?stat:lstat) (filename, &statbug)
     since stat might be a function-like macro.  */
  അല്ലെങ്കിൽ എങ്കിൽ ((follow_links
            ? stat (filename, &statbuf)
            : lstat (filename, &statbuf)) != 0)
    {
      error (0, errno, _("cannot stat %s"), quoteaf (filename));
      തിരികെ false;
    }

  എങ്കിൽ (S_ISBLK (statbuf.st_mode) || S_ISCHR (statbuf.st_mode))
    format = format2;

  bool fail = print_it (format, fd, filename, print_stat, &pa);
  തിരികെ ! fail;
}
#endif /* USE_STATX */

/* POSIX requires 'ls' to print file sizes without a sign, even
   when negative.  Be consistent with that.  */

സ്ഥിതം uintmax_t
unsigned_file_size (off_t size)
{
  തിരികെ size + (size < 0) * ((uintmax_t) OFF_T_MAX - OFF_T_MIN + 1);
}

/* Print stat info.  Return zero upon success, nonzero upon failure.  */
സ്ഥിതം bool
print_stat (അക്ഷരം *pformat, size_t prefix_len, അക്ഷരം mod, അക്ഷരം m,
            MAYBE_UNUSED പൂർണ്ണസംഖ്യ fd, അക്ഷരം സ്ഥിരം *filename, ശൂന്യം സ്ഥിരം *data)
{
  ഘടന print_args *parg = (ഘടന print_args *) data;
  ഘടന stat *statbuf = parg->st;
  ഘടന timespec btime = parg->btime;
  ഘടന passwd *pw_ent;
  ഘടന group *gw_ent;
  bool fail = false;

  തിരഞ്ഞെടുപ്പ് (m)
    {
    അവസ്ഥ 'n':
      എങ്കിൽ (mod == 'Q')
        {
          initialize_quoting_style ();
          filename = quoteN (filename);
        }
      out_string (pformat, prefix_len, filename);
      നിർത്തുക;
    അവസ്ഥ 'N':
      initialize_quoting_style ();
      out_string (pformat, prefix_len, quoteN (filename));
      എങ്കിൽ (S_ISLNK (statbuf->st_mode))
        {
          അക്ഷരം *linkname = areadlink_with_size (filename, statbuf->st_size);
          എങ്കിൽ (linkname == ശൂന്യ)
            {
              error (0, errno, _("cannot read symbolic link %s"),
                     quoteaf (filename));
              തിരികെ true;
            }
          printf (" -> ");
          out_string (pformat, prefix_len, quoteN (linkname));
          സ്വതന്ത്രം (linkname);
        }
      നിർത്തുക;
    അവസ്ഥ 'd':
      എങ്കിൽ (mod == 'H')
        out_uint (pformat, prefix_len, major (statbuf->st_dev));
      അല്ലെങ്കിൽ എങ്കിൽ (mod == 'L')
        out_uint (pformat, prefix_len, minor (statbuf->st_dev));
      അല്ലെങ്കിൽ
        out_uint (pformat, prefix_len, statbuf->st_dev);
      നിർത്തുക;
    അവസ്ഥ 'D':
      out_uint_x (pformat, prefix_len, statbuf->st_dev);
      നിർത്തുക;
    അവസ്ഥ 'i':
      out_uint (pformat, prefix_len, statbuf->st_ino);
      നിർത്തുക;
    അവസ്ഥ 'a':
      out_uint_o (pformat, prefix_len, statbuf->st_mode & CHMOD_MODE_BITS);
      നിർത്തുക;
    അവസ്ഥ 'A':
      out_string (pformat, prefix_len, human_access (statbuf));
      നിർത്തുക;
    അവസ്ഥ 'f':
      out_uint_x (pformat, prefix_len, statbuf->st_mode);
      നിർത്തുക;
    അവസ്ഥ 'F':
      out_string (pformat, prefix_len, file_type (statbuf));
      നിർത്തുക;
    അവസ്ഥ 'h':
      out_uint (pformat, prefix_len, statbuf->st_nlink);
      നിർത്തുക;
    അവസ്ഥ 'u':
      out_uint (pformat, prefix_len, statbuf->st_uid);
      നിർത്തുക;
    അവസ്ഥ 'U':
      pw_ent = getpwuid (statbuf->st_uid);
      out_string (pformat, prefix_len,
                  pw_ent ? pw_ent->pw_name : "UNKNOWN");
      നിർത്തുക;
    അവസ്ഥ 'g':
      out_uint (pformat, prefix_len, statbuf->st_gid);
      നിർത്തുക;
    അവസ്ഥ 'G':
      gw_ent = getgrgid (statbuf->st_gid);
      out_string (pformat, prefix_len,
                  gw_ent ? gw_ent->gr_name : "UNKNOWN");
      നിർത്തുക;
    അവസ്ഥ 'm':
      fail |= out_mount_point (filename, pformat, prefix_len, statbuf);
      നിർത്തുക;
    അവസ്ഥ 's':
      out_uint (pformat, prefix_len, unsigned_file_size (statbuf->st_size));
      നിർത്തുക;
    അവസ്ഥ 'r':
      എങ്കിൽ (mod == 'H')
        out_uint (pformat, prefix_len, major (statbuf->st_rdev));
      അല്ലെങ്കിൽ എങ്കിൽ (mod == 'L')
        out_uint (pformat, prefix_len, minor (statbuf->st_rdev));
      അല്ലെങ്കിൽ
        out_uint (pformat, prefix_len, statbuf->st_rdev);
      നിർത്തുക;
    അവസ്ഥ 'R':
      out_uint_x (pformat, prefix_len, statbuf->st_rdev);
      നിർത്തുക;
    അവസ്ഥ 't':
      out_uint_x (pformat, prefix_len, major (statbuf->st_rdev));
      നിർത്തുക;
    അവസ്ഥ 'T':
      out_uint_x (pformat, prefix_len, minor (statbuf->st_rdev));
      നിർത്തുക;
    അവസ്ഥ 'B':
      out_uint (pformat, prefix_len, ST_NBLOCKSIZE);
      നിർത്തുക;
    അവസ്ഥ 'b':
      out_uint (pformat, prefix_len, STP_NBLOCKS (statbuf));
      നിർത്തുക;
    അവസ്ഥ 'o':
      out_uint (pformat, prefix_len, STP_BLKSIZE (statbuf));
      നിർത്തുക;
    അവസ്ഥ 'w':
      {
#എങ്കിൽ ! USE_STATX
        btime = get_birthtime (fd, filename, statbuf);
#endif
        എങ്കിൽ (btime.tv_nsec < 0)
          out_string (pformat, prefix_len, "-");
        അല്ലെങ്കിൽ
          out_string (pformat, prefix_len, human_time (btime));
      }
      നിർത്തുക;
    അവസ്ഥ 'W':
      {
#എങ്കിൽ ! USE_STATX
        btime = get_birthtime (fd, filename, statbuf);
#endif
        out_epoch_sec (pformat, prefix_len, neg_to_zero (btime));
      }
      നിർത്തുക;
    അവസ്ഥ 'x':
      out_string (pformat, prefix_len, human_time (get_stat_atime (statbuf)));
      നിർത്തുക;
    അവസ്ഥ 'X':
      out_epoch_sec (pformat, prefix_len, get_stat_atime (statbuf));
      നിർത്തുക;
    അവസ്ഥ 'y':
      out_string (pformat, prefix_len, human_time (get_stat_mtime (statbuf)));
      നിർത്തുക;
    അവസ്ഥ 'Y':
      out_epoch_sec (pformat, prefix_len, get_stat_mtime (statbuf));
      നിർത്തുക;
    അവസ്ഥ 'z':
      out_string (pformat, prefix_len, human_time (get_stat_ctime (statbuf)));
      നിർത്തുക;
    അവസ്ഥ 'Z':
      out_epoch_sec (pformat, prefix_len, get_stat_ctime (statbuf));
      നിർത്തുക;
    അവസ്ഥ 'C':
      fail |= out_file_context (pformat, prefix_len, filename);
      നിർത്തുക;
    സ്ഥിരസ്ഥിതി:
      fputc ('?', stdout);
      നിർത്തുക;
    }
  തിരികെ fail;
}

/* Return an allocated format string in static storage that
   corresponds to whether FS and TERSE options were declared.  */
സ്ഥിതം അക്ഷരം *
default_format (bool fs, bool terse, bool device)
{
  അക്ഷരം *format;
  എങ്കിൽ (fs)
    {
      എങ്കിൽ (terse)
        format = xstrdup (fmt_terse_fs);
      അല്ലെങ്കിൽ
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
  അല്ലെങ്കിൽ /* ! fs */
    {
      എങ്കിൽ (terse)
        {
          എങ്കിൽ (0 < is_selinux_enabled ())
            format = xstrdup (fmt_terse_selinux);
          അല്ലെങ്കിൽ
            format = xstrdup (fmt_terse_regular);
        }
      അല്ലെങ്കിൽ
        {
          അക്ഷരം *temp;
          /* TRANSLATORS: This string uses format specifiers from
             'stat --help' without --file-system, and NOT from printf.  */
          format = xstrdup (_("\
  File: %N\n\
  Size: %-10s\tBlocks: %-10b IO Block: %-6o %F\n\
"));

          temp = format;
          എങ്കിൽ (device)
            {
              /* TRANSLATORS: This string uses format specifiers from
                 'stat --help' without --file-system, and NOT from printf.  */
              format = xasprintf ("%s%s", format, _("\
" "Device: %Hd,%Ld\tInode: %-10i  Links: %-5h Device type: %Hr,%Lr\n\
"));
            }
          അല്ലെങ്കിൽ
            {
              /* TRANSLATORS: This string uses format specifiers from
                 'stat --help' without --file-system, and NOT from printf.  */
              format = xasprintf ("%s%s", format, _("\
" "Device: %Hd,%Ld\tInode: %-10i  Links: %h\n\
"));
            }
          സ്വതന്ത്രം (temp);

          temp = format;
          /* TRANSLATORS: This string uses format specifiers from
             'stat --help' without --file-system, and NOT from printf.  */
          format = xasprintf ("%s%s", format, _("\
" "Access: (%04a/%10.10A)  Uid: (%5u/%8U)   Gid: (%5g/%8G)\n\
"));
          സ്വതന്ത്രം (temp);

          എങ്കിൽ (0 < is_selinux_enabled ())
            {
              temp = format;
              /* TRANSLATORS: This string uses format specifiers from
                 'stat --help' without --file-system, and NOT from printf.  */
              format = xasprintf ("%s%s", format, _("Context: %C\n"));
              സ്വതന്ത്രം (temp);
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
          സ്വതന്ത്രം (temp);
        }
    }
  തിരികെ format;
}

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
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
         use the specified FORMAT instead of the സ്ഥിരസ്ഥിതി;\n\
         output a newline after each use of FORMAT\n\
"));
      oputs (_("\
      --printf=FORMAT\n\
         like --format, but interpret backslash escapes,\n\
         and ചെയ്യുക not output a mandatory trailing newline;\n\
         എങ്കിൽ you want a newline, include \\n in FORMAT\n\
"));
      oputs (_("\
  -t, --terse\n\
         print the information in terse form\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);

      fputs (_("\n\
The MODE argument of --cached can be: always, never, or സ്ഥിരസ്ഥിതി.\n\
'always' will use cached attributes എങ്കിൽ available, ആകുമ്പോൾ\n\
'never' will try to synchronize with the latest attributes, and\n\
'default' will leave it up to the underlying file system.\n\
"), stdout);

      fputs (_("\n\
The valid format sequences വേണ്ടി files (without --file-system):\n\
\n\
  %a   permission bits in octal (see '#' and '0' printf flags)\n\
  %A   permission bits and file type in human readable form\n\
  %b   സംഖ്യ of blocks allocated (see %B)\n\
  %B   the size in bytes of each block reported by %b\n\
  %C   SELinux security context string\n\
"), stdout);
      fputs (_("\
  %d   device സംഖ്യ in decimal (st_dev)\n\
  %D   device സംഖ്യ in hex (st_dev)\n\
  %Hd  major device സംഖ്യ in decimal\n\
  %Ld  minor device സംഖ്യ in decimal\n\
  %f   raw mode in hex\n\
  %F   file type\n\
  %g   group ID of owner\n\
  %G   group name of owner\n\
"), stdout);
      fputs (_("\
  %h   സംഖ്യ of hard links\n\
  %i   inode സംഖ്യ\n\
  %m   mount point\n\
  %n   file name\n\
  %Qn  quoted file name\n\
  %N   quoted file name with dereference എങ്കിൽ symbolic link\n\
  %o   optimal I/O transfer size hint\n\
  %s   total size, in bytes\n\
  %r   device type in decimal (st_rdev)\n\
  %R   device type in hex (st_rdev)\n\
  %Hr  major device type in decimal, വേണ്ടി character/block device special files\n\
  %Lr  minor device type in decimal, വേണ്ടി character/block device special files\n\
  %t   major device type in hex, വേണ്ടി character/block device special files\n\
  %T   minor device type in hex, വേണ്ടി character/block device special files\n\
"), stdout);
      fputs (_("\
  %u   user ID of owner\n\
  %U   user name of owner\n\
  %w   time of file birth, human-readable; - എങ്കിൽ unknown\n\
  %W   time of file birth, seconds since Epoch; 0 എങ്കിൽ unknown\n\
  %x   time of last access, human-readable\n\
  %X   time of last access, seconds since Epoch\n\
  %y   time of last data modification, human-readable\n\
  %Y   time of last data modification, seconds since Epoch\n\
  %z   time of last status change, human-readable\n\
  %Z   time of last status change, seconds since Epoch\n\
\n\
"), stdout);

      fputs (_("\
Valid format sequences വേണ്ടി file systems:\n\
\n\
  %a   സ്വതന്ത്രം blocks available to non-superuser\n\
  %b   total data blocks in file system\n\
  %c   total file nodes in file system\n\
  %d   സ്വതന്ത്രം file nodes in file system\n\
  %f   സ്വതന്ത്രം blocks in file system\n\
"), stdout);
      fputs (_("\
  %i   file system ID in hex\n\
  %l   maximum length of filenames\n\
  %n   file name\n\
  %Qn  quoted file name\n\
  %s   block size (വേണ്ടി faster transfers)\n\
  %S   fundamental block size (വേണ്ടി block counts)\n\
  %t   file system type in hex\n\
  %T   file system type in human readable form\n\
"), stdout);

      printf (_("\n\
--terse is equivalent to the following FORMAT:\n\
    %s\
"),
#എങ്കിൽ HAVE_SELINUX_SELINUX_H
              fmt_terse_selinux
#അല്ലെങ്കിൽ
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

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം *argv[])
{
  പൂർണ്ണസംഖ്യ c;
  bool fs = false;
  bool terse = false;
  അക്ഷരം *format = ശൂന്യ;
  അക്ഷരം *format2;
  bool ok = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  ഘടന lconv സ്ഥിരം *locale = localeconv ();
  decimal_point = (locale->decimal_point[0] ? locale->decimal_point : ".");
  decimal_point_len = strlen (decimal_point);

  atexit (close_stdout);

  ആകുമ്പോൾ ((c = getopt_long (argc, argv, "c:fLt", long_options, ശൂന്യ)) != -1)
    {
      തിരഞ്ഞെടുപ്പ് (c)
        {
        അവസ്ഥ PRINTF_OPTION:
          format = optarg;
          interpret_backslash_escapes = true;
          trailing_delim = "";
          നിർത്തുക;

        അവസ്ഥ 'c':
          format = optarg;
          interpret_backslash_escapes = false;
          trailing_delim = "\n";
          നിർത്തുക;

        അവസ്ഥ 'L':
          follow_links = true;
          നിർത്തുക;

        അവസ്ഥ 'f':
          fs = true;
          നിർത്തുക;

        അവസ്ഥ 't':
          terse = true;
          നിർത്തുക;

        അവസ്ഥ 0:
          തിരഞ്ഞെടുപ്പ് (XARGMATCH ("--cached", optarg, cached_args, cached_modes))
            {
              അവസ്ഥ cached_never:
                force_sync = true;
                dont_sync = false;
                നിർത്തുക;
              അവസ്ഥ cached_always:
                force_sync = false;
                dont_sync = true;
                നിർത്തുക;
              അവസ്ഥ cached_default:
                force_sync = false;
                dont_sync = false;
            }
          നിർത്തുക;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        സ്ഥിരസ്ഥിതി:
          usage (EXIT_FAILURE);
        }
    }

  എങ്കിൽ (argc == optind)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  എങ്കിൽ (format)
    {
      format2 = format;
    }
  അല്ലെങ്കിൽ
    {
      format = default_format (fs, terse, /* device= */ false);
      format2 = default_format (fs, terse, /* device= */ true);
    }

  വേണ്ടി (പൂർണ്ണസംഖ്യ i = optind; i < argc; i++)
    ok &= (fs
           ? do_statfs (argv[i], format)
           : do_stat (argv[i], format, format2));

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
