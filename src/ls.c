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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <sys/types.h>

#અન્તર્ભાવ <termios.h>
#જો HAVE_STROPTS_H
# include <stropts.h>
#endif
#અન્તર્ભાવ <sys/ioctl.h>

#ifdef WINSIZE_IN_PTEM
# include <sys/stream.h>
# include <sys/ptem.h>
#endif

#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <setjmp.h>
#અન્તર્ભાવ <pwd.h>
#અન્તર્ભાવ <getopt.h>
#અન્તર્ભાવ <signal.h>

#જો HAVE_LANGINFO_CODESET
# include <langinfo.h>
#endif

/* NonStop circa 2011 lacks both SA_RESTART and siginterrupt, so don't
   restart syscalls after a signal handler fires.  This may cause
   colors to get messed up on the screen if 'ls' is interrupted, but
   that's the best we can do on such a platform.  */
#ifndef SA_RESTART
# define SA_RESTART 0
#endif

#અન્તર્ભાવ <fnmatch.h>

/* Gnulib includes.  */
#અન્તર્ભાવ "acl.h"
#અન્તર્ભાવ "areadlink.h"
#અન્તર્ભાવ "argmatch.h"
#અન્તર્ભાવ "assure.h"
#અન્તર્ભાવ "c-ctype.h"
#અન્તર્ભાવ "c-strcase.h"
#અન્તર્ભાવ "canonicalize.h"
#અન્તર્ભાવ "dev-ino.h"
#અન્તર્ભાવ "filemode.h"
#અન્તર્ભાવ "filenamecat.h"
#અન્તર્ભાવ "filevercmp.h"
#અન્તર્ભાવ "hard-locale.h"
#અન્તર્ભાવ "hash.h"
#અન્તર્ભાવ "human.h"
#અન્તર્ભાવ "idcache.h"
#અન્તર્ભાવ "mbswidth.h"
#અન્તર્ભાવ "mpsort.h"
#અન્તર્ભાવ "obstack.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "stat-size.h"
#અન્તર્ભાવ "stat-time.h"
#અન્તર્ભાવ "strftime.h"
#અન્તર્ભાવ "xdectoint.h"
#અન્તર્ભાવ "xgethostname.h"
#અન્તર્ભાવ "xmemdup0.h"
#અન્તર્ભાવ "xstrtol-error.h"
#અન્તર્ભાવ "xstrtol.h"

#અન્તર્ભાવ "system.h"

#અન્તર્ભાવ "dircolors.h"
#અન્તર્ભાવ "ls.h"
#અન્તર્ભાવ "statx.h"
#અન્તર્ભાવ "term-sig.h"

/* Include <sys/capability.h> last to avoid a clash of <sys/types.h>
   include guards with some premature versions of libcap.
   For more details, see <https://bugzilla.redhat.com/483548>.  */
#ifdef HAVE_CAP
# include <sys/capability.h>
#endif

#જો HAVE_LINUX_XATTR_H
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
#define obstack_chunk_free મુક્ત

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

#જો READDIR_LIES_ABOUT_MOUNTPOINT_D_INO
# define RELIABLE_D_INO(dp) NOT_AN_INODE_સંખ્યા
#નહીંતર
# define RELIABLE_D_INO(dp) D_INO (dp)
#endif

#જો ! HAVE_STRUCT_STAT_ST_AUTHOR
# define st_author st_uid
#endif

ગણના filetype
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
ગણના { filetype_cardinality = arg_directory + 1 };

/* Display letters and indicators for each filetype.
   Keep these in sync with enum filetype.  */
સ્થિર અક્ષર અચલ filetype_letter[] =
  {'?', 'p', 'c', 'd', 'b', '-', 'l', 's', 'w', 'd'};
static_assert (countof (filetype_letter) == filetype_cardinality);

/* Map enum filetype to <dirent.h> d_type values.  */
સ્થિર unsigned અક્ષર અચલ filetype_d_type[] =
  {
    DT_UNKNOWN, DT_FIFO, DT_CHR, DT_DIR, DT_BLK, DT_REG, DT_LNK, DT_SOCK,
    DT_WHT, DT_DIR
  };
static_assert (countof (filetype_d_type) == filetype_cardinality);

/* Map d_type values to enum filetype.  */
સ્થિર અક્ષર અચલ d_type_filetype[UCHAR_MAX + 1] =
  {
    [DT_BLK] = blockdev, [DT_CHR] = chardev, [DT_DIR] = directory,
    [DT_FIFO] = fifo, [DT_LNK] = symbolic_link, [DT_REG] = normal,
    [DT_SOCK] = sock, [DT_WHT] = whiteout
  };

ગણના acl_type
  {
    ACL_T_NONE,
    ACL_T_UNKNOWN,
    ACL_T_LSM_CONTEXT_ONLY,
    ACL_T_YES
  };

સંરચના fileinfo
  {
    /* The file name.  */
    અક્ષર *name;

    /* For symbolic link, name of the file linked to, otherwise zero.  */
    અક્ષર *linkname;

    /* For terminal hyperlinks. */
    અક્ષર *absolute_name;

    સંરચના stat stat;

    ગણના filetype filetype;

    /* For symbolic link and long listing, st_mode of file linked to, otherwise
       zero.  */
    mode_t linkmode;

    /* security context.  */
    અક્ષર *scontext;

    bool stat_ok;

    /* For symbolic link and color printing, true if linked-to file
       exists, otherwise false.  */
    bool linkok;

    /* For long listings, true if the file has an access control list,
       or a security context.  */
    ગણના acl_type acl_type;

    /* For color listings, true if a regular file has capability info.  */
    bool has_capability;

    /* Whether file name needs quoting. tri-state with -1 == unknown.  */
    પૂર્ણાંક quoted;

    /* Cached screen width (including quoting).  */
    size_t width;
  };

/* Null is a valid character in a color indicator (think about Epson
   printers, for example) so we have to use a length/buffer string
   type.  */

સંરચના bin_str
  {
    size_t len;			/* Number of bytes */
    અક્ષર અચલ *string;		/* Pointer to the same */
  };

#જો ! HAVE_TCGETPGRP
# define tcgetpgrp(Fd) 0
#endif

સ્થિર size_t quote_name (અક્ષર અચલ *name,
                          સંરચના quoting_options અચલ *options,
                          પૂર્ણાંક needs_general_quoting,
                          અચલ સંરચના bin_str *color,
                          bool allow_pad, સંરચના obstack *ઢગલો,
                          અક્ષર અચલ *absolute_name);
સ્થિર size_t quote_name_buf (અક્ષર **inbuf, size_t bufsize, અક્ષર *name,
                              સંરચના quoting_options અચલ *options,
                              પૂર્ણાંક needs_general_quoting, size_t *width,
                              bool *pad);
સ્થિર પૂર્ણાંક decode_switches (પૂર્ણાંક argc, અક્ષર **argv);
સ્થિર bool file_ignored (અક્ષર અચલ *name);
સ્થિર uintmax_t gobble_file (અક્ષર અચલ *name, ગણના filetype type,
                              ino_t inode, bool command_લીટી_arg,
                              અક્ષર અચલ *dirname);
સ્થિર અચલ સંરચના bin_str * get_color_indicator (અચલ સંરચના fileinfo *f,
                                                   bool symlink_target);
સ્થિર bool print_color_indicator (અચલ સંરચના bin_str *ind);
સ્થિર શૂન્ય put_indicator (અચલ સંરચના bin_str *ind);
સ્થિર શૂન્ય add_ignore_pattern (અક્ષર અચલ *pattern);
સ્થિર શૂન્ય attach (અક્ષર *dest, અક્ષર અચલ *dirname, અક્ષર અચલ *name);
સ્થિર શૂન્ય clear_files (શૂન્ય);
સ્થિર શૂન્ય extract_dirs_from_files (અક્ષર અચલ *dirname,
                                     bool command_લીટી_arg);
સ્થિર bool get_link_name (અક્ષર અચલ *filename, સંરચના fileinfo *f,
                           bool command_લીટી_arg);
સ્થિર શૂન્ય indent (size_t from, size_t to);
સ્થિર idx_t calculate_columns (bool by_columns);
સ્થિર શૂન્ય print_current_files (શૂન્ય);
સ્થિર શૂન્ય print_dir (અક્ષર અચલ *name, અક્ષર અચલ *realname,
                       bool command_લીટી_arg);
સ્થિર size_t print_file_name_and_frills (અચલ સંરચના fileinfo *f,
                                          size_t start_col);
સ્થિર શૂન્ય print_horizontal (શૂન્ય);
સ્થિર પૂર્ણાંક format_user_width (uid_t u);
સ્થિર પૂર્ણાંક format_group_width (gid_t g);
સ્થિર શૂન્ય print_long_format (અચલ સંરચના fileinfo *f);
સ્થિર શૂન્ય print_many_per_લીટી (શૂન્ય);
સ્થિર size_t print_name_with_quoting (અચલ સંરચના fileinfo *f,
                                       bool symlink_target,
                                       સંરચના obstack *ઢગલો,
                                       size_t start_col);
સ્થિર શૂન્ય prep_non_filename_text (શૂન્ય);
સ્થિર bool print_type_indicator (bool stat_ok, mode_t mode,
                                  ગણના filetype type);
સ્થિર શૂન્ય print_with_separator (અક્ષર sep);
સ્થિર શૂન્ય queue_directory (અક્ષર અચલ *name, અક્ષર અચલ *realname,
                             bool command_લીટી_arg);
સ્થિર શૂન્ય sort_files (શૂન્ય);
સ્થિર શૂન્ય parse_ls_color (શૂન્ય);

સ્થિર size_t quote_name_width (અક્ષર અચલ *name,
                                સંરચના quoting_options અચલ *options,
                                પૂર્ણાંક needs_general_quoting);

/* Initial size of hash table.
   Most hierarchies are likely to be shallower than this.  */
ગણના { INITIAL_TABLE_SIZE = 30 };

/* The set of 'active' directories, from the current command-line argument
   to the level in the hierarchy at which files are being listed.
   A directory is represented by its device and inode numbers (struct dev_ino).
   A directory is added to this set when ls begins listing it or its
   entries, and it is removed from the set just after ls has finished
   processing it.  This set is used solely to detect loops, e.g., with
   mkdir loop; cd loop; ln -s ../loop sub; ls -RL  */
સ્થિર Hash_table *active_dir_set;

#define LOOP_DETECT (!!active_dir_set)

/* The table of files in the current directory:

   'cwd_file' points to a vector of 'struct fileinfo', one per file.
   'cwd_n_alloc' is the number of elements space has been allocated for.
   'cwd_n_used' is the number actually in use.  */

/* Address of block containing the files that are described.  */
સ્થિર સંરચના fileinfo *cwd_file;

/* Length of block that 'cwd_file' points to, measured in files.  */
સ્થિર idx_t cwd_n_alloc;

/* Index of first unused slot in 'cwd_file'.  */
સ્થિર idx_t cwd_n_used;

/* Whether files needs may need padding due to quoting.  */
સ્થિર bool cwd_some_quoted;

/* Whether quoting style _may_ add outer quotes,
   and whether aligning those is useful.  */
સ્થિર bool align_variable_outer_quotes;

/* Vector of pointers to files, in proper sorted order, and the number
   of entries allocated for it.  */
સ્થિર શૂન્ય **sorted_file;
સ્થિર size_t sorted_file_alloc;

/* When true, in a color listing, color each symlink name according to the
   type of file it points to.  Otherwise, color them according to the 'ln'
   directive in LS_COLORS.  Dangling (orphan) symlinks are treated specially,
   regardless.  This is set when 'ln=target' appears in LS_COLORS.  */

સ્થિર bool color_symlink_as_referent;

સ્થિર અક્ષર અચલ *hostname;

/* Mode of appropriate file for coloring.  */
સ્થિર mode_t
file_or_link_mode (સંરચના fileinfo અચલ *file)
{
  પાછા (color_symlink_as_referent && file->linkok
          ? file->linkmode : file->stat.st_mode);
}


/* Record of one pending directory waiting to be listed.  */

સંરચના pending
  {
    અક્ષર *name;
    /* If the directory is actually the file pointed to by a symbolic link we
       were told to list, 'realname' will contain the name of the symbolic
       link, otherwise zero.  */
    અક્ષર *realname;
    bool command_લીટી_arg;
    સંરચના pending *next;
  };

સ્થિર સંરચના pending *pending_dirs;

/* Current time in seconds and nanoseconds since 1970, updated as
   needed when deciding whether a file is recent.  */

સ્થિર સંરચના timespec current_time;

સ્થિર bool print_scontext;
સ્થિર અક્ષર UNKNOWN_SECURITY_CONTEXT[] = "?";

/* Whether any of the files has an ACL.  This affects the width of the
   mode column.  */

સ્થિર bool any_has_acl;

/* The number of columns to use for columns containing inode numbers,
   block sizes, link counts, owners, groups, authors, major device
   numbers, minor device numbers, and file sizes, respectively.  */

સ્થિર પૂર્ણાંક inode_સંખ્યા_width;
સ્થિર પૂર્ણાંક block_size_width;
સ્થિર પૂર્ણાંક nlink_width;
સ્થિર પૂર્ણાંક scontext_width;
સ્થિર પૂર્ણાંક owner_width;
સ્થિર પૂર્ણાંક group_width;
સ્થિર પૂર્ણાંક author_width;
સ્થિર પૂર્ણાંક major_device_સંખ્યા_width;
સ્થિર પૂર્ણાંક minor_device_સંખ્યા_width;
સ્થિર પૂર્ણાંક file_size_width;

/* Option flags */

/* long_format for lots of info, one per line.
   one_per_line for just names, one per line.
   many_per_line for just names, many per line, sorted vertically.
   horizontal for just names, many per line, sorted horizontally.
   with_commas for just names, many per line, separated by commas.

   -l (and other options that imply -l), -1, -C, -x and -m control
   this parameter.  */

ગણના format
  {
    long_format,		/* -l and other options that imply -l */
    one_per_લીટી,		/* -1 */
    many_per_લીટી,		/* -C */
    horizontal,			/* -x */
    with_commas			/* -m */
  };

સ્થિર ગણના format format;

/* 'full-iso' uses full ISO-style dates and times.  'long-iso' uses longer
   ISO-style timestamps, though shorter than 'full-iso'.  'iso' uses shorter
   ISO-style timestamps.  'locale' uses locale-dependent timestamps.  */
ગણના time_style
  {
    full_iso_time_style,	/* --time-style=full-iso */
    long_iso_time_style,	/* --time-style=long-iso */
    iso_time_style,		/* --time-style=iso */
    locale_time_style		/* --time-style=locale */
  };

સ્થિર અક્ષર અચલ *અચલ time_style_args[] =
{
  "full-iso", "long-iso", "iso", "locale", ખાલી
};
સ્થિર ગણના time_style અચલ time_style_types[] =
{
  full_iso_time_style, long_iso_time_style, iso_time_style,
  locale_time_style
};
ARGMATCH_VERIFY (time_style_args, time_style_types);

/* Type of time to print or sort by.  Controlled by -c and -u.
   The values of each item of this enum are important since they are
   used as indices in the sort functions array (see sort_files()).  */

ગણના time_type
  {
    time_mtime = 0,		/* default */
    time_ctime,			/* -c */
    time_atime,			/* -u */
    time_btime,                 /* birth time */
    time_numtypes		/* the number of elements of this enum */
  };

સ્થિર ગણના time_type time_type;
સ્થિર bool explicit_time;

/* The file characteristic to sort by.  Controlled by -t, -S, -U, -X, -v.
   The values of each item of this enum are important since they are
   used as indices in the sort functions array (see sort_files()).  */

ગણના sort_type
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

સ્થિર ગણના sort_type sort_type;

/* Direction of sort.
   false means highest first if numeric,
   lowest first if alphabetic;
   these are the defaults.
   true means the opposite order in each case.  -r  */

સ્થિર bool sort_reverse;

/* True means to display owner information.  -g turns this off.  */

સ્થિર bool print_owner = true;

/* True means to display author information.  */

સ્થિર bool print_author;

/* True means to display group information.  -G and -o turn this off.  */

સ્થિર bool print_group = true;

/* True means print the user and group id's as numbers rather
   than as names.  -n  */

સ્થિર bool numeric_ids;

/* True means mention the size in blocks of each file.  -s  */

સ્થિર bool print_block_size;

/* Human-readable options for output, when printing block counts.  */
સ્થિર પૂર્ણાંક human_output_opts;

/* The units to use when printing block counts.  */
સ્થિર uintmax_t output_block_size;

/* Likewise, but for file sizes.  */
સ્થિર પૂર્ણાંક file_human_output_opts;
સ્થિર uintmax_t file_output_block_size = 1;

/* Follow the output with a special string.  Using this format,
   Emacs' dired mode starts up twice as fast, and can handle all
   strange characters in file names.  */
સ્થિર bool dired;

/* 'none' means don't mention the type of files.
   'slash' means mention directories only, with a '/'.
   'file_type' means mention file types.
   'classify' means mention file types and mark executables.

   Controlled by -F, -p, and --indicator-style.  */

ગણના indicator_style
  {
    none = 0,	/*     --indicator-style=none (default) */
    slash,	/* -p, --indicator-style=slash */
    file_type,	/*     --indicator-style=file-type */
    classify	/* -F, --indicator-style=classify */
  };

સ્થિર ગણના indicator_style indicator_style;

/* Names of indicator styles.  */
સ્થિર અક્ષર અચલ *અચલ indicator_style_args[] =
{
  "none", "slash", "file-type", "classify", ખાલી
};
સ્થિર ગણના indicator_style અચલ indicator_style_types[] =
{
  none, slash, file_type, classify
};
ARGMATCH_VERIFY (indicator_style_args, indicator_style_types);

/* True means use colors to mark types.  Also define the different
   colors as well as the stuff for the LS_COLORS environment variable.
   The LS_COLORS variable is now in a termcap-like format.  */

સ્થિર bool print_with_color;

સ્થિર bool print_hyperlink;

/* Whether we used any colors in the output so far.  If so, we will
   need to restore the default color later.  If not, we will need to
   call prep_non_filename_text before using color for the first time. */

સ્થિર bool used_color = false;

ગણના when_type
  {
    when_never,		/* 0: default or --color=never */
    when_always,	/* 1: --color=always */
    when_if_tty		/* 2: --color=tty */
  };

ગણના Dereference_symlink
  {
    DEREF_UNDEFINED = 0,		/* default */
    DEREF_NEVER,
    DEREF_COMMAND_લીટી_ARGUMENTS,	/* -H */
    DEREF_COMMAND_લીટી_SYMLINK_TO_DIR,	/* the default, in certain cases */
    DEREF_ALWAYS			/* -L */
  };

ગણના indicator_no
  {
    C_ડાબું, C_જમણું, C_END, C_RESET, C_NORM, C_FILE, C_DIR, C_LINK,
    C_FIFO, C_SOCK,
    C_BLK, C_CHR, C_MISSING, C_ORPHAN, C_EXEC, C_DOOR, C_SETUID, C_SETGID,
    C_STICKY, C_OTHER_WRITABLE, C_STICKY_OTHER_WRITABLE, C_CAP, C_MULTIHARDLINK,
    C_CLR_TO_EOL
  };

સ્થિર અક્ષર અચલ indicator_name[][2]=
  {
    {'l','c'}, {'r','c'}, {'e','c'}, {'r','s'}, {'n','o'},
    {'f','i'}, {'d','i'}, {'l','n'},
    {'p','i'}, {'s','o'},
    {'b','d'}, {'c','d'}, {'m','i'}, {'o','r'}, {'e','x'},
    {'d','o'}, {'s','u'}, {'s','g'},
    {'s','t'}, {'o','w'}, {'t','w'}, {'c','a'}, {'m','h'},
    {'c','l'}
  };

સંરચના color_ext_type
  {
    સંરચના bin_str ext;		/* The extension we're looking for */
    સંરચના bin_str seq;		/* The sequence to output when we do */
    bool   exact_match;		/* Whether to compare case insensitively */
    સંરચના color_ext_type *next;	/* Next in list */
  };

સ્થિર સંરચના bin_str color_indicator[] =
  {
    { 2, (અક્ષર અચલ []) {'\033','['} },/* lc: Left of color sequence */
    { 1, (અક્ષર અચલ []) {'m'} },	/* rc: Right of color sequence */
    { 0, ખાલી },			/* ec: End color (replaces lc+rs+rc) */
    { 1, (અક્ષર અચલ []) {'0'} },	/* rs: Reset to ordinary colors */
    { 0, ખાલી },			/* no: Normal */
    { 0, ખાલી },			/* fi: File: default */
    { 5, ((અક્ષર અચલ [])
          {'0','1',';','3','4'}) },	/* di: Directory: bright blue */
    { 5, ((અક્ષર અચલ [])
          {'0','1',';','3','6'}) },	/* ln: Symlink: bright cyan */
    { 2, (અક્ષર અચલ []) {'3','3'} },		/* pi: Pipe: yellow/brown */
    { 5, ((અક્ષર અચલ [])
          {'0','1',';','3','5'}) },	/* so: Socket: bright magenta */
    { 5, ((અક્ષર અચલ [])
          {'0','1',';','3','3'}) },	/* bd: Block device: bright yellow */
    { 5, ((અક્ષર અચલ [])
          {'0','1',';','3','3'}) },	/* cd: Char device: bright yellow */
    { 0, ખાલી },			/* mi: Missing file: undefined */
    { 0, ખાલી },			/* or: Orphaned symlink: undefined */
    { 5, ((અક્ષર અચલ [])
          {'0','1',';','3','2'}) },	/* ex: Executable: bright green */
    { 5, ((અક્ષર અચલ [])
          {'0','1',';','3','5'}) },	/* do: Door: bright magenta */
    { 5, ((અક્ષર અચલ [])
          {'3','7',';','4','1'}) },	/* su: setuid: white on red */
    { 5, ((અક્ષર અચલ [])
          {'3','0',';','4','3'}) },	/* sg: setgid: black on yellow */
    { 5, ((અક્ષર અચલ [])
          {'3','7',';','4','4'}) },	/* st: sticky: black on blue */
    { 5, ((અક્ષર અચલ [])
          {'3','4',';','4','2'}) },	/* ow: other-writable: blue on green */
    { 5, ((અક્ષર અચલ [])
          {'3','0',';','4','2'}) },	/* tw: ow w/ sticky: black on green */
    { 0, ખાલી },			/* ca: disabled by default */
    { 0, ખાલી },			/* mh: disabled by default */
    { 3, ((અક્ષર અચલ [])
          {'\033','[','K'}) },		/* cl: clear to end of line */
  };

