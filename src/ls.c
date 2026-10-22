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
#اگر_ختم
#شامل <sys/ioctl.h>

#اگر_تعریف_شدہ WINSIZE_اندر_PTEM
# include <sys/stream.h>
# include <sys/ptem.h>
#اگر_ختم

#شامل <stdio.h>
#شامل <setjmp.h>
#شامل <pwd.h>
#شامل <getopt.h>
#شامل <signal.h>

#اگر HAVE_LANGINFO_CODESET
# include <langinfo.h>
#اگر_ختم

/* NonStop circa 2011 lacks both SA_RESTART and siginterrupt, so don't
   restart syscalls after a signal handler fires.  This may cause
   colors to get messed up on the screen if 'ls' is interrupted, but
   that's the best we can do on such a platform.  */
#اگر_غیر_تعریف_شدہ SA_RESTART
# define SA_RESTART 0
#اگر_ختم

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
#اگر_تعریف_شدہ HAVE_CAP
# include <sys/capability.h>
#اگر_ختم

#اگر HAVE_LINUX_XATTR_H
# include <linux/xattr.h>
# ifndef XATTR_NAME_CAPS
#  define XATTR_NAME_CAPS "security.capability"
# endif
#اگر_ختم

#تعریف_کرو پروگرام_NAME (ls_منوال == LS_LS ? "ls" \
                      : (ls_منوال == LS_MULTI_ستون \
                         ? "dir" : "vdir"))

#تعریف_کرو AUTHORS \
  proper_name ("Richard M. Stallman"), \
  proper_name ("David MacKenzie")

#تعریف_کرو obstack_chunk_alloc حافظہ_دو
#تعریف_کرو obstack_chunk_free آزاد

/* Unix-based readdir implementations have historically returned a dirent.d_ino
   value that is sometimes not equal to the stat-obtained st_ino value for
   that same entry.  This error occurs for a readdir entry that refers
   to a mount point.  readdir's error is to return the inode number of
   the underlying directory -- one that typically cannot be stat'ed, as
   long as a file system is mounted on that directory.  RELIABLE_D_INO
   encapsulates whether we can use the more efficient approach of relying
   on readdir-supplied d_ino values, or whether we must incur the cost of
   calling stat or lstat to obtain each guaranteed-valid inode number.  */

#اگر_غیر_تعریف_شدہ READDIR_LIES_ABOUT_MOUNTPOINT_D_INO
# define READDIR_LIES_ABOUT_MOUNTPOINT_D_INO 1
#اگر_ختم

#اگر READDIR_LIES_ABOUT_MOUNTPOINT_D_INO
# define RELIABLE_D_INO(dp) NOT_AN_INODE_نمبر
#ورنہ
# define RELIABLE_D_INO(dp) D_INO (dp)
#اگر_ختم

#اگر ! HAVE_STRUCT_STAT_ST_AUTHOR
# define st_author st_uid
#اگر_ختم

شمار filetype
  {
    unknown,
    fifo,
    chardev,
    directory,
    blockdev,
    معمول,
    symbolic_link,
    sock,
    whiteout,
    arg_directory
  };
شمار { filetype_cardinality = arg_directory + 1 };

/* Display letters and indicators for each filetype.
   Keep these in sync with enum filetype.  */
ساکن حرف ثابت filetype_letter[] =
  {'?', 'p', 'c', 'd', 'b', '-', 'l', 's', 'w', 'd'};
static_assert (countof (filetype_letter) == filetype_cardinality);

/* Map enum filetype to <dirent.h> d_type values.  */
ساکن غیرعلامتی حرف ثابت filetype_d_type[] =
  {
    DT_UNKNOWN, DT_FIFO, DT_CHR, DT_DIR, DT_BLK, DT_REG, DT_LNK, DT_SOCK,
    DT_WHT, DT_DIR
  };
static_assert (countof (filetype_d_type) == filetype_cardinality);

/* Map d_type values to enum filetype.  */
ساکن حرف ثابت d_type_filetype[UCHAR_MAX + 1] =
  {
    [DT_BLK] = blockdev, [DT_CHR] = chardev, [DT_DIR] = directory,
    [DT_FIFO] = fifo, [DT_LNK] = symbolic_link, [DT_REG] = معمول,
    [DT_SOCK] = sock, [DT_WHT] = whiteout
  };

شمار acl_type
  {
    ACL_T_NONE,
    ACL_T_UNKNOWN,
    ACL_T_LSM_CONTEXT_ONLY,
    ACL_T_YES
  };

ساخت fileinfo
  {
    /* The file name.  */
    حرف *name;

    /* For symbolic link, name of the file linked to, otherwise zero.  */
    حرف *linkname;

    /* For terminal hyperlinks. */
    حرف *absolute_name;

    ساخت stat stat;

    شمار filetype filetype;

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
    شمار acl_type acl_type;

    /* For color listings, true if a regular file has capability info.  */
    bool has_capability;

    /* Whether file name needs quoting. tri-state with -1 == unknown.  */
    صحیح quoted;

    /* Cached screen width (including quoting).  */
    size_t چوڑائی;
  };

/* Null is a valid character in a color indicator (think about Epson
   printers, for example) so we have to use a length/buffer string
   type.  */

ساخت bin_str
  {
    size_t len;			/* Number of bytes */
    حرف ثابت *string;		/* Pointer to the same */
  };

#اگر ! HAVE_TCGETPGRP
# define tcgetpgrp(Fd) 0
#اگر_ختم

ساکن size_t quote_name (حرف ثابت *name,
                          ساخت quoting_options ثابت *options,
                          صحیح needs_general_quoting,
                          ثابت ساخت bin_str *color,
                          bool allow_pad, ساخت obstack *ڈھیر,
                          حرف ثابت *absolute_name);
ساکن size_t quote_name_buf (حرف **inbuf, size_t bufsize, حرف *name,
                              ساخت quoting_options ثابت *options,
                              صحیح needs_general_quoting, size_t *چوڑائی,
                              bool *pad);
ساکن صحیح decode_switches (صحیح argc, حرف **argv);
ساکن bool file_ignored (حرف ثابت *name);
ساکن uintmax_t gobble_file (حرف ثابت *name, شمار filetype type,
                              ino_t inode, bool command_سطر_arg,
                              حرف ثابت *dirname);
ساکن ثابت ساخت bin_str * get_color_indicator (ثابت ساخت fileinfo *f,
                                                   bool symlink_ہدف);
ساکن bool print_color_indicator (ثابت ساخت bin_str *ind);
ساکن خالی put_indicator (ثابت ساخت bin_str *ind);
ساکن خالی جوڑو_ignore_نمونہ (حرف ثابت *نمونہ_2);
ساکن خالی attach (حرف *dest, حرف ثابت *dirname, حرف ثابت *name);
ساکن خالی clear_files (خالی);
ساکن خالی نکالو_dirs_from_files (حرف ثابت *dirname,
                                     bool command_سطر_arg);
ساکن bool get_link_name (حرف ثابت *filename, ساخت fileinfo *f,
                           bool command_سطر_arg);
ساکن خالی indent (size_t from, size_t مقصد);
ساکن idx_t calculate_columns (bool by_columns);
ساکن خالی print_current_files (خالی);
ساکن خالی print_dir (حرف ثابت *name, حرف ثابت *realname,
                       bool command_سطر_arg);
ساکن size_t print_file_name_and_frills (ثابت ساخت fileinfo *f,
                                          size_t آغاز_ستون);
ساکن خالی print_horizontal (خالی);
ساکن صحیح format_user_چوڑائی (uid_t u);
ساکن صحیح format_گروہ_چوڑائی (gid_t g);
ساکن خالی print_long_format (ثابت ساخت fileinfo *f);
ساکن خالی print_many_فی_سطر (خالی);
ساکن size_t print_name_with_quoting (ثابت ساخت fileinfo *f,
                                       bool symlink_ہدف,
                                       ساخت obstack *ڈھیر,
                                       size_t آغاز_ستون);
ساکن خالی prep_non_filename_متن (خالی);
ساکن bool print_type_indicator (bool stat_ok, mode_t منوال_2,
                                  شمار filetype type);
ساکن خالی print_with_separator (حرف sep);
ساکن خالی قطار_directory (حرف ثابت *name, حرف ثابت *realname,
                             bool command_سطر_arg);
ساکن خالی ترتیب_دو_files (خالی);
ساکن خالی parse_ls_color (خالی);

ساکن size_t quote_name_چوڑائی (حرف ثابت *name,
                                ساخت quoting_options ثابت *options,
                                صحیح needs_general_quoting);

/* Initial size of hash table.
   Most hierarchies are likely to be shallower than this.  */
شمار { INITIAL_جدول_سائز = 30 };

/* The set of 'active' directories, from the current command-line argument
   to the level in the hierarchy at which files are being listed.
   A directory is represented by its device and inode numbers (struct dev_ino).
   A directory is added to this set when ls begins listing it or its
   entries, and it is removed from the set just after ls has finished
   processing it.  This set is used solely to detect loops, e.g., with
   mkdir loop; cd loop; ln -s ../loop sub; ls -RL  */
ساکن Hash_جدول *فعال_dir_سیٹ;

#تعریف_کرو LOOP_DETECT (!!فعال_dir_سیٹ)

/* The table of files in the current directory:

   'cwd_file' points to a vector of 'struct fileinfo', one per file.
   'cwd_n_alloc' is the number of elements space has been allocated for.
   'cwd_n_used' is the number actually in use.  */

/* Address of block containing the files that are described.  */
ساکن ساخت fileinfo *cwd_file;

/* Length of block that 'cwd_file' points to, measured in files.  */
ساکن idx_t cwd_n_alloc;

/* Index of first unused slot in 'cwd_file'.  */
ساکن idx_t cwd_n_مستعمل;

/* Whether files needs may need padding due to quoting.  */
ساکن bool cwd_some_quoted;

/* Whether quoting style _may_ add outer quotes,
   and whether aligning those is useful.  */
ساکن bool align_variable_outer_quotes;

/* Vector of pointers to files, in proper sorted order, and the number
   of entries allocated for it.  */
ساکن خالی **sorted_file;
ساکن size_t sorted_file_alloc;

/* When true, in a color listing, color each symlink name according to the
   type of file it points to.  Otherwise, color them according to the 'ln'
   directive in LS_COLORS.  Dangling (orphan) symlinks are treated specially,
   regardless.  This is set when 'ln=target' appears in LS_COLORS.  */

ساکن bool color_symlink_as_referent;

ساکن حرف ثابت *hostname;

/* Mode of appropriate file for coloring.  */
ساکن mode_t
file_or_link_منوال (ساخت fileinfo ثابت *file)
{
  واپس (color_symlink_as_referent && file->linkok
          ? file->linkmode : file->stat.st_منوال);
}


/* Record of one pending directory waiting to be listed.  */

ساخت pending
  {
    حرف *name;
    /* If the directory is actually the file pointed to by a symbolic link we
       were told to list, 'realname' will contain the name of the symbolic
       link, otherwise zero.  */
    حرف *realname;
    bool command_سطر_arg;
    ساخت pending *اگلا;
  };

ساکن ساخت pending *pending_dirs;

/* Current time in seconds and nanoseconds since 1970, updated as
   needed when deciding whether a file is recent.  */

ساکن ساخت timespec current_وقت;

ساکن bool print_scontext;
ساکن حرف UNKNOWN_SECURITY_CONTEXT[] = "?";

/* Whether any of the files has an ACL.  This affects the width of the
   mode column.  */

ساکن bool any_has_acl;

/* The number of columns to use for columns containing inode numbers,
   block sizes, link counts, owners, groups, authors, major device
   numbers, minor device numbers, and file sizes, respectively.  */

ساکن صحیح inode_نمبر_چوڑائی;
ساکن صحیح بلاک_سائز_چوڑائی;
ساکن صحیح nlink_چوڑائی;
ساکن صحیح scontext_چوڑائی;
ساکن صحیح owner_چوڑائی;
ساکن صحیح گروہ_چوڑائی;
ساکن صحیح author_چوڑائی;
ساکن صحیح major_device_نمبر_چوڑائی;
ساکن صحیح minor_device_نمبر_چوڑائی;
ساکن صحیح file_سائز_چوڑائی;

/* Option flags */

/* long_format for lots of info, one per line.
   one_per_line for just names, one per line.
   many_per_line for just names, many per line, sorted vertically.
   horizontal for just names, many per line, sorted horizontally.
   with_commas for just names, many per line, separated by commas.

   -l (and other options that imply -l), -1, -C, -x and -m control
   this parameter.  */

شمار format
  {
    long_format,		/* -l and other options that imply -l */
    one_فی_سطر,		/* -1 */
    many_فی_سطر,		/* -C */
    horizontal,			/* -x */
    with_commas			/* -m */
  };

ساکن شمار format format;

/* 'full-iso' uses full ISO-style dates and times.  'long-iso' uses longer
   ISO-style timestamps, though shorter than 'full-iso'.  'iso' uses shorter
   ISO-style timestamps.  'locale' uses locale-dependent timestamps.  */
شمار وقت_style
  {
    بھرا_iso_وقت_style,	/* --time-style=full-iso */
    long_iso_وقت_style,	/* --time-style=long-iso */
    iso_وقت_style,		/* --time-style=iso */
    locale_وقت_style		/* --time-style=locale */
  };

ساکن حرف ثابت *ثابت وقت_style_دلائل[] =
{
  "full-iso", "long-iso", "iso", "locale", تہی
};
ساکن شمار وقت_style ثابت وقت_style_types[] =
{
  بھرا_iso_وقت_style, long_iso_وقت_style, iso_وقت_style,
  locale_وقت_style
};
ARGMATCH_VERIFY (وقت_style_دلائل, وقت_style_types);

/* Type of time to print or sort by.  Controlled by -c and -u.
   The values of each item of this enum are important since they are
   used as indices in the sort functions array (see sort_files()).  */

شمار وقت_type
  {
    وقت_mtime = 0,		/* default */
    وقت_ctime,			/* -c */
    وقت_atime,			/* -u */
    وقت_btime,                 /* birth time */
    وقت_numtypes		/* the number of elements of this enum */
  };

ساکن شمار وقت_type وقت_type;
ساکن bool explicit_وقت;

/* The file characteristic to sort by.  Controlled by -t, -S, -U, -X, -v.
   The values of each item of this enum are important since they are
   used as indices in the sort functions array (see sort_files()).  */

شمار ترتیب_دو_type
  {
    ترتیب_دو_name = 0,		/* default */
    ترتیب_دو_extension,		/* -X */
    ترتیب_دو_چوڑائی,
    ترتیب_دو_سائز,			/* -S */
    ترتیب_دو_نسخہ,		/* -v */
    ترتیب_دو_وقت,			/* -t; must be second to last */
    ترتیب_دو_none,			/* -U; must be last */
    ترتیب_دو_numtypes		/* the number of elements of this enum */
  };

ساکن شمار ترتیب_دو_type ترتیب_دو_type;

/* Direction of sort.
   false means highest first if numeric,
   lowest first if alphabetic;
   these are the defaults.
   true means the opposite order in each case.  -r  */

ساکن bool ترتیب_دو_الٹ;

/* True means to display owner information.  -g turns this off.  */

ساکن bool print_owner = true;

/* True means to display author information.  */

ساکن bool print_author;

/* True means to display group information.  -G and -o turn this off.  */

ساکن bool print_گروہ = true;

/* True means print the user and group id's as numbers rather
   than as names.  -n  */

ساکن bool numeric_ids;

/* True means mention the size in blocks of each file.  -s  */

ساکن bool print_بلاک_سائز;

/* Human-readable options for output, when printing block counts.  */
ساکن صحیح human_output_opts;

/* The units to use when printing block counts.  */
ساکن uintmax_t output_بلاک_سائز;

/* Likewise, but for file sizes.  */
ساکن صحیح file_human_output_opts;
ساکن uintmax_t file_output_بلاک_سائز = 1;

/* Follow the output with a special string.  Using this format,
   Emacs' dired mode starts up twice as fast, and can handle all
   strange characters in file names.  */
ساکن bool dired;

/* 'none' means don't mention the type of files.
   'slash' means mention directories only, with a '/'.
   'file_type' means mention file types.
   'classify' means mention file types and mark executables.

   Controlled by -F, -p, and --indicator-style.  */

شمار indicator_style
  {
    none = 0,	/*     --indicator-style=none (default) */
    slash,	/* -p, --indicator-style=slash */
    file_type,	/*     --indicator-style=file-type */
    classify	/* -F, --indicator-style=classify */
  };

ساکن شمار indicator_style indicator_style;

/* Names of indicator styles.  */
ساکن حرف ثابت *ثابت indicator_style_دلائل[] =
{
  "none", "slash", "file-type", "classify", تہی
};
ساکن شمار indicator_style ثابت indicator_style_types[] =
{
  none, slash, file_type, classify
};
ARGMATCH_VERIFY (indicator_style_دلائل, indicator_style_types);

/* True means use colors to mark types.  Also define the different
   colors as well as the stuff for the LS_COLORS environment variable.
   The LS_COLORS variable is now in a termcap-like format.  */

ساکن bool print_with_color;

ساکن bool print_hyperlink;

/* Whether we used any colors in the output so far.  If so, we will
   need to restore the default color later.  If not, we will need to
   call prep_non_filename_text before using color for the first time. */

ساکن bool مستعمل_color = false;

شمار when_type
  {
    when_never,		/* 0: default or --color=never */
    when_always,	/* 1: --color=always */
    when_if_tty		/* 2: --color=tty */
  };

شمار Dereference_symlink
  {
    DEREF_UNDEFINED = 0,		/* default */
    DEREF_NEVER,
    DEREF_COMMAND_سطر_ARGUMENTS,	/* -H */
    DEREF_COMMAND_سطر_SYMLINK_مقصد_DIR,	/* the default, in certain cases */
    DEREF_ALWAYS			/* -L */
  };

شمار indicator_no
  {
    C_بایاں, C_دایاں, C_انتہا, C_RESET, C_NORM, C_FILE, C_DIR, C_LINK,
    C_FIFO, C_SOCK,
    C_BLK, C_CHR, C_MISSING, C_ORPHAN, C_EXEC, C_DOOR, C_SETUID, C_SETGID,
    C_STICKY, C_OTHER_WRITABLE, C_STICKY_OTHER_WRITABLE, C_CAP, C_MULTIHARDLINK,
    C_CLR_مقصد_EOL
  };

ساکن حرف ثابت indicator_name[][2]=
  {
    {'l','c'}, {'r','c'}, {'e','c'}, {'r','s'}, {'n','o'},
    {'f','i'}, {'d','i'}, {'l','n'},
    {'p','i'}, {'s','o'},
    {'b','d'}, {'c','d'}, {'m','i'}, {'o','r'}, {'e','x'},
    {'d','o'}, {'s','u'}, {'s','g'},
    {'s','t'}, {'o','w'}, {'t','w'}, {'c','a'}, {'m','h'},
    {'c','l'}
  };

ساخت color_ext_type
  {
    ساخت bin_str ext;		/* The extension we're looking for */
    ساخت bin_str seq;		/* The sequence to output when we do */
    bool   exact_مطابقت;		/* Whether to compare case insensitively */
    ساخت color_ext_type *اگلا;	/* Next in list */
  };

ساکن ساخت bin_str color_indicator[] =
  {
    { 2, (حرف ثابت []) {'\033','['} },/* lc: Left of color sequence */
    { 1, (حرف ثابت []) {'m'} },	/* rc: Right of color sequence */
    { 0, تہی },			/* ec: End color (replaces lc+rs+rc) */
    { 1, (حرف ثابت []) {'0'} },	/* rs: Reset to ordinary colors */
    { 0, تہی },			/* no: Normal */
    { 0, تہی },			/* fi: File: default */
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
    { 0, تہی },			/* mi: Missing file: undefined */
    { 0, تہی },			/* or: Orphaned symlink: undefined */
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
    { 0, تہی },			/* ca: disabled by default */
    { 0, تہی },			/* mh: disabled by default */
    { 3, ((حرف ثابت [])
          {'\033','[','K'}) },		/* cl: clear to end of line */
  };

/* A list mapping file extensions to corresponding display sequence.  */
ساکن ساخت color_ext_type *color_ext_list = تہی;

/* Buffer for color sequences */
ساکن حرف *color_buf;

/* True means to check for orphaned symbolic link, for displaying
   colors, or to group symlink to directories with other dirs.  */

ساکن bool check_symlink_منوال;

/* True means mention the inode number of each file.  -i  */

ساکن bool print_inode;

/* What to do with symbolic links.  Affected by -d, -F, -H, -l (and
   other options that imply -l), and -L.  */

ساکن شمار Dereference_symlink dereference;

/* True means when a directory is found, display info on its
   contents.  -R  */

ساکن bool recursive;

/* True means when an argument is a directory name, display info
   on it itself.  -d  */

ساکن bool فوری_dirs;

/* True means that directories are grouped before files. */

ساکن bool directories_پہلا;

/* Which files to ignore.  */

ساکن شمار
{
  /* Ignore files whose names start with '.', and files specified by
     --hide and --ignore.  */
  IGNORE_DEFAULT = 0,

  /* Ignore '.', '..', and files specified by --ignore.  */
  IGNORE_DOT_AND_DOTDOT,

  /* Ignore only files specified by --ignore.  */
  IGNORE_اقل
} ignore_منوال;

/* A linked list of shell-style globbing patterns.  If a non-argument
   file name matches any of these patterns, it is ignored.
   Controlled by -I.  Multiple -I options accumulate.
   The -B option adds '*~' and '.*~' to this list.  */

ساخت ignore_نمونہ
  {
    حرف ثابت *نمونہ_2;
    ساخت ignore_نمونہ *اگلا;
  };

ساکن ساخت ignore_نمونہ *ignore_patterns;

/* Similar to IGNORE_PATTERNS, except that -a or -A causes this
   variable itself to be ignored.  */
ساکن ساخت ignore_نمونہ *hide_patterns;

