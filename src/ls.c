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
#پایان_اگر
#ضمن <sys/ioctl.h>

#اگر_تعریف_شده WINSIZE_درون_PTEM
# include <sys/stream.h>
# include <sys/ptem.h>
#پایان_اگر

#ضمن <stdio.h>
#ضمن <setjmp.h>
#ضمن <pwd.h>
#ضمن <getopt.h>
#ضمن <signal.h>

#إذا HAVE_LANGINFO_CODESET
# include <langinfo.h>
#پایان_اگر

/* NonStop circa 2011 lacks both SA_RESTART and siginterrupt, so don't
   restart syscalls after a signal handler fires.  This may cause
   colors to get messed up on the screen if 'ls' is interrupted, but
   that's the best we can do on such a platform.  */
#اگر_تعریف_نشده SA_RESTART
# define SA_RESTART 0
#پایان_اگر

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
#اگر_تعریف_شده HAVE_CAP
# include <sys/capability.h>
#پایان_اگر

#إذا HAVE_LINUX_XATTR_H
# include <linux/xattr.h>
# ifndef XATTR_NAME_CAPS
#  define XATTR_NAME_CAPS "security.capability"
# endif
#پایان_اگر

#تعریف_کن برنامه_NAME (ls_نما == LS_LS ? "ls" \
                      : (ls_نما == LS_MULTI_ستون \
                         ? "dir" : "vdir"))

#تعریف_کن AUTHORS \
  proper_name ("Richard M. Stallman"), \
  proper_name ("David MacKenzie")

#تعریف_کن obstack_chunk_alloc حافظه_بده
#تعریف_کن obstack_chunk_free حرر

/* Unix-based readdir implementations have historically returned a dirent.d_ino
   value that is sometimes not equal to the stat-obtained st_ino value for
   that same entry.  This error occurs for a readdir entry that refers
   to a mount point.  readdir's error is to return the inode number of
   the underlying directory -- one that typically cannot be stat'ed, as
   long as a file system is mounted on that directory.  RELIABLE_D_INO
   encapsulates whether we can use the more efficient approach of relying
   on readdir-supplied d_ino values, or whether we must incur the cost of
   calling stat or lstat to obtain each guaranteed-valid inode number.  */

#اگر_تعریف_نشده READDIR_LIES_ABOUT_MOUNTPOINT_D_INO
# define READDIR_LIES_ABOUT_MOUNTPOINT_D_INO 1
#پایان_اگر

#إذا READDIR_LIES_ABOUT_MOUNTPOINT_D_INO
# define RELIABLE_D_INO(dp) NOT_AN_INODE_عدد
#وإلا
# define RELIABLE_D_INO(dp) D_INO (dp)
#پایان_اگر

#إذا ! HAVE_STRUCT_STAT_ST_AUTHOR
# define st_author st_uid
#پایان_اگر

تعداد filetype
  {
    unknown,
    fifo,
    chardev,
    directory,
    blockdev,
    نرمال,
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
ساكن بیعلامت حرف ثابت filetype_d_type[] =
  {
    DT_UNKNOWN, DT_FIFO, DT_CHR, DT_DIR, DT_BLK, DT_REG, DT_LNK, DT_SOCK,
    DT_WHT, DT_DIR
  };
static_assert (countof (filetype_d_type) == filetype_cardinality);

/* Map d_type values to enum filetype.  */
ساكن حرف ثابت d_type_filetype[UCHAR_MAX + 1] =
  {
    [DT_BLK] = blockdev, [DT_CHR] = chardev, [DT_DIR] = directory,
    [DT_FIFO] = fifo, [DT_LNK] = symbolic_link, [DT_REG] = نرمال,
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
    size_t عرض;
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
#پایان_اگر

ساكن size_t quote_name (حرف ثابت *name,
                          بنية quoting_options ثابت *options,
                          صحيح needs_general_quoting,
                          ثابت بنية bin_str *color,
                          bool allow_pad, بنية obstack *پشته,
                          حرف ثابت *absolute_name);
ساكن size_t quote_name_buf (حرف **inbuf, size_t bufsize, حرف *name,
                              بنية quoting_options ثابت *options,
                              صحيح needs_general_quoting, size_t *عرض,
                              bool *pad);
ساكن صحيح decode_switches (صحيح argc, حرف **argv);
ساكن bool file_ignored (حرف ثابت *name);
ساكن uintmax_t gobble_file (حرف ثابت *name, تعداد filetype type,
                              ino_t inode, bool command_سطر_arg,
                              حرف ثابت *dirname);
ساكن ثابت بنية bin_str * get_color_indicator (ثابت بنية fileinfo *f,
                                                   bool symlink_هدف);
ساكن bool print_color_indicator (ثابت بنية bin_str *ind);
ساكن فراغ put_indicator (ثابت بنية bin_str *ind);
ساكن فراغ جمع_کن_ignore_الگو (حرف ثابت *الگو_2);
ساكن فراغ attach (حرف *dest, حرف ثابت *dirname, حرف ثابت *name);
ساكن فراغ clear_files (فراغ);
ساكن فراغ استخراج_dirs_from_files (حرف ثابت *dirname,
                                     bool command_سطر_arg);
ساكن bool get_link_name (حرف ثابت *filename, بنية fileinfo *f,
                           bool command_سطر_arg);
ساكن فراغ indent (size_t from, size_t مقصد);
ساكن idx_t calculate_columns (bool by_columns);
ساكن فراغ print_current_files (فراغ);
ساكن فراغ print_dir (حرف ثابت *name, حرف ثابت *realname,
                       bool command_سطر_arg);
ساكن size_t print_file_name_and_frills (ثابت بنية fileinfo *f,
                                          size_t شروع_ستون);
ساكن فراغ print_horizontal (فراغ);
ساكن صحيح format_user_عرض (uid_t u);
ساكن صحيح format_گروه_عرض (gid_t g);
ساكن فراغ print_long_format (ثابت بنية fileinfo *f);
ساكن فراغ print_many_در_هر_سطر (فراغ);
ساكن size_t print_name_with_quoting (ثابت بنية fileinfo *f,
                                       bool symlink_هدف,
                                       بنية obstack *پشته,
                                       size_t شروع_ستون);
ساكن فراغ prep_non_filename_متن (فراغ);
ساكن bool print_type_indicator (bool stat_ok, mode_t نما_2,
                                  تعداد filetype type);
ساكن فراغ print_with_separator (حرف sep);
ساكن فراغ صف_directory (حرف ثابت *name, حرف ثابت *realname,
                             bool command_سطر_arg);
ساكن فراغ مرتب_سازی_files (فراغ);
ساكن فراغ parse_ls_color (فراغ);

ساكن size_t quote_name_عرض (حرف ثابت *name,
                                بنية quoting_options ثابت *options,
                                صحيح needs_general_quoting);

/* Initial size of hash table.
   Most hierarchies are likely to be shallower than this.  */
تعداد { INITIAL_جدول_اندازه = 30 };

/* The set of 'active' directories, from the current command-line argument
   to the level in the hierarchy at which files are being listed.
   A directory is represented by its device and inode numbers (struct dev_ino).
   A directory is added to this set when ls begins listing it or its
   entries, and it is removed from the set just after ls has finished
   processing it.  This set is used solely to detect loops, e.g., with
   mkdir loop; cd loop; ln -s ../loop sub; ls -RL  */
ساكن Hash_جدول *فعال_dir_مجموعه;

#تعریف_کن LOOP_DETECT (!!فعال_dir_مجموعه)

/* The table of files in the current directory:

   'cwd_file' points to a vector of 'struct fileinfo', one per file.
   'cwd_n_alloc' is the number of elements space has been allocated for.
   'cwd_n_used' is the number actually in use.  */

/* Address of block containing the files that are described.  */
ساكن بنية fileinfo *cwd_file;

/* Length of block that 'cwd_file' points to, measured in files.  */
ساكن idx_t cwd_n_alloc;

/* Index of first unused slot in 'cwd_file'.  */
ساكن idx_t cwd_n_استفاده_شده;

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
file_or_link_نما (بنية fileinfo ثابت *file)
{
  أرجع (color_symlink_as_referent && file->linkok
          ? file->linkmode : file->stat.st_نما);
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
    بنية pending *بعدی;
  };

ساكن بنية pending *pending_dirs;

/* Current time in seconds and nanoseconds since 1970, updated as
   needed when deciding whether a file is recent.  */

ساكن بنية timespec current_زمان;

ساكن bool print_scontext;
ساكن حرف UNKNOWN_SECURITY_CONTEXT[] = "?";

/* Whether any of the files has an ACL.  This affects the width of the
   mode column.  */

ساكن bool any_has_acl;

/* The number of columns to use for columns containing inode numbers,
   block sizes, link counts, owners, groups, authors, major device
   numbers, minor device numbers, and file sizes, respectively.  */

ساكن صحيح inode_عدد_عرض;
ساكن صحيح بلوک_اندازه_عرض;
ساكن صحيح nlink_عرض;
ساكن صحيح scontext_عرض;
ساكن صحيح owner_عرض;
ساكن صحيح گروه_عرض;
ساكن صحيح author_عرض;
ساكن صحيح major_device_عدد_عرض;
ساكن صحيح minor_device_عدد_عرض;
ساكن صحيح file_اندازه_عرض;

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
    one_در_هر_سطر,		/* -1 */
    many_در_هر_سطر,		/* -C */
    horizontal,			/* -x */
    with_commas			/* -m */
  };

ساكن تعداد format format;

/* 'full-iso' uses full ISO-style dates and times.  'long-iso' uses longer
   ISO-style timestamps, though shorter than 'full-iso'.  'iso' uses shorter
   ISO-style timestamps.  'locale' uses locale-dependent timestamps.  */
تعداد زمان_style
  {
    پر_iso_زمان_style,	/* --time-style=full-iso */
    long_iso_زمان_style,	/* --time-style=long-iso */
    iso_زمان_style,		/* --time-style=iso */
    locale_زمان_style		/* --time-style=locale */
  };

ساكن حرف ثابت *ثابت زمان_style_آرگومانها[] =
{
  "full-iso", "long-iso", "iso", "locale", عدم
};
ساكن تعداد زمان_style ثابت زمان_style_types[] =
{
  پر_iso_زمان_style, long_iso_زمان_style, iso_زمان_style,
  locale_زمان_style
};
ARGMATCH_VERIFY (زمان_style_آرگومانها, زمان_style_types);

/* Type of time to print or sort by.  Controlled by -c and -u.
   The values of each item of this enum are important since they are
   used as indices in the sort functions array (see sort_files()).  */

تعداد زمان_type
  {
    زمان_mtime = 0,		/* default */
    زمان_ctime,			/* -c */
    زمان_atime,			/* -u */
    زمان_btime,                 /* birth time */
    زمان_numtypes		/* the number of elements of this enum */
  };

ساكن تعداد زمان_type زمان_type;
ساكن bool explicit_زمان;

/* The file characteristic to sort by.  Controlled by -t, -S, -U, -X, -v.
   The values of each item of this enum are important since they are
   used as indices in the sort functions array (see sort_files()).  */

تعداد مرتب_سازی_type
  {
    مرتب_سازی_name = 0,		/* default */
    مرتب_سازی_extension,		/* -X */
    مرتب_سازی_عرض,
    مرتب_سازی_اندازه,			/* -S */
    مرتب_سازی_نسخه,		/* -v */
    مرتب_سازی_زمان,			/* -t; must be second to last */
    مرتب_سازی_none,			/* -U; must be last */
    مرتب_سازی_numtypes		/* the number of elements of this enum */
  };

ساكن تعداد مرتب_سازی_type مرتب_سازی_type;

/* Direction of sort.
   false means highest first if numeric,
   lowest first if alphabetic;
   these are the defaults.
   true means the opposite order in each case.  -r  */

ساكن bool مرتب_سازی_معکوس;

/* True means to display owner information.  -g turns this off.  */

ساكن bool print_owner = true;

/* True means to display author information.  */

ساكن bool print_author;

/* True means to display group information.  -G and -o turn this off.  */

ساكن bool print_گروه = true;

/* True means print the user and group id's as numbers rather
   than as names.  -n  */

ساكن bool numeric_ids;

/* True means mention the size in blocks of each file.  -s  */

ساكن bool print_بلوک_اندازه;

/* Human-readable options for output, when printing block counts.  */
ساكن صحيح human_output_opts;

/* The units to use when printing block counts.  */
ساكن uintmax_t output_بلوک_اندازه;

/* Likewise, but for file sizes.  */
ساكن صحيح file_human_output_opts;
ساكن uintmax_t file_output_بلوک_اندازه = 1;

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
ساكن حرف ثابت *ثابت indicator_style_آرگومانها[] =
{
  "none", "slash", "file-type", "classify", عدم
};
ساكن تعداد indicator_style ثابت indicator_style_types[] =
{
  none, slash, file_type, classify
};
ARGMATCH_VERIFY (indicator_style_آرگومانها, indicator_style_types);

/* True means use colors to mark types.  Also define the different
   colors as well as the stuff for the LS_COLORS environment variable.
   The LS_COLORS variable is now in a termcap-like format.  */

ساكن bool print_with_color;

ساكن bool print_hyperlink;

/* Whether we used any colors in the output so far.  If so, we will
   need to restore the default color later.  If not, we will need to
   call prep_non_filename_text before using color for the first time. */

ساكن bool استفاده_شده_color = false;

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
    DEREF_COMMAND_سطر_SYMLINK_مقصد_DIR,	/* the default, in certain cases */
    DEREF_ALWAYS			/* -L */
  };

تعداد indicator_no
  {
    C_چپ, C_راست, C_انتها, C_RESET, C_NORM, C_FILE, C_DIR, C_LINK,
    C_FIFO, C_SOCK,
    C_BLK, C_CHR, C_MISSING, C_ORPHAN, C_EXEC, C_DOOR, C_SETUID, C_SETGID,
    C_STICKY, C_OTHER_WRITABLE, C_STICKY_OTHER_WRITABLE, C_CAP, C_MULTIHARDLINK,
    C_CLR_مقصد_EOL
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
    bool   exact_تطابق;		/* Whether to compare case insensitively */
    بنية color_ext_type *بعدی;	/* Next in list */
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

ساكن bool check_symlink_نما;

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

ساكن bool فوری_dirs;

/* True means that directories are grouped before files. */

ساكن bool directories_اولی;

/* Which files to ignore.  */

ساكن تعداد
{
  /* Ignore files whose names start with '.', and files specified by
     --hide and --ignore.  */
  IGNORE_DEFAULT = 0,

  /* Ignore '.', '..', and files specified by --ignore.  */
  IGNORE_DOT_AND_DOTDOT,

  /* Ignore only files specified by --ignore.  */
  IGNORE_کمینه
} ignore_نما;

/* A linked list of shell-style globbing patterns.  If a non-argument
   file name matches any of these patterns, it is ignored.
   Controlled by -I.  Multiple -I options accumulate.
   The -B option adds '*~' and '.*~' to this list.  */

بنية ignore_الگو
  {
    حرف ثابت *الگو_2;
    بنية ignore_الگو *بعدی;
  };

ساكن بنية ignore_الگو *ignore_patterns;

/* Similar to IGNORE_PATTERNS, except that -a or -A causes this
   variable itself to be ignored.  */
ساكن بنية ignore_الگو *hide_patterns;

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

ساكن size_t سطر_درازی;

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

تعداد { زمان_STAMP_LEN_بیشینه = MAX (1000, INT_STRLEN_BOUND (time_t)) };

/* strftime formats for non-recent and recent files, respectively, in
   -l output.  */

ساكن حرف ثابت *long_زمان_format[2] =
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

ساكن sigset_t caught_سیگنالها;

/* If nonzero, the value of the pending fatal signal.  */

ساكن sig_atomic_t متطاير وقفه_signal;

/* A count of the number of pending stop signals that have been received.  */

ساكن sig_atomic_t متطاير stop_signal_شمار;

/* Desired exit status.  */

ساكن صحيح exit_وضعیت;

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
  بلوک_اندازه_OPTION,
  COLOR_OPTION,
  DEREFERENCE_COMMAND_سطر_SYMLINK_مقصد_DIR_OPTION,
  FILE_TYPE_INDICATOR_OPTION,
  FORMAT_OPTION,
  پر_زمان_OPTION,
  گروه_DIRECTORIES_اولی_OPTION,
  HIDE_OPTION,
  HYPERLINK_OPTION,
  INDICATOR_STYLE_OPTION,
  QUOTING_STYLE_OPTION,
  SHOW_CONTROL_CHARS_OPTION,
  SI_OPTION,
  مرتب_سازی_OPTION,
  زمان_OPTION,
  زمان_STYLE_OPTION,
  ZERO_OPTION,
};