/* A list mapping file extensions to corresponding display sequence.  */
સ્થિર સંરચના color_ext_type *color_ext_list = ખાલી;

/* Buffer for color sequences */
સ્થિર અક્ષર *color_buf;

/* True means to check for orphaned symbolic link, for displaying
   colors, or to group symlink to directories with other dirs.  */

સ્થિર bool check_symlink_mode;

/* True means mention the inode number of each file.  -i  */

સ્થિર bool print_inode;

/* What to do with symbolic links.  Affected by -d, -F, -H, -l (and
   other options that imply -l), and -L.  */

સ્થિર ગણના Dereference_symlink dereference;

/* True means when a directory is found, display info on its
   contents.  -R  */

સ્થિર bool recursive;

/* True means when an argument is a directory name, display info
   on it itself.  -d  */

સ્થિર bool immediate_dirs;

/* True means that directories are grouped before files. */

સ્થિર bool directories_first;

/* Which files to ignore.  */

સ્થિર ગણના
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

સંરચના ignore_pattern
  {
    અક્ષર અચલ *pattern;
    સંરચના ignore_pattern *next;
  };

સ્થિર સંરચના ignore_pattern *ignore_patterns;

/* Similar to IGNORE_PATTERNS, except that -a or -A causes this
   variable itself to be ignored.  */
સ્થિર સંરચના ignore_pattern *hide_patterns;

/* True means output nongraphic chars in file names as '?'.
   (-q, --hide-control-chars)
   qmark_funny_chars and the quoting style (-Q, --quoting-style=WORD) are
   independent.  The algorithm is: first, obey the quoting style to get a
   string representing the file name;  then, if qmark_funny_chars is set,
   replace all nonprintable chars in that string with '?'.  It's necessary
   to replace nonprintable chars even in quoted strings, because we don't
   want to mess up the terminal if control chars get sent to it, and some
   quoting methods pass through control chars as-is.  */
સ્થિર bool qmark_funny_chars;

/* True means extra replacement of newline with '?'
   in cases where a newline in a file name would be
   ambiguous with newline separators.  */
સ્થિર bool qmark_newline;

/* Quoting options for file and dir name output.  */

સ્થિર સંરચના quoting_options *filename_quoting_options;
સ્થિર સંરચના quoting_options *dirname_quoting_options;

/* The number of chars per hardware tab stop.  Setting this to zero
   inhibits the use of TAB characters for separating columns.  -T */
સ્થિર size_t tabsize;

/* True means print each directory name before listing it.  */

સ્થિર bool print_dir_name;

/* The line length to use for breaking lines in many-per-line format.
   Can be set with -w.  If zero, there is no limit.  */

સ્થિર size_t લીટી_length;

/* The local time zone rules, as per the TZ environment variable.  */

સ્થિર timezone_t localtz;

/* If true, the file listing format requires that stat be called on
   each file.  */

સ્થિર bool format_needs_stat;

/* Similar to 'format_needs_stat', but set if only the file type is
   needed.  */

સ્થિર bool format_needs_type;

/* Like 'format_needs_stat', but set only if capability colors are needed.  */

સ્થિર bool format_needs_capability;

/* An arbitrary limit on the number of bytes in a printed timestamp.
   This is set to a relatively small value to avoid the need to worry
   about denial-of-service attacks on servers that run "ls" on behalf
   of remote clients.  1000 bytes should be enough for any practical
   timestamp format.  */

ગણના { TIME_STAMP_LEN_MAXIMUM = MAX (1000, INT_STRLEN_BOUND (time_t)) };

/* strftime formats for non-recent and recent files, respectively, in
   -l output.  */

સ્થિર અક્ષર અચલ *long_time_format[2] =
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

સ્થિર sigset_t caught_signals;

/* If nonzero, the value of the pending fatal signal.  */

સ્થિર sig_atomic_t અસ્થિર interrupt_signal;

/* A count of the number of pending stop signals that have been received.  */

સ્થિર sig_atomic_t અસ્થિર stop_signal_ગણતરી;

/* Desired exit status.  */

સ્થિર પૂર્ણાંક exit_status;

