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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <termios.h>
#ਜੇ HAVE_STROPTS_H
# include <stropts.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/ioctl.h>

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ WINSIZE_ਵਿੱਚ_PTEM
# include <sys/stream.h>
# include <sys/ptem.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <setjmp.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <pwd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <signal.h>

#ਜੇ HAVE_LANGINFO_CODESET
# include <langinfo.h>
#ਜੇ_ਅੰਤ

/* NonStop circa 2011 lacks both SA_RESTART and siginterrupt, so don't
   restart syscalls after a signal handler fires.  This may cause
   colors to get messed up on the screen if 'ls' is interrupted, but
   that's the best we can do on such a platform.  */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ SA_RESTART
# define SA_RESTART 0
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ <fnmatch.h>

/* Gnulib includes.  */
#ਅਨ੍ਤਰ੍ਭਾਵ "acl.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "areadlink.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "argmatch.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "assure.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "c-ctype.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "c-strcase.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "canonicalize.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "dev-ino.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "filemode.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "filenamecat.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "filevercmp.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hard-locale.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hash.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "human.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "idcache.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "mbswidth.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "mpsort.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "obstack.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "stat-size.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "stat-time.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strftime.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xdectoint.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xgethostname.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xmemdup0.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xstrtol-error.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xstrtol.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "dircolors.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ls.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "statx.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "term-sig.h"

/* Include <sys/capability.h> last to avoid a clash of <sys/types.h>
   include guards with some premature versions of libcap.
   For more details, see <https://bugzilla.redhat.com/483548>.  */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CAP
# include <sys/capability.h>
#ਜੇ_ਅੰਤ

#ਜੇ HAVE_LINUX_XATTR_H
# include <linux/xattr.h>
# ifndef XATTR_NAME_CAPS
#  define XATTR_NAME_CAPS "security.capability"
# endif
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME (ls_ਬਹੁਲਕ == LS_LS ? "ls" \
                      : (ls_ਬਹੁਲਕ == LS_MULTI_ਖਾਨਾ \
                         ? "dir" : "vdir"))

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS \
  proper_name ("Richard M. Stallman"), \
  proper_name ("David MacKenzie")

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ obstack_chunk_alloc ਸਮ੍ਰਿਤੀ_ਦਿਓ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ obstack_chunk_free ਮੁਕਤ

/* Unix-based readdir implementations have historically returned a dirent.d_ino
   value that is sometimes not equal to the stat-obtained st_ino value for
   that same entry.  This error occurs for a readdir entry that refers
   to a mount point.  readdir's error is to return the inode number of
   the underlying directory -- one that typically cannot be stat'ed, as
   long as a file system is mounted on that directory.  RELIABLE_D_INO
   encapsulates whether we can use the more efficient approach of relying
   on readdir-supplied d_ino values, or whether we must incur the cost of
   calling stat or lstat to obtain each guaranteed-valid inode number.  */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ READDIR_LIES_ABOUT_MOUNTPOINT_D_INO
# define READDIR_LIES_ABOUT_MOUNTPOINT_D_INO 1
#ਜੇ_ਅੰਤ

#ਜੇ READDIR_LIES_ABOUT_MOUNTPOINT_D_INO
# define RELIABLE_D_INO(dp) NOT_AN_INODE_ਸੰਖਿਆ
#ਨਹੀਂਤਾਂ
# define RELIABLE_D_INO(dp) D_INO (dp)
#ਜੇ_ਅੰਤ

#ਜੇ ! HAVE_STRUCT_STAT_ST_AUTHOR
# define st_author st_uid
#ਜੇ_ਅੰਤ

ਗਿਣਤੀ filetype
  {
    unknown,
    fifo,
    chardev,
    directory,
    blockdev,
    ਸਾਧਾਰਨ,
    symbolic_link,
    sock,
    whiteout,
    arg_directory
  };
ਗਿਣਤੀ { filetype_cardinality = arg_directory + 1 };

/* Display letters and indicators for each filetype.
   Keep these in sync with enum filetype.  */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ filetype_letter[] =
  {'?', 'p', 'c', 'd', 'b', '-', 'l', 's', 'w', 'd'};
static_assert (countof (filetype_letter) == filetype_cardinality);

/* Map enum filetype to <dirent.h> d_type values.  */
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ਅਚਲ filetype_d_type[] =
  {
    DT_UNKNOWN, DT_FIFO, DT_CHR, DT_DIR, DT_BLK, DT_REG, DT_LNK, DT_SOCK,
    DT_WHT, DT_DIR
  };
static_assert (countof (filetype_d_type) == filetype_cardinality);

/* Map d_type values to enum filetype.  */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ d_type_filetype[UCHAR_MAX + 1] =
  {
    [DT_BLK] = blockdev, [DT_CHR] = chardev, [DT_DIR] = directory,
    [DT_FIFO] = fifo, [DT_LNK] = symbolic_link, [DT_REG] = ਸਾਧਾਰਨ,
    [DT_SOCK] = sock, [DT_WHT] = whiteout
  };

ਗਿਣਤੀ acl_type
  {
    ACL_T_NONE,
    ACL_T_UNKNOWN,
    ACL_T_LSM_CONTEXT_ONLY,
    ACL_T_YES
  };

ਬਣਤਰ fileinfo
  {
    /* The file name.  */
    ਅੱਖਰ *name;

    /* For symbolic link, name of the file linked to, otherwise zero.  */
    ਅੱਖਰ *linkname;

    /* For terminal hyperlinks. */
    ਅੱਖਰ *absolute_name;

    ਬਣਤਰ stat stat;

    ਗਿਣਤੀ filetype filetype;

    /* For symbolic link and long listing, st_mode of file linked to, otherwise
       zero.  */
    mode_t linkmode;

    /* security context.  */
    ਅੱਖਰ *scontext;

    bool stat_ok;

    /* For symbolic link and color printing, true if linked-to file
       exists, otherwise false.  */
    bool linkok;

    /* For long listings, true if the file has an access control list,
       or a security context.  */
    ਗਿਣਤੀ acl_type acl_type;

    /* For color listings, true if a regular file has capability info.  */
    bool has_capability;

    /* Whether file name needs quoting. tri-state with -1 == unknown.  */
    ਪੂਰਨ quoted;

    /* Cached screen width (including quoting).  */
    size_t ਚੌੜਾਈ;
  };

/* Null is a valid character in a color indicator (think about Epson
   printers, for example) so we have to use a length/buffer string
   type.  */

ਬਣਤਰ bin_str
  {
    size_t len;			/* Number of bytes */
    ਅੱਖਰ ਅਚਲ *string;		/* Pointer to the same */
  };

#ਜੇ ! HAVE_TCGETPGRP
# define tcgetpgrp(Fd) 0
#ਜੇ_ਅੰਤ

ਸਥਿਰ size_t quote_name (ਅੱਖਰ ਅਚਲ *name,
                          ਬਣਤਰ quoting_options ਅਚਲ *options,
                          ਪੂਰਨ needs_general_quoting,
                          ਅਚਲ ਬਣਤਰ bin_str *color,
                          bool allow_pad, ਬਣਤਰ obstack *ਢੇਰ,
                          ਅੱਖਰ ਅਚਲ *absolute_name);
ਸਥਿਰ size_t quote_name_buf (ਅੱਖਰ **inbuf, size_t bufsize, ਅੱਖਰ *name,
                              ਬਣਤਰ quoting_options ਅਚਲ *options,
                              ਪੂਰਨ needs_general_quoting, size_t *ਚੌੜਾਈ,
                              bool *pad);
ਸਥਿਰ ਪੂਰਨ decode_switches (ਪੂਰਨ argc, ਅੱਖਰ **argv);
ਸਥਿਰ bool file_ignored (ਅੱਖਰ ਅਚਲ *name);
ਸਥਿਰ uintmax_t gobble_file (ਅੱਖਰ ਅਚਲ *name, ਗਿਣਤੀ filetype type,
                              ino_t inode, bool command_ਸਤਰ_arg,
                              ਅੱਖਰ ਅਚਲ *dirname);
ਸਥਿਰ ਅਚਲ ਬਣਤਰ bin_str * get_color_indicator (ਅਚਲ ਬਣਤਰ fileinfo *f,
                                                   bool symlink_ਨਿਸ਼ਾਨਾ);
ਸਥਿਰ bool print_color_indicator (ਅਚਲ ਬਣਤਰ bin_str *ind);
ਸਥਿਰ ਖਾਲੀ put_indicator (ਅਚਲ ਬਣਤਰ bin_str *ind);
ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_ignore_ਨਮੂਨਾ (ਅੱਖਰ ਅਚਲ *ਨਮੂਨਾ_2);
ਸਥਿਰ ਖਾਲੀ attach (ਅੱਖਰ *dest, ਅੱਖਰ ਅਚਲ *dirname, ਅੱਖਰ ਅਚਲ *name);
ਸਥਿਰ ਖਾਲੀ clear_files (ਖਾਲੀ);
ਸਥਿਰ ਖਾਲੀ ਕੱਢੋ_dirs_from_files (ਅੱਖਰ ਅਚਲ *dirname,
                                     bool command_ਸਤਰ_arg);
ਸਥਿਰ bool get_link_name (ਅੱਖਰ ਅਚਲ *filename, ਬਣਤਰ fileinfo *f,
                           bool command_ਸਤਰ_arg);
ਸਥਿਰ ਖਾਲੀ indent (size_t from, size_t ਮੰਜ਼ਿਲ);
ਸਥਿਰ idx_t calculate_columns (bool by_columns);
ਸਥਿਰ ਖਾਲੀ print_current_files (ਖਾਲੀ);
ਸਥਿਰ ਖਾਲੀ print_dir (ਅੱਖਰ ਅਚਲ *name, ਅੱਖਰ ਅਚਲ *realname,
                       bool command_ਸਤਰ_arg);
ਸਥਿਰ size_t print_file_name_and_frills (ਅਚਲ ਬਣਤਰ fileinfo *f,
                                          size_t ਸ਼ੁਰੂ_ਖਾਨਾ);
ਸਥਿਰ ਖਾਲੀ print_horizontal (ਖਾਲੀ);
ਸਥਿਰ ਪੂਰਨ format_user_ਚੌੜਾਈ (uid_t u);
ਸਥਿਰ ਪੂਰਨ format_ਸਮੂਹ_ਚੌੜਾਈ (gid_t g);
ਸਥਿਰ ਖਾਲੀ print_long_format (ਅਚਲ ਬਣਤਰ fileinfo *f);
ਸਥਿਰ ਖਾਲੀ print_many_ਪ੍ਰਤੀ_ਸਤਰ (ਖਾਲੀ);
ਸਥਿਰ size_t print_name_with_quoting (ਅਚਲ ਬਣਤਰ fileinfo *f,
                                       bool symlink_ਨਿਸ਼ਾਨਾ,
                                       ਬਣਤਰ obstack *ਢੇਰ,
                                       size_t ਸ਼ੁਰੂ_ਖਾਨਾ);
ਸਥਿਰ ਖਾਲੀ prep_non_filename_ਪਾਠ (ਖਾਲੀ);
ਸਥਿਰ bool print_type_indicator (bool stat_ok, mode_t ਬਹੁਲਕ_2,
                                  ਗਿਣਤੀ filetype type);
ਸਥਿਰ ਖਾਲੀ print_with_separator (ਅੱਖਰ sep);
ਸਥਿਰ ਖਾਲੀ ਕਤਾਰ_directory (ਅੱਖਰ ਅਚਲ *name, ਅੱਖਰ ਅਚਲ *realname,
                             bool command_ਸਤਰ_arg);
ਸਥਿਰ ਖਾਲੀ ਛਾਂਟੋ_files (ਖਾਲੀ);
ਸਥਿਰ ਖਾਲੀ parse_ls_color (ਖਾਲੀ);

ਸਥਿਰ size_t quote_name_ਚੌੜਾਈ (ਅੱਖਰ ਅਚਲ *name,
                                ਬਣਤਰ quoting_options ਅਚਲ *options,
                                ਪੂਰਨ needs_general_quoting);

/* Initial size of hash table.
   Most hierarchies are likely to be shallower than this.  */
ਗਿਣਤੀ { INITIAL_ਸਾਰਣੀ_ਮਾਪ = 30 };

/* The set of 'active' directories, from the current command-line argument
   to the level in the hierarchy at which files are being listed.
   A directory is represented by its device and inode numbers (struct dev_ino).
   A directory is added to this set when ls begins listing it or its
   entries, and it is removed from the set just after ls has finished
   processing it.  This set is used solely to detect loops, e.g., with
   mkdir loop; cd loop; ln -s ../loop sub; ls -RL  */
ਸਥਿਰ Hash_ਸਾਰਣੀ *ਸਰਗਰਮ_dir_ਸੈੱਟ;

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LOOP_DETECT (!!ਸਰਗਰਮ_dir_ਸੈੱਟ)

/* The table of files in the current directory:

   'cwd_file' points to a vector of 'struct fileinfo', one per file.
   'cwd_n_alloc' is the number of elements space has been allocated for.
   'cwd_n_used' is the number actually in use.  */

/* Address of block containing the files that are described.  */
ਸਥਿਰ ਬਣਤਰ fileinfo *cwd_file;

/* Length of block that 'cwd_file' points to, measured in files.  */
ਸਥਿਰ idx_t cwd_n_alloc;

/* Index of first unused slot in 'cwd_file'.  */
ਸਥਿਰ idx_t cwd_n_ਵਰਤਿਆ;

/* Whether files needs may need padding due to quoting.  */
ਸਥਿਰ bool cwd_some_quoted;

/* Whether quoting style _may_ add outer quotes,
   and whether aligning those is useful.  */
ਸਥਿਰ bool align_variable_outer_quotes;

/* Vector of pointers to files, in proper sorted order, and the number
   of entries allocated for it.  */
ਸਥਿਰ ਖਾਲੀ **sorted_file;
ਸਥਿਰ size_t sorted_file_alloc;

/* When true, in a color listing, color each symlink name according to the
   type of file it points to.  Otherwise, color them according to the 'ln'
   directive in LS_COLORS.  Dangling (orphan) symlinks are treated specially,
   regardless.  This is set when 'ln=target' appears in LS_COLORS.  */

ਸਥਿਰ bool color_symlink_as_referent;

ਸਥਿਰ ਅੱਖਰ ਅਚਲ *hostname;

/* Mode of appropriate file for coloring.  */
ਸਥਿਰ mode_t
file_or_link_ਬਹੁਲਕ (ਬਣਤਰ fileinfo ਅਚਲ *file)
{
  ਵਾਪਸ (color_symlink_as_referent && file->linkok
          ? file->linkmode : file->stat.st_ਬਹੁਲਕ);
}


/* Record of one pending directory waiting to be listed.  */

ਬਣਤਰ pending
  {
    ਅੱਖਰ *name;
    /* If the directory is actually the file pointed to by a symbolic link we
       were told to list, 'realname' will contain the name of the symbolic
       link, otherwise zero.  */
    ਅੱਖਰ *realname;
    bool command_ਸਤਰ_arg;
    ਬਣਤਰ pending *ਅਗਲਾ;
  };

ਸਥਿਰ ਬਣਤਰ pending *pending_dirs;

/* Current time in seconds and nanoseconds since 1970, updated as
   needed when deciding whether a file is recent.  */

ਸਥਿਰ ਬਣਤਰ timespec current_ਸਮਾਂ;

ਸਥਿਰ bool print_scontext;
ਸਥਿਰ ਅੱਖਰ UNKNOWN_SECURITY_CONTEXT[] = "?";

/* Whether any of the files has an ACL.  This affects the width of the
   mode column.  */

ਸਥਿਰ bool any_has_acl;

/* The number of columns to use for columns containing inode numbers,
   block sizes, link counts, owners, groups, authors, major device
   numbers, minor device numbers, and file sizes, respectively.  */

ਸਥਿਰ ਪੂਰਨ inode_ਸੰਖਿਆ_ਚੌੜਾਈ;
ਸਥਿਰ ਪੂਰਨ ਖੰਡ_ਮਾਪ_ਚੌੜਾਈ;
ਸਥਿਰ ਪੂਰਨ nlink_ਚੌੜਾਈ;
ਸਥਿਰ ਪੂਰਨ scontext_ਚੌੜਾਈ;
ਸਥਿਰ ਪੂਰਨ owner_ਚੌੜਾਈ;
ਸਥਿਰ ਪੂਰਨ ਸਮੂਹ_ਚੌੜਾਈ;
ਸਥਿਰ ਪੂਰਨ author_ਚੌੜਾਈ;
ਸਥਿਰ ਪੂਰਨ major_device_ਸੰਖਿਆ_ਚੌੜਾਈ;
ਸਥਿਰ ਪੂਰਨ minor_device_ਸੰਖਿਆ_ਚੌੜਾਈ;
ਸਥਿਰ ਪੂਰਨ file_ਮਾਪ_ਚੌੜਾਈ;

/* Option flags */

/* long_format for lots of info, one per line.
   one_per_line for just names, one per line.
   many_per_line for just names, many per line, sorted vertically.
   horizontal for just names, many per line, sorted horizontally.
   with_commas for just names, many per line, separated by commas.

   -l (and other options that imply -l), -1, -C, -x and -m control
   this parameter.  */

ਗਿਣਤੀ format
  {
    long_format,		/* -l and other options that imply -l */
    one_ਪ੍ਰਤੀ_ਸਤਰ,		/* -1 */
    many_ਪ੍ਰਤੀ_ਸਤਰ,		/* -C */
    horizontal,			/* -x */
    with_commas			/* -m */
  };

ਸਥਿਰ ਗਿਣਤੀ format format;

/* 'full-iso' uses full ISO-style dates and times.  'long-iso' uses longer
   ISO-style timestamps, though shorter than 'full-iso'.  'iso' uses shorter
   ISO-style timestamps.  'locale' uses locale-dependent timestamps.  */
ਗਿਣਤੀ ਸਮਾਂ_style
  {
    ਭਰਿਆ_iso_ਸਮਾਂ_style,	/* --time-style=full-iso */
    long_iso_ਸਮਾਂ_style,	/* --time-style=long-iso */
    iso_ਸਮਾਂ_style,		/* --time-style=iso */
    locale_ਸਮਾਂ_style		/* --time-style=locale */
  };

ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ਅਚਲ ਸਮਾਂ_style_ਦਲੀਲਾਂ[] =
{
  "full-iso", "long-iso", "iso", "locale", ਨਿਰਾ
};
ਸਥਿਰ ਗਿਣਤੀ ਸਮਾਂ_style ਅਚਲ ਸਮਾਂ_style_types[] =
{
  ਭਰਿਆ_iso_ਸਮਾਂ_style, long_iso_ਸਮਾਂ_style, iso_ਸਮਾਂ_style,
  locale_ਸਮਾਂ_style
};
ARGMATCH_VERIFY (ਸਮਾਂ_style_ਦਲੀਲਾਂ, ਸਮਾਂ_style_types);

/* Type of time to print or sort by.  Controlled by -c and -u.
   The values of each item of this enum are important since they are
   used as indices in the sort functions array (see sort_files()).  */

ਗਿਣਤੀ ਸਮਾਂ_type
  {
    ਸਮਾਂ_mtime = 0,		/* default */
    ਸਮਾਂ_ctime,			/* -c */
    ਸਮਾਂ_atime,			/* -u */
    ਸਮਾਂ_btime,                 /* birth time */
    ਸਮਾਂ_numtypes		/* the number of elements of this enum */
  };

ਸਥਿਰ ਗਿਣਤੀ ਸਮਾਂ_type ਸਮਾਂ_type;
ਸਥਿਰ bool explicit_ਸਮਾਂ;

/* The file characteristic to sort by.  Controlled by -t, -S, -U, -X, -v.
   The values of each item of this enum are important since they are
   used as indices in the sort functions array (see sort_files()).  */

ਗਿਣਤੀ ਛਾਂਟੋ_type
  {
    ਛਾਂਟੋ_name = 0,		/* default */
    ਛਾਂਟੋ_extension,		/* -X */
    ਛਾਂਟੋ_ਚੌੜਾਈ,
    ਛਾਂਟੋ_ਮਾਪ,			/* -S */
    ਛਾਂਟੋ_ਸੰਸਕਰਣ,		/* -v */
    ਛਾਂਟੋ_ਸਮਾਂ,			/* -t; must be second to last */
    ਛਾਂਟੋ_none,			/* -U; must be last */
    ਛਾਂਟੋ_numtypes		/* the number of elements of this enum */
  };

ਸਥਿਰ ਗਿਣਤੀ ਛਾਂਟੋ_type ਛਾਂਟੋ_type;

/* Direction of sort.
   false means highest first if numeric,
   lowest first if alphabetic;
   these are the defaults.
   true means the opposite order in each case.  -r  */

ਸਥਿਰ bool ਛਾਂਟੋ_ਉਲਟ;

/* True means to display owner information.  -g turns this off.  */

ਸਥਿਰ bool print_owner = true;

/* True means to display author information.  */

ਸਥਿਰ bool print_author;

/* True means to display group information.  -G and -o turn this off.  */

ਸਥਿਰ bool print_ਸਮੂਹ = true;

/* True means print the user and group id's as numbers rather
   than as names.  -n  */

ਸਥਿਰ bool numeric_ids;

/* True means mention the size in blocks of each file.  -s  */

ਸਥਿਰ bool print_ਖੰਡ_ਮਾਪ;

/* Human-readable options for output, when printing block counts.  */
ਸਥਿਰ ਪੂਰਨ human_output_opts;

/* The units to use when printing block counts.  */
ਸਥਿਰ uintmax_t output_ਖੰਡ_ਮਾਪ;

/* Likewise, but for file sizes.  */
ਸਥਿਰ ਪੂਰਨ file_human_output_opts;
ਸਥਿਰ uintmax_t file_output_ਖੰਡ_ਮਾਪ = 1;

/* Follow the output with a special string.  Using this format,
   Emacs' dired mode starts up twice as fast, and can handle all
   strange characters in file names.  */
ਸਥਿਰ bool dired;

/* 'none' means don't mention the type of files.
   'slash' means mention directories only, with a '/'.
   'file_type' means mention file types.
   'classify' means mention file types and mark executables.

   Controlled by -F, -p, and --indicator-style.  */

ਗਿਣਤੀ indicator_style
  {
    none = 0,	/*     --indicator-style=none (default) */
    slash,	/* -p, --indicator-style=slash */
    file_type,	/*     --indicator-style=file-type */
    classify	/* -F, --indicator-style=classify */
  };

ਸਥਿਰ ਗਿਣਤੀ indicator_style indicator_style;

/* Names of indicator styles.  */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ਅਚਲ indicator_style_ਦਲੀਲਾਂ[] =
{
  "none", "slash", "file-type", "classify", ਨਿਰਾ
};
ਸਥਿਰ ਗਿਣਤੀ indicator_style ਅਚਲ indicator_style_types[] =
{
  none, slash, file_type, classify
};
ARGMATCH_VERIFY (indicator_style_ਦਲੀਲਾਂ, indicator_style_types);

/* True means use colors to mark types.  Also define the different
   colors as well as the stuff for the LS_COLORS environment variable.
   The LS_COLORS variable is now in a termcap-like format.  */

ਸਥਿਰ bool print_with_color;

ਸਥਿਰ bool print_hyperlink;

/* Whether we used any colors in the output so far.  If so, we will
   need to restore the default color later.  If not, we will need to
   call prep_non_filename_text before using color for the first time. */

ਸਥਿਰ bool ਵਰਤਿਆ_color = false;

ਗਿਣਤੀ when_type
  {
    when_never,		/* 0: default or --color=never */
    when_always,	/* 1: --color=always */
    when_if_tty		/* 2: --color=tty */
  };

ਗਿਣਤੀ Dereference_symlink
  {
    DEREF_UNDEFINED = 0,		/* default */
    DEREF_NEVER,
    DEREF_COMMAND_ਸਤਰ_ARGUMENTS,	/* -H */
    DEREF_COMMAND_ਸਤਰ_SYMLINK_ਮੰਜ਼ਿਲ_DIR,	/* the default, in certain cases */
    DEREF_ALWAYS			/* -L */
  };

ਗਿਣਤੀ indicator_no
  {
    C_ਖੱਬਾ, C_ਸੱਜਾ, C_ਸਿਰਾ, C_RESET, C_NORM, C_FILE, C_DIR, C_LINK,
    C_FIFO, C_SOCK,
    C_BLK, C_CHR, C_MISSING, C_ORPHAN, C_EXEC, C_DOOR, C_SETUID, C_SETGID,
    C_STICKY, C_OTHER_WRITABLE, C_STICKY_OTHER_WRITABLE, C_CAP, C_MULTIHARDLINK,
    C_CLR_ਮੰਜ਼ਿਲ_EOL
  };

ਸਥਿਰ ਅੱਖਰ ਅਚਲ indicator_name[][2]=
  {
    {'l','c'}, {'r','c'}, {'e','c'}, {'r','s'}, {'n','o'},
    {'f','i'}, {'d','i'}, {'l','n'},
    {'p','i'}, {'s','o'},
    {'b','d'}, {'c','d'}, {'m','i'}, {'o','r'}, {'e','x'},
    {'d','o'}, {'s','u'}, {'s','g'},
    {'s','t'}, {'o','w'}, {'t','w'}, {'c','a'}, {'m','h'},
    {'c','l'}
  };

ਬਣਤਰ color_ext_type
  {
    ਬਣਤਰ bin_str ext;		/* The extension we're looking for */
    ਬਣਤਰ bin_str seq;		/* The sequence to output when we do */
    bool   exact_ਮੇਲ;		/* Whether to compare case insensitively */
    ਬਣਤਰ color_ext_type *ਅਗਲਾ;	/* Next in list */
  };

ਸਥਿਰ ਬਣਤਰ bin_str color_indicator[] =
  {
    { 2, (ਅੱਖਰ ਅਚਲ []) {'\033','['} },/* lc: Left of color sequence */
    { 1, (ਅੱਖਰ ਅਚਲ []) {'m'} },	/* rc: Right of color sequence */
    { 0, ਨਿਰਾ },			/* ec: End color (replaces lc+rs+rc) */
    { 1, (ਅੱਖਰ ਅਚਲ []) {'0'} },	/* rs: Reset to ordinary colors */
    { 0, ਨਿਰਾ },			/* no: Normal */
    { 0, ਨਿਰਾ },			/* fi: File: default */
    { 5, ((ਅੱਖਰ ਅਚਲ [])
          {'0','1',';','3','4'}) },	/* di: Directory: bright blue */
    { 5, ((ਅੱਖਰ ਅਚਲ [])
          {'0','1',';','3','6'}) },	/* ln: Symlink: bright cyan */
    { 2, (ਅੱਖਰ ਅਚਲ []) {'3','3'} },		/* pi: Pipe: yellow/brown */
    { 5, ((ਅੱਖਰ ਅਚਲ [])
          {'0','1',';','3','5'}) },	/* so: Socket: bright magenta */
    { 5, ((ਅੱਖਰ ਅਚਲ [])
          {'0','1',';','3','3'}) },	/* bd: Block device: bright yellow */
    { 5, ((ਅੱਖਰ ਅਚਲ [])
          {'0','1',';','3','3'}) },	/* cd: Char device: bright yellow */
    { 0, ਨਿਰਾ },			/* mi: Missing file: undefined */
    { 0, ਨਿਰਾ },			/* or: Orphaned symlink: undefined */
    { 5, ((ਅੱਖਰ ਅਚਲ [])
          {'0','1',';','3','2'}) },	/* ex: Executable: bright green */
    { 5, ((ਅੱਖਰ ਅਚਲ [])
          {'0','1',';','3','5'}) },	/* do: Door: bright magenta */
    { 5, ((ਅੱਖਰ ਅਚਲ [])
          {'3','7',';','4','1'}) },	/* su: setuid: white on red */
    { 5, ((ਅੱਖਰ ਅਚਲ [])
          {'3','0',';','4','3'}) },	/* sg: setgid: black on yellow */
    { 5, ((ਅੱਖਰ ਅਚਲ [])
          {'3','7',';','4','4'}) },	/* st: sticky: black on blue */
    { 5, ((ਅੱਖਰ ਅਚਲ [])
          {'3','4',';','4','2'}) },	/* ow: other-writable: blue on green */
    { 5, ((ਅੱਖਰ ਅਚਲ [])
          {'3','0',';','4','2'}) },	/* tw: ow w/ sticky: black on green */
    { 0, ਨਿਰਾ },			/* ca: disabled by default */
    { 0, ਨਿਰਾ },			/* mh: disabled by default */
    { 3, ((ਅੱਖਰ ਅਚਲ [])
          {'\033','[','K'}) },		/* cl: clear to end of line */
  };

/* A list mapping file extensions to corresponding display sequence.  */
ਸਥਿਰ ਬਣਤਰ color_ext_type *color_ext_list = ਨਿਰਾ;

/* Buffer for color sequences */
ਸਥਿਰ ਅੱਖਰ *color_buf;

/* True means to check for orphaned symbolic link, for displaying
   colors, or to group symlink to directories with other dirs.  */

ਸਥਿਰ bool check_symlink_ਬਹੁਲਕ;

/* True means mention the inode number of each file.  -i  */

ਸਥਿਰ bool print_inode;

/* What to do with symbolic links.  Affected by -d, -F, -H, -l (and
   other options that imply -l), and -L.  */

ਸਥਿਰ ਗਿਣਤੀ Dereference_symlink dereference;

/* True means when a directory is found, display info on its
   contents.  -R  */

ਸਥਿਰ bool recursive;

/* True means when an argument is a directory name, display info
   on it itself.  -d  */

ਸਥਿਰ bool ਤੁਰੰਤ_dirs;

/* True means that directories are grouped before files. */

ਸਥਿਰ bool directories_ਪਹਿਲਾ;

/* Which files to ignore.  */

ਸਥਿਰ ਗਿਣਤੀ
{
  /* Ignore files whose names start with '.', and files specified by
     --hide and --ignore.  */
  IGNORE_DEFAULT = 0,

  /* Ignore '.', '..', and files specified by --ignore.  */
  IGNORE_DOT_AND_DOTDOT,

  /* Ignore only files specified by --ignore.  */
  IGNORE_ਨਿਊਨਤਮ
} ignore_ਬਹੁਲਕ;

/* A linked list of shell-style globbing patterns.  If a non-argument
   file name matches any of these patterns, it is ignored.
   Controlled by -I.  Multiple -I options accumulate.
   The -B option adds '*~' and '.*~' to this list.  */

ਬਣਤਰ ignore_ਨਮੂਨਾ
  {
    ਅੱਖਰ ਅਚਲ *ਨਮੂਨਾ_2;
    ਬਣਤਰ ignore_ਨਮੂਨਾ *ਅਗਲਾ;
  };

