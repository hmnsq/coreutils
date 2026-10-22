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
#जर ((STAT_STATVFS || STAT_STATVFS64)                                       \
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
#जर USE_STATVFS
# include <sys/statvfs.h>
#किंवाजर HAVE_SYS_VFS_H
# include <sys/vfs.h>
#किंवाजर HAVE_SYS_MOUNT_H && HAVE_SYS_PARAM_H
/* NOTE: freebsd5.0 needs sys/param.h and sys/mount.h for statfs.
   It does have statvfs.h, but shouldn't use it, since it doesn't
   HAVE_STRUCT_STATVFS_F_BASETYPE.  So find a clean way to fix it.  */
/* NetBSD 1.5.2 needs these, for the declaration of struct statfs. */
# include <sys/param.h>
# include <sys/mount.h>
# जर HAVE_NFS_NFS_CLNT_H && HAVE_NFS_VFS_H
/* Ultrix 4.4 needs these for the declaration of struct statfs.  */
#  include <netinet/in.h>
#  include <nfs/nfs_clnt.h>
#  include <nfs/vfs.h>
# endif
#किंवाजर HAVE_OS_H /* BeOS */
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

#जर HAVE_STATX && defined STATX_INO
# define USE_STATX 1
#अन्यथा
# define USE_STATX 0
#endif

#जर USE_STATVFS
# define STRUCT_STATXFS_F_FSID_IS_INTEGER STRUCT_STATVFS_F_FSID_IS_INTEGER
# define HAVE_STRUCT_STATXFS_F_TYPE HAVE_STRUCT_STATVFS_F_TYPE
# जर HAVE_STRUCT_STATVFS_F_NAMEMAX
#  define SB_F_NAMEMAX(S) ((S)->f_namemax)
# endif
# जर ! STAT_STATVFS && STAT_STATVFS64
#  define STRUCT_STATVFS संरचना statvfs64
#  define STATFS statvfs64
# अन्यथा
#  define STRUCT_STATVFS संरचना statvfs
#  define STATFS statvfs
# endif
# define STATFS_FRSIZE(S) ((S)->f_frsize)
#अन्यथा
# define HAVE_STRUCT_STATXFS_F_TYPE HAVE_STRUCT_STATFS_F_TYPE
# जर HAVE_STRUCT_STATFS_F_NAMELEN
#  define SB_F_NAMEMAX(S) ((S)->f_namelen)
# elif HAVE_STRUCT_STATFS_F_NAMEMAX
#  define SB_F_NAMEMAX(S) ((S)->f_namemax)
# endif
# define STATFS statfs
# जर HAVE_OS_H /* BeOS */
/* BeOS has a statvfs function, but it does not return sensible values
   for f_files, f_ffree and f_favail, and lacks f_type, f_basetype and
   f_fstypename.  Use 'struct fs_info' instead.  */
