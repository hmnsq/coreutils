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
#جيڪڏهن_پورو
#ضمن <sys/ioctl.h>

#جيڪڏهن_وصف_ٿيل WINSIZE_اندريون_PTEM
# include <sys/stream.h>
# include <sys/ptem.h>
#جيڪڏهن_پورو

#ضمن <stdio.h>
#ضمن <setjmp.h>
#ضمن <pwd.h>
#ضمن <getopt.h>
#ضمن <signal.h>

#إذا HAVE_LANGINFO_CODESET
# include <langinfo.h>
#جيڪڏهن_پورو

/* NonStop circa 2011 lacks both SA_RESTART and siginterrupt, so don't
   restart syscalls after a signal handler fires.  This may cause
   colors to get messed up on the screen if 'ls' is interrupted, but
   that's the best we can do on such a platform.  */
#جيڪڏهن_وصف_نه_ٿيل SA_RESTART
# define SA_RESTART 0
#جيڪڏهن_پورو

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
#جيڪڏهن_وصف_ٿيل HAVE_CAP
# include <sys/capability.h>
#جيڪڏهن_پورو

#إذا HAVE_LINUX_XATTR_H
# include <linux/xattr.h>
# ifndef XATTR_NAME_CAPS
#  define XATTR_NAME_CAPS "security.capability"
# endif
#جيڪڏهن_پورو

#وصف_ڪريو پروگرام_NAME (ls_گهڻائي == LS_LS ? "ls" \
                      : (ls_گهڻائي == LS_MULTI_کانو \
                         ? "dir" : "vdir"))

#وصف_ڪريو AUTHORS \
  proper_name ("Richard M. Stallman"), \
  proper_name ("David MacKenzie")

#وصف_ڪريو obstack_chunk_alloc يادگيري_ڏيو
#وصف_ڪريو obstack_chunk_free حرر

/* Unix-based readdir implementations have historically returned a dirent.d_ino
   value that is sometimes not equal to the stat-obtained st_ino value for
   that same entry.  This error occurs for a readdir entry that refers
   to a mount point.  readdir's error is to return the inode number of
   the underlying directory -- one that typically cannot be stat'ed, as
   long as a file system is mounted on that directory.  RELIABLE_D_INO
   encapsulates whether we can use the more efficient approach of relying
   on readdir-supplied d_ino values, or whether we must incur the cost of
   calling stat or lstat to obtain each guaranteed-valid inode number.  */

#جيڪڏهن_وصف_نه_ٿيل READDIR_LIES_ABOUT_MOUNTPOINT_D_INO
# define READDIR_LIES_ABOUT_MOUNTPOINT_D_INO 1
#جيڪڏهن_پورو

#إذا READDIR_LIES_ABOUT_MOUNTPOINT_D_INO
# define RELIABLE_D_INO(dp) NOT_AN_INODE_عدد
#وإلا
# define RELIABLE_D_INO(dp) D_INO (dp)
#جيڪڏهن_پورو

#إذا ! HAVE_STRUCT_STAT_ST_AUTHOR
# define st_author st_uid
#جيڪڏهن_پورو

تعداد filetype
  {
    unknown,
    fifo,
    chardev,
    directory,
    blockdev,
    عام_2,
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
ساكن بنانشان حرف ثابت filetype_d_type[] =
  {
    DT_UNKNOWN, DT_FIFO, DT_CHR, DT_DIR, DT_BLK, DT_REG, DT_LNK, DT_SOCK,
    DT_WHT, DT_DIR
  };
static_assert (countof (filetype_d_type) == filetype_cardinality);

/* Map d_type values to enum filetype.  */
ساكن حرف ثابت d_type_filetype[UCHAR_MAX + 1] =
  {
    [DT_BLK] = blockdev, [DT_CHR] = chardev, [DT_DIR] = directory,
    [DT_FIFO] = fifo, [DT_LNK] = symbolic_link, [DT_REG] = عام_2,
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
    size_t ويڪر;
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
#جيڪڏهن_پورو

ساكن size_t quote_name (حرف ثابت *name,
                          بنية quoting_options ثابت *options,
                          صحيح needs_general_quoting,
                          ثابت بنية bin_str *color,
                          bool allow_pad, بنية obstack *ڍير,
                          حرف ثابت *absolute_name);
ساكن size_t quote_name_buf (حرف **inbuf, size_t bufsize, حرف *name,
                              بنية quoting_options ثابت *options,
                              صحيح needs_general_quoting, size_t *ويڪر,
                              bool *pad);
ساكن صحيح decode_switches (صحيح argc, حرف **argv);
ساكن bool file_ignored (حرف ثابت *name);
ساكن uintmax_t gobble_file (حرف ثابت *name, تعداد filetype type,
                              ino_t inode, bool command_سٽ_arg,
                              حرف ثابت *dirname);
ساكن ثابت بنية bin_str * get_color_indicator (ثابت بنية fileinfo *f,
                                                   bool symlink_هدف);
ساكن bool print_color_indicator (ثابت بنية bin_str *ind);
ساكن فراغ put_indicator (ثابت بنية bin_str *ind);
ساكن فراغ جوڙيو_ignore_نمونو (حرف ثابت *نمونو_2);
ساكن فراغ attach (حرف *dest, حرف ثابت *dirname, حرف ثابت *name);
ساكن فراغ clear_files (فراغ);
ساكن فراغ ڪڍو_dirs_from_files (حرف ثابت *dirname,
                                     bool command_سٽ_arg);
ساكن bool get_link_name (حرف ثابت *filename, بنية fileinfo *f,
                           bool command_سٽ_arg);
ساكن فراغ indent (size_t from, size_t منزل);
ساكن idx_t calculate_columns (bool by_columns);
ساكن فراغ print_current_files (فراغ);
ساكن فراغ print_dir (حرف ثابت *name, حرف ثابت *realname,
                       bool command_سٽ_arg);
ساكن size_t print_file_name_and_frills (ثابت بنية fileinfo *f,
                                          size_t شروعات_کانو);
ساكن فراغ print_horizontal (فراغ);
ساكن صحيح format_user_ويڪر (uid_t u);
ساكن صحيح format_گروهه_ويڪر (gid_t g);
ساكن فراغ print_long_format (ثابت بنية fileinfo *f);
ساكن فراغ print_many_في_سٽ (فراغ);
ساكن size_t print_name_with_quoting (ثابت بنية fileinfo *f,
                                       bool symlink_هدف,
                                       بنية obstack *ڍير,
                                       size_t شروعات_کانو);
ساكن فراغ prep_non_filename_متن (فراغ);
ساكن bool print_type_indicator (bool stat_ok, mode_t گهڻائي_2,
                                  تعداد filetype type);
ساكن فراغ print_with_separator (حرف sep);
ساكن فراغ قطار_directory (حرف ثابت *name, حرف ثابت *realname,
                             bool command_سٽ_arg);
ساكن فراغ ترتيب_ڏيو_files (فراغ);
ساكن فراغ parse_ls_color (فراغ);

ساكن size_t quote_name_ويڪر (حرف ثابت *name,
                                بنية quoting_options ثابت *options,
                                صحيح needs_general_quoting);

/* Initial size of hash table.
   Most hierarchies are likely to be shallower than this.  */
تعداد { INITIAL_جدول_سائيز = 30 };

/* The set of 'active' directories, from the current command-line argument
   to the level in the hierarchy at which files are being listed.
   A directory is represented by its device and inode numbers (struct dev_ino).
   A directory is added to this set when ls begins listing it or its
   entries, and it is removed from the set just after ls has finished
   processing it.  This set is used solely to detect loops, e.g., with
   mkdir loop; cd loop; ln -s ../loop sub; ls -RL  */
ساكن Hash_جدول *سرگرم_dir_سيٽ;

#وصف_ڪريو LOOP_DETECT (!!سرگرم_dir_سيٽ)

/* The table of files in the current directory:

   'cwd_file' points to a vector of 'struct fileinfo', one per file.
   'cwd_n_alloc' is the number of elements space has been allocated for.
   'cwd_n_used' is the number actually in use.  */

/* Address of block containing the files that are described.  */
ساكن بنية fileinfo *cwd_file;

/* Length of block that 'cwd_file' points to, measured in files.  */
ساكن idx_t cwd_n_alloc;

/* Index of first unused slot in 'cwd_file'.  */
ساكن idx_t cwd_n_استعمال_ٿيل;

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
file_or_link_گهڻائي (بنية fileinfo ثابت *file)
{
  أرجع (color_symlink_as_referent && file->linkok
          ? file->linkmode : file->stat.st_گهڻائي);
}


/* Record of one pending directory waiting to be listed.  */

بنية pending
  {
    حرف *name;
    /* If the directory is actually the file pointed to by a symbolic link we
       were told to list, 'realname' will contain the name of the symbolic
       link, otherwise zero.  */
    حرف *realname;
    bool command_سٽ_arg;
    بنية pending *ايندڙ;
  };

ساكن بنية pending *pending_dirs;

/* Current time in seconds and nanoseconds since 1970, updated as
   needed when deciding whether a file is recent.  */

ساكن بنية timespec current_وقت;

ساكن bool print_scontext;
ساكن حرف UNKNOWN_SECURITY_CONTEXT[] = "?";

/* Whether any of the files has an ACL.  This affects the width of the
   mode column.  */

ساكن bool any_has_acl;

/* The number of columns to use for columns containing inode numbers,
   block sizes, link counts, owners, groups, authors, major device
   numbers, minor device numbers, and file sizes, respectively.  */

ساكن صحيح inode_عدد_ويڪر;
ساكن صحيح بلاڪ_سائيز_ويڪر;
ساكن صحيح nlink_ويڪر;
ساكن صحيح scontext_ويڪر;
ساكن صحيح owner_ويڪر;
ساكن صحيح گروهه_ويڪر;
ساكن صحيح author_ويڪر;
ساكن صحيح major_device_عدد_ويڪر;
ساكن صحيح minor_device_عدد_ويڪر;
ساكن صحيح file_سائيز_ويڪر;

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
    one_في_سٽ,		/* -1 */
    many_في_سٽ,		/* -C */
    horizontal,			/* -x */
    with_commas			/* -m */
  };

ساكن تعداد format format;

/* 'full-iso' uses full ISO-style dates and times.  'long-iso' uses longer
   ISO-style timestamps, though shorter than 'full-iso'.  'iso' uses shorter
   ISO-style timestamps.  'locale' uses locale-dependent timestamps.  */
تعداد وقت_style
  {
    ڀريل_iso_وقت_style,	/* --time-style=full-iso */
    long_iso_وقت_style,	/* --time-style=long-iso */
    iso_وقت_style,		/* --time-style=iso */
    locale_وقت_style		/* --time-style=locale */
  };

ساكن حرف ثابت *ثابت وقت_style_دليلون[] =
{
  "full-iso", "long-iso", "iso", "locale", عدم
};
ساكن تعداد وقت_style ثابت وقت_style_types[] =
{
  ڀريل_iso_وقت_style, long_iso_وقت_style, iso_وقت_style,
  locale_وقت_style
};
ARGMATCH_VERIFY (وقت_style_دليلون, وقت_style_types);

/* Type of time to print or sort by.  Controlled by -c and -u.
   The values of each item of this enum are important since they are
   used as indices in the sort functions array (see sort_files()).  */

تعداد وقت_type
  {
    وقت_mtime = 0,		/* default */
    وقت_ctime,			/* -c */
    وقت_atime,			/* -u */
    وقت_btime,                 /* birth time */
    وقت_numtypes		/* the number of elements of this enum */
  };

ساكن تعداد وقت_type وقت_type;
ساكن bool explicit_وقت;

/* The file characteristic to sort by.  Controlled by -t, -S, -U, -X, -v.
   The values of each item of this enum are important since they are
   used as indices in the sort functions array (see sort_files()).  */

تعداد ترتيب_ڏيو_type
  {
    ترتيب_ڏيو_name = 0,		/* default */
    ترتيب_ڏيو_extension,		/* -X */
    ترتيب_ڏيو_ويڪر,
    ترتيب_ڏيو_سائيز,			/* -S */
    ترتيب_ڏيو_نسخو,		/* -v */
    ترتيب_ڏيو_وقت,			/* -t; must be second to last */
    ترتيب_ڏيو_none,			/* -U; must be last */
    ترتيب_ڏيو_numtypes		/* the number of elements of this enum */
  };

ساكن تعداد ترتيب_ڏيو_type ترتيب_ڏيو_type;

/* Direction of sort.
   false means highest first if numeric,
   lowest first if alphabetic;
   these are the defaults.
   true means the opposite order in each case.  -r  */

ساكن bool ترتيب_ڏيو_ابتو;

/* True means to display owner information.  -g turns this off.  */

ساكن bool print_owner = true;

/* True means to display author information.  */

ساكن bool print_author;

/* True means to display group information.  -G and -o turn this off.  */

ساكن bool print_گروهه = true;

/* True means print the user and group id's as numbers rather
   than as names.  -n  */

ساكن bool numeric_ids;

/* True means mention the size in blocks of each file.  -s  */

ساكن bool print_بلاڪ_سائيز;

/* Human-readable options for output, when printing block counts.  */
ساكن صحيح human_output_opts;

/* The units to use when printing block counts.  */
ساكن uintmax_t output_بلاڪ_سائيز;

/* Likewise, but for file sizes.  */
ساكن صحيح file_human_output_opts;
ساكن uintmax_t file_output_بلاڪ_سائيز = 1;

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
ساكن حرف ثابت *ثابت indicator_style_دليلون[] =
{
  "none", "slash", "file-type", "classify", عدم
};
ساكن تعداد indicator_style ثابت indicator_style_types[] =
{
  none, slash, file_type, classify
};
ARGMATCH_VERIFY (indicator_style_دليلون, indicator_style_types);

/* True means use colors to mark types.  Also define the different
   colors as well as the stuff for the LS_COLORS environment variable.
   The LS_COLORS variable is now in a termcap-like format.  */

ساكن bool print_with_color;

ساكن bool print_hyperlink;

/* Whether we used any colors in the output so far.  If so, we will
   need to restore the default color later.  If not, we will need to
   call prep_non_filename_text before using color for the first time. */

ساكن bool استعمال_ٿيل_color = false;

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
    DEREF_COMMAND_سٽ_ARGUMENTS,	/* -H */
    DEREF_COMMAND_سٽ_SYMLINK_منزل_DIR,	/* the default, in certain cases */
    DEREF_ALWAYS			/* -L */
  };

تعداد indicator_no
  {
    C_کاٻو, C_ساڄو, C_پڇاڙي, C_RESET, C_NORM, C_FILE, C_DIR, C_LINK,
    C_FIFO, C_SOCK,
    C_BLK, C_CHR, C_MISSING, C_ORPHAN, C_EXEC, C_DOOR, C_SETUID, C_SETGID,
    C_STICKY, C_OTHER_WRITABLE, C_STICKY_OTHER_WRITABLE, C_CAP, C_MULTIHARDLINK,
    C_CLR_منزل_EOL
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
    bool   exact_ميلاپ;		/* Whether to compare case insensitively */
    بنية color_ext_type *ايندڙ;	/* Next in list */
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

ساكن bool check_symlink_گهڻائي;

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

ساكن bool فوري_dirs;

/* True means that directories are grouped before files. */

ساكن bool directories_پهريون;

/* Which files to ignore.  */

ساكن تعداد
{
  /* Ignore files whose names start with '.', and files specified by
     --hide and --ignore.  */
  IGNORE_DEFAULT = 0,

  /* Ignore '.', '..', and files specified by --ignore.  */
  IGNORE_DOT_AND_DOTDOT,

  /* Ignore only files specified by --ignore.  */
  IGNORE_گهٽتر
} ignore_گهڻائي;

/* A linked list of shell-style globbing patterns.  If a non-argument
   file name matches any of these patterns, it is ignored.
   Controlled by -I.  Multiple -I options accumulate.
   The -B option adds '*~' and '.*~' to this list.  */

بنية ignore_نمونو
  {
    حرف ثابت *نمونو_2;
    بنية ignore_نمونو *ايندڙ;
  };

ساكن بنية ignore_نمونو *ignore_patterns;

/* Similar to IGNORE_PATTERNS, except that -a or -A causes this
   variable itself to be ignored.  */
ساكن بنية ignore_نمونو *hide_patterns;

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

ساكن size_t سٽ_ڊيگهائي;

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

تعداد { وقت_STAMP_LEN_وڌ_کان_وڌ = MAX (1000, INT_STRLEN_BOUND (time_t)) };

/* strftime formats for non-recent and recent files, respectively, in
   -l output.  */

ساكن حرف ثابت *long_وقت_format[2] =
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

ساكن sigset_t caught_سگنل;

/* If nonzero, the value of the pending fatal signal.  */

ساكن sig_atomic_t متطاير مداخلت_signal;

/* A count of the number of pending stop signals that have been received.  */

ساكن sig_atomic_t متطاير stop_signal_ڳڻپ;

/* Desired exit status.  */

ساكن صحيح exit_حالت;

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
  بلاڪ_سائيز_OPTION,
  COLOR_OPTION,
  DEREFERENCE_COMMAND_سٽ_SYMLINK_منزل_DIR_OPTION,
  FILE_TYPE_INDICATOR_OPTION,
  FORMAT_OPTION,
  ڀريل_وقت_OPTION,
  گروهه_DIRECTORIES_پهريون_OPTION,
  HIDE_OPTION,
  HYPERLINK_OPTION,
  INDICATOR_STYLE_OPTION,
  QUOTING_STYLE_OPTION,
  SHOW_CONTROL_CHARS_OPTION,
  SI_OPTION,
  ترتيب_ڏيو_OPTION,
  وقت_OPTION,
  وقت_STYLE_OPTION,
  ZERO_OPTION,
};

