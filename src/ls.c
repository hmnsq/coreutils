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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <sys/types.h>

#അന്തര്ഭാവ <termios.h>
#എങ്കിൽ HAVE_STROPTS_H
# include <stropts.h>
#endif
#അന്തര്ഭാവ <sys/ioctl.h>

#ifdef WINSIZE_IN_PTEM
# include <sys/stream.h>
# include <sys/ptem.h>
#endif

#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <setjmp.h>
#അന്തര്ഭാവ <pwd.h>
#അന്തര്ഭാവ <getopt.h>
#അന്തര്ഭാവ <signal.h>

#എങ്കിൽ HAVE_LANGINFO_CODESET
# include <langinfo.h>
#endif

/* NonStop circa 2011 lacks both SA_RESTART and siginterrupt, so don't
   restart syscalls after a signal handler fires.  This may cause
   colors to get messed up on the screen if 'ls' is interrupted, but
   that's the best we can do on such a platform.  */
#ifndef SA_RESTART
# define SA_RESTART 0
#endif

#അന്തര്ഭാവ <fnmatch.h>

/* Gnulib includes.  */
#അന്തര്ഭാവ "acl.h"
#അന്തര്ഭാവ "areadlink.h"
#അന്തര്ഭാവ "argmatch.h"
#അന്തര്ഭാവ "assure.h"
#അന്തര്ഭാവ "c-ctype.h"
#അന്തര്ഭാവ "c-strcase.h"
#അന്തര്ഭാവ "canonicalize.h"
#അന്തര്ഭാവ "dev-ino.h"
#അന്തര്ഭാവ "filemode.h"
#അന്തര്ഭാവ "filenamecat.h"
#അന്തര്ഭാവ "filevercmp.h"
#അന്തര്ഭാവ "hard-locale.h"
#അന്തര്ഭാവ "hash.h"
#അന്തര്ഭാവ "human.h"
#അന്തര്ഭാവ "idcache.h"
#അന്തര്ഭാവ "mbswidth.h"
#അന്തര്ഭാവ "mpsort.h"
#അന്തര്ഭാവ "obstack.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "stat-size.h"
#അന്തര്ഭാവ "stat-time.h"
#അന്തര്ഭാവ "strftime.h"
#അന്തര്ഭാവ "xdectoint.h"
#അന്തര്ഭാവ "xgethostname.h"
#അന്തര്ഭാവ "xmemdup0.h"
#അന്തര്ഭാവ "xstrtol-error.h"
#അന്തര്ഭാവ "xstrtol.h"

#അന്തര്ഭാവ "system.h"

#അന്തര്ഭാവ "dircolors.h"
#അന്തര്ഭാവ "ls.h"
#അന്തര്ഭാവ "statx.h"
#അന്തര്ഭാവ "term-sig.h"

/* Include <sys/capability.h> last to avoid a clash of <sys/types.h>
   include guards with some premature versions of libcap.
   For more details, see <https://bugzilla.redhat.com/483548>.  */
#ifdef HAVE_CAP
# include <sys/capability.h>
#endif

#എങ്കിൽ HAVE_LINUX_XATTR_H
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
#define obstack_chunk_free സ്വതന്ത്രം

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

#എങ്കിൽ READDIR_LIES_ABOUT_MOUNTPOINT_D_INO
# define RELIABLE_D_INO(dp) NOT_AN_INODE_സംഖ്യ
#അല്ലെങ്കിൽ
# define RELIABLE_D_INO(dp) D_INO (dp)
#endif

#എങ്കിൽ ! HAVE_STRUCT_STAT_ST_AUTHOR
# define st_author st_uid
#endif

എണ്ണൽ filetype
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
എണ്ണൽ { filetype_cardinality = arg_directory + 1 };

/* Display letters and indicators for each filetype.
   Keep these in sync with enum filetype.  */
സ്ഥിതം അക്ഷരം സ്ഥിരം filetype_letter[] =
  {'?', 'p', 'c', 'd', 'b', '-', 'l', 's', 'w', 'd'};
static_assert (countof (filetype_letter) == filetype_cardinality);

/* Map enum filetype to <dirent.h> d_type values.  */
സ്ഥിതം unsigned അക്ഷരം സ്ഥിരം filetype_d_type[] =
  {
    DT_UNKNOWN, DT_FIFO, DT_CHR, DT_DIR, DT_BLK, DT_REG, DT_LNK, DT_SOCK,
    DT_WHT, DT_DIR
  };
static_assert (countof (filetype_d_type) == filetype_cardinality);

/* Map d_type values to enum filetype.  */
സ്ഥിതം അക്ഷരം സ്ഥിരം d_type_filetype[UCHAR_MAX + 1] =
  {
    [DT_BLK] = blockdev, [DT_CHR] = chardev, [DT_DIR] = directory,
    [DT_FIFO] = fifo, [DT_LNK] = symbolic_link, [DT_REG] = normal,
    [DT_SOCK] = sock, [DT_WHT] = whiteout
  };

എണ്ണൽ acl_type
  {
    ACL_T_NONE,
    ACL_T_UNKNOWN,
    ACL_T_LSM_CONTEXT_ONLY,
    ACL_T_YES
  };

ഘടന fileinfo
  {
    /* The file name.  */
    അക്ഷരം *name;

    /* For symbolic link, name of the file linked to, otherwise zero.  */
    അക്ഷരം *linkname;

    /* For terminal hyperlinks. */
    അക്ഷരം *absolute_name;

    ഘടന stat stat;

    എണ്ണൽ filetype filetype;

    /* For symbolic link and long listing, st_mode of file linked to, otherwise
       zero.  */
    mode_t linkmode;

    /* security context.  */
    അക്ഷരം *scontext;

    bool stat_ok;

    /* For symbolic link and color printing, true if linked-to file
       exists, otherwise false.  */
    bool linkok;

    /* For long listings, true if the file has an access control list,
       or a security context.  */
    എണ്ണൽ acl_type acl_type;

    /* For color listings, true if a regular file has capability info.  */
    bool has_capability;

    /* Whether file name needs quoting. tri-state with -1 == unknown.  */
    പൂർണ്ണസംഖ്യ quoted;

    /* Cached screen width (including quoting).  */
    size_t width;
  };

/* Null is a valid character in a color indicator (think about Epson
   printers, for example) so we have to use a length/buffer string
   type.  */

ഘടന bin_str
  {
    size_t len;			/* Number of bytes */
    അക്ഷരം സ്ഥിരം *string;		/* Pointer to the same */
  };

#എങ്കിൽ ! HAVE_TCGETPGRP
# define tcgetpgrp(Fd) 0
#endif

സ്ഥിതം size_t quote_name (അക്ഷരം സ്ഥിരം *name,
                          ഘടന quoting_options സ്ഥിരം *options,
                          പൂർണ്ണസംഖ്യ needs_general_quoting,
                          സ്ഥിരം ഘടന bin_str *color,
                          bool allow_pad, ഘടന obstack *അട്ടി,
                          അക്ഷരം സ്ഥിരം *absolute_name);
സ്ഥിതം size_t quote_name_buf (അക്ഷരം **inbuf, size_t bufsize, അക്ഷരം *name,
                              ഘടന quoting_options സ്ഥിരം *options,
                              പൂർണ്ണസംഖ്യ needs_general_quoting, size_t *width,
                              bool *pad);
സ്ഥിതം പൂർണ്ണസംഖ്യ decode_switches (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv);
സ്ഥിതം bool file_ignored (അക്ഷരം സ്ഥിരം *name);
സ്ഥിതം uintmax_t gobble_file (അക്ഷരം സ്ഥിരം *name, എണ്ണൽ filetype type,
                              ino_t inode, bool command_വരി_arg,
                              അക്ഷരം സ്ഥിരം *dirname);
സ്ഥിതം സ്ഥിരം ഘടന bin_str * get_color_indicator (സ്ഥിരം ഘടന fileinfo *f,
                                                   bool symlink_target);
സ്ഥിതം bool print_color_indicator (സ്ഥിരം ഘടന bin_str *ind);
സ്ഥിതം ശൂന്യം put_indicator (സ്ഥിരം ഘടന bin_str *ind);
സ്ഥിതം ശൂന്യം add_ignore_pattern (അക്ഷരം സ്ഥിരം *pattern);
സ്ഥിതം ശൂന്യം attach (അക്ഷരം *dest, അക്ഷരം സ്ഥിരം *dirname, അക്ഷരം സ്ഥിരം *name);
സ്ഥിതം ശൂന്യം clear_files (ശൂന്യം);
സ്ഥിതം ശൂന്യം extract_dirs_from_files (അക്ഷരം സ്ഥിരം *dirname,
                                     bool command_വരി_arg);
സ്ഥിതം bool get_link_name (അക്ഷരം സ്ഥിരം *filename, ഘടന fileinfo *f,
                           bool command_വരി_arg);
സ്ഥിതം ശൂന്യം indent (size_t from, size_t to);
സ്ഥിതം idx_t calculate_columns (bool by_columns);
സ്ഥിതം ശൂന്യം print_current_files (ശൂന്യം);
സ്ഥിതം ശൂന്യം print_dir (അക്ഷരം സ്ഥിരം *name, അക്ഷരം സ്ഥിരം *realname,
                       bool command_വരി_arg);
സ്ഥിതം size_t print_file_name_and_frills (സ്ഥിരം ഘടന fileinfo *f,
                                          size_t start_col);
സ്ഥിതം ശൂന്യം print_horizontal (ശൂന്യം);
സ്ഥിതം പൂർണ്ണസംഖ്യ format_user_width (uid_t u);
സ്ഥിതം പൂർണ്ണസംഖ്യ format_group_width (gid_t g);
സ്ഥിതം ശൂന്യം print_long_format (സ്ഥിരം ഘടന fileinfo *f);
സ്ഥിതം ശൂന്യം print_many_per_വരി (ശൂന്യം);
സ്ഥിതം size_t print_name_with_quoting (സ്ഥിരം ഘടന fileinfo *f,
                                       bool symlink_target,
                                       ഘടന obstack *അട്ടി,
                                       size_t start_col);
സ്ഥിതം ശൂന്യം prep_non_filename_text (ശൂന്യം);
സ്ഥിതം bool print_type_indicator (bool stat_ok, mode_t mode,
                                  എണ്ണൽ filetype type);
സ്ഥിതം ശൂന്യം print_with_separator (അക്ഷരം sep);
സ്ഥിതം ശൂന്യം queue_directory (അക്ഷരം സ്ഥിരം *name, അക്ഷരം സ്ഥിരം *realname,
                             bool command_വരി_arg);
സ്ഥിതം ശൂന്യം sort_files (ശൂന്യം);
സ്ഥിതം ശൂന്യം parse_ls_color (ശൂന്യം);

സ്ഥിതം size_t quote_name_width (അക്ഷരം സ്ഥിരം *name,
                                ഘടന quoting_options സ്ഥിരം *options,
                                പൂർണ്ണസംഖ്യ needs_general_quoting);

/* Initial size of hash table.
   Most hierarchies are likely to be shallower than this.  */
എണ്ണൽ { INITIAL_TABLE_SIZE = 30 };

/* The set of 'active' directories, from the current command-line argument
   to the level in the hierarchy at which files are being listed.
   A directory is represented by its device and inode numbers (struct dev_ino).
   A directory is added to this set when ls begins listing it or its
   entries, and it is removed from the set just after ls has finished
   processing it.  This set is used solely to detect loops, e.g., with
   mkdir loop; cd loop; ln -s ../loop sub; ls -RL  */
സ്ഥിതം Hash_table *active_dir_set;

#define LOOP_DETECT (!!active_dir_set)

/* The table of files in the current directory:

   'cwd_file' points to a vector of 'struct fileinfo', one per file.
   'cwd_n_alloc' is the number of elements space has been allocated for.
   'cwd_n_used' is the number actually in use.  */

/* Address of block containing the files that are described.  */
സ്ഥിതം ഘടന fileinfo *cwd_file;

/* Length of block that 'cwd_file' points to, measured in files.  */
സ്ഥിതം idx_t cwd_n_alloc;

/* Index of first unused slot in 'cwd_file'.  */
സ്ഥിതം idx_t cwd_n_used;

/* Whether files needs may need padding due to quoting.  */
സ്ഥിതം bool cwd_some_quoted;

/* Whether quoting style _may_ add outer quotes,
   and whether aligning those is useful.  */
സ്ഥിതം bool align_variable_outer_quotes;

/* Vector of pointers to files, in proper sorted order, and the number
   of entries allocated for it.  */
സ്ഥിതം ശൂന്യം **sorted_file;
സ്ഥിതം size_t sorted_file_alloc;

/* When true, in a color listing, color each symlink name according to the
   type of file it points to.  Otherwise, color them according to the 'ln'
   directive in LS_COLORS.  Dangling (orphan) symlinks are treated specially,
   regardless.  This is set when 'ln=target' appears in LS_COLORS.  */

സ്ഥിതം bool color_symlink_as_referent;

സ്ഥിതം അക്ഷരം സ്ഥിരം *hostname;

/* Mode of appropriate file for coloring.  */
സ്ഥിതം mode_t
file_or_link_mode (ഘടന fileinfo സ്ഥിരം *file)
{
  തിരികെ (color_symlink_as_referent && file->linkok
          ? file->linkmode : file->stat.st_mode);
}


/* Record of one pending directory waiting to be listed.  */

ഘടന pending
  {
    അക്ഷരം *name;
    /* If the directory is actually the file pointed to by a symbolic link we
       were told to list, 'realname' will contain the name of the symbolic
       link, otherwise zero.  */
    അക്ഷരം *realname;
    bool command_വരി_arg;
    ഘടന pending *next;
  };

സ്ഥിതം ഘടന pending *pending_dirs;

/* Current time in seconds and nanoseconds since 1970, updated as
   needed when deciding whether a file is recent.  */

സ്ഥിതം ഘടന timespec current_time;

സ്ഥിതം bool print_scontext;
സ്ഥിതം അക്ഷരം UNKNOWN_SECURITY_CONTEXT[] = "?";

/* Whether any of the files has an ACL.  This affects the width of the
   mode column.  */

സ്ഥിതം bool any_has_acl;

/* The number of columns to use for columns containing inode numbers,
   block sizes, link counts, owners, groups, authors, major device
   numbers, minor device numbers, and file sizes, respectively.  */

സ്ഥിതം പൂർണ്ണസംഖ്യ inode_സംഖ്യ_width;
സ്ഥിതം പൂർണ്ണസംഖ്യ block_size_width;
സ്ഥിതം പൂർണ്ണസംഖ്യ nlink_width;
സ്ഥിതം പൂർണ്ണസംഖ്യ scontext_width;
സ്ഥിതം പൂർണ്ണസംഖ്യ owner_width;
സ്ഥിതം പൂർണ്ണസംഖ്യ group_width;
സ്ഥിതം പൂർണ്ണസംഖ്യ author_width;
സ്ഥിതം പൂർണ്ണസംഖ്യ major_device_സംഖ്യ_width;
സ്ഥിതം പൂർണ്ണസംഖ്യ minor_device_സംഖ്യ_width;
സ്ഥിതം പൂർണ്ണസംഖ്യ file_size_width;

/* Option flags */

/* long_format for lots of info, one per line.
   one_per_line for just names, one per line.
   many_per_line for just names, many per line, sorted vertically.
   horizontal for just names, many per line, sorted horizontally.
   with_commas for just names, many per line, separated by commas.

   -l (and other options that imply -l), -1, -C, -x and -m control
   this parameter.  */

എണ്ണൽ format
  {
    long_format,		/* -l and other options that imply -l */
    one_per_വരി,		/* -1 */
    many_per_വരി,		/* -C */
    horizontal,			/* -x */
    with_commas			/* -m */
  };

സ്ഥിതം എണ്ണൽ format format;

/* 'full-iso' uses full ISO-style dates and times.  'long-iso' uses longer
   ISO-style timestamps, though shorter than 'full-iso'.  'iso' uses shorter
   ISO-style timestamps.  'locale' uses locale-dependent timestamps.  */
എണ്ണൽ time_style
  {
    full_iso_time_style,	/* --time-style=full-iso */
    long_iso_time_style,	/* --time-style=long-iso */
    iso_time_style,		/* --time-style=iso */
    locale_time_style		/* --time-style=locale */
  };

സ്ഥിതം അക്ഷരം സ്ഥിരം *സ്ഥിരം time_style_args[] =
{
  "full-iso", "long-iso", "iso", "locale", ശൂന്യ
};
സ്ഥിതം എണ്ണൽ time_style സ്ഥിരം time_style_types[] =
{
  full_iso_time_style, long_iso_time_style, iso_time_style,
  locale_time_style
};
ARGMATCH_VERIFY (time_style_args, time_style_types);

/* Type of time to print or sort by.  Controlled by -c and -u.
   The values of each item of this enum are important since they are
   used as indices in the sort functions array (see sort_files()).  */

എണ്ണൽ time_type
  {
    time_mtime = 0,		/* default */
    time_ctime,			/* -c */
    time_atime,			/* -u */
    time_btime,                 /* birth time */
    time_numtypes		/* the number of elements of this enum */
  };

സ്ഥിതം എണ്ണൽ time_type time_type;
സ്ഥിതം bool explicit_time;

/* The file characteristic to sort by.  Controlled by -t, -S, -U, -X, -v.
   The values of each item of this enum are important since they are
   used as indices in the sort functions array (see sort_files()).  */

എണ്ണൽ sort_type
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

സ്ഥിതം എണ്ണൽ sort_type sort_type;

/* Direction of sort.
   false means highest first if numeric,
   lowest first if alphabetic;
   these are the defaults.
   true means the opposite order in each case.  -r  */

സ്ഥിതം bool sort_reverse;

/* True means to display owner information.  -g turns this off.  */

സ്ഥിതം bool print_owner = true;

/* True means to display author information.  */

സ്ഥിതം bool print_author;

/* True means to display group information.  -G and -o turn this off.  */

സ്ഥിതം bool print_group = true;

/* True means print the user and group id's as numbers rather
   than as names.  -n  */

സ്ഥിതം bool numeric_ids;

/* True means mention the size in blocks of each file.  -s  */

സ്ഥിതം bool print_block_size;

/* Human-readable options for output, when printing block counts.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ human_output_opts;

/* The units to use when printing block counts.  */
സ്ഥിതം uintmax_t output_block_size;

/* Likewise, but for file sizes.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ file_human_output_opts;
സ്ഥിതം uintmax_t file_output_block_size = 1;

/* Follow the output with a special string.  Using this format,
   Emacs' dired mode starts up twice as fast, and can handle all
   strange characters in file names.  */
സ്ഥിതം bool dired;

/* 'none' means don't mention the type of files.
   'slash' means mention directories only, with a '/'.
   'file_type' means mention file types.
   'classify' means mention file types and mark executables.

   Controlled by -F, -p, and --indicator-style.  */

എണ്ണൽ indicator_style
  {
    none = 0,	/*     --indicator-style=none (default) */
    slash,	/* -p, --indicator-style=slash */
    file_type,	/*     --indicator-style=file-type */
    classify	/* -F, --indicator-style=classify */
  };

സ്ഥിതം എണ്ണൽ indicator_style indicator_style;

/* Names of indicator styles.  */
സ്ഥിതം അക്ഷരം സ്ഥിരം *സ്ഥിരം indicator_style_args[] =
{
  "none", "slash", "file-type", "classify", ശൂന്യ
};
സ്ഥിതം എണ്ണൽ indicator_style സ്ഥിരം indicator_style_types[] =
{
  none, slash, file_type, classify
};
ARGMATCH_VERIFY (indicator_style_args, indicator_style_types);

/* True means use colors to mark types.  Also define the different
   colors as well as the stuff for the LS_COLORS environment variable.
   The LS_COLORS variable is now in a termcap-like format.  */

സ്ഥിതം bool print_with_color;

സ്ഥിതം bool print_hyperlink;

/* Whether we used any colors in the output so far.  If so, we will
   need to restore the default color later.  If not, we will need to
   call prep_non_filename_text before using color for the first time. */

സ്ഥിതം bool used_color = false;

എണ്ണൽ when_type
  {
    when_never,		/* 0: default or --color=never */
    when_always,	/* 1: --color=always */
    when_if_tty		/* 2: --color=tty */
  };

എണ്ണൽ Dereference_symlink
  {
    DEREF_UNDEFINED = 0,		/* default */
    DEREF_NEVER,
    DEREF_COMMAND_വരി_ARGUMENTS,	/* -H */
    DEREF_COMMAND_വരി_SYMLINK_TO_DIR,	/* the default, in certain cases */
    DEREF_ALWAYS			/* -L */
  };

എണ്ണൽ indicator_no
  {
    C_ഇടത്, C_വലത്, C_END, C_RESET, C_NORM, C_FILE, C_DIR, C_LINK,
    C_FIFO, C_SOCK,
    C_BLK, C_CHR, C_MISSING, C_ORPHAN, C_EXEC, C_DOOR, C_SETUID, C_SETGID,
    C_STICKY, C_OTHER_WRITABLE, C_STICKY_OTHER_WRITABLE, C_CAP, C_MULTIHARDLINK,
    C_CLR_TO_EOL
  };

സ്ഥിതം അക്ഷരം സ്ഥിരം indicator_name[][2]=
  {
    {'l','c'}, {'r','c'}, {'e','c'}, {'r','s'}, {'n','o'},
    {'f','i'}, {'d','i'}, {'l','n'},
    {'p','i'}, {'s','o'},
    {'b','d'}, {'c','d'}, {'m','i'}, {'o','r'}, {'e','x'},
    {'d','o'}, {'s','u'}, {'s','g'},
    {'s','t'}, {'o','w'}, {'t','w'}, {'c','a'}, {'m','h'},
    {'c','l'}
  };

ഘടന color_ext_type
  {
    ഘടന bin_str ext;		/* The extension we're looking for */
    ഘടന bin_str seq;		/* The sequence to output when we do */
    bool   exact_match;		/* Whether to compare case insensitively */
    ഘടന color_ext_type *next;	/* Next in list */
  };

സ്ഥിതം ഘടന bin_str color_indicator[] =
  {
    { 2, (അക്ഷരം സ്ഥിരം []) {'\033','['} },/* lc: Left of color sequence */
    { 1, (അക്ഷരം സ്ഥിരം []) {'m'} },	/* rc: Right of color sequence */
    { 0, ശൂന്യ },			/* ec: End color (replaces lc+rs+rc) */
    { 1, (അക്ഷരം സ്ഥിരം []) {'0'} },	/* rs: Reset to ordinary colors */
    { 0, ശൂന്യ },			/* no: Normal */
    { 0, ശൂന്യ },			/* fi: File: default */
    { 5, ((അക്ഷരം സ്ഥിരം [])
          {'0','1',';','3','4'}) },	/* di: Directory: bright blue */
    { 5, ((അക്ഷരം സ്ഥിരം [])
          {'0','1',';','3','6'}) },	/* ln: Symlink: bright cyan */
    { 2, (അക്ഷരം സ്ഥിരം []) {'3','3'} },		/* pi: Pipe: yellow/brown */
    { 5, ((അക്ഷരം സ്ഥിരം [])
          {'0','1',';','3','5'}) },	/* so: Socket: bright magenta */
    { 5, ((അക്ഷരം സ്ഥിരം [])
          {'0','1',';','3','3'}) },	/* bd: Block device: bright yellow */
    { 5, ((അക്ഷരം സ്ഥിരം [])
          {'0','1',';','3','3'}) },	/* cd: Char device: bright yellow */
    { 0, ശൂന്യ },			/* mi: Missing file: undefined */
    { 0, ശൂന്യ },			/* or: Orphaned symlink: undefined */
    { 5, ((അക്ഷരം സ്ഥിരം [])
          {'0','1',';','3','2'}) },	/* ex: Executable: bright green */
    { 5, ((അക്ഷരം സ്ഥിരം [])
          {'0','1',';','3','5'}) },	/* do: Door: bright magenta */
    { 5, ((അക്ഷരം സ്ഥിരം [])
          {'3','7',';','4','1'}) },	/* su: setuid: white on red */
    { 5, ((അക്ഷരം സ്ഥിരം [])
          {'3','0',';','4','3'}) },	/* sg: setgid: black on yellow */
    { 5, ((അക്ഷരം സ്ഥിരം [])
          {'3','7',';','4','4'}) },	/* st: sticky: black on blue */
    { 5, ((അക്ഷരം സ്ഥിരം [])
          {'3','4',';','4','2'}) },	/* ow: other-writable: blue on green */
    { 5, ((അക്ഷരം സ്ഥിരം [])
          {'3','0',';','4','2'}) },	/* tw: ow w/ sticky: black on green */
    { 0, ശൂന്യ },			/* ca: disabled by default */
    { 0, ശൂന്യ },			/* mh: disabled by default */
    { 3, ((അക്ഷരം സ്ഥിരം [])
          {'\033','[','K'}) },		/* cl: clear to end of line */
  };