/* Exit statuses.  */
ગણના
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
ગણના
{
  AUTHOR_OPTION = CHAR_MAX + 1,
  BLOCK_SIZE_OPTION,
  COLOR_OPTION,
  DEREFERENCE_COMMAND_લીટી_SYMLINK_TO_DIR_OPTION,
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

સ્થિર સંરચના option અચલ long_options[] =
{
  {"all", no_argument, ખાલી, 'a'},
  {"escape", no_argument, ખાલી, 'b'},
  {"directory", no_argument, ખાલી, 'd'},
  {"dired", no_argument, ખાલી, 'D'},
  {"full-time", no_argument, ખાલી, FULL_TIME_OPTION},
  {"group-directories-first", no_argument, ખાલી,
   GROUP_DIRECTORIES_FIRST_OPTION},
  {"human-readable", no_argument, ખાલી, 'h'},
  {"inode", no_argument, ખાલી, 'i'},
  {"kibibytes", no_argument, ખાલી, 'k'},
  {"numeric-uid-gid", no_argument, ખાલી, 'n'},
  {"no-group", no_argument, ખાલી, 'G'},
  {"hide-control-chars", no_argument, ખાલી, 'q'},
  {"reverse", no_argument, ખાલી, 'r'},
  {"size", no_argument, ખાલી, 's'},
  {"width", required_argument, ખાલી, 'w'},
  {"almost-all", no_argument, ખાલી, 'A'},
  {"ignore-backups", no_argument, ખાલી, 'B'},
  {"classify", optional_argument, ખાલી, 'F'},
  {"file-type", no_argument, ખાલી, FILE_TYPE_INDICATOR_OPTION},
  {"si", no_argument, ખાલી, SI_OPTION},
  {"dereference-command-line", no_argument, ખાલી, 'H'},
  {"dereference-command-line-symlink-to-dir", no_argument, ખાલી,
   DEREFERENCE_COMMAND_લીટી_SYMLINK_TO_DIR_OPTION},
  {"hide", required_argument, ખાલી, HIDE_OPTION},
  {"ignore", required_argument, ખાલી, 'I'},
  {"indicator-style", required_argument, ખાલી, INDICATOR_STYLE_OPTION},
  {"dereference", no_argument, ખાલી, 'L'},
  {"literal", no_argument, ખાલી, 'N'},
  {"quote-name", no_argument, ખાલી, 'Q'},
  {"quoting-style", required_argument, ખાલી, QUOTING_STYLE_OPTION},
  {"recursive", no_argument, ખાલી, 'R'},
  {"format", required_argument, ખાલી, FORMAT_OPTION},
  {"show-control-chars", no_argument, ખાલી, SHOW_CONTROL_CHARS_OPTION},
  {"sort", required_argument, ખાલી, SORT_OPTION},
  {"tabsize", required_argument, ખાલી, 'T'},
  {"time", required_argument, ખાલી, TIME_OPTION},
  {"time-style", required_argument, ખાલી, TIME_STYLE_OPTION},
  {"zero", no_argument, ખાલી, ZERO_OPTION},
  {"color", optional_argument, ખાલી, COLOR_OPTION},
  {"hyperlink", optional_argument, ખાલી, HYPERLINK_OPTION},
  {"block-size", required_argument, ખાલી, BLOCK_SIZE_OPTION},
  {"context", no_argument, ખાલી, 'Z'},
  {"author", no_argument, ખાલી, AUTHOR_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ખાલી, 0, ખાલી, 0}
};

સ્થિર અક્ષર અચલ *અચલ format_args[] =
{
  "verbose", "long", "commas", "horizontal", "across",
  "vertical", "single-column", ખાલી
};
સ્થિર ગણના format અચલ format_types[] =
{
  long_format, long_format, with_commas, horizontal, horizontal,
  many_per_લીટી, one_per_લીટી
};
ARGMATCH_VERIFY (format_args, format_types);

સ્થિર અક્ષર અચલ *અચલ sort_args[] =
{
  "none", "size", "time", "version", "extension",
  "name", "width", ખાલી
};
સ્થિર ગણના sort_type અચલ sort_types[] =
{
  sort_none, sort_size, sort_time, sort_version, sort_extension,
  sort_name, sort_width
};
ARGMATCH_VERIFY (sort_args, sort_types);

સ્થિર અક્ષર અચલ *અચલ time_args[] =
{
  "atime", "access", "use",
  "ctime", "status",
  "mtime", "modification",
  "birth", "creation",
  ખાલી
};
સ્થિર ગણના time_type અચલ time_types[] =
{
  time_atime, time_atime, time_atime,
  time_ctime, time_ctime,
  time_mtime, time_mtime,
  time_btime, time_btime,
};
ARGMATCH_VERIFY (time_args, time_types);

સ્થિર અક્ષર અચલ *અચલ when_args[] =
{
  /* force and none are for compatibility with another color-ls version */
  "always", "yes", "force",
  "never", "no", "none",
  "auto", "tty", "if-tty", ખાલી
};
સ્થિર ગણના when_type અચલ when_types[] =
{
  when_always, when_always, when_always,
  when_never, when_never, when_never,
  when_if_tty, when_if_tty, when_if_tty
};
ARGMATCH_VERIFY (when_args, when_types);

/* Information about filling a column.  */
સંરચના column_info
{
  bool valid_len;
  size_t લીટી_len;
  size_t *col_arr;
};

/* Array with information about column fullness.  */
સ્થિર સંરચના column_info *column_info;

/* Maximum number of columns ever possible for this display.  */
સ્થિર size_t max_idx;

/* The minimum width of a non-final column is 3: 1 character for the name
   and 2 for the separating white space.  The final column is not padded.  */
ગણના { MIN_COLUMN_WIDTH = 3 };


/* This zero-based index is for the --dired option.  It is incremented
   for each byte of output generated by this program so that the beginning
   and ending indices (in that output) of every file name can be recorded
   and later output themselves.  */
સ્થિર off_t dired_pos;

સ્થિર શૂન્ય
dired_outbyte (અક્ષર c)
{
  dired_pos++;
  putchar (c);
}

/* Output the buffer S, of length S_LEN, and increment DIRED_POS by S_LEN.  */
સ્થિર શૂન્ય
dired_outbuf (અક્ષર અચલ *s, size_t s_len)
{
  dired_pos += s_len;
  fwrite (s, કદ *s, s_len, stdout);
}

/* Output the string S, and increment DIRED_POS by its length.  */
સ્થિર શૂન્ય
dired_outstring (અક્ષર અચલ *s)
{
  dired_outbuf (s, strlen (s));
}

સ્થિર શૂન્ય
dired_indent (શૂન્ય)
{
  જો (dired)
    dired_outstring ("  ");
}

/* With --dired, store pairs of beginning and ending indices of file names.  */
સ્થિર સંરચના obstack dired_obstack;

/* With --dired, store pairs of beginning and ending indices of any
   directory names that appear as headers (just before 'total' line)
   for lists of directory entries.  Such directory names are seen when
   listing hierarchies using -R and when a directory is listed with at
   least one other command line argument.  */
સ્થિર સંરચના obstack subdired_obstack;

/* Save the current index on the specified obstack, OBS.  */
સ્થિર શૂન્ય
ધકેલો_current_dired_pos (સંરચના obstack *obs)
{
  જો (dired)
    obstack_grow (obs, &dired_pos, કદ dired_pos);
}

/* With -R, this stack is used to help detect directory cycles.
   The device/inode pairs on this stack mirror the pairs in the
   active_dir_set hash table.  */
સ્થિર સંરચના obstack dev_ino_obstack;

/* Push a pair onto the device/inode stack.  */
સ્થિર શૂન્ય
dev_ino_ધકેલો (dev_t dev, ino_t ino)
{
  શૂન્ય *vdi;
  સંરચના dev_ino *di;
  પૂર્ણાંક dev_ino_size = કદ *di;
  obstack_blank (&dev_ino_obstack, dev_ino_size);
  vdi = obstack_next_free (&dev_ino_obstack);
  di = vdi;
  di--;
  di->st_dev = dev;
  di->st_ino = ino;
}

/* Pop a dev/ino struct off the global dev_ino_obstack
   and return that struct.  */
સ્થિર સંરચના dev_ino
dev_ino_કાઢો (શૂન્ય)
{
  શૂન્ય *vdi;
  સંરચના dev_ino *di;
  પૂર્ણાંક dev_ino_size = કદ *di;
  affirm (dev_ino_size <= obstack_object_size (&dev_ino_obstack));
  obstack_blank_fast (&dev_ino_obstack, -dev_ino_size);
  vdi = obstack_next_free (&dev_ino_obstack);
  di = vdi;
  પાછા *di;
}

સ્થિર શૂન્ય
assert_matching_dev_ino (અક્ષર અચલ *name, સંરચના dev_ino di)
{
  MAYBE_UNUSED સંરચના stat sb;
  assure (0 <= stat (name, &sb));
  assure (sb.st_dev == di.st_dev);
  assure (sb.st_ino == di.st_ino);
}

સ્થિર અક્ષર eolbyte = '\n';

/* Write to standard output PREFIX, followed by the quoting style and
   a space-separated list of the integers stored in OS all on one line.  */

સ્થિર શૂન્ય
dired_dump_obstack (અક્ષર અચલ *prefix, સંરચના obstack *os)
{
  size_t n_pos;

  n_pos = obstack_object_size (os) / કદ (dired_pos);
  જો (n_pos > 0)
    {
      off_t *pos = obstack_finish (os);
      fputs (prefix, stdout);
      માટે (size_t i = 0; i < n_pos; i++)
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
સ્થિર સંરચના timespec
get_stat_btime (સંરચના stat અચલ *st)
{
  સંરચના timespec btimespec;

#જો HAVE_STATX && defined STATX_INO
  btimespec = get_stat_mtime (st);
#નહીંતર
  btimespec = get_stat_birthtime (st);
#endif

  પાછા btimespec;
}

#જો HAVE_STATX && defined STATX_INO
ATTRIBUTE_PURE
સ્થિર unsigned પૂર્ણાંક
time_type_to_statx (શૂન્ય)
{
  પસંદગી (time_type)
    {
    સ્થિતિ time_ctime:
      પાછા STATX_CTIME;
    સ્થિતિ time_mtime:
      પાછા STATX_MTIME;
    સ્થિતિ time_atime:
      પાછા STATX_ATIME;
    સ્થિતિ time_btime:
      પાછા STATX_BTIME;
    સ્થિતિ time_numtypes: મૂળભૂત:
      unreachable ();
    }
    પાછા 0;
}

ATTRIBUTE_PURE
સ્થિર unsigned પૂર્ણાંક
calc_req_mask (શૂન્ય)
{
  unsigned પૂર્ણાંક mask = STATX_MODE;

  જો (print_inode)
    mask |= STATX_INO;

  જો (print_block_size)
    mask |= STATX_BLOCKS;

  જો (format == long_format) {
    mask |= STATX_NLINK | STATX_SIZE | time_type_to_statx ();
    જો (print_owner || print_author)
      mask |= STATX_UID;
    જો (print_group)
      mask |= STATX_GID;
  }

  પસંદગી (sort_type)
    {
    સ્થિતિ sort_none:
    સ્થિતિ sort_name:
    સ્થિતિ sort_version:
    સ્થિતિ sort_extension:
    સ્થિતિ sort_width:
      તોડો;
    સ્થિતિ sort_time:
      mask |= time_type_to_statx ();
      તોડો;
    સ્થિતિ sort_size:
      mask |= STATX_SIZE;
      તોડો;
    સ્થિતિ sort_numtypes: મૂળભૂત:
      unreachable ();
    }

  પાછા mask;
}

સ્થિર પૂર્ણાંક
do_statx (પૂર્ણાંક fd, અક્ષર અચલ *name, સંરચના stat *st, પૂર્ણાંક flags,
          unsigned પૂર્ણાંક mask)
{
  સંરચના statx stx;
  bool want_btime = mask & STATX_BTIME;
  પૂર્ણાંક ret = statx (fd, name, flags | AT_NO_AUTOMOUNT, mask, &stx);
  જો (ret >= 0)
    {
      statx_to_stat (&stx, st);
      /* Since we only need one timestamp type,
         store birth time in st_mtim.  */
      જો (want_btime)
        {
          જો (stx.stx_mask & STATX_BTIME)
            st->st_mtim = statx_timestamp_to_timespec (stx.stx_btime);
          નહીંતર
            st->st_mtim.tv_sec = st->st_mtim.tv_nsec = -1;
        }
    }

  પાછા ret;
}

સ્થિર પૂર્ણાંક
do_stat (અક્ષર અચલ *name, સંરચના stat *st)
{
  પાછા do_statx (AT_FDCWD, name, st, 0, calc_req_mask ());
}

સ્થિર પૂર્ણાંક
do_lstat (અક્ષર અચલ *name, સંરચના stat *st)
{
  પાછા do_statx (AT_FDCWD, name, st, AT_SYMLINK_NOFOLLOW, calc_req_mask ());
}

સ્થિર પૂર્ણાંક
stat_for_mode (અક્ષર અચલ *name, સંરચના stat *st)
{
  પાછા do_statx (AT_FDCWD, name, st, 0, STATX_MODE);
}

/* dev+ino should be static, so no need to sync with backing store */
સ્થિર પૂર્ણાંક
stat_for_ino (અક્ષર અચલ *name, સંરચના stat *st)
{
  પાછા do_statx (AT_FDCWD, name, st, 0, STATX_INO);
}

સ્થિર પૂર્ણાંક
fstat_for_ino (પૂર્ણાંક fd, સંરચના stat *st)
{
  પાછા do_statx (fd, "", st, AT_EMPTY_PATH, STATX_INO);
}
#નહીંતર
સ્થિર પૂર્ણાંક
do_stat (અક્ષર અચલ *name, સંરચના stat *st)
{
  પાછા stat (name, st);
}

સ્થિર પૂર્ણાંક
do_lstat (અક્ષર અચલ *name, સંરચના stat *st)
{
  પાછા lstat (name, st);
}

સ્થિર પૂર્ણાંક
stat_for_mode (અક્ષર અચલ *name, સંરચના stat *st)
{
  પાછા stat (name, st);
}

સ્થિર પૂર્ણાંક
stat_for_ino (અક્ષર અચલ *name, સંરચના stat *st)
{
  પાછા stat (name, st);
}

સ્થિર પૂર્ણાંક
fstat_for_ino (પૂર્ણાંક fd, સંરચના stat *st)
{
  પાછા fstat (fd, st);
}
#endif

/* Return the address of the first plain %b spec in FMT, or NULL if
   there is no such spec.  %5b etc. do not match, so that user
   widths/flags are honored.  */

ATTRIBUTE_PURE
સ્થિર અક્ષર અચલ *
first_percent_b (અક્ષર અચલ *fmt)
{
  માટે (; *fmt; fmt++)
    જો (fmt[0] == '%')
      પસંદગી (fmt[1])
        {
        સ્થિતિ 'b': પાછા fmt;
        સ્થિતિ '%': fmt++; તોડો;
        }
  પાછા ખાલી;
}

સ્થિર અક્ષર RFC3986[256];
સ્થિર શૂન્ય
file_escape_init (શૂન્ય)
{
  માટે (પૂર્ણાંક i = 0; i < 256; i++)
    RFC3986[i] |= c_isalnum (i) || i == '~' || i == '-' || i == '.' || i == '_';
}

ગણના { MBSWIDTH_FLAGS = MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE };

/* Read the abbreviated month names from the locale, to align them
   and to determine the max width of the field and to truncate names
   greater than our max allowed.
   Note even though this handles multibyte locales correctly
   it's not restricted to them as single byte locales can have
   variable width abbreviated months and also precomputing/caching
   the names was seen to increase the performance of ls significantly.  */

/* abformat[RECENT][MON] is the format to use for timestamps with
   recentness RECENT and month MON.  */
ગણના { ABFORMAT_SIZE = 128 };
સ્થિર અક્ષર abformat[2][12][ABFORMAT_SIZE];
/* True if precomputed formats should be used.  This can be false if
   nl_langinfo fails, if a format or month abbreviation is unusually
   long, or if a month abbreviation contains '%'.  */
સ્થિર bool use_abformat;

/* Store into ABMON the abbreviated month names, suitably aligned.
   Return true if successful.  */

સ્થિર bool
abmon_init (અક્ષર abmon[12][ABFORMAT_SIZE])
{
#ifndef HAVE_NL_LANGINFO
  પાછા false;
#નહીંતર
  પૂર્ણાંક max_mon_width = 0;
  પૂર્ણાંક mon_width[12];
  પૂર્ણાંક mon_len[12];

  માટે (પૂર્ણાંક i = 0; i < 12; i++)
    {
      અક્ષર અચલ *abbr = nl_langinfo (ABMON_1 + i);
      mon_len[i] = strnlen (abbr, ABFORMAT_SIZE);
      જો (mon_len[i] == ABFORMAT_SIZE)
        પાછા false;
      જો (strchr (abbr, '%'))
        પાછા false;
      mon_width[i] = mbswidth (strcpy (abmon[i], abbr), MBSWIDTH_FLAGS);
      જો (mon_width[i] < 0)
        પાછા false;
      max_mon_width = MAX (max_mon_width, mon_width[i]);
    }

  માટે (પૂર્ણાંક i = 0; i < 12; i++)
    {
      પૂર્ણાંક fill = max_mon_width - mon_width[i];
      જો (ABFORMAT_SIZE - mon_len[i] <= fill)
        પાછા false;
      bool align_ડાબું = !c_isdigit (abmon[i][0]);
      પૂર્ણાંક fill_offset;
      જો (align_ડાબું)
        fill_offset = mon_len[i];
      નહીંતર
        {
          memmove (abmon[i] + fill, abmon[i], mon_len[i]);
          fill_offset = 0;
        }
      memset (abmon[i] + fill_offset, ' ', fill);
      abmon[i][mon_len[i] + fill] = '\0';
    }

  પાછા true;
#endif
}

/* Initialize ABFORMAT and USE_ABFORMAT.  */

સ્થિર શૂન્ય
abformat_init (શૂન્ય)
{
  અક્ષર અચલ *pb[2];
  માટે (પૂર્ણાંક recent = 0; recent < 2; recent++)
    pb[recent] = first_percent_b (long_time_format[recent]);
  જો (! (pb[0] || pb[1]))
    પાછા;

  અક્ષર abmon[12][ABFORMAT_SIZE];
  જો (! abmon_init (abmon))
    પાછા;

  માટે (પૂર્ણાંક recent = 0; recent < 2; recent++)
    {
      અક્ષર અચલ *fmt = long_time_format[recent];
      માટે (પૂર્ણાંક i = 0; i < 12; i++)
        {
          અક્ષર *nfmt = abformat[recent][i];
          પૂર્ણાંક nbytes;

          જો (! pb[recent])
            nbytes = snprintf (nfmt, ABFORMAT_SIZE, "%s", fmt);
          નહીંતર
            {
              જો (! (pb[recent] - fmt <= MIN (ABFORMAT_SIZE, INT_MAX)))
                પાછા;
              પૂર્ણાંક prefix_len = pb[recent] - fmt;
              nbytes = snprintf (nfmt, ABFORMAT_SIZE, "%.*s%s%s",
                                 prefix_len, fmt, abmon[i], pb[recent] + 2);
            }

          જો (! (0 <= nbytes && nbytes < ABFORMAT_SIZE))
            પાછા;
        }
    }

  use_abformat = true;
}

સ્થિર size_t
dev_ino_hash (શૂન્ય અચલ *x, size_t table_size)
{
  સંરચના dev_ino અચલ *p = x;
  પાછા (uintmax_t) {p->st_ino} % table_size;
}

સ્થિર bool
dev_ino_compare (શૂન્ય અચલ *x, શૂન્ય અચલ *y)
{
  સંરચના dev_ino અચલ *a = x;
  સંરચના dev_ino અચલ *b = y;
  પાછા PSAME_INODE (a, b);
}

સ્થિર શૂન્ય
dev_ino_free (શૂન્ય *x)
{
  મુક્ત (x);
}

/* Add the device/inode pair (P->st_dev/P->st_ino) to the set of
   active directories.  Return true if there is already a matching
   entry in the table.  */

સ્થિર bool
visit_dir (dev_t dev, ino_t ino)
{
  સંરચના dev_ino *ent;
  સંરચના dev_ino *ent_from_table;
  bool found_match;

  ent = xmalloc (કદ *ent);
  ent->st_ino = ino;
  ent->st_dev = dev;

  /* Attempt to insert this entry into the table.  */
  ent_from_table = hash_ઉમેરો (active_dir_set, ent);

  જો (ent_from_table == ખાલી)
    {
      /* Insertion failed due to lack of memory.  */
      xalloc_die ();
    }

  found_match = (ent_from_table != ent);

  જો (found_match)
    {
      /* ent was not inserted, so free it.  */
      મુક્ત (ent);
    }

  પાછા found_match;
}

સ્થિર શૂન્ય
free_pending_ent (સંરચના pending *p)
{
  મુક્ત (p->name);
  મુક્ત (p->realname);
  મુક્ત (p);
}

સ્થિર bool
is_colored (ગણના indicator_no type)
{
  /* Return true unless the string is "", "0" or "00"; try to be efficient.  */
  size_t len = color_indicator[type].len;
  જો (len == 0)
    પાછા false;
  જો (2 < len)
    પાછા true;
  અક્ષર અચલ *s = color_indicator[type].string;
  પાછા (s[0] != '0') | (s[len - 1] != '0');
}

સ્થિર શૂન્ય
restore_default_color (શૂન્ય)
{
  put_indicator (&color_indicator[C_ડાબું]);
  put_indicator (&color_indicator[C_જમણું]);
}

સ્થિર શૂન્ય
set_normal_color (શૂન્ય)
{
  જો (print_with_color && is_colored (C_NORM))
    {
      put_indicator (&color_indicator[C_ડાબું]);
      put_indicator (&color_indicator[C_NORM]);
      put_indicator (&color_indicator[C_જમણું]);
    }
}

/* An ordinary signal was received; arrange for the program to exit.  */

સ્થિર શૂન્ય
sighandler (પૂર્ણાંક sig)
{
  જો (! interrupt_signal)
    interrupt_signal = sig;
}

/* A SIGTSTP was received; arrange for the program to suspend itself.  */

સ્થિર શૂન્ય
stophandler (MAYBE_UNUSED પૂર્ણાંક sig)
{
  જો (! interrupt_signal)
    stop_signal_ગણતરી++;
}

/* Process any pending signals.  If signals are caught, this function
   should be called periodically.  Ideally there should never be an
   unbounded amount of time when signals are not being processed.
   Signal handling can restore the default colors, so callers must
   immediately change colors after invoking this function.  */

સ્થિર શૂન્ય
process_signals (શૂન્ય)
{
  જ્યારે (interrupt_signal || stop_signal_ગણતરી)
    {
      પૂર્ણાંક sig;
      પૂર્ણાંક stops;
      sigset_t oldset;

      જો (used_color)
        restore_default_color ();
      fflush (stdout);

      sigprocmask (SIG_BLOCK, &caught_signals, &oldset);

      /* Reload interrupt_signal and stop_signal_count, in case a new
         signal was handled before sigprocmask took effect.  */
      sig = interrupt_signal;
      stops = stop_signal_ગણતરી;

      /* SIGTSTP is special, since the application can receive that signal
         more than once.  In this case, don't set the signal handler to the
         default.  Instead, just raise the uncatchable SIGSTOP.  */
      જો (stops)
        {
          stop_signal_ગણતરી = stops - 1;
          sig = SIGSTOP;
        }
      નહીંતર
        સંકેત (sig, SIG_DFL);

      /* Exit or suspend the program.  */
      ઉઠાવો (sig);
      sigprocmask (SIG_SETMASK, &oldset, ખાલી);

      /* If execution reaches here, then the program has been
         continued (after being suspended).  */
    }
}

/* Setup signal handlers if INIT is true,
   otherwise restore to the default.  */

સ્થિર શૂન્ય
signal_setup (bool init)
{
  /* The signals that are trapped, and the number of such signals.  */
  સ્થિર પૂર્ણાંક અચલ stop_sig[] =
    {
      /* This one is handled specially.  */
      SIGTSTP,
    };

  ગણના { nsigs = countof (term_sig), nstop = countof (stop_sig) };

  જો (init)
    {
      સંરચના sigaction act;

      sigemptyset (&caught_signals);
      માટે (પૂર્ણાંક j = 0; j < nsigs + nstop; j++)
        {
          પૂર્ણાંક sig = j < nsigs ? term_sig[j]: stop_sig[j - nsigs];
          sigaction (sig, ખાલી, &act);
          જો (act.sa_handler != SIG_IGN)
            sigaddset (&caught_signals, sig);
        }

      act.sa_mask = caught_signals;
      act.sa_flags = SA_RESTART;

      માટે (પૂર્ણાંક j = 0; j < nsigs + nstop; j++)
        {
          પૂર્ણાંક sig = j < nsigs ? term_sig[j]: stop_sig[j - nsigs];
          જો (sigismember (&caught_signals, sig))
            {
              act.sa_handler = j < nsigs ? sighandler : stophandler;
              sigaction (sig, &act, ખાલી);
            }
        }
    }
  નહીંતર /* restore.  */
    {
      માટે (પૂર્ણાંક j = 0; j < nsigs + nstop; j++)
        {
          પૂર્ણાંક sig = j < nsigs ? term_sig[j]: stop_sig[j - nsigs];
          જો (sigismember (&caught_signals, sig))
            સંકેત (sig, SIG_DFL);
        }
    }
}

સ્થિર શૂન્ય
signal_init (શૂન્ય)
{
  signal_setup (true);
}

સ્થિર શૂન્ય
signal_restore (શૂન્ય)
{
  signal_setup (false);
}

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  પૂર્ણાંક i;
  સંરચના pending *thispend;
  પૂર્ણાંક n_files;

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
  pending_dirs = ખાલી;

  current_time.tv_sec = TYPE_MINIMUM (time_t);
  current_time.tv_nsec = -1;

  i = decode_switches (argc, argv);

  જો (print_with_color)
    parse_ls_color ();

  /* Test print_with_color again, because the call to parse_ls_color
     may have just reset it -- e.g., if LS_COLORS is invalid.  */

  જો (print_with_color)
    {
      /* Don't use TAB characters in output.  Some terminal
         emulators can't handle the combination of tabs and
         color codes on the same line.  */
      tabsize = 0;
    }

  જો (directories_first)
    check_symlink_mode = true;
  નહીંતર જો (print_with_color)
    {
      /* Avoid following symbolic links when possible.  */
      જો (is_colored (C_ORPHAN)
          || (is_colored (C_EXEC) && color_symlink_as_referent)
          || (is_colored (C_MISSING) && format == long_format))
        check_symlink_mode = true;
    }

  જો (dereference == DEREF_UNDEFINED)
    dereference = ((immediate_dirs
                    || indicator_style == classify
                    || format == long_format)
                   ? DEREF_NEVER
                   : DEREF_COMMAND_લીટી_SYMLINK_TO_DIR);

  /* When using -R, initialize a data structure we'll use to
     detect any directory cycles.  */
  જો (recursive)
    {
      active_dir_set = hash_initialize (INITIAL_TABLE_SIZE, ખાલી,
                                        dev_ino_hash,
                                        dev_ino_compare,
                                        dev_ino_free);
      જો (active_dir_set == ખાલી)
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

  જો (dired)
    {
      obstack_init (&dired_obstack);
      obstack_init (&subdired_obstack);
    }

  જો (print_hyperlink)
    {
      file_escape_init ();

      hostname = xgethostname ();
      /* The hostname is generally ignored,
         so ignore failures obtaining it.  */
      જો (! hostname)
        hostname = "";
    }

  cwd_n_alloc = 100;
  cwd_file = xmalloc (cwd_n_alloc * કદ *cwd_file);
  cwd_n_used = 0;

  clear_files ();

  n_files = argc - i;

  જો (n_files <= 0)
    {
      જો (immediate_dirs)
        gobble_file (".", directory, NOT_AN_INODE_સંખ્યા, true, ખાલી);
      નહીંતર
        queue_directory (".", ખાલી, true);
    }
  નહીંતર
    કરો
      gobble_file (argv[i++], unknown, NOT_AN_INODE_સંખ્યા, true, ખાલી);
    જ્યારે (i < argc);

  જો (cwd_n_used)
    {
      sort_files ();
      જો (!immediate_dirs)
        extract_dirs_from_files (ખાલી, true);
      /* 'cwd_n_used' might be zero now.  */
    }

  /* In the following if/else blocks, it is sufficient to test 'pending_dirs'
     (and not pending_dirs->name) because there may be no markers in the queue
     at this point.  A marker may be enqueued when extract_dirs_from_files is
     called with a non-empty string or via print_dir.  */
  જો (cwd_n_used)
    {
      print_current_files ();
      જો (pending_dirs)
        dired_outbyte ('\n');
    }
  નહીંતર જો (n_files <= 1 && pending_dirs && !pending_dirs->next)
    print_dir_name = false;

  જ્યારે (pending_dirs)
    {
      thispend = pending_dirs;
      pending_dirs = pending_dirs->next;

      જો (LOOP_DETECT)
        {
          જો (thispend->name == ખાલી)
            {
              /* thispend->name == NULL means this is a marker entry
                 indicating we've finished processing the directory.
                 Use its dev/ino numbers to remove the corresponding
                 entry from the active_dir_set hash table.  */
              સંરચના dev_ino di = dev_ino_કાઢો ();
              સંરચના dev_ino *found = hash_remove (active_dir_set, &di);
              જો (false)
                assert_matching_dev_ino (thispend->realname, di);
              affirm (found);
              dev_ino_free (found);
              free_pending_ent (thispend);
              ચાલુ;
            }
        }

      print_dir (thispend->name, thispend->realname,
                 thispend->command_લીટી_arg);

      free_pending_ent (thispend);
      print_dir_name = true;
    }

  જો (print_with_color && used_color)
    {

      /* Skip the restore when it would be a no-op, i.e.,
         when left is "\033[" and right is "m".  */
      જો (!(color_indicator[C_ડાબું].len == 2
            && memeq (color_indicator[C_ડાબું].string, "\033[", 2)
            && color_indicator[C_જમણું].len == 1
            && color_indicator[C_જમણું].string[0] == 'm'))
        restore_default_color ();

      fflush (stdout);

      signal_restore ();

      /* Act on any signals that arrived before the default was restored.
         This can process signals out of order, but there doesn't seem to
         be an easy way to do them in order, and the order isn't that
         important anyway.  */
      માટે (પૂર્ણાંક j = stop_signal_ગણતરી; j; j--)
        ઉઠાવો (SIGSTOP);
      પૂર્ણાંક j = interrupt_signal;
      જો (j)
        ઉઠાવો (j);
    }

  જો (dired)
    {
      /* No need to free these since we're about to exit.  */
      dired_dump_obstack ("//DIRED//", &dired_obstack);
      dired_dump_obstack ("//SUBDIRED//", &subdired_obstack);
      printf ("//DIRED-OPTIONS// --quoting-style=%s\n",
              quoting_style_args[get_quoting_style (filename_quoting_options)]);
    }

  જો (LOOP_DETECT)
    {
      assure (hash_get_n_entries (active_dir_set) == 0);
      hash_free (active_dir_set);
    }

  પાછા exit_status;
}

/* Return the line length indicated by the value given by SPEC, or -1
   if unsuccessful.  0 means no limit on line length.  */

સ્થિર ptrdiff_t
decode_લીટી_length (અક્ષર અચલ *spec)
{
  uintmax_t val;

  /* Treat too-large values as if they were 0, which is
     effectively infinity.  */
  પસંદગી (xstrtoumax (spec, ખાલી, 0, &val, ""))
    {
    સ્થિતિ LONGINT_OK:
      પાછા val <= MIN (PTRDIFF_MAX, SIZE_MAX) ? val : 0;

    સ્થિતિ LONGINT_OVERFLOW:
      પાછા 0;

    સ્થિતિ LONGINT_INVALID:
    સ્થિતિ LONGINT_INVALID_SUFFIX_CHAR:
    સ્થિતિ LONGINT_INVALID_SUFFIX_CHAR_WITH_OVERFLOW:
      પાછા -1;

    મૂળભૂત:
      unreachable ();
    }
}

/* Return true if standard output is a tty, caching the result.  */

સ્થિર bool
stdout_isatty (શૂન્ય)
{
  સ્થિર signed અક્ષર out_tty = -1;
  જો (out_tty < 0)
    out_tty = isatty (STDOUT_FILENO);
  assume (out_tty == 0 || out_tty == 1);
  પાછા out_tty;
}

/* Set all the option flags according to the switches specified.
   Return the index of the first non-option argument.  */

સ્થિર પૂર્ણાંક
decode_switches (પૂર્ણાંક argc, અક્ષર **argv)
{
  અક્ષર *time_style_option = ખાલી;

  /* These variables are false or -1 unless a switch says otherwise.  */
  bool kibibytes_specified = false;
  પૂર્ણાંક format_opt = -1;
  પૂર્ણાંક hide_control_chars_opt = -1;
  પૂર્ણાંક quoting_style_opt = -1;
  પૂર્ણાંક sort_opt = -1;
  ptrdiff_t tabsize_opt = -1;
  ptrdiff_t width_opt = -1;

  જ્યારે (true)
    {
      પૂર્ણાંક oi = -1;
      પૂર્ણાંક c = getopt_long (argc, argv,
                           "abcdfghiklmnopqrstuvw:xABCDFGHI:LNQRST:UXZ1",
                           long_options, &oi);
      જો (c == -1)
        તોડો;

      પસંદગી (c)
        {
        સ્થિતિ 'a':
          ignore_mode = IGNORE_MINIMAL;
          તોડો;

        સ્થિતિ 'b':
          quoting_style_opt = escape_quoting_style;
          તોડો;

        સ્થિતિ 'c':
          time_type = time_ctime;
          explicit_time = true;
          તોડો;

        સ્થિતિ 'd':
          immediate_dirs = true;
          તોડો;

        સ્થિતિ 'f':
          ignore_mode = IGNORE_MINIMAL; /* enable -a */
          sort_opt = sort_none;         /* enable -U */
          તોડો;

        સ્થિતિ FILE_TYPE_INDICATOR_OPTION: /* --file-type */
          indicator_style = file_type;
          તોડો;

        સ્થિતિ 'g':
          format_opt = long_format;
          print_owner = false;
          તોડો;

        સ્થિતિ 'h':
          file_human_output_opts = human_output_opts =
            human_autoscale | human_SI | human_base_1024;
          file_output_block_size = output_block_size = 1;
          તોડો;

        સ્થિતિ 'i':
          print_inode = true;
          તોડો;

        સ્થિતિ 'k':
          kibibytes_specified = true;
          તોડો;

        સ્થિતિ 'l':
          format_opt = long_format;
          તોડો;

        સ્થિતિ 'm':
          format_opt = with_commas;
          તોડો;

        સ્થિતિ 'n':
          numeric_ids = true;
          format_opt = long_format;
          તોડો;

        સ્થિતિ 'o':  /* Just like -l, but don't display group info.  */
          format_opt = long_format;
          print_group = false;
          તોડો;

        સ્થિતિ 'p':
          indicator_style = slash;
          તોડો;

        સ્થિતિ 'q':
          hide_control_chars_opt = true;
          તોડો;

        સ્થિતિ 'r':
          sort_reverse = true;
          તોડો;

        સ્થિતિ 's':
          print_block_size = true;
          તોડો;

        સ્થિતિ 't':
          sort_opt = sort_time;
          તોડો;

        સ્થિતિ 'u':
          time_type = time_atime;
          explicit_time = true;
          તોડો;

        સ્થિતિ 'v':
          sort_opt = sort_version;
          તોડો;

        સ્થિતિ 'w':
          width_opt = decode_લીટી_length (optarg);
          જો (width_opt < 0)
            error (LS_FAILURE, 0, "%s: %s", _("invalid line width"),
                   quote (optarg));
          તોડો;

        સ્થિતિ 'x':
          format_opt = horizontal;
          તોડો;

        સ્થિતિ 'A':
          ignore_mode = IGNORE_DOT_AND_DOTDOT;
          તોડો;

        સ્થિતિ 'B':
          add_ignore_pattern ("*~");
          add_ignore_pattern (".*~");
          તોડો;

        સ્થિતિ 'C':
          format_opt = many_per_લીટી;
          તોડો;

        સ્થિતિ 'D':
          format_opt = long_format;
          print_hyperlink = false;
          dired = true;
          તોડો;

        સ્થિતિ 'F':
          {
            પૂર્ણાંક i;
            જો (optarg)
              i = XARGMATCH ("--classify", optarg, when_args, when_types);
            નહીંતર
              /* Using --classify with no argument is equivalent to using
                 --classify=always.  */
              i = when_always;

            જો (i == when_always || (i == when_if_tty && stdout_isatty ()))
              indicator_style = classify;
            તોડો;
          }

        સ્થિતિ 'G':		/* inhibit display of group info */
          print_group = false;
          તોડો;

        સ્થિતિ 'H':
          dereference = DEREF_COMMAND_લીટી_ARGUMENTS;
          તોડો;

        સ્થિતિ DEREFERENCE_COMMAND_લીટી_SYMLINK_TO_DIR_OPTION:
          dereference = DEREF_COMMAND_લીટી_SYMLINK_TO_DIR;
          તોડો;

        સ્થિતિ 'I':
          add_ignore_pattern (optarg);
          તોડો;

        સ્થિતિ 'L':
          dereference = DEREF_ALWAYS;
          તોડો;

        સ્થિતિ 'N':
          quoting_style_opt = literal_quoting_style;
          તોડો;

        સ્થિતિ 'Q':
          quoting_style_opt = c_quoting_style;
          તોડો;

        સ્થિતિ 'R':
          recursive = true;
          તોડો;

        સ્થિતિ 'S':
          sort_opt = sort_size;
          તોડો;

        સ્થિતિ 'T':
          tabsize_opt = xnumtoumax (optarg, 0, 0, MIN (PTRDIFF_MAX, SIZE_MAX),
                                    "", _("invalid tab size"), LS_FAILURE, 0);
          તોડો;

        સ્થિતિ 'U':
          sort_opt = sort_none;
          તોડો;

        સ્થિતિ 'X':
          sort_opt = sort_extension;
          તોડો;

        સ્થિતિ '1':
          /* -1 has no effect after -l.  */
          જો (format_opt != long_format)
            format_opt = one_per_લીટી;
          તોડો;

        સ્થિતિ AUTHOR_OPTION:
          print_author = true;
          તોડો;

        સ્થિતિ HIDE_OPTION:
          {
            સંરચના ignore_pattern *hide = xmalloc (કદ *hide);
            hide->pattern = optarg;
            hide->next = hide_patterns;
            hide_patterns = hide;
          }
          તોડો;

        સ્થિતિ SORT_OPTION:
          sort_opt = XARGMATCH ("--sort", optarg, sort_args, sort_types);
          તોડો;

        સ્થિતિ GROUP_DIRECTORIES_FIRST_OPTION:
          directories_first = true;
          તોડો;

        સ્થિતિ TIME_OPTION:
          time_type = XARGMATCH ("--time", optarg, time_args, time_types);
          explicit_time = true;
          તોડો;

        સ્થિતિ FORMAT_OPTION:
          format_opt = XARGMATCH ("--format", optarg, format_args,
                                  format_types);
          તોડો;

        સ્થિતિ FULL_TIME_OPTION:
          format_opt = long_format;
          time_style_option = (અક્ષર *) "full-iso";
          તોડો;

        સ્થિતિ COLOR_OPTION:
          {
            પૂર્ણાંક i;
            જો (optarg)
              i = XARGMATCH ("--color", optarg, when_args, when_types);
            નહીંતર
              /* Using --color with no argument is equivalent to using
                 --color=always.  */
              i = when_always;

            print_with_color = (i == when_always
                                || (i == when_if_tty && stdout_isatty ()));
            તોડો;
          }

        સ્થિતિ HYPERLINK_OPTION:
          {
            પૂર્ણાંક i;
            જો (optarg)
              i = XARGMATCH ("--hyperlink", optarg, when_args, when_types);
            નહીંતર
              /* Using --hyperlink with no argument is equivalent to using
                 --hyperlink=always.  */
              i = when_always;

            print_hyperlink = (i == when_always
                               || (i == when_if_tty && stdout_isatty ()));
            તોડો;
          }

        સ્થિતિ INDICATOR_STYLE_OPTION:
          indicator_style = XARGMATCH ("--indicator-style", optarg,
                                       indicator_style_args,
                                       indicator_style_types);
          તોડો;

        સ્થિતિ QUOTING_STYLE_OPTION:
          quoting_style_opt = XARGMATCH ("--quoting-style", optarg,
                                         quoting_style_args,
                                         quoting_style_vals);
          તોડો;

        સ્થિતિ TIME_STYLE_OPTION:
          time_style_option = optarg;
          તોડો;

        સ્થિતિ SHOW_CONTROL_CHARS_OPTION:
          hide_control_chars_opt = false;
          તોડો;

        સ્થિતિ BLOCK_SIZE_OPTION:
          {
            ગણના strtol_error e = human_options (optarg, &human_output_opts,
                                                 &output_block_size);
            જો (e != LONGINT_OK)
              xstrtol_fatal (e, oi, 0, long_options, optarg);
            file_human_output_opts = human_output_opts;
            file_output_block_size = output_block_size;
          }
          તોડો;

        સ્થિતિ SI_OPTION:
          file_human_output_opts = human_output_opts =
            human_autoscale | human_SI;
          file_output_block_size = output_block_size = 1;
          તોડો;

        સ્થિતિ 'Z':
          print_scontext = true;
          તોડો;

        સ્થિતિ ZERO_OPTION:
          eolbyte = 0;
          hide_control_chars_opt = false;
          જો (format_opt != long_format)
            format_opt = one_per_લીટી;
          print_with_color = false;
          quoting_style_opt = literal_quoting_style;
          તોડો;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        મૂળભૂત:
          usage (LS_FAILURE);
        }
    }

  જો (! output_block_size)
    {
      અક્ષર અચલ *ls_block_size = getenv ("LS_BLOCK_SIZE");
      human_options (ls_block_size,
                     &human_output_opts, &output_block_size);
      જો (ls_block_size || getenv ("BLOCK_SIZE"))
        {
          file_human_output_opts = human_output_opts;
          file_output_block_size = output_block_size;
        }
      જો (kibibytes_specified)
        {
          human_output_opts = 0;
          output_block_size = 1024;
        }
    }

  format = (0 <= format_opt ? format_opt
            : ls_mode == LS_LS ? (stdout_isatty ()
                                  ? many_per_લીટી : one_per_લીટી)
            : ls_mode == LS_MULTI_COL ? many_per_લીટી
            : /* ls_mode == LS_LONG_FORMAT */ long_format);

  /* If the line length was not set by a switch but is needed to determine
     output, go to the work of obtaining it from the environment.  */
  ptrdiff_t linelen = width_opt;
  જો (format == many_per_લીટી || format == horizontal || format == with_commas
      || print_with_color)
    {
#ifdef TIOCGWINSZ
      જો (linelen < 0)
        {
          સંરચના winsize ws;
          જો (stdout_isatty ()
              && 0 <= ioctl (STDOUT_FILENO, TIOCGWINSZ, &ws)
              && 0 < ws.ws_col)
            linelen = ws.ws_col <= MIN (PTRDIFF_MAX, SIZE_MAX) ? ws.ws_col : 0;
        }
#endif
      જો (linelen < 0)
        {
          અક્ષર અચલ *p = getenv ("COLUMNS");
          જો (p && *p)
            {
              linelen = decode_લીટી_length (p);
              જો (linelen < 0)
                error (0, 0,
                       _("ignoring invalid width"
                         " in environment variable COLUMNS: %s"),
                       quote (p));
            }
        }
    }

  લીટી_length = linelen < 0 ? 80 : linelen;

  /* Determine the max possible number of display columns.  */
  max_idx = લીટી_length / MIN_COLUMN_WIDTH;
  /* Account for first display column not having a separator,
     or line_lengths shorter than MIN_COLUMN_WIDTH.  */
  max_idx += લીટી_length % MIN_COLUMN_WIDTH != 0;

  જો (format == many_per_લીટી || format == horizontal || format == with_commas)
    {
      જો (0 <= tabsize_opt)
        tabsize = tabsize_opt;
      નહીંતર
        {
          tabsize = 8;
          અક્ષર અચલ *p = getenv ("TABSIZE");
          જો (p)
            {
              uintmax_t tmp;
              જો (xstrtoumax (p, ખાલી, 0, &tmp, "") == LONGINT_OK
                  && tmp <= SIZE_MAX)
                tabsize = tmp;
              નહીંતર
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
                  && ((લીટી_length
                       && (format == with_commas
                           || format == many_per_લીટી || format == horizontal))
                      || format == one_per_લીટી || format == long_format);

  પૂર્ણાંક qs = quoting_style_opt;
  જો (qs < 0)
    qs = getenv_quoting_style ();
  જો (qs < 0)
    qs = (ls_mode == LS_LS
          ? (stdout_isatty () ? shell_escape_quoting_style : -1)
          : escape_quoting_style);
  જો (0 <= qs)
    set_quoting_style (ખાલી, qs);
  qs = get_quoting_style (ખાલી);
  align_variable_outer_quotes
    = ((format == long_format
        || ((format == many_per_લીટી || format == horizontal) && લીટી_length))
       && (qs == shell_quoting_style
           || qs == shell_escape_quoting_style
           || qs == c_maybe_quoting_style));
  filename_quoting_options = clone_quoting_options (ખાલી);
  જો (qs == escape_quoting_style)
    set_char_quoting (filename_quoting_options, ' ', 1);
  જો (file_type <= indicator_style)
    {
      માટે (અક્ષર અચલ *p = &"*=>@|"[indicator_style - file_type]; *p; p++)
        set_char_quoting (filename_quoting_options, *p, 1);
    }
  જો (format == with_commas
      && (qs == shell_quoting_style
          || qs == shell_escape_quoting_style
          || qs == c_maybe_quoting_style
          || qs == escape_quoting_style))
    set_char_quoting (filename_quoting_options, ',', 1);

  dirname_quoting_options = clone_quoting_options (ખાલી);
  set_char_quoting (dirname_quoting_options, ':', 1);

  /* --dired implies --format=long (-l) and sans --hyperlink.
     So ignore it if those overridden.  */
  dired &= (format == long_format) & !print_hyperlink;

  જો (eolbyte < dired)
    error (LS_FAILURE, 0, _("--dired and --zero are incompatible"));

  /* If a time type is explicitly specified (with -c, -u, or --time=)
     and we're not showing a time (-l not specified), then sort by that time,
     rather than by name.  Note this behavior is unspecified by POSIX.  */

  sort_type = (0 <= sort_opt ? sort_opt
               : (format != long_format && explicit_time)
               ? sort_time : sort_name);

  જો (format == long_format)
    {
      અક્ષર *envstyle = ખાલી;
      અક્ષર *style = time_style_option;
      જો (! style)
        style = envstyle = getenv ("TIME_STYLE");

      જો (! style)
        style = (અક્ષર *) "locale";
      નહીંતર
        {
          સ્થિર અક્ષર અચલ posix_prefix[] = "posix-";
          જ્યારે (STREQ_LEN (style, posix_prefix, કદ posix_prefix - 1))
            {
              જો (! hard_locale (LC_TIME))
                પાછા optind;
              style += કદ posix_prefix - 1;
            }
        }

      જો (*style == '+')
        {
          અક્ષર *p0 = style + 1;
          અક્ષર *p0nl = strchr (p0, '\n');
          અક્ષર અચલ *p1 = p0;
          જો (p0nl)
            {
              p1 = p0nl + 1;
              જો (strchr (p1, '\n'))
                error (LS_FAILURE, 0, _("invalid time style format %s"),
                       quote (p0));
              જો (envstyle)
                p0 = xmemdup0 (p0, p0nl - p0);
              નહીંતર
                *p0nl = '\0';
            }
          long_time_format[0] = p0;
          long_time_format[1] = p1;
        }
      નહીંતર
        {
          પસંદગી (x_timestyle_match (style, /*allow_posix=*/ true,
                                     time_style_args,
                                     (અક્ષર અચલ *) time_style_types,
                                     કદ (*time_style_types), LS_FAILURE))
            {
            સ્થિતિ full_iso_time_style:
              long_time_format[0] = long_time_format[1] =
                "%Y-%m-%d %H:%M:%S.%N %z";
              તોડો;

            સ્થિતિ long_iso_time_style:
              long_time_format[0] = long_time_format[1] = "%Y-%m-%d %H:%M";
              તોડો;

            સ્થિતિ iso_time_style:
              long_time_format[0] = "%Y-%m-%d ";
              long_time_format[1] = "%m-%d %H:%M";
              તોડો;

            સ્થિતિ locale_time_style:
              જો (hard_locale (LC_TIME))
                {
                  માટે (પૂર્ણાંક i = 0; i < 2; i++)
                    long_time_format[i] =
                      dcgettext (ખાલી, long_time_format[i], LC_TIME);
                }
            }
        }

      abformat_init ();
    }

  પાછા optind;
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

સ્થિર bool
get_funky_string (અક્ષર **dest, અક્ષર અચલ **src, bool equals_end,
                  size_t *output_ગણતરી)
{
  અક્ષર અંક;			/* For numerical codes */
  size_t ગણતરી;			/* Something to count with */
  ગણના {
    ST_GND, ST_BACKSLASH, ST_OCTAL, ST_HEX, ST_CARET, ST_END, ST_ERROR
  } state;
  અક્ષર અચલ *p;
  અક્ષર *q;

  p = *src;			/* We don't want to double-indirect */
  q = *dest;			/* the whole darn time.  */

  ગણતરી = 0;			/* No characters counted in yet.  */
  અંક = 0;

  state = ST_GND;		/* Start in ground state.  */
  જ્યારે (state < ST_END)
    {
      પસંદગી (state)
        {
        સ્થિતિ ST_GND:		/* Ground state (no escapes) */
          પસંદગી (*p)
            {
            સ્થિતિ ':':
            સ્થિતિ '\0':
              state = ST_END;	/* End of string */
              તોડો;
            સ્થિતિ '\\':
              state = ST_BACKSLASH; /* Backslash escape sequence */
              ++p;
              તોડો;
            સ્થિતિ '^':
              state = ST_CARET; /* Caret escape */
              ++p;
              તોડો;
            સ્થિતિ '=':
              જો (equals_end)
                {
                  state = ST_END; /* End */
                  તોડો;
                }
              FALLTHROUGH;
            મૂળભૂત:
              *(q++) = *(p++);
              ++ગણતરી;
              તોડો;
            }
          તોડો;

        સ્થિતિ ST_BACKSLASH:	/* Backslash escaped character */
          પસંદગી (*p)
            {
            સ્થિતિ '0':
            સ્થિતિ '1':
            સ્થિતિ '2':
            સ્થિતિ '3':
            સ્થિતિ '4':
            સ્થિતિ '5':
            સ્થિતિ '6':
            સ્થિતિ '7':
              state = ST_OCTAL;	/* Octal sequence */
              અંક = *p - '0';
              તોડો;
            સ્થિતિ 'x':
            સ્થિતિ 'X':
              state = ST_HEX;	/* Hex sequence */
              અંક = 0;
              તોડો;
            સ્થિતિ 'a':		/* Bell */
              અંક = '\a';
              તોડો;
            સ્થિતિ 'b':		/* Backspace */
              અંક = '\b';
              તોડો;
            સ્થિતિ 'e':		/* Escape */
              અંક = 27;
              તોડો;
            સ્થિતિ 'f':		/* Form feed */
              અંક = '\f';
              તોડો;
            સ્થિતિ 'n':		/* Newline */
              અંક = '\n';
              તોડો;
            સ્થિતિ 'r':		/* Carriage return */
              અંક = '\r';
              તોડો;
            સ્થિતિ 't':		/* Tab */
              અંક = '\t';
              તોડો;
            સ્થિતિ 'v':		/* Vtab */
              અંક = '\v';
              તોડો;
            સ્થિતિ '?':		/* Delete */
              અંક = 127;
              તોડો;
            સ્થિતિ '_':		/* Space */
              અંક = ' ';
              તોડો;
            સ્થિતિ '\0':		/* End of string */
              state = ST_ERROR;	/* Error! */
              તોડો;
            મૂળભૂત:		/* Escaped character like \ ^ : = */
              અંક = *p;
              તોડો;
            }
          જો (state == ST_BACKSLASH)
            {
              *(q++) = અંક;
              ++ગણતરી;
              state = ST_GND;
            }
          ++p;
          તોડો;

        સ્થિતિ ST_OCTAL:		/* Octal sequence */
          જો (*p < '0' || *p > '7')
            {
              *(q++) = અંક;
              ++ગણતરી;
              state = ST_GND;
            }
          નહીંતર
            અંક = (અંક << 3) + (*(p++) - '0');
          તોડો;

        સ્થિતિ ST_HEX:		/* Hex sequence */
          પસંદગી (*p)
            {
            સ્થિતિ '0':
            સ્થિતિ '1':
            સ્થિતિ '2':
            સ્થિતિ '3':
            સ્થિતિ '4':
            સ્થિતિ '5':
            સ્થિતિ '6':
            સ્થિતિ '7':
            સ્થિતિ '8':
            સ્થિતિ '9':
              અંક = (અંક << 4) + (*(p++) - '0');
              તોડો;
            સ્થિતિ 'a':
            સ્થિતિ 'b':
            સ્થિતિ 'c':
            સ્થિતિ 'd':
            સ્થિતિ 'e':
            સ્થિતિ 'f':
              અંક = (અંક << 4) + (*(p++) - 'a') + 10;
              તોડો;
            સ્થિતિ 'A':
            સ્થિતિ 'B':
            સ્થિતિ 'C':
            સ્થિતિ 'D':
            સ્થિતિ 'E':
            સ્થિતિ 'F':
              અંક = (અંક << 4) + (*(p++) - 'A') + 10;
              તોડો;
            મૂળભૂત:
              *(q++) = અંક;
              ++ગણતરી;
              state = ST_GND;
              તોડો;
            }
          તોડો;

        સ્થિતિ ST_CARET:		/* Caret escape */
          state = ST_GND;	/* Should be the next state... */
          જો (*p >= '@' && *p <= '~')
            {
              *(q++) = *(p++) & 037;
              ++ગણતરી;
            }
          નહીંતર જો (*p == '?')
            {
              *(q++) = 127;
              ++ગણતરી;
            }
          નહીંતર
            state = ST_ERROR;
          તોડો;

        સ્થિતિ ST_END: સ્થિતિ ST_ERROR: મૂળભૂત:
          affirm (false);
        }
    }

  *dest = q;
  *src = p;
  *output_ગણતરી = ગણતરી;

  પાછા state != ST_ERROR;
}

ગણના parse_state
  {
    PS_START = 1,
    PS_2,
    PS_3,
    PS_4,
    PS_DONE,
    PS_FAIL
  };


/* Check if the content of TERM is a valid name in dircolors.  */

સ્થિર bool
known_term_type (શૂન્ય)
{
  અક્ષર અચલ *term = getenv ("TERM");
  જો (! term || ! *term)
    પાછા false;

  અક્ષર અચલ *લીટી = G_લીટી;
  જ્યારે (લીટી - G_લીટી < કદ (G_લીટી))
    {
      જો (STRNCMP_LIT (લીટી, "TERM ") == 0)
        {
          જો (fnmatch (લીટી + 5, term, 0) == 0)
            પાછા true;
        }
      લીટી += strlen (લીટી) + 1;
    }

  પાછા false;
}

સ્થિર શૂન્ય
parse_ls_color (શૂન્ય)
{
  અક્ષર અચલ *p;		/* Pointer to character being parsed */
  અક્ષર *buf;			/* color_buf buffer pointer */
  અક્ષર label0, label1;		/* Indicator label */
  સંરચના color_ext_type *ext;	/* Extension we are working on */

  જો ((p = getenv ("LS_COLORS")) == ખાલી || *p == '\0')
    {
      /* LS_COLORS takes precedence, but if that's not set then
         honor the COLORTERM and TERM env variables so that
         we only go with the internal ANSI color codes if the
         former is non empty or the latter is set to a known value.  */
      અક્ષર અચલ *colorterm = getenv ("COLORTERM");
      જો (! (colorterm && *colorterm) && ! known_term_type ())
        print_with_color = false;
      પાછા;
    }

  ext = ખાલી;

  /* This is an overly conservative estimate, but any possible
     LS_COLORS string will *not* generate a color_buf longer than
     itself, so it is a safe way of allocating a buffer in
     advance.  */
  buf = color_buf = xstrdup (p);

  ગણના parse_state state = PS_START;
  જ્યારે (true)
    {
      પસંદગી (state)
        {
        સ્થિતિ PS_START:		/* First label character */
          પસંદગી (*p)
            {
            સ્થિતિ ':':
              ++p;
              તોડો;

            સ્થિતિ '*':
              /* Allocate new extension block and add to head of
                 linked list (this way a later definition will
                 override an earlier one, which can be useful for
                 having terminal-specific defs override global).  */

              ext = xmalloc (કદ *ext);
              ext->next = color_ext_list;
              color_ext_list = ext;
              ext->exact_match = false;

              ++p;
              ext->ext.string = buf;

              state = (get_funky_string (&buf, &p, true, &ext->ext.len)
                       ? PS_4 : PS_FAIL);
              તોડો;

            સ્થિતિ '\0':
              state = PS_DONE;	/* Done! */
              જાઓ done;

            મૂળભૂત:	/* Assume it is file type label */
              label0 = *p++;
              state = PS_2;
              તોડો;
            }
          તોડો;

        સ્થિતિ PS_2:		/* Second label character */
          જો (*p)
            {
              label1 = *p++;
              state = PS_3;
            }
          નહીંતર
            state = PS_FAIL;	/* Error */
          તોડો;

        સ્થિતિ PS_3:		/* Equal sign after indicator label */
          state = PS_FAIL;	/* Assume failure...  */
          જો (*(p++) == '=')/* It *should* be...  */
            {
              માટે (પૂર્ણાંક i = 0; i < countof (indicator_name); i++)
                {
                  જો ((label0 == indicator_name[i][0])
                      && (label1 == indicator_name[i][1]))
                    {
                      color_indicator[i].string = buf;
                      state = (get_funky_string (&buf, &p, false,
                                                 &color_indicator[i].len)
                               ? PS_START : PS_FAIL);
                      તોડો;
                    }
                }
              જો (state == PS_FAIL)
                error (0, 0, _("unrecognized prefix: %s"),
                       quote ((અક્ષર []) {label0, label1, '\0'}));
            }
          તોડો;

        સ્થિતિ PS_4:		/* Equal sign after *.ext */
          જો (*(p++) == '=')
            {
              ext->seq.string = buf;
              state = (get_funky_string (&buf, &p, false, &ext->seq.len)
                       ? PS_START : PS_FAIL);
            }
          નહીંતર
            state = PS_FAIL;
          તોડો;

        સ્થિતિ PS_FAIL:
          જાઓ done;

        સ્થિતિ PS_DONE: મૂળભૂત:
          affirm (false);
        }
    }
 done:

  જો (state == PS_FAIL)
    {
      error (0, 0,
             _("unparsable value for LS_COLORS environment variable"));
      મુક્ત (color_buf);
      માટે (સંરચના color_ext_type *e = color_ext_list; e != ખાલી;
           /* empty */)
        {
          સંરચના color_ext_type *e2 = e;
          e = e->next;
          મુક્ત (e2);
        }
      print_with_color = false;
      પાછા;
    }
  નહીંતર
    {
      /* Postprocess list to set EXACT_MATCH on entries where there are
         different cased extensions with separate sequences defined.
         Also set ext.len to SIZE_MAX on any entries that can't
         match due to precedence, to avoid redundant string compares.  */
      માટે (સંરચના color_ext_type *e1 = color_ext_list; e1 != ખાલી;
           e1 = e1->next)
        {
          bool case_ignored = false;

          માટે (સંરચના color_ext_type *e2 = e1->next; e2 != ખાલી;
               e2 = e2->next)
            {
              જો (e2->ext.len < SIZE_MAX && e1->ext.len == e2->ext.len)
                {
                  જો (memeq (e1->ext.string, e2->ext.string, e1->ext.len))
                    e2->ext.len = SIZE_MAX; /* Ignore */
                  નહીંતર જો (c_strncasecmp (e1->ext.string, e2->ext.string,
                                          e1->ext.len) == 0)
                    {
                      જો (case_ignored)
                        {
                          e2->ext.len = SIZE_MAX; /* Ignore */
                        }
                      નહીંતર જો (e1->seq.len == e2->seq.len
                               && memeq (e1->seq.string, e2->seq.string,
                                         e1->seq.len))
                        {
                          e2->ext.len = SIZE_MAX; /* Ignore */
                          case_ignored = true;    /* Ignore all subsequent */
                        }
                      નહીંતર
                        {
                          e1->exact_match = true;
                          e2->exact_match = true;
                        }
                    }
                }
            }
        }
    }

  જો (color_indicator[C_LINK].len == 6
      && !STRNCMP_LIT (color_indicator[C_LINK].string, "target"))
    color_symlink_as_referent = true;
}

/* Set the exit status to report a failure.  If SERIOUS, it is a
   serious failure; otherwise, it is merely a minor problem.  */

સ્થિર શૂન્ય
set_exit_status (bool serious)
{
  જો (serious)
    exit_status = LS_FAILURE;
  નહીંતર જો (exit_status == EXIT_SUCCESS)
    exit_status = LS_MINOR_PROBLEM;
}

/* Assuming a failure is serious if SERIOUS, use the printf-style
   MESSAGE to report the failure to access a file named FILE.  Assume
   errno is set appropriately for the failure.  */

સ્થિર શૂન્ય
file_failure (bool serious, અક્ષર અચલ *message, અક્ષર અચલ *file)
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

સ્થિર શૂન્ય
queue_directory (અક્ષર અચલ *name, અક્ષર અચલ *realname, bool command_લીટી_arg)
{
  સંરચના pending *new = xmalloc (કદ *new);
  new->realname = realname ? xstrdup (realname) : ખાલી;
  new->name = name ? xstrdup (name) : ખાલી;
  new->command_લીટી_arg = command_લીટી_arg;
  new->next = pending_dirs;
  pending_dirs = new;
}

/* Read directory NAME, and list the files in it.
   If REALNAME is nonzero, print its name instead of NAME;
   this is used for symbolic links to directories.
   COMMAND_LINE_ARG means this directory was mentioned on the command line.  */

સ્થિર શૂન્ય
print_dir (અક્ષર અચલ *name, અક્ષર અચલ *realname, bool command_લીટી_arg)
{
  DIR *dirp;
  સંરચના dirent *next;
  uintmax_t total_blocks = 0;
  સ્થિર bool first = true;

  errno = 0;
  dirp = opendir (name);
  જો (!dirp)
    {
      જો (command_લીટી_arg || ! ignorable_traversal_errno (errno))
        file_failure (command_લીટી_arg, _("cannot open directory %s"), name);
      પાછા;
    }

  જો (LOOP_DETECT)
    {
      સંરચના stat dir_stat;
      પૂર્ણાંક fd = dirfd (dirp);

      /* If dirfd failed, endure the overhead of stat'ing by path  */
      જો ((0 <= fd
           ? fstat_for_ino (fd, &dir_stat)
           : stat_for_ino (name, &dir_stat)) < 0)
        {
          જો (! ignorable_traversal_errno (errno))
            file_failure (command_લીટી_arg,
                          _("cannot determine device and inode of %s"), name);
          closedir (dirp);
          પાછા;
        }

      /* If we've already visited this dev/inode pair, warn that
         we've found a loop, and do not process this directory.  */
      જો (visit_dir (dir_stat.st_dev, dir_stat.st_ino))
        {
          error (0, 0, _("%s: not listing already-listed directory"),
                 quotef (name));
          closedir (dirp);
          set_exit_status (true);
          પાછા;
        }

      dev_ino_ધકેલો (dir_stat.st_dev, dir_stat.st_ino);
    }

  clear_files ();

  જો (recursive || print_dir_name)
    {
      જો (!first)
        dired_outbyte ('\n');
      first = false;
      dired_indent ();

      અક્ષર *absolute_name = ખાલી;
      જો (print_hyperlink)
        {
          absolute_name = canonicalize_filename_mode (name, CAN_MISSING);
          જો (! absolute_name
              && ! ignorable_traversal_errno (errno))
            file_failure (command_લીટી_arg,
                          _("error canonicalizing %s"), name);
        }
      quote_name (realname ? realname : name, dirname_quoting_options, -1,
                  ખાલી, true, &subdired_obstack, absolute_name);

      મુક્ત (absolute_name);

      dired_outstring (":\n");
    }

  /* Read the directory entries, and insert the subfiles into the 'cwd_file'
     table.  */

  જ્યારે (true)
    {
      /* Set errno to zero so we can distinguish between a readdir failure
         and when readdir simply finds that there are no more entries.  */
      errno = 0;
      next = readdir (dirp);
      જો (next)
        {
          જો (! file_ignored (next->d_name))
            {
              ગણના filetype type;
#જો HAVE_STRUCT_DIRENT_D_TYPE
              type = d_type_filetype[next->d_type];
#નહીંતર
              type = unknown;
#endif
              total_blocks += gobble_file (next->d_name, type,
                                           RELIABLE_D_INO (next),
                                           false, name);

              /* In this narrow case, print out each name right away, so
                 ls uses constant memory while processing the entries of
                 this directory.  Useful when there are many (millions)
                 of entries in a directory.  */
              જો (format == one_per_લીટી && sort_type == sort_none
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
      નહીંતર
        {
          પૂર્ણાંક err = errno;
          જો (err == 0)
            તોડો;
          /* Ignore errors indicating that the directory was removed.  */
          જો (ignorable_traversal_errno (err))
            તોડો;
          file_failure (command_લીટી_arg, _("reading directory %s"), name);
          જો (err != EOVERFLOW)
            તોડો;
        }

      /* When processing a very large directory, and since we've inhibited
         interrupts, this loop would take so long that ls would be annoyingly
         uninterruptible.  This ensures that it handles signals promptly.  */
      process_signals ();
    }

  જો (closedir (dirp) != 0)
    {
      file_failure (command_લીટી_arg, _("closing directory %s"), name);
      /* Don't return; print whatever we got.  */
    }

  /* Sort the directory contents.  */
  sort_files ();

  /* If any member files are subdirectories, perhaps they should have their
     contents listed rather than being mentioned here as files.  */

  જો (recursive)
    extract_dirs_from_files (name, false);

  જો (format == long_format || print_block_size)
    {
      અક્ષર buf[LONGEST_HUMAN_READABLE + 3];
      અક્ષર *p = human_readable (total_blocks, buf + 1, human_output_opts,
                                ST_NBLOCKSIZE, output_block_size);
      અક્ષર *pend = p + strlen (p);
      *--p = ' ';
      *pend++ = eolbyte;
      dired_indent ();
      dired_outstring (_("total"));
      dired_outbuf (p, pend - p);
    }

  જો (cwd_n_used)
    print_current_files ();
}

/* Add 'pattern' to the list of patterns for which files that match are
   not listed.  */

સ્થિર શૂન્ય
add_ignore_pattern (અક્ષર અચલ *pattern)
{
  સંરચના ignore_pattern *ignore;

  ignore = xmalloc (કદ *ignore);
  ignore->pattern = pattern;
  /* Add it to the head of the linked list.  */
  ignore->next = ignore_patterns;
  ignore_patterns = ignore;
}

/* Return true if one of the PATTERNS matches FILE.  */

સ્થિર bool
patterns_match (સંરચના ignore_pattern અચલ *patterns, અક્ષર અચલ *file)
{
  માટે (સંરચના ignore_pattern અચલ *p = patterns; p; p = p->next)
    જો (fnmatch (p->pattern, file, FNM_PERIOD) == 0)
      પાછા true;
  પાછા false;
}

/* Return true if FILE should be ignored.  */

સ્થિર bool
file_ignored (અક્ષર અચલ *name)
{
  પાછા ((ignore_mode != IGNORE_MINIMAL
           && name[0] == '.'
           && (ignore_mode == IGNORE_DEFAULT || ! name[1 + (name[1] == '.')]))
          || (ignore_mode == IGNORE_DEFAULT
              && patterns_match (hide_patterns, name))
          || patterns_match (ignore_patterns, name));
}

/* POSIX requires that a file size be printed without a sign, even
   when negative.  Assume the typical case where negative sizes are
   actually positive values that have wrapped around.  */

સ્થિર uintmax_t
unsigned_file_size (off_t size)
{
  પાછા size + (size < 0) * ((uintmax_t) OFF_T_MAX - OFF_T_MIN + 1);
}

#ifdef HAVE_CAP
/* Return true if NAME has a capability (see linux/capability.h) */
સ્થિર bool
has_capability (અક્ષર અચલ *name)
{
  અક્ષર *result;
  bool has_cap;

  cap_t cap_d = cap_get_file (name);
  જો (cap_d == ખાલી)
    પાછા false;

  result = cap_to_text (cap_d, ખાલી);
  cap_free (cap_d);
  જો (!result)
    પાછા false;

  /* check if human-readable capability string is empty */
  has_cap = !!*result;

  cap_free (result);
  પાછા has_cap;
}
#નહીંતર
સ્થિર bool
has_capability (MAYBE_UNUSED અક્ષર અચલ *name)
{
  errno = ENOTSUP;
  પાછા false;
}
#endif

/* Enter and remove entries in the table 'cwd_file'.  */

સ્થિર શૂન્ય
free_ent (સંરચના fileinfo *f)
{
  મુક્ત (f->name);
  મુક્ત (f->linkname);
  મુક્ત (f->absolute_name);
  જો (f->scontext != UNKNOWN_SECURITY_CONTEXT)
    aclinfo_scontext_free (f->scontext);
}

/* Empty the table of files.  */
સ્થિર શૂન્ય
clear_files (શૂન્ય)
{
  માટે (idx_t i = 0; i < cwd_n_used; i++)
    {
      સંરચના fileinfo *f = sorted_file[i];
      free_ent (f);
    }

  cwd_n_used = 0;
  cwd_some_quoted = false;
  any_has_acl = false;
  inode_સંખ્યા_width = 0;
  block_size_width = 0;
  nlink_width = 0;
  owner_width = 0;
  group_width = 0;
  author_width = 0;
  scontext_width = 0;
  major_device_સંખ્યા_width = 0;
  minor_device_સંખ્યા_width = 0;
  file_size_width = 0;
}

/* Cache file_has_aclinfo failure, when it's trivial to do.
   Like file_has_aclinfo, but when F's st_dev says it's on a file
   system lacking ACL support, return 0 with ENOTSUP immediately.  */
સ્થિર પૂર્ણાંક
file_has_aclinfo_cache (અક્ષર અચલ *file, સંરચના fileinfo *f,
                        સંરચના aclinfo *ai, પૂર્ણાંક flags)
{
  /* If UNSUPPORTED_SCONTEXT, these variables hold the
     st_dev and associated info for the most recently processed device
     for which file_has_aclinfo failed indicating lack of support.  */
  સ્થિર પૂર્ણાંક unsupported_return;
  સ્થિર અક્ષર *unsupported_scontext;
  સ્થિર પૂર્ણાંક unsupported_scontext_err;
  સ્થિર dev_t unsupported_device;

  જો (f->stat_ok && unsupported_scontext
      && f->stat.st_dev == unsupported_device)
    {
      ai->buf = ai->u._gl_acl_ch;
      ai->size = 0;
      ai->scontext = unsupported_scontext;
      ai->scontext_err = unsupported_scontext_err;
      errno = ENOTSUP;
      પાછા unsupported_return;
    }

  errno = 0;
  પૂર્ણાંક n = file_has_aclinfo (file, ai, flags);
  પૂર્ણાંક err = errno;
  જો (!USE_ACL)
    n = 0;
  /* A security-context error does not imply that ACLs are unsupported.
     Require an unsupported attribute listing too if ACLs or capability
     colors are needed.  */
  જો (f->stat_ok && n <= 0 && !acl_errno_valid (err)
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
  પાછા n;
}

/* Cache has_capability failure, when it's trivial to do.
   Like has_capability, but when F's st_dev says it's on a file
   system lacking capability support, return 0 with ENOTSUP immediately.  */
સ્થિર bool
has_capability_cache (અક્ષર અચલ *file, સંરચના fileinfo *f)
{
  /* If UNSUPPORTED_CACHED, UNSUPPORTED_DEVICE is the cached
     st_dev of the most recently processed device for which we've
     found that has_capability fails indicating lack of support.  */
  સ્થિર bool unsupported_cached;
  સ્થિર dev_t unsupported_device;

  જો (f->stat_ok && unsupported_cached && f->stat.st_dev == unsupported_device)
    {
      errno = ENOTSUP;
      પાછા 0;
    }

  bool b = has_capability (file);
  જો (f->stat_ok && !b && !acl_errno_valid (errno))
    {
      unsupported_cached = true;
      unsupported_device = f->stat.st_dev;
    }
  પાછા b;
}

/* Get the ACL, security context, and capability information needed for F.
   FULL_NAME names the file; DO_DEREF says whether to follow symlinks.  */
સ્થિર શૂન્ય
get_extended_info (અક્ષર અચલ *full_name, સંરચના fileinfo *f, bool do_deref)
{
  /* FIXME: file_has_aclinfo() conflates capability and acl xattr lookup
     with security context attributes.  Ideally we'd have separate
     ACL_GET_{CAPABILITY,ACL} flags to lookup just the required items,
     simplifying the logic here.  */

  ગણના filetype type = f->filetype;
  bool check_acl = USE_ACL && format == long_format;
  bool get_scontext = false;
#જો HAVE_SMACK || USE_SELINUX_SELINUX_H
  get_scontext = (format == long_format) | print_scontext;
#endif
  bool check_capability = format_needs_capability & (type == normal);

  જો (type != arg_directory && (check_acl | get_scontext | check_capability))
    {
      સંરચના aclinfo ai;
      /* Without ACL support, request contexts to obtain the attribute
         names needed for capability coloring too.  */
      પૂર્ણાંક aclinfo_flags = ((do_deref ? ACL_SYMLINK_FOLLOW : 0)
                           | (get_scontext || (!USE_ACL && check_capability)
                              ? ACL_GET_SCONTEXT : 0)
                           | filetype_d_type[type]);
      પૂર્ણાંક n = file_has_aclinfo_cache (full_name, f, &ai, aclinfo_flags);
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

      જો (format == long_format && n < 0 && !cannot_access_acl)
        error (0, errno, "%s", quotef (full_name));
      નહીંતર
        {
          /* When requesting security context information, don't make
             ls fail just because the file (even a command line argument)
             isn't on the right type of file system.  I.e., a getfilecon
             failure isn't in the same class as a stat failure.  */
          જો (print_scontext && ai.scontext_err
              && ! ignorable_traversal_errno (ai.scontext_err)
              && (! (is_ENOTSUP (ai.scontext_err)
                     || ai.scontext_err == ENODATA)))
            error (0, ai.scontext_err, "%s", quotef (full_name));
        }

      /* has_capability adds around 30% runtime to 'ls --color',
         so call it only if really needed.  Note capability coloring
         is disabled in the default color config.  */
      જો (check_capability && aclinfo_has_xattr (&ai, XATTR_NAME_CAPS))
        f->has_capability = has_capability_cache (full_name, f);

      f->scontext = ai.scontext;
      ai.scontext = ખાલી;
      aclinfo_free (&ai);
    }
}

સ્થિર bool
needs_quoting (અક્ષર અચલ *name)
{
  અક્ષર test[2];
  size_t len = quotearg_buffer (test, કદ test , name, -1,
                                filename_quoting_options);
  પાછા *name != *test || strlen (name) != len;
}

/* Add a file to the current table of files.
   Verify that the file exists, and print an error message if it does not.
   Return the number of blocks that the file occupies.  */
સ્થિર uintmax_t
gobble_file (અક્ષર અચલ *name, ગણના filetype type, ino_t inode,
             bool command_લીટી_arg, અક્ષર અચલ *dirname)
{
  uintmax_t blocks = 0;
  સંરચના fileinfo *f;

  /* An inode value prior to gobble_file necessarily came from readdir,
     which is not used for command line arguments.  */
  affirm (! command_લીટી_arg || inode == NOT_AN_INODE_સંખ્યા);

  જો (cwd_n_used == cwd_n_alloc)
    cwd_file = xpalloc (cwd_file, &cwd_n_alloc, 1, -1, કદ *cwd_file);

  f = &cwd_file[cwd_n_used];
  memset (f, '\0', કદ *f);
  f->stat.st_ino = inode;
  f->filetype = type;
  f->scontext = UNKNOWN_SECURITY_CONTEXT;

  f->quoted = -1;
  જો ((! cwd_some_quoted) && align_variable_outer_quotes)
    {
      /* Determine if any quoted for padding purposes.  */
      f->quoted = needs_quoting (name);
      જો (f->quoted)
        cwd_some_quoted = 1;
    }

  bool check_stat =
     (command_લીટી_arg
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
      || (print_inode && inode == NOT_AN_INODE_સંખ્યા)
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
  અક્ષર અચલ *full_name = name;
  જો (check_stat | print_scontext | format_needs_capability
      && name[0] != '/' && dirname)
    {
      અક્ષર *p = alloca (strlen (name) + strlen (dirname) + 2);
      attach (p, dirname, name);
      full_name = p;
    }

  bool do_deref;

  જો (!check_stat)
    do_deref = dereference == DEREF_ALWAYS;
  નહીંતર
    {
      પૂર્ણાંક err;

      જો (print_hyperlink)
        {
          f->absolute_name = canonicalize_filename_mode (full_name,
                                                         CAN_MISSING);
          જો (! f->absolute_name
              && (command_લીટી_arg
                  || ! ignorable_traversal_errno (errno)))
            file_failure (command_લીટી_arg,
                          _("error canonicalizing %s"), full_name);
        }

      પસંદગી (dereference)
        {
        સ્થિતિ DEREF_ALWAYS:
          err = do_stat (full_name, &f->stat);
          do_deref = true;
          તોડો;

        સ્થિતિ DEREF_COMMAND_લીટી_ARGUMENTS:
        સ્થિતિ DEREF_COMMAND_લીટી_SYMLINK_TO_DIR:
          જો (command_લીટી_arg)
            {
              bool need_lstat;
              err = do_stat (full_name, &f->stat);
              do_deref = true;

              જો (dereference == DEREF_COMMAND_લીટી_ARGUMENTS)
                તોડો;

              need_lstat = (err < 0
                            ? (errno == ENOENT || errno == ELOOP)
                            : ! S_ISDIR (f->stat.st_mode));
              જો (!need_lstat)
                તોડો;

              /* stat failed because of ENOENT || ELOOP, maybe indicating a
                 non-traversable symlink.  Or stat succeeded,
                 FULL_NAME does not refer to a directory,
                 and --dereference-command-line-symlink-to-dir is in effect.
                 Fall through so that we call lstat instead.  */
            }
          FALLTHROUGH;

        સ્થિતિ DEREF_NEVER:
          err = do_lstat (full_name, &f->stat);
          do_deref = false;
          તોડો;

        સ્થિતિ DEREF_UNDEFINED: મૂળભૂત:
          affirm (false);
        }

      જો (err != 0)
        {
          જો (! command_લીટી_arg && ignorable_traversal_errno (errno))
            {
              પૂર્ણાંક stat_errno = errno;
              સંરચના stat linkstat;
              bool dangling_link = (do_deref
                                    && (type == symbolic_link
                                        || type == unknown)
                                    && do_lstat (full_name, &linkstat) == 0);
              errno = stat_errno;
              જો (! dangling_link)
                {
                  free_ent (f);
                  પાછા 0;
                }
            }

          /* Failure to stat a command line argument leads to
             an exit status of 2.  For other files, stat failure
             provokes an exit status of 1.  */
          file_failure (command_લીટી_arg,
                        _("cannot access %s"), full_name);

          જો (command_લીટી_arg)
            {
              free_ent (f);
              પાછા 0;
            }

          f->name = xstrdup (name);
          cwd_n_used++;

          પાછા 0;
        }

      f->stat_ok = true;
      f->filetype = type = d_type_filetype[IFTODT (f->stat.st_mode)];
    }

  જો (type == directory && command_લીટી_arg && !immediate_dirs)
    f->filetype = type = arg_directory;

  get_extended_info (full_name, f, do_deref);

  જો ((type == symbolic_link)
      & ((format == long_format) | check_symlink_mode))
    {
      સંરચના stat linkstats;

      જો (! get_link_name (full_name, f, command_લીટી_arg))
        {
          free_ent (f);
          પાછા 0;
        }

      /* Use the slower quoting path for this entry, though
         don't update CWD_SOME_QUOTED since alignment not affected.  */
      જો (f->linkname && f->quoted == 0 && needs_quoting (f->linkname))
        f->quoted = -1;

      /* Avoid following symbolic links when possible, i.e., when
         they won't be traced and when no indicator is needed.  */
      જો (f->linkname
          && (file_type <= indicator_style || check_symlink_mode)
          && stat_for_mode (full_name, &linkstats) == 0)
        {
          f->linkok = true;
          f->linkmode = linkstats.st_mode;
        }
    }

  blocks = STP_NBLOCKS (&f->stat);
  જો (format == long_format || print_block_size)
    {
      અક્ષર buf[LONGEST_HUMAN_READABLE + 1];
      પૂર્ણાંક len = mbswidth (human_readable (blocks, buf, human_output_opts,
                                          ST_NBLOCKSIZE, output_block_size),
                          MBSWIDTH_FLAGS);
      જો (block_size_width < len)
        block_size_width = len;
    }

  જો (format == long_format)
    {
      જો (print_owner)
        {
          પૂર્ણાંક len = format_user_width (f->stat.st_uid);
          જો (owner_width < len)
            owner_width = len;
        }

      જો (print_group)
        {
          પૂર્ણાંક len = format_group_width (f->stat.st_gid);
          જો (group_width < len)
            group_width = len;
        }

      જો (print_author)
        {
          પૂર્ણાંક len = format_user_width (f->stat.st_author);
          જો (author_width < len)
            author_width = len;
        }
    }

  જો (print_scontext)
    {
      પૂર્ણાંક len = strlen (f->scontext);
      જો (scontext_width < len)
        scontext_width = len;
    }

  જો (format == long_format)
    {
      અક્ષર b[INT_BUFSIZE_BOUND (uintmax_t)];
      પૂર્ણાંક b_len = strlen (umaxtostr (f->stat.st_nlink, b));
      જો (nlink_width < b_len)
        nlink_width = b_len;

      જો ((type == chardev) | (type == blockdev))
        {
          અક્ષર buf[INT_BUFSIZE_BOUND (uintmax_t)];
          પૂર્ણાંક len = strlen (umaxtostr (major (f->stat.st_rdev), buf));
          જો (major_device_સંખ્યા_width < len)
            major_device_સંખ્યા_width = len;
          len = strlen (umaxtostr (minor (f->stat.st_rdev), buf));
          જો (minor_device_સંખ્યા_width < len)
            minor_device_સંખ્યા_width = len;
          len = major_device_સંખ્યા_width + 2 + minor_device_સંખ્યા_width;
          જો (file_size_width < len)
            file_size_width = len;
        }
      નહીંતર
        {
          અક્ષર buf[LONGEST_HUMAN_READABLE + 1];
          uintmax_t size = unsigned_file_size (f->stat.st_size);
          પૂર્ણાંક len = mbswidth (human_readable (size, buf,
                                              file_human_output_opts,
                                              1, file_output_block_size),
                              MBSWIDTH_FLAGS);
          જો (file_size_width < len)
            file_size_width = len;
        }
    }

  જો (print_inode)
    {
      અક્ષર buf[INT_BUFSIZE_BOUND (uintmax_t)];
      પૂર્ણાંક len = strlen (umaxtostr (f->stat.st_ino, buf));
      જો (inode_સંખ્યા_width < len)
        inode_સંખ્યા_width = len;
    }

  f->name = xstrdup (name);
  cwd_n_used++;

  પાછા blocks;
}

/* Return true if F refers to a directory.  */
સ્થિર bool
is_directory (અચલ સંરચના fileinfo *f)
{
  પાછા f->filetype == directory || f->filetype == arg_directory;
}

/* Return true if F refers to a (symlinked) directory.  */
સ્થિર bool
is_linked_directory (અચલ સંરચના fileinfo *f)
{
  પાછા f->filetype == directory || f->filetype == arg_directory
         || S_ISDIR (f->linkmode);
}

/* Put the name of the file that FILENAME is a symbolic link to
   into the LINKNAME field of 'f'.  COMMAND_LINE_ARG indicates whether
   FILENAME is a command-line argument.  */

સ્થિર bool
get_link_name (અક્ષર અચલ *filename, સંરચના fileinfo *f, bool command_લીટી_arg)
{
  f->linkname = areadlink_with_size (filename, f->stat.st_size);
  જો (f->linkname == ખાલી)
    {
      જો (! command_લીટી_arg
          && ignorable_traversal_errno (errno))
        પાછા false;
      file_failure (command_લીટી_arg, _("cannot read symbolic link %s"),
                    filename);
    }
  પાછા true;
}

/* Return true if the last component of NAME is '.' or '..'
   This is so we don't try to recurse on '././././. ...' */

સ્થિર bool
basename_is_dot_or_dotdot (અક્ષર અચલ *name)
{
  અક્ષર અચલ *base = last_component (name);
  પાછા dot_or_dotdot (base);
}

/* Remove any entries from CWD_FILE that are for directories,
   and queue them to be listed as directories instead.
   DIRNAME is the prefix to prepend to each dirname
   to make it correct relative to ls's working dir;
   if it is null, no prefix is needed and "." and ".." should not be ignored.
   If COMMAND_LINE_ARG is true, this directory was mentioned at the top level,
   This is desirable when processing directories recursively.  */

સ્થિર શૂન્ય
extract_dirs_from_files (અક્ષર અચલ *dirname, bool command_લીટી_arg)
{
  idx_t i, j;
  bool ignore_dot_and_dot_dot = (dirname != ખાલી);

  જો (dirname && LOOP_DETECT)
    {
      /* Insert a marker entry first.  When we dequeue this marker entry,
         we'll know that DIRNAME has been processed and may be removed
         from the set of active directories.  */
      queue_directory (ખાલી, dirname, false);
    }

  /* Queue the directories last one first, because queueing reverses the
     order.  */
  માટે (i = cwd_n_used; 0 < i; )
    {
      i--;
      સંરચના fileinfo *f = sorted_file[i];

      જો (is_directory (f)
          && (! ignore_dot_and_dot_dot
              || ! basename_is_dot_or_dotdot (f->name)))
        {
          જો (!dirname || f->name[0] == '/')
            queue_directory (f->name, f->linkname, command_લીટી_arg);
          નહીંતર
            {
              અક્ષર *name = file_name_concat (dirname, f->name, ખાલી);
              queue_directory (name, f->linkname, command_લીટી_arg);
              મુક્ત (name);
            }
          જો (f->filetype == arg_directory)
            free_ent (f);
        }
    }

  /* Now delete the directories from the table, compacting all the remaining
     entries.  */

  માટે (i = 0, j = 0; i < cwd_n_used; i++)
    {
      સંરચના fileinfo *f = sorted_file[i];
      sorted_file[j] = f;
      j += (f->filetype != arg_directory);
    }
  cwd_n_used = j;
}

/* Use strcoll to compare strings in this locale.  If an error occurs,
   report an error and longjmp to failed_strcoll.  */

સ્થિર jmp_buf failed_strcoll;

સ્થિર પૂર્ણાંક
xstrcoll (અક્ષર અચલ *a, અક્ષર અચલ *b)
{
  પૂર્ણાંક diff;
  errno = 0;
  diff = strcoll (a, b);
  જો (errno)
    {
      error (0, errno, _("cannot compare file names %s and %s"),
             quote_n (0, a), quote_n (1, b));
      set_exit_status (false);
      longjmp (failed_strcoll, 1);
    }
  પાછા diff;
}

/* Comparison routines for sorting the files.  */

પ્રકારવ્યાખ્યા શૂન્ય અચલ *V;
પ્રકારવ્યાખ્યા પૂર્ણાંક (*qsortFunc)(V a, V b);

/* Used below in DEFINE_SORT_FUNCTIONS for _df_ sort function variants.  */
સ્થિર પૂર્ણાંક
dirfirst_check (સંરચના fileinfo અચલ *a, સંરચના fileinfo અચલ *b,
                પૂર્ણાંક (*cmp) (V, V))
{
  પૂર્ણાંક diff = is_linked_directory (b) - is_linked_directory (a);
  પાછા diff ? diff : cmp (a, b);
}

/* Define the 8 different sort function variants required for each sortkey.
   KEY_NAME is a token describing the sort key, e.g., ctime, atime, size.
   KEY_CMP_FUNC is a function to compare records based on that key, e.g.,
   ctime_cmp, atime_cmp, size_cmp.  Append KEY_NAME to the string,
   '[rev_][x]str{cmp|coll}[_df]_', to create each function name.  */
#define DEFINE_SORT_FUNCTIONS(ચાવી_name, ચાવી_cmp_func)			\
  /* direct, non-dirfirst versions */					\
  સ્થિર પૂર્ણાંક xstrcoll_##key_name (V a, V b)				\
  { પાછા ચાવી_cmp_func (a, b, xstrcoll); }				\
  ATTRIBUTE_PURE સ્થિર પૂર્ણાંક strcmp_##key_name (V a, V b)		\
  { પાછા ચાવી_cmp_func (a, b, strcmp); }				\
                                                                        \
  /* reverse, non-dirfirst versions */					\
  સ્થિર પૂર્ણાંક rev_xstrcoll_##key_name (V a, V b)				\
  { પાછા ચાવી_cmp_func (b, a, xstrcoll); }				\
  ATTRIBUTE_PURE સ્થિર પૂર્ણાંક rev_strcmp_##key_name (V a, V b)	\
  { પાછા ચાવી_cmp_func (b, a, strcmp); }				\
                                                                        \
  /* direct, dirfirst versions */					\
  સ્થિર પૂર્ણાંક xstrcoll_df_##key_name (V a, V b)				\
  { પાછા dirfirst_check (a, b, xstrcoll_##key_name); }		\
  ATTRIBUTE_PURE સ્થિર પૂર્ણાંક strcmp_df_##key_name (V a, V b)		\
  { પાછા dirfirst_check (a, b, strcmp_##key_name); }			\
                                                                        \
  /* reverse, dirfirst versions */					\
  સ્થિર પૂર્ણાંક rev_xstrcoll_df_##key_name (V a, V b)			\
  { પાછા dirfirst_check (a, b, rev_xstrcoll_##key_name); }		\
  ATTRIBUTE_PURE સ્થિર પૂર્ણાંક rev_strcmp_df_##key_name (V a, V b)	\
  { પાછા dirfirst_check (a, b, rev_strcmp_##key_name); }

સ્થિર પૂર્ણાંક
cmp_ctime (સંરચના fileinfo અચલ *a, સંરચના fileinfo અચલ *b,
           પૂર્ણાંક (*cmp) (અક્ષર અચલ *, અક્ષર અચલ *))
{
  પૂર્ણાંક diff = timespec_cmp (get_stat_ctime (&b->stat),
                           get_stat_ctime (&a->stat));
  પાછા diff ? diff : cmp (a->name, b->name);
}

સ્થિર પૂર્ણાંક
cmp_mtime (સંરચના fileinfo અચલ *a, સંરચના fileinfo અચલ *b,
           પૂર્ણાંક (*cmp) (અક્ષર અચલ *, અક્ષર અચલ *))
{
  પૂર્ણાંક diff = timespec_cmp (get_stat_mtime (&b->stat),
                           get_stat_mtime (&a->stat));
  પાછા diff ? diff : cmp (a->name, b->name);
}

સ્થિર પૂર્ણાંક
cmp_atime (સંરચના fileinfo અચલ *a, સંરચના fileinfo અચલ *b,
           પૂર્ણાંક (*cmp) (અક્ષર અચલ *, અક્ષર અચલ *))
{
  પૂર્ણાંક diff = timespec_cmp (get_stat_atime (&b->stat),
                           get_stat_atime (&a->stat));
  પાછા diff ? diff : cmp (a->name, b->name);
}

સ્થિર પૂર્ણાંક
cmp_btime (સંરચના fileinfo અચલ *a, સંરચના fileinfo અચલ *b,
           પૂર્ણાંક (*cmp) (અક્ષર અચલ *, અક્ષર અચલ *))
{
  પૂર્ણાંક diff = timespec_cmp (get_stat_btime (&b->stat),
                           get_stat_btime (&a->stat));
  પાછા diff ? diff : cmp (a->name, b->name);
}

સ્થિર પૂર્ણાંક
off_cmp (off_t a, off_t b)
{
  પાછા _GL_CMP (a, b);
}

સ્થિર પૂર્ણાંક
cmp_size (સંરચના fileinfo અચલ *a, સંરચના fileinfo અચલ *b,
          પૂર્ણાંક (*cmp) (અક્ષર અચલ *, અક્ષર અચલ *))
{
  પૂર્ણાંક diff = off_cmp (b->stat.st_size, a->stat.st_size);
  પાછા diff ? diff : cmp (a->name, b->name);
}

સ્થિર પૂર્ણાંક
cmp_name (સંરચના fileinfo અચલ *a, સંરચના fileinfo અચલ *b,
          પૂર્ણાંક (*cmp) (અક્ષર અચલ *, અક્ષર અચલ *))
{
  પાછા cmp (a->name, b->name);
}

/* Compare file extensions.  Files with no extension are 'smallest'.
   If extensions are the same, compare by file names instead.  */

સ્થિર પૂર્ણાંક
cmp_extension (સંરચના fileinfo અચલ *a, સંરચના fileinfo અચલ *b,
               પૂર્ણાંક (*cmp) (અક્ષર અચલ *, અક્ષર અચલ *))
{
  અક્ષર અચલ *base1 = strrchr (a->name, '.');
  અક્ષર અચલ *base2 = strrchr (b->name, '.');
  પૂર્ણાંક diff = cmp (base1 ? base1 : "", base2 ? base2 : "");
  પાછા diff ? diff : cmp (a->name, b->name);
}

/* Return the (cached) screen width,
   for the NAME associated with the passed fileinfo F.  */

સ્થિર size_t
fileinfo_name_width (સંરચના fileinfo અચલ *f)
{
  પાછા f->width
         ? f->width
         : quote_name_width (f->name, filename_quoting_options, f->quoted);
}

સ્થિર પૂર્ણાંક
cmp_width (સંરચના fileinfo અચલ *a, સંરચના fileinfo અચલ *b,
          પૂર્ણાંક (*cmp) (અક્ષર અચલ *, અક્ષર અચલ *))
{
  પૂર્ણાંક diff = fileinfo_name_width (a) - fileinfo_name_width (b);
  પાછા diff ? diff : cmp (a->name, b->name);
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
સ્થિર પૂર્ણાંક
cmp_version (સંરચના fileinfo અચલ *a, સંરચના fileinfo અચલ *b)
{
  પૂર્ણાંક diff = filevercmp (a->name, b->name);
  પાછા diff ? diff : strcmp (a->name, b->name);
}

સ્થિર પૂર્ણાંક
xstrcoll_version (V a, V b)
{
  પાછા cmp_version (a, b);
}
સ્થિર પૂર્ણાંક
rev_xstrcoll_version (V a, V b)
{
  પાછા cmp_version (b, a);
}
સ્થિર પૂર્ણાંક
xstrcoll_df_version (V a, V b)
{
  પાછા dirfirst_check (a, b, xstrcoll_version);
}
સ્થિર પૂર્ણાંક
rev_xstrcoll_df_version (V a, V b)
{
  પાછા dirfirst_check (a, b, rev_xstrcoll_version);
}


/* We have 2^3 different variants for each sort-key function
   (for 3 independent sort modes).
   The function pointers stored in this array must be dereferenced as:

    sort_variants[sort_key][use_strcmp][reverse][dirs_first]

   Note that the order in which sort keys are listed in the function pointer
   array below is defined by the order of the elements in the time_type and
   sort_type enums!  */

#define LIST_SORTFUNCTION_VARIANTS(ચાવી_name)                        \
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

સ્થિર qsortFunc અચલ sort_functions[][2][2][2] =
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
        { ખાલી, ખાલી },
        { ખાલી, ખાલી },
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

સ્થિર શૂન્ય
initialize_ordering_vector (શૂન્ય)
{
  માટે (idx_t i = 0; i < cwd_n_used; i++)
    sorted_file[i] = &cwd_file[i];
}

/* Cache values based on attributes global to all files.  */

સ્થિર શૂન્ય
update_current_files_info (શૂન્ય)
{
  /* Cache screen width of name, if needed multiple times.  */
  જો (sort_type == sort_width
      || (લીટી_length && (format == many_per_લીટી || format == horizontal)))
    {
      માટે (idx_t i = 0; i < cwd_n_used; i++)
        {
          સંરચના fileinfo *f = sorted_file[i];
          f->width = fileinfo_name_width (f);
        }
    }
}

/* Sort the files now in the table.  */

સ્થિર શૂન્ય
sort_files (શૂન્ય)
{
  bool use_strcmp;

  જો (sorted_file_alloc < cwd_n_used + (cwd_n_used >> 1))
    {
      મુક્ત (sorted_file);
      sorted_file = xinmalloc (cwd_n_used, 3 * કદ *sorted_file);
      sorted_file_alloc = 3 * cwd_n_used;
    }

  initialize_ordering_vector ();

  update_current_files_info ();

  જો (sort_type == sort_none)
    પાછા;

  /* Try strcoll.  If it fails, fall back on strcmp.  We can't safely
     ignore strcoll failures, as a failing strcoll might be a
     comparison function that is not a total order, and if we ignored
     the failure this might cause qsort to dump core.  */

  જો (! setjmp (failed_strcoll))
    use_strcmp = false;      /* strcoll() succeeded */
  નહીંતર
    {
      use_strcmp = true;
      affirm (sort_type != sort_version);
      initialize_ordering_vector ();
    }

  /* When sort_type == sort_time, use time_type as subindex.  */
  mpsort ((શૂન્ય અચલ **) sorted_file, cwd_n_used,
          sort_functions[sort_type + (sort_type == sort_time ? time_type : 0)]
                        [use_strcmp][sort_reverse]
                        [directories_first]);
}

/* List all the files now in the table.  */

સ્થિર શૂન્ય
print_current_files (શૂન્ય)
{
  પસંદગી (format)
    {
    સ્થિતિ one_per_લીટી:
      માટે (idx_t i = 0; i < cwd_n_used; i++)
        {
          print_file_name_and_frills (sorted_file[i], 0);
          putchar (eolbyte);
        }
      તોડો;

    સ્થિતિ many_per_લીટી:
      જો (! લીટી_length)
        print_with_separator (' ');
      નહીંતર
        print_many_per_લીટી ();
      તોડો;

    સ્થિતિ horizontal:
      જો (! લીટી_length)
        print_with_separator (' ');
      નહીંતર
        print_horizontal ();
      તોડો;

    સ્થિતિ with_commas:
      print_with_separator (',');
      તોડો;

    સ્થિતિ long_format:
      માટે (idx_t i = 0; i < cwd_n_used; i++)
        {
          set_normal_color ();
          print_long_format (sorted_file[i]);
          dired_outbyte (eolbyte);
        }
      તોડો;
    }
}

/* Replace the first %b with precomputed aligned month names.
   Note on glibc-2.7 at least, this speeds up the whole 'ls -lU'
   process by around 17%, compared to letting strftime() handle the %b.  */

સ્થિર ptrdiff_t
align_nstrftime (અક્ષર *buf, size_t size, bool recent, સંરચના tm અચલ *tm,
                 timezone_t tz, પૂર્ણાંક ns)
{
  અક્ષર અચલ *nfmt = (use_abformat
                      ? abformat[recent][tm->tm_mon]
                      : long_time_format[recent]);
  પાછા nstrftime (buf, size, nfmt, tm, tz, ns);
}

/* Return the expected number of columns in a long-format timestamp,
   or zero if it cannot be calculated.  */

સ્થિર પૂર્ણાંક
long_time_expected_width (શૂન્ય)
{
  સ્થિર પૂર્ણાંક width = -1;

  જો (width < 0)
    {
      time_t epoch = 0;
      સંરચના tm tm;
      અક્ષર buf[TIME_STAMP_LEN_MAXIMUM + 1];

      /* In case you're wondering if localtime_rz can fail with an input time_t
         value of 0, let's just say it's very unlikely, but not inconceivable.
         The TZ environment variable would have to specify a time zone that
         is 2**31-1900 years or more ahead of UTC.  This could happen only on
         a 64-bit system that blindly accepts e.g., TZ=UTC+20000000000000.
         However, this is not possible with Solaris 10 or glibc-2.3.5, since
         their implementations limit the offset to 167:59 and 24:00, resp.  */
      જો (localtime_rz (localtz, &epoch, &tm))
        {
          ptrdiff_t len = align_nstrftime (buf, કદ buf, false,
                                           &tm, localtz, 0);
          જો (len > 0)
            width = mbsnwidth (buf, len, MBSWIDTH_FLAGS);
        }

      જો (width < 0)
        width = 0;
    }

  પાછા width;
}

/* Print the user or group name NAME, with numeric id ID, using a
   print width of WIDTH columns.  */

સ્થિર શૂન્ય
format_user_or_group (અક્ષર અચલ *name, uintmax_t id, પૂર્ણાંક width)
{
  જો (name)
    {
      પૂર્ણાંક name_width = mbswidth (name, MBSWIDTH_FLAGS);
      પૂર્ણાંક width_gap = name_width < 0 ? 0 : width - name_width;
      પૂર્ણાંક pad = MAX (0, width_gap);
      dired_outstring (name);

      કરો
        dired_outbyte (' ');
      જ્યારે (pad--);
    }
  નહીંતર
    dired_pos += printf ("%*ju ", width, id);
}

/* Print the name or id of the user with id U, using a print width of
   WIDTH.  */

સ્થિર શૂન્ય
format_user (uid_t u, પૂર્ણાંક width, bool stat_ok)
{
  format_user_or_group (! stat_ok ? "?" :
                        (numeric_ids ? ખાલી : getuser (u)), u, width);
}

/* Likewise, for groups.  */

સ્થિર શૂન્ય
format_group (gid_t g, પૂર્ણાંક width, bool stat_ok)
{
  format_user_or_group (! stat_ok ? "?" :
                        (numeric_ids ? ખાલી : getgroup (g)), g, width);
}

/* Return the number of columns that format_user_or_group will print,
   or -1 if unknown.  */

સ્થિર પૂર્ણાંક
format_user_or_group_width (અક્ષર અચલ *name, uintmax_t id)
{
  પાછા (name
          ? mbswidth (name, MBSWIDTH_FLAGS)
          : snprintf (ખાલી, 0, "%ju", id));
}

/* Return the number of columns that format_user will print,
   or -1 if unknown.  */

સ્થિર પૂર્ણાંક
format_user_width (uid_t u)
{
  પાછા format_user_or_group_width (numeric_ids ? ખાલી : getuser (u), u);
}

/* Likewise, for groups.  */

સ્થિર પૂર્ણાંક
format_group_width (gid_t g)
{
  પાછા format_user_or_group_width (numeric_ids ? ખાલી : getgroup (g), g);
}

/* Return a pointer to a formatted version of F->stat.st_ino,
   possibly using buffer, which must be at least
   INT_BUFSIZE_BOUND (uintmax_t) bytes.  */
સ્થિર અક્ષર *
format_inode (અક્ષર buf[INT_BUFSIZE_BOUND (uintmax_t)],
              અચલ સંરચના fileinfo *f)
{
  પાછા (f->stat_ok && f->stat.st_ino != NOT_AN_INODE_સંખ્યા
          ? umaxtostr (f->stat.st_ino, buf)
          : (અક્ષર *) "?");
}

/* Print information about F in long format.  */
સ્થિર શૂન્ય
print_long_format (અચલ સંરચના fileinfo *f)
{
  અક્ષર modebuf[12];
  અક્ષર buf
    [LONGEST_HUMAN_READABLE + 1		/* inode */
     + LONGEST_HUMAN_READABLE + 1	/* size in blocks */
     + કદ (modebuf) - 1 + 1		/* mode string */
     + INT_BUFSIZE_BOUND (uintmax_t)	/* st_nlink */
     + LONGEST_HUMAN_READABLE + 2	/* major device number */
     + LONGEST_HUMAN_READABLE + 1	/* minor device number */
     + TIME_STAMP_LEN_MAXIMUM + 1	/* max length of time/date */
     ];
  ptrdiff_t s;
  અક્ષર *p;
  સંરચના timespec when_timespec;
  સંરચના tm when_local;
  bool btime_ok = true;

  /* Compute the mode string, except remove the trailing space if no
     file in this directory has an ACL or security context.  */
  જો (f->stat_ok)
    filemodestring (&f->stat, modebuf);
  નહીંતર
    {
      modebuf[0] = filetype_letter[f->filetype];
      memset (modebuf + 1, '?', 10);
      modebuf[11] = '\0';
    }
  જો (! any_has_acl)
    modebuf[10] = '\0';
  નહીંતર જો (f->acl_type == ACL_T_LSM_CONTEXT_ONLY)
    modebuf[10] = '.';
  નહીંતર જો (f->acl_type == ACL_T_YES)
    modebuf[10] = '+';
  નહીંતર જો (f->acl_type == ACL_T_UNKNOWN)
    modebuf[10] = '?';

  પસંદગી (time_type)
    {
    સ્થિતિ time_ctime:
      when_timespec = get_stat_ctime (&f->stat);
      તોડો;
    સ્થિતિ time_mtime:
      when_timespec = get_stat_mtime (&f->stat);
      તોડો;
    સ્થિતિ time_atime:
      when_timespec = get_stat_atime (&f->stat);
      તોડો;
    સ્થિતિ time_btime:
      when_timespec = get_stat_btime (&f->stat);
      જો (when_timespec.tv_sec == -1 && when_timespec.tv_nsec == -1)
        btime_ok = false;
      તોડો;
    સ્થિતિ time_numtypes: મૂળભૂત:
      unreachable ();
    }

  p = buf;

  જો (print_inode)
    {
      અક્ષર hbuf[INT_BUFSIZE_BOUND (uintmax_t)];
      p += sprintf (p, "%*s ", inode_સંખ્યા_width, format_inode (hbuf, f));
    }

  જો (print_block_size)
    {
      અક્ષર hbuf[LONGEST_HUMAN_READABLE + 1];
      અક્ષર અચલ *blocks =
        (! f->stat_ok
         ? "?"
         : human_readable (STP_NBLOCKS (&f->stat), hbuf, human_output_opts,
                           ST_NBLOCKSIZE, output_block_size));
      પૂર્ણાંક blocks_width = mbswidth (blocks, MBSWIDTH_FLAGS);
      માટે (પૂર્ણાંક pad = blocks_width < 0 ? 0 : block_size_width - blocks_width;
           0 < pad; pad--)
        *p++ = ' ';
      જ્યારે ((*p++ = *blocks++))
        ચાલુ;
      p[-1] = ' ';
    }

  /* The last byte of the mode string is the POSIX
     "optional alternate access method flag".  */
  {
    અક્ષર hbuf[INT_BUFSIZE_BOUND (uintmax_t)];
    p += sprintf (p, "%s %*s ", modebuf, nlink_width,
                  ! f->stat_ok ? "?" : umaxtostr (f->stat.st_nlink, hbuf));
  }

  dired_indent ();

  જો (print_owner || print_group || print_author || print_scontext)
    {
      dired_outbuf (buf, p - buf);

      જો (print_owner)
        format_user (f->stat.st_uid, owner_width, f->stat_ok);

      જો (print_group)
        format_group (f->stat.st_gid, group_width, f->stat_ok);

      જો (print_author)
        format_user (f->stat.st_author, author_width, f->stat_ok);

      જો (print_scontext)
        format_user_or_group (f->scontext, 0, scontext_width);

      p = buf;
    }

  જો (f->stat_ok
      && (S_ISCHR (f->stat.st_mode) || S_ISBLK (f->stat.st_mode)))
    {
      પૂર્ણાંક blanks_width = (file_size_width
                          - (major_device_સંખ્યા_width + 2
                             + minor_device_સંખ્યા_width));
      p += sprintf (p, "%*ju, %*ju ",
                    major_device_સંખ્યા_width + MAX (0, blanks_width),
                    (uintmax_t) major (f->stat.st_rdev),
                    minor_device_સંખ્યા_width,
                    (uintmax_t) minor (f->stat.st_rdev));
    }
  નહીંતર
    {
      અક્ષર hbuf[LONGEST_HUMAN_READABLE + 1];
      અક્ષર અચલ *size =
        (! f->stat_ok
         ? "?"
         : human_readable (unsigned_file_size (f->stat.st_size),
                           hbuf, file_human_output_opts, 1,
                           file_output_block_size));
      પૂર્ણાંક size_width = mbswidth (size, MBSWIDTH_FLAGS);
      માટે (પૂર્ણાંક pad = size_width < 0 ? 0 : file_size_width - size_width;
           0 < pad; pad--)
        *p++ = ' ';
      જ્યારે ((*p++ = *size++))
        ચાલુ;
      p[-1] = ' ';
    }

  s = -1;

  જો (f->stat_ok && btime_ok
      && localtime_rz (localtz, &when_timespec.tv_sec, &when_local))
    {
      સંરચના timespec six_months_ago;
      bool recent;

      /* If the file appears to be in the future, update the current
         time, in case the file happens to have been modified since
         the last time we checked the clock.  */
      જો (timespec_cmp (current_time, when_timespec) < 0)
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

  જો (0 <= s)
    {
      p += s;
      *p++ = ' ';
    }
  નહીંતર
    {
      /* The time cannot be converted using the desired format, so
         print it as a huge integer number of seconds.  */
      અક્ષર hbuf[INT_BUFSIZE_BOUND (intmax_t)];
      p += sprintf (p, "%*s ", long_time_expected_width (),
                    (! f->stat_ok || ! btime_ok
                     ? "?"
                     : timetostr (when_timespec.tv_sec, hbuf)));
      /* FIXME: (maybe) We discarded when_timespec.tv_nsec. */
    }

  dired_outbuf (buf, p - buf);
  size_t w = print_name_with_quoting (f, false, &dired_obstack, p - buf);

  જો (f->filetype == symbolic_link)
    {
      જો (f->linkname)
        {
          dired_outstring (" -> ");
          print_name_with_quoting (f, true, ખાલી, (p - buf) + w + 4);
          જો (indicator_style != none)
            print_type_indicator (true, f->linkmode, unknown);
        }
    }
  નહીંતર જો (indicator_style != none)
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

સ્થિર size_t
quote_name_buf (અક્ષર **inbuf, size_t bufsize, અક્ષર *name,
                સંરચના quoting_options અચલ *options,
                પૂર્ણાંક needs_general_quoting, size_t *width, bool *pad)
{
  અક્ષર *buf = *inbuf;
  size_t displayed_width IF_LINT ( = 0);
  size_t len = 0;
  bool quoted;

  ગણના quoting_style qs = get_quoting_style (options);
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

  જો (needs_general_quoting != 0)
    {
      len = quotearg_buffer (buf, bufsize, name, -1, options);
      જો (bufsize <= len)
        {
          buf = xmalloc (len + 1);
          quotearg_buffer (buf, len + 1, name, -1, options);
        }

      quoted = (*name != *buf) || strlen (name) != len;
    }
  નહીંતર જો (needs_further_quoting || extra_newline_protection)
    {
      len = strlen (name);
      જો (bufsize <= len)
        buf = xmalloc (len + 1);
      memcpy (buf, name, len + 1);

      quoted = false;
    }
  નહીંતર
    {
      len = strlen (name);
      buf = name;
      quoted = false;
    }

  /* It's safe to replace newlines without multi-byte iteration, as newlines
     do not appear as part of any multi-byte encoded character.  */
  જો (extra_newline_protection)
    માટે (અક્ષર *p = buf; p < buf + len; p++)
      જો (eolbyte && *p == '\n')
        *p = '?';

  જો (needs_further_quoting)
    {
      જો (MB_CUR_MAX > 1)
        {
          અક્ષર અચલ *p = buf;
          અક્ષર અચલ *plimit = buf + len;
          અક્ષર *q = buf;
          displayed_width = 0;

          જ્યારે (p < plimit)
            પસંદગી (*p)
              {
                સ્થિતિ ' ': સ્થિતિ '!': સ્થિતિ '"': સ્થિતિ '#': સ્થિતિ '%':
                સ્થિતિ '&': સ્થિતિ '\'': સ્થિતિ '(': સ્થિતિ ')': સ્થિતિ '*':
                સ્થિતિ '+': સ્થિતિ ',': સ્થિતિ '-': સ્થિતિ '.': સ્થિતિ '/':
                સ્થિતિ '0': સ્થિતિ '1': સ્થિતિ '2': સ્થિતિ '3': સ્થિતિ '4':
                સ્થિતિ '5': સ્થિતિ '6': સ્થિતિ '7': સ્થિતિ '8': સ્થિતિ '9':
                સ્થિતિ ':': સ્થિતિ ';': સ્થિતિ '<': સ્થિતિ '=': સ્થિતિ '>':
                સ્થિતિ '?':
                સ્થિતિ 'A': સ્થિતિ 'B': સ્થિતિ 'C': સ્થિતિ 'D': સ્થિતિ 'E':
                સ્થિતિ 'F': સ્થિતિ 'G': સ્થિતિ 'H': સ્થિતિ 'I': સ્થિતિ 'J':
                સ્થિતિ 'K': સ્થિતિ 'L': સ્થિતિ 'M': સ્થિતિ 'N': સ્થિતિ 'O':
                સ્થિતિ 'P': સ્થિતિ 'Q': સ્થિતિ 'R': સ્થિતિ 'S': સ્થિતિ 'T':
                સ્થિતિ 'U': સ્થિતિ 'V': સ્થિતિ 'W': સ્થિતિ 'X': સ્થિતિ 'Y':
                સ્થિતિ 'Z':
                સ્થિતિ '[': સ્થિતિ '\\': સ્થિતિ ']': સ્થિતિ '^': સ્થિતિ '_':
                સ્થિતિ 'a': સ્થિતિ 'b': સ્થિતિ 'c': સ્થિતિ 'd': સ્થિતિ 'e':
                સ્થિતિ 'f': સ્થિતિ 'g': સ્થિતિ 'h': સ્થિતિ 'i': સ્થિતિ 'j':
                સ્થિતિ 'k': સ્થિતિ 'l': સ્થિતિ 'm': સ્થિતિ 'n': સ્થિતિ 'o':
                સ્થિતિ 'p': સ્થિતિ 'q': સ્થિતિ 'r': સ્થિતિ 's': સ્થિતિ 't':
                સ્થિતિ 'u': સ્થિતિ 'v': સ્થિતિ 'w': સ્થિતિ 'x': સ્થિતિ 'y':
                સ્થિતિ 'z': સ્થિતિ '{': સ્થિતિ '|': સ્થિતિ '}': સ્થિતિ '~':
                  /* These characters are printable ASCII characters.  */
                  *q++ = *p++;
                  displayed_width += 1;
                  તોડો;
                મૂળભૂત:
                  /* If we have a multibyte sequence, copy it until we
                     reach its end, replacing each non-printable multibyte
                     character with a single question mark.  */
                  {
                    mbstate_t mbstate; mbszero (&mbstate);
                    કરો
                      {
                        char32_t wc;
                        size_t bytes;
                        પૂર્ણાંક w;

                        bytes = mbrtoc32 (&wc, p, plimit - p, &mbstate);

                        જો (bytes == (size_t) -1)
                          {
                            /* An invalid multibyte sequence was
                               encountered.  Skip one input byte, and
                               put a question mark.  */
                            p++;
                            *q++ = '?';
                            displayed_width += 1;
                            તોડો;
                          }

                        જો (bytes == (size_t) -2)
                          {
                            /* An incomplete multibyte character
                               at the end.  Replace it entirely with
                               a question mark.  */
                            p = plimit;
                            *q++ = '?';
                            displayed_width += 1;
                            તોડો;
                          }

                        જો (bytes == 0)
                          /* A null wide character was encountered.  */
                          bytes = 1;

                        w = c32width (wc);
                        જો (w >= 0)
                          {
                            /* A printable multibyte character.
                               Keep it.  */
                            માટે (; bytes > 0; --bytes)
                              *q++ = *p++;
                            displayed_width += w;
                          }
                        નહીંતર
                          {
                            /* An nonprintable multibyte character.
                               Replace it entirely with a question
                               mark.  */
                            p += bytes;
                            *q++ = '?';
                            displayed_width += 1;
                          }
                      }
                    જ્યારે (! mbsinit (&mbstate));
                  }
                  તોડો;
              }

          /* The buffer may have shrunk.  */
          len = q - buf;
        }
      નહીંતર
        {
          અક્ષર *p = buf;
          અક્ષર અચલ *plimit = buf + len;

          જ્યારે (p < plimit)
            {
              જો (! isprint (to_uchar (*p)))
                *p = '?';
              p++;
            }
          displayed_width = len;
        }
    }
  નહીંતર જો (width != ખાલી)
    {
      જો (MB_CUR_MAX > 1)
        {
          displayed_width = mbsnwidth (buf, len, MBSWIDTH_FLAGS);
          displayed_width = MAX (0, displayed_width);
        }
      નહીંતર
        {
          અક્ષર અચલ *p = buf;
          અક્ષર અચલ *plimit = buf + len;

          displayed_width = 0;
          જ્યારે (p < plimit)
            {
              જો (isprint (to_uchar (*p)))
                displayed_width++;
              p++;
            }
        }
    }

  /* Set padding to better align quoted items,
     and also give a visual indication that quotes are
     not actually part of the name.  */
  *pad = (align_variable_outer_quotes && cwd_some_quoted && ! quoted);

  જો (width != ખાલી)
    *width = displayed_width;

  *inbuf = buf;

  પાછા len;
}

સ્થિર size_t
quote_name_width (અક્ષર અચલ *name, સંરચના quoting_options અચલ *options,
                  પૂર્ણાંક needs_general_quoting)
{
  અક્ષર smallbuf[BUFSIZ];
  અક્ષર *buf = smallbuf;
  size_t width;
  bool pad;

  quote_name_buf (&buf, કદ smallbuf, (અક્ષર *) name, options,
                  needs_general_quoting, &width, &pad);

  જો (buf != smallbuf && buf != name)
    મુક્ત (buf);

  width += pad;

  પાછા width;
}

/* %XX escape any input out of range as defined in RFC3986,
   and also if PATH, convert all path separators to '/'.  */
સ્થિર અક્ષર *
file_escape (અક્ષર અચલ *str, bool path)
{
  અક્ષર *esc = xnmalloc (3, strlen (str) + 1);
  અક્ષર *p = esc;
  જ્યારે (*str)
    {
      જો (path && ISSLASH (*str))
        {
          *p++ = '/';
          str++;
        }
      નહીંતર જો (RFC3986[to_uchar (*str)])
        *p++ = *str++;
      નહીંતર
        p += sprintf (p, "%%%02x", to_uchar (*str++));
    }
  *p = '\0';
  પાછા esc;
}

સ્થિર size_t
quote_name (અક્ષર અચલ *name, સંરચના quoting_options અચલ *options,
            પૂર્ણાંક needs_general_quoting, અચલ સંરચના bin_str *color,
            bool allow_pad, સંરચના obstack *ઢગલો, અક્ષર અચલ *absolute_name)
{
  અક્ષર smallbuf[BUFSIZ];
  અક્ષર *buf = smallbuf;
  size_t len;
  bool pad;

  len = quote_name_buf (&buf, કદ smallbuf, (અક્ષર *) name, options,
                        needs_general_quoting, ખાલી, &pad);

  જો (pad && allow_pad)
    dired_outbyte (' ');

  જો (color)
    print_color_indicator (color);

  /* If we're padding, then don't include the outer quotes in
     the --hyperlink, to improve the alignment of those links.  */
  bool skip_quotes = false;

  જો (absolute_name)
    {
      જો (align_variable_outer_quotes && cwd_some_quoted && ! pad)
        {
          skip_quotes = true;
          putchar (*buf);
        }
      અક્ષર *h = file_escape (hostname, /* path= */ false);
      અક્ષર *n = file_escape (absolute_name, /* path= */ true);
      /* TODO: It would be good to be able to define parameters
         to give hints to the terminal as how best to render the URI.
         For example since ls is outputting a dense block of URIs
         it would be best to not underline by default, and only
         do so upon hover etc.  */
      printf ("\033]8;;file://%s%s%s\033\\", h, *n == '/' ? "" : "/", n);
      મુક્ત (h);
      મુક્ત (n);
    }

  જો (ઢગલો)
    ધકેલો_current_dired_pos (ઢગલો);

  fwrite (buf + skip_quotes, 1, len - (skip_quotes * 2), stdout);

  dired_pos += len;

  જો (ઢગલો)
    ધકેલો_current_dired_pos (ઢગલો);

  જો (absolute_name)
    {
      fputs ("\033]8;;\033\\", stdout);
      જો (skip_quotes)
        putchar (*(buf + len - 1));
    }

  જો (buf != smallbuf && buf != name)
    મુક્ત (buf);

  પાછા len + pad;
}

સ્થિર size_t
print_name_with_quoting (અચલ સંરચના fileinfo *f,
                         bool symlink_target,
                         સંરચના obstack *ઢગલો,
                         size_t start_col)
{
  અક્ષર અચલ *name = symlink_target ? f->linkname : f->name;

  અચલ સંરચના bin_str *color
    = print_with_color ? get_color_indicator (f, symlink_target) : ખાલી;

  bool used_color_this_time = (print_with_color
                               && (color || is_colored (C_NORM)));

  size_t len = quote_name (name, filename_quoting_options, f->quoted,
                           color, !symlink_target, ઢગલો, f->absolute_name);

  process_signals ();
  જો (used_color_this_time)
    {
      prep_non_filename_text ();

      /* We use the byte length rather than display width here as
         an optimization to avoid accurately calculating the width,
         because we only output the clear to EOL sequence if the name
         _might_ wrap to the next line.  This may output a sequence
         unnecessarily in multi-byte locales for example,
         but in that case it's inconsequential to the output.  */
      જો (લીટી_length
          && (start_col / લીટી_length != (start_col + len - 1) / લીટી_length))
        put_indicator (&color_indicator[C_CLR_TO_EOL]);
    }

  પાછા len;
}

સ્થિર શૂન્ય
prep_non_filename_text (શૂન્ય)
{
  જો (color_indicator[C_END].string != ખાલી)
    put_indicator (&color_indicator[C_END]);
  નહીંતર
    {
      put_indicator (&color_indicator[C_ડાબું]);
      put_indicator (&color_indicator[C_RESET]);
      put_indicator (&color_indicator[C_જમણું]);
    }
}

/* Print the file name of 'f' with appropriate quoting.
   Also print file size, inode number, and filetype indicator character,
   as requested by switches.  */

સ્થિર size_t
print_file_name_and_frills (અચલ સંરચના fileinfo *f, size_t start_col)
{
  અક્ષર buf[MAX (LONGEST_HUMAN_READABLE + 1, INT_BUFSIZE_BOUND (uintmax_t))];

  set_normal_color ();

  જો (print_inode)
    printf ("%*s ", format == with_commas ? 0 : inode_સંખ્યા_width,
            format_inode (buf, f));

  જો (print_block_size)
    {
      અક્ષર અચલ *blocks =
        (! f->stat_ok
         ? "?"
         : human_readable (STP_NBLOCKS (&f->stat), buf, human_output_opts,
                           ST_NBLOCKSIZE, output_block_size));
      પૂર્ણાંક blocks_width = mbswidth (blocks, MBSWIDTH_FLAGS);
      પૂર્ણાંક pad = 0;
      જો (0 <= blocks_width && block_size_width && format != with_commas)
        pad = block_size_width - blocks_width;
      printf ("%*s%s ", pad, "", blocks);
    }

  જો (print_scontext)
    printf ("%*s ", format == with_commas ? 0 : scontext_width, f->scontext);

  size_t width = print_name_with_quoting (f, false, ખાલી, start_col);

  જો (indicator_style != none)
    width += print_type_indicator (f->stat_ok, f->stat.st_mode, f->filetype);

  પાછા width;
}

/* Given these arguments describing a file, return the single-byte
   type indicator, or 0.  */
સ્થિર અક્ષર
get_type_indicator (bool stat_ok, mode_t mode, ગણના filetype type)
{
  અક્ષર c;

  જો (stat_ok ? S_ISREG (mode) : type == normal)
    {
      જો (stat_ok && indicator_style == classify && (mode & S_IXUGO))
        c = '*';
      નહીંતર
        c = 0;
    }
  નહીંતર
    {
      જો (stat_ok ? S_ISDIR (mode) : type == directory || type == arg_directory)
        c = '/';
      નહીંતર જો (indicator_style == slash)
        c = 0;
      નહીંતર જો (stat_ok ? S_ISLNK (mode) : type == symbolic_link)
        c = '@';
      નહીંતર જો (stat_ok ? S_ISFIFO (mode) : type == fifo)
        c = '|';
      નહીંતર જો (stat_ok ? S_ISSOCK (mode) : type == sock)
        c = '=';
      નહીંતર જો (stat_ok && S_ISDOOR (mode))
        c = '>';
      નહીંતર
        c = 0;
    }
  પાછા c;
}

સ્થિર bool
print_type_indicator (bool stat_ok, mode_t mode, ગણના filetype type)
{
  અક્ષર c = get_type_indicator (stat_ok, mode, type);
  જો (c)
    dired_outbyte (c);
  પાછા !!c;
}

/* Returns if color sequence was printed.  */
સ્થિર bool
print_color_indicator (અચલ સંરચના bin_str *ind)
{
  જો (ind)
    {
      /* Need to reset so not dealing with attribute combinations */
      જો (is_colored (C_NORM))
        restore_default_color ();
      put_indicator (&color_indicator[C_ડાબું]);
      put_indicator (ind);
      put_indicator (&color_indicator[C_જમણું]);
    }

  પાછા ind != ખાલી;
}

/* Returns color indicator or NULL if none.  */
ATTRIBUTE_PURE
સ્થિર અચલ સંરચના bin_str *
get_color_indicator (અચલ સંરચના fileinfo *f, bool symlink_target)
{
  ગણના indicator_no type;
  સંરચના color_ext_type *ext;	/* Color extension */
  size_t len;			/* Length of name */

  અક્ષર અચલ *name;
  mode_t mode;
  પૂર્ણાંક linkok;
  જો (symlink_target)
    {
      name = f->linkname;
      mode = f->linkmode;
      linkok = f->linkok ? 0 : -1;
    }
  નહીંતર
    {
      name = f->name;
      mode = file_or_link_mode (f);
      linkok = f->linkok;
    }

  /* Is this a nonexistent file?  If so, linkok == -1.  */

  જો (linkok == -1 && is_colored (C_MISSING))
    type = C_MISSING;
  નહીંતર જો (!f->stat_ok)
    {
      સ્થિર ગણના indicator_no અચલ filetype_indicator[] =
        {
          C_ORPHAN, C_FIFO, C_CHR, C_DIR, C_BLK, C_FILE,
          C_LINK, C_SOCK, C_FILE, C_DIR
        };
      static_assert (countof (filetype_indicator) == filetype_cardinality);
      type = filetype_indicator[f->filetype];
    }
  નહીંતર
    {
      જો (S_ISREG (mode))
        {
          type = C_FILE;

          જો ((mode & S_ISUID) != 0 && is_colored (C_SETUID))
            type = C_SETUID;
          નહીંતર જો ((mode & S_ISGID) != 0 && is_colored (C_SETGID))
            type = C_SETGID;
          નહીંતર જો (f->has_capability)
            type = C_CAP;
          નહીંતર જો ((mode & S_IXUGO) != 0 && is_colored (C_EXEC))
            type = C_EXEC;
          નહીંતર જો ((1 < f->stat.st_nlink) && is_colored (C_MULTIHARDLINK))
            type = C_MULTIHARDLINK;
        }
      નહીંતર જો (S_ISDIR (mode))
        {
          type = C_DIR;

          જો ((mode & S_ISVTX) && (mode & S_IWOTH)
              && is_colored (C_STICKY_OTHER_WRITABLE))
            type = C_STICKY_OTHER_WRITABLE;
          નહીંતર જો ((mode & S_IWOTH) != 0 && is_colored (C_OTHER_WRITABLE))
            type = C_OTHER_WRITABLE;
          નહીંતર જો ((mode & S_ISVTX) != 0 && is_colored (C_STICKY))
            type = C_STICKY;
        }
      નહીંતર જો (S_ISLNK (mode))
        type = C_LINK;
      નહીંતર જો (S_ISFIFO (mode))
        type = C_FIFO;
      નહીંતર જો (S_ISSOCK (mode))
        type = C_SOCK;
      નહીંતર જો (S_ISBLK (mode))
        type = C_BLK;
      નહીંતર જો (S_ISCHR (mode))
        type = C_CHR;
      નહીંતર જો (S_ISDOOR (mode))
        type = C_DOOR;
      નહીંતર
        {
          /* Classify a file of some other type as C_ORPHAN.  */
          type = C_ORPHAN;
        }
    }

  /* Check the file's suffix only if still classified as C_FILE.  */
  ext = ખાલી;
  જો (type == C_FILE)
    {
      /* Test if NAME has a recognized suffix.  */

      len = strlen (name);
      name += len;		/* Pointer to final \0.  */
      માટે (ext = color_ext_list; ext != ખાલી; ext = ext->next)
        {
          જો (ext->ext.len <= len)
            {
              જો (ext->exact_match)
                {
                  જો (STREQ_LEN (name - ext->ext.len, ext->ext.string,
                                 ext->ext.len))
                    તોડો;
                }
              નહીંતર
                {
                  જો (c_strncasecmp (name - ext->ext.len, ext->ext.string,
                                     ext->ext.len) == 0)
                    તોડો;
                }
            }
        }
    }

  /* Adjust the color for orphaned symlinks.  */
  જો (type == C_LINK && !linkok)
    {
      જો (color_symlink_as_referent || is_colored (C_ORPHAN))
        type = C_ORPHAN;
    }

  અચલ સંરચના bin_str *અચલ s
    = ext ? &(ext->seq) : &color_indicator[type];

  પાછા s->string ? s : ખાલી;
}

/* Output a color indicator (which may contain nulls).
   Note we don't update dired_pos here as emacs can be configured
   to handle color codes by stripping/converting them.  */
સ્થિર શૂન્ય
put_indicator (અચલ સંરચના bin_str *ind)
{
  જો (! used_color)
    {
      used_color = true;

      /* If the standard output is a controlling terminal, watch out
         for signals, so that the colors can be restored to the
         default state if "ls" is suspended or interrupted.  */

      જો (0 <= tcgetpgrp (STDOUT_FILENO))
        signal_init ();

      prep_non_filename_text ();
    }

  fwrite (ind->string, ind->len, 1, stdout);
}

સ્થિર size_t
length_of_file_name_and_frills (અચલ સંરચના fileinfo *f)
{
  size_t len = 0;
  અક્ષર buf[MAX (LONGEST_HUMAN_READABLE + 1, INT_BUFSIZE_BOUND (uintmax_t))];

  જો (print_inode)
    len += 1 + (format == with_commas
                ? strlen (umaxtostr (f->stat.st_ino, buf))
                : inode_સંખ્યા_width);

  જો (print_block_size)
    len += 1 + (format == with_commas
                ? strlen (! f->stat_ok ? "?"
                          : human_readable (STP_NBLOCKS (&f->stat), buf,
                                            human_output_opts, ST_NBLOCKSIZE,
                                            output_block_size))
                : block_size_width);

  જો (print_scontext)
    len += 1 + (format == with_commas ? strlen (f->scontext) : scontext_width);

  len += fileinfo_name_width (f);

  જો (indicator_style != none)
    {
      અક્ષર c = get_type_indicator (f->stat_ok, f->stat.st_mode, f->filetype);
      len += (c != 0);
    }

  પાછા len;
}

સ્થિર શૂન્ય
print_many_per_લીટી (શૂન્ય)
{
  idx_t cols = calculate_columns (true);
  સંરચના column_info અચલ *લીટી_fmt = &column_info[cols - 1];

  /* Calculate the number of rows that will be in each column except possibly
     for a short column on the right.  */
  idx_t rows = cwd_n_used / cols + (cwd_n_used % cols != 0);

  માટે (idx_t row = 0; row < rows; row++)
    {
      size_t col = 0;
      idx_t filesno = row;
      size_t pos = 0;

      /* Print the next row.  */
      જ્યારે (true)
        {
          સંરચના fileinfo અચલ *f = sorted_file[filesno];
          size_t name_length = length_of_file_name_and_frills (f);
          size_t max_name_length = લીટી_fmt->col_arr[col++];
          print_file_name_and_frills (f, pos);

          જો (cwd_n_used - rows <= filesno)
            તોડો;
          filesno += rows;

          indent (pos + name_length, pos + max_name_length);
          pos += max_name_length;
        }
      putchar (eolbyte);
    }
}

સ્થિર શૂન્ય
print_horizontal (શૂન્ય)
{
  size_t pos = 0;
  idx_t cols = calculate_columns (false);
  સંરચના column_info અચલ *લીટી_fmt = &column_info[cols - 1];
  સંરચના fileinfo અચલ *f = sorted_file[0];
  size_t name_length = length_of_file_name_and_frills (f);
  size_t max_name_length = લીટી_fmt->col_arr[0];

  /* Print first entry.  */
  print_file_name_and_frills (f, 0);

  /* Now the rest.  */
  માટે (idx_t filesno = 1; filesno < cwd_n_used; filesno++)
    {
      idx_t col = filesno % cols;

      જો (col == 0)
        {
          putchar (eolbyte);
          pos = 0;
        }
      નહીંતર
        {
          indent (pos + name_length, pos + max_name_length);
          pos += max_name_length;
        }

      f = sorted_file[filesno];
      print_file_name_and_frills (f, pos);

      name_length = length_of_file_name_and_frills (f);
      max_name_length = લીટી_fmt->col_arr[col];
    }
  putchar (eolbyte);
}

/* Output name + SEP + ' '.  */

સ્થિર શૂન્ય
print_with_separator (અક્ષર sep)
{
  size_t pos = 0;

  માટે (idx_t filesno = 0; filesno < cwd_n_used; filesno++)
    {
      સંરચના fileinfo અચલ *f = sorted_file[filesno];
      size_t len = લીટી_length ? length_of_file_name_and_frills (f) : 0;

      જો (filesno != 0)
        {
          અક્ષર separator;
          size_t next_pos = 2 + (sep == ',' && filesno < cwd_n_used - 1);

          જો (! લીટી_length
              || (! ckd_add (&next_pos, next_pos, pos)
                  && ! ckd_add (&next_pos, next_pos, len)
                  && next_pos <= લીટી_length))
            {
              pos += 2;
              separator = ' ';
            }
          નહીંતર
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

સ્થિર શૂન્ય
indent (size_t from, size_t to)
{
  જ્યારે (from < to)
    {
      જો (tabsize != 0 && to / tabsize > (from + 1) / tabsize)
        {
          putchar ('\t');
          from += tabsize - from % tabsize;
        }
      નહીંતર
        {
          putchar (' ');
          from++;
        }
    }
}

/* Put DIRNAME/NAME into DEST, handling '.' and '/' properly.  */
/* FIXME: maybe remove this function someday.  See about using a
   non-malloc'ing version of file_name_concat.  */

સ્થિર શૂન્ય
attach (અક્ષર *dest, અક્ષર અચલ *dirname, અક્ષર અચલ *name)
{
  અક્ષર અચલ *dirnamep = dirname;

  /* Copy dirname if it is not ".".  */
  જો (dirname[0] != '.' || dirname[1] != 0)
    {
      જ્યારે (*dirnamep)
        *dest++ = *dirnamep++;
      /* Add '/' if 'dirname' doesn't already end with it.  */
      જો (dirnamep > dirname && dirnamep[-1] != '/')
        *dest++ = '/';
    }
  જ્યારે (*name)
    *dest++ = *name++;
  *dest = 0;
}

/* Allocate enough column info suitable for the current number of
   files and display columns, and initialize the info to represent the
   narrowest possible columns.  */

સ્થિર શૂન્ય
init_column_info (idx_t max_cols)
{
  /* Currently allocated columns in column_info.  */
  સ્થિર idx_t column_info_alloc;

  જો (column_info_alloc < max_cols)
    {
      idx_t old_column_info_alloc = column_info_alloc;
      column_info = xpalloc (column_info, &column_info_alloc,
                             max_cols - column_info_alloc, -1,
                             કદ *column_info);

      /* Allocate the new size_t objects by computing the triangle
         formula n * (n + 1) / 2, except that we don't need to
         allocate the part of the triangle that we've already
         allocated.  Check for address arithmetic overflow.  */
      idx_t column_info_growth = column_info_alloc - old_column_info_alloc, s;
      જો (ckd_add (&s, old_column_info_alloc + 1, column_info_alloc)
          || ckd_mul (&s, s, column_info_growth))
        xalloc_die ();
      size_t *p = xinmalloc (s >> 1, કદ *p);

      /* Grow the triangle by parceling out the cells just allocated.  */
      માટે (idx_t i = old_column_info_alloc; i < column_info_alloc; i++)
        {
          column_info[i].col_arr = p;
          p += i + 1;
        }
    }

  માટે (idx_t i = 0; i < max_cols; ++i)
    {
      column_info[i].valid_len = true;
      column_info[i].લીટી_len = i * MIN_COLUMN_WIDTH + 1;
      માટે (idx_t j = 0; j < i; ++j)
        column_info[i].col_arr[j] = MIN_COLUMN_WIDTH;
      column_info[i].col_arr[i] = 1;
    }
}

/* Calculate the number of columns needed to represent the current set
   of files in the current display width.  */

સ્થિર idx_t
calculate_columns (bool by_columns)
{
  /* Normally the maximum number of columns is determined by the
     screen width.  But if few files are available this might limit it
     as well.  */
  idx_t max_cols = 0 < max_idx && max_idx < cwd_n_used ? max_idx : cwd_n_used;

  init_column_info (max_cols);

  /* Compute the maximum number of possible columns.  */
  માટે (idx_t filesno = 0; filesno < cwd_n_used; ++filesno)
    {
      સંરચના fileinfo અચલ *f = sorted_file[filesno];
      size_t name_length = length_of_file_name_and_frills (f);

      માટે (idx_t i = 0; i < max_cols; ++i)
        {
          જો (column_info[i].valid_len)
            {
              idx_t idx = (by_columns
                           ? filesno / ((cwd_n_used + i) / (i + 1))
                           : filesno % (i + 1));
              size_t real_length = name_length + (idx == i ? 0 : 2);

              જો (column_info[i].col_arr[idx] < real_length)
                {
                  column_info[i].લીટી_len += (real_length
                                              - column_info[i].col_arr[idx]);
                  column_info[i].col_arr[idx] = real_length;
                  column_info[i].valid_len = (column_info[i].લીટી_len
                                              <= લીટી_length);
                }
            }
        }
    }

  /* Find maximum allowed columns.  */
  idx_t cols;
  માટે (cols = max_cols; 1 < cols; --cols)
    {
      જો (column_info[cols - 1].valid_len)
        તોડો;
    }

  પાછા cols;
}

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
    {
      printf (_("Usage: %s [OPTION]... [FILE]...\n"), program_name);
      fputs (_("\
List information about the FILEs (the current directory by મૂળભૂત).\n\
Sort entries alphabetically જો none of -cftuvSUX nor --sort is specified.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -a, --all\n\
         કરો not ignore entries starting with .\n\
"));
      oputs (_("\
  -A, --almost-all\n\
         કરો not list implied . and ..\n\
"));
      oputs (_("\
      --author\n\
         with -l, print the author of each file\n\
"));
      oputs (_("\
  -b, --escape\n\
         print C-style escapes માટે nongraphic characters\n\
"));
      oputs (_("\
      --block-size=SIZE\n\
         with -l, scale sizes by SIZE when printing them;\n\
         e.g., '--block-size=M'; see SIZE format below\n\
"));
      oputs (_("\
  -B, --ignore-backups\n\
         કરો not list implied entries ending with ~\n\
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
         generate output designed માટે Emacs' dired mode\n\
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
         like -F, except કરો not append '*'\n\
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
         like -l, but કરો not list owner\n\
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
  -H, --dereference-command-લીટી\n\
         follow symbolic links listed on the command લીટી\n\
"));
      oputs (_("\
      --dereference-command-લીટી-symlink-to-dir\n\
         follow each command લીટી symbolic link that points to a directory\n\
"));
      oputs (_("\
      --hide=PATTERN\n\
         કરો not list implied entries matching shell PATTERN\n\
         (overridden by -a or -A)\n\
"));
      oputs (_("\
      --hyperlink[=WHEN]\n\
         hyperlink file names WHEN\n\
"));
      oputs (_("\
      --indicator-style=WORD\n\
         append indicator with style WORD to entry names:\n\
           none (મૂળભૂત), slash (-p), file-type (--file-type), classify (-F)\n\
"));
      oputs (_("\
  -i, --inode\n\
         print the index સંખ્યા of each file\n\
"));
      oputs (_("\
  -I, --ignore=PATTERN\n\
         કરો not list implied entries matching shell PATTERN\n\
"));
      oputs (_("\
  -k, --kibibytes\n\
         મૂળભૂત to 1024-byte blocks માટે file system usage;\n\
         used only with -s and per directory totals\n\
"));
      oputs (_("\
  -l\n\
         use a long listing format\n\
"));
      oputs (_("\
  -L, --dereference\n\
         when showing file information માટે a symbolic link,\n\
         show information માટે the file the link references\n\
         rather than માટે the link itself\n\
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
         like -l, but કરો not list group information\n\
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
         the મૂળભૂત, unless program is 'ls' and output is a terminal\n\
"));
      oputs (_("\
  -Q, --quote-name\n\
         enclose entry names in દ્વિગુણ quotes\n\
"));
      oputs (_("\
      --quoting-style=WORD\n\
         use quoting style WORD માટે entry names:\n\
           literal, locale, shell, shell-always,\n\
           shell-escape, shell-escape-always, c, escape\n\
         (overrides QUOTING_STYLE environment variable)\n\
"));
      oputs (_("\
  -r, --reverse\n\
         reverse order જ્યારે sorting\n\
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
         change મૂળભૂત 'name' sort to WORD:\n\
           none (-U), size (-S), time (-t),\n\
           version (-v), extension (-X), name, width\n\
"));
      oputs (_("\
      --time=WORD\n\
         select which timestamp used to display or sort;\n\
           access time (-u): atime, access, use;\n\
           metadata change time (-c): ctime, status;\n\
           modified time (મૂળભૂત): mtime, modification;\n\
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
         કરો not sort directory entries\n\
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
         list entries by લીટીઓ instead of by columns\n\
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
         end each output લીટી with NUL, not newline\n\
"));
      oputs (_("\
  -1\n\
         list one file per લીટી\n\
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
Also the TIME_STYLE environment variable sets the મૂળભૂત style to use.\n\
"), stdout);
      fputs (_("\
\n\
The WHEN argument defaults to 'always' and can also be 'auto' or 'never'.\n\
"), stdout);
      fputs (_("\
\n\
Using color to distinguish file types is disabled both by મૂળભૂત and\n\
with --color=never.  With --color=auto, ls emits color codes only when\n\
standard output is connected to a terminal.  The LS_COLORS environment\n\
variable can change the settings.  Use the dircolors(1) command to set it.\n\
"), stdout);
      fputs (_("\
\n\
Exit status:\n\
 0  જો OK,\n\
 1  જો minor problems (e.g., cannot access subdirectory),\n\
 2  જો serious trouble (e.g., cannot access command-લીટી argument).\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}
