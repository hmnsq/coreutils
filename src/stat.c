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

#ଅନ୍ତର୍ଭାଵ <config.h>

/* Keep this conditional in sync with the similar conditional in
   ../m4/stat-prog.m4.  */
#ଯଦି ((STAT_STATVFS || STAT_STATVFS64)                                       \
     && (HAVE_STRUCT_STATVFS_F_BASETYPE || HAVE_STRUCT_STATVFS_F_FSTYPENAME \
         || (! HAVE_STRUCT_STATFS_F_FSTYPENAME && HAVE_STRUCT_STATVFS_F_TYPE)))
# define USE_STATVFS 1
#ଅନ୍ୟଥା
# define USE_STATVFS 0
#endif

#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <pwd.h>
#ଅନ୍ତର୍ଭାଵ <grp.h>
#ଯଦି USE_STATVFS
# include <sys/statvfs.h>
#ନଚେତଯଦି HAVE_SYS_VFS_H
# include <sys/vfs.h>
#ନଚେତଯଦି HAVE_SYS_MOUNT_H && HAVE_SYS_PARAM_H
/* NOTE: freebsd5.0 needs sys/param.h and sys/mount.h for statfs.
   It does have statvfs.h, but shouldn't use it, since it doesn't
   HAVE_STRUCT_STATVFS_F_BASETYPE.  So find a clean way to fix it.  */
/* NetBSD 1.5.2 needs these, for the declaration of struct statfs. */
# include <sys/param.h>
# include <sys/mount.h>
# ଯଦି HAVE_NFS_NFS_CLNT_H && HAVE_NFS_VFS_H
/* Ultrix 4.4 needs these for the declaration of struct statfs.  */
#  include <netinet/in.h>
#  include <nfs/nfs_clnt.h>
#  include <nfs/vfs.h>
# endif
#ନଚେତଯଦି HAVE_OS_H /* BeOS */
# include <fs_info.h>
#endif
#ଅନ୍ତର୍ଭାଵ <selinux/selinux.h>
#ଅନ୍ତର୍ଭାଵ <getopt.h>

#ଅନ୍ତର୍ଭାଵ "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#ଅନ୍ତର୍ଭାଵ "system.h"

#ଅନ୍ତର୍ଭାଵ "areadlink.h"
#ଅନ୍ତର୍ଭାଵ "c-ctype.h"
#ଅନ୍ତର୍ଭାଵ "file-type.h"
#ଅନ୍ତର୍ଭାଵ "filemode.h"
#ଅନ୍ତର୍ଭାଵ "fs.h"
#ଅନ୍ତର୍ଭାଵ "mountlist.h"
#ଅନ୍ତର୍ଭାଵ "octhexdigits.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "stat-size.h"
#ଅନ୍ତର୍ଭାଵ "stat-time.h"
#ଅନ୍ତର୍ଭାଵ "strftime.h"
#ଅନ୍ତର୍ଭାଵ "find-mount-point.h"
#ଅନ୍ତର୍ଭାଵ "xvasprintf.h"
#ଅନ୍ତର୍ଭାଵ "statx.h"

#ଯଦି HAVE_STATX && defined STATX_INO
# define USE_STATX 1
#ଅନ୍ୟଥା
# define USE_STATX 0
#endif

#ଯଦି USE_STATVFS
# define STRUCT_STATXFS_F_FSID_IS_INTEGER STRUCT_STATVFS_F_FSID_IS_INTEGER
# define HAVE_STRUCT_STATXFS_F_TYPE HAVE_STRUCT_STATVFS_F_TYPE
# ଯଦି HAVE_STRUCT_STATVFS_F_NAMEMAX
#  define SB_F_NAMEMAX(S) ((S)->f_namemax)
# endif
# ଯଦି ! STAT_STATVFS && STAT_STATVFS64
#  define STRUCT_STATVFS ସଂରଚନା statvfs64
#  define STATFS statvfs64
# ଅନ୍ୟଥା
#  define STRUCT_STATVFS ସଂରଚନା statvfs
#  define STATFS statvfs
# endif
# define STATFS_FRSIZE(S) ((S)->f_frsize)
#ଅନ୍ୟଥା
# define HAVE_STRUCT_STATXFS_F_TYPE HAVE_STRUCT_STATFS_F_TYPE
# ଯଦି HAVE_STRUCT_STATFS_F_NAMELEN
#  define SB_F_NAMEMAX(S) ((S)->f_namelen)
# elif HAVE_STRUCT_STATFS_F_NAMEMAX
#  define SB_F_NAMEMAX(S) ((S)->f_namemax)
# endif
# define STATFS statfs
# ଯଦି HAVE_OS_H /* BeOS */
/* BeOS has a statvfs function, but it does not return sensible values
   for f_files, f_ffree and f_favail, and lacks f_type, f_basetype and
   f_fstypename.  Use 'struct fs_info' instead.  */
NODISCARD
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
statfs (ଅକ୍ଷର ଅଚଳ *filename, ସଂରଚନା fs_info *buf)
{
  dev_t device = dev_for_path (filename);
  ଯଦି (device < 0)
    {
      errno = (device == B_ENTRY_NOT_FOUND ? ENOENT
               : device == B_BAD_ମୂଲ୍ୟ ? EINVAL
               : device == B_NAME_TOO_LONG ? ENAMETOOLONG
               : device == B_NO_MEMORY ? ENOMEM
               : device == B_FILE_ERROR ? EIO
               : 0);
      ଫେରନ୍ତୁ -1;
    }
  /* If successful, buf->dev will be == device.  */
  ଫେରନ୍ତୁ fs_stat_dev (device, buf);
}
#  define f_fsid dev
#  define f_blocks total_blocks
#  define f_bfree free_blocks
#  define f_bavail free_blocks
#  define f_bsize io_size
#  define f_files total_nodes
#  define f_ffree free_nodes
#  define STRUCT_STATVFS ସଂରଚନା fs_info
#  define STRUCT_STATXFS_F_FSID_IS_INTEGER true
#  define STATFS_FRSIZE(S) ((S)->block_size)
# ଅନ୍ୟଥା
#  define STRUCT_STATVFS ସଂରଚନା statfs
#  define STRUCT_STATXFS_F_FSID_IS_INTEGER STRUCT_STATFS_F_FSID_IS_INTEGER
#  ଯଦି HAVE_STRUCT_STATFS_F_FRSIZE
#   define STATFS_FRSIZE(S) ((S)->f_frsize)
#  ଅନ୍ୟଥା
#   define STATFS_FRSIZE(S) 0
#  endif
# endif
#endif

#ifdef SB_F_NAMEMAX
# define OUT_NAMEMAX out_uint
#ଅନ୍ୟଥା
/* Depending on whether statvfs or statfs is used,
   neither f_namemax or f_namelen may be available.  */
# define SB_F_NAMEMAX(S) "?"
# define OUT_NAMEMAX out_string
#endif

#ଯଦି HAVE_STRUCT_STATVFS_F_BASETYPE
# define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME f_basetype
#ଅନ୍ୟଥା
# ଯଦି HAVE_STRUCT_STATVFS_F_FSTYPENAME || HAVE_STRUCT_STATFS_F_FSTYPENAME
#  define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME f_fstypename
# elif HAVE_OS_H /* BeOS */
#  define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME fsh_name
# endif
#endif

#ଯଦି HAVE_GETATTRAT
# include <attr.h>
# include <sys/nvpair.h>
#endif

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ digits[] = "0123456789";

/* Flags that are portable for use in printf, for at least one
   conversion specifier; make_format removes non-portable flags as
   needed for particular specifiers.  The glibc 2.2 extension "I" is
   listed here; it is removed by make_format because it has undefined
   behavior elsewhere and because it is incompatible with
   out_epoch_sec.  */
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ printf_flags[] = "'-+ #0I";

/* Formats for the --terse option.  */
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ fmt_terse_fs[] = "%Qn %i %l %t %s %S %b %f %a %c %d\n";
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ fmt_terse_regular[] = "%Qn %s %b %f %u %g %D %i %h %t %T"
                                        " %X %Y %Z %W %o\n";
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ fmt_terse_selinux[] = "%Qn %s %b %f %u %g %D %i %h %t %T"
                                        " %X %Y %Z %W %o %C\n";

#define PROGRAM_NAME "stat"

#define AUTHORS proper_name ("Michael Meskes")

ଗଣନା
{
  PRINTF_OPTION = CHAR_MAX + 1
};

ଗଣନା cached_mode
{
  cached_default,
  cached_never,
  cached_always
};

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *ଅଚଳ cached_args[] =
{
  "default", "never", "always", ଖାଲି
};

ସ୍ଥିର ଗଣନା cached_mode ଅଚଳ cached_modes[] =
{
  cached_default, cached_never, cached_always
};

ସ୍ଥିର ସଂରଚନା option ଅଚଳ long_options[] =
{
  {"dereference", no_argument, ଖାଲି, 'L'},
  {"file-system", no_argument, ଖାଲି, 'f'},
  {"format", required_argument, ଖାଲି, 'c'},
  {"printf", required_argument, ଖାଲି, PRINTF_OPTION},
  {"terse", no_argument, ଖାଲି, 't'},
  {"cached", required_argument, ଖାଲି, 0},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ଖାଲି, 0, ଖାଲି, 0}
};

/* Whether to follow symbolic links;  True for --dereference (-L).  */
ସ୍ଥିର bool follow_links;

/* Whether to interpret backslash-escape sequences.
   True for --printf=FMT, not for --format=FMT (-c).  */
ସ୍ଥିର bool interpret_backslash_escapes;

/* The trailing delimiter string:
   "" for --printf=FMT, "\n" for --format=FMT (-c).  */
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *trailing_delim = "";

/* The representation of the decimal point in the current locale.  */
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *decimal_point;
ସ୍ଥିର size_t decimal_point_len;

ସ୍ଥିର bool
print_stat (ଅକ୍ଷର *pformat, size_t prefix_len, ଅକ୍ଷର mod, ଅକ୍ଷର m,
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅକ୍ଷର ଅଚଳ *filename, ଶୂନ୍ୟ ଅଚଳ *data);