/* True means output nongraphic chars in file names as '?'.
   (-q, --hide-control-chars)
   qmark_funny_chars and the quoting style (-Q, --quoting-style=WORD) are
   independent.  The algorithm is: first, obey the quoting style to get a
   string representing the file name;  then, if qmark_funny_chars is set,
   replace all nonprintable chars in that string with '?'.  It's necessary
   to replace nonprintable chars even in quoted strings, because we don't
   want to mess up the terminal if control chars get sent to it, and some
   quoting methods pass through control chars as-is.  */
ساکن bool qmark_funny_chars;

/* True means extra replacement of newline with '?'
   in cases where a newline in a file name would be
   ambiguous with newline separators.  */
ساکن bool qmark_newline;

/* Quoting options for file and dir name output.  */

ساکن ساخت quoting_options *filename_quoting_options;
ساکن ساخت quoting_options *dirname_quoting_options;

/* The number of chars per hardware tab stop.  Setting this to zero
   inhibits the use of TAB characters for separating columns.  -T */
ساکن size_t tabsize;

/* True means print each directory name before listing it.  */

ساکن bool print_dir_name;

/* The line length to use for breaking lines in many-per-line format.
   Can be set with -w.  If zero, there is no limit.  */

ساکن size_t سطر_لمبائی;

/* The local time zone rules, as per the TZ environment variable.  */

ساکن timezone_t localtz;

/* If true, the file listing format requires that stat be called on
   each file.  */

ساکن bool format_needs_stat;

/* Similar to 'format_needs_stat', but set if only the file type is
   needed.  */

ساکن bool format_needs_type;

/* Like 'format_needs_stat', but set only if capability colors are needed.  */

ساکن bool format_needs_capability;

/* An arbitrary limit on the number of bytes in a printed timestamp.
   This is set to a relatively small value to avoid the need to worry
   about denial-of-service attacks on servers that run "ls" on behalf
   of remote clients.  1000 bytes should be enough for any practical
   timestamp format.  */

شمار { وقت_STAMP_LEN_زیادہ_سے_زیادہ = MAX (1000, INT_STRLEN_BOUND (time_t)) };

/* strftime formats for non-recent and recent files, respectively, in
   -l output.  */

ساکن حرف ثابت *long_وقت_format[2] =
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

ساکن sigset_t caught_اشارات;

/* If nonzero, the value of the pending fatal signal.  */

ساکن sig_atomic_t متلون مداخلت_signal;

/* A count of the number of pending stop signals that have been received.  */

ساکن sig_atomic_t متلون stop_signal_گنتی;

/* Desired exit status.  */

ساکن صحیح exit_حالت;

/* Exit statuses.  */
شمار
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
شمار
{
  AUTHOR_OPTION = CHAR_MAX + 1,
  بلاک_سائز_OPTION,
  COLOR_OPTION,
  DEREFERENCE_COMMAND_سطر_SYMLINK_مقصد_DIR_OPTION,
  FILE_TYPE_INDICATOR_OPTION,
  FORMAT_OPTION,
  بھرا_وقت_OPTION,
  گروہ_DIRECTORIES_پہلا_OPTION,
  HIDE_OPTION,
  HYPERLINK_OPTION,
  INDICATOR_STYLE_OPTION,
  QUOTING_STYLE_OPTION,
  SHOW_CONTROL_CHARS_OPTION,
  SI_OPTION,
  ترتیب_دو_OPTION,
  وقت_OPTION,
  وقت_STYLE_OPTION,
  ZERO_OPTION,
};

ساکن ساخت option ثابت long_options[] =
{
  {"all", no_دلیل, تہی, 'a'},
  {"escape", no_دلیل, تہی, 'b'},
  {"directory", no_دلیل, تہی, 'd'},
  {"dired", no_دلیل, تہی, 'D'},
  {"full-time", no_دلیل, تہی, بھرا_وقت_OPTION},
  {"group-directories-first", no_دلیل, تہی,
   گروہ_DIRECTORIES_پہلا_OPTION},
  {"human-readable", no_دلیل, تہی, 'h'},
  {"inode", no_دلیل, تہی, 'i'},
  {"kibibytes", no_دلیل, تہی, 'k'},
  {"numeric-uid-gid", no_دلیل, تہی, 'n'},
  {"no-group", no_دلیل, تہی, 'G'},
  {"hide-control-chars", no_دلیل, تہی, 'q'},
  {"reverse", no_دلیل, تہی, 'r'},
  {"size", no_دلیل, تہی, 's'},
  {"width", required_دلیل, تہی, 'w'},
  {"almost-all", no_دلیل, تہی, 'A'},
  {"ignore-backups", no_دلیل, تہی, 'B'},
  {"classify", optional_دلیل, تہی, 'F'},
  {"file-type", no_دلیل, تہی, FILE_TYPE_INDICATOR_OPTION},
  {"si", no_دلیل, تہی, SI_OPTION},
  {"dereference-command-line", no_دلیل, تہی, 'H'},
  {"dereference-command-line-symlink-to-dir", no_دلیل, تہی,
   DEREFERENCE_COMMAND_سطر_SYMLINK_مقصد_DIR_OPTION},
  {"hide", required_دلیل, تہی, HIDE_OPTION},
  {"ignore", required_دلیل, تہی, 'I'},
  {"indicator-style", required_دلیل, تہی, INDICATOR_STYLE_OPTION},
  {"dereference", no_دلیل, تہی, 'L'},
  {"literal", no_دلیل, تہی, 'N'},
  {"quote-name", no_دلیل, تہی, 'Q'},
  {"quoting-style", required_دلیل, تہی, QUOTING_STYLE_OPTION},
  {"recursive", no_دلیل, تہی, 'R'},
  {"format", required_دلیل, تہی, FORMAT_OPTION},
  {"show-control-chars", no_دلیل, تہی, SHOW_CONTROL_CHARS_OPTION},
  {"sort", required_دلیل, تہی, ترتیب_دو_OPTION},
  {"tabsize", required_دلیل, تہی, 'T'},
  {"time", required_دلیل, تہی, وقت_OPTION},
  {"time-style", required_دلیل, تہی, وقت_STYLE_OPTION},
  {"zero", no_دلیل, تہی, ZERO_OPTION},
  {"color", optional_دلیل, تہی, COLOR_OPTION},
  {"hyperlink", optional_دلیل, تہی, HYPERLINK_OPTION},
  {"block-size", required_دلیل, تہی, بلاک_سائز_OPTION},
  {"context", no_دلیل, تہی, 'Z'},
  {"author", no_دلیل, تہی, AUTHOR_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0}
};

ساکن حرف ثابت *ثابت format_دلائل[] =
{
  "verbose", "long", "commas", "horizontal", "across",
  "vertical", "single-column", تہی
};
ساکن شمار format ثابت format_types[] =
{
  long_format, long_format, with_commas, horizontal, horizontal,
  many_فی_سطر, one_فی_سطر
};
ARGMATCH_VERIFY (format_دلائل, format_types);

ساکن حرف ثابت *ثابت ترتیب_دو_دلائل[] =
{
  "none", "size", "time", "version", "extension",
  "name", "width", تہی
};
ساکن شمار ترتیب_دو_type ثابت ترتیب_دو_types[] =
{
  ترتیب_دو_none, ترتیب_دو_سائز, ترتیب_دو_وقت, ترتیب_دو_نسخہ, ترتیب_دو_extension,
  ترتیب_دو_name, ترتیب_دو_چوڑائی
};
ARGMATCH_VERIFY (ترتیب_دو_دلائل, ترتیب_دو_types);

ساکن حرف ثابت *ثابت وقت_دلائل[] =
{
  "atime", "access", "use",
  "ctime", "status",
  "mtime", "modification",
  "birth", "creation",
  تہی
};
ساکن شمار وقت_type ثابت وقت_types[] =
{
  وقت_atime, وقت_atime, وقت_atime,
  وقت_ctime, وقت_ctime,
  وقت_mtime, وقت_mtime,
  وقت_btime, وقت_btime,
};
ARGMATCH_VERIFY (وقت_دلائل, وقت_types);

ساکن حرف ثابت *ثابت when_دلائل[] =
{
  /* force and none are for compatibility with another color-ls version */
  "always", "yes", "force",
  "never", "no", "none",
  "auto", "tty", "if-tty", تہی
};
ساکن شمار when_type ثابت when_types[] =
{
  when_always, when_always, when_always,
  when_never, when_never, when_never,
  when_if_tty, when_if_tty, when_if_tty
};
ARGMATCH_VERIFY (when_دلائل, when_types);

/* Information about filling a column.  */
ساخت عمود_info
{
  bool valid_len;
  size_t سطر_len;
  size_t *ستون_arr;
};

/* Array with information about column fullness.  */
ساکن ساخت عمود_info *عمود_info;

/* Maximum number of columns ever possible for this display.  */
ساکن size_t max_idx;

/* The minimum width of a non-final column is 3: 1 character for the name
   and 2 for the separating white space.  The final column is not padded.  */
شمار { MIN_عمود_چوڑائی = 3 };


/* This zero-based index is for the --dired option.  It is incremented
   for each byte of output generated by this program so that the beginning
   and ending indices (in that output) of every file name can be recorded
   and later output themselves.  */
ساکن off_t dired_pos;

ساکن خالی
dired_outbyte (حرف c)
{
  dired_pos++;
  حرف_لکھو (c);
}

/* Output the buffer S, of length S_LEN, and increment DIRED_POS by S_LEN.  */
ساکن خالی
dired_outbuf (حرف ثابت *s, size_t s_len)
{
  dired_pos += s_len;
  fwrite (s, حجم *s, s_len, stdout);
}

/* Output the string S, and increment DIRED_POS by its length.  */
ساکن خالی
dired_outstring (حرف ثابت *s)
{
  dired_outbuf (s, لڑی_لمبائی (s));
}

ساکن خالی
dired_indent (خالی)
{
  اگر (dired)
    dired_outstring ("  ");
}

/* With --dired, store pairs of beginning and ending indices of file names.  */
ساکن ساخت obstack dired_obstack;

/* With --dired, store pairs of beginning and ending indices of any
   directory names that appear as headers (just before 'total' line)
   for lists of directory entries.  Such directory names are seen when
   listing hierarchies using -R and when a directory is listed with at
   least one other command line argument.  */
ساکن ساخت obstack subdired_obstack;

/* Save the current index on the specified obstack, OBS.  */
ساکن خالی
ڈالو_current_dired_pos (ساخت obstack *obs)
{
  اگر (dired)
    obstack_grow (obs, &dired_pos, حجم dired_pos);
}

/* With -R, this stack is used to help detect directory cycles.
   The device/inode pairs on this stack mirror the pairs in the
   active_dir_set hash table.  */
ساکن ساخت obstack dev_ino_obstack;

/* Push a pair onto the device/inode stack.  */
ساکن خالی
dev_ino_ڈالو (dev_t dev, ino_t ino)
{
  خالی *vdi;
  ساخت dev_ino *di;
  صحیح dev_ino_سائز = حجم *di;
  obstack_blank (&dev_ino_obstack, dev_ino_سائز);
  vdi = obstack_اگلا_free (&dev_ino_obstack);
  di = vdi;
  di--;
  di->st_dev = dev;
  di->st_ino = ino;
}

/* Pop a dev/ino struct off the global dev_ino_obstack
   and return that struct.  */
ساکن ساخت dev_ino
dev_ino_نکالو (خالی)
{
  خالی *vdi;
  ساخت dev_ino *di;
  صحیح dev_ino_سائز = حجم *di;
  affirm (dev_ino_سائز <= obstack_object_سائز (&dev_ino_obstack));
  obstack_blank_fast (&dev_ino_obstack, -dev_ino_سائز);
  vdi = obstack_اگلا_free (&dev_ino_obstack);
  di = vdi;
  واپس *di;
}

ساکن خالی
assert_matching_dev_ino (حرف ثابت *name, ساخت dev_ino di)
{
  MAYBE_غیر_مستعمل ساخت stat sb;
  assure (0 <= stat (name, &sb));
  assure (sb.st_dev == di.st_dev);
  assure (sb.st_ino == di.st_ino);
}

ساکن حرف eolbyte = '\n';

/* Write to standard output PREFIX, followed by the quoting style and
   a space-separated list of the integers stored in OS all on one line.  */

ساکن خالی
dired_dump_obstack (حرف ثابت *سابقہ, ساخت obstack *os)
{
  size_t n_pos;

  n_pos = obstack_object_سائز (os) / حجم (dired_pos);
  اگر (n_pos > 0)
    {
      off_t *pos = obstack_اختتام (os);
      fputs (سابقہ, stdout);
      برائے (size_t i = 0; i < n_pos; i++)
        {
          intmax_t p = pos[i];
          لکھو (" %jd", p);
        }
      حرف_لکھو ('\n');
    }
}

/* Return the platform birthtime member of the stat structure,
   or fallback to the mtime member, which we have populated
   from the statx structure or reset to an invalid timestamp
   where birth time is not supported.  */
ساکن ساخت timespec
get_stat_btime (ساخت stat ثابت *st)
{
  ساخت timespec btimespec;

#اگر HAVE_STATX && defined STATX_INO
  btimespec = get_stat_mtime (st);
#ورنہ
  btimespec = get_stat_birthtime (st);
#اگر_ختم

  واپس btimespec;
}

#اگر HAVE_STATX && defined STATX_INO
ATTRIBUTE_خالص
ساکن غیرعلامتی صحیح
وقت_type_مقصد_statx (خالی)
{
  انتخاب (وقت_type)
    {
    حالت وقت_ctime:
      واپس STATX_CTIME;
    حالت وقت_mtime:
      واپس STATX_MTIME;
    حالت وقت_atime:
      واپس STATX_ATIME;
    حالت وقت_btime:
      واپس STATX_BTIME;
    حالت وقت_numtypes: طےشدہ:
      unreachable ();
    }
    واپس 0;
}

ATTRIBUTE_خالص
ساکن غیرعلامتی صحیح
calc_req_نقاب (خالی)
{
  غیرعلامتی صحیح نقاب = STATX_منوال;

  اگر (print_inode)
    نقاب |= STATX_INO;

  اگر (print_بلاک_سائز)
    نقاب |= STATX_بلاکیں;

  اگر (format == long_format) {
    نقاب |= STATX_NLINK | STATX_سائز | وقت_type_مقصد_statx ();
    اگر (print_owner || print_author)
      نقاب |= STATX_UID;
    اگر (print_گروہ)
      نقاب |= STATX_GID;
  }

  انتخاب (ترتیب_دو_type)
    {
    حالت ترتیب_دو_none:
    حالت ترتیب_دو_name:
    حالت ترتیب_دو_نسخہ:
    حالت ترتیب_دو_extension:
    حالت ترتیب_دو_چوڑائی:
      توڑو;
    حالت ترتیب_دو_وقت:
      نقاب |= وقت_type_مقصد_statx ();
      توڑو;
    حالت ترتیب_دو_سائز:
      نقاب |= STATX_سائز;
      توڑو;
    حالت ترتیب_دو_numtypes: طےشدہ:
      unreachable ();
    }

  واپس نقاب;
}

ساکن صحیح
do_statx (صحیح fd, حرف ثابت *name, ساخت stat *st, صحیح flags,
          غیرعلامتی صحیح نقاب)
{
  ساخت statx stx;
  bool want_btime = نقاب & STATX_BTIME;
  صحیح ret = statx (fd, name, flags | AT_NO_AUTOMOUNT, نقاب, &stx);
  اگر (ret >= 0)
    {
      statx_مقصد_stat (&stx, st);
      /* Since we only need one timestamp type,
         store birth time in st_mtim.  */
      اگر (want_btime)
        {
          اگر (stx.stx_نقاب & STATX_BTIME)
            st->st_mtim = statx_timestamp_مقصد_timespec (stx.stx_btime);
          ورنہ
            st->st_mtim.tv_sec = st->st_mtim.tv_nsec = -1;
        }
    }

  واپس ret;
}

ساکن صحیح
do_stat (حرف ثابت *name, ساخت stat *st)
{
  واپس do_statx (AT_FDCWD, name, st, 0, calc_req_نقاب ());
}

ساکن صحیح
do_lstat (حرف ثابت *name, ساخت stat *st)
{
  واپس do_statx (AT_FDCWD, name, st, AT_SYMLINK_NOFOLLOW, calc_req_نقاب ());
}

ساکن صحیح
stat_for_منوال (حرف ثابت *name, ساخت stat *st)
{
  واپس do_statx (AT_FDCWD, name, st, 0, STATX_منوال);
}

/* dev+ino should be static, so no need to sync with backing store */
ساکن صحیح
stat_for_ino (حرف ثابت *name, ساخت stat *st)
{
  واپس do_statx (AT_FDCWD, name, st, 0, STATX_INO);
}

ساکن صحیح
fstat_for_ino (صحیح fd, ساخت stat *st)
{
  واپس do_statx (fd, "", st, AT_خالی_خانے_راستہ, STATX_INO);
}
#ورنہ
ساکن صحیح
do_stat (حرف ثابت *name, ساخت stat *st)
{
  واپس stat (name, st);
}

ساکن صحیح
do_lstat (حرف ثابت *name, ساخت stat *st)
{
  واپس lstat (name, st);
}

ساکن صحیح
stat_for_منوال (حرف ثابت *name, ساخت stat *st)
{
  واپس stat (name, st);
}

ساکن صحیح
stat_for_ino (حرف ثابت *name, ساخت stat *st)
{
  واپس stat (name, st);
}

ساکن صحیح
fstat_for_ino (صحیح fd, ساخت stat *st)
{
  واپس fstat (fd, st);
}
#اگر_ختم

/* Return the address of the first plain %b spec in FMT, or NULL if
   there is no such spec.  %5b etc. do not match, so that user
   widths/flags are honored.  */

ATTRIBUTE_خالص
ساکن حرف ثابت *
پہلا_percent_b (حرف ثابت *fmt)
{
  برائے (; *fmt; fmt++)
    اگر (fmt[0] == '%')
      انتخاب (fmt[1])
        {
        حالت 'b': واپس fmt;
        حالت '%': fmt++; توڑو;
        }
  واپس تہی;
}

ساکن حرف RFC3986[256];
ساکن خالی
file_escape_init (خالی)
{
  برائے (صحیح i = 0; i < 256; i++)
    RFC3986[i] |= c_isalnum (i) || i == '~' || i == '-' || i == '.' || i == '_';
}

شمار { MBSWIDTH_FLAGS = MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE };

/* Read the abbreviated month names from the locale, to align them
   and to determine the max width of the field and to truncate names
   greater than our max allowed.
   Note even though this handles multibyte locales correctly
   it's not restricted to them as single byte locales can have
   variable width abbreviated months and also precomputing/caching
   the names was seen to increase the performance of ls significantly.  */

/* abformat[RECENT][MON] is the format to use for timestamps with
   recentness RECENT and month MON.  */
شمار { ABFORMAT_سائز = 128 };
ساکن حرف abformat[2][12][ABFORMAT_سائز];
/* True if precomputed formats should be used.  This can be false if
   nl_langinfo fails, if a format or month abbreviation is unusually
   long, or if a month abbreviation contains '%'.  */
ساکن bool use_abformat;

/* Store into ABMON the abbreviated month names, suitably aligned.
   Return true if successful.  */

ساکن bool
abmon_init (حرف abmon[12][ABFORMAT_سائز])
{
#اگر_غیر_تعریف_شدہ HAVE_NL_LANGINFO
  واپس false;
#ورنہ
  صحیح max_mon_چوڑائی = 0;
  صحیح mon_چوڑائی[12];
  صحیح mon_len[12];

  برائے (صحیح i = 0; i < 12; i++)
    {
      حرف ثابت *abbr = nl_langinfo (ABMON_1 + i);
      mon_len[i] = strnlen (abbr, ABFORMAT_سائز);
      اگر (mon_len[i] == ABFORMAT_سائز)
        واپس false;
      اگر (strchr (abbr, '%'))
        واپس false;
      mon_چوڑائی[i] = mbswidth (strcpy (abmon[i], abbr), MBSWIDTH_FLAGS);
      اگر (mon_چوڑائی[i] < 0)
        واپس false;
      max_mon_چوڑائی = MAX (max_mon_چوڑائی, mon_چوڑائی[i]);
    }

  برائے (صحیح i = 0; i < 12; i++)
    {
      صحیح fill = max_mon_چوڑائی - mon_چوڑائی[i];
      اگر (ABFORMAT_سائز - mon_len[i] <= fill)
        واپس false;
      bool align_بایاں = !c_isdigit (abmon[i][0]);
      صحیح fill_انحراف;
      اگر (align_بایاں)
        fill_انحراف = mon_len[i];
      ورنہ
        {
          memmove (abmon[i] + fill, abmon[i], mon_len[i]);
          fill_انحراف = 0;
        }
      memset (abmon[i] + fill_انحراف, ' ', fill);
      abmon[i][mon_len[i] + fill] = '\0';
    }

  واپس true;
#اگر_ختم
}

/* Initialize ABFORMAT and USE_ABFORMAT.  */

ساکن خالی
abformat_init (خالی)
{
  حرف ثابت *pb[2];
  برائے (صحیح recent = 0; recent < 2; recent++)
    pb[recent] = پہلا_percent_b (long_وقت_format[recent]);
  اگر (! (pb[0] || pb[1]))
    واپس;

  حرف abmon[12][ABFORMAT_سائز];
  اگر (! abmon_init (abmon))
    واپس;

  برائے (صحیح recent = 0; recent < 2; recent++)
    {
      حرف ثابت *fmt = long_وقت_format[recent];
      برائے (صحیح i = 0; i < 12; i++)
        {
          حرف *nfmt = abformat[recent][i];
          صحیح nbytes;

          اگر (! pb[recent])
            nbytes = snprintf (nfmt, ABFORMAT_سائز, "%s", fmt);
          ورنہ
            {
              اگر (! (pb[recent] - fmt <= MIN (ABFORMAT_سائز, زیادہ_سے_زیادہ_عدد)))
                واپس;
              صحیح سابقہ_len = pb[recent] - fmt;
              nbytes = snprintf (nfmt, ABFORMAT_سائز, "%.*s%s%s",
                                 سابقہ_len, fmt, abmon[i], pb[recent] + 2);
            }

          اگر (! (0 <= nbytes && nbytes < ABFORMAT_سائز))
            واپس;
        }
    }

  use_abformat = true;
}

