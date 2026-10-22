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

#ضمن <config.h>
#ضمن <sys/types.h>

#ضمن <termios.h>
#إذا HAVE_STROPTS_H
# include <stropts.h>
#endif
#ضمن <sys/ioctl.h>

#ifdef WINSIZE_IN_PTEM
# include <sys/stream.h>
# include <sys/ptem.h>
#endif

#ضمن <stdio.h>
#ضمن <setjmp.h>
#ضمن <pwd.h>
#ضمن <getopt.h>
#ضمن <signal.h>

#إذا HAVE_LANGINFO_CODESET
# include <langinfo.h>
#endif

/* NonStop circa 2011 lacks both SA_RESTART and siginterrupt, so don't
   restart syscalls after a signal handler fires.  This may cause
   colors to get messed up on the screen if 'ls' is interrupted, but
   that's the best we can do on such a platform.  */
#ifndef SA_RESTART
# define SA_RESTART 0
#endif

#ضمن <fnmatch.h>

/* Gnulib includes.  */
#ضمن "acl.h"
#ضمن "areadlink.h"
#ضمن "argmatch.h"
#ضمن "assure.h"
#ضمن "c-ctype.h"
#ضمن "c-strcase.h"
#ضمن "canonicalize.h"
#ضمن "dev-ino.h"
#ضمن "filemode.h"
#ضمن "filenamecat.h"
#ضمن "filevercmp.h"
#ضمن "hard-locale.h"
#ضمن "hash.h"
#ضمن "human.h"
#ضمن "idcache.h"
#ضمن "mbswidth.h"
#ضمن "mpsort.h"
#ضمن "obstack.h"
#ضمن "quote.h"
#ضمن "stat-size.h"
#ضمن "stat-time.h"
#ضمن "strftime.h"
#ضمن "xdectoint.h"
#ضمن "xgethostname.h"
#ضمن "xmemdup0.h"
#ضمن "xstrtol-error.h"
#ضمن "xstrtol.h"

#ضمن "system.h"

#ضمن "dircolors.h"
#ضمن "ls.h"
#ضمن "statx.h"
#ضمن "term-sig.h"

/* Include <sys/capability.h> last to avoid a clash of <sys/types.h>
   include guards with some premature versions of libcap.
   For more details, see <https://bugzilla.redhat.com/483548>.  */
#ifdef HAVE_CAP
# include <sys/capability.h>
#endif

#إذا HAVE_LINUX_XATTR_H
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
#define obstack_chunk_free حرر

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

#إذا READDIR_LIES_ABOUT_MOUNTPOINT_D_INO
# define RELIABLE_D_INO(dp) NOT_AN_INODE_العدد
#وإلا
# define RELIABLE_D_INO(dp) D_INO (dp)
#endif

#إذا ! HAVE_STRUCT_STAT_ST_AUTHOR
# define st_author st_uid
#endif

تعداد filetype
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
تعداد { filetype_cardinality = arg_directory + 1 };

/* Display letters and indicators for each filetype.
   Keep these in sync with enum filetype.  */
ساكن حرف ثابت filetype_letter[] =
  {'?', 'p', 'c', 'd', 'b', '-', 'l', 's', 'w', 'd'};
static_assert (countof (filetype_letter) == filetype_cardinality);

/* Map enum filetype to <dirent.h> d_type values.  */
ساكن unsigned حرف ثابت filetype_d_type[] =
  {
    DT_UNKNOWN, DT_FIFO, DT_CHR, DT_DIR, DT_BLK, DT_REG, DT_LNK, DT_SOCK,
    DT_WHT, DT_DIR
  };
static_assert (countof (filetype_d_type) == filetype_cardinality);

/* Map d_type values to enum filetype.  */
ساكن حرف ثابت d_type_filetype[UCHAR_MAX + 1] =
  {
    [DT_BLK] = blockdev, [DT_CHR] = chardev, [DT_DIR] = directory,
    [DT_FIFO] = fifo, [DT_LNK] = symbolic_link, [DT_REG] = normal,
    [DT_SOCK] = sock, [DT_WHT] = whiteout
  };

تعداد acl_type
  {
    ACL_T_NONE,
    ACL_T_UNKNOWN,
    ACL_T_LSM_CONTEXT_ONLY,
    ACL_T_YES
  };

بنية fileinfo
  {
    /* The file name.  */
    حرف *name;

    /* For symbolic link, name of the file linked to, otherwise zero.  */
    حرف *linkname;

    /* For terminal hyperlinks. */
    حرف *absolute_name;

    بنية stat stat;

    تعداد filetype filetype;

    /* For symbolic link and long listing, st_mode of file linked to, otherwise
       zero.  */
    mode_t linkmode;

    /* security context.  */
    حرف *scontext;

    bool stat_ok;

    /* For symbolic link and color printing, true if linked-to file
       exists, otherwise false.  */
    bool linkok;

    /* For long listings, true if the file has an access control list,
       or a security context.  */
    تعداد acl_type acl_type;

    /* For color listings, true if a regular file has capability info.  */
    bool has_capability;

    /* Whether file name needs quoting. tri-state with -1 == unknown.  */
    صحيح quoted;

    /* Cached screen width (including quoting).  */
    size_t width;
  };

/* Null is a valid character in a color indicator (think about Epson
   printers, for example) so we have to use a length/buffer string
   type.  */

بنية bin_str
  {
    size_t len;			/* Number of bytes */
    حرف ثابت *string;		/* Pointer to the same */
  };

#إذا ! HAVE_TCGETPGRP
# define tcgetpgrp(Fd) 0
#endif

ساكن size_t quote_name (حرف ثابت *name,
                          بنية quoting_options ثابت *options,
                          صحيح needs_general_quoting,
                          ثابت بنية bin_str *color,
                          bool allow_pad, بنية obstack *مكدس,
                          حرف ثابت *absolute_name);
ساكن size_t quote_name_buf (حرف **inbuf, size_t bufsize, حرف *name,
                              بنية quoting_options ثابت *options,
                              صحيح needs_general_quoting, size_t *width,
                              bool *pad);
ساكن صحيح decode_switches (صحيح argc, حرف **argv);
ساكن bool file_ignored (حرف ثابت *name);
ساكن uintmax_t gobble_file (حرف ثابت *name, تعداد filetype type,
                              ino_t inode, bool command_سطر_arg,
                              حرف ثابت *dirname);
ساكن ثابت بنية bin_str * get_color_indicator (ثابت بنية fileinfo *f,
                                                   bool symlink_target);
ساكن bool print_color_indicator (ثابت بنية bin_str *ind);
ساكن فراغ put_indicator (ثابت بنية bin_str *ind);
ساكن فراغ add_ignore_pattern (حرف ثابت *pattern);
ساكن فراغ attach (حرف *dest, حرف ثابت *dirname, حرف ثابت *name);
ساكن فراغ clear_files (فراغ);
ساكن فراغ extract_dirs_from_files (حرف ثابت *dirname,
                                     bool command_سطر_arg);
ساكن bool get_link_name (حرف ثابت *filename, بنية fileinfo *f,
                           bool command_سطر_arg);
ساكن فراغ indent (size_t from, size_t to);
ساكن idx_t calculate_columns (bool by_columns);
ساكن فراغ print_current_files (فراغ);
ساكن فراغ print_dir (حرف ثابت *name, حرف ثابت *realname,
                       bool command_سطر_arg);
ساكن size_t print_file_name_and_frills (ثابت بنية fileinfo *f,
                                          size_t start_col);
ساكن فراغ print_horizontal (فراغ);
ساكن صحيح format_user_width (uid_t u);
ساكن صحيح format_group_width (gid_t g);
ساكن فراغ print_long_format (ثابت بنية fileinfo *f);
ساكن فراغ print_many_per_سطر (فراغ);
ساكن size_t print_name_with_quoting (ثابت بنية fileinfo *f,
                                       bool symlink_target,
                                       بنية obstack *مكدس,
                                       size_t start_col);
ساكن فراغ prep_non_filename_text (فراغ);
ساكن bool print_type_indicator (bool stat_ok, mode_t mode,
                                  تعداد filetype type);
ساكن فراغ print_with_separator (حرف sep);
ساكن فراغ queue_directory (حرف ثابت *name, حرف ثابت *realname,
                             bool command_سطر_arg);
ساكن فراغ sort_files (فراغ);
ساكن فراغ parse_ls_color (فراغ);

ساكن size_t quote_name_width (حرف ثابت *name,
                                بنية quoting_options ثابت *options,
                                صحيح needs_general_quoting);

/* Initial size of hash table.
   Most hierarchies are likely to be shallower than this.  */
تعداد { INITIAL_TABLE_SIZE = 30 };

/* The set of 'active' directories, from the current command-line argument
   to the level in the hierarchy at which files are being listed.
   A directory is represented by its device and inode numbers (struct dev_ino).
   A directory is added to this set when ls begins listing it or its
   entries, and it is removed from the set just after ls has finished
   processing it.  This set is used solely to detect loops, e.g., with
   mkdir loop; cd loop; ln -s ../loop sub; ls -RL  */
ساكن Hash_table *active_dir_set;

#define LOOP_DETECT (!!active_dir_set)

/* The table of files in the current directory:

   'cwd_file' points to a vector of 'struct fileinfo', one per file.
   'cwd_n_alloc' is the number of elements space has been allocated for.
   'cwd_n_used' is the number actually in use.  */

/* Address of block containing the files that are described.  */
ساكن بنية fileinfo *cwd_file;

/* Length of block that 'cwd_file' points to, measured in files.  */
ساكن idx_t cwd_n_alloc;

/* Index of first unused slot in 'cwd_file'.  */
ساكن idx_t cwd_n_used;

/* Whether files needs may need padding due to quoting.  */
ساكن bool cwd_some_quoted;

/* Whether quoting style _may_ add outer quotes,
   and whether aligning those is useful.  */
ساكن bool align_variable_outer_quotes;

/* Vector of pointers to files, in proper sorted order, and the number
   of entries allocated for it.  */
ساكن فراغ **sorted_file;
ساكن size_t sorted_file_alloc;

/* When true, in a color listing, color each symlink name according to the
   type of file it points to.  Otherwise, color them according to the 'ln'
   directive in LS_COLORS.  Dangling (orphan) symlinks are treated specially,
   regardless.  This is set when 'ln=target' appears in LS_COLORS.  */

ساكن bool color_symlink_as_referent;

ساكن حرف ثابت *hostname;

/* Mode of appropriate file for coloring.  */
ساكن mode_t
file_or_link_mode (بنية fileinfo ثابت *file)
{
  إرجاع (color_symlink_as_referent && file->linkok
          ? file->linkmode : file->stat.st_mode);
}


/* Record of one pending directory waiting to be listed.  */

بنية pending
  {
    حرف *name;
    /* If the directory is actually the file pointed to by a symbolic link we
       were told to list, 'realname' will contain the name of the symbolic
       link, otherwise zero.  */
    حرف *realname;
    bool command_سطر_arg;
    بنية pending *next;
  };

ساكن بنية pending *pending_dirs;

/* Current time in seconds and nanoseconds since 1970, updated as
   needed when deciding whether a file is recent.  */

ساكن بنية timespec current_time;

ساكن bool print_scontext;
ساكن حرف UNKNOWN_SECURITY_CONTEXT[] = "?";

/* Whether any of the files has an ACL.  This affects the width of the
   mode column.  */

ساكن bool any_has_acl;

/* The number of columns to use for columns containing inode numbers,
   block sizes, link counts, owners, groups, authors, major device
   numbers, minor device numbers, and file sizes, respectively.  */

ساكن صحيح inode_العدد_width;
ساكن صحيح block_size_width;
ساكن صحيح nlink_width;
ساكن صحيح scontext_width;
ساكن صحيح owner_width;
ساكن صحيح group_width;
ساكن صحيح author_width;
ساكن صحيح major_device_العدد_width;
ساكن صحيح minor_device_العدد_width;
ساكن صحيح file_size_width;

/* Option flags */

/* long_format for lots of info, one per line.
   one_per_line for just names, one per line.
   many_per_line for just names, many per line, sorted vertically.
   horizontal for just names, many per line, sorted horizontally.
   with_commas for just names, many per line, separated by commas.

   -l (and other options that imply -l), -1, -C, -x and -m control
   this parameter.  */

تعداد format
  {
    long_format,		/* -l and other options that imply -l */
    one_per_سطر,		/* -1 */
    many_per_سطر,		/* -C */
    horizontal,			/* -x */
    with_commas			/* -m */
  };

ساكن تعداد format format;

/* 'full-iso' uses full ISO-style dates and times.  'long-iso' uses longer
   ISO-style timestamps, though shorter than 'full-iso'.  'iso' uses shorter
   ISO-style timestamps.  'locale' uses locale-dependent timestamps.  */
تعداد time_style
  {
    full_iso_time_style,	/* --time-style=full-iso */
    long_iso_time_style,	/* --time-style=long-iso */
    iso_time_style,		/* --time-style=iso */
    locale_time_style		/* --time-style=locale */
  };

ساكن حرف ثابت *ثابت time_style_args[] =
{
  "full-iso", "long-iso", "iso", "locale", عدم
};
ساكن تعداد time_style ثابت time_style_types[] =
{
  full_iso_time_style, long_iso_time_style, iso_time_style,
  locale_time_style
};
ARGMATCH_VERIFY (time_style_args, time_style_types);

/* Type of time to print or sort by.  Controlled by -c and -u.
   The values of each item of this enum are important since they are
   used as indices in the sort functions array (see sort_files()).  */

تعداد time_type
  {
    time_mtime = 0,		/* default */
    time_ctime,			/* -c */
    time_atime,			/* -u */
    time_btime,                 /* birth time */
    time_numtypes		/* the number of elements of this enum */
  };

ساكن تعداد time_type time_type;
ساكن bool explicit_time;

/* The file characteristic to sort by.  Controlled by -t, -S, -U, -X, -v.
   The values of each item of this enum are important since they are
   used as indices in the sort functions array (see sort_files()).  */

تعداد sort_type
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

ساكن تعداد sort_type sort_type;

/* Direction of sort.
   false means highest first if numeric,
   lowest first if alphabetic;
   these are the defaults.
   true means the opposite order in each case.  -r  */

ساكن bool sort_reverse;

/* True means to display owner information.  -g turns this off.  */

ساكن bool print_owner = true;

/* True means to display author information.  */

ساكن bool print_author;

/* True means to display group information.  -G and -o turn this off.  */

ساكن bool print_group = true;

/* True means print the user and group id's as numbers rather
   than as names.  -n  */

ساكن bool numeric_ids;

/* True means mention the size in blocks of each file.  -s  */

ساكن bool print_block_size;

/* Human-readable options for output, when printing block counts.  */
ساكن صحيح human_output_opts;

/* The units to use when printing block counts.  */
ساكن uintmax_t output_block_size;

/* Likewise, but for file sizes.  */
ساكن صحيح file_human_output_opts;
ساكن uintmax_t file_output_block_size = 1;

/* Follow the output with a special string.  Using this format,
   Emacs' dired mode starts up twice as fast, and can handle all
   strange characters in file names.  */
ساكن bool dired;

/* 'none' means don't mention the type of files.
   'slash' means mention directories only, with a '/'.
   'file_type' means mention file types.
   'classify' means mention file types and mark executables.

   Controlled by -F, -p, and --indicator-style.  */

تعداد indicator_style
  {
    none = 0,	/*     --indicator-style=none (default) */
    slash,	/* -p, --indicator-style=slash */
    file_type,	/*     --indicator-style=file-type */
    classify	/* -F, --indicator-style=classify */
  };

ساكن تعداد indicator_style indicator_style;

/* Names of indicator styles.  */
ساكن حرف ثابت *ثابت indicator_style_args[] =
{
  "none", "slash", "file-type", "classify", عدم
};
ساكن تعداد indicator_style ثابت indicator_style_types[] =
{
  none, slash, file_type, classify
};
ARGMATCH_VERIFY (indicator_style_args, indicator_style_types);

/* True means use colors to mark types.  Also define the different
   colors as well as the stuff for the LS_COLORS environment variable.
   The LS_COLORS variable is now in a termcap-like format.  */

ساكن bool print_with_color;

ساكن bool print_hyperlink;

/* Whether we used any colors in the output so far.  If so, we will
   need to restore the default color later.  If not, we will need to
   call prep_non_filename_text before using color for the first time. */

ساكن bool used_color = false;

تعداد when_type
  {
    when_never,		/* 0: default or --color=never */
    when_always,	/* 1: --color=always */
    when_if_tty		/* 2: --color=tty */
  };

تعداد Dereference_symlink
  {
    DEREF_UNDEFINED = 0,		/* default */
    DEREF_NEVER,
    DEREF_COMMAND_سطر_ARGUMENTS,	/* -H */
    DEREF_COMMAND_سطر_SYMLINK_TO_DIR,	/* the default, in certain cases */
    DEREF_ALWAYS			/* -L */
  };

تعداد indicator_no
  {
    C_يسار, C_يمين, C_END, C_RESET, C_NORM, C_FILE, C_DIR, C_LINK,
    C_FIFO, C_SOCK,
    C_BLK, C_CHR, C_MISSING, C_ORPHAN, C_EXEC, C_DOOR, C_SETUID, C_SETGID,
    C_STICKY, C_OTHER_WRITABLE, C_STICKY_OTHER_WRITABLE, C_CAP, C_MULTIHARDLINK,
    C_CLR_TO_EOL
  };

ساكن حرف ثابت indicator_name[][2]=
  {
    {'l','c'}, {'r','c'}, {'e','c'}, {'r','s'}, {'n','o'},
    {'f','i'}, {'d','i'}, {'l','n'},
    {'p','i'}, {'s','o'},
    {'b','d'}, {'c','d'}, {'m','i'}, {'o','r'}, {'e','x'},
    {'d','o'}, {'s','u'}, {'s','g'},
    {'s','t'}, {'o','w'}, {'t','w'}, {'c','a'}, {'m','h'},
    {'c','l'}
  };

بنية color_ext_type
  {
    بنية bin_str ext;		/* The extension we're looking for */
    بنية bin_str seq;		/* The sequence to output when we do */
    bool   exact_match;		/* Whether to compare case insensitively */
    بنية color_ext_type *next;	/* Next in list */
  };

ساكن بنية bin_str color_indicator[] =
  {
    { 2, (حرف ثابت []) {'\033','['} },/* lc: Left of color sequence */
    { 1, (حرف ثابت []) {'m'} },	/* rc: Right of color sequence */
    { 0, عدم },			/* ec: End color (replaces lc+rs+rc) */
    { 1, (حرف ثابت []) {'0'} },	/* rs: Reset to ordinary colors */
    { 0, عدم },			/* no: Normal */
    { 0, عدم },			/* fi: File: default */
    { 5, ((حرف ثابت [])
          {'0','1',';','3','4'}) },	/* di: Directory: bright blue */
    { 5, ((حرف ثابت [])
          {'0','1',';','3','6'}) },	/* ln: Symlink: bright cyan */
    { 2, (حرف ثابت []) {'3','3'} },		/* pi: Pipe: yellow/brown */
    { 5, ((حرف ثابت [])
          {'0','1',';','3','5'}) },	/* so: Socket: bright magenta */
    { 5, ((حرف ثابت [])
          {'0','1',';','3','3'}) },	/* bd: Block device: bright yellow */
    { 5, ((حرف ثابت [])
          {'0','1',';','3','3'}) },	/* cd: Char device: bright yellow */
    { 0, عدم },			/* mi: Missing file: undefined */
    { 0, عدم },			/* or: Orphaned symlink: undefined */
    { 5, ((حرف ثابت [])
          {'0','1',';','3','2'}) },	/* ex: Executable: bright green */
    { 5, ((حرف ثابت [])
          {'0','1',';','3','5'}) },	/* do: Door: bright magenta */
    { 5, ((حرف ثابت [])
          {'3','7',';','4','1'}) },	/* su: setuid: white on red */
    { 5, ((حرف ثابت [])
          {'3','0',';','4','3'}) },	/* sg: setgid: black on yellow */
    { 5, ((حرف ثابت [])
          {'3','7',';','4','4'}) },	/* st: sticky: black on blue */
    { 5, ((حرف ثابت [])
          {'3','4',';','4','2'}) },	/* ow: other-writable: blue on green */
    { 5, ((حرف ثابت [])
          {'3','0',';','4','2'}) },	/* tw: ow w/ sticky: black on green */
    { 0, عدم },			/* ca: disabled by default */
    { 0, عدم },			/* mh: disabled by default */
    { 3, ((حرف ثابت [])
          {'\033','[','K'}) },		/* cl: clear to end of line */
  };