/* Return the type of the specified file system.
   Some systems have statfvs.f_basetype[FSTYPSZ] (AIX, HP-UX, and Solaris).
   Others have statvfs.f_fstypename[_VFS_NAMELEN] (NetBSD 3.0).
   Others have statfs.f_fstypename[MFSNAMELEN] (NetBSD 1.5.2).
   Still others have neither and have to get by with f_type (GNU/Linux).
   But f_type may only exist in statfs (Cygwin).  */
NODISCARD
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *
human_fstype (STRUCT_STATVFS ଅଚଳ *statfsbuf)
{
#ifdef STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME
  ଫେରନ୍ତୁ statfsbuf->STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME;
#ଅନ୍ୟଥା
  ବାଛ (statfsbuf->f_type)
    {
# ଯଦି defined __linux__ || defined __ANDROID__

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

    ସ୍ଥିତି S_MAGIC_AAFS: /* 0x5A3C69F0 local */
      ଫେରନ୍ତୁ "aafs";
    ସ୍ଥିତି S_MAGIC_ACFS: /* 0x61636673 remote */
      ଫେରନ୍ତୁ "acfs";
    ସ୍ଥିତି S_MAGIC_ADFS: /* 0xADF5 local */
      ଫେରନ୍ତୁ "adfs";
    ସ୍ଥିତି S_MAGIC_AFFS: /* 0xADFF local */
      ଫେରନ୍ତୁ "affs";
    ସ୍ଥିତି S_MAGIC_AFS: /* 0x5346414F remote */
      ଫେରନ୍ତୁ "afs";
    ସ୍ଥିତି S_MAGIC_ANON_INODE_FS: /* 0x09041934 local */
      ଫେରନ୍ତୁ "anon-inode FS";
    ସ୍ଥିତି S_MAGIC_AUFS: /* 0x61756673 remote */
      /* FIXME: change syntax or add an optional attribute like "inotify:no".
         The above is labeled as "remote" so that tail always uses polling,
         but this isn't really a remote file system type.  */
      ଫେରନ୍ତୁ "aufs";
    ସ୍ଥିତି S_MAGIC_AUTOFS: /* 0x0187 local */
      ଫେରନ୍ତୁ "autofs";
    ସ୍ଥିତି S_MAGIC_BALLOON_KVM: /* 0x13661366 local */
      ଫେରନ୍ତୁ "balloon-kvm-fs";
    ସ୍ଥିତି S_MAGIC_BCACHEFS: /* 0xCA451A4E local */
      ଫେରନ୍ତୁ "bcachefs";
    ସ୍ଥିତି S_MAGIC_BEFS: /* 0x42465331 local */
      ଫେରନ୍ତୁ "befs";
    ସ୍ଥିତି S_MAGIC_BDEVFS: /* 0x62646576 local */
      ଫେରନ୍ତୁ "bdevfs";
    ସ୍ଥିତି S_MAGIC_BFS: /* 0x1BADFACE local */
      ଫେରନ୍ତୁ "bfs";
    ସ୍ଥିତି S_MAGIC_BINDERFS: /* 0x6C6F6F70 local */
      ଫେରନ୍ତୁ "binderfs";
    ସ୍ଥିତି S_MAGIC_BPF_FS: /* 0xCAFE4A11 local */
      ଫେରନ୍ତୁ "bpf_fs";
    ସ୍ଥିତି S_MAGIC_BINFMTFS: /* 0x42494E4D local */
      ଫେରନ୍ତୁ "binfmt_misc";
    ସ୍ଥିତି S_MAGIC_BTRFS: /* 0x9123683E local */
      ଫେରନ୍ତୁ "btrfs";
    ସ୍ଥିତି S_MAGIC_BTRFS_TEST: /* 0x73727279 local */
      ଫେରନ୍ତୁ "btrfs_test";
    ସ୍ଥିତି S_MAGIC_CEPH: /* 0x00C36400 remote */
      ଫେରନ୍ତୁ "ceph";
    ସ୍ଥିତି S_MAGIC_CGROUP: /* 0x0027E0EB local */
      ଫେରନ୍ତୁ "cgroupfs";
    ସ୍ଥିତି S_MAGIC_CGROUP2: /* 0x63677270 local */
      ଫେରନ୍ତୁ "cgroup2fs";
    ସ୍ଥିତି S_MAGIC_CIFS: /* 0xFF534D42 remote */
      ଫେରନ୍ତୁ "cifs";
    ସ୍ଥିତି S_MAGIC_CODA: /* 0x73757245 remote */
      ଫେରନ୍ତୁ "coda";
    ସ୍ଥିତି S_MAGIC_COH: /* 0x012FF7B7 local */
      ଫେରନ୍ତୁ "coh";
    ସ୍ଥିତି S_MAGIC_CONFIGFS: /* 0x62656570 local */
      ଫେରନ୍ତୁ "configfs";
    ସ୍ଥିତି S_MAGIC_CRAMFS: /* 0x28CD3D45 local */
      ଫେରନ୍ତୁ "cramfs";
    ସ୍ଥିତି S_MAGIC_CRAMFS_WEND: /* 0x453DCD28 local */
      ଫେରନ୍ତୁ "cramfs-wend";
    ସ୍ଥିତି S_MAGIC_DAXFS: /* 0x64646178 local */
      ଫେରନ୍ତୁ "daxfs";
    ସ୍ଥିତି S_MAGIC_DEBUGFS: /* 0x64626720 local */
      ଫେରନ୍ତୁ "debugfs";
    ସ୍ଥିତି S_MAGIC_DEVFS: /* 0x1373 local */
      ଫେରନ୍ତୁ "devfs";
    ସ୍ଥିତି S_MAGIC_DEVMEM: /* 0x454D444D local */
      ଫେରନ୍ତୁ "devmem";
    ସ୍ଥିତି S_MAGIC_DEVPTS: /* 0x1CD1 local */
      ଫେରନ୍ତୁ "devpts";
    ସ୍ଥିତି S_MAGIC_DMA_BUF: /* 0x444D4142 local */
      ଫେରନ୍ତୁ "dma-buf-fs";
    ସ୍ଥିତି S_MAGIC_ECRYPTFS: /* 0xF15F local */
      ଫେରନ୍ତୁ "ecryptfs";
    ସ୍ଥିତି S_MAGIC_EFIVARFS: /* 0xDE5E81E4 local */
      ଫେରନ୍ତୁ "efivarfs";
    ସ୍ଥିତି S_MAGIC_EFS: /* 0x00414A53 local */
      ଫେରନ୍ତୁ "efs";
    ସ୍ଥିତି S_MAGIC_EROFS_V1: /* 0xE0F5E1E2 local */
      ଫେରନ୍ତୁ "erofs";
    ସ୍ଥିତି S_MAGIC_EXFAT: /* 0x2011BAB0 local */
      ଫେରନ୍ତୁ "exfat";
    ସ୍ଥିତି S_MAGIC_EXFS: /* 0x45584653 local */
      ଫେରନ୍ତୁ "exfs";
    ସ୍ଥିତି S_MAGIC_EXOFS: /* 0x5DF5 local */
      ଫେରନ୍ତୁ "exofs";
    ସ୍ଥିତି S_MAGIC_EXT: /* 0x137D local */
      ଫେରନ୍ତୁ "ext";
    ସ୍ଥିତି S_MAGIC_EXT2: /* 0xEF53 local */
      ଫେରନ୍ତୁ "ext2/ext3";
    ସ୍ଥିତି S_MAGIC_EXT2_OLD: /* 0xEF51 local */
      ଫେରନ୍ତୁ "ext2";
    ସ୍ଥିତି S_MAGIC_F2FS: /* 0xF2F52010 local */
      ଫେରନ୍ତୁ "f2fs";
    ସ୍ଥିତି S_MAGIC_FAIL_FS: /* 0x4641494C local */
      ଫେରନ୍ତୁ "failfs";
    ସ୍ଥିତି S_MAGIC_FAT: /* 0x4006 local */
      ଫେରନ୍ତୁ "fat";
    ସ୍ଥିତି S_MAGIC_FHGFS: /* 0x19830326 remote */
      ଫେରନ୍ତୁ "fhgfs";
    ସ୍ଥିତି S_MAGIC_FUSE: /* 0x65735546 remote */
      ଫେରନ୍ତୁ "fuse";
    ସ୍ଥିତି S_MAGIC_FUSECTL: /* 0x65735543 remote */
      ଫେରନ୍ତୁ "fusectl";
    ସ୍ଥିତି S_MAGIC_FUTEXFS: /* 0x0BAD1DEA local */
      ଫେରନ୍ତୁ "futexfs";
    ସ୍ଥିତି S_MAGIC_GFS: /* 0x01161970 remote */
      ଫେରନ୍ତୁ "gfs/gfs2";
    ସ୍ଥିତି S_MAGIC_GPFS: /* 0x47504653 remote */
      ଫେରନ୍ତୁ "gpfs";
    ସ୍ଥିତି S_MAGIC_GUEST_MEMFD: /* 0x474D454D remote */
      ଫେରନ୍ତୁ "guest-memfd";
    ସ୍ଥିତି S_MAGIC_HFS: /* 0x4244 local */
      ଫେରନ୍ତୁ "hfs";
    ସ୍ଥିତି S_MAGIC_HFS_PLUS: /* 0x482B local */
      ଫେରନ୍ତୁ "hfs+";
    ସ୍ଥିତି S_MAGIC_HFS_X: /* 0x4858 local */
      ଫେରନ୍ତୁ "hfsx";
    ସ୍ଥିତି S_MAGIC_HOSTFS: /* 0x00C0FFEE local */
      ଫେରନ୍ତୁ "hostfs";
    ସ୍ଥିତି S_MAGIC_HPFS: /* 0xF995E849 local */
      ଫେରନ୍ତୁ "hpfs";
    ସ୍ଥିତି S_MAGIC_HUGETLBFS: /* 0x958458F6 local */
      ଫେରନ୍ତୁ "hugetlbfs";
    ସ୍ଥିତି S_MAGIC_MTD_INODE_FS: /* 0x11307854 local */
      ଫେରନ୍ତୁ "inodefs";
    ସ୍ଥିତି S_MAGIC_IBRIX: /* 0x013111A8 remote */
      ଫେରନ୍ତୁ "ibrix";
    ସ୍ଥିତି S_MAGIC_INOTIFYFS: /* 0x2BAD1DEA local */
      ଫେରନ୍ତୁ "inotifyfs";
    ସ୍ଥିତି S_MAGIC_ISOFS: /* 0x9660 local */
      ଫେରନ୍ତୁ "isofs";
    ସ୍ଥିତି S_MAGIC_ISOFS_R_WIN: /* 0x4004 local */
      ଫେରନ୍ତୁ "isofs";
    ସ୍ଥିତି S_MAGIC_ISOFS_WIN: /* 0x4000 local */
      ଫେରନ୍ତୁ "isofs";
    ସ୍ଥିତି S_MAGIC_JFFS: /* 0x07C0 local */
      ଫେରନ୍ତୁ "jffs";
    ସ୍ଥିତି S_MAGIC_JFFS2: /* 0x72B6 local */
      ଫେରନ୍ତୁ "jffs2";
    ସ୍ଥିତି S_MAGIC_JFS: /* 0x3153464A local */
      ଫେରନ୍ତୁ "jfs";
    ସ୍ଥିତି S_MAGIC_KAFS: /* 0x6B414653 remote */
      ଫେରନ୍ତୁ "k-afs";
    ସ୍ଥିତି S_MAGIC_LOGFS: /* 0xC97E8168 local */
      ଫେରନ୍ତୁ "logfs";
    ସ୍ଥିତି S_MAGIC_LUSTRE: /* 0x0BD00BD0 remote */
      ଫେରନ୍ତୁ "lustre";
    ସ୍ଥିତି S_MAGIC_M1FS: /* 0x5346314D local */
      ଫେରନ୍ତୁ "m1fs";
    ସ୍ଥିତି S_MAGIC_MINIX: /* 0x137F local */
      ଫେରନ୍ତୁ "minix";
    ସ୍ଥିତି S_MAGIC_MINIX_30: /* 0x138F local */
      ଫେରନ୍ତୁ "minix (30 char.)";
    ସ୍ଥିତି S_MAGIC_MINIX_V2: /* 0x2468 local */
      ଫେରନ୍ତୁ "minix v2";
    ସ୍ଥିତି S_MAGIC_MINIX_V2_30: /* 0x2478 local */
      ଫେରନ୍ତୁ "minix v2 (30 char.)";
    ସ୍ଥିତି S_MAGIC_MINIX_V3: /* 0x4D5A local */
      ଫେରନ୍ତୁ "minix3";
    ସ୍ଥିତି S_MAGIC_MQUEUE: /* 0x19800202 local */
      ଫେରନ୍ତୁ "mqueue";
    ସ୍ଥିତି S_MAGIC_MSDOS: /* 0x4D44 local */
      ଫେରନ୍ତୁ "msdos";
    ସ୍ଥିତି S_MAGIC_NCP: /* 0x564C remote */
      ଫେରନ୍ତୁ "novell";
    ସ୍ଥିତି S_MAGIC_NFS: /* 0x6969 remote */
      ଫେରନ୍ତୁ "nfs";
    ସ୍ଥିତି S_MAGIC_NFSD: /* 0x6E667364 remote */
      ଫେରନ୍ତୁ "nfsd";
    ସ୍ଥିତି S_MAGIC_NILFS: /* 0x3434 local */
      ଫେରନ୍ତୁ "nilfs";
    ସ୍ଥିତି S_MAGIC_NSFS: /* 0x6E736673 local */
      ଫେରନ୍ତୁ "nsfs";
    ସ୍ଥିତି S_MAGIC_NTFS: /* 0x5346544E local */
      ଫେରନ୍ତୁ "ntfs";
    ସ୍ଥିତି S_MAGIC_NULL_FS: /* 0x4E554C4C local */
      ଫେରନ୍ତୁ "nullfs";
    ସ୍ଥିତି S_MAGIC_OPENPROM: /* 0x9FA1 local */
      ଫେରନ୍ତୁ "openprom";
    ସ୍ଥିତି S_MAGIC_OCFS2: /* 0x7461636F remote */
      ଫେରନ୍ତୁ "ocfs2";
    ସ୍ଥିତି S_MAGIC_OVERLAYFS: /* 0x794C7630 remote */
      /* This may overlay remote file systems.
         Also there have been issues reported with inotify and overlayfs,
         so mark as "remote" so that polling is used.  */
      ଫେରନ୍ତୁ "overlayfs";
    ସ୍ଥିତି S_MAGIC_PANFS: /* 0xAAD7AAEA remote */
      ଫେରନ୍ତୁ "panfs";
    ସ୍ଥିତି S_MAGIC_PID_FS: /* 0x50494446 local */
      ଫେରନ୍ତୁ "pidfs";
    ସ୍ଥିତି S_MAGIC_PIPEFS: /* 0x50495045 remote */
      /* FIXME: change syntax or add an optional attribute like "inotify:no".
         pipefs is labeled as "remote" so that tail always polls,
         but it isn't really remote file system type.  */
      ଫେରନ୍ତୁ "pipefs";
    ସ୍ଥିତି S_MAGIC_PPC_CMM: /* 0xC7571590 local */
      ଫେରନ୍ତୁ "ppc-cmm-fs";
    ସ୍ଥିତି S_MAGIC_PRL_FS: /* 0x7C7C6673 remote */
      ଫେରନ୍ତୁ "prl_fs";
    ସ୍ଥିତି S_MAGIC_PROC: /* 0x9FA0 local */
      ଫେରନ୍ତୁ "proc";
    ସ୍ଥିତି S_MAGIC_PSTOREFS: /* 0x6165676C local */
      ଫେରନ୍ତୁ "pstorefs";
    ସ୍ଥିତି S_MAGIC_QNX4: /* 0x002F local */
      ଫେରନ୍ତୁ "qnx4";
    ସ୍ଥିତି S_MAGIC_QNX6: /* 0x68191122 local */
      ଫେରନ୍ତୁ "qnx6";
    ସ୍ଥିତି S_MAGIC_RAMFS: /* 0x858458F6 local */
      ଫେରନ୍ତୁ "ramfs";
    ସ୍ଥିତି S_MAGIC_RDTGROUP: /* 0x07655821 local */
      ଫେରନ୍ତୁ "rdt";
    ସ୍ଥିତି S_MAGIC_REISERFS: /* 0x52654973 local */
      ଫେରନ୍ତୁ "reiserfs";
    ସ୍ଥିତି S_MAGIC_ROMFS: /* 0x7275 local */
      ଫେରନ୍ତୁ "romfs";
    ସ୍ଥିତି S_MAGIC_RPC_PIPEFS: /* 0x67596969 local */
      ଫେରନ୍ତୁ "rpc_pipefs";
    ସ୍ଥିତି S_MAGIC_SDCARDFS: /* 0x5DCA2DF5 local */
      ଫେରନ୍ତୁ "sdcardfs";
    ସ୍ଥିତି S_MAGIC_SECRETMEM: /* 0x5345434D local */
      ଫେରନ୍ତୁ "secretmem";
    ସ୍ଥିତି S_MAGIC_SECURITYFS: /* 0x73636673 local */
      ଫେରନ୍ତୁ "securityfs";
    ସ୍ଥିତି S_MAGIC_SELINUX: /* 0xF97CFF8C local */
      ଫେରନ୍ତୁ "selinux";
    ସ୍ଥିତି S_MAGIC_SMACK: /* 0x43415D53 local */
      ଫେରନ୍ତୁ "smackfs";
    ସ୍ଥିତି S_MAGIC_SMB: /* 0x517B remote */
      ଫେରନ୍ତୁ "smb";
    ସ୍ଥିତି S_MAGIC_SMB2: /* 0xFE534D42 remote */
      ଫେରନ୍ତୁ "smb2";
    ସ୍ଥିତି S_MAGIC_SNFS: /* 0xBEEFDEAD remote */
      ଫେରନ୍ତୁ "snfs";
    ସ୍ଥିତି S_MAGIC_SOCKFS: /* 0x534F434B local */
      ଫେରନ୍ତୁ "sockfs";
    ସ୍ଥିତି S_MAGIC_SQUASHFS: /* 0x73717368 local */
      ଫେରନ୍ତୁ "squashfs";
    ସ୍ଥିତି S_MAGIC_SYSFS: /* 0x62656572 local */
      ଫେରନ୍ତୁ "sysfs";
    ସ୍ଥିତି S_MAGIC_SYSV2: /* 0x012FF7B6 local */
      ଫେରନ୍ତୁ "sysv2";
    ସ୍ଥିତି S_MAGIC_SYSV4: /* 0x012FF7B5 local */
      ଫେରନ୍ତୁ "sysv4";
    ସ୍ଥିତି S_MAGIC_TMPFS: /* 0x01021994 local */
      ଫେରନ୍ତୁ "tmpfs";
    ସ୍ଥିତି S_MAGIC_TRACEFS: /* 0x74726163 local */
      ଫେରନ୍ତୁ "tracefs";
    ସ୍ଥିତି S_MAGIC_UBIFS: /* 0x24051905 local */
      ଫେରନ୍ତୁ "ubifs";
    ସ୍ଥିତି S_MAGIC_UDF: /* 0x15013346 local */
      ଫେରନ୍ତୁ "udf";
    ସ୍ଥିତି S_MAGIC_UFS: /* 0x00011954 local */
      ଫେରନ୍ତୁ "ufs";
    ସ୍ଥିତି S_MAGIC_UFS_BYTESWAPPED: /* 0x54190100 local */
      ଫେରନ୍ତୁ "ufs";
    ସ୍ଥିତି S_MAGIC_USBDEVFS: /* 0x9FA2 local */
      ଫେରନ୍ତୁ "usbdevfs";
    ସ୍ଥିତି S_MAGIC_V9FS: /* 0x01021997 local */
      ଫେରନ୍ତୁ "v9fs";
    ସ୍ଥିତି S_MAGIC_VBOXSF: /* 0x786F4256 remote */
      ଫେରନ୍ତୁ "vboxsf";
    ସ୍ଥିତି S_MAGIC_VMHGFS: /* 0xBACBACBC remote */
      ଫେରନ୍ତୁ "vmhgfs";
    ସ୍ଥିତି S_MAGIC_VXFS: /* 0xA501FCF5 remote */
      /* Veritas File System can run in single instance or clustered mode,
         so mark as remote to cater for the latter case.  */
      ଫେରନ୍ତୁ "vxfs";
    ସ୍ଥିତି S_MAGIC_VZFS: /* 0x565A4653 local */
      ଫେରନ୍ତୁ "vzfs";
    ସ୍ଥିତି S_MAGIC_WSLFS: /* 0x53464846 local */
      ଫେରନ୍ତୁ "wslfs";
    ସ୍ଥିତି S_MAGIC_XENFS: /* 0xABBA1974 local */
      ଫେରନ୍ତୁ "xenfs";
    ସ୍ଥିତି S_MAGIC_XENIX: /* 0x012FF7B4 local */
      ଫେରନ୍ତୁ "xenix";
    ସ୍ଥିତି S_MAGIC_XFS: /* 0x58465342 local */
      ଫେରନ୍ତୁ "xfs";
    ସ୍ଥିତି S_MAGIC_XIAFS: /* 0x012FD16D local */
      ଫେରନ୍ତୁ "xia";
    ସ୍ଥିତି S_MAGIC_Z3FOLD: /* 0x0033 local */
      ଫେରନ୍ତୁ "z3fold";
    ସ୍ଥିତି S_MAGIC_ZFS: /* 0x2FC12FC1 local */
      ଫେରନ୍ତୁ "zfs";
    ସ୍ଥିତି S_MAGIC_ZONEFS: /* 0x5A4F4653 local */
      ଫେରନ୍ତୁ "zonefs";
    ସ୍ଥିତି S_MAGIC_ZSMALLOC: /* 0x58295829 local */
      ଫେରନ୍ତୁ "zsmallocfs";


# elif __GNU__
    ସ୍ଥିତି FSTYPE_UFS:
      ଫେରନ୍ତୁ "ufs";
    ସ୍ଥିତି FSTYPE_NFS:
      ଫେରନ୍ତୁ "nfs";
    ସ୍ଥିତି FSTYPE_GFS:
      ଫେରନ୍ତୁ "gfs";
    ସ୍ଥିତି FSTYPE_LFS:
      ଫେରନ୍ତୁ "lfs";
    ସ୍ଥିତି FSTYPE_SYSV:
      ଫେରନ୍ତୁ "sysv";
    ସ୍ଥିତି FSTYPE_FTP:
      ଫେରନ୍ତୁ "ftp";
    ସ୍ଥିତି FSTYPE_TAR:
      ଫେରନ୍ତୁ "tar";
    ସ୍ଥିତି FSTYPE_AR:
      ଫେରନ୍ତୁ "ar";
    ସ୍ଥିତି FSTYPE_CPIO:
      ଫେରନ୍ତୁ "cpio";
    ସ୍ଥିତି FSTYPE_MSLOSS:
      ଫେରନ୍ତୁ "msloss";
    ସ୍ଥିତି FSTYPE_CPM:
      ଫେରନ୍ତୁ "cpm";
    ସ୍ଥିତି FSTYPE_HFS:
      ଫେରନ୍ତୁ "hfs";
    ସ୍ଥିତି FSTYPE_DTFS:
      ଫେରନ୍ତୁ "dtfs";
    ସ୍ଥିତି FSTYPE_GRFS:
      ଫେରନ୍ତୁ "grfs";
    ସ୍ଥିତି FSTYPE_TERM:
      ଫେରନ୍ତୁ "term";
    ସ୍ଥିତି FSTYPE_DEV:
      ଫେରନ୍ତୁ "dev";
    ସ୍ଥିତି FSTYPE_PROC:
      ଫେରନ୍ତୁ "proc";
    ସ୍ଥିତି FSTYPE_IFSOCK:
      ଫେରନ୍ତୁ "ifsock";
    ସ୍ଥିତି FSTYPE_AFS:
      ଫେରନ୍ତୁ "afs";
    ସ୍ଥିତି FSTYPE_DFS:
      ଫେରନ୍ତୁ "dfs";
    ସ୍ଥିତି FSTYPE_PROC9:
      ଫେରନ୍ତୁ "proc9";
    ସ୍ଥିତି FSTYPE_SOCKET:
      ଫେରନ୍ତୁ "socket";
    ସ୍ଥିତି FSTYPE_MISC:
      ଫେରନ୍ତୁ "misc";
    ସ୍ଥିତି FSTYPE_EXT2FS:
      ଫେରନ୍ତୁ "ext2/ext3";
    ସ୍ଥିତି FSTYPE_HTTP:
      ଫେରନ୍ତୁ "http";
    ସ୍ଥିତି FSTYPE_MEMFS:
      ଫେରନ୍ତୁ "memfs";
    ସ୍ଥିତି FSTYPE_ISO9660:
      ଫେରନ୍ତୁ "iso9660";
# endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      {
        unsigned long ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = statfsbuf->f_type;
        ସ୍ଥିର ଅକ୍ଷର buf[ଆକାର "UNKNOWN (0x%lx)" - 3
                        + (ଆକାର type * CHAR_BIT + 3) / 4];
        sprintf (buf, "UNKNOWN (0x%lx)", type);
        ଫେରନ୍ତୁ buf;
      }
    }