ਸਥਿਰ ਬਣਤਰ ignore_ਨਮੂਨਾ *ignore_patterns;

/* Similar to IGNORE_PATTERNS, except that -a or -A causes this
   variable itself to be ignored.  */
ਸਥਿਰ ਬਣਤਰ ignore_ਨਮੂਨਾ *hide_patterns;

/* True means output nongraphic chars in file names as '?'.
   (-q, --hide-control-chars)
   qmark_funny_chars and the quoting style (-Q, --quoting-style=WORD) are
   independent.  The algorithm is: first, obey the quoting style to get a
   string representing the file name;  then, if qmark_funny_chars is set,
   replace all nonprintable chars in that string with '?'.  It's necessary
   to replace nonprintable chars even in quoted strings, because we don't
   want to mess up the terminal if control chars get sent to it, and some
   quoting methods pass through control chars as-is.  */
ਸਥਿਰ bool qmark_funny_chars;

/* True means extra replacement of newline with '?'
   in cases where a newline in a file name would be
   ambiguous with newline separators.  */
ਸਥਿਰ bool qmark_newline;

/* Quoting options for file and dir name output.  */

ਸਥਿਰ ਬਣਤਰ quoting_options *filename_quoting_options;
ਸਥਿਰ ਬਣਤਰ quoting_options *dirname_quoting_options;

/* The number of chars per hardware tab stop.  Setting this to zero
   inhibits the use of TAB characters for separating columns.  -T */
ਸਥਿਰ size_t tabsize;

/* True means print each directory name before listing it.  */

ਸਥਿਰ bool print_dir_name;

/* The line length to use for breaking lines in many-per-line format.
   Can be set with -w.  If zero, there is no limit.  */

ਸਥਿਰ size_t ਸਤਰ_ਲੰਮਾਈ;

/* The local time zone rules, as per the TZ environment variable.  */

ਸਥਿਰ timezone_t localtz;

/* If true, the file listing format requires that stat be called on
   each file.  */

ਸਥਿਰ bool format_needs_stat;

/* Similar to 'format_needs_stat', but set if only the file type is
   needed.  */

ਸਥਿਰ bool format_needs_type;

/* Like 'format_needs_stat', but set only if capability colors are needed.  */

ਸਥਿਰ bool format_needs_capability;

/* An arbitrary limit on the number of bytes in a printed timestamp.
   This is set to a relatively small value to avoid the need to worry
   about denial-of-service attacks on servers that run "ls" on behalf
   of remote clients.  1000 bytes should be enough for any practical
   timestamp format.  */

ਗਿਣਤੀ { ਸਮਾਂ_STAMP_LEN_ਵੱਧ_ਤੋਂ_ਵੱਧ = MAX (1000, INT_STRLEN_BOUND (time_t)) };

/* strftime formats for non-recent and recent files, respectively, in
   -l output.  */

ਸਥਿਰ ਅੱਖਰ ਅਚਲ *long_ਸਮਾਂ_format[2] =
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

ਸਥਿਰ sigset_t caught_ਸੰਕੇਤ;

/* If nonzero, the value of the pending fatal signal.  */

ਸਥਿਰ sig_atomic_t ਅਸਥਿਰ ਰੁਕਾਵਟ_signal;

/* A count of the number of pending stop signals that have been received.  */

ਸਥਿਰ sig_atomic_t ਅਸਥਿਰ stop_signal_ਗਿਣਤੀ;

/* Desired exit status.  */

ਸਥਿਰ ਪੂਰਨ exit_ਸਥਿਤੀ;

/* Exit statuses.  */
ਗਿਣਤੀ
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
ਗਿਣਤੀ
{
  AUTHOR_OPTION = CHAR_MAX + 1,
  ਖੰਡ_ਮਾਪ_OPTION,
  COLOR_OPTION,
  DEREFERENCE_COMMAND_ਸਤਰ_SYMLINK_ਮੰਜ਼ਿਲ_DIR_OPTION,
  FILE_TYPE_INDICATOR_OPTION,
  FORMAT_OPTION,
  ਭਰਿਆ_ਸਮਾਂ_OPTION,
  ਸਮੂਹ_DIRECTORIES_ਪਹਿਲਾ_OPTION,
  HIDE_OPTION,
  HYPERLINK_OPTION,
  INDICATOR_STYLE_OPTION,
  QUOTING_STYLE_OPTION,
  SHOW_CONTROL_CHARS_OPTION,
  SI_OPTION,
  ਛਾਂਟੋ_OPTION,
  ਸਮਾਂ_OPTION,
  ਸਮਾਂ_STYLE_OPTION,
  ZERO_OPTION,
};

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ long_options[] =
{
  {"all", no_ਦਲੀਲ, ਨਿਰਾ, 'a'},
  {"escape", no_ਦਲੀਲ, ਨਿਰਾ, 'b'},
  {"directory", no_ਦਲੀਲ, ਨਿਰਾ, 'd'},
  {"dired", no_ਦਲੀਲ, ਨਿਰਾ, 'D'},
  {"full-time", no_ਦਲੀਲ, ਨਿਰਾ, ਭਰਿਆ_ਸਮਾਂ_OPTION},
  {"group-directories-first", no_ਦਲੀਲ, ਨਿਰਾ,
   ਸਮੂਹ_DIRECTORIES_ਪਹਿਲਾ_OPTION},
  {"human-readable", no_ਦਲੀਲ, ਨਿਰਾ, 'h'},
  {"inode", no_ਦਲੀਲ, ਨਿਰਾ, 'i'},
  {"kibibytes", no_ਦਲੀਲ, ਨਿਰਾ, 'k'},
  {"numeric-uid-gid", no_ਦਲੀਲ, ਨਿਰਾ, 'n'},
  {"no-group", no_ਦਲੀਲ, ਨਿਰਾ, 'G'},
  {"hide-control-chars", no_ਦਲੀਲ, ਨਿਰਾ, 'q'},
  {"reverse", no_ਦਲੀਲ, ਨਿਰਾ, 'r'},
  {"size", no_ਦਲੀਲ, ਨਿਰਾ, 's'},
  {"width", required_ਦਲੀਲ, ਨਿਰਾ, 'w'},
  {"almost-all", no_ਦਲੀਲ, ਨਿਰਾ, 'A'},
  {"ignore-backups", no_ਦਲੀਲ, ਨਿਰਾ, 'B'},
  {"classify", optional_ਦਲੀਲ, ਨਿਰਾ, 'F'},
  {"file-type", no_ਦਲੀਲ, ਨਿਰਾ, FILE_TYPE_INDICATOR_OPTION},
  {"si", no_ਦਲੀਲ, ਨਿਰਾ, SI_OPTION},
  {"dereference-command-line", no_ਦਲੀਲ, ਨਿਰਾ, 'H'},
  {"dereference-command-line-symlink-to-dir", no_ਦਲੀਲ, ਨਿਰਾ,
   DEREFERENCE_COMMAND_ਸਤਰ_SYMLINK_ਮੰਜ਼ਿਲ_DIR_OPTION},
  {"hide", required_ਦਲੀਲ, ਨਿਰਾ, HIDE_OPTION},
  {"ignore", required_ਦਲੀਲ, ਨਿਰਾ, 'I'},
  {"indicator-style", required_ਦਲੀਲ, ਨਿਰਾ, INDICATOR_STYLE_OPTION},
  {"dereference", no_ਦਲੀਲ, ਨਿਰਾ, 'L'},
  {"literal", no_ਦਲੀਲ, ਨਿਰਾ, 'N'},
  {"quote-name", no_ਦਲੀਲ, ਨਿਰਾ, 'Q'},
  {"quoting-style", required_ਦਲੀਲ, ਨਿਰਾ, QUOTING_STYLE_OPTION},
  {"recursive", no_ਦਲੀਲ, ਨਿਰਾ, 'R'},
  {"format", required_ਦਲੀਲ, ਨਿਰਾ, FORMAT_OPTION},
  {"show-control-chars", no_ਦਲੀਲ, ਨਿਰਾ, SHOW_CONTROL_CHARS_OPTION},
  {"sort", required_ਦਲੀਲ, ਨਿਰਾ, ਛਾਂਟੋ_OPTION},
  {"tabsize", required_ਦਲੀਲ, ਨਿਰਾ, 'T'},
  {"time", required_ਦਲੀਲ, ਨਿਰਾ, ਸਮਾਂ_OPTION},
  {"time-style", required_ਦਲੀਲ, ਨਿਰਾ, ਸਮਾਂ_STYLE_OPTION},
  {"zero", no_ਦਲੀਲ, ਨਿਰਾ, ZERO_OPTION},
  {"color", optional_ਦਲੀਲ, ਨਿਰਾ, COLOR_OPTION},
  {"hyperlink", optional_ਦਲੀਲ, ਨਿਰਾ, HYPERLINK_OPTION},
  {"block-size", required_ਦਲੀਲ, ਨਿਰਾ, ਖੰਡ_ਮਾਪ_OPTION},
  {"context", no_ਦਲੀਲ, ਨਿਰਾ, 'Z'},
  {"author", no_ਦਲੀਲ, ਨਿਰਾ, AUTHOR_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};

ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ਅਚਲ format_ਦਲੀਲਾਂ[] =
{
  "verbose", "long", "commas", "horizontal", "across",
  "vertical", "single-column", ਨਿਰਾ
};
ਸਥਿਰ ਗਿਣਤੀ format ਅਚਲ format_types[] =
{
  long_format, long_format, with_commas, horizontal, horizontal,
  many_ਪ੍ਰਤੀ_ਸਤਰ, one_ਪ੍ਰਤੀ_ਸਤਰ
};
ARGMATCH_VERIFY (format_ਦਲੀਲਾਂ, format_types);

ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ਅਚਲ ਛਾਂਟੋ_ਦਲੀਲਾਂ[] =
{
  "none", "size", "time", "version", "extension",
  "name", "width", ਨਿਰਾ
};
ਸਥਿਰ ਗਿਣਤੀ ਛਾਂਟੋ_type ਅਚਲ ਛਾਂਟੋ_types[] =
{
  ਛਾਂਟੋ_none, ਛਾਂਟੋ_ਮਾਪ, ਛਾਂਟੋ_ਸਮਾਂ, ਛਾਂਟੋ_ਸੰਸਕਰਣ, ਛਾਂਟੋ_extension,
  ਛਾਂਟੋ_name, ਛਾਂਟੋ_ਚੌੜਾਈ
};
ARGMATCH_VERIFY (ਛਾਂਟੋ_ਦਲੀਲਾਂ, ਛਾਂਟੋ_types);

ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ਅਚਲ ਸਮਾਂ_ਦਲੀਲਾਂ[] =
{
  "atime", "access", "use",
  "ctime", "status",
  "mtime", "modification",
  "birth", "creation",
  ਨਿਰਾ
};
ਸਥਿਰ ਗਿਣਤੀ ਸਮਾਂ_type ਅਚਲ ਸਮਾਂ_types[] =
{
  ਸਮਾਂ_atime, ਸਮਾਂ_atime, ਸਮਾਂ_atime,
  ਸਮਾਂ_ctime, ਸਮਾਂ_ctime,
  ਸਮਾਂ_mtime, ਸਮਾਂ_mtime,
  ਸਮਾਂ_btime, ਸਮਾਂ_btime,
};
ARGMATCH_VERIFY (ਸਮਾਂ_ਦਲੀਲਾਂ, ਸਮਾਂ_types);

ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ਅਚਲ when_ਦਲੀਲਾਂ[] =
{
  /* force and none are for compatibility with another color-ls version */
  "always", "yes", "force",
  "never", "no", "none",
  "auto", "tty", "if-tty", ਨਿਰਾ
};
ਸਥਿਰ ਗਿਣਤੀ when_type ਅਚਲ when_types[] =
{
  when_always, when_always, when_always,
  when_never, when_never, when_never,
  when_if_tty, when_if_tty, when_if_tty
};
ARGMATCH_VERIFY (when_ਦਲੀਲਾਂ, when_types);

/* Information about filling a column.  */
ਬਣਤਰ ਕਾਲਮ_info
{
  bool valid_len;
  size_t ਸਤਰ_len;
  size_t *ਖਾਨਾ_arr;
};

/* Array with information about column fullness.  */
ਸਥਿਰ ਬਣਤਰ ਕਾਲਮ_info *ਕਾਲਮ_info;

/* Maximum number of columns ever possible for this display.  */
ਸਥਿਰ size_t max_idx;

/* The minimum width of a non-final column is 3: 1 character for the name
   and 2 for the separating white space.  The final column is not padded.  */
ਗਿਣਤੀ { MIN_ਕਾਲਮ_ਚੌੜਾਈ = 3 };


/* This zero-based index is for the --dired option.  It is incremented
   for each byte of output generated by this program so that the beginning
   and ending indices (in that output) of every file name can be recorded
   and later output themselves.  */
ਸਥਿਰ off_t dired_pos;

ਸਥਿਰ ਖਾਲੀ
dired_outbyte (ਅੱਖਰ c)
{
  dired_pos++;
  ਅੱਖਰ_ਲਿਖੋ (c);
}

/* Output the buffer S, of length S_LEN, and increment DIRED_POS by S_LEN.  */
ਸਥਿਰ ਖਾਲੀ
dired_outbuf (ਅੱਖਰ ਅਚਲ *s, size_t s_len)
{
  dired_pos += s_len;
  fwrite (s, ਆਕਾਰ *s, s_len, stdout);
}

/* Output the string S, and increment DIRED_POS by its length.  */
ਸਥਿਰ ਖਾਲੀ
dired_outstring (ਅੱਖਰ ਅਚਲ *s)
{
  dired_outbuf (s, ਲੜੀ_ਲੰਬਾਈ (s));
}

ਸਥਿਰ ਖਾਲੀ
dired_indent (ਖਾਲੀ)
{
  ਜੇ (dired)
    dired_outstring ("  ");
}

/* With --dired, store pairs of beginning and ending indices of file names.  */
ਸਥਿਰ ਬਣਤਰ obstack dired_obstack;

/* With --dired, store pairs of beginning and ending indices of any
   directory names that appear as headers (just before 'total' line)
   for lists of directory entries.  Such directory names are seen when
   listing hierarchies using -R and when a directory is listed with at
   least one other command line argument.  */
ਸਥਿਰ ਬਣਤਰ obstack subdired_obstack;

/* Save the current index on the specified obstack, OBS.  */
ਸਥਿਰ ਖਾਲੀ
ਧੱਕੋ_current_dired_pos (ਬਣਤਰ obstack *obs)
{
  ਜੇ (dired)
    obstack_grow (obs, &dired_pos, ਆਕਾਰ dired_pos);
}

/* With -R, this stack is used to help detect directory cycles.
   The device/inode pairs on this stack mirror the pairs in the
   active_dir_set hash table.  */
ਸਥਿਰ ਬਣਤਰ obstack dev_ino_obstack;

/* Push a pair onto the device/inode stack.  */
ਸਥਿਰ ਖਾਲੀ
dev_ino_ਧੱਕੋ (dev_t dev, ino_t ino)
{
  ਖਾਲੀ *vdi;
  ਬਣਤਰ dev_ino *di;
  ਪੂਰਨ dev_ino_ਮਾਪ = ਆਕਾਰ *di;
  obstack_blank (&dev_ino_obstack, dev_ino_ਮਾਪ);
  vdi = obstack_ਅਗਲਾ_free (&dev_ino_obstack);
  di = vdi;
  di--;
  di->st_dev = dev;
  di->st_ino = ino;
}

/* Pop a dev/ino struct off the global dev_ino_obstack
   and return that struct.  */
ਸਥਿਰ ਬਣਤਰ dev_ino
dev_ino_ਕੱਢੋ (ਖਾਲੀ)
{
  ਖਾਲੀ *vdi;
  ਬਣਤਰ dev_ino *di;
  ਪੂਰਨ dev_ino_ਮਾਪ = ਆਕਾਰ *di;
  affirm (dev_ino_ਮਾਪ <= obstack_object_ਮਾਪ (&dev_ino_obstack));
  obstack_blank_fast (&dev_ino_obstack, -dev_ino_ਮਾਪ);
  vdi = obstack_ਅਗਲਾ_free (&dev_ino_obstack);
  di = vdi;
  ਵਾਪਸ *di;
}

ਸਥਿਰ ਖਾਲੀ
assert_matching_dev_ino (ਅੱਖਰ ਅਚਲ *name, ਬਣਤਰ dev_ino di)
{
  MAYBE_ਅਣਵਰਤਿਆ ਬਣਤਰ stat sb;
  assure (0 <= stat (name, &sb));
  assure (sb.st_dev == di.st_dev);
  assure (sb.st_ino == di.st_ino);
}

ਸਥਿਰ ਅੱਖਰ eolbyte = '\n';

/* Write to standard output PREFIX, followed by the quoting style and
   a space-separated list of the integers stored in OS all on one line.  */

ਸਥਿਰ ਖਾਲੀ
dired_dump_obstack (ਅੱਖਰ ਅਚਲ *ਅਗੇਤਰ, ਬਣਤਰ obstack *os)
{
  size_t n_pos;

  n_pos = obstack_object_ਮਾਪ (os) / ਆਕਾਰ (dired_pos);
  ਜੇ (n_pos > 0)
    {
      off_t *pos = obstack_ਅੰਤ (os);
      fputs (ਅਗੇਤਰ, stdout);
      ਲਈ (size_t i = 0; i < n_pos; i++)
        {
          intmax_t p = pos[i];
          ਲਿਖੋ (" %jd", p);
        }
      ਅੱਖਰ_ਲਿਖੋ ('\n');
    }
}

/* Return the platform birthtime member of the stat structure,
   or fallback to the mtime member, which we have populated
   from the statx structure or reset to an invalid timestamp
   where birth time is not supported.  */
ਸਥਿਰ ਬਣਤਰ timespec
get_stat_btime (ਬਣਤਰ stat ਅਚਲ *st)
{
  ਬਣਤਰ timespec btimespec;

#ਜੇ HAVE_STATX && defined STATX_INO
  btimespec = get_stat_mtime (st);
#ਨਹੀਂਤਾਂ
  btimespec = get_stat_birthtime (st);
#ਜੇ_ਅੰਤ

  ਵਾਪਸ btimespec;
}

#ਜੇ HAVE_STATX && defined STATX_INO
ATTRIBUTE_ਸ਼ੁੱਧ
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
ਸਮਾਂ_type_ਮੰਜ਼ਿਲ_statx (ਖਾਲੀ)
{
  ਚੋਣ (ਸਮਾਂ_type)
    {
    ਹਾਲਤ ਸਮਾਂ_ctime:
      ਵਾਪਸ STATX_CTIME;
    ਹਾਲਤ ਸਮਾਂ_mtime:
      ਵਾਪਸ STATX_MTIME;
    ਹਾਲਤ ਸਮਾਂ_atime:
      ਵਾਪਸ STATX_ATIME;
    ਹਾਲਤ ਸਮਾਂ_btime:
      ਵਾਪਸ STATX_BTIME;
    ਹਾਲਤ ਸਮਾਂ_numtypes: ਮੂਲ:
      unreachable ();
    }
    ਵਾਪਸ 0;
}

ATTRIBUTE_ਸ਼ੁੱਧ
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
calc_req_ਨਕਾਬ (ਖਾਲੀ)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਨਕਾਬ = STATX_ਬਹੁਲਕ;

  ਜੇ (print_inode)
    ਨਕਾਬ |= STATX_INO;

  ਜੇ (print_ਖੰਡ_ਮਾਪ)
    ਨਕਾਬ |= STATX_ਖੰਡਾਂ;

  ਜੇ (format == long_format) {
    ਨਕਾਬ |= STATX_NLINK | STATX_ਮਾਪ | ਸਮਾਂ_type_ਮੰਜ਼ਿਲ_statx ();
    ਜੇ (print_owner || print_author)
      ਨਕਾਬ |= STATX_UID;
    ਜੇ (print_ਸਮੂਹ)
      ਨਕਾਬ |= STATX_GID;
  }

  ਚੋਣ (ਛਾਂਟੋ_type)
    {
    ਹਾਲਤ ਛਾਂਟੋ_none:
    ਹਾਲਤ ਛਾਂਟੋ_name:
    ਹਾਲਤ ਛਾਂਟੋ_ਸੰਸਕਰਣ:
    ਹਾਲਤ ਛਾਂਟੋ_extension:
    ਹਾਲਤ ਛਾਂਟੋ_ਚੌੜਾਈ:
      ਰੋਕੋ;
    ਹਾਲਤ ਛਾਂਟੋ_ਸਮਾਂ:
      ਨਕਾਬ |= ਸਮਾਂ_type_ਮੰਜ਼ਿਲ_statx ();
      ਰੋਕੋ;
    ਹਾਲਤ ਛਾਂਟੋ_ਮਾਪ:
      ਨਕਾਬ |= STATX_ਮਾਪ;
      ਰੋਕੋ;
    ਹਾਲਤ ਛਾਂਟੋ_numtypes: ਮੂਲ:
      unreachable ();
    }

  ਵਾਪਸ ਨਕਾਬ;
}

ਸਥਿਰ ਪੂਰਨ
do_statx (ਪੂਰਨ fd, ਅੱਖਰ ਅਚਲ *name, ਬਣਤਰ stat *st, ਪੂਰਨ flags,
          ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਨਕਾਬ)
{
  ਬਣਤਰ statx stx;
  bool want_btime = ਨਕਾਬ & STATX_BTIME;
  ਪੂਰਨ ret = statx (fd, name, flags | AT_NO_AUTOMOUNT, ਨਕਾਬ, &stx);
  ਜੇ (ret >= 0)
    {
      statx_ਮੰਜ਼ਿਲ_stat (&stx, st);
      /* Since we only need one timestamp type,
         store birth time in st_mtim.  */
      ਜੇ (want_btime)
        {
          ਜੇ (stx.stx_ਨਕਾਬ & STATX_BTIME)
            st->st_mtim = statx_timestamp_ਮੰਜ਼ਿਲ_timespec (stx.stx_btime);
          ਨਹੀਂਤਾਂ
            st->st_mtim.tv_sec = st->st_mtim.tv_nsec = -1;
        }
    }

  ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ
do_stat (ਅੱਖਰ ਅਚਲ *name, ਬਣਤਰ stat *st)
{
  ਵਾਪਸ do_statx (AT_FDCWD, name, st, 0, calc_req_ਨਕਾਬ ());
}

ਸਥਿਰ ਪੂਰਨ
do_lstat (ਅੱਖਰ ਅਚਲ *name, ਬਣਤਰ stat *st)
{
  ਵਾਪਸ do_statx (AT_FDCWD, name, st, AT_SYMLINK_NOFOLLOW, calc_req_ਨਕਾਬ ());
}

ਸਥਿਰ ਪੂਰਨ
stat_for_ਬਹੁਲਕ (ਅੱਖਰ ਅਚਲ *name, ਬਣਤਰ stat *st)
{
  ਵਾਪਸ do_statx (AT_FDCWD, name, st, 0, STATX_ਬਹੁਲਕ);
}

/* dev+ino should be static, so no need to sync with backing store */
ਸਥਿਰ ਪੂਰਨ
stat_for_ino (ਅੱਖਰ ਅਚਲ *name, ਬਣਤਰ stat *st)
{
  ਵਾਪਸ do_statx (AT_FDCWD, name, st, 0, STATX_INO);
}

ਸਥਿਰ ਪੂਰਨ
fstat_for_ino (ਪੂਰਨ fd, ਬਣਤਰ stat *st)
{
  ਵਾਪਸ do_statx (fd, "", st, AT_ਖਾਲੀ_ਥਾਵਾਂ_ਰਸਤਾ, STATX_INO);
}
#ਨਹੀਂਤਾਂ
ਸਥਿਰ ਪੂਰਨ
do_stat (ਅੱਖਰ ਅਚਲ *name, ਬਣਤਰ stat *st)
{
  ਵਾਪਸ stat (name, st);
}

ਸਥਿਰ ਪੂਰਨ
do_lstat (ਅੱਖਰ ਅਚਲ *name, ਬਣਤਰ stat *st)
{
  ਵਾਪਸ lstat (name, st);
}

ਸਥਿਰ ਪੂਰਨ
stat_for_ਬਹੁਲਕ (ਅੱਖਰ ਅਚਲ *name, ਬਣਤਰ stat *st)
{
  ਵਾਪਸ stat (name, st);
}

ਸਥਿਰ ਪੂਰਨ
stat_for_ino (ਅੱਖਰ ਅਚਲ *name, ਬਣਤਰ stat *st)
{
  ਵਾਪਸ stat (name, st);
}

ਸਥਿਰ ਪੂਰਨ
fstat_for_ino (ਪੂਰਨ fd, ਬਣਤਰ stat *st)
{
  ਵਾਪਸ fstat (fd, st);
}
#ਜੇ_ਅੰਤ

/* Return the address of the first plain %b spec in FMT, or NULL if
   there is no such spec.  %5b etc. do not match, so that user
   widths/flags are honored.  */

ATTRIBUTE_ਸ਼ੁੱਧ
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *
ਪਹਿਲਾ_percent_b (ਅੱਖਰ ਅਚਲ *fmt)
{
  ਲਈ (; *fmt; fmt++)
    ਜੇ (fmt[0] == '%')
      ਚੋਣ (fmt[1])
        {
        ਹਾਲਤ 'b': ਵਾਪਸ fmt;
        ਹਾਲਤ '%': fmt++; ਰੋਕੋ;
        }
  ਵਾਪਸ ਨਿਰਾ;
}

ਸਥਿਰ ਅੱਖਰ RFC3986[256];
ਸਥਿਰ ਖਾਲੀ
file_escape_init (ਖਾਲੀ)
{
  ਲਈ (ਪੂਰਨ i = 0; i < 256; i++)
    RFC3986[i] |= c_isalnum (i) || i == '~' || i == '-' || i == '.' || i == '_';
}

ਗਿਣਤੀ { MBSWIDTH_FLAGS = MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE };

/* Read the abbreviated month names from the locale, to align them
   and to determine the max width of the field and to truncate names
   greater than our max allowed.
   Note even though this handles multibyte locales correctly
   it's not restricted to them as single byte locales can have
   variable width abbreviated months and also precomputing/caching
   the names was seen to increase the performance of ls significantly.  */

/* abformat[RECENT][MON] is the format to use for timestamps with
   recentness RECENT and month MON.  */
ਗਿਣਤੀ { ABFORMAT_ਮਾਪ = 128 };
ਸਥਿਰ ਅੱਖਰ abformat[2][12][ABFORMAT_ਮਾਪ];
/* True if precomputed formats should be used.  This can be false if
   nl_langinfo fails, if a format or month abbreviation is unusually
   long, or if a month abbreviation contains '%'.  */
ਸਥਿਰ bool use_abformat;

/* Store into ABMON the abbreviated month names, suitably aligned.
   Return true if successful.  */

ਸਥਿਰ bool
abmon_init (ਅੱਖਰ abmon[12][ABFORMAT_ਮਾਪ])
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_NL_LANGINFO
  ਵਾਪਸ false;
#ਨਹੀਂਤਾਂ
  ਪੂਰਨ max_mon_ਚੌੜਾਈ = 0;
  ਪੂਰਨ mon_ਚੌੜਾਈ[12];
  ਪੂਰਨ mon_len[12];

  ਲਈ (ਪੂਰਨ i = 0; i < 12; i++)
    {
      ਅੱਖਰ ਅਚਲ *abbr = nl_langinfo (ABMON_1 + i);
      mon_len[i] = strnlen (abbr, ABFORMAT_ਮਾਪ);
      ਜੇ (mon_len[i] == ABFORMAT_ਮਾਪ)
        ਵਾਪਸ false;
      ਜੇ (strchr (abbr, '%'))
        ਵਾਪਸ false;
      mon_ਚੌੜਾਈ[i] = mbswidth (strcpy (abmon[i], abbr), MBSWIDTH_FLAGS);
      ਜੇ (mon_ਚੌੜਾਈ[i] < 0)
        ਵਾਪਸ false;
      max_mon_ਚੌੜਾਈ = MAX (max_mon_ਚੌੜਾਈ, mon_ਚੌੜਾਈ[i]);
    }

  ਲਈ (ਪੂਰਨ i = 0; i < 12; i++)
    {
      ਪੂਰਨ fill = max_mon_ਚੌੜਾਈ - mon_ਚੌੜਾਈ[i];
      ਜੇ (ABFORMAT_ਮਾਪ - mon_len[i] <= fill)
        ਵਾਪਸ false;
      bool align_ਖੱਬਾ = !c_isdigit (abmon[i][0]);
      ਪੂਰਨ fill_ਵਿੱਥ;
      ਜੇ (align_ਖੱਬਾ)
        fill_ਵਿੱਥ = mon_len[i];
      ਨਹੀਂਤਾਂ
        {
          memmove (abmon[i] + fill, abmon[i], mon_len[i]);
          fill_ਵਿੱਥ = 0;
        }
      memset (abmon[i] + fill_ਵਿੱਥ, ' ', fill);
      abmon[i][mon_len[i] + fill] = '\0';
    }

  ਵਾਪਸ true;
#ਜੇ_ਅੰਤ
}

/* Initialize ABFORMAT and USE_ABFORMAT.  */

ਸਥਿਰ ਖਾਲੀ
abformat_init (ਖਾਲੀ)
{
  ਅੱਖਰ ਅਚਲ *pb[2];
  ਲਈ (ਪੂਰਨ recent = 0; recent < 2; recent++)
    pb[recent] = ਪਹਿਲਾ_percent_b (long_ਸਮਾਂ_format[recent]);
  ਜੇ (! (pb[0] || pb[1]))
    ਵਾਪਸ;

  ਅੱਖਰ abmon[12][ABFORMAT_ਮਾਪ];
  ਜੇ (! abmon_init (abmon))
    ਵਾਪਸ;

  ਲਈ (ਪੂਰਨ recent = 0; recent < 2; recent++)
    {
      ਅੱਖਰ ਅਚਲ *fmt = long_ਸਮਾਂ_format[recent];
      ਲਈ (ਪੂਰਨ i = 0; i < 12; i++)
        {
          ਅੱਖਰ *nfmt = abformat[recent][i];
          ਪੂਰਨ nbytes;

          ਜੇ (! pb[recent])
            nbytes = snprintf (nfmt, ABFORMAT_ਮਾਪ, "%s", fmt);
          ਨਹੀਂਤਾਂ
            {
              ਜੇ (! (pb[recent] - fmt <= MIN (ABFORMAT_ਮਾਪ, ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ)))
                ਵਾਪਸ;
              ਪੂਰਨ ਅਗੇਤਰ_len = pb[recent] - fmt;
              nbytes = snprintf (nfmt, ABFORMAT_ਮਾਪ, "%.*s%s%s",
                                 ਅਗੇਤਰ_len, fmt, abmon[i], pb[recent] + 2);
            }

          ਜੇ (! (0 <= nbytes && nbytes < ABFORMAT_ਮਾਪ))
            ਵਾਪਸ;
        }
    }

  use_abformat = true;
}

