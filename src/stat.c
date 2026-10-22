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

#समावेश <config.h>

/* Keep this conditional in sync with the similar conditional in
   ../m4/stat-prog.m4.  */
#अगर ((STAT_STATVFS || STAT_STATVFS64)                                       \
     && (HAVE_STRUCT_STATVFS_F_BASETYPE || HAVE_STRUCT_STATVFS_F_FSTYPENAME \
         || (! HAVE_STRUCT_STATFS_F_FSTYPENAME && HAVE_STRUCT_STATVFS_F_TYPE)))
# define USE_STATVFS 1
#अन्यथा
# define USE_STATVFS 0
#पूर्ण_अगर

#समावेश <stdio.h>
#समावेश <sys/types.h>
#समावेश <pwd.h>
#समावेश <grp.h>
#अगर USE_STATVFS
# include <sys/statvfs.h>
#या_अगर HAVE_SYS_VFS_H
# include <sys/vfs.h>
#या_अगर HAVE_SYS_MOUNT_H && HAVE_SYS_PARAM_H
/* NOTE: freebsd5.0 needs sys/param.h and sys/mount.h for statfs.
   It does have statvfs.h, but shouldn't use it, since it doesn't
   HAVE_STRUCT_STATVFS_F_BASETYPE.  So find a clean way to fix it.  */
/* NetBSD 1.5.2 needs these, for the declaration of struct statfs. */
# include <sys/param.h>
# include <sys/mount.h>
# अगर HAVE_NFS_NFS_CLNT_H && HAVE_NFS_VFS_H
/* Ultrix 4.4 needs these for the declaration of struct statfs.  */
#  include <netinet/in.h>
#  include <nfs/nfs_clnt.h>
#  include <nfs/vfs.h>
# endif
#या_अगर HAVE_OS_H /* BeOS */
# include <fs_info.h>
#पूर्ण_अगर
#समावेश <selinux/selinux.h>
#समावेश <getopt.h>

#समावेश "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#समावेश "system.h"

#समावेश "areadlink.h"
#समावेश "c-ctype.h"
#समावेश "file-type.h"
#समावेश "filemode.h"
#समावेश "fs.h"
#समावेश "mountlist.h"
#समावेश "octhexdigits.h"
#समावेश "quote.h"
#समावेश "stat-size.h"
#समावेश "stat-time.h"
#समावेश "strftime.h"
#समावेश "find-mount-point.h"
#समावेश "xvasprintf.h"
#समावेश "statx.h"

#अगर HAVE_STATX && defined STATX_INO
# define USE_STATX 1
#अन्यथा
# define USE_STATX 0
#पूर्ण_अगर

#अगर USE_STATVFS
# define STRUCT_STATXFS_F_FSID_IS_INTEGER STRUCT_STATVFS_F_FSID_IS_INTEGER
# define HAVE_STRUCT_STATXFS_F_TYPE HAVE_STRUCT_STATVFS_F_TYPE
# अगर HAVE_STRUCT_STATVFS_F_NAMEMAX
#  define SB_F_NAMEMAX(S) ((S)->f_namemax)
# endif
# अगर ! STAT_STATVFS && STAT_STATVFS64
#  define STRUCT_STATVFS काष्ठा statvfs64
#  define STATFS statvfs64
# अन्यथा
#  define STRUCT_STATVFS काष्ठा statvfs
#  define STATFS statvfs
# endif
# define STATFS_FRSIZE(S) ((S)->f_frsize)
#अन्यथा
# define HAVE_STRUCT_STATXFS_F_TYPE HAVE_STRUCT_STATFS_F_TYPE
# अगर HAVE_STRUCT_STATFS_F_NAMELEN
#  define SB_F_NAMEMAX(S) ((S)->f_namelen)
# elif HAVE_STRUCT_STATFS_F_NAMEMAX
#  define SB_F_NAMEMAX(S) ((S)->f_namemax)
# endif
# define STATFS statfs
# अगर HAVE_OS_H /* BeOS */
/* BeOS has a statvfs function, but it does not return sensible values
   for f_files, f_ffree and f_favail, and lacks f_type, f_basetype and
   f_fstypename.  Use 'struct fs_info' instead.  */
NODISCARD
अटल पूर्णांक
statfs (अक्षर स्थिर *filename, काष्ठा fs_info *buf)
{
  dev_t device = dev_for_path (filename);
  अगर (device < 0)
    {
      त्रुटि_सं = (device == B_ENTRY_NOT_FOUND ? ENOENT
               : device == B_BAD_मूल्य ? EINVAL
               : device == B_NAME_TOO_LONG ? ENAMETOOLONG
               : device == B_NO_MEMORY ? ENOMEM
               : device == B_FILE_ERROR ? EIO
               : 0);
      वापस -1;
    }
  /* If successful, buf->dev will be == device.  */
  वापस fs_stat_dev (device, buf);
}
#  define f_fsid dev
#  define f_blocks योग_blocks
#  define f_bfree free_blocks
#  define f_bavail free_blocks
#  define f_bsize io_आकार
#  define f_files योग_nodes
#  define f_ffree free_nodes
#  define STRUCT_STATVFS काष्ठा fs_info
#  define STRUCT_STATXFS_F_FSID_IS_INTEGER true
#  define STATFS_FRSIZE(S) ((S)->block_आकार)
# अन्यथा
#  define STRUCT_STATVFS काष्ठा statfs
#  define STRUCT_STATXFS_F_FSID_IS_INTEGER STRUCT_STATFS_F_FSID_IS_INTEGER
#  अगर HAVE_STRUCT_STATFS_F_FRSIZE
#   define STATFS_FRSIZE(S) ((S)->f_frsize)
#  अन्यथा
#   define STATFS_FRSIZE(S) 0
#  endif
# endif
#पूर्ण_अगर

#अगर_घोषित SB_F_NAMEMAX
# define OUT_NAMEMAX out_uint
#अन्यथा
/* Depending on whether statvfs or statfs is used,
   neither f_namemax or f_namelen may be available.  */
# define SB_F_NAMEMAX(S) "?"
# define OUT_NAMEMAX out_string
#पूर्ण_अगर

#अगर HAVE_STRUCT_STATVFS_F_BASETYPE
# define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME f_basetype
#अन्यथा
# अगर HAVE_STRUCT_STATVFS_F_FSTYPENAME || HAVE_STRUCT_STATFS_F_FSTYPENAME
#  define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME f_fstypename
# elif HAVE_OS_H /* BeOS */
#  define STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME fsh_name
# endif
#पूर्ण_अगर

#अगर HAVE_GETATTRAT
# include <attr.h>
# include <sys/nvpair.h>
#पूर्ण_अगर

अटल अक्षर स्थिर digits[] = "0123456789";

/* Flags that are portable for use in printf, for at least one
   conversion specifier; make_format removes non-portable flags as
   needed for particular specifiers.  The glibc 2.2 extension "I" is
   listed here; it is removed by make_format because it has undefined
   behavior elsewhere and because it is incompatible with
   out_epoch_sec.  */
अटल अक्षर स्थिर printf_flags[] = "'-+ #0I";

/* Formats for the --terse option.  */
अटल अक्षर स्थिर fmt_terse_fs[] = "%Qn %i %l %t %s %S %b %f %a %c %d\n";
अटल अक्षर स्थिर fmt_terse_regular[] = "%Qn %s %b %f %u %g %D %i %h %t %T"
                                        " %X %Y %Z %W %o\n";
अटल अक्षर स्थिर fmt_terse_selinux[] = "%Qn %s %b %f %u %g %D %i %h %t %T"
                                        " %X %Y %Z %W %o %C\n";

#घोषणा PROGRAM_NAME "stat"

#घोषणा AUTHORS proper_name ("Michael Meskes")

क्रमागत
{
  PRINTF_OPTION = अक्षर_उच्च + 1
};

क्रमागत cached_mode
{
  cached_default,
  cached_never,
  cached_always
};

अटल अक्षर स्थिर *स्थिर cached_args[] =
{
  "default", "never", "always", शून्य
};

अटल क्रमागत cached_mode स्थिर cached_modes[] =
{
  cached_default, cached_never, cached_always
};

अटल काष्ठा option स्थिर long_options[] =
{
  {"dereference", no_argument, शून्य, 'L'},
  {"file-system", no_argument, शून्य, 'f'},
  {"format", required_argument, शून्य, 'c'},
  {"printf", required_argument, शून्य, PRINTF_OPTION},
  {"terse", no_argument, शून्य, 't'},
  {"cached", required_argument, शून्य, 0},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {शून्य, 0, शून्य, 0}
};

