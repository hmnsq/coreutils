/* 'dir', 'vdir' and 'ls' directory listing programs for GNU.
   Copyright (C) 1985-2026 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* If ls_mode is LS_MULTI_COL,
   the multi-column format is the default regardless
   of the type of output device.
   This is for the 'dir' program.

   If ls_mode is LS_LONG_FORMAT,
   the long format is the default regardless of the
   type of output device.
   This is for the 'vdir' program.

   If ls_mode is LS_LS,
   the output format depends on whether the output
   device is a terminal.
   This is for the 'ls' program.  */

/* Written by Richard Stallman and David MacKenzie.  */

/* Color support by Peter Anvin <Peter.Anvin@linux.org> and Dennis
   Flaherty <dennisf@denix.elk.miles.com> based on original patches by
   Greg Lee <lee@uhunix.uhcc.hawaii.edu>.  */

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>

#ಅನ್ತರ್ಭಾವ <termios.h>
#ಆದರೆ HAVE_STROPTS_H
# include <stropts.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <sys/ioctl.h>

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ WINSIZE_ಒಳ_PTEM
# include <sys/stream.h>
# include <sys/ptem.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <setjmp.h>
#ಅನ್ತರ್ಭಾವ <pwd.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <signal.h>

#ಆದರೆ HAVE_LANGINFO_CODESET
# include <langinfo.h>
#ಆದರೆ_ಅಂತ್ಯ

/* NonStop circa 2011 lacks both SA_RESTART and siginterrupt, so don't
   restart syscalls after a signal handler fires.  This may cause
   colors to get messed up on the screen if 'ls' is interrupted, but
   that's the best we can do on such a platform.  */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ SA_RESTART
# define SA_RESTART 0
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ <fnmatch.h>

/* Gnulib includes.  */
#ಅನ್ತರ್ಭಾವ "acl.h"
#ಅನ್ತರ್ಭಾವ "areadlink.h"
#ಅನ್ತರ್ಭಾವ "argmatch.h"
#ಅನ್ತರ್ಭಾವ "assure.h"
#ಅನ್ತರ್ಭಾವ "c-ctype.h"
#ಅನ್ತರ್ಭಾವ "c-strcase.h"
#ಅನ್ತರ್ಭಾವ "canonicalize.h"
#ಅನ್ತರ್ಭಾವ "dev-ino.h"
#ಅನ್ತರ್ಭಾವ "filemode.h"
#ಅನ್ತರ್ಭಾವ "filenamecat.h"
#ಅನ್ತರ್ಭಾವ "filevercmp.h"
#ಅನ್ತರ್ಭಾವ "hard-locale.h"
#ಅನ್ತರ್ಭಾವ "hash.h"
#ಅನ್ತರ್ಭಾವ "human.h"
#ಅನ್ತರ್ಭಾವ "idcache.h"
#ಅನ್ತರ್ಭಾವ "mbswidth.h"
#ಅನ್ತರ್ಭಾವ "mpsort.h"
#ಅನ್ತರ್ಭಾವ "obstack.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "stat-size.h"
#ಅನ್ತರ್ಭಾವ "stat-time.h"
#ಅನ್ತರ್ಭಾವ "strftime.h"
#ಅನ್ತರ್ಭಾವ "xdectoint.h"
#ಅನ್ತರ್ಭಾವ "xgethostname.h"
#ಅನ್ತರ್ಭಾವ "xmemdup0.h"
#ಅನ್ತರ್ಭಾವ "xstrtol-error.h"
#ಅನ್ತರ್ಭಾವ "xstrtol.h"

#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ "dircolors.h"
#ಅನ್ತರ್ಭಾವ "ls.h"
#ಅನ್ತರ್ಭಾವ "statx.h"
#ಅನ್ತರ್ಭಾವ "term-sig.h"

/* Include <sys/capability.h> last to avoid a clash of <sys/types.h>
   include guards with some premature versions of libcap.
   For more details, see <https://bugzilla.redhat.com/483548>.  */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CAP
# include <sys/capability.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ HAVE_LINUX_XATTR_H
# include <linux/xattr.h>
# ifndef XATTR_NAME_CAPS
#  define XATTR_NAME_CAPS "security.capability"
# endif
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME (ls_ಬಹುಲಕ == LS_LS ? "ls" \
                      : (ls_ಬಹುಲಕ == LS_MULTI_ಲಂಬಸಾಲು \
                         ? "dir" : "vdir"))

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS \
  proper_name ("Richard M. Stallman"), \
  proper_name ("David MacKenzie")

#ವ್ಯಾಖ್ಯಾನಿಸು obstack_chunk_alloc ಸ್ಮರಣೆ_ಕೊಡು
#ವ್ಯಾಖ್ಯಾನಿಸು obstack_chunk_free ಬಿಡುಗಡೆ

/* Unix-based readdir implementations have historically returned a dirent.d_ino
   value that is sometimes not equal to the stat-obtained st_ino value for
   that same entry.  This error occurs for a readdir entry that refers
   to a mount point.  readdir's error is to return the inode number of
   the underlying directory -- one that typically cannot be stat'ed, as
   long as a file system is mounted on that directory.  RELIABLE_D_INO
   encapsulates whether we can use the more efficient approach of relying
   on readdir-supplied d_ino values, or whether we must incur the cost of
   calling stat or lstat to obtain each guaranteed-valid inode number.  */

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ READDIR_LIES_ABOUT_MOUNTPOINT_D_INO
# define READDIR_LIES_ABOUT_MOUNTPOINT_D_INO 1
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ READDIR_LIES_ABOUT_MOUNTPOINT_D_INO
# define RELIABLE_D_INO(dp) NOT_AN_INODE_ಸಂಖ್ಯೆ
#ಇಲ್ಲದಿದ್ದರೆ
# define RELIABLE_D_INO(dp) D_INO (dp)
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ ! HAVE_STRUCT_STAT_ST_AUTHOR
# define st_author st_uid
#ಆದರೆ_ಅಂತ್ಯ

ಎಣಿಕೆ filetype
  {
    unknown,
    fifo,
    chardev,
    directory,
    blockdev,
    ಸಾಮಾನ್ಯ,
    symbolic_link,
    sock,
    whiteout,
    arg_directory
  };
ಎಣಿಕೆ { filetype_cardinality = arg_directory + 1 };

/* Display letters and indicators for each filetype.
   Keep these in sync with enum filetype.  */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ filetype_letter[] =
  {'?', 'p', 'c', 'd', 'b', '-', 'l', 's', 'w', 'd'};
static_assert (countof (filetype_letter) == filetype_cardinality);

/* Map enum filetype to <dirent.h> d_type values.  */
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ filetype_d_type[] =
  {
    DT_UNKNOWN, DT_FIFO, DT_CHR, DT_DIR, DT_BLK, DT_REG, DT_LNK, DT_SOCK,
    DT_WHT, DT_DIR
  };
static_assert (countof (filetype_d_type) == filetype_cardinality);

/* Map d_type values to enum filetype.  */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ d_type_filetype[UCHAR_MAX + 1] =
  {
    [DT_BLK] = blockdev, [DT_CHR] = chardev, [DT_DIR] = directory,
    [DT_FIFO] = fifo, [DT_LNK] = symbolic_link, [DT_REG] = ಸಾಮಾನ್ಯ,
    [DT_SOCK] = sock, [DT_WHT] = whiteout
  };

ಎಣಿಕೆ acl_type
  {
    ACL_T_NONE,
    ACL_T_UNKNOWN,
    ACL_T_LSM_CONTEXT_ONLY,
    ACL_T_YES
  };

ರಚನೆ fileinfo
  {
    /* The file name.  */
    ಅಕ್ಷರ *name;

    /* For symbolic link, name of the file linked to, otherwise zero.  */
    ಅಕ್ಷರ *linkname;

    /* For terminal hyperlinks. */
    ಅಕ್ಷರ *absolute_name;

    ರಚನೆ stat stat;

    ಎಣಿಕೆ filetype filetype;

    /* For symbolic link and long listing, st_mode of file linked to, otherwise
       zero.  */
    mode_t linkmode;

    /* security context.  */
    ಅಕ್ಷರ *scontext;

    bool stat_ok;

    /* For symbolic link and color printing, true if linked-to file
       exists, otherwise false.  */
    bool linkok;

    /* For long listings, true if the file has an access control list,
       or a security context.  */
    ಎಣಿಕೆ acl_type acl_type;

    /* For color listings, true if a regular file has capability info.  */
    bool has_capability;

    /* Whether file name needs quoting. tri-state with -1 == unknown.  */
    ಪೂರ್ಣಾಂಕ quoted;

    /* Cached screen width (including quoting).  */
    size_t ಅಗಲ;
  };

/* Null is a valid character in a color indicator (think about Epson
   printers, for example) so we have to use a length/buffer string
   type.  */

ರಚನೆ bin_str
  {
    size_t len;			/* Number of bytes */
    ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *string;		/* Pointer to the same */
  };

#ಆದರೆ ! HAVE_TCGETPGRP
# define tcgetpgrp(Fd) 0
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ size_t quote_name (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name,
                          ರಚನೆ quoting_options ಸ್ಥಿರಾಂಕ *options,
                          ಪೂರ್ಣಾಂಕ needs_general_quoting,
                          ಸ್ಥಿರಾಂಕ ರಚನೆ bin_str *color,
                          bool allow_pad, ರಚನೆ obstack *ರಾಶಿ,
                          ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *absolute_name);
ಸ್ಥಿರ size_t quote_name_buf (ಅಕ್ಷರ **inbuf, size_t bufsize, ಅಕ್ಷರ *name,
                              ರಚನೆ quoting_options ಸ್ಥಿರಾಂಕ *options,
                              ಪೂರ್ಣಾಂಕ needs_general_quoting, size_t *ಅಗಲ,
                              bool *pad);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ decode_switches (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv);
ಸ್ಥಿರ bool file_ignored (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name);
ಸ್ಥಿರ uintmax_t gobble_file (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name, ಎಣಿಕೆ filetype type,
                              ino_t inode, bool command_ಸಾಲು_arg,
                              ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dirname);
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ bin_str * get_color_indicator (ಸ್ಥಿರಾಂಕ ರಚನೆ fileinfo *f,
                                                   bool symlink_ಗುರಿ);
ಸ್ಥಿರ bool print_color_indicator (ಸ್ಥಿರಾಂಕ ರಚನೆ bin_str *ind);
ಸ್ಥಿರ ಶೂನ್ಯ put_indicator (ಸ್ಥಿರಾಂಕ ರಚನೆ bin_str *ind);
ಸ್ಥಿರ ಶೂನ್ಯ ಕೂಡಿಸು_ignore_ನಮೂನೆ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ನಮೂನೆ_2);
ಸ್ಥಿರ ಶೂನ್ಯ attach (ಅಕ್ಷರ *dest, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dirname, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name);
ಸ್ಥಿರ ಶೂನ್ಯ clear_files (ಶೂನ್ಯ);
ಸ್ಥಿರ ಶೂನ್ಯ ಹೊರತೆಗೆ_dirs_from_files (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dirname,
                                     bool command_ಸಾಲು_arg);
ಸ್ಥಿರ bool get_link_name (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename, ರಚನೆ fileinfo *f,
                           bool command_ಸಾಲು_arg);
ಸ್ಥಿರ ಶೂನ್ಯ indent (size_t from, size_t ಗಮ್ಯ);
ಸ್ಥಿರ idx_t calculate_columns (bool by_columns);
ಸ್ಥಿರ ಶೂನ್ಯ print_current_files (ಶೂನ್ಯ);
ಸ್ಥಿರ ಶೂನ್ಯ print_dir (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *realname,
                       bool command_ಸಾಲು_arg);
ಸ್ಥಿರ size_t print_file_name_and_frills (ಸ್ಥಿರಾಂಕ ರಚನೆ fileinfo *f,
                                          size_t ಆರಂಭ_ಲಂಬಸಾಲು);
ಸ್ಥಿರ ಶೂನ್ಯ print_horizontal (ಶೂನ್ಯ);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ format_user_ಅಗಲ (uid_t u);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ format_ಗುಂಪು_ಅಗಲ (gid_t g);
ಸ್ಥಿರ ಶೂನ್ಯ print_long_format (ಸ್ಥಿರಾಂಕ ರಚನೆ fileinfo *f);
ಸ್ಥಿರ ಶೂನ್ಯ print_many_ಪ್ರತಿ_ಸಾಲು (ಶೂನ್ಯ);
ಸ್ಥಿರ size_t print_name_with_quoting (ಸ್ಥಿರಾಂಕ ರಚನೆ fileinfo *f,
                                       bool symlink_ಗುರಿ,
                                       ರಚನೆ obstack *ರಾಶಿ,
                                       size_t ಆರಂಭ_ಲಂಬಸಾಲು);
ಸ್ಥಿರ ಶೂನ್ಯ prep_non_filename_ಪಠ್ಯ (ಶೂನ್ಯ);
ಸ್ಥಿರ bool print_type_indicator (bool stat_ok, mode_t ಬಹುಲಕ_2,
                                  ಎಣಿಕೆ filetype type);
ಸ್ಥಿರ ಶೂನ್ಯ print_with_separator (ಅಕ್ಷರ sep);
ಸ್ಥಿರ ಶೂನ್ಯ ಸರತಿ_directory (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *realname,
                             bool command_ಸಾಲು_arg);
ಸ್ಥಿರ ಶೂನ್ಯ ವಿಂಗಡಿಸು_files (ಶೂನ್ಯ);
ಸ್ಥಿರ ಶೂನ್ಯ parse_ls_color (ಶೂನ್ಯ);

ಸ್ಥಿರ size_t quote_name_ಅಗಲ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name,
                                ರಚನೆ quoting_options ಸ್ಥಿರಾಂಕ *options,
                                ಪೂರ್ಣಾಂಕ needs_general_quoting);

/* Initial size of hash table.
   Most hierarchies are likely to be shallower than this.  */
ಎಣಿಕೆ { INITIAL_ಕೋಷ್ಟಕ_ಗಾತ್ರ = 30 };

/* The set of 'active' directories, from the current command-line argument
   to the level in the hierarchy at which files are being listed.
   A directory is represented by its device and inode numbers (struct dev_ino).
   A directory is added to this set when ls begins listing it or its
   entries, and it is removed from the set just after ls has finished
   processing it.  This set is used solely to detect loops, e.g., with
   mkdir loop; cd loop; ln -s ../loop sub; ls -RL  */
ಸ್ಥಿರ Hash_ಕೋಷ್ಟಕ *ಸಕ್ರಿಯ_dir_ಗಣ;

#ವ್ಯಾಖ್ಯಾನಿಸು LOOP_DETECT (!!ಸಕ್ರಿಯ_dir_ಗಣ)

/* The table of files in the current directory:

   'cwd_file' points to a vector of 'struct fileinfo', one per file.
   'cwd_n_alloc' is the number of elements space has been allocated for.
   'cwd_n_used' is the number actually in use.  */

/* Address of block containing the files that are described.  */
ಸ್ಥಿರ ರಚನೆ fileinfo *cwd_file;

/* Length of block that 'cwd_file' points to, measured in files.  */
ಸ್ಥಿರ idx_t cwd_n_alloc;

/* Index of first unused slot in 'cwd_file'.  */
ಸ್ಥಿರ idx_t cwd_n_ಬಳಸಿದ;

/* Whether files needs may need padding due to quoting.  */
ಸ್ಥಿರ bool cwd_some_quoted;

/* Whether quoting style _may_ add outer quotes,
   and whether aligning those is useful.  */
ಸ್ಥಿರ bool align_variable_outer_quotes;

/* Vector of pointers to files, in proper sorted order, and the number
   of entries allocated for it.  */
ಸ್ಥಿರ ಶೂನ್ಯ **sorted_file;
ಸ್ಥಿರ size_t sorted_file_alloc;

/* When true, in a color listing, color each symlink name according to the
   type of file it points to.  Otherwise, color them according to the 'ln'
   directive in LS_COLORS.  Dangling (orphan) symlinks are treated specially,
   regardless.  This is set when 'ln=target' appears in LS_COLORS.  */

ಸ್ಥಿರ bool color_symlink_as_referent;

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *hostname;

/* Mode of appropriate file for coloring.  */
ಸ್ಥಿರ mode_t
file_or_link_ಬಹುಲಕ (ರಚನೆ fileinfo ಸ್ಥಿರಾಂಕ *file)
{
  ಹಿಂತಿರುಗಿ (color_symlink_as_referent && file->linkok
          ? file->linkmode : file->stat.st_ಬಹುಲಕ);
}


/* Record of one pending directory waiting to be listed.  */

ರಚನೆ pending
  {
    ಅಕ್ಷರ *name;
    /* If the directory is actually the file pointed to by a symbolic link we
       were told to list, 'realname' will contain the name of the symbolic
       link, otherwise zero.  */
    ಅಕ್ಷರ *realname;
    bool command_ಸಾಲು_arg;
    ರಚನೆ pending *ಮುಂದಿನ;
  };

ಸ್ಥಿರ ರಚನೆ pending *pending_dirs;

/* Current time in seconds and nanoseconds since 1970, updated as
   needed when deciding whether a file is recent.  */

ಸ್ಥಿರ ರಚನೆ timespec current_ಸಮಯ;

ಸ್ಥಿರ bool print_scontext;
ಸ್ಥಿರ ಅಕ್ಷರ UNKNOWN_SECURITY_CONTEXT[] = "?";

/* Whether any of the files has an ACL.  This affects the width of the
   mode column.  */

ಸ್ಥಿರ bool any_has_acl;

/* The number of columns to use for columns containing inode numbers,
   block sizes, link counts, owners, groups, authors, major device
   numbers, minor device numbers, and file sizes, respectively.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ inode_ಸಂಖ್ಯೆ_ಅಗಲ;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಖಂಡ_ಗಾತ್ರ_ಅಗಲ;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ nlink_ಅಗಲ;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ scontext_ಅಗಲ;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ owner_ಅಗಲ;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಗುಂಪು_ಅಗಲ;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ author_ಅಗಲ;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ major_device_ಸಂಖ್ಯೆ_ಅಗಲ;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ minor_device_ಸಂಖ್ಯೆ_ಅಗಲ;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ file_ಗಾತ್ರ_ಅಗಲ;

/* Option flags */

/* long_format for lots of info, one per line.
   one_per_line for just names, one per line.
   many_per_line for just names, many per line, sorted vertically.
   horizontal for just names, many per line, sorted horizontally.
   with_commas for just names, many per line, separated by commas.

   -l (and other options that imply -l), -1, -C, -x and -m control
   this parameter.  */

ಎಣಿಕೆ format
  {
    long_format,		/* -l and other options that imply -l */
    one_ಪ್ರತಿ_ಸಾಲು,		/* -1 */
    many_ಪ್ರತಿ_ಸಾಲು,		/* -C */
    horizontal,			/* -x */
    with_commas			/* -m */
  };

ಸ್ಥಿರ ಎಣಿಕೆ format format;

/* 'full-iso' uses full ISO-style dates and times.  'long-iso' uses longer
   ISO-style timestamps, though shorter than 'full-iso'.  'iso' uses shorter
   ISO-style timestamps.  'locale' uses locale-dependent timestamps.  */
ಎಣಿಕೆ ಸಮಯ_style
  {
    ತುಂಬಿದ_iso_ಸಮಯ_style,	/* --time-style=full-iso */
    long_iso_ಸಮಯ_style,	/* --time-style=long-iso */
    iso_ಸಮಯ_style,		/* --time-style=iso */
    locale_ಸಮಯ_style		/* --time-style=locale */
  };

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸ್ಥಿರಾಂಕ ಸಮಯ_style_ವಾದಗಳು[] =
{
  "full-iso", "long-iso", "iso", "locale", ಖಾಲಿ
};
ಸ್ಥಿರ ಎಣಿಕೆ ಸಮಯ_style ಸ್ಥಿರಾಂಕ ಸಮಯ_style_types[] =
{
  ತುಂಬಿದ_iso_ಸಮಯ_style, long_iso_ಸಮಯ_style, iso_ಸಮಯ_style,
  locale_ಸಮಯ_style
};
ARGMATCH_VERIFY (ಸಮಯ_style_ವಾದಗಳು, ಸಮಯ_style_types);

/* Type of time to print or sort by.  Controlled by -c and -u.
   The values of each item of this enum are important since they are
   used as indices in the sort functions array (see sort_files()).  */

ಎಣಿಕೆ ಸಮಯ_type
  {
    ಸಮಯ_mtime = 0,		/* default */
    ಸಮಯ_ctime,			/* -c */
    ಸಮಯ_atime,			/* -u */
    ಸಮಯ_btime,                 /* birth time */
    ಸಮಯ_numtypes		/* the number of elements of this enum */
  };

ಸ್ಥಿರ ಎಣಿಕೆ ಸಮಯ_type ಸಮಯ_type;
ಸ್ಥಿರ bool explicit_ಸಮಯ;

/* The file characteristic to sort by.  Controlled by -t, -S, -U, -X, -v.
   The values of each item of this enum are important since they are
   used as indices in the sort functions array (see sort_files()).  */

ಎಣಿಕೆ ವಿಂಗಡಿಸು_type
  {
    ವಿಂಗಡಿಸು_name = 0,		/* default */
    ವಿಂಗಡಿಸು_extension,		/* -X */
    ವಿಂಗಡಿಸು_ಅಗಲ,
    ವಿಂಗಡಿಸು_ಗಾತ್ರ,			/* -S */
    ವಿಂಗಡಿಸು_ಆವೃತ್ತಿ,		/* -v */
    ವಿಂಗಡಿಸು_ಸಮಯ,			/* -t; must be second to last */
    ವಿಂಗಡಿಸು_none,			/* -U; must be last */
    ವಿಂಗಡಿಸು_numtypes		/* the number of elements of this enum */
  };

ಸ್ಥಿರ ಎಣಿಕೆ ವಿಂಗಡಿಸು_type ವಿಂಗಡಿಸು_type;

/* Direction of sort.
   false means highest first if numeric,
   lowest first if alphabetic;
   these are the defaults.
   true means the opposite order in each case.  -r  */

ಸ್ಥಿರ bool ವಿಂಗಡಿಸು_ಹಿಮ್ಮುಖ;

/* True means to display owner information.  -g turns this off.  */

ಸ್ಥಿರ bool print_owner = true;

/* True means to display author information.  */

ಸ್ಥಿರ bool print_author;

/* True means to display group information.  -G and -o turn this off.  */

ಸ್ಥಿರ bool print_ಗುಂಪು = true;

/* True means print the user and group id's as numbers rather
   than as names.  -n  */

ಸ್ಥಿರ bool numeric_ids;

/* True means mention the size in blocks of each file.  -s  */

ಸ್ಥಿರ bool print_ಖಂಡ_ಗಾತ್ರ;

/* Human-readable options for output, when printing block counts.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ human_output_opts;

/* The units to use when printing block counts.  */
ಸ್ಥಿರ uintmax_t output_ಖಂಡ_ಗಾತ್ರ;

/* Likewise, but for file sizes.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ file_human_output_opts;
ಸ್ಥಿರ uintmax_t file_output_ಖಂಡ_ಗಾತ್ರ = 1;

/* Follow the output with a special string.  Using this format,
   Emacs' dired mode starts up twice as fast, and can handle all
   strange characters in file names.  */
ಸ್ಥಿರ bool dired;

/* 'none' means don't mention the type of files.
   'slash' means mention directories only, with a '/'.
   'file_type' means mention file types.
   'classify' means mention file types and mark executables.

   Controlled by -F, -p, and --indicator-style.  */

ಎಣಿಕೆ indicator_style
  {
    none = 0,	/*     --indicator-style=none (default) */
    slash,	/* -p, --indicator-style=slash */
    file_type,	/*     --indicator-style=file-type */
    classify	/* -F, --indicator-style=classify */
  };

ಸ್ಥಿರ ಎಣಿಕೆ indicator_style indicator_style;

/* Names of indicator styles.  */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸ್ಥಿರಾಂಕ indicator_style_ವಾದಗಳು[] =
{
  "none", "slash", "file-type", "classify", ಖಾಲಿ
};
ಸ್ಥಿರ ಎಣಿಕೆ indicator_style ಸ್ಥಿರಾಂಕ indicator_style_types[] =
{
  none, slash, file_type, classify
};
ARGMATCH_VERIFY (indicator_style_ವಾದಗಳು, indicator_style_types);

/* True means use colors to mark types.  Also define the different
   colors as well as the stuff for the LS_COLORS environment variable.
   The LS_COLORS variable is now in a termcap-like format.  */

ಸ್ಥಿರ bool print_with_color;

ಸ್ಥಿರ bool print_hyperlink;

/* Whether we used any colors in the output so far.  If so, we will
   need to restore the default color later.  If not, we will need to
   call prep_non_filename_text before using color for the first time. */

ಸ್ಥಿರ bool ಬಳಸಿದ_color = false;

ಎಣಿಕೆ when_type
  {
    when_never,		/* 0: default or --color=never */
    when_always,	/* 1: --color=always */
    when_if_tty		/* 2: --color=tty */
  };

ಎಣಿಕೆ Dereference_symlink
  {
    DEREF_UNDEFINED = 0,		/* default */
    DEREF_NEVER,
    DEREF_COMMAND_ಸಾಲು_ARGUMENTS,	/* -H */
    DEREF_COMMAND_ಸಾಲು_SYMLINK_ಗಮ್ಯ_DIR,	/* the default, in certain cases */
    DEREF_ALWAYS			/* -L */
  };

ಎಣಿಕೆ indicator_no
  {
    C_ಎಡ, C_ಬಲ, C_ಕೊನೆ, C_RESET, C_NORM, C_FILE, C_DIR, C_LINK,
    C_FIFO, C_SOCK,
    C_BLK, C_CHR, C_MISSING, C_ORPHAN, C_EXEC, C_DOOR, C_SETUID, C_SETGID,
    C_STICKY, C_OTHER_WRITABLE, C_STICKY_OTHER_WRITABLE, C_CAP, C_MULTIHARDLINK,
    C_CLR_ಗಮ್ಯ_EOL
  };

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ indicator_name[][2]=
  {
    {'l','c'}, {'r','c'}, {'e','c'}, {'r','s'}, {'n','o'},
    {'f','i'}, {'d','i'}, {'l','n'},
    {'p','i'}, {'s','o'},
    {'b','d'}, {'c','d'}, {'m','i'}, {'o','r'}, {'e','x'},
    {'d','o'}, {'s','u'}, {'s','g'},
    {'s','t'}, {'o','w'}, {'t','w'}, {'c','a'}, {'m','h'},
    {'c','l'}
  };

ರಚನೆ color_ext_type
  {
    ರಚನೆ bin_str ext;		/* The extension we're looking for */
    ರಚನೆ bin_str seq;		/* The sequence to output when we do */
    bool   exact_ಹೊಂದಿಕೆ;		/* Whether to compare case insensitively */
    ರಚನೆ color_ext_type *ಮುಂದಿನ;	/* Next in list */
  };

ಸ್ಥಿರ ರಚನೆ bin_str color_indicator[] =
  {
    { 2, (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ []) {'\033','['} },/* lc: Left of color sequence */
    { 1, (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ []) {'m'} },	/* rc: Right of color sequence */
    { 0, ಖಾಲಿ },			/* ec: End color (replaces lc+rs+rc) */
    { 1, (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ []) {'0'} },	/* rs: Reset to ordinary colors */
    { 0, ಖಾಲಿ },			/* no: Normal */
    { 0, ಖಾಲಿ },			/* fi: File: default */
    { 5, ((ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ [])
          {'0','1',';','3','4'}) },	/* di: Directory: bright blue */
    { 5, ((ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ [])
          {'0','1',';','3','6'}) },	/* ln: Symlink: bright cyan */
    { 2, (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ []) {'3','3'} },		/* pi: Pipe: yellow/brown */
    { 5, ((ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ [])
          {'0','1',';','3','5'}) },	/* so: Socket: bright magenta */
    { 5, ((ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ [])
          {'0','1',';','3','3'}) },	/* bd: Block device: bright yellow */
    { 5, ((ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ [])
          {'0','1',';','3','3'}) },	/* cd: Char device: bright yellow */
    { 0, ಖಾಲಿ },			/* mi: Missing file: undefined */
    { 0, ಖಾಲಿ },			/* or: Orphaned symlink: undefined */
    { 5, ((ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ [])
          {'0','1',';','3','2'}) },	/* ex: Executable: bright green */
    { 5, ((ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ [])
          {'0','1',';','3','5'}) },	/* do: Door: bright magenta */
    { 5, ((ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ [])
          {'3','7',';','4','1'}) },	/* su: setuid: white on red */
    { 5, ((ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ [])
          {'3','0',';','4','3'}) },	/* sg: setgid: black on yellow */
    { 5, ((ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ [])
          {'3','7',';','4','4'}) },	/* st: sticky: black on blue */
    { 5, ((ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ [])
          {'3','4',';','4','2'}) },	/* ow: other-writable: blue on green */
    { 5, ((ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ [])
          {'3','0',';','4','2'}) },	/* tw: ow w/ sticky: black on green */
    { 0, ಖಾಲಿ },			/* ca: disabled by default */
    { 0, ಖಾಲಿ },			/* mh: disabled by default */
    { 3, ((ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ [])
          {'\033','[','K'}) },		/* cl: clear to end of line */
  };

/* A list mapping file extensions to corresponding display sequence.  */
ಸ್ಥಿರ ರಚನೆ color_ext_type *color_ext_list = ಖಾಲಿ;

/* Buffer for color sequences */
ಸ್ಥಿರ ಅಕ್ಷರ *color_buf;

/* True means to check for orphaned symbolic link, for displaying
   colors, or to group symlink to directories with other dirs.  */

ಸ್ಥಿರ bool check_symlink_ಬಹುಲಕ;

/* True means mention the inode number of each file.  -i  */

ಸ್ಥಿರ bool print_inode;

/* What to do with symbolic links.  Affected by -d, -F, -H, -l (and
   other options that imply -l), and -L.  */

ಸ್ಥಿರ ಎಣಿಕೆ Dereference_symlink dereference;

/* True means when a directory is found, display info on its
   contents.  -R  */

ಸ್ಥಿರ bool recursive;

/* True means when an argument is a directory name, display info
   on it itself.  -d  */

ಸ್ಥಿರ bool ತಕ್ಷಣ_dirs;

/* True means that directories are grouped before files. */

ಸ್ಥಿರ bool directories_ಮೊದಲ;

/* Which files to ignore.  */

