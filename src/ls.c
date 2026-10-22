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

#अन्तर्भाव <config.h>
#अन्तर्भाव <sys/types.h>

#अन्तर्भाव <termios.h>
#जइ HAVE_STROPTS_H
# include <stropts.h>
#endif
#अन्तर्भाव <sys/ioctl.h>

#ifdef WINSIZE_IN_PTEM
# include <sys/stream.h>
# include <sys/ptem.h>
#endif

#अन्तर्भाव <stdio.h>
#अन्तर्भाव <setjmp.h>
#अन्तर्भाव <pwd.h>
#अन्तर्भाव <getopt.h>
#अन्तर्भाव <signal.h>

#जइ HAVE_LANGINFO_CODESET
# include <langinfo.h>
#endif

/* NonStop circa 2011 lacks both SA_RESTART and siginterrupt, so don't
   restart syscalls after a signal handler fires.  This may cause
   colors to get messed up on the screen if 'ls' is interrupted, but
   that's the best we can do on such a platform.  */
#ifndef SA_RESTART
# define SA_RESTART 0
#endif

#अन्तर्भाव <fnmatch.h>

/* Gnulib includes.  */
#अन्तर्भाव "acl.h"
#अन्तर्भाव "areadlink.h"
#अन्तर्भाव "argmatch.h"
#अन्तर्भाव "assure.h"
#अन्तर्भाव "c-ctype.h"
#अन्तर्भाव "c-strcase.h"
#अन्तर्भाव "canonicalize.h"
#अन्तर्भाव "dev-ino.h"
#अन्तर्भाव "filemode.h"
#अन्तर्भाव "filenamecat.h"
#अन्तर्भाव "filevercmp.h"
#अन्तर्भाव "hard-locale.h"
#अन्तर्भाव "hash.h"
#अन्तर्भाव "human.h"
#अन्तर्भाव "idcache.h"
#अन्तर्भाव "mbswidth.h"
#अन्तर्भाव "mpsort.h"
#अन्तर्भाव "obstack.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "stat-size.h"
#अन्तर्भाव "stat-time.h"
#अन्तर्भाव "strftime.h"
#अन्तर्भाव "xdectoint.h"
#अन्तर्भाव "xgethostname.h"
#अन्तर्भाव "xmemdup0.h"
#अन्तर्भाव "xstrtol-error.h"
#अन्तर्भाव "xstrtol.h"

#अन्तर्भाव "system.h"

#अन्तर्भाव "dircolors.h"
#अन्तर्भाव "ls.h"
#अन्तर्भाव "statx.h"
#अन्तर्भाव "term-sig.h"

/* Include <sys/capability.h> last to avoid a clash of <sys/types.h>
   include guards with some premature versions of libcap.
   For more details, see <https://bugzilla.redhat.com/483548>.  */
#ifdef HAVE_CAP
# include <sys/capability.h>
#endif

#जइ HAVE_LINUX_XATTR_H
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
#define obstack_chunk_free मुक्त

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

#जइ READDIR_LIES_ABOUT_MOUNTPOINT_D_INO
# define RELIABLE_D_INO(dp) NOT_AN_INODE_सङ्ख्या
#अण्णहा
# define RELIABLE_D_INO(dp) D_INO (dp)
#endif

#जइ ! HAVE_STRUCT_STAT_ST_AUTHOR
# define st_author st_uid
#endif

गणना filetype
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
गणना { filetype_cardinality = arg_directory + 1 };

/* Display letters and indicators for each filetype.
   Keep these in sync with enum filetype.  */
स्थिर वर्ण ध्रुव filetype_letter[] =
  {'?', 'p', 'c', 'd', 'b', '-', 'l', 's', 'w', 'd'};
static_assert (countof (filetype_letter) == filetype_cardinality);

/* Map enum filetype to <dirent.h> d_type values.  */
स्थिर unsigned वर्ण ध्रुव filetype_d_type[] =
  {
    DT_UNKNOWN, DT_FIFO, DT_CHR, DT_DIR, DT_BLK, DT_REG, DT_LNK, DT_SOCK,
    DT_WHT, DT_DIR
  };
static_assert (countof (filetype_d_type) == filetype_cardinality);

/* Map d_type values to enum filetype.  */
स्थिर वर्ण ध्रुव d_type_filetype[UCHAR_MAX + 1] =
  {
    [DT_BLK] = blockdev, [DT_CHR] = chardev, [DT_DIR] = directory,
    [DT_FIFO] = fifo, [DT_LNK] = symbolic_link, [DT_REG] = normal,
    [DT_SOCK] = sock, [DT_WHT] = whiteout
  };

गणना acl_type
  {
    ACL_T_NONE,
    ACL_T_UNKNOWN,
    ACL_T_LSM_CONTEXT_ONLY,
    ACL_T_YES
  };

संरचना fileinfo
  {
    /* The file name.  */
    वर्ण *name;

    /* For symbolic link, name of the file linked to, otherwise zero.  */
    वर्ण *linkname;

    /* For terminal hyperlinks. */
    वर्ण *absolute_name;

    संरचना stat stat;

    गणना filetype filetype;

    /* For symbolic link and long listing, st_mode of file linked to, otherwise
       zero.  */
    mode_t linkmode;

    /* security context.  */
    वर्ण *scontext;

    bool stat_ok;

    /* For symbolic link and color printing, true if linked-to file
       exists, otherwise false.  */
    bool linkok;

    /* For long listings, true if the file has an access control list,
       or a security context.  */
    गणना acl_type acl_type;

    /* For color listings, true if a regular file has capability info.  */
    bool has_capability;

    /* Whether file name needs quoting. tri-state with -1 == unknown.  */
    पूर्णाङ्क quoted;

    /* Cached screen width (including quoting).  */
    size_t width;
  };

/* Null is a valid character in a color indicator (think about Epson
   printers, for example) so we have to use a length/buffer string
   type.  */

संरचना bin_str
  {
    size_t len;			/* Number of bytes */
    वर्ण ध्रुव *string;		/* Pointer to the same */
  };

#जइ ! HAVE_TCGETPGRP
# define tcgetpgrp(Fd) 0
#endif

स्थिर size_t quote_name (वर्ण ध्रुव *name,
                          संरचना quoting_options ध्रुव *options,
                          पूर्णाङ्क needs_general_quoting,
                          ध्रुव संरचना bin_str *color,
                          bool allow_pad, संरचना obstack *राशिः,
                          वर्ण ध्रुव *absolute_name);
स्थिर size_t quote_name_buf (वर्ण **inbuf, size_t bufsize, वर्ण *name,
                              संरचना quoting_options ध्रुव *options,
                              पूर्णाङ्क needs_general_quoting, size_t *width,
                              bool *pad);
स्थिर पूर्णाङ्क decode_switches (पूर्णाङ्क argc, वर्ण **argv);
स्थिर bool file_ignored (वर्ण ध्रुव *name);
स्थिर uintmax_t gobble_file (वर्ण ध्रुव *name, गणना filetype type,
                              ino_t inode, bool command_पङ्क्तिः_arg,
                              वर्ण ध्रुव *dirname);
स्थिर ध्रुव संरचना bin_str * get_color_indicator (ध्रुव संरचना fileinfo *f,
                                                   bool symlink_target);
स्थिर bool print_color_indicator (ध्रुव संरचना bin_str *ind);
स्थिर शून्य put_indicator (ध्रुव संरचना bin_str *ind);
स्थिर शून्य add_ignore_pattern (वर्ण ध्रुव *pattern);
स्थिर शून्य attach (वर्ण *dest, वर्ण ध्रुव *dirname, वर्ण ध्रुव *name);
स्थिर शून्य clear_files (शून्य);
स्थिर शून्य extract_dirs_from_files (वर्ण ध्रुव *dirname,
                                     bool command_पङ्क्तिः_arg);
स्थिर bool get_link_name (वर्ण ध्रुव *filename, संरचना fileinfo *f,
                           bool command_पङ्क्तिः_arg);
स्थिर शून्य indent (size_t from, size_t to);
स्थिर idx_t calculate_columns (bool by_columns);
स्थिर शून्य print_current_files (शून्य);
स्थिर शून्य print_dir (वर्ण ध्रुव *name, वर्ण ध्रुव *realname,
                       bool command_पङ्क्तिः_arg);
स्थिर size_t print_file_name_and_frills (ध्रुव संरचना fileinfo *f,
                                          size_t start_col);
स्थिर शून्य print_horizontal (शून्य);
स्थिर पूर्णाङ्क format_user_width (uid_t u);
स्थिर पूर्णाङ्क format_group_width (gid_t g);
स्थिर शून्य print_long_format (ध्रुव संरचना fileinfo *f);
स्थिर शून्य print_many_per_पङ्क्तिः (शून्य);
स्थिर size_t print_name_with_quoting (ध्रुव संरचना fileinfo *f,
                                       bool symlink_target,
                                       संरचना obstack *राशिः,
                                       size_t start_col);
स्थिर शून्य prep_non_filename_text (शून्य);
स्थिर bool print_type_indicator (bool stat_ok, mode_t mode,
                                  गणना filetype type);
स्थिर शून्य print_with_separator (वर्ण sep);
स्थिर शून्य queue_directory (वर्ण ध्रुव *name, वर्ण ध्रुव *realname,
                             bool command_पङ्क्तिः_arg);
स्थिर शून्य sort_files (शून्य);
स्थिर शून्य parse_ls_color (शून्य);

स्थिर size_t quote_name_width (वर्ण ध्रुव *name,
                                संरचना quoting_options ध्रुव *options,
                                पूर्णाङ्क needs_general_quoting);

/* Initial size of hash table.
   Most hierarchies are likely to be shallower than this.  */
गणना { INITIAL_TABLE_SIZE = 30 };

/* The set of 'active' directories, from the current command-line argument
   to the level in the hierarchy at which files are being listed.
   A directory is represented by its device and inode numbers (struct dev_ino).
   A directory is added to this set when ls begins listing it or its
   entries, and it is removed from the set just after ls has finished
   processing it.  This set is used solely to detect loops, e.g., with
   mkdir loop; cd loop; ln -s ../loop sub; ls -RL  */
स्थिर Hash_table *active_dir_set;

#define LOOP_DETECT (!!active_dir_set)

/* The table of files in the current directory:

   'cwd_file' points to a vector of 'struct fileinfo', one per file.
   'cwd_n_alloc' is the number of elements space has been allocated for.
   'cwd_n_used' is the number actually in use.  */

/* Address of block containing the files that are described.  */
स्थिर संरचना fileinfo *cwd_file;

/* Length of block that 'cwd_file' points to, measured in files.  */
स्थिर idx_t cwd_n_alloc;

/* Index of first unused slot in 'cwd_file'.  */
स्थिर idx_t cwd_n_used;

/* Whether files needs may need padding due to quoting.  */
स्थिर bool cwd_some_quoted;

/* Whether quoting style _may_ add outer quotes,
   and whether aligning those is useful.  */
स्थिर bool align_variable_outer_quotes;

/* Vector of pointers to files, in proper sorted order, and the number
   of entries allocated for it.  */
स्थिर शून्य **sorted_file;
स्थिर size_t sorted_file_alloc;

/* When true, in a color listing, color each symlink name according to the
   type of file it points to.  Otherwise, color them according to the 'ln'
   directive in LS_COLORS.  Dangling (orphan) symlinks are treated specially,
   regardless.  This is set when 'ln=target' appears in LS_COLORS.  */

स्थिर bool color_symlink_as_referent;

स्थिर वर्ण ध्रुव *hostname;

/* Mode of appropriate file for coloring.  */
स्थिर mode_t
file_or_link_mode (संरचना fileinfo ध्रुव *file)
{
  प्रत्यागम (color_symlink_as_referent && file->linkok
          ? file->linkmode : file->stat.st_mode);
}


/* Record of one pending directory waiting to be listed.  */

संरचना pending
  {
    वर्ण *name;
    /* If the directory is actually the file pointed to by a symbolic link we
       were told to list, 'realname' will contain the name of the symbolic
       link, otherwise zero.  */
    वर्ण *realname;
    bool command_पङ्क्तिः_arg;
    संरचना pending *next;
  };

स्थिर संरचना pending *pending_dirs;

/* Current time in seconds and nanoseconds since 1970, updated as
   needed when deciding whether a file is recent.  */

स्थिर संरचना timespec current_time;

स्थिर bool print_scontext;
स्थिर वर्ण UNKNOWN_SECURITY_CONTEXT[] = "?";

/* Whether any of the files has an ACL.  This affects the width of the
   mode column.  */

स्थिर bool any_has_acl;

/* The number of columns to use for columns containing inode numbers,
   block sizes, link counts, owners, groups, authors, major device
   numbers, minor device numbers, and file sizes, respectively.  */

स्थिर पूर्णाङ्क inode_सङ्ख्या_width;
स्थिर पूर्णाङ्क block_size_width;
स्थिर पूर्णाङ्क nlink_width;
स्थिर पूर्णाङ्क scontext_width;
स्थिर पूर्णाङ्क owner_width;
स्थिर पूर्णाङ्क group_width;
स्थिर पूर्णाङ्क author_width;
स्थिर पूर्णाङ्क major_device_सङ्ख्या_width;
स्थिर पूर्णाङ्क minor_device_सङ्ख्या_width;
स्थिर पूर्णाङ्क file_size_width;

/* Option flags */

/* long_format for lots of info, one per line.
   one_per_line for just names, one per line.
   many_per_line for just names, many per line, sorted vertically.
   horizontal for just names, many per line, sorted horizontally.
   with_commas for just names, many per line, separated by commas.

   -l (and other options that imply -l), -1, -C, -x and -m control
   this parameter.  */

गणना format
  {
    long_format,		/* -l and other options that imply -l */
    one_per_पङ्क्तिः,		/* -1 */
    many_per_पङ्क्तिः,		/* -C */
    horizontal,			/* -x */
    with_commas			/* -m */
  };

स्थिर गणना format format;

/* 'full-iso' uses full ISO-style dates and times.  'long-iso' uses longer
   ISO-style timestamps, though shorter than 'full-iso'.  'iso' uses shorter
   ISO-style timestamps.  'locale' uses locale-dependent timestamps.  */
गणना time_style
  {
    full_iso_time_style,	/* --time-style=full-iso */
    long_iso_time_style,	/* --time-style=long-iso */
    iso_time_style,		/* --time-style=iso */
    locale_time_style		/* --time-style=locale */
  };

स्थिर वर्ण ध्रुव *ध्रुव time_style_args[] =
{
  "full-iso", "long-iso", "iso", "locale", रिक्त
};
स्थिर गणना time_style ध्रुव time_style_types[] =
{
  full_iso_time_style, long_iso_time_style, iso_time_style,
  locale_time_style
};
ARGMATCH_VERIFY (time_style_args, time_style_types);

/* Type of time to print or sort by.  Controlled by -c and -u.
   The values of each item of this enum are important since they are
   used as indices in the sort functions array (see sort_files()).  */

गणना time_type
  {
    time_mtime = 0,		/* default */
    time_ctime,			/* -c */
    time_atime,			/* -u */
    time_btime,                 /* birth time */
    time_numtypes		/* the number of elements of this enum */
  };

स्थिर गणना time_type time_type;
स्थिर bool explicit_time;

/* The file characteristic to sort by.  Controlled by -t, -S, -U, -X, -v.
   The values of each item of this enum are important since they are
   used as indices in the sort functions array (see sort_files()).  */

गणना sort_type
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

स्थिर गणना sort_type sort_type;

/* Direction of sort.
   false means highest first if numeric,
   lowest first if alphabetic;
   these are the defaults.
   true means the opposite order in each case.  -r  */

स्थिर bool sort_reverse;

/* True means to display owner information.  -g turns this off.  */

स्थिर bool print_owner = true;

/* True means to display author information.  */

स्थिर bool print_author;

/* True means to display group information.  -G and -o turn this off.  */

स्थिर bool print_group = true;

/* True means print the user and group id's as numbers rather
   than as names.  -n  */

स्थिर bool numeric_ids;

/* True means mention the size in blocks of each file.  -s  */

स्थिर bool print_block_size;

/* Human-readable options for output, when printing block counts.  */
स्थिर पूर्णाङ्क human_output_opts;

/* The units to use when printing block counts.  */
स्थिर uintmax_t output_block_size;

/* Likewise, but for file sizes.  */
स्थिर पूर्णाङ्क file_human_output_opts;
स्थिर uintmax_t file_output_block_size = 1;

/* Follow the output with a special string.  Using this format,
   Emacs' dired mode starts up twice as fast, and can handle all
   strange characters in file names.  */
स्थिर bool dired;

/* 'none' means don't mention the type of files.
   'slash' means mention directories only, with a '/'.
   'file_type' means mention file types.
   'classify' means mention file types and mark executables.

   Controlled by -F, -p, and --indicator-style.  */

गणना indicator_style
  {
    none = 0,	/*     --indicator-style=none (default) */
    slash,	/* -p, --indicator-style=slash */
    file_type,	/*     --indicator-style=file-type */
    classify	/* -F, --indicator-style=classify */
  };

स्थिर गणना indicator_style indicator_style;

/* Names of indicator styles.  */
स्थिर वर्ण ध्रुव *ध्रुव indicator_style_args[] =
{
  "none", "slash", "file-type", "classify", रिक्त
};
स्थिर गणना indicator_style ध्रुव indicator_style_types[] =
{
  none, slash, file_type, classify
};
ARGMATCH_VERIFY (indicator_style_args, indicator_style_types);

/* True means use colors to mark types.  Also define the different
   colors as well as the stuff for the LS_COLORS environment variable.
   The LS_COLORS variable is now in a termcap-like format.  */

स्थिर bool print_with_color;

स्थिर bool print_hyperlink;

/* Whether we used any colors in the output so far.  If so, we will
   need to restore the default color later.  If not, we will need to
   call prep_non_filename_text before using color for the first time. */

स्थिर bool used_color = false;

गणना when_type
  {
    when_never,		/* 0: default or --color=never */
    when_always,	/* 1: --color=always */
    when_if_tty		/* 2: --color=tty */
  };

गणना Dereference_symlink
  {
    DEREF_UNDEFINED = 0,		/* default */
    DEREF_NEVER,
    DEREF_COMMAND_पङ्क्तिः_ARGUMENTS,	/* -H */
    DEREF_COMMAND_पङ्क्तिः_SYMLINK_TO_DIR,	/* the default, in certain cases */
    DEREF_ALWAYS			/* -L */
  };

गणना indicator_no
  {
    C_वामः, C_दक्षिणः, C_END, C_RESET, C_NORM, C_FILE, C_DIR, C_LINK,
    C_FIFO, C_SOCK,
    C_BLK, C_CHR, C_MISSING, C_ORPHAN, C_EXEC, C_DOOR, C_SETUID, C_SETGID,
    C_STICKY, C_OTHER_WRITABLE, C_STICKY_OTHER_WRITABLE, C_CAP, C_MULTIHARDLINK,
    C_CLR_TO_EOL
  };

स्थिर वर्ण ध्रुव indicator_name[][2]=
  {
    {'l','c'}, {'r','c'}, {'e','c'}, {'r','s'}, {'n','o'},
    {'f','i'}, {'d','i'}, {'l','n'},
    {'p','i'}, {'s','o'},
    {'b','d'}, {'c','d'}, {'m','i'}, {'o','r'}, {'e','x'},
    {'d','o'}, {'s','u'}, {'s','g'},
    {'s','t'}, {'o','w'}, {'t','w'}, {'c','a'}, {'m','h'},
    {'c','l'}
  };

संरचना color_ext_type
  {
    संरचना bin_str ext;		/* The extension we're looking for */
    संरचना bin_str seq;		/* The sequence to output when we do */
    bool   exact_match;		/* Whether to compare case insensitively */
    संरचना color_ext_type *next;	/* Next in list */
  };

स्थिर संरचना bin_str color_indicator[] =
  {
    { 2, (वर्ण ध्रुव []) {'\033','['} },/* lc: Left of color sequence */
    { 1, (वर्ण ध्रुव []) {'m'} },	/* rc: Right of color sequence */
    { 0, रिक्त },			/* ec: End color (replaces lc+rs+rc) */
    { 1, (वर्ण ध्रुव []) {'0'} },	/* rs: Reset to ordinary colors */
    { 0, रिक्त },			/* no: Normal */
    { 0, रिक्त },			/* fi: File: default */
    { 5, ((वर्ण ध्रुव [])
          {'0','1',';','3','4'}) },	/* di: Directory: bright blue */
    { 5, ((वर्ण ध्रुव [])
          {'0','1',';','3','6'}) },	/* ln: Symlink: bright cyan */
    { 2, (वर्ण ध्रुव []) {'3','3'} },		/* pi: Pipe: yellow/brown */
    { 5, ((वर्ण ध्रुव [])
          {'0','1',';','3','5'}) },	/* so: Socket: bright magenta */
    { 5, ((वर्ण ध्रुव [])
          {'0','1',';','3','3'}) },	/* bd: Block device: bright yellow */
    { 5, ((वर्ण ध्रुव [])
          {'0','1',';','3','3'}) },	/* cd: Char device: bright yellow */
    { 0, रिक्त },			/* mi: Missing file: undefined */
    { 0, रिक्त },			/* or: Orphaned symlink: undefined */
    { 5, ((वर्ण ध्रुव [])
          {'0','1',';','3','2'}) },	/* ex: Executable: bright green */
    { 5, ((वर्ण ध्रुव [])
          {'0','1',';','3','5'}) },	/* do: Door: bright magenta */
    { 5, ((वर्ण ध्रुव [])
          {'3','7',';','4','1'}) },	/* su: setuid: white on red */
    { 5, ((वर्ण ध्रुव [])
          {'3','0',';','4','3'}) },	/* sg: setgid: black on yellow */
    { 5, ((वर्ण ध्रुव [])
          {'3','7',';','4','4'}) },	/* st: sticky: black on blue */
    { 5, ((वर्ण ध्रुव [])
          {'3','4',';','4','2'}) },	/* ow: other-writable: blue on green */
    { 5, ((वर्ण ध्रुव [])
          {'3','0',';','4','2'}) },	/* tw: ow w/ sticky: black on green */
    { 0, रिक्त },			/* ca: disabled by default */
    { 0, रिक्त },			/* mh: disabled by default */
    { 3, ((वर्ण ध्रुव [])
          {'\033','[','K'}) },		/* cl: clear to end of line */
  };