#endif
}

NODISCARD
ସ୍ଥିର ଅକ୍ଷର *
human_access (ସଂରଚନା stat ଅଚଳ *statbuf)
{
  ସ୍ଥିର ଅକ୍ଷର modebuf[12];
  filemodestring (statbuf, modebuf);
  modebuf[10] = 0;
  ଫେରନ୍ତୁ modebuf;
}

NODISCARD
ସ୍ଥିର ଅକ୍ଷର *
human_time (ସଂରଚନା timespec t)
{
  /* STR must be at least INT_BUFSIZE_BOUND (intmax_t) big, either
     because localtime_rz fails, or because the time zone is truly
     outlandish so that %z expands to a long string.  */
  ସ୍ଥିର ଅକ୍ଷର str[INT_BUFSIZE_BOUND (intmax_t)
                  + INT_STRLEN_BOUND (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) /* YYYY */
                  + 1 /* because YYYY might equal INT_MAX + 1900 */
                  + ଆକାର "-MM-DD HH:MM:SS.NNNNNNNNN +"];
  ସ୍ଥିର timezone_t tz;
  ଯଦି (!tz)
    tz = tzalloc (getenv ("TZ"));
  ସଂରଚନା tm tm;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ns = t.tv_nsec;
  ଯଦି (localtime_rz (tz, &t.tv_sec, &tm))
    nstrftime (str, ଆକାର str, "%Y-%m-%d %H:%M:%S.%N %z", &tm, tz, ns);
  ଅନ୍ୟଥା
    {
      ଅକ୍ଷର secbuf[INT_BUFSIZE_BOUND (intmax_t)];
      sprintf (str, "%s.%09d", timetostr (t.tv_sec, secbuf), ns);
    }
  ଫେରନ୍ତୁ str;
}