/* Whether to follow symbolic links;  True for --dereference (-L).  */
अटल bool follow_links;

/* Whether to interpret backslash-escape sequences.
   True for --printf=FMT, not for --format=FMT (-c).  */
अटल bool interpret_backslash_escapes;

/* The trailing delimiter string:
   "" for --printf=FMT, "\n" for --format=FMT (-c).  */
अटल अक्षर स्थिर *trailing_delim = "";

/* The representation of the decimal point in the current locale.  */
अटल अक्षर स्थिर *decimal_point;
अटल माप_प्रकार decimal_point_len;

अटल bool
print_stat (अक्षर *pformat, माप_प्रकार prefix_len, अक्षर mod, अक्षर m,
            पूर्णांक fd, अक्षर स्थिर *filename, व्योम स्थिर *तथ्य);

/* Return the type of the specified file system.
   Some systems have statfvs.f_basetype[FSTYPSZ] (AIX, HP-UX, and Solaris).
   Others have statvfs.f_fstypename[_VFS_NAMELEN] (NetBSD 3.0).
   Others have statfs.f_fstypename[MFSNAMELEN] (NetBSD 1.5.2).
   Still others have neither and have to get by with f_type (GNU/Linux).
   But f_type may only exist in statfs (Cygwin).  */
NODISCARD
अटल अक्षर स्थिर *
human_fstype (STRUCT_STATVFS स्थिर *statfsbuf)
{
#अगर_घोषित STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME
  वापस statfsbuf->STATXFS_FILE_SYSTEM_TYPE_MEMBER_NAME;
#अन्यथा
  चयन (statfsbuf->f_type)
    {
# अगर defined __linux__ || defined __ANDROID__

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

    हाल S_MAGIC_AAFS: /* 0x5A3C69F0 local */
      वापस "aafs";
    हाल S_MAGIC_ACFS: /* 0x61636673 remote */
      वापस "acfs";
    हाल S_MAGIC_ADFS: /* 0xADF5 local */
      वापस "adfs";
    हाल S_MAGIC_AFFS: /* 0xADFF local */
      वापस "affs";
    हाल S_MAGIC_AFS: /* 0x5346414F remote */
      वापस "afs";
    हाल S_MAGIC_ANON_INODE_FS: /* 0x09041934 local */
      वापस "anon-inode FS";
    हाल S_MAGIC_AUFS: /* 0x61756673 remote */
      /* FIXME: change syntax or add an optional attribute like "inotify:no".
         The above is labeled as "remote" so that tail always uses polling,
         but this isn't really a remote file system type.  */
      वापस "aufs";
    हाल S_MAGIC_AUTOFS: /* 0x0187 local */
      वापस "autofs";
    हाल S_MAGIC_BALLOON_KVM: /* 0x13661366 local */
      वापस "balloon-kvm-fs";
    हाल S_MAGIC_BCACHEFS: /* 0xCA451A4E local */
      वापस "bcachefs";
    हाल S_MAGIC_BEFS: /* 0x42465331 local */
      वापस "befs";
    हाल S_MAGIC_BDEVFS: /* 0x62646576 local */
      वापस "bdevfs";
    हाल S_MAGIC_BFS: /* 0x1BADFACE local */
      वापस "bfs";
    हाल S_MAGIC_BINDERFS: /* 0x6C6F6F70 local */
      वापस "binderfs";
    हाल S_MAGIC_BPF_FS: /* 0xCAFE4A11 local */
      वापस "bpf_fs";
    हाल S_MAGIC_BINFMTFS: /* 0x42494E4D local */
      वापस "binfmt_misc";
    हाल S_MAGIC_BTRFS: /* 0x9123683E local */
      वापस "btrfs";
    हाल S_MAGIC_BTRFS_TEST: /* 0x73727279 local */
      वापस "btrfs_test";
    हाल S_MAGIC_CEPH: /* 0x00C36400 remote */
      वापस "ceph";
    हाल S_MAGIC_CGROUP: /* 0x0027E0EB local */
      वापस "cgroupfs";
    हाल S_MAGIC_CGROUP2: /* 0x63677270 local */
      वापस "cgroup2fs";
    हाल S_MAGIC_CIFS: /* 0xFF534D42 remote */
      वापस "cifs";
    हाल S_MAGIC_CODA: /* 0x73757245 remote */
      वापस "coda";
    हाल S_MAGIC_COH: /* 0x012FF7B7 local */
      वापस "coh";
    हाल S_MAGIC_CONFIGFS: /* 0x62656570 local */
      वापस "configfs";
    हाल S_MAGIC_CRAMFS: /* 0x28CD3D45 local */
      वापस "cramfs";
    हाल S_MAGIC_CRAMFS_WEND: /* 0x453DCD28 local */
      वापस "cramfs-wend";
    हाल S_MAGIC_DAXFS: /* 0x64646178 local */
      वापस "daxfs";
    हाल S_MAGIC_DEBUGFS: /* 0x64626720 local */
      वापस "debugfs";
    हाल S_MAGIC_DEVFS: /* 0x1373 local */
      वापस "devfs";
    हाल S_MAGIC_DEVMEM: /* 0x454D444D local */
      वापस "devmem";
    हाल S_MAGIC_DEVPTS: /* 0x1CD1 local */
      वापस "devpts";
    हाल S_MAGIC_DMA_BUF: /* 0x444D4142 local */
      वापस "dma-buf-fs";
    हाल S_MAGIC_ECRYPTFS: /* 0xF15F local */
      वापस "ecryptfs";
    हाल S_MAGIC_EFIVARFS: /* 0xDE5E81E4 local */
      वापस "efivarfs";
    हाल S_MAGIC_EFS: /* 0x00414A53 local */
      वापस "efs";
    हाल S_MAGIC_EROFS_V1: /* 0xE0F5E1E2 local */
      वापस "erofs";
    हाल S_MAGIC_EXFAT: /* 0x2011BAB0 local */
      वापस "exfat";
    हाल S_MAGIC_EXFS: /* 0x45584653 local */
      वापस "exfs";
    हाल S_MAGIC_EXOFS: /* 0x5DF5 local */
      वापस "exofs";
    हाल S_MAGIC_EXT: /* 0x137D local */
      वापस "ext";
    हाल S_MAGIC_EXT2: /* 0xEF53 local */
      वापस "ext2/ext3";
    हाल S_MAGIC_EXT2_OLD: /* 0xEF51 local */
      वापस "ext2";
    हाल S_MAGIC_F2FS: /* 0xF2F52010 local */
      वापस "f2fs";
    हाल S_MAGIC_FAIL_FS: /* 0x4641494C local */
      वापस "failfs";
    हाल S_MAGIC_FAT: /* 0x4006 local */
      वापस "fat";
    हाल S_MAGIC_FHGFS: /* 0x19830326 remote */
      वापस "fhgfs";
    हाल S_MAGIC_FUSE: /* 0x65735546 remote */
      वापस "fuse";
    हाल S_MAGIC_FUSECTL: /* 0x65735543 remote */
      वापस "fusectl";
    हाल S_MAGIC_FUTEXFS: /* 0x0BAD1DEA local */
      वापस "futexfs";
    हाल S_MAGIC_GFS: /* 0x01161970 remote */
      वापस "gfs/gfs2";
    हाल S_MAGIC_GPFS: /* 0x47504653 remote */
      वापस "gpfs";
    हाल S_MAGIC_GUEST_MEMFD: /* 0x474D454D remote */
      वापस "guest-memfd";
    हाल S_MAGIC_HFS: /* 0x4244 local */
      वापस "hfs";
    हाल S_MAGIC_HFS_PLUS: /* 0x482B local */
      वापस "hfs+";
    हाल S_MAGIC_HFS_X: /* 0x4858 local */
      वापस "hfsx";
    हाल S_MAGIC_HOSTFS: /* 0x00C0FFEE local */
      वापस "hostfs";
    हाल S_MAGIC_HPFS: /* 0xF995E849 local */
      वापस "hpfs";
    हाल S_MAGIC_HUGETLBFS: /* 0x958458F6 local */
      वापस "hugetlbfs";
    हाल S_MAGIC_MTD_INODE_FS: /* 0x11307854 local */
      वापस "inodefs";
    हाल S_MAGIC_IBRIX: /* 0x013111A8 remote */
      वापस "ibrix";
    हाल S_MAGIC_INOTIFYFS: /* 0x2BAD1DEA local */
      वापस "inotifyfs";
    हाल S_MAGIC_ISOFS: /* 0x9660 local */
      वापस "isofs";
    हाल S_MAGIC_ISOFS_R_WIN: /* 0x4004 local */
      वापस "isofs";
    हाल S_MAGIC_ISOFS_WIN: /* 0x4000 local */
      वापस "isofs";
    हाल S_MAGIC_JFFS: /* 0x07C0 local */
      वापस "jffs";
    हाल S_MAGIC_JFFS2: /* 0x72B6 local */
      वापस "jffs2";
    हाल S_MAGIC_JFS: /* 0x3153464A local */
      वापस "jfs";
    हाल S_MAGIC_KAFS: /* 0x6B414653 remote */
      वापस "k-afs";
    हाल S_MAGIC_LOGFS: /* 0xC97E8168 local */
      वापस "logfs";
    हाल S_MAGIC_LUSTRE: /* 0x0BD00BD0 remote */
      वापस "lustre";
    हाल S_MAGIC_M1FS: /* 0x5346314D local */
      वापस "m1fs";
    हाल S_MAGIC_MINIX: /* 0x137F local */
      वापस "minix";
    हाल S_MAGIC_MINIX_30: /* 0x138F local */
      वापस "minix (30 char.)";
    हाल S_MAGIC_MINIX_V2: /* 0x2468 local */
      वापस "minix v2";
    हाल S_MAGIC_MINIX_V2_30: /* 0x2478 local */
      वापस "minix v2 (30 char.)";
    हाल S_MAGIC_MINIX_V3: /* 0x4D5A local */
      वापस "minix3";
    हाल S_MAGIC_MQUEUE: /* 0x19800202 local */
      वापस "mqueue";
    हाल S_MAGIC_MSDOS: /* 0x4D44 local */
      वापस "msdos";
    हाल S_MAGIC_NCP: /* 0x564C remote */
      वापस "novell";
    हाल S_MAGIC_NFS: /* 0x6969 remote */
      वापस "nfs";
    हाल S_MAGIC_NFSD: /* 0x6E667364 remote */
      वापस "nfsd";
    हाल S_MAGIC_NILFS: /* 0x3434 local */
      वापस "nilfs";
    हाल S_MAGIC_NSFS: /* 0x6E736673 local */
      वापस "nsfs";
    हाल S_MAGIC_NTFS: /* 0x5346544E local */
      वापस "ntfs";
    हाल S_MAGIC_NULL_FS: /* 0x4E554C4C local */
      वापस "nullfs";
    हाल S_MAGIC_OPENPROM: /* 0x9FA1 local */
      वापस "openprom";
    हाल S_MAGIC_OCFS2: /* 0x7461636F remote */
      वापस "ocfs2";
    हाल S_MAGIC_OVERLAYFS: /* 0x794C7630 remote */
      /* This may overlay remote file systems.
         Also there have been issues reported with inotify and overlayfs,
         so mark as "remote" so that polling is used.  */
      वापस "overlayfs";
    हाल S_MAGIC_PANFS: /* 0xAAD7AAEA remote */
      वापस "panfs";
    हाल S_MAGIC_PID_FS: /* 0x50494446 local */
      वापस "pidfs";
    हाल S_MAGIC_PIPEFS: /* 0x50495045 remote */
      /* FIXME: change syntax or add an optional attribute like "inotify:no".
         pipefs is labeled as "remote" so that tail always polls,
         but it isn't really remote file system type.  */
      वापस "pipefs";
    हाल S_MAGIC_PPC_CMM: /* 0xC7571590 local */
      वापस "ppc-cmm-fs";
    हाल S_MAGIC_PRL_FS: /* 0x7C7C6673 remote */
      वापस "prl_fs";
    हाल S_MAGIC_PROC: /* 0x9FA0 local */
      वापस "proc";
    हाल S_MAGIC_PSTOREFS: /* 0x6165676C local */
      वापस "pstorefs";
    हाल S_MAGIC_QNX4: /* 0x002F local */
      वापस "qnx4";
    हाल S_MAGIC_QNX6: /* 0x68191122 local */
      वापस "qnx6";
    हाल S_MAGIC_RAMFS: /* 0x858458F6 local */
      वापस "ramfs";
    हाल S_MAGIC_RDTGROUP: /* 0x07655821 local */
      वापस "rdt";
    हाल S_MAGIC_REISERFS: /* 0x52654973 local */
      वापस "reiserfs";
    हाल S_MAGIC_ROMFS: /* 0x7275 local */
      वापस "romfs";
    हाल S_MAGIC_RPC_PIPEFS: /* 0x67596969 local */
      वापस "rpc_pipefs";
    हाल S_MAGIC_SDCARDFS: /* 0x5DCA2DF5 local */
      वापस "sdcardfs";
    हाल S_MAGIC_SECRETMEM: /* 0x5345434D local */
      वापस "secretmem";
    हाल S_MAGIC_SECURITYFS: /* 0x73636673 local */
      वापस "securityfs";
    हाल S_MAGIC_SELINUX: /* 0xF97CFF8C local */
      वापस "selinux";
    हाल S_MAGIC_SMACK: /* 0x43415D53 local */
      वापस "smackfs";
    हाल S_MAGIC_SMB: /* 0x517B remote */
      वापस "smb";
    हाल S_MAGIC_SMB2: /* 0xFE534D42 remote */
      वापस "smb2";
    हाल S_MAGIC_SNFS: /* 0xBEEFDEAD remote */
      वापस "snfs";
    हाल S_MAGIC_SOCKFS: /* 0x534F434B local */
      वापस "sockfs";
    हाल S_MAGIC_SQUASHFS: /* 0x73717368 local */
      वापस "squashfs";
    हाल S_MAGIC_SYSFS: /* 0x62656572 local */
      वापस "sysfs";
    हाल S_MAGIC_SYSV2: /* 0x012FF7B6 local */
      वापस "sysv2";
    हाल S_MAGIC_SYSV4: /* 0x012FF7B5 local */
      वापस "sysv4";
    हाल S_MAGIC_TMPFS: /* 0x01021994 local */
      वापस "tmpfs";
    हाल S_MAGIC_TRACEFS: /* 0x74726163 local */
      वापस "tracefs";
    हाल S_MAGIC_UBIFS: /* 0x24051905 local */
      वापस "ubifs";
    हाल S_MAGIC_UDF: /* 0x15013346 local */
      वापस "udf";
    हाल S_MAGIC_UFS: /* 0x00011954 local */
      वापस "ufs";
    हाल S_MAGIC_UFS_BYTESWAPPED: /* 0x54190100 local */
      वापस "ufs";
    हाल S_MAGIC_USBDEVFS: /* 0x9FA2 local */
      वापस "usbdevfs";
    हाल S_MAGIC_V9FS: /* 0x01021997 local */
      वापस "v9fs";
    हाल S_MAGIC_VBOXSF: /* 0x786F4256 remote */
      वापस "vboxsf";
    हाल S_MAGIC_VMHGFS: /* 0xBACBACBC remote */
      वापस "vmhgfs";
    हाल S_MAGIC_VXFS: /* 0xA501FCF5 remote */
      /* Veritas File System can run in single instance or clustered mode,
         so mark as remote to cater for the latter case.  */
      वापस "vxfs";
    हाल S_MAGIC_VZFS: /* 0x565A4653 local */
      वापस "vzfs";
    हाल S_MAGIC_WSLFS: /* 0x53464846 local */
      वापस "wslfs";
    हाल S_MAGIC_XENFS: /* 0xABBA1974 local */
      वापस "xenfs";
    हाल S_MAGIC_XENIX: /* 0x012FF7B4 local */
      वापस "xenix";
    हाल S_MAGIC_XFS: /* 0x58465342 local */
      वापस "xfs";
    हाल S_MAGIC_XIAFS: /* 0x012FD16D local */
      वापस "xia";
    हाल S_MAGIC_Z3FOLD: /* 0x0033 local */
      वापस "z3fold";
    हाल S_MAGIC_ZFS: /* 0x2FC12FC1 local */
      वापस "zfs";
    हाल S_MAGIC_ZONEFS: /* 0x5A4F4653 local */
      वापस "zonefs";
    हाल S_MAGIC_ZSMALLOC: /* 0x58295829 local */
      वापस "zsmallocfs";


# elif __GNU__
    हाल FSTYPE_UFS:
      वापस "ufs";
    हाल FSTYPE_NFS:
      वापस "nfs";
    हाल FSTYPE_GFS:
      वापस "gfs";
    हाल FSTYPE_LFS:
      वापस "lfs";
    हाल FSTYPE_SYSV:
      वापस "sysv";
    हाल FSTYPE_FTP:
      वापस "ftp";
    हाल FSTYPE_TAR:
      वापस "tar";
    हाल FSTYPE_AR:
      वापस "ar";
    हाल FSTYPE_CPIO:
      वापस "cpio";
    हाल FSTYPE_MSLOSS:
      वापस "msloss";
    हाल FSTYPE_CPM:
      वापस "cpm";
    हाल FSTYPE_HFS:
      वापस "hfs";
    हाल FSTYPE_DTFS:
      वापस "dtfs";
    हाल FSTYPE_GRFS:
      वापस "grfs";
    हाल FSTYPE_TERM:
      वापस "term";
    हाल FSTYPE_DEV:
      वापस "dev";
    हाल FSTYPE_PROC:
      वापस "proc";
    हाल FSTYPE_IFSOCK:
      वापस "ifsock";
    हाल FSTYPE_AFS:
      वापस "afs";
    हाल FSTYPE_DFS:
      वापस "dfs";
    हाल FSTYPE_PROC9:
      वापस "proc9";
    हाल FSTYPE_SOCKET:
      वापस "socket";
    हाल FSTYPE_MISC:
      वापस "misc";
    हाल FSTYPE_EXT2FS:
      वापस "ext2/ext3";
    हाल FSTYPE_HTTP:
      वापस "http";
    हाल FSTYPE_MEMFS:
      वापस "memfs";
    हाल FSTYPE_ISO9660:
      वापस "iso9660";
# endif
    शेष:
      {
        अचिन्हित दीर्घ पूर्णांक type = statfsbuf->f_type;
        अटल अक्षर buf[माप "UNKNOWN (0x%lx)" - 3
                        + (माप type * अक्षर_बिट + 3) / 4];
        प्र_लिखो (buf, "UNKNOWN (0x%lx)", type);
        वापस buf;
      }
    }
#पूर्ण_अगर
}

