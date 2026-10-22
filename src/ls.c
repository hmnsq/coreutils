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
#ಯದಿ HAVE_STROPTS_H
# include <stropts.h>
#endif
#ಅನ್ತರ್ಭಾವ <sys/ioctl.h>

#ifdef WINSIZE_IN_PTEM
# include <sys/stream.h>
# include <sys/ptem.h>
#endif

#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <setjmp.h>
#ಅನ್ತರ್ಭಾವ <pwd.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <signal.h>

#ಯದಿ HAVE_LANGINFO_CODESET
# include <langinfo.h>
#endif

/* NonStop circa 2011 lacks both SA_RESTART and siginterrupt, so don't
   restart syscalls after a signal handler fires.  This may cause
   colors to get messed up on the screen if 'ls' is interrupted, but
   that's the best we can do on such a platform.  */
#ifndef SA_RESTART
# define SA_RESTART 0
#endif

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
#ifdef HAVE_CAP
# include <sys/capability.h>
#endif

#ಯದಿ HAVE_LINUX_XATTR_H
# include <linux/xattr.h>
# ifndef XATTR_NAME_CAPS
#  define XATTR_NAME_CAPS "security.capability"
# endif
#endif

#define PROGRAM_NAME (ls_mode == LS_LS ? "ls" \
                      : (ls_mode == LS_MULTI_COL \
                         ? "dir" : "vdir"))

#define AUTHORS \
  proper_name ("Richard M. Stallman"), \
  proper_name ("David MacKenzie")

#define obstack_chunk_alloc malloc
#define obstack_chunk_free ಮುಕ್ತ

/* Unix-based readdir implementations have historically returned a dirent.d_ino
   value that is sometimes not equal to the stat-obtained st_ino value for
   that same entry.  This error occurs for a readdir entry that refers
   to a mount point.  readdir's error is to return the inode number of
   the underlying directory -- one that typically cannot be stat'ed, as
   long as a file system is mounted on that directory.  RELIABLE_D_INO
   encapsulates whether we can use the more efficient approach of relying
   on readdir-supplied d_ino values, or whether we must incur the cost of
   calling stat or lstat to obtain each guaranteed-valid inode number.  */

#ifndef READDIR_LIES_ABOUT_MOUNTPOINT_D_INO
# define READDIR_LIES_ABOUT_MOUNTPOINT_D_INO 1
#endif

#ಯದಿ READDIR_LIES_ABOUT_MOUNTPOINT_D_INO
# define RELIABLE_D_INO(dp) NOT_AN_INODE_ಸಂಖ್ಯೆ
#ಅನ್ಯಥಾ
# define RELIABLE_D_INO(dp) D_INO (dp)
#endif

#ಯದಿ ! HAVE_STRUCT_STAT_ST_AUTHOR
# define st_author st_uid
#endif

ಗಣನಾ filetype
  {
    unknown,
    fifo,
    chardev,
    directory,
    blockdev,
    normal,
    symbolic_link,
    sock,
    whiteout,
    arg_directory
  };
ಗಣನಾ { filetype_cardinality = arg_directory + 1 };

/* Display letters and indicators for each filetype.
   Keep these in sync with enum filetype.  */
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ filetype_letter[] =
  {'?', 'p', 'c', 'd', 'b', '-', 'l', 's', 'w', 'd'};
static_assert (countof (filetype_letter) == filetype_cardinality);

/* Map enum filetype to <dirent.h> d_type values.  */
ಸ್ಥಿರ unsigned ವರ್ಣ ಧ್ರುವ filetype_d_type[] =
  {
    DT_UNKNOWN, DT_FIFO, DT_CHR, DT_DIR, DT_BLK, DT_REG, DT_LNK, DT_SOCK,
    DT_WHT, DT_DIR
  };
static_assert (countof (filetype_d_type) == filetype_cardinality);

/* Map d_type values to enum filetype.  */
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ d_type_filetype[UCHAR_MAX + 1] =
  {
    [DT_BLK] = blockdev, [DT_CHR] = chardev, [DT_DIR] = directory,
    [DT_FIFO] = fifo, [DT_LNK] = symbolic_link, [DT_REG] = normal,
    [DT_SOCK] = sock, [DT_WHT] = whiteout
  };

ಗಣನಾ acl_type
  {
    ACL_T_NONE,
    ACL_T_UNKNOWN,
    ACL_T_LSM_CONTEXT_ONLY,
    ACL_T_YES
  };

ಸಂರಚನಾ fileinfo
  {
    /* The file name.  */
    ವರ್ಣ *name;

    /* For symbolic link, name of the file linked to, otherwise zero.  */
    ವರ್ಣ *linkname;

    /* For terminal hyperlinks. */
    ವರ್ಣ *absolute_name;

    ಸಂರಚನಾ stat stat;

    ಗಣನಾ filetype filetype;

    /* For symbolic link and long listing, st_mode of file linked to, otherwise
       zero.  */
    mode_t linkmode;

    /* security context.  */
    ವರ್ಣ *scontext;

    bool stat_ok;

    /* For symbolic link and color printing, true if linked-to file
       exists, otherwise false.  */
    bool linkok;

    /* For long listings, true if the file has an access control list,
       or a security context.  */
    ಗಣನಾ acl_type acl_type;

    /* For color listings, true if a regular file has capability info.  */
    bool has_capability;

    /* Whether file name needs quoting. tri-state with -1 == unknown.  */
    ಪೂರ್ಣಾಙ್ಕ quoted;

    /* Cached screen width (including quoting).  */
    size_t width;
  };

/* Null is a valid character in a color indicator (think about Epson
   printers, for example) so we have to use a length/buffer string
   type.  */

ಸಂರಚನಾ bin_str
  {
    size_t len;			/* Number of bytes */
    ವರ್ಣ ಧ್ರುವ *string;		/* Pointer to the same */
  };

#ಯದಿ ! HAVE_TCGETPGRP
# define tcgetpgrp(Fd) 0
#endif

ಸ್ಥಿರ size_t quote_name (ವರ್ಣ ಧ್ರುವ *name,
                          ಸಂರಚನಾ quoting_options ಧ್ರುವ *options,
                          ಪೂರ್ಣಾಙ್ಕ needs_general_quoting,
                          ಧ್ರುವ ಸಂರಚನಾ bin_str *color,
                          bool allow_pad, ಸಂರಚನಾ obstack *ರಾಶಿ,
                          ವರ್ಣ ಧ್ರುವ *absolute_name);
ಸ್ಥಿರ size_t quote_name_buf (ವರ್ಣ **inbuf, size_t bufsize, ವರ್ಣ *name,
                              ಸಂರಚನಾ quoting_options ಧ್ರುವ *options,
                              ಪೂರ್ಣಾಙ್ಕ needs_general_quoting, size_t *width,
                              bool *pad);
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ decode_switches (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv);
ಸ್ಥಿರ bool file_ignored (ವರ್ಣ ಧ್ರುವ *name);
ಸ್ಥಿರ uintmax_t gobble_file (ವರ್ಣ ಧ್ರುವ *name, ಗಣನಾ filetype type,
                              ino_t inode, bool command_ಸಾಲು_arg,
                              ವರ್ಣ ಧ್ರುವ *dirname);
ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ bin_str * get_color_indicator (ಧ್ರುವ ಸಂರಚನಾ fileinfo *f,
                                                   bool symlink_target);
ಸ್ಥಿರ bool print_color_indicator (ಧ್ರುವ ಸಂರಚನಾ bin_str *ind);
ಸ್ಥಿರ ಶೂನ್ಯ put_indicator (ಧ್ರುವ ಸಂರಚನಾ bin_str *ind);
ಸ್ಥಿರ ಶೂನ್ಯ add_ignore_pattern (ವರ್ಣ ಧ್ರುವ *pattern);
ಸ್ಥಿರ ಶೂನ್ಯ attach (ವರ್ಣ *dest, ವರ್ಣ ಧ್ರುವ *dirname, ವರ್ಣ ಧ್ರುವ *name);
ಸ್ಥಿರ ಶೂನ್ಯ clear_files (ಶೂನ್ಯ);
ಸ್ಥಿರ ಶೂನ್ಯ extract_dirs_from_files (ವರ್ಣ ಧ್ರುವ *dirname,
                                     bool command_ಸಾಲು_arg);
ಸ್ಥಿರ bool get_link_name (ವರ್ಣ ಧ್ರುವ *filename, ಸಂರಚನಾ fileinfo *f,
                           bool command_ಸಾಲು_arg);
ಸ್ಥಿರ ಶೂನ್ಯ indent (size_t from, size_t to);
ಸ್ಥಿರ idx_t calculate_columns (bool by_columns);
ಸ್ಥಿರ ಶೂನ್ಯ print_current_files (ಶೂನ್ಯ);
ಸ್ಥಿರ ಶೂನ್ಯ print_dir (ವರ್ಣ ಧ್ರುವ *name, ವರ್ಣ ಧ್ರುವ *realname,
                       bool command_ಸಾಲು_arg);
ಸ್ಥಿರ size_t print_file_name_and_frills (ಧ್ರುವ ಸಂರಚನಾ fileinfo *f,
                                          size_t start_col);
ಸ್ಥಿರ ಶೂನ್ಯ print_horizontal (ಶೂನ್ಯ);
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ format_user_width (uid_t u);
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ format_group_width (gid_t g);
ಸ್ಥಿರ ಶೂನ್ಯ print_long_format (ಧ್ರುವ ಸಂರಚನಾ fileinfo *f);
ಸ್ಥಿರ ಶೂನ್ಯ print_many_per_ಸಾಲು (ಶೂನ್ಯ);
ಸ್ಥಿರ size_t print_name_with_quoting (ಧ್ರುವ ಸಂರಚನಾ fileinfo *f,
                                       bool symlink_target,
                                       ಸಂರಚನಾ obstack *ರಾಶಿ,
                                       size_t start_col);
ಸ್ಥಿರ ಶೂನ್ಯ prep_non_filename_text (ಶೂನ್ಯ);
ಸ್ಥಿರ bool print_type_indicator (bool stat_ok, mode_t mode,
                                  ಗಣನಾ filetype type);
ಸ್ಥಿರ ಶೂನ್ಯ print_with_separator (ವರ್ಣ sep);
ಸ್ಥಿರ ಶೂನ್ಯ queue_directory (ವರ್ಣ ಧ್ರುವ *name, ವರ್ಣ ಧ್ರುವ *realname,
                             bool command_ಸಾಲು_arg);
ಸ್ಥಿರ ಶೂನ್ಯ sort_files (ಶೂನ್ಯ);
ಸ್ಥಿರ ಶೂನ್ಯ parse_ls_color (ಶೂನ್ಯ);

ಸ್ಥಿರ size_t quote_name_width (ವರ್ಣ ಧ್ರುವ *name,
                                ಸಂರಚನಾ quoting_options ಧ್ರುವ *options,
                                ಪೂರ್ಣಾಙ್ಕ needs_general_quoting);

/* Initial size of hash table.
   Most hierarchies are likely to be shallower than this.  */
ಗಣನಾ { INITIAL_TABLE_SIZE = 30 };

/* The set of 'active' directories, from the current command-line argument
   to the level in the hierarchy at which files are being listed.
   A directory is represented by its device and inode numbers (struct dev_ino).
   A directory is added to this set when ls begins listing it or its
   entries, and it is removed from the set just after ls has finished
   processing it.  This set is used solely to detect loops, e.g., with
   mkdir loop; cd loop; ln -s ../loop sub; ls -RL  */
ಸ್ಥಿರ Hash_table *active_dir_set;

#define LOOP_DETECT (!!active_dir_set)

/* The table of files in the current directory:

   'cwd_file' points to a vector of 'struct fileinfo', one per file.
   'cwd_n_alloc' is the number of elements space has been allocated for.
   'cwd_n_used' is the number actually in use.  */

/* Address of block containing the files that are described.  */
ಸ್ಥಿರ ಸಂರಚನಾ fileinfo *cwd_file;

/* Length of block that 'cwd_file' points to, measured in files.  */
ಸ್ಥಿರ idx_t cwd_n_alloc;

/* Index of first unused slot in 'cwd_file'.  */
ಸ್ಥಿರ idx_t cwd_n_used;

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

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *hostname;

/* Mode of appropriate file for coloring.  */
ಸ್ಥಿರ mode_t
file_or_link_mode (ಸಂರಚನಾ fileinfo ಧ್ರುವ *file)
{
  ಪ್ರತ್ಯಾಗಮ (color_symlink_as_referent && file->linkok
          ? file->linkmode : file->stat.st_mode);
}


/* Record of one pending directory waiting to be listed.  */

ಸಂರಚನಾ pending
  {
    ವರ್ಣ *name;
    /* If the directory is actually the file pointed to by a symbolic link we
       were told to list, 'realname' will contain the name of the symbolic
       link, otherwise zero.  */
    ವರ್ಣ *realname;
    bool command_ಸಾಲು_arg;
    ಸಂರಚನಾ pending *next;
  };

ಸ್ಥಿರ ಸಂರಚನಾ pending *pending_dirs;

/* Current time in seconds and nanoseconds since 1970, updated as
   needed when deciding whether a file is recent.  */

ಸ್ಥಿರ ಸಂರಚನಾ timespec current_time;

ಸ್ಥಿರ bool print_scontext;
ಸ್ಥಿರ ವರ್ಣ UNKNOWN_SECURITY_CONTEXT[] = "?";

/* Whether any of the files has an ACL.  This affects the width of the
   mode column.  */

ಸ್ಥಿರ bool any_has_acl;

/* The number of columns to use for columns containing inode numbers,
   block sizes, link counts, owners, groups, authors, major device
   numbers, minor device numbers, and file sizes, respectively.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ inode_ಸಂಖ್ಯೆ_width;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ block_size_width;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ nlink_width;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ scontext_width;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ owner_width;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ group_width;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ author_width;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ major_device_ಸಂಖ್ಯೆ_width;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ minor_device_ಸಂಖ್ಯೆ_width;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ file_size_width;

/* Option flags */

/* long_format for lots of info, one per line.
   one_per_line for just names, one per line.
   many_per_line for just names, many per line, sorted vertically.
   horizontal for just names, many per line, sorted horizontally.
   with_commas for just names, many per line, separated by commas.

   -l (and other options that imply -l), -1, -C, -x and -m control
   this parameter.  */

ಗಣನಾ format
  {
    long_format,		/* -l and other options that imply -l */
    one_per_ಸಾಲು,		/* -1 */
    many_per_ಸಾಲು,		/* -C */
    horizontal,			/* -x */
    with_commas			/* -m */
  };

ಸ್ಥಿರ ಗಣನಾ format format;

/* 'full-iso' uses full ISO-style dates and times.  'long-iso' uses longer
   ISO-style timestamps, though shorter than 'full-iso'.  'iso' uses shorter
   ISO-style timestamps.  'locale' uses locale-dependent timestamps.  */
ಗಣನಾ time_style
  {
    full_iso_time_style,	/* --time-style=full-iso */
    long_iso_time_style,	/* --time-style=long-iso */
    iso_time_style,		/* --time-style=iso */
    locale_time_style		/* --time-style=locale */
  };

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *ಧ್ರುವ time_style_args[] =
{
  "full-iso", "long-iso", "iso", "locale", ರಿಕ್ತ
};
ಸ್ಥಿರ ಗಣನಾ time_style ಧ್ರುವ time_style_types[] =
{
  full_iso_time_style, long_iso_time_style, iso_time_style,
  locale_time_style
};
ARGMATCH_VERIFY (time_style_args, time_style_types);

/* Type of time to print or sort by.  Controlled by -c and -u.
   The values of each item of this enum are important since they are
   used as indices in the sort functions array (see sort_files()).  */

ಗಣನಾ time_type
  {
    time_mtime = 0,		/* default */
    time_ctime,			/* -c */
    time_atime,			/* -u */
    time_btime,                 /* birth time */
    time_numtypes		/* the number of elements of this enum */
  };

ಸ್ಥಿರ ಗಣನಾ time_type time_type;
ಸ್ಥಿರ bool explicit_time;

/* The file characteristic to sort by.  Controlled by -t, -S, -U, -X, -v.
   The values of each item of this enum are important since they are
   used as indices in the sort functions array (see sort_files()).  */

ಗಣನಾ sort_type
  {
    sort_name = 0,		/* default */
    sort_extension,		/* -X */
    sort_width,
    sort_size,			/* -S */
    sort_version,		/* -v */
    sort_time,			/* -t; must be second to last */
    sort_none,			/* -U; must be last */
    sort_numtypes		/* the number of elements of this enum */
  };

ಸ್ಥಿರ ಗಣನಾ sort_type sort_type;

/* Direction of sort.
   false means highest first if numeric,
   lowest first if alphabetic;
   these are the defaults.
   true means the opposite order in each case.  -r  */

ಸ್ಥಿರ bool sort_reverse;

/* True means to display owner information.  -g turns this off.  */

ಸ್ಥಿರ bool print_owner = true;

/* True means to display author information.  */

ಸ್ಥಿರ bool print_author;

/* True means to display group information.  -G and -o turn this off.  */

ಸ್ಥಿರ bool print_group = true;

/* True means print the user and group id's as numbers rather
   than as names.  -n  */

ಸ್ಥಿರ bool numeric_ids;

/* True means mention the size in blocks of each file.  -s  */

ಸ್ಥಿರ bool print_block_size;

/* Human-readable options for output, when printing block counts.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ human_output_opts;

/* The units to use when printing block counts.  */
ಸ್ಥಿರ uintmax_t output_block_size;

/* Likewise, but for file sizes.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ file_human_output_opts;
ಸ್ಥಿರ uintmax_t file_output_block_size = 1;

/* Follow the output with a special string.  Using this format,
   Emacs' dired mode starts up twice as fast, and can handle all
   strange characters in file names.  */
ಸ್ಥಿರ bool dired;

/* 'none' means don't mention the type of files.
   'slash' means mention directories only, with a '/'.
   'file_type' means mention file types.
   'classify' means mention file types and mark executables.

   Controlled by -F, -p, and --indicator-style.  */

ಗಣನಾ indicator_style
  {
    none = 0,	/*     --indicator-style=none (default) */
    slash,	/* -p, --indicator-style=slash */
    file_type,	/*     --indicator-style=file-type */
    classify	/* -F, --indicator-style=classify */
  };

ಸ್ಥಿರ ಗಣನಾ indicator_style indicator_style;

/* Names of indicator styles.  */
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *ಧ್ರುವ indicator_style_args[] =
{
  "none", "slash", "file-type", "classify", ರಿಕ್ತ
};
ಸ್ಥಿರ ಗಣನಾ indicator_style ಧ್ರುವ indicator_style_types[] =
{
  none, slash, file_type, classify
};
ARGMATCH_VERIFY (indicator_style_args, indicator_style_types);

/* True means use colors to mark types.  Also define the different
   colors as well as the stuff for the LS_COLORS environment variable.
   The LS_COLORS variable is now in a termcap-like format.  */

ಸ್ಥಿರ bool print_with_color;

ಸ್ಥಿರ bool print_hyperlink;

/* Whether we used any colors in the output so far.  If so, we will
   need to restore the default color later.  If not, we will need to
   call prep_non_filename_text before using color for the first time. */

ಸ್ಥಿರ bool used_color = false;

ಗಣನಾ when_type
  {
    when_never,		/* 0: default or --color=never */
    when_always,	/* 1: --color=always */
    when_if_tty		/* 2: --color=tty */
  };

ಗಣನಾ Dereference_symlink
  {
    DEREF_UNDEFINED = 0,		/* default */
    DEREF_NEVER,
    DEREF_COMMAND_ಸಾಲು_ARGUMENTS,	/* -H */
    DEREF_COMMAND_ಸಾಲು_SYMLINK_TO_DIR,	/* the default, in certain cases */
    DEREF_ALWAYS			/* -L */
  };

ಗಣನಾ indicator_no
  {
    C_ಎಡ, C_ಬಲ, C_END, C_RESET, C_NORM, C_FILE, C_DIR, C_LINK,
    C_FIFO, C_SOCK,
    C_BLK, C_CHR, C_MISSING, C_ORPHAN, C_EXEC, C_DOOR, C_SETUID, C_SETGID,
    C_STICKY, C_OTHER_WRITABLE, C_STICKY_OTHER_WRITABLE, C_CAP, C_MULTIHARDLINK,
    C_CLR_TO_EOL
  };

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ indicator_name[][2]=
  {
    {'l','c'}, {'r','c'}, {'e','c'}, {'r','s'}, {'n','o'},
    {'f','i'}, {'d','i'}, {'l','n'},
    {'p','i'}, {'s','o'},
    {'b','d'}, {'c','d'}, {'m','i'}, {'o','r'}, {'e','x'},
    {'d','o'}, {'s','u'}, {'s','g'},
    {'s','t'}, {'o','w'}, {'t','w'}, {'c','a'}, {'m','h'},
    {'c','l'}
  };

ಸಂರಚನಾ color_ext_type
  {
    ಸಂರಚನಾ bin_str ext;		/* The extension we're looking for */
    ಸಂರಚನಾ bin_str seq;		/* The sequence to output when we do */
    bool   exact_match;		/* Whether to compare case insensitively */
    ಸಂರಚನಾ color_ext_type *next;	/* Next in list */
  };

ಸ್ಥಿರ ಸಂರಚನಾ bin_str color_indicator[] =
  {
    { 2, (ವರ್ಣ ಧ್ರುವ []) {'\033','['} },/* lc: Left of color sequence */
    { 1, (ವರ್ಣ ಧ್ರುವ []) {'m'} },	/* rc: Right of color sequence */
    { 0, ರಿಕ್ತ },			/* ec: End color (replaces lc+rs+rc) */
    { 1, (ವರ್ಣ ಧ್ರುವ []) {'0'} },	/* rs: Reset to ordinary colors */
    { 0, ರಿಕ್ತ },			/* no: Normal */
    { 0, ರಿಕ್ತ },			/* fi: File: default */
    { 5, ((ವರ್ಣ ಧ್ರುವ [])
          {'0','1',';','3','4'}) },	/* di: Directory: bright blue */
    { 5, ((ವರ್ಣ ಧ್ರುವ [])
          {'0','1',';','3','6'}) },	/* ln: Symlink: bright cyan */
    { 2, (ವರ್ಣ ಧ್ರುವ []) {'3','3'} },		/* pi: Pipe: yellow/brown */
    { 5, ((ವರ್ಣ ಧ್ರುವ [])
          {'0','1',';','3','5'}) },	/* so: Socket: bright magenta */
    { 5, ((ವರ್ಣ ಧ್ರುವ [])
          {'0','1',';','3','3'}) },	/* bd: Block device: bright yellow */
    { 5, ((ವರ್ಣ ಧ್ರುವ [])
          {'0','1',';','3','3'}) },	/* cd: Char device: bright yellow */
    { 0, ರಿಕ್ತ },			/* mi: Missing file: undefined */
    { 0, ರಿಕ್ತ },			/* or: Orphaned symlink: undefined */
    { 5, ((ವರ್ಣ ಧ್ರುವ [])
          {'0','1',';','3','2'}) },	/* ex: Executable: bright green */
    { 5, ((ವರ್ಣ ಧ್ರುವ [])
          {'0','1',';','3','5'}) },	/* do: Door: bright magenta */
    { 5, ((ವರ್ಣ ಧ್ರುವ [])
          {'3','7',';','4','1'}) },	/* su: setuid: white on red */
    { 5, ((ವರ್ಣ ಧ್ರುವ [])
          {'3','0',';','4','3'}) },	/* sg: setgid: black on yellow */
    { 5, ((ವರ್ಣ ಧ್ರುವ [])
          {'3','7',';','4','4'}) },	/* st: sticky: black on blue */
    { 5, ((ವರ್ಣ ಧ್ರುವ [])
          {'3','4',';','4','2'}) },	/* ow: other-writable: blue on green */
    { 5, ((ವರ್ಣ ಧ್ರುವ [])
          {'3','0',';','4','2'}) },	/* tw: ow w/ sticky: black on green */
    { 0, ರಿಕ್ತ },			/* ca: disabled by default */
    { 0, ರಿಕ್ತ },			/* mh: disabled by default */
    { 3, ((ವರ್ಣ ಧ್ರುವ [])
          {'\033','[','K'}) },		/* cl: clear to end of line */
  };

/* A list mapping file extensions to corresponding display sequence.  */
ಸ್ಥಿರ ಸಂರಚನಾ color_ext_type *color_ext_list = ರಿಕ್ತ;

/* Buffer for color sequences */
ಸ್ಥಿರ ವರ್ಣ *color_buf;

/* True means to check for orphaned symbolic link, for displaying
   colors, or to group symlink to directories with other dirs.  */

ಸ್ಥಿರ bool check_symlink_mode;

/* True means mention the inode number of each file.  -i  */

ಸ್ಥಿರ bool print_inode;

/* What to do with symbolic links.  Affected by -d, -F, -H, -l (and
   other options that imply -l), and -L.  */

ಸ್ಥಿರ ಗಣನಾ Dereference_symlink dereference;

/* True means when a directory is found, display info on its
   contents.  -R  */

ಸ್ಥಿರ bool recursive;

/* True means when an argument is a directory name, display info
   on it itself.  -d  */

ಸ್ಥಿರ bool immediate_dirs;

/* True means that directories are grouped before files. */

ಸ್ಥಿರ bool directories_first;

/* Which files to ignore.  */

ಸ್ಥಿರ ಗಣನಾ
{
  /* Ignore files whose names start with '.', and files specified by
     --hide and --ignore.  */
  IGNORE_DEFAULT = 0,

  /* Ignore '.', '..', and files specified by --ignore.  */
  IGNORE_DOT_AND_DOTDOT,

  /* Ignore only files specified by --ignore.  */
  IGNORE_MINIMAL
} ignore_mode;

/* A linked list of shell-style globbing patterns.  If a non-argument
   file name matches any of these patterns, it is ignored.
   Controlled by -I.  Multiple -I options accumulate.
   The -B option adds '*~' and '.*~' to this list.  */