ساکن size_t
dev_ino_hash (خالی ثابت *x, size_t جدول_سائز)
{
  ساخت dev_ino ثابت *p = x;
  واپس (uintmax_t) {p->st_ino} % جدول_سائز;
}

ساکن bool
dev_ino_compare (خالی ثابت *x, خالی ثابت *y)
{
  ساخت dev_ino ثابت *a = x;
  ساخت dev_ino ثابت *b = y;
  واپس PSAME_INODE (a, b);
}

ساکن خالی
dev_ino_free (خالی *x)
{
  آزاد (x);
}

/* Add the device/inode pair (P->st_dev/P->st_ino) to the set of
   active directories.  Return true if there is already a matching
   entry in the table.  */

ساکن bool
دورہ_dir (dev_t dev, ino_t ino)
{
  ساخت dev_ino *ent;
  ساخت dev_ino *ent_from_جدول;
  bool ملا_مطابقت;

  ent = xmalloc (حجم *ent);
  ent->st_ino = ino;
  ent->st_dev = dev;

  /* Attempt to insert this entry into the table.  */
  ent_from_جدول = hash_داخل_کرو (فعال_dir_سیٹ, ent);

  اگر (ent_from_جدول == تہی)
    {
      /* Insertion failed due to lack of memory.  */
      xalloc_die ();
    }

  ملا_مطابقت = (ent_from_جدول != ent);

  اگر (ملا_مطابقت)
    {
      /* ent was not inserted, so free it.  */
      آزاد (ent);
    }

  واپس ملا_مطابقت;
}

ساکن خالی
free_pending_ent (ساخت pending *p)
{
  آزاد (p->name);
  آزاد (p->realname);
  آزاد (p);
}

ساکن bool
is_colored (شمار indicator_no type)
{
  /* Return true unless the string is "", "0" or "00"; try to be efficient.  */
  size_t len = color_indicator[type].len;
  اگر (len == 0)
    واپس false;
  اگر (2 < len)
    واپس true;
  حرف ثابت *s = color_indicator[type].string;
  واپس (s[0] != '0') | (s[len - 1] != '0');
}

ساکن خالی
restore_default_color (خالی)
{
  put_indicator (&color_indicator[C_بایاں]);
  put_indicator (&color_indicator[C_دایاں]);
}

ساکن خالی
سیٹ_معمول_color (خالی)
{
  اگر (print_with_color && is_colored (C_NORM))
    {
      put_indicator (&color_indicator[C_بایاں]);
      put_indicator (&color_indicator[C_NORM]);
      put_indicator (&color_indicator[C_دایاں]);
    }
}

/* An ordinary signal was received; arrange for the program to exit.  */

ساکن خالی
sighandler (صحیح sig)
{
  اگر (! مداخلت_signal)
    مداخلت_signal = sig;
}

/* A SIGTSTP was received; arrange for the program to suspend itself.  */

ساکن خالی
stophandler (MAYBE_غیر_مستعمل صحیح sig)
{
  اگر (! مداخلت_signal)
    stop_signal_گنتی++;
}

/* Process any pending signals.  If signals are caught, this function
   should be called periodically.  Ideally there should never be an
   unbounded amount of time when signals are not being processed.
   Signal handling can restore the default colors, so callers must
   immediately change colors after invoking this function.  */

ساکن خالی
process_اشارات (خالی)
{
  جب (مداخلت_signal || stop_signal_گنتی)
    {
      صحیح sig;
      صحیح stops;
      sigset_t oldset;

      اگر (مستعمل_color)
        restore_default_color ();
      fflush (stdout);

      sigprocmask (SIG_BLOCK, &caught_اشارات, &oldset);

      /* Reload interrupt_signal and stop_signal_count, in case a new
         signal was handled before sigprocmask took effect.  */
      sig = مداخلت_signal;
      stops = stop_signal_گنتی;

      /* SIGTSTP is special, since the application can receive that signal
         more than once.  In this case, don't set the signal handler to the
         default.  Instead, just raise the uncatchable SIGSTOP.  */
      اگر (stops)
        {
          stop_signal_گنتی = stops - 1;
          sig = SIGSTOP;
        }
      ورنہ
        اشارہ (sig, SIG_DFL);

      /* Exit or suspend the program.  */
      اٹھاؤ (sig);
      sigprocmask (SIG_SETMASK, &oldset, تہی);

      /* If execution reaches here, then the program has been
         continued (after being suspended).  */
    }
}

/* Setup signal handlers if INIT is true,
   otherwise restore to the default.  */

ساکن خالی
signal_تیاری (bool init)
{
  /* The signals that are trapped, and the number of such signals.  */
  ساکن صحیح ثابت stop_sig[] =
    {
      /* This one is handled specially.  */
      SIGTSTP,
    };

  شمار { nsigs = countof (رکن_sig), nstop = countof (stop_sig) };

  اگر (init)
    {
      ساخت sigaction act;

      sigemptyset (&caught_اشارات);
      برائے (صحیح j = 0; j < nsigs + nstop; j++)
        {
          صحیح sig = j < nsigs ? رکن_sig[j]: stop_sig[j - nsigs];
          sigaction (sig, تہی, &act);
          اگر (act.sa_handler != SIG_IGN)
            sigaddset (&caught_اشارات, sig);
        }

      act.sa_mask = caught_اشارات;
      act.sa_flags = SA_RESTART;

      برائے (صحیح j = 0; j < nsigs + nstop; j++)
        {
          صحیح sig = j < nsigs ? رکن_sig[j]: stop_sig[j - nsigs];
          اگر (sigismember (&caught_اشارات, sig))
            {
              act.sa_handler = j < nsigs ? sighandler : stophandler;
              sigaction (sig, &act, تہی);
            }
        }
    }
  ورنہ /* restore.  */
    {
      برائے (صحیح j = 0; j < nsigs + nstop; j++)
        {
          صحیح sig = j < nsigs ? رکن_sig[j]: stop_sig[j - nsigs];
          اگر (sigismember (&caught_اشارات, sig))
            اشارہ (sig, SIG_DFL);
        }
    }
}

ساکن خالی
signal_init (خالی)
{
  signal_تیاری (true);
}

ساکن خالی
signal_restore (خالی)
{
  signal_تیاری (false);
}