NODISCARD
अटल अक्षर *
human_access (काष्ठा stat स्थिर *statbuf)
{
  अटल अक्षर modebuf[12];
  filemodestring (statbuf, modebuf);
  modebuf[10] = 0;
  वापस modebuf;
}

NODISCARD
अटल अक्षर *
human_time (काष्ठा timespec t)
{
  /* STR must be at least INT_BUFSIZE_BOUND (intmax_t) big, either
     because localtime_rz fails, or because the time zone is truly
     outlandish so that %z expands to a long string.  */
  अटल अक्षर str[INT_BUFSIZE_BOUND (intmax_t)
                  + INT_STRLEN_BOUND (पूर्णांक) /* YYYY */
                  + 1 /* because YYYY might equal INT_MAX + 1900 */
                  + माप "-MM-DD HH:MM:SS.NNNNNNNNN +"];
  अटल timezone_t tz;
  अगर (!tz)
    tz = tzalloc (दो_पर्या ("TZ"));
  काष्ठा पंचांग पंचांग;
  पूर्णांक ns = t.tv_nsec;
  अगर (localtime_rz (tz, &t.tv_sec, &पंचांग))
    nstrftime (str, माप str, "%Y-%m-%d %H:%M:%S.%N %z", &पंचांग, tz, ns);
  अन्यथा
    {
      अक्षर secbuf[INT_BUFSIZE_BOUND (intmax_t)];
      प्र_लिखो (str, "%s.%09d", timetostr (t.tv_sec, secbuf), ns);
    }
  वापस str;
}