/* A list mapping file extensions to corresponding display sequence.  */
സ്ഥിതം ഘടന color_ext_type *color_ext_list = ശൂന്യ;

/* Buffer for color sequences */
സ്ഥിതം അക്ഷരം *color_buf;

/* True means to check for orphaned symbolic link, for displaying
   colors, or to group symlink to directories with other dirs.  */

സ്ഥിതം bool check_symlink_mode;

/* True means mention the inode number of each file.  -i  */

സ്ഥിതം bool print_inode;

/* What to do with symbolic links.  Affected by -d, -F, -H, -l (and
   other options that imply -l), and -L.  */

സ്ഥിതം എണ്ണൽ Dereference_symlink dereference;

/* True means when a directory is found, display info on its
   contents.  -R  */

സ്ഥിതം bool recursive;

/* True means when an argument is a directory name, display info
   on it itself.  -d  */

സ്ഥിതം bool immediate_dirs;

/* True means that directories are grouped before files. */

സ്ഥിതം bool directories_first;

/* Which files to ignore.  */

സ്ഥിതം എണ്ണൽ
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

ഘടന ignore_pattern
  {
    അക്ഷരം സ്ഥിരം *pattern;
    ഘടന ignore_pattern *next;
  };

സ്ഥിതം ഘടന ignore_pattern *ignore_patterns;

/* Similar to IGNORE_PATTERNS, except that -a or -A causes this
   variable itself to be ignored.  */
സ്ഥിതം ഘടന ignore_pattern *hide_patterns;

/* True means output nongraphic chars in file names as '?'.
   (-q, --hide-control-chars)
   qmark_funny_chars and the quoting style (-Q, --quoting-style=WORD) are
   independent.  The algorithm is: first, obey the quoting style to get a
   string representing the file name;  then, if qmark_funny_chars is set,
   replace all nonprintable chars in that string with '?'.  It's necessary
   to replace nonprintable chars even in quoted strings, because we don't
   want to mess up the terminal if control chars get sent to it, and some
   quoting methods pass through control chars as-is.  */
സ്ഥിതം bool qmark_funny_chars;

/* True means extra replacement of newline with '?'
   in cases where a newline in a file name would be
   ambiguous with newline separators.  */
സ്ഥിതം bool qmark_newline;

/* Quoting options for file and dir name output.  */

സ്ഥിതം ഘടന quoting_options *filename_quoting_options;
സ്ഥിതം ഘടന quoting_options *dirname_quoting_options;

/* The number of chars per hardware tab stop.  Setting this to zero
   inhibits the use of TAB characters for separating columns.  -T */
സ്ഥിതം size_t tabsize;

/* True means print each directory name before listing it.  */

സ്ഥിതം bool print_dir_name;

/* The line length to use for breaking lines in many-per-line format.
   Can be set with -w.  If zero, there is no limit.  */

സ്ഥിതം size_t വരി_length;

/* The local time zone rules, as per the TZ environment variable.  */

സ്ഥിതം timezone_t localtz;

/* If true, the file listing format requires that stat be called on
   each file.  */

സ്ഥിതം bool format_needs_stat;

/* Similar to 'format_needs_stat', but set if only the file type is
   needed.  */

സ്ഥിതം bool format_needs_type;

/* Like 'format_needs_stat', but set only if capability colors are needed.  */

സ്ഥിതം bool format_needs_capability;

/* An arbitrary limit on the number of bytes in a printed timestamp.
   This is set to a relatively small value to avoid the need to worry
   about denial-of-service attacks on servers that run "ls" on behalf
   of remote clients.  1000 bytes should be enough for any practical
   timestamp format.  */

എണ്ണൽ { TIME_STAMP_LEN_MAXIMUM = MAX (1000, INT_STRLEN_BOUND (time_t)) };

/* strftime formats for non-recent and recent files, respectively, in
   -l output.  */

സ്ഥിതം അക്ഷരം സ്ഥിരം *long_time_format[2] =
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

സ്ഥിതം sigset_t caught_signals;

/* If nonzero, the value of the pending fatal signal.  */

സ്ഥിതം sig_atomic_t അസ്ഥിര interrupt_signal;

/* A count of the number of pending stop signals that have been received.  */

സ്ഥിതം sig_atomic_t അസ്ഥിര stop_signal_എണ്ണം;

/* Desired exit status.  */

സ്ഥിതം പൂർണ്ണസംഖ്യ exit_status;