NODISCARD
स्थिर पूर्णांक
statfs (अक्षर स्थिरांक *filename, संरचना fs_info *buf)
{
  dev_t device = dev_for_path (filename);
  जर (device < 0)
    {
      त्रुटि_सं = (device == B_ENTRY_NOT_FOUND ? ENOENT
               : device == B_BAD_मूल्य ? EINVAL
               : device == B_NAME_TOO_LONG ? ENAMETOOLONG
               : device == B_NO_MEMORY ? ENOMEM
               : device == B_FILE_ERROR ? EIO
               : 0);
      परत -1;
    }
  /* If successful, buf->dev will be == device.  */
  परत fs_stat_dev (device, buf);
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
#  जर HAVE_STRUCT_STATFS_F_FRSIZE
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

#जर HAVE_STRUCT_STATVFS_F_BASETYPE
# define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME f_basetype
#अन्यथा
# जर HAVE_STRUCT_STATVFS_F_FSTYPENAME || HAVE_STRUCT_STATFS_F_FSTYPENAME
#  define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME f_fstypename
# elif HAVE_OS_H /* BeOS */
#  define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME fsh_name
# endif
#endif

#जर HAVE_GETATTRAT
# include <attr.h>
# include <sys/nvpair.h>
#endif

स्थिर अक्षर स्थिरांक digits[] = "0123456789";

/* Flags that are portable for use in printf, for at least one
   conversion specifier; make_format removes non-portable flags as
   needed for particular specifiers.  The glibc 2.2 extension "I" is
   listed here; it is removed by make_format because it has undefined
   behavior elsewhere and because it is incompatible with
   out_epoch_sec.  */
स्थिर अक्षर स्थिरांक printf_flags[] = "'-+ #0I";

/* Formats for the --terse option.  */
स्थिर अक्षर स्थिरांक fmt_terse_fs[] = "%Qn %i %l %t %s %S %b %f %a %c %d\n";
स्थिर अक्षर स्थिरांक fmt_terse_regular[] = "%Qn %s %b %f %u %g %D %i %h %t %T"
                                        " %X %Y %Z %W %o\n";
स्थिर अक्षर स्थिरांक fmt_terse_selinux[] = "%Qn %s %b %f %u %g %D %i %h %t %T"
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

स्थिर अक्षर स्थिरांक *स्थिरांक cached_args[] =
{
  "default", "never", "always", रिक्त
};

स्थिर गणना cached_mode स्थिरांक cached_modes[] =
{
  cached_default, cached_never, cached_always
};

स्थिर संरचना option स्थिरांक long_options[] =
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
स्थिर अक्षर स्थिरांक *trailing_delim = "";

/* The representation of the decimal point in the current locale.  */
स्थिर अक्षर स्थिरांक *decimal_point;
स्थिर माप_प्रकार decimal_point_len;

स्थिर bool
print_stat (अक्षर *pformat, माप_प्रकार prefix_len, अक्षर mod, अक्षर m,
            पूर्णांक fd, अक्षर स्थिरांक *filename, शून्य स्थिरांक *data);

/* Return the type of the specified file system.
   Some systems have statfvs.f_basetype[FSTYPSZ] (AIX, HP-UX, and Solaris).
   Others have statvfs.f_fstypename[_VFS_NAMELEN] (NetBSD 3.0).
   Others have statfs.f_fstypename[MFSNAMELEN] (NetBSD 1.5.2).
   Still others have neither and have to get by with f_type (GNU/Linux).
   But f_type may only exist in statfs (Cygwin).  */
NODISCARD
स्थिर अक्षर स्थिरांक *
human_fstype (STRUCT_STATVFS स्थिरांक *statfsbuf)
{
#ifdef STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME
  परत statfsbuf->STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME;
#अन्यथा
  निवड (statfsbuf->f_type)
    {
# जर defined __linux__ || defined __ANDROID__

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

    स्थिती S_MAGIC_AAFS: /* 0x5A3C69F0 local */
      परत "aafs";
    स्थिती S_MAGIC_ACFS: /* 0x61636673 remote */
      परत "acfs";
    स्थिती S_MAGIC_ADFS: /* 0xADF5 local */
      परत "adfs";
    स्थिती S_MAGIC_AFFS: /* 0xADFF local */
      परत "affs";
    स्थिती S_MAGIC_AFS: /* 0x5346414F remote */
      परत "afs";
    स्थिती S_MAGIC_ANON_INODE_FS: /* 0x09041934 local */
      परत "anon-inode FS";
    स्थिती S_MAGIC_AUFS: /* 0x61756673 remote */
      /* FIXME: change syntax or add an optional attribute like "inotify:no".
         The above is labeled as "remote" so that tail always uses polling,
         but this isn't really a remote file system type.  */
      परत "aufs";
    स्थिती S_MAGIC_AUTOFS: /* 0x0187 local */
      परत "autofs";
    स्थिती S_MAGIC_BALLOON_KVM: /* 0x13661366 local */
      परत "balloon-kvm-fs";
    स्थिती S_MAGIC_BCACHEFS: /* 0xCA451A4E local */
      परत "bcachefs";
    स्थिती S_MAGIC_BEFS: /* 0x42465331 local */
      परत "befs";
    स्थिती S_MAGIC_BDEVFS: /* 0x62646576 local */
      परत "bdevfs";
    स्थिती S_MAGIC_BFS: /* 0x1BADFACE local */
      परत "bfs";
    स्थिती S_MAGIC_BINDERFS: /* 0x6C6F6F70 local */
      परत "binderfs";
    स्थिती S_MAGIC_BPF_FS: /* 0xCAFE4A11 local */
      परत "bpf_fs";
    स्थिती S_MAGIC_BINFMTFS: /* 0x42494E4D local */
      परत "binfmt_misc";
    स्थिती S_MAGIC_BTRFS: /* 0x9123683E local */
      परत "btrfs";
    स्थिती S_MAGIC_BTRFS_TEST: /* 0x73727279 local */
      परत "btrfs_test";
    स्थिती S_MAGIC_CEPH: /* 0x00C36400 remote */
      परत "ceph";
    स्थिती S_MAGIC_CGROUP: /* 0x0027E0EB local */
      परत "cgroupfs";
    स्थिती S_MAGIC_CGROUP2: /* 0x63677270 local */
      परत "cgroup2fs";
    स्थिती S_MAGIC_CIFS: /* 0xFF534D42 remote */
      परत "cifs";
    स्थिती S_MAGIC_CODA: /* 0x73757245 remote */
      परत "coda";
    स्थिती S_MAGIC_COH: /* 0x012FF7B7 local */
      परत "coh";
    स्थिती S_MAGIC_CONFIGFS: /* 0x62656570 local */
      परत "configfs";
    स्थिती S_MAGIC_CRAMFS: /* 0x28CD3D45 local */
      परत "cramfs";
    स्थिती S_MAGIC_CRAMFS_WEND: /* 0x453DCD28 local */
      परत "cramfs-wend";
    स्थिती S_MAGIC_DAXFS: /* 0x64646178 local */
      परत "daxfs";
    स्थिती S_MAGIC_DEBUGFS: /* 0x64626720 local */
      परत "debugfs";
    स्थिती S_MAGIC_DEVFS: /* 0x1373 local */
      परत "devfs";
    स्थिती S_MAGIC_DEVMEM: /* 0x454D444D local */
      परत "devmem";
    स्थिती S_MAGIC_DEVPTS: /* 0x1CD1 local */
      परत "devpts";
    स्थिती S_MAGIC_DMA_BUF: /* 0x444D4142 local */
      परत "dma-buf-fs";
    स्थिती S_MAGIC_ECRYPTFS: /* 0xF15F local */
      परत "ecryptfs";
    स्थिती S_MAGIC_EFIVARFS: /* 0xDE5E81E4 local */
      परत "efivarfs";
    स्थिती S_MAGIC_EFS: /* 0x00414A53 local */
      परत "efs";
    स्थिती S_MAGIC_EROFS_V1: /* 0xE0F5E1E2 local */
      परत "erofs";
    स्थिती S_MAGIC_EXFAT: /* 0x2011BAB0 local */
      परत "exfat";
    स्थिती S_MAGIC_EXFS: /* 0x45584653 local */
      परत "exfs";
    स्थिती S_MAGIC_EXOFS: /* 0x5DF5 local */
      परत "exofs";
    स्थिती S_MAGIC_EXT: /* 0x137D local */
      परत "ext";
    स्थिती S_MAGIC_EXT2: /* 0xEF53 local */
      परत "ext2/ext3";
    स्थिती S_MAGIC_EXT2_OLD: /* 0xEF51 local */
      परत "ext2";
    स्थिती S_MAGIC_F2FS: /* 0xF2F52010 local */
      परत "f2fs";
    स्थिती S_MAGIC_FAIL_FS: /* 0x4641494C local */
      परत "failfs";
    स्थिती S_MAGIC_FAT: /* 0x4006 local */
      परत "fat";
    स्थिती S_MAGIC_FHGFS: /* 0x19830326 remote */
      परत "fhgfs";
    स्थिती S_MAGIC_FUSE: /* 0x65735546 remote */
      परत "fuse";
    स्थिती S_MAGIC_FUSECTL: /* 0x65735543 remote */
      परत "fusectl";
    स्थिती S_MAGIC_FUTEXFS: /* 0x0BAD1DEA local */
      परत "futexfs";
    स्थिती S_MAGIC_GFS: /* 0x01161970 remote */
      परत "gfs/gfs2";
    स्थिती S_MAGIC_GPFS: /* 0x47504653 remote */
      परत "gpfs";
    स्थिती S_MAGIC_GUEST_MEMFD: /* 0x474D454D remote */
      परत "guest-memfd";
    स्थिती S_MAGIC_HFS: /* 0x4244 local */
      परत "hfs";
    स्थिती S_MAGIC_HFS_PLUS: /* 0x482B local */
      परत "hfs+";
    स्थिती S_MAGIC_HFS_X: /* 0x4858 local */
      परत "hfsx";
    स्थिती S_MAGIC_HOSTFS: /* 0x00C0FFEE local */
      परत "hostfs";
    स्थिती S_MAGIC_HPFS: /* 0xF995E849 local */
      परत "hpfs";
    स्थिती S_MAGIC_HUGETLBFS: /* 0x958458F6 local */
      परत "hugetlbfs";
    स्थिती S_MAGIC_MTD_INODE_FS: /* 0x11307854 local */
      परत "inodefs";
    स्थिती S_MAGIC_IBRIX: /* 0x013111A8 remote */
      परत "ibrix";
    स्थिती S_MAGIC_INOTIFYFS: /* 0x2BAD1DEA local */
      परत "inotifyfs";
    स्थिती S_MAGIC_ISOFS: /* 0x9660 local */
      परत "isofs";
    स्थिती S_MAGIC_ISOFS_R_WIN: /* 0x4004 local */
      परत "isofs";
    स्थिती S_MAGIC_ISOFS_WIN: /* 0x4000 local */
      परत "isofs";
    स्थिती S_MAGIC_JFFS: /* 0x07C0 local */
      परत "jffs";
    स्थिती S_MAGIC_JFFS2: /* 0x72B6 local */
      परत "jffs2";
    स्थिती S_MAGIC_JFS: /* 0x3153464A local */
      परत "jfs";
    स्थिती S_MAGIC_KAFS: /* 0x6B414653 remote */
      परत "k-afs";
    स्थिती S_MAGIC_LOGFS: /* 0xC97E8168 local */
      परत "logfs";
    स्थिती S_MAGIC_LUSTRE: /* 0x0BD00BD0 remote */
      परत "lustre";
    स्थिती S_MAGIC_M1FS: /* 0x5346314D local */
      परत "m1fs";
    स्थिती S_MAGIC_MINIX: /* 0x137F local */
      परत "minix";
    स्थिती S_MAGIC_MINIX_30: /* 0x138F local */
      परत "minix (30 char.)";
    स्थिती S_MAGIC_MINIX_V2: /* 0x2468 local */
      परत "minix v2";
    स्थिती S_MAGIC_MINIX_V2_30: /* 0x2478 local */
      परत "minix v2 (30 char.)";
    स्थिती S_MAGIC_MINIX_V3: /* 0x4D5A local */
      परत "minix3";
    स्थिती S_MAGIC_MQUEUE: /* 0x19800202 local */
      परत "mqueue";
    स्थिती S_MAGIC_MSDOS: /* 0x4D44 local */
      परत "msdos";
    स्थिती S_MAGIC_NCP: /* 0x564C remote */
      परत "novell";
    स्थिती S_MAGIC_NFS: /* 0x6969 remote */
      परत "nfs";
    स्थिती S_MAGIC_NFSD: /* 0x6E667364 remote */
      परत "nfsd";
    स्थिती S_MAGIC_NILFS: /* 0x3434 local */
      परत "nilfs";
    स्थिती S_MAGIC_NSFS: /* 0x6E736673 local */
      परत "nsfs";
    स्थिती S_MAGIC_NTFS: /* 0x5346544E local */
      परत "ntfs";
    स्थिती S_MAGIC_NULL_FS: /* 0x4E554C4C local */
      परत "nullfs";
    स्थिती S_MAGIC_OPENPROM: /* 0x9FA1 local */
      परत "openprom";
    स्थिती S_MAGIC_OCFS2: /* 0x7461636F remote */
      परत "ocfs2";
    स्थिती S_MAGIC_OVERLAYFS: /* 0x794C7630 remote */
      /* This may overlay remote file systems.
         Also there have been issues reported with inotify and overlayfs,
         so mark as "remote" so that polling is used.  */
      परत "overlayfs";
    स्थिती S_MAGIC_PANFS: /* 0xAAD7AAEA remote */
      परत "panfs";
    स्थिती S_MAGIC_PID_FS: /* 0x50494446 local */
      परत "pidfs";
    स्थिती S_MAGIC_PIPEFS: /* 0x50495045 remote */
      /* FIXME: change syntax or add an optional attribute like "inotify:no".
         pipefs is labeled as "remote" so that tail always polls,
         but it isn't really remote file system type.  */
      परत "pipefs";
    स्थिती S_MAGIC_PPC_CMM: /* 0xC7571590 local */
      परत "ppc-cmm-fs";
    स्थिती S_MAGIC_PRL_FS: /* 0x7C7C6673 remote */
      परत "prl_fs";
    स्थिती S_MAGIC_PROC: /* 0x9FA0 local */
      परत "proc";
    स्थिती S_MAGIC_PSTOREFS: /* 0x6165676C local */
      परत "pstorefs";
    स्थिती S_MAGIC_QNX4: /* 0x002F local */
      परत "qnx4";
    स्थिती S_MAGIC_QNX6: /* 0x68191122 local */
      परत "qnx6";
    स्थिती S_MAGIC_RAMFS: /* 0x858458F6 local */
      परत "ramfs";
    स्थिती S_MAGIC_RDTGROUP: /* 0x07655821 local */
      परत "rdt";
    स्थिती S_MAGIC_REISERFS: /* 0x52654973 local */
      परत "reiserfs";
    स्थिती S_MAGIC_ROMFS: /* 0x7275 local */
      परत "romfs";
    स्थिती S_MAGIC_RPC_PIPEFS: /* 0x67596969 local */
      परत "rpc_pipefs";
    स्थिती S_MAGIC_SDCARDFS: /* 0x5DCA2DF5 local */
      परत "sdcardfs";
    स्थिती S_MAGIC_SECRETMEM: /* 0x5345434D local */
      परत "secretmem";
    स्थिती S_MAGIC_SECURITYFS: /* 0x73636673 local */
      परत "securityfs";
    स्थिती S_MAGIC_SELINUX: /* 0xF97CFF8C local */
      परत "selinux";
    स्थिती S_MAGIC_SMACK: /* 0x43415D53 local */
      परत "smackfs";
    स्थिती S_MAGIC_SMB: /* 0x517B remote */
      परत "smb";
    स्थिती S_MAGIC_SMB2: /* 0xFE534D42 remote */
      परत "smb2";
    स्थिती S_MAGIC_SNFS: /* 0xBEEFDEAD remote */
      परत "snfs";
    स्थिती S_MAGIC_SOCKFS: /* 0x534F434B local */
      परत "sockfs";
    स्थिती S_MAGIC_SQUASHFS: /* 0x73717368 local */
      परत "squashfs";
    स्थिती S_MAGIC_SYSFS: /* 0x62656572 local */
      परत "sysfs";
    स्थिती S_MAGIC_SYSV2: /* 0x012FF7B6 local */
      परत "sysv2";
    स्थिती S_MAGIC_SYSV4: /* 0x012FF7B5 local */
      परत "sysv4";
    स्थिती S_MAGIC_TMPFS: /* 0x01021994 local */
      परत "tmpfs";
    स्थिती S_MAGIC_TRACEFS: /* 0x74726163 local */
      परत "tracefs";
    स्थिती S_MAGIC_UBIFS: /* 0x24051905 local */
      परत "ubifs";
    स्थिती S_MAGIC_UDF: /* 0x15013346 local */
      परत "udf";
    स्थिती S_MAGIC_UFS: /* 0x00011954 local */
      परत "ufs";
    स्थिती S_MAGIC_UFS_BYTESWAPPED: /* 0x54190100 local */
      परत "ufs";
    स्थिती S_MAGIC_USBDEVFS: /* 0x9FA2 local */
      परत "usbdevfs";
    स्थिती S_MAGIC_V9FS: /* 0x01021997 local */
      परत "v9fs";
    स्थिती S_MAGIC_VBOXSF: /* 0x786F4256 remote */
      परत "vboxsf";
    स्थिती S_MAGIC_VMHGFS: /* 0xBACBACBC remote */
      परत "vmhgfs";
    स्थिती S_MAGIC_VXFS: /* 0xA501FCF5 remote */
      /* Veritas File System can run in single instance or clustered mode,
         so mark as remote to cater for the latter case.  */
      परत "vxfs";
    स्थिती S_MAGIC_VZFS: /* 0x565A4653 local */
      परत "vzfs";
    स्थिती S_MAGIC_WSLFS: /* 0x53464846 local */
      परत "wslfs";
    स्थिती S_MAGIC_XENFS: /* 0xABBA1974 local */
      परत "xenfs";
    स्थिती S_MAGIC_XENIX: /* 0x012FF7B4 local */
      परत "xenix";
    स्थिती S_MAGIC_XFS: /* 0x58465342 local */
      परत "xfs";
    स्थिती S_MAGIC_XIAFS: /* 0x012FD16D local */
      परत "xia";
    स्थिती S_MAGIC_Z3FOLD: /* 0x0033 local */
      परत "z3fold";
    स्थिती S_MAGIC_ZFS: /* 0x2FC12FC1 local */
      परत "zfs";
    स्थिती S_MAGIC_ZONEFS: /* 0x5A4F4653 local */
      परत "zonefs";
    स्थिती S_MAGIC_ZSMALLOC: /* 0x58295829 local */
      परत "zsmallocfs";


# elif __GNU__
    स्थिती FSTYPE_UFS:
      परत "ufs";
    स्थिती FSTYPE_NFS:
      परत "nfs";
    स्थिती FSTYPE_GFS:
      परत "gfs";
    स्थिती FSTYPE_LFS:
      परत "lfs";
    स्थिती FSTYPE_SYSV:
      परत "sysv";
    स्थिती FSTYPE_FTP:
      परत "ftp";
    स्थिती FSTYPE_TAR:
      परत "tar";
    स्थिती FSTYPE_AR:
      परत "ar";
    स्थिती FSTYPE_CPIO:
      परत "cpio";
    स्थिती FSTYPE_MSLOSS:
      परत "msloss";
    स्थिती FSTYPE_CPM:
      परत "cpm";
    स्थिती FSTYPE_HFS:
      परत "hfs";
    स्थिती FSTYPE_DTFS:
      परत "dtfs";
    स्थिती FSTYPE_GRFS:
      परत "grfs";
    स्थिती FSTYPE_TERM:
      परत "term";
    स्थिती FSTYPE_DEV:
      परत "dev";
    स्थिती FSTYPE_PROC:
      परत "proc";
    स्थिती FSTYPE_IFSOCK:
      परत "ifsock";
    स्थिती FSTYPE_AFS:
      परत "afs";
    स्थिती FSTYPE_DFS:
      परत "dfs";
    स्थिती FSTYPE_PROC9:
      परत "proc9";
    स्थिती FSTYPE_SOCKET:
      परत "socket";
    स्थिती FSTYPE_MISC:
      परत "misc";
    स्थिती FSTYPE_EXT2FS:
      परत "ext2/ext3";
    स्थिती FSTYPE_HTTP:
      परत "http";
    स्थिती FSTYPE_MEMFS:
      परत "memfs";
    स्थिती FSTYPE_ISO9660:
      परत "iso9660";
# endif
    पूर्वनिर्धारित:
      {
        अचिन्हित दीर्घ पूर्णांक type = statfsbuf->f_type;
        स्थिर अक्षर buf[आकार "UNKNOWN (0x%lx)" - 3
                        + (आकार type * अक्षर_बिट + 3) / 4];
        प्र_लिखो (buf, "UNKNOWN (0x%lx)", type);
        परत buf;
      }
    }
#endif
}

NODISCARD
स्थिर अक्षर *
human_access (संरचना stat स्थिरांक *statbuf)
{
  स्थिर अक्षर modebuf[12];
  filemodestring (statbuf, modebuf);
  modebuf[10] = 0;
  परत modebuf;
}

NODISCARD
स्थिर अक्षर *
human_time (संरचना timespec t)
{
  /* STR must be at least INT_BUFSIZE_BOUND (intmax_t) big, either
     because localtime_rz fails, or because the time zone is truly
     outlandish so that %z expands to a long string.  */
  स्थिर अक्षर str[INT_BUFSIZE_BOUND (intmax_t)
                  + INT_STRLEN_BOUND (पूर्णांक) /* YYYY */
                  + 1 /* because YYYY might equal INT_MAX + 1900 */
                  + आकार "-MM-DD HH:MM:SS.NNNNNNNNN +"];
  स्थिर timezone_t tz;
  जर (!tz)
    tz = tzalloc (दो_पर्या ("TZ"));
  संरचना पंचांग पंचांग;
  पूर्णांक ns = t.tv_nsec;
  जर (localtime_rz (tz, &t.tv_sec, &पंचांग))
    nstrftime (str, आकार str, "%Y-%m-%d %H:%M:%S.%N %z", &पंचांग, tz, ns);
  अन्यथा
    {
      अक्षर secbuf[INT_BUFSIZE_BOUND (intmax_t)];
      प्र_लिखो (str, "%s.%09d", timetostr (t.tv_sec, secbuf), ns);
    }
  परत str;
}

/* PFORMAT points to a '%' followed by a prefix of a format, all of
   size PREFIX_LEN.  The flags allowed for this format are
   ALLOWED_FLAGS; remove other printf flags from the prefix, then
   append SUFFIX.  */
स्थिर शून्य
make_format (अक्षर *pformat, माप_प्रकार prefix_len, अक्षर स्थिरांक *allowed_flags,
             अक्षर स्थिरांक *suffix)
{
  अक्षर *dst = pformat + 1;
  अक्षर स्थिरांक *src;
  अक्षर स्थिरांक *srclim = pformat + prefix_len;
  साठी (src = dst; src < srclim && म_अक्षर (printf_flags, *src); src++)
    जर (म_अक्षर (allowed_flags, *src))
      *dst++ = *src;
  पर्यंत (src < srclim)
    *dst++ = *src++;
  म_नकल (dst, suffix);
}

स्थिर शून्य
out_string (अक्षर *pformat, माप_प्रकार prefix_len, अक्षर स्थिरांक *arg)
{
  make_format (pformat, prefix_len, "-", "s");
  म_लिखो (pformat, arg);
}
स्थिर पूर्णांक
out_int (अक्षर *pformat, माप_प्रकार prefix_len, intmax_t arg)
{
  make_format (pformat, prefix_len, "'-+ 0", "jd");
  परत म_लिखो (pformat, arg);
}
स्थिर पूर्णांक
out_uint (अक्षर *pformat, माप_प्रकार prefix_len, uintmax_t arg)
{
  make_format (pformat, prefix_len, "'-0", "ju");
  परत म_लिखो (pformat, arg);
}
स्थिर शून्य
out_uint_o (अक्षर *pformat, माप_प्रकार prefix_len, uintmax_t arg)
{
  make_format (pformat, prefix_len, "-#0", "jo");
  म_लिखो (pformat, arg);
}
स्थिर शून्य
out_uint_x (अक्षर *pformat, माप_प्रकार prefix_len, uintmax_t arg)
{
  make_format (pformat, prefix_len, "-#0", "jx");
  म_लिखो (pformat, arg);
}
स्थिर पूर्णांक
out_minus_zero (अक्षर *pformat, माप_प्रकार prefix_len)
{
  make_format (pformat, prefix_len, "'-+ 0", ".0f");
  परत म_लिखो (pformat, -0.25);
}

/* Output the number of seconds since the Epoch, using a format that
   acts like printf's %f format.  */
स्थिर शून्य
out_epoch_sec (अक्षर *pformat, माप_प्रकार prefix_len,
               संरचना timespec arg)
{
  अक्षर *dot = स_प्रथम (pformat, '.', prefix_len);
  माप_प्रकार sec_prefix_len = prefix_len;
  पूर्णांक width = 0;
  पूर्णांक precision = 0;
  bool frac_डावा_adjust = false;

  जर (dot)
    {
      sec_prefix_len = dot - pformat;
      pformat[prefix_len] = '\0';

      जर (c_isdigit (dot[1]))
        {
          दीर्घ पूर्णांक lprec = म_से_दीर्घ (dot + 1, रिक्त, 10);
          precision = (lprec <= पूर्णांक_उच्च ? lprec : पूर्णांक_उच्च);
        }
      अन्यथा
        {
          precision = 9;
        }

      जर (precision && c_isdigit (dot[-1]))
        {
          /* If a nontrivial width is given, subtract the width of the
             decimal point and PRECISION digits that will be output
             later.  */
          अक्षर *p = dot;
          *dot = '\0';

          करा
            --p;
          पर्यंत (c_isdigit (p[-1]));

          दीर्घ पूर्णांक lwidth = म_से_दीर्घ (p, रिक्त, 10);
          width = (lwidth <= पूर्णांक_उच्च ? lwidth : पूर्णांक_उच्च);
          जर (1 < width)
            {
              p += (*p == '0');
              sec_prefix_len = p - pformat;
              पूर्णांक w_d = (decimal_point_len < width
                         ? width - decimal_point_len
                         : 0);
              जर (1 < w_d)
                {
                  पूर्णांक w = w_d - precision;
                  जर (1 < w)
                    {
                      अक्षर *dst = pformat;
                      साठी (अक्षर स्थिरांक *src = dst; src < p; src++)
                        {
                          जर (*src == '-')
                            frac_डावा_adjust = true;
                          अन्यथा
                            *dst++ = *src;
                        }
                      sec_prefix_len =
                        (dst - pformat
                         + (frac_डावा_adjust ? 0 : प्र_लिखो (dst, "%d", w)));
                    }
                }
            }
        }
    }

  पूर्णांक भाजक = 1;
  साठी (पूर्णांक i = precision; i < 9; i++)
    भाजक *= 10;
  पूर्णांक frac_sec = arg.tv_nsec / भाजक;
  पूर्णांक int_len;

  जर (TYPE_SIGNED (समय_प्रकार))
    {
      bool minus_zero = false;
      जर (arg.tv_sec < 0 && arg.tv_nsec != 0)
        {
          पूर्णांक frac_sec_modulus = 1000000000 / भाजक;
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

  जर (precision)
    {
      पूर्णांक prec = (precision < 9 ? precision : 9);
      पूर्णांक trailing_prec = precision - prec;
      पूर्णांक ilen = (int_len < 0 ? 0 : int_len);
      पूर्णांक trailing_width = (ilen < width && decimal_point_len < width - ilen
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
out_file_context (अक्षर *pformat, माप_प्रकार prefix_len, अक्षर स्थिरांक *filename)
{
  अक्षर *scontext;
  bool fail = false;

  जर ((follow_links
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
  जर (scontext)
    freecon (scontext);
  परत fail;
}

/* Set the quoting style once it is needed.  */
स्थिर शून्य
initialize_quoting_style (शून्य)
{
  स्थिर bool initialized;
  जर (!initialized)
    {
      initialized = true;
      पूर्णांक qs = getenv_quoting_style ();
      set_quoting_style (रिक्त, 0 <= qs ? qs : shell_escape_quoting_style);
    }
}

/* Print statfs info.  Return zero upon success, nonzero upon failure.  */
NODISCARD
स्थिर bool
print_statfs (अक्षर *pformat, माप_प्रकार prefix_len, MAYBE_UNUSED अक्षर mod, अक्षर m,
              MAYBE_UNUSED पूर्णांक fd, अक्षर स्थिरांक *filename,
              शून्य स्थिरांक *data)
{
  STRUCT_STATVFS स्थिरांक *statfsbuf = data;
  bool fail = false;

  निवड (m)
    {
    स्थिती 'n':
      जर (mod == 'Q')
        {
          initialize_quoting_style ();
          filename = quoteN (filename);
        }
      out_string (pformat, prefix_len, filename);
      थांबा;

    स्थिती 'i':
      {
#जर STRUCT_STATXFS_F_FSID_IS_INTEGER
        uintmax_t fsid = statfsbuf->f_fsid;
#अन्यथा
        प्रकारव्याख्या अचिन्हित पूर्णांक fsid_word;
        static_assert (alignof (STRUCT_STATVFS) % alignof (fsid_word) == 0);
        static_assert (दुरत्व (STRUCT_STATVFS, f_fsid) % alignof (fsid_word)
                       == 0);
        static_assert (आकार statfsbuf->f_fsid % alignof (fsid_word) == 0);
        fsid_word स्थिरांक *p = (fsid_word *) &statfsbuf->f_fsid;

        /* Assume a little-endian word order, as that is compatible
           with glibc's statvfs implementation.  */
        uintmax_t fsid = 0;
        पूर्णांक शब्द = आकार statfsbuf->f_fsid / आकार *p;
        साठी (पूर्णांक i = 0; i < शब्द && i * आकार *p < आकार fsid; i++)
          {
            uintmax_t u = p[शब्द - 1 - i];
            fsid |= u << (i * अक्षर_बिट * आकार *p);
          }
#endif
        out_uint_x (pformat, prefix_len, fsid);
      }
      थांबा;

    स्थिती 'l':
      OUT_NAMEMAX (pformat, prefix_len, SB_F_NAMEMAX (statfsbuf));
      थांबा;
    स्थिती 't':
#जर HAVE_STRUCT_STATXFS_F_TYPE
      out_uint_x (pformat, prefix_len, statfsbuf->f_type);
#अन्यथा
      ख_अक्षर_दो ('?', मानक_निकास);
#endif
      थांबा;
    स्थिती 'T':
      out_string (pformat, prefix_len, human_fstype (statfsbuf));
      थांबा;
    स्थिती 'b':
      out_int (pformat, prefix_len, statfsbuf->f_blocks);
      थांबा;
    स्थिती 'f':
      out_int (pformat, prefix_len, statfsbuf->f_bfree);
      थांबा;
    स्थिती 'a':
      out_int (pformat, prefix_len, statfsbuf->f_bavail);
      थांबा;
    स्थिती 's':
      out_uint (pformat, prefix_len, statfsbuf->f_bsize);
      थांबा;
    स्थिती 'S':
      {
        uintmax_t frsize = STATFS_FRSIZE (statfsbuf);
        जर (! frsize)
          frsize = statfsbuf->f_bsize;
        out_uint (pformat, prefix_len, frsize);
      }
      थांबा;
    स्थिती 'c':
      out_uint (pformat, prefix_len, statfsbuf->f_files);
      थांबा;
    स्थिती 'd':
      out_int (pformat, prefix_len, statfsbuf->f_ffree);
      थांबा;
    पूर्वनिर्धारित:
      ख_अक्षर_दो ('?', मानक_निकास);
      थांबा;
    }
  परत fail;
}

/* Return any bind mounted source for a path.
   The caller should not free the returned buffer.
   Return NULL if no bind mount found.  */
NODISCARD
स्थिर अक्षर स्थिरांक *
find_bind_mount (अक्षर स्थिरांक * name)
{
  अक्षर स्थिरांक * bind_mount = रिक्त;

  स्थिर संरचना mount_entry *mount_list;
  स्थिर bool tried_mount_list = false;
  जर (!tried_mount_list) /* attempt/warn once per process.  */
    {
      जर (!(mount_list = read_file_system_list (false)))
        error (0, त्रुटि_सं, "%s", _("cannot read table of mounted file systems"));
      tried_mount_list = true;
    }

  संरचना stat name_stats;
  जर (stat (name, &name_stats) != 0)
    परत रिक्त;

  साठी (संरचना mount_entry *me = mount_list; me; me = me->me_next)
    {
      जर (me->me_dummy && me->me_devname[0] == '/'
          && streq (me->me_mountdir, name))
        {
          संरचना stat dev_stats;

          जर (stat (me->me_devname, &dev_stats) == 0
              && psame_inode (&name_stats, &dev_stats))
            {
              bind_mount = me->me_devname;
              थांबा;
            }
        }
    }

  परत bind_mount;
}

/* Print mount point.  Return zero upon success, nonzero upon failure.  */
NODISCARD
स्थिर bool
out_mount_point (अक्षर स्थिरांक *filename, अक्षर *pformat, माप_प्रकार prefix_len,
                 स्थिरांक संरचना stat *statp)
{

  अक्षर स्थिरांक *np = "?", *bp = रिक्त;
  अक्षर *mp = रिक्त;
  bool fail = true;

  /* Look for bind mounts first.  Note we output the immediate alias,
     rather than further resolving to a base device mount point.  */
  जर (follow_links || !S_ISLNK (statp->st_mode))
    {
      अक्षर *resolved = canonicalize_file_name (filename);
      जर (!resolved)
        {
          error (0, त्रुटि_सं, _("failed to canonicalize %s"), quoteaf (filename));
          जा print_mount_point;
        }
      bp = find_bind_mount (resolved);
      मुक्त (resolved);
      जर (bp)
        {
          fail = false;
          जा print_mount_point;
        }
    }

  /* If there is no direct bind mount, then navigate
     back up the tree looking for a device change.
     Note we don't detect if any of the directory components
     are bind mounted to the same device, but that's OK
     since we've not directly queried them.  */
  जर ((mp = find_mount_point (filename, statp)))
    {
      /* This dir might be bind mounted to another device,
         so we resolve the bound source in that case also.  */
      bp = find_bind_mount (mp);
      fail = false;
    }

print_mount_point:

  out_string (pformat, prefix_len, bp ? bp : mp ? mp : np);
  मुक्त (mp);
  परत fail;
}

/* Map a TS with negative TS.tv_nsec to {0,0}.  */
स्थिर ओळीत संरचना timespec
neg_to_zero (संरचना timespec ts)
{
  जर (0 <= ts.tv_nsec)
    परत ts;
  संरचना timespec z = {0};
  परत z;
}

/* Output a single-character \ escape.  */

स्थिर शून्य
print_esc_char (अक्षर c)
{
  निवड (c)
    {
    स्थिती 'a':			/* Alert. */
      c ='\a';
      थांबा;
    स्थिती 'b':			/* Backspace. */
      c ='\b';
      थांबा;
    स्थिती 'e':			/* Escape. */
      c ='\x1B';
      थांबा;
    स्थिती 'f':			/* Form feed. */
      c ='\f';
      थांबा;
    स्थिती 'n':			/* New line. */
      c ='\n';
      थांबा;
    स्थिती 'r':			/* Carriage return. */
      c ='\r';
      थांबा;
    स्थिती 't':			/* Horizontal tab. */
      c ='\t';
      थांबा;
    स्थिती 'v':			/* Vertical tab. */
      c ='\v';
      थांबा;
    स्थिती '"':
    स्थिती '\\':
      थांबा;
    पूर्वनिर्धारित:
      error (0, 0, _("warning: unrecognized escape '\\%c'"), c);
      थांबा;
    }
  अक्षर_दो (c);
}

ATTRIBUTE_PURE
स्थिर माप_प्रकार
format_code_offset (अक्षर स्थिरांक *directive)
{
  माप_प्रकार len = म_अखोज (directive + 1, printf_flags);
  अक्षर स्थिरांक *fmt_char = directive + len + 1;
  fmt_char += म_अखोज (fmt_char, digits);
  जर (*fmt_char == '.')
    fmt_char += 1 + म_अखोज (fmt_char + 1, digits);
  परत fmt_char - directive;
}

/* Print the information specified by the format string, FORMAT,
   calling PRINT_FUNC for each %-directive encountered.
   Return zero upon success, nonzero upon failure.  */
NODISCARD
स्थिर bool
print_it (अक्षर स्थिरांक *format, पूर्णांक fd, अक्षर स्थिरांक *filename,
          bool (*print_func) (अक्षर *, माप_प्रकार, अक्षर, अक्षर,
                              पूर्णांक, अक्षर स्थिरांक *, शून्य स्थिरांक *),
          शून्य स्थिरांक *data)
{
  bool fail = false;

  /* Add 2 to accommodate our conversion of the stat '%s' format string
     to the longer printf '%llu' one.  */
  गणना
    {
      MAX_ADDITIONAL_BYTES =
        (MAX (आकार "jd",
              MAX (आकार "jo", MAX (आकार "ju", आकार "jx")))
         - 1)
    };
  माप_प्रकार n_alloc = म_माप (format) + MAX_ADDITIONAL_BYTES + 1;
  अक्षर *dest = xmalloc (n_alloc);
  साठी (अक्षर स्थिरांक *b = format; *b; b++)
    {
      निवड (*b)
        {
        स्थिती '%':
          {
            माप_प्रकार len = format_code_offset (b);
            अक्षर fmt_char = *(b + len);
            अक्षर mod_char = 0;
            स_नकल (dest, b, len);
            b += len;

            निवड (fmt_char)
              {
              स्थिती '\0':
                --b;
                FALLTHROUGH;
              स्थिती '%':
                जर (1 < len)
                  {
                    dest[len] = fmt_char;
                    dest[len + 1] = '\0';
                    error (निकास_त्रुटि, 0, _("%s: invalid directive"),
                           quote (dest));
                  }
                अक्षर_दो ('%');
                थांबा;
              स्थिती 'H':
              स्थिती 'L':
              स्थिती 'Q':
                mod_char = fmt_char;
                fmt_char = *(b + 1);
                जर ((mod_char == 'Q' && fmt_char == 'n')
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
              पूर्वनिर्धारित:
                fail |= print_func (dest, len, mod_char, fmt_char,
                                    fd, filename, data);
                थांबा;
              }
            थांबा;
          }

        स्थिती '\\':
          जर ( ! interpret_backslash_escapes)
            {
              अक्षर_दो ('\\');
              थांबा;
            }
          ++b;
          जर (isoct (*b))
            {
              पूर्णांक esc_मूल्य = fromoct (*b);
              पूर्णांक esc_length = 1;	/* number of octal digits */
              साठी (++b; esc_length < 3 && isoct (*b);
                   ++esc_length, ++b)
                {
                  esc_मूल्य = esc_मूल्य * 8 + fromoct (*b);
                }
              अक्षर_दो (esc_मूल्य);
              --b;
            }
          अन्यथा जर (*b == 'x' && c_isxdigit (b[1]))
            {
              पूर्णांक esc_मूल्य = fromhex (b[1]);	/* Value of \xhh escape. */
              /* A hexadecimal \xhh escape sequence must have
                 1 or 2 hex. digits.  */
              ++b;
              जर (c_isxdigit (b[1]))
                {
                  ++b;
                  esc_मूल्य = esc_मूल्य * 16 + fromhex (*b);
                }
              अक्षर_दो (esc_मूल्य);
            }
          अन्यथा जर (*b == '\0')
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
          थांबा;

        पूर्वनिर्धारित:
          अक्षर_दो (*b);
          थांबा;
        }
    }
  मुक्त (dest);

  ख_माला_दो (trailing_delim, मानक_निकास);

  परत fail;
}

/* Stat the file system and print what we find.  */
NODISCARD
स्थिर bool
do_statfs (अक्षर स्थिरांक *filename, अक्षर स्थिरांक *format)
{
  STRUCT_STATVFS statfsbuf;

  जर (streq (filename, "-"))
    {
      error (0, 0, _("using %s to denote standard input does not work"
                     " in file system mode"), quoteaf (filename));
      परत false;
    }

  जर (STATFS (filename, &statfsbuf) != 0)
    {
      error (0, त्रुटि_सं, _("cannot read file system information for %s"),
             quoteaf (filename));
      परत false;
    }

  bool fail = print_it (format, -1, filename, print_statfs, &statfsbuf);
  परत ! fail;
}

संरचना print_args {
  संरचना stat *st;
  संरचना timespec btime;
};

/* Ask statx to avoid syncing? */
स्थिर bool dont_sync;

/* Ask statx to force sync? */
स्थिर bool force_sync;

#जर USE_STATX
स्थिर अचिन्हित पूर्णांक
fmt_to_mask (अक्षर fmt)
{
  निवड (fmt)
    {
    स्थिती 'N':
      परत STATX_MODE;
    स्थिती 'd':
    स्थिती 'D':
      परत STATX_MODE;
    स्थिती 'i':
      परत STATX_INO;
    स्थिती 'a':
    स्थिती 'A':
      परत STATX_MODE;
    स्थिती 'f':
      परत STATX_MODE|STATX_TYPE;
    स्थिती 'F':
      परत STATX_TYPE;
    स्थिती 'h':
      परत STATX_NLINK;
    स्थिती 'u':
    स्थिती 'U':
      परत STATX_UID;
    स्थिती 'g':
    स्थिती 'G':
      परत STATX_GID;
    स्थिती 'm':
      परत STATX_MODE|STATX_INO;
    स्थिती 's':
      परत STATX_SIZE;
    स्थिती 't':
    स्थिती 'T':
      परत STATX_MODE;
    स्थिती 'b':
      परत STATX_BLOCKS;
    स्थिती 'w':
    स्थिती 'W':
      परत STATX_BTIME;
    स्थिती 'x':
    स्थिती 'X':
      परत STATX_ATIME;
    स्थिती 'y':
    स्थिती 'Y':
      परत STATX_MTIME;
    स्थिती 'z':
    स्थिती 'Z':
      परत STATX_CTIME;
    }
  परत 0;
}

ATTRIBUTE_PURE
स्थिर अचिन्हित पूर्णांक
format_to_mask (अक्षर स्थिरांक *format)
{
  अचिन्हित पूर्णांक mask = 0;

  साठी (अक्षर स्थिरांक *b = format; *b; b++)
    {
      जर (*b != '%')
        सुरूठेवा;

      b += format_code_offset (b);
      जर (*b == '\0')
        थांबा;
      mask |= fmt_to_mask (*b);
    }
  परत mask;
}

/* statx the file and print what we find */
NODISCARD
स्थिर bool
do_stat (अक्षर स्थिरांक *filename, अक्षर स्थिरांक *format, अक्षर स्थिरांक *format2)
{
  पूर्णांक fd = streq (filename, "-") ? 0 : AT_FDCWD;
  पूर्णांक flags = 0;
  संरचना stat st;
  संरचना statx stx = {0};
  अक्षर स्थिरांक *pathname = filename;
  संरचना print_args pa;
  pa.st = &st;
  pa.btime = (संरचना timespec) {.tv_sec = -1, .tv_nsec = -1};

  जर (AT_FDCWD != fd)
    {
      pathname = "";
      flags = AT_EMPTY_PATH;
    }
  अन्यथा जर (!follow_links)
    {
      flags = AT_SYMLINK_NOFOLLOW;
    }

  जर (dont_sync)
    flags |= AT_STATX_DONT_SYNC;
  अन्यथा जर (force_sync)
    flags |= AT_STATX_FORCE_SYNC;

  जर (! force_sync)
    flags |= AT_NO_AUTOMOUNT;

  fd = statx (fd, pathname, flags, format_to_mask (format), &stx);
  जर (fd < 0)
    {
      जर (flags & AT_EMPTY_PATH)
        error (0, त्रुटि_सं, _("cannot stat standard input"));
      अन्यथा
        error (0, त्रुटि_सं, _("cannot statx %s"), quoteaf (filename));
      परत false;
    }

  जर (S_ISBLK (stx.stx_mode) || S_ISCHR (stx.stx_mode))
    format = format2;

  statx_to_stat (&stx, &st);
  जर (stx.stx_mask & STATX_BTIME)
    pa.btime = statx_timestamp_to_timespec (stx.stx_btime);

  bool fail = print_it (format, fd, filename, print_stat, &pa);
  परत ! fail;
}

#अन्यथा /* USE_STATX */

स्थिर संरचना timespec
get_birthtime (पूर्णांक fd, अक्षर स्थिरांक *filename, संरचना stat स्थिरांक *st)
{
  संरचना timespec ts = get_stat_birthtime (st);

# जर HAVE_GETATTRAT
  जर (ts.tv_nsec < 0)
    {
      nvlist_t *response;
      जर ((fd < 0
           ? getattrat (AT_FDCWD, XATTR_VIEW_READWRITE, filename, &response)
           : fgetattr (fd, XATTR_VIEW_READWRITE, &response))
          == 0)
        {
          uint64_t *val;
          uint_t n;
          जर (nvlist_lookup_uint64_array (response, A_CRTIME, &val, &n) == 0
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

  परत ts;
}


/* stat the file and print what we find */
NODISCARD
स्थिर bool
do_stat (अक्षर स्थिरांक *filename, अक्षर स्थिरांक *format,
         अक्षर स्थिरांक *format2)
{
  पूर्णांक fd = streq (filename, "-") ? 0 : -1;
  संरचना stat statbuf;
  संरचना print_args pa;
  pa.st = &statbuf;
  pa.btime = (संरचना timespec) {.tv_sec = -1, .tv_nsec = -1};

  जर (0 <= fd)
    {
      जर (ख_स्थिति (fd, &statbuf) != 0)
        {
          error (0, त्रुटि_सं, _("cannot stat standard input"));
          परत false;
        }
    }
  /* We can't use the shorter
     (follow_links?stat:lstat) (filename, &statbug)
     since stat might be a function-like macro.  */
  अन्यथा जर ((follow_links
            ? stat (filename, &statbuf)
            : lstat (filename, &statbuf)) != 0)
    {
      error (0, त्रुटि_सं, _("cannot stat %s"), quoteaf (filename));
      परत false;
    }

  जर (S_ISBLK (statbuf.st_mode) || S_ISCHR (statbuf.st_mode))
    format = format2;

  bool fail = print_it (format, fd, filename, print_stat, &pa);
  परत ! fail;
}
#endif /* USE_STATX */

/* POSIX requires 'ls' to print file sizes without a sign, even
   when negative.  Be consistent with that.  */

स्थिर uintmax_t
unsigned_file_size (off_t size)
{
  परत size + (size < 0) * ((uintmax_t) OFF_T_MAX - OFF_T_MIN + 1);
}

/* Print stat info.  Return zero upon success, nonzero upon failure.  */
स्थिर bool
print_stat (अक्षर *pformat, माप_प्रकार prefix_len, अक्षर mod, अक्षर m,
            MAYBE_UNUSED पूर्णांक fd, अक्षर स्थिरांक *filename, शून्य स्थिरांक *data)
{
  संरचना print_args *parg = (संरचना print_args *) data;
  संरचना stat *statbuf = parg->st;
  संरचना timespec btime = parg->btime;
  संरचना passwd *pw_ent;
  संरचना group *gw_ent;
  bool fail = false;

  निवड (m)
    {
    स्थिती 'n':
      जर (mod == 'Q')
        {
          initialize_quoting_style ();
          filename = quoteN (filename);
        }
      out_string (pformat, prefix_len, filename);
      थांबा;
    स्थिती 'N':
      initialize_quoting_style ();
      out_string (pformat, prefix_len, quoteN (filename));
      जर (S_ISLNK (statbuf->st_mode))
        {
          अक्षर *linkname = areadlink_with_size (filename, statbuf->st_size);
          जर (linkname == रिक्त)
            {
              error (0, त्रुटि_सं, _("cannot read symbolic link %s"),
                     quoteaf (filename));
              परत true;
            }
          म_लिखो (" -> ");
          out_string (pformat, prefix_len, quoteN (linkname));
          मुक्त (linkname);
        }
      थांबा;
    स्थिती 'd':
      जर (mod == 'H')
        out_uint (pformat, prefix_len, major (statbuf->st_dev));
      अन्यथा जर (mod == 'L')
        out_uint (pformat, prefix_len, minor (statbuf->st_dev));
      अन्यथा
        out_uint (pformat, prefix_len, statbuf->st_dev);
      थांबा;
    स्थिती 'D':
      out_uint_x (pformat, prefix_len, statbuf->st_dev);
      थांबा;
    स्थिती 'i':
      out_uint (pformat, prefix_len, statbuf->st_ino);
      थांबा;
    स्थिती 'a':
      out_uint_o (pformat, prefix_len, statbuf->st_mode & CHMOD_MODE_BITS);
      थांबा;
    स्थिती 'A':
      out_string (pformat, prefix_len, human_access (statbuf));
      थांबा;
    स्थिती 'f':
      out_uint_x (pformat, prefix_len, statbuf->st_mode);
      थांबा;
    स्थिती 'F':
      out_string (pformat, prefix_len, file_type (statbuf));
      थांबा;
    स्थिती 'h':
      out_uint (pformat, prefix_len, statbuf->st_nlink);
      थांबा;
    स्थिती 'u':
      out_uint (pformat, prefix_len, statbuf->st_uid);
      थांबा;
    स्थिती 'U':
      pw_ent = getpwuid (statbuf->st_uid);
      out_string (pformat, prefix_len,
                  pw_ent ? pw_ent->pw_name : "UNKNOWN");
      थांबा;
    स्थिती 'g':
      out_uint (pformat, prefix_len, statbuf->st_gid);
      थांबा;
    स्थिती 'G':
      gw_ent = getgrgid (statbuf->st_gid);
      out_string (pformat, prefix_len,
                  gw_ent ? gw_ent->gr_name : "UNKNOWN");
      थांबा;
    स्थिती 'm':
      fail |= out_mount_point (filename, pformat, prefix_len, statbuf);
      थांबा;
    स्थिती 's':
      out_uint (pformat, prefix_len, unsigned_file_size (statbuf->st_size));
      थांबा;
    स्थिती 'r':
      जर (mod == 'H')
        out_uint (pformat, prefix_len, major (statbuf->st_rdev));
      अन्यथा जर (mod == 'L')
        out_uint (pformat, prefix_len, minor (statbuf->st_rdev));
      अन्यथा
        out_uint (pformat, prefix_len, statbuf->st_rdev);
      थांबा;
    स्थिती 'R':
      out_uint_x (pformat, prefix_len, statbuf->st_rdev);
      थांबा;
    स्थिती 't':
      out_uint_x (pformat, prefix_len, major (statbuf->st_rdev));
      थांबा;
    स्थिती 'T':
      out_uint_x (pformat, prefix_len, minor (statbuf->st_rdev));
      थांबा;
    स्थिती 'B':
      out_uint (pformat, prefix_len, ST_NBLOCKSIZE);
      थांबा;
    स्थिती 'b':
      out_uint (pformat, prefix_len, STP_NBLOCKS (statbuf));
      थांबा;
    स्थिती 'o':
      out_uint (pformat, prefix_len, STP_BLKSIZE (statbuf));
      थांबा;
    स्थिती 'w':
      {
#जर ! USE_STATX
        btime = get_birthtime (fd, filename, statbuf);
#endif
        जर (btime.tv_nsec < 0)
          out_string (pformat, prefix_len, "-");
        अन्यथा
          out_string (pformat, prefix_len, human_time (btime));
      }
      थांबा;
    स्थिती 'W':
      {
#जर ! USE_STATX
        btime = get_birthtime (fd, filename, statbuf);
#endif
        out_epoch_sec (pformat, prefix_len, neg_to_zero (btime));
      }
      थांबा;
    स्थिती 'x':
      out_string (pformat, prefix_len, human_time (get_stat_atime (statbuf)));
      थांबा;
    स्थिती 'X':
      out_epoch_sec (pformat, prefix_len, get_stat_atime (statbuf));
      थांबा;
    स्थिती 'y':
      out_string (pformat, prefix_len, human_time (get_stat_mtime (statbuf)));
      थांबा;
    स्थिती 'Y':
      out_epoch_sec (pformat, prefix_len, get_stat_mtime (statbuf));
      थांबा;
    स्थिती 'z':
      out_string (pformat, prefix_len, human_time (get_stat_ctime (statbuf)));
      थांबा;
    स्थिती 'Z':
      out_epoch_sec (pformat, prefix_len, get_stat_ctime (statbuf));
      थांबा;
    स्थिती 'C':
      fail |= out_file_context (pformat, prefix_len, filename);
      थांबा;
    पूर्वनिर्धारित:
      ख_अक्षर_दो ('?', मानक_निकास);
      थांबा;
    }
  परत fail;
}

/* Return an allocated format string in static storage that
   corresponds to whether FS and TERSE options were declared.  */
स्थिर अक्षर *
default_format (bool fs, bool terse, bool device)
{
  अक्षर *format;
  जर (fs)
    {
      जर (terse)
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
      जर (terse)
        {
          जर (0 < is_selinux_enabled ())
            format = xstrdup (fmt_terse_selinux);
          अन्यथा
            format = xstrdup (fmt_terse_regular);
        }
      अन्यथा
        {
          अक्षर *temp;
          /* TRANSLATORS: This string uses format specifiers from
             'stat --help' without --file-system, and NOT from printf.  */
          format = xstrdup (_("\
  File: %N\n\
  Size: %-10s\tBlocks: %-10b IO Block: %-6o %F\n\
"));

          temp = format;
          जर (device)
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

          जर (0 < is_selinux_enabled ())
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
  परत format;
}

शून्य
usage (पूर्णांक status)
{
  जर (status != निकास_सफल)
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
         use the specified FORMAT instead of the पूर्वनिर्धारित;\n\
         output a newline after each use of FORMAT\n\
"));
      oputs (_("\
      --म_लिखो=FORMAT\n\
         like --format, but interpret backslash escapes,\n\
         and करा not output a mandatory trailing newline;\n\
         जर you want a newline, include \\n in FORMAT\n\
"));
      oputs (_("\
  -t, --terse\n\
         print the information in terse form\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);

      ख_माला_दो (_("\n\
The MODE argument of --cached can be: always, never, or पूर्वनिर्धारित.\n\
'always' will use cached attributes जर available, पर्यंत\n\
'never' will try to synchronize with the latest attributes, and\n\
'default' will leave it up to the underlying file प्रणाली.\n\
"), मानक_निकास);

      ख_माला_दो (_("\n\
The valid format sequences साठी files (without --file-प्रणाली):\n\
\n\
  %a   permission bits in octal (see '#' and '0' म_लिखो flags)\n\
  %A   permission bits and file type in human readable form\n\
  %b   संख्या of blocks allocated (see %B)\n\
  %B   the size in bytes of each block reported by %b\n\
  %C   SELinux security context string\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  %d   device संख्या in decimal (st_dev)\n\
  %D   device संख्या in hex (st_dev)\n\
  %Hd  major device संख्या in decimal\n\
  %Ld  minor device संख्या in decimal\n\
  %f   raw mode in hex\n\
  %F   file type\n\
  %g   group ID of owner\n\
  %G   group name of owner\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  %h   संख्या of hard links\n\
  %i   inode संख्या\n\
  %m   mount point\n\
  %n   file name\n\
  %Qn  quoted file name\n\
  %N   quoted file name with dereference जर symbolic link\n\
  %o   optimal I/O transfer size hint\n\
  %s   total size, in bytes\n\
  %r   device type in decimal (st_rdev)\n\
  %R   device type in hex (st_rdev)\n\
  %Hr  major device type in decimal, साठी character/block device special files\n\
  %Lr  minor device type in decimal, साठी character/block device special files\n\
  %t   major device type in hex, साठी character/block device special files\n\
  %T   minor device type in hex, साठी character/block device special files\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  %u   user ID of owner\n\
  %U   user name of owner\n\
  %w   समय of file birth, human-readable; - जर unknown\n\
  %W   समय of file birth, seconds since Epoch; 0 जर unknown\n\
  %x   समय of last access, human-readable\n\
  %X   समय of last access, seconds since Epoch\n\
  %y   समय of last data modification, human-readable\n\
  %Y   समय of last data modification, seconds since Epoch\n\
  %z   समय of last status change, human-readable\n\
  %Z   समय of last status change, seconds since Epoch\n\
\n\
"), मानक_निकास);

      ख_माला_दो (_("\
Valid format sequences साठी file systems:\n\
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
  %s   block size (साठी faster transfers)\n\
  %S   fundamental block size (साठी block counts)\n\
  %t   file प्रणाली type in hex\n\
  %T   file प्रणाली type in human readable form\n\
"), मानक_निकास);

      म_लिखो (_("\n\
--terse is equivalent to the following FORMAT:\n\
    %s\
"),
#जर HAVE_SELINUX_SELINUX_H
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

पूर्णांक
main (पूर्णांक argc, अक्षर *argv[])
{
  पूर्णांक c;
  bool fs = false;
  bool terse = false;
  अक्षर *format = रिक्त;
  अक्षर *format2;
  bool ok = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  संरचना स्था_बदल स्थिरांक *locale = क्षेत्र_बदलो ();
  decimal_point = (locale->decimal_point[0] ? locale->decimal_point : ".");
  decimal_point_len = म_माप (decimal_point);

  निकास_पर (close_stdout);

  पर्यंत ((c = getopt_long (argc, argv, "c:fLt", long_options, रिक्त)) != -1)
    {
      निवड (c)
        {
        स्थिती PRINTF_OPTION:
          format = optarg;
          interpret_backslash_escapes = true;
          trailing_delim = "";
          थांबा;

        स्थिती 'c':
          format = optarg;
          interpret_backslash_escapes = false;
          trailing_delim = "\n";
          थांबा;

        स्थिती 'L':
          follow_links = true;
          थांबा;

        स्थिती 'f':
          fs = true;
          थांबा;

        स्थिती 't':
          terse = true;
          थांबा;

        स्थिती 0:
          निवड (XARGMATCH ("--cached", optarg, cached_args, cached_modes))
            {
              स्थिती cached_never:
                force_sync = true;
                dont_sync = false;
                थांबा;
              स्थिती cached_always:
                force_sync = false;
                dont_sync = true;
                थांबा;
              स्थिती cached_default:
                force_sync = false;
                dont_sync = false;
            }
          थांबा;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        पूर्वनिर्धारित:
          usage (निकास_त्रुटि);
        }
    }

  जर (argc == optind)
    {
      error (0, 0, _("missing operand"));
      usage (निकास_त्रुटि);
    }

  जर (format)
    {
      format2 = format;
    }
  अन्यथा
    {
      format = default_format (fs, terse, /* device= */ false);
      format2 = default_format (fs, terse, /* device= */ true);
    }

  साठी (पूर्णांक i = optind; i < argc; i++)
    ok &= (fs
           ? do_statfs (argv[i], format)
           : do_stat (argv[i], format, format2));

  main_exit (ok ? निकास_सफल : निकास_त्रुटि);
}
