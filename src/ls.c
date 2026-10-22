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

#شامل <config.h>
#شامل <sys/types.h>

#شامل <termios.h>
#اگر HAVE_STROPTS_H
# include <stropts.h>
#endif
#شامل <sys/ioctl.h>

#ifdef WINSIZE_IN_PTEM
# include <sys/stream.h>
# include <sys/ptem.h>
#endif

#شامل <stdio.h>
#شامل <setjmp.h>
#شامل <pwd.h>
#شامل <getopt.h>
#شامل <signal.h>

#اگر HAVE_LANGINFO_CODESET
# include <langinfo.h>
#endif

/* NonStop circa 2011 lacks both SA_RESTART and siginterrupt, so don't
   restart syscalls after a signal handler fires.  This may cause
   colors to get messed up on the screen if 'ls' is interrupted, but
   that's the best we can do on such a platform.  */
#ifndef SA_RESTART
# define SA_RESTART 0
#endif

#شامل <fnmatch.h>

/* Gnulib includes.  */
#شامل "acl.h"
#شامل "areadlink.h"
#شامل "argmatch.h"
#شامل "assure.h"
#شامل "c-ctype.h"
#شامل "c-strcase.h"
#شامل "canonicalize.h"
#شامل "dev-ino.h"
#شامل "filemode.h"
#شامل "filenamecat.h"
#شامل "filevercmp.h"
#شامل "hard-locale.h"
#شامل "hash.h"
#شامل "human.h"
#شامل "idcache.h"
#شامل "mbswidth.h"
#شامل "mpsort.h"
#شامل "obstack.h"
#شامل "quote.h"
#شامل "stat-size.h"
#شامل "stat-time.h"
#شامل "strftime.h"
#شامل "xdectoint.h"
#شامل "xgethostname.h"
#شامل "xmemdup0.h"
#شامل "xstrtol-error.h"
#شامل "xstrtol.h"

#شامل "system.h"

#شامل "dircolors.h"
#شامل "ls.h"
#شامل "statx.h"
#شامل "term-sig.h"

/* Include <sys/capability.h> last to avoid a clash of <sys/types.h>
   include guards with some premature versions of libcap.
   For more details, see <https://bugzilla.redhat.com/483548>.  */
#ifdef HAVE_CAP
# include <sys/capability.h>
#endif

#اگر HAVE_LINUX_XATTR_H
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
#define obstack_chunk_free آزاد

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

#اگر READDIR_LIES_ABOUT_MOUNTPOINT_D_INO
# define RELIABLE_D_INO(dp) NOT_AN_INODE_عدد
#وگرنه
# define RELIABLE_D_INO(dp) D_INO (dp)
#endif

#اگر ! HAVE_STRUCT_STAT_ST_AUTHOR
# define st_author st_uid
#endif

شمارش filetype
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
شمارش { filetype_cardinality = arg_directory + 1 };

/* Display letters and indicators for each filetype.
   Keep these in sync with enum filetype.  */
ایستا نویسه ثابت filetype_letter[] =
  {'?', 'p', 'c', 'd', 'b', '-', 'l', 's', 'w', 'd'};
static_assert (countof (filetype_letter) == filetype_cardinality);

/* Map enum filetype to <dirent.h> d_type values.  */
ایستا unsigned نویسه ثابت filetype_d_type[] =
  {
    DT_UNKNOWN, DT_FIFO, DT_CHR, DT_DIR, DT_BLK, DT_REG, DT_LNK, DT_SOCK,
    DT_WHT, DT_DIR
  };
static_assert (countof (filetype_d_type) == filetype_cardinality);

/* Map d_type values to enum filetype.  */
ایستا نویسه ثابت d_type_filetype[UCHAR_MAX + 1] =
  {
    [DT_BLK] = blockdev, [DT_CHR] = chardev, [DT_DIR] = directory,
    [DT_FIFO] = fifo, [DT_LNK] = symbolic_link, [DT_REG] = normal,
    [DT_SOCK] = sock, [DT_WHT] = whiteout
  };

شمارش acl_type
  {
    ACL_T_NONE,
    ACL_T_UNKNOWN,
    ACL_T_LSM_CONTEXT_ONLY,
    ACL_T_YES
  };

ساختار fileinfo
  {
    /* The file name.  */
    نویسه *name;

    /* For symbolic link, name of the file linked to, otherwise zero.  */
    نویسه *linkname;

    /* For terminal hyperlinks. */
    نویسه *absolute_name;

    ساختار stat stat;

    شمارش filetype filetype;

    /* For symbolic link and long listing, st_mode of file linked to, otherwise
       zero.  */
    mode_t linkmode;

    /* security context.  */
    نویسه *scontext;

    bool stat_ok;

    /* For symbolic link and color printing, true if linked-to file
       exists, otherwise false.  */
    bool linkok;

    /* For long listings, true if the file has an access control list,
       or a security context.  */
    شمارش acl_type acl_type;

    /* For color listings, true if a regular file has capability info.  */
    bool has_capability;

    /* Whether file name needs quoting. tri-state with -1 == unknown.  */
    صحیح quoted;

    /* Cached screen width (including quoting).  */
    size_t width;
  };

/* Null is a valid character in a color indicator (think about Epson
   printers, for example) so we have to use a length/buffer string
   type.  */

ساختار bin_str
  {
    size_t len;			/* Number of bytes */
    نویسه ثابت *string;		/* Pointer to the same */
  };

#اگر ! HAVE_TCGETPGRP
# define tcgetpgrp(Fd) 0
#endif

ایستا size_t quote_name (نویسه ثابت *name,
                          ساختار quoting_options ثابت *options,
                          صحیح needs_general_quoting,
                          ثابت ساختار bin_str *color,
                          bool allow_pad, ساختار obstack *پشته,
                          نویسه ثابت *absolute_name);
ایستا size_t quote_name_buf (نویسه **inbuf, size_t bufsize, نویسه *name,
                              ساختار quoting_options ثابت *options,
                              صحیح needs_general_quoting, size_t *width,
                              bool *pad);
ایستا صحیح decode_switches (صحیح argc, نویسه **argv);
ایستا bool file_ignored (نویسه ثابت *name);
ایستا uintmax_t gobble_file (نویسه ثابت *name, شمارش filetype type,
                              ino_t inode, bool command_سطر_arg,
                              نویسه ثابت *dirname);
ایستا ثابت ساختار bin_str * get_color_indicator (ثابت ساختار fileinfo *f,
                                                   bool symlink_target);
ایستا bool print_color_indicator (ثابت ساختار bin_str *ind);
ایستا تهی put_indicator (ثابت ساختار bin_str *ind);
ایستا تهی add_ignore_pattern (نویسه ثابت *pattern);
ایستا تهی attach (نویسه *dest, نویسه ثابت *dirname, نویسه ثابت *name);
ایستا تهی clear_files (تهی);
ایستا تهی extract_dirs_from_files (نویسه ثابت *dirname,
                                     bool command_سطر_arg);
ایستا bool get_link_name (نویسه ثابت *filename, ساختار fileinfo *f,
                           bool command_سطر_arg);
ایستا تهی indent (size_t from, size_t to);
ایستا idx_t calculate_columns (bool by_columns);
ایستا تهی print_current_files (تهی);
ایستا تهی print_dir (نویسه ثابت *name, نویسه ثابت *realname,
                       bool command_سطر_arg);
ایستا size_t print_file_name_and_frills (ثابت ساختار fileinfo *f,
                                          size_t start_col);
ایستا تهی print_horizontal (تهی);
ایستا صحیح format_user_width (uid_t u);
ایستا صحیح format_group_width (gid_t g);
ایستا تهی print_long_format (ثابت ساختار fileinfo *f);
ایستا تهی print_many_per_سطر (تهی);
ایستا size_t print_name_with_quoting (ثابت ساختار fileinfo *f,
                                       bool symlink_target,
                                       ساختار obstack *پشته,
                                       size_t start_col);
ایستا تهی prep_non_filename_text (تهی);
ایستا bool print_type_indicator (bool stat_ok, mode_t mode,
                                  شمارش filetype type);
ایستا تهی print_with_separator (نویسه sep);
ایستا تهی queue_directory (نویسه ثابت *name, نویسه ثابت *realname,
                             bool command_سطر_arg);
ایستا تهی sort_files (تهی);
ایستا تهی parse_ls_color (تهی);

ایستا size_t quote_name_width (نویسه ثابت *name,
                                ساختار quoting_options ثابت *options,
                                صحیح needs_general_quoting);

/* Initial size of hash table.
   Most hierarchies are likely to be shallower than this.  */
شمارش { INITIAL_TABLE_SIZE = 30 };

/* The set of 'active' directories, from the current command-line argument
   to the level in the hierarchy at which files are being listed.
   A directory is represented by its device and inode numbers (struct dev_ino).
   A directory is added to this set when ls begins listing it or its
   entries, and it is removed from the set just after ls has finished
   processing it.  This set is used solely to detect loops, e.g., with
   mkdir loop; cd loop; ln -s ../loop sub; ls -RL  */
ایستا Hash_table *active_dir_set;

#define LOOP_DETECT (!!active_dir_set)

/* The table of files in the current directory:

   'cwd_file' points to a vector of 'struct fileinfo', one per file.
   'cwd_n_alloc' is the number of elements space has been allocated for.
   'cwd_n_used' is the number actually in use.  */

/* Address of block containing the files that are described.  */
ایستا ساختار fileinfo *cwd_file;

/* Length of block that 'cwd_file' points to, measured in files.  */
ایستا idx_t cwd_n_alloc;

/* Index of first unused slot in 'cwd_file'.  */
ایستا idx_t cwd_n_used;

/* Whether files needs may need padding due to quoting.  */
ایستا bool cwd_some_quoted;

/* Whether quoting style _may_ add outer quotes,
   and whether aligning those is useful.  */
ایستا bool align_variable_outer_quotes;

/* Vector of pointers to files, in proper sorted order, and the number
   of entries allocated for it.  */
ایستا تهی **sorted_file;
ایستا size_t sorted_file_alloc;

/* When true, in a color listing, color each symlink name according to the
   type of file it points to.  Otherwise, color them according to the 'ln'
   directive in LS_COLORS.  Dangling (orphan) symlinks are treated specially,
   regardless.  This is set when 'ln=target' appears in LS_COLORS.  */

ایستا bool color_symlink_as_referent;

ایستا نویسه ثابت *hostname;

/* Mode of appropriate file for coloring.  */
ایستا mode_t
file_or_link_mode (ساختار fileinfo ثابت *file)
{
  بازگشت (color_symlink_as_referent && file->linkok
          ? file->linkmode : file->stat.st_mode);
}


/* Record of one pending directory waiting to be listed.  */

ساختار pending
  {
    نویسه *name;
    /* If the directory is actually the file pointed to by a symbolic link we
       were told to list, 'realname' will contain the name of the symbolic
       link, otherwise zero.  */
    نویسه *realname;
    bool command_سطر_arg;
    ساختار pending *next;
  };

ایستا ساختار pending *pending_dirs;

/* Current time in seconds and nanoseconds since 1970, updated as
   needed when deciding whether a file is recent.  */

ایستا ساختار timespec current_time;

ایستا bool print_scontext;
ایستا نویسه UNKNOWN_SECURITY_CONTEXT[] = "?";

/* Whether any of the files has an ACL.  This affects the width of the
   mode column.  */

ایستا bool any_has_acl;

/* The number of columns to use for columns containing inode numbers,
   block sizes, link counts, owners, groups, authors, major device
   numbers, minor device numbers, and file sizes, respectively.  */

ایستا صحیح inode_عدد_width;
ایستا صحیح block_size_width;
ایستا صحیح nlink_width;
ایستا صحیح scontext_width;
ایستا صحیح owner_width;
ایستا صحیح group_width;
ایستا صحیح author_width;
ایستا صحیح major_device_عدد_width;
ایستا صحیح minor_device_عدد_width;
ایستا صحیح file_size_width;

/* Option flags */

/* long_format for lots of info, one per line.
   one_per_line for just names, one per line.
   many_per_line for just names, many per line, sorted vertically.
   horizontal for just names, many per line, sorted horizontally.
   with_commas for just names, many per line, separated by commas.

   -l (and other options that imply -l), -1, -C, -x and -m control
   this parameter.  */

شمارش format
  {
    long_format,		/* -l and other options that imply -l */
    one_per_سطر,		/* -1 */
    many_per_سطر,		/* -C */
    horizontal,			/* -x */
    with_commas			/* -m */
  };

ایستا شمارش format format;

/* 'full-iso' uses full ISO-style dates and times.  'long-iso' uses longer
   ISO-style timestamps, though shorter than 'full-iso'.  'iso' uses shorter
   ISO-style timestamps.  'locale' uses locale-dependent timestamps.  */
شمارش time_style
  {
    full_iso_time_style,	/* --time-style=full-iso */
    long_iso_time_style,	/* --time-style=long-iso */
    iso_time_style,		/* --time-style=iso */
    locale_time_style		/* --time-style=locale */
  };

ایستا نویسه ثابت *ثابت time_style_args[] =
{
  "full-iso", "long-iso", "iso", "locale", پوچ
};
ایستا شمارش time_style ثابت time_style_types[] =
{
  full_iso_time_style, long_iso_time_style, iso_time_style,
  locale_time_style
};
ARGMATCH_VERIFY (time_style_args, time_style_types);

/* Type of time to print or sort by.  Controlled by -c and -u.
   The values of each item of this enum are important since they are
   used as indices in the sort functions array (see sort_files()).  */

شمارش time_type
  {
    time_mtime = 0,		/* default */
    time_ctime,			/* -c */
    time_atime,			/* -u */
    time_btime,                 /* birth time */
    time_numtypes		/* the number of elements of this enum */
  };

ایستا شمارش time_type time_type;
ایستا bool explicit_time;

/* The file characteristic to sort by.  Controlled by -t, -S, -U, -X, -v.
   The values of each item of this enum are important since they are
   used as indices in the sort functions array (see sort_files()).  */

شمارش sort_type
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

ایستا شمارش sort_type sort_type;

/* Direction of sort.
   false means highest first if numeric,
   lowest first if alphabetic;
   these are the defaults.
   true means the opposite order in each case.  -r  */

ایستا bool sort_reverse;

/* True means to display owner information.  -g turns this off.  */

ایستا bool print_owner = true;

/* True means to display author information.  */

ایستا bool print_author;

/* True means to display group information.  -G and -o turn this off.  */

ایستا bool print_group = true;

/* True means print the user and group id's as numbers rather
   than as names.  -n  */

ایستا bool numeric_ids;

/* True means mention the size in blocks of each file.  -s  */

ایستا bool print_block_size;

/* Human-readable options for output, when printing block counts.  */
ایستا صحیح human_output_opts;

/* The units to use when printing block counts.  */
ایستا uintmax_t output_block_size;

/* Likewise, but for file sizes.  */
ایستا صحیح file_human_output_opts;
ایستا uintmax_t file_output_block_size = 1;

/* Follow the output with a special string.  Using this format,
   Emacs' dired mode starts up twice as fast, and can handle all
   strange characters in file names.  */
ایستا bool dired;

/* 'none' means don't mention the type of files.
   'slash' means mention directories only, with a '/'.
   'file_type' means mention file types.
   'classify' means mention file types and mark executables.

   Controlled by -F, -p, and --indicator-style.  */

شمارش indicator_style
  {
    none = 0,	/*     --indicator-style=none (default) */
    slash,	/* -p, --indicator-style=slash */
    file_type,	/*     --indicator-style=file-type */
    classify	/* -F, --indicator-style=classify */
  };

ایستا شمارش indicator_style indicator_style;

/* Names of indicator styles.  */
ایستا نویسه ثابت *ثابت indicator_style_args[] =
{
  "none", "slash", "file-type", "classify", پوچ
};
ایستا شمارش indicator_style ثابت indicator_style_types[] =
{
  none, slash, file_type, classify
};
ARGMATCH_VERIFY (indicator_style_args, indicator_style_types);

/* True means use colors to mark types.  Also define the different
   colors as well as the stuff for the LS_COLORS environment variable.
   The LS_COLORS variable is now in a termcap-like format.  */

ایستا bool print_with_color;

ایستا bool print_hyperlink;

/* Whether we used any colors in the output so far.  If so, we will
   need to restore the default color later.  If not, we will need to
   call prep_non_filename_text before using color for the first time. */

ایستا bool used_color = false;

شمارش when_type
  {
    when_never,		/* 0: default or --color=never */
    when_always,	/* 1: --color=always */
    when_if_tty		/* 2: --color=tty */
  };

شمارش Dereference_symlink
  {
    DEREF_UNDEFINED = 0,		/* default */
    DEREF_NEVER,
    DEREF_COMMAND_سطر_ARGUMENTS,	/* -H */
    DEREF_COMMAND_سطر_SYMLINK_TO_DIR,	/* the default, in certain cases */
    DEREF_ALWAYS			/* -L */
  };

شمارش indicator_no
  {
    C_چپ, C_راست, C_END, C_RESET, C_NORM, C_FILE, C_DIR, C_LINK,
    C_FIFO, C_SOCK,
    C_BLK, C_CHR, C_MISSING, C_ORPHAN, C_EXEC, C_DOOR, C_SETUID, C_SETGID,
    C_STICKY, C_OTHER_WRITABLE, C_STICKY_OTHER_WRITABLE, C_CAP, C_MULTIHARDLINK,
    C_CLR_TO_EOL
  };

ایستا نویسه ثابت indicator_name[][2]=
  {
    {'l','c'}, {'r','c'}, {'e','c'}, {'r','s'}, {'n','o'},
    {'f','i'}, {'d','i'}, {'l','n'},
    {'p','i'}, {'s','o'},
    {'b','d'}, {'c','d'}, {'m','i'}, {'o','r'}, {'e','x'},
    {'d','o'}, {'s','u'}, {'s','g'},
    {'s','t'}, {'o','w'}, {'t','w'}, {'c','a'}, {'m','h'},
    {'c','l'}
  };

ساختار color_ext_type
  {
    ساختار bin_str ext;		/* The extension we're looking for */
    ساختار bin_str seq;		/* The sequence to output when we do */
    bool   exact_match;		/* Whether to compare case insensitively */
    ساختار color_ext_type *next;	/* Next in list */
  };

ایستا ساختار bin_str color_indicator[] =
  {
    { 2, (نویسه ثابت []) {'\033','['} },/* lc: Left of color sequence */
    { 1, (نویسه ثابت []) {'m'} },	/* rc: Right of color sequence */
    { 0, پوچ },			/* ec: End color (replaces lc+rs+rc) */
    { 1, (نویسه ثابت []) {'0'} },	/* rs: Reset to ordinary colors */
    { 0, پوچ },			/* no: Normal */
    { 0, پوچ },			/* fi: File: default */
    { 5, ((نویسه ثابت [])
          {'0','1',';','3','4'}) },	/* di: Directory: bright blue */
    { 5, ((نویسه ثابت [])
          {'0','1',';','3','6'}) },	/* ln: Symlink: bright cyan */
    { 2, (نویسه ثابت []) {'3','3'} },		/* pi: Pipe: yellow/brown */
    { 5, ((نویسه ثابت [])
          {'0','1',';','3','5'}) },	/* so: Socket: bright magenta */
    { 5, ((نویسه ثابت [])
          {'0','1',';','3','3'}) },	/* bd: Block device: bright yellow */
    { 5, ((نویسه ثابت [])
          {'0','1',';','3','3'}) },	/* cd: Char device: bright yellow */
    { 0, پوچ },			/* mi: Missing file: undefined */
    { 0, پوچ },			/* or: Orphaned symlink: undefined */
    { 5, ((نویسه ثابت [])
          {'0','1',';','3','2'}) },	/* ex: Executable: bright green */
    { 5, ((نویسه ثابت [])
          {'0','1',';','3','5'}) },	/* do: Door: bright magenta */
    { 5, ((نویسه ثابت [])
          {'3','7',';','4','1'}) },	/* su: setuid: white on red */
    { 5, ((نویسه ثابت [])
          {'3','0',';','4','3'}) },	/* sg: setgid: black on yellow */
    { 5, ((نویسه ثابت [])
          {'3','7',';','4','4'}) },	/* st: sticky: black on blue */
    { 5, ((نویسه ثابت [])
          {'3','4',';','4','2'}) },	/* ow: other-writable: blue on green */
    { 5, ((نویسه ثابت [])
          {'3','0',';','4','2'}) },	/* tw: ow w/ sticky: black on green */
    { 0, پوچ },			/* ca: disabled by default */
    { 0, پوچ },			/* mh: disabled by default */
    { 3, ((نویسه ثابت [])
          {'\033','[','K'}) },		/* cl: clear to end of line */
  };