/* PFORMAT points to a '%' followed by a prefix of a format, all of
   size PREFIX_LEN.  The flags allowed for this format are
   ALLOWED_FLAGS; remove other printf flags from the prefix, then
   append SUFFIX.  */
ସ୍ଥିର ଶୂନ୍ୟ
make_format (ଅକ୍ଷର *pformat, size_t prefix_len, ଅକ୍ଷର ଅଚଳ *allowed_flags,
             ଅକ୍ଷର ଅଚଳ *suffix)
{
  ଅକ୍ଷର *dst = pformat + 1;
  ଅକ୍ଷର ଅଚଳ *src;
  ଅକ୍ଷର ଅଚଳ *srclim = pformat + prefix_len;
  ପାଇଁ (src = dst; src < srclim && strchr (printf_flags, *src); src++)
    ଯଦି (strchr (allowed_flags, *src))
      *dst++ = *src;
  ଯେତେବେଳେ (src < srclim)
    *dst++ = *src++;
  strcpy (dst, suffix);
}

ସ୍ଥିର ଶୂନ୍ୟ
out_string (ଅକ୍ଷର *pformat, size_t prefix_len, ଅକ୍ଷର ଅଚଳ *arg)
{
  make_format (pformat, prefix_len, "-", "s");
  printf (pformat, arg);
}
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
out_int (ଅକ୍ଷର *pformat, size_t prefix_len, intmax_t arg)
{
  make_format (pformat, prefix_len, "'-+ 0", "jd");
  ଫେରନ୍ତୁ printf (pformat, arg);
}
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
out_uint (ଅକ୍ଷର *pformat, size_t prefix_len, uintmax_t arg)
{
  make_format (pformat, prefix_len, "'-0", "ju");
  ଫେରନ୍ତୁ printf (pformat, arg);
}
ସ୍ଥିର ଶୂନ୍ୟ
out_uint_o (ଅକ୍ଷର *pformat, size_t prefix_len, uintmax_t arg)
{
  make_format (pformat, prefix_len, "-#0", "jo");
  printf (pformat, arg);
}
ସ୍ଥିର ଶୂନ୍ୟ
out_uint_x (ଅକ୍ଷର *pformat, size_t prefix_len, uintmax_t arg)
{
  make_format (pformat, prefix_len, "-#0", "jx");
  printf (pformat, arg);
}
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
out_minus_zero (ଅକ୍ଷର *pformat, size_t prefix_len)
{
  make_format (pformat, prefix_len, "'-+ 0", ".0f");
  ଫେରନ୍ତୁ printf (pformat, -0.25);
}

/* Output the number of seconds since the Epoch, using a format that
   acts like printf's %f format.  */