/* A list mapping file extensions to corresponding display sequence.  */
ساكن بنية color_ext_type *color_ext_list = عدم;

/* Buffer for color sequences */
ساكن حرف *color_buf;

/* True means to check for orphaned symbolic link, for displaying
   colors, or to group symlink to directories with other dirs.  */

ساكن bool check_symlink_mode;

/* True means mention the inode number of each file.  -i  */

ساكن bool print_inode;

/* What to do with symbolic links.  Affected by -d, -F, -H, -l (and
   other options that imply -l), and -L.  */

ساكن تعداد Dereference_symlink dereference;

/* True means when a directory is found, display info on its
   contents.  -R  */

ساكن bool recursive;

/* True means when an argument is a directory name, display info
   on it itself.  -d  */

ساكن bool immediate_dirs;

/* True means that directories are grouped before files. */

ساكن bool directories_first;

/* Which files to ignore.  */

ساكن تعداد
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

بنية ignore_pattern
  {
    حرف ثابت *pattern;
    بنية ignore_pattern *next;
  };

ساكن بنية ignore_pattern *ignore_patterns;

/* Similar to IGNORE_PATTERNS, except that -a or -A causes this
   variable itself to be ignored.  */
ساكن بنية ignore_pattern *hide_patterns;

/* True means output nongraphic chars in file names as '?'.
   (-q, --hide-control-chars)
   qmark_funny_chars and the quoting style (-Q, --quoting-style=WORD) are
   independent.  The algorithm is: first, obey the quoting style to get a
   string representing the file name;  then, if qmark_funny_chars is set,
   replace all nonprintable chars in that string with '?'.  It's necessary
   to replace nonprintable chars even in quoted strings, because we don't
   want to mess up the terminal if control chars get sent to it, and some
   quoting methods pass through control chars as-is.  */
ساكن bool qmark_funny_chars;

/* True means extra replacement of newline with '?'
   in cases where a newline in a file name would be
   ambiguous with newline separators.  */
ساكن bool qmark_newline;

/* Quoting options for file and dir name output.  */

ساكن بنية quoting_options *filename_quoting_options;
ساكن بنية quoting_options *dirname_quoting_options;

/* The number of chars per hardware tab stop.  Setting this to zero
   inhibits the use of TAB characters for separating columns.  -T */
ساكن size_t tabsize;

/* True means print each directory name before listing it.  */

ساكن bool print_dir_name;

/* The line length to use for breaking lines in many-per-line format.
   Can be set with -w.  If zero, there is no limit.  */

ساكن size_t سطر_length;

/* The local time zone rules, as per the TZ environment variable.  */

ساكن timezone_t localtz;

/* If true, the file listing format requires that stat be called on
   each file.  */

ساكن bool format_needs_stat;

/* Similar to 'format_needs_stat', but set if only the file type is
   needed.  */

ساكن bool format_needs_type;

/* Like 'format_needs_stat', but set only if capability colors are needed.  */

ساكن bool format_needs_capability;

/* An arbitrary limit on the number of bytes in a printed timestamp.
   This is set to a relatively small value to avoid the need to worry
   about denial-of-service attacks on servers that run "ls" on behalf
   of remote clients.  1000 bytes should be enough for any practical
   timestamp format.  */

تعداد { TIME_STAMP_LEN_MAXIMUM = MAX (1000, INT_STRLEN_BOUND (time_t)) };

/* strftime formats for non-recent and recent files, respectively, in
   -l output.  */

ساكن حرف ثابت *long_time_format[2] =
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

ساكن sigset_t caught_signals;

/* If nonzero, the value of the pending fatal signal.  */

ساكن sig_atomic_t متطاير interrupt_signal;

/* A count of the number of pending stop signals that have been received.  */

ساكن sig_atomic_t متطاير stop_signal_تعداد;

/* Desired exit status.  */

ساكن صحيح exit_status;