/* A list mapping file extensions to corresponding display sequence.  */
ایستا ساختار color_ext_type *color_ext_list = پوچ;

/* Buffer for color sequences */
ایستا نویسه *color_buf;

/* True means to check for orphaned symbolic link, for displaying
   colors, or to group symlink to directories with other dirs.  */

ایستا bool check_symlink_mode;

/* True means mention the inode number of each file.  -i  */

ایستا bool print_inode;

/* What to do with symbolic links.  Affected by -d, -F, -H, -l (and
   other options that imply -l), and -L.  */

ایستا شمارش Dereference_symlink dereference;

/* True means when a directory is found, display info on its
   contents.  -R  */

ایستا bool recursive;

/* True means when an argument is a directory name, display info
   on it itself.  -d  */

ایستا bool immediate_dirs;

/* True means that directories are grouped before files. */

ایستا bool directories_first;

/* Which files to ignore.  */

ایستا شمارش
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

ساختار ignore_pattern
  {
    نویسه ثابت *pattern;
    ساختار ignore_pattern *next;
  };

ایستا ساختار ignore_pattern *ignore_patterns;

/* Similar to IGNORE_PATTERNS, except that -a or -A causes this
   variable itself to be ignored.  */
ایستا ساختار ignore_pattern *hide_patterns;

/* True means output nongraphic chars in file names as '?'.
   (-q, --hide-control-chars)
   qmark_funny_chars and the quoting style (-Q, --quoting-style=WORD) are
   independent.  The algorithm is: first, obey the quoting style to get a
   string representing the file name;  then, if qmark_funny_chars is set,
   replace all nonprintable chars in that string with '?'.  It's necessary
   to replace nonprintable chars even in quoted strings, because we don't
   want to mess up the terminal if control chars get sent to it, and some
   quoting methods pass through control chars as-is.  */
ایستا bool qmark_funny_chars;

/* True means extra replacement of newline with '?'
   in cases where a newline in a file name would be
   ambiguous with newline separators.  */
ایستا bool qmark_newline;

/* Quoting options for file and dir name output.  */

ایستا ساختار quoting_options *filename_quoting_options;
ایستا ساختار quoting_options *dirname_quoting_options;

/* The number of chars per hardware tab stop.  Setting this to zero
   inhibits the use of TAB characters for separating columns.  -T */
ایستا size_t tabsize;

/* True means print each directory name before listing it.  */

ایستا bool print_dir_name;

/* The line length to use for breaking lines in many-per-line format.
   Can be set with -w.  If zero, there is no limit.  */

ایستا size_t سطر_length;

/* The local time zone rules, as per the TZ environment variable.  */

ایستا timezone_t localtz;

/* If true, the file listing format requires that stat be called on
   each file.  */

ایستا bool format_needs_stat;

/* Similar to 'format_needs_stat', but set if only the file type is
   needed.  */

ایستا bool format_needs_type;

/* Like 'format_needs_stat', but set only if capability colors are needed.  */

ایستا bool format_needs_capability;

/* An arbitrary limit on the number of bytes in a printed timestamp.
   This is set to a relatively small value to avoid the need to worry
   about denial-of-service attacks on servers that run "ls" on behalf
   of remote clients.  1000 bytes should be enough for any practical
   timestamp format.  */

شمارش { TIME_STAMP_LEN_MAXIMUM = MAX (1000, INT_STRLEN_BOUND (time_t)) };

/* strftime formats for non-recent and recent files, respectively, in
   -l output.  */

ایستا نویسه ثابت *long_time_format[2] =
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

ایستا sigset_t caught_signals;

/* If nonzero, the value of the pending fatal signal.  */

ایستا sig_atomic_t فرار interrupt_signal;

/* A count of the number of pending stop signals that have been received.  */

ایستا sig_atomic_t فرار stop_signal_شمار;

/* Desired exit status.  */

ایستا صحیح exit_status;

/* Exit statuses.  */
شمارش
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
شمارش
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

