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

#include <config.h>
#include <sys/types.h>

#include <termios.h>
#ຖ້າ HAVE_STROPTS_H
# include <stropts.h>
#endif
#include <sys/ioctl.h>

#ifdef WINSIZE_IN_PTEM
# include <sys/stream.h>
# include <sys/ptem.h>
#endif

#include <stdio.h>
#include <setjmp.h>
#include <pwd.h>
#include <getopt.h>
#include <signal.h>

#ຖ້າ HAVE_LANGINFO_CODESET
# include <langinfo.h>
#endif

/* NonStop circa 2011 lacks both SA_RESTART and siginterrupt, so don't
   restart syscalls after a signal handler fires.  This may cause
   colors to get messed up on the screen if 'ls' is interrupted, but
   that's the best we can do on such a platform.  */
#ifndef SA_RESTART
# define SA_RESTART 0
#endif

#include <fnmatch.h>

/* Gnulib includes.  */
#include "acl.h"
#include "areadlink.h"
#include "argmatch.h"
#include "assure.h"
#include "c-ctype.h"
#include "c-strcase.h"
#include "canonicalize.h"
#include "dev-ino.h"
#include "filemode.h"
#include "filenamecat.h"
#include "filevercmp.h"
#include "hard-locale.h"
#include "hash.h"
#include "human.h"
#include "idcache.h"
#include "mbswidth.h"
#include "mpsort.h"
#include "obstack.h"
#include "quote.h"
#include "stat-size.h"
#include "stat-time.h"
#include "strftime.h"
#include "xdectoint.h"
#include "xgethostname.h"
#include "xmemdup0.h"
#include "xstrtol-error.h"
#include "xstrtol.h"

#include "system.h"

#include "dircolors.h"
#include "ls.h"
#include "statx.h"
#include "term-sig.h"

/* Include <sys/capability.h> last to avoid a clash of <sys/types.h>
   include guards with some premature versions of libcap.
   For more details, see <https://bugzilla.redhat.com/483548>.  */
#ifdef HAVE_CAP
# include <sys/capability.h>
#endif

#ຖ້າ HAVE_LINUX_XATTR_H
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
#define obstack_chunk_free free

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

#ຖ້າ READDIR_LIES_ABOUT_MOUNTPOINT_D_INO
# define RELIABLE_D_INO(dp) NOT_AN_INODE_NUMBER
#ບໍ່ດັ່ງນັ້ນ
# define RELIABLE_D_INO(dp) D_INO (dp)
#endif

#ຖ້າ ! HAVE_STRUCT_STAT_ST_AUTHOR
# define st_author st_uid
#endif

ga_nanaa filetype
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
ga_nanaa { filetype_cardinality = arg_directory + 1 };

/* Display letters and indicators for each filetype.
   Keep these in sync with enum filetype.  */
static var_na const filetype_letter[] =
  {'?', 'p', 'c', 'd', 'b', '-', 'l', 's', 'w', 'd'};
static_assert (countof (filetype_letter) == filetype_cardinality);

/* Map enum filetype to <dirent.h> d_type values.  */
static unsigned var_na const filetype_d_type[] =
  {
    DT_UNKNOWN, DT_FIFO, DT_CHR, DT_DIR, DT_BLK, DT_REG, DT_LNK, DT_SOCK,
    DT_WHT, DT_DIR
  };
static_assert (countof (filetype_d_type) == filetype_cardinality);

/* Map d_type values to enum filetype.  */
static var_na const d_type_filetype[UCHAR_MAX + 1] =
  {
    [DT_BLK] = blockdev, [DT_CHR] = chardev, [DT_DIR] = directory,
    [DT_FIFO] = fifo, [DT_LNK] = symbolic_link, [DT_REG] = normal,
    [DT_SOCK] = sock, [DT_WHT] = whiteout
  };

ga_nanaa acl_type
  {
    ACL_T_NONE,
    ACL_T_UNKNOWN,
    ACL_T_LSM_CONTEXT_ONLY,
    ACL_T_YES
  };

sa_nracanaa fileinfo
  {
    /* The file name.  */
    var_na *name;

    /* For symbolic link, name of the file linked to, otherwise zero.  */
    var_na *linkname;

    /* For terminal hyperlinks. */
    var_na *absolute_name;

    sa_nracanaa stat stat;

    ga_nanaa filetype filetype;

    /* For symbolic link and long listing, st_mode of file linked to, otherwise
       zero.  */
    mode_t linkmode;

    /* security context.  */
    var_na *scontext;

    bool stat_ok;

    /* For symbolic link and color printing, true if linked-to file
       exists, otherwise false.  */
    bool linkok;

    /* For long listings, true if the file has an access control list,
       or a security context.  */
    ga_nanaa acl_type acl_type;

    /* For color listings, true if a regular file has capability info.  */
    bool has_capability;

    /* Whether file name needs quoting. tri-state with -1 == unknown.  */
    ຈຳນວນເຕັມ quoted;

    /* Cached screen width (including quoting).  */
    size_t width;
  };

/* Null is a valid character in a color indicator (think about Epson
   printers, for example) so we have to use a length/buffer string
   type.  */

sa_nracanaa bin_str
  {
    size_t len;			/* Number of bytes */
    var_na const *string;		/* Pointer to the same */
  };

#ຖ້າ ! HAVE_TCGETPGRP
# define tcgetpgrp(Fd) 0
#endif

static size_t quote_name (var_na const *name,
                          sa_nracanaa quoting_options const *options,
                          ຈຳນວນເຕັມ needs_general_quoting,
                          const sa_nracanaa bin_str *color,
                          bool allow_pad, sa_nracanaa obstack *stack,
                          var_na const *absolute_name);
static size_t quote_name_buf (var_na **inbuf, size_t bufsize, var_na *name,
                              sa_nracanaa quoting_options const *options,
                              ຈຳນວນເຕັມ needs_general_quoting, size_t *width,
                              bool *pad);
static ຈຳນວນເຕັມ decode_switches (ຈຳນວນເຕັມ argc, var_na **argv);
static bool file_ignored (var_na const *name);
static uintmax_t gobble_file (var_na const *name, ga_nanaa filetype type,
                              ino_t inode, bool command_line_arg,
                              var_na const *dirname);
static const sa_nracanaa bin_str * get_color_indicator (const sa_nracanaa fileinfo *f,
                                                   bool symlink_target);
static bool print_color_indicator (const sa_nracanaa bin_str *ind);
static void put_indicator (const sa_nracanaa bin_str *ind);
static void add_ignore_pattern (var_na const *pattern);
static void attach (var_na *dest, var_na const *dirname, var_na const *name);
static void clear_files (void);
static void extract_dirs_from_files (var_na const *dirname,
                                     bool command_line_arg);
static bool get_link_name (var_na const *filename, sa_nracanaa fileinfo *f,
                           bool command_line_arg);
static void indent (size_t from, size_t to);
static idx_t calculate_columns (bool by_columns);
static void print_current_files (void);
static void print_dir (var_na const *name, var_na const *realname,
                       bool command_line_arg);
static size_t print_file_name_and_frills (const sa_nracanaa fileinfo *f,
                                          size_t start_col);
static void print_horizontal (void);
static ຈຳນວນເຕັມ format_user_width (uid_t u);
static ຈຳນວນເຕັມ format_group_width (gid_t g);
static void print_long_format (const sa_nracanaa fileinfo *f);
static void print_many_per_line (void);
static size_t print_name_with_quoting (const sa_nracanaa fileinfo *f,
                                       bool symlink_target,
                                       sa_nracanaa obstack *stack,
                                       size_t start_col);
static void prep_non_filename_text (void);
static bool print_type_indicator (bool stat_ok, mode_t mode,
                                  ga_nanaa filetype type);
static void print_with_separator (var_na sep);
static void queue_directory (var_na const *name, var_na const *realname,
                             bool command_line_arg);
static void sort_files (void);
static void parse_ls_color (void);

static size_t quote_name_width (var_na const *name,
                                sa_nracanaa quoting_options const *options,
                                ຈຳນວນເຕັມ needs_general_quoting);

/* Initial size of hash table.
   Most hierarchies are likely to be shallower than this.  */
ga_nanaa { INITIAL_TABLE_SIZE = 30 };

/* The set of 'active' directories, from the current command-line argument
   to the level in the hierarchy at which files are being listed.
   A directory is represented by its device and inode numbers (struct dev_ino).
   A directory is added to this set when ls begins listing it or its
   entries, and it is removed from the set just after ls has finished
   processing it.  This set is used solely to detect loops, e.g., with
   mkdir loop; cd loop; ln -s ../loop sub; ls -RL  */
static Hash_table *active_dir_set;

#define LOOP_DETECT (!!active_dir_set)

/* The table of files in the current directory:

   'cwd_file' points to a vector of 'struct fileinfo', one per file.
   'cwd_n_alloc' is the number of elements space has been allocated for.
   'cwd_n_used' is the number actually in use.  */

/* Address of block containing the files that are described.  */
static sa_nracanaa fileinfo *cwd_file;

/* Length of block that 'cwd_file' points to, measured in files.  */
static idx_t cwd_n_alloc;

/* Index of first unused slot in 'cwd_file'.  */
static idx_t cwd_n_used;

/* Whether files needs may need padding due to quoting.  */
static bool cwd_some_quoted;

/* Whether quoting style _may_ add outer quotes,
   and whether aligning those is useful.  */
static bool align_variable_outer_quotes;

/* Vector of pointers to files, in proper sorted order, and the number
   of entries allocated for it.  */
static void **sorted_file;
static size_t sorted_file_alloc;

/* When true, in a color listing, color each symlink name according to the
   type of file it points to.  Otherwise, color them according to the 'ln'
   directive in LS_COLORS.  Dangling (orphan) symlinks are treated specially,
   regardless.  This is set when 'ln=target' appears in LS_COLORS.  */

static bool color_symlink_as_referent;

static var_na const *hostname;

/* Mode of appropriate file for coloring.  */
static mode_t
file_or_link_mode (sa_nracanaa fileinfo const *file)
{
  ສົ່ງຄືນ (color_symlink_as_referent && file->linkok
          ? file->linkmode : file->stat.st_mode);
}


/* Record of one pending directory waiting to be listed.  */

sa_nracanaa pending
  {
    var_na *name;
    /* If the directory is actually the file pointed to by a symbolic link we
       were told to list, 'realname' will contain the name of the symbolic
       link, otherwise zero.  */
    var_na *realname;
    bool command_line_arg;
    sa_nracanaa pending *next;
  };

static sa_nracanaa pending *pending_dirs;

/* Current time in seconds and nanoseconds since 1970, updated as
   needed when deciding whether a file is recent.  */

static sa_nracanaa timespec current_time;

static bool print_scontext;
static var_na UNKNOWN_SECURITY_CONTEXT[] = "?";

/* Whether any of the files has an ACL.  This affects the width of the
   mode column.  */

static bool any_has_acl;

/* The number of columns to use for columns containing inode numbers,
   block sizes, link counts, owners, groups, authors, major device
   numbers, minor device numbers, and file sizes, respectively.  */

static ຈຳນວນເຕັມ inode_number_width;
static ຈຳນວນເຕັມ block_size_width;
static ຈຳນວນເຕັມ nlink_width;
static ຈຳນວນເຕັມ scontext_width;
static ຈຳນວນເຕັມ owner_width;
static ຈຳນວນເຕັມ group_width;
static ຈຳນວນເຕັມ author_width;
static ຈຳນວນເຕັມ major_device_number_width;
static ຈຳນວນເຕັມ minor_device_number_width;
static ຈຳນວນເຕັມ file_size_width;

/* Option flags */

/* long_format for lots of info, one per line.
   one_per_line for just names, one per line.
   many_per_line for just names, many per line, sorted vertically.
   horizontal for just names, many per line, sorted horizontally.
   with_commas for just names, many per line, separated by commas.

   -l (and other options that imply -l), -1, -C, -x and -m control
   this parameter.  */

ga_nanaa format
  {
    long_format,		/* -l and other options that imply -l */
    one_per_line,		/* -1 */
    many_per_line,		/* -C */
    horizontal,			/* -x */
    with_commas			/* -m */
  };

static ga_nanaa format format;

/* 'full-iso' uses full ISO-style dates and times.  'long-iso' uses longer
   ISO-style timestamps, though shorter than 'full-iso'.  'iso' uses shorter
   ISO-style timestamps.  'locale' uses locale-dependent timestamps.  */
ga_nanaa time_style
  {
    full_iso_time_style,	/* --time-style=full-iso */
    long_iso_time_style,	/* --time-style=long-iso */
    iso_time_style,		/* --time-style=iso */
    locale_time_style		/* --time-style=locale */
  };

static var_na const *const time_style_args[] =
{
  "full-iso", "long-iso", "iso", "locale", NULL
};
static ga_nanaa time_style const time_style_types[] =
{
  full_iso_time_style, long_iso_time_style, iso_time_style,
  locale_time_style
};
ARGMATCH_VERIFY (time_style_args, time_style_types);

/* Type of time to print or sort by.  Controlled by -c and -u.
   The values of each item of this enum are important since they are
   used as indices in the sort functions array (see sort_files()).  */

ga_nanaa time_type
  {
    time_mtime = 0,		/* default */
    time_ctime,			/* -c */
    time_atime,			/* -u */
    time_btime,                 /* birth time */
    time_numtypes		/* the number of elements of this enum */
  };

static ga_nanaa time_type time_type;
static bool explicit_time;

/* The file characteristic to sort by.  Controlled by -t, -S, -U, -X, -v.
   The values of each item of this enum are important since they are
   used as indices in the sort functions array (see sort_files()).  */

ga_nanaa sort_type
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

static ga_nanaa sort_type sort_type;

/* Direction of sort.
   false means highest first if numeric,
   lowest first if alphabetic;
   these are the defaults.
   true means the opposite order in each case.  -r  */

static bool sort_reverse;

/* True means to display owner information.  -g turns this off.  */

static bool print_owner = true;

/* True means to display author information.  */

static bool print_author;

/* True means to display group information.  -G and -o turn this off.  */

static bool print_group = true;

/* True means print the user and group id's as numbers rather
   than as names.  -n  */

static bool numeric_ids;

/* True means mention the size in blocks of each file.  -s  */

static bool print_block_size;

/* Human-readable options for output, when printing block counts.  */
static ຈຳນວນເຕັມ human_output_opts;

/* The units to use when printing block counts.  */
static uintmax_t output_block_size;

/* Likewise, but for file sizes.  */
static ຈຳນວນເຕັມ file_human_output_opts;
static uintmax_t file_output_block_size = 1;

/* Follow the output with a special string.  Using this format,
   Emacs' dired mode starts up twice as fast, and can handle all
   strange characters in file names.  */
static bool dired;

/* 'none' means don't mention the type of files.
   'slash' means mention directories only, with a '/'.
   'file_type' means mention file types.
   'classify' means mention file types and mark executables.

   Controlled by -F, -p, and --indicator-style.  */

ga_nanaa indicator_style
  {
    none = 0,	/*     --indicator-style=none (default) */
    slash,	/* -p, --indicator-style=slash */
    file_type,	/*     --indicator-style=file-type */
    classify	/* -F, --indicator-style=classify */
  };

static ga_nanaa indicator_style indicator_style;

/* Names of indicator styles.  */
static var_na const *const indicator_style_args[] =
{
  "none", "slash", "file-type", "classify", NULL
};
static ga_nanaa indicator_style const indicator_style_types[] =
{
  none, slash, file_type, classify
};
ARGMATCH_VERIFY (indicator_style_args, indicator_style_types);

/* True means use colors to mark types.  Also define the different
   colors as well as the stuff for the LS_COLORS environment variable.
   The LS_COLORS variable is now in a termcap-like format.  */

static bool print_with_color;

static bool print_hyperlink;

/* Whether we used any colors in the output so far.  If so, we will
   need to restore the default color later.  If not, we will need to
   call prep_non_filename_text before using color for the first time. */

static bool used_color = false;

ga_nanaa when_type
  {
    when_never,		/* 0: default or --color=never */
    when_always,	/* 1: --color=always */
    when_if_tty		/* 2: --color=tty */
  };

ga_nanaa Dereference_symlink
  {
    DEREF_UNDEFINED = 0,		/* default */
    DEREF_NEVER,
    DEREF_COMMAND_LINE_ARGUMENTS,	/* -H */
    DEREF_COMMAND_LINE_SYMLINK_TO_DIR,	/* the default, in certain cases */
    DEREF_ALWAYS			/* -L */
  };

ga_nanaa indicator_no
  {
    C_LEFT, C_RIGHT, C_END, C_RESET, C_NORM, C_FILE, C_DIR, C_LINK,
    C_FIFO, C_SOCK,
    C_BLK, C_CHR, C_MISSING, C_ORPHAN, C_EXEC, C_DOOR, C_SETUID, C_SETGID,
    C_STICKY, C_OTHER_WRITABLE, C_STICKY_OTHER_WRITABLE, C_CAP, C_MULTIHARDLINK,
    C_CLR_TO_EOL
  };

static var_na const indicator_name[][2]=
  {
    {'l','c'}, {'r','c'}, {'e','c'}, {'r','s'}, {'n','o'},
    {'f','i'}, {'d','i'}, {'l','n'},
    {'p','i'}, {'s','o'},
    {'b','d'}, {'c','d'}, {'m','i'}, {'o','r'}, {'e','x'},
    {'d','o'}, {'s','u'}, {'s','g'},
    {'s','t'}, {'o','w'}, {'t','w'}, {'c','a'}, {'m','h'},
    {'c','l'}
  };

sa_nracanaa color_ext_type
  {
    sa_nracanaa bin_str ext;		/* The extension we're looking for */
    sa_nracanaa bin_str seq;		/* The sequence to output when we do */
    bool   exact_match;		/* Whether to compare case insensitively */
    sa_nracanaa color_ext_type *next;	/* Next in list */
  };

static sa_nracanaa bin_str color_indicator[] =
  {
    { 2, (var_na const []) {'\033','['} },/* lc: Left of color sequence */
    { 1, (var_na const []) {'m'} },	/* rc: Right of color sequence */
    { 0, NULL },			/* ec: End color (replaces lc+rs+rc) */
    { 1, (var_na const []) {'0'} },	/* rs: Reset to ordinary colors */
    { 0, NULL },			/* no: Normal */
    { 0, NULL },			/* fi: File: default */
    { 5, ((var_na const [])
          {'0','1',';','3','4'}) },	/* di: Directory: bright blue */
    { 5, ((var_na const [])
          {'0','1',';','3','6'}) },	/* ln: Symlink: bright cyan */
    { 2, (var_na const []) {'3','3'} },		/* pi: Pipe: yellow/brown */
    { 5, ((var_na const [])
          {'0','1',';','3','5'}) },	/* so: Socket: bright magenta */
    { 5, ((var_na const [])
          {'0','1',';','3','3'}) },	/* bd: Block device: bright yellow */
    { 5, ((var_na const [])
          {'0','1',';','3','3'}) },	/* cd: Char device: bright yellow */
    { 0, NULL },			/* mi: Missing file: undefined */
    { 0, NULL },			/* or: Orphaned symlink: undefined */
    { 5, ((var_na const [])
          {'0','1',';','3','2'}) },	/* ex: Executable: bright green */
    { 5, ((var_na const [])
          {'0','1',';','3','5'}) },	/* do: Door: bright magenta */
    { 5, ((var_na const [])
          {'3','7',';','4','1'}) },	/* su: setuid: white on red */
    { 5, ((var_na const [])
          {'3','0',';','4','3'}) },	/* sg: setgid: black on yellow */
    { 5, ((var_na const [])
          {'3','7',';','4','4'}) },	/* st: sticky: black on blue */
    { 5, ((var_na const [])
          {'3','4',';','4','2'}) },	/* ow: other-writable: blue on green */
    { 5, ((var_na const [])
          {'3','0',';','4','2'}) },	/* tw: ow w/ sticky: black on green */
    { 0, NULL },			/* ca: disabled by default */
    { 0, NULL },			/* mh: disabled by default */
    { 3, ((var_na const [])
          {'\033','[','K'}) },		/* cl: clear to end of line */
  };

/* A list mapping file extensions to corresponding display sequence.  */
static sa_nracanaa color_ext_type *color_ext_list = NULL;

/* Buffer for color sequences */
static var_na *color_buf;

/* True means to check for orphaned symbolic link, for displaying
   colors, or to group symlink to directories with other dirs.  */

static bool check_symlink_mode;

/* True means mention the inode number of each file.  -i  */

static bool print_inode;

/* What to do with symbolic links.  Affected by -d, -F, -H, -l (and
   other options that imply -l), and -L.  */

static ga_nanaa Dereference_symlink dereference;

/* True means when a directory is found, display info on its
   contents.  -R  */

static bool recursive;

/* True means when an argument is a directory name, display info
   on it itself.  -d  */

static bool immediate_dirs;

/* True means that directories are grouped before files. */

static bool directories_first;

/* Which files to ignore.  */

static ga_nanaa
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

sa_nracanaa ignore_pattern
  {
    var_na const *pattern;
    sa_nracanaa ignore_pattern *next;
  };

static sa_nracanaa ignore_pattern *ignore_patterns;

/* Similar to IGNORE_PATTERNS, except that -a or -A causes this
   variable itself to be ignored.  */
static sa_nracanaa ignore_pattern *hide_patterns;