ಸ್ಥಿರ ಎಣಿಕೆ
{
  /* Ignore files whose names start with '.', and files specified by
     --hide and --ignore.  */
  IGNORE_DEFAULT = 0,

  /* Ignore '.', '..', and files specified by --ignore.  */
  IGNORE_DOT_AND_DOTDOT,

  /* Ignore only files specified by --ignore.  */
  IGNORE_ಕನಿಷ್ಠತಮ
} ignore_ಬಹುಲಕ;

/* A linked list of shell-style globbing patterns.  If a non-argument
   file name matches any of these patterns, it is ignored.
   Controlled by -I.  Multiple -I options accumulate.
   The -B option adds '*~' and '.*~' to this list.  */

ರಚನೆ ignore_ನಮೂನೆ
  {
    ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ನಮೂನೆ_2;
    ರಚನೆ ignore_ನಮೂನೆ *ಮುಂದಿನ;
  };

ಸ್ಥಿರ ರಚನೆ ignore_ನಮೂನೆ *ignore_patterns;

/* Similar to IGNORE_PATTERNS, except that -a or -A causes this
   variable itself to be ignored.  */
ಸ್ಥಿರ ರಚನೆ ignore_ನಮೂನೆ *hide_patterns;

/* True means output nongraphic chars in file names as '?'.
   (-q, --hide-control-chars)
   qmark_funny_chars and the quoting style (-Q, --quoting-style=WORD) are
   independent.  The algorithm is: first, obey the quoting style to get a
   string representing the file name;  then, if qmark_funny_chars is set,
   replace all nonprintable chars in that string with '?'.  It's necessary
   to replace nonprintable chars even in quoted strings, because we don't
   want to mess up the terminal if control chars get sent to it, and some
   quoting methods pass through control chars as-is.  */
ಸ್ಥಿರ bool qmark_funny_chars;

/* True means extra replacement of newline with '?'
   in cases where a newline in a file name would be
   ambiguous with newline separators.  */
ಸ್ಥಿರ bool qmark_newline;

/* Quoting options for file and dir name output.  */

ಸ್ಥಿರ ರಚನೆ quoting_options *filename_quoting_options;
ಸ್ಥಿರ ರಚನೆ quoting_options *dirname_quoting_options;

/* The number of chars per hardware tab stop.  Setting this to zero
   inhibits the use of TAB characters for separating columns.  -T */
ಸ್ಥಿರ size_t tabsize;

/* True means print each directory name before listing it.  */

ಸ್ಥಿರ bool print_dir_name;

/* The line length to use for breaking lines in many-per-line format.
   Can be set with -w.  If zero, there is no limit.  */

ಸ್ಥಿರ size_t ಸಾಲು_ಉದ್ದಳತೆ;

/* The local time zone rules, as per the TZ environment variable.  */

ಸ್ಥಿರ timezone_t localtz;

/* If true, the file listing format requires that stat be called on
   each file.  */

ಸ್ಥಿರ bool format_needs_stat;

/* Similar to 'format_needs_stat', but set if only the file type is
   needed.  */

ಸ್ಥಿರ bool format_needs_type;

/* Like 'format_needs_stat', but set only if capability colors are needed.  */

ಸ್ಥಿರ bool format_needs_capability;

/* An arbitrary limit on the number of bytes in a printed timestamp.
   This is set to a relatively small value to avoid the need to worry
   about denial-of-service attacks on servers that run "ls" on behalf
   of remote clients.  1000 bytes should be enough for any practical
   timestamp format.  */

ಎಣಿಕೆ { ಸಮಯ_STAMP_LEN_ಗರಿಷ್ಠ = MAX (1000, INT_STRLEN_BOUND (time_t)) };

/* strftime formats for non-recent and recent files, respectively, in
   -l output.  */

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *long_ಸಮಯ_format[2] =
  {
    /* strftime format for non-recent files (older than 6 months), in
       -l output.  This should contain the year, month and day (at
       least), in an order that is understood by people in your
       locale's territory.  Please try to keep the number of used
       screen columns small, because many people work in windows with
       only 80 columns.  But make this as wide as the other string
       below, for recent files.  */
    /* TRANSLATORS: ls output needs to be aligned for ease of reading,
       so be wary of using variable width fields from the locale.
       Note %b is handled specially by ls and aligned correctly.
       Note also that specifying a width as in %5b is erroneous as strftime
       will count bytes rather than characters in multibyte locales.  */
    N_("%b %e  %Y"),
    /* strftime format for recent files (younger than 6 months), in -l
       output.  This should contain the month, day and time (at
       least), in an order that is understood by people in your
       locale's territory.  Please try to keep the number of used
       screen columns small, because many people work in windows with
       only 80 columns.  But make this as wide as the other string
       above, for non-recent files.  */
    /* TRANSLATORS: ls output needs to be aligned for ease of reading,
       so be wary of using variable width fields from the locale.
       Note %b is handled specially by ls and aligned correctly.
       Note also that specifying a width as in %5b is erroneous as strftime
       will count bytes rather than characters in multibyte locales.  */
    N_("%b %e %H:%M")
  };

/* The set of signals that are caught.  */

ಸ್ಥಿರ sigset_t caught_ಸಂಕೇತಗಳು;

/* If nonzero, the value of the pending fatal signal.  */

ಸ್ಥಿರ sig_atomic_t ಅಸ್ಥಿರ ಅಡಚಣೆ_signal;

/* A count of the number of pending stop signals that have been received.  */

ಸ್ಥಿರ sig_atomic_t ಅಸ್ಥಿರ stop_signal_ಎಣಿಕೆ;