ਸਥਿਰ size_t
dev_ino_hash (ਖਾਲੀ ਅਚਲ *x, size_t ਸਾਰਣੀ_ਮਾਪ)
{
  ਬਣਤਰ dev_ino ਅਚਲ *p = x;
  ਵਾਪਸ (uintmax_t) {p->st_ino} % ਸਾਰਣੀ_ਮਾਪ;
}

ਸਥਿਰ bool
dev_ino_compare (ਖਾਲੀ ਅਚਲ *x, ਖਾਲੀ ਅਚਲ *y)
{
  ਬਣਤਰ dev_ino ਅਚਲ *a = x;
  ਬਣਤਰ dev_ino ਅਚਲ *b = y;
  ਵਾਪਸ PSAME_INODE (a, b);
}

ਸਥਿਰ ਖਾਲੀ
dev_ino_free (ਖਾਲੀ *x)
{
  ਮੁਕਤ (x);
}

/* Add the device/inode pair (P->st_dev/P->st_ino) to the set of
   active directories.  Return true if there is already a matching
   entry in the table.  */

ਸਥਿਰ bool
ਦੇਖੋ_dir (dev_t dev, ino_t ino)
{
  ਬਣਤਰ dev_ino *ent;
  ਬਣਤਰ dev_ino *ent_from_ਸਾਰਣੀ;
  bool ਮਿਲਿਆ_ਮੇਲ;

  ent = xmalloc (ਆਕਾਰ *ent);
  ent->st_ino = ino;
  ent->st_dev = dev;

  /* Attempt to insert this entry into the table.  */
  ent_from_ਸਾਰਣੀ = hash_ਪਾਓ (ਸਰਗਰਮ_dir_ਸੈੱਟ, ent);

  ਜੇ (ent_from_ਸਾਰਣੀ == ਨਿਰਾ)
    {
      /* Insertion failed due to lack of memory.  */
      xalloc_die ();
    }

  ਮਿਲਿਆ_ਮੇਲ = (ent_from_ਸਾਰਣੀ != ent);

  ਜੇ (ਮਿਲਿਆ_ਮੇਲ)
    {
      /* ent was not inserted, so free it.  */
      ਮੁਕਤ (ent);
    }

  ਵਾਪਸ ਮਿਲਿਆ_ਮੇਲ;
}

ਸਥਿਰ ਖਾਲੀ
free_pending_ent (ਬਣਤਰ pending *p)
{
  ਮੁਕਤ (p->name);
  ਮੁਕਤ (p->realname);
  ਮੁਕਤ (p);
}

ਸਥਿਰ bool
is_colored (ਗਿਣਤੀ indicator_no type)
{
  /* Return true unless the string is "", "0" or "00"; try to be efficient.  */
  size_t len = color_indicator[type].len;
  ਜੇ (len == 0)
    ਵਾਪਸ false;
  ਜੇ (2 < len)
    ਵਾਪਸ true;
  ਅੱਖਰ ਅਚਲ *s = color_indicator[type].string;
  ਵਾਪਸ (s[0] != '0') | (s[len - 1] != '0');
}

ਸਥਿਰ ਖਾਲੀ
restore_default_color (ਖਾਲੀ)
{
  put_indicator (&color_indicator[C_ਖੱਬਾ]);
  put_indicator (&color_indicator[C_ਸੱਜਾ]);
}

ਸਥਿਰ ਖਾਲੀ
ਸੈੱਟ_ਸਾਧਾਰਨ_color (ਖਾਲੀ)
{
  ਜੇ (print_with_color && is_colored (C_NORM))
    {
      put_indicator (&color_indicator[C_ਖੱਬਾ]);
      put_indicator (&color_indicator[C_NORM]);
      put_indicator (&color_indicator[C_ਸੱਜਾ]);
    }
}

/* An ordinary signal was received; arrange for the program to exit.  */

ਸਥਿਰ ਖਾਲੀ
sighandler (ਪੂਰਨ sig)
{
  ਜੇ (! ਰੁਕਾਵਟ_signal)
    ਰੁਕਾਵਟ_signal = sig;
}

/* A SIGTSTP was received; arrange for the program to suspend itself.  */

ਸਥਿਰ ਖਾਲੀ
stophandler (MAYBE_ਅਣਵਰਤਿਆ ਪੂਰਨ sig)
{
  ਜੇ (! ਰੁਕਾਵਟ_signal)
    stop_signal_ਗਿਣਤੀ++;
}

/* Process any pending signals.  If signals are caught, this function
   should be called periodically.  Ideally there should never be an
   unbounded amount of time when signals are not being processed.
   Signal handling can restore the default colors, so callers must
   immediately change colors after invoking this function.  */

ਸਥਿਰ ਖਾਲੀ
process_ਸੰਕੇਤ (ਖਾਲੀ)
{
  ਜਦੋਂ (ਰੁਕਾਵਟ_signal || stop_signal_ਗਿਣਤੀ)
    {
      ਪੂਰਨ sig;
      ਪੂਰਨ stops;
      sigset_t oldset;

      ਜੇ (ਵਰਤਿਆ_color)
        restore_default_color ();
      fflush (stdout);

      sigprocmask (SIG_BLOCK, &caught_ਸੰਕੇਤ, &oldset);

      /* Reload interrupt_signal and stop_signal_count, in case a new
         signal was handled before sigprocmask took effect.  */
      sig = ਰੁਕਾਵਟ_signal;
      stops = stop_signal_ਗਿਣਤੀ;

      /* SIGTSTP is special, since the application can receive that signal
         more than once.  In this case, don't set the signal handler to the
         default.  Instead, just raise the uncatchable SIGSTOP.  */
      ਜੇ (stops)
        {
          stop_signal_ਗਿਣਤੀ = stops - 1;
          sig = SIGSTOP;
        }
      ਨਹੀਂਤਾਂ
        ਸੰਕੇਤ (sig, SIG_DFL);

      /* Exit or suspend the program.  */
      ਉਠਾਓ (sig);
      sigprocmask (SIG_SETMASK, &oldset, ਨਿਰਾ);

      /* If execution reaches here, then the program has been
         continued (after being suspended).  */
    }
}

/* Setup signal handlers if INIT is true,
   otherwise restore to the default.  */

ਸਥਿਰ ਖਾਲੀ
signal_ਤਿਆਰੀ (bool init)
{
  /* The signals that are trapped, and the number of such signals.  */
  ਸਥਿਰ ਪੂਰਨ ਅਚਲ stop_sig[] =
    {
      /* This one is handled specially.  */
      SIGTSTP,
    };

  ਗਿਣਤੀ { nsigs = countof (ਪਦ_sig), nstop = countof (stop_sig) };

  ਜੇ (init)
    {
      ਬਣਤਰ sigaction act;

      sigemptyset (&caught_ਸੰਕੇਤ);
      ਲਈ (ਪੂਰਨ j = 0; j < nsigs + nstop; j++)
        {
          ਪੂਰਨ sig = j < nsigs ? ਪਦ_sig[j]: stop_sig[j - nsigs];
          sigaction (sig, ਨਿਰਾ, &act);
          ਜੇ (act.sa_handler != SIG_IGN)
            sigaddset (&caught_ਸੰਕੇਤ, sig);
        }

      act.sa_mask = caught_ਸੰਕੇਤ;
      act.sa_flags = SA_RESTART;

      ਲਈ (ਪੂਰਨ j = 0; j < nsigs + nstop; j++)
        {
          ਪੂਰਨ sig = j < nsigs ? ਪਦ_sig[j]: stop_sig[j - nsigs];
          ਜੇ (sigismember (&caught_ਸੰਕੇਤ, sig))
            {
              act.sa_handler = j < nsigs ? sighandler : stophandler;
              sigaction (sig, &act, ਨਿਰਾ);
            }
        }
    }
  ਨਹੀਂਤਾਂ /* restore.  */
    {
      ਲਈ (ਪੂਰਨ j = 0; j < nsigs + nstop; j++)
        {
          ਪੂਰਨ sig = j < nsigs ? ਪਦ_sig[j]: stop_sig[j - nsigs];
          ਜੇ (sigismember (&caught_ਸੰਕੇਤ, sig))
            ਸੰਕੇਤ (sig, SIG_DFL);
        }
    }
}

ਸਥਿਰ ਖਾਲੀ
signal_init (ਖਾਲੀ)
{
  signal_ਤਿਆਰੀ (true);
}

ਸਥਿਰ ਖਾਲੀ
signal_restore (ਖਾਲੀ)
{
  signal_ਤਿਆਰੀ (false);
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਪੂਰਨ i;
  ਬਣਤਰ pending *thispend;
  ਪੂਰਨ n_files;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (LS_FAILURE);
  atexit (close_stdout);

  static_assert (countof (color_indicator) == countof (indicator_name));

  exit_ਸਥਿਤੀ = EXIT_SUCCESS;
  print_dir_name = true;
  pending_dirs = ਨਿਰਾ;

  current_ਸਮਾਂ.tv_sec = TYPE_MINIMUM (time_t);
  current_ਸਮਾਂ.tv_nsec = -1;

  i = decode_switches (argc, argv);

  ਜੇ (print_with_color)
    parse_ls_color ();

  /* Test print_with_color again, because the call to parse_ls_color
     may have just reset it -- e.g., if LS_COLORS is invalid.  */

  ਜੇ (print_with_color)
    {
      /* Don't use TAB characters in output.  Some terminal
         emulators can't handle the combination of tabs and
         color codes on the same line.  */
      tabsize = 0;
    }

  ਜੇ (directories_ਪਹਿਲਾ)
    check_symlink_ਬਹੁਲਕ = true;
  ਨਹੀਂਤਾਂ ਜੇ (print_with_color)
    {
      /* Avoid following symbolic links when possible.  */
      ਜੇ (is_colored (C_ORPHAN)
          || (is_colored (C_EXEC) && color_symlink_as_referent)
          || (is_colored (C_MISSING) && format == long_format))
        check_symlink_ਬਹੁਲਕ = true;
    }

  ਜੇ (dereference == DEREF_UNDEFINED)
    dereference = ((ਤੁਰੰਤ_dirs
                    || indicator_style == classify
                    || format == long_format)
                   ? DEREF_NEVER
                   : DEREF_COMMAND_ਸਤਰ_SYMLINK_ਮੰਜ਼ਿਲ_DIR);

  /* When using -R, initialize a data structure we'll use to
     detect any directory cycles.  */
  ਜੇ (recursive)
    {
      ਸਰਗਰਮ_dir_ਸੈੱਟ = hash_initialize (INITIAL_ਸਾਰਣੀ_ਮਾਪ, ਨਿਰਾ,
                                        dev_ino_hash,
                                        dev_ino_compare,
                                        dev_ino_free);
      ਜੇ (ਸਰਗਰਮ_dir_ਸੈੱਟ == ਨਿਰਾ)
        xalloc_die ();

      obstack_init (&dev_ino_obstack);
    }

  localtz = tzalloc (getenv ("TZ"));

  format_needs_stat = ((ਛਾਂਟੋ_type == ਛਾਂਟੋ_ਸਮਾਂ) | (ਛਾਂਟੋ_type == ਛਾਂਟੋ_ਮਾਪ)
                       | (format == long_format)
                       | print_ਖੰਡ_ਮਾਪ | print_hyperlink | print_scontext);
  format_needs_type = ((! format_needs_stat)
                       & (recursive | print_with_color | print_scontext
                          | directories_ਪਹਿਲਾ
                          | (indicator_style != none)));
  format_needs_capability = print_with_color && is_colored (C_CAP);

  ਜੇ (dired)
    {
      obstack_init (&dired_obstack);
      obstack_init (&subdired_obstack);
    }

  ਜੇ (print_hyperlink)
    {
      file_escape_init ();

      hostname = xgethostname ();
      /* The hostname is generally ignored,
         so ignore failures obtaining it.  */
      ਜੇ (! hostname)
        hostname = "";
    }

  cwd_n_alloc = 100;
  cwd_file = xmalloc (cwd_n_alloc * ਆਕਾਰ *cwd_file);
  cwd_n_ਵਰਤਿਆ = 0;

  clear_files ();

  n_files = argc - i;

  ਜੇ (n_files <= 0)
    {
      ਜੇ (ਤੁਰੰਤ_dirs)
        gobble_file (".", directory, NOT_AN_INODE_ਸੰਖਿਆ, true, ਨਿਰਾ);
      ਨਹੀਂਤਾਂ
        ਕਤਾਰ_directory (".", ਨਿਰਾ, true);
    }
  ਨਹੀਂਤਾਂ
    ਕਰੋ
      gobble_file (argv[i++], unknown, NOT_AN_INODE_ਸੰਖਿਆ, true, ਨਿਰਾ);
    ਜਦੋਂ (i < argc);

  ਜੇ (cwd_n_ਵਰਤਿਆ)
    {
      ਛਾਂਟੋ_files ();
      ਜੇ (!ਤੁਰੰਤ_dirs)
        ਕੱਢੋ_dirs_from_files (ਨਿਰਾ, true);
      /* 'cwd_n_used' might be zero now.  */
    }

  /* In the following if/else blocks, it is sufficient to test 'pending_dirs'
     (and not pending_dirs->name) because there may be no markers in the queue
     at this point.  A marker may be enqueued when extract_dirs_from_files is
     called with a non-empty string or via print_dir.  */
  ਜੇ (cwd_n_ਵਰਤਿਆ)
    {
      print_current_files ();
      ਜੇ (pending_dirs)
        dired_outbyte ('\n');
    }
  ਨਹੀਂਤਾਂ ਜੇ (n_files <= 1 && pending_dirs && !pending_dirs->ਅਗਲਾ)
    print_dir_name = false;

  ਜਦੋਂ (pending_dirs)
    {
      thispend = pending_dirs;
      pending_dirs = pending_dirs->ਅਗਲਾ;

      ਜੇ (LOOP_DETECT)
        {
          ਜੇ (thispend->name == ਨਿਰਾ)
            {
              /* thispend->name == NULL means this is a marker entry
                 indicating we've finished processing the directory.
                 Use its dev/ino numbers to remove the corresponding
                 entry from the active_dir_set hash table.  */
              ਬਣਤਰ dev_ino di = dev_ino_ਕੱਢੋ ();
              ਬਣਤਰ dev_ino *ਮਿਲਿਆ = hash_remove (ਸਰਗਰਮ_dir_ਸੈੱਟ, &di);
              ਜੇ (false)
                assert_matching_dev_ino (thispend->realname, di);
              affirm (ਮਿਲਿਆ);
              dev_ino_free (ਮਿਲਿਆ);
              free_pending_ent (thispend);
              ਜਾਰੀ;
            }
        }

      print_dir (thispend->name, thispend->realname,
                 thispend->command_ਸਤਰ_arg);

      free_pending_ent (thispend);
      print_dir_name = true;
    }

  ਜੇ (print_with_color && ਵਰਤਿਆ_color)
    {

      /* Skip the restore when it would be a no-op, i.e.,
         when left is "\033[" and right is "m".  */
      ਜੇ (!(color_indicator[C_ਖੱਬਾ].len == 2
            && memeq (color_indicator[C_ਖੱਬਾ].string, "\033[", 2)
            && color_indicator[C_ਸੱਜਾ].len == 1
            && color_indicator[C_ਸੱਜਾ].string[0] == 'm'))
        restore_default_color ();

      fflush (stdout);

      signal_restore ();

      /* Act on any signals that arrived before the default was restored.
         This can process signals out of order, but there doesn't seem to
         be an easy way to do them in order, and the order isn't that
         important anyway.  */
      ਲਈ (ਪੂਰਨ j = stop_signal_ਗਿਣਤੀ; j; j--)
        ਉਠਾਓ (SIGSTOP);
      ਪੂਰਨ j = ਰੁਕਾਵਟ_signal;
      ਜੇ (j)
        ਉਠਾਓ (j);
    }

  ਜੇ (dired)
    {
      /* No need to free these since we're about to exit.  */
      dired_dump_obstack ("//DIRED//", &dired_obstack);
      dired_dump_obstack ("//SUBDIRED//", &subdired_obstack);
      ਲਿਖੋ ("//DIRED-OPTIONS// --quoting-style=%s\n",
              quoting_style_ਦਲੀਲਾਂ[get_quoting_style (filename_quoting_options)]);
    }

  ਜੇ (LOOP_DETECT)
    {
      assure (hash_get_n_entries (ਸਰਗਰਮ_dir_ਸੈੱਟ) == 0);
      hash_free (ਸਰਗਰਮ_dir_ਸੈੱਟ);
    }

  ਵਾਪਸ exit_ਸਥਿਤੀ;
}

/* Return the line length indicated by the value given by SPEC, or -1
   if unsuccessful.  0 means no limit on line length.  */

ਸਥਿਰ ptrdiff_t
decode_ਸਤਰ_ਲੰਮਾਈ (ਅੱਖਰ ਅਚਲ *spec)
{
  uintmax_t val;

  /* Treat too-large values as if they were 0, which is
     effectively infinity.  */
  ਚੋਣ (xstrtoumax (spec, ਨਿਰਾ, 0, &val, ""))
    {
    ਹਾਲਤ LONGINT_OK:
      ਵਾਪਸ val <= MIN (PTRDIFF_MAX, SIZE_MAX) ? val : 0;

    ਹਾਲਤ LONGINT_ਉਛਾਲ:
      ਵਾਪਸ 0;

    ਹਾਲਤ LONGINT_INVALID:
    ਹਾਲਤ LONGINT_INVALID_SUFFIX_CHAR:
    ਹਾਲਤ LONGINT_INVALID_SUFFIX_CHAR_WITH_ਉਛਾਲ:
      ਵਾਪਸ -1;

    ਮੂਲ:
      unreachable ();
    }
}

/* Return true if standard output is a tty, caching the result.  */

ਸਥਿਰ bool
stdout_isatty (ਖਾਲੀ)
{
  ਸਥਿਰ ਨਿਸ਼ਾਨੀਵਾਲਾ ਅੱਖਰ ਬਾਹਰਲਾ_tty = -1;
  ਜੇ (ਬਾਹਰਲਾ_tty < 0)
    ਬਾਹਰਲਾ_tty = isatty (STDOUT_FILENO);
  assume (ਬਾਹਰਲਾ_tty == 0 || ਬਾਹਰਲਾ_tty == 1);
  ਵਾਪਸ ਬਾਹਰਲਾ_tty;
}

/* Set all the option flags according to the switches specified.
   Return the index of the first non-option argument.  */

ਸਥਿਰ ਪੂਰਨ
decode_switches (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਅੱਖਰ *ਸਮਾਂ_style_option = ਨਿਰਾ;

  /* These variables are false or -1 unless a switch says otherwise.  */
  bool kibibytes_specified = false;
  ਪੂਰਨ format_opt = -1;
  ਪੂਰਨ hide_control_chars_opt = -1;
  ਪੂਰਨ quoting_style_opt = -1;
  ਪੂਰਨ ਛਾਂਟੋ_opt = -1;
  ptrdiff_t tabsize_opt = -1;
  ptrdiff_t ਚੌੜਾਈ_opt = -1;

  ਜਦੋਂ (true)
    {
      ਪੂਰਨ oi = -1;
      ਪੂਰਨ c = getopt_long (argc, argv,
                           "abcdfghiklmnopqrstuvw:xABCDFGHI:LNQRST:UXZ1",
                           long_options, &oi);
      ਜੇ (c == -1)
        ਰੋਕੋ;

      ਚੋਣ (c)
        {
        ਹਾਲਤ 'a':
          ignore_ਬਹੁਲਕ = IGNORE_ਨਿਊਨਤਮ;
          ਰੋਕੋ;

        ਹਾਲਤ 'b':
          quoting_style_opt = escape_quoting_style;
          ਰੋਕੋ;

        ਹਾਲਤ 'c':
          ਸਮਾਂ_type = ਸਮਾਂ_ctime;
          explicit_ਸਮਾਂ = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'd':
          ਤੁਰੰਤ_dirs = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'f':
          ignore_ਬਹੁਲਕ = IGNORE_ਨਿਊਨਤਮ; /* enable -a */
          ਛਾਂਟੋ_opt = ਛਾਂਟੋ_none;         /* enable -U */
          ਰੋਕੋ;

        ਹਾਲਤ FILE_TYPE_INDICATOR_OPTION: /* --file-type */
          indicator_style = file_type;
          ਰੋਕੋ;

        ਹਾਲਤ 'g':
          format_opt = long_format;
          print_owner = false;
          ਰੋਕੋ;

        ਹਾਲਤ 'h':
          file_human_output_opts = human_output_opts =
            human_autoscale | human_SI | human_ਅਧਾਰ_1024;
          file_output_ਖੰਡ_ਮਾਪ = output_ਖੰਡ_ਮਾਪ = 1;
          ਰੋਕੋ;

        ਹਾਲਤ 'i':
          print_inode = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'k':
          kibibytes_specified = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'l':
          format_opt = long_format;
          ਰੋਕੋ;

        ਹਾਲਤ 'm':
          format_opt = with_commas;
          ਰੋਕੋ;

        ਹਾਲਤ 'n':
          numeric_ids = true;
          format_opt = long_format;
          ਰੋਕੋ;

        ਹਾਲਤ 'o':  /* Just like -l, but don't display group info.  */
          format_opt = long_format;
          print_ਸਮੂਹ = false;
          ਰੋਕੋ;

        ਹਾਲਤ 'p':
          indicator_style = slash;
          ਰੋਕੋ;

        ਹਾਲਤ 'q':
          hide_control_chars_opt = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'r':
          ਛਾਂਟੋ_ਉਲਟ = true;
          ਰੋਕੋ;

        ਹਾਲਤ 's':
          print_ਖੰਡ_ਮਾਪ = true;
          ਰੋਕੋ;

        ਹਾਲਤ 't':
          ਛਾਂਟੋ_opt = ਛਾਂਟੋ_ਸਮਾਂ;
          ਰੋਕੋ;

        ਹਾਲਤ 'u':
          ਸਮਾਂ_type = ਸਮਾਂ_atime;
          explicit_ਸਮਾਂ = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'v':
          ਛਾਂਟੋ_opt = ਛਾਂਟੋ_ਸੰਸਕਰਣ;
          ਰੋਕੋ;

        ਹਾਲਤ 'w':
          ਚੌੜਾਈ_opt = decode_ਸਤਰ_ਲੰਮਾਈ (optarg);
          ਜੇ (ਚੌੜਾਈ_opt < 0)
            error (LS_FAILURE, 0, "%s: %s", _("invalid line width"),
                   quote (optarg));
          ਰੋਕੋ;

        ਹਾਲਤ 'x':
          format_opt = horizontal;
          ਰੋਕੋ;

        ਹਾਲਤ 'A':
          ignore_ਬਹੁਲਕ = IGNORE_DOT_AND_DOTDOT;
          ਰੋਕੋ;

        ਹਾਲਤ 'B':
          ਜੋੜੋ_ignore_ਨਮੂਨਾ ("*~");
          ਜੋੜੋ_ignore_ਨਮੂਨਾ (".*~");
          ਰੋਕੋ;

        ਹਾਲਤ 'C':
          format_opt = many_ਪ੍ਰਤੀ_ਸਤਰ;
          ਰੋਕੋ;

        ਹਾਲਤ 'D':
          format_opt = long_format;
          print_hyperlink = false;
          dired = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'F':
          {
            ਪੂਰਨ i;
            ਜੇ (optarg)
              i = XARGMATCH ("--classify", optarg, when_ਦਲੀਲਾਂ, when_types);
            ਨਹੀਂਤਾਂ
              /* Using --classify with no argument is equivalent to using
                 --classify=always.  */
              i = when_always;

            ਜੇ (i == when_always || (i == when_if_tty && stdout_isatty ()))
              indicator_style = classify;
            ਰੋਕੋ;
          }

        ਹਾਲਤ 'G':		/* inhibit display of group info */
          print_ਸਮੂਹ = false;
          ਰੋਕੋ;

        ਹਾਲਤ 'H':
          dereference = DEREF_COMMAND_ਸਤਰ_ARGUMENTS;
          ਰੋਕੋ;

        ਹਾਲਤ DEREFERENCE_COMMAND_ਸਤਰ_SYMLINK_ਮੰਜ਼ਿਲ_DIR_OPTION:
          dereference = DEREF_COMMAND_ਸਤਰ_SYMLINK_ਮੰਜ਼ਿਲ_DIR;
          ਰੋਕੋ;

        ਹਾਲਤ 'I':
          ਜੋੜੋ_ignore_ਨਮੂਨਾ (optarg);
          ਰੋਕੋ;

        ਹਾਲਤ 'L':
          dereference = DEREF_ALWAYS;
          ਰੋਕੋ;

        ਹਾਲਤ 'N':
          quoting_style_opt = literal_quoting_style;
          ਰੋਕੋ;

        ਹਾਲਤ 'Q':
          quoting_style_opt = c_quoting_style;
          ਰੋਕੋ;

        ਹਾਲਤ 'R':
          recursive = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'S':
          ਛਾਂਟੋ_opt = ਛਾਂਟੋ_ਮਾਪ;
          ਰੋਕੋ;

        ਹਾਲਤ 'T':
          tabsize_opt = xnumtoumax (optarg, 0, 0, MIN (PTRDIFF_MAX, SIZE_MAX),
                                    "", _("invalid tab size"), LS_FAILURE, 0);
          ਰੋਕੋ;

        ਹਾਲਤ 'U':
          ਛਾਂਟੋ_opt = ਛਾਂਟੋ_none;
          ਰੋਕੋ;

        ਹਾਲਤ 'X':
          ਛਾਂਟੋ_opt = ਛਾਂਟੋ_extension;
          ਰੋਕੋ;

        ਹਾਲਤ '1':
          /* -1 has no effect after -l.  */
          ਜੇ (format_opt != long_format)
            format_opt = one_ਪ੍ਰਤੀ_ਸਤਰ;
          ਰੋਕੋ;

        ਹਾਲਤ AUTHOR_OPTION:
          print_author = true;
          ਰੋਕੋ;

        ਹਾਲਤ HIDE_OPTION:
          {
            ਬਣਤਰ ignore_ਨਮੂਨਾ *hide = xmalloc (ਆਕਾਰ *hide);
            hide->ਨਮੂਨਾ_2 = optarg;
            hide->ਅਗਲਾ = hide_patterns;
            hide_patterns = hide;
          }
          ਰੋਕੋ;

        ਹਾਲਤ ਛਾਂਟੋ_OPTION:
          ਛਾਂਟੋ_opt = XARGMATCH ("--sort", optarg, ਛਾਂਟੋ_ਦਲੀਲਾਂ, ਛਾਂਟੋ_types);
          ਰੋਕੋ;

        ਹਾਲਤ ਸਮੂਹ_DIRECTORIES_ਪਹਿਲਾ_OPTION:
          directories_ਪਹਿਲਾ = true;
          ਰੋਕੋ;

        ਹਾਲਤ ਸਮਾਂ_OPTION:
          ਸਮਾਂ_type = XARGMATCH ("--time", optarg, ਸਮਾਂ_ਦਲੀਲਾਂ, ਸਮਾਂ_types);
          explicit_ਸਮਾਂ = true;
          ਰੋਕੋ;

        ਹਾਲਤ FORMAT_OPTION:
          format_opt = XARGMATCH ("--format", optarg, format_ਦਲੀਲਾਂ,
                                  format_types);
          ਰੋਕੋ;

        ਹਾਲਤ ਭਰਿਆ_ਸਮਾਂ_OPTION:
          format_opt = long_format;
          ਸਮਾਂ_style_option = (ਅੱਖਰ *) "full-iso";
          ਰੋਕੋ;

        ਹਾਲਤ COLOR_OPTION:
          {
            ਪੂਰਨ i;
            ਜੇ (optarg)
              i = XARGMATCH ("--color", optarg, when_ਦਲੀਲਾਂ, when_types);
            ਨਹੀਂਤਾਂ
              /* Using --color with no argument is equivalent to using
                 --color=always.  */
              i = when_always;

            print_with_color = (i == when_always
                                || (i == when_if_tty && stdout_isatty ()));
            ਰੋਕੋ;
          }

        ਹਾਲਤ HYPERLINK_OPTION:
          {
            ਪੂਰਨ i;
            ਜੇ (optarg)
              i = XARGMATCH ("--hyperlink", optarg, when_ਦਲੀਲਾਂ, when_types);
            ਨਹੀਂਤਾਂ
              /* Using --hyperlink with no argument is equivalent to using
                 --hyperlink=always.  */
              i = when_always;

            print_hyperlink = (i == when_always
                               || (i == when_if_tty && stdout_isatty ()));
            ਰੋਕੋ;
          }

        ਹਾਲਤ INDICATOR_STYLE_OPTION:
          indicator_style = XARGMATCH ("--indicator-style", optarg,
                                       indicator_style_ਦਲੀਲਾਂ,
                                       indicator_style_types);
          ਰੋਕੋ;

        ਹਾਲਤ QUOTING_STYLE_OPTION:
          quoting_style_opt = XARGMATCH ("--quoting-style", optarg,
                                         quoting_style_ਦਲੀਲਾਂ,
                                         quoting_style_vals);
          ਰੋਕੋ;

        ਹਾਲਤ ਸਮਾਂ_STYLE_OPTION:
          ਸਮਾਂ_style_option = optarg;
          ਰੋਕੋ;

        ਹਾਲਤ SHOW_CONTROL_CHARS_OPTION:
          hide_control_chars_opt = false;
          ਰੋਕੋ;

        ਹਾਲਤ ਖੰਡ_ਮਾਪ_OPTION:
          {
            ਗਿਣਤੀ strtol_error e = human_options (optarg, &human_output_opts,
                                                 &output_ਖੰਡ_ਮਾਪ);
            ਜੇ (e != LONGINT_OK)
              xstrtol_fatal (e, oi, 0, long_options, optarg);
            file_human_output_opts = human_output_opts;
            file_output_ਖੰਡ_ਮਾਪ = output_ਖੰਡ_ਮਾਪ;
          }
          ਰੋਕੋ;

        ਹਾਲਤ SI_OPTION:
          file_human_output_opts = human_output_opts =
            human_autoscale | human_SI;
          file_output_ਖੰਡ_ਮਾਪ = output_ਖੰਡ_ਮਾਪ = 1;
          ਰੋਕੋ;

        ਹਾਲਤ 'Z':
          print_scontext = true;
          ਰੋਕੋ;

        ਹਾਲਤ ZERO_OPTION:
          eolbyte = 0;
          hide_control_chars_opt = false;
          ਜੇ (format_opt != long_format)
            format_opt = one_ਪ੍ਰਤੀ_ਸਤਰ;
          print_with_color = false;
          quoting_style_opt = literal_quoting_style;
          ਰੋਕੋ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);

        ਮੂਲ:
          usage (LS_FAILURE);
        }
    }

  ਜੇ (! output_ਖੰਡ_ਮਾਪ)
    {
      ਅੱਖਰ ਅਚਲ *ls_ਖੰਡ_ਮਾਪ = getenv ("LS_BLOCK_SIZE");
      human_options (ls_ਖੰਡ_ਮਾਪ,
                     &human_output_opts, &output_ਖੰਡ_ਮਾਪ);
      ਜੇ (ls_ਖੰਡ_ਮਾਪ || getenv ("BLOCK_SIZE"))
        {
          file_human_output_opts = human_output_opts;
          file_output_ਖੰਡ_ਮਾਪ = output_ਖੰਡ_ਮਾਪ;
        }
      ਜੇ (kibibytes_specified)
        {
          human_output_opts = 0;
          output_ਖੰਡ_ਮਾਪ = 1024;
        }
    }

  format = (0 <= format_opt ? format_opt
            : ls_ਬਹੁਲਕ == LS_LS ? (stdout_isatty ()
                                  ? many_ਪ੍ਰਤੀ_ਸਤਰ : one_ਪ੍ਰਤੀ_ਸਤਰ)
            : ls_ਬਹੁਲਕ == LS_MULTI_ਖਾਨਾ ? many_ਪ੍ਰਤੀ_ਸਤਰ
            : /* ls_mode == LS_LONG_FORMAT */ long_format);

  /* If the line length was not set by a switch but is needed to determine
     output, go to the work of obtaining it from the environment.  */
  ptrdiff_t linelen = ਚੌੜਾਈ_opt;
  ਜੇ (format == many_ਪ੍ਰਤੀ_ਸਤਰ || format == horizontal || format == with_commas
      || print_with_color)
    {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ TIOCGWINSZ
      ਜੇ (linelen < 0)
        {
          ਬਣਤਰ winsize ws;
          ਜੇ (stdout_isatty ()
              && 0 <= ioctl (STDOUT_FILENO, TIOCGWINSZ, &ws)
              && 0 < ws.ws_ਖਾਨਾ)
            linelen = ws.ws_ਖਾਨਾ <= MIN (PTRDIFF_MAX, SIZE_MAX) ? ws.ws_ਖਾਨਾ : 0;
        }
#ਜੇ_ਅੰਤ
      ਜੇ (linelen < 0)
        {
          ਅੱਖਰ ਅਚਲ *p = getenv ("COLUMNS");
          ਜੇ (p && *p)
            {
              linelen = decode_ਸਤਰ_ਲੰਮਾਈ (p);
              ਜੇ (linelen < 0)
                error (0, 0,
                       _("ignoring invalid width"
                         " in environment variable COLUMNS: %s"),
                       quote (p));
            }
        }
    }

  ਸਤਰ_ਲੰਮਾਈ = linelen < 0 ? 80 : linelen;

  /* Determine the max possible number of display columns.  */
  max_idx = ਸਤਰ_ਲੰਮਾਈ / MIN_ਕਾਲਮ_ਚੌੜਾਈ;
  /* Account for first display column not having a separator,
     or line_lengths shorter than MIN_COLUMN_WIDTH.  */
  max_idx += ਸਤਰ_ਲੰਮਾਈ % MIN_ਕਾਲਮ_ਚੌੜਾਈ != 0;

  ਜੇ (format == many_ਪ੍ਰਤੀ_ਸਤਰ || format == horizontal || format == with_commas)
    {
      ਜੇ (0 <= tabsize_opt)
        tabsize = tabsize_opt;
      ਨਹੀਂਤਾਂ
        {
          tabsize = 8;
          ਅੱਖਰ ਅਚਲ *p = getenv ("TABSIZE");
          ਜੇ (p)
            {
              uintmax_t tmp;
              ਜੇ (xstrtoumax (p, ਨਿਰਾ, 0, &tmp, "") == LONGINT_OK
                  && tmp <= SIZE_MAX)
                tabsize = tmp;
              ਨਹੀਂਤਾਂ
                error (0, 0,
                       _("ignoring invalid tab size"
                         " in environment variable TABSIZE: %s"),
                       quote (p));
            }
        }
    }

  qmark_funny_chars = (hide_control_chars_opt < 0
                       ? ls_ਬਹੁਲਕ == LS_LS && stdout_isatty ()
                       : hide_control_chars_opt);

  qmark_newline = hide_control_chars_opt != false && eolbyte == '\n'
                  && ((ਸਤਰ_ਲੰਮਾਈ
                       && (format == with_commas
                           || format == many_ਪ੍ਰਤੀ_ਸਤਰ || format == horizontal))
                      || format == one_ਪ੍ਰਤੀ_ਸਤਰ || format == long_format);

  ਪੂਰਨ qs = quoting_style_opt;
  ਜੇ (qs < 0)
    qs = getenv_quoting_style ();
  ਜੇ (qs < 0)
    qs = (ls_ਬਹੁਲਕ == LS_LS
          ? (stdout_isatty () ? shell_escape_quoting_style : -1)
          : escape_quoting_style);
  ਜੇ (0 <= qs)
    ਸੈੱਟ_quoting_style (ਨਿਰਾ, qs);
  qs = get_quoting_style (ਨਿਰਾ);
  align_variable_outer_quotes
    = ((format == long_format
        || ((format == many_ਪ੍ਰਤੀ_ਸਤਰ || format == horizontal) && ਸਤਰ_ਲੰਮਾਈ))
       && (qs == shell_quoting_style
           || qs == shell_escape_quoting_style
           || qs == c_maybe_quoting_style));
  filename_quoting_options = clone_quoting_options (ਨਿਰਾ);
  ਜੇ (qs == escape_quoting_style)
    ਸੈੱਟ_char_quoting (filename_quoting_options, ' ', 1);
  ਜੇ (file_type <= indicator_style)
    {
      ਲਈ (ਅੱਖਰ ਅਚਲ *p = &"*=>@|"[indicator_style - file_type]; *p; p++)
        ਸੈੱਟ_char_quoting (filename_quoting_options, *p, 1);
    }
  ਜੇ (format == with_commas
      && (qs == shell_quoting_style
          || qs == shell_escape_quoting_style
          || qs == c_maybe_quoting_style
          || qs == escape_quoting_style))
    ਸੈੱਟ_char_quoting (filename_quoting_options, ',', 1);

  dirname_quoting_options = clone_quoting_options (ਨਿਰਾ);
  ਸੈੱਟ_char_quoting (dirname_quoting_options, ':', 1);

  /* --dired implies --format=long (-l) and sans --hyperlink.
     So ignore it if those overridden.  */
  dired &= (format == long_format) & !print_hyperlink;

  ਜੇ (eolbyte < dired)
    error (LS_FAILURE, 0, _("--dired and --zero are incompatible"));

  /* If a time type is explicitly specified (with -c, -u, or --time=)
     and we're not showing a time (-l not specified), then sort by that time,
     rather than by name.  Note this behavior is unspecified by POSIX.  */

  ਛਾਂਟੋ_type = (0 <= ਛਾਂਟੋ_opt ? ਛਾਂਟੋ_opt
               : (format != long_format && explicit_ਸਮਾਂ)
               ? ਛਾਂਟੋ_ਸਮਾਂ : ਛਾਂਟੋ_name);

  ਜੇ (format == long_format)
    {
      ਅੱਖਰ *envstyle = ਨਿਰਾ;
      ਅੱਖਰ *style = ਸਮਾਂ_style_option;
      ਜੇ (! style)
        style = envstyle = getenv ("TIME_STYLE");

      ਜੇ (! style)
        style = (ਅੱਖਰ *) "locale";
      ਨਹੀਂਤਾਂ
        {
          ਸਥਿਰ ਅੱਖਰ ਅਚਲ posix_ਅਗੇਤਰ[] = "posix-";
          ਜਦੋਂ (STREQ_LEN (style, posix_ਅਗੇਤਰ, ਆਕਾਰ posix_ਅਗੇਤਰ - 1))
            {
              ਜੇ (! hard_locale (LC_ਸਮਾਂ))
                ਵਾਪਸ optind;
              style += ਆਕਾਰ posix_ਅਗੇਤਰ - 1;
            }
        }

      ਜੇ (*style == '+')
        {
          ਅੱਖਰ *p0 = style + 1;
          ਅੱਖਰ *p0nl = strchr (p0, '\n');
          ਅੱਖਰ ਅਚਲ *p1 = p0;
          ਜੇ (p0nl)
            {
              p1 = p0nl + 1;
              ਜੇ (strchr (p1, '\n'))
                error (LS_FAILURE, 0, _("invalid time style format %s"),
                       quote (p0));
              ਜੇ (envstyle)
                p0 = xmemdup0 (p0, p0nl - p0);
              ਨਹੀਂਤਾਂ
                *p0nl = '\0';
            }
          long_ਸਮਾਂ_format[0] = p0;
          long_ਸਮਾਂ_format[1] = p1;
        }
      ਨਹੀਂਤਾਂ
        {
          ਚੋਣ (x_timestyle_ਮੇਲ (style, /*allow_posix=*/ true,
                                     ਸਮਾਂ_style_ਦਲੀਲਾਂ,
                                     (ਅੱਖਰ ਅਚਲ *) ਸਮਾਂ_style_types,
                                     ਆਕਾਰ (*ਸਮਾਂ_style_types), LS_FAILURE))
            {
            ਹਾਲਤ ਭਰਿਆ_iso_ਸਮਾਂ_style:
              long_ਸਮਾਂ_format[0] = long_ਸਮਾਂ_format[1] =
                "%Y-%m-%d %H:%M:%S.%N %z";
              ਰੋਕੋ;

            ਹਾਲਤ long_iso_ਸਮਾਂ_style:
              long_ਸਮਾਂ_format[0] = long_ਸਮਾਂ_format[1] = "%Y-%m-%d %H:%M";
              ਰੋਕੋ;

            ਹਾਲਤ iso_ਸਮਾਂ_style:
              long_ਸਮਾਂ_format[0] = "%Y-%m-%d ";
              long_ਸਮਾਂ_format[1] = "%m-%d %H:%M";
              ਰੋਕੋ;

            ਹਾਲਤ locale_ਸਮਾਂ_style:
              ਜੇ (hard_locale (LC_ਸਮਾਂ))
                {
                  ਲਈ (ਪੂਰਨ i = 0; i < 2; i++)
                    long_ਸਮਾਂ_format[i] =
                      dcgettext (ਨਿਰਾ, long_ਸਮਾਂ_format[i], LC_ਸਮਾਂ);
                }
            }
        }

      abformat_init ();
    }

  ਵਾਪਸ optind;
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