/* PFORMAT points to a '%' followed by a prefix of a format, all of
   size PREFIX_LEN.  The flags allowed for this format are
   ALLOWED_FLAGS; remove other printf flags from the prefix, then
   append SUFFIX.  */
अटल व्योम
make_format (अक्षर *pformat, माप_प्रकार prefix_len, अक्षर स्थिर *allowed_flags,
             अक्षर स्थिर *suffix)
{
  अक्षर *dst = pformat + 1;
  अक्षर स्थिर *src;
  अक्षर स्थिर *srclim = pformat + prefix_len;
  क्रम (src = dst; src < srclim && म_अक्षर (printf_flags, *src); src++)
    अगर (म_अक्षर (allowed_flags, *src))
      *dst++ = *src;
  जबतक (src < srclim)
    *dst++ = *src++;
  म_नकल (dst, suffix);
}

अटल व्योम
out_string (अक्षर *pformat, माप_प्रकार prefix_len, अक्षर स्थिर *arg)
{
  make_format (pformat, prefix_len, "-", "s");
  म_लिखो (pformat, arg);
}
अटल पूर्णांक
out_int (अक्षर *pformat, माप_प्रकार prefix_len, intmax_t arg)
{
  make_format (pformat, prefix_len, "'-+ 0", "jd");
  वापस म_लिखो (pformat, arg);
}
अटल पूर्णांक
out_uint (अक्षर *pformat, माप_प्रकार prefix_len, uintmax_t arg)
{
  make_format (pformat, prefix_len, "'-0", "ju");
  वापस म_लिखो (pformat, arg);
}
अटल व्योम
out_uint_o (अक्षर *pformat, माप_प्रकार prefix_len, uintmax_t arg)
{
  make_format (pformat, prefix_len, "-#0", "jo");
  म_लिखो (pformat, arg);
}
अटल व्योम
out_uint_x (अक्षर *pformat, माप_प्रकार prefix_len, uintmax_t arg)
{
  make_format (pformat, prefix_len, "-#0", "jx");
  म_लिखो (pformat, arg);
}
अटल पूर्णांक
out_minus_zero (अक्षर *pformat, माप_प्रकार prefix_len)
{
  make_format (pformat, prefix_len, "'-+ 0", ".0f");
  वापस म_लिखो (pformat, -0.25);
}

/* Output the number of seconds since the Epoch, using a format that
   acts like printf's %f format.  */