/* Desired exit status.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ exit_ಸ್ಥಿತಿ;

/* Exit statuses.  */
ಎಣಿಕೆ
  {
    /* "ls" had a minor problem.  E.g., while processing a directory,
       ls obtained the name of an entry via readdir, yet was later
       unable to stat that name.  This happens when listing a directory
       in which entries are actively being removed or renamed.  */
    LS_MINOR_PROBLEM = 1,

    /* "ls" had more serious trouble (e.g., memory exhausted, invalid
       option or failure to stat a command line argument.  */
    LS_FAILURE = 2
  };

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ಎಣಿಕೆ
{
  AUTHOR_OPTION = CHAR_MAX + 1,
  ಖಂಡ_ಗಾತ್ರ_OPTION,
  COLOR_OPTION,
  DEREFERENCE_COMMAND_ಸಾಲು_SYMLINK_ಗಮ್ಯ_DIR_OPTION,
  FILE_TYPE_INDICATOR_OPTION,
  FORMAT_OPTION,
  ತುಂಬಿದ_ಸಮಯ_OPTION,
  ಗುಂಪು_DIRECTORIES_ಮೊದಲ_OPTION,
  HIDE_OPTION,
  HYPERLINK_OPTION,
  INDICATOR_STYLE_OPTION,
  QUOTING_STYLE_OPTION,
  SHOW_CONTROL_CHARS_OPTION,
  SI_OPTION,
  ವಿಂಗಡಿಸು_OPTION,
  ಸಮಯ_OPTION,
  ಸಮಯ_STYLE_OPTION,
  ZERO_OPTION,
};

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ long_options[] =
{
  {"all", no_ವಾದ, ಖಾಲಿ, 'a'},
  {"escape", no_ವಾದ, ಖಾಲಿ, 'b'},
  {"directory", no_ವಾದ, ಖಾಲಿ, 'd'},
  {"dired", no_ವಾದ, ಖಾಲಿ, 'D'},
  {"full-time", no_ವಾದ, ಖಾಲಿ, ತುಂಬಿದ_ಸಮಯ_OPTION},
  {"group-directories-first", no_ವಾದ, ಖಾಲಿ,
   ಗುಂಪು_DIRECTORIES_ಮೊದಲ_OPTION},
  {"human-readable", no_ವಾದ, ಖಾಲಿ, 'h'},
  {"inode", no_ವಾದ, ಖಾಲಿ, 'i'},
  {"kibibytes", no_ವಾದ, ಖಾಲಿ, 'k'},
  {"numeric-uid-gid", no_ವಾದ, ಖಾಲಿ, 'n'},
  {"no-group", no_ವಾದ, ಖಾಲಿ, 'G'},
  {"hide-control-chars", no_ವಾದ, ಖಾಲಿ, 'q'},
  {"reverse", no_ವಾದ, ಖಾಲಿ, 'r'},
  {"size", no_ವಾದ, ಖಾಲಿ, 's'},
  {"width", required_ವಾದ, ಖಾಲಿ, 'w'},
  {"almost-all", no_ವಾದ, ಖಾಲಿ, 'A'},
  {"ignore-backups", no_ವಾದ, ಖಾಲಿ, 'B'},
  {"classify", optional_ವಾದ, ಖಾಲಿ, 'F'},
  {"file-type", no_ವಾದ, ಖಾಲಿ, FILE_TYPE_INDICATOR_OPTION},
  {"si", no_ವಾದ, ಖಾಲಿ, SI_OPTION},
  {"dereference-command-line", no_ವಾದ, ಖಾಲಿ, 'H'},
  {"dereference-command-line-symlink-to-dir", no_ವಾದ, ಖಾಲಿ,
   DEREFERENCE_COMMAND_ಸಾಲು_SYMLINK_ಗಮ್ಯ_DIR_OPTION},
  {"hide", required_ವಾದ, ಖಾಲಿ, HIDE_OPTION},
  {"ignore", required_ವಾದ, ಖಾಲಿ, 'I'},
  {"indicator-style", required_ವಾದ, ಖಾಲಿ, INDICATOR_STYLE_OPTION},
  {"dereference", no_ವಾದ, ಖಾಲಿ, 'L'},
  {"literal", no_ವಾದ, ಖಾಲಿ, 'N'},
  {"quote-name", no_ವಾದ, ಖಾಲಿ, 'Q'},
  {"quoting-style", required_ವಾದ, ಖಾಲಿ, QUOTING_STYLE_OPTION},
  {"recursive", no_ವಾದ, ಖಾಲಿ, 'R'},
  {"format", required_ವಾದ, ಖಾಲಿ, FORMAT_OPTION},
  {"show-control-chars", no_ವಾದ, ಖಾಲಿ, SHOW_CONTROL_CHARS_OPTION},
  {"sort", required_ವಾದ, ಖಾಲಿ, ವಿಂಗಡಿಸು_OPTION},
  {"tabsize", required_ವಾದ, ಖಾಲಿ, 'T'},
  {"time", required_ವಾದ, ಖಾಲಿ, ಸಮಯ_OPTION},
  {"time-style", required_ವಾದ, ಖಾಲಿ, ಸಮಯ_STYLE_OPTION},
  {"zero", no_ವಾದ, ಖಾಲಿ, ZERO_OPTION},
  {"color", optional_ವಾದ, ಖಾಲಿ, COLOR_OPTION},
  {"hyperlink", optional_ವಾದ, ಖಾಲಿ, HYPERLINK_OPTION},
  {"block-size", required_ವಾದ, ಖಾಲಿ, ಖಂಡ_ಗಾತ್ರ_OPTION},
  {"context", no_ವಾದ, ಖಾಲಿ, 'Z'},
  {"author", no_ವಾದ, ಖಾಲಿ, AUTHOR_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸ್ಥಿರಾಂಕ format_ವಾದಗಳು[] =
{
  "verbose", "long", "commas", "horizontal", "across",
  "vertical", "single-column", ಖಾಲಿ
};
ಸ್ಥಿರ ಎಣಿಕೆ format ಸ್ಥಿರಾಂಕ format_types[] =
{
  long_format, long_format, with_commas, horizontal, horizontal,
  many_ಪ್ರತಿ_ಸಾಲು, one_ಪ್ರತಿ_ಸಾಲು
};
ARGMATCH_VERIFY (format_ವಾದಗಳು, format_types);

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸ್ಥಿರಾಂಕ ವಿಂಗಡಿಸು_ವಾದಗಳು[] =
{
  "none", "size", "time", "version", "extension",
  "name", "width", ಖಾಲಿ
};
ಸ್ಥಿರ ಎಣಿಕೆ ವಿಂಗಡಿಸು_type ಸ್ಥಿರಾಂಕ ವಿಂಗಡಿಸು_types[] =
{
  ವಿಂಗಡಿಸು_none, ವಿಂಗಡಿಸು_ಗಾತ್ರ, ವಿಂಗಡಿಸು_ಸಮಯ, ವಿಂಗಡಿಸು_ಆವೃತ್ತಿ, ವಿಂಗಡಿಸು_extension,
  ವಿಂಗಡಿಸು_name, ವಿಂಗಡಿಸು_ಅಗಲ
};
ARGMATCH_VERIFY (ವಿಂಗಡಿಸು_ವಾದಗಳು, ವಿಂಗಡಿಸು_types);

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸ್ಥಿರಾಂಕ ಸಮಯ_ವಾದಗಳು[] =
{
  "atime", "access", "use",
  "ctime", "status",
  "mtime", "modification",
  "birth", "creation",
  ಖಾಲಿ
};
ಸ್ಥಿರ ಎಣಿಕೆ ಸಮಯ_type ಸ್ಥಿರಾಂಕ ಸಮಯ_types[] =
{
  ಸಮಯ_atime, ಸಮಯ_atime, ಸಮಯ_atime,
  ಸಮಯ_ctime, ಸಮಯ_ctime,
  ಸಮಯ_mtime, ಸಮಯ_mtime,
  ಸಮಯ_btime, ಸಮಯ_btime,
};
ARGMATCH_VERIFY (ಸಮಯ_ವಾದಗಳು, ಸಮಯ_types);

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸ್ಥಿರಾಂಕ when_ವಾದಗಳು[] =
{
  /* force and none are for compatibility with another color-ls version */
  "always", "yes", "force",
  "never", "no", "none",
  "auto", "tty", "if-tty", ಖಾಲಿ
};
ಸ್ಥಿರ ಎಣಿಕೆ when_type ಸ್ಥಿರಾಂಕ when_types[] =
{
  when_always, when_always, when_always,
  when_never, when_never, when_never,
  when_if_tty, when_if_tty, when_if_tty
};
ARGMATCH_VERIFY (when_ವಾದಗಳು, when_types);

/* Information about filling a column.  */
ರಚನೆ ಸ್ತಂಭ_info
{
  bool valid_len;
  size_t ಸಾಲು_len;
  size_t *ಲಂಬಸಾಲು_arr;
};

/* Array with information about column fullness.  */
ಸ್ಥಿರ ರಚನೆ ಸ್ತಂಭ_info *ಸ್ತಂಭ_info;

/* Maximum number of columns ever possible for this display.  */
ಸ್ಥಿರ size_t max_idx;

/* The minimum width of a non-final column is 3: 1 character for the name
   and 2 for the separating white space.  The final column is not padded.  */
ಎಣಿಕೆ { MIN_ಸ್ತಂಭ_ಅಗಲ = 3 };


/* This zero-based index is for the --dired option.  It is incremented
   for each byte of output generated by this program so that the beginning
   and ending indices (in that output) of every file name can be recorded
   and later output themselves.  */
ಸ್ಥಿರ off_t dired_pos;

ಸ್ಥಿರ ಶೂನ್ಯ
dired_outbyte (ಅಕ್ಷರ c)
{
  dired_pos++;
  ಅಕ್ಷರ_ಬರೆ (c);
}

/* Output the buffer S, of length S_LEN, and increment DIRED_POS by S_LEN.  */
ಸ್ಥಿರ ಶೂನ್ಯ
dired_outbuf (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *s, size_t s_len)
{
  dired_pos += s_len;
  fwrite (s, ಗಾತ್ರ *s, s_len, stdout);
}

/* Output the string S, and increment DIRED_POS by its length.  */
ಸ್ಥಿರ ಶೂನ್ಯ
dired_outstring (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *s)
{
  dired_outbuf (s, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (s));
}

ಸ್ಥಿರ ಶೂನ್ಯ
dired_indent (ಶೂನ್ಯ)
{
  ಆದರೆ (dired)
    dired_outstring ("  ");
}

/* With --dired, store pairs of beginning and ending indices of file names.  */
ಸ್ಥಿರ ರಚನೆ obstack dired_obstack;

/* With --dired, store pairs of beginning and ending indices of any
   directory names that appear as headers (just before 'total' line)
   for lists of directory entries.  Such directory names are seen when
   listing hierarchies using -R and when a directory is listed with at
   least one other command line argument.  */
ಸ್ಥಿರ ರಚನೆ obstack subdired_obstack;

/* Save the current index on the specified obstack, OBS.  */
ಸ್ಥಿರ ಶೂನ್ಯ
ತಳ್ಳು_current_dired_pos (ರಚನೆ obstack *obs)
{
  ಆದರೆ (dired)
    obstack_grow (obs, &dired_pos, ಗಾತ್ರ dired_pos);
}

/* With -R, this stack is used to help detect directory cycles.
   The device/inode pairs on this stack mirror the pairs in the
   active_dir_set hash table.  */
ಸ್ಥಿರ ರಚನೆ obstack dev_ino_obstack;

/* Push a pair onto the device/inode stack.  */
ಸ್ಥಿರ ಶೂನ್ಯ
dev_ino_ತಳ್ಳು (dev_t dev, ino_t ino)
{
  ಶೂನ್ಯ *vdi;
  ರಚನೆ dev_ino *di;
  ಪೂರ್ಣಾಂಕ dev_ino_ಗಾತ್ರ = ಗಾತ್ರ *di;
  obstack_blank (&dev_ino_obstack, dev_ino_ಗಾತ್ರ);
  vdi = obstack_ಮುಂದಿನ_free (&dev_ino_obstack);
  di = vdi;
  di--;
  di->st_dev = dev;
  di->st_ino = ino;
}

/* Pop a dev/ino struct off the global dev_ino_obstack
   and return that struct.  */
ಸ್ಥಿರ ರಚನೆ dev_ino
dev_ino_ತೆಗೆ (ಶೂನ್ಯ)
{
  ಶೂನ್ಯ *vdi;
  ರಚನೆ dev_ino *di;
  ಪೂರ್ಣಾಂಕ dev_ino_ಗಾತ್ರ = ಗಾತ್ರ *di;
  affirm (dev_ino_ಗಾತ್ರ <= obstack_object_ಗಾತ್ರ (&dev_ino_obstack));
  obstack_blank_fast (&dev_ino_obstack, -dev_ino_ಗಾತ್ರ);
  vdi = obstack_ಮುಂದಿನ_free (&dev_ino_obstack);
  di = vdi;
  ಹಿಂತಿರುಗಿ *di;
}

ಸ್ಥಿರ ಶೂನ್ಯ
assert_matching_dev_ino (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name, ರಚನೆ dev_ino di)
{
  MAYBE_ಬಳಸದ ರಚನೆ stat sb;
  assure (0 <= stat (name, &sb));
  assure (sb.st_dev == di.st_dev);
  assure (sb.st_ino == di.st_ino);
}

ಸ್ಥಿರ ಅಕ್ಷರ eolbyte = '\n';

/* Write to standard output PREFIX, followed by the quoting style and
   a space-separated list of the integers stored in OS all on one line.  */

ಸ್ಥಿರ ಶೂನ್ಯ
dired_dump_obstack (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಪೂರ್ವಪ್ರತ್ಯಯ, ರಚನೆ obstack *os)
{
  size_t n_pos;

  n_pos = obstack_object_ಗಾತ್ರ (os) / ಗಾತ್ರ (dired_pos);
  ಆದರೆ (n_pos > 0)
    {
      off_t *pos = obstack_ಮುಕ್ತಾಯ (os);
      fputs (ಪೂರ್ವಪ್ರತ್ಯಯ, stdout);
      ಗಾಗಿ (size_t i = 0; i < n_pos; i++)
        {
          intmax_t p = pos[i];
          ಬರೆ (" %jd", p);
        }
      ಅಕ್ಷರ_ಬರೆ ('\n');
    }
}

/* Return the platform birthtime member of the stat structure,
   or fallback to the mtime member, which we have populated
   from the statx structure or reset to an invalid timestamp
   where birth time is not supported.  */
ಸ್ಥಿರ ರಚನೆ timespec
get_stat_btime (ರಚನೆ stat ಸ್ಥಿರಾಂಕ *st)
{
  ರಚನೆ timespec btimespec;

#ಆದರೆ HAVE_STATX && defined STATX_INO
  btimespec = get_stat_mtime (st);
#ಇಲ್ಲದಿದ್ದರೆ
  btimespec = get_stat_birthtime (st);
#ಆದರೆ_ಅಂತ್ಯ

  ಹಿಂತಿರುಗಿ btimespec;
}

#ಆದರೆ HAVE_STATX && defined STATX_INO
ATTRIBUTE_ಶುದ್ಧ
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ
ಸಮಯ_type_ಗಮ್ಯ_statx (ಶೂನ್ಯ)
{
  ಆಯ್ಕೆ (ಸಮಯ_type)
    {
    ಸ್ಥಿತಿ ಸಮಯ_ctime:
      ಹಿಂತಿರುಗಿ STATX_CTIME;
    ಸ್ಥಿತಿ ಸಮಯ_mtime:
      ಹಿಂತಿರುಗಿ STATX_MTIME;
    ಸ್ಥಿತಿ ಸಮಯ_atime:
      ಹಿಂತಿರುಗಿ STATX_ATIME;
    ಸ್ಥಿತಿ ಸಮಯ_btime:
      ಹಿಂತಿರುಗಿ STATX_BTIME;
    ಸ್ಥಿತಿ ಸಮಯ_numtypes: ಪೂರ್ವನಿಯೋಜಿತ:
      unreachable ();
    }
    ಹಿಂತಿರುಗಿ 0;
}

ATTRIBUTE_ಶುದ್ಧ
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ
calc_req_ಮುಖವಾಡ (ಶೂನ್ಯ)
{
  ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಮುಖವಾಡ = STATX_ಬಹುಲಕ;

  ಆದರೆ (print_inode)
    ಮುಖವಾಡ |= STATX_INO;

  ಆದರೆ (print_ಖಂಡ_ಗಾತ್ರ)
    ಮುಖವಾಡ |= STATX_ಖಂಡಗಳು;

  ಆದರೆ (format == long_format) {
    ಮುಖವಾಡ |= STATX_NLINK | STATX_ಗಾತ್ರ | ಸಮಯ_type_ಗಮ್ಯ_statx ();
    ಆದರೆ (print_owner || print_author)
      ಮುಖವಾಡ |= STATX_UID;
    ಆದರೆ (print_ಗುಂಪು)
      ಮುಖವಾಡ |= STATX_GID;
  }

  ಆಯ್ಕೆ (ವಿಂಗಡಿಸು_type)
    {
    ಸ್ಥಿತಿ ವಿಂಗಡಿಸು_none:
    ಸ್ಥಿತಿ ವಿಂಗಡಿಸು_name:
    ಸ್ಥಿತಿ ವಿಂಗಡಿಸು_ಆವೃತ್ತಿ:
    ಸ್ಥಿತಿ ವಿಂಗಡಿಸು_extension:
    ಸ್ಥಿತಿ ವಿಂಗಡಿಸು_ಅಗಲ:
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ ವಿಂಗಡಿಸು_ಸಮಯ:
      ಮುಖವಾಡ |= ಸಮಯ_type_ಗಮ್ಯ_statx ();
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ ವಿಂಗಡಿಸು_ಗಾತ್ರ:
      ಮುಖವಾಡ |= STATX_ಗಾತ್ರ;
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ ವಿಂಗಡಿಸು_numtypes: ಪೂರ್ವನಿಯೋಜಿತ:
      unreachable ();
    }

  ಹಿಂತಿರುಗಿ ಮುಖವಾಡ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
do_statx (ಪೂರ್ಣಾಂಕ fd, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name, ರಚನೆ stat *st, ಪೂರ್ಣಾಂಕ flags,
          ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಮುಖವಾಡ)
{
  ರಚನೆ statx stx;
  bool want_btime = ಮುಖವಾಡ & STATX_BTIME;
  ಪೂರ್ಣಾಂಕ ret = statx (fd, name, flags | AT_NO_AUTOMOUNT, ಮುಖವಾಡ, &stx);
  ಆದರೆ (ret >= 0)
    {
      statx_ಗಮ್ಯ_stat (&stx, st);
      /* Since we only need one timestamp type,
         store birth time in st_mtim.  */
      ಆದರೆ (want_btime)
        {
          ಆದರೆ (stx.stx_ಮುಖವಾಡ & STATX_BTIME)
            st->st_mtim = statx_timestamp_ಗಮ್ಯ_timespec (stx.stx_btime);
          ಇಲ್ಲದಿದ್ದರೆ
            st->st_mtim.tv_sec = st->st_mtim.tv_nsec = -1;
        }
    }

  ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
do_stat (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name, ರಚನೆ stat *st)
{
  ಹಿಂತಿರುಗಿ do_statx (AT_FDCWD, name, st, 0, calc_req_ಮುಖವಾಡ ());
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
do_lstat (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name, ರಚನೆ stat *st)
{
  ಹಿಂತಿರುಗಿ do_statx (AT_FDCWD, name, st, AT_SYMLINK_NOFOLLOW, calc_req_ಮುಖವಾಡ ());
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
stat_for_ಬಹುಲಕ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name, ರಚನೆ stat *st)
{
  ಹಿಂತಿರುಗಿ do_statx (AT_FDCWD, name, st, 0, STATX_ಬಹುಲಕ);
}

/* dev+ino should be static, so no need to sync with backing store */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
stat_for_ino (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name, ರಚನೆ stat *st)
{
  ಹಿಂತಿರುಗಿ do_statx (AT_FDCWD, name, st, 0, STATX_INO);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
fstat_for_ino (ಪೂರ್ಣಾಂಕ fd, ರಚನೆ stat *st)
{
  ಹಿಂತಿರುಗಿ do_statx (fd, "", st, AT_ಖಾಲಿಗಳು_ಮಾರ್ಗ, STATX_INO);
}
#ಇಲ್ಲದಿದ್ದರೆ
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
do_stat (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name, ರಚನೆ stat *st)
{
  ಹಿಂತಿರುಗಿ stat (name, st);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
do_lstat (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name, ರಚನೆ stat *st)
{
  ಹಿಂತಿರುಗಿ lstat (name, st);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
stat_for_ಬಹುಲಕ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name, ರಚನೆ stat *st)
{
  ಹಿಂತಿರುಗಿ stat (name, st);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
stat_for_ino (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name, ರಚನೆ stat *st)
{
  ಹಿಂತಿರುಗಿ stat (name, st);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
fstat_for_ino (ಪೂರ್ಣಾಂಕ fd, ರಚನೆ stat *st)
{
  ಹಿಂತಿರುಗಿ fstat (fd, st);
}
#ಆದರೆ_ಅಂತ್ಯ

/* Return the address of the first plain %b spec in FMT, or NULL if
   there is no such spec.  %5b etc. do not match, so that user
   widths/flags are honored.  */

ATTRIBUTE_ಶುದ್ಧ
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *
ಮೊದಲ_percent_b (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *fmt)
{
  ಗಾಗಿ (; *fmt; fmt++)
    ಆದರೆ (fmt[0] == '%')
      ಆಯ್ಕೆ (fmt[1])
        {
        ಸ್ಥಿತಿ 'b': ಹಿಂತಿರುಗಿ fmt;
        ಸ್ಥಿತಿ '%': fmt++; ನಿಲ್ಲಿಸು;
        }
  ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಸ್ಥಿರ ಅಕ್ಷರ RFC3986[256];
ಸ್ಥಿರ ಶೂನ್ಯ
file_escape_init (ಶೂನ್ಯ)
{
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < 256; i++)
    RFC3986[i] |= c_isalnum (i) || i == '~' || i == '-' || i == '.' || i == '_';
}

ಎಣಿಕೆ { MBSWIDTH_FLAGS = MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE };

/* Read the abbreviated month names from the locale, to align them
   and to determine the max width of the field and to truncate names
   greater than our max allowed.
   Note even though this handles multibyte locales correctly
   it's not restricted to them as single byte locales can have
   variable width abbreviated months and also precomputing/caching
   the names was seen to increase the performance of ls significantly.  */

/* abformat[RECENT][MON] is the format to use for timestamps with
   recentness RECENT and month MON.  */
ಎಣಿಕೆ { ABFORMAT_ಗಾತ್ರ = 128 };
ಸ್ಥಿರ ಅಕ್ಷರ abformat[2][12][ABFORMAT_ಗಾತ್ರ];
/* True if precomputed formats should be used.  This can be false if
   nl_langinfo fails, if a format or month abbreviation is unusually
   long, or if a month abbreviation contains '%'.  */
ಸ್ಥಿರ bool use_abformat;

/* Store into ABMON the abbreviated month names, suitably aligned.
   Return true if successful.  */

ಸ್ಥಿರ bool
abmon_init (ಅಕ್ಷರ abmon[12][ABFORMAT_ಗಾತ್ರ])
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_NL_LANGINFO
  ಹಿಂತಿರುಗಿ false;
#ಇಲ್ಲದಿದ್ದರೆ
  ಪೂರ್ಣಾಂಕ max_mon_ಅಗಲ = 0;
  ಪೂರ್ಣಾಂಕ mon_ಅಗಲ[12];
  ಪೂರ್ಣಾಂಕ mon_len[12];

  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < 12; i++)
    {
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *abbr = nl_langinfo (ABMON_1 + i);
      mon_len[i] = strnlen (abbr, ABFORMAT_ಗಾತ್ರ);
      ಆದರೆ (mon_len[i] == ABFORMAT_ಗಾತ್ರ)
        ಹಿಂತಿರುಗಿ false;
      ಆದರೆ (strchr (abbr, '%'))
        ಹಿಂತಿರುಗಿ false;
      mon_ಅಗಲ[i] = mbswidth (strcpy (abmon[i], abbr), MBSWIDTH_FLAGS);
      ಆದರೆ (mon_ಅಗಲ[i] < 0)
        ಹಿಂತಿರುಗಿ false;
      max_mon_ಅಗಲ = MAX (max_mon_ಅಗಲ, mon_ಅಗಲ[i]);
    }

  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < 12; i++)
    {
      ಪೂರ್ಣಾಂಕ fill = max_mon_ಅಗಲ - mon_ಅಗಲ[i];
      ಆದರೆ (ABFORMAT_ಗಾತ್ರ - mon_len[i] <= fill)
        ಹಿಂತಿರುಗಿ false;
      bool align_ಎಡ = !c_isdigit (abmon[i][0]);
      ಪೂರ್ಣಾಂಕ fill_ಸ್ಥಾನಾಂತರ;
      ಆದರೆ (align_ಎಡ)
        fill_ಸ್ಥಾನಾಂತರ = mon_len[i];
      ಇಲ್ಲದಿದ್ದರೆ
        {
          memmove (abmon[i] + fill, abmon[i], mon_len[i]);
          fill_ಸ್ಥಾನಾಂತರ = 0;
        }
      memset (abmon[i] + fill_ಸ್ಥಾನಾಂತರ, ' ', fill);
      abmon[i][mon_len[i] + fill] = '\0';
    }

  ಹಿಂತಿರುಗಿ true;
#ಆದರೆ_ಅಂತ್ಯ
}

/* Initialize ABFORMAT and USE_ABFORMAT.  */

ಸ್ಥಿರ ಶೂನ್ಯ
abformat_init (ಶೂನ್ಯ)
{
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *pb[2];
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ recent = 0; recent < 2; recent++)
    pb[recent] = ಮೊದಲ_percent_b (long_ಸಮಯ_format[recent]);
  ಆದರೆ (! (pb[0] || pb[1]))
    ಹಿಂತಿರುಗಿ;

  ಅಕ್ಷರ abmon[12][ABFORMAT_ಗಾತ್ರ];
  ಆದರೆ (! abmon_init (abmon))
    ಹಿಂತಿರುಗಿ;

  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ recent = 0; recent < 2; recent++)
    {
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *fmt = long_ಸಮಯ_format[recent];
      ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < 12; i++)
        {
          ಅಕ್ಷರ *nfmt = abformat[recent][i];
          ಪೂರ್ಣಾಂಕ nbytes;

          ಆದರೆ (! pb[recent])
            nbytes = snprintf (nfmt, ABFORMAT_ಗಾತ್ರ, "%s", fmt);
          ಇಲ್ಲದಿದ್ದರೆ
            {
              ಆದರೆ (! (pb[recent] - fmt <= MIN (ABFORMAT_ಗಾತ್ರ, ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ)))
                ಹಿಂತಿರುಗಿ;
              ಪೂರ್ಣಾಂಕ ಪೂರ್ವಪ್ರತ್ಯಯ_len = pb[recent] - fmt;
              nbytes = snprintf (nfmt, ABFORMAT_ಗಾತ್ರ, "%.*s%s%s",
                                 ಪೂರ್ವಪ್ರತ್ಯಯ_len, fmt, abmon[i], pb[recent] + 2);
            }

          ಆದರೆ (! (0 <= nbytes && nbytes < ABFORMAT_ಗಾತ್ರ))
            ಹಿಂತಿರುಗಿ;
        }
    }

  use_abformat = true;
}

ಸ್ಥಿರ size_t
dev_ino_hash (ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *x, size_t ಕೋಷ್ಟಕ_ಗಾತ್ರ)
{
  ರಚನೆ dev_ino ಸ್ಥಿರಾಂಕ *p = x;
  ಹಿಂತಿರುಗಿ (uintmax_t) {p->st_ino} % ಕೋಷ್ಟಕ_ಗಾತ್ರ;
}

ಸ್ಥಿರ bool
dev_ino_compare (ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *x, ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *y)
{
  ರಚನೆ dev_ino ಸ್ಥಿರಾಂಕ *a = x;
  ರಚನೆ dev_ino ಸ್ಥಿರಾಂಕ *b = y;
  ಹಿಂತಿರುಗಿ PSAME_INODE (a, b);
}

ಸ್ಥಿರ ಶೂನ್ಯ
dev_ino_free (ಶೂನ್ಯ *x)
{
  ಬಿಡುಗಡೆ (x);
}

/* Add the device/inode pair (P->st_dev/P->st_ino) to the set of
   active directories.  Return true if there is already a matching
   entry in the table.  */

ಸ್ಥಿರ bool
ಭೇಟಿ_dir (dev_t dev, ino_t ino)
{
  ರಚನೆ dev_ino *ent;
  ರಚನೆ dev_ino *ent_from_ಕೋಷ್ಟಕ;
  bool ಸಿಕ್ಕಿತು_ಹೊಂದಿಕೆ;

  ent = xmalloc (ಗಾತ್ರ *ent);
  ent->st_ino = ino;
  ent->st_dev = dev;

  /* Attempt to insert this entry into the table.  */
  ent_from_ಕೋಷ್ಟಕ = hash_ಸೇರಿಸು (ಸಕ್ರಿಯ_dir_ಗಣ, ent);

  ಆದರೆ (ent_from_ಕೋಷ್ಟಕ == ಖಾಲಿ)
    {
      /* Insertion failed due to lack of memory.  */
      xalloc_die ();
    }

  ಸಿಕ್ಕಿತು_ಹೊಂದಿಕೆ = (ent_from_ಕೋಷ್ಟಕ != ent);

  ಆದರೆ (ಸಿಕ್ಕಿತು_ಹೊಂದಿಕೆ)
    {
      /* ent was not inserted, so free it.  */
      ಬಿಡುಗಡೆ (ent);
    }

  ಹಿಂತಿರುಗಿ ಸಿಕ್ಕಿತು_ಹೊಂದಿಕೆ;
}

ಸ್ಥಿರ ಶೂನ್ಯ
free_pending_ent (ರಚನೆ pending *p)
{
  ಬಿಡುಗಡೆ (p->name);
  ಬಿಡುಗಡೆ (p->realname);
  ಬಿಡುಗಡೆ (p);
}

ಸ್ಥಿರ bool
is_colored (ಎಣಿಕೆ indicator_no type)
{
  /* Return true unless the string is "", "0" or "00"; try to be efficient.  */
  size_t len = color_indicator[type].len;
  ಆದರೆ (len == 0)
    ಹಿಂತಿರುಗಿ false;
  ಆದರೆ (2 < len)
    ಹಿಂತಿರುಗಿ true;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *s = color_indicator[type].string;
  ಹಿಂತಿರುಗಿ (s[0] != '0') | (s[len - 1] != '0');
}

ಸ್ಥಿರ ಶೂನ್ಯ
restore_default_color (ಶೂನ್ಯ)
{
  put_indicator (&color_indicator[C_ಎಡ]);
  put_indicator (&color_indicator[C_ಬಲ]);
}

ಸ್ಥಿರ ಶೂನ್ಯ
ಗಣ_ಸಾಮಾನ್ಯ_color (ಶೂನ್ಯ)
{
  ಆದರೆ (print_with_color && is_colored (C_NORM))
    {
      put_indicator (&color_indicator[C_ಎಡ]);
      put_indicator (&color_indicator[C_NORM]);
      put_indicator (&color_indicator[C_ಬಲ]);
    }
}

/* An ordinary signal was received; arrange for the program to exit.  */

ಸ್ಥಿರ ಶೂನ್ಯ
sighandler (ಪೂರ್ಣಾಂಕ sig)
{
  ಆದರೆ (! ಅಡಚಣೆ_signal)
    ಅಡಚಣೆ_signal = sig;
}

/* A SIGTSTP was received; arrange for the program to suspend itself.  */

ಸ್ಥಿರ ಶೂನ್ಯ
stophandler (MAYBE_ಬಳಸದ ಪೂರ್ಣಾಂಕ sig)
{
  ಆದರೆ (! ಅಡಚಣೆ_signal)
    stop_signal_ಎಣಿಕೆ++;
}

/* Process any pending signals.  If signals are caught, this function
   should be called periodically.  Ideally there should never be an
   unbounded amount of time when signals are not being processed.
   Signal handling can restore the default colors, so callers must
   immediately change colors after invoking this function.  */

ಸ್ಥಿರ ಶೂನ್ಯ
process_ಸಂಕೇತಗಳು (ಶೂನ್ಯ)
{
  ಆಗ (ಅಡಚಣೆ_signal || stop_signal_ಎಣಿಕೆ)
    {
      ಪೂರ್ಣಾಂಕ sig;
      ಪೂರ್ಣಾಂಕ stops;
      sigset_t oldset;

      ಆದರೆ (ಬಳಸಿದ_color)
        restore_default_color ();
      fflush (stdout);

      sigprocmask (SIG_BLOCK, &caught_ಸಂಕೇತಗಳು, &oldset);

      /* Reload interrupt_signal and stop_signal_count, in case a new
         signal was handled before sigprocmask took effect.  */
      sig = ಅಡಚಣೆ_signal;
      stops = stop_signal_ಎಣಿಕೆ;

      /* SIGTSTP is special, since the application can receive that signal
         more than once.  In this case, don't set the signal handler to the
         default.  Instead, just raise the uncatchable SIGSTOP.  */
      ಆದರೆ (stops)
        {
          stop_signal_ಎಣಿಕೆ = stops - 1;
          sig = SIGSTOP;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        ಸಂಕೇತ (sig, SIG_DFL);

      /* Exit or suspend the program.  */
      ಎತ್ತು (sig);
      sigprocmask (SIG_SETMASK, &oldset, ಖಾಲಿ);

      /* If execution reaches here, then the program has been
         continued (after being suspended).  */
    }
}

/* Setup signal handlers if INIT is true,
   otherwise restore to the default.  */

ಸ್ಥಿರ ಶೂನ್ಯ
signal_ಸಿದ್ಧತೆ (bool init)
{
  /* The signals that are trapped, and the number of such signals.  */
  ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಸ್ಥಿರಾಂಕ stop_sig[] =
    {
      /* This one is handled specially.  */
      SIGTSTP,
    };

  ಎಣಿಕೆ { nsigs = countof (ಪದ_sig), nstop = countof (stop_sig) };

  ಆದರೆ (init)
    {
      ರಚನೆ sigaction act;

      sigemptyset (&caught_ಸಂಕೇತಗಳು);
      ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < nsigs + nstop; j++)
        {
          ಪೂರ್ಣಾಂಕ sig = j < nsigs ? ಪದ_sig[j]: stop_sig[j - nsigs];
          sigaction (sig, ಖಾಲಿ, &act);
          ಆದರೆ (act.sa_handler != SIG_IGN)
            sigaddset (&caught_ಸಂಕೇತಗಳು, sig);
        }

      act.sa_mask = caught_ಸಂಕೇತಗಳು;
      act.sa_flags = SA_RESTART;

      ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < nsigs + nstop; j++)
        {
          ಪೂರ್ಣಾಂಕ sig = j < nsigs ? ಪದ_sig[j]: stop_sig[j - nsigs];
          ಆದರೆ (sigismember (&caught_ಸಂಕೇತಗಳು, sig))
            {
              act.sa_handler = j < nsigs ? sighandler : stophandler;
              sigaction (sig, &act, ಖಾಲಿ);
            }
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ /* restore.  */
    {
      ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < nsigs + nstop; j++)
        {
          ಪೂರ್ಣಾಂಕ sig = j < nsigs ? ಪದ_sig[j]: stop_sig[j - nsigs];
          ಆದರೆ (sigismember (&caught_ಸಂಕೇತಗಳು, sig))
            ಸಂಕೇತ (sig, SIG_DFL);
        }
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
signal_init (ಶೂನ್ಯ)
{
  signal_ಸಿದ್ಧತೆ (true);
}

ಸ್ಥಿರ ಶೂನ್ಯ
signal_restore (ಶೂನ್ಯ)
{
  signal_ಸಿದ್ಧತೆ (false);
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  ಪೂರ್ಣಾಂಕ i;
  ರಚನೆ pending *thispend;
  ಪೂರ್ಣಾಂಕ n_files;

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (LS_FAILURE);
  atexit (close_stdout);

  static_assert (countof (color_indicator) == countof (indicator_name));

  exit_ಸ್ಥಿತಿ = EXIT_SUCCESS;
  print_dir_name = true;
  pending_dirs = ಖಾಲಿ;

  current_ಸಮಯ.tv_sec = TYPE_MINIMUM (time_t);
  current_ಸಮಯ.tv_nsec = -1;

  i = decode_switches (argc, argv);

  ಆದರೆ (print_with_color)
    parse_ls_color ();

  /* Test print_with_color again, because the call to parse_ls_color
     may have just reset it -- e.g., if LS_COLORS is invalid.  */

  ಆದರೆ (print_with_color)
    {
      /* Don't use TAB characters in output.  Some terminal
         emulators can't handle the combination of tabs and
         color codes on the same line.  */
      tabsize = 0;
    }

  ಆದರೆ (directories_ಮೊದಲ)
    check_symlink_ಬಹುಲಕ = true;
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (print_with_color)
    {
      /* Avoid following symbolic links when possible.  */
      ಆದರೆ (is_colored (C_ORPHAN)
          || (is_colored (C_EXEC) && color_symlink_as_referent)
          || (is_colored (C_MISSING) && format == long_format))
        check_symlink_ಬಹುಲಕ = true;
    }

  ಆದರೆ (dereference == DEREF_UNDEFINED)
    dereference = ((ತಕ್ಷಣ_dirs
                    || indicator_style == classify
                    || format == long_format)
                   ? DEREF_NEVER
                   : DEREF_COMMAND_ಸಾಲು_SYMLINK_ಗಮ್ಯ_DIR);

  /* When using -R, initialize a data structure we'll use to
     detect any directory cycles.  */
  ಆದರೆ (recursive)
    {
      ಸಕ್ರಿಯ_dir_ಗಣ = hash_initialize (INITIAL_ಕೋಷ್ಟಕ_ಗಾತ್ರ, ಖಾಲಿ,
                                        dev_ino_hash,
                                        dev_ino_compare,
                                        dev_ino_free);
      ಆದರೆ (ಸಕ್ರಿಯ_dir_ಗಣ == ಖಾಲಿ)
        xalloc_die ();

      obstack_init (&dev_ino_obstack);
    }

  localtz = tzalloc (getenv ("TZ"));

  format_needs_stat = ((ವಿಂಗಡಿಸು_type == ವಿಂಗಡಿಸು_ಸಮಯ) | (ವಿಂಗಡಿಸು_type == ವಿಂಗಡಿಸು_ಗಾತ್ರ)
                       | (format == long_format)
                       | print_ಖಂಡ_ಗಾತ್ರ | print_hyperlink | print_scontext);
  format_needs_type = ((! format_needs_stat)
                       & (recursive | print_with_color | print_scontext
                          | directories_ಮೊದಲ
                          | (indicator_style != none)));
  format_needs_capability = print_with_color && is_colored (C_CAP);

  ಆದರೆ (dired)
    {
      obstack_init (&dired_obstack);
      obstack_init (&subdired_obstack);
    }

  ಆದರೆ (print_hyperlink)
    {
      file_escape_init ();

      hostname = xgethostname ();
      /* The hostname is generally ignored,
         so ignore failures obtaining it.  */
      ಆದರೆ (! hostname)
        hostname = "";
    }

  cwd_n_alloc = 100;
  cwd_file = xmalloc (cwd_n_alloc * ಗಾತ್ರ *cwd_file);
  cwd_n_ಬಳಸಿದ = 0;

  clear_files ();

  n_files = argc - i;

  ಆದರೆ (n_files <= 0)
    {
      ಆದರೆ (ತಕ್ಷಣ_dirs)
        gobble_file (".", directory, NOT_AN_INODE_ಸಂಖ್ಯೆ, true, ಖಾಲಿ);
      ಇಲ್ಲದಿದ್ದರೆ
        ಸರತಿ_directory (".", ಖಾಲಿ, true);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    ಮಾಡು
      gobble_file (argv[i++], unknown, NOT_AN_INODE_ಸಂಖ್ಯೆ, true, ಖಾಲಿ);
    ಆಗ (i < argc);

  ಆದರೆ (cwd_n_ಬಳಸಿದ)
    {
      ವಿಂಗಡಿಸು_files ();
      ಆದರೆ (!ತಕ್ಷಣ_dirs)
        ಹೊರತೆಗೆ_dirs_from_files (ಖಾಲಿ, true);
      /* 'cwd_n_used' might be zero now.  */
    }

  /* In the following if/else blocks, it is sufficient to test 'pending_dirs'
     (and not pending_dirs->name) because there may be no markers in the queue
     at this point.  A marker may be enqueued when extract_dirs_from_files is
     called with a non-empty string or via print_dir.  */
  ಆದರೆ (cwd_n_ಬಳಸಿದ)
    {
      print_current_files ();
      ಆದರೆ (pending_dirs)
        dired_outbyte ('\n');
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (n_files <= 1 && pending_dirs && !pending_dirs->ಮುಂದಿನ)
    print_dir_name = false;

  ಆಗ (pending_dirs)
    {
      thispend = pending_dirs;
      pending_dirs = pending_dirs->ಮುಂದಿನ;

      ಆದರೆ (LOOP_DETECT)
        {
          ಆದರೆ (thispend->name == ಖಾಲಿ)
            {
              /* thispend->name == NULL means this is a marker entry
                 indicating we've finished processing the directory.
                 Use its dev/ino numbers to remove the corresponding
                 entry from the active_dir_set hash table.  */
              ರಚನೆ dev_ino di = dev_ino_ತೆಗೆ ();
              ರಚನೆ dev_ino *ಸಿಕ್ಕಿತು = hash_remove (ಸಕ್ರಿಯ_dir_ಗಣ, &di);
              ಆದರೆ (false)
                assert_matching_dev_ino (thispend->realname, di);
              affirm (ಸಿಕ್ಕಿತು);
              dev_ino_free (ಸಿಕ್ಕಿತು);
              free_pending_ent (thispend);
              ಮುಂದುವರಿ;
            }
        }

      print_dir (thispend->name, thispend->realname,
                 thispend->command_ಸಾಲು_arg);

      free_pending_ent (thispend);
      print_dir_name = true;
    }

  ಆದರೆ (print_with_color && ಬಳಸಿದ_color)
    {

      /* Skip the restore when it would be a no-op, i.e.,
         when left is "\033[" and right is "m".  */
      ಆದರೆ (!(color_indicator[C_ಎಡ].len == 2
            && memeq (color_indicator[C_ಎಡ].string, "\033[", 2)
            && color_indicator[C_ಬಲ].len == 1
            && color_indicator[C_ಬಲ].string[0] == 'm'))
        restore_default_color ();

      fflush (stdout);

      signal_restore ();

      /* Act on any signals that arrived before the default was restored.
         This can process signals out of order, but there doesn't seem to
         be an easy way to do them in order, and the order isn't that
         important anyway.  */
      ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = stop_signal_ಎಣಿಕೆ; j; j--)
        ಎತ್ತು (SIGSTOP);
      ಪೂರ್ಣಾಂಕ j = ಅಡಚಣೆ_signal;
      ಆದರೆ (j)
        ಎತ್ತು (j);
    }

  ಆದರೆ (dired)
    {
      /* No need to free these since we're about to exit.  */
      dired_dump_obstack ("//DIRED//", &dired_obstack);
      dired_dump_obstack ("//SUBDIRED//", &subdired_obstack);
      ಬರೆ ("//DIRED-OPTIONS// --quoting-style=%s\n",
              quoting_style_ವಾದಗಳು[get_quoting_style (filename_quoting_options)]);
    }

  ಆದರೆ (LOOP_DETECT)
    {
      assure (hash_get_n_entries (ಸಕ್ರಿಯ_dir_ಗಣ) == 0);
      hash_free (ಸಕ್ರಿಯ_dir_ಗಣ);
    }

  ಹಿಂತಿರುಗಿ exit_ಸ್ಥಿತಿ;
}

/* Return the line length indicated by the value given by SPEC, or -1
   if unsuccessful.  0 means no limit on line length.  */

ಸ್ಥಿರ ptrdiff_t
decode_ಸಾಲು_ಉದ್ದಳತೆ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *spec)
{
  uintmax_t val;

  /* Treat too-large values as if they were 0, which is
     effectively infinity.  */
  ಆಯ್ಕೆ (xstrtoumax (spec, ಖಾಲಿ, 0, &val, ""))
    {
    ಸ್ಥಿತಿ LONGINT_OK:
      ಹಿಂತಿರುಗಿ val <= MIN (PTRDIFF_MAX, SIZE_MAX) ? val : 0;

    ಸ್ಥಿತಿ LONGINT_ಅತಿಪ್ರವಾಹ:
      ಹಿಂತಿರುಗಿ 0;

    ಸ್ಥಿತಿ LONGINT_INVALID:
    ಸ್ಥಿತಿ LONGINT_INVALID_SUFFIX_CHAR:
    ಸ್ಥಿತಿ LONGINT_INVALID_SUFFIX_CHAR_WITH_ಅತಿಪ್ರವಾಹ:
      ಹಿಂತಿರುಗಿ -1;

    ಪೂರ್ವನಿಯೋಜಿತ:
      unreachable ();
    }
}

/* Return true if standard output is a tty, caching the result.  */

ಸ್ಥಿರ bool
stdout_isatty (ಶೂನ್ಯ)
{
  ಸ್ಥಿರ ಚಿಹ್ನೆಯುಳ್ಳ ಅಕ್ಷರ ಹೊರಹೋಗು_tty = -1;
  ಆದರೆ (ಹೊರಹೋಗು_tty < 0)
    ಹೊರಹೋಗು_tty = isatty (STDOUT_FILENO);
  assume (ಹೊರಹೋಗು_tty == 0 || ಹೊರಹೋಗು_tty == 1);
  ಹಿಂತಿರುಗಿ ಹೊರಹೋಗು_tty;
}

/* Set all the option flags according to the switches specified.
   Return the index of the first non-option argument.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
decode_switches (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  ಅಕ್ಷರ *ಸಮಯ_style_option = ಖಾಲಿ;

  /* These variables are false or -1 unless a switch says otherwise.  */
  bool kibibytes_specified = false;
  ಪೂರ್ಣಾಂಕ format_opt = -1;
  ಪೂರ್ಣಾಂಕ hide_control_chars_opt = -1;
  ಪೂರ್ಣಾಂಕ quoting_style_opt = -1;
  ಪೂರ್ಣಾಂಕ ವಿಂಗಡಿಸು_opt = -1;
  ptrdiff_t tabsize_opt = -1;
  ptrdiff_t ಅಗಲ_opt = -1;

  ಆಗ (true)
    {
      ಪೂರ್ಣಾಂಕ oi = -1;
      ಪೂರ್ಣಾಂಕ c = getopt_long (argc, argv,
                           "abcdfghiklmnopqrstuvw:xABCDFGHI:LNQRST:UXZ1",
                           long_options, &oi);
      ಆದರೆ (c == -1)
        ನಿಲ್ಲಿಸು;

      ಆಯ್ಕೆ (c)
        {
        ಸ್ಥಿತಿ 'a':
          ignore_ಬಹುಲಕ = IGNORE_ಕನಿಷ್ಠತಮ;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'b':
          quoting_style_opt = escape_quoting_style;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'c':
          ಸಮಯ_type = ಸಮಯ_ctime;
          explicit_ಸಮಯ = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'd':
          ತಕ್ಷಣ_dirs = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'f':
          ignore_ಬಹುಲಕ = IGNORE_ಕನಿಷ್ಠತಮ; /* enable -a */
          ವಿಂಗಡಿಸು_opt = ವಿಂಗಡಿಸು_none;         /* enable -U */
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ FILE_TYPE_INDICATOR_OPTION: /* --file-type */
          indicator_style = file_type;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'g':
          format_opt = long_format;
          print_owner = false;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'h':
          file_human_output_opts = human_output_opts =
            human_autoscale | human_SI | human_ತಳ_1024;
          file_output_ಖಂಡ_ಗಾತ್ರ = output_ಖಂಡ_ಗಾತ್ರ = 1;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'i':
          print_inode = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'k':
          kibibytes_specified = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'l':
          format_opt = long_format;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'm':
          format_opt = with_commas;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'n':
          numeric_ids = true;
          format_opt = long_format;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'o':  /* Just like -l, but don't display group info.  */
          format_opt = long_format;
          print_ಗುಂಪು = false;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'p':
          indicator_style = slash;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'q':
          hide_control_chars_opt = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'r':
          ವಿಂಗಡಿಸು_ಹಿಮ್ಮುಖ = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 's':
          print_ಖಂಡ_ಗಾತ್ರ = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 't':
          ವಿಂಗಡಿಸು_opt = ವಿಂಗಡಿಸು_ಸಮಯ;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'u':
          ಸಮಯ_type = ಸಮಯ_atime;
          explicit_ಸಮಯ = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'v':
          ವಿಂಗಡಿಸು_opt = ವಿಂಗಡಿಸು_ಆವೃತ್ತಿ;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'w':
          ಅಗಲ_opt = decode_ಸಾಲು_ಉದ್ದಳತೆ (optarg);
          ಆದರೆ (ಅಗಲ_opt < 0)
            error (LS_FAILURE, 0, "%s: %s", _("invalid line width"),
                   quote (optarg));
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'x':
          format_opt = horizontal;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'A':
          ignore_ಬಹುಲಕ = IGNORE_DOT_AND_DOTDOT;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'B':
          ಕೂಡಿಸು_ignore_ನಮೂನೆ ("*~");
          ಕೂಡಿಸು_ignore_ನಮೂನೆ (".*~");
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'C':
          format_opt = many_ಪ್ರತಿ_ಸಾಲು;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'D':
          format_opt = long_format;
          print_hyperlink = false;
          dired = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'F':
          {
            ಪೂರ್ಣಾಂಕ i;
            ಆದರೆ (optarg)
              i = XARGMATCH ("--classify", optarg, when_ವಾದಗಳು, when_types);
            ಇಲ್ಲದಿದ್ದರೆ
              /* Using --classify with no argument is equivalent to using
                 --classify=always.  */
              i = when_always;

            ಆದರೆ (i == when_always || (i == when_if_tty && stdout_isatty ()))
              indicator_style = classify;
            ನಿಲ್ಲಿಸು;
          }

        ಸ್ಥಿತಿ 'G':		/* inhibit display of group info */
          print_ಗುಂಪು = false;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'H':
          dereference = DEREF_COMMAND_ಸಾಲು_ARGUMENTS;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ DEREFERENCE_COMMAND_ಸಾಲು_SYMLINK_ಗಮ್ಯ_DIR_OPTION:
          dereference = DEREF_COMMAND_ಸಾಲು_SYMLINK_ಗಮ್ಯ_DIR;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'I':
          ಕೂಡಿಸು_ignore_ನಮೂನೆ (optarg);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'L':
          dereference = DEREF_ALWAYS;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'N':
          quoting_style_opt = literal_quoting_style;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'Q':
          quoting_style_opt = c_quoting_style;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'R':
          recursive = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'S':
          ವಿಂಗಡಿಸು_opt = ವಿಂಗಡಿಸು_ಗಾತ್ರ;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'T':
          tabsize_opt = xnumtoumax (optarg, 0, 0, MIN (PTRDIFF_MAX, SIZE_MAX),
                                    "", _("invalid tab size"), LS_FAILURE, 0);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'U':
          ವಿಂಗಡಿಸು_opt = ವಿಂಗಡಿಸು_none;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'X':
          ವಿಂಗಡಿಸು_opt = ವಿಂಗಡಿಸು_extension;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ '1':
          /* -1 has no effect after -l.  */
          ಆದರೆ (format_opt != long_format)
            format_opt = one_ಪ್ರತಿ_ಸಾಲು;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ AUTHOR_OPTION:
          print_author = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ HIDE_OPTION:
          {
            ರಚನೆ ignore_ನಮೂನೆ *hide = xmalloc (ಗಾತ್ರ *hide);
            hide->ನಮೂನೆ_2 = optarg;
            hide->ಮುಂದಿನ = hide_patterns;
            hide_patterns = hide;
          }
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ ವಿಂಗಡಿಸು_OPTION:
          ವಿಂಗಡಿಸು_opt = XARGMATCH ("--sort", optarg, ವಿಂಗಡಿಸು_ವಾದಗಳು, ವಿಂಗಡಿಸು_types);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ ಗುಂಪು_DIRECTORIES_ಮೊದಲ_OPTION:
          directories_ಮೊದಲ = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ ಸಮಯ_OPTION:
          ಸಮಯ_type = XARGMATCH ("--time", optarg, ಸಮಯ_ವಾದಗಳು, ಸಮಯ_types);
          explicit_ಸಮಯ = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ FORMAT_OPTION:
          format_opt = XARGMATCH ("--format", optarg, format_ವಾದಗಳು,
                                  format_types);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ ತುಂಬಿದ_ಸಮಯ_OPTION:
          format_opt = long_format;
          ಸಮಯ_style_option = (ಅಕ್ಷರ *) "full-iso";
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ COLOR_OPTION:
          {
            ಪೂರ್ಣಾಂಕ i;
            ಆದರೆ (optarg)
              i = XARGMATCH ("--color", optarg, when_ವಾದಗಳು, when_types);
            ಇಲ್ಲದಿದ್ದರೆ
              /* Using --color with no argument is equivalent to using
                 --color=always.  */
              i = when_always;

            print_with_color = (i == when_always
                                || (i == when_if_tty && stdout_isatty ()));
            ನಿಲ್ಲಿಸು;
          }

        ಸ್ಥಿತಿ HYPERLINK_OPTION:
          {
            ಪೂರ್ಣಾಂಕ i;
            ಆದರೆ (optarg)
              i = XARGMATCH ("--hyperlink", optarg, when_ವಾದಗಳು, when_types);
            ಇಲ್ಲದಿದ್ದರೆ
              /* Using --hyperlink with no argument is equivalent to using
                 --hyperlink=always.  */
              i = when_always;

            print_hyperlink = (i == when_always
                               || (i == when_if_tty && stdout_isatty ()));
            ನಿಲ್ಲಿಸು;
          }

        ಸ್ಥಿತಿ INDICATOR_STYLE_OPTION:
          indicator_style = XARGMATCH ("--indicator-style", optarg,
                                       indicator_style_ವಾದಗಳು,
                                       indicator_style_types);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ QUOTING_STYLE_OPTION:
          quoting_style_opt = XARGMATCH ("--quoting-style", optarg,
                                         quoting_style_ವಾದಗಳು,
                                         quoting_style_vals);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ ಸಮಯ_STYLE_OPTION:
          ಸಮಯ_style_option = optarg;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ SHOW_CONTROL_CHARS_OPTION:
          hide_control_chars_opt = false;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ ಖಂಡ_ಗಾತ್ರ_OPTION:
          {
            ಎಣಿಕೆ strtol_error e = human_options (optarg, &human_output_opts,
                                                 &output_ಖಂಡ_ಗಾತ್ರ);
            ಆದರೆ (e != LONGINT_OK)
              xstrtol_fatal (e, oi, 0, long_options, optarg);
            file_human_output_opts = human_output_opts;
            file_output_ಖಂಡ_ಗಾತ್ರ = output_ಖಂಡ_ಗಾತ್ರ;
          }
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ SI_OPTION:
          file_human_output_opts = human_output_opts =
            human_autoscale | human_SI;
          file_output_ಖಂಡ_ಗಾತ್ರ = output_ಖಂಡ_ಗಾತ್ರ = 1;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'Z':
          print_scontext = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ ZERO_OPTION:
          eolbyte = 0;
          hide_control_chars_opt = false;
          ಆದರೆ (format_opt != long_format)
            format_opt = one_ಪ್ರತಿ_ಸಾಲು;
          print_with_color = false;
          quoting_style_opt = literal_quoting_style;
          ನಿಲ್ಲಿಸು;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);

        ಪೂರ್ವನಿಯೋಜಿತ:
          usage (LS_FAILURE);
        }
    }

  ಆದರೆ (! output_ಖಂಡ_ಗಾತ್ರ)
    {
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ls_ಖಂಡ_ಗಾತ್ರ = getenv ("LS_BLOCK_SIZE");
      human_options (ls_ಖಂಡ_ಗಾತ್ರ,
                     &human_output_opts, &output_ಖಂಡ_ಗಾತ್ರ);
      ಆದರೆ (ls_ಖಂಡ_ಗಾತ್ರ || getenv ("BLOCK_SIZE"))
        {
          file_human_output_opts = human_output_opts;
          file_output_ಖಂಡ_ಗಾತ್ರ = output_ಖಂಡ_ಗಾತ್ರ;
        }
      ಆದರೆ (kibibytes_specified)
        {
          human_output_opts = 0;
          output_ಖಂಡ_ಗಾತ್ರ = 1024;
        }
    }

  format = (0 <= format_opt ? format_opt
            : ls_ಬಹುಲಕ == LS_LS ? (stdout_isatty ()
                                  ? many_ಪ್ರತಿ_ಸಾಲು : one_ಪ್ರತಿ_ಸಾಲು)
            : ls_ಬಹುಲಕ == LS_MULTI_ಲಂಬಸಾಲು ? many_ಪ್ರತಿ_ಸಾಲು
            : /* ls_mode == LS_LONG_FORMAT */ long_format);

  /* If the line length was not set by a switch but is needed to determine
     output, go to the work of obtaining it from the environment.  */
  ptrdiff_t linelen = ಅಗಲ_opt;
  ಆದರೆ (format == many_ಪ್ರತಿ_ಸಾಲು || format == horizontal || format == with_commas
      || print_with_color)
    {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ TIOCGWINSZ
      ಆದರೆ (linelen < 0)
        {
          ರಚನೆ winsize ws;
          ಆದರೆ (stdout_isatty ()
              && 0 <= ioctl (STDOUT_FILENO, TIOCGWINSZ, &ws)
              && 0 < ws.ws_ಲಂಬಸಾಲು)
            linelen = ws.ws_ಲಂಬಸಾಲು <= MIN (PTRDIFF_MAX, SIZE_MAX) ? ws.ws_ಲಂಬಸಾಲು : 0;
        }
#ಆದರೆ_ಅಂತ್ಯ
      ಆದರೆ (linelen < 0)
        {
          ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *p = getenv ("COLUMNS");
          ಆದರೆ (p && *p)
            {
              linelen = decode_ಸಾಲು_ಉದ್ದಳತೆ (p);
              ಆದರೆ (linelen < 0)
                error (0, 0,
                       _("ignoring invalid width"
                         " in environment variable COLUMNS: %s"),
                       quote (p));
            }
        }
    }

  ಸಾಲು_ಉದ್ದಳತೆ = linelen < 0 ? 80 : linelen;

  /* Determine the max possible number of display columns.  */
  max_idx = ಸಾಲು_ಉದ್ದಳತೆ / MIN_ಸ್ತಂಭ_ಅಗಲ;
  /* Account for first display column not having a separator,
     or line_lengths shorter than MIN_COLUMN_WIDTH.  */
  max_idx += ಸಾಲು_ಉದ್ದಳತೆ % MIN_ಸ್ತಂಭ_ಅಗಲ != 0;

  ಆದರೆ (format == many_ಪ್ರತಿ_ಸಾಲು || format == horizontal || format == with_commas)
    {
      ಆದರೆ (0 <= tabsize_opt)
        tabsize = tabsize_opt;
      ಇಲ್ಲದಿದ್ದರೆ
        {
          tabsize = 8;
          ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *p = getenv ("TABSIZE");
          ಆದರೆ (p)
            {
              uintmax_t tmp;
              ಆದರೆ (xstrtoumax (p, ಖಾಲಿ, 0, &tmp, "") == LONGINT_OK
                  && tmp <= SIZE_MAX)
                tabsize = tmp;
              ಇಲ್ಲದಿದ್ದರೆ
                error (0, 0,
                       _("ignoring invalid tab size"
                         " in environment variable TABSIZE: %s"),
                       quote (p));
            }
        }
    }

  qmark_funny_chars = (hide_control_chars_opt < 0
                       ? ls_ಬಹುಲಕ == LS_LS && stdout_isatty ()
                       : hide_control_chars_opt);

  qmark_newline = hide_control_chars_opt != false && eolbyte == '\n'
                  && ((ಸಾಲು_ಉದ್ದಳತೆ
                       && (format == with_commas
                           || format == many_ಪ್ರತಿ_ಸಾಲು || format == horizontal))
                      || format == one_ಪ್ರತಿ_ಸಾಲು || format == long_format);

  ಪೂರ್ಣಾಂಕ qs = quoting_style_opt;
  ಆದರೆ (qs < 0)
    qs = getenv_quoting_style ();
  ಆದರೆ (qs < 0)
    qs = (ls_ಬಹುಲಕ == LS_LS
          ? (stdout_isatty () ? shell_escape_quoting_style : -1)
          : escape_quoting_style);
  ಆದರೆ (0 <= qs)
    ಗಣ_quoting_style (ಖಾಲಿ, qs);
  qs = get_quoting_style (ಖಾಲಿ);
  align_variable_outer_quotes
    = ((format == long_format
        || ((format == many_ಪ್ರತಿ_ಸಾಲು || format == horizontal) && ಸಾಲು_ಉದ್ದಳತೆ))
       && (qs == shell_quoting_style
           || qs == shell_escape_quoting_style
           || qs == c_maybe_quoting_style));
  filename_quoting_options = clone_quoting_options (ಖಾಲಿ);
  ಆದರೆ (qs == escape_quoting_style)
    ಗಣ_char_quoting (filename_quoting_options, ' ', 1);
  ಆದರೆ (file_type <= indicator_style)
    {
      ಗಾಗಿ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *p = &"*=>@|"[indicator_style - file_type]; *p; p++)
        ಗಣ_char_quoting (filename_quoting_options, *p, 1);
    }
  ಆದರೆ (format == with_commas
      && (qs == shell_quoting_style
          || qs == shell_escape_quoting_style
          || qs == c_maybe_quoting_style
          || qs == escape_quoting_style))
    ಗಣ_char_quoting (filename_quoting_options, ',', 1);

  dirname_quoting_options = clone_quoting_options (ಖಾಲಿ);
  ಗಣ_char_quoting (dirname_quoting_options, ':', 1);

  /* --dired implies --format=long (-l) and sans --hyperlink.
     So ignore it if those overridden.  */
  dired &= (format == long_format) & !print_hyperlink;

  ಆದರೆ (eolbyte < dired)
    error (LS_FAILURE, 0, _("--dired and --zero are incompatible"));

  /* If a time type is explicitly specified (with -c, -u, or --time=)
     and we're not showing a time (-l not specified), then sort by that time,
     rather than by name.  Note this behavior is unspecified by POSIX.  */

  ವಿಂಗಡಿಸು_type = (0 <= ವಿಂಗಡಿಸು_opt ? ವಿಂಗಡಿಸು_opt
               : (format != long_format && explicit_ಸಮಯ)
               ? ವಿಂಗಡಿಸು_ಸಮಯ : ವಿಂಗಡಿಸು_name);

  ಆದರೆ (format == long_format)
    {
      ಅಕ್ಷರ *envstyle = ಖಾಲಿ;
      ಅಕ್ಷರ *style = ಸಮಯ_style_option;
      ಆದರೆ (! style)
        style = envstyle = getenv ("TIME_STYLE");

      ಆದರೆ (! style)
        style = (ಅಕ್ಷರ *) "locale";
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ posix_ಪೂರ್ವಪ್ರತ್ಯಯ[] = "posix-";
          ಆಗ (STREQ_LEN (style, posix_ಪೂರ್ವಪ್ರತ್ಯಯ, ಗಾತ್ರ posix_ಪೂರ್ವಪ್ರತ್ಯಯ - 1))
            {
              ಆದರೆ (! hard_locale (LC_ಸಮಯ))
                ಹಿಂತಿರುಗಿ optind;
              style += ಗಾತ್ರ posix_ಪೂರ್ವಪ್ರತ್ಯಯ - 1;
            }
        }

      ಆದರೆ (*style == '+')
        {
          ಅಕ್ಷರ *p0 = style + 1;
          ಅಕ್ಷರ *p0nl = strchr (p0, '\n');
          ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *p1 = p0;
          ಆದರೆ (p0nl)
            {
              p1 = p0nl + 1;
              ಆದರೆ (strchr (p1, '\n'))
                error (LS_FAILURE, 0, _("invalid time style format %s"),
                       quote (p0));
              ಆದರೆ (envstyle)
                p0 = xmemdup0 (p0, p0nl - p0);
              ಇಲ್ಲದಿದ್ದರೆ
                *p0nl = '\0';
            }
          long_ಸಮಯ_format[0] = p0;
          long_ಸಮಯ_format[1] = p1;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಆಯ್ಕೆ (x_timestyle_ಹೊಂದಿಕೆ (style, /*allow_posix=*/ true,
                                     ಸಮಯ_style_ವಾದಗಳು,
                                     (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *) ಸಮಯ_style_types,
                                     ಗಾತ್ರ (*ಸಮಯ_style_types), LS_FAILURE))
            {
            ಸ್ಥಿತಿ ತುಂಬಿದ_iso_ಸಮಯ_style:
              long_ಸಮಯ_format[0] = long_ಸಮಯ_format[1] =
                "%Y-%m-%d %H:%M:%S.%N %z";
              ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ long_iso_ಸಮಯ_style:
              long_ಸಮಯ_format[0] = long_ಸಮಯ_format[1] = "%Y-%m-%d %H:%M";
              ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ iso_ಸಮಯ_style:
              long_ಸಮಯ_format[0] = "%Y-%m-%d ";
              long_ಸಮಯ_format[1] = "%m-%d %H:%M";
              ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ locale_ಸಮಯ_style:
              ಆದರೆ (hard_locale (LC_ಸಮಯ))
                {
                  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < 2; i++)
                    long_ಸಮಯ_format[i] =
                      dcgettext (ಖಾಲಿ, long_ಸಮಯ_format[i], LC_ಸಮಯ);
                }
            }
        }

      abformat_init ();
    }

  ಹಿಂತಿರುಗಿ optind;
}