ਸਥਿਰ bool
get_funky_string (ਅੱਖਰ **dest, ਅੱਖਰ ਅਚਲ **src, bool equals_ਸਿਰਾ,
                  size_t *output_ਗਿਣਤੀ)
{
  ਅੱਖਰ ਅੰਕ;			/* For numerical codes */
  size_t ਗਿਣਤੀ_2;			/* Something to count with */
  ਗਿਣਤੀ {
    ST_GND, ST_BACKSLASH, ST_OCTAL, ST_HEX, ST_CARET, ST_ਸਿਰਾ, ST_ERROR
  } state;
  ਅੱਖਰ ਅਚਲ *p;
  ਅੱਖਰ *q;

  p = *src;			/* We don't want to double-indirect */
  q = *dest;			/* the whole darn time.  */

  ਗਿਣਤੀ_2 = 0;			/* No characters counted in yet.  */
  ਅੰਕ = 0;

  state = ST_GND;		/* Start in ground state.  */
  ਜਦੋਂ (state < ST_ਸਿਰਾ)
    {
      ਚੋਣ (state)
        {
        ਹਾਲਤ ST_GND:		/* Ground state (no escapes) */
          ਚੋਣ (*p)
            {
            ਹਾਲਤ ':':
            ਹਾਲਤ '\0':
              state = ST_ਸਿਰਾ;	/* End of string */
              ਰੋਕੋ;
            ਹਾਲਤ '\\':
              state = ST_BACKSLASH; /* Backslash escape sequence */
              ++p;
              ਰੋਕੋ;
            ਹਾਲਤ '^':
              state = ST_CARET; /* Caret escape */
              ++p;
              ਰੋਕੋ;
            ਹਾਲਤ '=':
              ਜੇ (equals_ਸਿਰਾ)
                {
                  state = ST_ਸਿਰਾ; /*ਸਿਰਾ*/
                  ਰੋਕੋ;
                }
              FALLTHROUGH;
            ਮੂਲ:
              *(q++) = *(p++);
              ++ਗਿਣਤੀ_2;
              ਰੋਕੋ;
            }
          ਰੋਕੋ;

        ਹਾਲਤ ST_BACKSLASH:	/* Backslash escaped character */
          ਚੋਣ (*p)
            {
            ਹਾਲਤ '0':
            ਹਾਲਤ '1':
            ਹਾਲਤ '2':
            ਹਾਲਤ '3':
            ਹਾਲਤ '4':
            ਹਾਲਤ '5':
            ਹਾਲਤ '6':
            ਹਾਲਤ '7':
              state = ST_OCTAL;	/* Octal sequence */
              ਅੰਕ = *p - '0';
              ਰੋਕੋ;
            ਹਾਲਤ 'x':
            ਹਾਲਤ 'X':
              state = ST_HEX;	/* Hex sequence */
              ਅੰਕ = 0;
              ਰੋਕੋ;
            ਹਾਲਤ 'a':		/* Bell */
              ਅੰਕ = '\a';
              ਰੋਕੋ;
            ਹਾਲਤ 'b':		/* Backspace */
              ਅੰਕ = '\b';
              ਰੋਕੋ;
            ਹਾਲਤ 'e':		/* Escape */
              ਅੰਕ = 27;
              ਰੋਕੋ;
            ਹਾਲਤ 'f':		/* Form feed */
              ਅੰਕ = '\f';
              ਰੋਕੋ;
            ਹਾਲਤ 'n':		/* Newline */
              ਅੰਕ = '\n';
              ਰੋਕੋ;
            ਹਾਲਤ 'r':		/* Carriage return */
              ਅੰਕ = '\r';
              ਰੋਕੋ;
            ਹਾਲਤ 't':		/* Tab */
              ਅੰਕ = '\t';
              ਰੋਕੋ;
            ਹਾਲਤ 'v':		/* Vtab */
              ਅੰਕ = '\v';
              ਰੋਕੋ;
            ਹਾਲਤ '?':		/* Delete */
              ਅੰਕ = 127;
              ਰੋਕੋ;
            ਹਾਲਤ '_':		/* Space */
              ਅੰਕ = ' ';
              ਰੋਕੋ;
            ਹਾਲਤ '\0':		/* End of string */
              state = ST_ERROR;	/* Error! */
              ਰੋਕੋ;
            ਮੂਲ:		/* Escaped character like \ ^ : = */
              ਅੰਕ = *p;
              ਰੋਕੋ;
            }
          ਜੇ (state == ST_BACKSLASH)
            {
              *(q++) = ਅੰਕ;
              ++ਗਿਣਤੀ_2;
              state = ST_GND;
            }
          ++p;
          ਰੋਕੋ;

        ਹਾਲਤ ST_OCTAL:		/* Octal sequence */
          ਜੇ (*p < '0' || *p > '7')
            {
              *(q++) = ਅੰਕ;
              ++ਗਿਣਤੀ_2;
              state = ST_GND;
            }
          ਨਹੀਂਤਾਂ
            ਅੰਕ = (ਅੰਕ << 3) + (*(p++) - '0');
          ਰੋਕੋ;

        ਹਾਲਤ ST_HEX:		/* Hex sequence */
          ਚੋਣ (*p)
            {
            ਹਾਲਤ '0':
            ਹਾਲਤ '1':
            ਹਾਲਤ '2':
            ਹਾਲਤ '3':
            ਹਾਲਤ '4':
            ਹਾਲਤ '5':
            ਹਾਲਤ '6':
            ਹਾਲਤ '7':
            ਹਾਲਤ '8':
            ਹਾਲਤ '9':
              ਅੰਕ = (ਅੰਕ << 4) + (*(p++) - '0');
              ਰੋਕੋ;
            ਹਾਲਤ 'a':
            ਹਾਲਤ 'b':
            ਹਾਲਤ 'c':
            ਹਾਲਤ 'd':
            ਹਾਲਤ 'e':
            ਹਾਲਤ 'f':
              ਅੰਕ = (ਅੰਕ << 4) + (*(p++) - 'a') + 10;
              ਰੋਕੋ;
            ਹਾਲਤ 'A':
            ਹਾਲਤ 'B':
            ਹਾਲਤ 'C':
            ਹਾਲਤ 'D':
            ਹਾਲਤ 'E':
            ਹਾਲਤ 'F':
              ਅੰਕ = (ਅੰਕ << 4) + (*(p++) - 'A') + 10;
              ਰੋਕੋ;
            ਮੂਲ:
              *(q++) = ਅੰਕ;
              ++ਗਿਣਤੀ_2;
              state = ST_GND;
              ਰੋਕੋ;
            }
          ਰੋਕੋ;

        ਹਾਲਤ ST_CARET:		/* Caret escape */
          state = ST_GND;	/* Should be the next state... */
          ਜੇ (*p >= '@' && *p <= '~')
            {
              *(q++) = *(p++) & 037;
              ++ਗਿਣਤੀ_2;
            }
          ਨਹੀਂਤਾਂ ਜੇ (*p == '?')
            {
              *(q++) = 127;
              ++ਗਿਣਤੀ_2;
            }
          ਨਹੀਂਤਾਂ
            state = ST_ERROR;
          ਰੋਕੋ;

        ਹਾਲਤ ST_ਸਿਰਾ: ਹਾਲਤ ST_ERROR: ਮੂਲ:
          affirm (false);
        }
    }

  *dest = q;
  *src = p;
  *output_ਗਿਣਤੀ = ਗਿਣਤੀ_2;

  ਵਾਪਸ state != ST_ERROR;
}

ਗਿਣਤੀ parse_state
  {
    PS_ਸ਼ੁਰੂ = 1,
    PS_2,
    PS_3,
    PS_4,
    PS_ਪੂਰਾ,
    PS_FAIL
  };


/* Check if the content of TERM is a valid name in dircolors.  */

ਸਥਿਰ bool
ਗਿਆਤ_ਪਦ_type (ਖਾਲੀ)
{
  ਅੱਖਰ ਅਚਲ *ਪਦ = getenv ("TERM");
  ਜੇ (! ਪਦ || ! *ਪਦ)
    ਵਾਪਸ false;

  ਅੱਖਰ ਅਚਲ *ਸਤਰ = G_ਸਤਰ;
  ਜਦੋਂ (ਸਤਰ - G_ਸਤਰ < ਆਕਾਰ (G_ਸਤਰ))
    {
      ਜੇ (STRNCMP_LIT (ਸਤਰ, "TERM ") == 0)
        {
          ਜੇ (fnmatch (ਸਤਰ + 5, ਪਦ, 0) == 0)
            ਵਾਪਸ true;
        }
      ਸਤਰ += ਲੜੀ_ਲੰਬਾਈ (ਸਤਰ) + 1;
    }

  ਵਾਪਸ false;
}

ਸਥਿਰ ਖਾਲੀ
parse_ls_color (ਖਾਲੀ)
{
  ਅੱਖਰ ਅਚਲ *p;		/* Pointer to character being parsed */
  ਅੱਖਰ *buf;			/* color_buf buffer pointer */
  ਅੱਖਰ label0, label1;		/* Indicator label */
  ਬਣਤਰ color_ext_type *ext;	/* Extension we are working on */

  ਜੇ ((p = getenv ("LS_COLORS")) == ਨਿਰਾ || *p == '\0')
    {
      /* LS_COLORS takes precedence, but if that's not set then
         honor the COLORTERM and TERM env variables so that
         we only go with the internal ANSI color codes if the
         former is non empty or the latter is set to a known value.  */
      ਅੱਖਰ ਅਚਲ *colorterm = getenv ("COLORTERM");
      ਜੇ (! (colorterm && *colorterm) && ! ਗਿਆਤ_ਪਦ_type ())
        print_with_color = false;
      ਵਾਪਸ;
    }

  ext = ਨਿਰਾ;

  /* This is an overly conservative estimate, but any possible
     LS_COLORS string will *not* generate a color_buf longer than
     itself, so it is a safe way of allocating a buffer in
     advance.  */
  buf = color_buf = xstrdup (p);

  ਗਿਣਤੀ parse_state state = PS_ਸ਼ੁਰੂ;
  ਜਦੋਂ (true)
    {
      ਚੋਣ (state)
        {
        ਹਾਲਤ PS_ਸ਼ੁਰੂ:		/* First label character */
          ਚੋਣ (*p)
            {
            ਹਾਲਤ ':':
              ++p;
              ਰੋਕੋ;

            ਹਾਲਤ '*':
              /* Allocate new extension block and add to head of
                 linked list (this way a later definition will
                 override an earlier one, which can be useful for
                 having terminal-specific defs override global).  */

              ext = xmalloc (ਆਕਾਰ *ext);
              ext->ਅਗਲਾ = color_ext_list;
              color_ext_list = ext;
              ext->exact_ਮੇਲ = false;

              ++p;
              ext->ext.string = buf;

              state = (get_funky_string (&buf, &p, true, &ext->ext.len)
                       ? PS_4 : PS_FAIL);
              ਰੋਕੋ;

            ਹਾਲਤ '\0':
              state = PS_ਪੂਰਾ;	/* Done! */
              ਜਾਓ ਪੂਰਾ;

            ਮੂਲ:	/* Assume it is file type label */
              label0 = *p++;
              state = PS_2;
              ਰੋਕੋ;
            }
          ਰੋਕੋ;

        ਹਾਲਤ PS_2:		/* Second label character */
          ਜੇ (*p)
            {
              label1 = *p++;
              state = PS_3;
            }
          ਨਹੀਂਤਾਂ
            state = PS_FAIL;	/* Error */
          ਰੋਕੋ;

        ਹਾਲਤ PS_3:		/* Equal sign after indicator label */
          state = PS_FAIL;	/* Assume failure...  */
          ਜੇ (*(p++) == '=')/* It *should* be...  */
            {
              ਲਈ (ਪੂਰਨ i = 0; i < countof (indicator_name); i++)
                {
                  ਜੇ ((label0 == indicator_name[i][0])
                      && (label1 == indicator_name[i][1]))
                    {
                      color_indicator[i].string = buf;
                      state = (get_funky_string (&buf, &p, false,
                                                 &color_indicator[i].len)
                               ? PS_ਸ਼ੁਰੂ : PS_FAIL);
                      ਰੋਕੋ;
                    }
                }
              ਜੇ (state == PS_FAIL)
                error (0, 0, _("unrecognized prefix: %s"),
                       quote ((ਅੱਖਰ []) {label0, label1, '\0'}));
            }
          ਰੋਕੋ;

        ਹਾਲਤ PS_4:		/* Equal sign after *.ext */
          ਜੇ (*(p++) == '=')
            {
              ext->seq.string = buf;
              state = (get_funky_string (&buf, &p, false, &ext->seq.len)
                       ? PS_ਸ਼ੁਰੂ : PS_FAIL);
            }
          ਨਹੀਂਤਾਂ
            state = PS_FAIL;
          ਰੋਕੋ;

        ਹਾਲਤ PS_FAIL:
          ਜਾਓ ਪੂਰਾ;

        ਹਾਲਤ PS_ਪੂਰਾ: ਮੂਲ:
          affirm (false);
        }
    }
 ਪੂਰਾ:

  ਜੇ (state == PS_FAIL)
    {
      error (0, 0,
             _("unparsable value for LS_COLORS environment variable"));
      ਮੁਕਤ (color_buf);
      ਲਈ (ਬਣਤਰ color_ext_type *e = color_ext_list; e != ਨਿਰਾ;
           /*ਖਾਲੀ_ਥਾਵਾਂ*/)
        {
          ਬਣਤਰ color_ext_type *e2 = e;
          e = e->ਅਗਲਾ;
          ਮੁਕਤ (e2);
        }
      print_with_color = false;
      ਵਾਪਸ;
    }
  ਨਹੀਂਤਾਂ
    {
      /* Postprocess list to set EXACT_MATCH on entries where there are
         different cased extensions with separate sequences defined.
         Also set ext.len to SIZE_MAX on any entries that can't
         match due to precedence, to avoid redundant string compares.  */
      ਲਈ (ਬਣਤਰ color_ext_type *e1 = color_ext_list; e1 != ਨਿਰਾ;
           e1 = e1->ਅਗਲਾ)
        {
          bool case_ignored = false;

          ਲਈ (ਬਣਤਰ color_ext_type *e2 = e1->ਅਗਲਾ; e2 != ਨਿਰਾ;
               e2 = e2->ਅਗਲਾ)
            {
              ਜੇ (e2->ext.len < SIZE_MAX && e1->ext.len == e2->ext.len)
                {
                  ਜੇ (memeq (e1->ext.string, e2->ext.string, e1->ext.len))
                    e2->ext.len = SIZE_MAX; /* Ignore */
                  ਨਹੀਂਤਾਂ ਜੇ (c_strncasecmp (e1->ext.string, e2->ext.string,
                                          e1->ext.len) == 0)
                    {
                      ਜੇ (case_ignored)
                        {
                          e2->ext.len = SIZE_MAX; /* Ignore */
                        }
                      ਨਹੀਂਤਾਂ ਜੇ (e1->seq.len == e2->seq.len
                               && memeq (e1->seq.string, e2->seq.string,
                                         e1->seq.len))
                        {
                          e2->ext.len = SIZE_MAX; /* Ignore */
                          case_ignored = true;    /* Ignore all subsequent */
                        }
                      ਨਹੀਂਤਾਂ
                        {
                          e1->exact_ਮੇਲ = true;
                          e2->exact_ਮੇਲ = true;
                        }
                    }
                }
            }
        }
    }

  ਜੇ (color_indicator[C_LINK].len == 6
      && !STRNCMP_LIT (color_indicator[C_LINK].string, "target"))
    color_symlink_as_referent = true;
}

/* Set the exit status to report a failure.  If SERIOUS, it is a
   serious failure; otherwise, it is merely a minor problem.  */

ਸਥਿਰ ਖਾਲੀ
ਸੈੱਟ_exit_ਸਥਿਤੀ (bool serious)
{
  ਜੇ (serious)
    exit_ਸਥਿਤੀ = LS_FAILURE;
  ਨਹੀਂਤਾਂ ਜੇ (exit_ਸਥਿਤੀ == EXIT_SUCCESS)
    exit_ਸਥਿਤੀ = LS_MINOR_PROBLEM;
}

/* Assuming a failure is serious if SERIOUS, use the printf-style
   MESSAGE to report the failure to access a file named FILE.  Assume
   errno is set appropriately for the failure.  */