/* A list mapping file extensions to corresponding display sequence.  */
स्थिर संरचना color_ext_type *color_ext_list = रिक्त;

/* Buffer for color sequences */
स्थिर वर्ण *color_buf;

/* True means to check for orphaned symbolic link, for displaying
   colors, or to group symlink to directories with other dirs.  */

स्थिर bool check_symlink_mode;

/* True means mention the inode number of each file.  -i  */

स्थिर bool print_inode;

/* What to do with symbolic links.  Affected by -d, -F, -H, -l (and
   other options that imply -l), and -L.  */

स्थिर गणना Dereference_symlink dereference;

/* True means when a directory is found, display info on its
   contents.  -R  */

स्थिर bool recursive;

/* True means when an argument is a directory name, display info
   on it itself.  -d  */

स्थिर bool immediate_dirs;

/* True means that directories are grouped before files. */

स्थिर bool directories_first;

/* Which files to ignore.  */

स्थिर गणना
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

संरचना ignore_pattern
  {
    वर्ण ध्रुव *pattern;
    संरचना ignore_pattern *next;
  };

स्थिर संरचना ignore_pattern *ignore_patterns;

/* Similar to IGNORE_PATTERNS, except that -a or -A causes this
   variable itself to be ignored.  */
स्थिर संरचना ignore_pattern *hide_patterns;

/* True means output nongraphic chars in file names as '?'.
   (-q, --hide-control-chars)
   qmark_funny_chars and the quoting style (-Q, --quoting-style=WORD) are
   independent.  The algorithm is: first, obey the quoting style to get a
   string representing the file name;  then, if qmark_funny_chars is set,
   replace all nonprintable chars in that string with '?'.  It's necessary
   to replace nonprintable chars even in quoted strings, because we don't
   want to mess up the terminal if control chars get sent to it, and some
   quoting methods pass through control chars as-is.  */
स्थिर bool qmark_funny_chars;

/* True means extra replacement of newline with '?'
   in cases where a newline in a file name would be
   ambiguous with newline separators.  */
स्थिर bool qmark_newline;

/* Quoting options for file and dir name output.  */

स्थिर संरचना quoting_options *filename_quoting_options;
स्थिर संरचना quoting_options *dirname_quoting_options;

/* The number of chars per hardware tab stop.  Setting this to zero
   inhibits the use of TAB characters for separating columns.  -T */
स्थिर size_t tabsize;

/* True means print each directory name before listing it.  */

स्थिर bool print_dir_name;

/* The line length to use for breaking lines in many-per-line format.
   Can be set with -w.  If zero, there is no limit.  */

स्थिर size_t पङ्क्तिः_length;

/* The local time zone rules, as per the TZ environment variable.  */

स्थिर timezone_t localtz;

/* If true, the file listing format requires that stat be called on
   each file.  */

स्थिर bool format_needs_stat;

/* Similar to 'format_needs_stat', but set if only the file type is
   needed.  */

स्थिर bool format_needs_type;

/* Like 'format_needs_stat', but set only if capability colors are needed.  */

स्थिर bool format_needs_capability;

/* An arbitrary limit on the number of bytes in a printed timestamp.
   This is set to a relatively small value to avoid the need to worry
   about denial-of-service attacks on servers that run "ls" on behalf
   of remote clients.  1000 bytes should be enough for any practical
   timestamp format.  */

गणना { TIME_STAMP_LEN_MAXIMUM = MAX (1000, INT_STRLEN_BOUND (time_t)) };

/* strftime formats for non-recent and recent files, respectively, in
   -l output.  */

स्थिर वर्ण ध्रुव *long_time_format[2] =
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

स्थिर sigset_t caught_signals;

/* If nonzero, the value of the pending fatal signal.  */

स्थिर sig_atomic_t चञ्चल interrupt_signal;

/* A count of the number of pending stop signals that have been received.  */

स्थिर sig_atomic_t चञ्चल stop_signal_गणनम्;

/* Desired exit status.  */

स्थिर पूर्णाङ्क exit_status;