/* Exit statuses.  */
تعداد
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
تعداد
{
  AUTHOR_OPTION = CHAR_MAX + 1,
  BLOCK_SIZE_OPTION,
  COLOR_OPTION,
  DEREFERENCE_COMMAND_سطر_SYMLINK_TO_DIR_OPTION,
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

ساكن بنية option ثابت long_options[] =
{
  {"all", no_argument, عدم, 'a'},
  {"escape", no_argument, عدم, 'b'},
  {"directory", no_argument, عدم, 'd'},
  {"dired", no_argument, عدم, 'D'},
  {"full-time", no_argument, عدم, FULL_TIME_OPTION},
  {"group-directories-first", no_argument, عدم,
   GROUP_DIRECTORIES_FIRST_OPTION},
  {"human-readable", no_argument, عدم, 'h'},
  {"inode", no_argument, عدم, 'i'},
  {"kibibytes", no_argument, عدم, 'k'},
  {"numeric-uid-gid", no_argument, عدم, 'n'},
  {"no-group", no_argument, عدم, 'G'},
  {"hide-control-chars", no_argument, عدم, 'q'},
  {"reverse", no_argument, عدم, 'r'},
  {"size", no_argument, عدم, 's'},
  {"width", required_argument, عدم, 'w'},
  {"almost-all", no_argument, عدم, 'A'},
  {"ignore-backups", no_argument, عدم, 'B'},
  {"classify", optional_argument, عدم, 'F'},
  {"file-type", no_argument, عدم, FILE_TYPE_INDICATOR_OPTION},
  {"si", no_argument, عدم, SI_OPTION},
  {"dereference-command-line", no_argument, عدم, 'H'},
  {"dereference-command-line-symlink-to-dir", no_argument, عدم,
   DEREFERENCE_COMMAND_سطر_SYMLINK_TO_DIR_OPTION},
  {"hide", required_argument, عدم, HIDE_OPTION},
  {"ignore", required_argument, عدم, 'I'},
  {"indicator-style", required_argument, عدم, INDICATOR_STYLE_OPTION},
  {"dereference", no_argument, عدم, 'L'},
  {"literal", no_argument, عدم, 'N'},
  {"quote-name", no_argument, عدم, 'Q'},
  {"quoting-style", required_argument, عدم, QUOTING_STYLE_OPTION},
  {"recursive", no_argument, عدم, 'R'},
  {"format", required_argument, عدم, FORMAT_OPTION},
  {"show-control-chars", no_argument, عدم, SHOW_CONTROL_CHARS_OPTION},
  {"sort", required_argument, عدم, SORT_OPTION},
  {"tabsize", required_argument, عدم, 'T'},
  {"time", required_argument, عدم, TIME_OPTION},
  {"time-style", required_argument, عدم, TIME_STYLE_OPTION},
  {"zero", no_argument, عدم, ZERO_OPTION},
  {"color", optional_argument, عدم, COLOR_OPTION},
  {"hyperlink", optional_argument, عدم, HYPERLINK_OPTION},
  {"block-size", required_argument, عدم, BLOCK_SIZE_OPTION},
  {"context", no_argument, عدم, 'Z'},
  {"author", no_argument, عدم, AUTHOR_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

ساكن حرف ثابت *ثابت format_args[] =
{
  "verbose", "long", "commas", "horizontal", "across",
  "vertical", "single-column", عدم
};
ساكن تعداد format ثابت format_types[] =
{
  long_format, long_format, with_commas, horizontal, horizontal,
  many_per_سطر, one_per_سطر
};
ARGMATCH_VERIFY (format_args, format_types);

ساكن حرف ثابت *ثابت sort_args[] =
{
  "none", "size", "time", "version", "extension",
  "name", "width", عدم
};
ساكن تعداد sort_type ثابت sort_types[] =
{
  sort_none, sort_size, sort_time, sort_version, sort_extension,
  sort_name, sort_width
};
ARGMATCH_VERIFY (sort_args, sort_types);

ساكن حرف ثابت *ثابت time_args[] =
{
  "atime", "access", "use",
  "ctime", "status",
  "mtime", "modification",
  "birth", "creation",
  عدم
};
ساكن تعداد time_type ثابت time_types[] =
{
  time_atime, time_atime, time_atime,
  time_ctime, time_ctime,
  time_mtime, time_mtime,
  time_btime, time_btime,
};
ARGMATCH_VERIFY (time_args, time_types);

ساكن حرف ثابت *ثابت when_args[] =
{
  /* force and none are for compatibility with another color-ls version */
  "always", "yes", "force",
  "never", "no", "none",
  "auto", "tty", "if-tty", عدم
};
ساكن تعداد when_type ثابت when_types[] =
{
  when_always, when_always, when_always,
  when_never, when_never, when_never,
  when_if_tty, when_if_tty, when_if_tty
};
ARGMATCH_VERIFY (when_args, when_types);

/* Information about filling a column.  */
بنية column_info
{
  bool valid_len;
  size_t سطر_len;
  size_t *col_arr;
};

/* Array with information about column fullness.  */
ساكن بنية column_info *column_info;

/* Maximum number of columns ever possible for this display.  */
ساكن size_t max_idx;

/* The minimum width of a non-final column is 3: 1 character for the name
   and 2 for the separating white space.  The final column is not padded.  */
تعداد { MIN_COLUMN_WIDTH = 3 };


/* This zero-based index is for the --dired option.  It is incremented
   for each byte of output generated by this program so that the beginning
   and ending indices (in that output) of every file name can be recorded
   and later output themselves.  */
ساكن off_t dired_pos;

ساكن فراغ
dired_outbyte (حرف c)
{
  dired_pos++;
  putchar (c);
}

/* Output the buffer S, of length S_LEN, and increment DIRED_POS by S_LEN.  */
ساكن فراغ
dired_outbuf (حرف ثابت *s, size_t s_len)
{
  dired_pos += s_len;
  fwrite (s, حجم *s, s_len, stdout);
}

/* Output the string S, and increment DIRED_POS by its length.  */
ساكن فراغ
dired_outstring (حرف ثابت *s)
{
  dired_outbuf (s, strlen (s));
}

ساكن فراغ
dired_indent (فراغ)
{
  إذا (dired)
    dired_outstring ("  ");
}

/* With --dired, store pairs of beginning and ending indices of file names.  */
ساكن بنية obstack dired_obstack;

/* With --dired, store pairs of beginning and ending indices of any
   directory names that appear as headers (just before 'total' line)
   for lists of directory entries.  Such directory names are seen when
   listing hierarchies using -R and when a directory is listed with at
   least one other command line argument.  */
ساكن بنية obstack subdired_obstack;

/* Save the current index on the specified obstack, OBS.  */
ساكن فراغ
ادفع_current_dired_pos (بنية obstack *obs)
{
  إذا (dired)
    obstack_grow (obs, &dired_pos, حجم dired_pos);
}

/* With -R, this stack is used to help detect directory cycles.
   The device/inode pairs on this stack mirror the pairs in the
   active_dir_set hash table.  */
ساكن بنية obstack dev_ino_obstack;

/* Push a pair onto the device/inode stack.  */
ساكن فراغ
dev_ino_ادفع (dev_t dev, ino_t ino)
{
  فراغ *vdi;
  بنية dev_ino *di;
  صحيح dev_ino_size = حجم *di;
  obstack_blank (&dev_ino_obstack, dev_ino_size);
  vdi = obstack_next_free (&dev_ino_obstack);
  di = vdi;
  di--;
  di->st_dev = dev;
  di->st_ino = ino;
}

/* Pop a dev/ino struct off the global dev_ino_obstack
   and return that struct.  */
ساكن بنية dev_ino
dev_ino_اسحب (فراغ)
{
  فراغ *vdi;
  بنية dev_ino *di;
  صحيح dev_ino_size = حجم *di;
  affirm (dev_ino_size <= obstack_object_size (&dev_ino_obstack));
  obstack_blank_fast (&dev_ino_obstack, -dev_ino_size);
  vdi = obstack_next_free (&dev_ino_obstack);
  di = vdi;
  إرجاع *di;
}

ساكن فراغ
assert_matching_dev_ino (حرف ثابت *name, بنية dev_ino di)
{
  MAYBE_UNUSED بنية stat sb;
  assure (0 <= stat (name, &sb));
  assure (sb.st_dev == di.st_dev);
  assure (sb.st_ino == di.st_ino);
}

ساكن حرف eolbyte = '\n';

/* Write to standard output PREFIX, followed by the quoting style and
   a space-separated list of the integers stored in OS all on one line.  */

ساكن فراغ
dired_dump_obstack (حرف ثابت *prefix, بنية obstack *os)
{
  size_t n_pos;

  n_pos = obstack_object_size (os) / حجم (dired_pos);
  إذا (n_pos > 0)
    {
      off_t *pos = obstack_finish (os);
      fputs (prefix, stdout);
      لكل (size_t i = 0; i < n_pos; i++)
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
ساكن بنية timespec
get_stat_btime (بنية stat ثابت *st)
{
  بنية timespec btimespec;

#إذا HAVE_STATX && defined STATX_INO
  btimespec = get_stat_mtime (st);
#وإلا
  btimespec = get_stat_birthtime (st);
#endif

  إرجاع btimespec;
}

#إذا HAVE_STATX && defined STATX_INO
ATTRIBUTE_PURE
ساكن unsigned صحيح
time_type_to_statx (فراغ)
{
  بدل (time_type)
    {
    حالة time_ctime:
      إرجاع STATX_CTIME;
    حالة time_mtime:
      إرجاع STATX_MTIME;
    حالة time_atime:
      إرجاع STATX_ATIME;
    حالة time_btime:
      إرجاع STATX_BTIME;
    حالة time_numtypes: افتراضي:
      unreachable ();
    }
    إرجاع 0;
}

ATTRIBUTE_PURE
ساكن unsigned صحيح
calc_req_mask (فراغ)
{
  unsigned صحيح mask = STATX_MODE;

  إذا (print_inode)
    mask |= STATX_INO;

  إذا (print_block_size)
    mask |= STATX_BLOCKS;

  إذا (format == long_format) {
    mask |= STATX_NLINK | STATX_SIZE | time_type_to_statx ();
    إذا (print_owner || print_author)
      mask |= STATX_UID;
    إذا (print_group)
      mask |= STATX_GID;
  }

  بدل (sort_type)
    {
    حالة sort_none:
    حالة sort_name:
    حالة sort_version:
    حالة sort_extension:
    حالة sort_width:
      اكسر;
    حالة sort_time:
      mask |= time_type_to_statx ();
      اكسر;
    حالة sort_size:
      mask |= STATX_SIZE;
      اكسر;
    حالة sort_numtypes: افتراضي:
      unreachable ();
    }

  إرجاع mask;
}

ساكن صحيح
do_statx (صحيح fd, حرف ثابت *name, بنية stat *st, صحيح flags,
          unsigned صحيح mask)
{
  بنية statx stx;
  bool want_btime = mask & STATX_BTIME;
  صحيح ret = statx (fd, name, flags | AT_NO_AUTOMOUNT, mask, &stx);
  إذا (ret >= 0)
    {
      statx_to_stat (&stx, st);
      /* Since we only need one timestamp type,
         store birth time in st_mtim.  */
      إذا (want_btime)
        {
          إذا (stx.stx_mask & STATX_BTIME)
            st->st_mtim = statx_timestamp_to_timespec (stx.stx_btime);
          وإلا
            st->st_mtim.tv_sec = st->st_mtim.tv_nsec = -1;
        }
    }

  إرجاع ret;
}

ساكن صحيح
do_stat (حرف ثابت *name, بنية stat *st)
{
  إرجاع do_statx (AT_FDCWD, name, st, 0, calc_req_mask ());
}

ساكن صحيح
do_lstat (حرف ثابت *name, بنية stat *st)
{
  إرجاع do_statx (AT_FDCWD, name, st, AT_SYMLINK_NOFOLLOW, calc_req_mask ());
}

ساكن صحيح
stat_for_mode (حرف ثابت *name, بنية stat *st)
{
  إرجاع do_statx (AT_FDCWD, name, st, 0, STATX_MODE);
}

/* dev+ino should be static, so no need to sync with backing store */
ساكن صحيح
stat_for_ino (حرف ثابت *name, بنية stat *st)
{
  إرجاع do_statx (AT_FDCWD, name, st, 0, STATX_INO);
}

ساكن صحيح
fstat_for_ino (صحيح fd, بنية stat *st)
{
  إرجاع do_statx (fd, "", st, AT_EMPTY_PATH, STATX_INO);
}
#وإلا
ساكن صحيح
do_stat (حرف ثابت *name, بنية stat *st)
{
  إرجاع stat (name, st);
}

ساكن صحيح
do_lstat (حرف ثابت *name, بنية stat *st)
{
  إرجاع lstat (name, st);
}

ساكن صحيح
stat_for_mode (حرف ثابت *name, بنية stat *st)
{
  إرجاع stat (name, st);
}

ساكن صحيح
stat_for_ino (حرف ثابت *name, بنية stat *st)
{
  إرجاع stat (name, st);
}

ساكن صحيح
fstat_for_ino (صحيح fd, بنية stat *st)
{
  إرجاع fstat (fd, st);
}
#endif

/* Return the address of the first plain %b spec in FMT, or NULL if
   there is no such spec.  %5b etc. do not match, so that user
   widths/flags are honored.  */

ATTRIBUTE_PURE
ساكن حرف ثابت *
first_percent_b (حرف ثابت *fmt)
{
  لكل (; *fmt; fmt++)
    إذا (fmt[0] == '%')
      بدل (fmt[1])
        {
        حالة 'b': إرجاع fmt;
        حالة '%': fmt++; اكسر;
        }
  إرجاع عدم;
}

ساكن حرف RFC3986[256];
ساكن فراغ
file_escape_init (فراغ)
{
  لكل (صحيح i = 0; i < 256; i++)
    RFC3986[i] |= c_isalnum (i) || i == '~' || i == '-' || i == '.' || i == '_';
}

تعداد { MBSWIDTH_FLAGS = MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE };

/* Read the abbreviated month names from the locale, to align them
   and to determine the max width of the field and to truncate names
   greater than our max allowed.
   Note even though this handles multibyte locales correctly
   it's not restricted to them as single byte locales can have
   variable width abbreviated months and also precomputing/caching
   the names was seen to increase the performance of ls significantly.  */

/* abformat[RECENT][MON] is the format to use for timestamps with
   recentness RECENT and month MON.  */
تعداد { ABFORMAT_SIZE = 128 };
ساكن حرف abformat[2][12][ABFORMAT_SIZE];
/* True if precomputed formats should be used.  This can be false if
   nl_langinfo fails, if a format or month abbreviation is unusually
   long, or if a month abbreviation contains '%'.  */
ساكن bool use_abformat;

/* Store into ABMON the abbreviated month names, suitably aligned.
   Return true if successful.  */

ساكن bool
abmon_init (حرف abmon[12][ABFORMAT_SIZE])
{
#ifndef HAVE_NL_LANGINFO
  إرجاع false;
#وإلا
  صحيح max_mon_width = 0;
  صحيح mon_width[12];
  صحيح mon_len[12];

  لكل (صحيح i = 0; i < 12; i++)
    {
      حرف ثابت *abbr = nl_langinfo (ABMON_1 + i);
      mon_len[i] = strnlen (abbr, ABFORMAT_SIZE);
      إذا (mon_len[i] == ABFORMAT_SIZE)
        إرجاع false;
      إذا (strchr (abbr, '%'))
        إرجاع false;
      mon_width[i] = mbswidth (strcpy (abmon[i], abbr), MBSWIDTH_FLAGS);
      إذا (mon_width[i] < 0)
        إرجاع false;
      max_mon_width = MAX (max_mon_width, mon_width[i]);
    }

  لكل (صحيح i = 0; i < 12; i++)
    {
      صحيح fill = max_mon_width - mon_width[i];
      إذا (ABFORMAT_SIZE - mon_len[i] <= fill)
        إرجاع false;
      bool align_يسار = !c_isdigit (abmon[i][0]);
      صحيح fill_offset;
      إذا (align_يسار)
        fill_offset = mon_len[i];
      وإلا
        {
          memmove (abmon[i] + fill, abmon[i], mon_len[i]);
          fill_offset = 0;
        }
      memset (abmon[i] + fill_offset, ' ', fill);
      abmon[i][mon_len[i] + fill] = '\0';
    }

  إرجاع true;
#endif
}

/* Initialize ABFORMAT and USE_ABFORMAT.  */

ساكن فراغ
abformat_init (فراغ)
{
  حرف ثابت *pb[2];
  لكل (صحيح recent = 0; recent < 2; recent++)
    pb[recent] = first_percent_b (long_time_format[recent]);
  إذا (! (pb[0] || pb[1]))
    إرجاع;

  حرف abmon[12][ABFORMAT_SIZE];
  إذا (! abmon_init (abmon))
    إرجاع;

  لكل (صحيح recent = 0; recent < 2; recent++)
    {
      حرف ثابت *fmt = long_time_format[recent];
      لكل (صحيح i = 0; i < 12; i++)
        {
          حرف *nfmt = abformat[recent][i];
          صحيح nbytes;

          إذا (! pb[recent])
            nbytes = snprintf (nfmt, ABFORMAT_SIZE, "%s", fmt);
          وإلا
            {
              إذا (! (pb[recent] - fmt <= MIN (ABFORMAT_SIZE, INT_MAX)))
                إرجاع;
              صحيح prefix_len = pb[recent] - fmt;
              nbytes = snprintf (nfmt, ABFORMAT_SIZE, "%.*s%s%s",
                                 prefix_len, fmt, abmon[i], pb[recent] + 2);
            }

          إذا (! (0 <= nbytes && nbytes < ABFORMAT_SIZE))
            إرجاع;
        }
    }

  use_abformat = true;
}

ساكن size_t
dev_ino_hash (فراغ ثابت *x, size_t table_size)
{
  بنية dev_ino ثابت *p = x;
  إرجاع (uintmax_t) {p->st_ino} % table_size;
}

ساكن bool
dev_ino_compare (فراغ ثابت *x, فراغ ثابت *y)
{
  بنية dev_ino ثابت *a = x;
  بنية dev_ino ثابت *b = y;
  إرجاع PSAME_INODE (a, b);
}

ساكن فراغ
dev_ino_free (فراغ *x)
{
  حرر (x);
}

/* Add the device/inode pair (P->st_dev/P->st_ino) to the set of
   active directories.  Return true if there is already a matching
   entry in the table.  */

ساكن bool
visit_dir (dev_t dev, ino_t ino)
{
  بنية dev_ino *ent;
  بنية dev_ino *ent_from_table;
  bool found_match;

  ent = xmalloc (حجم *ent);
  ent->st_ino = ino;
  ent->st_dev = dev;

  /* Attempt to insert this entry into the table.  */
  ent_from_table = hash_أدرج (active_dir_set, ent);

  إذا (ent_from_table == عدم)
    {
      /* Insertion failed due to lack of memory.  */
      xalloc_die ();
    }

  found_match = (ent_from_table != ent);

  إذا (found_match)
    {
      /* ent was not inserted, so free it.  */
      حرر (ent);
    }

  إرجاع found_match;
}

ساكن فراغ
free_pending_ent (بنية pending *p)
{
  حرر (p->name);
  حرر (p->realname);
  حرر (p);
}

ساكن bool
is_colored (تعداد indicator_no type)
{
  /* Return true unless the string is "", "0" or "00"; try to be efficient.  */
  size_t len = color_indicator[type].len;
  إذا (len == 0)
    إرجاع false;
  إذا (2 < len)
    إرجاع true;
  حرف ثابت *s = color_indicator[type].string;
  إرجاع (s[0] != '0') | (s[len - 1] != '0');
}

ساكن فراغ
restore_default_color (فراغ)
{
  put_indicator (&color_indicator[C_يسار]);
  put_indicator (&color_indicator[C_يمين]);
}

ساكن فراغ
set_normal_color (فراغ)
{
  إذا (print_with_color && is_colored (C_NORM))
    {
      put_indicator (&color_indicator[C_يسار]);
      put_indicator (&color_indicator[C_NORM]);
      put_indicator (&color_indicator[C_يمين]);
    }
}

/* An ordinary signal was received; arrange for the program to exit.  */

ساكن فراغ
sighandler (صحيح sig)
{
  إذا (! interrupt_signal)
    interrupt_signal = sig;
}

/* A SIGTSTP was received; arrange for the program to suspend itself.  */

ساكن فراغ
stophandler (MAYBE_UNUSED صحيح sig)
{
  إذا (! interrupt_signal)
    stop_signal_تعداد++;
}

/* Process any pending signals.  If signals are caught, this function
   should be called periodically.  Ideally there should never be an
   unbounded amount of time when signals are not being processed.
   Signal handling can restore the default colors, so callers must
   immediately change colors after invoking this function.  */

ساكن فراغ
process_signals (فراغ)
{
  بينما (interrupt_signal || stop_signal_تعداد)
    {
      صحيح sig;
      صحيح stops;
      sigset_t oldset;

      إذا (used_color)
        restore_default_color ();
      fflush (stdout);

      sigprocmask (SIG_BLOCK, &caught_signals, &oldset);

      /* Reload interrupt_signal and stop_signal_count, in case a new
         signal was handled before sigprocmask took effect.  */
      sig = interrupt_signal;
      stops = stop_signal_تعداد;

      /* SIGTSTP is special, since the application can receive that signal
         more than once.  In this case, don't set the signal handler to the
         default.  Instead, just raise the uncatchable SIGSTOP.  */
      إذا (stops)
        {
          stop_signal_تعداد = stops - 1;
          sig = SIGSTOP;
        }
      وإلا
        إشارة (sig, SIG_DFL);

      /* Exit or suspend the program.  */
      ارفع (sig);
      sigprocmask (SIG_SETMASK, &oldset, عدم);

      /* If execution reaches here, then the program has been
         continued (after being suspended).  */
    }
}

/* Setup signal handlers if INIT is true,
   otherwise restore to the default.  */

ساكن فراغ
signal_setup (bool init)
{
  /* The signals that are trapped, and the number of such signals.  */
  ساكن صحيح ثابت stop_sig[] =
    {
      /* This one is handled specially.  */
      SIGTSTP,
    };

  تعداد { nsigs = countof (term_sig), nstop = countof (stop_sig) };

  إذا (init)
    {
      بنية sigaction act;

      sigemptyset (&caught_signals);
      لكل (صحيح j = 0; j < nsigs + nstop; j++)
        {
          صحيح sig = j < nsigs ? term_sig[j]: stop_sig[j - nsigs];
          sigaction (sig, عدم, &act);
          إذا (act.sa_handler != SIG_IGN)
            sigaddset (&caught_signals, sig);
        }

      act.sa_mask = caught_signals;
      act.sa_flags = SA_RESTART;

      لكل (صحيح j = 0; j < nsigs + nstop; j++)
        {
          صحيح sig = j < nsigs ? term_sig[j]: stop_sig[j - nsigs];
          إذا (sigismember (&caught_signals, sig))
            {
              act.sa_handler = j < nsigs ? sighandler : stophandler;
              sigaction (sig, &act, عدم);
            }
        }
    }
  وإلا /* restore.  */
    {
      لكل (صحيح j = 0; j < nsigs + nstop; j++)
        {
          صحيح sig = j < nsigs ? term_sig[j]: stop_sig[j - nsigs];
          إذا (sigismember (&caught_signals, sig))
            إشارة (sig, SIG_DFL);
        }
    }
}

ساكن فراغ
signal_init (فراغ)
{
  signal_setup (true);
}

ساكن فراغ
signal_restore (فراغ)
{
  signal_setup (false);
}

صحيح
الرئيسية (صحيح argc, حرف **argv)
{
  صحيح i;
  بنية pending *thispend;
  صحيح n_files;

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
  pending_dirs = عدم;

  current_time.tv_sec = TYPE_MINIMUM (time_t);
  current_time.tv_nsec = -1;

  i = decode_switches (argc, argv);

  إذا (print_with_color)
    parse_ls_color ();

  /* Test print_with_color again, because the call to parse_ls_color
     may have just reset it -- e.g., if LS_COLORS is invalid.  */

  إذا (print_with_color)
    {
      /* Don't use TAB characters in output.  Some terminal
         emulators can't handle the combination of tabs and
         color codes on the same line.  */
      tabsize = 0;
    }

  إذا (directories_first)
    check_symlink_mode = true;
  وإلا إذا (print_with_color)
    {
      /* Avoid following symbolic links when possible.  */
      إذا (is_colored (C_ORPHAN)
          || (is_colored (C_EXEC) && color_symlink_as_referent)
          || (is_colored (C_MISSING) && format == long_format))
        check_symlink_mode = true;
    }

  إذا (dereference == DEREF_UNDEFINED)
    dereference = ((immediate_dirs
                    || indicator_style == classify
                    || format == long_format)
                   ? DEREF_NEVER
                   : DEREF_COMMAND_سطر_SYMLINK_TO_DIR);

  /* When using -R, initialize a data structure we'll use to
     detect any directory cycles.  */
  إذا (recursive)
    {
      active_dir_set = hash_initialize (INITIAL_TABLE_SIZE, عدم,
                                        dev_ino_hash,
                                        dev_ino_compare,
                                        dev_ino_free);
      إذا (active_dir_set == عدم)
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

  إذا (dired)
    {
      obstack_init (&dired_obstack);
      obstack_init (&subdired_obstack);
    }

  إذا (print_hyperlink)
    {
      file_escape_init ();

      hostname = xgethostname ();
      /* The hostname is generally ignored,
         so ignore failures obtaining it.  */
      إذا (! hostname)
        hostname = "";
    }

  cwd_n_alloc = 100;
  cwd_file = xmalloc (cwd_n_alloc * حجم *cwd_file);
  cwd_n_used = 0;

  clear_files ();

  n_files = argc - i;

  إذا (n_files <= 0)
    {
      إذا (immediate_dirs)
        gobble_file (".", directory, NOT_AN_INODE_العدد, true, عدم);
      وإلا
        queue_directory (".", عدم, true);
    }
  وإلا
    افعل
      gobble_file (argv[i++], unknown, NOT_AN_INODE_العدد, true, عدم);
    بينما (i < argc);

  إذا (cwd_n_used)
    {
      sort_files ();
      إذا (!immediate_dirs)
        extract_dirs_from_files (عدم, true);
      /* 'cwd_n_used' might be zero now.  */
    }

  /* In the following if/else blocks, it is sufficient to test 'pending_dirs'
     (and not pending_dirs->name) because there may be no markers in the queue
     at this point.  A marker may be enqueued when extract_dirs_from_files is
     called with a non-empty string or via print_dir.  */
  إذا (cwd_n_used)
    {
      print_current_files ();
      إذا (pending_dirs)
        dired_outbyte ('\n');
    }
  وإلا إذا (n_files <= 1 && pending_dirs && !pending_dirs->next)
    print_dir_name = false;

  بينما (pending_dirs)
    {
      thispend = pending_dirs;
      pending_dirs = pending_dirs->next;

      إذا (LOOP_DETECT)
        {
          إذا (thispend->name == عدم)
            {
              /* thispend->name == NULL means this is a marker entry
                 indicating we've finished processing the directory.
                 Use its dev/ino numbers to remove the corresponding
                 entry from the active_dir_set hash table.  */
              بنية dev_ino di = dev_ino_اسحب ();
              بنية dev_ino *found = hash_remove (active_dir_set, &di);
              إذا (false)
                assert_matching_dev_ino (thispend->realname, di);
              affirm (found);
              dev_ino_free (found);
              free_pending_ent (thispend);
              استمر;
            }
        }

      print_dir (thispend->name, thispend->realname,
                 thispend->command_سطر_arg);

      free_pending_ent (thispend);
      print_dir_name = true;
    }

  إذا (print_with_color && used_color)
    {

      /* Skip the restore when it would be a no-op, i.e.,
         when left is "\033[" and right is "m".  */
      إذا (!(color_indicator[C_يسار].len == 2
            && memeq (color_indicator[C_يسار].string, "\033[", 2)
            && color_indicator[C_يمين].len == 1
            && color_indicator[C_يمين].string[0] == 'm'))
        restore_default_color ();

      fflush (stdout);

      signal_restore ();

      /* Act on any signals that arrived before the default was restored.
         This can process signals out of order, but there doesn't seem to
         be an easy way to do them in order, and the order isn't that
         important anyway.  */
      لكل (صحيح j = stop_signal_تعداد; j; j--)
        ارفع (SIGSTOP);
      صحيح j = interrupt_signal;
      إذا (j)
        ارفع (j);
    }

  إذا (dired)
    {
      /* No need to free these since we're about to exit.  */
      dired_dump_obstack ("//DIRED//", &dired_obstack);
      dired_dump_obstack ("//SUBDIRED//", &subdired_obstack);
      printf ("//DIRED-OPTIONS// --quoting-style=%s\n",
              quoting_style_args[get_quoting_style (filename_quoting_options)]);
    }

  إذا (LOOP_DETECT)
    {
      assure (hash_get_n_entries (active_dir_set) == 0);
      hash_free (active_dir_set);
    }

  إرجاع exit_status;
}

/* Return the line length indicated by the value given by SPEC, or -1
   if unsuccessful.  0 means no limit on line length.  */

ساكن ptrdiff_t
decode_سطر_length (حرف ثابت *spec)
{
  uintmax_t val;

  /* Treat too-large values as if they were 0, which is
     effectively infinity.  */
  بدل (xstrtoumax (spec, عدم, 0, &val, ""))
    {
    حالة LONGINT_OK:
      إرجاع val <= MIN (PTRDIFF_MAX, SIZE_MAX) ? val : 0;

    حالة LONGINT_OVERFLOW:
      إرجاع 0;

    حالة LONGINT_INVALID:
    حالة LONGINT_INVALID_SUFFIX_CHAR:
    حالة LONGINT_INVALID_SUFFIX_CHAR_WITH_OVERFLOW:
      إرجاع -1;

    افتراضي:
      unreachable ();
    }
}

/* Return true if standard output is a tty, caching the result.  */

ساكن bool
stdout_isatty (فراغ)
{
  ساكن signed حرف out_tty = -1;
  إذا (out_tty < 0)
    out_tty = isatty (STDOUT_FILENO);
  assume (out_tty == 0 || out_tty == 1);
  إرجاع out_tty;
}

/* Set all the option flags according to the switches specified.
   Return the index of the first non-option argument.  */

ساكن صحيح
decode_switches (صحيح argc, حرف **argv)
{
  حرف *time_style_option = عدم;

  /* These variables are false or -1 unless a switch says otherwise.  */
  bool kibibytes_specified = false;
  صحيح format_opt = -1;
  صحيح hide_control_chars_opt = -1;
  صحيح quoting_style_opt = -1;
  صحيح sort_opt = -1;
  ptrdiff_t tabsize_opt = -1;
  ptrdiff_t width_opt = -1;

  بينما (true)
    {
      صحيح oi = -1;
      صحيح c = getopt_long (argc, argv,
                           "abcdfghiklmnopqrstuvw:xABCDFGHI:LNQRST:UXZ1",
                           long_options, &oi);
      إذا (c == -1)
        اكسر;

      بدل (c)
        {
        حالة 'a':
          ignore_mode = IGNORE_MINIMAL;
          اكسر;

        حالة 'b':
          quoting_style_opt = escape_quoting_style;
          اكسر;

        حالة 'c':
          time_type = time_ctime;
          explicit_time = true;
          اكسر;

        حالة 'd':
          immediate_dirs = true;
          اكسر;

        حالة 'f':
          ignore_mode = IGNORE_MINIMAL; /* enable -a */
          sort_opt = sort_none;         /* enable -U */
          اكسر;

        حالة FILE_TYPE_INDICATOR_OPTION: /* --file-type */
          indicator_style = file_type;
          اكسر;

        حالة 'g':
          format_opt = long_format;
          print_owner = false;
          اكسر;

        حالة 'h':
          file_human_output_opts = human_output_opts =
            human_autoscale | human_SI | human_base_1024;
          file_output_block_size = output_block_size = 1;
          اكسر;

        حالة 'i':
          print_inode = true;
          اكسر;

        حالة 'k':
          kibibytes_specified = true;
          اكسر;

        حالة 'l':
          format_opt = long_format;
          اكسر;

        حالة 'm':
          format_opt = with_commas;
          اكسر;

        حالة 'n':
          numeric_ids = true;
          format_opt = long_format;
          اكسر;

        حالة 'o':  /* Just like -l, but don't display group info.  */
          format_opt = long_format;
          print_group = false;
          اكسر;

        حالة 'p':
          indicator_style = slash;
          اكسر;

        حالة 'q':
          hide_control_chars_opt = true;
          اكسر;

        حالة 'r':
          sort_reverse = true;
          اكسر;

        حالة 's':
          print_block_size = true;
          اكسر;

        حالة 't':
          sort_opt = sort_time;
          اكسر;

        حالة 'u':
          time_type = time_atime;
          explicit_time = true;
          اكسر;

        حالة 'v':
          sort_opt = sort_version;
          اكسر;

        حالة 'w':
          width_opt = decode_سطر_length (optarg);
          إذا (width_opt < 0)
            error (LS_FAILURE, 0, "%s: %s", _("invalid line width"),
                   quote (optarg));
          اكسر;

        حالة 'x':
          format_opt = horizontal;
          اكسر;

        حالة 'A':
          ignore_mode = IGNORE_DOT_AND_DOTDOT;
          اكسر;

        حالة 'B':
          add_ignore_pattern ("*~");
          add_ignore_pattern (".*~");
          اكسر;

        حالة 'C':
          format_opt = many_per_سطر;
          اكسر;

        حالة 'D':
          format_opt = long_format;
          print_hyperlink = false;
          dired = true;
          اكسر;

        حالة 'F':
          {
            صحيح i;
            إذا (optarg)
              i = XARGMATCH ("--classify", optarg, when_args, when_types);
            وإلا
              /* Using --classify with no argument is equivalent to using
                 --classify=always.  */
              i = when_always;

            إذا (i == when_always || (i == when_if_tty && stdout_isatty ()))
              indicator_style = classify;
            اكسر;
          }

        حالة 'G':		/* inhibit display of group info */
          print_group = false;
          اكسر;

        حالة 'H':
          dereference = DEREF_COMMAND_سطر_ARGUMENTS;
          اكسر;

        حالة DEREFERENCE_COMMAND_سطر_SYMLINK_TO_DIR_OPTION:
          dereference = DEREF_COMMAND_سطر_SYMLINK_TO_DIR;
          اكسر;

        حالة 'I':
          add_ignore_pattern (optarg);
          اكسر;

        حالة 'L':
          dereference = DEREF_ALWAYS;
          اكسر;

        حالة 'N':
          quoting_style_opt = literal_quoting_style;
          اكسر;

        حالة 'Q':
          quoting_style_opt = c_quoting_style;
          اكسر;

        حالة 'R':
          recursive = true;
          اكسر;

        حالة 'S':
          sort_opt = sort_size;
          اكسر;

        حالة 'T':
          tabsize_opt = xnumtoumax (optarg, 0, 0, MIN (PTRDIFF_MAX, SIZE_MAX),
                                    "", _("invalid tab size"), LS_FAILURE, 0);
          اكسر;

        حالة 'U':
          sort_opt = sort_none;
          اكسر;

        حالة 'X':
          sort_opt = sort_extension;
          اكسر;

        حالة '1':
          /* -1 has no effect after -l.  */
          إذا (format_opt != long_format)
            format_opt = one_per_سطر;
          اكسر;

        حالة AUTHOR_OPTION:
          print_author = true;
          اكسر;

        حالة HIDE_OPTION:
          {
            بنية ignore_pattern *hide = xmalloc (حجم *hide);
            hide->pattern = optarg;
            hide->next = hide_patterns;
            hide_patterns = hide;
          }
          اكسر;

        حالة SORT_OPTION:
          sort_opt = XARGMATCH ("--sort", optarg, sort_args, sort_types);
          اكسر;

        حالة GROUP_DIRECTORIES_FIRST_OPTION:
          directories_first = true;
          اكسر;

        حالة TIME_OPTION:
          time_type = XARGMATCH ("--time", optarg, time_args, time_types);
          explicit_time = true;
          اكسر;

        حالة FORMAT_OPTION:
          format_opt = XARGMATCH ("--format", optarg, format_args,
                                  format_types);
          اكسر;

        حالة FULL_TIME_OPTION:
          format_opt = long_format;
          time_style_option = (حرف *) "full-iso";
          اكسر;

        حالة COLOR_OPTION:
          {
            صحيح i;
            إذا (optarg)
              i = XARGMATCH ("--color", optarg, when_args, when_types);
            وإلا
              /* Using --color with no argument is equivalent to using
                 --color=always.  */
              i = when_always;

            print_with_color = (i == when_always
                                || (i == when_if_tty && stdout_isatty ()));
            اكسر;
          }

        حالة HYPERLINK_OPTION:
          {
            صحيح i;
            إذا (optarg)
              i = XARGMATCH ("--hyperlink", optarg, when_args, when_types);
            وإلا
              /* Using --hyperlink with no argument is equivalent to using
                 --hyperlink=always.  */
              i = when_always;

            print_hyperlink = (i == when_always
                               || (i == when_if_tty && stdout_isatty ()));
            اكسر;
          }

        حالة INDICATOR_STYLE_OPTION:
          indicator_style = XARGMATCH ("--indicator-style", optarg,
                                       indicator_style_args,
                                       indicator_style_types);
          اكسر;

        حالة QUOTING_STYLE_OPTION:
          quoting_style_opt = XARGMATCH ("--quoting-style", optarg,
                                         quoting_style_args,
                                         quoting_style_vals);
          اكسر;

        حالة TIME_STYLE_OPTION:
          time_style_option = optarg;
          اكسر;

        حالة SHOW_CONTROL_CHARS_OPTION:
          hide_control_chars_opt = false;
          اكسر;

        حالة BLOCK_SIZE_OPTION:
          {
            تعداد strtol_error e = human_options (optarg, &human_output_opts,
                                                 &output_block_size);
            إذا (e != LONGINT_OK)
              xstrtol_fatal (e, oi, 0, long_options, optarg);
            file_human_output_opts = human_output_opts;
            file_output_block_size = output_block_size;
          }
          اكسر;

        حالة SI_OPTION:
          file_human_output_opts = human_output_opts =
            human_autoscale | human_SI;
          file_output_block_size = output_block_size = 1;
          اكسر;

        حالة 'Z':
          print_scontext = true;
          اكسر;

        حالة ZERO_OPTION:
          eolbyte = 0;
          hide_control_chars_opt = false;
          إذا (format_opt != long_format)
            format_opt = one_per_سطر;
          print_with_color = false;
          quoting_style_opt = literal_quoting_style;
          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        افتراضي:
          usage (LS_FAILURE);
        }
    }

  إذا (! output_block_size)
    {
      حرف ثابت *ls_block_size = getenv ("LS_BLOCK_SIZE");
      human_options (ls_block_size,
                     &human_output_opts, &output_block_size);
      إذا (ls_block_size || getenv ("BLOCK_SIZE"))
        {
          file_human_output_opts = human_output_opts;
          file_output_block_size = output_block_size;
        }
      إذا (kibibytes_specified)
        {
          human_output_opts = 0;
          output_block_size = 1024;
        }
    }

  format = (0 <= format_opt ? format_opt
            : ls_mode == LS_LS ? (stdout_isatty ()
                                  ? many_per_سطر : one_per_سطر)
            : ls_mode == LS_MULTI_COL ? many_per_سطر
            : /* ls_mode == LS_LONG_FORMAT */ long_format);

  /* If the line length was not set by a switch but is needed to determine
     output, go to the work of obtaining it from the environment.  */
  ptrdiff_t linelen = width_opt;
  إذا (format == many_per_سطر || format == horizontal || format == with_commas
      || print_with_color)
    {
#ifdef TIOCGWINSZ
      إذا (linelen < 0)
        {
          بنية winsize ws;
          إذا (stdout_isatty ()
              && 0 <= ioctl (STDOUT_FILENO, TIOCGWINSZ, &ws)
              && 0 < ws.ws_col)
            linelen = ws.ws_col <= MIN (PTRDIFF_MAX, SIZE_MAX) ? ws.ws_col : 0;
        }
#endif
      إذا (linelen < 0)
        {
          حرف ثابت *p = getenv ("COLUMNS");
          إذا (p && *p)
            {
              linelen = decode_سطر_length (p);
              إذا (linelen < 0)
                error (0, 0,
                       _("ignoring invalid width"
                         " in environment variable COLUMNS: %s"),
                       quote (p));
            }
        }
    }

  سطر_length = linelen < 0 ? 80 : linelen;

  /* Determine the max possible number of display columns.  */
  max_idx = سطر_length / MIN_COLUMN_WIDTH;
  /* Account for first display column not having a separator,
     or line_lengths shorter than MIN_COLUMN_WIDTH.  */
  max_idx += سطر_length % MIN_COLUMN_WIDTH != 0;

  إذا (format == many_per_سطر || format == horizontal || format == with_commas)
    {
      إذا (0 <= tabsize_opt)
        tabsize = tabsize_opt;
      وإلا
        {
          tabsize = 8;
          حرف ثابت *p = getenv ("TABSIZE");
          إذا (p)
            {
              uintmax_t tmp;
              إذا (xstrtoumax (p, عدم, 0, &tmp, "") == LONGINT_OK
                  && tmp <= SIZE_MAX)
                tabsize = tmp;
              وإلا
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
                  && ((سطر_length
                       && (format == with_commas
                           || format == many_per_سطر || format == horizontal))
                      || format == one_per_سطر || format == long_format);

  صحيح qs = quoting_style_opt;
  إذا (qs < 0)
    qs = getenv_quoting_style ();
  إذا (qs < 0)
    qs = (ls_mode == LS_LS
          ? (stdout_isatty () ? shell_escape_quoting_style : -1)
          : escape_quoting_style);
  إذا (0 <= qs)
    set_quoting_style (عدم, qs);
  qs = get_quoting_style (عدم);
  align_variable_outer_quotes
    = ((format == long_format
        || ((format == many_per_سطر || format == horizontal) && سطر_length))
       && (qs == shell_quoting_style
           || qs == shell_escape_quoting_style
           || qs == c_maybe_quoting_style));
  filename_quoting_options = clone_quoting_options (عدم);
  إذا (qs == escape_quoting_style)
    set_char_quoting (filename_quoting_options, ' ', 1);
  إذا (file_type <= indicator_style)
    {
      لكل (حرف ثابت *p = &"*=>@|"[indicator_style - file_type]; *p; p++)
        set_char_quoting (filename_quoting_options, *p, 1);
    }
  إذا (format == with_commas
      && (qs == shell_quoting_style
          || qs == shell_escape_quoting_style
          || qs == c_maybe_quoting_style
          || qs == escape_quoting_style))
    set_char_quoting (filename_quoting_options, ',', 1);

  dirname_quoting_options = clone_quoting_options (عدم);
  set_char_quoting (dirname_quoting_options, ':', 1);

  /* --dired implies --format=long (-l) and sans --hyperlink.
     So ignore it if those overridden.  */
  dired &= (format == long_format) & !print_hyperlink;

  إذا (eolbyte < dired)
    error (LS_FAILURE, 0, _("--dired and --zero are incompatible"));

  /* If a time type is explicitly specified (with -c, -u, or --time=)
     and we're not showing a time (-l not specified), then sort by that time,
     rather than by name.  Note this behavior is unspecified by POSIX.  */

  sort_type = (0 <= sort_opt ? sort_opt
               : (format != long_format && explicit_time)
               ? sort_time : sort_name);

  إذا (format == long_format)
    {
      حرف *envstyle = عدم;
      حرف *style = time_style_option;
      إذا (! style)
        style = envstyle = getenv ("TIME_STYLE");

      إذا (! style)
        style = (حرف *) "locale";
      وإلا
        {
          ساكن حرف ثابت posix_prefix[] = "posix-";
          بينما (STREQ_LEN (style, posix_prefix, حجم posix_prefix - 1))
            {
              إذا (! hard_locale (LC_TIME))
                إرجاع optind;
              style += حجم posix_prefix - 1;
            }
        }

      إذا (*style == '+')
        {
          حرف *p0 = style + 1;
          حرف *p0nl = strchr (p0, '\n');
          حرف ثابت *p1 = p0;
          إذا (p0nl)
            {
              p1 = p0nl + 1;
              إذا (strchr (p1, '\n'))
                error (LS_FAILURE, 0, _("invalid time style format %s"),
                       quote (p0));
              إذا (envstyle)
                p0 = xmemdup0 (p0, p0nl - p0);
              وإلا
                *p0nl = '\0';
            }
          long_time_format[0] = p0;
          long_time_format[1] = p1;
        }
      وإلا
        {
          بدل (x_timestyle_match (style, /*allow_posix=*/ true,
                                     time_style_args,
                                     (حرف ثابت *) time_style_types,
                                     حجم (*time_style_types), LS_FAILURE))
            {
            حالة full_iso_time_style:
              long_time_format[0] = long_time_format[1] =
                "%Y-%m-%d %H:%M:%S.%N %z";
              اكسر;

            حالة long_iso_time_style:
              long_time_format[0] = long_time_format[1] = "%Y-%m-%d %H:%M";
              اكسر;

            حالة iso_time_style:
              long_time_format[0] = "%Y-%m-%d ";
              long_time_format[1] = "%m-%d %H:%M";
              اكسر;

            حالة locale_time_style:
              إذا (hard_locale (LC_TIME))
                {
                  لكل (صحيح i = 0; i < 2; i++)
                    long_time_format[i] =
                      dcgettext (عدم, long_time_format[i], LC_TIME);
                }
            }
        }

      abformat_init ();
    }

  إرجاع optind;
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

ساكن bool
get_funky_string (حرف **dest, حرف ثابت **src, bool equals_end,
                  size_t *output_تعداد)
{
  حرف رقم;			/* For numerical codes */
  size_t تعداد_2;			/* Something to count with */
  تعداد {
    ST_GND, ST_BACKSLASH, ST_OCTAL, ST_HEX, ST_CARET, ST_END, ST_ERROR
  } state;
  حرف ثابت *p;
  حرف *q;

  p = *src;			/* We don't want to double-indirect */
  q = *dest;			/* the whole darn time.  */

  تعداد_2 = 0;			/* No characters counted in yet.  */
  رقم = 0;

  state = ST_GND;		/* Start in ground state.  */
  بينما (state < ST_END)
    {
      بدل (state)
        {
        حالة ST_GND:		/* Ground state (no escapes) */
          بدل (*p)
            {
            حالة ':':
            حالة '\0':
              state = ST_END;	/* End of string */
              اكسر;
            حالة '\\':
              state = ST_BACKSLASH; /* Backslash escape sequence */
              ++p;
              اكسر;
            حالة '^':
              state = ST_CARET; /* Caret escape */
              ++p;
              اكسر;
            حالة '=':
              إذا (equals_end)
                {
                  state = ST_END; /* End */
                  اكسر;
                }
              FALLTHROUGH;
            افتراضي:
              *(q++) = *(p++);
              ++تعداد_2;
              اكسر;
            }
          اكسر;

        حالة ST_BACKSLASH:	/* Backslash escaped character */
          بدل (*p)
            {
            حالة '0':
            حالة '1':
            حالة '2':
            حالة '3':
            حالة '4':
            حالة '5':
            حالة '6':
            حالة '7':
              state = ST_OCTAL;	/* Octal sequence */
              رقم = *p - '0';
              اكسر;
            حالة 'x':
            حالة 'X':
              state = ST_HEX;	/* Hex sequence */
              رقم = 0;
              اكسر;
            حالة 'a':		/* Bell */
              رقم = '\a';
              اكسر;
            حالة 'b':		/* Backspace */
              رقم = '\b';
              اكسر;
            حالة 'e':		/* Escape */
              رقم = 27;
              اكسر;
            حالة 'f':		/* Form feed */
              رقم = '\f';
              اكسر;
            حالة 'n':		/* Newline */
              رقم = '\n';
              اكسر;
            حالة 'r':		/* Carriage return */
              رقم = '\r';
              اكسر;
            حالة 't':		/* Tab */
              رقم = '\t';
              اكسر;
            حالة 'v':		/* Vtab */
              رقم = '\v';
              اكسر;
            حالة '?':		/* Delete */
              رقم = 127;
              اكسر;
            حالة '_':		/* Space */
              رقم = ' ';
              اكسر;
            حالة '\0':		/* End of string */
              state = ST_ERROR;	/* Error! */
              اكسر;
            افتراضي:		/* Escaped character like \ ^ : = */
              رقم = *p;
              اكسر;
            }
          إذا (state == ST_BACKSLASH)
            {
              *(q++) = رقم;
              ++تعداد_2;
              state = ST_GND;
            }
          ++p;
          اكسر;

        حالة ST_OCTAL:		/* Octal sequence */
          إذا (*p < '0' || *p > '7')
            {
              *(q++) = رقم;
              ++تعداد_2;
              state = ST_GND;
            }
          وإلا
            رقم = (رقم << 3) + (*(p++) - '0');
          اكسر;

        حالة ST_HEX:		/* Hex sequence */
          بدل (*p)
            {
            حالة '0':
            حالة '1':
            حالة '2':
            حالة '3':
            حالة '4':
            حالة '5':
            حالة '6':
            حالة '7':
            حالة '8':
            حالة '9':
              رقم = (رقم << 4) + (*(p++) - '0');
              اكسر;
            حالة 'a':
            حالة 'b':
            حالة 'c':
            حالة 'd':
            حالة 'e':
            حالة 'f':
              رقم = (رقم << 4) + (*(p++) - 'a') + 10;
              اكسر;
            حالة 'A':
            حالة 'B':
            حالة 'C':
            حالة 'D':
            حالة 'E':
            حالة 'F':
              رقم = (رقم << 4) + (*(p++) - 'A') + 10;
              اكسر;
            افتراضي:
              *(q++) = رقم;
              ++تعداد_2;
              state = ST_GND;
              اكسر;
            }
          اكسر;

        حالة ST_CARET:		/* Caret escape */
          state = ST_GND;	/* Should be the next state... */
          إذا (*p >= '@' && *p <= '~')
            {
              *(q++) = *(p++) & 037;
              ++تعداد_2;
            }
          وإلا إذا (*p == '?')
            {
              *(q++) = 127;
              ++تعداد_2;
            }
          وإلا
            state = ST_ERROR;
          اكسر;

        حالة ST_END: حالة ST_ERROR: افتراضي:
          affirm (false);
        }
    }

  *dest = q;
  *src = p;
  *output_تعداد = تعداد_2;

  إرجاع state != ST_ERROR;
}

تعداد parse_state
  {
    PS_START = 1,
    PS_2,
    PS_3,
    PS_4,
    PS_DONE,
    PS_FAIL
  };


/* Check if the content of TERM is a valid name in dircolors.  */

ساكن bool
known_term_type (فراغ)
{
  حرف ثابت *term = getenv ("TERM");
  إذا (! term || ! *term)
    إرجاع false;

  حرف ثابت *سطر = G_سطر;
  بينما (سطر - G_سطر < حجم (G_سطر))
    {
      إذا (STRNCMP_LIT (سطر, "TERM ") == 0)
        {
          إذا (fnmatch (سطر + 5, term, 0) == 0)
            إرجاع true;
        }
      سطر += strlen (سطر) + 1;
    }

  إرجاع false;
}

ساكن فراغ
parse_ls_color (فراغ)
{
  حرف ثابت *p;		/* Pointer to character being parsed */
  حرف *buf;			/* color_buf buffer pointer */
  حرف label0, label1;		/* Indicator label */
  بنية color_ext_type *ext;	/* Extension we are working on */

  إذا ((p = getenv ("LS_COLORS")) == عدم || *p == '\0')
    {
      /* LS_COLORS takes precedence, but if that's not set then
         honor the COLORTERM and TERM env variables so that
         we only go with the internal ANSI color codes if the
         former is non empty or the latter is set to a known value.  */
      حرف ثابت *colorterm = getenv ("COLORTERM");
      إذا (! (colorterm && *colorterm) && ! known_term_type ())
        print_with_color = false;
      إرجاع;
    }

  ext = عدم;

  /* This is an overly conservative estimate, but any possible
     LS_COLORS string will *not* generate a color_buf longer than
     itself, so it is a safe way of allocating a buffer in
     advance.  */
  buf = color_buf = xstrdup (p);

  تعداد parse_state state = PS_START;
  بينما (true)
    {
      بدل (state)
        {
        حالة PS_START:		/* First label character */
          بدل (*p)
            {
            حالة ':':
              ++p;
              اكسر;

            حالة '*':
              /* Allocate new extension block and add to head of
                 linked list (this way a later definition will
                 override an earlier one, which can be useful for
                 having terminal-specific defs override global).  */

              ext = xmalloc (حجم *ext);
              ext->next = color_ext_list;
              color_ext_list = ext;
              ext->exact_match = false;

              ++p;
              ext->ext.string = buf;

              state = (get_funky_string (&buf, &p, true, &ext->ext.len)
                       ? PS_4 : PS_FAIL);
              اكسر;

            حالة '\0':
              state = PS_DONE;	/* Done! */
              اذهب done;

            افتراضي:	/* Assume it is file type label */
              label0 = *p++;
              state = PS_2;
              اكسر;
            }
          اكسر;

        حالة PS_2:		/* Second label character */
          إذا (*p)
            {
              label1 = *p++;
              state = PS_3;
            }
          وإلا
            state = PS_FAIL;	/* Error */
          اكسر;

        حالة PS_3:		/* Equal sign after indicator label */
          state = PS_FAIL;	/* Assume failure...  */
          إذا (*(p++) == '=')/* It *should* be...  */
            {
              لكل (صحيح i = 0; i < countof (indicator_name); i++)
                {
                  إذا ((label0 == indicator_name[i][0])
                      && (label1 == indicator_name[i][1]))
                    {
                      color_indicator[i].string = buf;
                      state = (get_funky_string (&buf, &p, false,
                                                 &color_indicator[i].len)
                               ? PS_START : PS_FAIL);
                      اكسر;
                    }
                }
              إذا (state == PS_FAIL)
                error (0, 0, _("unrecognized prefix: %s"),
                       quote ((حرف []) {label0, label1, '\0'}));
            }
          اكسر;

        حالة PS_4:		/* Equal sign after *.ext */
          إذا (*(p++) == '=')
            {
              ext->seq.string = buf;
              state = (get_funky_string (&buf, &p, false, &ext->seq.len)
                       ? PS_START : PS_FAIL);
            }
          وإلا
            state = PS_FAIL;
          اكسر;

        حالة PS_FAIL:
          اذهب done;

        حالة PS_DONE: افتراضي:
          affirm (false);
        }
    }
 done:

  إذا (state == PS_FAIL)
    {
      error (0, 0,
             _("unparsable value for LS_COLORS environment variable"));
      حرر (color_buf);
      لكل (بنية color_ext_type *e = color_ext_list; e != عدم;
           /* empty */)
        {
          بنية color_ext_type *e2 = e;
          e = e->next;
          حرر (e2);
        }
      print_with_color = false;
      إرجاع;
    }
  وإلا
    {
      /* Postprocess list to set EXACT_MATCH on entries where there are
         different cased extensions with separate sequences defined.
         Also set ext.len to SIZE_MAX on any entries that can't
         match due to precedence, to avoid redundant string compares.  */
      لكل (بنية color_ext_type *e1 = color_ext_list; e1 != عدم;
           e1 = e1->next)
        {
          bool case_ignored = false;

          لكل (بنية color_ext_type *e2 = e1->next; e2 != عدم;
               e2 = e2->next)
            {
              إذا (e2->ext.len < SIZE_MAX && e1->ext.len == e2->ext.len)
                {
                  إذا (memeq (e1->ext.string, e2->ext.string, e1->ext.len))
                    e2->ext.len = SIZE_MAX; /* Ignore */
                  وإلا إذا (c_strncasecmp (e1->ext.string, e2->ext.string,
                                          e1->ext.len) == 0)
                    {
                      إذا (case_ignored)
                        {
                          e2->ext.len = SIZE_MAX; /* Ignore */
                        }
                      وإلا إذا (e1->seq.len == e2->seq.len
                               && memeq (e1->seq.string, e2->seq.string,
                                         e1->seq.len))
                        {
                          e2->ext.len = SIZE_MAX; /* Ignore */
                          case_ignored = true;    /* Ignore all subsequent */
                        }
                      وإلا
                        {
                          e1->exact_match = true;
                          e2->exact_match = true;
                        }
                    }
                }
            }
        }
    }

  إذا (color_indicator[C_LINK].len == 6
      && !STRNCMP_LIT (color_indicator[C_LINK].string, "target"))
    color_symlink_as_referent = true;
}

/* Set the exit status to report a failure.  If SERIOUS, it is a
   serious failure; otherwise, it is merely a minor problem.  */

ساكن فراغ
set_exit_status (bool serious)
{
  إذا (serious)
    exit_status = LS_FAILURE;
  وإلا إذا (exit_status == EXIT_SUCCESS)
    exit_status = LS_MINOR_PROBLEM;
}

/* Assuming a failure is serious if SERIOUS, use the printf-style
   MESSAGE to report the failure to access a file named FILE.  Assume
   errno is set appropriately for the failure.  */

ساكن فراغ
file_failure (bool serious, حرف ثابت *message, حرف ثابت *file)
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

ساكن فراغ
queue_directory (حرف ثابت *name, حرف ثابت *realname, bool command_سطر_arg)
{
  بنية pending *new = xmalloc (حجم *new);
  new->realname = realname ? xstrdup (realname) : عدم;
  new->name = name ? xstrdup (name) : عدم;
  new->command_سطر_arg = command_سطر_arg;
  new->next = pending_dirs;
  pending_dirs = new;
}

/* Read directory NAME, and list the files in it.
   If REALNAME is nonzero, print its name instead of NAME;
   this is used for symbolic links to directories.
   COMMAND_LINE_ARG means this directory was mentioned on the command line.  */

ساكن فراغ
print_dir (حرف ثابت *name, حرف ثابت *realname, bool command_سطر_arg)
{
  DIR *dirp;
  بنية dirent *next;
  uintmax_t total_blocks = 0;
  ساكن bool first = true;

  errno = 0;
  dirp = opendir (name);
  إذا (!dirp)
    {
      إذا (command_سطر_arg || ! ignorable_traversal_errno (errno))
        file_failure (command_سطر_arg, _("cannot open directory %s"), name);
      إرجاع;
    }

  إذا (LOOP_DETECT)
    {
      بنية stat dir_stat;
      صحيح fd = dirfd (dirp);

      /* If dirfd failed, endure the overhead of stat'ing by path  */
      إذا ((0 <= fd
           ? fstat_for_ino (fd, &dir_stat)
           : stat_for_ino (name, &dir_stat)) < 0)
        {
          إذا (! ignorable_traversal_errno (errno))
            file_failure (command_سطر_arg,
                          _("cannot determine device and inode of %s"), name);
          closedir (dirp);
          إرجاع;
        }

      /* If we've already visited this dev/inode pair, warn that
         we've found a loop, and do not process this directory.  */
      إذا (visit_dir (dir_stat.st_dev, dir_stat.st_ino))
        {
          error (0, 0, _("%s: not listing already-listed directory"),
                 quotef (name));
          closedir (dirp);
          set_exit_status (true);
          إرجاع;
        }

      dev_ino_ادفع (dir_stat.st_dev, dir_stat.st_ino);
    }

  clear_files ();

  إذا (recursive || print_dir_name)
    {
      إذا (!first)
        dired_outbyte ('\n');
      first = false;
      dired_indent ();

      حرف *absolute_name = عدم;
      إذا (print_hyperlink)
        {
          absolute_name = canonicalize_filename_mode (name, CAN_MISSING);
          إذا (! absolute_name
              && ! ignorable_traversal_errno (errno))
            file_failure (command_سطر_arg,
                          _("error canonicalizing %s"), name);
        }
      quote_name (realname ? realname : name, dirname_quoting_options, -1,
                  عدم, true, &subdired_obstack, absolute_name);

      حرر (absolute_name);

      dired_outstring (":\n");
    }

  /* Read the directory entries, and insert the subfiles into the 'cwd_file'
     table.  */

  بينما (true)
    {
      /* Set errno to zero so we can distinguish between a readdir failure
         and when readdir simply finds that there are no more entries.  */
      errno = 0;
      next = readdir (dirp);
      إذا (next)
        {
          إذا (! file_ignored (next->d_name))
            {
              تعداد filetype type;
#إذا HAVE_STRUCT_DIRENT_D_TYPE
              type = d_type_filetype[next->d_type];
#وإلا
              type = unknown;
#endif
              total_blocks += gobble_file (next->d_name, type,
                                           RELIABLE_D_INO (next),
                                           false, name);

              /* In this narrow case, print out each name right away, so
                 ls uses constant memory while processing the entries of
                 this directory.  Useful when there are many (millions)
                 of entries in a directory.  */
              إذا (format == one_per_سطر && sort_type == sort_none
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
      وإلا
        {
          صحيح err = errno;
          إذا (err == 0)
            اكسر;
          /* Ignore errors indicating that the directory was removed.  */
          إذا (ignorable_traversal_errno (err))
            اكسر;
          file_failure (command_سطر_arg, _("reading directory %s"), name);
          إذا (err != EOVERFLOW)
            اكسر;
        }

      /* When processing a very large directory, and since we've inhibited
         interrupts, this loop would take so long that ls would be annoyingly
         uninterruptible.  This ensures that it handles signals promptly.  */
      process_signals ();
    }

  إذا (closedir (dirp) != 0)
    {
      file_failure (command_سطر_arg, _("closing directory %s"), name);
      /* Don't return; print whatever we got.  */
    }

  /* Sort the directory contents.  */
  sort_files ();

  /* If any member files are subdirectories, perhaps they should have their
     contents listed rather than being mentioned here as files.  */

  إذا (recursive)
    extract_dirs_from_files (name, false);

  إذا (format == long_format || print_block_size)
    {
      حرف buf[LONGEST_HUMAN_READABLE + 3];
      حرف *p = human_readable (total_blocks, buf + 1, human_output_opts,
                                ST_NBLOCKSIZE, output_block_size);
      حرف *pend = p + strlen (p);
      *--p = ' ';
      *pend++ = eolbyte;
      dired_indent ();
      dired_outstring (_("total"));
      dired_outbuf (p, pend - p);
    }

  إذا (cwd_n_used)
    print_current_files ();
}

/* Add 'pattern' to the list of patterns for which files that match are
   not listed.  */

ساكن فراغ
add_ignore_pattern (حرف ثابت *pattern)
{
  بنية ignore_pattern *ignore;

  ignore = xmalloc (حجم *ignore);
  ignore->pattern = pattern;
  /* Add it to the head of the linked list.  */
  ignore->next = ignore_patterns;
  ignore_patterns = ignore;
}

/* Return true if one of the PATTERNS matches FILE.  */

ساكن bool
patterns_match (بنية ignore_pattern ثابت *patterns, حرف ثابت *file)
{
  لكل (بنية ignore_pattern ثابت *p = patterns; p; p = p->next)
    إذا (fnmatch (p->pattern, file, FNM_PERIOD) == 0)
      إرجاع true;
  إرجاع false;
}

/* Return true if FILE should be ignored.  */

ساكن bool
file_ignored (حرف ثابت *name)
{
  إرجاع ((ignore_mode != IGNORE_MINIMAL
           && name[0] == '.'
           && (ignore_mode == IGNORE_DEFAULT || ! name[1 + (name[1] == '.')]))
          || (ignore_mode == IGNORE_DEFAULT
              && patterns_match (hide_patterns, name))
          || patterns_match (ignore_patterns, name));
}

/* POSIX requires that a file size be printed without a sign, even
   when negative.  Assume the typical case where negative sizes are
   actually positive values that have wrapped around.  */

ساكن uintmax_t
unsigned_file_size (off_t size)
{
  إرجاع size + (size < 0) * ((uintmax_t) OFF_T_MAX - OFF_T_MIN + 1);
}

#ifdef HAVE_CAP
/* Return true if NAME has a capability (see linux/capability.h) */
ساكن bool
has_capability (حرف ثابت *name)
{
  حرف *result;
  bool has_cap;

  cap_t cap_d = cap_get_file (name);
  إذا (cap_d == عدم)
    إرجاع false;

  result = cap_to_text (cap_d, عدم);
  cap_free (cap_d);
  إذا (!result)
    إرجاع false;

  /* check if human-readable capability string is empty */
  has_cap = !!*result;

  cap_free (result);
  إرجاع has_cap;
}
#وإلا
ساكن bool
has_capability (MAYBE_UNUSED حرف ثابت *name)
{
  errno = ENOTSUP;
  إرجاع false;
}
#endif

/* Enter and remove entries in the table 'cwd_file'.  */

ساكن فراغ
free_ent (بنية fileinfo *f)
{
  حرر (f->name);
  حرر (f->linkname);
  حرر (f->absolute_name);
  إذا (f->scontext != UNKNOWN_SECURITY_CONTEXT)
    aclinfo_scontext_free (f->scontext);
}

/* Empty the table of files.  */
ساكن فراغ
clear_files (فراغ)
{
  لكل (idx_t i = 0; i < cwd_n_used; i++)
    {
      بنية fileinfo *f = sorted_file[i];
      free_ent (f);
    }

  cwd_n_used = 0;
  cwd_some_quoted = false;
  any_has_acl = false;
  inode_العدد_width = 0;
  block_size_width = 0;
  nlink_width = 0;
  owner_width = 0;
  group_width = 0;
  author_width = 0;
  scontext_width = 0;
  major_device_العدد_width = 0;
  minor_device_العدد_width = 0;
  file_size_width = 0;
}

/* Cache file_has_aclinfo failure, when it's trivial to do.
   Like file_has_aclinfo, but when F's st_dev says it's on a file
   system lacking ACL support, return 0 with ENOTSUP immediately.  */
ساكن صحيح
file_has_aclinfo_cache (حرف ثابت *file, بنية fileinfo *f,
                        بنية aclinfo *ai, صحيح flags)
{
  /* If UNSUPPORTED_SCONTEXT, these variables hold the
     st_dev and associated info for the most recently processed device
     for which file_has_aclinfo failed indicating lack of support.  */
  ساكن صحيح unsupported_return;
  ساكن حرف *unsupported_scontext;
  ساكن صحيح unsupported_scontext_err;
  ساكن dev_t unsupported_device;

  إذا (f->stat_ok && unsupported_scontext
      && f->stat.st_dev == unsupported_device)
    {
      ai->buf = ai->u._gl_acl_ch;
      ai->size = 0;
      ai->scontext = unsupported_scontext;
      ai->scontext_err = unsupported_scontext_err;
      errno = ENOTSUP;
      إرجاع unsupported_return;
    }

  errno = 0;
  صحيح n = file_has_aclinfo (file, ai, flags);
  صحيح err = errno;
  إذا (!USE_ACL)
    n = 0;
  /* A security-context error does not imply that ACLs are unsupported.
     Require an unsupported attribute listing too if ACLs or capability
     colors are needed.  */
  إذا (f->stat_ok && n <= 0 && !acl_errno_valid (err)
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
  إرجاع n;
}

/* Cache has_capability failure, when it's trivial to do.
   Like has_capability, but when F's st_dev says it's on a file
   system lacking capability support, return 0 with ENOTSUP immediately.  */
ساكن bool
has_capability_cache (حرف ثابت *file, بنية fileinfo *f)
{
  /* If UNSUPPORTED_CACHED, UNSUPPORTED_DEVICE is the cached
     st_dev of the most recently processed device for which we've
     found that has_capability fails indicating lack of support.  */
  ساكن bool unsupported_cached;
  ساكن dev_t unsupported_device;

  إذا (f->stat_ok && unsupported_cached && f->stat.st_dev == unsupported_device)
    {
      errno = ENOTSUP;
      إرجاع 0;
    }

  bool b = has_capability (file);
  إذا (f->stat_ok && !b && !acl_errno_valid (errno))
    {
      unsupported_cached = true;
      unsupported_device = f->stat.st_dev;
    }
  إرجاع b;
}

/* Get the ACL, security context, and capability information needed for F.
   FULL_NAME names the file; DO_DEREF says whether to follow symlinks.  */
ساكن فراغ
get_extended_info (حرف ثابت *full_name, بنية fileinfo *f, bool do_deref)
{
  /* FIXME: file_has_aclinfo() conflates capability and acl xattr lookup
     with security context attributes.  Ideally we'd have separate
     ACL_GET_{CAPABILITY,ACL} flags to lookup just the required items,
     simplifying the logic here.  */

  تعداد filetype type = f->filetype;
  bool check_acl = USE_ACL && format == long_format;
  bool get_scontext = false;
#إذا HAVE_SMACK || USE_SELINUX_SELINUX_H
  get_scontext = (format == long_format) | print_scontext;
#endif
  bool check_capability = format_needs_capability & (type == normal);

  إذا (type != arg_directory && (check_acl | get_scontext | check_capability))
    {
      بنية aclinfo ai;
      /* Without ACL support, request contexts to obtain the attribute
         names needed for capability coloring too.  */
      صحيح aclinfo_flags = ((do_deref ? ACL_SYMLINK_FOLLOW : 0)
                           | (get_scontext || (!USE_ACL && check_capability)
                              ? ACL_GET_SCONTEXT : 0)
                           | filetype_d_type[type]);
      صحيح n = file_has_aclinfo_cache (full_name, f, &ai, aclinfo_flags);
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

      إذا (format == long_format && n < 0 && !cannot_access_acl)
        error (0, errno, "%s", quotef (full_name));
      وإلا
        {
          /* When requesting security context information, don't make
             ls fail just because the file (even a command line argument)
             isn't on the right type of file system.  I.e., a getfilecon
             failure isn't in the same class as a stat failure.  */
          إذا (print_scontext && ai.scontext_err
              && ! ignorable_traversal_errno (ai.scontext_err)
              && (! (is_ENOTSUP (ai.scontext_err)
                     || ai.scontext_err == ENODATA)))
            error (0, ai.scontext_err, "%s", quotef (full_name));
        }

      /* has_capability adds around 30% runtime to 'ls --color',
         so call it only if really needed.  Note capability coloring
         is disabled in the default color config.  */
      إذا (check_capability && aclinfo_has_xattr (&ai, XATTR_NAME_CAPS))
        f->has_capability = has_capability_cache (full_name, f);

      f->scontext = ai.scontext;
      ai.scontext = عدم;
      aclinfo_free (&ai);
    }
}

ساكن bool
needs_quoting (حرف ثابت *name)
{
  حرف test[2];
  size_t len = quotearg_buffer (test, حجم test , name, -1,
                                filename_quoting_options);
  إرجاع *name != *test || strlen (name) != len;
}

/* Add a file to the current table of files.
   Verify that the file exists, and print an error message if it does not.
   Return the number of blocks that the file occupies.  */
ساكن uintmax_t
gobble_file (حرف ثابت *name, تعداد filetype type, ino_t inode,
             bool command_سطر_arg, حرف ثابت *dirname)
{
  uintmax_t blocks = 0;
  بنية fileinfo *f;

  /* An inode value prior to gobble_file necessarily came from readdir,
     which is not used for command line arguments.  */
  affirm (! command_سطر_arg || inode == NOT_AN_INODE_العدد);

  إذا (cwd_n_used == cwd_n_alloc)
    cwd_file = xpalloc (cwd_file, &cwd_n_alloc, 1, -1, حجم *cwd_file);

  f = &cwd_file[cwd_n_used];
  memset (f, '\0', حجم *f);
  f->stat.st_ino = inode;
  f->filetype = type;
  f->scontext = UNKNOWN_SECURITY_CONTEXT;

  f->quoted = -1;
  إذا ((! cwd_some_quoted) && align_variable_outer_quotes)
    {
      /* Determine if any quoted for padding purposes.  */
      f->quoted = needs_quoting (name);
      إذا (f->quoted)
        cwd_some_quoted = 1;
    }

  bool check_stat =
     (command_سطر_arg
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
      || (print_inode && inode == NOT_AN_INODE_العدد)
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
  حرف ثابت *full_name = name;
  إذا (check_stat | print_scontext | format_needs_capability
      && name[0] != '/' && dirname)
    {
      حرف *p = alloca (strlen (name) + strlen (dirname) + 2);
      attach (p, dirname, name);
      full_name = p;
    }

  bool do_deref;

  إذا (!check_stat)
    do_deref = dereference == DEREF_ALWAYS;
  وإلا
    {
      صحيح err;

      إذا (print_hyperlink)
        {
          f->absolute_name = canonicalize_filename_mode (full_name,
                                                         CAN_MISSING);
          إذا (! f->absolute_name
              && (command_سطر_arg
                  || ! ignorable_traversal_errno (errno)))
            file_failure (command_سطر_arg,
                          _("error canonicalizing %s"), full_name);
        }

      بدل (dereference)
        {
        حالة DEREF_ALWAYS:
          err = do_stat (full_name, &f->stat);
          do_deref = true;
          اكسر;

        حالة DEREF_COMMAND_سطر_ARGUMENTS:
        حالة DEREF_COMMAND_سطر_SYMLINK_TO_DIR:
          إذا (command_سطر_arg)
            {
              bool need_lstat;
              err = do_stat (full_name, &f->stat);
              do_deref = true;

              إذا (dereference == DEREF_COMMAND_سطر_ARGUMENTS)
                اكسر;

              need_lstat = (err < 0
                            ? (errno == ENOENT || errno == ELOOP)
                            : ! S_ISDIR (f->stat.st_mode));
              إذا (!need_lstat)
                اكسر;

              /* stat failed because of ENOENT || ELOOP, maybe indicating a
                 non-traversable symlink.  Or stat succeeded,
                 FULL_NAME does not refer to a directory,
                 and --dereference-command-line-symlink-to-dir is in effect.
                 Fall through so that we call lstat instead.  */
            }
          FALLTHROUGH;

        حالة DEREF_NEVER:
          err = do_lstat (full_name, &f->stat);
          do_deref = false;
          اكسر;

        حالة DEREF_UNDEFINED: افتراضي:
          affirm (false);
        }

      إذا (err != 0)
        {
          إذا (! command_سطر_arg && ignorable_traversal_errno (errno))
            {
              صحيح stat_errno = errno;
              بنية stat linkstat;
              bool dangling_link = (do_deref
                                    && (type == symbolic_link
                                        || type == unknown)
                                    && do_lstat (full_name, &linkstat) == 0);
              errno = stat_errno;
              إذا (! dangling_link)
                {
                  free_ent (f);
                  إرجاع 0;
                }
            }

          /* Failure to stat a command line argument leads to
             an exit status of 2.  For other files, stat failure
             provokes an exit status of 1.  */
          file_failure (command_سطر_arg,
                        _("cannot access %s"), full_name);

          إذا (command_سطر_arg)
            {
              free_ent (f);
              إرجاع 0;
            }

          f->name = xstrdup (name);
          cwd_n_used++;

          إرجاع 0;
        }

      f->stat_ok = true;
      f->filetype = type = d_type_filetype[IFTODT (f->stat.st_mode)];
    }

  إذا (type == directory && command_سطر_arg && !immediate_dirs)
    f->filetype = type = arg_directory;

  get_extended_info (full_name, f, do_deref);

  إذا ((type == symbolic_link)
      & ((format == long_format) | check_symlink_mode))
    {
      بنية stat linkstats;

      إذا (! get_link_name (full_name, f, command_سطر_arg))
        {
          free_ent (f);
          إرجاع 0;
        }

      /* Use the slower quoting path for this entry, though
         don't update CWD_SOME_QUOTED since alignment not affected.  */
      إذا (f->linkname && f->quoted == 0 && needs_quoting (f->linkname))
        f->quoted = -1;

      /* Avoid following symbolic links when possible, i.e., when
         they won't be traced and when no indicator is needed.  */
      إذا (f->linkname
          && (file_type <= indicator_style || check_symlink_mode)
          && stat_for_mode (full_name, &linkstats) == 0)
        {
          f->linkok = true;
          f->linkmode = linkstats.st_mode;
        }
    }

  blocks = STP_NBLOCKS (&f->stat);
  إذا (format == long_format || print_block_size)
    {
      حرف buf[LONGEST_HUMAN_READABLE + 1];
      صحيح len = mbswidth (human_readable (blocks, buf, human_output_opts,
                                          ST_NBLOCKSIZE, output_block_size),
                          MBSWIDTH_FLAGS);
      إذا (block_size_width < len)
        block_size_width = len;
    }

  إذا (format == long_format)
    {
      إذا (print_owner)
        {
          صحيح len = format_user_width (f->stat.st_uid);
          إذا (owner_width < len)
            owner_width = len;
        }

      إذا (print_group)
        {
          صحيح len = format_group_width (f->stat.st_gid);
          إذا (group_width < len)
            group_width = len;
        }

      إذا (print_author)
        {
          صحيح len = format_user_width (f->stat.st_author);
          إذا (author_width < len)
            author_width = len;
        }
    }

  إذا (print_scontext)
    {
      صحيح len = strlen (f->scontext);
      إذا (scontext_width < len)
        scontext_width = len;
    }

  إذا (format == long_format)
    {
      حرف b[INT_BUFSIZE_BOUND (uintmax_t)];
      صحيح b_len = strlen (umaxtostr (f->stat.st_nlink, b));
      إذا (nlink_width < b_len)
        nlink_width = b_len;

      إذا ((type == chardev) | (type == blockdev))
        {
          حرف buf[INT_BUFSIZE_BOUND (uintmax_t)];
          صحيح len = strlen (umaxtostr (major (f->stat.st_rdev), buf));
          إذا (major_device_العدد_width < len)
            major_device_العدد_width = len;
          len = strlen (umaxtostr (minor (f->stat.st_rdev), buf));
          إذا (minor_device_العدد_width < len)
            minor_device_العدد_width = len;
          len = major_device_العدد_width + 2 + minor_device_العدد_width;
          إذا (file_size_width < len)
            file_size_width = len;
        }
      وإلا
        {
          حرف buf[LONGEST_HUMAN_READABLE + 1];
          uintmax_t size = unsigned_file_size (f->stat.st_size);
          صحيح len = mbswidth (human_readable (size, buf,
                                              file_human_output_opts,
                                              1, file_output_block_size),
                              MBSWIDTH_FLAGS);
          إذا (file_size_width < len)
            file_size_width = len;
        }
    }

  إذا (print_inode)
    {
      حرف buf[INT_BUFSIZE_BOUND (uintmax_t)];
      صحيح len = strlen (umaxtostr (f->stat.st_ino, buf));
      إذا (inode_العدد_width < len)
        inode_العدد_width = len;
    }

  f->name = xstrdup (name);
  cwd_n_used++;

  إرجاع blocks;
}

/* Return true if F refers to a directory.  */
ساكن bool
is_directory (ثابت بنية fileinfo *f)
{
  إرجاع f->filetype == directory || f->filetype == arg_directory;
}

/* Return true if F refers to a (symlinked) directory.  */
ساكن bool
is_linked_directory (ثابت بنية fileinfo *f)
{
  إرجاع f->filetype == directory || f->filetype == arg_directory
         || S_ISDIR (f->linkmode);
}

/* Put the name of the file that FILENAME is a symbolic link to
   into the LINKNAME field of 'f'.  COMMAND_LINE_ARG indicates whether
   FILENAME is a command-line argument.  */

ساكن bool
get_link_name (حرف ثابت *filename, بنية fileinfo *f, bool command_سطر_arg)
{
  f->linkname = areadlink_with_size (filename, f->stat.st_size);
  إذا (f->linkname == عدم)
    {
      إذا (! command_سطر_arg
          && ignorable_traversal_errno (errno))
        إرجاع false;
      file_failure (command_سطر_arg, _("cannot read symbolic link %s"),
                    filename);
    }
  إرجاع true;
}

/* Return true if the last component of NAME is '.' or '..'
   This is so we don't try to recurse on '././././. ...' */

ساكن bool
basename_is_dot_or_dotdot (حرف ثابت *name)
{
  حرف ثابت *base = last_component (name);
  إرجاع dot_or_dotdot (base);
}

/* Remove any entries from CWD_FILE that are for directories,
   and queue them to be listed as directories instead.
   DIRNAME is the prefix to prepend to each dirname
   to make it correct relative to ls's working dir;
   if it is null, no prefix is needed and "." and ".." should not be ignored.
   If COMMAND_LINE_ARG is true, this directory was mentioned at the top level,
   This is desirable when processing directories recursively.  */

ساكن فراغ
extract_dirs_from_files (حرف ثابت *dirname, bool command_سطر_arg)
{
  idx_t i, j;
  bool ignore_dot_and_dot_dot = (dirname != عدم);

  إذا (dirname && LOOP_DETECT)
    {
      /* Insert a marker entry first.  When we dequeue this marker entry,
         we'll know that DIRNAME has been processed and may be removed
         from the set of active directories.  */
      queue_directory (عدم, dirname, false);
    }

  /* Queue the directories last one first, because queueing reverses the
     order.  */
  لكل (i = cwd_n_used; 0 < i; )
    {
      i--;
      بنية fileinfo *f = sorted_file[i];

      إذا (is_directory (f)
          && (! ignore_dot_and_dot_dot
              || ! basename_is_dot_or_dotdot (f->name)))
        {
          إذا (!dirname || f->name[0] == '/')
            queue_directory (f->name, f->linkname, command_سطر_arg);
          وإلا
            {
              حرف *name = file_name_concat (dirname, f->name, عدم);
              queue_directory (name, f->linkname, command_سطر_arg);
              حرر (name);
            }
          إذا (f->filetype == arg_directory)
            free_ent (f);
        }
    }

  /* Now delete the directories from the table, compacting all the remaining
     entries.  */

  لكل (i = 0, j = 0; i < cwd_n_used; i++)
    {
      بنية fileinfo *f = sorted_file[i];
      sorted_file[j] = f;
      j += (f->filetype != arg_directory);
    }
  cwd_n_used = j;
}

/* Use strcoll to compare strings in this locale.  If an error occurs,
   report an error and longjmp to failed_strcoll.  */

ساكن jmp_buf failed_strcoll;

ساكن صحيح
xstrcoll (حرف ثابت *a, حرف ثابت *b)
{
  صحيح diff;
  errno = 0;
  diff = strcoll (a, b);
  إذا (errno)
    {
      error (0, errno, _("cannot compare file names %s and %s"),
             quote_n (0, a), quote_n (1, b));
      set_exit_status (false);
      longjmp (failed_strcoll, 1);
    }
  إرجاع diff;
}

/* Comparison routines for sorting the files.  */

تعريفنوع فراغ ثابت *V;
تعريفنوع صحيح (*qsortFunc)(V a, V b);

/* Used below in DEFINE_SORT_FUNCTIONS for _df_ sort function variants.  */
ساكن صحيح
dirfirst_check (بنية fileinfo ثابت *a, بنية fileinfo ثابت *b,
                صحيح (*cmp) (V, V))
{
  صحيح diff = is_linked_directory (b) - is_linked_directory (a);
  إرجاع diff ? diff : cmp (a, b);
}

/* Define the 8 different sort function variants required for each sortkey.
   KEY_NAME is a token describing the sort key, e.g., ctime, atime, size.
   KEY_CMP_FUNC is a function to compare records based on that key, e.g.,
   ctime_cmp, atime_cmp, size_cmp.  Append KEY_NAME to the string,
   '[rev_][x]str{cmp|coll}[_df]_', to create each function name.  */
#define DEFINE_SORT_FUNCTIONS(مفتاح_name, مفتاح_cmp_func)			\
  /* direct, non-dirfirst versions */					\
  ساكن صحيح xstrcoll_##key_name (V a, V b)				\
  { إرجاع مفتاح_cmp_func (a, b, xstrcoll); }				\
  ATTRIBUTE_PURE ساكن صحيح strcmp_##key_name (V a, V b)		\
  { إرجاع مفتاح_cmp_func (a, b, strcmp); }				\
                                                                        \
  /* reverse, non-dirfirst versions */					\
  ساكن صحيح rev_xstrcoll_##key_name (V a, V b)				\
  { إرجاع مفتاح_cmp_func (b, a, xstrcoll); }				\
  ATTRIBUTE_PURE ساكن صحيح rev_strcmp_##key_name (V a, V b)	\
  { إرجاع مفتاح_cmp_func (b, a, strcmp); }				\
                                                                        \
  /* direct, dirfirst versions */					\
  ساكن صحيح xstrcoll_df_##key_name (V a, V b)				\
  { إرجاع dirfirst_check (a, b, xstrcoll_##key_name); }		\
  ATTRIBUTE_PURE ساكن صحيح strcmp_df_##key_name (V a, V b)		\
  { إرجاع dirfirst_check (a, b, strcmp_##key_name); }			\
                                                                        \
  /* reverse, dirfirst versions */					\
  ساكن صحيح rev_xstrcoll_df_##key_name (V a, V b)			\
  { إرجاع dirfirst_check (a, b, rev_xstrcoll_##key_name); }		\
  ATTRIBUTE_PURE ساكن صحيح rev_strcmp_df_##key_name (V a, V b)	\
  { إرجاع dirfirst_check (a, b, rev_strcmp_##key_name); }

ساكن صحيح
cmp_ctime (بنية fileinfo ثابت *a, بنية fileinfo ثابت *b,
           صحيح (*cmp) (حرف ثابت *, حرف ثابت *))
{
  صحيح diff = timespec_cmp (get_stat_ctime (&b->stat),
                           get_stat_ctime (&a->stat));
  إرجاع diff ? diff : cmp (a->name, b->name);
}

ساكن صحيح
cmp_mtime (بنية fileinfo ثابت *a, بنية fileinfo ثابت *b,
           صحيح (*cmp) (حرف ثابت *, حرف ثابت *))
{
  صحيح diff = timespec_cmp (get_stat_mtime (&b->stat),
                           get_stat_mtime (&a->stat));
  إرجاع diff ? diff : cmp (a->name, b->name);
}

ساكن صحيح
cmp_atime (بنية fileinfo ثابت *a, بنية fileinfo ثابت *b,
           صحيح (*cmp) (حرف ثابت *, حرف ثابت *))
{
  صحيح diff = timespec_cmp (get_stat_atime (&b->stat),
                           get_stat_atime (&a->stat));
  إرجاع diff ? diff : cmp (a->name, b->name);
}

ساكن صحيح
cmp_btime (بنية fileinfo ثابت *a, بنية fileinfo ثابت *b,
           صحيح (*cmp) (حرف ثابت *, حرف ثابت *))
{
  صحيح diff = timespec_cmp (get_stat_btime (&b->stat),
                           get_stat_btime (&a->stat));
  إرجاع diff ? diff : cmp (a->name, b->name);
}

ساكن صحيح
off_cmp (off_t a, off_t b)
{
  إرجاع _GL_CMP (a, b);
}

ساكن صحيح
cmp_size (بنية fileinfo ثابت *a, بنية fileinfo ثابت *b,
          صحيح (*cmp) (حرف ثابت *, حرف ثابت *))
{
  صحيح diff = off_cmp (b->stat.st_size, a->stat.st_size);
  إرجاع diff ? diff : cmp (a->name, b->name);
}

ساكن صحيح
cmp_name (بنية fileinfo ثابت *a, بنية fileinfo ثابت *b,
          صحيح (*cmp) (حرف ثابت *, حرف ثابت *))
{
  إرجاع cmp (a->name, b->name);
}

/* Compare file extensions.  Files with no extension are 'smallest'.
   If extensions are the same, compare by file names instead.  */

ساكن صحيح
cmp_extension (بنية fileinfo ثابت *a, بنية fileinfo ثابت *b,
               صحيح (*cmp) (حرف ثابت *, حرف ثابت *))
{
  حرف ثابت *base1 = strrchr (a->name, '.');
  حرف ثابت *base2 = strrchr (b->name, '.');
  صحيح diff = cmp (base1 ? base1 : "", base2 ? base2 : "");
  إرجاع diff ? diff : cmp (a->name, b->name);
}

/* Return the (cached) screen width,
   for the NAME associated with the passed fileinfo F.  */

ساكن size_t
fileinfo_name_width (بنية fileinfo ثابت *f)
{
  إرجاع f->width
         ? f->width
         : quote_name_width (f->name, filename_quoting_options, f->quoted);
}

ساكن صحيح
cmp_width (بنية fileinfo ثابت *a, بنية fileinfo ثابت *b,
          صحيح (*cmp) (حرف ثابت *, حرف ثابت *))
{
  صحيح diff = fileinfo_name_width (a) - fileinfo_name_width (b);
  إرجاع diff ? diff : cmp (a->name, b->name);
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
ساكن صحيح
cmp_version (بنية fileinfo ثابت *a, بنية fileinfo ثابت *b)
{
  صحيح diff = filevercmp (a->name, b->name);
  إرجاع diff ? diff : strcmp (a->name, b->name);
}

ساكن صحيح
xstrcoll_version (V a, V b)
{
  إرجاع cmp_version (a, b);
}
ساكن صحيح
rev_xstrcoll_version (V a, V b)
{
  إرجاع cmp_version (b, a);
}
ساكن صحيح
xstrcoll_df_version (V a, V b)
{
  إرجاع dirfirst_check (a, b, xstrcoll_version);
}
ساكن صحيح
rev_xstrcoll_df_version (V a, V b)
{
  إرجاع dirfirst_check (a, b, rev_xstrcoll_version);
}


/* We have 2^3 different variants for each sort-key function
   (for 3 independent sort modes).
   The function pointers stored in this array must be dereferenced as:

    sort_variants[sort_key][use_strcmp][reverse][dirs_first]

   Note that the order in which sort keys are listed in the function pointer
   array below is defined by the order of the elements in the time_type and
   sort_type enums!  */

#define LIST_SORTFUNCTION_VARIANTS(مفتاح_name)                        \
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

ساكن qsortFunc ثابت sort_functions[][2][2][2] =
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
        { عدم, عدم },
        { عدم, عدم },
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

ساكن فراغ
initialize_ordering_vector (فراغ)
{
  لكل (idx_t i = 0; i < cwd_n_used; i++)
    sorted_file[i] = &cwd_file[i];
}

/* Cache values based on attributes global to all files.  */

ساكن فراغ
update_current_files_info (فراغ)
{
  /* Cache screen width of name, if needed multiple times.  */
  إذا (sort_type == sort_width
      || (سطر_length && (format == many_per_سطر || format == horizontal)))
    {
      لكل (idx_t i = 0; i < cwd_n_used; i++)
        {
          بنية fileinfo *f = sorted_file[i];
          f->width = fileinfo_name_width (f);
        }
    }
}

/* Sort the files now in the table.  */

ساكن فراغ
sort_files (فراغ)
{
  bool use_strcmp;

  إذا (sorted_file_alloc < cwd_n_used + (cwd_n_used >> 1))
    {
      حرر (sorted_file);
      sorted_file = xinmalloc (cwd_n_used, 3 * حجم *sorted_file);
      sorted_file_alloc = 3 * cwd_n_used;
    }

  initialize_ordering_vector ();

  update_current_files_info ();

  إذا (sort_type == sort_none)
    إرجاع;

  /* Try strcoll.  If it fails, fall back on strcmp.  We can't safely
     ignore strcoll failures, as a failing strcoll might be a
     comparison function that is not a total order, and if we ignored
     the failure this might cause qsort to dump core.  */

  إذا (! setjmp (failed_strcoll))
    use_strcmp = false;      /* strcoll() succeeded */
  وإلا
    {
      use_strcmp = true;
      affirm (sort_type != sort_version);
      initialize_ordering_vector ();
    }

  /* When sort_type == sort_time, use time_type as subindex.  */
  mpsort ((فراغ ثابت **) sorted_file, cwd_n_used,
          sort_functions[sort_type + (sort_type == sort_time ? time_type : 0)]
                        [use_strcmp][sort_reverse]
                        [directories_first]);
}

/* List all the files now in the table.  */

ساكن فراغ
print_current_files (فراغ)
{
  بدل (format)
    {
    حالة one_per_سطر:
      لكل (idx_t i = 0; i < cwd_n_used; i++)
        {
          print_file_name_and_frills (sorted_file[i], 0);
          putchar (eolbyte);
        }
      اكسر;

    حالة many_per_سطر:
      إذا (! سطر_length)
        print_with_separator (' ');
      وإلا
        print_many_per_سطر ();
      اكسر;

    حالة horizontal:
      إذا (! سطر_length)
        print_with_separator (' ');
      وإلا
        print_horizontal ();
      اكسر;

    حالة with_commas:
      print_with_separator (',');
      اكسر;

    حالة long_format:
      لكل (idx_t i = 0; i < cwd_n_used; i++)
        {
          set_normal_color ();
          print_long_format (sorted_file[i]);
          dired_outbyte (eolbyte);
        }
      اكسر;
    }
}

/* Replace the first %b with precomputed aligned month names.
   Note on glibc-2.7 at least, this speeds up the whole 'ls -lU'
   process by around 17%, compared to letting strftime() handle the %b.  */

ساكن ptrdiff_t
align_nstrftime (حرف *buf, size_t size, bool recent, بنية tm ثابت *tm,
                 timezone_t tz, صحيح ns)
{
  حرف ثابت *nfmt = (use_abformat
                      ? abformat[recent][tm->tm_mon]
                      : long_time_format[recent]);
  إرجاع nstrftime (buf, size, nfmt, tm, tz, ns);
}

/* Return the expected number of columns in a long-format timestamp,
   or zero if it cannot be calculated.  */

ساكن صحيح
long_time_expected_width (فراغ)
{
  ساكن صحيح width = -1;

  إذا (width < 0)
    {
      time_t epoch = 0;
      بنية tm tm;
      حرف buf[TIME_STAMP_LEN_MAXIMUM + 1];

      /* In case you're wondering if localtime_rz can fail with an input time_t
         value of 0, let's just say it's very unlikely, but not inconceivable.
         The TZ environment variable would have to specify a time zone that
         is 2**31-1900 years or more ahead of UTC.  This could happen only on
         a 64-bit system that blindly accepts e.g., TZ=UTC+20000000000000.
         However, this is not possible with Solaris 10 or glibc-2.3.5, since
         their implementations limit the offset to 167:59 and 24:00, resp.  */
      إذا (localtime_rz (localtz, &epoch, &tm))
        {
          ptrdiff_t len = align_nstrftime (buf, حجم buf, false,
                                           &tm, localtz, 0);
          إذا (len > 0)
            width = mbsnwidth (buf, len, MBSWIDTH_FLAGS);
        }

      إذا (width < 0)
        width = 0;
    }

  إرجاع width;
}

/* Print the user or group name NAME, with numeric id ID, using a
   print width of WIDTH columns.  */

ساكن فراغ
format_user_or_group (حرف ثابت *name, uintmax_t id, صحيح width)
{
  إذا (name)
    {
      صحيح name_width = mbswidth (name, MBSWIDTH_FLAGS);
      صحيح width_gap = name_width < 0 ? 0 : width - name_width;
      صحيح pad = MAX (0, width_gap);
      dired_outstring (name);

      افعل
        dired_outbyte (' ');
      بينما (pad--);
    }
  وإلا
    dired_pos += printf ("%*ju ", width, id);
}

/* Print the name or id of the user with id U, using a print width of
   WIDTH.  */

ساكن فراغ
format_user (uid_t u, صحيح width, bool stat_ok)
{
  format_user_or_group (! stat_ok ? "?" :
                        (numeric_ids ? عدم : getuser (u)), u, width);
}

/* Likewise, for groups.  */

ساكن فراغ
format_group (gid_t g, صحيح width, bool stat_ok)
{
  format_user_or_group (! stat_ok ? "?" :
                        (numeric_ids ? عدم : getgroup (g)), g, width);
}

/* Return the number of columns that format_user_or_group will print,
   or -1 if unknown.  */

ساكن صحيح
format_user_or_group_width (حرف ثابت *name, uintmax_t id)
{
  إرجاع (name
          ? mbswidth (name, MBSWIDTH_FLAGS)
          : snprintf (عدم, 0, "%ju", id));
}

/* Return the number of columns that format_user will print,
   or -1 if unknown.  */

ساكن صحيح
format_user_width (uid_t u)
{
  إرجاع format_user_or_group_width (numeric_ids ? عدم : getuser (u), u);
}

/* Likewise, for groups.  */

ساكن صحيح
format_group_width (gid_t g)
{
  إرجاع format_user_or_group_width (numeric_ids ? عدم : getgroup (g), g);
}

/* Return a pointer to a formatted version of F->stat.st_ino,
   possibly using buffer, which must be at least
   INT_BUFSIZE_BOUND (uintmax_t) bytes.  */
ساكن حرف *
format_inode (حرف buf[INT_BUFSIZE_BOUND (uintmax_t)],
              ثابت بنية fileinfo *f)
{
  إرجاع (f->stat_ok && f->stat.st_ino != NOT_AN_INODE_العدد
          ? umaxtostr (f->stat.st_ino, buf)
          : (حرف *) "?");
}

/* Print information about F in long format.  */
ساكن فراغ
print_long_format (ثابت بنية fileinfo *f)
{
  حرف modebuf[12];
  حرف buf
    [LONGEST_HUMAN_READABLE + 1		/* inode */
     + LONGEST_HUMAN_READABLE + 1	/* size in blocks */
     + حجم (modebuf) - 1 + 1		/* mode string */
     + INT_BUFSIZE_BOUND (uintmax_t)	/* st_nlink */
     + LONGEST_HUMAN_READABLE + 2	/* major device number */
     + LONGEST_HUMAN_READABLE + 1	/* minor device number */
     + TIME_STAMP_LEN_MAXIMUM + 1	/* max length of time/date */
     ];
  ptrdiff_t s;
  حرف *p;
  بنية timespec when_timespec;
  بنية tm when_local;
  bool btime_ok = true;

  /* Compute the mode string, except remove the trailing space if no
     file in this directory has an ACL or security context.  */
  إذا (f->stat_ok)
    filemodestring (&f->stat, modebuf);
  وإلا
    {
      modebuf[0] = filetype_letter[f->filetype];
      memset (modebuf + 1, '?', 10);
      modebuf[11] = '\0';
    }
  إذا (! any_has_acl)
    modebuf[10] = '\0';
  وإلا إذا (f->acl_type == ACL_T_LSM_CONTEXT_ONLY)
    modebuf[10] = '.';
  وإلا إذا (f->acl_type == ACL_T_YES)
    modebuf[10] = '+';
  وإلا إذا (f->acl_type == ACL_T_UNKNOWN)
    modebuf[10] = '?';

  بدل (time_type)
    {
    حالة time_ctime:
      when_timespec = get_stat_ctime (&f->stat);
      اكسر;
    حالة time_mtime:
      when_timespec = get_stat_mtime (&f->stat);
      اكسر;
    حالة time_atime:
      when_timespec = get_stat_atime (&f->stat);
      اكسر;
    حالة time_btime:
      when_timespec = get_stat_btime (&f->stat);
      إذا (when_timespec.tv_sec == -1 && when_timespec.tv_nsec == -1)
        btime_ok = false;
      اكسر;
    حالة time_numtypes: افتراضي:
      unreachable ();
    }

  p = buf;

  إذا (print_inode)
    {
      حرف hbuf[INT_BUFSIZE_BOUND (uintmax_t)];
      p += sprintf (p, "%*s ", inode_العدد_width, format_inode (hbuf, f));
    }

  إذا (print_block_size)
    {
      حرف hbuf[LONGEST_HUMAN_READABLE + 1];
      حرف ثابت *blocks =
        (! f->stat_ok
         ? "?"
         : human_readable (STP_NBLOCKS (&f->stat), hbuf, human_output_opts,
                           ST_NBLOCKSIZE, output_block_size));
      صحيح blocks_width = mbswidth (blocks, MBSWIDTH_FLAGS);
      لكل (صحيح pad = blocks_width < 0 ? 0 : block_size_width - blocks_width;
           0 < pad; pad--)
        *p++ = ' ';
      بينما ((*p++ = *blocks++))
        استمر;
      p[-1] = ' ';
    }

  /* The last byte of the mode string is the POSIX
     "optional alternate access method flag".  */
  {
    حرف hbuf[INT_BUFSIZE_BOUND (uintmax_t)];
    p += sprintf (p, "%s %*s ", modebuf, nlink_width,
                  ! f->stat_ok ? "?" : umaxtostr (f->stat.st_nlink, hbuf));
  }

  dired_indent ();

  إذا (print_owner || print_group || print_author || print_scontext)
    {
      dired_outbuf (buf, p - buf);

      إذا (print_owner)
        format_user (f->stat.st_uid, owner_width, f->stat_ok);

      إذا (print_group)
        format_group (f->stat.st_gid, group_width, f->stat_ok);

      إذا (print_author)
        format_user (f->stat.st_author, author_width, f->stat_ok);

      إذا (print_scontext)
        format_user_or_group (f->scontext, 0, scontext_width);

      p = buf;
    }

  إذا (f->stat_ok
      && (S_ISCHR (f->stat.st_mode) || S_ISBLK (f->stat.st_mode)))
    {
      صحيح blanks_width = (file_size_width
                          - (major_device_العدد_width + 2
                             + minor_device_العدد_width));
      p += sprintf (p, "%*ju, %*ju ",
                    major_device_العدد_width + MAX (0, blanks_width),
                    (uintmax_t) major (f->stat.st_rdev),
                    minor_device_العدد_width,
                    (uintmax_t) minor (f->stat.st_rdev));
    }
  وإلا
    {
      حرف hbuf[LONGEST_HUMAN_READABLE + 1];
      حرف ثابت *size =
        (! f->stat_ok
         ? "?"
         : human_readable (unsigned_file_size (f->stat.st_size),
                           hbuf, file_human_output_opts, 1,
                           file_output_block_size));
      صحيح size_width = mbswidth (size, MBSWIDTH_FLAGS);
      لكل (صحيح pad = size_width < 0 ? 0 : file_size_width - size_width;
           0 < pad; pad--)
        *p++ = ' ';
      بينما ((*p++ = *size++))
        استمر;
      p[-1] = ' ';
    }

  s = -1;

  إذا (f->stat_ok && btime_ok
      && localtime_rz (localtz, &when_timespec.tv_sec, &when_local))
    {
      بنية timespec six_months_ago;
      bool recent;

      /* If the file appears to be in the future, update the current
         time, in case the file happens to have been modified since
         the last time we checked the clock.  */
      إذا (timespec_cmp (current_time, when_timespec) < 0)
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

  إذا (0 <= s)
    {
      p += s;
      *p++ = ' ';
    }
  وإلا
    {
      /* The time cannot be converted using the desired format, so
         print it as a huge integer number of seconds.  */
      حرف hbuf[INT_BUFSIZE_BOUND (intmax_t)];
      p += sprintf (p, "%*s ", long_time_expected_width (),
                    (! f->stat_ok || ! btime_ok
                     ? "?"
                     : timetostr (when_timespec.tv_sec, hbuf)));
      /* FIXME: (maybe) We discarded when_timespec.tv_nsec. */
    }

  dired_outbuf (buf, p - buf);
  size_t w = print_name_with_quoting (f, false, &dired_obstack, p - buf);

  إذا (f->filetype == symbolic_link)
    {
      إذا (f->linkname)
        {
          dired_outstring (" -> ");
          print_name_with_quoting (f, true, عدم, (p - buf) + w + 4);
          إذا (indicator_style != none)
            print_type_indicator (true, f->linkmode, unknown);
        }
    }
  وإلا إذا (indicator_style != none)
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

ساكن size_t
quote_name_buf (حرف **inbuf, size_t bufsize, حرف *name,
                بنية quoting_options ثابت *options,
                صحيح needs_general_quoting, size_t *width, bool *pad)
{
  حرف *buf = *inbuf;
  size_t displayed_width IF_LINT ( = 0);
  size_t len = 0;
  bool quoted;

  تعداد quoting_style qs = get_quoting_style (options);
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

  إذا (needs_general_quoting != 0)
    {
      len = quotearg_buffer (buf, bufsize, name, -1, options);
      إذا (bufsize <= len)
        {
          buf = xmalloc (len + 1);
          quotearg_buffer (buf, len + 1, name, -1, options);
        }

      quoted = (*name != *buf) || strlen (name) != len;
    }
  وإلا إذا (needs_further_quoting || extra_newline_protection)
    {
      len = strlen (name);
      إذا (bufsize <= len)
        buf = xmalloc (len + 1);
      memcpy (buf, name, len + 1);

      quoted = false;
    }
  وإلا
    {
      len = strlen (name);
      buf = name;
      quoted = false;
    }

  /* It's safe to replace newlines without multi-byte iteration, as newlines
     do not appear as part of any multi-byte encoded character.  */
  إذا (extra_newline_protection)
    لكل (حرف *p = buf; p < buf + len; p++)
      إذا (eolbyte && *p == '\n')
        *p = '?';

  إذا (needs_further_quoting)
    {
      إذا (MB_CUR_MAX > 1)
        {
          حرف ثابت *p = buf;
          حرف ثابت *plimit = buf + len;
          حرف *q = buf;
          displayed_width = 0;

          بينما (p < plimit)
            بدل (*p)
              {
                حالة ' ': حالة '!': حالة '"': حالة '#': حالة '%':
                حالة '&': حالة '\'': حالة '(': حالة ')': حالة '*':
                حالة '+': حالة ',': حالة '-': حالة '.': حالة '/':
                حالة '0': حالة '1': حالة '2': حالة '3': حالة '4':
                حالة '5': حالة '6': حالة '7': حالة '8': حالة '9':
                حالة ':': حالة ';': حالة '<': حالة '=': حالة '>':
                حالة '?':
                حالة 'A': حالة 'B': حالة 'C': حالة 'D': حالة 'E':
                حالة 'F': حالة 'G': حالة 'H': حالة 'I': حالة 'J':
                حالة 'K': حالة 'L': حالة 'M': حالة 'N': حالة 'O':
                حالة 'P': حالة 'Q': حالة 'R': حالة 'S': حالة 'T':
                حالة 'U': حالة 'V': حالة 'W': حالة 'X': حالة 'Y':
                حالة 'Z':
                حالة '[': حالة '\\': حالة ']': حالة '^': حالة '_':
                حالة 'a': حالة 'b': حالة 'c': حالة 'd': حالة 'e':
                حالة 'f': حالة 'g': حالة 'h': حالة 'i': حالة 'j':
                حالة 'k': حالة 'l': حالة 'm': حالة 'n': حالة 'o':
                حالة 'p': حالة 'q': حالة 'r': حالة 's': حالة 't':
                حالة 'u': حالة 'v': حالة 'w': حالة 'x': حالة 'y':
                حالة 'z': حالة '{': حالة '|': حالة '}': حالة '~':
                  /* These characters are printable ASCII characters.  */
                  *q++ = *p++;
                  displayed_width += 1;
                  اكسر;
                افتراضي:
                  /* If we have a multibyte sequence, copy it until we
                     reach its end, replacing each non-printable multibyte
                     character with a single question mark.  */
                  {
                    mbstate_t mbstate; mbszero (&mbstate);
                    افعل
                      {
                        char32_t wc;
                        size_t bytes;
                        صحيح w;

                        bytes = mbrtoc32 (&wc, p, plimit - p, &mbstate);

                        إذا (bytes == (size_t) -1)
                          {
                            /* An invalid multibyte sequence was
                               encountered.  Skip one input byte, and
                               put a question mark.  */
                            p++;
                            *q++ = '?';
                            displayed_width += 1;
                            اكسر;
                          }

                        إذا (bytes == (size_t) -2)
                          {
                            /* An incomplete multibyte character
                               at the end.  Replace it entirely with
                               a question mark.  */
                            p = plimit;
                            *q++ = '?';
                            displayed_width += 1;
                            اكسر;
                          }

                        إذا (bytes == 0)
                          /* A null wide character was encountered.  */
                          bytes = 1;

                        w = c32width (wc);
                        إذا (w >= 0)
                          {
                            /* A printable multibyte character.
                               Keep it.  */
                            لكل (; bytes > 0; --bytes)
                              *q++ = *p++;
                            displayed_width += w;
                          }
                        وإلا
                          {
                            /* An nonprintable multibyte character.
                               Replace it entirely with a question
                               mark.  */
                            p += bytes;
                            *q++ = '?';
                            displayed_width += 1;
                          }
                      }
                    بينما (! mbsinit (&mbstate));
                  }
                  اكسر;
              }

          /* The buffer may have shrunk.  */
          len = q - buf;
        }
      وإلا
        {
          حرف *p = buf;
          حرف ثابت *plimit = buf + len;

          بينما (p < plimit)
            {
              إذا (! isprint (to_uchar (*p)))
                *p = '?';
              p++;
            }
          displayed_width = len;
        }
    }
  وإلا إذا (width != عدم)
    {
      إذا (MB_CUR_MAX > 1)
        {
          displayed_width = mbsnwidth (buf, len, MBSWIDTH_FLAGS);
          displayed_width = MAX (0, displayed_width);
        }
      وإلا
        {
          حرف ثابت *p = buf;
          حرف ثابت *plimit = buf + len;

          displayed_width = 0;
          بينما (p < plimit)
            {
              إذا (isprint (to_uchar (*p)))
                displayed_width++;
              p++;
            }
        }
    }

  /* Set padding to better align quoted items,
     and also give a visual indication that quotes are
     not actually part of the name.  */
  *pad = (align_variable_outer_quotes && cwd_some_quoted && ! quoted);

  إذا (width != عدم)
    *width = displayed_width;

  *inbuf = buf;

  إرجاع len;
}

ساكن size_t
quote_name_width (حرف ثابت *name, بنية quoting_options ثابت *options,
                  صحيح needs_general_quoting)
{
  حرف smallbuf[BUFSIZ];
  حرف *buf = smallbuf;
  size_t width;
  bool pad;

  quote_name_buf (&buf, حجم smallbuf, (حرف *) name, options,
                  needs_general_quoting, &width, &pad);

  إذا (buf != smallbuf && buf != name)
    حرر (buf);

  width += pad;

  إرجاع width;
}

/* %XX escape any input out of range as defined in RFC3986,
   and also if PATH, convert all path separators to '/'.  */
ساكن حرف *
file_escape (حرف ثابت *str, bool path)
{
  حرف *esc = xnmalloc (3, strlen (str) + 1);
  حرف *p = esc;
  بينما (*str)
    {
      إذا (path && ISSLASH (*str))
        {
          *p++ = '/';
          str++;
        }
      وإلا إذا (RFC3986[to_uchar (*str)])
        *p++ = *str++;
      وإلا
        p += sprintf (p, "%%%02x", to_uchar (*str++));
    }
  *p = '\0';
  إرجاع esc;
}

ساكن size_t
quote_name (حرف ثابت *name, بنية quoting_options ثابت *options,
            صحيح needs_general_quoting, ثابت بنية bin_str *color,
            bool allow_pad, بنية obstack *مكدس, حرف ثابت *absolute_name)
{
  حرف smallbuf[BUFSIZ];
  حرف *buf = smallbuf;
  size_t len;
  bool pad;

  len = quote_name_buf (&buf, حجم smallbuf, (حرف *) name, options,
                        needs_general_quoting, عدم, &pad);

  إذا (pad && allow_pad)
    dired_outbyte (' ');

  إذا (color)
    print_color_indicator (color);

  /* If we're padding, then don't include the outer quotes in
     the --hyperlink, to improve the alignment of those links.  */
  bool skip_quotes = false;

  إذا (absolute_name)
    {
      إذا (align_variable_outer_quotes && cwd_some_quoted && ! pad)
        {
          skip_quotes = true;
          putchar (*buf);
        }
      حرف *h = file_escape (hostname, /* path= */ false);
      حرف *n = file_escape (absolute_name, /* path= */ true);
      /* TODO: It would be good to be able to define parameters
         to give hints to the terminal as how best to render the URI.
         For example since ls is outputting a dense block of URIs
         it would be best to not underline by default, and only
         do so upon hover etc.  */
      printf ("\033]8;;file://%s%s%s\033\\", h, *n == '/' ? "" : "/", n);
      حرر (h);
      حرر (n);
    }

  إذا (مكدس)
    ادفع_current_dired_pos (مكدس);

  fwrite (buf + skip_quotes, 1, len - (skip_quotes * 2), stdout);

  dired_pos += len;

  إذا (مكدس)
    ادفع_current_dired_pos (مكدس);

  إذا (absolute_name)
    {
      fputs ("\033]8;;\033\\", stdout);
      إذا (skip_quotes)
        putchar (*(buf + len - 1));
    }

  إذا (buf != smallbuf && buf != name)
    حرر (buf);

  إرجاع len + pad;
}

ساكن size_t
print_name_with_quoting (ثابت بنية fileinfo *f,
                         bool symlink_target,
                         بنية obstack *مكدس,
                         size_t start_col)
{
  حرف ثابت *name = symlink_target ? f->linkname : f->name;

  ثابت بنية bin_str *color
    = print_with_color ? get_color_indicator (f, symlink_target) : عدم;

  bool used_color_this_time = (print_with_color
                               && (color || is_colored (C_NORM)));

  size_t len = quote_name (name, filename_quoting_options, f->quoted,
                           color, !symlink_target, مكدس, f->absolute_name);

  process_signals ();
  إذا (used_color_this_time)
    {
      prep_non_filename_text ();

      /* We use the byte length rather than display width here as
         an optimization to avoid accurately calculating the width,
         because we only output the clear to EOL sequence if the name
         _might_ wrap to the next line.  This may output a sequence
         unnecessarily in multi-byte locales for example,
         but in that case it's inconsequential to the output.  */
      إذا (سطر_length
          && (start_col / سطر_length != (start_col + len - 1) / سطر_length))
        put_indicator (&color_indicator[C_CLR_TO_EOL]);
    }

  إرجاع len;
}

ساكن فراغ
prep_non_filename_text (فراغ)
{
  إذا (color_indicator[C_END].string != عدم)
    put_indicator (&color_indicator[C_END]);
  وإلا
    {
      put_indicator (&color_indicator[C_يسار]);
      put_indicator (&color_indicator[C_RESET]);
      put_indicator (&color_indicator[C_يمين]);
    }
}

/* Print the file name of 'f' with appropriate quoting.
   Also print file size, inode number, and filetype indicator character,
   as requested by switches.  */

ساكن size_t
print_file_name_and_frills (ثابت بنية fileinfo *f, size_t start_col)
{
  حرف buf[MAX (LONGEST_HUMAN_READABLE + 1, INT_BUFSIZE_BOUND (uintmax_t))];

  set_normal_color ();

  إذا (print_inode)
    printf ("%*s ", format == with_commas ? 0 : inode_العدد_width,
            format_inode (buf, f));

  إذا (print_block_size)
    {
      حرف ثابت *blocks =
        (! f->stat_ok
         ? "?"
         : human_readable (STP_NBLOCKS (&f->stat), buf, human_output_opts,
                           ST_NBLOCKSIZE, output_block_size));
      صحيح blocks_width = mbswidth (blocks, MBSWIDTH_FLAGS);
      صحيح pad = 0;
      إذا (0 <= blocks_width && block_size_width && format != with_commas)
        pad = block_size_width - blocks_width;
      printf ("%*s%s ", pad, "", blocks);
    }

  إذا (print_scontext)
    printf ("%*s ", format == with_commas ? 0 : scontext_width, f->scontext);

  size_t width = print_name_with_quoting (f, false, عدم, start_col);

  إذا (indicator_style != none)
    width += print_type_indicator (f->stat_ok, f->stat.st_mode, f->filetype);

  إرجاع width;
}

/* Given these arguments describing a file, return the single-byte
   type indicator, or 0.  */
ساكن حرف
get_type_indicator (bool stat_ok, mode_t mode, تعداد filetype type)
{
  حرف c;

  إذا (stat_ok ? S_ISREG (mode) : type == normal)
    {
      إذا (stat_ok && indicator_style == classify && (mode & S_IXUGO))
        c = '*';
      وإلا
        c = 0;
    }
  وإلا
    {
      إذا (stat_ok ? S_ISDIR (mode) : type == directory || type == arg_directory)
        c = '/';
      وإلا إذا (indicator_style == slash)
        c = 0;
      وإلا إذا (stat_ok ? S_ISLNK (mode) : type == symbolic_link)
        c = '@';
      وإلا إذا (stat_ok ? S_ISFIFO (mode) : type == fifo)
        c = '|';
      وإلا إذا (stat_ok ? S_ISSOCK (mode) : type == sock)
        c = '=';
      وإلا إذا (stat_ok && S_ISDOOR (mode))
        c = '>';
      وإلا
        c = 0;
    }
  إرجاع c;
}

ساكن bool
print_type_indicator (bool stat_ok, mode_t mode, تعداد filetype type)
{
  حرف c = get_type_indicator (stat_ok, mode, type);
  إذا (c)
    dired_outbyte (c);
  إرجاع !!c;
}

/* Returns if color sequence was printed.  */
ساكن bool
print_color_indicator (ثابت بنية bin_str *ind)
{
  إذا (ind)
    {
      /* Need to reset so not dealing with attribute combinations */
      إذا (is_colored (C_NORM))
        restore_default_color ();
      put_indicator (&color_indicator[C_يسار]);
      put_indicator (ind);
      put_indicator (&color_indicator[C_يمين]);
    }

  إرجاع ind != عدم;
}

/* Returns color indicator or NULL if none.  */
ATTRIBUTE_PURE
ساكن ثابت بنية bin_str *
get_color_indicator (ثابت بنية fileinfo *f, bool symlink_target)
{
  تعداد indicator_no type;
  بنية color_ext_type *ext;	/* Color extension */
  size_t len;			/* Length of name */

  حرف ثابت *name;
  mode_t mode;
  صحيح linkok;
  إذا (symlink_target)
    {
      name = f->linkname;
      mode = f->linkmode;
      linkok = f->linkok ? 0 : -1;
    }
  وإلا
    {
      name = f->name;
      mode = file_or_link_mode (f);
      linkok = f->linkok;
    }

  /* Is this a nonexistent file?  If so, linkok == -1.  */

  إذا (linkok == -1 && is_colored (C_MISSING))
    type = C_MISSING;
  وإلا إذا (!f->stat_ok)
    {
      ساكن تعداد indicator_no ثابت filetype_indicator[] =
        {
          C_ORPHAN, C_FIFO, C_CHR, C_DIR, C_BLK, C_FILE,
          C_LINK, C_SOCK, C_FILE, C_DIR
        };
      static_assert (countof (filetype_indicator) == filetype_cardinality);
      type = filetype_indicator[f->filetype];
    }
  وإلا
    {
      إذا (S_ISREG (mode))
        {
          type = C_FILE;

          إذا ((mode & S_ISUID) != 0 && is_colored (C_SETUID))
            type = C_SETUID;
          وإلا إذا ((mode & S_ISGID) != 0 && is_colored (C_SETGID))
            type = C_SETGID;
          وإلا إذا (f->has_capability)
            type = C_CAP;
          وإلا إذا ((mode & S_IXUGO) != 0 && is_colored (C_EXEC))
            type = C_EXEC;
          وإلا إذا ((1 < f->stat.st_nlink) && is_colored (C_MULTIHARDLINK))
            type = C_MULTIHARDLINK;
        }
      وإلا إذا (S_ISDIR (mode))
        {
          type = C_DIR;

          إذا ((mode & S_ISVTX) && (mode & S_IWOTH)
              && is_colored (C_STICKY_OTHER_WRITABLE))
            type = C_STICKY_OTHER_WRITABLE;
          وإلا إذا ((mode & S_IWOTH) != 0 && is_colored (C_OTHER_WRITABLE))
            type = C_OTHER_WRITABLE;
          وإلا إذا ((mode & S_ISVTX) != 0 && is_colored (C_STICKY))
            type = C_STICKY;
        }
      وإلا إذا (S_ISLNK (mode))
        type = C_LINK;
      وإلا إذا (S_ISFIFO (mode))
        type = C_FIFO;
      وإلا إذا (S_ISSOCK (mode))
        type = C_SOCK;
      وإلا إذا (S_ISBLK (mode))
        type = C_BLK;
      وإلا إذا (S_ISCHR (mode))
        type = C_CHR;
      وإلا إذا (S_ISDOOR (mode))
        type = C_DOOR;
      وإلا
        {
          /* Classify a file of some other type as C_ORPHAN.  */
          type = C_ORPHAN;
        }
    }

  /* Check the file's suffix only if still classified as C_FILE.  */
  ext = عدم;
  إذا (type == C_FILE)
    {
      /* Test if NAME has a recognized suffix.  */

      len = strlen (name);
      name += len;		/* Pointer to final \0.  */
      لكل (ext = color_ext_list; ext != عدم; ext = ext->next)
        {
          إذا (ext->ext.len <= len)
            {
              إذا (ext->exact_match)
                {
                  إذا (STREQ_LEN (name - ext->ext.len, ext->ext.string,
                                 ext->ext.len))
                    اكسر;
                }
              وإلا
                {
                  إذا (c_strncasecmp (name - ext->ext.len, ext->ext.string,
                                     ext->ext.len) == 0)
                    اكسر;
                }
            }
        }
    }

  /* Adjust the color for orphaned symlinks.  */
  إذا (type == C_LINK && !linkok)
    {
      إذا (color_symlink_as_referent || is_colored (C_ORPHAN))
        type = C_ORPHAN;
    }

  ثابت بنية bin_str *ثابت s
    = ext ? &(ext->seq) : &color_indicator[type];

  إرجاع s->string ? s : عدم;
}

/* Output a color indicator (which may contain nulls).
   Note we don't update dired_pos here as emacs can be configured
   to handle color codes by stripping/converting them.  */
ساكن فراغ
put_indicator (ثابت بنية bin_str *ind)
{
  إذا (! used_color)
    {
      used_color = true;

      /* If the standard output is a controlling terminal, watch out
         for signals, so that the colors can be restored to the
         default state if "ls" is suspended or interrupted.  */

      إذا (0 <= tcgetpgrp (STDOUT_FILENO))
        signal_init ();

      prep_non_filename_text ();
    }

  fwrite (ind->string, ind->len, 1, stdout);
}

ساكن size_t
length_of_file_name_and_frills (ثابت بنية fileinfo *f)
{
  size_t len = 0;
  حرف buf[MAX (LONGEST_HUMAN_READABLE + 1, INT_BUFSIZE_BOUND (uintmax_t))];

  إذا (print_inode)
    len += 1 + (format == with_commas
                ? strlen (umaxtostr (f->stat.st_ino, buf))
                : inode_العدد_width);

  إذا (print_block_size)
    len += 1 + (format == with_commas
                ? strlen (! f->stat_ok ? "?"
                          : human_readable (STP_NBLOCKS (&f->stat), buf,
                                            human_output_opts, ST_NBLOCKSIZE,
                                            output_block_size))
                : block_size_width);

  إذا (print_scontext)
    len += 1 + (format == with_commas ? strlen (f->scontext) : scontext_width);

  len += fileinfo_name_width (f);

  إذا (indicator_style != none)
    {
      حرف c = get_type_indicator (f->stat_ok, f->stat.st_mode, f->filetype);
      len += (c != 0);
    }

  إرجاع len;
}

ساكن فراغ
print_many_per_سطر (فراغ)
{
  idx_t cols = calculate_columns (true);
  بنية column_info ثابت *سطر_fmt = &column_info[cols - 1];

  /* Calculate the number of rows that will be in each column except possibly
     for a short column on the right.  */
  idx_t rows = cwd_n_used / cols + (cwd_n_used % cols != 0);

  لكل (idx_t row = 0; row < rows; row++)
    {
      size_t col = 0;
      idx_t filesno = row;
      size_t pos = 0;

      /* Print the next row.  */
      بينما (true)
        {
          بنية fileinfo ثابت *f = sorted_file[filesno];
          size_t name_length = length_of_file_name_and_frills (f);
          size_t max_name_length = سطر_fmt->col_arr[col++];
          print_file_name_and_frills (f, pos);

          إذا (cwd_n_used - rows <= filesno)
            اكسر;
          filesno += rows;

          indent (pos + name_length, pos + max_name_length);
          pos += max_name_length;
        }
      putchar (eolbyte);
    }
}

ساكن فراغ
print_horizontal (فراغ)
{
  size_t pos = 0;
  idx_t cols = calculate_columns (false);
  بنية column_info ثابت *سطر_fmt = &column_info[cols - 1];
  بنية fileinfo ثابت *f = sorted_file[0];
  size_t name_length = length_of_file_name_and_frills (f);
  size_t max_name_length = سطر_fmt->col_arr[0];

  /* Print first entry.  */
  print_file_name_and_frills (f, 0);

  /* Now the rest.  */
  لكل (idx_t filesno = 1; filesno < cwd_n_used; filesno++)
    {
      idx_t col = filesno % cols;

      إذا (col == 0)
        {
          putchar (eolbyte);
          pos = 0;
        }
      وإلا
        {
          indent (pos + name_length, pos + max_name_length);
          pos += max_name_length;
        }

      f = sorted_file[filesno];
      print_file_name_and_frills (f, pos);

      name_length = length_of_file_name_and_frills (f);
      max_name_length = سطر_fmt->col_arr[col];
    }
  putchar (eolbyte);
}

/* Output name + SEP + ' '.  */

ساكن فراغ
print_with_separator (حرف sep)
{
  size_t pos = 0;

  لكل (idx_t filesno = 0; filesno < cwd_n_used; filesno++)
    {
      بنية fileinfo ثابت *f = sorted_file[filesno];
      size_t len = سطر_length ? length_of_file_name_and_frills (f) : 0;

      إذا (filesno != 0)
        {
          حرف separator;
          size_t next_pos = 2 + (sep == ',' && filesno < cwd_n_used - 1);

          إذا (! سطر_length
              || (! ckd_add (&next_pos, next_pos, pos)
                  && ! ckd_add (&next_pos, next_pos, len)
                  && next_pos <= سطر_length))
            {
              pos += 2;
              separator = ' ';
            }
          وإلا
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

ساكن فراغ
indent (size_t from, size_t to)
{
  بينما (from < to)
    {
      إذا (tabsize != 0 && to / tabsize > (from + 1) / tabsize)
        {
          putchar ('\t');
          from += tabsize - from % tabsize;
        }
      وإلا
        {
          putchar (' ');
          from++;
        }
    }
}

/* Put DIRNAME/NAME into DEST, handling '.' and '/' properly.  */
/* FIXME: maybe remove this function someday.  See about using a
   non-malloc'ing version of file_name_concat.  */

ساكن فراغ
attach (حرف *dest, حرف ثابت *dirname, حرف ثابت *name)
{
  حرف ثابت *dirnamep = dirname;

  /* Copy dirname if it is not ".".  */
  إذا (dirname[0] != '.' || dirname[1] != 0)
    {
      بينما (*dirnamep)
        *dest++ = *dirnamep++;
      /* Add '/' if 'dirname' doesn't already end with it.  */
      إذا (dirnamep > dirname && dirnamep[-1] != '/')
        *dest++ = '/';
    }
  بينما (*name)
    *dest++ = *name++;
  *dest = 0;
}

/* Allocate enough column info suitable for the current number of
   files and display columns, and initialize the info to represent the
   narrowest possible columns.  */

ساكن فراغ
init_column_info (idx_t max_cols)
{
  /* Currently allocated columns in column_info.  */
  ساكن idx_t column_info_alloc;

  إذا (column_info_alloc < max_cols)
    {
      idx_t old_column_info_alloc = column_info_alloc;
      column_info = xpalloc (column_info, &column_info_alloc,
                             max_cols - column_info_alloc, -1,
                             حجم *column_info);

      /* Allocate the new size_t objects by computing the triangle
         formula n * (n + 1) / 2, except that we don't need to
         allocate the part of the triangle that we've already
         allocated.  Check for address arithmetic overflow.  */
      idx_t column_info_growth = column_info_alloc - old_column_info_alloc, s;
      إذا (ckd_add (&s, old_column_info_alloc + 1, column_info_alloc)
          || ckd_mul (&s, s, column_info_growth))
        xalloc_die ();
      size_t *p = xinmalloc (s >> 1, حجم *p);

      /* Grow the triangle by parceling out the cells just allocated.  */
      لكل (idx_t i = old_column_info_alloc; i < column_info_alloc; i++)
        {
          column_info[i].col_arr = p;
          p += i + 1;
        }
    }

  لكل (idx_t i = 0; i < max_cols; ++i)
    {
      column_info[i].valid_len = true;
      column_info[i].سطر_len = i * MIN_COLUMN_WIDTH + 1;
      لكل (idx_t j = 0; j < i; ++j)
        column_info[i].col_arr[j] = MIN_COLUMN_WIDTH;
      column_info[i].col_arr[i] = 1;
    }
}

/* Calculate the number of columns needed to represent the current set
   of files in the current display width.  */

ساكن idx_t
calculate_columns (bool by_columns)
{
  /* Normally the maximum number of columns is determined by the
     screen width.  But if few files are available this might limit it
     as well.  */
  idx_t max_cols = 0 < max_idx && max_idx < cwd_n_used ? max_idx : cwd_n_used;

  init_column_info (max_cols);

  /* Compute the maximum number of possible columns.  */
  لكل (idx_t filesno = 0; filesno < cwd_n_used; ++filesno)
    {
      بنية fileinfo ثابت *f = sorted_file[filesno];
      size_t name_length = length_of_file_name_and_frills (f);

      لكل (idx_t i = 0; i < max_cols; ++i)
        {
          إذا (column_info[i].valid_len)
            {
              idx_t idx = (by_columns
                           ? filesno / ((cwd_n_used + i) / (i + 1))
                           : filesno % (i + 1));
              size_t real_length = name_length + (idx == i ? 0 : 2);

              إذا (column_info[i].col_arr[idx] < real_length)
                {
                  column_info[i].سطر_len += (real_length
                                              - column_info[i].col_arr[idx]);
                  column_info[i].col_arr[idx] = real_length;
                  column_info[i].valid_len = (column_info[i].سطر_len
                                              <= سطر_length);
                }
            }
        }
    }

  /* Find maximum allowed columns.  */
  idx_t cols;
  لكل (cols = max_cols; 1 < cols; --cols)
    {
      إذا (column_info[cols - 1].valid_len)
        اكسر;
    }

  إرجاع cols;
}

فراغ
usage (صحيح status)
{
  إذا (status != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      printf (_("Usage: %s [OPTION]... [FILE]...\n"), program_name);
      fputs (_("\
List information about the FILEs (the current directory by افتراضي).\n\
Sort entries alphabetically إذا none of -cftuvSUX nor --sort is specified.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -a, --all\n\
         افعل not ignore entries starting with .\n\
"));
      oputs (_("\
  -A, --almost-all\n\
         افعل not list implied . and ..\n\
"));
      oputs (_("\
      --author\n\
         with -l, print the author of each file\n\
"));
      oputs (_("\
  -b, --escape\n\
         print C-style escapes لكل nongraphic characters\n\
"));
      oputs (_("\
      --block-size=SIZE\n\
         with -l, scale sizes by SIZE when printing them;\n\
         e.g., '--block-size=M'; see SIZE format below\n\
"));
      oputs (_("\
  -B, --ignore-backups\n\
         افعل not list implied entries ending with ~\n\
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
         generate output designed لكل Emacs' dired mode\n\
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
         like -F, except افعل not append '*'\n\
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
         like -l, but افعل not list owner\n\
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
  -H, --dereference-command-سطر\n\
         follow symbolic links listed on the command سطر\n\
"));
      oputs (_("\
      --dereference-command-سطر-symlink-to-dir\n\
         follow each command سطر symbolic link that points to a directory\n\
"));
      oputs (_("\
      --hide=PATTERN\n\
         افعل not list implied entries matching shell PATTERN\n\
         (overridden by -a or -A)\n\
"));
      oputs (_("\
      --hyperlink[=WHEN]\n\
         hyperlink file names WHEN\n\
"));
      oputs (_("\
      --indicator-style=WORD\n\
         append indicator with style WORD to entry names:\n\
           none (افتراضي), slash (-p), file-type (--file-type), classify (-F)\n\
"));
      oputs (_("\
  -i, --inode\n\
         print the index العدد of each file\n\
"));
      oputs (_("\
  -I, --ignore=PATTERN\n\
         افعل not list implied entries matching shell PATTERN\n\
"));
      oputs (_("\
  -k, --kibibytes\n\
         افتراضي to 1024-byte blocks لكل file system usage;\n\
         used only with -s and per directory totals\n\
"));
      oputs (_("\
  -l\n\
         use a long listing format\n\
"));
      oputs (_("\
  -L, --dereference\n\
         when showing file information لكل a symbolic link,\n\
         show information لكل the file the link references\n\
         rather than لكل the link itself\n\
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
         like -l, but افعل not list group information\n\
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
         the افتراضي, unless program is 'ls' and output is a terminal\n\
"));
      oputs (_("\
  -Q, --quote-name\n\
         enclose entry names in مزدوج quotes\n\
"));
      oputs (_("\
      --quoting-style=WORD\n\
         use quoting style WORD لكل entry names:\n\
           literal, locale, shell, shell-always,\n\
           shell-escape, shell-escape-always, c, escape\n\
         (overrides QUOTING_STYLE environment variable)\n\
"));
      oputs (_("\
  -r, --reverse\n\
         reverse order بينما sorting\n\
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
         change افتراضي 'name' sort to WORD:\n\
           none (-U), size (-S), time (-t),\n\
           version (-v), extension (-X), name, width\n\
"));
      oputs (_("\
      --time=WORD\n\
         select which timestamp used to display or sort;\n\
           access time (-u): atime, access, use;\n\
           metadata change time (-c): ctime, status;\n\
           modified time (افتراضي): mtime, modification;\n\
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
         افعل not sort directory entries\n\
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
         list entries by أسطر instead of by columns\n\
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
         end each output سطر with NUL, not newline\n\
"));
      oputs (_("\
  -1\n\
         list one file per سطر\n\
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
Also the TIME_STYLE environment variable sets the افتراضي style to use.\n\
"), stdout);
      fputs (_("\
\n\
The WHEN argument defaults to 'always' and can also be 'auto' or 'never'.\n\
"), stdout);
      fputs (_("\
\n\
Using color to distinguish file types is disabled both by افتراضي and\n\
with --color=never.  With --color=auto, ls emits color codes only when\n\
standard output is connected to a terminal.  The LS_COLORS environment\n\
variable can change the settings.  Use the dircolors(1) command to set it.\n\
"), stdout);
      fputs (_("\
\n\
Exit status:\n\
 0  إذا OK,\n\
 1  إذا minor problems (e.g., cannot access subdirectory),\n\
 2  إذا serious trouble (e.g., cannot access command-سطر argument).\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}