ਸਥਿਰ ਖਾਲੀ
file_failure (bool serious, ਅੱਖਰ ਅਚਲ *ਸੁਨੇਹਾ_2, ਅੱਖਰ ਅਚਲ *file)
{
  error (0, errno, ਸੁਨੇਹਾ_2, quoteaf (file));
  ਸੈੱਟ_exit_ਸਥਿਤੀ (serious);
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

ਸਥਿਰ ਖਾਲੀ
ਕਤਾਰ_directory (ਅੱਖਰ ਅਚਲ *name, ਅੱਖਰ ਅਚਲ *realname, bool command_ਸਤਰ_arg)
{
  ਬਣਤਰ pending *new = xmalloc (ਆਕਾਰ *new);
  new->realname = realname ? xstrdup (realname) : ਨਿਰਾ;
  new->name = name ? xstrdup (name) : ਨਿਰਾ;
  new->command_ਸਤਰ_arg = command_ਸਤਰ_arg;
  new->ਅਗਲਾ = pending_dirs;
  pending_dirs = new;
}

/* Read directory NAME, and list the files in it.
   If REALNAME is nonzero, print its name instead of NAME;
   this is used for symbolic links to directories.
   COMMAND_LINE_ARG means this directory was mentioned on the command line.  */

ਸਥਿਰ ਖਾਲੀ
print_dir (ਅੱਖਰ ਅਚਲ *name, ਅੱਖਰ ਅਚਲ *realname, bool command_ਸਤਰ_arg)
{
  DIR *dirp;
  ਬਣਤਰ dirent *ਅਗਲਾ;
  uintmax_t ਕੁੱਲ_ਖੰਡਾਂ = 0;
  ਸਥਿਰ bool ਪਹਿਲਾ = true;

  errno = 0;
  dirp = opendir (name);
  ਜੇ (!dirp)
    {
      ਜੇ (command_ਸਤਰ_arg || ! ignorable_traversal_errno (errno))
        file_failure (command_ਸਤਰ_arg, _("cannot open directory %s"), name);
      ਵਾਪਸ;
    }

  ਜੇ (LOOP_DETECT)
    {
      ਬਣਤਰ stat dir_stat;
      ਪੂਰਨ fd = dirfd (dirp);

      /* If dirfd failed, endure the overhead of stat'ing by path  */
      ਜੇ ((0 <= fd
           ? fstat_for_ino (fd, &dir_stat)
           : stat_for_ino (name, &dir_stat)) < 0)
        {
          ਜੇ (! ignorable_traversal_errno (errno))
            file_failure (command_ਸਤਰ_arg,
                          _("cannot determine device and inode of %s"), name);
          closedir (dirp);
          ਵਾਪਸ;
        }

      /* If we've already visited this dev/inode pair, warn that
         we've found a loop, and do not process this directory.  */
      ਜੇ (ਦੇਖੋ_dir (dir_stat.st_dev, dir_stat.st_ino))
        {
          error (0, 0, _("%s: not listing already-listed directory"),
                 quotef (name));
          closedir (dirp);
          ਸੈੱਟ_exit_ਸਥਿਤੀ (true);
          ਵਾਪਸ;
        }

      dev_ino_ਧੱਕੋ (dir_stat.st_dev, dir_stat.st_ino);
    }

  clear_files ();

  ਜੇ (recursive || print_dir_name)
    {
      ਜੇ (!ਪਹਿਲਾ)
        dired_outbyte ('\n');
      ਪਹਿਲਾ = false;
      dired_indent ();

      ਅੱਖਰ *absolute_name = ਨਿਰਾ;
      ਜੇ (print_hyperlink)
        {
          absolute_name = canonicalize_filename_ਬਹੁਲਕ (name, CAN_MISSING);
          ਜੇ (! absolute_name
              && ! ignorable_traversal_errno (errno))
            file_failure (command_ਸਤਰ_arg,
                          _("error canonicalizing %s"), name);
        }
      quote_name (realname ? realname : name, dirname_quoting_options, -1,
                  ਨਿਰਾ, true, &subdired_obstack, absolute_name);

      ਮੁਕਤ (absolute_name);

      dired_outstring (":\n");
    }

  /* Read the directory entries, and insert the subfiles into the 'cwd_file'
     table.  */

  ਜਦੋਂ (true)
    {
      /* Set errno to zero so we can distinguish between a readdir failure
         and when readdir simply finds that there are no more entries.  */
      errno = 0;
      ਅਗਲਾ = readdir (dirp);
      ਜੇ (ਅਗਲਾ)
        {
          ਜੇ (! file_ignored (ਅਗਲਾ->d_name))
            {
              ਗਿਣਤੀ filetype type;
#ਜੇ HAVE_STRUCT_DIRENT_D_TYPE
              type = d_type_filetype[ਅਗਲਾ->d_type];
#ਨਹੀਂਤਾਂ
              type = unknown;
#ਜੇ_ਅੰਤ
              ਕੁੱਲ_ਖੰਡਾਂ += gobble_file (ਅਗਲਾ->d_name, type,
                                           RELIABLE_D_INO (ਅਗਲਾ),
                                           false, name);

              /* In this narrow case, print out each name right away, so
                 ls uses constant memory while processing the entries of
                 this directory.  Useful when there are many (millions)
                 of entries in a directory.  */
              ਜੇ (format == one_ਪ੍ਰਤੀ_ਸਤਰ && ਛਾਂਟੋ_type == ਛਾਂਟੋ_none
                      && !print_ਖੰਡ_ਮਾਪ && !recursive)
                {
                  /* We must call sort_files in spite of
                     "sort_type == sort_none" for its initialization
                     of the sorted_file vector.  */
                  ਛਾਂਟੋ_files ();
                  print_current_files ();
                  clear_files ();
                }
            }
        }
      ਨਹੀਂਤਾਂ
        {
          ਪੂਰਨ err = errno;
          ਜੇ (err == 0)
            ਰੋਕੋ;
          /* Ignore errors indicating that the directory was removed.  */
          ਜੇ (ignorable_traversal_errno (err))
            ਰੋਕੋ;
          file_failure (command_ਸਤਰ_arg, _("reading directory %s"), name);
          ਜੇ (err != EOVERFLOW)
            ਰੋਕੋ;
        }

      /* When processing a very large directory, and since we've inhibited
         interrupts, this loop would take so long that ls would be annoyingly
         uninterruptible.  This ensures that it handles signals promptly.  */
      process_ਸੰਕੇਤ ();
    }

  ਜੇ (closedir (dirp) != 0)
    {
      file_failure (command_ਸਤਰ_arg, _("closing directory %s"), name);
      /* Don't return; print whatever we got.  */
    }

  /* Sort the directory contents.  */
  ਛਾਂਟੋ_files ();

  /* If any member files are subdirectories, perhaps they should have their
     contents listed rather than being mentioned here as files.  */

  ਜੇ (recursive)
    ਕੱਢੋ_dirs_from_files (name, false);

  ਜੇ (format == long_format || print_ਖੰਡ_ਮਾਪ)
    {
      ਅੱਖਰ buf[LONGEST_HUMAN_READABLE + 3];
      ਅੱਖਰ *p = human_readable (ਕੁੱਲ_ਖੰਡਾਂ, buf + 1, human_output_opts,
                                ST_NBLOCKSIZE, output_ਖੰਡ_ਮਾਪ);
      ਅੱਖਰ *pend = p + ਲੜੀ_ਲੰਬਾਈ (p);
      *--p = ' ';
      *pend++ = eolbyte;
      dired_indent ();
      dired_outstring (_("total"));
      dired_outbuf (p, pend - p);
    }

  ਜੇ (cwd_n_ਵਰਤਿਆ)
    print_current_files ();
}

/* Add 'pattern' to the list of patterns for which files that match are
   not listed.  */

ਸਥਿਰ ਖਾਲੀ
ਜੋੜੋ_ignore_ਨਮੂਨਾ (ਅੱਖਰ ਅਚਲ *ਨਮੂਨਾ_2)
{
  ਬਣਤਰ ignore_ਨਮੂਨਾ *ignore;

  ignore = xmalloc (ਆਕਾਰ *ignore);
  ignore->ਨਮੂਨਾ_2 = ਨਮੂਨਾ_2;
  /* Add it to the head of the linked list.  */
  ignore->ਅਗਲਾ = ignore_patterns;
  ignore_patterns = ignore;
}

/* Return true if one of the PATTERNS matches FILE.  */

ਸਥਿਰ bool
patterns_ਮੇਲ (ਬਣਤਰ ignore_ਨਮੂਨਾ ਅਚਲ *patterns, ਅੱਖਰ ਅਚਲ *file)
{
  ਲਈ (ਬਣਤਰ ignore_ਨਮੂਨਾ ਅਚਲ *p = patterns; p; p = p->ਅਗਲਾ)
    ਜੇ (fnmatch (p->ਨਮੂਨਾ_2, file, FNM_PERIOD) == 0)
      ਵਾਪਸ true;
  ਵਾਪਸ false;
}

/* Return true if FILE should be ignored.  */

ਸਥਿਰ bool
file_ignored (ਅੱਖਰ ਅਚਲ *name)
{
  ਵਾਪਸ ((ignore_ਬਹੁਲਕ != IGNORE_ਨਿਊਨਤਮ
           && name[0] == '.'
           && (ignore_ਬਹੁਲਕ == IGNORE_DEFAULT || ! name[1 + (name[1] == '.')]))
          || (ignore_ਬਹੁਲਕ == IGNORE_DEFAULT
              && patterns_ਮੇਲ (hide_patterns, name))
          || patterns_ਮੇਲ (ignore_patterns, name));
}

/* POSIX requires that a file size be printed without a sign, even
   when negative.  Assume the typical case where negative sizes are
   actually positive values that have wrapped around.  */

ਸਥਿਰ uintmax_t
unsigned_file_ਮਾਪ (off_t ਮਾਪ)
{
  ਵਾਪਸ ਮਾਪ + (ਮਾਪ < 0) * ((uintmax_t) OFF_T_MAX - OFF_T_MIN + 1);
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CAP
/* Return true if NAME has a capability (see linux/capability.h) */
ਸਥਿਰ bool
has_capability (ਅੱਖਰ ਅਚਲ *name)
{
  ਅੱਖਰ *ਨਤੀਜਾ;
  bool has_cap;

  cap_t cap_d = cap_get_file (name);
  ਜੇ (cap_d == ਨਿਰਾ)
    ਵਾਪਸ false;

  ਨਤੀਜਾ = cap_ਮੰਜ਼ਿਲ_ਪਾਠ (cap_d, ਨਿਰਾ);
  cap_free (cap_d);
  ਜੇ (!ਨਤੀਜਾ)
    ਵਾਪਸ false;

  /* check if human-readable capability string is empty */
  has_cap = !!*ਨਤੀਜਾ;

  cap_free (ਨਤੀਜਾ);
  ਵਾਪਸ has_cap;
}
#ਨਹੀਂਤਾਂ
ਸਥਿਰ bool
has_capability (MAYBE_ਅਣਵਰਤਿਆ ਅੱਖਰ ਅਚਲ *name)
{
  errno = ENOTSUP;
  ਵਾਪਸ false;
}
#ਜੇ_ਅੰਤ

/* Enter and remove entries in the table 'cwd_file'.  */

ਸਥਿਰ ਖਾਲੀ
free_ent (ਬਣਤਰ fileinfo *f)
{
  ਮੁਕਤ (f->name);
  ਮੁਕਤ (f->linkname);
  ਮੁਕਤ (f->absolute_name);
  ਜੇ (f->scontext != UNKNOWN_SECURITY_CONTEXT)
    aclinfo_scontext_free (f->scontext);
}

/* Empty the table of files.  */
ਸਥਿਰ ਖਾਲੀ
clear_files (ਖਾਲੀ)
{
  ਲਈ (idx_t i = 0; i < cwd_n_ਵਰਤਿਆ; i++)
    {
      ਬਣਤਰ fileinfo *f = sorted_file[i];
      free_ent (f);
    }

  cwd_n_ਵਰਤਿਆ = 0;
  cwd_some_quoted = false;
  any_has_acl = false;
  inode_ਸੰਖਿਆ_ਚੌੜਾਈ = 0;
  ਖੰਡ_ਮਾਪ_ਚੌੜਾਈ = 0;
  nlink_ਚੌੜਾਈ = 0;
  owner_ਚੌੜਾਈ = 0;
  ਸਮੂਹ_ਚੌੜਾਈ = 0;
  author_ਚੌੜਾਈ = 0;
  scontext_ਚੌੜਾਈ = 0;
  major_device_ਸੰਖਿਆ_ਚੌੜਾਈ = 0;
  minor_device_ਸੰਖਿਆ_ਚੌੜਾਈ = 0;
  file_ਮਾਪ_ਚੌੜਾਈ = 0;
}

/* Cache file_has_aclinfo failure, when it's trivial to do.
   Like file_has_aclinfo, but when F's st_dev says it's on a file
   system lacking ACL support, return 0 with ENOTSUP immediately.  */
ਸਥਿਰ ਪੂਰਨ
file_has_aclinfo_cache (ਅੱਖਰ ਅਚਲ *file, ਬਣਤਰ fileinfo *f,
                        ਬਣਤਰ aclinfo *ai, ਪੂਰਨ flags)
{
  /* If UNSUPPORTED_SCONTEXT, these variables hold the
     st_dev and associated info for the most recently processed device
     for which file_has_aclinfo failed indicating lack of support.  */
  ਸਥਿਰ ਪੂਰਨ unsupported_return;
  ਸਥਿਰ ਅੱਖਰ *unsupported_scontext;
  ਸਥਿਰ ਪੂਰਨ unsupported_scontext_err;
  ਸਥਿਰ dev_t unsupported_device;

  ਜੇ (f->stat_ok && unsupported_scontext
      && f->stat.st_dev == unsupported_device)
    {
      ai->buf = ai->u._gl_acl_ch;
      ai->ਮਾਪ = 0;
      ai->scontext = unsupported_scontext;
      ai->scontext_err = unsupported_scontext_err;
      errno = ENOTSUP;
      ਵਾਪਸ unsupported_return;
    }

  errno = 0;
  ਪੂਰਨ n = file_has_aclinfo (file, ai, flags);
  ਪੂਰਨ err = errno;
  ਜੇ (!USE_ACL)
    n = 0;
  /* A security-context error does not imply that ACLs are unsupported.
     Require an unsupported attribute listing too if ACLs or capability
     colors are needed.  */
  ਜੇ (f->stat_ok && n <= 0 && !acl_errno_valid (err)
      && ((!USE_ACL && !format_needs_capability)
          || (ai->ਮਾਪ < 0 && !acl_errno_valid (ai->u.err)))
      && (!(flags & ACL_GET_SCONTEXT) || !acl_errno_valid (ai->scontext_err)))
    {
      /* FIXME: acl_errno_valid() may be a bit too broad determine file system
         non support.  E.g. EBUSY was added for per file reasons (on cygwin). */
      unsupported_return = n;
      unsupported_scontext = ai->scontext;
      unsupported_scontext_err = ai->scontext_err;
      unsupported_device = f->stat.st_dev;
    }
  ਵਾਪਸ n;
}

/* Cache has_capability failure, when it's trivial to do.
   Like has_capability, but when F's st_dev says it's on a file
   system lacking capability support, return 0 with ENOTSUP immediately.  */
ਸਥਿਰ bool
has_capability_cache (ਅੱਖਰ ਅਚਲ *file, ਬਣਤਰ fileinfo *f)
{
  /* If UNSUPPORTED_CACHED, UNSUPPORTED_DEVICE is the cached
     st_dev of the most recently processed device for which we've
     found that has_capability fails indicating lack of support.  */
  ਸਥਿਰ bool unsupported_cached;
  ਸਥਿਰ dev_t unsupported_device;

  ਜੇ (f->stat_ok && unsupported_cached && f->stat.st_dev == unsupported_device)
    {
      errno = ENOTSUP;
      ਵਾਪਸ 0;
    }

  bool b = has_capability (file);
  ਜੇ (f->stat_ok && !b && !acl_errno_valid (errno))
    {
      unsupported_cached = true;
      unsupported_device = f->stat.st_dev;
    }
  ਵਾਪਸ b;
}

/* Get the ACL, security context, and capability information needed for F.
   FULL_NAME names the file; DO_DEREF says whether to follow symlinks.  */
ਸਥਿਰ ਖਾਲੀ
get_extended_info (ਅੱਖਰ ਅਚਲ *ਭਰਿਆ_name, ਬਣਤਰ fileinfo *f, bool do_deref)
{
  /* FIXME: file_has_aclinfo() conflates capability and acl xattr lookup
     with security context attributes.  Ideally we'd have separate
     ACL_GET_{CAPABILITY,ACL} flags to lookup just the required items,
     simplifying the logic here.  */

  ਗਿਣਤੀ filetype type = f->filetype;
  bool check_acl = USE_ACL && format == long_format;
  bool get_scontext = false;
#ਜੇ HAVE_SMACK || USE_SELINUX_SELINUX_H
  get_scontext = (format == long_format) | print_scontext;
#ਜੇ_ਅੰਤ
  bool check_capability = format_needs_capability & (type == ਸਾਧਾਰਨ);

  ਜੇ (type != arg_directory && (check_acl | get_scontext | check_capability))
    {
      ਬਣਤਰ aclinfo ai;
      /* Without ACL support, request contexts to obtain the attribute
         names needed for capability coloring too.  */
      ਪੂਰਨ aclinfo_flags = ((do_deref ? ACL_SYMLINK_FOLLOW : 0)
                           | (get_scontext || (!USE_ACL && check_capability)
                              ? ACL_GET_SCONTEXT : 0)
                           | filetype_d_type[type]);
      ਪੂਰਨ n = file_has_aclinfo_cache (ਭਰਿਆ_name, f, &ai, aclinfo_flags);
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

      ਜੇ (format == long_format && n < 0 && !cannot_access_acl)
        error (0, errno, "%s", quotef (ਭਰਿਆ_name));
      ਨਹੀਂਤਾਂ
        {
          /* When requesting security context information, don't make
             ls fail just because the file (even a command line argument)
             isn't on the right type of file system.  I.e., a getfilecon
             failure isn't in the same class as a stat failure.  */
          ਜੇ (print_scontext && ai.scontext_err
              && ! ignorable_traversal_errno (ai.scontext_err)
              && (! (is_ENOTSUP (ai.scontext_err)
                     || ai.scontext_err == ENODATA)))
            error (0, ai.scontext_err, "%s", quotef (ਭਰਿਆ_name));
        }

      /* has_capability adds around 30% runtime to 'ls --color',
         so call it only if really needed.  Note capability coloring
         is disabled in the default color config.  */
      ਜੇ (check_capability && aclinfo_has_xattr (&ai, XATTR_NAME_CAPS))
        f->has_capability = has_capability_cache (ਭਰਿਆ_name, f);

      f->scontext = ai.scontext;
      ai.scontext = ਨਿਰਾ;
      aclinfo_free (&ai);
    }
}

ਸਥਿਰ bool
needs_quoting (ਅੱਖਰ ਅਚਲ *name)
{
  ਅੱਖਰ test[2];
  size_t len = quotearg_ਬਫਰ (test, ਆਕਾਰ test , name, -1,
                                filename_quoting_options);
  ਵਾਪਸ *name != *test || ਲੜੀ_ਲੰਬਾਈ (name) != len;
}

/* Add a file to the current table of files.
   Verify that the file exists, and print an error message if it does not.
   Return the number of blocks that the file occupies.  */
ਸਥਿਰ uintmax_t
gobble_file (ਅੱਖਰ ਅਚਲ *name, ਗਿਣਤੀ filetype type, ino_t inode,
             bool command_ਸਤਰ_arg, ਅੱਖਰ ਅਚਲ *dirname)
{
  uintmax_t ਖੰਡਾਂ = 0;
  ਬਣਤਰ fileinfo *f;

  /* An inode value prior to gobble_file necessarily came from readdir,
     which is not used for command line arguments.  */
  affirm (! command_ਸਤਰ_arg || inode == NOT_AN_INODE_ਸੰਖਿਆ);

  ਜੇ (cwd_n_ਵਰਤਿਆ == cwd_n_alloc)
    cwd_file = xpalloc (cwd_file, &cwd_n_alloc, 1, -1, ਆਕਾਰ *cwd_file);

  f = &cwd_file[cwd_n_ਵਰਤਿਆ];
  memset (f, '\0', ਆਕਾਰ *f);
  f->stat.st_ino = inode;
  f->filetype = type;
  f->scontext = UNKNOWN_SECURITY_CONTEXT;

  f->quoted = -1;
  ਜੇ ((! cwd_some_quoted) && align_variable_outer_quotes)
    {
      /* Determine if any quoted for padding purposes.  */
      f->quoted = needs_quoting (name);
      ਜੇ (f->quoted)
        cwd_some_quoted = 1;
    }

  bool check_stat =
     (command_ਸਤਰ_arg
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
              || color_symlink_as_referent || check_symlink_ਬਹੁਲਕ))
      /* Command line dereferences are already taken care of by the above
         assertion that the inode number is not yet known.  */
      || (print_inode && inode == NOT_AN_INODE_ਸੰਖਿਆ)
      /* --indicator-style=classify (aka -F) requires statting each
         regular file to see whether it's executable.  */
      || ((type == ਸਾਧਾਰਨ || type == unknown)
          && (indicator_style == classify
              /* This is so that --color ends up highlighting files with these
                 mode bits set even when options like -F are not specified.  */
              || (print_with_color && (is_colored (C_EXEC)
                                       || is_colored (C_SETUID)
                                       || is_colored (C_SETGID))))));

  /* Absolute name of this file, if needed.  */
  ਅੱਖਰ ਅਚਲ *ਭਰਿਆ_name = name;
  ਜੇ (check_stat | print_scontext | format_needs_capability
      && name[0] != '/' && dirname)
    {
      ਅੱਖਰ *p = alloca (ਲੜੀ_ਲੰਬਾਈ (name) + ਲੜੀ_ਲੰਬਾਈ (dirname) + 2);
      attach (p, dirname, name);
      ਭਰਿਆ_name = p;
    }

  bool do_deref;

  ਜੇ (!check_stat)
    do_deref = dereference == DEREF_ALWAYS;
  ਨਹੀਂਤਾਂ
    {
      ਪੂਰਨ err;

      ਜੇ (print_hyperlink)
        {
          f->absolute_name = canonicalize_filename_ਬਹੁਲਕ (ਭਰਿਆ_name,
                                                         CAN_MISSING);
          ਜੇ (! f->absolute_name
              && (command_ਸਤਰ_arg
                  || ! ignorable_traversal_errno (errno)))
            file_failure (command_ਸਤਰ_arg,
                          _("error canonicalizing %s"), ਭਰਿਆ_name);
        }

      ਚੋਣ (dereference)
        {
        ਹਾਲਤ DEREF_ALWAYS:
          err = do_stat (ਭਰਿਆ_name, &f->stat);
          do_deref = true;
          ਰੋਕੋ;

        ਹਾਲਤ DEREF_COMMAND_ਸਤਰ_ARGUMENTS:
        ਹਾਲਤ DEREF_COMMAND_ਸਤਰ_SYMLINK_ਮੰਜ਼ਿਲ_DIR:
          ਜੇ (command_ਸਤਰ_arg)
            {
              bool need_lstat;
              err = do_stat (ਭਰਿਆ_name, &f->stat);
              do_deref = true;

              ਜੇ (dereference == DEREF_COMMAND_ਸਤਰ_ARGUMENTS)
                ਰੋਕੋ;

              need_lstat = (err < 0
                            ? (errno == ENOENT || errno == ELOOP)
                            : ! S_ISDIR (f->stat.st_ਬਹੁਲਕ));
              ਜੇ (!need_lstat)
                ਰੋਕੋ;

              /* stat failed because of ENOENT || ELOOP, maybe indicating a
                 non-traversable symlink.  Or stat succeeded,
                 FULL_NAME does not refer to a directory,
                 and --dereference-command-line-symlink-to-dir is in effect.
                 Fall through so that we call lstat instead.  */
            }
          FALLTHROUGH;

        ਹਾਲਤ DEREF_NEVER:
          err = do_lstat (ਭਰਿਆ_name, &f->stat);
          do_deref = false;
          ਰੋਕੋ;

        ਹਾਲਤ DEREF_UNDEFINED: ਮੂਲ:
          affirm (false);
        }

      ਜੇ (err != 0)
        {
          ਜੇ (! command_ਸਤਰ_arg && ignorable_traversal_errno (errno))
            {
              ਪੂਰਨ stat_errno = errno;
              ਬਣਤਰ stat linkstat;
              bool dangling_link = (do_deref
                                    && (type == symbolic_link
                                        || type == unknown)
                                    && do_lstat (ਭਰਿਆ_name, &linkstat) == 0);
              errno = stat_errno;
              ਜੇ (! dangling_link)
                {
                  free_ent (f);
                  ਵਾਪਸ 0;
                }
            }

          /* Failure to stat a command line argument leads to
             an exit status of 2.  For other files, stat failure
             provokes an exit status of 1.  */
          file_failure (command_ਸਤਰ_arg,
                        _("cannot access %s"), ਭਰਿਆ_name);

          ਜੇ (command_ਸਤਰ_arg)
            {
              free_ent (f);
              ਵਾਪਸ 0;
            }

          f->name = xstrdup (name);
          cwd_n_ਵਰਤਿਆ++;

          ਵਾਪਸ 0;
        }

      f->stat_ok = true;
      f->filetype = type = d_type_filetype[IFTODT (f->stat.st_ਬਹੁਲਕ)];
    }

  ਜੇ (type == directory && command_ਸਤਰ_arg && !ਤੁਰੰਤ_dirs)
    f->filetype = type = arg_directory;

  get_extended_info (ਭਰਿਆ_name, f, do_deref);

  ਜੇ ((type == symbolic_link)
      & ((format == long_format) | check_symlink_ਬਹੁਲਕ))
    {
      ਬਣਤਰ stat linkstats;

      ਜੇ (! get_link_name (ਭਰਿਆ_name, f, command_ਸਤਰ_arg))
        {
          free_ent (f);
          ਵਾਪਸ 0;
        }

      /* Use the slower quoting path for this entry, though
         don't update CWD_SOME_QUOTED since alignment not affected.  */
      ਜੇ (f->linkname && f->quoted == 0 && needs_quoting (f->linkname))
        f->quoted = -1;

      /* Avoid following symbolic links when possible, i.e., when
         they won't be traced and when no indicator is needed.  */
      ਜੇ (f->linkname
          && (file_type <= indicator_style || check_symlink_ਬਹੁਲਕ)
          && stat_for_ਬਹੁਲਕ (ਭਰਿਆ_name, &linkstats) == 0)
        {
          f->linkok = true;
          f->linkmode = linkstats.st_ਬਹੁਲਕ;
        }
    }

  ਖੰਡਾਂ = STP_NBLOCKS (&f->stat);
  ਜੇ (format == long_format || print_ਖੰਡ_ਮਾਪ)
    {
      ਅੱਖਰ buf[LONGEST_HUMAN_READABLE + 1];
      ਪੂਰਨ len = mbswidth (human_readable (ਖੰਡਾਂ, buf, human_output_opts,
                                          ST_NBLOCKSIZE, output_ਖੰਡ_ਮਾਪ),
                          MBSWIDTH_FLAGS);
      ਜੇ (ਖੰਡ_ਮਾਪ_ਚੌੜਾਈ < len)
        ਖੰਡ_ਮਾਪ_ਚੌੜਾਈ = len;
    }

  ਜੇ (format == long_format)
    {
      ਜੇ (print_owner)
        {
          ਪੂਰਨ len = format_user_ਚੌੜਾਈ (f->stat.st_uid);
          ਜੇ (owner_ਚੌੜਾਈ < len)
            owner_ਚੌੜਾਈ = len;
        }

      ਜੇ (print_ਸਮੂਹ)
        {
          ਪੂਰਨ len = format_ਸਮੂਹ_ਚੌੜਾਈ (f->stat.st_gid);
          ਜੇ (ਸਮੂਹ_ਚੌੜਾਈ < len)
            ਸਮੂਹ_ਚੌੜਾਈ = len;
        }

      ਜੇ (print_author)
        {
          ਪੂਰਨ len = format_user_ਚੌੜਾਈ (f->stat.st_author);
          ਜੇ (author_ਚੌੜਾਈ < len)
            author_ਚੌੜਾਈ = len;
        }
    }

  ਜੇ (print_scontext)
    {
      ਪੂਰਨ len = ਲੜੀ_ਲੰਬਾਈ (f->scontext);
      ਜੇ (scontext_ਚੌੜਾਈ < len)
        scontext_ਚੌੜਾਈ = len;
    }

  ਜੇ (format == long_format)
    {
      ਅੱਖਰ b[INT_BUFSIZE_BOUND (uintmax_t)];
      ਪੂਰਨ b_len = ਲੜੀ_ਲੰਬਾਈ (umaxtostr (f->stat.st_nlink, b));
      ਜੇ (nlink_ਚੌੜਾਈ < b_len)
        nlink_ਚੌੜਾਈ = b_len;

      ਜੇ ((type == chardev) | (type == blockdev))
        {
          ਅੱਖਰ buf[INT_BUFSIZE_BOUND (uintmax_t)];
          ਪੂਰਨ len = ਲੜੀ_ਲੰਬਾਈ (umaxtostr (major (f->stat.st_rdev), buf));
          ਜੇ (major_device_ਸੰਖਿਆ_ਚੌੜਾਈ < len)
            major_device_ਸੰਖਿਆ_ਚੌੜਾਈ = len;
          len = ਲੜੀ_ਲੰਬਾਈ (umaxtostr (minor (f->stat.st_rdev), buf));
          ਜੇ (minor_device_ਸੰਖਿਆ_ਚੌੜਾਈ < len)
            minor_device_ਸੰਖਿਆ_ਚੌੜਾਈ = len;
          len = major_device_ਸੰਖਿਆ_ਚੌੜਾਈ + 2 + minor_device_ਸੰਖਿਆ_ਚੌੜਾਈ;
          ਜੇ (file_ਮਾਪ_ਚੌੜਾਈ < len)
            file_ਮਾਪ_ਚੌੜਾਈ = len;
        }
      ਨਹੀਂਤਾਂ
        {
          ਅੱਖਰ buf[LONGEST_HUMAN_READABLE + 1];
          uintmax_t ਮਾਪ = unsigned_file_ਮਾਪ (f->stat.st_ਮਾਪ);
          ਪੂਰਨ len = mbswidth (human_readable (ਮਾਪ, buf,
                                              file_human_output_opts,
                                              1, file_output_ਖੰਡ_ਮਾਪ),
                              MBSWIDTH_FLAGS);
          ਜੇ (file_ਮਾਪ_ਚੌੜਾਈ < len)
            file_ਮਾਪ_ਚੌੜਾਈ = len;
        }
    }

  ਜੇ (print_inode)
    {
      ਅੱਖਰ buf[INT_BUFSIZE_BOUND (uintmax_t)];
      ਪੂਰਨ len = ਲੜੀ_ਲੰਬਾਈ (umaxtostr (f->stat.st_ino, buf));
      ਜੇ (inode_ਸੰਖਿਆ_ਚੌੜਾਈ < len)
        inode_ਸੰਖਿਆ_ਚੌੜਾਈ = len;
    }

  f->name = xstrdup (name);
  cwd_n_ਵਰਤਿਆ++;

  ਵਾਪਸ ਖੰਡਾਂ;
}

/* Return true if F refers to a directory.  */
ਸਥਿਰ bool
is_directory (ਅਚਲ ਬਣਤਰ fileinfo *f)
{
  ਵਾਪਸ f->filetype == directory || f->filetype == arg_directory;
}

/* Return true if F refers to a (symlinked) directory.  */
ਸਥਿਰ bool
is_linked_directory (ਅਚਲ ਬਣਤਰ fileinfo *f)
{
  ਵਾਪਸ f->filetype == directory || f->filetype == arg_directory
         || S_ISDIR (f->linkmode);
}

/* Put the name of the file that FILENAME is a symbolic link to
   into the LINKNAME field of 'f'.  COMMAND_LINE_ARG indicates whether
   FILENAME is a command-line argument.  */

ਸਥਿਰ bool
get_link_name (ਅੱਖਰ ਅਚਲ *filename, ਬਣਤਰ fileinfo *f, bool command_ਸਤਰ_arg)
{
  f->linkname = areadlink_with_ਮਾਪ (filename, f->stat.st_ਮਾਪ);
  ਜੇ (f->linkname == ਨਿਰਾ)
    {
      ਜੇ (! command_ਸਤਰ_arg
          && ignorable_traversal_errno (errno))
        ਵਾਪਸ false;
      file_failure (command_ਸਤਰ_arg, _("cannot read symbolic link %s"),
                    filename);
    }
  ਵਾਪਸ true;
}

/* Return true if the last component of NAME is '.' or '..'
   This is so we don't try to recurse on '././././. ...' */

ਸਥਿਰ bool
basename_is_dot_or_dotdot (ਅੱਖਰ ਅਚਲ *name)
{
  ਅੱਖਰ ਅਚਲ *ਅਧਾਰ_2 = ਆਖਰੀ_component (name);
  ਵਾਪਸ dot_or_dotdot (ਅਧਾਰ_2);
}

/* Remove any entries from CWD_FILE that are for directories,
   and queue them to be listed as directories instead.
   DIRNAME is the prefix to prepend to each dirname
   to make it correct relative to ls's working dir;
   if it is null, no prefix is needed and "." and ".." should not be ignored.
   If COMMAND_LINE_ARG is true, this directory was mentioned at the top level,
   This is desirable when processing directories recursively.  */