/* Exit statuses.  */
എണ്ണൽ
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
എണ്ണൽ
{
  AUTHOR_OPTION = CHAR_MAX + 1,
  BLOCK_SIZE_OPTION,
  COLOR_OPTION,
  DEREFERENCE_COMMAND_വരി_SYMLINK_TO_DIR_OPTION,
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

സ്ഥിതം ഘടന option സ്ഥിരം long_options[] =
{
  {"all", no_argument, ശൂന്യ, 'a'},
  {"escape", no_argument, ശൂന്യ, 'b'},
  {"directory", no_argument, ശൂന്യ, 'd'},
  {"dired", no_argument, ശൂന്യ, 'D'},
  {"full-time", no_argument, ശൂന്യ, FULL_TIME_OPTION},
  {"group-directories-first", no_argument, ശൂന്യ,
   GROUP_DIRECTORIES_FIRST_OPTION},
  {"human-readable", no_argument, ശൂന്യ, 'h'},
  {"inode", no_argument, ശൂന്യ, 'i'},
  {"kibibytes", no_argument, ശൂന്യ, 'k'},
  {"numeric-uid-gid", no_argument, ശൂന്യ, 'n'},
  {"no-group", no_argument, ശൂന്യ, 'G'},
  {"hide-control-chars", no_argument, ശൂന്യ, 'q'},
  {"reverse", no_argument, ശൂന്യ, 'r'},
  {"size", no_argument, ശൂന്യ, 's'},
  {"width", required_argument, ശൂന്യ, 'w'},
  {"almost-all", no_argument, ശൂന്യ, 'A'},
  {"ignore-backups", no_argument, ശൂന്യ, 'B'},
  {"classify", optional_argument, ശൂന്യ, 'F'},
  {"file-type", no_argument, ശൂന്യ, FILE_TYPE_INDICATOR_OPTION},
  {"si", no_argument, ശൂന്യ, SI_OPTION},
  {"dereference-command-line", no_argument, ശൂന്യ, 'H'},
  {"dereference-command-line-symlink-to-dir", no_argument, ശൂന്യ,
   DEREFERENCE_COMMAND_വരി_SYMLINK_TO_DIR_OPTION},
  {"hide", required_argument, ശൂന്യ, HIDE_OPTION},
  {"ignore", required_argument, ശൂന്യ, 'I'},
  {"indicator-style", required_argument, ശൂന്യ, INDICATOR_STYLE_OPTION},
  {"dereference", no_argument, ശൂന്യ, 'L'},
  {"literal", no_argument, ശൂന്യ, 'N'},
  {"quote-name", no_argument, ശൂന്യ, 'Q'},
  {"quoting-style", required_argument, ശൂന്യ, QUOTING_STYLE_OPTION},
  {"recursive", no_argument, ശൂന്യ, 'R'},
  {"format", required_argument, ശൂന്യ, FORMAT_OPTION},
  {"show-control-chars", no_argument, ശൂന്യ, SHOW_CONTROL_CHARS_OPTION},
  {"sort", required_argument, ശൂന്യ, SORT_OPTION},
  {"tabsize", required_argument, ശൂന്യ, 'T'},
  {"time", required_argument, ശൂന്യ, TIME_OPTION},
  {"time-style", required_argument, ശൂന്യ, TIME_STYLE_OPTION},
  {"zero", no_argument, ശൂന്യ, ZERO_OPTION},
  {"color", optional_argument, ശൂന്യ, COLOR_OPTION},
  {"hyperlink", optional_argument, ശൂന്യ, HYPERLINK_OPTION},
  {"block-size", required_argument, ശൂന്യ, BLOCK_SIZE_OPTION},
  {"context", no_argument, ശൂന്യ, 'Z'},
  {"author", no_argument, ശൂന്യ, AUTHOR_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ശൂന്യ, 0, ശൂന്യ, 0}
};

സ്ഥിതം അക്ഷരം സ്ഥിരം *സ്ഥിരം format_args[] =
{
  "verbose", "long", "commas", "horizontal", "across",
  "vertical", "single-column", ശൂന്യ
};
സ്ഥിതം എണ്ണൽ format സ്ഥിരം format_types[] =
{
  long_format, long_format, with_commas, horizontal, horizontal,
  many_per_വരി, one_per_വരി
};
ARGMATCH_VERIFY (format_args, format_types);

സ്ഥിതം അക്ഷരം സ്ഥിരം *സ്ഥിരം sort_args[] =
{
  "none", "size", "time", "version", "extension",
  "name", "width", ശൂന്യ
};
സ്ഥിതം എണ്ണൽ sort_type സ്ഥിരം sort_types[] =
{
  sort_none, sort_size, sort_time, sort_version, sort_extension,
  sort_name, sort_width
};
ARGMATCH_VERIFY (sort_args, sort_types);

സ്ഥിതം അക്ഷരം സ്ഥിരം *സ്ഥിരം time_args[] =
{
  "atime", "access", "use",
  "ctime", "status",
  "mtime", "modification",
  "birth", "creation",
  ശൂന്യ
};
സ്ഥിതം എണ്ണൽ time_type സ്ഥിരം time_types[] =
{
  time_atime, time_atime, time_atime,
  time_ctime, time_ctime,
  time_mtime, time_mtime,
  time_btime, time_btime,
};
ARGMATCH_VERIFY (time_args, time_types);

സ്ഥിതം അക്ഷരം സ്ഥിരം *സ്ഥിരം when_args[] =
{
  /* force and none are for compatibility with another color-ls version */
  "always", "yes", "force",
  "never", "no", "none",
  "auto", "tty", "if-tty", ശൂന്യ
};
സ്ഥിതം എണ്ണൽ when_type സ്ഥിരം when_types[] =
{
  when_always, when_always, when_always,
  when_never, when_never, when_never,
  when_if_tty, when_if_tty, when_if_tty
};
ARGMATCH_VERIFY (when_args, when_types);

/* Information about filling a column.  */
ഘടന column_info
{
  bool valid_len;
  size_t വരി_len;
  size_t *col_arr;
};

/* Array with information about column fullness.  */
സ്ഥിതം ഘടന column_info *column_info;

/* Maximum number of columns ever possible for this display.  */
സ്ഥിതം size_t max_idx;

/* The minimum width of a non-final column is 3: 1 character for the name
   and 2 for the separating white space.  The final column is not padded.  */
എണ്ണൽ { MIN_COLUMN_WIDTH = 3 };


/* This zero-based index is for the --dired option.  It is incremented
   for each byte of output generated by this program so that the beginning
   and ending indices (in that output) of every file name can be recorded
   and later output themselves.  */
സ്ഥിതം off_t dired_pos;

സ്ഥിതം ശൂന്യം
dired_outbyte (അക്ഷരം c)
{
  dired_pos++;
  putchar (c);
}

/* Output the buffer S, of length S_LEN, and increment DIRED_POS by S_LEN.  */
സ്ഥിതം ശൂന്യം
dired_outbuf (അക്ഷരം സ്ഥിരം *s, size_t s_len)
{
  dired_pos += s_len;
  fwrite (s, വലുപ്പം *s, s_len, stdout);
}

/* Output the string S, and increment DIRED_POS by its length.  */
സ്ഥിതം ശൂന്യം
dired_outstring (അക്ഷരം സ്ഥിരം *s)
{
  dired_outbuf (s, strlen (s));
}

സ്ഥിതം ശൂന്യം
dired_indent (ശൂന്യം)
{
  എങ്കിൽ (dired)
    dired_outstring ("  ");
}

/* With --dired, store pairs of beginning and ending indices of file names.  */
സ്ഥിതം ഘടന obstack dired_obstack;

/* With --dired, store pairs of beginning and ending indices of any
   directory names that appear as headers (just before 'total' line)
   for lists of directory entries.  Such directory names are seen when
   listing hierarchies using -R and when a directory is listed with at
   least one other command line argument.  */
സ്ഥിതം ഘടന obstack subdired_obstack;

/* Save the current index on the specified obstack, OBS.  */
സ്ഥിതം ശൂന്യം
തള്ളുക_current_dired_pos (ഘടന obstack *obs)
{
  എങ്കിൽ (dired)
    obstack_grow (obs, &dired_pos, വലുപ്പം dired_pos);
}

/* With -R, this stack is used to help detect directory cycles.
   The device/inode pairs on this stack mirror the pairs in the
   active_dir_set hash table.  */
സ്ഥിതം ഘടന obstack dev_ino_obstack;

/* Push a pair onto the device/inode stack.  */
സ്ഥിതം ശൂന്യം
dev_ino_തള്ളുക (dev_t dev, ino_t ino)
{
  ശൂന്യം *vdi;
  ഘടന dev_ino *di;
  പൂർണ്ണസംഖ്യ dev_ino_size = വലുപ്പം *di;
  obstack_blank (&dev_ino_obstack, dev_ino_size);
  vdi = obstack_next_free (&dev_ino_obstack);
  di = vdi;
  di--;
  di->st_dev = dev;
  di->st_ino = ino;
}

/* Pop a dev/ino struct off the global dev_ino_obstack
   and return that struct.  */
സ്ഥിതം ഘടന dev_ino
dev_ino_എടുക്കുക (ശൂന്യം)
{
  ശൂന്യം *vdi;
  ഘടന dev_ino *di;
  പൂർണ്ണസംഖ്യ dev_ino_size = വലുപ്പം *di;
  affirm (dev_ino_size <= obstack_object_size (&dev_ino_obstack));
  obstack_blank_fast (&dev_ino_obstack, -dev_ino_size);
  vdi = obstack_next_free (&dev_ino_obstack);
  di = vdi;
  തിരികെ *di;
}

സ്ഥിതം ശൂന്യം
assert_matching_dev_ino (അക്ഷരം സ്ഥിരം *name, ഘടന dev_ino di)
{
  MAYBE_UNUSED ഘടന stat sb;
  assure (0 <= stat (name, &sb));
  assure (sb.st_dev == di.st_dev);
  assure (sb.st_ino == di.st_ino);
}

സ്ഥിതം അക്ഷരം eolbyte = '\n';

/* Write to standard output PREFIX, followed by the quoting style and
   a space-separated list of the integers stored in OS all on one line.  */

സ്ഥിതം ശൂന്യം
dired_dump_obstack (അക്ഷരം സ്ഥിരം *prefix, ഘടന obstack *os)
{
  size_t n_pos;

  n_pos = obstack_object_size (os) / വലുപ്പം (dired_pos);
  എങ്കിൽ (n_pos > 0)
    {
      off_t *pos = obstack_finish (os);
      fputs (prefix, stdout);
      വേണ്ടി (size_t i = 0; i < n_pos; i++)
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
സ്ഥിതം ഘടന timespec
get_stat_btime (ഘടന stat സ്ഥിരം *st)
{
  ഘടന timespec btimespec;

#എങ്കിൽ HAVE_STATX && defined STATX_INO
  btimespec = get_stat_mtime (st);
#അല്ലെങ്കിൽ
  btimespec = get_stat_birthtime (st);
#endif

  തിരികെ btimespec;
}

#എങ്കിൽ HAVE_STATX && defined STATX_INO
ATTRIBUTE_PURE
സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ
time_type_to_statx (ശൂന്യം)
{
  തിരഞ്ഞെടുപ്പ് (time_type)
    {
    അവസ്ഥ time_ctime:
      തിരികെ STATX_CTIME;
    അവസ്ഥ time_mtime:
      തിരികെ STATX_MTIME;
    അവസ്ഥ time_atime:
      തിരികെ STATX_ATIME;
    അവസ്ഥ time_btime:
      തിരികെ STATX_BTIME;
    അവസ്ഥ time_numtypes: സ്ഥിരസ്ഥിതി:
      unreachable ();
    }
    തിരികെ 0;
}

ATTRIBUTE_PURE
സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ
calc_req_mask (ശൂന്യം)
{
  unsigned പൂർണ്ണസംഖ്യ mask = STATX_MODE;

  എങ്കിൽ (print_inode)
    mask |= STATX_INO;

  എങ്കിൽ (print_block_size)
    mask |= STATX_BLOCKS;

  എങ്കിൽ (format == long_format) {
    mask |= STATX_NLINK | STATX_SIZE | time_type_to_statx ();
    എങ്കിൽ (print_owner || print_author)
      mask |= STATX_UID;
    എങ്കിൽ (print_group)
      mask |= STATX_GID;
  }

  തിരഞ്ഞെടുപ്പ് (sort_type)
    {
    അവസ്ഥ sort_none:
    അവസ്ഥ sort_name:
    അവസ്ഥ sort_version:
    അവസ്ഥ sort_extension:
    അവസ്ഥ sort_width:
      നിർത്തുക;
    അവസ്ഥ sort_time:
      mask |= time_type_to_statx ();
      നിർത്തുക;
    അവസ്ഥ sort_size:
      mask |= STATX_SIZE;
      നിർത്തുക;
    അവസ്ഥ sort_numtypes: സ്ഥിരസ്ഥിതി:
      unreachable ();
    }

  തിരികെ mask;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
do_statx (പൂർണ്ണസംഖ്യ fd, അക്ഷരം സ്ഥിരം *name, ഘടന stat *st, പൂർണ്ണസംഖ്യ flags,
          unsigned പൂർണ്ണസംഖ്യ mask)
{
  ഘടന statx stx;
  bool want_btime = mask & STATX_BTIME;
  പൂർണ്ണസംഖ്യ ret = statx (fd, name, flags | AT_NO_AUTOMOUNT, mask, &stx);
  എങ്കിൽ (ret >= 0)
    {
      statx_to_stat (&stx, st);
      /* Since we only need one timestamp type,
         store birth time in st_mtim.  */
      എങ്കിൽ (want_btime)
        {
          എങ്കിൽ (stx.stx_mask & STATX_BTIME)
            st->st_mtim = statx_timestamp_to_timespec (stx.stx_btime);
          അല്ലെങ്കിൽ
            st->st_mtim.tv_sec = st->st_mtim.tv_nsec = -1;
        }
    }

  തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
do_stat (അക്ഷരം സ്ഥിരം *name, ഘടന stat *st)
{
  തിരികെ do_statx (AT_FDCWD, name, st, 0, calc_req_mask ());
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
do_lstat (അക്ഷരം സ്ഥിരം *name, ഘടന stat *st)
{
  തിരികെ do_statx (AT_FDCWD, name, st, AT_SYMLINK_NOFOLLOW, calc_req_mask ());
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
stat_for_mode (അക്ഷരം സ്ഥിരം *name, ഘടന stat *st)
{
  തിരികെ do_statx (AT_FDCWD, name, st, 0, STATX_MODE);
}

/* dev+ino should be static, so no need to sync with backing store */
സ്ഥിതം പൂർണ്ണസംഖ്യ
stat_for_ino (അക്ഷരം സ്ഥിരം *name, ഘടന stat *st)
{
  തിരികെ do_statx (AT_FDCWD, name, st, 0, STATX_INO);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
fstat_for_ino (പൂർണ്ണസംഖ്യ fd, ഘടന stat *st)
{
  തിരികെ do_statx (fd, "", st, AT_EMPTY_PATH, STATX_INO);
}
#അല്ലെങ്കിൽ
സ്ഥിതം പൂർണ്ണസംഖ്യ
do_stat (അക്ഷരം സ്ഥിരം *name, ഘടന stat *st)
{
  തിരികെ stat (name, st);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
do_lstat (അക്ഷരം സ്ഥിരം *name, ഘടന stat *st)
{
  തിരികെ lstat (name, st);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
stat_for_mode (അക്ഷരം സ്ഥിരം *name, ഘടന stat *st)
{
  തിരികെ stat (name, st);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
stat_for_ino (അക്ഷരം സ്ഥിരം *name, ഘടന stat *st)
{
  തിരികെ stat (name, st);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
fstat_for_ino (പൂർണ്ണസംഖ്യ fd, ഘടന stat *st)
{
  തിരികെ fstat (fd, st);
}
#endif

/* Return the address of the first plain %b spec in FMT, or NULL if
   there is no such spec.  %5b etc. do not match, so that user
   widths/flags are honored.  */

ATTRIBUTE_PURE
സ്ഥിതം അക്ഷരം സ്ഥിരം *
first_percent_b (അക്ഷരം സ്ഥിരം *fmt)
{
  വേണ്ടി (; *fmt; fmt++)
    എങ്കിൽ (fmt[0] == '%')
      തിരഞ്ഞെടുപ്പ് (fmt[1])
        {
        അവസ്ഥ 'b': തിരികെ fmt;
        അവസ്ഥ '%': fmt++; നിർത്തുക;
        }
  തിരികെ ശൂന്യ;
}

സ്ഥിതം അക്ഷരം RFC3986[256];
സ്ഥിതം ശൂന്യം
file_escape_init (ശൂന്യം)
{
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < 256; i++)
    RFC3986[i] |= c_isalnum (i) || i == '~' || i == '-' || i == '.' || i == '_';
}

എണ്ണൽ { MBSWIDTH_FLAGS = MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE };

/* Read the abbreviated month names from the locale, to align them
   and to determine the max width of the field and to truncate names
   greater than our max allowed.
   Note even though this handles multibyte locales correctly
   it's not restricted to them as single byte locales can have
   variable width abbreviated months and also precomputing/caching
   the names was seen to increase the performance of ls significantly.  */

/* abformat[RECENT][MON] is the format to use for timestamps with
   recentness RECENT and month MON.  */
എണ്ണൽ { ABFORMAT_SIZE = 128 };
സ്ഥിതം അക്ഷരം abformat[2][12][ABFORMAT_SIZE];
/* True if precomputed formats should be used.  This can be false if
   nl_langinfo fails, if a format or month abbreviation is unusually
   long, or if a month abbreviation contains '%'.  */
സ്ഥിതം bool use_abformat;

/* Store into ABMON the abbreviated month names, suitably aligned.
   Return true if successful.  */

സ്ഥിതം bool
abmon_init (അക്ഷരം abmon[12][ABFORMAT_SIZE])
{
#ifndef HAVE_NL_LANGINFO
  തിരികെ false;
#അല്ലെങ്കിൽ
  പൂർണ്ണസംഖ്യ max_mon_width = 0;
  പൂർണ്ണസംഖ്യ mon_width[12];
  പൂർണ്ണസംഖ്യ mon_len[12];

  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < 12; i++)
    {
      അക്ഷരം സ്ഥിരം *abbr = nl_langinfo (ABMON_1 + i);
      mon_len[i] = strnlen (abbr, ABFORMAT_SIZE);
      എങ്കിൽ (mon_len[i] == ABFORMAT_SIZE)
        തിരികെ false;
      എങ്കിൽ (strchr (abbr, '%'))
        തിരികെ false;
      mon_width[i] = mbswidth (strcpy (abmon[i], abbr), MBSWIDTH_FLAGS);
      എങ്കിൽ (mon_width[i] < 0)
        തിരികെ false;
      max_mon_width = MAX (max_mon_width, mon_width[i]);
    }

  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < 12; i++)
    {
      പൂർണ്ണസംഖ്യ fill = max_mon_width - mon_width[i];
      എങ്കിൽ (ABFORMAT_SIZE - mon_len[i] <= fill)
        തിരികെ false;
      bool align_ഇടത് = !c_isdigit (abmon[i][0]);
      പൂർണ്ണസംഖ്യ fill_offset;
      എങ്കിൽ (align_ഇടത്)
        fill_offset = mon_len[i];
      അല്ലെങ്കിൽ
        {
          memmove (abmon[i] + fill, abmon[i], mon_len[i]);
          fill_offset = 0;
        }
      memset (abmon[i] + fill_offset, ' ', fill);
      abmon[i][mon_len[i] + fill] = '\0';
    }

  തിരികെ true;
#endif
}

/* Initialize ABFORMAT and USE_ABFORMAT.  */

സ്ഥിതം ശൂന്യം
abformat_init (ശൂന്യം)
{
  അക്ഷരം സ്ഥിരം *pb[2];
  വേണ്ടി (പൂർണ്ണസംഖ്യ recent = 0; recent < 2; recent++)
    pb[recent] = first_percent_b (long_time_format[recent]);
  എങ്കിൽ (! (pb[0] || pb[1]))
    തിരികെ;

  അക്ഷരം abmon[12][ABFORMAT_SIZE];
  എങ്കിൽ (! abmon_init (abmon))
    തിരികെ;

  വേണ്ടി (പൂർണ്ണസംഖ്യ recent = 0; recent < 2; recent++)
    {
      അക്ഷരം സ്ഥിരം *fmt = long_time_format[recent];
      വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < 12; i++)
        {
          അക്ഷരം *nfmt = abformat[recent][i];
          പൂർണ്ണസംഖ്യ nbytes;

          എങ്കിൽ (! pb[recent])
            nbytes = snprintf (nfmt, ABFORMAT_SIZE, "%s", fmt);
          അല്ലെങ്കിൽ
            {
              എങ്കിൽ (! (pb[recent] - fmt <= MIN (ABFORMAT_SIZE, INT_MAX)))
                തിരികെ;
              പൂർണ്ണസംഖ്യ prefix_len = pb[recent] - fmt;
              nbytes = snprintf (nfmt, ABFORMAT_SIZE, "%.*s%s%s",
                                 prefix_len, fmt, abmon[i], pb[recent] + 2);
            }

          എങ്കിൽ (! (0 <= nbytes && nbytes < ABFORMAT_SIZE))
            തിരികെ;
        }
    }

  use_abformat = true;
}

സ്ഥിതം size_t
dev_ino_hash (ശൂന്യം സ്ഥിരം *x, size_t table_size)
{
  ഘടന dev_ino സ്ഥിരം *p = x;
  തിരികെ (uintmax_t) {p->st_ino} % table_size;
}

സ്ഥിതം bool
dev_ino_compare (ശൂന്യം സ്ഥിരം *x, ശൂന്യം സ്ഥിരം *y)
{
  ഘടന dev_ino സ്ഥിരം *a = x;
  ഘടന dev_ino സ്ഥിരം *b = y;
  തിരികെ PSAME_INODE (a, b);
}

സ്ഥിതം ശൂന്യം
dev_ino_free (ശൂന്യം *x)
{
  സ്വതന്ത്രം (x);
}

/* Add the device/inode pair (P->st_dev/P->st_ino) to the set of
   active directories.  Return true if there is already a matching
   entry in the table.  */

സ്ഥിതം bool
visit_dir (dev_t dev, ino_t ino)
{
  ഘടന dev_ino *ent;
  ഘടന dev_ino *ent_from_table;
  bool found_match;

  ent = xmalloc (വലുപ്പം *ent);
  ent->st_ino = ino;
  ent->st_dev = dev;

  /* Attempt to insert this entry into the table.  */
  ent_from_table = hash_തിരുകുക (active_dir_set, ent);

  എങ്കിൽ (ent_from_table == ശൂന്യ)
    {
      /* Insertion failed due to lack of memory.  */
      xalloc_die ();
    }

  found_match = (ent_from_table != ent);

  എങ്കിൽ (found_match)
    {
      /* ent was not inserted, so free it.  */
      സ്വതന്ത്രം (ent);
    }

  തിരികെ found_match;
}

സ്ഥിതം ശൂന്യം
free_pending_ent (ഘടന pending *p)
{
  സ്വതന്ത്രം (p->name);
  സ്വതന്ത്രം (p->realname);
  സ്വതന്ത്രം (p);
}

സ്ഥിതം bool
is_colored (എണ്ണൽ indicator_no type)
{
  /* Return true unless the string is "", "0" or "00"; try to be efficient.  */
  size_t len = color_indicator[type].len;
  എങ്കിൽ (len == 0)
    തിരികെ false;
  എങ്കിൽ (2 < len)
    തിരികെ true;
  അക്ഷരം സ്ഥിരം *s = color_indicator[type].string;
  തിരികെ (s[0] != '0') | (s[len - 1] != '0');
}

സ്ഥിതം ശൂന്യം
restore_default_color (ശൂന്യം)
{
  put_indicator (&color_indicator[C_ഇടത്]);
  put_indicator (&color_indicator[C_വലത്]);
}

സ്ഥിതം ശൂന്യം
set_normal_color (ശൂന്യം)
{
  എങ്കിൽ (print_with_color && is_colored (C_NORM))
    {
      put_indicator (&color_indicator[C_ഇടത്]);
      put_indicator (&color_indicator[C_NORM]);
      put_indicator (&color_indicator[C_വലത്]);
    }
}

/* An ordinary signal was received; arrange for the program to exit.  */

സ്ഥിതം ശൂന്യം
sighandler (പൂർണ്ണസംഖ്യ sig)
{
  എങ്കിൽ (! interrupt_signal)
    interrupt_signal = sig;
}

/* A SIGTSTP was received; arrange for the program to suspend itself.  */

സ്ഥിതം ശൂന്യം
stophandler (MAYBE_UNUSED പൂർണ്ണസംഖ്യ sig)
{
  എങ്കിൽ (! interrupt_signal)
    stop_signal_എണ്ണം++;
}

/* Process any pending signals.  If signals are caught, this function
   should be called periodically.  Ideally there should never be an
   unbounded amount of time when signals are not being processed.
   Signal handling can restore the default colors, so callers must
   immediately change colors after invoking this function.  */

സ്ഥിതം ശൂന്യം
process_signals (ശൂന്യം)
{
  ആകുമ്പോൾ (interrupt_signal || stop_signal_എണ്ണം)
    {
      പൂർണ്ണസംഖ്യ sig;
      പൂർണ്ണസംഖ്യ stops;
      sigset_t oldset;

      എങ്കിൽ (used_color)
        restore_default_color ();
      fflush (stdout);

      sigprocmask (SIG_BLOCK, &caught_signals, &oldset);

      /* Reload interrupt_signal and stop_signal_count, in case a new
         signal was handled before sigprocmask took effect.  */
      sig = interrupt_signal;
      stops = stop_signal_എണ്ണം;

      /* SIGTSTP is special, since the application can receive that signal
         more than once.  In this case, don't set the signal handler to the
         default.  Instead, just raise the uncatchable SIGSTOP.  */
      എങ്കിൽ (stops)
        {
          stop_signal_എണ്ണം = stops - 1;
          sig = SIGSTOP;
        }
      അല്ലെങ്കിൽ
        സൂചന (sig, SIG_DFL);

      /* Exit or suspend the program.  */
      ഉയർത്തുക (sig);
      sigprocmask (SIG_SETMASK, &oldset, ശൂന്യ);

      /* If execution reaches here, then the program has been
         continued (after being suspended).  */
    }
}

/* Setup signal handlers if INIT is true,
   otherwise restore to the default.  */

സ്ഥിതം ശൂന്യം
signal_setup (bool init)
{
  /* The signals that are trapped, and the number of such signals.  */
  സ്ഥിതം പൂർണ്ണസംഖ്യ സ്ഥിരം stop_sig[] =
    {
      /* This one is handled specially.  */
      SIGTSTP,
    };

  എണ്ണൽ { nsigs = countof (term_sig), nstop = countof (stop_sig) };

  എങ്കിൽ (init)
    {
      ഘടന sigaction act;

      sigemptyset (&caught_signals);
      വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < nsigs + nstop; j++)
        {
          പൂർണ്ണസംഖ്യ sig = j < nsigs ? term_sig[j]: stop_sig[j - nsigs];
          sigaction (sig, ശൂന്യ, &act);
          എങ്കിൽ (act.sa_handler != SIG_IGN)
            sigaddset (&caught_signals, sig);
        }

      act.sa_mask = caught_signals;
      act.sa_flags = SA_RESTART;

      വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < nsigs + nstop; j++)
        {
          പൂർണ്ണസംഖ്യ sig = j < nsigs ? term_sig[j]: stop_sig[j - nsigs];
          എങ്കിൽ (sigismember (&caught_signals, sig))
            {
              act.sa_handler = j < nsigs ? sighandler : stophandler;
              sigaction (sig, &act, ശൂന്യ);
            }
        }
    }
  അല്ലെങ്കിൽ /* restore.  */
    {
      വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < nsigs + nstop; j++)
        {
          പൂർണ്ണസംഖ്യ sig = j < nsigs ? term_sig[j]: stop_sig[j - nsigs];
          എങ്കിൽ (sigismember (&caught_signals, sig))
            സൂചന (sig, SIG_DFL);
        }
    }
}

സ്ഥിതം ശൂന്യം
signal_init (ശൂന്യം)
{
  signal_setup (true);
}

സ്ഥിതം ശൂന്യം
signal_restore (ശൂന്യം)
{
  signal_setup (false);
}

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  പൂർണ്ണസംഖ്യ i;
  ഘടന pending *thispend;
  പൂർണ്ണസംഖ്യ n_files;

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
  pending_dirs = ശൂന്യ;

  current_time.tv_sec = TYPE_MINIMUM (time_t);
  current_time.tv_nsec = -1;

  i = decode_switches (argc, argv);

  എങ്കിൽ (print_with_color)
    parse_ls_color ();

  /* Test print_with_color again, because the call to parse_ls_color
     may have just reset it -- e.g., if LS_COLORS is invalid.  */

  എങ്കിൽ (print_with_color)
    {
      /* Don't use TAB characters in output.  Some terminal
         emulators can't handle the combination of tabs and
         color codes on the same line.  */
      tabsize = 0;
    }

  എങ്കിൽ (directories_first)
    check_symlink_mode = true;
  അല്ലെങ്കിൽ എങ്കിൽ (print_with_color)
    {
      /* Avoid following symbolic links when possible.  */
      എങ്കിൽ (is_colored (C_ORPHAN)
          || (is_colored (C_EXEC) && color_symlink_as_referent)
          || (is_colored (C_MISSING) && format == long_format))
        check_symlink_mode = true;
    }

  എങ്കിൽ (dereference == DEREF_UNDEFINED)
    dereference = ((immediate_dirs
                    || indicator_style == classify
                    || format == long_format)
                   ? DEREF_NEVER
                   : DEREF_COMMAND_വരി_SYMLINK_TO_DIR);

  /* When using -R, initialize a data structure we'll use to
     detect any directory cycles.  */
  എങ്കിൽ (recursive)
    {
      active_dir_set = hash_initialize (INITIAL_TABLE_SIZE, ശൂന്യ,
                                        dev_ino_hash,
                                        dev_ino_compare,
                                        dev_ino_free);
      എങ്കിൽ (active_dir_set == ശൂന്യ)
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

  എങ്കിൽ (dired)
    {
      obstack_init (&dired_obstack);
      obstack_init (&subdired_obstack);
    }

  എങ്കിൽ (print_hyperlink)
    {
      file_escape_init ();

      hostname = xgethostname ();
      /* The hostname is generally ignored,
         so ignore failures obtaining it.  */
      എങ്കിൽ (! hostname)
        hostname = "";
    }

  cwd_n_alloc = 100;
  cwd_file = xmalloc (cwd_n_alloc * വലുപ്പം *cwd_file);
  cwd_n_used = 0;

  clear_files ();

  n_files = argc - i;

  എങ്കിൽ (n_files <= 0)
    {
      എങ്കിൽ (immediate_dirs)
        gobble_file (".", directory, NOT_AN_INODE_സംഖ്യ, true, ശൂന്യ);
      അല്ലെങ്കിൽ
        queue_directory (".", ശൂന്യ, true);
    }
  അല്ലെങ്കിൽ
    ചെയ്യുക
      gobble_file (argv[i++], unknown, NOT_AN_INODE_സംഖ്യ, true, ശൂന്യ);
    ആകുമ്പോൾ (i < argc);

  എങ്കിൽ (cwd_n_used)
    {
      sort_files ();
      എങ്കിൽ (!immediate_dirs)
        extract_dirs_from_files (ശൂന്യ, true);
      /* 'cwd_n_used' might be zero now.  */
    }

  /* In the following if/else blocks, it is sufficient to test 'pending_dirs'
     (and not pending_dirs->name) because there may be no markers in the queue
     at this point.  A marker may be enqueued when extract_dirs_from_files is
     called with a non-empty string or via print_dir.  */
  എങ്കിൽ (cwd_n_used)
    {
      print_current_files ();
      എങ്കിൽ (pending_dirs)
        dired_outbyte ('\n');
    }
  അല്ലെങ്കിൽ എങ്കിൽ (n_files <= 1 && pending_dirs && !pending_dirs->next)
    print_dir_name = false;

  ആകുമ്പോൾ (pending_dirs)
    {
      thispend = pending_dirs;
      pending_dirs = pending_dirs->next;

      എങ്കിൽ (LOOP_DETECT)
        {
          എങ്കിൽ (thispend->name == ശൂന്യ)
            {
              /* thispend->name == NULL means this is a marker entry
                 indicating we've finished processing the directory.
                 Use its dev/ino numbers to remove the corresponding
                 entry from the active_dir_set hash table.  */
              ഘടന dev_ino di = dev_ino_എടുക്കുക ();
              ഘടന dev_ino *found = hash_remove (active_dir_set, &di);
              എങ്കിൽ (false)
                assert_matching_dev_ino (thispend->realname, di);
              affirm (found);
              dev_ino_free (found);
              free_pending_ent (thispend);
              തുടരുക;
            }
        }

      print_dir (thispend->name, thispend->realname,
                 thispend->command_വരി_arg);

      free_pending_ent (thispend);
      print_dir_name = true;
    }

  എങ്കിൽ (print_with_color && used_color)
    {

      /* Skip the restore when it would be a no-op, i.e.,
         when left is "\033[" and right is "m".  */
      എങ്കിൽ (!(color_indicator[C_ഇടത്].len == 2
            && memeq (color_indicator[C_ഇടത്].string, "\033[", 2)
            && color_indicator[C_വലത്].len == 1
            && color_indicator[C_വലത്].string[0] == 'm'))
        restore_default_color ();

      fflush (stdout);

      signal_restore ();

      /* Act on any signals that arrived before the default was restored.
         This can process signals out of order, but there doesn't seem to
         be an easy way to do them in order, and the order isn't that
         important anyway.  */
      വേണ്ടി (പൂർണ്ണസംഖ്യ j = stop_signal_എണ്ണം; j; j--)
        ഉയർത്തുക (SIGSTOP);
      പൂർണ്ണസംഖ്യ j = interrupt_signal;
      എങ്കിൽ (j)
        ഉയർത്തുക (j);
    }

  എങ്കിൽ (dired)
    {
      /* No need to free these since we're about to exit.  */
      dired_dump_obstack ("//DIRED//", &dired_obstack);
      dired_dump_obstack ("//SUBDIRED//", &subdired_obstack);
      printf ("//DIRED-OPTIONS// --quoting-style=%s\n",
              quoting_style_args[get_quoting_style (filename_quoting_options)]);
    }

  എങ്കിൽ (LOOP_DETECT)
    {
      assure (hash_get_n_entries (active_dir_set) == 0);
      hash_free (active_dir_set);
    }

  തിരികെ exit_status;
}

/* Return the line length indicated by the value given by SPEC, or -1
   if unsuccessful.  0 means no limit on line length.  */

സ്ഥിതം ptrdiff_t
decode_വരി_length (അക്ഷരം സ്ഥിരം *spec)
{
  uintmax_t val;

  /* Treat too-large values as if they were 0, which is
     effectively infinity.  */
  തിരഞ്ഞെടുപ്പ് (xstrtoumax (spec, ശൂന്യ, 0, &val, ""))
    {
    അവസ്ഥ LONGINT_OK:
      തിരികെ val <= MIN (PTRDIFF_MAX, SIZE_MAX) ? val : 0;

    അവസ്ഥ LONGINT_OVERFLOW:
      തിരികെ 0;

    അവസ്ഥ LONGINT_INVALID:
    അവസ്ഥ LONGINT_INVALID_SUFFIX_CHAR:
    അവസ്ഥ LONGINT_INVALID_SUFFIX_CHAR_WITH_OVERFLOW:
      തിരികെ -1;

    സ്ഥിരസ്ഥിതി:
      unreachable ();
    }
}

/* Return true if standard output is a tty, caching the result.  */

സ്ഥിതം bool
stdout_isatty (ശൂന്യം)
{
  സ്ഥിതം signed അക്ഷരം out_tty = -1;
  എങ്കിൽ (out_tty < 0)
    out_tty = isatty (STDOUT_FILENO);
  assume (out_tty == 0 || out_tty == 1);
  തിരികെ out_tty;
}

/* Set all the option flags according to the switches specified.
   Return the index of the first non-option argument.  */

സ്ഥിതം പൂർണ്ണസംഖ്യ
decode_switches (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  അക്ഷരം *time_style_option = ശൂന്യ;

  /* These variables are false or -1 unless a switch says otherwise.  */
  bool kibibytes_specified = false;
  പൂർണ്ണസംഖ്യ format_opt = -1;
  പൂർണ്ണസംഖ്യ hide_control_chars_opt = -1;
  പൂർണ്ണസംഖ്യ quoting_style_opt = -1;
  പൂർണ്ണസംഖ്യ sort_opt = -1;
  ptrdiff_t tabsize_opt = -1;
  ptrdiff_t width_opt = -1;

  ആകുമ്പോൾ (true)
    {
      പൂർണ്ണസംഖ്യ oi = -1;
      പൂർണ്ണസംഖ്യ c = getopt_long (argc, argv,
                           "abcdfghiklmnopqrstuvw:xABCDFGHI:LNQRST:UXZ1",
                           long_options, &oi);
      എങ്കിൽ (c == -1)
        നിർത്തുക;

      തിരഞ്ഞെടുപ്പ് (c)
        {
        അവസ്ഥ 'a':
          ignore_mode = IGNORE_MINIMAL;
          നിർത്തുക;

        അവസ്ഥ 'b':
          quoting_style_opt = escape_quoting_style;
          നിർത്തുക;

        അവസ്ഥ 'c':
          time_type = time_ctime;
          explicit_time = true;
          നിർത്തുക;

        അവസ്ഥ 'd':
          immediate_dirs = true;
          നിർത്തുക;

        അവസ്ഥ 'f':
          ignore_mode = IGNORE_MINIMAL; /* enable -a */
          sort_opt = sort_none;         /* enable -U */
          നിർത്തുക;

        അവസ്ഥ FILE_TYPE_INDICATOR_OPTION: /* --file-type */
          indicator_style = file_type;
          നിർത്തുക;

        അവസ്ഥ 'g':
          format_opt = long_format;
          print_owner = false;
          നിർത്തുക;

        അവസ്ഥ 'h':
          file_human_output_opts = human_output_opts =
            human_autoscale | human_SI | human_base_1024;
          file_output_block_size = output_block_size = 1;
          നിർത്തുക;

        അവസ്ഥ 'i':
          print_inode = true;
          നിർത്തുക;

        അവസ്ഥ 'k':
          kibibytes_specified = true;
          നിർത്തുക;

        അവസ്ഥ 'l':
          format_opt = long_format;
          നിർത്തുക;

        അവസ്ഥ 'm':
          format_opt = with_commas;
          നിർത്തുക;

        അവസ്ഥ 'n':
          numeric_ids = true;
          format_opt = long_format;
          നിർത്തുക;

        അവസ്ഥ 'o':  /* Just like -l, but don't display group info.  */
          format_opt = long_format;
          print_group = false;
          നിർത്തുക;

        അവസ്ഥ 'p':
          indicator_style = slash;
          നിർത്തുക;

        അവസ്ഥ 'q':
          hide_control_chars_opt = true;
          നിർത്തുക;

        അവസ്ഥ 'r':
          sort_reverse = true;
          നിർത്തുക;

        അവസ്ഥ 's':
          print_block_size = true;
          നിർത്തുക;

        അവസ്ഥ 't':
          sort_opt = sort_time;
          നിർത്തുക;

        അവസ്ഥ 'u':
          time_type = time_atime;
          explicit_time = true;
          നിർത്തുക;

        അവസ്ഥ 'v':
          sort_opt = sort_version;
          നിർത്തുക;

        അവസ്ഥ 'w':
          width_opt = decode_വരി_length (optarg);
          എങ്കിൽ (width_opt < 0)
            error (LS_FAILURE, 0, "%s: %s", _("invalid line width"),
                   quote (optarg));
          നിർത്തുക;

        അവസ്ഥ 'x':
          format_opt = horizontal;
          നിർത്തുക;

        അവസ്ഥ 'A':
          ignore_mode = IGNORE_DOT_AND_DOTDOT;
          നിർത്തുക;

        അവസ്ഥ 'B':
          add_ignore_pattern ("*~");
          add_ignore_pattern (".*~");
          നിർത്തുക;

        അവസ്ഥ 'C':
          format_opt = many_per_വരി;
          നിർത്തുക;

        അവസ്ഥ 'D':
          format_opt = long_format;
          print_hyperlink = false;
          dired = true;
          നിർത്തുക;

        അവസ്ഥ 'F':
          {
            പൂർണ്ണസംഖ്യ i;
            എങ്കിൽ (optarg)
              i = XARGMATCH ("--classify", optarg, when_args, when_types);
            അല്ലെങ്കിൽ
              /* Using --classify with no argument is equivalent to using
                 --classify=always.  */
              i = when_always;

            എങ്കിൽ (i == when_always || (i == when_if_tty && stdout_isatty ()))
              indicator_style = classify;
            നിർത്തുക;
          }

        അവസ്ഥ 'G':		/* inhibit display of group info */
          print_group = false;
          നിർത്തുക;

        അവസ്ഥ 'H':
          dereference = DEREF_COMMAND_വരി_ARGUMENTS;
          നിർത്തുക;

        അവസ്ഥ DEREFERENCE_COMMAND_വരി_SYMLINK_TO_DIR_OPTION:
          dereference = DEREF_COMMAND_വരി_SYMLINK_TO_DIR;
          നിർത്തുക;

        അവസ്ഥ 'I':
          add_ignore_pattern (optarg);
          നിർത്തുക;

        അവസ്ഥ 'L':
          dereference = DEREF_ALWAYS;
          നിർത്തുക;

        അവസ്ഥ 'N':
          quoting_style_opt = literal_quoting_style;
          നിർത്തുക;

        അവസ്ഥ 'Q':
          quoting_style_opt = c_quoting_style;
          നിർത്തുക;

        അവസ്ഥ 'R':
          recursive = true;
          നിർത്തുക;

        അവസ്ഥ 'S':
          sort_opt = sort_size;
          നിർത്തുക;

        അവസ്ഥ 'T':
          tabsize_opt = xnumtoumax (optarg, 0, 0, MIN (PTRDIFF_MAX, SIZE_MAX),
                                    "", _("invalid tab size"), LS_FAILURE, 0);
          നിർത്തുക;

        അവസ്ഥ 'U':
          sort_opt = sort_none;
          നിർത്തുക;

        അവസ്ഥ 'X':
          sort_opt = sort_extension;
          നിർത്തുക;

        അവസ്ഥ '1':
          /* -1 has no effect after -l.  */
          എങ്കിൽ (format_opt != long_format)
            format_opt = one_per_വരി;
          നിർത്തുക;

        അവസ്ഥ AUTHOR_OPTION:
          print_author = true;
          നിർത്തുക;

        അവസ്ഥ HIDE_OPTION:
          {
            ഘടന ignore_pattern *hide = xmalloc (വലുപ്പം *hide);
            hide->pattern = optarg;
            hide->next = hide_patterns;
            hide_patterns = hide;
          }
          നിർത്തുക;

        അവസ്ഥ SORT_OPTION:
          sort_opt = XARGMATCH ("--sort", optarg, sort_args, sort_types);
          നിർത്തുക;

        അവസ്ഥ GROUP_DIRECTORIES_FIRST_OPTION:
          directories_first = true;
          നിർത്തുക;

        അവസ്ഥ TIME_OPTION:
          time_type = XARGMATCH ("--time", optarg, time_args, time_types);
          explicit_time = true;
          നിർത്തുക;

        അവസ്ഥ FORMAT_OPTION:
          format_opt = XARGMATCH ("--format", optarg, format_args,
                                  format_types);
          നിർത്തുക;

        അവസ്ഥ FULL_TIME_OPTION:
          format_opt = long_format;
          time_style_option = (അക്ഷരം *) "full-iso";
          നിർത്തുക;

        അവസ്ഥ COLOR_OPTION:
          {
            പൂർണ്ണസംഖ്യ i;
            എങ്കിൽ (optarg)
              i = XARGMATCH ("--color", optarg, when_args, when_types);
            അല്ലെങ്കിൽ
              /* Using --color with no argument is equivalent to using
                 --color=always.  */
              i = when_always;

            print_with_color = (i == when_always
                                || (i == when_if_tty && stdout_isatty ()));
            നിർത്തുക;
          }

        അവസ്ഥ HYPERLINK_OPTION:
          {
            പൂർണ്ണസംഖ്യ i;
            എങ്കിൽ (optarg)
              i = XARGMATCH ("--hyperlink", optarg, when_args, when_types);
            അല്ലെങ്കിൽ
              /* Using --hyperlink with no argument is equivalent to using
                 --hyperlink=always.  */
              i = when_always;

            print_hyperlink = (i == when_always
                               || (i == when_if_tty && stdout_isatty ()));
            നിർത്തുക;
          }

        അവസ്ഥ INDICATOR_STYLE_OPTION:
          indicator_style = XARGMATCH ("--indicator-style", optarg,
                                       indicator_style_args,
                                       indicator_style_types);
          നിർത്തുക;

        അവസ്ഥ QUOTING_STYLE_OPTION:
          quoting_style_opt = XARGMATCH ("--quoting-style", optarg,
                                         quoting_style_args,
                                         quoting_style_vals);
          നിർത്തുക;

        അവസ്ഥ TIME_STYLE_OPTION:
          time_style_option = optarg;
          നിർത്തുക;

        അവസ്ഥ SHOW_CONTROL_CHARS_OPTION:
          hide_control_chars_opt = false;
          നിർത്തുക;

        അവസ്ഥ BLOCK_SIZE_OPTION:
          {
            എണ്ണൽ strtol_error e = human_options (optarg, &human_output_opts,
                                                 &output_block_size);
            എങ്കിൽ (e != LONGINT_OK)
              xstrtol_fatal (e, oi, 0, long_options, optarg);
            file_human_output_opts = human_output_opts;
            file_output_block_size = output_block_size;
          }
          നിർത്തുക;

        അവസ്ഥ SI_OPTION:
          file_human_output_opts = human_output_opts =
            human_autoscale | human_SI;
          file_output_block_size = output_block_size = 1;
          നിർത്തുക;

        അവസ്ഥ 'Z':
          print_scontext = true;
          നിർത്തുക;

        അവസ്ഥ ZERO_OPTION:
          eolbyte = 0;
          hide_control_chars_opt = false;
          എങ്കിൽ (format_opt != long_format)
            format_opt = one_per_വരി;
          print_with_color = false;
          quoting_style_opt = literal_quoting_style;
          നിർത്തുക;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        സ്ഥിരസ്ഥിതി:
          usage (LS_FAILURE);
        }
    }

  എങ്കിൽ (! output_block_size)
    {
      അക്ഷരം സ്ഥിരം *ls_block_size = getenv ("LS_BLOCK_SIZE");
      human_options (ls_block_size,
                     &human_output_opts, &output_block_size);
      എങ്കിൽ (ls_block_size || getenv ("BLOCK_SIZE"))
        {
          file_human_output_opts = human_output_opts;
          file_output_block_size = output_block_size;
        }
      എങ്കിൽ (kibibytes_specified)
        {
          human_output_opts = 0;
          output_block_size = 1024;
        }
    }

  format = (0 <= format_opt ? format_opt
            : ls_mode == LS_LS ? (stdout_isatty ()
                                  ? many_per_വരി : one_per_വരി)
            : ls_mode == LS_MULTI_COL ? many_per_വരി
            : /* ls_mode == LS_LONG_FORMAT */ long_format);

  /* If the line length was not set by a switch but is needed to determine
     output, go to the work of obtaining it from the environment.  */
  ptrdiff_t linelen = width_opt;
  എങ്കിൽ (format == many_per_വരി || format == horizontal || format == with_commas
      || print_with_color)
    {
#ifdef TIOCGWINSZ
      എങ്കിൽ (linelen < 0)
        {
          ഘടന winsize ws;
          എങ്കിൽ (stdout_isatty ()
              && 0 <= ioctl (STDOUT_FILENO, TIOCGWINSZ, &ws)
              && 0 < ws.ws_col)
            linelen = ws.ws_col <= MIN (PTRDIFF_MAX, SIZE_MAX) ? ws.ws_col : 0;
        }
#endif
      എങ്കിൽ (linelen < 0)
        {
          അക്ഷരം സ്ഥിരം *p = getenv ("COLUMNS");
          എങ്കിൽ (p && *p)
            {
              linelen = decode_വരി_length (p);
              എങ്കിൽ (linelen < 0)
                error (0, 0,
                       _("ignoring invalid width"
                         " in environment variable COLUMNS: %s"),
                       quote (p));
            }
        }
    }

  വരി_length = linelen < 0 ? 80 : linelen;

  /* Determine the max possible number of display columns.  */
  max_idx = വരി_length / MIN_COLUMN_WIDTH;
  /* Account for first display column not having a separator,
     or line_lengths shorter than MIN_COLUMN_WIDTH.  */
  max_idx += വരി_length % MIN_COLUMN_WIDTH != 0;

  എങ്കിൽ (format == many_per_വരി || format == horizontal || format == with_commas)
    {
      എങ്കിൽ (0 <= tabsize_opt)
        tabsize = tabsize_opt;
      അല്ലെങ്കിൽ
        {
          tabsize = 8;
          അക്ഷരം സ്ഥിരം *p = getenv ("TABSIZE");
          എങ്കിൽ (p)
            {
              uintmax_t tmp;
              എങ്കിൽ (xstrtoumax (p, ശൂന്യ, 0, &tmp, "") == LONGINT_OK
                  && tmp <= SIZE_MAX)
                tabsize = tmp;
              അല്ലെങ്കിൽ
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
                  && ((വരി_length
                       && (format == with_commas
                           || format == many_per_വരി || format == horizontal))
                      || format == one_per_വരി || format == long_format);

  പൂർണ്ണസംഖ്യ qs = quoting_style_opt;
  എങ്കിൽ (qs < 0)
    qs = getenv_quoting_style ();
  എങ്കിൽ (qs < 0)
    qs = (ls_mode == LS_LS
          ? (stdout_isatty () ? shell_escape_quoting_style : -1)
          : escape_quoting_style);
  എങ്കിൽ (0 <= qs)
    set_quoting_style (ശൂന്യ, qs);
  qs = get_quoting_style (ശൂന്യ);
  align_variable_outer_quotes
    = ((format == long_format
        || ((format == many_per_വരി || format == horizontal) && വരി_length))
       && (qs == shell_quoting_style
           || qs == shell_escape_quoting_style
           || qs == c_maybe_quoting_style));
  filename_quoting_options = clone_quoting_options (ശൂന്യ);
  എങ്കിൽ (qs == escape_quoting_style)
    set_char_quoting (filename_quoting_options, ' ', 1);
  എങ്കിൽ (file_type <= indicator_style)
    {
      വേണ്ടി (അക്ഷരം സ്ഥിരം *p = &"*=>@|"[indicator_style - file_type]; *p; p++)
        set_char_quoting (filename_quoting_options, *p, 1);
    }
  എങ്കിൽ (format == with_commas
      && (qs == shell_quoting_style
          || qs == shell_escape_quoting_style
          || qs == c_maybe_quoting_style
          || qs == escape_quoting_style))
    set_char_quoting (filename_quoting_options, ',', 1);

  dirname_quoting_options = clone_quoting_options (ശൂന്യ);
  set_char_quoting (dirname_quoting_options, ':', 1);

  /* --dired implies --format=long (-l) and sans --hyperlink.
     So ignore it if those overridden.  */
  dired &= (format == long_format) & !print_hyperlink;

  എങ്കിൽ (eolbyte < dired)
    error (LS_FAILURE, 0, _("--dired and --zero are incompatible"));

  /* If a time type is explicitly specified (with -c, -u, or --time=)
     and we're not showing a time (-l not specified), then sort by that time,
     rather than by name.  Note this behavior is unspecified by POSIX.  */

  sort_type = (0 <= sort_opt ? sort_opt
               : (format != long_format && explicit_time)
               ? sort_time : sort_name);

  എങ്കിൽ (format == long_format)
    {
      അക്ഷരം *envstyle = ശൂന്യ;
      അക്ഷരം *style = time_style_option;
      എങ്കിൽ (! style)
        style = envstyle = getenv ("TIME_STYLE");

      എങ്കിൽ (! style)
        style = (അക്ഷരം *) "locale";
      അല്ലെങ്കിൽ
        {
          സ്ഥിതം അക്ഷരം സ്ഥിരം posix_prefix[] = "posix-";
          ആകുമ്പോൾ (STREQ_LEN (style, posix_prefix, വലുപ്പം posix_prefix - 1))
            {
              എങ്കിൽ (! hard_locale (LC_TIME))
                തിരികെ optind;
              style += വലുപ്പം posix_prefix - 1;
            }
        }

      എങ്കിൽ (*style == '+')
        {
          അക്ഷരം *p0 = style + 1;
          അക്ഷരം *p0nl = strchr (p0, '\n');
          അക്ഷരം സ്ഥിരം *p1 = p0;
          എങ്കിൽ (p0nl)
            {
              p1 = p0nl + 1;
              എങ്കിൽ (strchr (p1, '\n'))
                error (LS_FAILURE, 0, _("invalid time style format %s"),
                       quote (p0));
              എങ്കിൽ (envstyle)
                p0 = xmemdup0 (p0, p0nl - p0);
              അല്ലെങ്കിൽ
                *p0nl = '\0';
            }
          long_time_format[0] = p0;
          long_time_format[1] = p1;
        }
      അല്ലെങ്കിൽ
        {
          തിരഞ്ഞെടുപ്പ് (x_timestyle_match (style, /*allow_posix=*/ true,
                                     time_style_args,
                                     (അക്ഷരം സ്ഥിരം *) time_style_types,
                                     വലുപ്പം (*time_style_types), LS_FAILURE))
            {
            അവസ്ഥ full_iso_time_style:
              long_time_format[0] = long_time_format[1] =
                "%Y-%m-%d %H:%M:%S.%N %z";
              നിർത്തുക;

            അവസ്ഥ long_iso_time_style:
              long_time_format[0] = long_time_format[1] = "%Y-%m-%d %H:%M";
              നിർത്തുക;

            അവസ്ഥ iso_time_style:
              long_time_format[0] = "%Y-%m-%d ";
              long_time_format[1] = "%m-%d %H:%M";
              നിർത്തുക;

            അവസ്ഥ locale_time_style:
              എങ്കിൽ (hard_locale (LC_TIME))
                {
                  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < 2; i++)
                    long_time_format[i] =
                      dcgettext (ശൂന്യ, long_time_format[i], LC_TIME);
                }
            }
        }

      abformat_init ();
    }

  തിരികെ optind;
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

സ്ഥിതം bool
get_funky_string (അക്ഷരം **dest, അക്ഷരം സ്ഥിരം **src, bool equals_end,
                  size_t *output_എണ്ണം)
{
  അക്ഷരം അക്കം;			/* For numerical codes */
  size_t എണ്ണം;			/* Something to count with */
  എണ്ണൽ {
    ST_GND, ST_BACKSLASH, ST_OCTAL, ST_HEX, ST_CARET, ST_END, ST_ERROR
  } state;
  അക്ഷരം സ്ഥിരം *p;
  അക്ഷരം *q;

  p = *src;			/* We don't want to double-indirect */
  q = *dest;			/* the whole darn time.  */

  എണ്ണം = 0;			/* No characters counted in yet.  */
  അക്കം = 0;

  state = ST_GND;		/* Start in ground state.  */
  ആകുമ്പോൾ (state < ST_END)
    {
      തിരഞ്ഞെടുപ്പ് (state)
        {
        അവസ്ഥ ST_GND:		/* Ground state (no escapes) */
          തിരഞ്ഞെടുപ്പ് (*p)
            {
            അവസ്ഥ ':':
            അവസ്ഥ '\0':
              state = ST_END;	/* End of string */
              നിർത്തുക;
            അവസ്ഥ '\\':
              state = ST_BACKSLASH; /* Backslash escape sequence */
              ++p;
              നിർത്തുക;
            അവസ്ഥ '^':
              state = ST_CARET; /* Caret escape */
              ++p;
              നിർത്തുക;
            അവസ്ഥ '=':
              എങ്കിൽ (equals_end)
                {
                  state = ST_END; /* End */
                  നിർത്തുക;
                }
              FALLTHROUGH;
            സ്ഥിരസ്ഥിതി:
              *(q++) = *(p++);
              ++എണ്ണം;
              നിർത്തുക;
            }
          നിർത്തുക;

        അവസ്ഥ ST_BACKSLASH:	/* Backslash escaped character */
          തിരഞ്ഞെടുപ്പ് (*p)
            {
            അവസ്ഥ '0':
            അവസ്ഥ '1':
            അവസ്ഥ '2':
            അവസ്ഥ '3':
            അവസ്ഥ '4':
            അവസ്ഥ '5':
            അവസ്ഥ '6':
            അവസ്ഥ '7':
              state = ST_OCTAL;	/* Octal sequence */
              അക്കം = *p - '0';
              നിർത്തുക;
            അവസ്ഥ 'x':
            അവസ്ഥ 'X':
              state = ST_HEX;	/* Hex sequence */
              അക്കം = 0;
              നിർത്തുക;
            അവസ്ഥ 'a':		/* Bell */
              അക്കം = '\a';
              നിർത്തുക;
            അവസ്ഥ 'b':		/* Backspace */
              അക്കം = '\b';
              നിർത്തുക;
            അവസ്ഥ 'e':		/* Escape */
              അക്കം = 27;
              നിർത്തുക;
            അവസ്ഥ 'f':		/* Form feed */
              അക്കം = '\f';
              നിർത്തുക;
            അവസ്ഥ 'n':		/* Newline */
              അക്കം = '\n';
              നിർത്തുക;
            അവസ്ഥ 'r':		/* Carriage return */
              അക്കം = '\r';
              നിർത്തുക;
            അവസ്ഥ 't':		/* Tab */
              അക്കം = '\t';
              നിർത്തുക;
            അവസ്ഥ 'v':		/* Vtab */
              അക്കം = '\v';
              നിർത്തുക;
            അവസ്ഥ '?':		/* Delete */
              അക്കം = 127;
              നിർത്തുക;
            അവസ്ഥ '_':		/* Space */
              അക്കം = ' ';
              നിർത്തുക;
            അവസ്ഥ '\0':		/* End of string */
              state = ST_ERROR;	/* Error! */
              നിർത്തുക;
            സ്ഥിരസ്ഥിതി:		/* Escaped character like \ ^ : = */
              അക്കം = *p;
              നിർത്തുക;
            }
          എങ്കിൽ (state == ST_BACKSLASH)
            {
              *(q++) = അക്കം;
              ++എണ്ണം;
              state = ST_GND;
            }
          ++p;
          നിർത്തുക;

        അവസ്ഥ ST_OCTAL:		/* Octal sequence */
          എങ്കിൽ (*p < '0' || *p > '7')
            {
              *(q++) = അക്കം;
              ++എണ്ണം;
              state = ST_GND;
            }
          അല്ലെങ്കിൽ
            അക്കം = (അക്കം << 3) + (*(p++) - '0');
          നിർത്തുക;

        അവസ്ഥ ST_HEX:		/* Hex sequence */
          തിരഞ്ഞെടുപ്പ് (*p)
            {
            അവസ്ഥ '0':
            അവസ്ഥ '1':
            അവസ്ഥ '2':
            അവസ്ഥ '3':
            അവസ്ഥ '4':
            അവസ്ഥ '5':
            അവസ്ഥ '6':
            അവസ്ഥ '7':
            അവസ്ഥ '8':
            അവസ്ഥ '9':
              അക്കം = (അക്കം << 4) + (*(p++) - '0');
              നിർത്തുക;
            അവസ്ഥ 'a':
            അവസ്ഥ 'b':
            അവസ്ഥ 'c':
            അവസ്ഥ 'd':
            അവസ്ഥ 'e':
            അവസ്ഥ 'f':
              അക്കം = (അക്കം << 4) + (*(p++) - 'a') + 10;
              നിർത്തുക;
            അവസ്ഥ 'A':
            അവസ്ഥ 'B':
            അവസ്ഥ 'C':
            അവസ്ഥ 'D':
            അവസ്ഥ 'E':
            അവസ്ഥ 'F':
              അക്കം = (അക്കം << 4) + (*(p++) - 'A') + 10;
              നിർത്തുക;
            സ്ഥിരസ്ഥിതി:
              *(q++) = അക്കം;
              ++എണ്ണം;
              state = ST_GND;
              നിർത്തുക;
            }
          നിർത്തുക;

        അവസ്ഥ ST_CARET:		/* Caret escape */
          state = ST_GND;	/* Should be the next state... */
          എങ്കിൽ (*p >= '@' && *p <= '~')
            {
              *(q++) = *(p++) & 037;
              ++എണ്ണം;
            }
          അല്ലെങ്കിൽ എങ്കിൽ (*p == '?')
            {
              *(q++) = 127;
              ++എണ്ണം;
            }
          അല്ലെങ്കിൽ
            state = ST_ERROR;
          നിർത്തുക;

        അവസ്ഥ ST_END: അവസ്ഥ ST_ERROR: സ്ഥിരസ്ഥിതി:
          affirm (false);
        }
    }

  *dest = q;
  *src = p;
  *output_എണ്ണം = എണ്ണം;

  തിരികെ state != ST_ERROR;
}

എണ്ണൽ parse_state
  {
    PS_START = 1,
    PS_2,
    PS_3,
    PS_4,
    PS_DONE,
    PS_FAIL
  };


/* Check if the content of TERM is a valid name in dircolors.  */

സ്ഥിതം bool
known_term_type (ശൂന്യം)
{
  അക്ഷരം സ്ഥിരം *term = getenv ("TERM");
  എങ്കിൽ (! term || ! *term)
    തിരികെ false;

  അക്ഷരം സ്ഥിരം *വരി = G_വരി;
  ആകുമ്പോൾ (വരി - G_വരി < വലുപ്പം (G_വരി))
    {
      എങ്കിൽ (STRNCMP_LIT (വരി, "TERM ") == 0)
        {
          എങ്കിൽ (fnmatch (വരി + 5, term, 0) == 0)
            തിരികെ true;
        }
      വരി += strlen (വരി) + 1;
    }

  തിരികെ false;
}

സ്ഥിതം ശൂന്യം
parse_ls_color (ശൂന്യം)
{
  അക്ഷരം സ്ഥിരം *p;		/* Pointer to character being parsed */
  അക്ഷരം *buf;			/* color_buf buffer pointer */
  അക്ഷരം label0, label1;		/* Indicator label */
  ഘടന color_ext_type *ext;	/* Extension we are working on */

  എങ്കിൽ ((p = getenv ("LS_COLORS")) == ശൂന്യ || *p == '\0')
    {
      /* LS_COLORS takes precedence, but if that's not set then
         honor the COLORTERM and TERM env variables so that
         we only go with the internal ANSI color codes if the
         former is non empty or the latter is set to a known value.  */
      അക്ഷരം സ്ഥിരം *colorterm = getenv ("COLORTERM");
      എങ്കിൽ (! (colorterm && *colorterm) && ! known_term_type ())
        print_with_color = false;
      തിരികെ;
    }

  ext = ശൂന്യ;

  /* This is an overly conservative estimate, but any possible
     LS_COLORS string will *not* generate a color_buf longer than
     itself, so it is a safe way of allocating a buffer in
     advance.  */
  buf = color_buf = xstrdup (p);

  എണ്ണൽ parse_state state = PS_START;
  ആകുമ്പോൾ (true)
    {
      തിരഞ്ഞെടുപ്പ് (state)
        {
        അവസ്ഥ PS_START:		/* First label character */
          തിരഞ്ഞെടുപ്പ് (*p)
            {
            അവസ്ഥ ':':
              ++p;
              നിർത്തുക;

            അവസ്ഥ '*':
              /* Allocate new extension block and add to head of
                 linked list (this way a later definition will
                 override an earlier one, which can be useful for
                 having terminal-specific defs override global).  */

              ext = xmalloc (വലുപ്പം *ext);
              ext->next = color_ext_list;
              color_ext_list = ext;
              ext->exact_match = false;

              ++p;
              ext->ext.string = buf;

              state = (get_funky_string (&buf, &p, true, &ext->ext.len)
                       ? PS_4 : PS_FAIL);
              നിർത്തുക;

            അവസ്ഥ '\0':
              state = PS_DONE;	/* Done! */
              പോകുക done;

            സ്ഥിരസ്ഥിതി:	/* Assume it is file type label */
              label0 = *p++;
              state = PS_2;
              നിർത്തുക;
            }
          നിർത്തുക;

        അവസ്ഥ PS_2:		/* Second label character */
          എങ്കിൽ (*p)
            {
              label1 = *p++;
              state = PS_3;
            }
          അല്ലെങ്കിൽ
            state = PS_FAIL;	/* Error */
          നിർത്തുക;

        അവസ്ഥ PS_3:		/* Equal sign after indicator label */
          state = PS_FAIL;	/* Assume failure...  */
          എങ്കിൽ (*(p++) == '=')/* It *should* be...  */
            {
              വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < countof (indicator_name); i++)
                {
                  എങ്കിൽ ((label0 == indicator_name[i][0])
                      && (label1 == indicator_name[i][1]))
                    {
                      color_indicator[i].string = buf;
                      state = (get_funky_string (&buf, &p, false,
                                                 &color_indicator[i].len)
                               ? PS_START : PS_FAIL);
                      നിർത്തുക;
                    }
                }
              എങ്കിൽ (state == PS_FAIL)
                error (0, 0, _("unrecognized prefix: %s"),
                       quote ((അക്ഷരം []) {label0, label1, '\0'}));
            }
          നിർത്തുക;

        അവസ്ഥ PS_4:		/* Equal sign after *.ext */
          എങ്കിൽ (*(p++) == '=')
            {
              ext->seq.string = buf;
              state = (get_funky_string (&buf, &p, false, &ext->seq.len)
                       ? PS_START : PS_FAIL);
            }
          അല്ലെങ്കിൽ
            state = PS_FAIL;
          നിർത്തുക;

        അവസ്ഥ PS_FAIL:
          പോകുക done;

        അവസ്ഥ PS_DONE: സ്ഥിരസ്ഥിതി:
          affirm (false);
        }
    }
 done:

  എങ്കിൽ (state == PS_FAIL)
    {
      error (0, 0,
             _("unparsable value for LS_COLORS environment variable"));
      സ്വതന്ത്രം (color_buf);
      വേണ്ടി (ഘടന color_ext_type *e = color_ext_list; e != ശൂന്യ;
           /* empty */)
        {
          ഘടന color_ext_type *e2 = e;
          e = e->next;
          സ്വതന്ത്രം (e2);
        }
      print_with_color = false;
      തിരികെ;
    }
  അല്ലെങ്കിൽ
    {
      /* Postprocess list to set EXACT_MATCH on entries where there are
         different cased extensions with separate sequences defined.
         Also set ext.len to SIZE_MAX on any entries that can't
         match due to precedence, to avoid redundant string compares.  */
      വേണ്ടി (ഘടന color_ext_type *e1 = color_ext_list; e1 != ശൂന്യ;
           e1 = e1->next)
        {
          bool case_ignored = false;

          വേണ്ടി (ഘടന color_ext_type *e2 = e1->next; e2 != ശൂന്യ;
               e2 = e2->next)
            {
              എങ്കിൽ (e2->ext.len < SIZE_MAX && e1->ext.len == e2->ext.len)
                {
                  എങ്കിൽ (memeq (e1->ext.string, e2->ext.string, e1->ext.len))
                    e2->ext.len = SIZE_MAX; /* Ignore */
                  അല്ലെങ്കിൽ എങ്കിൽ (c_strncasecmp (e1->ext.string, e2->ext.string,
                                          e1->ext.len) == 0)
                    {
                      എങ്കിൽ (case_ignored)
                        {
                          e2->ext.len = SIZE_MAX; /* Ignore */
                        }
                      അല്ലെങ്കിൽ എങ്കിൽ (e1->seq.len == e2->seq.len
                               && memeq (e1->seq.string, e2->seq.string,
                                         e1->seq.len))
                        {
                          e2->ext.len = SIZE_MAX; /* Ignore */
                          case_ignored = true;    /* Ignore all subsequent */
                        }
                      അല്ലെങ്കിൽ
                        {
                          e1->exact_match = true;
                          e2->exact_match = true;
                        }
                    }
                }
            }
        }
    }

  എങ്കിൽ (color_indicator[C_LINK].len == 6
      && !STRNCMP_LIT (color_indicator[C_LINK].string, "target"))
    color_symlink_as_referent = true;
}

/* Set the exit status to report a failure.  If SERIOUS, it is a
   serious failure; otherwise, it is merely a minor problem.  */

സ്ഥിതം ശൂന്യം
set_exit_status (bool serious)
{
  എങ്കിൽ (serious)
    exit_status = LS_FAILURE;
  അല്ലെങ്കിൽ എങ്കിൽ (exit_status == EXIT_SUCCESS)
    exit_status = LS_MINOR_PROBLEM;
}

/* Assuming a failure is serious if SERIOUS, use the printf-style
   MESSAGE to report the failure to access a file named FILE.  Assume
   errno is set appropriately for the failure.  */

സ്ഥിതം ശൂന്യം
file_failure (bool serious, അക്ഷരം സ്ഥിരം *message, അക്ഷരം സ്ഥിരം *file)
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

സ്ഥിതം ശൂന്യം
queue_directory (അക്ഷരം സ്ഥിരം *name, അക്ഷരം സ്ഥിരം *realname, bool command_വരി_arg)
{
  ഘടന pending *new = xmalloc (വലുപ്പം *new);
  new->realname = realname ? xstrdup (realname) : ശൂന്യ;
  new->name = name ? xstrdup (name) : ശൂന്യ;
  new->command_വരി_arg = command_വരി_arg;
  new->next = pending_dirs;
  pending_dirs = new;
}

/* Read directory NAME, and list the files in it.
   If REALNAME is nonzero, print its name instead of NAME;
   this is used for symbolic links to directories.
   COMMAND_LINE_ARG means this directory was mentioned on the command line.  */

സ്ഥിതം ശൂന്യം
print_dir (അക്ഷരം സ്ഥിരം *name, അക്ഷരം സ്ഥിരം *realname, bool command_വരി_arg)
{
  DIR *dirp;
  ഘടന dirent *next;
  uintmax_t total_blocks = 0;
  സ്ഥിതം bool first = true;

  errno = 0;
  dirp = opendir (name);
  എങ്കിൽ (!dirp)
    {
      എങ്കിൽ (command_വരി_arg || ! ignorable_traversal_errno (errno))
        file_failure (command_വരി_arg, _("cannot open directory %s"), name);
      തിരികെ;
    }

  എങ്കിൽ (LOOP_DETECT)
    {
      ഘടന stat dir_stat;
      പൂർണ്ണസംഖ്യ fd = dirfd (dirp);

      /* If dirfd failed, endure the overhead of stat'ing by path  */
      എങ്കിൽ ((0 <= fd
           ? fstat_for_ino (fd, &dir_stat)
           : stat_for_ino (name, &dir_stat)) < 0)
        {
          എങ്കിൽ (! ignorable_traversal_errno (errno))
            file_failure (command_വരി_arg,
                          _("cannot determine device and inode of %s"), name);
          closedir (dirp);
          തിരികെ;
        }

      /* If we've already visited this dev/inode pair, warn that
         we've found a loop, and do not process this directory.  */
      എങ്കിൽ (visit_dir (dir_stat.st_dev, dir_stat.st_ino))
        {
          error (0, 0, _("%s: not listing already-listed directory"),
                 quotef (name));
          closedir (dirp);
          set_exit_status (true);
          തിരികെ;
        }

      dev_ino_തള്ളുക (dir_stat.st_dev, dir_stat.st_ino);
    }

  clear_files ();

  എങ്കിൽ (recursive || print_dir_name)
    {
      എങ്കിൽ (!first)
        dired_outbyte ('\n');
      first = false;
      dired_indent ();

      അക്ഷരം *absolute_name = ശൂന്യ;
      എങ്കിൽ (print_hyperlink)
        {
          absolute_name = canonicalize_filename_mode (name, CAN_MISSING);
          എങ്കിൽ (! absolute_name
              && ! ignorable_traversal_errno (errno))
            file_failure (command_വരി_arg,
                          _("error canonicalizing %s"), name);
        }
      quote_name (realname ? realname : name, dirname_quoting_options, -1,
                  ശൂന്യ, true, &subdired_obstack, absolute_name);

      സ്വതന്ത്രം (absolute_name);

      dired_outstring (":\n");
    }

  /* Read the directory entries, and insert the subfiles into the 'cwd_file'
     table.  */

  ആകുമ്പോൾ (true)
    {
      /* Set errno to zero so we can distinguish between a readdir failure
         and when readdir simply finds that there are no more entries.  */
      errno = 0;
      next = readdir (dirp);
      എങ്കിൽ (next)
        {
          എങ്കിൽ (! file_ignored (next->d_name))
            {
              എണ്ണൽ filetype type;
#എങ്കിൽ HAVE_STRUCT_DIRENT_D_TYPE
              type = d_type_filetype[next->d_type];
#അല്ലെങ്കിൽ
              type = unknown;
#endif
              total_blocks += gobble_file (next->d_name, type,
                                           RELIABLE_D_INO (next),
                                           false, name);

              /* In this narrow case, print out each name right away, so
                 ls uses constant memory while processing the entries of
                 this directory.  Useful when there are many (millions)
                 of entries in a directory.  */
              എങ്കിൽ (format == one_per_വരി && sort_type == sort_none
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
      അല്ലെങ്കിൽ
        {
          പൂർണ്ണസംഖ്യ err = errno;
          എങ്കിൽ (err == 0)
            നിർത്തുക;
          /* Ignore errors indicating that the directory was removed.  */
          എങ്കിൽ (ignorable_traversal_errno (err))
            നിർത്തുക;
          file_failure (command_വരി_arg, _("reading directory %s"), name);
          എങ്കിൽ (err != EOVERFLOW)
            നിർത്തുക;
        }

      /* When processing a very large directory, and since we've inhibited
         interrupts, this loop would take so long that ls would be annoyingly
         uninterruptible.  This ensures that it handles signals promptly.  */
      process_signals ();
    }

  എങ്കിൽ (closedir (dirp) != 0)
    {
      file_failure (command_വരി_arg, _("closing directory %s"), name);
      /* Don't return; print whatever we got.  */
    }

  /* Sort the directory contents.  */
  sort_files ();

  /* If any member files are subdirectories, perhaps they should have their
     contents listed rather than being mentioned here as files.  */

  എങ്കിൽ (recursive)
    extract_dirs_from_files (name, false);

  എങ്കിൽ (format == long_format || print_block_size)
    {
      അക്ഷരം buf[LONGEST_HUMAN_READABLE + 3];
      അക്ഷരം *p = human_readable (total_blocks, buf + 1, human_output_opts,
                                ST_NBLOCKSIZE, output_block_size);
      അക്ഷരം *pend = p + strlen (p);
      *--p = ' ';
      *pend++ = eolbyte;
      dired_indent ();
      dired_outstring (_("total"));
      dired_outbuf (p, pend - p);
    }

  എങ്കിൽ (cwd_n_used)
    print_current_files ();
}

/* Add 'pattern' to the list of patterns for which files that match are
   not listed.  */

സ്ഥിതം ശൂന്യം
add_ignore_pattern (അക്ഷരം സ്ഥിരം *pattern)
{
  ഘടന ignore_pattern *ignore;

  ignore = xmalloc (വലുപ്പം *ignore);
  ignore->pattern = pattern;
  /* Add it to the head of the linked list.  */
  ignore->next = ignore_patterns;
  ignore_patterns = ignore;
}

/* Return true if one of the PATTERNS matches FILE.  */

സ്ഥിതം bool
patterns_match (ഘടന ignore_pattern സ്ഥിരം *patterns, അക്ഷരം സ്ഥിരം *file)
{
  വേണ്ടി (ഘടന ignore_pattern സ്ഥിരം *p = patterns; p; p = p->next)
    എങ്കിൽ (fnmatch (p->pattern, file, FNM_PERIOD) == 0)
      തിരികെ true;
  തിരികെ false;
}

/* Return true if FILE should be ignored.  */

സ്ഥിതം bool
file_ignored (അക്ഷരം സ്ഥിരം *name)
{
  തിരികെ ((ignore_mode != IGNORE_MINIMAL
           && name[0] == '.'
           && (ignore_mode == IGNORE_DEFAULT || ! name[1 + (name[1] == '.')]))
          || (ignore_mode == IGNORE_DEFAULT
              && patterns_match (hide_patterns, name))
          || patterns_match (ignore_patterns, name));
}

/* POSIX requires that a file size be printed without a sign, even
   when negative.  Assume the typical case where negative sizes are
   actually positive values that have wrapped around.  */

സ്ഥിതം uintmax_t
unsigned_file_size (off_t size)
{
  തിരികെ size + (size < 0) * ((uintmax_t) OFF_T_MAX - OFF_T_MIN + 1);
}

#ifdef HAVE_CAP
/* Return true if NAME has a capability (see linux/capability.h) */
സ്ഥിതം bool
has_capability (അക്ഷരം സ്ഥിരം *name)
{
  അക്ഷരം *result;
  bool has_cap;

  cap_t cap_d = cap_get_file (name);
  എങ്കിൽ (cap_d == ശൂന്യ)
    തിരികെ false;

  result = cap_to_text (cap_d, ശൂന്യ);
  cap_free (cap_d);
  എങ്കിൽ (!result)
    തിരികെ false;

  /* check if human-readable capability string is empty */
  has_cap = !!*result;

  cap_free (result);
  തിരികെ has_cap;
}
#അല്ലെങ്കിൽ
സ്ഥിതം bool
has_capability (MAYBE_UNUSED അക്ഷരം സ്ഥിരം *name)
{
  errno = ENOTSUP;
  തിരികെ false;
}
#endif

/* Enter and remove entries in the table 'cwd_file'.  */

സ്ഥിതം ശൂന്യം
free_ent (ഘടന fileinfo *f)
{
  സ്വതന്ത്രം (f->name);
  സ്വതന്ത്രം (f->linkname);
  സ്വതന്ത്രം (f->absolute_name);
  എങ്കിൽ (f->scontext != UNKNOWN_SECURITY_CONTEXT)
    aclinfo_scontext_free (f->scontext);
}

/* Empty the table of files.  */
സ്ഥിതം ശൂന്യം
clear_files (ശൂന്യം)
{
  വേണ്ടി (idx_t i = 0; i < cwd_n_used; i++)
    {
      ഘടന fileinfo *f = sorted_file[i];
      free_ent (f);
    }

  cwd_n_used = 0;
  cwd_some_quoted = false;
  any_has_acl = false;
  inode_സംഖ്യ_width = 0;
  block_size_width = 0;
  nlink_width = 0;
  owner_width = 0;
  group_width = 0;
  author_width = 0;
  scontext_width = 0;
  major_device_സംഖ്യ_width = 0;
  minor_device_സംഖ്യ_width = 0;
  file_size_width = 0;
}

/* Cache file_has_aclinfo failure, when it's trivial to do.
   Like file_has_aclinfo, but when F's st_dev says it's on a file
   system lacking ACL support, return 0 with ENOTSUP immediately.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
file_has_aclinfo_cache (അക്ഷരം സ്ഥിരം *file, ഘടന fileinfo *f,
                        ഘടന aclinfo *ai, പൂർണ്ണസംഖ്യ flags)
{
  /* If UNSUPPORTED_SCONTEXT, these variables hold the
     st_dev and associated info for the most recently processed device
     for which file_has_aclinfo failed indicating lack of support.  */
  സ്ഥിതം പൂർണ്ണസംഖ്യ unsupported_return;
  സ്ഥിതം അക്ഷരം *unsupported_scontext;
  സ്ഥിതം പൂർണ്ണസംഖ്യ unsupported_scontext_err;
  സ്ഥിതം dev_t unsupported_device;

  എങ്കിൽ (f->stat_ok && unsupported_scontext
      && f->stat.st_dev == unsupported_device)
    {
      ai->buf = ai->u._gl_acl_ch;
      ai->size = 0;
      ai->scontext = unsupported_scontext;
      ai->scontext_err = unsupported_scontext_err;
      errno = ENOTSUP;
      തിരികെ unsupported_return;
    }

  errno = 0;
  പൂർണ്ണസംഖ്യ n = file_has_aclinfo (file, ai, flags);
  പൂർണ്ണസംഖ്യ err = errno;
  എങ്കിൽ (!USE_ACL)
    n = 0;
  /* A security-context error does not imply that ACLs are unsupported.
     Require an unsupported attribute listing too if ACLs or capability
     colors are needed.  */
  എങ്കിൽ (f->stat_ok && n <= 0 && !acl_errno_valid (err)
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
  തിരികെ n;
}

/* Cache has_capability failure, when it's trivial to do.
   Like has_capability, but when F's st_dev says it's on a file
   system lacking capability support, return 0 with ENOTSUP immediately.  */
സ്ഥിതം bool
has_capability_cache (അക്ഷരം സ്ഥിരം *file, ഘടന fileinfo *f)
{
  /* If UNSUPPORTED_CACHED, UNSUPPORTED_DEVICE is the cached
     st_dev of the most recently processed device for which we've
     found that has_capability fails indicating lack of support.  */
  സ്ഥിതം bool unsupported_cached;
  സ്ഥിതം dev_t unsupported_device;

  എങ്കിൽ (f->stat_ok && unsupported_cached && f->stat.st_dev == unsupported_device)
    {
      errno = ENOTSUP;
      തിരികെ 0;
    }

  bool b = has_capability (file);
  എങ്കിൽ (f->stat_ok && !b && !acl_errno_valid (errno))
    {
      unsupported_cached = true;
      unsupported_device = f->stat.st_dev;
    }
  തിരികെ b;
}

/* Get the ACL, security context, and capability information needed for F.
   FULL_NAME names the file; DO_DEREF says whether to follow symlinks.  */
സ്ഥിതം ശൂന്യം
get_extended_info (അക്ഷരം സ്ഥിരം *full_name, ഘടന fileinfo *f, bool do_deref)
{
  /* FIXME: file_has_aclinfo() conflates capability and acl xattr lookup
     with security context attributes.  Ideally we'd have separate
     ACL_GET_{CAPABILITY,ACL} flags to lookup just the required items,
     simplifying the logic here.  */

  എണ്ണൽ filetype type = f->filetype;
  bool check_acl = USE_ACL && format == long_format;
  bool get_scontext = false;
#എങ്കിൽ HAVE_SMACK || USE_SELINUX_SELINUX_H
  get_scontext = (format == long_format) | print_scontext;
#endif
  bool check_capability = format_needs_capability & (type == normal);

  എങ്കിൽ (type != arg_directory && (check_acl | get_scontext | check_capability))
    {
      ഘടന aclinfo ai;
      /* Without ACL support, request contexts to obtain the attribute
         names needed for capability coloring too.  */
      പൂർണ്ണസംഖ്യ aclinfo_flags = ((do_deref ? ACL_SYMLINK_FOLLOW : 0)
                           | (get_scontext || (!USE_ACL && check_capability)
                              ? ACL_GET_SCONTEXT : 0)
                           | filetype_d_type[type]);
      പൂർണ്ണസംഖ്യ n = file_has_aclinfo_cache (full_name, f, &ai, aclinfo_flags);
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

      എങ്കിൽ (format == long_format && n < 0 && !cannot_access_acl)
        error (0, errno, "%s", quotef (full_name));
      അല്ലെങ്കിൽ
        {
          /* When requesting security context information, don't make
             ls fail just because the file (even a command line argument)
             isn't on the right type of file system.  I.e., a getfilecon
             failure isn't in the same class as a stat failure.  */
          എങ്കിൽ (print_scontext && ai.scontext_err
              && ! ignorable_traversal_errno (ai.scontext_err)
              && (! (is_ENOTSUP (ai.scontext_err)
                     || ai.scontext_err == ENODATA)))
            error (0, ai.scontext_err, "%s", quotef (full_name));
        }

      /* has_capability adds around 30% runtime to 'ls --color',
         so call it only if really needed.  Note capability coloring
         is disabled in the default color config.  */
      എങ്കിൽ (check_capability && aclinfo_has_xattr (&ai, XATTR_NAME_CAPS))
        f->has_capability = has_capability_cache (full_name, f);

      f->scontext = ai.scontext;
      ai.scontext = ശൂന്യ;
      aclinfo_free (&ai);
    }
}

സ്ഥിതം bool
needs_quoting (അക്ഷരം സ്ഥിരം *name)
{
  അക്ഷരം test[2];
  size_t len = quotearg_buffer (test, വലുപ്പം test , name, -1,
                                filename_quoting_options);
  തിരികെ *name != *test || strlen (name) != len;
}

/* Add a file to the current table of files.
   Verify that the file exists, and print an error message if it does not.
   Return the number of blocks that the file occupies.  */
സ്ഥിതം uintmax_t
gobble_file (അക്ഷരം സ്ഥിരം *name, എണ്ണൽ filetype type, ino_t inode,
             bool command_വരി_arg, അക്ഷരം സ്ഥിരം *dirname)
{
  uintmax_t blocks = 0;
  ഘടന fileinfo *f;

  /* An inode value prior to gobble_file necessarily came from readdir,
     which is not used for command line arguments.  */
  affirm (! command_വരി_arg || inode == NOT_AN_INODE_സംഖ്യ);

  എങ്കിൽ (cwd_n_used == cwd_n_alloc)
    cwd_file = xpalloc (cwd_file, &cwd_n_alloc, 1, -1, വലുപ്പം *cwd_file);

  f = &cwd_file[cwd_n_used];
  memset (f, '\0', വലുപ്പം *f);
  f->stat.st_ino = inode;
  f->filetype = type;
  f->scontext = UNKNOWN_SECURITY_CONTEXT;

  f->quoted = -1;
  എങ്കിൽ ((! cwd_some_quoted) && align_variable_outer_quotes)
    {
      /* Determine if any quoted for padding purposes.  */
      f->quoted = needs_quoting (name);
      എങ്കിൽ (f->quoted)
        cwd_some_quoted = 1;
    }

  bool check_stat =
     (command_വരി_arg
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
      || (print_inode && inode == NOT_AN_INODE_സംഖ്യ)
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
  അക്ഷരം സ്ഥിരം *full_name = name;
  എങ്കിൽ (check_stat | print_scontext | format_needs_capability
      && name[0] != '/' && dirname)
    {
      അക്ഷരം *p = alloca (strlen (name) + strlen (dirname) + 2);
      attach (p, dirname, name);
      full_name = p;
    }

  bool do_deref;

  എങ്കിൽ (!check_stat)
    do_deref = dereference == DEREF_ALWAYS;
  അല്ലെങ്കിൽ
    {
      പൂർണ്ണസംഖ്യ err;

      എങ്കിൽ (print_hyperlink)
        {
          f->absolute_name = canonicalize_filename_mode (full_name,
                                                         CAN_MISSING);
          എങ്കിൽ (! f->absolute_name
              && (command_വരി_arg
                  || ! ignorable_traversal_errno (errno)))
            file_failure (command_വരി_arg,
                          _("error canonicalizing %s"), full_name);
        }

      തിരഞ്ഞെടുപ്പ് (dereference)
        {
        അവസ്ഥ DEREF_ALWAYS:
          err = do_stat (full_name, &f->stat);
          do_deref = true;
          നിർത്തുക;

        അവസ്ഥ DEREF_COMMAND_വരി_ARGUMENTS:
        അവസ്ഥ DEREF_COMMAND_വരി_SYMLINK_TO_DIR:
          എങ്കിൽ (command_വരി_arg)
            {
              bool need_lstat;
              err = do_stat (full_name, &f->stat);
              do_deref = true;

              എങ്കിൽ (dereference == DEREF_COMMAND_വരി_ARGUMENTS)
                നിർത്തുക;

              need_lstat = (err < 0
                            ? (errno == ENOENT || errno == ELOOP)
                            : ! S_ISDIR (f->stat.st_mode));
              എങ്കിൽ (!need_lstat)
                നിർത്തുക;

              /* stat failed because of ENOENT || ELOOP, maybe indicating a
                 non-traversable symlink.  Or stat succeeded,
                 FULL_NAME does not refer to a directory,
                 and --dereference-command-line-symlink-to-dir is in effect.
                 Fall through so that we call lstat instead.  */
            }
          FALLTHROUGH;

        അവസ്ഥ DEREF_NEVER:
          err = do_lstat (full_name, &f->stat);
          do_deref = false;
          നിർത്തുക;

        അവസ്ഥ DEREF_UNDEFINED: സ്ഥിരസ്ഥിതി:
          affirm (false);
        }

      എങ്കിൽ (err != 0)
        {
          എങ്കിൽ (! command_വരി_arg && ignorable_traversal_errno (errno))
            {
              പൂർണ്ണസംഖ്യ stat_errno = errno;
              ഘടന stat linkstat;
              bool dangling_link = (do_deref
                                    && (type == symbolic_link
                                        || type == unknown)
                                    && do_lstat (full_name, &linkstat) == 0);
              errno = stat_errno;
              എങ്കിൽ (! dangling_link)
                {
                  free_ent (f);
                  തിരികെ 0;
                }
            }

          /* Failure to stat a command line argument leads to
             an exit status of 2.  For other files, stat failure
             provokes an exit status of 1.  */
          file_failure (command_വരി_arg,
                        _("cannot access %s"), full_name);

          എങ്കിൽ (command_വരി_arg)
            {
              free_ent (f);
              തിരികെ 0;
            }

          f->name = xstrdup (name);
          cwd_n_used++;

          തിരികെ 0;
        }

      f->stat_ok = true;
      f->filetype = type = d_type_filetype[IFTODT (f->stat.st_mode)];
    }

  എങ്കിൽ (type == directory && command_വരി_arg && !immediate_dirs)
    f->filetype = type = arg_directory;

  get_extended_info (full_name, f, do_deref);

  എങ്കിൽ ((type == symbolic_link)
      & ((format == long_format) | check_symlink_mode))
    {
      ഘടന stat linkstats;

      എങ്കിൽ (! get_link_name (full_name, f, command_വരി_arg))
        {
          free_ent (f);
          തിരികെ 0;
        }

      /* Use the slower quoting path for this entry, though
         don't update CWD_SOME_QUOTED since alignment not affected.  */
      എങ്കിൽ (f->linkname && f->quoted == 0 && needs_quoting (f->linkname))
        f->quoted = -1;

      /* Avoid following symbolic links when possible, i.e., when
         they won't be traced and when no indicator is needed.  */
      എങ്കിൽ (f->linkname
          && (file_type <= indicator_style || check_symlink_mode)
          && stat_for_mode (full_name, &linkstats) == 0)
        {
          f->linkok = true;
          f->linkmode = linkstats.st_mode;
        }
    }

  blocks = STP_NBLOCKS (&f->stat);
  എങ്കിൽ (format == long_format || print_block_size)
    {
      അക്ഷരം buf[LONGEST_HUMAN_READABLE + 1];
      പൂർണ്ണസംഖ്യ len = mbswidth (human_readable (blocks, buf, human_output_opts,
                                          ST_NBLOCKSIZE, output_block_size),
                          MBSWIDTH_FLAGS);
      എങ്കിൽ (block_size_width < len)
        block_size_width = len;
    }

  എങ്കിൽ (format == long_format)
    {
      എങ്കിൽ (print_owner)
        {
          പൂർണ്ണസംഖ്യ len = format_user_width (f->stat.st_uid);
          എങ്കിൽ (owner_width < len)
            owner_width = len;
        }

      എങ്കിൽ (print_group)
        {
          പൂർണ്ണസംഖ്യ len = format_group_width (f->stat.st_gid);
          എങ്കിൽ (group_width < len)
            group_width = len;
        }

      എങ്കിൽ (print_author)
        {
          പൂർണ്ണസംഖ്യ len = format_user_width (f->stat.st_author);
          എങ്കിൽ (author_width < len)
            author_width = len;
        }
    }

  എങ്കിൽ (print_scontext)
    {
      പൂർണ്ണസംഖ്യ len = strlen (f->scontext);
      എങ്കിൽ (scontext_width < len)
        scontext_width = len;
    }

  എങ്കിൽ (format == long_format)
    {
      അക്ഷരം b[INT_BUFSIZE_BOUND (uintmax_t)];
      പൂർണ്ണസംഖ്യ b_len = strlen (umaxtostr (f->stat.st_nlink, b));
      എങ്കിൽ (nlink_width < b_len)
        nlink_width = b_len;

      എങ്കിൽ ((type == chardev) | (type == blockdev))
        {
          അക്ഷരം buf[INT_BUFSIZE_BOUND (uintmax_t)];
          പൂർണ്ണസംഖ്യ len = strlen (umaxtostr (major (f->stat.st_rdev), buf));
          എങ്കിൽ (major_device_സംഖ്യ_width < len)
            major_device_സംഖ്യ_width = len;
          len = strlen (umaxtostr (minor (f->stat.st_rdev), buf));
          എങ്കിൽ (minor_device_സംഖ്യ_width < len)
            minor_device_സംഖ്യ_width = len;
          len = major_device_സംഖ്യ_width + 2 + minor_device_സംഖ്യ_width;
          എങ്കിൽ (file_size_width < len)
            file_size_width = len;
        }
      അല്ലെങ്കിൽ
        {
          അക്ഷരം buf[LONGEST_HUMAN_READABLE + 1];
          uintmax_t size = unsigned_file_size (f->stat.st_size);
          പൂർണ്ണസംഖ്യ len = mbswidth (human_readable (size, buf,
                                              file_human_output_opts,
                                              1, file_output_block_size),
                              MBSWIDTH_FLAGS);
          എങ്കിൽ (file_size_width < len)
            file_size_width = len;
        }
    }

  എങ്കിൽ (print_inode)
    {
      അക്ഷരം buf[INT_BUFSIZE_BOUND (uintmax_t)];
      പൂർണ്ണസംഖ്യ len = strlen (umaxtostr (f->stat.st_ino, buf));
      എങ്കിൽ (inode_സംഖ്യ_width < len)
        inode_സംഖ്യ_width = len;
    }

  f->name = xstrdup (name);
  cwd_n_used++;

  തിരികെ blocks;
}

/* Return true if F refers to a directory.  */
സ്ഥിതം bool
is_directory (സ്ഥിരം ഘടന fileinfo *f)
{
  തിരികെ f->filetype == directory || f->filetype == arg_directory;
}

/* Return true if F refers to a (symlinked) directory.  */
സ്ഥിതം bool
is_linked_directory (സ്ഥിരം ഘടന fileinfo *f)
{
  തിരികെ f->filetype == directory || f->filetype == arg_directory
         || S_ISDIR (f->linkmode);
}

/* Put the name of the file that FILENAME is a symbolic link to
   into the LINKNAME field of 'f'.  COMMAND_LINE_ARG indicates whether
   FILENAME is a command-line argument.  */

സ്ഥിതം bool
get_link_name (അക്ഷരം സ്ഥിരം *filename, ഘടന fileinfo *f, bool command_വരി_arg)
{
  f->linkname = areadlink_with_size (filename, f->stat.st_size);
  എങ്കിൽ (f->linkname == ശൂന്യ)
    {
      എങ്കിൽ (! command_വരി_arg
          && ignorable_traversal_errno (errno))
        തിരികെ false;
      file_failure (command_വരി_arg, _("cannot read symbolic link %s"),
                    filename);
    }
  തിരികെ true;
}

/* Return true if the last component of NAME is '.' or '..'
   This is so we don't try to recurse on '././././. ...' */

സ്ഥിതം bool
basename_is_dot_or_dotdot (അക്ഷരം സ്ഥിരം *name)
{
  അക്ഷരം സ്ഥിരം *base = last_component (name);
  തിരികെ dot_or_dotdot (base);
}

/* Remove any entries from CWD_FILE that are for directories,
   and queue them to be listed as directories instead.
   DIRNAME is the prefix to prepend to each dirname
   to make it correct relative to ls's working dir;
   if it is null, no prefix is needed and "." and ".." should not be ignored.
   If COMMAND_LINE_ARG is true, this directory was mentioned at the top level,
   This is desirable when processing directories recursively.  */

സ്ഥിതം ശൂന്യം
extract_dirs_from_files (അക്ഷരം സ്ഥിരം *dirname, bool command_വരി_arg)
{
  idx_t i, j;
  bool ignore_dot_and_dot_dot = (dirname != ശൂന്യ);

  എങ്കിൽ (dirname && LOOP_DETECT)
    {
      /* Insert a marker entry first.  When we dequeue this marker entry,
         we'll know that DIRNAME has been processed and may be removed
         from the set of active directories.  */
      queue_directory (ശൂന്യ, dirname, false);
    }

  /* Queue the directories last one first, because queueing reverses the
     order.  */
  വേണ്ടി (i = cwd_n_used; 0 < i; )
    {
      i--;
      ഘടന fileinfo *f = sorted_file[i];

      എങ്കിൽ (is_directory (f)
          && (! ignore_dot_and_dot_dot
              || ! basename_is_dot_or_dotdot (f->name)))
        {
          എങ്കിൽ (!dirname || f->name[0] == '/')
            queue_directory (f->name, f->linkname, command_വരി_arg);
          അല്ലെങ്കിൽ
            {
              അക്ഷരം *name = file_name_concat (dirname, f->name, ശൂന്യ);
              queue_directory (name, f->linkname, command_വരി_arg);
              സ്വതന്ത്രം (name);
            }
          എങ്കിൽ (f->filetype == arg_directory)
            free_ent (f);
        }
    }

  /* Now delete the directories from the table, compacting all the remaining
     entries.  */

  വേണ്ടി (i = 0, j = 0; i < cwd_n_used; i++)
    {
      ഘടന fileinfo *f = sorted_file[i];
      sorted_file[j] = f;
      j += (f->filetype != arg_directory);
    }
  cwd_n_used = j;
}

/* Use strcoll to compare strings in this locale.  If an error occurs,
   report an error and longjmp to failed_strcoll.  */

സ്ഥിതം jmp_buf failed_strcoll;

സ്ഥിതം പൂർണ്ണസംഖ്യ
xstrcoll (അക്ഷരം സ്ഥിരം *a, അക്ഷരം സ്ഥിരം *b)
{
  പൂർണ്ണസംഖ്യ diff;
  errno = 0;
  diff = strcoll (a, b);
  എങ്കിൽ (errno)
    {
      error (0, errno, _("cannot compare file names %s and %s"),
             quote_n (0, a), quote_n (1, b));
      set_exit_status (false);
      longjmp (failed_strcoll, 1);
    }
  തിരികെ diff;
}

/* Comparison routines for sorting the files.  */

തരനിർവചനം ശൂന്യം സ്ഥിരം *V;
തരനിർവചനം പൂർണ്ണസംഖ്യ (*qsortFunc)(V a, V b);

/* Used below in DEFINE_SORT_FUNCTIONS for _df_ sort function variants.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
dirfirst_check (ഘടന fileinfo സ്ഥിരം *a, ഘടന fileinfo സ്ഥിരം *b,
                പൂർണ്ണസംഖ്യ (*cmp) (V, V))
{
  പൂർണ്ണസംഖ്യ diff = is_linked_directory (b) - is_linked_directory (a);
  തിരികെ diff ? diff : cmp (a, b);
}

/* Define the 8 different sort function variants required for each sortkey.
   KEY_NAME is a token describing the sort key, e.g., ctime, atime, size.
   KEY_CMP_FUNC is a function to compare records based on that key, e.g.,
   ctime_cmp, atime_cmp, size_cmp.  Append KEY_NAME to the string,
   '[rev_][x]str{cmp|coll}[_df]_', to create each function name.  */
#define DEFINE_SORT_FUNCTIONS(താക്കോൽ_name, താക്കോൽ_cmp_func)			\
  /* direct, non-dirfirst versions */					\
  സ്ഥിതം പൂർണ്ണസംഖ്യ xstrcoll_##key_name (V a, V b)				\
  { തിരികെ താക്കോൽ_cmp_func (a, b, xstrcoll); }				\
  ATTRIBUTE_PURE സ്ഥിതം പൂർണ്ണസംഖ്യ strcmp_##key_name (V a, V b)		\
  { തിരികെ താക്കോൽ_cmp_func (a, b, strcmp); }				\
                                                                        \
  /* reverse, non-dirfirst versions */					\
  സ്ഥിതം പൂർണ്ണസംഖ്യ rev_xstrcoll_##key_name (V a, V b)				\
  { തിരികെ താക്കോൽ_cmp_func (b, a, xstrcoll); }				\
  ATTRIBUTE_PURE സ്ഥിതം പൂർണ്ണസംഖ്യ rev_strcmp_##key_name (V a, V b)	\
  { തിരികെ താക്കോൽ_cmp_func (b, a, strcmp); }				\
                                                                        \
  /* direct, dirfirst versions */					\
  സ്ഥിതം പൂർണ്ണസംഖ്യ xstrcoll_df_##key_name (V a, V b)				\
  { തിരികെ dirfirst_check (a, b, xstrcoll_##key_name); }		\
  ATTRIBUTE_PURE സ്ഥിതം പൂർണ്ണസംഖ്യ strcmp_df_##key_name (V a, V b)		\
  { തിരികെ dirfirst_check (a, b, strcmp_##key_name); }			\
                                                                        \
  /* reverse, dirfirst versions */					\
  സ്ഥിതം പൂർണ്ണസംഖ്യ rev_xstrcoll_df_##key_name (V a, V b)			\
  { തിരികെ dirfirst_check (a, b, rev_xstrcoll_##key_name); }		\
  ATTRIBUTE_PURE സ്ഥിതം പൂർണ്ണസംഖ്യ rev_strcmp_df_##key_name (V a, V b)	\
  { തിരികെ dirfirst_check (a, b, rev_strcmp_##key_name); }

സ്ഥിതം പൂർണ്ണസംഖ്യ
cmp_ctime (ഘടന fileinfo സ്ഥിരം *a, ഘടന fileinfo സ്ഥിരം *b,
           പൂർണ്ണസംഖ്യ (*cmp) (അക്ഷരം സ്ഥിരം *, അക്ഷരം സ്ഥിരം *))
{
  പൂർണ്ണസംഖ്യ diff = timespec_cmp (get_stat_ctime (&b->stat),
                           get_stat_ctime (&a->stat));
  തിരികെ diff ? diff : cmp (a->name, b->name);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
cmp_mtime (ഘടന fileinfo സ്ഥിരം *a, ഘടന fileinfo സ്ഥിരം *b,
           പൂർണ്ണസംഖ്യ (*cmp) (അക്ഷരം സ്ഥിരം *, അക്ഷരം സ്ഥിരം *))
{
  പൂർണ്ണസംഖ്യ diff = timespec_cmp (get_stat_mtime (&b->stat),
                           get_stat_mtime (&a->stat));
  തിരികെ diff ? diff : cmp (a->name, b->name);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
cmp_atime (ഘടന fileinfo സ്ഥിരം *a, ഘടന fileinfo സ്ഥിരം *b,
           പൂർണ്ണസംഖ്യ (*cmp) (അക്ഷരം സ്ഥിരം *, അക്ഷരം സ്ഥിരം *))
{
  പൂർണ്ണസംഖ്യ diff = timespec_cmp (get_stat_atime (&b->stat),
                           get_stat_atime (&a->stat));
  തിരികെ diff ? diff : cmp (a->name, b->name);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
cmp_btime (ഘടന fileinfo സ്ഥിരം *a, ഘടന fileinfo സ്ഥിരം *b,
           പൂർണ്ണസംഖ്യ (*cmp) (അക്ഷരം സ്ഥിരം *, അക്ഷരം സ്ഥിരം *))
{
  പൂർണ്ണസംഖ്യ diff = timespec_cmp (get_stat_btime (&b->stat),
                           get_stat_btime (&a->stat));
  തിരികെ diff ? diff : cmp (a->name, b->name);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
off_cmp (off_t a, off_t b)
{
  തിരികെ _GL_CMP (a, b);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
cmp_size (ഘടന fileinfo സ്ഥിരം *a, ഘടന fileinfo സ്ഥിരം *b,
          പൂർണ്ണസംഖ്യ (*cmp) (അക്ഷരം സ്ഥിരം *, അക്ഷരം സ്ഥിരം *))
{
  പൂർണ്ണസംഖ്യ diff = off_cmp (b->stat.st_size, a->stat.st_size);
  തിരികെ diff ? diff : cmp (a->name, b->name);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
cmp_name (ഘടന fileinfo സ്ഥിരം *a, ഘടന fileinfo സ്ഥിരം *b,
          പൂർണ്ണസംഖ്യ (*cmp) (അക്ഷരം സ്ഥിരം *, അക്ഷരം സ്ഥിരം *))
{
  തിരികെ cmp (a->name, b->name);
}

/* Compare file extensions.  Files with no extension are 'smallest'.
   If extensions are the same, compare by file names instead.  */

സ്ഥിതം പൂർണ്ണസംഖ്യ
cmp_extension (ഘടന fileinfo സ്ഥിരം *a, ഘടന fileinfo സ്ഥിരം *b,
               പൂർണ്ണസംഖ്യ (*cmp) (അക്ഷരം സ്ഥിരം *, അക്ഷരം സ്ഥിരം *))
{
  അക്ഷരം സ്ഥിരം *base1 = strrchr (a->name, '.');
  അക്ഷരം സ്ഥിരം *base2 = strrchr (b->name, '.');
  പൂർണ്ണസംഖ്യ diff = cmp (base1 ? base1 : "", base2 ? base2 : "");
  തിരികെ diff ? diff : cmp (a->name, b->name);
}

/* Return the (cached) screen width,
   for the NAME associated with the passed fileinfo F.  */

സ്ഥിതം size_t
fileinfo_name_width (ഘടന fileinfo സ്ഥിരം *f)
{
  തിരികെ f->width
         ? f->width
         : quote_name_width (f->name, filename_quoting_options, f->quoted);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
cmp_width (ഘടന fileinfo സ്ഥിരം *a, ഘടന fileinfo സ്ഥിരം *b,
          പൂർണ്ണസംഖ്യ (*cmp) (അക്ഷരം സ്ഥിരം *, അക്ഷരം സ്ഥിരം *))
{
  പൂർണ്ണസംഖ്യ diff = fileinfo_name_width (a) - fileinfo_name_width (b);
  തിരികെ diff ? diff : cmp (a->name, b->name);
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
സ്ഥിതം പൂർണ്ണസംഖ്യ
cmp_version (ഘടന fileinfo സ്ഥിരം *a, ഘടന fileinfo സ്ഥിരം *b)
{
  പൂർണ്ണസംഖ്യ diff = filevercmp (a->name, b->name);
  തിരികെ diff ? diff : strcmp (a->name, b->name);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
xstrcoll_version (V a, V b)
{
  തിരികെ cmp_version (a, b);
}
സ്ഥിതം പൂർണ്ണസംഖ്യ
rev_xstrcoll_version (V a, V b)
{
  തിരികെ cmp_version (b, a);
}
സ്ഥിതം പൂർണ്ണസംഖ്യ
xstrcoll_df_version (V a, V b)
{
  തിരികെ dirfirst_check (a, b, xstrcoll_version);
}
സ്ഥിതം പൂർണ്ണസംഖ്യ
rev_xstrcoll_df_version (V a, V b)
{
  തിരികെ dirfirst_check (a, b, rev_xstrcoll_version);
}


/* We have 2^3 different variants for each sort-key function
   (for 3 independent sort modes).
   The function pointers stored in this array must be dereferenced as:

    sort_variants[sort_key][use_strcmp][reverse][dirs_first]

   Note that the order in which sort keys are listed in the function pointer
   array below is defined by the order of the elements in the time_type and
   sort_type enums!  */

#define LIST_SORTFUNCTION_VARIANTS(താക്കോൽ_name)                        \
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

സ്ഥിതം qsortFunc സ്ഥിരം sort_functions[][2][2][2] =
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
        { ശൂന്യ, ശൂന്യ },
        { ശൂന്യ, ശൂന്യ },
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

സ്ഥിതം ശൂന്യം
initialize_ordering_vector (ശൂന്യം)
{
  വേണ്ടി (idx_t i = 0; i < cwd_n_used; i++)
    sorted_file[i] = &cwd_file[i];
}

/* Cache values based on attributes global to all files.  */

സ്ഥിതം ശൂന്യം
update_current_files_info (ശൂന്യം)
{
  /* Cache screen width of name, if needed multiple times.  */
  എങ്കിൽ (sort_type == sort_width
      || (വരി_length && (format == many_per_വരി || format == horizontal)))
    {
      വേണ്ടി (idx_t i = 0; i < cwd_n_used; i++)
        {
          ഘടന fileinfo *f = sorted_file[i];
          f->width = fileinfo_name_width (f);
        }
    }
}

/* Sort the files now in the table.  */

സ്ഥിതം ശൂന്യം
sort_files (ശൂന്യം)
{
  bool use_strcmp;

  എങ്കിൽ (sorted_file_alloc < cwd_n_used + (cwd_n_used >> 1))
    {
      സ്വതന്ത്രം (sorted_file);
      sorted_file = xinmalloc (cwd_n_used, 3 * വലുപ്പം *sorted_file);
      sorted_file_alloc = 3 * cwd_n_used;
    }

  initialize_ordering_vector ();

  update_current_files_info ();

  എങ്കിൽ (sort_type == sort_none)
    തിരികെ;

  /* Try strcoll.  If it fails, fall back on strcmp.  We can't safely
     ignore strcoll failures, as a failing strcoll might be a
     comparison function that is not a total order, and if we ignored
     the failure this might cause qsort to dump core.  */

  എങ്കിൽ (! setjmp (failed_strcoll))
    use_strcmp = false;      /* strcoll() succeeded */
  അല്ലെങ്കിൽ
    {
      use_strcmp = true;
      affirm (sort_type != sort_version);
      initialize_ordering_vector ();
    }

  /* When sort_type == sort_time, use time_type as subindex.  */
  mpsort ((ശൂന്യം സ്ഥിരം **) sorted_file, cwd_n_used,
          sort_functions[sort_type + (sort_type == sort_time ? time_type : 0)]
                        [use_strcmp][sort_reverse]
                        [directories_first]);
}

/* List all the files now in the table.  */

സ്ഥിതം ശൂന്യം
print_current_files (ശൂന്യം)
{
  തിരഞ്ഞെടുപ്പ് (format)
    {
    അവസ്ഥ one_per_വരി:
      വേണ്ടി (idx_t i = 0; i < cwd_n_used; i++)
        {
          print_file_name_and_frills (sorted_file[i], 0);
          putchar (eolbyte);
        }
      നിർത്തുക;

    അവസ്ഥ many_per_വരി:
      എങ്കിൽ (! വരി_length)
        print_with_separator (' ');
      അല്ലെങ്കിൽ
        print_many_per_വരി ();
      നിർത്തുക;

    അവസ്ഥ horizontal:
      എങ്കിൽ (! വരി_length)
        print_with_separator (' ');
      അല്ലെങ്കിൽ
        print_horizontal ();
      നിർത്തുക;

    അവസ്ഥ with_commas:
      print_with_separator (',');
      നിർത്തുക;

    അവസ്ഥ long_format:
      വേണ്ടി (idx_t i = 0; i < cwd_n_used; i++)
        {
          set_normal_color ();
          print_long_format (sorted_file[i]);
          dired_outbyte (eolbyte);
        }
      നിർത്തുക;
    }
}

/* Replace the first %b with precomputed aligned month names.
   Note on glibc-2.7 at least, this speeds up the whole 'ls -lU'
   process by around 17%, compared to letting strftime() handle the %b.  */

സ്ഥിതം ptrdiff_t
align_nstrftime (അക്ഷരം *buf, size_t size, bool recent, ഘടന tm സ്ഥിരം *tm,
                 timezone_t tz, പൂർണ്ണസംഖ്യ ns)
{
  അക്ഷരം സ്ഥിരം *nfmt = (use_abformat
                      ? abformat[recent][tm->tm_mon]
                      : long_time_format[recent]);
  തിരികെ nstrftime (buf, size, nfmt, tm, tz, ns);
}

/* Return the expected number of columns in a long-format timestamp,
   or zero if it cannot be calculated.  */

സ്ഥിതം പൂർണ്ണസംഖ്യ
long_time_expected_width (ശൂന്യം)
{
  സ്ഥിതം പൂർണ്ണസംഖ്യ width = -1;

  എങ്കിൽ (width < 0)
    {
      time_t epoch = 0;
      ഘടന tm tm;
      അക്ഷരം buf[TIME_STAMP_LEN_MAXIMUM + 1];

      /* In case you're wondering if localtime_rz can fail with an input time_t
         value of 0, let's just say it's very unlikely, but not inconceivable.
         The TZ environment variable would have to specify a time zone that
         is 2**31-1900 years or more ahead of UTC.  This could happen only on
         a 64-bit system that blindly accepts e.g., TZ=UTC+20000000000000.
         However, this is not possible with Solaris 10 or glibc-2.3.5, since
         their implementations limit the offset to 167:59 and 24:00, resp.  */
      എങ്കിൽ (localtime_rz (localtz, &epoch, &tm))
        {
          ptrdiff_t len = align_nstrftime (buf, വലുപ്പം buf, false,
                                           &tm, localtz, 0);
          എങ്കിൽ (len > 0)
            width = mbsnwidth (buf, len, MBSWIDTH_FLAGS);
        }

      എങ്കിൽ (width < 0)
        width = 0;
    }

  തിരികെ width;
}

/* Print the user or group name NAME, with numeric id ID, using a
   print width of WIDTH columns.  */

സ്ഥിതം ശൂന്യം
format_user_or_group (അക്ഷരം സ്ഥിരം *name, uintmax_t id, പൂർണ്ണസംഖ്യ width)
{
  എങ്കിൽ (name)
    {
      പൂർണ്ണസംഖ്യ name_width = mbswidth (name, MBSWIDTH_FLAGS);
      പൂർണ്ണസംഖ്യ width_gap = name_width < 0 ? 0 : width - name_width;
      പൂർണ്ണസംഖ്യ pad = MAX (0, width_gap);
      dired_outstring (name);

      ചെയ്യുക
        dired_outbyte (' ');
      ആകുമ്പോൾ (pad--);
    }
  അല്ലെങ്കിൽ
    dired_pos += printf ("%*ju ", width, id);
}

/* Print the name or id of the user with id U, using a print width of
   WIDTH.  */

സ്ഥിതം ശൂന്യം
format_user (uid_t u, പൂർണ്ണസംഖ്യ width, bool stat_ok)
{
  format_user_or_group (! stat_ok ? "?" :
                        (numeric_ids ? ശൂന്യ : getuser (u)), u, width);
}

/* Likewise, for groups.  */

സ്ഥിതം ശൂന്യം
format_group (gid_t g, പൂർണ്ണസംഖ്യ width, bool stat_ok)
{
  format_user_or_group (! stat_ok ? "?" :
                        (numeric_ids ? ശൂന്യ : getgroup (g)), g, width);
}

/* Return the number of columns that format_user_or_group will print,
   or -1 if unknown.  */

സ്ഥിതം പൂർണ്ണസംഖ്യ
format_user_or_group_width (അക്ഷരം സ്ഥിരം *name, uintmax_t id)
{
  തിരികെ (name
          ? mbswidth (name, MBSWIDTH_FLAGS)
          : snprintf (ശൂന്യ, 0, "%ju", id));
}

/* Return the number of columns that format_user will print,
   or -1 if unknown.  */

സ്ഥിതം പൂർണ്ണസംഖ്യ
format_user_width (uid_t u)
{
  തിരികെ format_user_or_group_width (numeric_ids ? ശൂന്യ : getuser (u), u);
}

/* Likewise, for groups.  */

സ്ഥിതം പൂർണ്ണസംഖ്യ
format_group_width (gid_t g)
{
  തിരികെ format_user_or_group_width (numeric_ids ? ശൂന്യ : getgroup (g), g);
}

/* Return a pointer to a formatted version of F->stat.st_ino,
   possibly using buffer, which must be at least
   INT_BUFSIZE_BOUND (uintmax_t) bytes.  */
സ്ഥിതം അക്ഷരം *
format_inode (അക്ഷരം buf[INT_BUFSIZE_BOUND (uintmax_t)],
              സ്ഥിരം ഘടന fileinfo *f)
{
  തിരികെ (f->stat_ok && f->stat.st_ino != NOT_AN_INODE_സംഖ്യ
          ? umaxtostr (f->stat.st_ino, buf)
          : (അക്ഷരം *) "?");
}

/* Print information about F in long format.  */
സ്ഥിതം ശൂന്യം
print_long_format (സ്ഥിരം ഘടന fileinfo *f)
{
  അക്ഷരം modebuf[12];
  അക്ഷരം buf
    [LONGEST_HUMAN_READABLE + 1		/* inode */
     + LONGEST_HUMAN_READABLE + 1	/* size in blocks */
     + വലുപ്പം (modebuf) - 1 + 1		/* mode string */
     + INT_BUFSIZE_BOUND (uintmax_t)	/* st_nlink */
     + LONGEST_HUMAN_READABLE + 2	/* major device number */
     + LONGEST_HUMAN_READABLE + 1	/* minor device number */
     + TIME_STAMP_LEN_MAXIMUM + 1	/* max length of time/date */
     ];
  ptrdiff_t s;
  അക്ഷരം *p;
  ഘടന timespec when_timespec;
  ഘടന tm when_local;
  bool btime_ok = true;

  /* Compute the mode string, except remove the trailing space if no
     file in this directory has an ACL or security context.  */
  എങ്കിൽ (f->stat_ok)
    filemodestring (&f->stat, modebuf);
  അല്ലെങ്കിൽ
    {
      modebuf[0] = filetype_letter[f->filetype];
      memset (modebuf + 1, '?', 10);
      modebuf[11] = '\0';
    }
  എങ്കിൽ (! any_has_acl)
    modebuf[10] = '\0';
  അല്ലെങ്കിൽ എങ്കിൽ (f->acl_type == ACL_T_LSM_CONTEXT_ONLY)
    modebuf[10] = '.';
  അല്ലെങ്കിൽ എങ്കിൽ (f->acl_type == ACL_T_YES)
    modebuf[10] = '+';
  അല്ലെങ്കിൽ എങ്കിൽ (f->acl_type == ACL_T_UNKNOWN)
    modebuf[10] = '?';

  തിരഞ്ഞെടുപ്പ് (time_type)
    {
    അവസ്ഥ time_ctime:
      when_timespec = get_stat_ctime (&f->stat);
      നിർത്തുക;
    അവസ്ഥ time_mtime:
      when_timespec = get_stat_mtime (&f->stat);
      നിർത്തുക;
    അവസ്ഥ time_atime:
      when_timespec = get_stat_atime (&f->stat);
      നിർത്തുക;
    അവസ്ഥ time_btime:
      when_timespec = get_stat_btime (&f->stat);
      എങ്കിൽ (when_timespec.tv_sec == -1 && when_timespec.tv_nsec == -1)
        btime_ok = false;
      നിർത്തുക;
    അവസ്ഥ time_numtypes: സ്ഥിരസ്ഥിതി:
      unreachable ();
    }

  p = buf;

  എങ്കിൽ (print_inode)
    {
      അക്ഷരം hbuf[INT_BUFSIZE_BOUND (uintmax_t)];
      p += sprintf (p, "%*s ", inode_സംഖ്യ_width, format_inode (hbuf, f));
    }

  എങ്കിൽ (print_block_size)
    {
      അക്ഷരം hbuf[LONGEST_HUMAN_READABLE + 1];
      അക്ഷരം സ്ഥിരം *blocks =
        (! f->stat_ok
         ? "?"
         : human_readable (STP_NBLOCKS (&f->stat), hbuf, human_output_opts,
                           ST_NBLOCKSIZE, output_block_size));
      പൂർണ്ണസംഖ്യ blocks_width = mbswidth (blocks, MBSWIDTH_FLAGS);
      വേണ്ടി (പൂർണ്ണസംഖ്യ pad = blocks_width < 0 ? 0 : block_size_width - blocks_width;
           0 < pad; pad--)
        *p++ = ' ';
      ആകുമ്പോൾ ((*p++ = *blocks++))
        തുടരുക;
      p[-1] = ' ';
    }

  /* The last byte of the mode string is the POSIX
     "optional alternate access method flag".  */
  {
    അക്ഷരം hbuf[INT_BUFSIZE_BOUND (uintmax_t)];
    p += sprintf (p, "%s %*s ", modebuf, nlink_width,
                  ! f->stat_ok ? "?" : umaxtostr (f->stat.st_nlink, hbuf));
  }

  dired_indent ();

  എങ്കിൽ (print_owner || print_group || print_author || print_scontext)
    {
      dired_outbuf (buf, p - buf);

      എങ്കിൽ (print_owner)
        format_user (f->stat.st_uid, owner_width, f->stat_ok);

      എങ്കിൽ (print_group)
        format_group (f->stat.st_gid, group_width, f->stat_ok);

      എങ്കിൽ (print_author)
        format_user (f->stat.st_author, author_width, f->stat_ok);

      എങ്കിൽ (print_scontext)
        format_user_or_group (f->scontext, 0, scontext_width);

      p = buf;
    }

  എങ്കിൽ (f->stat_ok
      && (S_ISCHR (f->stat.st_mode) || S_ISBLK (f->stat.st_mode)))
    {
      പൂർണ്ണസംഖ്യ blanks_width = (file_size_width
                          - (major_device_സംഖ്യ_width + 2
                             + minor_device_സംഖ്യ_width));
      p += sprintf (p, "%*ju, %*ju ",
                    major_device_സംഖ്യ_width + MAX (0, blanks_width),
                    (uintmax_t) major (f->stat.st_rdev),
                    minor_device_സംഖ്യ_width,
                    (uintmax_t) minor (f->stat.st_rdev));
    }
  അല്ലെങ്കിൽ
    {
      അക്ഷരം hbuf[LONGEST_HUMAN_READABLE + 1];
      അക്ഷരം സ്ഥിരം *size =
        (! f->stat_ok
         ? "?"
         : human_readable (unsigned_file_size (f->stat.st_size),
                           hbuf, file_human_output_opts, 1,
                           file_output_block_size));
      പൂർണ്ണസംഖ്യ size_width = mbswidth (size, MBSWIDTH_FLAGS);
      വേണ്ടി (പൂർണ്ണസംഖ്യ pad = size_width < 0 ? 0 : file_size_width - size_width;
           0 < pad; pad--)
        *p++ = ' ';
      ആകുമ്പോൾ ((*p++ = *size++))
        തുടരുക;
      p[-1] = ' ';
    }

  s = -1;

  എങ്കിൽ (f->stat_ok && btime_ok
      && localtime_rz (localtz, &when_timespec.tv_sec, &when_local))
    {
      ഘടന timespec six_months_ago;
      bool recent;

      /* If the file appears to be in the future, update the current
         time, in case the file happens to have been modified since
         the last time we checked the clock.  */
      എങ്കിൽ (timespec_cmp (current_time, when_timespec) < 0)
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

  എങ്കിൽ (0 <= s)
    {
      p += s;
      *p++ = ' ';
    }
  അല്ലെങ്കിൽ
    {
      /* The time cannot be converted using the desired format, so
         print it as a huge integer number of seconds.  */
      അക്ഷരം hbuf[INT_BUFSIZE_BOUND (intmax_t)];
      p += sprintf (p, "%*s ", long_time_expected_width (),
                    (! f->stat_ok || ! btime_ok
                     ? "?"
                     : timetostr (when_timespec.tv_sec, hbuf)));
      /* FIXME: (maybe) We discarded when_timespec.tv_nsec. */
    }

  dired_outbuf (buf, p - buf);
  size_t w = print_name_with_quoting (f, false, &dired_obstack, p - buf);

  എങ്കിൽ (f->filetype == symbolic_link)
    {
      എങ്കിൽ (f->linkname)
        {
          dired_outstring (" -> ");
          print_name_with_quoting (f, true, ശൂന്യ, (p - buf) + w + 4);
          എങ്കിൽ (indicator_style != none)
            print_type_indicator (true, f->linkmode, unknown);
        }
    }
  അല്ലെങ്കിൽ എങ്കിൽ (indicator_style != none)
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

സ്ഥിതം size_t
quote_name_buf (അക്ഷരം **inbuf, size_t bufsize, അക്ഷരം *name,
                ഘടന quoting_options സ്ഥിരം *options,
                പൂർണ്ണസംഖ്യ needs_general_quoting, size_t *width, bool *pad)
{
  അക്ഷരം *buf = *inbuf;
  size_t displayed_width IF_LINT ( = 0);
  size_t len = 0;
  bool quoted;

  എണ്ണൽ quoting_style qs = get_quoting_style (options);
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

  എങ്കിൽ (needs_general_quoting != 0)
    {
      len = quotearg_buffer (buf, bufsize, name, -1, options);
      എങ്കിൽ (bufsize <= len)
        {
          buf = xmalloc (len + 1);
          quotearg_buffer (buf, len + 1, name, -1, options);
        }

      quoted = (*name != *buf) || strlen (name) != len;
    }
  അല്ലെങ്കിൽ എങ്കിൽ (needs_further_quoting || extra_newline_protection)
    {
      len = strlen (name);
      എങ്കിൽ (bufsize <= len)
        buf = xmalloc (len + 1);
      memcpy (buf, name, len + 1);

      quoted = false;
    }
  അല്ലെങ്കിൽ
    {
      len = strlen (name);
      buf = name;
      quoted = false;
    }

  /* It's safe to replace newlines without multi-byte iteration, as newlines
     do not appear as part of any multi-byte encoded character.  */
  എങ്കിൽ (extra_newline_protection)
    വേണ്ടി (അക്ഷരം *p = buf; p < buf + len; p++)
      എങ്കിൽ (eolbyte && *p == '\n')
        *p = '?';

  എങ്കിൽ (needs_further_quoting)
    {
      എങ്കിൽ (MB_CUR_MAX > 1)
        {
          അക്ഷരം സ്ഥിരം *p = buf;
          അക്ഷരം സ്ഥിരം *plimit = buf + len;
          അക്ഷരം *q = buf;
          displayed_width = 0;

          ആകുമ്പോൾ (p < plimit)
            തിരഞ്ഞെടുപ്പ് (*p)
              {
                അവസ്ഥ ' ': അവസ്ഥ '!': അവസ്ഥ '"': അവസ്ഥ '#': അവസ്ഥ '%':
                അവസ്ഥ '&': അവസ്ഥ '\'': അവസ്ഥ '(': അവസ്ഥ ')': അവസ്ഥ '*':
                അവസ്ഥ '+': അവസ്ഥ ',': അവസ്ഥ '-': അവസ്ഥ '.': അവസ്ഥ '/':
                അവസ്ഥ '0': അവസ്ഥ '1': അവസ്ഥ '2': അവസ്ഥ '3': അവസ്ഥ '4':
                അവസ്ഥ '5': അവസ്ഥ '6': അവസ്ഥ '7': അവസ്ഥ '8': അവസ്ഥ '9':
                അവസ്ഥ ':': അവസ്ഥ ';': അവസ്ഥ '<': അവസ്ഥ '=': അവസ്ഥ '>':
                അവസ്ഥ '?':
                അവസ്ഥ 'A': അവസ്ഥ 'B': അവസ്ഥ 'C': അവസ്ഥ 'D': അവസ്ഥ 'E':
                അവസ്ഥ 'F': അവസ്ഥ 'G': അവസ്ഥ 'H': അവസ്ഥ 'I': അവസ്ഥ 'J':
                അവസ്ഥ 'K': അവസ്ഥ 'L': അവസ്ഥ 'M': അവസ്ഥ 'N': അവസ്ഥ 'O':
                അവസ്ഥ 'P': അവസ്ഥ 'Q': അവസ്ഥ 'R': അവസ്ഥ 'S': അവസ്ഥ 'T':
                അവസ്ഥ 'U': അവസ്ഥ 'V': അവസ്ഥ 'W': അവസ്ഥ 'X': അവസ്ഥ 'Y':
                അവസ്ഥ 'Z':
                അവസ്ഥ '[': അവസ്ഥ '\\': അവസ്ഥ ']': അവസ്ഥ '^': അവസ്ഥ '_':
                അവസ്ഥ 'a': അവസ്ഥ 'b': അവസ്ഥ 'c': അവസ്ഥ 'd': അവസ്ഥ 'e':
                അവസ്ഥ 'f': അവസ്ഥ 'g': അവസ്ഥ 'h': അവസ്ഥ 'i': അവസ്ഥ 'j':
                അവസ്ഥ 'k': അവസ്ഥ 'l': അവസ്ഥ 'm': അവസ്ഥ 'n': അവസ്ഥ 'o':
                അവസ്ഥ 'p': അവസ്ഥ 'q': അവസ്ഥ 'r': അവസ്ഥ 's': അവസ്ഥ 't':
                അവസ്ഥ 'u': അവസ്ഥ 'v': അവസ്ഥ 'w': അവസ്ഥ 'x': അവസ്ഥ 'y':
                അവസ്ഥ 'z': അവസ്ഥ '{': അവസ്ഥ '|': അവസ്ഥ '}': അവസ്ഥ '~':
                  /* These characters are printable ASCII characters.  */
                  *q++ = *p++;
                  displayed_width += 1;
                  നിർത്തുക;
                സ്ഥിരസ്ഥിതി:
                  /* If we have a multibyte sequence, copy it until we
                     reach its end, replacing each non-printable multibyte
                     character with a single question mark.  */
                  {
                    mbstate_t mbstate; mbszero (&mbstate);
                    ചെയ്യുക
                      {
                        char32_t wc;
                        size_t bytes;
                        പൂർണ്ണസംഖ്യ w;

                        bytes = mbrtoc32 (&wc, p, plimit - p, &mbstate);

                        എങ്കിൽ (bytes == (size_t) -1)
                          {
                            /* An invalid multibyte sequence was
                               encountered.  Skip one input byte, and
                               put a question mark.  */
                            p++;
                            *q++ = '?';
                            displayed_width += 1;
                            നിർത്തുക;
                          }

                        എങ്കിൽ (bytes == (size_t) -2)
                          {
                            /* An incomplete multibyte character
                               at the end.  Replace it entirely with
                               a question mark.  */
                            p = plimit;
                            *q++ = '?';
                            displayed_width += 1;
                            നിർത്തുക;
                          }

                        എങ്കിൽ (bytes == 0)
                          /* A null wide character was encountered.  */
                          bytes = 1;

                        w = c32width (wc);
                        എങ്കിൽ (w >= 0)
                          {
                            /* A printable multibyte character.
                               Keep it.  */
                            വേണ്ടി (; bytes > 0; --bytes)
                              *q++ = *p++;
                            displayed_width += w;
                          }
                        അല്ലെങ്കിൽ
                          {
                            /* An nonprintable multibyte character.
                               Replace it entirely with a question
                               mark.  */
                            p += bytes;
                            *q++ = '?';
                            displayed_width += 1;
                          }
                      }
                    ആകുമ്പോൾ (! mbsinit (&mbstate));
                  }
                  നിർത്തുക;
              }

          /* The buffer may have shrunk.  */
          len = q - buf;
        }
      അല്ലെങ്കിൽ
        {
          അക്ഷരം *p = buf;
          അക്ഷരം സ്ഥിരം *plimit = buf + len;

          ആകുമ്പോൾ (p < plimit)
            {
              എങ്കിൽ (! isprint (to_uchar (*p)))
                *p = '?';
              p++;
            }
          displayed_width = len;
        }
    }
  അല്ലെങ്കിൽ എങ്കിൽ (width != ശൂന്യ)
    {
      എങ്കിൽ (MB_CUR_MAX > 1)
        {
          displayed_width = mbsnwidth (buf, len, MBSWIDTH_FLAGS);
          displayed_width = MAX (0, displayed_width);
        }
      അല്ലെങ്കിൽ
        {
          അക്ഷരം സ്ഥിരം *p = buf;
          അക്ഷരം സ്ഥിരം *plimit = buf + len;

          displayed_width = 0;
          ആകുമ്പോൾ (p < plimit)
            {
              എങ്കിൽ (isprint (to_uchar (*p)))
                displayed_width++;
              p++;
            }
        }
    }

  /* Set padding to better align quoted items,
     and also give a visual indication that quotes are
     not actually part of the name.  */
  *pad = (align_variable_outer_quotes && cwd_some_quoted && ! quoted);

  എങ്കിൽ (width != ശൂന്യ)
    *width = displayed_width;

  *inbuf = buf;

  തിരികെ len;
}

സ്ഥിതം size_t
quote_name_width (അക്ഷരം സ്ഥിരം *name, ഘടന quoting_options സ്ഥിരം *options,
                  പൂർണ്ണസംഖ്യ needs_general_quoting)
{
  അക്ഷരം smallbuf[BUFSIZ];
  അക്ഷരം *buf = smallbuf;
  size_t width;
  bool pad;

  quote_name_buf (&buf, വലുപ്പം smallbuf, (അക്ഷരം *) name, options,
                  needs_general_quoting, &width, &pad);

  എങ്കിൽ (buf != smallbuf && buf != name)
    സ്വതന്ത്രം (buf);

  width += pad;

  തിരികെ width;
}

/* %XX escape any input out of range as defined in RFC3986,
   and also if PATH, convert all path separators to '/'.  */
സ്ഥിതം അക്ഷരം *
file_escape (അക്ഷരം സ്ഥിരം *str, bool path)
{
  അക്ഷരം *esc = xnmalloc (3, strlen (str) + 1);
  അക്ഷരം *p = esc;
  ആകുമ്പോൾ (*str)
    {
      എങ്കിൽ (path && ISSLASH (*str))
        {
          *p++ = '/';
          str++;
        }
      അല്ലെങ്കിൽ എങ്കിൽ (RFC3986[to_uchar (*str)])
        *p++ = *str++;
      അല്ലെങ്കിൽ
        p += sprintf (p, "%%%02x", to_uchar (*str++));
    }
  *p = '\0';
  തിരികെ esc;
}

സ്ഥിതം size_t
quote_name (അക്ഷരം സ്ഥിരം *name, ഘടന quoting_options സ്ഥിരം *options,
            പൂർണ്ണസംഖ്യ needs_general_quoting, സ്ഥിരം ഘടന bin_str *color,
            bool allow_pad, ഘടന obstack *അട്ടി, അക്ഷരം സ്ഥിരം *absolute_name)
{
  അക്ഷരം smallbuf[BUFSIZ];
  അക്ഷരം *buf = smallbuf;
  size_t len;
  bool pad;

  len = quote_name_buf (&buf, വലുപ്പം smallbuf, (അക്ഷരം *) name, options,
                        needs_general_quoting, ശൂന്യ, &pad);

  എങ്കിൽ (pad && allow_pad)
    dired_outbyte (' ');

  എങ്കിൽ (color)
    print_color_indicator (color);

  /* If we're padding, then don't include the outer quotes in
     the --hyperlink, to improve the alignment of those links.  */
  bool skip_quotes = false;

  എങ്കിൽ (absolute_name)
    {
      എങ്കിൽ (align_variable_outer_quotes && cwd_some_quoted && ! pad)
        {
          skip_quotes = true;
          putchar (*buf);
        }
      അക്ഷരം *h = file_escape (hostname, /* path= */ false);
      അക്ഷരം *n = file_escape (absolute_name, /* path= */ true);
      /* TODO: It would be good to be able to define parameters
         to give hints to the terminal as how best to render the URI.
         For example since ls is outputting a dense block of URIs
         it would be best to not underline by default, and only
         do so upon hover etc.  */
      printf ("\033]8;;file://%s%s%s\033\\", h, *n == '/' ? "" : "/", n);
      സ്വതന്ത്രം (h);
      സ്വതന്ത്രം (n);
    }

  എങ്കിൽ (അട്ടി)
    തള്ളുക_current_dired_pos (അട്ടി);

  fwrite (buf + skip_quotes, 1, len - (skip_quotes * 2), stdout);

  dired_pos += len;

  എങ്കിൽ (അട്ടി)
    തള്ളുക_current_dired_pos (അട്ടി);

  എങ്കിൽ (absolute_name)
    {
      fputs ("\033]8;;\033\\", stdout);
      എങ്കിൽ (skip_quotes)
        putchar (*(buf + len - 1));
    }

  എങ്കിൽ (buf != smallbuf && buf != name)
    സ്വതന്ത്രം (buf);

  തിരികെ len + pad;
}

സ്ഥിതം size_t
print_name_with_quoting (സ്ഥിരം ഘടന fileinfo *f,
                         bool symlink_target,
                         ഘടന obstack *അട്ടി,
                         size_t start_col)
{
  അക്ഷരം സ്ഥിരം *name = symlink_target ? f->linkname : f->name;

  സ്ഥിരം ഘടന bin_str *color
    = print_with_color ? get_color_indicator (f, symlink_target) : ശൂന്യ;

  bool used_color_this_time = (print_with_color
                               && (color || is_colored (C_NORM)));

  size_t len = quote_name (name, filename_quoting_options, f->quoted,
                           color, !symlink_target, അട്ടി, f->absolute_name);

  process_signals ();
  എങ്കിൽ (used_color_this_time)
    {
      prep_non_filename_text ();

      /* We use the byte length rather than display width here as
         an optimization to avoid accurately calculating the width,
         because we only output the clear to EOL sequence if the name
         _might_ wrap to the next line.  This may output a sequence
         unnecessarily in multi-byte locales for example,
         but in that case it's inconsequential to the output.  */
      എങ്കിൽ (വരി_length
          && (start_col / വരി_length != (start_col + len - 1) / വരി_length))
        put_indicator (&color_indicator[C_CLR_TO_EOL]);
    }

  തിരികെ len;
}

സ്ഥിതം ശൂന്യം
prep_non_filename_text (ശൂന്യം)
{
  എങ്കിൽ (color_indicator[C_END].string != ശൂന്യ)
    put_indicator (&color_indicator[C_END]);
  അല്ലെങ്കിൽ
    {
      put_indicator (&color_indicator[C_ഇടത്]);
      put_indicator (&color_indicator[C_RESET]);
      put_indicator (&color_indicator[C_വലത്]);
    }
}

/* Print the file name of 'f' with appropriate quoting.
   Also print file size, inode number, and filetype indicator character,
   as requested by switches.  */

സ്ഥിതം size_t
print_file_name_and_frills (സ്ഥിരം ഘടന fileinfo *f, size_t start_col)
{
  അക്ഷരം buf[MAX (LONGEST_HUMAN_READABLE + 1, INT_BUFSIZE_BOUND (uintmax_t))];

  set_normal_color ();

  എങ്കിൽ (print_inode)
    printf ("%*s ", format == with_commas ? 0 : inode_സംഖ്യ_width,
            format_inode (buf, f));

  എങ്കിൽ (print_block_size)
    {
      അക്ഷരം സ്ഥിരം *blocks =
        (! f->stat_ok
         ? "?"
         : human_readable (STP_NBLOCKS (&f->stat), buf, human_output_opts,
                           ST_NBLOCKSIZE, output_block_size));
      പൂർണ്ണസംഖ്യ blocks_width = mbswidth (blocks, MBSWIDTH_FLAGS);
      പൂർണ്ണസംഖ്യ pad = 0;
      എങ്കിൽ (0 <= blocks_width && block_size_width && format != with_commas)
        pad = block_size_width - blocks_width;
      printf ("%*s%s ", pad, "", blocks);
    }

  എങ്കിൽ (print_scontext)
    printf ("%*s ", format == with_commas ? 0 : scontext_width, f->scontext);

  size_t width = print_name_with_quoting (f, false, ശൂന്യ, start_col);

  എങ്കിൽ (indicator_style != none)
    width += print_type_indicator (f->stat_ok, f->stat.st_mode, f->filetype);

  തിരികെ width;
}

/* Given these arguments describing a file, return the single-byte
   type indicator, or 0.  */
സ്ഥിതം അക്ഷരം
get_type_indicator (bool stat_ok, mode_t mode, എണ്ണൽ filetype type)
{
  അക്ഷരം c;

  എങ്കിൽ (stat_ok ? S_ISREG (mode) : type == normal)
    {
      എങ്കിൽ (stat_ok && indicator_style == classify && (mode & S_IXUGO))
        c = '*';
      അല്ലെങ്കിൽ
        c = 0;
    }
  അല്ലെങ്കിൽ
    {
      എങ്കിൽ (stat_ok ? S_ISDIR (mode) : type == directory || type == arg_directory)
        c = '/';
      അല്ലെങ്കിൽ എങ്കിൽ (indicator_style == slash)
        c = 0;
      അല്ലെങ്കിൽ എങ്കിൽ (stat_ok ? S_ISLNK (mode) : type == symbolic_link)
        c = '@';
      അല്ലെങ്കിൽ എങ്കിൽ (stat_ok ? S_ISFIFO (mode) : type == fifo)
        c = '|';
      അല്ലെങ്കിൽ എങ്കിൽ (stat_ok ? S_ISSOCK (mode) : type == sock)
        c = '=';
      അല്ലെങ്കിൽ എങ്കിൽ (stat_ok && S_ISDOOR (mode))
        c = '>';
      അല്ലെങ്കിൽ
        c = 0;
    }
  തിരികെ c;
}

സ്ഥിതം bool
print_type_indicator (bool stat_ok, mode_t mode, എണ്ണൽ filetype type)
{
  അക്ഷരം c = get_type_indicator (stat_ok, mode, type);
  എങ്കിൽ (c)
    dired_outbyte (c);
  തിരികെ !!c;
}

/* Returns if color sequence was printed.  */
സ്ഥിതം bool
print_color_indicator (സ്ഥിരം ഘടന bin_str *ind)
{
  എങ്കിൽ (ind)
    {
      /* Need to reset so not dealing with attribute combinations */
      എങ്കിൽ (is_colored (C_NORM))
        restore_default_color ();
      put_indicator (&color_indicator[C_ഇടത്]);
      put_indicator (ind);
      put_indicator (&color_indicator[C_വലത്]);
    }

  തിരികെ ind != ശൂന്യ;
}

/* Returns color indicator or NULL if none.  */
ATTRIBUTE_PURE
സ്ഥിതം സ്ഥിരം ഘടന bin_str *
get_color_indicator (സ്ഥിരം ഘടന fileinfo *f, bool symlink_target)
{
  എണ്ണൽ indicator_no type;
  ഘടന color_ext_type *ext;	/* Color extension */
  size_t len;			/* Length of name */

  അക്ഷരം സ്ഥിരം *name;
  mode_t mode;
  പൂർണ്ണസംഖ്യ linkok;
  എങ്കിൽ (symlink_target)
    {
      name = f->linkname;
      mode = f->linkmode;
      linkok = f->linkok ? 0 : -1;
    }
  അല്ലെങ്കിൽ
    {
      name = f->name;
      mode = file_or_link_mode (f);
      linkok = f->linkok;
    }

  /* Is this a nonexistent file?  If so, linkok == -1.  */

  എങ്കിൽ (linkok == -1 && is_colored (C_MISSING))
    type = C_MISSING;
  അല്ലെങ്കിൽ എങ്കിൽ (!f->stat_ok)
    {
      സ്ഥിതം എണ്ണൽ indicator_no സ്ഥിരം filetype_indicator[] =
        {
          C_ORPHAN, C_FIFO, C_CHR, C_DIR, C_BLK, C_FILE,
          C_LINK, C_SOCK, C_FILE, C_DIR
        };
      static_assert (countof (filetype_indicator) == filetype_cardinality);
      type = filetype_indicator[f->filetype];
    }
  അല്ലെങ്കിൽ
    {
      എങ്കിൽ (S_ISREG (mode))
        {
          type = C_FILE;

          എങ്കിൽ ((mode & S_ISUID) != 0 && is_colored (C_SETUID))
            type = C_SETUID;
          അല്ലെങ്കിൽ എങ്കിൽ ((mode & S_ISGID) != 0 && is_colored (C_SETGID))
            type = C_SETGID;
          അല്ലെങ്കിൽ എങ്കിൽ (f->has_capability)
            type = C_CAP;
          അല്ലെങ്കിൽ എങ്കിൽ ((mode & S_IXUGO) != 0 && is_colored (C_EXEC))
            type = C_EXEC;
          അല്ലെങ്കിൽ എങ്കിൽ ((1 < f->stat.st_nlink) && is_colored (C_MULTIHARDLINK))
            type = C_MULTIHARDLINK;
        }
      അല്ലെങ്കിൽ എങ്കിൽ (S_ISDIR (mode))
        {
          type = C_DIR;

          എങ്കിൽ ((mode & S_ISVTX) && (mode & S_IWOTH)
              && is_colored (C_STICKY_OTHER_WRITABLE))
            type = C_STICKY_OTHER_WRITABLE;
          അല്ലെങ്കിൽ എങ്കിൽ ((mode & S_IWOTH) != 0 && is_colored (C_OTHER_WRITABLE))
            type = C_OTHER_WRITABLE;
          അല്ലെങ്കിൽ എങ്കിൽ ((mode & S_ISVTX) != 0 && is_colored (C_STICKY))
            type = C_STICKY;
        }
      അല്ലെങ്കിൽ എങ്കിൽ (S_ISLNK (mode))
        type = C_LINK;
      അല്ലെങ്കിൽ എങ്കിൽ (S_ISFIFO (mode))
        type = C_FIFO;
      അല്ലെങ്കിൽ എങ്കിൽ (S_ISSOCK (mode))
        type = C_SOCK;
      അല്ലെങ്കിൽ എങ്കിൽ (S_ISBLK (mode))
        type = C_BLK;
      അല്ലെങ്കിൽ എങ്കിൽ (S_ISCHR (mode))
        type = C_CHR;
      അല്ലെങ്കിൽ എങ്കിൽ (S_ISDOOR (mode))
        type = C_DOOR;
      അല്ലെങ്കിൽ
        {
          /* Classify a file of some other type as C_ORPHAN.  */
          type = C_ORPHAN;
        }
    }

  /* Check the file's suffix only if still classified as C_FILE.  */
  ext = ശൂന്യ;
  എങ്കിൽ (type == C_FILE)
    {
      /* Test if NAME has a recognized suffix.  */

      len = strlen (name);
      name += len;		/* Pointer to final \0.  */
      വേണ്ടി (ext = color_ext_list; ext != ശൂന്യ; ext = ext->next)
        {
          എങ്കിൽ (ext->ext.len <= len)
            {
              എങ്കിൽ (ext->exact_match)
                {
                  എങ്കിൽ (STREQ_LEN (name - ext->ext.len, ext->ext.string,
                                 ext->ext.len))
                    നിർത്തുക;
                }
              അല്ലെങ്കിൽ
                {
                  എങ്കിൽ (c_strncasecmp (name - ext->ext.len, ext->ext.string,
                                     ext->ext.len) == 0)
                    നിർത്തുക;
                }
            }
        }
    }

  /* Adjust the color for orphaned symlinks.  */
  എങ്കിൽ (type == C_LINK && !linkok)
    {
      എങ്കിൽ (color_symlink_as_referent || is_colored (C_ORPHAN))
        type = C_ORPHAN;
    }

  സ്ഥിരം ഘടന bin_str *സ്ഥിരം s
    = ext ? &(ext->seq) : &color_indicator[type];

  തിരികെ s->string ? s : ശൂന്യ;
}

/* Output a color indicator (which may contain nulls).
   Note we don't update dired_pos here as emacs can be configured
   to handle color codes by stripping/converting them.  */
സ്ഥിതം ശൂന്യം
put_indicator (സ്ഥിരം ഘടന bin_str *ind)
{
  എങ്കിൽ (! used_color)
    {
      used_color = true;

      /* If the standard output is a controlling terminal, watch out
         for signals, so that the colors can be restored to the
         default state if "ls" is suspended or interrupted.  */

      എങ്കിൽ (0 <= tcgetpgrp (STDOUT_FILENO))
        signal_init ();

      prep_non_filename_text ();
    }

  fwrite (ind->string, ind->len, 1, stdout);
}

സ്ഥിതം size_t
length_of_file_name_and_frills (സ്ഥിരം ഘടന fileinfo *f)
{
  size_t len = 0;
  അക്ഷരം buf[MAX (LONGEST_HUMAN_READABLE + 1, INT_BUFSIZE_BOUND (uintmax_t))];

  എങ്കിൽ (print_inode)
    len += 1 + (format == with_commas
                ? strlen (umaxtostr (f->stat.st_ino, buf))
                : inode_സംഖ്യ_width);

  എങ്കിൽ (print_block_size)
    len += 1 + (format == with_commas
                ? strlen (! f->stat_ok ? "?"
                          : human_readable (STP_NBLOCKS (&f->stat), buf,
                                            human_output_opts, ST_NBLOCKSIZE,
                                            output_block_size))
                : block_size_width);

  എങ്കിൽ (print_scontext)
    len += 1 + (format == with_commas ? strlen (f->scontext) : scontext_width);

  len += fileinfo_name_width (f);

  എങ്കിൽ (indicator_style != none)
    {
      അക്ഷരം c = get_type_indicator (f->stat_ok, f->stat.st_mode, f->filetype);
      len += (c != 0);
    }

  തിരികെ len;
}

സ്ഥിതം ശൂന്യം
print_many_per_വരി (ശൂന്യം)
{
  idx_t cols = calculate_columns (true);
  ഘടന column_info സ്ഥിരം *വരി_fmt = &column_info[cols - 1];

  /* Calculate the number of rows that will be in each column except possibly
     for a short column on the right.  */
  idx_t rows = cwd_n_used / cols + (cwd_n_used % cols != 0);

  വേണ്ടി (idx_t row = 0; row < rows; row++)
    {
      size_t col = 0;
      idx_t filesno = row;
      size_t pos = 0;

      /* Print the next row.  */
      ആകുമ്പോൾ (true)
        {
          ഘടന fileinfo സ്ഥിരം *f = sorted_file[filesno];
          size_t name_length = length_of_file_name_and_frills (f);
          size_t max_name_length = വരി_fmt->col_arr[col++];
          print_file_name_and_frills (f, pos);

          എങ്കിൽ (cwd_n_used - rows <= filesno)
            നിർത്തുക;
          filesno += rows;

          indent (pos + name_length, pos + max_name_length);
          pos += max_name_length;
        }
      putchar (eolbyte);
    }
}

സ്ഥിതം ശൂന്യം
print_horizontal (ശൂന്യം)
{
  size_t pos = 0;
  idx_t cols = calculate_columns (false);
  ഘടന column_info സ്ഥിരം *വരി_fmt = &column_info[cols - 1];
  ഘടന fileinfo സ്ഥിരം *f = sorted_file[0];
  size_t name_length = length_of_file_name_and_frills (f);
  size_t max_name_length = വരി_fmt->col_arr[0];

  /* Print first entry.  */
  print_file_name_and_frills (f, 0);

  /* Now the rest.  */
  വേണ്ടി (idx_t filesno = 1; filesno < cwd_n_used; filesno++)
    {
      idx_t col = filesno % cols;

      എങ്കിൽ (col == 0)
        {
          putchar (eolbyte);
          pos = 0;
        }
      അല്ലെങ്കിൽ
        {
          indent (pos + name_length, pos + max_name_length);
          pos += max_name_length;
        }

      f = sorted_file[filesno];
      print_file_name_and_frills (f, pos);

      name_length = length_of_file_name_and_frills (f);
      max_name_length = വരി_fmt->col_arr[col];
    }
  putchar (eolbyte);
}

/* Output name + SEP + ' '.  */

സ്ഥിതം ശൂന്യം
print_with_separator (അക്ഷരം sep)
{
  size_t pos = 0;

  വേണ്ടി (idx_t filesno = 0; filesno < cwd_n_used; filesno++)
    {
      ഘടന fileinfo സ്ഥിരം *f = sorted_file[filesno];
      size_t len = വരി_length ? length_of_file_name_and_frills (f) : 0;

      എങ്കിൽ (filesno != 0)
        {
          അക്ഷരം separator;
          size_t next_pos = 2 + (sep == ',' && filesno < cwd_n_used - 1);

          എങ്കിൽ (! വരി_length
              || (! ckd_add (&next_pos, next_pos, pos)
                  && ! ckd_add (&next_pos, next_pos, len)
                  && next_pos <= വരി_length))
            {
              pos += 2;
              separator = ' ';
            }
          അല്ലെങ്കിൽ
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

സ്ഥിതം ശൂന്യം
indent (size_t from, size_t to)
{
  ആകുമ്പോൾ (from < to)
    {
      എങ്കിൽ (tabsize != 0 && to / tabsize > (from + 1) / tabsize)
        {
          putchar ('\t');
          from += tabsize - from % tabsize;
        }
      അല്ലെങ്കിൽ
        {
          putchar (' ');
          from++;
        }
    }
}

/* Put DIRNAME/NAME into DEST, handling '.' and '/' properly.  */
/* FIXME: maybe remove this function someday.  See about using a
   non-malloc'ing version of file_name_concat.  */

സ്ഥിതം ശൂന്യം
attach (അക്ഷരം *dest, അക്ഷരം സ്ഥിരം *dirname, അക്ഷരം സ്ഥിരം *name)
{
  അക്ഷരം സ്ഥിരം *dirnamep = dirname;

  /* Copy dirname if it is not ".".  */
  എങ്കിൽ (dirname[0] != '.' || dirname[1] != 0)
    {
      ആകുമ്പോൾ (*dirnamep)
        *dest++ = *dirnamep++;
      /* Add '/' if 'dirname' doesn't already end with it.  */
      എങ്കിൽ (dirnamep > dirname && dirnamep[-1] != '/')
        *dest++ = '/';
    }
  ആകുമ്പോൾ (*name)
    *dest++ = *name++;
  *dest = 0;
}

/* Allocate enough column info suitable for the current number of
   files and display columns, and initialize the info to represent the
   narrowest possible columns.  */

സ്ഥിതം ശൂന്യം
init_column_info (idx_t max_cols)
{
  /* Currently allocated columns in column_info.  */
  സ്ഥിതം idx_t column_info_alloc;

  എങ്കിൽ (column_info_alloc < max_cols)
    {
      idx_t old_column_info_alloc = column_info_alloc;
      column_info = xpalloc (column_info, &column_info_alloc,
                             max_cols - column_info_alloc, -1,
                             വലുപ്പം *column_info);

      /* Allocate the new size_t objects by computing the triangle
         formula n * (n + 1) / 2, except that we don't need to
         allocate the part of the triangle that we've already
         allocated.  Check for address arithmetic overflow.  */
      idx_t column_info_growth = column_info_alloc - old_column_info_alloc, s;
      എങ്കിൽ (ckd_add (&s, old_column_info_alloc + 1, column_info_alloc)
          || ckd_mul (&s, s, column_info_growth))
        xalloc_die ();
      size_t *p = xinmalloc (s >> 1, വലുപ്പം *p);

      /* Grow the triangle by parceling out the cells just allocated.  */
      വേണ്ടി (idx_t i = old_column_info_alloc; i < column_info_alloc; i++)
        {
          column_info[i].col_arr = p;
          p += i + 1;
        }
    }

  വേണ്ടി (idx_t i = 0; i < max_cols; ++i)
    {
      column_info[i].valid_len = true;
      column_info[i].വരി_len = i * MIN_COLUMN_WIDTH + 1;
      വേണ്ടി (idx_t j = 0; j < i; ++j)
        column_info[i].col_arr[j] = MIN_COLUMN_WIDTH;
      column_info[i].col_arr[i] = 1;
    }
}

/* Calculate the number of columns needed to represent the current set
   of files in the current display width.  */

സ്ഥിതം idx_t
calculate_columns (bool by_columns)
{
  /* Normally the maximum number of columns is determined by the
     screen width.  But if few files are available this might limit it
     as well.  */
  idx_t max_cols = 0 < max_idx && max_idx < cwd_n_used ? max_idx : cwd_n_used;

  init_column_info (max_cols);

  /* Compute the maximum number of possible columns.  */
  വേണ്ടി (idx_t filesno = 0; filesno < cwd_n_used; ++filesno)
    {
      ഘടന fileinfo സ്ഥിരം *f = sorted_file[filesno];
      size_t name_length = length_of_file_name_and_frills (f);

      വേണ്ടി (idx_t i = 0; i < max_cols; ++i)
        {
          എങ്കിൽ (column_info[i].valid_len)
            {
              idx_t idx = (by_columns
                           ? filesno / ((cwd_n_used + i) / (i + 1))
                           : filesno % (i + 1));
              size_t real_length = name_length + (idx == i ? 0 : 2);

              എങ്കിൽ (column_info[i].col_arr[idx] < real_length)
                {
                  column_info[i].വരി_len += (real_length
                                              - column_info[i].col_arr[idx]);
                  column_info[i].col_arr[idx] = real_length;
                  column_info[i].valid_len = (column_info[i].വരി_len
                                              <= വരി_length);
                }
            }
        }
    }

  /* Find maximum allowed columns.  */
  idx_t cols;
  വേണ്ടി (cols = max_cols; 1 < cols; --cols)
    {
      എങ്കിൽ (column_info[cols - 1].valid_len)
        നിർത്തുക;
    }

  തിരികെ cols;
}

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
    {
      printf (_("Usage: %s [OPTION]... [FILE]...\n"), program_name);
      fputs (_("\
List information about the FILEs (the current directory by സ്ഥിരസ്ഥിതി).\n\
Sort entries alphabetically എങ്കിൽ none of -cftuvSUX nor --sort is specified.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -a, --all\n\
         ചെയ്യുക not ignore entries starting with .\n\
"));
      oputs (_("\
  -A, --almost-all\n\
         ചെയ്യുക not list implied . and ..\n\
"));
      oputs (_("\
      --author\n\
         with -l, print the author of each file\n\
"));
      oputs (_("\
  -b, --escape\n\
         print C-style escapes വേണ്ടി nongraphic characters\n\
"));
      oputs (_("\
      --block-size=SIZE\n\
         with -l, scale sizes by SIZE when printing them;\n\
         e.g., '--block-size=M'; see SIZE format below\n\
"));
      oputs (_("\
  -B, --ignore-backups\n\
         ചെയ്യുക not list implied entries ending with ~\n\
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
         generate output designed വേണ്ടി Emacs' dired mode\n\
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
         like -F, except ചെയ്യുക not append '*'\n\
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
         like -l, but ചെയ്യുക not list owner\n\
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
  -H, --dereference-command-വരി\n\
         follow symbolic links listed on the command വരി\n\
"));
      oputs (_("\
      --dereference-command-വരി-symlink-to-dir\n\
         follow each command വരി symbolic link that points to a directory\n\
"));
      oputs (_("\
      --hide=PATTERN\n\
         ചെയ്യുക not list implied entries matching shell PATTERN\n\
         (overridden by -a or -A)\n\
"));
      oputs (_("\
      --hyperlink[=WHEN]\n\
         hyperlink file names WHEN\n\
"));
      oputs (_("\
      --indicator-style=WORD\n\
         append indicator with style WORD to entry names:\n\
           none (സ്ഥിരസ്ഥിതി), slash (-p), file-type (--file-type), classify (-F)\n\
"));
      oputs (_("\
  -i, --inode\n\
         print the index സംഖ്യ of each file\n\
"));
      oputs (_("\
  -I, --ignore=PATTERN\n\
         ചെയ്യുക not list implied entries matching shell PATTERN\n\
"));
      oputs (_("\
  -k, --kibibytes\n\
         സ്ഥിരസ്ഥിതി to 1024-byte blocks വേണ്ടി file system usage;\n\
         used only with -s and per directory totals\n\
"));
      oputs (_("\
  -l\n\
         use a long listing format\n\
"));
      oputs (_("\
  -L, --dereference\n\
         when showing file information വേണ്ടി a symbolic link,\n\
         show information വേണ്ടി the file the link references\n\
         rather than വേണ്ടി the link itself\n\
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
         like -l, but ചെയ്യുക not list group information\n\
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
         the സ്ഥിരസ്ഥിതി, unless program is 'ls' and output is a terminal\n\
"));
      oputs (_("\
  -Q, --quote-name\n\
         enclose entry names in ഇരട്ട quotes\n\
"));
      oputs (_("\
      --quoting-style=WORD\n\
         use quoting style WORD വേണ്ടി entry names:\n\
           literal, locale, shell, shell-always,\n\
           shell-escape, shell-escape-always, c, escape\n\
         (overrides QUOTING_STYLE environment variable)\n\
"));
      oputs (_("\
  -r, --reverse\n\
         reverse order ആകുമ്പോൾ sorting\n\
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
         change സ്ഥിരസ്ഥിതി 'name' sort to WORD:\n\
           none (-U), size (-S), time (-t),\n\
           version (-v), extension (-X), name, width\n\
"));
      oputs (_("\
      --time=WORD\n\
         select which timestamp used to display or sort;\n\
           access time (-u): atime, access, use;\n\
           metadata change time (-c): ctime, status;\n\
           modified time (സ്ഥിരസ്ഥിതി): mtime, modification;\n\
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
         ചെയ്യുക not sort directory entries\n\
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
         list entries by വരികൾ instead of by columns\n\
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
         end each output വരി with NUL, not newline\n\
"));
      oputs (_("\
  -1\n\
         list one file per വരി\n\
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
Also the TIME_STYLE environment variable sets the സ്ഥിരസ്ഥിതി style to use.\n\
"), stdout);
      fputs (_("\
\n\
The WHEN argument defaults to 'always' and can also be 'auto' or 'never'.\n\
"), stdout);
      fputs (_("\
\n\
Using color to distinguish file types is disabled both by സ്ഥിരസ്ഥിതി and\n\
with --color=never.  With --color=auto, ls emits color codes only when\n\
standard output is connected to a terminal.  The LS_COLORS environment\n\
variable can change the settings.  Use the dircolors(1) command to set it.\n\
"), stdout);
      fputs (_("\
\n\
Exit status:\n\
 0  എങ്കിൽ OK,\n\
 1  എങ്കിൽ minor problems (e.g., cannot access subdirectory),\n\
 2  എങ്കിൽ serious trouble (e.g., cannot access command-വരി argument).\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}