ସ୍ଥିର ଶୂନ୍ୟ
out_epoch_sec (ଅକ୍ଷର *pformat, size_t prefix_len,
               ସଂରଚନା timespec arg)
{
  ଅକ୍ଷର *dot = memchr (pformat, '.', prefix_len);
  size_t sec_prefix_len = prefix_len;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା width = 0;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା precision = 0;
  bool frac_ବାମ_adjust = false;

  ଯଦି (dot)
    {
      sec_prefix_len = dot - pformat;
      pformat[prefix_len] = '\0';

      ଯଦି (c_isdigit (dot[1]))
        {
          long ପୂର୍ଣ୍ଣସଂଖ୍ୟା lprec = strtol (dot + 1, ଖାଲି, 10);
          precision = (lprec <= INT_MAX ? lprec : INT_MAX);
        }
      ଅନ୍ୟଥା
        {
          precision = 9;
        }

      ଯଦି (precision && c_isdigit (dot[-1]))
        {
          /* If a nontrivial width is given, subtract the width of the
             decimal point and PRECISION digits that will be output
             later.  */
          ଅକ୍ଷର *p = dot;
          *dot = '\0';

          କର
            --p;
          ଯେତେବେଳେ (c_isdigit (p[-1]));

          long ପୂର୍ଣ୍ଣସଂଖ୍ୟା lwidth = strtol (p, ଖାଲି, 10);
          width = (lwidth <= INT_MAX ? lwidth : INT_MAX);
          ଯଦି (1 < width)
            {
              p += (*p == '0');
              sec_prefix_len = p - pformat;
              ପୂର୍ଣ୍ଣସଂଖ୍ୟା w_d = (decimal_point_len < width
                         ? width - decimal_point_len
                         : 0);
              ଯଦି (1 < w_d)
                {
                  ପୂର୍ଣ୍ଣସଂଖ୍ୟା w = w_d - precision;
                  ଯଦି (1 < w)
                    {
                      ଅକ୍ଷର *dst = pformat;
                      ପାଇଁ (ଅକ୍ଷର ଅଚଳ *src = dst; src < p; src++)
                        {
                          ଯଦି (*src == '-')
                            frac_ବାମ_adjust = true;
                          ଅନ୍ୟଥା
                            *dst++ = *src;
                        }
                      sec_prefix_len =
                        (dst - pformat
                         + (frac_ବାମ_adjust ? 0 : sprintf (dst, "%d", w)));
                    }
                }
            }
        }
    }

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଭାଜକ = 1;
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = precision; i < 9; i++)
    ଭାଜକ *= 10;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା frac_sec = arg.tv_nsec / ଭାଜକ;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା int_len;

  ଯଦି (TYPE_SIGNED (time_t))
    {
      bool minus_zero = false;
      ଯଦି (arg.tv_sec < 0 && arg.tv_nsec != 0)
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା frac_sec_modulus = 1000000000 / ଭାଜକ;
          frac_sec = (frac_sec_modulus - frac_sec
                      - (arg.tv_nsec % ଭାଜକ != 0));
          arg.tv_sec += (frac_sec != 0);
          minus_zero = (arg.tv_sec == 0);
        }
      int_len = (minus_zero
                 ? out_minus_zero (pformat, sec_prefix_len)
                 : out_int (pformat, sec_prefix_len, arg.tv_sec));
    }
  ଅନ୍ୟଥା
    int_len = out_uint (pformat, sec_prefix_len, arg.tv_sec);

  ଯଦି (precision)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା prec = (precision < 9 ? precision : 9);
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା trailing_prec = precision - prec;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ilen = (int_len < 0 ? 0 : int_len);
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା trailing_width = (ilen < width && decimal_point_len < width - ilen
                            ? width - ilen - decimal_point_len - prec
                            : 0);
      printf ("%s%.*d%-*.*d", decimal_point, prec, frac_sec,
              trailing_width, trailing_prec, 0);
    }
}

/* Print the context information of FILENAME, and return true iff the
   context could not be obtained.  */
NODISCARD
ସ୍ଥିର bool
out_file_context (ଅକ୍ଷର *pformat, size_t prefix_len, ଅକ୍ଷର ଅଚଳ *filename)
{
  ଅକ୍ଷର *scontext;
  bool fail = false;

  ଯଦି ((follow_links
       ? getfilecon (filename, &scontext)
       : lgetfilecon (filename, &scontext)) < 0)
    {
      error (0, errno, _("failed to get security context of %s"),
             quoteaf (filename));
      scontext = ଖାଲି;
      fail = true;
    }
  strcpy (pformat + prefix_len, "s");
  printf (pformat, (scontext ? scontext : "?"));
  ଯଦି (scontext)
    freecon (scontext);
  ଫେରନ୍ତୁ fail;
}

/* Set the quoting style once it is needed.  */
ସ୍ଥିର ଶୂନ୍ୟ
initialize_quoting_style (ଶୂନ୍ୟ)
{
  ସ୍ଥିର bool initialized;
  ଯଦି (!initialized)
    {
      initialized = true;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା qs = getenv_quoting_style ();
      set_quoting_style (ଖାଲି, 0 <= qs ? qs : shell_escape_quoting_style);
    }
}

/* Print statfs info.  Return zero upon success, nonzero upon failure.  */
NODISCARD
ସ୍ଥିର bool
print_statfs (ଅକ୍ଷର *pformat, size_t prefix_len, MAYBE_UNUSED ଅକ୍ଷର mod, ଅକ୍ଷର m,
              MAYBE_UNUSED ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅକ୍ଷର ଅଚଳ *filename,
              ଶୂନ୍ୟ ଅଚଳ *data)
{
  STRUCT_STATVFS ଅଚଳ *statfsbuf = data;
  bool fail = false;

  ବାଛ (m)
    {
    ସ୍ଥିତି 'n':
      ଯଦି (mod == 'Q')
        {
          initialize_quoting_style ();
          filename = quoteN (filename);
        }
      out_string (pformat, prefix_len, filename);
      ଭାଙ୍ଗ;

    ସ୍ଥିତି 'i':
      {
#ଯଦି STRUCT_STATXFS_F_FSID_IS_INTEGER
        uintmax_t fsid = statfsbuf->f_fsid;
#ଅନ୍ୟଥା
        ପ୍ରକାରବ୍ୟାଖ୍ୟା unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା fsid_word;
        static_assert (alignof (STRUCT_STATVFS) % alignof (fsid_word) == 0);
        static_assert (offsetof (STRUCT_STATVFS, f_fsid) % alignof (fsid_word)
                       == 0);
        static_assert (ଆକାର statfsbuf->f_fsid % alignof (fsid_word) == 0);
        fsid_word ଅଚଳ *p = (fsid_word *) &statfsbuf->f_fsid;

        /* Assume a little-endian word order, as that is compatible
           with glibc's statvfs implementation.  */
        uintmax_t fsid = 0;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଶବ୍ଦଗୁଡ଼ିକ = ଆକାର statfsbuf->f_fsid / ଆକାର *p;
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଶବ୍ଦଗୁଡ଼ିକ && i * ଆକାର *p < ଆକାର fsid; i++)
          {
            uintmax_t u = p[ଶବ୍ଦଗୁଡ଼ିକ - 1 - i];
            fsid |= u << (i * CHAR_BIT * ଆକାର *p);
          }
#endif
        out_uint_x (pformat, prefix_len, fsid);
      }
      ଭାଙ୍ଗ;

    ସ୍ଥିତି 'l':
      OUT_NAMEMAX (pformat, prefix_len, SB_F_NAMEMAX (statfsbuf));
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 't':
#ଯଦି HAVE_STRUCT_STATXFS_F_TYPE
      out_uint_x (pformat, prefix_len, statfsbuf->f_type);
#ଅନ୍ୟଥା
      fputc ('?', stdout);
#endif
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'T':
      out_string (pformat, prefix_len, human_fstype (statfsbuf));
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'b':
      out_int (pformat, prefix_len, statfsbuf->f_blocks);
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'f':
      out_int (pformat, prefix_len, statfsbuf->f_bfree);
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'a':
      out_int (pformat, prefix_len, statfsbuf->f_bavail);
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 's':
      out_uint (pformat, prefix_len, statfsbuf->f_bsize);
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'S':
      {
        uintmax_t frsize = STATFS_FRSIZE (statfsbuf);
        ଯଦି (! frsize)
          frsize = statfsbuf->f_bsize;
        out_uint (pformat, prefix_len, frsize);
      }
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'c':
      out_uint (pformat, prefix_len, statfsbuf->f_files);
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'd':
      out_int (pformat, prefix_len, statfsbuf->f_ffree);
      ଭାଙ୍ଗ;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      fputc ('?', stdout);
      ଭାଙ୍ଗ;
    }
  ଫେରନ୍ତୁ fail;
}

/* Return any bind mounted source for a path.
   The caller should not free the returned buffer.
   Return NULL if no bind mount found.  */
NODISCARD
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *
find_bind_mount (ଅକ୍ଷର ଅଚଳ * name)
{
  ଅକ୍ଷର ଅଚଳ * bind_mount = ଖାଲି;

  ସ୍ଥିର ସଂରଚନା mount_entry *mount_list;
  ସ୍ଥିର bool tried_mount_list = false;
  ଯଦି (!tried_mount_list) /* attempt/warn once per process.  */
    {
      ଯଦି (!(mount_list = read_file_system_list (false)))
        error (0, errno, "%s", _("cannot read table of mounted file systems"));
      tried_mount_list = true;
    }

  ସଂରଚନା stat name_stats;
  ଯଦି (stat (name, &name_stats) != 0)
    ଫେରନ୍ତୁ ଖାଲି;

  ପାଇଁ (ସଂରଚନା mount_entry *me = mount_list; me; me = me->me_next)
    {
      ଯଦି (me->me_dummy && me->me_devname[0] == '/'
          && streq (me->me_mountdir, name))
        {
          ସଂରଚନା stat dev_stats;

          ଯଦି (stat (me->me_devname, &dev_stats) == 0
              && psame_inode (&name_stats, &dev_stats))
            {
              bind_mount = me->me_devname;
              ଭାଙ୍ଗ;
            }
        }
    }

  ଫେରନ୍ତୁ bind_mount;
}