ਸਥਿਰ ਖਾਲੀ
ਕੱਢੋ_dirs_from_files (ਅੱਖਰ ਅਚਲ *dirname, bool command_ਸਤਰ_arg)
{
  idx_t i, j;
  bool ignore_dot_and_dot_dot = (dirname != ਨਿਰਾ);

  ਜੇ (dirname && LOOP_DETECT)
    {
      /* Insert a marker entry first.  When we dequeue this marker entry,
         we'll know that DIRNAME has been processed and may be removed
         from the set of active directories.  */
      ਕਤਾਰ_directory (ਨਿਰਾ, dirname, false);
    }

  /* Queue the directories last one first, because queueing reverses the
     order.  */
  ਲਈ (i = cwd_n_ਵਰਤਿਆ; 0 < i; )
    {
      i--;
      ਬਣਤਰ fileinfo *f = sorted_file[i];

      ਜੇ (is_directory (f)
          && (! ignore_dot_and_dot_dot
              || ! basename_is_dot_or_dotdot (f->name)))
        {
          ਜੇ (!dirname || f->name[0] == '/')
            ਕਤਾਰ_directory (f->name, f->linkname, command_ਸਤਰ_arg);
          ਨਹੀਂਤਾਂ
            {
              ਅੱਖਰ *name = file_name_concat (dirname, f->name, ਨਿਰਾ);
              ਕਤਾਰ_directory (name, f->linkname, command_ਸਤਰ_arg);
              ਮੁਕਤ (name);
            }
          ਜੇ (f->filetype == arg_directory)
            free_ent (f);
        }
    }

  /* Now delete the directories from the table, compacting all the remaining
     entries.  */

  ਲਈ (i = 0, j = 0; i < cwd_n_ਵਰਤਿਆ; i++)
    {
      ਬਣਤਰ fileinfo *f = sorted_file[i];
      sorted_file[j] = f;
      j += (f->filetype != arg_directory);
    }
  cwd_n_ਵਰਤਿਆ = j;
}

/* Use strcoll to compare strings in this locale.  If an error occurs,
   report an error and longjmp to failed_strcoll.  */

ਸਥਿਰ jmp_buf failed_strcoll;

ਸਥਿਰ ਪੂਰਨ
xstrcoll (ਅੱਖਰ ਅਚਲ *a, ਅੱਖਰ ਅਚਲ *b)
{
  ਪੂਰਨ diff;
  errno = 0;
  diff = strcoll (a, b);
  ਜੇ (errno)
    {
      error (0, errno, _("cannot compare file names %s and %s"),
             quote_n (0, a), quote_n (1, b));
      ਸੈੱਟ_exit_ਸਥਿਤੀ (false);
      longjmp (failed_strcoll, 1);
    }
  ਵਾਪਸ diff;
}

/* Comparison routines for sorting the files.  */

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ ਅਚਲ *V;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਪੂਰਨ (*qsortFunc)(V a, V b);

/* Used below in DEFINE_SORT_FUNCTIONS for _df_ sort function variants.  */
ਸਥਿਰ ਪੂਰਨ
dirfirst_check (ਬਣਤਰ fileinfo ਅਚਲ *a, ਬਣਤਰ fileinfo ਅਚਲ *b,
                ਪੂਰਨ (*cmp) (V, V))
{
  ਪੂਰਨ diff = is_linked_directory (b) - is_linked_directory (a);
  ਵਾਪਸ diff ? diff : cmp (a, b);
}