/* Parse a string as part of the LS_COLORS variable; this may involve
   decoding all kinds of escape characters.  If equals_end is set an
   unescaped equal sign ends the string, otherwise only a : or \0
   does.  Set *OUTPUT_COUNT to the number of bytes output.  Return
   true if successful.

   The resulting string is *not* null-terminated, but may contain
   embedded nulls.

   Note that both dest and src are char **; on return they point to
   the first free byte after the array and the character that ended
   the input string, respectively.  */

ಸ್ಥಿರ bool
get_funky_string (ಅಕ್ಷರ **dest, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ **src, bool equals_ಕೊನೆ,
                  size_t *output_ಎಣಿಕೆ)
{
  ಅಕ್ಷರ ಅಂಕೆ;			/* For numerical codes */
  size_t ಎಣಿಕೆ_2;			/* Something to count with */
  ಎಣಿಕೆ {
    ST_GND, ST_BACKSLASH, ST_OCTAL, ST_HEX, ST_CARET, ST_ಕೊನೆ, ST_ERROR
  } state;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *p;
  ಅಕ್ಷರ *q;

  p = *src;			/* We don't want to double-indirect */
  q = *dest;			/* the whole darn time.  */

  ಎಣಿಕೆ_2 = 0;			/* No characters counted in yet.  */
  ಅಂಕೆ = 0;

  state = ST_GND;		/* Start in ground state.  */
  ಆಗ (state < ST_ಕೊನೆ)
    {
      ಆಯ್ಕೆ (state)
        {
        ಸ್ಥಿತಿ ST_GND:		/* Ground state (no escapes) */
          ಆಯ್ಕೆ (*p)
            {
            ಸ್ಥಿತಿ ':':
            ಸ್ಥಿತಿ '\0':
              state = ST_ಕೊನೆ;	/* End of string */
              ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ '\\':
              state = ST_BACKSLASH; /* Backslash escape sequence */
              ++p;
              ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ '^':
              state = ST_CARET; /* Caret escape */
              ++p;
              ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ '=':
              ಆದರೆ (equals_ಕೊನೆ)
                {
                  state = ST_ಕೊನೆ; /*ಕೊನೆ*/
                  ನಿಲ್ಲಿಸು;
                }
              FALLTHROUGH;
            ಪೂರ್ವನಿಯೋಜಿತ:
              *(q++) = *(p++);
              ++ಎಣಿಕೆ_2;
              ನಿಲ್ಲಿಸು;
            }
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ ST_BACKSLASH:	/* Backslash escaped character */
          ಆಯ್ಕೆ (*p)
            {
            ಸ್ಥಿತಿ '0':
            ಸ್ಥಿತಿ '1':
            ಸ್ಥಿತಿ '2':
            ಸ್ಥಿತಿ '3':
            ಸ್ಥಿತಿ '4':
            ಸ್ಥಿತಿ '5':
            ಸ್ಥಿತಿ '6':
            ಸ್ಥಿತಿ '7':
              state = ST_OCTAL;	/* Octal sequence */
              ಅಂಕೆ = *p - '0';
              ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ 'x':
            ಸ್ಥಿತಿ 'X':
              state = ST_HEX;	/* Hex sequence */
              ಅಂಕೆ = 0;
              ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ 'a':		/* Bell */
              ಅಂಕೆ = '\a';
              ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ 'b':		/* Backspace */
              ಅಂಕೆ = '\b';
              ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ 'e':		/* Escape */
              ಅಂಕೆ = 27;
              ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ 'f':		/* Form feed */
              ಅಂಕೆ = '\f';
              ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ 'n':		/* Newline */
              ಅಂಕೆ = '\n';
              ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ 'r':		/* Carriage return */
              ಅಂಕೆ = '\r';
              ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ 't':		/* Tab */
              ಅಂಕೆ = '\t';
              ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ 'v':		/* Vtab */
              ಅಂಕೆ = '\v';
              ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ '?':		/* Delete */
              ಅಂಕೆ = 127;
              ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ '_':		/* Space */
              ಅಂಕೆ = ' ';
              ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ '\0':		/* End of string */
              state = ST_ERROR;	/* Error! */
              ನಿಲ್ಲಿಸು;
            ಪೂರ್ವನಿಯೋಜಿತ:		/* Escaped character like \ ^ : = */
              ಅಂಕೆ = *p;
              ನಿಲ್ಲಿಸು;
            }
          ಆದರೆ (state == ST_BACKSLASH)
            {
              *(q++) = ಅಂಕೆ;
              ++ಎಣಿಕೆ_2;
              state = ST_GND;
            }
          ++p;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ ST_OCTAL:		/* Octal sequence */
          ಆದರೆ (*p < '0' || *p > '7')
            {
              *(q++) = ಅಂಕೆ;
              ++ಎಣಿಕೆ_2;
              state = ST_GND;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            ಅಂಕೆ = (ಅಂಕೆ << 3) + (*(p++) - '0');
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ ST_HEX:		/* Hex sequence */
          ಆಯ್ಕೆ (*p)
            {
            ಸ್ಥಿತಿ '0':
            ಸ್ಥಿತಿ '1':
            ಸ್ಥಿತಿ '2':
            ಸ್ಥಿತಿ '3':
            ಸ್ಥಿತಿ '4':
            ಸ್ಥಿತಿ '5':
            ಸ್ಥಿತಿ '6':
            ಸ್ಥಿತಿ '7':
            ಸ್ಥಿತಿ '8':
            ಸ್ಥಿತಿ '9':
              ಅಂಕೆ = (ಅಂಕೆ << 4) + (*(p++) - '0');
              ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ 'a':
            ಸ್ಥಿತಿ 'b':
            ಸ್ಥಿತಿ 'c':
            ಸ್ಥಿತಿ 'd':
            ಸ್ಥಿತಿ 'e':
            ಸ್ಥಿತಿ 'f':
              ಅಂಕೆ = (ಅಂಕೆ << 4) + (*(p++) - 'a') + 10;
              ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ 'A':
            ಸ್ಥಿತಿ 'B':
            ಸ್ಥಿತಿ 'C':
            ಸ್ಥಿತಿ 'D':
            ಸ್ಥಿತಿ 'E':
            ಸ್ಥಿತಿ 'F':
              ಅಂಕೆ = (ಅಂಕೆ << 4) + (*(p++) - 'A') + 10;
              ನಿಲ್ಲಿಸು;
            ಪೂರ್ವನಿಯೋಜಿತ:
              *(q++) = ಅಂಕೆ;
              ++ಎಣಿಕೆ_2;
              state = ST_GND;
              ನಿಲ್ಲಿಸು;
            }
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ ST_CARET:		/* Caret escape */
          state = ST_GND;	/* Should be the next state... */
          ಆದರೆ (*p >= '@' && *p <= '~')
            {
              *(q++) = *(p++) & 037;
              ++ಎಣಿಕೆ_2;
            }
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (*p == '?')
            {
              *(q++) = 127;
              ++ಎಣಿಕೆ_2;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            state = ST_ERROR;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ ST_ಕೊನೆ: ಸ್ಥಿತಿ ST_ERROR: ಪೂರ್ವನಿಯೋಜಿತ:
          affirm (false);
        }
    }

  *dest = q;
  *src = p;
  *output_ಎಣಿಕೆ = ಎಣಿಕೆ_2;

  ಹಿಂತಿರುಗಿ state != ST_ERROR;
}

ಎಣಿಕೆ parse_state
  {
    PS_ಆರಂಭ = 1,
    PS_2,
    PS_3,
    PS_4,
    PS_ಮುಗಿದ,
    PS_FAIL
  };


/* Check if the content of TERM is a valid name in dircolors.  */

ಸ್ಥಿರ bool
ತಿಳಿದ_ಪದ_type (ಶೂನ್ಯ)
{
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಪದ = getenv ("TERM");
  ಆದರೆ (! ಪದ || ! *ಪದ)
    ಹಿಂತಿರುಗಿ false;

  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸಾಲು = G_ಸಾಲು;
  ಆಗ (ಸಾಲು - G_ಸಾಲು < ಗಾತ್ರ (G_ಸಾಲು))
    {
      ಆದರೆ (STRNCMP_LIT (ಸಾಲು, "TERM ") == 0)
        {
          ಆದರೆ (fnmatch (ಸಾಲು + 5, ಪದ, 0) == 0)
            ಹಿಂತಿರುಗಿ true;
        }
      ಸಾಲು += ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ಸಾಲು) + 1;
    }

  ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ ಶೂನ್ಯ
parse_ls_color (ಶೂನ್ಯ)
{
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *p;		/* Pointer to character being parsed */
  ಅಕ್ಷರ *buf;			/* color_buf buffer pointer */
  ಅಕ್ಷರ label0, label1;		/* Indicator label */
  ರಚನೆ color_ext_type *ext;	/* Extension we are working on */

  ಆದರೆ ((p = getenv ("LS_COLORS")) == ಖಾಲಿ || *p == '\0')
    {
      /* LS_COLORS takes precedence, but if that's not set then
         honor the COLORTERM and TERM env variables so that
         we only go with the internal ANSI color codes if the
         former is non empty or the latter is set to a known value.  */
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *colorterm = getenv ("COLORTERM");
      ಆದರೆ (! (colorterm && *colorterm) && ! ತಿಳಿದ_ಪದ_type ())
        print_with_color = false;
      ಹಿಂತಿರುಗಿ;
    }

  ext = ಖಾಲಿ;

  /* This is an overly conservative estimate, but any possible
     LS_COLORS string will *not* generate a color_buf longer than
     itself, so it is a safe way of allocating a buffer in
     advance.  */
  buf = color_buf = xstrdup (p);

  ಎಣಿಕೆ parse_state state = PS_ಆರಂಭ;
  ಆಗ (true)
    {
      ಆಯ್ಕೆ (state)
        {
        ಸ್ಥಿತಿ PS_ಆರಂಭ:		/* First label character */
          ಆಯ್ಕೆ (*p)
            {
            ಸ್ಥಿತಿ ':':
              ++p;
              ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ '*':
              /* Allocate new extension block and add to head of
                 linked list (this way a later definition will
                 override an earlier one, which can be useful for
                 having terminal-specific defs override global).  */

              ext = xmalloc (ಗಾತ್ರ *ext);
              ext->ಮುಂದಿನ = color_ext_list;
              color_ext_list = ext;
              ext->exact_ಹೊಂದಿಕೆ = false;

              ++p;
              ext->ext.string = buf;

              state = (get_funky_string (&buf, &p, true, &ext->ext.len)
                       ? PS_4 : PS_FAIL);
              ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ '\0':
              state = PS_ಮುಗಿದ;	/* Done! */
              ಹೋಗು ಮುಗಿದ;

            ಪೂರ್ವನಿಯೋಜಿತ:	/* Assume it is file type label */
              label0 = *p++;
              state = PS_2;
              ನಿಲ್ಲಿಸು;
            }
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ PS_2:		/* Second label character */
          ಆದರೆ (*p)
            {
              label1 = *p++;
              state = PS_3;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            state = PS_FAIL;	/* Error */
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ PS_3:		/* Equal sign after indicator label */
          state = PS_FAIL;	/* Assume failure...  */
          ಆದರೆ (*(p++) == '=')/* It *should* be...  */
            {
              ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < countof (indicator_name); i++)
                {
                  ಆದರೆ ((label0 == indicator_name[i][0])
                      && (label1 == indicator_name[i][1]))
                    {
                      color_indicator[i].string = buf;
                      state = (get_funky_string (&buf, &p, false,
                                                 &color_indicator[i].len)
                               ? PS_ಆರಂಭ : PS_FAIL);
                      ನಿಲ್ಲಿಸು;
                    }
                }
              ಆದರೆ (state == PS_FAIL)
                error (0, 0, _("unrecognized prefix: %s"),
                       quote ((ಅಕ್ಷರ []) {label0, label1, '\0'}));
            }
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ PS_4:		/* Equal sign after *.ext */
          ಆದರೆ (*(p++) == '=')
            {
              ext->seq.string = buf;
              state = (get_funky_string (&buf, &p, false, &ext->seq.len)
                       ? PS_ಆರಂಭ : PS_FAIL);
            }
          ಇಲ್ಲದಿದ್ದರೆ
            state = PS_FAIL;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ PS_FAIL:
          ಹೋಗು ಮುಗಿದ;

        ಸ್ಥಿತಿ PS_ಮುಗಿದ: ಪೂರ್ವನಿಯೋಜಿತ:
          affirm (false);
        }
    }
 ಮುಗಿದ:

  ಆದರೆ (state == PS_FAIL)
    {
      error (0, 0,
             _("unparsable value for LS_COLORS environment variable"));
      ಬಿಡುಗಡೆ (color_buf);
      ಗಾಗಿ (ರಚನೆ color_ext_type *e = color_ext_list; e != ಖಾಲಿ;
           /*ಖಾಲಿಗಳು*/)
        {
          ರಚನೆ color_ext_type *e2 = e;
          e = e->ಮುಂದಿನ;
          ಬಿಡುಗಡೆ (e2);
        }
      print_with_color = false;
      ಹಿಂತಿರುಗಿ;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      /* Postprocess list to set EXACT_MATCH on entries where there are
         different cased extensions with separate sequences defined.
         Also set ext.len to SIZE_MAX on any entries that can't
         match due to precedence, to avoid redundant string compares.  */
      ಗಾಗಿ (ರಚನೆ color_ext_type *e1 = color_ext_list; e1 != ಖಾಲಿ;
           e1 = e1->ಮುಂದಿನ)
        {
          bool case_ignored = false;

          ಗಾಗಿ (ರಚನೆ color_ext_type *e2 = e1->ಮುಂದಿನ; e2 != ಖಾಲಿ;
               e2 = e2->ಮುಂದಿನ)
            {
              ಆದರೆ (e2->ext.len < SIZE_MAX && e1->ext.len == e2->ext.len)
                {
                  ಆದರೆ (memeq (e1->ext.string, e2->ext.string, e1->ext.len))
                    e2->ext.len = SIZE_MAX; /* Ignore */
                  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (c_strncasecmp (e1->ext.string, e2->ext.string,
                                          e1->ext.len) == 0)
                    {
                      ಆದರೆ (case_ignored)
                        {
                          e2->ext.len = SIZE_MAX; /* Ignore */
                        }
                      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (e1->seq.len == e2->seq.len
                               && memeq (e1->seq.string, e2->seq.string,
                                         e1->seq.len))
                        {
                          e2->ext.len = SIZE_MAX; /* Ignore */
                          case_ignored = true;    /* Ignore all subsequent */
                        }
                      ಇಲ್ಲದಿದ್ದರೆ
                        {
                          e1->exact_ಹೊಂದಿಕೆ = true;
                          e2->exact_ಹೊಂದಿಕೆ = true;
                        }
                    }
                }
            }
        }
    }

  ಆದರೆ (color_indicator[C_LINK].len == 6
      && !STRNCMP_LIT (color_indicator[C_LINK].string, "target"))
    color_symlink_as_referent = true;
}

/* Set the exit status to report a failure.  If SERIOUS, it is a
   serious failure; otherwise, it is merely a minor problem.  */

ಸ್ಥಿರ ಶೂನ್ಯ
ಗಣ_exit_ಸ್ಥಿತಿ (bool serious)
{
  ಆದರೆ (serious)
    exit_ಸ್ಥಿತಿ = LS_FAILURE;
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (exit_ಸ್ಥಿತಿ == EXIT_SUCCESS)
    exit_ಸ್ಥಿತಿ = LS_MINOR_PROBLEM;
}

/* Assuming a failure is serious if SERIOUS, use the printf-style
   MESSAGE to report the failure to access a file named FILE.  Assume
   errno is set appropriately for the failure.  */

ಸ್ಥಿರ ಶೂನ್ಯ
file_failure (bool serious, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸಂದೇಶ_2, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file)
{
  error (0, errno, ಸಂದೇಶ_2, quoteaf (file));
  ಗಣ_exit_ಸ್ಥಿತಿ (serious);
}

/* Request that the directory named NAME have its contents listed later.
   If REALNAME is nonzero, it will be used instead of NAME when the
   directory name is printed.  This allows symbolic links to directories
   to be treated as regular directories but still be listed under their
   real names.  NAME == NULL is used to insert a marker entry for the
   directory named in REALNAME.
   If NAME is non-null, we use its dev/ino information to save
   a call to stat -- when doing a recursive (-R) traversal.
   COMMAND_LINE_ARG means this directory was mentioned on the command line.  */

ಸ್ಥಿರ ಶೂನ್ಯ
ಸರತಿ_directory (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *realname, bool command_ಸಾಲು_arg)
{
  ರಚನೆ pending *new = xmalloc (ಗಾತ್ರ *new);
  new->realname = realname ? xstrdup (realname) : ಖಾಲಿ;
  new->name = name ? xstrdup (name) : ಖಾಲಿ;
  new->command_ಸಾಲು_arg = command_ಸಾಲು_arg;
  new->ಮುಂದಿನ = pending_dirs;
  pending_dirs = new;
}

/* Read directory NAME, and list the files in it.
   If REALNAME is nonzero, print its name instead of NAME;
   this is used for symbolic links to directories.
   COMMAND_LINE_ARG means this directory was mentioned on the command line.  */

ಸ್ಥಿರ ಶೂನ್ಯ
print_dir (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *realname, bool command_ಸಾಲು_arg)
{
  DIR *dirp;
  ರಚನೆ dirent *ಮುಂದಿನ;
  uintmax_t ಒಟ್ಟು_ಖಂಡಗಳು = 0;
  ಸ್ಥಿರ bool ಮೊದಲ = true;

  errno = 0;
  dirp = opendir (name);
  ಆದರೆ (!dirp)
    {
      ಆದರೆ (command_ಸಾಲು_arg || ! ignorable_traversal_errno (errno))
        file_failure (command_ಸಾಲು_arg, _("cannot open directory %s"), name);
      ಹಿಂತಿರುಗಿ;
    }

  ಆದರೆ (LOOP_DETECT)
    {
      ರಚನೆ stat dir_stat;
      ಪೂರ್ಣಾಂಕ fd = dirfd (dirp);

      /* If dirfd failed, endure the overhead of stat'ing by path  */
      ಆದರೆ ((0 <= fd
           ? fstat_for_ino (fd, &dir_stat)
           : stat_for_ino (name, &dir_stat)) < 0)
        {
          ಆದರೆ (! ignorable_traversal_errno (errno))
            file_failure (command_ಸಾಲು_arg,
                          _("cannot determine device and inode of %s"), name);
          closedir (dirp);
          ಹಿಂತಿರುಗಿ;
        }

      /* If we've already visited this dev/inode pair, warn that
         we've found a loop, and do not process this directory.  */
      ಆದರೆ (ಭೇಟಿ_dir (dir_stat.st_dev, dir_stat.st_ino))
        {
          error (0, 0, _("%s: not listing already-listed directory"),
                 quotef (name));
          closedir (dirp);
          ಗಣ_exit_ಸ್ಥಿತಿ (true);
          ಹಿಂತಿರುಗಿ;
        }

      dev_ino_ತಳ್ಳು (dir_stat.st_dev, dir_stat.st_ino);
    }

  clear_files ();

  ಆದರೆ (recursive || print_dir_name)
    {
      ಆದರೆ (!ಮೊದಲ)
        dired_outbyte ('\n');
      ಮೊದಲ = false;
      dired_indent ();

      ಅಕ್ಷರ *absolute_name = ಖಾಲಿ;
      ಆದರೆ (print_hyperlink)
        {
          absolute_name = canonicalize_filename_ಬಹುಲಕ (name, CAN_MISSING);
          ಆದರೆ (! absolute_name
              && ! ignorable_traversal_errno (errno))
            file_failure (command_ಸಾಲು_arg,
                          _("error canonicalizing %s"), name);
        }
      quote_name (realname ? realname : name, dirname_quoting_options, -1,
                  ಖಾಲಿ, true, &subdired_obstack, absolute_name);

      ಬಿಡುಗಡೆ (absolute_name);

      dired_outstring (":\n");
    }

  /* Read the directory entries, and insert the subfiles into the 'cwd_file'
     table.  */

  ಆಗ (true)
    {
      /* Set errno to zero so we can distinguish between a readdir failure
         and when readdir simply finds that there are no more entries.  */
      errno = 0;
      ಮುಂದಿನ = readdir (dirp);
      ಆದರೆ (ಮುಂದಿನ)
        {
          ಆದರೆ (! file_ignored (ಮುಂದಿನ->d_name))
            {
              ಎಣಿಕೆ filetype type;
#ಆದರೆ HAVE_STRUCT_DIRENT_D_TYPE
              type = d_type_filetype[ಮುಂದಿನ->d_type];
#ಇಲ್ಲದಿದ್ದರೆ
              type = unknown;
#ಆದರೆ_ಅಂತ್ಯ
              ಒಟ್ಟು_ಖಂಡಗಳು += gobble_file (ಮುಂದಿನ->d_name, type,
                                           RELIABLE_D_INO (ಮುಂದಿನ),
                                           false, name);

              /* In this narrow case, print out each name right away, so
                 ls uses constant memory while processing the entries of
                 this directory.  Useful when there are many (millions)
                 of entries in a directory.  */
              ಆದರೆ (format == one_ಪ್ರತಿ_ಸಾಲು && ವಿಂಗಡಿಸು_type == ವಿಂಗಡಿಸು_none
                      && !print_ಖಂಡ_ಗಾತ್ರ && !recursive)
                {
                  /* We must call sort_files in spite of
                     "sort_type == sort_none" for its initialization
                     of the sorted_file vector.  */
                  ವಿಂಗಡಿಸು_files ();
                  print_current_files ();
                  clear_files ();
                }
            }
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಪೂರ್ಣಾಂಕ err = errno;
          ಆದರೆ (err == 0)
            ನಿಲ್ಲಿಸು;
          /* Ignore errors indicating that the directory was removed.  */
          ಆದರೆ (ignorable_traversal_errno (err))
            ನಿಲ್ಲಿಸು;
          file_failure (command_ಸಾಲು_arg, _("reading directory %s"), name);
          ಆದರೆ (err != EOVERFLOW)
            ನಿಲ್ಲಿಸು;
        }

      /* When processing a very large directory, and since we've inhibited
         interrupts, this loop would take so long that ls would be annoyingly
         uninterruptible.  This ensures that it handles signals promptly.  */
      process_ಸಂಕೇತಗಳು ();
    }

  ಆದರೆ (closedir (dirp) != 0)
    {
      file_failure (command_ಸಾಲು_arg, _("closing directory %s"), name);
      /* Don't return; print whatever we got.  */
    }

  /* Sort the directory contents.  */
  ವಿಂಗಡಿಸು_files ();

  /* If any member files are subdirectories, perhaps they should have their
     contents listed rather than being mentioned here as files.  */

  ಆದರೆ (recursive)
    ಹೊರತೆಗೆ_dirs_from_files (name, false);

  ಆದರೆ (format == long_format || print_ಖಂಡ_ಗಾತ್ರ)
    {
      ಅಕ್ಷರ buf[LONGEST_HUMAN_READABLE + 3];
      ಅಕ್ಷರ *p = human_readable (ಒಟ್ಟು_ಖಂಡಗಳು, buf + 1, human_output_opts,
                                ST_NBLOCKSIZE, output_ಖಂಡ_ಗಾತ್ರ);
      ಅಕ್ಷರ *pend = p + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (p);
      *--p = ' ';
      *pend++ = eolbyte;
      dired_indent ();
      dired_outstring (_("total"));
      dired_outbuf (p, pend - p);
    }

  ಆದರೆ (cwd_n_ಬಳಸಿದ)
    print_current_files ();
}

/* Add 'pattern' to the list of patterns for which files that match are
   not listed.  */

ಸ್ಥಿರ ಶೂನ್ಯ
ಕೂಡಿಸು_ignore_ನಮೂನೆ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ನಮೂನೆ_2)
{
  ರಚನೆ ignore_ನಮೂನೆ *ignore;

  ignore = xmalloc (ಗಾತ್ರ *ignore);
  ignore->ನಮೂನೆ_2 = ನಮೂನೆ_2;
  /* Add it to the head of the linked list.  */
  ignore->ಮುಂದಿನ = ignore_patterns;
  ignore_patterns = ignore;
}

/* Return true if one of the PATTERNS matches FILE.  */

ಸ್ಥಿರ bool
patterns_ಹೊಂದಿಕೆ (ರಚನೆ ignore_ನಮೂನೆ ಸ್ಥಿರಾಂಕ *patterns, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file)
{
  ಗಾಗಿ (ರಚನೆ ignore_ನಮೂನೆ ಸ್ಥಿರಾಂಕ *p = patterns; p; p = p->ಮುಂದಿನ)
    ಆದರೆ (fnmatch (p->ನಮೂನೆ_2, file, FNM_PERIOD) == 0)
      ಹಿಂತಿರುಗಿ true;
  ಹಿಂತಿರುಗಿ false;
}

/* Return true if FILE should be ignored.  */

ಸ್ಥಿರ bool
file_ignored (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name)
{
  ಹಿಂತಿರುಗಿ ((ignore_ಬಹುಲಕ != IGNORE_ಕನಿಷ್ಠತಮ
           && name[0] == '.'
           && (ignore_ಬಹುಲಕ == IGNORE_DEFAULT || ! name[1 + (name[1] == '.')]))
          || (ignore_ಬಹುಲಕ == IGNORE_DEFAULT
              && patterns_ಹೊಂದಿಕೆ (hide_patterns, name))
          || patterns_ಹೊಂದಿಕೆ (ignore_patterns, name));
}

/* POSIX requires that a file size be printed without a sign, even
   when negative.  Assume the typical case where negative sizes are
   actually positive values that have wrapped around.  */

ಸ್ಥಿರ uintmax_t
unsigned_file_ಗಾತ್ರ (off_t ಗಾತ್ರ_2)
{
  ಹಿಂತಿರುಗಿ ಗಾತ್ರ_2 + (ಗಾತ್ರ_2 < 0) * ((uintmax_t) OFF_T_MAX - OFF_T_MIN + 1);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CAP
/* Return true if NAME has a capability (see linux/capability.h) */
ಸ್ಥಿರ bool
has_capability (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name)
{
  ಅಕ್ಷರ *ಫಲಿತಾಂಶ;
  bool has_cap;

  cap_t cap_d = cap_get_file (name);
  ಆದರೆ (cap_d == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ false;

  ಫಲಿತಾಂಶ = cap_ಗಮ್ಯ_ಪಠ್ಯ (cap_d, ಖಾಲಿ);
  cap_free (cap_d);
  ಆದರೆ (!ಫಲಿತಾಂಶ)
    ಹಿಂತಿರುಗಿ false;

  /* check if human-readable capability string is empty */
  has_cap = !!*ಫಲಿತಾಂಶ;

  cap_free (ಫಲಿತಾಂಶ);
  ಹಿಂತಿರುಗಿ has_cap;
}
#ಇಲ್ಲದಿದ್ದರೆ
ಸ್ಥಿರ bool
has_capability (MAYBE_ಬಳಸದ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name)
{
  errno = ENOTSUP;
  ಹಿಂತಿರುಗಿ false;
}
#ಆದರೆ_ಅಂತ್ಯ

/* Enter and remove entries in the table 'cwd_file'.  */

ಸ್ಥಿರ ಶೂನ್ಯ
free_ent (ರಚನೆ fileinfo *f)
{
  ಬಿಡುಗಡೆ (f->name);
  ಬಿಡುಗಡೆ (f->linkname);
  ಬಿಡುಗಡೆ (f->absolute_name);
  ಆದರೆ (f->scontext != UNKNOWN_SECURITY_CONTEXT)
    aclinfo_scontext_free (f->scontext);
}

/* Empty the table of files.  */
ಸ್ಥಿರ ಶೂನ್ಯ
clear_files (ಶೂನ್ಯ)
{
  ಗಾಗಿ (idx_t i = 0; i < cwd_n_ಬಳಸಿದ; i++)
    {
      ರಚನೆ fileinfo *f = sorted_file[i];
      free_ent (f);
    }

  cwd_n_ಬಳಸಿದ = 0;
  cwd_some_quoted = false;
  any_has_acl = false;
  inode_ಸಂಖ್ಯೆ_ಅಗಲ = 0;
  ಖಂಡ_ಗಾತ್ರ_ಅಗಲ = 0;
  nlink_ಅಗಲ = 0;
  owner_ಅಗಲ = 0;
  ಗುಂಪು_ಅಗಲ = 0;
  author_ಅಗಲ = 0;
  scontext_ಅಗಲ = 0;
  major_device_ಸಂಖ್ಯೆ_ಅಗಲ = 0;
  minor_device_ಸಂಖ್ಯೆ_ಅಗಲ = 0;
  file_ಗಾತ್ರ_ಅಗಲ = 0;
}

/* Cache file_has_aclinfo failure, when it's trivial to do.
   Like file_has_aclinfo, but when F's st_dev says it's on a file
   system lacking ACL support, return 0 with ENOTSUP immediately.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
file_has_aclinfo_cache (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file, ರಚನೆ fileinfo *f,
                        ರಚನೆ aclinfo *ai, ಪೂರ್ಣಾಂಕ flags)
{
  /* If UNSUPPORTED_SCONTEXT, these variables hold the
     st_dev and associated info for the most recently processed device
     for which file_has_aclinfo failed indicating lack of support.  */
  ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ unsupported_return;
  ಸ್ಥಿರ ಅಕ್ಷರ *unsupported_scontext;
  ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ unsupported_scontext_err;
  ಸ್ಥಿರ dev_t unsupported_device;

  ಆದರೆ (f->stat_ok && unsupported_scontext
      && f->stat.st_dev == unsupported_device)
    {
      ai->buf = ai->u._gl_acl_ch;
      ai->ಗಾತ್ರ_2 = 0;
      ai->scontext = unsupported_scontext;
      ai->scontext_err = unsupported_scontext_err;
      errno = ENOTSUP;
      ಹಿಂತಿರುಗಿ unsupported_return;
    }

  errno = 0;
  ಪೂರ್ಣಾಂಕ n = file_has_aclinfo (file, ai, flags);
  ಪೂರ್ಣಾಂಕ err = errno;
  ಆದರೆ (!USE_ACL)
    n = 0;
  /* A security-context error does not imply that ACLs are unsupported.
     Require an unsupported attribute listing too if ACLs or capability
     colors are needed.  */
  ಆದರೆ (f->stat_ok && n <= 0 && !acl_errno_valid (err)
      && ((!USE_ACL && !format_needs_capability)
          || (ai->ಗಾತ್ರ_2 < 0 && !acl_errno_valid (ai->u.err)))
      && (!(flags & ACL_GET_SCONTEXT) || !acl_errno_valid (ai->scontext_err)))
    {
      /* FIXME: acl_errno_valid() may be a bit too broad determine file system
         non support.  E.g. EBUSY was added for per file reasons (on cygwin). */
      unsupported_return = n;
      unsupported_scontext = ai->scontext;
      unsupported_scontext_err = ai->scontext_err;
      unsupported_device = f->stat.st_dev;
    }
  ಹಿಂತಿರುಗಿ n;
}

/* Cache has_capability failure, when it's trivial to do.
   Like has_capability, but when F's st_dev says it's on a file
   system lacking capability support, return 0 with ENOTSUP immediately.  */
ಸ್ಥಿರ bool
has_capability_cache (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file, ರಚನೆ fileinfo *f)
{
  /* If UNSUPPORTED_CACHED, UNSUPPORTED_DEVICE is the cached
     st_dev of the most recently processed device for which we've
     found that has_capability fails indicating lack of support.  */
  ಸ್ಥಿರ bool unsupported_cached;
  ಸ್ಥಿರ dev_t unsupported_device;

  ಆದರೆ (f->stat_ok && unsupported_cached && f->stat.st_dev == unsupported_device)
    {
      errno = ENOTSUP;
      ಹಿಂತಿರುಗಿ 0;
    }

  bool b = has_capability (file);
  ಆದರೆ (f->stat_ok && !b && !acl_errno_valid (errno))
    {
      unsupported_cached = true;
      unsupported_device = f->stat.st_dev;
    }
  ಹಿಂತಿರುಗಿ b;
}

/* Get the ACL, security context, and capability information needed for F.
   FULL_NAME names the file; DO_DEREF says whether to follow symlinks.  */
ಸ್ಥಿರ ಶೂನ್ಯ
get_extended_info (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ತುಂಬಿದ_name, ರಚನೆ fileinfo *f, bool do_deref)
{
  /* FIXME: file_has_aclinfo() conflates capability and acl xattr lookup
     with security context attributes.  Ideally we'd have separate
     ACL_GET_{CAPABILITY,ACL} flags to lookup just the required items,
     simplifying the logic here.  */

  ಎಣಿಕೆ filetype type = f->filetype;
  bool check_acl = USE_ACL && format == long_format;
  bool get_scontext = false;
#ಆದರೆ HAVE_SMACK || USE_SELINUX_SELINUX_H
  get_scontext = (format == long_format) | print_scontext;
#ಆದರೆ_ಅಂತ್ಯ
  bool check_capability = format_needs_capability & (type == ಸಾಮಾನ್ಯ);

  ಆದರೆ (type != arg_directory && (check_acl | get_scontext | check_capability))
    {
      ರಚನೆ aclinfo ai;
      /* Without ACL support, request contexts to obtain the attribute
         names needed for capability coloring too.  */
      ಪೂರ್ಣಾಂಕ aclinfo_flags = ((do_deref ? ACL_SYMLINK_FOLLOW : 0)
                           | (get_scontext || (!USE_ACL && check_capability)
                              ? ACL_GET_SCONTEXT : 0)
                           | filetype_d_type[type]);
      ಪೂರ್ಣಾಂಕ n = file_has_aclinfo_cache (ತುಂಬಿದ_name, f, &ai, aclinfo_flags);
      bool have_acl = 0 < n;
      bool have_scontext = !ai.scontext_err;

      /* Let the user know via '?' if errno is EACCES, which can
         happen with Linux kernel 6.12 on an NFS file system.
         That's better than a long-winded diagnostic.

         Similarly, ignore ENOENT which may happen on some versions
         of cygwin when processing dangling symlinks for example.
         Also if a file is removed while we're reading ACL info,
         ACL_T_UNKNOWN is sufficient indication for that edge case.  */
      bool cannot_access_acl = n < 0
           && (errno == EACCES
               || ignorable_traversal_errno (errno));

      f->acl_type = (!have_scontext && !have_acl
                     ? (cannot_access_acl ? ACL_T_UNKNOWN : ACL_T_NONE)
                     : (have_scontext && !have_acl
                        ? ACL_T_LSM_CONTEXT_ONLY
                        : ACL_T_YES));
      any_has_acl |= f->acl_type != ACL_T_NONE;

      ಆದರೆ (format == long_format && n < 0 && !cannot_access_acl)
        error (0, errno, "%s", quotef (ತುಂಬಿದ_name));
      ಇಲ್ಲದಿದ್ದರೆ
        {
          /* When requesting security context information, don't make
             ls fail just because the file (even a command line argument)
             isn't on the right type of file system.  I.e., a getfilecon
             failure isn't in the same class as a stat failure.  */
          ಆದರೆ (print_scontext && ai.scontext_err
              && ! ignorable_traversal_errno (ai.scontext_err)
              && (! (is_ENOTSUP (ai.scontext_err)
                     || ai.scontext_err == ENODATA)))
            error (0, ai.scontext_err, "%s", quotef (ತುಂಬಿದ_name));
        }

      /* has_capability adds around 30% runtime to 'ls --color',
         so call it only if really needed.  Note capability coloring
         is disabled in the default color config.  */
      ಆದರೆ (check_capability && aclinfo_has_xattr (&ai, XATTR_NAME_CAPS))
        f->has_capability = has_capability_cache (ತುಂಬಿದ_name, f);

      f->scontext = ai.scontext;
      ai.scontext = ಖಾಲಿ;
      aclinfo_free (&ai);
    }
}

ಸ್ಥಿರ bool
needs_quoting (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name)
{
  ಅಕ್ಷರ test[2];
  size_t len = quotearg_ಬಫರ್ (test, ಗಾತ್ರ test , name, -1,
                                filename_quoting_options);
  ಹಿಂತಿರುಗಿ *name != *test || ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (name) != len;
}

/* Add a file to the current table of files.
   Verify that the file exists, and print an error message if it does not.
   Return the number of blocks that the file occupies.  */
ಸ್ಥಿರ uintmax_t
gobble_file (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name, ಎಣಿಕೆ filetype type, ino_t inode,
             bool command_ಸಾಲು_arg, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dirname)
{
  uintmax_t ಖಂಡಗಳು = 0;
  ರಚನೆ fileinfo *f;

  /* An inode value prior to gobble_file necessarily came from readdir,
     which is not used for command line arguments.  */
  affirm (! command_ಸಾಲು_arg || inode == NOT_AN_INODE_ಸಂಖ್ಯೆ);

  ಆದರೆ (cwd_n_ಬಳಸಿದ == cwd_n_alloc)
    cwd_file = xpalloc (cwd_file, &cwd_n_alloc, 1, -1, ಗಾತ್ರ *cwd_file);

  f = &cwd_file[cwd_n_ಬಳಸಿದ];
  memset (f, '\0', ಗಾತ್ರ *f);
  f->stat.st_ino = inode;
  f->filetype = type;
  f->scontext = UNKNOWN_SECURITY_CONTEXT;

  f->quoted = -1;
  ಆದರೆ ((! cwd_some_quoted) && align_variable_outer_quotes)
    {
      /* Determine if any quoted for padding purposes.  */
      f->quoted = needs_quoting (name);
      ಆದರೆ (f->quoted)
        cwd_some_quoted = 1;
    }

  bool check_stat =
     (command_ಸಾಲು_arg
      || print_hyperlink
      || format_needs_stat
      || (format_needs_type && type == unknown)
      /* When coloring a directory, stat it to indicate
         sticky and/or other-writable attributes.  */
      || ((type == directory || type == unknown) && print_with_color
          && (is_colored (C_OTHER_WRITABLE)
              || is_colored (C_STICKY)
              || is_colored (C_STICKY_OTHER_WRITABLE)))
      /* When dereferencing symlinks, the inode and type must come from
         stat, but readdir provides the inode and type of lstat.  */
      || ((print_inode || format_needs_type)
          && (type == symbolic_link || type == unknown)
          && (dereference == DEREF_ALWAYS
              || color_symlink_as_referent || check_symlink_ಬಹುಲಕ))
      /* Command line dereferences are already taken care of by the above
         assertion that the inode number is not yet known.  */
      || (print_inode && inode == NOT_AN_INODE_ಸಂಖ್ಯೆ)
      /* --indicator-style=classify (aka -F) requires statting each
         regular file to see whether it's executable.  */
      || ((type == ಸಾಮಾನ್ಯ || type == unknown)
          && (indicator_style == classify
              /* This is so that --color ends up highlighting files with these
                 mode bits set even when options like -F are not specified.  */
              || (print_with_color && (is_colored (C_EXEC)
                                       || is_colored (C_SETUID)
                                       || is_colored (C_SETGID))))));

  /* Absolute name of this file, if needed.  */
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ತುಂಬಿದ_name = name;
  ಆದರೆ (check_stat | print_scontext | format_needs_capability
      && name[0] != '/' && dirname)
    {
      ಅಕ್ಷರ *p = alloca (ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (name) + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (dirname) + 2);
      attach (p, dirname, name);
      ತುಂಬಿದ_name = p;
    }

  bool do_deref;

  ಆದರೆ (!check_stat)
    do_deref = dereference == DEREF_ALWAYS;
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಪೂರ್ಣಾಂಕ err;

      ಆದರೆ (print_hyperlink)
        {
          f->absolute_name = canonicalize_filename_ಬಹುಲಕ (ತುಂಬಿದ_name,
                                                         CAN_MISSING);
          ಆದರೆ (! f->absolute_name
              && (command_ಸಾಲು_arg
                  || ! ignorable_traversal_errno (errno)))
            file_failure (command_ಸಾಲು_arg,
                          _("error canonicalizing %s"), ತುಂಬಿದ_name);
        }

      ಆಯ್ಕೆ (dereference)
        {
        ಸ್ಥಿತಿ DEREF_ALWAYS:
          err = do_stat (ತುಂಬಿದ_name, &f->stat);
          do_deref = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ DEREF_COMMAND_ಸಾಲು_ARGUMENTS:
        ಸ್ಥಿತಿ DEREF_COMMAND_ಸಾಲು_SYMLINK_ಗಮ್ಯ_DIR:
          ಆದರೆ (command_ಸಾಲು_arg)
            {
              bool need_lstat;
              err = do_stat (ತುಂಬಿದ_name, &f->stat);
              do_deref = true;

              ಆದರೆ (dereference == DEREF_COMMAND_ಸಾಲು_ARGUMENTS)
                ನಿಲ್ಲಿಸು;

              need_lstat = (err < 0
                            ? (errno == ENOENT || errno == ELOOP)
                            : ! S_ISDIR (f->stat.st_ಬಹುಲಕ));
              ಆದರೆ (!need_lstat)
                ನಿಲ್ಲಿಸು;

              /* stat failed because of ENOENT || ELOOP, maybe indicating a
                 non-traversable symlink.  Or stat succeeded,
                 FULL_NAME does not refer to a directory,
                 and --dereference-command-line-symlink-to-dir is in effect.
                 Fall through so that we call lstat instead.  */
            }
          FALLTHROUGH;

        ಸ್ಥಿತಿ DEREF_NEVER:
          err = do_lstat (ತುಂಬಿದ_name, &f->stat);
          do_deref = false;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ DEREF_UNDEFINED: ಪೂರ್ವನಿಯೋಜಿತ:
          affirm (false);
        }

      ಆದರೆ (err != 0)
        {
          ಆದರೆ (! command_ಸಾಲು_arg && ignorable_traversal_errno (errno))
            {
              ಪೂರ್ಣಾಂಕ stat_errno = errno;
              ರಚನೆ stat linkstat;
              bool dangling_link = (do_deref
                                    && (type == symbolic_link
                                        || type == unknown)
                                    && do_lstat (ತುಂಬಿದ_name, &linkstat) == 0);
              errno = stat_errno;
              ಆದರೆ (! dangling_link)
                {
                  free_ent (f);
                  ಹಿಂತಿರುಗಿ 0;
                }
            }

          /* Failure to stat a command line argument leads to
             an exit status of 2.  For other files, stat failure
             provokes an exit status of 1.  */
          file_failure (command_ಸಾಲು_arg,
                        _("cannot access %s"), ತುಂಬಿದ_name);

          ಆದರೆ (command_ಸಾಲು_arg)
            {
              free_ent (f);
              ಹಿಂತಿರುಗಿ 0;
            }

          f->name = xstrdup (name);
          cwd_n_ಬಳಸಿದ++;

          ಹಿಂತಿರುಗಿ 0;
        }

      f->stat_ok = true;
      f->filetype = type = d_type_filetype[IFTODT (f->stat.st_ಬಹುಲಕ)];
    }

  ಆದರೆ (type == directory && command_ಸಾಲು_arg && !ತಕ್ಷಣ_dirs)
    f->filetype = type = arg_directory;

  get_extended_info (ತುಂಬಿದ_name, f, do_deref);

  ಆದರೆ ((type == symbolic_link)
      & ((format == long_format) | check_symlink_ಬಹುಲಕ))
    {
      ರಚನೆ stat linkstats;

      ಆದರೆ (! get_link_name (ತುಂಬಿದ_name, f, command_ಸಾಲು_arg))
        {
          free_ent (f);
          ಹಿಂತಿರುಗಿ 0;
        }

      /* Use the slower quoting path for this entry, though
         don't update CWD_SOME_QUOTED since alignment not affected.  */
      ಆದರೆ (f->linkname && f->quoted == 0 && needs_quoting (f->linkname))
        f->quoted = -1;

      /* Avoid following symbolic links when possible, i.e., when
         they won't be traced and when no indicator is needed.  */
      ಆದರೆ (f->linkname
          && (file_type <= indicator_style || check_symlink_ಬಹುಲಕ)
          && stat_for_ಬಹುಲಕ (ತುಂಬಿದ_name, &linkstats) == 0)
        {
          f->linkok = true;
          f->linkmode = linkstats.st_ಬಹುಲಕ;
        }
    }

  ಖಂಡಗಳು = STP_NBLOCKS (&f->stat);
  ಆದರೆ (format == long_format || print_ಖಂಡ_ಗಾತ್ರ)
    {
      ಅಕ್ಷರ buf[LONGEST_HUMAN_READABLE + 1];
      ಪೂರ್ಣಾಂಕ len = mbswidth (human_readable (ಖಂಡಗಳು, buf, human_output_opts,
                                          ST_NBLOCKSIZE, output_ಖಂಡ_ಗಾತ್ರ),
                          MBSWIDTH_FLAGS);
      ಆದರೆ (ಖಂಡ_ಗಾತ್ರ_ಅಗಲ < len)
        ಖಂಡ_ಗಾತ್ರ_ಅಗಲ = len;
    }

  ಆದರೆ (format == long_format)
    {
      ಆದರೆ (print_owner)
        {
          ಪೂರ್ಣಾಂಕ len = format_user_ಅಗಲ (f->stat.st_uid);
          ಆದರೆ (owner_ಅಗಲ < len)
            owner_ಅಗಲ = len;
        }

      ಆದರೆ (print_ಗುಂಪು)
        {
          ಪೂರ್ಣಾಂಕ len = format_ಗುಂಪು_ಅಗಲ (f->stat.st_gid);
          ಆದರೆ (ಗುಂಪು_ಅಗಲ < len)
            ಗುಂಪು_ಅಗಲ = len;
        }

      ಆದರೆ (print_author)
        {
          ಪೂರ್ಣಾಂಕ len = format_user_ಅಗಲ (f->stat.st_author);
          ಆದರೆ (author_ಅಗಲ < len)
            author_ಅಗಲ = len;
        }
    }

  ಆದರೆ (print_scontext)
    {
      ಪೂರ್ಣಾಂಕ len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (f->scontext);
      ಆದರೆ (scontext_ಅಗಲ < len)
        scontext_ಅಗಲ = len;
    }

  ಆದರೆ (format == long_format)
    {
      ಅಕ್ಷರ b[INT_BUFSIZE_BOUND (uintmax_t)];
      ಪೂರ್ಣಾಂಕ b_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (umaxtostr (f->stat.st_nlink, b));
      ಆದರೆ (nlink_ಅಗಲ < b_len)
        nlink_ಅಗಲ = b_len;

      ಆದರೆ ((type == chardev) | (type == blockdev))
        {
          ಅಕ್ಷರ buf[INT_BUFSIZE_BOUND (uintmax_t)];
          ಪೂರ್ಣಾಂಕ len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (umaxtostr (major (f->stat.st_rdev), buf));
          ಆದರೆ (major_device_ಸಂಖ್ಯೆ_ಅಗಲ < len)
            major_device_ಸಂಖ್ಯೆ_ಅಗಲ = len;
          len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (umaxtostr (minor (f->stat.st_rdev), buf));
          ಆದರೆ (minor_device_ಸಂಖ್ಯೆ_ಅಗಲ < len)
            minor_device_ಸಂಖ್ಯೆ_ಅಗಲ = len;
          len = major_device_ಸಂಖ್ಯೆ_ಅಗಲ + 2 + minor_device_ಸಂಖ್ಯೆ_ಅಗಲ;
          ಆದರೆ (file_ಗಾತ್ರ_ಅಗಲ < len)
            file_ಗಾತ್ರ_ಅಗಲ = len;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಅಕ್ಷರ buf[LONGEST_HUMAN_READABLE + 1];
          uintmax_t ಗಾತ್ರ_2 = unsigned_file_ಗಾತ್ರ (f->stat.st_ಗಾತ್ರ);
          ಪೂರ್ಣಾಂಕ len = mbswidth (human_readable (ಗಾತ್ರ_2, buf,
                                              file_human_output_opts,
                                              1, file_output_ಖಂಡ_ಗಾತ್ರ),
                              MBSWIDTH_FLAGS);
          ಆದರೆ (file_ಗಾತ್ರ_ಅಗಲ < len)
            file_ಗಾತ್ರ_ಅಗಲ = len;
        }
    }

  ಆದರೆ (print_inode)
    {
      ಅಕ್ಷರ buf[INT_BUFSIZE_BOUND (uintmax_t)];
      ಪೂರ್ಣಾಂಕ len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (umaxtostr (f->stat.st_ino, buf));
      ಆದರೆ (inode_ಸಂಖ್ಯೆ_ಅಗಲ < len)
        inode_ಸಂಖ್ಯೆ_ಅಗಲ = len;
    }

  f->name = xstrdup (name);
  cwd_n_ಬಳಸಿದ++;

  ಹಿಂತಿರುಗಿ ಖಂಡಗಳು;
}

/* Return true if F refers to a directory.  */
ಸ್ಥಿರ bool
is_directory (ಸ್ಥಿರಾಂಕ ರಚನೆ fileinfo *f)
{
  ಹಿಂತಿರುಗಿ f->filetype == directory || f->filetype == arg_directory;
}

/* Return true if F refers to a (symlinked) directory.  */
ಸ್ಥಿರ bool
is_linked_directory (ಸ್ಥಿರಾಂಕ ರಚನೆ fileinfo *f)
{
  ಹಿಂತಿರುಗಿ f->filetype == directory || f->filetype == arg_directory
         || S_ISDIR (f->linkmode);
}

/* Put the name of the file that FILENAME is a symbolic link to
   into the LINKNAME field of 'f'.  COMMAND_LINE_ARG indicates whether
   FILENAME is a command-line argument.  */

ಸ್ಥಿರ bool
get_link_name (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename, ರಚನೆ fileinfo *f, bool command_ಸಾಲು_arg)
{
  f->linkname = areadlink_with_ಗಾತ್ರ (filename, f->stat.st_ಗಾತ್ರ);
  ಆದರೆ (f->linkname == ಖಾಲಿ)
    {
      ಆದರೆ (! command_ಸಾಲು_arg
          && ignorable_traversal_errno (errno))
        ಹಿಂತಿರುಗಿ false;
      file_failure (command_ಸಾಲು_arg, _("cannot read symbolic link %s"),
                    filename);
    }
  ಹಿಂತಿರುಗಿ true;
}

/* Return true if the last component of NAME is '.' or '..'
   This is so we don't try to recurse on '././././. ...' */

ಸ್ಥಿರ bool
basename_is_dot_or_dotdot (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name)
{
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ತಳ_2 = ಕೊನೆಯ_component (name);
  ಹಿಂತಿರುಗಿ dot_or_dotdot (ತಳ_2);
}

/* Remove any entries from CWD_FILE that are for directories,
   and queue them to be listed as directories instead.
   DIRNAME is the prefix to prepend to each dirname
   to make it correct relative to ls's working dir;
   if it is null, no prefix is needed and "." and ".." should not be ignored.
   If COMMAND_LINE_ARG is true, this directory was mentioned at the top level,
   This is desirable when processing directories recursively.  */

ಸ್ಥಿರ ಶೂನ್ಯ
ಹೊರತೆಗೆ_dirs_from_files (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dirname, bool command_ಸಾಲು_arg)
{
  idx_t i, j;
  bool ignore_dot_and_dot_dot = (dirname != ಖಾಲಿ);

  ಆದರೆ (dirname && LOOP_DETECT)
    {
      /* Insert a marker entry first.  When we dequeue this marker entry,
         we'll know that DIRNAME has been processed and may be removed
         from the set of active directories.  */
      ಸರತಿ_directory (ಖಾಲಿ, dirname, false);
    }

  /* Queue the directories last one first, because queueing reverses the
     order.  */
  ಗಾಗಿ (i = cwd_n_ಬಳಸಿದ; 0 < i; )
    {
      i--;
      ರಚನೆ fileinfo *f = sorted_file[i];

      ಆದರೆ (is_directory (f)
          && (! ignore_dot_and_dot_dot
              || ! basename_is_dot_or_dotdot (f->name)))
        {
          ಆದರೆ (!dirname || f->name[0] == '/')
            ಸರತಿ_directory (f->name, f->linkname, command_ಸಾಲು_arg);
          ಇಲ್ಲದಿದ್ದರೆ
            {
              ಅಕ್ಷರ *name = file_name_concat (dirname, f->name, ಖಾಲಿ);
              ಸರತಿ_directory (name, f->linkname, command_ಸಾಲು_arg);
              ಬಿಡುಗಡೆ (name);
            }
          ಆದರೆ (f->filetype == arg_directory)
            free_ent (f);
        }
    }

  /* Now delete the directories from the table, compacting all the remaining
     entries.  */

  ಗಾಗಿ (i = 0, j = 0; i < cwd_n_ಬಳಸಿದ; i++)
    {
      ರಚನೆ fileinfo *f = sorted_file[i];
      sorted_file[j] = f;
      j += (f->filetype != arg_directory);
    }
  cwd_n_ಬಳಸಿದ = j;
}

/* Use strcoll to compare strings in this locale.  If an error occurs,
   report an error and longjmp to failed_strcoll.  */

ಸ್ಥಿರ jmp_buf failed_strcoll;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
xstrcoll (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *a, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *b)
{
  ಪೂರ್ಣಾಂಕ diff;
  errno = 0;
  diff = strcoll (a, b);
  ಆದರೆ (errno)
    {
      error (0, errno, _("cannot compare file names %s and %s"),
             quote_n (0, a), quote_n (1, b));
      ಗಣ_exit_ಸ್ಥಿತಿ (false);
      longjmp (failed_strcoll, 1);
    }
  ಹಿಂತಿರುಗಿ diff;
}

/* Comparison routines for sorting the files.  */

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *V;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಪೂರ್ಣಾಂಕ (*qsortFunc)(V a, V b);

/* Used below in DEFINE_SORT_FUNCTIONS for _df_ sort function variants.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
dirfirst_check (ರಚನೆ fileinfo ಸ್ಥಿರಾಂಕ *a, ರಚನೆ fileinfo ಸ್ಥಿರಾಂಕ *b,
                ಪೂರ್ಣಾಂಕ (*cmp) (V, V))
{
  ಪೂರ್ಣಾಂಕ diff = is_linked_directory (b) - is_linked_directory (a);
  ಹಿಂತಿರುಗಿ diff ? diff : cmp (a, b);
}

/* Define the 8 different sort function variants required for each sortkey.
   KEY_NAME is a token describing the sort key, e.g., ctime, atime, size.
   KEY_CMP_FUNC is a function to compare records based on that key, e.g.,
   ctime_cmp, atime_cmp, size_cmp.  Append KEY_NAME to the string,
   '[rev_][x]str{cmp|coll}[_df]_', to create each function name.  */
#ವ್ಯಾಖ್ಯಾನಿಸು DEFINE_ವಿಂಗಡಿಸು_FUNCTIONS(ಕೀಲಿ_name, ಕೀಲಿ_cmp_func)			\
  /* direct, non-dirfirst versions */					\
  ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ xstrcoll_##key_name (V a, V b)				\
  { ಹಿಂತಿರುಗಿ ಕೀಲಿ_cmp_func (a, b, xstrcoll); }				\
  ATTRIBUTE_ಶುದ್ಧ ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ strcmp_##key_name (V a, V b)		\
  { ಹಿಂತಿರುಗಿ ಕೀಲಿ_cmp_func (a, b, strcmp); }				\
                                                                        \
  /* reverse, non-dirfirst versions */					\
  ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ rev_xstrcoll_##key_name (V a, V b)				\
  { ಹಿಂತಿರುಗಿ ಕೀಲಿ_cmp_func (b, a, xstrcoll); }				\
  ATTRIBUTE_ಶುದ್ಧ ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ rev_strcmp_##key_name (V a, V b)	\
  { ಹಿಂತಿರುಗಿ ಕೀಲಿ_cmp_func (b, a, strcmp); }				\
                                                                        \
  /* direct, dirfirst versions */					\
  ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ xstrcoll_df_##key_name (V a, V b)				\
  { ಹಿಂತಿರುಗಿ dirfirst_check (a, b, xstrcoll_##key_name); }		\
  ATTRIBUTE_ಶುದ್ಧ ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ strcmp_df_##key_name (V a, V b)		\
  { ಹಿಂತಿರುಗಿ dirfirst_check (a, b, strcmp_##key_name); }			\
                                                                        \
  /* reverse, dirfirst versions */					\
  ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ rev_xstrcoll_df_##key_name (V a, V b)			\
  { ಹಿಂತಿರುಗಿ dirfirst_check (a, b, rev_xstrcoll_##key_name); }		\
  ATTRIBUTE_ಶುದ್ಧ ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ rev_strcmp_df_##key_name (V a, V b)	\
  { ಹಿಂತಿರುಗಿ dirfirst_check (a, b, rev_strcmp_##key_name); }

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
cmp_ctime (ರಚನೆ fileinfo ಸ್ಥಿರಾಂಕ *a, ರಚನೆ fileinfo ಸ್ಥಿರಾಂಕ *b,
           ಪೂರ್ಣಾಂಕ (*cmp) (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *))
{
  ಪೂರ್ಣಾಂಕ diff = timespec_cmp (get_stat_ctime (&b->stat),
                           get_stat_ctime (&a->stat));
  ಹಿಂತಿರುಗಿ diff ? diff : cmp (a->name, b->name);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
cmp_mtime (ರಚನೆ fileinfo ಸ್ಥಿರಾಂಕ *a, ರಚನೆ fileinfo ಸ್ಥಿರಾಂಕ *b,
           ಪೂರ್ಣಾಂಕ (*cmp) (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *))
{
  ಪೂರ್ಣಾಂಕ diff = timespec_cmp (get_stat_mtime (&b->stat),
                           get_stat_mtime (&a->stat));
  ಹಿಂತಿರುಗಿ diff ? diff : cmp (a->name, b->name);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
cmp_atime (ರಚನೆ fileinfo ಸ್ಥಿರಾಂಕ *a, ರಚನೆ fileinfo ಸ್ಥಿರಾಂಕ *b,
           ಪೂರ್ಣಾಂಕ (*cmp) (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *))
{
  ಪೂರ್ಣಾಂಕ diff = timespec_cmp (get_stat_atime (&b->stat),
                           get_stat_atime (&a->stat));
  ಹಿಂತಿರುಗಿ diff ? diff : cmp (a->name, b->name);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
cmp_btime (ರಚನೆ fileinfo ಸ್ಥಿರಾಂಕ *a, ರಚನೆ fileinfo ಸ್ಥಿರಾಂಕ *b,
           ಪೂರ್ಣಾಂಕ (*cmp) (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *))
{
  ಪೂರ್ಣಾಂಕ diff = timespec_cmp (get_stat_btime (&b->stat),
                           get_stat_btime (&a->stat));
  ಹಿಂತಿರುಗಿ diff ? diff : cmp (a->name, b->name);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
off_cmp (off_t a, off_t b)
{
  ಹಿಂತಿರುಗಿ _GL_CMP (a, b);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
cmp_ಗಾತ್ರ (ರಚನೆ fileinfo ಸ್ಥಿರಾಂಕ *a, ರಚನೆ fileinfo ಸ್ಥಿರಾಂಕ *b,
          ಪೂರ್ಣಾಂಕ (*cmp) (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *))
{
  ಪೂರ್ಣಾಂಕ diff = off_cmp (b->stat.st_ಗಾತ್ರ, a->stat.st_ಗಾತ್ರ);
  ಹಿಂತಿರುಗಿ diff ? diff : cmp (a->name, b->name);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
cmp_name (ರಚನೆ fileinfo ಸ್ಥಿರಾಂಕ *a, ರಚನೆ fileinfo ಸ್ಥಿರಾಂಕ *b,
          ಪೂರ್ಣಾಂಕ (*cmp) (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *))
{
  ಹಿಂತಿರುಗಿ cmp (a->name, b->name);
}

/* Compare file extensions.  Files with no extension are 'smallest'.
   If extensions are the same, compare by file names instead.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
cmp_extension (ರಚನೆ fileinfo ಸ್ಥಿರಾಂಕ *a, ರಚನೆ fileinfo ಸ್ಥಿರಾಂಕ *b,
               ಪೂರ್ಣಾಂಕ (*cmp) (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *))
{
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *base1 = strrchr (a->name, '.');
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *base2 = strrchr (b->name, '.');
  ಪೂರ್ಣಾಂಕ diff = cmp (base1 ? base1 : "", base2 ? base2 : "");
  ಹಿಂತಿರುಗಿ diff ? diff : cmp (a->name, b->name);
}

/* Return the (cached) screen width,
   for the NAME associated with the passed fileinfo F.  */

ಸ್ಥಿರ size_t
fileinfo_name_ಅಗಲ (ರಚನೆ fileinfo ಸ್ಥಿರಾಂಕ *f)
{
  ಹಿಂತಿರುಗಿ f->ಅಗಲ
         ? f->ಅಗಲ
         : quote_name_ಅಗಲ (f->name, filename_quoting_options, f->quoted);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
cmp_ಅಗಲ (ರಚನೆ fileinfo ಸ್ಥಿರಾಂಕ *a, ರಚನೆ fileinfo ಸ್ಥಿರಾಂಕ *b,
          ಪೂರ್ಣಾಂಕ (*cmp) (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *))
{
  ಪೂರ್ಣಾಂಕ diff = fileinfo_name_ಅಗಲ (a) - fileinfo_name_ಅಗಲ (b);
  ಹಿಂತಿರುಗಿ diff ? diff : cmp (a->name, b->name);
}

DEFINE_ವಿಂಗಡಿಸು_FUNCTIONS (ctime, cmp_ctime)
DEFINE_ವಿಂಗಡಿಸು_FUNCTIONS (mtime, cmp_mtime)
DEFINE_ವಿಂಗಡಿಸು_FUNCTIONS (atime, cmp_atime)
DEFINE_ವಿಂಗಡಿಸು_FUNCTIONS (btime, cmp_btime)
DEFINE_ವಿಂಗಡಿಸು_FUNCTIONS (ಗಾತ್ರ_2, cmp_ಗಾತ್ರ)
DEFINE_ವಿಂಗಡಿಸು_FUNCTIONS (name, cmp_name)
DEFINE_ವಿಂಗಡಿಸು_FUNCTIONS (extension, cmp_extension)
DEFINE_ವಿಂಗಡಿಸು_FUNCTIONS (ಅಗಲ, cmp_ಅಗಲ)

/* Compare file versions.
   Unlike the other compare functions, cmp_version does not fail
   because filevercmp and strcmp do not fail; cmp_version uses strcmp
   instead of xstrcoll because filevercmp is locale-independent so
   strcmp is its appropriate secondary.

   All the other sort options need xstrcoll and strcmp variants,
   because they all use xstrcoll (either as the primary or secondary
   sort key), and xstrcoll has the ability to do a longjmp if strcoll fails for
   locale reasons.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
cmp_ಆವೃತ್ತಿ (ರಚನೆ fileinfo ಸ್ಥಿರಾಂಕ *a, ರಚನೆ fileinfo ಸ್ಥಿರಾಂಕ *b)
{
  ಪೂರ್ಣಾಂಕ diff = filevercmp (a->name, b->name);
  ಹಿಂತಿರುಗಿ diff ? diff : strcmp (a->name, b->name);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
xstrcoll_ಆವೃತ್ತಿ (V a, V b)
{
  ಹಿಂತಿರುಗಿ cmp_ಆವೃತ್ತಿ (a, b);
}
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
rev_xstrcoll_ಆವೃತ್ತಿ (V a, V b)
{
  ಹಿಂತಿರುಗಿ cmp_ಆವೃತ್ತಿ (b, a);
}
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
xstrcoll_df_ಆವೃತ್ತಿ (V a, V b)
{
  ಹಿಂತಿರುಗಿ dirfirst_check (a, b, xstrcoll_ಆವೃತ್ತಿ);
}
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
rev_xstrcoll_df_ಆವೃತ್ತಿ (V a, V b)
{
  ಹಿಂತಿರುಗಿ dirfirst_check (a, b, rev_xstrcoll_ಆವೃತ್ತಿ);
}


/* We have 2^3 different variants for each sort-key function
   (for 3 independent sort modes).
   The function pointers stored in this array must be dereferenced as:

    sort_variants[sort_key][use_strcmp][reverse][dirs_first]

   Note that the order in which sort keys are listed in the function pointer
   array below is defined by the order of the elements in the time_type and
   sort_type enums!  */

#ವ್ಯಾಖ್ಯಾನಿಸು LIST_SORTFUNCTION_VARIANTS(ಕೀಲಿ_name)                        \
  {                                                                 \
    {                                                               \
      { xstrcoll_##key_name, xstrcoll_df_##key_name },              \
      { rev_xstrcoll_##key_name, rev_xstrcoll_df_##key_name },      \
    },                                                              \
    {                                                               \
      { strcmp_##key_name, strcmp_df_##key_name },                  \
      { rev_strcmp_##key_name, rev_strcmp_df_##key_name },          \
    }                                                               \
  }

ಸ್ಥಿರ qsortFunc ಸ್ಥಿರಾಂಕ ವಿಂಗಡಿಸು_functions[][2][2][2] =
  {
    LIST_SORTFUNCTION_VARIANTS (name),
    LIST_SORTFUNCTION_VARIANTS (extension),
    LIST_SORTFUNCTION_VARIANTS (ಅಗಲ),
    LIST_SORTFUNCTION_VARIANTS (ಗಾತ್ರ_2),

    {
      {
        { xstrcoll_ಆವೃತ್ತಿ, xstrcoll_df_ಆವೃತ್ತಿ },
        { rev_xstrcoll_ಆವೃತ್ತಿ, rev_xstrcoll_df_ಆವೃತ್ತಿ },
      },

      /* We use NULL for the strcmp variants of version comparison
         since as explained in cmp_version definition, version comparison
         does not rely on xstrcoll, so it will never longjmp, and never
         need to try the strcmp fallback. */
      {
        { ಖಾಲಿ, ಖಾಲಿ },
        { ಖಾಲಿ, ಖಾಲಿ },
      }
    },

    /* last are time sort functions */
    LIST_SORTFUNCTION_VARIANTS (mtime),
    LIST_SORTFUNCTION_VARIANTS (ctime),
    LIST_SORTFUNCTION_VARIANTS (atime),
    LIST_SORTFUNCTION_VARIANTS (btime)
  };

/* The number of sort keys is calculated as the sum of
     the number of elements in the sort_type enum (i.e., sort_numtypes)
     -2 because neither sort_time nor sort_none use entries themselves
     the number of elements in the time_type enum (i.e., time_numtypes)
   This is because when sort_type==sort_time, we have up to
   time_numtypes possible sort keys.

   This line verifies at compile-time that the array of sort functions has been
   initialized for all possible sort keys. */
static_assert (countof (ವಿಂಗಡಿಸು_functions) == ವಿಂಗಡಿಸು_numtypes - 2 + ಸಮಯ_numtypes);

/* Set up SORTED_FILE to point to the in-use entries in CWD_FILE, in order.  */

ಸ್ಥಿರ ಶೂನ್ಯ
initialize_ordering_ಸದಿಶ (ಶೂನ್ಯ)
{
  ಗಾಗಿ (idx_t i = 0; i < cwd_n_ಬಳಸಿದ; i++)
    sorted_file[i] = &cwd_file[i];
}

/* Cache values based on attributes global to all files.  */

ಸ್ಥಿರ ಶೂನ್ಯ
update_current_files_info (ಶೂನ್ಯ)
{
  /* Cache screen width of name, if needed multiple times.  */
  ಆದರೆ (ವಿಂಗಡಿಸು_type == ವಿಂಗಡಿಸು_ಅಗಲ
      || (ಸಾಲು_ಉದ್ದಳತೆ && (format == many_ಪ್ರತಿ_ಸಾಲು || format == horizontal)))
    {
      ಗಾಗಿ (idx_t i = 0; i < cwd_n_ಬಳಸಿದ; i++)
        {
          ರಚನೆ fileinfo *f = sorted_file[i];
          f->ಅಗಲ = fileinfo_name_ಅಗಲ (f);
        }
    }
}

/* Sort the files now in the table.  */

ಸ್ಥಿರ ಶೂನ್ಯ
ವಿಂಗಡಿಸು_files (ಶೂನ್ಯ)
{
  bool use_strcmp;

  ಆದರೆ (sorted_file_alloc < cwd_n_ಬಳಸಿದ + (cwd_n_ಬಳಸಿದ >> 1))
    {
      ಬಿಡುಗಡೆ (sorted_file);
      sorted_file = xinmalloc (cwd_n_ಬಳಸಿದ, 3 * ಗಾತ್ರ *sorted_file);
      sorted_file_alloc = 3 * cwd_n_ಬಳಸಿದ;
    }

  initialize_ordering_ಸದಿಶ ();

  update_current_files_info ();

  ಆದರೆ (ವಿಂಗಡಿಸು_type == ವಿಂಗಡಿಸು_none)
    ಹಿಂತಿರುಗಿ;

  /* Try strcoll.  If it fails, fall back on strcmp.  We can't safely
     ignore strcoll failures, as a failing strcoll might be a
     comparison function that is not a total order, and if we ignored
     the failure this might cause qsort to dump core.  */

  ಆದರೆ (! setjmp (failed_strcoll))
    use_strcmp = false;      /* strcoll() succeeded */
  ಇಲ್ಲದಿದ್ದರೆ
    {
      use_strcmp = true;
      affirm (ವಿಂಗಡಿಸು_type != ವಿಂಗಡಿಸು_ಆವೃತ್ತಿ);
      initialize_ordering_ಸದಿಶ ();
    }

  /* When sort_type == sort_time, use time_type as subindex.  */
  mpsort ((ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ **) sorted_file, cwd_n_ಬಳಸಿದ,
          ವಿಂಗಡಿಸು_functions[ವಿಂಗಡಿಸು_type + (ವಿಂಗಡಿಸು_type == ವಿಂಗಡಿಸು_ಸಮಯ ? ಸಮಯ_type : 0)]
                        [use_strcmp][ವಿಂಗಡಿಸು_ಹಿಮ್ಮುಖ]
                        [directories_ಮೊದಲ]);
}

/* List all the files now in the table.  */

ಸ್ಥಿರ ಶೂನ್ಯ
print_current_files (ಶೂನ್ಯ)
{
  ಆಯ್ಕೆ (format)
    {
    ಸ್ಥಿತಿ one_ಪ್ರತಿ_ಸಾಲು:
      ಗಾಗಿ (idx_t i = 0; i < cwd_n_ಬಳಸಿದ; i++)
        {
          print_file_name_and_frills (sorted_file[i], 0);
          ಅಕ್ಷರ_ಬರೆ (eolbyte);
        }
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ many_ಪ್ರತಿ_ಸಾಲು:
      ಆದರೆ (! ಸಾಲು_ಉದ್ದಳತೆ)
        print_with_separator (' ');
      ಇಲ್ಲದಿದ್ದರೆ
        print_many_ಪ್ರತಿ_ಸಾಲು ();
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ horizontal:
      ಆದರೆ (! ಸಾಲು_ಉದ್ದಳತೆ)
        print_with_separator (' ');
      ಇಲ್ಲದಿದ್ದರೆ
        print_horizontal ();
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ with_commas:
      print_with_separator (',');
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ long_format:
      ಗಾಗಿ (idx_t i = 0; i < cwd_n_ಬಳಸಿದ; i++)
        {
          ಗಣ_ಸಾಮಾನ್ಯ_color ();
          print_long_format (sorted_file[i]);
          dired_outbyte (eolbyte);
        }
      ನಿಲ್ಲಿಸು;
    }
}

/* Replace the first %b with precomputed aligned month names.
   Note on glibc-2.7 at least, this speeds up the whole 'ls -lU'
   process by around 17%, compared to letting strftime() handle the %b.  */

ಸ್ಥಿರ ptrdiff_t
align_nstrftime (ಅಕ್ಷರ *buf, size_t ಗಾತ್ರ_2, bool recent, ರಚನೆ tm ಸ್ಥಿರಾಂಕ *tm,
                 timezone_t tz, ಪೂರ್ಣಾಂಕ ns)
{
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *nfmt = (use_abformat
                      ? abformat[recent][tm->tm_mon]
                      : long_ಸಮಯ_format[recent]);
  ಹಿಂತಿರುಗಿ nstrftime (buf, ಗಾತ್ರ_2, nfmt, tm, tz, ns);
}

/* Return the expected number of columns in a long-format timestamp,
   or zero if it cannot be calculated.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
long_ಸಮಯ_expected_ಅಗಲ (ಶೂನ್ಯ)
{
  ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಅಗಲ = -1;

  ಆದರೆ (ಅಗಲ < 0)
    {
      time_t epoch = 0;
      ರಚನೆ tm tm;
      ಅಕ್ಷರ buf[ಸಮಯ_STAMP_LEN_ಗರಿಷ್ಠ + 1];

      /* In case you're wondering if localtime_rz can fail with an input time_t
         value of 0, let's just say it's very unlikely, but not inconceivable.
         The TZ environment variable would have to specify a time zone that
         is 2**31-1900 years or more ahead of UTC.  This could happen only on
         a 64-bit system that blindly accepts e.g., TZ=UTC+20000000000000.
         However, this is not possible with Solaris 10 or glibc-2.3.5, since
         their implementations limit the offset to 167:59 and 24:00, resp.  */
      ಆದರೆ (localtime_rz (localtz, &epoch, &tm))
        {
          ptrdiff_t len = align_nstrftime (buf, ಗಾತ್ರ buf, false,
                                           &tm, localtz, 0);
          ಆದರೆ (len > 0)
            ಅಗಲ = mbsnwidth (buf, len, MBSWIDTH_FLAGS);
        }

      ಆದರೆ (ಅಗಲ < 0)
        ಅಗಲ = 0;
    }

  ಹಿಂತಿರುಗಿ ಅಗಲ;
}

/* Print the user or group name NAME, with numeric id ID, using a
   print width of WIDTH columns.  */

ಸ್ಥಿರ ಶೂನ್ಯ
format_user_or_ಗುಂಪು (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name, uintmax_t id, ಪೂರ್ಣಾಂಕ ಅಗಲ)
{
  ಆದರೆ (name)
    {
      ಪೂರ್ಣಾಂಕ name_ಅಗಲ = mbswidth (name, MBSWIDTH_FLAGS);
      ಪೂರ್ಣಾಂಕ ಅಗಲ_gap = name_ಅಗಲ < 0 ? 0 : ಅಗಲ - name_ಅಗಲ;
      ಪೂರ್ಣಾಂಕ pad = MAX (0, ಅಗಲ_gap);
      dired_outstring (name);

      ಮಾಡು
        dired_outbyte (' ');
      ಆಗ (pad--);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    dired_pos += ಬರೆ ("%*ju ", ಅಗಲ, id);
}

/* Print the name or id of the user with id U, using a print width of
   WIDTH.  */

ಸ್ಥಿರ ಶೂನ್ಯ
format_user (uid_t u, ಪೂರ್ಣಾಂಕ ಅಗಲ, bool stat_ok)
{
  format_user_or_ಗುಂಪು (! stat_ok ? "?" :
                        (numeric_ids ? ಖಾಲಿ : getuser (u)), u, ಅಗಲ);
}

/* Likewise, for groups.  */

ಸ್ಥಿರ ಶೂನ್ಯ
format_ಗುಂಪು (gid_t g, ಪೂರ್ಣಾಂಕ ಅಗಲ, bool stat_ok)
{
  format_user_or_ಗುಂಪು (! stat_ok ? "?" :
                        (numeric_ids ? ಖಾಲಿ : getgroup (g)), g, ಅಗಲ);
}

/* Return the number of columns that format_user_or_group will print,
   or -1 if unknown.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
format_user_or_ಗುಂಪು_ಅಗಲ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name, uintmax_t id)
{
  ಹಿಂತಿರುಗಿ (name
          ? mbswidth (name, MBSWIDTH_FLAGS)
          : snprintf (ಖಾಲಿ, 0, "%ju", id));
}

/* Return the number of columns that format_user will print,
   or -1 if unknown.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
format_user_ಅಗಲ (uid_t u)
{
  ಹಿಂತಿರುಗಿ format_user_or_ಗುಂಪು_ಅಗಲ (numeric_ids ? ಖಾಲಿ : getuser (u), u);
}

/* Likewise, for groups.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
format_ಗುಂಪು_ಅಗಲ (gid_t g)
{
  ಹಿಂತಿರುಗಿ format_user_or_ಗುಂಪು_ಅಗಲ (numeric_ids ? ಖಾಲಿ : getgroup (g), g);
}

/* Return a pointer to a formatted version of F->stat.st_ino,
   possibly using buffer, which must be at least
   INT_BUFSIZE_BOUND (uintmax_t) bytes.  */
ಸ್ಥಿರ ಅಕ್ಷರ *
format_inode (ಅಕ್ಷರ buf[INT_BUFSIZE_BOUND (uintmax_t)],
              ಸ್ಥಿರಾಂಕ ರಚನೆ fileinfo *f)
{
  ಹಿಂತಿರುಗಿ (f->stat_ok && f->stat.st_ino != NOT_AN_INODE_ಸಂಖ್ಯೆ
          ? umaxtostr (f->stat.st_ino, buf)
          : (ಅಕ್ಷರ *) "?");
}

/* Print information about F in long format.  */
ಸ್ಥಿರ ಶೂನ್ಯ
print_long_format (ಸ್ಥಿರಾಂಕ ರಚನೆ fileinfo *f)
{
  ಅಕ್ಷರ modebuf[12];
  ಅಕ್ಷರ buf
    [LONGEST_HUMAN_READABLE + 1		/* inode */
     + LONGEST_HUMAN_READABLE + 1	/* size in blocks */
     + ಗಾತ್ರ (modebuf) - 1 + 1		/* mode string */
     + INT_BUFSIZE_BOUND (uintmax_t)	/* st_nlink */
     + LONGEST_HUMAN_READABLE + 2	/* major device number */
     + LONGEST_HUMAN_READABLE + 1	/* minor device number */
     + ಸಮಯ_STAMP_LEN_ಗರಿಷ್ಠ + 1	/* max length of time/date */
     ];
  ptrdiff_t s;
  ಅಕ್ಷರ *p;
  ರಚನೆ timespec when_timespec;
  ರಚನೆ tm when_local;
  bool btime_ok = true;

  /* Compute the mode string, except remove the trailing space if no
     file in this directory has an ACL or security context.  */
  ಆದರೆ (f->stat_ok)
    filemodestring (&f->stat, modebuf);
  ಇಲ್ಲದಿದ್ದರೆ
    {
      modebuf[0] = filetype_letter[f->filetype];
      memset (modebuf + 1, '?', 10);
      modebuf[11] = '\0';
    }
  ಆದರೆ (! any_has_acl)
    modebuf[10] = '\0';
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (f->acl_type == ACL_T_LSM_CONTEXT_ONLY)
    modebuf[10] = '.';
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (f->acl_type == ACL_T_YES)
    modebuf[10] = '+';
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (f->acl_type == ACL_T_UNKNOWN)
    modebuf[10] = '?';

  ಆಯ್ಕೆ (ಸಮಯ_type)
    {
    ಸ್ಥಿತಿ ಸಮಯ_ctime:
      when_timespec = get_stat_ctime (&f->stat);
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ ಸಮಯ_mtime:
      when_timespec = get_stat_mtime (&f->stat);
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ ಸಮಯ_atime:
      when_timespec = get_stat_atime (&f->stat);
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ ಸಮಯ_btime:
      when_timespec = get_stat_btime (&f->stat);
      ಆದರೆ (when_timespec.tv_sec == -1 && when_timespec.tv_nsec == -1)
        btime_ok = false;
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ ಸಮಯ_numtypes: ಪೂರ್ವನಿಯೋಜಿತ:
      unreachable ();
    }

  p = buf;

  ಆದರೆ (print_inode)
    {
      ಅಕ್ಷರ hbuf[INT_BUFSIZE_BOUND (uintmax_t)];
      p += sprintf (p, "%*s ", inode_ಸಂಖ್ಯೆ_ಅಗಲ, format_inode (hbuf, f));
    }

  ಆದರೆ (print_ಖಂಡ_ಗಾತ್ರ)
    {
      ಅಕ್ಷರ hbuf[LONGEST_HUMAN_READABLE + 1];
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಖಂಡಗಳು =
        (! f->stat_ok
         ? "?"
         : human_readable (STP_NBLOCKS (&f->stat), hbuf, human_output_opts,
                           ST_NBLOCKSIZE, output_ಖಂಡ_ಗಾತ್ರ));
      ಪೂರ್ಣಾಂಕ ಖಂಡಗಳು_ಅಗಲ = mbswidth (ಖಂಡಗಳು, MBSWIDTH_FLAGS);
      ಗಾಗಿ (ಪೂರ್ಣಾಂಕ pad = ಖಂಡಗಳು_ಅಗಲ < 0 ? 0 : ಖಂಡ_ಗಾತ್ರ_ಅಗಲ - ಖಂಡಗಳು_ಅಗಲ;
           0 < pad; pad--)
        *p++ = ' ';
      ಆಗ ((*p++ = *ಖಂಡಗಳು++))
        ಮುಂದುವರಿ;
      p[-1] = ' ';
    }

  /* The last byte of the mode string is the POSIX
     "optional alternate access method flag".  */
  {
    ಅಕ್ಷರ hbuf[INT_BUFSIZE_BOUND (uintmax_t)];
    p += sprintf (p, "%s %*s ", modebuf, nlink_ಅಗಲ,
                  ! f->stat_ok ? "?" : umaxtostr (f->stat.st_nlink, hbuf));
  }

  dired_indent ();

  ಆದರೆ (print_owner || print_ಗುಂಪು || print_author || print_scontext)
    {
      dired_outbuf (buf, p - buf);

      ಆದರೆ (print_owner)
        format_user (f->stat.st_uid, owner_ಅಗಲ, f->stat_ok);

      ಆದರೆ (print_ಗುಂಪು)
        format_ಗುಂಪು (f->stat.st_gid, ಗುಂಪು_ಅಗಲ, f->stat_ok);

      ಆದರೆ (print_author)
        format_user (f->stat.st_author, author_ಅಗಲ, f->stat_ok);

      ಆದರೆ (print_scontext)
        format_user_or_ಗುಂಪು (f->scontext, 0, scontext_ಅಗಲ);

      p = buf;
    }

  ಆದರೆ (f->stat_ok
      && (S_ISCHR (f->stat.st_ಬಹುಲಕ) || S_ISBLK (f->stat.st_ಬಹುಲಕ)))
    {
      ಪೂರ್ಣಾಂಕ blanks_ಅಗಲ = (file_ಗಾತ್ರ_ಅಗಲ
                          - (major_device_ಸಂಖ್ಯೆ_ಅಗಲ + 2
                             + minor_device_ಸಂಖ್ಯೆ_ಅಗಲ));
      p += sprintf (p, "%*ju, %*ju ",
                    major_device_ಸಂಖ್ಯೆ_ಅಗಲ + MAX (0, blanks_ಅಗಲ),
                    (uintmax_t) major (f->stat.st_rdev),
                    minor_device_ಸಂಖ್ಯೆ_ಅಗಲ,
                    (uintmax_t) minor (f->stat.st_rdev));
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಅಕ್ಷರ hbuf[LONGEST_HUMAN_READABLE + 1];
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಗಾತ್ರ_2 =
        (! f->stat_ok
         ? "?"
         : human_readable (unsigned_file_ಗಾತ್ರ (f->stat.st_ಗಾತ್ರ),
                           hbuf, file_human_output_opts, 1,
                           file_output_ಖಂಡ_ಗಾತ್ರ));
      ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_ಅಗಲ = mbswidth (ಗಾತ್ರ_2, MBSWIDTH_FLAGS);
      ಗಾಗಿ (ಪೂರ್ಣಾಂಕ pad = ಗಾತ್ರ_ಅಗಲ < 0 ? 0 : file_ಗಾತ್ರ_ಅಗಲ - ಗಾತ್ರ_ಅಗಲ;
           0 < pad; pad--)
        *p++ = ' ';
      ಆಗ ((*p++ = *ಗಾತ್ರ_2++))
        ಮುಂದುವರಿ;
      p[-1] = ' ';
    }

  s = -1;

  ಆದರೆ (f->stat_ok && btime_ok
      && localtime_rz (localtz, &when_timespec.tv_sec, &when_local))
    {
      ರಚನೆ timespec six_months_ago;
      bool recent;

      /* If the file appears to be in the future, update the current
         time, in case the file happens to have been modified since
         the last time we checked the clock.  */
      ಆದರೆ (timespec_cmp (current_ಸಮಯ, when_timespec) < 0)
        gettime (&current_ಸಮಯ);

      /* Consider a time to be recent if it is within the past six months.
         A Gregorian year has 365.2425 * 24 * 60 * 60 == 31556952 seconds
         on the average.  Write this value as an integer constant to
         avoid floating point hassles.  */
      six_months_ago.tv_sec = current_ಸಮಯ.tv_sec - 31556952 / 2;
      six_months_ago.tv_nsec = current_ಸಮಯ.tv_nsec;

      recent = (timespec_cmp (six_months_ago, when_timespec) < 0
                && timespec_cmp (when_timespec, current_ಸಮಯ) < 0);

      /* We assume here that all time zones are offset from UTC by a
         whole number of seconds.  */
      s = align_nstrftime (p, ಸಮಯ_STAMP_LEN_ಗರಿಷ್ಠ + 1, recent,
                           &when_local, localtz, when_timespec.tv_nsec);
    }

  ಆದರೆ (0 <= s)
    {
      p += s;
      *p++ = ' ';
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      /* The time cannot be converted using the desired format, so
         print it as a huge integer number of seconds.  */
      ಅಕ್ಷರ hbuf[INT_BUFSIZE_BOUND (intmax_t)];
      p += sprintf (p, "%*s ", long_ಸಮಯ_expected_ಅಗಲ (),
                    (! f->stat_ok || ! btime_ok
                     ? "?"
                     : timetostr (when_timespec.tv_sec, hbuf)));
      /* FIXME: (maybe) We discarded when_timespec.tv_nsec. */
    }

  dired_outbuf (buf, p - buf);
  size_t w = print_name_with_quoting (f, false, &dired_obstack, p - buf);

  ಆದರೆ (f->filetype == symbolic_link)
    {
      ಆದರೆ (f->linkname)
        {
          dired_outstring (" -> ");
          print_name_with_quoting (f, true, ಖಾಲಿ, (p - buf) + w + 4);
          ಆದರೆ (indicator_style != none)
            print_type_indicator (true, f->linkmode, unknown);
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (indicator_style != none)
    print_type_indicator (f->stat_ok, f->stat.st_ಬಹುಲಕ, f->filetype);
}

/* Write to *BUF a quoted representation of the file name NAME, if non-null,
   using OPTIONS to control quoting.  *BUF is set to NAME if no quoting
   is required.  *BUF is allocated if more space required (and the original
   *BUF is not deallocated).
   Store the number of screen columns occupied by NAME's quoted
   representation into WIDTH, if non-null.
   Store into PAD whether an initial space is needed for padding.
   Return the number of bytes in *BUF.  */

ಸ್ಥಿರ size_t
quote_name_buf (ಅಕ್ಷರ **inbuf, size_t bufsize, ಅಕ್ಷರ *name,
                ರಚನೆ quoting_options ಸ್ಥಿರಾಂಕ *options,
                ಪೂರ್ಣಾಂಕ needs_general_quoting, size_t *ಅಗಲ, bool *pad)
{
  ಅಕ್ಷರ *buf = *inbuf;
  size_t displayed_ಅಗಲ IF_LINT ( = 0);
  size_t len = 0;
  bool quoted;

  ಎಣಿಕೆ quoting_style qs = get_quoting_style (options);
  bool partial_quoting = qs == shell_quoting_style
                         || qs == shell_always_quoting_style
                         || qs == literal_quoting_style;
  bool needs_further_quoting = qmark_funny_chars && partial_quoting;

  /* Note we provide extra protection only for newline separators.
     I.e., we don't bother protecting "double space" or "comma space"
     separated entries, as for interactive output, shell escape quoting
     is best to disambiguate, and for programmatic, --zero is best.
     Extra processing to mark ', ' and '  ' would be overkill
     as interactively there will still be ambiguities with multi-byte spaces,
     and marked characters impact programmatic processing anyway.  */
  bool extra_newline_protection = (!needs_further_quoting
                                   && qmark_newline && partial_quoting
                                   && !dired && strchr (name, '\n'));

  ಆದರೆ (needs_general_quoting != 0)
    {
      len = quotearg_ಬಫರ್ (buf, bufsize, name, -1, options);
      ಆದರೆ (bufsize <= len)
        {
          buf = xmalloc (len + 1);
          quotearg_ಬಫರ್ (buf, len + 1, name, -1, options);
        }

      quoted = (*name != *buf) || ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (name) != len;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (needs_further_quoting || extra_newline_protection)
    {
      len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (name);
      ಆದರೆ (bufsize <= len)
        buf = xmalloc (len + 1);
      ಸ್ಮರಣೆ_ನಕಲು (buf, name, len + 1);

      quoted = false;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (name);
      buf = name;
      quoted = false;
    }

  /* It's safe to replace newlines without multi-byte iteration, as newlines
     do not appear as part of any multi-byte encoded character.  */
  ಆದರೆ (extra_newline_protection)
    ಗಾಗಿ (ಅಕ್ಷರ *p = buf; p < buf + len; p++)
      ಆದರೆ (eolbyte && *p == '\n')
        *p = '?';

  ಆದರೆ (needs_further_quoting)
    {
      ಆದರೆ (MB_CUR_MAX > 1)
        {
          ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *p = buf;
          ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *plimit = buf + len;
          ಅಕ್ಷರ *q = buf;
          displayed_ಅಗಲ = 0;

          ಆಗ (p < plimit)
            ಆಯ್ಕೆ (*p)
              {
                ಸ್ಥಿತಿ ' ': ಸ್ಥಿತಿ '!': ಸ್ಥಿತಿ '"': ಸ್ಥಿತಿ '#': ಸ್ಥಿತಿ '%':
                ಸ್ಥಿತಿ '&': ಸ್ಥಿತಿ '\'': ಸ್ಥಿತಿ '(': ಸ್ಥಿತಿ ')': ಸ್ಥಿತಿ '*':
                ಸ್ಥಿತಿ '+': ಸ್ಥಿತಿ ',': ಸ್ಥಿತಿ '-': ಸ್ಥಿತಿ '.': ಸ್ಥಿತಿ '/':
                ಸ್ಥಿತಿ '0': ಸ್ಥಿತಿ '1': ಸ್ಥಿತಿ '2': ಸ್ಥಿತಿ '3': ಸ್ಥಿತಿ '4':
                ಸ್ಥಿತಿ '5': ಸ್ಥಿತಿ '6': ಸ್ಥಿತಿ '7': ಸ್ಥಿತಿ '8': ಸ್ಥಿತಿ '9':
                ಸ್ಥಿತಿ ':': ಸ್ಥಿತಿ ';': ಸ್ಥಿತಿ '<': ಸ್ಥಿತಿ '=': ಸ್ಥಿತಿ '>':
                ಸ್ಥಿತಿ '?':
                ಸ್ಥಿತಿ 'A': ಸ್ಥಿತಿ 'B': ಸ್ಥಿತಿ 'C': ಸ್ಥಿತಿ 'D': ಸ್ಥಿತಿ 'E':
                ಸ್ಥಿತಿ 'F': ಸ್ಥಿತಿ 'G': ಸ್ಥಿತಿ 'H': ಸ್ಥಿತಿ 'I': ಸ್ಥಿತಿ 'J':
                ಸ್ಥಿತಿ 'K': ಸ್ಥಿತಿ 'L': ಸ್ಥಿತಿ 'M': ಸ್ಥಿತಿ 'N': ಸ್ಥಿತಿ 'O':
                ಸ್ಥಿತಿ 'P': ಸ್ಥಿತಿ 'Q': ಸ್ಥಿತಿ 'R': ಸ್ಥಿತಿ 'S': ಸ್ಥಿತಿ 'T':
                ಸ್ಥಿತಿ 'U': ಸ್ಥಿತಿ 'V': ಸ್ಥಿತಿ 'W': ಸ್ಥಿತಿ 'X': ಸ್ಥಿತಿ 'Y':
                ಸ್ಥಿತಿ 'Z':
                ಸ್ಥಿತಿ '[': ಸ್ಥಿತಿ '\\': ಸ್ಥಿತಿ ']': ಸ್ಥಿತಿ '^': ಸ್ಥಿತಿ '_':
                ಸ್ಥಿತಿ 'a': ಸ್ಥಿತಿ 'b': ಸ್ಥಿತಿ 'c': ಸ್ಥಿತಿ 'd': ಸ್ಥಿತಿ 'e':
                ಸ್ಥಿತಿ 'f': ಸ್ಥಿತಿ 'g': ಸ್ಥಿತಿ 'h': ಸ್ಥಿತಿ 'i': ಸ್ಥಿತಿ 'j':
                ಸ್ಥಿತಿ 'k': ಸ್ಥಿತಿ 'l': ಸ್ಥಿತಿ 'm': ಸ್ಥಿತಿ 'n': ಸ್ಥಿತಿ 'o':
                ಸ್ಥಿತಿ 'p': ಸ್ಥಿತಿ 'q': ಸ್ಥಿತಿ 'r': ಸ್ಥಿತಿ 's': ಸ್ಥಿತಿ 't':
                ಸ್ಥಿತಿ 'u': ಸ್ಥಿತಿ 'v': ಸ್ಥಿತಿ 'w': ಸ್ಥಿತಿ 'x': ಸ್ಥಿತಿ 'y':
                ಸ್ಥಿತಿ 'z': ಸ್ಥಿತಿ '{': ಸ್ಥಿತಿ '|': ಸ್ಥಿತಿ '}': ಸ್ಥಿತಿ '~':
                  /* These characters are printable ASCII characters.  */
                  *q++ = *p++;
                  displayed_ಅಗಲ += 1;
                  ನಿಲ್ಲಿಸು;
                ಪೂರ್ವನಿಯೋಜಿತ:
                  /* If we have a multibyte sequence, copy it until we
                     reach its end, replacing each non-printable multibyte
                     character with a single question mark.  */
                  {
                    mbstate_t mbstate; mbszero (&mbstate);
                    ಮಾಡು
                      {
                        char32_t wc;
                        size_t ಬೈಟುಗಳು;
                        ಪೂರ್ಣಾಂಕ w;

                        ಬೈಟುಗಳು = mbrtoc32 (&wc, p, plimit - p, &mbstate);

                        ಆದರೆ (ಬೈಟುಗಳು == (size_t) -1)
                          {
                            /* An invalid multibyte sequence was
                               encountered.  Skip one input byte, and
                               put a question mark.  */
                            p++;
                            *q++ = '?';
                            displayed_ಅಗಲ += 1;
                            ನಿಲ್ಲಿಸು;
                          }

                        ಆದರೆ (ಬೈಟುಗಳು == (size_t) -2)
                          {
                            /* An incomplete multibyte character
                               at the end.  Replace it entirely with
                               a question mark.  */
                            p = plimit;
                            *q++ = '?';
                            displayed_ಅಗಲ += 1;
                            ನಿಲ್ಲಿಸು;
                          }

                        ಆದರೆ (ಬೈಟುಗಳು == 0)
                          /* A null wide character was encountered.  */
                          ಬೈಟುಗಳು = 1;

                        w = c32width (wc);
                        ಆದರೆ (w >= 0)
                          {
                            /* A printable multibyte character.
                               Keep it.  */
                            ಗಾಗಿ (; ಬೈಟುಗಳು > 0; --ಬೈಟುಗಳು)
                              *q++ = *p++;
                            displayed_ಅಗಲ += w;
                          }
                        ಇಲ್ಲದಿದ್ದರೆ
                          {
                            /* An nonprintable multibyte character.
                               Replace it entirely with a question
                               mark.  */
                            p += ಬೈಟುಗಳು;
                            *q++ = '?';
                            displayed_ಅಗಲ += 1;
                          }
                      }
                    ಆಗ (! mbsinit (&mbstate));
                  }
                  ನಿಲ್ಲಿಸು;
              }

          /* The buffer may have shrunk.  */
          len = q - buf;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಅಕ್ಷರ *p = buf;
          ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *plimit = buf + len;

          ಆಗ (p < plimit)
            {
              ಆದರೆ (! isprint (ಗಮ್ಯ_uchar (*p)))
                *p = '?';
              p++;
            }
          displayed_ಅಗಲ = len;
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಅಗಲ != ಖಾಲಿ)
    {
      ಆದರೆ (MB_CUR_MAX > 1)
        {
          displayed_ಅಗಲ = mbsnwidth (buf, len, MBSWIDTH_FLAGS);
          displayed_ಅಗಲ = MAX (0, displayed_ಅಗಲ);
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *p = buf;
          ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *plimit = buf + len;

          displayed_ಅಗಲ = 0;
          ಆಗ (p < plimit)
            {
              ಆದರೆ (isprint (ಗಮ್ಯ_uchar (*p)))
                displayed_ಅಗಲ++;
              p++;
            }
        }
    }

  /* Set padding to better align quoted items,
     and also give a visual indication that quotes are
     not actually part of the name.  */
  *pad = (align_variable_outer_quotes && cwd_some_quoted && ! quoted);

  ಆದರೆ (ಅಗಲ != ಖಾಲಿ)
    *ಅಗಲ = displayed_ಅಗಲ;

  *inbuf = buf;

  ಹಿಂತಿರುಗಿ len;
}

ಸ್ಥಿರ size_t
quote_name_ಅಗಲ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name, ರಚನೆ quoting_options ಸ್ಥಿರಾಂಕ *options,
                  ಪೂರ್ಣಾಂಕ needs_general_quoting)
{
  ಅಕ್ಷರ smallbuf[BUFSIZ];
  ಅಕ್ಷರ *buf = smallbuf;
  size_t ಅಗಲ;
  bool pad;

  quote_name_buf (&buf, ಗಾತ್ರ smallbuf, (ಅಕ್ಷರ *) name, options,
                  needs_general_quoting, &ಅಗಲ, &pad);

  ಆದರೆ (buf != smallbuf && buf != name)
    ಬಿಡುಗಡೆ (buf);

  ಅಗಲ += pad;

  ಹಿಂತಿರುಗಿ ಅಗಲ;
}

/* %XX escape any input out of range as defined in RFC3986,
   and also if PATH, convert all path separators to '/'.  */
ಸ್ಥಿರ ಅಕ್ಷರ *
file_escape (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *str, bool ಮಾರ್ಗ)
{
  ಅಕ್ಷರ *esc = xnmalloc (3, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (str) + 1);
  ಅಕ್ಷರ *p = esc;
  ಆಗ (*str)
    {
      ಆದರೆ (ಮಾರ್ಗ && ISSLASH (*str))
        {
          *p++ = '/';
          str++;
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (RFC3986[ಗಮ್ಯ_uchar (*str)])
        *p++ = *str++;
      ಇಲ್ಲದಿದ್ದರೆ
        p += sprintf (p, "%%%02x", ಗಮ್ಯ_uchar (*str++));
    }
  *p = '\0';
  ಹಿಂತಿರುಗಿ esc;
}

ಸ್ಥಿರ size_t
quote_name (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name, ರಚನೆ quoting_options ಸ್ಥಿರಾಂಕ *options,
            ಪೂರ್ಣಾಂಕ needs_general_quoting, ಸ್ಥಿರಾಂಕ ರಚನೆ bin_str *color,
            bool allow_pad, ರಚನೆ obstack *ರಾಶಿ, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *absolute_name)
{
  ಅಕ್ಷರ smallbuf[BUFSIZ];
  ಅಕ್ಷರ *buf = smallbuf;
  size_t len;
  bool pad;

  len = quote_name_buf (&buf, ಗಾತ್ರ smallbuf, (ಅಕ್ಷರ *) name, options,
                        needs_general_quoting, ಖಾಲಿ, &pad);

  ಆದರೆ (pad && allow_pad)
    dired_outbyte (' ');

  ಆದರೆ (color)
    print_color_indicator (color);

  /* If we're padding, then don't include the outer quotes in
     the --hyperlink, to improve the alignment of those links.  */
  bool skip_quotes = false;

  ಆದರೆ (absolute_name)
    {
      ಆದರೆ (align_variable_outer_quotes && cwd_some_quoted && ! pad)
        {
          skip_quotes = true;
          ಅಕ್ಷರ_ಬರೆ (*buf);
        }
      ಅಕ್ಷರ *h = file_escape (hostname, /* path= */ false);
      ಅಕ್ಷರ *n = file_escape (absolute_name, /* path= */ true);
      /* TODO: It would be good to be able to define parameters
         to give hints to the terminal as how best to render the URI.
         For example since ls is outputting a dense block of URIs
         it would be best to not underline by default, and only
         do so upon hover etc.  */
      ಬರೆ ("\033]8;;file://%s%s%s\033\\", h, *n == '/' ? "" : "/", n);
      ಬಿಡುಗಡೆ (h);
      ಬಿಡುಗಡೆ (n);
    }

  ಆದರೆ (ರಾಶಿ)
    ತಳ್ಳು_current_dired_pos (ರಾಶಿ);

  fwrite (buf + skip_quotes, 1, len - (skip_quotes * 2), stdout);

  dired_pos += len;

  ಆದರೆ (ರಾಶಿ)
    ತಳ್ಳು_current_dired_pos (ರಾಶಿ);

  ಆದರೆ (absolute_name)
    {
      fputs ("\033]8;;\033\\", stdout);
      ಆದರೆ (skip_quotes)
        ಅಕ್ಷರ_ಬರೆ (*(buf + len - 1));
    }

  ಆದರೆ (buf != smallbuf && buf != name)
    ಬಿಡುಗಡೆ (buf);

  ಹಿಂತಿರುಗಿ len + pad;
}

ಸ್ಥಿರ size_t
print_name_with_quoting (ಸ್ಥಿರಾಂಕ ರಚನೆ fileinfo *f,
                         bool symlink_ಗುರಿ,
                         ರಚನೆ obstack *ರಾಶಿ,
                         size_t ಆರಂಭ_ಲಂಬಸಾಲು)
{
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name = symlink_ಗುರಿ ? f->linkname : f->name;

  ಸ್ಥಿರಾಂಕ ರಚನೆ bin_str *color
    = print_with_color ? get_color_indicator (f, symlink_ಗುರಿ) : ಖಾಲಿ;

  bool ಬಳಸಿದ_color_this_ಸಮಯ = (print_with_color
                               && (color || is_colored (C_NORM)));

  size_t len = quote_name (name, filename_quoting_options, f->quoted,
                           color, !symlink_ಗುರಿ, ರಾಶಿ, f->absolute_name);

  process_ಸಂಕೇತಗಳು ();
  ಆದರೆ (ಬಳಸಿದ_color_this_ಸಮಯ)
    {
      prep_non_filename_ಪಠ್ಯ ();

      /* We use the byte length rather than display width here as
         an optimization to avoid accurately calculating the width,
         because we only output the clear to EOL sequence if the name
         _might_ wrap to the next line.  This may output a sequence
         unnecessarily in multi-byte locales for example,
         but in that case it's inconsequential to the output.  */
      ಆದರೆ (ಸಾಲು_ಉದ್ದಳತೆ
          && (ಆರಂಭ_ಲಂಬಸಾಲು / ಸಾಲು_ಉದ್ದಳತೆ != (ಆರಂಭ_ಲಂಬಸಾಲು + len - 1) / ಸಾಲು_ಉದ್ದಳತೆ))
        put_indicator (&color_indicator[C_CLR_ಗಮ್ಯ_EOL]);
    }

  ಹಿಂತಿರುಗಿ len;
}

ಸ್ಥಿರ ಶೂನ್ಯ
prep_non_filename_ಪಠ್ಯ (ಶೂನ್ಯ)
{
  ಆದರೆ (color_indicator[C_ಕೊನೆ].string != ಖಾಲಿ)
    put_indicator (&color_indicator[C_ಕೊನೆ]);
  ಇಲ್ಲದಿದ್ದರೆ
    {
      put_indicator (&color_indicator[C_ಎಡ]);
      put_indicator (&color_indicator[C_RESET]);
      put_indicator (&color_indicator[C_ಬಲ]);
    }
}

/* Print the file name of 'f' with appropriate quoting.
   Also print file size, inode number, and filetype indicator character,
   as requested by switches.  */

ಸ್ಥಿರ size_t
print_file_name_and_frills (ಸ್ಥಿರಾಂಕ ರಚನೆ fileinfo *f, size_t ಆರಂಭ_ಲಂಬಸಾಲು)
{
  ಅಕ್ಷರ buf[MAX (LONGEST_HUMAN_READABLE + 1, INT_BUFSIZE_BOUND (uintmax_t))];

  ಗಣ_ಸಾಮಾನ್ಯ_color ();

  ಆದರೆ (print_inode)
    ಬರೆ ("%*s ", format == with_commas ? 0 : inode_ಸಂಖ್ಯೆ_ಅಗಲ,
            format_inode (buf, f));

  ಆದರೆ (print_ಖಂಡ_ಗಾತ್ರ)
    {
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಖಂಡಗಳು =
        (! f->stat_ok
         ? "?"
         : human_readable (STP_NBLOCKS (&f->stat), buf, human_output_opts,
                           ST_NBLOCKSIZE, output_ಖಂಡ_ಗಾತ್ರ));
      ಪೂರ್ಣಾಂಕ ಖಂಡಗಳು_ಅಗಲ = mbswidth (ಖಂಡಗಳು, MBSWIDTH_FLAGS);
      ಪೂರ್ಣಾಂಕ pad = 0;
      ಆದರೆ (0 <= ಖಂಡಗಳು_ಅಗಲ && ಖಂಡ_ಗಾತ್ರ_ಅಗಲ && format != with_commas)
        pad = ಖಂಡ_ಗಾತ್ರ_ಅಗಲ - ಖಂಡಗಳು_ಅಗಲ;
      ಬರೆ ("%*s%s ", pad, "", ಖಂಡಗಳು);
    }

  ಆದರೆ (print_scontext)
    ಬರೆ ("%*s ", format == with_commas ? 0 : scontext_ಅಗಲ, f->scontext);

  size_t ಅಗಲ = print_name_with_quoting (f, false, ಖಾಲಿ, ಆರಂಭ_ಲಂಬಸಾಲು);

  ಆದರೆ (indicator_style != none)
    ಅಗಲ += print_type_indicator (f->stat_ok, f->stat.st_ಬಹುಲಕ, f->filetype);

  ಹಿಂತಿರುಗಿ ಅಗಲ;
}

/* Given these arguments describing a file, return the single-byte
   type indicator, or 0.  */
ಸ್ಥಿರ ಅಕ್ಷರ
get_type_indicator (bool stat_ok, mode_t ಬಹುಲಕ_2, ಎಣಿಕೆ filetype type)
{
  ಅಕ್ಷರ c;

  ಆದರೆ (stat_ok ? S_ISREG (ಬಹುಲಕ_2) : type == ಸಾಮಾನ್ಯ)
    {
      ಆದರೆ (stat_ok && indicator_style == classify && (ಬಹುಲಕ_2 & S_IXUGO))
        c = '*';
      ಇಲ್ಲದಿದ್ದರೆ
        c = 0;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಆದರೆ (stat_ok ? S_ISDIR (ಬಹುಲಕ_2) : type == directory || type == arg_directory)
        c = '/';
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (indicator_style == slash)
        c = 0;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (stat_ok ? S_ISLNK (ಬಹುಲಕ_2) : type == symbolic_link)
        c = '@';
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (stat_ok ? S_ISFIFO (ಬಹುಲಕ_2) : type == fifo)
        c = '|';
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (stat_ok ? S_ISSOCK (ಬಹುಲಕ_2) : type == sock)
        c = '=';
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (stat_ok && S_ISDOOR (ಬಹುಲಕ_2))
        c = '>';
      ಇಲ್ಲದಿದ್ದರೆ
        c = 0;
    }
  ಹಿಂತಿರುಗಿ c;
}

ಸ್ಥಿರ bool
print_type_indicator (bool stat_ok, mode_t ಬಹುಲಕ_2, ಎಣಿಕೆ filetype type)
{
  ಅಕ್ಷರ c = get_type_indicator (stat_ok, ಬಹುಲಕ_2, type);
  ಆದರೆ (c)
    dired_outbyte (c);
  ಹಿಂತಿರುಗಿ !!c;
}

/* Returns if color sequence was printed.  */
ಸ್ಥಿರ bool
print_color_indicator (ಸ್ಥಿರಾಂಕ ರಚನೆ bin_str *ind)
{
  ಆದರೆ (ind)
    {
      /* Need to reset so not dealing with attribute combinations */
      ಆದರೆ (is_colored (C_NORM))
        restore_default_color ();
      put_indicator (&color_indicator[C_ಎಡ]);
      put_indicator (ind);
      put_indicator (&color_indicator[C_ಬಲ]);
    }

  ಹಿಂತಿರುಗಿ ind != ಖಾಲಿ;
}

/* Returns color indicator or NULL if none.  */
ATTRIBUTE_ಶುದ್ಧ
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ bin_str *
get_color_indicator (ಸ್ಥಿರಾಂಕ ರಚನೆ fileinfo *f, bool symlink_ಗುರಿ)
{
  ಎಣಿಕೆ indicator_no type;
  ರಚನೆ color_ext_type *ext;	/* Color extension */
  size_t len;			/* Length of name */

  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name;
  mode_t ಬಹುಲಕ_2;
  ಪೂರ್ಣಾಂಕ linkok;
  ಆದರೆ (symlink_ಗುರಿ)
    {
      name = f->linkname;
      ಬಹುಲಕ_2 = f->linkmode;
      linkok = f->linkok ? 0 : -1;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      name = f->name;
      ಬಹುಲಕ_2 = file_or_link_ಬಹುಲಕ (f);
      linkok = f->linkok;
    }

  /* Is this a nonexistent file?  If so, linkok == -1.  */

  ಆದರೆ (linkok == -1 && is_colored (C_MISSING))
    type = C_MISSING;
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!f->stat_ok)
    {
      ಸ್ಥಿರ ಎಣಿಕೆ indicator_no ಸ್ಥಿರಾಂಕ filetype_indicator[] =
        {
          C_ORPHAN, C_FIFO, C_CHR, C_DIR, C_BLK, C_FILE,
          C_LINK, C_SOCK, C_FILE, C_DIR
        };
      static_assert (countof (filetype_indicator) == filetype_cardinality);
      type = filetype_indicator[f->filetype];
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಆದರೆ (S_ISREG (ಬಹುಲಕ_2))
        {
          type = C_FILE;

          ಆದರೆ ((ಬಹುಲಕ_2 & S_ISUID) != 0 && is_colored (C_SETUID))
            type = C_SETUID;
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ((ಬಹುಲಕ_2 & S_ISGID) != 0 && is_colored (C_SETGID))
            type = C_SETGID;
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (f->has_capability)
            type = C_CAP;
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ((ಬಹುಲಕ_2 & S_IXUGO) != 0 && is_colored (C_EXEC))
            type = C_EXEC;
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ((1 < f->stat.st_nlink) && is_colored (C_MULTIHARDLINK))
            type = C_MULTIHARDLINK;
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (S_ISDIR (ಬಹುಲಕ_2))
        {
          type = C_DIR;

          ಆದರೆ ((ಬಹುಲಕ_2 & S_ISVTX) && (ಬಹುಲಕ_2 & S_IWOTH)
              && is_colored (C_STICKY_OTHER_WRITABLE))
            type = C_STICKY_OTHER_WRITABLE;
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ((ಬಹುಲಕ_2 & S_IWOTH) != 0 && is_colored (C_OTHER_WRITABLE))
            type = C_OTHER_WRITABLE;
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ((ಬಹುಲಕ_2 & S_ISVTX) != 0 && is_colored (C_STICKY))
            type = C_STICKY;
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (S_ISLNK (ಬಹುಲಕ_2))
        type = C_LINK;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (S_ISFIFO (ಬಹುಲಕ_2))
        type = C_FIFO;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (S_ISSOCK (ಬಹುಲಕ_2))
        type = C_SOCK;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (S_ISBLK (ಬಹುಲಕ_2))
        type = C_BLK;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (S_ISCHR (ಬಹುಲಕ_2))
        type = C_CHR;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (S_ISDOOR (ಬಹುಲಕ_2))
        type = C_DOOR;
      ಇಲ್ಲದಿದ್ದರೆ
        {
          /* Classify a file of some other type as C_ORPHAN.  */
          type = C_ORPHAN;
        }
    }

  /* Check the file's suffix only if still classified as C_FILE.  */
  ext = ಖಾಲಿ;
  ಆದರೆ (type == C_FILE)
    {
      /* Test if NAME has a recognized suffix.  */

      len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (name);
      name += len;		/* Pointer to final \0.  */
      ಗಾಗಿ (ext = color_ext_list; ext != ಖಾಲಿ; ext = ext->ಮುಂದಿನ)
        {
          ಆದರೆ (ext->ext.len <= len)
            {
              ಆದರೆ (ext->exact_ಹೊಂದಿಕೆ)
                {
                  ಆದರೆ (STREQ_LEN (name - ext->ext.len, ext->ext.string,
                                 ext->ext.len))
                    ನಿಲ್ಲಿಸು;
                }
              ಇಲ್ಲದಿದ್ದರೆ
                {
                  ಆದರೆ (c_strncasecmp (name - ext->ext.len, ext->ext.string,
                                     ext->ext.len) == 0)
                    ನಿಲ್ಲಿಸು;
                }
            }
        }
    }

  /* Adjust the color for orphaned symlinks.  */
  ಆದರೆ (type == C_LINK && !linkok)
    {
      ಆದರೆ (color_symlink_as_referent || is_colored (C_ORPHAN))
        type = C_ORPHAN;
    }

  ಸ್ಥಿರಾಂಕ ರಚನೆ bin_str *ಸ್ಥಿರಾಂಕ s
    = ext ? &(ext->seq) : &color_indicator[type];

  ಹಿಂತಿರುಗಿ s->string ? s : ಖಾಲಿ;
}

/* Output a color indicator (which may contain nulls).
   Note we don't update dired_pos here as emacs can be configured
   to handle color codes by stripping/converting them.  */
ಸ್ಥಿರ ಶೂನ್ಯ
put_indicator (ಸ್ಥಿರಾಂಕ ರಚನೆ bin_str *ind)
{
  ಆದರೆ (! ಬಳಸಿದ_color)
    {
      ಬಳಸಿದ_color = true;

      /* If the standard output is a controlling terminal, watch out
         for signals, so that the colors can be restored to the
         default state if "ls" is suspended or interrupted.  */

      ಆದರೆ (0 <= tcgetpgrp (STDOUT_FILENO))
        signal_init ();

      prep_non_filename_ಪಠ್ಯ ();
    }

  fwrite (ind->string, ind->len, 1, stdout);
}

ಸ್ಥಿರ size_t
ಉದ್ದಳತೆ_ದ_file_name_and_frills (ಸ್ಥಿರಾಂಕ ರಚನೆ fileinfo *f)
{
  size_t len = 0;
  ಅಕ್ಷರ buf[MAX (LONGEST_HUMAN_READABLE + 1, INT_BUFSIZE_BOUND (uintmax_t))];

  ಆದರೆ (print_inode)
    len += 1 + (format == with_commas
                ? ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (umaxtostr (f->stat.st_ino, buf))
                : inode_ಸಂಖ್ಯೆ_ಅಗಲ);

  ಆದರೆ (print_ಖಂಡ_ಗಾತ್ರ)
    len += 1 + (format == with_commas
                ? ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (! f->stat_ok ? "?"
                          : human_readable (STP_NBLOCKS (&f->stat), buf,
                                            human_output_opts, ST_NBLOCKSIZE,
                                            output_ಖಂಡ_ಗಾತ್ರ))
                : ಖಂಡ_ಗಾತ್ರ_ಅಗಲ);

  ಆದರೆ (print_scontext)
    len += 1 + (format == with_commas ? ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (f->scontext) : scontext_ಅಗಲ);

  len += fileinfo_name_ಅಗಲ (f);

  ಆದರೆ (indicator_style != none)
    {
      ಅಕ್ಷರ c = get_type_indicator (f->stat_ok, f->stat.st_ಬಹುಲಕ, f->filetype);
      len += (c != 0);
    }

  ಹಿಂತಿರುಗಿ len;
}

ಸ್ಥಿರ ಶೂನ್ಯ
print_many_ಪ್ರತಿ_ಸಾಲು (ಶೂನ್ಯ)
{
  idx_t ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2 = calculate_columns (true);
  ರಚನೆ ಸ್ತಂಭ_info ಸ್ಥಿರಾಂಕ *ಸಾಲು_fmt = &ಸ್ತಂಭ_info[ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2 - 1];

  /* Calculate the number of rows that will be in each column except possibly
     for a short column on the right.  */
  idx_t ಅಡ್ಡಸಾಲುಗಳು = cwd_n_ಬಳಸಿದ / ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2 + (cwd_n_ಬಳಸಿದ % ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2 != 0);

  ಗಾಗಿ (idx_t ಅಡ್ಡಸಾಲು = 0; ಅಡ್ಡಸಾಲು < ಅಡ್ಡಸಾಲುಗಳು; ಅಡ್ಡಸಾಲು++)
    {
      size_t ಲಂಬಸಾಲು = 0;
      idx_t filesno = ಅಡ್ಡಸಾಲು;
      size_t pos = 0;

      /* Print the next row.  */
      ಆಗ (true)
        {
          ರಚನೆ fileinfo ಸ್ಥಿರಾಂಕ *f = sorted_file[filesno];
          size_t name_ಉದ್ದಳತೆ = ಉದ್ದಳತೆ_ದ_file_name_and_frills (f);
          size_t max_name_ಉದ್ದಳತೆ = ಸಾಲು_fmt->ಲಂಬಸಾಲು_arr[ಲಂಬಸಾಲು++];
          print_file_name_and_frills (f, pos);

          ಆದರೆ (cwd_n_ಬಳಸಿದ - ಅಡ್ಡಸಾಲುಗಳು <= filesno)
            ನಿಲ್ಲಿಸು;
          filesno += ಅಡ್ಡಸಾಲುಗಳು;

          indent (pos + name_ಉದ್ದಳತೆ, pos + max_name_ಉದ್ದಳತೆ);
          pos += max_name_ಉದ್ದಳತೆ;
        }
      ಅಕ್ಷರ_ಬರೆ (eolbyte);
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
print_horizontal (ಶೂನ್ಯ)
{
  size_t pos = 0;
  idx_t ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2 = calculate_columns (false);
  ರಚನೆ ಸ್ತಂಭ_info ಸ್ಥಿರಾಂಕ *ಸಾಲು_fmt = &ಸ್ತಂಭ_info[ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2 - 1];
  ರಚನೆ fileinfo ಸ್ಥಿರಾಂಕ *f = sorted_file[0];
  size_t name_ಉದ್ದಳತೆ = ಉದ್ದಳತೆ_ದ_file_name_and_frills (f);
  size_t max_name_ಉದ್ದಳತೆ = ಸಾಲು_fmt->ಲಂಬಸಾಲು_arr[0];

  /* Print first entry.  */
  print_file_name_and_frills (f, 0);

  /* Now the rest.  */
  ಗಾಗಿ (idx_t filesno = 1; filesno < cwd_n_ಬಳಸಿದ; filesno++)
    {
      idx_t ಲಂಬಸಾಲು = filesno % ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2;

      ಆದರೆ (ಲಂಬಸಾಲು == 0)
        {
          ಅಕ್ಷರ_ಬರೆ (eolbyte);
          pos = 0;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          indent (pos + name_ಉದ್ದಳತೆ, pos + max_name_ಉದ್ದಳತೆ);
          pos += max_name_ಉದ್ದಳತೆ;
        }

      f = sorted_file[filesno];
      print_file_name_and_frills (f, pos);

      name_ಉದ್ದಳತೆ = ಉದ್ದಳತೆ_ದ_file_name_and_frills (f);
      max_name_ಉದ್ದಳತೆ = ಸಾಲು_fmt->ಲಂಬಸಾಲು_arr[ಲಂಬಸಾಲು];
    }
  ಅಕ್ಷರ_ಬರೆ (eolbyte);
}

/* Output name + SEP + ' '.  */

ಸ್ಥಿರ ಶೂನ್ಯ
print_with_separator (ಅಕ್ಷರ sep)
{
  size_t pos = 0;

  ಗಾಗಿ (idx_t filesno = 0; filesno < cwd_n_ಬಳಸಿದ; filesno++)
    {
      ರಚನೆ fileinfo ಸ್ಥಿರಾಂಕ *f = sorted_file[filesno];
      size_t len = ಸಾಲು_ಉದ್ದಳತೆ ? ಉದ್ದಳತೆ_ದ_file_name_and_frills (f) : 0;

      ಆದರೆ (filesno != 0)
        {
          ಅಕ್ಷರ separator;
          size_t ಮುಂದಿನ_pos = 2 + (sep == ',' && filesno < cwd_n_ಬಳಸಿದ - 1);

          ಆದರೆ (! ಸಾಲು_ಉದ್ದಳತೆ
              || (! ckd_ಕೂಡಿಸು (&ಮುಂದಿನ_pos, ಮುಂದಿನ_pos, pos)
                  && ! ckd_ಕೂಡಿಸು (&ಮುಂದಿನ_pos, ಮುಂದಿನ_pos, len)
                  && ಮುಂದಿನ_pos <= ಸಾಲು_ಉದ್ದಳತೆ))
            {
              pos += 2;
              separator = ' ';
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              pos = 0;
              separator = eolbyte;
            }

          ಅಕ್ಷರ_ಬರೆ (sep);
          ಅಕ್ಷರ_ಬರೆ (separator);
        }

      print_file_name_and_frills (f, pos);
      pos += len;
    }
  ಅಕ್ಷರ_ಬರೆ (eolbyte);
}

/* Assuming cursor is at position FROM, indent up to position TO.
   Use a TAB character instead of two or more spaces whenever possible.  */

ಸ್ಥಿರ ಶೂನ್ಯ
indent (size_t from, size_t ಗಮ್ಯ)
{
  ಆಗ (from < ಗಮ್ಯ)
    {
      ಆದರೆ (tabsize != 0 && ಗಮ್ಯ / tabsize > (from + 1) / tabsize)
        {
          ಅಕ್ಷರ_ಬರೆ ('\t');
          from += tabsize - from % tabsize;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಅಕ್ಷರ_ಬರೆ (' ');
          from++;
        }
    }
}

/* Put DIRNAME/NAME into DEST, handling '.' and '/' properly.  */
/* FIXME: maybe remove this function someday.  See about using a
   non-malloc'ing version of file_name_concat.  */

ಸ್ಥಿರ ಶೂನ್ಯ
attach (ಅಕ್ಷರ *dest, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dirname, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name)
{
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dirnamep = dirname;

  /* Copy dirname if it is not ".".  */
  ಆದರೆ (dirname[0] != '.' || dirname[1] != 0)
    {
      ಆಗ (*dirnamep)
        *dest++ = *dirnamep++;
      /* Add '/' if 'dirname' doesn't already end with it.  */
      ಆದರೆ (dirnamep > dirname && dirnamep[-1] != '/')
        *dest++ = '/';
    }
  ಆಗ (*name)
    *dest++ = *name++;
  *dest = 0;
}

/* Allocate enough column info suitable for the current number of
   files and display columns, and initialize the info to represent the
   narrowest possible columns.  */

ಸ್ಥಿರ ಶೂನ್ಯ
init_ಸ್ತಂಭ_info (idx_t max_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ)
{
  /* Currently allocated columns in column_info.  */
  ಸ್ಥಿರ idx_t ಸ್ತಂಭ_info_alloc;

  ಆದರೆ (ಸ್ತಂಭ_info_alloc < max_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ)
    {
      idx_t old_ಸ್ತಂಭ_info_alloc = ಸ್ತಂಭ_info_alloc;
      ಸ್ತಂಭ_info = xpalloc (ಸ್ತಂಭ_info, &ಸ್ತಂಭ_info_alloc,
                             max_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ - ಸ್ತಂಭ_info_alloc, -1,
                             ಗಾತ್ರ *ಸ್ತಂಭ_info);

      /* Allocate the new size_t objects by computing the triangle
         formula n * (n + 1) / 2, except that we don't need to
         allocate the part of the triangle that we've already
         allocated.  Check for address arithmetic overflow.  */
      idx_t ಸ್ತಂಭ_info_growth = ಸ್ತಂಭ_info_alloc - old_ಸ್ತಂಭ_info_alloc, s;
      ಆದರೆ (ckd_ಕೂಡಿಸು (&s, old_ಸ್ತಂಭ_info_alloc + 1, ಸ್ತಂಭ_info_alloc)
          || ckd_mul (&s, s, ಸ್ತಂಭ_info_growth))
        xalloc_die ();
      size_t *p = xinmalloc (s >> 1, ಗಾತ್ರ *p);

      /* Grow the triangle by parceling out the cells just allocated.  */
      ಗಾಗಿ (idx_t i = old_ಸ್ತಂಭ_info_alloc; i < ಸ್ತಂಭ_info_alloc; i++)
        {
          ಸ್ತಂಭ_info[i].ಲಂಬಸಾಲು_arr = p;
          p += i + 1;
        }
    }

  ಗಾಗಿ (idx_t i = 0; i < max_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ; ++i)
    {
      ಸ್ತಂಭ_info[i].valid_len = true;
      ಸ್ತಂಭ_info[i].ಸಾಲು_len = i * MIN_ಸ್ತಂಭ_ಅಗಲ + 1;
      ಗಾಗಿ (idx_t j = 0; j < i; ++j)
        ಸ್ತಂಭ_info[i].ಲಂಬಸಾಲು_arr[j] = MIN_ಸ್ತಂಭ_ಅಗಲ;
      ಸ್ತಂಭ_info[i].ಲಂಬಸಾಲು_arr[i] = 1;
    }
}

/* Calculate the number of columns needed to represent the current set
   of files in the current display width.  */

ಸ್ಥಿರ idx_t
calculate_columns (bool by_columns)
{
  /* Normally the maximum number of columns is determined by the
     screen width.  But if few files are available this might limit it
     as well.  */
  idx_t max_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = 0 < max_idx && max_idx < cwd_n_ಬಳಸಿದ ? max_idx : cwd_n_ಬಳಸಿದ;

  init_ಸ್ತಂಭ_info (max_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ);

  /* Compute the maximum number of possible columns.  */
  ಗಾಗಿ (idx_t filesno = 0; filesno < cwd_n_ಬಳಸಿದ; ++filesno)
    {
      ರಚನೆ fileinfo ಸ್ಥಿರಾಂಕ *f = sorted_file[filesno];
      size_t name_ಉದ್ದಳತೆ = ಉದ್ದಳತೆ_ದ_file_name_and_frills (f);

      ಗಾಗಿ (idx_t i = 0; i < max_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ; ++i)
        {
          ಆದರೆ (ಸ್ತಂಭ_info[i].valid_len)
            {
              idx_t idx = (by_columns
                           ? filesno / ((cwd_n_ಬಳಸಿದ + i) / (i + 1))
                           : filesno % (i + 1));
              size_t real_ಉದ್ದಳತೆ = name_ಉದ್ದಳತೆ + (idx == i ? 0 : 2);

              ಆದರೆ (ಸ್ತಂಭ_info[i].ಲಂಬಸಾಲು_arr[idx] < real_ಉದ್ದಳತೆ)
                {
                  ಸ್ತಂಭ_info[i].ಸಾಲು_len += (real_ಉದ್ದಳತೆ
                                              - ಸ್ತಂಭ_info[i].ಲಂಬಸಾಲು_arr[idx]);
                  ಸ್ತಂಭ_info[i].ಲಂಬಸಾಲು_arr[idx] = real_ಉದ್ದಳತೆ;
                  ಸ್ತಂಭ_info[i].valid_len = (ಸ್ತಂಭ_info[i].ಸಾಲು_len
                                              <= ಸಾಲು_ಉದ್ದಳತೆ);
                }
            }
        }
    }

  /* Find maximum allowed columns.  */
  idx_t ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2;
  ಗಾಗಿ (ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2 = max_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ; 1 < ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2; --ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2)
    {
      ಆದರೆ (ಸ್ತಂಭ_info[ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2 - 1].valid_len)
        ನಿಲ್ಲಿಸು;
    }

  ಹಿಂತಿರುಗಿ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2;
}

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("Usage: %s [OPTION]... [FILE]...\n"), ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
List information about the FILEs (the current directory by ಪೂರ್ವನಿಯೋಜಿತ).\n\
ವಿಂಗಡಿಸು_2 entries alphabetically ಆದರೆ none ದ -cftuvSUX nor --ವಿಂಗಡಿಸು is specified.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -a, --all\n\
         ಮಾಡು not ignore entries starting with .\n\
"));
      oputs (_("\
  -A, --almost-all\n\
         ಮಾಡು not list implied . and ..\n\
"));
      oputs (_("\
      --author\n\
         with -l, print the author ದ each file\n\
"));
      oputs (_("\
  -b, --escape\n\
         print C-style escapes ಗಾಗಿ nongraphic characters\n\
"));
      oputs (_("\
      --ಖಂಡ-ಗಾತ್ರ_2=ಗಾತ್ರ_3\n\
         with -l, scale sizes by ಗಾತ್ರ_3 when printing them;\n\
         e.g., '--block-size=M'; see ಗಾತ್ರ_3 format ಕೆಳಗೆ\n\
"));
      oputs (_("\
  -B, --ignore-backups\n\
         ಮಾಡು not list implied entries ending with ~\n\
"));
      oputs (_("\
  -c\n\
         with -lt: ವಿಂಗಡಿಸು by, and show, ctime\n\
           (ಸಮಯ ದ ಕೊನೆಯ change ದ file ಸ್ಥಿತಿ_2 information);\n\
         with -l: show ctime and ವಿಂಗಡಿಸು by name;\n\
         otherwise: ವಿಂಗಡಿಸು by ctime, newest ಮೊದಲ\n\
"));
      oputs (_("\
  -C\n\
         list entries by columns\n\
"));
      oputs (_("\
      --color[=WHEN]\n\
         color the output WHEN; ಇನ್ನಷ್ಟು info ಕೆಳಗೆ\n\
"));
      oputs (_("\
  -d, --directory\n\
         list directories themselves, not their contents\n\
"));
      oputs (_("\
  -D, --dired\n\
         generate output designed ಗಾಗಿ Emacs' dired ಬಹುಲಕ_2\n\
"));
      oputs (_("\
  -f\n\
         same as -a -U\n\
"));
      oputs (_("\
  -F, --classify[=WHEN]\n\
         append indicator (one ದ */=>@|) ಗಮ್ಯ entries WHEN\n\
"));
      oputs (_("\
      --file-type\n\
         like -F, except ಮಾಡು not append '*'\n\
"));
      oputs (_("\
      --format=ಶಬ್ದ\n\
         across,horizontal (-x), commas (-m), ದೀರ್ಘ (-l),\n\
         single-ಸ್ತಂಭ (-1), verbose (-l), vertical (-C)\n\
"));
      oputs (_("\
      --ತುಂಬಿದ-ಸಮಯ\n\
         like -l --ಸಮಯ-style=ತುಂಬಿದ-iso\n\
"));
      oputs (_("\
  -g\n\
         like -l, but ಮಾಡು not list owner\n\
"));
      oputs (_("\
      --ಗುಂಪು-directories-ಮೊದಲ\n\
         ಗುಂಪು directories before files\n\
"));
      oputs (_("\
  -G, --no-ಗುಂಪು\n\
         ಒಳ a ದೀರ್ಘ listing, don't print ಗುಂಪು ಹೆಸರುಗಳು\n\
"));
      oputs (_("\
  -h, --human-readable\n\
         with -l and -s, print sizes like 1K 234M 2G etc.\n\
"));
      oputs (_("\
      --si\n\
         likewise, but use powers ದ 1000 not 1024\n\
"));
      oputs (_("\
  -H, --dereference-command-ಸಾಲು\n\
         follow symbolic links listed on the command ಸಾಲು\n\
"));
      oputs (_("\
      --dereference-command-ಸಾಲು-symlink-ಗಮ್ಯ-dir\n\
         follow each command ಸಾಲು symbolic link that points ಗಮ್ಯ a directory\n\
"));
      oputs (_("\
      --hide=ನಮೂನೆ\n\
         ಮಾಡು not list implied entries matching shell ನಮೂನೆ\n\
         (overridden by -a or -A)\n\
"));
      oputs (_("\
      --hyperlink[=WHEN]\n\
         hyperlink file ಹೆಸರುಗಳು WHEN\n\
"));
      oputs (_("\
      --indicator-style=ಶಬ್ದ\n\
         append indicator with style ಶಬ್ದ ಗಮ್ಯ ನಮೂದು ಹೆಸರುಗಳು:\n\
           none (ಪೂರ್ವನಿಯೋಜಿತ), slash (-p), file-type (--file-type), classify (-F)\n\
"));
      oputs (_("\
  -i, --inode\n\
         print the index ಸಂಖ್ಯೆ ದ each file\n\
"));
      oputs (_("\
  -I, --ignore=ನಮೂನೆ\n\
         ಮಾಡು not list implied entries matching shell ನಮೂನೆ\n\
"));
      oputs (_("\
  -k, --kibibytes\n\
         ಪೂರ್ವನಿಯೋಜಿತ ಗಮ್ಯ 1024-byte ಖಂಡಗಳು ಗಾಗಿ file system usage;\n\
         ಬಳಸಿದ only with -s and ಪ್ರತಿ directory totals\n\
"));
      oputs (_("\
  -l\n\
         use a ದೀರ್ಘ listing format\n\
"));
      oputs (_("\
  -L, --dereference\n\
         when showing file information ಗಾಗಿ a symbolic link,\n\
         show information ಗಾಗಿ the file the link references\n\
         rather than ಗಾಗಿ the link itself\n\
"));
      oputs (_("\
  -m\n\
         fill ಅಗಲ with a comma separated list ದ entries\n\
"));
      oputs (_("\
  -n, --numeric-uid-gid\n\
         like -l, but list numeric user and ಗುಂಪು IDs\n\
"));
      oputs (_("\
  -N, --literal\n\
         print ನಮೂದು ಹೆಸರುಗಳು without quoting\n\
"));
      oputs (_("\
  -o\n\
         like -l, but ಮಾಡು not list ಗುಂಪು information\n\
"));
      oputs (_("\
  -p, --indicator-style=slash\n\
         append / indicator ಗಮ್ಯ directories\n\
"));
      oputs (_("\
  -q, --hide-control-chars\n\
         print ? instead ದ nongraphic characters\n\
"));
      oputs (_("\
      --show-control-chars\n\
         show nongraphic characters as-is;\n\
         the ಪೂರ್ವನಿಯೋಜಿತ, unless ಕಾರ್ಯಕ್ರಮ is 'ls' and output is a terminal\n\
"));
      oputs (_("\
  -Q, --quote-name\n\
         enclose ನಮೂದು ಹೆಸರುಗಳು ಒಳ ದ್ವಿಗುಣ quotes\n\
"));
      oputs (_("\
      --quoting-style=ಶಬ್ದ\n\
         use quoting style ಶಬ್ದ ಗಾಗಿ ನಮೂದು ಹೆಸರುಗಳು:\n\
           literal, locale, shell, shell-always,\n\
           shell-escape, shell-escape-always, c, escape\n\
         (overrides QUOTING_STYLE environment variable)\n\
"));
      oputs (_("\
  -r, --ಹಿಮ್ಮುಖ\n\
         ಹಿಮ್ಮುಖ ಕ್ರಮ ಆಗ ವಿಂಗಡಣೆ\n\
"));
      oputs (_("\
  -R, --recursive\n\
         list subdirectories recursively\n\
"));
      oputs (_("\
  -s, --ಗಾತ್ರ_2\n\
         print the allocated ಗಾತ್ರ_2 ದ each file, ಒಳ ಖಂಡಗಳು\n\
"));
      oputs (_("\
  -S\n\
         ವಿಂಗಡಿಸು by file ಗಾತ್ರ_2, ಅತಿದೊಡ್ಡ ಮೊದಲ\n\
"));
      oputs (_("\
      --ವಿಂಗಡಿಸು=ಶಬ್ದ\n\
         change ಪೂರ್ವನಿಯೋಜಿತ 'name' ವಿಂಗಡಿಸು ಗಮ್ಯ ಶಬ್ದ:\n\
           none (-U), ಗಾತ್ರ_2 (-S), ಸಮಯ (-t),\n\
           ಆವೃತ್ತಿ_3 (-v), extension (-X), name, ಅಗಲ\n\
"));
      oputs (_("\
      --ಸಮಯ=ಶಬ್ದ\n\
         select which timestamp ಬಳಸಿದ ಗಮ್ಯ display or ವಿಂಗಡಿಸು;\n\
           access ಸಮಯ (-u): atime, access, use;\n\
           metadata change ಸಮಯ (-c): ctime, ಸ್ಥಿತಿ_2;\n\
           modified ಸಮಯ (ಪೂರ್ವನಿಯೋಜಿತ): mtime, modification;\n\
           birth ಸಮಯ: birth, creation;\n\
         with -l, ಶಬ್ದ determines which ಸಮಯ ಗಮ್ಯ show;\n\
         with --ವಿಂಗಡಿಸು=ಸಮಯ, ವಿಂಗಡಿಸು by ಶಬ್ದ (newest ಮೊದಲ)\n\
"));
      oputs (_("\
      --ಸಮಯ-style=ಸಮಯ_STYLE\n\
         ಸಮಯ/date format with -l; see ಸಮಯ_STYLE ಕೆಳಗೆ\n\
"));
      oputs (_("\
  -t\n\
         ವಿಂಗಡಿಸು by ಸಮಯ, newest ಮೊದಲ; see --ಸಮಯ\n\
"));
      oputs (_("\
  -T, --tabsize=ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ\n\
         assume tab stops at each ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ instead ದ 8\n\
"));
      oputs (_("\
  -u\n\
         with -lt: ವಿಂಗಡಿಸು by, and show, access ಸಮಯ;\n\
         with -l: show access ಸಮಯ and ವಿಂಗಡಿಸು by name;\n\
         otherwise: ವಿಂಗಡಿಸು by access ಸಮಯ, newest ಮೊದಲ\n\
"));
      oputs (_("\
  -U\n\
         ಮಾಡು not ವಿಂಗಡಿಸು directory entries\n\
"));
      oputs (_("\
  -v\n\
         natural ವಿಂಗಡಿಸು ದ (ಆವೃತ್ತಿ_3) numbers within ಪಠ್ಯ\n\
"));
      oputs (_("\
  -w, --ಅಗಲ=ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ\n\
         ಗಣ_2 output ಅಗಲ ಗಮ್ಯ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ.  0 means no ಎಲ್ಲೆ\n\
"));
      oputs (_("\
  -x\n\
         list entries by ಸಾಲುಗಳು instead ದ by columns\n\
"));
      oputs (_("\
  -X\n\
         ವಿಂಗಡಿಸು alphabetically by ನಮೂದು extension\n\
"));
      oputs (_("\
  -Z, --context\n\
         print any security context ದ each file\n\
"));
      oputs (_("\
      --zero\n\
         ಕೊನೆ each output ಸಾಲು with NUL, not newline\n\
"));
      oputs (_("\
  -1\n\
         list one file ಪ್ರತಿ ಸಾಲು\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      emit_ಗಾತ್ರ_note ();
      fputs (_("\
\n\
The ಸಮಯ_STYLE ವಾದ can be ತುಂಬಿದ-iso, ದೀರ್ಘ-iso, iso, locale, or +FORMAT.\n\
FORMAT is interpreted like ಒಳ date(1).  If FORMAT is FORMAT1<newline>FORMAT2,\n\
then FORMAT1 applies ಗಮ್ಯ non-recent files and FORMAT2 ಗಮ್ಯ recent files.\n\
ಸಮಯ_STYLE prefixed with 'posix-' takes effect only outside the POSIX locale.\n\
Also the ಸಮಯ_STYLE environment variable sets the ಪೂರ್ವನಿಯೋಜಿತ style ಗಮ್ಯ use.\n\
"), stdout);
      fputs (_("\
\n\
The WHEN ವಾದ defaults ಗಮ್ಯ 'always' and can also be 'auto' or 'never'.\n\
"), stdout);
      fputs (_("\
\n\
Using color ಗಮ್ಯ distinguish file types is disabled both by ಪೂರ್ವನಿಯೋಜಿತ and\n\
with --color=never.  With --color=ಸ್ವಯಂಚಾಲಿತ, ls emits color codes only when\n\
standard output is connected ಗಮ್ಯ a terminal.  The LS_COLORS environment\n\
variable can change the settings.  Use the dircolors(1) command ಗಮ್ಯ ಗಣ_2 it.\n\
"), stdout);
      fputs (_("\
\n\
Exit ಸ್ಥಿತಿ_2:\n\
 0  ಆದರೆ OK,\n\
 1  ಆದರೆ minor problems (e.g., cannot access subdirectory),\n\
 2  ಆದರೆ serious trouble (e.g., cannot access command-ಸಾಲು ವಾದ).\n\
"), stdout);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}