ایستا ساختار option ثابت long_options[] =
{
  {"all", no_argument, پوچ, 'a'},
  {"escape", no_argument, پوچ, 'b'},
  {"directory", no_argument, پوچ, 'd'},
  {"dired", no_argument, پوچ, 'D'},
  {"full-time", no_argument, پوچ, FULL_TIME_OPTION},
  {"group-directories-first", no_argument, پوچ,
   GROUP_DIRECTORIES_FIRST_OPTION},
  {"human-readable", no_argument, پوچ, 'h'},
  {"inode", no_argument, پوچ, 'i'},
  {"kibibytes", no_argument, پوچ, 'k'},
  {"numeric-uid-gid", no_argument, پوچ, 'n'},
  {"no-group", no_argument, پوچ, 'G'},
  {"hide-control-chars", no_argument, پوچ, 'q'},
  {"reverse", no_argument, پوچ, 'r'},
  {"size", no_argument, پوچ, 's'},
  {"width", required_argument, پوچ, 'w'},
  {"almost-all", no_argument, پوچ, 'A'},
  {"ignore-backups", no_argument, پوچ, 'B'},
  {"classify", optional_argument, پوچ, 'F'},
  {"file-type", no_argument, پوچ, FILE_TYPE_INDICATOR_OPTION},
  {"si", no_argument, پوچ, SI_OPTION},
  {"dereference-command-line", no_argument, پوچ, 'H'},
  {"dereference-command-line-symlink-to-dir", no_argument, پوچ,
   DEREFERENCE_COMMAND_سطر_SYMLINK_TO_DIR_OPTION},
  {"hide", required_argument, پوچ, HIDE_OPTION},
  {"ignore", required_argument, پوچ, 'I'},
  {"indicator-style", required_argument, پوچ, INDICATOR_STYLE_OPTION},
  {"dereference", no_argument, پوچ, 'L'},
  {"literal", no_argument, پوچ, 'N'},
  {"quote-name", no_argument, پوچ, 'Q'},
  {"quoting-style", required_argument, پوچ, QUOTING_STYLE_OPTION},
  {"recursive", no_argument, پوچ, 'R'},
  {"format", required_argument, پوچ, FORMAT_OPTION},
  {"show-control-chars", no_argument, پوچ, SHOW_CONTROL_CHARS_OPTION},
  {"sort", required_argument, پوچ, SORT_OPTION},
  {"tabsize", required_argument, پوچ, 'T'},
  {"time", required_argument, پوچ, TIME_OPTION},
  {"time-style", required_argument, پوچ, TIME_STYLE_OPTION},
  {"zero", no_argument, پوچ, ZERO_OPTION},
  {"color", optional_argument, پوچ, COLOR_OPTION},
  {"hyperlink", optional_argument, پوچ, HYPERLINK_OPTION},
  {"block-size", required_argument, پوچ, BLOCK_SIZE_OPTION},
  {"context", no_argument, پوچ, 'Z'},
  {"author", no_argument, پوچ, AUTHOR_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {پوچ, 0, پوچ, 0}
};

ایستا نویسه ثابت *ثابت format_args[] =
{
  "verbose", "long", "commas", "horizontal", "across",
  "vertical", "single-column", پوچ
};
ایستا شمارش format ثابت format_types[] =
{
  long_format, long_format, with_commas, horizontal, horizontal,
  many_per_سطر, one_per_سطر
};
ARGMATCH_VERIFY (format_args, format_types);

ایستا نویسه ثابت *ثابت sort_args[] =
{
  "none", "size", "time", "version", "extension",
  "name", "width", پوچ
};
ایستا شمارش sort_type ثابت sort_types[] =
{
  sort_none, sort_size, sort_time, sort_version, sort_extension,
  sort_name, sort_width
};
ARGMATCH_VERIFY (sort_args, sort_types);

ایستا نویسه ثابت *ثابت time_args[] =
{
  "atime", "access", "use",
  "ctime", "status",
  "mtime", "modification",
  "birth", "creation",
  پوچ
};
ایستا شمارش time_type ثابت time_types[] =
{
  time_atime, time_atime, time_atime,
  time_ctime, time_ctime,
  time_mtime, time_mtime,
  time_btime, time_btime,
};
ARGMATCH_VERIFY (time_args, time_types);

ایستا نویسه ثابت *ثابت when_args[] =
{
  /* force and none are for compatibility with another color-ls version */
  "always", "yes", "force",
  "never", "no", "none",
  "auto", "tty", "if-tty", پوچ
};
ایستا شمارش when_type ثابت when_types[] =
{
  when_always, when_always, when_always,
  when_never, when_never, when_never,
  when_if_tty, when_if_tty, when_if_tty
};
ARGMATCH_VERIFY (when_args, when_types);

/* Information about filling a column.  */
ساختار column_info
{
  bool valid_len;
  size_t سطر_len;
  size_t *col_arr;
};

/* Array with information about column fullness.  */
ایستا ساختار column_info *column_info;

/* Maximum number of columns ever possible for this display.  */
ایستا size_t max_idx;

/* The minimum width of a non-final column is 3: 1 character for the name
   and 2 for the separating white space.  The final column is not padded.  */
شمارش { MIN_COLUMN_WIDTH = 3 };


/* This zero-based index is for the --dired option.  It is incremented
   for each byte of output generated by this program so that the beginning
   and ending indices (in that output) of every file name can be recorded
   and later output themselves.  */
ایستا off_t dired_pos;

ایستا تهی
dired_outbyte (نویسه c)
{
  dired_pos++;
  putchar (c);
}

/* Output the buffer S, of length S_LEN, and increment DIRED_POS by S_LEN.  */
ایستا تهی
dired_outbuf (نویسه ثابت *s, size_t s_len)
{
  dired_pos += s_len;
  fwrite (s, اندازه *s, s_len, stdout);
}

/* Output the string S, and increment DIRED_POS by its length.  */
ایستا تهی
dired_outstring (نویسه ثابت *s)
{
  dired_outbuf (s, strlen (s));
}

ایستا تهی
dired_indent (تهی)
{
  اگر (dired)
    dired_outstring ("  ");
}

/* With --dired, store pairs of beginning and ending indices of file names.  */
ایستا ساختار obstack dired_obstack;

/* With --dired, store pairs of beginning and ending indices of any
   directory names that appear as headers (just before 'total' line)
   for lists of directory entries.  Such directory names are seen when
   listing hierarchies using -R and when a directory is listed with at
   least one other command line argument.  */
ایستا ساختار obstack subdired_obstack;

/* Save the current index on the specified obstack, OBS.  */
ایستا تهی
افزودن_current_dired_pos (ساختار obstack *obs)
{
  اگر (dired)
    obstack_grow (obs, &dired_pos, اندازه dired_pos);
}

/* With -R, this stack is used to help detect directory cycles.
   The device/inode pairs on this stack mirror the pairs in the
   active_dir_set hash table.  */
ایستا ساختار obstack dev_ino_obstack;

/* Push a pair onto the device/inode stack.  */
ایستا تهی
dev_ino_افزودن (dev_t dev, ino_t ino)
{
  تهی *vdi;
  ساختار dev_ino *di;
  صحیح dev_ino_size = اندازه *di;
  obstack_blank (&dev_ino_obstack, dev_ino_size);
  vdi = obstack_next_free (&dev_ino_obstack);
  di = vdi;
  di--;
  di->st_dev = dev;
  di->st_ino = ino;
}

/* Pop a dev/ino struct off the global dev_ino_obstack
   and return that struct.  */
ایستا ساختار dev_ino
dev_ino_برداشت (تهی)
{
  تهی *vdi;
  ساختار dev_ino *di;
  صحیح dev_ino_size = اندازه *di;
  affirm (dev_ino_size <= obstack_object_size (&dev_ino_obstack));
  obstack_blank_fast (&dev_ino_obstack, -dev_ino_size);
  vdi = obstack_next_free (&dev_ino_obstack);
  di = vdi;
  بازگشت *di;
}

ایستا تهی
assert_matching_dev_ino (نویسه ثابت *name, ساختار dev_ino di)
{
  MAYBE_UNUSED ساختار stat sb;
  assure (0 <= stat (name, &sb));
  assure (sb.st_dev == di.st_dev);
  assure (sb.st_ino == di.st_ino);
}

ایستا نویسه eolbyte = '\n';

/* Write to standard output PREFIX, followed by the quoting style and
   a space-separated list of the integers stored in OS all on one line.  */

ایستا تهی
dired_dump_obstack (نویسه ثابت *prefix, ساختار obstack *os)
{
  size_t n_pos;

  n_pos = obstack_object_size (os) / اندازه (dired_pos);
  اگر (n_pos > 0)
    {
      off_t *pos = obstack_finish (os);
      fputs (prefix, stdout);
      برای (size_t i = 0; i < n_pos; i++)
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
ایستا ساختار timespec
get_stat_btime (ساختار stat ثابت *st)
{
  ساختار timespec btimespec;

#اگر HAVE_STATX && defined STATX_INO
  btimespec = get_stat_mtime (st);
#وگرنه
  btimespec = get_stat_birthtime (st);
#endif

  بازگشت btimespec;
}

#اگر HAVE_STATX && defined STATX_INO
ATTRIBUTE_PURE
ایستا unsigned صحیح
time_type_to_statx (تهی)
{
  انتخاب (time_type)
    {
    حالت time_ctime:
      بازگشت STATX_CTIME;
    حالت time_mtime:
      بازگشت STATX_MTIME;
    حالت time_atime:
      بازگشت STATX_ATIME;
    حالت time_btime:
      بازگشت STATX_BTIME;
    حالت time_numtypes: پیشفرض:
      unreachable ();
    }
    بازگشت 0;
}

ATTRIBUTE_PURE
ایستا unsigned صحیح
calc_req_mask (تهی)
{
  unsigned صحیح mask = STATX_MODE;

  اگر (print_inode)
    mask |= STATX_INO;

  اگر (print_block_size)
    mask |= STATX_BLOCKS;

  اگر (format == long_format) {
    mask |= STATX_NLINK | STATX_SIZE | time_type_to_statx ();
    اگر (print_owner || print_author)
      mask |= STATX_UID;
    اگر (print_group)
      mask |= STATX_GID;
  }

  انتخاب (sort_type)
    {
    حالت sort_none:
    حالت sort_name:
    حالت sort_version:
    حالت sort_extension:
    حالت sort_width:
      بشکن;
    حالت sort_time:
      mask |= time_type_to_statx ();
      بشکن;
    حالت sort_size:
      mask |= STATX_SIZE;
      بشکن;
    حالت sort_numtypes: پیشفرض:
      unreachable ();
    }

  بازگشت mask;
}

ایستا صحیح
do_statx (صحیح fd, نویسه ثابت *name, ساختار stat *st, صحیح flags,
          unsigned صحیح mask)
{
  ساختار statx stx;
  bool want_btime = mask & STATX_BTIME;
  صحیح ret = statx (fd, name, flags | AT_NO_AUTOMOUNT, mask, &stx);
  اگر (ret >= 0)
    {
      statx_to_stat (&stx, st);
      /* Since we only need one timestamp type,
         store birth time in st_mtim.  */
      اگر (want_btime)
        {
          اگر (stx.stx_mask & STATX_BTIME)
            st->st_mtim = statx_timestamp_to_timespec (stx.stx_btime);
          وگرنه
            st->st_mtim.tv_sec = st->st_mtim.tv_nsec = -1;
        }
    }

  بازگشت ret;
}

ایستا صحیح
do_stat (نویسه ثابت *name, ساختار stat *st)
{
  بازگشت do_statx (AT_FDCWD, name, st, 0, calc_req_mask ());
}

ایستا صحیح
do_lstat (نویسه ثابت *name, ساختار stat *st)
{
  بازگشت do_statx (AT_FDCWD, name, st, AT_SYMLINK_NOFOLLOW, calc_req_mask ());
}

ایستا صحیح
stat_for_mode (نویسه ثابت *name, ساختار stat *st)
{
  بازگشت do_statx (AT_FDCWD, name, st, 0, STATX_MODE);
}

/* dev+ino should be static, so no need to sync with backing store */
ایستا صحیح
stat_for_ino (نویسه ثابت *name, ساختار stat *st)
{
  بازگشت do_statx (AT_FDCWD, name, st, 0, STATX_INO);
}

ایستا صحیح
fstat_for_ino (صحیح fd, ساختار stat *st)
{
  بازگشت do_statx (fd, "", st, AT_EMPTY_PATH, STATX_INO);
}
#وگرنه
ایستا صحیح
do_stat (نویسه ثابت *name, ساختار stat *st)
{
  بازگشت stat (name, st);
}

ایستا صحیح
do_lstat (نویسه ثابت *name, ساختار stat *st)
{
  بازگشت lstat (name, st);
}

ایستا صحیح
stat_for_mode (نویسه ثابت *name, ساختار stat *st)
{
  بازگشت stat (name, st);
}

ایستا صحیح
stat_for_ino (نویسه ثابت *name, ساختار stat *st)
{
  بازگشت stat (name, st);
}

ایستا صحیح
fstat_for_ino (صحیح fd, ساختار stat *st)
{
  بازگشت fstat (fd, st);
}
#endif

/* Return the address of the first plain %b spec in FMT, or NULL if
   there is no such spec.  %5b etc. do not match, so that user
   widths/flags are honored.  */

ATTRIBUTE_PURE
ایستا نویسه ثابت *
first_percent_b (نویسه ثابت *fmt)
{
  برای (; *fmt; fmt++)
    اگر (fmt[0] == '%')
      انتخاب (fmt[1])
        {
        حالت 'b': بازگشت fmt;
        حالت '%': fmt++; بشکن;
        }
  بازگشت پوچ;
}

ایستا نویسه RFC3986[256];
ایستا تهی
file_escape_init (تهی)
{
  برای (صحیح i = 0; i < 256; i++)
    RFC3986[i] |= c_isalnum (i) || i == '~' || i == '-' || i == '.' || i == '_';
}

شمارش { MBSWIDTH_FLAGS = MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE };

/* Read the abbreviated month names from the locale, to align them
   and to determine the max width of the field and to truncate names
   greater than our max allowed.
   Note even though this handles multibyte locales correctly
   it's not restricted to them as single byte locales can have
   variable width abbreviated months and also precomputing/caching
   the names was seen to increase the performance of ls significantly.  */

/* abformat[RECENT][MON] is the format to use for timestamps with
   recentness RECENT and month MON.  */
شمارش { ABFORMAT_SIZE = 128 };
ایستا نویسه abformat[2][12][ABFORMAT_SIZE];
/* True if precomputed formats should be used.  This can be false if
   nl_langinfo fails, if a format or month abbreviation is unusually
   long, or if a month abbreviation contains '%'.  */
ایستا bool use_abformat;

/* Store into ABMON the abbreviated month names, suitably aligned.
   Return true if successful.  */

ایستا bool
abmon_init (نویسه abmon[12][ABFORMAT_SIZE])
{
#ifndef HAVE_NL_LANGINFO
  بازگشت false;
#وگرنه
  صحیح max_mon_width = 0;
  صحیح mon_width[12];
  صحیح mon_len[12];

  برای (صحیح i = 0; i < 12; i++)
    {
      نویسه ثابت *abbr = nl_langinfo (ABMON_1 + i);
      mon_len[i] = strnlen (abbr, ABFORMAT_SIZE);
      اگر (mon_len[i] == ABFORMAT_SIZE)
        بازگشت false;
      اگر (strchr (abbr, '%'))
        بازگشت false;
      mon_width[i] = mbswidth (strcpy (abmon[i], abbr), MBSWIDTH_FLAGS);
      اگر (mon_width[i] < 0)
        بازگشت false;
      max_mon_width = MAX (max_mon_width, mon_width[i]);
    }

  برای (صحیح i = 0; i < 12; i++)
    {
      صحیح fill = max_mon_width - mon_width[i];
      اگر (ABFORMAT_SIZE - mon_len[i] <= fill)
        بازگشت false;
      bool align_چپ = !c_isdigit (abmon[i][0]);
      صحیح fill_offset;
      اگر (align_چپ)
        fill_offset = mon_len[i];
      وگرنه
        {
          memmove (abmon[i] + fill, abmon[i], mon_len[i]);
          fill_offset = 0;
        }
      memset (abmon[i] + fill_offset, ' ', fill);
      abmon[i][mon_len[i] + fill] = '\0';
    }

  بازگشت true;
#endif
}

/* Initialize ABFORMAT and USE_ABFORMAT.  */

ایستا تهی
abformat_init (تهی)
{
  نویسه ثابت *pb[2];
  برای (صحیح recent = 0; recent < 2; recent++)
    pb[recent] = first_percent_b (long_time_format[recent]);
  اگر (! (pb[0] || pb[1]))
    بازگشت;

  نویسه abmon[12][ABFORMAT_SIZE];
  اگر (! abmon_init (abmon))
    بازگشت;

  برای (صحیح recent = 0; recent < 2; recent++)
    {
      نویسه ثابت *fmt = long_time_format[recent];
      برای (صحیح i = 0; i < 12; i++)
        {
          نویسه *nfmt = abformat[recent][i];
          صحیح nbytes;

          اگر (! pb[recent])
            nbytes = snprintf (nfmt, ABFORMAT_SIZE, "%s", fmt);
          وگرنه
            {
              اگر (! (pb[recent] - fmt <= MIN (ABFORMAT_SIZE, INT_MAX)))
                بازگشت;
              صحیح prefix_len = pb[recent] - fmt;
              nbytes = snprintf (nfmt, ABFORMAT_SIZE, "%.*s%s%s",
                                 prefix_len, fmt, abmon[i], pb[recent] + 2);
            }

          اگر (! (0 <= nbytes && nbytes < ABFORMAT_SIZE))
            بازگشت;
        }
    }

  use_abformat = true;
}

ایستا size_t
dev_ino_hash (تهی ثابت *x, size_t table_size)
{
  ساختار dev_ino ثابت *p = x;
  بازگشت (uintmax_t) {p->st_ino} % table_size;
}

ایستا bool
dev_ino_compare (تهی ثابت *x, تهی ثابت *y)
{
  ساختار dev_ino ثابت *a = x;
  ساختار dev_ino ثابت *b = y;
  بازگشت PSAME_INODE (a, b);
}

ایستا تهی
dev_ino_free (تهی *x)
{
  آزاد (x);
}

/* Add the device/inode pair (P->st_dev/P->st_ino) to the set of
   active directories.  Return true if there is already a matching
   entry in the table.  */

ایستا bool
visit_dir (dev_t dev, ino_t ino)
{
  ساختار dev_ino *ent;
  ساختار dev_ino *ent_from_table;
  bool found_match;

  ent = xmalloc (اندازه *ent);
  ent->st_ino = ino;
  ent->st_dev = dev;

  /* Attempt to insert this entry into the table.  */
  ent_from_table = hash_درج (active_dir_set, ent);

  اگر (ent_from_table == پوچ)
    {
      /* Insertion failed due to lack of memory.  */
      xalloc_die ();
    }

  found_match = (ent_from_table != ent);

  اگر (found_match)
    {
      /* ent was not inserted, so free it.  */
      آزاد (ent);
    }

  بازگشت found_match;
}

ایستا تهی
free_pending_ent (ساختار pending *p)
{
  آزاد (p->name);
  آزاد (p->realname);
  آزاد (p);
}

ایستا bool
is_colored (شمارش indicator_no type)
{
  /* Return true unless the string is "", "0" or "00"; try to be efficient.  */
  size_t len = color_indicator[type].len;
  اگر (len == 0)
    بازگشت false;
  اگر (2 < len)
    بازگشت true;
  نویسه ثابت *s = color_indicator[type].string;
  بازگشت (s[0] != '0') | (s[len - 1] != '0');
}

ایستا تهی
restore_default_color (تهی)
{
  put_indicator (&color_indicator[C_چپ]);
  put_indicator (&color_indicator[C_راست]);
}

ایستا تهی
set_normal_color (تهی)
{
  اگر (print_with_color && is_colored (C_NORM))
    {
      put_indicator (&color_indicator[C_چپ]);
      put_indicator (&color_indicator[C_NORM]);
      put_indicator (&color_indicator[C_راست]);
    }
}

/* An ordinary signal was received; arrange for the program to exit.  */

ایستا تهی
sighandler (صحیح sig)
{
  اگر (! interrupt_signal)
    interrupt_signal = sig;
}

/* A SIGTSTP was received; arrange for the program to suspend itself.  */

ایستا تهی
stophandler (MAYBE_UNUSED صحیح sig)
{
  اگر (! interrupt_signal)
    stop_signal_شمار++;
}

/* Process any pending signals.  If signals are caught, this function
   should be called periodically.  Ideally there should never be an
   unbounded amount of time when signals are not being processed.
   Signal handling can restore the default colors, so callers must
   immediately change colors after invoking this function.  */

ایستا تهی
process_signals (تهی)
{
  وقتی (interrupt_signal || stop_signal_شمار)
    {
      صحیح sig;
      صحیح stops;
      sigset_t oldset;

      اگر (used_color)
        restore_default_color ();
      fflush (stdout);

      sigprocmask (SIG_BLOCK, &caught_signals, &oldset);

      /* Reload interrupt_signal and stop_signal_count, in case a new
         signal was handled before sigprocmask took effect.  */
      sig = interrupt_signal;
      stops = stop_signal_شمار;

      /* SIGTSTP is special, since the application can receive that signal
         more than once.  In this case, don't set the signal handler to the
         default.  Instead, just raise the uncatchable SIGSTOP.  */
      اگر (stops)
        {
          stop_signal_شمار = stops - 1;
          sig = SIGSTOP;
        }
      وگرنه
        سیگنال (sig, SIG_DFL);

      /* Exit or suspend the program.  */
      بالاببر (sig);
      sigprocmask (SIG_SETMASK, &oldset, پوچ);

      /* If execution reaches here, then the program has been
         continued (after being suspended).  */
    }
}

/* Setup signal handlers if INIT is true,
   otherwise restore to the default.  */

ایستا تهی
signal_setup (bool init)
{
  /* The signals that are trapped, and the number of such signals.  */
  ایستا صحیح ثابت stop_sig[] =
    {
      /* This one is handled specially.  */
      SIGTSTP,
    };

  شمارش { nsigs = countof (term_sig), nstop = countof (stop_sig) };

  اگر (init)
    {
      ساختار sigaction act;

      sigemptyset (&caught_signals);
      برای (صحیح j = 0; j < nsigs + nstop; j++)
        {
          صحیح sig = j < nsigs ? term_sig[j]: stop_sig[j - nsigs];
          sigaction (sig, پوچ, &act);
          اگر (act.sa_handler != SIG_IGN)
            sigaddset (&caught_signals, sig);
        }

      act.sa_mask = caught_signals;
      act.sa_flags = SA_RESTART;

      برای (صحیح j = 0; j < nsigs + nstop; j++)
        {
          صحیح sig = j < nsigs ? term_sig[j]: stop_sig[j - nsigs];
          اگر (sigismember (&caught_signals, sig))
            {
              act.sa_handler = j < nsigs ? sighandler : stophandler;
              sigaction (sig, &act, پوچ);
            }
        }
    }
  وگرنه /* restore.  */
    {
      برای (صحیح j = 0; j < nsigs + nstop; j++)
        {
          صحیح sig = j < nsigs ? term_sig[j]: stop_sig[j - nsigs];
          اگر (sigismember (&caught_signals, sig))
            سیگنال (sig, SIG_DFL);
        }
    }
}

ایستا تهی
signal_init (تهی)
{
  signal_setup (true);
}

ایستا تهی
signal_restore (تهی)
{
  signal_setup (false);
}

صحیح
اصلی (صحیح argc, نویسه **argv)
{
  صحیح i;
  ساختار pending *thispend;
  صحیح n_files;

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
  pending_dirs = پوچ;

  current_time.tv_sec = TYPE_MINIMUM (time_t);
  current_time.tv_nsec = -1;

  i = decode_switches (argc, argv);

  اگر (print_with_color)
    parse_ls_color ();

  /* Test print_with_color again, because the call to parse_ls_color
     may have just reset it -- e.g., if LS_COLORS is invalid.  */

  اگر (print_with_color)
    {
      /* Don't use TAB characters in output.  Some terminal
         emulators can't handle the combination of tabs and
         color codes on the same line.  */
      tabsize = 0;
    }

  اگر (directories_first)
    check_symlink_mode = true;
  وگرنه اگر (print_with_color)
    {
      /* Avoid following symbolic links when possible.  */
      اگر (is_colored (C_ORPHAN)
          || (is_colored (C_EXEC) && color_symlink_as_referent)
          || (is_colored (C_MISSING) && format == long_format))
        check_symlink_mode = true;
    }

  اگر (dereference == DEREF_UNDEFINED)
    dereference = ((immediate_dirs
                    || indicator_style == classify
                    || format == long_format)
                   ? DEREF_NEVER
                   : DEREF_COMMAND_سطر_SYMLINK_TO_DIR);

  /* When using -R, initialize a data structure we'll use to
     detect any directory cycles.  */
  اگر (recursive)
    {
      active_dir_set = hash_initialize (INITIAL_TABLE_SIZE, پوچ,
                                        dev_ino_hash,
                                        dev_ino_compare,
                                        dev_ino_free);
      اگر (active_dir_set == پوچ)
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

  اگر (dired)
    {
      obstack_init (&dired_obstack);
      obstack_init (&subdired_obstack);
    }

  اگر (print_hyperlink)
    {
      file_escape_init ();

      hostname = xgethostname ();
      /* The hostname is generally ignored,
         so ignore failures obtaining it.  */
      اگر (! hostname)
        hostname = "";
    }

  cwd_n_alloc = 100;
  cwd_file = xmalloc (cwd_n_alloc * اندازه *cwd_file);
  cwd_n_used = 0;

  clear_files ();

  n_files = argc - i;

  اگر (n_files <= 0)
    {
      اگر (immediate_dirs)
        gobble_file (".", directory, NOT_AN_INODE_عدد, true, پوچ);
      وگرنه
        queue_directory (".", پوچ, true);
    }
  وگرنه
    انجامبده
      gobble_file (argv[i++], unknown, NOT_AN_INODE_عدد, true, پوچ);
    وقتی (i < argc);

  اگر (cwd_n_used)
    {
      sort_files ();
      اگر (!immediate_dirs)
        extract_dirs_from_files (پوچ, true);
      /* 'cwd_n_used' might be zero now.  */
    }

  /* In the following if/else blocks, it is sufficient to test 'pending_dirs'
     (and not pending_dirs->name) because there may be no markers in the queue
     at this point.  A marker may be enqueued when extract_dirs_from_files is
     called with a non-empty string or via print_dir.  */
  اگر (cwd_n_used)
    {
      print_current_files ();
      اگر (pending_dirs)
        dired_outbyte ('\n');
    }
  وگرنه اگر (n_files <= 1 && pending_dirs && !pending_dirs->next)
    print_dir_name = false;

  وقتی (pending_dirs)
    {
      thispend = pending_dirs;
      pending_dirs = pending_dirs->next;

      اگر (LOOP_DETECT)
        {
          اگر (thispend->name == پوچ)
            {
              /* thispend->name == NULL means this is a marker entry
                 indicating we've finished processing the directory.
                 Use its dev/ino numbers to remove the corresponding
                 entry from the active_dir_set hash table.  */
              ساختار dev_ino di = dev_ino_برداشت ();
              ساختار dev_ino *found = hash_remove (active_dir_set, &di);
              اگر (false)
                assert_matching_dev_ino (thispend->realname, di);
              affirm (found);
              dev_ino_free (found);
              free_pending_ent (thispend);
              ادامه;
            }
        }

      print_dir (thispend->name, thispend->realname,
                 thispend->command_سطر_arg);

      free_pending_ent (thispend);
      print_dir_name = true;
    }

  اگر (print_with_color && used_color)
    {

      /* Skip the restore when it would be a no-op, i.e.,
         when left is "\033[" and right is "m".  */
      اگر (!(color_indicator[C_چپ].len == 2
            && memeq (color_indicator[C_چپ].string, "\033[", 2)
            && color_indicator[C_راست].len == 1
            && color_indicator[C_راست].string[0] == 'm'))
        restore_default_color ();

      fflush (stdout);

      signal_restore ();

      /* Act on any signals that arrived before the default was restored.
         This can process signals out of order, but there doesn't seem to
         be an easy way to do them in order, and the order isn't that
         important anyway.  */
      برای (صحیح j = stop_signal_شمار; j; j--)
        بالاببر (SIGSTOP);
      صحیح j = interrupt_signal;
      اگر (j)
        بالاببر (j);
    }

  اگر (dired)
    {
      /* No need to free these since we're about to exit.  */
      dired_dump_obstack ("//DIRED//", &dired_obstack);
      dired_dump_obstack ("//SUBDIRED//", &subdired_obstack);
      printf ("//DIRED-OPTIONS// --quoting-style=%s\n",
              quoting_style_args[get_quoting_style (filename_quoting_options)]);
    }

  اگر (LOOP_DETECT)
    {
      assure (hash_get_n_entries (active_dir_set) == 0);
      hash_free (active_dir_set);
    }

  بازگشت exit_status;
}

/* Return the line length indicated by the value given by SPEC, or -1
   if unsuccessful.  0 means no limit on line length.  */

ایستا ptrdiff_t
decode_سطر_length (نویسه ثابت *spec)
{
  uintmax_t val;

  /* Treat too-large values as if they were 0, which is
     effectively infinity.  */
  انتخاب (xstrtoumax (spec, پوچ, 0, &val, ""))
    {
    حالت LONGINT_OK:
      بازگشت val <= MIN (PTRDIFF_MAX, SIZE_MAX) ? val : 0;

    حالت LONGINT_OVERFLOW:
      بازگشت 0;

    حالت LONGINT_INVALID:
    حالت LONGINT_INVALID_SUFFIX_CHAR:
    حالت LONGINT_INVALID_SUFFIX_CHAR_WITH_OVERFLOW:
      بازگشت -1;

    پیشفرض:
      unreachable ();
    }
}

/* Return true if standard output is a tty, caching the result.  */

ایستا bool
stdout_isatty (تهی)
{
  ایستا signed نویسه out_tty = -1;
  اگر (out_tty < 0)
    out_tty = isatty (STDOUT_FILENO);
  assume (out_tty == 0 || out_tty == 1);
  بازگشت out_tty;
}

/* Set all the option flags according to the switches specified.
   Return the index of the first non-option argument.  */

ایستا صحیح
decode_switches (صحیح argc, نویسه **argv)
{
  نویسه *time_style_option = پوچ;

  /* These variables are false or -1 unless a switch says otherwise.  */
  bool kibibytes_specified = false;
  صحیح format_opt = -1;
  صحیح hide_control_chars_opt = -1;
  صحیح quoting_style_opt = -1;
  صحیح sort_opt = -1;
  ptrdiff_t tabsize_opt = -1;
  ptrdiff_t width_opt = -1;

  وقتی (true)
    {
      صحیح oi = -1;
      صحیح c = getopt_long (argc, argv,
                           "abcdfghiklmnopqrstuvw:xABCDFGHI:LNQRST:UXZ1",
                           long_options, &oi);
      اگر (c == -1)
        بشکن;

      انتخاب (c)
        {
        حالت 'a':
          ignore_mode = IGNORE_MINIMAL;
          بشکن;

        حالت 'b':
          quoting_style_opt = escape_quoting_style;
          بشکن;

        حالت 'c':
          time_type = time_ctime;
          explicit_time = true;
          بشکن;

        حالت 'd':
          immediate_dirs = true;
          بشکن;

        حالت 'f':
          ignore_mode = IGNORE_MINIMAL; /* enable -a */
          sort_opt = sort_none;         /* enable -U */
          بشکن;

        حالت FILE_TYPE_INDICATOR_OPTION: /* --file-type */
          indicator_style = file_type;
          بشکن;

        حالت 'g':
          format_opt = long_format;
          print_owner = false;
          بشکن;

        حالت 'h':
          file_human_output_opts = human_output_opts =
            human_autoscale | human_SI | human_base_1024;
          file_output_block_size = output_block_size = 1;
          بشکن;

        حالت 'i':
          print_inode = true;
          بشکن;

        حالت 'k':
          kibibytes_specified = true;
          بشکن;

        حالت 'l':
          format_opt = long_format;
          بشکن;

        حالت 'm':
          format_opt = with_commas;
          بشکن;

        حالت 'n':
          numeric_ids = true;
          format_opt = long_format;
          بشکن;

        حالت 'o':  /* Just like -l, but don't display group info.  */
          format_opt = long_format;
          print_group = false;
          بشکن;

        حالت 'p':
          indicator_style = slash;
          بشکن;

        حالت 'q':
          hide_control_chars_opt = true;
          بشکن;

        حالت 'r':
          sort_reverse = true;
          بشکن;

        حالت 's':
          print_block_size = true;
          بشکن;

        حالت 't':
          sort_opt = sort_time;
          بشکن;

        حالت 'u':
          time_type = time_atime;
          explicit_time = true;
          بشکن;

        حالت 'v':
          sort_opt = sort_version;
          بشکن;

        حالت 'w':
          width_opt = decode_سطر_length (optarg);
          اگر (width_opt < 0)
            error (LS_FAILURE, 0, "%s: %s", _("invalid line width"),
                   quote (optarg));
          بشکن;

        حالت 'x':
          format_opt = horizontal;
          بشکن;

        حالت 'A':
          ignore_mode = IGNORE_DOT_AND_DOTDOT;
          بشکن;

        حالت 'B':
          add_ignore_pattern ("*~");
          add_ignore_pattern (".*~");
          بشکن;

        حالت 'C':
          format_opt = many_per_سطر;
          بشکن;

        حالت 'D':
          format_opt = long_format;
          print_hyperlink = false;
          dired = true;
          بشکن;

        حالت 'F':
          {
            صحیح i;
            اگر (optarg)
              i = XARGMATCH ("--classify", optarg, when_args, when_types);
            وگرنه
              /* Using --classify with no argument is equivalent to using
                 --classify=always.  */
              i = when_always;

            اگر (i == when_always || (i == when_if_tty && stdout_isatty ()))
              indicator_style = classify;
            بشکن;
          }

        حالت 'G':		/* inhibit display of group info */
          print_group = false;
          بشکن;

        حالت 'H':
          dereference = DEREF_COMMAND_سطر_ARGUMENTS;
          بشکن;

        حالت DEREFERENCE_COMMAND_سطر_SYMLINK_TO_DIR_OPTION:
          dereference = DEREF_COMMAND_سطر_SYMLINK_TO_DIR;
          بشکن;

        حالت 'I':
          add_ignore_pattern (optarg);
          بشکن;

        حالت 'L':
          dereference = DEREF_ALWAYS;
          بشکن;

        حالت 'N':
          quoting_style_opt = literal_quoting_style;
          بشکن;

        حالت 'Q':
          quoting_style_opt = c_quoting_style;
          بشکن;

        حالت 'R':
          recursive = true;
          بشکن;

        حالت 'S':
          sort_opt = sort_size;
          بشکن;

        حالت 'T':
          tabsize_opt = xnumtoumax (optarg, 0, 0, MIN (PTRDIFF_MAX, SIZE_MAX),
                                    "", _("invalid tab size"), LS_FAILURE, 0);
          بشکن;

        حالت 'U':
          sort_opt = sort_none;
          بشکن;

        حالت 'X':
          sort_opt = sort_extension;
          بشکن;

        حالت '1':
          /* -1 has no effect after -l.  */
          اگر (format_opt != long_format)
            format_opt = one_per_سطر;
          بشکن;

        حالت AUTHOR_OPTION:
          print_author = true;
          بشکن;

        حالت HIDE_OPTION:
          {
            ساختار ignore_pattern *hide = xmalloc (اندازه *hide);
            hide->pattern = optarg;
            hide->next = hide_patterns;
            hide_patterns = hide;
          }
          بشکن;

        حالت SORT_OPTION:
          sort_opt = XARGMATCH ("--sort", optarg, sort_args, sort_types);
          بشکن;

        حالت GROUP_DIRECTORIES_FIRST_OPTION:
          directories_first = true;
          بشکن;

        حالت TIME_OPTION:
          time_type = XARGMATCH ("--time", optarg, time_args, time_types);
          explicit_time = true;
          بشکن;

        حالت FORMAT_OPTION:
          format_opt = XARGMATCH ("--format", optarg, format_args,
                                  format_types);
          بشکن;

        حالت FULL_TIME_OPTION:
          format_opt = long_format;
          time_style_option = (نویسه *) "full-iso";
          بشکن;

        حالت COLOR_OPTION:
          {
            صحیح i;
            اگر (optarg)
              i = XARGMATCH ("--color", optarg, when_args, when_types);
            وگرنه
              /* Using --color with no argument is equivalent to using
                 --color=always.  */
              i = when_always;

            print_with_color = (i == when_always
                                || (i == when_if_tty && stdout_isatty ()));
            بشکن;
          }

        حالت HYPERLINK_OPTION:
          {
            صحیح i;
            اگر (optarg)
              i = XARGMATCH ("--hyperlink", optarg, when_args, when_types);
            وگرنه
              /* Using --hyperlink with no argument is equivalent to using
                 --hyperlink=always.  */
              i = when_always;

            print_hyperlink = (i == when_always
                               || (i == when_if_tty && stdout_isatty ()));
            بشکن;
          }

        حالت INDICATOR_STYLE_OPTION:
          indicator_style = XARGMATCH ("--indicator-style", optarg,
                                       indicator_style_args,
                                       indicator_style_types);
          بشکن;

        حالت QUOTING_STYLE_OPTION:
          quoting_style_opt = XARGMATCH ("--quoting-style", optarg,
                                         quoting_style_args,
                                         quoting_style_vals);
          بشکن;

        حالت TIME_STYLE_OPTION:
          time_style_option = optarg;
          بشکن;

        حالت SHOW_CONTROL_CHARS_OPTION:
          hide_control_chars_opt = false;
          بشکن;

        حالت BLOCK_SIZE_OPTION:
          {
            شمارش strtol_error e = human_options (optarg, &human_output_opts,
                                                 &output_block_size);
            اگر (e != LONGINT_OK)
              xstrtol_fatal (e, oi, 0, long_options, optarg);
            file_human_output_opts = human_output_opts;
            file_output_block_size = output_block_size;
          }
          بشکن;

        حالت SI_OPTION:
          file_human_output_opts = human_output_opts =
            human_autoscale | human_SI;
          file_output_block_size = output_block_size = 1;
          بشکن;

        حالت 'Z':
          print_scontext = true;
          بشکن;

        حالت ZERO_OPTION:
          eolbyte = 0;
          hide_control_chars_opt = false;
          اگر (format_opt != long_format)
            format_opt = one_per_سطر;
          print_with_color = false;
          quoting_style_opt = literal_quoting_style;
          بشکن;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        پیشفرض:
          usage (LS_FAILURE);
        }
    }

  اگر (! output_block_size)
    {
      نویسه ثابت *ls_block_size = getenv ("LS_BLOCK_SIZE");
      human_options (ls_block_size,
                     &human_output_opts, &output_block_size);
      اگر (ls_block_size || getenv ("BLOCK_SIZE"))
        {
          file_human_output_opts = human_output_opts;
          file_output_block_size = output_block_size;
        }
      اگر (kibibytes_specified)
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
  اگر (format == many_per_سطر || format == horizontal || format == with_commas
      || print_with_color)
    {
#ifdef TIOCGWINSZ
      اگر (linelen < 0)
        {
          ساختار winsize ws;
          اگر (stdout_isatty ()
              && 0 <= ioctl (STDOUT_FILENO, TIOCGWINSZ, &ws)
              && 0 < ws.ws_col)
            linelen = ws.ws_col <= MIN (PTRDIFF_MAX, SIZE_MAX) ? ws.ws_col : 0;
        }
#endif
      اگر (linelen < 0)
        {
          نویسه ثابت *p = getenv ("COLUMNS");
          اگر (p && *p)
            {
              linelen = decode_سطر_length (p);
              اگر (linelen < 0)
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

  اگر (format == many_per_سطر || format == horizontal || format == with_commas)
    {
      اگر (0 <= tabsize_opt)
        tabsize = tabsize_opt;
      وگرنه
        {
          tabsize = 8;
          نویسه ثابت *p = getenv ("TABSIZE");
          اگر (p)
            {
              uintmax_t tmp;
              اگر (xstrtoumax (p, پوچ, 0, &tmp, "") == LONGINT_OK
                  && tmp <= SIZE_MAX)
                tabsize = tmp;
              وگرنه
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

  صحیح qs = quoting_style_opt;
  اگر (qs < 0)
    qs = getenv_quoting_style ();
  اگر (qs < 0)
    qs = (ls_mode == LS_LS
          ? (stdout_isatty () ? shell_escape_quoting_style : -1)
          : escape_quoting_style);
  اگر (0 <= qs)
    set_quoting_style (پوچ, qs);
  qs = get_quoting_style (پوچ);
  align_variable_outer_quotes
    = ((format == long_format
        || ((format == many_per_سطر || format == horizontal) && سطر_length))
       && (qs == shell_quoting_style
           || qs == shell_escape_quoting_style
           || qs == c_maybe_quoting_style));
  filename_quoting_options = clone_quoting_options (پوچ);
  اگر (qs == escape_quoting_style)
    set_char_quoting (filename_quoting_options, ' ', 1);
  اگر (file_type <= indicator_style)
    {
      برای (نویسه ثابت *p = &"*=>@|"[indicator_style - file_type]; *p; p++)
        set_char_quoting (filename_quoting_options, *p, 1);
    }
  اگر (format == with_commas
      && (qs == shell_quoting_style
          || qs == shell_escape_quoting_style
          || qs == c_maybe_quoting_style
          || qs == escape_quoting_style))
    set_char_quoting (filename_quoting_options, ',', 1);

  dirname_quoting_options = clone_quoting_options (پوچ);
  set_char_quoting (dirname_quoting_options, ':', 1);

  /* --dired implies --format=long (-l) and sans --hyperlink.
     So ignore it if those overridden.  */
  dired &= (format == long_format) & !print_hyperlink;

  اگر (eolbyte < dired)
    error (LS_FAILURE, 0, _("--dired and --zero are incompatible"));

  /* If a time type is explicitly specified (with -c, -u, or --time=)
     and we're not showing a time (-l not specified), then sort by that time,
     rather than by name.  Note this behavior is unspecified by POSIX.  */

  sort_type = (0 <= sort_opt ? sort_opt
               : (format != long_format && explicit_time)
               ? sort_time : sort_name);

  اگر (format == long_format)
    {
      نویسه *envstyle = پوچ;
      نویسه *style = time_style_option;
      اگر (! style)
        style = envstyle = getenv ("TIME_STYLE");

      اگر (! style)
        style = (نویسه *) "locale";
      وگرنه
        {
          ایستا نویسه ثابت posix_prefix[] = "posix-";
          وقتی (STREQ_LEN (style, posix_prefix, اندازه posix_prefix - 1))
            {
              اگر (! hard_locale (LC_TIME))
                بازگشت optind;
              style += اندازه posix_prefix - 1;
            }
        }

      اگر (*style == '+')
        {
          نویسه *p0 = style + 1;
          نویسه *p0nl = strchr (p0, '\n');
          نویسه ثابت *p1 = p0;
          اگر (p0nl)
            {
              p1 = p0nl + 1;
              اگر (strchr (p1, '\n'))
                error (LS_FAILURE, 0, _("invalid time style format %s"),
                       quote (p0));
              اگر (envstyle)
                p0 = xmemdup0 (p0, p0nl - p0);
              وگرنه
                *p0nl = '\0';
            }
          long_time_format[0] = p0;
          long_time_format[1] = p1;
        }
      وگرنه
        {
          انتخاب (x_timestyle_match (style, /*allow_posix=*/ true,
                                     time_style_args,
                                     (نویسه ثابت *) time_style_types,
                                     اندازه (*time_style_types), LS_FAILURE))
            {
            حالت full_iso_time_style:
              long_time_format[0] = long_time_format[1] =
                "%Y-%m-%d %H:%M:%S.%N %z";
              بشکن;

            حالت long_iso_time_style:
              long_time_format[0] = long_time_format[1] = "%Y-%m-%d %H:%M";
              بشکن;

            حالت iso_time_style:
              long_time_format[0] = "%Y-%m-%d ";
              long_time_format[1] = "%m-%d %H:%M";
              بشکن;

            حالت locale_time_style:
              اگر (hard_locale (LC_TIME))
                {
                  برای (صحیح i = 0; i < 2; i++)
                    long_time_format[i] =
                      dcgettext (پوچ, long_time_format[i], LC_TIME);
                }
            }
        }

      abformat_init ();
    }

  بازگشت optind;
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

ایستا bool
get_funky_string (نویسه **dest, نویسه ثابت **src, bool equals_end,
                  size_t *output_شمار)
{
  نویسه رقم;			/* For numerical codes */
  size_t شمار;			/* Something to count with */
  شمارش {
    ST_GND, ST_BACKSLASH, ST_OCTAL, ST_HEX, ST_CARET, ST_END, ST_ERROR
  } state;
  نویسه ثابت *p;
  نویسه *q;

  p = *src;			/* We don't want to double-indirect */
  q = *dest;			/* the whole darn time.  */

  شمار = 0;			/* No characters counted in yet.  */
  رقم = 0;

  state = ST_GND;		/* Start in ground state.  */
  وقتی (state < ST_END)
    {
      انتخاب (state)
        {
        حالت ST_GND:		/* Ground state (no escapes) */
          انتخاب (*p)
            {
            حالت ':':
            حالت '\0':
              state = ST_END;	/* End of string */
              بشکن;
            حالت '\\':
              state = ST_BACKSLASH; /* Backslash escape sequence */
              ++p;
              بشکن;
            حالت '^':
              state = ST_CARET; /* Caret escape */
              ++p;
              بشکن;
            حالت '=':
              اگر (equals_end)
                {
                  state = ST_END; /* End */
                  بشکن;
                }
              FALLTHROUGH;
            پیشفرض:
              *(q++) = *(p++);
              ++شمار;
              بشکن;
            }
          بشکن;

        حالت ST_BACKSLASH:	/* Backslash escaped character */
          انتخاب (*p)
            {
            حالت '0':
            حالت '1':
            حالت '2':
            حالت '3':
            حالت '4':
            حالت '5':
            حالت '6':
            حالت '7':
              state = ST_OCTAL;	/* Octal sequence */
              رقم = *p - '0';
              بشکن;
            حالت 'x':
            حالت 'X':
              state = ST_HEX;	/* Hex sequence */
              رقم = 0;
              بشکن;
            حالت 'a':		/* Bell */
              رقم = '\a';
              بشکن;
            حالت 'b':		/* Backspace */
              رقم = '\b';
              بشکن;
            حالت 'e':		/* Escape */
              رقم = 27;
              بشکن;
            حالت 'f':		/* Form feed */
              رقم = '\f';
              بشکن;
            حالت 'n':		/* Newline */
              رقم = '\n';
              بشکن;
            حالت 'r':		/* Carriage return */
              رقم = '\r';
              بشکن;
            حالت 't':		/* Tab */
              رقم = '\t';
              بشکن;
            حالت 'v':		/* Vtab */
              رقم = '\v';
              بشکن;
            حالت '?':		/* Delete */
              رقم = 127;
              بشکن;
            حالت '_':		/* Space */
              رقم = ' ';
              بشکن;
            حالت '\0':		/* End of string */
              state = ST_ERROR;	/* Error! */
              بشکن;
            پیشفرض:		/* Escaped character like \ ^ : = */
              رقم = *p;
              بشکن;
            }
          اگر (state == ST_BACKSLASH)
            {
              *(q++) = رقم;
              ++شمار;
              state = ST_GND;
            }
          ++p;
          بشکن;

        حالت ST_OCTAL:		/* Octal sequence */
          اگر (*p < '0' || *p > '7')
            {
              *(q++) = رقم;
              ++شمار;
              state = ST_GND;
            }
          وگرنه
            رقم = (رقم << 3) + (*(p++) - '0');
          بشکن;

        حالت ST_HEX:		/* Hex sequence */
          انتخاب (*p)
            {
            حالت '0':
            حالت '1':
            حالت '2':
            حالت '3':
            حالت '4':
            حالت '5':
            حالت '6':
            حالت '7':
            حالت '8':
            حالت '9':
              رقم = (رقم << 4) + (*(p++) - '0');
              بشکن;
            حالت 'a':
            حالت 'b':
            حالت 'c':
            حالت 'd':
            حالت 'e':
            حالت 'f':
              رقم = (رقم << 4) + (*(p++) - 'a') + 10;
              بشکن;
            حالت 'A':
            حالت 'B':
            حالت 'C':
            حالت 'D':
            حالت 'E':
            حالت 'F':
              رقم = (رقم << 4) + (*(p++) - 'A') + 10;
              بشکن;
            پیشفرض:
              *(q++) = رقم;
              ++شمار;
              state = ST_GND;
              بشکن;
            }
          بشکن;

        حالت ST_CARET:		/* Caret escape */
          state = ST_GND;	/* Should be the next state... */
          اگر (*p >= '@' && *p <= '~')
            {
              *(q++) = *(p++) & 037;
              ++شمار;
            }
          وگرنه اگر (*p == '?')
            {
              *(q++) = 127;
              ++شمار;
            }
          وگرنه
            state = ST_ERROR;
          بشکن;

        حالت ST_END: حالت ST_ERROR: پیشفرض:
          affirm (false);
        }
    }

  *dest = q;
  *src = p;
  *output_شمار = شمار;

  بازگشت state != ST_ERROR;
}

شمارش parse_state
  {
    PS_START = 1,
    PS_2,
    PS_3,
    PS_4,
    PS_DONE,
    PS_FAIL
  };


/* Check if the content of TERM is a valid name in dircolors.  */

ایستا bool
known_term_type (تهی)
{
  نویسه ثابت *term = getenv ("TERM");
  اگر (! term || ! *term)
    بازگشت false;

  نویسه ثابت *سطر = G_سطر;
  وقتی (سطر - G_سطر < اندازه (G_سطر))
    {
      اگر (STRNCMP_LIT (سطر, "TERM ") == 0)
        {
          اگر (fnmatch (سطر + 5, term, 0) == 0)
            بازگشت true;
        }
      سطر += strlen (سطر) + 1;
    }

  بازگشت false;
}

ایستا تهی
parse_ls_color (تهی)
{
  نویسه ثابت *p;		/* Pointer to character being parsed */
  نویسه *buf;			/* color_buf buffer pointer */
  نویسه label0, label1;		/* Indicator label */
  ساختار color_ext_type *ext;	/* Extension we are working on */

  اگر ((p = getenv ("LS_COLORS")) == پوچ || *p == '\0')
    {
      /* LS_COLORS takes precedence, but if that's not set then
         honor the COLORTERM and TERM env variables so that
         we only go with the internal ANSI color codes if the
         former is non empty or the latter is set to a known value.  */
      نویسه ثابت *colorterm = getenv ("COLORTERM");
      اگر (! (colorterm && *colorterm) && ! known_term_type ())
        print_with_color = false;
      بازگشت;
    }

  ext = پوچ;

  /* This is an overly conservative estimate, but any possible
     LS_COLORS string will *not* generate a color_buf longer than
     itself, so it is a safe way of allocating a buffer in
     advance.  */
  buf = color_buf = xstrdup (p);

  شمارش parse_state state = PS_START;
  وقتی (true)
    {
      انتخاب (state)
        {
        حالت PS_START:		/* First label character */
          انتخاب (*p)
            {
            حالت ':':
              ++p;
              بشکن;

            حالت '*':
              /* Allocate new extension block and add to head of
                 linked list (this way a later definition will
                 override an earlier one, which can be useful for
                 having terminal-specific defs override global).  */

              ext = xmalloc (اندازه *ext);
              ext->next = color_ext_list;
              color_ext_list = ext;
              ext->exact_match = false;

              ++p;
              ext->ext.string = buf;

              state = (get_funky_string (&buf, &p, true, &ext->ext.len)
                       ? PS_4 : PS_FAIL);
              بشکن;

            حالت '\0':
              state = PS_DONE;	/* Done! */
              برو done;

            پیشفرض:	/* Assume it is file type label */
              label0 = *p++;
              state = PS_2;
              بشکن;
            }
          بشکن;

        حالت PS_2:		/* Second label character */
          اگر (*p)
            {
              label1 = *p++;
              state = PS_3;
            }
          وگرنه
            state = PS_FAIL;	/* Error */
          بشکن;

        حالت PS_3:		/* Equal sign after indicator label */
          state = PS_FAIL;	/* Assume failure...  */
          اگر (*(p++) == '=')/* It *should* be...  */
            {
              برای (صحیح i = 0; i < countof (indicator_name); i++)
                {
                  اگر ((label0 == indicator_name[i][0])
                      && (label1 == indicator_name[i][1]))
                    {
                      color_indicator[i].string = buf;
                      state = (get_funky_string (&buf, &p, false,
                                                 &color_indicator[i].len)
                               ? PS_START : PS_FAIL);
                      بشکن;
                    }
                }
              اگر (state == PS_FAIL)
                error (0, 0, _("unrecognized prefix: %s"),
                       quote ((نویسه []) {label0, label1, '\0'}));
            }
          بشکن;

        حالت PS_4:		/* Equal sign after *.ext */
          اگر (*(p++) == '=')
            {
              ext->seq.string = buf;
              state = (get_funky_string (&buf, &p, false, &ext->seq.len)
                       ? PS_START : PS_FAIL);
            }
          وگرنه
            state = PS_FAIL;
          بشکن;

        حالت PS_FAIL:
          برو done;

        حالت PS_DONE: پیشفرض:
          affirm (false);
        }
    }
 done:

  اگر (state == PS_FAIL)
    {
      error (0, 0,
             _("unparsable value for LS_COLORS environment variable"));
      آزاد (color_buf);
      برای (ساختار color_ext_type *e = color_ext_list; e != پوچ;
           /* empty */)
        {
          ساختار color_ext_type *e2 = e;
          e = e->next;
          آزاد (e2);
        }
      print_with_color = false;
      بازگشت;
    }
  وگرنه
    {
      /* Postprocess list to set EXACT_MATCH on entries where there are
         different cased extensions with separate sequences defined.
         Also set ext.len to SIZE_MAX on any entries that can't
         match due to precedence, to avoid redundant string compares.  */
      برای (ساختار color_ext_type *e1 = color_ext_list; e1 != پوچ;
           e1 = e1->next)
        {
          bool case_ignored = false;

          برای (ساختار color_ext_type *e2 = e1->next; e2 != پوچ;
               e2 = e2->next)
            {
              اگر (e2->ext.len < SIZE_MAX && e1->ext.len == e2->ext.len)
                {
                  اگر (memeq (e1->ext.string, e2->ext.string, e1->ext.len))
                    e2->ext.len = SIZE_MAX; /* Ignore */
                  وگرنه اگر (c_strncasecmp (e1->ext.string, e2->ext.string,
                                          e1->ext.len) == 0)
                    {
                      اگر (case_ignored)
                        {
                          e2->ext.len = SIZE_MAX; /* Ignore */
                        }
                      وگرنه اگر (e1->seq.len == e2->seq.len
                               && memeq (e1->seq.string, e2->seq.string,
                                         e1->seq.len))
                        {
                          e2->ext.len = SIZE_MAX; /* Ignore */
                          case_ignored = true;    /* Ignore all subsequent */
                        }
                      وگرنه
                        {
                          e1->exact_match = true;
                          e2->exact_match = true;
                        }
                    }
                }
            }
        }
    }

  اگر (color_indicator[C_LINK].len == 6
      && !STRNCMP_LIT (color_indicator[C_LINK].string, "target"))
    color_symlink_as_referent = true;
}

/* Set the exit status to report a failure.  If SERIOUS, it is a
   serious failure; otherwise, it is merely a minor problem.  */

ایستا تهی
set_exit_status (bool serious)
{
  اگر (serious)
    exit_status = LS_FAILURE;
  وگرنه اگر (exit_status == EXIT_SUCCESS)
    exit_status = LS_MINOR_PROBLEM;
}

/* Assuming a failure is serious if SERIOUS, use the printf-style
   MESSAGE to report the failure to access a file named FILE.  Assume
   errno is set appropriately for the failure.  */

ایستا تهی
file_failure (bool serious, نویسه ثابت *message, نویسه ثابت *file)
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

ایستا تهی
queue_directory (نویسه ثابت *name, نویسه ثابت *realname, bool command_سطر_arg)
{
  ساختار pending *new = xmalloc (اندازه *new);
  new->realname = realname ? xstrdup (realname) : پوچ;
  new->name = name ? xstrdup (name) : پوچ;
  new->command_سطر_arg = command_سطر_arg;
  new->next = pending_dirs;
  pending_dirs = new;
}

/* Read directory NAME, and list the files in it.
   If REALNAME is nonzero, print its name instead of NAME;
   this is used for symbolic links to directories.
   COMMAND_LINE_ARG means this directory was mentioned on the command line.  */

ایستا تهی
print_dir (نویسه ثابت *name, نویسه ثابت *realname, bool command_سطر_arg)
{
  DIR *dirp;
  ساختار dirent *next;
  uintmax_t total_blocks = 0;
  ایستا bool first = true;

  errno = 0;
  dirp = opendir (name);
  اگر (!dirp)
    {
      اگر (command_سطر_arg || ! ignorable_traversal_errno (errno))
        file_failure (command_سطر_arg, _("cannot open directory %s"), name);
      بازگشت;
    }

  اگر (LOOP_DETECT)
    {
      ساختار stat dir_stat;
      صحیح fd = dirfd (dirp);

      /* If dirfd failed, endure the overhead of stat'ing by path  */
      اگر ((0 <= fd
           ? fstat_for_ino (fd, &dir_stat)
           : stat_for_ino (name, &dir_stat)) < 0)
        {
          اگر (! ignorable_traversal_errno (errno))
            file_failure (command_سطر_arg,
                          _("cannot determine device and inode of %s"), name);
          closedir (dirp);
          بازگشت;
        }

      /* If we've already visited this dev/inode pair, warn that
         we've found a loop, and do not process this directory.  */
      اگر (visit_dir (dir_stat.st_dev, dir_stat.st_ino))
        {
          error (0, 0, _("%s: not listing already-listed directory"),
                 quotef (name));
          closedir (dirp);
          set_exit_status (true);
          بازگشت;
        }

      dev_ino_افزودن (dir_stat.st_dev, dir_stat.st_ino);
    }

  clear_files ();

  اگر (recursive || print_dir_name)
    {
      اگر (!first)
        dired_outbyte ('\n');
      first = false;
      dired_indent ();

      نویسه *absolute_name = پوچ;
      اگر (print_hyperlink)
        {
          absolute_name = canonicalize_filename_mode (name, CAN_MISSING);
          اگر (! absolute_name
              && ! ignorable_traversal_errno (errno))
            file_failure (command_سطر_arg,
                          _("error canonicalizing %s"), name);
        }
      quote_name (realname ? realname : name, dirname_quoting_options, -1,
                  پوچ, true, &subdired_obstack, absolute_name);

      آزاد (absolute_name);

      dired_outstring (":\n");
    }

  /* Read the directory entries, and insert the subfiles into the 'cwd_file'
     table.  */

  وقتی (true)
    {
      /* Set errno to zero so we can distinguish between a readdir failure
         and when readdir simply finds that there are no more entries.  */
      errno = 0;
      next = readdir (dirp);
      اگر (next)
        {
          اگر (! file_ignored (next->d_name))
            {
              شمارش filetype type;
#اگر HAVE_STRUCT_DIRENT_D_TYPE
              type = d_type_filetype[next->d_type];
#وگرنه
              type = unknown;
#endif
              total_blocks += gobble_file (next->d_name, type,
                                           RELIABLE_D_INO (next),
                                           false, name);

              /* In this narrow case, print out each name right away, so
                 ls uses constant memory while processing the entries of
                 this directory.  Useful when there are many (millions)
                 of entries in a directory.  */
              اگر (format == one_per_سطر && sort_type == sort_none
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
      وگرنه
        {
          صحیح err = errno;
          اگر (err == 0)
            بشکن;
          /* Ignore errors indicating that the directory was removed.  */
          اگر (ignorable_traversal_errno (err))
            بشکن;
          file_failure (command_سطر_arg, _("reading directory %s"), name);
          اگر (err != EOVERFLOW)
            بشکن;
        }

      /* When processing a very large directory, and since we've inhibited
         interrupts, this loop would take so long that ls would be annoyingly
         uninterruptible.  This ensures that it handles signals promptly.  */
      process_signals ();
    }

  اگر (closedir (dirp) != 0)
    {
      file_failure (command_سطر_arg, _("closing directory %s"), name);
      /* Don't return; print whatever we got.  */
    }

  /* Sort the directory contents.  */
  sort_files ();

  /* If any member files are subdirectories, perhaps they should have their
     contents listed rather than being mentioned here as files.  */

  اگر (recursive)
    extract_dirs_from_files (name, false);

  اگر (format == long_format || print_block_size)
    {
      نویسه buf[LONGEST_HUMAN_READABLE + 3];
      نویسه *p = human_readable (total_blocks, buf + 1, human_output_opts,
                                ST_NBLOCKSIZE, output_block_size);
      نویسه *pend = p + strlen (p);
      *--p = ' ';
      *pend++ = eolbyte;
      dired_indent ();
      dired_outstring (_("total"));
      dired_outbuf (p, pend - p);
    }

  اگر (cwd_n_used)
    print_current_files ();
}

/* Add 'pattern' to the list of patterns for which files that match are
   not listed.  */

ایستا تهی
add_ignore_pattern (نویسه ثابت *pattern)
{
  ساختار ignore_pattern *ignore;

  ignore = xmalloc (اندازه *ignore);
  ignore->pattern = pattern;
  /* Add it to the head of the linked list.  */
  ignore->next = ignore_patterns;
  ignore_patterns = ignore;
}

/* Return true if one of the PATTERNS matches FILE.  */

ایستا bool
patterns_match (ساختار ignore_pattern ثابت *patterns, نویسه ثابت *file)
{
  برای (ساختار ignore_pattern ثابت *p = patterns; p; p = p->next)
    اگر (fnmatch (p->pattern, file, FNM_PERIOD) == 0)
      بازگشت true;
  بازگشت false;
}

/* Return true if FILE should be ignored.  */

ایستا bool
file_ignored (نویسه ثابت *name)
{
  بازگشت ((ignore_mode != IGNORE_MINIMAL
           && name[0] == '.'
           && (ignore_mode == IGNORE_DEFAULT || ! name[1 + (name[1] == '.')]))
          || (ignore_mode == IGNORE_DEFAULT
              && patterns_match (hide_patterns, name))
          || patterns_match (ignore_patterns, name));
}

/* POSIX requires that a file size be printed without a sign, even
   when negative.  Assume the typical case where negative sizes are
   actually positive values that have wrapped around.  */

ایستا uintmax_t
unsigned_file_size (off_t size)
{
  بازگشت size + (size < 0) * ((uintmax_t) OFF_T_MAX - OFF_T_MIN + 1);
}

#ifdef HAVE_CAP
/* Return true if NAME has a capability (see linux/capability.h) */
ایستا bool
has_capability (نویسه ثابت *name)
{
  نویسه *result;
  bool has_cap;

  cap_t cap_d = cap_get_file (name);
  اگر (cap_d == پوچ)
    بازگشت false;

  result = cap_to_text (cap_d, پوچ);
  cap_free (cap_d);
  اگر (!result)
    بازگشت false;

  /* check if human-readable capability string is empty */
  has_cap = !!*result;

  cap_free (result);
  بازگشت has_cap;
}
#وگرنه
ایستا bool
has_capability (MAYBE_UNUSED نویسه ثابت *name)
{
  errno = ENOTSUP;
  بازگشت false;
}
#endif

/* Enter and remove entries in the table 'cwd_file'.  */

ایستا تهی
free_ent (ساختار fileinfo *f)
{
  آزاد (f->name);
  آزاد (f->linkname);
  آزاد (f->absolute_name);
  اگر (f->scontext != UNKNOWN_SECURITY_CONTEXT)
    aclinfo_scontext_free (f->scontext);
}

/* Empty the table of files.  */
ایستا تهی
clear_files (تهی)
{
  برای (idx_t i = 0; i < cwd_n_used; i++)
    {
      ساختار fileinfo *f = sorted_file[i];
      free_ent (f);
    }

  cwd_n_used = 0;
  cwd_some_quoted = false;
  any_has_acl = false;
  inode_عدد_width = 0;
  block_size_width = 0;
  nlink_width = 0;
  owner_width = 0;
  group_width = 0;
  author_width = 0;
  scontext_width = 0;
  major_device_عدد_width = 0;
  minor_device_عدد_width = 0;
  file_size_width = 0;
}

/* Cache file_has_aclinfo failure, when it's trivial to do.
   Like file_has_aclinfo, but when F's st_dev says it's on a file
   system lacking ACL support, return 0 with ENOTSUP immediately.  */
ایستا صحیح
file_has_aclinfo_cache (نویسه ثابت *file, ساختار fileinfo *f,
                        ساختار aclinfo *ai, صحیح flags)
{
  /* If UNSUPPORTED_SCONTEXT, these variables hold the
     st_dev and associated info for the most recently processed device
     for which file_has_aclinfo failed indicating lack of support.  */
  ایستا صحیح unsupported_return;
  ایستا نویسه *unsupported_scontext;
  ایستا صحیح unsupported_scontext_err;
  ایستا dev_t unsupported_device;

  اگر (f->stat_ok && unsupported_scontext
      && f->stat.st_dev == unsupported_device)
    {
      ai->buf = ai->u._gl_acl_ch;
      ai->size = 0;
      ai->scontext = unsupported_scontext;
      ai->scontext_err = unsupported_scontext_err;
      errno = ENOTSUP;
      بازگشت unsupported_return;
    }

  errno = 0;
  صحیح n = file_has_aclinfo (file, ai, flags);
  صحیح err = errno;
  اگر (!USE_ACL)
    n = 0;
  /* A security-context error does not imply that ACLs are unsupported.
     Require an unsupported attribute listing too if ACLs or capability
     colors are needed.  */
  اگر (f->stat_ok && n <= 0 && !acl_errno_valid (err)
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
  بازگشت n;
}

/* Cache has_capability failure, when it's trivial to do.
   Like has_capability, but when F's st_dev says it's on a file
   system lacking capability support, return 0 with ENOTSUP immediately.  */
ایستا bool
has_capability_cache (نویسه ثابت *file, ساختار fileinfo *f)
{
  /* If UNSUPPORTED_CACHED, UNSUPPORTED_DEVICE is the cached
     st_dev of the most recently processed device for which we've
     found that has_capability fails indicating lack of support.  */
  ایستا bool unsupported_cached;
  ایستا dev_t unsupported_device;

  اگر (f->stat_ok && unsupported_cached && f->stat.st_dev == unsupported_device)
    {
      errno = ENOTSUP;
      بازگشت 0;
    }

  bool b = has_capability (file);
  اگر (f->stat_ok && !b && !acl_errno_valid (errno))
    {
      unsupported_cached = true;
      unsupported_device = f->stat.st_dev;
    }
  بازگشت b;
}

/* Get the ACL, security context, and capability information needed for F.
   FULL_NAME names the file; DO_DEREF says whether to follow symlinks.  */
ایستا تهی
get_extended_info (نویسه ثابت *full_name, ساختار fileinfo *f, bool do_deref)
{
  /* FIXME: file_has_aclinfo() conflates capability and acl xattr lookup
     with security context attributes.  Ideally we'd have separate
     ACL_GET_{CAPABILITY,ACL} flags to lookup just the required items,
     simplifying the logic here.  */

  شمارش filetype type = f->filetype;
  bool check_acl = USE_ACL && format == long_format;
  bool get_scontext = false;
#اگر HAVE_SMACK || USE_SELINUX_SELINUX_H
  get_scontext = (format == long_format) | print_scontext;
#endif
  bool check_capability = format_needs_capability & (type == normal);

  اگر (type != arg_directory && (check_acl | get_scontext | check_capability))
    {
      ساختار aclinfo ai;
      /* Without ACL support, request contexts to obtain the attribute
         names needed for capability coloring too.  */
      صحیح aclinfo_flags = ((do_deref ? ACL_SYMLINK_FOLLOW : 0)
                           | (get_scontext || (!USE_ACL && check_capability)
                              ? ACL_GET_SCONTEXT : 0)
                           | filetype_d_type[type]);
      صحیح n = file_has_aclinfo_cache (full_name, f, &ai, aclinfo_flags);
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

      اگر (format == long_format && n < 0 && !cannot_access_acl)
        error (0, errno, "%s", quotef (full_name));
      وگرنه
        {
          /* When requesting security context information, don't make
             ls fail just because the file (even a command line argument)
             isn't on the right type of file system.  I.e., a getfilecon
             failure isn't in the same class as a stat failure.  */
          اگر (print_scontext && ai.scontext_err
              && ! ignorable_traversal_errno (ai.scontext_err)
              && (! (is_ENOTSUP (ai.scontext_err)
                     || ai.scontext_err == ENODATA)))
            error (0, ai.scontext_err, "%s", quotef (full_name));
        }

      /* has_capability adds around 30% runtime to 'ls --color',
         so call it only if really needed.  Note capability coloring
         is disabled in the default color config.  */
      اگر (check_capability && aclinfo_has_xattr (&ai, XATTR_NAME_CAPS))
        f->has_capability = has_capability_cache (full_name, f);

      f->scontext = ai.scontext;
      ai.scontext = پوچ;
      aclinfo_free (&ai);
    }
}

ایستا bool
needs_quoting (نویسه ثابت *name)
{
  نویسه test[2];
  size_t len = quotearg_buffer (test, اندازه test , name, -1,
                                filename_quoting_options);
  بازگشت *name != *test || strlen (name) != len;
}

/* Add a file to the current table of files.
   Verify that the file exists, and print an error message if it does not.
   Return the number of blocks that the file occupies.  */
ایستا uintmax_t
gobble_file (نویسه ثابت *name, شمارش filetype type, ino_t inode,
             bool command_سطر_arg, نویسه ثابت *dirname)
{
  uintmax_t blocks = 0;
  ساختار fileinfo *f;

  /* An inode value prior to gobble_file necessarily came from readdir,
     which is not used for command line arguments.  */
  affirm (! command_سطر_arg || inode == NOT_AN_INODE_عدد);

  اگر (cwd_n_used == cwd_n_alloc)
    cwd_file = xpalloc (cwd_file, &cwd_n_alloc, 1, -1, اندازه *cwd_file);

  f = &cwd_file[cwd_n_used];
  memset (f, '\0', اندازه *f);
  f->stat.st_ino = inode;
  f->filetype = type;
  f->scontext = UNKNOWN_SECURITY_CONTEXT;

  f->quoted = -1;
  اگر ((! cwd_some_quoted) && align_variable_outer_quotes)
    {
      /* Determine if any quoted for padding purposes.  */
      f->quoted = needs_quoting (name);
      اگر (f->quoted)
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
      || (print_inode && inode == NOT_AN_INODE_عدد)
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
  نویسه ثابت *full_name = name;
  اگر (check_stat | print_scontext | format_needs_capability
      && name[0] != '/' && dirname)
    {
      نویسه *p = alloca (strlen (name) + strlen (dirname) + 2);
      attach (p, dirname, name);
      full_name = p;
    }

  bool do_deref;

  اگر (!check_stat)
    do_deref = dereference == DEREF_ALWAYS;
  وگرنه
    {
      صحیح err;

      اگر (print_hyperlink)
        {
          f->absolute_name = canonicalize_filename_mode (full_name,
                                                         CAN_MISSING);
          اگر (! f->absolute_name
              && (command_سطر_arg
                  || ! ignorable_traversal_errno (errno)))
            file_failure (command_سطر_arg,
                          _("error canonicalizing %s"), full_name);
        }

      انتخاب (dereference)
        {
        حالت DEREF_ALWAYS:
          err = do_stat (full_name, &f->stat);
          do_deref = true;
          بشکن;

        حالت DEREF_COMMAND_سطر_ARGUMENTS:
        حالت DEREF_COMMAND_سطر_SYMLINK_TO_DIR:
          اگر (command_سطر_arg)
            {
              bool need_lstat;
              err = do_stat (full_name, &f->stat);
              do_deref = true;

              اگر (dereference == DEREF_COMMAND_سطر_ARGUMENTS)
                بشکن;

              need_lstat = (err < 0
                            ? (errno == ENOENT || errno == ELOOP)
                            : ! S_ISDIR (f->stat.st_mode));
              اگر (!need_lstat)
                بشکن;

              /* stat failed because of ENOENT || ELOOP, maybe indicating a
                 non-traversable symlink.  Or stat succeeded,
                 FULL_NAME does not refer to a directory,
                 and --dereference-command-line-symlink-to-dir is in effect.
                 Fall through so that we call lstat instead.  */
            }
          FALLTHROUGH;

        حالت DEREF_NEVER:
          err = do_lstat (full_name, &f->stat);
          do_deref = false;
          بشکن;

        حالت DEREF_UNDEFINED: پیشفرض:
          affirm (false);
        }

      اگر (err != 0)
        {
          اگر (! command_سطر_arg && ignorable_traversal_errno (errno))
            {
              صحیح stat_errno = errno;
              ساختار stat linkstat;
              bool dangling_link = (do_deref
                                    && (type == symbolic_link
                                        || type == unknown)
                                    && do_lstat (full_name, &linkstat) == 0);
              errno = stat_errno;
              اگر (! dangling_link)
                {
                  free_ent (f);
                  بازگشت 0;
                }
            }

          /* Failure to stat a command line argument leads to
             an exit status of 2.  For other files, stat failure
             provokes an exit status of 1.  */
          file_failure (command_سطر_arg,
                        _("cannot access %s"), full_name);

          اگر (command_سطر_arg)
            {
              free_ent (f);
              بازگشت 0;
            }

          f->name = xstrdup (name);
          cwd_n_used++;

          بازگشت 0;
        }

      f->stat_ok = true;
      f->filetype = type = d_type_filetype[IFTODT (f->stat.st_mode)];
    }

  اگر (type == directory && command_سطر_arg && !immediate_dirs)
    f->filetype = type = arg_directory;

  get_extended_info (full_name, f, do_deref);

  اگر ((type == symbolic_link)
      & ((format == long_format) | check_symlink_mode))
    {
      ساختار stat linkstats;

      اگر (! get_link_name (full_name, f, command_سطر_arg))
        {
          free_ent (f);
          بازگشت 0;
        }

      /* Use the slower quoting path for this entry, though
         don't update CWD_SOME_QUOTED since alignment not affected.  */
      اگر (f->linkname && f->quoted == 0 && needs_quoting (f->linkname))
        f->quoted = -1;

      /* Avoid following symbolic links when possible, i.e., when
         they won't be traced and when no indicator is needed.  */
      اگر (f->linkname
          && (file_type <= indicator_style || check_symlink_mode)
          && stat_for_mode (full_name, &linkstats) == 0)
        {
          f->linkok = true;
          f->linkmode = linkstats.st_mode;
        }
    }

  blocks = STP_NBLOCKS (&f->stat);
  اگر (format == long_format || print_block_size)
    {
      نویسه buf[LONGEST_HUMAN_READABLE + 1];
      صحیح len = mbswidth (human_readable (blocks, buf, human_output_opts,
                                          ST_NBLOCKSIZE, output_block_size),
                          MBSWIDTH_FLAGS);
      اگر (block_size_width < len)
        block_size_width = len;
    }

  اگر (format == long_format)
    {
      اگر (print_owner)
        {
          صحیح len = format_user_width (f->stat.st_uid);
          اگر (owner_width < len)
            owner_width = len;
        }

      اگر (print_group)
        {
          صحیح len = format_group_width (f->stat.st_gid);
          اگر (group_width < len)
            group_width = len;
        }

      اگر (print_author)
        {
          صحیح len = format_user_width (f->stat.st_author);
          اگر (author_width < len)
            author_width = len;
        }
    }

  اگر (print_scontext)
    {
      صحیح len = strlen (f->scontext);
      اگر (scontext_width < len)
        scontext_width = len;
    }

  اگر (format == long_format)
    {
      نویسه b[INT_BUFSIZE_BOUND (uintmax_t)];
      صحیح b_len = strlen (umaxtostr (f->stat.st_nlink, b));
      اگر (nlink_width < b_len)
        nlink_width = b_len;

      اگر ((type == chardev) | (type == blockdev))
        {
          نویسه buf[INT_BUFSIZE_BOUND (uintmax_t)];
          صحیح len = strlen (umaxtostr (major (f->stat.st_rdev), buf));
          اگر (major_device_عدد_width < len)
            major_device_عدد_width = len;
          len = strlen (umaxtostr (minor (f->stat.st_rdev), buf));
          اگر (minor_device_عدد_width < len)
            minor_device_عدد_width = len;
          len = major_device_عدد_width + 2 + minor_device_عدد_width;
          اگر (file_size_width < len)
            file_size_width = len;
        }
      وگرنه
        {
          نویسه buf[LONGEST_HUMAN_READABLE + 1];
          uintmax_t size = unsigned_file_size (f->stat.st_size);
          صحیح len = mbswidth (human_readable (size, buf,
                                              file_human_output_opts,
                                              1, file_output_block_size),
                              MBSWIDTH_FLAGS);
          اگر (file_size_width < len)
            file_size_width = len;
        }
    }

  اگر (print_inode)
    {
      نویسه buf[INT_BUFSIZE_BOUND (uintmax_t)];
      صحیح len = strlen (umaxtostr (f->stat.st_ino, buf));
      اگر (inode_عدد_width < len)
        inode_عدد_width = len;
    }

  f->name = xstrdup (name);
  cwd_n_used++;

  بازگشت blocks;
}

/* Return true if F refers to a directory.  */
ایستا bool
is_directory (ثابت ساختار fileinfo *f)
{
  بازگشت f->filetype == directory || f->filetype == arg_directory;
}

/* Return true if F refers to a (symlinked) directory.  */
ایستا bool
is_linked_directory (ثابت ساختار fileinfo *f)
{
  بازگشت f->filetype == directory || f->filetype == arg_directory
         || S_ISDIR (f->linkmode);
}

/* Put the name of the file that FILENAME is a symbolic link to
   into the LINKNAME field of 'f'.  COMMAND_LINE_ARG indicates whether
   FILENAME is a command-line argument.  */

ایستا bool
get_link_name (نویسه ثابت *filename, ساختار fileinfo *f, bool command_سطر_arg)
{
  f->linkname = areadlink_with_size (filename, f->stat.st_size);
  اگر (f->linkname == پوچ)
    {
      اگر (! command_سطر_arg
          && ignorable_traversal_errno (errno))
        بازگشت false;
      file_failure (command_سطر_arg, _("cannot read symbolic link %s"),
                    filename);
    }
  بازگشت true;
}

/* Return true if the last component of NAME is '.' or '..'
   This is so we don't try to recurse on '././././. ...' */

ایستا bool
basename_is_dot_or_dotdot (نویسه ثابت *name)
{
  نویسه ثابت *base = last_component (name);
  بازگشت dot_or_dotdot (base);
}

/* Remove any entries from CWD_FILE that are for directories,
   and queue them to be listed as directories instead.
   DIRNAME is the prefix to prepend to each dirname
   to make it correct relative to ls's working dir;
   if it is null, no prefix is needed and "." and ".." should not be ignored.
   If COMMAND_LINE_ARG is true, this directory was mentioned at the top level,
   This is desirable when processing directories recursively.  */

ایستا تهی
extract_dirs_from_files (نویسه ثابت *dirname, bool command_سطر_arg)
{
  idx_t i, j;
  bool ignore_dot_and_dot_dot = (dirname != پوچ);

  اگر (dirname && LOOP_DETECT)
    {
      /* Insert a marker entry first.  When we dequeue this marker entry,
         we'll know that DIRNAME has been processed and may be removed
         from the set of active directories.  */
      queue_directory (پوچ, dirname, false);
    }

  /* Queue the directories last one first, because queueing reverses the
     order.  */
  برای (i = cwd_n_used; 0 < i; )
    {
      i--;
      ساختار fileinfo *f = sorted_file[i];

      اگر (is_directory (f)
          && (! ignore_dot_and_dot_dot
              || ! basename_is_dot_or_dotdot (f->name)))
        {
          اگر (!dirname || f->name[0] == '/')
            queue_directory (f->name, f->linkname, command_سطر_arg);
          وگرنه
            {
              نویسه *name = file_name_concat (dirname, f->name, پوچ);
              queue_directory (name, f->linkname, command_سطر_arg);
              آزاد (name);
            }
          اگر (f->filetype == arg_directory)
            free_ent (f);
        }
    }

  /* Now delete the directories from the table, compacting all the remaining
     entries.  */

  برای (i = 0, j = 0; i < cwd_n_used; i++)
    {
      ساختار fileinfo *f = sorted_file[i];
      sorted_file[j] = f;
      j += (f->filetype != arg_directory);
    }
  cwd_n_used = j;
}

/* Use strcoll to compare strings in this locale.  If an error occurs,
   report an error and longjmp to failed_strcoll.  */

ایستا jmp_buf failed_strcoll;

ایستا صحیح
xstrcoll (نویسه ثابت *a, نویسه ثابت *b)
{
  صحیح diff;
  errno = 0;
  diff = strcoll (a, b);
  اگر (errno)
    {
      error (0, errno, _("cannot compare file names %s and %s"),
             quote_n (0, a), quote_n (1, b));
      set_exit_status (false);
      longjmp (failed_strcoll, 1);
    }
  بازگشت diff;
}

/* Comparison routines for sorting the files.  */

تعریفنوع تهی ثابت *V;
تعریفنوع صحیح (*qsortFunc)(V a, V b);

/* Used below in DEFINE_SORT_FUNCTIONS for _df_ sort function variants.  */
ایستا صحیح
dirfirst_check (ساختار fileinfo ثابت *a, ساختار fileinfo ثابت *b,
                صحیح (*cmp) (V, V))
{
  صحیح diff = is_linked_directory (b) - is_linked_directory (a);
  بازگشت diff ? diff : cmp (a, b);
}

/* Define the 8 different sort function variants required for each sortkey.
   KEY_NAME is a token describing the sort key, e.g., ctime, atime, size.
   KEY_CMP_FUNC is a function to compare records based on that key, e.g.,
   ctime_cmp, atime_cmp, size_cmp.  Append KEY_NAME to the string,
   '[rev_][x]str{cmp|coll}[_df]_', to create each function name.  */
#define DEFINE_SORT_FUNCTIONS(کلید_name, کلید_cmp_func)			\
  /* direct, non-dirfirst versions */					\
  ایستا صحیح xstrcoll_##key_name (V a, V b)				\
  { بازگشت کلید_cmp_func (a, b, xstrcoll); }				\
  ATTRIBUTE_PURE ایستا صحیح strcmp_##key_name (V a, V b)		\
  { بازگشت کلید_cmp_func (a, b, strcmp); }				\
                                                                        \
  /* reverse, non-dirfirst versions */					\
  ایستا صحیح rev_xstrcoll_##key_name (V a, V b)				\
  { بازگشت کلید_cmp_func (b, a, xstrcoll); }				\
  ATTRIBUTE_PURE ایستا صحیح rev_strcmp_##key_name (V a, V b)	\
  { بازگشت کلید_cmp_func (b, a, strcmp); }				\
                                                                        \
  /* direct, dirfirst versions */					\
  ایستا صحیح xstrcoll_df_##key_name (V a, V b)				\
  { بازگشت dirfirst_check (a, b, xstrcoll_##key_name); }		\
  ATTRIBUTE_PURE ایستا صحیح strcmp_df_##key_name (V a, V b)		\
  { بازگشت dirfirst_check (a, b, strcmp_##key_name); }			\
                                                                        \
  /* reverse, dirfirst versions */					\
  ایستا صحیح rev_xstrcoll_df_##key_name (V a, V b)			\
  { بازگشت dirfirst_check (a, b, rev_xstrcoll_##key_name); }		\
  ATTRIBUTE_PURE ایستا صحیح rev_strcmp_df_##key_name (V a, V b)	\
  { بازگشت dirfirst_check (a, b, rev_strcmp_##key_name); }

ایستا صحیح
cmp_ctime (ساختار fileinfo ثابت *a, ساختار fileinfo ثابت *b,
           صحیح (*cmp) (نویسه ثابت *, نویسه ثابت *))
{
  صحیح diff = timespec_cmp (get_stat_ctime (&b->stat),
                           get_stat_ctime (&a->stat));
  بازگشت diff ? diff : cmp (a->name, b->name);
}

ایستا صحیح
cmp_mtime (ساختار fileinfo ثابت *a, ساختار fileinfo ثابت *b,
           صحیح (*cmp) (نویسه ثابت *, نویسه ثابت *))
{
  صحیح diff = timespec_cmp (get_stat_mtime (&b->stat),
                           get_stat_mtime (&a->stat));
  بازگشت diff ? diff : cmp (a->name, b->name);
}

ایستا صحیح
cmp_atime (ساختار fileinfo ثابت *a, ساختار fileinfo ثابت *b,
           صحیح (*cmp) (نویسه ثابت *, نویسه ثابت *))
{
  صحیح diff = timespec_cmp (get_stat_atime (&b->stat),
                           get_stat_atime (&a->stat));
  بازگشت diff ? diff : cmp (a->name, b->name);
}

ایستا صحیح
cmp_btime (ساختار fileinfo ثابت *a, ساختار fileinfo ثابت *b,
           صحیح (*cmp) (نویسه ثابت *, نویسه ثابت *))
{
  صحیح diff = timespec_cmp (get_stat_btime (&b->stat),
                           get_stat_btime (&a->stat));
  بازگشت diff ? diff : cmp (a->name, b->name);
}

ایستا صحیح
off_cmp (off_t a, off_t b)
{
  بازگشت _GL_CMP (a, b);
}

ایستا صحیح
cmp_size (ساختار fileinfo ثابت *a, ساختار fileinfo ثابت *b,
          صحیح (*cmp) (نویسه ثابت *, نویسه ثابت *))
{
  صحیح diff = off_cmp (b->stat.st_size, a->stat.st_size);
  بازگشت diff ? diff : cmp (a->name, b->name);
}

ایستا صحیح
cmp_name (ساختار fileinfo ثابت *a, ساختار fileinfo ثابت *b,
          صحیح (*cmp) (نویسه ثابت *, نویسه ثابت *))
{
  بازگشت cmp (a->name, b->name);
}

/* Compare file extensions.  Files with no extension are 'smallest'.
   If extensions are the same, compare by file names instead.  */

ایستا صحیح
cmp_extension (ساختار fileinfo ثابت *a, ساختار fileinfo ثابت *b,
               صحیح (*cmp) (نویسه ثابت *, نویسه ثابت *))
{
  نویسه ثابت *base1 = strrchr (a->name, '.');
  نویسه ثابت *base2 = strrchr (b->name, '.');
  صحیح diff = cmp (base1 ? base1 : "", base2 ? base2 : "");
  بازگشت diff ? diff : cmp (a->name, b->name);
}

/* Return the (cached) screen width,
   for the NAME associated with the passed fileinfo F.  */

ایستا size_t
fileinfo_name_width (ساختار fileinfo ثابت *f)
{
  بازگشت f->width
         ? f->width
         : quote_name_width (f->name, filename_quoting_options, f->quoted);
}

ایستا صحیح
cmp_width (ساختار fileinfo ثابت *a, ساختار fileinfo ثابت *b,
          صحیح (*cmp) (نویسه ثابت *, نویسه ثابت *))
{
  صحیح diff = fileinfo_name_width (a) - fileinfo_name_width (b);
  بازگشت diff ? diff : cmp (a->name, b->name);
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
ایستا صحیح
cmp_version (ساختار fileinfo ثابت *a, ساختار fileinfo ثابت *b)
{
  صحیح diff = filevercmp (a->name, b->name);
  بازگشت diff ? diff : strcmp (a->name, b->name);
}

ایستا صحیح
xstrcoll_version (V a, V b)
{
  بازگشت cmp_version (a, b);
}
ایستا صحیح
rev_xstrcoll_version (V a, V b)
{
  بازگشت cmp_version (b, a);
}
ایستا صحیح
xstrcoll_df_version (V a, V b)
{
  بازگشت dirfirst_check (a, b, xstrcoll_version);
}
ایستا صحیح
rev_xstrcoll_df_version (V a, V b)
{
  بازگشت dirfirst_check (a, b, rev_xstrcoll_version);
}


/* We have 2^3 different variants for each sort-key function
   (for 3 independent sort modes).
   The function pointers stored in this array must be dereferenced as:

    sort_variants[sort_key][use_strcmp][reverse][dirs_first]

   Note that the order in which sort keys are listed in the function pointer
   array below is defined by the order of the elements in the time_type and
   sort_type enums!  */

#define LIST_SORTFUNCTION_VARIANTS(کلید_name)                        \
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

ایستا qsortFunc ثابت sort_functions[][2][2][2] =
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
        { پوچ, پوچ },
        { پوچ, پوچ },
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

ایستا تهی
initialize_ordering_vector (تهی)
{
  برای (idx_t i = 0; i < cwd_n_used; i++)
    sorted_file[i] = &cwd_file[i];
}

/* Cache values based on attributes global to all files.  */

ایستا تهی
update_current_files_info (تهی)
{
  /* Cache screen width of name, if needed multiple times.  */
  اگر (sort_type == sort_width
      || (سطر_length && (format == many_per_سطر || format == horizontal)))
    {
      برای (idx_t i = 0; i < cwd_n_used; i++)
        {
          ساختار fileinfo *f = sorted_file[i];
          f->width = fileinfo_name_width (f);
        }
    }
}

/* Sort the files now in the table.  */

ایستا تهی
sort_files (تهی)
{
  bool use_strcmp;

  اگر (sorted_file_alloc < cwd_n_used + (cwd_n_used >> 1))
    {
      آزاد (sorted_file);
      sorted_file = xinmalloc (cwd_n_used, 3 * اندازه *sorted_file);
      sorted_file_alloc = 3 * cwd_n_used;
    }

  initialize_ordering_vector ();

  update_current_files_info ();

  اگر (sort_type == sort_none)
    بازگشت;

  /* Try strcoll.  If it fails, fall back on strcmp.  We can't safely
     ignore strcoll failures, as a failing strcoll might be a
     comparison function that is not a total order, and if we ignored
     the failure this might cause qsort to dump core.  */

  اگر (! setjmp (failed_strcoll))
    use_strcmp = false;      /* strcoll() succeeded */
  وگرنه
    {
      use_strcmp = true;
      affirm (sort_type != sort_version);
      initialize_ordering_vector ();
    }

  /* When sort_type == sort_time, use time_type as subindex.  */
  mpsort ((تهی ثابت **) sorted_file, cwd_n_used,
          sort_functions[sort_type + (sort_type == sort_time ? time_type : 0)]
                        [use_strcmp][sort_reverse]
                        [directories_first]);
}

/* List all the files now in the table.  */

ایستا تهی
print_current_files (تهی)
{
  انتخاب (format)
    {
    حالت one_per_سطر:
      برای (idx_t i = 0; i < cwd_n_used; i++)
        {
          print_file_name_and_frills (sorted_file[i], 0);
          putchar (eolbyte);
        }
      بشکن;

    حالت many_per_سطر:
      اگر (! سطر_length)
        print_with_separator (' ');
      وگرنه
        print_many_per_سطر ();
      بشکن;

    حالت horizontal:
      اگر (! سطر_length)
        print_with_separator (' ');
      وگرنه
        print_horizontal ();
      بشکن;

    حالت with_commas:
      print_with_separator (',');
      بشکن;

    حالت long_format:
      برای (idx_t i = 0; i < cwd_n_used; i++)
        {
          set_normal_color ();
          print_long_format (sorted_file[i]);
          dired_outbyte (eolbyte);
        }
      بشکن;
    }
}

/* Replace the first %b with precomputed aligned month names.
   Note on glibc-2.7 at least, this speeds up the whole 'ls -lU'
   process by around 17%, compared to letting strftime() handle the %b.  */

ایستا ptrdiff_t
align_nstrftime (نویسه *buf, size_t size, bool recent, ساختار tm ثابت *tm,
                 timezone_t tz, صحیح ns)
{
  نویسه ثابت *nfmt = (use_abformat
                      ? abformat[recent][tm->tm_mon]
                      : long_time_format[recent]);
  بازگشت nstrftime (buf, size, nfmt, tm, tz, ns);
}

/* Return the expected number of columns in a long-format timestamp,
   or zero if it cannot be calculated.  */

ایستا صحیح
long_time_expected_width (تهی)
{
  ایستا صحیح width = -1;

  اگر (width < 0)
    {
      time_t epoch = 0;
      ساختار tm tm;
      نویسه buf[TIME_STAMP_LEN_MAXIMUM + 1];

      /* In case you're wondering if localtime_rz can fail with an input time_t
         value of 0, let's just say it's very unlikely, but not inconceivable.
         The TZ environment variable would have to specify a time zone that
         is 2**31-1900 years or more ahead of UTC.  This could happen only on
         a 64-bit system that blindly accepts e.g., TZ=UTC+20000000000000.
         However, this is not possible with Solaris 10 or glibc-2.3.5, since
         their implementations limit the offset to 167:59 and 24:00, resp.  */
      اگر (localtime_rz (localtz, &epoch, &tm))
        {
          ptrdiff_t len = align_nstrftime (buf, اندازه buf, false,
                                           &tm, localtz, 0);
          اگر (len > 0)
            width = mbsnwidth (buf, len, MBSWIDTH_FLAGS);
        }

      اگر (width < 0)
        width = 0;
    }

  بازگشت width;
}

/* Print the user or group name NAME, with numeric id ID, using a
   print width of WIDTH columns.  */

ایستا تهی
format_user_or_group (نویسه ثابت *name, uintmax_t id, صحیح width)
{
  اگر (name)
    {
      صحیح name_width = mbswidth (name, MBSWIDTH_FLAGS);
      صحیح width_gap = name_width < 0 ? 0 : width - name_width;
      صحیح pad = MAX (0, width_gap);
      dired_outstring (name);

      انجامبده
        dired_outbyte (' ');
      وقتی (pad--);
    }
  وگرنه
    dired_pos += printf ("%*ju ", width, id);
}

/* Print the name or id of the user with id U, using a print width of
   WIDTH.  */

ایستا تهی
format_user (uid_t u, صحیح width, bool stat_ok)
{
  format_user_or_group (! stat_ok ? "?" :
                        (numeric_ids ? پوچ : getuser (u)), u, width);
}

/* Likewise, for groups.  */

ایستا تهی
format_group (gid_t g, صحیح width, bool stat_ok)
{
  format_user_or_group (! stat_ok ? "?" :
                        (numeric_ids ? پوچ : getgroup (g)), g, width);
}

/* Return the number of columns that format_user_or_group will print,
   or -1 if unknown.  */

ایستا صحیح
format_user_or_group_width (نویسه ثابت *name, uintmax_t id)
{
  بازگشت (name
          ? mbswidth (name, MBSWIDTH_FLAGS)
          : snprintf (پوچ, 0, "%ju", id));
}

/* Return the number of columns that format_user will print,
   or -1 if unknown.  */

ایستا صحیح
format_user_width (uid_t u)
{
  بازگشت format_user_or_group_width (numeric_ids ? پوچ : getuser (u), u);
}

/* Likewise, for groups.  */

ایستا صحیح
format_group_width (gid_t g)
{
  بازگشت format_user_or_group_width (numeric_ids ? پوچ : getgroup (g), g);
}

/* Return a pointer to a formatted version of F->stat.st_ino,
   possibly using buffer, which must be at least
   INT_BUFSIZE_BOUND (uintmax_t) bytes.  */
ایستا نویسه *
format_inode (نویسه buf[INT_BUFSIZE_BOUND (uintmax_t)],
              ثابت ساختار fileinfo *f)
{
  بازگشت (f->stat_ok && f->stat.st_ino != NOT_AN_INODE_عدد
          ? umaxtostr (f->stat.st_ino, buf)
          : (نویسه *) "?");
}

/* Print information about F in long format.  */
ایستا تهی
print_long_format (ثابت ساختار fileinfo *f)
{
  نویسه modebuf[12];
  نویسه buf
    [LONGEST_HUMAN_READABLE + 1		/* inode */
     + LONGEST_HUMAN_READABLE + 1	/* size in blocks */
     + اندازه (modebuf) - 1 + 1		/* mode string */
     + INT_BUFSIZE_BOUND (uintmax_t)	/* st_nlink */
     + LONGEST_HUMAN_READABLE + 2	/* major device number */
     + LONGEST_HUMAN_READABLE + 1	/* minor device number */
     + TIME_STAMP_LEN_MAXIMUM + 1	/* max length of time/date */
     ];
  ptrdiff_t s;
  نویسه *p;
  ساختار timespec when_timespec;
  ساختار tm when_local;
  bool btime_ok = true;

  /* Compute the mode string, except remove the trailing space if no
     file in this directory has an ACL or security context.  */
  اگر (f->stat_ok)
    filemodestring (&f->stat, modebuf);
  وگرنه
    {
      modebuf[0] = filetype_letter[f->filetype];
      memset (modebuf + 1, '?', 10);
      modebuf[11] = '\0';
    }
  اگر (! any_has_acl)
    modebuf[10] = '\0';
  وگرنه اگر (f->acl_type == ACL_T_LSM_CONTEXT_ONLY)
    modebuf[10] = '.';
  وگرنه اگر (f->acl_type == ACL_T_YES)
    modebuf[10] = '+';
  وگرنه اگر (f->acl_type == ACL_T_UNKNOWN)
    modebuf[10] = '?';

  انتخاب (time_type)
    {
    حالت time_ctime:
      when_timespec = get_stat_ctime (&f->stat);
      بشکن;
    حالت time_mtime:
      when_timespec = get_stat_mtime (&f->stat);
      بشکن;
    حالت time_atime:
      when_timespec = get_stat_atime (&f->stat);
      بشکن;
    حالت time_btime:
      when_timespec = get_stat_btime (&f->stat);
      اگر (when_timespec.tv_sec == -1 && when_timespec.tv_nsec == -1)
        btime_ok = false;
      بشکن;
    حالت time_numtypes: پیشفرض:
      unreachable ();
    }

  p = buf;

  اگر (print_inode)
    {
      نویسه hbuf[INT_BUFSIZE_BOUND (uintmax_t)];
      p += sprintf (p, "%*s ", inode_عدد_width, format_inode (hbuf, f));
    }

  اگر (print_block_size)
    {
      نویسه hbuf[LONGEST_HUMAN_READABLE + 1];
      نویسه ثابت *blocks =
        (! f->stat_ok
         ? "?"
         : human_readable (STP_NBLOCKS (&f->stat), hbuf, human_output_opts,
                           ST_NBLOCKSIZE, output_block_size));
      صحیح blocks_width = mbswidth (blocks, MBSWIDTH_FLAGS);
      برای (صحیح pad = blocks_width < 0 ? 0 : block_size_width - blocks_width;
           0 < pad; pad--)
        *p++ = ' ';
      وقتی ((*p++ = *blocks++))
        ادامه;
      p[-1] = ' ';
    }

  /* The last byte of the mode string is the POSIX
     "optional alternate access method flag".  */
  {
    نویسه hbuf[INT_BUFSIZE_BOUND (uintmax_t)];
    p += sprintf (p, "%s %*s ", modebuf, nlink_width,
                  ! f->stat_ok ? "?" : umaxtostr (f->stat.st_nlink, hbuf));
  }

  dired_indent ();

  اگر (print_owner || print_group || print_author || print_scontext)
    {
      dired_outbuf (buf, p - buf);

      اگر (print_owner)
        format_user (f->stat.st_uid, owner_width, f->stat_ok);

      اگر (print_group)
        format_group (f->stat.st_gid, group_width, f->stat_ok);

      اگر (print_author)
        format_user (f->stat.st_author, author_width, f->stat_ok);

      اگر (print_scontext)
        format_user_or_group (f->scontext, 0, scontext_width);

      p = buf;
    }

  اگر (f->stat_ok
      && (S_ISCHR (f->stat.st_mode) || S_ISBLK (f->stat.st_mode)))
    {
      صحیح blanks_width = (file_size_width
                          - (major_device_عدد_width + 2
                             + minor_device_عدد_width));
      p += sprintf (p, "%*ju, %*ju ",
                    major_device_عدد_width + MAX (0, blanks_width),
                    (uintmax_t) major (f->stat.st_rdev),
                    minor_device_عدد_width,
                    (uintmax_t) minor (f->stat.st_rdev));
    }
  وگرنه
    {
      نویسه hbuf[LONGEST_HUMAN_READABLE + 1];
      نویسه ثابت *size =
        (! f->stat_ok
         ? "?"
         : human_readable (unsigned_file_size (f->stat.st_size),
                           hbuf, file_human_output_opts, 1,
                           file_output_block_size));
      صحیح size_width = mbswidth (size, MBSWIDTH_FLAGS);
      برای (صحیح pad = size_width < 0 ? 0 : file_size_width - size_width;
           0 < pad; pad--)
        *p++ = ' ';
      وقتی ((*p++ = *size++))
        ادامه;
      p[-1] = ' ';
    }

  s = -1;

  اگر (f->stat_ok && btime_ok
      && localtime_rz (localtz, &when_timespec.tv_sec, &when_local))
    {
      ساختار timespec six_months_ago;
      bool recent;

      /* If the file appears to be in the future, update the current
         time, in case the file happens to have been modified since
         the last time we checked the clock.  */
      اگر (timespec_cmp (current_time, when_timespec) < 0)
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

  اگر (0 <= s)
    {
      p += s;
      *p++ = ' ';
    }
  وگرنه
    {
      /* The time cannot be converted using the desired format, so
         print it as a huge integer number of seconds.  */
      نویسه hbuf[INT_BUFSIZE_BOUND (intmax_t)];
      p += sprintf (p, "%*s ", long_time_expected_width (),
                    (! f->stat_ok || ! btime_ok
                     ? "?"
                     : timetostr (when_timespec.tv_sec, hbuf)));
      /* FIXME: (maybe) We discarded when_timespec.tv_nsec. */
    }

  dired_outbuf (buf, p - buf);
  size_t w = print_name_with_quoting (f, false, &dired_obstack, p - buf);

  اگر (f->filetype == symbolic_link)
    {
      اگر (f->linkname)
        {
          dired_outstring (" -> ");
          print_name_with_quoting (f, true, پوچ, (p - buf) + w + 4);
          اگر (indicator_style != none)
            print_type_indicator (true, f->linkmode, unknown);
        }
    }
  وگرنه اگر (indicator_style != none)
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

ایستا size_t
quote_name_buf (نویسه **inbuf, size_t bufsize, نویسه *name,
                ساختار quoting_options ثابت *options,
                صحیح needs_general_quoting, size_t *width, bool *pad)
{
  نویسه *buf = *inbuf;
  size_t displayed_width IF_LINT ( = 0);
  size_t len = 0;
  bool quoted;

  شمارش quoting_style qs = get_quoting_style (options);
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

  اگر (needs_general_quoting != 0)
    {
      len = quotearg_buffer (buf, bufsize, name, -1, options);
      اگر (bufsize <= len)
        {
          buf = xmalloc (len + 1);
          quotearg_buffer (buf, len + 1, name, -1, options);
        }

      quoted = (*name != *buf) || strlen (name) != len;
    }
  وگرنه اگر (needs_further_quoting || extra_newline_protection)
    {
      len = strlen (name);
      اگر (bufsize <= len)
        buf = xmalloc (len + 1);
      memcpy (buf, name, len + 1);

      quoted = false;
    }
  وگرنه
    {
      len = strlen (name);
      buf = name;
      quoted = false;
    }

  /* It's safe to replace newlines without multi-byte iteration, as newlines
     do not appear as part of any multi-byte encoded character.  */
  اگر (extra_newline_protection)
    برای (نویسه *p = buf; p < buf + len; p++)
      اگر (eolbyte && *p == '\n')
        *p = '?';

  اگر (needs_further_quoting)
    {
      اگر (MB_CUR_MAX > 1)
        {
          نویسه ثابت *p = buf;
          نویسه ثابت *plimit = buf + len;
          نویسه *q = buf;
          displayed_width = 0;

          وقتی (p < plimit)
            انتخاب (*p)
              {
                حالت ' ': حالت '!': حالت '"': حالت '#': حالت '%':
                حالت '&': حالت '\'': حالت '(': حالت ')': حالت '*':
                حالت '+': حالت ',': حالت '-': حالت '.': حالت '/':
                حالت '0': حالت '1': حالت '2': حالت '3': حالت '4':
                حالت '5': حالت '6': حالت '7': حالت '8': حالت '9':
                حالت ':': حالت ';': حالت '<': حالت '=': حالت '>':
                حالت '?':
                حالت 'A': حالت 'B': حالت 'C': حالت 'D': حالت 'E':
                حالت 'F': حالت 'G': حالت 'H': حالت 'I': حالت 'J':
                حالت 'K': حالت 'L': حالت 'M': حالت 'N': حالت 'O':
                حالت 'P': حالت 'Q': حالت 'R': حالت 'S': حالت 'T':
                حالت 'U': حالت 'V': حالت 'W': حالت 'X': حالت 'Y':
                حالت 'Z':
                حالت '[': حالت '\\': حالت ']': حالت '^': حالت '_':
                حالت 'a': حالت 'b': حالت 'c': حالت 'd': حالت 'e':
                حالت 'f': حالت 'g': حالت 'h': حالت 'i': حالت 'j':
                حالت 'k': حالت 'l': حالت 'm': حالت 'n': حالت 'o':
                حالت 'p': حالت 'q': حالت 'r': حالت 's': حالت 't':
                حالت 'u': حالت 'v': حالت 'w': حالت 'x': حالت 'y':
                حالت 'z': حالت '{': حالت '|': حالت '}': حالت '~':
                  /* These characters are printable ASCII characters.  */
                  *q++ = *p++;
                  displayed_width += 1;
                  بشکن;
                پیشفرض:
                  /* If we have a multibyte sequence, copy it until we
                     reach its end, replacing each non-printable multibyte
                     character with a single question mark.  */
                  {
                    mbstate_t mbstate; mbszero (&mbstate);
                    انجامبده
                      {
                        char32_t wc;
                        size_t bytes;
                        صحیح w;

                        bytes = mbrtoc32 (&wc, p, plimit - p, &mbstate);

                        اگر (bytes == (size_t) -1)
                          {
                            /* An invalid multibyte sequence was
                               encountered.  Skip one input byte, and
                               put a question mark.  */
                            p++;
                            *q++ = '?';
                            displayed_width += 1;
                            بشکن;
                          }

                        اگر (bytes == (size_t) -2)
                          {
                            /* An incomplete multibyte character
                               at the end.  Replace it entirely with
                               a question mark.  */
                            p = plimit;
                            *q++ = '?';
                            displayed_width += 1;
                            بشکن;
                          }

                        اگر (bytes == 0)
                          /* A null wide character was encountered.  */
                          bytes = 1;

                        w = c32width (wc);
                        اگر (w >= 0)
                          {
                            /* A printable multibyte character.
                               Keep it.  */
                            برای (; bytes > 0; --bytes)
                              *q++ = *p++;
                            displayed_width += w;
                          }
                        وگرنه
                          {
                            /* An nonprintable multibyte character.
                               Replace it entirely with a question
                               mark.  */
                            p += bytes;
                            *q++ = '?';
                            displayed_width += 1;
                          }
                      }
                    وقتی (! mbsinit (&mbstate));
                  }
                  بشکن;
              }

          /* The buffer may have shrunk.  */
          len = q - buf;
        }
      وگرنه
        {
          نویسه *p = buf;
          نویسه ثابت *plimit = buf + len;

          وقتی (p < plimit)
            {
              اگر (! isprint (to_uchar (*p)))
                *p = '?';
              p++;
            }
          displayed_width = len;
        }
    }
  وگرنه اگر (width != پوچ)
    {
      اگر (MB_CUR_MAX > 1)
        {
          displayed_width = mbsnwidth (buf, len, MBSWIDTH_FLAGS);
          displayed_width = MAX (0, displayed_width);
        }
      وگرنه
        {
          نویسه ثابت *p = buf;
          نویسه ثابت *plimit = buf + len;

          displayed_width = 0;
          وقتی (p < plimit)
            {
              اگر (isprint (to_uchar (*p)))
                displayed_width++;
              p++;
            }
        }
    }

  /* Set padding to better align quoted items,
     and also give a visual indication that quotes are
     not actually part of the name.  */
  *pad = (align_variable_outer_quotes && cwd_some_quoted && ! quoted);

  اگر (width != پوچ)
    *width = displayed_width;

  *inbuf = buf;

  بازگشت len;
}

ایستا size_t
quote_name_width (نویسه ثابت *name, ساختار quoting_options ثابت *options,
                  صحیح needs_general_quoting)
{
  نویسه smallbuf[BUFSIZ];
  نویسه *buf = smallbuf;
  size_t width;
  bool pad;

  quote_name_buf (&buf, اندازه smallbuf, (نویسه *) name, options,
                  needs_general_quoting, &width, &pad);

  اگر (buf != smallbuf && buf != name)
    آزاد (buf);

  width += pad;

  بازگشت width;
}

/* %XX escape any input out of range as defined in RFC3986,
   and also if PATH, convert all path separators to '/'.  */
ایستا نویسه *
file_escape (نویسه ثابت *str, bool path)
{
  نویسه *esc = xnmalloc (3, strlen (str) + 1);
  نویسه *p = esc;
  وقتی (*str)
    {
      اگر (path && ISSLASH (*str))
        {
          *p++ = '/';
          str++;
        }
      وگرنه اگر (RFC3986[to_uchar (*str)])
        *p++ = *str++;
      وگرنه
        p += sprintf (p, "%%%02x", to_uchar (*str++));
    }
  *p = '\0';
  بازگشت esc;
}

ایستا size_t
quote_name (نویسه ثابت *name, ساختار quoting_options ثابت *options,
            صحیح needs_general_quoting, ثابت ساختار bin_str *color,
            bool allow_pad, ساختار obstack *پشته, نویسه ثابت *absolute_name)
{
  نویسه smallbuf[BUFSIZ];
  نویسه *buf = smallbuf;
  size_t len;
  bool pad;

  len = quote_name_buf (&buf, اندازه smallbuf, (نویسه *) name, options,
                        needs_general_quoting, پوچ, &pad);

  اگر (pad && allow_pad)
    dired_outbyte (' ');

  اگر (color)
    print_color_indicator (color);

  /* If we're padding, then don't include the outer quotes in
     the --hyperlink, to improve the alignment of those links.  */
  bool skip_quotes = false;

  اگر (absolute_name)
    {
      اگر (align_variable_outer_quotes && cwd_some_quoted && ! pad)
        {
          skip_quotes = true;
          putchar (*buf);
        }
      نویسه *h = file_escape (hostname, /* path= */ false);
      نویسه *n = file_escape (absolute_name, /* path= */ true);
      /* TODO: It would be good to be able to define parameters
         to give hints to the terminal as how best to render the URI.
         For example since ls is outputting a dense block of URIs
         it would be best to not underline by default, and only
         do so upon hover etc.  */
      printf ("\033]8;;file://%s%s%s\033\\", h, *n == '/' ? "" : "/", n);
      آزاد (h);
      آزاد (n);
    }

  اگر (پشته)
    افزودن_current_dired_pos (پشته);

  fwrite (buf + skip_quotes, 1, len - (skip_quotes * 2), stdout);

  dired_pos += len;

  اگر (پشته)
    افزودن_current_dired_pos (پشته);

  اگر (absolute_name)
    {
      fputs ("\033]8;;\033\\", stdout);
      اگر (skip_quotes)
        putchar (*(buf + len - 1));
    }

  اگر (buf != smallbuf && buf != name)
    آزاد (buf);

  بازگشت len + pad;
}

ایستا size_t
print_name_with_quoting (ثابت ساختار fileinfo *f,
                         bool symlink_target,
                         ساختار obstack *پشته,
                         size_t start_col)
{
  نویسه ثابت *name = symlink_target ? f->linkname : f->name;

  ثابت ساختار bin_str *color
    = print_with_color ? get_color_indicator (f, symlink_target) : پوچ;

  bool used_color_this_time = (print_with_color
                               && (color || is_colored (C_NORM)));

  size_t len = quote_name (name, filename_quoting_options, f->quoted,
                           color, !symlink_target, پشته, f->absolute_name);

  process_signals ();
  اگر (used_color_this_time)
    {
      prep_non_filename_text ();

      /* We use the byte length rather than display width here as
         an optimization to avoid accurately calculating the width,
         because we only output the clear to EOL sequence if the name
         _might_ wrap to the next line.  This may output a sequence
         unnecessarily in multi-byte locales for example,
         but in that case it's inconsequential to the output.  */
      اگر (سطر_length
          && (start_col / سطر_length != (start_col + len - 1) / سطر_length))
        put_indicator (&color_indicator[C_CLR_TO_EOL]);
    }

  بازگشت len;
}

ایستا تهی
prep_non_filename_text (تهی)
{
  اگر (color_indicator[C_END].string != پوچ)
    put_indicator (&color_indicator[C_END]);
  وگرنه
    {
      put_indicator (&color_indicator[C_چپ]);
      put_indicator (&color_indicator[C_RESET]);
      put_indicator (&color_indicator[C_راست]);
    }
}

/* Print the file name of 'f' with appropriate quoting.
   Also print file size, inode number, and filetype indicator character,
   as requested by switches.  */

ایستا size_t
print_file_name_and_frills (ثابت ساختار fileinfo *f, size_t start_col)
{
  نویسه buf[MAX (LONGEST_HUMAN_READABLE + 1, INT_BUFSIZE_BOUND (uintmax_t))];

  set_normal_color ();

  اگر (print_inode)
    printf ("%*s ", format == with_commas ? 0 : inode_عدد_width,
            format_inode (buf, f));

  اگر (print_block_size)
    {
      نویسه ثابت *blocks =
        (! f->stat_ok
         ? "?"
         : human_readable (STP_NBLOCKS (&f->stat), buf, human_output_opts,
                           ST_NBLOCKSIZE, output_block_size));
      صحیح blocks_width = mbswidth (blocks, MBSWIDTH_FLAGS);
      صحیح pad = 0;
      اگر (0 <= blocks_width && block_size_width && format != with_commas)
        pad = block_size_width - blocks_width;
      printf ("%*s%s ", pad, "", blocks);
    }

  اگر (print_scontext)
    printf ("%*s ", format == with_commas ? 0 : scontext_width, f->scontext);

  size_t width = print_name_with_quoting (f, false, پوچ, start_col);

  اگر (indicator_style != none)
    width += print_type_indicator (f->stat_ok, f->stat.st_mode, f->filetype);

  بازگشت width;
}

/* Given these arguments describing a file, return the single-byte
   type indicator, or 0.  */
ایستا نویسه
get_type_indicator (bool stat_ok, mode_t mode, شمارش filetype type)
{
  نویسه c;

  اگر (stat_ok ? S_ISREG (mode) : type == normal)
    {
      اگر (stat_ok && indicator_style == classify && (mode & S_IXUGO))
        c = '*';
      وگرنه
        c = 0;
    }
  وگرنه
    {
      اگر (stat_ok ? S_ISDIR (mode) : type == directory || type == arg_directory)
        c = '/';
      وگرنه اگر (indicator_style == slash)
        c = 0;
      وگرنه اگر (stat_ok ? S_ISLNK (mode) : type == symbolic_link)
        c = '@';
      وگرنه اگر (stat_ok ? S_ISFIFO (mode) : type == fifo)
        c = '|';
      وگرنه اگر (stat_ok ? S_ISSOCK (mode) : type == sock)
        c = '=';
      وگرنه اگر (stat_ok && S_ISDOOR (mode))
        c = '>';
      وگرنه
        c = 0;
    }
  بازگشت c;
}

ایستا bool
print_type_indicator (bool stat_ok, mode_t mode, شمارش filetype type)
{
  نویسه c = get_type_indicator (stat_ok, mode, type);
  اگر (c)
    dired_outbyte (c);
  بازگشت !!c;
}

/* Returns if color sequence was printed.  */
ایستا bool
print_color_indicator (ثابت ساختار bin_str *ind)
{
  اگر (ind)
    {
      /* Need to reset so not dealing with attribute combinations */
      اگر (is_colored (C_NORM))
        restore_default_color ();
      put_indicator (&color_indicator[C_چپ]);
      put_indicator (ind);
      put_indicator (&color_indicator[C_راست]);
    }

  بازگشت ind != پوچ;
}

/* Returns color indicator or NULL if none.  */
ATTRIBUTE_PURE
ایستا ثابت ساختار bin_str *
get_color_indicator (ثابت ساختار fileinfo *f, bool symlink_target)
{
  شمارش indicator_no type;
  ساختار color_ext_type *ext;	/* Color extension */
  size_t len;			/* Length of name */

  نویسه ثابت *name;
  mode_t mode;
  صحیح linkok;
  اگر (symlink_target)
    {
      name = f->linkname;
      mode = f->linkmode;
      linkok = f->linkok ? 0 : -1;
    }
  وگرنه
    {
      name = f->name;
      mode = file_or_link_mode (f);
      linkok = f->linkok;
    }

  /* Is this a nonexistent file?  If so, linkok == -1.  */

  اگر (linkok == -1 && is_colored (C_MISSING))
    type = C_MISSING;
  وگرنه اگر (!f->stat_ok)
    {
      ایستا شمارش indicator_no ثابت filetype_indicator[] =
        {
          C_ORPHAN, C_FIFO, C_CHR, C_DIR, C_BLK, C_FILE,
          C_LINK, C_SOCK, C_FILE, C_DIR
        };
      static_assert (countof (filetype_indicator) == filetype_cardinality);
      type = filetype_indicator[f->filetype];
    }
  وگرنه
    {
      اگر (S_ISREG (mode))
        {
          type = C_FILE;

          اگر ((mode & S_ISUID) != 0 && is_colored (C_SETUID))
            type = C_SETUID;
          وگرنه اگر ((mode & S_ISGID) != 0 && is_colored (C_SETGID))
            type = C_SETGID;
          وگرنه اگر (f->has_capability)
            type = C_CAP;
          وگرنه اگر ((mode & S_IXUGO) != 0 && is_colored (C_EXEC))
            type = C_EXEC;
          وگرنه اگر ((1 < f->stat.st_nlink) && is_colored (C_MULTIHARDLINK))
            type = C_MULTIHARDLINK;
        }
      وگرنه اگر (S_ISDIR (mode))
        {
          type = C_DIR;

          اگر ((mode & S_ISVTX) && (mode & S_IWOTH)
              && is_colored (C_STICKY_OTHER_WRITABLE))
            type = C_STICKY_OTHER_WRITABLE;
          وگرنه اگر ((mode & S_IWOTH) != 0 && is_colored (C_OTHER_WRITABLE))
            type = C_OTHER_WRITABLE;
          وگرنه اگر ((mode & S_ISVTX) != 0 && is_colored (C_STICKY))
            type = C_STICKY;
        }
      وگرنه اگر (S_ISLNK (mode))
        type = C_LINK;
      وگرنه اگر (S_ISFIFO (mode))
        type = C_FIFO;
      وگرنه اگر (S_ISSOCK (mode))
        type = C_SOCK;
      وگرنه اگر (S_ISBLK (mode))
        type = C_BLK;
      وگرنه اگر (S_ISCHR (mode))
        type = C_CHR;
      وگرنه اگر (S_ISDOOR (mode))
        type = C_DOOR;
      وگرنه
        {
          /* Classify a file of some other type as C_ORPHAN.  */
          type = C_ORPHAN;
        }
    }

  /* Check the file's suffix only if still classified as C_FILE.  */
  ext = پوچ;
  اگر (type == C_FILE)
    {
      /* Test if NAME has a recognized suffix.  */

      len = strlen (name);
      name += len;		/* Pointer to final \0.  */
      برای (ext = color_ext_list; ext != پوچ; ext = ext->next)
        {
          اگر (ext->ext.len <= len)
            {
              اگر (ext->exact_match)
                {
                  اگر (STREQ_LEN (name - ext->ext.len, ext->ext.string,
                                 ext->ext.len))
                    بشکن;
                }
              وگرنه
                {
                  اگر (c_strncasecmp (name - ext->ext.len, ext->ext.string,
                                     ext->ext.len) == 0)
                    بشکن;
                }
            }
        }
    }

  /* Adjust the color for orphaned symlinks.  */
  اگر (type == C_LINK && !linkok)
    {
      اگر (color_symlink_as_referent || is_colored (C_ORPHAN))
        type = C_ORPHAN;
    }

  ثابت ساختار bin_str *ثابت s
    = ext ? &(ext->seq) : &color_indicator[type];

  بازگشت s->string ? s : پوچ;
}

/* Output a color indicator (which may contain nulls).
   Note we don't update dired_pos here as emacs can be configured
   to handle color codes by stripping/converting them.  */
ایستا تهی
put_indicator (ثابت ساختار bin_str *ind)
{
  اگر (! used_color)
    {
      used_color = true;

      /* If the standard output is a controlling terminal, watch out
         for signals, so that the colors can be restored to the
         default state if "ls" is suspended or interrupted.  */

      اگر (0 <= tcgetpgrp (STDOUT_FILENO))
        signal_init ();

      prep_non_filename_text ();
    }

  fwrite (ind->string, ind->len, 1, stdout);
}

ایستا size_t
length_of_file_name_and_frills (ثابت ساختار fileinfo *f)
{
  size_t len = 0;
  نویسه buf[MAX (LONGEST_HUMAN_READABLE + 1, INT_BUFSIZE_BOUND (uintmax_t))];

  اگر (print_inode)
    len += 1 + (format == with_commas
                ? strlen (umaxtostr (f->stat.st_ino, buf))
                : inode_عدد_width);

  اگر (print_block_size)
    len += 1 + (format == with_commas
                ? strlen (! f->stat_ok ? "?"
                          : human_readable (STP_NBLOCKS (&f->stat), buf,
                                            human_output_opts, ST_NBLOCKSIZE,
                                            output_block_size))
                : block_size_width);

  اگر (print_scontext)
    len += 1 + (format == with_commas ? strlen (f->scontext) : scontext_width);

  len += fileinfo_name_width (f);

  اگر (indicator_style != none)
    {
      نویسه c = get_type_indicator (f->stat_ok, f->stat.st_mode, f->filetype);
      len += (c != 0);
    }

  بازگشت len;
}

ایستا تهی
print_many_per_سطر (تهی)
{
  idx_t cols = calculate_columns (true);
  ساختار column_info ثابت *سطر_fmt = &column_info[cols - 1];

  /* Calculate the number of rows that will be in each column except possibly
     for a short column on the right.  */
  idx_t rows = cwd_n_used / cols + (cwd_n_used % cols != 0);

  برای (idx_t row = 0; row < rows; row++)
    {
      size_t col = 0;
      idx_t filesno = row;
      size_t pos = 0;

      /* Print the next row.  */
      وقتی (true)
        {
          ساختار fileinfo ثابت *f = sorted_file[filesno];
          size_t name_length = length_of_file_name_and_frills (f);
          size_t max_name_length = سطر_fmt->col_arr[col++];
          print_file_name_and_frills (f, pos);

          اگر (cwd_n_used - rows <= filesno)
            بشکن;
          filesno += rows;

          indent (pos + name_length, pos + max_name_length);
          pos += max_name_length;
        }
      putchar (eolbyte);
    }
}

ایستا تهی
print_horizontal (تهی)
{
  size_t pos = 0;
  idx_t cols = calculate_columns (false);
  ساختار column_info ثابت *سطر_fmt = &column_info[cols - 1];
  ساختار fileinfo ثابت *f = sorted_file[0];
  size_t name_length = length_of_file_name_and_frills (f);
  size_t max_name_length = سطر_fmt->col_arr[0];

  /* Print first entry.  */
  print_file_name_and_frills (f, 0);

  /* Now the rest.  */
  برای (idx_t filesno = 1; filesno < cwd_n_used; filesno++)
    {
      idx_t col = filesno % cols;

      اگر (col == 0)
        {
          putchar (eolbyte);
          pos = 0;
        }
      وگرنه
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

ایستا تهی
print_with_separator (نویسه sep)
{
  size_t pos = 0;

  برای (idx_t filesno = 0; filesno < cwd_n_used; filesno++)
    {
      ساختار fileinfo ثابت *f = sorted_file[filesno];
      size_t len = سطر_length ? length_of_file_name_and_frills (f) : 0;

      اگر (filesno != 0)
        {
          نویسه separator;
          size_t next_pos = 2 + (sep == ',' && filesno < cwd_n_used - 1);

          اگر (! سطر_length
              || (! ckd_add (&next_pos, next_pos, pos)
                  && ! ckd_add (&next_pos, next_pos, len)
                  && next_pos <= سطر_length))
            {
              pos += 2;
              separator = ' ';
            }
          وگرنه
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

ایستا تهی
indent (size_t from, size_t to)
{
  وقتی (from < to)
    {
      اگر (tabsize != 0 && to / tabsize > (from + 1) / tabsize)
        {
          putchar ('\t');
          from += tabsize - from % tabsize;
        }
      وگرنه
        {
          putchar (' ');
          from++;
        }
    }
}

/* Put DIRNAME/NAME into DEST, handling '.' and '/' properly.  */
/* FIXME: maybe remove this function someday.  See about using a
   non-malloc'ing version of file_name_concat.  */

ایستا تهی
attach (نویسه *dest, نویسه ثابت *dirname, نویسه ثابت *name)
{
  نویسه ثابت *dirnamep = dirname;

  /* Copy dirname if it is not ".".  */
  اگر (dirname[0] != '.' || dirname[1] != 0)
    {
      وقتی (*dirnamep)
        *dest++ = *dirnamep++;
      /* Add '/' if 'dirname' doesn't already end with it.  */
      اگر (dirnamep > dirname && dirnamep[-1] != '/')
        *dest++ = '/';
    }
  وقتی (*name)
    *dest++ = *name++;
  *dest = 0;
}

/* Allocate enough column info suitable for the current number of
   files and display columns, and initialize the info to represent the
   narrowest possible columns.  */

ایستا تهی
init_column_info (idx_t max_cols)
{
  /* Currently allocated columns in column_info.  */
  ایستا idx_t column_info_alloc;

  اگر (column_info_alloc < max_cols)
    {
      idx_t old_column_info_alloc = column_info_alloc;
      column_info = xpalloc (column_info, &column_info_alloc,
                             max_cols - column_info_alloc, -1,
                             اندازه *column_info);

      /* Allocate the new size_t objects by computing the triangle
         formula n * (n + 1) / 2, except that we don't need to
         allocate the part of the triangle that we've already
         allocated.  Check for address arithmetic overflow.  */
      idx_t column_info_growth = column_info_alloc - old_column_info_alloc, s;
      اگر (ckd_add (&s, old_column_info_alloc + 1, column_info_alloc)
          || ckd_mul (&s, s, column_info_growth))
        xalloc_die ();
      size_t *p = xinmalloc (s >> 1, اندازه *p);

      /* Grow the triangle by parceling out the cells just allocated.  */
      برای (idx_t i = old_column_info_alloc; i < column_info_alloc; i++)
        {
          column_info[i].col_arr = p;
          p += i + 1;
        }
    }

  برای (idx_t i = 0; i < max_cols; ++i)
    {
      column_info[i].valid_len = true;
      column_info[i].سطر_len = i * MIN_COLUMN_WIDTH + 1;
      برای (idx_t j = 0; j < i; ++j)
        column_info[i].col_arr[j] = MIN_COLUMN_WIDTH;
      column_info[i].col_arr[i] = 1;
    }
}

/* Calculate the number of columns needed to represent the current set
   of files in the current display width.  */

ایستا idx_t
calculate_columns (bool by_columns)
{
  /* Normally the maximum number of columns is determined by the
     screen width.  But if few files are available this might limit it
     as well.  */
  idx_t max_cols = 0 < max_idx && max_idx < cwd_n_used ? max_idx : cwd_n_used;

  init_column_info (max_cols);

  /* Compute the maximum number of possible columns.  */
  برای (idx_t filesno = 0; filesno < cwd_n_used; ++filesno)
    {
      ساختار fileinfo ثابت *f = sorted_file[filesno];
      size_t name_length = length_of_file_name_and_frills (f);

      برای (idx_t i = 0; i < max_cols; ++i)
        {
          اگر (column_info[i].valid_len)
            {
              idx_t idx = (by_columns
                           ? filesno / ((cwd_n_used + i) / (i + 1))
                           : filesno % (i + 1));
              size_t real_length = name_length + (idx == i ? 0 : 2);

              اگر (column_info[i].col_arr[idx] < real_length)
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
  برای (cols = max_cols; 1 < cols; --cols)
    {
      اگر (column_info[cols - 1].valid_len)
        بشکن;
    }

  بازگشت cols;
}

تهی
usage (صحیح status)
{
  اگر (status != EXIT_SUCCESS)
    emit_try_help ();
  وگرنه
    {
      printf (_("Usage: %s [OPTION]... [FILE]...\n"), program_name);
      fputs (_("\
List information about the FILEs (the current directory by پیشفرض).\n\
Sort entries alphabetically اگر none of -cftuvSUX nor --sort is specified.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -a, --all\n\
         انجامبده not ignore entries starting with .\n\
"));
      oputs (_("\
  -A, --almost-all\n\
         انجامبده not list implied . and ..\n\
"));
      oputs (_("\
      --author\n\
         with -l, print the author of each file\n\
"));
      oputs (_("\
  -b, --escape\n\
         print C-style escapes برای nongraphic characters\n\
"));
      oputs (_("\
      --block-size=SIZE\n\
         with -l, scale sizes by SIZE when printing them;\n\
         e.g., '--block-size=M'; see SIZE format below\n\
"));
      oputs (_("\
  -B, --ignore-backups\n\
         انجامبده not list implied entries ending with ~\n\
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
         generate output designed برای Emacs' dired mode\n\
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
         like -F, except انجامبده not append '*'\n\
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
         like -l, but انجامبده not list owner\n\
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
         انجامبده not list implied entries matching shell PATTERN\n\
         (overridden by -a or -A)\n\
"));
      oputs (_("\
      --hyperlink[=WHEN]\n\
         hyperlink file names WHEN\n\
"));
      oputs (_("\
      --indicator-style=WORD\n\
         append indicator with style WORD to entry names:\n\
           none (پیشفرض), slash (-p), file-type (--file-type), classify (-F)\n\
"));
      oputs (_("\
  -i, --inode\n\
         print the index عدد of each file\n\
"));
      oputs (_("\
  -I, --ignore=PATTERN\n\
         انجامبده not list implied entries matching shell PATTERN\n\
"));
      oputs (_("\
  -k, --kibibytes\n\
         پیشفرض to 1024-byte blocks برای file system usage;\n\
         used only with -s and per directory totals\n\
"));
      oputs (_("\
  -l\n\
         use a long listing format\n\
"));
      oputs (_("\
  -L, --dereference\n\
         when showing file information برای a symbolic link,\n\
         show information برای the file the link references\n\
         rather than برای the link itself\n\
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
         like -l, but انجامبده not list group information\n\
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
         the پیشفرض, unless program is 'ls' and output is a terminal\n\
"));
      oputs (_("\
  -Q, --quote-name\n\
         enclose entry names in دوگانه quotes\n\
"));
      oputs (_("\
      --quoting-style=WORD\n\
         use quoting style WORD برای entry names:\n\
           literal, locale, shell, shell-always,\n\
           shell-escape, shell-escape-always, c, escape\n\
         (overrides QUOTING_STYLE environment variable)\n\
"));
      oputs (_("\
  -r, --reverse\n\
         reverse order وقتی sorting\n\
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
         change پیشفرض 'name' sort to WORD:\n\
           none (-U), size (-S), time (-t),\n\
           version (-v), extension (-X), name, width\n\
"));
      oputs (_("\
      --time=WORD\n\
         select which timestamp used to display or sort;\n\
           access time (-u): atime, access, use;\n\
           metadata change time (-c): ctime, status;\n\
           modified time (پیشفرض): mtime, modification;\n\
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
         انجامبده not sort directory entries\n\
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
         list entries by سطرها instead of by columns\n\
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
Also the TIME_STYLE environment variable sets the پیشفرض style to use.\n\
"), stdout);
      fputs (_("\
\n\
The WHEN argument defaults to 'always' and can also be 'auto' or 'never'.\n\
"), stdout);
      fputs (_("\
\n\
Using color to distinguish file types is disabled both by پیشفرض and\n\
with --color=never.  With --color=auto, ls emits color codes only when\n\
standard output is connected to a terminal.  The LS_COLORS environment\n\
variable can change the settings.  Use the dircolors(1) command to set it.\n\
"), stdout);
      fputs (_("\
\n\
Exit status:\n\
 0  اگر OK,\n\
 1  اگر minor problems (e.g., cannot access subdirectory),\n\
 2  اگر serious trouble (e.g., cannot access command-سطر argument).\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}