ساكن بنية option ثابت long_options[] =
{
  {"all", no_دليل, عدم, 'a'},
  {"escape", no_دليل, عدم, 'b'},
  {"directory", no_دليل, عدم, 'd'},
  {"dired", no_دليل, عدم, 'D'},
  {"full-time", no_دليل, عدم, ڀريل_وقت_OPTION},
  {"group-directories-first", no_دليل, عدم,
   گروهه_DIRECTORIES_پهريون_OPTION},
  {"human-readable", no_دليل, عدم, 'h'},
  {"inode", no_دليل, عدم, 'i'},
  {"kibibytes", no_دليل, عدم, 'k'},
  {"numeric-uid-gid", no_دليل, عدم, 'n'},
  {"no-group", no_دليل, عدم, 'G'},
  {"hide-control-chars", no_دليل, عدم, 'q'},
  {"reverse", no_دليل, عدم, 'r'},
  {"size", no_دليل, عدم, 's'},
  {"width", required_دليل, عدم, 'w'},
  {"almost-all", no_دليل, عدم, 'A'},
  {"ignore-backups", no_دليل, عدم, 'B'},
  {"classify", optional_دليل, عدم, 'F'},
  {"file-type", no_دليل, عدم, FILE_TYPE_INDICATOR_OPTION},
  {"si", no_دليل, عدم, SI_OPTION},
  {"dereference-command-line", no_دليل, عدم, 'H'},
  {"dereference-command-line-symlink-to-dir", no_دليل, عدم,
   DEREFERENCE_COMMAND_سٽ_SYMLINK_منزل_DIR_OPTION},
  {"hide", required_دليل, عدم, HIDE_OPTION},
  {"ignore", required_دليل, عدم, 'I'},
  {"indicator-style", required_دليل, عدم, INDICATOR_STYLE_OPTION},
  {"dereference", no_دليل, عدم, 'L'},
  {"literal", no_دليل, عدم, 'N'},
  {"quote-name", no_دليل, عدم, 'Q'},
  {"quoting-style", required_دليل, عدم, QUOTING_STYLE_OPTION},
  {"recursive", no_دليل, عدم, 'R'},
  {"format", required_دليل, عدم, FORMAT_OPTION},
  {"show-control-chars", no_دليل, عدم, SHOW_CONTROL_CHARS_OPTION},
  {"sort", required_دليل, عدم, ترتيب_ڏيو_OPTION},
  {"tabsize", required_دليل, عدم, 'T'},
  {"time", required_دليل, عدم, وقت_OPTION},
  {"time-style", required_دليل, عدم, وقت_STYLE_OPTION},
  {"zero", no_دليل, عدم, ZERO_OPTION},
  {"color", optional_دليل, عدم, COLOR_OPTION},
  {"hyperlink", optional_دليل, عدم, HYPERLINK_OPTION},
  {"block-size", required_دليل, عدم, بلاڪ_سائيز_OPTION},
  {"context", no_دليل, عدم, 'Z'},
  {"author", no_دليل, عدم, AUTHOR_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخو_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

ساكن حرف ثابت *ثابت format_دليلون[] =
{
  "verbose", "long", "commas", "horizontal", "across",
  "vertical", "single-column", عدم
};
ساكن تعداد format ثابت format_types[] =
{
  long_format, long_format, with_commas, horizontal, horizontal,
  many_في_سٽ, one_في_سٽ
};
ARGMATCH_VERIFY (format_دليلون, format_types);

ساكن حرف ثابت *ثابت ترتيب_ڏيو_دليلون[] =
{
  "none", "size", "time", "version", "extension",
  "name", "width", عدم
};
ساكن تعداد ترتيب_ڏيو_type ثابت ترتيب_ڏيو_types[] =
{
  ترتيب_ڏيو_none, ترتيب_ڏيو_سائيز, ترتيب_ڏيو_وقت, ترتيب_ڏيو_نسخو, ترتيب_ڏيو_extension,
  ترتيب_ڏيو_name, ترتيب_ڏيو_ويڪر
};
ARGMATCH_VERIFY (ترتيب_ڏيو_دليلون, ترتيب_ڏيو_types);

ساكن حرف ثابت *ثابت وقت_دليلون[] =
{
  "atime", "access", "use",
  "ctime", "status",
  "mtime", "modification",
  "birth", "creation",
  عدم
};
ساكن تعداد وقت_type ثابت وقت_types[] =
{
  وقت_atime, وقت_atime, وقت_atime,
  وقت_ctime, وقت_ctime,
  وقت_mtime, وقت_mtime,
  وقت_btime, وقت_btime,
};
ARGMATCH_VERIFY (وقت_دليلون, وقت_types);

ساكن حرف ثابت *ثابت when_دليلون[] =
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
ARGMATCH_VERIFY (when_دليلون, when_types);

/* Information about filling a column.  */
بنية ڪالم_info
{
  bool valid_len;
  size_t سٽ_len;
  size_t *کانو_arr;
};

/* Array with information about column fullness.  */
ساكن بنية ڪالم_info *ڪالم_info;

/* Maximum number of columns ever possible for this display.  */
ساكن size_t max_idx;

/* The minimum width of a non-final column is 3: 1 character for the name
   and 2 for the separating white space.  The final column is not padded.  */
تعداد { MIN_ڪالم_ويڪر = 3 };


/* This zero-based index is for the --dired option.  It is incremented
   for each byte of output generated by this program so that the beginning
   and ending indices (in that output) of every file name can be recorded
   and later output themselves.  */
ساكن off_t dired_pos;

ساكن فراغ
dired_outbyte (حرف c)
{
  dired_pos++;
  اکر_لکو (c);
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
  dired_outbuf (s, لڙي_ڊيگهه (s));
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
ڌڪو_current_dired_pos (بنية obstack *obs)
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
dev_ino_ڌڪو (dev_t dev, ino_t ino)
{
  فراغ *vdi;
  بنية dev_ino *di;
  صحيح dev_ino_سائيز = حجم *di;
  obstack_blank (&dev_ino_obstack, dev_ino_سائيز);
  vdi = obstack_ايندڙ_free (&dev_ino_obstack);
  di = vdi;
  di--;
  di->st_dev = dev;
  di->st_ino = ino;
}

/* Pop a dev/ino struct off the global dev_ino_obstack
   and return that struct.  */
ساكن بنية dev_ino
dev_ino_ڪڍو (فراغ)
{
  فراغ *vdi;
  بنية dev_ino *di;
  صحيح dev_ino_سائيز = حجم *di;
  affirm (dev_ino_سائيز <= obstack_object_سائيز (&dev_ino_obstack));
  obstack_blank_fast (&dev_ino_obstack, -dev_ino_سائيز);
  vdi = obstack_ايندڙ_free (&dev_ino_obstack);
  di = vdi;
  أرجع *di;
}

ساكن فراغ
assert_matching_dev_ino (حرف ثابت *name, بنية dev_ino di)
{
  MAYBE_اڻ_استعمال_ٿيل بنية stat sb;
  assure (0 <= stat (name, &sb));
  assure (sb.st_dev == di.st_dev);
  assure (sb.st_ino == di.st_ino);
}

ساكن حرف eolbyte = '\n';

/* Write to standard output PREFIX, followed by the quoting style and
   a space-separated list of the integers stored in OS all on one line.  */

ساكن فراغ
dired_dump_obstack (حرف ثابت *اڳياڙي, بنية obstack *os)
{
  size_t n_pos;

  n_pos = obstack_object_سائيز (os) / حجم (dired_pos);
  إذا (n_pos > 0)
    {
      off_t *pos = obstack_پڄاڻي (os);
      fputs (اڳياڙي, stdout);
      لكل (size_t i = 0; i < n_pos; i++)
        {
          intmax_t p = pos[i];
          لکو (" %jd", p);
        }
      اکر_لکو ('\n');
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
#جيڪڏهن_پورو

  أرجع btimespec;
}

#إذا HAVE_STATX && defined STATX_INO
ATTRIBUTE_خالص
ساكن بنانشان صحيح
وقت_type_منزل_statx (فراغ)
{
  بدل (وقت_type)
    {
    حالة وقت_ctime:
      أرجع STATX_CTIME;
    حالة وقت_mtime:
      أرجع STATX_MTIME;
    حالة وقت_atime:
      أرجع STATX_ATIME;
    حالة وقت_btime:
      أرجع STATX_BTIME;
    حالة وقت_numtypes: افتراضي:
      unreachable ();
    }
    أرجع 0;
}

ATTRIBUTE_خالص
ساكن بنانشان صحيح
calc_req_نقاب (فراغ)
{
  بنانشان صحيح نقاب = STATX_گهڻائي;

  إذا (print_inode)
    نقاب |= STATX_INO;

  إذا (print_بلاڪ_سائيز)
    نقاب |= STATX_بلاڪون;

  إذا (format == long_format) {
    نقاب |= STATX_NLINK | STATX_سائيز | وقت_type_منزل_statx ();
    إذا (print_owner || print_author)
      نقاب |= STATX_UID;
    إذا (print_گروهه)
      نقاب |= STATX_GID;
  }

  بدل (ترتيب_ڏيو_type)
    {
    حالة ترتيب_ڏيو_none:
    حالة ترتيب_ڏيو_name:
    حالة ترتيب_ڏيو_نسخو:
    حالة ترتيب_ڏيو_extension:
    حالة ترتيب_ڏيو_ويڪر:
      اكسر;
    حالة ترتيب_ڏيو_وقت:
      نقاب |= وقت_type_منزل_statx ();
      اكسر;
    حالة ترتيب_ڏيو_سائيز:
      نقاب |= STATX_سائيز;
      اكسر;
    حالة ترتيب_ڏيو_numtypes: افتراضي:
      unreachable ();
    }

  أرجع نقاب;
}

ساكن صحيح
do_statx (صحيح fd, حرف ثابت *name, بنية stat *st, صحيح flags,
          بنانشان صحيح نقاب)
{
  بنية statx stx;
  bool want_btime = نقاب & STATX_BTIME;
  صحيح ret = statx (fd, name, flags | AT_NO_AUTOMOUNT, نقاب, &stx);
  إذا (ret >= 0)
    {
      statx_منزل_stat (&stx, st);
      /* Since we only need one timestamp type,
         store birth time in st_mtim.  */
      إذا (want_btime)
        {
          إذا (stx.stx_نقاب & STATX_BTIME)
            st->st_mtim = statx_timestamp_منزل_timespec (stx.stx_btime);
          وإلا
            st->st_mtim.tv_sec = st->st_mtim.tv_nsec = -1;
        }
    }

  أرجع ret;
}

ساكن صحيح
do_stat (حرف ثابت *name, بنية stat *st)
{
  أرجع do_statx (AT_FDCWD, name, st, 0, calc_req_نقاب ());
}

ساكن صحيح
do_lstat (حرف ثابت *name, بنية stat *st)
{
  أرجع do_statx (AT_FDCWD, name, st, AT_SYMLINK_NOFOLLOW, calc_req_نقاب ());
}

ساكن صحيح
stat_for_گهڻائي (حرف ثابت *name, بنية stat *st)
{
  أرجع do_statx (AT_FDCWD, name, st, 0, STATX_گهڻائي);
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
  أرجع do_statx (fd, "", st, AT_خالي_جايون_رستو, STATX_INO);
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
stat_for_گهڻائي (حرف ثابت *name, بنية stat *st)
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
#جيڪڏهن_پورو

/* Return the address of the first plain %b spec in FMT, or NULL if
   there is no such spec.  %5b etc. do not match, so that user
   widths/flags are honored.  */

ATTRIBUTE_خالص
ساكن حرف ثابت *
پهريون_percent_b (حرف ثابت *fmt)
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
تعداد { ABFORMAT_سائيز = 128 };
ساكن حرف abformat[2][12][ABFORMAT_سائيز];
/* True if precomputed formats should be used.  This can be false if
   nl_langinfo fails, if a format or month abbreviation is unusually
   long, or if a month abbreviation contains '%'.  */
ساكن bool use_abformat;

/* Store into ABMON the abbreviated month names, suitably aligned.
   Return true if successful.  */

ساكن bool
abmon_init (حرف abmon[12][ABFORMAT_سائيز])
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_NL_LANGINFO
  أرجع false;
#وإلا
  صحيح max_mon_ويڪر = 0;
  صحيح mon_ويڪر[12];
  صحيح mon_len[12];

  لكل (صحيح i = 0; i < 12; i++)
    {
      حرف ثابت *abbr = nl_langinfo (ABMON_1 + i);
      mon_len[i] = strnlen (abbr, ABFORMAT_سائيز);
      إذا (mon_len[i] == ABFORMAT_سائيز)
        أرجع false;
      إذا (strchr (abbr, '%'))
        أرجع false;
      mon_ويڪر[i] = mbswidth (strcpy (abmon[i], abbr), MBSWIDTH_FLAGS);
      إذا (mon_ويڪر[i] < 0)
        أرجع false;
      max_mon_ويڪر = MAX (max_mon_ويڪر, mon_ويڪر[i]);
    }

  لكل (صحيح i = 0; i < 12; i++)
    {
      صحيح fill = max_mon_ويڪر - mon_ويڪر[i];
      إذا (ABFORMAT_سائيز - mon_len[i] <= fill)
        أرجع false;
      bool align_کاٻو = !c_isdigit (abmon[i][0]);
      صحيح fill_وٿي;
      إذا (align_کاٻو)
        fill_وٿي = mon_len[i];
      وإلا
        {
          memmove (abmon[i] + fill, abmon[i], mon_len[i]);
          fill_وٿي = 0;
        }
      memset (abmon[i] + fill_وٿي, ' ', fill);
      abmon[i][mon_len[i] + fill] = '\0';
    }

  أرجع true;
#جيڪڏهن_پورو
}

/* Initialize ABFORMAT and USE_ABFORMAT.  */

ساكن فراغ
abformat_init (فراغ)
{
  حرف ثابت *pb[2];
  لكل (صحيح recent = 0; recent < 2; recent++)
    pb[recent] = پهريون_percent_b (long_وقت_format[recent]);
  إذا (! (pb[0] || pb[1]))
    أرجع;

  حرف abmon[12][ABFORMAT_سائيز];
  إذا (! abmon_init (abmon))
    أرجع;

  لكل (صحيح recent = 0; recent < 2; recent++)
    {
      حرف ثابت *fmt = long_وقت_format[recent];
      لكل (صحيح i = 0; i < 12; i++)
        {
          حرف *nfmt = abformat[recent][i];
          صحيح nbytes;

          إذا (! pb[recent])
            nbytes = snprintf (nfmt, ABFORMAT_سائيز, "%s", fmt);
          وإلا
            {
              إذا (! (pb[recent] - fmt <= MIN (ABFORMAT_سائيز, وڌ_کان_وڌ_عدد)))
                أرجع;
              صحيح اڳياڙي_len = pb[recent] - fmt;
              nbytes = snprintf (nfmt, ABFORMAT_سائيز, "%.*s%s%s",
                                 اڳياڙي_len, fmt, abmon[i], pb[recent] + 2);
            }

          إذا (! (0 <= nbytes && nbytes < ABFORMAT_سائيز))
            أرجع;
        }
    }

  use_abformat = true;
}

ساكن size_t
dev_ino_hash (فراغ ثابت *x, size_t جدول_سائيز)
{
  بنية dev_ino ثابت *p = x;
  أرجع (uintmax_t) {p->st_ino} % جدول_سائيز;
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
گهمو_dir (dev_t dev, ino_t ino)
{
  بنية dev_ino *ent;
  بنية dev_ino *ent_from_جدول;
  bool مليو_ميلاپ;

  ent = xmalloc (حجم *ent);
  ent->st_ino = ino;
  ent->st_dev = dev;

  /* Attempt to insert this entry into the table.  */
  ent_from_جدول = hash_وجهو (سرگرم_dir_سيٽ, ent);

  إذا (ent_from_جدول == عدم)
    {
      /* Insertion failed due to lack of memory.  */
      xalloc_die ();
    }

  مليو_ميلاپ = (ent_from_جدول != ent);

  إذا (مليو_ميلاپ)
    {
      /* ent was not inserted, so free it.  */
      حرر (ent);
    }

  أرجع مليو_ميلاپ;
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
  put_indicator (&color_indicator[C_کاٻو]);
  put_indicator (&color_indicator[C_ساڄو]);
}

ساكن فراغ
سيٽ_عام_color (فراغ)
{
  إذا (print_with_color && is_colored (C_NORM))
    {
      put_indicator (&color_indicator[C_کاٻو]);
      put_indicator (&color_indicator[C_NORM]);
      put_indicator (&color_indicator[C_ساڄو]);
    }
}

/* An ordinary signal was received; arrange for the program to exit.  */

ساكن فراغ
sighandler (صحيح sig)
{
  إذا (! مداخلت_signal)
    مداخلت_signal = sig;
}

/* A SIGTSTP was received; arrange for the program to suspend itself.  */

ساكن فراغ
stophandler (MAYBE_اڻ_استعمال_ٿيل صحيح sig)
{
  إذا (! مداخلت_signal)
    stop_signal_ڳڻپ++;
}

/* Process any pending signals.  If signals are caught, this function
   should be called periodically.  Ideally there should never be an
   unbounded amount of time when signals are not being processed.
   Signal handling can restore the default colors, so callers must
   immediately change colors after invoking this function.  */

ساكن فراغ
process_سگنل (فراغ)
{
  طالما (مداخلت_signal || stop_signal_ڳڻپ)
    {
      صحيح sig;
      صحيح stops;
      sigset_t oldset;

      إذا (استعمال_ٿيل_color)
        restore_default_color ();
      fflush (stdout);

      sigprocmask (SIG_BLOCK, &caught_سگنل, &oldset);

      /* Reload interrupt_signal and stop_signal_count, in case a new
         signal was handled before sigprocmask took effect.  */
      sig = مداخلت_signal;
      stops = stop_signal_ڳڻپ;

      /* SIGTSTP is special, since the application can receive that signal
         more than once.  In this case, don't set the signal handler to the
         default.  Instead, just raise the uncatchable SIGSTOP.  */
      إذا (stops)
        {
          stop_signal_ڳڻپ = stops - 1;
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
signal_تياري (bool init)
{
  /* The signals that are trapped, and the number of such signals.  */
  ساكن صحيح ثابت stop_sig[] =
    {
      /* This one is handled specially.  */
      SIGTSTP,
    };

  تعداد { nsigs = countof (رڪن_sig), nstop = countof (stop_sig) };

  إذا (init)
    {
      بنية sigaction act;

      sigemptyset (&caught_سگنل);
      لكل (صحيح j = 0; j < nsigs + nstop; j++)
        {
          صحيح sig = j < nsigs ? رڪن_sig[j]: stop_sig[j - nsigs];
          sigaction (sig, عدم, &act);
          إذا (act.sa_handler != SIG_IGN)
            sigaddset (&caught_سگنل, sig);
        }

      act.sa_mask = caught_سگنل;
      act.sa_flags = SA_RESTART;

      لكل (صحيح j = 0; j < nsigs + nstop; j++)
        {
          صحيح sig = j < nsigs ? رڪن_sig[j]: stop_sig[j - nsigs];
          إذا (sigismember (&caught_سگنل, sig))
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
          صحيح sig = j < nsigs ? رڪن_sig[j]: stop_sig[j - nsigs];
          إذا (sigismember (&caught_سگنل, sig))
            إشارة (sig, SIG_DFL);
        }
    }
}

ساكن فراغ
signal_init (فراغ)
{
  signal_تياري (true);
}

ساكن فراغ
signal_restore (فراغ)
{
  signal_تياري (false);
}

صحيح
مکيه (صحيح argc, حرف **argv)
{
  صحيح i;
  بنية pending *thispend;
  صحيح n_files;

  initialize_main (&argc, &argv);
  سيٽ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (LS_FAILURE);
  atexit (close_stdout);

  static_assert (countof (color_indicator) == countof (indicator_name));

  exit_حالت = EXIT_SUCCESS;
  print_dir_name = true;
  pending_dirs = عدم;

  current_وقت.tv_sec = TYPE_MINIMUM (time_t);
  current_وقت.tv_nsec = -1;

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

  إذا (directories_پهريون)
    check_symlink_گهڻائي = true;
  وإلا إذا (print_with_color)
    {
      /* Avoid following symbolic links when possible.  */
      إذا (is_colored (C_ORPHAN)
          || (is_colored (C_EXEC) && color_symlink_as_referent)
          || (is_colored (C_MISSING) && format == long_format))
        check_symlink_گهڻائي = true;
    }

  إذا (dereference == DEREF_UNDEFINED)
    dereference = ((فوري_dirs
                    || indicator_style == classify
                    || format == long_format)
                   ? DEREF_NEVER
                   : DEREF_COMMAND_سٽ_SYMLINK_منزل_DIR);

  /* When using -R, initialize a data structure we'll use to
     detect any directory cycles.  */
  إذا (recursive)
    {
      سرگرم_dir_سيٽ = hash_initialize (INITIAL_جدول_سائيز, عدم,
                                        dev_ino_hash,
                                        dev_ino_compare,
                                        dev_ino_free);
      إذا (سرگرم_dir_سيٽ == عدم)
        xalloc_die ();

      obstack_init (&dev_ino_obstack);
    }

  localtz = tzalloc (getenv ("TZ"));

  format_needs_stat = ((ترتيب_ڏيو_type == ترتيب_ڏيو_وقت) | (ترتيب_ڏيو_type == ترتيب_ڏيو_سائيز)
                       | (format == long_format)
                       | print_بلاڪ_سائيز | print_hyperlink | print_scontext);
  format_needs_type = ((! format_needs_stat)
                       & (recursive | print_with_color | print_scontext
                          | directories_پهريون
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
  cwd_n_استعمال_ٿيل = 0;

  clear_files ();

  n_files = argc - i;

  إذا (n_files <= 0)
    {
      إذا (فوري_dirs)
        gobble_file (".", directory, NOT_AN_INODE_عدد, true, عدم);
      وإلا
        قطار_directory (".", عدم, true);
    }
  وإلا
    افعل
      gobble_file (argv[i++], unknown, NOT_AN_INODE_عدد, true, عدم);
    طالما (i < argc);

  إذا (cwd_n_استعمال_ٿيل)
    {
      ترتيب_ڏيو_files ();
      إذا (!فوري_dirs)
        ڪڍو_dirs_from_files (عدم, true);
      /* 'cwd_n_used' might be zero now.  */
    }

  /* In the following if/else blocks, it is sufficient to test 'pending_dirs'
     (and not pending_dirs->name) because there may be no markers in the queue
     at this point.  A marker may be enqueued when extract_dirs_from_files is
     called with a non-empty string or via print_dir.  */
  إذا (cwd_n_استعمال_ٿيل)
    {
      print_current_files ();
      إذا (pending_dirs)
        dired_outbyte ('\n');
    }
  وإلا إذا (n_files <= 1 && pending_dirs && !pending_dirs->ايندڙ)
    print_dir_name = false;

  طالما (pending_dirs)
    {
      thispend = pending_dirs;
      pending_dirs = pending_dirs->ايندڙ;

      إذا (LOOP_DETECT)
        {
          إذا (thispend->name == عدم)
            {
              /* thispend->name == NULL means this is a marker entry
                 indicating we've finished processing the directory.
                 Use its dev/ino numbers to remove the corresponding
                 entry from the active_dir_set hash table.  */
              بنية dev_ino di = dev_ino_ڪڍو ();
              بنية dev_ino *مليو = hash_remove (سرگرم_dir_سيٽ, &di);
              إذا (false)
                assert_matching_dev_ino (thispend->realname, di);
              affirm (مليو);
              dev_ino_free (مليو);
              free_pending_ent (thispend);
              استمر;
            }
        }

      print_dir (thispend->name, thispend->realname,
                 thispend->command_سٽ_arg);

      free_pending_ent (thispend);
      print_dir_name = true;
    }

  إذا (print_with_color && استعمال_ٿيل_color)
    {

      /* Skip the restore when it would be a no-op, i.e.,
         when left is "\033[" and right is "m".  */
      إذا (!(color_indicator[C_کاٻو].len == 2
            && memeq (color_indicator[C_کاٻو].string, "\033[", 2)
            && color_indicator[C_ساڄو].len == 1
            && color_indicator[C_ساڄو].string[0] == 'm'))
        restore_default_color ();

      fflush (stdout);

      signal_restore ();

      /* Act on any signals that arrived before the default was restored.
         This can process signals out of order, but there doesn't seem to
         be an easy way to do them in order, and the order isn't that
         important anyway.  */
      لكل (صحيح j = stop_signal_ڳڻپ; j; j--)
        ارفع (SIGSTOP);
      صحيح j = مداخلت_signal;
      إذا (j)
        ارفع (j);
    }

  إذا (dired)
    {
      /* No need to free these since we're about to exit.  */
      dired_dump_obstack ("//DIRED//", &dired_obstack);
      dired_dump_obstack ("//SUBDIRED//", &subdired_obstack);
      لکو ("//DIRED-OPTIONS// --quoting-style=%s\n",
              quoting_style_دليلون[get_quoting_style (filename_quoting_options)]);
    }

  إذا (LOOP_DETECT)
    {
      assure (hash_get_n_entries (سرگرم_dir_سيٽ) == 0);
      hash_free (سرگرم_dir_سيٽ);
    }

  أرجع exit_حالت;
}

/* Return the line length indicated by the value given by SPEC, or -1
   if unsuccessful.  0 means no limit on line length.  */

ساكن ptrdiff_t
decode_سٽ_ڊيگهائي (حرف ثابت *spec)
{
  uintmax_t val;

  /* Treat too-large values as if they were 0, which is
     effectively infinity.  */
  بدل (xstrtoumax (spec, عدم, 0, &val, ""))
    {
    حالة LONGINT_OK:
      أرجع val <= MIN (PTRDIFF_MAX, SIZE_MAX) ? val : 0;

    حالة LONGINT_حد_کان_مٿي:
      أرجع 0;

    حالة LONGINT_INVALID:
    حالة LONGINT_INVALID_SUFFIX_CHAR:
    حالة LONGINT_INVALID_SUFFIX_CHAR_WITH_حد_کان_مٿي:
      أرجع -1;

    افتراضي:
      unreachable ();
    }
}

/* Return true if standard output is a tty, caching the result.  */

ساكن bool
stdout_isatty (فراغ)
{
  ساكن نشانوارو حرف ٻاهريون_tty = -1;
  إذا (ٻاهريون_tty < 0)
    ٻاهريون_tty = isatty (STDOUT_FILENO);
  assume (ٻاهريون_tty == 0 || ٻاهريون_tty == 1);
  أرجع ٻاهريون_tty;
}

/* Set all the option flags according to the switches specified.
   Return the index of the first non-option argument.  */

ساكن صحيح
decode_switches (صحيح argc, حرف **argv)
{
  حرف *وقت_style_option = عدم;

  /* These variables are false or -1 unless a switch says otherwise.  */
  bool kibibytes_specified = false;
  صحيح format_opt = -1;
  صحيح hide_control_chars_opt = -1;
  صحيح quoting_style_opt = -1;
  صحيح ترتيب_ڏيو_opt = -1;
  ptrdiff_t tabsize_opt = -1;
  ptrdiff_t ويڪر_opt = -1;

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
          ignore_گهڻائي = IGNORE_گهٽتر;
          اكسر;

        حالة 'b':
          quoting_style_opt = escape_quoting_style;
          اكسر;

        حالة 'c':
          وقت_type = وقت_ctime;
          explicit_وقت = true;
          اكسر;

        حالة 'd':
          فوري_dirs = true;
          اكسر;

        حالة 'f':
          ignore_گهڻائي = IGNORE_گهٽتر; /* enable -a */
          ترتيب_ڏيو_opt = ترتيب_ڏيو_none;         /* enable -U */
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
            human_autoscale | human_SI | human_بنياد_1024;
          file_output_بلاڪ_سائيز = output_بلاڪ_سائيز = 1;
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
          print_گروهه = false;
          اكسر;

        حالة 'p':
          indicator_style = slash;
          اكسر;

        حالة 'q':
          hide_control_chars_opt = true;
          اكسر;

        حالة 'r':
          ترتيب_ڏيو_ابتو = true;
          اكسر;

        حالة 's':
          print_بلاڪ_سائيز = true;
          اكسر;

        حالة 't':
          ترتيب_ڏيو_opt = ترتيب_ڏيو_وقت;
          اكسر;

        حالة 'u':
          وقت_type = وقت_atime;
          explicit_وقت = true;
          اكسر;

        حالة 'v':
          ترتيب_ڏيو_opt = ترتيب_ڏيو_نسخو;
          اكسر;

        حالة 'w':
          ويڪر_opt = decode_سٽ_ڊيگهائي (optarg);
          إذا (ويڪر_opt < 0)
            error (LS_FAILURE, 0, "%s: %s", _("invalid line width"),
                   quote (optarg));
          اكسر;

        حالة 'x':
          format_opt = horizontal;
          اكسر;

        حالة 'A':
          ignore_گهڻائي = IGNORE_DOT_AND_DOTDOT;
          اكسر;

        حالة 'B':
          جوڙيو_ignore_نمونو ("*~");
          جوڙيو_ignore_نمونو (".*~");
          اكسر;

        حالة 'C':
          format_opt = many_في_سٽ;
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
              i = XARGMATCH ("--classify", optarg, when_دليلون, when_types);
            وإلا
              /* Using --classify with no argument is equivalent to using
                 --classify=always.  */
              i = when_always;

            إذا (i == when_always || (i == when_if_tty && stdout_isatty ()))
              indicator_style = classify;
            اكسر;
          }

        حالة 'G':		/* inhibit display of group info */
          print_گروهه = false;
          اكسر;

        حالة 'H':
          dereference = DEREF_COMMAND_سٽ_ARGUMENTS;
          اكسر;

        حالة DEREFERENCE_COMMAND_سٽ_SYMLINK_منزل_DIR_OPTION:
          dereference = DEREF_COMMAND_سٽ_SYMLINK_منزل_DIR;
          اكسر;

        حالة 'I':
          جوڙيو_ignore_نمونو (optarg);
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
          ترتيب_ڏيو_opt = ترتيب_ڏيو_سائيز;
          اكسر;

        حالة 'T':
          tabsize_opt = xnumtoumax (optarg, 0, 0, MIN (PTRDIFF_MAX, SIZE_MAX),
                                    "", _("invalid tab size"), LS_FAILURE, 0);
          اكسر;

        حالة 'U':
          ترتيب_ڏيو_opt = ترتيب_ڏيو_none;
          اكسر;

        حالة 'X':
          ترتيب_ڏيو_opt = ترتيب_ڏيو_extension;
          اكسر;

        حالة '1':
          /* -1 has no effect after -l.  */
          إذا (format_opt != long_format)
            format_opt = one_في_سٽ;
          اكسر;

        حالة AUTHOR_OPTION:
          print_author = true;
          اكسر;

        حالة HIDE_OPTION:
          {
            بنية ignore_نمونو *hide = xmalloc (حجم *hide);
            hide->نمونو_2 = optarg;
            hide->ايندڙ = hide_patterns;
            hide_patterns = hide;
          }
          اكسر;

        حالة ترتيب_ڏيو_OPTION:
          ترتيب_ڏيو_opt = XARGMATCH ("--sort", optarg, ترتيب_ڏيو_دليلون, ترتيب_ڏيو_types);
          اكسر;

        حالة گروهه_DIRECTORIES_پهريون_OPTION:
          directories_پهريون = true;
          اكسر;

        حالة وقت_OPTION:
          وقت_type = XARGMATCH ("--time", optarg, وقت_دليلون, وقت_types);
          explicit_وقت = true;
          اكسر;

        حالة FORMAT_OPTION:
          format_opt = XARGMATCH ("--format", optarg, format_دليلون,
                                  format_types);
          اكسر;

        حالة ڀريل_وقت_OPTION:
          format_opt = long_format;
          وقت_style_option = (حرف *) "full-iso";
          اكسر;

        حالة COLOR_OPTION:
          {
            صحيح i;
            إذا (optarg)
              i = XARGMATCH ("--color", optarg, when_دليلون, when_types);
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
              i = XARGMATCH ("--hyperlink", optarg, when_دليلون, when_types);
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
                                       indicator_style_دليلون,
                                       indicator_style_types);
          اكسر;

        حالة QUOTING_STYLE_OPTION:
          quoting_style_opt = XARGMATCH ("--quoting-style", optarg,
                                         quoting_style_دليلون,
                                         quoting_style_vals);
          اكسر;

        حالة وقت_STYLE_OPTION:
          وقت_style_option = optarg;
          اكسر;

        حالة SHOW_CONTROL_CHARS_OPTION:
          hide_control_chars_opt = false;
          اكسر;

        حالة بلاڪ_سائيز_OPTION:
          {
            تعداد strtol_error e = human_options (optarg, &human_output_opts,
                                                 &output_بلاڪ_سائيز);
            إذا (e != LONGINT_OK)
              xstrtol_fatal (e, oi, 0, long_options, optarg);
            file_human_output_opts = human_output_opts;
            file_output_بلاڪ_سائيز = output_بلاڪ_سائيز;
          }
          اكسر;

        حالة SI_OPTION:
          file_human_output_opts = human_output_opts =
            human_autoscale | human_SI;
          file_output_بلاڪ_سائيز = output_بلاڪ_سائيز = 1;
          اكسر;

        حالة 'Z':
          print_scontext = true;
          اكسر;

        حالة ZERO_OPTION:
          eolbyte = 0;
          hide_control_chars_opt = false;
          إذا (format_opt != long_format)
            format_opt = one_في_سٽ;
          print_with_color = false;
          quoting_style_opt = literal_quoting_style;
          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخو_CHAR (پروگرام_NAME, AUTHORS);

        افتراضي:
          usage (LS_FAILURE);
        }
    }

  إذا (! output_بلاڪ_سائيز)
    {
      حرف ثابت *ls_بلاڪ_سائيز = getenv ("LS_BLOCK_SIZE");
      human_options (ls_بلاڪ_سائيز,
                     &human_output_opts, &output_بلاڪ_سائيز);
      إذا (ls_بلاڪ_سائيز || getenv ("BLOCK_SIZE"))
        {
          file_human_output_opts = human_output_opts;
          file_output_بلاڪ_سائيز = output_بلاڪ_سائيز;
        }
      إذا (kibibytes_specified)
        {
          human_output_opts = 0;
          output_بلاڪ_سائيز = 1024;
        }
    }

  format = (0 <= format_opt ? format_opt
            : ls_گهڻائي == LS_LS ? (stdout_isatty ()
                                  ? many_في_سٽ : one_في_سٽ)
            : ls_گهڻائي == LS_MULTI_کانو ? many_في_سٽ
            : /* ls_mode == LS_LONG_FORMAT */ long_format);

  /* If the line length was not set by a switch but is needed to determine
     output, go to the work of obtaining it from the environment.  */
  ptrdiff_t linelen = ويڪر_opt;
  إذا (format == many_في_سٽ || format == horizontal || format == with_commas
      || print_with_color)
    {
#جيڪڏهن_وصف_ٿيل TIOCGWINSZ
      إذا (linelen < 0)
        {
          بنية winsize ws;
          إذا (stdout_isatty ()
              && 0 <= ioctl (STDOUT_FILENO, TIOCGWINSZ, &ws)
              && 0 < ws.ws_کانو)
            linelen = ws.ws_کانو <= MIN (PTRDIFF_MAX, SIZE_MAX) ? ws.ws_کانو : 0;
        }
#جيڪڏهن_پورو
      إذا (linelen < 0)
        {
          حرف ثابت *p = getenv ("COLUMNS");
          إذا (p && *p)
            {
              linelen = decode_سٽ_ڊيگهائي (p);
              إذا (linelen < 0)
                error (0, 0,
                       _("ignoring invalid width"
                         " in environment variable COLUMNS: %s"),
                       quote (p));
            }
        }
    }

  سٽ_ڊيگهائي = linelen < 0 ? 80 : linelen;

  /* Determine the max possible number of display columns.  */
  max_idx = سٽ_ڊيگهائي / MIN_ڪالم_ويڪر;
  /* Account for first display column not having a separator,
     or line_lengths shorter than MIN_COLUMN_WIDTH.  */
  max_idx += سٽ_ڊيگهائي % MIN_ڪالم_ويڪر != 0;

  إذا (format == many_في_سٽ || format == horizontal || format == with_commas)
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
                       ? ls_گهڻائي == LS_LS && stdout_isatty ()
                       : hide_control_chars_opt);

  qmark_newline = hide_control_chars_opt != false && eolbyte == '\n'
                  && ((سٽ_ڊيگهائي
                       && (format == with_commas
                           || format == many_في_سٽ || format == horizontal))
                      || format == one_في_سٽ || format == long_format);

  صحيح qs = quoting_style_opt;
  إذا (qs < 0)
    qs = getenv_quoting_style ();
  إذا (qs < 0)
    qs = (ls_گهڻائي == LS_LS
          ? (stdout_isatty () ? shell_escape_quoting_style : -1)
          : escape_quoting_style);
  إذا (0 <= qs)
    سيٽ_quoting_style (عدم, qs);
  qs = get_quoting_style (عدم);
  align_variable_outer_quotes
    = ((format == long_format
        || ((format == many_في_سٽ || format == horizontal) && سٽ_ڊيگهائي))
       && (qs == shell_quoting_style
           || qs == shell_escape_quoting_style
           || qs == c_maybe_quoting_style));
  filename_quoting_options = clone_quoting_options (عدم);
  إذا (qs == escape_quoting_style)
    سيٽ_char_quoting (filename_quoting_options, ' ', 1);
  إذا (file_type <= indicator_style)
    {
      لكل (حرف ثابت *p = &"*=>@|"[indicator_style - file_type]; *p; p++)
        سيٽ_char_quoting (filename_quoting_options, *p, 1);
    }
  إذا (format == with_commas
      && (qs == shell_quoting_style
          || qs == shell_escape_quoting_style
          || qs == c_maybe_quoting_style
          || qs == escape_quoting_style))
    سيٽ_char_quoting (filename_quoting_options, ',', 1);

  dirname_quoting_options = clone_quoting_options (عدم);
  سيٽ_char_quoting (dirname_quoting_options, ':', 1);

  /* --dired implies --format=long (-l) and sans --hyperlink.
     So ignore it if those overridden.  */
  dired &= (format == long_format) & !print_hyperlink;

  إذا (eolbyte < dired)
    error (LS_FAILURE, 0, _("--dired and --zero are incompatible"));

  /* If a time type is explicitly specified (with -c, -u, or --time=)
     and we're not showing a time (-l not specified), then sort by that time,
     rather than by name.  Note this behavior is unspecified by POSIX.  */

  ترتيب_ڏيو_type = (0 <= ترتيب_ڏيو_opt ? ترتيب_ڏيو_opt
               : (format != long_format && explicit_وقت)
               ? ترتيب_ڏيو_وقت : ترتيب_ڏيو_name);

  إذا (format == long_format)
    {
      حرف *envstyle = عدم;
      حرف *style = وقت_style_option;
      إذا (! style)
        style = envstyle = getenv ("TIME_STYLE");

      إذا (! style)
        style = (حرف *) "locale";
      وإلا
        {
          ساكن حرف ثابت posix_اڳياڙي[] = "posix-";
          طالما (STREQ_LEN (style, posix_اڳياڙي, حجم posix_اڳياڙي - 1))
            {
              إذا (! hard_locale (LC_وقت))
                أرجع optind;
              style += حجم posix_اڳياڙي - 1;
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
          long_وقت_format[0] = p0;
          long_وقت_format[1] = p1;
        }
      وإلا
        {
          بدل (x_timestyle_ميلاپ (style, /*allow_posix=*/ true,
                                     وقت_style_دليلون,
                                     (حرف ثابت *) وقت_style_types,
                                     حجم (*وقت_style_types), LS_FAILURE))
            {
            حالة ڀريل_iso_وقت_style:
              long_وقت_format[0] = long_وقت_format[1] =
                "%Y-%m-%d %H:%M:%S.%N %z";
              اكسر;

            حالة long_iso_وقت_style:
              long_وقت_format[0] = long_وقت_format[1] = "%Y-%m-%d %H:%M";
              اكسر;

            حالة iso_وقت_style:
              long_وقت_format[0] = "%Y-%m-%d ";
              long_وقت_format[1] = "%m-%d %H:%M";
              اكسر;

            حالة locale_وقت_style:
              إذا (hard_locale (LC_وقت))
                {
                  لكل (صحيح i = 0; i < 2; i++)
                    long_وقت_format[i] =
                      dcgettext (عدم, long_وقت_format[i], LC_وقت);
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
get_funky_string (حرف **dest, حرف ثابت **src, bool equals_پڇاڙي,
                  size_t *output_ڳڻپ)
{
  حرف انگ;			/* For numerical codes */
  size_t ڳڻپ;			/* Something to count with */
  تعداد {
    ST_GND, ST_BACKSLASH, ST_OCTAL, ST_HEX, ST_CARET, ST_پڇاڙي, ST_ERROR
  } state;
  حرف ثابت *p;
  حرف *q;

  p = *src;			/* We don't want to double-indirect */
  q = *dest;			/* the whole darn time.  */

  ڳڻپ = 0;			/* No characters counted in yet.  */
  انگ = 0;

  state = ST_GND;		/* Start in ground state.  */
  طالما (state < ST_پڇاڙي)
    {
      بدل (state)
        {
        حالة ST_GND:		/* Ground state (no escapes) */
          بدل (*p)
            {
            حالة ':':
            حالة '\0':
              state = ST_پڇاڙي;	/* End of string */
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
              إذا (equals_پڇاڙي)
                {
                  state = ST_پڇاڙي; /*پڇاڙي*/
                  اكسر;
                }
              FALLTHROUGH;
            افتراضي:
              *(q++) = *(p++);
              ++ڳڻپ;
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
              انگ = *p - '0';
              اكسر;
            حالة 'x':
            حالة 'X':
              state = ST_HEX;	/* Hex sequence */
              انگ = 0;
              اكسر;
            حالة 'a':		/* Bell */
              انگ = '\a';
              اكسر;
            حالة 'b':		/* Backspace */
              انگ = '\b';
              اكسر;
            حالة 'e':		/* Escape */
              انگ = 27;
              اكسر;
            حالة 'f':		/* Form feed */
              انگ = '\f';
              اكسر;
            حالة 'n':		/* Newline */
              انگ = '\n';
              اكسر;
            حالة 'r':		/* Carriage return */
              انگ = '\r';
              اكسر;
            حالة 't':		/* Tab */
              انگ = '\t';
              اكسر;
            حالة 'v':		/* Vtab */
              انگ = '\v';
              اكسر;
            حالة '?':		/* Delete */
              انگ = 127;
              اكسر;
            حالة '_':		/* Space */
              انگ = ' ';
              اكسر;
            حالة '\0':		/* End of string */
              state = ST_ERROR;	/* Error! */
              اكسر;
            افتراضي:		/* Escaped character like \ ^ : = */
              انگ = *p;
              اكسر;
            }
          إذا (state == ST_BACKSLASH)
            {
              *(q++) = انگ;
              ++ڳڻپ;
              state = ST_GND;
            }
          ++p;
          اكسر;

        حالة ST_OCTAL:		/* Octal sequence */
          إذا (*p < '0' || *p > '7')
            {
              *(q++) = انگ;
              ++ڳڻپ;
              state = ST_GND;
            }
          وإلا
            انگ = (انگ << 3) + (*(p++) - '0');
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
              انگ = (انگ << 4) + (*(p++) - '0');
              اكسر;
            حالة 'a':
            حالة 'b':
            حالة 'c':
            حالة 'd':
            حالة 'e':
            حالة 'f':
              انگ = (انگ << 4) + (*(p++) - 'a') + 10;
              اكسر;
            حالة 'A':
            حالة 'B':
            حالة 'C':
            حالة 'D':
            حالة 'E':
            حالة 'F':
              انگ = (انگ << 4) + (*(p++) - 'A') + 10;
              اكسر;
            افتراضي:
              *(q++) = انگ;
              ++ڳڻپ;
              state = ST_GND;
              اكسر;
            }
          اكسر;

        حالة ST_CARET:		/* Caret escape */
          state = ST_GND;	/* Should be the next state... */
          إذا (*p >= '@' && *p <= '~')
            {
              *(q++) = *(p++) & 037;
              ++ڳڻپ;
            }
          وإلا إذا (*p == '?')
            {
              *(q++) = 127;
              ++ڳڻپ;
            }
          وإلا
            state = ST_ERROR;
          اكسر;

        حالة ST_پڇاڙي: حالة ST_ERROR: افتراضي:
          affirm (false);
        }
    }

  *dest = q;
  *src = p;
  *output_ڳڻپ = ڳڻپ;

  أرجع state != ST_ERROR;
}

تعداد parse_state
  {
    PS_شروعات = 1,
    PS_2,
    PS_3,
    PS_4,
    PS_پورو,
    PS_FAIL
  };


/* Check if the content of TERM is a valid name in dircolors.  */

ساكن bool
معلوم_رڪن_type (فراغ)
{
  حرف ثابت *رڪن = getenv ("TERM");
  إذا (! رڪن || ! *رڪن)
    أرجع false;

  حرف ثابت *سٽ = G_سٽ;
  طالما (سٽ - G_سٽ < حجم (G_سٽ))
    {
      إذا (STRNCMP_LIT (سٽ, "TERM ") == 0)
        {
          إذا (fnmatch (سٽ + 5, رڪن, 0) == 0)
            أرجع true;
        }
      سٽ += لڙي_ڊيگهه (سٽ) + 1;
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
      إذا (! (colorterm && *colorterm) && ! معلوم_رڪن_type ())
        print_with_color = false;
      أرجع;
    }

  ext = عدم;

  /* This is an overly conservative estimate, but any possible
     LS_COLORS string will *not* generate a color_buf longer than
     itself, so it is a safe way of allocating a buffer in
     advance.  */
  buf = color_buf = xstrdup (p);

  تعداد parse_state state = PS_شروعات;
  طالما (true)
    {
      بدل (state)
        {
        حالة PS_شروعات:		/* First label character */
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
              ext->ايندڙ = color_ext_list;
              color_ext_list = ext;
              ext->exact_ميلاپ = false;

              ++p;
              ext->ext.string = buf;

              state = (get_funky_string (&buf, &p, true, &ext->ext.len)
                       ? PS_4 : PS_FAIL);
              اكسر;

            حالة '\0':
              state = PS_پورو;	/* Done! */
              اذهب پورو;

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
                               ? PS_شروعات : PS_FAIL);
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
                       ? PS_شروعات : PS_FAIL);
            }
          وإلا
            state = PS_FAIL;
          اكسر;

        حالة PS_FAIL:
          اذهب پورو;

        حالة PS_پورو: افتراضي:
          affirm (false);
        }
    }
 پورو:

  إذا (state == PS_FAIL)
    {
      error (0, 0,
             _("unparsable value for LS_COLORS environment variable"));
      حرر (color_buf);
      لكل (بنية color_ext_type *e = color_ext_list; e != عدم;
           /*خالي_جايون*/)
        {
          بنية color_ext_type *e2 = e;
          e = e->ايندڙ;
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
           e1 = e1->ايندڙ)
        {
          bool case_ignored = false;

          لكل (بنية color_ext_type *e2 = e1->ايندڙ; e2 != عدم;
               e2 = e2->ايندڙ)
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
                          e1->exact_ميلاپ = true;
                          e2->exact_ميلاپ = true;
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
سيٽ_exit_حالت (bool serious)
{
  إذا (serious)
    exit_حالت = LS_FAILURE;
  وإلا إذا (exit_حالت == EXIT_SUCCESS)
    exit_حالت = LS_MINOR_PROBLEM;
}

/* Assuming a failure is serious if SERIOUS, use the printf-style
   MESSAGE to report the failure to access a file named FILE.  Assume
   errno is set appropriately for the failure.  */

ساكن فراغ
file_failure (bool serious, حرف ثابت *پيغام_2, حرف ثابت *file)
{
  error (0, errno, پيغام_2, quoteaf (file));
  سيٽ_exit_حالت (serious);
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
قطار_directory (حرف ثابت *name, حرف ثابت *realname, bool command_سٽ_arg)
{
  بنية pending *new = xmalloc (حجم *new);
  new->realname = realname ? xstrdup (realname) : عدم;
  new->name = name ? xstrdup (name) : عدم;
  new->command_سٽ_arg = command_سٽ_arg;
  new->ايندڙ = pending_dirs;
  pending_dirs = new;
}

/* Read directory NAME, and list the files in it.
   If REALNAME is nonzero, print its name instead of NAME;
   this is used for symbolic links to directories.
   COMMAND_LINE_ARG means this directory was mentioned on the command line.  */

ساكن فراغ
print_dir (حرف ثابت *name, حرف ثابت *realname, bool command_سٽ_arg)
{
  DIR *dirp;
  بنية dirent *ايندڙ;
  uintmax_t ڪل_بلاڪون = 0;
  ساكن bool پهريون = true;

  errno = 0;
  dirp = opendir (name);
  إذا (!dirp)
    {
      إذا (command_سٽ_arg || ! ignorable_traversal_errno (errno))
        file_failure (command_سٽ_arg, _("cannot open directory %s"), name);
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
            file_failure (command_سٽ_arg,
                          _("cannot determine device and inode of %s"), name);
          closedir (dirp);
          أرجع;
        }

      /* If we've already visited this dev/inode pair, warn that
         we've found a loop, and do not process this directory.  */
      إذا (گهمو_dir (dir_stat.st_dev, dir_stat.st_ino))
        {
          error (0, 0, _("%s: not listing already-listed directory"),
                 quotef (name));
          closedir (dirp);
          سيٽ_exit_حالت (true);
          أرجع;
        }

      dev_ino_ڌڪو (dir_stat.st_dev, dir_stat.st_ino);
    }

  clear_files ();

  إذا (recursive || print_dir_name)
    {
      إذا (!پهريون)
        dired_outbyte ('\n');
      پهريون = false;
      dired_indent ();

      حرف *absolute_name = عدم;
      إذا (print_hyperlink)
        {
          absolute_name = canonicalize_filename_گهڻائي (name, CAN_MISSING);
          إذا (! absolute_name
              && ! ignorable_traversal_errno (errno))
            file_failure (command_سٽ_arg,
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
      ايندڙ = readdir (dirp);
      إذا (ايندڙ)
        {
          إذا (! file_ignored (ايندڙ->d_name))
            {
              تعداد filetype type;
#إذا HAVE_STRUCT_DIRENT_D_TYPE
              type = d_type_filetype[ايندڙ->d_type];
#وإلا
              type = unknown;
#جيڪڏهن_پورو
              ڪل_بلاڪون += gobble_file (ايندڙ->d_name, type,
                                           RELIABLE_D_INO (ايندڙ),
                                           false, name);

              /* In this narrow case, print out each name right away, so
                 ls uses constant memory while processing the entries of
                 this directory.  Useful when there are many (millions)
                 of entries in a directory.  */
              إذا (format == one_في_سٽ && ترتيب_ڏيو_type == ترتيب_ڏيو_none
                      && !print_بلاڪ_سائيز && !recursive)
                {
                  /* We must call sort_files in spite of
                     "sort_type == sort_none" for its initialization
                     of the sorted_file vector.  */
                  ترتيب_ڏيو_files ();
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
          file_failure (command_سٽ_arg, _("reading directory %s"), name);
          إذا (err != EOVERFLOW)
            اكسر;
        }

      /* When processing a very large directory, and since we've inhibited
         interrupts, this loop would take so long that ls would be annoyingly
         uninterruptible.  This ensures that it handles signals promptly.  */
      process_سگنل ();
    }

  إذا (closedir (dirp) != 0)
    {
      file_failure (command_سٽ_arg, _("closing directory %s"), name);
      /* Don't return; print whatever we got.  */
    }

  /* Sort the directory contents.  */
  ترتيب_ڏيو_files ();

  /* If any member files are subdirectories, perhaps they should have their
     contents listed rather than being mentioned here as files.  */

  إذا (recursive)
    ڪڍو_dirs_from_files (name, false);

  إذا (format == long_format || print_بلاڪ_سائيز)
    {
      حرف buf[LONGEST_HUMAN_READABLE + 3];
      حرف *p = human_readable (ڪل_بلاڪون, buf + 1, human_output_opts,
                                ST_NBLOCKSIZE, output_بلاڪ_سائيز);
      حرف *pend = p + لڙي_ڊيگهه (p);
      *--p = ' ';
      *pend++ = eolbyte;
      dired_indent ();
      dired_outstring (_("total"));
      dired_outbuf (p, pend - p);
    }

  إذا (cwd_n_استعمال_ٿيل)
    print_current_files ();
}

/* Add 'pattern' to the list of patterns for which files that match are
   not listed.  */

ساكن فراغ
جوڙيو_ignore_نمونو (حرف ثابت *نمونو_2)
{
  بنية ignore_نمونو *ignore;

  ignore = xmalloc (حجم *ignore);
  ignore->نمونو_2 = نمونو_2;
  /* Add it to the head of the linked list.  */
  ignore->ايندڙ = ignore_patterns;
  ignore_patterns = ignore;
}

/* Return true if one of the PATTERNS matches FILE.  */

ساكن bool
patterns_ميلاپ (بنية ignore_نمونو ثابت *patterns, حرف ثابت *file)
{
  لكل (بنية ignore_نمونو ثابت *p = patterns; p; p = p->ايندڙ)
    إذا (fnmatch (p->نمونو_2, file, FNM_PERIOD) == 0)
      أرجع true;
  أرجع false;
}

/* Return true if FILE should be ignored.  */

ساكن bool
file_ignored (حرف ثابت *name)
{
  أرجع ((ignore_گهڻائي != IGNORE_گهٽتر
           && name[0] == '.'
           && (ignore_گهڻائي == IGNORE_DEFAULT || ! name[1 + (name[1] == '.')]))
          || (ignore_گهڻائي == IGNORE_DEFAULT
              && patterns_ميلاپ (hide_patterns, name))
          || patterns_ميلاپ (ignore_patterns, name));
}

/* POSIX requires that a file size be printed without a sign, even
   when negative.  Assume the typical case where negative sizes are
   actually positive values that have wrapped around.  */

ساكن uintmax_t
unsigned_file_سائيز (off_t سائيز)
{
  أرجع سائيز + (سائيز < 0) * ((uintmax_t) OFF_T_MAX - OFF_T_MIN + 1);
}

#جيڪڏهن_وصف_ٿيل HAVE_CAP
/* Return true if NAME has a capability (see linux/capability.h) */
ساكن bool
has_capability (حرف ثابت *name)
{
  حرف *نتيجو;
  bool has_cap;

  cap_t cap_d = cap_get_file (name);
  إذا (cap_d == عدم)
    أرجع false;

  نتيجو = cap_منزل_متن (cap_d, عدم);
  cap_free (cap_d);
  إذا (!نتيجو)
    أرجع false;

  /* check if human-readable capability string is empty */
  has_cap = !!*نتيجو;

  cap_free (نتيجو);
  أرجع has_cap;
}
#وإلا
ساكن bool
has_capability (MAYBE_اڻ_استعمال_ٿيل حرف ثابت *name)
{
  errno = ENOTSUP;
  أرجع false;
}
#جيڪڏهن_پورو

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
  لكل (idx_t i = 0; i < cwd_n_استعمال_ٿيل; i++)
    {
      بنية fileinfo *f = sorted_file[i];
      free_ent (f);
    }

  cwd_n_استعمال_ٿيل = 0;
  cwd_some_quoted = false;
  any_has_acl = false;
  inode_عدد_ويڪر = 0;
  بلاڪ_سائيز_ويڪر = 0;
  nlink_ويڪر = 0;
  owner_ويڪر = 0;
  گروهه_ويڪر = 0;
  author_ويڪر = 0;
  scontext_ويڪر = 0;
  major_device_عدد_ويڪر = 0;
  minor_device_عدد_ويڪر = 0;
  file_سائيز_ويڪر = 0;
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
      ai->سائيز = 0;
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
          || (ai->سائيز < 0 && !acl_errno_valid (ai->u.err)))
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
get_extended_info (حرف ثابت *ڀريل_name, بنية fileinfo *f, bool do_deref)
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
#جيڪڏهن_پورو
  bool check_capability = format_needs_capability & (type == عام_2);

  إذا (type != arg_directory && (check_acl | get_scontext | check_capability))
    {
      بنية aclinfo ai;
      /* Without ACL support, request contexts to obtain the attribute
         names needed for capability coloring too.  */
      صحيح aclinfo_flags = ((do_deref ? ACL_SYMLINK_FOLLOW : 0)
                           | (get_scontext || (!USE_ACL && check_capability)
                              ? ACL_GET_SCONTEXT : 0)
                           | filetype_d_type[type]);
      صحيح n = file_has_aclinfo_cache (ڀريل_name, f, &ai, aclinfo_flags);
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
        error (0, errno, "%s", quotef (ڀريل_name));
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
            error (0, ai.scontext_err, "%s", quotef (ڀريل_name));
        }

      /* has_capability adds around 30% runtime to 'ls --color',
         so call it only if really needed.  Note capability coloring
         is disabled in the default color config.  */
      إذا (check_capability && aclinfo_has_xattr (&ai, XATTR_NAME_CAPS))
        f->has_capability = has_capability_cache (ڀريل_name, f);

      f->scontext = ai.scontext;
      ai.scontext = عدم;
      aclinfo_free (&ai);
    }
}

ساكن bool
needs_quoting (حرف ثابت *name)
{
  حرف test[2];
  size_t len = quotearg_بفر (test, حجم test , name, -1,
                                filename_quoting_options);
  أرجع *name != *test || لڙي_ڊيگهه (name) != len;
}

/* Add a file to the current table of files.
   Verify that the file exists, and print an error message if it does not.
   Return the number of blocks that the file occupies.  */
ساكن uintmax_t
gobble_file (حرف ثابت *name, تعداد filetype type, ino_t inode,
             bool command_سٽ_arg, حرف ثابت *dirname)
{
  uintmax_t بلاڪون = 0;
  بنية fileinfo *f;

  /* An inode value prior to gobble_file necessarily came from readdir,
     which is not used for command line arguments.  */
  affirm (! command_سٽ_arg || inode == NOT_AN_INODE_عدد);

  إذا (cwd_n_استعمال_ٿيل == cwd_n_alloc)
    cwd_file = xpalloc (cwd_file, &cwd_n_alloc, 1, -1, حجم *cwd_file);

  f = &cwd_file[cwd_n_استعمال_ٿيل];
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
     (command_سٽ_arg
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
              || color_symlink_as_referent || check_symlink_گهڻائي))
      /* Command line dereferences are already taken care of by the above
         assertion that the inode number is not yet known.  */
      || (print_inode && inode == NOT_AN_INODE_عدد)
      /* --indicator-style=classify (aka -F) requires statting each
         regular file to see whether it's executable.  */
      || ((type == عام_2 || type == unknown)
          && (indicator_style == classify
              /* This is so that --color ends up highlighting files with these
                 mode bits set even when options like -F are not specified.  */
              || (print_with_color && (is_colored (C_EXEC)
                                       || is_colored (C_SETUID)
                                       || is_colored (C_SETGID))))));

  /* Absolute name of this file, if needed.  */
  حرف ثابت *ڀريل_name = name;
  إذا (check_stat | print_scontext | format_needs_capability
      && name[0] != '/' && dirname)
    {
      حرف *p = alloca (لڙي_ڊيگهه (name) + لڙي_ڊيگهه (dirname) + 2);
      attach (p, dirname, name);
      ڀريل_name = p;
    }

  bool do_deref;

  إذا (!check_stat)
    do_deref = dereference == DEREF_ALWAYS;
  وإلا
    {
      صحيح err;

      إذا (print_hyperlink)
        {
          f->absolute_name = canonicalize_filename_گهڻائي (ڀريل_name,
                                                         CAN_MISSING);
          إذا (! f->absolute_name
              && (command_سٽ_arg
                  || ! ignorable_traversal_errno (errno)))
            file_failure (command_سٽ_arg,
                          _("error canonicalizing %s"), ڀريل_name);
        }

      بدل (dereference)
        {
        حالة DEREF_ALWAYS:
          err = do_stat (ڀريل_name, &f->stat);
          do_deref = true;
          اكسر;

        حالة DEREF_COMMAND_سٽ_ARGUMENTS:
        حالة DEREF_COMMAND_سٽ_SYMLINK_منزل_DIR:
          إذا (command_سٽ_arg)
            {
              bool need_lstat;
              err = do_stat (ڀريل_name, &f->stat);
              do_deref = true;

              إذا (dereference == DEREF_COMMAND_سٽ_ARGUMENTS)
                اكسر;

              need_lstat = (err < 0
                            ? (errno == ENOENT || errno == ELOOP)
                            : ! S_ISDIR (f->stat.st_گهڻائي));
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
          err = do_lstat (ڀريل_name, &f->stat);
          do_deref = false;
          اكسر;

        حالة DEREF_UNDEFINED: افتراضي:
          affirm (false);
        }

      إذا (err != 0)
        {
          إذا (! command_سٽ_arg && ignorable_traversal_errno (errno))
            {
              صحيح stat_errno = errno;
              بنية stat linkstat;
              bool dangling_link = (do_deref
                                    && (type == symbolic_link
                                        || type == unknown)
                                    && do_lstat (ڀريل_name, &linkstat) == 0);
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
          file_failure (command_سٽ_arg,
                        _("cannot access %s"), ڀريل_name);

          إذا (command_سٽ_arg)
            {
              free_ent (f);
              أرجع 0;
            }

          f->name = xstrdup (name);
          cwd_n_استعمال_ٿيل++;

          أرجع 0;
        }

      f->stat_ok = true;
      f->filetype = type = d_type_filetype[IFTODT (f->stat.st_گهڻائي)];
    }

  إذا (type == directory && command_سٽ_arg && !فوري_dirs)
    f->filetype = type = arg_directory;

  get_extended_info (ڀريل_name, f, do_deref);

  إذا ((type == symbolic_link)
      & ((format == long_format) | check_symlink_گهڻائي))
    {
      بنية stat linkstats;

      إذا (! get_link_name (ڀريل_name, f, command_سٽ_arg))
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
          && (file_type <= indicator_style || check_symlink_گهڻائي)
          && stat_for_گهڻائي (ڀريل_name, &linkstats) == 0)
        {
          f->linkok = true;
          f->linkmode = linkstats.st_گهڻائي;
        }
    }

  بلاڪون = STP_NBLOCKS (&f->stat);
  إذا (format == long_format || print_بلاڪ_سائيز)
    {
      حرف buf[LONGEST_HUMAN_READABLE + 1];
      صحيح len = mbswidth (human_readable (بلاڪون, buf, human_output_opts,
                                          ST_NBLOCKSIZE, output_بلاڪ_سائيز),
                          MBSWIDTH_FLAGS);
      إذا (بلاڪ_سائيز_ويڪر < len)
        بلاڪ_سائيز_ويڪر = len;
    }

  إذا (format == long_format)
    {
      إذا (print_owner)
        {
          صحيح len = format_user_ويڪر (f->stat.st_uid);
          إذا (owner_ويڪر < len)
            owner_ويڪر = len;
        }

      إذا (print_گروهه)
        {
          صحيح len = format_گروهه_ويڪر (f->stat.st_gid);
          إذا (گروهه_ويڪر < len)
            گروهه_ويڪر = len;
        }

      إذا (print_author)
        {
          صحيح len = format_user_ويڪر (f->stat.st_author);
          إذا (author_ويڪر < len)
            author_ويڪر = len;
        }
    }

  إذا (print_scontext)
    {
      صحيح len = لڙي_ڊيگهه (f->scontext);
      إذا (scontext_ويڪر < len)
        scontext_ويڪر = len;
    }

  إذا (format == long_format)
    {
      حرف b[INT_BUFSIZE_BOUND (uintmax_t)];
      صحيح b_len = لڙي_ڊيگهه (umaxtostr (f->stat.st_nlink, b));
      إذا (nlink_ويڪر < b_len)
        nlink_ويڪر = b_len;

      إذا ((type == chardev) | (type == blockdev))
        {
          حرف buf[INT_BUFSIZE_BOUND (uintmax_t)];
          صحيح len = لڙي_ڊيگهه (umaxtostr (major (f->stat.st_rdev), buf));
          إذا (major_device_عدد_ويڪر < len)
            major_device_عدد_ويڪر = len;
          len = لڙي_ڊيگهه (umaxtostr (minor (f->stat.st_rdev), buf));
          إذا (minor_device_عدد_ويڪر < len)
            minor_device_عدد_ويڪر = len;
          len = major_device_عدد_ويڪر + 2 + minor_device_عدد_ويڪر;
          إذا (file_سائيز_ويڪر < len)
            file_سائيز_ويڪر = len;
        }
      وإلا
        {
          حرف buf[LONGEST_HUMAN_READABLE + 1];
          uintmax_t سائيز = unsigned_file_سائيز (f->stat.st_سائيز);
          صحيح len = mbswidth (human_readable (سائيز, buf,
                                              file_human_output_opts,
                                              1, file_output_بلاڪ_سائيز),
                              MBSWIDTH_FLAGS);
          إذا (file_سائيز_ويڪر < len)
            file_سائيز_ويڪر = len;
        }
    }

  إذا (print_inode)
    {
      حرف buf[INT_BUFSIZE_BOUND (uintmax_t)];
      صحيح len = لڙي_ڊيگهه (umaxtostr (f->stat.st_ino, buf));
      إذا (inode_عدد_ويڪر < len)
        inode_عدد_ويڪر = len;
    }

  f->name = xstrdup (name);
  cwd_n_استعمال_ٿيل++;

  أرجع بلاڪون;
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
get_link_name (حرف ثابت *filename, بنية fileinfo *f, bool command_سٽ_arg)
{
  f->linkname = areadlink_with_سائيز (filename, f->stat.st_سائيز);
  إذا (f->linkname == عدم)
    {
      إذا (! command_سٽ_arg
          && ignorable_traversal_errno (errno))
        أرجع false;
      file_failure (command_سٽ_arg, _("cannot read symbolic link %s"),
                    filename);
    }
  أرجع true;
}

/* Return true if the last component of NAME is '.' or '..'
   This is so we don't try to recurse on '././././. ...' */

ساكن bool
basename_is_dot_or_dotdot (حرف ثابت *name)
{
  حرف ثابت *بنياد_2 = آخري_component (name);
  أرجع dot_or_dotdot (بنياد_2);
}

/* Remove any entries from CWD_FILE that are for directories,
   and queue them to be listed as directories instead.
   DIRNAME is the prefix to prepend to each dirname
   to make it correct relative to ls's working dir;
   if it is null, no prefix is needed and "." and ".." should not be ignored.
   If COMMAND_LINE_ARG is true, this directory was mentioned at the top level,
   This is desirable when processing directories recursively.  */

ساكن فراغ
ڪڍو_dirs_from_files (حرف ثابت *dirname, bool command_سٽ_arg)
{
  idx_t i, j;
  bool ignore_dot_and_dot_dot = (dirname != عدم);

  إذا (dirname && LOOP_DETECT)
    {
      /* Insert a marker entry first.  When we dequeue this marker entry,
         we'll know that DIRNAME has been processed and may be removed
         from the set of active directories.  */
      قطار_directory (عدم, dirname, false);
    }

  /* Queue the directories last one first, because queueing reverses the
     order.  */
  لكل (i = cwd_n_استعمال_ٿيل; 0 < i; )
    {
      i--;
      بنية fileinfo *f = sorted_file[i];

      إذا (is_directory (f)
          && (! ignore_dot_and_dot_dot
              || ! basename_is_dot_or_dotdot (f->name)))
        {
          إذا (!dirname || f->name[0] == '/')
            قطار_directory (f->name, f->linkname, command_سٽ_arg);
          وإلا
            {
              حرف *name = file_name_concat (dirname, f->name, عدم);
              قطار_directory (name, f->linkname, command_سٽ_arg);
              حرر (name);
            }
          إذا (f->filetype == arg_directory)
            free_ent (f);
        }
    }

  /* Now delete the directories from the table, compacting all the remaining
     entries.  */

  لكل (i = 0, j = 0; i < cwd_n_استعمال_ٿيل; i++)
    {
      بنية fileinfo *f = sorted_file[i];
      sorted_file[j] = f;
      j += (f->filetype != arg_directory);
    }
  cwd_n_استعمال_ٿيل = j;
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
      سيٽ_exit_حالت (false);
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
#وصف_ڪريو DEFINE_ترتيب_ڏيو_FUNCTIONS(ڪنجي_name, ڪنجي_cmp_func)			\
  /* direct, non-dirfirst versions */					\
  ساكن صحيح xstrcoll_##key_name (V a, V b)				\
  { أرجع ڪنجي_cmp_func (a, b, xstrcoll); }				\
  ATTRIBUTE_خالص ساكن صحيح strcmp_##key_name (V a, V b)		\
  { أرجع ڪنجي_cmp_func (a, b, strcmp); }				\
                                                                        \
  /* reverse, non-dirfirst versions */					\
  ساكن صحيح rev_xstrcoll_##key_name (V a, V b)				\
  { أرجع ڪنجي_cmp_func (b, a, xstrcoll); }				\
  ATTRIBUTE_خالص ساكن صحيح rev_strcmp_##key_name (V a, V b)	\
  { أرجع ڪنجي_cmp_func (b, a, strcmp); }				\
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
cmp_سائيز (بنية fileinfo ثابت *a, بنية fileinfo ثابت *b,
          صحيح (*cmp) (حرف ثابت *, حرف ثابت *))
{
  صحيح diff = off_cmp (b->stat.st_سائيز, a->stat.st_سائيز);
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
fileinfo_name_ويڪر (بنية fileinfo ثابت *f)
{
  أرجع f->ويڪر
         ? f->ويڪر
         : quote_name_ويڪر (f->name, filename_quoting_options, f->quoted);
}

ساكن صحيح
cmp_ويڪر (بنية fileinfo ثابت *a, بنية fileinfo ثابت *b,
          صحيح (*cmp) (حرف ثابت *, حرف ثابت *))
{
  صحيح diff = fileinfo_name_ويڪر (a) - fileinfo_name_ويڪر (b);
  أرجع diff ? diff : cmp (a->name, b->name);
}

DEFINE_ترتيب_ڏيو_FUNCTIONS (ctime, cmp_ctime)
DEFINE_ترتيب_ڏيو_FUNCTIONS (mtime, cmp_mtime)
DEFINE_ترتيب_ڏيو_FUNCTIONS (atime, cmp_atime)
DEFINE_ترتيب_ڏيو_FUNCTIONS (btime, cmp_btime)
DEFINE_ترتيب_ڏيو_FUNCTIONS (سائيز, cmp_سائيز)
DEFINE_ترتيب_ڏيو_FUNCTIONS (name, cmp_name)
DEFINE_ترتيب_ڏيو_FUNCTIONS (extension, cmp_extension)
DEFINE_ترتيب_ڏيو_FUNCTIONS (ويڪر, cmp_ويڪر)

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
cmp_نسخو (بنية fileinfo ثابت *a, بنية fileinfo ثابت *b)
{
  صحيح diff = filevercmp (a->name, b->name);
  أرجع diff ? diff : strcmp (a->name, b->name);
}

ساكن صحيح
xstrcoll_نسخو (V a, V b)
{
  أرجع cmp_نسخو (a, b);
}
ساكن صحيح
rev_xstrcoll_نسخو (V a, V b)
{
  أرجع cmp_نسخو (b, a);
}
ساكن صحيح
xstrcoll_df_نسخو (V a, V b)
{
  أرجع dirfirst_check (a, b, xstrcoll_نسخو);
}
ساكن صحيح
rev_xstrcoll_df_نسخو (V a, V b)
{
  أرجع dirfirst_check (a, b, rev_xstrcoll_نسخو);
}


/* We have 2^3 different variants for each sort-key function
   (for 3 independent sort modes).
   The function pointers stored in this array must be dereferenced as:

    sort_variants[sort_key][use_strcmp][reverse][dirs_first]

   Note that the order in which sort keys are listed in the function pointer
   array below is defined by the order of the elements in the time_type and
   sort_type enums!  */

#وصف_ڪريو LIST_SORTFUNCTION_VARIANTS(ڪنجي_name)                        \
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

ساكن qsortFunc ثابت ترتيب_ڏيو_functions[][2][2][2] =
  {
    LIST_SORTFUNCTION_VARIANTS (name),
    LIST_SORTFUNCTION_VARIANTS (extension),
    LIST_SORTFUNCTION_VARIANTS (ويڪر),
    LIST_SORTFUNCTION_VARIANTS (سائيز),

    {
      {
        { xstrcoll_نسخو, xstrcoll_df_نسخو },
        { rev_xstrcoll_نسخو, rev_xstrcoll_df_نسخو },
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
static_assert (countof (ترتيب_ڏيو_functions) == ترتيب_ڏيو_numtypes - 2 + وقت_numtypes);

/* Set up SORTED_FILE to point to the in-use entries in CWD_FILE, in order.  */

ساكن فراغ
initialize_ordering_ويڪٽر (فراغ)
{
  لكل (idx_t i = 0; i < cwd_n_استعمال_ٿيل; i++)
    sorted_file[i] = &cwd_file[i];
}

/* Cache values based on attributes global to all files.  */

ساكن فراغ
update_current_files_info (فراغ)
{
  /* Cache screen width of name, if needed multiple times.  */
  إذا (ترتيب_ڏيو_type == ترتيب_ڏيو_ويڪر
      || (سٽ_ڊيگهائي && (format == many_في_سٽ || format == horizontal)))
    {
      لكل (idx_t i = 0; i < cwd_n_استعمال_ٿيل; i++)
        {
          بنية fileinfo *f = sorted_file[i];
          f->ويڪر = fileinfo_name_ويڪر (f);
        }
    }
}

/* Sort the files now in the table.  */

ساكن فراغ
ترتيب_ڏيو_files (فراغ)
{
  bool use_strcmp;

  إذا (sorted_file_alloc < cwd_n_استعمال_ٿيل + (cwd_n_استعمال_ٿيل >> 1))
    {
      حرر (sorted_file);
      sorted_file = xinmalloc (cwd_n_استعمال_ٿيل, 3 * حجم *sorted_file);
      sorted_file_alloc = 3 * cwd_n_استعمال_ٿيل;
    }

  initialize_ordering_ويڪٽر ();

  update_current_files_info ();

  إذا (ترتيب_ڏيو_type == ترتيب_ڏيو_none)
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
      affirm (ترتيب_ڏيو_type != ترتيب_ڏيو_نسخو);
      initialize_ordering_ويڪٽر ();
    }

  /* When sort_type == sort_time, use time_type as subindex.  */
  mpsort ((فراغ ثابت **) sorted_file, cwd_n_استعمال_ٿيل,
          ترتيب_ڏيو_functions[ترتيب_ڏيو_type + (ترتيب_ڏيو_type == ترتيب_ڏيو_وقت ? وقت_type : 0)]
                        [use_strcmp][ترتيب_ڏيو_ابتو]
                        [directories_پهريون]);
}

/* List all the files now in the table.  */

ساكن فراغ
print_current_files (فراغ)
{
  بدل (format)
    {
    حالة one_في_سٽ:
      لكل (idx_t i = 0; i < cwd_n_استعمال_ٿيل; i++)
        {
          print_file_name_and_frills (sorted_file[i], 0);
          اکر_لکو (eolbyte);
        }
      اكسر;

    حالة many_في_سٽ:
      إذا (! سٽ_ڊيگهائي)
        print_with_separator (' ');
      وإلا
        print_many_في_سٽ ();
      اكسر;

    حالة horizontal:
      إذا (! سٽ_ڊيگهائي)
        print_with_separator (' ');
      وإلا
        print_horizontal ();
      اكسر;

    حالة with_commas:
      print_with_separator (',');
      اكسر;

    حالة long_format:
      لكل (idx_t i = 0; i < cwd_n_استعمال_ٿيل; i++)
        {
          سيٽ_عام_color ();
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
align_nstrftime (حرف *buf, size_t سائيز, bool recent, بنية tm ثابت *tm,
                 timezone_t tz, صحيح ns)
{
  حرف ثابت *nfmt = (use_abformat
                      ? abformat[recent][tm->tm_mon]
                      : long_وقت_format[recent]);
  أرجع nstrftime (buf, سائيز, nfmt, tm, tz, ns);
}

/* Return the expected number of columns in a long-format timestamp,
   or zero if it cannot be calculated.  */

ساكن صحيح
long_وقت_expected_ويڪر (فراغ)
{
  ساكن صحيح ويڪر = -1;

  إذا (ويڪر < 0)
    {
      time_t epoch = 0;
      بنية tm tm;
      حرف buf[وقت_STAMP_LEN_وڌ_کان_وڌ + 1];

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
            ويڪر = mbsnwidth (buf, len, MBSWIDTH_FLAGS);
        }

      إذا (ويڪر < 0)
        ويڪر = 0;
    }

  أرجع ويڪر;
}

/* Print the user or group name NAME, with numeric id ID, using a
   print width of WIDTH columns.  */

ساكن فراغ
format_user_or_گروهه (حرف ثابت *name, uintmax_t id, صحيح ويڪر)
{
  إذا (name)
    {
      صحيح name_ويڪر = mbswidth (name, MBSWIDTH_FLAGS);
      صحيح ويڪر_gap = name_ويڪر < 0 ? 0 : ويڪر - name_ويڪر;
      صحيح pad = MAX (0, ويڪر_gap);
      dired_outstring (name);

      افعل
        dired_outbyte (' ');
      طالما (pad--);
    }
  وإلا
    dired_pos += لکو ("%*ju ", ويڪر, id);
}

/* Print the name or id of the user with id U, using a print width of
   WIDTH.  */

ساكن فراغ
format_user (uid_t u, صحيح ويڪر, bool stat_ok)
{
  format_user_or_گروهه (! stat_ok ? "?" :
                        (numeric_ids ? عدم : getuser (u)), u, ويڪر);
}

/* Likewise, for groups.  */

ساكن فراغ
format_گروهه (gid_t g, صحيح ويڪر, bool stat_ok)
{
  format_user_or_گروهه (! stat_ok ? "?" :
                        (numeric_ids ? عدم : getgroup (g)), g, ويڪر);
}

/* Return the number of columns that format_user_or_group will print,
   or -1 if unknown.  */

ساكن صحيح
format_user_or_گروهه_ويڪر (حرف ثابت *name, uintmax_t id)
{
  أرجع (name
          ? mbswidth (name, MBSWIDTH_FLAGS)
          : snprintf (عدم, 0, "%ju", id));
}

/* Return the number of columns that format_user will print,
   or -1 if unknown.  */

ساكن صحيح
format_user_ويڪر (uid_t u)
{
  أرجع format_user_or_گروهه_ويڪر (numeric_ids ? عدم : getuser (u), u);
}

/* Likewise, for groups.  */

ساكن صحيح
format_گروهه_ويڪر (gid_t g)
{
  أرجع format_user_or_گروهه_ويڪر (numeric_ids ? عدم : getgroup (g), g);
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
     + وقت_STAMP_LEN_وڌ_کان_وڌ + 1	/* max length of time/date */
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

  بدل (وقت_type)
    {
    حالة وقت_ctime:
      when_timespec = get_stat_ctime (&f->stat);
      اكسر;
    حالة وقت_mtime:
      when_timespec = get_stat_mtime (&f->stat);
      اكسر;
    حالة وقت_atime:
      when_timespec = get_stat_atime (&f->stat);
      اكسر;
    حالة وقت_btime:
      when_timespec = get_stat_btime (&f->stat);
      إذا (when_timespec.tv_sec == -1 && when_timespec.tv_nsec == -1)
        btime_ok = false;
      اكسر;
    حالة وقت_numtypes: افتراضي:
      unreachable ();
    }

  p = buf;

  إذا (print_inode)
    {
      حرف hbuf[INT_BUFSIZE_BOUND (uintmax_t)];
      p += sprintf (p, "%*s ", inode_عدد_ويڪر, format_inode (hbuf, f));
    }

  إذا (print_بلاڪ_سائيز)
    {
      حرف hbuf[LONGEST_HUMAN_READABLE + 1];
      حرف ثابت *بلاڪون =
        (! f->stat_ok
         ? "?"
         : human_readable (STP_NBLOCKS (&f->stat), hbuf, human_output_opts,
                           ST_NBLOCKSIZE, output_بلاڪ_سائيز));
      صحيح بلاڪون_ويڪر = mbswidth (بلاڪون, MBSWIDTH_FLAGS);
      لكل (صحيح pad = بلاڪون_ويڪر < 0 ? 0 : بلاڪ_سائيز_ويڪر - بلاڪون_ويڪر;
           0 < pad; pad--)
        *p++ = ' ';
      طالما ((*p++ = *بلاڪون++))
        استمر;
      p[-1] = ' ';
    }

  /* The last byte of the mode string is the POSIX
     "optional alternate access method flag".  */
  {
    حرف hbuf[INT_BUFSIZE_BOUND (uintmax_t)];
    p += sprintf (p, "%s %*s ", modebuf, nlink_ويڪر,
                  ! f->stat_ok ? "?" : umaxtostr (f->stat.st_nlink, hbuf));
  }

  dired_indent ();

  إذا (print_owner || print_گروهه || print_author || print_scontext)
    {
      dired_outbuf (buf, p - buf);

      إذا (print_owner)
        format_user (f->stat.st_uid, owner_ويڪر, f->stat_ok);

      إذا (print_گروهه)
        format_گروهه (f->stat.st_gid, گروهه_ويڪر, f->stat_ok);

      إذا (print_author)
        format_user (f->stat.st_author, author_ويڪر, f->stat_ok);

      إذا (print_scontext)
        format_user_or_گروهه (f->scontext, 0, scontext_ويڪر);

      p = buf;
    }

  إذا (f->stat_ok
      && (S_ISCHR (f->stat.st_گهڻائي) || S_ISBLK (f->stat.st_گهڻائي)))
    {
      صحيح blanks_ويڪر = (file_سائيز_ويڪر
                          - (major_device_عدد_ويڪر + 2
                             + minor_device_عدد_ويڪر));
      p += sprintf (p, "%*ju, %*ju ",
                    major_device_عدد_ويڪر + MAX (0, blanks_ويڪر),
                    (uintmax_t) major (f->stat.st_rdev),
                    minor_device_عدد_ويڪر,
                    (uintmax_t) minor (f->stat.st_rdev));
    }
  وإلا
    {
      حرف hbuf[LONGEST_HUMAN_READABLE + 1];
      حرف ثابت *سائيز =
        (! f->stat_ok
         ? "?"
         : human_readable (unsigned_file_سائيز (f->stat.st_سائيز),
                           hbuf, file_human_output_opts, 1,
                           file_output_بلاڪ_سائيز));
      صحيح سائيز_ويڪر = mbswidth (سائيز, MBSWIDTH_FLAGS);
      لكل (صحيح pad = سائيز_ويڪر < 0 ? 0 : file_سائيز_ويڪر - سائيز_ويڪر;
           0 < pad; pad--)
        *p++ = ' ';
      طالما ((*p++ = *سائيز++))
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
      إذا (timespec_cmp (current_وقت, when_timespec) < 0)
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
      s = align_nstrftime (p, وقت_STAMP_LEN_وڌ_کان_وڌ + 1, recent,
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
      p += sprintf (p, "%*s ", long_وقت_expected_ويڪر (),
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
    print_type_indicator (f->stat_ok, f->stat.st_گهڻائي, f->filetype);
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
                صحيح needs_general_quoting, size_t *ويڪر, bool *pad)
{
  حرف *buf = *inbuf;
  size_t displayed_ويڪر IF_LINT ( = 0);
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
      len = quotearg_بفر (buf, bufsize, name, -1, options);
      إذا (bufsize <= len)
        {
          buf = xmalloc (len + 1);
          quotearg_بفر (buf, len + 1, name, -1, options);
        }

      quoted = (*name != *buf) || لڙي_ڊيگهه (name) != len;
    }
  وإلا إذا (needs_further_quoting || extra_newline_protection)
    {
      len = لڙي_ڊيگهه (name);
      إذا (bufsize <= len)
        buf = xmalloc (len + 1);
      يادگيري_نقل (buf, name, len + 1);

      quoted = false;
    }
  وإلا
    {
      len = لڙي_ڊيگهه (name);
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
          displayed_ويڪر = 0;

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
                  displayed_ويڪر += 1;
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
                        size_t بائيٽون;
                        صحيح w;

                        بائيٽون = mbrtoc32 (&wc, p, plimit - p, &mbstate);

                        إذا (بائيٽون == (size_t) -1)
                          {
                            /* An invalid multibyte sequence was
                               encountered.  Skip one input byte, and
                               put a question mark.  */
                            p++;
                            *q++ = '?';
                            displayed_ويڪر += 1;
                            اكسر;
                          }

                        إذا (بائيٽون == (size_t) -2)
                          {
                            /* An incomplete multibyte character
                               at the end.  Replace it entirely with
                               a question mark.  */
                            p = plimit;
                            *q++ = '?';
                            displayed_ويڪر += 1;
                            اكسر;
                          }

                        إذا (بائيٽون == 0)
                          /* A null wide character was encountered.  */
                          بائيٽون = 1;

                        w = c32width (wc);
                        إذا (w >= 0)
                          {
                            /* A printable multibyte character.
                               Keep it.  */
                            لكل (; بائيٽون > 0; --بائيٽون)
                              *q++ = *p++;
                            displayed_ويڪر += w;
                          }
                        وإلا
                          {
                            /* An nonprintable multibyte character.
                               Replace it entirely with a question
                               mark.  */
                            p += بائيٽون;
                            *q++ = '?';
                            displayed_ويڪر += 1;
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
              إذا (! isprint (منزل_uchar (*p)))
                *p = '?';
              p++;
            }
          displayed_ويڪر = len;
        }
    }
  وإلا إذا (ويڪر != عدم)
    {
      إذا (MB_CUR_MAX > 1)
        {
          displayed_ويڪر = mbsnwidth (buf, len, MBSWIDTH_FLAGS);
          displayed_ويڪر = MAX (0, displayed_ويڪر);
        }
      وإلا
        {
          حرف ثابت *p = buf;
          حرف ثابت *plimit = buf + len;

          displayed_ويڪر = 0;
          طالما (p < plimit)
            {
              إذا (isprint (منزل_uchar (*p)))
                displayed_ويڪر++;
              p++;
            }
        }
    }

  /* Set padding to better align quoted items,
     and also give a visual indication that quotes are
     not actually part of the name.  */
  *pad = (align_variable_outer_quotes && cwd_some_quoted && ! quoted);

  إذا (ويڪر != عدم)
    *ويڪر = displayed_ويڪر;

  *inbuf = buf;

  أرجع len;
}

ساكن size_t
quote_name_ويڪر (حرف ثابت *name, بنية quoting_options ثابت *options,
                  صحيح needs_general_quoting)
{
  حرف smallbuf[BUFSIZ];
  حرف *buf = smallbuf;
  size_t ويڪر;
  bool pad;

  quote_name_buf (&buf, حجم smallbuf, (حرف *) name, options,
                  needs_general_quoting, &ويڪر, &pad);

  إذا (buf != smallbuf && buf != name)
    حرر (buf);

  ويڪر += pad;

  أرجع ويڪر;
}

/* %XX escape any input out of range as defined in RFC3986,
   and also if PATH, convert all path separators to '/'.  */
ساكن حرف *
file_escape (حرف ثابت *str, bool رستو)
{
  حرف *esc = xnmalloc (3, لڙي_ڊيگهه (str) + 1);
  حرف *p = esc;
  طالما (*str)
    {
      إذا (رستو && ISSLASH (*str))
        {
          *p++ = '/';
          str++;
        }
      وإلا إذا (RFC3986[منزل_uchar (*str)])
        *p++ = *str++;
      وإلا
        p += sprintf (p, "%%%02x", منزل_uchar (*str++));
    }
  *p = '\0';
  أرجع esc;
}

ساكن size_t
quote_name (حرف ثابت *name, بنية quoting_options ثابت *options,
            صحيح needs_general_quoting, ثابت بنية bin_str *color,
            bool allow_pad, بنية obstack *ڍير, حرف ثابت *absolute_name)
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
          اکر_لکو (*buf);
        }
      حرف *h = file_escape (hostname, /* path= */ false);
      حرف *n = file_escape (absolute_name, /* path= */ true);
      /* TODO: It would be good to be able to define parameters
         to give hints to the terminal as how best to render the URI.
         For example since ls is outputting a dense block of URIs
         it would be best to not underline by default, and only
         do so upon hover etc.  */
      لکو ("\033]8;;file://%s%s%s\033\\", h, *n == '/' ? "" : "/", n);
      حرر (h);
      حرر (n);
    }

  إذا (ڍير)
    ڌڪو_current_dired_pos (ڍير);

  fwrite (buf + skip_quotes, 1, len - (skip_quotes * 2), stdout);

  dired_pos += len;

  إذا (ڍير)
    ڌڪو_current_dired_pos (ڍير);

  إذا (absolute_name)
    {
      fputs ("\033]8;;\033\\", stdout);
      إذا (skip_quotes)
        اکر_لکو (*(buf + len - 1));
    }

  إذا (buf != smallbuf && buf != name)
    حرر (buf);

  أرجع len + pad;
}

ساكن size_t
print_name_with_quoting (ثابت بنية fileinfo *f,
                         bool symlink_هدف,
                         بنية obstack *ڍير,
                         size_t شروعات_کانو)
{
  حرف ثابت *name = symlink_هدف ? f->linkname : f->name;

  ثابت بنية bin_str *color
    = print_with_color ? get_color_indicator (f, symlink_هدف) : عدم;

  bool استعمال_ٿيل_color_this_وقت = (print_with_color
                               && (color || is_colored (C_NORM)));

  size_t len = quote_name (name, filename_quoting_options, f->quoted,
                           color, !symlink_هدف, ڍير, f->absolute_name);

  process_سگنل ();
  إذا (استعمال_ٿيل_color_this_وقت)
    {
      prep_non_filename_متن ();

      /* We use the byte length rather than display width here as
         an optimization to avoid accurately calculating the width,
         because we only output the clear to EOL sequence if the name
         _might_ wrap to the next line.  This may output a sequence
         unnecessarily in multi-byte locales for example,
         but in that case it's inconsequential to the output.  */
      إذا (سٽ_ڊيگهائي
          && (شروعات_کانو / سٽ_ڊيگهائي != (شروعات_کانو + len - 1) / سٽ_ڊيگهائي))
        put_indicator (&color_indicator[C_CLR_منزل_EOL]);
    }

  أرجع len;
}

ساكن فراغ
prep_non_filename_متن (فراغ)
{
  إذا (color_indicator[C_پڇاڙي].string != عدم)
    put_indicator (&color_indicator[C_پڇاڙي]);
  وإلا
    {
      put_indicator (&color_indicator[C_کاٻو]);
      put_indicator (&color_indicator[C_RESET]);
      put_indicator (&color_indicator[C_ساڄو]);
    }
}

/* Print the file name of 'f' with appropriate quoting.
   Also print file size, inode number, and filetype indicator character,
   as requested by switches.  */

ساكن size_t
print_file_name_and_frills (ثابت بنية fileinfo *f, size_t شروعات_کانو)
{
  حرف buf[MAX (LONGEST_HUMAN_READABLE + 1, INT_BUFSIZE_BOUND (uintmax_t))];

  سيٽ_عام_color ();

  إذا (print_inode)
    لکو ("%*s ", format == with_commas ? 0 : inode_عدد_ويڪر,
            format_inode (buf, f));

  إذا (print_بلاڪ_سائيز)
    {
      حرف ثابت *بلاڪون =
        (! f->stat_ok
         ? "?"
         : human_readable (STP_NBLOCKS (&f->stat), buf, human_output_opts,
                           ST_NBLOCKSIZE, output_بلاڪ_سائيز));
      صحيح بلاڪون_ويڪر = mbswidth (بلاڪون, MBSWIDTH_FLAGS);
      صحيح pad = 0;
      إذا (0 <= بلاڪون_ويڪر && بلاڪ_سائيز_ويڪر && format != with_commas)
        pad = بلاڪ_سائيز_ويڪر - بلاڪون_ويڪر;
      لکو ("%*s%s ", pad, "", بلاڪون);
    }

  إذا (print_scontext)
    لکو ("%*s ", format == with_commas ? 0 : scontext_ويڪر, f->scontext);

  size_t ويڪر = print_name_with_quoting (f, false, عدم, شروعات_کانو);

  إذا (indicator_style != none)
    ويڪر += print_type_indicator (f->stat_ok, f->stat.st_گهڻائي, f->filetype);

  أرجع ويڪر;
}

/* Given these arguments describing a file, return the single-byte
   type indicator, or 0.  */
ساكن حرف
get_type_indicator (bool stat_ok, mode_t گهڻائي_2, تعداد filetype type)
{
  حرف c;

  إذا (stat_ok ? S_ISREG (گهڻائي_2) : type == عام_2)
    {
      إذا (stat_ok && indicator_style == classify && (گهڻائي_2 & S_IXUGO))
        c = '*';
      وإلا
        c = 0;
    }
  وإلا
    {
      إذا (stat_ok ? S_ISDIR (گهڻائي_2) : type == directory || type == arg_directory)
        c = '/';
      وإلا إذا (indicator_style == slash)
        c = 0;
      وإلا إذا (stat_ok ? S_ISLNK (گهڻائي_2) : type == symbolic_link)
        c = '@';
      وإلا إذا (stat_ok ? S_ISFIFO (گهڻائي_2) : type == fifo)
        c = '|';
      وإلا إذا (stat_ok ? S_ISSOCK (گهڻائي_2) : type == sock)
        c = '=';
      وإلا إذا (stat_ok && S_ISDOOR (گهڻائي_2))
        c = '>';
      وإلا
        c = 0;
    }
  أرجع c;
}

ساكن bool
print_type_indicator (bool stat_ok, mode_t گهڻائي_2, تعداد filetype type)
{
  حرف c = get_type_indicator (stat_ok, گهڻائي_2, type);
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
      put_indicator (&color_indicator[C_کاٻو]);
      put_indicator (ind);
      put_indicator (&color_indicator[C_ساڄو]);
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
  mode_t گهڻائي_2;
  صحيح linkok;
  إذا (symlink_هدف)
    {
      name = f->linkname;
      گهڻائي_2 = f->linkmode;
      linkok = f->linkok ? 0 : -1;
    }
  وإلا
    {
      name = f->name;
      گهڻائي_2 = file_or_link_گهڻائي (f);
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
      إذا (S_ISREG (گهڻائي_2))
        {
          type = C_FILE;

          إذا ((گهڻائي_2 & S_ISUID) != 0 && is_colored (C_SETUID))
            type = C_SETUID;
          وإلا إذا ((گهڻائي_2 & S_ISGID) != 0 && is_colored (C_SETGID))
            type = C_SETGID;
          وإلا إذا (f->has_capability)
            type = C_CAP;
          وإلا إذا ((گهڻائي_2 & S_IXUGO) != 0 && is_colored (C_EXEC))
            type = C_EXEC;
          وإلا إذا ((1 < f->stat.st_nlink) && is_colored (C_MULTIHARDLINK))
            type = C_MULTIHARDLINK;
        }
      وإلا إذا (S_ISDIR (گهڻائي_2))
        {
          type = C_DIR;

          إذا ((گهڻائي_2 & S_ISVTX) && (گهڻائي_2 & S_IWOTH)
              && is_colored (C_STICKY_OTHER_WRITABLE))
            type = C_STICKY_OTHER_WRITABLE;
          وإلا إذا ((گهڻائي_2 & S_IWOTH) != 0 && is_colored (C_OTHER_WRITABLE))
            type = C_OTHER_WRITABLE;
          وإلا إذا ((گهڻائي_2 & S_ISVTX) != 0 && is_colored (C_STICKY))
            type = C_STICKY;
        }
      وإلا إذا (S_ISLNK (گهڻائي_2))
        type = C_LINK;
      وإلا إذا (S_ISFIFO (گهڻائي_2))
        type = C_FIFO;
      وإلا إذا (S_ISSOCK (گهڻائي_2))
        type = C_SOCK;
      وإلا إذا (S_ISBLK (گهڻائي_2))
        type = C_BLK;
      وإلا إذا (S_ISCHR (گهڻائي_2))
        type = C_CHR;
      وإلا إذا (S_ISDOOR (گهڻائي_2))
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

      len = لڙي_ڊيگهه (name);
      name += len;		/* Pointer to final \0.  */
      لكل (ext = color_ext_list; ext != عدم; ext = ext->ايندڙ)
        {
          إذا (ext->ext.len <= len)
            {
              إذا (ext->exact_ميلاپ)
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
  إذا (! استعمال_ٿيل_color)
    {
      استعمال_ٿيل_color = true;

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
ڊيگهائي_جو_file_name_and_frills (ثابت بنية fileinfo *f)
{
  size_t len = 0;
  حرف buf[MAX (LONGEST_HUMAN_READABLE + 1, INT_BUFSIZE_BOUND (uintmax_t))];

  إذا (print_inode)
    len += 1 + (format == with_commas
                ? لڙي_ڊيگهه (umaxtostr (f->stat.st_ino, buf))
                : inode_عدد_ويڪر);

  إذا (print_بلاڪ_سائيز)
    len += 1 + (format == with_commas
                ? لڙي_ڊيگهه (! f->stat_ok ? "?"
                          : human_readable (STP_NBLOCKS (&f->stat), buf,
                                            human_output_opts, ST_NBLOCKSIZE,
                                            output_بلاڪ_سائيز))
                : بلاڪ_سائيز_ويڪر);

  إذا (print_scontext)
    len += 1 + (format == with_commas ? لڙي_ڊيگهه (f->scontext) : scontext_ويڪر);

  len += fileinfo_name_ويڪر (f);

  إذا (indicator_style != none)
    {
      حرف c = get_type_indicator (f->stat_ok, f->stat.st_گهڻائي, f->filetype);
      len += (c != 0);
    }

  أرجع len;
}

ساكن فراغ
print_many_في_سٽ (فراغ)
{
  idx_t کانن_ڳڻپ_2 = calculate_columns (true);
  بنية ڪالم_info ثابت *سٽ_fmt = &ڪالم_info[کانن_ڳڻپ_2 - 1];

  /* Calculate the number of rows that will be in each column except possibly
     for a short column on the right.  */
  idx_t صفون = cwd_n_استعمال_ٿيل / کانن_ڳڻپ_2 + (cwd_n_استعمال_ٿيل % کانن_ڳڻپ_2 != 0);

  لكل (idx_t صف = 0; صف < صفون; صف++)
    {
      size_t کانو = 0;
      idx_t filesno = صف;
      size_t pos = 0;

      /* Print the next row.  */
      طالما (true)
        {
          بنية fileinfo ثابت *f = sorted_file[filesno];
          size_t name_ڊيگهائي = ڊيگهائي_جو_file_name_and_frills (f);
          size_t max_name_ڊيگهائي = سٽ_fmt->کانو_arr[کانو++];
          print_file_name_and_frills (f, pos);

          إذا (cwd_n_استعمال_ٿيل - صفون <= filesno)
            اكسر;
          filesno += صفون;

          indent (pos + name_ڊيگهائي, pos + max_name_ڊيگهائي);
          pos += max_name_ڊيگهائي;
        }
      اکر_لکو (eolbyte);
    }
}

ساكن فراغ
print_horizontal (فراغ)
{
  size_t pos = 0;
  idx_t کانن_ڳڻپ_2 = calculate_columns (false);
  بنية ڪالم_info ثابت *سٽ_fmt = &ڪالم_info[کانن_ڳڻپ_2 - 1];
  بنية fileinfo ثابت *f = sorted_file[0];
  size_t name_ڊيگهائي = ڊيگهائي_جو_file_name_and_frills (f);
  size_t max_name_ڊيگهائي = سٽ_fmt->کانو_arr[0];

  /* Print first entry.  */
  print_file_name_and_frills (f, 0);

  /* Now the rest.  */
  لكل (idx_t filesno = 1; filesno < cwd_n_استعمال_ٿيل; filesno++)
    {
      idx_t کانو = filesno % کانن_ڳڻپ_2;

      إذا (کانو == 0)
        {
          اکر_لکو (eolbyte);
          pos = 0;
        }
      وإلا
        {
          indent (pos + name_ڊيگهائي, pos + max_name_ڊيگهائي);
          pos += max_name_ڊيگهائي;
        }

      f = sorted_file[filesno];
      print_file_name_and_frills (f, pos);

      name_ڊيگهائي = ڊيگهائي_جو_file_name_and_frills (f);
      max_name_ڊيگهائي = سٽ_fmt->کانو_arr[کانو];
    }
  اکر_لکو (eolbyte);
}

/* Output name + SEP + ' '.  */

ساكن فراغ
print_with_separator (حرف sep)
{
  size_t pos = 0;

  لكل (idx_t filesno = 0; filesno < cwd_n_استعمال_ٿيل; filesno++)
    {
      بنية fileinfo ثابت *f = sorted_file[filesno];
      size_t len = سٽ_ڊيگهائي ? ڊيگهائي_جو_file_name_and_frills (f) : 0;

      إذا (filesno != 0)
        {
          حرف separator;
          size_t ايندڙ_pos = 2 + (sep == ',' && filesno < cwd_n_استعمال_ٿيل - 1);

          إذا (! سٽ_ڊيگهائي
              || (! ckd_جوڙيو (&ايندڙ_pos, ايندڙ_pos, pos)
                  && ! ckd_جوڙيو (&ايندڙ_pos, ايندڙ_pos, len)
                  && ايندڙ_pos <= سٽ_ڊيگهائي))
            {
              pos += 2;
              separator = ' ';
            }
          وإلا
            {
              pos = 0;
              separator = eolbyte;
            }

          اکر_لکو (sep);
          اکر_لکو (separator);
        }

      print_file_name_and_frills (f, pos);
      pos += len;
    }
  اکر_لکو (eolbyte);
}

/* Assuming cursor is at position FROM, indent up to position TO.
   Use a TAB character instead of two or more spaces whenever possible.  */

ساكن فراغ
indent (size_t from, size_t منزل)
{
  طالما (from < منزل)
    {
      إذا (tabsize != 0 && منزل / tabsize > (from + 1) / tabsize)
        {
          اکر_لکو ('\t');
          from += tabsize - from % tabsize;
        }
      وإلا
        {
          اکر_لکو (' ');
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
init_ڪالم_info (idx_t max_کانن_ڳڻپ)
{
  /* Currently allocated columns in column_info.  */
  ساكن idx_t ڪالم_info_alloc;

  إذا (ڪالم_info_alloc < max_کانن_ڳڻپ)
    {
      idx_t old_ڪالم_info_alloc = ڪالم_info_alloc;
      ڪالم_info = xpalloc (ڪالم_info, &ڪالم_info_alloc,
                             max_کانن_ڳڻپ - ڪالم_info_alloc, -1,
                             حجم *ڪالم_info);

      /* Allocate the new size_t objects by computing the triangle
         formula n * (n + 1) / 2, except that we don't need to
         allocate the part of the triangle that we've already
         allocated.  Check for address arithmetic overflow.  */
      idx_t ڪالم_info_growth = ڪالم_info_alloc - old_ڪالم_info_alloc, s;
      إذا (ckd_جوڙيو (&s, old_ڪالم_info_alloc + 1, ڪالم_info_alloc)
          || ckd_mul (&s, s, ڪالم_info_growth))
        xalloc_die ();
      size_t *p = xinmalloc (s >> 1, حجم *p);

      /* Grow the triangle by parceling out the cells just allocated.  */
      لكل (idx_t i = old_ڪالم_info_alloc; i < ڪالم_info_alloc; i++)
        {
          ڪالم_info[i].کانو_arr = p;
          p += i + 1;
        }
    }

  لكل (idx_t i = 0; i < max_کانن_ڳڻپ; ++i)
    {
      ڪالم_info[i].valid_len = true;
      ڪالم_info[i].سٽ_len = i * MIN_ڪالم_ويڪر + 1;
      لكل (idx_t j = 0; j < i; ++j)
        ڪالم_info[i].کانو_arr[j] = MIN_ڪالم_ويڪر;
      ڪالم_info[i].کانو_arr[i] = 1;
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
  idx_t max_کانن_ڳڻپ = 0 < max_idx && max_idx < cwd_n_استعمال_ٿيل ? max_idx : cwd_n_استعمال_ٿيل;

  init_ڪالم_info (max_کانن_ڳڻپ);

  /* Compute the maximum number of possible columns.  */
  لكل (idx_t filesno = 0; filesno < cwd_n_استعمال_ٿيل; ++filesno)
    {
      بنية fileinfo ثابت *f = sorted_file[filesno];
      size_t name_ڊيگهائي = ڊيگهائي_جو_file_name_and_frills (f);

      لكل (idx_t i = 0; i < max_کانن_ڳڻپ; ++i)
        {
          إذا (ڪالم_info[i].valid_len)
            {
              idx_t idx = (by_columns
                           ? filesno / ((cwd_n_استعمال_ٿيل + i) / (i + 1))
                           : filesno % (i + 1));
              size_t real_ڊيگهائي = name_ڊيگهائي + (idx == i ? 0 : 2);

              إذا (ڪالم_info[i].کانو_arr[idx] < real_ڊيگهائي)
                {
                  ڪالم_info[i].سٽ_len += (real_ڊيگهائي
                                              - ڪالم_info[i].کانو_arr[idx]);
                  ڪالم_info[i].کانو_arr[idx] = real_ڊيگهائي;
                  ڪالم_info[i].valid_len = (ڪالم_info[i].سٽ_len
                                              <= سٽ_ڊيگهائي);
                }
            }
        }
    }

  /* Find maximum allowed columns.  */
  idx_t کانن_ڳڻپ_2;
  لكل (کانن_ڳڻپ_2 = max_کانن_ڳڻپ; 1 < کانن_ڳڻپ_2; --کانن_ڳڻپ_2)
    {
      إذا (ڪالم_info[کانن_ڳڻپ_2 - 1].valid_len)
        اكسر;
    }

  أرجع کانن_ڳڻپ_2;
}

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکو (_("Usage: %s [OPTION]... [FILE]...\n"), پروگرام_name);
      fputs (_("\
List information about the FILEs (the current directory by افتراضي).\n\
ترتيب_ڏيو_2 entries alphabetically إذا none جو -cftuvSUX nor --ترتيب_ڏيو is specified.\n\
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
         with -l, print the author جو each file\n\
"));
      oputs (_("\
  -b, --escape\n\
         print C-style escapes لكل nongraphic characters\n\
"));
      oputs (_("\
      --بلاڪ-سائيز=سائيز_2\n\
         with -l, scale sizes by سائيز_2 when printing them;\n\
         e.g., '--block-size=M'; see سائيز_2 format هيٺ\n\
"));
      oputs (_("\
  -B, --ignore-backups\n\
         افعل not list implied entries ending with ~\n\
"));
      oputs (_("\
  -c\n\
         with -lt: ترتيب_ڏيو by, and show, ctime\n\
           (وقت جو آخري change جو file حالت information);\n\
         with -l: show ctime and ترتيب_ڏيو by name;\n\
         otherwise: ترتيب_ڏيو by ctime, newest پهريون\n\
"));
      oputs (_("\
  -C\n\
         list entries by columns\n\
"));
      oputs (_("\
      --color[=WHEN]\n\
         color the output WHEN; وڌيڪ info هيٺ\n\
"));
      oputs (_("\
  -d, --directory\n\
         list directories themselves, not their contents\n\
"));
      oputs (_("\
  -D, --dired\n\
         generate output designed لكل Emacs' dired گهڻائي_2\n\
"));
      oputs (_("\
  -f\n\
         same as -a -U\n\
"));
      oputs (_("\
  -F, --classify[=WHEN]\n\
         append indicator (one جو */=>@|) منزل entries WHEN\n\
"));
      oputs (_("\
      --file-type\n\
         like -F, except افعل not append '*'\n\
"));
      oputs (_("\
      --format=لفظ\n\
         across,horizontal (-x), commas (-m), ڊگهو (-l),\n\
         single-ڪالم (-1), verbose (-l), vertical (-C)\n\
"));
      oputs (_("\
      --ڀريل-وقت\n\
         like -l --وقت-style=ڀريل-iso\n\
"));
      oputs (_("\
  -g\n\
         like -l, but افعل not list owner\n\
"));
      oputs (_("\
      --گروهه-directories-پهريون\n\
         گروهه directories before files\n\
"));
      oputs (_("\
  -G, --no-گروهه\n\
         اندريون a ڊگهو listing, don't print گروهه نالا\n\
"));
      oputs (_("\
  -h, --human-readable\n\
         with -l and -s, print sizes like 1K 234M 2G etc.\n\
"));
      oputs (_("\
      --si\n\
         likewise, but use powers جو 1000 not 1024\n\
"));
      oputs (_("\
  -H, --dereference-command-سٽ\n\
         follow symbolic links listed on the command سٽ\n\
"));
      oputs (_("\
      --dereference-command-سٽ-symlink-منزل-dir\n\
         follow each command سٽ symbolic link that points منزل a directory\n\
"));
      oputs (_("\
      --hide=نمونو\n\
         افعل not list implied entries matching shell نمونو\n\
         (overridden by -a or -A)\n\
"));
      oputs (_("\
      --hyperlink[=WHEN]\n\
         hyperlink file نالا WHEN\n\
"));
      oputs (_("\
      --indicator-style=لفظ\n\
         append indicator with style لفظ منزل داخلا_2 نالا:\n\
           none (افتراضي), slash (-p), file-type (--file-type), classify (-F)\n\
"));
      oputs (_("\
  -i, --inode\n\
         print the index عدد جو each file\n\
"));
      oputs (_("\
  -I, --ignore=نمونو\n\
         افعل not list implied entries matching shell نمونو\n\
"));
      oputs (_("\
  -k, --kibibytes\n\
         افتراضي منزل 1024-byte بلاڪون لكل file system usage;\n\
         استعمال_ٿيل only with -s and في directory totals\n\
"));
      oputs (_("\
  -l\n\
         use a ڊگهو listing format\n\
"));
      oputs (_("\
  -L, --dereference\n\
         when showing file information لكل a symbolic link,\n\
         show information لكل the file the link references\n\
         rather than لكل the link itself\n\
"));
      oputs (_("\
  -m\n\
         fill ويڪر with a comma separated list جو entries\n\
"));
      oputs (_("\
  -n, --numeric-uid-gid\n\
         like -l, but list numeric user and گروهه IDs\n\
"));
      oputs (_("\
  -N, --literal\n\
         print داخلا_2 نالا without quoting\n\
"));
      oputs (_("\
  -o\n\
         like -l, but افعل not list گروهه information\n\
"));
      oputs (_("\
  -p, --indicator-style=slash\n\
         append / indicator منزل directories\n\
"));
      oputs (_("\
  -q, --hide-control-chars\n\
         print ? instead جو nongraphic characters\n\
"));
      oputs (_("\
      --show-control-chars\n\
         show nongraphic characters as-is;\n\
         the افتراضي, unless پروگرام is 'ls' and output is a terminal\n\
"));
      oputs (_("\
  -Q, --quote-name\n\
         enclose داخلا_2 نالا اندريون مزدوج quotes\n\
"));
      oputs (_("\
      --quoting-style=لفظ\n\
         use quoting style لفظ لكل داخلا_2 نالا:\n\
           literal, locale, shell, shell-always,\n\
           shell-escape, shell-escape-always, c, escape\n\
         (overrides QUOTING_STYLE environment variable)\n\
"));
      oputs (_("\
  -r, --ابتو\n\
         ابتو ترتيب طالما ترتيب_ڏيڻ\n\
"));
      oputs (_("\
  -R, --recursive\n\
         list subdirectories recursively\n\
"));
      oputs (_("\
  -s, --سائيز\n\
         print the allocated سائيز جو each file, اندريون بلاڪون\n\
"));
      oputs (_("\
  -S\n\
         ترتيب_ڏيو by file سائيز, سڀ_کان_وڏو پهريون\n\
"));
      oputs (_("\
      --ترتيب_ڏيو=لفظ\n\
         change افتراضي 'name' ترتيب_ڏيو منزل لفظ:\n\
           none (-U), سائيز (-S), وقت (-t),\n\
           نسخو_3 (-v), extension (-X), name, ويڪر\n\
"));
      oputs (_("\
      --وقت=لفظ\n\
         select which timestamp استعمال_ٿيل منزل display or ترتيب_ڏيو;\n\
           access وقت (-u): atime, access, use;\n\
           metadata change وقت (-c): ctime, حالت;\n\
           modified وقت (افتراضي): mtime, modification;\n\
           birth وقت: birth, creation;\n\
         with -l, لفظ determines which وقت منزل show;\n\
         with --ترتيب_ڏيو=وقت, ترتيب_ڏيو by لفظ (newest پهريون)\n\
"));
      oputs (_("\
      --وقت-style=وقت_STYLE\n\
         وقت/date format with -l; see وقت_STYLE هيٺ\n\
"));
      oputs (_("\
  -t\n\
         ترتيب_ڏيو by وقت, newest پهريون; see --وقت\n\
"));
      oputs (_("\
  -T, --tabsize=کانن_ڳڻپ\n\
         assume tab stops at each کانن_ڳڻپ instead جو 8\n\
"));
      oputs (_("\
  -u\n\
         with -lt: ترتيب_ڏيو by, and show, access وقت;\n\
         with -l: show access وقت and ترتيب_ڏيو by name;\n\
         otherwise: ترتيب_ڏيو by access وقت, newest پهريون\n\
"));
      oputs (_("\
  -U\n\
         افعل not ترتيب_ڏيو directory entries\n\
"));
      oputs (_("\
  -v\n\
         natural ترتيب_ڏيو جو (نسخو_3) numbers within متن\n\
"));
      oputs (_("\
  -w, --ويڪر=کانن_ڳڻپ\n\
         سيٽ_2 output ويڪر منزل کانن_ڳڻپ.  0 means no حد_بندي\n\
"));
      oputs (_("\
  -x\n\
         list entries by سٽون instead جو by columns\n\
"));
      oputs (_("\
  -X\n\
         ترتيب_ڏيو alphabetically by داخلا_2 extension\n\
"));
      oputs (_("\
  -Z, --context\n\
         print any security context جو each file\n\
"));
      oputs (_("\
      --zero\n\
         پڇاڙي each output سٽ with NUL, not newline\n\
"));
      oputs (_("\
  -1\n\
         list one file في سٽ\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخو_OPTION_DESCRIPTION);
      emit_سائيز_note ();
      fputs (_("\
\n\
The وقت_STYLE دليل can be ڀريل-iso, ڊگهو-iso, iso, locale, or +FORMAT.\n\
FORMAT is interpreted like اندريون date(1).  If FORMAT is FORMAT1<newline>FORMAT2,\n\
then FORMAT1 applies منزل non-recent files and FORMAT2 منزل recent files.\n\
وقت_STYLE prefixed with 'posix-' takes effect only outside the POSIX locale.\n\
Also the وقت_STYLE environment variable sets the افتراضي style منزل use.\n\
"), stdout);
      fputs (_("\
\n\
The WHEN دليل defaults منزل 'always' and can also be 'auto' or 'never'.\n\
"), stdout);
      fputs (_("\
\n\
Using color منزل distinguish file types is disabled both by افتراضي and\n\
with --color=never.  With --color=خودڪار, ls emits color codes only when\n\
standard output is connected منزل a terminal.  The LS_COLORS environment\n\
variable can change the settings.  Use the dircolors(1) command منزل سيٽ_2 it.\n\
"), stdout);
      fputs (_("\
\n\
Exit حالت:\n\
 0  إذا OK,\n\
 1  إذا minor problems (e.g., cannot access subdirectory),\n\
 2  إذا serious trouble (e.g., cannot access command-سٽ دليل).\n\
"), stdout);
      emit_ancillary_info (پروگرام_NAME);
    }
  ٻاهر_نڪرو (حالت);
}