/* Define the 8 different sort function variants required for each sortkey.
   KEY_NAME is a token describing the sort key, e.g., ctime, atime, size.
   KEY_CMP_FUNC is a function to compare records based on that key, e.g.,
   ctime_cmp, atime_cmp, size_cmp.  Append KEY_NAME to the string,
   '[rev_][x]str{cmp|coll}[_df]_', to create each function name.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEFINE_ਛਾਂਟੋ_FUNCTIONS(ਕੁੰਜੀ_name, ਕੁੰਜੀ_cmp_func)			\
  /* direct, non-dirfirst versions */					\
  ਸਥਿਰ ਪੂਰਨ xstrcoll_##key_name (V a, V b)				\
  { ਵਾਪਸ ਕੁੰਜੀ_cmp_func (a, b, xstrcoll); }				\
  ATTRIBUTE_ਸ਼ੁੱਧ ਸਥਿਰ ਪੂਰਨ strcmp_##key_name (V a, V b)		\
  { ਵਾਪਸ ਕੁੰਜੀ_cmp_func (a, b, strcmp); }				\
                                                                        \
  /* reverse, non-dirfirst versions */					\
  ਸਥਿਰ ਪੂਰਨ rev_xstrcoll_##key_name (V a, V b)				\
  { ਵਾਪਸ ਕੁੰਜੀ_cmp_func (b, a, xstrcoll); }				\
  ATTRIBUTE_ਸ਼ੁੱਧ ਸਥਿਰ ਪੂਰਨ rev_strcmp_##key_name (V a, V b)	\
  { ਵਾਪਸ ਕੁੰਜੀ_cmp_func (b, a, strcmp); }				\
                                                                        \
  /* direct, dirfirst versions */					\
  ਸਥਿਰ ਪੂਰਨ xstrcoll_df_##key_name (V a, V b)				\
  { ਵਾਪਸ dirfirst_check (a, b, xstrcoll_##key_name); }		\
  ATTRIBUTE_ਸ਼ੁੱਧ ਸਥਿਰ ਪੂਰਨ strcmp_df_##key_name (V a, V b)		\
  { ਵਾਪਸ dirfirst_check (a, b, strcmp_##key_name); }			\
                                                                        \
  /* reverse, dirfirst versions */					\
  ਸਥਿਰ ਪੂਰਨ rev_xstrcoll_df_##key_name (V a, V b)			\
  { ਵਾਪਸ dirfirst_check (a, b, rev_xstrcoll_##key_name); }		\
  ATTRIBUTE_ਸ਼ੁੱਧ ਸਥਿਰ ਪੂਰਨ rev_strcmp_df_##key_name (V a, V b)	\
  { ਵਾਪਸ dirfirst_check (a, b, rev_strcmp_##key_name); }

ਸਥਿਰ ਪੂਰਨ
cmp_ctime (ਬਣਤਰ fileinfo ਅਚਲ *a, ਬਣਤਰ fileinfo ਅਚਲ *b,
           ਪੂਰਨ (*cmp) (ਅੱਖਰ ਅਚਲ *, ਅੱਖਰ ਅਚਲ *))
{
  ਪੂਰਨ diff = timespec_cmp (get_stat_ctime (&b->stat),
                           get_stat_ctime (&a->stat));
  ਵਾਪਸ diff ? diff : cmp (a->name, b->name);
}

ਸਥਿਰ ਪੂਰਨ
cmp_mtime (ਬਣਤਰ fileinfo ਅਚਲ *a, ਬਣਤਰ fileinfo ਅਚਲ *b,
           ਪੂਰਨ (*cmp) (ਅੱਖਰ ਅਚਲ *, ਅੱਖਰ ਅਚਲ *))
{
  ਪੂਰਨ diff = timespec_cmp (get_stat_mtime (&b->stat),
                           get_stat_mtime (&a->stat));
  ਵਾਪਸ diff ? diff : cmp (a->name, b->name);
}

ਸਥਿਰ ਪੂਰਨ
cmp_atime (ਬਣਤਰ fileinfo ਅਚਲ *a, ਬਣਤਰ fileinfo ਅਚਲ *b,
           ਪੂਰਨ (*cmp) (ਅੱਖਰ ਅਚਲ *, ਅੱਖਰ ਅਚਲ *))
{
  ਪੂਰਨ diff = timespec_cmp (get_stat_atime (&b->stat),
                           get_stat_atime (&a->stat));
  ਵਾਪਸ diff ? diff : cmp (a->name, b->name);
}

ਸਥਿਰ ਪੂਰਨ
cmp_btime (ਬਣਤਰ fileinfo ਅਚਲ *a, ਬਣਤਰ fileinfo ਅਚਲ *b,
           ਪੂਰਨ (*cmp) (ਅੱਖਰ ਅਚਲ *, ਅੱਖਰ ਅਚਲ *))
{
  ਪੂਰਨ diff = timespec_cmp (get_stat_btime (&b->stat),
                           get_stat_btime (&a->stat));
  ਵਾਪਸ diff ? diff : cmp (a->name, b->name);
}

ਸਥਿਰ ਪੂਰਨ
off_cmp (off_t a, off_t b)
{
  ਵਾਪਸ _GL_CMP (a, b);
}

ਸਥਿਰ ਪੂਰਨ
cmp_ਮਾਪ (ਬਣਤਰ fileinfo ਅਚਲ *a, ਬਣਤਰ fileinfo ਅਚਲ *b,
          ਪੂਰਨ (*cmp) (ਅੱਖਰ ਅਚਲ *, ਅੱਖਰ ਅਚਲ *))
{
  ਪੂਰਨ diff = off_cmp (b->stat.st_ਮਾਪ, a->stat.st_ਮਾਪ);
  ਵਾਪਸ diff ? diff : cmp (a->name, b->name);
}

ਸਥਿਰ ਪੂਰਨ
cmp_name (ਬਣਤਰ fileinfo ਅਚਲ *a, ਬਣਤਰ fileinfo ਅਚਲ *b,
          ਪੂਰਨ (*cmp) (ਅੱਖਰ ਅਚਲ *, ਅੱਖਰ ਅਚਲ *))
{
  ਵਾਪਸ cmp (a->name, b->name);
}

/* Compare file extensions.  Files with no extension are 'smallest'.
   If extensions are the same, compare by file names instead.  */

ਸਥਿਰ ਪੂਰਨ
cmp_extension (ਬਣਤਰ fileinfo ਅਚਲ *a, ਬਣਤਰ fileinfo ਅਚਲ *b,
               ਪੂਰਨ (*cmp) (ਅੱਖਰ ਅਚਲ *, ਅੱਖਰ ਅਚਲ *))
{
  ਅੱਖਰ ਅਚਲ *base1 = strrchr (a->name, '.');
  ਅੱਖਰ ਅਚਲ *base2 = strrchr (b->name, '.');
  ਪੂਰਨ diff = cmp (base1 ? base1 : "", base2 ? base2 : "");
  ਵਾਪਸ diff ? diff : cmp (a->name, b->name);
}

/* Return the (cached) screen width,
   for the NAME associated with the passed fileinfo F.  */

ਸਥਿਰ size_t
fileinfo_name_ਚੌੜਾਈ (ਬਣਤਰ fileinfo ਅਚਲ *f)
{
  ਵਾਪਸ f->ਚੌੜਾਈ
         ? f->ਚੌੜਾਈ
         : quote_name_ਚੌੜਾਈ (f->name, filename_quoting_options, f->quoted);
}

ਸਥਿਰ ਪੂਰਨ
cmp_ਚੌੜਾਈ (ਬਣਤਰ fileinfo ਅਚਲ *a, ਬਣਤਰ fileinfo ਅਚਲ *b,
          ਪੂਰਨ (*cmp) (ਅੱਖਰ ਅਚਲ *, ਅੱਖਰ ਅਚਲ *))
{
  ਪੂਰਨ diff = fileinfo_name_ਚੌੜਾਈ (a) - fileinfo_name_ਚੌੜਾਈ (b);
  ਵਾਪਸ diff ? diff : cmp (a->name, b->name);
}

DEFINE_ਛਾਂਟੋ_FUNCTIONS (ctime, cmp_ctime)
DEFINE_ਛਾਂਟੋ_FUNCTIONS (mtime, cmp_mtime)
DEFINE_ਛਾਂਟੋ_FUNCTIONS (atime, cmp_atime)
DEFINE_ਛਾਂਟੋ_FUNCTIONS (btime, cmp_btime)
DEFINE_ਛਾਂਟੋ_FUNCTIONS (ਮਾਪ, cmp_ਮਾਪ)
DEFINE_ਛਾਂਟੋ_FUNCTIONS (name, cmp_name)
DEFINE_ਛਾਂਟੋ_FUNCTIONS (extension, cmp_extension)
DEFINE_ਛਾਂਟੋ_FUNCTIONS (ਚੌੜਾਈ, cmp_ਚੌੜਾਈ)

/* Compare file versions.
   Unlike the other compare functions, cmp_version does not fail
   because filevercmp and strcmp do not fail; cmp_version uses strcmp
   instead of xstrcoll because filevercmp is locale-independent so
   strcmp is its appropriate secondary.

   All the other sort options need xstrcoll and strcmp variants,
   because they all use xstrcoll (either as the primary or secondary
   sort key), and xstrcoll has the ability to do a longjmp if strcoll fails for
   locale reasons.  */
ਸਥਿਰ ਪੂਰਨ
cmp_ਸੰਸਕਰਣ (ਬਣਤਰ fileinfo ਅਚਲ *a, ਬਣਤਰ fileinfo ਅਚਲ *b)
{
  ਪੂਰਨ diff = filevercmp (a->name, b->name);
  ਵਾਪਸ diff ? diff : strcmp (a->name, b->name);
}

ਸਥਿਰ ਪੂਰਨ
xstrcoll_ਸੰਸਕਰਣ (V a, V b)
{
  ਵਾਪਸ cmp_ਸੰਸਕਰਣ (a, b);
}
ਸਥਿਰ ਪੂਰਨ
rev_xstrcoll_ਸੰਸਕਰਣ (V a, V b)
{
  ਵਾਪਸ cmp_ਸੰਸਕਰਣ (b, a);
}
ਸਥਿਰ ਪੂਰਨ
xstrcoll_df_ਸੰਸਕਰਣ (V a, V b)
{
  ਵਾਪਸ dirfirst_check (a, b, xstrcoll_ਸੰਸਕਰਣ);
}
ਸਥਿਰ ਪੂਰਨ
rev_xstrcoll_df_ਸੰਸਕਰਣ (V a, V b)
{
  ਵਾਪਸ dirfirst_check (a, b, rev_xstrcoll_ਸੰਸਕਰਣ);
}


/* We have 2^3 different variants for each sort-key function
   (for 3 independent sort modes).
   The function pointers stored in this array must be dereferenced as:

    sort_variants[sort_key][use_strcmp][reverse][dirs_first]

   Note that the order in which sort keys are listed in the function pointer
   array below is defined by the order of the elements in the time_type and
   sort_type enums!  */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LIST_SORTFUNCTION_VARIANTS(ਕੁੰਜੀ_name)                        \
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

ਸਥਿਰ qsortFunc ਅਚਲ ਛਾਂਟੋ_functions[][2][2][2] =
  {
    LIST_SORTFUNCTION_VARIANTS (name),
    LIST_SORTFUNCTION_VARIANTS (extension),
    LIST_SORTFUNCTION_VARIANTS (ਚੌੜਾਈ),
    LIST_SORTFUNCTION_VARIANTS (ਮਾਪ),

    {
      {
        { xstrcoll_ਸੰਸਕਰਣ, xstrcoll_df_ਸੰਸਕਰਣ },
        { rev_xstrcoll_ਸੰਸਕਰਣ, rev_xstrcoll_df_ਸੰਸਕਰਣ },
      },

      /* We use NULL for the strcmp variants of version comparison
         since as explained in cmp_version definition, version comparison
         does not rely on xstrcoll, so it will never longjmp, and never
         need to try the strcmp fallback. */
      {
        { ਨਿਰਾ, ਨਿਰਾ },
        { ਨਿਰਾ, ਨਿਰਾ },
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
static_assert (countof (ਛਾਂਟੋ_functions) == ਛਾਂਟੋ_numtypes - 2 + ਸਮਾਂ_numtypes);

/* Set up SORTED_FILE to point to the in-use entries in CWD_FILE, in order.  */

ਸਥਿਰ ਖਾਲੀ
initialize_ordering_ਸਦਿਸ਼ (ਖਾਲੀ)
{
  ਲਈ (idx_t i = 0; i < cwd_n_ਵਰਤਿਆ; i++)
    sorted_file[i] = &cwd_file[i];
}

/* Cache values based on attributes global to all files.  */

ਸਥਿਰ ਖਾਲੀ
update_current_files_info (ਖਾਲੀ)
{
  /* Cache screen width of name, if needed multiple times.  */
  ਜੇ (ਛਾਂਟੋ_type == ਛਾਂਟੋ_ਚੌੜਾਈ
      || (ਸਤਰ_ਲੰਮਾਈ && (format == many_ਪ੍ਰਤੀ_ਸਤਰ || format == horizontal)))
    {
      ਲਈ (idx_t i = 0; i < cwd_n_ਵਰਤਿਆ; i++)
        {
          ਬਣਤਰ fileinfo *f = sorted_file[i];
          f->ਚੌੜਾਈ = fileinfo_name_ਚੌੜਾਈ (f);
        }
    }
}

/* Sort the files now in the table.  */

ਸਥਿਰ ਖਾਲੀ
ਛਾਂਟੋ_files (ਖਾਲੀ)
{
  bool use_strcmp;

  ਜੇ (sorted_file_alloc < cwd_n_ਵਰਤਿਆ + (cwd_n_ਵਰਤਿਆ >> 1))
    {
      ਮੁਕਤ (sorted_file);
      sorted_file = xinmalloc (cwd_n_ਵਰਤਿਆ, 3 * ਆਕਾਰ *sorted_file);
      sorted_file_alloc = 3 * cwd_n_ਵਰਤਿਆ;
    }

  initialize_ordering_ਸਦਿਸ਼ ();

  update_current_files_info ();

  ਜੇ (ਛਾਂਟੋ_type == ਛਾਂਟੋ_none)
    ਵਾਪਸ;

  /* Try strcoll.  If it fails, fall back on strcmp.  We can't safely
     ignore strcoll failures, as a failing strcoll might be a
     comparison function that is not a total order, and if we ignored
     the failure this might cause qsort to dump core.  */

  ਜੇ (! setjmp (failed_strcoll))
    use_strcmp = false;      /* strcoll() succeeded */
  ਨਹੀਂਤਾਂ
    {
      use_strcmp = true;
      affirm (ਛਾਂਟੋ_type != ਛਾਂਟੋ_ਸੰਸਕਰਣ);
      initialize_ordering_ਸਦਿਸ਼ ();
    }

  /* When sort_type == sort_time, use time_type as subindex.  */
  mpsort ((ਖਾਲੀ ਅਚਲ **) sorted_file, cwd_n_ਵਰਤਿਆ,
          ਛਾਂਟੋ_functions[ਛਾਂਟੋ_type + (ਛਾਂਟੋ_type == ਛਾਂਟੋ_ਸਮਾਂ ? ਸਮਾਂ_type : 0)]
                        [use_strcmp][ਛਾਂਟੋ_ਉਲਟ]
                        [directories_ਪਹਿਲਾ]);
}

/* List all the files now in the table.  */

ਸਥਿਰ ਖਾਲੀ
print_current_files (ਖਾਲੀ)
{
  ਚੋਣ (format)
    {
    ਹਾਲਤ one_ਪ੍ਰਤੀ_ਸਤਰ:
      ਲਈ (idx_t i = 0; i < cwd_n_ਵਰਤਿਆ; i++)
        {
          print_file_name_and_frills (sorted_file[i], 0);
          ਅੱਖਰ_ਲਿਖੋ (eolbyte);
        }
      ਰੋਕੋ;

    ਹਾਲਤ many_ਪ੍ਰਤੀ_ਸਤਰ:
      ਜੇ (! ਸਤਰ_ਲੰਮਾਈ)
        print_with_separator (' ');
      ਨਹੀਂਤਾਂ
        print_many_ਪ੍ਰਤੀ_ਸਤਰ ();
      ਰੋਕੋ;

    ਹਾਲਤ horizontal:
      ਜੇ (! ਸਤਰ_ਲੰਮਾਈ)
        print_with_separator (' ');
      ਨਹੀਂਤਾਂ
        print_horizontal ();
      ਰੋਕੋ;

    ਹਾਲਤ with_commas:
      print_with_separator (',');
      ਰੋਕੋ;

    ਹਾਲਤ long_format:
      ਲਈ (idx_t i = 0; i < cwd_n_ਵਰਤਿਆ; i++)
        {
          ਸੈੱਟ_ਸਾਧਾਰਨ_color ();
          print_long_format (sorted_file[i]);
          dired_outbyte (eolbyte);
        }
      ਰੋਕੋ;
    }
}

/* Replace the first %b with precomputed aligned month names.
   Note on glibc-2.7 at least, this speeds up the whole 'ls -lU'
   process by around 17%, compared to letting strftime() handle the %b.  */

ਸਥਿਰ ptrdiff_t
align_nstrftime (ਅੱਖਰ *buf, size_t ਮਾਪ, bool recent, ਬਣਤਰ tm ਅਚਲ *tm,
                 timezone_t tz, ਪੂਰਨ ns)
{
  ਅੱਖਰ ਅਚਲ *nfmt = (use_abformat
                      ? abformat[recent][tm->tm_mon]
                      : long_ਸਮਾਂ_format[recent]);
  ਵਾਪਸ nstrftime (buf, ਮਾਪ, nfmt, tm, tz, ns);
}

/* Return the expected number of columns in a long-format timestamp,
   or zero if it cannot be calculated.  */

ਸਥਿਰ ਪੂਰਨ
long_ਸਮਾਂ_expected_ਚੌੜਾਈ (ਖਾਲੀ)
{
  ਸਥਿਰ ਪੂਰਨ ਚੌੜਾਈ = -1;

  ਜੇ (ਚੌੜਾਈ < 0)
    {
      time_t epoch = 0;
      ਬਣਤਰ tm tm;
      ਅੱਖਰ buf[ਸਮਾਂ_STAMP_LEN_ਵੱਧ_ਤੋਂ_ਵੱਧ + 1];

      /* In case you're wondering if localtime_rz can fail with an input time_t
         value of 0, let's just say it's very unlikely, but not inconceivable.
         The TZ environment variable would have to specify a time zone that
         is 2**31-1900 years or more ahead of UTC.  This could happen only on
         a 64-bit system that blindly accepts e.g., TZ=UTC+20000000000000.
         However, this is not possible with Solaris 10 or glibc-2.3.5, since
         their implementations limit the offset to 167:59 and 24:00, resp.  */
      ਜੇ (localtime_rz (localtz, &epoch, &tm))
        {
          ptrdiff_t len = align_nstrftime (buf, ਆਕਾਰ buf, false,
                                           &tm, localtz, 0);
          ਜੇ (len > 0)
            ਚੌੜਾਈ = mbsnwidth (buf, len, MBSWIDTH_FLAGS);
        }

      ਜੇ (ਚੌੜਾਈ < 0)
        ਚੌੜਾਈ = 0;
    }

  ਵਾਪਸ ਚੌੜਾਈ;
}

/* Print the user or group name NAME, with numeric id ID, using a
   print width of WIDTH columns.  */

ਸਥਿਰ ਖਾਲੀ
format_user_or_ਸਮੂਹ (ਅੱਖਰ ਅਚਲ *name, uintmax_t id, ਪੂਰਨ ਚੌੜਾਈ)
{
  ਜੇ (name)
    {
      ਪੂਰਨ name_ਚੌੜਾਈ = mbswidth (name, MBSWIDTH_FLAGS);
      ਪੂਰਨ ਚੌੜਾਈ_gap = name_ਚੌੜਾਈ < 0 ? 0 : ਚੌੜਾਈ - name_ਚੌੜਾਈ;
      ਪੂਰਨ pad = MAX (0, ਚੌੜਾਈ_gap);
      dired_outstring (name);

      ਕਰੋ
        dired_outbyte (' ');
      ਜਦੋਂ (pad--);
    }
  ਨਹੀਂਤਾਂ
    dired_pos += ਲਿਖੋ ("%*ju ", ਚੌੜਾਈ, id);
}

/* Print the name or id of the user with id U, using a print width of
   WIDTH.  */

ਸਥਿਰ ਖਾਲੀ
format_user (uid_t u, ਪੂਰਨ ਚੌੜਾਈ, bool stat_ok)
{
  format_user_or_ਸਮੂਹ (! stat_ok ? "?" :
                        (numeric_ids ? ਨਿਰਾ : getuser (u)), u, ਚੌੜਾਈ);
}

/* Likewise, for groups.  */

ਸਥਿਰ ਖਾਲੀ
format_ਸਮੂਹ (gid_t g, ਪੂਰਨ ਚੌੜਾਈ, bool stat_ok)
{
  format_user_or_ਸਮੂਹ (! stat_ok ? "?" :
                        (numeric_ids ? ਨਿਰਾ : getgroup (g)), g, ਚੌੜਾਈ);
}

/* Return the number of columns that format_user_or_group will print,
   or -1 if unknown.  */

ਸਥਿਰ ਪੂਰਨ
format_user_or_ਸਮੂਹ_ਚੌੜਾਈ (ਅੱਖਰ ਅਚਲ *name, uintmax_t id)
{
  ਵਾਪਸ (name
          ? mbswidth (name, MBSWIDTH_FLAGS)
          : snprintf (ਨਿਰਾ, 0, "%ju", id));
}

/* Return the number of columns that format_user will print,
   or -1 if unknown.  */

ਸਥਿਰ ਪੂਰਨ
format_user_ਚੌੜਾਈ (uid_t u)
{
  ਵਾਪਸ format_user_or_ਸਮੂਹ_ਚੌੜਾਈ (numeric_ids ? ਨਿਰਾ : getuser (u), u);
}

/* Likewise, for groups.  */

ਸਥਿਰ ਪੂਰਨ
format_ਸਮੂਹ_ਚੌੜਾਈ (gid_t g)
{
  ਵਾਪਸ format_user_or_ਸਮੂਹ_ਚੌੜਾਈ (numeric_ids ? ਨਿਰਾ : getgroup (g), g);
}

/* Return a pointer to a formatted version of F->stat.st_ino,
   possibly using buffer, which must be at least
   INT_BUFSIZE_BOUND (uintmax_t) bytes.  */
ਸਥਿਰ ਅੱਖਰ *
format_inode (ਅੱਖਰ buf[INT_BUFSIZE_BOUND (uintmax_t)],
              ਅਚਲ ਬਣਤਰ fileinfo *f)
{
  ਵਾਪਸ (f->stat_ok && f->stat.st_ino != NOT_AN_INODE_ਸੰਖਿਆ
          ? umaxtostr (f->stat.st_ino, buf)
          : (ਅੱਖਰ *) "?");
}

/* Print information about F in long format.  */
ਸਥਿਰ ਖਾਲੀ
print_long_format (ਅਚਲ ਬਣਤਰ fileinfo *f)
{
  ਅੱਖਰ modebuf[12];
  ਅੱਖਰ buf
    [LONGEST_HUMAN_READABLE + 1		/* inode */
     + LONGEST_HUMAN_READABLE + 1	/* size in blocks */
     + ਆਕਾਰ (modebuf) - 1 + 1		/* mode string */
     + INT_BUFSIZE_BOUND (uintmax_t)	/* st_nlink */
     + LONGEST_HUMAN_READABLE + 2	/* major device number */
     + LONGEST_HUMAN_READABLE + 1	/* minor device number */
     + ਸਮਾਂ_STAMP_LEN_ਵੱਧ_ਤੋਂ_ਵੱਧ + 1	/* max length of time/date */
     ];
  ptrdiff_t s;
  ਅੱਖਰ *p;
  ਬਣਤਰ timespec when_timespec;
  ਬਣਤਰ tm when_local;
  bool btime_ok = true;

  /* Compute the mode string, except remove the trailing space if no
     file in this directory has an ACL or security context.  */
  ਜੇ (f->stat_ok)
    filemodestring (&f->stat, modebuf);
  ਨਹੀਂਤਾਂ
    {
      modebuf[0] = filetype_letter[f->filetype];
      memset (modebuf + 1, '?', 10);
      modebuf[11] = '\0';
    }
  ਜੇ (! any_has_acl)
    modebuf[10] = '\0';
  ਨਹੀਂਤਾਂ ਜੇ (f->acl_type == ACL_T_LSM_CONTEXT_ONLY)
    modebuf[10] = '.';
  ਨਹੀਂਤਾਂ ਜੇ (f->acl_type == ACL_T_YES)
    modebuf[10] = '+';
  ਨਹੀਂਤਾਂ ਜੇ (f->acl_type == ACL_T_UNKNOWN)
    modebuf[10] = '?';

  ਚੋਣ (ਸਮਾਂ_type)
    {
    ਹਾਲਤ ਸਮਾਂ_ctime:
      when_timespec = get_stat_ctime (&f->stat);
      ਰੋਕੋ;
    ਹਾਲਤ ਸਮਾਂ_mtime:
      when_timespec = get_stat_mtime (&f->stat);
      ਰੋਕੋ;
    ਹਾਲਤ ਸਮਾਂ_atime:
      when_timespec = get_stat_atime (&f->stat);
      ਰੋਕੋ;
    ਹਾਲਤ ਸਮਾਂ_btime:
      when_timespec = get_stat_btime (&f->stat);
      ਜੇ (when_timespec.tv_sec == -1 && when_timespec.tv_nsec == -1)
        btime_ok = false;
      ਰੋਕੋ;
    ਹਾਲਤ ਸਮਾਂ_numtypes: ਮੂਲ:
      unreachable ();
    }

  p = buf;

  ਜੇ (print_inode)
    {
      ਅੱਖਰ hbuf[INT_BUFSIZE_BOUND (uintmax_t)];
      p += sprintf (p, "%*s ", inode_ਸੰਖਿਆ_ਚੌੜਾਈ, format_inode (hbuf, f));
    }

  ਜੇ (print_ਖੰਡ_ਮਾਪ)
    {
      ਅੱਖਰ hbuf[LONGEST_HUMAN_READABLE + 1];
      ਅੱਖਰ ਅਚਲ *ਖੰਡਾਂ =
        (! f->stat_ok
         ? "?"
         : human_readable (STP_NBLOCKS (&f->stat), hbuf, human_output_opts,
                           ST_NBLOCKSIZE, output_ਖੰਡ_ਮਾਪ));
      ਪੂਰਨ ਖੰਡਾਂ_ਚੌੜਾਈ = mbswidth (ਖੰਡਾਂ, MBSWIDTH_FLAGS);
      ਲਈ (ਪੂਰਨ pad = ਖੰਡਾਂ_ਚੌੜਾਈ < 0 ? 0 : ਖੰਡ_ਮਾਪ_ਚੌੜਾਈ - ਖੰਡਾਂ_ਚੌੜਾਈ;
           0 < pad; pad--)
        *p++ = ' ';
      ਜਦੋਂ ((*p++ = *ਖੰਡਾਂ++))
        ਜਾਰੀ;
      p[-1] = ' ';
    }

  /* The last byte of the mode string is the POSIX
     "optional alternate access method flag".  */
  {
    ਅੱਖਰ hbuf[INT_BUFSIZE_BOUND (uintmax_t)];
    p += sprintf (p, "%s %*s ", modebuf, nlink_ਚੌੜਾਈ,
                  ! f->stat_ok ? "?" : umaxtostr (f->stat.st_nlink, hbuf));
  }

  dired_indent ();

  ਜੇ (print_owner || print_ਸਮੂਹ || print_author || print_scontext)
    {
      dired_outbuf (buf, p - buf);

      ਜੇ (print_owner)
        format_user (f->stat.st_uid, owner_ਚੌੜਾਈ, f->stat_ok);

      ਜੇ (print_ਸਮੂਹ)
        format_ਸਮੂਹ (f->stat.st_gid, ਸਮੂਹ_ਚੌੜਾਈ, f->stat_ok);

      ਜੇ (print_author)
        format_user (f->stat.st_author, author_ਚੌੜਾਈ, f->stat_ok);

      ਜੇ (print_scontext)
        format_user_or_ਸਮੂਹ (f->scontext, 0, scontext_ਚੌੜਾਈ);

      p = buf;
    }

  ਜੇ (f->stat_ok
      && (S_ISCHR (f->stat.st_ਬਹੁਲਕ) || S_ISBLK (f->stat.st_ਬਹੁਲਕ)))
    {
      ਪੂਰਨ blanks_ਚੌੜਾਈ = (file_ਮਾਪ_ਚੌੜਾਈ
                          - (major_device_ਸੰਖਿਆ_ਚੌੜਾਈ + 2
                             + minor_device_ਸੰਖਿਆ_ਚੌੜਾਈ));
      p += sprintf (p, "%*ju, %*ju ",
                    major_device_ਸੰਖਿਆ_ਚੌੜਾਈ + MAX (0, blanks_ਚੌੜਾਈ),
                    (uintmax_t) major (f->stat.st_rdev),
                    minor_device_ਸੰਖਿਆ_ਚੌੜਾਈ,
                    (uintmax_t) minor (f->stat.st_rdev));
    }
  ਨਹੀਂਤਾਂ
    {
      ਅੱਖਰ hbuf[LONGEST_HUMAN_READABLE + 1];
      ਅੱਖਰ ਅਚਲ *ਮਾਪ =
        (! f->stat_ok
         ? "?"
         : human_readable (unsigned_file_ਮਾਪ (f->stat.st_ਮਾਪ),
                           hbuf, file_human_output_opts, 1,
                           file_output_ਖੰਡ_ਮਾਪ));
      ਪੂਰਨ ਮਾਪ_ਚੌੜਾਈ = mbswidth (ਮਾਪ, MBSWIDTH_FLAGS);
      ਲਈ (ਪੂਰਨ pad = ਮਾਪ_ਚੌੜਾਈ < 0 ? 0 : file_ਮਾਪ_ਚੌੜਾਈ - ਮਾਪ_ਚੌੜਾਈ;
           0 < pad; pad--)
        *p++ = ' ';
      ਜਦੋਂ ((*p++ = *ਮਾਪ++))
        ਜਾਰੀ;
      p[-1] = ' ';
    }

  s = -1;

  ਜੇ (f->stat_ok && btime_ok
      && localtime_rz (localtz, &when_timespec.tv_sec, &when_local))
    {
      ਬਣਤਰ timespec six_months_ago;
      bool recent;

      /* If the file appears to be in the future, update the current
         time, in case the file happens to have been modified since
         the last time we checked the clock.  */
      ਜੇ (timespec_cmp (current_ਸਮਾਂ, when_timespec) < 0)
        gettime (&current_ਸਮਾਂ);

      /* Consider a time to be recent if it is within the past six months.
         A Gregorian year has 365.2425 * 24 * 60 * 60 == 31556952 seconds
         on the average.  Write this value as an integer constant to
         avoid floating point hassles.  */
      six_months_ago.tv_sec = current_ਸਮਾਂ.tv_sec - 31556952 / 2;
      six_months_ago.tv_nsec = current_ਸਮਾਂ.tv_nsec;

      recent = (timespec_cmp (six_months_ago, when_timespec) < 0
                && timespec_cmp (when_timespec, current_ਸਮਾਂ) < 0);

      /* We assume here that all time zones are offset from UTC by a
         whole number of seconds.  */
      s = align_nstrftime (p, ਸਮਾਂ_STAMP_LEN_ਵੱਧ_ਤੋਂ_ਵੱਧ + 1, recent,
                           &when_local, localtz, when_timespec.tv_nsec);
    }

  ਜੇ (0 <= s)
    {
      p += s;
      *p++ = ' ';
    }
  ਨਹੀਂਤਾਂ
    {
      /* The time cannot be converted using the desired format, so
         print it as a huge integer number of seconds.  */
      ਅੱਖਰ hbuf[INT_BUFSIZE_BOUND (intmax_t)];
      p += sprintf (p, "%*s ", long_ਸਮਾਂ_expected_ਚੌੜਾਈ (),
                    (! f->stat_ok || ! btime_ok
                     ? "?"
                     : timetostr (when_timespec.tv_sec, hbuf)));
      /* FIXME: (maybe) We discarded when_timespec.tv_nsec. */
    }

  dired_outbuf (buf, p - buf);
  size_t w = print_name_with_quoting (f, false, &dired_obstack, p - buf);

  ਜੇ (f->filetype == symbolic_link)
    {
      ਜੇ (f->linkname)
        {
          dired_outstring (" -> ");
          print_name_with_quoting (f, true, ਨਿਰਾ, (p - buf) + w + 4);
          ਜੇ (indicator_style != none)
            print_type_indicator (true, f->linkmode, unknown);
        }
    }
  ਨਹੀਂਤਾਂ ਜੇ (indicator_style != none)
    print_type_indicator (f->stat_ok, f->stat.st_ਬਹੁਲਕ, f->filetype);
}

/* Write to *BUF a quoted representation of the file name NAME, if non-null,
   using OPTIONS to control quoting.  *BUF is set to NAME if no quoting
   is required.  *BUF is allocated if more space required (and the original
   *BUF is not deallocated).
   Store the number of screen columns occupied by NAME's quoted
   representation into WIDTH, if non-null.
   Store into PAD whether an initial space is needed for padding.
   Return the number of bytes in *BUF.  */

ਸਥਿਰ size_t
quote_name_buf (ਅੱਖਰ **inbuf, size_t bufsize, ਅੱਖਰ *name,
                ਬਣਤਰ quoting_options ਅਚਲ *options,
                ਪੂਰਨ needs_general_quoting, size_t *ਚੌੜਾਈ, bool *pad)
{
  ਅੱਖਰ *buf = *inbuf;
  size_t displayed_ਚੌੜਾਈ IF_LINT ( = 0);
  size_t len = 0;
  bool quoted;

  ਗਿਣਤੀ quoting_style qs = get_quoting_style (options);
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

  ਜੇ (needs_general_quoting != 0)
    {
      len = quotearg_ਬਫਰ (buf, bufsize, name, -1, options);
      ਜੇ (bufsize <= len)
        {
          buf = xmalloc (len + 1);
          quotearg_ਬਫਰ (buf, len + 1, name, -1, options);
        }

      quoted = (*name != *buf) || ਲੜੀ_ਲੰਬਾਈ (name) != len;
    }
  ਨਹੀਂਤਾਂ ਜੇ (needs_further_quoting || extra_newline_protection)
    {
      len = ਲੜੀ_ਲੰਬਾਈ (name);
      ਜੇ (bufsize <= len)
        buf = xmalloc (len + 1);
      ਸਮ੍ਰਿਤੀ_ਨਕਲ (buf, name, len + 1);

      quoted = false;
    }
  ਨਹੀਂਤਾਂ
    {
      len = ਲੜੀ_ਲੰਬਾਈ (name);
      buf = name;
      quoted = false;
    }

  /* It's safe to replace newlines without multi-byte iteration, as newlines
     do not appear as part of any multi-byte encoded character.  */
  ਜੇ (extra_newline_protection)
    ਲਈ (ਅੱਖਰ *p = buf; p < buf + len; p++)
      ਜੇ (eolbyte && *p == '\n')
        *p = '?';

  ਜੇ (needs_further_quoting)
    {
      ਜੇ (MB_CUR_MAX > 1)
        {
          ਅੱਖਰ ਅਚਲ *p = buf;
          ਅੱਖਰ ਅਚਲ *plimit = buf + len;
          ਅੱਖਰ *q = buf;
          displayed_ਚੌੜਾਈ = 0;

          ਜਦੋਂ (p < plimit)
            ਚੋਣ (*p)
              {
                ਹਾਲਤ ' ': ਹਾਲਤ '!': ਹਾਲਤ '"': ਹਾਲਤ '#': ਹਾਲਤ '%':
                ਹਾਲਤ '&': ਹਾਲਤ '\'': ਹਾਲਤ '(': ਹਾਲਤ ')': ਹਾਲਤ '*':
                ਹਾਲਤ '+': ਹਾਲਤ ',': ਹਾਲਤ '-': ਹਾਲਤ '.': ਹਾਲਤ '/':
                ਹਾਲਤ '0': ਹਾਲਤ '1': ਹਾਲਤ '2': ਹਾਲਤ '3': ਹਾਲਤ '4':
                ਹਾਲਤ '5': ਹਾਲਤ '6': ਹਾਲਤ '7': ਹਾਲਤ '8': ਹਾਲਤ '9':
                ਹਾਲਤ ':': ਹਾਲਤ ';': ਹਾਲਤ '<': ਹਾਲਤ '=': ਹਾਲਤ '>':
                ਹਾਲਤ '?':
                ਹਾਲਤ 'A': ਹਾਲਤ 'B': ਹਾਲਤ 'C': ਹਾਲਤ 'D': ਹਾਲਤ 'E':
                ਹਾਲਤ 'F': ਹਾਲਤ 'G': ਹਾਲਤ 'H': ਹਾਲਤ 'I': ਹਾਲਤ 'J':
                ਹਾਲਤ 'K': ਹਾਲਤ 'L': ਹਾਲਤ 'M': ਹਾਲਤ 'N': ਹਾਲਤ 'O':
                ਹਾਲਤ 'P': ਹਾਲਤ 'Q': ਹਾਲਤ 'R': ਹਾਲਤ 'S': ਹਾਲਤ 'T':
                ਹਾਲਤ 'U': ਹਾਲਤ 'V': ਹਾਲਤ 'W': ਹਾਲਤ 'X': ਹਾਲਤ 'Y':
                ਹਾਲਤ 'Z':
                ਹਾਲਤ '[': ਹਾਲਤ '\\': ਹਾਲਤ ']': ਹਾਲਤ '^': ਹਾਲਤ '_':
                ਹਾਲਤ 'a': ਹਾਲਤ 'b': ਹਾਲਤ 'c': ਹਾਲਤ 'd': ਹਾਲਤ 'e':
                ਹਾਲਤ 'f': ਹਾਲਤ 'g': ਹਾਲਤ 'h': ਹਾਲਤ 'i': ਹਾਲਤ 'j':
                ਹਾਲਤ 'k': ਹਾਲਤ 'l': ਹਾਲਤ 'm': ਹਾਲਤ 'n': ਹਾਲਤ 'o':
                ਹਾਲਤ 'p': ਹਾਲਤ 'q': ਹਾਲਤ 'r': ਹਾਲਤ 's': ਹਾਲਤ 't':
                ਹਾਲਤ 'u': ਹਾਲਤ 'v': ਹਾਲਤ 'w': ਹਾਲਤ 'x': ਹਾਲਤ 'y':
                ਹਾਲਤ 'z': ਹਾਲਤ '{': ਹਾਲਤ '|': ਹਾਲਤ '}': ਹਾਲਤ '~':
                  /* These characters are printable ASCII characters.  */
                  *q++ = *p++;
                  displayed_ਚੌੜਾਈ += 1;
                  ਰੋਕੋ;
                ਮੂਲ:
                  /* If we have a multibyte sequence, copy it until we
                     reach its end, replacing each non-printable multibyte
                     character with a single question mark.  */
                  {
                    mbstate_t mbstate; mbszero (&mbstate);
                    ਕਰੋ
                      {
                        char32_t wc;
                        size_t ਬਾਈਟਾਂ;
                        ਪੂਰਨ w;

                        ਬਾਈਟਾਂ = mbrtoc32 (&wc, p, plimit - p, &mbstate);

                        ਜੇ (ਬਾਈਟਾਂ == (size_t) -1)
                          {
                            /* An invalid multibyte sequence was
                               encountered.  Skip one input byte, and
                               put a question mark.  */
                            p++;
                            *q++ = '?';
                            displayed_ਚੌੜਾਈ += 1;
                            ਰੋਕੋ;
                          }

                        ਜੇ (ਬਾਈਟਾਂ == (size_t) -2)
                          {
                            /* An incomplete multibyte character
                               at the end.  Replace it entirely with
                               a question mark.  */
                            p = plimit;
                            *q++ = '?';
                            displayed_ਚੌੜਾਈ += 1;
                            ਰੋਕੋ;
                          }

                        ਜੇ (ਬਾਈਟਾਂ == 0)
                          /* A null wide character was encountered.  */
                          ਬਾਈਟਾਂ = 1;

                        w = c32width (wc);
                        ਜੇ (w >= 0)
                          {
                            /* A printable multibyte character.
                               Keep it.  */
                            ਲਈ (; ਬਾਈਟਾਂ > 0; --ਬਾਈਟਾਂ)
                              *q++ = *p++;
                            displayed_ਚੌੜਾਈ += w;
                          }
                        ਨਹੀਂਤਾਂ
                          {
                            /* An nonprintable multibyte character.
                               Replace it entirely with a question
                               mark.  */
                            p += ਬਾਈਟਾਂ;
                            *q++ = '?';
                            displayed_ਚੌੜਾਈ += 1;
                          }
                      }
                    ਜਦੋਂ (! mbsinit (&mbstate));
                  }
                  ਰੋਕੋ;
              }

          /* The buffer may have shrunk.  */
          len = q - buf;
        }
      ਨਹੀਂਤਾਂ
        {
          ਅੱਖਰ *p = buf;
          ਅੱਖਰ ਅਚਲ *plimit = buf + len;

          ਜਦੋਂ (p < plimit)
            {
              ਜੇ (! isprint (ਮੰਜ਼ਿਲ_uchar (*p)))
                *p = '?';
              p++;
            }
          displayed_ਚੌੜਾਈ = len;
        }
    }
  ਨਹੀਂਤਾਂ ਜੇ (ਚੌੜਾਈ != ਨਿਰਾ)
    {
      ਜੇ (MB_CUR_MAX > 1)
        {
          displayed_ਚੌੜਾਈ = mbsnwidth (buf, len, MBSWIDTH_FLAGS);
          displayed_ਚੌੜਾਈ = MAX (0, displayed_ਚੌੜਾਈ);
        }
      ਨਹੀਂਤਾਂ
        {
          ਅੱਖਰ ਅਚਲ *p = buf;
          ਅੱਖਰ ਅਚਲ *plimit = buf + len;

          displayed_ਚੌੜਾਈ = 0;
          ਜਦੋਂ (p < plimit)
            {
              ਜੇ (isprint (ਮੰਜ਼ਿਲ_uchar (*p)))
                displayed_ਚੌੜਾਈ++;
              p++;
            }
        }
    }

  /* Set padding to better align quoted items,
     and also give a visual indication that quotes are
     not actually part of the name.  */
  *pad = (align_variable_outer_quotes && cwd_some_quoted && ! quoted);

  ਜੇ (ਚੌੜਾਈ != ਨਿਰਾ)
    *ਚੌੜਾਈ = displayed_ਚੌੜਾਈ;

  *inbuf = buf;

  ਵਾਪਸ len;
}

ਸਥਿਰ size_t
quote_name_ਚੌੜਾਈ (ਅੱਖਰ ਅਚਲ *name, ਬਣਤਰ quoting_options ਅਚਲ *options,
                  ਪੂਰਨ needs_general_quoting)
{
  ਅੱਖਰ smallbuf[BUFSIZ];
  ਅੱਖਰ *buf = smallbuf;
  size_t ਚੌੜਾਈ;
  bool pad;

  quote_name_buf (&buf, ਆਕਾਰ smallbuf, (ਅੱਖਰ *) name, options,
                  needs_general_quoting, &ਚੌੜਾਈ, &pad);

  ਜੇ (buf != smallbuf && buf != name)
    ਮੁਕਤ (buf);

  ਚੌੜਾਈ += pad;

  ਵਾਪਸ ਚੌੜਾਈ;
}

/* %XX escape any input out of range as defined in RFC3986,
   and also if PATH, convert all path separators to '/'.  */
ਸਥਿਰ ਅੱਖਰ *
file_escape (ਅੱਖਰ ਅਚਲ *str, bool ਰਸਤਾ)
{
  ਅੱਖਰ *esc = xnmalloc (3, ਲੜੀ_ਲੰਬਾਈ (str) + 1);
  ਅੱਖਰ *p = esc;
  ਜਦੋਂ (*str)
    {
      ਜੇ (ਰਸਤਾ && ISSLASH (*str))
        {
          *p++ = '/';
          str++;
        }
      ਨਹੀਂਤਾਂ ਜੇ (RFC3986[ਮੰਜ਼ਿਲ_uchar (*str)])
        *p++ = *str++;
      ਨਹੀਂਤਾਂ
        p += sprintf (p, "%%%02x", ਮੰਜ਼ਿਲ_uchar (*str++));
    }
  *p = '\0';
  ਵਾਪਸ esc;
}

ਸਥਿਰ size_t
quote_name (ਅੱਖਰ ਅਚਲ *name, ਬਣਤਰ quoting_options ਅਚਲ *options,
            ਪੂਰਨ needs_general_quoting, ਅਚਲ ਬਣਤਰ bin_str *color,
            bool allow_pad, ਬਣਤਰ obstack *ਢੇਰ, ਅੱਖਰ ਅਚਲ *absolute_name)
{
  ਅੱਖਰ smallbuf[BUFSIZ];
  ਅੱਖਰ *buf = smallbuf;
  size_t len;
  bool pad;

  len = quote_name_buf (&buf, ਆਕਾਰ smallbuf, (ਅੱਖਰ *) name, options,
                        needs_general_quoting, ਨਿਰਾ, &pad);

  ਜੇ (pad && allow_pad)
    dired_outbyte (' ');

  ਜੇ (color)
    print_color_indicator (color);

  /* If we're padding, then don't include the outer quotes in
     the --hyperlink, to improve the alignment of those links.  */
  bool skip_quotes = false;

  ਜੇ (absolute_name)
    {
      ਜੇ (align_variable_outer_quotes && cwd_some_quoted && ! pad)
        {
          skip_quotes = true;
          ਅੱਖਰ_ਲਿਖੋ (*buf);
        }
      ਅੱਖਰ *h = file_escape (hostname, /* path= */ false);
      ਅੱਖਰ *n = file_escape (absolute_name, /* path= */ true);
      /* TODO: It would be good to be able to define parameters
         to give hints to the terminal as how best to render the URI.
         For example since ls is outputting a dense block of URIs
         it would be best to not underline by default, and only
         do so upon hover etc.  */
      ਲਿਖੋ ("\033]8;;file://%s%s%s\033\\", h, *n == '/' ? "" : "/", n);
      ਮੁਕਤ (h);
      ਮੁਕਤ (n);
    }

  ਜੇ (ਢੇਰ)
    ਧੱਕੋ_current_dired_pos (ਢੇਰ);

  fwrite (buf + skip_quotes, 1, len - (skip_quotes * 2), stdout);

  dired_pos += len;

  ਜੇ (ਢੇਰ)
    ਧੱਕੋ_current_dired_pos (ਢੇਰ);

  ਜੇ (absolute_name)
    {
      fputs ("\033]8;;\033\\", stdout);
      ਜੇ (skip_quotes)
        ਅੱਖਰ_ਲਿਖੋ (*(buf + len - 1));
    }

  ਜੇ (buf != smallbuf && buf != name)
    ਮੁਕਤ (buf);

  ਵਾਪਸ len + pad;
}

ਸਥਿਰ size_t
print_name_with_quoting (ਅਚਲ ਬਣਤਰ fileinfo *f,
                         bool symlink_ਨਿਸ਼ਾਨਾ,
                         ਬਣਤਰ obstack *ਢੇਰ,
                         size_t ਸ਼ੁਰੂ_ਖਾਨਾ)
{
  ਅੱਖਰ ਅਚਲ *name = symlink_ਨਿਸ਼ਾਨਾ ? f->linkname : f->name;

  ਅਚਲ ਬਣਤਰ bin_str *color
    = print_with_color ? get_color_indicator (f, symlink_ਨਿਸ਼ਾਨਾ) : ਨਿਰਾ;

  bool ਵਰਤਿਆ_color_this_ਸਮਾਂ = (print_with_color
                               && (color || is_colored (C_NORM)));

  size_t len = quote_name (name, filename_quoting_options, f->quoted,
                           color, !symlink_ਨਿਸ਼ਾਨਾ, ਢੇਰ, f->absolute_name);

  process_ਸੰਕੇਤ ();
  ਜੇ (ਵਰਤਿਆ_color_this_ਸਮਾਂ)
    {
      prep_non_filename_ਪਾਠ ();

      /* We use the byte length rather than display width here as
         an optimization to avoid accurately calculating the width,
         because we only output the clear to EOL sequence if the name
         _might_ wrap to the next line.  This may output a sequence
         unnecessarily in multi-byte locales for example,
         but in that case it's inconsequential to the output.  */
      ਜੇ (ਸਤਰ_ਲੰਮਾਈ
          && (ਸ਼ੁਰੂ_ਖਾਨਾ / ਸਤਰ_ਲੰਮਾਈ != (ਸ਼ੁਰੂ_ਖਾਨਾ + len - 1) / ਸਤਰ_ਲੰਮਾਈ))
        put_indicator (&color_indicator[C_CLR_ਮੰਜ਼ਿਲ_EOL]);
    }

  ਵਾਪਸ len;
}

ਸਥਿਰ ਖਾਲੀ
prep_non_filename_ਪਾਠ (ਖਾਲੀ)
{
  ਜੇ (color_indicator[C_ਸਿਰਾ].string != ਨਿਰਾ)
    put_indicator (&color_indicator[C_ਸਿਰਾ]);
  ਨਹੀਂਤਾਂ
    {
      put_indicator (&color_indicator[C_ਖੱਬਾ]);
      put_indicator (&color_indicator[C_RESET]);
      put_indicator (&color_indicator[C_ਸੱਜਾ]);
    }
}

/* Print the file name of 'f' with appropriate quoting.
   Also print file size, inode number, and filetype indicator character,
   as requested by switches.  */

ਸਥਿਰ size_t
print_file_name_and_frills (ਅਚਲ ਬਣਤਰ fileinfo *f, size_t ਸ਼ੁਰੂ_ਖਾਨਾ)
{
  ਅੱਖਰ buf[MAX (LONGEST_HUMAN_READABLE + 1, INT_BUFSIZE_BOUND (uintmax_t))];

  ਸੈੱਟ_ਸਾਧਾਰਨ_color ();

  ਜੇ (print_inode)
    ਲਿਖੋ ("%*s ", format == with_commas ? 0 : inode_ਸੰਖਿਆ_ਚੌੜਾਈ,
            format_inode (buf, f));

  ਜੇ (print_ਖੰਡ_ਮਾਪ)
    {
      ਅੱਖਰ ਅਚਲ *ਖੰਡਾਂ =
        (! f->stat_ok
         ? "?"
         : human_readable (STP_NBLOCKS (&f->stat), buf, human_output_opts,
                           ST_NBLOCKSIZE, output_ਖੰਡ_ਮਾਪ));
      ਪੂਰਨ ਖੰਡਾਂ_ਚੌੜਾਈ = mbswidth (ਖੰਡਾਂ, MBSWIDTH_FLAGS);
      ਪੂਰਨ pad = 0;
      ਜੇ (0 <= ਖੰਡਾਂ_ਚੌੜਾਈ && ਖੰਡ_ਮਾਪ_ਚੌੜਾਈ && format != with_commas)
        pad = ਖੰਡ_ਮਾਪ_ਚੌੜਾਈ - ਖੰਡਾਂ_ਚੌੜਾਈ;
      ਲਿਖੋ ("%*s%s ", pad, "", ਖੰਡਾਂ);
    }

  ਜੇ (print_scontext)
    ਲਿਖੋ ("%*s ", format == with_commas ? 0 : scontext_ਚੌੜਾਈ, f->scontext);

  size_t ਚੌੜਾਈ = print_name_with_quoting (f, false, ਨਿਰਾ, ਸ਼ੁਰੂ_ਖਾਨਾ);

  ਜੇ (indicator_style != none)
    ਚੌੜਾਈ += print_type_indicator (f->stat_ok, f->stat.st_ਬਹੁਲਕ, f->filetype);

  ਵਾਪਸ ਚੌੜਾਈ;
}

/* Given these arguments describing a file, return the single-byte
   type indicator, or 0.  */
ਸਥਿਰ ਅੱਖਰ
get_type_indicator (bool stat_ok, mode_t ਬਹੁਲਕ_2, ਗਿਣਤੀ filetype type)
{
  ਅੱਖਰ c;

  ਜੇ (stat_ok ? S_ISREG (ਬਹੁਲਕ_2) : type == ਸਾਧਾਰਨ)
    {
      ਜੇ (stat_ok && indicator_style == classify && (ਬਹੁਲਕ_2 & S_IXUGO))
        c = '*';
      ਨਹੀਂਤਾਂ
        c = 0;
    }
  ਨਹੀਂਤਾਂ
    {
      ਜੇ (stat_ok ? S_ISDIR (ਬਹੁਲਕ_2) : type == directory || type == arg_directory)
        c = '/';
      ਨਹੀਂਤਾਂ ਜੇ (indicator_style == slash)
        c = 0;
      ਨਹੀਂਤਾਂ ਜੇ (stat_ok ? S_ISLNK (ਬਹੁਲਕ_2) : type == symbolic_link)
        c = '@';
      ਨਹੀਂਤਾਂ ਜੇ (stat_ok ? S_ISFIFO (ਬਹੁਲਕ_2) : type == fifo)
        c = '|';
      ਨਹੀਂਤਾਂ ਜੇ (stat_ok ? S_ISSOCK (ਬਹੁਲਕ_2) : type == sock)
        c = '=';
      ਨਹੀਂਤਾਂ ਜੇ (stat_ok && S_ISDOOR (ਬਹੁਲਕ_2))
        c = '>';
      ਨਹੀਂਤਾਂ
        c = 0;
    }
  ਵਾਪਸ c;
}

ਸਥਿਰ bool
print_type_indicator (bool stat_ok, mode_t ਬਹੁਲਕ_2, ਗਿਣਤੀ filetype type)
{
  ਅੱਖਰ c = get_type_indicator (stat_ok, ਬਹੁਲਕ_2, type);
  ਜੇ (c)
    dired_outbyte (c);
  ਵਾਪਸ !!c;
}

/* Returns if color sequence was printed.  */
ਸਥਿਰ bool
print_color_indicator (ਅਚਲ ਬਣਤਰ bin_str *ind)
{
  ਜੇ (ind)
    {
      /* Need to reset so not dealing with attribute combinations */
      ਜੇ (is_colored (C_NORM))
        restore_default_color ();
      put_indicator (&color_indicator[C_ਖੱਬਾ]);
      put_indicator (ind);
      put_indicator (&color_indicator[C_ਸੱਜਾ]);
    }

  ਵਾਪਸ ind != ਨਿਰਾ;
}

/* Returns color indicator or NULL if none.  */
ATTRIBUTE_ਸ਼ੁੱਧ
ਸਥਿਰ ਅਚਲ ਬਣਤਰ bin_str *
get_color_indicator (ਅਚਲ ਬਣਤਰ fileinfo *f, bool symlink_ਨਿਸ਼ਾਨਾ)
{
  ਗਿਣਤੀ indicator_no type;
  ਬਣਤਰ color_ext_type *ext;	/* Color extension */
  size_t len;			/* Length of name */

  ਅੱਖਰ ਅਚਲ *name;
  mode_t ਬਹੁਲਕ_2;
  ਪੂਰਨ linkok;
  ਜੇ (symlink_ਨਿਸ਼ਾਨਾ)
    {
      name = f->linkname;
      ਬਹੁਲਕ_2 = f->linkmode;
      linkok = f->linkok ? 0 : -1;
    }
  ਨਹੀਂਤਾਂ
    {
      name = f->name;
      ਬਹੁਲਕ_2 = file_or_link_ਬਹੁਲਕ (f);
      linkok = f->linkok;
    }

  /* Is this a nonexistent file?  If so, linkok == -1.  */

  ਜੇ (linkok == -1 && is_colored (C_MISSING))
    type = C_MISSING;
  ਨਹੀਂਤਾਂ ਜੇ (!f->stat_ok)
    {
      ਸਥਿਰ ਗਿਣਤੀ indicator_no ਅਚਲ filetype_indicator[] =
        {
          C_ORPHAN, C_FIFO, C_CHR, C_DIR, C_BLK, C_FILE,
          C_LINK, C_SOCK, C_FILE, C_DIR
        };
      static_assert (countof (filetype_indicator) == filetype_cardinality);
      type = filetype_indicator[f->filetype];
    }
  ਨਹੀਂਤਾਂ
    {
      ਜੇ (S_ISREG (ਬਹੁਲਕ_2))
        {
          type = C_FILE;

          ਜੇ ((ਬਹੁਲਕ_2 & S_ISUID) != 0 && is_colored (C_SETUID))
            type = C_SETUID;
          ਨਹੀਂਤਾਂ ਜੇ ((ਬਹੁਲਕ_2 & S_ISGID) != 0 && is_colored (C_SETGID))
            type = C_SETGID;
          ਨਹੀਂਤਾਂ ਜੇ (f->has_capability)
            type = C_CAP;
          ਨਹੀਂਤਾਂ ਜੇ ((ਬਹੁਲਕ_2 & S_IXUGO) != 0 && is_colored (C_EXEC))
            type = C_EXEC;
          ਨਹੀਂਤਾਂ ਜੇ ((1 < f->stat.st_nlink) && is_colored (C_MULTIHARDLINK))
            type = C_MULTIHARDLINK;
        }
      ਨਹੀਂਤਾਂ ਜੇ (S_ISDIR (ਬਹੁਲਕ_2))
        {
          type = C_DIR;

          ਜੇ ((ਬਹੁਲਕ_2 & S_ISVTX) && (ਬਹੁਲਕ_2 & S_IWOTH)
              && is_colored (C_STICKY_OTHER_WRITABLE))
            type = C_STICKY_OTHER_WRITABLE;
          ਨਹੀਂਤਾਂ ਜੇ ((ਬਹੁਲਕ_2 & S_IWOTH) != 0 && is_colored (C_OTHER_WRITABLE))
            type = C_OTHER_WRITABLE;
          ਨਹੀਂਤਾਂ ਜੇ ((ਬਹੁਲਕ_2 & S_ISVTX) != 0 && is_colored (C_STICKY))
            type = C_STICKY;
        }
      ਨਹੀਂਤਾਂ ਜੇ (S_ISLNK (ਬਹੁਲਕ_2))
        type = C_LINK;
      ਨਹੀਂਤਾਂ ਜੇ (S_ISFIFO (ਬਹੁਲਕ_2))
        type = C_FIFO;
      ਨਹੀਂਤਾਂ ਜੇ (S_ISSOCK (ਬਹੁਲਕ_2))
        type = C_SOCK;
      ਨਹੀਂਤਾਂ ਜੇ (S_ISBLK (ਬਹੁਲਕ_2))
        type = C_BLK;
      ਨਹੀਂਤਾਂ ਜੇ (S_ISCHR (ਬਹੁਲਕ_2))
        type = C_CHR;
      ਨਹੀਂਤਾਂ ਜੇ (S_ISDOOR (ਬਹੁਲਕ_2))
        type = C_DOOR;
      ਨਹੀਂਤਾਂ
        {
          /* Classify a file of some other type as C_ORPHAN.  */
          type = C_ORPHAN;
        }
    }

  /* Check the file's suffix only if still classified as C_FILE.  */
  ext = ਨਿਰਾ;
  ਜੇ (type == C_FILE)
    {
      /* Test if NAME has a recognized suffix.  */

      len = ਲੜੀ_ਲੰਬਾਈ (name);
      name += len;		/* Pointer to final \0.  */
      ਲਈ (ext = color_ext_list; ext != ਨਿਰਾ; ext = ext->ਅਗਲਾ)
        {
          ਜੇ (ext->ext.len <= len)
            {
              ਜੇ (ext->exact_ਮੇਲ)
                {
                  ਜੇ (STREQ_LEN (name - ext->ext.len, ext->ext.string,
                                 ext->ext.len))
                    ਰੋਕੋ;
                }
              ਨਹੀਂਤਾਂ
                {
                  ਜੇ (c_strncasecmp (name - ext->ext.len, ext->ext.string,
                                     ext->ext.len) == 0)
                    ਰੋਕੋ;
                }
            }
        }
    }

  /* Adjust the color for orphaned symlinks.  */
  ਜੇ (type == C_LINK && !linkok)
    {
      ਜੇ (color_symlink_as_referent || is_colored (C_ORPHAN))
        type = C_ORPHAN;
    }

  ਅਚਲ ਬਣਤਰ bin_str *ਅਚਲ s
    = ext ? &(ext->seq) : &color_indicator[type];

  ਵਾਪਸ s->string ? s : ਨਿਰਾ;
}

/* Output a color indicator (which may contain nulls).
   Note we don't update dired_pos here as emacs can be configured
   to handle color codes by stripping/converting them.  */
ਸਥਿਰ ਖਾਲੀ
put_indicator (ਅਚਲ ਬਣਤਰ bin_str *ind)
{
  ਜੇ (! ਵਰਤਿਆ_color)
    {
      ਵਰਤਿਆ_color = true;

      /* If the standard output is a controlling terminal, watch out
         for signals, so that the colors can be restored to the
         default state if "ls" is suspended or interrupted.  */

      ਜੇ (0 <= tcgetpgrp (STDOUT_FILENO))
        signal_init ();

      prep_non_filename_ਪਾਠ ();
    }

  fwrite (ind->string, ind->len, 1, stdout);
}

ਸਥਿਰ size_t
ਲੰਮਾਈ_ਦਾ_file_name_and_frills (ਅਚਲ ਬਣਤਰ fileinfo *f)
{
  size_t len = 0;
  ਅੱਖਰ buf[MAX (LONGEST_HUMAN_READABLE + 1, INT_BUFSIZE_BOUND (uintmax_t))];

  ਜੇ (print_inode)
    len += 1 + (format == with_commas
                ? ਲੜੀ_ਲੰਬਾਈ (umaxtostr (f->stat.st_ino, buf))
                : inode_ਸੰਖਿਆ_ਚੌੜਾਈ);

  ਜੇ (print_ਖੰਡ_ਮਾਪ)
    len += 1 + (format == with_commas
                ? ਲੜੀ_ਲੰਬਾਈ (! f->stat_ok ? "?"
                          : human_readable (STP_NBLOCKS (&f->stat), buf,
                                            human_output_opts, ST_NBLOCKSIZE,
                                            output_ਖੰਡ_ਮਾਪ))
                : ਖੰਡ_ਮਾਪ_ਚੌੜਾਈ);

  ਜੇ (print_scontext)
    len += 1 + (format == with_commas ? ਲੜੀ_ਲੰਬਾਈ (f->scontext) : scontext_ਚੌੜਾਈ);

  len += fileinfo_name_ਚੌੜਾਈ (f);

  ਜੇ (indicator_style != none)
    {
      ਅੱਖਰ c = get_type_indicator (f->stat_ok, f->stat.st_ਬਹੁਲਕ, f->filetype);
      len += (c != 0);
    }

  ਵਾਪਸ len;
}

ਸਥਿਰ ਖਾਲੀ
print_many_ਪ੍ਰਤੀ_ਸਤਰ (ਖਾਲੀ)
{
  idx_t ਖਾਨੇ_ਗਿਣਤੀ_2 = calculate_columns (true);
  ਬਣਤਰ ਕਾਲਮ_info ਅਚਲ *ਸਤਰ_fmt = &ਕਾਲਮ_info[ਖਾਨੇ_ਗਿਣਤੀ_2 - 1];

  /* Calculate the number of rows that will be in each column except possibly
     for a short column on the right.  */
  idx_t ਪੰਕਤੀਆਂ = cwd_n_ਵਰਤਿਆ / ਖਾਨੇ_ਗਿਣਤੀ_2 + (cwd_n_ਵਰਤਿਆ % ਖਾਨੇ_ਗਿਣਤੀ_2 != 0);

  ਲਈ (idx_t ਪੰਕਤੀ = 0; ਪੰਕਤੀ < ਪੰਕਤੀਆਂ; ਪੰਕਤੀ++)
    {
      size_t ਖਾਨਾ = 0;
      idx_t filesno = ਪੰਕਤੀ;
      size_t pos = 0;

      /* Print the next row.  */
      ਜਦੋਂ (true)
        {
          ਬਣਤਰ fileinfo ਅਚਲ *f = sorted_file[filesno];
          size_t name_ਲੰਮਾਈ = ਲੰਮਾਈ_ਦਾ_file_name_and_frills (f);
          size_t max_name_ਲੰਮਾਈ = ਸਤਰ_fmt->ਖਾਨਾ_arr[ਖਾਨਾ++];
          print_file_name_and_frills (f, pos);

          ਜੇ (cwd_n_ਵਰਤਿਆ - ਪੰਕਤੀਆਂ <= filesno)
            ਰੋਕੋ;
          filesno += ਪੰਕਤੀਆਂ;

          indent (pos + name_ਲੰਮਾਈ, pos + max_name_ਲੰਮਾਈ);
          pos += max_name_ਲੰਮਾਈ;
        }
      ਅੱਖਰ_ਲਿਖੋ (eolbyte);
    }
}

ਸਥਿਰ ਖਾਲੀ
print_horizontal (ਖਾਲੀ)
{
  size_t pos = 0;
  idx_t ਖਾਨੇ_ਗਿਣਤੀ_2 = calculate_columns (false);
  ਬਣਤਰ ਕਾਲਮ_info ਅਚਲ *ਸਤਰ_fmt = &ਕਾਲਮ_info[ਖਾਨੇ_ਗਿਣਤੀ_2 - 1];
  ਬਣਤਰ fileinfo ਅਚਲ *f = sorted_file[0];
  size_t name_ਲੰਮਾਈ = ਲੰਮਾਈ_ਦਾ_file_name_and_frills (f);
  size_t max_name_ਲੰਮਾਈ = ਸਤਰ_fmt->ਖਾਨਾ_arr[0];

  /* Print first entry.  */
  print_file_name_and_frills (f, 0);

  /* Now the rest.  */
  ਲਈ (idx_t filesno = 1; filesno < cwd_n_ਵਰਤਿਆ; filesno++)
    {
      idx_t ਖਾਨਾ = filesno % ਖਾਨੇ_ਗਿਣਤੀ_2;

      ਜੇ (ਖਾਨਾ == 0)
        {
          ਅੱਖਰ_ਲਿਖੋ (eolbyte);
          pos = 0;
        }
      ਨਹੀਂਤਾਂ
        {
          indent (pos + name_ਲੰਮਾਈ, pos + max_name_ਲੰਮਾਈ);
          pos += max_name_ਲੰਮਾਈ;
        }

      f = sorted_file[filesno];
      print_file_name_and_frills (f, pos);

      name_ਲੰਮਾਈ = ਲੰਮਾਈ_ਦਾ_file_name_and_frills (f);
      max_name_ਲੰਮਾਈ = ਸਤਰ_fmt->ਖਾਨਾ_arr[ਖਾਨਾ];
    }
  ਅੱਖਰ_ਲਿਖੋ (eolbyte);
}

/* Output name + SEP + ' '.  */

ਸਥਿਰ ਖਾਲੀ
print_with_separator (ਅੱਖਰ sep)
{
  size_t pos = 0;

  ਲਈ (idx_t filesno = 0; filesno < cwd_n_ਵਰਤਿਆ; filesno++)
    {
      ਬਣਤਰ fileinfo ਅਚਲ *f = sorted_file[filesno];
      size_t len = ਸਤਰ_ਲੰਮਾਈ ? ਲੰਮਾਈ_ਦਾ_file_name_and_frills (f) : 0;

      ਜੇ (filesno != 0)
        {
          ਅੱਖਰ separator;
          size_t ਅਗਲਾ_pos = 2 + (sep == ',' && filesno < cwd_n_ਵਰਤਿਆ - 1);

          ਜੇ (! ਸਤਰ_ਲੰਮਾਈ
              || (! ckd_ਜੋੜੋ (&ਅਗਲਾ_pos, ਅਗਲਾ_pos, pos)
                  && ! ckd_ਜੋੜੋ (&ਅਗਲਾ_pos, ਅਗਲਾ_pos, len)
                  && ਅਗਲਾ_pos <= ਸਤਰ_ਲੰਮਾਈ))
            {
              pos += 2;
              separator = ' ';
            }
          ਨਹੀਂਤਾਂ
            {
              pos = 0;
              separator = eolbyte;
            }

          ਅੱਖਰ_ਲਿਖੋ (sep);
          ਅੱਖਰ_ਲਿਖੋ (separator);
        }

      print_file_name_and_frills (f, pos);
      pos += len;
    }
  ਅੱਖਰ_ਲਿਖੋ (eolbyte);
}

/* Assuming cursor is at position FROM, indent up to position TO.
   Use a TAB character instead of two or more spaces whenever possible.  */

ਸਥਿਰ ਖਾਲੀ
indent (size_t from, size_t ਮੰਜ਼ਿਲ)
{
  ਜਦੋਂ (from < ਮੰਜ਼ਿਲ)
    {
      ਜੇ (tabsize != 0 && ਮੰਜ਼ਿਲ / tabsize > (from + 1) / tabsize)
        {
          ਅੱਖਰ_ਲਿਖੋ ('\t');
          from += tabsize - from % tabsize;
        }
      ਨਹੀਂਤਾਂ
        {
          ਅੱਖਰ_ਲਿਖੋ (' ');
          from++;
        }
    }
}

/* Put DIRNAME/NAME into DEST, handling '.' and '/' properly.  */
/* FIXME: maybe remove this function someday.  See about using a
   non-malloc'ing version of file_name_concat.  */

ਸਥਿਰ ਖਾਲੀ
attach (ਅੱਖਰ *dest, ਅੱਖਰ ਅਚਲ *dirname, ਅੱਖਰ ਅਚਲ *name)
{
  ਅੱਖਰ ਅਚਲ *dirnamep = dirname;

  /* Copy dirname if it is not ".".  */
  ਜੇ (dirname[0] != '.' || dirname[1] != 0)
    {
      ਜਦੋਂ (*dirnamep)
        *dest++ = *dirnamep++;
      /* Add '/' if 'dirname' doesn't already end with it.  */
      ਜੇ (dirnamep > dirname && dirnamep[-1] != '/')
        *dest++ = '/';
    }
  ਜਦੋਂ (*name)
    *dest++ = *name++;
  *dest = 0;
}

/* Allocate enough column info suitable for the current number of
   files and display columns, and initialize the info to represent the
   narrowest possible columns.  */

ਸਥਿਰ ਖਾਲੀ
init_ਕਾਲਮ_info (idx_t max_ਖਾਨੇ_ਗਿਣਤੀ)
{
  /* Currently allocated columns in column_info.  */
  ਸਥਿਰ idx_t ਕਾਲਮ_info_alloc;

  ਜੇ (ਕਾਲਮ_info_alloc < max_ਖਾਨੇ_ਗਿਣਤੀ)
    {
      idx_t old_ਕਾਲਮ_info_alloc = ਕਾਲਮ_info_alloc;
      ਕਾਲਮ_info = xpalloc (ਕਾਲਮ_info, &ਕਾਲਮ_info_alloc,
                             max_ਖਾਨੇ_ਗਿਣਤੀ - ਕਾਲਮ_info_alloc, -1,
                             ਆਕਾਰ *ਕਾਲਮ_info);

      /* Allocate the new size_t objects by computing the triangle
         formula n * (n + 1) / 2, except that we don't need to
         allocate the part of the triangle that we've already
         allocated.  Check for address arithmetic overflow.  */
      idx_t ਕਾਲਮ_info_growth = ਕਾਲਮ_info_alloc - old_ਕਾਲਮ_info_alloc, s;
      ਜੇ (ckd_ਜੋੜੋ (&s, old_ਕਾਲਮ_info_alloc + 1, ਕਾਲਮ_info_alloc)
          || ckd_mul (&s, s, ਕਾਲਮ_info_growth))
        xalloc_die ();
      size_t *p = xinmalloc (s >> 1, ਆਕਾਰ *p);

      /* Grow the triangle by parceling out the cells just allocated.  */
      ਲਈ (idx_t i = old_ਕਾਲਮ_info_alloc; i < ਕਾਲਮ_info_alloc; i++)
        {
          ਕਾਲਮ_info[i].ਖਾਨਾ_arr = p;
          p += i + 1;
        }
    }

  ਲਈ (idx_t i = 0; i < max_ਖਾਨੇ_ਗਿਣਤੀ; ++i)
    {
      ਕਾਲਮ_info[i].valid_len = true;
      ਕਾਲਮ_info[i].ਸਤਰ_len = i * MIN_ਕਾਲਮ_ਚੌੜਾਈ + 1;
      ਲਈ (idx_t j = 0; j < i; ++j)
        ਕਾਲਮ_info[i].ਖਾਨਾ_arr[j] = MIN_ਕਾਲਮ_ਚੌੜਾਈ;
      ਕਾਲਮ_info[i].ਖਾਨਾ_arr[i] = 1;
    }
}

/* Calculate the number of columns needed to represent the current set
   of files in the current display width.  */

ਸਥਿਰ idx_t
calculate_columns (bool by_columns)
{
  /* Normally the maximum number of columns is determined by the
     screen width.  But if few files are available this might limit it
     as well.  */
  idx_t max_ਖਾਨੇ_ਗਿਣਤੀ = 0 < max_idx && max_idx < cwd_n_ਵਰਤਿਆ ? max_idx : cwd_n_ਵਰਤਿਆ;

  init_ਕਾਲਮ_info (max_ਖਾਨੇ_ਗਿਣਤੀ);

  /* Compute the maximum number of possible columns.  */
  ਲਈ (idx_t filesno = 0; filesno < cwd_n_ਵਰਤਿਆ; ++filesno)
    {
      ਬਣਤਰ fileinfo ਅਚਲ *f = sorted_file[filesno];
      size_t name_ਲੰਮਾਈ = ਲੰਮਾਈ_ਦਾ_file_name_and_frills (f);

      ਲਈ (idx_t i = 0; i < max_ਖਾਨੇ_ਗਿਣਤੀ; ++i)
        {
          ਜੇ (ਕਾਲਮ_info[i].valid_len)
            {
              idx_t idx = (by_columns
                           ? filesno / ((cwd_n_ਵਰਤਿਆ + i) / (i + 1))
                           : filesno % (i + 1));
              size_t real_ਲੰਮਾਈ = name_ਲੰਮਾਈ + (idx == i ? 0 : 2);

              ਜੇ (ਕਾਲਮ_info[i].ਖਾਨਾ_arr[idx] < real_ਲੰਮਾਈ)
                {
                  ਕਾਲਮ_info[i].ਸਤਰ_len += (real_ਲੰਮਾਈ
                                              - ਕਾਲਮ_info[i].ਖਾਨਾ_arr[idx]);
                  ਕਾਲਮ_info[i].ਖਾਨਾ_arr[idx] = real_ਲੰਮਾਈ;
                  ਕਾਲਮ_info[i].valid_len = (ਕਾਲਮ_info[i].ਸਤਰ_len
                                              <= ਸਤਰ_ਲੰਮਾਈ);
                }
            }
        }
    }

  /* Find maximum allowed columns.  */
  idx_t ਖਾਨੇ_ਗਿਣਤੀ_2;
  ਲਈ (ਖਾਨੇ_ਗਿਣਤੀ_2 = max_ਖਾਨੇ_ਗਿਣਤੀ; 1 < ਖਾਨੇ_ਗਿਣਤੀ_2; --ਖਾਨੇ_ਗਿਣਤੀ_2)
    {
      ਜੇ (ਕਾਲਮ_info[ਖਾਨੇ_ਗਿਣਤੀ_2 - 1].valid_len)
        ਰੋਕੋ;
    }

  ਵਾਪਸ ਖਾਨੇ_ਗਿਣਤੀ_2;
}

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("Usage: %s [OPTION]... [FILE]...\n"), ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
List information about the FILEs (the current directory by ਮੂਲ).\n\
ਛਾਂਟੋ_2 entries alphabetically ਜੇ none ਦਾ -cftuvSUX nor --ਛਾਂਟੋ is specified.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -a, --all\n\
         ਕਰੋ not ignore entries starting with .\n\
"));
      oputs (_("\
  -A, --almost-all\n\
         ਕਰੋ not list implied . and ..\n\
"));
      oputs (_("\
      --author\n\
         with -l, print the author ਦਾ each file\n\
"));
      oputs (_("\
  -b, --escape\n\
         print C-style escapes ਲਈ nongraphic characters\n\
"));
      oputs (_("\
      --ਖੰਡ-ਮਾਪ=ਮਾਪ_2\n\
         with -l, scale sizes by ਮਾਪ_2 when printing them;\n\
         e.g., '--block-size=M'; see ਮਾਪ_2 format ਹੇਠਾਂ\n\
"));
      oputs (_("\
  -B, --ignore-backups\n\
         ਕਰੋ not list implied entries ending with ~\n\
"));
      oputs (_("\
  -c\n\
         with -lt: ਛਾਂਟੋ by, and show, ctime\n\
           (ਸਮਾਂ ਦਾ ਆਖਰੀ change ਦਾ file ਸਥਿਤੀ information);\n\
         with -l: show ctime and ਛਾਂਟੋ by name;\n\
         otherwise: ਛਾਂਟੋ by ctime, newest ਪਹਿਲਾ\n\
"));
      oputs (_("\
  -C\n\
         list entries by columns\n\
"));
      oputs (_("\
      --color[=WHEN]\n\
         color the output WHEN; ਹੋਰ info ਹੇਠਾਂ\n\
"));
      oputs (_("\
  -d, --directory\n\
         list directories themselves, not their contents\n\
"));
      oputs (_("\
  -D, --dired\n\
         generate output designed ਲਈ Emacs' dired ਬਹੁਲਕ_2\n\
"));
      oputs (_("\
  -f\n\
         same as -a -U\n\
"));
      oputs (_("\
  -F, --classify[=WHEN]\n\
         append indicator (one ਦਾ */=>@|) ਮੰਜ਼ਿਲ entries WHEN\n\
"));
      oputs (_("\
      --file-type\n\
         like -F, except ਕਰੋ not append '*'\n\
"));
      oputs (_("\
      --format=ਸ਼ਬਦ\n\
         across,horizontal (-x), commas (-m), ਲੰਮਾ (-l),\n\
         single-ਕਾਲਮ (-1), verbose (-l), vertical (-C)\n\
"));
      oputs (_("\
      --ਭਰਿਆ-ਸਮਾਂ\n\
         like -l --ਸਮਾਂ-style=ਭਰਿਆ-iso\n\
"));
      oputs (_("\
  -g\n\
         like -l, but ਕਰੋ not list owner\n\
"));
      oputs (_("\
      --ਸਮੂਹ-directories-ਪਹਿਲਾ\n\
         ਸਮੂਹ directories before files\n\
"));
      oputs (_("\
  -G, --no-ਸਮੂਹ\n\
         ਵਿੱਚ a ਲੰਮਾ listing, don't print ਸਮੂਹ ਨਾਂ\n\
"));
      oputs (_("\
  -h, --human-readable\n\
         with -l and -s, print sizes like 1K 234M 2G etc.\n\
"));
      oputs (_("\
      --si\n\
         likewise, but use powers ਦਾ 1000 not 1024\n\
"));
      oputs (_("\
  -H, --dereference-command-ਸਤਰ\n\
         follow symbolic links listed on the command ਸਤਰ\n\
"));
      oputs (_("\
      --dereference-command-ਸਤਰ-symlink-ਮੰਜ਼ਿਲ-dir\n\
         follow each command ਸਤਰ symbolic link that points ਮੰਜ਼ਿਲ a directory\n\
"));
      oputs (_("\
      --hide=ਨਮੂਨਾ\n\
         ਕਰੋ not list implied entries matching shell ਨਮੂਨਾ\n\
         (overridden by -a or -A)\n\
"));
      oputs (_("\
      --hyperlink[=WHEN]\n\
         hyperlink file ਨਾਂ WHEN\n\
"));
      oputs (_("\
      --indicator-style=ਸ਼ਬਦ\n\
         append indicator with style ਸ਼ਬਦ ਮੰਜ਼ਿਲ ਇੰਦਰਾਜ ਨਾਂ:\n\
           none (ਮੂਲ), slash (-p), file-type (--file-type), classify (-F)\n\
"));
      oputs (_("\
  -i, --inode\n\
         print the index ਸੰਖਿਆ ਦਾ each file\n\
"));
      oputs (_("\
  -I, --ignore=ਨਮੂਨਾ\n\
         ਕਰੋ not list implied entries matching shell ਨਮੂਨਾ\n\
"));
      oputs (_("\
  -k, --kibibytes\n\
         ਮੂਲ ਮੰਜ਼ਿਲ 1024-byte ਖੰਡਾਂ ਲਈ file system usage;\n\
         ਵਰਤਿਆ only with -s and ਪ੍ਰਤੀ directory totals\n\
"));
      oputs (_("\
  -l\n\
         use a ਲੰਮਾ listing format\n\
"));
      oputs (_("\
  -L, --dereference\n\
         when showing file information ਲਈ a symbolic link,\n\
         show information ਲਈ the file the link references\n\
         rather than ਲਈ the link itself\n\
"));
      oputs (_("\
  -m\n\
         fill ਚੌੜਾਈ with a comma separated list ਦਾ entries\n\
"));
      oputs (_("\
  -n, --numeric-uid-gid\n\
         like -l, but list numeric user and ਸਮੂਹ IDs\n\
"));
      oputs (_("\
  -N, --literal\n\
         print ਇੰਦਰਾਜ ਨਾਂ without quoting\n\
"));
      oputs (_("\
  -o\n\
         like -l, but ਕਰੋ not list ਸਮੂਹ information\n\
"));
      oputs (_("\
  -p, --indicator-style=slash\n\
         append / indicator ਮੰਜ਼ਿਲ directories\n\
"));
      oputs (_("\
  -q, --hide-control-chars\n\
         print ? instead ਦਾ nongraphic characters\n\
"));
      oputs (_("\
      --show-control-chars\n\
         show nongraphic characters as-is;\n\
         the ਮੂਲ, unless ਪ੍ਰੋਗਰਾਮ is 'ls' and output is a terminal\n\
"));
      oputs (_("\
  -Q, --quote-name\n\
         enclose ਇੰਦਰਾਜ ਨਾਂ ਵਿੱਚ ਦੁੱਗਣਾ quotes\n\
"));
      oputs (_("\
      --quoting-style=ਸ਼ਬਦ\n\
         use quoting style ਸ਼ਬਦ ਲਈ ਇੰਦਰਾਜ ਨਾਂ:\n\
           literal, locale, shell, shell-always,\n\
           shell-escape, shell-escape-always, c, escape\n\
         (overrides QUOTING_STYLE environment variable)\n\
"));
      oputs (_("\
  -r, --ਉਲਟ\n\
         ਉਲਟ ਕ੍ਰਮ ਜਦੋਂ ਛਾਂਟੀ\n\
"));
      oputs (_("\
  -R, --recursive\n\
         list subdirectories recursively\n\
"));
      oputs (_("\
  -s, --ਮਾਪ\n\
         print the allocated ਮਾਪ ਦਾ each file, ਵਿੱਚ ਖੰਡਾਂ\n\
"));
      oputs (_("\
  -S\n\
         ਛਾਂਟੋ by file ਮਾਪ, ਸਭ_ਤੋਂ_ਵੱਡਾ ਪਹਿਲਾ\n\
"));
      oputs (_("\
      --ਛਾਂਟੋ=ਸ਼ਬਦ\n\
         change ਮੂਲ 'name' ਛਾਂਟੋ ਮੰਜ਼ਿਲ ਸ਼ਬਦ:\n\
           none (-U), ਮਾਪ (-S), ਸਮਾਂ (-t),\n\
           ਸੰਸਕਰਣ_3 (-v), extension (-X), name, ਚੌੜਾਈ\n\
"));
      oputs (_("\
      --ਸਮਾਂ=ਸ਼ਬਦ\n\
         select which timestamp ਵਰਤਿਆ ਮੰਜ਼ਿਲ display or ਛਾਂਟੋ;\n\
           access ਸਮਾਂ (-u): atime, access, use;\n\
           metadata change ਸਮਾਂ (-c): ctime, ਸਥਿਤੀ;\n\
           modified ਸਮਾਂ (ਮੂਲ): mtime, modification;\n\
           birth ਸਮਾਂ: birth, creation;\n\
         with -l, ਸ਼ਬਦ determines which ਸਮਾਂ ਮੰਜ਼ਿਲ show;\n\
         with --ਛਾਂਟੋ=ਸਮਾਂ, ਛਾਂਟੋ by ਸ਼ਬਦ (newest ਪਹਿਲਾ)\n\
"));
      oputs (_("\
      --ਸਮਾਂ-style=ਸਮਾਂ_STYLE\n\
         ਸਮਾਂ/date format with -l; see ਸਮਾਂ_STYLE ਹੇਠਾਂ\n\
"));
      oputs (_("\
  -t\n\
         ਛਾਂਟੋ by ਸਮਾਂ, newest ਪਹਿਲਾ; see --ਸਮਾਂ\n\
"));
      oputs (_("\
  -T, --tabsize=ਖਾਨੇ_ਗਿਣਤੀ\n\
         assume tab stops at each ਖਾਨੇ_ਗਿਣਤੀ instead ਦਾ 8\n\
"));
      oputs (_("\
  -u\n\
         with -lt: ਛਾਂਟੋ by, and show, access ਸਮਾਂ;\n\
         with -l: show access ਸਮਾਂ and ਛਾਂਟੋ by name;\n\
         otherwise: ਛਾਂਟੋ by access ਸਮਾਂ, newest ਪਹਿਲਾ\n\
"));
      oputs (_("\
  -U\n\
         ਕਰੋ not ਛਾਂਟੋ directory entries\n\
"));
      oputs (_("\
  -v\n\
         natural ਛਾਂਟੋ ਦਾ (ਸੰਸਕਰਣ_3) numbers within ਪਾਠ\n\
"));
      oputs (_("\
  -w, --ਚੌੜਾਈ=ਖਾਨੇ_ਗਿਣਤੀ\n\
         ਸੈੱਟ_2 output ਚੌੜਾਈ ਮੰਜ਼ਿਲ ਖਾਨੇ_ਗਿਣਤੀ.  0 means no ਹੱਦਬੰਦੀ\n\
"));
      oputs (_("\
  -x\n\
         list entries by ਸਤਰਾਂ instead ਦਾ by columns\n\
"));
      oputs (_("\
  -X\n\
         ਛਾਂਟੋ alphabetically by ਇੰਦਰਾਜ extension\n\
"));
      oputs (_("\
  -Z, --context\n\
         print any security context ਦਾ each file\n\
"));
      oputs (_("\
      --zero\n\
         ਸਿਰਾ each output ਸਤਰ with NUL, not newline\n\
"));
      oputs (_("\
  -1\n\
         list one file ਪ੍ਰਤੀ ਸਤਰ\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      emit_ਮਾਪ_note ();
      fputs (_("\
\n\
The ਸਮਾਂ_STYLE ਦਲੀਲ can be ਭਰਿਆ-iso, ਲੰਮਾ-iso, iso, locale, or +FORMAT.\n\
FORMAT is interpreted like ਵਿੱਚ date(1).  If FORMAT is FORMAT1<newline>FORMAT2,\n\
then FORMAT1 applies ਮੰਜ਼ਿਲ non-recent files and FORMAT2 ਮੰਜ਼ਿਲ recent files.\n\
ਸਮਾਂ_STYLE prefixed with 'posix-' takes effect only outside the POSIX locale.\n\
Also the ਸਮਾਂ_STYLE environment variable sets the ਮੂਲ style ਮੰਜ਼ਿਲ use.\n\
"), stdout);
      fputs (_("\
\n\
The WHEN ਦਲੀਲ defaults ਮੰਜ਼ਿਲ 'always' and can also be 'auto' or 'never'.\n\
"), stdout);
      fputs (_("\
\n\
Using color ਮੰਜ਼ਿਲ distinguish file types is disabled both by ਮੂਲ and\n\
with --color=never.  With --color=ਸਵੈਚਾਲਿਤ, ls emits color codes only when\n\
standard output is connected ਮੰਜ਼ਿਲ a terminal.  The LS_COLORS environment\n\
variable can change the settings.  Use the dircolors(1) command ਮੰਜ਼ਿਲ ਸੈੱਟ_2 it.\n\
"), stdout);
      fputs (_("\
\n\
Exit ਸਥਿਤੀ:\n\
 0  ਜੇ OK,\n\
 1  ਜੇ minor problems (e.g., cannot access subdirectory),\n\
 2  ਜੇ serious trouble (e.g., cannot access command-ਸਤਰ ਦਲੀਲ).\n\
"), stdout);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}