/* Print mount point.  Return zero upon success, nonzero upon failure.  */
NODISCARD
ସ୍ଥିର bool
out_mount_point (ଅକ୍ଷର ଅଚଳ *filename, ଅକ୍ଷର *pformat, size_t prefix_len,
                 ଅଚଳ ସଂରଚନା stat *statp)
{

  ଅକ୍ଷର ଅଚଳ *np = "?", *bp = ଖାଲି;
  ଅକ୍ଷର *mp = ଖାଲି;
  bool fail = true;

  /* Look for bind mounts first.  Note we output the immediate alias,
     rather than further resolving to a base device mount point.  */
  ଯଦି (follow_links || !S_ISLNK (statp->st_mode))
    {
      ଅକ୍ଷର *resolved = canonicalize_file_name (filename);
      ଯଦି (!resolved)
        {
          error (0, errno, _("failed to canonicalize %s"), quoteaf (filename));
          ଯାଅ print_mount_point;
        }
      bp = find_bind_mount (resolved);
      ମୁକ୍ତ (resolved);
      ଯଦି (bp)
        {
          fail = false;
          ଯାଅ print_mount_point;
        }
    }

  /* If there is no direct bind mount, then navigate
     back up the tree looking for a device change.
     Note we don't detect if any of the directory components
     are bind mounted to the same device, but that's OK
     since we've not directly queried them.  */
  ଯଦି ((mp = find_mount_point (filename, statp)))
    {
      /* This dir might be bind mounted to another device,
         so we resolve the bound source in that case also.  */
      bp = find_bind_mount (mp);
      fail = false;
    }

print_mount_point:

  out_string (pformat, prefix_len, bp ? bp : mp ? mp : np);
  ମୁକ୍ତ (mp);
  ଫେରନ୍ତୁ fail;
}

/* Map a TS with negative TS.tv_nsec to {0,0}.  */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା timespec
neg_to_zero (ସଂରଚନା timespec ts)
{
  ଯଦି (0 <= ts.tv_nsec)
    ଫେରନ୍ତୁ ts;
  ସଂରଚନା timespec z = {0};
  ଫେରନ୍ତୁ z;
}

/* Output a single-character \ escape.  */

ସ୍ଥିର ଶୂନ୍ୟ
print_esc_char (ଅକ୍ଷର c)
{
  ବାଛ (c)
    {
    ସ୍ଥିତି 'a':			/* Alert. */
      c ='\a';
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'b':			/* Backspace. */
      c ='\b';
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'e':			/* Escape. */
      c ='\x1B';
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'f':			/* Form feed. */
      c ='\f';
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'n':			/* New line. */
      c ='\n';
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'r':			/* Carriage return. */
      c ='\r';
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 't':			/* Horizontal tab. */
      c ='\t';
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'v':			/* Vertical tab. */
      c ='\v';
      ଭାଙ୍ଗ;
    ସ୍ଥିତି '"':
    ସ୍ଥିତି '\\':
      ଭାଙ୍ଗ;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      error (0, 0, _("warning: unrecognized escape '\\%c'"), c);
      ଭାଙ୍ଗ;
    }
  putchar (c);
}

ATTRIBUTE_PURE
ସ୍ଥିର size_t
format_code_offset (ଅକ୍ଷର ଅଚଳ *directive)
{
  size_t len = strspn (directive + 1, printf_flags);
  ଅକ୍ଷର ଅଚଳ *fmt_char = directive + len + 1;
  fmt_char += strspn (fmt_char, digits);
  ଯଦି (*fmt_char == '.')
    fmt_char += 1 + strspn (fmt_char + 1, digits);
  ଫେରନ୍ତୁ fmt_char - directive;
}

/* Print the information specified by the format string, FORMAT,
   calling PRINT_FUNC for each %-directive encountered.
   Return zero upon success, nonzero upon failure.  */
NODISCARD
ସ୍ଥିର bool
print_it (ଅକ୍ଷର ଅଚଳ *format, ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅକ୍ଷର ଅଚଳ *filename,
          bool (*print_func) (ଅକ୍ଷର *, size_t, ଅକ୍ଷର, ଅକ୍ଷର,
                              ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ଅକ୍ଷର ଅଚଳ *, ଶୂନ୍ୟ ଅଚଳ *),
          ଶୂନ୍ୟ ଅଚଳ *data)
{
  bool fail = false;

  /* Add 2 to accommodate our conversion of the stat '%s' format string
     to the longer printf '%llu' one.  */
  ଗଣନା
    {
      MAX_ADDITIONAL_BYTES =
        (MAX (ଆକାର "jd",
              MAX (ଆକାର "jo", MAX (ଆକାର "ju", ଆକାର "jx")))
         - 1)
    };
  size_t n_alloc = strlen (format) + MAX_ADDITIONAL_BYTES + 1;
  ଅକ୍ଷର *dest = xmalloc (n_alloc);
  ପାଇଁ (ଅକ୍ଷର ଅଚଳ *b = format; *b; b++)
    {
      ବାଛ (*b)
        {
        ସ୍ଥିତି '%':
          {
            size_t len = format_code_offset (b);
            ଅକ୍ଷର fmt_char = *(b + len);
            ଅକ୍ଷର mod_char = 0;
            memcpy (dest, b, len);
            b += len;

            ବାଛ (fmt_char)
              {
              ସ୍ଥିତି '\0':
                --b;
                FALLTHROUGH;
              ସ୍ଥିତି '%':
                ଯଦି (1 < len)
                  {
                    dest[len] = fmt_char;
                    dest[len + 1] = '\0';
                    error (EXIT_FAILURE, 0, _("%s: invalid directive"),
                           quote (dest));
                  }
                putchar ('%');
                ଭାଙ୍ଗ;
              ସ୍ଥିତି 'H':
              ସ୍ଥିତି 'L':
              ସ୍ଥିତି 'Q':
                mod_char = fmt_char;
                fmt_char = *(b + 1);
                ଯଦି ((mod_char == 'Q' && fmt_char == 'n')
                    || (print_func == print_stat
                        && (mod_char == 'H' || mod_char == 'L')
                        && (fmt_char == 'd' || fmt_char == 'r')))
                  {
                    b++;
                  }
                ଅନ୍ୟଥା
                  {
                    fmt_char = mod_char;
                    mod_char = 0;
                  }
                FALLTHROUGH;
              ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
                fail |= print_func (dest, len, mod_char, fmt_char,
                                    fd, filename, data);
                ଭାଙ୍ଗ;
              }
            ଭାଙ୍ଗ;
          }

        ସ୍ଥିତି '\\':
          ଯଦି ( ! interpret_backslash_escapes)
            {
              putchar ('\\');
              ଭାଙ୍ଗ;
            }
          ++b;
          ଯଦି (isoct (*b))
            {
              ପୂର୍ଣ୍ଣସଂଖ୍ୟା esc_ମୂଲ୍ୟ = fromoct (*b);
              ପୂର୍ଣ୍ଣସଂଖ୍ୟା esc_length = 1;	/* number of octal digits */
              ପାଇଁ (++b; esc_length < 3 && isoct (*b);
                   ++esc_length, ++b)
                {
                  esc_ମୂଲ୍ୟ = esc_ମୂଲ୍ୟ * 8 + fromoct (*b);
                }
              putchar (esc_ମୂଲ୍ୟ);
              --b;
            }
          ଅନ୍ୟଥା ଯଦି (*b == 'x' && c_isxdigit (b[1]))
            {
              ପୂର୍ଣ୍ଣସଂଖ୍ୟା esc_ମୂଲ୍ୟ = fromhex (b[1]);	/* Value of \xhh escape. */
              /* A hexadecimal \xhh escape sequence must have
                 1 or 2 hex. digits.  */
              ++b;
              ଯଦି (c_isxdigit (b[1]))
                {
                  ++b;
                  esc_ମୂଲ୍ୟ = esc_ମୂଲ୍ୟ * 16 + fromhex (*b);
                }
              putchar (esc_ମୂଲ୍ୟ);
            }
          ଅନ୍ୟଥା ଯଦି (*b == '\0')
            {
              error (0, 0, _("warning: backslash at end of format"));
              putchar ('\\');
              /* Arrange to exit the loop.  */
              --b;
            }
          ଅନ୍ୟଥା
            {
              print_esc_char (*b);
            }
          ଭାଙ୍ଗ;

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          putchar (*b);
          ଭାଙ୍ଗ;
        }
    }
  ମୁକ୍ତ (dest);

  fputs (trailing_delim, stdout);

  ଫେରନ୍ତୁ fail;
}

/* Stat the file system and print what we find.  */
NODISCARD
ସ୍ଥିର bool
do_statfs (ଅକ୍ଷର ଅଚଳ *filename, ଅକ୍ଷର ଅଚଳ *format)
{
  STRUCT_STATVFS statfsbuf;

  ଯଦି (streq (filename, "-"))
    {
      error (0, 0, _("using %s to denote standard input does not work"
                     " in file system mode"), quoteaf (filename));
      ଫେରନ୍ତୁ false;
    }

  ଯଦି (STATFS (filename, &statfsbuf) != 0)
    {
      error (0, errno, _("cannot read file system information for %s"),
             quoteaf (filename));
      ଫେରନ୍ତୁ false;
    }

  bool fail = print_it (format, -1, filename, print_statfs, &statfsbuf);
  ଫେରନ୍ତୁ ! fail;
}

ସଂରଚନା print_args {
  ସଂରଚନା stat *st;
  ସଂରଚନା timespec btime;
};

/* Ask statx to avoid syncing? */
ସ୍ଥିର bool dont_sync;

/* Ask statx to force sync? */
ସ୍ଥିର bool force_sync;

#ଯଦି USE_STATX
ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା
fmt_to_mask (ଅକ୍ଷର fmt)
{
  ବାଛ (fmt)
    {
    ସ୍ଥିତି 'N':
      ଫେରନ୍ତୁ STATX_MODE;
    ସ୍ଥିତି 'd':
    ସ୍ଥିତି 'D':
      ଫେରନ୍ତୁ STATX_MODE;
    ସ୍ଥିତି 'i':
      ଫେରନ୍ତୁ STATX_INO;
    ସ୍ଥିତି 'a':
    ସ୍ଥିତି 'A':
      ଫେରନ୍ତୁ STATX_MODE;
    ସ୍ଥିତି 'f':
      ଫେରନ୍ତୁ STATX_MODE|STATX_TYPE;
    ସ୍ଥିତି 'F':
      ଫେରନ୍ତୁ STATX_TYPE;
    ସ୍ଥିତି 'h':
      ଫେରନ୍ତୁ STATX_NLINK;
    ସ୍ଥିତି 'u':
    ସ୍ଥିତି 'U':
      ଫେରନ୍ତୁ STATX_UID;
    ସ୍ଥିତି 'g':
    ସ୍ଥିତି 'G':
      ଫେରନ୍ତୁ STATX_GID;
    ସ୍ଥିତି 'm':
      ଫେରନ୍ତୁ STATX_MODE|STATX_INO;
    ସ୍ଥିତି 's':
      ଫେରନ୍ତୁ STATX_SIZE;
    ସ୍ଥିତି 't':
    ସ୍ଥିତି 'T':
      ଫେରନ୍ତୁ STATX_MODE;
    ସ୍ଥିତି 'b':
      ଫେରନ୍ତୁ STATX_BLOCKS;
    ସ୍ଥିତି 'w':
    ସ୍ଥିତି 'W':
      ଫେରନ୍ତୁ STATX_BTIME;
    ସ୍ଥିତି 'x':
    ସ୍ଥିତି 'X':
      ଫେରନ୍ତୁ STATX_ATIME;
    ସ୍ଥିତି 'y':
    ସ୍ଥିତି 'Y':
      ଫେରନ୍ତୁ STATX_MTIME;
    ସ୍ଥିତି 'z':
    ସ୍ଥିତି 'Z':
      ଫେରନ୍ତୁ STATX_CTIME;
    }
  ଫେରନ୍ତୁ 0;
}