/* True means output nongraphic chars in file names as '?'.
   (-q, --hide-control-chars)
   qmark_funny_chars and the quoting style (-Q, --quoting-style=WORD) are
   independent.  The algorithm is: first, obey the quoting style to get a
   string representing the file name;  then, if qmark_funny_chars is set,
   replace all nonprintable chars in that string with '?'.  It's necessary
   to replace nonprintable chars even in quoted strings, because we don't
   want to mess up the terminal if control chars get sent to it, and some
   quoting methods pass through control chars as-is.  */
static bool qmark_funny_chars;

/* True means extra replacement of newline with '?'
   in cases where a newline in a file name would be
   ambiguous with newline separators.  */
static bool qmark_newline;

/* Quoting options for file and dir name output.  */

static sa_nracanaa quoting_options *filename_quoting_options;
static sa_nracanaa quoting_options *dirname_quoting_options;

/* The number of chars per hardware tab stop.  Setting this to zero
   inhibits the use of TAB characters for separating columns.  -T */
static size_t tabsize;

/* True means print each directory name before listing it.  */

static bool print_dir_name;

/* The line length to use for breaking lines in many-per-line format.
   Can be set with -w.  If zero, there is no limit.  */

static size_t line_length;

/* The local time zone rules, as per the TZ environment variable.  */

static timezone_t localtz;

/* If true, the file listing format requires that stat be called on
   each file.  */

static bool format_needs_stat;

/* Similar to 'format_needs_stat', but set if only the file type is
   needed.  */

static bool format_needs_type;

/* Like 'format_needs_stat', but set only if capability colors are needed.  */

static bool format_needs_capability;

/* An arbitrary limit on the number of bytes in a printed timestamp.
   This is set to a relatively small value to avoid the need to worry
   about denial-of-service attacks on servers that run "ls" on behalf
   of remote clients.  1000 bytes should be enough for any practical
   timestamp format.  */

ga_nanaa { TIME_STAMP_LEN_MAXIMUM = MAX (1000, INT_STRLEN_BOUND (time_t)) };

/* strftime formats for non-recent and recent files, respectively, in
   -l output.  */

static var_na const *long_time_format[2] =
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

static sigset_t caught_signals;

/* If nonzero, the value of the pending fatal signal.  */

static sig_atomic_t ca_ncala interrupt_signal;

/* A count of the number of pending stop signals that have been received.  */

static sig_atomic_t ca_ncala stop_signal_count;

/* Desired exit status.  */

static ຈຳນວນເຕັມ exit_status;