صحیح
مین (صحیح argc, حرف **argv)
{
  صحیح i;
  ساخت pending *thispend;
  صحیح n_files;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (LS_FAILURE);
  atexit (close_stdout);

  static_assert (countof (color_indicator) == countof (indicator_name));

  exit_حالت = EXIT_SUCCESS;
  print_dir_name = true;
  pending_dirs = تہی;

  current_وقت.tv_sec = TYPE_MINIMUM (time_t);
  current_وقت.tv_nsec = -1;

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

  اگر (directories_پہلا)
    check_symlink_منوال = true;
  ورنہ اگر (print_with_color)
    {
      /* Avoid following symbolic links when possible.  */
      اگر (is_colored (C_ORPHAN)
          || (is_colored (C_EXEC) && color_symlink_as_referent)
          || (is_colored (C_MISSING) && format == long_format))
        check_symlink_منوال = true;
    }

  اگر (dereference == DEREF_UNDEFINED)
    dereference = ((فوری_dirs
                    || indicator_style == classify
                    || format == long_format)
                   ? DEREF_NEVER
                   : DEREF_COMMAND_سطر_SYMLINK_مقصد_DIR);

  /* When using -R, initialize a data structure we'll use to
     detect any directory cycles.  */
  اگر (recursive)
    {
      فعال_dir_سیٹ = hash_initialize (INITIAL_جدول_سائز, تہی,
                                        dev_ino_hash,
                                        dev_ino_compare,
                                        dev_ino_free);
      اگر (فعال_dir_سیٹ == تہی)
        xalloc_die ();

      obstack_init (&dev_ino_obstack);
    }

  localtz = tzalloc (getenv ("TZ"));

  format_needs_stat = ((ترتیب_دو_type == ترتیب_دو_وقت) | (ترتیب_دو_type == ترتیب_دو_سائز)
                       | (format == long_format)
                       | print_بلاک_سائز | print_hyperlink | print_scontext);
  format_needs_type = ((! format_needs_stat)
                       & (recursive | print_with_color | print_scontext
                          | directories_پہلا
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
  cwd_file = xmalloc (cwd_n_alloc * حجم *cwd_file);
  cwd_n_مستعمل = 0;

  clear_files ();

  n_files = argc - i;

  اگر (n_files <= 0)
    {
      اگر (فوری_dirs)
        gobble_file (".", directory, NOT_AN_INODE_نمبر, true, تہی);
      ورنہ
        قطار_directory (".", تہی, true);
    }
  ورنہ
    کرو
      gobble_file (argv[i++], unknown, NOT_AN_INODE_نمبر, true, تہی);
    جب (i < argc);

  اگر (cwd_n_مستعمل)
    {
      ترتیب_دو_files ();
      اگر (!فوری_dirs)
        نکالو_dirs_from_files (تہی, true);
      /* 'cwd_n_used' might be zero now.  */
    }

  /* In the following if/else blocks, it is sufficient to test 'pending_dirs'
     (and not pending_dirs->name) because there may be no markers in the queue
     at this point.  A marker may be enqueued when extract_dirs_from_files is
     called with a non-empty string or via print_dir.  */
  اگر (cwd_n_مستعمل)
    {
      print_current_files ();
      اگر (pending_dirs)
        dired_outbyte ('\n');
    }
  ورنہ اگر (n_files <= 1 && pending_dirs && !pending_dirs->اگلا)
    print_dir_name = false;

  جب (pending_dirs)
    {
      thispend = pending_dirs;
      pending_dirs = pending_dirs->اگلا;

      اگر (LOOP_DETECT)
        {
          اگر (thispend->name == تہی)
            {
              /* thispend->name == NULL means this is a marker entry
                 indicating we've finished processing the directory.
                 Use its dev/ino numbers to remove the corresponding
                 entry from the active_dir_set hash table.  */
              ساخت dev_ino di = dev_ino_نکالو ();
              ساخت dev_ino *ملا = hash_remove (فعال_dir_سیٹ, &di);
              اگر (false)
                assert_matching_dev_ino (thispend->realname, di);
              affirm (ملا);
              dev_ino_free (ملا);
              free_pending_ent (thispend);
              جاری;
            }
        }

      print_dir (thispend->name, thispend->realname,
                 thispend->command_سطر_arg);

      free_pending_ent (thispend);
      print_dir_name = true;
    }

  اگر (print_with_color && مستعمل_color)
    {

      /* Skip the restore when it would be a no-op, i.e.,
         when left is "\033[" and right is "m".  */
      اگر (!(color_indicator[C_بایاں].len == 2
            && memeq (color_indicator[C_بایاں].string, "\033[", 2)
            && color_indicator[C_دایاں].len == 1
            && color_indicator[C_دایاں].string[0] == 'm'))
        restore_default_color ();

      fflush (stdout);

      signal_restore ();

      /* Act on any signals that arrived before the default was restored.
         This can process signals out of order, but there doesn't seem to
         be an easy way to do them in order, and the order isn't that
         important anyway.  */
      برائے (صحیح j = stop_signal_گنتی; j; j--)
        اٹھاؤ (SIGSTOP);
      صحیح j = مداخلت_signal;
      اگر (j)
        اٹھاؤ (j);
    }

  اگر (dired)
    {
      /* No need to free these since we're about to exit.  */
      dired_dump_obstack ("//DIRED//", &dired_obstack);
      dired_dump_obstack ("//SUBDIRED//", &subdired_obstack);
      لکھو ("//DIRED-OPTIONS// --quoting-style=%s\n",
              quoting_style_دلائل[get_quoting_style (filename_quoting_options)]);
    }

  اگر (LOOP_DETECT)
    {
      assure (hash_get_n_entries (فعال_dir_سیٹ) == 0);
      hash_free (فعال_dir_سیٹ);
    }

  واپس exit_حالت;
}

/* Return the line length indicated by the value given by SPEC, or -1
   if unsuccessful.  0 means no limit on line length.  */

ساکن ptrdiff_t
decode_سطر_لمبائی (حرف ثابت *spec)
{
  uintmax_t val;

  /* Treat too-large values as if they were 0, which is
     effectively infinity.  */
  انتخاب (xstrtoumax (spec, تہی, 0, &val, ""))
    {
    حالت LONGINT_OK:
      واپس val <= MIN (PTRDIFF_MAX, SIZE_MAX) ? val : 0;

    حالت LONGINT_حد_سے_تجاوز:
      واپس 0;

    حالت LONGINT_INVALID:
    حالت LONGINT_INVALID_SUFFIX_CHAR:
    حالت LONGINT_INVALID_SUFFIX_CHAR_WITH_حد_سے_تجاوز:
      واپس -1;

    طےشدہ:
      unreachable ();
    }
}

/* Return true if standard output is a tty, caching the result.  */

ساکن bool
stdout_isatty (خالی)
{
  ساکن علامتی حرف برآمد_tty = -1;
  اگر (برآمد_tty < 0)
    برآمد_tty = isatty (STDOUT_FILENO);
  assume (برآمد_tty == 0 || برآمد_tty == 1);
  واپس برآمد_tty;
}

/* Set all the option flags according to the switches specified.
   Return the index of the first non-option argument.  */

ساکن صحیح
decode_switches (صحیح argc, حرف **argv)
{
  حرف *وقت_style_option = تہی;

  /* These variables are false or -1 unless a switch says otherwise.  */
  bool kibibytes_specified = false;
  صحیح format_opt = -1;
  صحیح hide_control_chars_opt = -1;
  صحیح quoting_style_opt = -1;
  صحیح ترتیب_دو_opt = -1;
  ptrdiff_t tabsize_opt = -1;
  ptrdiff_t چوڑائی_opt = -1;

  جب (true)
    {
      صحیح oi = -1;
      صحیح c = getopt_long (argc, argv,
                           "abcdfghiklmnopqrstuvw:xABCDFGHI:LNQRST:UXZ1",
                           long_options, &oi);
      اگر (c == -1)
        توڑو;

      انتخاب (c)
        {
        حالت 'a':
          ignore_منوال = IGNORE_اقل;
          توڑو;

        حالت 'b':
          quoting_style_opt = escape_quoting_style;
          توڑو;

        حالت 'c':
          وقت_type = وقت_ctime;
          explicit_وقت = true;
          توڑو;

        حالت 'd':
          فوری_dirs = true;
          توڑو;

        حالت 'f':
          ignore_منوال = IGNORE_اقل; /* enable -a */
          ترتیب_دو_opt = ترتیب_دو_none;         /* enable -U */
          توڑو;

        حالت FILE_TYPE_INDICATOR_OPTION: /* --file-type */
          indicator_style = file_type;
          توڑو;

        حالت 'g':
          format_opt = long_format;
          print_owner = false;
          توڑو;

        حالت 'h':
          file_human_output_opts = human_output_opts =
            human_autoscale | human_SI | human_بنیاد_1024;
          file_output_بلاک_سائز = output_بلاک_سائز = 1;
          توڑو;

        حالت 'i':
          print_inode = true;
          توڑو;

        حالت 'k':
          kibibytes_specified = true;
          توڑو;

        حالت 'l':
          format_opt = long_format;
          توڑو;

        حالت 'm':
          format_opt = with_commas;
          توڑو;

        حالت 'n':
          numeric_ids = true;
          format_opt = long_format;
          توڑو;

        حالت 'o':  /* Just like -l, but don't display group info.  */
          format_opt = long_format;
          print_گروہ = false;
          توڑو;

        حالت 'p':
          indicator_style = slash;
          توڑو;

        حالت 'q':
          hide_control_chars_opt = true;
          توڑو;

        حالت 'r':
          ترتیب_دو_الٹ = true;
          توڑو;

        حالت 's':
          print_بلاک_سائز = true;
          توڑو;

        حالت 't':
          ترتیب_دو_opt = ترتیب_دو_وقت;
          توڑو;

        حالت 'u':
          وقت_type = وقت_atime;
          explicit_وقت = true;
          توڑو;

        حالت 'v':
          ترتیب_دو_opt = ترتیب_دو_نسخہ;
          توڑو;

        حالت 'w':
          چوڑائی_opt = decode_سطر_لمبائی (optarg);
          اگر (چوڑائی_opt < 0)
            error (LS_FAILURE, 0, "%s: %s", _("invalid line width"),
                   quote (optarg));
          توڑو;

        حالت 'x':
          format_opt = horizontal;
          توڑو;

        حالت 'A':
          ignore_منوال = IGNORE_DOT_AND_DOTDOT;
          توڑو;

        حالت 'B':
          جوڑو_ignore_نمونہ ("*~");
          جوڑو_ignore_نمونہ (".*~");
          توڑو;

        حالت 'C':
          format_opt = many_فی_سطر;
          توڑو;

        حالت 'D':
          format_opt = long_format;
          print_hyperlink = false;
          dired = true;
          توڑو;

        حالت 'F':
          {
            صحیح i;
            اگر (optarg)
              i = XARGMATCH ("--classify", optarg, when_دلائل, when_types);
            ورنہ
              /* Using --classify with no argument is equivalent to using
                 --classify=always.  */
              i = when_always;

            اگر (i == when_always || (i == when_if_tty && stdout_isatty ()))
              indicator_style = classify;
            توڑو;
          }

        حالت 'G':		/* inhibit display of group info */
          print_گروہ = false;
          توڑو;

        حالت 'H':
          dereference = DEREF_COMMAND_سطر_ARGUMENTS;
          توڑو;

        حالت DEREFERENCE_COMMAND_سطر_SYMLINK_مقصد_DIR_OPTION:
          dereference = DEREF_COMMAND_سطر_SYMLINK_مقصد_DIR;
          توڑو;

        حالت 'I':
          جوڑو_ignore_نمونہ (optarg);
          توڑو;

        حالت 'L':
          dereference = DEREF_ALWAYS;
          توڑو;

        حالت 'N':
          quoting_style_opt = literal_quoting_style;
          توڑو;

        حالت 'Q':
          quoting_style_opt = c_quoting_style;
          توڑو;

        حالت 'R':
          recursive = true;
          توڑو;

        حالت 'S':
          ترتیب_دو_opt = ترتیب_دو_سائز;
          توڑو;

        حالت 'T':
          tabsize_opt = xnumtoumax (optarg, 0, 0, MIN (PTRDIFF_MAX, SIZE_MAX),
                                    "", _("invalid tab size"), LS_FAILURE, 0);
          توڑو;

        حالت 'U':
          ترتیب_دو_opt = ترتیب_دو_none;
          توڑو;

        حالت 'X':
          ترتیب_دو_opt = ترتیب_دو_extension;
          توڑو;

        حالت '1':
          /* -1 has no effect after -l.  */
          اگر (format_opt != long_format)
            format_opt = one_فی_سطر;
          توڑو;

        حالت AUTHOR_OPTION:
          print_author = true;
          توڑو;

        حالت HIDE_OPTION:
          {
            ساخت ignore_نمونہ *hide = xmalloc (حجم *hide);
            hide->نمونہ_2 = optarg;
            hide->اگلا = hide_patterns;
            hide_patterns = hide;
          }
          توڑو;

        حالت ترتیب_دو_OPTION:
          ترتیب_دو_opt = XARGMATCH ("--sort", optarg, ترتیب_دو_دلائل, ترتیب_دو_types);
          توڑو;

        حالت گروہ_DIRECTORIES_پہلا_OPTION:
          directories_پہلا = true;
          توڑو;

        حالت وقت_OPTION:
          وقت_type = XARGMATCH ("--time", optarg, وقت_دلائل, وقت_types);
          explicit_وقت = true;
          توڑو;

        حالت FORMAT_OPTION:
          format_opt = XARGMATCH ("--format", optarg, format_دلائل,
                                  format_types);
          توڑو;

        حالت بھرا_وقت_OPTION:
          format_opt = long_format;
          وقت_style_option = (حرف *) "full-iso";
          توڑو;

        حالت COLOR_OPTION:
          {
            صحیح i;
            اگر (optarg)
              i = XARGMATCH ("--color", optarg, when_دلائل, when_types);
            ورنہ
              /* Using --color with no argument is equivalent to using
                 --color=always.  */
              i = when_always;

            print_with_color = (i == when_always
                                || (i == when_if_tty && stdout_isatty ()));
            توڑو;
          }

        حالت HYPERLINK_OPTION:
          {
            صحیح i;
            اگر (optarg)
              i = XARGMATCH ("--hyperlink", optarg, when_دلائل, when_types);
            ورنہ
              /* Using --hyperlink with no argument is equivalent to using
                 --hyperlink=always.  */
              i = when_always;

            print_hyperlink = (i == when_always
                               || (i == when_if_tty && stdout_isatty ()));
            توڑو;
          }

        حالت INDICATOR_STYLE_OPTION:
          indicator_style = XARGMATCH ("--indicator-style", optarg,
                                       indicator_style_دلائل,
                                       indicator_style_types);
          توڑو;

        حالت QUOTING_STYLE_OPTION:
          quoting_style_opt = XARGMATCH ("--quoting-style", optarg,
                                         quoting_style_دلائل,
                                         quoting_style_vals);
          توڑو;

        حالت وقت_STYLE_OPTION:
          وقت_style_option = optarg;
          توڑو;

        حالت SHOW_CONTROL_CHARS_OPTION:
          hide_control_chars_opt = false;
          توڑو;

        حالت بلاک_سائز_OPTION:
          {
            شمار strtol_error e = human_options (optarg, &human_output_opts,
                                                 &output_بلاک_سائز);
            اگر (e != LONGINT_OK)
              xstrtol_fatal (e, oi, 0, long_options, optarg);
            file_human_output_opts = human_output_opts;
            file_output_بلاک_سائز = output_بلاک_سائز;
          }
          توڑو;

        حالت SI_OPTION:
          file_human_output_opts = human_output_opts =
            human_autoscale | human_SI;
          file_output_بلاک_سائز = output_بلاک_سائز = 1;
          توڑو;

        حالت 'Z':
          print_scontext = true;
          توڑو;

        حالت ZERO_OPTION:
          eolbyte = 0;
          hide_control_chars_opt = false;
          اگر (format_opt != long_format)
            format_opt = one_فی_سطر;
          print_with_color = false;
          quoting_style_opt = literal_quoting_style;
          توڑو;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

        طےشدہ:
          usage (LS_FAILURE);
        }
    }

  اگر (! output_بلاک_سائز)
    {
      حرف ثابت *ls_بلاک_سائز = getenv ("LS_BLOCK_SIZE");
      human_options (ls_بلاک_سائز,
                     &human_output_opts, &output_بلاک_سائز);
      اگر (ls_بلاک_سائز || getenv ("BLOCK_SIZE"))
        {
          file_human_output_opts = human_output_opts;
          file_output_بلاک_سائز = output_بلاک_سائز;
        }
      اگر (kibibytes_specified)
        {
          human_output_opts = 0;
          output_بلاک_سائز = 1024;
        }
    }

  format = (0 <= format_opt ? format_opt
            : ls_منوال == LS_LS ? (stdout_isatty ()
                                  ? many_فی_سطر : one_فی_سطر)
            : ls_منوال == LS_MULTI_ستون ? many_فی_سطر
            : /* ls_mode == LS_LONG_FORMAT */ long_format);

  /* If the line length was not set by a switch but is needed to determine
     output, go to the work of obtaining it from the environment.  */
  ptrdiff_t linelen = چوڑائی_opt;
  اگر (format == many_فی_سطر || format == horizontal || format == with_commas
      || print_with_color)
    {
#اگر_تعریف_شدہ TIOCGWINSZ
      اگر (linelen < 0)
        {
          ساخت winsize ws;
          اگر (stdout_isatty ()
              && 0 <= ioctl (STDOUT_FILENO, TIOCGWINSZ, &ws)
              && 0 < ws.ws_ستون)
            linelen = ws.ws_ستون <= MIN (PTRDIFF_MAX, SIZE_MAX) ? ws.ws_ستون : 0;
        }
#اگر_ختم
      اگر (linelen < 0)
        {
          حرف ثابت *p = getenv ("COLUMNS");
          اگر (p && *p)
            {
              linelen = decode_سطر_لمبائی (p);
              اگر (linelen < 0)
                error (0, 0,
                       _("ignoring invalid width"
                         " in environment variable COLUMNS: %s"),
                       quote (p));
            }
        }
    }

  سطر_لمبائی = linelen < 0 ? 80 : linelen;

  /* Determine the max possible number of display columns.  */
  max_idx = سطر_لمبائی / MIN_عمود_چوڑائی;
  /* Account for first display column not having a separator,
     or line_lengths shorter than MIN_COLUMN_WIDTH.  */
  max_idx += سطر_لمبائی % MIN_عمود_چوڑائی != 0;

  اگر (format == many_فی_سطر || format == horizontal || format == with_commas)
    {
      اگر (0 <= tabsize_opt)
        tabsize = tabsize_opt;
      ورنہ
        {
          tabsize = 8;
          حرف ثابت *p = getenv ("TABSIZE");
          اگر (p)
            {
              uintmax_t tmp;
              اگر (xstrtoumax (p, تہی, 0, &tmp, "") == LONGINT_OK
                  && tmp <= SIZE_MAX)
                tabsize = tmp;
              ورنہ
                error (0, 0,
                       _("ignoring invalid tab size"
                         " in environment variable TABSIZE: %s"),
                       quote (p));
            }
        }
    }

  qmark_funny_chars = (hide_control_chars_opt < 0
                       ? ls_منوال == LS_LS && stdout_isatty ()
                       : hide_control_chars_opt);

  qmark_newline = hide_control_chars_opt != false && eolbyte == '\n'
                  && ((سطر_لمبائی
                       && (format == with_commas
                           || format == many_فی_سطر || format == horizontal))
                      || format == one_فی_سطر || format == long_format);

  صحیح qs = quoting_style_opt;
  اگر (qs < 0)
    qs = getenv_quoting_style ();
  اگر (qs < 0)
    qs = (ls_منوال == LS_LS
          ? (stdout_isatty () ? shell_escape_quoting_style : -1)
          : escape_quoting_style);
  اگر (0 <= qs)
    سیٹ_quoting_style (تہی, qs);
  qs = get_quoting_style (تہی);
  align_variable_outer_quotes
    = ((format == long_format
        || ((format == many_فی_سطر || format == horizontal) && سطر_لمبائی))
       && (qs == shell_quoting_style
           || qs == shell_escape_quoting_style
           || qs == c_maybe_quoting_style));
  filename_quoting_options = clone_quoting_options (تہی);
  اگر (qs == escape_quoting_style)
    سیٹ_char_quoting (filename_quoting_options, ' ', 1);
  اگر (file_type <= indicator_style)
    {
      برائے (حرف ثابت *p = &"*=>@|"[indicator_style - file_type]; *p; p++)
        سیٹ_char_quoting (filename_quoting_options, *p, 1);
    }
  اگر (format == with_commas
      && (qs == shell_quoting_style
          || qs == shell_escape_quoting_style
          || qs == c_maybe_quoting_style
          || qs == escape_quoting_style))
    سیٹ_char_quoting (filename_quoting_options, ',', 1);

  dirname_quoting_options = clone_quoting_options (تہی);
  سیٹ_char_quoting (dirname_quoting_options, ':', 1);

  /* --dired implies --format=long (-l) and sans --hyperlink.
     So ignore it if those overridden.  */
  dired &= (format == long_format) & !print_hyperlink;

  اگر (eolbyte < dired)
    error (LS_FAILURE, 0, _("--dired and --zero are incompatible"));

  /* If a time type is explicitly specified (with -c, -u, or --time=)
     and we're not showing a time (-l not specified), then sort by that time,
     rather than by name.  Note this behavior is unspecified by POSIX.  */

  ترتیب_دو_type = (0 <= ترتیب_دو_opt ? ترتیب_دو_opt
               : (format != long_format && explicit_وقت)
               ? ترتیب_دو_وقت : ترتیب_دو_name);

  اگر (format == long_format)
    {
      حرف *envstyle = تہی;
      حرف *style = وقت_style_option;
      اگر (! style)
        style = envstyle = getenv ("TIME_STYLE");

      اگر (! style)
        style = (حرف *) "locale";
      ورنہ
        {
          ساکن حرف ثابت posix_سابقہ[] = "posix-";
          جب (STREQ_LEN (style, posix_سابقہ, حجم posix_سابقہ - 1))
            {
              اگر (! hard_locale (LC_وقت))
                واپس optind;
              style += حجم posix_سابقہ - 1;
            }
        }

      اگر (*style == '+')
        {
          حرف *p0 = style + 1;
          حرف *p0nl = strchr (p0, '\n');
          حرف ثابت *p1 = p0;
          اگر (p0nl)
            {
              p1 = p0nl + 1;
              اگر (strchr (p1, '\n'))
                error (LS_FAILURE, 0, _("invalid time style format %s"),
                       quote (p0));
              اگر (envstyle)
                p0 = xmemdup0 (p0, p0nl - p0);
              ورنہ
                *p0nl = '\0';
            }
          long_وقت_format[0] = p0;
          long_وقت_format[1] = p1;
        }
      ورنہ
        {
          انتخاب (x_timestyle_مطابقت (style, /*allow_posix=*/ true,
                                     وقت_style_دلائل,
                                     (حرف ثابت *) وقت_style_types,
                                     حجم (*وقت_style_types), LS_FAILURE))
            {
            حالت بھرا_iso_وقت_style:
              long_وقت_format[0] = long_وقت_format[1] =
                "%Y-%m-%d %H:%M:%S.%N %z";
              توڑو;

            حالت long_iso_وقت_style:
              long_وقت_format[0] = long_وقت_format[1] = "%Y-%m-%d %H:%M";
              توڑو;

            حالت iso_وقت_style:
              long_وقت_format[0] = "%Y-%m-%d ";
              long_وقت_format[1] = "%m-%d %H:%M";
              توڑو;

            حالت locale_وقت_style:
              اگر (hard_locale (LC_وقت))
                {
                  برائے (صحیح i = 0; i < 2; i++)
                    long_وقت_format[i] =
                      dcgettext (تہی, long_وقت_format[i], LC_وقت);
                }
            }
        }

      abformat_init ();
    }

  واپس optind;
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

ساکن bool
get_funky_string (حرف **dest, حرف ثابت **src, bool equals_انتہا,
                  size_t *output_گنتی)
{
  حرف عدد;			/* For numerical codes */
  size_t گنتی;			/* Something to count with */
  شمار {
    ST_GND, ST_BACKSLASH, ST_OCTAL, ST_HEX, ST_CARET, ST_انتہا, ST_ERROR
  } state;
  حرف ثابت *p;
  حرف *q;

  p = *src;			/* We don't want to double-indirect */
  q = *dest;			/* the whole darn time.  */

  گنتی = 0;			/* No characters counted in yet.  */
  عدد = 0;

  state = ST_GND;		/* Start in ground state.  */
  جب (state < ST_انتہا)
    {
      انتخاب (state)
        {
        حالت ST_GND:		/* Ground state (no escapes) */
          انتخاب (*p)
            {
            حالت ':':
            حالت '\0':
              state = ST_انتہا;	/* End of string */
              توڑو;
            حالت '\\':
              state = ST_BACKSLASH; /* Backslash escape sequence */
              ++p;
              توڑو;
            حالت '^':
              state = ST_CARET; /* Caret escape */
              ++p;
              توڑو;
            حالت '=':
              اگر (equals_انتہا)
                {
                  state = ST_انتہا; /*انتہا*/
                  توڑو;
                }
              FALLTHROUGH;
            طےشدہ:
              *(q++) = *(p++);
              ++گنتی;
              توڑو;
            }
          توڑو;

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
              عدد = *p - '0';
              توڑو;
            حالت 'x':
            حالت 'X':
              state = ST_HEX;	/* Hex sequence */
              عدد = 0;
              توڑو;
            حالت 'a':		/* Bell */
              عدد = '\a';
              توڑو;
            حالت 'b':		/* Backspace */
              عدد = '\b';
              توڑو;
            حالت 'e':		/* Escape */
              عدد = 27;
              توڑو;
            حالت 'f':		/* Form feed */
              عدد = '\f';
              توڑو;
            حالت 'n':		/* Newline */
              عدد = '\n';
              توڑو;
            حالت 'r':		/* Carriage return */
              عدد = '\r';
              توڑو;
            حالت 't':		/* Tab */
              عدد = '\t';
              توڑو;
            حالت 'v':		/* Vtab */
              عدد = '\v';
              توڑو;
            حالت '?':		/* Delete */
              عدد = 127;
              توڑو;
            حالت '_':		/* Space */
              عدد = ' ';
              توڑو;
            حالت '\0':		/* End of string */
              state = ST_ERROR;	/* Error! */
              توڑو;
            طےشدہ:		/* Escaped character like \ ^ : = */
              عدد = *p;
              توڑو;
            }
          اگر (state == ST_BACKSLASH)
            {
              *(q++) = عدد;
              ++گنتی;
              state = ST_GND;
            }
          ++p;
          توڑو;

        حالت ST_OCTAL:		/* Octal sequence */
          اگر (*p < '0' || *p > '7')
            {
              *(q++) = عدد;
              ++گنتی;
              state = ST_GND;
            }
          ورنہ
            عدد = (عدد << 3) + (*(p++) - '0');
          توڑو;

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
              عدد = (عدد << 4) + (*(p++) - '0');
              توڑو;
            حالت 'a':
            حالت 'b':
            حالت 'c':
            حالت 'd':
            حالت 'e':
            حالت 'f':
              عدد = (عدد << 4) + (*(p++) - 'a') + 10;
              توڑو;
            حالت 'A':
            حالت 'B':
            حالت 'C':
            حالت 'D':
            حالت 'E':
            حالت 'F':
              عدد = (عدد << 4) + (*(p++) - 'A') + 10;
              توڑو;
            طےشدہ:
              *(q++) = عدد;
              ++گنتی;
              state = ST_GND;
              توڑو;
            }
          توڑو;

        حالت ST_CARET:		/* Caret escape */
          state = ST_GND;	/* Should be the next state... */
          اگر (*p >= '@' && *p <= '~')
            {
              *(q++) = *(p++) & 037;
              ++گنتی;
            }
          ورنہ اگر (*p == '?')
            {
              *(q++) = 127;
              ++گنتی;
            }
          ورنہ
            state = ST_ERROR;
          توڑو;

        حالت ST_انتہا: حالت ST_ERROR: طےشدہ:
          affirm (false);
        }
    }

  *dest = q;
  *src = p;
  *output_گنتی = گنتی;

  واپس state != ST_ERROR;
}

شمار parse_state
  {
    PS_آغاز = 1,
    PS_2,
    PS_3,
    PS_4,
    PS_مکمل,
    PS_FAIL
  };


/* Check if the content of TERM is a valid name in dircolors.  */

ساکن bool
معلوم_رکن_type (خالی)
{
  حرف ثابت *رکن = getenv ("TERM");
  اگر (! رکن || ! *رکن)
    واپس false;

  حرف ثابت *سطر = G_سطر;
  جب (سطر - G_سطر < حجم (G_سطر))
    {
      اگر (STRNCMP_LIT (سطر, "TERM ") == 0)
        {
          اگر (fnmatch (سطر + 5, رکن, 0) == 0)
            واپس true;
        }
      سطر += لڑی_لمبائی (سطر) + 1;
    }

  واپس false;
}

ساکن خالی
parse_ls_color (خالی)
{
  حرف ثابت *p;		/* Pointer to character being parsed */
  حرف *buf;			/* color_buf buffer pointer */
  حرف label0, label1;		/* Indicator label */
  ساخت color_ext_type *ext;	/* Extension we are working on */

  اگر ((p = getenv ("LS_COLORS")) == تہی || *p == '\0')
    {
      /* LS_COLORS takes precedence, but if that's not set then
         honor the COLORTERM and TERM env variables so that
         we only go with the internal ANSI color codes if the
         former is non empty or the latter is set to a known value.  */
      حرف ثابت *colorterm = getenv ("COLORTERM");
      اگر (! (colorterm && *colorterm) && ! معلوم_رکن_type ())
        print_with_color = false;
      واپس;
    }

  ext = تہی;

  /* This is an overly conservative estimate, but any possible
     LS_COLORS string will *not* generate a color_buf longer than
     itself, so it is a safe way of allocating a buffer in
     advance.  */
  buf = color_buf = xstrdup (p);

  شمار parse_state state = PS_آغاز;
  جب (true)
    {
      انتخاب (state)
        {
        حالت PS_آغاز:		/* First label character */
          انتخاب (*p)
            {
            حالت ':':
              ++p;
              توڑو;

            حالت '*':
              /* Allocate new extension block and add to head of
                 linked list (this way a later definition will
                 override an earlier one, which can be useful for
                 having terminal-specific defs override global).  */

              ext = xmalloc (حجم *ext);
              ext->اگلا = color_ext_list;
              color_ext_list = ext;
              ext->exact_مطابقت = false;

              ++p;
              ext->ext.string = buf;

              state = (get_funky_string (&buf, &p, true, &ext->ext.len)
                       ? PS_4 : PS_FAIL);
              توڑو;

            حالت '\0':
              state = PS_مکمل;	/* Done! */
              جاؤ مکمل;

            طےشدہ:	/* Assume it is file type label */
              label0 = *p++;
              state = PS_2;
              توڑو;
            }
          توڑو;

        حالت PS_2:		/* Second label character */
          اگر (*p)
            {
              label1 = *p++;
              state = PS_3;
            }
          ورنہ
            state = PS_FAIL;	/* Error */
          توڑو;

        حالت PS_3:		/* Equal sign after indicator label */
          state = PS_FAIL;	/* Assume failure...  */
          اگر (*(p++) == '=')/* It *should* be...  */
            {
              برائے (صحیح i = 0; i < countof (indicator_name); i++)
                {
                  اگر ((label0 == indicator_name[i][0])
                      && (label1 == indicator_name[i][1]))
                    {
                      color_indicator[i].string = buf;
                      state = (get_funky_string (&buf, &p, false,
                                                 &color_indicator[i].len)
                               ? PS_آغاز : PS_FAIL);
                      توڑو;
                    }
                }
              اگر (state == PS_FAIL)
                error (0, 0, _("unrecognized prefix: %s"),
                       quote ((حرف []) {label0, label1, '\0'}));
            }
          توڑو;

        حالت PS_4:		/* Equal sign after *.ext */
          اگر (*(p++) == '=')
            {
              ext->seq.string = buf;
              state = (get_funky_string (&buf, &p, false, &ext->seq.len)
                       ? PS_آغاز : PS_FAIL);
            }
          ورنہ
            state = PS_FAIL;
          توڑو;

        حالت PS_FAIL:
          جاؤ مکمل;

        حالت PS_مکمل: طےشدہ:
          affirm (false);
        }
    }
 مکمل:

  اگر (state == PS_FAIL)
    {
      error (0, 0,
             _("unparsable value for LS_COLORS environment variable"));
      آزاد (color_buf);
      برائے (ساخت color_ext_type *e = color_ext_list; e != تہی;
           /*خالی_خانے*/)
        {
          ساخت color_ext_type *e2 = e;
          e = e->اگلا;
          آزاد (e2);
        }
      print_with_color = false;
      واپس;
    }
  ورنہ
    {
      /* Postprocess list to set EXACT_MATCH on entries where there are
         different cased extensions with separate sequences defined.
         Also set ext.len to SIZE_MAX on any entries that can't
         match due to precedence, to avoid redundant string compares.  */
      برائے (ساخت color_ext_type *e1 = color_ext_list; e1 != تہی;
           e1 = e1->اگلا)
        {
          bool case_ignored = false;

          برائے (ساخت color_ext_type *e2 = e1->اگلا; e2 != تہی;
               e2 = e2->اگلا)
            {
              اگر (e2->ext.len < SIZE_MAX && e1->ext.len == e2->ext.len)
                {
                  اگر (memeq (e1->ext.string, e2->ext.string, e1->ext.len))
                    e2->ext.len = SIZE_MAX; /* Ignore */
                  ورنہ اگر (c_strncasecmp (e1->ext.string, e2->ext.string,
                                          e1->ext.len) == 0)
                    {
                      اگر (case_ignored)
                        {
                          e2->ext.len = SIZE_MAX; /* Ignore */
                        }
                      ورنہ اگر (e1->seq.len == e2->seq.len
                               && memeq (e1->seq.string, e2->seq.string,
                                         e1->seq.len))
                        {
                          e2->ext.len = SIZE_MAX; /* Ignore */
                          case_ignored = true;    /* Ignore all subsequent */
                        }
                      ورنہ
                        {
                          e1->exact_مطابقت = true;
                          e2->exact_مطابقت = true;
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

ساکن خالی
سیٹ_exit_حالت (bool serious)
{
  اگر (serious)
    exit_حالت = LS_FAILURE;
  ورنہ اگر (exit_حالت == EXIT_SUCCESS)
    exit_حالت = LS_MINOR_PROBLEM;
}

/* Assuming a failure is serious if SERIOUS, use the printf-style
   MESSAGE to report the failure to access a file named FILE.  Assume
   errno is set appropriately for the failure.  */

ساکن خالی
file_failure (bool serious, حرف ثابت *پیغام_2, حرف ثابت *file)
{
  error (0, errno, پیغام_2, quoteaf (file));
  سیٹ_exit_حالت (serious);
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

ساکن خالی
قطار_directory (حرف ثابت *name, حرف ثابت *realname, bool command_سطر_arg)
{
  ساخت pending *new = xmalloc (حجم *new);
  new->realname = realname ? xstrdup (realname) : تہی;
  new->name = name ? xstrdup (name) : تہی;
  new->command_سطر_arg = command_سطر_arg;
  new->اگلا = pending_dirs;
  pending_dirs = new;
}

/* Read directory NAME, and list the files in it.
   If REALNAME is nonzero, print its name instead of NAME;
   this is used for symbolic links to directories.
   COMMAND_LINE_ARG means this directory was mentioned on the command line.  */

ساکن خالی
print_dir (حرف ثابت *name, حرف ثابت *realname, bool command_سطر_arg)
{
  DIR *dirp;
  ساخت dirent *اگلا;
  uintmax_t کل_بلاکیں = 0;
  ساکن bool پہلا = true;

  errno = 0;
  dirp = opendir (name);
  اگر (!dirp)
    {
      اگر (command_سطر_arg || ! ignorable_traversal_errno (errno))
        file_failure (command_سطر_arg, _("cannot open directory %s"), name);
      واپس;
    }

  اگر (LOOP_DETECT)
    {
      ساخت stat dir_stat;
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
          واپس;
        }

      /* If we've already visited this dev/inode pair, warn that
         we've found a loop, and do not process this directory.  */
      اگر (دورہ_dir (dir_stat.st_dev, dir_stat.st_ino))
        {
          error (0, 0, _("%s: not listing already-listed directory"),
                 quotef (name));
          closedir (dirp);
          سیٹ_exit_حالت (true);
          واپس;
        }

      dev_ino_ڈالو (dir_stat.st_dev, dir_stat.st_ino);
    }

  clear_files ();

  اگر (recursive || print_dir_name)
    {
      اگر (!پہلا)
        dired_outbyte ('\n');
      پہلا = false;
      dired_indent ();

      حرف *absolute_name = تہی;
      اگر (print_hyperlink)
        {
          absolute_name = canonicalize_filename_منوال (name, CAN_MISSING);
          اگر (! absolute_name
              && ! ignorable_traversal_errno (errno))
            file_failure (command_سطر_arg,
                          _("error canonicalizing %s"), name);
        }
      quote_name (realname ? realname : name, dirname_quoting_options, -1,
                  تہی, true, &subdired_obstack, absolute_name);

      آزاد (absolute_name);

      dired_outstring (":\n");
    }

  /* Read the directory entries, and insert the subfiles into the 'cwd_file'
     table.  */

  جب (true)
    {
      /* Set errno to zero so we can distinguish between a readdir failure
         and when readdir simply finds that there are no more entries.  */
      errno = 0;
      اگلا = readdir (dirp);
      اگر (اگلا)
        {
          اگر (! file_ignored (اگلا->d_name))
            {
              شمار filetype type;
#اگر HAVE_STRUCT_DIRENT_D_TYPE
              type = d_type_filetype[اگلا->d_type];
#ورنہ
              type = unknown;
#اگر_ختم
              کل_بلاکیں += gobble_file (اگلا->d_name, type,
                                           RELIABLE_D_INO (اگلا),
                                           false, name);

              /* In this narrow case, print out each name right away, so
                 ls uses constant memory while processing the entries of
                 this directory.  Useful when there are many (millions)
                 of entries in a directory.  */
              اگر (format == one_فی_سطر && ترتیب_دو_type == ترتیب_دو_none
                      && !print_بلاک_سائز && !recursive)
                {
                  /* We must call sort_files in spite of
                     "sort_type == sort_none" for its initialization
                     of the sorted_file vector.  */
                  ترتیب_دو_files ();
                  print_current_files ();
                  clear_files ();
                }
            }
        }
      ورنہ
        {
          صحیح err = errno;
          اگر (err == 0)
            توڑو;
          /* Ignore errors indicating that the directory was removed.  */
          اگر (ignorable_traversal_errno (err))
            توڑو;
          file_failure (command_سطر_arg, _("reading directory %s"), name);
          اگر (err != EOVERFLOW)
            توڑو;
        }

      /* When processing a very large directory, and since we've inhibited
         interrupts, this loop would take so long that ls would be annoyingly
         uninterruptible.  This ensures that it handles signals promptly.  */
      process_اشارات ();
    }

  اگر (closedir (dirp) != 0)
    {
      file_failure (command_سطر_arg, _("closing directory %s"), name);
      /* Don't return; print whatever we got.  */
    }

  /* Sort the directory contents.  */
  ترتیب_دو_files ();

  /* If any member files are subdirectories, perhaps they should have their
     contents listed rather than being mentioned here as files.  */

  اگر (recursive)
    نکالو_dirs_from_files (name, false);

  اگر (format == long_format || print_بلاک_سائز)
    {
      حرف buf[LONGEST_HUMAN_READABLE + 3];
      حرف *p = human_readable (کل_بلاکیں, buf + 1, human_output_opts,
                                ST_NBLOCKSIZE, output_بلاک_سائز);
      حرف *pend = p + لڑی_لمبائی (p);
      *--p = ' ';
      *pend++ = eolbyte;
      dired_indent ();
      dired_outstring (_("total"));
      dired_outbuf (p, pend - p);
    }

  اگر (cwd_n_مستعمل)
    print_current_files ();
}

/* Add 'pattern' to the list of patterns for which files that match are
   not listed.  */

ساکن خالی
جوڑو_ignore_نمونہ (حرف ثابت *نمونہ_2)
{
  ساخت ignore_نمونہ *ignore;

  ignore = xmalloc (حجم *ignore);
  ignore->نمونہ_2 = نمونہ_2;
  /* Add it to the head of the linked list.  */
  ignore->اگلا = ignore_patterns;
  ignore_patterns = ignore;
}

/* Return true if one of the PATTERNS matches FILE.  */

ساکن bool
patterns_مطابقت (ساخت ignore_نمونہ ثابت *patterns, حرف ثابت *file)
{
  برائے (ساخت ignore_نمونہ ثابت *p = patterns; p; p = p->اگلا)
    اگر (fnmatch (p->نمونہ_2, file, FNM_PERIOD) == 0)
      واپس true;
  واپس false;
}

/* Return true if FILE should be ignored.  */

ساکن bool
file_ignored (حرف ثابت *name)
{
  واپس ((ignore_منوال != IGNORE_اقل
           && name[0] == '.'
           && (ignore_منوال == IGNORE_DEFAULT || ! name[1 + (name[1] == '.')]))
          || (ignore_منوال == IGNORE_DEFAULT
              && patterns_مطابقت (hide_patterns, name))
          || patterns_مطابقت (ignore_patterns, name));
}

/* POSIX requires that a file size be printed without a sign, even
   when negative.  Assume the typical case where negative sizes are
   actually positive values that have wrapped around.  */

ساکن uintmax_t
unsigned_file_سائز (off_t سائز)
{
  واپس سائز + (سائز < 0) * ((uintmax_t) OFF_T_MAX - OFF_T_MIN + 1);
}

#اگر_تعریف_شدہ HAVE_CAP
/* Return true if NAME has a capability (see linux/capability.h) */
ساکن bool
has_capability (حرف ثابت *name)
{
  حرف *نتیجہ;
  bool has_cap;

  cap_t cap_d = cap_get_file (name);
  اگر (cap_d == تہی)
    واپس false;

  نتیجہ = cap_مقصد_متن (cap_d, تہی);
  cap_free (cap_d);
  اگر (!نتیجہ)
    واپس false;

  /* check if human-readable capability string is empty */
  has_cap = !!*نتیجہ;

  cap_free (نتیجہ);
  واپس has_cap;
}
#ورنہ
ساکن bool
has_capability (MAYBE_غیر_مستعمل حرف ثابت *name)
{
  errno = ENOTSUP;
  واپس false;
}
#اگر_ختم

/* Enter and remove entries in the table 'cwd_file'.  */

ساکن خالی
free_ent (ساخت fileinfo *f)
{
  آزاد (f->name);
  آزاد (f->linkname);
  آزاد (f->absolute_name);
  اگر (f->scontext != UNKNOWN_SECURITY_CONTEXT)
    aclinfo_scontext_free (f->scontext);
}

/* Empty the table of files.  */
ساکن خالی
clear_files (خالی)
{
  برائے (idx_t i = 0; i < cwd_n_مستعمل; i++)
    {
      ساخت fileinfo *f = sorted_file[i];
      free_ent (f);
    }

  cwd_n_مستعمل = 0;
  cwd_some_quoted = false;
  any_has_acl = false;
  inode_نمبر_چوڑائی = 0;
  بلاک_سائز_چوڑائی = 0;
  nlink_چوڑائی = 0;
  owner_چوڑائی = 0;
  گروہ_چوڑائی = 0;
  author_چوڑائی = 0;
  scontext_چوڑائی = 0;
  major_device_نمبر_چوڑائی = 0;
  minor_device_نمبر_چوڑائی = 0;
  file_سائز_چوڑائی = 0;
}

/* Cache file_has_aclinfo failure, when it's trivial to do.
   Like file_has_aclinfo, but when F's st_dev says it's on a file
   system lacking ACL support, return 0 with ENOTSUP immediately.  */
ساکن صحیح
file_has_aclinfo_cache (حرف ثابت *file, ساخت fileinfo *f,
                        ساخت aclinfo *ai, صحیح flags)
{
  /* If UNSUPPORTED_SCONTEXT, these variables hold the
     st_dev and associated info for the most recently processed device
     for which file_has_aclinfo failed indicating lack of support.  */
  ساکن صحیح unsupported_return;
  ساکن حرف *unsupported_scontext;
  ساکن صحیح unsupported_scontext_err;
  ساکن dev_t unsupported_device;

  اگر (f->stat_ok && unsupported_scontext
      && f->stat.st_dev == unsupported_device)
    {
      ai->buf = ai->u._gl_acl_ch;
      ai->سائز = 0;
      ai->scontext = unsupported_scontext;
      ai->scontext_err = unsupported_scontext_err;
      errno = ENOTSUP;
      واپس unsupported_return;
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
          || (ai->سائز < 0 && !acl_errno_valid (ai->u.err)))
      && (!(flags & ACL_GET_SCONTEXT) || !acl_errno_valid (ai->scontext_err)))
    {
      /* FIXME: acl_errno_valid() may be a bit too broad determine file system
         non support.  E.g. EBUSY was added for per file reasons (on cygwin). */
      unsupported_return = n;
      unsupported_scontext = ai->scontext;
      unsupported_scontext_err = ai->scontext_err;
      unsupported_device = f->stat.st_dev;
    }
  واپس n;
}

/* Cache has_capability failure, when it's trivial to do.
   Like has_capability, but when F's st_dev says it's on a file
   system lacking capability support, return 0 with ENOTSUP immediately.  */
ساکن bool
has_capability_cache (حرف ثابت *file, ساخت fileinfo *f)
{
  /* If UNSUPPORTED_CACHED, UNSUPPORTED_DEVICE is the cached
     st_dev of the most recently processed device for which we've
     found that has_capability fails indicating lack of support.  */
  ساکن bool unsupported_cached;
  ساکن dev_t unsupported_device;

  اگر (f->stat_ok && unsupported_cached && f->stat.st_dev == unsupported_device)
    {
      errno = ENOTSUP;
      واپس 0;
    }

  bool b = has_capability (file);
  اگر (f->stat_ok && !b && !acl_errno_valid (errno))
    {
      unsupported_cached = true;
      unsupported_device = f->stat.st_dev;
    }
  واپس b;
}

/* Get the ACL, security context, and capability information needed for F.
   FULL_NAME names the file; DO_DEREF says whether to follow symlinks.  */
ساکن خالی
get_extended_info (حرف ثابت *بھرا_name, ساخت fileinfo *f, bool do_deref)
{
  /* FIXME: file_has_aclinfo() conflates capability and acl xattr lookup
     with security context attributes.  Ideally we'd have separate
     ACL_GET_{CAPABILITY,ACL} flags to lookup just the required items,
     simplifying the logic here.  */

  شمار filetype type = f->filetype;
  bool check_acl = USE_ACL && format == long_format;
  bool get_scontext = false;
#اگر HAVE_SMACK || USE_SELINUX_SELINUX_H
  get_scontext = (format == long_format) | print_scontext;
#اگر_ختم
  bool check_capability = format_needs_capability & (type == معمول);

  اگر (type != arg_directory && (check_acl | get_scontext | check_capability))
    {
      ساخت aclinfo ai;
      /* Without ACL support, request contexts to obtain the attribute
         names needed for capability coloring too.  */
      صحیح aclinfo_flags = ((do_deref ? ACL_SYMLINK_FOLLOW : 0)
                           | (get_scontext || (!USE_ACL && check_capability)
                              ? ACL_GET_SCONTEXT : 0)
                           | filetype_d_type[type]);
      صحیح n = file_has_aclinfo_cache (بھرا_name, f, &ai, aclinfo_flags);
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
        error (0, errno, "%s", quotef (بھرا_name));
      ورنہ
        {
          /* When requesting security context information, don't make
             ls fail just because the file (even a command line argument)
             isn't on the right type of file system.  I.e., a getfilecon
             failure isn't in the same class as a stat failure.  */
          اگر (print_scontext && ai.scontext_err
              && ! ignorable_traversal_errno (ai.scontext_err)
              && (! (is_ENOTSUP (ai.scontext_err)
                     || ai.scontext_err == ENODATA)))
            error (0, ai.scontext_err, "%s", quotef (بھرا_name));
        }

      /* has_capability adds around 30% runtime to 'ls --color',
         so call it only if really needed.  Note capability coloring
         is disabled in the default color config.  */
      اگر (check_capability && aclinfo_has_xattr (&ai, XATTR_NAME_CAPS))
        f->has_capability = has_capability_cache (بھرا_name, f);

      f->scontext = ai.scontext;
      ai.scontext = تہی;
      aclinfo_free (&ai);
    }
}

ساکن bool
needs_quoting (حرف ثابت *name)
{
  حرف test[2];
  size_t len = quotearg_بفر (test, حجم test , name, -1,
                                filename_quoting_options);
  واپس *name != *test || لڑی_لمبائی (name) != len;
}

/* Add a file to the current table of files.
   Verify that the file exists, and print an error message if it does not.
   Return the number of blocks that the file occupies.  */
ساکن uintmax_t
gobble_file (حرف ثابت *name, شمار filetype type, ino_t inode,
             bool command_سطر_arg, حرف ثابت *dirname)
{
  uintmax_t بلاکیں = 0;
  ساخت fileinfo *f;

  /* An inode value prior to gobble_file necessarily came from readdir,
     which is not used for command line arguments.  */
  affirm (! command_سطر_arg || inode == NOT_AN_INODE_نمبر);

  اگر (cwd_n_مستعمل == cwd_n_alloc)
    cwd_file = xpalloc (cwd_file, &cwd_n_alloc, 1, -1, حجم *cwd_file);

  f = &cwd_file[cwd_n_مستعمل];
  memset (f, '\0', حجم *f);
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
              || color_symlink_as_referent || check_symlink_منوال))
      /* Command line dereferences are already taken care of by the above
         assertion that the inode number is not yet known.  */
      || (print_inode && inode == NOT_AN_INODE_نمبر)
      /* --indicator-style=classify (aka -F) requires statting each
         regular file to see whether it's executable.  */
      || ((type == معمول || type == unknown)
          && (indicator_style == classify
              /* This is so that --color ends up highlighting files with these
                 mode bits set even when options like -F are not specified.  */
              || (print_with_color && (is_colored (C_EXEC)
                                       || is_colored (C_SETUID)
                                       || is_colored (C_SETGID))))));

  /* Absolute name of this file, if needed.  */
  حرف ثابت *بھرا_name = name;
  اگر (check_stat | print_scontext | format_needs_capability
      && name[0] != '/' && dirname)
    {
      حرف *p = alloca (لڑی_لمبائی (name) + لڑی_لمبائی (dirname) + 2);
      attach (p, dirname, name);
      بھرا_name = p;
    }

  bool do_deref;

  اگر (!check_stat)
    do_deref = dereference == DEREF_ALWAYS;
  ورنہ
    {
      صحیح err;

      اگر (print_hyperlink)
        {
          f->absolute_name = canonicalize_filename_منوال (بھرا_name,
                                                         CAN_MISSING);
          اگر (! f->absolute_name
              && (command_سطر_arg
                  || ! ignorable_traversal_errno (errno)))
            file_failure (command_سطر_arg,
                          _("error canonicalizing %s"), بھرا_name);
        }

      انتخاب (dereference)
        {
        حالت DEREF_ALWAYS:
          err = do_stat (بھرا_name, &f->stat);
          do_deref = true;
          توڑو;

        حالت DEREF_COMMAND_سطر_ARGUMENTS:
        حالت DEREF_COMMAND_سطر_SYMLINK_مقصد_DIR:
          اگر (command_سطر_arg)
            {
              bool need_lstat;
              err = do_stat (بھرا_name, &f->stat);
              do_deref = true;

              اگر (dereference == DEREF_COMMAND_سطر_ARGUMENTS)
                توڑو;

              need_lstat = (err < 0
                            ? (errno == ENOENT || errno == ELOOP)
                            : ! S_ISDIR (f->stat.st_منوال));
              اگر (!need_lstat)
                توڑو;

              /* stat failed because of ENOENT || ELOOP, maybe indicating a
                 non-traversable symlink.  Or stat succeeded,
                 FULL_NAME does not refer to a directory,
                 and --dereference-command-line-symlink-to-dir is in effect.
                 Fall through so that we call lstat instead.  */
            }
          FALLTHROUGH;

        حالت DEREF_NEVER:
          err = do_lstat (بھرا_name, &f->stat);
          do_deref = false;
          توڑو;

        حالت DEREF_UNDEFINED: طےشدہ:
          affirm (false);
        }

      اگر (err != 0)
        {
          اگر (! command_سطر_arg && ignorable_traversal_errno (errno))
            {
              صحیح stat_errno = errno;
              ساخت stat linkstat;
              bool dangling_link = (do_deref
                                    && (type == symbolic_link
                                        || type == unknown)
                                    && do_lstat (بھرا_name, &linkstat) == 0);
              errno = stat_errno;
              اگر (! dangling_link)
                {
                  free_ent (f);
                  واپس 0;
                }
            }

          /* Failure to stat a command line argument leads to
             an exit status of 2.  For other files, stat failure
             provokes an exit status of 1.  */
          file_failure (command_سطر_arg,
                        _("cannot access %s"), بھرا_name);

          اگر (command_سطر_arg)
            {
              free_ent (f);
              واپس 0;
            }

          f->name = xstrdup (name);
          cwd_n_مستعمل++;

          واپس 0;
        }

      f->stat_ok = true;
      f->filetype = type = d_type_filetype[IFTODT (f->stat.st_منوال)];
    }

  اگر (type == directory && command_سطر_arg && !فوری_dirs)
    f->filetype = type = arg_directory;

  get_extended_info (بھرا_name, f, do_deref);

  اگر ((type == symbolic_link)
      & ((format == long_format) | check_symlink_منوال))
    {
      ساخت stat linkstats;

      اگر (! get_link_name (بھرا_name, f, command_سطر_arg))
        {
          free_ent (f);
          واپس 0;
        }

      /* Use the slower quoting path for this entry, though
         don't update CWD_SOME_QUOTED since alignment not affected.  */
      اگر (f->linkname && f->quoted == 0 && needs_quoting (f->linkname))
        f->quoted = -1;

      /* Avoid following symbolic links when possible, i.e., when
         they won't be traced and when no indicator is needed.  */
      اگر (f->linkname
          && (file_type <= indicator_style || check_symlink_منوال)
          && stat_for_منوال (بھرا_name, &linkstats) == 0)
        {
          f->linkok = true;
          f->linkmode = linkstats.st_منوال;
        }
    }

  بلاکیں = STP_NBLOCKS (&f->stat);
  اگر (format == long_format || print_بلاک_سائز)
    {
      حرف buf[LONGEST_HUMAN_READABLE + 1];
      صحیح len = mbswidth (human_readable (بلاکیں, buf, human_output_opts,
                                          ST_NBLOCKSIZE, output_بلاک_سائز),
                          MBSWIDTH_FLAGS);
      اگر (بلاک_سائز_چوڑائی < len)
        بلاک_سائز_چوڑائی = len;
    }

  اگر (format == long_format)
    {
      اگر (print_owner)
        {
          صحیح len = format_user_چوڑائی (f->stat.st_uid);
          اگر (owner_چوڑائی < len)
            owner_چوڑائی = len;
        }

      اگر (print_گروہ)
        {
          صحیح len = format_گروہ_چوڑائی (f->stat.st_gid);
          اگر (گروہ_چوڑائی < len)
            گروہ_چوڑائی = len;
        }

      اگر (print_author)
        {
          صحیح len = format_user_چوڑائی (f->stat.st_author);
          اگر (author_چوڑائی < len)
            author_چوڑائی = len;
        }
    }

  اگر (print_scontext)
    {
      صحیح len = لڑی_لمبائی (f->scontext);
      اگر (scontext_چوڑائی < len)
        scontext_چوڑائی = len;
    }

  اگر (format == long_format)
    {
      حرف b[INT_BUFSIZE_BOUND (uintmax_t)];
      صحیح b_len = لڑی_لمبائی (umaxtostr (f->stat.st_nlink, b));
      اگر (nlink_چوڑائی < b_len)
        nlink_چوڑائی = b_len;

      اگر ((type == chardev) | (type == blockdev))
        {
          حرف buf[INT_BUFSIZE_BOUND (uintmax_t)];
          صحیح len = لڑی_لمبائی (umaxtostr (major (f->stat.st_rdev), buf));
          اگر (major_device_نمبر_چوڑائی < len)
            major_device_نمبر_چوڑائی = len;
          len = لڑی_لمبائی (umaxtostr (minor (f->stat.st_rdev), buf));
          اگر (minor_device_نمبر_چوڑائی < len)
            minor_device_نمبر_چوڑائی = len;
          len = major_device_نمبر_چوڑائی + 2 + minor_device_نمبر_چوڑائی;
          اگر (file_سائز_چوڑائی < len)
            file_سائز_چوڑائی = len;
        }
      ورنہ
        {
          حرف buf[LONGEST_HUMAN_READABLE + 1];
          uintmax_t سائز = unsigned_file_سائز (f->stat.st_سائز);
          صحیح len = mbswidth (human_readable (سائز, buf,
                                              file_human_output_opts,
                                              1, file_output_بلاک_سائز),
                              MBSWIDTH_FLAGS);
          اگر (file_سائز_چوڑائی < len)
            file_سائز_چوڑائی = len;
        }
    }

  اگر (print_inode)
    {
      حرف buf[INT_BUFSIZE_BOUND (uintmax_t)];
      صحیح len = لڑی_لمبائی (umaxtostr (f->stat.st_ino, buf));
      اگر (inode_نمبر_چوڑائی < len)
        inode_نمبر_چوڑائی = len;
    }

  f->name = xstrdup (name);
  cwd_n_مستعمل++;

  واپس بلاکیں;
}

/* Return true if F refers to a directory.  */
ساکن bool
is_directory (ثابت ساخت fileinfo *f)
{
  واپس f->filetype == directory || f->filetype == arg_directory;
}

/* Return true if F refers to a (symlinked) directory.  */
ساکن bool
is_linked_directory (ثابت ساخت fileinfo *f)
{
  واپس f->filetype == directory || f->filetype == arg_directory
         || S_ISDIR (f->linkmode);
}

/* Put the name of the file that FILENAME is a symbolic link to
   into the LINKNAME field of 'f'.  COMMAND_LINE_ARG indicates whether
   FILENAME is a command-line argument.  */

ساکن bool
get_link_name (حرف ثابت *filename, ساخت fileinfo *f, bool command_سطر_arg)
{
  f->linkname = areadlink_with_سائز (filename, f->stat.st_سائز);
  اگر (f->linkname == تہی)
    {
      اگر (! command_سطر_arg
          && ignorable_traversal_errno (errno))
        واپس false;
      file_failure (command_سطر_arg, _("cannot read symbolic link %s"),
                    filename);
    }
  واپس true;
}

/* Return true if the last component of NAME is '.' or '..'
   This is so we don't try to recurse on '././././. ...' */

ساکن bool
basename_is_dot_or_dotdot (حرف ثابت *name)
{
  حرف ثابت *بنیاد_2 = آخری_component (name);
  واپس dot_or_dotdot (بنیاد_2);
}

/* Remove any entries from CWD_FILE that are for directories,
   and queue them to be listed as directories instead.
   DIRNAME is the prefix to prepend to each dirname
   to make it correct relative to ls's working dir;
   if it is null, no prefix is needed and "." and ".." should not be ignored.
   If COMMAND_LINE_ARG is true, this directory was mentioned at the top level,
   This is desirable when processing directories recursively.  */

ساکن خالی
نکالو_dirs_from_files (حرف ثابت *dirname, bool command_سطر_arg)
{
  idx_t i, j;
  bool ignore_dot_and_dot_dot = (dirname != تہی);

  اگر (dirname && LOOP_DETECT)
    {
      /* Insert a marker entry first.  When we dequeue this marker entry,
         we'll know that DIRNAME has been processed and may be removed
         from the set of active directories.  */
      قطار_directory (تہی, dirname, false);
    }

  /* Queue the directories last one first, because queueing reverses the
     order.  */
  برائے (i = cwd_n_مستعمل; 0 < i; )
    {
      i--;
      ساخت fileinfo *f = sorted_file[i];

      اگر (is_directory (f)
          && (! ignore_dot_and_dot_dot
              || ! basename_is_dot_or_dotdot (f->name)))
        {
          اگر (!dirname || f->name[0] == '/')
            قطار_directory (f->name, f->linkname, command_سطر_arg);
          ورنہ
            {
              حرف *name = file_name_concat (dirname, f->name, تہی);
              قطار_directory (name, f->linkname, command_سطر_arg);
              آزاد (name);
            }
          اگر (f->filetype == arg_directory)
            free_ent (f);
        }
    }

  /* Now delete the directories from the table, compacting all the remaining
     entries.  */

  برائے (i = 0, j = 0; i < cwd_n_مستعمل; i++)
    {
      ساخت fileinfo *f = sorted_file[i];
      sorted_file[j] = f;
      j += (f->filetype != arg_directory);
    }
  cwd_n_مستعمل = j;
}

/* Use strcoll to compare strings in this locale.  If an error occurs,
   report an error and longjmp to failed_strcoll.  */

ساکن jmp_buf failed_strcoll;

ساکن صحیح
xstrcoll (حرف ثابت *a, حرف ثابت *b)
{
  صحیح diff;
  errno = 0;
  diff = strcoll (a, b);
  اگر (errno)
    {
      error (0, errno, _("cannot compare file names %s and %s"),
             quote_n (0, a), quote_n (1, b));
      سیٹ_exit_حالت (false);
      longjmp (failed_strcoll, 1);
    }
  واپس diff;
}

/* Comparison routines for sorting the files.  */

قسمتعریف خالی ثابت *V;
قسمتعریف صحیح (*qsortFunc)(V a, V b);

/* Used below in DEFINE_SORT_FUNCTIONS for _df_ sort function variants.  */
ساکن صحیح
dirfirst_check (ساخت fileinfo ثابت *a, ساخت fileinfo ثابت *b,
                صحیح (*cmp) (V, V))
{
  صحیح diff = is_linked_directory (b) - is_linked_directory (a);
  واپس diff ? diff : cmp (a, b);
}

/* Define the 8 different sort function variants required for each sortkey.
   KEY_NAME is a token describing the sort key, e.g., ctime, atime, size.
   KEY_CMP_FUNC is a function to compare records based on that key, e.g.,
   ctime_cmp, atime_cmp, size_cmp.  Append KEY_NAME to the string,
   '[rev_][x]str{cmp|coll}[_df]_', to create each function name.  */
#تعریف_کرو DEFINE_ترتیب_دو_FUNCTIONS(کلید_name, کلید_cmp_func)			\
  /* direct, non-dirfirst versions */					\
  ساکن صحیح xstrcoll_##key_name (V a, V b)				\
  { واپس کلید_cmp_func (a, b, xstrcoll); }				\
  ATTRIBUTE_خالص ساکن صحیح strcmp_##key_name (V a, V b)		\
  { واپس کلید_cmp_func (a, b, strcmp); }				\
                                                                        \
  /* reverse, non-dirfirst versions */					\
  ساکن صحیح rev_xstrcoll_##key_name (V a, V b)				\
  { واپس کلید_cmp_func (b, a, xstrcoll); }				\
  ATTRIBUTE_خالص ساکن صحیح rev_strcmp_##key_name (V a, V b)	\
  { واپس کلید_cmp_func (b, a, strcmp); }				\
                                                                        \
  /* direct, dirfirst versions */					\
  ساکن صحیح xstrcoll_df_##key_name (V a, V b)				\
  { واپس dirfirst_check (a, b, xstrcoll_##key_name); }		\
  ATTRIBUTE_خالص ساکن صحیح strcmp_df_##key_name (V a, V b)		\
  { واپس dirfirst_check (a, b, strcmp_##key_name); }			\
                                                                        \
  /* reverse, dirfirst versions */					\
  ساکن صحیح rev_xstrcoll_df_##key_name (V a, V b)			\
  { واپس dirfirst_check (a, b, rev_xstrcoll_##key_name); }		\
  ATTRIBUTE_خالص ساکن صحیح rev_strcmp_df_##key_name (V a, V b)	\
  { واپس dirfirst_check (a, b, rev_strcmp_##key_name); }

ساکن صحیح
cmp_ctime (ساخت fileinfo ثابت *a, ساخت fileinfo ثابت *b,
           صحیح (*cmp) (حرف ثابت *, حرف ثابت *))
{
  صحیح diff = timespec_cmp (get_stat_ctime (&b->stat),
                           get_stat_ctime (&a->stat));
  واپس diff ? diff : cmp (a->name, b->name);
}

ساکن صحیح
cmp_mtime (ساخت fileinfo ثابت *a, ساخت fileinfo ثابت *b,
           صحیح (*cmp) (حرف ثابت *, حرف ثابت *))
{
  صحیح diff = timespec_cmp (get_stat_mtime (&b->stat),
                           get_stat_mtime (&a->stat));
  واپس diff ? diff : cmp (a->name, b->name);
}

ساکن صحیح
cmp_atime (ساخت fileinfo ثابت *a, ساخت fileinfo ثابت *b,
           صحیح (*cmp) (حرف ثابت *, حرف ثابت *))
{
  صحیح diff = timespec_cmp (get_stat_atime (&b->stat),
                           get_stat_atime (&a->stat));
  واپس diff ? diff : cmp (a->name, b->name);
}

ساکن صحیح
cmp_btime (ساخت fileinfo ثابت *a, ساخت fileinfo ثابت *b,
           صحیح (*cmp) (حرف ثابت *, حرف ثابت *))
{
  صحیح diff = timespec_cmp (get_stat_btime (&b->stat),
                           get_stat_btime (&a->stat));
  واپس diff ? diff : cmp (a->name, b->name);
}

ساکن صحیح
off_cmp (off_t a, off_t b)
{
  واپس _GL_CMP (a, b);
}

ساکن صحیح
cmp_سائز (ساخت fileinfo ثابت *a, ساخت fileinfo ثابت *b,
          صحیح (*cmp) (حرف ثابت *, حرف ثابت *))
{
  صحیح diff = off_cmp (b->stat.st_سائز, a->stat.st_سائز);
  واپس diff ? diff : cmp (a->name, b->name);
}

ساکن صحیح
cmp_name (ساخت fileinfo ثابت *a, ساخت fileinfo ثابت *b,
          صحیح (*cmp) (حرف ثابت *, حرف ثابت *))
{
  واپس cmp (a->name, b->name);
}

/* Compare file extensions.  Files with no extension are 'smallest'.
   If extensions are the same, compare by file names instead.  */

ساکن صحیح
cmp_extension (ساخت fileinfo ثابت *a, ساخت fileinfo ثابت *b,
               صحیح (*cmp) (حرف ثابت *, حرف ثابت *))
{
  حرف ثابت *base1 = strrchr (a->name, '.');
  حرف ثابت *base2 = strrchr (b->name, '.');
  صحیح diff = cmp (base1 ? base1 : "", base2 ? base2 : "");
  واپس diff ? diff : cmp (a->name, b->name);
}

/* Return the (cached) screen width,
   for the NAME associated with the passed fileinfo F.  */

ساکن size_t
fileinfo_name_چوڑائی (ساخت fileinfo ثابت *f)
{
  واپس f->چوڑائی
         ? f->چوڑائی
         : quote_name_چوڑائی (f->name, filename_quoting_options, f->quoted);
}

ساکن صحیح
cmp_چوڑائی (ساخت fileinfo ثابت *a, ساخت fileinfo ثابت *b,
          صحیح (*cmp) (حرف ثابت *, حرف ثابت *))
{
  صحیح diff = fileinfo_name_چوڑائی (a) - fileinfo_name_چوڑائی (b);
  واپس diff ? diff : cmp (a->name, b->name);
}

DEFINE_ترتیب_دو_FUNCTIONS (ctime, cmp_ctime)
DEFINE_ترتیب_دو_FUNCTIONS (mtime, cmp_mtime)
DEFINE_ترتیب_دو_FUNCTIONS (atime, cmp_atime)
DEFINE_ترتیب_دو_FUNCTIONS (btime, cmp_btime)
DEFINE_ترتیب_دو_FUNCTIONS (سائز, cmp_سائز)
DEFINE_ترتیب_دو_FUNCTIONS (name, cmp_name)
DEFINE_ترتیب_دو_FUNCTIONS (extension, cmp_extension)
DEFINE_ترتیب_دو_FUNCTIONS (چوڑائی, cmp_چوڑائی)

/* Compare file versions.
   Unlike the other compare functions, cmp_version does not fail
   because filevercmp and strcmp do not fail; cmp_version uses strcmp
   instead of xstrcoll because filevercmp is locale-independent so
   strcmp is its appropriate secondary.

   All the other sort options need xstrcoll and strcmp variants,
   because they all use xstrcoll (either as the primary or secondary
   sort key), and xstrcoll has the ability to do a longjmp if strcoll fails for
   locale reasons.  */
ساکن صحیح
cmp_نسخہ (ساخت fileinfo ثابت *a, ساخت fileinfo ثابت *b)
{
  صحیح diff = filevercmp (a->name, b->name);
  واپس diff ? diff : strcmp (a->name, b->name);
}

ساکن صحیح
xstrcoll_نسخہ (V a, V b)
{
  واپس cmp_نسخہ (a, b);
}
ساکن صحیح
rev_xstrcoll_نسخہ (V a, V b)
{
  واپس cmp_نسخہ (b, a);
}
ساکن صحیح
xstrcoll_df_نسخہ (V a, V b)
{
  واپس dirfirst_check (a, b, xstrcoll_نسخہ);
}
ساکن صحیح
rev_xstrcoll_df_نسخہ (V a, V b)
{
  واپس dirfirst_check (a, b, rev_xstrcoll_نسخہ);
}


/* We have 2^3 different variants for each sort-key function
   (for 3 independent sort modes).
   The function pointers stored in this array must be dereferenced as:

    sort_variants[sort_key][use_strcmp][reverse][dirs_first]

   Note that the order in which sort keys are listed in the function pointer
   array below is defined by the order of the elements in the time_type and
   sort_type enums!  */

#تعریف_کرو LIST_SORTFUNCTION_VARIANTS(کلید_name)                        \
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

ساکن qsortFunc ثابت ترتیب_دو_functions[][2][2][2] =
  {
    LIST_SORTFUNCTION_VARIANTS (name),
    LIST_SORTFUNCTION_VARIANTS (extension),
    LIST_SORTFUNCTION_VARIANTS (چوڑائی),
    LIST_SORTFUNCTION_VARIANTS (سائز),

    {
      {
        { xstrcoll_نسخہ, xstrcoll_df_نسخہ },
        { rev_xstrcoll_نسخہ, rev_xstrcoll_df_نسخہ },
      },

      /* We use NULL for the strcmp variants of version comparison
         since as explained in cmp_version definition, version comparison
         does not rely on xstrcoll, so it will never longjmp, and never
         need to try the strcmp fallback. */
      {
        { تہی, تہی },
        { تہی, تہی },
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
static_assert (countof (ترتیب_دو_functions) == ترتیب_دو_numtypes - 2 + وقت_numtypes);

/* Set up SORTED_FILE to point to the in-use entries in CWD_FILE, in order.  */

ساکن خالی
initialize_ordering_سمتیہ (خالی)
{
  برائے (idx_t i = 0; i < cwd_n_مستعمل; i++)
    sorted_file[i] = &cwd_file[i];
}

/* Cache values based on attributes global to all files.  */

ساکن خالی
update_current_files_info (خالی)
{
  /* Cache screen width of name, if needed multiple times.  */
  اگر (ترتیب_دو_type == ترتیب_دو_چوڑائی
      || (سطر_لمبائی && (format == many_فی_سطر || format == horizontal)))
    {
      برائے (idx_t i = 0; i < cwd_n_مستعمل; i++)
        {
          ساخت fileinfo *f = sorted_file[i];
          f->چوڑائی = fileinfo_name_چوڑائی (f);
        }
    }
}

/* Sort the files now in the table.  */

ساکن خالی
ترتیب_دو_files (خالی)
{
  bool use_strcmp;

  اگر (sorted_file_alloc < cwd_n_مستعمل + (cwd_n_مستعمل >> 1))
    {
      آزاد (sorted_file);
      sorted_file = xinmalloc (cwd_n_مستعمل, 3 * حجم *sorted_file);
      sorted_file_alloc = 3 * cwd_n_مستعمل;
    }

  initialize_ordering_سمتیہ ();

  update_current_files_info ();

  اگر (ترتیب_دو_type == ترتیب_دو_none)
    واپس;

  /* Try strcoll.  If it fails, fall back on strcmp.  We can't safely
     ignore strcoll failures, as a failing strcoll might be a
     comparison function that is not a total order, and if we ignored
     the failure this might cause qsort to dump core.  */

  اگر (! setjmp (failed_strcoll))
    use_strcmp = false;      /* strcoll() succeeded */
  ورنہ
    {
      use_strcmp = true;
      affirm (ترتیب_دو_type != ترتیب_دو_نسخہ);
      initialize_ordering_سمتیہ ();
    }

  /* When sort_type == sort_time, use time_type as subindex.  */
  mpsort ((خالی ثابت **) sorted_file, cwd_n_مستعمل,
          ترتیب_دو_functions[ترتیب_دو_type + (ترتیب_دو_type == ترتیب_دو_وقت ? وقت_type : 0)]
                        [use_strcmp][ترتیب_دو_الٹ]
                        [directories_پہلا]);
}

/* List all the files now in the table.  */

ساکن خالی
print_current_files (خالی)
{
  انتخاب (format)
    {
    حالت one_فی_سطر:
      برائے (idx_t i = 0; i < cwd_n_مستعمل; i++)
        {
          print_file_name_and_frills (sorted_file[i], 0);
          حرف_لکھو (eolbyte);
        }
      توڑو;

    حالت many_فی_سطر:
      اگر (! سطر_لمبائی)
        print_with_separator (' ');
      ورنہ
        print_many_فی_سطر ();
      توڑو;

    حالت horizontal:
      اگر (! سطر_لمبائی)
        print_with_separator (' ');
      ورنہ
        print_horizontal ();
      توڑو;

    حالت with_commas:
      print_with_separator (',');
      توڑو;

    حالت long_format:
      برائے (idx_t i = 0; i < cwd_n_مستعمل; i++)
        {
          سیٹ_معمول_color ();
          print_long_format (sorted_file[i]);
          dired_outbyte (eolbyte);
        }
      توڑو;
    }
}

/* Replace the first %b with precomputed aligned month names.
   Note on glibc-2.7 at least, this speeds up the whole 'ls -lU'
   process by around 17%, compared to letting strftime() handle the %b.  */

ساکن ptrdiff_t
align_nstrftime (حرف *buf, size_t سائز, bool recent, ساخت tm ثابت *tm,
                 timezone_t tz, صحیح ns)
{
  حرف ثابت *nfmt = (use_abformat
                      ? abformat[recent][tm->tm_mon]
                      : long_وقت_format[recent]);
  واپس nstrftime (buf, سائز, nfmt, tm, tz, ns);
}

/* Return the expected number of columns in a long-format timestamp,
   or zero if it cannot be calculated.  */

ساکن صحیح
long_وقت_expected_چوڑائی (خالی)
{
  ساکن صحیح چوڑائی = -1;

  اگر (چوڑائی < 0)
    {
      time_t epoch = 0;
      ساخت tm tm;
      حرف buf[وقت_STAMP_LEN_زیادہ_سے_زیادہ + 1];

      /* In case you're wondering if localtime_rz can fail with an input time_t
         value of 0, let's just say it's very unlikely, but not inconceivable.
         The TZ environment variable would have to specify a time zone that
         is 2**31-1900 years or more ahead of UTC.  This could happen only on
         a 64-bit system that blindly accepts e.g., TZ=UTC+20000000000000.
         However, this is not possible with Solaris 10 or glibc-2.3.5, since
         their implementations limit the offset to 167:59 and 24:00, resp.  */
      اگر (localtime_rz (localtz, &epoch, &tm))
        {
          ptrdiff_t len = align_nstrftime (buf, حجم buf, false,
                                           &tm, localtz, 0);
          اگر (len > 0)
            چوڑائی = mbsnwidth (buf, len, MBSWIDTH_FLAGS);
        }

      اگر (چوڑائی < 0)
        چوڑائی = 0;
    }

  واپس چوڑائی;
}

/* Print the user or group name NAME, with numeric id ID, using a
   print width of WIDTH columns.  */

ساکن خالی
format_user_or_گروہ (حرف ثابت *name, uintmax_t id, صحیح چوڑائی)
{
  اگر (name)
    {
      صحیح name_چوڑائی = mbswidth (name, MBSWIDTH_FLAGS);
      صحیح چوڑائی_gap = name_چوڑائی < 0 ? 0 : چوڑائی - name_چوڑائی;
      صحیح pad = MAX (0, چوڑائی_gap);
      dired_outstring (name);

      کرو
        dired_outbyte (' ');
      جب (pad--);
    }
  ورنہ
    dired_pos += لکھو ("%*ju ", چوڑائی, id);
}

/* Print the name or id of the user with id U, using a print width of
   WIDTH.  */

ساکن خالی
format_user (uid_t u, صحیح چوڑائی, bool stat_ok)
{
  format_user_or_گروہ (! stat_ok ? "?" :
                        (numeric_ids ? تہی : getuser (u)), u, چوڑائی);
}

/* Likewise, for groups.  */

ساکن خالی
format_گروہ (gid_t g, صحیح چوڑائی, bool stat_ok)
{
  format_user_or_گروہ (! stat_ok ? "?" :
                        (numeric_ids ? تہی : getgroup (g)), g, چوڑائی);
}

/* Return the number of columns that format_user_or_group will print,
   or -1 if unknown.  */

ساکن صحیح
format_user_or_گروہ_چوڑائی (حرف ثابت *name, uintmax_t id)
{
  واپس (name
          ? mbswidth (name, MBSWIDTH_FLAGS)
          : snprintf (تہی, 0, "%ju", id));
}

/* Return the number of columns that format_user will print,
   or -1 if unknown.  */

ساکن صحیح
format_user_چوڑائی (uid_t u)
{
  واپس format_user_or_گروہ_چوڑائی (numeric_ids ? تہی : getuser (u), u);
}

/* Likewise, for groups.  */

ساکن صحیح
format_گروہ_چوڑائی (gid_t g)
{
  واپس format_user_or_گروہ_چوڑائی (numeric_ids ? تہی : getgroup (g), g);
}

/* Return a pointer to a formatted version of F->stat.st_ino,
   possibly using buffer, which must be at least
   INT_BUFSIZE_BOUND (uintmax_t) bytes.  */
ساکن حرف *
format_inode (حرف buf[INT_BUFSIZE_BOUND (uintmax_t)],
              ثابت ساخت fileinfo *f)
{
  واپس (f->stat_ok && f->stat.st_ino != NOT_AN_INODE_نمبر
          ? umaxtostr (f->stat.st_ino, buf)
          : (حرف *) "?");
}

/* Print information about F in long format.  */
ساکن خالی
print_long_format (ثابت ساخت fileinfo *f)
{
  حرف modebuf[12];
  حرف buf
    [LONGEST_HUMAN_READABLE + 1		/* inode */
     + LONGEST_HUMAN_READABLE + 1	/* size in blocks */
     + حجم (modebuf) - 1 + 1		/* mode string */
     + INT_BUFSIZE_BOUND (uintmax_t)	/* st_nlink */
     + LONGEST_HUMAN_READABLE + 2	/* major device number */
     + LONGEST_HUMAN_READABLE + 1	/* minor device number */
     + وقت_STAMP_LEN_زیادہ_سے_زیادہ + 1	/* max length of time/date */
     ];
  ptrdiff_t s;
  حرف *p;
  ساخت timespec when_timespec;
  ساخت tm when_local;
  bool btime_ok = true;

  /* Compute the mode string, except remove the trailing space if no
     file in this directory has an ACL or security context.  */
  اگر (f->stat_ok)
    filemodestring (&f->stat, modebuf);
  ورنہ
    {
      modebuf[0] = filetype_letter[f->filetype];
      memset (modebuf + 1, '?', 10);
      modebuf[11] = '\0';
    }
  اگر (! any_has_acl)
    modebuf[10] = '\0';
  ورنہ اگر (f->acl_type == ACL_T_LSM_CONTEXT_ONLY)
    modebuf[10] = '.';
  ورنہ اگر (f->acl_type == ACL_T_YES)
    modebuf[10] = '+';
  ورنہ اگر (f->acl_type == ACL_T_UNKNOWN)
    modebuf[10] = '?';

  انتخاب (وقت_type)
    {
    حالت وقت_ctime:
      when_timespec = get_stat_ctime (&f->stat);
      توڑو;
    حالت وقت_mtime:
      when_timespec = get_stat_mtime (&f->stat);
      توڑو;
    حالت وقت_atime:
      when_timespec = get_stat_atime (&f->stat);
      توڑو;
    حالت وقت_btime:
      when_timespec = get_stat_btime (&f->stat);
      اگر (when_timespec.tv_sec == -1 && when_timespec.tv_nsec == -1)
        btime_ok = false;
      توڑو;
    حالت وقت_numtypes: طےشدہ:
      unreachable ();
    }

  p = buf;

  اگر (print_inode)
    {
      حرف hbuf[INT_BUFSIZE_BOUND (uintmax_t)];
      p += sprintf (p, "%*s ", inode_نمبر_چوڑائی, format_inode (hbuf, f));
    }

  اگر (print_بلاک_سائز)
    {
      حرف hbuf[LONGEST_HUMAN_READABLE + 1];
      حرف ثابت *بلاکیں =
        (! f->stat_ok
         ? "?"
         : human_readable (STP_NBLOCKS (&f->stat), hbuf, human_output_opts,
                           ST_NBLOCKSIZE, output_بلاک_سائز));
      صحیح بلاکیں_چوڑائی = mbswidth (بلاکیں, MBSWIDTH_FLAGS);
      برائے (صحیح pad = بلاکیں_چوڑائی < 0 ? 0 : بلاک_سائز_چوڑائی - بلاکیں_چوڑائی;
           0 < pad; pad--)
        *p++ = ' ';
      جب ((*p++ = *بلاکیں++))
        جاری;
      p[-1] = ' ';
    }

  /* The last byte of the mode string is the POSIX
     "optional alternate access method flag".  */
  {
    حرف hbuf[INT_BUFSIZE_BOUND (uintmax_t)];
    p += sprintf (p, "%s %*s ", modebuf, nlink_چوڑائی,
                  ! f->stat_ok ? "?" : umaxtostr (f->stat.st_nlink, hbuf));
  }

  dired_indent ();

  اگر (print_owner || print_گروہ || print_author || print_scontext)
    {
      dired_outbuf (buf, p - buf);

      اگر (print_owner)
        format_user (f->stat.st_uid, owner_چوڑائی, f->stat_ok);

      اگر (print_گروہ)
        format_گروہ (f->stat.st_gid, گروہ_چوڑائی, f->stat_ok);

      اگر (print_author)
        format_user (f->stat.st_author, author_چوڑائی, f->stat_ok);

      اگر (print_scontext)
        format_user_or_گروہ (f->scontext, 0, scontext_چوڑائی);

      p = buf;
    }

  اگر (f->stat_ok
      && (S_ISCHR (f->stat.st_منوال) || S_ISBLK (f->stat.st_منوال)))
    {
      صحیح blanks_چوڑائی = (file_سائز_چوڑائی
                          - (major_device_نمبر_چوڑائی + 2
                             + minor_device_نمبر_چوڑائی));
      p += sprintf (p, "%*ju, %*ju ",
                    major_device_نمبر_چوڑائی + MAX (0, blanks_چوڑائی),
                    (uintmax_t) major (f->stat.st_rdev),
                    minor_device_نمبر_چوڑائی,
                    (uintmax_t) minor (f->stat.st_rdev));
    }
  ورنہ
    {
      حرف hbuf[LONGEST_HUMAN_READABLE + 1];
      حرف ثابت *سائز =
        (! f->stat_ok
         ? "?"
         : human_readable (unsigned_file_سائز (f->stat.st_سائز),
                           hbuf, file_human_output_opts, 1,
                           file_output_بلاک_سائز));
      صحیح سائز_چوڑائی = mbswidth (سائز, MBSWIDTH_FLAGS);
      برائے (صحیح pad = سائز_چوڑائی < 0 ? 0 : file_سائز_چوڑائی - سائز_چوڑائی;
           0 < pad; pad--)
        *p++ = ' ';
      جب ((*p++ = *سائز++))
        جاری;
      p[-1] = ' ';
    }

  s = -1;

  اگر (f->stat_ok && btime_ok
      && localtime_rz (localtz, &when_timespec.tv_sec, &when_local))
    {
      ساخت timespec six_months_ago;
      bool recent;

      /* If the file appears to be in the future, update the current
         time, in case the file happens to have been modified since
         the last time we checked the clock.  */
      اگر (timespec_cmp (current_وقت, when_timespec) < 0)
        gettime (&current_وقت);

      /* Consider a time to be recent if it is within the past six months.
         A Gregorian year has 365.2425 * 24 * 60 * 60 == 31556952 seconds
         on the average.  Write this value as an integer constant to
         avoid floating point hassles.  */
      six_months_ago.tv_sec = current_وقت.tv_sec - 31556952 / 2;
      six_months_ago.tv_nsec = current_وقت.tv_nsec;

      recent = (timespec_cmp (six_months_ago, when_timespec) < 0
                && timespec_cmp (when_timespec, current_وقت) < 0);

      /* We assume here that all time zones are offset from UTC by a
         whole number of seconds.  */
      s = align_nstrftime (p, وقت_STAMP_LEN_زیادہ_سے_زیادہ + 1, recent,
                           &when_local, localtz, when_timespec.tv_nsec);
    }

  اگر (0 <= s)
    {
      p += s;
      *p++ = ' ';
    }
  ورنہ
    {
      /* The time cannot be converted using the desired format, so
         print it as a huge integer number of seconds.  */
      حرف hbuf[INT_BUFSIZE_BOUND (intmax_t)];
      p += sprintf (p, "%*s ", long_وقت_expected_چوڑائی (),
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
          print_name_with_quoting (f, true, تہی, (p - buf) + w + 4);
          اگر (indicator_style != none)
            print_type_indicator (true, f->linkmode, unknown);
        }
    }
  ورنہ اگر (indicator_style != none)
    print_type_indicator (f->stat_ok, f->stat.st_منوال, f->filetype);
}

/* Write to *BUF a quoted representation of the file name NAME, if non-null,
   using OPTIONS to control quoting.  *BUF is set to NAME if no quoting
   is required.  *BUF is allocated if more space required (and the original
   *BUF is not deallocated).
   Store the number of screen columns occupied by NAME's quoted
   representation into WIDTH, if non-null.
   Store into PAD whether an initial space is needed for padding.
   Return the number of bytes in *BUF.  */

ساکن size_t
quote_name_buf (حرف **inbuf, size_t bufsize, حرف *name,
                ساخت quoting_options ثابت *options,
                صحیح needs_general_quoting, size_t *چوڑائی, bool *pad)
{
  حرف *buf = *inbuf;
  size_t displayed_چوڑائی IF_LINT ( = 0);
  size_t len = 0;
  bool quoted;

  شمار quoting_style qs = get_quoting_style (options);
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
      len = quotearg_بفر (buf, bufsize, name, -1, options);
      اگر (bufsize <= len)
        {
          buf = xmalloc (len + 1);
          quotearg_بفر (buf, len + 1, name, -1, options);
        }

      quoted = (*name != *buf) || لڑی_لمبائی (name) != len;
    }
  ورنہ اگر (needs_further_quoting || extra_newline_protection)
    {
      len = لڑی_لمبائی (name);
      اگر (bufsize <= len)
        buf = xmalloc (len + 1);
      حافظہ_نقل (buf, name, len + 1);

      quoted = false;
    }
  ورنہ
    {
      len = لڑی_لمبائی (name);
      buf = name;
      quoted = false;
    }

  /* It's safe to replace newlines without multi-byte iteration, as newlines
     do not appear as part of any multi-byte encoded character.  */
  اگر (extra_newline_protection)
    برائے (حرف *p = buf; p < buf + len; p++)
      اگر (eolbyte && *p == '\n')
        *p = '?';

  اگر (needs_further_quoting)
    {
      اگر (MB_CUR_MAX > 1)
        {
          حرف ثابت *p = buf;
          حرف ثابت *plimit = buf + len;
          حرف *q = buf;
          displayed_چوڑائی = 0;

          جب (p < plimit)
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
                  displayed_چوڑائی += 1;
                  توڑو;
                طےشدہ:
                  /* If we have a multibyte sequence, copy it until we
                     reach its end, replacing each non-printable multibyte
                     character with a single question mark.  */
                  {
                    mbstate_t mbstate; mbszero (&mbstate);
                    کرو
                      {
                        char32_t wc;
                        size_t بائٹیں;
                        صحیح w;

                        بائٹیں = mbrtoc32 (&wc, p, plimit - p, &mbstate);

                        اگر (بائٹیں == (size_t) -1)
                          {
                            /* An invalid multibyte sequence was
                               encountered.  Skip one input byte, and
                               put a question mark.  */
                            p++;
                            *q++ = '?';
                            displayed_چوڑائی += 1;
                            توڑو;
                          }

                        اگر (بائٹیں == (size_t) -2)
                          {
                            /* An incomplete multibyte character
                               at the end.  Replace it entirely with
                               a question mark.  */
                            p = plimit;
                            *q++ = '?';
                            displayed_چوڑائی += 1;
                            توڑو;
                          }

                        اگر (بائٹیں == 0)
                          /* A null wide character was encountered.  */
                          بائٹیں = 1;

                        w = c32width (wc);
                        اگر (w >= 0)
                          {
                            /* A printable multibyte character.
                               Keep it.  */
                            برائے (; بائٹیں > 0; --بائٹیں)
                              *q++ = *p++;
                            displayed_چوڑائی += w;
                          }
                        ورنہ
                          {
                            /* An nonprintable multibyte character.
                               Replace it entirely with a question
                               mark.  */
                            p += بائٹیں;
                            *q++ = '?';
                            displayed_چوڑائی += 1;
                          }
                      }
                    جب (! mbsinit (&mbstate));
                  }
                  توڑو;
              }

          /* The buffer may have shrunk.  */
          len = q - buf;
        }
      ورنہ
        {
          حرف *p = buf;
          حرف ثابت *plimit = buf + len;

          جب (p < plimit)
            {
              اگر (! isprint (مقصد_uchar (*p)))
                *p = '?';
              p++;
            }
          displayed_چوڑائی = len;
        }
    }
  ورنہ اگر (چوڑائی != تہی)
    {
      اگر (MB_CUR_MAX > 1)
        {
          displayed_چوڑائی = mbsnwidth (buf, len, MBSWIDTH_FLAGS);
          displayed_چوڑائی = MAX (0, displayed_چوڑائی);
        }
      ورنہ
        {
          حرف ثابت *p = buf;
          حرف ثابت *plimit = buf + len;

          displayed_چوڑائی = 0;
          جب (p < plimit)
            {
              اگر (isprint (مقصد_uchar (*p)))
                displayed_چوڑائی++;
              p++;
            }
        }
    }

  /* Set padding to better align quoted items,
     and also give a visual indication that quotes are
     not actually part of the name.  */
  *pad = (align_variable_outer_quotes && cwd_some_quoted && ! quoted);

  اگر (چوڑائی != تہی)
    *چوڑائی = displayed_چوڑائی;

  *inbuf = buf;

  واپس len;
}

ساکن size_t
quote_name_چوڑائی (حرف ثابت *name, ساخت quoting_options ثابت *options,
                  صحیح needs_general_quoting)
{
  حرف smallbuf[BUFSIZ];
  حرف *buf = smallbuf;
  size_t چوڑائی;
  bool pad;

  quote_name_buf (&buf, حجم smallbuf, (حرف *) name, options,
                  needs_general_quoting, &چوڑائی, &pad);

  اگر (buf != smallbuf && buf != name)
    آزاد (buf);

  چوڑائی += pad;

  واپس چوڑائی;
}

/* %XX escape any input out of range as defined in RFC3986,
   and also if PATH, convert all path separators to '/'.  */
ساکن حرف *
file_escape (حرف ثابت *str, bool راستہ)
{
  حرف *esc = xnmalloc (3, لڑی_لمبائی (str) + 1);
  حرف *p = esc;
  جب (*str)
    {
      اگر (راستہ && ISSLASH (*str))
        {
          *p++ = '/';
          str++;
        }
      ورنہ اگر (RFC3986[مقصد_uchar (*str)])
        *p++ = *str++;
      ورنہ
        p += sprintf (p, "%%%02x", مقصد_uchar (*str++));
    }
  *p = '\0';
  واپس esc;
}

ساکن size_t
quote_name (حرف ثابت *name, ساخت quoting_options ثابت *options,
            صحیح needs_general_quoting, ثابت ساخت bin_str *color,
            bool allow_pad, ساخت obstack *ڈھیر, حرف ثابت *absolute_name)
{
  حرف smallbuf[BUFSIZ];
  حرف *buf = smallbuf;
  size_t len;
  bool pad;

  len = quote_name_buf (&buf, حجم smallbuf, (حرف *) name, options,
                        needs_general_quoting, تہی, &pad);

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
          حرف_لکھو (*buf);
        }
      حرف *h = file_escape (hostname, /* path= */ false);
      حرف *n = file_escape (absolute_name, /* path= */ true);
      /* TODO: It would be good to be able to define parameters
         to give hints to the terminal as how best to render the URI.
         For example since ls is outputting a dense block of URIs
         it would be best to not underline by default, and only
         do so upon hover etc.  */
      لکھو ("\033]8;;file://%s%s%s\033\\", h, *n == '/' ? "" : "/", n);
      آزاد (h);
      آزاد (n);
    }

  اگر (ڈھیر)
    ڈالو_current_dired_pos (ڈھیر);

  fwrite (buf + skip_quotes, 1, len - (skip_quotes * 2), stdout);

  dired_pos += len;

  اگر (ڈھیر)
    ڈالو_current_dired_pos (ڈھیر);

  اگر (absolute_name)
    {
      fputs ("\033]8;;\033\\", stdout);
      اگر (skip_quotes)
        حرف_لکھو (*(buf + len - 1));
    }

  اگر (buf != smallbuf && buf != name)
    آزاد (buf);

  واپس len + pad;
}

ساکن size_t
print_name_with_quoting (ثابت ساخت fileinfo *f,
                         bool symlink_ہدف,
                         ساخت obstack *ڈھیر,
                         size_t آغاز_ستون)
{
  حرف ثابت *name = symlink_ہدف ? f->linkname : f->name;

  ثابت ساخت bin_str *color
    = print_with_color ? get_color_indicator (f, symlink_ہدف) : تہی;

  bool مستعمل_color_this_وقت = (print_with_color
                               && (color || is_colored (C_NORM)));

  size_t len = quote_name (name, filename_quoting_options, f->quoted,
                           color, !symlink_ہدف, ڈھیر, f->absolute_name);

  process_اشارات ();
  اگر (مستعمل_color_this_وقت)
    {
      prep_non_filename_متن ();

      /* We use the byte length rather than display width here as
         an optimization to avoid accurately calculating the width,
         because we only output the clear to EOL sequence if the name
         _might_ wrap to the next line.  This may output a sequence
         unnecessarily in multi-byte locales for example,
         but in that case it's inconsequential to the output.  */
      اگر (سطر_لمبائی
          && (آغاز_ستون / سطر_لمبائی != (آغاز_ستون + len - 1) / سطر_لمبائی))
        put_indicator (&color_indicator[C_CLR_مقصد_EOL]);
    }

  واپس len;
}

ساکن خالی
prep_non_filename_متن (خالی)
{
  اگر (color_indicator[C_انتہا].string != تہی)
    put_indicator (&color_indicator[C_انتہا]);
  ورنہ
    {
      put_indicator (&color_indicator[C_بایاں]);
      put_indicator (&color_indicator[C_RESET]);
      put_indicator (&color_indicator[C_دایاں]);
    }
}

/* Print the file name of 'f' with appropriate quoting.
   Also print file size, inode number, and filetype indicator character,
   as requested by switches.  */

ساکن size_t
print_file_name_and_frills (ثابت ساخت fileinfo *f, size_t آغاز_ستون)
{
  حرف buf[MAX (LONGEST_HUMAN_READABLE + 1, INT_BUFSIZE_BOUND (uintmax_t))];

  سیٹ_معمول_color ();

  اگر (print_inode)
    لکھو ("%*s ", format == with_commas ? 0 : inode_نمبر_چوڑائی,
            format_inode (buf, f));

  اگر (print_بلاک_سائز)
    {
      حرف ثابت *بلاکیں =
        (! f->stat_ok
         ? "?"
         : human_readable (STP_NBLOCKS (&f->stat), buf, human_output_opts,
                           ST_NBLOCKSIZE, output_بلاک_سائز));
      صحیح بلاکیں_چوڑائی = mbswidth (بلاکیں, MBSWIDTH_FLAGS);
      صحیح pad = 0;
      اگر (0 <= بلاکیں_چوڑائی && بلاک_سائز_چوڑائی && format != with_commas)
        pad = بلاک_سائز_چوڑائی - بلاکیں_چوڑائی;
      لکھو ("%*s%s ", pad, "", بلاکیں);
    }

  اگر (print_scontext)
    لکھو ("%*s ", format == with_commas ? 0 : scontext_چوڑائی, f->scontext);

  size_t چوڑائی = print_name_with_quoting (f, false, تہی, آغاز_ستون);

  اگر (indicator_style != none)
    چوڑائی += print_type_indicator (f->stat_ok, f->stat.st_منوال, f->filetype);

  واپس چوڑائی;
}

/* Given these arguments describing a file, return the single-byte
   type indicator, or 0.  */
ساکن حرف
get_type_indicator (bool stat_ok, mode_t منوال_2, شمار filetype type)
{
  حرف c;

  اگر (stat_ok ? S_ISREG (منوال_2) : type == معمول)
    {
      اگر (stat_ok && indicator_style == classify && (منوال_2 & S_IXUGO))
        c = '*';
      ورنہ
        c = 0;
    }
  ورنہ
    {
      اگر (stat_ok ? S_ISDIR (منوال_2) : type == directory || type == arg_directory)
        c = '/';
      ورنہ اگر (indicator_style == slash)
        c = 0;
      ورنہ اگر (stat_ok ? S_ISLNK (منوال_2) : type == symbolic_link)
        c = '@';
      ورنہ اگر (stat_ok ? S_ISFIFO (منوال_2) : type == fifo)
        c = '|';
      ورنہ اگر (stat_ok ? S_ISSOCK (منوال_2) : type == sock)
        c = '=';
      ورنہ اگر (stat_ok && S_ISDOOR (منوال_2))
        c = '>';
      ورنہ
        c = 0;
    }
  واپس c;
}

ساکن bool
print_type_indicator (bool stat_ok, mode_t منوال_2, شمار filetype type)
{
  حرف c = get_type_indicator (stat_ok, منوال_2, type);
  اگر (c)
    dired_outbyte (c);
  واپس !!c;
}

/* Returns if color sequence was printed.  */
ساکن bool
print_color_indicator (ثابت ساخت bin_str *ind)
{
  اگر (ind)
    {
      /* Need to reset so not dealing with attribute combinations */
      اگر (is_colored (C_NORM))
        restore_default_color ();
      put_indicator (&color_indicator[C_بایاں]);
      put_indicator (ind);
      put_indicator (&color_indicator[C_دایاں]);
    }

  واپس ind != تہی;
}

/* Returns color indicator or NULL if none.  */
ATTRIBUTE_خالص
ساکن ثابت ساخت bin_str *
get_color_indicator (ثابت ساخت fileinfo *f, bool symlink_ہدف)
{
  شمار indicator_no type;
  ساخت color_ext_type *ext;	/* Color extension */
  size_t len;			/* Length of name */

  حرف ثابت *name;
  mode_t منوال_2;
  صحیح linkok;
  اگر (symlink_ہدف)
    {
      name = f->linkname;
      منوال_2 = f->linkmode;
      linkok = f->linkok ? 0 : -1;
    }
  ورنہ
    {
      name = f->name;
      منوال_2 = file_or_link_منوال (f);
      linkok = f->linkok;
    }

  /* Is this a nonexistent file?  If so, linkok == -1.  */

  اگر (linkok == -1 && is_colored (C_MISSING))
    type = C_MISSING;
  ورنہ اگر (!f->stat_ok)
    {
      ساکن شمار indicator_no ثابت filetype_indicator[] =
        {
          C_ORPHAN, C_FIFO, C_CHR, C_DIR, C_BLK, C_FILE,
          C_LINK, C_SOCK, C_FILE, C_DIR
        };
      static_assert (countof (filetype_indicator) == filetype_cardinality);
      type = filetype_indicator[f->filetype];
    }
  ورنہ
    {
      اگر (S_ISREG (منوال_2))
        {
          type = C_FILE;

          اگر ((منوال_2 & S_ISUID) != 0 && is_colored (C_SETUID))
            type = C_SETUID;
          ورنہ اگر ((منوال_2 & S_ISGID) != 0 && is_colored (C_SETGID))
            type = C_SETGID;
          ورنہ اگر (f->has_capability)
            type = C_CAP;
          ورنہ اگر ((منوال_2 & S_IXUGO) != 0 && is_colored (C_EXEC))
            type = C_EXEC;
          ورنہ اگر ((1 < f->stat.st_nlink) && is_colored (C_MULTIHARDLINK))
            type = C_MULTIHARDLINK;
        }
      ورنہ اگر (S_ISDIR (منوال_2))
        {
          type = C_DIR;

          اگر ((منوال_2 & S_ISVTX) && (منوال_2 & S_IWOTH)
              && is_colored (C_STICKY_OTHER_WRITABLE))
            type = C_STICKY_OTHER_WRITABLE;
          ورنہ اگر ((منوال_2 & S_IWOTH) != 0 && is_colored (C_OTHER_WRITABLE))
            type = C_OTHER_WRITABLE;
          ورنہ اگر ((منوال_2 & S_ISVTX) != 0 && is_colored (C_STICKY))
            type = C_STICKY;
        }
      ورنہ اگر (S_ISLNK (منوال_2))
        type = C_LINK;
      ورنہ اگر (S_ISFIFO (منوال_2))
        type = C_FIFO;
      ورنہ اگر (S_ISSOCK (منوال_2))
        type = C_SOCK;
      ورنہ اگر (S_ISBLK (منوال_2))
        type = C_BLK;
      ورنہ اگر (S_ISCHR (منوال_2))
        type = C_CHR;
      ورنہ اگر (S_ISDOOR (منوال_2))
        type = C_DOOR;
      ورنہ
        {
          /* Classify a file of some other type as C_ORPHAN.  */
          type = C_ORPHAN;
        }
    }

  /* Check the file's suffix only if still classified as C_FILE.  */
  ext = تہی;
  اگر (type == C_FILE)
    {
      /* Test if NAME has a recognized suffix.  */

      len = لڑی_لمبائی (name);
      name += len;		/* Pointer to final \0.  */
      برائے (ext = color_ext_list; ext != تہی; ext = ext->اگلا)
        {
          اگر (ext->ext.len <= len)
            {
              اگر (ext->exact_مطابقت)
                {
                  اگر (STREQ_LEN (name - ext->ext.len, ext->ext.string,
                                 ext->ext.len))
                    توڑو;
                }
              ورنہ
                {
                  اگر (c_strncasecmp (name - ext->ext.len, ext->ext.string,
                                     ext->ext.len) == 0)
                    توڑو;
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

  ثابت ساخت bin_str *ثابت s
    = ext ? &(ext->seq) : &color_indicator[type];

  واپس s->string ? s : تہی;
}

/* Output a color indicator (which may contain nulls).
   Note we don't update dired_pos here as emacs can be configured
   to handle color codes by stripping/converting them.  */
ساکن خالی
put_indicator (ثابت ساخت bin_str *ind)
{
  اگر (! مستعمل_color)
    {
      مستعمل_color = true;

      /* If the standard output is a controlling terminal, watch out
         for signals, so that the colors can be restored to the
         default state if "ls" is suspended or interrupted.  */

      اگر (0 <= tcgetpgrp (STDOUT_FILENO))
        signal_init ();

      prep_non_filename_متن ();
    }

  fwrite (ind->string, ind->len, 1, stdout);
}

ساکن size_t
لمبائی_کا_file_name_and_frills (ثابت ساخت fileinfo *f)
{
  size_t len = 0;
  حرف buf[MAX (LONGEST_HUMAN_READABLE + 1, INT_BUFSIZE_BOUND (uintmax_t))];

  اگر (print_inode)
    len += 1 + (format == with_commas
                ? لڑی_لمبائی (umaxtostr (f->stat.st_ino, buf))
                : inode_نمبر_چوڑائی);

  اگر (print_بلاک_سائز)
    len += 1 + (format == with_commas
                ? لڑی_لمبائی (! f->stat_ok ? "?"
                          : human_readable (STP_NBLOCKS (&f->stat), buf,
                                            human_output_opts, ST_NBLOCKSIZE,
                                            output_بلاک_سائز))
                : بلاک_سائز_چوڑائی);

  اگر (print_scontext)
    len += 1 + (format == with_commas ? لڑی_لمبائی (f->scontext) : scontext_چوڑائی);

  len += fileinfo_name_چوڑائی (f);

  اگر (indicator_style != none)
    {
      حرف c = get_type_indicator (f->stat_ok, f->stat.st_منوال, f->filetype);
      len += (c != 0);
    }

  واپس len;
}

ساکن خالی
print_many_فی_سطر (خالی)
{
  idx_t ستون_تعداد_2 = calculate_columns (true);
  ساخت عمود_info ثابت *سطر_fmt = &عمود_info[ستون_تعداد_2 - 1];

  /* Calculate the number of rows that will be in each column except possibly
     for a short column on the right.  */
  idx_t صفیں = cwd_n_مستعمل / ستون_تعداد_2 + (cwd_n_مستعمل % ستون_تعداد_2 != 0);

  برائے (idx_t صف = 0; صف < صفیں; صف++)
    {
      size_t ستون = 0;
      idx_t filesno = صف;
      size_t pos = 0;

      /* Print the next row.  */
      جب (true)
        {
          ساخت fileinfo ثابت *f = sorted_file[filesno];
          size_t name_لمبائی = لمبائی_کا_file_name_and_frills (f);
          size_t max_name_لمبائی = سطر_fmt->ستون_arr[ستون++];
          print_file_name_and_frills (f, pos);

          اگر (cwd_n_مستعمل - صفیں <= filesno)
            توڑو;
          filesno += صفیں;

          indent (pos + name_لمبائی, pos + max_name_لمبائی);
          pos += max_name_لمبائی;
        }
      حرف_لکھو (eolbyte);
    }
}

ساکن خالی
print_horizontal (خالی)
{
  size_t pos = 0;
  idx_t ستون_تعداد_2 = calculate_columns (false);
  ساخت عمود_info ثابت *سطر_fmt = &عمود_info[ستون_تعداد_2 - 1];
  ساخت fileinfo ثابت *f = sorted_file[0];
  size_t name_لمبائی = لمبائی_کا_file_name_and_frills (f);
  size_t max_name_لمبائی = سطر_fmt->ستون_arr[0];

  /* Print first entry.  */
  print_file_name_and_frills (f, 0);

  /* Now the rest.  */
  برائے (idx_t filesno = 1; filesno < cwd_n_مستعمل; filesno++)
    {
      idx_t ستون = filesno % ستون_تعداد_2;

      اگر (ستون == 0)
        {
          حرف_لکھو (eolbyte);
          pos = 0;
        }
      ورنہ
        {
          indent (pos + name_لمبائی, pos + max_name_لمبائی);
          pos += max_name_لمبائی;
        }

      f = sorted_file[filesno];
      print_file_name_and_frills (f, pos);

      name_لمبائی = لمبائی_کا_file_name_and_frills (f);
      max_name_لمبائی = سطر_fmt->ستون_arr[ستون];
    }
  حرف_لکھو (eolbyte);
}

/* Output name + SEP + ' '.  */

ساکن خالی
print_with_separator (حرف sep)
{
  size_t pos = 0;

  برائے (idx_t filesno = 0; filesno < cwd_n_مستعمل; filesno++)
    {
      ساخت fileinfo ثابت *f = sorted_file[filesno];
      size_t len = سطر_لمبائی ? لمبائی_کا_file_name_and_frills (f) : 0;

      اگر (filesno != 0)
        {
          حرف separator;
          size_t اگلا_pos = 2 + (sep == ',' && filesno < cwd_n_مستعمل - 1);

          اگر (! سطر_لمبائی
              || (! ckd_جوڑو (&اگلا_pos, اگلا_pos, pos)
                  && ! ckd_جوڑو (&اگلا_pos, اگلا_pos, len)
                  && اگلا_pos <= سطر_لمبائی))
            {
              pos += 2;
              separator = ' ';
            }
          ورنہ
            {
              pos = 0;
              separator = eolbyte;
            }

          حرف_لکھو (sep);
          حرف_لکھو (separator);
        }

      print_file_name_and_frills (f, pos);
      pos += len;
    }
  حرف_لکھو (eolbyte);
}

/* Assuming cursor is at position FROM, indent up to position TO.
   Use a TAB character instead of two or more spaces whenever possible.  */

ساکن خالی
indent (size_t from, size_t مقصد)
{
  جب (from < مقصد)
    {
      اگر (tabsize != 0 && مقصد / tabsize > (from + 1) / tabsize)
        {
          حرف_لکھو ('\t');
          from += tabsize - from % tabsize;
        }
      ورنہ
        {
          حرف_لکھو (' ');
          from++;
        }
    }
}

/* Put DIRNAME/NAME into DEST, handling '.' and '/' properly.  */
/* FIXME: maybe remove this function someday.  See about using a
   non-malloc'ing version of file_name_concat.  */

ساکن خالی
attach (حرف *dest, حرف ثابت *dirname, حرف ثابت *name)
{
  حرف ثابت *dirnamep = dirname;

  /* Copy dirname if it is not ".".  */
  اگر (dirname[0] != '.' || dirname[1] != 0)
    {
      جب (*dirnamep)
        *dest++ = *dirnamep++;
      /* Add '/' if 'dirname' doesn't already end with it.  */
      اگر (dirnamep > dirname && dirnamep[-1] != '/')
        *dest++ = '/';
    }
  جب (*name)
    *dest++ = *name++;
  *dest = 0;
}

/* Allocate enough column info suitable for the current number of
   files and display columns, and initialize the info to represent the
   narrowest possible columns.  */

ساکن خالی
init_عمود_info (idx_t max_ستون_تعداد)
{
  /* Currently allocated columns in column_info.  */
  ساکن idx_t عمود_info_alloc;

  اگر (عمود_info_alloc < max_ستون_تعداد)
    {
      idx_t old_عمود_info_alloc = عمود_info_alloc;
      عمود_info = xpalloc (عمود_info, &عمود_info_alloc,
                             max_ستون_تعداد - عمود_info_alloc, -1,
                             حجم *عمود_info);

      /* Allocate the new size_t objects by computing the triangle
         formula n * (n + 1) / 2, except that we don't need to
         allocate the part of the triangle that we've already
         allocated.  Check for address arithmetic overflow.  */
      idx_t عمود_info_growth = عمود_info_alloc - old_عمود_info_alloc, s;
      اگر (ckd_جوڑو (&s, old_عمود_info_alloc + 1, عمود_info_alloc)
          || ckd_mul (&s, s, عمود_info_growth))
        xalloc_die ();
      size_t *p = xinmalloc (s >> 1, حجم *p);

      /* Grow the triangle by parceling out the cells just allocated.  */
      برائے (idx_t i = old_عمود_info_alloc; i < عمود_info_alloc; i++)
        {
          عمود_info[i].ستون_arr = p;
          p += i + 1;
        }
    }

  برائے (idx_t i = 0; i < max_ستون_تعداد; ++i)
    {
      عمود_info[i].valid_len = true;
      عمود_info[i].سطر_len = i * MIN_عمود_چوڑائی + 1;
      برائے (idx_t j = 0; j < i; ++j)
        عمود_info[i].ستون_arr[j] = MIN_عمود_چوڑائی;
      عمود_info[i].ستون_arr[i] = 1;
    }
}

/* Calculate the number of columns needed to represent the current set
   of files in the current display width.  */

ساکن idx_t
calculate_columns (bool by_columns)
{
  /* Normally the maximum number of columns is determined by the
     screen width.  But if few files are available this might limit it
     as well.  */
  idx_t max_ستون_تعداد = 0 < max_idx && max_idx < cwd_n_مستعمل ? max_idx : cwd_n_مستعمل;

  init_عمود_info (max_ستون_تعداد);

  /* Compute the maximum number of possible columns.  */
  برائے (idx_t filesno = 0; filesno < cwd_n_مستعمل; ++filesno)
    {
      ساخت fileinfo ثابت *f = sorted_file[filesno];
      size_t name_لمبائی = لمبائی_کا_file_name_and_frills (f);

      برائے (idx_t i = 0; i < max_ستون_تعداد; ++i)
        {
          اگر (عمود_info[i].valid_len)
            {
              idx_t idx = (by_columns
                           ? filesno / ((cwd_n_مستعمل + i) / (i + 1))
                           : filesno % (i + 1));
              size_t real_لمبائی = name_لمبائی + (idx == i ? 0 : 2);

              اگر (عمود_info[i].ستون_arr[idx] < real_لمبائی)
                {
                  عمود_info[i].سطر_len += (real_لمبائی
                                              - عمود_info[i].ستون_arr[idx]);
                  عمود_info[i].ستون_arr[idx] = real_لمبائی;
                  عمود_info[i].valid_len = (عمود_info[i].سطر_len
                                              <= سطر_لمبائی);
                }
            }
        }
    }

  /* Find maximum allowed columns.  */
  idx_t ستون_تعداد_2;
  برائے (ستون_تعداد_2 = max_ستون_تعداد; 1 < ستون_تعداد_2; --ستون_تعداد_2)
    {
      اگر (عمود_info[ستون_تعداد_2 - 1].valid_len)
        توڑو;
    }

  واپس ستون_تعداد_2;
}

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("Usage: %s [OPTION]... [FILE]...\n"), پروگرام_name);
      fputs (_("\
List information about the FILEs (the current directory by طےشدہ).\n\
ترتیب_دو_2 entries alphabetically اگر none کا -cftuvSUX nor --ترتیب_دو is specified.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -a, --all\n\
         کرو not ignore entries starting with .\n\
"));
      oputs (_("\
  -A, --almost-all\n\
         کرو not list implied . and ..\n\
"));
      oputs (_("\
      --author\n\
         with -l, print the author کا each file\n\
"));
      oputs (_("\
  -b, --escape\n\
         print C-style escapes برائے nongraphic characters\n\
"));
      oputs (_("\
      --بلاک-سائز=سائز_2\n\
         with -l, scale sizes by سائز_2 when printing them;\n\
         e.g., '--block-size=M'; see سائز_2 format نیچے\n\
"));
      oputs (_("\
  -B, --ignore-backups\n\
         کرو not list implied entries ending with ~\n\
"));
      oputs (_("\
  -c\n\
         with -lt: ترتیب_دو by, and show, ctime\n\
           (وقت کا آخری change کا file حالت_2 information);\n\
         with -l: show ctime and ترتیب_دو by name;\n\
         otherwise: ترتیب_دو by ctime, newest پہلا\n\
"));
      oputs (_("\
  -C\n\
         list entries by columns\n\
"));
      oputs (_("\
      --color[=WHEN]\n\
         color the output WHEN; مزید info نیچے\n\
"));
      oputs (_("\
  -d, --directory\n\
         list directories themselves, not their contents\n\
"));
      oputs (_("\
  -D, --dired\n\
         generate output designed برائے Emacs' dired منوال_2\n\
"));
      oputs (_("\
  -f\n\
         same as -a -U\n\
"));
      oputs (_("\
  -F, --classify[=WHEN]\n\
         append indicator (one کا */=>@|) مقصد entries WHEN\n\
"));
      oputs (_("\
      --file-type\n\
         like -F, except کرو not append '*'\n\
"));
      oputs (_("\
      --format=لفظ\n\
         across,horizontal (-x), commas (-m), طویل (-l),\n\
         single-عمود (-1), verbose (-l), vertical (-C)\n\
"));
      oputs (_("\
      --بھرا-وقت\n\
         like -l --وقت-style=بھرا-iso\n\
"));
      oputs (_("\
  -g\n\
         like -l, but کرو not list owner\n\
"));
      oputs (_("\
      --گروہ-directories-پہلا\n\
         گروہ directories before files\n\
"));
      oputs (_("\
  -G, --no-گروہ\n\
         اندر a طویل listing, don't print گروہ نام\n\
"));
      oputs (_("\
  -h, --human-readable\n\
         with -l and -s, print sizes like 1K 234M 2G etc.\n\
"));
      oputs (_("\
      --si\n\
         likewise, but use powers کا 1000 not 1024\n\
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
      --hide=نمونہ\n\
         کرو not list implied entries matching shell نمونہ\n\
         (overridden by -a or -A)\n\
"));
      oputs (_("\
      --hyperlink[=WHEN]\n\
         hyperlink file نام WHEN\n\
"));
      oputs (_("\
      --indicator-style=لفظ\n\
         append indicator with style لفظ مقصد اندراج_2 نام:\n\
           none (طےشدہ), slash (-p), file-type (--file-type), classify (-F)\n\
"));
      oputs (_("\
  -i, --inode\n\
         print the index نمبر کا each file\n\
"));
      oputs (_("\
  -I, --ignore=نمونہ\n\
         کرو not list implied entries matching shell نمونہ\n\
"));
      oputs (_("\
  -k, --kibibytes\n\
         طےشدہ مقصد 1024-byte بلاکیں برائے file system usage;\n\
         مستعمل only with -s and فی directory totals\n\
"));
      oputs (_("\
  -l\n\
         use a طویل listing format\n\
"));
      oputs (_("\
  -L, --dereference\n\
         when showing file information برائے a symbolic link,\n\
         show information برائے the file the link references\n\
         rather than برائے the link itself\n\
"));
      oputs (_("\
  -m\n\
         fill چوڑائی with a comma separated list کا entries\n\
"));
      oputs (_("\
  -n, --numeric-uid-gid\n\
         like -l, but list numeric user and گروہ IDs\n\
"));
      oputs (_("\
  -N, --literal\n\
         print اندراج_2 نام without quoting\n\
"));
      oputs (_("\
  -o\n\
         like -l, but کرو not list گروہ information\n\
"));
      oputs (_("\
  -p, --indicator-style=slash\n\
         append / indicator مقصد directories\n\
"));
      oputs (_("\
  -q, --hide-control-chars\n\
         print ? instead کا nongraphic characters\n\
"));
      oputs (_("\
      --show-control-chars\n\
         show nongraphic characters as-is;\n\
         the طےشدہ, unless پروگرام is 'ls' and output is a terminal\n\
"));
      oputs (_("\
  -Q, --quote-name\n\
         enclose اندراج_2 نام اندر دوہرا quotes\n\
"));
      oputs (_("\
      --quoting-style=لفظ\n\
         use quoting style لفظ برائے اندراج_2 نام:\n\
           literal, locale, shell, shell-always,\n\
           shell-escape, shell-escape-always, c, escape\n\
         (overrides QUOTING_STYLE environment variable)\n\
"));
      oputs (_("\
  -r, --الٹ\n\
         الٹ ترتیب جب ترتیب_کاری\n\
"));
      oputs (_("\
  -R, --recursive\n\
         list subdirectories recursively\n\
"));
      oputs (_("\
  -s, --سائز\n\
         print the allocated سائز کا each file, اندر بلاکیں\n\
"));
      oputs (_("\
  -S\n\
         ترتیب_دو by file سائز, سب_سے_بڑا پہلا\n\
"));
      oputs (_("\
      --ترتیب_دو=لفظ\n\
         change طےشدہ 'name' ترتیب_دو مقصد لفظ:\n\
           none (-U), سائز (-S), وقت (-t),\n\
           نسخہ_3 (-v), extension (-X), name, چوڑائی\n\
"));
      oputs (_("\
      --وقت=لفظ\n\
         select which timestamp مستعمل مقصد display or ترتیب_دو;\n\
           access وقت (-u): atime, access, use;\n\
           metadata change وقت (-c): ctime, حالت_2;\n\
           modified وقت (طےشدہ): mtime, modification;\n\
           birth وقت: birth, creation;\n\
         with -l, لفظ determines which وقت مقصد show;\n\
         with --ترتیب_دو=وقت, ترتیب_دو by لفظ (newest پہلا)\n\
"));
      oputs (_("\
      --وقت-style=وقت_STYLE\n\
         وقت/date format with -l; see وقت_STYLE نیچے\n\
"));
      oputs (_("\
  -t\n\
         ترتیب_دو by وقت, newest پہلا; see --وقت\n\
"));
      oputs (_("\
  -T, --tabsize=ستون_تعداد\n\
         assume tab stops at each ستون_تعداد instead کا 8\n\
"));
      oputs (_("\
  -u\n\
         with -lt: ترتیب_دو by, and show, access وقت;\n\
         with -l: show access وقت and ترتیب_دو by name;\n\
         otherwise: ترتیب_دو by access وقت, newest پہلا\n\
"));
      oputs (_("\
  -U\n\
         کرو not ترتیب_دو directory entries\n\
"));
      oputs (_("\
  -v\n\
         natural ترتیب_دو کا (نسخہ_3) numbers within متن\n\
"));
      oputs (_("\
  -w, --چوڑائی=ستون_تعداد\n\
         سیٹ_2 output چوڑائی مقصد ستون_تعداد.  0 means no حد_بندی\n\
"));
      oputs (_("\
  -x\n\
         list entries by سطریں instead کا by columns\n\
"));
      oputs (_("\
  -X\n\
         ترتیب_دو alphabetically by اندراج_2 extension\n\
"));
      oputs (_("\
  -Z, --context\n\
         print any security context کا each file\n\
"));
      oputs (_("\
      --zero\n\
         انتہا each output سطر with NUL, not newline\n\
"));
      oputs (_("\
  -1\n\
         list one file فی سطر\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      emit_سائز_note ();
      fputs (_("\
\n\
The وقت_STYLE دلیل can be بھرا-iso, طویل-iso, iso, locale, or +FORMAT.\n\
FORMAT is interpreted like اندر date(1).  If FORMAT is FORMAT1<newline>FORMAT2,\n\
then FORMAT1 applies مقصد non-recent files and FORMAT2 مقصد recent files.\n\
وقت_STYLE prefixed with 'posix-' takes effect only outside the POSIX locale.\n\
Also the وقت_STYLE environment variable sets the طےشدہ style مقصد use.\n\
"), stdout);
      fputs (_("\
\n\
The WHEN دلیل defaults مقصد 'always' and can also be 'auto' or 'never'.\n\
"), stdout);
      fputs (_("\
\n\
Using color مقصد distinguish file types is disabled both by طےشدہ and\n\
with --color=never.  With --color=خودکار, ls emits color codes only when\n\
standard output is connected مقصد a terminal.  The LS_COLORS environment\n\
variable can change the settings.  Use the dircolors(1) command مقصد سیٹ_2 it.\n\
"), stdout);
      fputs (_("\
\n\
Exit حالت_2:\n\
 0  اگر OK,\n\
 1  اگر minor problems (e.g., cannot access subdirectory),\n\
 2  اگر serious trouble (e.g., cannot access command-سطر دلیل).\n\
"), stdout);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}