ATTRIBUTE_PURE
ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା
format_to_mask (ଅକ୍ଷର ଅଚଳ *format)
{
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା mask = 0;

  ପାଇଁ (ଅକ୍ଷର ଅଚଳ *b = format; *b; b++)
    {
      ଯଦି (*b != '%')
        ଜାରି;

      b += format_code_offset (b);
      ଯଦି (*b == '\0')
        ଭାଙ୍ଗ;
      mask |= fmt_to_mask (*b);
    }
  ଫେରନ୍ତୁ mask;
}

/* statx the file and print what we find */
NODISCARD
ସ୍ଥିର bool
do_stat (ଅକ୍ଷର ଅଚଳ *filename, ଅକ୍ଷର ଅଚଳ *format, ଅକ୍ଷର ଅଚଳ *format2)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = streq (filename, "-") ? 0 : AT_FDCWD;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags = 0;
  ସଂରଚନା stat st;
  ସଂରଚନା statx stx = {0};
  ଅକ୍ଷର ଅଚଳ *pathname = filename;
  ସଂରଚନା print_args pa;
  pa.st = &st;
  pa.btime = (ସଂରଚନା timespec) {.tv_sec = -1, .tv_nsec = -1};

  ଯଦି (AT_FDCWD != fd)
    {
      pathname = "";
      flags = AT_EMPTY_PATH;
    }
  ଅନ୍ୟଥା ଯଦି (!follow_links)
    {
      flags = AT_SYMLINK_NOFOLLOW;
    }

  ଯଦି (dont_sync)
    flags |= AT_STATX_DONT_SYNC;
  ଅନ୍ୟଥା ଯଦି (force_sync)
    flags |= AT_STATX_FORCE_SYNC;

  ଯଦି (! force_sync)
    flags |= AT_NO_AUTOMOUNT;

  fd = statx (fd, pathname, flags, format_to_mask (format), &stx);
  ଯଦି (fd < 0)
    {
      ଯଦି (flags & AT_EMPTY_PATH)
        error (0, errno, _("cannot stat standard input"));
      ଅନ୍ୟଥା
        error (0, errno, _("cannot statx %s"), quoteaf (filename));
      ଫେରନ୍ତୁ false;
    }

  ଯଦି (S_ISBLK (stx.stx_mode) || S_ISCHR (stx.stx_mode))
    format = format2;

  statx_to_stat (&stx, &st);
  ଯଦି (stx.stx_mask & STATX_BTIME)
    pa.btime = statx_timestamp_to_timespec (stx.stx_btime);

  bool fail = print_it (format, fd, filename, print_stat, &pa);
  ଫେରନ୍ତୁ ! fail;
}

#ଅନ୍ୟଥା /* USE_STATX */