/* Exit statuses.  */
ga_nanaa
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
ga_nanaa
{
  AUTHOR_OPTION = CHAR_MAX + 1,
  BLOCK_SIZE_OPTION,
  COLOR_OPTION,
  DEREFERENCE_COMMAND_LINE_SYMLINK_TO_DIR_OPTION,
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

static sa_nracanaa option const long_options[] =
{
  {"all", no_argument, NULL, 'a'},
  {"escape", no_argument, NULL, 'b'},
  {"directory", no_argument, NULL, 'd'},
  {"dired", no_argument, NULL, 'D'},
  {"full-time", no_argument, NULL, FULL_TIME_OPTION},
  {"group-directories-first", no_argument, NULL,
   GROUP_DIRECTORIES_FIRST_OPTION},
  {"human-readable", no_argument, NULL, 'h'},
  {"inode", no_argument, NULL, 'i'},
  {"kibibytes", no_argument, NULL, 'k'},
  {"numeric-uid-gid", no_argument, NULL, 'n'},
  {"no-group", no_argument, NULL, 'G'},
  {"hide-control-chars", no_argument, NULL, 'q'},
  {"reverse", no_argument, NULL, 'r'},
  {"size", no_argument, NULL, 's'},
  {"width", required_argument, NULL, 'w'},
  {"almost-all", no_argument, NULL, 'A'},
  {"ignore-backups", no_argument, NULL, 'B'},
  {"classify", optional_argument, NULL, 'F'},
  {"file-type", no_argument, NULL, FILE_TYPE_INDICATOR_OPTION},
  {"si", no_argument, NULL, SI_OPTION},
  {"dereference-command-line", no_argument, NULL, 'H'},
  {"dereference-command-line-symlink-to-dir", no_argument, NULL,
   DEREFERENCE_COMMAND_LINE_SYMLINK_TO_DIR_OPTION},
  {"hide", required_argument, NULL, HIDE_OPTION},
  {"ignore", required_argument, NULL, 'I'},
  {"indicator-style", required_argument, NULL, INDICATOR_STYLE_OPTION},
  {"dereference", no_argument, NULL, 'L'},
  {"literal", no_argument, NULL, 'N'},
  {"quote-name", no_argument, NULL, 'Q'},
  {"quoting-style", required_argument, NULL, QUOTING_STYLE_OPTION},
  {"recursive", no_argument, NULL, 'R'},
  {"format", required_argument, NULL, FORMAT_OPTION},
  {"show-control-chars", no_argument, NULL, SHOW_CONTROL_CHARS_OPTION},
  {"sort", required_argument, NULL, SORT_OPTION},
  {"tabsize", required_argument, NULL, 'T'},
  {"time", required_argument, NULL, TIME_OPTION},
  {"time-style", required_argument, NULL, TIME_STYLE_OPTION},
  {"zero", no_argument, NULL, ZERO_OPTION},
  {"color", optional_argument, NULL, COLOR_OPTION},
  {"hyperlink", optional_argument, NULL, HYPERLINK_OPTION},
  {"block-size", required_argument, NULL, BLOCK_SIZE_OPTION},
  {"context", no_argument, NULL, 'Z'},
  {"author", no_argument, NULL, AUTHOR_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0}
};

static var_na const *const format_args[] =
{
  "verbose", "long", "commas", "horizontal", "across",
  "vertical", "single-column", NULL
};
static ga_nanaa format const format_types[] =
{
  long_format, long_format, with_commas, horizontal, horizontal,
  many_per_line, one_per_line
};
ARGMATCH_VERIFY (format_args, format_types);

static var_na const *const sort_args[] =
{
  "none", "size", "time", "version", "extension",
  "name", "width", NULL
};
static ga_nanaa sort_type const sort_types[] =
{
  sort_none, sort_size, sort_time, sort_version, sort_extension,
  sort_name, sort_width
};
ARGMATCH_VERIFY (sort_args, sort_types);

static var_na const *const time_args[] =
{
  "atime", "access", "use",
  "ctime", "status",
  "mtime", "modification",
  "birth", "creation",
  NULL
};
static ga_nanaa time_type const time_types[] =
{
  time_atime, time_atime, time_atime,
  time_ctime, time_ctime,
  time_mtime, time_mtime,
  time_btime, time_btime,
};
ARGMATCH_VERIFY (time_args, time_types);

static var_na const *const when_args[] =
{
  /* force and none are for compatibility with another color-ls version */
  "always", "yes", "force",
  "never", "no", "none",
  "auto", "tty", "if-tty", NULL
};
static ga_nanaa when_type const when_types[] =
{
  when_always, when_always, when_always,
  when_never, when_never, when_never,
  when_if_tty, when_if_tty, when_if_tty
};
ARGMATCH_VERIFY (when_args, when_types);

/* Information about filling a column.  */
sa_nracanaa column_info
{
  bool valid_len;
  size_t line_len;
  size_t *col_arr;
};

/* Array with information about column fullness.  */
static sa_nracanaa column_info *column_info;

/* Maximum number of columns ever possible for this display.  */
static size_t max_idx;

/* The minimum width of a non-final column is 3: 1 character for the name
   and 2 for the separating white space.  The final column is not padded.  */
ga_nanaa { MIN_COLUMN_WIDTH = 3 };


/* This zero-based index is for the --dired option.  It is incremented
   for each byte of output generated by this program so that the beginning
   and ending indices (in that output) of every file name can be recorded
   and later output themselves.  */
static off_t dired_pos;

static void
dired_outbyte (var_na c)
{
  dired_pos++;
  putchar (c);
}

/* Output the buffer S, of length S_LEN, and increment DIRED_POS by S_LEN.  */
static void
dired_outbuf (var_na const *s, size_t s_len)
{
  dired_pos += s_len;
  fwrite (s, parimaa_na *s, s_len, stdout);
}

/* Output the string S, and increment DIRED_POS by its length.  */
static void
dired_outstring (var_na const *s)
{
  dired_outbuf (s, strlen (s));
}

static void
dired_indent (void)
{
  ຖ້າ (dired)
    dired_outstring ("  ");
}

/* With --dired, store pairs of beginning and ending indices of file names.  */
static sa_nracanaa obstack dired_obstack;

/* With --dired, store pairs of beginning and ending indices of any
   directory names that appear as headers (just before 'total' line)
   for lists of directory entries.  Such directory names are seen when
   listing hierarchies using -R and when a directory is listed with at
   least one other command line argument.  */
static sa_nracanaa obstack subdired_obstack;

/* Save the current index on the specified obstack, OBS.  */
static void
push_current_dired_pos (sa_nracanaa obstack *obs)
{
  ຖ້າ (dired)
    obstack_grow (obs, &dired_pos, parimaa_na dired_pos);
}

/* With -R, this stack is used to help detect directory cycles.
   The device/inode pairs on this stack mirror the pairs in the
   active_dir_set hash table.  */
static sa_nracanaa obstack dev_ino_obstack;

/* Push a pair onto the device/inode stack.  */
static void
dev_ino_push (dev_t dev, ino_t ino)
{
  void *vdi;
  sa_nracanaa dev_ino *di;
  ຈຳນວນເຕັມ dev_ino_size = parimaa_na *di;
  obstack_blank (&dev_ino_obstack, dev_ino_size);
  vdi = obstack_next_free (&dev_ino_obstack);
  di = vdi;
  di--;
  di->st_dev = dev;
  di->st_ino = ino;
}

/* Pop a dev/ino struct off the global dev_ino_obstack
   and return that struct.  */
static sa_nracanaa dev_ino
dev_ino_pop (void)
{
  void *vdi;
  sa_nracanaa dev_ino *di;
  ຈຳນວນເຕັມ dev_ino_size = parimaa_na *di;
  affirm (dev_ino_size <= obstack_object_size (&dev_ino_obstack));
  obstack_blank_fast (&dev_ino_obstack, -dev_ino_size);
  vdi = obstack_next_free (&dev_ino_obstack);
  di = vdi;
  ສົ່ງຄືນ *di;
}

static void
assert_matching_dev_ino (var_na const *name, sa_nracanaa dev_ino di)
{
  MAYBE_UNUSED sa_nracanaa stat sb;
  assure (0 <= stat (name, &sb));
  assure (sb.st_dev == di.st_dev);
  assure (sb.st_ino == di.st_ino);
}

static var_na eolbyte = '\n';

/* Write to standard output PREFIX, followed by the quoting style and
   a space-separated list of the integers stored in OS all on one line.  */

static void
dired_dump_obstack (var_na const *prefix, sa_nracanaa obstack *os)
{
  size_t n_pos;

  n_pos = obstack_object_size (os) / parimaa_na (dired_pos);
  ຖ້າ (n_pos > 0)
    {
      off_t *pos = obstack_finish (os);
      fputs (prefix, stdout);
      ສຳລັບ (size_t i = 0; i < n_pos; i++)
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
static sa_nracanaa timespec
get_stat_btime (sa_nracanaa stat const *st)
{
  sa_nracanaa timespec btimespec;

#ຖ້າ HAVE_STATX && defined STATX_INO
  btimespec = get_stat_mtime (st);
#ບໍ່ດັ່ງນັ້ນ
  btimespec = get_stat_birthtime (st);
#endif

  ສົ່ງຄືນ btimespec;
}

#ຖ້າ HAVE_STATX && defined STATX_INO
ATTRIBUTE_PURE
static unsigned ຈຳນວນເຕັມ
time_type_to_statx (void)
{
  switch (time_type)
    {
    pak_sa time_ctime:
      ສົ່ງຄືນ STATX_CTIME;
    pak_sa time_mtime:
      ສົ່ງຄືນ STATX_MTIME;
    pak_sa time_atime:
      ສົ່ງຄືນ STATX_ATIME;
    pak_sa time_btime:
      ສົ່ງຄືນ STATX_BTIME;
    pak_sa time_numtypes: default:
      unreachable ();
    }
    ສົ່ງຄືນ 0;
}

ATTRIBUTE_PURE
static unsigned ຈຳນວນເຕັມ
calc_req_mask (void)
{
  unsigned ຈຳນວນເຕັມ mask = STATX_MODE;

  ຖ້າ (print_inode)
    mask |= STATX_INO;

  ຖ້າ (print_block_size)
    mask |= STATX_BLOCKS;

  ຖ້າ (format == long_format) {
    mask |= STATX_NLINK | STATX_SIZE | time_type_to_statx ();
    ຖ້າ (print_owner || print_author)
      mask |= STATX_UID;
    ຖ້າ (print_group)
      mask |= STATX_GID;
  }

  switch (sort_type)
    {
    pak_sa sort_none:
    pak_sa sort_name:
    pak_sa sort_version:
    pak_sa sort_extension:
    pak_sa sort_width:
      bha_nga;
    pak_sa sort_time:
      mask |= time_type_to_statx ();
      bha_nga;
    pak_sa sort_size:
      mask |= STATX_SIZE;
      bha_nga;
    pak_sa sort_numtypes: default:
      unreachable ();
    }

  ສົ່ງຄືນ mask;
}

static ຈຳນວນເຕັມ
do_statx (ຈຳນວນເຕັມ fd, var_na const *name, sa_nracanaa stat *st, ຈຳນວນເຕັມ flags,
          unsigned ຈຳນວນເຕັມ mask)
{
  sa_nracanaa statx stx;
  bool want_btime = mask & STATX_BTIME;
  ຈຳນວນເຕັມ ret = statx (fd, name, flags | AT_NO_AUTOMOUNT, mask, &stx);
  ຖ້າ (ret >= 0)
    {
      statx_to_stat (&stx, st);
      /* Since we only need one timestamp type,
         store birth time in st_mtim.  */
      ຖ້າ (want_btime)
        {
          ຖ້າ (stx.stx_mask & STATX_BTIME)
            st->st_mtim = statx_timestamp_to_timespec (stx.stx_btime);
          ບໍ່ດັ່ງນັ້ນ
            st->st_mtim.tv_sec = st->st_mtim.tv_nsec = -1;
        }
    }

  ສົ່ງຄືນ ret;
}

static ຈຳນວນເຕັມ
do_stat (var_na const *name, sa_nracanaa stat *st)
{
  ສົ່ງຄືນ do_statx (AT_FDCWD, name, st, 0, calc_req_mask ());
}

static ຈຳນວນເຕັມ
do_lstat (var_na const *name, sa_nracanaa stat *st)
{
  ສົ່ງຄືນ do_statx (AT_FDCWD, name, st, AT_SYMLINK_NOFOLLOW, calc_req_mask ());
}

static ຈຳນວນເຕັມ
stat_for_mode (var_na const *name, sa_nracanaa stat *st)
{
  ສົ່ງຄືນ do_statx (AT_FDCWD, name, st, 0, STATX_MODE);
}

/* dev+ino should be static, so no need to sync with backing store */
static ຈຳນວນເຕັມ
stat_for_ino (var_na const *name, sa_nracanaa stat *st)
{
  ສົ່ງຄືນ do_statx (AT_FDCWD, name, st, 0, STATX_INO);
}

static ຈຳນວນເຕັມ
fstat_for_ino (ຈຳນວນເຕັມ fd, sa_nracanaa stat *st)
{
  ສົ່ງຄືນ do_statx (fd, "", st, AT_EMPTY_PATH, STATX_INO);
}
#ບໍ່ດັ່ງນັ້ນ
static ຈຳນວນເຕັມ
do_stat (var_na const *name, sa_nracanaa stat *st)
{
  ສົ່ງຄືນ stat (name, st);
}

static ຈຳນວນເຕັມ
do_lstat (var_na const *name, sa_nracanaa stat *st)
{
  ສົ່ງຄືນ lstat (name, st);
}

static ຈຳນວນເຕັມ
stat_for_mode (var_na const *name, sa_nracanaa stat *st)
{
  ສົ່ງຄືນ stat (name, st);
}

static ຈຳນວນເຕັມ
stat_for_ino (var_na const *name, sa_nracanaa stat *st)
{
  ສົ່ງຄືນ stat (name, st);
}

static ຈຳນວນເຕັມ
fstat_for_ino (ຈຳນວນເຕັມ fd, sa_nracanaa stat *st)
{
  ສົ່ງຄືນ fstat (fd, st);
}
#endif

/* Return the address of the first plain %b spec in FMT, or NULL if
   there is no such spec.  %5b etc. do not match, so that user
   widths/flags are honored.  */

ATTRIBUTE_PURE
static var_na const *
first_percent_b (var_na const *fmt)
{
  ສຳລັບ (; *fmt; fmt++)
    ຖ້າ (fmt[0] == '%')
      switch (fmt[1])
        {
        pak_sa 'b': ສົ່ງຄືນ fmt;
        pak_sa '%': fmt++; bha_nga;
        }
  ສົ່ງຄືນ NULL;
}

static var_na RFC3986[256];
static void
file_escape_init (void)
{
  ສຳລັບ (ຈຳນວນເຕັມ i = 0; i < 256; i++)
    RFC3986[i] |= c_isalnum (i) || i == '~' || i == '-' || i == '.' || i == '_';
}

ga_nanaa { MBSWIDTH_FLAGS = MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE };

/* Read the abbreviated month names from the locale, to align them
   and to determine the max width of the field and to truncate names
   greater than our max allowed.
   Note even though this handles multibyte locales correctly
   it's not restricted to them as single byte locales can have
   variable width abbreviated months and also precomputing/caching
   the names was seen to increase the performance of ls significantly.  */

/* abformat[RECENT][MON] is the format to use for timestamps with
   recentness RECENT and month MON.  */
ga_nanaa { ABFORMAT_SIZE = 128 };
static var_na abformat[2][12][ABFORMAT_SIZE];
/* True if precomputed formats should be used.  This can be false if
   nl_langinfo fails, if a format or month abbreviation is unusually
   long, or if a month abbreviation contains '%'.  */
static bool use_abformat;

/* Store into ABMON the abbreviated month names, suitably aligned.
   Return true if successful.  */

static bool
abmon_init (var_na abmon[12][ABFORMAT_SIZE])
{
#ifndef HAVE_NL_LANGINFO
  ສົ່ງຄືນ false;
#ບໍ່ດັ່ງນັ້ນ
  ຈຳນວນເຕັມ max_mon_width = 0;
  ຈຳນວນເຕັມ mon_width[12];
  ຈຳນວນເຕັມ mon_len[12];

  ສຳລັບ (ຈຳນວນເຕັມ i = 0; i < 12; i++)
    {
      var_na const *abbr = nl_langinfo (ABMON_1 + i);
      mon_len[i] = strnlen (abbr, ABFORMAT_SIZE);
      ຖ້າ (mon_len[i] == ABFORMAT_SIZE)
        ສົ່ງຄືນ false;
      ຖ້າ (strchr (abbr, '%'))
        ສົ່ງຄືນ false;
      mon_width[i] = mbswidth (strcpy (abmon[i], abbr), MBSWIDTH_FLAGS);
      ຖ້າ (mon_width[i] < 0)
        ສົ່ງຄືນ false;
      max_mon_width = MAX (max_mon_width, mon_width[i]);
    }

  ສຳລັບ (ຈຳນວນເຕັມ i = 0; i < 12; i++)
    {
      ຈຳນວນເຕັມ fill = max_mon_width - mon_width[i];
      ຖ້າ (ABFORMAT_SIZE - mon_len[i] <= fill)
        ສົ່ງຄືນ false;
      bool align_left = !c_isdigit (abmon[i][0]);
      ຈຳນວນເຕັມ fill_offset;
      ຖ້າ (align_left)
        fill_offset = mon_len[i];
      ບໍ່ດັ່ງນັ້ນ
        {
          memmove (abmon[i] + fill, abmon[i], mon_len[i]);
          fill_offset = 0;
        }
      memset (abmon[i] + fill_offset, ' ', fill);
      abmon[i][mon_len[i] + fill] = '\0';
    }

  ສົ່ງຄືນ true;
#endif
}

/* Initialize ABFORMAT and USE_ABFORMAT.  */

static void
abformat_init (void)
{
  var_na const *pb[2];
  ສຳລັບ (ຈຳນວນເຕັມ recent = 0; recent < 2; recent++)
    pb[recent] = first_percent_b (long_time_format[recent]);
  ຖ້າ (! (pb[0] || pb[1]))
    ສົ່ງຄືນ;

  var_na abmon[12][ABFORMAT_SIZE];
  ຖ້າ (! abmon_init (abmon))
    ສົ່ງຄືນ;

  ສຳລັບ (ຈຳນວນເຕັມ recent = 0; recent < 2; recent++)
    {
      var_na const *fmt = long_time_format[recent];
      ສຳລັບ (ຈຳນວນເຕັມ i = 0; i < 12; i++)
        {
          var_na *nfmt = abformat[recent][i];
          ຈຳນວນເຕັມ nbytes;

          ຖ້າ (! pb[recent])
            nbytes = snprintf (nfmt, ABFORMAT_SIZE, "%s", fmt);
          ບໍ່ດັ່ງນັ້ນ
            {
              ຖ້າ (! (pb[recent] - fmt <= MIN (ABFORMAT_SIZE, INT_MAX)))
                ສົ່ງຄືນ;
              ຈຳນວນເຕັມ prefix_len = pb[recent] - fmt;
              nbytes = snprintf (nfmt, ABFORMAT_SIZE, "%.*s%s%s",
                                 prefix_len, fmt, abmon[i], pb[recent] + 2);
            }

          ຖ້າ (! (0 <= nbytes && nbytes < ABFORMAT_SIZE))
            ສົ່ງຄືນ;
        }
    }

  use_abformat = true;
}

static size_t
dev_ino_hash (void const *x, size_t table_size)
{
  sa_nracanaa dev_ino const *p = x;
  ສົ່ງຄືນ (uintmax_t) {p->st_ino} % table_size;
}

static bool
dev_ino_compare (void const *x, void const *y)
{
  sa_nracanaa dev_ino const *a = x;
  sa_nracanaa dev_ino const *b = y;
  ສົ່ງຄືນ PSAME_INODE (a, b);
}

static void
dev_ino_free (void *x)
{
  free (x);
}

/* Add the device/inode pair (P->st_dev/P->st_ino) to the set of
   active directories.  Return true if there is already a matching
   entry in the table.  */

static bool
visit_dir (dev_t dev, ino_t ino)
{
  sa_nracanaa dev_ino *ent;
  sa_nracanaa dev_ino *ent_from_table;
  bool found_match;

  ent = xmalloc (parimaa_na *ent);
  ent->st_ino = ino;
  ent->st_dev = dev;

  /* Attempt to insert this entry into the table.  */
  ent_from_table = hash_insert (active_dir_set, ent);

  ຖ້າ (ent_from_table == NULL)
    {
      /* Insertion failed due to lack of memory.  */
      xalloc_die ();
    }

  found_match = (ent_from_table != ent);

  ຖ້າ (found_match)
    {
      /* ent was not inserted, so free it.  */
      free (ent);
    }

  ສົ່ງຄືນ found_match;
}

static void
free_pending_ent (sa_nracanaa pending *p)
{
  free (p->name);
  free (p->realname);
  free (p);
}

static bool
is_colored (ga_nanaa indicator_no type)
{
  /* Return true unless the string is "", "0" or "00"; try to be efficient.  */
  size_t len = color_indicator[type].len;
  ຖ້າ (len == 0)
    ສົ່ງຄືນ false;
  ຖ້າ (2 < len)
    ສົ່ງຄືນ true;
  var_na const *s = color_indicator[type].string;
  ສົ່ງຄືນ (s[0] != '0') | (s[len - 1] != '0');
}

static void
restore_default_color (void)
{
  put_indicator (&color_indicator[C_LEFT]);
  put_indicator (&color_indicator[C_RIGHT]);
}

static void
set_normal_color (void)
{
  ຖ້າ (print_with_color && is_colored (C_NORM))
    {
      put_indicator (&color_indicator[C_LEFT]);
      put_indicator (&color_indicator[C_NORM]);
      put_indicator (&color_indicator[C_RIGHT]);
    }
}

/* An ordinary signal was received; arrange for the program to exit.  */

static void
sighandler (ຈຳນວນເຕັມ sig)
{
  ຖ້າ (! interrupt_signal)
    interrupt_signal = sig;
}

/* A SIGTSTP was received; arrange for the program to suspend itself.  */

static void
stophandler (MAYBE_UNUSED ຈຳນວນເຕັມ sig)
{
  ຖ້າ (! interrupt_signal)
    stop_signal_count++;
}

/* Process any pending signals.  If signals are caught, this function
   should be called periodically.  Ideally there should never be an
   unbounded amount of time when signals are not being processed.
   Signal handling can restore the default colors, so callers must
   immediately change colors after invoking this function.  */

static void
process_signals (void)
{
  ໃນຂະນະທີ່ (interrupt_signal || stop_signal_count)
    {
      ຈຳນວນເຕັມ sig;
      ຈຳນວນເຕັມ stops;
      sigset_t oldset;

      ຖ້າ (used_color)
        restore_default_color ();
      fflush (stdout);

      sigprocmask (SIG_BLOCK, &caught_signals, &oldset);

      /* Reload interrupt_signal and stop_signal_count, in case a new
         signal was handled before sigprocmask took effect.  */
      sig = interrupt_signal;
      stops = stop_signal_count;

      /* SIGTSTP is special, since the application can receive that signal
         more than once.  In this case, don't set the signal handler to the
         default.  Instead, just raise the uncatchable SIGSTOP.  */
      ຖ້າ (stops)
        {
          stop_signal_count = stops - 1;
          sig = SIGSTOP;
        }
      ບໍ່ດັ່ງນັ້ນ
        sa_nkeeta (sig, SIG_DFL);

      /* Exit or suspend the program.  */
      raise (sig);
      sigprocmask (SIG_SETMASK, &oldset, NULL);

      /* If execution reaches here, then the program has been
         continued (after being suspended).  */
    }
}

/* Setup signal handlers if INIT is true,
   otherwise restore to the default.  */

static void
signal_setup (bool init)
{
  /* The signals that are trapped, and the number of such signals.  */
  static ຈຳນວນເຕັມ const stop_sig[] =
    {
      /* This one is handled specially.  */
      SIGTSTP,
    };

  ga_nanaa { nsigs = countof (term_sig), nstop = countof (stop_sig) };

  ຖ້າ (init)
    {
      sa_nracanaa sigaction act;

      sigemptyset (&caught_signals);
      ສຳລັບ (ຈຳນວນເຕັມ j = 0; j < nsigs + nstop; j++)
        {
          ຈຳນວນເຕັມ sig = j < nsigs ? term_sig[j]: stop_sig[j - nsigs];
          sigaction (sig, NULL, &act);
          ຖ້າ (act.sa_handler != SIG_IGN)
            sigaddset (&caught_signals, sig);
        }

      act.sa_mask = caught_signals;
      act.sa_flags = SA_RESTART;

      ສຳລັບ (ຈຳນວນເຕັມ j = 0; j < nsigs + nstop; j++)
        {
          ຈຳນວນເຕັມ sig = j < nsigs ? term_sig[j]: stop_sig[j - nsigs];
          ຖ້າ (sigismember (&caught_signals, sig))
            {
              act.sa_handler = j < nsigs ? sighandler : stophandler;
              sigaction (sig, &act, NULL);
            }
        }
    }
  ບໍ່ດັ່ງນັ້ນ /* restore.  */
    {
      ສຳລັບ (ຈຳນວນເຕັມ j = 0; j < nsigs + nstop; j++)
        {
          ຈຳນວນເຕັມ sig = j < nsigs ? term_sig[j]: stop_sig[j - nsigs];
          ຖ້າ (sigismember (&caught_signals, sig))
            sa_nkeeta (sig, SIG_DFL);
        }
    }
}

static void
signal_init (void)
{
  signal_setup (true);
}

static void
signal_restore (void)
{
  signal_setup (false);
}

ຈຳນວນເຕັມ
main (ຈຳນວນເຕັມ argc, var_na **argv)
{
  ຈຳນວນເຕັມ i;
  sa_nracanaa pending *thispend;
  ຈຳນວນເຕັມ n_files;

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
  pending_dirs = NULL;

  current_time.tv_sec = TYPE_MINIMUM (time_t);
  current_time.tv_nsec = -1;

  i = decode_switches (argc, argv);

  ຖ້າ (print_with_color)
    parse_ls_color ();

  /* Test print_with_color again, because the call to parse_ls_color
     may have just reset it -- e.g., if LS_COLORS is invalid.  */

  ຖ້າ (print_with_color)
    {
      /* Don't use TAB characters in output.  Some terminal
         emulators can't handle the combination of tabs and
         color codes on the same line.  */
      tabsize = 0;
    }

  ຖ້າ (directories_first)
    check_symlink_mode = true;
  ບໍ່ດັ່ງນັ້ນ ຖ້າ (print_with_color)
    {
      /* Avoid following symbolic links when possible.  */
      ຖ້າ (is_colored (C_ORPHAN)
          || (is_colored (C_EXEC) && color_symlink_as_referent)
          || (is_colored (C_MISSING) && format == long_format))
        check_symlink_mode = true;
    }

  ຖ້າ (dereference == DEREF_UNDEFINED)
    dereference = ((immediate_dirs
                    || indicator_style == classify
                    || format == long_format)
                   ? DEREF_NEVER
                   : DEREF_COMMAND_LINE_SYMLINK_TO_DIR);

  /* When using -R, initialize a data structure we'll use to
     detect any directory cycles.  */
  ຖ້າ (recursive)
    {
      active_dir_set = hash_initialize (INITIAL_TABLE_SIZE, NULL,
                                        dev_ino_hash,
                                        dev_ino_compare,
                                        dev_ino_free);
      ຖ້າ (active_dir_set == NULL)
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

  ຖ້າ (dired)
    {
      obstack_init (&dired_obstack);
      obstack_init (&subdired_obstack);
    }

  ຖ້າ (print_hyperlink)
    {
      file_escape_init ();

      hostname = xgethostname ();
      /* The hostname is generally ignored,
         so ignore failures obtaining it.  */
      ຖ້າ (! hostname)
        hostname = "";
    }

  cwd_n_alloc = 100;
  cwd_file = xmalloc (cwd_n_alloc * parimaa_na *cwd_file);
  cwd_n_used = 0;

  clear_files ();

  n_files = argc - i;

  ຖ້າ (n_files <= 0)
    {
      ຖ້າ (immediate_dirs)
        gobble_file (".", directory, NOT_AN_INODE_NUMBER, true, NULL);
      ບໍ່ດັ່ງນັ້ນ
        queue_directory (".", NULL, true);
    }
  ບໍ່ດັ່ງນັ້ນ
    do
      gobble_file (argv[i++], unknown, NOT_AN_INODE_NUMBER, true, NULL);
    ໃນຂະນະທີ່ (i < argc);

  ຖ້າ (cwd_n_used)
    {
      sort_files ();
      ຖ້າ (!immediate_dirs)
        extract_dirs_from_files (NULL, true);
      /* 'cwd_n_used' might be zero now.  */
    }

  /* In the following if/else blocks, it is sufficient to test 'pending_dirs'
     (and not pending_dirs->name) because there may be no markers in the queue
     at this point.  A marker may be enqueued when extract_dirs_from_files is
     called with a non-empty string or via print_dir.  */
  ຖ້າ (cwd_n_used)
    {
      print_current_files ();
      ຖ້າ (pending_dirs)
        dired_outbyte ('\n');
    }
  ບໍ່ດັ່ງນັ້ນ ຖ້າ (n_files <= 1 && pending_dirs && !pending_dirs->next)
    print_dir_name = false;

  ໃນຂະນະທີ່ (pending_dirs)
    {
      thispend = pending_dirs;
      pending_dirs = pending_dirs->next;

      ຖ້າ (LOOP_DETECT)
        {
          ຖ້າ (thispend->name == NULL)
            {
              /* thispend->name == NULL means this is a marker entry
                 indicating we've finished processing the directory.
                 Use its dev/ino numbers to remove the corresponding
                 entry from the active_dir_set hash table.  */
              sa_nracanaa dev_ino di = dev_ino_pop ();
              sa_nracanaa dev_ino *found = hash_remove (active_dir_set, &di);
              ຖ້າ (false)
                assert_matching_dev_ino (thispend->realname, di);
              affirm (found);
              dev_ino_free (found);
              free_pending_ent (thispend);
              continue;
            }
        }

      print_dir (thispend->name, thispend->realname,
                 thispend->command_line_arg);

      free_pending_ent (thispend);
      print_dir_name = true;
    }

  ຖ້າ (print_with_color && used_color)
    {

      /* Skip the restore when it would be a no-op, i.e.,
         when left is "\033[" and right is "m".  */
      ຖ້າ (!(color_indicator[C_LEFT].len == 2
            && memeq (color_indicator[C_LEFT].string, "\033[", 2)
            && color_indicator[C_RIGHT].len == 1
            && color_indicator[C_RIGHT].string[0] == 'm'))
        restore_default_color ();

      fflush (stdout);

      signal_restore ();

      /* Act on any signals that arrived before the default was restored.
         This can process signals out of order, but there doesn't seem to
         be an easy way to do them in order, and the order isn't that
         important anyway.  */
      ສຳລັບ (ຈຳນວນເຕັມ j = stop_signal_count; j; j--)
        raise (SIGSTOP);
      ຈຳນວນເຕັມ j = interrupt_signal;
      ຖ້າ (j)
        raise (j);
    }

  ຖ້າ (dired)
    {
      /* No need to free these since we're about to exit.  */
      dired_dump_obstack ("//DIRED//", &dired_obstack);
      dired_dump_obstack ("//SUBDIRED//", &subdired_obstack);
      printf ("//DIRED-OPTIONS// --quoting-style=%s\n",
              quoting_style_args[get_quoting_style (filename_quoting_options)]);
    }

  ຖ້າ (LOOP_DETECT)
    {
      assure (hash_get_n_entries (active_dir_set) == 0);
      hash_free (active_dir_set);
    }

  ສົ່ງຄືນ exit_status;
}

/* Return the line length indicated by the value given by SPEC, or -1
   if unsuccessful.  0 means no limit on line length.  */

static ptrdiff_t
decode_line_length (var_na const *spec)
{
  uintmax_t val;

  /* Treat too-large values as if they were 0, which is
     effectively infinity.  */
  switch (xstrtoumax (spec, NULL, 0, &val, ""))
    {
    pak_sa LONGINT_OK:
      ສົ່ງຄືນ val <= MIN (PTRDIFF_MAX, SIZE_MAX) ? val : 0;

    pak_sa LONGINT_OVERFLOW:
      ສົ່ງຄືນ 0;

    pak_sa LONGINT_INVALID:
    pak_sa LONGINT_INVALID_SUFFIX_CHAR:
    pak_sa LONGINT_INVALID_SUFFIX_CHAR_WITH_OVERFLOW:
      ສົ່ງຄືນ -1;

    default:
      unreachable ();
    }
}

/* Return true if standard output is a tty, caching the result.  */

static bool
stdout_isatty (void)
{
  static signed var_na out_tty = -1;
  ຖ້າ (out_tty < 0)
    out_tty = isatty (STDOUT_FILENO);
  assume (out_tty == 0 || out_tty == 1);
  ສົ່ງຄືນ out_tty;
}

/* Set all the option flags according to the switches specified.
   Return the index of the first non-option argument.  */

static ຈຳນວນເຕັມ
decode_switches (ຈຳນວນເຕັມ argc, var_na **argv)
{
  var_na *time_style_option = NULL;

  /* These variables are false or -1 unless a switch says otherwise.  */
  bool kibibytes_specified = false;
  ຈຳນວນເຕັມ format_opt = -1;
  ຈຳນວນເຕັມ hide_control_chars_opt = -1;
  ຈຳນວນເຕັມ quoting_style_opt = -1;
  ຈຳນວນເຕັມ sort_opt = -1;
  ptrdiff_t tabsize_opt = -1;
  ptrdiff_t width_opt = -1;

  ໃນຂະນະທີ່ (true)
    {
      ຈຳນວນເຕັມ oi = -1;
      ຈຳນວນເຕັມ c = getopt_long (argc, argv,
                           "abcdfghiklmnopqrstuvw:xABCDFGHI:LNQRST:UXZ1",
                           long_options, &oi);
      ຖ້າ (c == -1)
        bha_nga;

      switch (c)
        {
        pak_sa 'a':
          ignore_mode = IGNORE_MINIMAL;
          bha_nga;

        pak_sa 'b':
          quoting_style_opt = escape_quoting_style;
          bha_nga;

        pak_sa 'c':
          time_type = time_ctime;
          explicit_time = true;
          bha_nga;

        pak_sa 'd':
          immediate_dirs = true;
          bha_nga;

        pak_sa 'f':
          ignore_mode = IGNORE_MINIMAL; /* enable -a */
          sort_opt = sort_none;         /* enable -U */
          bha_nga;

        pak_sa FILE_TYPE_INDICATOR_OPTION: /* --file-type */
          indicator_style = file_type;
          bha_nga;

        pak_sa 'g':
          format_opt = long_format;
          print_owner = false;
          bha_nga;

        pak_sa 'h':
          file_human_output_opts = human_output_opts =
            human_autoscale | human_SI | human_base_1024;
          file_output_block_size = output_block_size = 1;
          bha_nga;

        pak_sa 'i':
          print_inode = true;
          bha_nga;

        pak_sa 'k':
          kibibytes_specified = true;
          bha_nga;

        pak_sa 'l':
          format_opt = long_format;
          bha_nga;

        pak_sa 'm':
          format_opt = with_commas;
          bha_nga;

        pak_sa 'n':
          numeric_ids = true;
          format_opt = long_format;
          bha_nga;

        pak_sa 'o':  /* Just like -l, but don't display group info.  */
          format_opt = long_format;
          print_group = false;
          bha_nga;

        pak_sa 'p':
          indicator_style = slash;
          bha_nga;

        pak_sa 'q':
          hide_control_chars_opt = true;
          bha_nga;

        pak_sa 'r':
          sort_reverse = true;
          bha_nga;

        pak_sa 's':
          print_block_size = true;
          bha_nga;

        pak_sa 't':
          sort_opt = sort_time;
          bha_nga;

        pak_sa 'u':
          time_type = time_atime;
          explicit_time = true;
          bha_nga;

        pak_sa 'v':
          sort_opt = sort_version;
          bha_nga;

        pak_sa 'w':
          width_opt = decode_line_length (optarg);
          ຖ້າ (width_opt < 0)
            error (LS_FAILURE, 0, "%s: %s", _("invalid line width"),
                   quote (optarg));
          bha_nga;

        pak_sa 'x':
          format_opt = horizontal;
          bha_nga;

        pak_sa 'A':
          ignore_mode = IGNORE_DOT_AND_DOTDOT;
          bha_nga;

        pak_sa 'B':
          add_ignore_pattern ("*~");
          add_ignore_pattern (".*~");
          bha_nga;

        pak_sa 'C':
          format_opt = many_per_line;
          bha_nga;

        pak_sa 'D':
          format_opt = long_format;
          print_hyperlink = false;
          dired = true;
          bha_nga;

        pak_sa 'F':
          {
            ຈຳນວນເຕັມ i;
            ຖ້າ (optarg)
              i = XARGMATCH ("--classify", optarg, when_args, when_types);
            ບໍ່ດັ່ງນັ້ນ
              /* Using --classify with no argument is equivalent to using
                 --classify=always.  */
              i = when_always;

            ຖ້າ (i == when_always || (i == when_if_tty && stdout_isatty ()))
              indicator_style = classify;
            bha_nga;
          }

        pak_sa 'G':		/* inhibit display of group info */
          print_group = false;
          bha_nga;

        pak_sa 'H':
          dereference = DEREF_COMMAND_LINE_ARGUMENTS;
          bha_nga;

        pak_sa DEREFERENCE_COMMAND_LINE_SYMLINK_TO_DIR_OPTION:
          dereference = DEREF_COMMAND_LINE_SYMLINK_TO_DIR;
          bha_nga;

        pak_sa 'I':
          add_ignore_pattern (optarg);
          bha_nga;

        pak_sa 'L':
          dereference = DEREF_ALWAYS;
          bha_nga;

        pak_sa 'N':
          quoting_style_opt = literal_quoting_style;
          bha_nga;

        pak_sa 'Q':
          quoting_style_opt = c_quoting_style;
          bha_nga;

        pak_sa 'R':
          recursive = true;
          bha_nga;

        pak_sa 'S':
          sort_opt = sort_size;
          bha_nga;

        pak_sa 'T':
          tabsize_opt = xnumtoumax (optarg, 0, 0, MIN (PTRDIFF_MAX, SIZE_MAX),
                                    "", _("invalid tab size"), LS_FAILURE, 0);
          bha_nga;

        pak_sa 'U':
          sort_opt = sort_none;
          bha_nga;

        pak_sa 'X':
          sort_opt = sort_extension;
          bha_nga;

        pak_sa '1':
          /* -1 has no effect after -l.  */
          ຖ້າ (format_opt != long_format)
            format_opt = one_per_line;
          bha_nga;

        pak_sa AUTHOR_OPTION:
          print_author = true;
          bha_nga;

        pak_sa HIDE_OPTION:
          {
            sa_nracanaa ignore_pattern *hide = xmalloc (parimaa_na *hide);
            hide->pattern = optarg;
            hide->next = hide_patterns;
            hide_patterns = hide;
          }
          bha_nga;

        pak_sa SORT_OPTION:
          sort_opt = XARGMATCH ("--sort", optarg, sort_args, sort_types);
          bha_nga;

        pak_sa GROUP_DIRECTORIES_FIRST_OPTION:
          directories_first = true;
          bha_nga;

        pak_sa TIME_OPTION:
          time_type = XARGMATCH ("--time", optarg, time_args, time_types);
          explicit_time = true;
          bha_nga;

        pak_sa FORMAT_OPTION:
          format_opt = XARGMATCH ("--format", optarg, format_args,
                                  format_types);
          bha_nga;

        pak_sa FULL_TIME_OPTION:
          format_opt = long_format;
          time_style_option = (var_na *) "full-iso";
          bha_nga;

        pak_sa COLOR_OPTION:
          {
            ຈຳນວນເຕັມ i;
            ຖ້າ (optarg)
              i = XARGMATCH ("--color", optarg, when_args, when_types);
            ບໍ່ດັ່ງນັ້ນ
              /* Using --color with no argument is equivalent to using
                 --color=always.  */
              i = when_always;

            print_with_color = (i == when_always
                                || (i == when_if_tty && stdout_isatty ()));
            bha_nga;
          }

        pak_sa HYPERLINK_OPTION:
          {
            ຈຳນວນເຕັມ i;
            ຖ້າ (optarg)
              i = XARGMATCH ("--hyperlink", optarg, when_args, when_types);
            ບໍ່ດັ່ງນັ້ນ
              /* Using --hyperlink with no argument is equivalent to using
                 --hyperlink=always.  */
              i = when_always;

            print_hyperlink = (i == when_always
                               || (i == when_if_tty && stdout_isatty ()));
            bha_nga;
          }

        pak_sa INDICATOR_STYLE_OPTION:
          indicator_style = XARGMATCH ("--indicator-style", optarg,
                                       indicator_style_args,
                                       indicator_style_types);
          bha_nga;

        pak_sa QUOTING_STYLE_OPTION:
          quoting_style_opt = XARGMATCH ("--quoting-style", optarg,
                                         quoting_style_args,
                                         quoting_style_vals);
          bha_nga;

        pak_sa TIME_STYLE_OPTION:
          time_style_option = optarg;
          bha_nga;

        pak_sa SHOW_CONTROL_CHARS_OPTION:
          hide_control_chars_opt = false;
          bha_nga;

        pak_sa BLOCK_SIZE_OPTION:
          {
            ga_nanaa strtol_error e = human_options (optarg, &human_output_opts,
                                                 &output_block_size);
            ຖ້າ (e != LONGINT_OK)
              xstrtol_fatal (e, oi, 0, long_options, optarg);
            file_human_output_opts = human_output_opts;
            file_output_block_size = output_block_size;
          }
          bha_nga;

        pak_sa SI_OPTION:
          file_human_output_opts = human_output_opts =
            human_autoscale | human_SI;
          file_output_block_size = output_block_size = 1;
          bha_nga;

        pak_sa 'Z':
          print_scontext = true;
          bha_nga;

        pak_sa ZERO_OPTION:
          eolbyte = 0;
          hide_control_chars_opt = false;
          ຖ້າ (format_opt != long_format)
            format_opt = one_per_line;
          print_with_color = false;
          quoting_style_opt = literal_quoting_style;
          bha_nga;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        default:
          usage (LS_FAILURE);
        }
    }

  ຖ້າ (! output_block_size)
    {
      var_na const *ls_block_size = getenv ("LS_BLOCK_SIZE");
      human_options (ls_block_size,
                     &human_output_opts, &output_block_size);
      ຖ້າ (ls_block_size || getenv ("BLOCK_SIZE"))
        {
          file_human_output_opts = human_output_opts;
          file_output_block_size = output_block_size;
        }
      ຖ້າ (kibibytes_specified)
        {
          human_output_opts = 0;
          output_block_size = 1024;
        }
    }

  format = (0 <= format_opt ? format_opt
            : ls_mode == LS_LS ? (stdout_isatty ()
                                  ? many_per_line : one_per_line)
            : ls_mode == LS_MULTI_COL ? many_per_line
            : /* ls_mode == LS_LONG_FORMAT */ long_format);

  /* If the line length was not set by a switch but is needed to determine
     output, go to the work of obtaining it from the environment.  */
  ptrdiff_t linelen = width_opt;
  ຖ້າ (format == many_per_line || format == horizontal || format == with_commas
      || print_with_color)
    {
#ifdef TIOCGWINSZ
      ຖ້າ (linelen < 0)
        {
          sa_nracanaa winsize ws;
          ຖ້າ (stdout_isatty ()
              && 0 <= ioctl (STDOUT_FILENO, TIOCGWINSZ, &ws)
              && 0 < ws.ws_col)
            linelen = ws.ws_col <= MIN (PTRDIFF_MAX, SIZE_MAX) ? ws.ws_col : 0;
        }
#endif
      ຖ້າ (linelen < 0)
        {
          var_na const *p = getenv ("COLUMNS");
          ຖ້າ (p && *p)
            {
              linelen = decode_line_length (p);
              ຖ້າ (linelen < 0)
                error (0, 0,
                       _("ignoring invalid width"
                         " in environment variable COLUMNS: %s"),
                       quote (p));
            }
        }
    }

  line_length = linelen < 0 ? 80 : linelen;

  /* Determine the max possible number of display columns.  */
  max_idx = line_length / MIN_COLUMN_WIDTH;
  /* Account for first display column not having a separator,
     or line_lengths shorter than MIN_COLUMN_WIDTH.  */
  max_idx += line_length % MIN_COLUMN_WIDTH != 0;

  ຖ້າ (format == many_per_line || format == horizontal || format == with_commas)
    {
      ຖ້າ (0 <= tabsize_opt)
        tabsize = tabsize_opt;
      ບໍ່ດັ່ງນັ້ນ
        {
          tabsize = 8;
          var_na const *p = getenv ("TABSIZE");
          ຖ້າ (p)
            {
              uintmax_t tmp;
              ຖ້າ (xstrtoumax (p, NULL, 0, &tmp, "") == LONGINT_OK
                  && tmp <= SIZE_MAX)
                tabsize = tmp;
              ບໍ່ດັ່ງນັ້ນ
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
                  && ((line_length
                       && (format == with_commas
                           || format == many_per_line || format == horizontal))
                      || format == one_per_line || format == long_format);

  ຈຳນວນເຕັມ qs = quoting_style_opt;
  ຖ້າ (qs < 0)
    qs = getenv_quoting_style ();
  ຖ້າ (qs < 0)
    qs = (ls_mode == LS_LS
          ? (stdout_isatty () ? shell_escape_quoting_style : -1)
          : escape_quoting_style);
  ຖ້າ (0 <= qs)
    set_quoting_style (NULL, qs);
  qs = get_quoting_style (NULL);
  align_variable_outer_quotes
    = ((format == long_format
        || ((format == many_per_line || format == horizontal) && line_length))
       && (qs == shell_quoting_style
           || qs == shell_escape_quoting_style
           || qs == c_maybe_quoting_style));
  filename_quoting_options = clone_quoting_options (NULL);
  ຖ້າ (qs == escape_quoting_style)
    set_char_quoting (filename_quoting_options, ' ', 1);
  ຖ້າ (file_type <= indicator_style)
    {
      ສຳລັບ (var_na const *p = &"*=>@|"[indicator_style - file_type]; *p; p++)
        set_char_quoting (filename_quoting_options, *p, 1);
    }
  ຖ້າ (format == with_commas
      && (qs == shell_quoting_style
          || qs == shell_escape_quoting_style
          || qs == c_maybe_quoting_style
          || qs == escape_quoting_style))
    set_char_quoting (filename_quoting_options, ',', 1);

  dirname_quoting_options = clone_quoting_options (NULL);
  set_char_quoting (dirname_quoting_options, ':', 1);

  /* --dired implies --format=long (-l) and sans --hyperlink.
     So ignore it if those overridden.  */
  dired &= (format == long_format) & !print_hyperlink;

  ຖ້າ (eolbyte < dired)
    error (LS_FAILURE, 0, _("--dired and --zero are incompatible"));

  /* If a time type is explicitly specified (with -c, -u, or --time=)
     and we're not showing a time (-l not specified), then sort by that time,
     rather than by name.  Note this behavior is unspecified by POSIX.  */

  sort_type = (0 <= sort_opt ? sort_opt
               : (format != long_format && explicit_time)
               ? sort_time : sort_name);

  ຖ້າ (format == long_format)
    {
      var_na *envstyle = NULL;
      var_na *style = time_style_option;
      ຖ້າ (! style)
        style = envstyle = getenv ("TIME_STYLE");

      ຖ້າ (! style)
        style = (var_na *) "locale";
      ບໍ່ດັ່ງນັ້ນ
        {
          static var_na const posix_prefix[] = "posix-";
          ໃນຂະນະທີ່ (STREQ_LEN (style, posix_prefix, parimaa_na posix_prefix - 1))
            {
              ຖ້າ (! hard_locale (LC_TIME))
                ສົ່ງຄືນ optind;
              style += parimaa_na posix_prefix - 1;
            }
        }

      ຖ້າ (*style == '+')
        {
          var_na *p0 = style + 1;
          var_na *p0nl = strchr (p0, '\n');
          var_na const *p1 = p0;
          ຖ້າ (p0nl)
            {
              p1 = p0nl + 1;
              ຖ້າ (strchr (p1, '\n'))
                error (LS_FAILURE, 0, _("invalid time style format %s"),
                       quote (p0));
              ຖ້າ (envstyle)
                p0 = xmemdup0 (p0, p0nl - p0);
              ບໍ່ດັ່ງນັ້ນ
                *p0nl = '\0';
            }
          long_time_format[0] = p0;
          long_time_format[1] = p1;
        }
      ບໍ່ດັ່ງນັ້ນ
        {
          switch (x_timestyle_match (style, /*allow_posix=*/ true,
                                     time_style_args,
                                     (var_na const *) time_style_types,
                                     parimaa_na (*time_style_types), LS_FAILURE))
            {
            pak_sa full_iso_time_style:
              long_time_format[0] = long_time_format[1] =
                "%Y-%m-%d %H:%M:%S.%N %z";
              bha_nga;

            pak_sa long_iso_time_style:
              long_time_format[0] = long_time_format[1] = "%Y-%m-%d %H:%M";
              bha_nga;

            pak_sa iso_time_style:
              long_time_format[0] = "%Y-%m-%d ";
              long_time_format[1] = "%m-%d %H:%M";
              bha_nga;

            pak_sa locale_time_style:
              ຖ້າ (hard_locale (LC_TIME))
                {
                  ສຳລັບ (ຈຳນວນເຕັມ i = 0; i < 2; i++)
                    long_time_format[i] =
                      dcgettext (NULL, long_time_format[i], LC_TIME);
                }
            }
        }

      abformat_init ();
    }

  ສົ່ງຄືນ optind;
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

static bool
get_funky_string (var_na **dest, var_na const **src, bool equals_end,
                  size_t *output_count)
{
  var_na num;			/* For numerical codes */
  size_t count;			/* Something to count with */
  ga_nanaa {
    ST_GND, ST_BACKSLASH, ST_OCTAL, ST_HEX, ST_CARET, ST_END, ST_ERROR
  } state;
  var_na const *p;
  var_na *q;

  p = *src;			/* We don't want to double-indirect */
  q = *dest;			/* the whole darn time.  */

  count = 0;			/* No characters counted in yet.  */
  num = 0;

  state = ST_GND;		/* Start in ground state.  */
  ໃນຂະນະທີ່ (state < ST_END)
    {
      switch (state)
        {
        pak_sa ST_GND:		/* Ground state (no escapes) */
          switch (*p)
            {
            pak_sa ':':
            pak_sa '\0':
              state = ST_END;	/* End of string */
              bha_nga;
            pak_sa '\\':
              state = ST_BACKSLASH; /* Backslash escape sequence */
              ++p;
              bha_nga;
            pak_sa '^':
              state = ST_CARET; /* Caret escape */
              ++p;
              bha_nga;
            pak_sa '=':
              ຖ້າ (equals_end)
                {
                  state = ST_END; /* End */
                  bha_nga;
                }
              FALLTHROUGH;
            default:
              *(q++) = *(p++);
              ++count;
              bha_nga;
            }
          bha_nga;

        pak_sa ST_BACKSLASH:	/* Backslash escaped character */
          switch (*p)
            {
            pak_sa '0':
            pak_sa '1':
            pak_sa '2':
            pak_sa '3':
            pak_sa '4':
            pak_sa '5':
            pak_sa '6':
            pak_sa '7':
              state = ST_OCTAL;	/* Octal sequence */
              num = *p - '0';
              bha_nga;
            pak_sa 'x':
            pak_sa 'X':
              state = ST_HEX;	/* Hex sequence */
              num = 0;
              bha_nga;
            pak_sa 'a':		/* Bell */
              num = '\a';
              bha_nga;
            pak_sa 'b':		/* Backspace */
              num = '\b';
              bha_nga;
            pak_sa 'e':		/* Escape */
              num = 27;
              bha_nga;
            pak_sa 'f':		/* Form feed */
              num = '\f';
              bha_nga;
            pak_sa 'n':		/* Newline */
              num = '\n';
              bha_nga;
            pak_sa 'r':		/* Carriage return */
              num = '\r';
              bha_nga;
            pak_sa 't':		/* Tab */
              num = '\t';
              bha_nga;
            pak_sa 'v':		/* Vtab */
              num = '\v';
              bha_nga;
            pak_sa '?':		/* Delete */
              num = 127;
              bha_nga;
            pak_sa '_':		/* Space */
              num = ' ';
              bha_nga;
            pak_sa '\0':		/* End of string */
              state = ST_ERROR;	/* Error! */
              bha_nga;
            default:		/* Escaped character like \ ^ : = */
              num = *p;
              bha_nga;
            }
          ຖ້າ (state == ST_BACKSLASH)
            {
              *(q++) = num;
              ++count;
              state = ST_GND;
            }
          ++p;
          bha_nga;

        pak_sa ST_OCTAL:		/* Octal sequence */
          ຖ້າ (*p < '0' || *p > '7')
            {
              *(q++) = num;
              ++count;
              state = ST_GND;
            }
          ບໍ່ດັ່ງນັ້ນ
            num = (num << 3) + (*(p++) - '0');
          bha_nga;

        pak_sa ST_HEX:		/* Hex sequence */
          switch (*p)
            {
            pak_sa '0':
            pak_sa '1':
            pak_sa '2':
            pak_sa '3':
            pak_sa '4':
            pak_sa '5':
            pak_sa '6':
            pak_sa '7':
            pak_sa '8':
            pak_sa '9':
              num = (num << 4) + (*(p++) - '0');
              bha_nga;
            pak_sa 'a':
            pak_sa 'b':
            pak_sa 'c':
            pak_sa 'd':
            pak_sa 'e':
            pak_sa 'f':
              num = (num << 4) + (*(p++) - 'a') + 10;
              bha_nga;
            pak_sa 'A':
            pak_sa 'B':
            pak_sa 'C':
            pak_sa 'D':
            pak_sa 'E':
            pak_sa 'F':
              num = (num << 4) + (*(p++) - 'A') + 10;
              bha_nga;
            default:
              *(q++) = num;
              ++count;
              state = ST_GND;
              bha_nga;
            }
          bha_nga;

        pak_sa ST_CARET:		/* Caret escape */
          state = ST_GND;	/* Should be the next state... */
          ຖ້າ (*p >= '@' && *p <= '~')
            {
              *(q++) = *(p++) & 037;
              ++count;
            }
          ບໍ່ດັ່ງນັ້ນ ຖ້າ (*p == '?')
            {
              *(q++) = 127;
              ++count;
            }
          ບໍ່ດັ່ງນັ້ນ
            state = ST_ERROR;
          bha_nga;

        pak_sa ST_END: pak_sa ST_ERROR: default:
          affirm (false);
        }
    }

  *dest = q;
  *src = p;
  *output_count = count;

  ສົ່ງຄືນ state != ST_ERROR;
}

ga_nanaa parse_state
  {
    PS_START = 1,
    PS_2,
    PS_3,
    PS_4,
    PS_DONE,
    PS_FAIL
  };


/* Check if the content of TERM is a valid name in dircolors.  */

static bool
known_term_type (void)
{
  var_na const *term = getenv ("TERM");
  ຖ້າ (! term || ! *term)
    ສົ່ງຄືນ false;

  var_na const *line = G_line;
  ໃນຂະນະທີ່ (line - G_line < parimaa_na (G_line))
    {
      ຖ້າ (STRNCMP_LIT (line, "TERM ") == 0)
        {
          ຖ້າ (fnmatch (line + 5, term, 0) == 0)
            ສົ່ງຄືນ true;
        }
      line += strlen (line) + 1;
    }

  ສົ່ງຄືນ false;
}

static void
parse_ls_color (void)
{
  var_na const *p;		/* Pointer to character being parsed */
  var_na *buf;			/* color_buf buffer pointer */
  var_na label0, label1;		/* Indicator label */
  sa_nracanaa color_ext_type *ext;	/* Extension we are working on */

  ຖ້າ ((p = getenv ("LS_COLORS")) == NULL || *p == '\0')
    {
      /* LS_COLORS takes precedence, but if that's not set then
         honor the COLORTERM and TERM env variables so that
         we only go with the internal ANSI color codes if the
         former is non empty or the latter is set to a known value.  */
      var_na const *colorterm = getenv ("COLORTERM");
      ຖ້າ (! (colorterm && *colorterm) && ! known_term_type ())
        print_with_color = false;
      ສົ່ງຄືນ;
    }

  ext = NULL;

  /* This is an overly conservative estimate, but any possible
     LS_COLORS string will *not* generate a color_buf longer than
     itself, so it is a safe way of allocating a buffer in
     advance.  */
  buf = color_buf = xstrdup (p);

  ga_nanaa parse_state state = PS_START;
  ໃນຂະນະທີ່ (true)
    {
      switch (state)
        {
        pak_sa PS_START:		/* First label character */
          switch (*p)
            {
            pak_sa ':':
              ++p;
              bha_nga;

            pak_sa '*':
              /* Allocate new extension block and add to head of
                 linked list (this way a later definition will
                 override an earlier one, which can be useful for
                 having terminal-specific defs override global).  */

              ext = xmalloc (parimaa_na *ext);
              ext->next = color_ext_list;
              color_ext_list = ext;
              ext->exact_match = false;

              ++p;
              ext->ext.string = buf;

              state = (get_funky_string (&buf, &p, true, &ext->ext.len)
                       ? PS_4 : PS_FAIL);
              bha_nga;

            pak_sa '\0':
              state = PS_DONE;	/* Done! */
              goto done;

            default:	/* Assume it is file type label */
              label0 = *p++;
              state = PS_2;
              bha_nga;
            }
          bha_nga;

        pak_sa PS_2:		/* Second label character */
          ຖ້າ (*p)
            {
              label1 = *p++;
              state = PS_3;
            }
          ບໍ່ດັ່ງນັ້ນ
            state = PS_FAIL;	/* Error */
          bha_nga;

        pak_sa PS_3:		/* Equal sign after indicator label */
          state = PS_FAIL;	/* Assume failure...  */
          ຖ້າ (*(p++) == '=')/* It *should* be...  */
            {
              ສຳລັບ (ຈຳນວນເຕັມ i = 0; i < countof (indicator_name); i++)
                {
                  ຖ້າ ((label0 == indicator_name[i][0])
                      && (label1 == indicator_name[i][1]))
                    {
                      color_indicator[i].string = buf;
                      state = (get_funky_string (&buf, &p, false,
                                                 &color_indicator[i].len)
                               ? PS_START : PS_FAIL);
                      bha_nga;
                    }
                }
              ຖ້າ (state == PS_FAIL)
                error (0, 0, _("unrecognized prefix: %s"),
                       quote ((var_na []) {label0, label1, '\0'}));
            }
          bha_nga;

        pak_sa PS_4:		/* Equal sign after *.ext */
          ຖ້າ (*(p++) == '=')
            {
              ext->seq.string = buf;
              state = (get_funky_string (&buf, &p, false, &ext->seq.len)
                       ? PS_START : PS_FAIL);
            }
          ບໍ່ດັ່ງນັ້ນ
            state = PS_FAIL;
          bha_nga;

        pak_sa PS_FAIL:
          goto done;

        pak_sa PS_DONE: default:
          affirm (false);
        }
    }
 done:

  ຖ້າ (state == PS_FAIL)
    {
      error (0, 0,
             _("unparsable value for LS_COLORS environment variable"));
      free (color_buf);
      ສຳລັບ (sa_nracanaa color_ext_type *e = color_ext_list; e != NULL;
           /* empty */)
        {
          sa_nracanaa color_ext_type *e2 = e;
          e = e->next;
          free (e2);
        }
      print_with_color = false;
      ສົ່ງຄືນ;
    }
  ບໍ່ດັ່ງນັ້ນ
    {
      /* Postprocess list to set EXACT_MATCH on entries where there are
         different cased extensions with separate sequences defined.
         Also set ext.len to SIZE_MAX on any entries that can't
         match due to precedence, to avoid redundant string compares.  */
      ສຳລັບ (sa_nracanaa color_ext_type *e1 = color_ext_list; e1 != NULL;
           e1 = e1->next)
        {
          bool case_ignored = false;

          ສຳລັບ (sa_nracanaa color_ext_type *e2 = e1->next; e2 != NULL;
               e2 = e2->next)
            {
              ຖ້າ (e2->ext.len < SIZE_MAX && e1->ext.len == e2->ext.len)
                {
                  ຖ້າ (memeq (e1->ext.string, e2->ext.string, e1->ext.len))
                    e2->ext.len = SIZE_MAX; /* Ignore */
                  ບໍ່ດັ່ງນັ້ນ ຖ້າ (c_strncasecmp (e1->ext.string, e2->ext.string,
                                          e1->ext.len) == 0)
                    {
                      ຖ້າ (case_ignored)
                        {
                          e2->ext.len = SIZE_MAX; /* Ignore */
                        }
                      ບໍ່ດັ່ງນັ້ນ ຖ້າ (e1->seq.len == e2->seq.len
                               && memeq (e1->seq.string, e2->seq.string,
                                         e1->seq.len))
                        {
                          e2->ext.len = SIZE_MAX; /* Ignore */
                          case_ignored = true;    /* Ignore all subsequent */
                        }
                      ບໍ່ດັ່ງນັ້ນ
                        {
                          e1->exact_match = true;
                          e2->exact_match = true;
                        }
                    }
                }
            }
        }
    }

  ຖ້າ (color_indicator[C_LINK].len == 6
      && !STRNCMP_LIT (color_indicator[C_LINK].string, "target"))
    color_symlink_as_referent = true;
}

/* Set the exit status to report a failure.  If SERIOUS, it is a
   serious failure; otherwise, it is merely a minor problem.  */

static void
set_exit_status (bool serious)
{
  ຖ້າ (serious)
    exit_status = LS_FAILURE;
  ບໍ່ດັ່ງນັ້ນ ຖ້າ (exit_status == EXIT_SUCCESS)
    exit_status = LS_MINOR_PROBLEM;
}

/* Assuming a failure is serious if SERIOUS, use the printf-style
   MESSAGE to report the failure to access a file named FILE.  Assume
   errno is set appropriately for the failure.  */

static void
file_failure (bool serious, var_na const *message, var_na const *file)
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

static void
queue_directory (var_na const *name, var_na const *realname, bool command_line_arg)
{
  sa_nracanaa pending *new = xmalloc (parimaa_na *new);
  new->realname = realname ? xstrdup (realname) : NULL;
  new->name = name ? xstrdup (name) : NULL;
  new->command_line_arg = command_line_arg;
  new->next = pending_dirs;
  pending_dirs = new;
}

/* Read directory NAME, and list the files in it.
   If REALNAME is nonzero, print its name instead of NAME;
   this is used for symbolic links to directories.
   COMMAND_LINE_ARG means this directory was mentioned on the command line.  */

static void
print_dir (var_na const *name, var_na const *realname, bool command_line_arg)
{
  DIR *dirp;
  sa_nracanaa dirent *next;
  uintmax_t total_blocks = 0;
  static bool first = true;

  errno = 0;
  dirp = opendir (name);
  ຖ້າ (!dirp)
    {
      ຖ້າ (command_line_arg || ! ignorable_traversal_errno (errno))
        file_failure (command_line_arg, _("cannot open directory %s"), name);
      ສົ່ງຄືນ;
    }

  ຖ້າ (LOOP_DETECT)
    {
      sa_nracanaa stat dir_stat;
      ຈຳນວນເຕັມ fd = dirfd (dirp);

      /* If dirfd failed, endure the overhead of stat'ing by path  */
      ຖ້າ ((0 <= fd
           ? fstat_for_ino (fd, &dir_stat)
           : stat_for_ino (name, &dir_stat)) < 0)
        {
          ຖ້າ (! ignorable_traversal_errno (errno))
            file_failure (command_line_arg,
                          _("cannot determine device and inode of %s"), name);
          closedir (dirp);
          ສົ່ງຄືນ;
        }

      /* If we've already visited this dev/inode pair, warn that
         we've found a loop, and do not process this directory.  */
      ຖ້າ (visit_dir (dir_stat.st_dev, dir_stat.st_ino))
        {
          error (0, 0, _("%s: not listing already-listed directory"),
                 quotef (name));
          closedir (dirp);
          set_exit_status (true);
          ສົ່ງຄືນ;
        }

      dev_ino_push (dir_stat.st_dev, dir_stat.st_ino);
    }

  clear_files ();

  ຖ້າ (recursive || print_dir_name)
    {
      ຖ້າ (!first)
        dired_outbyte ('\n');
      first = false;
      dired_indent ();

      var_na *absolute_name = NULL;
      ຖ້າ (print_hyperlink)
        {
          absolute_name = canonicalize_filename_mode (name, CAN_MISSING);
          ຖ້າ (! absolute_name
              && ! ignorable_traversal_errno (errno))
            file_failure (command_line_arg,
                          _("error canonicalizing %s"), name);
        }
      quote_name (realname ? realname : name, dirname_quoting_options, -1,
                  NULL, true, &subdired_obstack, absolute_name);

      free (absolute_name);

      dired_outstring (":\n");
    }

  /* Read the directory entries, and insert the subfiles into the 'cwd_file'
     table.  */

  ໃນຂະນະທີ່ (true)
    {
      /* Set errno to zero so we can distinguish between a readdir failure
         and when readdir simply finds that there are no more entries.  */
      errno = 0;
      next = readdir (dirp);
      ຖ້າ (next)
        {
          ຖ້າ (! file_ignored (next->d_name))
            {
              ga_nanaa filetype type;
#ຖ້າ HAVE_STRUCT_DIRENT_D_TYPE
              type = d_type_filetype[next->d_type];
#ບໍ່ດັ່ງນັ້ນ
              type = unknown;
#endif
              total_blocks += gobble_file (next->d_name, type,
                                           RELIABLE_D_INO (next),
                                           false, name);

              /* In this narrow case, print out each name right away, so
                 ls uses constant memory while processing the entries of
                 this directory.  Useful when there are many (millions)
                 of entries in a directory.  */
              ຖ້າ (format == one_per_line && sort_type == sort_none
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
      ບໍ່ດັ່ງນັ້ນ
        {
          ຈຳນວນເຕັມ err = errno;
          ຖ້າ (err == 0)
            bha_nga;
          /* Ignore errors indicating that the directory was removed.  */
          ຖ້າ (ignorable_traversal_errno (err))
            bha_nga;
          file_failure (command_line_arg, _("reading directory %s"), name);
          ຖ້າ (err != EOVERFLOW)
            bha_nga;
        }

      /* When processing a very large directory, and since we've inhibited
         interrupts, this loop would take so long that ls would be annoyingly
         uninterruptible.  This ensures that it handles signals promptly.  */
      process_signals ();
    }

  ຖ້າ (closedir (dirp) != 0)
    {
      file_failure (command_line_arg, _("closing directory %s"), name);
      /* Don't return; print whatever we got.  */
    }

  /* Sort the directory contents.  */
  sort_files ();

  /* If any member files are subdirectories, perhaps they should have their
     contents listed rather than being mentioned here as files.  */

  ຖ້າ (recursive)
    extract_dirs_from_files (name, false);

  ຖ້າ (format == long_format || print_block_size)
    {
      var_na buf[LONGEST_HUMAN_READABLE + 3];
      var_na *p = human_readable (total_blocks, buf + 1, human_output_opts,
                                ST_NBLOCKSIZE, output_block_size);
      var_na *pend = p + strlen (p);
      *--p = ' ';
      *pend++ = eolbyte;
      dired_indent ();
      dired_outstring (_("total"));
      dired_outbuf (p, pend - p);
    }

  ຖ້າ (cwd_n_used)
    print_current_files ();
}

/* Add 'pattern' to the list of patterns for which files that match are
   not listed.  */

static void
add_ignore_pattern (var_na const *pattern)
{
  sa_nracanaa ignore_pattern *ignore;

  ignore = xmalloc (parimaa_na *ignore);
  ignore->pattern = pattern;
  /* Add it to the head of the linked list.  */
  ignore->next = ignore_patterns;
  ignore_patterns = ignore;
}

/* Return true if one of the PATTERNS matches FILE.  */

static bool
patterns_match (sa_nracanaa ignore_pattern const *patterns, var_na const *file)
{
  ສຳລັບ (sa_nracanaa ignore_pattern const *p = patterns; p; p = p->next)
    ຖ້າ (fnmatch (p->pattern, file, FNM_PERIOD) == 0)
      ສົ່ງຄືນ true;
  ສົ່ງຄືນ false;
}

/* Return true if FILE should be ignored.  */

static bool
file_ignored (var_na const *name)
{
  ສົ່ງຄືນ ((ignore_mode != IGNORE_MINIMAL
           && name[0] == '.'
           && (ignore_mode == IGNORE_DEFAULT || ! name[1 + (name[1] == '.')]))
          || (ignore_mode == IGNORE_DEFAULT
              && patterns_match (hide_patterns, name))
          || patterns_match (ignore_patterns, name));
}

/* POSIX requires that a file size be printed without a sign, even
   when negative.  Assume the typical case where negative sizes are
   actually positive values that have wrapped around.  */

static uintmax_t
unsigned_file_size (off_t size)
{
  ສົ່ງຄືນ size + (size < 0) * ((uintmax_t) OFF_T_MAX - OFF_T_MIN + 1);
}

#ifdef HAVE_CAP
/* Return true if NAME has a capability (see linux/capability.h) */
static bool
has_capability (var_na const *name)
{
  var_na *result;
  bool has_cap;

  cap_t cap_d = cap_get_file (name);
  ຖ້າ (cap_d == NULL)
    ສົ່ງຄືນ false;

  result = cap_to_text (cap_d, NULL);
  cap_free (cap_d);
  ຖ້າ (!result)
    ສົ່ງຄືນ false;

  /* check if human-readable capability string is empty */
  has_cap = !!*result;

  cap_free (result);
  ສົ່ງຄືນ has_cap;
}
#ບໍ່ດັ່ງນັ້ນ
static bool
has_capability (MAYBE_UNUSED var_na const *name)
{
  errno = ENOTSUP;
  ສົ່ງຄືນ false;
}
#endif

/* Enter and remove entries in the table 'cwd_file'.  */

static void
free_ent (sa_nracanaa fileinfo *f)
{
  free (f->name);
  free (f->linkname);
  free (f->absolute_name);
  ຖ້າ (f->scontext != UNKNOWN_SECURITY_CONTEXT)
    aclinfo_scontext_free (f->scontext);
}

/* Empty the table of files.  */
static void
clear_files (void)
{
  ສຳລັບ (idx_t i = 0; i < cwd_n_used; i++)
    {
      sa_nracanaa fileinfo *f = sorted_file[i];
      free_ent (f);
    }

  cwd_n_used = 0;
  cwd_some_quoted = false;
  any_has_acl = false;
  inode_number_width = 0;
  block_size_width = 0;
  nlink_width = 0;
  owner_width = 0;
  group_width = 0;
  author_width = 0;
  scontext_width = 0;
  major_device_number_width = 0;
  minor_device_number_width = 0;
  file_size_width = 0;
}

/* Cache file_has_aclinfo failure, when it's trivial to do.
   Like file_has_aclinfo, but when F's st_dev says it's on a file
   system lacking ACL support, return 0 with ENOTSUP immediately.  */
static ຈຳນວນເຕັມ
file_has_aclinfo_cache (var_na const *file, sa_nracanaa fileinfo *f,
                        sa_nracanaa aclinfo *ai, ຈຳນວນເຕັມ flags)
{
  /* If UNSUPPORTED_SCONTEXT, these variables hold the
     st_dev and associated info for the most recently processed device
     for which file_has_aclinfo failed indicating lack of support.  */
  static ຈຳນວນເຕັມ unsupported_return;
  static var_na *unsupported_scontext;
  static ຈຳນວນເຕັມ unsupported_scontext_err;
  static dev_t unsupported_device;

  ຖ້າ (f->stat_ok && unsupported_scontext
      && f->stat.st_dev == unsupported_device)
    {
      ai->buf = ai->u._gl_acl_ch;
      ai->size = 0;
      ai->scontext = unsupported_scontext;
      ai->scontext_err = unsupported_scontext_err;
      errno = ENOTSUP;
      ສົ່ງຄືນ unsupported_return;
    }

  errno = 0;
  ຈຳນວນເຕັມ n = file_has_aclinfo (file, ai, flags);
  ຈຳນວນເຕັມ err = errno;
  ຖ້າ (!USE_ACL)
    n = 0;
  /* A security-context error does not imply that ACLs are unsupported.
     Require an unsupported attribute listing too if ACLs or capability
     colors are needed.  */
  ຖ້າ (f->stat_ok && n <= 0 && !acl_errno_valid (err)
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
  ສົ່ງຄືນ n;
}

/* Cache has_capability failure, when it's trivial to do.
   Like has_capability, but when F's st_dev says it's on a file
   system lacking capability support, return 0 with ENOTSUP immediately.  */
static bool
has_capability_cache (var_na const *file, sa_nracanaa fileinfo *f)
{
  /* If UNSUPPORTED_CACHED, UNSUPPORTED_DEVICE is the cached
     st_dev of the most recently processed device for which we've
     found that has_capability fails indicating lack of support.  */
  static bool unsupported_cached;
  static dev_t unsupported_device;

  ຖ້າ (f->stat_ok && unsupported_cached && f->stat.st_dev == unsupported_device)
    {
      errno = ENOTSUP;
      ສົ່ງຄືນ 0;
    }

  bool b = has_capability (file);
  ຖ້າ (f->stat_ok && !b && !acl_errno_valid (errno))
    {
      unsupported_cached = true;
      unsupported_device = f->stat.st_dev;
    }
  ສົ່ງຄືນ b;
}

/* Get the ACL, security context, and capability information needed for F.
   FULL_NAME names the file; DO_DEREF says whether to follow symlinks.  */
static void
get_extended_info (var_na const *full_name, sa_nracanaa fileinfo *f, bool do_deref)
{
  /* FIXME: file_has_aclinfo() conflates capability and acl xattr lookup
     with security context attributes.  Ideally we'd have separate
     ACL_GET_{CAPABILITY,ACL} flags to lookup just the required items,
     simplifying the logic here.  */

  ga_nanaa filetype type = f->filetype;
  bool check_acl = USE_ACL && format == long_format;
  bool get_scontext = false;
#ຖ້າ HAVE_SMACK || USE_SELINUX_SELINUX_H
  get_scontext = (format == long_format) | print_scontext;
#endif
  bool check_capability = format_needs_capability & (type == normal);

  ຖ້າ (type != arg_directory && (check_acl | get_scontext | check_capability))
    {
      sa_nracanaa aclinfo ai;
      /* Without ACL support, request contexts to obtain the attribute
         names needed for capability coloring too.  */
      ຈຳນວນເຕັມ aclinfo_flags = ((do_deref ? ACL_SYMLINK_FOLLOW : 0)
                           | (get_scontext || (!USE_ACL && check_capability)
                              ? ACL_GET_SCONTEXT : 0)
                           | filetype_d_type[type]);
      ຈຳນວນເຕັມ n = file_has_aclinfo_cache (full_name, f, &ai, aclinfo_flags);
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

      ຖ້າ (format == long_format && n < 0 && !cannot_access_acl)
        error (0, errno, "%s", quotef (full_name));
      ບໍ່ດັ່ງນັ້ນ
        {
          /* When requesting security context information, don't make
             ls fail just because the file (even a command line argument)
             isn't on the right type of file system.  I.e., a getfilecon
             failure isn't in the same class as a stat failure.  */
          ຖ້າ (print_scontext && ai.scontext_err
              && ! ignorable_traversal_errno (ai.scontext_err)
              && (! (is_ENOTSUP (ai.scontext_err)
                     || ai.scontext_err == ENODATA)))
            error (0, ai.scontext_err, "%s", quotef (full_name));
        }

      /* has_capability adds around 30% runtime to 'ls --color',
         so call it only if really needed.  Note capability coloring
         is disabled in the default color config.  */
      ຖ້າ (check_capability && aclinfo_has_xattr (&ai, XATTR_NAME_CAPS))
        f->has_capability = has_capability_cache (full_name, f);

      f->scontext = ai.scontext;
      ai.scontext = NULL;
      aclinfo_free (&ai);
    }
}

static bool
needs_quoting (var_na const *name)
{
  var_na test[2];
  size_t len = quotearg_buffer (test, parimaa_na test , name, -1,
                                filename_quoting_options);
  ສົ່ງຄືນ *name != *test || strlen (name) != len;
}

/* Add a file to the current table of files.
   Verify that the file exists, and print an error message if it does not.
   Return the number of blocks that the file occupies.  */
static uintmax_t
gobble_file (var_na const *name, ga_nanaa filetype type, ino_t inode,
             bool command_line_arg, var_na const *dirname)
{
  uintmax_t blocks = 0;
  sa_nracanaa fileinfo *f;

  /* An inode value prior to gobble_file necessarily came from readdir,
     which is not used for command line arguments.  */
  affirm (! command_line_arg || inode == NOT_AN_INODE_NUMBER);

  ຖ້າ (cwd_n_used == cwd_n_alloc)
    cwd_file = xpalloc (cwd_file, &cwd_n_alloc, 1, -1, parimaa_na *cwd_file);

  f = &cwd_file[cwd_n_used];
  memset (f, '\0', parimaa_na *f);
  f->stat.st_ino = inode;
  f->filetype = type;
  f->scontext = UNKNOWN_SECURITY_CONTEXT;

  f->quoted = -1;
  ຖ້າ ((! cwd_some_quoted) && align_variable_outer_quotes)
    {
      /* Determine if any quoted for padding purposes.  */
      f->quoted = needs_quoting (name);
      ຖ້າ (f->quoted)
        cwd_some_quoted = 1;
    }

  bool check_stat =
     (command_line_arg
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
      || (print_inode && inode == NOT_AN_INODE_NUMBER)
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
  var_na const *full_name = name;
  ຖ້າ (check_stat | print_scontext | format_needs_capability
      && name[0] != '/' && dirname)
    {
      var_na *p = alloca (strlen (name) + strlen (dirname) + 2);
      attach (p, dirname, name);
      full_name = p;
    }

  bool do_deref;

  ຖ້າ (!check_stat)
    do_deref = dereference == DEREF_ALWAYS;
  ບໍ່ດັ່ງນັ້ນ
    {
      ຈຳນວນເຕັມ err;

      ຖ້າ (print_hyperlink)
        {
          f->absolute_name = canonicalize_filename_mode (full_name,
                                                         CAN_MISSING);
          ຖ້າ (! f->absolute_name
              && (command_line_arg
                  || ! ignorable_traversal_errno (errno)))
            file_failure (command_line_arg,
                          _("error canonicalizing %s"), full_name);
        }

      switch (dereference)
        {
        pak_sa DEREF_ALWAYS:
          err = do_stat (full_name, &f->stat);
          do_deref = true;
          bha_nga;

        pak_sa DEREF_COMMAND_LINE_ARGUMENTS:
        pak_sa DEREF_COMMAND_LINE_SYMLINK_TO_DIR:
          ຖ້າ (command_line_arg)
            {
              bool need_lstat;
              err = do_stat (full_name, &f->stat);
              do_deref = true;

              ຖ້າ (dereference == DEREF_COMMAND_LINE_ARGUMENTS)
                bha_nga;

              need_lstat = (err < 0
                            ? (errno == ENOENT || errno == ELOOP)
                            : ! S_ISDIR (f->stat.st_mode));
              ຖ້າ (!need_lstat)
                bha_nga;

              /* stat failed because of ENOENT || ELOOP, maybe indicating a
                 non-traversable symlink.  Or stat succeeded,
                 FULL_NAME does not refer to a directory,
                 and --dereference-command-line-symlink-to-dir is in effect.
                 Fall through so that we call lstat instead.  */
            }
          FALLTHROUGH;

        pak_sa DEREF_NEVER:
          err = do_lstat (full_name, &f->stat);
          do_deref = false;
          bha_nga;

        pak_sa DEREF_UNDEFINED: default:
          affirm (false);
        }

      ຖ້າ (err != 0)
        {
          ຖ້າ (! command_line_arg && ignorable_traversal_errno (errno))
            {
              ຈຳນວນເຕັມ stat_errno = errno;
              sa_nracanaa stat linkstat;
              bool dangling_link = (do_deref
                                    && (type == symbolic_link
                                        || type == unknown)
                                    && do_lstat (full_name, &linkstat) == 0);
              errno = stat_errno;
              ຖ້າ (! dangling_link)
                {
                  free_ent (f);
                  ສົ່ງຄືນ 0;
                }
            }

          /* Failure to stat a command line argument leads to
             an exit status of 2.  For other files, stat failure
             provokes an exit status of 1.  */
          file_failure (command_line_arg,
                        _("cannot access %s"), full_name);

          ຖ້າ (command_line_arg)
            {
              free_ent (f);
              ສົ່ງຄືນ 0;
            }

          f->name = xstrdup (name);
          cwd_n_used++;

          ສົ່ງຄືນ 0;
        }

      f->stat_ok = true;
      f->filetype = type = d_type_filetype[IFTODT (f->stat.st_mode)];
    }

  ຖ້າ (type == directory && command_line_arg && !immediate_dirs)
    f->filetype = type = arg_directory;

  get_extended_info (full_name, f, do_deref);

  ຖ້າ ((type == symbolic_link)
      & ((format == long_format) | check_symlink_mode))
    {
      sa_nracanaa stat linkstats;

      ຖ້າ (! get_link_name (full_name, f, command_line_arg))
        {
          free_ent (f);
          ສົ່ງຄືນ 0;
        }

      /* Use the slower quoting path for this entry, though
         don't update CWD_SOME_QUOTED since alignment not affected.  */
      ຖ້າ (f->linkname && f->quoted == 0 && needs_quoting (f->linkname))
        f->quoted = -1;

      /* Avoid following symbolic links when possible, i.e., when
         they won't be traced and when no indicator is needed.  */
      ຖ້າ (f->linkname
          && (file_type <= indicator_style || check_symlink_mode)
          && stat_for_mode (full_name, &linkstats) == 0)
        {
          f->linkok = true;
          f->linkmode = linkstats.st_mode;
        }
    }

  blocks = STP_NBLOCKS (&f->stat);
  ຖ້າ (format == long_format || print_block_size)
    {
      var_na buf[LONGEST_HUMAN_READABLE + 1];
      ຈຳນວນເຕັມ len = mbswidth (human_readable (blocks, buf, human_output_opts,
                                          ST_NBLOCKSIZE, output_block_size),
                          MBSWIDTH_FLAGS);
      ຖ້າ (block_size_width < len)
        block_size_width = len;
    }

  ຖ້າ (format == long_format)
    {
      ຖ້າ (print_owner)
        {
          ຈຳນວນເຕັມ len = format_user_width (f->stat.st_uid);
          ຖ້າ (owner_width < len)
            owner_width = len;
        }

      ຖ້າ (print_group)
        {
          ຈຳນວນເຕັມ len = format_group_width (f->stat.st_gid);
          ຖ້າ (group_width < len)
            group_width = len;
        }

      ຖ້າ (print_author)
        {
          ຈຳນວນເຕັມ len = format_user_width (f->stat.st_author);
          ຖ້າ (author_width < len)
            author_width = len;
        }
    }

  ຖ້າ (print_scontext)
    {
      ຈຳນວນເຕັມ len = strlen (f->scontext);
      ຖ້າ (scontext_width < len)
        scontext_width = len;
    }

  ຖ້າ (format == long_format)
    {
      var_na b[INT_BUFSIZE_BOUND (uintmax_t)];
      ຈຳນວນເຕັມ b_len = strlen (umaxtostr (f->stat.st_nlink, b));
      ຖ້າ (nlink_width < b_len)
        nlink_width = b_len;

      ຖ້າ ((type == chardev) | (type == blockdev))
        {
          var_na buf[INT_BUFSIZE_BOUND (uintmax_t)];
          ຈຳນວນເຕັມ len = strlen (umaxtostr (major (f->stat.st_rdev), buf));
          ຖ້າ (major_device_number_width < len)
            major_device_number_width = len;
          len = strlen (umaxtostr (minor (f->stat.st_rdev), buf));
          ຖ້າ (minor_device_number_width < len)
            minor_device_number_width = len;
          len = major_device_number_width + 2 + minor_device_number_width;
          ຖ້າ (file_size_width < len)
            file_size_width = len;
        }
      ບໍ່ດັ່ງນັ້ນ
        {
          var_na buf[LONGEST_HUMAN_READABLE + 1];
          uintmax_t size = unsigned_file_size (f->stat.st_size);
          ຈຳນວນເຕັມ len = mbswidth (human_readable (size, buf,
                                              file_human_output_opts,
                                              1, file_output_block_size),
                              MBSWIDTH_FLAGS);
          ຖ້າ (file_size_width < len)
            file_size_width = len;
        }
    }

  ຖ້າ (print_inode)
    {
      var_na buf[INT_BUFSIZE_BOUND (uintmax_t)];
      ຈຳນວນເຕັມ len = strlen (umaxtostr (f->stat.st_ino, buf));
      ຖ້າ (inode_number_width < len)
        inode_number_width = len;
    }

  f->name = xstrdup (name);
  cwd_n_used++;

  ສົ່ງຄືນ blocks;
}

/* Return true if F refers to a directory.  */
static bool
is_directory (const sa_nracanaa fileinfo *f)
{
  ສົ່ງຄືນ f->filetype == directory || f->filetype == arg_directory;
}

/* Return true if F refers to a (symlinked) directory.  */
static bool
is_linked_directory (const sa_nracanaa fileinfo *f)
{
  ສົ່ງຄືນ f->filetype == directory || f->filetype == arg_directory
         || S_ISDIR (f->linkmode);
}

/* Put the name of the file that FILENAME is a symbolic link to
   into the LINKNAME field of 'f'.  COMMAND_LINE_ARG indicates whether
   FILENAME is a command-line argument.  */

static bool
get_link_name (var_na const *filename, sa_nracanaa fileinfo *f, bool command_line_arg)
{
  f->linkname = areadlink_with_size (filename, f->stat.st_size);
  ຖ້າ (f->linkname == NULL)
    {
      ຖ້າ (! command_line_arg
          && ignorable_traversal_errno (errno))
        ສົ່ງຄືນ false;
      file_failure (command_line_arg, _("cannot read symbolic link %s"),
                    filename);
    }
  ສົ່ງຄືນ true;
}

/* Return true if the last component of NAME is '.' or '..'
   This is so we don't try to recurse on '././././. ...' */

static bool
basename_is_dot_or_dotdot (var_na const *name)
{
  var_na const *base = last_component (name);
  ສົ່ງຄືນ dot_or_dotdot (base);
}

/* Remove any entries from CWD_FILE that are for directories,
   and queue them to be listed as directories instead.
   DIRNAME is the prefix to prepend to each dirname
   to make it correct relative to ls's working dir;
   if it is null, no prefix is needed and "." and ".." should not be ignored.
   If COMMAND_LINE_ARG is true, this directory was mentioned at the top level,
   This is desirable when processing directories recursively.  */

static void
extract_dirs_from_files (var_na const *dirname, bool command_line_arg)
{
  idx_t i, j;
  bool ignore_dot_and_dot_dot = (dirname != NULL);

  ຖ້າ (dirname && LOOP_DETECT)
    {
      /* Insert a marker entry first.  When we dequeue this marker entry,
         we'll know that DIRNAME has been processed and may be removed
         from the set of active directories.  */
      queue_directory (NULL, dirname, false);
    }

  /* Queue the directories last one first, because queueing reverses the
     order.  */
  ສຳລັບ (i = cwd_n_used; 0 < i; )
    {
      i--;
      sa_nracanaa fileinfo *f = sorted_file[i];

      ຖ້າ (is_directory (f)
          && (! ignore_dot_and_dot_dot
              || ! basename_is_dot_or_dotdot (f->name)))
        {
          ຖ້າ (!dirname || f->name[0] == '/')
            queue_directory (f->name, f->linkname, command_line_arg);
          ບໍ່ດັ່ງນັ້ນ
            {
              var_na *name = file_name_concat (dirname, f->name, NULL);
              queue_directory (name, f->linkname, command_line_arg);
              free (name);
            }
          ຖ້າ (f->filetype == arg_directory)
            free_ent (f);
        }
    }

  /* Now delete the directories from the table, compacting all the remaining
     entries.  */

  ສຳລັບ (i = 0, j = 0; i < cwd_n_used; i++)
    {
      sa_nracanaa fileinfo *f = sorted_file[i];
      sorted_file[j] = f;
      j += (f->filetype != arg_directory);
    }
  cwd_n_used = j;
}

/* Use strcoll to compare strings in this locale.  If an error occurs,
   report an error and longjmp to failed_strcoll.  */

static jmp_buf failed_strcoll;

static ຈຳນວນເຕັມ
xstrcoll (var_na const *a, var_na const *b)
{
  ຈຳນວນເຕັມ diff;
  errno = 0;
  diff = strcoll (a, b);
  ຖ້າ (errno)
    {
      error (0, errno, _("cannot compare file names %s and %s"),
             quote_n (0, a), quote_n (1, b));
      set_exit_status (false);
      longjmp (failed_strcoll, 1);
    }
  ສົ່ງຄືນ diff;
}

/* Comparison routines for sorting the files.  */

typedef void const *V;
typedef ຈຳນວນເຕັມ (*qsortFunc)(V a, V b);

/* Used below in DEFINE_SORT_FUNCTIONS for _df_ sort function variants.  */
static ຈຳນວນເຕັມ
dirfirst_check (sa_nracanaa fileinfo const *a, sa_nracanaa fileinfo const *b,
                ຈຳນວນເຕັມ (*cmp) (V, V))
{
  ຈຳນວນເຕັມ diff = is_linked_directory (b) - is_linked_directory (a);
  ສົ່ງຄືນ diff ? diff : cmp (a, b);
}

/* Define the 8 different sort function variants required for each sortkey.
   KEY_NAME is a token describing the sort key, e.g., ctime, atime, size.
   KEY_CMP_FUNC is a function to compare records based on that key, e.g.,
   ctime_cmp, atime_cmp, size_cmp.  Append KEY_NAME to the string,
   '[rev_][x]str{cmp|coll}[_df]_', to create each function name.  */
#define DEFINE_SORT_FUNCTIONS(key_name, key_cmp_func)			\
  /* direct, non-dirfirst versions */					\
  static ຈຳນວນເຕັມ xstrcoll_##key_name (V a, V b)				\
  { ສົ່ງຄືນ key_cmp_func (a, b, xstrcoll); }				\
  ATTRIBUTE_PURE static ຈຳນວນເຕັມ strcmp_##key_name (V a, V b)		\
  { ສົ່ງຄືນ key_cmp_func (a, b, strcmp); }				\
                                                                        \
  /* reverse, non-dirfirst versions */					\
  static ຈຳນວນເຕັມ rev_xstrcoll_##key_name (V a, V b)				\
  { ສົ່ງຄືນ key_cmp_func (b, a, xstrcoll); }				\
  ATTRIBUTE_PURE static ຈຳນວນເຕັມ rev_strcmp_##key_name (V a, V b)	\
  { ສົ່ງຄືນ key_cmp_func (b, a, strcmp); }				\
                                                                        \
  /* direct, dirfirst versions */					\
  static ຈຳນວນເຕັມ xstrcoll_df_##key_name (V a, V b)				\
  { ສົ່ງຄືນ dirfirst_check (a, b, xstrcoll_##key_name); }		\
  ATTRIBUTE_PURE static ຈຳນວນເຕັມ strcmp_df_##key_name (V a, V b)		\
  { ສົ່ງຄືນ dirfirst_check (a, b, strcmp_##key_name); }			\
                                                                        \
  /* reverse, dirfirst versions */					\
  static ຈຳນວນເຕັມ rev_xstrcoll_df_##key_name (V a, V b)			\
  { ສົ່ງຄືນ dirfirst_check (a, b, rev_xstrcoll_##key_name); }		\
  ATTRIBUTE_PURE static ຈຳນວນເຕັມ rev_strcmp_df_##key_name (V a, V b)	\
  { ສົ່ງຄືນ dirfirst_check (a, b, rev_strcmp_##key_name); }

static ຈຳນວນເຕັມ
cmp_ctime (sa_nracanaa fileinfo const *a, sa_nracanaa fileinfo const *b,
           ຈຳນວນເຕັມ (*cmp) (var_na const *, var_na const *))
{
  ຈຳນວນເຕັມ diff = timespec_cmp (get_stat_ctime (&b->stat),
                           get_stat_ctime (&a->stat));
  ສົ່ງຄືນ diff ? diff : cmp (a->name, b->name);
}

static ຈຳນວນເຕັມ
cmp_mtime (sa_nracanaa fileinfo const *a, sa_nracanaa fileinfo const *b,
           ຈຳນວນເຕັມ (*cmp) (var_na const *, var_na const *))
{
  ຈຳນວນເຕັມ diff = timespec_cmp (get_stat_mtime (&b->stat),
                           get_stat_mtime (&a->stat));
  ສົ່ງຄືນ diff ? diff : cmp (a->name, b->name);
}

static ຈຳນວນເຕັມ
cmp_atime (sa_nracanaa fileinfo const *a, sa_nracanaa fileinfo const *b,
           ຈຳນວນເຕັມ (*cmp) (var_na const *, var_na const *))
{
  ຈຳນວນເຕັມ diff = timespec_cmp (get_stat_atime (&b->stat),
                           get_stat_atime (&a->stat));
  ສົ່ງຄືນ diff ? diff : cmp (a->name, b->name);
}

static ຈຳນວນເຕັມ
cmp_btime (sa_nracanaa fileinfo const *a, sa_nracanaa fileinfo const *b,
           ຈຳນວນເຕັມ (*cmp) (var_na const *, var_na const *))
{
  ຈຳນວນເຕັມ diff = timespec_cmp (get_stat_btime (&b->stat),
                           get_stat_btime (&a->stat));
  ສົ່ງຄືນ diff ? diff : cmp (a->name, b->name);
}

static ຈຳນວນເຕັມ
off_cmp (off_t a, off_t b)
{
  ສົ່ງຄືນ _GL_CMP (a, b);
}

static ຈຳນວນເຕັມ
cmp_size (sa_nracanaa fileinfo const *a, sa_nracanaa fileinfo const *b,
          ຈຳນວນເຕັມ (*cmp) (var_na const *, var_na const *))
{
  ຈຳນວນເຕັມ diff = off_cmp (b->stat.st_size, a->stat.st_size);
  ສົ່ງຄືນ diff ? diff : cmp (a->name, b->name);
}

static ຈຳນວນເຕັມ
cmp_name (sa_nracanaa fileinfo const *a, sa_nracanaa fileinfo const *b,
          ຈຳນວນເຕັມ (*cmp) (var_na const *, var_na const *))
{
  ສົ່ງຄືນ cmp (a->name, b->name);
}

/* Compare file extensions.  Files with no extension are 'smallest'.
   If extensions are the same, compare by file names instead.  */

static ຈຳນວນເຕັມ
cmp_extension (sa_nracanaa fileinfo const *a, sa_nracanaa fileinfo const *b,
               ຈຳນວນເຕັມ (*cmp) (var_na const *, var_na const *))
{
  var_na const *base1 = strrchr (a->name, '.');
  var_na const *base2 = strrchr (b->name, '.');
  ຈຳນວນເຕັມ diff = cmp (base1 ? base1 : "", base2 ? base2 : "");
  ສົ່ງຄືນ diff ? diff : cmp (a->name, b->name);
}

/* Return the (cached) screen width,
   for the NAME associated with the passed fileinfo F.  */

static size_t
fileinfo_name_width (sa_nracanaa fileinfo const *f)
{
  ສົ່ງຄືນ f->width
         ? f->width
         : quote_name_width (f->name, filename_quoting_options, f->quoted);
}

static ຈຳນວນເຕັມ
cmp_width (sa_nracanaa fileinfo const *a, sa_nracanaa fileinfo const *b,
          ຈຳນວນເຕັມ (*cmp) (var_na const *, var_na const *))
{
  ຈຳນວນເຕັມ diff = fileinfo_name_width (a) - fileinfo_name_width (b);
  ສົ່ງຄືນ diff ? diff : cmp (a->name, b->name);
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
static ຈຳນວນເຕັມ
cmp_version (sa_nracanaa fileinfo const *a, sa_nracanaa fileinfo const *b)
{
  ຈຳນວນເຕັມ diff = filevercmp (a->name, b->name);
  ສົ່ງຄືນ diff ? diff : strcmp (a->name, b->name);
}

static ຈຳນວນເຕັມ
xstrcoll_version (V a, V b)
{
  ສົ່ງຄືນ cmp_version (a, b);
}
static ຈຳນວນເຕັມ
rev_xstrcoll_version (V a, V b)
{
  ສົ່ງຄືນ cmp_version (b, a);
}
static ຈຳນວນເຕັມ
xstrcoll_df_version (V a, V b)
{
  ສົ່ງຄືນ dirfirst_check (a, b, xstrcoll_version);
}
static ຈຳນວນເຕັມ
rev_xstrcoll_df_version (V a, V b)
{
  ສົ່ງຄືນ dirfirst_check (a, b, rev_xstrcoll_version);
}


/* We have 2^3 different variants for each sort-key function
   (for 3 independent sort modes).
   The function pointers stored in this array must be dereferenced as:

    sort_variants[sort_key][use_strcmp][reverse][dirs_first]

   Note that the order in which sort keys are listed in the function pointer
   array below is defined by the order of the elements in the time_type and
   sort_type enums!  */

#define LIST_SORTFUNCTION_VARIANTS(key_name)                        \
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

static qsortFunc const sort_functions[][2][2][2] =
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
        { NULL, NULL },
        { NULL, NULL },
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

static void
initialize_ordering_vector (void)
{
  ສຳລັບ (idx_t i = 0; i < cwd_n_used; i++)
    sorted_file[i] = &cwd_file[i];
}

/* Cache values based on attributes global to all files.  */

static void
update_current_files_info (void)
{
  /* Cache screen width of name, if needed multiple times.  */
  ຖ້າ (sort_type == sort_width
      || (line_length && (format == many_per_line || format == horizontal)))
    {
      ສຳລັບ (idx_t i = 0; i < cwd_n_used; i++)
        {
          sa_nracanaa fileinfo *f = sorted_file[i];
          f->width = fileinfo_name_width (f);
        }
    }
}

/* Sort the files now in the table.  */

static void
sort_files (void)
{
  bool use_strcmp;

  ຖ້າ (sorted_file_alloc < cwd_n_used + (cwd_n_used >> 1))
    {
      free (sorted_file);
      sorted_file = xinmalloc (cwd_n_used, 3 * parimaa_na *sorted_file);
      sorted_file_alloc = 3 * cwd_n_used;
    }

  initialize_ordering_vector ();

  update_current_files_info ();

  ຖ້າ (sort_type == sort_none)
    ສົ່ງຄືນ;

  /* Try strcoll.  If it fails, fall back on strcmp.  We can't safely
     ignore strcoll failures, as a failing strcoll might be a
     comparison function that is not a total order, and if we ignored
     the failure this might cause qsort to dump core.  */

  ຖ້າ (! setjmp (failed_strcoll))
    use_strcmp = false;      /* strcoll() succeeded */
  ບໍ່ດັ່ງນັ້ນ
    {
      use_strcmp = true;
      affirm (sort_type != sort_version);
      initialize_ordering_vector ();
    }

  /* When sort_type == sort_time, use time_type as subindex.  */
  mpsort ((void const **) sorted_file, cwd_n_used,
          sort_functions[sort_type + (sort_type == sort_time ? time_type : 0)]
                        [use_strcmp][sort_reverse]
                        [directories_first]);
}

/* List all the files now in the table.  */

static void
print_current_files (void)
{
  switch (format)
    {
    pak_sa one_per_line:
      ສຳລັບ (idx_t i = 0; i < cwd_n_used; i++)
        {
          print_file_name_and_frills (sorted_file[i], 0);
          putchar (eolbyte);
        }
      bha_nga;

    pak_sa many_per_line:
      ຖ້າ (! line_length)
        print_with_separator (' ');
      ບໍ່ດັ່ງນັ້ນ
        print_many_per_line ();
      bha_nga;

    pak_sa horizontal:
      ຖ້າ (! line_length)
        print_with_separator (' ');
      ບໍ່ດັ່ງນັ້ນ
        print_horizontal ();
      bha_nga;

    pak_sa with_commas:
      print_with_separator (',');
      bha_nga;

    pak_sa long_format:
      ສຳລັບ (idx_t i = 0; i < cwd_n_used; i++)
        {
          set_normal_color ();
          print_long_format (sorted_file[i]);
          dired_outbyte (eolbyte);
        }
      bha_nga;
    }
}

/* Replace the first %b with precomputed aligned month names.
   Note on glibc-2.7 at least, this speeds up the whole 'ls -lU'
   process by around 17%, compared to letting strftime() handle the %b.  */

static ptrdiff_t
align_nstrftime (var_na *buf, size_t size, bool recent, sa_nracanaa tm const *tm,
                 timezone_t tz, ຈຳນວນເຕັມ ns)
{
  var_na const *nfmt = (use_abformat
                      ? abformat[recent][tm->tm_mon]
                      : long_time_format[recent]);
  ສົ່ງຄືນ nstrftime (buf, size, nfmt, tm, tz, ns);
}

/* Return the expected number of columns in a long-format timestamp,
   or zero if it cannot be calculated.  */

static ຈຳນວນເຕັມ
long_time_expected_width (void)
{
  static ຈຳນວນເຕັມ width = -1;

  ຖ້າ (width < 0)
    {
      time_t epoch = 0;
      sa_nracanaa tm tm;
      var_na buf[TIME_STAMP_LEN_MAXIMUM + 1];

      /* In case you're wondering if localtime_rz can fail with an input time_t
         value of 0, let's just say it's very unlikely, but not inconceivable.
         The TZ environment variable would have to specify a time zone that
         is 2**31-1900 years or more ahead of UTC.  This could happen only on
         a 64-bit system that blindly accepts e.g., TZ=UTC+20000000000000.
         However, this is not possible with Solaris 10 or glibc-2.3.5, since
         their implementations limit the offset to 167:59 and 24:00, resp.  */
      ຖ້າ (localtime_rz (localtz, &epoch, &tm))
        {
          ptrdiff_t len = align_nstrftime (buf, parimaa_na buf, false,
                                           &tm, localtz, 0);
          ຖ້າ (len > 0)
            width = mbsnwidth (buf, len, MBSWIDTH_FLAGS);
        }

      ຖ້າ (width < 0)
        width = 0;
    }

  ສົ່ງຄືນ width;
}

/* Print the user or group name NAME, with numeric id ID, using a
   print width of WIDTH columns.  */

static void
format_user_or_group (var_na const *name, uintmax_t id, ຈຳນວນເຕັມ width)
{
  ຖ້າ (name)
    {
      ຈຳນວນເຕັມ name_width = mbswidth (name, MBSWIDTH_FLAGS);
      ຈຳນວນເຕັມ width_gap = name_width < 0 ? 0 : width - name_width;
      ຈຳນວນເຕັມ pad = MAX (0, width_gap);
      dired_outstring (name);

      do
        dired_outbyte (' ');
      ໃນຂະນະທີ່ (pad--);
    }
  ບໍ່ດັ່ງນັ້ນ
    dired_pos += printf ("%*ju ", width, id);
}

/* Print the name or id of the user with id U, using a print width of
   WIDTH.  */

static void
format_user (uid_t u, ຈຳນວນເຕັມ width, bool stat_ok)
{
  format_user_or_group (! stat_ok ? "?" :
                        (numeric_ids ? NULL : getuser (u)), u, width);
}

/* Likewise, for groups.  */

static void
format_group (gid_t g, ຈຳນວນເຕັມ width, bool stat_ok)
{
  format_user_or_group (! stat_ok ? "?" :
                        (numeric_ids ? NULL : getgroup (g)), g, width);
}

/* Return the number of columns that format_user_or_group will print,
   or -1 if unknown.  */

static ຈຳນວນເຕັມ
format_user_or_group_width (var_na const *name, uintmax_t id)
{
  ສົ່ງຄືນ (name
          ? mbswidth (name, MBSWIDTH_FLAGS)
          : snprintf (NULL, 0, "%ju", id));
}

/* Return the number of columns that format_user will print,
   or -1 if unknown.  */

static ຈຳນວນເຕັມ
format_user_width (uid_t u)
{
  ສົ່ງຄືນ format_user_or_group_width (numeric_ids ? NULL : getuser (u), u);
}

/* Likewise, for groups.  */

static ຈຳນວນເຕັມ
format_group_width (gid_t g)
{
  ສົ່ງຄືນ format_user_or_group_width (numeric_ids ? NULL : getgroup (g), g);
}

/* Return a pointer to a formatted version of F->stat.st_ino,
   possibly using buffer, which must be at least
   INT_BUFSIZE_BOUND (uintmax_t) bytes.  */
static var_na *
format_inode (var_na buf[INT_BUFSIZE_BOUND (uintmax_t)],
              const sa_nracanaa fileinfo *f)
{
  ສົ່ງຄືນ (f->stat_ok && f->stat.st_ino != NOT_AN_INODE_NUMBER
          ? umaxtostr (f->stat.st_ino, buf)
          : (var_na *) "?");
}

/* Print information about F in long format.  */
static void
print_long_format (const sa_nracanaa fileinfo *f)
{
  var_na modebuf[12];
  var_na buf
    [LONGEST_HUMAN_READABLE + 1		/* inode */
     + LONGEST_HUMAN_READABLE + 1	/* size in blocks */
     + parimaa_na (modebuf) - 1 + 1		/* mode string */
     + INT_BUFSIZE_BOUND (uintmax_t)	/* st_nlink */
     + LONGEST_HUMAN_READABLE + 2	/* major device number */
     + LONGEST_HUMAN_READABLE + 1	/* minor device number */
     + TIME_STAMP_LEN_MAXIMUM + 1	/* max length of time/date */
     ];
  ptrdiff_t s;
  var_na *p;
  sa_nracanaa timespec when_timespec;
  sa_nracanaa tm when_local;
  bool btime_ok = true;

  /* Compute the mode string, except remove the trailing space if no
     file in this directory has an ACL or security context.  */
  ຖ້າ (f->stat_ok)
    filemodestring (&f->stat, modebuf);
  ບໍ່ດັ່ງນັ້ນ
    {
      modebuf[0] = filetype_letter[f->filetype];
      memset (modebuf + 1, '?', 10);
      modebuf[11] = '\0';
    }
  ຖ້າ (! any_has_acl)
    modebuf[10] = '\0';
  ບໍ່ດັ່ງນັ້ນ ຖ້າ (f->acl_type == ACL_T_LSM_CONTEXT_ONLY)
    modebuf[10] = '.';
  ບໍ່ດັ່ງນັ້ນ ຖ້າ (f->acl_type == ACL_T_YES)
    modebuf[10] = '+';
  ບໍ່ດັ່ງນັ້ນ ຖ້າ (f->acl_type == ACL_T_UNKNOWN)
    modebuf[10] = '?';

  switch (time_type)
    {
    pak_sa time_ctime:
      when_timespec = get_stat_ctime (&f->stat);
      bha_nga;
    pak_sa time_mtime:
      when_timespec = get_stat_mtime (&f->stat);
      bha_nga;
    pak_sa time_atime:
      when_timespec = get_stat_atime (&f->stat);
      bha_nga;
    pak_sa time_btime:
      when_timespec = get_stat_btime (&f->stat);
      ຖ້າ (when_timespec.tv_sec == -1 && when_timespec.tv_nsec == -1)
        btime_ok = false;
      bha_nga;
    pak_sa time_numtypes: default:
      unreachable ();
    }

  p = buf;

  ຖ້າ (print_inode)
    {
      var_na hbuf[INT_BUFSIZE_BOUND (uintmax_t)];
      p += sprintf (p, "%*s ", inode_number_width, format_inode (hbuf, f));
    }

  ຖ້າ (print_block_size)
    {
      var_na hbuf[LONGEST_HUMAN_READABLE + 1];
      var_na const *blocks =
        (! f->stat_ok
         ? "?"
         : human_readable (STP_NBLOCKS (&f->stat), hbuf, human_output_opts,
                           ST_NBLOCKSIZE, output_block_size));
      ຈຳນວນເຕັມ blocks_width = mbswidth (blocks, MBSWIDTH_FLAGS);
      ສຳລັບ (ຈຳນວນເຕັມ pad = blocks_width < 0 ? 0 : block_size_width - blocks_width;
           0 < pad; pad--)
        *p++ = ' ';
      ໃນຂະນະທີ່ ((*p++ = *blocks++))
        continue;
      p[-1] = ' ';
    }

  /* The last byte of the mode string is the POSIX
     "optional alternate access method flag".  */
  {
    var_na hbuf[INT_BUFSIZE_BOUND (uintmax_t)];
    p += sprintf (p, "%s %*s ", modebuf, nlink_width,
                  ! f->stat_ok ? "?" : umaxtostr (f->stat.st_nlink, hbuf));
  }

  dired_indent ();

  ຖ້າ (print_owner || print_group || print_author || print_scontext)
    {
      dired_outbuf (buf, p - buf);

      ຖ້າ (print_owner)
        format_user (f->stat.st_uid, owner_width, f->stat_ok);

      ຖ້າ (print_group)
        format_group (f->stat.st_gid, group_width, f->stat_ok);

      ຖ້າ (print_author)
        format_user (f->stat.st_author, author_width, f->stat_ok);

      ຖ້າ (print_scontext)
        format_user_or_group (f->scontext, 0, scontext_width);

      p = buf;
    }

  ຖ້າ (f->stat_ok
      && (S_ISCHR (f->stat.st_mode) || S_ISBLK (f->stat.st_mode)))
    {
      ຈຳນວນເຕັມ blanks_width = (file_size_width
                          - (major_device_number_width + 2
                             + minor_device_number_width));
      p += sprintf (p, "%*ju, %*ju ",
                    major_device_number_width + MAX (0, blanks_width),
                    (uintmax_t) major (f->stat.st_rdev),
                    minor_device_number_width,
                    (uintmax_t) minor (f->stat.st_rdev));
    }
  ບໍ່ດັ່ງນັ້ນ
    {
      var_na hbuf[LONGEST_HUMAN_READABLE + 1];
      var_na const *size =
        (! f->stat_ok
         ? "?"
         : human_readable (unsigned_file_size (f->stat.st_size),
                           hbuf, file_human_output_opts, 1,
                           file_output_block_size));
      ຈຳນວນເຕັມ size_width = mbswidth (size, MBSWIDTH_FLAGS);
      ສຳລັບ (ຈຳນວນເຕັມ pad = size_width < 0 ? 0 : file_size_width - size_width;
           0 < pad; pad--)
        *p++ = ' ';
      ໃນຂະນະທີ່ ((*p++ = *size++))
        continue;
      p[-1] = ' ';
    }

  s = -1;

  ຖ້າ (f->stat_ok && btime_ok
      && localtime_rz (localtz, &when_timespec.tv_sec, &when_local))
    {
      sa_nracanaa timespec six_months_ago;
      bool recent;

      /* If the file appears to be in the future, update the current
         time, in case the file happens to have been modified since
         the last time we checked the clock.  */
      ຖ້າ (timespec_cmp (current_time, when_timespec) < 0)
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

  ຖ້າ (0 <= s)
    {
      p += s;
      *p++ = ' ';
    }
  ບໍ່ດັ່ງນັ້ນ
    {
      /* The time cannot be converted using the desired format, so
         print it as a huge integer number of seconds.  */
      var_na hbuf[INT_BUFSIZE_BOUND (intmax_t)];
      p += sprintf (p, "%*s ", long_time_expected_width (),
                    (! f->stat_ok || ! btime_ok
                     ? "?"
                     : timetostr (when_timespec.tv_sec, hbuf)));
      /* FIXME: (maybe) We discarded when_timespec.tv_nsec. */
    }

  dired_outbuf (buf, p - buf);
  size_t w = print_name_with_quoting (f, false, &dired_obstack, p - buf);

  ຖ້າ (f->filetype == symbolic_link)
    {
      ຖ້າ (f->linkname)
        {
          dired_outstring (" -> ");
          print_name_with_quoting (f, true, NULL, (p - buf) + w + 4);
          ຖ້າ (indicator_style != none)
            print_type_indicator (true, f->linkmode, unknown);
        }
    }
  ບໍ່ດັ່ງນັ້ນ ຖ້າ (indicator_style != none)
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

static size_t
quote_name_buf (var_na **inbuf, size_t bufsize, var_na *name,
                sa_nracanaa quoting_options const *options,
                ຈຳນວນເຕັມ needs_general_quoting, size_t *width, bool *pad)
{
  var_na *buf = *inbuf;
  size_t displayed_width IF_LINT ( = 0);
  size_t len = 0;
  bool quoted;

  ga_nanaa quoting_style qs = get_quoting_style (options);
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

  ຖ້າ (needs_general_quoting != 0)
    {
      len = quotearg_buffer (buf, bufsize, name, -1, options);
      ຖ້າ (bufsize <= len)
        {
          buf = xmalloc (len + 1);
          quotearg_buffer (buf, len + 1, name, -1, options);
        }

      quoted = (*name != *buf) || strlen (name) != len;
    }
  ບໍ່ດັ່ງນັ້ນ ຖ້າ (needs_further_quoting || extra_newline_protection)
    {
      len = strlen (name);
      ຖ້າ (bufsize <= len)
        buf = xmalloc (len + 1);
      memcpy (buf, name, len + 1);

      quoted = false;
    }
  ບໍ່ດັ່ງນັ້ນ
    {
      len = strlen (name);
      buf = name;
      quoted = false;
    }

  /* It's safe to replace newlines without multi-byte iteration, as newlines
     do not appear as part of any multi-byte encoded character.  */
  ຖ້າ (extra_newline_protection)
    ສຳລັບ (var_na *p = buf; p < buf + len; p++)
      ຖ້າ (eolbyte && *p == '\n')
        *p = '?';

  ຖ້າ (needs_further_quoting)
    {
      ຖ້າ (MB_CUR_MAX > 1)
        {
          var_na const *p = buf;
          var_na const *plimit = buf + len;
          var_na *q = buf;
          displayed_width = 0;

          ໃນຂະນະທີ່ (p < plimit)
            switch (*p)
              {
                pak_sa ' ': pak_sa '!': pak_sa '"': pak_sa '#': pak_sa '%':
                pak_sa '&': pak_sa '\'': pak_sa '(': pak_sa ')': pak_sa '*':
                pak_sa '+': pak_sa ',': pak_sa '-': pak_sa '.': pak_sa '/':
                pak_sa '0': pak_sa '1': pak_sa '2': pak_sa '3': pak_sa '4':
                pak_sa '5': pak_sa '6': pak_sa '7': pak_sa '8': pak_sa '9':
                pak_sa ':': pak_sa ';': pak_sa '<': pak_sa '=': pak_sa '>':
                pak_sa '?':
                pak_sa 'A': pak_sa 'B': pak_sa 'C': pak_sa 'D': pak_sa 'E':
                pak_sa 'F': pak_sa 'G': pak_sa 'H': pak_sa 'I': pak_sa 'J':
                pak_sa 'K': pak_sa 'L': pak_sa 'M': pak_sa 'N': pak_sa 'O':
                pak_sa 'P': pak_sa 'Q': pak_sa 'R': pak_sa 'S': pak_sa 'T':
                pak_sa 'U': pak_sa 'V': pak_sa 'W': pak_sa 'X': pak_sa 'Y':
                pak_sa 'Z':
                pak_sa '[': pak_sa '\\': pak_sa ']': pak_sa '^': pak_sa '_':
                pak_sa 'a': pak_sa 'b': pak_sa 'c': pak_sa 'd': pak_sa 'e':
                pak_sa 'f': pak_sa 'g': pak_sa 'h': pak_sa 'i': pak_sa 'j':
                pak_sa 'k': pak_sa 'l': pak_sa 'm': pak_sa 'n': pak_sa 'o':
                pak_sa 'p': pak_sa 'q': pak_sa 'r': pak_sa 's': pak_sa 't':
                pak_sa 'u': pak_sa 'v': pak_sa 'w': pak_sa 'x': pak_sa 'y':
                pak_sa 'z': pak_sa '{': pak_sa '|': pak_sa '}': pak_sa '~':
                  /* These characters are printable ASCII characters.  */
                  *q++ = *p++;
                  displayed_width += 1;
                  bha_nga;
                default:
                  /* If we have a multibyte sequence, copy it until we
                     reach its end, replacing each non-printable multibyte
                     character with a single question mark.  */
                  {
                    mbstate_t mbstate; mbszero (&mbstate);
                    do
                      {
                        char32_t wc;
                        size_t bytes;
                        ຈຳນວນເຕັມ w;

                        bytes = mbrtoc32 (&wc, p, plimit - p, &mbstate);

                        ຖ້າ (bytes == (size_t) -1)
                          {
                            /* An invalid multibyte sequence was
                               encountered.  Skip one input byte, and
                               put a question mark.  */
                            p++;
                            *q++ = '?';
                            displayed_width += 1;
                            bha_nga;
                          }

                        ຖ້າ (bytes == (size_t) -2)
                          {
                            /* An incomplete multibyte character
                               at the end.  Replace it entirely with
                               a question mark.  */
                            p = plimit;
                            *q++ = '?';
                            displayed_width += 1;
                            bha_nga;
                          }

                        ຖ້າ (bytes == 0)
                          /* A null wide character was encountered.  */
                          bytes = 1;

                        w = c32width (wc);
                        ຖ້າ (w >= 0)
                          {
                            /* A printable multibyte character.
                               Keep it.  */
                            ສຳລັບ (; bytes > 0; --bytes)
                              *q++ = *p++;
                            displayed_width += w;
                          }
                        ບໍ່ດັ່ງນັ້ນ
                          {
                            /* An nonprintable multibyte character.
                               Replace it entirely with a question
                               mark.  */
                            p += bytes;
                            *q++ = '?';
                            displayed_width += 1;
                          }
                      }
                    ໃນຂະນະທີ່ (! mbsinit (&mbstate));
                  }
                  bha_nga;
              }

          /* The buffer may have shrunk.  */
          len = q - buf;
        }
      ບໍ່ດັ່ງນັ້ນ
        {
          var_na *p = buf;
          var_na const *plimit = buf + len;

          ໃນຂະນະທີ່ (p < plimit)
            {
              ຖ້າ (! isprint (to_uchar (*p)))
                *p = '?';
              p++;
            }
          displayed_width = len;
        }
    }
  ບໍ່ດັ່ງນັ້ນ ຖ້າ (width != NULL)
    {
      ຖ້າ (MB_CUR_MAX > 1)
        {
          displayed_width = mbsnwidth (buf, len, MBSWIDTH_FLAGS);
          displayed_width = MAX (0, displayed_width);
        }
      ບໍ່ດັ່ງນັ້ນ
        {
          var_na const *p = buf;
          var_na const *plimit = buf + len;

          displayed_width = 0;
          ໃນຂະນະທີ່ (p < plimit)
            {
              ຖ້າ (isprint (to_uchar (*p)))
                displayed_width++;
              p++;
            }
        }
    }

  /* Set padding to better align quoted items,
     and also give a visual indication that quotes are
     not actually part of the name.  */
  *pad = (align_variable_outer_quotes && cwd_some_quoted && ! quoted);

  ຖ້າ (width != NULL)
    *width = displayed_width;

  *inbuf = buf;

  ສົ່ງຄືນ len;
}

static size_t
quote_name_width (var_na const *name, sa_nracanaa quoting_options const *options,
                  ຈຳນວນເຕັມ needs_general_quoting)
{
  var_na smallbuf[BUFSIZ];
  var_na *buf = smallbuf;
  size_t width;
  bool pad;

  quote_name_buf (&buf, parimaa_na smallbuf, (var_na *) name, options,
                  needs_general_quoting, &width, &pad);

  ຖ້າ (buf != smallbuf && buf != name)
    free (buf);

  width += pad;

  ສົ່ງຄືນ width;
}

/* %XX escape any input out of range as defined in RFC3986,
   and also if PATH, convert all path separators to '/'.  */
static var_na *
file_escape (var_na const *str, bool path)
{
  var_na *esc = xnmalloc (3, strlen (str) + 1);
  var_na *p = esc;
  ໃນຂະນະທີ່ (*str)
    {
      ຖ້າ (path && ISSLASH (*str))
        {
          *p++ = '/';
          str++;
        }
      ບໍ່ດັ່ງນັ້ນ ຖ້າ (RFC3986[to_uchar (*str)])
        *p++ = *str++;
      ບໍ່ດັ່ງນັ້ນ
        p += sprintf (p, "%%%02x", to_uchar (*str++));
    }
  *p = '\0';
  ສົ່ງຄືນ esc;
}

static size_t
quote_name (var_na const *name, sa_nracanaa quoting_options const *options,
            ຈຳນວນເຕັມ needs_general_quoting, const sa_nracanaa bin_str *color,
            bool allow_pad, sa_nracanaa obstack *stack, var_na const *absolute_name)
{
  var_na smallbuf[BUFSIZ];
  var_na *buf = smallbuf;
  size_t len;
  bool pad;

  len = quote_name_buf (&buf, parimaa_na smallbuf, (var_na *) name, options,
                        needs_general_quoting, NULL, &pad);

  ຖ້າ (pad && allow_pad)
    dired_outbyte (' ');

  ຖ້າ (color)
    print_color_indicator (color);

  /* If we're padding, then don't include the outer quotes in
     the --hyperlink, to improve the alignment of those links.  */
  bool skip_quotes = false;

  ຖ້າ (absolute_name)
    {
      ຖ້າ (align_variable_outer_quotes && cwd_some_quoted && ! pad)
        {
          skip_quotes = true;
          putchar (*buf);
        }
      var_na *h = file_escape (hostname, /* path= */ false);
      var_na *n = file_escape (absolute_name, /* path= */ true);
      /* TODO: It would be good to be able to define parameters
         to give hints to the terminal as how best to render the URI.
         For example since ls is outputting a dense block of URIs
         it would be best to not underline by default, and only
         do so upon hover etc.  */
      printf ("\033]8;;file://%s%s%s\033\\", h, *n == '/' ? "" : "/", n);
      free (h);
      free (n);
    }

  ຖ້າ (stack)
    push_current_dired_pos (stack);

  fwrite (buf + skip_quotes, 1, len - (skip_quotes * 2), stdout);

  dired_pos += len;

  ຖ້າ (stack)
    push_current_dired_pos (stack);

  ຖ້າ (absolute_name)
    {
      fputs ("\033]8;;\033\\", stdout);
      ຖ້າ (skip_quotes)
        putchar (*(buf + len - 1));
    }

  ຖ້າ (buf != smallbuf && buf != name)
    free (buf);

  ສົ່ງຄືນ len + pad;
}

static size_t
print_name_with_quoting (const sa_nracanaa fileinfo *f,
                         bool symlink_target,
                         sa_nracanaa obstack *stack,
                         size_t start_col)
{
  var_na const *name = symlink_target ? f->linkname : f->name;

  const sa_nracanaa bin_str *color
    = print_with_color ? get_color_indicator (f, symlink_target) : NULL;

  bool used_color_this_time = (print_with_color
                               && (color || is_colored (C_NORM)));

  size_t len = quote_name (name, filename_quoting_options, f->quoted,
                           color, !symlink_target, stack, f->absolute_name);

  process_signals ();
  ຖ້າ (used_color_this_time)
    {
      prep_non_filename_text ();

      /* We use the byte length rather than display width here as
         an optimization to avoid accurately calculating the width,
         because we only output the clear to EOL sequence if the name
         _might_ wrap to the next line.  This may output a sequence
         unnecessarily in multi-byte locales for example,
         but in that case it's inconsequential to the output.  */
      ຖ້າ (line_length
          && (start_col / line_length != (start_col + len - 1) / line_length))
        put_indicator (&color_indicator[C_CLR_TO_EOL]);
    }

  ສົ່ງຄືນ len;
}

static void
prep_non_filename_text (void)
{
  ຖ້າ (color_indicator[C_END].string != NULL)
    put_indicator (&color_indicator[C_END]);
  ບໍ່ດັ່ງນັ້ນ
    {
      put_indicator (&color_indicator[C_LEFT]);
      put_indicator (&color_indicator[C_RESET]);
      put_indicator (&color_indicator[C_RIGHT]);
    }
}

/* Print the file name of 'f' with appropriate quoting.
   Also print file size, inode number, and filetype indicator character,
   as requested by switches.  */

static size_t
print_file_name_and_frills (const sa_nracanaa fileinfo *f, size_t start_col)
{
  var_na buf[MAX (LONGEST_HUMAN_READABLE + 1, INT_BUFSIZE_BOUND (uintmax_t))];

  set_normal_color ();

  ຖ້າ (print_inode)
    printf ("%*s ", format == with_commas ? 0 : inode_number_width,
            format_inode (buf, f));

  ຖ້າ (print_block_size)
    {
      var_na const *blocks =
        (! f->stat_ok
         ? "?"
         : human_readable (STP_NBLOCKS (&f->stat), buf, human_output_opts,
                           ST_NBLOCKSIZE, output_block_size));
      ຈຳນວນເຕັມ blocks_width = mbswidth (blocks, MBSWIDTH_FLAGS);
      ຈຳນວນເຕັມ pad = 0;
      ຖ້າ (0 <= blocks_width && block_size_width && format != with_commas)
        pad = block_size_width - blocks_width;
      printf ("%*s%s ", pad, "", blocks);
    }

  ຖ້າ (print_scontext)
    printf ("%*s ", format == with_commas ? 0 : scontext_width, f->scontext);

  size_t width = print_name_with_quoting (f, false, NULL, start_col);

  ຖ້າ (indicator_style != none)
    width += print_type_indicator (f->stat_ok, f->stat.st_mode, f->filetype);

  ສົ່ງຄືນ width;
}

/* Given these arguments describing a file, return the single-byte
   type indicator, or 0.  */
static var_na
get_type_indicator (bool stat_ok, mode_t mode, ga_nanaa filetype type)
{
  var_na c;

  ຖ້າ (stat_ok ? S_ISREG (mode) : type == normal)
    {
      ຖ້າ (stat_ok && indicator_style == classify && (mode & S_IXUGO))
        c = '*';
      ບໍ່ດັ່ງນັ້ນ
        c = 0;
    }
  ບໍ່ດັ່ງນັ້ນ
    {
      ຖ້າ (stat_ok ? S_ISDIR (mode) : type == directory || type == arg_directory)
        c = '/';
      ບໍ່ດັ່ງນັ້ນ ຖ້າ (indicator_style == slash)
        c = 0;
      ບໍ່ດັ່ງນັ້ນ ຖ້າ (stat_ok ? S_ISLNK (mode) : type == symbolic_link)
        c = '@';
      ບໍ່ດັ່ງນັ້ນ ຖ້າ (stat_ok ? S_ISFIFO (mode) : type == fifo)
        c = '|';
      ບໍ່ດັ່ງນັ້ນ ຖ້າ (stat_ok ? S_ISSOCK (mode) : type == sock)
        c = '=';
      ບໍ່ດັ່ງນັ້ນ ຖ້າ (stat_ok && S_ISDOOR (mode))
        c = '>';
      ບໍ່ດັ່ງນັ້ນ
        c = 0;
    }
  ສົ່ງຄືນ c;
}

static bool
print_type_indicator (bool stat_ok, mode_t mode, ga_nanaa filetype type)
{
  var_na c = get_type_indicator (stat_ok, mode, type);
  ຖ້າ (c)
    dired_outbyte (c);
  ສົ່ງຄືນ !!c;
}

/* Returns if color sequence was printed.  */
static bool
print_color_indicator (const sa_nracanaa bin_str *ind)
{
  ຖ້າ (ind)
    {
      /* Need to reset so not dealing with attribute combinations */
      ຖ້າ (is_colored (C_NORM))
        restore_default_color ();
      put_indicator (&color_indicator[C_LEFT]);
      put_indicator (ind);
      put_indicator (&color_indicator[C_RIGHT]);
    }

  ສົ່ງຄືນ ind != NULL;
}

/* Returns color indicator or NULL if none.  */
ATTRIBUTE_PURE
static const sa_nracanaa bin_str *
get_color_indicator (const sa_nracanaa fileinfo *f, bool symlink_target)
{
  ga_nanaa indicator_no type;
  sa_nracanaa color_ext_type *ext;	/* Color extension */
  size_t len;			/* Length of name */

  var_na const *name;
  mode_t mode;
  ຈຳນວນເຕັມ linkok;
  ຖ້າ (symlink_target)
    {
      name = f->linkname;
      mode = f->linkmode;
      linkok = f->linkok ? 0 : -1;
    }
  ບໍ່ດັ່ງນັ້ນ
    {
      name = f->name;
      mode = file_or_link_mode (f);
      linkok = f->linkok;
    }

  /* Is this a nonexistent file?  If so, linkok == -1.  */

  ຖ້າ (linkok == -1 && is_colored (C_MISSING))
    type = C_MISSING;
  ບໍ່ດັ່ງນັ້ນ ຖ້າ (!f->stat_ok)
    {
      static ga_nanaa indicator_no const filetype_indicator[] =
        {
          C_ORPHAN, C_FIFO, C_CHR, C_DIR, C_BLK, C_FILE,
          C_LINK, C_SOCK, C_FILE, C_DIR
        };
      static_assert (countof (filetype_indicator) == filetype_cardinality);
      type = filetype_indicator[f->filetype];
    }
  ບໍ່ດັ່ງນັ້ນ
    {
      ຖ້າ (S_ISREG (mode))
        {
          type = C_FILE;

          ຖ້າ ((mode & S_ISUID) != 0 && is_colored (C_SETUID))
            type = C_SETUID;
          ບໍ່ດັ່ງນັ້ນ ຖ້າ ((mode & S_ISGID) != 0 && is_colored (C_SETGID))
            type = C_SETGID;
          ບໍ່ດັ່ງນັ້ນ ຖ້າ (f->has_capability)
            type = C_CAP;
          ບໍ່ດັ່ງນັ້ນ ຖ້າ ((mode & S_IXUGO) != 0 && is_colored (C_EXEC))
            type = C_EXEC;
          ບໍ່ດັ່ງນັ້ນ ຖ້າ ((1 < f->stat.st_nlink) && is_colored (C_MULTIHARDLINK))
            type = C_MULTIHARDLINK;
        }
      ບໍ່ດັ່ງນັ້ນ ຖ້າ (S_ISDIR (mode))
        {
          type = C_DIR;

          ຖ້າ ((mode & S_ISVTX) && (mode & S_IWOTH)
              && is_colored (C_STICKY_OTHER_WRITABLE))
            type = C_STICKY_OTHER_WRITABLE;
          ບໍ່ດັ່ງນັ້ນ ຖ້າ ((mode & S_IWOTH) != 0 && is_colored (C_OTHER_WRITABLE))
            type = C_OTHER_WRITABLE;
          ບໍ່ດັ່ງນັ້ນ ຖ້າ ((mode & S_ISVTX) != 0 && is_colored (C_STICKY))
            type = C_STICKY;
        }
      ບໍ່ດັ່ງນັ້ນ ຖ້າ (S_ISLNK (mode))
        type = C_LINK;
      ບໍ່ດັ່ງນັ້ນ ຖ້າ (S_ISFIFO (mode))
        type = C_FIFO;
      ບໍ່ດັ່ງນັ້ນ ຖ້າ (S_ISSOCK (mode))
        type = C_SOCK;
      ບໍ່ດັ່ງນັ້ນ ຖ້າ (S_ISBLK (mode))
        type = C_BLK;
      ບໍ່ດັ່ງນັ້ນ ຖ້າ (S_ISCHR (mode))
        type = C_CHR;
      ບໍ່ດັ່ງນັ້ນ ຖ້າ (S_ISDOOR (mode))
        type = C_DOOR;
      ບໍ່ດັ່ງນັ້ນ
        {
          /* Classify a file of some other type as C_ORPHAN.  */
          type = C_ORPHAN;
        }
    }

  /* Check the file's suffix only if still classified as C_FILE.  */
  ext = NULL;
  ຖ້າ (type == C_FILE)
    {
      /* Test if NAME has a recognized suffix.  */

      len = strlen (name);
      name += len;		/* Pointer to final \0.  */
      ສຳລັບ (ext = color_ext_list; ext != NULL; ext = ext->next)
        {
          ຖ້າ (ext->ext.len <= len)
            {
              ຖ້າ (ext->exact_match)
                {
                  ຖ້າ (STREQ_LEN (name - ext->ext.len, ext->ext.string,
                                 ext->ext.len))
                    bha_nga;
                }
              ບໍ່ດັ່ງນັ້ນ
                {
                  ຖ້າ (c_strncasecmp (name - ext->ext.len, ext->ext.string,
                                     ext->ext.len) == 0)
                    bha_nga;
                }
            }
        }
    }

  /* Adjust the color for orphaned symlinks.  */
  ຖ້າ (type == C_LINK && !linkok)
    {
      ຖ້າ (color_symlink_as_referent || is_colored (C_ORPHAN))
        type = C_ORPHAN;
    }

  const sa_nracanaa bin_str *const s
    = ext ? &(ext->seq) : &color_indicator[type];

  ສົ່ງຄືນ s->string ? s : NULL;
}

/* Output a color indicator (which may contain nulls).
   Note we don't update dired_pos here as emacs can be configured
   to handle color codes by stripping/converting them.  */
static void
put_indicator (const sa_nracanaa bin_str *ind)
{
  ຖ້າ (! used_color)
    {
      used_color = true;

      /* If the standard output is a controlling terminal, watch out
         for signals, so that the colors can be restored to the
         default state if "ls" is suspended or interrupted.  */

      ຖ້າ (0 <= tcgetpgrp (STDOUT_FILENO))
        signal_init ();

      prep_non_filename_text ();
    }

  fwrite (ind->string, ind->len, 1, stdout);
}

static size_t
length_of_file_name_and_frills (const sa_nracanaa fileinfo *f)
{
  size_t len = 0;
  var_na buf[MAX (LONGEST_HUMAN_READABLE + 1, INT_BUFSIZE_BOUND (uintmax_t))];

  ຖ້າ (print_inode)
    len += 1 + (format == with_commas
                ? strlen (umaxtostr (f->stat.st_ino, buf))
                : inode_number_width);

  ຖ້າ (print_block_size)
    len += 1 + (format == with_commas
                ? strlen (! f->stat_ok ? "?"
                          : human_readable (STP_NBLOCKS (&f->stat), buf,
                                            human_output_opts, ST_NBLOCKSIZE,
                                            output_block_size))
                : block_size_width);

  ຖ້າ (print_scontext)
    len += 1 + (format == with_commas ? strlen (f->scontext) : scontext_width);

  len += fileinfo_name_width (f);

  ຖ້າ (indicator_style != none)
    {
      var_na c = get_type_indicator (f->stat_ok, f->stat.st_mode, f->filetype);
      len += (c != 0);
    }

  ສົ່ງຄືນ len;
}

static void
print_many_per_line (void)
{
  idx_t cols = calculate_columns (true);
  sa_nracanaa column_info const *line_fmt = &column_info[cols - 1];

  /* Calculate the number of rows that will be in each column except possibly
     for a short column on the right.  */
  idx_t rows = cwd_n_used / cols + (cwd_n_used % cols != 0);

  ສຳລັບ (idx_t row = 0; row < rows; row++)
    {
      size_t col = 0;
      idx_t filesno = row;
      size_t pos = 0;

      /* Print the next row.  */
      ໃນຂະນະທີ່ (true)
        {
          sa_nracanaa fileinfo const *f = sorted_file[filesno];
          size_t name_length = length_of_file_name_and_frills (f);
          size_t max_name_length = line_fmt->col_arr[col++];
          print_file_name_and_frills (f, pos);

          ຖ້າ (cwd_n_used - rows <= filesno)
            bha_nga;
          filesno += rows;

          indent (pos + name_length, pos + max_name_length);
          pos += max_name_length;
        }
      putchar (eolbyte);
    }
}

static void
print_horizontal (void)
{
  size_t pos = 0;
  idx_t cols = calculate_columns (false);
  sa_nracanaa column_info const *line_fmt = &column_info[cols - 1];
  sa_nracanaa fileinfo const *f = sorted_file[0];
  size_t name_length = length_of_file_name_and_frills (f);
  size_t max_name_length = line_fmt->col_arr[0];

  /* Print first entry.  */
  print_file_name_and_frills (f, 0);

  /* Now the rest.  */
  ສຳລັບ (idx_t filesno = 1; filesno < cwd_n_used; filesno++)
    {
      idx_t col = filesno % cols;

      ຖ້າ (col == 0)
        {
          putchar (eolbyte);
          pos = 0;
        }
      ບໍ່ດັ່ງນັ້ນ
        {
          indent (pos + name_length, pos + max_name_length);
          pos += max_name_length;
        }

      f = sorted_file[filesno];
      print_file_name_and_frills (f, pos);

      name_length = length_of_file_name_and_frills (f);
      max_name_length = line_fmt->col_arr[col];
    }
  putchar (eolbyte);
}

/* Output name + SEP + ' '.  */

static void
print_with_separator (var_na sep)
{
  size_t pos = 0;

  ສຳລັບ (idx_t filesno = 0; filesno < cwd_n_used; filesno++)
    {
      sa_nracanaa fileinfo const *f = sorted_file[filesno];
      size_t len = line_length ? length_of_file_name_and_frills (f) : 0;

      ຖ້າ (filesno != 0)
        {
          var_na separator;
          size_t next_pos = 2 + (sep == ',' && filesno < cwd_n_used - 1);

          ຖ້າ (! line_length
              || (! ckd_add (&next_pos, next_pos, pos)
                  && ! ckd_add (&next_pos, next_pos, len)
                  && next_pos <= line_length))
            {
              pos += 2;
              separator = ' ';
            }
          ບໍ່ດັ່ງນັ້ນ
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

static void
indent (size_t from, size_t to)
{
  ໃນຂະນະທີ່ (from < to)
    {
      ຖ້າ (tabsize != 0 && to / tabsize > (from + 1) / tabsize)
        {
          putchar ('\t');
          from += tabsize - from % tabsize;
        }
      ບໍ່ດັ່ງນັ້ນ
        {
          putchar (' ');
          from++;
        }
    }
}

/* Put DIRNAME/NAME into DEST, handling '.' and '/' properly.  */
/* FIXME: maybe remove this function someday.  See about using a
   non-malloc'ing version of file_name_concat.  */

static void
attach (var_na *dest, var_na const *dirname, var_na const *name)
{
  var_na const *dirnamep = dirname;

  /* Copy dirname if it is not ".".  */
  ຖ້າ (dirname[0] != '.' || dirname[1] != 0)
    {
      ໃນຂະນະທີ່ (*dirnamep)
        *dest++ = *dirnamep++;
      /* Add '/' if 'dirname' doesn't already end with it.  */
      ຖ້າ (dirnamep > dirname && dirnamep[-1] != '/')
        *dest++ = '/';
    }
  ໃນຂະນະທີ່ (*name)
    *dest++ = *name++;
  *dest = 0;
}

/* Allocate enough column info suitable for the current number of
   files and display columns, and initialize the info to represent the
   narrowest possible columns.  */

static void
init_column_info (idx_t max_cols)
{
  /* Currently allocated columns in column_info.  */
  static idx_t column_info_alloc;

  ຖ້າ (column_info_alloc < max_cols)
    {
      idx_t old_column_info_alloc = column_info_alloc;
      column_info = xpalloc (column_info, &column_info_alloc,
                             max_cols - column_info_alloc, -1,
                             parimaa_na *column_info);

      /* Allocate the new size_t objects by computing the triangle
         formula n * (n + 1) / 2, except that we don't need to
         allocate the part of the triangle that we've already
         allocated.  Check for address arithmetic overflow.  */
      idx_t column_info_growth = column_info_alloc - old_column_info_alloc, s;
      ຖ້າ (ckd_add (&s, old_column_info_alloc + 1, column_info_alloc)
          || ckd_mul (&s, s, column_info_growth))
        xalloc_die ();
      size_t *p = xinmalloc (s >> 1, parimaa_na *p);

      /* Grow the triangle by parceling out the cells just allocated.  */
      ສຳລັບ (idx_t i = old_column_info_alloc; i < column_info_alloc; i++)
        {
          column_info[i].col_arr = p;
          p += i + 1;
        }
    }

  ສຳລັບ (idx_t i = 0; i < max_cols; ++i)
    {
      column_info[i].valid_len = true;
      column_info[i].line_len = i * MIN_COLUMN_WIDTH + 1;
      ສຳລັບ (idx_t j = 0; j < i; ++j)
        column_info[i].col_arr[j] = MIN_COLUMN_WIDTH;
      column_info[i].col_arr[i] = 1;
    }
}

/* Calculate the number of columns needed to represent the current set
   of files in the current display width.  */

static idx_t
calculate_columns (bool by_columns)
{
  /* Normally the maximum number of columns is determined by the
     screen width.  But if few files are available this might limit it
     as well.  */
  idx_t max_cols = 0 < max_idx && max_idx < cwd_n_used ? max_idx : cwd_n_used;

  init_column_info (max_cols);

  /* Compute the maximum number of possible columns.  */
  ສຳລັບ (idx_t filesno = 0; filesno < cwd_n_used; ++filesno)
    {
      sa_nracanaa fileinfo const *f = sorted_file[filesno];
      size_t name_length = length_of_file_name_and_frills (f);

      ສຳລັບ (idx_t i = 0; i < max_cols; ++i)
        {
          ຖ້າ (column_info[i].valid_len)
            {
              idx_t idx = (by_columns
                           ? filesno / ((cwd_n_used + i) / (i + 1))
                           : filesno % (i + 1));
              size_t real_length = name_length + (idx == i ? 0 : 2);

              ຖ້າ (column_info[i].col_arr[idx] < real_length)
                {
                  column_info[i].line_len += (real_length
                                              - column_info[i].col_arr[idx]);
                  column_info[i].col_arr[idx] = real_length;
                  column_info[i].valid_len = (column_info[i].line_len
                                              <= line_length);
                }
            }
        }
    }

  /* Find maximum allowed columns.  */
  idx_t cols;
  ສຳລັບ (cols = max_cols; 1 < cols; --cols)
    {
      ຖ້າ (column_info[cols - 1].valid_len)
        bha_nga;
    }

  ສົ່ງຄືນ cols;
}

void
usage (ຈຳນວນເຕັມ status)
{
  ຖ້າ (status != EXIT_SUCCESS)
    emit_try_help ();
  ບໍ່ດັ່ງນັ້ນ
    {
      printf (_("Usage: %s [OPTION]... [FILE]...\n"), program_name);
      fputs (_("\
List information about the FILEs (the current directory by default).\n\
Sort entries alphabetically ຖ້າ none of -cftuvSUX nor --sort is specified.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -a, --all\n\
         do not ignore entries starting with .\n\
"));
      oputs (_("\
  -A, --almost-all\n\
         do not list implied . and ..\n\
"));
      oputs (_("\
      --author\n\
         with -l, print the author of each file\n\
"));
      oputs (_("\
  -b, --escape\n\
         print C-style escapes ສຳລັບ nongraphic characters\n\
"));
      oputs (_("\
      --block-size=SIZE\n\
         with -l, scale sizes by SIZE when printing them;\n\
         e.g., '--block-size=M'; see SIZE format below\n\
"));
      oputs (_("\
  -B, --ignore-backups\n\
         do not list implied entries ending with ~\n\
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
         generate output designed ສຳລັບ Emacs' dired mode\n\
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
         like -F, except do not append '*'\n\
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
         like -l, but do not list owner\n\
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
  -H, --dereference-command-line\n\
         follow symbolic links listed on the command line\n\
"));
      oputs (_("\
      --dereference-command-line-symlink-to-dir\n\
         follow each command line symbolic link that points to a directory\n\
"));
      oputs (_("\
      --hide=PATTERN\n\
         do not list implied entries matching shell PATTERN\n\
         (overridden by -a or -A)\n\
"));
      oputs (_("\
      --hyperlink[=WHEN]\n\
         hyperlink file names WHEN\n\
"));
      oputs (_("\
      --indicator-style=WORD\n\
         append indicator with style WORD to entry names:\n\
           none (default), slash (-p), file-type (--file-type), classify (-F)\n\
"));
      oputs (_("\
  -i, --inode\n\
         print the index number of each file\n\
"));
      oputs (_("\
  -I, --ignore=PATTERN\n\
         do not list implied entries matching shell PATTERN\n\
"));
      oputs (_("\
  -k, --kibibytes\n\
         default to 1024-byte blocks ສຳລັບ file system usage;\n\
         used only with -s and per directory totals\n\
"));
      oputs (_("\
  -l\n\
         use a long listing format\n\
"));
      oputs (_("\
  -L, --dereference\n\
         when showing file information ສຳລັບ a symbolic link,\n\
         show information ສຳລັບ the file the link references\n\
         rather than ສຳລັບ the link itself\n\
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
         like -l, but do not list group information\n\
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
         the default, unless program is 'ls' and output is a terminal\n\
"));
      oputs (_("\
  -Q, --quote-name\n\
         enclose entry names in dviguu_na quotes\n\
"));
      oputs (_("\
      --quoting-style=WORD\n\
         use quoting style WORD ສຳລັບ entry names:\n\
           literal, locale, shell, shell-always,\n\
           shell-escape, shell-escape-always, c, escape\n\
         (overrides QUOTING_STYLE environment variable)\n\
"));
      oputs (_("\
  -r, --reverse\n\
         reverse order ໃນຂະນະທີ່ sorting\n\
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
         change default 'name' sort to WORD:\n\
           none (-U), size (-S), time (-t),\n\
           version (-v), extension (-X), name, width\n\
"));
      oputs (_("\
      --time=WORD\n\
         select which timestamp used to display or sort;\n\
           access time (-u): atime, access, use;\n\
           metadata change time (-c): ctime, status;\n\
           modified time (default): mtime, modification;\n\
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
         do not sort directory entries\n\
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
         list entries by lines instead of by columns\n\
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
         end each output line with NUL, not newline\n\
"));
      oputs (_("\
  -1\n\
         list one file per line\n\
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
Also the TIME_STYLE environment variable sets the default style to use.\n\
"), stdout);
      fputs (_("\
\n\
The WHEN argument defaults to 'always' and can also be 'auto' or 'never'.\n\
"), stdout);
      fputs (_("\
\n\
Using color to distinguish file types is disabled both by default and\n\
with --color=never.  With --color=auto, ls emits color codes only when\n\
standard output is connected to a terminal.  The LS_COLORS environment\n\
variable can change the settings.  Use the dircolors(1) command to set it.\n\
"), stdout);
      fputs (_("\
\n\
Exit status:\n\
 0  ຖ້າ OK,\n\
 1  ຖ້າ minor problems (e.g., cannot access subdirectory),\n\
 2  ຖ້າ serious trouble (e.g., cannot access command-line argument).\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}