ساكن بنية option ثابت long_options[] =
{
  {"all", no_آرگومان, عدم, 'a'},
  {"escape", no_آرگومان, عدم, 'b'},
  {"directory", no_آرگومان, عدم, 'd'},
  {"dired", no_آرگومان, عدم, 'D'},
  {"full-time", no_آرگومان, عدم, پر_زمان_OPTION},
  {"group-directories-first", no_آرگومان, عدم,
   گروه_DIRECTORIES_اولی_OPTION},
  {"human-readable", no_آرگومان, عدم, 'h'},
  {"inode", no_آرگومان, عدم, 'i'},
  {"kibibytes", no_آرگومان, عدم, 'k'},
  {"numeric-uid-gid", no_آرگومان, عدم, 'n'},
  {"no-group", no_آرگومان, عدم, 'G'},
  {"hide-control-chars", no_آرگومان, عدم, 'q'},
  {"reverse", no_آرگومان, عدم, 'r'},
  {"size", no_آرگومان, عدم, 's'},
  {"width", required_آرگومان, عدم, 'w'},
  {"almost-all", no_آرگومان, عدم, 'A'},
  {"ignore-backups", no_آرگومان, عدم, 'B'},
  {"classify", optional_آرگومان, عدم, 'F'},
  {"file-type", no_آرگومان, عدم, FILE_TYPE_INDICATOR_OPTION},
  {"si", no_آرگومان, عدم, SI_OPTION},
  {"dereference-command-line", no_آرگومان, عدم, 'H'},
  {"dereference-command-line-symlink-to-dir", no_آرگومان, عدم,
   DEREFERENCE_COMMAND_سطر_SYMLINK_مقصد_DIR_OPTION},
  {"hide", required_آرگومان, عدم, HIDE_OPTION},
  {"ignore", required_آرگومان, عدم, 'I'},
  {"indicator-style", required_آرگومان, عدم, INDICATOR_STYLE_OPTION},
  {"dereference", no_آرگومان, عدم, 'L'},
  {"literal", no_آرگومان, عدم, 'N'},
  {"quote-name", no_آرگومان, عدم, 'Q'},
  {"quoting-style", required_آرگومان, عدم, QUOTING_STYLE_OPTION},
  {"recursive", no_آرگومان, عدم, 'R'},
  {"format", required_آرگومان, عدم, FORMAT_OPTION},
  {"show-control-chars", no_آرگومان, عدم, SHOW_CONTROL_CHARS_OPTION},
  {"sort", required_آرگومان, عدم, مرتب_سازی_OPTION},
  {"tabsize", required_آرگومان, عدم, 'T'},
  {"time", required_آرگومان, عدم, زمان_OPTION},
  {"time-style", required_آرگومان, عدم, زمان_STYLE_OPTION},
  {"zero", no_آرگومان, عدم, ZERO_OPTION},
  {"color", optional_آرگومان, عدم, COLOR_OPTION},
  {"hyperlink", optional_آرگومان, عدم, HYPERLINK_OPTION},
  {"block-size", required_آرگومان, عدم, بلوک_اندازه_OPTION},
  {"context", no_آرگومان, عدم, 'Z'},
  {"author", no_آرگومان, عدم, AUTHOR_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخه_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

ساكن حرف ثابت *ثابت format_آرگومانها[] =
{
  "verbose", "long", "commas", "horizontal", "across",
  "vertical", "single-column", عدم
};
ساكن تعداد format ثابت format_types[] =
{
  long_format, long_format, with_commas, horizontal, horizontal,
  many_در_هر_سطر, one_در_هر_سطر
};
ARGMATCH_VERIFY (format_آرگومانها, format_types);

ساكن حرف ثابت *ثابت مرتب_سازی_آرگومانها[] =
{
  "none", "size", "time", "version", "extension",
  "name", "width", عدم
};
ساكن تعداد مرتب_سازی_type ثابت مرتب_سازی_types[] =
{
  مرتب_سازی_none, مرتب_سازی_اندازه, مرتب_سازی_زمان, مرتب_سازی_نسخه, مرتب_سازی_extension,
  مرتب_سازی_name, مرتب_سازی_عرض
};
ARGMATCH_VERIFY (مرتب_سازی_آرگومانها, مرتب_سازی_types);

ساكن حرف ثابت *ثابت زمان_آرگومانها[] =
{
  "atime", "access", "use",
  "ctime", "status",
  "mtime", "modification",
  "birth", "creation",
  عدم
};
ساكن تعداد زمان_type ثابت زمان_types[] =
{
  زمان_atime, زمان_atime, زمان_atime,
  زمان_ctime, زمان_ctime,
  زمان_mtime, زمان_mtime,
  زمان_btime, زمان_btime,
};
ARGMATCH_VERIFY (زمان_آرگومانها, زمان_types);

ساكن حرف ثابت *ثابت when_آرگومانها[] =
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
ARGMATCH_VERIFY (when_آرگومانها, when_types);

/* Information about filling a column.  */
بنية ستونی_info
{
  bool valid_len;
  size_t سطر_len;
  size_t *ستون_arr;
};

/* Array with information about column fullness.  */
ساكن بنية ستونی_info *ستونی_info;

/* Maximum number of columns ever possible for this display.  */
ساكن size_t max_idx;

/* The minimum width of a non-final column is 3: 1 character for the name
   and 2 for the separating white space.  The final column is not padded.  */
تعداد { MIN_ستونی_عرض = 3 };


/* This zero-based index is for the --dired option.  It is incremented
   for each byte of output generated by this program so that the beginning
   and ending indices (in that output) of every file name can be recorded
   and later output themselves.  */
ساكن off_t dired_pos;

ساكن فراغ
dired_outbyte (حرف c)
{
  dired_pos++;
  حرف_چاپ_کن (c);
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
  dired_outbuf (s, طول_رشته (s));
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
افزودن_current_dired_pos (بنية obstack *obs)
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
dev_ino_افزودن (dev_t dev, ino_t ino)
{
  فراغ *vdi;
  بنية dev_ino *di;
  صحيح dev_ino_اندازه = حجم *di;
  obstack_blank (&dev_ino_obstack, dev_ino_اندازه);
  vdi = obstack_بعدی_free (&dev_ino_obstack);
  di = vdi;
  di--;
  di->st_dev = dev;
  di->st_ino = ino;
}

/* Pop a dev/ino struct off the global dev_ino_obstack
   and return that struct.  */
ساكن بنية dev_ino
dev_ino_برداشت (فراغ)
{
  فراغ *vdi;
  بنية dev_ino *di;
  صحيح dev_ino_اندازه = حجم *di;
  affirm (dev_ino_اندازه <= obstack_object_اندازه (&dev_ino_obstack));
  obstack_blank_fast (&dev_ino_obstack, -dev_ino_اندازه);
  vdi = obstack_بعدی_free (&dev_ino_obstack);
  di = vdi;
  أرجع *di;
}

ساكن فراغ
assert_matching_dev_ino (حرف ثابت *name, بنية dev_ino di)
{
  MAYBE_استفادهنشده بنية stat sb;
  assure (0 <= stat (name, &sb));
  assure (sb.st_dev == di.st_dev);
  assure (sb.st_ino == di.st_ino);
}

ساكن حرف eolbyte = '\n';

/* Write to standard output PREFIX, followed by the quoting style and
   a space-separated list of the integers stored in OS all on one line.  */

ساكن فراغ
dired_dump_obstack (حرف ثابت *پیشوند, بنية obstack *os)
{
  size_t n_pos;

  n_pos = obstack_object_اندازه (os) / حجم (dired_pos);
  إذا (n_pos > 0)
    {
      off_t *pos = obstack_پایان (os);
      fputs (پیشوند, stdout);
      لكل (size_t i = 0; i < n_pos; i++)
        {
          intmax_t p = pos[i];
          چاپ_کن (" %jd", p);
        }
      حرف_چاپ_کن ('\n');
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
#پایان_اگر

  أرجع btimespec;
}

#إذا HAVE_STATX && defined STATX_INO
ATTRIBUTE_خالص
ساكن بیعلامت صحيح
زمان_type_مقصد_statx (فراغ)
{
  بدل (زمان_type)
    {
    حالة زمان_ctime:
      أرجع STATX_CTIME;
    حالة زمان_mtime:
      أرجع STATX_MTIME;
    حالة زمان_atime:
      أرجع STATX_ATIME;
    حالة زمان_btime:
      أرجع STATX_BTIME;
    حالة زمان_numtypes: افتراضي:
      unreachable ();
    }
    أرجع 0;
}

ATTRIBUTE_خالص
ساكن بیعلامت صحيح
calc_req_ماسک (فراغ)
{
  بیعلامت صحيح ماسک = STATX_نما;

  إذا (print_inode)
    ماسک |= STATX_INO;

  إذا (print_بلوک_اندازه)
    ماسک |= STATX_بلوکها;

  إذا (format == long_format) {
    ماسک |= STATX_NLINK | STATX_اندازه | زمان_type_مقصد_statx ();
    إذا (print_owner || print_author)
      ماسک |= STATX_UID;
    إذا (print_گروه)
      ماسک |= STATX_GID;
  }

  بدل (مرتب_سازی_type)
    {
    حالة مرتب_سازی_none:
    حالة مرتب_سازی_name:
    حالة مرتب_سازی_نسخه:
    حالة مرتب_سازی_extension:
    حالة مرتب_سازی_عرض:
      اكسر;
    حالة مرتب_سازی_زمان:
      ماسک |= زمان_type_مقصد_statx ();
      اكسر;
    حالة مرتب_سازی_اندازه:
      ماسک |= STATX_اندازه;
      اكسر;
    حالة مرتب_سازی_numtypes: افتراضي:
      unreachable ();
    }

  أرجع ماسک;
}

ساكن صحيح
do_statx (صحيح fd, حرف ثابت *name, بنية stat *st, صحيح flags,
          بیعلامت صحيح ماسک)
{
  بنية statx stx;
  bool want_btime = ماسک & STATX_BTIME;
  صحيح ret = statx (fd, name, flags | AT_NO_AUTOMOUNT, ماسک, &stx);
  إذا (ret >= 0)
    {
      statx_مقصد_stat (&stx, st);
      /* Since we only need one timestamp type,
         store birth time in st_mtim.  */
      إذا (want_btime)
        {
          إذا (stx.stx_ماسک & STATX_BTIME)
            st->st_mtim = statx_timestamp_مقصد_timespec (stx.stx_btime);
          وإلا
            st->st_mtim.tv_sec = st->st_mtim.tv_nsec = -1;
        }
    }

  أرجع ret;
}

ساكن صحيح
do_stat (حرف ثابت *name, بنية stat *st)
{
  أرجع do_statx (AT_FDCWD, name, st, 0, calc_req_ماسک ());
}

ساكن صحيح
do_lstat (حرف ثابت *name, بنية stat *st)
{
  أرجع do_statx (AT_FDCWD, name, st, AT_SYMLINK_NOFOLLOW, calc_req_ماسک ());
}

ساكن صحيح
stat_for_نما (حرف ثابت *name, بنية stat *st)
{
  أرجع do_statx (AT_FDCWD, name, st, 0, STATX_نما);
}

/* dev+ino should be static, so no need to sync with backing store */
ساكن صحيح
stat_for_ino (حرف ثابت *name, بنية stat *st)
{
  أرجع do_statx (AT_FDCWD, name, st, 0, STATX_INO);
}

ساكن صحيح
fstat_for_ino (صحيح fd, بنية stat *st)
{
  أرجع do_statx (fd, "", st, AT_خالی_مسیر, STATX_INO);
}
#وإلا
ساكن صحيح
do_stat (حرف ثابت *name, بنية stat *st)
{
  أرجع stat (name, st);
}

ساكن صحيح
do_lstat (حرف ثابت *name, بنية stat *st)
{
  أرجع lstat (name, st);
}

ساكن صحيح
stat_for_نما (حرف ثابت *name, بنية stat *st)
{
  أرجع stat (name, st);
}

ساكن صحيح
stat_for_ino (حرف ثابت *name, بنية stat *st)
{
  أرجع stat (name, st);
}

ساكن صحيح
fstat_for_ino (صحيح fd, بنية stat *st)
{
  أرجع fstat (fd, st);
}
#پایان_اگر

/* Return the address of the first plain %b spec in FMT, or NULL if
   there is no such spec.  %5b etc. do not match, so that user
   widths/flags are honored.  */

ATTRIBUTE_خالص
ساكن حرف ثابت *
اولی_percent_b (حرف ثابت *fmt)
{
  لكل (; *fmt; fmt++)
    إذا (fmt[0] == '%')
      بدل (fmt[1])
        {
        حالة 'b': أرجع fmt;
        حالة '%': fmt++; اكسر;
        }
  أرجع عدم;
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
تعداد { ABFORMAT_اندازه = 128 };
ساكن حرف abformat[2][12][ABFORMAT_اندازه];
/* True if precomputed formats should be used.  This can be false if
   nl_langinfo fails, if a format or month abbreviation is unusually
   long, or if a month abbreviation contains '%'.  */
ساكن bool use_abformat;

/* Store into ABMON the abbreviated month names, suitably aligned.
   Return true if successful.  */

ساكن bool
abmon_init (حرف abmon[12][ABFORMAT_اندازه])
{
#اگر_تعریف_نشده HAVE_NL_LANGINFO
  أرجع false;
#وإلا
  صحيح max_mon_عرض = 0;
  صحيح mon_عرض[12];
  صحيح mon_len[12];

  لكل (صحيح i = 0; i < 12; i++)
    {
      حرف ثابت *abbr = nl_langinfo (ABMON_1 + i);
      mon_len[i] = strnlen (abbr, ABFORMAT_اندازه);
      إذا (mon_len[i] == ABFORMAT_اندازه)
        أرجع false;
      إذا (strchr (abbr, '%'))
        أرجع false;
      mon_عرض[i] = mbswidth (strcpy (abmon[i], abbr), MBSWIDTH_FLAGS);
      إذا (mon_عرض[i] < 0)
        أرجع false;
      max_mon_عرض = MAX (max_mon_عرض, mon_عرض[i]);
    }

  لكل (صحيح i = 0; i < 12; i++)
    {
      صحيح fill = max_mon_عرض - mon_عرض[i];
      إذا (ABFORMAT_اندازه - mon_len[i] <= fill)
        أرجع false;
      bool align_چپ = !c_isdigit (abmon[i][0]);
      صحيح fill_جابه_جایی;
      إذا (align_چپ)
        fill_جابه_جایی = mon_len[i];
      وإلا
        {
          memmove (abmon[i] + fill, abmon[i], mon_len[i]);
          fill_جابه_جایی = 0;
        }
      memset (abmon[i] + fill_جابه_جایی, ' ', fill);
      abmon[i][mon_len[i] + fill] = '\0';
    }

  أرجع true;
#پایان_اگر
}

/* Initialize ABFORMAT and USE_ABFORMAT.  */

ساكن فراغ
abformat_init (فراغ)
{
  حرف ثابت *pb[2];
  لكل (صحيح recent = 0; recent < 2; recent++)
    pb[recent] = اولی_percent_b (long_زمان_format[recent]);
  إذا (! (pb[0] || pb[1]))
    أرجع;

  حرف abmon[12][ABFORMAT_اندازه];
  إذا (! abmon_init (abmon))
    أرجع;

  لكل (صحيح recent = 0; recent < 2; recent++)
    {
      حرف ثابت *fmt = long_زمان_format[recent];
      لكل (صحيح i = 0; i < 12; i++)
        {
          حرف *nfmt = abformat[recent][i];
          صحيح nbytes;

          إذا (! pb[recent])
            nbytes = snprintf (nfmt, ABFORMAT_اندازه, "%s", fmt);
          وإلا
            {
              إذا (! (pb[recent] - fmt <= MIN (ABFORMAT_اندازه, بیشترین_صحیح)))
                أرجع;
              صحيح پیشوند_len = pb[recent] - fmt;
              nbytes = snprintf (nfmt, ABFORMAT_اندازه, "%.*s%s%s",
                                 پیشوند_len, fmt, abmon[i], pb[recent] + 2);
            }

          إذا (! (0 <= nbytes && nbytes < ABFORMAT_اندازه))
            أرجع;
        }
    }

  use_abformat = true;
}

ساكن size_t
dev_ino_hash (فراغ ثابت *x, size_t جدول_اندازه)
{
  بنية dev_ino ثابت *p = x;
  أرجع (uintmax_t) {p->st_ino} % جدول_اندازه;
}

ساكن bool
dev_ino_compare (فراغ ثابت *x, فراغ ثابت *y)
{
  بنية dev_ino ثابت *a = x;
  بنية dev_ino ثابت *b = y;
  أرجع PSAME_INODE (a, b);
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
بازدید_dir (dev_t dev, ino_t ino)
{
  بنية dev_ino *ent;
  بنية dev_ino *ent_from_جدول;
  bool یافت_تطابق;

  ent = xmalloc (حجم *ent);
  ent->st_ino = ino;
  ent->st_dev = dev;

  /* Attempt to insert this entry into the table.  */
  ent_from_جدول = hash_درج (فعال_dir_مجموعه, ent);

  إذا (ent_from_جدول == عدم)
    {
      /* Insertion failed due to lack of memory.  */
      xalloc_die ();
    }

  یافت_تطابق = (ent_from_جدول != ent);

  إذا (یافت_تطابق)
    {
      /* ent was not inserted, so free it.  */
      حرر (ent);
    }

  أرجع یافت_تطابق;
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
    أرجع false;
  إذا (2 < len)
    أرجع true;
  حرف ثابت *s = color_indicator[type].string;
  أرجع (s[0] != '0') | (s[len - 1] != '0');
}

ساكن فراغ
restore_default_color (فراغ)
{
  put_indicator (&color_indicator[C_چپ]);
  put_indicator (&color_indicator[C_راست]);
}

ساكن فراغ
مجموعه_نرمال_color (فراغ)
{
  إذا (print_with_color && is_colored (C_NORM))
    {
      put_indicator (&color_indicator[C_چپ]);
      put_indicator (&color_indicator[C_NORM]);
      put_indicator (&color_indicator[C_راست]);
    }
}

/* An ordinary signal was received; arrange for the program to exit.  */

ساكن فراغ
sighandler (صحيح sig)
{
  إذا (! وقفه_signal)
    وقفه_signal = sig;
}

/* A SIGTSTP was received; arrange for the program to suspend itself.  */

ساكن فراغ
stophandler (MAYBE_استفادهنشده صحيح sig)
{
  إذا (! وقفه_signal)
    stop_signal_شمار++;
}

/* Process any pending signals.  If signals are caught, this function
   should be called periodically.  Ideally there should never be an
   unbounded amount of time when signals are not being processed.
   Signal handling can restore the default colors, so callers must
   immediately change colors after invoking this function.  */

ساكن فراغ
process_سیگنالها (فراغ)
{
  طالما (وقفه_signal || stop_signal_شمار)
    {
      صحيح sig;
      صحيح stops;
      sigset_t oldset;

      إذا (استفاده_شده_color)
        restore_default_color ();
      fflush (stdout);

      sigprocmask (SIG_BLOCK, &caught_سیگنالها, &oldset);

      /* Reload interrupt_signal and stop_signal_count, in case a new
         signal was handled before sigprocmask took effect.  */
      sig = وقفه_signal;
      stops = stop_signal_شمار;

      /* SIGTSTP is special, since the application can receive that signal
         more than once.  In this case, don't set the signal handler to the
         default.  Instead, just raise the uncatchable SIGSTOP.  */
      إذا (stops)
        {
          stop_signal_شمار = stops - 1;
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
signal_راهاندازی (bool init)
{
  /* The signals that are trapped, and the number of such signals.  */
  ساكن صحيح ثابت stop_sig[] =
    {
      /* This one is handled specially.  */
      SIGTSTP,
    };

  تعداد { nsigs = countof (جمله_sig), nstop = countof (stop_sig) };

  إذا (init)
    {
      بنية sigaction act;

      sigemptyset (&caught_سیگنالها);
      لكل (صحيح j = 0; j < nsigs + nstop; j++)
        {
          صحيح sig = j < nsigs ? جمله_sig[j]: stop_sig[j - nsigs];
          sigaction (sig, عدم, &act);
          إذا (act.sa_handler != SIG_IGN)
            sigaddset (&caught_سیگنالها, sig);
        }

      act.sa_mask = caught_سیگنالها;
      act.sa_flags = SA_RESTART;

      لكل (صحيح j = 0; j < nsigs + nstop; j++)
        {
          صحيح sig = j < nsigs ? جمله_sig[j]: stop_sig[j - nsigs];
          إذا (sigismember (&caught_سیگنالها, sig))
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
          صحيح sig = j < nsigs ? جمله_sig[j]: stop_sig[j - nsigs];
          إذا (sigismember (&caught_سیگنالها, sig))
            إشارة (sig, SIG_DFL);
        }
    }
}

ساكن فراغ
signal_init (فراغ)
{
  signal_راهاندازی (true);
}

ساكن فراغ
signal_restore (فراغ)
{
  signal_راهاندازی (false);
}

صحيح
اصلی (صحيح argc, حرف **argv)
{
  صحيح i;
  بنية pending *thispend;
  صحيح n_files;

  initialize_main (&argc, &argv);
  مجموعه_برنامه_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (LS_FAILURE);
  atexit (close_stdout);

  static_assert (countof (color_indicator) == countof (indicator_name));

  exit_وضعیت = EXIT_SUCCESS;
  print_dir_name = true;
  pending_dirs = عدم;

  current_زمان.tv_sec = TYPE_MINIMUM (time_t);
  current_زمان.tv_nsec = -1;

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

  إذا (directories_اولی)
    check_symlink_نما = true;
  وإلا إذا (print_with_color)
    {
      /* Avoid following symbolic links when possible.  */
      إذا (is_colored (C_ORPHAN)
          || (is_colored (C_EXEC) && color_symlink_as_referent)
          || (is_colored (C_MISSING) && format == long_format))
        check_symlink_نما = true;
    }

  إذا (dereference == DEREF_UNDEFINED)
    dereference = ((فوری_dirs
                    || indicator_style == classify
                    || format == long_format)
                   ? DEREF_NEVER
                   : DEREF_COMMAND_سطر_SYMLINK_مقصد_DIR);

  /* When using -R, initialize a data structure we'll use to
     detect any directory cycles.  */
  إذا (recursive)
    {
      فعال_dir_مجموعه = hash_initialize (INITIAL_جدول_اندازه, عدم,
                                        dev_ino_hash,
                                        dev_ino_compare,
                                        dev_ino_free);
      إذا (فعال_dir_مجموعه == عدم)
        xalloc_die ();

      obstack_init (&dev_ino_obstack);
    }

  localtz = tzalloc (getenv ("TZ"));

  format_needs_stat = ((مرتب_سازی_type == مرتب_سازی_زمان) | (مرتب_سازی_type == مرتب_سازی_اندازه)
                       | (format == long_format)
                       | print_بلوک_اندازه | print_hyperlink | print_scontext);
  format_needs_type = ((! format_needs_stat)
                       & (recursive | print_with_color | print_scontext
                          | directories_اولی
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
  cwd_n_استفاده_شده = 0;

  clear_files ();

  n_files = argc - i;

  إذا (n_files <= 0)
    {
      إذا (فوری_dirs)
        gobble_file (".", directory, NOT_AN_INODE_عدد, true, عدم);
      وإلا
        صف_directory (".", عدم, true);
    }
  وإلا
    افعل
      gobble_file (argv[i++], unknown, NOT_AN_INODE_عدد, true, عدم);
    طالما (i < argc);

  إذا (cwd_n_استفاده_شده)
    {
      مرتب_سازی_files ();
      إذا (!فوری_dirs)
        استخراج_dirs_from_files (عدم, true);
      /* 'cwd_n_used' might be zero now.  */
    }

  /* In the following if/else blocks, it is sufficient to test 'pending_dirs'
     (and not pending_dirs->name) because there may be no markers in the queue
     at this point.  A marker may be enqueued when extract_dirs_from_files is
     called with a non-empty string or via print_dir.  */
  إذا (cwd_n_استفاده_شده)
    {
      print_current_files ();
      إذا (pending_dirs)
        dired_outbyte ('\n');
    }
  وإلا إذا (n_files <= 1 && pending_dirs && !pending_dirs->بعدی)
    print_dir_name = false;

  طالما (pending_dirs)
    {
      thispend = pending_dirs;
      pending_dirs = pending_dirs->بعدی;

      إذا (LOOP_DETECT)
        {
          إذا (thispend->name == عدم)
            {
              /* thispend->name == NULL means this is a marker entry
                 indicating we've finished processing the directory.
                 Use its dev/ino numbers to remove the corresponding
                 entry from the active_dir_set hash table.  */
              بنية dev_ino di = dev_ino_برداشت ();
              بنية dev_ino *یافت = hash_remove (فعال_dir_مجموعه, &di);
              إذا (false)
                assert_matching_dev_ino (thispend->realname, di);
              affirm (یافت);
              dev_ino_free (یافت);
              free_pending_ent (thispend);
              استمر;
            }
        }

      print_dir (thispend->name, thispend->realname,
                 thispend->command_سطر_arg);

      free_pending_ent (thispend);
      print_dir_name = true;
    }

  إذا (print_with_color && استفاده_شده_color)
    {

      /* Skip the restore when it would be a no-op, i.e.,
         when left is "\033[" and right is "m".  */
      إذا (!(color_indicator[C_چپ].len == 2
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
      لكل (صحيح j = stop_signal_شمار; j; j--)
        ارفع (SIGSTOP);
      صحيح j = وقفه_signal;
      إذا (j)
        ارفع (j);
    }

  إذا (dired)
    {
      /* No need to free these since we're about to exit.  */
      dired_dump_obstack ("//DIRED//", &dired_obstack);
      dired_dump_obstack ("//SUBDIRED//", &subdired_obstack);
      چاپ_کن ("//DIRED-OPTIONS// --quoting-style=%s\n",
              quoting_style_آرگومانها[get_quoting_style (filename_quoting_options)]);
    }

  إذا (LOOP_DETECT)
    {
      assure (hash_get_n_entries (فعال_dir_مجموعه) == 0);
      hash_free (فعال_dir_مجموعه);
    }

  أرجع exit_وضعیت;
}

/* Return the line length indicated by the value given by SPEC, or -1
   if unsuccessful.  0 means no limit on line length.  */

ساكن ptrdiff_t
decode_سطر_درازی (حرف ثابت *spec)
{
  uintmax_t val;

  /* Treat too-large values as if they were 0, which is
     effectively infinity.  */
  بدل (xstrtoumax (spec, عدم, 0, &val, ""))
    {
    حالة LONGINT_OK:
      أرجع val <= MIN (PTRDIFF_MAX, SIZE_MAX) ? val : 0;

    حالة LONGINT_سرریز:
      أرجع 0;

    حالة LONGINT_INVALID:
    حالة LONGINT_INVALID_SUFFIX_CHAR:
    حالة LONGINT_INVALID_SUFFIX_CHAR_WITH_سرریز:
      أرجع -1;

    افتراضي:
      unreachable ();
    }
}

/* Return true if standard output is a tty, caching the result.  */

ساكن bool
stdout_isatty (فراغ)
{
  ساكن علامتدار حرف بیرون_tty = -1;
  إذا (بیرون_tty < 0)
    بیرون_tty = isatty (STDOUT_FILENO);
  assume (بیرون_tty == 0 || بیرون_tty == 1);
  أرجع بیرون_tty;
}

/* Set all the option flags according to the switches specified.
   Return the index of the first non-option argument.  */

ساكن صحيح
decode_switches (صحيح argc, حرف **argv)
{
  حرف *زمان_style_option = عدم;

  /* These variables are false or -1 unless a switch says otherwise.  */
  bool kibibytes_specified = false;
  صحيح format_opt = -1;
  صحيح hide_control_chars_opt = -1;
  صحيح quoting_style_opt = -1;
  صحيح مرتب_سازی_opt = -1;
  ptrdiff_t tabsize_opt = -1;
  ptrdiff_t عرض_opt = -1;

  طالما (true)
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
          ignore_نما = IGNORE_کمینه;
          اكسر;

        حالة 'b':
          quoting_style_opt = escape_quoting_style;
          اكسر;

        حالة 'c':
          زمان_type = زمان_ctime;
          explicit_زمان = true;
          اكسر;

        حالة 'd':
          فوری_dirs = true;
          اكسر;

        حالة 'f':
          ignore_نما = IGNORE_کمینه; /* enable -a */
          مرتب_سازی_opt = مرتب_سازی_none;         /* enable -U */
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
            human_autoscale | human_SI | human_پایه_1024;
          file_output_بلوک_اندازه = output_بلوک_اندازه = 1;
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
          print_گروه = false;
          اكسر;

        حالة 'p':
          indicator_style = slash;
          اكسر;

        حالة 'q':
          hide_control_chars_opt = true;
          اكسر;

        حالة 'r':
          مرتب_سازی_معکوس = true;
          اكسر;

        حالة 's':
          print_بلوک_اندازه = true;
          اكسر;

        حالة 't':
          مرتب_سازی_opt = مرتب_سازی_زمان;
          اكسر;

        حالة 'u':
          زمان_type = زمان_atime;
          explicit_زمان = true;
          اكسر;

        حالة 'v':
          مرتب_سازی_opt = مرتب_سازی_نسخه;
          اكسر;

        حالة 'w':
          عرض_opt = decode_سطر_درازی (optarg);
          إذا (عرض_opt < 0)
            error (LS_FAILURE, 0, "%s: %s", _("invalid line width"),
                   quote (optarg));
          اكسر;

        حالة 'x':
          format_opt = horizontal;
          اكسر;

        حالة 'A':
          ignore_نما = IGNORE_DOT_AND_DOTDOT;
          اكسر;

        حالة 'B':
          جمع_کن_ignore_الگو ("*~");
          جمع_کن_ignore_الگو (".*~");
          اكسر;

        حالة 'C':
          format_opt = many_در_هر_سطر;
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
              i = XARGMATCH ("--classify", optarg, when_آرگومانها, when_types);
            وإلا
              /* Using --classify with no argument is equivalent to using
                 --classify=always.  */
              i = when_always;

            إذا (i == when_always || (i == when_if_tty && stdout_isatty ()))
              indicator_style = classify;
            اكسر;
          }

        حالة 'G':		/* inhibit display of group info */
          print_گروه = false;
          اكسر;

        حالة 'H':
          dereference = DEREF_COMMAND_سطر_ARGUMENTS;
          اكسر;

        حالة DEREFERENCE_COMMAND_سطر_SYMLINK_مقصد_DIR_OPTION:
          dereference = DEREF_COMMAND_سطر_SYMLINK_مقصد_DIR;
          اكسر;

        حالة 'I':
          جمع_کن_ignore_الگو (optarg);
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
          مرتب_سازی_opt = مرتب_سازی_اندازه;
          اكسر;

        حالة 'T':
          tabsize_opt = xnumtoumax (optarg, 0, 0, MIN (PTRDIFF_MAX, SIZE_MAX),
                                    "", _("invalid tab size"), LS_FAILURE, 0);
          اكسر;

        حالة 'U':
          مرتب_سازی_opt = مرتب_سازی_none;
          اكسر;

        حالة 'X':
          مرتب_سازی_opt = مرتب_سازی_extension;
          اكسر;

        حالة '1':
          /* -1 has no effect after -l.  */
          إذا (format_opt != long_format)
            format_opt = one_در_هر_سطر;
          اكسر;

        حالة AUTHOR_OPTION:
          print_author = true;
          اكسر;

        حالة HIDE_OPTION:
          {
            بنية ignore_الگو *hide = xmalloc (حجم *hide);
            hide->الگو_2 = optarg;
            hide->بعدی = hide_patterns;
            hide_patterns = hide;
          }
          اكسر;

        حالة مرتب_سازی_OPTION:
          مرتب_سازی_opt = XARGMATCH ("--sort", optarg, مرتب_سازی_آرگومانها, مرتب_سازی_types);
          اكسر;

        حالة گروه_DIRECTORIES_اولی_OPTION:
          directories_اولی = true;
          اكسر;

        حالة زمان_OPTION:
          زمان_type = XARGMATCH ("--time", optarg, زمان_آرگومانها, زمان_types);
          explicit_زمان = true;
          اكسر;

        حالة FORMAT_OPTION:
          format_opt = XARGMATCH ("--format", optarg, format_آرگومانها,
                                  format_types);
          اكسر;

        حالة پر_زمان_OPTION:
          format_opt = long_format;
          زمان_style_option = (حرف *) "full-iso";
          اكسر;

        حالة COLOR_OPTION:
          {
            صحيح i;
            إذا (optarg)
              i = XARGMATCH ("--color", optarg, when_آرگومانها, when_types);
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
              i = XARGMATCH ("--hyperlink", optarg, when_آرگومانها, when_types);
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
                                       indicator_style_آرگومانها,
                                       indicator_style_types);
          اكسر;

        حالة QUOTING_STYLE_OPTION:
          quoting_style_opt = XARGMATCH ("--quoting-style", optarg,
                                         quoting_style_آرگومانها,
                                         quoting_style_vals);
          اكسر;

        حالة زمان_STYLE_OPTION:
          زمان_style_option = optarg;
          اكسر;

        حالة SHOW_CONTROL_CHARS_OPTION:
          hide_control_chars_opt = false;
          اكسر;

        حالة بلوک_اندازه_OPTION:
          {
            تعداد strtol_error e = human_options (optarg, &human_output_opts,
                                                 &output_بلوک_اندازه);
            إذا (e != LONGINT_OK)
              xstrtol_fatal (e, oi, 0, long_options, optarg);
            file_human_output_opts = human_output_opts;
            file_output_بلوک_اندازه = output_بلوک_اندازه;
          }
          اكسر;

        حالة SI_OPTION:
          file_human_output_opts = human_output_opts =
            human_autoscale | human_SI;
          file_output_بلوک_اندازه = output_بلوک_اندازه = 1;
          اكسر;

        حالة 'Z':
          print_scontext = true;
          اكسر;

        حالة ZERO_OPTION:
          eolbyte = 0;
          hide_control_chars_opt = false;
          إذا (format_opt != long_format)
            format_opt = one_در_هر_سطر;
          print_with_color = false;
          quoting_style_opt = literal_quoting_style;
          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخه_CHAR (برنامه_NAME, AUTHORS);

        افتراضي:
          usage (LS_FAILURE);
        }
    }

  إذا (! output_بلوک_اندازه)
    {
      حرف ثابت *ls_بلوک_اندازه = getenv ("LS_BLOCK_SIZE");
      human_options (ls_بلوک_اندازه,
                     &human_output_opts, &output_بلوک_اندازه);
      إذا (ls_بلوک_اندازه || getenv ("BLOCK_SIZE"))
        {
          file_human_output_opts = human_output_opts;
          file_output_بلوک_اندازه = output_بلوک_اندازه;
        }
      إذا (kibibytes_specified)
        {
          human_output_opts = 0;
          output_بلوک_اندازه = 1024;
        }
    }

  format = (0 <= format_opt ? format_opt
            : ls_نما == LS_LS ? (stdout_isatty ()
                                  ? many_در_هر_سطر : one_در_هر_سطر)
            : ls_نما == LS_MULTI_ستون ? many_در_هر_سطر
            : /* ls_mode == LS_LONG_FORMAT */ long_format);

  /* If the line length was not set by a switch but is needed to determine
     output, go to the work of obtaining it from the environment.  */
  ptrdiff_t linelen = عرض_opt;
  إذا (format == many_در_هر_سطر || format == horizontal || format == with_commas
      || print_with_color)
    {
#اگر_تعریف_شده TIOCGWINSZ
      إذا (linelen < 0)
        {
          بنية winsize ws;
          إذا (stdout_isatty ()
              && 0 <= ioctl (STDOUT_FILENO, TIOCGWINSZ, &ws)
              && 0 < ws.ws_ستون)
            linelen = ws.ws_ستون <= MIN (PTRDIFF_MAX, SIZE_MAX) ? ws.ws_ستون : 0;
        }
#پایان_اگر
      إذا (linelen < 0)
        {
          حرف ثابت *p = getenv ("COLUMNS");
          إذا (p && *p)
            {
              linelen = decode_سطر_درازی (p);
              إذا (linelen < 0)
                error (0, 0,
                       _("ignoring invalid width"
                         " in environment variable COLUMNS: %s"),
                       quote (p));
            }
        }
    }

  سطر_درازی = linelen < 0 ? 80 : linelen;

  /* Determine the max possible number of display columns.  */
  max_idx = سطر_درازی / MIN_ستونی_عرض;
  /* Account for first display column not having a separator,
     or line_lengths shorter than MIN_COLUMN_WIDTH.  */
  max_idx += سطر_درازی % MIN_ستونی_عرض != 0;

  إذا (format == many_در_هر_سطر || format == horizontal || format == with_commas)
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
                       ? ls_نما == LS_LS && stdout_isatty ()
                       : hide_control_chars_opt);

  qmark_newline = hide_control_chars_opt != false && eolbyte == '\n'
                  && ((سطر_درازی
                       && (format == with_commas
                           || format == many_در_هر_سطر || format == horizontal))
                      || format == one_در_هر_سطر || format == long_format);

  صحيح qs = quoting_style_opt;
  إذا (qs < 0)
    qs = getenv_quoting_style ();
  إذا (qs < 0)
    qs = (ls_نما == LS_LS
          ? (stdout_isatty () ? shell_escape_quoting_style : -1)
          : escape_quoting_style);
  إذا (0 <= qs)
    مجموعه_quoting_style (عدم, qs);
  qs = get_quoting_style (عدم);
  align_variable_outer_quotes
    = ((format == long_format
        || ((format == many_در_هر_سطر || format == horizontal) && سطر_درازی))
       && (qs == shell_quoting_style
           || qs == shell_escape_quoting_style
           || qs == c_maybe_quoting_style));
  filename_quoting_options = clone_quoting_options (عدم);
  إذا (qs == escape_quoting_style)
    مجموعه_char_quoting (filename_quoting_options, ' ', 1);
  إذا (file_type <= indicator_style)
    {
      لكل (حرف ثابت *p = &"*=>@|"[indicator_style - file_type]; *p; p++)
        مجموعه_char_quoting (filename_quoting_options, *p, 1);
    }
  إذا (format == with_commas
      && (qs == shell_quoting_style
          || qs == shell_escape_quoting_style
          || qs == c_maybe_quoting_style
          || qs == escape_quoting_style))
    مجموعه_char_quoting (filename_quoting_options, ',', 1);

  dirname_quoting_options = clone_quoting_options (عدم);
  مجموعه_char_quoting (dirname_quoting_options, ':', 1);

  /* --dired implies --format=long (-l) and sans --hyperlink.
     So ignore it if those overridden.  */
  dired &= (format == long_format) & !print_hyperlink;

  إذا (eolbyte < dired)
    error (LS_FAILURE, 0, _("--dired and --zero are incompatible"));

  /* If a time type is explicitly specified (with -c, -u, or --time=)
     and we're not showing a time (-l not specified), then sort by that time,
     rather than by name.  Note this behavior is unspecified by POSIX.  */

  مرتب_سازی_type = (0 <= مرتب_سازی_opt ? مرتب_سازی_opt
               : (format != long_format && explicit_زمان)
               ? مرتب_سازی_زمان : مرتب_سازی_name);

  إذا (format == long_format)
    {
      حرف *envstyle = عدم;
      حرف *style = زمان_style_option;
      إذا (! style)
        style = envstyle = getenv ("TIME_STYLE");

      إذا (! style)
        style = (حرف *) "locale";
      وإلا
        {
          ساكن حرف ثابت posix_پیشوند[] = "posix-";
          طالما (STREQ_LEN (style, posix_پیشوند, حجم posix_پیشوند - 1))
            {
              إذا (! hard_locale (LC_زمان))
                أرجع optind;
              style += حجم posix_پیشوند - 1;
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
          long_زمان_format[0] = p0;
          long_زمان_format[1] = p1;
        }
      وإلا
        {
          بدل (x_timestyle_تطابق (style, /*allow_posix=*/ true,
                                     زمان_style_آرگومانها,
                                     (حرف ثابت *) زمان_style_types,
                                     حجم (*زمان_style_types), LS_FAILURE))
            {
            حالة پر_iso_زمان_style:
              long_زمان_format[0] = long_زمان_format[1] =
                "%Y-%m-%d %H:%M:%S.%N %z";
              اكسر;

            حالة long_iso_زمان_style:
              long_زمان_format[0] = long_زمان_format[1] = "%Y-%m-%d %H:%M";
              اكسر;

            حالة iso_زمان_style:
              long_زمان_format[0] = "%Y-%m-%d ";
              long_زمان_format[1] = "%m-%d %H:%M";
              اكسر;

            حالة locale_زمان_style:
              إذا (hard_locale (LC_زمان))
                {
                  لكل (صحيح i = 0; i < 2; i++)
                    long_زمان_format[i] =
                      dcgettext (عدم, long_زمان_format[i], LC_زمان);
                }
            }
        }

      abformat_init ();
    }

  أرجع optind;
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
get_funky_string (حرف **dest, حرف ثابت **src, bool equals_انتها,
                  size_t *output_شمار)
{
  حرف رقم;			/* For numerical codes */
  size_t شمار;			/* Something to count with */
  تعداد {
    ST_GND, ST_BACKSLASH, ST_OCTAL, ST_HEX, ST_CARET, ST_انتها, ST_ERROR
  } state;
  حرف ثابت *p;
  حرف *q;

  p = *src;			/* We don't want to double-indirect */
  q = *dest;			/* the whole darn time.  */

  شمار = 0;			/* No characters counted in yet.  */
  رقم = 0;

  state = ST_GND;		/* Start in ground state.  */
  طالما (state < ST_انتها)
    {
      بدل (state)
        {
        حالة ST_GND:		/* Ground state (no escapes) */
          بدل (*p)
            {
            حالة ':':
            حالة '\0':
              state = ST_انتها;	/* End of string */
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
              إذا (equals_انتها)
                {
                  state = ST_انتها; /*انتها*/
                  اكسر;
                }
              FALLTHROUGH;
            افتراضي:
              *(q++) = *(p++);
              ++شمار;
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
              ++شمار;
              state = ST_GND;
            }
          ++p;
          اكسر;

        حالة ST_OCTAL:		/* Octal sequence */
          إذا (*p < '0' || *p > '7')
            {
              *(q++) = رقم;
              ++شمار;
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
              ++شمار;
              state = ST_GND;
              اكسر;
            }
          اكسر;

        حالة ST_CARET:		/* Caret escape */
          state = ST_GND;	/* Should be the next state... */
          إذا (*p >= '@' && *p <= '~')
            {
              *(q++) = *(p++) & 037;
              ++شمار;
            }
          وإلا إذا (*p == '?')
            {
              *(q++) = 127;
              ++شمار;
            }
          وإلا
            state = ST_ERROR;
          اكسر;

        حالة ST_انتها: حالة ST_ERROR: افتراضي:
          affirm (false);
        }
    }

  *dest = q;
  *src = p;
  *output_شمار = شمار;

  أرجع state != ST_ERROR;
}

تعداد parse_state
  {
    PS_شروع = 1,
    PS_2,
    PS_3,
    PS_4,
    PS_انجامشده,
    PS_FAIL
  };


/* Check if the content of TERM is a valid name in dircolors.  */

ساكن bool
معلوم_جمله_type (فراغ)
{
  حرف ثابت *جمله = getenv ("TERM");
  إذا (! جمله || ! *جمله)
    أرجع false;

  حرف ثابت *سطر = G_سطر;
  طالما (سطر - G_سطر < حجم (G_سطر))
    {
      إذا (STRNCMP_LIT (سطر, "TERM ") == 0)
        {
          إذا (fnmatch (سطر + 5, جمله, 0) == 0)
            أرجع true;
        }
      سطر += طول_رشته (سطر) + 1;
    }

  أرجع false;
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
      إذا (! (colorterm && *colorterm) && ! معلوم_جمله_type ())
        print_with_color = false;
      أرجع;
    }

  ext = عدم;

  /* This is an overly conservative estimate, but any possible
     LS_COLORS string will *not* generate a color_buf longer than
     itself, so it is a safe way of allocating a buffer in
     advance.  */
  buf = color_buf = xstrdup (p);

  تعداد parse_state state = PS_شروع;
  طالما (true)
    {
      بدل (state)
        {
        حالة PS_شروع:		/* First label character */
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
              ext->بعدی = color_ext_list;
              color_ext_list = ext;
              ext->exact_تطابق = false;

              ++p;
              ext->ext.string = buf;

              state = (get_funky_string (&buf, &p, true, &ext->ext.len)
                       ? PS_4 : PS_FAIL);
              اكسر;

            حالة '\0':
              state = PS_انجامشده;	/* Done! */
              اذهب انجامشده;

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
                               ? PS_شروع : PS_FAIL);
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
                       ? PS_شروع : PS_FAIL);
            }
          وإلا
            state = PS_FAIL;
          اكسر;

        حالة PS_FAIL:
          اذهب انجامشده;

        حالة PS_انجامشده: افتراضي:
          affirm (false);
        }
    }
 انجامشده:

  إذا (state == PS_FAIL)
    {
      error (0, 0,
             _("unparsable value for LS_COLORS environment variable"));
      حرر (color_buf);
      لكل (بنية color_ext_type *e = color_ext_list; e != عدم;
           /*خالی*/)
        {
          بنية color_ext_type *e2 = e;
          e = e->بعدی;
          حرر (e2);
        }
      print_with_color = false;
      أرجع;
    }
  وإلا
    {
      /* Postprocess list to set EXACT_MATCH on entries where there are
         different cased extensions with separate sequences defined.
         Also set ext.len to SIZE_MAX on any entries that can't
         match due to precedence, to avoid redundant string compares.  */
      لكل (بنية color_ext_type *e1 = color_ext_list; e1 != عدم;
           e1 = e1->بعدی)
        {
          bool case_ignored = false;

          لكل (بنية color_ext_type *e2 = e1->بعدی; e2 != عدم;
               e2 = e2->بعدی)
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
                          e1->exact_تطابق = true;
                          e2->exact_تطابق = true;
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
مجموعه_exit_وضعیت (bool serious)
{
  إذا (serious)
    exit_وضعیت = LS_FAILURE;
  وإلا إذا (exit_وضعیت == EXIT_SUCCESS)
    exit_وضعیت = LS_MINOR_PROBLEM;
}

/* Assuming a failure is serious if SERIOUS, use the printf-style
   MESSAGE to report the failure to access a file named FILE.  Assume
   errno is set appropriately for the failure.  */

ساكن فراغ
file_failure (bool serious, حرف ثابت *پیام_2, حرف ثابت *file)
{
  error (0, errno, پیام_2, quoteaf (file));
  مجموعه_exit_وضعیت (serious);
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
صف_directory (حرف ثابت *name, حرف ثابت *realname, bool command_سطر_arg)
{
  بنية pending *new = xmalloc (حجم *new);
  new->realname = realname ? xstrdup (realname) : عدم;
  new->name = name ? xstrdup (name) : عدم;
  new->command_سطر_arg = command_سطر_arg;
  new->بعدی = pending_dirs;
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
  بنية dirent *بعدی;
  uintmax_t مجموع_بلوکها = 0;
  ساكن bool اولی = true;

  errno = 0;
  dirp = opendir (name);
  إذا (!dirp)
    {
      إذا (command_سطر_arg || ! ignorable_traversal_errno (errno))
        file_failure (command_سطر_arg, _("cannot open directory %s"), name);
      أرجع;
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
          أرجع;
        }

      /* If we've already visited this dev/inode pair, warn that
         we've found a loop, and do not process this directory.  */
      إذا (بازدید_dir (dir_stat.st_dev, dir_stat.st_ino))
        {
          error (0, 0, _("%s: not listing already-listed directory"),
                 quotef (name));
          closedir (dirp);
          مجموعه_exit_وضعیت (true);
          أرجع;
        }

      dev_ino_افزودن (dir_stat.st_dev, dir_stat.st_ino);
    }

  clear_files ();

  إذا (recursive || print_dir_name)
    {
      إذا (!اولی)
        dired_outbyte ('\n');
      اولی = false;
      dired_indent ();

      حرف *absolute_name = عدم;
      إذا (print_hyperlink)
        {
          absolute_name = canonicalize_filename_نما (name, CAN_MISSING);
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

  طالما (true)
    {
      /* Set errno to zero so we can distinguish between a readdir failure
         and when readdir simply finds that there are no more entries.  */
      errno = 0;
      بعدی = readdir (dirp);
      إذا (بعدی)
        {
          إذا (! file_ignored (بعدی->d_name))
            {
              تعداد filetype type;
#إذا HAVE_STRUCT_DIRENT_D_TYPE
              type = d_type_filetype[بعدی->d_type];
#وإلا
              type = unknown;
#پایان_اگر
              مجموع_بلوکها += gobble_file (بعدی->d_name, type,
                                           RELIABLE_D_INO (بعدی),
                                           false, name);

              /* In this narrow case, print out each name right away, so
                 ls uses constant memory while processing the entries of
                 this directory.  Useful when there are many (millions)
                 of entries in a directory.  */
              إذا (format == one_در_هر_سطر && مرتب_سازی_type == مرتب_سازی_none
                      && !print_بلوک_اندازه && !recursive)
                {
                  /* We must call sort_files in spite of
                     "sort_type == sort_none" for its initialization
                     of the sorted_file vector.  */
                  مرتب_سازی_files ();
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
      process_سیگنالها ();
    }

  إذا (closedir (dirp) != 0)
    {
      file_failure (command_سطر_arg, _("closing directory %s"), name);
      /* Don't return; print whatever we got.  */
    }

  /* Sort the directory contents.  */
  مرتب_سازی_files ();

  /* If any member files are subdirectories, perhaps they should have their
     contents listed rather than being mentioned here as files.  */

  إذا (recursive)
    استخراج_dirs_from_files (name, false);

  إذا (format == long_format || print_بلوک_اندازه)
    {
      حرف buf[LONGEST_HUMAN_READABLE + 3];
      حرف *p = human_readable (مجموع_بلوکها, buf + 1, human_output_opts,
                                ST_NBLOCKSIZE, output_بلوک_اندازه);
      حرف *pend = p + طول_رشته (p);
      *--p = ' ';
      *pend++ = eolbyte;
      dired_indent ();
      dired_outstring (_("total"));
      dired_outbuf (p, pend - p);
    }

  إذا (cwd_n_استفاده_شده)
    print_current_files ();
}

/* Add 'pattern' to the list of patterns for which files that match are
   not listed.  */

ساكن فراغ
جمع_کن_ignore_الگو (حرف ثابت *الگو_2)
{
  بنية ignore_الگو *ignore;

  ignore = xmalloc (حجم *ignore);
  ignore->الگو_2 = الگو_2;
  /* Add it to the head of the linked list.  */
  ignore->بعدی = ignore_patterns;
  ignore_patterns = ignore;
}

/* Return true if one of the PATTERNS matches FILE.  */

ساكن bool
patterns_تطابق (بنية ignore_الگو ثابت *patterns, حرف ثابت *file)
{
  لكل (بنية ignore_الگو ثابت *p = patterns; p; p = p->بعدی)
    إذا (fnmatch (p->الگو_2, file, FNM_PERIOD) == 0)
      أرجع true;
  أرجع false;
}

/* Return true if FILE should be ignored.  */

ساكن bool
file_ignored (حرف ثابت *name)
{
  أرجع ((ignore_نما != IGNORE_کمینه
           && name[0] == '.'
           && (ignore_نما == IGNORE_DEFAULT || ! name[1 + (name[1] == '.')]))
          || (ignore_نما == IGNORE_DEFAULT
              && patterns_تطابق (hide_patterns, name))
          || patterns_تطابق (ignore_patterns, name));
}

/* POSIX requires that a file size be printed without a sign, even
   when negative.  Assume the typical case where negative sizes are
   actually positive values that have wrapped around.  */

ساكن uintmax_t
unsigned_file_اندازه (off_t اندازه)
{
  أرجع اندازه + (اندازه < 0) * ((uintmax_t) OFF_T_MAX - OFF_T_MIN + 1);
}

#اگر_تعریف_شده HAVE_CAP
/* Return true if NAME has a capability (see linux/capability.h) */
ساكن bool
has_capability (حرف ثابت *name)
{
  حرف *نتیجه;
  bool has_cap;

  cap_t cap_d = cap_get_file (name);
  إذا (cap_d == عدم)
    أرجع false;

  نتیجه = cap_مقصد_متن (cap_d, عدم);
  cap_free (cap_d);
  إذا (!نتیجه)
    أرجع false;

  /* check if human-readable capability string is empty */
  has_cap = !!*نتیجه;

  cap_free (نتیجه);
  أرجع has_cap;
}
#وإلا
ساكن bool
has_capability (MAYBE_استفادهنشده حرف ثابت *name)
{
  errno = ENOTSUP;
  أرجع false;
}
#پایان_اگر

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
  لكل (idx_t i = 0; i < cwd_n_استفاده_شده; i++)
    {
      بنية fileinfo *f = sorted_file[i];
      free_ent (f);
    }

  cwd_n_استفاده_شده = 0;
  cwd_some_quoted = false;
  any_has_acl = false;
  inode_عدد_عرض = 0;
  بلوک_اندازه_عرض = 0;
  nlink_عرض = 0;
  owner_عرض = 0;
  گروه_عرض = 0;
  author_عرض = 0;
  scontext_عرض = 0;
  major_device_عدد_عرض = 0;
  minor_device_عدد_عرض = 0;
  file_اندازه_عرض = 0;
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
      ai->اندازه = 0;
      ai->scontext = unsupported_scontext;
      ai->scontext_err = unsupported_scontext_err;
      errno = ENOTSUP;
      أرجع unsupported_return;
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
          || (ai->اندازه < 0 && !acl_errno_valid (ai->u.err)))
      && (!(flags & ACL_GET_SCONTEXT) || !acl_errno_valid (ai->scontext_err)))
    {
      /* FIXME: acl_errno_valid() may be a bit too broad determine file system
         non support.  E.g. EBUSY was added for per file reasons (on cygwin). */
      unsupported_return = n;
      unsupported_scontext = ai->scontext;
      unsupported_scontext_err = ai->scontext_err;
      unsupported_device = f->stat.st_dev;
    }
  أرجع n;
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
      أرجع 0;
    }

  bool b = has_capability (file);
  إذا (f->stat_ok && !b && !acl_errno_valid (errno))
    {
      unsupported_cached = true;
      unsupported_device = f->stat.st_dev;
    }
  أرجع b;
}

/* Get the ACL, security context, and capability information needed for F.
   FULL_NAME names the file; DO_DEREF says whether to follow symlinks.  */
ساكن فراغ
get_extended_info (حرف ثابت *پر_name, بنية fileinfo *f, bool do_deref)
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
#پایان_اگر
  bool check_capability = format_needs_capability & (type == نرمال);

  إذا (type != arg_directory && (check_acl | get_scontext | check_capability))
    {
      بنية aclinfo ai;
      /* Without ACL support, request contexts to obtain the attribute
         names needed for capability coloring too.  */
      صحيح aclinfo_flags = ((do_deref ? ACL_SYMLINK_FOLLOW : 0)
                           | (get_scontext || (!USE_ACL && check_capability)
                              ? ACL_GET_SCONTEXT : 0)
                           | filetype_d_type[type]);
      صحيح n = file_has_aclinfo_cache (پر_name, f, &ai, aclinfo_flags);
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
        error (0, errno, "%s", quotef (پر_name));
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
            error (0, ai.scontext_err, "%s", quotef (پر_name));
        }

      /* has_capability adds around 30% runtime to 'ls --color',
         so call it only if really needed.  Note capability coloring
         is disabled in the default color config.  */
      إذا (check_capability && aclinfo_has_xattr (&ai, XATTR_NAME_CAPS))
        f->has_capability = has_capability_cache (پر_name, f);

      f->scontext = ai.scontext;
      ai.scontext = عدم;
      aclinfo_free (&ai);
    }
}

ساكن bool
needs_quoting (حرف ثابت *name)
{
  حرف test[2];
  size_t len = quotearg_میانگیر (test, حجم test , name, -1,
                                filename_quoting_options);
  أرجع *name != *test || طول_رشته (name) != len;
}

/* Add a file to the current table of files.
   Verify that the file exists, and print an error message if it does not.
   Return the number of blocks that the file occupies.  */
ساكن uintmax_t
gobble_file (حرف ثابت *name, تعداد filetype type, ino_t inode,
             bool command_سطر_arg, حرف ثابت *dirname)
{
  uintmax_t بلوکها = 0;
  بنية fileinfo *f;

  /* An inode value prior to gobble_file necessarily came from readdir,
     which is not used for command line arguments.  */
  affirm (! command_سطر_arg || inode == NOT_AN_INODE_عدد);

  إذا (cwd_n_استفاده_شده == cwd_n_alloc)
    cwd_file = xpalloc (cwd_file, &cwd_n_alloc, 1, -1, حجم *cwd_file);

  f = &cwd_file[cwd_n_استفاده_شده];
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
              || color_symlink_as_referent || check_symlink_نما))
      /* Command line dereferences are already taken care of by the above
         assertion that the inode number is not yet known.  */
      || (print_inode && inode == NOT_AN_INODE_عدد)
      /* --indicator-style=classify (aka -F) requires statting each
         regular file to see whether it's executable.  */
      || ((type == نرمال || type == unknown)
          && (indicator_style == classify
              /* This is so that --color ends up highlighting files with these
                 mode bits set even when options like -F are not specified.  */
              || (print_with_color && (is_colored (C_EXEC)
                                       || is_colored (C_SETUID)
                                       || is_colored (C_SETGID))))));

  /* Absolute name of this file, if needed.  */
  حرف ثابت *پر_name = name;
  إذا (check_stat | print_scontext | format_needs_capability
      && name[0] != '/' && dirname)
    {
      حرف *p = alloca (طول_رشته (name) + طول_رشته (dirname) + 2);
      attach (p, dirname, name);
      پر_name = p;
    }

  bool do_deref;

  إذا (!check_stat)
    do_deref = dereference == DEREF_ALWAYS;
  وإلا
    {
      صحيح err;

      إذا (print_hyperlink)
        {
          f->absolute_name = canonicalize_filename_نما (پر_name,
                                                         CAN_MISSING);
          إذا (! f->absolute_name
              && (command_سطر_arg
                  || ! ignorable_traversal_errno (errno)))
            file_failure (command_سطر_arg,
                          _("error canonicalizing %s"), پر_name);
        }

      بدل (dereference)
        {
        حالة DEREF_ALWAYS:
          err = do_stat (پر_name, &f->stat);
          do_deref = true;
          اكسر;

        حالة DEREF_COMMAND_سطر_ARGUMENTS:
        حالة DEREF_COMMAND_سطر_SYMLINK_مقصد_DIR:
          إذا (command_سطر_arg)
            {
              bool need_lstat;
              err = do_stat (پر_name, &f->stat);
              do_deref = true;

              إذا (dereference == DEREF_COMMAND_سطر_ARGUMENTS)
                اكسر;

              need_lstat = (err < 0
                            ? (errno == ENOENT || errno == ELOOP)
                            : ! S_ISDIR (f->stat.st_نما));
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
          err = do_lstat (پر_name, &f->stat);
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
                                    && do_lstat (پر_name, &linkstat) == 0);
              errno = stat_errno;
              إذا (! dangling_link)
                {
                  free_ent (f);
                  أرجع 0;
                }
            }

          /* Failure to stat a command line argument leads to
             an exit status of 2.  For other files, stat failure
             provokes an exit status of 1.  */
          file_failure (command_سطر_arg,
                        _("cannot access %s"), پر_name);

          إذا (command_سطر_arg)
            {
              free_ent (f);
              أرجع 0;
            }

          f->name = xstrdup (name);
          cwd_n_استفاده_شده++;

          أرجع 0;
        }

      f->stat_ok = true;
      f->filetype = type = d_type_filetype[IFTODT (f->stat.st_نما)];
    }

  إذا (type == directory && command_سطر_arg && !فوری_dirs)
    f->filetype = type = arg_directory;

  get_extended_info (پر_name, f, do_deref);

  إذا ((type == symbolic_link)
      & ((format == long_format) | check_symlink_نما))
    {
      بنية stat linkstats;

      إذا (! get_link_name (پر_name, f, command_سطر_arg))
        {
          free_ent (f);
          أرجع 0;
        }

      /* Use the slower quoting path for this entry, though
         don't update CWD_SOME_QUOTED since alignment not affected.  */
      إذا (f->linkname && f->quoted == 0 && needs_quoting (f->linkname))
        f->quoted = -1;

      /* Avoid following symbolic links when possible, i.e., when
         they won't be traced and when no indicator is needed.  */
      إذا (f->linkname
          && (file_type <= indicator_style || check_symlink_نما)
          && stat_for_نما (پر_name, &linkstats) == 0)
        {
          f->linkok = true;
          f->linkmode = linkstats.st_نما;
        }
    }

  بلوکها = STP_NBLOCKS (&f->stat);
  إذا (format == long_format || print_بلوک_اندازه)
    {
      حرف buf[LONGEST_HUMAN_READABLE + 1];
      صحيح len = mbswidth (human_readable (بلوکها, buf, human_output_opts,
                                          ST_NBLOCKSIZE, output_بلوک_اندازه),
                          MBSWIDTH_FLAGS);
      إذا (بلوک_اندازه_عرض < len)
        بلوک_اندازه_عرض = len;
    }

  إذا (format == long_format)
    {
      إذا (print_owner)
        {
          صحيح len = format_user_عرض (f->stat.st_uid);
          إذا (owner_عرض < len)
            owner_عرض = len;
        }

      إذا (print_گروه)
        {
          صحيح len = format_گروه_عرض (f->stat.st_gid);
          إذا (گروه_عرض < len)
            گروه_عرض = len;
        }

      إذا (print_author)
        {
          صحيح len = format_user_عرض (f->stat.st_author);
          إذا (author_عرض < len)
            author_عرض = len;
        }
    }

  إذا (print_scontext)
    {
      صحيح len = طول_رشته (f->scontext);
      إذا (scontext_عرض < len)
        scontext_عرض = len;
    }

  إذا (format == long_format)
    {
      حرف b[INT_BUFSIZE_BOUND (uintmax_t)];
      صحيح b_len = طول_رشته (umaxtostr (f->stat.st_nlink, b));
      إذا (nlink_عرض < b_len)
        nlink_عرض = b_len;

      إذا ((type == chardev) | (type == blockdev))
        {
          حرف buf[INT_BUFSIZE_BOUND (uintmax_t)];
          صحيح len = طول_رشته (umaxtostr (major (f->stat.st_rdev), buf));
          إذا (major_device_عدد_عرض < len)
            major_device_عدد_عرض = len;
          len = طول_رشته (umaxtostr (minor (f->stat.st_rdev), buf));
          إذا (minor_device_عدد_عرض < len)
            minor_device_عدد_عرض = len;
          len = major_device_عدد_عرض + 2 + minor_device_عدد_عرض;
          إذا (file_اندازه_عرض < len)
            file_اندازه_عرض = len;
        }
      وإلا
        {
          حرف buf[LONGEST_HUMAN_READABLE + 1];
          uintmax_t اندازه = unsigned_file_اندازه (f->stat.st_اندازه);
          صحيح len = mbswidth (human_readable (اندازه, buf,
                                              file_human_output_opts,
                                              1, file_output_بلوک_اندازه),
                              MBSWIDTH_FLAGS);
          إذا (file_اندازه_عرض < len)
            file_اندازه_عرض = len;
        }
    }

  إذا (print_inode)
    {
      حرف buf[INT_BUFSIZE_BOUND (uintmax_t)];
      صحيح len = طول_رشته (umaxtostr (f->stat.st_ino, buf));
      إذا (inode_عدد_عرض < len)
        inode_عدد_عرض = len;
    }

  f->name = xstrdup (name);
  cwd_n_استفاده_شده++;

  أرجع بلوکها;
}

/* Return true if F refers to a directory.  */
ساكن bool
is_directory (ثابت بنية fileinfo *f)
{
  أرجع f->filetype == directory || f->filetype == arg_directory;
}

/* Return true if F refers to a (symlinked) directory.  */
ساكن bool
is_linked_directory (ثابت بنية fileinfo *f)
{
  أرجع f->filetype == directory || f->filetype == arg_directory
         || S_ISDIR (f->linkmode);
}

/* Put the name of the file that FILENAME is a symbolic link to
   into the LINKNAME field of 'f'.  COMMAND_LINE_ARG indicates whether
   FILENAME is a command-line argument.  */

ساكن bool
get_link_name (حرف ثابت *filename, بنية fileinfo *f, bool command_سطر_arg)
{
  f->linkname = areadlink_with_اندازه (filename, f->stat.st_اندازه);
  إذا (f->linkname == عدم)
    {
      إذا (! command_سطر_arg
          && ignorable_traversal_errno (errno))
        أرجع false;
      file_failure (command_سطر_arg, _("cannot read symbolic link %s"),
                    filename);
    }
  أرجع true;
}

/* Return true if the last component of NAME is '.' or '..'
   This is so we don't try to recurse on '././././. ...' */

ساكن bool
basename_is_dot_or_dotdot (حرف ثابت *name)
{
  حرف ثابت *پایه_2 = آخرین_component (name);
  أرجع dot_or_dotdot (پایه_2);
}

/* Remove any entries from CWD_FILE that are for directories,
   and queue them to be listed as directories instead.
   DIRNAME is the prefix to prepend to each dirname
   to make it correct relative to ls's working dir;
   if it is null, no prefix is needed and "." and ".." should not be ignored.
   If COMMAND_LINE_ARG is true, this directory was mentioned at the top level,
   This is desirable when processing directories recursively.  */

ساكن فراغ
استخراج_dirs_from_files (حرف ثابت *dirname, bool command_سطر_arg)
{
  idx_t i, j;
  bool ignore_dot_and_dot_dot = (dirname != عدم);

  إذا (dirname && LOOP_DETECT)
    {
      /* Insert a marker entry first.  When we dequeue this marker entry,
         we'll know that DIRNAME has been processed and may be removed
         from the set of active directories.  */
      صف_directory (عدم, dirname, false);
    }

  /* Queue the directories last one first, because queueing reverses the
     order.  */
  لكل (i = cwd_n_استفاده_شده; 0 < i; )
    {
      i--;
      بنية fileinfo *f = sorted_file[i];

      إذا (is_directory (f)
          && (! ignore_dot_and_dot_dot
              || ! basename_is_dot_or_dotdot (f->name)))
        {
          إذا (!dirname || f->name[0] == '/')
            صف_directory (f->name, f->linkname, command_سطر_arg);
          وإلا
            {
              حرف *name = file_name_concat (dirname, f->name, عدم);
              صف_directory (name, f->linkname, command_سطر_arg);
              حرر (name);
            }
          إذا (f->filetype == arg_directory)
            free_ent (f);
        }
    }

  /* Now delete the directories from the table, compacting all the remaining
     entries.  */

  لكل (i = 0, j = 0; i < cwd_n_استفاده_شده; i++)
    {
      بنية fileinfo *f = sorted_file[i];
      sorted_file[j] = f;
      j += (f->filetype != arg_directory);
    }
  cwd_n_استفاده_شده = j;
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
      مجموعه_exit_وضعیت (false);
      longjmp (failed_strcoll, 1);
    }
  أرجع diff;
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
  أرجع diff ? diff : cmp (a, b);
}

/* Define the 8 different sort function variants required for each sortkey.
   KEY_NAME is a token describing the sort key, e.g., ctime, atime, size.
   KEY_CMP_FUNC is a function to compare records based on that key, e.g.,
   ctime_cmp, atime_cmp, size_cmp.  Append KEY_NAME to the string,
   '[rev_][x]str{cmp|coll}[_df]_', to create each function name.  */
#تعریف_کن DEFINE_مرتب_سازی_FUNCTIONS(کلید_name, کلید_cmp_func)			\
  /* direct, non-dirfirst versions */					\
  ساكن صحيح xstrcoll_##key_name (V a, V b)				\
  { أرجع کلید_cmp_func (a, b, xstrcoll); }				\
  ATTRIBUTE_خالص ساكن صحيح strcmp_##key_name (V a, V b)		\
  { أرجع کلید_cmp_func (a, b, strcmp); }				\
                                                                        \
  /* reverse, non-dirfirst versions */					\
  ساكن صحيح rev_xstrcoll_##key_name (V a, V b)				\
  { أرجع کلید_cmp_func (b, a, xstrcoll); }				\
  ATTRIBUTE_خالص ساكن صحيح rev_strcmp_##key_name (V a, V b)	\
  { أرجع کلید_cmp_func (b, a, strcmp); }				\
                                                                        \
  /* direct, dirfirst versions */					\
  ساكن صحيح xstrcoll_df_##key_name (V a, V b)				\
  { أرجع dirfirst_check (a, b, xstrcoll_##key_name); }		\
  ATTRIBUTE_خالص ساكن صحيح strcmp_df_##key_name (V a, V b)		\
  { أرجع dirfirst_check (a, b, strcmp_##key_name); }			\
                                                                        \
  /* reverse, dirfirst versions */					\
  ساكن صحيح rev_xstrcoll_df_##key_name (V a, V b)			\
  { أرجع dirfirst_check (a, b, rev_xstrcoll_##key_name); }		\
  ATTRIBUTE_خالص ساكن صحيح rev_strcmp_df_##key_name (V a, V b)	\
  { أرجع dirfirst_check (a, b, rev_strcmp_##key_name); }

ساكن صحيح
cmp_ctime (بنية fileinfo ثابت *a, بنية fileinfo ثابت *b,
           صحيح (*cmp) (حرف ثابت *, حرف ثابت *))
{
  صحيح diff = timespec_cmp (get_stat_ctime (&b->stat),
                           get_stat_ctime (&a->stat));
  أرجع diff ? diff : cmp (a->name, b->name);
}

ساكن صحيح
cmp_mtime (بنية fileinfo ثابت *a, بنية fileinfo ثابت *b,
           صحيح (*cmp) (حرف ثابت *, حرف ثابت *))
{
  صحيح diff = timespec_cmp (get_stat_mtime (&b->stat),
                           get_stat_mtime (&a->stat));
  أرجع diff ? diff : cmp (a->name, b->name);
}

ساكن صحيح
cmp_atime (بنية fileinfo ثابت *a, بنية fileinfo ثابت *b,
           صحيح (*cmp) (حرف ثابت *, حرف ثابت *))
{
  صحيح diff = timespec_cmp (get_stat_atime (&b->stat),
                           get_stat_atime (&a->stat));
  أرجع diff ? diff : cmp (a->name, b->name);
}

ساكن صحيح
cmp_btime (بنية fileinfo ثابت *a, بنية fileinfo ثابت *b,
           صحيح (*cmp) (حرف ثابت *, حرف ثابت *))
{
  صحيح diff = timespec_cmp (get_stat_btime (&b->stat),
                           get_stat_btime (&a->stat));
  أرجع diff ? diff : cmp (a->name, b->name);
}

ساكن صحيح
off_cmp (off_t a, off_t b)
{
  أرجع _GL_CMP (a, b);
}

ساكن صحيح
cmp_اندازه (بنية fileinfo ثابت *a, بنية fileinfo ثابت *b,
          صحيح (*cmp) (حرف ثابت *, حرف ثابت *))
{
  صحيح diff = off_cmp (b->stat.st_اندازه, a->stat.st_اندازه);
  أرجع diff ? diff : cmp (a->name, b->name);
}

ساكن صحيح
cmp_name (بنية fileinfo ثابت *a, بنية fileinfo ثابت *b,
          صحيح (*cmp) (حرف ثابت *, حرف ثابت *))
{
  أرجع cmp (a->name, b->name);
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
  أرجع diff ? diff : cmp (a->name, b->name);
}

/* Return the (cached) screen width,
   for the NAME associated with the passed fileinfo F.  */

ساكن size_t
fileinfo_name_عرض (بنية fileinfo ثابت *f)
{
  أرجع f->عرض
         ? f->عرض
         : quote_name_عرض (f->name, filename_quoting_options, f->quoted);
}

ساكن صحيح
cmp_عرض (بنية fileinfo ثابت *a, بنية fileinfo ثابت *b,
          صحيح (*cmp) (حرف ثابت *, حرف ثابت *))
{
  صحيح diff = fileinfo_name_عرض (a) - fileinfo_name_عرض (b);
  أرجع diff ? diff : cmp (a->name, b->name);
}

DEFINE_مرتب_سازی_FUNCTIONS (ctime, cmp_ctime)
DEFINE_مرتب_سازی_FUNCTIONS (mtime, cmp_mtime)
DEFINE_مرتب_سازی_FUNCTIONS (atime, cmp_atime)
DEFINE_مرتب_سازی_FUNCTIONS (btime, cmp_btime)
DEFINE_مرتب_سازی_FUNCTIONS (اندازه, cmp_اندازه)
DEFINE_مرتب_سازی_FUNCTIONS (name, cmp_name)
DEFINE_مرتب_سازی_FUNCTIONS (extension, cmp_extension)
DEFINE_مرتب_سازی_FUNCTIONS (عرض, cmp_عرض)

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
cmp_نسخه (بنية fileinfo ثابت *a, بنية fileinfo ثابت *b)
{
  صحيح diff = filevercmp (a->name, b->name);
  أرجع diff ? diff : strcmp (a->name, b->name);
}

ساكن صحيح
xstrcoll_نسخه (V a, V b)
{
  أرجع cmp_نسخه (a, b);
}
ساكن صحيح
rev_xstrcoll_نسخه (V a, V b)
{
  أرجع cmp_نسخه (b, a);
}
ساكن صحيح
xstrcoll_df_نسخه (V a, V b)
{
  أرجع dirfirst_check (a, b, xstrcoll_نسخه);
}
ساكن صحيح
rev_xstrcoll_df_نسخه (V a, V b)
{
  أرجع dirfirst_check (a, b, rev_xstrcoll_نسخه);
}


/* We have 2^3 different variants for each sort-key function
   (for 3 independent sort modes).
   The function pointers stored in this array must be dereferenced as:

    sort_variants[sort_key][use_strcmp][reverse][dirs_first]

   Note that the order in which sort keys are listed in the function pointer
   array below is defined by the order of the elements in the time_type and
   sort_type enums!  */

#تعریف_کن LIST_SORTFUNCTION_VARIANTS(کلید_name)                        \
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

ساكن qsortFunc ثابت مرتب_سازی_functions[][2][2][2] =
  {
    LIST_SORTFUNCTION_VARIANTS (name),
    LIST_SORTFUNCTION_VARIANTS (extension),
    LIST_SORTFUNCTION_VARIANTS (عرض),
    LIST_SORTFUNCTION_VARIANTS (اندازه),

    {
      {
        { xstrcoll_نسخه, xstrcoll_df_نسخه },
        { rev_xstrcoll_نسخه, rev_xstrcoll_df_نسخه },
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
static_assert (countof (مرتب_سازی_functions) == مرتب_سازی_numtypes - 2 + زمان_numtypes);

/* Set up SORTED_FILE to point to the in-use entries in CWD_FILE, in order.  */

ساكن فراغ
initialize_ordering_وکتور (فراغ)
{
  لكل (idx_t i = 0; i < cwd_n_استفاده_شده; i++)
    sorted_file[i] = &cwd_file[i];
}

/* Cache values based on attributes global to all files.  */

ساكن فراغ
update_current_files_info (فراغ)
{
  /* Cache screen width of name, if needed multiple times.  */
  إذا (مرتب_سازی_type == مرتب_سازی_عرض
      || (سطر_درازی && (format == many_در_هر_سطر || format == horizontal)))
    {
      لكل (idx_t i = 0; i < cwd_n_استفاده_شده; i++)
        {
          بنية fileinfo *f = sorted_file[i];
          f->عرض = fileinfo_name_عرض (f);
        }
    }
}

/* Sort the files now in the table.  */

ساكن فراغ
مرتب_سازی_files (فراغ)
{
  bool use_strcmp;

  إذا (sorted_file_alloc < cwd_n_استفاده_شده + (cwd_n_استفاده_شده >> 1))
    {
      حرر (sorted_file);
      sorted_file = xinmalloc (cwd_n_استفاده_شده, 3 * حجم *sorted_file);
      sorted_file_alloc = 3 * cwd_n_استفاده_شده;
    }

  initialize_ordering_وکتور ();

  update_current_files_info ();

  إذا (مرتب_سازی_type == مرتب_سازی_none)
    أرجع;

  /* Try strcoll.  If it fails, fall back on strcmp.  We can't safely
     ignore strcoll failures, as a failing strcoll might be a
     comparison function that is not a total order, and if we ignored
     the failure this might cause qsort to dump core.  */

  إذا (! setjmp (failed_strcoll))
    use_strcmp = false;      /* strcoll() succeeded */
  وإلا
    {
      use_strcmp = true;
      affirm (مرتب_سازی_type != مرتب_سازی_نسخه);
      initialize_ordering_وکتور ();
    }

  /* When sort_type == sort_time, use time_type as subindex.  */
  mpsort ((فراغ ثابت **) sorted_file, cwd_n_استفاده_شده,
          مرتب_سازی_functions[مرتب_سازی_type + (مرتب_سازی_type == مرتب_سازی_زمان ? زمان_type : 0)]
                        [use_strcmp][مرتب_سازی_معکوس]
                        [directories_اولی]);
}

/* List all the files now in the table.  */

ساكن فراغ
print_current_files (فراغ)
{
  بدل (format)
    {
    حالة one_در_هر_سطر:
      لكل (idx_t i = 0; i < cwd_n_استفاده_شده; i++)
        {
          print_file_name_and_frills (sorted_file[i], 0);
          حرف_چاپ_کن (eolbyte);
        }
      اكسر;

    حالة many_در_هر_سطر:
      إذا (! سطر_درازی)
        print_with_separator (' ');
      وإلا
        print_many_در_هر_سطر ();
      اكسر;

    حالة horizontal:
      إذا (! سطر_درازی)
        print_with_separator (' ');
      وإلا
        print_horizontal ();
      اكسر;

    حالة with_commas:
      print_with_separator (',');
      اكسر;

    حالة long_format:
      لكل (idx_t i = 0; i < cwd_n_استفاده_شده; i++)
        {
          مجموعه_نرمال_color ();
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
align_nstrftime (حرف *buf, size_t اندازه, bool recent, بنية tm ثابت *tm,
                 timezone_t tz, صحيح ns)
{
  حرف ثابت *nfmt = (use_abformat
                      ? abformat[recent][tm->tm_mon]
                      : long_زمان_format[recent]);
  أرجع nstrftime (buf, اندازه, nfmt, tm, tz, ns);
}

/* Return the expected number of columns in a long-format timestamp,
   or zero if it cannot be calculated.  */

ساكن صحيح
long_زمان_expected_عرض (فراغ)
{
  ساكن صحيح عرض = -1;

  إذا (عرض < 0)
    {
      time_t epoch = 0;
      بنية tm tm;
      حرف buf[زمان_STAMP_LEN_بیشینه + 1];

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
            عرض = mbsnwidth (buf, len, MBSWIDTH_FLAGS);
        }

      إذا (عرض < 0)
        عرض = 0;
    }

  أرجع عرض;
}

/* Print the user or group name NAME, with numeric id ID, using a
   print width of WIDTH columns.  */

ساكن فراغ
format_user_or_گروه (حرف ثابت *name, uintmax_t id, صحيح عرض)
{
  إذا (name)
    {
      صحيح name_عرض = mbswidth (name, MBSWIDTH_FLAGS);
      صحيح عرض_gap = name_عرض < 0 ? 0 : عرض - name_عرض;
      صحيح pad = MAX (0, عرض_gap);
      dired_outstring (name);

      افعل
        dired_outbyte (' ');
      طالما (pad--);
    }
  وإلا
    dired_pos += چاپ_کن ("%*ju ", عرض, id);
}

/* Print the name or id of the user with id U, using a print width of
   WIDTH.  */

ساكن فراغ
format_user (uid_t u, صحيح عرض, bool stat_ok)
{
  format_user_or_گروه (! stat_ok ? "?" :
                        (numeric_ids ? عدم : getuser (u)), u, عرض);
}

/* Likewise, for groups.  */

ساكن فراغ
format_گروه (gid_t g, صحيح عرض, bool stat_ok)
{
  format_user_or_گروه (! stat_ok ? "?" :
                        (numeric_ids ? عدم : getgroup (g)), g, عرض);
}

/* Return the number of columns that format_user_or_group will print,
   or -1 if unknown.  */

ساكن صحيح
format_user_or_گروه_عرض (حرف ثابت *name, uintmax_t id)
{
  أرجع (name
          ? mbswidth (name, MBSWIDTH_FLAGS)
          : snprintf (عدم, 0, "%ju", id));
}

/* Return the number of columns that format_user will print,
   or -1 if unknown.  */

ساكن صحيح
format_user_عرض (uid_t u)
{
  أرجع format_user_or_گروه_عرض (numeric_ids ? عدم : getuser (u), u);
}

/* Likewise, for groups.  */

ساكن صحيح
format_گروه_عرض (gid_t g)
{
  أرجع format_user_or_گروه_عرض (numeric_ids ? عدم : getgroup (g), g);
}

/* Return a pointer to a formatted version of F->stat.st_ino,
   possibly using buffer, which must be at least
   INT_BUFSIZE_BOUND (uintmax_t) bytes.  */
ساكن حرف *
format_inode (حرف buf[INT_BUFSIZE_BOUND (uintmax_t)],
              ثابت بنية fileinfo *f)
{
  أرجع (f->stat_ok && f->stat.st_ino != NOT_AN_INODE_عدد
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
     + زمان_STAMP_LEN_بیشینه + 1	/* max length of time/date */
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

  بدل (زمان_type)
    {
    حالة زمان_ctime:
      when_timespec = get_stat_ctime (&f->stat);
      اكسر;
    حالة زمان_mtime:
      when_timespec = get_stat_mtime (&f->stat);
      اكسر;
    حالة زمان_atime:
      when_timespec = get_stat_atime (&f->stat);
      اكسر;
    حالة زمان_btime:
      when_timespec = get_stat_btime (&f->stat);
      إذا (when_timespec.tv_sec == -1 && when_timespec.tv_nsec == -1)
        btime_ok = false;
      اكسر;
    حالة زمان_numtypes: افتراضي:
      unreachable ();
    }

  p = buf;

  إذا (print_inode)
    {
      حرف hbuf[INT_BUFSIZE_BOUND (uintmax_t)];
      p += sprintf (p, "%*s ", inode_عدد_عرض, format_inode (hbuf, f));
    }

  إذا (print_بلوک_اندازه)
    {
      حرف hbuf[LONGEST_HUMAN_READABLE + 1];
      حرف ثابت *بلوکها =
        (! f->stat_ok
         ? "?"
         : human_readable (STP_NBLOCKS (&f->stat), hbuf, human_output_opts,
                           ST_NBLOCKSIZE, output_بلوک_اندازه));
      صحيح بلوکها_عرض = mbswidth (بلوکها, MBSWIDTH_FLAGS);
      لكل (صحيح pad = بلوکها_عرض < 0 ? 0 : بلوک_اندازه_عرض - بلوکها_عرض;
           0 < pad; pad--)
        *p++ = ' ';
      طالما ((*p++ = *بلوکها++))
        استمر;
      p[-1] = ' ';
    }

  /* The last byte of the mode string is the POSIX
     "optional alternate access method flag".  */
  {
    حرف hbuf[INT_BUFSIZE_BOUND (uintmax_t)];
    p += sprintf (p, "%s %*s ", modebuf, nlink_عرض,
                  ! f->stat_ok ? "?" : umaxtostr (f->stat.st_nlink, hbuf));
  }

  dired_indent ();

  إذا (print_owner || print_گروه || print_author || print_scontext)
    {
      dired_outbuf (buf, p - buf);

      إذا (print_owner)
        format_user (f->stat.st_uid, owner_عرض, f->stat_ok);

      إذا (print_گروه)
        format_گروه (f->stat.st_gid, گروه_عرض, f->stat_ok);

      إذا (print_author)
        format_user (f->stat.st_author, author_عرض, f->stat_ok);

      إذا (print_scontext)
        format_user_or_گروه (f->scontext, 0, scontext_عرض);

      p = buf;
    }

  إذا (f->stat_ok
      && (S_ISCHR (f->stat.st_نما) || S_ISBLK (f->stat.st_نما)))
    {
      صحيح blanks_عرض = (file_اندازه_عرض
                          - (major_device_عدد_عرض + 2
                             + minor_device_عدد_عرض));
      p += sprintf (p, "%*ju, %*ju ",
                    major_device_عدد_عرض + MAX (0, blanks_عرض),
                    (uintmax_t) major (f->stat.st_rdev),
                    minor_device_عدد_عرض,
                    (uintmax_t) minor (f->stat.st_rdev));
    }
  وإلا
    {
      حرف hbuf[LONGEST_HUMAN_READABLE + 1];
      حرف ثابت *اندازه =
        (! f->stat_ok
         ? "?"
         : human_readable (unsigned_file_اندازه (f->stat.st_اندازه),
                           hbuf, file_human_output_opts, 1,
                           file_output_بلوک_اندازه));
      صحيح اندازه_عرض = mbswidth (اندازه, MBSWIDTH_FLAGS);
      لكل (صحيح pad = اندازه_عرض < 0 ? 0 : file_اندازه_عرض - اندازه_عرض;
           0 < pad; pad--)
        *p++ = ' ';
      طالما ((*p++ = *اندازه++))
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
      إذا (timespec_cmp (current_زمان, when_timespec) < 0)
        gettime (&current_زمان);

      /* Consider a time to be recent if it is within the past six months.
         A Gregorian year has 365.2425 * 24 * 60 * 60 == 31556952 seconds
         on the average.  Write this value as an integer constant to
         avoid floating point hassles.  */
      six_months_ago.tv_sec = current_زمان.tv_sec - 31556952 / 2;
      six_months_ago.tv_nsec = current_زمان.tv_nsec;

      recent = (timespec_cmp (six_months_ago, when_timespec) < 0
                && timespec_cmp (when_timespec, current_زمان) < 0);

      /* We assume here that all time zones are offset from UTC by a
         whole number of seconds.  */
      s = align_nstrftime (p, زمان_STAMP_LEN_بیشینه + 1, recent,
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
      p += sprintf (p, "%*s ", long_زمان_expected_عرض (),
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
    print_type_indicator (f->stat_ok, f->stat.st_نما, f->filetype);
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
                صحيح needs_general_quoting, size_t *عرض, bool *pad)
{
  حرف *buf = *inbuf;
  size_t displayed_عرض IF_LINT ( = 0);
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
      len = quotearg_میانگیر (buf, bufsize, name, -1, options);
      إذا (bufsize <= len)
        {
          buf = xmalloc (len + 1);
          quotearg_میانگیر (buf, len + 1, name, -1, options);
        }

      quoted = (*name != *buf) || طول_رشته (name) != len;
    }
  وإلا إذا (needs_further_quoting || extra_newline_protection)
    {
      len = طول_رشته (name);
      إذا (bufsize <= len)
        buf = xmalloc (len + 1);
      کپی_حافظه (buf, name, len + 1);

      quoted = false;
    }
  وإلا
    {
      len = طول_رشته (name);
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
          displayed_عرض = 0;

          طالما (p < plimit)
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
                  displayed_عرض += 1;
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
                        size_t بایتها;
                        صحيح w;

                        بایتها = mbrtoc32 (&wc, p, plimit - p, &mbstate);

                        إذا (بایتها == (size_t) -1)
                          {
                            /* An invalid multibyte sequence was
                               encountered.  Skip one input byte, and
                               put a question mark.  */
                            p++;
                            *q++ = '?';
                            displayed_عرض += 1;
                            اكسر;
                          }

                        إذا (بایتها == (size_t) -2)
                          {
                            /* An incomplete multibyte character
                               at the end.  Replace it entirely with
                               a question mark.  */
                            p = plimit;
                            *q++ = '?';
                            displayed_عرض += 1;
                            اكسر;
                          }

                        إذا (بایتها == 0)
                          /* A null wide character was encountered.  */
                          بایتها = 1;

                        w = c32width (wc);
                        إذا (w >= 0)
                          {
                            /* A printable multibyte character.
                               Keep it.  */
                            لكل (; بایتها > 0; --بایتها)
                              *q++ = *p++;
                            displayed_عرض += w;
                          }
                        وإلا
                          {
                            /* An nonprintable multibyte character.
                               Replace it entirely with a question
                               mark.  */
                            p += بایتها;
                            *q++ = '?';
                            displayed_عرض += 1;
                          }
                      }
                    طالما (! mbsinit (&mbstate));
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

          طالما (p < plimit)
            {
              إذا (! isprint (مقصد_uchar (*p)))
                *p = '?';
              p++;
            }
          displayed_عرض = len;
        }
    }
  وإلا إذا (عرض != عدم)
    {
      إذا (MB_CUR_MAX > 1)
        {
          displayed_عرض = mbsnwidth (buf, len, MBSWIDTH_FLAGS);
          displayed_عرض = MAX (0, displayed_عرض);
        }
      وإلا
        {
          حرف ثابت *p = buf;
          حرف ثابت *plimit = buf + len;

          displayed_عرض = 0;
          طالما (p < plimit)
            {
              إذا (isprint (مقصد_uchar (*p)))
                displayed_عرض++;
              p++;
            }
        }
    }

  /* Set padding to better align quoted items,
     and also give a visual indication that quotes are
     not actually part of the name.  */
  *pad = (align_variable_outer_quotes && cwd_some_quoted && ! quoted);

  إذا (عرض != عدم)
    *عرض = displayed_عرض;

  *inbuf = buf;

  أرجع len;
}

ساكن size_t
quote_name_عرض (حرف ثابت *name, بنية quoting_options ثابت *options,
                  صحيح needs_general_quoting)
{
  حرف smallbuf[BUFSIZ];
  حرف *buf = smallbuf;
  size_t عرض;
  bool pad;

  quote_name_buf (&buf, حجم smallbuf, (حرف *) name, options,
                  needs_general_quoting, &عرض, &pad);

  إذا (buf != smallbuf && buf != name)
    حرر (buf);

  عرض += pad;

  أرجع عرض;
}

/* %XX escape any input out of range as defined in RFC3986,
   and also if PATH, convert all path separators to '/'.  */
ساكن حرف *
file_escape (حرف ثابت *str, bool مسیر)
{
  حرف *esc = xnmalloc (3, طول_رشته (str) + 1);
  حرف *p = esc;
  طالما (*str)
    {
      إذا (مسیر && ISSLASH (*str))
        {
          *p++ = '/';
          str++;
        }
      وإلا إذا (RFC3986[مقصد_uchar (*str)])
        *p++ = *str++;
      وإلا
        p += sprintf (p, "%%%02x", مقصد_uchar (*str++));
    }
  *p = '\0';
  أرجع esc;
}

ساكن size_t
quote_name (حرف ثابت *name, بنية quoting_options ثابت *options,
            صحيح needs_general_quoting, ثابت بنية bin_str *color,
            bool allow_pad, بنية obstack *پشته, حرف ثابت *absolute_name)
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
          حرف_چاپ_کن (*buf);
        }
      حرف *h = file_escape (hostname, /* path= */ false);
      حرف *n = file_escape (absolute_name, /* path= */ true);
      /* TODO: It would be good to be able to define parameters
         to give hints to the terminal as how best to render the URI.
         For example since ls is outputting a dense block of URIs
         it would be best to not underline by default, and only
         do so upon hover etc.  */
      چاپ_کن ("\033]8;;file://%s%s%s\033\\", h, *n == '/' ? "" : "/", n);
      حرر (h);
      حرر (n);
    }

  إذا (پشته)
    افزودن_current_dired_pos (پشته);

  fwrite (buf + skip_quotes, 1, len - (skip_quotes * 2), stdout);

  dired_pos += len;

  إذا (پشته)
    افزودن_current_dired_pos (پشته);

  إذا (absolute_name)
    {
      fputs ("\033]8;;\033\\", stdout);
      إذا (skip_quotes)
        حرف_چاپ_کن (*(buf + len - 1));
    }

  إذا (buf != smallbuf && buf != name)
    حرر (buf);

  أرجع len + pad;
}

ساكن size_t
print_name_with_quoting (ثابت بنية fileinfo *f,
                         bool symlink_هدف,
                         بنية obstack *پشته,
                         size_t شروع_ستون)
{
  حرف ثابت *name = symlink_هدف ? f->linkname : f->name;

  ثابت بنية bin_str *color
    = print_with_color ? get_color_indicator (f, symlink_هدف) : عدم;

  bool استفاده_شده_color_this_زمان = (print_with_color
                               && (color || is_colored (C_NORM)));

  size_t len = quote_name (name, filename_quoting_options, f->quoted,
                           color, !symlink_هدف, پشته, f->absolute_name);

  process_سیگنالها ();
  إذا (استفاده_شده_color_this_زمان)
    {
      prep_non_filename_متن ();

      /* We use the byte length rather than display width here as
         an optimization to avoid accurately calculating the width,
         because we only output the clear to EOL sequence if the name
         _might_ wrap to the next line.  This may output a sequence
         unnecessarily in multi-byte locales for example,
         but in that case it's inconsequential to the output.  */
      إذا (سطر_درازی
          && (شروع_ستون / سطر_درازی != (شروع_ستون + len - 1) / سطر_درازی))
        put_indicator (&color_indicator[C_CLR_مقصد_EOL]);
    }

  أرجع len;
}

ساكن فراغ
prep_non_filename_متن (فراغ)
{
  إذا (color_indicator[C_انتها].string != عدم)
    put_indicator (&color_indicator[C_انتها]);
  وإلا
    {
      put_indicator (&color_indicator[C_چپ]);
      put_indicator (&color_indicator[C_RESET]);
      put_indicator (&color_indicator[C_راست]);
    }
}

/* Print the file name of 'f' with appropriate quoting.
   Also print file size, inode number, and filetype indicator character,
   as requested by switches.  */

ساكن size_t
print_file_name_and_frills (ثابت بنية fileinfo *f, size_t شروع_ستون)
{
  حرف buf[MAX (LONGEST_HUMAN_READABLE + 1, INT_BUFSIZE_BOUND (uintmax_t))];

  مجموعه_نرمال_color ();

  إذا (print_inode)
    چاپ_کن ("%*s ", format == with_commas ? 0 : inode_عدد_عرض,
            format_inode (buf, f));

  إذا (print_بلوک_اندازه)
    {
      حرف ثابت *بلوکها =
        (! f->stat_ok
         ? "?"
         : human_readable (STP_NBLOCKS (&f->stat), buf, human_output_opts,
                           ST_NBLOCKSIZE, output_بلوک_اندازه));
      صحيح بلوکها_عرض = mbswidth (بلوکها, MBSWIDTH_FLAGS);
      صحيح pad = 0;
      إذا (0 <= بلوکها_عرض && بلوک_اندازه_عرض && format != with_commas)
        pad = بلوک_اندازه_عرض - بلوکها_عرض;
      چاپ_کن ("%*s%s ", pad, "", بلوکها);
    }

  إذا (print_scontext)
    چاپ_کن ("%*s ", format == with_commas ? 0 : scontext_عرض, f->scontext);

  size_t عرض = print_name_with_quoting (f, false, عدم, شروع_ستون);

  إذا (indicator_style != none)
    عرض += print_type_indicator (f->stat_ok, f->stat.st_نما, f->filetype);

  أرجع عرض;
}

/* Given these arguments describing a file, return the single-byte
   type indicator, or 0.  */
ساكن حرف
get_type_indicator (bool stat_ok, mode_t نما_2, تعداد filetype type)
{
  حرف c;

  إذا (stat_ok ? S_ISREG (نما_2) : type == نرمال)
    {
      إذا (stat_ok && indicator_style == classify && (نما_2 & S_IXUGO))
        c = '*';
      وإلا
        c = 0;
    }
  وإلا
    {
      إذا (stat_ok ? S_ISDIR (نما_2) : type == directory || type == arg_directory)
        c = '/';
      وإلا إذا (indicator_style == slash)
        c = 0;
      وإلا إذا (stat_ok ? S_ISLNK (نما_2) : type == symbolic_link)
        c = '@';
      وإلا إذا (stat_ok ? S_ISFIFO (نما_2) : type == fifo)
        c = '|';
      وإلا إذا (stat_ok ? S_ISSOCK (نما_2) : type == sock)
        c = '=';
      وإلا إذا (stat_ok && S_ISDOOR (نما_2))
        c = '>';
      وإلا
        c = 0;
    }
  أرجع c;
}

ساكن bool
print_type_indicator (bool stat_ok, mode_t نما_2, تعداد filetype type)
{
  حرف c = get_type_indicator (stat_ok, نما_2, type);
  إذا (c)
    dired_outbyte (c);
  أرجع !!c;
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
      put_indicator (&color_indicator[C_چپ]);
      put_indicator (ind);
      put_indicator (&color_indicator[C_راست]);
    }

  أرجع ind != عدم;
}

/* Returns color indicator or NULL if none.  */
ATTRIBUTE_خالص
ساكن ثابت بنية bin_str *
get_color_indicator (ثابت بنية fileinfo *f, bool symlink_هدف)
{
  تعداد indicator_no type;
  بنية color_ext_type *ext;	/* Color extension */
  size_t len;			/* Length of name */

  حرف ثابت *name;
  mode_t نما_2;
  صحيح linkok;
  إذا (symlink_هدف)
    {
      name = f->linkname;
      نما_2 = f->linkmode;
      linkok = f->linkok ? 0 : -1;
    }
  وإلا
    {
      name = f->name;
      نما_2 = file_or_link_نما (f);
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
      إذا (S_ISREG (نما_2))
        {
          type = C_FILE;

          إذا ((نما_2 & S_ISUID) != 0 && is_colored (C_SETUID))
            type = C_SETUID;
          وإلا إذا ((نما_2 & S_ISGID) != 0 && is_colored (C_SETGID))
            type = C_SETGID;
          وإلا إذا (f->has_capability)
            type = C_CAP;
          وإلا إذا ((نما_2 & S_IXUGO) != 0 && is_colored (C_EXEC))
            type = C_EXEC;
          وإلا إذا ((1 < f->stat.st_nlink) && is_colored (C_MULTIHARDLINK))
            type = C_MULTIHARDLINK;
        }
      وإلا إذا (S_ISDIR (نما_2))
        {
          type = C_DIR;

          إذا ((نما_2 & S_ISVTX) && (نما_2 & S_IWOTH)
              && is_colored (C_STICKY_OTHER_WRITABLE))
            type = C_STICKY_OTHER_WRITABLE;
          وإلا إذا ((نما_2 & S_IWOTH) != 0 && is_colored (C_OTHER_WRITABLE))
            type = C_OTHER_WRITABLE;
          وإلا إذا ((نما_2 & S_ISVTX) != 0 && is_colored (C_STICKY))
            type = C_STICKY;
        }
      وإلا إذا (S_ISLNK (نما_2))
        type = C_LINK;
      وإلا إذا (S_ISFIFO (نما_2))
        type = C_FIFO;
      وإلا إذا (S_ISSOCK (نما_2))
        type = C_SOCK;
      وإلا إذا (S_ISBLK (نما_2))
        type = C_BLK;
      وإلا إذا (S_ISCHR (نما_2))
        type = C_CHR;
      وإلا إذا (S_ISDOOR (نما_2))
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

      len = طول_رشته (name);
      name += len;		/* Pointer to final \0.  */
      لكل (ext = color_ext_list; ext != عدم; ext = ext->بعدی)
        {
          إذا (ext->ext.len <= len)
            {
              إذا (ext->exact_تطابق)
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

  أرجع s->string ? s : عدم;
}

/* Output a color indicator (which may contain nulls).
   Note we don't update dired_pos here as emacs can be configured
   to handle color codes by stripping/converting them.  */
ساكن فراغ
put_indicator (ثابت بنية bin_str *ind)
{
  إذا (! استفاده_شده_color)
    {
      استفاده_شده_color = true;

      /* If the standard output is a controlling terminal, watch out
         for signals, so that the colors can be restored to the
         default state if "ls" is suspended or interrupted.  */

      إذا (0 <= tcgetpgrp (STDOUT_FILENO))
        signal_init ();

      prep_non_filename_متن ();
    }

  fwrite (ind->string, ind->len, 1, stdout);
}

ساكن size_t
درازی_از_file_name_and_frills (ثابت بنية fileinfo *f)
{
  size_t len = 0;
  حرف buf[MAX (LONGEST_HUMAN_READABLE + 1, INT_BUFSIZE_BOUND (uintmax_t))];

  إذا (print_inode)
    len += 1 + (format == with_commas
                ? طول_رشته (umaxtostr (f->stat.st_ino, buf))
                : inode_عدد_عرض);

  إذا (print_بلوک_اندازه)
    len += 1 + (format == with_commas
                ? طول_رشته (! f->stat_ok ? "?"
                          : human_readable (STP_NBLOCKS (&f->stat), buf,
                                            human_output_opts, ST_NBLOCKSIZE,
                                            output_بلوک_اندازه))
                : بلوک_اندازه_عرض);

  إذا (print_scontext)
    len += 1 + (format == with_commas ? طول_رشته (f->scontext) : scontext_عرض);

  len += fileinfo_name_عرض (f);

  إذا (indicator_style != none)
    {
      حرف c = get_type_indicator (f->stat_ok, f->stat.st_نما, f->filetype);
      len += (c != 0);
    }

  أرجع len;
}

ساكن فراغ
print_many_در_هر_سطر (فراغ)
{
  idx_t تعداد_ستون_2 = calculate_columns (true);
  بنية ستونی_info ثابت *سطر_fmt = &ستونی_info[تعداد_ستون_2 - 1];

  /* Calculate the number of rows that will be in each column except possibly
     for a short column on the right.  */
  idx_t ردیفها = cwd_n_استفاده_شده / تعداد_ستون_2 + (cwd_n_استفاده_شده % تعداد_ستون_2 != 0);

  لكل (idx_t ردیف = 0; ردیف < ردیفها; ردیف++)
    {
      size_t ستون = 0;
      idx_t filesno = ردیف;
      size_t pos = 0;

      /* Print the next row.  */
      طالما (true)
        {
          بنية fileinfo ثابت *f = sorted_file[filesno];
          size_t name_درازی = درازی_از_file_name_and_frills (f);
          size_t max_name_درازی = سطر_fmt->ستون_arr[ستون++];
          print_file_name_and_frills (f, pos);

          إذا (cwd_n_استفاده_شده - ردیفها <= filesno)
            اكسر;
          filesno += ردیفها;

          indent (pos + name_درازی, pos + max_name_درازی);
          pos += max_name_درازی;
        }
      حرف_چاپ_کن (eolbyte);
    }
}

ساكن فراغ
print_horizontal (فراغ)
{
  size_t pos = 0;
  idx_t تعداد_ستون_2 = calculate_columns (false);
  بنية ستونی_info ثابت *سطر_fmt = &ستونی_info[تعداد_ستون_2 - 1];
  بنية fileinfo ثابت *f = sorted_file[0];
  size_t name_درازی = درازی_از_file_name_and_frills (f);
  size_t max_name_درازی = سطر_fmt->ستون_arr[0];

  /* Print first entry.  */
  print_file_name_and_frills (f, 0);

  /* Now the rest.  */
  لكل (idx_t filesno = 1; filesno < cwd_n_استفاده_شده; filesno++)
    {
      idx_t ستون = filesno % تعداد_ستون_2;

      إذا (ستون == 0)
        {
          حرف_چاپ_کن (eolbyte);
          pos = 0;
        }
      وإلا
        {
          indent (pos + name_درازی, pos + max_name_درازی);
          pos += max_name_درازی;
        }

      f = sorted_file[filesno];
      print_file_name_and_frills (f, pos);

      name_درازی = درازی_از_file_name_and_frills (f);
      max_name_درازی = سطر_fmt->ستون_arr[ستون];
    }
  حرف_چاپ_کن (eolbyte);
}

/* Output name + SEP + ' '.  */

ساكن فراغ
print_with_separator (حرف sep)
{
  size_t pos = 0;

  لكل (idx_t filesno = 0; filesno < cwd_n_استفاده_شده; filesno++)
    {
      بنية fileinfo ثابت *f = sorted_file[filesno];
      size_t len = سطر_درازی ? درازی_از_file_name_and_frills (f) : 0;

      إذا (filesno != 0)
        {
          حرف separator;
          size_t بعدی_pos = 2 + (sep == ',' && filesno < cwd_n_استفاده_شده - 1);

          إذا (! سطر_درازی
              || (! ckd_جمع_کن (&بعدی_pos, بعدی_pos, pos)
                  && ! ckd_جمع_کن (&بعدی_pos, بعدی_pos, len)
                  && بعدی_pos <= سطر_درازی))
            {
              pos += 2;
              separator = ' ';
            }
          وإلا
            {
              pos = 0;
              separator = eolbyte;
            }

          حرف_چاپ_کن (sep);
          حرف_چاپ_کن (separator);
        }

      print_file_name_and_frills (f, pos);
      pos += len;
    }
  حرف_چاپ_کن (eolbyte);
}

/* Assuming cursor is at position FROM, indent up to position TO.
   Use a TAB character instead of two or more spaces whenever possible.  */

ساكن فراغ
indent (size_t from, size_t مقصد)
{
  طالما (from < مقصد)
    {
      إذا (tabsize != 0 && مقصد / tabsize > (from + 1) / tabsize)
        {
          حرف_چاپ_کن ('\t');
          from += tabsize - from % tabsize;
        }
      وإلا
        {
          حرف_چاپ_کن (' ');
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
      طالما (*dirnamep)
        *dest++ = *dirnamep++;
      /* Add '/' if 'dirname' doesn't already end with it.  */
      إذا (dirnamep > dirname && dirnamep[-1] != '/')
        *dest++ = '/';
    }
  طالما (*name)
    *dest++ = *name++;
  *dest = 0;
}

/* Allocate enough column info suitable for the current number of
   files and display columns, and initialize the info to represent the
   narrowest possible columns.  */

ساكن فراغ
init_ستونی_info (idx_t max_تعداد_ستون)
{
  /* Currently allocated columns in column_info.  */
  ساكن idx_t ستونی_info_alloc;

  إذا (ستونی_info_alloc < max_تعداد_ستون)
    {
      idx_t old_ستونی_info_alloc = ستونی_info_alloc;
      ستونی_info = xpalloc (ستونی_info, &ستونی_info_alloc,
                             max_تعداد_ستون - ستونی_info_alloc, -1,
                             حجم *ستونی_info);

      /* Allocate the new size_t objects by computing the triangle
         formula n * (n + 1) / 2, except that we don't need to
         allocate the part of the triangle that we've already
         allocated.  Check for address arithmetic overflow.  */
      idx_t ستونی_info_growth = ستونی_info_alloc - old_ستونی_info_alloc, s;
      إذا (ckd_جمع_کن (&s, old_ستونی_info_alloc + 1, ستونی_info_alloc)
          || ckd_mul (&s, s, ستونی_info_growth))
        xalloc_die ();
      size_t *p = xinmalloc (s >> 1, حجم *p);

      /* Grow the triangle by parceling out the cells just allocated.  */
      لكل (idx_t i = old_ستونی_info_alloc; i < ستونی_info_alloc; i++)
        {
          ستونی_info[i].ستون_arr = p;
          p += i + 1;
        }
    }

  لكل (idx_t i = 0; i < max_تعداد_ستون; ++i)
    {
      ستونی_info[i].valid_len = true;
      ستونی_info[i].سطر_len = i * MIN_ستونی_عرض + 1;
      لكل (idx_t j = 0; j < i; ++j)
        ستونی_info[i].ستون_arr[j] = MIN_ستونی_عرض;
      ستونی_info[i].ستون_arr[i] = 1;
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
  idx_t max_تعداد_ستون = 0 < max_idx && max_idx < cwd_n_استفاده_شده ? max_idx : cwd_n_استفاده_شده;

  init_ستونی_info (max_تعداد_ستون);

  /* Compute the maximum number of possible columns.  */
  لكل (idx_t filesno = 0; filesno < cwd_n_استفاده_شده; ++filesno)
    {
      بنية fileinfo ثابت *f = sorted_file[filesno];
      size_t name_درازی = درازی_از_file_name_and_frills (f);

      لكل (idx_t i = 0; i < max_تعداد_ستون; ++i)
        {
          إذا (ستونی_info[i].valid_len)
            {
              idx_t idx = (by_columns
                           ? filesno / ((cwd_n_استفاده_شده + i) / (i + 1))
                           : filesno % (i + 1));
              size_t real_درازی = name_درازی + (idx == i ? 0 : 2);

              إذا (ستونی_info[i].ستون_arr[idx] < real_درازی)
                {
                  ستونی_info[i].سطر_len += (real_درازی
                                              - ستونی_info[i].ستون_arr[idx]);
                  ستونی_info[i].ستون_arr[idx] = real_درازی;
                  ستونی_info[i].valid_len = (ستونی_info[i].سطر_len
                                              <= سطر_درازی);
                }
            }
        }
    }

  /* Find maximum allowed columns.  */
  idx_t تعداد_ستون_2;
  لكل (تعداد_ستون_2 = max_تعداد_ستون; 1 < تعداد_ستون_2; --تعداد_ستون_2)
    {
      إذا (ستونی_info[تعداد_ستون_2 - 1].valid_len)
        اكسر;
    }

  أرجع تعداد_ستون_2;
}

فراغ
usage (صحيح وضعیت)
{
  إذا (وضعیت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کن (_("Usage: %s [OPTION]... [FILE]...\n"), برنامه_name);
      fputs (_("\
List information about the FILEs (the current directory by افتراضي).\n\
مرتب_سازی_2 entries alphabetically إذا none از -cftuvSUX nor --مرتب_سازی is specified.\n\
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
         with -l, print the author از each file\n\
"));
      oputs (_("\
  -b, --escape\n\
         print C-style escapes لكل nongraphic characters\n\
"));
      oputs (_("\
      --بلوک-اندازه=اندازه_2\n\
         with -l, scale sizes by اندازه_2 when printing them;\n\
         e.g., '--block-size=M'; see اندازه_2 format زیر\n\
"));
      oputs (_("\
  -B, --ignore-backups\n\
         افعل not list implied entries ending with ~\n\
"));
      oputs (_("\
  -c\n\
         with -lt: مرتب_سازی by, and show, ctime\n\
           (زمان از آخرین change از file وضعیت information);\n\
         with -l: show ctime and مرتب_سازی by name;\n\
         otherwise: مرتب_سازی by ctime, newest اولی\n\
"));
      oputs (_("\
  -C\n\
         list entries by columns\n\
"));
      oputs (_("\
      --color[=WHEN]\n\
         color the output WHEN; بیشتر info زیر\n\
"));
      oputs (_("\
  -d, --directory\n\
         list directories themselves, not their contents\n\
"));
      oputs (_("\
  -D, --dired\n\
         generate output designed لكل Emacs' dired نما_2\n\
"));
      oputs (_("\
  -f\n\
         same as -a -U\n\
"));
      oputs (_("\
  -F, --classify[=WHEN]\n\
         append indicator (one از */=>@|) مقصد entries WHEN\n\
"));
      oputs (_("\
      --file-type\n\
         like -F, except افعل not append '*'\n\
"));
      oputs (_("\
      --format=کلمه\n\
         across,horizontal (-x), commas (-m), دراز (-l),\n\
         single-ستونی (-1), verbose (-l), vertical (-C)\n\
"));
      oputs (_("\
      --پر-زمان\n\
         like -l --زمان-style=پر-iso\n\
"));
      oputs (_("\
  -g\n\
         like -l, but افعل not list owner\n\
"));
      oputs (_("\
      --گروه-directories-اولی\n\
         گروه directories before files\n\
"));
      oputs (_("\
  -G, --no-گروه\n\
         درون a دراز listing, don't print گروه نامها\n\
"));
      oputs (_("\
  -h, --human-readable\n\
         with -l and -s, print sizes like 1K 234M 2G etc.\n\
"));
      oputs (_("\
      --si\n\
         likewise, but use powers از 1000 not 1024\n\
"));
      oputs (_("\
  -H, --dereference-command-سطر\n\
         follow symbolic links listed on the command سطر\n\
"));
      oputs (_("\
      --dereference-command-سطر-symlink-مقصد-dir\n\
         follow each command سطر symbolic link that points مقصد a directory\n\
"));
      oputs (_("\
      --hide=الگو\n\
         افعل not list implied entries matching shell الگو\n\
         (overridden by -a or -A)\n\
"));
      oputs (_("\
      --hyperlink[=WHEN]\n\
         hyperlink file نامها WHEN\n\
"));
      oputs (_("\
      --indicator-style=کلمه\n\
         append indicator with style کلمه مقصد مدخل نامها:\n\
           none (افتراضي), slash (-p), file-type (--file-type), classify (-F)\n\
"));
      oputs (_("\
  -i, --inode\n\
         print the index عدد از each file\n\
"));
      oputs (_("\
  -I, --ignore=الگو\n\
         افعل not list implied entries matching shell الگو\n\
"));
      oputs (_("\
  -k, --kibibytes\n\
         افتراضي مقصد 1024-byte بلوکها لكل file system usage;\n\
         استفاده_شده only with -s and در_هر directory totals\n\
"));
      oputs (_("\
  -l\n\
         use a دراز listing format\n\
"));
      oputs (_("\
  -L, --dereference\n\
         when showing file information لكل a symbolic link,\n\
         show information لكل the file the link references\n\
         rather than لكل the link itself\n\
"));
      oputs (_("\
  -m\n\
         fill عرض with a comma separated list از entries\n\
"));
      oputs (_("\
  -n, --numeric-uid-gid\n\
         like -l, but list numeric user and گروه IDs\n\
"));
      oputs (_("\
  -N, --literal\n\
         print مدخل نامها without quoting\n\
"));
      oputs (_("\
  -o\n\
         like -l, but افعل not list گروه information\n\
"));
      oputs (_("\
  -p, --indicator-style=slash\n\
         append / indicator مقصد directories\n\
"));
      oputs (_("\
  -q, --hide-control-chars\n\
         print ? instead از nongraphic characters\n\
"));
      oputs (_("\
      --show-control-chars\n\
         show nongraphic characters as-is;\n\
         the افتراضي, unless برنامه is 'ls' and output is a terminal\n\
"));
      oputs (_("\
  -Q, --quote-name\n\
         enclose مدخل نامها درون مزدوج quotes\n\
"));
      oputs (_("\
      --quoting-style=کلمه\n\
         use quoting style کلمه لكل مدخل نامها:\n\
           literal, locale, shell, shell-always,\n\
           shell-escape, shell-escape-always, c, escape\n\
         (overrides QUOTING_STYLE environment variable)\n\
"));
      oputs (_("\
  -r, --معکوس\n\
         معکوس ترتیب طالما ترتیبدهی\n\
"));
      oputs (_("\
  -R, --recursive\n\
         list subdirectories recursively\n\
"));
      oputs (_("\
  -s, --اندازه\n\
         print the allocated اندازه از each file, درون بلوکها\n\
"));
      oputs (_("\
  -S\n\
         مرتب_سازی by file اندازه, بزرگترین اولی\n\
"));
      oputs (_("\
      --مرتب_سازی=کلمه\n\
         change افتراضي 'name' مرتب_سازی مقصد کلمه:\n\
           none (-U), اندازه (-S), زمان (-t),\n\
           نسخه_3 (-v), extension (-X), name, عرض\n\
"));
      oputs (_("\
      --زمان=کلمه\n\
         select which timestamp استفاده_شده مقصد display or مرتب_سازی;\n\
           access زمان (-u): atime, access, use;\n\
           metadata change زمان (-c): ctime, وضعیت;\n\
           modified زمان (افتراضي): mtime, modification;\n\
           birth زمان: birth, creation;\n\
         with -l, کلمه determines which زمان مقصد show;\n\
         with --مرتب_سازی=زمان, مرتب_سازی by کلمه (newest اولی)\n\
"));
      oputs (_("\
      --زمان-style=زمان_STYLE\n\
         زمان/date format with -l; see زمان_STYLE زیر\n\
"));
      oputs (_("\
  -t\n\
         مرتب_سازی by زمان, newest اولی; see --زمان\n\
"));
      oputs (_("\
  -T, --tabsize=تعداد_ستون\n\
         assume tab stops at each تعداد_ستون instead از 8\n\
"));
      oputs (_("\
  -u\n\
         with -lt: مرتب_سازی by, and show, access زمان;\n\
         with -l: show access زمان and مرتب_سازی by name;\n\
         otherwise: مرتب_سازی by access زمان, newest اولی\n\
"));
      oputs (_("\
  -U\n\
         افعل not مرتب_سازی directory entries\n\
"));
      oputs (_("\
  -v\n\
         natural مرتب_سازی از (نسخه_3) numbers within متن\n\
"));
      oputs (_("\
  -w, --عرض=تعداد_ستون\n\
         مجموعه_2 output عرض مقصد تعداد_ستون.  0 means no حد\n\
"));
      oputs (_("\
  -x\n\
         list entries by سطرها instead از by columns\n\
"));
      oputs (_("\
  -X\n\
         مرتب_سازی alphabetically by مدخل extension\n\
"));
      oputs (_("\
  -Z, --context\n\
         print any security context از each file\n\
"));
      oputs (_("\
      --zero\n\
         انتها each output سطر with NUL, not newline\n\
"));
      oputs (_("\
  -1\n\
         list one file در_هر سطر\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);
      emit_اندازه_note ();
      fputs (_("\
\n\
The زمان_STYLE آرگومان can be پر-iso, دراز-iso, iso, locale, or +FORMAT.\n\
FORMAT is interpreted like درون date(1).  If FORMAT is FORMAT1<newline>FORMAT2,\n\
then FORMAT1 applies مقصد non-recent files and FORMAT2 مقصد recent files.\n\
زمان_STYLE prefixed with 'posix-' takes effect only outside the POSIX locale.\n\
Also the زمان_STYLE environment variable sets the افتراضي style مقصد use.\n\
"), stdout);
      fputs (_("\
\n\
The WHEN آرگومان defaults مقصد 'always' and can also be 'auto' or 'never'.\n\
"), stdout);
      fputs (_("\
\n\
Using color مقصد distinguish file types is disabled both by افتراضي and\n\
with --color=never.  With --color=خودکار, ls emits color codes only when\n\
standard output is connected مقصد a terminal.  The LS_COLORS environment\n\
variable can change the settings.  Use the dircolors(1) command مقصد مجموعه_2 it.\n\
"), stdout);
      fputs (_("\
\n\
Exit وضعیت:\n\
 0  إذا OK,\n\
 1  إذا minor problems (e.g., cannot access subdirectory),\n\
 2  إذا serious trouble (e.g., cannot access command-سطر آرگومان).\n\
"), stdout);
      emit_ancillary_info (برنامه_NAME);
    }
  خروج (وضعیت);
}