ସ୍ଥିର ସଂରଚନା timespec
get_birthtime (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅକ୍ଷର ଅଚଳ *filename, ସଂରଚନା stat ଅଚଳ *st)
{
  ସଂରଚନା timespec ts = get_stat_birthtime (st);

# ଯଦି HAVE_GETATTRAT
  ଯଦି (ts.tv_nsec < 0)
    {
      nvlist_t *response;
      ଯଦି ((fd < 0
           ? getattrat (AT_FDCWD, XATTR_VIEW_READWRITE, filename, &response)
           : fgetattr (fd, XATTR_VIEW_READWRITE, &response))
          == 0)
        {
          uint64_t *val;
          uint_t n;
          ଯଦି (nvlist_lookup_uint64_array (response, A_CRTIME, &val, &n) == 0
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

  ଫେରନ୍ତୁ ts;
}


/* stat the file and print what we find */
NODISCARD
ସ୍ଥିର bool
do_stat (ଅକ୍ଷର ଅଚଳ *filename, ଅକ୍ଷର ଅଚଳ *format,
         ଅକ୍ଷର ଅଚଳ *format2)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = streq (filename, "-") ? 0 : -1;
  ସଂରଚନା stat statbuf;
  ସଂରଚନା print_args pa;
  pa.st = &statbuf;
  pa.btime = (ସଂରଚନା timespec) {.tv_sec = -1, .tv_nsec = -1};

  ଯଦି (0 <= fd)
    {
      ଯଦି (fstat (fd, &statbuf) != 0)
        {
          error (0, errno, _("cannot stat standard input"));
          ଫେରନ୍ତୁ false;
        }
    }
  /* We can't use the shorter
     (follow_links?stat:lstat) (filename, &statbug)
     since stat might be a function-like macro.  */
  ଅନ୍ୟଥା ଯଦି ((follow_links
            ? stat (filename, &statbuf)
            : lstat (filename, &statbuf)) != 0)
    {
      error (0, errno, _("cannot stat %s"), quoteaf (filename));
      ଫେରନ୍ତୁ false;
    }

  ଯଦି (S_ISBLK (statbuf.st_mode) || S_ISCHR (statbuf.st_mode))
    format = format2;

  bool fail = print_it (format, fd, filename, print_stat, &pa);
  ଫେରନ୍ତୁ ! fail;
}
#endif /* USE_STATX */

/* POSIX requires 'ls' to print file sizes without a sign, even
   when negative.  Be consistent with that.  */

ସ୍ଥିର uintmax_t
unsigned_file_size (off_t size)
{
  ଫେରନ୍ତୁ size + (size < 0) * ((uintmax_t) OFF_T_MAX - OFF_T_MIN + 1);
}

/* Print stat info.  Return zero upon success, nonzero upon failure.  */
ସ୍ଥିର bool
print_stat (ଅକ୍ଷର *pformat, size_t prefix_len, ଅକ୍ଷର mod, ଅକ୍ଷର m,
            MAYBE_UNUSED ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅକ୍ଷର ଅଚଳ *filename, ଶୂନ୍ୟ ଅଚଳ *data)
{
  ସଂରଚନା print_args *parg = (ସଂରଚନା print_args *) data;
  ସଂରଚନା stat *statbuf = parg->st;
  ସଂରଚନା timespec btime = parg->btime;
  ସଂରଚନା passwd *pw_ent;
  ସଂରଚନା group *gw_ent;
  bool fail = false;

  ବାଛ (m)
    {
    ସ୍ଥିତି 'n':
      ଯଦି (mod == 'Q')
        {
          initialize_quoting_style ();
          filename = quoteN (filename);
        }
      out_string (pformat, prefix_len, filename);
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'N':
      initialize_quoting_style ();
      out_string (pformat, prefix_len, quoteN (filename));
      ଯଦି (S_ISLNK (statbuf->st_mode))
        {
          ଅକ୍ଷର *linkname = areadlink_with_size (filename, statbuf->st_size);
          ଯଦି (linkname == ଖାଲି)
            {
              error (0, errno, _("cannot read symbolic link %s"),
                     quoteaf (filename));
              ଫେରନ୍ତୁ true;
            }
          printf (" -> ");
          out_string (pformat, prefix_len, quoteN (linkname));
          ମୁକ୍ତ (linkname);
        }
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'd':
      ଯଦି (mod == 'H')
        out_uint (pformat, prefix_len, major (statbuf->st_dev));
      ଅନ୍ୟଥା ଯଦି (mod == 'L')
        out_uint (pformat, prefix_len, minor (statbuf->st_dev));
      ଅନ୍ୟଥା
        out_uint (pformat, prefix_len, statbuf->st_dev);
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'D':
      out_uint_x (pformat, prefix_len, statbuf->st_dev);
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'i':
      out_uint (pformat, prefix_len, statbuf->st_ino);
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'a':
      out_uint_o (pformat, prefix_len, statbuf->st_mode & CHMOD_MODE_BITS);
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'A':
      out_string (pformat, prefix_len, human_access (statbuf));
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'f':
      out_uint_x (pformat, prefix_len, statbuf->st_mode);
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'F':
      out_string (pformat, prefix_len, file_type (statbuf));
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'h':
      out_uint (pformat, prefix_len, statbuf->st_nlink);
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'u':
      out_uint (pformat, prefix_len, statbuf->st_uid);
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'U':
      pw_ent = getpwuid (statbuf->st_uid);
      out_string (pformat, prefix_len,
                  pw_ent ? pw_ent->pw_name : "UNKNOWN");
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'g':
      out_uint (pformat, prefix_len, statbuf->st_gid);
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'G':
      gw_ent = getgrgid (statbuf->st_gid);
      out_string (pformat, prefix_len,
                  gw_ent ? gw_ent->gr_name : "UNKNOWN");
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'm':
      fail |= out_mount_point (filename, pformat, prefix_len, statbuf);
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 's':
      out_uint (pformat, prefix_len, unsigned_file_size (statbuf->st_size));
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'r':
      ଯଦି (mod == 'H')
        out_uint (pformat, prefix_len, major (statbuf->st_rdev));
      ଅନ୍ୟଥା ଯଦି (mod == 'L')
        out_uint (pformat, prefix_len, minor (statbuf->st_rdev));
      ଅନ୍ୟଥା
        out_uint (pformat, prefix_len, statbuf->st_rdev);
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'R':
      out_uint_x (pformat, prefix_len, statbuf->st_rdev);
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 't':
      out_uint_x (pformat, prefix_len, major (statbuf->st_rdev));
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'T':
      out_uint_x (pformat, prefix_len, minor (statbuf->st_rdev));
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'B':
      out_uint (pformat, prefix_len, ST_NBLOCKSIZE);
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'b':
      out_uint (pformat, prefix_len, STP_NBLOCKS (statbuf));
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'o':
      out_uint (pformat, prefix_len, STP_BLKSIZE (statbuf));
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'w':
      {
#ଯଦି ! USE_STATX
        btime = get_birthtime (fd, filename, statbuf);
#endif
        ଯଦି (btime.tv_nsec < 0)
          out_string (pformat, prefix_len, "-");
        ଅନ୍ୟଥା
          out_string (pformat, prefix_len, human_time (btime));
      }
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'W':
      {
#ଯଦି ! USE_STATX
        btime = get_birthtime (fd, filename, statbuf);
#endif
        out_epoch_sec (pformat, prefix_len, neg_to_zero (btime));
      }
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'x':
      out_string (pformat, prefix_len, human_time (get_stat_atime (statbuf)));
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'X':
      out_epoch_sec (pformat, prefix_len, get_stat_atime (statbuf));
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'y':
      out_string (pformat, prefix_len, human_time (get_stat_mtime (statbuf)));
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'Y':
      out_epoch_sec (pformat, prefix_len, get_stat_mtime (statbuf));
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'z':
      out_string (pformat, prefix_len, human_time (get_stat_ctime (statbuf)));
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'Z':
      out_epoch_sec (pformat, prefix_len, get_stat_ctime (statbuf));
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'C':
      fail |= out_file_context (pformat, prefix_len, filename);
      ଭାଙ୍ଗ;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      fputc ('?', stdout);
      ଭାଙ୍ଗ;
    }
  ଫେରନ୍ତୁ fail;
}

/* Return an allocated format string in static storage that
   corresponds to whether FS and TERSE options were declared.  */
ସ୍ଥିର ଅକ୍ଷର *
default_format (bool fs, bool terse, bool device)
{
  ଅକ୍ଷର *format;
  ଯଦି (fs)
    {
      ଯଦି (terse)
        format = xstrdup (fmt_terse_fs);
      ଅନ୍ୟଥା
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
  ଅନ୍ୟଥା /* ! fs */
    {
      ଯଦି (terse)
        {
          ଯଦି (0 < is_selinux_enabled ())
            format = xstrdup (fmt_terse_selinux);
          ଅନ୍ୟଥା
            format = xstrdup (fmt_terse_regular);
        }
      ଅନ୍ୟଥା
        {
          ଅକ୍ଷର *temp;
          /* TRANSLATORS: This string uses format specifiers from
             'stat --help' without --file-system, and NOT from printf.  */
          format = xstrdup (_("\
  File: %N\n\
  Size: %-10s\tBlocks: %-10b IO Block: %-6o %F\n\
"));

          temp = format;
          ଯଦି (device)
            {
              /* TRANSLATORS: This string uses format specifiers from
                 'stat --help' without --file-system, and NOT from printf.  */
              format = xasprintf ("%s%s", format, _("\
" "Device: %Hd,%Ld\tInode: %-10i  Links: %-5h Device type: %Hr,%Lr\n\
"));
            }
          ଅନ୍ୟଥା
            {
              /* TRANSLATORS: This string uses format specifiers from
                 'stat --help' without --file-system, and NOT from printf.  */
              format = xasprintf ("%s%s", format, _("\
" "Device: %Hd,%Ld\tInode: %-10i  Links: %h\n\
"));
            }
          ମୁକ୍ତ (temp);

          temp = format;
          /* TRANSLATORS: This string uses format specifiers from
             'stat --help' without --file-system, and NOT from printf.  */
          format = xasprintf ("%s%s", format, _("\
" "Access: (%04a/%10.10A)  Uid: (%5u/%8U)   Gid: (%5g/%8G)\n\
"));
          ମୁକ୍ତ (temp);

          ଯଦି (0 < is_selinux_enabled ())
            {
              temp = format;
              /* TRANSLATORS: This string uses format specifiers from
                 'stat --help' without --file-system, and NOT from printf.  */
              format = xasprintf ("%s%s", format, _("Context: %C\n"));
              ମୁକ୍ତ (temp);
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
          ମୁକ୍ତ (temp);
        }
    }
  ଫେରନ୍ତୁ format;
}

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
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
         use the specified FORMAT instead of the ପୂର୍ବନିର୍ଦ୍ଧାରିତ;\n\
         output a newline after each use of FORMAT\n\
"));
      oputs (_("\
      --printf=FORMAT\n\
         like --format, but interpret backslash escapes,\n\
         and କର not output a mandatory trailing newline;\n\
         ଯଦି you want a newline, include \\n in FORMAT\n\
"));
      oputs (_("\
  -t, --terse\n\
         print the information in terse form\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);

      fputs (_("\n\
The MODE argument of --cached can be: always, never, or ପୂର୍ବନିର୍ଦ୍ଧାରିତ.\n\
'always' will use cached attributes ଯଦି available, ଯେତେବେଳେ\n\
'never' will try to synchronize with the latest attributes, and\n\
'default' will leave it up to the underlying file system.\n\
"), stdout);

      fputs (_("\n\
The valid format sequences ପାଇଁ files (without --file-system):\n\
\n\
  %a   permission bits in octal (see '#' and '0' printf flags)\n\
  %A   permission bits and file type in human readable form\n\
  %b   ସଂଖ୍ୟା of blocks allocated (see %B)\n\
  %B   the size in bytes of each block reported by %b\n\
  %C   SELinux security context string\n\
"), stdout);
      fputs (_("\
  %d   device ସଂଖ୍ୟା in decimal (st_dev)\n\
  %D   device ସଂଖ୍ୟା in hex (st_dev)\n\
  %Hd  major device ସଂଖ୍ୟା in decimal\n\
  %Ld  minor device ସଂଖ୍ୟା in decimal\n\
  %f   raw mode in hex\n\
  %F   file type\n\
  %g   group ID of owner\n\
  %G   group name of owner\n\
"), stdout);
      fputs (_("\
  %h   ସଂଖ୍ୟା of hard links\n\
  %i   inode ସଂଖ୍ୟା\n\
  %m   mount point\n\
  %n   file name\n\
  %Qn  quoted file name\n\
  %N   quoted file name with dereference ଯଦି symbolic link\n\
  %o   optimal I/O transfer size hint\n\
  %s   total size, in bytes\n\
  %r   device type in decimal (st_rdev)\n\
  %R   device type in hex (st_rdev)\n\
  %Hr  major device type in decimal, ପାଇଁ character/block device special files\n\
  %Lr  minor device type in decimal, ପାଇଁ character/block device special files\n\
  %t   major device type in hex, ପାଇଁ character/block device special files\n\
  %T   minor device type in hex, ପାଇଁ character/block device special files\n\
"), stdout);
      fputs (_("\
  %u   user ID of owner\n\
  %U   user name of owner\n\
  %w   time of file birth, human-readable; - ଯଦି unknown\n\
  %W   time of file birth, seconds since Epoch; 0 ଯଦି unknown\n\
  %x   time of last access, human-readable\n\
  %X   time of last access, seconds since Epoch\n\
  %y   time of last data modification, human-readable\n\
  %Y   time of last data modification, seconds since Epoch\n\
  %z   time of last status change, human-readable\n\
  %Z   time of last status change, seconds since Epoch\n\
\n\
"), stdout);

      fputs (_("\
Valid format sequences ପାଇଁ file systems:\n\
\n\
  %a   ମୁକ୍ତ blocks available to non-superuser\n\
  %b   total data blocks in file system\n\
  %c   total file nodes in file system\n\
  %d   ମୁକ୍ତ file nodes in file system\n\
  %f   ମୁକ୍ତ blocks in file system\n\
"), stdout);
      fputs (_("\
  %i   file system ID in hex\n\
  %l   maximum length of filenames\n\
  %n   file name\n\
  %Qn  quoted file name\n\
  %s   block size (ପାଇଁ faster transfers)\n\
  %S   fundamental block size (ପାଇଁ block counts)\n\
  %t   file system type in hex\n\
  %T   file system type in human readable form\n\
"), stdout);

      printf (_("\n\
--terse is equivalent to the following FORMAT:\n\
    %s\
"),
#ଯଦି HAVE_SELINUX_SELINUX_H
              fmt_terse_selinux
#ଅନ୍ୟଥା
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

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର *argv[])
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା c;
  bool fs = false;
  bool terse = false;
  ଅକ୍ଷର *format = ଖାଲି;
  ଅକ୍ଷର *format2;
  bool ok = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  ସଂରଚନା lconv ଅଚଳ *locale = localeconv ();
  decimal_point = (locale->decimal_point[0] ? locale->decimal_point : ".");
  decimal_point_len = strlen (decimal_point);

  atexit (close_stdout);

  ଯେତେବେଳେ ((c = getopt_long (argc, argv, "c:fLt", long_options, ଖାଲି)) != -1)
    {
      ବାଛ (c)
        {
        ସ୍ଥିତି PRINTF_OPTION:
          format = optarg;
          interpret_backslash_escapes = true;
          trailing_delim = "";
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'c':
          format = optarg;
          interpret_backslash_escapes = false;
          trailing_delim = "\n";
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'L':
          follow_links = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'f':
          fs = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 't':
          terse = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 0:
          ବାଛ (XARGMATCH ("--cached", optarg, cached_args, cached_modes))
            {
              ସ୍ଥିତି cached_never:
                force_sync = true;
                dont_sync = false;
                ଭାଙ୍ଗ;
              ସ୍ଥିତି cached_always:
                force_sync = false;
                dont_sync = true;
                ଭାଙ୍ଗ;
              ସ୍ଥିତି cached_default:
                force_sync = false;
                dont_sync = false;
            }
          ଭାଙ୍ଗ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          usage (EXIT_FAILURE);
        }
    }

  ଯଦି (argc == optind)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  ଯଦି (format)
    {
      format2 = format;
    }
  ଅନ୍ୟଥା
    {
      format = default_format (fs, terse, /* device= */ false);
      format2 = default_format (fs, terse, /* device= */ true);
    }

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = optind; i < argc; i++)
    ok &= (fs
           ? do_statfs (argv[i], format)
           : do_stat (argv[i], format, format2));

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