ಸಂರಚನಾ ignore_pattern
  {
    ವರ್ಣ ಧ್ರುವ *pattern;
    ಸಂರಚನಾ ignore_pattern *next;
  };

ಸ್ಥಿರ ಸಂರಚನಾ ignore_pattern *ignore_patterns;

/* Similar to IGNORE_PATTERNS, except that -a or -A causes this
   variable itself to be ignored.  */
ಸ್ಥಿರ ಸಂರಚನಾ ignore_pattern *hide_patterns;

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

ಸ್ಥಿರ ಸಂರಚನಾ quoting_options *filename_quoting_options;
ಸ್ಥಿರ ಸಂರಚನಾ quoting_options *dirname_quoting_options;

/* The number of chars per hardware tab stop.  Setting this to zero
   inhibits the use of TAB characters for separating columns.  -T */
ಸ್ಥಿರ size_t tabsize;

/* True means print each directory name before listing it.  */

ಸ್ಥಿರ bool print_dir_name;

/* The line length to use for breaking lines in many-per-line format.
   Can be set with -w.  If zero, there is no limit.  */

ಸ್ಥಿರ size_t ಸಾಲು_length;

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

ಗಣನಾ { TIME_STAMP_LEN_MAXIMUM = MAX (1000, INT_STRLEN_BOUND (time_t)) };

/* strftime formats for non-recent and recent files, respectively, in
   -l output.  */

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *long_time_format[2] =
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

ಸ್ಥಿರ sigset_t caught_signals;

/* If nonzero, the value of the pending fatal signal.  */

ಸ್ಥಿರ sig_atomic_t ಚಞ್ಚಲ interrupt_signal;

/* A count of the number of pending stop signals that have been received.  */

ಸ್ಥಿರ sig_atomic_t ಚಞ್ಚಲ stop_signal_ಎಣಿಕೆ;

/* Desired exit status.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ exit_status;

/* Exit statuses.  */
ಗಣನಾ
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
ಗಣನಾ
{
  AUTHOR_OPTION = CHAR_MAX + 1,
  BLOCK_SIZE_OPTION,
  COLOR_OPTION,
  DEREFERENCE_COMMAND_ಸಾಲು_SYMLINK_TO_DIR_OPTION,
  FILE_TYPE_INDICATOR_OPTION,
  FORMAT_OPTION,
  FULL_TIME_OPTION,
  GROUP_DIRECTORIES_FIRST_OPTION,
  HIDE_OPTION,
  HYPERLINK_OPTION,
  INDICATOR_STYLE_OPTION,
  QUOTING_STYLE_OPTION,
  SHOW_CONTROL_CHARS_OPTION,
  SI_OPTION,
  SORT_OPTION,
  TIME_OPTION,
  TIME_STYLE_OPTION,
  ZERO_OPTION,
};

ಸ್ಥಿರ ಸಂರಚನಾ option ಧ್ರುವ long_options[] =
{
  {"all", no_argument, ರಿಕ್ತ, 'a'},
  {"escape", no_argument, ರಿಕ್ತ, 'b'},
  {"directory", no_argument, ರಿಕ್ತ, 'd'},
  {"dired", no_argument, ರಿಕ್ತ, 'D'},
  {"full-time", no_argument, ರಿಕ್ತ, FULL_TIME_OPTION},
  {"group-directories-first", no_argument, ರಿಕ್ತ,
   GROUP_DIRECTORIES_FIRST_OPTION},
  {"human-readable", no_argument, ರಿಕ್ತ, 'h'},
  {"inode", no_argument, ರಿಕ್ತ, 'i'},
  {"kibibytes", no_argument, ರಿಕ್ತ, 'k'},
  {"numeric-uid-gid", no_argument, ರಿಕ್ತ, 'n'},
  {"no-group", no_argument, ರಿಕ್ತ, 'G'},
  {"hide-control-chars", no_argument, ರಿಕ್ತ, 'q'},
  {"reverse", no_argument, ರಿಕ್ತ, 'r'},
  {"size", no_argument, ರಿಕ್ತ, 's'},
  {"width", required_argument, ರಿಕ್ತ, 'w'},
  {"almost-all", no_argument, ರಿಕ್ತ, 'A'},
  {"ignore-backups", no_argument, ರಿಕ್ತ, 'B'},
  {"classify", optional_argument, ರಿಕ್ತ, 'F'},
  {"file-type", no_argument, ರಿಕ್ತ, FILE_TYPE_INDICATOR_OPTION},
  {"si", no_argument, ರಿಕ್ತ, SI_OPTION},
  {"dereference-command-line", no_argument, ರಿಕ್ತ, 'H'},
  {"dereference-command-line-symlink-to-dir", no_argument, ರಿಕ್ತ,
   DEREFERENCE_COMMAND_ಸಾಲು_SYMLINK_TO_DIR_OPTION},
  {"hide", required_argument, ರಿಕ್ತ, HIDE_OPTION},
  {"ignore", required_argument, ರಿಕ್ತ, 'I'},
  {"indicator-style", required_argument, ರಿಕ್ತ, INDICATOR_STYLE_OPTION},
  {"dereference", no_argument, ರಿಕ್ತ, 'L'},
  {"literal", no_argument, ರಿಕ್ತ, 'N'},
  {"quote-name", no_argument, ರಿಕ್ತ, 'Q'},
  {"quoting-style", required_argument, ರಿಕ್ತ, QUOTING_STYLE_OPTION},
  {"recursive", no_argument, ರಿಕ್ತ, 'R'},
  {"format", required_argument, ರಿಕ್ತ, FORMAT_OPTION},
  {"show-control-chars", no_argument, ರಿಕ್ತ, SHOW_CONTROL_CHARS_OPTION},
  {"sort", required_argument, ರಿಕ್ತ, SORT_OPTION},
  {"tabsize", required_argument, ರಿಕ್ತ, 'T'},
  {"time", required_argument, ರಿಕ್ತ, TIME_OPTION},
  {"time-style", required_argument, ರಿಕ್ತ, TIME_STYLE_OPTION},
  {"zero", no_argument, ರಿಕ್ತ, ZERO_OPTION},
  {"color", optional_argument, ರಿಕ್ತ, COLOR_OPTION},
  {"hyperlink", optional_argument, ರಿಕ್ತ, HYPERLINK_OPTION},
  {"block-size", required_argument, ರಿಕ್ತ, BLOCK_SIZE_OPTION},
  {"context", no_argument, ರಿಕ್ತ, 'Z'},
  {"author", no_argument, ರಿಕ್ತ, AUTHOR_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ರಿಕ್ತ, 0, ರಿಕ್ತ, 0}
};

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *ಧ್ರುವ format_args[] =
{
  "verbose", "long", "commas", "horizontal", "across",
  "vertical", "single-column", ರಿಕ್ತ
};
ಸ್ಥಿರ ಗಣನಾ format ಧ್ರುವ format_types[] =
{
  long_format, long_format, with_commas, horizontal, horizontal,
  many_per_ಸಾಲು, one_per_ಸಾಲು
};
ARGMATCH_VERIFY (format_args, format_types);

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *ಧ್ರುವ sort_args[] =
{
  "none", "size", "time", "version", "extension",
  "name", "width", ರಿಕ್ತ
};
ಸ್ಥಿರ ಗಣನಾ sort_type ಧ್ರುವ sort_types[] =
{
  sort_none, sort_size, sort_time, sort_version, sort_extension,
  sort_name, sort_width
};
ARGMATCH_VERIFY (sort_args, sort_types);

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *ಧ್ರುವ time_args[] =
{
  "atime", "access", "use",
  "ctime", "status",
  "mtime", "modification",
  "birth", "creation",
  ರಿಕ್ತ
};
ಸ್ಥಿರ ಗಣನಾ time_type ಧ್ರುವ time_types[] =
{
  time_atime, time_atime, time_atime,
  time_ctime, time_ctime,
  time_mtime, time_mtime,
  time_btime, time_btime,
};
ARGMATCH_VERIFY (time_args, time_types);

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *ಧ್ರುವ when_args[] =
{
  /* force and none are for compatibility with another color-ls version */
  "always", "yes", "force",
  "never", "no", "none",
  "auto", "tty", "if-tty", ರಿಕ್ತ
};
ಸ್ಥಿರ ಗಣನಾ when_type ಧ್ರುವ when_types[] =
{
  when_always, when_always, when_always,
  when_never, when_never, when_never,
  when_if_tty, when_if_tty, when_if_tty
};
ARGMATCH_VERIFY (when_args, when_types);

/* Information about filling a column.  */
ಸಂರಚನಾ column_info
{
  bool valid_len;
  size_t ಸಾಲು_len;
  size_t *col_arr;
};

/* Array with information about column fullness.  */
ಸ್ಥಿರ ಸಂರಚನಾ column_info *column_info;

/* Maximum number of columns ever possible for this display.  */
ಸ್ಥಿರ size_t max_idx;

/* The minimum width of a non-final column is 3: 1 character for the name
   and 2 for the separating white space.  The final column is not padded.  */
ಗಣನಾ { MIN_COLUMN_WIDTH = 3 };


/* This zero-based index is for the --dired option.  It is incremented
   for each byte of output generated by this program so that the beginning
   and ending indices (in that output) of every file name can be recorded
   and later output themselves.  */
ಸ್ಥಿರ off_t dired_pos;

ಸ್ಥಿರ ಶೂನ್ಯ
dired_outbyte (ವರ್ಣ c)
{
  dired_pos++;
  putchar (c);
}

/* Output the buffer S, of length S_LEN, and increment DIRED_POS by S_LEN.  */
ಸ್ಥಿರ ಶೂನ್ಯ
dired_outbuf (ವರ್ಣ ಧ್ರುವ *s, size_t s_len)
{
  dired_pos += s_len;
  fwrite (s, ಪರಿಮಾಣ *s, s_len, stdout);
}

/* Output the string S, and increment DIRED_POS by its length.  */
ಸ್ಥಿರ ಶೂನ್ಯ
dired_outstring (ವರ್ಣ ಧ್ರುವ *s)
{
  dired_outbuf (s, strlen (s));
}

ಸ್ಥಿರ ಶೂನ್ಯ
dired_indent (ಶೂನ್ಯ)
{
  ಯದಿ (dired)
    dired_outstring ("  ");
}

/* With --dired, store pairs of beginning and ending indices of file names.  */
ಸ್ಥಿರ ಸಂರಚನಾ obstack dired_obstack;

/* With --dired, store pairs of beginning and ending indices of any
   directory names that appear as headers (just before 'total' line)
   for lists of directory entries.  Such directory names are seen when
   listing hierarchies using -R and when a directory is listed with at
   least one other command line argument.  */
ಸ್ಥಿರ ಸಂರಚನಾ obstack subdired_obstack;

/* Save the current index on the specified obstack, OBS.  */
ಸ್ಥಿರ ಶೂನ್ಯ
ತಳ್ಳು_current_dired_pos (ಸಂರಚನಾ obstack *obs)
{
  ಯದಿ (dired)
    obstack_grow (obs, &dired_pos, ಪರಿಮಾಣ dired_pos);
}

/* With -R, this stack is used to help detect directory cycles.
   The device/inode pairs on this stack mirror the pairs in the
   active_dir_set hash table.  */
ಸ್ಥಿರ ಸಂರಚನಾ obstack dev_ino_obstack;

/* Push a pair onto the device/inode stack.  */
ಸ್ಥಿರ ಶೂನ್ಯ
dev_ino_ತಳ್ಳು (dev_t dev, ino_t ino)
{
  ಶೂನ್ಯ *vdi;
  ಸಂರಚನಾ dev_ino *di;
  ಪೂರ್ಣಾಙ್ಕ dev_ino_size = ಪರಿಮಾಣ *di;
  obstack_blank (&dev_ino_obstack, dev_ino_size);
  vdi = obstack_next_free (&dev_ino_obstack);
  di = vdi;
  di--;
  di->st_dev = dev;
  di->st_ino = ino;
}

/* Pop a dev/ino struct off the global dev_ino_obstack
   and return that struct.  */
ಸ್ಥಿರ ಸಂರಚನಾ dev_ino
dev_ino_ತೆಗೆ (ಶೂನ್ಯ)
{
  ಶೂನ್ಯ *vdi;
  ಸಂರಚನಾ dev_ino *di;
  ಪೂರ್ಣಾಙ್ಕ dev_ino_size = ಪರಿಮಾಣ *di;
  affirm (dev_ino_size <= obstack_object_size (&dev_ino_obstack));
  obstack_blank_fast (&dev_ino_obstack, -dev_ino_size);
  vdi = obstack_next_free (&dev_ino_obstack);
  di = vdi;
  ಪ್ರತ್ಯಾಗಮ *di;
}

ಸ್ಥಿರ ಶೂನ್ಯ
assert_matching_dev_ino (ವರ್ಣ ಧ್ರುವ *name, ಸಂರಚನಾ dev_ino di)
{
  MAYBE_UNUSED ಸಂರಚನಾ stat sb;
  assure (0 <= stat (name, &sb));
  assure (sb.st_dev == di.st_dev);
  assure (sb.st_ino == di.st_ino);
}

ಸ್ಥಿರ ವರ್ಣ eolbyte = '\n';

/* Write to standard output PREFIX, followed by the quoting style and
   a space-separated list of the integers stored in OS all on one line.  */

ಸ್ಥಿರ ಶೂನ್ಯ
dired_dump_obstack (ವರ್ಣ ಧ್ರುವ *prefix, ಸಂರಚನಾ obstack *os)
{
  size_t n_pos;

  n_pos = obstack_object_size (os) / ಪರಿಮಾಣ (dired_pos);
  ಯದಿ (n_pos > 0)
    {
      off_t *pos = obstack_finish (os);
      fputs (prefix, stdout);
      ಪ್ರತಿ (size_t i = 0; i < n_pos; i++)
        {
          intmax_t p = pos[i];
          printf (" %jd", p);
        }
      putchar ('\n');
    }
}

/* Return the platform birthtime member of the stat structure,
   or fallback to the mtime member, which we have populated
   from the statx structure or reset to an invalid timestamp
   where birth time is not supported.  */
ಸ್ಥಿರ ಸಂರಚನಾ timespec
get_stat_btime (ಸಂರಚನಾ stat ಧ್ರುವ *st)
{
  ಸಂರಚನಾ timespec btimespec;

#ಯದಿ HAVE_STATX && defined STATX_INO
  btimespec = get_stat_mtime (st);
#ಅನ್ಯಥಾ
  btimespec = get_stat_birthtime (st);
#endif

  ಪ್ರತ್ಯಾಗಮ btimespec;
}

#ಯದಿ HAVE_STATX && defined STATX_INO
ATTRIBUTE_PURE
ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ
time_type_to_statx (ಶೂನ್ಯ)
{
  ವಿಕಲ್ಪ (time_type)
    {
    ಪಕ್ಷ time_ctime:
      ಪ್ರತ್ಯಾಗಮ STATX_CTIME;
    ಪಕ್ಷ time_mtime:
      ಪ್ರತ್ಯಾಗಮ STATX_MTIME;
    ಪಕ್ಷ time_atime:
      ಪ್ರತ್ಯಾಗಮ STATX_ATIME;
    ಪಕ್ಷ time_btime:
      ಪ್ರತ್ಯಾಗಮ STATX_BTIME;
    ಪಕ್ಷ time_numtypes: ಸ್ವಭಾವ:
      unreachable ();
    }
    ಪ್ರತ್ಯಾಗಮ 0;
}