/* Exit statuses.  */
गणना
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
गणना
{
  AUTHOR_OPTION = CHAR_MAX + 1,
  BLOCK_SIZE_OPTION,
  COLOR_OPTION,
  DEREFERENCE_COMMAND_पङ्क्तिः_SYMLINK_TO_DIR_OPTION,
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

स्थिर संरचना option ध्रुव long_options[] =
{
  {"all", no_argument, रिक्त, 'a'},
  {"escape", no_argument, रिक्त, 'b'},
  {"directory", no_argument, रिक्त, 'd'},
  {"dired", no_argument, रिक्त, 'D'},
  {"full-time", no_argument, रिक्त, FULL_TIME_OPTION},
  {"group-directories-first", no_argument, रिक्त,
   GROUP_DIRECTORIES_FIRST_OPTION},
  {"human-readable", no_argument, रिक्त, 'h'},
  {"inode", no_argument, रिक्त, 'i'},
  {"kibibytes", no_argument, रिक्त, 'k'},
  {"numeric-uid-gid", no_argument, रिक्त, 'n'},
  {"no-group", no_argument, रिक्त, 'G'},
  {"hide-control-chars", no_argument, रिक्त, 'q'},
  {"reverse", no_argument, रिक्त, 'r'},
  {"size", no_argument, रिक्त, 's'},
  {"width", required_argument, रिक्त, 'w'},
  {"almost-all", no_argument, रिक्त, 'A'},
  {"ignore-backups", no_argument, रिक्त, 'B'},
  {"classify", optional_argument, रिक्त, 'F'},
  {"file-type", no_argument, रिक्त, FILE_TYPE_INDICATOR_OPTION},
  {"si", no_argument, रिक्त, SI_OPTION},
  {"dereference-command-line", no_argument, रिक्त, 'H'},
  {"dereference-command-line-symlink-to-dir", no_argument, रिक्त,
   DEREFERENCE_COMMAND_पङ्क्तिः_SYMLINK_TO_DIR_OPTION},
  {"hide", required_argument, रिक्त, HIDE_OPTION},
  {"ignore", required_argument, रिक्त, 'I'},
  {"indicator-style", required_argument, रिक्त, INDICATOR_STYLE_OPTION},
  {"dereference", no_argument, रिक्त, 'L'},
  {"literal", no_argument, रिक्त, 'N'},
  {"quote-name", no_argument, रिक्त, 'Q'},
  {"quoting-style", required_argument, रिक्त, QUOTING_STYLE_OPTION},
  {"recursive", no_argument, रिक्त, 'R'},
  {"format", required_argument, रिक्त, FORMAT_OPTION},
  {"show-control-chars", no_argument, रिक्त, SHOW_CONTROL_CHARS_OPTION},
  {"sort", required_argument, रिक्त, SORT_OPTION},
  {"tabsize", required_argument, रिक्त, 'T'},
  {"time", required_argument, रिक्त, TIME_OPTION},
  {"time-style", required_argument, रिक्त, TIME_STYLE_OPTION},
  {"zero", no_argument, रिक्त, ZERO_OPTION},
  {"color", optional_argument, रिक्त, COLOR_OPTION},
  {"hyperlink", optional_argument, रिक्त, HYPERLINK_OPTION},
  {"block-size", required_argument, रिक्त, BLOCK_SIZE_OPTION},
  {"context", no_argument, रिक्त, 'Z'},
  {"author", no_argument, रिक्त, AUTHOR_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

स्थिर वर्ण ध्रुव *ध्रुव format_args[] =
{
  "verbose", "long", "commas", "horizontal", "across",
  "vertical", "single-column", रिक्त
};
स्थिर गणना format ध्रुव format_types[] =
{
  long_format, long_format, with_commas, horizontal, horizontal,
  many_per_पङ्क्तिः, one_per_पङ्क्तिः
};
ARGMATCH_VERIFY (format_args, format_types);

स्थिर वर्ण ध्रुव *ध्रुव sort_args[] =
{
  "none", "size", "time", "version", "extension",
  "name", "width", रिक्त
};
स्थिर गणना sort_type ध्रुव sort_types[] =
{
  sort_none, sort_size, sort_time, sort_version, sort_extension,
  sort_name, sort_width
};
ARGMATCH_VERIFY (sort_args, sort_types);

स्थिर वर्ण ध्रुव *ध्रुव time_args[] =
{
  "atime", "access", "use",
  "ctime", "status",
  "mtime", "modification",
  "birth", "creation",
  रिक्त
};
स्थिर गणना time_type ध्रुव time_types[] =
{
  time_atime, time_atime, time_atime,
  time_ctime, time_ctime,
  time_mtime, time_mtime,
  time_btime, time_btime,
};
ARGMATCH_VERIFY (time_args, time_types);

स्थिर वर्ण ध्रुव *ध्रुव when_args[] =
{
  /* force and none are for compatibility with another color-ls version */
  "always", "yes", "force",
  "never", "no", "none",
  "auto", "tty", "if-tty", रिक्त
};
स्थिर गणना when_type ध्रुव when_types[] =
{
  when_always, when_always, when_always,
  when_never, when_never, when_never,
  when_if_tty, when_if_tty, when_if_tty
};
ARGMATCH_VERIFY (when_args, when_types);

/* Information about filling a column.  */
संरचना column_info
{
  bool valid_len;
  size_t पङ्क्तिः_len;
  size_t *col_arr;
};

/* Array with information about column fullness.  */
स्थिर संरचना column_info *column_info;

/* Maximum number of columns ever possible for this display.  */
स्थिर size_t max_idx;

/* The minimum width of a non-final column is 3: 1 character for the name
   and 2 for the separating white space.  The final column is not padded.  */
गणना { MIN_COLUMN_WIDTH = 3 };


/* This zero-based index is for the --dired option.  It is incremented
   for each byte of output generated by this program so that the beginning
   and ending indices (in that output) of every file name can be recorded
   and later output themselves.  */
स्थिर off_t dired_pos;

स्थिर शून्य
dired_outbyte (वर्ण c)
{
  dired_pos++;
  putchar (c);
}

/* Output the buffer S, of length S_LEN, and increment DIRED_POS by S_LEN.  */
स्थिर शून्य
dired_outbuf (वर्ण ध्रुव *s, size_t s_len)
{
  dired_pos += s_len;
  fwrite (s, परिमाण *s, s_len, stdout);
}

/* Output the string S, and increment DIRED_POS by its length.  */
स्थिर शून्य
dired_outstring (वर्ण ध्रुव *s)
{
  dired_outbuf (s, strlen (s));
}

स्थिर शून्य
dired_indent (शून्य)
{
  जइ (dired)
    dired_outstring ("  ");
}

/* With --dired, store pairs of beginning and ending indices of file names.  */
स्थिर संरचना obstack dired_obstack;

/* With --dired, store pairs of beginning and ending indices of any
   directory names that appear as headers (just before 'total' line)
   for lists of directory entries.  Such directory names are seen when
   listing hierarchies using -R and when a directory is listed with at
   least one other command line argument.  */
स्थिर संरचना obstack subdired_obstack;

/* Save the current index on the specified obstack, OBS.  */
स्थिर शून्य
प्रक्षिप_current_dired_pos (संरचना obstack *obs)
{
  जइ (dired)
    obstack_grow (obs, &dired_pos, परिमाण dired_pos);
}

/* With -R, this stack is used to help detect directory cycles.
   The device/inode pairs on this stack mirror the pairs in the
   active_dir_set hash table.  */
स्थिर संरचना obstack dev_ino_obstack;

/* Push a pair onto the device/inode stack.  */
स्थिर शून्य
dev_ino_प्रक्षिप (dev_t dev, ino_t ino)
{
  शून्य *vdi;
  संरचना dev_ino *di;
  पूर्णाङ्क dev_ino_size = परिमाण *di;
  obstack_blank (&dev_ino_obstack, dev_ino_size);
  vdi = obstack_next_free (&dev_ino_obstack);
  di = vdi;
  di--;
  di->st_dev = dev;
  di->st_ino = ino;
}

/* Pop a dev/ino struct off the global dev_ino_obstack
   and return that struct.  */
स्थिर संरचना dev_ino
dev_ino_निष्कासय (शून्य)
{
  शून्य *vdi;
  संरचना dev_ino *di;
  पूर्णाङ्क dev_ino_size = परिमाण *di;
  affirm (dev_ino_size <= obstack_object_size (&dev_ino_obstack));
  obstack_blank_fast (&dev_ino_obstack, -dev_ino_size);
  vdi = obstack_next_free (&dev_ino_obstack);
  di = vdi;
  प्रत्यागम *di;
}

स्थिर शून्य
assert_matching_dev_ino (वर्ण ध्रुव *name, संरचना dev_ino di)
{
  MAYBE_UNUSED संरचना stat sb;
  assure (0 <= stat (name, &sb));
  assure (sb.st_dev == di.st_dev);
  assure (sb.st_ino == di.st_ino);
}

स्थिर वर्ण eolbyte = '\n';

/* Write to standard output PREFIX, followed by the quoting style and
   a space-separated list of the integers stored in OS all on one line.  */

स्थिर शून्य
dired_dump_obstack (वर्ण ध्रुव *prefix, संरचना obstack *os)
{
  size_t n_pos;

  n_pos = obstack_object_size (os) / परिमाण (dired_pos);
  जइ (n_pos > 0)
    {
      off_t *pos = obstack_finish (os);
      fputs (prefix, stdout);
      प्रति (size_t i = 0; i < n_pos; i++)
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
स्थिर संरचना timespec
get_stat_btime (संरचना stat ध्रुव *st)
{
  संरचना timespec btimespec;

#जइ HAVE_STATX && defined STATX_INO
  btimespec = get_stat_mtime (st);
#अण्णहा
  btimespec = get_stat_birthtime (st);
#endif

  प्रत्यागम btimespec;
}

#जइ HAVE_STATX && defined STATX_INO
ATTRIBUTE_PURE
स्थिर unsigned पूर्णाङ्क
time_type_to_statx (शून्य)
{
  विकल्प (time_type)
    {
    पक्ष time_ctime:
      प्रत्यागम STATX_CTIME;
    पक्ष time_mtime:
      प्रत्यागम STATX_MTIME;
    पक्ष time_atime:
      प्रत्यागम STATX_ATIME;
    पक्ष time_btime:
      प्रत्यागम STATX_BTIME;
    पक्ष time_numtypes: स्वभाव:
      unreachable ();
    }
    प्रत्यागम 0;
}

ATTRIBUTE_PURE
स्थिर unsigned पूर्णाङ्क
calc_req_mask (शून्य)
{
  unsigned पूर्णाङ्क mask = STATX_MODE;

  जइ (print_inode)
    mask |= STATX_INO;

  जइ (print_block_size)
    mask |= STATX_BLOCKS;

  जइ (format == long_format) {
    mask |= STATX_NLINK | STATX_SIZE | time_type_to_statx ();
    जइ (print_owner || print_author)
      mask |= STATX_UID;
    जइ (print_group)
      mask |= STATX_GID;
  }

  विकल्प (sort_type)
    {
    पक्ष sort_none:
    पक्ष sort_name:
    पक्ष sort_version:
    पक्ष sort_extension:
    पक्ष sort_width:
      भङ्ग;
    पक्ष sort_time:
      mask |= time_type_to_statx ();
      भङ्ग;
    पक्ष sort_size:
      mask |= STATX_SIZE;
      भङ्ग;
    पक्ष sort_numtypes: स्वभाव:
      unreachable ();
    }

  प्रत्यागम mask;
}

स्थिर पूर्णाङ्क
do_statx (पूर्णाङ्क fd, वर्ण ध्रुव *name, संरचना stat *st, पूर्णाङ्क flags,
          unsigned पूर्णाङ्क mask)
{
  संरचना statx stx;
  bool want_btime = mask & STATX_BTIME;
  पूर्णाङ्क ret = statx (fd, name, flags | AT_NO_AUTOMOUNT, mask, &stx);
  जइ (ret >= 0)
    {
      statx_to_stat (&stx, st);
      /* Since we only need one timestamp type,
         store birth time in st_mtim.  */
      जइ (want_btime)
        {
          जइ (stx.stx_mask & STATX_BTIME)
            st->st_mtim = statx_timestamp_to_timespec (stx.stx_btime);
          अण्णहा
            st->st_mtim.tv_sec = st->st_mtim.tv_nsec = -1;
        }
    }

  प्रत्यागम ret;
}

स्थिर पूर्णाङ्क
do_stat (वर्ण ध्रुव *name, संरचना stat *st)
{
  प्रत्यागम do_statx (AT_FDCWD, name, st, 0, calc_req_mask ());
}

स्थिर पूर्णाङ्क
do_lstat (वर्ण ध्रुव *name, संरचना stat *st)
{
  प्रत्यागम do_statx (AT_FDCWD, name, st, AT_SYMLINK_NOFOLLOW, calc_req_mask ());
}

स्थिर पूर्णाङ्क
stat_for_mode (वर्ण ध्रुव *name, संरचना stat *st)
{
  प्रत्यागम do_statx (AT_FDCWD, name, st, 0, STATX_MODE);
}

/* dev+ino should be static, so no need to sync with backing store */
स्थिर पूर्णाङ्क
stat_for_ino (वर्ण ध्रुव *name, संरचना stat *st)
{
  प्रत्यागम do_statx (AT_FDCWD, name, st, 0, STATX_INO);
}

स्थिर पूर्णाङ्क
fstat_for_ino (पूर्णाङ्क fd, संरचना stat *st)
{
  प्रत्यागम do_statx (fd, "", st, AT_EMPTY_PATH, STATX_INO);
}
#अण्णहा
स्थिर पूर्णाङ्क
do_stat (वर्ण ध्रुव *name, संरचना stat *st)
{
  प्रत्यागम stat (name, st);
}

स्थिर पूर्णाङ्क
do_lstat (वर्ण ध्रुव *name, संरचना stat *st)
{
  प्रत्यागम lstat (name, st);
}

स्थिर पूर्णाङ्क
stat_for_mode (वर्ण ध्रुव *name, संरचना stat *st)
{
  प्रत्यागम stat (name, st);
}

स्थिर पूर्णाङ्क
stat_for_ino (वर्ण ध्रुव *name, संरचना stat *st)
{
  प्रत्यागम stat (name, st);
}

स्थिर पूर्णाङ्क
fstat_for_ino (पूर्णाङ्क fd, संरचना stat *st)
{
  प्रत्यागम fstat (fd, st);
}
#endif

/* Return the address of the first plain %b spec in FMT, or NULL if
   there is no such spec.  %5b etc. do not match, so that user
   widths/flags are honored.  */

ATTRIBUTE_PURE
स्थिर वर्ण ध्रुव *
first_percent_b (वर्ण ध्रुव *fmt)
{
  प्रति (; *fmt; fmt++)
    जइ (fmt[0] == '%')
      विकल्प (fmt[1])
        {
        पक्ष 'b': प्रत्यागम fmt;
        पक्ष '%': fmt++; भङ्ग;
        }
  प्रत्यागम रिक्त;
}

स्थिर वर्ण RFC3986[256];
स्थिर शून्य
file_escape_init (शून्य)
{
  प्रति (पूर्णाङ्क i = 0; i < 256; i++)
    RFC3986[i] |= c_isalnum (i) || i == '~' || i == '-' || i == '.' || i == '_';
}

गणना { MBSWIDTH_FLAGS = MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE };

/* Read the abbreviated month names from the locale, to align them
   and to determine the max width of the field and to truncate names
   greater than our max allowed.
   Note even though this handles multibyte locales correctly
   it's not restricted to them as single byte locales can have
   variable width abbreviated months and also precomputing/caching
   the names was seen to increase the performance of ls significantly.  */

/* abformat[RECENT][MON] is the format to use for timestamps with
   recentness RECENT and month MON.  */
गणना { ABFORMAT_SIZE = 128 };
स्थिर वर्ण abformat[2][12][ABFORMAT_SIZE];
/* True if precomputed formats should be used.  This can be false if
   nl_langinfo fails, if a format or month abbreviation is unusually
   long, or if a month abbreviation contains '%'.  */
स्थिर bool use_abformat;

/* Store into ABMON the abbreviated month names, suitably aligned.
   Return true if successful.  */

स्थिर bool
abmon_init (वर्ण abmon[12][ABFORMAT_SIZE])
{
#ifndef HAVE_NL_LANGINFO
  प्रत्यागम false;
#अण्णहा
  पूर्णाङ्क max_mon_width = 0;
  पूर्णाङ्क mon_width[12];
  पूर्णाङ्क mon_len[12];

  प्रति (पूर्णाङ्क i = 0; i < 12; i++)
    {
      वर्ण ध्रुव *abbr = nl_langinfo (ABMON_1 + i);
      mon_len[i] = strnlen (abbr, ABFORMAT_SIZE);
      जइ (mon_len[i] == ABFORMAT_SIZE)
        प्रत्यागम false;
      जइ (strchr (abbr, '%'))
        प्रत्यागम false;
      mon_width[i] = mbswidth (strcpy (abmon[i], abbr), MBSWIDTH_FLAGS);
      जइ (mon_width[i] < 0)
        प्रत्यागम false;
      max_mon_width = MAX (max_mon_width, mon_width[i]);
    }

  प्रति (पूर्णाङ्क i = 0; i < 12; i++)
    {
      पूर्णाङ्क fill = max_mon_width - mon_width[i];
      जइ (ABFORMAT_SIZE - mon_len[i] <= fill)
        प्रत्यागम false;
      bool align_वामः = !c_isdigit (abmon[i][0]);
      पूर्णाङ्क fill_offset;
      जइ (align_वामः)
        fill_offset = mon_len[i];
      अण्णहा
        {
          memmove (abmon[i] + fill, abmon[i], mon_len[i]);
          fill_offset = 0;
        }
      memset (abmon[i] + fill_offset, ' ', fill);
      abmon[i][mon_len[i] + fill] = '\0';
    }

  प्रत्यागम true;
#endif
}

/* Initialize ABFORMAT and USE_ABFORMAT.  */

स्थिर शून्य
abformat_init (शून्य)
{
  वर्ण ध्रुव *pb[2];
  प्रति (पूर्णाङ्क recent = 0; recent < 2; recent++)
    pb[recent] = first_percent_b (long_time_format[recent]);
  जइ (! (pb[0] || pb[1]))
    प्रत्यागम;

  वर्ण abmon[12][ABFORMAT_SIZE];
  जइ (! abmon_init (abmon))
    प्रत्यागम;

  प्रति (पूर्णाङ्क recent = 0; recent < 2; recent++)
    {
      वर्ण ध्रुव *fmt = long_time_format[recent];
      प्रति (पूर्णाङ्क i = 0; i < 12; i++)
        {
          वर्ण *nfmt = abformat[recent][i];
          पूर्णाङ्क nbytes;

          जइ (! pb[recent])
            nbytes = snprintf (nfmt, ABFORMAT_SIZE, "%s", fmt);
          अण्णहा
            {
              जइ (! (pb[recent] - fmt <= MIN (ABFORMAT_SIZE, INT_MAX)))
                प्रत्यागम;
              पूर्णाङ्क prefix_len = pb[recent] - fmt;
              nbytes = snprintf (nfmt, ABFORMAT_SIZE, "%.*s%s%s",
                                 prefix_len, fmt, abmon[i], pb[recent] + 2);
            }

          जइ (! (0 <= nbytes && nbytes < ABFORMAT_SIZE))
            प्रत्यागम;
        }
    }

  use_abformat = true;
}

स्थिर size_t
dev_ino_hash (शून्य ध्रुव *x, size_t table_size)
{
  संरचना dev_ino ध्रुव *p = x;
  प्रत्यागम (uintmax_t) {p->st_ino} % table_size;
}

स्थिर bool
dev_ino_compare (शून्य ध्रुव *x, शून्य ध्रुव *y)
{
  संरचना dev_ino ध्रुव *a = x;
  संरचना dev_ino ध्रुव *b = y;
  प्रत्यागम PSAME_INODE (a, b);
}

स्थिर शून्य
dev_ino_free (शून्य *x)
{
  मुक्त (x);
}

/* Add the device/inode pair (P->st_dev/P->st_ino) to the set of
   active directories.  Return true if there is already a matching
   entry in the table.  */

स्थिर bool
visit_dir (dev_t dev, ino_t ino)
{
  संरचना dev_ino *ent;
  संरचना dev_ino *ent_from_table;
  bool found_match;

  ent = xmalloc (परिमाण *ent);
  ent->st_ino = ino;
  ent->st_dev = dev;

  /* Attempt to insert this entry into the table.  */
  ent_from_table = hash_निवेशय (active_dir_set, ent);

  जइ (ent_from_table == रिक्त)
    {
      /* Insertion failed due to lack of memory.  */
      xalloc_die ();
    }

  found_match = (ent_from_table != ent);

  जइ (found_match)
    {
      /* ent was not inserted, so free it.  */
      मुक्त (ent);
    }

  प्रत्यागम found_match;
}

स्थिर शून्य
free_pending_ent (संरचना pending *p)
{
  मुक्त (p->name);
  मुक्त (p->realname);
  मुक्त (p);
}

स्थिर bool
is_colored (गणना indicator_no type)
{
  /* Return true unless the string is "", "0" or "00"; try to be efficient.  */
  size_t len = color_indicator[type].len;
  जइ (len == 0)
    प्रत्यागम false;
  जइ (2 < len)
    प्रत्यागम true;
  वर्ण ध्रुव *s = color_indicator[type].string;
  प्रत्यागम (s[0] != '0') | (s[len - 1] != '0');
}

स्थिर शून्य
restore_default_color (शून्य)
{
  put_indicator (&color_indicator[C_वामः]);
  put_indicator (&color_indicator[C_दक्षिणः]);
}

स्थिर शून्य
set_normal_color (शून्य)
{
  जइ (print_with_color && is_colored (C_NORM))
    {
      put_indicator (&color_indicator[C_वामः]);
      put_indicator (&color_indicator[C_NORM]);
      put_indicator (&color_indicator[C_दक्षिणः]);
    }
}

/* An ordinary signal was received; arrange for the program to exit.  */

स्थिर शून्य
sighandler (पूर्णाङ्क sig)
{
  जइ (! interrupt_signal)
    interrupt_signal = sig;
}

/* A SIGTSTP was received; arrange for the program to suspend itself.  */

स्थिर शून्य
stophandler (MAYBE_UNUSED पूर्णाङ्क sig)
{
  जइ (! interrupt_signal)
    stop_signal_गणनम्++;
}

/* Process any pending signals.  If signals are caught, this function
   should be called periodically.  Ideally there should never be an
   unbounded amount of time when signals are not being processed.
   Signal handling can restore the default colors, so callers must
   immediately change colors after invoking this function.  */

स्थिर शून्य
process_signals (शून्य)
{
  जाव (interrupt_signal || stop_signal_गणनम्)
    {
      पूर्णाङ्क sig;
      पूर्णाङ्क stops;
      sigset_t oldset;

      जइ (used_color)
        restore_default_color ();
      fflush (stdout);

      sigprocmask (SIG_BLOCK, &caught_signals, &oldset);

      /* Reload interrupt_signal and stop_signal_count, in case a new
         signal was handled before sigprocmask took effect.  */
      sig = interrupt_signal;
      stops = stop_signal_गणनम्;

      /* SIGTSTP is special, since the application can receive that signal
         more than once.  In this case, don't set the signal handler to the
         default.  Instead, just raise the uncatchable SIGSTOP.  */
      जइ (stops)
        {
          stop_signal_गणनम् = stops - 1;
          sig = SIGSTOP;
        }
      अण्णहा
        सङ्केत (sig, SIG_DFL);

      /* Exit or suspend the program.  */
      उत्थापय (sig);
      sigprocmask (SIG_SETMASK, &oldset, रिक्त);

      /* If execution reaches here, then the program has been
         continued (after being suspended).  */
    }
}

/* Setup signal handlers if INIT is true,
   otherwise restore to the default.  */

स्थिर शून्य
signal_setup (bool init)
{
  /* The signals that are trapped, and the number of such signals.  */
  स्थिर पूर्णाङ्क ध्रुव stop_sig[] =
    {
      /* This one is handled specially.  */
      SIGTSTP,
    };

  गणना { nsigs = countof (term_sig), nstop = countof (stop_sig) };

  जइ (init)
    {
      संरचना sigaction act;

      sigemptyset (&caught_signals);
      प्रति (पूर्णाङ्क j = 0; j < nsigs + nstop; j++)
        {
          पूर्णाङ्क sig = j < nsigs ? term_sig[j]: stop_sig[j - nsigs];
          sigaction (sig, रिक्त, &act);
          जइ (act.sa_handler != SIG_IGN)
            sigaddset (&caught_signals, sig);
        }

      act.sa_mask = caught_signals;
      act.sa_flags = SA_RESTART;

      प्रति (पूर्णाङ्क j = 0; j < nsigs + nstop; j++)
        {
          पूर्णाङ्क sig = j < nsigs ? term_sig[j]: stop_sig[j - nsigs];
          जइ (sigismember (&caught_signals, sig))
            {
              act.sa_handler = j < nsigs ? sighandler : stophandler;
              sigaction (sig, &act, रिक्त);
            }
        }
    }
  अण्णहा /* restore.  */
    {
      प्रति (पूर्णाङ्क j = 0; j < nsigs + nstop; j++)
        {
          पूर्णाङ्क sig = j < nsigs ? term_sig[j]: stop_sig[j - nsigs];
          जइ (sigismember (&caught_signals, sig))
            सङ्केत (sig, SIG_DFL);
        }
    }
}

स्थिर शून्य
signal_init (शून्य)
{
  signal_setup (true);
}

स्थिर शून्य
signal_restore (शून्य)
{
  signal_setup (false);
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  पूर्णाङ्क i;
  संरचना pending *thispend;
  पूर्णाङ्क n_files;

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
  pending_dirs = रिक्त;

  current_time.tv_sec = TYPE_MINIMUM (time_t);
  current_time.tv_nsec = -1;

  i = decode_switches (argc, argv);

  जइ (print_with_color)
    parse_ls_color ();

  /* Test print_with_color again, because the call to parse_ls_color
     may have just reset it -- e.g., if LS_COLORS is invalid.  */

  जइ (print_with_color)
    {
      /* Don't use TAB characters in output.  Some terminal
         emulators can't handle the combination of tabs and
         color codes on the same line.  */
      tabsize = 0;
    }

  जइ (directories_first)
    check_symlink_mode = true;
  अण्णहा जइ (print_with_color)
    {
      /* Avoid following symbolic links when possible.  */
      जइ (is_colored (C_ORPHAN)
          || (is_colored (C_EXEC) && color_symlink_as_referent)
          || (is_colored (C_MISSING) && format == long_format))
        check_symlink_mode = true;
    }

  जइ (dereference == DEREF_UNDEFINED)
    dereference = ((immediate_dirs
                    || indicator_style == classify
                    || format == long_format)
                   ? DEREF_NEVER
                   : DEREF_COMMAND_पङ्क्तिः_SYMLINK_TO_DIR);

  /* When using -R, initialize a data structure we'll use to
     detect any directory cycles.  */
  जइ (recursive)
    {
      active_dir_set = hash_initialize (INITIAL_TABLE_SIZE, रिक्त,
                                        dev_ino_hash,
                                        dev_ino_compare,
                                        dev_ino_free);
      जइ (active_dir_set == रिक्त)
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

  जइ (dired)
    {
      obstack_init (&dired_obstack);
      obstack_init (&subdired_obstack);
    }

  जइ (print_hyperlink)
    {
      file_escape_init ();

      hostname = xgethostname ();
      /* The hostname is generally ignored,
         so ignore failures obtaining it.  */
      जइ (! hostname)
        hostname = "";
    }

  cwd_n_alloc = 100;
  cwd_file = xmalloc (cwd_n_alloc * परिमाण *cwd_file);
  cwd_n_used = 0;

  clear_files ();

  n_files = argc - i;

  जइ (n_files <= 0)
    {
      जइ (immediate_dirs)
        gobble_file (".", directory, NOT_AN_INODE_सङ्ख्या, true, रिक्त);
      अण्णहा
        queue_directory (".", रिक्त, true);
    }
  अण्णहा
    करेहि
      gobble_file (argv[i++], unknown, NOT_AN_INODE_सङ्ख्या, true, रिक्त);
    जाव (i < argc);

  जइ (cwd_n_used)
    {
      sort_files ();
      जइ (!immediate_dirs)
        extract_dirs_from_files (रिक्त, true);
      /* 'cwd_n_used' might be zero now.  */
    }

  /* In the following if/else blocks, it is sufficient to test 'pending_dirs'
     (and not pending_dirs->name) because there may be no markers in the queue
     at this point.  A marker may be enqueued when extract_dirs_from_files is
     called with a non-empty string or via print_dir.  */
  जइ (cwd_n_used)
    {
      print_current_files ();
      जइ (pending_dirs)
        dired_outbyte ('\n');
    }
  अण्णहा जइ (n_files <= 1 && pending_dirs && !pending_dirs->next)
    print_dir_name = false;

  जाव (pending_dirs)
    {
      thispend = pending_dirs;
      pending_dirs = pending_dirs->next;

      जइ (LOOP_DETECT)
        {
          जइ (thispend->name == रिक्त)
            {
              /* thispend->name == NULL means this is a marker entry
                 indicating we've finished processing the directory.
                 Use its dev/ino numbers to remove the corresponding
                 entry from the active_dir_set hash table.  */
              संरचना dev_ino di = dev_ino_निष्कासय ();
              संरचना dev_ino *found = hash_remove (active_dir_set, &di);
              जइ (false)
                assert_matching_dev_ino (thispend->realname, di);
              affirm (found);
              dev_ino_free (found);
              free_pending_ent (thispend);
              अनुवर्त;
            }
        }

      print_dir (thispend->name, thispend->realname,
                 thispend->command_पङ्क्तिः_arg);

      free_pending_ent (thispend);
      print_dir_name = true;
    }

  जइ (print_with_color && used_color)
    {

      /* Skip the restore when it would be a no-op, i.e.,
         when left is "\033[" and right is "m".  */
      जइ (!(color_indicator[C_वामः].len == 2
            && memeq (color_indicator[C_वामः].string, "\033[", 2)
            && color_indicator[C_दक्षिणः].len == 1
            && color_indicator[C_दक्षिणः].string[0] == 'm'))
        restore_default_color ();

      fflush (stdout);

      signal_restore ();

      /* Act on any signals that arrived before the default was restored.
         This can process signals out of order, but there doesn't seem to
         be an easy way to do them in order, and the order isn't that
         important anyway.  */
      प्रति (पूर्णाङ्क j = stop_signal_गणनम्; j; j--)
        उत्थापय (SIGSTOP);
      पूर्णाङ्क j = interrupt_signal;
      जइ (j)
        उत्थापय (j);
    }

  जइ (dired)
    {
      /* No need to free these since we're about to exit.  */
      dired_dump_obstack ("//DIRED//", &dired_obstack);
      dired_dump_obstack ("//SUBDIRED//", &subdired_obstack);
      printf ("//DIRED-OPTIONS// --quoting-style=%s\n",
              quoting_style_args[get_quoting_style (filename_quoting_options)]);
    }

  जइ (LOOP_DETECT)
    {
      assure (hash_get_n_entries (active_dir_set) == 0);
      hash_free (active_dir_set);
    }

  प्रत्यागम exit_status;
}

/* Return the line length indicated by the value given by SPEC, or -1
   if unsuccessful.  0 means no limit on line length.  */

स्थिर ptrdiff_t
decode_पङ्क्तिः_length (वर्ण ध्रुव *spec)
{
  uintmax_t val;

  /* Treat too-large values as if they were 0, which is
     effectively infinity.  */
  विकल्प (xstrtoumax (spec, रिक्त, 0, &val, ""))
    {
    पक्ष LONGINT_OK:
      प्रत्यागम val <= MIN (PTRDIFF_MAX, SIZE_MAX) ? val : 0;

    पक्ष LONGINT_OVERFLOW:
      प्रत्यागम 0;

    पक्ष LONGINT_INVALID:
    पक्ष LONGINT_INVALID_SUFFIX_CHAR:
    पक्ष LONGINT_INVALID_SUFFIX_CHAR_WITH_OVERFLOW:
      प्रत्यागम -1;

    स्वभाव:
      unreachable ();
    }
}

/* Return true if standard output is a tty, caching the result.  */

स्थिर bool
stdout_isatty (शून्य)
{
  स्थिर signed वर्ण out_tty = -1;
  जइ (out_tty < 0)
    out_tty = isatty (STDOUT_FILENO);
  assume (out_tty == 0 || out_tty == 1);
  प्रत्यागम out_tty;
}

/* Set all the option flags according to the switches specified.
   Return the index of the first non-option argument.  */

स्थिर पूर्णाङ्क
decode_switches (पूर्णाङ्क argc, वर्ण **argv)
{
  वर्ण *time_style_option = रिक्त;

  /* These variables are false or -1 unless a switch says otherwise.  */
  bool kibibytes_specified = false;
  पूर्णाङ्क format_opt = -1;
  पूर्णाङ्क hide_control_chars_opt = -1;
  पूर्णाङ्क quoting_style_opt = -1;
  पूर्णाङ्क sort_opt = -1;
  ptrdiff_t tabsize_opt = -1;
  ptrdiff_t width_opt = -1;

  जाव (true)
    {
      पूर्णाङ्क oi = -1;
      पूर्णाङ्क c = getopt_long (argc, argv,
                           "abcdfghiklmnopqrstuvw:xABCDFGHI:LNQRST:UXZ1",
                           long_options, &oi);
      जइ (c == -1)
        भङ्ग;

      विकल्प (c)
        {
        पक्ष 'a':
          ignore_mode = IGNORE_MINIMAL;
          भङ्ग;

        पक्ष 'b':
          quoting_style_opt = escape_quoting_style;
          भङ्ग;

        पक्ष 'c':
          time_type = time_ctime;
          explicit_time = true;
          भङ्ग;

        पक्ष 'd':
          immediate_dirs = true;
          भङ्ग;

        पक्ष 'f':
          ignore_mode = IGNORE_MINIMAL; /* enable -a */
          sort_opt = sort_none;         /* enable -U */
          भङ्ग;

        पक्ष FILE_TYPE_INDICATOR_OPTION: /* --file-type */
          indicator_style = file_type;
          भङ्ग;

        पक्ष 'g':
          format_opt = long_format;
          print_owner = false;
          भङ्ग;

        पक्ष 'h':
          file_human_output_opts = human_output_opts =
            human_autoscale | human_SI | human_base_1024;
          file_output_block_size = output_block_size = 1;
          भङ्ग;

        पक्ष 'i':
          print_inode = true;
          भङ्ग;

        पक्ष 'k':
          kibibytes_specified = true;
          भङ्ग;

        पक्ष 'l':
          format_opt = long_format;
          भङ्ग;

        पक्ष 'm':
          format_opt = with_commas;
          भङ्ग;

        पक्ष 'n':
          numeric_ids = true;
          format_opt = long_format;
          भङ्ग;

        पक्ष 'o':  /* Just like -l, but don't display group info.  */
          format_opt = long_format;
          print_group = false;
          भङ्ग;

        पक्ष 'p':
          indicator_style = slash;
          भङ्ग;

        पक्ष 'q':
          hide_control_chars_opt = true;
          भङ्ग;

        पक्ष 'r':
          sort_reverse = true;
          भङ्ग;

        पक्ष 's':
          print_block_size = true;
          भङ्ग;

        पक्ष 't':
          sort_opt = sort_time;
          भङ्ग;

        पक्ष 'u':
          time_type = time_atime;
          explicit_time = true;
          भङ्ग;

        पक्ष 'v':
          sort_opt = sort_version;
          भङ्ग;

        पक्ष 'w':
          width_opt = decode_पङ्क्तिः_length (optarg);
          जइ (width_opt < 0)
            error (LS_FAILURE, 0, "%s: %s", _("invalid line width"),
                   quote (optarg));
          भङ्ग;

        पक्ष 'x':
          format_opt = horizontal;
          भङ्ग;

        पक्ष 'A':
          ignore_mode = IGNORE_DOT_AND_DOTDOT;
          भङ्ग;

        पक्ष 'B':
          add_ignore_pattern ("*~");
          add_ignore_pattern (".*~");
          भङ्ग;

        पक्ष 'C':
          format_opt = many_per_पङ्क्तिः;
          भङ्ग;

        पक्ष 'D':
          format_opt = long_format;
          print_hyperlink = false;
          dired = true;
          भङ्ग;

        पक्ष 'F':
          {
            पूर्णाङ्क i;
            जइ (optarg)
              i = XARGMATCH ("--classify", optarg, when_args, when_types);
            अण्णहा
              /* Using --classify with no argument is equivalent to using
                 --classify=always.  */
              i = when_always;

            जइ (i == when_always || (i == when_if_tty && stdout_isatty ()))
              indicator_style = classify;
            भङ्ग;
          }

        पक्ष 'G':		/* inhibit display of group info */
          print_group = false;
          भङ्ग;

        पक्ष 'H':
          dereference = DEREF_COMMAND_पङ्क्तिः_ARGUMENTS;
          भङ्ग;

        पक्ष DEREFERENCE_COMMAND_पङ्क्तिः_SYMLINK_TO_DIR_OPTION:
          dereference = DEREF_COMMAND_पङ्क्तिः_SYMLINK_TO_DIR;
          भङ्ग;

        पक्ष 'I':
          add_ignore_pattern (optarg);
          भङ्ग;

        पक्ष 'L':
          dereference = DEREF_ALWAYS;
          भङ्ग;

        पक्ष 'N':
          quoting_style_opt = literal_quoting_style;
          भङ्ग;

        पक्ष 'Q':
          quoting_style_opt = c_quoting_style;
          भङ्ग;

        पक्ष 'R':
          recursive = true;
          भङ्ग;

        पक्ष 'S':
          sort_opt = sort_size;
          भङ्ग;

        पक्ष 'T':
          tabsize_opt = xnumtoumax (optarg, 0, 0, MIN (PTRDIFF_MAX, SIZE_MAX),
                                    "", _("invalid tab size"), LS_FAILURE, 0);
          भङ्ग;

        पक्ष 'U':
          sort_opt = sort_none;
          भङ्ग;

        पक्ष 'X':
          sort_opt = sort_extension;
          भङ्ग;

        पक्ष '1':
          /* -1 has no effect after -l.  */
          जइ (format_opt != long_format)
            format_opt = one_per_पङ्क्तिः;
          भङ्ग;

        पक्ष AUTHOR_OPTION:
          print_author = true;
          भङ्ग;

        पक्ष HIDE_OPTION:
          {
            संरचना ignore_pattern *hide = xmalloc (परिमाण *hide);
            hide->pattern = optarg;
            hide->next = hide_patterns;
            hide_patterns = hide;
          }
          भङ्ग;

        पक्ष SORT_OPTION:
          sort_opt = XARGMATCH ("--sort", optarg, sort_args, sort_types);
          भङ्ग;

        पक्ष GROUP_DIRECTORIES_FIRST_OPTION:
          directories_first = true;
          भङ्ग;

        पक्ष TIME_OPTION:
          time_type = XARGMATCH ("--time", optarg, time_args, time_types);
          explicit_time = true;
          भङ्ग;

        पक्ष FORMAT_OPTION:
          format_opt = XARGMATCH ("--format", optarg, format_args,
                                  format_types);
          भङ्ग;

        पक्ष FULL_TIME_OPTION:
          format_opt = long_format;
          time_style_option = (वर्ण *) "full-iso";
          भङ्ग;

        पक्ष COLOR_OPTION:
          {
            पूर्णाङ्क i;
            जइ (optarg)
              i = XARGMATCH ("--color", optarg, when_args, when_types);
            अण्णहा
              /* Using --color with no argument is equivalent to using
                 --color=always.  */
              i = when_always;

            print_with_color = (i == when_always
                                || (i == when_if_tty && stdout_isatty ()));
            भङ्ग;
          }

        पक्ष HYPERLINK_OPTION:
          {
            पूर्णाङ्क i;
            जइ (optarg)
              i = XARGMATCH ("--hyperlink", optarg, when_args, when_types);
            अण्णहा
              /* Using --hyperlink with no argument is equivalent to using
                 --hyperlink=always.  */
              i = when_always;

            print_hyperlink = (i == when_always
                               || (i == when_if_tty && stdout_isatty ()));
            भङ्ग;
          }

        पक्ष INDICATOR_STYLE_OPTION:
          indicator_style = XARGMATCH ("--indicator-style", optarg,
                                       indicator_style_args,
                                       indicator_style_types);
          भङ्ग;

        पक्ष QUOTING_STYLE_OPTION:
          quoting_style_opt = XARGMATCH ("--quoting-style", optarg,
                                         quoting_style_args,
                                         quoting_style_vals);
          भङ्ग;

        पक्ष TIME_STYLE_OPTION:
          time_style_option = optarg;
          भङ्ग;

        पक्ष SHOW_CONTROL_CHARS_OPTION:
          hide_control_chars_opt = false;
          भङ्ग;

        पक्ष BLOCK_SIZE_OPTION:
          {
            गणना strtol_error e = human_options (optarg, &human_output_opts,
                                                 &output_block_size);
            जइ (e != LONGINT_OK)
              xstrtol_fatal (e, oi, 0, long_options, optarg);
            file_human_output_opts = human_output_opts;
            file_output_block_size = output_block_size;
          }
          भङ्ग;

        पक्ष SI_OPTION:
          file_human_output_opts = human_output_opts =
            human_autoscale | human_SI;
          file_output_block_size = output_block_size = 1;
          भङ्ग;

        पक्ष 'Z':
          print_scontext = true;
          भङ्ग;

        पक्ष ZERO_OPTION:
          eolbyte = 0;
          hide_control_chars_opt = false;
          जइ (format_opt != long_format)
            format_opt = one_per_पङ्क्तिः;
          print_with_color = false;
          quoting_style_opt = literal_quoting_style;
          भङ्ग;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        स्वभाव:
          usage (LS_FAILURE);
        }
    }

  जइ (! output_block_size)
    {
      वर्ण ध्रुव *ls_block_size = getenv ("LS_BLOCK_SIZE");
      human_options (ls_block_size,
                     &human_output_opts, &output_block_size);
      जइ (ls_block_size || getenv ("BLOCK_SIZE"))
        {
          file_human_output_opts = human_output_opts;
          file_output_block_size = output_block_size;
        }
      जइ (kibibytes_specified)
        {
          human_output_opts = 0;
          output_block_size = 1024;
        }
    }

  format = (0 <= format_opt ? format_opt
            : ls_mode == LS_LS ? (stdout_isatty ()
                                  ? many_per_पङ्क्तिः : one_per_पङ्क्तिः)
            : ls_mode == LS_MULTI_COL ? many_per_पङ्क्तिः
            : /* ls_mode == LS_LONG_FORMAT */ long_format);

  /* If the line length was not set by a switch but is needed to determine
     output, go to the work of obtaining it from the environment.  */
  ptrdiff_t linelen = width_opt;
  जइ (format == many_per_पङ्क्तिः || format == horizontal || format == with_commas
      || print_with_color)
    {
#ifdef TIOCGWINSZ
      जइ (linelen < 0)
        {
          संरचना winsize ws;
          जइ (stdout_isatty ()
              && 0 <= ioctl (STDOUT_FILENO, TIOCGWINSZ, &ws)
              && 0 < ws.ws_col)
            linelen = ws.ws_col <= MIN (PTRDIFF_MAX, SIZE_MAX) ? ws.ws_col : 0;
        }
#endif
      जइ (linelen < 0)
        {
          वर्ण ध्रुव *p = getenv ("COLUMNS");
          जइ (p && *p)
            {
              linelen = decode_पङ्क्तिः_length (p);
              जइ (linelen < 0)
                error (0, 0,
                       _("ignoring invalid width"
                         " in environment variable COLUMNS: %s"),
                       quote (p));
            }
        }
    }

  पङ्क्तिः_length = linelen < 0 ? 80 : linelen;

  /* Determine the max possible number of display columns.  */
  max_idx = पङ्क्तिः_length / MIN_COLUMN_WIDTH;
  /* Account for first display column not having a separator,
     or line_lengths shorter than MIN_COLUMN_WIDTH.  */
  max_idx += पङ्क्तिः_length % MIN_COLUMN_WIDTH != 0;

  जइ (format == many_per_पङ्क्तिः || format == horizontal || format == with_commas)
    {
      जइ (0 <= tabsize_opt)
        tabsize = tabsize_opt;
      अण्णहा
        {
          tabsize = 8;
          वर्ण ध्रुव *p = getenv ("TABSIZE");
          जइ (p)
            {
              uintmax_t tmp;
              जइ (xstrtoumax (p, रिक्त, 0, &tmp, "") == LONGINT_OK
                  && tmp <= SIZE_MAX)
                tabsize = tmp;
              अण्णहा
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
                  && ((पङ्क्तिः_length
                       && (format == with_commas
                           || format == many_per_पङ्क्तिः || format == horizontal))
                      || format == one_per_पङ्क्तिः || format == long_format);

  पूर्णाङ्क qs = quoting_style_opt;
  जइ (qs < 0)
    qs = getenv_quoting_style ();
  जइ (qs < 0)
    qs = (ls_mode == LS_LS
          ? (stdout_isatty () ? shell_escape_quoting_style : -1)
          : escape_quoting_style);
  जइ (0 <= qs)
    set_quoting_style (रिक्त, qs);
  qs = get_quoting_style (रिक्त);
  align_variable_outer_quotes
    = ((format == long_format
        || ((format == many_per_पङ्क्तिः || format == horizontal) && पङ्क्तिः_length))
       && (qs == shell_quoting_style
           || qs == shell_escape_quoting_style
           || qs == c_maybe_quoting_style));
  filename_quoting_options = clone_quoting_options (रिक्त);
  जइ (qs == escape_quoting_style)
    set_char_quoting (filename_quoting_options, ' ', 1);
  जइ (file_type <= indicator_style)
    {
      प्रति (वर्ण ध्रुव *p = &"*=>@|"[indicator_style - file_type]; *p; p++)
        set_char_quoting (filename_quoting_options, *p, 1);
    }
  जइ (format == with_commas
      && (qs == shell_quoting_style
          || qs == shell_escape_quoting_style
          || qs == c_maybe_quoting_style
          || qs == escape_quoting_style))
    set_char_quoting (filename_quoting_options, ',', 1);

  dirname_quoting_options = clone_quoting_options (रिक्त);
  set_char_quoting (dirname_quoting_options, ':', 1);

  /* --dired implies --format=long (-l) and sans --hyperlink.
     So ignore it if those overridden.  */
  dired &= (format == long_format) & !print_hyperlink;

  जइ (eolbyte < dired)
    error (LS_FAILURE, 0, _("--dired and --zero are incompatible"));

  /* If a time type is explicitly specified (with -c, -u, or --time=)
     and we're not showing a time (-l not specified), then sort by that time,
     rather than by name.  Note this behavior is unspecified by POSIX.  */

  sort_type = (0 <= sort_opt ? sort_opt
               : (format != long_format && explicit_time)
               ? sort_time : sort_name);

  जइ (format == long_format)
    {
      वर्ण *envstyle = रिक्त;
      वर्ण *style = time_style_option;
      जइ (! style)
        style = envstyle = getenv ("TIME_STYLE");

      जइ (! style)
        style = (वर्ण *) "locale";
      अण्णहा
        {
          स्थिर वर्ण ध्रुव posix_prefix[] = "posix-";
          जाव (STREQ_LEN (style, posix_prefix, परिमाण posix_prefix - 1))
            {
              जइ (! hard_locale (LC_TIME))
                प्रत्यागम optind;
              style += परिमाण posix_prefix - 1;
            }
        }

      जइ (*style == '+')
        {
          वर्ण *p0 = style + 1;
          वर्ण *p0nl = strchr (p0, '\n');
          वर्ण ध्रुव *p1 = p0;
          जइ (p0nl)
            {
              p1 = p0nl + 1;
              जइ (strchr (p1, '\n'))
                error (LS_FAILURE, 0, _("invalid time style format %s"),
                       quote (p0));
              जइ (envstyle)
                p0 = xmemdup0 (p0, p0nl - p0);
              अण्णहा
                *p0nl = '\0';
            }
          long_time_format[0] = p0;
          long_time_format[1] = p1;
        }
      अण्णहा
        {
          विकल्प (x_timestyle_match (style, /*allow_posix=*/ true,
                                     time_style_args,
                                     (वर्ण ध्रुव *) time_style_types,
                                     परिमाण (*time_style_types), LS_FAILURE))
            {
            पक्ष full_iso_time_style:
              long_time_format[0] = long_time_format[1] =
                "%Y-%m-%d %H:%M:%S.%N %z";
              भङ्ग;

            पक्ष long_iso_time_style:
              long_time_format[0] = long_time_format[1] = "%Y-%m-%d %H:%M";
              भङ्ग;

            पक्ष iso_time_style:
              long_time_format[0] = "%Y-%m-%d ";
              long_time_format[1] = "%m-%d %H:%M";
              भङ्ग;

            पक्ष locale_time_style:
              जइ (hard_locale (LC_TIME))
                {
                  प्रति (पूर्णाङ्क i = 0; i < 2; i++)
                    long_time_format[i] =
                      dcgettext (रिक्त, long_time_format[i], LC_TIME);
                }
            }
        }

      abformat_init ();
    }

  प्रत्यागम optind;
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

स्थिर bool
get_funky_string (वर्ण **dest, वर्ण ध्रुव **src, bool equals_end,
                  size_t *output_गणनम्)
{
  वर्ण अङ्कः;			/* For numerical codes */
  size_t गणनम्;			/* Something to count with */
  गणना {
    ST_GND, ST_BACKSLASH, ST_OCTAL, ST_HEX, ST_CARET, ST_END, ST_ERROR
  } state;
  वर्ण ध्रुव *p;
  वर्ण *q;

  p = *src;			/* We don't want to double-indirect */
  q = *dest;			/* the whole darn time.  */

  गणनम् = 0;			/* No characters counted in yet.  */
  अङ्कः = 0;

  state = ST_GND;		/* Start in ground state.  */
  जाव (state < ST_END)
    {
      विकल्प (state)
        {
        पक्ष ST_GND:		/* Ground state (no escapes) */
          विकल्प (*p)
            {
            पक्ष ':':
            पक्ष '\0':
              state = ST_END;	/* End of string */
              भङ्ग;
            पक्ष '\\':
              state = ST_BACKSLASH; /* Backslash escape sequence */
              ++p;
              भङ्ग;
            पक्ष '^':
              state = ST_CARET; /* Caret escape */
              ++p;
              भङ्ग;
            पक्ष '=':
              जइ (equals_end)
                {
                  state = ST_END; /* End */
                  भङ्ग;
                }
              FALLTHROUGH;
            स्वभाव:
              *(q++) = *(p++);
              ++गणनम्;
              भङ्ग;
            }
          भङ्ग;

        पक्ष ST_BACKSLASH:	/* Backslash escaped character */
          विकल्प (*p)
            {
            पक्ष '0':
            पक्ष '1':
            पक्ष '2':
            पक्ष '3':
            पक्ष '4':
            पक्ष '5':
            पक्ष '6':
            पक्ष '7':
              state = ST_OCTAL;	/* Octal sequence */
              अङ्कः = *p - '0';
              भङ्ग;
            पक्ष 'x':
            पक्ष 'X':
              state = ST_HEX;	/* Hex sequence */
              अङ्कः = 0;
              भङ्ग;
            पक्ष 'a':		/* Bell */
              अङ्कः = '\a';
              भङ्ग;
            पक्ष 'b':		/* Backspace */
              अङ्कः = '\b';
              भङ्ग;
            पक्ष 'e':		/* Escape */
              अङ्कः = 27;
              भङ्ग;
            पक्ष 'f':		/* Form feed */
              अङ्कः = '\f';
              भङ्ग;
            पक्ष 'n':		/* Newline */
              अङ्कः = '\n';
              भङ्ग;
            पक्ष 'r':		/* Carriage return */
              अङ्कः = '\r';
              भङ्ग;
            पक्ष 't':		/* Tab */
              अङ्कः = '\t';
              भङ्ग;
            पक्ष 'v':		/* Vtab */
              अङ्कः = '\v';
              भङ्ग;
            पक्ष '?':		/* Delete */
              अङ्कः = 127;
              भङ्ग;
            पक्ष '_':		/* Space */
              अङ्कः = ' ';
              भङ्ग;
            पक्ष '\0':		/* End of string */
              state = ST_ERROR;	/* Error! */
              भङ्ग;
            स्वभाव:		/* Escaped character like \ ^ : = */
              अङ्कः = *p;
              भङ्ग;
            }
          जइ (state == ST_BACKSLASH)
            {
              *(q++) = अङ्कः;
              ++गणनम्;
              state = ST_GND;
            }
          ++p;
          भङ्ग;

        पक्ष ST_OCTAL:		/* Octal sequence */
          जइ (*p < '0' || *p > '7')
            {
              *(q++) = अङ्कः;
              ++गणनम्;
              state = ST_GND;
            }
          अण्णहा
            अङ्कः = (अङ्कः << 3) + (*(p++) - '0');
          भङ्ग;

        पक्ष ST_HEX:		/* Hex sequence */
          विकल्प (*p)
            {
            पक्ष '0':
            पक्ष '1':
            पक्ष '2':
            पक्ष '3':
            पक्ष '4':
            पक्ष '5':
            पक्ष '6':
            पक्ष '7':
            पक्ष '8':
            पक्ष '9':
              अङ्कः = (अङ्कः << 4) + (*(p++) - '0');
              भङ्ग;
            पक्ष 'a':
            पक्ष 'b':
            पक्ष 'c':
            पक्ष 'd':
            पक्ष 'e':
            पक्ष 'f':
              अङ्कः = (अङ्कः << 4) + (*(p++) - 'a') + 10;
              भङ्ग;
            पक्ष 'A':
            पक्ष 'B':
            पक्ष 'C':
            पक्ष 'D':
            पक्ष 'E':
            पक्ष 'F':
              अङ्कः = (अङ्कः << 4) + (*(p++) - 'A') + 10;
              भङ्ग;
            स्वभाव:
              *(q++) = अङ्कः;
              ++गणनम्;
              state = ST_GND;
              भङ्ग;
            }
          भङ्ग;

        पक्ष ST_CARET:		/* Caret escape */
          state = ST_GND;	/* Should be the next state... */
          जइ (*p >= '@' && *p <= '~')
            {
              *(q++) = *(p++) & 037;
              ++गणनम्;
            }
          अण्णहा जइ (*p == '?')
            {
              *(q++) = 127;
              ++गणनम्;
            }
          अण्णहा
            state = ST_ERROR;
          भङ्ग;

        पक्ष ST_END: पक्ष ST_ERROR: स्वभाव:
          affirm (false);
        }
    }

  *dest = q;
  *src = p;
  *output_गणनम् = गणनम्;

  प्रत्यागम state != ST_ERROR;
}

गणना parse_state
  {
    PS_START = 1,
    PS_2,
    PS_3,
    PS_4,
    PS_DONE,
    PS_FAIL
  };


/* Check if the content of TERM is a valid name in dircolors.  */

स्थिर bool
known_term_type (शून्य)
{
  वर्ण ध्रुव *term = getenv ("TERM");
  जइ (! term || ! *term)
    प्रत्यागम false;

  वर्ण ध्रुव *पङ्क्तिः = G_पङ्क्तिः;
  जाव (पङ्क्तिः - G_पङ्क्तिः < परिमाण (G_पङ्क्तिः))
    {
      जइ (STRNCMP_LIT (पङ्क्तिः, "TERM ") == 0)
        {
          जइ (fnmatch (पङ्क्तिः + 5, term, 0) == 0)
            प्रत्यागम true;
        }
      पङ्क्तिः += strlen (पङ्क्तिः) + 1;
    }

  प्रत्यागम false;
}

स्थिर शून्य
parse_ls_color (शून्य)
{
  वर्ण ध्रुव *p;		/* Pointer to character being parsed */
  वर्ण *buf;			/* color_buf buffer pointer */
  वर्ण label0, label1;		/* Indicator label */
  संरचना color_ext_type *ext;	/* Extension we are working on */

  जइ ((p = getenv ("LS_COLORS")) == रिक्त || *p == '\0')
    {
      /* LS_COLORS takes precedence, but if that's not set then
         honor the COLORTERM and TERM env variables so that
         we only go with the internal ANSI color codes if the
         former is non empty or the latter is set to a known value.  */
      वर्ण ध्रुव *colorterm = getenv ("COLORTERM");
      जइ (! (colorterm && *colorterm) && ! known_term_type ())
        print_with_color = false;
      प्रत्यागम;
    }

  ext = रिक्त;

  /* This is an overly conservative estimate, but any possible
     LS_COLORS string will *not* generate a color_buf longer than
     itself, so it is a safe way of allocating a buffer in
     advance.  */
  buf = color_buf = xstrdup (p);

  गणना parse_state state = PS_START;
  जाव (true)
    {
      विकल्प (state)
        {
        पक्ष PS_START:		/* First label character */
          विकल्प (*p)
            {
            पक्ष ':':
              ++p;
              भङ्ग;

            पक्ष '*':
              /* Allocate new extension block and add to head of
                 linked list (this way a later definition will
                 override an earlier one, which can be useful for
                 having terminal-specific defs override global).  */

              ext = xmalloc (परिमाण *ext);
              ext->next = color_ext_list;
              color_ext_list = ext;
              ext->exact_match = false;

              ++p;
              ext->ext.string = buf;

              state = (get_funky_string (&buf, &p, true, &ext->ext.len)
                       ? PS_4 : PS_FAIL);
              भङ्ग;

            पक्ष '\0':
              state = PS_DONE;	/* Done! */
              गच्छ done;

            स्वभाव:	/* Assume it is file type label */
              label0 = *p++;
              state = PS_2;
              भङ्ग;
            }
          भङ्ग;

        पक्ष PS_2:		/* Second label character */
          जइ (*p)
            {
              label1 = *p++;
              state = PS_3;
            }
          अण्णहा
            state = PS_FAIL;	/* Error */
          भङ्ग;

        पक्ष PS_3:		/* Equal sign after indicator label */
          state = PS_FAIL;	/* Assume failure...  */
          जइ (*(p++) == '=')/* It *should* be...  */
            {
              प्रति (पूर्णाङ्क i = 0; i < countof (indicator_name); i++)
                {
                  जइ ((label0 == indicator_name[i][0])
                      && (label1 == indicator_name[i][1]))
                    {
                      color_indicator[i].string = buf;
                      state = (get_funky_string (&buf, &p, false,
                                                 &color_indicator[i].len)
                               ? PS_START : PS_FAIL);
                      भङ्ग;
                    }
                }
              जइ (state == PS_FAIL)
                error (0, 0, _("unrecognized prefix: %s"),
                       quote ((वर्ण []) {label0, label1, '\0'}));
            }
          भङ्ग;

        पक्ष PS_4:		/* Equal sign after *.ext */
          जइ (*(p++) == '=')
            {
              ext->seq.string = buf;
              state = (get_funky_string (&buf, &p, false, &ext->seq.len)
                       ? PS_START : PS_FAIL);
            }
          अण्णहा
            state = PS_FAIL;
          भङ्ग;

        पक्ष PS_FAIL:
          गच्छ done;

        पक्ष PS_DONE: स्वभाव:
          affirm (false);
        }
    }
 done:

  जइ (state == PS_FAIL)
    {
      error (0, 0,
             _("unparsable value for LS_COLORS environment variable"));
      मुक्त (color_buf);
      प्रति (संरचना color_ext_type *e = color_ext_list; e != रिक्त;
           /* empty */)
        {
          संरचना color_ext_type *e2 = e;
          e = e->next;
          मुक्त (e2);
        }
      print_with_color = false;
      प्रत्यागम;
    }
  अण्णहा
    {
      /* Postprocess list to set EXACT_MATCH on entries where there are
         different cased extensions with separate sequences defined.
         Also set ext.len to SIZE_MAX on any entries that can't
         match due to precedence, to avoid redundant string compares.  */
      प्रति (संरचना color_ext_type *e1 = color_ext_list; e1 != रिक्त;
           e1 = e1->next)
        {
          bool case_ignored = false;

          प्रति (संरचना color_ext_type *e2 = e1->next; e2 != रिक्त;
               e2 = e2->next)
            {
              जइ (e2->ext.len < SIZE_MAX && e1->ext.len == e2->ext.len)
                {
                  जइ (memeq (e1->ext.string, e2->ext.string, e1->ext.len))
                    e2->ext.len = SIZE_MAX; /* Ignore */
                  अण्णहा जइ (c_strncasecmp (e1->ext.string, e2->ext.string,
                                          e1->ext.len) == 0)
                    {
                      जइ (case_ignored)
                        {
                          e2->ext.len = SIZE_MAX; /* Ignore */
                        }
                      अण्णहा जइ (e1->seq.len == e2->seq.len
                               && memeq (e1->seq.string, e2->seq.string,
                                         e1->seq.len))
                        {
                          e2->ext.len = SIZE_MAX; /* Ignore */
                          case_ignored = true;    /* Ignore all subsequent */
                        }
                      अण्णहा
                        {
                          e1->exact_match = true;
                          e2->exact_match = true;
                        }
                    }
                }
            }
        }
    }

  जइ (color_indicator[C_LINK].len == 6
      && !STRNCMP_LIT (color_indicator[C_LINK].string, "target"))
    color_symlink_as_referent = true;
}

/* Set the exit status to report a failure.  If SERIOUS, it is a
   serious failure; otherwise, it is merely a minor problem.  */

स्थिर शून्य
set_exit_status (bool serious)
{
  जइ (serious)
    exit_status = LS_FAILURE;
  अण्णहा जइ (exit_status == EXIT_SUCCESS)
    exit_status = LS_MINOR_PROBLEM;
}

/* Assuming a failure is serious if SERIOUS, use the printf-style
   MESSAGE to report the failure to access a file named FILE.  Assume
   errno is set appropriately for the failure.  */

स्थिर शून्य
file_failure (bool serious, वर्ण ध्रुव *message, वर्ण ध्रुव *file)
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

स्थिर शून्य
queue_directory (वर्ण ध्रुव *name, वर्ण ध्रुव *realname, bool command_पङ्क्तिः_arg)
{
  संरचना pending *new = xmalloc (परिमाण *new);
  new->realname = realname ? xstrdup (realname) : रिक्त;
  new->name = name ? xstrdup (name) : रिक्त;
  new->command_पङ्क्तिः_arg = command_पङ्क्तिः_arg;
  new->next = pending_dirs;
  pending_dirs = new;
}

/* Read directory NAME, and list the files in it.
   If REALNAME is nonzero, print its name instead of NAME;
   this is used for symbolic links to directories.
   COMMAND_LINE_ARG means this directory was mentioned on the command line.  */

स्थिर शून्य
print_dir (वर्ण ध्रुव *name, वर्ण ध्रुव *realname, bool command_पङ्क्तिः_arg)
{
  DIR *dirp;
  संरचना dirent *next;
  uintmax_t total_blocks = 0;
  स्थिर bool first = true;

  errno = 0;
  dirp = opendir (name);
  जइ (!dirp)
    {
      जइ (command_पङ्क्तिः_arg || ! ignorable_traversal_errno (errno))
        file_failure (command_पङ्क्तिः_arg, _("cannot open directory %s"), name);
      प्रत्यागम;
    }

  जइ (LOOP_DETECT)
    {
      संरचना stat dir_stat;
      पूर्णाङ्क fd = dirfd (dirp);

      /* If dirfd failed, endure the overhead of stat'ing by path  */
      जइ ((0 <= fd
           ? fstat_for_ino (fd, &dir_stat)
           : stat_for_ino (name, &dir_stat)) < 0)
        {
          जइ (! ignorable_traversal_errno (errno))
            file_failure (command_पङ्क्तिः_arg,
                          _("cannot determine device and inode of %s"), name);
          closedir (dirp);
          प्रत्यागम;
        }

      /* If we've already visited this dev/inode pair, warn that
         we've found a loop, and do not process this directory.  */
      जइ (visit_dir (dir_stat.st_dev, dir_stat.st_ino))
        {
          error (0, 0, _("%s: not listing already-listed directory"),
                 quotef (name));
          closedir (dirp);
          set_exit_status (true);
          प्रत्यागम;
        }

      dev_ino_प्रक्षिप (dir_stat.st_dev, dir_stat.st_ino);
    }

  clear_files ();

  जइ (recursive || print_dir_name)
    {
      जइ (!first)
        dired_outbyte ('\n');
      first = false;
      dired_indent ();

      वर्ण *absolute_name = रिक्त;
      जइ (print_hyperlink)
        {
          absolute_name = canonicalize_filename_mode (name, CAN_MISSING);
          जइ (! absolute_name
              && ! ignorable_traversal_errno (errno))
            file_failure (command_पङ्क्तिः_arg,
                          _("error canonicalizing %s"), name);
        }
      quote_name (realname ? realname : name, dirname_quoting_options, -1,
                  रिक्त, true, &subdired_obstack, absolute_name);

      मुक्त (absolute_name);

      dired_outstring (":\n");
    }

  /* Read the directory entries, and insert the subfiles into the 'cwd_file'
     table.  */

  जाव (true)
    {
      /* Set errno to zero so we can distinguish between a readdir failure
         and when readdir simply finds that there are no more entries.  */
      errno = 0;
      next = readdir (dirp);
      जइ (next)
        {
          जइ (! file_ignored (next->d_name))
            {
              गणना filetype type;
#जइ HAVE_STRUCT_DIRENT_D_TYPE
              type = d_type_filetype[next->d_type];
#अण्णहा
              type = unknown;
#endif
              total_blocks += gobble_file (next->d_name, type,
                                           RELIABLE_D_INO (next),
                                           false, name);

              /* In this narrow case, print out each name right away, so
                 ls uses constant memory while processing the entries of
                 this directory.  Useful when there are many (millions)
                 of entries in a directory.  */
              जइ (format == one_per_पङ्क्तिः && sort_type == sort_none
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
      अण्णहा
        {
          पूर्णाङ्क err = errno;
          जइ (err == 0)
            भङ्ग;
          /* Ignore errors indicating that the directory was removed.  */
          जइ (ignorable_traversal_errno (err))
            भङ्ग;
          file_failure (command_पङ्क्तिः_arg, _("reading directory %s"), name);
          जइ (err != EOVERFLOW)
            भङ्ग;
        }

      /* When processing a very large directory, and since we've inhibited
         interrupts, this loop would take so long that ls would be annoyingly
         uninterruptible.  This ensures that it handles signals promptly.  */
      process_signals ();
    }

  जइ (closedir (dirp) != 0)
    {
      file_failure (command_पङ्क्तिः_arg, _("closing directory %s"), name);
      /* Don't return; print whatever we got.  */
    }

  /* Sort the directory contents.  */
  sort_files ();

  /* If any member files are subdirectories, perhaps they should have their
     contents listed rather than being mentioned here as files.  */

  जइ (recursive)
    extract_dirs_from_files (name, false);

  जइ (format == long_format || print_block_size)
    {
      वर्ण buf[LONGEST_HUMAN_READABLE + 3];
      वर्ण *p = human_readable (total_blocks, buf + 1, human_output_opts,
                                ST_NBLOCKSIZE, output_block_size);
      वर्ण *pend = p + strlen (p);
      *--p = ' ';
      *pend++ = eolbyte;
      dired_indent ();
      dired_outstring (_("total"));
      dired_outbuf (p, pend - p);
    }

  जइ (cwd_n_used)
    print_current_files ();
}

/* Add 'pattern' to the list of patterns for which files that match are
   not listed.  */

स्थिर शून्य
add_ignore_pattern (वर्ण ध्रुव *pattern)
{
  संरचना ignore_pattern *ignore;

  ignore = xmalloc (परिमाण *ignore);
  ignore->pattern = pattern;
  /* Add it to the head of the linked list.  */
  ignore->next = ignore_patterns;
  ignore_patterns = ignore;
}

/* Return true if one of the PATTERNS matches FILE.  */

स्थिर bool
patterns_match (संरचना ignore_pattern ध्रुव *patterns, वर्ण ध्रुव *file)
{
  प्रति (संरचना ignore_pattern ध्रुव *p = patterns; p; p = p->next)
    जइ (fnmatch (p->pattern, file, FNM_PERIOD) == 0)
      प्रत्यागम true;
  प्रत्यागम false;
}

/* Return true if FILE should be ignored.  */

स्थिर bool
file_ignored (वर्ण ध्रुव *name)
{
  प्रत्यागम ((ignore_mode != IGNORE_MINIMAL
           && name[0] == '.'
           && (ignore_mode == IGNORE_DEFAULT || ! name[1 + (name[1] == '.')]))
          || (ignore_mode == IGNORE_DEFAULT
              && patterns_match (hide_patterns, name))
          || patterns_match (ignore_patterns, name));
}

/* POSIX requires that a file size be printed without a sign, even
   when negative.  Assume the typical case where negative sizes are
   actually positive values that have wrapped around.  */

स्थिर uintmax_t
unsigned_file_size (off_t size)
{
  प्रत्यागम size + (size < 0) * ((uintmax_t) OFF_T_MAX - OFF_T_MIN + 1);
}

#ifdef HAVE_CAP
/* Return true if NAME has a capability (see linux/capability.h) */
स्थिर bool
has_capability (वर्ण ध्रुव *name)
{
  वर्ण *result;
  bool has_cap;

  cap_t cap_d = cap_get_file (name);
  जइ (cap_d == रिक्त)
    प्रत्यागम false;

  result = cap_to_text (cap_d, रिक्त);
  cap_free (cap_d);
  जइ (!result)
    प्रत्यागम false;

  /* check if human-readable capability string is empty */
  has_cap = !!*result;

  cap_free (result);
  प्रत्यागम has_cap;
}
#अण्णहा
स्थिर bool
has_capability (MAYBE_UNUSED वर्ण ध्रुव *name)
{
  errno = ENOTSUP;
  प्रत्यागम false;
}
#endif

/* Enter and remove entries in the table 'cwd_file'.  */

स्थिर शून्य
free_ent (संरचना fileinfo *f)
{
  मुक्त (f->name);
  मुक्त (f->linkname);
  मुक्त (f->absolute_name);
  जइ (f->scontext != UNKNOWN_SECURITY_CONTEXT)
    aclinfo_scontext_free (f->scontext);
}

/* Empty the table of files.  */
स्थिर शून्य
clear_files (शून्य)
{
  प्रति (idx_t i = 0; i < cwd_n_used; i++)
    {
      संरचना fileinfo *f = sorted_file[i];
      free_ent (f);
    }

  cwd_n_used = 0;
  cwd_some_quoted = false;
  any_has_acl = false;
  inode_सङ्ख्या_width = 0;
  block_size_width = 0;
  nlink_width = 0;
  owner_width = 0;
  group_width = 0;
  author_width = 0;
  scontext_width = 0;
  major_device_सङ्ख्या_width = 0;
  minor_device_सङ्ख्या_width = 0;
  file_size_width = 0;
}

/* Cache file_has_aclinfo failure, when it's trivial to do.
   Like file_has_aclinfo, but when F's st_dev says it's on a file
   system lacking ACL support, return 0 with ENOTSUP immediately.  */
स्थिर पूर्णाङ्क
file_has_aclinfo_cache (वर्ण ध्रुव *file, संरचना fileinfo *f,
                        संरचना aclinfo *ai, पूर्णाङ्क flags)
{
  /* If UNSUPPORTED_SCONTEXT, these variables hold the
     st_dev and associated info for the most recently processed device
     for which file_has_aclinfo failed indicating lack of support.  */
  स्थिर पूर्णाङ्क unsupported_return;
  स्थिर वर्ण *unsupported_scontext;
  स्थिर पूर्णाङ्क unsupported_scontext_err;
  स्थिर dev_t unsupported_device;

  जइ (f->stat_ok && unsupported_scontext
      && f->stat.st_dev == unsupported_device)
    {
      ai->buf = ai->u._gl_acl_ch;
      ai->size = 0;
      ai->scontext = unsupported_scontext;
      ai->scontext_err = unsupported_scontext_err;
      errno = ENOTSUP;
      प्रत्यागम unsupported_return;
    }

  errno = 0;
  पूर्णाङ्क n = file_has_aclinfo (file, ai, flags);
  पूर्णाङ्क err = errno;
  जइ (!USE_ACL)
    n = 0;
  /* A security-context error does not imply that ACLs are unsupported.
     Require an unsupported attribute listing too if ACLs or capability
     colors are needed.  */
  जइ (f->stat_ok && n <= 0 && !acl_errno_valid (err)
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
  प्रत्यागम n;
}

/* Cache has_capability failure, when it's trivial to do.
   Like has_capability, but when F's st_dev says it's on a file
   system lacking capability support, return 0 with ENOTSUP immediately.  */
स्थिर bool
has_capability_cache (वर्ण ध्रुव *file, संरचना fileinfo *f)
{
  /* If UNSUPPORTED_CACHED, UNSUPPORTED_DEVICE is the cached
     st_dev of the most recently processed device for which we've
     found that has_capability fails indicating lack of support.  */
  स्थिर bool unsupported_cached;
  स्थिर dev_t unsupported_device;

  जइ (f->stat_ok && unsupported_cached && f->stat.st_dev == unsupported_device)
    {
      errno = ENOTSUP;
      प्रत्यागम 0;
    }

  bool b = has_capability (file);
  जइ (f->stat_ok && !b && !acl_errno_valid (errno))
    {
      unsupported_cached = true;
      unsupported_device = f->stat.st_dev;
    }
  प्रत्यागम b;
}

/* Get the ACL, security context, and capability information needed for F.
   FULL_NAME names the file; DO_DEREF says whether to follow symlinks.  */
स्थिर शून्य
get_extended_info (वर्ण ध्रुव *full_name, संरचना fileinfo *f, bool do_deref)
{
  /* FIXME: file_has_aclinfo() conflates capability and acl xattr lookup
     with security context attributes.  Ideally we'd have separate
     ACL_GET_{CAPABILITY,ACL} flags to lookup just the required items,
     simplifying the logic here.  */

  गणना filetype type = f->filetype;
  bool check_acl = USE_ACL && format == long_format;
  bool get_scontext = false;
#जइ HAVE_SMACK || USE_SELINUX_SELINUX_H
  get_scontext = (format == long_format) | print_scontext;
#endif
  bool check_capability = format_needs_capability & (type == normal);

  जइ (type != arg_directory && (check_acl | get_scontext | check_capability))
    {
      संरचना aclinfo ai;
      /* Without ACL support, request contexts to obtain the attribute
         names needed for capability coloring too.  */
      पूर्णाङ्क aclinfo_flags = ((do_deref ? ACL_SYMLINK_FOLLOW : 0)
                           | (get_scontext || (!USE_ACL && check_capability)
                              ? ACL_GET_SCONTEXT : 0)
                           | filetype_d_type[type]);
      पूर्णाङ्क n = file_has_aclinfo_cache (full_name, f, &ai, aclinfo_flags);
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

      जइ (format == long_format && n < 0 && !cannot_access_acl)
        error (0, errno, "%s", quotef (full_name));
      अण्णहा
        {
          /* When requesting security context information, don't make
             ls fail just because the file (even a command line argument)
             isn't on the right type of file system.  I.e., a getfilecon
             failure isn't in the same class as a stat failure.  */
          जइ (print_scontext && ai.scontext_err
              && ! ignorable_traversal_errno (ai.scontext_err)
              && (! (is_ENOTSUP (ai.scontext_err)
                     || ai.scontext_err == ENODATA)))
            error (0, ai.scontext_err, "%s", quotef (full_name));
        }

      /* has_capability adds around 30% runtime to 'ls --color',
         so call it only if really needed.  Note capability coloring
         is disabled in the default color config.  */
      जइ (check_capability && aclinfo_has_xattr (&ai, XATTR_NAME_CAPS))
        f->has_capability = has_capability_cache (full_name, f);

      f->scontext = ai.scontext;
      ai.scontext = रिक्त;
      aclinfo_free (&ai);
    }
}

स्थिर bool
needs_quoting (वर्ण ध्रुव *name)
{
  वर्ण test[2];
  size_t len = quotearg_buffer (test, परिमाण test , name, -1,
                                filename_quoting_options);
  प्रत्यागम *name != *test || strlen (name) != len;
}

/* Add a file to the current table of files.
   Verify that the file exists, and print an error message if it does not.
   Return the number of blocks that the file occupies.  */
स्थिर uintmax_t
gobble_file (वर्ण ध्रुव *name, गणना filetype type, ino_t inode,
             bool command_पङ्क्तिः_arg, वर्ण ध्रुव *dirname)
{
  uintmax_t blocks = 0;
  संरचना fileinfo *f;

  /* An inode value prior to gobble_file necessarily came from readdir,
     which is not used for command line arguments.  */
  affirm (! command_पङ्क्तिः_arg || inode == NOT_AN_INODE_सङ्ख्या);

  जइ (cwd_n_used == cwd_n_alloc)
    cwd_file = xpalloc (cwd_file, &cwd_n_alloc, 1, -1, परिमाण *cwd_file);

  f = &cwd_file[cwd_n_used];
  memset (f, '\0', परिमाण *f);
  f->stat.st_ino = inode;
  f->filetype = type;
  f->scontext = UNKNOWN_SECURITY_CONTEXT;

  f->quoted = -1;
  जइ ((! cwd_some_quoted) && align_variable_outer_quotes)
    {
      /* Determine if any quoted for padding purposes.  */
      f->quoted = needs_quoting (name);
      जइ (f->quoted)
        cwd_some_quoted = 1;
    }

  bool check_stat =
     (command_पङ्क्तिः_arg
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
      || (print_inode && inode == NOT_AN_INODE_सङ्ख्या)
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
  वर्ण ध्रुव *full_name = name;
  जइ (check_stat | print_scontext | format_needs_capability
      && name[0] != '/' && dirname)
    {
      वर्ण *p = alloca (strlen (name) + strlen (dirname) + 2);
      attach (p, dirname, name);
      full_name = p;
    }

  bool do_deref;

  जइ (!check_stat)
    do_deref = dereference == DEREF_ALWAYS;
  अण्णहा
    {
      पूर्णाङ्क err;

      जइ (print_hyperlink)
        {
          f->absolute_name = canonicalize_filename_mode (full_name,
                                                         CAN_MISSING);
          जइ (! f->absolute_name
              && (command_पङ्क्तिः_arg
                  || ! ignorable_traversal_errno (errno)))
            file_failure (command_पङ्क्तिः_arg,
                          _("error canonicalizing %s"), full_name);
        }

      विकल्प (dereference)
        {
        पक्ष DEREF_ALWAYS:
          err = do_stat (full_name, &f->stat);
          do_deref = true;
          भङ्ग;

        पक्ष DEREF_COMMAND_पङ्क्तिः_ARGUMENTS:
        पक्ष DEREF_COMMAND_पङ्क्तिः_SYMLINK_TO_DIR:
          जइ (command_पङ्क्तिः_arg)
            {
              bool need_lstat;
              err = do_stat (full_name, &f->stat);
              do_deref = true;

              जइ (dereference == DEREF_COMMAND_पङ्क्तिः_ARGUMENTS)
                भङ्ग;

              need_lstat = (err < 0
                            ? (errno == ENOENT || errno == ELOOP)
                            : ! S_ISDIR (f->stat.st_mode));
              जइ (!need_lstat)
                भङ्ग;

              /* stat failed because of ENOENT || ELOOP, maybe indicating a
                 non-traversable symlink.  Or stat succeeded,
                 FULL_NAME does not refer to a directory,
                 and --dereference-command-line-symlink-to-dir is in effect.
                 Fall through so that we call lstat instead.  */
            }
          FALLTHROUGH;

        पक्ष DEREF_NEVER:
          err = do_lstat (full_name, &f->stat);
          do_deref = false;
          भङ्ग;

        पक्ष DEREF_UNDEFINED: स्वभाव:
          affirm (false);
        }

      जइ (err != 0)
        {
          जइ (! command_पङ्क्तिः_arg && ignorable_traversal_errno (errno))
            {
              पूर्णाङ्क stat_errno = errno;
              संरचना stat linkstat;
              bool dangling_link = (do_deref
                                    && (type == symbolic_link
                                        || type == unknown)
                                    && do_lstat (full_name, &linkstat) == 0);
              errno = stat_errno;
              जइ (! dangling_link)
                {
                  free_ent (f);
                  प्रत्यागम 0;
                }
            }

          /* Failure to stat a command line argument leads to
             an exit status of 2.  For other files, stat failure
             provokes an exit status of 1.  */
          file_failure (command_पङ्क्तिः_arg,
                        _("cannot access %s"), full_name);

          जइ (command_पङ्क्तिः_arg)
            {
              free_ent (f);
              प्रत्यागम 0;
            }

          f->name = xstrdup (name);
          cwd_n_used++;

          प्रत्यागम 0;
        }

      f->stat_ok = true;
      f->filetype = type = d_type_filetype[IFTODT (f->stat.st_mode)];
    }

  जइ (type == directory && command_पङ्क्तिः_arg && !immediate_dirs)
    f->filetype = type = arg_directory;

  get_extended_info (full_name, f, do_deref);

  जइ ((type == symbolic_link)
      & ((format == long_format) | check_symlink_mode))
    {
      संरचना stat linkstats;

      जइ (! get_link_name (full_name, f, command_पङ्क्तिः_arg))
        {
          free_ent (f);
          प्रत्यागम 0;
        }

      /* Use the slower quoting path for this entry, though
         don't update CWD_SOME_QUOTED since alignment not affected.  */
      जइ (f->linkname && f->quoted == 0 && needs_quoting (f->linkname))
        f->quoted = -1;

      /* Avoid following symbolic links when possible, i.e., when
         they won't be traced and when no indicator is needed.  */
      जइ (f->linkname
          && (file_type <= indicator_style || check_symlink_mode)
          && stat_for_mode (full_name, &linkstats) == 0)
        {
          f->linkok = true;
          f->linkmode = linkstats.st_mode;
        }
    }

  blocks = STP_NBLOCKS (&f->stat);
  जइ (format == long_format || print_block_size)
    {
      वर्ण buf[LONGEST_HUMAN_READABLE + 1];
      पूर्णाङ्क len = mbswidth (human_readable (blocks, buf, human_output_opts,
                                          ST_NBLOCKSIZE, output_block_size),
                          MBSWIDTH_FLAGS);
      जइ (block_size_width < len)
        block_size_width = len;
    }

  जइ (format == long_format)
    {
      जइ (print_owner)
        {
          पूर्णाङ्क len = format_user_width (f->stat.st_uid);
          जइ (owner_width < len)
            owner_width = len;
        }

      जइ (print_group)
        {
          पूर्णाङ्क len = format_group_width (f->stat.st_gid);
          जइ (group_width < len)
            group_width = len;
        }

      जइ (print_author)
        {
          पूर्णाङ्क len = format_user_width (f->stat.st_author);
          जइ (author_width < len)
            author_width = len;
        }
    }

  जइ (print_scontext)
    {
      पूर्णाङ्क len = strlen (f->scontext);
      जइ (scontext_width < len)
        scontext_width = len;
    }

  जइ (format == long_format)
    {
      वर्ण b[INT_BUFSIZE_BOUND (uintmax_t)];
      पूर्णाङ्क b_len = strlen (umaxtostr (f->stat.st_nlink, b));
      जइ (nlink_width < b_len)
        nlink_width = b_len;

      जइ ((type == chardev) | (type == blockdev))
        {
          वर्ण buf[INT_BUFSIZE_BOUND (uintmax_t)];
          पूर्णाङ्क len = strlen (umaxtostr (major (f->stat.st_rdev), buf));
          जइ (major_device_सङ्ख्या_width < len)
            major_device_सङ्ख्या_width = len;
          len = strlen (umaxtostr (minor (f->stat.st_rdev), buf));
          जइ (minor_device_सङ्ख्या_width < len)
            minor_device_सङ्ख्या_width = len;
          len = major_device_सङ्ख्या_width + 2 + minor_device_सङ्ख्या_width;
          जइ (file_size_width < len)
            file_size_width = len;
        }
      अण्णहा
        {
          वर्ण buf[LONGEST_HUMAN_READABLE + 1];
          uintmax_t size = unsigned_file_size (f->stat.st_size);
          पूर्णाङ्क len = mbswidth (human_readable (size, buf,
                                              file_human_output_opts,
                                              1, file_output_block_size),
                              MBSWIDTH_FLAGS);
          जइ (file_size_width < len)
            file_size_width = len;
        }
    }

  जइ (print_inode)
    {
      वर्ण buf[INT_BUFSIZE_BOUND (uintmax_t)];
      पूर्णाङ्क len = strlen (umaxtostr (f->stat.st_ino, buf));
      जइ (inode_सङ्ख्या_width < len)
        inode_सङ्ख्या_width = len;
    }

  f->name = xstrdup (name);
  cwd_n_used++;

  प्रत्यागम blocks;
}

/* Return true if F refers to a directory.  */
स्थिर bool
is_directory (ध्रुव संरचना fileinfo *f)
{
  प्रत्यागम f->filetype == directory || f->filetype == arg_directory;
}

/* Return true if F refers to a (symlinked) directory.  */
स्थिर bool
is_linked_directory (ध्रुव संरचना fileinfo *f)
{
  प्रत्यागम f->filetype == directory || f->filetype == arg_directory
         || S_ISDIR (f->linkmode);
}

/* Put the name of the file that FILENAME is a symbolic link to
   into the LINKNAME field of 'f'.  COMMAND_LINE_ARG indicates whether
   FILENAME is a command-line argument.  */

स्थिर bool
get_link_name (वर्ण ध्रुव *filename, संरचना fileinfo *f, bool command_पङ्क्तिः_arg)
{
  f->linkname = areadlink_with_size (filename, f->stat.st_size);
  जइ (f->linkname == रिक्त)
    {
      जइ (! command_पङ्क्तिः_arg
          && ignorable_traversal_errno (errno))
        प्रत्यागम false;
      file_failure (command_पङ्क्तिः_arg, _("cannot read symbolic link %s"),
                    filename);
    }
  प्रत्यागम true;
}

/* Return true if the last component of NAME is '.' or '..'
   This is so we don't try to recurse on '././././. ...' */

स्थिर bool
basename_is_dot_or_dotdot (वर्ण ध्रुव *name)
{
  वर्ण ध्रुव *base = last_component (name);
  प्रत्यागम dot_or_dotdot (base);
}

/* Remove any entries from CWD_FILE that are for directories,
   and queue them to be listed as directories instead.
   DIRNAME is the prefix to prepend to each dirname
   to make it correct relative to ls's working dir;
   if it is null, no prefix is needed and "." and ".." should not be ignored.
   If COMMAND_LINE_ARG is true, this directory was mentioned at the top level,
   This is desirable when processing directories recursively.  */

स्थिर शून्य
extract_dirs_from_files (वर्ण ध्रुव *dirname, bool command_पङ्क्तिः_arg)
{
  idx_t i, j;
  bool ignore_dot_and_dot_dot = (dirname != रिक्त);

  जइ (dirname && LOOP_DETECT)
    {
      /* Insert a marker entry first.  When we dequeue this marker entry,
         we'll know that DIRNAME has been processed and may be removed
         from the set of active directories.  */
      queue_directory (रिक्त, dirname, false);
    }

  /* Queue the directories last one first, because queueing reverses the
     order.  */
  प्रति (i = cwd_n_used; 0 < i; )
    {
      i--;
      संरचना fileinfo *f = sorted_file[i];

      जइ (is_directory (f)
          && (! ignore_dot_and_dot_dot
              || ! basename_is_dot_or_dotdot (f->name)))
        {
          जइ (!dirname || f->name[0] == '/')
            queue_directory (f->name, f->linkname, command_पङ्क्तिः_arg);
          अण्णहा
            {
              वर्ण *name = file_name_concat (dirname, f->name, रिक्त);
              queue_directory (name, f->linkname, command_पङ्क्तिः_arg);
              मुक्त (name);
            }
          जइ (f->filetype == arg_directory)
            free_ent (f);
        }
    }

  /* Now delete the directories from the table, compacting all the remaining
     entries.  */

  प्रति (i = 0, j = 0; i < cwd_n_used; i++)
    {
      संरचना fileinfo *f = sorted_file[i];
      sorted_file[j] = f;
      j += (f->filetype != arg_directory);
    }
  cwd_n_used = j;
}

/* Use strcoll to compare strings in this locale.  If an error occurs,
   report an error and longjmp to failed_strcoll.  */

स्थिर jmp_buf failed_strcoll;

स्थिर पूर्णाङ्क
xstrcoll (वर्ण ध्रुव *a, वर्ण ध्रुव *b)
{
  पूर्णाङ्क diff;
  errno = 0;
  diff = strcoll (a, b);
  जइ (errno)
    {
      error (0, errno, _("cannot compare file names %s and %s"),
             quote_n (0, a), quote_n (1, b));
      set_exit_status (false);
      longjmp (failed_strcoll, 1);
    }
  प्रत्यागम diff;
}

/* Comparison routines for sorting the files.  */

प्रकारनिर्देश शून्य ध्रुव *V;
प्रकारनिर्देश पूर्णाङ्क (*qsortFunc)(V a, V b);

/* Used below in DEFINE_SORT_FUNCTIONS for _df_ sort function variants.  */
स्थिर पूर्णाङ्क
dirfirst_check (संरचना fileinfo ध्रुव *a, संरचना fileinfo ध्रुव *b,
                पूर्णाङ्क (*cmp) (V, V))
{
  पूर्णाङ्क diff = is_linked_directory (b) - is_linked_directory (a);
  प्रत्यागम diff ? diff : cmp (a, b);
}

/* Define the 8 different sort function variants required for each sortkey.
   KEY_NAME is a token describing the sort key, e.g., ctime, atime, size.
   KEY_CMP_FUNC is a function to compare records based on that key, e.g.,
   ctime_cmp, atime_cmp, size_cmp.  Append KEY_NAME to the string,
   '[rev_][x]str{cmp|coll}[_df]_', to create each function name.  */
#define DEFINE_SORT_FUNCTIONS(कुञ्चिका_name, कुञ्चिका_cmp_func)			\
  /* direct, non-dirfirst versions */					\
  स्थिर पूर्णाङ्क xstrcoll_##key_name (V a, V b)				\
  { प्रत्यागम कुञ्चिका_cmp_func (a, b, xstrcoll); }				\
  ATTRIBUTE_PURE स्थिर पूर्णाङ्क strcmp_##key_name (V a, V b)		\
  { प्रत्यागम कुञ्चिका_cmp_func (a, b, strcmp); }				\
                                                                        \
  /* reverse, non-dirfirst versions */					\
  स्थिर पूर्णाङ्क rev_xstrcoll_##key_name (V a, V b)				\
  { प्रत्यागम कुञ्चिका_cmp_func (b, a, xstrcoll); }				\
  ATTRIBUTE_PURE स्थिर पूर्णाङ्क rev_strcmp_##key_name (V a, V b)	\
  { प्रत्यागम कुञ्चिका_cmp_func (b, a, strcmp); }				\
                                                                        \
  /* direct, dirfirst versions */					\
  स्थिर पूर्णाङ्क xstrcoll_df_##key_name (V a, V b)				\
  { प्रत्यागम dirfirst_check (a, b, xstrcoll_##key_name); }		\
  ATTRIBUTE_PURE स्थिर पूर्णाङ्क strcmp_df_##key_name (V a, V b)		\
  { प्रत्यागम dirfirst_check (a, b, strcmp_##key_name); }			\
                                                                        \
  /* reverse, dirfirst versions */					\
  स्थिर पूर्णाङ्क rev_xstrcoll_df_##key_name (V a, V b)			\
  { प्रत्यागम dirfirst_check (a, b, rev_xstrcoll_##key_name); }		\
  ATTRIBUTE_PURE स्थिर पूर्णाङ्क rev_strcmp_df_##key_name (V a, V b)	\
  { प्रत्यागम dirfirst_check (a, b, rev_strcmp_##key_name); }

स्थिर पूर्णाङ्क
cmp_ctime (संरचना fileinfo ध्रुव *a, संरचना fileinfo ध्रुव *b,
           पूर्णाङ्क (*cmp) (वर्ण ध्रुव *, वर्ण ध्रुव *))
{
  पूर्णाङ्क diff = timespec_cmp (get_stat_ctime (&b->stat),
                           get_stat_ctime (&a->stat));
  प्रत्यागम diff ? diff : cmp (a->name, b->name);
}

स्थिर पूर्णाङ्क
cmp_mtime (संरचना fileinfo ध्रुव *a, संरचना fileinfo ध्रुव *b,
           पूर्णाङ्क (*cmp) (वर्ण ध्रुव *, वर्ण ध्रुव *))
{
  पूर्णाङ्क diff = timespec_cmp (get_stat_mtime (&b->stat),
                           get_stat_mtime (&a->stat));
  प्रत्यागम diff ? diff : cmp (a->name, b->name);
}

स्थिर पूर्णाङ्क
cmp_atime (संरचना fileinfo ध्रुव *a, संरचना fileinfo ध्रुव *b,
           पूर्णाङ्क (*cmp) (वर्ण ध्रुव *, वर्ण ध्रुव *))
{
  पूर्णाङ्क diff = timespec_cmp (get_stat_atime (&b->stat),
                           get_stat_atime (&a->stat));
  प्रत्यागम diff ? diff : cmp (a->name, b->name);
}

स्थिर पूर्णाङ्क
cmp_btime (संरचना fileinfo ध्रुव *a, संरचना fileinfo ध्रुव *b,
           पूर्णाङ्क (*cmp) (वर्ण ध्रुव *, वर्ण ध्रुव *))
{
  पूर्णाङ्क diff = timespec_cmp (get_stat_btime (&b->stat),
                           get_stat_btime (&a->stat));
  प्रत्यागम diff ? diff : cmp (a->name, b->name);
}

स्थिर पूर्णाङ्क
off_cmp (off_t a, off_t b)
{
  प्रत्यागम _GL_CMP (a, b);
}

स्थिर पूर्णाङ्क
cmp_size (संरचना fileinfo ध्रुव *a, संरचना fileinfo ध्रुव *b,
          पूर्णाङ्क (*cmp) (वर्ण ध्रुव *, वर्ण ध्रुव *))
{
  पूर्णाङ्क diff = off_cmp (b->stat.st_size, a->stat.st_size);
  प्रत्यागम diff ? diff : cmp (a->name, b->name);
}

स्थिर पूर्णाङ्क
cmp_name (संरचना fileinfo ध्रुव *a, संरचना fileinfo ध्रुव *b,
          पूर्णाङ्क (*cmp) (वर्ण ध्रुव *, वर्ण ध्रुव *))
{
  प्रत्यागम cmp (a->name, b->name);
}

/* Compare file extensions.  Files with no extension are 'smallest'.
   If extensions are the same, compare by file names instead.  */

स्थिर पूर्णाङ्क
cmp_extension (संरचना fileinfo ध्रुव *a, संरचना fileinfo ध्रुव *b,
               पूर्णाङ्क (*cmp) (वर्ण ध्रुव *, वर्ण ध्रुव *))
{
  वर्ण ध्रुव *base1 = strrchr (a->name, '.');
  वर्ण ध्रुव *base2 = strrchr (b->name, '.');
  पूर्णाङ्क diff = cmp (base1 ? base1 : "", base2 ? base2 : "");
  प्रत्यागम diff ? diff : cmp (a->name, b->name);
}

/* Return the (cached) screen width,
   for the NAME associated with the passed fileinfo F.  */

स्थिर size_t
fileinfo_name_width (संरचना fileinfo ध्रुव *f)
{
  प्रत्यागम f->width
         ? f->width
         : quote_name_width (f->name, filename_quoting_options, f->quoted);
}

स्थिर पूर्णाङ्क
cmp_width (संरचना fileinfo ध्रुव *a, संरचना fileinfo ध्रुव *b,
          पूर्णाङ्क (*cmp) (वर्ण ध्रुव *, वर्ण ध्रुव *))
{
  पूर्णाङ्क diff = fileinfo_name_width (a) - fileinfo_name_width (b);
  प्रत्यागम diff ? diff : cmp (a->name, b->name);
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
स्थिर पूर्णाङ्क
cmp_version (संरचना fileinfo ध्रुव *a, संरचना fileinfo ध्रुव *b)
{
  पूर्णाङ्क diff = filevercmp (a->name, b->name);
  प्रत्यागम diff ? diff : strcmp (a->name, b->name);
}

स्थिर पूर्णाङ्क
xstrcoll_version (V a, V b)
{
  प्रत्यागम cmp_version (a, b);
}
स्थिर पूर्णाङ्क
rev_xstrcoll_version (V a, V b)
{
  प्रत्यागम cmp_version (b, a);
}
स्थिर पूर्णाङ्क
xstrcoll_df_version (V a, V b)
{
  प्रत्यागम dirfirst_check (a, b, xstrcoll_version);
}
स्थिर पूर्णाङ्क
rev_xstrcoll_df_version (V a, V b)
{
  प्रत्यागम dirfirst_check (a, b, rev_xstrcoll_version);
}


/* We have 2^3 different variants for each sort-key function
   (for 3 independent sort modes).
   The function pointers stored in this array must be dereferenced as:

    sort_variants[sort_key][use_strcmp][reverse][dirs_first]

   Note that the order in which sort keys are listed in the function pointer
   array below is defined by the order of the elements in the time_type and
   sort_type enums!  */

#define LIST_SORTFUNCTION_VARIANTS(कुञ्चिका_name)                        \
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

स्थिर qsortFunc ध्रुव sort_functions[][2][2][2] =
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
        { रिक्त, रिक्त },
        { रिक्त, रिक्त },
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

स्थिर शून्य
initialize_ordering_vector (शून्य)
{
  प्रति (idx_t i = 0; i < cwd_n_used; i++)
    sorted_file[i] = &cwd_file[i];
}

/* Cache values based on attributes global to all files.  */

स्थिर शून्य
update_current_files_info (शून्य)
{
  /* Cache screen width of name, if needed multiple times.  */
  जइ (sort_type == sort_width
      || (पङ्क्तिः_length && (format == many_per_पङ्क्तिः || format == horizontal)))
    {
      प्रति (idx_t i = 0; i < cwd_n_used; i++)
        {
          संरचना fileinfo *f = sorted_file[i];
          f->width = fileinfo_name_width (f);
        }
    }
}

/* Sort the files now in the table.  */

स्थिर शून्य
sort_files (शून्य)
{
  bool use_strcmp;

  जइ (sorted_file_alloc < cwd_n_used + (cwd_n_used >> 1))
    {
      मुक्त (sorted_file);
      sorted_file = xinmalloc (cwd_n_used, 3 * परिमाण *sorted_file);
      sorted_file_alloc = 3 * cwd_n_used;
    }

  initialize_ordering_vector ();

  update_current_files_info ();

  जइ (sort_type == sort_none)
    प्रत्यागम;

  /* Try strcoll.  If it fails, fall back on strcmp.  We can't safely
     ignore strcoll failures, as a failing strcoll might be a
     comparison function that is not a total order, and if we ignored
     the failure this might cause qsort to dump core.  */

  जइ (! setjmp (failed_strcoll))
    use_strcmp = false;      /* strcoll() succeeded */
  अण्णहा
    {
      use_strcmp = true;
      affirm (sort_type != sort_version);
      initialize_ordering_vector ();
    }

  /* When sort_type == sort_time, use time_type as subindex.  */
  mpsort ((शून्य ध्रुव **) sorted_file, cwd_n_used,
          sort_functions[sort_type + (sort_type == sort_time ? time_type : 0)]
                        [use_strcmp][sort_reverse]
                        [directories_first]);
}

/* List all the files now in the table.  */

स्थिर शून्य
print_current_files (शून्य)
{
  विकल्प (format)
    {
    पक्ष one_per_पङ्क्तिः:
      प्रति (idx_t i = 0; i < cwd_n_used; i++)
        {
          print_file_name_and_frills (sorted_file[i], 0);
          putchar (eolbyte);
        }
      भङ्ग;

    पक्ष many_per_पङ्क्तिः:
      जइ (! पङ्क्तिः_length)
        print_with_separator (' ');
      अण्णहा
        print_many_per_पङ्क्तिः ();
      भङ्ग;

    पक्ष horizontal:
      जइ (! पङ्क्तिः_length)
        print_with_separator (' ');
      अण्णहा
        print_horizontal ();
      भङ्ग;

    पक्ष with_commas:
      print_with_separator (',');
      भङ्ग;

    पक्ष long_format:
      प्रति (idx_t i = 0; i < cwd_n_used; i++)
        {
          set_normal_color ();
          print_long_format (sorted_file[i]);
          dired_outbyte (eolbyte);
        }
      भङ्ग;
    }
}

/* Replace the first %b with precomputed aligned month names.
   Note on glibc-2.7 at least, this speeds up the whole 'ls -lU'
   process by around 17%, compared to letting strftime() handle the %b.  */

स्थिर ptrdiff_t
align_nstrftime (वर्ण *buf, size_t size, bool recent, संरचना tm ध्रुव *tm,
                 timezone_t tz, पूर्णाङ्क ns)
{
  वर्ण ध्रुव *nfmt = (use_abformat
                      ? abformat[recent][tm->tm_mon]
                      : long_time_format[recent]);
  प्रत्यागम nstrftime (buf, size, nfmt, tm, tz, ns);
}

/* Return the expected number of columns in a long-format timestamp,
   or zero if it cannot be calculated.  */

स्थिर पूर्णाङ्क
long_time_expected_width (शून्य)
{
  स्थिर पूर्णाङ्क width = -1;

  जइ (width < 0)
    {
      time_t epoch = 0;
      संरचना tm tm;
      वर्ण buf[TIME_STAMP_LEN_MAXIMUM + 1];

      /* In case you're wondering if localtime_rz can fail with an input time_t
         value of 0, let's just say it's very unlikely, but not inconceivable.
         The TZ environment variable would have to specify a time zone that
         is 2**31-1900 years or more ahead of UTC.  This could happen only on
         a 64-bit system that blindly accepts e.g., TZ=UTC+20000000000000.
         However, this is not possible with Solaris 10 or glibc-2.3.5, since
         their implementations limit the offset to 167:59 and 24:00, resp.  */
      जइ (localtime_rz (localtz, &epoch, &tm))
        {
          ptrdiff_t len = align_nstrftime (buf, परिमाण buf, false,
                                           &tm, localtz, 0);
          जइ (len > 0)
            width = mbsnwidth (buf, len, MBSWIDTH_FLAGS);
        }

      जइ (width < 0)
        width = 0;
    }

  प्रत्यागम width;
}

/* Print the user or group name NAME, with numeric id ID, using a
   print width of WIDTH columns.  */

स्थिर शून्य
format_user_or_group (वर्ण ध्रुव *name, uintmax_t id, पूर्णाङ्क width)
{
  जइ (name)
    {
      पूर्णाङ्क name_width = mbswidth (name, MBSWIDTH_FLAGS);
      पूर्णाङ्क width_gap = name_width < 0 ? 0 : width - name_width;
      पूर्णाङ्क pad = MAX (0, width_gap);
      dired_outstring (name);

      करेहि
        dired_outbyte (' ');
      जाव (pad--);
    }
  अण्णहा
    dired_pos += printf ("%*ju ", width, id);
}

/* Print the name or id of the user with id U, using a print width of
   WIDTH.  */

स्थिर शून्य
format_user (uid_t u, पूर्णाङ्क width, bool stat_ok)
{
  format_user_or_group (! stat_ok ? "?" :
                        (numeric_ids ? रिक्त : getuser (u)), u, width);
}

/* Likewise, for groups.  */

स्थिर शून्य
format_group (gid_t g, पूर्णाङ्क width, bool stat_ok)
{
  format_user_or_group (! stat_ok ? "?" :
                        (numeric_ids ? रिक्त : getgroup (g)), g, width);
}

/* Return the number of columns that format_user_or_group will print,
   or -1 if unknown.  */

स्थिर पूर्णाङ्क
format_user_or_group_width (वर्ण ध्रुव *name, uintmax_t id)
{
  प्रत्यागम (name
          ? mbswidth (name, MBSWIDTH_FLAGS)
          : snprintf (रिक्त, 0, "%ju", id));
}

/* Return the number of columns that format_user will print,
   or -1 if unknown.  */

स्थिर पूर्णाङ्क
format_user_width (uid_t u)
{
  प्रत्यागम format_user_or_group_width (numeric_ids ? रिक्त : getuser (u), u);
}

/* Likewise, for groups.  */

स्थिर पूर्णाङ्क
format_group_width (gid_t g)
{
  प्रत्यागम format_user_or_group_width (numeric_ids ? रिक्त : getgroup (g), g);
}

/* Return a pointer to a formatted version of F->stat.st_ino,
   possibly using buffer, which must be at least
   INT_BUFSIZE_BOUND (uintmax_t) bytes.  */
स्थिर वर्ण *
format_inode (वर्ण buf[INT_BUFSIZE_BOUND (uintmax_t)],
              ध्रुव संरचना fileinfo *f)
{
  प्रत्यागम (f->stat_ok && f->stat.st_ino != NOT_AN_INODE_सङ्ख्या
          ? umaxtostr (f->stat.st_ino, buf)
          : (वर्ण *) "?");
}

/* Print information about F in long format.  */
स्थिर शून्य
print_long_format (ध्रुव संरचना fileinfo *f)
{
  वर्ण modebuf[12];
  वर्ण buf
    [LONGEST_HUMAN_READABLE + 1		/* inode */
     + LONGEST_HUMAN_READABLE + 1	/* size in blocks */
     + परिमाण (modebuf) - 1 + 1		/* mode string */
     + INT_BUFSIZE_BOUND (uintmax_t)	/* st_nlink */
     + LONGEST_HUMAN_READABLE + 2	/* major device number */
     + LONGEST_HUMAN_READABLE + 1	/* minor device number */
     + TIME_STAMP_LEN_MAXIMUM + 1	/* max length of time/date */
     ];
  ptrdiff_t s;
  वर्ण *p;
  संरचना timespec when_timespec;
  संरचना tm when_local;
  bool btime_ok = true;

  /* Compute the mode string, except remove the trailing space if no
     file in this directory has an ACL or security context.  */
  जइ (f->stat_ok)
    filemodestring (&f->stat, modebuf);
  अण्णहा
    {
      modebuf[0] = filetype_letter[f->filetype];
      memset (modebuf + 1, '?', 10);
      modebuf[11] = '\0';
    }
  जइ (! any_has_acl)
    modebuf[10] = '\0';
  अण्णहा जइ (f->acl_type == ACL_T_LSM_CONTEXT_ONLY)
    modebuf[10] = '.';
  अण्णहा जइ (f->acl_type == ACL_T_YES)
    modebuf[10] = '+';
  अण्णहा जइ (f->acl_type == ACL_T_UNKNOWN)
    modebuf[10] = '?';

  विकल्प (time_type)
    {
    पक्ष time_ctime:
      when_timespec = get_stat_ctime (&f->stat);
      भङ्ग;
    पक्ष time_mtime:
      when_timespec = get_stat_mtime (&f->stat);
      भङ्ग;
    पक्ष time_atime:
      when_timespec = get_stat_atime (&f->stat);
      भङ्ग;
    पक्ष time_btime:
      when_timespec = get_stat_btime (&f->stat);
      जइ (when_timespec.tv_sec == -1 && when_timespec.tv_nsec == -1)
        btime_ok = false;
      भङ्ग;
    पक्ष time_numtypes: स्वभाव:
      unreachable ();
    }

  p = buf;

  जइ (print_inode)
    {
      वर्ण hbuf[INT_BUFSIZE_BOUND (uintmax_t)];
      p += sprintf (p, "%*s ", inode_सङ्ख्या_width, format_inode (hbuf, f));
    }

  जइ (print_block_size)
    {
      वर्ण hbuf[LONGEST_HUMAN_READABLE + 1];
      वर्ण ध्रुव *blocks =
        (! f->stat_ok
         ? "?"
         : human_readable (STP_NBLOCKS (&f->stat), hbuf, human_output_opts,
                           ST_NBLOCKSIZE, output_block_size));
      पूर्णाङ्क blocks_width = mbswidth (blocks, MBSWIDTH_FLAGS);
      प्रति (पूर्णाङ्क pad = blocks_width < 0 ? 0 : block_size_width - blocks_width;
           0 < pad; pad--)
        *p++ = ' ';
      जाव ((*p++ = *blocks++))
        अनुवर्त;
      p[-1] = ' ';
    }

  /* The last byte of the mode string is the POSIX
     "optional alternate access method flag".  */
  {
    वर्ण hbuf[INT_BUFSIZE_BOUND (uintmax_t)];
    p += sprintf (p, "%s %*s ", modebuf, nlink_width,
                  ! f->stat_ok ? "?" : umaxtostr (f->stat.st_nlink, hbuf));
  }

  dired_indent ();

  जइ (print_owner || print_group || print_author || print_scontext)
    {
      dired_outbuf (buf, p - buf);

      जइ (print_owner)
        format_user (f->stat.st_uid, owner_width, f->stat_ok);

      जइ (print_group)
        format_group (f->stat.st_gid, group_width, f->stat_ok);

      जइ (print_author)
        format_user (f->stat.st_author, author_width, f->stat_ok);

      जइ (print_scontext)
        format_user_or_group (f->scontext, 0, scontext_width);

      p = buf;
    }

  जइ (f->stat_ok
      && (S_ISCHR (f->stat.st_mode) || S_ISBLK (f->stat.st_mode)))
    {
      पूर्णाङ्क blanks_width = (file_size_width
                          - (major_device_सङ्ख्या_width + 2
                             + minor_device_सङ्ख्या_width));
      p += sprintf (p, "%*ju, %*ju ",
                    major_device_सङ्ख्या_width + MAX (0, blanks_width),
                    (uintmax_t) major (f->stat.st_rdev),
                    minor_device_सङ्ख्या_width,
                    (uintmax_t) minor (f->stat.st_rdev));
    }
  अण्णहा
    {
      वर्ण hbuf[LONGEST_HUMAN_READABLE + 1];
      वर्ण ध्रुव *size =
        (! f->stat_ok
         ? "?"
         : human_readable (unsigned_file_size (f->stat.st_size),
                           hbuf, file_human_output_opts, 1,
                           file_output_block_size));
      पूर्णाङ्क size_width = mbswidth (size, MBSWIDTH_FLAGS);
      प्रति (पूर्णाङ्क pad = size_width < 0 ? 0 : file_size_width - size_width;
           0 < pad; pad--)
        *p++ = ' ';
      जाव ((*p++ = *size++))
        अनुवर्त;
      p[-1] = ' ';
    }

  s = -1;

  जइ (f->stat_ok && btime_ok
      && localtime_rz (localtz, &when_timespec.tv_sec, &when_local))
    {
      संरचना timespec six_months_ago;
      bool recent;

      /* If the file appears to be in the future, update the current
         time, in case the file happens to have been modified since
         the last time we checked the clock.  */
      जइ (timespec_cmp (current_time, when_timespec) < 0)
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

  जइ (0 <= s)
    {
      p += s;
      *p++ = ' ';
    }
  अण्णहा
    {
      /* The time cannot be converted using the desired format, so
         print it as a huge integer number of seconds.  */
      वर्ण hbuf[INT_BUFSIZE_BOUND (intmax_t)];
      p += sprintf (p, "%*s ", long_time_expected_width (),
                    (! f->stat_ok || ! btime_ok
                     ? "?"
                     : timetostr (when_timespec.tv_sec, hbuf)));
      /* FIXME: (maybe) We discarded when_timespec.tv_nsec. */
    }

  dired_outbuf (buf, p - buf);
  size_t w = print_name_with_quoting (f, false, &dired_obstack, p - buf);

  जइ (f->filetype == symbolic_link)
    {
      जइ (f->linkname)
        {
          dired_outstring (" -> ");
          print_name_with_quoting (f, true, रिक्त, (p - buf) + w + 4);
          जइ (indicator_style != none)
            print_type_indicator (true, f->linkmode, unknown);
        }
    }
  अण्णहा जइ (indicator_style != none)
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

स्थिर size_t
quote_name_buf (वर्ण **inbuf, size_t bufsize, वर्ण *name,
                संरचना quoting_options ध्रुव *options,
                पूर्णाङ्क needs_general_quoting, size_t *width, bool *pad)
{
  वर्ण *buf = *inbuf;
  size_t displayed_width IF_LINT ( = 0);
  size_t len = 0;
  bool quoted;

  गणना quoting_style qs = get_quoting_style (options);
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

  जइ (needs_general_quoting != 0)
    {
      len = quotearg_buffer (buf, bufsize, name, -1, options);
      जइ (bufsize <= len)
        {
          buf = xmalloc (len + 1);
          quotearg_buffer (buf, len + 1, name, -1, options);
        }

      quoted = (*name != *buf) || strlen (name) != len;
    }
  अण्णहा जइ (needs_further_quoting || extra_newline_protection)
    {
      len = strlen (name);
      जइ (bufsize <= len)
        buf = xmalloc (len + 1);
      memcpy (buf, name, len + 1);

      quoted = false;
    }
  अण्णहा
    {
      len = strlen (name);
      buf = name;
      quoted = false;
    }

  /* It's safe to replace newlines without multi-byte iteration, as newlines
     do not appear as part of any multi-byte encoded character.  */
  जइ (extra_newline_protection)
    प्रति (वर्ण *p = buf; p < buf + len; p++)
      जइ (eolbyte && *p == '\n')
        *p = '?';

  जइ (needs_further_quoting)
    {
      जइ (MB_CUR_MAX > 1)
        {
          वर्ण ध्रुव *p = buf;
          वर्ण ध्रुव *plimit = buf + len;
          वर्ण *q = buf;
          displayed_width = 0;

          जाव (p < plimit)
            विकल्प (*p)
              {
                पक्ष ' ': पक्ष '!': पक्ष '"': पक्ष '#': पक्ष '%':
                पक्ष '&': पक्ष '\'': पक्ष '(': पक्ष ')': पक्ष '*':
                पक्ष '+': पक्ष ',': पक्ष '-': पक्ष '.': पक्ष '/':
                पक्ष '0': पक्ष '1': पक्ष '2': पक्ष '3': पक्ष '4':
                पक्ष '5': पक्ष '6': पक्ष '7': पक्ष '8': पक्ष '9':
                पक्ष ':': पक्ष ';': पक्ष '<': पक्ष '=': पक्ष '>':
                पक्ष '?':
                पक्ष 'A': पक्ष 'B': पक्ष 'C': पक्ष 'D': पक्ष 'E':
                पक्ष 'F': पक्ष 'G': पक्ष 'H': पक्ष 'I': पक्ष 'J':
                पक्ष 'K': पक्ष 'L': पक्ष 'M': पक्ष 'N': पक्ष 'O':
                पक्ष 'P': पक्ष 'Q': पक्ष 'R': पक्ष 'S': पक्ष 'T':
                पक्ष 'U': पक्ष 'V': पक्ष 'W': पक्ष 'X': पक्ष 'Y':
                पक्ष 'Z':
                पक्ष '[': पक्ष '\\': पक्ष ']': पक्ष '^': पक्ष '_':
                पक्ष 'a': पक्ष 'b': पक्ष 'c': पक्ष 'd': पक्ष 'e':
                पक्ष 'f': पक्ष 'g': पक्ष 'h': पक्ष 'i': पक्ष 'j':
                पक्ष 'k': पक्ष 'l': पक्ष 'm': पक्ष 'n': पक्ष 'o':
                पक्ष 'p': पक्ष 'q': पक्ष 'r': पक्ष 's': पक्ष 't':
                पक्ष 'u': पक्ष 'v': पक्ष 'w': पक्ष 'x': पक्ष 'y':
                पक्ष 'z': पक्ष '{': पक्ष '|': पक्ष '}': पक्ष '~':
                  /* These characters are printable ASCII characters.  */
                  *q++ = *p++;
                  displayed_width += 1;
                  भङ्ग;
                स्वभाव:
                  /* If we have a multibyte sequence, copy it until we
                     reach its end, replacing each non-printable multibyte
                     character with a single question mark.  */
                  {
                    mbstate_t mbstate; mbszero (&mbstate);
                    करेहि
                      {
                        char32_t wc;
                        size_t bytes;
                        पूर्णाङ्क w;

                        bytes = mbrtoc32 (&wc, p, plimit - p, &mbstate);

                        जइ (bytes == (size_t) -1)
                          {
                            /* An invalid multibyte sequence was
                               encountered.  Skip one input byte, and
                               put a question mark.  */
                            p++;
                            *q++ = '?';
                            displayed_width += 1;
                            भङ्ग;
                          }

                        जइ (bytes == (size_t) -2)
                          {
                            /* An incomplete multibyte character
                               at the end.  Replace it entirely with
                               a question mark.  */
                            p = plimit;
                            *q++ = '?';
                            displayed_width += 1;
                            भङ्ग;
                          }

                        जइ (bytes == 0)
                          /* A null wide character was encountered.  */
                          bytes = 1;

                        w = c32width (wc);
                        जइ (w >= 0)
                          {
                            /* A printable multibyte character.
                               Keep it.  */
                            प्रति (; bytes > 0; --bytes)
                              *q++ = *p++;
                            displayed_width += w;
                          }
                        अण्णहा
                          {
                            /* An nonprintable multibyte character.
                               Replace it entirely with a question
                               mark.  */
                            p += bytes;
                            *q++ = '?';
                            displayed_width += 1;
                          }
                      }
                    जाव (! mbsinit (&mbstate));
                  }
                  भङ्ग;
              }

          /* The buffer may have shrunk.  */
          len = q - buf;
        }
      अण्णहा
        {
          वर्ण *p = buf;
          वर्ण ध्रुव *plimit = buf + len;

          जाव (p < plimit)
            {
              जइ (! isprint (to_uchar (*p)))
                *p = '?';
              p++;
            }
          displayed_width = len;
        }
    }
  अण्णहा जइ (width != रिक्त)
    {
      जइ (MB_CUR_MAX > 1)
        {
          displayed_width = mbsnwidth (buf, len, MBSWIDTH_FLAGS);
          displayed_width = MAX (0, displayed_width);
        }
      अण्णहा
        {
          वर्ण ध्रुव *p = buf;
          वर्ण ध्रुव *plimit = buf + len;

          displayed_width = 0;
          जाव (p < plimit)
            {
              जइ (isprint (to_uchar (*p)))
                displayed_width++;
              p++;
            }
        }
    }

  /* Set padding to better align quoted items,
     and also give a visual indication that quotes are
     not actually part of the name.  */
  *pad = (align_variable_outer_quotes && cwd_some_quoted && ! quoted);

  जइ (width != रिक्त)
    *width = displayed_width;

  *inbuf = buf;

  प्रत्यागम len;
}

स्थिर size_t
quote_name_width (वर्ण ध्रुव *name, संरचना quoting_options ध्रुव *options,
                  पूर्णाङ्क needs_general_quoting)
{
  वर्ण smallbuf[BUFSIZ];
  वर्ण *buf = smallbuf;
  size_t width;
  bool pad;

  quote_name_buf (&buf, परिमाण smallbuf, (वर्ण *) name, options,
                  needs_general_quoting, &width, &pad);

  जइ (buf != smallbuf && buf != name)
    मुक्त (buf);

  width += pad;

  प्रत्यागम width;
}

/* %XX escape any input out of range as defined in RFC3986,
   and also if PATH, convert all path separators to '/'.  */
स्थिर वर्ण *
file_escape (वर्ण ध्रुव *str, bool path)
{
  वर्ण *esc = xnmalloc (3, strlen (str) + 1);
  वर्ण *p = esc;
  जाव (*str)
    {
      जइ (path && ISSLASH (*str))
        {
          *p++ = '/';
          str++;
        }
      अण्णहा जइ (RFC3986[to_uchar (*str)])
        *p++ = *str++;
      अण्णहा
        p += sprintf (p, "%%%02x", to_uchar (*str++));
    }
  *p = '\0';
  प्रत्यागम esc;
}

स्थिर size_t
quote_name (वर्ण ध्रुव *name, संरचना quoting_options ध्रुव *options,
            पूर्णाङ्क needs_general_quoting, ध्रुव संरचना bin_str *color,
            bool allow_pad, संरचना obstack *राशिः, वर्ण ध्रुव *absolute_name)
{
  वर्ण smallbuf[BUFSIZ];
  वर्ण *buf = smallbuf;
  size_t len;
  bool pad;

  len = quote_name_buf (&buf, परिमाण smallbuf, (वर्ण *) name, options,
                        needs_general_quoting, रिक्त, &pad);

  जइ (pad && allow_pad)
    dired_outbyte (' ');

  जइ (color)
    print_color_indicator (color);

  /* If we're padding, then don't include the outer quotes in
     the --hyperlink, to improve the alignment of those links.  */
  bool skip_quotes = false;

  जइ (absolute_name)
    {
      जइ (align_variable_outer_quotes && cwd_some_quoted && ! pad)
        {
          skip_quotes = true;
          putchar (*buf);
        }
      वर्ण *h = file_escape (hostname, /* path= */ false);
      वर्ण *n = file_escape (absolute_name, /* path= */ true);
      /* TODO: It would be good to be able to define parameters
         to give hints to the terminal as how best to render the URI.
         For example since ls is outputting a dense block of URIs
         it would be best to not underline by default, and only
         do so upon hover etc.  */
      printf ("\033]8;;file://%s%s%s\033\\", h, *n == '/' ? "" : "/", n);
      मुक्त (h);
      मुक्त (n);
    }

  जइ (राशिः)
    प्रक्षिप_current_dired_pos (राशिः);

  fwrite (buf + skip_quotes, 1, len - (skip_quotes * 2), stdout);

  dired_pos += len;

  जइ (राशिः)
    प्रक्षिप_current_dired_pos (राशिः);

  जइ (absolute_name)
    {
      fputs ("\033]8;;\033\\", stdout);
      जइ (skip_quotes)
        putchar (*(buf + len - 1));
    }

  जइ (buf != smallbuf && buf != name)
    मुक्त (buf);

  प्रत्यागम len + pad;
}

स्थिर size_t
print_name_with_quoting (ध्रुव संरचना fileinfo *f,
                         bool symlink_target,
                         संरचना obstack *राशिः,
                         size_t start_col)
{
  वर्ण ध्रुव *name = symlink_target ? f->linkname : f->name;

  ध्रुव संरचना bin_str *color
    = print_with_color ? get_color_indicator (f, symlink_target) : रिक्त;

  bool used_color_this_time = (print_with_color
                               && (color || is_colored (C_NORM)));

  size_t len = quote_name (name, filename_quoting_options, f->quoted,
                           color, !symlink_target, राशिः, f->absolute_name);

  process_signals ();
  जइ (used_color_this_time)
    {
      prep_non_filename_text ();

      /* We use the byte length rather than display width here as
         an optimization to avoid accurately calculating the width,
         because we only output the clear to EOL sequence if the name
         _might_ wrap to the next line.  This may output a sequence
         unnecessarily in multi-byte locales for example,
         but in that case it's inconsequential to the output.  */
      जइ (पङ्क्तिः_length
          && (start_col / पङ्क्तिः_length != (start_col + len - 1) / पङ्क्तिः_length))
        put_indicator (&color_indicator[C_CLR_TO_EOL]);
    }

  प्रत्यागम len;
}

स्थिर शून्य
prep_non_filename_text (शून्य)
{
  जइ (color_indicator[C_END].string != रिक्त)
    put_indicator (&color_indicator[C_END]);
  अण्णहा
    {
      put_indicator (&color_indicator[C_वामः]);
      put_indicator (&color_indicator[C_RESET]);
      put_indicator (&color_indicator[C_दक्षिणः]);
    }
}

/* Print the file name of 'f' with appropriate quoting.
   Also print file size, inode number, and filetype indicator character,
   as requested by switches.  */

स्थिर size_t
print_file_name_and_frills (ध्रुव संरचना fileinfo *f, size_t start_col)
{
  वर्ण buf[MAX (LONGEST_HUMAN_READABLE + 1, INT_BUFSIZE_BOUND (uintmax_t))];

  set_normal_color ();

  जइ (print_inode)
    printf ("%*s ", format == with_commas ? 0 : inode_सङ्ख्या_width,
            format_inode (buf, f));

  जइ (print_block_size)
    {
      वर्ण ध्रुव *blocks =
        (! f->stat_ok
         ? "?"
         : human_readable (STP_NBLOCKS (&f->stat), buf, human_output_opts,
                           ST_NBLOCKSIZE, output_block_size));
      पूर्णाङ्क blocks_width = mbswidth (blocks, MBSWIDTH_FLAGS);
      पूर्णाङ्क pad = 0;
      जइ (0 <= blocks_width && block_size_width && format != with_commas)
        pad = block_size_width - blocks_width;
      printf ("%*s%s ", pad, "", blocks);
    }

  जइ (print_scontext)
    printf ("%*s ", format == with_commas ? 0 : scontext_width, f->scontext);

  size_t width = print_name_with_quoting (f, false, रिक्त, start_col);

  जइ (indicator_style != none)
    width += print_type_indicator (f->stat_ok, f->stat.st_mode, f->filetype);

  प्रत्यागम width;
}

/* Given these arguments describing a file, return the single-byte
   type indicator, or 0.  */
स्थिर वर्ण
get_type_indicator (bool stat_ok, mode_t mode, गणना filetype type)
{
  वर्ण c;

  जइ (stat_ok ? S_ISREG (mode) : type == normal)
    {
      जइ (stat_ok && indicator_style == classify && (mode & S_IXUGO))
        c = '*';
      अण्णहा
        c = 0;
    }
  अण्णहा
    {
      जइ (stat_ok ? S_ISDIR (mode) : type == directory || type == arg_directory)
        c = '/';
      अण्णहा जइ (indicator_style == slash)
        c = 0;
      अण्णहा जइ (stat_ok ? S_ISLNK (mode) : type == symbolic_link)
        c = '@';
      अण्णहा जइ (stat_ok ? S_ISFIFO (mode) : type == fifo)
        c = '|';
      अण्णहा जइ (stat_ok ? S_ISSOCK (mode) : type == sock)
        c = '=';
      अण्णहा जइ (stat_ok && S_ISDOOR (mode))
        c = '>';
      अण्णहा
        c = 0;
    }
  प्रत्यागम c;
}

स्थिर bool
print_type_indicator (bool stat_ok, mode_t mode, गणना filetype type)
{
  वर्ण c = get_type_indicator (stat_ok, mode, type);
  जइ (c)
    dired_outbyte (c);
  प्रत्यागम !!c;
}

/* Returns if color sequence was printed.  */
स्थिर bool
print_color_indicator (ध्रुव संरचना bin_str *ind)
{
  जइ (ind)
    {
      /* Need to reset so not dealing with attribute combinations */
      जइ (is_colored (C_NORM))
        restore_default_color ();
      put_indicator (&color_indicator[C_वामः]);
      put_indicator (ind);
      put_indicator (&color_indicator[C_दक्षिणः]);
    }

  प्रत्यागम ind != रिक्त;
}

/* Returns color indicator or NULL if none.  */
ATTRIBUTE_PURE
स्थिर ध्रुव संरचना bin_str *
get_color_indicator (ध्रुव संरचना fileinfo *f, bool symlink_target)
{
  गणना indicator_no type;
  संरचना color_ext_type *ext;	/* Color extension */
  size_t len;			/* Length of name */

  वर्ण ध्रुव *name;
  mode_t mode;
  पूर्णाङ्क linkok;
  जइ (symlink_target)
    {
      name = f->linkname;
      mode = f->linkmode;
      linkok = f->linkok ? 0 : -1;
    }
  अण्णहा
    {
      name = f->name;
      mode = file_or_link_mode (f);
      linkok = f->linkok;
    }

  /* Is this a nonexistent file?  If so, linkok == -1.  */

  जइ (linkok == -1 && is_colored (C_MISSING))
    type = C_MISSING;
  अण्णहा जइ (!f->stat_ok)
    {
      स्थिर गणना indicator_no ध्रुव filetype_indicator[] =
        {
          C_ORPHAN, C_FIFO, C_CHR, C_DIR, C_BLK, C_FILE,
          C_LINK, C_SOCK, C_FILE, C_DIR
        };
      static_assert (countof (filetype_indicator) == filetype_cardinality);
      type = filetype_indicator[f->filetype];
    }
  अण्णहा
    {
      जइ (S_ISREG (mode))
        {
          type = C_FILE;

          जइ ((mode & S_ISUID) != 0 && is_colored (C_SETUID))
            type = C_SETUID;
          अण्णहा जइ ((mode & S_ISGID) != 0 && is_colored (C_SETGID))
            type = C_SETGID;
          अण्णहा जइ (f->has_capability)
            type = C_CAP;
          अण्णहा जइ ((mode & S_IXUGO) != 0 && is_colored (C_EXEC))
            type = C_EXEC;
          अण्णहा जइ ((1 < f->stat.st_nlink) && is_colored (C_MULTIHARDLINK))
            type = C_MULTIHARDLINK;
        }
      अण्णहा जइ (S_ISDIR (mode))
        {
          type = C_DIR;

          जइ ((mode & S_ISVTX) && (mode & S_IWOTH)
              && is_colored (C_STICKY_OTHER_WRITABLE))
            type = C_STICKY_OTHER_WRITABLE;
          अण्णहा जइ ((mode & S_IWOTH) != 0 && is_colored (C_OTHER_WRITABLE))
            type = C_OTHER_WRITABLE;
          अण्णहा जइ ((mode & S_ISVTX) != 0 && is_colored (C_STICKY))
            type = C_STICKY;
        }
      अण्णहा जइ (S_ISLNK (mode))
        type = C_LINK;
      अण्णहा जइ (S_ISFIFO (mode))
        type = C_FIFO;
      अण्णहा जइ (S_ISSOCK (mode))
        type = C_SOCK;
      अण्णहा जइ (S_ISBLK (mode))
        type = C_BLK;
      अण्णहा जइ (S_ISCHR (mode))
        type = C_CHR;
      अण्णहा जइ (S_ISDOOR (mode))
        type = C_DOOR;
      अण्णहा
        {
          /* Classify a file of some other type as C_ORPHAN.  */
          type = C_ORPHAN;
        }
    }

  /* Check the file's suffix only if still classified as C_FILE.  */
  ext = रिक्त;
  जइ (type == C_FILE)
    {
      /* Test if NAME has a recognized suffix.  */

      len = strlen (name);
      name += len;		/* Pointer to final \0.  */
      प्रति (ext = color_ext_list; ext != रिक्त; ext = ext->next)
        {
          जइ (ext->ext.len <= len)
            {
              जइ (ext->exact_match)
                {
                  जइ (STREQ_LEN (name - ext->ext.len, ext->ext.string,
                                 ext->ext.len))
                    भङ्ग;
                }
              अण्णहा
                {
                  जइ (c_strncasecmp (name - ext->ext.len, ext->ext.string,
                                     ext->ext.len) == 0)
                    भङ्ग;
                }
            }
        }
    }

  /* Adjust the color for orphaned symlinks.  */
  जइ (type == C_LINK && !linkok)
    {
      जइ (color_symlink_as_referent || is_colored (C_ORPHAN))
        type = C_ORPHAN;
    }

  ध्रुव संरचना bin_str *ध्रुव s
    = ext ? &(ext->seq) : &color_indicator[type];

  प्रत्यागम s->string ? s : रिक्त;
}

/* Output a color indicator (which may contain nulls).
   Note we don't update dired_pos here as emacs can be configured
   to handle color codes by stripping/converting them.  */
स्थिर शून्य
put_indicator (ध्रुव संरचना bin_str *ind)
{
  जइ (! used_color)
    {
      used_color = true;

      /* If the standard output is a controlling terminal, watch out
         for signals, so that the colors can be restored to the
         default state if "ls" is suspended or interrupted.  */

      जइ (0 <= tcgetpgrp (STDOUT_FILENO))
        signal_init ();

      prep_non_filename_text ();
    }

  fwrite (ind->string, ind->len, 1, stdout);
}

स्थिर size_t
length_of_file_name_and_frills (ध्रुव संरचना fileinfo *f)
{
  size_t len = 0;
  वर्ण buf[MAX (LONGEST_HUMAN_READABLE + 1, INT_BUFSIZE_BOUND (uintmax_t))];

  जइ (print_inode)
    len += 1 + (format == with_commas
                ? strlen (umaxtostr (f->stat.st_ino, buf))
                : inode_सङ्ख्या_width);

  जइ (print_block_size)
    len += 1 + (format == with_commas
                ? strlen (! f->stat_ok ? "?"
                          : human_readable (STP_NBLOCKS (&f->stat), buf,
                                            human_output_opts, ST_NBLOCKSIZE,
                                            output_block_size))
                : block_size_width);

  जइ (print_scontext)
    len += 1 + (format == with_commas ? strlen (f->scontext) : scontext_width);

  len += fileinfo_name_width (f);

  जइ (indicator_style != none)
    {
      वर्ण c = get_type_indicator (f->stat_ok, f->stat.st_mode, f->filetype);
      len += (c != 0);
    }

  प्रत्यागम len;
}

स्थिर शून्य
print_many_per_पङ्क्तिः (शून्य)
{
  idx_t cols = calculate_columns (true);
  संरचना column_info ध्रुव *पङ्क्तिः_fmt = &column_info[cols - 1];

  /* Calculate the number of rows that will be in each column except possibly
     for a short column on the right.  */
  idx_t rows = cwd_n_used / cols + (cwd_n_used % cols != 0);

  प्रति (idx_t row = 0; row < rows; row++)
    {
      size_t col = 0;
      idx_t filesno = row;
      size_t pos = 0;

      /* Print the next row.  */
      जाव (true)
        {
          संरचना fileinfo ध्रुव *f = sorted_file[filesno];
          size_t name_length = length_of_file_name_and_frills (f);
          size_t max_name_length = पङ्क्तिः_fmt->col_arr[col++];
          print_file_name_and_frills (f, pos);

          जइ (cwd_n_used - rows <= filesno)
            भङ्ग;
          filesno += rows;

          indent (pos + name_length, pos + max_name_length);
          pos += max_name_length;
        }
      putchar (eolbyte);
    }
}

स्थिर शून्य
print_horizontal (शून्य)
{
  size_t pos = 0;
  idx_t cols = calculate_columns (false);
  संरचना column_info ध्रुव *पङ्क्तिः_fmt = &column_info[cols - 1];
  संरचना fileinfo ध्रुव *f = sorted_file[0];
  size_t name_length = length_of_file_name_and_frills (f);
  size_t max_name_length = पङ्क्तिः_fmt->col_arr[0];

  /* Print first entry.  */
  print_file_name_and_frills (f, 0);

  /* Now the rest.  */
  प्रति (idx_t filesno = 1; filesno < cwd_n_used; filesno++)
    {
      idx_t col = filesno % cols;

      जइ (col == 0)
        {
          putchar (eolbyte);
          pos = 0;
        }
      अण्णहा
        {
          indent (pos + name_length, pos + max_name_length);
          pos += max_name_length;
        }

      f = sorted_file[filesno];
      print_file_name_and_frills (f, pos);

      name_length = length_of_file_name_and_frills (f);
      max_name_length = पङ्क्तिः_fmt->col_arr[col];
    }
  putchar (eolbyte);
}

/* Output name + SEP + ' '.  */

स्थिर शून्य
print_with_separator (वर्ण sep)
{
  size_t pos = 0;

  प्रति (idx_t filesno = 0; filesno < cwd_n_used; filesno++)
    {
      संरचना fileinfo ध्रुव *f = sorted_file[filesno];
      size_t len = पङ्क्तिः_length ? length_of_file_name_and_frills (f) : 0;

      जइ (filesno != 0)
        {
          वर्ण separator;
          size_t next_pos = 2 + (sep == ',' && filesno < cwd_n_used - 1);

          जइ (! पङ्क्तिः_length
              || (! ckd_add (&next_pos, next_pos, pos)
                  && ! ckd_add (&next_pos, next_pos, len)
                  && next_pos <= पङ्क्तिः_length))
            {
              pos += 2;
              separator = ' ';
            }
          अण्णहा
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

स्थिर शून्य
indent (size_t from, size_t to)
{
  जाव (from < to)
    {
      जइ (tabsize != 0 && to / tabsize > (from + 1) / tabsize)
        {
          putchar ('\t');
          from += tabsize - from % tabsize;
        }
      अण्णहा
        {
          putchar (' ');
          from++;
        }
    }
}

/* Put DIRNAME/NAME into DEST, handling '.' and '/' properly.  */
/* FIXME: maybe remove this function someday.  See about using a
   non-malloc'ing version of file_name_concat.  */

स्थिर शून्य
attach (वर्ण *dest, वर्ण ध्रुव *dirname, वर्ण ध्रुव *name)
{
  वर्ण ध्रुव *dirnamep = dirname;

  /* Copy dirname if it is not ".".  */
  जइ (dirname[0] != '.' || dirname[1] != 0)
    {
      जाव (*dirnamep)
        *dest++ = *dirnamep++;
      /* Add '/' if 'dirname' doesn't already end with it.  */
      जइ (dirnamep > dirname && dirnamep[-1] != '/')
        *dest++ = '/';
    }
  जाव (*name)
    *dest++ = *name++;
  *dest = 0;
}

/* Allocate enough column info suitable for the current number of
   files and display columns, and initialize the info to represent the
   narrowest possible columns.  */

स्थिर शून्य
init_column_info (idx_t max_cols)
{
  /* Currently allocated columns in column_info.  */
  स्थिर idx_t column_info_alloc;

  जइ (column_info_alloc < max_cols)
    {
      idx_t old_column_info_alloc = column_info_alloc;
      column_info = xpalloc (column_info, &column_info_alloc,
                             max_cols - column_info_alloc, -1,
                             परिमाण *column_info);

      /* Allocate the new size_t objects by computing the triangle
         formula n * (n + 1) / 2, except that we don't need to
         allocate the part of the triangle that we've already
         allocated.  Check for address arithmetic overflow.  */
      idx_t column_info_growth = column_info_alloc - old_column_info_alloc, s;
      जइ (ckd_add (&s, old_column_info_alloc + 1, column_info_alloc)
          || ckd_mul (&s, s, column_info_growth))
        xalloc_die ();
      size_t *p = xinmalloc (s >> 1, परिमाण *p);

      /* Grow the triangle by parceling out the cells just allocated.  */
      प्रति (idx_t i = old_column_info_alloc; i < column_info_alloc; i++)
        {
          column_info[i].col_arr = p;
          p += i + 1;
        }
    }

  प्रति (idx_t i = 0; i < max_cols; ++i)
    {
      column_info[i].valid_len = true;
      column_info[i].पङ्क्तिः_len = i * MIN_COLUMN_WIDTH + 1;
      प्रति (idx_t j = 0; j < i; ++j)
        column_info[i].col_arr[j] = MIN_COLUMN_WIDTH;
      column_info[i].col_arr[i] = 1;
    }
}

/* Calculate the number of columns needed to represent the current set
   of files in the current display width.  */

स्थिर idx_t
calculate_columns (bool by_columns)
{
  /* Normally the maximum number of columns is determined by the
     screen width.  But if few files are available this might limit it
     as well.  */
  idx_t max_cols = 0 < max_idx && max_idx < cwd_n_used ? max_idx : cwd_n_used;

  init_column_info (max_cols);

  /* Compute the maximum number of possible columns.  */
  प्रति (idx_t filesno = 0; filesno < cwd_n_used; ++filesno)
    {
      संरचना fileinfo ध्रुव *f = sorted_file[filesno];
      size_t name_length = length_of_file_name_and_frills (f);

      प्रति (idx_t i = 0; i < max_cols; ++i)
        {
          जइ (column_info[i].valid_len)
            {
              idx_t idx = (by_columns
                           ? filesno / ((cwd_n_used + i) / (i + 1))
                           : filesno % (i + 1));
              size_t real_length = name_length + (idx == i ? 0 : 2);

              जइ (column_info[i].col_arr[idx] < real_length)
                {
                  column_info[i].पङ्क्तिः_len += (real_length
                                              - column_info[i].col_arr[idx]);
                  column_info[i].col_arr[idx] = real_length;
                  column_info[i].valid_len = (column_info[i].पङ्क्तिः_len
                                              <= पङ्क्तिः_length);
                }
            }
        }
    }

  /* Find maximum allowed columns.  */
  idx_t cols;
  प्रति (cols = max_cols; 1 < cols; --cols)
    {
      जइ (column_info[cols - 1].valid_len)
        भङ्ग;
    }

  प्रत्यागम cols;
}

शून्य
usage (पूर्णाङ्क status)
{
  जइ (status != EXIT_SUCCESS)
    emit_try_help ();
  अण्णहा
    {
      printf (_("Usage: %s [OPTION]... [FILE]...\n"), program_name);
      fputs (_("\
List information about the FILEs (the current directory by स्वभाव).\n\
Sort entries alphabetically जइ none of -cftuvSUX nor --sort is specified.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -a, --all\n\
         करेहि not ignore entries starting with .\n\
"));
      oputs (_("\
  -A, --almost-all\n\
         करेहि not list implied . and ..\n\
"));
      oputs (_("\
      --author\n\
         with -l, print the author of each file\n\
"));
      oputs (_("\
  -b, --escape\n\
         print C-style escapes प्रति nongraphic characters\n\
"));
      oputs (_("\
      --block-size=SIZE\n\
         with -l, scale sizes by SIZE when printing them;\n\
         e.g., '--block-size=M'; see SIZE format below\n\
"));
      oputs (_("\
  -B, --ignore-backups\n\
         करेहि not list implied entries ending with ~\n\
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
         generate output designed प्रति Emacs' dired mode\n\
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
         like -F, except करेहि not append '*'\n\
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
         like -l, but करेहि not list owner\n\
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
  -H, --dereference-command-पङ्क्तिः\n\
         follow symbolic links listed on the command पङ्क्तिः\n\
"));
      oputs (_("\
      --dereference-command-पङ्क्तिः-symlink-to-dir\n\
         follow each command पङ्क्तिः symbolic link that points to a directory\n\
"));
      oputs (_("\
      --hide=PATTERN\n\
         करेहि not list implied entries matching shell PATTERN\n\
         (overridden by -a or -A)\n\
"));
      oputs (_("\
      --hyperlink[=WHEN]\n\
         hyperlink file names WHEN\n\
"));
      oputs (_("\
      --indicator-style=WORD\n\
         append indicator with style WORD to entry names:\n\
           none (स्वभाव), slash (-p), file-type (--file-type), classify (-F)\n\
"));
      oputs (_("\
  -i, --inode\n\
         print the index सङ्ख्या of each file\n\
"));
      oputs (_("\
  -I, --ignore=PATTERN\n\
         करेहि not list implied entries matching shell PATTERN\n\
"));
      oputs (_("\
  -k, --kibibytes\n\
         स्वभाव to 1024-byte blocks प्रति file system usage;\n\
         used only with -s and per directory totals\n\
"));
      oputs (_("\
  -l\n\
         use a long listing format\n\
"));
      oputs (_("\
  -L, --dereference\n\
         when showing file information प्रति a symbolic link,\n\
         show information प्रति the file the link references\n\
         rather than प्रति the link itself\n\
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
         like -l, but करेहि not list group information\n\
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
         the स्वभाव, unless program is 'ls' and output is a terminal\n\
"));
      oputs (_("\
  -Q, --quote-name\n\
         enclose entry names in द्विगुण quotes\n\
"));
      oputs (_("\
      --quoting-style=WORD\n\
         use quoting style WORD प्रति entry names:\n\
           literal, locale, shell, shell-always,\n\
           shell-escape, shell-escape-always, c, escape\n\
         (overrides QUOTING_STYLE environment variable)\n\
"));
      oputs (_("\
  -r, --reverse\n\
         reverse order जाव sorting\n\
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
         change स्वभाव 'name' sort to WORD:\n\
           none (-U), size (-S), time (-t),\n\
           version (-v), extension (-X), name, width\n\
"));
      oputs (_("\
      --time=WORD\n\
         select which timestamp used to display or sort;\n\
           access time (-u): atime, access, use;\n\
           metadata change time (-c): ctime, status;\n\
           modified time (स्वभाव): mtime, modification;\n\
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
         करेहि not sort directory entries\n\
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
         list entries by पङ्क्तयः instead of by columns\n\
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
         end each output पङ्क्तिः with NUL, not newline\n\
"));
      oputs (_("\
  -1\n\
         list one file per पङ्क्तिः\n\
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
Also the TIME_STYLE environment variable sets the स्वभाव style to use.\n\
"), stdout);
      fputs (_("\
\n\
The WHEN argument defaults to 'always' and can also be 'auto' or 'never'.\n\
"), stdout);
      fputs (_("\
\n\
Using color to distinguish file types is disabled both by स्वभाव and\n\
with --color=never.  With --color=auto, ls emits color codes only when\n\
standard output is connected to a terminal.  The LS_COLORS environment\n\
variable can change the settings.  Use the dircolors(1) command to set it.\n\
"), stdout);
      fputs (_("\
\n\
Exit status:\n\
 0  जइ OK,\n\
 1  जइ minor problems (e.g., cannot access subdirectory),\n\
 2  जइ serious trouble (e.g., cannot access command-पङ्क्तिः argument).\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}