अटल व्योम
out_epoch_sec (अक्षर *pformat, माप_प्रकार prefix_len,
               काष्ठा timespec arg)
{
  अक्षर *dot = स_प्रथम (pformat, '.', prefix_len);
  माप_प्रकार sec_prefix_len = prefix_len;
  पूर्णांक width = 0;
  पूर्णांक precision = 0;
  bool frac_बायाँ_adjust = false;

  अगर (dot)
    {
      sec_prefix_len = dot - pformat;
      pformat[prefix_len] = '\0';

      अगर (c_isdigit (dot[1]))
        {
          दीर्घ पूर्णांक lprec = म_से_दीर्घ (dot + 1, शून्य, 10);
          precision = (lprec <= पूर्णांक_उच्च ? lprec : पूर्णांक_उच्च);
        }
      अन्यथा
        {
          precision = 9;
        }

      अगर (precision && c_isdigit (dot[-1]))
        {
          /* If a nontrivial width is given, subtract the width of the
             decimal point and PRECISION digits that will be output
             later.  */
          अक्षर *p = dot;
          *dot = '\0';

          करो
            --p;
          जबतक (c_isdigit (p[-1]));

          दीर्घ पूर्णांक lwidth = म_से_दीर्घ (p, शून्य, 10);
          width = (lwidth <= पूर्णांक_उच्च ? lwidth : पूर्णांक_उच्च);
          अगर (1 < width)
            {
              p += (*p == '0');
              sec_prefix_len = p - pformat;
              पूर्णांक w_d = (decimal_point_len < width
                         ? width - decimal_point_len
                         : 0);
              अगर (1 < w_d)
                {
                  पूर्णांक w = w_d - precision;
                  अगर (1 < w)
                    {
                      अक्षर *dst = pformat;
                      क्रम (अक्षर स्थिर *src = dst; src < p; src++)
                        {
                          अगर (*src == '-')
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

  पूर्णांक भाजक = 1;
  क्रम (पूर्णांक i = precision; i < 9; i++)
    भाजक *= 10;
  पूर्णांक frac_sec = arg.tv_nsec / भाजक;
  पूर्णांक int_len;

  अगर (TYPE_SIGNED (समय_प्रकार))
    {
      bool minus_zero = false;
      अगर (arg.tv_sec < 0 && arg.tv_nsec != 0)
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

  अगर (precision)
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
अटल bool
out_file_context (अक्षर *pformat, माप_प्रकार prefix_len, अक्षर स्थिर *filename)
{
  अक्षर *scontext;
  bool fail = false;

  अगर ((follow_links
       ? getfilecon (filename, &scontext)
       : lgetfilecon (filename, &scontext)) < 0)
    {
      error (0, त्रुटि_सं, _("failed to get security context of %s"),
             quoteaf (filename));
      scontext = शून्य;
      fail = true;
    }
  म_नकल (pformat + prefix_len, "s");
  म_लिखो (pformat, (scontext ? scontext : "?"));
  अगर (scontext)
    freecon (scontext);
  वापस fail;
}

/* Set the quoting style once it is needed.  */
अटल व्योम
initialize_quoting_style (व्योम)
{
  अटल bool initialized;
  अगर (!initialized)
    {
      initialized = true;
      पूर्णांक qs = getenv_quoting_style ();
      set_quoting_style (शून्य, 0 <= qs ? qs : shell_escape_quoting_style);
    }
}

/* Print statfs info.  Return zero upon success, nonzero upon failure.  */
NODISCARD
अटल bool
print_statfs (अक्षर *pformat, माप_प्रकार prefix_len, MAYBE_UNUSED अक्षर mod, अक्षर m,
              MAYBE_UNUSED पूर्णांक fd, अक्षर स्थिर *filename,
              व्योम स्थिर *तथ्य)
{
  STRUCT_STATVFS स्थिर *statfsbuf = तथ्य;
  bool fail = false;

  चयन (m)
    {
    हाल 'n':
      अगर (mod == 'Q')
        {
          initialize_quoting_style ();
          filename = quoteN (filename);
        }
      out_string (pformat, prefix_len, filename);
      अवरोध;

    हाल 'i':
      {
#अगर STRUCT_STATXFS_F_FSID_IS_INTEGER
        uintmax_t fsid = statfsbuf->f_fsid;
#अन्यथा
        प्रकार अचिन्हित पूर्णांक fsid_word;
        static_assert (alignof (STRUCT_STATVFS) % alignof (fsid_word) == 0);
        static_assert (दुरत्व (STRUCT_STATVFS, f_fsid) % alignof (fsid_word)
                       == 0);
        static_assert (माप statfsbuf->f_fsid % alignof (fsid_word) == 0);
        fsid_word स्थिर *p = (fsid_word *) &statfsbuf->f_fsid;

        /* Assume a little-endian word order, as that is compatible
           with glibc's statvfs implementation.  */
        uintmax_t fsid = 0;
        पूर्णांक शब्दगण = माप statfsbuf->f_fsid / माप *p;
        क्रम (पूर्णांक i = 0; i < शब्दगण && i * माप *p < माप fsid; i++)
          {
            uintmax_t u = p[शब्दगण - 1 - i];
            fsid |= u << (i * अक्षर_बिट * माप *p);
          }
#पूर्ण_अगर
        out_uint_x (pformat, prefix_len, fsid);
      }
      अवरोध;

    हाल 'l':
      OUT_NAMEMAX (pformat, prefix_len, SB_F_NAMEMAX (statfsbuf));
      अवरोध;
    हाल 't':
#अगर HAVE_STRUCT_STATXFS_F_TYPE
      out_uint_x (pformat, prefix_len, statfsbuf->f_type);
#अन्यथा
      ख_अक्षर_दो ('?', मानक_निकास);
#पूर्ण_अगर
      अवरोध;
    हाल 'T':
      out_string (pformat, prefix_len, human_fstype (statfsbuf));
      अवरोध;
    हाल 'b':
      out_int (pformat, prefix_len, statfsbuf->f_blocks);
      अवरोध;
    हाल 'f':
      out_int (pformat, prefix_len, statfsbuf->f_bfree);
      अवरोध;
    हाल 'a':
      out_int (pformat, prefix_len, statfsbuf->f_bavail);
      अवरोध;
    हाल 's':
      out_uint (pformat, prefix_len, statfsbuf->f_bsize);
      अवरोध;
    हाल 'S':
      {
        uintmax_t frsize = STATFS_FRSIZE (statfsbuf);
        अगर (! frsize)
          frsize = statfsbuf->f_bsize;
        out_uint (pformat, prefix_len, frsize);
      }
      अवरोध;
    हाल 'c':
      out_uint (pformat, prefix_len, statfsbuf->f_files);
      अवरोध;
    हाल 'd':
      out_int (pformat, prefix_len, statfsbuf->f_ffree);
      अवरोध;
    शेष:
      ख_अक्षर_दो ('?', मानक_निकास);
      अवरोध;
    }
  वापस fail;
}

/* Return any bind mounted source for a path.
   The caller should not free the returned buffer.
   Return NULL if no bind mount found.  */
NODISCARD
अटल अक्षर स्थिर *
find_bind_mount (अक्षर स्थिर * name)
{
  अक्षर स्थिर * bind_mount = शून्य;

  अटल काष्ठा mount_entry *mount_list;
  अटल bool tried_mount_list = false;
  अगर (!tried_mount_list) /* attempt/warn once per process.  */
    {
      अगर (!(mount_list = read_file_system_list (false)))
        error (0, त्रुटि_सं, "%s", _("cannot read table of mounted file systems"));
      tried_mount_list = true;
    }

  काष्ठा stat name_stats;
  अगर (stat (name, &name_stats) != 0)
    वापस शून्य;

  क्रम (काष्ठा mount_entry *me = mount_list; me; me = me->me_अग्र)
    {
      अगर (me->me_dummy && me->me_devname[0] == '/'
          && streq (me->me_mountdir, name))
        {
          काष्ठा stat dev_stats;

          अगर (stat (me->me_devname, &dev_stats) == 0
              && psame_inode (&name_stats, &dev_stats))
            {
              bind_mount = me->me_devname;
              अवरोध;
            }
        }
    }

  वापस bind_mount;
}

/* Print mount point.  Return zero upon success, nonzero upon failure.  */
NODISCARD
अटल bool
out_mount_point (अक्षर स्थिर *filename, अक्षर *pformat, माप_प्रकार prefix_len,
                 स्थिर काष्ठा stat *statp)
{

  अक्षर स्थिर *np = "?", *bp = शून्य;
  अक्षर *mp = शून्य;
  bool fail = true;

  /* Look for bind mounts first.  Note we output the immediate alias,
     rather than further resolving to a base device mount point.  */
  अगर (follow_links || !S_ISLNK (statp->st_mode))
    {
      अक्षर *resolved = canonicalize_file_name (filename);
      अगर (!resolved)
        {
          error (0, त्रुटि_सं, _("failed to canonicalize %s"), quoteaf (filename));
          जाओ print_mount_point;
        }
      bp = find_bind_mount (resolved);
      मुक्त (resolved);
      अगर (bp)
        {
          fail = false;
          जाओ print_mount_point;
        }
    }

  /* If there is no direct bind mount, then navigate
     back up the tree looking for a device change.
     Note we don't detect if any of the directory components
     are bind mounted to the same device, but that's OK
     since we've not directly queried them.  */
  अगर ((mp = find_mount_point (filename, statp)))
    {
      /* This dir might be bind mounted to another device,
         so we resolve the bound source in that case also.  */
      bp = find_bind_mount (mp);
      fail = false;
    }

print_mount_point:

  out_string (pformat, prefix_len, bp ? bp : mp ? mp : np);
  मुक्त (mp);
  वापस fail;
}

/* Map a TS with negative TS.tv_nsec to {0,0}.  */
अटल अंतरभूत काष्ठा timespec
neg_to_zero (काष्ठा timespec ts)
{
  अगर (0 <= ts.tv_nsec)
    वापस ts;
  काष्ठा timespec z = {0};
  वापस z;
}

/* Output a single-character \ escape.  */

अटल व्योम
print_esc_char (अक्षर c)
{
  चयन (c)
    {
    हाल 'a':			/* Alert. */
      c ='\a';
      अवरोध;
    हाल 'b':			/* Backspace. */
      c ='\b';
      अवरोध;
    हाल 'e':			/* Escape. */
      c ='\x1B';
      अवरोध;
    हाल 'f':			/* Form feed. */
      c ='\f';
      अवरोध;
    हाल 'n':			/* New line. */
      c ='\n';
      अवरोध;
    हाल 'r':			/* Carriage return. */
      c ='\r';
      अवरोध;
    हाल 't':			/* Horizontal tab. */
      c ='\t';
      अवरोध;
    हाल 'v':			/* Vertical tab. */
      c ='\v';
      अवरोध;
    हाल '"':
    हाल '\\':
      अवरोध;
    शेष:
      error (0, 0, _("warning: unrecognized escape '\\%c'"), c);
      अवरोध;
    }
  अक्षर_दो (c);
}

ATTRIBUTE_PURE
अटल माप_प्रकार
format_code_offset (अक्षर स्थिर *directive)
{
  माप_प्रकार len = म_अखोज (directive + 1, printf_flags);
  अक्षर स्थिर *fmt_char = directive + len + 1;
  fmt_char += म_अखोज (fmt_char, digits);
  अगर (*fmt_char == '.')
    fmt_char += 1 + म_अखोज (fmt_char + 1, digits);
  वापस fmt_char - directive;
}

/* Print the information specified by the format string, FORMAT,
   calling PRINT_FUNC for each %-directive encountered.
   Return zero upon success, nonzero upon failure.  */
NODISCARD
अटल bool
print_it (अक्षर स्थिर *format, पूर्णांक fd, अक्षर स्थिर *filename,
          bool (*print_func) (अक्षर *, माप_प्रकार, अक्षर, अक्षर,
                              पूर्णांक, अक्षर स्थिर *, व्योम स्थिर *),
          व्योम स्थिर *तथ्य)
{
  bool fail = false;

  /* Add 2 to accommodate our conversion of the stat '%s' format string
     to the longer printf '%llu' one.  */
  क्रमागत
    {
      MAX_ADDITIONAL_BYTES =
        (MAX (माप "jd",
              MAX (माप "jo", MAX (माप "ju", माप "jx")))
         - 1)
    };
  माप_प्रकार n_alloc = म_माप (format) + MAX_ADDITIONAL_BYTES + 1;
  अक्षर *dest = xmalloc (n_alloc);
  क्रम (अक्षर स्थिर *b = format; *b; b++)
    {
      चयन (*b)
        {
        हाल '%':
          {
            माप_प्रकार len = format_code_offset (b);
            अक्षर fmt_char = *(b + len);
            अक्षर mod_char = 0;
            स_नकल (dest, b, len);
            b += len;

            चयन (fmt_char)
              {
              हाल '\0':
                --b;
                FALLTHROUGH;
              हाल '%':
                अगर (1 < len)
                  {
                    dest[len] = fmt_char;
                    dest[len + 1] = '\0';
                    error (निकास_त्रुटि, 0, _("%s: invalid directive"),
                           quote (dest));
                  }
                अक्षर_दो ('%');
                अवरोध;
              हाल 'H':
              हाल 'L':
              हाल 'Q':
                mod_char = fmt_char;
                fmt_char = *(b + 1);
                अगर ((mod_char == 'Q' && fmt_char == 'n')
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
              शेष:
                fail |= print_func (dest, len, mod_char, fmt_char,
                                    fd, filename, तथ्य);
                अवरोध;
              }
            अवरोध;
          }

        हाल '\\':
          अगर ( ! interpret_backslash_escapes)
            {
              अक्षर_दो ('\\');
              अवरोध;
            }
          ++b;
          अगर (isoct (*b))
            {
              पूर्णांक esc_मूल्य = fromoct (*b);
              पूर्णांक esc_लंबाई = 1;	/* number of octal digits */
              क्रम (++b; esc_लंबाई < 3 && isoct (*b);
                   ++esc_लंबाई, ++b)
                {
                  esc_मूल्य = esc_मूल्य * 8 + fromoct (*b);
                }
              अक्षर_दो (esc_मूल्य);
              --b;
            }
          अन्यथा अगर (*b == 'x' && c_isxdigit (b[1]))
            {
              पूर्णांक esc_मूल्य = fromhex (b[1]);	/* Value of \xhh escape. */
              /* A hexadecimal \xhh escape sequence must have
                 1 or 2 hex. digits.  */
              ++b;
              अगर (c_isxdigit (b[1]))
                {
                  ++b;
                  esc_मूल्य = esc_मूल्य * 16 + fromhex (*b);
                }
              अक्षर_दो (esc_मूल्य);
            }
          अन्यथा अगर (*b == '\0')
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
          अवरोध;

        शेष:
          अक्षर_दो (*b);
          अवरोध;
        }
    }
  मुक्त (dest);

  ख_माला_दो (trailing_delim, मानक_निकास);

  वापस fail;
}

/* Stat the file system and print what we find.  */
NODISCARD
अटल bool
do_statfs (अक्षर स्थिर *filename, अक्षर स्थिर *format)
{
  STRUCT_STATVFS statfsbuf;

  अगर (streq (filename, "-"))
    {
      error (0, 0, _("using %s to denote standard input does not work"
                     " in file system mode"), quoteaf (filename));
      वापस false;
    }

  अगर (STATFS (filename, &statfsbuf) != 0)
    {
      error (0, त्रुटि_सं, _("cannot read file system information for %s"),
             quoteaf (filename));
      वापस false;
    }

  bool fail = print_it (format, -1, filename, print_statfs, &statfsbuf);
  वापस ! fail;
}

काष्ठा print_args {
  काष्ठा stat *st;
  काष्ठा timespec btime;
};

/* Ask statx to avoid syncing? */
अटल bool dont_sync;

/* Ask statx to force sync? */
अटल bool force_sync;

#अगर USE_STATX
अटल अचिन्हित पूर्णांक
fmt_to_mask (अक्षर fmt)
{
  चयन (fmt)
    {
    हाल 'N':
      वापस STATX_MODE;
    हाल 'd':
    हाल 'D':
      वापस STATX_MODE;
    हाल 'i':
      वापस STATX_INO;
    हाल 'a':
    हाल 'A':
      वापस STATX_MODE;
    हाल 'f':
      वापस STATX_MODE|STATX_TYPE;
    हाल 'F':
      वापस STATX_TYPE;
    हाल 'h':
      वापस STATX_NLINK;
    हाल 'u':
    हाल 'U':
      वापस STATX_UID;
    हाल 'g':
    हाल 'G':
      वापस STATX_GID;
    हाल 'm':
      वापस STATX_MODE|STATX_INO;
    हाल 's':
      वापस STATX_आकार;
    हाल 't':
    हाल 'T':
      वापस STATX_MODE;
    हाल 'b':
      वापस STATX_BLOCKS;
    हाल 'w':
    हाल 'W':
      वापस STATX_BTIME;
    हाल 'x':
    हाल 'X':
      वापस STATX_ATIME;
    हाल 'y':
    हाल 'Y':
      वापस STATX_MTIME;
    हाल 'z':
    हाल 'Z':
      वापस STATX_CTIME;
    }
  वापस 0;
}

ATTRIBUTE_PURE
अटल अचिन्हित पूर्णांक
format_to_mask (अक्षर स्थिर *format)
{
  अचिन्हित पूर्णांक mask = 0;

  क्रम (अक्षर स्थिर *b = format; *b; b++)
    {
      अगर (*b != '%')
        जारी;

      b += format_code_offset (b);
      अगर (*b == '\0')
        अवरोध;
      mask |= fmt_to_mask (*b);
    }
  वापस mask;
}

/* statx the file and print what we find */
NODISCARD
अटल bool
do_stat (अक्षर स्थिर *filename, अक्षर स्थिर *format, अक्षर स्थिर *format2)
{
  पूर्णांक fd = streq (filename, "-") ? 0 : AT_FDCWD;
  पूर्णांक flags = 0;
  काष्ठा stat st;
  काष्ठा statx stx = {0};
  अक्षर स्थिर *pathname = filename;
  काष्ठा print_args pa;
  pa.st = &st;
  pa.btime = (काष्ठा timespec) {.tv_sec = -1, .tv_nsec = -1};

  अगर (AT_FDCWD != fd)
    {
      pathname = "";
      flags = AT_EMPTY_PATH;
    }
  अन्यथा अगर (!follow_links)
    {
      flags = AT_SYMLINK_NOFOLLOW;
    }

  अगर (dont_sync)
    flags |= AT_STATX_DONT_SYNC;
  अन्यथा अगर (force_sync)
    flags |= AT_STATX_FORCE_SYNC;

  अगर (! force_sync)
    flags |= AT_NO_AUTOMOUNT;

  fd = statx (fd, pathname, flags, format_to_mask (format), &stx);
  अगर (fd < 0)
    {
      अगर (flags & AT_EMPTY_PATH)
        error (0, त्रुटि_सं, _("cannot stat standard input"));
      अन्यथा
        error (0, त्रुटि_सं, _("cannot statx %s"), quoteaf (filename));
      वापस false;
    }

  अगर (S_ISBLK (stx.stx_mode) || S_ISCHR (stx.stx_mode))
    format = format2;

  statx_to_stat (&stx, &st);
  अगर (stx.stx_mask & STATX_BTIME)
    pa.btime = statx_timestamp_to_timespec (stx.stx_btime);

  bool fail = print_it (format, fd, filename, print_stat, &pa);
  वापस ! fail;
}

#अन्यथा /* USE_STATX */

अटल काष्ठा timespec
get_birthtime (पूर्णांक fd, अक्षर स्थिर *filename, काष्ठा stat स्थिर *st)
{
  काष्ठा timespec ts = get_stat_birthtime (st);

# अगर HAVE_GETATTRAT
  अगर (ts.tv_nsec < 0)
    {
      nvlist_t *response;
      अगर ((fd < 0
           ? getattrat (AT_FDCWD, XATTR_VIEW_READWRITE, filename, &response)
           : fgetattr (fd, XATTR_VIEW_READWRITE, &response))
          == 0)
        {
          uint64_t *val;
          uint_t n;
          अगर (nvlist_lookup_uint64_array (response, A_CRTIME, &val, &n) == 0
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

  वापस ts;
}


/* stat the file and print what we find */
NODISCARD
अटल bool
do_stat (अक्षर स्थिर *filename, अक्षर स्थिर *format,
         अक्षर स्थिर *format2)
{
  पूर्णांक fd = streq (filename, "-") ? 0 : -1;
  काष्ठा stat statbuf;
  काष्ठा print_args pa;
  pa.st = &statbuf;
  pa.btime = (काष्ठा timespec) {.tv_sec = -1, .tv_nsec = -1};

  अगर (0 <= fd)
    {
      अगर (ख_स्थिति (fd, &statbuf) != 0)
        {
          error (0, त्रुटि_सं, _("cannot stat standard input"));
          वापस false;
        }
    }
  /* We can't use the shorter
     (follow_links?stat:lstat) (filename, &statbug)
     since stat might be a function-like macro.  */
  अन्यथा अगर ((follow_links
            ? stat (filename, &statbuf)
            : lstat (filename, &statbuf)) != 0)
    {
      error (0, त्रुटि_सं, _("cannot stat %s"), quoteaf (filename));
      वापस false;
    }

  अगर (S_ISBLK (statbuf.st_mode) || S_ISCHR (statbuf.st_mode))
    format = format2;

  bool fail = print_it (format, fd, filename, print_stat, &pa);
  वापस ! fail;
}
#पूर्ण_अगर /* USE_STATX */

/* POSIX requires 'ls' to print file sizes without a sign, even
   when negative.  Be consistent with that.  */

अटल uintmax_t
unsigned_file_आकार (off_t आकार)
{
  वापस आकार + (आकार < 0) * ((uintmax_t) OFF_T_MAX - OFF_T_MIN + 1);
}

/* Print stat info.  Return zero upon success, nonzero upon failure.  */
अटल bool
print_stat (अक्षर *pformat, माप_प्रकार prefix_len, अक्षर mod, अक्षर m,
            MAYBE_UNUSED पूर्णांक fd, अक्षर स्थिर *filename, व्योम स्थिर *तथ्य)
{
  काष्ठा print_args *parg = (काष्ठा print_args *) तथ्य;
  काष्ठा stat *statbuf = parg->st;
  काष्ठा timespec btime = parg->btime;
  काष्ठा passwd *pw_ent;
  काष्ठा group *gw_ent;
  bool fail = false;

  चयन (m)
    {
    हाल 'n':
      अगर (mod == 'Q')
        {
          initialize_quoting_style ();
          filename = quoteN (filename);
        }
      out_string (pformat, prefix_len, filename);
      अवरोध;
    हाल 'N':
      initialize_quoting_style ();
      out_string (pformat, prefix_len, quoteN (filename));
      अगर (S_ISLNK (statbuf->st_mode))
        {
          अक्षर *linkname = areadlink_with_आकार (filename, statbuf->st_आकार);
          अगर (linkname == शून्य)
            {
              error (0, त्रुटि_सं, _("cannot read symbolic link %s"),
                     quoteaf (filename));
              वापस true;
            }
          म_लिखो (" -> ");
          out_string (pformat, prefix_len, quoteN (linkname));
          मुक्त (linkname);
        }
      अवरोध;
    हाल 'd':
      अगर (mod == 'H')
        out_uint (pformat, prefix_len, major (statbuf->st_dev));
      अन्यथा अगर (mod == 'L')
        out_uint (pformat, prefix_len, minor (statbuf->st_dev));
      अन्यथा
        out_uint (pformat, prefix_len, statbuf->st_dev);
      अवरोध;
    हाल 'D':
      out_uint_x (pformat, prefix_len, statbuf->st_dev);
      अवरोध;
    हाल 'i':
      out_uint (pformat, prefix_len, statbuf->st_ino);
      अवरोध;
    हाल 'a':
      out_uint_o (pformat, prefix_len, statbuf->st_mode & CHMOD_MODE_BITS);
      अवरोध;
    हाल 'A':
      out_string (pformat, prefix_len, human_access (statbuf));
      अवरोध;
    हाल 'f':
      out_uint_x (pformat, prefix_len, statbuf->st_mode);
      अवरोध;
    हाल 'F':
      out_string (pformat, prefix_len, file_type (statbuf));
      अवरोध;
    हाल 'h':
      out_uint (pformat, prefix_len, statbuf->st_nlink);
      अवरोध;
    हाल 'u':
      out_uint (pformat, prefix_len, statbuf->st_uid);
      अवरोध;
    हाल 'U':
      pw_ent = getpwuid (statbuf->st_uid);
      out_string (pformat, prefix_len,
                  pw_ent ? pw_ent->pw_name : "UNKNOWN");
      अवरोध;
    हाल 'g':
      out_uint (pformat, prefix_len, statbuf->st_gid);
      अवरोध;
    हाल 'G':
      gw_ent = getgrgid (statbuf->st_gid);
      out_string (pformat, prefix_len,
                  gw_ent ? gw_ent->gr_name : "UNKNOWN");
      अवरोध;
    हाल 'm':
      fail |= out_mount_point (filename, pformat, prefix_len, statbuf);
      अवरोध;
    हाल 's':
      out_uint (pformat, prefix_len, unsigned_file_आकार (statbuf->st_आकार));
      अवरोध;
    हाल 'r':
      अगर (mod == 'H')
        out_uint (pformat, prefix_len, major (statbuf->st_rdev));
      अन्यथा अगर (mod == 'L')
        out_uint (pformat, prefix_len, minor (statbuf->st_rdev));
      अन्यथा
        out_uint (pformat, prefix_len, statbuf->st_rdev);
      अवरोध;
    हाल 'R':
      out_uint_x (pformat, prefix_len, statbuf->st_rdev);
      अवरोध;
    हाल 't':
      out_uint_x (pformat, prefix_len, major (statbuf->st_rdev));
      अवरोध;
    हाल 'T':
      out_uint_x (pformat, prefix_len, minor (statbuf->st_rdev));
      अवरोध;
    हाल 'B':
      out_uint (pformat, prefix_len, ST_NBLOCKSIZE);
      अवरोध;
    हाल 'b':
      out_uint (pformat, prefix_len, STP_NBLOCKS (statbuf));
      अवरोध;
    हाल 'o':
      out_uint (pformat, prefix_len, STP_BLKSIZE (statbuf));
      अवरोध;
    हाल 'w':
      {
#अगर ! USE_STATX
        btime = get_birthtime (fd, filename, statbuf);
#पूर्ण_अगर
        अगर (btime.tv_nsec < 0)
          out_string (pformat, prefix_len, "-");
        अन्यथा
          out_string (pformat, prefix_len, human_time (btime));
      }
      अवरोध;
    हाल 'W':
      {
#अगर ! USE_STATX
        btime = get_birthtime (fd, filename, statbuf);
#पूर्ण_अगर
        out_epoch_sec (pformat, prefix_len, neg_to_zero (btime));
      }
      अवरोध;
    हाल 'x':
      out_string (pformat, prefix_len, human_time (get_stat_atime (statbuf)));
      अवरोध;
    हाल 'X':
      out_epoch_sec (pformat, prefix_len, get_stat_atime (statbuf));
      अवरोध;
    हाल 'y':
      out_string (pformat, prefix_len, human_time (get_stat_mtime (statbuf)));
      अवरोध;
    हाल 'Y':
      out_epoch_sec (pformat, prefix_len, get_stat_mtime (statbuf));
      अवरोध;
    हाल 'z':
      out_string (pformat, prefix_len, human_time (get_stat_ctime (statbuf)));
      अवरोध;
    हाल 'Z':
      out_epoch_sec (pformat, prefix_len, get_stat_ctime (statbuf));
      अवरोध;
    हाल 'C':
      fail |= out_file_context (pformat, prefix_len, filename);
      अवरोध;
    शेष:
      ख_अक्षर_दो ('?', मानक_निकास);
      अवरोध;
    }
  वापस fail;
}

/* Return an allocated format string in static storage that
   corresponds to whether FS and TERSE options were declared.  */
अटल अक्षर *
default_format (bool fs, bool terse, bool device)
{
  अक्षर *format;
  अगर (fs)
    {
      अगर (terse)
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
      अगर (terse)
        {
          अगर (0 < is_selinux_enabled ())
            format = xstrdup (fmt_terse_selinux);
          अन्यथा
            format = xstrdup (fmt_terse_regular);
        }
      अन्यथा
        {
          अक्षर *अस्थायी;
          /* TRANSLATORS: This string uses format specifiers from
             'stat --help' without --file-system, and NOT from printf.  */
          format = xstrdup (_("\
  File: %N\n\
  आकार_3: %-10s\tBlocks: %-10b IO Block: %-6o %F\n\
"));

          अस्थायी = format;
          अगर (device)
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
          मुक्त (अस्थायी);

          अस्थायी = format;
          /* TRANSLATORS: This string uses format specifiers from
             'stat --help' without --file-system, and NOT from printf.  */
          format = xasprintf ("%s%s", format, _("\
" "Access: (%04a/%10.10A)  Uid: (%5u/%8U)   Gid: (%5g/%8G)\n\
"));
          मुक्त (अस्थायी);

          अगर (0 < is_selinux_enabled ())
            {
              अस्थायी = format;
              /* TRANSLATORS: This string uses format specifiers from
                 'stat --help' without --file-system, and NOT from printf.  */
              format = xasprintf ("%s%s", format, _("Context: %C\n"));
              मुक्त (अस्थायी);
            }

          अस्थायी = format;
          format = xasprintf ("%s%s", format,
                              /* TRANSLATORS: This string uses format specifiers
                                 from 'stat --help' without --file-system, and
                                 NOT from printf.  */
                              _("Access: %x\n"
                                "Modify: %y\n"
                                "Change: %z\n"
                                " Birth: %w\n"));
          मुक्त (अस्थायी);
        }
    }
  वापस format;
}

व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
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
         use the specified FORMAT instead of the शेष;\n\
         output a newline after each use of FORMAT\n\
"));
      oputs (_("\
      --म_लिखो=FORMAT\n\
         like --format, but interpret backslash escapes,\n\
         and करो not output a mandatory trailing newline;\n\
         अगर you want a newline, include \\n in FORMAT\n\
"));
      oputs (_("\
  -t, --terse\n\
         print the information in terse form\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);

      ख_माला_दो (_("\n\
The MODE argument of --cached can be: always, never, or शेष.\n\
'always' will use cached attributes अगर available, जबतक\n\
'never' will try to synchronize with the latest attributes, and\n\
'default' will leave it up to the underlying file प्रणाली.\n\
"), मानक_निकास);

      ख_माला_दो (_("\n\
The valid format sequences क्रम files (without --file-प्रणाली):\n\
\n\
  %a   permission bits in octal (see '#' and '0' म_लिखो flags)\n\
  %A   permission bits and file type in human readable form\n\
  %b   संख्या of blocks allocated (see %B)\n\
  %B   the आकार in bytes of each block reported by %b\n\
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
  %N   quoted file name with dereference अगर symbolic link\n\
  %o   optimal I/O transfer आकार hint\n\
  %s   योग_2 आकार, in bytes\n\
  %r   device type in decimal (st_rdev)\n\
  %R   device type in hex (st_rdev)\n\
  %Hr  major device type in decimal, क्रम character/block device special files\n\
  %Lr  minor device type in decimal, क्रम character/block device special files\n\
  %t   major device type in hex, क्रम character/block device special files\n\
  %T   minor device type in hex, क्रम character/block device special files\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  %u   user ID of owner\n\
  %U   user name of owner\n\
  %w   समय of file birth, human-readable; - अगर unknown\n\
  %W   समय of file birth, seconds since Epoch; 0 अगर unknown\n\
  %x   समय of last access, human-readable\n\
  %X   समय of last access, seconds since Epoch\n\
  %y   समय of last तथ्य modification, human-readable\n\
  %Y   समय of last तथ्य modification, seconds since Epoch\n\
  %z   समय of last status change, human-readable\n\
  %Z   समय of last status change, seconds since Epoch\n\
\n\
"), मानक_निकास);

      ख_माला_दो (_("\
Valid format sequences क्रम file systems:\n\
\n\
  %a   मुक्त blocks available to non-superuser\n\
  %b   योग_2 तथ्य blocks in file प्रणाली\n\
  %c   योग_2 file nodes in file प्रणाली\n\
  %d   मुक्त file nodes in file प्रणाली\n\
  %f   मुक्त blocks in file प्रणाली\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  %i   file प्रणाली ID in hex\n\
  %l   maximum लंबाई of filenames\n\
  %n   file name\n\
  %Qn  quoted file name\n\
  %s   block आकार (क्रम faster transfers)\n\
  %S   fundamental block आकार (क्रम block counts)\n\
  %t   file प्रणाली type in hex\n\
  %T   file प्रणाली type in human readable form\n\
"), मानक_निकास);

      म_लिखो (_("\n\
--terse is equivalent to the following FORMAT:\n\
    %s\
"),
#अगर HAVE_SELINUX_SELINUX_H
              fmt_terse_selinux
#अन्यथा
              fmt_terse_regular
#पूर्ण_अगर
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
मुख्य (पूर्णांक argc, अक्षर *argv[])
{
  पूर्णांक c;
  bool fs = false;
  bool terse = false;
  अक्षर *format = शून्य;
  अक्षर *format2;
  bool ok = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  काष्ठा स्था_बदल स्थिर *locale = क्षेत्र_बदलो ();
  decimal_point = (locale->decimal_point[0] ? locale->decimal_point : ".");
  decimal_point_len = म_माप (decimal_point);

  निकास_पर (close_stdout);

  जबतक ((c = getopt_long (argc, argv, "c:fLt", long_options, शून्य)) != -1)
    {
      चयन (c)
        {
        हाल PRINTF_OPTION:
          format = optarg;
          interpret_backslash_escapes = true;
          trailing_delim = "";
          अवरोध;

        हाल 'c':
          format = optarg;
          interpret_backslash_escapes = false;
          trailing_delim = "\n";
          अवरोध;

        हाल 'L':
          follow_links = true;
          अवरोध;

        हाल 'f':
          fs = true;
          अवरोध;

        हाल 't':
          terse = true;
          अवरोध;

        हाल 0:
          चयन (XARGMATCH ("--cached", optarg, cached_args, cached_modes))
            {
              हाल cached_never:
                force_sync = true;
                dont_sync = false;
                अवरोध;
              हाल cached_always:
                force_sync = false;
                dont_sync = true;
                अवरोध;
              हाल cached_default:
                force_sync = false;
                dont_sync = false;
            }
          अवरोध;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        शेष:
          usage (निकास_त्रुटि);
        }
    }

  अगर (argc == optind)
    {
      error (0, 0, _("missing operand"));
      usage (निकास_त्रुटि);
    }

  अगर (format)
    {
      format2 = format;
    }
  अन्यथा
    {
      format = default_format (fs, terse, /* device= */ false);
      format2 = default_format (fs, terse, /* device= */ true);
    }

  क्रम (पूर्णांक i = optind; i < argc; i++)
    ok &= (fs
           ? do_statfs (argv[i], format)
           : do_stat (argv[i], format, format2));

  main_exit (ok ? निकास_सफल : निकास_त्रुटि);
}