ATTRIBUTE_PURE
ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ
calc_req_mask (ಶೂನ್ಯ)
{
  unsigned ಪೂರ್ಣಾಙ್ಕ mask = STATX_MODE;

  ಯದಿ (print_inode)
    mask |= STATX_INO;

  ಯದಿ (print_block_size)
    mask |= STATX_BLOCKS;

  ಯದಿ (format == long_format) {
    mask |= STATX_NLINK | STATX_SIZE | time_type_to_statx ();
    ಯದಿ (print_owner || print_author)
      mask |= STATX_UID;
    ಯದಿ (print_group)
      mask |= STATX_GID;
  }

  ವಿಕಲ್ಪ (sort_type)
    {
    ಪಕ್ಷ sort_none:
    ಪಕ್ಷ sort_name:
    ಪಕ್ಷ sort_version:
    ಪಕ್ಷ sort_extension:
    ಪಕ್ಷ sort_width:
      ಭಙ್ಗ;
    ಪಕ್ಷ sort_time:
      mask |= time_type_to_statx ();
      ಭಙ್ಗ;
    ಪಕ್ಷ sort_size:
      mask |= STATX_SIZE;
      ಭಙ್ಗ;
    ಪಕ್ಷ sort_numtypes: ಸ್ವಭಾವ:
      unreachable ();
    }

  ಪ್ರತ್ಯಾಗಮ mask;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
do_statx (ಪೂರ್ಣಾಙ್ಕ fd, ವರ್ಣ ಧ್ರುವ *name, ಸಂರಚನಾ stat *st, ಪೂರ್ಣಾಙ್ಕ flags,
          unsigned ಪೂರ್ಣಾಙ್ಕ mask)
{
  ಸಂರಚನಾ statx stx;
  bool want_btime = mask & STATX_BTIME;
  ಪೂರ್ಣಾಙ್ಕ ret = statx (fd, name, flags | AT_NO_AUTOMOUNT, mask, &stx);
  ಯದಿ (ret >= 0)
    {
      statx_to_stat (&stx, st);
      /* Since we only need one timestamp type,
         store birth time in st_mtim.  */
      ಯದಿ (want_btime)
        {
          ಯದಿ (stx.stx_mask & STATX_BTIME)
            st->st_mtim = statx_timestamp_to_timespec (stx.stx_btime);
          ಅನ್ಯಥಾ
            st->st_mtim.tv_sec = st->st_mtim.tv_nsec = -1;
        }
    }

  ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
do_stat (ವರ್ಣ ಧ್ರುವ *name, ಸಂರಚನಾ stat *st)
{
  ಪ್ರತ್ಯಾಗಮ do_statx (AT_FDCWD, name, st, 0, calc_req_mask ());
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
do_lstat (ವರ್ಣ ಧ್ರುವ *name, ಸಂರಚನಾ stat *st)
{
  ಪ್ರತ್ಯಾಗಮ do_statx (AT_FDCWD, name, st, AT_SYMLINK_NOFOLLOW, calc_req_mask ());
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
stat_for_mode (ವರ್ಣ ಧ್ರುವ *name, ಸಂರಚನಾ stat *st)
{
  ಪ್ರತ್ಯಾಗಮ do_statx (AT_FDCWD, name, st, 0, STATX_MODE);
}

/* dev+ino should be static, so no need to sync with backing store */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
stat_for_ino (ವರ್ಣ ಧ್ರುವ *name, ಸಂರಚನಾ stat *st)
{
  ಪ್ರತ್ಯಾಗಮ do_statx (AT_FDCWD, name, st, 0, STATX_INO);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
fstat_for_ino (ಪೂರ್ಣಾಙ್ಕ fd, ಸಂರಚನಾ stat *st)
{
  ಪ್ರತ್ಯಾಗಮ do_statx (fd, "", st, AT_EMPTY_PATH, STATX_INO);
}
#ಅನ್ಯಥಾ
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
do_stat (ವರ್ಣ ಧ್ರುವ *name, ಸಂರಚನಾ stat *st)
{
  ಪ್ರತ್ಯಾಗಮ stat (name, st);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
do_lstat (ವರ್ಣ ಧ್ರುವ *name, ಸಂರಚನಾ stat *st)
{
  ಪ್ರತ್ಯಾಗಮ lstat (name, st);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
stat_for_mode (ವರ್ಣ ಧ್ರುವ *name, ಸಂರಚನಾ stat *st)
{
  ಪ್ರತ್ಯಾಗಮ stat (name, st);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
stat_for_ino (ವರ್ಣ ಧ್ರುವ *name, ಸಂರಚನಾ stat *st)
{
  ಪ್ರತ್ಯಾಗಮ stat (name, st);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
fstat_for_ino (ಪೂರ್ಣಾಙ್ಕ fd, ಸಂರಚನಾ stat *st)
{
  ಪ್ರತ್ಯಾಗಮ fstat (fd, st);
}
#endif

/* Return the address of the first plain %b spec in FMT, or NULL if
   there is no such spec.  %5b etc. do not match, so that user
   widths/flags are honored.  */

ATTRIBUTE_PURE
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *
first_percent_b (ವರ್ಣ ಧ್ರುವ *fmt)
{
  ಪ್ರತಿ (; *fmt; fmt++)
    ಯದಿ (fmt[0] == '%')
      ವಿಕಲ್ಪ (fmt[1])
        {
        ಪಕ್ಷ 'b': ಪ್ರತ್ಯಾಗಮ fmt;
        ಪಕ್ಷ '%': fmt++; ಭಙ್ಗ;
        }
  ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

ಸ್ಥಿರ ವರ್ಣ RFC3986[256];
ಸ್ಥಿರ ಶೂನ್ಯ
file_escape_init (ಶೂನ್ಯ)
{
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < 256; i++)
    RFC3986[i] |= c_isalnum (i) || i == '~' || i == '-' || i == '.' || i == '_';
}

ಗಣನಾ { MBSWIDTH_FLAGS = MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE };

/* Read the abbreviated month names from the locale, to align them
   and to determine the max width of the field and to truncate names
   greater than our max allowed.
   Note even though this handles multibyte locales correctly
   it's not restricted to them as single byte locales can have
   variable width abbreviated months and also precomputing/caching
   the names was seen to increase the performance of ls significantly.  */

/* abformat[RECENT][MON] is the format to use for timestamps with
   recentness RECENT and month MON.  */
ಗಣನಾ { ABFORMAT_SIZE = 128 };
ಸ್ಥಿರ ವರ್ಣ abformat[2][12][ABFORMAT_SIZE];
/* True if precomputed formats should be used.  This can be false if
   nl_langinfo fails, if a format or month abbreviation is unusually
   long, or if a month abbreviation contains '%'.  */
ಸ್ಥಿರ bool use_abformat;

/* Store into ABMON the abbreviated month names, suitably aligned.
   Return true if successful.  */

ಸ್ಥಿರ bool
abmon_init (ವರ್ಣ abmon[12][ABFORMAT_SIZE])
{
#ifndef HAVE_NL_LANGINFO
  ಪ್ರತ್ಯಾಗಮ false;
#ಅನ್ಯಥಾ
  ಪೂರ್ಣಾಙ್ಕ max_mon_width = 0;
  ಪೂರ್ಣಾಙ್ಕ mon_width[12];
  ಪೂರ್ಣಾಙ್ಕ mon_len[12];

  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < 12; i++)
    {
      ವರ್ಣ ಧ್ರುವ *abbr = nl_langinfo (ABMON_1 + i);
      mon_len[i] = strnlen (abbr, ABFORMAT_SIZE);
      ಯದಿ (mon_len[i] == ABFORMAT_SIZE)
        ಪ್ರತ್ಯಾಗಮ false;
      ಯದಿ (strchr (abbr, '%'))
        ಪ್ರತ್ಯಾಗಮ false;
      mon_width[i] = mbswidth (strcpy (abmon[i], abbr), MBSWIDTH_FLAGS);
      ಯದಿ (mon_width[i] < 0)
        ಪ್ರತ್ಯಾಗಮ false;
      max_mon_width = MAX (max_mon_width, mon_width[i]);
    }

  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < 12; i++)
    {
      ಪೂರ್ಣಾಙ್ಕ fill = max_mon_width - mon_width[i];
      ಯದಿ (ABFORMAT_SIZE - mon_len[i] <= fill)
        ಪ್ರತ್ಯಾಗಮ false;
      bool align_ಎಡ = !c_isdigit (abmon[i][0]);
      ಪೂರ್ಣಾಙ್ಕ fill_offset;
      ಯದಿ (align_ಎಡ)
        fill_offset = mon_len[i];
      ಅನ್ಯಥಾ
        {
          memmove (abmon[i] + fill, abmon[i], mon_len[i]);
          fill_offset = 0;
        }
      memset (abmon[i] + fill_offset, ' ', fill);
      abmon[i][mon_len[i] + fill] = '\0';
    }

  ಪ್ರತ್ಯಾಗಮ true;
#endif
}

/* Initialize ABFORMAT and USE_ABFORMAT.  */

ಸ್ಥಿರ ಶೂನ್ಯ
abformat_init (ಶೂನ್ಯ)
{
  ವರ್ಣ ಧ್ರುವ *pb[2];
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ recent = 0; recent < 2; recent++)
    pb[recent] = first_percent_b (long_time_format[recent]);
  ಯದಿ (! (pb[0] || pb[1]))
    ಪ್ರತ್ಯಾಗಮ;

  ವರ್ಣ abmon[12][ABFORMAT_SIZE];
  ಯದಿ (! abmon_init (abmon))
    ಪ್ರತ್ಯಾಗಮ;

  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ recent = 0; recent < 2; recent++)
    {
      ವರ್ಣ ಧ್ರುವ *fmt = long_time_format[recent];
      ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < 12; i++)
        {
          ವರ್ಣ *nfmt = abformat[recent][i];
          ಪೂರ್ಣಾಙ್ಕ nbytes;

          ಯದಿ (! pb[recent])
            nbytes = snprintf (nfmt, ABFORMAT_SIZE, "%s", fmt);
          ಅನ್ಯಥಾ
            {
              ಯದಿ (! (pb[recent] - fmt <= MIN (ABFORMAT_SIZE, INT_MAX)))
                ಪ್ರತ್ಯಾಗಮ;
              ಪೂರ್ಣಾಙ್ಕ prefix_len = pb[recent] - fmt;
              nbytes = snprintf (nfmt, ABFORMAT_SIZE, "%.*s%s%s",
                                 prefix_len, fmt, abmon[i], pb[recent] + 2);
            }

          ಯದಿ (! (0 <= nbytes && nbytes < ABFORMAT_SIZE))
            ಪ್ರತ್ಯಾಗಮ;
        }
    }

  use_abformat = true;
}

ಸ್ಥಿರ size_t
dev_ino_hash (ಶೂನ್ಯ ಧ್ರುವ *x, size_t table_size)
{
  ಸಂರಚನಾ dev_ino ಧ್ರುವ *p = x;
  ಪ್ರತ್ಯಾಗಮ (uintmax_t) {p->st_ino} % table_size;
}

ಸ್ಥಿರ bool
dev_ino_compare (ಶೂನ್ಯ ಧ್ರುವ *x, ಶೂನ್ಯ ಧ್ರುವ *y)
{
  ಸಂರಚನಾ dev_ino ಧ್ರುವ *a = x;
  ಸಂರಚನಾ dev_ino ಧ್ರುವ *b = y;
  ಪ್ರತ್ಯಾಗಮ PSAME_INODE (a, b);
}

ಸ್ಥಿರ ಶೂನ್ಯ
dev_ino_free (ಶೂನ್ಯ *x)
{
  ಮುಕ್ತ (x);
}

/* Add the device/inode pair (P->st_dev/P->st_ino) to the set of
   active directories.  Return true if there is already a matching
   entry in the table.  */

ಸ್ಥಿರ bool
visit_dir (dev_t dev, ino_t ino)
{
  ಸಂರಚನಾ dev_ino *ent;
  ಸಂರಚನಾ dev_ino *ent_from_table;
  bool found_match;

  ent = xmalloc (ಪರಿಮಾಣ *ent);
  ent->st_ino = ino;
  ent->st_dev = dev;

  /* Attempt to insert this entry into the table.  */
  ent_from_table = hash_ಸೇರಿಸು (active_dir_set, ent);

  ಯದಿ (ent_from_table == ರಿಕ್ತ)
    {
      /* Insertion failed due to lack of memory.  */
      xalloc_die ();
    }

  found_match = (ent_from_table != ent);

  ಯದಿ (found_match)
    {
      /* ent was not inserted, so free it.  */
      ಮುಕ್ತ (ent);
    }

  ಪ್ರತ್ಯಾಗಮ found_match;
}

ಸ್ಥಿರ ಶೂನ್ಯ
free_pending_ent (ಸಂರಚನಾ pending *p)
{
  ಮುಕ್ತ (p->name);
  ಮುಕ್ತ (p->realname);
  ಮುಕ್ತ (p);
}

ಸ್ಥಿರ bool
is_colored (ಗಣನಾ indicator_no type)
{
  /* Return true unless the string is "", "0" or "00"; try to be efficient.  */
  size_t len = color_indicator[type].len;
  ಯದಿ (len == 0)
    ಪ್ರತ್ಯಾಗಮ false;
  ಯದಿ (2 < len)
    ಪ್ರತ್ಯಾಗಮ true;
  ವರ್ಣ ಧ್ರುವ *s = color_indicator[type].string;
  ಪ್ರತ್ಯಾಗಮ (s[0] != '0') | (s[len - 1] != '0');
}

ಸ್ಥಿರ ಶೂನ್ಯ
restore_default_color (ಶೂನ್ಯ)
{
  put_indicator (&color_indicator[C_ಎಡ]);
  put_indicator (&color_indicator[C_ಬಲ]);
}

ಸ್ಥಿರ ಶೂನ್ಯ
set_normal_color (ಶೂನ್ಯ)
{
  ಯದಿ (print_with_color && is_colored (C_NORM))
    {
      put_indicator (&color_indicator[C_ಎಡ]);
      put_indicator (&color_indicator[C_NORM]);
      put_indicator (&color_indicator[C_ಬಲ]);
    }
}

/* An ordinary signal was received; arrange for the program to exit.  */

ಸ್ಥಿರ ಶೂನ್ಯ
sighandler (ಪೂರ್ಣಾಙ್ಕ sig)
{
  ಯದಿ (! interrupt_signal)
    interrupt_signal = sig;
}

/* A SIGTSTP was received; arrange for the program to suspend itself.  */

ಸ್ಥಿರ ಶೂನ್ಯ
stophandler (MAYBE_UNUSED ಪೂರ್ಣಾಙ್ಕ sig)
{
  ಯದಿ (! interrupt_signal)
    stop_signal_ಎಣಿಕೆ++;
}

/* Process any pending signals.  If signals are caught, this function
   should be called periodically.  Ideally there should never be an
   unbounded amount of time when signals are not being processed.
   Signal handling can restore the default colors, so callers must
   immediately change colors after invoking this function.  */

ಸ್ಥಿರ ಶೂನ್ಯ
process_signals (ಶೂನ್ಯ)
{
  ಯಾವತ್ (interrupt_signal || stop_signal_ಎಣಿಕೆ)
    {
      ಪೂರ್ಣಾಙ್ಕ sig;
      ಪೂರ್ಣಾಙ್ಕ stops;
      sigset_t oldset;

      ಯದಿ (used_color)
        restore_default_color ();
      fflush (stdout);

      sigprocmask (SIG_BLOCK, &caught_signals, &oldset);

      /* Reload interrupt_signal and stop_signal_count, in case a new
         signal was handled before sigprocmask took effect.  */
      sig = interrupt_signal;
      stops = stop_signal_ಎಣಿಕೆ;

      /* SIGTSTP is special, since the application can receive that signal
         more than once.  In this case, don't set the signal handler to the
         default.  Instead, just raise the uncatchable SIGSTOP.  */
      ಯದಿ (stops)
        {
          stop_signal_ಎಣಿಕೆ = stops - 1;
          sig = SIGSTOP;
        }
      ಅನ್ಯಥಾ
        ಸಙ್ಕೇತ (sig, SIG_DFL);

      /* Exit or suspend the program.  */
      ಉತ್ಥಾಪಯ (sig);
      sigprocmask (SIG_SETMASK, &oldset, ರಿಕ್ತ);

      /* If execution reaches here, then the program has been
         continued (after being suspended).  */
    }
}

/* Setup signal handlers if INIT is true,
   otherwise restore to the default.  */

ಸ್ಥಿರ ಶೂನ್ಯ
signal_setup (bool init)
{
  /* The signals that are trapped, and the number of such signals.  */
  ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ ಧ್ರುವ stop_sig[] =
    {
      /* This one is handled specially.  */
      SIGTSTP,
    };

  ಗಣನಾ { nsigs = countof (term_sig), nstop = countof (stop_sig) };

  ಯದಿ (init)
    {
      ಸಂರಚನಾ sigaction act;

      sigemptyset (&caught_signals);
      ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; j < nsigs + nstop; j++)
        {
          ಪೂರ್ಣಾಙ್ಕ sig = j < nsigs ? term_sig[j]: stop_sig[j - nsigs];
          sigaction (sig, ರಿಕ್ತ, &act);
          ಯದಿ (act.sa_handler != SIG_IGN)
            sigaddset (&caught_signals, sig);
        }

      act.sa_mask = caught_signals;
      act.sa_flags = SA_RESTART;

      ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; j < nsigs + nstop; j++)
        {
          ಪೂರ್ಣಾಙ್ಕ sig = j < nsigs ? term_sig[j]: stop_sig[j - nsigs];
          ಯದಿ (sigismember (&caught_signals, sig))
            {
              act.sa_handler = j < nsigs ? sighandler : stophandler;
              sigaction (sig, &act, ರಿಕ್ತ);
            }
        }
    }
  ಅನ್ಯಥಾ /* restore.  */
    {
      ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; j < nsigs + nstop; j++)
        {
          ಪೂರ್ಣಾಙ್ಕ sig = j < nsigs ? term_sig[j]: stop_sig[j - nsigs];
          ಯದಿ (sigismember (&caught_signals, sig))
            ಸಙ್ಕೇತ (sig, SIG_DFL);
        }
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
signal_init (ಶೂನ್ಯ)
{
  signal_setup (true);
}

ಸ್ಥಿರ ಶೂನ್ಯ
signal_restore (ಶೂನ್ಯ)
{
  signal_setup (false);
}

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  ಪೂರ್ಣಾಙ್ಕ i;
  ಸಂರಚನಾ pending *thispend;
  ಪೂರ್ಣಾಙ್ಕ n_files;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (LS_FAILURE);
  atexit (close_stdout);

  static_assert (countof (color_indicator) == countof (indicator_name));

  exit_status = EXIT_SUCCESS;
  print_dir_name = true;
  pending_dirs = ರಿಕ್ತ;

  current_time.tv_sec = TYPE_MINIMUM (time_t);
  current_time.tv_nsec = -1;

  i = decode_switches (argc, argv);

  ಯದಿ (print_with_color)
    parse_ls_color ();

  /* Test print_with_color again, because the call to parse_ls_color
     may have just reset it -- e.g., if LS_COLORS is invalid.  */

  ಯದಿ (print_with_color)
    {
      /* Don't use TAB characters in output.  Some terminal
         emulators can't handle the combination of tabs and
         color codes on the same line.  */
      tabsize = 0;
    }

  ಯದಿ (directories_first)
    check_symlink_mode = true;
  ಅನ್ಯಥಾ ಯದಿ (print_with_color)
    {
      /* Avoid following symbolic links when possible.  */
      ಯದಿ (is_colored (C_ORPHAN)
          || (is_colored (C_EXEC) && color_symlink_as_referent)
          || (is_colored (C_MISSING) && format == long_format))
        check_symlink_mode = true;
    }

  ಯದಿ (dereference == DEREF_UNDEFINED)
    dereference = ((immediate_dirs
                    || indicator_style == classify
                    || format == long_format)
                   ? DEREF_NEVER
                   : DEREF_COMMAND_ಸಾಲು_SYMLINK_TO_DIR);

  /* When using -R, initialize a data structure we'll use to
     detect any directory cycles.  */
  ಯದಿ (recursive)
    {
      active_dir_set = hash_initialize (INITIAL_TABLE_SIZE, ರಿಕ್ತ,
                                        dev_ino_hash,
                                        dev_ino_compare,
                                        dev_ino_free);
      ಯದಿ (active_dir_set == ರಿಕ್ತ)
        xalloc_die ();

      obstack_init (&dev_ino_obstack);
    }

  localtz = tzalloc (getenv ("TZ"));

  format_needs_stat = ((sort_type == sort_time) | (sort_type == sort_size)
                       | (format == long_format)
                       | print_block_size | print_hyperlink | print_scontext);
  format_needs_type = ((! format_needs_stat)
                       & (recursive | print_with_color | print_scontext
                          | directories_first
                          | (indicator_style != none)));
  format_needs_capability = print_with_color && is_colored (C_CAP);

  ಯದಿ (dired)
    {
      obstack_init (&dired_obstack);
      obstack_init (&subdired_obstack);
    }

  ಯದಿ (print_hyperlink)
    {
      file_escape_init ();

      hostname = xgethostname ();
      /* The hostname is generally ignored,
         so ignore failures obtaining it.  */
      ಯದಿ (! hostname)
        hostname = "";
    }

  cwd_n_alloc = 100;
  cwd_file = xmalloc (cwd_n_alloc * ಪರಿಮಾಣ *cwd_file);
  cwd_n_used = 0;

  clear_files ();

  n_files = argc - i;

  ಯದಿ (n_files <= 0)
    {
      ಯದಿ (immediate_dirs)
        gobble_file (".", directory, NOT_AN_INODE_ಸಂಖ್ಯೆ, true, ರಿಕ್ತ);
      ಅನ್ಯಥಾ
        queue_directory (".", ರಿಕ್ತ, true);
    }
  ಅನ್ಯಥಾ
    ಕುರು
      gobble_file (argv[i++], unknown, NOT_AN_INODE_ಸಂಖ್ಯೆ, true, ರಿಕ್ತ);
    ಯಾವತ್ (i < argc);

  ಯದಿ (cwd_n_used)
    {
      sort_files ();
      ಯದಿ (!immediate_dirs)
        extract_dirs_from_files (ರಿಕ್ತ, true);
      /* 'cwd_n_used' might be zero now.  */
    }

  /* In the following if/else blocks, it is sufficient to test 'pending_dirs'
     (and not pending_dirs->name) because there may be no markers in the queue
     at this point.  A marker may be enqueued when extract_dirs_from_files is
     called with a non-empty string or via print_dir.  */
  ಯದಿ (cwd_n_used)
    {
      print_current_files ();
      ಯದಿ (pending_dirs)
        dired_outbyte ('\n');
    }
  ಅನ್ಯಥಾ ಯದಿ (n_files <= 1 && pending_dirs && !pending_dirs->next)
    print_dir_name = false;

  ಯಾವತ್ (pending_dirs)
    {
      thispend = pending_dirs;
      pending_dirs = pending_dirs->next;

      ಯದಿ (LOOP_DETECT)
        {
          ಯದಿ (thispend->name == ರಿಕ್ತ)
            {
              /* thispend->name == NULL means this is a marker entry
                 indicating we've finished processing the directory.
                 Use its dev/ino numbers to remove the corresponding
                 entry from the active_dir_set hash table.  */
              ಸಂರಚನಾ dev_ino di = dev_ino_ತೆಗೆ ();
              ಸಂರಚನಾ dev_ino *found = hash_remove (active_dir_set, &di);
              ಯದಿ (false)
                assert_matching_dev_ino (thispend->realname, di);
              affirm (found);
              dev_ino_free (found);
              free_pending_ent (thispend);
              ಅನುವರ್ತ;
            }
        }

      print_dir (thispend->name, thispend->realname,
                 thispend->command_ಸಾಲು_arg);

      free_pending_ent (thispend);
      print_dir_name = true;
    }

  ಯದಿ (print_with_color && used_color)
    {

      /* Skip the restore when it would be a no-op, i.e.,
         when left is "\033[" and right is "m".  */
      ಯದಿ (!(color_indicator[C_ಎಡ].len == 2
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
      ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = stop_signal_ಎಣಿಕೆ; j; j--)
        ಉತ್ಥಾಪಯ (SIGSTOP);
      ಪೂರ್ಣಾಙ್ಕ j = interrupt_signal;
      ಯದಿ (j)
        ಉತ್ಥಾಪಯ (j);
    }

  ಯದಿ (dired)
    {
      /* No need to free these since we're about to exit.  */
      dired_dump_obstack ("//DIRED//", &dired_obstack);
      dired_dump_obstack ("//SUBDIRED//", &subdired_obstack);
      printf ("//DIRED-OPTIONS// --quoting-style=%s\n",
              quoting_style_args[get_quoting_style (filename_quoting_options)]);
    }

  ಯದಿ (LOOP_DETECT)
    {
      assure (hash_get_n_entries (active_dir_set) == 0);
      hash_free (active_dir_set);
    }

  ಪ್ರತ್ಯಾಗಮ exit_status;
}

/* Return the line length indicated by the value given by SPEC, or -1
   if unsuccessful.  0 means no limit on line length.  */

ಸ್ಥಿರ ptrdiff_t
decode_ಸಾಲು_length (ವರ್ಣ ಧ್ರುವ *spec)
{
  uintmax_t val;

  /* Treat too-large values as if they were 0, which is
     effectively infinity.  */
  ವಿಕಲ್ಪ (xstrtoumax (spec, ರಿಕ್ತ, 0, &val, ""))
    {
    ಪಕ್ಷ LONGINT_OK:
      ಪ್ರತ್ಯಾಗಮ val <= MIN (PTRDIFF_MAX, SIZE_MAX) ? val : 0;

    ಪಕ್ಷ LONGINT_OVERFLOW:
      ಪ್ರತ್ಯಾಗಮ 0;

    ಪಕ್ಷ LONGINT_INVALID:
    ಪಕ್ಷ LONGINT_INVALID_SUFFIX_CHAR:
    ಪಕ್ಷ LONGINT_INVALID_SUFFIX_CHAR_WITH_OVERFLOW:
      ಪ್ರತ್ಯಾಗಮ -1;

    ಸ್ವಭಾವ:
      unreachable ();
    }
}

/* Return true if standard output is a tty, caching the result.  */

ಸ್ಥಿರ bool
stdout_isatty (ಶೂನ್ಯ)
{
  ಸ್ಥಿರ signed ವರ್ಣ out_tty = -1;
  ಯದಿ (out_tty < 0)
    out_tty = isatty (STDOUT_FILENO);
  assume (out_tty == 0 || out_tty == 1);
  ಪ್ರತ್ಯಾಗಮ out_tty;
}

/* Set all the option flags according to the switches specified.
   Return the index of the first non-option argument.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
decode_switches (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  ವರ್ಣ *time_style_option = ರಿಕ್ತ;

  /* These variables are false or -1 unless a switch says otherwise.  */
  bool kibibytes_specified = false;
  ಪೂರ್ಣಾಙ್ಕ format_opt = -1;
  ಪೂರ್ಣಾಙ್ಕ hide_control_chars_opt = -1;
  ಪೂರ್ಣಾಙ್ಕ quoting_style_opt = -1;
  ಪೂರ್ಣಾಙ್ಕ sort_opt = -1;
  ptrdiff_t tabsize_opt = -1;
  ptrdiff_t width_opt = -1;

  ಯಾವತ್ (true)
    {
      ಪೂರ್ಣಾಙ್ಕ oi = -1;
      ಪೂರ್ಣಾಙ್ಕ c = getopt_long (argc, argv,
                           "abcdfghiklmnopqrstuvw:xABCDFGHI:LNQRST:UXZ1",
                           long_options, &oi);
      ಯದಿ (c == -1)
        ಭಙ್ಗ;

      ವಿಕಲ್ಪ (c)
        {
        ಪಕ್ಷ 'a':
          ignore_mode = IGNORE_MINIMAL;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'b':
          quoting_style_opt = escape_quoting_style;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'c':
          time_type = time_ctime;
          explicit_time = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'd':
          immediate_dirs = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'f':
          ignore_mode = IGNORE_MINIMAL; /* enable -a */
          sort_opt = sort_none;         /* enable -U */
          ಭಙ್ಗ;

        ಪಕ್ಷ FILE_TYPE_INDICATOR_OPTION: /* --file-type */
          indicator_style = file_type;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'g':
          format_opt = long_format;
          print_owner = false;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'h':
          file_human_output_opts = human_output_opts =
            human_autoscale | human_SI | human_base_1024;
          file_output_block_size = output_block_size = 1;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'i':
          print_inode = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'k':
          kibibytes_specified = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'l':
          format_opt = long_format;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'm':
          format_opt = with_commas;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'n':
          numeric_ids = true;
          format_opt = long_format;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'o':  /* Just like -l, but don't display group info.  */
          format_opt = long_format;
          print_group = false;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'p':
          indicator_style = slash;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'q':
          hide_control_chars_opt = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'r':
          sort_reverse = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 's':
          print_block_size = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 't':
          sort_opt = sort_time;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'u':
          time_type = time_atime;
          explicit_time = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'v':
          sort_opt = sort_version;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'w':
          width_opt = decode_ಸಾಲು_length (optarg);
          ಯದಿ (width_opt < 0)
            error (LS_FAILURE, 0, "%s: %s", _("invalid line width"),
                   quote (optarg));
          ಭಙ್ಗ;

        ಪಕ್ಷ 'x':
          format_opt = horizontal;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'A':
          ignore_mode = IGNORE_DOT_AND_DOTDOT;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'B':
          add_ignore_pattern ("*~");
          add_ignore_pattern (".*~");
          ಭಙ್ಗ;

        ಪಕ್ಷ 'C':
          format_opt = many_per_ಸಾಲು;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'D':
          format_opt = long_format;
          print_hyperlink = false;
          dired = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'F':
          {
            ಪೂರ್ಣಾಙ್ಕ i;
            ಯದಿ (optarg)
              i = XARGMATCH ("--classify", optarg, when_args, when_types);
            ಅನ್ಯಥಾ
              /* Using --classify with no argument is equivalent to using
                 --classify=always.  */
              i = when_always;

            ಯದಿ (i == when_always || (i == when_if_tty && stdout_isatty ()))
              indicator_style = classify;
            ಭಙ್ಗ;
          }

        ಪಕ್ಷ 'G':		/* inhibit display of group info */
          print_group = false;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'H':
          dereference = DEREF_COMMAND_ಸಾಲು_ARGUMENTS;
          ಭಙ್ಗ;

        ಪಕ್ಷ DEREFERENCE_COMMAND_ಸಾಲು_SYMLINK_TO_DIR_OPTION:
          dereference = DEREF_COMMAND_ಸಾಲು_SYMLINK_TO_DIR;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'I':
          add_ignore_pattern (optarg);
          ಭಙ್ಗ;

        ಪಕ್ಷ 'L':
          dereference = DEREF_ALWAYS;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'N':
          quoting_style_opt = literal_quoting_style;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'Q':
          quoting_style_opt = c_quoting_style;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'R':
          recursive = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'S':
          sort_opt = sort_size;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'T':
          tabsize_opt = xnumtoumax (optarg, 0, 0, MIN (PTRDIFF_MAX, SIZE_MAX),
                                    "", _("invalid tab size"), LS_FAILURE, 0);
          ಭಙ್ಗ;

        ಪಕ್ಷ 'U':
          sort_opt = sort_none;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'X':
          sort_opt = sort_extension;
          ಭಙ್ಗ;

        ಪಕ್ಷ '1':
          /* -1 has no effect after -l.  */
          ಯದಿ (format_opt != long_format)
            format_opt = one_per_ಸಾಲು;
          ಭಙ್ಗ;

        ಪಕ್ಷ AUTHOR_OPTION:
          print_author = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ HIDE_OPTION:
          {
            ಸಂರಚನಾ ignore_pattern *hide = xmalloc (ಪರಿಮಾಣ *hide);
            hide->pattern = optarg;
            hide->next = hide_patterns;
            hide_patterns = hide;
          }
          ಭಙ್ಗ;

        ಪಕ್ಷ SORT_OPTION:
          sort_opt = XARGMATCH ("--sort", optarg, sort_args, sort_types);
          ಭಙ್ಗ;

        ಪಕ್ಷ GROUP_DIRECTORIES_FIRST_OPTION:
          directories_first = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ TIME_OPTION:
          time_type = XARGMATCH ("--time", optarg, time_args, time_types);
          explicit_time = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ FORMAT_OPTION:
          format_opt = XARGMATCH ("--format", optarg, format_args,
                                  format_types);
          ಭಙ್ಗ;

        ಪಕ್ಷ FULL_TIME_OPTION:
          format_opt = long_format;
          time_style_option = (ವರ್ಣ *) "full-iso";
          ಭಙ್ಗ;

        ಪಕ್ಷ COLOR_OPTION:
          {
            ಪೂರ್ಣಾಙ್ಕ i;
            ಯದಿ (optarg)
              i = XARGMATCH ("--color", optarg, when_args, when_types);
            ಅನ್ಯಥಾ
              /* Using --color with no argument is equivalent to using
                 --color=always.  */
              i = when_always;

            print_with_color = (i == when_always
                                || (i == when_if_tty && stdout_isatty ()));
            ಭಙ್ಗ;
          }

        ಪಕ್ಷ HYPERLINK_OPTION:
          {
            ಪೂರ್ಣಾಙ್ಕ i;
            ಯದಿ (optarg)
              i = XARGMATCH ("--hyperlink", optarg, when_args, when_types);
            ಅನ್ಯಥಾ
              /* Using --hyperlink with no argument is equivalent to using
                 --hyperlink=always.  */
              i = when_always;

            print_hyperlink = (i == when_always
                               || (i == when_if_tty && stdout_isatty ()));
            ಭಙ್ಗ;
          }

        ಪಕ್ಷ INDICATOR_STYLE_OPTION:
          indicator_style = XARGMATCH ("--indicator-style", optarg,
                                       indicator_style_args,
                                       indicator_style_types);
          ಭಙ್ಗ;

        ಪಕ್ಷ QUOTING_STYLE_OPTION:
          quoting_style_opt = XARGMATCH ("--quoting-style", optarg,
                                         quoting_style_args,
                                         quoting_style_vals);
          ಭಙ್ಗ;

        ಪಕ್ಷ TIME_STYLE_OPTION:
          time_style_option = optarg;
          ಭಙ್ಗ;

        ಪಕ್ಷ SHOW_CONTROL_CHARS_OPTION:
          hide_control_chars_opt = false;
          ಭಙ್ಗ;

        ಪಕ್ಷ BLOCK_SIZE_OPTION:
          {
            ಗಣನಾ strtol_error e = human_options (optarg, &human_output_opts,
                                                 &output_block_size);
            ಯದಿ (e != LONGINT_OK)
              xstrtol_fatal (e, oi, 0, long_options, optarg);
            file_human_output_opts = human_output_opts;
            file_output_block_size = output_block_size;
          }
          ಭಙ್ಗ;

        ಪಕ್ಷ SI_OPTION:
          file_human_output_opts = human_output_opts =
            human_autoscale | human_SI;
          file_output_block_size = output_block_size = 1;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'Z':
          print_scontext = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ ZERO_OPTION:
          eolbyte = 0;
          hide_control_chars_opt = false;
          ಯದಿ (format_opt != long_format)
            format_opt = one_per_ಸಾಲು;
          print_with_color = false;
          quoting_style_opt = literal_quoting_style;
          ಭಙ್ಗ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        ಸ್ವಭಾವ:
          usage (LS_FAILURE);
        }
    }

  ಯದಿ (! output_block_size)
    {
      ವರ್ಣ ಧ್ರುವ *ls_block_size = getenv ("LS_BLOCK_SIZE");
      human_options (ls_block_size,
                     &human_output_opts, &output_block_size);
      ಯದಿ (ls_block_size || getenv ("BLOCK_SIZE"))
        {
          file_human_output_opts = human_output_opts;
          file_output_block_size = output_block_size;
        }
      ಯದಿ (kibibytes_specified)
        {
          human_output_opts = 0;
          output_block_size = 1024;
        }
    }

  format = (0 <= format_opt ? format_opt
            : ls_mode == LS_LS ? (stdout_isatty ()
                                  ? many_per_ಸಾಲು : one_per_ಸಾಲು)
            : ls_mode == LS_MULTI_COL ? many_per_ಸಾಲು
            : /* ls_mode == LS_LONG_FORMAT */ long_format);

  /* If the line length was not set by a switch but is needed to determine
     output, go to the work of obtaining it from the environment.  */
  ptrdiff_t linelen = width_opt;
  ಯದಿ (format == many_per_ಸಾಲು || format == horizontal || format == with_commas
      || print_with_color)
    {
#ifdef TIOCGWINSZ
      ಯದಿ (linelen < 0)
        {
          ಸಂರಚನಾ winsize ws;
          ಯದಿ (stdout_isatty ()
              && 0 <= ioctl (STDOUT_FILENO, TIOCGWINSZ, &ws)
              && 0 < ws.ws_col)
            linelen = ws.ws_col <= MIN (PTRDIFF_MAX, SIZE_MAX) ? ws.ws_col : 0;
        }
#endif
      ಯದಿ (linelen < 0)
        {
          ವರ್ಣ ಧ್ರುವ *p = getenv ("COLUMNS");
          ಯದಿ (p && *p)
            {
              linelen = decode_ಸಾಲು_length (p);
              ಯದಿ (linelen < 0)
                error (0, 0,
                       _("ignoring invalid width"
                         " in environment variable COLUMNS: %s"),
                       quote (p));
            }
        }
    }

  ಸಾಲು_length = linelen < 0 ? 80 : linelen;

  /* Determine the max possible number of display columns.  */
  max_idx = ಸಾಲು_length / MIN_COLUMN_WIDTH;
  /* Account for first display column not having a separator,
     or line_lengths shorter than MIN_COLUMN_WIDTH.  */
  max_idx += ಸಾಲು_length % MIN_COLUMN_WIDTH != 0;

  ಯದಿ (format == many_per_ಸಾಲು || format == horizontal || format == with_commas)
    {
      ಯದಿ (0 <= tabsize_opt)
        tabsize = tabsize_opt;
      ಅನ್ಯಥಾ
        {
          tabsize = 8;
          ವರ್ಣ ಧ್ರುವ *p = getenv ("TABSIZE");
          ಯದಿ (p)
            {
              uintmax_t tmp;
              ಯದಿ (xstrtoumax (p, ರಿಕ್ತ, 0, &tmp, "") == LONGINT_OK
                  && tmp <= SIZE_MAX)
                tabsize = tmp;
              ಅನ್ಯಥಾ
                error (0, 0,
                       _("ignoring invalid tab size"
                         " in environment variable TABSIZE: %s"),
                       quote (p));
            }
        }
    }

  qmark_funny_chars = (hide_control_chars_opt < 0
                       ? ls_mode == LS_LS && stdout_isatty ()
                       : hide_control_chars_opt);

  qmark_newline = hide_control_chars_opt != false && eolbyte == '\n'
                  && ((ಸಾಲು_length
                       && (format == with_commas
                           || format == many_per_ಸಾಲು || format == horizontal))
                      || format == one_per_ಸಾಲು || format == long_format);

  ಪೂರ್ಣಾಙ್ಕ qs = quoting_style_opt;
  ಯದಿ (qs < 0)
    qs = getenv_quoting_style ();
  ಯದಿ (qs < 0)
    qs = (ls_mode == LS_LS
          ? (stdout_isatty () ? shell_escape_quoting_style : -1)
          : escape_quoting_style);
  ಯದಿ (0 <= qs)
    set_quoting_style (ರಿಕ್ತ, qs);
  qs = get_quoting_style (ರಿಕ್ತ);
  align_variable_outer_quotes
    = ((format == long_format
        || ((format == many_per_ಸಾಲು || format == horizontal) && ಸಾಲು_length))
       && (qs == shell_quoting_style
           || qs == shell_escape_quoting_style
           || qs == c_maybe_quoting_style));
  filename_quoting_options = clone_quoting_options (ರಿಕ್ತ);
  ಯದಿ (qs == escape_quoting_style)
    set_char_quoting (filename_quoting_options, ' ', 1);
  ಯದಿ (file_type <= indicator_style)
    {
      ಪ್ರತಿ (ವರ್ಣ ಧ್ರುವ *p = &"*=>@|"[indicator_style - file_type]; *p; p++)
        set_char_quoting (filename_quoting_options, *p, 1);
    }
  ಯದಿ (format == with_commas
      && (qs == shell_quoting_style
          || qs == shell_escape_quoting_style
          || qs == c_maybe_quoting_style
          || qs == escape_quoting_style))
    set_char_quoting (filename_quoting_options, ',', 1);

  dirname_quoting_options = clone_quoting_options (ರಿಕ್ತ);
  set_char_quoting (dirname_quoting_options, ':', 1);

  /* --dired implies --format=long (-l) and sans --hyperlink.
     So ignore it if those overridden.  */
  dired &= (format == long_format) & !print_hyperlink;

  ಯದಿ (eolbyte < dired)
    error (LS_FAILURE, 0, _("--dired and --zero are incompatible"));

  /* If a time type is explicitly specified (with -c, -u, or --time=)
     and we're not showing a time (-l not specified), then sort by that time,
     rather than by name.  Note this behavior is unspecified by POSIX.  */

  sort_type = (0 <= sort_opt ? sort_opt
               : (format != long_format && explicit_time)
               ? sort_time : sort_name);

  ಯದಿ (format == long_format)
    {
      ವರ್ಣ *envstyle = ರಿಕ್ತ;
      ವರ್ಣ *style = time_style_option;
      ಯದಿ (! style)
        style = envstyle = getenv ("TIME_STYLE");

      ಯದಿ (! style)
        style = (ವರ್ಣ *) "locale";
      ಅನ್ಯಥಾ
        {
          ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ posix_prefix[] = "posix-";
          ಯಾವತ್ (STREQ_LEN (style, posix_prefix, ಪರಿಮಾಣ posix_prefix - 1))
            {
              ಯದಿ (! hard_locale (LC_TIME))
                ಪ್ರತ್ಯಾಗಮ optind;
              style += ಪರಿಮಾಣ posix_prefix - 1;
            }
        }

      ಯದಿ (*style == '+')
        {
          ವರ್ಣ *p0 = style + 1;
          ವರ್ಣ *p0nl = strchr (p0, '\n');
          ವರ್ಣ ಧ್ರುವ *p1 = p0;
          ಯದಿ (p0nl)
            {
              p1 = p0nl + 1;
              ಯದಿ (strchr (p1, '\n'))
                error (LS_FAILURE, 0, _("invalid time style format %s"),
                       quote (p0));
              ಯದಿ (envstyle)
                p0 = xmemdup0 (p0, p0nl - p0);
              ಅನ್ಯಥಾ
                *p0nl = '\0';
            }
          long_time_format[0] = p0;
          long_time_format[1] = p1;
        }
      ಅನ್ಯಥಾ
        {
          ವಿಕಲ್ಪ (x_timestyle_match (style, /*allow_posix=*/ true,
                                     time_style_args,
                                     (ವರ್ಣ ಧ್ರುವ *) time_style_types,
                                     ಪರಿಮಾಣ (*time_style_types), LS_FAILURE))
            {
            ಪಕ್ಷ full_iso_time_style:
              long_time_format[0] = long_time_format[1] =
                "%Y-%m-%d %H:%M:%S.%N %z";
              ಭಙ್ಗ;

            ಪಕ್ಷ long_iso_time_style:
              long_time_format[0] = long_time_format[1] = "%Y-%m-%d %H:%M";
              ಭಙ್ಗ;

            ಪಕ್ಷ iso_time_style:
              long_time_format[0] = "%Y-%m-%d ";
              long_time_format[1] = "%m-%d %H:%M";
              ಭಙ್ಗ;

            ಪಕ್ಷ locale_time_style:
              ಯದಿ (hard_locale (LC_TIME))
                {
                  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < 2; i++)
                    long_time_format[i] =
                      dcgettext (ರಿಕ್ತ, long_time_format[i], LC_TIME);
                }
            }
        }

      abformat_init ();
    }

  ಪ್ರತ್ಯಾಗಮ optind;
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
get_funky_string (ವರ್ಣ **dest, ವರ್ಣ ಧ್ರುವ **src, bool equals_end,
                  size_t *output_ಎಣಿಕೆ)
{
  ವರ್ಣ ಅಂಕೆ;			/* For numerical codes */
  size_t ಎಣಿಕೆ;			/* Something to count with */
  ಗಣನಾ {
    ST_GND, ST_BACKSLASH, ST_OCTAL, ST_HEX, ST_CARET, ST_END, ST_ERROR
  } state;
  ವರ್ಣ ಧ್ರುವ *p;
  ವರ್ಣ *q;

  p = *src;			/* We don't want to double-indirect */
  q = *dest;			/* the whole darn time.  */

  ಎಣಿಕೆ = 0;			/* No characters counted in yet.  */
  ಅಂಕೆ = 0;

  state = ST_GND;		/* Start in ground state.  */
  ಯಾವತ್ (state < ST_END)
    {
      ವಿಕಲ್ಪ (state)
        {
        ಪಕ್ಷ ST_GND:		/* Ground state (no escapes) */
          ವಿಕಲ್ಪ (*p)
            {
            ಪಕ್ಷ ':':
            ಪಕ್ಷ '\0':
              state = ST_END;	/* End of string */
              ಭಙ್ಗ;
            ಪಕ್ಷ '\\':
              state = ST_BACKSLASH; /* Backslash escape sequence */
              ++p;
              ಭಙ್ಗ;
            ಪಕ್ಷ '^':
              state = ST_CARET; /* Caret escape */
              ++p;
              ಭಙ್ಗ;
            ಪಕ್ಷ '=':
              ಯದಿ (equals_end)
                {
                  state = ST_END; /* End */
                  ಭಙ್ಗ;
                }
              FALLTHROUGH;
            ಸ್ವಭಾವ:
              *(q++) = *(p++);
              ++ಎಣಿಕೆ;
              ಭಙ್ಗ;
            }
          ಭಙ್ಗ;

        ಪಕ್ಷ ST_BACKSLASH:	/* Backslash escaped character */
          ವಿಕಲ್ಪ (*p)
            {
            ಪಕ್ಷ '0':
            ಪಕ್ಷ '1':
            ಪಕ್ಷ '2':
            ಪಕ್ಷ '3':
            ಪಕ್ಷ '4':
            ಪಕ್ಷ '5':
            ಪಕ್ಷ '6':
            ಪಕ್ಷ '7':
              state = ST_OCTAL;	/* Octal sequence */
              ಅಂಕೆ = *p - '0';
              ಭಙ್ಗ;
            ಪಕ್ಷ 'x':
            ಪಕ್ಷ 'X':
              state = ST_HEX;	/* Hex sequence */
              ಅಂಕೆ = 0;
              ಭಙ್ಗ;
            ಪಕ್ಷ 'a':		/* Bell */
              ಅಂಕೆ = '\a';
              ಭಙ್ಗ;
            ಪಕ್ಷ 'b':		/* Backspace */
              ಅಂಕೆ = '\b';
              ಭಙ್ಗ;
            ಪಕ್ಷ 'e':		/* Escape */
              ಅಂಕೆ = 27;
              ಭಙ್ಗ;
            ಪಕ್ಷ 'f':		/* Form feed */
              ಅಂಕೆ = '\f';
              ಭಙ್ಗ;
            ಪಕ್ಷ 'n':		/* Newline */
              ಅಂಕೆ = '\n';
              ಭಙ್ಗ;
            ಪಕ್ಷ 'r':		/* Carriage return */
              ಅಂಕೆ = '\r';
              ಭಙ್ಗ;
            ಪಕ್ಷ 't':		/* Tab */
              ಅಂಕೆ = '\t';
              ಭಙ್ಗ;
            ಪಕ್ಷ 'v':		/* Vtab */
              ಅಂಕೆ = '\v';
              ಭಙ್ಗ;
            ಪಕ್ಷ '?':		/* Delete */
              ಅಂಕೆ = 127;
              ಭಙ್ಗ;
            ಪಕ್ಷ '_':		/* Space */
              ಅಂಕೆ = ' ';
              ಭಙ್ಗ;
            ಪಕ್ಷ '\0':		/* End of string */
              state = ST_ERROR;	/* Error! */
              ಭಙ್ಗ;
            ಸ್ವಭಾವ:		/* Escaped character like \ ^ : = */
              ಅಂಕೆ = *p;
              ಭಙ್ಗ;
            }
          ಯದಿ (state == ST_BACKSLASH)
            {
              *(q++) = ಅಂಕೆ;
              ++ಎಣಿಕೆ;
              state = ST_GND;
            }
          ++p;
          ಭಙ್ಗ;

        ಪಕ್ಷ ST_OCTAL:		/* Octal sequence */
          ಯದಿ (*p < '0' || *p > '7')
            {
              *(q++) = ಅಂಕೆ;
              ++ಎಣಿಕೆ;
              state = ST_GND;
            }
          ಅನ್ಯಥಾ
            ಅಂಕೆ = (ಅಂಕೆ << 3) + (*(p++) - '0');
          ಭಙ್ಗ;

        ಪಕ್ಷ ST_HEX:		/* Hex sequence */
          ವಿಕಲ್ಪ (*p)
            {
            ಪಕ್ಷ '0':
            ಪಕ್ಷ '1':
            ಪಕ್ಷ '2':
            ಪಕ್ಷ '3':
            ಪಕ್ಷ '4':
            ಪಕ್ಷ '5':
            ಪಕ್ಷ '6':
            ಪಕ್ಷ '7':
            ಪಕ್ಷ '8':
            ಪಕ್ಷ '9':
              ಅಂಕೆ = (ಅಂಕೆ << 4) + (*(p++) - '0');
              ಭಙ್ಗ;
            ಪಕ್ಷ 'a':
            ಪಕ್ಷ 'b':
            ಪಕ್ಷ 'c':
            ಪಕ್ಷ 'd':
            ಪಕ್ಷ 'e':
            ಪಕ್ಷ 'f':
              ಅಂಕೆ = (ಅಂಕೆ << 4) + (*(p++) - 'a') + 10;
              ಭಙ್ಗ;
            ಪಕ್ಷ 'A':
            ಪಕ್ಷ 'B':
            ಪಕ್ಷ 'C':
            ಪಕ್ಷ 'D':
            ಪಕ್ಷ 'E':
            ಪಕ್ಷ 'F':
              ಅಂಕೆ = (ಅಂಕೆ << 4) + (*(p++) - 'A') + 10;
              ಭಙ್ಗ;
            ಸ್ವಭಾವ:
              *(q++) = ಅಂಕೆ;
              ++ಎಣಿಕೆ;
              state = ST_GND;
              ಭಙ್ಗ;
            }
          ಭಙ್ಗ;

        ಪಕ್ಷ ST_CARET:		/* Caret escape */
          state = ST_GND;	/* Should be the next state... */
          ಯದಿ (*p >= '@' && *p <= '~')
            {
              *(q++) = *(p++) & 037;
              ++ಎಣಿಕೆ;
            }
          ಅನ್ಯಥಾ ಯದಿ (*p == '?')
            {
              *(q++) = 127;
              ++ಎಣಿಕೆ;
            }
          ಅನ್ಯಥಾ
            state = ST_ERROR;
          ಭಙ್ಗ;

        ಪಕ್ಷ ST_END: ಪಕ್ಷ ST_ERROR: ಸ್ವಭಾವ:
          affirm (false);
        }
    }

  *dest = q;
  *src = p;
  *output_ಎಣಿಕೆ = ಎಣಿಕೆ;

  ಪ್ರತ್ಯಾಗಮ state != ST_ERROR;
}

ಗಣನಾ parse_state
  {
    PS_START = 1,
    PS_2,
    PS_3,
    PS_4,
    PS_DONE,
    PS_FAIL
  };


/* Check if the content of TERM is a valid name in dircolors.  */

ಸ್ಥಿರ bool
known_term_type (ಶೂನ್ಯ)
{
  ವರ್ಣ ಧ್ರುವ *term = getenv ("TERM");
  ಯದಿ (! term || ! *term)
    ಪ್ರತ್ಯಾಗಮ false;

  ವರ್ಣ ಧ್ರುವ *ಸಾಲು = G_ಸಾಲು;
  ಯಾವತ್ (ಸಾಲು - G_ಸಾಲು < ಪರಿಮಾಣ (G_ಸಾಲು))
    {
      ಯದಿ (STRNCMP_LIT (ಸಾಲು, "TERM ") == 0)
        {
          ಯದಿ (fnmatch (ಸಾಲು + 5, term, 0) == 0)
            ಪ್ರತ್ಯಾಗಮ true;
        }
      ಸಾಲು += strlen (ಸಾಲು) + 1;
    }

  ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ ಶೂನ್ಯ
parse_ls_color (ಶೂನ್ಯ)
{
  ವರ್ಣ ಧ್ರುವ *p;		/* Pointer to character being parsed */
  ವರ್ಣ *buf;			/* color_buf buffer pointer */
  ವರ್ಣ label0, label1;		/* Indicator label */
  ಸಂರಚನಾ color_ext_type *ext;	/* Extension we are working on */

  ಯದಿ ((p = getenv ("LS_COLORS")) == ರಿಕ್ತ || *p == '\0')
    {
      /* LS_COLORS takes precedence, but if that's not set then
         honor the COLORTERM and TERM env variables so that
         we only go with the internal ANSI color codes if the
         former is non empty or the latter is set to a known value.  */
      ವರ್ಣ ಧ್ರುವ *colorterm = getenv ("COLORTERM");
      ಯದಿ (! (colorterm && *colorterm) && ! known_term_type ())
        print_with_color = false;
      ಪ್ರತ್ಯಾಗಮ;
    }

  ext = ರಿಕ್ತ;

  /* This is an overly conservative estimate, but any possible
     LS_COLORS string will *not* generate a color_buf longer than
     itself, so it is a safe way of allocating a buffer in
     advance.  */
  buf = color_buf = xstrdup (p);

  ಗಣನಾ parse_state state = PS_START;
  ಯಾವತ್ (true)
    {
      ವಿಕಲ್ಪ (state)
        {
        ಪಕ್ಷ PS_START:		/* First label character */
          ವಿಕಲ್ಪ (*p)
            {
            ಪಕ್ಷ ':':
              ++p;
              ಭಙ್ಗ;

            ಪಕ್ಷ '*':
              /* Allocate new extension block and add to head of
                 linked list (this way a later definition will
                 override an earlier one, which can be useful for
                 having terminal-specific defs override global).  */

              ext = xmalloc (ಪರಿಮಾಣ *ext);
              ext->next = color_ext_list;
              color_ext_list = ext;
              ext->exact_match = false;

              ++p;
              ext->ext.string = buf;

              state = (get_funky_string (&buf, &p, true, &ext->ext.len)
                       ? PS_4 : PS_FAIL);
              ಭಙ್ಗ;

            ಪಕ್ಷ '\0':
              state = PS_DONE;	/* Done! */
              ಗಚ್ಛ done;

            ಸ್ವಭಾವ:	/* Assume it is file type label */
              label0 = *p++;
              state = PS_2;
              ಭಙ್ಗ;
            }
          ಭಙ್ಗ;

        ಪಕ್ಷ PS_2:		/* Second label character */
          ಯದಿ (*p)
            {
              label1 = *p++;
              state = PS_3;
            }
          ಅನ್ಯಥಾ
            state = PS_FAIL;	/* Error */
          ಭಙ್ಗ;

        ಪಕ್ಷ PS_3:		/* Equal sign after indicator label */
          state = PS_FAIL;	/* Assume failure...  */
          ಯದಿ (*(p++) == '=')/* It *should* be...  */
            {
              ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < countof (indicator_name); i++)
                {
                  ಯದಿ ((label0 == indicator_name[i][0])
                      && (label1 == indicator_name[i][1]))
                    {
                      color_indicator[i].string = buf;
                      state = (get_funky_string (&buf, &p, false,
                                                 &color_indicator[i].len)
                               ? PS_START : PS_FAIL);
                      ಭಙ್ಗ;
                    }
                }
              ಯದಿ (state == PS_FAIL)
                error (0, 0, _("unrecognized prefix: %s"),
                       quote ((ವರ್ಣ []) {label0, label1, '\0'}));
            }
          ಭಙ್ಗ;

        ಪಕ್ಷ PS_4:		/* Equal sign after *.ext */
          ಯದಿ (*(p++) == '=')
            {
              ext->seq.string = buf;
              state = (get_funky_string (&buf, &p, false, &ext->seq.len)
                       ? PS_START : PS_FAIL);
            }
          ಅನ್ಯಥಾ
            state = PS_FAIL;
          ಭಙ್ಗ;

        ಪಕ್ಷ PS_FAIL:
          ಗಚ್ಛ done;

        ಪಕ್ಷ PS_DONE: ಸ್ವಭಾವ:
          affirm (false);
        }
    }
 done:

  ಯದಿ (state == PS_FAIL)
    {
      error (0, 0,
             _("unparsable value for LS_COLORS environment variable"));
      ಮುಕ್ತ (color_buf);
      ಪ್ರತಿ (ಸಂರಚನಾ color_ext_type *e = color_ext_list; e != ರಿಕ್ತ;
           /* empty */)
        {
          ಸಂರಚನಾ color_ext_type *e2 = e;
          e = e->next;
          ಮುಕ್ತ (e2);
        }
      print_with_color = false;
      ಪ್ರತ್ಯಾಗಮ;
    }
  ಅನ್ಯಥಾ
    {
      /* Postprocess list to set EXACT_MATCH on entries where there are
         different cased extensions with separate sequences defined.
         Also set ext.len to SIZE_MAX on any entries that can't
         match due to precedence, to avoid redundant string compares.  */
      ಪ್ರತಿ (ಸಂರಚನಾ color_ext_type *e1 = color_ext_list; e1 != ರಿಕ್ತ;
           e1 = e1->next)
        {
          bool case_ignored = false;

          ಪ್ರತಿ (ಸಂರಚನಾ color_ext_type *e2 = e1->next; e2 != ರಿಕ್ತ;
               e2 = e2->next)
            {
              ಯದಿ (e2->ext.len < SIZE_MAX && e1->ext.len == e2->ext.len)
                {
                  ಯದಿ (memeq (e1->ext.string, e2->ext.string, e1->ext.len))
                    e2->ext.len = SIZE_MAX; /* Ignore */
                  ಅನ್ಯಥಾ ಯದಿ (c_strncasecmp (e1->ext.string, e2->ext.string,
                                          e1->ext.len) == 0)
                    {
                      ಯದಿ (case_ignored)
                        {
                          e2->ext.len = SIZE_MAX; /* Ignore */
                        }
                      ಅನ್ಯಥಾ ಯದಿ (e1->seq.len == e2->seq.len
                               && memeq (e1->seq.string, e2->seq.string,
                                         e1->seq.len))
                        {
                          e2->ext.len = SIZE_MAX; /* Ignore */
                          case_ignored = true;    /* Ignore all subsequent */
                        }
                      ಅನ್ಯಥಾ
                        {
                          e1->exact_match = true;
                          e2->exact_match = true;
                        }
                    }
                }
            }
        }
    }

  ಯದಿ (color_indicator[C_LINK].len == 6
      && !STRNCMP_LIT (color_indicator[C_LINK].string, "target"))
    color_symlink_as_referent = true;
}

/* Set the exit status to report a failure.  If SERIOUS, it is a
   serious failure; otherwise, it is merely a minor problem.  */

ಸ್ಥಿರ ಶೂನ್ಯ
set_exit_status (bool serious)
{
  ಯದಿ (serious)
    exit_status = LS_FAILURE;
  ಅನ್ಯಥಾ ಯದಿ (exit_status == EXIT_SUCCESS)
    exit_status = LS_MINOR_PROBLEM;
}

/* Assuming a failure is serious if SERIOUS, use the printf-style
   MESSAGE to report the failure to access a file named FILE.  Assume
   errno is set appropriately for the failure.  */

ಸ್ಥಿರ ಶೂನ್ಯ
file_failure (bool serious, ವರ್ಣ ಧ್ರುವ *message, ವರ್ಣ ಧ್ರುವ *file)
{
  error (0, errno, message, quoteaf (file));
  set_exit_status (serious);
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
queue_directory (ವರ್ಣ ಧ್ರುವ *name, ವರ್ಣ ಧ್ರುವ *realname, bool command_ಸಾಲು_arg)
{
  ಸಂರಚನಾ pending *new = xmalloc (ಪರಿಮಾಣ *new);
  new->realname = realname ? xstrdup (realname) : ರಿಕ್ತ;
  new->name = name ? xstrdup (name) : ರಿಕ್ತ;
  new->command_ಸಾಲು_arg = command_ಸಾಲು_arg;
  new->next = pending_dirs;
  pending_dirs = new;
}

/* Read directory NAME, and list the files in it.
   If REALNAME is nonzero, print its name instead of NAME;
   this is used for symbolic links to directories.
   COMMAND_LINE_ARG means this directory was mentioned on the command line.  */

ಸ್ಥಿರ ಶೂನ್ಯ
print_dir (ವರ್ಣ ಧ್ರುವ *name, ವರ್ಣ ಧ್ರುವ *realname, bool command_ಸಾಲು_arg)
{
  DIR *dirp;
  ಸಂರಚನಾ dirent *next;
  uintmax_t total_blocks = 0;
  ಸ್ಥಿರ bool first = true;

  errno = 0;
  dirp = opendir (name);
  ಯದಿ (!dirp)
    {
      ಯದಿ (command_ಸಾಲು_arg || ! ignorable_traversal_errno (errno))
        file_failure (command_ಸಾಲು_arg, _("cannot open directory %s"), name);
      ಪ್ರತ್ಯಾಗಮ;
    }

  ಯದಿ (LOOP_DETECT)
    {
      ಸಂರಚನಾ stat dir_stat;
      ಪೂರ್ಣಾಙ್ಕ fd = dirfd (dirp);

      /* If dirfd failed, endure the overhead of stat'ing by path  */
      ಯದಿ ((0 <= fd
           ? fstat_for_ino (fd, &dir_stat)
           : stat_for_ino (name, &dir_stat)) < 0)
        {
          ಯದಿ (! ignorable_traversal_errno (errno))
            file_failure (command_ಸಾಲು_arg,
                          _("cannot determine device and inode of %s"), name);
          closedir (dirp);
          ಪ್ರತ್ಯಾಗಮ;
        }

      /* If we've already visited this dev/inode pair, warn that
         we've found a loop, and do not process this directory.  */
      ಯದಿ (visit_dir (dir_stat.st_dev, dir_stat.st_ino))
        {
          error (0, 0, _("%s: not listing already-listed directory"),
                 quotef (name));
          closedir (dirp);
          set_exit_status (true);
          ಪ್ರತ್ಯಾಗಮ;
        }

      dev_ino_ತಳ್ಳು (dir_stat.st_dev, dir_stat.st_ino);
    }

  clear_files ();

  ಯದಿ (recursive || print_dir_name)
    {
      ಯದಿ (!first)
        dired_outbyte ('\n');
      first = false;
      dired_indent ();

      ವರ್ಣ *absolute_name = ರಿಕ್ತ;
      ಯದಿ (print_hyperlink)
        {
          absolute_name = canonicalize_filename_mode (name, CAN_MISSING);
          ಯದಿ (! absolute_name
              && ! ignorable_traversal_errno (errno))
            file_failure (command_ಸಾಲು_arg,
                          _("error canonicalizing %s"), name);
        }
      quote_name (realname ? realname : name, dirname_quoting_options, -1,
                  ರಿಕ್ತ, true, &subdired_obstack, absolute_name);

      ಮುಕ್ತ (absolute_name);

      dired_outstring (":\n");
    }

  /* Read the directory entries, and insert the subfiles into the 'cwd_file'
     table.  */

  ಯಾವತ್ (true)
    {
      /* Set errno to zero so we can distinguish between a readdir failure
         and when readdir simply finds that there are no more entries.  */
      errno = 0;
      next = readdir (dirp);
      ಯದಿ (next)
        {
          ಯದಿ (! file_ignored (next->d_name))
            {
              ಗಣನಾ filetype type;
#ಯದಿ HAVE_STRUCT_DIRENT_D_TYPE
              type = d_type_filetype[next->d_type];
#ಅನ್ಯಥಾ
              type = unknown;
#endif
              total_blocks += gobble_file (next->d_name, type,
                                           RELIABLE_D_INO (next),
                                           false, name);

              /* In this narrow case, print out each name right away, so
                 ls uses constant memory while processing the entries of
                 this directory.  Useful when there are many (millions)
                 of entries in a directory.  */
              ಯದಿ (format == one_per_ಸಾಲು && sort_type == sort_none
                      && !print_block_size && !recursive)
                {
                  /* We must call sort_files in spite of
                     "sort_type == sort_none" for its initialization
                     of the sorted_file vector.  */
                  sort_files ();
                  print_current_files ();
                  clear_files ();
                }
            }
        }
      ಅನ್ಯಥಾ
        {
          ಪೂರ್ಣಾಙ್ಕ err = errno;
          ಯದಿ (err == 0)
            ಭಙ್ಗ;
          /* Ignore errors indicating that the directory was removed.  */
          ಯದಿ (ignorable_traversal_errno (err))
            ಭಙ್ಗ;
          file_failure (command_ಸಾಲು_arg, _("reading directory %s"), name);
          ಯದಿ (err != EOVERFLOW)
            ಭಙ್ಗ;
        }

      /* When processing a very large directory, and since we've inhibited
         interrupts, this loop would take so long that ls would be annoyingly
         uninterruptible.  This ensures that it handles signals promptly.  */
      process_signals ();
    }

  ಯದಿ (closedir (dirp) != 0)
    {
      file_failure (command_ಸಾಲು_arg, _("closing directory %s"), name);
      /* Don't return; print whatever we got.  */
    }

  /* Sort the directory contents.  */
  sort_files ();

  /* If any member files are subdirectories, perhaps they should have their
     contents listed rather than being mentioned here as files.  */

  ಯದಿ (recursive)
    extract_dirs_from_files (name, false);

  ಯದಿ (format == long_format || print_block_size)
    {
      ವರ್ಣ buf[LONGEST_HUMAN_READABLE + 3];
      ವರ್ಣ *p = human_readable (total_blocks, buf + 1, human_output_opts,
                                ST_NBLOCKSIZE, output_block_size);
      ವರ್ಣ *pend = p + strlen (p);
      *--p = ' ';
      *pend++ = eolbyte;
      dired_indent ();
      dired_outstring (_("total"));
      dired_outbuf (p, pend - p);
    }

  ಯದಿ (cwd_n_used)
    print_current_files ();
}

/* Add 'pattern' to the list of patterns for which files that match are
   not listed.  */

ಸ್ಥಿರ ಶೂನ್ಯ
add_ignore_pattern (ವರ್ಣ ಧ್ರುವ *pattern)
{
  ಸಂರಚನಾ ignore_pattern *ignore;

  ignore = xmalloc (ಪರಿಮಾಣ *ignore);
  ignore->pattern = pattern;
  /* Add it to the head of the linked list.  */
  ignore->next = ignore_patterns;
  ignore_patterns = ignore;
}

/* Return true if one of the PATTERNS matches FILE.  */

ಸ್ಥಿರ bool
patterns_match (ಸಂರಚನಾ ignore_pattern ಧ್ರುವ *patterns, ವರ್ಣ ಧ್ರುವ *file)
{
  ಪ್ರತಿ (ಸಂರಚನಾ ignore_pattern ಧ್ರುವ *p = patterns; p; p = p->next)
    ಯದಿ (fnmatch (p->pattern, file, FNM_PERIOD) == 0)
      ಪ್ರತ್ಯಾಗಮ true;
  ಪ್ರತ್ಯಾಗಮ false;
}

/* Return true if FILE should be ignored.  */

ಸ್ಥಿರ bool
file_ignored (ವರ್ಣ ಧ್ರುವ *name)
{
  ಪ್ರತ್ಯಾಗಮ ((ignore_mode != IGNORE_MINIMAL
           && name[0] == '.'
           && (ignore_mode == IGNORE_DEFAULT || ! name[1 + (name[1] == '.')]))
          || (ignore_mode == IGNORE_DEFAULT
              && patterns_match (hide_patterns, name))
          || patterns_match (ignore_patterns, name));
}

/* POSIX requires that a file size be printed without a sign, even
   when negative.  Assume the typical case where negative sizes are
   actually positive values that have wrapped around.  */

ಸ್ಥಿರ uintmax_t
unsigned_file_size (off_t size)
{
  ಪ್ರತ್ಯಾಗಮ size + (size < 0) * ((uintmax_t) OFF_T_MAX - OFF_T_MIN + 1);
}

#ifdef HAVE_CAP
/* Return true if NAME has a capability (see linux/capability.h) */
ಸ್ಥಿರ bool
has_capability (ವರ್ಣ ಧ್ರುವ *name)
{
  ವರ್ಣ *result;
  bool has_cap;

  cap_t cap_d = cap_get_file (name);
  ಯದಿ (cap_d == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ false;

  result = cap_to_text (cap_d, ರಿಕ್ತ);
  cap_free (cap_d);
  ಯದಿ (!result)
    ಪ್ರತ್ಯಾಗಮ false;

  /* check if human-readable capability string is empty */
  has_cap = !!*result;

  cap_free (result);
  ಪ್ರತ್ಯಾಗಮ has_cap;
}
#ಅನ್ಯಥಾ
ಸ್ಥಿರ bool
has_capability (MAYBE_UNUSED ವರ್ಣ ಧ್ರುವ *name)
{
  errno = ENOTSUP;
  ಪ್ರತ್ಯಾಗಮ false;
}
#endif

/* Enter and remove entries in the table 'cwd_file'.  */

ಸ್ಥಿರ ಶೂನ್ಯ
free_ent (ಸಂರಚನಾ fileinfo *f)
{
  ಮುಕ್ತ (f->name);
  ಮುಕ್ತ (f->linkname);
  ಮುಕ್ತ (f->absolute_name);
  ಯದಿ (f->scontext != UNKNOWN_SECURITY_CONTEXT)
    aclinfo_scontext_free (f->scontext);
}

/* Empty the table of files.  */
ಸ್ಥಿರ ಶೂನ್ಯ
clear_files (ಶೂನ್ಯ)
{
  ಪ್ರತಿ (idx_t i = 0; i < cwd_n_used; i++)
    {
      ಸಂರಚನಾ fileinfo *f = sorted_file[i];
      free_ent (f);
    }

  cwd_n_used = 0;
  cwd_some_quoted = false;
  any_has_acl = false;
  inode_ಸಂಖ್ಯೆ_width = 0;
  block_size_width = 0;
  nlink_width = 0;
  owner_width = 0;
  group_width = 0;
  author_width = 0;
  scontext_width = 0;
  major_device_ಸಂಖ್ಯೆ_width = 0;
  minor_device_ಸಂಖ್ಯೆ_width = 0;
  file_size_width = 0;
}

/* Cache file_has_aclinfo failure, when it's trivial to do.
   Like file_has_aclinfo, but when F's st_dev says it's on a file
   system lacking ACL support, return 0 with ENOTSUP immediately.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
file_has_aclinfo_cache (ವರ್ಣ ಧ್ರುವ *file, ಸಂರಚನಾ fileinfo *f,
                        ಸಂರಚನಾ aclinfo *ai, ಪೂರ್ಣಾಙ್ಕ flags)
{
  /* If UNSUPPORTED_SCONTEXT, these variables hold the
     st_dev and associated info for the most recently processed device
     for which file_has_aclinfo failed indicating lack of support.  */
  ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ unsupported_return;
  ಸ್ಥಿರ ವರ್ಣ *unsupported_scontext;
  ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ unsupported_scontext_err;
  ಸ್ಥಿರ dev_t unsupported_device;

  ಯದಿ (f->stat_ok && unsupported_scontext
      && f->stat.st_dev == unsupported_device)
    {
      ai->buf = ai->u._gl_acl_ch;
      ai->size = 0;
      ai->scontext = unsupported_scontext;
      ai->scontext_err = unsupported_scontext_err;
      errno = ENOTSUP;
      ಪ್ರತ್ಯಾಗಮ unsupported_return;
    }

  errno = 0;
  ಪೂರ್ಣಾಙ್ಕ n = file_has_aclinfo (file, ai, flags);
  ಪೂರ್ಣಾಙ್ಕ err = errno;
  ಯದಿ (!USE_ACL)
    n = 0;
  /* A security-context error does not imply that ACLs are unsupported.
     Require an unsupported attribute listing too if ACLs or capability
     colors are needed.  */
  ಯದಿ (f->stat_ok && n <= 0 && !acl_errno_valid (err)
      && ((!USE_ACL && !format_needs_capability)
          || (ai->size < 0 && !acl_errno_valid (ai->u.err)))
      && (!(flags & ACL_GET_SCONTEXT) || !acl_errno_valid (ai->scontext_err)))
    {
      /* FIXME: acl_errno_valid() may be a bit too broad determine file system
         non support.  E.g. EBUSY was added for per file reasons (on cygwin). */
      unsupported_return = n;
      unsupported_scontext = ai->scontext;
      unsupported_scontext_err = ai->scontext_err;
      unsupported_device = f->stat.st_dev;
    }
  ಪ್ರತ್ಯಾಗಮ n;
}

/* Cache has_capability failure, when it's trivial to do.
   Like has_capability, but when F's st_dev says it's on a file
   system lacking capability support, return 0 with ENOTSUP immediately.  */
ಸ್ಥಿರ bool
has_capability_cache (ವರ್ಣ ಧ್ರುವ *file, ಸಂರಚನಾ fileinfo *f)
{
  /* If UNSUPPORTED_CACHED, UNSUPPORTED_DEVICE is the cached
     st_dev of the most recently processed device for which we've
     found that has_capability fails indicating lack of support.  */
  ಸ್ಥಿರ bool unsupported_cached;
  ಸ್ಥಿರ dev_t unsupported_device;

  ಯದಿ (f->stat_ok && unsupported_cached && f->stat.st_dev == unsupported_device)
    {
      errno = ENOTSUP;
      ಪ್ರತ್ಯಾಗಮ 0;
    }

  bool b = has_capability (file);
  ಯದಿ (f->stat_ok && !b && !acl_errno_valid (errno))
    {
      unsupported_cached = true;
      unsupported_device = f->stat.st_dev;
    }
  ಪ್ರತ್ಯಾಗಮ b;
}

/* Get the ACL, security context, and capability information needed for F.
   FULL_NAME names the file; DO_DEREF says whether to follow symlinks.  */
ಸ್ಥಿರ ಶೂನ್ಯ
get_extended_info (ವರ್ಣ ಧ್ರುವ *full_name, ಸಂರಚನಾ fileinfo *f, bool do_deref)
{
  /* FIXME: file_has_aclinfo() conflates capability and acl xattr lookup
     with security context attributes.  Ideally we'd have separate
     ACL_GET_{CAPABILITY,ACL} flags to lookup just the required items,
     simplifying the logic here.  */

  ಗಣನಾ filetype type = f->filetype;
  bool check_acl = USE_ACL && format == long_format;
  bool get_scontext = false;
#ಯದಿ HAVE_SMACK || USE_SELINUX_SELINUX_H
  get_scontext = (format == long_format) | print_scontext;
#endif
  bool check_capability = format_needs_capability & (type == normal);

  ಯದಿ (type != arg_directory && (check_acl | get_scontext | check_capability))
    {
      ಸಂರಚನಾ aclinfo ai;
      /* Without ACL support, request contexts to obtain the attribute
         names needed for capability coloring too.  */
      ಪೂರ್ಣಾಙ್ಕ aclinfo_flags = ((do_deref ? ACL_SYMLINK_FOLLOW : 0)
                           | (get_scontext || (!USE_ACL && check_capability)
                              ? ACL_GET_SCONTEXT : 0)
                           | filetype_d_type[type]);
      ಪೂರ್ಣಾಙ್ಕ n = file_has_aclinfo_cache (full_name, f, &ai, aclinfo_flags);
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

      ಯದಿ (format == long_format && n < 0 && !cannot_access_acl)
        error (0, errno, "%s", quotef (full_name));
      ಅನ್ಯಥಾ
        {
          /* When requesting security context information, don't make
             ls fail just because the file (even a command line argument)
             isn't on the right type of file system.  I.e., a getfilecon
             failure isn't in the same class as a stat failure.  */
          ಯದಿ (print_scontext && ai.scontext_err
              && ! ignorable_traversal_errno (ai.scontext_err)
              && (! (is_ENOTSUP (ai.scontext_err)
                     || ai.scontext_err == ENODATA)))
            error (0, ai.scontext_err, "%s", quotef (full_name));
        }

      /* has_capability adds around 30% runtime to 'ls --color',
         so call it only if really needed.  Note capability coloring
         is disabled in the default color config.  */
      ಯದಿ (check_capability && aclinfo_has_xattr (&ai, XATTR_NAME_CAPS))
        f->has_capability = has_capability_cache (full_name, f);

      f->scontext = ai.scontext;
      ai.scontext = ರಿಕ್ತ;
      aclinfo_free (&ai);
    }
}

ಸ್ಥಿರ bool
needs_quoting (ವರ್ಣ ಧ್ರುವ *name)
{
  ವರ್ಣ test[2];
  size_t len = quotearg_buffer (test, ಪರಿಮಾಣ test , name, -1,
                                filename_quoting_options);
  ಪ್ರತ್ಯಾಗಮ *name != *test || strlen (name) != len;
}

/* Add a file to the current table of files.
   Verify that the file exists, and print an error message if it does not.
   Return the number of blocks that the file occupies.  */
ಸ್ಥಿರ uintmax_t
gobble_file (ವರ್ಣ ಧ್ರುವ *name, ಗಣನಾ filetype type, ino_t inode,
             bool command_ಸಾಲು_arg, ವರ್ಣ ಧ್ರುವ *dirname)
{
  uintmax_t blocks = 0;
  ಸಂರಚನಾ fileinfo *f;

  /* An inode value prior to gobble_file necessarily came from readdir,
     which is not used for command line arguments.  */
  affirm (! command_ಸಾಲು_arg || inode == NOT_AN_INODE_ಸಂಖ್ಯೆ);

  ಯದಿ (cwd_n_used == cwd_n_alloc)
    cwd_file = xpalloc (cwd_file, &cwd_n_alloc, 1, -1, ಪರಿಮಾಣ *cwd_file);

  f = &cwd_file[cwd_n_used];
  memset (f, '\0', ಪರಿಮಾಣ *f);
  f->stat.st_ino = inode;
  f->filetype = type;
  f->scontext = UNKNOWN_SECURITY_CONTEXT;

  f->quoted = -1;
  ಯದಿ ((! cwd_some_quoted) && align_variable_outer_quotes)
    {
      /* Determine if any quoted for padding purposes.  */
      f->quoted = needs_quoting (name);
      ಯದಿ (f->quoted)
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
              || color_symlink_as_referent || check_symlink_mode))
      /* Command line dereferences are already taken care of by the above
         assertion that the inode number is not yet known.  */
      || (print_inode && inode == NOT_AN_INODE_ಸಂಖ್ಯೆ)
      /* --indicator-style=classify (aka -F) requires statting each
         regular file to see whether it's executable.  */
      || ((type == normal || type == unknown)
          && (indicator_style == classify
              /* This is so that --color ends up highlighting files with these
                 mode bits set even when options like -F are not specified.  */
              || (print_with_color && (is_colored (C_EXEC)
                                       || is_colored (C_SETUID)
                                       || is_colored (C_SETGID))))));

  /* Absolute name of this file, if needed.  */
  ವರ್ಣ ಧ್ರುವ *full_name = name;
  ಯದಿ (check_stat | print_scontext | format_needs_capability
      && name[0] != '/' && dirname)
    {
      ವರ್ಣ *p = alloca (strlen (name) + strlen (dirname) + 2);
      attach (p, dirname, name);
      full_name = p;
    }

  bool do_deref;

  ಯದಿ (!check_stat)
    do_deref = dereference == DEREF_ALWAYS;
  ಅನ್ಯಥಾ
    {
      ಪೂರ್ಣಾಙ್ಕ err;

      ಯದಿ (print_hyperlink)
        {
          f->absolute_name = canonicalize_filename_mode (full_name,
                                                         CAN_MISSING);
          ಯದಿ (! f->absolute_name
              && (command_ಸಾಲು_arg
                  || ! ignorable_traversal_errno (errno)))
            file_failure (command_ಸಾಲು_arg,
                          _("error canonicalizing %s"), full_name);
        }

      ವಿಕಲ್ಪ (dereference)
        {
        ಪಕ್ಷ DEREF_ALWAYS:
          err = do_stat (full_name, &f->stat);
          do_deref = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ DEREF_COMMAND_ಸಾಲು_ARGUMENTS:
        ಪಕ್ಷ DEREF_COMMAND_ಸಾಲು_SYMLINK_TO_DIR:
          ಯದಿ (command_ಸಾಲು_arg)
            {
              bool need_lstat;
              err = do_stat (full_name, &f->stat);
              do_deref = true;

              ಯದಿ (dereference == DEREF_COMMAND_ಸಾಲು_ARGUMENTS)
                ಭಙ್ಗ;

              need_lstat = (err < 0
                            ? (errno == ENOENT || errno == ELOOP)
                            : ! S_ISDIR (f->stat.st_mode));
              ಯದಿ (!need_lstat)
                ಭಙ್ಗ;

              /* stat failed because of ENOENT || ELOOP, maybe indicating a
                 non-traversable symlink.  Or stat succeeded,
                 FULL_NAME does not refer to a directory,
                 and --dereference-command-line-symlink-to-dir is in effect.
                 Fall through so that we call lstat instead.  */
            }
          FALLTHROUGH;

        ಪಕ್ಷ DEREF_NEVER:
          err = do_lstat (full_name, &f->stat);
          do_deref = false;
          ಭಙ್ಗ;

        ಪಕ್ಷ DEREF_UNDEFINED: ಸ್ವಭಾವ:
          affirm (false);
        }

      ಯದಿ (err != 0)
        {
          ಯದಿ (! command_ಸಾಲು_arg && ignorable_traversal_errno (errno))
            {
              ಪೂರ್ಣಾಙ್ಕ stat_errno = errno;
              ಸಂರಚನಾ stat linkstat;
              bool dangling_link = (do_deref
                                    && (type == symbolic_link
                                        || type == unknown)
                                    && do_lstat (full_name, &linkstat) == 0);
              errno = stat_errno;
              ಯದಿ (! dangling_link)
                {
                  free_ent (f);
                  ಪ್ರತ್ಯಾಗಮ 0;
                }
            }

          /* Failure to stat a command line argument leads to
             an exit status of 2.  For other files, stat failure
             provokes an exit status of 1.  */
          file_failure (command_ಸಾಲು_arg,
                        _("cannot access %s"), full_name);

          ಯದಿ (command_ಸಾಲು_arg)
            {
              free_ent (f);
              ಪ್ರತ್ಯಾಗಮ 0;
            }

          f->name = xstrdup (name);
          cwd_n_used++;

          ಪ್ರತ್ಯಾಗಮ 0;
        }

      f->stat_ok = true;
      f->filetype = type = d_type_filetype[IFTODT (f->stat.st_mode)];
    }

  ಯದಿ (type == directory && command_ಸಾಲು_arg && !immediate_dirs)
    f->filetype = type = arg_directory;

  get_extended_info (full_name, f, do_deref);

  ಯದಿ ((type == symbolic_link)
      & ((format == long_format) | check_symlink_mode))
    {
      ಸಂರಚನಾ stat linkstats;

      ಯದಿ (! get_link_name (full_name, f, command_ಸಾಲು_arg))
        {
          free_ent (f);
          ಪ್ರತ್ಯಾಗಮ 0;
        }

      /* Use the slower quoting path for this entry, though
         don't update CWD_SOME_QUOTED since alignment not affected.  */
      ಯದಿ (f->linkname && f->quoted == 0 && needs_quoting (f->linkname))
        f->quoted = -1;

      /* Avoid following symbolic links when possible, i.e., when
         they won't be traced and when no indicator is needed.  */
      ಯದಿ (f->linkname
          && (file_type <= indicator_style || check_symlink_mode)
          && stat_for_mode (full_name, &linkstats) == 0)
        {
          f->linkok = true;
          f->linkmode = linkstats.st_mode;
        }
    }

  blocks = STP_NBLOCKS (&f->stat);
  ಯದಿ (format == long_format || print_block_size)
    {
      ವರ್ಣ buf[LONGEST_HUMAN_READABLE + 1];
      ಪೂರ್ಣಾಙ್ಕ len = mbswidth (human_readable (blocks, buf, human_output_opts,
                                          ST_NBLOCKSIZE, output_block_size),
                          MBSWIDTH_FLAGS);
      ಯದಿ (block_size_width < len)
        block_size_width = len;
    }

  ಯದಿ (format == long_format)
    {
      ಯದಿ (print_owner)
        {
          ಪೂರ್ಣಾಙ್ಕ len = format_user_width (f->stat.st_uid);
          ಯದಿ (owner_width < len)
            owner_width = len;
        }

      ಯದಿ (print_group)
        {
          ಪೂರ್ಣಾಙ್ಕ len = format_group_width (f->stat.st_gid);
          ಯದಿ (group_width < len)
            group_width = len;
        }

      ಯದಿ (print_author)
        {
          ಪೂರ್ಣಾಙ್ಕ len = format_user_width (f->stat.st_author);
          ಯದಿ (author_width < len)
            author_width = len;
        }
    }

  ಯದಿ (print_scontext)
    {
      ಪೂರ್ಣಾಙ್ಕ len = strlen (f->scontext);
      ಯದಿ (scontext_width < len)
        scontext_width = len;
    }

  ಯದಿ (format == long_format)
    {
      ವರ್ಣ b[INT_BUFSIZE_BOUND (uintmax_t)];
      ಪೂರ್ಣಾಙ್ಕ b_len = strlen (umaxtostr (f->stat.st_nlink, b));
      ಯದಿ (nlink_width < b_len)
        nlink_width = b_len;

      ಯದಿ ((type == chardev) | (type == blockdev))
        {
          ವರ್ಣ buf[INT_BUFSIZE_BOUND (uintmax_t)];
          ಪೂರ್ಣಾಙ್ಕ len = strlen (umaxtostr (major (f->stat.st_rdev), buf));
          ಯದಿ (major_device_ಸಂಖ್ಯೆ_width < len)
            major_device_ಸಂಖ್ಯೆ_width = len;
          len = strlen (umaxtostr (minor (f->stat.st_rdev), buf));
          ಯದಿ (minor_device_ಸಂಖ್ಯೆ_width < len)
            minor_device_ಸಂಖ್ಯೆ_width = len;
          len = major_device_ಸಂಖ್ಯೆ_width + 2 + minor_device_ಸಂಖ್ಯೆ_width;
          ಯದಿ (file_size_width < len)
            file_size_width = len;
        }
      ಅನ್ಯಥಾ
        {
          ವರ್ಣ buf[LONGEST_HUMAN_READABLE + 1];
          uintmax_t size = unsigned_file_size (f->stat.st_size);
          ಪೂರ್ಣಾಙ್ಕ len = mbswidth (human_readable (size, buf,
                                              file_human_output_opts,
                                              1, file_output_block_size),
                              MBSWIDTH_FLAGS);
          ಯದಿ (file_size_width < len)
            file_size_width = len;
        }
    }

  ಯದಿ (print_inode)
    {
      ವರ್ಣ buf[INT_BUFSIZE_BOUND (uintmax_t)];
      ಪೂರ್ಣಾಙ್ಕ len = strlen (umaxtostr (f->stat.st_ino, buf));
      ಯದಿ (inode_ಸಂಖ್ಯೆ_width < len)
        inode_ಸಂಖ್ಯೆ_width = len;
    }

  f->name = xstrdup (name);
  cwd_n_used++;

  ಪ್ರತ್ಯಾಗಮ blocks;
}

/* Return true if F refers to a directory.  */
ಸ್ಥಿರ bool
is_directory (ಧ್ರುವ ಸಂರಚನಾ fileinfo *f)
{
  ಪ್ರತ್ಯಾಗಮ f->filetype == directory || f->filetype == arg_directory;
}

/* Return true if F refers to a (symlinked) directory.  */
ಸ್ಥಿರ bool
is_linked_directory (ಧ್ರುವ ಸಂರಚನಾ fileinfo *f)
{
  ಪ್ರತ್ಯಾಗಮ f->filetype == directory || f->filetype == arg_directory
         || S_ISDIR (f->linkmode);
}

/* Put the name of the file that FILENAME is a symbolic link to
   into the LINKNAME field of 'f'.  COMMAND_LINE_ARG indicates whether
   FILENAME is a command-line argument.  */

ಸ್ಥಿರ bool
get_link_name (ವರ್ಣ ಧ್ರುವ *filename, ಸಂರಚನಾ fileinfo *f, bool command_ಸಾಲು_arg)
{
  f->linkname = areadlink_with_size (filename, f->stat.st_size);
  ಯದಿ (f->linkname == ರಿಕ್ತ)
    {
      ಯದಿ (! command_ಸಾಲು_arg
          && ignorable_traversal_errno (errno))
        ಪ್ರತ್ಯಾಗಮ false;
      file_failure (command_ಸಾಲು_arg, _("cannot read symbolic link %s"),
                    filename);
    }
  ಪ್ರತ್ಯಾಗಮ true;
}

/* Return true if the last component of NAME is '.' or '..'
   This is so we don't try to recurse on '././././. ...' */

ಸ್ಥಿರ bool
basename_is_dot_or_dotdot (ವರ್ಣ ಧ್ರುವ *name)
{
  ವರ್ಣ ಧ್ರುವ *base = last_component (name);
  ಪ್ರತ್ಯಾಗಮ dot_or_dotdot (base);
}

/* Remove any entries from CWD_FILE that are for directories,
   and queue them to be listed as directories instead.
   DIRNAME is the prefix to prepend to each dirname
   to make it correct relative to ls's working dir;
   if it is null, no prefix is needed and "." and ".." should not be ignored.
   If COMMAND_LINE_ARG is true, this directory was mentioned at the top level,
   This is desirable when processing directories recursively.  */

ಸ್ಥಿರ ಶೂನ್ಯ
extract_dirs_from_files (ವರ್ಣ ಧ್ರುವ *dirname, bool command_ಸಾಲು_arg)
{
  idx_t i, j;
  bool ignore_dot_and_dot_dot = (dirname != ರಿಕ್ತ);

  ಯದಿ (dirname && LOOP_DETECT)
    {
      /* Insert a marker entry first.  When we dequeue this marker entry,
         we'll know that DIRNAME has been processed and may be removed
         from the set of active directories.  */
      queue_directory (ರಿಕ್ತ, dirname, false);
    }

  /* Queue the directories last one first, because queueing reverses the
     order.  */
  ಪ್ರತಿ (i = cwd_n_used; 0 < i; )
    {
      i--;
      ಸಂರಚನಾ fileinfo *f = sorted_file[i];

      ಯದಿ (is_directory (f)
          && (! ignore_dot_and_dot_dot
              || ! basename_is_dot_or_dotdot (f->name)))
        {
          ಯದಿ (!dirname || f->name[0] == '/')
            queue_directory (f->name, f->linkname, command_ಸಾಲು_arg);
          ಅನ್ಯಥಾ
            {
              ವರ್ಣ *name = file_name_concat (dirname, f->name, ರಿಕ್ತ);
              queue_directory (name, f->linkname, command_ಸಾಲು_arg);
              ಮುಕ್ತ (name);
            }
          ಯದಿ (f->filetype == arg_directory)
            free_ent (f);
        }
    }

  /* Now delete the directories from the table, compacting all the remaining
     entries.  */

  ಪ್ರತಿ (i = 0, j = 0; i < cwd_n_used; i++)
    {
      ಸಂರಚನಾ fileinfo *f = sorted_file[i];
      sorted_file[j] = f;
      j += (f->filetype != arg_directory);
    }
  cwd_n_used = j;
}

/* Use strcoll to compare strings in this locale.  If an error occurs,
   report an error and longjmp to failed_strcoll.  */

ಸ್ಥಿರ jmp_buf failed_strcoll;

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
xstrcoll (ವರ್ಣ ಧ್ರುವ *a, ವರ್ಣ ಧ್ರುವ *b)
{
  ಪೂರ್ಣಾಙ್ಕ diff;
  errno = 0;
  diff = strcoll (a, b);
  ಯದಿ (errno)
    {
      error (0, errno, _("cannot compare file names %s and %s"),
             quote_n (0, a), quote_n (1, b));
      set_exit_status (false);
      longjmp (failed_strcoll, 1);
    }
  ಪ್ರತ್ಯಾಗಮ diff;
}

/* Comparison routines for sorting the files.  */

ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ ಧ್ರುವ *V;
ಪ್ರಕಾರನಿರ್ದೇಶ ಪೂರ್ಣಾಙ್ಕ (*qsortFunc)(V a, V b);

/* Used below in DEFINE_SORT_FUNCTIONS for _df_ sort function variants.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
dirfirst_check (ಸಂರಚನಾ fileinfo ಧ್ರುವ *a, ಸಂರಚನಾ fileinfo ಧ್ರುವ *b,
                ಪೂರ್ಣಾಙ್ಕ (*cmp) (V, V))
{
  ಪೂರ್ಣಾಙ್ಕ diff = is_linked_directory (b) - is_linked_directory (a);
  ಪ್ರತ್ಯಾಗಮ diff ? diff : cmp (a, b);
}

/* Define the 8 different sort function variants required for each sortkey.
   KEY_NAME is a token describing the sort key, e.g., ctime, atime, size.
   KEY_CMP_FUNC is a function to compare records based on that key, e.g.,
   ctime_cmp, atime_cmp, size_cmp.  Append KEY_NAME to the string,
   '[rev_][x]str{cmp|coll}[_df]_', to create each function name.  */
#define DEFINE_SORT_FUNCTIONS(ಕೀಲಿ_name, ಕೀಲಿ_cmp_func)			\
  /* direct, non-dirfirst versions */					\
  ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ xstrcoll_##key_name (V a, V b)				\
  { ಪ್ರತ್ಯಾಗಮ ಕೀಲಿ_cmp_func (a, b, xstrcoll); }				\
  ATTRIBUTE_PURE ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ strcmp_##key_name (V a, V b)		\
  { ಪ್ರತ್ಯಾಗಮ ಕೀಲಿ_cmp_func (a, b, strcmp); }				\
                                                                        \
  /* reverse, non-dirfirst versions */					\
  ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ rev_xstrcoll_##key_name (V a, V b)				\
  { ಪ್ರತ್ಯಾಗಮ ಕೀಲಿ_cmp_func (b, a, xstrcoll); }				\
  ATTRIBUTE_PURE ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ rev_strcmp_##key_name (V a, V b)	\
  { ಪ್ರತ್ಯಾಗಮ ಕೀಲಿ_cmp_func (b, a, strcmp); }				\
                                                                        \
  /* direct, dirfirst versions */					\
  ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ xstrcoll_df_##key_name (V a, V b)				\
  { ಪ್ರತ್ಯಾಗಮ dirfirst_check (a, b, xstrcoll_##key_name); }		\
  ATTRIBUTE_PURE ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ strcmp_df_##key_name (V a, V b)		\
  { ಪ್ರತ್ಯಾಗಮ dirfirst_check (a, b, strcmp_##key_name); }			\
                                                                        \
  /* reverse, dirfirst versions */					\
  ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ rev_xstrcoll_df_##key_name (V a, V b)			\
  { ಪ್ರತ್ಯಾಗಮ dirfirst_check (a, b, rev_xstrcoll_##key_name); }		\
  ATTRIBUTE_PURE ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ rev_strcmp_df_##key_name (V a, V b)	\
  { ಪ್ರತ್ಯಾಗಮ dirfirst_check (a, b, rev_strcmp_##key_name); }

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
cmp_ctime (ಸಂರಚನಾ fileinfo ಧ್ರುವ *a, ಸಂರಚನಾ fileinfo ಧ್ರುವ *b,
           ಪೂರ್ಣಾಙ್ಕ (*cmp) (ವರ್ಣ ಧ್ರುವ *, ವರ್ಣ ಧ್ರುವ *))
{
  ಪೂರ್ಣಾಙ್ಕ diff = timespec_cmp (get_stat_ctime (&b->stat),
                           get_stat_ctime (&a->stat));
  ಪ್ರತ್ಯಾಗಮ diff ? diff : cmp (a->name, b->name);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
cmp_mtime (ಸಂರಚನಾ fileinfo ಧ್ರುವ *a, ಸಂರಚನಾ fileinfo ಧ್ರುವ *b,
           ಪೂರ್ಣಾಙ್ಕ (*cmp) (ವರ್ಣ ಧ್ರುವ *, ವರ್ಣ ಧ್ರುವ *))
{
  ಪೂರ್ಣಾಙ್ಕ diff = timespec_cmp (get_stat_mtime (&b->stat),
                           get_stat_mtime (&a->stat));
  ಪ್ರತ್ಯಾಗಮ diff ? diff : cmp (a->name, b->name);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
cmp_atime (ಸಂರಚನಾ fileinfo ಧ್ರುವ *a, ಸಂರಚನಾ fileinfo ಧ್ರುವ *b,
           ಪೂರ್ಣಾಙ್ಕ (*cmp) (ವರ್ಣ ಧ್ರುವ *, ವರ್ಣ ಧ್ರುವ *))
{
  ಪೂರ್ಣಾಙ್ಕ diff = timespec_cmp (get_stat_atime (&b->stat),
                           get_stat_atime (&a->stat));
  ಪ್ರತ್ಯಾಗಮ diff ? diff : cmp (a->name, b->name);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
cmp_btime (ಸಂರಚನಾ fileinfo ಧ್ರುವ *a, ಸಂರಚನಾ fileinfo ಧ್ರುವ *b,
           ಪೂರ್ಣಾಙ್ಕ (*cmp) (ವರ್ಣ ಧ್ರುವ *, ವರ್ಣ ಧ್ರುವ *))
{
  ಪೂರ್ಣಾಙ್ಕ diff = timespec_cmp (get_stat_btime (&b->stat),
                           get_stat_btime (&a->stat));
  ಪ್ರತ್ಯಾಗಮ diff ? diff : cmp (a->name, b->name);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
off_cmp (off_t a, off_t b)
{
  ಪ್ರತ್ಯಾಗಮ _GL_CMP (a, b);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
cmp_size (ಸಂರಚನಾ fileinfo ಧ್ರುವ *a, ಸಂರಚನಾ fileinfo ಧ್ರುವ *b,
          ಪೂರ್ಣಾಙ್ಕ (*cmp) (ವರ್ಣ ಧ್ರುವ *, ವರ್ಣ ಧ್ರುವ *))
{
  ಪೂರ್ಣಾಙ್ಕ diff = off_cmp (b->stat.st_size, a->stat.st_size);
  ಪ್ರತ್ಯಾಗಮ diff ? diff : cmp (a->name, b->name);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
cmp_name (ಸಂರಚನಾ fileinfo ಧ್ರುವ *a, ಸಂರಚನಾ fileinfo ಧ್ರುವ *b,
          ಪೂರ್ಣಾಙ್ಕ (*cmp) (ವರ್ಣ ಧ್ರುವ *, ವರ್ಣ ಧ್ರುವ *))
{
  ಪ್ರತ್ಯಾಗಮ cmp (a->name, b->name);
}

/* Compare file extensions.  Files with no extension are 'smallest'.
   If extensions are the same, compare by file names instead.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
cmp_extension (ಸಂರಚನಾ fileinfo ಧ್ರುವ *a, ಸಂರಚನಾ fileinfo ಧ್ರುವ *b,
               ಪೂರ್ಣಾಙ್ಕ (*cmp) (ವರ್ಣ ಧ್ರುವ *, ವರ್ಣ ಧ್ರುವ *))
{
  ವರ್ಣ ಧ್ರುವ *base1 = strrchr (a->name, '.');
  ವರ್ಣ ಧ್ರುವ *base2 = strrchr (b->name, '.');
  ಪೂರ್ಣಾಙ್ಕ diff = cmp (base1 ? base1 : "", base2 ? base2 : "");
  ಪ್ರತ್ಯಾಗಮ diff ? diff : cmp (a->name, b->name);
}

/* Return the (cached) screen width,
   for the NAME associated with the passed fileinfo F.  */

ಸ್ಥಿರ size_t
fileinfo_name_width (ಸಂರಚನಾ fileinfo ಧ್ರುವ *f)
{
  ಪ್ರತ್ಯಾಗಮ f->width
         ? f->width
         : quote_name_width (f->name, filename_quoting_options, f->quoted);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
cmp_width (ಸಂರಚನಾ fileinfo ಧ್ರುವ *a, ಸಂರಚನಾ fileinfo ಧ್ರುವ *b,
          ಪೂರ್ಣಾಙ್ಕ (*cmp) (ವರ್ಣ ಧ್ರುವ *, ವರ್ಣ ಧ್ರುವ *))
{
  ಪೂರ್ಣಾಙ್ಕ diff = fileinfo_name_width (a) - fileinfo_name_width (b);
  ಪ್ರತ್ಯಾಗಮ diff ? diff : cmp (a->name, b->name);
}

DEFINE_SORT_FUNCTIONS (ctime, cmp_ctime)
DEFINE_SORT_FUNCTIONS (mtime, cmp_mtime)
DEFINE_SORT_FUNCTIONS (atime, cmp_atime)
DEFINE_SORT_FUNCTIONS (btime, cmp_btime)
DEFINE_SORT_FUNCTIONS (size, cmp_size)
DEFINE_SORT_FUNCTIONS (name, cmp_name)
DEFINE_SORT_FUNCTIONS (extension, cmp_extension)
DEFINE_SORT_FUNCTIONS (width, cmp_width)

/* Compare file versions.
   Unlike the other compare functions, cmp_version does not fail
   because filevercmp and strcmp do not fail; cmp_version uses strcmp
   instead of xstrcoll because filevercmp is locale-independent so
   strcmp is its appropriate secondary.

   All the other sort options need xstrcoll and strcmp variants,
   because they all use xstrcoll (either as the primary or secondary
   sort key), and xstrcoll has the ability to do a longjmp if strcoll fails for
   locale reasons.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
cmp_version (ಸಂರಚನಾ fileinfo ಧ್ರುವ *a, ಸಂರಚನಾ fileinfo ಧ್ರುವ *b)
{
  ಪೂರ್ಣಾಙ್ಕ diff = filevercmp (a->name, b->name);
  ಪ್ರತ್ಯಾಗಮ diff ? diff : strcmp (a->name, b->name);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
xstrcoll_version (V a, V b)
{
  ಪ್ರತ್ಯಾಗಮ cmp_version (a, b);
}
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
rev_xstrcoll_version (V a, V b)
{
  ಪ್ರತ್ಯಾಗಮ cmp_version (b, a);
}
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
xstrcoll_df_version (V a, V b)
{
  ಪ್ರತ್ಯಾಗಮ dirfirst_check (a, b, xstrcoll_version);
}
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
rev_xstrcoll_df_version (V a, V b)
{
  ಪ್ರತ್ಯಾಗಮ dirfirst_check (a, b, rev_xstrcoll_version);
}


/* We have 2^3 different variants for each sort-key function
   (for 3 independent sort modes).
   The function pointers stored in this array must be dereferenced as:

    sort_variants[sort_key][use_strcmp][reverse][dirs_first]

   Note that the order in which sort keys are listed in the function pointer
   array below is defined by the order of the elements in the time_type and
   sort_type enums!  */

#define LIST_SORTFUNCTION_VARIANTS(ಕೀಲಿ_name)                        \
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

ಸ್ಥಿರ qsortFunc ಧ್ರುವ sort_functions[][2][2][2] =
  {
    LIST_SORTFUNCTION_VARIANTS (name),
    LIST_SORTFUNCTION_VARIANTS (extension),
    LIST_SORTFUNCTION_VARIANTS (width),
    LIST_SORTFUNCTION_VARIANTS (size),

    {
      {
        { xstrcoll_version, xstrcoll_df_version },
        { rev_xstrcoll_version, rev_xstrcoll_df_version },
      },

      /* We use NULL for the strcmp variants of version comparison
         since as explained in cmp_version definition, version comparison
         does not rely on xstrcoll, so it will never longjmp, and never
         need to try the strcmp fallback. */
      {
        { ರಿಕ್ತ, ರಿಕ್ತ },
        { ರಿಕ್ತ, ರಿಕ್ತ },
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
static_assert (countof (sort_functions) == sort_numtypes - 2 + time_numtypes);

/* Set up SORTED_FILE to point to the in-use entries in CWD_FILE, in order.  */

ಸ್ಥಿರ ಶೂನ್ಯ
initialize_ordering_vector (ಶೂನ್ಯ)
{
  ಪ್ರತಿ (idx_t i = 0; i < cwd_n_used; i++)
    sorted_file[i] = &cwd_file[i];
}

/* Cache values based on attributes global to all files.  */

ಸ್ಥಿರ ಶೂನ್ಯ
update_current_files_info (ಶೂನ್ಯ)
{
  /* Cache screen width of name, if needed multiple times.  */
  ಯದಿ (sort_type == sort_width
      || (ಸಾಲು_length && (format == many_per_ಸಾಲು || format == horizontal)))
    {
      ಪ್ರತಿ (idx_t i = 0; i < cwd_n_used; i++)
        {
          ಸಂರಚನಾ fileinfo *f = sorted_file[i];
          f->width = fileinfo_name_width (f);
        }
    }
}

/* Sort the files now in the table.  */

ಸ್ಥಿರ ಶೂನ್ಯ
sort_files (ಶೂನ್ಯ)
{
  bool use_strcmp;

  ಯದಿ (sorted_file_alloc < cwd_n_used + (cwd_n_used >> 1))
    {
      ಮುಕ್ತ (sorted_file);
      sorted_file = xinmalloc (cwd_n_used, 3 * ಪರಿಮಾಣ *sorted_file);
      sorted_file_alloc = 3 * cwd_n_used;
    }

  initialize_ordering_vector ();

  update_current_files_info ();

  ಯದಿ (sort_type == sort_none)
    ಪ್ರತ್ಯಾಗಮ;

  /* Try strcoll.  If it fails, fall back on strcmp.  We can't safely
     ignore strcoll failures, as a failing strcoll might be a
     comparison function that is not a total order, and if we ignored
     the failure this might cause qsort to dump core.  */

  ಯದಿ (! setjmp (failed_strcoll))
    use_strcmp = false;      /* strcoll() succeeded */
  ಅನ್ಯಥಾ
    {
      use_strcmp = true;
      affirm (sort_type != sort_version);
      initialize_ordering_vector ();
    }

  /* When sort_type == sort_time, use time_type as subindex.  */
  mpsort ((ಶೂನ್ಯ ಧ್ರುವ **) sorted_file, cwd_n_used,
          sort_functions[sort_type + (sort_type == sort_time ? time_type : 0)]
                        [use_strcmp][sort_reverse]
                        [directories_first]);
}

/* List all the files now in the table.  */

ಸ್ಥಿರ ಶೂನ್ಯ
print_current_files (ಶೂನ್ಯ)
{
  ವಿಕಲ್ಪ (format)
    {
    ಪಕ್ಷ one_per_ಸಾಲು:
      ಪ್ರತಿ (idx_t i = 0; i < cwd_n_used; i++)
        {
          print_file_name_and_frills (sorted_file[i], 0);
          putchar (eolbyte);
        }
      ಭಙ್ಗ;

    ಪಕ್ಷ many_per_ಸಾಲು:
      ಯದಿ (! ಸಾಲು_length)
        print_with_separator (' ');
      ಅನ್ಯಥಾ
        print_many_per_ಸಾಲು ();
      ಭಙ್ಗ;

    ಪಕ್ಷ horizontal:
      ಯದಿ (! ಸಾಲು_length)
        print_with_separator (' ');
      ಅನ್ಯಥಾ
        print_horizontal ();
      ಭಙ್ಗ;

    ಪಕ್ಷ with_commas:
      print_with_separator (',');
      ಭಙ್ಗ;

    ಪಕ್ಷ long_format:
      ಪ್ರತಿ (idx_t i = 0; i < cwd_n_used; i++)
        {
          set_normal_color ();
          print_long_format (sorted_file[i]);
          dired_outbyte (eolbyte);
        }
      ಭಙ್ಗ;
    }
}

/* Replace the first %b with precomputed aligned month names.
   Note on glibc-2.7 at least, this speeds up the whole 'ls -lU'
   process by around 17%, compared to letting strftime() handle the %b.  */

ಸ್ಥಿರ ptrdiff_t
align_nstrftime (ವರ್ಣ *buf, size_t size, bool recent, ಸಂರಚನಾ tm ಧ್ರುವ *tm,
                 timezone_t tz, ಪೂರ್ಣಾಙ್ಕ ns)
{
  ವರ್ಣ ಧ್ರುವ *nfmt = (use_abformat
                      ? abformat[recent][tm->tm_mon]
                      : long_time_format[recent]);
  ಪ್ರತ್ಯಾಗಮ nstrftime (buf, size, nfmt, tm, tz, ns);
}

/* Return the expected number of columns in a long-format timestamp,
   or zero if it cannot be calculated.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
long_time_expected_width (ಶೂನ್ಯ)
{
  ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ width = -1;

  ಯದಿ (width < 0)
    {
      time_t epoch = 0;
      ಸಂರಚನಾ tm tm;
      ವರ್ಣ buf[TIME_STAMP_LEN_MAXIMUM + 1];

      /* In case you're wondering if localtime_rz can fail with an input time_t
         value of 0, let's just say it's very unlikely, but not inconceivable.
         The TZ environment variable would have to specify a time zone that
         is 2**31-1900 years or more ahead of UTC.  This could happen only on
         a 64-bit system that blindly accepts e.g., TZ=UTC+20000000000000.
         However, this is not possible with Solaris 10 or glibc-2.3.5, since
         their implementations limit the offset to 167:59 and 24:00, resp.  */
      ಯದಿ (localtime_rz (localtz, &epoch, &tm))
        {
          ptrdiff_t len = align_nstrftime (buf, ಪರಿಮಾಣ buf, false,
                                           &tm, localtz, 0);
          ಯದಿ (len > 0)
            width = mbsnwidth (buf, len, MBSWIDTH_FLAGS);
        }

      ಯದಿ (width < 0)
        width = 0;
    }

  ಪ್ರತ್ಯಾಗಮ width;
}

/* Print the user or group name NAME, with numeric id ID, using a
   print width of WIDTH columns.  */

ಸ್ಥಿರ ಶೂನ್ಯ
format_user_or_group (ವರ್ಣ ಧ್ರುವ *name, uintmax_t id, ಪೂರ್ಣಾಙ್ಕ width)
{
  ಯದಿ (name)
    {
      ಪೂರ್ಣಾಙ್ಕ name_width = mbswidth (name, MBSWIDTH_FLAGS);
      ಪೂರ್ಣಾಙ್ಕ width_gap = name_width < 0 ? 0 : width - name_width;
      ಪೂರ್ಣಾಙ್ಕ pad = MAX (0, width_gap);
      dired_outstring (name);

      ಕುರು
        dired_outbyte (' ');
      ಯಾವತ್ (pad--);
    }
  ಅನ್ಯಥಾ
    dired_pos += printf ("%*ju ", width, id);
}

/* Print the name or id of the user with id U, using a print width of
   WIDTH.  */

ಸ್ಥಿರ ಶೂನ್ಯ
format_user (uid_t u, ಪೂರ್ಣಾಙ್ಕ width, bool stat_ok)
{
  format_user_or_group (! stat_ok ? "?" :
                        (numeric_ids ? ರಿಕ್ತ : getuser (u)), u, width);
}

/* Likewise, for groups.  */

ಸ್ಥಿರ ಶೂನ್ಯ
format_group (gid_t g, ಪೂರ್ಣಾಙ್ಕ width, bool stat_ok)
{
  format_user_or_group (! stat_ok ? "?" :
                        (numeric_ids ? ರಿಕ್ತ : getgroup (g)), g, width);
}

/* Return the number of columns that format_user_or_group will print,
   or -1 if unknown.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
format_user_or_group_width (ವರ್ಣ ಧ್ರುವ *name, uintmax_t id)
{
  ಪ್ರತ್ಯಾಗಮ (name
          ? mbswidth (name, MBSWIDTH_FLAGS)
          : snprintf (ರಿಕ್ತ, 0, "%ju", id));
}

/* Return the number of columns that format_user will print,
   or -1 if unknown.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
format_user_width (uid_t u)
{
  ಪ್ರತ್ಯಾಗಮ format_user_or_group_width (numeric_ids ? ರಿಕ್ತ : getuser (u), u);
}

/* Likewise, for groups.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
format_group_width (gid_t g)
{
  ಪ್ರತ್ಯಾಗಮ format_user_or_group_width (numeric_ids ? ರಿಕ್ತ : getgroup (g), g);
}

/* Return a pointer to a formatted version of F->stat.st_ino,
   possibly using buffer, which must be at least
   INT_BUFSIZE_BOUND (uintmax_t) bytes.  */
ಸ್ಥಿರ ವರ್ಣ *
format_inode (ವರ್ಣ buf[INT_BUFSIZE_BOUND (uintmax_t)],
              ಧ್ರುವ ಸಂರಚನಾ fileinfo *f)
{
  ಪ್ರತ್ಯಾಗಮ (f->stat_ok && f->stat.st_ino != NOT_AN_INODE_ಸಂಖ್ಯೆ
          ? umaxtostr (f->stat.st_ino, buf)
          : (ವರ್ಣ *) "?");
}

/* Print information about F in long format.  */
ಸ್ಥಿರ ಶೂನ್ಯ
print_long_format (ಧ್ರುವ ಸಂರಚನಾ fileinfo *f)
{
  ವರ್ಣ modebuf[12];
  ವರ್ಣ buf
    [LONGEST_HUMAN_READABLE + 1		/* inode */
     + LONGEST_HUMAN_READABLE + 1	/* size in blocks */
     + ಪರಿಮಾಣ (modebuf) - 1 + 1		/* mode string */
     + INT_BUFSIZE_BOUND (uintmax_t)	/* st_nlink */
     + LONGEST_HUMAN_READABLE + 2	/* major device number */
     + LONGEST_HUMAN_READABLE + 1	/* minor device number */
     + TIME_STAMP_LEN_MAXIMUM + 1	/* max length of time/date */
     ];
  ptrdiff_t s;
  ವರ್ಣ *p;
  ಸಂರಚನಾ timespec when_timespec;
  ಸಂರಚನಾ tm when_local;
  bool btime_ok = true;

  /* Compute the mode string, except remove the trailing space if no
     file in this directory has an ACL or security context.  */
  ಯದಿ (f->stat_ok)
    filemodestring (&f->stat, modebuf);
  ಅನ್ಯಥಾ
    {
      modebuf[0] = filetype_letter[f->filetype];
      memset (modebuf + 1, '?', 10);
      modebuf[11] = '\0';
    }
  ಯದಿ (! any_has_acl)
    modebuf[10] = '\0';
  ಅನ್ಯಥಾ ಯದಿ (f->acl_type == ACL_T_LSM_CONTEXT_ONLY)
    modebuf[10] = '.';
  ಅನ್ಯಥಾ ಯದಿ (f->acl_type == ACL_T_YES)
    modebuf[10] = '+';
  ಅನ್ಯಥಾ ಯದಿ (f->acl_type == ACL_T_UNKNOWN)
    modebuf[10] = '?';

  ವಿಕಲ್ಪ (time_type)
    {
    ಪಕ್ಷ time_ctime:
      when_timespec = get_stat_ctime (&f->stat);
      ಭಙ್ಗ;
    ಪಕ್ಷ time_mtime:
      when_timespec = get_stat_mtime (&f->stat);
      ಭಙ್ಗ;
    ಪಕ್ಷ time_atime:
      when_timespec = get_stat_atime (&f->stat);
      ಭಙ್ಗ;
    ಪಕ್ಷ time_btime:
      when_timespec = get_stat_btime (&f->stat);
      ಯದಿ (when_timespec.tv_sec == -1 && when_timespec.tv_nsec == -1)
        btime_ok = false;
      ಭಙ್ಗ;
    ಪಕ್ಷ time_numtypes: ಸ್ವಭಾವ:
      unreachable ();
    }

  p = buf;

  ಯದಿ (print_inode)
    {
      ವರ್ಣ hbuf[INT_BUFSIZE_BOUND (uintmax_t)];
      p += sprintf (p, "%*s ", inode_ಸಂಖ್ಯೆ_width, format_inode (hbuf, f));
    }

  ಯದಿ (print_block_size)
    {
      ವರ್ಣ hbuf[LONGEST_HUMAN_READABLE + 1];
      ವರ್ಣ ಧ್ರುವ *blocks =
        (! f->stat_ok
         ? "?"
         : human_readable (STP_NBLOCKS (&f->stat), hbuf, human_output_opts,
                           ST_NBLOCKSIZE, output_block_size));
      ಪೂರ್ಣಾಙ್ಕ blocks_width = mbswidth (blocks, MBSWIDTH_FLAGS);
      ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ pad = blocks_width < 0 ? 0 : block_size_width - blocks_width;
           0 < pad; pad--)
        *p++ = ' ';
      ಯಾವತ್ ((*p++ = *blocks++))
        ಅನುವರ್ತ;
      p[-1] = ' ';
    }

  /* The last byte of the mode string is the POSIX
     "optional alternate access method flag".  */
  {
    ವರ್ಣ hbuf[INT_BUFSIZE_BOUND (uintmax_t)];
    p += sprintf (p, "%s %*s ", modebuf, nlink_width,
                  ! f->stat_ok ? "?" : umaxtostr (f->stat.st_nlink, hbuf));
  }

  dired_indent ();

  ಯದಿ (print_owner || print_group || print_author || print_scontext)
    {
      dired_outbuf (buf, p - buf);

      ಯದಿ (print_owner)
        format_user (f->stat.st_uid, owner_width, f->stat_ok);

      ಯದಿ (print_group)
        format_group (f->stat.st_gid, group_width, f->stat_ok);

      ಯದಿ (print_author)
        format_user (f->stat.st_author, author_width, f->stat_ok);

      ಯದಿ (print_scontext)
        format_user_or_group (f->scontext, 0, scontext_width);

      p = buf;
    }

  ಯದಿ (f->stat_ok
      && (S_ISCHR (f->stat.st_mode) || S_ISBLK (f->stat.st_mode)))
    {
      ಪೂರ್ಣಾಙ್ಕ blanks_width = (file_size_width
                          - (major_device_ಸಂಖ್ಯೆ_width + 2
                             + minor_device_ಸಂಖ್ಯೆ_width));
      p += sprintf (p, "%*ju, %*ju ",
                    major_device_ಸಂಖ್ಯೆ_width + MAX (0, blanks_width),
                    (uintmax_t) major (f->stat.st_rdev),
                    minor_device_ಸಂಖ್ಯೆ_width,
                    (uintmax_t) minor (f->stat.st_rdev));
    }
  ಅನ್ಯಥಾ
    {
      ವರ್ಣ hbuf[LONGEST_HUMAN_READABLE + 1];
      ವರ್ಣ ಧ್ರುವ *size =
        (! f->stat_ok
         ? "?"
         : human_readable (unsigned_file_size (f->stat.st_size),
                           hbuf, file_human_output_opts, 1,
                           file_output_block_size));
      ಪೂರ್ಣಾಙ್ಕ size_width = mbswidth (size, MBSWIDTH_FLAGS);
      ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ pad = size_width < 0 ? 0 : file_size_width - size_width;
           0 < pad; pad--)
        *p++ = ' ';
      ಯಾವತ್ ((*p++ = *size++))
        ಅನುವರ್ತ;
      p[-1] = ' ';
    }

  s = -1;

  ಯದಿ (f->stat_ok && btime_ok
      && localtime_rz (localtz, &when_timespec.tv_sec, &when_local))
    {
      ಸಂರಚನಾ timespec six_months_ago;
      bool recent;

      /* If the file appears to be in the future, update the current
         time, in case the file happens to have been modified since
         the last time we checked the clock.  */
      ಯದಿ (timespec_cmp (current_time, when_timespec) < 0)
        gettime (&current_time);

      /* Consider a time to be recent if it is within the past six months.
         A Gregorian year has 365.2425 * 24 * 60 * 60 == 31556952 seconds
         on the average.  Write this value as an integer constant to
         avoid floating point hassles.  */
      six_months_ago.tv_sec = current_time.tv_sec - 31556952 / 2;
      six_months_ago.tv_nsec = current_time.tv_nsec;

      recent = (timespec_cmp (six_months_ago, when_timespec) < 0
                && timespec_cmp (when_timespec, current_time) < 0);

      /* We assume here that all time zones are offset from UTC by a
         whole number of seconds.  */
      s = align_nstrftime (p, TIME_STAMP_LEN_MAXIMUM + 1, recent,
                           &when_local, localtz, when_timespec.tv_nsec);
    }

  ಯದಿ (0 <= s)
    {
      p += s;
      *p++ = ' ';
    }
  ಅನ್ಯಥಾ
    {
      /* The time cannot be converted using the desired format, so
         print it as a huge integer number of seconds.  */
      ವರ್ಣ hbuf[INT_BUFSIZE_BOUND (intmax_t)];
      p += sprintf (p, "%*s ", long_time_expected_width (),
                    (! f->stat_ok || ! btime_ok
                     ? "?"
                     : timetostr (when_timespec.tv_sec, hbuf)));
      /* FIXME: (maybe) We discarded when_timespec.tv_nsec. */
    }

  dired_outbuf (buf, p - buf);
  size_t w = print_name_with_quoting (f, false, &dired_obstack, p - buf);

  ಯದಿ (f->filetype == symbolic_link)
    {
      ಯದಿ (f->linkname)
        {
          dired_outstring (" -> ");
          print_name_with_quoting (f, true, ರಿಕ್ತ, (p - buf) + w + 4);
          ಯದಿ (indicator_style != none)
            print_type_indicator (true, f->linkmode, unknown);
        }
    }
  ಅನ್ಯಥಾ ಯದಿ (indicator_style != none)
    print_type_indicator (f->stat_ok, f->stat.st_mode, f->filetype);
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
quote_name_buf (ವರ್ಣ **inbuf, size_t bufsize, ವರ್ಣ *name,
                ಸಂರಚನಾ quoting_options ಧ್ರುವ *options,
                ಪೂರ್ಣಾಙ್ಕ needs_general_quoting, size_t *width, bool *pad)
{
  ವರ್ಣ *buf = *inbuf;
  size_t displayed_width IF_LINT ( = 0);
  size_t len = 0;
  bool quoted;

  ಗಣನಾ quoting_style qs = get_quoting_style (options);
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

  ಯದಿ (needs_general_quoting != 0)
    {
      len = quotearg_buffer (buf, bufsize, name, -1, options);
      ಯದಿ (bufsize <= len)
        {
          buf = xmalloc (len + 1);
          quotearg_buffer (buf, len + 1, name, -1, options);
        }

      quoted = (*name != *buf) || strlen (name) != len;
    }
  ಅನ್ಯಥಾ ಯದಿ (needs_further_quoting || extra_newline_protection)
    {
      len = strlen (name);
      ಯದಿ (bufsize <= len)
        buf = xmalloc (len + 1);
      memcpy (buf, name, len + 1);

      quoted = false;
    }
  ಅನ್ಯಥಾ
    {
      len = strlen (name);
      buf = name;
      quoted = false;
    }

  /* It's safe to replace newlines without multi-byte iteration, as newlines
     do not appear as part of any multi-byte encoded character.  */
  ಯದಿ (extra_newline_protection)
    ಪ್ರತಿ (ವರ್ಣ *p = buf; p < buf + len; p++)
      ಯದಿ (eolbyte && *p == '\n')
        *p = '?';

  ಯದಿ (needs_further_quoting)
    {
      ಯದಿ (MB_CUR_MAX > 1)
        {
          ವರ್ಣ ಧ್ರುವ *p = buf;
          ವರ್ಣ ಧ್ರುವ *plimit = buf + len;
          ವರ್ಣ *q = buf;
          displayed_width = 0;

          ಯಾವತ್ (p < plimit)
            ವಿಕಲ್ಪ (*p)
              {
                ಪಕ್ಷ ' ': ಪಕ್ಷ '!': ಪಕ್ಷ '"': ಪಕ್ಷ '#': ಪಕ್ಷ '%':
                ಪಕ್ಷ '&': ಪಕ್ಷ '\'': ಪಕ್ಷ '(': ಪಕ್ಷ ')': ಪಕ್ಷ '*':
                ಪಕ್ಷ '+': ಪಕ್ಷ ',': ಪಕ್ಷ '-': ಪಕ್ಷ '.': ಪಕ್ಷ '/':
                ಪಕ್ಷ '0': ಪಕ್ಷ '1': ಪಕ್ಷ '2': ಪಕ್ಷ '3': ಪಕ್ಷ '4':
                ಪಕ್ಷ '5': ಪಕ್ಷ '6': ಪಕ್ಷ '7': ಪಕ್ಷ '8': ಪಕ್ಷ '9':
                ಪಕ್ಷ ':': ಪಕ್ಷ ';': ಪಕ್ಷ '<': ಪಕ್ಷ '=': ಪಕ್ಷ '>':
                ಪಕ್ಷ '?':
                ಪಕ್ಷ 'A': ಪಕ್ಷ 'B': ಪಕ್ಷ 'C': ಪಕ್ಷ 'D': ಪಕ್ಷ 'E':
                ಪಕ್ಷ 'F': ಪಕ್ಷ 'G': ಪಕ್ಷ 'H': ಪಕ್ಷ 'I': ಪಕ್ಷ 'J':
                ಪಕ್ಷ 'K': ಪಕ್ಷ 'L': ಪಕ್ಷ 'M': ಪಕ್ಷ 'N': ಪಕ್ಷ 'O':
                ಪಕ್ಷ 'P': ಪಕ್ಷ 'Q': ಪಕ್ಷ 'R': ಪಕ್ಷ 'S': ಪಕ್ಷ 'T':
                ಪಕ್ಷ 'U': ಪಕ್ಷ 'V': ಪಕ್ಷ 'W': ಪಕ್ಷ 'X': ಪಕ್ಷ 'Y':
                ಪಕ್ಷ 'Z':
                ಪಕ್ಷ '[': ಪಕ್ಷ '\\': ಪಕ್ಷ ']': ಪಕ್ಷ '^': ಪಕ್ಷ '_':
                ಪಕ್ಷ 'a': ಪಕ್ಷ 'b': ಪಕ್ಷ 'c': ಪಕ್ಷ 'd': ಪಕ್ಷ 'e':
                ಪಕ್ಷ 'f': ಪಕ್ಷ 'g': ಪಕ್ಷ 'h': ಪಕ್ಷ 'i': ಪಕ್ಷ 'j':
                ಪಕ್ಷ 'k': ಪಕ್ಷ 'l': ಪಕ್ಷ 'm': ಪಕ್ಷ 'n': ಪಕ್ಷ 'o':
                ಪಕ್ಷ 'p': ಪಕ್ಷ 'q': ಪಕ್ಷ 'r': ಪಕ್ಷ 's': ಪಕ್ಷ 't':
                ಪಕ್ಷ 'u': ಪಕ್ಷ 'v': ಪಕ್ಷ 'w': ಪಕ್ಷ 'x': ಪಕ್ಷ 'y':
                ಪಕ್ಷ 'z': ಪಕ್ಷ '{': ಪಕ್ಷ '|': ಪಕ್ಷ '}': ಪಕ್ಷ '~':
                  /* These characters are printable ASCII characters.  */
                  *q++ = *p++;
                  displayed_width += 1;
                  ಭಙ್ಗ;
                ಸ್ವಭಾವ:
                  /* If we have a multibyte sequence, copy it until we
                     reach its end, replacing each non-printable multibyte
                     character with a single question mark.  */
                  {
                    mbstate_t mbstate; mbszero (&mbstate);
                    ಕುರು
                      {
                        char32_t wc;
                        size_t bytes;
                        ಪೂರ್ಣಾಙ್ಕ w;

                        bytes = mbrtoc32 (&wc, p, plimit - p, &mbstate);

                        ಯದಿ (bytes == (size_t) -1)
                          {
                            /* An invalid multibyte sequence was
                               encountered.  Skip one input byte, and
                               put a question mark.  */
                            p++;
                            *q++ = '?';
                            displayed_width += 1;
                            ಭಙ್ಗ;
                          }

                        ಯದಿ (bytes == (size_t) -2)
                          {
                            /* An incomplete multibyte character
                               at the end.  Replace it entirely with
                               a question mark.  */
                            p = plimit;
                            *q++ = '?';
                            displayed_width += 1;
                            ಭಙ್ಗ;
                          }

                        ಯದಿ (bytes == 0)
                          /* A null wide character was encountered.  */
                          bytes = 1;

                        w = c32width (wc);
                        ಯದಿ (w >= 0)
                          {
                            /* A printable multibyte character.
                               Keep it.  */
                            ಪ್ರತಿ (; bytes > 0; --bytes)
                              *q++ = *p++;
                            displayed_width += w;
                          }
                        ಅನ್ಯಥಾ
                          {
                            /* An nonprintable multibyte character.
                               Replace it entirely with a question
                               mark.  */
                            p += bytes;
                            *q++ = '?';
                            displayed_width += 1;
                          }
                      }
                    ಯಾವತ್ (! mbsinit (&mbstate));
                  }
                  ಭಙ್ಗ;
              }

          /* The buffer may have shrunk.  */
          len = q - buf;
        }
      ಅನ್ಯಥಾ
        {
          ವರ್ಣ *p = buf;
          ವರ್ಣ ಧ್ರುವ *plimit = buf + len;

          ಯಾವತ್ (p < plimit)
            {
              ಯದಿ (! isprint (to_uchar (*p)))
                *p = '?';
              p++;
            }
          displayed_width = len;
        }
    }
  ಅನ್ಯಥಾ ಯದಿ (width != ರಿಕ್ತ)
    {
      ಯದಿ (MB_CUR_MAX > 1)
        {
          displayed_width = mbsnwidth (buf, len, MBSWIDTH_FLAGS);
          displayed_width = MAX (0, displayed_width);
        }
      ಅನ್ಯಥಾ
        {
          ವರ್ಣ ಧ್ರುವ *p = buf;
          ವರ್ಣ ಧ್ರುವ *plimit = buf + len;

          displayed_width = 0;
          ಯಾವತ್ (p < plimit)
            {
              ಯದಿ (isprint (to_uchar (*p)))
                displayed_width++;
              p++;
            }
        }
    }

  /* Set padding to better align quoted items,
     and also give a visual indication that quotes are
     not actually part of the name.  */
  *pad = (align_variable_outer_quotes && cwd_some_quoted && ! quoted);

  ಯದಿ (width != ರಿಕ್ತ)
    *width = displayed_width;

  *inbuf = buf;

  ಪ್ರತ್ಯಾಗಮ len;
}

ಸ್ಥಿರ size_t
quote_name_width (ವರ್ಣ ಧ್ರುವ *name, ಸಂರಚನಾ quoting_options ಧ್ರುವ *options,
                  ಪೂರ್ಣಾಙ್ಕ needs_general_quoting)
{
  ವರ್ಣ smallbuf[BUFSIZ];
  ವರ್ಣ *buf = smallbuf;
  size_t width;
  bool pad;

  quote_name_buf (&buf, ಪರಿಮಾಣ smallbuf, (ವರ್ಣ *) name, options,
                  needs_general_quoting, &width, &pad);

  ಯದಿ (buf != smallbuf && buf != name)
    ಮುಕ್ತ (buf);

  width += pad;

  ಪ್ರತ್ಯಾಗಮ width;
}

/* %XX escape any input out of range as defined in RFC3986,
   and also if PATH, convert all path separators to '/'.  */
ಸ್ಥಿರ ವರ್ಣ *
file_escape (ವರ್ಣ ಧ್ರುವ *str, bool path)
{
  ವರ್ಣ *esc = xnmalloc (3, strlen (str) + 1);
  ವರ್ಣ *p = esc;
  ಯಾವತ್ (*str)
    {
      ಯದಿ (path && ISSLASH (*str))
        {
          *p++ = '/';
          str++;
        }
      ಅನ್ಯಥಾ ಯದಿ (RFC3986[to_uchar (*str)])
        *p++ = *str++;
      ಅನ್ಯಥಾ
        p += sprintf (p, "%%%02x", to_uchar (*str++));
    }
  *p = '\0';
  ಪ್ರತ್ಯಾಗಮ esc;
}

ಸ್ಥಿರ size_t
quote_name (ವರ್ಣ ಧ್ರುವ *name, ಸಂರಚನಾ quoting_options ಧ್ರುವ *options,
            ಪೂರ್ಣಾಙ್ಕ needs_general_quoting, ಧ್ರುವ ಸಂರಚನಾ bin_str *color,
            bool allow_pad, ಸಂರಚನಾ obstack *ರಾಶಿ, ವರ್ಣ ಧ್ರುವ *absolute_name)
{
  ವರ್ಣ smallbuf[BUFSIZ];
  ವರ್ಣ *buf = smallbuf;
  size_t len;
  bool pad;

  len = quote_name_buf (&buf, ಪರಿಮಾಣ smallbuf, (ವರ್ಣ *) name, options,
                        needs_general_quoting, ರಿಕ್ತ, &pad);

  ಯದಿ (pad && allow_pad)
    dired_outbyte (' ');

  ಯದಿ (color)
    print_color_indicator (color);

  /* If we're padding, then don't include the outer quotes in
     the --hyperlink, to improve the alignment of those links.  */
  bool skip_quotes = false;

  ಯದಿ (absolute_name)
    {
      ಯದಿ (align_variable_outer_quotes && cwd_some_quoted && ! pad)
        {
          skip_quotes = true;
          putchar (*buf);
        }
      ವರ್ಣ *h = file_escape (hostname, /* path= */ false);
      ವರ್ಣ *n = file_escape (absolute_name, /* path= */ true);
      /* TODO: It would be good to be able to define parameters
         to give hints to the terminal as how best to render the URI.
         For example since ls is outputting a dense block of URIs
         it would be best to not underline by default, and only
         do so upon hover etc.  */
      printf ("\033]8;;file://%s%s%s\033\\", h, *n == '/' ? "" : "/", n);
      ಮುಕ್ತ (h);
      ಮುಕ್ತ (n);
    }

  ಯದಿ (ರಾಶಿ)
    ತಳ್ಳು_current_dired_pos (ರಾಶಿ);

  fwrite (buf + skip_quotes, 1, len - (skip_quotes * 2), stdout);

  dired_pos += len;

  ಯದಿ (ರಾಶಿ)
    ತಳ್ಳು_current_dired_pos (ರಾಶಿ);

  ಯದಿ (absolute_name)
    {
      fputs ("\033]8;;\033\\", stdout);
      ಯದಿ (skip_quotes)
        putchar (*(buf + len - 1));
    }

  ಯದಿ (buf != smallbuf && buf != name)
    ಮುಕ್ತ (buf);

  ಪ್ರತ್ಯಾಗಮ len + pad;
}

ಸ್ಥಿರ size_t
print_name_with_quoting (ಧ್ರುವ ಸಂರಚನಾ fileinfo *f,
                         bool symlink_target,
                         ಸಂರಚನಾ obstack *ರಾಶಿ,
                         size_t start_col)
{
  ವರ್ಣ ಧ್ರುವ *name = symlink_target ? f->linkname : f->name;

  ಧ್ರುವ ಸಂರಚನಾ bin_str *color
    = print_with_color ? get_color_indicator (f, symlink_target) : ರಿಕ್ತ;

  bool used_color_this_time = (print_with_color
                               && (color || is_colored (C_NORM)));

  size_t len = quote_name (name, filename_quoting_options, f->quoted,
                           color, !symlink_target, ರಾಶಿ, f->absolute_name);

  process_signals ();
  ಯದಿ (used_color_this_time)
    {
      prep_non_filename_text ();

      /* We use the byte length rather than display width here as
         an optimization to avoid accurately calculating the width,
         because we only output the clear to EOL sequence if the name
         _might_ wrap to the next line.  This may output a sequence
         unnecessarily in multi-byte locales for example,
         but in that case it's inconsequential to the output.  */
      ಯದಿ (ಸಾಲು_length
          && (start_col / ಸಾಲು_length != (start_col + len - 1) / ಸಾಲು_length))
        put_indicator (&color_indicator[C_CLR_TO_EOL]);
    }

  ಪ್ರತ್ಯಾಗಮ len;
}

ಸ್ಥಿರ ಶೂನ್ಯ
prep_non_filename_text (ಶೂನ್ಯ)
{
  ಯದಿ (color_indicator[C_END].string != ರಿಕ್ತ)
    put_indicator (&color_indicator[C_END]);
  ಅನ್ಯಥಾ
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
print_file_name_and_frills (ಧ್ರುವ ಸಂರಚನಾ fileinfo *f, size_t start_col)
{
  ವರ್ಣ buf[MAX (LONGEST_HUMAN_READABLE + 1, INT_BUFSIZE_BOUND (uintmax_t))];

  set_normal_color ();

  ಯದಿ (print_inode)
    printf ("%*s ", format == with_commas ? 0 : inode_ಸಂಖ್ಯೆ_width,
            format_inode (buf, f));

  ಯದಿ (print_block_size)
    {
      ವರ್ಣ ಧ್ರುವ *blocks =
        (! f->stat_ok
         ? "?"
         : human_readable (STP_NBLOCKS (&f->stat), buf, human_output_opts,
                           ST_NBLOCKSIZE, output_block_size));
      ಪೂರ್ಣಾಙ್ಕ blocks_width = mbswidth (blocks, MBSWIDTH_FLAGS);
      ಪೂರ್ಣಾಙ್ಕ pad = 0;
      ಯದಿ (0 <= blocks_width && block_size_width && format != with_commas)
        pad = block_size_width - blocks_width;
      printf ("%*s%s ", pad, "", blocks);
    }

  ಯದಿ (print_scontext)
    printf ("%*s ", format == with_commas ? 0 : scontext_width, f->scontext);

  size_t width = print_name_with_quoting (f, false, ರಿಕ್ತ, start_col);

  ಯದಿ (indicator_style != none)
    width += print_type_indicator (f->stat_ok, f->stat.st_mode, f->filetype);

  ಪ್ರತ್ಯಾಗಮ width;
}

/* Given these arguments describing a file, return the single-byte
   type indicator, or 0.  */
ಸ್ಥಿರ ವರ್ಣ
get_type_indicator (bool stat_ok, mode_t mode, ಗಣನಾ filetype type)
{
  ವರ್ಣ c;

  ಯದಿ (stat_ok ? S_ISREG (mode) : type == normal)
    {
      ಯದಿ (stat_ok && indicator_style == classify && (mode & S_IXUGO))
        c = '*';
      ಅನ್ಯಥಾ
        c = 0;
    }
  ಅನ್ಯಥಾ
    {
      ಯದಿ (stat_ok ? S_ISDIR (mode) : type == directory || type == arg_directory)
        c = '/';
      ಅನ್ಯಥಾ ಯದಿ (indicator_style == slash)
        c = 0;
      ಅನ್ಯಥಾ ಯದಿ (stat_ok ? S_ISLNK (mode) : type == symbolic_link)
        c = '@';
      ಅನ್ಯಥಾ ಯದಿ (stat_ok ? S_ISFIFO (mode) : type == fifo)
        c = '|';
      ಅನ್ಯಥಾ ಯದಿ (stat_ok ? S_ISSOCK (mode) : type == sock)
        c = '=';
      ಅನ್ಯಥಾ ಯದಿ (stat_ok && S_ISDOOR (mode))
        c = '>';
      ಅನ್ಯಥಾ
        c = 0;
    }
  ಪ್ರತ್ಯಾಗಮ c;
}

ಸ್ಥಿರ bool
print_type_indicator (bool stat_ok, mode_t mode, ಗಣನಾ filetype type)
{
  ವರ್ಣ c = get_type_indicator (stat_ok, mode, type);
  ಯದಿ (c)
    dired_outbyte (c);
  ಪ್ರತ್ಯಾಗಮ !!c;
}

/* Returns if color sequence was printed.  */
ಸ್ಥಿರ bool
print_color_indicator (ಧ್ರುವ ಸಂರಚನಾ bin_str *ind)
{
  ಯದಿ (ind)
    {
      /* Need to reset so not dealing with attribute combinations */
      ಯದಿ (is_colored (C_NORM))
        restore_default_color ();
      put_indicator (&color_indicator[C_ಎಡ]);
      put_indicator (ind);
      put_indicator (&color_indicator[C_ಬಲ]);
    }

  ಪ್ರತ್ಯಾಗಮ ind != ರಿಕ್ತ;
}

/* Returns color indicator or NULL if none.  */
ATTRIBUTE_PURE
ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ bin_str *
get_color_indicator (ಧ್ರುವ ಸಂರಚನಾ fileinfo *f, bool symlink_target)
{
  ಗಣನಾ indicator_no type;
  ಸಂರಚನಾ color_ext_type *ext;	/* Color extension */
  size_t len;			/* Length of name */

  ವರ್ಣ ಧ್ರುವ *name;
  mode_t mode;
  ಪೂರ್ಣಾಙ್ಕ linkok;
  ಯದಿ (symlink_target)
    {
      name = f->linkname;
      mode = f->linkmode;
      linkok = f->linkok ? 0 : -1;
    }
  ಅನ್ಯಥಾ
    {
      name = f->name;
      mode = file_or_link_mode (f);
      linkok = f->linkok;
    }

  /* Is this a nonexistent file?  If so, linkok == -1.  */

  ಯದಿ (linkok == -1 && is_colored (C_MISSING))
    type = C_MISSING;
  ಅನ್ಯಥಾ ಯದಿ (!f->stat_ok)
    {
      ಸ್ಥಿರ ಗಣನಾ indicator_no ಧ್ರುವ filetype_indicator[] =
        {
          C_ORPHAN, C_FIFO, C_CHR, C_DIR, C_BLK, C_FILE,
          C_LINK, C_SOCK, C_FILE, C_DIR
        };
      static_assert (countof (filetype_indicator) == filetype_cardinality);
      type = filetype_indicator[f->filetype];
    }
  ಅನ್ಯಥಾ
    {
      ಯದಿ (S_ISREG (mode))
        {
          type = C_FILE;

          ಯದಿ ((mode & S_ISUID) != 0 && is_colored (C_SETUID))
            type = C_SETUID;
          ಅನ್ಯಥಾ ಯದಿ ((mode & S_ISGID) != 0 && is_colored (C_SETGID))
            type = C_SETGID;
          ಅನ್ಯಥಾ ಯದಿ (f->has_capability)
            type = C_CAP;
          ಅನ್ಯಥಾ ಯದಿ ((mode & S_IXUGO) != 0 && is_colored (C_EXEC))
            type = C_EXEC;
          ಅನ್ಯಥಾ ಯದಿ ((1 < f->stat.st_nlink) && is_colored (C_MULTIHARDLINK))
            type = C_MULTIHARDLINK;
        }
      ಅನ್ಯಥಾ ಯದಿ (S_ISDIR (mode))
        {
          type = C_DIR;

          ಯದಿ ((mode & S_ISVTX) && (mode & S_IWOTH)
              && is_colored (C_STICKY_OTHER_WRITABLE))
            type = C_STICKY_OTHER_WRITABLE;
          ಅನ್ಯಥಾ ಯದಿ ((mode & S_IWOTH) != 0 && is_colored (C_OTHER_WRITABLE))
            type = C_OTHER_WRITABLE;
          ಅನ್ಯಥಾ ಯದಿ ((mode & S_ISVTX) != 0 && is_colored (C_STICKY))
            type = C_STICKY;
        }
      ಅನ್ಯಥಾ ಯದಿ (S_ISLNK (mode))
        type = C_LINK;
      ಅನ್ಯಥಾ ಯದಿ (S_ISFIFO (mode))
        type = C_FIFO;
      ಅನ್ಯಥಾ ಯದಿ (S_ISSOCK (mode))
        type = C_SOCK;
      ಅನ್ಯಥಾ ಯದಿ (S_ISBLK (mode))
        type = C_BLK;
      ಅನ್ಯಥಾ ಯದಿ (S_ISCHR (mode))
        type = C_CHR;
      ಅನ್ಯಥಾ ಯದಿ (S_ISDOOR (mode))
        type = C_DOOR;
      ಅನ್ಯಥಾ
        {
          /* Classify a file of some other type as C_ORPHAN.  */
          type = C_ORPHAN;
        }
    }

  /* Check the file's suffix only if still classified as C_FILE.  */
  ext = ರಿಕ್ತ;
  ಯದಿ (type == C_FILE)
    {
      /* Test if NAME has a recognized suffix.  */

      len = strlen (name);
      name += len;		/* Pointer to final \0.  */
      ಪ್ರತಿ (ext = color_ext_list; ext != ರಿಕ್ತ; ext = ext->next)
        {
          ಯದಿ (ext->ext.len <= len)
            {
              ಯದಿ (ext->exact_match)
                {
                  ಯದಿ (STREQ_LEN (name - ext->ext.len, ext->ext.string,
                                 ext->ext.len))
                    ಭಙ್ಗ;
                }
              ಅನ್ಯಥಾ
                {
                  ಯದಿ (c_strncasecmp (name - ext->ext.len, ext->ext.string,
                                     ext->ext.len) == 0)
                    ಭಙ್ಗ;
                }
            }
        }
    }

  /* Adjust the color for orphaned symlinks.  */
  ಯದಿ (type == C_LINK && !linkok)
    {
      ಯದಿ (color_symlink_as_referent || is_colored (C_ORPHAN))
        type = C_ORPHAN;
    }

  ಧ್ರುವ ಸಂರಚನಾ bin_str *ಧ್ರುವ s
    = ext ? &(ext->seq) : &color_indicator[type];

  ಪ್ರತ್ಯಾಗಮ s->string ? s : ರಿಕ್ತ;
}

/* Output a color indicator (which may contain nulls).
   Note we don't update dired_pos here as emacs can be configured
   to handle color codes by stripping/converting them.  */
ಸ್ಥಿರ ಶೂನ್ಯ
put_indicator (ಧ್ರುವ ಸಂರಚನಾ bin_str *ind)
{
  ಯದಿ (! used_color)
    {
      used_color = true;

      /* If the standard output is a controlling terminal, watch out
         for signals, so that the colors can be restored to the
         default state if "ls" is suspended or interrupted.  */

      ಯದಿ (0 <= tcgetpgrp (STDOUT_FILENO))
        signal_init ();

      prep_non_filename_text ();
    }

  fwrite (ind->string, ind->len, 1, stdout);
}

ಸ್ಥಿರ size_t
length_of_file_name_and_frills (ಧ್ರುವ ಸಂರಚನಾ fileinfo *f)
{
  size_t len = 0;
  ವರ್ಣ buf[MAX (LONGEST_HUMAN_READABLE + 1, INT_BUFSIZE_BOUND (uintmax_t))];

  ಯದಿ (print_inode)
    len += 1 + (format == with_commas
                ? strlen (umaxtostr (f->stat.st_ino, buf))
                : inode_ಸಂಖ್ಯೆ_width);

  ಯದಿ (print_block_size)
    len += 1 + (format == with_commas
                ? strlen (! f->stat_ok ? "?"
                          : human_readable (STP_NBLOCKS (&f->stat), buf,
                                            human_output_opts, ST_NBLOCKSIZE,
                                            output_block_size))
                : block_size_width);

  ಯದಿ (print_scontext)
    len += 1 + (format == with_commas ? strlen (f->scontext) : scontext_width);

  len += fileinfo_name_width (f);

  ಯದಿ (indicator_style != none)
    {
      ವರ್ಣ c = get_type_indicator (f->stat_ok, f->stat.st_mode, f->filetype);
      len += (c != 0);
    }

  ಪ್ರತ್ಯಾಗಮ len;
}

ಸ್ಥಿರ ಶೂನ್ಯ
print_many_per_ಸಾಲು (ಶೂನ್ಯ)
{
  idx_t cols = calculate_columns (true);
  ಸಂರಚನಾ column_info ಧ್ರುವ *ಸಾಲು_fmt = &column_info[cols - 1];

  /* Calculate the number of rows that will be in each column except possibly
     for a short column on the right.  */
  idx_t rows = cwd_n_used / cols + (cwd_n_used % cols != 0);

  ಪ್ರತಿ (idx_t row = 0; row < rows; row++)
    {
      size_t col = 0;
      idx_t filesno = row;
      size_t pos = 0;

      /* Print the next row.  */
      ಯಾವತ್ (true)
        {
          ಸಂರಚನಾ fileinfo ಧ್ರುವ *f = sorted_file[filesno];
          size_t name_length = length_of_file_name_and_frills (f);
          size_t max_name_length = ಸಾಲು_fmt->col_arr[col++];
          print_file_name_and_frills (f, pos);

          ಯದಿ (cwd_n_used - rows <= filesno)
            ಭಙ್ಗ;
          filesno += rows;

          indent (pos + name_length, pos + max_name_length);
          pos += max_name_length;
        }
      putchar (eolbyte);
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
print_horizontal (ಶೂನ್ಯ)
{
  size_t pos = 0;
  idx_t cols = calculate_columns (false);
  ಸಂರಚನಾ column_info ಧ್ರುವ *ಸಾಲು_fmt = &column_info[cols - 1];
  ಸಂರಚನಾ fileinfo ಧ್ರುವ *f = sorted_file[0];
  size_t name_length = length_of_file_name_and_frills (f);
  size_t max_name_length = ಸಾಲು_fmt->col_arr[0];

  /* Print first entry.  */
  print_file_name_and_frills (f, 0);

  /* Now the rest.  */
  ಪ್ರತಿ (idx_t filesno = 1; filesno < cwd_n_used; filesno++)
    {
      idx_t col = filesno % cols;

      ಯದಿ (col == 0)
        {
          putchar (eolbyte);
          pos = 0;
        }
      ಅನ್ಯಥಾ
        {
          indent (pos + name_length, pos + max_name_length);
          pos += max_name_length;
        }

      f = sorted_file[filesno];
      print_file_name_and_frills (f, pos);

      name_length = length_of_file_name_and_frills (f);
      max_name_length = ಸಾಲು_fmt->col_arr[col];
    }
  putchar (eolbyte);
}

/* Output name + SEP + ' '.  */

ಸ್ಥಿರ ಶೂನ್ಯ
print_with_separator (ವರ್ಣ sep)
{
  size_t pos = 0;

  ಪ್ರತಿ (idx_t filesno = 0; filesno < cwd_n_used; filesno++)
    {
      ಸಂರಚನಾ fileinfo ಧ್ರುವ *f = sorted_file[filesno];
      size_t len = ಸಾಲು_length ? length_of_file_name_and_frills (f) : 0;

      ಯದಿ (filesno != 0)
        {
          ವರ್ಣ separator;
          size_t next_pos = 2 + (sep == ',' && filesno < cwd_n_used - 1);

          ಯದಿ (! ಸಾಲು_length
              || (! ckd_add (&next_pos, next_pos, pos)
                  && ! ckd_add (&next_pos, next_pos, len)
                  && next_pos <= ಸಾಲು_length))
            {
              pos += 2;
              separator = ' ';
            }
          ಅನ್ಯಥಾ
            {
              pos = 0;
              separator = eolbyte;
            }

          putchar (sep);
          putchar (separator);
        }

      print_file_name_and_frills (f, pos);
      pos += len;
    }
  putchar (eolbyte);
}

/* Assuming cursor is at position FROM, indent up to position TO.
   Use a TAB character instead of two or more spaces whenever possible.  */

ಸ್ಥಿರ ಶೂನ್ಯ
indent (size_t from, size_t to)
{
  ಯಾವತ್ (from < to)
    {
      ಯದಿ (tabsize != 0 && to / tabsize > (from + 1) / tabsize)
        {
          putchar ('\t');
          from += tabsize - from % tabsize;
        }
      ಅನ್ಯಥಾ
        {
          putchar (' ');
          from++;
        }
    }
}

/* Put DIRNAME/NAME into DEST, handling '.' and '/' properly.  */
/* FIXME: maybe remove this function someday.  See about using a
   non-malloc'ing version of file_name_concat.  */

ಸ್ಥಿರ ಶೂನ್ಯ
attach (ವರ್ಣ *dest, ವರ್ಣ ಧ್ರುವ *dirname, ವರ್ಣ ಧ್ರುವ *name)
{
  ವರ್ಣ ಧ್ರುವ *dirnamep = dirname;

  /* Copy dirname if it is not ".".  */
  ಯದಿ (dirname[0] != '.' || dirname[1] != 0)
    {
      ಯಾವತ್ (*dirnamep)
        *dest++ = *dirnamep++;
      /* Add '/' if 'dirname' doesn't already end with it.  */
      ಯದಿ (dirnamep > dirname && dirnamep[-1] != '/')
        *dest++ = '/';
    }
  ಯಾವತ್ (*name)
    *dest++ = *name++;
  *dest = 0;
}

/* Allocate enough column info suitable for the current number of
   files and display columns, and initialize the info to represent the
   narrowest possible columns.  */

ಸ್ಥಿರ ಶೂನ್ಯ
init_column_info (idx_t max_cols)
{
  /* Currently allocated columns in column_info.  */
  ಸ್ಥಿರ idx_t column_info_alloc;

  ಯದಿ (column_info_alloc < max_cols)
    {
      idx_t old_column_info_alloc = column_info_alloc;
      column_info = xpalloc (column_info, &column_info_alloc,
                             max_cols - column_info_alloc, -1,
                             ಪರಿಮಾಣ *column_info);

      /* Allocate the new size_t objects by computing the triangle
         formula n * (n + 1) / 2, except that we don't need to
         allocate the part of the triangle that we've already
         allocated.  Check for address arithmetic overflow.  */
      idx_t column_info_growth = column_info_alloc - old_column_info_alloc, s;
      ಯದಿ (ckd_add (&s, old_column_info_alloc + 1, column_info_alloc)
          || ckd_mul (&s, s, column_info_growth))
        xalloc_die ();
      size_t *p = xinmalloc (s >> 1, ಪರಿಮಾಣ *p);

      /* Grow the triangle by parceling out the cells just allocated.  */
      ಪ್ರತಿ (idx_t i = old_column_info_alloc; i < column_info_alloc; i++)
        {
          column_info[i].col_arr = p;
          p += i + 1;
        }
    }

  ಪ್ರತಿ (idx_t i = 0; i < max_cols; ++i)
    {
      column_info[i].valid_len = true;
      column_info[i].ಸಾಲು_len = i * MIN_COLUMN_WIDTH + 1;
      ಪ್ರತಿ (idx_t j = 0; j < i; ++j)
        column_info[i].col_arr[j] = MIN_COLUMN_WIDTH;
      column_info[i].col_arr[i] = 1;
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
  idx_t max_cols = 0 < max_idx && max_idx < cwd_n_used ? max_idx : cwd_n_used;

  init_column_info (max_cols);

  /* Compute the maximum number of possible columns.  */
  ಪ್ರತಿ (idx_t filesno = 0; filesno < cwd_n_used; ++filesno)
    {
      ಸಂರಚನಾ fileinfo ಧ್ರುವ *f = sorted_file[filesno];
      size_t name_length = length_of_file_name_and_frills (f);

      ಪ್ರತಿ (idx_t i = 0; i < max_cols; ++i)
        {
          ಯದಿ (column_info[i].valid_len)
            {
              idx_t idx = (by_columns
                           ? filesno / ((cwd_n_used + i) / (i + 1))
                           : filesno % (i + 1));
              size_t real_length = name_length + (idx == i ? 0 : 2);

              ಯದಿ (column_info[i].col_arr[idx] < real_length)
                {
                  column_info[i].ಸಾಲು_len += (real_length
                                              - column_info[i].col_arr[idx]);
                  column_info[i].col_arr[idx] = real_length;
                  column_info[i].valid_len = (column_info[i].ಸಾಲು_len
                                              <= ಸಾಲು_length);
                }
            }
        }
    }

  /* Find maximum allowed columns.  */
  idx_t cols;
  ಪ್ರತಿ (cols = max_cols; 1 < cols; --cols)
    {
      ಯದಿ (column_info[cols - 1].valid_len)
        ಭಙ್ಗ;
    }

  ಪ್ರತ್ಯಾಗಮ cols;
}

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
    {
      printf (_("Usage: %s [OPTION]... [FILE]...\n"), program_name);
      fputs (_("\
List information about the FILEs (the current directory by ಸ್ವಭಾವ).\n\
Sort entries alphabetically ಯದಿ none of -cftuvSUX nor --sort is specified.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -a, --all\n\
         ಕುರು not ignore entries starting with .\n\
"));
      oputs (_("\
  -A, --almost-all\n\
         ಕುರು not list implied . and ..\n\
"));
      oputs (_("\
      --author\n\
         with -l, print the author of each file\n\
"));
      oputs (_("\
  -b, --escape\n\
         print C-style escapes ಪ್ರತಿ nongraphic characters\n\
"));
      oputs (_("\
      --block-size=SIZE\n\
         with -l, scale sizes by SIZE when printing them;\n\
         e.g., '--block-size=M'; see SIZE format below\n\
"));
      oputs (_("\
  -B, --ignore-backups\n\
         ಕುರು not list implied entries ending with ~\n\
"));
      oputs (_("\
  -c\n\
         with -lt: sort by, and show, ctime\n\
           (time of last change of file status information);\n\
         with -l: show ctime and sort by name;\n\
         otherwise: sort by ctime, newest first\n\
"));
      oputs (_("\
  -C\n\
         list entries by columns\n\
"));
      oputs (_("\
      --color[=WHEN]\n\
         color the output WHEN; more info below\n\
"));
      oputs (_("\
  -d, --directory\n\
         list directories themselves, not their contents\n\
"));
      oputs (_("\
  -D, --dired\n\
         generate output designed ಪ್ರತಿ Emacs' dired mode\n\
"));
      oputs (_("\
  -f\n\
         same as -a -U\n\
"));
      oputs (_("\
  -F, --classify[=WHEN]\n\
         append indicator (one of */=>@|) to entries WHEN\n\
"));
      oputs (_("\
      --file-type\n\
         like -F, except ಕುರು not append '*'\n\
"));
      oputs (_("\
      --format=WORD\n\
         across,horizontal (-x), commas (-m), long (-l),\n\
         single-column (-1), verbose (-l), vertical (-C)\n\
"));
      oputs (_("\
      --full-time\n\
         like -l --time-style=full-iso\n\
"));
      oputs (_("\
  -g\n\
         like -l, but ಕುರು not list owner\n\
"));
      oputs (_("\
      --group-directories-first\n\
         group directories before files\n\
"));
      oputs (_("\
  -G, --no-group\n\
         in a long listing, don't print group names\n\
"));
      oputs (_("\
  -h, --human-readable\n\
         with -l and -s, print sizes like 1K 234M 2G etc.\n\
"));
      oputs (_("\
      --si\n\
         likewise, but use powers of 1000 not 1024\n\
"));
      oputs (_("\
  -H, --dereference-command-ಸಾಲು\n\
         follow symbolic links listed on the command ಸಾಲು\n\
"));
      oputs (_("\
      --dereference-command-ಸಾಲು-symlink-to-dir\n\
         follow each command ಸಾಲು symbolic link that points to a directory\n\
"));
      oputs (_("\
      --hide=PATTERN\n\
         ಕುರು not list implied entries matching shell PATTERN\n\
         (overridden by -a or -A)\n\
"));
      oputs (_("\
      --hyperlink[=WHEN]\n\
         hyperlink file names WHEN\n\
"));
      oputs (_("\
      --indicator-style=WORD\n\
         append indicator with style WORD to entry names:\n\
           none (ಸ್ವಭಾವ), slash (-p), file-type (--file-type), classify (-F)\n\
"));
      oputs (_("\
  -i, --inode\n\
         print the index ಸಂಖ್ಯೆ of each file\n\
"));
      oputs (_("\
  -I, --ignore=PATTERN\n\
         ಕುರು not list implied entries matching shell PATTERN\n\
"));
      oputs (_("\
  -k, --kibibytes\n\
         ಸ್ವಭಾವ to 1024-byte blocks ಪ್ರತಿ file system usage;\n\
         used only with -s and per directory totals\n\
"));
      oputs (_("\
  -l\n\
         use a long listing format\n\
"));
      oputs (_("\
  -L, --dereference\n\
         when showing file information ಪ್ರತಿ a symbolic link,\n\
         show information ಪ್ರತಿ the file the link references\n\
         rather than ಪ್ರತಿ the link itself\n\
"));
      oputs (_("\
  -m\n\
         fill width with a comma separated list of entries\n\
"));
      oputs (_("\
  -n, --numeric-uid-gid\n\
         like -l, but list numeric user and group IDs\n\
"));
      oputs (_("\
  -N, --literal\n\
         print entry names without quoting\n\
"));
      oputs (_("\
  -o\n\
         like -l, but ಕುರು not list group information\n\
"));
      oputs (_("\
  -p, --indicator-style=slash\n\
         append / indicator to directories\n\
"));
      oputs (_("\
  -q, --hide-control-chars\n\
         print ? instead of nongraphic characters\n\
"));
      oputs (_("\
      --show-control-chars\n\
         show nongraphic characters as-is;\n\
         the ಸ್ವಭಾವ, unless program is 'ls' and output is a terminal\n\
"));
      oputs (_("\
  -Q, --quote-name\n\
         enclose entry names in ದ್ವಿಗುಣ quotes\n\
"));
      oputs (_("\
      --quoting-style=WORD\n\
         use quoting style WORD ಪ್ರತಿ entry names:\n\
           literal, locale, shell, shell-always,\n\
           shell-escape, shell-escape-always, c, escape\n\
         (overrides QUOTING_STYLE environment variable)\n\
"));
      oputs (_("\
  -r, --reverse\n\
         reverse order ಯಾವತ್ sorting\n\
"));
      oputs (_("\
  -R, --recursive\n\
         list subdirectories recursively\n\
"));
      oputs (_("\
  -s, --size\n\
         print the allocated size of each file, in blocks\n\
"));
      oputs (_("\
  -S\n\
         sort by file size, largest first\n\
"));
      oputs (_("\
      --sort=WORD\n\
         change ಸ್ವಭಾವ 'name' sort to WORD:\n\
           none (-U), size (-S), time (-t),\n\
           version (-v), extension (-X), name, width\n\
"));
      oputs (_("\
      --time=WORD\n\
         select which timestamp used to display or sort;\n\
           access time (-u): atime, access, use;\n\
           metadata change time (-c): ctime, status;\n\
           modified time (ಸ್ವಭಾವ): mtime, modification;\n\
           birth time: birth, creation;\n\
         with -l, WORD determines which time to show;\n\
         with --sort=time, sort by WORD (newest first)\n\
"));
      oputs (_("\
      --time-style=TIME_STYLE\n\
         time/date format with -l; see TIME_STYLE below\n\
"));
      oputs (_("\
  -t\n\
         sort by time, newest first; see --time\n\
"));
      oputs (_("\
  -T, --tabsize=COLS\n\
         assume tab stops at each COLS instead of 8\n\
"));
      oputs (_("\
  -u\n\
         with -lt: sort by, and show, access time;\n\
         with -l: show access time and sort by name;\n\
         otherwise: sort by access time, newest first\n\
"));
      oputs (_("\
  -U\n\
         ಕುರು not sort directory entries\n\
"));
      oputs (_("\
  -v\n\
         natural sort of (version) numbers within text\n\
"));
      oputs (_("\
  -w, --width=COLS\n\
         set output width to COLS.  0 means no limit\n\
"));
      oputs (_("\
  -x\n\
         list entries by ಸಾಲುಗಳು instead of by columns\n\
"));
      oputs (_("\
  -X\n\
         sort alphabetically by entry extension\n\
"));
      oputs (_("\
  -Z, --context\n\
         print any security context of each file\n\
"));
      oputs (_("\
      --zero\n\
         end each output ಸಾಲು with NUL, not newline\n\
"));
      oputs (_("\
  -1\n\
         list one file per ಸಾಲು\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_size_note ();
      fputs (_("\
\n\
The TIME_STYLE argument can be full-iso, long-iso, iso, locale, or +FORMAT.\n\
FORMAT is interpreted like in date(1).  If FORMAT is FORMAT1<newline>FORMAT2,\n\
then FORMAT1 applies to non-recent files and FORMAT2 to recent files.\n\
TIME_STYLE prefixed with 'posix-' takes effect only outside the POSIX locale.\n\
Also the TIME_STYLE environment variable sets the ಸ್ವಭಾವ style to use.\n\
"), stdout);
      fputs (_("\
\n\
The WHEN argument defaults to 'always' and can also be 'auto' or 'never'.\n\
"), stdout);
      fputs (_("\
\n\
Using color to distinguish file types is disabled both by ಸ್ವಭಾವ and\n\
with --color=never.  With --color=auto, ls emits color codes only when\n\
standard output is connected to a terminal.  The LS_COLORS environment\n\
variable can change the settings.  Use the dircolors(1) command to set it.\n\
"), stdout);
      fputs (_("\
\n\
Exit status:\n\
 0  ಯದಿ OK,\n\
 1  ಯದಿ minor problems (e.g., cannot access subdirectory),\n\
 2  ಯದಿ serious trouble (e.g., cannot access command-ಸಾಲು argument).\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}
